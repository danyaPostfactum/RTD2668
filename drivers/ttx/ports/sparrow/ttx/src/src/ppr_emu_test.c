/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ppr_emu_test.c
 * @brief    Prototypes of public application interface.
 * @details  This header declares the public access application interface for this TtLib library.
 * @author   Janus Cheng
 * @date     Dec. 11, 2007
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/

#include <ppr.h>            /* Provides: PPR_SOFTWARE_EMULATOR */
#include <ppr_hal.h>        /* Provides: PPR_FB_PKT1_RMK() */

#if PPR_SOFTWARE_EMULATOR

#include "ppr_emu.h"        /* Provides: functions to be implemented */
#include "ttx_cd.h"         /* Provides: TTX_CD_ChannelDecode() */

#include "ttx_pkg.h"

#include <stdlib.h>         /* Provides: NULL */
#include <string.h>         /* Provides: memset() */

#include <ttx.h>            /* Provides: TTX_DEFAULT_PAGEx_ADDRESS() */
#include <ttx_buff.h>       /* Provides: TTX_BUF_Open(), TTX_BUF_Close() and TTX_BUF_InsPage() */
#include <ttx_assert.h>
#include <ttx_log.h>

#include <osd.h>            /* Provides: OSD_SOFTWARE_EMULATOR */


TTX_DEFINE_THIS_MODULE( ppr_emu_test.c )


/* -- Prototypes of Private Functions ---------------------------------------*/
static void                 PPR_TestRegsReadWrite       (void);
static void                 PPR_TestPfeMode0Mag1_0      (void TTX_MEM* pPageBuf, const uint_least16_t bytes);
static void                 PPR_TestPfeMode0Mag1_1      (void TTX_MEM* pPageBuf, const uint_least16_t bytes);
static void                 PPR_TestPfeMode0Mag4_0      (void TTX_MEM* pPageBuf, const uint_least16_t bytes);
static void                 PPR_TestPfeMode1Mag4_0      (void TTX_MEM* pPageBuf, const uint_least16_t bytes);
static void                 PPR_TestPfeMode2Mag1_0      (void TTX_MEM* pPageBuf, const uint_least16_t bytes);
static void                 PPR_TestPfeMode3Mag2_0      (void TTX_MEM* pPageBuf, const uint_least16_t bytes);
static void                 PPR_FillFieldBufferFromPg   (const uint8_t pkts[][PPR_PTNGEN_PKT_LEN_BYTES], size_t numOfPkts);
static bool_t               PPR_GenerateOnePacket       (const uint8_t pkt[PPR_PTNGEN_PKT_LEN_BYTES]);
static bool_t               PPR_SetPgLineData           (const uint8_t ptn[PPR_PTNGEN_PKT_LEN_BYTES]);
static bool_t               PPR_PfeMoveMag              (TTX_STD_MagazineNum_t mag, const void TTX_MEM* pDest, PPR_PfeMode_t mode, bool_t bEnableOddParityChk);
static bool_t               PPR_PfeMoveM29              (const void TTX_MEM* pDest, bool_t bEnableOddParityChk);


static bool_t               PPR_ParseLineDataToFieldBuff(const uint8_t lineData[PPR_PTNGEN_PKT_LEN_BYTES]);

static bool_t               PPREMU_GetPktAddr           (TTX_STD_Packet_Address_t*  pPktAddr , const uint8_t* buf);
static bool_t               xGetMagNum                  (TTX_STD_MagazineNum_t*     pMagNum  , const uint8_t* buf);
static bool_t               xGetPktNum                  (TTX_STD_Packet_Num_t*      pPktNum  , const uint8_t* buf);

static bool_t               PPREMU_GetPageAddr          (TTX_STD_Page_AddrWithSubCode_t*         pPageAddr  , const uint8_t* buf);
static bool_t               xGetPageNum                 (TTX_STD_Page_Addr_Number_t*  pPageNum   , const uint8_t* buf);
static bool_t               xGetSubpageNum              (TTX_STD_Page_Addr_SubCode_t* pSubpageNum, const uint8_t* buf);

static bool_t               PPREMU_GetCtrlBits          (TTX_STD_Page_CtrlBits_t*        pCtrlBits  , const uint8_t* buf);

/* -- Implementations of Public Functions -----------------------------------*/


/*---------------------------------------------------------------------------*/
bool_t
PPR_Test (void)
/*****************************************************************************\
 *  @brief   The initialization routine.
 *  @details Use this function to initialize this library.
 *  @note    Must be called in advance of any other functions.
 *
 *  @return  the initialization status.
 *  @retval  TTX_TRUE  success
 *  @retval  TTX_FALSE failure
 *
 *  @ingroup ttx
 *  @see     TTX_Deinit()
\*****************************************************************************/
{
    /* Register Read/Write Test */
    PPR_TestRegsReadWrite();

    {
        /* Values of this pageAddr doesn't matter. The purpose of declaring a
         *   pageAddr here is to call TTX_BUF_InsPage() to obtain a page buffer pointer.
         */
        const TTX_STD_Page_AddrWithSubCode_t pageAddr =
        {
            1,              /* magNum  */
            {0, 0},         /* pageNum */
            {0, 0, 0, 1}    /* subCode */
        };

        void TTX_MEM*        pPageBuf = NULL;
        const uint_least16_t bytesLen = 26*40;
        TTX_BUF_Open();

        TTX_BUF_InsPage( &pageAddr, &pPageBuf, NULL, NULL );    /* obtain a page buffer pointer */
        TTX_REQUIRE( pPageBuf != NULL );
        if ( pPageBuf != NULL )
        {
            PPR_TestPfeMode0Mag1_0( pPageBuf, bytesLen );
            PPR_TestPfeMode0Mag1_1( pPageBuf, bytesLen );
            PPR_TestPfeMode0Mag4_0( pPageBuf, bytesLen );

            PPR_TestPfeMode1Mag4_0( pPageBuf, bytesLen );

            PPR_TestPfeMode2Mag1_0( pPageBuf, bytesLen );
            PPR_TestPfeMode3Mag2_0( pPageBuf, bytesLen );
        }

        TTX_BUF_Close();
    }

    return( TRUE );
}

/*---------------------------------------------------------------------------*/
void
PPR_PgEnable                (void)
/*****************************************************************************\
 *  @brief   Generate multiple or single TT packets (pkts[]) as lines to
 *             field buffer by pattern generator.
 *           This function simulates Slicer.
 *  @note    This is a blocking function.
\*****************************************************************************/
{
    PPREMU_Handle hPpr = &pprEmu;

    /* Because after this function, hPpr->m_pfe.m_addrLastPkt and
     *   hPpr->m_pfe.m_addrNextProc will be updated, so we have to
     *   reset all contents of field buffer to ensure correct data
     *   after calling this function.
     */
    {
        /* Reset content of field buffer */
        memset( hPpr->m_dacqu.m_fieldBuff, 0, PPR_FIELD_BUFF_PKT_NUM*PPR_FIELD_BUFF_PKT_SIZE );

        /* Reset packet info */
        memset( hPpr->m_dacqu.m_fieldBuffInfo, 0, PPR_FIELD_BUFF_PKT_NUM * sizeof(FB_PktInfo) );
    }

    hPpr->m_pfe.m_addrLastPkt  = 0;
    hPpr->m_pfe.m_addrNextProc = 0;

    hPpr->m_pg.m_bFromSlicer = FALSE;  // set data source to pattern generator
    hPpr->m_pg.m_bEnabled    = TRUE;
}

/*---------------------------------------------------------------------------*/
void
PPR_PgDisable                (void)
/*****************************************************************************\
 *  @brief   Generate multiple or single TT packets (pkts[]) as lines to
 *             field buffer by pattern generator.
 *           This function simulates Slicer.
 *  @note    This is a blocking function.
\*****************************************************************************/
{
    PPREMU_Handle hPpr = &pprEmu;

    hPpr->m_pg.m_bFromSlicer = TRUE;  // set data source to pattern generator
    hPpr->m_pg.m_bEnabled    = FALSE;
}

/*---------------------------------------------------------------------------*/
size_t
PPR_PgGenerateOneField (const uint8_t pkts[][PPR_PTNGEN_PKT_LEN_BYTES], size_t numOfPkts)
/*****************************************************************************\
 *  @brief   Generate multiple or single TT packets (pkts[]) as lines to
 *             field buffer by pattern generator.
 *           This function simulates Slicer.
 *  @note    This is a blocking function.
 *  @ret     number of packets generated
\*****************************************************************************/
{
    PPREMU_Handle hPpr = &pprEmu;

    size_t i;
    size_t pktsGenerated = 0;

    PPR_REQUIRE( numOfPkts <= PPR_FIELD_BUFF_PKT_NUM );

    // This line is active
    hPpr->m_pg.m_bLineActive = TRUE;

    for ( i=0; i<numOfPkts; i++ )
    {
        if ( PPR_GenerateOnePacket( pkts[i] ) )
            pktsGenerated++;
    }

    /* Dump debug info
    {
        PPR_Log( LOGGER_VERBOSE, "[PPR]" "=====================\n");
        PPR_Log( LOGGER_VERBOSE, "[PPR]" "LAST_PKT_ADDR == 0x%X\n", hPpr->m_pfe.m_addrLastPkt );
        PPR_Log( LOGGER_VERBOSE, "[PPR]" "PKT_X0_IND    == 0x%X\n", hPpr->m_pfe.m_pktX0Ind    );

        // Dump FB_INFO
        {
            FB_PktInfo* pPktInfo;
            for( i=0; i<PPR_FIELD_BUFF_PKT_NUM; i++ )
            {
                pPktInfo = &(hPpr->m_dacqu.m_fieldBuffInfo[i]);
                PPR_Log( LOGGER_VERBOSE, "[PPR]" "PKT%d  == 0x%X\n", i+1, PPR_FB_PKT1_RMK(pPktInfo->m_derr,
                                                                           pPktInfo->m_hcerr,
                                                                           pPktInfo->m_dcode,
                                                                           pPktInfo->m_pktNum,
                                                                           pPktInfo->m_magNum )
                         );
            }
        }
        PPR_Log( LOGGER_VERBOSE, "[PPR]" "=====================\n");
    } */


    hPpr->m_pg.m_bLineActive = FALSE;

    hPpr->m_dacqu.m_bFbValid = TRUE;

    return( pktsGenerated );
}



/* -- Implementations of Private Functions ----------------------------------*/

/*---------------------------------------------------------------------------*/
static void
PPR_TestRegsReadWrite   (void)
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
\*****************************************************************************/
{
    PPR_Log( LOGGER_VERBOSE, "[PPR]" "[Trace]" "Entering PPR_TestRegsReadWrite()... ");
    PPR_Log( LOGGER_VERBOSE, "[done]\n");
}


/*---------------------------------------------------------------------------*/
static void
PPR_TestPfeMode0Mag1_0        (void TTX_MEM* pPageBuf, const uint_least16_t bytesLen)
/*****************************************************************************\
 *  @brief   1. Test prefetch moving engine mode-0 and magazine-1.
 *           2. No odd-parity check.
 *  @note    This is a blocking function.
 *  @param   pPageBuf points to the pre-allocated page buffer with length bytesLen bytes.
 *  @param   bytesLen is the length in bytes of *pPageBuf can be written.
 *
 *  @pre     bytesLen must be no less than (26*40) bytes.
\*****************************************************************************/
{
    extern uint8_t const TTX_ROM PPR_testMag1WithError[18][47];
    extern uint8_t const TTX_ROM PPR_testMag1_0_out_nochk[26*40];
    bool_t bOk;
    const TTX_STD_MagazineNum_t mag   = 1;  // PPR_testMag1WithError[][] contains only magazine-1 packets
    const PPR_PfeMode_t     mode      = PPR_PFE_MODE0;
    const size_t            numOfPkts = 18;
    uint8_t TTX_MEM*  pDest = (      uint8_t TTX_MEM*)(pPageBuf);
    const uint8_t TTX_ROM*  pAns  = (const uint8_t TTX_ROM*)PPR_testMag1_0_out_nochk;
    const uint16_t          len   = 26*40;  // in bytes
    const bool_t            bEnableOddParityChk = FALSE;

    PPR_Log( LOGGER_VERBOSE, "[PPR]" "[Trace]" "Entering PPR_TestPfeMode0Mag1_0()... ");

    PPR_REQUIRE( ( 1 <= mag ) && ( mag <= 8 ) );    // ERROR: mag is not within valid range of TTX_STD_MagazineNum_t.
    PPR_REQUIRE( PPR_testMag1_0_out_nochk[40] == 0x13 );    // ERROR: Cannot read PPR_testMag1_0_out_nochk[]
    PPR_REQUIRE( pDest != NULL );
    PPR_REQUIRE( bytesLen >= len );
    memset( pDest, 0xFF, bytesLen ); // reset destination memory to default 0xFF

    // Generates PPR_testMag1WithError[] to field buffer
    PPR_FillFieldBufferFromPg( PPR_testMag1WithError, numOfPkts );

    // Wait for field buffer valid
    while ( pprEmu.m_dacqu.m_bFbValid  != PPR_DATA_ACQU_ST_FB_VALID_TRUE );

    // ERROR: Our software cannot process field buffer before next comming of field buffer.
    //        In other words, our software didn't meet real-time requirements.
    //        This will cause frame loss.
    TTX_ENSURE( pprEmu.m_dacqu.m_bFbFull != PPR_DATA_ACQU_ST_FB_FULL_TRUE );

    // Move from field buffer to pDest.
    PPR_REQUIRE( (TTX_Address_t)pDest % 8 == 0 );
    bOk = PPR_PfeMoveMag( mag, pDest, mode, bEnableOddParityChk );
    PPR_ENSURE( bOk == TRUE );

    // Verify results
    {
        PPR_REQUIRE( len == 26*40 );
        PPR_ENSURE( memcmp( pDest, pAns, len ) == 0 );

        PPR_ENSURE( PPREMU_GetPfeFldMode0Left() == FALSE );   // ERROR: mode-0 packet(s) left
        PPR_ENSURE( PPREMU_GetPfeFldMode1Left() == TRUE  );   // ERROR: mode-1 packet(s) doesn't left
        PPR_ENSURE( PPREMU_GetPfeFldMode2Left() == FALSE );   // ERROR: mode-2 packet(s) left
        PPR_ENSURE( PPREMU_GetPfeFldMode3Left() == FALSE );   // ERROR: mode-3 packet(s) left
    }

    PPR_Log( LOGGER_VERBOSE, "[done]\n");
}

/*---------------------------------------------------------------------------*/
static void
PPR_TestPfeMode0Mag1_1        (void TTX_MEM* pPageBuf, const uint_least16_t bytesLen)
/*****************************************************************************\
 *  @brief   1. Test prefetch moving engine mode-0 and magazine-1.
 *           2. With odd-parity check.
 *  @note    This is a blocking function.
 *  @param   pPageBuf points to the pre-allocated page buffer with length bytesLen bytes.
 *  @param   bytesLen is the length in bytes of *pPageBuf can be written.
 *
 *  @pre     bytesLen must be no less than (26*40) bytes.
\*****************************************************************************/
{
    extern uint8_t const TTX_ROM PPR_testMag1WithError[18][47];
    extern uint8_t const TTX_ROM PPR_testMag1_0_out_chk[26*40];
    bool_t bOk;

    const TTX_STD_MagazineNum_t mag   = 1;  // PPR_testMag1WithError[][] contains only magazine-1 packets
    const PPR_PfeMode_t     mode      = PPR_PFE_MODE0;
    const size_t            numOfPkts = 18;

    uint8_t TTX_MEM*  pDest = (      uint8_t TTX_MEM*)(pPageBuf);
    const uint8_t TTX_ROM*  pAns  = (const uint8_t TTX_ROM*)PPR_testMag1_0_out_chk;
    const uint16_t          len   = 26*40;  // in bytes
    const bool_t            bEnableOddParityChk = TRUE;

    PPR_Log( LOGGER_VERBOSE, "[PPR]" "[Trace]" "Entering PPR_TestPfeMode0Mag1_1()... ");

    PPR_REQUIRE( ( 1 <= mag ) && ( mag <= 8 ) );    // ERROR: mag is not within valid range of TTX_STD_MagazineNum_t.
    PPR_REQUIRE( PPR_testMag1_0_out_chk[40] == 0x13 );    // ERROR: Cannot read PPR_testMag1_0_out_chk[]
    PPR_REQUIRE( pDest != NULL );
    PPR_REQUIRE( bytesLen >= len );
    memset( pDest, 0xFF, len ); // reset destination memory to default 0xFF

    // Generates PPR_testMag1WithError[] to field buffer
    PPR_FillFieldBufferFromPg( PPR_testMag1WithError, numOfPkts );

    // Wait for field buffer valid
    while ( pprEmu.m_dacqu.m_bFbValid  != PPR_DATA_ACQU_ST_FB_VALID_TRUE );

    // ERROR: Our software cannot process field buffer before next comming of field buffer.
    //        In other words, our software didn't meet real-time requirements.
    //        This will cause frame loss.
    TTX_ENSURE( pprEmu.m_dacqu.m_bFbFull != PPR_DATA_ACQU_ST_FB_FULL_TRUE );

    // Move from field buffer to pDest.
    PPR_REQUIRE( (TTX_Address_t)pDest % 8 == 0 );
    bOk = PPR_PfeMoveMag( mag, pDest, mode, bEnableOddParityChk );
    PPR_ENSURE( bOk == TRUE );

    // Verify results
    {
        PPR_REQUIRE( len == 26*40 );
        PPR_ENSURE( memcmp( pDest, pAns, len ) == 0 );

        PPR_ENSURE( PPREMU_GetPfeFldMode0Left() == FALSE );   // ERROR: mode-0 packet(s) left
        PPR_ENSURE( PPREMU_GetPfeFldMode1Left() == TRUE  );   // ERROR: mode-1 packet(s) doesn't left
        PPR_ENSURE( PPREMU_GetPfeFldMode2Left() == FALSE );   // ERROR: mode-2 packet(s) left
        PPR_ENSURE( PPREMU_GetPfeFldMode3Left() == FALSE );   // ERROR: mode-3 packet(s) left
    }

    PPR_Log( LOGGER_VERBOSE, "[done]\n");
}

/*---------------------------------------------------------------------------*/
static void
PPR_TestPfeMode0Mag4_0        (void TTX_MEM* pPageBuf, const uint_least16_t bytesLen)
/*****************************************************************************\
 *  @brief   1. Test prefetch moving engine mode-0 and magazine-1.
 *           2. No odd-parity check.
 *  @note    This is a blocking function.
 *  @param   pPageBuf points to the pre-allocated page buffer with length bytesLen bytes.
 *  @param   bytesLen is the length in bytes of *pPageBuf can be written.
 *
 *  @pre     bytesLen must be no less than (26*40) bytes.
\*****************************************************************************/
{
    extern uint8_t const TTX_ROM PPR_testMag4_0[18][47];
    extern uint8_t const TTX_ROM PPR_testMag4_0_out[26*40];
    bool_t bOk;

    const TTX_STD_MagazineNum_t mag   = 4;  // PPR_testMag1WithError[][] contains only magazine-4 packets
    const PPR_PfeMode_t     mode      = PPR_PFE_MODE0;
    const size_t            numOfPkts = 18;
    uint8_t TTX_MEM*  pDest = (      uint8_t TTX_MEM*)(pPageBuf);
    const uint8_t TTX_ROM*  pAns  = (const uint8_t TTX_ROM*)PPR_testMag4_0_out;
    const uint16_t          len   = 26*40;  // in bytes
    const bool_t            bEnableOddParityChk = TRUE;

    PPR_Log( LOGGER_VERBOSE, "[PPR]" "[Trace]" "Entering PPR_TestPfeMode0Mag4_0()... ");

    PPR_REQUIRE( ( 1 <= mag ) && ( mag <= 8 ) );    // ERROR: mag is not within valid range of TTX_STD_MagazineNum_t.
    PPR_REQUIRE( PPR_testMag4_0_out[0] == 0x04 );    // ERROR: Cannot read PPR_testMag4_0_out[]
    PPR_REQUIRE( pDest != NULL );
    PPR_REQUIRE( bytesLen >= len );
    memset( pDest, 0xFF, len ); // reset destination memory to default 0xFF

    // Generates PPR_testMag4_0[] to field buffer
    PPR_FillFieldBufferFromPg( PPR_testMag4_0, numOfPkts );

    // Wait for field buffer valid
    while ( pprEmu.m_dacqu.m_bFbValid  != PPR_DATA_ACQU_ST_FB_VALID_TRUE );

    // ERROR: Our software cannot process field buffer before next comming of field buffer.
    //        In other words, our software didn't meet real-time requirements.
    //        This will cause frame loss.
    TTX_ENSURE( pprEmu.m_dacqu.m_bFbFull != PPR_DATA_ACQU_ST_FB_FULL_TRUE );

    // Move from field buffer to pDest.
    PPR_REQUIRE( (TTX_Address_t)pDest % 8 == 0 );
    bOk = PPR_PfeMoveMag( mag, pDest, mode, bEnableOddParityChk );
    PPR_ENSURE( bOk == TRUE );

    // Verify results
    {
        PPR_REQUIRE( len == 26*40 );
        PPR_ENSURE( memcmp( pDest, pAns, len ) == 0 );

        PPR_ENSURE( PPREMU_GetPfeFldMode0Left() == FALSE );   // ERROR: mode-0 packet(s) left
        PPR_ENSURE( PPREMU_GetPfeFldMode1Left() == TRUE  );   // ERROR: mode-1 packet(s) doesn't left
        PPR_ENSURE( PPREMU_GetPfeFldMode2Left() == FALSE );   // ERROR: mode-2 packet(s) left
        PPR_ENSURE( PPREMU_GetPfeFldMode3Left() == FALSE );   // ERROR: mode-3 packet(s) left
    }

    PPR_Log( LOGGER_VERBOSE, "[done]\n");
}

/*---------------------------------------------------------------------------*/
static void
PPR_TestPfeMode1Mag4_0        (void TTX_MEM* pPageBuf, const uint_least16_t bytesLen)
/*****************************************************************************\
 *  @brief   1. Test prefetch moving engine mode-0 and magazine-1.
 *           2. No odd-parity check.
 *  @note    This is a blocking function.
 *  @param   pPageBuf points to the pre-allocated page buffer with length bytesLen bytes.
 *  @param   bytesLen is the length in bytes of *pPageBuf can be written.
 *
 *  @pre     bytesLen must be no less than (26*40) bytes.
\*****************************************************************************/
{
    extern uint8_t const TTX_ROM PPR_testMag4_0[18][47];
    extern uint8_t const TTX_ROM PPR_testMag4_1_out[26*40];
    bool_t bOk;

    const TTX_STD_MagazineNum_t mag   = 4;  // PPR_testMag1WithError[][] contains only magazine-4 packets
    const PPR_PfeMode_t     mode      = PPR_PFE_MODE1;
    const size_t            numOfPkts = 18;
    uint8_t TTX_MEM*  pDest = (      uint8_t TTX_MEM*)(pPageBuf);
    const uint8_t TTX_ROM*  pAns  = (const uint8_t TTX_ROM*)PPR_testMag4_1_out;
    const uint16_t          len   = 26*40;  // in bytes
    const bool_t            bEnableOddParityChk = TRUE;

    PPR_Log( LOGGER_VERBOSE, "[PPR]" "[Trace]" "Entering PPR_TestPfeMode1Mag4_0()... ");

    PPR_REQUIRE( ( 1 <= mag ) && ( mag <= 8 ) );    // ERROR: mag is not within valid range of TTX_STD_MagazineNum_t.
    PPR_REQUIRE( PPR_testMag4_1_out[641] == 0x0B );    // ERROR: Cannot read PPR_testMag4_1_out[]
    PPR_REQUIRE( pDest != NULL );
    PPR_REQUIRE( bytesLen >= len );
    memset( pDest, 0xFF, len ); // reset destination memory to default 0xFF

    // Generates PPR_testMag4_0[] to field buffer
    PPR_FillFieldBufferFromPg( PPR_testMag4_0, numOfPkts );

    // Wait for field buffer valid
    while ( pprEmu.m_dacqu.m_bFbValid  != PPR_DATA_ACQU_ST_FB_VALID_TRUE );

    // ERROR: Our software cannot process field buffer before next comming of field buffer.
    //        In other words, our software didn't meet real-time requirements.
    //        This will cause frame loss.
    TTX_ENSURE( pprEmu.m_dacqu.m_bFbFull != PPR_DATA_ACQU_ST_FB_FULL_TRUE );

    // Move from field buffer to pDest.
    PPR_REQUIRE( (TTX_Address_t)pDest % 8 == 0 );
    bOk = PPR_PfeMoveMag( mag, pDest, mode, bEnableOddParityChk );
    PPR_ENSURE( bOk == TRUE );

    // Verify results
    {
        PPR_REQUIRE( len == 26*40 );
        PPR_ENSURE( memcmp( pDest, pAns, len ) == 0 );

        PPR_ENSURE( PPREMU_GetPfeFldMode0Left() == TRUE  );   // ERROR: mode-0 packet(s) doesn't left
        PPR_ENSURE( PPREMU_GetPfeFldMode1Left() == FALSE );   // ERROR: mode-1 packet(s) left
        PPR_ENSURE( PPREMU_GetPfeFldMode2Left() == FALSE );   // ERROR: mode-2 packet(s) left
        PPR_ENSURE( PPREMU_GetPfeFldMode3Left() == FALSE );   // ERROR: mode-3 packet(s) left
    }

    PPR_Log( LOGGER_VERBOSE, "[done]\n");
}


/*---------------------------------------------------------------------------*/
static void
PPR_TestPfeMode2Mag1_0        (void TTX_MEM* pPageBuf, const uint_least16_t bytesLen)
/*****************************************************************************\
 *  @brief   1. Test prefetch moving engine mode-0 and magazine-1.
 *           2. With odd-parity check.
 *  @note    This is a blocking function.
 *  @param   pPageBuf points to the pre-allocated page buffer with length bytesLen bytes.
 *  @param   bytesLen is the length in bytes of *pPageBuf can be written.
 *
 *  @pre     bytesLen must be no less than (26*40) bytes.
\*****************************************************************************/
{
    extern uint8_t const TTX_ROM PPR_testMag1_0[18][47];
    extern uint8_t const TTX_ROM PPR_testMag1_0_out[26*40];
    bool_t bOk;

    const TTX_STD_MagazineNum_t mag = 1;  // PPR_testMag1WithError[][] contains only magazine-1 packets
    const PPR_PfeMode_t     mode  = PPR_PFE_MODE2;
    const size_t            numOfPkts = 18;
    uint8_t TTX_MEM*  pDest = (      uint8_t TTX_MEM*)(pPageBuf);
    const uint8_t TTX_ROM*  pAns  = (const uint8_t TTX_ROM*)PPR_testMag1_0_out;
    const uint16_t          len   = 26*40;  // in bytes
    const bool_t            bEnableOddParityChk = TRUE;

    PPR_Log( LOGGER_VERBOSE, "[PPR]" "[Trace]" "Entering PPR_TestPfeMode2Mag1_0()... ");

    PPR_REQUIRE( ( 1 <= mag ) && ( mag <= 8 ) );    // ERROR: mag is not within valid range of TTX_STD_MagazineNum_t.
    PPR_REQUIRE( PPR_testMag1_0_out[2] == 0xB4 );    // ERROR: Cannot read PPR_testMag1_0_out[]
    PPR_REQUIRE( pDest != NULL );
    PPR_REQUIRE( bytesLen >= len );
    memset( pDest, 0xFF, len ); // reset destination memory to default 0xFF

    // Generates PPR_testMag1_0[] to field buffer
    PPR_FillFieldBufferFromPg( PPR_testMag1_0, numOfPkts );

    // Wait for field buffer valid
    while ( pprEmu.m_dacqu.m_bFbValid  != PPR_DATA_ACQU_ST_FB_VALID_TRUE );

    // ERROR: Our software cannot process field buffer before next comming of field buffer.
    //        In other words, our software didn't meet real-time requirements.
    //        This will cause frame loss.
    TTX_ENSURE( pprEmu.m_dacqu.m_bFbFull != PPR_DATA_ACQU_ST_FB_FULL_TRUE );

    // Move from field buffer to pDest.
    PPR_REQUIRE( (TTX_Address_t)pDest % 8 == 0 );
    bOk = PPR_PfeMoveMag( mag, pDest, mode, bEnableOddParityChk );
    PPR_ENSURE( bOk == TRUE );

    // Verify results
    {
        PPR_REQUIRE( len == 26*40 );
        PPR_ENSURE( memcmp( pDest, pAns, len ) == 0 );

        PPR_ENSURE( PPREMU_GetPfeFldMode0Left() == FALSE );   // ERROR: mode-0 packet(s) left
        PPR_ENSURE( PPREMU_GetPfeFldMode1Left() == FALSE );   // ERROR: mode-1 packet(s) left
        PPR_ENSURE( PPREMU_GetPfeFldMode2Left() == FALSE );   // ERROR: mode-2 packet(s) left
        PPR_ENSURE( PPREMU_GetPfeFldMode3Left() == FALSE );   // ERROR: mode-3 packet(s) left
    }

    PPR_Log( LOGGER_VERBOSE, "[done]\n");
}

/*---------------------------------------------------------------------------*/
static void
PPR_TestPfeMode3Mag2_0        (void TTX_MEM* pPageBuf, const uint_least16_t bytesLen)
/*****************************************************************************\
 *  @brief   1. Test prefetch moving engine mode-3 and magazine-2.
 *           2. No odd-parity check.
 *  @note    This is a blocking function.
 *  @param   pPageBuf points to the pre-allocated page buffer with length bytesLen bytes.
 *  @param   bytesLen is the length in bytes of *pPageBuf can be written.
 *
 *  @pre     bytesLen must be no less than (26*40) bytes.
\*****************************************************************************/
{
    extern uint8_t const TTX_ROM PPR_testMag2_0[18][47];
    extern uint8_t const TTX_ROM PPR_testMag2_0_out[26*40];
    bool_t bOk;

    const TTX_STD_MagazineNum_t mag   = 2;  // PPR_testMag2_0[][] contains only magazine-2 packets
    const size_t            numOfPkts = 18;
    uint8_t TTX_MEM*  pDest = (      uint8_t TTX_MEM*)(pPageBuf);
    const uint8_t TTX_ROM*  pAns  = (const uint8_t TTX_ROM*)PPR_testMag2_0_out;
    const uint16_t          len   = 26*40;  // in bytes
    const bool_t            bEnableOddParityChk = TRUE;

    PPR_Log( LOGGER_VERBOSE, "[PPR]" "[Trace]" "Entering PPR_TestPfeMode3Mag2_0()... ");

    PPR_REQUIRE( ( 1 <= mag ) && ( mag <= 8 ) );    // ERROR: mag is not within valid range of TTX_STD_MagazineNum_t.
    PPR_REQUIRE( PPR_testMag2_0_out[81] == 0x09 );    // ERROR: Cannot read PPR_testMag2_0_out[]
    PPR_REQUIRE( pDest != NULL );
    PPR_REQUIRE( bytesLen >= len );
    memset( pDest, 0xFF, len ); // reset destination memory to default 0xFF

    // Generates PPR_testMag2_0[] to field buffer
    PPR_FillFieldBufferFromPg( PPR_testMag2_0, numOfPkts );

    // Wait for field buffer valid
    while ( pprEmu.m_dacqu.m_bFbValid  != PPR_DATA_ACQU_ST_FB_VALID_TRUE );

    // ERROR: Our software cannot process field buffer before next comming of field buffer.
    //        In other words, our software didn't meet real-time requirements.
    //        This will cause frame loss.
    TTX_ENSURE( pprEmu.m_dacqu.m_bFbFull != PPR_DATA_ACQU_ST_FB_FULL_TRUE );

    // Move from field buffer to pDest.
    PPR_REQUIRE( (TTX_Address_t)pDest % 8 == 0 );
    bOk = PPR_PfeMoveM29( pDest, bEnableOddParityChk );
    PPR_ENSURE( bOk == TRUE );

    // Verify results
    {
        PPR_REQUIRE( len == 26*40 );
        PPR_ENSURE( memcmp( pDest, pAns, len ) == 0 );

        PPR_ENSURE( PPREMU_GetPfeFldMode0Left() == FALSE );   // ERROR: mode-0 packet(s) left
        PPR_ENSURE( PPREMU_GetPfeFldMode1Left() == FALSE );   // ERROR: mode-1 packet(s) left
        PPR_ENSURE( PPREMU_GetPfeFldMode2Left() == FALSE );   // ERROR: mode-2 packet(s) left
        PPR_ENSURE( PPREMU_GetPfeFldMode3Left() == FALSE );   // ERROR: mode-3 packet(s) left
    }

    PPR_Log( LOGGER_VERBOSE, "[done]\n");
}

/*---------------------------------------------------------------------------*/
static void
PPR_FillFieldBufferFromPg (const uint8_t pkts[][PPR_PTNGEN_PKT_LEN_BYTES], size_t numOfPkts)
/*****************************************************************************\
 *  @brief   Use pattern gen to send pkts[0][] to pkts[numOfPkts-1][] to field buffer.
 *  @pre     numOfPkts <= PPR_FIELD_BUFF_PKT_NUM
 *  @post    1. Data acquisition status will be modified.
 *  @post    2. Data acquisition from slicer will be disabled.
 *  @post    3. Pattern generator will be disabled.
\*****************************************************************************/
{
    PPREMU_Handle hPpr = &pprEmu;

    PPR_REQUIRE( numOfPkts <= PPR_FIELD_BUFF_PKT_NUM );

    // Limit numOfPkts to PPR_FIELD_BUFF_PKT_NUM
    if ( numOfPkts > PPR_FIELD_BUFF_PKT_NUM )
    {
        numOfPkts = PPR_FIELD_BUFF_PKT_NUM;
    }

    // Enable pattern gen
    PPR_PgEnable();

    // Clear data aquisition status
    hPpr->m_dacqu.m_bFbFull  = FALSE;
    hPpr->m_dacqu.m_bFbValid = FALSE;

    // Reset field-buffer indicators
    hPpr->m_pfe.m_addrLastPkt = 0;
    hPpr->m_pfe.m_pktX0Ind    = 0;


    // Accept data from Pattern Gen
    PPREMU_SetDacquEnable();

    // Generates pkts[0][] to pkts[numOfPkts-1][] to field buffer
    PPR_PgGenerateOneField( pkts, numOfPkts );

    // Do not accept data from Pattern Gen
    PPREMU_SetDacquDisable();

    // Disable pattern gen
    PPR_PgDisable();
}

/*---------------------------------------------------------------------------*/
static bool_t
PPR_GenerateOnePacket   (const uint8_t pkt[PPR_PTNGEN_PKT_LEN_BYTES])
/*****************************************************************************\
 *  @brief   Generate a TT packet (pkt[]) as a line to field buffer by pattern generator.
 *  @note    Do not use this auxiliary function. Use PPR_PgGenerateOneField() instead.
 *  @note    This is a blocking function.
\*****************************************************************************/
{
    PPREMU_Handle hPpr = &pprEmu;

//@todo  Model state machine
//    PPR_REQUIRE( hPpr->m_pg.m_bEnabled == FALSE );
//    if( hPpr->m_pg.m_bEnabled == FALSE )
    {
        // Record field info
        //const bool_t field = pkt[3] & 0x01;  // 0: top/odd/field1    1: bottom/even/field2
        _PPR_PG_MODE_TYPE mode;

        if ( PPR_SetPgLineData( pkt ) )
        {
            bool_t bGenerateOk = FALSE;

            // Set pattern generator's mode according to pkt[3].
            {
                // Teletext 625B standard
                switch ( (pkt[3]>>1)&0x7F )
                {
                default:
                case (0x22>>1):
                {
                    mode = PPR_PG_MODE_MODE_TT;
                    break;
                }
                case (0x00>>1):
                {
                    mode = PPR_PG_MODE_MODE_CC;
                    break;
                }
                case (0x24>>1):
                {
                    mode = PPR_PG_MODE_MODE_VPS;
                    break;
                }
                case (0x26>>1):
                {
                    mode = PPR_PG_MODE_MODE_WSS;
                    break;
                }
                }
                hPpr->m_pg.m_mode = mode;
            }


            //PPR_REQUIRE( mode == PPR_PG_MODE_MODE_TT ); // ERROR: In our verification, we shall encounter TT only.
            if ( mode == PPR_PG_MODE_MODE_TT )
            {
                // Start sending pattern
                hPpr->m_pg.m_bEnabled = PPR_PG_MODE_PTNGEN_BUSY;

                // Currently, our code can handle TT only
                if ( hPpr->m_pg.m_mode == PPR_PG_MODE_MODE_TT )
                {
                    bGenerateOk = PPR_ParseLineDataToFieldBuff( hPpr->m_dacqu.m_vbiData );
                }

                hPpr->m_pg.m_bEnabled = PPR_PG_MODE_PTNGEN_READY;

                PPR_ENSURE( hPpr->m_pg.m_bEnabled == PPR_PG_MODE_PTNGEN_READY );

                return( bGenerateOk );
            }

            return( FALSE );    /* FAIL: unsupported mode */
        }
        else
        {
            PPR_Log(LOGGER_ERROR, "[ERROR] %s (%d): Cannot send test pattern data!\n", __FILE__, __LINE__);
        }
    }   // end of if( PPR_FGET( PG_MODE, PTNGEN ) == PPR_PG_MODE_PTNGEN_READY )

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
static bool_t
PPR_SetPgLineData   (const uint8_t ptn[PPR_PTNGEN_PKT_LEN_BYTES])
/*****************************************************************************\
 *  @brief   Set line data.
\*****************************************************************************/
{
    PPREMU_Handle hPpr = &pprEmu;

    uint8_t i;
    PPR_REQUIRE( ptn != NULL );
    if ( ptn != NULL )
    {
        for ( i = 0; i<PPR_PTNGEN_PKT_LEN_BYTES ; i++ )
        {
            hPpr->m_dacqu.m_vbiData[i] = ptn[i];
        }

        return( TRUE );
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
static bool_t
PPR_PfeMoveMag   (TTX_STD_MagazineNum_t mag, const void TTX_MEM* pDest, PPR_PfeMode_t mode, bool_t bEnableOddParityChk)
/*****************************************************************************\
 *  @brief   Start prefetch moving engine to move magazine mag to pDest with mode.
 *  @param   bEnableOddParityChk    enable odd parity check for TT packet in
 *                                  X/1~X/25: 0~39 bytes
 *
 *  @return  the initialization status.
 *  @retval  TRUE  success
 *  @retval  FALSE failure
 *
 *  @ingroup ttx
 *  @see     TTX_Deinit()
\*****************************************************************************/
{
    PPREMU_Handle hPpr = &pprEmu;

    bool_t bOk;

    PPR_REQUIRE( (TTX_Address_t)pDest % 8 == 0 );
    PPR_REQUIRE( mode != PPR_PFE_MODE3 );  // mode-3 is M/29 which is independent of magazines and shouldn't call PPR_PfeActivateMag().
    bOk = PPR_PfeSetDestMag( mag, pDest ); // PFE will move packets for mag to pDest.
    PPR_ENSURE( bOk == TRUE );

    /* Despite we shouldn't not invoke this block here,
     *   the patterns used by functions: PPR_TestPfeMode0Mag1_0(),
     *   PPR_TestPfeMode0Mag1_1(), PPR_TestPfeMode0Mag4_0(),
     *   PPR_TestPfeMode1Mag4_0() and PPR_TestPfeMode2Mag1_0() are
     *   generated by using configuration of this block.
     *
     * @todo Remove this block and update the patterns of above functions.
     */
    {
        // Since PFE can be blocked by PKT_X0_IND, we
        //   reset it first.
        hPpr->m_pfe.m_pktX0Ind    = 0;

        // Reset prefetch start address of field buffer
        hPpr->m_pfe.m_addrNextProc = 0;
    }

    bOk |= PPR_PfeActivateMag( mag );  // Activate mag
    PPR_ENSURE( bOk == TRUE );

    // Start moving
    bOk |= PPR_PfeMove( mode, bEnableOddParityChk );
    PPR_ENSURE( bOk == TRUE );

    bOk |= PPR_PfeDeactivateMag( mag );  // Deactivate mag
    PPR_ENSURE( bOk == TRUE );

    return( bOk );
}

/*---------------------------------------------------------------------------*/
static bool_t
PPR_PfeMoveM29   (const void TTX_MEM* pDest, bool_t bEnableOddParityChk)
/*****************************************************************************\
 *  @brief   Start prefetch moving engine for M/29 packets.
 *  @param   bEnableOddParityChk    enable odd parity check for TT packet in
 *                                  X/1~X/25: 0~39 bytes
 *
 *  @return  the initialization status.
 *  @retval  TRUE  success
 *  @retval  FALSE failure
 *
 *  @ingroup ttx
 *  @see     TTX_Deinit()
\*****************************************************************************/
{
    const PPR_PfeMode_t mode = PPR_PFE_MODE3;
    bool_t bOk;

    PPR_REQUIRE( (TTX_Address_t)pDest % 8 == 0 );
    bOk = PPR_PfeSetDestM29( pDest );
    PPR_ENSURE( bOk == TRUE );

    /* Despite we shouldn't not invoke this block here,
     *   the pattern used by function: PPR_TestPfeMode3Mag2_0(),
     *   is generated by using configuration of this block.
     *
     * @todo Remove this block and update the patterns of above functions.
     */
    {
        // Reset prefetch start address of field buffer
        pprEmu.m_pfe.m_addrNextProc = 0;
    }

    bOk |= PPR_PfeActivateM29();
    PPR_ENSURE( bOk == TRUE );

    // Start moving
    bOk |= PPR_PfeMove( mode, bEnableOddParityChk );
    PPR_ENSURE( bOk == TRUE );

    bOk |= PPR_PfeDeactivateM29();
    PPR_ENSURE( bOk == TRUE );

    return( bOk );
}

/*---------------------------------------------------------------------------*/
static bool_t
PPR_ParseLineDataToFieldBuff    (const uint8_t lineData[PPR_PTNGEN_PKT_LEN_BYTES])
/*****************************************************************************\
 *  @brief   Parse line data to field buffer.
 *  @param   lineData[]  the line data to be parsed and placed into field buffer.
 *  @post    pprEmu will be updated.
\*****************************************************************************/
{
    PPREMU_Handle hPpr = &pprEmu;

    uint8_t     idxPkt;   // Valid range is 0~17, current packet index of field buffer to be updated
    uint8_t*    pFieldBuff;
    FB_PktInfo* pFbPktInfo;

    if ( hPpr->m_pfe.m_addrLastPkt < (PPR_FIELD_BUFF_PKT_NUM*40)/4 ) // there's at least one packet space available
    {
        PPR_REQUIRE( (UINT32_C(4)*hPpr->m_pfe.m_addrLastPkt) % 40 == 0 );

        idxPkt = (UINT32_C(4)*hPpr->m_pfe.m_addrLastPkt) / 40;
    }
    else
    {
        hPpr->m_dacqu.m_bFbFull = TRUE;

        PPR_Log(LOGGER_ERROR, "[PPR] Field-Buff Full\n");
        return( FALSE );
    }

    PPR_REQUIRE( idxPkt < PPR_FIELD_BUFF_PKT_NUM ); // ERROR: Invalid packet index.
    if ( idxPkt < PPR_FIELD_BUFF_PKT_NUM )
    {
        pFieldBuff = hPpr->m_dacqu.m_fieldBuff[idxPkt];
        pFbPktInfo = &( hPpr->m_dacqu.m_fieldBuffInfo[idxPkt] );
    }
    else
    {
        PPR_Log(LOGGER_ERROR, "[PPR] Invalid packet index\n");
        return( FALSE );
    }

    // Check if starting bytes are valid VBI data
    if ( (lineData[0] == 0x00) && (lineData[1] == 0xFF) && (lineData[2] == 0xFF) )
    {
        //const bool_t field = lineData[3] & 0x01;  // 0: top/odd/field1    1: bottom/even/field2

        // Teletext 625B standard: Check if it's Teletext data
        if ( ((lineData[3]>>1)&0x7F) == (uint8_t)(0x22>>1) )
        {
            const uint8_t numOfBytes = lineData[4];
            if ( numOfBytes == 42 )
            {
                TTX_STD_Packet_Address_t pktAddr;
                const uint8_t* const pPkt = &(lineData[5]);  // points to starting of 42-byte packet

                if ( PPREMU_GetPktAddr( &pktAddr, &(pPkt[0]) ) )
                {
                    // Update *pFbPktInfo
                    pFbPktInfo->m_magNum = pktAddr.magNum;
                    pFbPktInfo->m_pktNum = pktAddr.pktNum;

                    if ( pktAddr.pktNum == 0 )  // X/0
                    {
                        // Only X/0 contains page address and control bits
                        TTX_STD_Page_AddrWithSubCode_t  pageAddr;
                        TTX_STD_Page_CtrlBits_t ctrlBits;
                        bool_t bHammingErr = FALSE;

                        pageAddr.magNum = pktAddr.magNum;

                        pFieldBuff[0] = pktAddr.magNum & 0x07;
                        pFieldBuff[1] = pktAddr.pktNum & 0x1F;
                        pFieldBuff[2] = 0;


                        if ( PPREMU_GetPageAddr( &pageAddr, &(pPkt[2]) ) )
                        {
                            pFieldBuff[3] = TTX_STD_PageAddrNumToHex( &( pageAddr.pageNum ) );
                            pFieldBuff[4] = ( (uint8_t)pageAddr.subCode.s4 << 4) | (pageAddr.subCode.s3 & 0x0F);
                            pFieldBuff[5] = ( (uint8_t)pageAddr.subCode.s2 << 4) | (pageAddr.subCode.s1 & 0x0F);
                        }
                        else
                        {
                            bHammingErr = TRUE;
                        }

                        if ( PPREMU_GetCtrlBits( &ctrlBits, &(pPkt[5]) ) )
                        {
                            pFieldBuff[6] =  ((uint8_t)ctrlBits.bErasePage            << 6) \
                                             |((uint8_t)ctrlBits.bNewsFlash            << 5) \
                                             |((uint8_t)ctrlBits.bSubtitle             << 4) \
                                             |((uint8_t)ctrlBits.bSuppressHeader       << 3) \
                                             |((uint8_t)ctrlBits.bUpdate               << 2) \
                                             |((uint8_t)ctrlBits.bInterruptedSequence  << 1) \
                                             |((uint8_t)ctrlBits.bInhibitDisplay       << 0);

                            pFieldBuff[7] =  ((uint8_t)ctrlBits.bSerialMode << 3) \
                                             |          ctrlBits.natlOpt  ;
                        }
                        else
                        {
                            bHammingErr = TRUE;
                        }

                        // Fill pFieldBuff[8] ~ [39] with odd-parity decoded data
                        if ( ! TTX_CD_ChannelDecode(&(pPkt[10]), &(pFieldBuff[8]), 32, TTX_CD_ODD_PARITY) )
                        {
                            pFbPktInfo->m_derr = TRUE;
                            pFieldBuff[39] |= 0x80; // odd-parity error
                        }

                        if ( bHammingErr )
                        {
                            pFbPktInfo->m_hcerr = TRUE;
                            pFieldBuff[38] |= 0x80; // hamming error
                        }

                        // Update pkt_x0_ind
                        hPpr->m_pfe.m_pktX0Ind |= (UINT32_C(1) << idxPkt);


                        // Implements OSD rolling header
#if OSD_SOFTWARE_EMULATOR
                        if ( ! pFbPktInfo->m_hcerr ) /* X/0 is protected by Hamming 8/4 */
                        {
                            extern void OSDEMU_onReceivedX0 (const uint8_t* oddParityDecodedData, const TTX_STD_Page_CtrlBits_t* pCtrlBits);
                            OSDEMU_onReceivedX0( pFieldBuff, &ctrlBits );
                        }
#endif  /* end of #if OSD_SOFTWARE_EMULATOR */

                    }   // end of if( pktAddr.pktNum == 0 )
                    else if ( (  1 <= pktAddr.pktNum )&&( pktAddr.pktNum <= 25 ) )   // X/1 to X/25
                    {
                        // Fill pFieldBuff[0] ~ [39] with odd-parity encoded data
                        memcpy( &(pFieldBuff[0]), &(pPkt[2]), 40 );
                    }
                    else if ( (pktAddr.magNum == 0) && (pktAddr.pktNum == 30 ) ) // 8/30
                    {
                    }
                    else // Only packets X/26, X/27, X/28, X/29 and X/30 have designation code.
                    {
                        TTX_STD_DesigCode_t desigCode;
                        if ( TTX_CD_ChannelDecode(&(pPkt[2]), &desigCode, sizeof( TTX_STD_DesigCode_t ), TTX_CD_HAMMING_8_4_MODE_0) )
                        {
                            pFbPktInfo->m_dcode = desigCode;

                            // Fill field buffer
                            {
                                pFieldBuff[0] = desigCode;

                                // Fill pFieldBuff[1] ~ [39] with odd-parity encoded data
                                memcpy( &(pFieldBuff[1]), &(pPkt[3]), 39 );
                            }
                        }
                    }

                    // Update last_pkt_addr
                    hPpr->m_pfe.m_addrLastPkt = (idxPkt+1)*40/4;

                    return( TRUE );
                }   // end of if( PPREMU_GetPktAddr( &pktAddr, &(pPkt[0]) ) )
            }   // end of if( numOfBytes == 42 )
        }
    }
    return( FALSE );
}

/*---------------------------------------------------------------------------*/
static bool_t
PPREMU_GetPktAddr          (TTX_STD_Packet_Address_t* pPktAddr, const uint8_t* buf)
/*****************************************************************************\
 *  @brief   Compute *pMagNum from buf[0] and buf[1].
 *  @details Get magazine number (or packet number) from byte-4 of
 *             scrambled Teletext packet.
 *           Get row-number (or equivalently, packet number) from byte-4 and byte-5 of
 *             scrambled Teletext packet.
 *  @param   buf[0] and buf[1] are byte-4 and byte-5 of scrambled Teletext packet.
 *  @return  The desramble status.
 *  @retval  true if *pPktAddr can be used.
 *  @retval  false if *pPktAddr cannot be used.
 *
 *  @pre     buf[0] is the byte-4 of scrambled Teletext packet
 *  @pre     buf[1] is the byte-5 of scrambled Teletext packet
 *  @post    *pPktAddr will be the descrambled packet address.
 *  @post    if( pPktAddr->magNum == 0 ) then it means magazine-8.
\*****************************************************************************/
{
    if ( pPktAddr != NULL )
    {
        if ( xGetMagNum( &(pPktAddr->magNum), &(buf[0]) ) )
        {
            if ( xGetPktNum( &(pPktAddr->pktNum), &(buf[0]) ) )
            {
                return( TRUE );
            }
        }
    }

    return( FALSE );
}


/*---------------------------------------------------------------------------*/
static bool_t
xGetMagNum                  (TTX_STD_MagazineNum_t* pMagNum, const uint8_t *buf)
/*****************************************************************************\
 *  @brief   Compute *pMagNum from buf[0].
 *  @details Get row-number (or packet number) from byte-4 of
 *             scrambled Teletext packet.
 *  @param   buf[0] is the byte-4 of scrambled Teletext packet.
 *  @return  The desramble status.
 *  @retval  true if *pMagNum can be used.
 *  @retval  false if *pMagNum cannot be used.
 *
 *  @pre     buf[0] is the byte-4 of scrambled Teletext packet
 *  @post    *pMagNum will be the descrambled magazine number.
 *  @post    if( *pMagNum == 0 ) then it means magazine-8.
\*****************************************************************************/
{
    uint8_t temp = 0;

    // hamming8/4 decode dies on input = 0.
    if ( buf[0] == 0 )
    {
        if ( pMagNum != NULL )
        {
            *pMagNum = (TTX_STD_MagazineNum_t)0;
        }
    }
    else
    {
        if ( TTX_CD_ChannelDecode(buf, &temp, 1, TTX_CD_HAMMING_8_4_MODE_0) == FALSE )
            return( FALSE );

        if ( pMagNum != NULL )
        {
            *pMagNum = (TTX_STD_MagazineNum_t)(temp & 0x7);
        }
    }

    return( TRUE );
}

/*---------------------------------------------------------------------------*/
static bool_t
xGetPktNum                  (TTX_STD_Packet_Num_t*  pPktNum, const uint8_t* buf)
/*****************************************************************************\
 *  @brief   Compute *rowNum from buf[0] and buf[1].
 *  @details Get row-number (or equivalently, packet number) from byte-4 and byte-5 of
 *             scrambled Teletext packet.
 *  @param   buf[0] and buf[1] are byte-4 and byte-5 of scrambled Teletext packet.
 *  @return  The desramble status.
 *  @retval  true if *rowNum can be used.
 *  @retval  false if *rowNum cannot be used.
 *
 *  @pre     buf[0] is the byte-4 of scrambled Teletext packet
 *  @pre     buf[1] is the byte-5 of scrambled Teletext packet
 *  @post    *rowNum will be the descrambled row-number (packet-number).
\*****************************************************************************/
{
    uint8_t des1 = 0, des2 = 0;

    if ( pPktNum != NULL )
    {
        *pPktNum = 0;
    }

    // hamming8/4 decode dies on input = 0.
    if ( buf[0] != 0 )
    {
        if ( TTX_CD_ChannelDecode(buf, &des1, 1, TTX_CD_HAMMING_8_4_MODE_0 ) == FALSE )
            return FALSE;
    }
    else
        return( FALSE );

    if ( buf[1] != 0 )
    {
        if ( TTX_CD_ChannelDecode( buf+1, &des2, 1, TTX_CD_HAMMING_8_4_MODE_0 ) == FALSE )
            return FALSE;
    }
    else
        return FALSE;

    if ( pPktNum != NULL )
    {
        *pPktNum += (des1>>3)&1;
        *pPktNum += (des2&1)*2;
        *pPktNum += ((des2>>1)&1)*4;
        *pPktNum += ((des2>>2)&1)*8;
        *pPktNum += ((des2>>3)&1)*16;

        return( TRUE );
    }
    return( FALSE );
}

/*---------------------------------------------------------------------------*/
static bool_t
PPREMU_GetPageAddr         (TTX_STD_Page_AddrWithSubCode_t*    pPageAddr, const uint8_t* buf)
/*****************************************************************************\
 *  @brief   Compute page address from buf[0]~[5].
 *  @details Get page address from byte-6 ~11  of scrambled Teletext X/0  packet.
 *  @note    pPageAddr->magNum will NOT be updated.
 *
 *  @param   buf[0]~[5] byte-6 ~ 11 of scrambled Teletext X/0 packet.
 *  @return  The desramble status.
 *  @retval  true if *pPageAddr can be used.
 *  @retval  false if *pPageAddr cannot be used.
 *
 *  @pre     buf[0]~buf[5] are the byte-6~11 of scrambled Teletext X/0 packet
 *  @post    *pPageAddr will be the descrambled packet address.
\*****************************************************************************/
{
    if ( pPageAddr != NULL )
    {
        if ( xGetPageNum( &(pPageAddr->pageNum), &(buf[0]) ) == TRUE )
        {
            if ( xGetSubpageNum( &(pPageAddr->subCode), &(buf[2]) ) == TRUE )
            {
                return( TRUE );
            }
        }
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
static bool_t
xGetPageNum                 (TTX_STD_Page_Addr_Number_t*  pPageNum   , const uint8_t* buf)
/*****************************************************************************\
 *  @details Get (pPageNum->units) and (pPageNum->tens) from
 *             buf[0] and buf[1].
 *  @return  The desramble status.
 *  @retval  true  if (pPageNum->units) and (pPageNum->tens) can be used.
 *  @retval  false otherwise.
 *
 *  @pre     buf[0] is the byte-6 of scrambled Teletext page header packet.
 *  @pre     buf[1] is the byte-7 of scrambled Teletext page header packet.
 *  @post    (pPageNum->units) will be the desrambled page units.
 *  @post    (pPageNum->tens) will be the desrambled page tens.
\*****************************************************************************/
{
    uint8_t temp = 0;

    if ( TTX_CD_ChannelDecode(buf, &temp, 1, TTX_CD_HAMMING_8_4_MODE_0) == FALSE )
        return( FALSE );

    if ( pPageNum != NULL )
    {
        pPageNum->units = temp;
    }

    if ( TTX_CD_ChannelDecode(buf+1, &temp, 1, TTX_CD_HAMMING_8_4_MODE_0) == FALSE )
        return( FALSE );

    if ( pPageNum != NULL )
    {
        pPageNum->tens = temp;
    }

    return( TRUE );

}

/*---------------------------------------------------------------------------*/
static bool_t
xGetSubpageNum              (TTX_STD_Page_Addr_SubCode_t* pSubpageNum, const uint8_t* buf)
/*****************************************************************************\
 *  @brief   Compute sub page page address from buf[0]~[3].
 *  @details Get page address from byte-8 ~11  of scrambled Teletext X/0  packet.
 *  @note    pPageAddr->magNum will NOT be updated.
 *
 *  @param   buf[0]~[3] byte-8 ~ 11 of scrambled Teletext X/0 packet.
 *  @return  The desramble status.
 *  @retval  true if *pPageAddr can be used.
 *  @retval  false if *pPageAddr cannot be used.
 *
 *  @pre     buf[0]~buf[3] are the byte-8~11 of scrambled Teletext X/0 packet
 *  @post    *pSubpageNum will be the descrambled subpage address.
\*****************************************************************************/
{
    uint8_t S1 = 0, S2 = 0, S3 = 0, S4 = 0;

    if ( TTX_CD_ChannelDecode(buf  , &S1, 1, TTX_CD_HAMMING_8_4_MODE_0) == FALSE )
        return( FALSE );

    if ( TTX_CD_ChannelDecode(buf+1, &S2, 1, TTX_CD_HAMMING_8_4_MODE_0) == FALSE )
        return( FALSE );

    if ( TTX_CD_ChannelDecode(buf+2, &S3, 1, TTX_CD_HAMMING_8_4_MODE_0) == FALSE )
        return( FALSE );

    if ( TTX_CD_ChannelDecode(buf+3, &S4, 1, TTX_CD_HAMMING_8_4_MODE_0) == FALSE )
        return( FALSE );

    if ( pSubpageNum != NULL )
    {
        pSubpageNum->s1 = S1;
        pSubpageNum->s2 = S2;
        pSubpageNum->s3 = S3;
        pSubpageNum->s4 = S4;

        return( TRUE );
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
static bool_t
PPREMU_GetCtrlBits (TTX_STD_Page_CtrlBits_t* pCtrlBits, const uint8_t* buf)
/*****************************************************************************\
 *  @brief   Compute control bits from buf[0] to [4].
 *  @details Get page address from byte-9 ~13  of scrambled Teletext X/0  packet.
 *
 *  @param   buf[0]~[4] byte-9 ~ 13 of scrambled Teletext X/0 packet.
 *  @return  The desramble status.
 *  @retval  true if *pCtrlBits can be used.
 *  @retval  false if *pCtrlBits cannot be used.
 *
 *  @pre     buf[0]~buf[4] are the byte-9~13 of scrambled Teletext X/0 packet
 *  @post    *pCtrlBits will be the descrambled control bits.
\*****************************************************************************/
{
    uint8_t c12, c13, c14;

    if ( pCtrlBits != NULL )
    {
        pCtrlBits->bErasePage           = (buf[0]>>7) & 1; /* C4 */
        pCtrlBits->bNewsFlash           = (buf[2]>>5) & 1; /* C5 */
        pCtrlBits->bSubtitle            = (buf[2]>>7) & 1; /* C6 */
        pCtrlBits->bSuppressHeader      = (buf[3]>>1) & 1; /* C7 */
        pCtrlBits->bUpdate              = (buf[3]>>3) & 1; /* C8 */
        pCtrlBits->bInterruptedSequence = (buf[3]>>5) & 1; /* C9 */
        pCtrlBits->bInhibitDisplay      = (buf[3]>>7) & 1; /* C10 */
        pCtrlBits->bSerialMode          = (buf[4]>>1) & 1; /* C11 */

        c12                             = (buf[4]>>3) & 1; /* C12 */
        c13                             = (buf[4]>>5) & 1; /* C13 */
        c14                             = (buf[4]>>7) & 1; /* C14 */

        pCtrlBits->natlOpt              = ((c12<<2) | (c13<< 1) | c14);

        return( TRUE );
    }

    return( FALSE );
}



#endif /* end of #if !PPR_SOFTWARE_EMULATOR */

/*---------------------------------------------------------------------------*\
 * end of ppr_emu_test.c                                                     *
\*---------------------------------------------------------------------------*/
