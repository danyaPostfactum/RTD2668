/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     osd_emu.c
 * @brief    Sparrow VBI OSD emulator.
 * @author   Janus Cheng
 * @date     Dec. 30, 2008
\*****************************************************************************/

#include <osd.h>            /* Provides: OSD_SOFTWARE_EMULATOR */

#if OSD_SOFTWARE_EMULATOR

#include "osd_emu.h"        /* Provides: functions to be implemented */
#include <ttx_disp.h>       /* Provides: TTX_DISP_MAX_ROW_NUM */
#include <ttx.h>            /* Provides: TTX_DumpMem() */
#include <ttx_buff.h>       /* Provides: TTX_BUF_DecodeX0() */
#include <ttx_dec.h>        /* Provides: TTX_LopParam_t */
#include <ttx_bmposd.h>     /* Provides: TTX_BMPOSD_POINT_t and TTX_BMPOSD_SIZE_t */

#include <stdlib.h>         /* Provides: NULL */
#include <string.h>         /* Provides: memcpy() and strlen() */


#define TEST_ON_V_SYNC      (0)

#define PKT_BUF_SIZE_BYTES  (40)  // size of field buffer containing descrambeld packet data (including packet address)

TTX_DEFINE_THIS_MODULE( osd_emu.c )

static int32_t          g_cntUsage = 0;   // counts the currently OSD_Open()'d osdEmu.

#define _STATIC_OSDEMU_BMP_MEM_SIZE  (TTX_DISP_MAX_ROW_LEN*TTX_DISP_MAX_ROW_NUM*OSD_FONT_MAX_WIDTH*OSD_FONT_MAX_HEIGHT*OSDEMU_ZOOM_V*OSDEMU_ZOOM_H*sizeof(TTX_BMPOSD_PxFmt_t)) /* the memory size in bytes used when not using TTX_USE_MALLOC */
TTX_ASSERT_COMPILE( _STATIC_OSDEMU_BMP_MEM_SIZE > 0 );

#if !TTX_USE_MALLOC
/* Output display bitmap memory */
static uint8_t             pOSDEMU_PreAllocatedBmpDispBase [_STATIC_OSDEMU_BMP_MEM_SIZE];
#endif  /* end of #if !TTX_USE_MALLOC */


OSDEMU_Obj              osdEmu;



/* -- Prototypes of Private Functions ---------------------------------------*/
static void     OSDEMU_AllocBmp             (TTX_BMPOSD_PxFmt_t** ppBmpDisp);
static void     OSDEMU_onRefreshingScreen   (OSDEMU_Handle hOsd);
static void     OSDEMU_RefreshScreen        (OSDEMU_Handle hOsd);
void     OSDEMU_PaintRow0            (void);
void     OSDEMU_PaintPageContent     (void);
void     OSDEMU_PaintTopCtrl         (void);
void     OSDEMU_PaintSubPageNavCtrl  (void);
static void     OSDEMU_PaintPage            (OSDEMU_Handle hOsd, OSD_TtAttr_t lineAttr[], const int32_t row);
static void     OSDEMU_PaintColsOfaRow      (OSDEMU_Handle hOsd, const size_t outputBufPixelsWidth, OSD_TtAttr_t lineAttr[], const size_t startCol, const size_t numOfCols, const size_t row, const size_t pageShift);
static void     OSDEMU_GetCharBmp           (uint8_t* bmp, const uint8_t fontHeight, const uint8_t fontWidth, const uint8_t* fontSrc, const uint8_t bytesPerChar);
static void     OSDEMU_DrawMosaics          (uint8_t* bmp, const uint8_t fontHeight, const uint8_t fontWidth);
static void     OSDEMU_CalculateSize        (uint8_t* bmp, const uint8_t fontHeight, const uint8_t fontWidth, const TTX_STD_SizeCfg_t sizecfg);
static bool_t   OSDEMU_DrawSolidRect        (TTX_BMPOSD_PxFmt_t* pOutputBuf, const size_t outputBufPixelsWidth, const TTX_BMPOSD_POINT_t* pPointUpperLeft, const TTX_BMPOSD_SIZE_t* pSize, const TTX_BMPOSD_PxFmt_t* pColorToFill);
static bool_t   OSDEMU_DrawTransparentRect  (TTX_BMPOSD_PxFmt_t* outputBuf, const size_t outputBufPixelsWidth, const TTX_BMPOSD_POINT_t* pPointUpperLeft, const TTX_BMPOSD_SIZE_t* pSize);
static bool_t   OSDEMU_DrawPixel            (TTX_BMPOSD_PxFmt_t* pOutputBuf, const size_t outputBufPixelsWidth, const TTX_BMPOSD_POINT_t* pPxPos, const TTX_BMPOSD_PxFmt_t* pColor);
static bool_t   OSDEMU_Uint32ToTtAttr       (const volatile OSD_TtAttr_t* pRaw, TTX_DISP_Attr_t* pAttr);
static bool_t   OSDEMU_X0isChanged          (const uint8_t originalRow0[PKT_BUF_SIZE_BYTES], const uint8_t receivedRow0[PKT_BUF_SIZE_BYTES]);
static bool_t   OSDEMU_DecodeRow0           (TTX_DISP_Attr_t dest[TTX_DISP_MAX_ROW_LEN], const uint8_t TTX_MEM* src, const bool_t rolling);
static bool_t   OSDEMU_InitLopParam         (TTX_LopParam_t* pParam, TTX_STD_MagazineNum_t* pMag, const uint8_t TTX_MEM* src, const uint8_t rowNum);
static bool_t   OSDEMU_DrawScreenAsTransparent(OSDEMU_Handle hOsd);


//    void     OSDEMU_onReceivedX0         (const uint8_t* pOddParityDecodedData, const TTX_STD_Page_CtrlBits_t* pCtrlBits);
static bool_t   OSDEMU_onUp                 (OSDEMU_Handle hOsd);
static bool_t   OSDEMU_onDown               (OSDEMU_Handle hOsd);



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
    OSDEMU_Handle hOsd = &osdEmu;
    {
        hOsd->bIntendToDisplay = FALSE;
        hOsd->bIsDisplaying    = FALSE;
        hOsd->bBoxEnable       = FALSE;

        hOsd->pageNum.bEnable = TRUE;
        hOsd->pageNum.csel    = TTX_CHARSET_G0_LATIN;
        hOsd->pageNum.fg      = TTX_COLOR_WHITE;
        hOsd->pageNum.bg      = TTX_COLOR_RED;
        memset( hOsd->pageNum.str, ' ', OSD_MAX_PNUM_LEN );

        hOsd->bDisplayRow25          = TRUE;

        hOsd->bRefreshRow0           = FALSE;
        hOsd->bRefreshPageContent    = FALSE;
        hOsd->bRefreshTopCtrl        = FALSE;
        hOsd->bRefreshSubPageNavCtrl = FALSE;

        //@todo  Initialize hOsd->timecode
        hOsd->timecode.bDisplay      = FALSE;
        hOsd->header.bDisplay        = TRUE;
        hOsd->header.bRoll           = FALSE;

        hOsd->flashState.stepsTotal = 6;
        hOsd->flashState.stepsCnt   = 0;   // There're stepsTotal steps to complete a period of flashing.

        hOsd->sidePanel.bDisplay = FALSE;
        hOsd->sidePanel.nLeftColNum    = 0;

        //@todo  Initialize hOsd->clut[]

        hOsd->font.bytesPerFont = OSD_FONT_CHAR_SIZE;
        hOsd->font.pBmpFonts    = NULL;
        hOsd->font.pxHeight     = OSD_FONT_HEIGHT;
        hOsd->font.pxWidth      = OSD_FONT_WIDTH;

        hOsd->pAttr1 = NULL;

        memset( hOsd->latestRow0Raw, 0, PKT_BUF_SIZE_BYTES );
        memset( hOsd->latestRow0   , 0, TTX_DISP_MAX_ROW_LEN * sizeof( hOsd->latestRow0[0] ) );

        if ( g_cntUsage == 0 )
        {
            OSDEMU_AllocBmp( &(hOsd->pBmpDisp) );
            OSD_ENSURE( hOsd->pBmpDisp != NULL );
        }
        g_cntUsage++;

    }

    if ( flags & OSD_OPEN_RESET )
    {
        OSD_Reset();
    }

    TTX_BMPOSD_Open();

    /*
        if( !_bOsdIsOpened )
        {
            OSD_Rgb2Yuv(0, OSD_TOTAL_CLUT_NUM-1);
            _bOsdIsOpened = TRUE;
        }
    */

    OSD_SetYuvToggle( TRUE );   // Enable U/V toggle

    OSD_SetOutputFormat( TRUE );    // Set to interlace


    OSD_SetDisplayMode( FALSE );    // FALSE for TT; TRUE for CC

    OSD_SetOsdPg( FALSE );          // FALSE for disable pattern gen; TRUE for enable pattern gen
//        OSD_ApplyChanges( TRUE );    // TRUE for blocking; FALSE for non-blocking

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
    OSD_REQUIRE( g_cntUsage > 0 );  // ERROR: Unbalanced OSD_Open() and OSD_Close() pair?

    g_cntUsage--;

#if TTX_USE_MALLOC
    if ( g_cntUsage == 0 )
    {
        OSDEMU_Handle hOsd = &osdEmu;
        if ( hOsd->pBmpDisp != NULL )
        {
            free( hOsd->pBmpDisp );
            hOsd->pBmpDisp = NULL;
        }
    }
#endif

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
    size_t i;
    OSDEMU_Handle hOsd = &osdEmu;

    OSDEMU_DrawScreenAsTransparent( hOsd );

    for ( i=OSD_MAX_PNUM_LEN; i<TTX_DISP_MAX_ROW_LEN_WITHOUT_SP-OSDEMU_TIMECODE_LEN; i++ )
    {
        hOsd->latestRow0[i] = 0;
    }

    hOsd->bRefreshRow0           = FALSE;
    hOsd->bRefreshPageContent    = FALSE;
    hOsd->bRefreshTopCtrl        = FALSE;
    hOsd->bRefreshSubPageNavCtrl = FALSE;
}

/*---------------------------------------------------------------------------*/
void
OSD_SetYuvToggle    (bool_t bEnable )
/*****************************************************************************\
 *  @brief   Enable/Disable U/V toggle for video 16.
 *  @param   bEnable	: [input] Set '1' to enable, or set '0' to disable.
\*****************************************************************************/
{
}

/*---------------------------------------------------------------------------*/
void
OSD_SetOutputFormat             (bool_t bInterlace)
/*****************************************************************************\
 *  @brief   Set OSD ouput format (interlance or progressive).
 *  @param   mode : [input] Set '1' for interlance mode, or set '0' for progressive mode.
\*****************************************************************************/
{
}

/*---------------------------------------------------------------------------*/
void
OSD_SetDisplayMode              (bool_t bClosedCaption)
/*****************************************************************************\
 *  @brief   Set display mode (Teletext or Closed Cpation).
 *  @param   bClosedCaption : [input] Set '1' for CC mode, or set '0' for TT mode.
\*****************************************************************************/
{
}

/*---------------------------------------------------------------------------*/
void
OSD_SetMixedMode    (bool_t bEnable)
/*****************************************************************************\
 *  @brief   Enable/Disable mixed mode.
 *  @param   bEnable	: [input] Set '1' to enable, or set '0' to disable.
\*****************************************************************************/
{
}

/*---------------------------------------------------------------------------*/
void
OSD_SetVbiEnable    (bool_t bEnable)
/*****************************************************************************\
 *  @brief   Enable/Disable VBI OSD display.
 *  @param   bEnable	: [input] Set '1' to enable, or set '0' to disable.
\*****************************************************************************/
{
}

/*---------------------------------------------------------------------------*/
void
OSD_SetVdelay    (uint16_t vdelay)
/*****************************************************************************\
 *  @brief   Set vdelay value.
 *  @param   vdelay	: [input] the value of vdelay to set
\*****************************************************************************/
{
}

/*---------------------------------------------------------------------------*/
void
OSD_SetHdelay1  (uint8_t hdelay1)
/*****************************************************************************\
 *  @brief   Set hdelay1 value.
 *  @param   hdelay1	: [input] the value of hdelay1 to set
\*****************************************************************************/
{
}

/*---------------------------------------------------------------------------*/
void
OSD_SetHdelay2  (uint16_t hdelay2)
/*****************************************************************************\
 *  @brief   Set hdelay2 value.
 *  @param   hdelay2	: [input] the value of hdelay2 to set
\*****************************************************************************/
{
}

/*---------------------------------------------------------------------------*/
void
OSD_SetHdelay3  (uint16_t hdelay3)
/*****************************************************************************\
 *  @brief   Set hdelay3 value.
 *  @param   hdelay3	: [input] the value of hdelay3 to set
\*****************************************************************************/
{
}

/*---------------------------------------------------------------------------*/
bool_t
OSD_GetVsyncIntrStatus          (void)
/*****************************************************************************\
 *  @brief   Get current vsync interrupt status.
 *  @ret     the current status of vsync interrupt
\*****************************************************************************/
{
    return( TRUE );
}

/*---------------------------------------------------------------------------*/
void
OSD_ClearVsyncIntrStatus        (void)
/*****************************************************************************\
 *  @brief   Clear vsync interrupt status.
\*****************************************************************************/
{
}


/*---------------------------------------------------------------------------*/
void
OSD_SetClutAll  (OSD_CLUT_t clut_ary[OSD_TOTAL_CLUT_NUM][OSD_MAX_CLUT_ENTRY_NUM])
/*****************************************************************************\
 *  @brief   Setup the value of every entry in all color lookup tables.
 *  @param   clut_ary	: [input] the array of values to set
\*****************************************************************************/
{
    OSDEMU_Handle hOsd = &osdEmu;
    uint8_t i, j;

    for ( i = 0; i < OSD_TOTAL_CLUT_NUM; i++ )
        for ( j = 0; j < OSD_MAX_CLUT_ENTRY_NUM; j++ )
            hOsd->clut[i][j] = clut_ary[i][j];
}

/*---------------------------------------------------------------------------*/
uint32_t
OSD_CombineRGB  (uint8_t redValue, uint8_t greenValue, uint8_t blueValue)
/*****************************************************************************\
 *  @brief   Setup the value of every entry in all color lookup tables.
 *  @param   clut_ary	: [input] the array of values to set
\*****************************************************************************/
{
    return( 0 );
}

/*---------------------------------------------------------------------------*/
void
OSD_ApplyEntireScreenChanges             (bool_t bBlock)
/*****************************************************************************\
 *  @brief   Update entire screen.
 *  @param   bBlock : [input] Wait for vsync before return. Set '1' to wait, or set '0' to return immediately.
\*****************************************************************************/
{
    OSD_ApplyRow0Changes( bBlock );
    OSD_ApplyPageContentChanges( bBlock );
    OSD_ApplyTopCtrlChanges( bBlock );
    OSD_ApplySubPageNavCtrlChanges( bBlock );
}

/*---------------------------------------------------------------------------*/
void
OSD_ApplyRow0Changes             (bool_t bBlock)
/*****************************************************************************\
 *  @brief   Update only row-0.
 *  @param   bBlock : [input] Wait for vsync before return. Set '1' to wait, or set '0' to return immediately.
\*****************************************************************************/
{
    osdEmu.bRefreshRow0 = TRUE;
}

/*---------------------------------------------------------------------------*/
void
OSD_ApplyPageContentChanges      (bool_t bBlock)
/*****************************************************************************\
 *  @brief   Update row-1 to row-23 only.
 *  @param   bBlock : [input] Wait for vsync before return. Set '1' to wait, or set '0' to return immediately.
\*****************************************************************************/
{
    osdEmu.bRefreshPageContent = TRUE;
}

/*---------------------------------------------------------------------------*/
void
OSD_ApplyTopCtrlChanges           (bool_t bBlock)
/*****************************************************************************\
 *  @brief   Update TOP control only.
 *  @param   bBlock : [input] Wait for vsync before return. Set '1' to wait, or set '0' to return immediately.
\*****************************************************************************/
{
    osdEmu.bRefreshTopCtrl = TRUE;
}

/*---------------------------------------------------------------------------*/
void
OSD_ApplySubPageNavCtrlChanges    (bool_t bBlock)
/*****************************************************************************\
 *  @brief   Update sub-page navigation control only.
 *  @param   bBlock : [input] Wait for vsync before return. Set '1' to wait, or set '0' to return immediately.
\*****************************************************************************/
{
    osdEmu.bRefreshSubPageNavCtrl = TRUE;
}

/*---------------------------------------------------------------------------*/
void
OSD_SetOsdPg                    (bool_t bEnable)
/*****************************************************************************\
 *  @brief   Enable/Disable OSD pattern generator.
 *  @param   bEnable	: [input] Set '1' to enable, or set '0' to disable.
\*****************************************************************************/
{
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
    osdEmu.font.pBmpFonts = (const uint8_t (*)[][OSD_FONT_CHAR_NUM_PER_SET][OSD_FONT_CHAR_SIZE])addr;
}

/*---------------------------------------------------------------------------*/
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
    return( (OSD_Address_t)osdEmu.font.pBmpFonts );
}

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
    osdEmu.pAttr1 = (OSD_TtAttr_t (*)[TTX_DISP_MAX_ROW_NUM][TTX_DISP_MAX_ROW_LEN])addr;
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
    return( (OSD_Address_t) osdEmu.pAttr1 );
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
    osdEmu.bDisplayRow25 = bEnable;
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
    return( osdEmu.bDisplayRow25 );
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
    osdEmu.bIntendToDisplay = bEnable;
}

/*---------------------------------------------------------------------------*/
bool_t
OSD_isDisplaying    (void)
/*****************************************************************************\
 *  @brief   Enable/Disable the display of the specified page in teletext.
 *  @param   bEnable Set '1' to enable, or set '0' to disable.
\*****************************************************************************/
{
    return( osdEmu.bIsDisplaying );
}


/*---------------------------------------------------------------------------*/
void
OSD_SetTtBoxEnable    (bool_t bEnable)
/*****************************************************************************\
 *  @brief   Enable box area or window area of the specified page in teletext.
 *  @param   Set '1' to enable box area, or set '0' to enable window area.
\*****************************************************************************/
{
    osdEmu.bBoxEnable = bEnable;
}

/*---------------------------------------------------------------------------*/
bool_t
OSD_GetTtBoxEnable    (void)
/*****************************************************************************\
 *  @brief   Enable box area or window area of the specified page in teletext.
 *  @param   Set '1' to enable box area, or set '0' to enable window area.
\*****************************************************************************/
{
    return( osdEmu.bBoxEnable );
}

/*---------------------------------------------------------------------------*/
void
OSD_SetTtRollingEnable    (bool_t bEnable)
/*****************************************************************************\
 *  @brief   Enable/Disable the display of rolling header of the specified page in teletext.
 *  @param   Set '1' to enable, or set '0' to disable.
\*****************************************************************************/
{
    osdEmu.header.bRoll = bEnable;
}

/*---------------------------------------------------------------------------*/
void
OSD_SetTtTimecodeEnable    (bool_t bEnable)
/*****************************************************************************\
 *  @brief   Enable/Disable the display of time code of the specified page in teletext.
 *  @param   Set '1' to enable, or set '0' to disable.
\*****************************************************************************/
{
    osdEmu.timecode.bDisplay = bEnable;
}

/*---------------------------------------------------------------------------*/
void
OSD_SetTtR0Enable    (bool_t bEnable)
/*****************************************************************************\
 *  @brief   Enable/Disable the display of row 0 of the specified page in teletext.
 *  @param   Set '1' to enable, or set '0' to disable.
\*****************************************************************************/
{
    osdEmu.header.bDisplay = bEnable;
}

#if 0   /* THIS_FUNCTION is not used */

/*---------------------------------------------------------------------------*/
void
OSD_SetTtR0DhDwConfig           (bool_t extForDh, bool_t dwEnable, bool_t dhEnable, bool_t dwMode, bool_t setSpace)
/*****************************************************************************\
 *  @brief  Set the config of row 0 in teletext.
 *  @param	extForDh	: [input] whether header can be extended for DH or not
 *  @param	dwEnable	: [input] whether header allows double width or not
 *  @param	dhEnable	: [input] whether header allows double high or not
 *  @param	dwMode		: [input] the DW mode for header. Set '0' for Micronas mode, or set '1' for Philips mode.
 *  @param	setSpace	: [input] whether row 1 allows to be set space bellow page number when row 0 has a DH
\*****************************************************************************/
{
}

/*---------------------------------------------------------------------------*/
void
OSD_SetTtR0DhCmdEnable          (bool_t bEnable)
/*****************************************************************************\
 *  @brief   Enable/Disable the "double height" command in row 0 in teletext.
 *  @param   Set '1' to enable, or set '0' to disable.
\*****************************************************************************/
{
}

/*---------------------------------------------------------------------------*/
void
OSD_SetTtR0DwCmdEnable          (bool_t bEnable)
/*****************************************************************************\
 *  @brief   Enable/Disable the "double width" command in row 0 in teletext.
 *  @param   Set '1' to enable, or set '0' to disable.
\*****************************************************************************/
{
}

/*---------------------------------------------------------------------------*/
void
OSD_SetTtR0DsCmdEnable          (bool_t bEnable)
/*****************************************************************************\
 *  @brief   Enable/Disable the "double size" command in row 0 in teletext.
 *  @param   Set '1' to enable, or set '0' to disable.
\*****************************************************************************/
{
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
}

/*---------------------------------------------------------------------------*/
void
OSD_SetTtSidepanelEn    (bool_t bEnable)
/*****************************************************************************\
 *  @brief   Enable/Disable the display of side panel of the specified page in teletext.
 *  @param   Set '1' to enable, or set '0' to disable.
\*****************************************************************************/
{
    osdEmu.sidePanel.bDisplay = bEnable;
}

/*---------------------------------------------------------------------------*/
bool_t
OSD_GetTtSidepanelEn            (void)
/*****************************************************************************\
 *  @brief   The display of side panel of the specified page in teletext is enabled or disabled.
 *  @ret     Whether the display of side panel of the specified page is enabled or not
\*****************************************************************************/
{
    return( osdEmu.sidePanel.bDisplay );
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
    OSDEMU_Handle hOsd = &osdEmu;

    hOsd->pageNum.csel = csel;
    hOsd->pageNum.fg   = fg;
    hOsd->pageNum.bg   = bg;
    strncpy( hOsd->pageNum.str, str, OSD_MAX_PNUM_LEN );
    hOsd->pageNum.bEnable = enable;

    OSD_ApplyRow0Changes( FALSE );
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
    if ( pn != NULL )
    {
        OSDEMU_Handle hOsd = &osdEmu;

        pn->csel = hOsd->pageNum.csel;
        pn->fg   = hOsd->pageNum.fg;
        pn->bg   = hOsd->pageNum.bg;
        strncpy( pn->str, hOsd->pageNum.str, OSD_MAX_PNUM_LEN );
        pn->bEnable = hOsd->pageNum.bEnable;
    }
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
    static uint16_t const TTX_ROM OSD_ttDmBitmap[OSD_MAX_DM_NUM][OSD_MAX_DM_LINE_NUM] =
    {
        { 0x000, 0x000, 0x000 },
        { 0x060, 0x030, 0x000 },
        { 0x030, 0x060, 0x000 },
        { 0x070, 0x0d8, 0x000 },
        { 0x1c4, 0x238, 0x000 },
        { 0x3fc, 0x000, 0x000 },
        //{ 0x198, 0x060, 0x000 },
        { 0x198, 0x0f0, 0x000 },
        { 0x060, 0x000, 0x000 },
        { 0x198, 0x000, 0x000 },
        { 0x000, 0x000, 0x060 },
        { 0x0f0, 0x198, 0x0f0 },
        { 0x000, 0x020, 0x060 },
        { 0x000, 0x000, 0x7fe },
        { 0x0cc, 0x198, 0x000 },
        { 0x000, 0x040, 0x060 },
        //{ 0x0d8, 0x070, 0x000 }
        { 0x198, 0x060, 0x000 }
    };

    OSDEMU_Handle hOsd = &osdEmu;
    if ( OSD_ttDmBitmap != NULL )
    {
        uint8_t i, j;
        for ( i = 0 ; i < OSD_MAX_DM_NUM; i++ )
            for ( j = 0; j < OSD_MAX_DM_LINE_NUM; j++ )
                hOsd->font.bmpDm[i][j] = OSD_ttDmBitmap[i][j];
    }
}

/*---------------------------------------------------------------------------*/
void
OSD_SetTtScaleCoefficient       (const uint32_t coeff[OSD_MAX_COFF_NUM])
/*****************************************************************************\
 *  @brief   Set the scaler coefficients in teletext.
\*****************************************************************************/
{
}

/*---------------------------------------------------------------------------*/
void
OSD_onVerticalSync                  (void)
/*****************************************************************************\
 *  @brief   Will be triggered when vsync is arrived.
\*****************************************************************************/
{
    OSDEMU_Handle hOsd = &osdEmu;


#if TEST_ON_V_SYNC
    // OK: Test TTX_BMPOSD_Draw().
    {
        const uint8_t FONT_WIDTH  = hOsd->font.pxWidth;     // number of pixels of width  of a character
        const uint8_t FONT_HEIGHT = hOsd->font.pxHeight;
        const size_t  pxWidth    = TTX_DISP_MAX_ROW_LEN * FONT_WIDTH  * OSDEMU_ZOOM_H;
        const size_t pageShift   = 0;
        const size_t outputBufPixelsWidth = pxWidth;
        const size_t row = 0, col = 0;
        TTX_BMPOSD_POINT_t pxPos = { 30+OSDEMU_ZOOM_H  * (pageShift + col * FONT_WIDTH) , 20+OSDEMU_ZOOM_V * (row * FONT_HEIGHT)};
        const TTX_BMPOSD_SIZE_t  size  = { 20 * FONT_WIDTH * OSDEMU_ZOOM_H, 20 * FONT_HEIGHT * OSDEMU_ZOOM_V };

        TTX_BMPOSD_PxFmt_t color;

        TTX_BMPOSD_SplitRGB( &color, 0xFF, 0x00, 0xFF, 0x00 );   // A R G B

        OSDEMU_DrawSolidRect( hOsd->pBmpDisp, outputBufPixelsWidth, &pxPos, &size, &color );
        /*
                {   // OK: Draw Vertical line at the end of row
                    int i;
                    TTX_Log(LOGGER_INFO, "[TTX] TTX_DISP_MAX_ROW_LEN == %d\n", TTX_DISP_MAX_ROW_LEN);
                    TTX_Log(LOGGER_INFO, "[TTX] FONT_WIDTH           == %d\n", FONT_WIDTH);
                    TTX_Log(LOGGER_INFO, "[TTX] OSDEMU_ZOOM_H        == %d\n", OSDEMU_ZOOM_H);
                    TTX_Log(LOGGER_INFO, "[TTX] pxWidth              == %d\n", pxWidth);
                    TTX_Log(LOGGER_INFO, "[TTX] outputBufPixelsWidth == %d\n", outputBufPixelsWidth);
                    for(i=1; i<=519; i++ )
                    {
                        hOsd->pBmpDisp[i*480-1]   = color;
                    }
                }
        */

    }
#else
    OSDEMU_onRefreshingScreen( hOsd ); // update the output bitmap to be displayed
#endif  /* end of #if TEST_ON_V_SYNC */


    if ( TTX_BMPOSD_Draw( hOsd->pBmpDisp ) )
    {
        TTX_BMPOSD_ApplyChanges();
    }

}

/*---------------------------------------------------------------------------*/
void
OSD_onScreenCanBeRefreshed       (void)
/*****************************************************************************\
 *  @brief   Will be triggered when display is able to be refreshed. Because
 *             all the attributes are ready.
 *  @note    Will be triggered each TTX_Tick().
\*****************************************************************************/
{
    static uint8_t cnt10ms = 0;
    if ( cnt10ms & 0x01 )   // the duration ov v-sync of PAL is 20 ms
    {
        OSD_onVerticalSync();
    }
    cnt10ms++;
}

/* -- Implementations of Private Functions ----------------------------------*/

/*---------------------------------------------------------------------------*/
static void
OSDEMU_AllocBmp    (TTX_BMPOSD_PxFmt_t** ppBmpDisp)
/*****************************************************************************\
 *  @brief   Allocates memory for hDisp->pBmp.
\*****************************************************************************/
{
    if ( ppBmpDisp != NULL )
    {
#if TTX_USE_MALLOC
        *ppBmpDisp = (TTX_BMPOSD_PxFmt_t*)malloc( _STATIC_OSDEMU_BMP_MEM_SIZE );
#else
        *ppBmpDisp = (TTX_BMPOSD_PxFmt_t*)pOSDEMU_PreAllocatedBmpDispBase;
#endif
        TTX_ENSURE( *ppBmpDisp != NULL );
        if ( *ppBmpDisp != NULL )
        {
#if !DV_SIM /* don't waste time in DV_SIM */
            memset( *ppBmpDisp, 0, _STATIC_OSDEMU_BMP_MEM_SIZE );
#endif
        }
    }
}

/*---------------------------------------------------------------------------*/
static void
OSDEMU_onRefreshingScreen    (OSDEMU_Handle hOsd)
/*****************************************************************************\
 *  @brief   Runs the procedures that each refresh should run.
 *  @details Update hOsd->pBmpDisp[] from hOsd->pAttr1[][].
 *  @pre     hOsd->pAttr1 != NULL
 *  @post    hOsd->pBmpDisp[outputBmpIndex] will be updated.
\*****************************************************************************/
{
    if ( hOsd->pBmpDisp != NULL )
    {
        if ( hOsd->bIntendToDisplay )
        {
            if ( ! hOsd->bIsDisplaying )
            {
                if ( OSDEMU_onUp( hOsd ) )
                {
                    hOsd->bIsDisplaying = TRUE;
                }
            }
        }
        else
        {
            if ( hOsd->bIsDisplaying )
            {
                if ( OSDEMU_onDown( hOsd ) )
                {
                    hOsd->bIsDisplaying = FALSE;
                }
            }
        }

        if ( hOsd->bIsDisplaying )
        {
            OSDEMU_RefreshScreen( hOsd );
        }
    }   // end of if( hOsd->pBmpDisp != NULL )

}
/*---------------------------------------------------------------------------*/
static void
OSDEMU_RefreshScreen    (OSDEMU_Handle hOsd)
/*****************************************************************************\
 *  @brief   Runs the procedures that each refresh should run.
 *  @details Update hOsd->pBmpDisp[] from hOsd->pAttr1[][].
 *  @pre     hOsd->pAttr1 != NULL
 *  @post    hOsd->pBmpDisp[outputBmpIndex] will be updated.
\*****************************************************************************/
{
    if ( hOsd->pAttr1 != NULL )
    {
        if ( hOsd->font.pBmpFonts != NULL ) // no font, no paint
        {
            {
                static uint8_t cntVsync = 0;
                /* The calling duration of this function is 20 ms, and
                 *   the duration of flashing is
                 */
                if ( cntVsync > 500/20 )
                {
                    // Flashing
                    hOsd->flashState.stepsCnt++;
                    if ( hOsd->flashState.stepsCnt >= hOsd->flashState.stepsTotal )
                    {
                        hOsd->flashState.stepsCnt = 0;  // Reset flashState.stepsCnt if it's greater than or equal to flashState.stepsTotal.
                    }
                }
                cntVsync++;
            }

            // Render row-0 if required
            if ( hOsd->header.bDisplay )
            {
                if ( hOsd->bRefreshRow0 )
                {
                    TTX_BMPOSD_CallbackEvtArgs_t arg;
                    arg.pBmp = hOsd->pBmpDisp;

                    TTX_BMPOSD_Callback( TTX_BMPOSD_CALLBACK_ON_ROW0_NEEDS_REFRESHED, &arg );
                }
            }


            if ( hOsd->bRefreshPageContent )
            {
                TTX_BMPOSD_CallbackEvtArgs_t arg;
                arg.pBmp = hOsd->pBmpDisp;

                TTX_BMPOSD_Callback( TTX_BMPOSD_CALLBACK_ON_PAGE_CONTENT_NEEDS_REFRESHED, &arg );
            }

            if ( hOsd->bRefreshTopCtrl )
            {
                TTX_BMPOSD_CallbackEvtArgs_t arg;
                arg.pBmp = hOsd->pBmpDisp;

                TTX_BMPOSD_Callback( TTX_BMPOSD_CALLBACK_ON_TOP_CTRL_NEEDS_REFRESHED, &arg );
            }

            if ( hOsd->bRefreshSubPageNavCtrl )
            {
                TTX_REQUIRE( TTX_DISP_SUBPAGE_NAV_CTRL_ROW_NUM == 25 );
                if ( hOsd->bDisplayRow25 )
                {
                    TTX_BMPOSD_CallbackEvtArgs_t arg;
                    arg.pBmp = hOsd->pBmpDisp;

                    TTX_BMPOSD_Callback( TTX_BMPOSD_CALLBACK_ON_SUBPAGE_NAV_CTRL_NEEDS_REFRESHED, &arg );
                }
            }

        }
    }
}

/*---------------------------------------------------------------------------*/
void
OSDEMU_PaintRow0    (void)
/*****************************************************************************\
 *  @brief   Runs the procedures that each refresh should run.
 *  @details Update hOsd->pBmpDisp[0][] from hOsd->pAttr1[0][].
 *  @pre     hOsd->pAttr1 != NULL
 *  @post    hOsd->pBmpDisp[] will be updated.
\*****************************************************************************/
{
    OSDEMU_Handle hOsd = &osdEmu;

    const int row = 0;
    int       i;


    if ( hOsd->pAttr1 != NULL )
    {
        TTX_DISP_Attr_t     x0Proc;  // X/0 buffer used for processing
        OSD_TtAttr_t        x0Draw[TTX_DISP_MAX_ROW_LEN];  // X/0 buffer used to be drawn

        // Fill x0Draw[] with hOsd->pAttr1[row][]
        memcpy( x0Draw, (*hOsd->pAttr1)[row], TTX_DISP_MAX_ROW_LEN * sizeof(x0Draw[0]) );

        // Overlay page number
        if ( hOsd->pageNum.bEnable )
        {
            OSD_REQUIRE( OSD_MAX_PNUM_LEN <= TTX_DISP_MAX_ROW_LEN );  // ERROR: OSD_MAX_PNUM_LEN is too large or TTX_DISP_MAX_ROW_LEN is too small.
            for ( i=0; i<OSD_MAX_PNUM_LEN; i++ )
            {
                // Update page numbers of x0Draw[]
                if ( TTX_DISP_OsdAttrToAttr( &(x0Draw[i]), &x0Proc ) == TRUE )
                {
                    x0Proc.charset    = hOsd->pageNum.csel;
                    x0Proc.fgColorIdx = hOsd->pageNum.fg;
                    x0Proc.bgColorIdx = hOsd->pageNum.bg;
                    x0Proc.charIdx    = hOsd->pageNum.str[i];
                    x0Proc.conceal    = FALSE;  /* page numbers shall never be concealed */

                    // Store updated attribute back to x0Draw[]
                    TTX_DISP_AttrToOsdAttr( &x0Proc, &(x0Draw[i]) );
                }

            }
        }

        // Overlay rolling header if necessary
        if ( hOsd->header.bRoll )
        {
            for ( i=OSD_MAX_PNUM_LEN; i<TTX_DISP_MAX_ROW_LEN_WITHOUT_SP-OSDEMU_TIMECODE_LEN; i++ )
            {
                x0Draw[i] = hOsd->latestRow0[i];
            }
        }

        // Overlay timecode if necessary
        if ( hOsd->timecode.bDisplay )
        {
            for ( i=TTX_DISP_MAX_ROW_LEN_WITHOUT_SP-OSDEMU_TIMECODE_LEN; i<TTX_DISP_MAX_ROW_LEN_WITHOUT_SP; i++ )
            {
                x0Draw[i] = hOsd->latestRow0[i];
            }
        }

        if ( hOsd->font.pBmpFonts != NULL ) // no font, no paint
        {
            OSDEMU_PaintPage( hOsd, x0Draw, row );
        }

        hOsd->bRefreshRow0 = FALSE; /* indicates we have refreshed row-0 */
    }

}

/*---------------------------------------------------------------------------*/
void
OSDEMU_PaintPageContent    (void)
/*****************************************************************************\
 *  @brief   Runs the procedures that each refresh should run.
 *  @details Update hOsd->pBmpDisp[0][] from hOsd->pAttr1[0][].
 *  @pre     hOsd->pAttr1 != NULL
 *  @post    hOsd->pBmpDisp[] will be updated.
\*****************************************************************************/
{
    OSDEMU_Handle hOsd = &osdEmu;

    uint8_t row;

    // Render row_0 to row_23 if required
    for ( row = 1 ; row <= 23 ; row++ )
    {
        OSDEMU_PaintPage( hOsd, (*hOsd->pAttr1)[row], row );
    }

    hOsd->bRefreshPageContent = FALSE; /* indicates we have refreshed row-1 to row-23 */
}

/*---------------------------------------------------------------------------*/
void
OSDEMU_PaintTopCtrl    (void)
/*****************************************************************************\
 *  @brief   Runs the procedures that each refresh should run.
 *  @details Update hOsd->pBmpDisp[0][] from hOsd->pAttr1[0][].
 *  @pre     hOsd->pAttr1 != NULL
 *  @post    hOsd->pBmpDisp[] will be updated.
\*****************************************************************************/
{
    OSDEMU_Handle hOsd = &osdEmu;

    const uint8_t row = TTX_DISP_TOP_CTRL_ROW_NUM;
    OSDEMU_PaintPage( hOsd, (*hOsd->pAttr1)[row], row );

    hOsd->bRefreshTopCtrl = FALSE; /* indicates we have refreshed row-TTX_DISP_TOP_CTRL_ROW_NUM */
}

/*---------------------------------------------------------------------------*/
void
OSDEMU_PaintSubPageNavCtrl    (void)
/*****************************************************************************\
 *  @brief   Runs the procedures that each refresh should run.
 *  @details Update hOsd->pBmpDisp[0][] from hOsd->pAttr1[0][].
 *  @pre     hOsd->pAttr1 != NULL
 *  @post    hOsd->pBmpDisp[] will be updated.
\*****************************************************************************/
{
    OSDEMU_Handle hOsd = &osdEmu;

    const uint8_t row = TTX_DISP_SUBPAGE_NAV_CTRL_ROW_NUM;
    OSDEMU_PaintPage( hOsd, (*hOsd->pAttr1)[row], row );

    hOsd->bRefreshSubPageNavCtrl = FALSE; /* indicates we have refreshed row-TTX_DISP_TOP_CTRL_ROW_NUM */
}

/*---------------------------------------------------------------------------*/
static void
OSDEMU_PaintPage  (OSDEMU_Handle hOsd, OSD_TtAttr_t lineAttr[], \
                   const int32_t row )
/*****************************************************************************\
 *  @brief   Draws a row of a page.
 *  @details Use this function to draw a bitmap of a row of a page.
 *  @param   hOsd          the hOsd->pBmpDisp will be painted
 *  @param   lineAttr      the attributes of entire row
 *  @param   row           the row of the page to be painted. Starting from zero.
 *
 *  @pre     <ol>
 *              <li>_AllocBmp() has been called</li>
 *              <li>lineAttr[row][0] to
 *                  lineAttr[row][TTX_DISP_MAX_ROW_LEN_WITHOUT_SP-1]
 *                  stores main page</li>
 *              <li>lineAttr[row][TTX_DISP_MAX_ROW_LEN_WITHOUT_SP] to
 *                  lineAttr[row][TTX_DISP_MAX_ROW_LEN_WITHOUT_SP+hOsd->sidePanel.nLeftColNum-1]
 *                  stores left-sidepanel</li>
 *              <li>lineAttr[row][TTX_DISP_MAX_ROW_LEN_WITHOUT_SP+hOsd->sidePanel.nLeftColNum] to
 *                  lineAttr[row][TTX_DISP_MAX_ROW_LEN-1]
 *                  stores right-sidepanel</li>
 *           </ol>
 *  @post    hOsd->pBmpDisp will be painted.
\*****************************************************************************/
{
    const uint8_t FONT_WIDTH = hOsd->font.pxWidth;     // number of pixels of width  of a character
    const size_t  pxWidth    = TTX_DISP_MAX_ROW_LEN * FONT_WIDTH  * OSDEMU_ZOOM_H;

    const size_t outputBufPixelsWidth = pxWidth;

    size_t pageShift;   // number of pixels to be shift horizontally to the right


    if ( outputBufPixelsWidth > 0 )
    {
        if ( hOsd->sidePanel.bDisplay ) // display of side-panels is enabled
        {
            /* Render left side-panel */
            pageShift = 0;   // number of pixels to shift horizontally to the right
            OSDEMU_PaintColsOfaRow( hOsd, outputBufPixelsWidth, lineAttr, \
                                    TTX_DISP_MAX_ROW_LEN_WITHOUT_SP,      \
                                    hOsd->sidePanel.nLeftColNum,          \
                                    row, pageShift);

            /* Render right side-panel */
            pageShift = (hOsd->sidePanel.nLeftColNum + TTX_DISP_MAX_ROW_LEN_WITHOUT_SP) * FONT_WIDTH;   // number of pixels to shift horizontally to the right
            OSDEMU_PaintColsOfaRow( hOsd, outputBufPixelsWidth, lineAttr,                                                 \
                                    TTX_DISP_MAX_ROW_LEN_WITHOUT_SP + hOsd->sidePanel.nLeftColNum,                        \
                                    TTX_DISP_MAX_ROW_LEN - TTX_DISP_MAX_ROW_LEN_WITHOUT_SP - hOsd->sidePanel.nLeftColNum, \
                                    row, pageShift);


            /* Render main page */
            pageShift = (hOsd->sidePanel.nLeftColNum + 1) * FONT_WIDTH;   // number of pixels to shift horizontally to the right
            OSDEMU_PaintColsOfaRow( hOsd, outputBufPixelsWidth, lineAttr, \
                                    0,                                    \
                                    TTX_DISP_MAX_ROW_LEN_WITHOUT_SP,      \
                                    row, pageShift);

        }
        else
        {
            /* Render main page only */
            // Attempt to center text output on the screen.
            pageShift = (outputBufPixelsWidth - (TTX_DISP_MAX_ROW_LEN_WITHOUT_SP * FONT_WIDTH))/2;   // number of pixels to shift horizontally to the right
            OSDEMU_PaintColsOfaRow( hOsd, outputBufPixelsWidth, lineAttr,  \
                                    0,                                     \
                                    TTX_DISP_MAX_ROW_LEN_WITHOUT_SP,            \
                                    row, pageShift);
        }


    }

    return;
}

/*---------------------------------------------------------------------------*/
static void
OSDEMU_PaintColsOfaRow  (OSDEMU_Handle hOsd,
                         const size_t  outputBufPixelsWidth,
                         OSD_TtAttr_t  lineAttr[],
                         const size_t  startCol,
                         const size_t  numOfCols,
                         const size_t  row,
                         const size_t  pageShift)
/*****************************************************************************\
 *  @brief   Draws columns of a row.
 *  @details This function render lineAttr[row][startCol] to lineAttr[row][numOfCols-1] to
 *             *outputBuf with horizontal pageShift.
 *  @param   outputBuf  the renderend output bitmap of a whole page.
 *  @param   outputBufPixelsWidth   the width of outputBuf in pixel.
 *  @param   lineAttr[] display attributes of a whole page.
 *  @param   startCol   the leftmost column of lineAttr[] to be drawn. Starting from zero.
 *  @param   numOfCols  the number of columns of lineAttr[] to be drawn.
 *  @param   row        the row number. 0 <= row <= DISPLAY_ROW_NUM.
 *  @param   pageShift  the number of pixels of the leftmost vertical border (invisible) in outputBuf[]
 *  @pre     lineAttr[row][startCol] to lineAttr[row][numOfCols-1] stores
 *             display attributes to be drawn.
\*****************************************************************************/
{
    uint8_t i, j;
    const unsigned char (* const fonts)[][OSD_FONT_CHAR_NUM_PER_SET][OSD_FONT_CHAR_SIZE] \
    = hOsd->font.pBmpFonts;
    const uint8_t FONT_WIDTH  = hOsd->font.pxWidth;         // number of pixels of width  of a character in present resolution
    const uint8_t FONT_HEIGHT = hOsd->font.pxHeight;        // number of pixels of height of a character in present resolution
    const uint8_t FONT_BYTES_PER_CHAR = hOsd->font.bytesPerFont;
    uint8_t bmpFont[OSD_FONT_MAX_HEIGHT][OSD_FONT_MAX_WIDTH];   // the bitmap of single character

    TTX_STD_FlashMode_t flashMode;  // This is the two   bits of D1_D0 of "Additional Flash Functions" of "Table-29 of Function of Column Address triplets"

    TTX_DISP_Attr_t attr;

    TTX_STD_Color_t bgc, fgc;

    const uint8_t fgctable = 0, bgctable = 0;   // Sparrow supports only one CLUT

    uint8_t col;
    uint8_t index = 0;

    bool_t bTransparent = FALSE;

    OSD_REQUIRE( fonts != NULL );

    if ( fonts != NULL )
    {
        // Output Display Page
        for ( col=startCol ; (col<numOfCols) && (col<TTX_DISP_MAX_ROW_LEN) ; col++ )
        {
            if ( OSDEMU_Uint32ToTtAttr( &(lineAttr[col]), &attr ) )
            {
                // if there is no character code, assign it "space" character
                if ( attr.charIdx == 0 )
                    attr.charIdx = ' ';

                // Subtitle/News flash.
                if ( hOsd->bBoxEnable )
                {
                    if ( attr.box == TTX_BOX_START )
                    {
                        bTransparent = FALSE;
                    }
                    else
                    {
                        bTransparent = TRUE;
                    }
                }


                if ( ! bTransparent )
                {
                    bgc = attr.bgColorIdx;
                    fgc = attr.fgColorIdx;

                    // Flashing
                    {
                        OSD_REQUIRE( hOsd->flashState.stepsCnt < hOsd->flashState.stepsTotal );

                        flashMode = attr.flashMode;

                        if ( flashMode != TTX_FLASH_STEADY )
                        {
                            if ( hOsd->flashState.stepsCnt >= (hOsd->flashState.stepsTotal>>1) )
                            {
                                fgc = bgc;
                            }
                        }   // end of if( flashMode != FLASH_STEADY )
                    }

                    if ( attr.conceal == TTX_CONCEAL_ON )
                    {
                        fgc = bgc; // process conceal
                    }
                    else
                    {
                        TTX_REQUIRE( attr.charIdx >= 0x20 );
                        index = attr.charIdx - 0x20;  // To use OSD_ttFont[]
                    }

                    // If this is a "space" character, we will perform this optimization.
                    if (    ( (attr.charset == TTX_CHARSET_G0_LATIN ) && ( index == 0 ) )  // In OSD_ttFont[][][] for Sparrow, index 0 of national option subsets such as TTX_CHARSET_G0_LATIN_CZECH_SLOVAK, TTX_CHARSET_G0_LATIN_ENGLISH to TTX_CHARSET_G0_LATIN_TURKISH are not SPACE character.
                            || ( fgc == bgc ) )
                    {
                        const TTX_BMPOSD_POINT_t p = { OSDEMU_ZOOM_H  * (pageShift + col * FONT_WIDTH) , OSDEMU_ZOOM_V * (row * FONT_HEIGHT)};
                        const TTX_BMPOSD_SIZE_t  s = { FONT_WIDTH * OSDEMU_ZOOM_H, FONT_HEIGHT * OSDEMU_ZOOM_V };
                        const OSD_CLUT_t*        pClut = &(hOsd->clut[bgctable][bgc]);

                        TTX_BMPOSD_PxFmt_t space;

                        OSD_REQUIRE( bgc < OSD_MAX_CLUT_ENTRY_NUM );
                        if (  bgc  < OSD_MAX_CLUT_ENTRY_NUM )
                        {
                            TTX_BMPOSD_SplitRGB( &space, pClut->a, pClut->r, pClut->g, pClut->b );  // Get space
#if !DV_SIM // Peformance bottleneck
                            OSDEMU_DrawSolidRect(hOsd->pBmpDisp, outputBufPixelsWidth, &p, &s, &space );
#endif
                        }
                    }
                    else    // else of if( lineAttr[col].cindex_l <= 0x20 )
                    {
                        {
                            // To use OSD_ttFont[]
                            {
                                // Get bitmap for the character.
                                if ( attr.charset < OSD_FONT_TOTAL_CHARSETS )
                                {
                                    if ( index < OSD_FONT_CHAR_NUM_PER_SET )
                                    {
                                        OSDEMU_GetCharBmp( &(bmpFont[0][0]), FONT_HEIGHT, FONT_WIDTH, (*fonts)[attr.charset][index], FONT_BYTES_PER_CHAR );
                                    }
                                }
                            }   // end of Added by Janus


                            // Added by by Janus
                            {
                                // Diacratical Process
                                if ( attr.dmIdx != TTX_DIAC_MARK_DISABLE )
                                {
                                    uint16_t diacri_bitmap[OSD_MAX_DM_LINE_NUM];

                                    // Get bitmap for diacritical characters.
                                    index =  attr.dmIdx - TTX_DIAC_MARK_NONE;

                                    //assert( index < OSD_MAX_DM_NUM );
                                    if ( index < OSD_MAX_DM_NUM )
                                    {
                                        memcpy(diacri_bitmap, hOsd->font.bmpDm[index], OSD_MAX_DM_LINE_NUM * sizeof(uint16_t) );

                                        // Combine diacritical character with normal character.
                                        OSD_ENSURE( OSD_MAX_DM_LINE_NUM <= FONT_HEIGHT );
                                        for ( i=0; i<OSD_MAX_DM_LINE_NUM; i++ )
                                        {
                                            for ( j=0; j<FONT_WIDTH; j++ )
                                            {
                                                bmpFont[i][j] |= ((diacri_bitmap[i] >>(FONT_WIDTH-j-1)) & 0x01);
                                            }
                                        }
                                    }

                                }   // end of if( attr.dm != TTX_DIAC_MARK_DISABLE )

                                /* Sparrow doesn't supports over line
                                                                // Overline Process
                                                                if( attr.ol )
                                                                {
                                                                    // Fill the top row of bmpFont with a horizontal bar
                                                                    for( i=0; i<FONT_WIDTH; i++ )
                                                                    {
                                                                        bmpFont[0][i] = 1;
                                                                    }
                                                                }
                                */
                                // Underline Process
                                if ( attr.bUnderline )
                                {
                                    // Fill the bottom row of bmpFont with a horizontal bar
                                    for ( i=0; i<FONT_WIDTH; i++ )
                                    {
                                        bmpFont[FONT_HEIGHT-1][i] = 1;
                                    }
                                }

                            }   // end of Added by Janus

                            // Mosaics Separated Process
                            // GRAPHIC_ON: OSD shouldn't perform smoothing on the bmpFont.
                            if ( (attr.mosaics == TTX_MOSAICS_SEP) && (attr.graphic == TTX_GRAPHIC_ON) )
                            {
                                OSDEMU_DrawMosaics( &(bmpFont[0][0]), FONT_HEIGHT, FONT_WIDTH ); // SEP_MOSAICS & Graphic
                            }

                            // Size Process
                            OSDEMU_CalculateSize( &(bmpFont[0][0]), FONT_HEIGHT, FONT_WIDTH, attr.size );
                        }

                        // Render hOsd->pBmpDisp from bmpFont[][].
                        {
                            const OSD_CLUT_t* pFgcTable = &(hOsd->clut[fgctable][fgc]);
                            const OSD_CLUT_t* pBgcTable = &(hOsd->clut[bgctable][bgc]);

                            TTX_BMPOSD_PxFmt_t fgColor, bgColor;
                            TTX_BMPOSD_SplitRGB( &fgColor, pFgcTable->a, pFgcTable->r, pFgcTable->g, pFgcTable->b );    // Get fgColor.
                            TTX_BMPOSD_SplitRGB( &bgColor, pBgcTable->a, pBgcTable->r, pBgcTable->g, pBgcTable->b );    // Get bgColor.

                            for ( j=0; j<FONT_HEIGHT; j++ )
                            {
                                const uint32_t pixelRow = OSDEMU_ZOOM_V * (row * FONT_HEIGHT + j);        // This is the actual row count on output bitmap.

                                for ( i=0; i<FONT_WIDTH; i++ )
                                {
                                    const TTX_BMPOSD_POINT_t pxPos = { OSDEMU_ZOOM_H*(pageShift + col*FONT_WIDTH + i), pixelRow };
                                    const TTX_BMPOSD_SIZE_t  s     = { OSDEMU_ZOOM_H, OSDEMU_ZOOM_V };
                                    if ( bmpFont[j][i] != 0 ) // foreground
                                    {
#if !DV_SIM // Peformance bottleneck
                                        OSDEMU_DrawSolidRect (hOsd->pBmpDisp, outputBufPixelsWidth, &pxPos, &s, &fgColor );
#endif
                                    }
                                    else // background
                                    {
#if !DV_SIM // Peformance bottleneck
                                        OSDEMU_DrawSolidRect (hOsd->pBmpDisp, outputBufPixelsWidth, &pxPos, &s, &bgColor );
#endif
                                    }
                                }
                            }

                        }
                    }   // End of if( lineAttr[col].ds.ci <= 0x20 )

                }   // End of if( ! bTransparent )
                else
                {
                    const TTX_BMPOSD_POINT_t p = { OSDEMU_ZOOM_H * (pageShift + col * FONT_WIDTH) , OSDEMU_ZOOM_V * (row * FONT_HEIGHT)};
                    const TTX_BMPOSD_SIZE_t  s = { FONT_WIDTH * OSDEMU_ZOOM_H, FONT_HEIGHT * OSDEMU_ZOOM_V };

                    OSDEMU_DrawTransparentRect(hOsd->pBmpDisp, outputBufPixelsWidth, &p, &s );

                }


            }   // end of if( OSDEMU_Uint32ToTtAttr( &(lineAttr[col]), &attr ) )
            else
            {
                OSD_ERROR();
            }

        }   // end of for( col=startCol ; (col<numOfCols) && (col<TTX_DISP_MAX_ROW_LEN) ; col++ )

    }   // end of if( fonts != NULL )
}

/*---------------------------------------------------------------------------*/
static void
OSDEMU_GetCharBmp           (uint8_t* bitmap, const uint8_t fontHeight, const uint8_t fontWidth,
                             const uint8_t* fontSrc, const uint8_t bytesPerChar)
/*****************************************************************************\
 *  @todo    Improve efficiency of this function!
 *  @brief   Draw the bitmap of a character.
 *  @details Draws a character that fontSrc[0] to fontSrc[bytesPerChar-1]
 *             to bitmap[0] to bitmap[fontWidth*fontHeight-1].
 *  @param   bitmap       the buffer to be painted
 *  @param   fontSrc[0] to fontSrc[bytesPerChar-1] is the font to be drawn
\*****************************************************************************/
{
    OSD_REQUIRE( bitmap && fontSrc );
    if ( bitmap && fontSrc )
    {
        uint8_t i, j;

        const int PIXELS_PER_BYTE = 8;  // ceil( fontHeight * fontWidth / bytesPerCha )

        const size_t OFFSET_PER_TWO_LINES = (fontWidth << 1)/PIXELS_PER_BYTE; // (Times Two) because each byte describes two lines. (Plus One) because the third byte counts despite that we only use its highest two bits.

        TTX_ENSURE( ((fontWidth << 1)%PIXELS_PER_BYTE) == 0 );

        memset( bitmap, 0, fontWidth*fontHeight*sizeof(bitmap[0]) );

        for ( i=0, j=0; (i<fontHeight-1)&&(j*OFFSET_PER_TWO_LINES<OSD_FONT_CHAR_SIZE); i+=2, j++ )  // (Minus One) because we have a trailing zero byte
        {
            const uint8_t k = i * fontWidth;
            const uint8_t m = k + fontWidth;    /* m = (i+1) * fontWidth */
            const uint8_t p = j * OFFSET_PER_TWO_LINES;

            // First line
            bitmap[k +  0] = ( fontSrc[p+0] >> 7 ) & 1;
            bitmap[k +  1] = ( fontSrc[p+0] >> 6 ) & 1;
            bitmap[k +  2] = ( fontSrc[p+0] >> 5 ) & 1;
            bitmap[k +  3] = ( fontSrc[p+0] >> 4 ) & 1;

            bitmap[k +  4] = ( fontSrc[p+1] >> 7 ) & 1;
            bitmap[k +  5] = ( fontSrc[p+1] >> 6 ) & 1;
            bitmap[k +  6] = ( fontSrc[p+1] >> 5 ) & 1;
            bitmap[k +  7] = ( fontSrc[p+1] >> 4 ) & 1;

            bitmap[k +  8] = ( fontSrc[p+2] >> 7 ) & 1;
            bitmap[k +  9] = ( fontSrc[p+2] >> 6 ) & 1;
            bitmap[k + 10] = ( fontSrc[p+2] >> 5 ) & 1;
            bitmap[k + 11] = ( fontSrc[p+2] >> 4 ) & 1;

            // Second line
            bitmap[m +  0] = ( fontSrc[p+0] >> 3 ) & 1;
            bitmap[m +  1] = ( fontSrc[p+0] >> 2 ) & 1;
            bitmap[m +  2] = ( fontSrc[p+0] >> 1 ) & 1;
            bitmap[m +  3] = ( fontSrc[p+0] >> 0 ) & 1;

            bitmap[m +  4] = ( fontSrc[p+1] >> 3 ) & 1;
            bitmap[m +  5] = ( fontSrc[p+1] >> 2 ) & 1;
            bitmap[m +  6] = ( fontSrc[p+1] >> 1 ) & 1;
            bitmap[m +  7] = ( fontSrc[p+1] >> 0 ) & 1;

            bitmap[m +  8] = ( fontSrc[p+2] >> 3 ) & 1;
            bitmap[m +  9] = ( fontSrc[p+2] >> 2 ) & 1;
            bitmap[m + 10] = ( fontSrc[p+2] >> 1 ) & 1;
            bitmap[m + 11] = ( fontSrc[p+2] >> 0 ) & 1;
        }
    }   // end of if( bitmap )
}

/*---------------------------------------------------------------------------*/
static void
OSDEMU_DrawMosaics  (uint8_t* bmp, const uint8_t fontHeight, const uint8_t fontWidth)
/*****************************************************************************\
 *  @brief   Draws seperated mosaic.
 *  @details To force pixels of column 0, 5, 6, 11 and pixels of
 *             four of ten rows (rows 0, 3, 6, 9) of character cell
 *             to background color when character is mosaic.
\*****************************************************************************/
{
    uint8_t i, j;


    OSD_REQUIRE( fontWidth  == 12 );   // ERROR: this implementation is for font width  in 12 pixels only.
    OSD_REQUIRE( fontHeight == 10 );   // ERROR: this implementation is for font height in 10 pixels only.

    for ( j=0; j<fontHeight; j++ )
    {
        const uint8_t m = j*fontWidth;

        switch ( j )
        {
        case 0:
        case 3:
        case 6:
        case 9:
        {
            memset(&(bmp[m+0]), 0x00, fontWidth);
            break;
        }
        }

        for ( i=0; i<fontWidth; i++ )
        {
            switch ( i )
            {
            case 0:
            case 5:
            case 6:
            case 11:
            {
                bmp[m+i] = 0x00;
                break;
            }
            }

        }
    }

}

/*---------------------------------------------------------------------------*/
static void
OSDEMU_CalculateSize    (uint8_t* bmp, const uint8_t fontHeight, const uint8_t fontWidth,
                         const TTX_STD_SizeCfg_t sizecfg)
/*****************************************************************************\
 *  @brief   Draw the bitmap of a character.
 *  @details Draws a character that fontSrc[0] to fontSrc[bytesPerChar-1]
 *             to bmp[0] to bmp[fontWidth*fontHeight-1].
 *  @param   bmp[]      the buffer to be painted
 *  @param   fontSrc[0] to fontSrc[bytesPerChar-1] is the font to be drawn
\*****************************************************************************/
{
    int i, j, m, n;
    uint8_t midHeightRow = fontHeight >> 1;
    uint8_t midWidthCol  = fontWidth  >> 1;

    switch ( sizecfg )
    {
    default:
    case TTX_STD_SIZE_CFG_NORMAL:
        break;
    case TTX_STD_SIZE_CFG_DH_UPPER:
        for (j=(midHeightRow-1); j>=0; j--)
        {
            m = j*fontWidth;
            n = (j<<1)*fontWidth;
            memcpy(&bmp[n          +0], &bmp[m+0], fontWidth);
            memcpy(&bmp[n+fontWidth+0], &bmp[m+0], fontWidth);
        }
        break;
    case TTX_STD_SIZE_CFG_DH_LOWER:
        for (j=midHeightRow; j<fontHeight; j++)
        {
            m = j*fontWidth;
            n = ((j-midHeightRow)<<1)*fontWidth;
            memcpy(&bmp[n+fontWidth+0], &bmp[m+0], fontWidth);
            memcpy(&bmp[n          +0], &bmp[m+0], fontWidth);
        }
        break;
    case TTX_STD_SIZE_CFG_DW_LEFT:
        for (j=0; j<fontHeight; j++)
        {
            m = j*fontWidth;
            for (i=(midWidthCol-1); i>=0; i--)
            {
                bmp[m+((i<<1)  )] = bmp[m+i];
                bmp[m+((i<<1)+1)] = bmp[m+i];
            }
        }
        break;
    case TTX_STD_SIZE_CFG_DW_RIGHT:
        for (j=0; j<fontHeight; j++)
        {
            m = j*fontWidth;
            for (i=midWidthCol; i<fontWidth; i++)
            {
                n = ((i-midWidthCol)<<1);
                bmp[m+n  ] = bmp[m+i];
                bmp[m+n+1] = bmp[m+i];
            }
        }
        break;
    case TTX_STD_SIZE_CFG_DS_L_UPPER:
        for (j=0; j<fontHeight; j++)
        {
            m = j*fontWidth;
            for (i=(midWidthCol-1); i>=0; i--)
            {
                n = (i<<1);
                bmp[m+n  ] = bmp[m+i];
                bmp[m+n+1] = bmp[m+i];
            }
        }

        for (j=(midHeightRow-1); j>=0; j--)
        {
            m = j*fontWidth;
            n = ((j<<1) * fontWidth);
            for (i=0; i<fontWidth; i++)
            {
                bmp[n+fontWidth+i] = bmp[m+i];
                bmp[n          +i] = bmp[m+i];
            }
        }
        break;
    case TTX_STD_SIZE_CFG_DS_L_LOWER:
        for (j=0; j<fontHeight; j++)
        {
            m = j*fontWidth;
            for (i=(midWidthCol-1); i>=0; i--)
            {
                n = (i<<1);
                bmp[m+n  ] = bmp[m+i];
                bmp[m+n+1] = bmp[m+i];
            }
        }
        for (j=midHeightRow; j<fontHeight; j++)
        {
            m = j*fontWidth;
            n = ((j-midHeightRow)<<1)*fontWidth;
            for (i=0; i<fontWidth; i++)
            {
                bmp[j+i+fontWidth] = bmp[m+i];
                bmp[j+i          ] = bmp[m+i];
            }
        }
        break;
    case TTX_STD_SIZE_CFG_DS_R_UPPER:
        for (j=0; j<fontHeight; j++)
        {
            m = j*fontWidth;
            for (i=midWidthCol; i<fontWidth; i++)
            {
                n = ((i-midWidthCol)<<1);
                bmp[m+n  ] = bmp[m+i];
                bmp[m+n+1] = bmp[m+i];
            }
        }
        for (j=(midHeightRow-1); j>=0; j--)
        {
            m = j*fontWidth;
            n = (j<<1)*fontWidth;
            for (i=0; i<fontWidth; i++)
            {
                bmp[n+fontWidth+i] = bmp[m+i];
                bmp[n          +i] = bmp[m+i];
            }
        }
        break;
    case TTX_STD_SIZE_CFG_DS_R_LOWER:
        for (j=0; j<fontHeight; j++)
        {
            m = j*fontWidth;
            for (i=midWidthCol; i<fontWidth; i++)
            {
                n = ((i-midWidthCol)<<1);
                bmp[m+n  ] = bmp[m+i];
                bmp[m+n+1] = bmp[m+i];
            }
        }
        for (j=midHeightRow; j<fontHeight; j++)
        {
            m = j*fontWidth;
            n = ((j-midHeightRow)<<1)*fontWidth;
            for (i=0; i<fontWidth; i++)
            {
                bmp[n          +i] = bmp[m+i];
                bmp[n+fontWidth+i] = bmp[m+i];
            }
        }
        break;
    }
}

/*---------------------------------------------------------------------------*/
static bool_t
OSDEMU_DrawSolidRect (      TTX_BMPOSD_PxFmt_t* pOutputBuf,            \
                            const size_t              outputBufPixelsWidth,  \
                            const TTX_BMPOSD_POINT_t* pPointUpperLeft,       \
                            const TTX_BMPOSD_SIZE_t*  pSize,                 \
                            const TTX_BMPOSD_PxFmt_t* pColorToFill           )
/*****************************************************************************\
 *  @brief Draw a solid-filled rectange using the color *pColorToFill.
 *  @param pOutputBuf           is a virtually two-dimension array that to be drawn. Its width is outputBufPixelsWidth.
 *  @param outputBufPixelsWidth is the width of pOutputBuf[][].
 *  @param *pPointUpperLeft     is the upper-left corner of the rectangle to be drawn.
 *                              Coordinate system's origin is on the upper-left corner and
 *                                 its x towards right horizontally while y towards down vertically.
 *  @param *pSize               the number of pixels of width and height of the rectangle to be drwan.
 *  @param *pColorToFill            the color to draw the solid-filled rectangle.
 *  @post  pOutputBuf will contain the solid rectangle filled with *pColorToFill.
\*****************************************************************************/
{
    if ( ( pSize != NULL ) && ( pColorToFill != NULL ) )
    {
        int i;
        const size_t bytesWidth  = pSize->cx * sizeof( TTX_BMPOSD_PxFmt_t );
        const size_t start = pPointUpperLeft->y*outputBufPixelsWidth + pPointUpperLeft->x;
        size_t now;
        if ( ( pOutputBuf != NULL ) && ( pPointUpperLeft != NULL ) )
        {
            TTX_BMPOSD_POINT_t pxPos;

            pxPos.y = pPointUpperLeft->y;
            /*
                        // Slower but safer
                        {
                            int32_t j;
                            for( i = 0; i< pSize->cy; i++ )
                            {
                                pxPos.y = pPointUpperLeft->y + i;
                                for( j=0; j<pSize->cx; j++ )
                                {
                                    pxPos.x = pPointUpperLeft->x + j;
                                    OSDEMU_DrawPixel( pOutputBuf, outputBufPixelsWidth, &pxPos , pColorToFill );
                                }
                            }
                        }
            */

            // Faster
            {
                // Render the first horizontal line of the rectangle.
                for ( i=0; i< pSize->cx; i++ )
                {
                    pxPos.x = pPointUpperLeft->x + i;
                    OSDEMU_DrawPixel( pOutputBuf, outputBufPixelsWidth, &pxPos, pColorToFill );
                }

                // Rendering remaining horizontal lines of the rectangle by
                //   copying the first line.
                now = ((pPointUpperLeft->y + 1)*outputBufPixelsWidth + pPointUpperLeft->x);
                for ( i=1; i<pSize->cy; i++ )
                {
                    memcpy( &(pOutputBuf[now]), &(pOutputBuf[start]) , bytesWidth);
                    now += outputBufPixelsWidth;
                }

                /* Slower, but easier to understand above for-loop.
                                for( i=1; i<pSize->cy; i++ )
                                {
                                    const size_t now = (pPointUpperLeft->y + i)*outputBufPixelsWidth + pPointUpperLeft->x;
                                    memcpy( &(pOutputBuf[now]), &(pOutputBuf[start]) , bytesWidth);
                                }
                */
            }

            return( TRUE );
        }
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
static bool_t
OSDEMU_DrawTransparentRect (TTX_BMPOSD_PxFmt_t* outputBuf, const size_t outputBufPixelsWidth, \
                            const TTX_BMPOSD_POINT_t* pPointUpperLeft, const TTX_BMPOSD_SIZE_t* pSize)
/*****************************************************************************\
 *  @brief Draw a solid-filled rectange using the color *pRgbFill.
 *  @param outputBuf            is a virtually two-dimension array that to be drawn. Its width is outputBufPixelsWidth.
 *  @param outputBufPixelsWidth is the width of outputBuf[][].
 *  @param *pPointUpperLeft     is the upper-left corner of the rectangle to be drawn.
 *                              Coordinate system's origin is on the upper-left corner and
 *                                 its x towards right horizontally while y towards down vertically.
 *  @param *pSize               the number of pixels of width and height of the rectangle to be drwan.
 *  @param *pRgbFill            the color to draw the solid-filled rectangle.
 *  @post  outputBuf will contain the solid rectangle filled with *pRgbFill.
\*****************************************************************************/
{
#if !DV_SIM // Peformance bottleneck
    const TTX_BMPOSD_PxFmt_t rgbFill  = TTX_BMPOSD_COLOR_TRANSPARENCY;
    OSDEMU_DrawSolidRect(outputBuf, outputBufPixelsWidth, pPointUpperLeft, pSize, &rgbFill );
#endif

    return( TRUE );
}

/*---------------------------------------------------------------------------*/
static bool_t
OSDEMU_DrawPixel       (      TTX_BMPOSD_PxFmt_t* pOutputBuf,           \
                              const size_t              outputBufPixelsWidth, \
                              const TTX_BMPOSD_POINT_t* pPxPos,               \
                              const TTX_BMPOSD_PxFmt_t* pColor                )
/*****************************************************************************\
 *  @brief Draw a single pixel at *pPxPos with *pColor.
 *  @param pOutputBuf            is a virtually two-dimension array that to be drawn. Its width is outputBufPixelsWidth.
 *  @param outputBufPixelsWidth is the width of pOutputBuf[][].
 *  @param *pPointUpperLeft     is the upper-left corner of the rectangle to be drawn.
 *                              Coordinate system's origin is on the upper-left corner and
 *                                 its x towards right horizontally while y towards down vertically.
 *  @param *pPxPos              x is the horizontal distance
 *                              y is the vertical   distance
 *  @param *pColor              the color to draw the pixel.
 *  @post  pOutputBuf will contain the pixel filled with *pColor.
\*****************************************************************************/
{
    if ( pPxPos != NULL )
    {
        const size_t offset = pPxPos->y * outputBufPixelsWidth + pPxPos->x;
        if ( ( pOutputBuf != NULL ) && ( pColor != NULL ) )
        {
            pOutputBuf[ offset ] = *pColor;

            return( TRUE );
        }
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
bool_t
OSDEMU_Uint32ToTtAttr  (const volatile OSD_TtAttr_t* pRaw, TTX_DISP_Attr_t* pAttr)
/*****************************************************************************\
 *  @brief   Converts machine readable attribute data (uint32_t) stored in
 *             OSD memory to "emulator" readable format (OSD_TtAttr_t).
 *  @param   *pRaw  is the machine readable attribute data.
 *  @param   *pAttr will be the converted human readable attribute data.
 *
 *  @post    (*hOsd->font.pBmpFonts)[pAttr->csel][pAttr->cindex][] will be
 *              the selected character set and index.
 *
 *  @return  the conversion status.
 *  @retval  TTX_TRUE  success
 *  @retval  TTX_FALSE failure
 *
 *  @see     OSD_Uint32ToTtAttr()
\*****************************************************************************/
{
    if ( (pRaw != NULL)&&(pAttr != NULL) )
    {
        TTX_REQUIRE( sizeof(OSD_TtAttr_t) >= 4 );    /* ERROR: The following implementation requires OSD_TtAttr_t greater than or equal to 32-bit */

        pAttr->conceal    =  (*pRaw >> 31) & 0x01;
        pAttr->box        =  (*pRaw >> 30) & 0x01;
        pAttr->graphic    =  (*pRaw >> 29) & 0x01;
        pAttr->size       =  (*pRaw >> 24) & 0x0F;
        pAttr->bUnderline =  (*pRaw >> 22) & 0x01;
        pAttr->mosaics    =  (*pRaw >> 21) & 0x01;
        pAttr->dmIdx      =  (*pRaw >> 16) & 0x1F;
        pAttr->flashMode  =  (*pRaw >> 15) & 0x01;
        pAttr->fgColorIdx =  (*pRaw >> 12) & 0x07;
        pAttr->bgColorIdx =  (*pRaw >>  9) & 0x07;
        pAttr->charset    = ((*pRaw >>  7) & 0x03)|((*pRaw>>(23-2))&0x04);
        pAttr->charIdx    =  (*pRaw      ) & 0x7F;

        return( TRUE );
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
static bool_t
OSDEMU_X0isChanged          (const uint8_t originalRow0[PKT_BUF_SIZE_BYTES],
                             const uint8_t receivedRow0[PKT_BUF_SIZE_BYTES])
/*****************************************************************************\
 *  @brief   Determine X/0 has been changed by judging previously received
 *             originalTimeCode and latest receivedTimeCode.
 *  @param   originalRow0 is previously received X/0.
 *  @param   receivedRow0 is latest     received X/0.
\*****************************************************************************/
{
    const uint8_t* const pOriginalTimecode = &(originalRow0[PKT_BUF_SIZE_BYTES-OSDEMU_TIMECODE_LEN]);
    const uint8_t* const pReceivedTimecode = &(receivedRow0[PKT_BUF_SIZE_BYTES-OSDEMU_TIMECODE_LEN]);

    /* Check the time code to determine if X/0 has been updated or not. */
    if ( pOriginalTimecode[OSDEMU_TIMECODE_LEN-1] == pReceivedTimecode[OSDEMU_TIMECODE_LEN-1] ) /* compare the last digit of time-code */
    {
        if ( memcmp( pOriginalTimecode, pReceivedTimecode, OSDEMU_TIMECODE_LEN-1 ) == 0 )   /* compare the remaining part of time-code */
        {
            const uint8_t* const pOriginalHeader = &(originalRow0[OSD_MAX_PNUM_LEN]);
            const uint8_t* const pReceivedHeader = &(receivedRow0[OSD_MAX_PNUM_LEN]);
            return( memcmp( pOriginalHeader, pReceivedHeader, PKT_BUF_SIZE_BYTES-OSDEMU_TIMECODE_LEN-OSD_MAX_PNUM_LEN ) != 0 ); /* compare the header (without both page-number and time-code) */
        }
    }
    return( TRUE );
}


/*---------------------------------------------------------------------------*/
static bool_t
OSDEMU_DecodeRow0  (TTX_DISP_Attr_t decodedDispAttr[TTX_DISP_MAX_ROW_LEN], const uint8_t TTX_MEM* src, const bool_t rolling)
/*****************************************************************************\
 *  @brief   Decode content of src[] to decoded[].
 *  @param   src[0] to src[PKT_BUF_SIZE_BYTES-1] is the decoded row-0.
 *  @post    decoded[] will be updated.
 *  @return  the initialization result.
 *  @see     TTX_DecodePage()
\*****************************************************************************/
{
    /* Use private functions of TTX_DEC module */
    extern bool_t  TTX_PacketParse         (TTX_STD_Packet_t*   pPkt, const uint8_t src[PKT_BUF_SIZE_BYTES]);    // Parse src[0]~src[41] to TTX_STD_Packet_t
    extern bool_t  TTX_DecodeLine          (TTX_DISP_Attr_t dest[TTX_DISP_MAX_ROW_LEN], TTX_STD_Packet_t* pPkt,
                                            TTX_LopParam_t* pParam, uint8_t* pIgnoreLine, const bool_t rolling);

    uint8_t             row, ignoreLine;
    uint8_t             i;
    TTX_LopParam_t      param;
    TTX_STD_Packet_t    pkt;

    const uint8_t startRow = 0;
    const uint8_t endRow   = 0;

    OSD_REQUIRE( startRow <= endRow );

    // Channel decode src page to "cd mem"
    TTX_BUF_DecodeX0( src );

#if TTX_CFG_PRESENT_LEVEL >= TTX_STD_PRESENT_LEVEL(25)
    // @todo Port level 2.5 functions here.
#endif  /* end of #if TTX_CFG_PRESENT_LEVEL >= TTX_STD_PRESENT_LEVEL(25) */

    /* Initialize param from information provided by src[] and startRow */
    OSDEMU_InitLopParam( &param, &(pkt.addr.magNum), src, startRow );  // Each packets within the same page has the same magazine-number.

    ignoreLine = 0xFF;  // do not ignore any lines


    for ( row = startRow; row <= endRow; row++ )
    {
        // Set Default Value of Attribute
        if ( ignoreLine == 0xFF )
        {
            memset(decodedDispAttr, 0, TTX_DISP_MAX_ROW_LEN * sizeof(TTX_DISP_Attr_t) );
            for ( i=0; i<TTX_DISP_MAX_ROW_LEN; i++ )
            {
                decodedDispAttr[i].fgColorIdx  = TTX_COLOR_WHITE;
                decodedDispAttr[i].charIdx     = 0x10;
                decodedDispAttr[i].charset     = param.DEFAULT_G0_CHARSET;
            }
        }

        pkt.addr.pktNum = row;  // packet number equals row number

        // Get decoded packet from "cd mem" and then parse it.
        TTX_PacketParse( &pkt, TTX_BUF_GetDecodedPacket( row ) );

        // rolling header
        if ( rolling )
        {
            pkt.dataBytes[7] = 0x02; // specify rolling header foreground color to green
        }

        // Decode the pkt to decodedDispAttr[].
        TTX_DecodeLine( decodedDispAttr, &pkt, &param, &ignoreLine, rolling );
    }


    return( TRUE );
}

/*---------------------------------------------------------------------------*/
static bool_t
OSDEMU_InitLopParam         (TTX_LopParam_t* pParam, TTX_STD_MagazineNum_t* pMag,
                             const uint8_t TTX_MEM* src, const uint8_t rowNum)
/*****************************************************************************\
 *  @brief   Initialize a TTX_LopParam_t for decoding.
 *  @param   *pParam will the initialized param.
 *  @param   If( rowNum == 0 ), *pMag will be the magazine number.
 *  @param   src[0]~[39]  is the channel decoded data of rowNum.
 *  @param   rowNum       is the packet number of src[].
 *  @post    *pParam will be updated.
 *  @return  the initialization result.
 *  @see     TTX_InitLopParam()
\*****************************************************************************/
{
    extern bool_t  TTX_PacketX0Parse       (TTX_STD_PacketX0_t* pPkt, const uint8_t src[PKT_BUF_SIZE_BYTES]);    // Parse src[0]~src[41] to TTX_STD_PacketX0_t

    uint8_t             Mode;
    TTX_STD_PacketX0_t  pktX0;
    uint8_t             c12c13c14, default_G0G2_Char_Dcode, second_G0_Char_Dcode;
    TTX_STD_CharSet_t   DEFAULT_G0_CHARSET,SECOND_G0_CHARSET;
    uint32_t            word_data = 0;

    // set Micronas like(0) / Philips like(1) for DS/DH/DW
    word_data |= TTX_SIZE_ATTR_MODE_PHILIPS << 1;

    // set X/0 & X/24 box enable
    word_data |= UINT32_C(0x01) << 29;

    // set X/24 enable
    word_data |= UINT32_C(0x00) << 28; //timhuang@20061103: for auto update

    // set new background enable
    word_data |= UINT32_C(0x01) << 26;

    // set esc enable
    word_data |= UINT32_C(0x01) << 25;

    // set Alpha black enable
    word_data |= UINT32_C(0x01) << 24;

    // set double width
    word_data |= UINT32_C(0x01) << 23;

    // set double size
    word_data |= UINT32_C(0x01) << 22;

    // set mosaic black
    word_data |= UINT32_C(0x01) << 21;

    // set G1
    word_data|= ((uint32_t)TTX_CHARSET_G1_MOSAICS) << 13;

    if ( rowNum == 0 )
    {
        // Parse src[] and update packet X/0
        TTX_PacketX0Parse( &pktX0, src );

        if ( pktX0.ctrlBits.bSuppressHeader )
        {
            word_data|= UINT32_C(0x01) << 30;
            //TTX_DISP_EnableTimecode( FALSE );   // Don't display timecode
        }
        else
        {
            //TTX_DISP_EnableTimecode( TRUE );    // Display timecode
        }

        // set subtitle/newflash enable
        if ( pktX0.ctrlBits.bNewsFlash || pktX0.ctrlBits.bSubtitle )
        {
            word_data|= UINT32_C(0x01) << 27;
            //TTX_DISP_EnableBox( TRUE );
        }
        else
        {
            //TTX_DISP_EnableBox( FALSE );
        }

        //  Set VBI inhibit enable
        if ( pktX0.ctrlBits.bInhibitDisplay )
            word_data |= UINT32_C(1)<<12;

        c12c13c14 = pktX0.ctrlBits.natlOpt;
        default_G0G2_Char_Dcode = second_G0_Char_Dcode = 0;

        TTX_STD_GetCharSetFromDefaultG0G2( default_G0G2_Char_Dcode, c12c13c14, &DEFAULT_G0_CHARSET );
        if ( DEFAULT_G0_CHARSET == TTX_CHARSET_RESERVED )
            DEFAULT_G0_CHARSET = TTX_CHARSET_G0_LATIN;

        TTX_STD_GetCharSetFromDefaultG0G2( second_G0_Char_Dcode, c12c13c14, &SECOND_G0_CHARSET );
        if ( SECOND_G0_CHARSET == TTX_CHARSET_RESERVED )
            SECOND_G0_CHARSET = TTX_CHARSET_G0_LATIN;

        // set national option
        word_data |= ((uint32_t)c12c13c14) << 18;

        // set default G0
        word_data |= ((uint32_t)DEFAULT_G0_CHARSET) << 3;

        // set second G0
        word_data |= ((uint32_t)SECOND_G0_CHARSET) << 8;

        if ( pMag != NULL )
        {
            *pMag = pktX0.addr.magNum;
        }
    }


    // set transfer operation mode
    Mode = 0;//WITH_SDRM_MODE;
    word_data|= ((uint32_t)Mode) << 1;


    if ( pParam != NULL )
    {
        // decide char set
        pParam->DEFAULT_G0_CHARSET = (word_data>>3)&0x1F;
        pParam->SECOND_G0_CHARSET  = (word_data>>8)&0x1F;
        pParam->G1_CHARSET         = (word_data>>13)&0x1F;
        pParam->C12C13C14          = (word_data>>18)&0x7;

        // L1.5 or L2.5
        pParam->Alpha_Black_Enable   = (word_data>>24)&0x1;
        pParam->Double_Width_Enable  = (word_data>>23)&0x1;
        pParam->Double_Size_Enable   = (word_data>>22)&0x1;
        pParam->ESC_Enable           = (word_data>>25)&0x1;
        pParam->MOSAICS_BLACK_Enable = (word_data>>21)&0x1;
        pParam->NewBackground_Enable = (word_data>>26)&0x1;
        pParam->SizeAttrMode         = (word_data>>31)&0x1;


        // Sub-title or Newflash
        pParam->Newsflash_Subtitle = (word_data>>27)&0x1;

        // X24 Process
        pParam->X24_Enable         = (word_data>>29)&0x1;

        // Mode Select
        pParam->Mode = Mode;    // 0: with DRAM Mode

        // Color LUT
        pParam->FG_CLUT     = (0>>5)&0x3;
        pParam->BG_CLUT     = (0>>3)&0x3;
        pParam->REMAP_CLUT  = (0>>7)&0x3;
        pParam->Default_BGC = TTX_COLOR_BLACK;

        return( TRUE );
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
static bool_t
OSDEMU_DrawScreenAsTransparent  (OSDEMU_Handle hOsd)
/*****************************************************************************\
 *  @brief   Set entire screen as transparent.
 *  @param   pOddParityDecodedData[0]~[PKT_BUF_SIZE_BYTES-1] is the odd-parity
 *             decoded data of received X/0.
\*****************************************************************************/
{
    if ( hOsd != NULL )
    {
        const uint8_t FONT_WIDTH           = hOsd->font.pxWidth;     // number of pixels of width  of a character
        const uint8_t FONT_HEIGHT          = hOsd->font.pxHeight;        // number of pixels of height of a character in present resolution
        const size_t  pxWidth              = TTX_DISP_MAX_ROW_LEN * FONT_WIDTH  * OSDEMU_ZOOM_H;
        const size_t  outputBufPixelsWidth = pxWidth;

        const TTX_BMPOSD_POINT_t p = { 0, 0 };  /* upper-left point */
        const TTX_BMPOSD_SIZE_t  s = { TTX_DISP_MAX_ROW_LEN_WITHOUT_SP * FONT_WIDTH  * OSDEMU_ZOOM_H,
                                       TTX_DISP_MAX_ROW_NUM            * FONT_HEIGHT * OSDEMU_ZOOM_V
                                     };

        return( OSDEMU_DrawTransparentRect( hOsd->pBmpDisp, outputBufPixelsWidth, &p, &s ) );
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
void
OSDEMU_onReceivedX0         (const uint8_t*                 pReceivedRow0,
                             const TTX_STD_Page_CtrlBits_t* pCtrlBits)
/*****************************************************************************\
 *  @brief   Used to implements rolling header and time-code.
 *  @param   pOddParityDecodedData[0]~[PKT_BUF_SIZE_BYTES-1] is the odd-parity
 *             decoded data of received X/0.
\*****************************************************************************/
{
    OSDEMU_Handle hOsd = &osdEmu;

    if ( pCtrlBits != NULL )
    {
        if ( hOsd->header.bRoll && pCtrlBits->bInterruptedSequence )
        {
            return; /* package is out of order. Don't update header. */
        }

#if 0
        if ( pCtrlBits->bInterruptedSequence )
        {
            return; /* package is out of order. Don't update header. */
        }
#endif

        if ( pCtrlBits->bInhibitDisplay )
        {
            /* broadcaster don't want this page to be displayed. Including X/0.
             * pattern: P100 of AUS_20040331_7Digital_TTX_Subtitle_CommonPlay_pid0x204.PES
             */
            return;
        }

        if ( pCtrlBits->bSuppressHeader )
        {
            return;
        }
#if 0
        if ( pCtrlBits->bSubtitle )
        {
            /* Do not draw X/0 of subtitle pages */
            return;
        }
#endif
    }

    /* For debug time-code
    {
        TTX_DISP_Attr_t     x0Latest[TTX_DISP_MAX_ROW_LEN];  // X/0 buffer used to be drawn
        TTX_STD_CharIndex_t timecode[OSDEMU_TIMECODE_LEN];

        if( OSDEMU_DecodeRow0( x0Latest, pReceivedRow0 ) )
        {
            size_t i;
            for( i=TTX_DISP_MAX_ROW_LEN_WITHOUT_SP-OSDEMU_TIMECODE_LEN; i<TTX_DISP_MAX_ROW_LEN_WITHOUT_SP; i++ )
            {
                timecode[i-(TTX_DISP_MAX_ROW_LEN_WITHOUT_SP-OSDEMU_TIMECODE_LEN)] = x0Latest[i].charIdx;
            }

            TTX_Log(LOGGER_VERBOSE, "time: ");

            for( i=0; i<OSDEMU_TIMECODE_LEN; i++ )
            {
                TTX_Log(LOGGER_VERBOSE, "%c", timecode[i]);
            }

            TTX_Log(LOGGER_VERBOSE, "\t%d, %d, %d, %d | %d, %d, %d, %d | %d", pCtrlBits->bErasePage,
                                                                pCtrlBits->bNewsFlash,
                                                                pCtrlBits->bSubtitle,
                                                                pCtrlBits->bSuppressHeader,
                                                                pCtrlBits->bUpdate,
                                                                pCtrlBits->bInterruptedSequence,
                                                                pCtrlBits->bInhibitDisplay,
                                                                pCtrlBits->bSerialMode,
                                                                pCtrlBits->natlOpt);
            TTX_Log(LOGGER_VERBOSE, "\n");
        }
    }*/

    if ( pReceivedRow0 != NULL )
    {
        //OSDEMU_Handle hOsd = &osdEmu;

        /* Only update hOsd->x0LatestRow0Raw[] and hOsd->latestRow0[] when needed */
        if (hOsd->header.bDisplay && (hOsd->header.bRoll || hOsd->timecode.bDisplay) )
        {
            /* Detect changes since previously received row0 (hOsd->latestRow0Raw) with
             *   this new received one.
             */
            if ( OSDEMU_X0isChanged( hOsd->latestRow0Raw, pReceivedRow0 ) )
            {
                TTX_DISP_Attr_t x0Latest[TTX_DISP_MAX_ROW_LEN];  // X/0 buffer used to be drawn

                if ( hOsd->header.bRoll )  // rolling header
                {
                    memcpy( hOsd->latestRow0Raw, pReceivedRow0, PKT_BUF_SIZE_BYTES );   /* update hosd->latestRow0Raw[] */
                }
                else   // only update timecode
                {
                    if ( hOsd->latestRow0Raw[PKT_BUF_SIZE_BYTES-9] == 0 )
                        memcpy( hOsd->latestRow0Raw, pReceivedRow0, PKT_BUF_SIZE_BYTES );   /* update hosd->latestRow0Raw[] */
                    else
                        memcpy( &hOsd->latestRow0Raw[PKT_BUF_SIZE_BYTES-8], pReceivedRow0+PKT_BUF_SIZE_BYTES-8, 8 );   /* update timecode */
                }

                if ( OSDEMU_DecodeRow0( x0Latest, pReceivedRow0, hOsd->header.bRoll) )
                {
                    size_t i;
                    for ( i=0; i<TTX_DISP_MAX_ROW_LEN; i++ )
                    {
                        TTX_DISP_AttrToOsdAttr( &(x0Latest[i]), &(hOsd->latestRow0[i]) );
                    }
                }

                hOsd->bRefreshRow0 = TRUE;  /* signals the screen needs to be refreshed to update the rolling header or time-code. */
            }
        }
    }
}

/*---------------------------------------------------------------------------*/
static bool_t
OSDEMU_onUp  (OSDEMU_Handle hOsd)
/*****************************************************************************\
 *  @brief   This event handler should be invoked whenever hOsd->bIsDisplaying
 *            from FALSE to TRUE.
\*****************************************************************************/
{
    if ( hOsd != NULL )
    {
        /* Draw entire screen as transparent */
        /* There's no need to call TTX_BMPOSD_Draw() here because
         *   it will be invoked later in OSDEMU_onRefreshingScreen().
         */
        return( OSDEMU_DrawScreenAsTransparent( hOsd ) );
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
static bool_t
OSDEMU_onDown  (OSDEMU_Handle hOsd)
/*****************************************************************************\
 *  @brief   This event handler should be invoked whenever hOsd->bIsDisplaying
 *            from TRUE to FALSE.
\*****************************************************************************/
{
    if ( hOsd != NULL )
    {
        /* Draw entire screen as transparent */
        if ( OSDEMU_DrawScreenAsTransparent( hOsd ) )
        {
            TTX_BMPOSD_Draw( hOsd->pBmpDisp );
            return( TRUE );
        }
    }
    return( FALSE );
}





#endif  /* end of #if OSD_SOFTWARE_EMULATOR */


/*---------------------------------------------------------------------------*\
 * end of osd_emu.c                                                          *
\*---------------------------------------------------------------------------*/

