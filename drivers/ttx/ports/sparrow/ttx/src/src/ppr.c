/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ppr.c
 * @brief    PPR hardware driver.
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
#include <ttx_assert.h>
#include <ttx_source.h> /* Provides: functions to be implemented */



#include "ppr_hal_test.h"   /* Provides: compile-time tests for CCL PPR */

TTX_DEFINE_THIS_MODULE( ppr.c )




/* -- Prototypes of Private Functions ---------------------------------------*/

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
    static bool_t bFirst = TRUE;

    if ( bFirst == TRUE )
    {
        // Power-On default Tests
        PPR_TestPwrOnDefaults();
        bFirst = FALSE;
    }

    if ( flags & PPR_OPEN_RESET )
    {
        PPR_Reset();
    }

    // Initialize interrupt
    {
//        PPR_FSETS( INT_EN, FB_VALID, ENABLE );
        TTX_Reg32Write( VBIPPR_INT_EN_REG_0x400c, _BIT6 | _BIT31);
//        PPR_FSETS( INT_EN, MASTER  , ENABLE );  // Enable PPR's Master Interrupt
        TTX_onPprEnablingInterrupt();
    }

    //  set the bottom line, the default IC value is wrong
    rtd_outl(VBIPPR_LVLD_BTM_REG_0x42e4, 0x0150013d);

    /*
        // Ported from Chin-How's mice commands: startice.cmd
        {
            PPR_RSET( CTRL         , 0x00000001 );   // ew	bf00a000 = 0x00000001
            PPR_RSET( PG_LINE      , 0x00000001 );   // ew	bf00a304 = 0x00000001
            PPR_RSET( PGDATA_B1B4  , 0x00ffff23 );   // ew	bf00a308 = 0x00ffff23
            PPR_RSET( PGDATA_B5B8  , 0x2a021515 );   // ew	bf00a30c = 0x2a021515
            PPR_RSET( PGDATA_B9B12 , 0x15151515 );   // ew	bf00a310 = 0x15151515


            PPR_RSET( PGDATA_B9B12 , 0x20202043 );   // ew	bf00a310 = 20202043
            PPR_RSET( PGDATA_B13B16, 0xaec4aed5 );   // ew	bf00a314 = aec4aed5
            PPR_RSET( PGDATA_B17B20, 0xae865445 );   // ew	bf00a318 = ae865445
            PPR_RSET( PGDATA_B21B24, 0xd35420d0 );   // ew	bf00a31c = d35420d0
            PPR_RSET( PGDATA_B25B28, 0xc1c74520 );   // ew	bf00a320 = c1c74520
            PPR_RSET( PGDATA_B29B32, 0x31202002 );   // ew	bf00a324 = 31202002
            PPR_RSET( PGDATA_B33B36, 0x2032b0b0 );   // ew	bf00a328 = 2032b0b0
            PPR_RSET( PGDATA_B37B40, 0x20202020 );   // ew	bf00a32c = 20202020
            PPR_RSET( PGDATA_B41B44, 0x00170005 );   // ew	bf00a220 = 00170005
            PPR_RSET( PGDATA_B45B47, 0x0150013e );   // ew	bf00a224 = 0150013e
            // ew	bf01b000 = 40040000
            // ew	bf01b008 = 800027fb

        }
    */



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
    // Initialize interrupt
    {
        TTX_onPprDisablingInterrupt();

        PPR_FSETS( INT_DIS, MASTER, DISABLE );  // Disable PPR's Master Interrupt
    }

    return( TRUE );
}

#if TTX_VERIFY  /* if enabled verficiation of TTX related IPs */

/*---------------------------------------------------------------------------*/
bool_t PPR_PfeGetDestMag
(TTX_STD_MagazineNum_t mag, void TTX_MEM** pDest)
/*****************************************************************************\
 *  @brief   Get destination by magazine m.
 *
 *  @return  status.
 *  @retval  TRUE  success
 *  @retval  FALSE failure
\*****************************************************************************/
{
#if 0
    PPR_ASSERT_COMPILE( PPR_ADDR( MAG2 ) - PPR_ADDR( MAG1 ) == 4 );
    PPR_ASSERT_COMPILE( PPR_ADDR( MAG3 ) - PPR_ADDR( MAG2 ) == 4 );
    PPR_ASSERT_COMPILE( PPR_ADDR( MAG4 ) - PPR_ADDR( MAG3 ) == 4 );
    PPR_ASSERT_COMPILE( PPR_ADDR( MAG5 ) - PPR_ADDR( MAG4 ) == 4 );
    PPR_ASSERT_COMPILE( PPR_ADDR( MAG6 ) - PPR_ADDR( MAG5 ) == 4 );
    PPR_ASSERT_COMPILE( PPR_ADDR( MAG7 ) - PPR_ADDR( MAG6 ) == 4 );
    PPR_ASSERT_COMPILE( PPR_ADDR( MAG8 ) - PPR_ADDR( MAG7 ) == 4 );
#endif
    const TTX_Address_t addrReg = PPR_ADDR( MAG1 )+ (mag-1)*4;
    const TTX_Address_t addrDes = ((TTX_Address_t)8) * PPR_RegRead( addrReg );

//    PPR_REQUIRE( TTX_STD_MagNumIsValid( mag ) );    // ERROR: mag is not within valid range of TTX_STD_MagazineNum_t.


    if ( pDest != NULL )
    {
        *pDest = (void TTX_MEM*)addrDes;
        return( TRUE );
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
 *  @retval  FALSE failure: May be due to pDest is too large to be held in BPKT register.
 *
 *  @ingroup ttx
 *  @see     TTX_Init()
\*****************************************************************************/
{
    bool_t bOk = FALSE;

    const TTX_Address_t dest = ((TTX_Address_t)pDest) >> 3; // 8-byte basis

    PPR_REQUIRE( (((TTX_Address_t)pDest) & 0x07) == 0 );  // ERROR: mis-aligned pDest.

    TTX_Reg32Write( VBIPPR_BPKT_DES_BADDR_REG_0x40d8, dest);
    bOk = ( ((TTX_Address_t)8)*TTX_Reg32Read(VBIPPR_BPKT_DES_BADDR_REG_0x40d8) == ((TTX_Address_t)pDest) );   // 8-byte basis
    PPR_ENSURE( bOk );

    return( bOk );
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

#define     PPR_DEFAULT_TEST(reg)  PPR_ENSURE( PPR_RGET( reg ) == PPR_##reg##_DEFAULT )

    PPR_DEFAULT_TEST( CTRL );
    PPR_DEFAULT_TEST( VBIINT_ST );
    PPR_DEFAULT_TEST( DEBUG );
    PPR_DEFAULT_TEST( DBOUNCE2 );


    /* PPR_MAGx_REG */
    {
        PPR_DEFAULT_TEST( MAG1 );
        PPR_DEFAULT_TEST( MAG2 );
        PPR_DEFAULT_TEST( MAG3 );
        PPR_DEFAULT_TEST( MAG4 );
        PPR_DEFAULT_TEST( MAG5 );
        PPR_DEFAULT_TEST( MAG6 );
        PPR_DEFAULT_TEST( MAG7 );
        PPR_DEFAULT_TEST( MAG8 );
    }

    /* PGDATA_BxBy_REG */
    {
#define     PPR_PGDATA_BxBy_DEFAULT_TEST(x,y)  PPR_ENSURE( PPR_RGET( PGDATA_B##x##B##y ) == PPR_PGDATA_B##x##B##y##_DEFAULT )

        PPR_PGDATA_BxBy_DEFAULT_TEST( 1, 4);
        PPR_PGDATA_BxBy_DEFAULT_TEST( 5, 8);
        PPR_PGDATA_BxBy_DEFAULT_TEST( 9,12);
        PPR_PGDATA_BxBy_DEFAULT_TEST(13,16);
        PPR_PGDATA_BxBy_DEFAULT_TEST(17,20);
        PPR_PGDATA_BxBy_DEFAULT_TEST(21,24);
        PPR_PGDATA_BxBy_DEFAULT_TEST(25,28);
        PPR_PGDATA_BxBy_DEFAULT_TEST(29,32);
        PPR_PGDATA_BxBy_DEFAULT_TEST(33,36);
        PPR_PGDATA_BxBy_DEFAULT_TEST(37,40);
        PPR_PGDATA_BxBy_DEFAULT_TEST(41,44);
        PPR_PGDATA_BxBy_DEFAULT_TEST(45,47);
    }

    /* PPR_PFE_ACTIVE_REG */
    PPR_DEFAULT_TEST( PFE_ACTIVE );

    /* PPR_LVLD_TOP_REG */
    PPR_DEFAULT_TEST( LVLD_TOP );

    /* PPR_LVLD_BTM_REG */
    PPR_DEFAULT_TEST( LVLD_BTM );

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
    PPR_RSET( PG_MODE, PPR_PG_MODE_DEFAULT );
    PPR_ENSURE( PPR_RGET( PG_MODE ) == PPR_PG_MODE_DEFAULT );

#define PPR_PFE_ACTIVE_FIELD_SET(field,val)           \
        PPR_FSETS( PFE_ACTIVE,field,val );            \
        PPR_ENSURE( PPR_FGET( PFE_ACTIVE, field ) == PPR_PFE_ACTIVE_##field##_##val )

    PPR_PFE_ACTIVE_FIELD_SET( BPKT, INACTIVE ); /* Broadcast Packet: M/29 */
    PPR_PFE_ACTIVE_FIELD_SET( MAG1, INACTIVE );
    PPR_PFE_ACTIVE_FIELD_SET( MAG2, INACTIVE );
    PPR_PFE_ACTIVE_FIELD_SET( MAG3, INACTIVE );
    PPR_PFE_ACTIVE_FIELD_SET( MAG4, INACTIVE );
    PPR_PFE_ACTIVE_FIELD_SET( MAG5, INACTIVE );
    PPR_PFE_ACTIVE_FIELD_SET( MAG6, INACTIVE );
    PPR_PFE_ACTIVE_FIELD_SET( MAG7, INACTIVE );
    PPR_PFE_ACTIVE_FIELD_SET( MAG8, INACTIVE );

    // PFE_CTRL
    {
        PPR_RSET( PFE_CTRL, PPR_PFE_CTRL_RMKS(  RESET,    /*    VPS     */   \
                                                RESET,    /*    CC      */   \
                                                RESET,    /*    WSS     */   \
                                                RESET,    /*   CGMS     */   \
                                                RESET,    /*    830     */   \
                                                DISABLE,  /* PARITY_CHK */   \
                                                CLEAR,    /* MODE3_IND  */   \
                                                DISABLE,  /* MODE3_EN   */   \
                                                CLEAR,    /* MODE2_IND  */   \
                                                DISABLE,  /* MODE2_EN   */   \
                                                CLEAR,    /* MODE1_IND  */   \
                                                DISABLE,  /* MODE1_EN   */   \
                                                CLEAR,    /* MODE0_IND  */   \
                                                DISABLE,  /* MODE0_EN   */   \
                                                10,     /*   PAGE     */   \
                                                DISABLE   /*   START    */   \
                                             ));

        PPR_ENSURE( PPR_FGET( PFE_CTRL, PARITY_CHK ) == PPR_PFE_CTRL_PARITY_CHK_DISABLE );
        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE3_IND  ) == PPR_PFE_CTRL_MODE3_IND_CLEARED  );
        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE3_EN   ) == PPR_PFE_CTRL_MODE3_EN_DISABLE   );
        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE2_IND  ) == PPR_PFE_CTRL_MODE2_IND_CLEARED  );
        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE2_EN   ) == PPR_PFE_CTRL_MODE2_EN_DISABLE   );
        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE1_IND  ) == PPR_PFE_CTRL_MODE1_IND_CLEARED  );
        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE1_EN   ) == PPR_PFE_CTRL_MODE1_EN_DISABLE   );
        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE0_IND  ) == PPR_PFE_CTRL_MODE0_IND_CLEARED  );
        PPR_ENSURE( PPR_FGET( PFE_CTRL, MODE0_EN   ) == PPR_PFE_CTRL_MODE0_EN_DISABLE   );
        PPR_ENSURE( PPR_FGET( PFE_CTRL, PAGE       ) == PPR_PFE_CTRL_PAGE_10            );
        PPR_ENSURE( PPR_FGET( PFE_CTRL, START      ) == PPR_PFE_CTRL_START_DISABLE      );
    }



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
//    PPR_FSETS( PFE_ACTIVE, BPKT, ACTIVE );  // Activate M29
    TTX_Reg32Write( VBIPPR_PFE_ACTIVE_REG_0x40dc, \
                    TTX_Reg32Read( VBIPPR_PFE_ACTIVE_REG_0x40dc) | _BIT8);
    PPR_ENSURE( PPR_FGET( PFE_ACTIVE, BPKT ) == PPR_PFE_ACTIVE_BPKT_ACTIVE );

    return( PPR_FGET( PFE_ACTIVE, BPKT ) == PPR_PFE_ACTIVE_BPKT_ACTIVE );
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
//    PPR_FSETS( PFE_ACTIVE, BPKT, INACTIVE );  // Deactivate M29
    TTX_Reg32Write( VBIPPR_PFE_ACTIVE_REG_0x40dc, \
                    TTX_Reg32Read( VBIPPR_PFE_ACTIVE_REG_0x40dc) & (~_BIT8));

    PPR_ENSURE( PPR_FGET( PFE_ACTIVE, BPKT ) == PPR_PFE_ACTIVE_BPKT_INACTIVE );

    return( PPR_FGET( PFE_ACTIVE, BPKT ) == PPR_PFE_ACTIVE_BPKT_INACTIVE );
}

#endif  /* end of #if TTX_VERIFY */



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

    size_t  i;
    uint8_t j;

    uint32_t raw;

    for ( i=0; i<PPR_FIELD_BUFF_PKT_NUM; i++ )
    {
        PPR_Log( LOGGER_VERBOSE, "[PPR]" "FB%d: ", i);
        for ( j=0; j<PPR_FIELD_BUFF_PKT_SIZE; j+=sizeof(raw) )
        {
            raw = PPR_RegRead( PPR_ADDR(FB_WORDX) + i*PPR_FIELD_BUFF_PKT_SIZE + j );
            PPR_Log( LOGGER_VERBOSE,  "%X %X %X %X ", (raw >> 24) & 0xFF, \
                     (raw >> 16) & 0xFF, \
                     (raw >>  8) & 0xFF, \
                     raw        & 0xFF );
        }
        PPR_Log( LOGGER_VERBOSE, "\n");
    }
}
#endif

/* -- Implementations of Private Functions ----------------------------------*/


#endif  /* end of #if !PPR_SOFTWARE_EMULATOR */

/*---------------------------------------------------------------------------*\
 * end of ppr.c                                                              *
\*---------------------------------------------------------------------------*/
