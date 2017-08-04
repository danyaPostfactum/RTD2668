/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ppr_test.c
 * @brief    Prototypes of public application interface.
 * @details  This header declares the public access application interface for this TtLib library.
 * @author   Janus Cheng
 * @date     Dec. 11, 2007
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/

#include <ppr.h>            /* Provides: functions to be implemented */

#if !PPR_SOFTWARE_EMULATOR

#include "ttx_pkg.h"

#include <stdlib.h>         /* Provides: NULL */
#include <string.h>         /* Provides: memset() */

#include <ttx.h>            /* Provides: TTX_DEFAULT_PAGEx_ADDRESS() */
#include <ttx_buff.h>       /* Provides: TTX_BUF_Open(), TTX_BUF_Close() and TTX_BUF_InsPage() */
#include <ttx_assert.h>
#include <ttx_log.h>


TTX_DEFINE_THIS_MODULE( ppr_test.c )


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

/* -- Implementations of Public Functions -----------------------------------*/

#if TTX_VERIFY  /* if enabled verficiation of TTX related IPs */

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
    PPR_FSETS( PG_MODE, DATA_SRC, PTNGEN );
    PPR_ENSURE( PPR_FGET( PG_MODE, DATA_SRC ) == PPR_PG_MODE_DATA_SRC_PTNGEN );

    PPR_FSETS( PG_MODE, PTNGEN, ENABLE );
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
    PPR_FSETS( PG_MODE, PTNGEN, DISABLE );

    PPR_FSETS( PG_MODE, DATA_SRC, SLICER );
    PPR_ENSURE( PPR_FGET( PG_MODE, DATA_SRC ) == PPR_PG_MODE_DATA_SRC_SLICER );
}

/*---------------------------------------------------------------------------*/
size_t
PPR_PgGenerateOneField (const uint8_t pkts[][PPR_PTNGEN_PKT_LEN_BYTES], size_t numOfPkts)
/*****************************************************************************\
 *  @brief   Generate multiple or single TT packets (pkts[]) as lines to
 *             field buffer by pattern generator.
 *           This function simulates Slicer.
 *  @note    This is a blocking function.
\*****************************************************************************/
{
    size_t i;
    size_t pktsGenerated = 0;

    PPR_REQUIRE( numOfPkts <= PPR_FIELD_BUFF_PKT_NUM );

    // This line is active
    PPR_FSETS ( PG_LINE, ACT, ENABLE );
    PPR_ENSURE( PPR_FGET( PG_LINE, ACT ) == PPR_PG_LINE_ACT_ENABLE );

    for ( i=0; i<numOfPkts; i++ )
    {
        if ( PPR_GenerateOnePacket( pkts[i] ) )
            pktsGenerated++;
    }

    /* Dump debug info
    {
        PPR_Log( LOGGER_VERBOSE, "[PPR]" "=====================\n");
        PPR_Log( LOGGER_VERBOSE, "[PPR]" "LAST_PKT_ADDR == 0x%X\n", PPR_FGET( FB_IND, LAST_PKT_ADDR ));
        PPR_Log( LOGGER_VERBOSE, "[PPR]" "PKT_X0_IND    == 0x%X\n", PPR_FGET( FB_IND, PKT_X0_IND ) );

        // Dump FB_INFO
        {
            PPR_Log( LOGGER_VERBOSE, "[PPR]" "PKT1  == 0x%X\n", PPR_RGET( FB_PKT1  ) );
            PPR_Log( LOGGER_VERBOSE, "[PPR]" "PKT2  == 0x%X\n", PPR_RGET( FB_PKT2  ) );
            PPR_Log( LOGGER_VERBOSE, "[PPR]" "PKT3  == 0x%X\n", PPR_RGET( FB_PKT3  ) );
            PPR_Log( LOGGER_VERBOSE, "[PPR]" "PKT4  == 0x%X\n", PPR_RGET( FB_PKT4  ) );
            PPR_Log( LOGGER_VERBOSE, "[PPR]" "PKT5  == 0x%X\n", PPR_RGET( FB_PKT5  ) );
            PPR_Log( LOGGER_VERBOSE, "[PPR]" "PKT6  == 0x%X\n", PPR_RGET( FB_PKT6  ) );
            PPR_Log( LOGGER_VERBOSE, "[PPR]" "PKT7  == 0x%X\n", PPR_RGET( FB_PKT7  ) );
            PPR_Log( LOGGER_VERBOSE, "[PPR]" "PKT8  == 0x%X\n", PPR_RGET( FB_PKT8  ) );
            PPR_Log( LOGGER_VERBOSE, "[PPR]" "PKT9  == 0x%X\n", PPR_RGET( FB_PKT9  ) );
            PPR_Log( LOGGER_VERBOSE, "[PPR]" "PKT10 == 0x%X\n", PPR_RGET( FB_PKT10 ) );
            PPR_Log( LOGGER_VERBOSE, "[PPR]" "PKT11 == 0x%X\n", PPR_RGET( FB_PKT11 ) );
            PPR_Log( LOGGER_VERBOSE, "[PPR]" "PKT12 == 0x%X\n", PPR_RGET( FB_PKT12 ) );
            PPR_Log( LOGGER_VERBOSE, "[PPR]" "PKT13 == 0x%X\n", PPR_RGET( FB_PKT13 ) );
            PPR_Log( LOGGER_VERBOSE, "[PPR]" "PKT14 == 0x%X\n", PPR_RGET( FB_PKT14 ) );
            PPR_Log( LOGGER_VERBOSE, "[PPR]" "PKT15 == 0x%X\n", PPR_RGET( FB_PKT15 ) );
            PPR_Log( LOGGER_VERBOSE, "[PPR]" "PKT16 == 0x%X\n", PPR_RGET( FB_PKT16 ) );
            PPR_Log( LOGGER_VERBOSE, "[PPR]" "PKT17 == 0x%X\n", PPR_RGET( FB_PKT17 ) );
            PPR_Log( LOGGER_VERBOSE, "[PPR]" "PKT18 == 0x%X\n", PPR_RGET( FB_PKT18 ) );
        }
        PPR_Log( LOGGER_VERBOSE, "[PPR]" "=====================\n");
    }*/


    PPR_FSETS( PG_LINE, ACT, DISABLE );
    PPR_ENSURE( PPR_FGET( PG_LINE, ACT ) == PPR_PG_LINE_ACT_DISABLE );

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
    volatile uint32_t origValue;
    PPR_Log( LOGGER_VERBOSE, "[PPR]" "[Trace]" "Entering PPR_TestRegsReadWrite()... ");

    /* PPR_MAGx_REG */
    {
#define     _PPR_MAGx_READ_WRITE_TEST_VAL(x,val)                  \
            origValue = PPR_RGET( MAG##x );                       \
            PPR_FSET( MAG##x, DES_BADDR, val );                   \
            PPR_ENSURE( PPR_FGET( MAG##x, DES_BADDR ) == val ); \
            PPR_RSET( MAG##x, origValue )

#define     PPR_MAGx_READ_WRITE_TEST(x)                     \
            _PPR_MAGx_READ_WRITE_TEST_VAL(x, 0x00000000);   \
            _PPR_MAGx_READ_WRITE_TEST_VAL(x, 0x003FFFFF);   \
            _PPR_MAGx_READ_WRITE_TEST_VAL(x, 0x001A5A5A);   \
            _PPR_MAGx_READ_WRITE_TEST_VAL(x, 0x0025A5A5)

        PPR_MAGx_READ_WRITE_TEST( 1 );
        PPR_MAGx_READ_WRITE_TEST( 2 );
        PPR_MAGx_READ_WRITE_TEST( 3 );
        PPR_MAGx_READ_WRITE_TEST( 4 );
        PPR_MAGx_READ_WRITE_TEST( 5 );
        PPR_MAGx_READ_WRITE_TEST( 6 );
        PPR_MAGx_READ_WRITE_TEST( 7 );
        PPR_MAGx_READ_WRITE_TEST( 8 );
    }

#define PPR_REG_TEST(reg,val)                 \
        origValue = PPR_RGET( reg );          \
        PPR_RSET  ( reg, val );               \
        PPR_ENSURE( PPR_RGET( reg ) == val ); \
        PPR_RSET( reg, origValue )

    /* PG_MODE_REG */
    {
        // Because the period of PTNEN_ENABLE is too short to be read by CPU,
        //   and it's auto cleared by PFE, we shouldn't test PTNEN_ENABLE here.
        PPR_REG_TEST( PG_MODE, PPR_PG_MODE_RMKS(TT, PTNGEN, DISABLE) );
        PPR_REG_TEST( PG_MODE, PPR_PG_MODE_RMKS(TT, SLICER, DISABLE) );
    }

    /* PG_LINE_REG */
    {
        PPR_REG_TEST( PG_LINE, PPR_PG_LINE_RMKS( ENABLE  ) );
        PPR_REG_TEST( PG_LINE, PPR_PG_LINE_RMKS( DISABLE ) );
    }

    /* PGDATA_BxBy_REG */
    {
        /* Test individual fields */
        {
#define         _PPR_PGDATA_BxBy_FIELD_READ_WRITE_TEST_VAL(byteX,byteZ,byteW,byteY,valX,valZ,valW,valY)    \
                origValue = PPR_RGET( PGDATA_B##byteX##B##byteY );                        \
                PPR_FSET( PGDATA_B##byteX##B##byteY, BYTE##byteX, valX );                 \
                PPR_FSET( PGDATA_B##byteX##B##byteY, BYTE##byteZ, valZ );                 \
                PPR_FSET( PGDATA_B##byteX##B##byteY, BYTE##byteW, valW );                 \
                PPR_FSET( PGDATA_B##byteX##B##byteY, BYTE##byteY, valY );                 \
                PPR_ENSURE( PPR_FGET( PGDATA_B##byteX##B##byteY, BYTE##byteX ) == valX ); \
                PPR_ENSURE( PPR_FGET( PGDATA_B##byteX##B##byteY, BYTE##byteZ ) == valZ ); \
                PPR_ENSURE( PPR_FGET( PGDATA_B##byteX##B##byteY, BYTE##byteW ) == valW ); \
                PPR_ENSURE( PPR_FGET( PGDATA_B##byteX##B##byteY, BYTE##byteY ) == valY ); \
                PPR_RSET( PGDATA_B##byteX##B##byteY, origValue )

#define         PPR_PGDATA_BxBy_FIELD_READ_WRITE_TEST(x,z,w,y) \
                _PPR_PGDATA_BxBy_FIELD_READ_WRITE_TEST_VAL(x,z,w,y, 0x00,0xFF,0x5A,0xA5)

            PPR_PGDATA_BxBy_FIELD_READ_WRITE_TEST( 1, 2, 3, 4);
            PPR_PGDATA_BxBy_FIELD_READ_WRITE_TEST( 5, 6, 7, 8);
            PPR_PGDATA_BxBy_FIELD_READ_WRITE_TEST( 9,10,11,12);
            PPR_PGDATA_BxBy_FIELD_READ_WRITE_TEST(13,14,15,16);
            PPR_PGDATA_BxBy_FIELD_READ_WRITE_TEST(17,18,19,20);
            PPR_PGDATA_BxBy_FIELD_READ_WRITE_TEST(21,22,23,24);
            PPR_PGDATA_BxBy_FIELD_READ_WRITE_TEST(25,26,27,28);
            PPR_PGDATA_BxBy_FIELD_READ_WRITE_TEST(29,30,31,32);
            PPR_PGDATA_BxBy_FIELD_READ_WRITE_TEST(33,34,35,36);
            PPR_PGDATA_BxBy_FIELD_READ_WRITE_TEST(37,38,39,40);
            PPR_PGDATA_BxBy_FIELD_READ_WRITE_TEST(41,42,43,44);

            /* Since PPR_PGDATA_B45B47_REG has only 3 fields which is different from other registers,
               we shall write custom tests for this register.
            */
#define         _PPR_PGDATA_B45B47_FIELD_READ_WRITE_TEST_VAL(byteX,byteZ,byteY,valX,valZ,valY)   \
                origValue = PPR_RGET( PGDATA_B##byteX##B##byteY );                         \
                PPR_FSET( PGDATA_B##byteX##B##byteY, BYTE##byteX, valX );                  \
                PPR_FSET( PGDATA_B##byteX##B##byteY, BYTE##byteZ, valZ );                  \
                PPR_FSET( PGDATA_B##byteX##B##byteY, BYTE##byteY, valY );                  \
                PPR_ENSURE( PPR_FGET( PGDATA_B##byteX##B##byteY, BYTE##byteX ) == valX );  \
                PPR_ENSURE( PPR_FGET( PGDATA_B##byteX##B##byteY, BYTE##byteZ ) == valZ );  \
                PPR_ENSURE( PPR_FGET( PGDATA_B##byteX##B##byteY, BYTE##byteY ) == valY );  \
                PPR_RSET( PGDATA_B##byteX##B##byteY, origValue )

#define         PPR_PGDATA_B45B47_FIELD_READ_WRITE_TEST(x,z,y) \
                _PPR_PGDATA_B45B47_FIELD_READ_WRITE_TEST_VAL(x,z,y,0x00,0xFF,0xA5)
            PPR_PGDATA_B45B47_FIELD_READ_WRITE_TEST(45,46,47);

        }

        /* Test overall register */
        {
#define         _PPR_PGDATA_BxBy_REG_READ_WRITE_TEST_VAL(x,y,valX,valZ,valW,valY)                               \
                origValue = PPR_RGET( PGDATA_B##x##B##y );                                                      \
                PPR_RSET( PGDATA_B##x##B##y, PPR_PGDATA_B##x##B##y##_RMK(valX, valZ, valW, valY) );             \
                PPR_ENSURE( PPR_RGET( PGDATA_B##x##B##y ) == PPR_PGDATA_B##x##B##y##_RMK(valX,valZ,valW,valY) );\
                PPR_RSET( PGDATA_B##x##B##y, origValue )

#define         PPR_PGDATA_BxBy_REG_READ_WRITE_TEST(x,y)                               \
                _PPR_PGDATA_BxBy_REG_READ_WRITE_TEST_VAL(x,y,0xFF,0x00,0xA5,0x5A); \
                _PPR_PGDATA_BxBy_REG_READ_WRITE_TEST_VAL(x,y,0x00,0xFF,0x5A,0xA5); \
                _PPR_PGDATA_BxBy_REG_READ_WRITE_TEST_VAL(x,y,0xA5,0x5A,0x00,0xFF); \
                _PPR_PGDATA_BxBy_REG_READ_WRITE_TEST_VAL(x,y,0x5A,0xA5,0xFF,0x00)

            PPR_PGDATA_BxBy_REG_READ_WRITE_TEST( 1, 4);
            PPR_PGDATA_BxBy_REG_READ_WRITE_TEST( 5, 8);
            PPR_PGDATA_BxBy_REG_READ_WRITE_TEST( 9,12);
            PPR_PGDATA_BxBy_REG_READ_WRITE_TEST(13,16);
            PPR_PGDATA_BxBy_REG_READ_WRITE_TEST(17,20);
            PPR_PGDATA_BxBy_REG_READ_WRITE_TEST(21,24);
            PPR_PGDATA_BxBy_REG_READ_WRITE_TEST(25,28);
            PPR_PGDATA_BxBy_REG_READ_WRITE_TEST(29,32);
            PPR_PGDATA_BxBy_REG_READ_WRITE_TEST(33,36);
            PPR_PGDATA_BxBy_REG_READ_WRITE_TEST(37,40);
            PPR_PGDATA_BxBy_REG_READ_WRITE_TEST(41,44);

            /* Since PPR_PGDATA_B45B47_REG has only 3 fields which is different from other registers,
               we shall write custom tests for this register.
            */
#define         _PPR_PGDATA_B45B47_REG_READ_WRITE_TEST_VAL(byte45,byte46,byte47)                       \
                origValue = PPR_RGET( PGDATA_B45B47 );                                                 \
                PPR_RSET( PGDATA_B45B47, PPR_PGDATA_B45B47_RMK(byte45, byte46, byte47) );              \
                PPR_ENSURE( PPR_RGET( PGDATA_B45B47 ) == PPR_PGDATA_B45B47_RMK(byte45,byte46,byte47) );\
                PPR_RSET( PGDATA_B45B47, origValue )

            _PPR_PGDATA_B45B47_REG_READ_WRITE_TEST_VAL(0xFF,0x00,0xA5);
            _PPR_PGDATA_B45B47_REG_READ_WRITE_TEST_VAL(0x00,0xFF,0x5A);
            _PPR_PGDATA_B45B47_REG_READ_WRITE_TEST_VAL(0xA5,0x5A,0x00);
            _PPR_PGDATA_B45B47_REG_READ_WRITE_TEST_VAL(0x5A,0xA5,0xFF);
        }
    }

    /* PPR_PFE_ACTIVE_REG */
    {
        PPR_REG_TEST( PFE_ACTIVE, PPR_PFE_ACTIVE_RMKS(INACTIVE, INACTIVE, INACTIVE, INACTIVE, INACTIVE, INACTIVE, INACTIVE, INACTIVE, INACTIVE ) );
        PPR_REG_TEST( PFE_ACTIVE, PPR_PFE_ACTIVE_RMKS( ACTIVE ,  ACTIVE ,  ACTIVE ,  ACTIVE ,  ACTIVE ,  ACTIVE ,  ACTIVE ,  ACTIVE , INACTIVE ) );
        PPR_REG_TEST( PFE_ACTIVE, PPR_PFE_ACTIVE_RMKS(INACTIVE,  ACTIVE , INACTIVE,  ACTIVE , INACTIVE,  ACTIVE , INACTIVE,  ACTIVE , INACTIVE ) );
        PPR_REG_TEST( PFE_ACTIVE, PPR_PFE_ACTIVE_RMKS( ACTIVE , INACTIVE,  ACTIVE , INACTIVE,  ACTIVE , INACTIVE,  ACTIVE , INACTIVE,  ACTIVE  ) );
    }

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
    memset( pDest, 0xFF, len ); // reset destination memory to default 0xFF

    // Generates PPR_testMag1WithError[] to field buffer
    PPR_FillFieldBufferFromPg( PPR_testMag1WithError, numOfPkts );

    // Wait for field buffer valid
    while ( PPR_FGET( DATA_ACQU_ST, FB_VALID ) != PPR_DATA_ACQU_ST_FB_VALID_TRUE );

    // ERROR: Our software cannot process field buffer before next comming of field buffer.
    //        In other words, our software didn't meet real-time requirements.
    //        This will cause frame loss.
    TTX_ENSURE( PPR_FGET( DATA_ACQU_ST, FB_FULL ) != PPR_DATA_ACQU_ST_FB_FULL_TRUE );

    // Move from field buffer to pDest.
    PPR_REQUIRE( (TTX_Address_t)pDest % 8 == 0 );
    bOk = PPR_PfeMoveMag( mag, pDest, mode, bEnableOddParityChk );
    PPR_ENSURE( bOk == TRUE );

    // Verify results
    {
        PPR_REQUIRE( len == 26*40 );
        PPR_ENSURE( memcmp( pDest, pAns, len ) == 0 );

        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE0_IND ) == PPR_PFE_CTRL_MODE0_IND_CLEARED );   // ERROR: mode-0 packet(s) left
        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE1_IND ) == PPR_PFE_CTRL_MODE1_IND_LEFT    );   // ERROR: mode-1 packet(s) doesn't left
        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE2_IND ) == PPR_PFE_CTRL_MODE2_IND_CLEARED );   // ERROR: mode-2 packet(s) left
        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE3_IND ) == PPR_PFE_CTRL_MODE3_IND_CLEARED );   // ERROR: mode-3 packet(s) left
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
    while ( PPR_FGET( DATA_ACQU_ST, FB_VALID ) != PPR_DATA_ACQU_ST_FB_VALID_TRUE );

    // ERROR: Our software cannot process field buffer before next comming of field buffer.
    //        In other words, our software didn't meet real-time requirements.
    //        This will cause frame loss.
    TTX_ENSURE( PPR_FGET( DATA_ACQU_ST, FB_FULL ) != PPR_DATA_ACQU_ST_FB_FULL_TRUE );

    // Move from field buffer to pDest.
    PPR_REQUIRE( (TTX_Address_t)pDest % 8 == 0 );
    bOk = PPR_PfeMoveMag( mag, pDest, mode, bEnableOddParityChk );
    PPR_ENSURE( bOk == TRUE );

    // Verify results
    {
        PPR_REQUIRE( len == 26*40 );
        PPR_ENSURE( memcmp( pDest, pAns, len ) == 0 );

        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE0_IND ) == PPR_PFE_CTRL_MODE0_IND_CLEARED );   // ERROR: mode-0 packet(s) left
        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE1_IND ) == PPR_PFE_CTRL_MODE1_IND_LEFT    );   // ERROR: mode-1 packet(s) doesn't left
        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE2_IND ) == PPR_PFE_CTRL_MODE2_IND_CLEARED );   // ERROR: mode-2 packet(s) left
        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE3_IND ) == PPR_PFE_CTRL_MODE3_IND_CLEARED );   // ERROR: mode-3 packet(s) left
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
    while ( PPR_FGET( DATA_ACQU_ST, FB_VALID ) != PPR_DATA_ACQU_ST_FB_VALID_TRUE );

    // ERROR: Our software cannot process field buffer before next comming of field buffer.
    //        In other words, our software didn't meet real-time requirements.
    //        This will cause frame loss.
    TTX_ENSURE( PPR_FGET( DATA_ACQU_ST, FB_FULL ) != PPR_DATA_ACQU_ST_FB_FULL_TRUE );

    // Move from field buffer to pDest.
    PPR_REQUIRE( (TTX_Address_t)pDest % 8 == 0 );
    bOk = PPR_PfeMoveMag( mag, pDest, mode, bEnableOddParityChk );
    PPR_ENSURE( bOk == TRUE );

    // Verify results
    {
        PPR_REQUIRE( len == 26*40 );
        PPR_ENSURE( memcmp( pDest, pAns, len ) == 0 );

        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE0_IND ) == PPR_PFE_CTRL_MODE0_IND_CLEARED );   // ERROR: mode-0 packet(s) left
        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE1_IND ) == PPR_PFE_CTRL_MODE1_IND_LEFT    );   // ERROR: mode-1 packet(s) doesn't left
        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE2_IND ) == PPR_PFE_CTRL_MODE2_IND_CLEARED );   // ERROR: mode-2 packet(s) left
        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE3_IND ) == PPR_PFE_CTRL_MODE3_IND_CLEARED );   // ERROR: mode-3 packet(s) left
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
    while ( PPR_FGET( DATA_ACQU_ST, FB_VALID ) != PPR_DATA_ACQU_ST_FB_VALID_TRUE );

    // ERROR: Our software cannot process field buffer before next comming of field buffer.
    //        In other words, our software didn't meet real-time requirements.
    //        This will cause frame loss.
    TTX_ENSURE( PPR_FGET( DATA_ACQU_ST, FB_FULL ) != PPR_DATA_ACQU_ST_FB_FULL_TRUE );

    // Move from field buffer to pDest.
    PPR_REQUIRE( (TTX_Address_t)pDest % 8 == 0 );
    bOk = PPR_PfeMoveMag( mag, pDest, mode, bEnableOddParityChk );
    PPR_ENSURE( bOk == TRUE );

    // Verify results
    {
        PPR_REQUIRE( len == 26*40 );
        PPR_ENSURE( memcmp( pDest, pAns, len ) == 0 );

        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE0_IND ) == PPR_PFE_CTRL_MODE0_IND_LEFT    );   // ERROR: mode-0 packet(s) doesn't left
        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE1_IND ) == PPR_PFE_CTRL_MODE1_IND_CLEARED );   // ERROR: mode-1 packet(s) left
        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE2_IND ) == PPR_PFE_CTRL_MODE2_IND_CLEARED );   // ERROR: mode-2 packet(s) left
        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE3_IND ) == PPR_PFE_CTRL_MODE3_IND_CLEARED );   // ERROR: mode-3 packet(s) left
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
    while ( PPR_FGET( DATA_ACQU_ST, FB_VALID ) != PPR_DATA_ACQU_ST_FB_VALID_TRUE );

    // ERROR: Our software cannot process field buffer before next comming of field buffer.
    //        In other words, our software didn't meet real-time requirements.
    //        This will cause frame loss.
    TTX_ENSURE( PPR_FGET( DATA_ACQU_ST, FB_FULL ) != PPR_DATA_ACQU_ST_FB_FULL_TRUE );

    // Move from field buffer to pDest.
    PPR_REQUIRE( (TTX_Address_t)pDest % 8 == 0 );
    bOk = PPR_PfeMoveMag( mag, pDest, mode, bEnableOddParityChk );
    PPR_ENSURE( bOk == TRUE );

    // Verify results
    {
        PPR_REQUIRE( len == 26*40 );
        PPR_ENSURE( memcmp( pDest, pAns, len ) == 0 );

        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE0_IND ) == PPR_PFE_CTRL_MODE0_IND_CLEARED );   // ERROR: mode-0 packet(s) left
        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE1_IND ) == PPR_PFE_CTRL_MODE1_IND_CLEARED );   // ERROR: mode-1 packet(s) left
        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE2_IND ) == PPR_PFE_CTRL_MODE2_IND_CLEARED );   // ERROR: mode-2 packet(s) left
        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE3_IND ) == PPR_PFE_CTRL_MODE3_IND_CLEARED );   // ERROR: mode-3 packet(s) left
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
\*****************************************************************************/
{
    extern uint8_t const TTX_ROM PPR_testMag2_0[18][47];
    extern uint8_t const TTX_ROM PPR_testMag2_0_out[26*40];
    bool_t bOk;

    const TTX_STD_MagazineNum_t mag   = 2;  // PPR_testMag2_0[][] contains only magazine-2 packets
    const PPR_PfeMode_t     mode      = PPR_PFE_MODE3;
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
    while ( PPR_FGET( DATA_ACQU_ST, FB_VALID ) != PPR_DATA_ACQU_ST_FB_VALID_TRUE );

    // ERROR: Our software cannot process field buffer before next comming of field buffer.
    //        In other words, our software didn't meet real-time requirements.
    //        This will cause frame loss.
    TTX_ENSURE( PPR_FGET( DATA_ACQU_ST, FB_FULL ) != PPR_DATA_ACQU_ST_FB_FULL_TRUE );

    // Move from field buffer to pDest.
    PPR_REQUIRE( (TTX_Address_t)pDest % 8 == 0 );
    bOk = PPR_PfeMoveM29( pDest, bEnableOddParityChk );
    PPR_ENSURE( bOk == TRUE );

    // Verify results
    {
        PPR_REQUIRE( len == 26*40 );
        PPR_ENSURE( memcmp( pDest, pAns, len ) == 0 );

        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE0_IND ) == PPR_PFE_CTRL_MODE0_IND_CLEARED );   // ERROR: mode-0 packet(s) left
        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE1_IND ) == PPR_PFE_CTRL_MODE0_IND_CLEARED );   // ERROR: mode-1 packet(s) left
        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE2_IND ) == PPR_PFE_CTRL_MODE2_IND_CLEARED );   // ERROR: mode-2 packet(s) left
        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE3_IND ) == PPR_PFE_CTRL_MODE3_IND_CLEARED );   // ERROR: mode-3 packet(s) left
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
    PPR_REQUIRE( numOfPkts <= PPR_FIELD_BUFF_PKT_NUM );

    // Limit numOfPkts to PPR_FIELD_BUFF_PKT_NUM
    if ( numOfPkts > PPR_FIELD_BUFF_PKT_NUM )
    {
        numOfPkts = PPR_FIELD_BUFF_PKT_NUM;
    }

    // Enable pattern gen
    PPR_PgEnable();

    // Clear data aquisition status
    PPR_RSET( DATA_ACQU_ST, PPR_DATA_ACQU_ST_RMKS( CLEAR, /* FB_FULL  */ \
              CLEAR  /* FB_VALID */ \
                                                 ));

    // Reset field-buffer indicators
    PPR_RSET( FB_IND, PPR_FB_IND_RMK( 0, /* LAST_PKT_ADDR */ \
                                      0  /*  PKT_X0_IND   */ ) );


    // Accept data from Slicer
    PPR_FSETS( CTRL, DATA_ACQU, ENABLE );
    PPR_ENSURE( PPR_FGET( CTRL, DATA_ACQU ) == PPR_CTRL_DATA_ACQU_ENABLE );

    // Generates pkts[0][] to pkts[numOfPkts-1][] to field buffer
    PPR_PgGenerateOneField( pkts, numOfPkts );

    // Do not accept data from Slicer
    PPR_FSETS( CTRL, DATA_ACQU, DISABLE );
    PPR_ENSURE( PPR_FGET( CTRL, DATA_ACQU ) == PPR_CTRL_DATA_ACQU_DISABLE );

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
    PPR_REQUIRE( PPR_FGET( PG_MODE, PTNGEN ) == PPR_PG_MODE_PTNGEN_READY );
    if ( PPR_FGET( PG_MODE, PTNGEN ) == PPR_PG_MODE_PTNGEN_READY )
    {
        // Record field info
        const bool_t field = pkt[3] & 0x01;  // 0: top/odd/field1    1: bottom/even/field2
        _PPR_PG_MODE_TYPE mode;

        if ( PPR_SetPgLineData( pkt ) )
        {
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

                PPR_FSET( PG_MODE, MODE, mode );
                PPR_ENSURE( PPR_FGET( PG_MODE, MODE ) == mode );
            }


            PPR_REQUIRE( mode == PPR_PG_MODE_MODE_TT ); // ERROR: In our verification, we shall encounter TT only.

            // Start sending pattern
            PPR_FSETS( PG_MODE, PTNGEN, ENABLE );

            // Because the period of PTNEN_ENABLE is too short to be read by CPU,
            //   and it's auto cleared by PFE, we shouldn't read PTNEN_ENABLE here.

            while     ( PPR_FGET( PG_MODE, PTNGEN ) == PPR_PG_MODE_PTNGEN_BUSY  );    // wait for patten gen ready
            PPR_ENSURE( PPR_FGET( PG_MODE, PTNGEN ) == PPR_PG_MODE_PTNGEN_READY );

            return( TRUE );
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
    PPR_REQUIRE( ptn != NULL );
    if ( ptn != NULL )
    {
        PPR_RSET( PGDATA_B1B4  , PPR_PGDATA_B1B4_RMK  ( ptn[ 0], ptn[ 1], ptn[ 2], ptn[ 3] ) );
        PPR_RSET( PGDATA_B5B8  , PPR_PGDATA_B5B8_RMK  ( ptn[ 4], ptn[ 5], ptn[ 6], ptn[ 7] ) );
        PPR_RSET( PGDATA_B9B12 , PPR_PGDATA_B9B12_RMK ( ptn[ 8], ptn[ 9], ptn[10], ptn[11] ) );
        PPR_RSET( PGDATA_B13B16, PPR_PGDATA_B13B16_RMK( ptn[12], ptn[13], ptn[14], ptn[15] ) );
        PPR_RSET( PGDATA_B17B20, PPR_PGDATA_B17B20_RMK( ptn[16], ptn[17], ptn[18], ptn[19] ) );
        PPR_RSET( PGDATA_B21B24, PPR_PGDATA_B21B24_RMK( ptn[20], ptn[21], ptn[22], ptn[23] ) );
        PPR_RSET( PGDATA_B25B28, PPR_PGDATA_B25B28_RMK( ptn[24], ptn[25], ptn[26], ptn[27] ) );
        PPR_RSET( PGDATA_B29B32, PPR_PGDATA_B29B32_RMK( ptn[28], ptn[29], ptn[30], ptn[31] ) );
        PPR_RSET( PGDATA_B33B36, PPR_PGDATA_B33B36_RMK( ptn[32], ptn[33], ptn[34], ptn[35] ) );
        PPR_RSET( PGDATA_B37B40, PPR_PGDATA_B37B40_RMK( ptn[36], ptn[37], ptn[38], ptn[39] ) );
        PPR_RSET( PGDATA_B41B44, PPR_PGDATA_B41B44_RMK( ptn[40], ptn[41], ptn[42], ptn[43] ) );
        PPR_RSET( PGDATA_B45B47, PPR_PGDATA_B45B47_RMK( ptn[44], ptn[45], ptn[46]          ) );

        // Verify above writes
        PPR_ENSURE( PPR_RGET( PGDATA_B1B4   ) == PPR_PGDATA_B1B4_RMK  ( ptn[ 0], ptn[ 1], ptn[ 2], ptn[ 3] ) );
        PPR_ENSURE( PPR_RGET( PGDATA_B5B8   ) == PPR_PGDATA_B5B8_RMK  ( ptn[ 4], ptn[ 5], ptn[ 6], ptn[ 7] ) );
        PPR_ENSURE( PPR_RGET( PGDATA_B9B12  ) == PPR_PGDATA_B9B12_RMK ( ptn[ 8], ptn[ 9], ptn[10], ptn[11] ) );
        PPR_ENSURE( PPR_RGET( PGDATA_B13B16 ) == PPR_PGDATA_B13B16_RMK( ptn[12], ptn[13], ptn[14], ptn[15] ) );
        PPR_ENSURE( PPR_RGET( PGDATA_B17B20 ) == PPR_PGDATA_B17B20_RMK( ptn[16], ptn[17], ptn[18], ptn[19] ) );
        PPR_ENSURE( PPR_RGET( PGDATA_B21B24 ) == PPR_PGDATA_B21B24_RMK( ptn[20], ptn[21], ptn[22], ptn[23] ) );
        PPR_ENSURE( PPR_RGET( PGDATA_B25B28 ) == PPR_PGDATA_B25B28_RMK( ptn[24], ptn[25], ptn[26], ptn[27] ) );
        PPR_ENSURE( PPR_RGET( PGDATA_B29B32 ) == PPR_PGDATA_B29B32_RMK( ptn[28], ptn[29], ptn[30], ptn[31] ) );
        PPR_ENSURE( PPR_RGET( PGDATA_B33B36 ) == PPR_PGDATA_B33B36_RMK( ptn[32], ptn[33], ptn[34], ptn[35] ) );
        PPR_ENSURE( PPR_RGET( PGDATA_B37B40 ) == PPR_PGDATA_B37B40_RMK( ptn[36], ptn[37], ptn[38], ptn[39] ) );
        PPR_ENSURE( PPR_RGET( PGDATA_B41B44 ) == PPR_PGDATA_B41B44_RMK( ptn[40], ptn[41], ptn[42], ptn[43] ) );
        PPR_ENSURE( PPR_RGET( PGDATA_B45B47 ) == PPR_PGDATA_B45B47_RMK( ptn[44], ptn[45], ptn[46]          ) );

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
        PPR_FSET( FB_IND, PKT_X0_IND, 0 );

        // Reset prefetch start address of field buffer
        PPR_FSETS( PFE_NPROC_ADDR, ADDR, PKT(0) );
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
        PPR_FSETS( PFE_NPROC_ADDR, ADDR, PKT(0) );
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

#endif  /* end of #if TTX_VERIFY */

#endif  /* end of #if !PPR_SOFTWARE_EMULATOR */

/*---------------------------------------------------------------------------*\
 * end of ppr_test.c                                                         *
\*---------------------------------------------------------------------------*/
