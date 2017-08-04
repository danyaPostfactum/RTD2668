/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ppr_emu.c
 * @brief    PPR software emulator
 * @details  This is a replacement of hardware PPR.
 * @author   Janus Cheng
 * @date     Dec. 11, 2008
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/

#include <ppr.h>            /* Provides: PPR_SOFTWARE_EMULATOR */

#if PPR_SOFTWARE_EMULATOR

#include "ppr_emu.h"        /* Provides: functions to be implemented */
#include "ttx_cd.h"         /* Provides: TTX_CD_ChannelDecode() */

#include "ttx_pkg.h"

#include <stdlib.h>         /* Provides: NULL */
#include <string.h>         /* Provides: memcpy() */

#include <ttx_assert.h>


PPREMU_Obj pprEmu;

TTX_DEFINE_THIS_MODULE( ppr_emu.c )




/* -- Prototypes of Private Functions ---------------------------------------*/
static bool_t           PPR_PfeSetMode                  (PPR_PfeMode_t mode);

// Clear all contents of *pFbPktInfo
static void             PPR_ResetFbPktInfo              (FB_PktInfo* pFbPktInfo);

// Clear all contents of DecodedStatus
static void             PPR_ResetDecodedStatus          (PFE_FbPktDecodedStatus* pDecodedStatus);

static uint16_t         PPR_PfeMoveMode0                (TTX_STD_Packet_Num_t pktNum, const uint8_t* pFrom, uint8_t* pToBase, bool_t bOddParityChk);
static uint16_t         PPR_PfeMoveMode1                (TTX_STD_Packet_Num_t pktNum, TTX_STD_DesigCode_t desigCode, const uint8_t* pFrom, uint8_t* pToBase);
static uint16_t         PPR_PfeMoveMode2                (TTX_STD_Packet_Num_t pktNum, TTX_STD_DesigCode_t desigCode, const uint8_t* pFrom, uint8_t* pToBase);
static uint16_t         PPR_PfeMoveMode3                (TTX_STD_MagazineNum_t magNum, TTX_STD_DesigCode_t desigCode, const uint8_t* pFrom, uint8_t* pToBase);

static bool_t           PPR_PfeGetMode0Offset           (TTX_STD_Packet_Num_t pktNum, uint16_t* pOffset);
static bool_t           PPR_PfeGetMode1Offset           (TTX_STD_Packet_Num_t pktNum, TTX_STD_DesigCode_t desigCode, uint16_t* pOffset);
static bool_t           PPR_PfeGetMode2Offset           (TTX_STD_Packet_Num_t pktNum, TTX_STD_DesigCode_t desigCode, uint16_t* pOffset);
static bool_t           PPR_PfeGetMode3Offset           (TTX_STD_MagazineNum_t magNum, TTX_STD_DesigCode_t desigCode, uint16_t* pOffset);

static bool_t           PPR_GetPfeMode                  (TTX_STD_Packet_Num_t pktNum, TTX_STD_DesigCode_t desigCode, PPR_PfeMode_t* pMode);


/* -- Implementations of Public Functions -----------------------------------*/

/*---------------------------------------------------------------------------*/
bool_t
PPR_Open (const PPR_OpenFlag_t flags)
/*****************************************************************************\
 *  @brief   The initialization routine.
 *  @details Use this function to initialize this library.
 *  @note    Must be called in advance of any other functions.
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

    TTX_STD_MagazineNum_t m;
    for ( m = 1; m<= 8; m++ )
    {
        hPpr->m_pfe.m_cfgMag[m].m_bIsActivated     = FALSE;
        hPpr->m_pfe.m_cfgMag[m].m_8byteDesBaseAddr = 0;
    }


    if ( flags & PPR_OPEN_RESET )
    {
        PPR_Reset();
    }

    return( TRUE );
}

/*---------------------------------------------------------------------------*/
bool_t
PPR_Close (void)
/*****************************************************************************\
 *  @brief   The de-initialization routine.
 *  @details Use this function to de-initialize this library.
 *  @note    Must be called in advance of any other functions.
 *
 *  @return  the initialization status.
 *  @retval  TRUE  success
 *  @retval  FALSE failure
 *
 *  @ingroup ttx
 *  @see     TTX_Init()
\*****************************************************************************/
{

    return( TRUE );
}

/*---------------------------------------------------------------------------*/
bool_t
PPR_PfeSetDestMag             (TTX_STD_MagazineNum_t mag, const void TTX_MEM* pDest)
/*****************************************************************************\
 *  @brief   Set destination of magazine m.
 *  @details Use this function to de-initialize this library.
 *  @note    Must be called in advance of any other functions.
 *
 *  @return  the initialization status.
 *  @retval  TRUE  success
 *  @retval  FALSE failure
 *
 *  @ingroup ttx
 *  @see     TTX_Init()
\*****************************************************************************/
{
    PPREMU_Handle hPpr = &pprEmu;

    const TTX_Address_t dest    = ((TTX_Address_t)pDest) >> 3; // 8-byte basis

    PPR_REQUIRE( TTX_STD_MagNumIsValid( mag ) );    // ERROR: mag is not within valid range of TTX_STD_MagazineNum_t.
    PPR_REQUIRE( ( 1 <= mag ) && ( mag <= 8 ) );    // ERROR: mag is not within valid range of TTX_STD_MagazineNum_t.
    PPR_REQUIRE( (((TTX_Address_t)pDest) & 0x07) == 0 ); // ERROR: mis-aligned pDest.

    if ( ( 1 <= mag ) && ( mag <= 8 ) )
    {
        hPpr->m_pfe.m_cfgMag[mag].m_8byteDesBaseAddr = dest;
        return( TRUE );
    }

    return( FALSE );
}

#if TTX_VERIFY  /* if enabled verficiation of TTX related IPs */

/*---------------------------------------------------------------------------*/
bool_t PPR_PfeGetDestMag
(TTX_STD_MagazineNum_t mag, void TTX_MEM** pDest)
/*****************************************************************************\
 *  @brief   Get destination address by magazine m.
 *
 *  @return  status.
 *  @retval  TRUE  success
 *  @retval  FALSE failure
\*****************************************************************************/
{
    PPREMU_Handle hPpr = &pprEmu;

    const TTX_Address_t dest    = ((TTX_Address_t)hPpr->m_pfe.m_cfgMag[mag].m_8byteDesBaseAddr)<<3; // 8-byte basis

    PPR_REQUIRE( TTX_STD_MagNumIsValid( mag ) );    // ERROR: mag is not within valid range of TTX_STD_MagazineNum_t.
    PPR_REQUIRE( ( 1 <= mag ) && ( mag <= 8 ) );    // ERROR: mag is not within valid range of TTX_STD_MagazineNum_t.

    if ( ( 1 <= mag ) && ( mag <= 8 ) )
    {
        if ( pDest != NULL )
        {
            *pDest = (void TTX_MEM*)(dest);
            return( TRUE );
        }
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
bool_t
PPR_PfeSetDestM29             (const void TTX_MEM* pDest)
/*****************************************************************************\
 *  @brief   Set destination of packet M/29.
 *  @details Use this function to de-initialize this library.
 *  @note    Must be called in advance of any other functions.
 *
 *  @return  the initialization status.
 *  @retval  TRUE  success
 *  @retval  FALSE failure
 *
 *  @ingroup ttx
 *  @see     TTX_Init()
\*****************************************************************************/
{
    PPREMU_Handle hPpr = &pprEmu;

    const TTX_Address_t dest = ((TTX_Address_t)pDest) >> 3; // 8-byte basis

    PPR_REQUIRE( (((TTX_Address_t)pDest) & 0x07) == 0 );  // ERROR: mis-aligned pDest.

    hPpr->m_pfe.m_cfgBpkt.m_8byteDesBaseAddr = dest;

    return( TRUE );
}

#endif  /* end of #if TTX_VERIFY */

/*---------------------------------------------------------------------------*/
void
PPR_TestPwrOnDefaults   (void)
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
\*****************************************************************************/
{
    PPR_Log( LOGGER_VERBOSE, "[PPR]" "[Trace]" "Entering PPR_TestPwrOnDefaults()... ");

    PPR_Log( LOGGER_VERBOSE, "[done]\n");
}


/*---------------------------------------------------------------------------*/
void
PPR_Reset (void)
/*****************************************************************************\
 *  @brief   The de-initialization routine.
 *  @details Use this function to de-initialize this library.
 *  @note    Must be called in advance of any other functions.
 *
 *  @return  the initialization status.
 *  @retval  TTX_TRUE  success
 *  @retval  TTX_FALSE failure
 *
 *  @ingroup ttx
 *  @see     TTX_Init()
\*****************************************************************************/
{
    PPREMU_Handle hPpr = &pprEmu;

    uint8_t pkt;

    // Reset m_pfe
    {
        TTX_STD_MagazineNum_t m;
        PPR_PfeMode_t i;

        for ( m = 1; m<= 8; m++ )
        {
            hPpr->m_pfe.m_cfgMag[m].m_bIsActivated = FALSE;
        }
        hPpr->m_pfe.m_cfgBpkt.m_bIsActivated = FALSE;

        for ( i = 0; i < PPREMU_PFE_MODE_NUM; i++ )
        {
            hPpr->m_pfe.m_bLeft[i] = FALSE;
        }

        hPpr->m_pfe.m_addrLastPkt  = 0;
        hPpr->m_pfe.m_pktX0Ind     = 0;

        hPpr->m_pfe.m_addrNextProc = 0;

        for ( i = 0; i< PPR_FIELD_BUFF_PKT_NUM; i++ )
        {
            PPR_ResetDecodedStatus( &(hPpr->m_pfe.m_fbPktDecodedStatus[i]) );
        }
    }

    // Reset m_dacqu
    {
        hPpr->m_dacqu.m_cntPktRx  = 0;
        hPpr->m_dacqu.m_cntPktErr = 0;
        hPpr->m_dacqu.m_bEnabled  = FALSE;

        for ( pkt = 0; pkt<18; pkt++ )
        {
            PPR_ResetFbPktInfo( &(hPpr->m_dacqu.m_fieldBuffInfo[pkt]) );
        }
    }

    // Reset m_pg
    {
        hPpr->m_pg.m_bLineActive = FALSE;
        hPpr->m_pg.m_bFromSlicer = TRUE;
        hPpr->m_pg.m_bEnabled    = FALSE;
        hPpr->m_pg.m_mode        = PG_MODE_TT;
    }

}


/*---------------------------------------------------------------------------*/
bool_t
PPR_PfeActivateMag              (TTX_STD_MagazineNum_t mag)
/*****************************************************************************\
 *  @brief   Activate moving mag for prefetch moving engine.
 *  @param   mag    the magazine you wanna enable.
 *
 *  @return  the activation status.
 *  @retval  TRUE  success
 *  @retval  FALSE failure
\*****************************************************************************/
{
    PPREMU_Handle hPpr = &pprEmu;

    PPR_REQUIRE( TTX_STD_MagNumIsValid( mag ) );    // ERROR: mag is not within valid range of TTX_STD_MagazineNum_t.
    PPR_REQUIRE( ( 1 <= mag ) && ( mag <= 8 ) );    // ERROR: mag is not within valid range of TTX_STD_MagazineNum_t.

    if ( ( 1 <= mag ) && ( mag <= 8 ) )
    {
        hPpr->m_pfe.m_cfgMag[mag].m_bIsActivated = TRUE;
        return( TRUE );
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
bool_t
PPR_PfeDeactivateMag              (TTX_STD_MagazineNum_t mag)
/*****************************************************************************\
 *  @brief   Deactivate moving mag for prefetch moving engine.
 *  @param   mag    the magazine you wanna disable.
 *
 *  @return  the deactivation status.
 *  @retval  TRUE  success
 *  @retval  FALSE failure
\*****************************************************************************/
{
    PPREMU_Handle hPpr = &pprEmu;

    PPR_REQUIRE( TTX_STD_MagNumIsValid( mag ) );    // ERROR: mag is not within valid range of TTX_STD_MagazineNum_t.
    PPR_REQUIRE( ( 1 <= mag ) && ( mag <= 8 ) );    // ERROR: mag is not within valid range of TTX_STD_MagazineNum_t.

    if ( ( 1 <= mag ) && ( mag <= 8 ) )
    {
        hPpr->m_pfe.m_cfgMag[mag].m_bIsActivated = FALSE;
        return( TRUE );
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
bool_t
PPR_PfeMagIsActive              (TTX_STD_MagazineNum_t mag)
/*****************************************************************************\
 *  @brief   Return whether mag has been activated or not.
 *  @return  the activation status.
 *  @retval  TRUE  mag has already been activated.
 *  @retval  FALSE mag has already been deactivated.
\*****************************************************************************/
{
    PPREMU_Handle hPpr = &pprEmu;

    PPR_REQUIRE( TTX_STD_MagNumIsValid( mag ) );    // ERROR: mag is not within valid range of TTX_STD_MagazineNum_t.
    PPR_REQUIRE( ( 1 <= mag ) && ( mag <= 8 ) );    // ERROR: mag is not within valid range of TTX_STD_MagazineNum_t.

    if ( ( 1 <= mag ) && ( mag <= 8 ) )
    {
        return( hPpr->m_pfe.m_cfgMag[mag].m_bIsActivated );
    }

    return( FALSE );
}

#if TTX_VERIFY  /* if enabled verficiation of TTX related IPs */


/*---------------------------------------------------------------------------*/
bool_t
PPR_PfeActivateM29              (void)
/*****************************************************************************\
 *  @brief   Activate moving mag for prefetch moving engine.
 *           Packet M29 is independent of magazines.
 *
 *  @return  the activation status.
 *  @retval  TRUE  success
 *  @retval  FALSE failure
\*****************************************************************************/
{
    pprEmu.m_pfe.m_cfgBpkt.m_bIsActivated = TRUE;
    return( TRUE );
}

/*---------------------------------------------------------------------------*/
bool_t
PPR_PfeDeactivateM29              (void)
/*****************************************************************************\
 *  @brief   Deactivate moving mag for prefetch moving engine.
 *           Packet M29 is independent of magazines.
 *
 *  @return  the activation status.
 *  @retval  TRUE  success
 *  @retval  FALSE failure
\*****************************************************************************/
{
    pprEmu.m_pfe.m_cfgBpkt.m_bIsActivated = FALSE;

    return( TRUE );
}

#endif  /* end of #if TTX_VERIFY */

/*---------------------------------------------------------------------------*/
void
PPREMU_SetDacquEnable          (void)
/*****************************************************************************\
 *  @brief   Set 1 to the field of VBIPPR_DATA_ACQU_EN.
 *  @note    Models VBIPPR_CTRL_REG.
\*****************************************************************************/
{
    PPREMU_Handle hPpr = &pprEmu;

    uint8_t i;

    hPpr->m_dacqu.m_bEnabled = TRUE;

    for ( i=0; i<4; i++ )
    {
        hPpr->m_pfe.m_bLeft[i] = FALSE;
    }
}

/*---------------------------------------------------------------------------*/
void
PPREMU_SetDacquDisable          (void)
/*****************************************************************************\
 *  @brief   Set 0 to the field of VBIPPR_DATA_ACQU_EN.
 *  @note    Models VBIPPR_CTRL_REG.
\*****************************************************************************/
{
    pprEmu.m_dacqu.m_bEnabled = FALSE;
}

/*---------------------------------------------------------------------------*/
bool_t
PPREMU_GetPfeFldMode0Left  (void)
/*****************************************************************************\
 *  @brief   Returns value of the field VBIPPR_PFE_10P_X025_IND.
 *  @note    Models VBIPPR_PFE_CTRL_REG.
 *
 *  @return  There're MODE0 packets to be moved.
 *  @retval  TRUE  There're MODE0 packets to be moved.
 *  @retval  FALSE There're no MODE0 packets to be moved.
\*****************************************************************************/
{
    return( pprEmu.m_pfe.m_bLeft[0] );
}

/*---------------------------------------------------------------------------*/
bool_t
PPREMU_GetPfeFldMode1Left  (void)
/*****************************************************************************\
 *  @brief   Returns value of the field VBIPPR_PFE_10P_X2628_IND.
 *  @note    Models VBIPPR_PFE_CTRL_REG.
 *
 *  @return  There're MODE1 packets to be moved.
 *  @retval  TRUE  There're MODE1 packets to be moved.
 *  @retval  FALSE There're no MODE1 packets to be moved.
\*****************************************************************************/
{
    return( pprEmu.m_pfe.m_bLeft[1] );
}

/*---------------------------------------------------------------------------*/
bool_t
PPREMU_GetPfeFldMode2Left  (void)
/*****************************************************************************\
 *  @brief   Returns value of the field VBIPPR_PFE_10P_LEVEL25_IND.
 *  @note    Models VBIPPR_PFE_CTRL_REG.
 *
 *  @return  There're MODE2 packets to be moved.
 *  @retval  TRUE  There're MODE2 packets to be moved.
 *  @retval  FALSE There're no MODE2 packets to be moved.
\*****************************************************************************/
{
    return( pprEmu.m_pfe.m_bLeft[2] );
}

/*---------------------------------------------------------------------------*/
bool_t
PPREMU_GetPfeFldMode3Left  (void)
/*****************************************************************************\
 *  @brief   Returns value of the field VBIPPR_PFE_10P_X29_IND.
 *  @note    Models VBIPPR_PFE_CTRL_REG.
 *
 *  @return  There're MODE3 packets to be moved.
 *  @retval  TRUE  There're MODE3 packets to be moved.
 *  @retval  FALSE There're no MODE3 packets to be moved.
\*****************************************************************************/
{
    return( pprEmu.m_pfe.m_bLeft[3] );
}

/*---------------------------------------------------------------------------*/
bool_t
PPR_FieldBuffIsValid            (void)
/*****************************************************************************\
 *  @brief   Returns whether Field Buffer is valid or not.
 *
 *  @return  Validity of field buffer.
 *  @retval  TRUE  Content of field buffer can be used.
 *  @retval  FALSE Content of field buffer can not be used.
\*****************************************************************************/
{
    return( pprEmu.m_dacqu.m_bFbValid );
}

#if 0
/*---------------------------------------------------------------------------*/
void
PPR_DumpFieldBuff            (void)
/*****************************************************************************\
 *  @brief   Dump content of field buffer.
\*****************************************************************************/
{
#ifndef WIN32   /* Visual C++ doesn't support #warning */
#warning "PPR_DumpFieldBuff() is being compiled. It drains much spaces!"
#endif  /* end of #ifndef WIN32 */

    PPREMU_Handle hPpr = &pprEmu;

    size_t  i;
    uint8_t j;

    for ( i=0; i<PPR_FIELD_BUFF_PKT_NUM; i++ )
    {
        PPR_Log( LOGGER_VERBOSE, "[PPR]" "FB%d: ", i);
        for ( j=0; j<PPR_FIELD_BUFF_PKT_SIZE; j++ )
        {
            PPR_Log( LOGGER_VERBOSE,  "%X ", hPpr->m_dacqu.m_fieldBuff[i][j] );
        }
        PPR_Log( LOGGER_VERBOSE, "\n");
    }
}
#endif


/* -- Implementations of Private Functions ----------------------------------*/

/*---------------------------------------------------------------------------*/
static bool_t
PPR_PfeSetMode                  (PPR_PfeMode_t mode)
/*****************************************************************************\
 *  @brief   Set mode of prefetch move engine.
 *
 *  @return  the initialization status.
 *  @retval  TRUE  success
 *  @retval  FALSE failure
 *
 *  @ingroup ttx
 *  @see     TTX_Deinit()
\*****************************************************************************/
{
    pprEmu.m_pfe.m_mode = mode;
    return( TRUE );
}

/*---------------------------------------------------------------------------*/
bool_t
PPR_PfeMove    (PPR_PfeMode_t mode, bool_t bEnableOddParityChk)
/*****************************************************************************\
 *  @brief   Start prefetch moving engine.
 *  @details Move from field buffer to the destination specified by PPR_PfeSetDestMag().
 *  @param   bEnableOddParityChk    enable odd parity check for TT packet in
 *                                  X/1~X/25: 0~39 bytes
 *
 *  @return  the initialization status.
 *  @retval  TRUE  success
 *  @retval  FALSE failure
 *
 *  @post    pprEmu.m_pfe.m_addrNextProc will be updated
 *  @see     TTX_Deinit()
\*****************************************************************************/
{
    PPREMU_Handle hPpr = &pprEmu;

    const uint8_t PKT_SIZE = PPR_FIELD_BUFF_PKT_SIZE;    // bytes
    bool_t bOk = FALSE;

    TTX_STD_MagazineNum_t magNum;

    uint8_t* const pAddrNextProc = &(hPpr->m_pfe.m_addrNextProc);
    const uint8_t* pSrc;

    uint8_t idxPkt = *pAddrNextProc/(PKT_SIZE/4);   /* initialize current packet to the last packet of last invoking this __FUNCTION__. */
    const FB_PktInfo* pFbPktInfo;

    PPR_REQUIRE( (*pAddrNextProc%(PKT_SIZE/4)) == 0 );

    if ( PPR_PfeSetMode( mode ) == TRUE )
    {
        PPR_REQUIRE( UINT8_MAX >= PPR_FIELD_BUFF_PKT_NUM );  /* ERROR: typeof( idxPkt ) cannot represent all packets of field buffer */
        while ( idxPkt < (hPpr->m_pfe.m_addrLastPkt/(PKT_SIZE/4)) )
        {
            PPR_REQUIRE( idxPkt < PPR_FIELD_BUFF_PKT_NUM ); /* ERROR: idxPkt is out of range. Invalid hPpr->m_pfe.m_addrLastPkt? */
            if ( idxPkt < PPR_FIELD_BUFF_PKT_NUM )
            {
                pSrc       =   hPpr->m_dacqu.m_fieldBuff    [idxPkt];   /* points to the content of packet to be moved */
                pFbPktInfo = &(hPpr->m_dacqu.m_fieldBuffInfo[idxPkt]);  /* points to the info    of packet to be moved */

                // When encountering X/0, PFE shall stop.
                if ( hPpr->m_pfe.m_pktX0Ind & (UINT32_C(1)<<idxPkt) )
                {
                    /* before leaving this while-loop, we shall update *pAddrNextProc,
                     *   in case PPR_PfeMoveModeX() hasn't been run.
                     */
                    *pAddrNextProc = idxPkt * (PKT_SIZE/4);
                    break;
                }

                if ( *pAddrNextProc >= ((PPR_FIELD_BUFF_PKT_NUM * PKT_SIZE)/4) )
                {
                    PPR_ERROR();    /* ERROR: pAddrNextProc has exceeds max length of field buffer. */
                    break;
                }

                // To ensure magNum is valid for PPR_PfeMagIsActive().
                magNum = ( (pFbPktInfo->m_magNum == 0)?8: pFbPktInfo->m_magNum );

                // Only move magazines that is activated. Note: M29 is independent of magazines.
                if ( (PPR_PfeMagIsActive( magNum )) || ( (pFbPktInfo->m_pktNum == 29) && (hPpr->m_pfe.m_cfgBpkt.m_bIsActivated) ) )
                {
                    TTX_Address_t addrDes;
                    uint8_t TTX_MEM* pDes;

                    PPR_PfeMode_t modeSupposed;   /* the supposed PPR_PfeMode_t to move current packet */
                    if ( PPR_GetPfeMode( pFbPktInfo->m_pktNum, pFbPktInfo->m_dcode, &modeSupposed ) )
                    {
                        /* If the mode given by PPR_PfeMove() doesn't equal to modeSupposedTo */
                        if ( (mode != modeSupposed) && (modeSupposed < PPREMU_PFE_MODE_NUM ) )
                        {
                            /* Report user to invoke PPR_PfeMove( modeSupposed ) again
                             *   to move this packet.
                             */
                            hPpr->m_pfe.m_bLeft[ modeSupposed ] = TRUE;
                        }
                    }

                    switch ( mode )
                    {
                    case PPR_PFE_MODE0:
                    {
                        if ( pFbPktInfo->m_pktNum <= 25 )
                        {
                            addrDes = (hPpr->m_pfe.m_cfgMag[magNum].m_8byteDesBaseAddr << 3);   /* shift left by three bits to times eight to express in bytes */
                            pDes = (uint8_t TTX_MEM*)(addrDes);

                            *pAddrNextProc += (PPR_PfeMoveMode0( pFbPktInfo->m_pktNum, pSrc, pDes, bEnableOddParityChk )>>2);    // *pAddrNextProc is 4-byte basis
                        }
                        break;
                    }

                    case PPR_PFE_MODE1:
                    {
                        if ( ( 26 <= pFbPktInfo->m_pktNum )&& (pFbPktInfo->m_pktNum <= 28) )
                        {
                            addrDes = (hPpr->m_pfe.m_cfgMag[magNum].m_8byteDesBaseAddr << 3);   /* shift left by three bits to times eight to express in bytes */
                            pDes = (uint8_t TTX_MEM*)(addrDes);

                            *pAddrNextProc += (PPR_PfeMoveMode1( pFbPktInfo->m_pktNum, pFbPktInfo->m_dcode, pSrc, pDes )>>2);    // *pAddrNextProc is 4-byte basis
                        }
                        break;
                    }

                    case PPR_PFE_MODE2:
                    {
                        if ( ( 26 <= pFbPktInfo->m_pktNum )&& (pFbPktInfo->m_pktNum <= 28) )
                        {
                            addrDes = (hPpr->m_pfe.m_cfgMag[magNum].m_8byteDesBaseAddr <<3);   /* shift left by three bits to times eight to express in bytes */
                            pDes = (uint8_t TTX_MEM*)(addrDes);

                            *pAddrNextProc += (PPR_PfeMoveMode2( pFbPktInfo->m_pktNum, pFbPktInfo->m_dcode, pSrc, pDes )>>2);    // *pAddrNextProc is 4-byte basis
                        }
                        break;
                    }

                    case PPR_PFE_MODE3:
                    {
                        if ( pFbPktInfo->m_pktNum == 29 )
                        {
                            addrDes = (hPpr->m_pfe.m_cfgBpkt.m_8byteDesBaseAddr << 3);    /* shift left by three bits to times eight to express in bytes */
                            pDes = (uint8_t TTX_MEM*)(addrDes);

                            PPR_REQUIRE( pFbPktInfo->m_magNum <= 7 );    // ERROR: Required by PPR_PfeMoveMode3().
                            *pAddrNextProc += ( PPR_PfeMoveMode3( pFbPktInfo->m_magNum, pFbPktInfo->m_dcode, pSrc, pDes )>>2);    // *pAddrNextProc is 4-byte basis
                        }
                        break;
                    }

                    default:
                    {
                        PPR_ERROR();    // ERROR: Invalid mode
                        break;
                    }
                    }
                }

                idxPkt++;
            }
        }

        return( TRUE );
    }
    return( bOk );
}

/*---------------------------------------------------------------------------*/
static uint16_t
PPR_PfeMoveMode0    (TTX_STD_Packet_Num_t pktNum, const uint8_t* pFrom, uint8_t* pToBase, bool_t bOddParityChk)
/*****************************************************************************\
 *  @brief   Start prefetch moving engine.
 *  @details Move from field buffer to the destination specified by PPR_PfeSetDestMag().
 *  @param   bEnableOddParityChk    enable odd parity check for TT packet in
 *                                  X/1~X/25: 0~39 bytes
 *
 *  @return  number of bytes that moved.
 *
 *  @post    pprEmu.m_pfe.m_addrNextProc will be updated
 *  @see     TTX_Deinit()
\*****************************************************************************/
{
    const size_t PKT_SIZE = PPR_FIELD_BUFF_PKT_SIZE; // bytes
    uint16_t offset;
    uint16_t i;

    if ( (pFrom == NULL) || (pToBase == NULL) )
        return( 0 );

    if ( PPR_PfeGetMode0Offset(pktNum, &offset) )
    {
        if ( pktNum == 0 )
        {
            memcpy( pToBase + offset, pFrom, PKT_SIZE );
            return( PKT_SIZE );
        }
        else if ( (1<=pktNum)&&(pktNum<=25) )
        {
            uint8_t decoded;

            // Models: VBIPPR_PFE_PARITY_CHK
            if ( bOddParityChk )
            {
                // byte-wise check and move
                for ( i=0; i<PKT_SIZE; i++ )
                {
                    if ( TTX_CD_ChannelDecode(&(pFrom[i]), &decoded, 1, TTX_CD_ODD_PARITY) )
                    {
                        pToBase[offset+i] = decoded;
                    }
                }
            }
            else
            {
                memcpy( pToBase + offset, pFrom, PKT_SIZE );
            }

            return( PKT_SIZE );
        }
        else
        {
            PPR_ERROR();    // ERROR: Invalid pktNum
        }
    }

    PPR_ERROR();    // ERROR: Shouldn't run here!
    return( 0 ); // do nothing when moving other pktNum
}

/*---------------------------------------------------------------------------*/
static uint16_t
PPR_PfeMoveMode1    (TTX_STD_Packet_Num_t pktNum, TTX_STD_DesigCode_t desigCode, const uint8_t* pFrom, uint8_t* pToBase)
/*****************************************************************************\
 *  @brief   Start prefetch moving engine.
 *  @details Move from field buffer to the destination specified by PPR_PfeSetDestMag().
 *  @param   bEnableOddParityChk    enable odd parity check for TT packet in
 *                                  X/1~X/25: 0~39 bytes
 *
 *  @return  number of bytes that moved.
 *
 *  @post    pprEmu.m_pfe.m_addrNextProc will be updated
 *  @see     TTX_Deinit()
\*****************************************************************************/
{
    const size_t PKT_SIZE = PPR_FIELD_BUFF_PKT_SIZE; // bytes

    uint16_t offset;

    if ( (pFrom == NULL) || (pToBase == NULL) )
        return( 0 );

    if ( PPR_PfeGetMode1Offset( pktNum, desigCode, &offset ) )
    {
        memcpy( pToBase + offset, pFrom, PKT_SIZE );
        return( PKT_SIZE );
    }

    return( 0 ); // do nothing when moving other pktNum
}

/*---------------------------------------------------------------------------*/
static uint16_t
PPR_PfeMoveMode2    (TTX_STD_Packet_Num_t pktNum, TTX_STD_DesigCode_t desigCode, const uint8_t* pFrom, uint8_t* pToBase)
/*****************************************************************************\
 *  @brief   Start prefetch moving engine.
 *  @details Move from field buffer to the destination specified by PPR_PfeSetDestMag().
 *  @param   bEnableOddParityChk    enable odd parity check for TT packet in
 *                                  X/1~X/25: 0~39 bytes
 *
 *  @return  number of bytes that moved.
 *
 *  @post    pprEmu.m_pfe.m_addrNextProc will be updated
 *  @see     TTX_Deinit()
\*****************************************************************************/
{
    const size_t PKT_SIZE = PPR_FIELD_BUFF_PKT_SIZE; // bytes

    uint16_t offset;

    if ( (pFrom == NULL) || (pToBase == NULL) )
        return( 0 );

    if ( PPR_PfeGetMode2Offset( pktNum, desigCode, &offset ) )
    {
        memcpy( pToBase + offset, pFrom, PKT_SIZE );
        return( PKT_SIZE );
    }

    return( 0 ); // do nothing when moving other pktNum
}

/*---------------------------------------------------------------------------*/
static uint16_t
PPR_PfeMoveMode3    (TTX_STD_MagazineNum_t magNum, TTX_STD_DesigCode_t desigCode, const uint8_t* pFrom, uint8_t* pToBase)
/*****************************************************************************\
 *  @brief   Start prefetch moving engine.
 *  @details Move from field buffer to the destination specified by PPR_PfeSetDestMag().
 *  @param   bEnableOddParityChk    enable odd parity check for TT packet in
 *                                  X/1~X/25: 0~39 bytes
 *
 *  @return  number of bytes that moved.
 *
 *  @post    pprEmu.m_pfe.m_addrNextProc will be updated
 *  @see     TTX_Deinit()
\*****************************************************************************/
{
    const size_t PKT_SIZE = PPR_FIELD_BUFF_PKT_SIZE; // bytes

    uint16_t offset;

    if ( (pFrom == NULL) || (pToBase == NULL) )
        return( 0 );

    if ( PPR_PfeGetMode3Offset( magNum, desigCode, &offset ) )
    {
        memcpy( pToBase + offset, pFrom, PKT_SIZE );
        return( PKT_SIZE );
    }

    return( 0 ); // do nothing when moving other pktNum
}

/*---------------------------------------------------------------------------*/
static bool_t
PPR_PfeGetMode0Offset    (TTX_STD_Packet_Num_t pktNum, uint16_t* pOffset)
/*****************************************************************************\
 *  @brief   Start prefetch moving engine.
 *  @details Move from field buffer to the destination specified by PPR_PfeSetDestMag().
 *  @param   bEnableOddParityChk    enable odd parity check for TT packet in
 *                                  X/1~X/25: 0~39 bytes
 *
 *  @return  the initialization status.
 *  @retval  TRUE  success
 *  @retval  FALSE failure
 *
 *  @post    pprEmu.m_pfe.m_addrNextProc will be updated
 *  @see     TTX_Deinit()
\*****************************************************************************/
{
    if ( pktNum <= 25 )
    {
        const uint8_t  PKT_SIZE = PPR_FIELD_BUFF_PKT_SIZE; // bytes
        const uint16_t offset   = pktNum * PKT_SIZE;
        if ( pOffset != NULL )
        {
            *pOffset = offset;
            return( TRUE );
        }
    }
    return( FALSE );
}

/*---------------------------------------------------------------------------*/
static bool_t
PPR_PfeGetMode1Offset   (TTX_STD_Packet_Num_t pktNum, TTX_STD_DesigCode_t desigCode, uint16_t* pOffset)
/*****************************************************************************\
 *  @brief   Start prefetch moving engine.
 *  @details Move from field buffer to the destination specified by PPR_PfeSetDestMag().
 *  @param   bEnableOddParityChk    enable odd parity check for TT packet in
 *                                  X/1~X/25: 0~39 bytes
 *
 *  @return  the initialization status.
 *  @retval  TRUE  success
 *  @retval  FALSE failure
 *
 *  @post    pprEmu.m_pfe.m_addrNextProc will be updated
 *  @see     TTX_Deinit()
\*****************************************************************************/
{
    const size_t PKT_SIZE = PPR_FIELD_BUFF_PKT_SIZE; // bytes

    size_t offset       = 0;
    bool_t bOffsetValid = FALSE;

    if ( pktNum == 26 )
    {
        if ( desigCode <= 14 )
        {
            offset = PKT_SIZE * desigCode;
            bOffsetValid = TRUE;
        }
    }
    else if ( pktNum == 27 )
    {
        if ( desigCode == 0 )
        {
            offset = 15 * PKT_SIZE; // We're behind X/26/0~14 packets
            bOffsetValid = TRUE;
        }
    }
    else if ( pktNum == 28 )
    {
        if ( desigCode <= 1 )
        {
            offset = (15 + 1 + desigCode) * PKT_SIZE ;
            bOffsetValid = TRUE;
        }
    }

    if ( bOffsetValid )
    {
        if ( pOffset != NULL )
        {
            *pOffset = offset;
        }
    }

    return( bOffsetValid );
}

/*---------------------------------------------------------------------------*/
static bool_t
PPR_PfeGetMode2Offset   (TTX_STD_Packet_Num_t pktNum, TTX_STD_DesigCode_t desigCode, uint16_t* pOffset)
/*****************************************************************************\
 *  @brief   Start prefetch moving engine.
 *  @details Move from field buffer to the destination specified by PPR_PfeSetDestMag().
 *  @param   bEnableOddParityChk    enable odd parity check for TT packet in
 *                                  X/1~X/25: 0~39 bytes
 *
 *  @return  the initialization status.
 *  @retval  TRUE  success
 *  @retval  FALSE failure
 *
 *  @post    pprEmu.m_pfe.m_addrNextProc will be updated
 *  @see     TTX_Deinit()
\*****************************************************************************/
{
    const size_t PKT_SIZE = PPR_FIELD_BUFF_PKT_SIZE; // bytes

    size_t offset       = 0;
    bool_t bOffsetValid = FALSE;

    if ( pktNum == 26 )
    {
        if ( desigCode == 15 )
        {
            offset = 0;
            bOffsetValid = TRUE;
        }
    }
    else if ( pktNum == 27 )
    {
        if ( (1<=desigCode)&&(desigCode<=7) )
        {
            offset = PKT_SIZE * desigCode; // We're behind X/26/15 packet
            bOffsetValid = TRUE;
        }
    }
    else if ( pktNum == 28 )
    {
        if ( (2<=desigCode)&&(desigCode<=4) )
        {
            offset = (6 + desigCode) * PKT_SIZE;
            bOffsetValid = TRUE;
        }
    }

    if ( bOffsetValid )
    {
        if ( pOffset != NULL )
        {
            *pOffset = offset;
        }
    }

    return( bOffsetValid );
}

/*---------------------------------------------------------------------------*/
static bool_t
PPR_PfeGetMode3Offset   (TTX_STD_MagazineNum_t magNum, TTX_STD_DesigCode_t desigCode, uint16_t* pOffset)
/*****************************************************************************\
 *  @brief   Start prefetch moving engine.
 *  @details Move from field buffer to the destination specified by PPR_PfeSetDestMag().
 *  @param   bEnableOddParityChk    enable odd parity check for TT packet in
 *                                  X/1~X/25: 0~39 bytes
 *
 *  @return  the initialization status.
 *  @retval  TRUE  success
 *  @retval  FALSE failure
 *
 *  @pre     magNum when magazine-8 is 0.
 *  @post    pprEmu.m_pfe.m_addrNextProc will be updated
 *  @see     TTX_Deinit()
\*****************************************************************************/
{
    const size_t PKT_SIZE = PPR_FIELD_BUFF_PKT_SIZE; // bytes

    size_t offset       = 0;
    bool_t bOffsetValid = FALSE;

    if ( (1 <= magNum) && (magNum <= 7) )
    {
        offset = ( 2 * (magNum - 1) + desigCode) * PKT_SIZE;
        bOffsetValid = TRUE;
    }
    else if ( magNum == 0 )
    {
        offset = ( 2 * ( 8     - 1) + desigCode) * PKT_SIZE;
        bOffsetValid = TRUE;
    }
    else
    {
        PPR_ERROR();   // ERROR: Invalid magNum.
    }

    if ( bOffsetValid )
    {
        if ( pOffset != NULL )
        {
            *pOffset = offset;
        }
    }

    return( bOffsetValid );

}

/*---------------------------------------------------------------------------*/
static void
PPR_ResetFbPktInfo              (FB_PktInfo* pFbPktInfo)
/*****************************************************************************\
 *  @brief   Clear all contents of *pFbPktInfo.
\*****************************************************************************/
{
    if ( pFbPktInfo != NULL )
    {
        memset( pFbPktInfo, 0, sizeof( FB_PktInfo ) );
    }
}

/*---------------------------------------------------------------------------*/
static void
PPR_ResetDecodedStatus          (PFE_FbPktDecodedStatus* pDecodedStatus)
/*****************************************************************************\
 *  @brief   Clear all contents of DecodedStatus.
\*****************************************************************************/
{
    if ( pDecodedStatus != NULL )
    {
        pDecodedStatus->m_bOddParityErr = FALSE;
        pDecodedStatus->m_bHammingErr   = FALSE;
        pDecodedStatus->m_desigCode     = 0;
        pDecodedStatus->m_pktNum        = 0;
        pDecodedStatus->m_magNum        = 1;
    }
}


/*---------------------------------------------------------------------------*/
bool_t
PPR_GetPfeMode (TTX_STD_Packet_Num_t pktNum, TTX_STD_DesigCode_t desigCode, PPR_PfeMode_t* pMode)
/*****************************************************************************\
 *  @brief   Hamming 24/18 decoder routine.
 *  @details Use this function to decode Hamming 24/18 encoded data.
 *  @param   des  points to the destination to store the decoded *src
 *  @param   src  points to the source Hamming 24/18 encoded data
 *
 *  @return  the decode status
 *  @retval  true  success
 *  @retval  false failure
 *
 *  @ingroup Tim
 *  @see     TTApi_DeInit()
 *  @todo
 *           nothing to do.
\*****************************************************************************/
{
    PPR_PfeMode_t mode       = PPR_PFE_MODE0;
    bool_t        bModeValid = FALSE;

    if ( pktNum <= 25 )
    {
        mode       = PPR_PFE_MODE0;
        bModeValid = TRUE;
    }
    else if (   ((pktNum == 26) && (desigCode <= 14) ) // X/26/0 ~ X/26/14
                ||((pktNum == 27) && (desigCode ==  0) )                    // X/27/0
                ||((pktNum == 28) && (desigCode <=  1) )  // X/28/0 ~ X/28/1
            )
    {
        mode       = PPR_PFE_MODE1;
        bModeValid = TRUE;
    }
    else if (   ((pktNum == 26) && (desigCode == 0) )               // X/26/0
                ||((pktNum == 27) && (1<=desigCode)&&(desigCode<=7))   // X/27/1 ~ X/27/7
                ||((pktNum == 28) && (2<=desigCode)&&(desigCode<=4))   // X/28/2 ~ X/28/4
            )
    {
        mode       = PPR_PFE_MODE2;
        bModeValid = TRUE;
    }
    else if ( (pktNum == 29) && (desigCode<=1) )  // X/29/0 ~ X/29/1
    {
        mode       = PPR_PFE_MODE3;
        bModeValid = TRUE;
    }

    if ( bModeValid )
    {
        if ( pMode != NULL )
        {
            *pMode = mode;
        }
    }

    return( bModeValid );
}


#endif  /* end of #if PPR_SOFTWARE_EMULATOR */

/*---------------------------------------------------------------------------*\
 * end of ppr_emu.c                                                          *
\*---------------------------------------------------------------------------*/
