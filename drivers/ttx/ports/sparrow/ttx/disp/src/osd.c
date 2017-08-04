/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     osd.c
 * @brief    Prototypes of public application interface.
 * @details  This header declares the public access application interface for this TtLib library.
 * @author   Janus Cheng
 * @date     Dec. 11, 2007
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/

#include "osd.h"            /* Provides: functions to be implemented */

TTX_DEFINE_THIS_MODULE( osd.c )




#if !OSD_SOFTWARE_EMULATOR


#include <stdlib.h>         /* Provides: NULL */
#include <string.h>         /* Provides: memcpy() and strlen() */
#include <autoconf.h>
#include "vd.h"             /* Provides: VD_Reset() */
#include "sys.h"            /* Provides: SYS_SDMA_Copy() */
#ifdef CONFIG_TTX_SUPPORT
#include <ttx.h>            /* Provides: TTX_DumpMem() */
#include <ttx_source.h>     /* Provides: TTX_SRC_Open() */
#endif
#include <panel\panel.h>


#define TO_FLASH_ADDR(bank,addr)    (((uint32_t)bank)<<16|addr|0x10000000u) // "OR 0x10000000" means Flash



/* -- Prototypes of Private Functions ---------------------------------------*/
static void OSD_ApplyChanges             (bool_t bBlock);

static bool_t   _bOsdIsOpened = FALSE;


/* -- Implementations of Public Functions -----------------------------------*/

/*---------------------------------------------------------------------------*/
bool_t
OSD_Open (const OSD_OpenFlag_t flags)
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

    static bool_t bFirst = TRUE;

    if ( bFirst == TRUE )
    {
        // Power-On Tests
        OSD_TestPwrOnDefaults();
        bFirst = FALSE;
    }

    if ( flags & OSD_OPEN_RESET )
    {
        OSD_Reset();
    }

    // Initialize control registerts
    OSD_RSET( CTRL1      , OSD_CTRL1_DEFAULT       );
    OSD_RSET( CTRL2      , OSD_CTRL2_DEFAULT       );
    OSD_RSET( CTRL3      , OSD_CTRL3_DEFAULT       );
    OSD_RSET( CTRL4      , OSD_CTRL4_DEFAULT       );
//    OSD_RSET( CTRL5      , OSD_CTRL5_DEFAULT       );
    rtd_outl(VBIOSD_CTRL5_REG_0x4810, 0x00000200); // G0 in font position 0, G1 in font position 2
    OSD_RSET( P1CTRL     , OSD_P1CTRL_DEFAULT      );
    OSD_RSET( P1PNUM_CTRL, OSD_P1PNUM_CTRL_DEFAULT );
    OSD_RSET( P1MSG_CTRL1, OSD_P1MSG_CTRL1_DEFAULT );

    OSD_SetYuvToggle( FALSE );   // Disable U/V toggle

    OSD_SetOutputFormat( TRUE );    // Set to interlace
    /*
        // For FPGA only, not for ASIC
        OSD_FSETS( ITG1, FPGABP, FPGA );
    */
    OSD_SetDisplayMode( FALSE );    // FALSE for TT; TRUE for CC

    OSD_SetOsdPg( FALSE );          // FALSE for disable pattern gen; TRUE for enable pattern gen
    OSD_ApplyChanges( TRUE );    // TRUE for blocking; FALSE for non-blocking

    TTX_onOsdEnablingInterrupt();

    return( TRUE );
}

/*---------------------------------------------------------------------------*/
bool_t
OSD_Close (void)
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
    _bOsdIsOpened = FALSE;

    TTX_onOsdDisablingInterrupt();

    return( TRUE );
}

/*---------------------------------------------------------------------------*/
void
OSD_Reset (void)
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
}

/*---------------------------------------------------------------------------*/
void
OSD_SetYuvToggle    (bool_t bEnable )
/*****************************************************************************\
 *  @brief   Enable/Disable U/V toggle for video 16.
 *  @param   bEnable	: [input] Set '1' to enable, or set '0' to disable.
\*****************************************************************************/
{
    if ( bEnable )
    {
        OSD_FSETS( CTRL1, UVTOG, ENABLE );
        OSD_ENSURE( OSD_FGET( CTRL1, UVTOG ) == OSD_CTRL1_UVTOG_ENABLE );
    }
    else
    {
        OSD_FSETS( CTRL1, UVTOG, DISABLE );
        OSD_ENSURE( OSD_FGET( CTRL1, UVTOG ) == OSD_CTRL1_UVTOG_DISABLE );
    }
}

/*---------------------------------------------------------------------------*/
void
OSD_SetOutputFormat             (bool_t bInterlace)
/*****************************************************************************\
 *  @brief   Set OSD ouput format (interlance or progressive).
 *  @param   mode : [input] Set '1' for interlance mode, or set '0' for progressive mode.
\*****************************************************************************/
{
    if ( bInterlace )
    {
        OSD_FSETS( CTRL1, VTYPE, INTL );
        OSD_ENSURE( OSD_FGET( CTRL1, VTYPE ) == OSD_CTRL1_VTYPE_INTL );
    }
    else
    {
        OSD_FSETS( CTRL1, VTYPE, PROG );
        OSD_ENSURE( OSD_FGET( CTRL1, VTYPE ) == OSD_CTRL1_VTYPE_PROG );
    }
}

/*---------------------------------------------------------------------------*/
void
OSD_SetDisplayMode              (bool_t bClosedCaption)
/*****************************************************************************\
 *  @brief   Set display mode (Teletext or Closed Cpation).
 *  @param   bClosedCaption : [input] Set '1' for CC mode, or set '0' for TT mode.
\*****************************************************************************/
{
    if ( bClosedCaption )
    {
        OSD_FSETS( CTRL1, OTYPE, CC );
        OSD_ENSURE( OSD_FGET( CTRL1, OTYPE ) == OSD_CTRL1_OTYPE_CC );
    }
    else
    {
        OSD_FSETS( CTRL1, OTYPE, TT );
        OSD_ENSURE( OSD_FGET( CTRL1, OTYPE ) == OSD_CTRL1_OTYPE_TT );
    }
}

/*---------------------------------------------------------------------------*/
void
OSD_SetMixedMode    (bool_t bEnable)
/*****************************************************************************\
 *  @brief   Enable/Disable mixed mode.
 *  @param   bEnable	: [input] Set '1' to enable, or set '0' to disable.
\*****************************************************************************/
{
    if ( bEnable )
    {
        OSD_FSETS( CTRL1, OMODE, MIX );
        OSD_ENSURE( OSD_FGET( CTRL1, OMODE ) == OSD_CTRL1_OMODE_MIX );
    }
    else
    {
        OSD_FSETS( CTRL1, OMODE, STD );
        OSD_ENSURE( OSD_FGET( CTRL1, OMODE ) == OSD_CTRL1_OMODE_STD );
    }
}

/*---------------------------------------------------------------------------*/
void
OSD_SetVbiEnable    (bool_t bEnable)
/*****************************************************************************\
 *  @brief   Enable/Disable VBI OSD display.
 *  @param   bEnable	: [input] Set '1' to enable, or set '0' to disable.
\*****************************************************************************/
{
    if ( bEnable )
    {
        OSD_FSETS( CTRL1, OE, ENABLE );
        OSD_ENSURE( OSD_FGET( CTRL1, OE ) == OSD_CTRL1_OE_ENABLE );
    }
    else
    {
        OSD_FSETS( CTRL1, OE, DISABLE );
        OSD_ENSURE( OSD_FGET( CTRL1, OE ) == OSD_CTRL1_OE_DISABLE );
    }
}

/*---------------------------------------------------------------------------*/
void
OSD_SetVdelay    (uint16_t vdelay)
/*****************************************************************************\
 *  @brief   Set vdelay value.
 *  @param   vdelay	: [input] the value of vdelay to set
\*****************************************************************************/
{
    OSD_REQUIRE( OSD_CTRL3_VDLY_MIN <= vdelay );
    OSD_REQUIRE( vdelay <= OSD_CTRL3_VDLY_MAX );
    OSD_FSET( CTRL3, VDLY, vdelay );
    OSD_ENSURE( OSD_FGET( CTRL3, VDLY ) == vdelay );
}

/*---------------------------------------------------------------------------*/
void
OSD_SetHdelay1  (uint8_t hdelay1)
/*****************************************************************************\
 *  @brief   Set hdelay1 value.
 *  @param   hdelay1	: [input] the value of hdelay1 to set
\*****************************************************************************/
{
    OSD_REQUIRE( OSD_CTRL3_HDLY1_MIN <= hdelay1 );
    OSD_REQUIRE( hdelay1 <= OSD_CTRL3_HDLY1_MAX );

    OSD_FSET( CTRL3, HDLY1, hdelay1 );

    /* This is a latch-double-flip-flop register, cannot be verify above writes immediately here. */
}

/*---------------------------------------------------------------------------*/
void
OSD_SetHdelay2  (uint16_t hdelay2)
/*****************************************************************************\
 *  @brief   Set hdelay2 value.
 *  @param   hdelay2	: [input] the value of hdelay2 to set
\*****************************************************************************/
{
    OSD_REQUIRE( OSD_CTRL4_HDLY2_MIN <= hdelay2 );
    OSD_REQUIRE( hdelay2 <= OSD_CTRL4_HDLY2_MAX );

    OSD_FSET( CTRL4, HDLY2, hdelay2 );

    /* This is a latch-double-flip-flop register, cannot be verify above writes immediately here. */
}

/*---------------------------------------------------------------------------*/
void
OSD_SetHdelay3  (uint16_t hdelay3)
/*****************************************************************************\
 *  @brief   Set hdelay3 value.
 *  @param   hdelay3	: [input] the value of hdelay3 to set
\*****************************************************************************/
{
    OSD_REQUIRE( OSD_CTRL4_HDLY3_MIN <= hdelay3 );
    OSD_REQUIRE( hdelay3 <= OSD_CTRL4_HDLY3_MAX );

    OSD_FSET( CTRL4, HDLY3, hdelay3 );

    /* This is a latch-double-flip-flop register, cannot be verify above writes immediately here. */
}

#if 0	// not used
/*---------------------------------------------------------------------------*/
bool_t
OSD_GetVsyncIntrStatus          (void)
/*****************************************************************************\
 *  @brief   Get current vsync interrupt status.
 *  @ret     the current status of vsync interrupt
\*****************************************************************************/
{
    return( OSD_FGET( INTERRUPT, VSYNCIS ) == OSD_INTERRUPT_VSYNCIS_ASSERTED );
}
#endif

/*---------------------------------------------------------------------------*/
void
OSD_ClearVsyncIntrStatus        (void)
/*****************************************************************************\
 *  @brief   Clear vsync interrupt status.
\*****************************************************************************/
{
    OSD_FSETS( INTERRUPT, VSYNCIS, CLEAR );
    /* <Janus>2009/08/10: Commented for causing assertion fail in demo board.
        OSD_ENSURE( OSD_FGET( INTERRUPT, VSYNCIS ) == OSD_INTERRUPT_VSYNCIS_NOP );
    */
}

/*---------------------------------------------------------------------------*/
void
OSD_SetClutAll  (OSD_CLUT_t clut_ary[OSD_TOTAL_CLUT_NUM][OSD_MAX_CLUT_ENTRY_NUM])
/*****************************************************************************\
 *  @brief   Setup the value of every entry in all color lookup tables.
 *  @param   clut_ary	: [input] the array of values to set
\*****************************************************************************/
{
    OSD_RSET(CLUT0_ENTRY0, OSD_CombineRGB( clut_ary[0][0].r, clut_ary[0][0].g, clut_ary[0][0].b ) );
    OSD_RSET(CLUT0_ENTRY1, OSD_CombineRGB( clut_ary[0][1].r, clut_ary[0][1].g, clut_ary[0][1].b ) );
    OSD_RSET(CLUT0_ENTRY2, OSD_CombineRGB( clut_ary[0][2].r, clut_ary[0][2].g, clut_ary[0][2].b ) );
    OSD_RSET(CLUT0_ENTRY3, OSD_CombineRGB( clut_ary[0][3].r, clut_ary[0][3].g, clut_ary[0][3].b ) );
    OSD_RSET(CLUT0_ENTRY4, OSD_CombineRGB( clut_ary[0][4].r, clut_ary[0][4].g, clut_ary[0][4].b ) );
    OSD_RSET(CLUT0_ENTRY5, OSD_CombineRGB( clut_ary[0][5].r, clut_ary[0][5].g, clut_ary[0][5].b ) );
    OSD_RSET(CLUT0_ENTRY6, OSD_CombineRGB( clut_ary[0][6].r, clut_ary[0][6].g, clut_ary[0][6].b ) );
    OSD_RSET(CLUT0_ENTRY7, OSD_CombineRGB( clut_ary[0][7].r, clut_ary[0][7].g, clut_ary[0][7].b ) );
}

/*---------------------------------------------------------------------------*/
uint32_t
OSD_CombineRGB  (uint8_t redValue, uint8_t greenValue, uint8_t blueValue)
/*****************************************************************************\
 *  @brief   Setup the value of every entry in all color lookup tables.
 *  @param   clut_ary	: [input] the array of values to set
\*****************************************************************************/
{
    if (redValue >= 0xFF)
    {
        redValue = 0xFE;
    }

    if (greenValue >= 0xFF)
    {
        greenValue = 0xFE;
    }

    if (blueValue >= 0xFF)
    {
        blueValue = 0xFE;
    }

    return( OSD_CLUT0_ENTRY0_RMK(redValue, greenValue, blueValue) );
}

/*---------------------------------------------------------------------------*/
static void
OSD_ApplyChanges             (bool_t bBlock)
/*****************************************************************************\
 *  @brief   Update all registers that is double buffered.
 *  @param   bBlock : [input] Wait for vsync before return. Set '1' to wait, or set '0' to return immediately.
\*****************************************************************************/
{
    /* Update both GRU and Page-1 */
    OSD_RSET( UPDATE, OSD_UPDATE_RMKS( NOTIFY, NOTIFY ) );

    /* Value of GRU will be auto cleared when next VSync occurs, so no need to
     *   verify above register write.
     */
    if ( bBlock )
    {
        OSD_ClearVsyncIntrStatus();

        // Wait for OSD_GetVsyncIntrStatus() == TRUE
//        while( OSD_GetVsyncIntrStatus() == FALSE );
        /*
        while( OSD_GetVsyncIntrStatus() == FALSE )
        {
            OSD_Log( LOGGER_INFO, "In OSD_ApplyChanges(), line %d\n", (uint32_t)__LINE__ );
        }
        */
    }
}

/*---------------------------------------------------------------------------*/
void
OSD_ApplyEntireScreenChanges             (bool_t bBlock)
/*****************************************************************************\
 *  @brief   Update entire screen.
 *  @param   bBlock : [input] Wait for vsync before return. Set '1' to wait, or set '0' to return immediately.
\*****************************************************************************/
{
    OSD_ApplyChanges( bBlock );
}

/*---------------------------------------------------------------------------*/
void
OSD_ApplyRow0Changes             (bool_t bBlock)
/*****************************************************************************\
 *  @brief   Update only row-0.
 *  @param   bBlock : [input] Wait for vsync before return. Set '1' to wait, or set '0' to return immediately.
\*****************************************************************************/
{
    OSD_ApplyChanges( bBlock );
}

/*---------------------------------------------------------------------------*/
void
OSD_ApplyPageContentChanges      (bool_t bBlock)
/*****************************************************************************\
 *  @brief   Update row-1 to row-23 only.
 *  @param   bBlock : [input] Wait for vsync before return. Set '1' to wait, or set '0' to return immediately.
\*****************************************************************************/
{
    OSD_ApplyChanges( bBlock );
}

/*---------------------------------------------------------------------------*/
void
OSD_ApplyTopCtrlChanges           (bool_t bBlock)
/*****************************************************************************\
 *  @brief   Update TOP control only.
 *  @param   bBlock : [input] Wait for vsync before return. Set '1' to wait, or set '0' to return immediately.
\*****************************************************************************/
{
    OSD_ApplyChanges( bBlock );
}

/*---------------------------------------------------------------------------*/
void
OSD_ApplySubPageNavCtrlChanges    (bool_t bBlock)
/*****************************************************************************\
 *  @brief   Update sub-page navigation control only.
 *  @param   bBlock : [input] Wait for vsync before return. Set '1' to wait, or set '0' to return immediately.
\*****************************************************************************/
{
    OSD_ApplyChanges( bBlock );
}

/*---------------------------------------------------------------------------*/
void
OSD_SetOsdPg                    (bool_t bEnable)
/*****************************************************************************\
 *  @brief   Enable/Disable OSD pattern generator.
 *  @param   bEnable	: [input] Set '1' to enable, or set '0' to disable.
\*****************************************************************************/
{
    if ( bEnable )
    {
        OSD_FSETS( CTRL1, PATEN, ENABLE );
        OSD_ENSURE( OSD_FGET( CTRL1, PATEN ) == OSD_CTRL1_PATEN_ENABLE );
    }
    else
    {
        OSD_FSETS( CTRL1, PATEN, DISABLE );
        OSD_ENSURE( OSD_FGET( CTRL1, PATEN ) == OSD_CTRL1_PATEN_DISABLE );
    }
}


/*---------------------------------------------------------------------------*/
void
OSD_SetTtCharsetBitmapAddress   (OSD_Address_t addr)
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
    _OSD_CSBMPST_TYPE flashAddr = TO_FLASH_ADDR( TTX_FONT_BANK, addr );
    OSD_REQUIRE( flashAddr % 4 == 0 ); // font address must be four-byte-algigned.
    OSD_Log( LOGGER_VERBOSE, "[OSD] " "Setting font address: %x\n", (uint32_t)flashAddr);
    OSD_FSET( CSBMPST, CSBMPST, OSD_CSBMPST_RMK( flashAddr ) );
    OSD_ENSURE( OSD_FGET( CSBMPST, CSBMPST ) == flashAddr );
}

/*---------------------------------------------------------------------------*/
#if 0 // not used
OSD_Address_t
OSD_GetTtCharsetBitmapAddress   (void)
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
    return( OSD_FGET( CSBMPST, CSBMPST ) & 0x0000FFFF );    /* AND 0x0000FFFF to get only the address, no bank number */
}
#endif

/*---------------------------------------------------------------------------*/
void
OSD_SetTtAttr1Address  (OSD_Address_t addr)
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
    OSD_FSET( ATTST1, ATTST, OSD_ATTST1_RMK( addr ) );

    /* This is a latch-double-flip-flop register, cannot be verify above writes immediately here. */
}

/*---------------------------------------------------------------------------*/
OSD_Address_t
OSD_GetTtAttr1Address  (void)
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
//    return( OSD_RegRead( VBIOSD_ATTST1_REG_ADDR ) );
    return( OSD_FGET( ATTST1, ATTST ) );
}

/*---------------------------------------------------------------------------*/
void
OSD_SetTtR25E                   (bool_t bEnable)
/*****************************************************************************\
 *  @brief   Enable/Disable the display of row 25 in teletext.
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
    if ( bEnable == TRUE )
        OSD_FSETS( CTRL1, TTR25E, ENABLE  );
    else
        OSD_FSETS( CTRL1, TTR25E, DISABLE );

    OSD_ENSURE( OSD_FGET( CTRL1, TTR25E ) == bEnable );
}

/*---------------------------------------------------------------------------*/
bool_t
OSD_GetTtR25E                   (void)
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
    return( OSD_FGET(CTRL1, TTR25E) == OSD_CTRL1_TTR25E_ENABLE );
}

/*---------------------------------------------------------------------------*/
void
OSD_SetTtVsr    (uint8_t rate, uint8_t rowfix)
/*****************************************************************************\
 *  @brief   Set vertical scaling rate and vertical scaling mode in teletext.
 *  @param   rate   OSD_CTRL3_VSR_MODE0
 *                  OSD_CTRL3_VSR_MODE1
 *                  OSD_CTRL3_VSR_MODE2
 *  @param   rowfix OSD_CTRL3_TTVS_MODE0
 *                  OSD_CTRL3_TTVS_MODE1
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
    OSD_FSET( CTRL3, VSR , rate );
    OSD_FSET( CTRL3, TTVS, rowfix );

    OSD_ENSURE( OSD_FGET( CTRL3, VSR  ) == rate   );
    OSD_ENSURE( OSD_FGET( CTRL3, TTVS ) == rowfix );
}

#if OSD_VERIFY  /* if enabled verficiation of TTX related IPs */

/*---------------------------------------------------------------------------*/
void
OSD_SetTtHsr    (uint8_t rate)
/*****************************************************************************\
 *  @brief   Set the horizontal scaling rate of specified page in teletext.
 *  @param   rate OSD_P1CTRL_HSR_1
 *                OSD_P1CTRL_HSR_1OVER2
 *                OSD_P1CTRL_HSR_7OVER5
 *                OSD_P1CTRL_HSR_7OVER10
\*****************************************************************************/
{
    OSD_FSET( P1CTRL, HSR , rate );

    /* This is a latch-double-flip-flop register, cannot be verify above writes immediately here. */
}
#endif  /* end of #if OSD_VERIFY */

/*---------------------------------------------------------------------------*/
void
OSD_SetTtMultipageMode    (bool_t bEnable)
/*****************************************************************************\
 *  @brief   Set the horizontal scaling rate of specified page in teletext.
 *  @param   rate OSD_P1CTRL_HSR_1
 *                OSD_P1CTRL_HSR_1OVER2
 *                OSD_P1CTRL_HSR_7OVER5
 *                OSD_P1CTRL_HSR_7OVER10
\*****************************************************************************/
{
    bEnable = FALSE; // eliminate compiler warning
    OSD_FSETS( CTRL1, TTMODE , 1P ); // Currently, we support one-page only

    OSD_ENSURE( OSD_FGET( CTRL1, TTMODE ) == OSD_CTRL1_TTMODE_1P );
}

/*---------------------------------------------------------------------------*/
void
OSD_SetTtSmooth                   (bool_t bEnable)
/*****************************************************************************\
 *  @brief   Enable/Disable character smooth in teletext.
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
    if ( bEnable == TRUE )
        OSD_FSETS( CTRL3, SMEN, ENABLE  );
    else
        OSD_FSETS( CTRL3, SMEN, DISABLE );

    OSD_ENSURE( OSD_FGET( CTRL3, SMEN ) == bEnable );
}

#if OSD_VERIFY  /* if enabled verficiation of TTX related IPs */

/*---------------------------------------------------------------------------*/
void
OSD_SetTtConceal                  (bool_t bEnable)
/*****************************************************************************\
 *  @brief   Enable/Disable conceal in teletext.
 *  @param   bEnable  Set '1' to enable conceal.
 *                    set '0' to disable conceal.
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
    if ( bEnable == TRUE )
        OSD_FSETS( CTRL3, CNCDIS, ENABLE  );
    else
        OSD_FSETS( CTRL3, CNCDIS, DISABLE );

    OSD_ENSURE( OSD_FGET( CTRL3, CNCDIS ) == bEnable );
}

#endif  /* end of #if OSD_VERIFY */

/*---------------------------------------------------------------------------*/
void
OSD_SetTtR0G1Set                (uint8_t cset)
/*****************************************************************************\
 *  @brief   Set header default G1 character set in teletext.
 *  @param   bEnable  Set '1' to enable conceal.
 *                    set '0' to disable conceal.
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
    OSD_FSET( CTRL5, HDG1CS, cset  );

    OSD_ENSURE( OSD_FGET( CTRL5, HDG1CS ) == cset );
}

/*---------------------------------------------------------------------------*/
void
OSD_SetTtMosaicSeparatedMask    (uint16_t mlm, uint16_t mbm)
/*****************************************************************************\
 *  @brief   Set mosaic character bit and line masks in teletext.
 *  @param   mlm  the line mask to set
 *  @param   mbm  the bit mask to set
\*****************************************************************************/
{
    OSD_RSET( CTRL2, OSD_CTRL2_RMK( mbm,   /* MBM */  \
                                    mlm    /* MLM */  \
                                  ) );

    OSD_ENSURE( OSD_RGET( CTRL2 ) == OSD_CTRL2_RMK( mbm,   /* MBM */  \
                mlm    /* MLM */  \
                                                  ) );
}

#if OSD_VERIFY  /* if enabled verficiation of TTX related IPs */

/*---------------------------------------------------------------------------*/
void
OSD_GetTtMosaicSeparatedMask    (uint16_t* mlm, uint16_t* mbm)
/*****************************************************************************\
 *  @brief   Get mosaic character bit and line masks in teletext.
 *  @param   mlm  the line mask to set
 *  @param   mbm  the bit mask to set
\*****************************************************************************/
{
    if ( mlm != NULL )
    {
        *mlm = OSD_FGET( CTRL2, MLM );
    }

    if ( mbm != NULL )
    {
        *mbm = OSD_FGET( CTRL2, MBM );
    }
}
#endif  /* end of #if OSD_VERIFY */


/*---------------------------------------------------------------------------*/
void
OSD_SetTtDisplayEnable    (bool_t bEnable)
/*****************************************************************************\
 *  @brief   Enable/Disable the display of the specified page in teletext.
 *  @param   bEnable Set '1' to enable, or set '0' to disable.
\*****************************************************************************/
{
    if ( bEnable == TRUE )
    {
        OSD_FSETS( P1CTRL, DE, ENABLE  );   /* This is a latch-double-flip-flop register, cannot be verify above writes immediately here. */

        _OSD_REG_WRITE( 0x4734, _OSD_REG_READ( 0x4734 ) | UINT32_C(0x00000001) ); /* enable mixer */
    }
    else
    {
        OSD_FSETS( P1CTRL, DE, DISABLE );   /* This is a latch-double-flip-flop register, cannot be verify above writes immediately here. */

        _OSD_REG_WRITE( 0x4734, _OSD_REG_READ( 0x4734 ) & ~UINT32_C(0x00000001) ); /* enable mixer */

    }
}

/*---------------------------------------------------------------------------*/
bool_t
OSD_isDisplaying    (void)
/*****************************************************************************\
 *  @brief   Enable/Disable the display of the specified page in teletext.
 *  @param   bEnable Set '1' to enable, or set '0' to disable.
\*****************************************************************************/
{
    return(    ( OSD_FGET( P1CTRL, DE ) ==  OSD_P1CTRL_DE_ENABLE )
               && ( _OSD_REG_READ( 0x4734 ) & UINT32_C(0x00000001) ) );
}

/*---------------------------------------------------------------------------*/
void
OSD_SetTtBoxEnable    (bool_t bEnable)
/*****************************************************************************\
 *  @brief   Enable box area or window area of the specified page in teletext.
 *  @param   Set '1' to enable box area, or set '0' to enable window area.
\*****************************************************************************/
{
    if ( bEnable == TRUE )
        OSD_FSETS( P1CTRL, BOX, BOX    );
    else
        OSD_FSETS( P1CTRL, BOX, WINDOW );

    /* This is a latch-double-flip-flop register, cannot be verify above writes immediately here. */
}

/*---------------------------------------------------------------------------*/
bool_t
OSD_GetTtBoxEnable    (void)
/*****************************************************************************\
 *  @brief   Enable box area or window area of the specified page in teletext.
 *  @param   Set '1' to enable box area, or set '0' to enable window area.
\*****************************************************************************/
{
    return( OSD_FGET( P1CTRL, BOX ) == OSD_P1CTRL_BOX_BOX );
    /* This is a latch-double-flip-flop register, cannot be verify above writes immediately here. */
}

/*---------------------------------------------------------------------------*/
void
OSD_SetTtRollingEnable    (bool_t bEnable)
/*****************************************************************************\
 *  @brief   Enable/Disable the display of rolling header of the specified page in teletext.
 *  @param   Set '1' to enable, or set '0' to disable.
\*****************************************************************************/
{
    if ( bEnable == TRUE )
    {
        OSD_FSETS( P1CTRL, RHDE, ENABLE  );
        rtd_outl(VBIOSD_CTRL5_REG_0x4810, 0x00000200); // G0 in font position 0, G1 in font position 2
    }
    else
        OSD_FSETS( P1CTRL, RHDE, DISABLE );

    /* This is a latch-double-flip-flop register, cannot be verify above writes immediately here. */
}

/*---------------------------------------------------------------------------*/
void
OSD_SetTtTimecodeEnable    (bool_t bEnable)
/*****************************************************************************\
 *  @brief   Enable/Disable the display of time code of the specified page in teletext.
 *  @param   Set '1' to enable, or set '0' to disable.
\*****************************************************************************/
{
    if ( bEnable == TRUE )
        OSD_FSETS( P1CTRL, TCDE, ENABLE  );
    else
        OSD_FSETS( P1CTRL, TCDE, DISABLE );

    /* This is a latch-double-flip-flop register, cannot be verify above writes immediately here. */
}

/*---------------------------------------------------------------------------*/
void
OSD_SetTtR0Enable    (bool_t bEnable)
/*****************************************************************************\
 *  @brief   Enable/Disable the display of row 0 of the specified page in teletext.
 *  @param   Set '1' to enable, or set '0' to disable.
\*****************************************************************************/
{
    if ( bEnable == TRUE )
    {
        OSD_FSETS( P1CTRL, RHDE, ENABLE  );
        OSD_FSETS( P1CTRL, TCDE, ENABLE  );
    }
    else
    {
        OSD_FSETS( P1CTRL, RHDE, DISABLE );
        OSD_FSETS( P1CTRL, TCDE, DISABLE );
    }

    /* This is a latch-double-flip-flop register, cannot be verify above writes immediately here. */
}

#if 0   /* THIS_FUNCTION is not used */

/*---------------------------------------------------------------------------*/
void
OSD_SetTtR0DhDwConfig           (bool_t extForDh, bool_t dwEnable, bool_t dhEnable, bool_t dwMode, bool_t setSpace)
/*****************************************************************************\
 *  @brief   Set the config of row 0 in teletext.
 *  @param	extForDh	: [input] whether header can be extended for DH or not
 *  @param	dwEnable	: [input] whether header allows double width or not
 *  @param	dhEnable	: [input] whether header allows double high or not
 *  @param	dwMode		: [input] the DW mode for header. Set '0' for Micronas mode, or set '1' for Philips mode.
 *  @param	setSpace	: [input] whether row 1 allows to be set space bellow page number when row 0 has a DH
\*****************************************************************************/
{
    if ( extForDh == TRUE )
        OSD_FSETS( CTRL5, HDREXT, ENABLE  );
    else
        OSD_FSETS( CTRL5, HDREXT, DISABLE );

    OSD_ENSURE( OSD_FGET( CTRL5, HDREXT ) == extForDh );


    if ( dwEnable == TRUE )
        OSD_FSETS( CTRL5, HDDWEN, ENABLE  );
    else
        OSD_FSETS( CTRL5, HDDWEN, DISABLE );

    OSD_ENSURE( OSD_FGET( CTRL5, HDDWEN ) == dwEnable );


    if ( dhEnable == TRUE )
        OSD_FSETS( CTRL5, HDDHEN, ENABLE  );
    else
        OSD_FSETS( CTRL5, HDDHEN, DISABLE );

    OSD_ENSURE( OSD_FGET( CTRL5, HDDHEN ) == dhEnable );


    if ( dwMode == TRUE )
        OSD_FSETS( CTRL5, HDDWMD, PHILIPS );
    else
        OSD_FSETS( CTRL5, HDDWMD, MICRONAS );

    OSD_ENSURE( OSD_FGET( CTRL5, HDDWMD ) == dwMode );


    if ( setSpace == TRUE )
        OSD_FSETS( CTRL5, HDPNSP, ENABLE  );
    else
        OSD_FSETS( CTRL5, HDPNSP, DISABLE );

    OSD_ENSURE( OSD_FGET( CTRL5, HDPNSP ) == setSpace );
}

/*---------------------------------------------------------------------------*/
void
OSD_SetTtR0DhCmdEnable          (bool_t bEnable)
/*****************************************************************************\
 *  @brief   Enable/Disable the "double height" command in row 0 in teletext.
 *  @param   Set '1' to enable, or set '0' to disable.
\*****************************************************************************/
{
    if ( bEnable == TRUE )
        OSD_FSETS( CTRL5, DHCMDEN, ENABLE  );
    else
        OSD_FSETS( CTRL5, DHCMDEN, DISABLE );

    OSD_ENSURE( OSD_FGET( CTRL5, DHCMDEN ) == bEnable );
}

/*---------------------------------------------------------------------------*/
void
OSD_SetTtR0DwCmdEnable          (bool_t bEnable)
/*****************************************************************************\
 *  @brief   Enable/Disable the "double width" command in row 0 in teletext.
 *  @param   Set '1' to enable, or set '0' to disable.
\*****************************************************************************/
{
    if ( bEnable == TRUE )
        OSD_FSETS( CTRL5, DWCMDEN, ENABLE  );
    else
        OSD_FSETS( CTRL5, DWCMDEN, DISABLE );

    OSD_ENSURE( OSD_FGET( CTRL5, DWCMDEN ) == bEnable );
}

/*---------------------------------------------------------------------------*/
void
OSD_SetTtR0DsCmdEnable          (bool_t bEnable)
/*****************************************************************************\
 *  @brief   Enable/Disable the "double size" command in row 0 in teletext.
 *  @param   Set '1' to enable, or set '0' to disable.
\*****************************************************************************/
{
    if ( bEnable == TRUE )
        OSD_FSETS( CTRL5, DSCMDEN, ENABLE  );
    else
        OSD_FSETS( CTRL5, DSCMDEN, DISABLE );

    OSD_ENSURE( OSD_FGET( CTRL5, DSCMDEN ) == bEnable );
}

#endif  /* end of #if 0 */

/*---------------------------------------------------------------------------*/
void
OSD_SetTtLspNo    (uint8_t numOfCols)
/*****************************************************************************\
 *  @brief   Set the number columns in left side panel of the specified page in teletext.
 *  @param   Set '1' to enable, or set '0' to disable.
\*****************************************************************************/
{
    OSD_FSET( P1CTRL, LSPNUM, numOfCols );

    /* This is a latch-double-flip-flop register, cannot be verify above writes immediately here. */
}

/*---------------------------------------------------------------------------*/
void
OSD_SetTtSidepanelEn    (bool_t bEnable)
/*****************************************************************************\
 *  @brief   Enable/Disable the display of side panel of the specified page in teletext.
 *  @param   Set '1' to enable, or set '0' to disable.
\*****************************************************************************/
{
    if ( bEnable == TRUE )
        OSD_FSETS( P1CTRL, SPO, ON  );
    else
        OSD_FSETS( P1CTRL, SPO, OFF );

    /* This is a latch-double-flip-flop register, cannot be verify above writes immediately here. */
}

/*---------------------------------------------------------------------------*/
bool_t
OSD_GetTtSidepanelEn            (void)
/*****************************************************************************\
 *  @brief   The display of side panel of the specified page in teletext is enabled or disabled.
 *  @ret     Whether the display of side panel of the specified page is enabled or not
\*****************************************************************************/
{
    return( OSD_FGET(P1CTRL, SPO) == OSD_P1CTRL_SPO_ON );
}

/*---------------------------------------------------------------------------*/
void
OSD_SetTtScreenColor            (uint8_t clut_entry)
/*****************************************************************************\
 *  @brief   Set the screen color of the specified page in teletext.
 *  @param   clut_entry	: [input] the color value of the screen color to set
 *           clut_entry = { clut[5,4], entry[2..0] }
\*****************************************************************************/
{
    // In Sparrow, we have only one CLUT, i.e., CLUT-0.
    OSD_FSET(P1CTRL, SCT, 0 );

    OSD_FSET(P1CTRL, SCE, clut_entry & 0x07 );

    /* This is a latch-double-flip-flop register, cannot be verify above writes immediately here. */
}

/*---------------------------------------------------------------------------*/
void
OSD_SetTtRowColorIndex          (uint8_t row, uint8_t clut_entry)
/*****************************************************************************\
 *  @brief   Set the row color of the specified row in the specified page in teletext.
 *  @param   row	: [input] the row of which the row color is set
 *  @param   clut_entry	: [input] the color value of the row color to set
\*****************************************************************************/
{
    OSD_REQUIRE( ( 0 <= row ) && ( row <= 25 ) );
    switch ( row )
    {
    case 0:
    {
        OSD_FSET( P1_PARA1, R0DC, clut_entry );
        break;
    }

    case 1:
    {
        OSD_FSET( P1_PARA1, R1DC, clut_entry );
        break;
    }

    case 2:
    {
        OSD_FSET( P1_PARA1, R2DC, clut_entry );
        break;
    }

    case 3:
    {
        OSD_FSET( P1_PARA1, R3DC, clut_entry );
        break;
    }

    case 4:
    {
        OSD_FSET( P1_PARA2, R4DC, clut_entry );
        break;
    }

    case 5:
    {
        OSD_FSET( P1_PARA2, R5DC, clut_entry );
        break;
    }

    case 6:
    {
        OSD_FSET( P1_PARA2, R6DC, clut_entry );
        break;
    }

    case 7:
    {
        OSD_FSET( P1_PARA2, R7DC, clut_entry );
        break;
    }

    case 8:
    {
        OSD_FSET( P1_PARA3, R8DC, clut_entry );
        break;
    }

    case 9:
    {
        OSD_FSET( P1_PARA3, R9DC, clut_entry );
        break;
    }

    case 10:
    {
        OSD_FSET( P1_PARA3, R10DC, clut_entry );
        break;
    }

    case 11:
    {
        OSD_FSET( P1_PARA3, R11DC, clut_entry );
        break;
    }

    case 12:
    {
        OSD_FSET( P1_PARA4, R12DC, clut_entry );
        break;
    }

    case 13:
    {
        OSD_FSET( P1_PARA4, R13DC, clut_entry );
        break;
    }

    case 14:
    {
        OSD_FSET( P1_PARA4, R14DC, clut_entry );
        break;
    }

    case 15:
    {
        OSD_FSET( P1_PARA4, R15DC, clut_entry );
        break;
    }

    case 16:
    {
        OSD_FSET( P1_PARA5, R16DC, clut_entry );
        break;
    }

    case 17:
    {
        OSD_FSET( P1_PARA5, R17DC, clut_entry );
        break;
    }

    case 18:
    {
        OSD_FSET( P1_PARA5, R18DC, clut_entry );
        break;
    }

    case 19:
    {
        OSD_FSET( P1_PARA5, R19DC, clut_entry );
        break;
    }

    case 20:
    {
        OSD_FSET( P1_PARA6, R20DC, clut_entry );
        break;
    }

    case 21:
    {
        OSD_FSET( P1_PARA6, R21DC, clut_entry );
        break;
    }

    case 22:
    {
        OSD_FSET( P1_PARA6, R22DC, clut_entry );
        break;
    }

    case 23:
    {
        OSD_FSET( P1_PARA6, R23DC, clut_entry );
        break;
    }

    case 24:
    {
        OSD_FSET( P1_PARA7, R24DC, clut_entry );
        break;
    }

    case 25:
    {
        OSD_FSET( P1_PARA7, R25DC, clut_entry );
        break;
    }

    default:
    {
        OSD_ERROR();    // ERROR: Shouldn't run here.
        break;
    }
    }

    /* This is a latch-double-flip-flop register, cannot be verify above writes immediately here. */
}

/*---------------------------------------------------------------------------*/
void
OSD_SetTtPageNumberSeq  (bool_t enable, uint8_t csel, uint8_t fg, uint8_t bg, const char *str)
/*****************************************************************************\
 *  @brief   The display of side panel of the specified page in teletext is enabled or disabled.
 *  @param   csel   character set selection ( 0~ OSD_MAX_CHAR_SET_NUM-1 )
 *  @ret     Whether the display of side panel of the specified page is enabled or not
\*****************************************************************************/
{
    OSD_REQUIRE( csel < OSD_MAX_CHAR_SET_NUM );

    OSD_RSET( P1PNUM_CINDEX1, OSD_P1PNUM_CINDEX1_RMK(str[0],   /*  C0  */ \
              str[1],   /*  C1  */ \
              str[2],   /*  C2  */ \
              str[3]    /*  C3  */ \
                                                    ) );

    OSD_RSET( P1PNUM_CINDEX2, OSD_P1PNUM_CINDEX2_RMK(str[4],   /*  C4  */ \
              str[5],   /*  C5  */ \
              str[6],   /*  C6  */ \
              str[7]    /*  C7  */ \
                                                    ) );

    OSD_RSET( P1PNUM_CTRL, OSD_P1PNUM_CTRL_RMK( fg  ,   /*   FGC   */ \
              bg  ,   /*   BGC   */ \
              csel ,   /*   CS    */ \
              enable   /*   DE    */ \
                                              ) );

    // Verify above register writes
    {
        OSD_ENSURE( OSD_RGET( P1PNUM_CINDEX1 ) == OSD_P1PNUM_CINDEX1_RMK(str[0],   /*  C0  */ \
                    str[1],   /*  C1  */ \
                    str[2],   /*  C2  */ \
                    str[3]    /*  C3  */ \
                                                                        ) );

        OSD_ENSURE( OSD_RGET( P1PNUM_CINDEX2 ) == OSD_P1PNUM_CINDEX2_RMK(str[4],   /*  C4  */ \
                    str[5],   /*  C5  */ \
                    str[6],   /*  C6  */ \
                    str[7]    /*  C7  */ \
                                                                        ) );

        OSD_ENSURE( OSD_RGET( P1PNUM_CTRL ) == OSD_P1PNUM_CTRL_RMK( fg  ,   /*   FGC   */ \
                    bg  ,   /*   BGC   */ \
                    csel ,   /*   CS    */ \
                    enable   /*   DE    */ \
                                                                  ) );
    }
}

#if 0   /* THIS_FUNCTION is not used */

/*---------------------------------------------------------------------------*/
void
OSD_GetTtPageNumber             (OSD_PageNum_t* pn)
/*****************************************************************************\
 *  @brief   Get the attributes of the page number in the specified page in teletext.
 *  @param   pn    [output] the attributes structure of the page number in the specified page
\*****************************************************************************/
{
    pn->str[0] = OSD_FGET( P1PNUM_CINDEX1, C0 );
    pn->str[1] = OSD_FGET( P1PNUM_CINDEX1, C1 );
    pn->str[2] = OSD_FGET( P1PNUM_CINDEX1, C2 );
    pn->str[3] = OSD_FGET( P1PNUM_CINDEX1, C3 );
    pn->str[4] = OSD_FGET( P1PNUM_CINDEX2, C4 );
    pn->str[5] = OSD_FGET( P1PNUM_CINDEX2, C5 );
    pn->str[6] = OSD_FGET( P1PNUM_CINDEX2, C6 );
    pn->str[7] = OSD_FGET( P1PNUM_CINDEX2, C7 );

    pn->csel    = OSD_FGET( P1PNUM_CTRL, CS  );
    pn->fg      = OSD_FGET( P1PNUM_CTRL, FGC );
    pn->bg      = OSD_FGET( P1PNUM_CTRL, BGC );
    pn->bEnable = OSD_FGET( P1PNUM_CTRL, DE  );


    OSD_REQUIRE( pn->csel < OSD_MAX_CHAR_SET_NUM );
}
#endif

/*---------------------------------------------------------------------------*/
void
OSD_SetTtMessageSeq (bool_t enable, uint8_t csel, uint8_t fg, uint8_t bg, bool_t o, bool_t u,
                     bool_t i, bool_t fl, uint8_t row, uint8_t col, const char* str)
/*****************************************************************************\
 * @param	enable		: [input] Set '1' to enable message, or set '0' to disable.
 * @param	inpage		: [input] the page in which the message is displayed
 * @param	csel		: [input] the character set of the message
 * @param	fg			: [input] the foreground color of the message
 * @param	bg			: [input] the background color of the message
 * @param	o			: [input] add an overline to the message or not
 * @param	u			: [input] add an underline to the message or not
 * @param	i			: [input] the characters in the message is italic or not
 * @param	fl			: [input] flash the message or not
 * @param	row			: [input] the row of the message
 * @param	column		: [input] the start column of the message
 * @param	str			: [input] the content of the message
\*****************************************************************************/
{
    const uint8_t psel = OSD_P1MSG_CTRL1_PS_P1; // Currently, only page-1 is supported.

    uint8_t maxRowNum, maxRowLen;
    size_t  len;

    if ( csel >= OSD_MAX_CHAR_SET_NUM )
    {
        OSD_Log( LOGGER_WARN, "[OSD][Warning]Invalid character set %d of message. Set it to %d\n",
                 (uint16_t)csel, (uint16_t)OSD_MAX_CHAR_SET_NUM-1);
        csel = OSD_MAX_CHAR_SET_NUM - 1;
    }

    len = strlen( str );

    OSD_REQUIRE( len <= OSD_MAX_MSG_LEN );

    if ( len == 0 )
    {
        enable = FALSE;
    }
    else if ( len >= OSD_MAX_MSG_LEN )
    {
        len = OSD_MAX_MSG_LEN;
    }

    if ( OSD_GetTtR25E() )
    {
        maxRowNum = TTX_DISP_MAX_ROW_NUM;
    }
    else
    {
        maxRowNum = TTX_DISP_MAX_ROW_NUM - 1;
    }


    if ( row >= maxRowNum )
    {
        OSD_Log( LOGGER_WARN, "[OSD][Warning]Invalid message row (%d). Set it to %d\n", (uint16_t)row, (uint16_t)maxRowNum-1);
        row = maxRowNum - 1;
    }

    if ( OSD_GetTtSidepanelEn() )
    {
        maxRowLen = TTX_DISP_MAX_ROW_LEN;
    }
    else
    {
        maxRowLen = TTX_DISP_MAX_ROW_LEN_WITHOUT_SP;
    }

    if ( col >= maxRowLen )
    {
        OSD_Log( LOGGER_WARN, "[OSD][Warning]Invalid message first column (%d). Set it to %d\n",
                 (uint16_t)col, (uint16_t)maxRowLen-1);
    }

    if ((col + len - 1) >= maxRowLen)
    {
        OSD_Log( LOGGER_WARN, "[OSD][Warning]Invalid message last column (%d). Set it to %d\n",
                 (uint16_t)col+len-1, (uint16_t)maxRowLen-1);
        len = maxRowLen - col;
    }

    OSD_RSET( P1MSG_CTRL2, OSD_P1MSG_CTRL2_RMK( len   ,   /*   LEN   */ \
              row   ,   /*   ROW   */ \
              col       /*   COL   */ \
                                              ) );

    OSD_RSET( P1MSG_CINDEX1, OSD_P1MSG_CINDEX1_RMK(str[0],   /*  C0  */ \
              str[1],   /*  C1  */ \
              str[2],   /*  C2  */ \
              str[3]    /*  C3  */ \
                                                  ) );

    OSD_RSET( P1MSG_CINDEX2, OSD_P1MSG_CINDEX2_RMK(str[4],   /*  C4  */ \
              str[5],   /*  C5  */ \
              str[6],   /*  C6  */ \
              str[7]    /*  C7  */ \
                                                  ) );

    OSD_RSET( P1MSG_CINDEX3, OSD_P1MSG_CINDEX3_RMK(str[8],   /*  C8  */ \
              str[9]    /*  C9  */ \
                                                  ) );

    OSD_RSET( P1MSG_CTRL1, OSD_P1MSG_CTRL1_RMK(fg     ,   /*   FGC   */ \
              bg     ,   /*   BGC   */ \
              csel   ,   /*   CS    */ \
              psel   ,   /*   PS    */ \
              o      ,   /*   OL    */ \
              u      ,   /*   UL    */ \
              i      ,   /*   IT    */ \
              fl     ,   /*   FL    */ \
              enable     /*   DE    */ \
                                              ) );



    // Verify above register writes.
    {
        OSD_ENSURE( OSD_RGET( P1MSG_CTRL2 ) == OSD_P1MSG_CTRL2_RMK( len   ,   /*   LEN   */ \
                    row   ,   /*   ROW   */ \
                    col       /*   COL   */ \
                                                                  ) );

        OSD_ENSURE( OSD_RGET( P1MSG_CINDEX1 ) == OSD_P1MSG_CINDEX1_RMK(str[0],   /*  C0  */ \
                    str[1],   /*  C1  */ \
                    str[2],   /*  C2  */ \
                    str[3]    /*  C3  */ \
                                                                      ) );

        OSD_ENSURE( OSD_RGET( P1MSG_CINDEX2 ) == OSD_P1MSG_CINDEX2_RMK(str[4],   /*  C4  */ \
                    str[5],   /*  C5  */ \
                    str[6],   /*  C6  */ \
                    str[7]    /*  C7  */ \
                                                                      ) );

        OSD_ENSURE( OSD_RGET( P1MSG_CINDEX3 ) == OSD_P1MSG_CINDEX3_RMK(str[8],   /*  C8  */ \
                    str[9]    /*  C9  */ \
                                                                      ) );

        OSD_ENSURE( OSD_RGET( P1MSG_CTRL1 ) == OSD_P1MSG_CTRL1_RMK(fg     ,   /*   FGC   */ \
                    bg     ,   /*   BGC   */ \
                    csel   ,   /*   CS    */ \
                    psel   ,   /*   PS    */ \
                    o      ,   /*   OL    */ \
                    u      ,   /*   UL    */ \
                    i      ,   /*   IT    */ \
                    fl     ,   /*   FL    */ \
                    enable     /*   DE    */ \
                                                                  ) );
    }
}

#if OSD_VERIFY  /* if enabled verficiation of TTX related IPs */

/*---------------------------------------------------------------------------*/
void
OSD_GetTtMessage                (OSD_Message_t* msg)
/*****************************************************************************\
 *  @brief   Get the attributes of a message in teletext.
\*****************************************************************************/
{
    if ( msg != NULL )
    {
        OSD_REQUIRE( OSD_FGET( P1MSG_CTRL1, PS ) == OSD_P1MSG_CTRL1_PS_P1 ); // Currently, only page-1 is supported.

        msg->enable = OSD_FGET( P1MSG_CTRL1, DE  );
        msg->fg     = OSD_FGET( P1MSG_CTRL1, FGC );
        msg->bg     = OSD_FGET( P1MSG_CTRL1, BGC );
        msg->csel   = OSD_FGET( P1MSG_CTRL1, CS  );
        msg->o      = OSD_FGET( P1MSG_CTRL1, OL  );
        msg->u      = OSD_FGET( P1MSG_CTRL1, UL  );
        msg->i      = OSD_FGET( P1MSG_CTRL1, IT  );
        msg->fl     = OSD_FGET( P1MSG_CTRL1, FL  );

        msg->len    = OSD_FGET( P1MSG_CTRL2, LEN );
        msg->row    = OSD_FGET( P1MSG_CTRL2, ROW );
        msg->col    = OSD_FGET( P1MSG_CTRL2, COL );

        msg->str[0] = OSD_FGET( P1MSG_CINDEX1, C0 );
        msg->str[1] = OSD_FGET( P1MSG_CINDEX1, C1 );
        msg->str[2] = OSD_FGET( P1MSG_CINDEX1, C2 );
        msg->str[3] = OSD_FGET( P1MSG_CINDEX1, C3 );

        msg->str[4] = OSD_FGET( P1MSG_CINDEX2, C4 );
        msg->str[5] = OSD_FGET( P1MSG_CINDEX2, C5 );
        msg->str[6] = OSD_FGET( P1MSG_CINDEX2, C6 );
        msg->str[7] = OSD_FGET( P1MSG_CINDEX2, C7 );

        msg->str[8] = OSD_FGET( P1MSG_CINDEX3, C8 );
        msg->str[9] = OSD_FGET( P1MSG_CINDEX3, C9 );
    }
}
#endif  /* end of #if OSD_VERIFY */

/*---------------------------------------------------------------------------*/
void
OSD_SetTtAllDiacriticalmarkBitmap   (void)
/*****************************************************************************\
 *  @brief   Set the bitmaps of all diacriticalmarks in teletext.
\*****************************************************************************/
{
#if 1 // rewrite for efficiency
    static uint32_t const TTX_ROM OSD_ttDmBitmap[OSD_MAX_DM_NUM+2] =
    {
        0x00000000, // DM_MSB00
        0x06003000, // DM_MSB01
        0x03006000, // DM_MSB02
        0x0700d800, // DM_MSB03
        0x1c423800, // DM_MSB04
        0x3fc00000, // DM_MSB05
        0x1980f000, // DM_MSB06
        0x06000000, // DM_MSB07
        0x19800000, // DM_MSB08
        0x00000006, // DM_MSB09
        0x0f01980f, // DM_MSB10
        0x00002006, // DM_MSB11
        0x0000007f, // DM_MSB12
        0x0cc19800, // DM_MSB13
        0x00004006, // DM_MSB14
        0x19806000, // DM_MSB15
        0x00000000, // DM_LSB0 (DM 0~7)
        0x0000e000, // DM_LSB1 (DM 8~15)
    };

    TTX_Address_t addr;
    uint8_t i;

    for ( i=0, addr=OSD_ADDR(DM_MSB00) ; i<OSD_MAX_DM_NUM+2 ; ++i, addr+=4 )
        OSD_RegWrite(addr, OSD_ttDmBitmap[i]);

#else
    static uint16_t const TTX_ROM OSD_ttDmBitmap[OSD_MAX_DM_NUM][OSD_MAX_DM_LINE_NUM] =
    {
        { 0x000, 0x000, 0x000 }, // DM 00
        { 0x060, 0x030, 0x000 }, // DM 01
        { 0x030, 0x060, 0x000 }, // DM 02
        { 0x070, 0x0d8, 0x000 }, // DM 03
        { 0x1c4, 0x238, 0x000 }, // DM 04
        { 0x3fc, 0x000, 0x000 }, // DM 05
        //{ 0x198, 0x060, 0x000 },
        { 0x198, 0x0f0, 0x000 }, // DM 06
        { 0x060, 0x000, 0x000 }, // DM 07
        { 0x198, 0x000, 0x000 }, // DM 08
        { 0x000, 0x000, 0x060 }, // DM 09
        { 0x0f0, 0x198, 0x0f0 }, // DM 10
        { 0x000, 0x020, 0x060 }, // DM 11
        { 0x000, 0x000, 0x7fe }, // DM 12
        { 0x0cc, 0x198, 0x000 }, // DM 13
        { 0x000, 0x040, 0x060 }, // DM 14
        //{ 0x0d8, 0x070, 0x000 }
        { 0x198, 0x060, 0x000 } // DM 15
    };

    if ( OSD_ttDmBitmap != NULL )
    {
        const uint8_t DM_LINE0_L_OFFSET =   20;     // the left shift offset of line 0 of a diacritical mark
        const uint8_t DM_LINE1_L_OFFSET =    8;     // the left shift offset of line 1 of a diacritical mark
        const uint8_t DM_LINE2_R_OFFSET =    4;     // the right shift offset of line 2 of a diacritical mark
        const uint8_t DM_LINE2_MASK     = 0x0F;     // the bit mask of line 2 of a diacritical mark

        _OSD_DM_MSB00_TYPE val;
        TTX_Address_t addr;
        uint32_t lsbRegLineNum;
        size_t i;

        addr = OSD_ADDR( DM_MSB00 );
        for ( i = 0; i < OSD_MAX_DM_NUM; i++ )
        {
            val = ((uint32_t)(OSD_ttDmBitmap[i][0])<<DM_LINE0_L_OFFSET) | ((uint32_t)(OSD_ttDmBitmap[i][1])<<DM_LINE1_L_OFFSET)  |  \
                  ((uint32_t)(OSD_ttDmBitmap[i][2])>>DM_LINE2_R_OFFSET);
            OSD_RegWrite(addr, val);
            addr+=4;
        }
        val = 0;
        addr = OSD_ADDR( DM_LSB0 );
        lsbRegLineNum = 32 / DM_LINE2_R_OFFSET;
        for ( i = 0; i < OSD_MAX_DM_NUM; i++ )
        {
            val = (val<<DM_LINE2_R_OFFSET) | (OSD_ttDmBitmap[i][2]&DM_LINE2_MASK);
            if ( i % lsbRegLineNum == lsbRegLineNum - 1 )
            {
                OSD_RegWrite(addr, val);
                addr += 4;
                val = 0;
            }
        }
    }
#endif
}

/*---------------------------------------------------------------------------*/
void
OSD_SetTtScaleCoefficient       (const uint32_t coeff[OSD_MAX_COFF_NUM])
/*****************************************************************************\
 *  @brief   Set the scaler coefficients in teletext.
\*****************************************************************************/
{
    OSD_ASSERT_COMPILE( OSD_ADDR( COEF1 ) - OSD_ADDR( COEF0 ) == 4 );
    OSD_ASSERT_COMPILE( OSD_ADDR( COEF2 ) - OSD_ADDR( COEF1 ) == 4 );
    OSD_ASSERT_COMPILE( OSD_ADDR( COEF3 ) - OSD_ADDR( COEF2 ) == 4 );
    OSD_ASSERT_COMPILE( OSD_ADDR( COEF4 ) - OSD_ADDR( COEF3 ) == 4 );
    OSD_ASSERT_COMPILE( OSD_ADDR( COEF5 ) - OSD_ADDR( COEF4 ) == 4 );
    OSD_ASSERT_COMPILE( OSD_ADDR( COEF6 ) - OSD_ADDR( COEF5 ) == 4 );

    TTX_Address_t addr = OSD_ADDR( COEF0 );
    size_t i;

    if ( coeff != NULL )
    {
        for ( i = 0; i < OSD_MAX_COFF_NUM; i += 2 )
        {
            OSD_RegWrite( addr, OSD_COEF0_RMK( coeff[i], coeff[i+1] ) );
        }
        addr += 4;
    }
}

#if 0	// not used
/*---------------------------------------------------------------------------*/
void
OSD_onVerticalSync                  (void)
/*****************************************************************************\
 *  @brief   Will be triggered when vsync is arrived.
\*****************************************************************************/
{
}
#endif

/*---------------------------------------------------------------------------*/
void
OSD_onScreenCanBeRefreshed       (void)
/*****************************************************************************\
 *  @brief   Will be triggered when display is able to be refreshed.
 *  @note    Will be triggered each TTX_Tick().
\*****************************************************************************/
{
}

/* -- Implementations of Private Functions ----------------------------------*/

#endif  /* end of #if !OSD_SOFTWARE_EMULATOR */

/* -- Implementations of Functions Shared by OSD and OSDEMU -----------------*/


/*---------------------------------------------------------------------------*/
void
OSD_SetTtPageNumber             (const OSD_PageNum_t* pn)
/*****************************************************************************\
 *  @brief   The display of side panel of the specified page in teletext is enabled or disabled.
 *  @param   csel   character set selection ( 0~ OSD_MAX_CHAR_SET_NUM-1 )
 *  @ret     Whether the display of side panel of the specified page is enabled or not
\*****************************************************************************/
{
    OSD_SetTtPageNumberSeq( pn->bEnable, pn->csel, pn->fg, pn->bg, pn->str );
    OSD_ApplyChanges(0);
}

#if OSD_VERIFY  /* if enabled verficiation of TTX related IPs */

/*---------------------------------------------------------------------------*/
void
OSD_SetTtMessage                (const OSD_Message_t* msg)
/*****************************************************************************\
 *  @brief   Set the attributes of a message in teletext.
\*****************************************************************************/
{
    if ( msg != NULL )
    {
        OSD_SetTtMessageSeq( msg->enable,
                             msg->csel  ,
                             msg->fg    ,
                             msg->bg    ,
                             msg->o     ,
                             msg->u     ,
                             msg->i     ,
                             msg->fl    ,
                             msg->row   ,
                             msg->col   ,
                             msg->str   );
    }
}

#endif  /* end of #if OSD_VERIFY */



/*---------------------------------------------------------------------------*\
 * end of osd.c                                                              *
\*---------------------------------------------------------------------------*/

