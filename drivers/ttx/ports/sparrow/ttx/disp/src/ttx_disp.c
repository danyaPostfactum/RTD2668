/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_disp.c
 * @brief    Teletext output display interface.
 * @author   Janus Cheng
 * @date     Aug. 21, 2009
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/
#ifdef CONFIG_TTX_SUPPORT

#include <ttx_disp.h>   /* Provides: functions to be implemented */
#include <ttx_port.h>   /* Provides: TTX_USE_MALLOC */

#include <osd.h>        /* Provides: OSD/OSD_EMU module */

#include <string.h>     /* Provides: memset() */

TTX_DEFINE_THIS_MODULE( ttx_disp.c )

#if OSD_SOFTWARE_EMULATOR

#include "osd_emu.h"    /* Provides: OSDEMU_Handle */

#if TTX_USE_MALLOC
#include <malloc.h>         /* Provides: malloc() and free() */
#else
/* Helper object for pMemBase when #if !TTX_USE_MALLOC */
#define _STATIC_TTX_DISP_MEM_SIZE  (0x4000) /* the memory size in bytes used when not using TTX_USE_MALLOC */
OSD_ASSERT_COMPILE( _STATIC_TTX_DISP_MEM_SIZE % sizeof( OSD_TtAttr_t ) == 0 );
static uint8_t           _gpMemBase [_STATIC_TTX_DISP_MEM_SIZE];
#endif  /* end of #if TTX_USE_MALLOC */

typedef struct TTX_DISP_Obj
{
    int32_t         cntUsage;
    OSD_TtAttr_t*   pMemBase;
} TTX_DISP_Obj, *TTX_DISP_Handle;

static TTX_DISP_Obj dispObj =
{
    0,          /* cntUsage   */

#if TTX_USE_MALLOC
    NULL        /* pMemBase   */
#else
    (OSD_TtAttr_t*)_gpMemBase
#endif  /* end of #if TTX_USE_MALLOC */

};

#define TTX_DISP_MEM_START_ADDRESS          ((OSD_Address_t)dispObj.pMemBase)
#else
#define TTX_DISP_MEM_START_ADDRESS          (0xC000)

#include <vd.h>         /* Provides: Video Decoder module */

#endif  /* end of #if OSD_SOFTWARE_EMULATOR */


#define OSD_DISP_ATTR_SIZE              (sizeof(OSD_TtAttr_t))     // bytes

#if 0 // cannot cross bank to access data
uint16_t const TTX_ROM OSD_ttDmBitmap[OSD_MAX_DM_NUM][OSD_MAX_DM_LINE_NUM] =
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
#endif

OSD_CLUT_t OSD_clutValue[OSD_TOTAL_CLUT_NUM][OSD_MAX_CLUT_ENTRY_NUM] =
{
#if OSD_SOFTWARE_EMULATOR
    // CLUT 0
    // (R G B A)
    {
        {0x00, 0x00, 0x00, 0xff},
        {0xff, 0x00, 0x00, 0xff},
        {0x00, 0xff, 0x00, 0xff},
        {0xff, 0xff, 0x00, 0xff},
        {0x00, 0x00, 0xff, 0xff},
        {0xff, 0x00, 0xff, 0xff},
        {0x00, 0xff, 0xff, 0xff},
        {0xff, 0xff, 0xff, 0xff}
    }
#else
    // <Janus>2008/11/12: Because OSD_Rgb2Yuv() cannot obtain the following results,
    //                      I write it directly here to avoid using OSD_Rgb2Yuv().
    // (Y U V A)
    {
        {0x10, 0x80, 0x80, 0xff},
        {0x41, 0x64, 0xD4, 0xff},
        {0x70, 0x48, 0x3A, 0xff},
        {0xA2, 0x2C, 0x8E, 0xff},
        {0x23, 0xD4, 0x72, 0xff},
        {0x54, 0xB8, 0xC6, 0xff},
        {0x83, 0x9C, 0x2C, 0xff},
        {0xB4, 0x80, 0x80, 0xff}
    }
#endif  /* end of #if OSD_SOFTWARE_EMULATOR */
};

uint32_t const TTX_ROM OSD_coeff[OSD_MAX_COFF_NUM] =
{
    0x7d3, 0x7c3, 0x7a6, 0x78f, 0x78a, 0x7a4, 0x7e8,
    0x05b, 0x0f7, 0x1b1, 0x271, 0x31f, 0x3a2, 0x3ea
};


/* since TTX_DISP_AttrMemValues_t has only two members, we use bool_t as storage type */
typedef bool_t TTX_DISP_AttrMem_t;

/* permissible values of type TTX_DISP_AttrMem_t */
typedef enum   TTX_DISP_AttrMemValues_t
{
    TTX_DISP_ATTR_MEM_SHOWING = 0,  /* current displaying attribute memory */
    TTX_DISP_ATTR_MEM_BG            /* background and not showing attribute memory */
} TTX_DISP_AttrMemValues_t;

static bool_t hold_subpage = FALSE; // hold subpage

TTX_DISP_SubPageRotateCtrl_t ttx_subpage_auto_rotation = { FALSE, TRUE}; // auto rotate subpage, enable it as default

/* Auto-computed macros */

// OSD memory
#define TTX_DISP_ATTR_ADDRESS                 TTX_DISP_MEM_START_ADDRESS
#define TTX_DISP_ATTR_SIZE                   (TTX_DISP_MAX_ROW_LEN*OSD_DISP_ATTR_SIZE*TTX_DISP_MAX_ROW_NUM)
#define TTX_DISP_ATTR0_ADDRESS                TTX_DISP_ATTR_ADDRESS
#define TTX_DISP_ATTR1_ADDRESS               (TTX_DISP_ATTR_SIZE + TTX_DISP_ATTR0_ADDRESS)
#define TTX_DISP_ATTRx_ADDRESS(x)             TTX_DISP_ATTR##x##_ADDRESS
#define TTX_DISP_MEM_LAST_ADDRESS            (TTX_DISP_ATTR_SIZE + TTX_DISP_ATTR1_ADDRESS)
#if !OSD_SOFTWARE_EMULATOR
OSD_ASSERT_COMPILE( TTX_DISP_MEM_LAST_ADDRESS <= 0xE1FF );  // ERROR: Exceeds share memory capacity: (0xC000~0xE1FF)!
#endif  /* end of #if !OSD_SOFTWARE_EMULATOR */
#define TTX_DISP_MEM_SIZE                    (TTX_DISP_MEM_LAST_ADDRESS-TTX_DISP_MEM_START_ADDRESS)    // OSD memory in bytes


/* -- Prototypes of Private Functions ---------------------------------------*/
static OSD_TtAttr_t TTX_MEM*    TTX_DISP_GetAttrMem                 (const TTX_DISP_AttrMem_t attrMem);
static bool_t                   TTX_DISP_SetAttrHelper              (      OSD_TtAttr_t TTX_MEM* pAttrBase, const uint8_t row         , const uint8_t column, const TTX_DISP_Attr_t* pAttr);
static bool_t                   TTX_DISP_GetAttrHelper              (const OSD_TtAttr_t TTX_MEM* pAttrBase, const uint8_t row         , const uint8_t column,       TTX_DISP_Attr_t* pAttr);
static bool_t                   TTX_DISP_FillRowAttrHelper          (      OSD_TtAttr_t TTX_MEM* pAttrBase, const uint8_t row                               , const TTX_DISP_Attr_t* pAttr);
static bool_t                   TTX_DISP_FillRectAttrHelper         (      OSD_TtAttr_t TTX_MEM* pAttrBase, const uint8_t upperLeftRow, const uint8_t upperLeftCol, const uint8_t lowerRightRow, const uint8_t lowerRightCol, const TTX_DISP_Attr_t* pAttr );
static void                     TTX_DISP_DrawSubPageNavCtrlHelper   (const uint8_t row         , const TTX_DISP_SubPageNavCtrl_t* pParam);
static void                     TTX_DISP_ZeroAttrMemHelper          (      OSD_TtAttr_t TTX_MEM* pAttrBase);
static void                     TTX_DISP_CopyToAttrMemHelper        (      OSD_TtAttr_t TTX_MEM* pAttrBase, const TTX_DISP_Attr_t attr[TTX_DISP_MAX_ROW_LEN], uint8_t row);
static bool_t			TTX_DISP_CustermizedCharSet (OSD_TtAttr_t TTX_MEM* pAttrBase, const uint8_t Row , const uint8_t Col, const uint8_t modifiedCharset, const TTX_DISP_Attr_t* pAttr );


static void                     TTX_DISP_SetClut                    (void);
//static void                   TTX_DISP_Rgb2Yuv                    (uint8_t clutStartNum, uint8_t clutEndNum );
//static int32_t                TTX_DISP_SetUpAndDownLimit          (int32_t input_value, int32_t Up_Level, int32_t Down_Level);
static void                     TTX_DISP_AttrWrite                  (TTX_Address_t addr, OSD_TtAttr_t raw);

#if TTX_VERIFY  /* if enabled verficiation of TTX related IPs */
static OSD_TtAttr_t     TTX_DISP_AttrRead                   (TTX_Address_t addr);
#endif  /* end of #if TTX_VERIFY */


/* -- Implementations of Public Functions -----------------------------------*/

/*---------------------------------------------------------------------------*/
bool_t
TTX_DISP_Open    (const TTX_DISP_OpenFlag_t flags)
/*****************************************************************************\
 *  @brief   Initialize Teletext display device.
\*****************************************************************************/
{
    extern uint8_t  const TTX_ROM OSD_ttFont    [][96][16];

    uint8_t i;


#if OSD_SOFTWARE_EMULATOR
    TTX_DISP_Handle hDisp = &dispObj;

    if ( hDisp->cntUsage == 0 )
    {
#if TTX_USE_MALLOC
        hDisp->pMemBase = malloc( TTX_DISP_MEM_SIZE );
#else
        OSD_REQUIRE( _STATIC_TTX_DISP_MEM_SIZE >= TTX_DISP_MEM_SIZE );  // ERROR: _STATIC_TTX_DISP_MEM_SIZE is too small for TTX_DISP_pMemBase[]
#endif

        OSD_ENSURE( hDisp->pMemBase != NULL );
    }
    hDisp->cntUsage++;
#else
    /* VD is already been initialized in firmware. The following code is
     *   necessary only in standalone mode.
     */
    // VD_Open( VD_OPEN_RESET );
#endif  // end of #if OSD_SOFTWARE_EMULATOR


    if ( flags & TTX_DISP_OPEN_RESET )
    {
        OSD_Open( OSD_OPEN_RESET );
    }
    else
    {
        OSD_Open( OSD_OPEN_NOP );
    }

    /* Commented to remove unpleasant screen splash
        // Disable all
        OSD_SetTtDisplayEnable( FALSE );
        OSD_SetVbiEnable( FALSE );
    */

    TTX_DISP_SetClut();
    OSD_SetTtScaleCoefficient( OSD_coeff );
    OSD_SetTtR25E( TRUE );

    OSD_SetMixedMode( FALSE );

    OSD_SetTtSmooth( FALSE );       // FALSE for disable smoothing
    OSD_SetTtVsr( OSD_CTRL3_VSR_MODE0, OSD_CTRL3_TTVS_MODE0 );

    // initialize char font
    OSD_SetTtCharsetBitmapAddress( (OSD_Address_t)OSD_ttFont );
    OSD_ENSURE( OSD_GetTtCharsetBitmapAddress() == (OSD_Address_t)OSD_ttFont );

    OSD_SetTtAttr1Address( TTX_DISP_ATTRx_ADDRESS(0) );  // <Added by Janus>2008/12/04

    // initialize Diacriticalmark Bitmap
    OSD_SetTtAllDiacriticalmarkBitmap();

    OSD_SetVdelay( OSD_VDELAY );
    OSD_SetTtMosaicSeparatedMask( 0x249, 0x861 ); 	// (mlm_mask,  mbm_mask)
    OSD_SetTtR0G1Set( 24 );

    // Hdelay
    //vbiTtOsd_setPixelDelay(0xbf01b000, VBI_TTOSD_PIXELDELAY_ONE_PAGE);
    OSD_SetHdelay1( OSD_HDELAY1 );
    OSD_SetHdelay2( OSD_HDELAY2 );
    OSD_SetHdelay3( OSD_HDELAY3 );

    //Global Pixels H Delay
    rtd_maskl(VBIOSD_CTRL1_REG_0x4800, ~(_BIT20|_BIT21|_BIT22|_BIT23|_BIT24|_BIT25|_BIT26|_BIT27), (_BIT20|_BIT21|_BIT25));

    OSD_SetTtMultipageMode( FALSE );    // Currently, we support one-page only.

    OSD_SetTtMessageSeq( FALSE,0,2,7,FALSE,FALSE,TRUE,TRUE,8,22,"RTK--Page1");


    // row color
    for ( i = 0 ; i < 26 ; i++ )
    {
        // (row, clut_entry)
//		OSD_SetTtRowColorIndex(i, 0);   // Tim Huang's Original: TTX_COLOR_BLACK
        OSD_SetTtRowColorIndex(i, 0);   // <Modified Janus>2008/12/04: TTX_COLOR_WHITE
    }

    // Update Page Number of OSD
    {
        OSD_PageNum_t pageNum;

        pageNum.str[0]  = ' ';
        pageNum.str[1]  = ' ';
        pageNum.str[2]  = 'P';
        pageNum.str[3]  = '-';
        pageNum.str[4]  = '-';
        pageNum.str[5]  = '-';
        pageNum.str[6]  = ' ';
        pageNum.str[7]  = ' ';
        pageNum.csel    = TTX_CHARSET_G0_LATIN;
        pageNum.fg      = TTX_COLOR_WHITE;
        pageNum.bg      = TTX_COLOR_BLACK;
        pageNum.bEnable = TRUE;
        OSD_SetTtPageNumber( &pageNum );
    }

    OSD_SetTtTimecodeEnable( TRUE );
    OSD_SetTtLspNo( 5 );    // col no of left side pnl

    OSD_SetTtSidepanelEn( FALSE );
    OSD_SetTtScreenColor( 0 );

    OSD_SetTtBoxEnable( FALSE );
    OSD_SetTtR0Enable( TRUE );    // Commented by Janus: This may cause page number to be shifted by one pattern in FPGA PPR test patterns
//OSD_SetTtR0Enable( FALSE );    // Commented by Janus: This may cause page number to be shifted by one pattern in FPGA PPR test patterns


    /* Initially, we don't show output.
        // enable output
        OSD_SetTtDisplayEnable( TRUE );
        OSD_SetVbiEnable( TRUE );
    */

#if !DV_SIM /* don't waste time in DV_SIM */
    OSD_ApplyEntireScreenChanges( TRUE );
#endif

    TTX_Log(LOGGER_VERBOSE, "[DISP] " "TTX DISP initialized!\n");


    return( TRUE );
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_DISP_Close    (void)
/*****************************************************************************\
 *  @brief   Close Teletext display device.
\*****************************************************************************/
{
#if OSD_SOFTWARE_EMULATOR
    TTX_DISP_Handle hDisp = &dispObj;

    OSD_REQUIRE( hDisp->cntUsage > 0 );  // ERROR: Unbalanced TTX_DISP_Open() and TTX_DISP_Close()

    hDisp->cntUsage--;


#if TTX_USE_MALLOC
    if ( hDisp->cntUsage == 0 )
    {
        if ( hDisp->pMemBase != NULL )
        {
            free( hDisp->pMemBase );
            hDisp->pMemBase = NULL;
        }
    }
#endif  /* end of #if TTX_USE_MALLOC */
#else
    VD_Close();
#endif  /* end of #if OSD_SOFTWARE_EMULATOR */


    return( OSD_Close() );
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_DISP_Reset    (void)
/*****************************************************************************\
 *  @brief   Close Teletext display device.
\*****************************************************************************/
{
    TTX_DISP_Show( FALSE ); /* Display should be hide when entering READY state */
    OSD_Reset();

    TTX_DISP_ZeroAttrMemHelper( TTX_DISP_GetAttrMem( TTX_DISP_ATTR_MEM_SHOWING ) );
    TTX_DISP_ZeroAttrMemHelper( TTX_DISP_GetAttrMem( TTX_DISP_ATTR_MEM_BG      ) );

    return( TRUE );
}

#if TTX_VERIFY  /* if enabled verficiation of TTX related IPs */

/*---------------------------------------------------------------------------*/
bool_t
TTX_DISP_Test    (void)
/*****************************************************************************\
 *  @brief   Clear output display.
 *  @details Render bitmap from OSD display attributes.
\*****************************************************************************/
{
    return( OSD_Test() );
}

#endif  /* end of #if TTX_VERIFY */

#if 0
/*---------------------------------------------------------------------------*/
void
TTX_DISP_PostRefreshEntireScreen  (void)
/*****************************************************************************\
 *  @brief   Signals the entire screen needs to be updated. (non-blocking)
 *  @details Render bitmap from OSD display attributes.
\*****************************************************************************/
{
    OSD_ApplyEntireScreenChanges( FALSE );  /* FALSE for non-blocking */
}
#endif

/*---------------------------------------------------------------------------*/
void
TTX_DISP_PostRefreshRow0    (void)
/*****************************************************************************\
 *  @brief   Signals the row-0 needs to be refreshed (non-blocking).
 *  @details Render bitmap from OSD display attributes.
\*****************************************************************************/
{
    OSD_ApplyRow0Changes( FALSE );  /* FALSE for non-blocking */
}

/*---------------------------------------------------------------------------*/
void
TTX_DISP_PostRefreshPageContent    (void)
/*****************************************************************************\
 *  @brief   Signals the content of row-0 to row-23 needs to be updated (non-blocking).
 *  @details Render bitmap from OSD display attributes.
\*****************************************************************************/
{
    OSD_ApplyPageContentChanges( FALSE );  /* FALSE for non-blocking */
}

/*---------------------------------------------------------------------------*/
void
TTX_DISP_PostRefreshTopCtrl    (void)
/*****************************************************************************\
 *  @brief   signals the TOP control bar (row-24) needs to be refreshed (non-blocking).
 *  @details Render bitmap from OSD display attributes.
\*****************************************************************************/
{
    OSD_ApplyTopCtrlChanges( FALSE );   /* FALSE for non-blocking */
}

/*---------------------------------------------------------------------------*/
void
TTX_DISP_PostRefreshSubPageNavCtrl    (void)
/*****************************************************************************\
 *  @brief   signals the sub-page navigation control (row-25) needs to be refreshed (non-blocking).
 *  @details Render bitmap from OSD display attributes.
\*****************************************************************************/
{
    OSD_ApplySubPageNavCtrlChanges( FALSE );  /* FALSE for non-blocking */
}


#if 0
/*---------------------------------------------------------------------------*/
void
TTX_DISP_DoRefresh    (void)
/*****************************************************************************\
 *  @brief   Actually do refreshing screen if someone did TTX_DISP_PostRefresh().
 *             (blocking)
\*****************************************************************************/
{
    OSD_onVerticalSync();
}
#endif

/*---------------------------------------------------------------------------*/
void
TTX_DISP_onScreenCanBeRefreshed    (void)
/*****************************************************************************\
 *  @brief   event handler of signaling display to TTX_DISP_DoRefresh().
 *  @note    Will be triggered when display is able to be refreshed. That is,
 *             display attributes are ready for displaying.
 *  @note    Will be triggered each TTX_Tick().
\*****************************************************************************/
{
    OSD_onScreenCanBeRefreshed();
}

/*---------------------------------------------------------------------------*/
void
TTX_DISP_EnableTimecode             (const bool_t bEnable)
/*****************************************************************************\
 *  @brief   Clear output display.
 *  @details Render bitmap from OSD display attributes.
\*****************************************************************************/
{
    OSD_SetTtTimecodeEnable( bEnable );
}

/*---------------------------------------------------------------------------*/
void
TTX_DISP_EnableRolling             (const bool_t bEnable)
/*****************************************************************************\
 *  @brief   Clear output display.
 *  @note    Mimic vbiTtOsd_setTtRolling().
 *  @details Render bitmap from OSD display attributes.
\*****************************************************************************/
{
    OSD_SetTtRollingEnable( bEnable );
    OSD_ApplyRow0Changes( FALSE );
}

/*---------------------------------------------------------------------------*/
void
TTX_DISP_EnableBox      (const bool_t bEnable)
/*****************************************************************************\
 *  @brief   Enable box area or window area of the specified page in teletext.
 *  @note    Mimic VbiOsd_SetTtBoxEnable().
\*****************************************************************************/
{
    OSD_SetTtBoxEnable( bEnable );
}

/*---------------------------------------------------------------------------*/
#if 0 // not called by any
void
TTX_DISP_ZeroAttrMem               (void)
/*****************************************************************************\
 *  @brief   Dump attr[] to n-th row of addrBaseOsdAttrMem.
 *  @details attr[0] ~ attr[TTX_DISP_MAX_ROW_LEN-1]
\*****************************************************************************/
{
    // Note: Calling TTX_DISP_GetAttrMem() must be after OSD_Open().
    //         Because TTX_DISP_GetAttrMem calls OSD_GetTtAttr1Address().
    TTX_DISP_ZeroAttrMemHelper( TTX_DISP_GetAttrMem( TTX_DISP_ATTR_MEM_BG ) );
}
#endif

/*---------------------------------------------------------------------------*/
void
TTX_DISP_CopyToAttrMem    (const TTX_DISP_Attr_t attr[TTX_DISP_MAX_ROW_LEN],
                           uint8_t row)
/*****************************************************************************\
 *  @brief   Copy attr[] to n-th row of addrBaseOsdAttrMem.
 *  @details attr[0] ~ attr[TTX_DISP_MAX_ROW_LEN-1]
\*****************************************************************************/
{
    // Note: Calling TTX_DISP_GetAttrMem() must be after OSD_Open().
    //         Because TTX_DISP_GetAttrMem calls OSD_GetTtAttr1Address().
    OSD_TtAttr_t TTX_MEM* pAttrBase = TTX_DISP_GetAttrMem( TTX_DISP_ATTR_MEM_BG );

    OSD_REQUIRE( row < TTX_DISP_MAX_ROW_NUM );
    if ( (pAttrBase != NULL)&&(attr != NULL) )
    {
        TTX_DISP_CopyToAttrMemHelper( pAttrBase, attr, row );
    }
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_DISP_SetAttr       (const uint8_t row, const uint8_t column,
                        const TTX_DISP_Attr_t* pAttr)
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
    // Note: Calling TTX_DISP_GetAttrMem() must be after OSD_Open().
    //         Because TTX_DISP_GetAttrMem calls OSD_GetTtAttr1Address().
    return( TTX_DISP_SetAttrHelper( TTX_DISP_GetAttrMem( TTX_DISP_ATTR_MEM_BG ),
                                    row, column, pAttr ) );
}

#if TTX_VERIFY  /* if enabled verficiation of TTX related IPs */

/*---------------------------------------------------------------------------*/
bool_t
TTX_DISP_GetAttr   (const uint8_t row, const uint8_t col,
                    TTX_DISP_Attr_t* pAttr)
/*****************************************************************************\
 *  @brief   Get the attributes of current display.
 *  @param   row    the row you want to get.    0~TTX_DISP_MAX_ROW_NUM.
 *  @param   col    the column you want to get. 0~TTX_DISP_MAX_ROW_LEN.
\*****************************************************************************/
{
    // Note: Calling TTX_DISP_GetAttrMem() must be after OSD_Open().
    //         Because TTX_DISP_GetAttrMem calls OSD_GetTtAttr1Address().
    return( TTX_DISP_GetAttrHelper( TTX_DISP_GetAttrMem( TTX_DISP_ATTR_MEM_BG ),
                                    row, col, pAttr ) );
}

#endif  /* end of #if TTX_VERIFY */

/*---------------------------------------------------------------------------*/
bool_t
TTX_DISP_FillRowAttr  (const uint8_t row,
                       const TTX_DISP_Attr_t* pAttr)
/*****************************************************************************\
 *  @brief   Fill a rectangle from (upperLeftRow, upperLeftCol) to (lowerLeftRow,
 *             lowerLeftCol) with the same attribute *pAttr.
 *  @param   *pAttr is the attribute you want to use as fill pattern.
 *  @pre     0 <= row  < TTX_DISP_MAX_ROW_NUM
\*****************************************************************************/
{
    // Note: Calling TTX_DISP_GetAttrMem() must be after OSD_Open().
    //         Because TTX_DISP_GetAttrMem calls OSD_GetTtAttr1Address().
    return( TTX_DISP_FillRowAttrHelper( TTX_DISP_GetAttrMem( TTX_DISP_ATTR_MEM_BG ),
                                        row, pAttr ) );
}
#if 0
/*---------------------------------------------------------------------------*/
bool_t
TTX_DISP_FillRectAttr  (const uint8_t upperLeftRow , const uint8_t upperLeftCol,
                        const uint8_t lowerRightRow, const uint8_t lowerRightCol,
                        const TTX_DISP_Attr_t* pAttr )
/*****************************************************************************\
 *  @brief   Fill a rectangle from (upperLeftRow, upperLeftCol) to (lowerLeftRow,
 *             lowerLeftCol) with the same attribute *pAttr.
 *  @param   *pAttr is the attribute you want to use as fill pattern.
 *  @pre     0 <= upperLeftRow  < TTX_DISP_MAX_ROW_NUM
 *  @pre     0 <= upperLeftCol  < TTX_DISP_MAX_ROW_LEN
 *  @pre     0 <= lowerRightRow < TTX_DISP_MAX_ROW_NUM
 *  @pre     0 <= lowerRightCol < TTX_DISP_MAX_ROW_LEN
 *  @pre     upperLeftRow <= lowerRightRow
 *  @pre     upperLeftCol <= lowerRightCol
\*****************************************************************************/
{
    // Note: Calling TTX_DISP_GetAttrMem() must be after OSD_Open().
    //         Because TTX_DISP_GetAttrMem calls OSD_GetTtAttr1Address().
    return( TTX_DISP_FillRectAttrHelper( TTX_DISP_GetAttrMem( TTX_DISP_ATTR_MEM_BG ),
                                         upperLeftRow, upperLeftCol,
                                         lowerRightRow, lowerRightCol, pAttr ) );
}
#endif
/*---------------------------------------------------------------------------*/
void
TTX_DISP_SwitchAttrMem              (void)
/*****************************************************************************\
 *  @brief   Toggle showing attribute memory between TTX_DISP_AttrMemValues_t.
 *  @details Implements ping-pong buffer.
 *  @note    Mimic Tim's vbiTtOsd_switchDisplayNondisplayPage().
\*****************************************************************************/
{
    /* If current displaying attr-0 */
    if ( OSD_GetTtAttr1Address() == TTX_DISP_ATTRx_ADDRESS(0) )
    {
        OSD_SetTtAttr1Address( TTX_DISP_ATTRx_ADDRESS(1) );  // Set current displaying attributes memory to attr-1
    }
    else
    {
        OSD_SetTtAttr1Address( TTX_DISP_ATTRx_ADDRESS(0) );  // Set current displaying attributes memory to attr-0
    }

    OSD_ApplyEntireScreenChanges( TRUE );   /* wait for above changes to be applied */
}

/*---------------------------------------------------------------------------*/
void
TTX_DISP_ClrNavCtrl()
/*****************************************************************************\
 *  @brief   Reset the "sub-page control".
 *  @note    Mimic vbiSys_ShowSubPage().
 *  @param   [in]  attrMem is the attribute memory to be drawn.
 *  @param   [in]  pParam is the drawing parameter that contains the sub-page
 *                   info to be drawn. If you leave (pParam == NULL), this
 *                   __FUNCTION__ will erase existing sub-page nav control.
\*****************************************************************************/
{
    const uint8_t row = TTX_DISP_SUBPAGE_NAV_CTRL_ROW_NUM;

    TTX_DISP_Attr_t attr;

    memset( &attr, 0, sizeof( attr ) );

    attr.conceal = TRUE;

    TTX_DISP_FillRowAttr( row, &attr );

}


/*---------------------------------------------------------------------------*/
void
TTX_DISP_DrawSubPageNavCtrl  (const TTX_DISP_SubPageNavCtrl_t* pParam)
/*****************************************************************************\
 *  @brief   Renders the "sub-page control".
 *  @note    Mimic vbiSys_ShowSubPage().
 *  @param   [in]  attrMem is the attribute memory to be drawn.
 *  @param   [in]  pParam is the drawing parameter that contains the sub-page
 *                   info to be drawn. If you leave (pParam == NULL), this
 *                   __FUNCTION__ will erase existing sub-page nav control.
\*****************************************************************************/
{
    const uint8_t row = TTX_DISP_SUBPAGE_NAV_CTRL_ROW_NUM;  /* render sub-page control at the last row */

    // Note: Calling TTX_DISP_GetAttrMem() must be after OSD_Open().
    //         Because TTX_DISP_GetAttrMem calls OSD_GetTtAttr1Address().
    TTX_DISP_DrawSubPageNavCtrlHelper(row, pParam);
}

/*---------------------------------------------------------------------------*/
void
TTX_DISP_Show         (const bool_t bShow)
/*****************************************************************************\
 *  @brief   Show or Hide entire display.
\*****************************************************************************/
{
    OSD_SetTtDisplayEnable( bShow );
    OSD_SetVbiEnable( bShow );
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_DISP_isShowing                  (void)
/*****************************************************************************\
 *  @brief   Show or Hide entire display.
\*****************************************************************************/
{
    return( OSD_isDisplaying() );
}
#if 0
/*---------------------------------------------------------------------------*/
void
TTX_DISP_ShowRow0         (const bool_t bShow)
/*****************************************************************************\
 *  @brief   Show or Hide Row-0.
\*****************************************************************************/
{
    OSD_SetTtR0Enable( bShow );
}
#endif
/*---------------------------------------------------------------------------*/
void TTX_DISP_SetPageNum (
    const TTX_STD_Page_AddrWithSubCode_t* pPageAddr,
    const TTX_DISP_PageNumAttr_t PageNumAttr )
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
    // show "hold" icon
    if ( hold_subpage )
    {
        TTX_DISP_SubpageHold();
        return;
    }


    if ( pPageAddr != NULL )
    {
        OSD_PageNum_t pageNum;
        uint8_t	mag = pPageAddr->pageA >>8;
        uint8_t	page_ten = ((pPageAddr->pageA&0x00F0)>>4) ;
        uint8_t	page_unit = pPageAddr->pageA & 0x000F;

        if ( TTX_STD_Page_AddrIsValid( pPageAddr ) )
        {
            //pageNum.bg = TTX_COLOR_BLUE;
            pageNum.bg = PageNumAttr.ValidBackgroundColor;
        }
        else
        {
            //pageNum.bg = TTX_COLOR_RED;
            pageNum.bg = PageNumAttr.InvalidBackgroundColor;
        }

        pageNum.str[0] = ' ';
        pageNum.str[1] = ' ';
        if ( PageNumAttr.bEnableP )
            pageNum.str[2] = 'P';
        else
            pageNum.str[2] = ' ';
        if ( PageNumAttr.bEnableMag )
        {
            pageNum.str[3] = '0' + mag;
        }
        else
        {
            pageNum.str[3] = mag;
        }
        if ( PageNumAttr.bEnablePageTens )
        {
            if ( page_ten >= 10 )
                pageNum.str[4] = 'A' + (page_ten - 10);
            else
                pageNum.str[4] = '0' + page_ten;
        }
        else
        {
            pageNum.str[4] = '-';
        }
        if ( PageNumAttr.bEnablePageUnits )
        {
            if ( page_unit >= 10 )
                pageNum.str[5] = 'A' + (page_unit - 10);
            else
                pageNum.str[5] = '0' + page_unit;
        }
        else
        {
            pageNum.str[5] = '-';
        }
        pageNum.str[6] = ' ';
        pageNum.str[7] = ' ';
        pageNum.csel = TTX_CHARSET_G0_LATIN;
        //pageNum.fg = TTX_COLOR_YELLOW;
        pageNum.fg = PageNumAttr.ForegroundColor;
        pageNum.bEnable = TRUE;

        OSD_SetTtPageNumber( &pageNum );
    }
}

/*---------------------------------------------------------------------------*/
void TTX_DISP_SubpageHold (void)
/*****************************************************************************\
 * @brief		set hold icon on the page number
\*****************************************************************************/
{
    OSD_PageNum_t pageNum;

#if 1
    pageNum.str[0] = ' ';
    pageNum.str[1] = ' ';
    pageNum.str[2] = 'H';
    pageNum.str[3] = 'O';
    pageNum.str[4] = 'L';
    pageNum.str[5] = 'D';
    pageNum.str[6] = ' ';
    pageNum.str[7] = ' ';
    pageNum.csel = TTX_CHARSET_G0_LATIN;
    pageNum.fg = TTX_COLOR_GREEN;
    pageNum.bg = TTX_COLOR_BLACK;
    pageNum.bEnable = TRUE;
#else
    pageNum.str[0] = ' ';
    pageNum.str[1] = ' ';
    pageNum.str[2] = ' ';
    pageNum.str[3] = 0x7F; // solid block
    pageNum.str[4] = ' ';
    pageNum.str[5] = ' ';
    pageNum.str[6] = ' ';
    pageNum.str[7] = ' ';
    pageNum.csel = TTX_CHARSET_G0_LATIN;
    pageNum.fg = TTX_COLOR_YELLOW;
    pageNum.bg = TTX_COLOR_BLUE;
    pageNum.bEnable = TRUE;
#endif

    OSD_SetTtPageNumber( &pageNum );

    hold_subpage = TRUE;
}

/*---------------------------------------------------------------------------*/
void TTX_DISP_SubpageRotate (void)
/*****************************************************************************\
 *  @brief	set hold icon on the page number
 *  @details	set hold icon on the page number
\*****************************************************************************/
{
    hold_subpage = FALSE;
}


/*---------------------------------------------------------------------------*/
bool_t
TTX_DISP_AttrToOsdAttr  (const TTX_DISP_Attr_t* pAttr, OSD_TtAttr_t* pRaw)
/*****************************************************************************\
 *  @brief   Converts "human" readable format (TTX_DISP_TtAttr_t) to machine
 *             readable attribute data (OSD_TtAttr_t) stored in OSD memory .
 *  @param   *pRaw  is the machine readable attribute data.
 *  @param   *pAttr will be the converted human readable attribute data.
 *
 *  @return  the conversion status.
 *  @retval  TTX_TRUE  success
 *  @retval  TTX_FALSE failure
 *
 *  @see     TTX_DISP_OsdAttrToAttr()
\*****************************************************************************/
{
    uint8_t tmp0 = 0, tmp1 = 0, tmp2 = 0, tmp3 = 0;
    uint8_t *p_raw = NULL;
    uint32_t raw = 0;

    if ( pAttr != NULL )
    {
        uint8_t             modifiedCharSet;
        TTX_STD_CharIndex_t modifiedCharIndex;

        // Transforms the char selection of display attributes to char set offset of fonts.
        TTX_DISP_TtAttrCselToFontOffset( pAttr->dmIdx, pAttr->charset, pAttr->charIdx, &modifiedCharSet, &modifiedCharIndex );
#if 0
        raw =  (uint32_t)(pAttr->conceal    & 0x01) << 31 \
               |(uint32_t)(pAttr->box        & 0x01) << 30 \
               |(uint32_t)(pAttr->graphic    & 0x01) << 29 \
               |(uint32_t)(pAttr->size       & 0x0F) << 24 \
               |(uint32_t)(modifiedCharSet   & 0x04) << (23-2) \
               |(uint32_t)(pAttr->bUnderline & 0x01) << 22 \
               |(uint32_t)(pAttr->mosaics    & 0x01) << 21 \
               |(uint32_t)(pAttr->dmIdx      & 0x1F) << 16 \
               |(uint32_t)(pAttr->flashMode  & 0x01) << 15 \
               |(uint32_t)(pAttr->fgColorIdx & 0x07) << 12 \
               |(uint32_t)(pAttr->bgColorIdx & 0x07) <<  9 \
               |(uint32_t)(modifiedCharSet   & 0x03) <<  7 \
               |(uint32_t)(modifiedCharIndex & 0x7F);
#else
        p_raw = (UINT8*) &raw;
        if (pAttr->conceal)
            tmp0 |= _BIT7;
        if (pAttr->box)
            tmp0 |= _BIT6;
        if (pAttr->graphic)
            tmp0 |= _BIT5;
        tmp0 |= (pAttr->size & 0x0F);
        *p_raw = tmp0;

        if (modifiedCharSet & 0x04)
            tmp1|= _BIT7;
        if (pAttr->bUnderline)
            tmp1 |= _BIT6;
        if (pAttr->mosaics)
            tmp1 |= _BIT5;
        tmp1 |= (pAttr->dmIdx & 0x1F);
        *(p_raw+1) = tmp1;

        if (pAttr->flashMode)
            tmp2 |= _BIT7;
        if (pAttr->fgColorIdx & 0x04)
            tmp2 |= _BIT6;
        if (pAttr->fgColorIdx & 0x02)
            tmp2 |= _BIT5;
        if (pAttr->fgColorIdx & 0x01)
            tmp2 |= _BIT4;
        if (pAttr->bgColorIdx & 0x04)
            tmp2 |= _BIT3;
        if (pAttr->bgColorIdx & 0x02)
            tmp2 |= _BIT2;
        if (pAttr->bgColorIdx & 0x01)
            tmp2 |= _BIT1;
        if (modifiedCharSet & 0x02)
            tmp2 |= _BIT0;
        *(p_raw+2) = tmp2;

        if (modifiedCharSet & 0x01)
            tmp3 |= _BIT7;
        tmp3 |= (modifiedCharIndex & 0x7F);
        *(p_raw+3) = tmp3;

#endif
        //RTD_Log(LOGGER_INFO, "TTX_DISP_AttrToOsdAttr : (raw : %x) (%x, %x, %x, %x) \n", (uint32_t) raw, (uint32_t) tmp0, (uint32_t) tmp1, (uint32_t) tmp2, (uint32_t) tmp3);

        if ( pRaw != NULL )
        {
            *pRaw = raw;

            return( TRUE );
        }
    }

    return( FALSE );
}

#if TTX_VERIFY  /* if enabled verficiation of TTX related IPs */
/*---------------------------------------------------------------------------*/
bool_t
TTX_DISP_OsdAttrToAttr  (const volatile OSD_TtAttr_t* pRaw, TTX_DISP_Attr_t* pAttr)
/*****************************************************************************\
 *  @brief   Converts machine readable attribute data (OSD_TtAttr_t) stored in
 *             OSD memory to "human" readable format (TTX_DISP_TtAttr_t).
 *  @param   *pRaw  is the machine readable attribute data.
 *  @param   *pAttr will be the converted human readable attribute data.
 *
 *  @return  the conversion status.
 *  @retval  TTX_TRUE  success
 *  @retval  TTX_FALSE failure
 *
 *  @see     TTX_DISP_OsdAttrToAttr()
\*****************************************************************************/
{

    if ( (pRaw != NULL)&&(pAttr != NULL) )
    {
        TTX_STD_CharSet_t     cset;
        TTX_STD_CharIndex_t   cindex;

        TTX_DISP_FontOffsetToTtAttrCsel(((*pRaw >>  7) & 0x03)|((*pRaw>>(23-2))&0x04), (*pRaw & 0x7F), &cset, &cindex );

        pAttr->conceal    = (*pRaw >> 31) & 0x01;
        pAttr->box        = (*pRaw >> 30) & 0x01;
        pAttr->graphic    = (*pRaw >> 29) & 0x01;
        pAttr->size       = (*pRaw >> 24) & 0x0F;
        pAttr->bUnderline = (*pRaw >> 22) & 0x01;
        pAttr->mosaics    = (*pRaw >> 21) & 0x01;
        pAttr->dmIdx      = (*pRaw >> 16) & 0x1F;
        pAttr->flashMode  = (*pRaw >> 15) & 0x01;
        pAttr->fgColorIdx = (*pRaw >> 12) & 0x07;
        pAttr->bgColorIdx = (*pRaw >>  9) & 0x07;
        pAttr->charset    = cset;
        pAttr->charIdx    = cindex;


        return( TRUE );
    }

    return( FALSE );
}

#endif







/* -- Implementations of Private Functions ----------------------------------*/



/*---------------------------------------------------------------------------*/
void TTX_DISP_TtAttrCselToFontOffset
(TTX_STD_DiacMark_t dm_index, TTX_STD_CharSet_t cset, TTX_STD_CharIndex_t cindex,
 uint8_t* pModifiedCharSet, TTX_STD_CharIndex_t* pModifiedCharIndex)
/*****************************************************************************\
 *  @brief   Convert teletext standard character set to offset of ttFonts[].
 *  @return  the starting offset of the selected font.
\*****************************************************************************/
{
#define CHAR_SET_NUM    (30)
    uint8_t             modifiedCharSet;
    TTX_STD_CharIndex_t modifiedCharIndex;
    uint8_t col, row;

    // This lookup table maps the character set of standard Teletext to offset of OSD_ttFont[].
    //   '0' means the first character set of OSD_ttFont[];
    //   '1' means the second character set of OSD_ttFont[].
    const uint8_t CHAR_SETS[CHAR_SET_NUM] =
    {
        0,  // TTX_CHARSET_G0_LATIN
        4,  // TTX_CHARSET_G0_LATIN_CZECH_SLOVAK
        4,  // TTX_CHARSET_G0_LATIN_ENGLISH
        4,  // TTX_CHARSET_G0_LATIN_ESTONIAN
        4,  // TTX_CHARSET_G0_LATIN_FRENCH
        4,  // TTX_CHARSET_G0_LATIN_GERMAN
        4,  // TTX_CHARSET_G0_LATIN_ITALIAN
        5,  // TTX_CHARSET_G0_LATIN_LETTISH_LITHUANIAN
        5,  // TTX_CHARSET_G0_LATIN_POLISH
        5,  // TTX_CHARSET_G0_LATIN_PORTUGUESE_SPANISH
        5,  // TTX_CHARSET_G0_LATIN_RUMANIAN
        5,  // TTX_CHARSET_G0_LATIN_SERBIAN_CROATIAN_SLOVENIAN
        5,  // TTX_CHARSET_G0_LATIN_SWEDISH_FINNISH
        6,  // TTX_CHARSET_G0_LATIN_TURKISH
        1,  // TTX_CHARSET_G2_LATIN
        0,  // TTX_CHARSET_G0_CYRILLIC_OP1
        0,  // TTX_CHARSET_G0_CYRILLIC_OP2
        0,  // TTX_CHARSET_G0_CYRILLIC_OP3
        1,  // TTX_CHARSET_G2_CYRILLIC
        0,  // TTX_CHARSET_G0_GREEK
        1,  // TTX_CHARSET_G2_GREEK
        0,  // TTX_CHARSET_G0_ARABIC
        1,  // TTX_CHARSET_G2_ARABIC
        0,  // TTX_CHARSET_G0_HEBREW
        2,  // TTX_CHARSET_G1_MOSAICS
        3,  // TTX_CHARSET_G3_MOSAICS
#if TTX_CFG_PRESENT_LEVEL >= TTX_STD_PRESENT_LEVEL(25)
        0,  // TTX_CHARSET_GDRCS
        0,  // TTX_CHARSET_DRCS
        0,  // TTX_CHARSET_GDRCS2
        0   // TTX_CHARSET_DRCS2
#endif  // end of #if TTX_CFG_PRESENT_LEVEL >= TTX_STD_PRESENT_LEVEL(25)
    };

    OSD_REQUIRE( TTX_CHARSET_G0_LATIN                            ==  0 );
    OSD_REQUIRE( TTX_CHARSET_G0_LATIN_CZECH_SLOVAK               ==  1 );
    OSD_REQUIRE( TTX_CHARSET_G0_LATIN_ENGLISH                    ==  2 );
    OSD_REQUIRE( TTX_CHARSET_G0_LATIN_ESTONIAN                   ==  3 );
    OSD_REQUIRE( TTX_CHARSET_G0_LATIN_FRENCH                     ==  4 );
    OSD_REQUIRE( TTX_CHARSET_G0_LATIN_GERMAN                     ==  5 );
    OSD_REQUIRE( TTX_CHARSET_G0_LATIN_ITALIAN                    ==  6 );
    OSD_REQUIRE( TTX_CHARSET_G0_LATIN_LETTISH_LITHUANIAN         ==  7 );
    OSD_REQUIRE( TTX_CHARSET_G0_LATIN_POLISH                     ==  8 );
    OSD_REQUIRE( TTX_CHARSET_G0_LATIN_PORTUGUESE_SPANISH         ==  9 );
    OSD_REQUIRE( TTX_CHARSET_G0_LATIN_RUMANIAN                   == 10 );
    OSD_REQUIRE( TTX_CHARSET_G0_LATIN_SERBIAN_CROATIAN_SLOVENIAN == 11 );
    OSD_REQUIRE( TTX_CHARSET_G0_LATIN_SWEDISH_FINNISH            == 12 );
    OSD_REQUIRE( TTX_CHARSET_G0_LATIN_TURKISH                    == 13 );
    OSD_REQUIRE( TTX_CHARSET_G2_LATIN                            == 14 );
    OSD_REQUIRE( TTX_CHARSET_G0_CYRILLIC_OP1                     == 15 );
    OSD_REQUIRE( TTX_CHARSET_G0_CYRILLIC_OP2                     == 16 );
    OSD_REQUIRE( TTX_CHARSET_G0_CYRILLIC_OP3                     == 17 );
    OSD_REQUIRE( TTX_CHARSET_G2_CYRILLIC                         == 18 );
    OSD_REQUIRE( TTX_CHARSET_G0_GREEK                            == 19 );
    OSD_REQUIRE( TTX_CHARSET_G2_GREEK                            == 20 );
    OSD_REQUIRE( TTX_CHARSET_G0_ARABIC                           == 21 );
    OSD_REQUIRE( TTX_CHARSET_G2_ARABIC                           == 22 );
    OSD_REQUIRE( TTX_CHARSET_G0_HEBREW                           == 23 );
    OSD_REQUIRE( TTX_CHARSET_G1_MOSAICS                          == 24 );
    OSD_REQUIRE( TTX_CHARSET_G3_MOSAICS                          == 25 );
    OSD_REQUIRE( TTX_CHARSET_GDRCS                               == 26 );
    OSD_REQUIRE( TTX_CHARSET_DRCS                                == 27 );
    OSD_REQUIRE( TTX_CHARSET_GDRCS2                              == 28 );
    OSD_REQUIRE( TTX_CHARSET_DRCS2                               == 29 );

    if ( cset < CHAR_SET_NUM )
    {
        modifiedCharSet = CHAR_SETS[cset];
    }
    else
    {
        modifiedCharSet = 0;    // the first character set of OSD_ttFont[]
    }

    modifiedCharIndex = cindex;

    // use small character for diacritical mark
    if ( (dm_index>=0x11) && (dm_index<=0x1F) ) // with diacritical mark
    {
        if ( (dm_index!=0x19) && (dm_index!=0x1B) && (dm_index!=0x1C) && (dm_index!=0x1E) ) // ignore 4 bottom diacritical mark
        {
            if ( (cset>=TTX_CHARSET_G0_LATIN) && (cset<=TTX_CHARSET_G0_LATIN_TURKISH) ) // Latin G0
            {
                if ( (cindex>=0x30) && (cindex<=0x39) ) // 0 ~ 9
                {
                    modifiedCharSet = 6;
                    goto set_CharSet_CharIdx;
                }
                else if ( (cindex>=0x41) && (cindex<=0x5A) ) // A ~ Z
                {
                    modifiedCharSet = 6;
                    goto set_CharSet_CharIdx;
                }
                else if ( (cindex==0x66) || (cindex==0x69) || (cindex==0x6A) || (cindex==0x6C) || (cindex==0x74) ) // f, i, j, l, t
                {
                    modifiedCharSet = 6;
                    switch (cindex)
                    {
                    case 0x66: // f
                        modifiedCharIndex = 0x5B;
                        break;
                    case 0x69: // i
                        modifiedCharIndex = 0x5C;
                        break;
                    case 0x6A: // j
                        modifiedCharIndex = 0x5D;
                        break;
                    case 0x6C: // l
                        modifiedCharIndex = 0x5E;
                        break;
                    case 0x74: // t
                        modifiedCharIndex = 0x5F;
                        break;
                    }
                    goto set_CharSet_CharIdx;
                }
            }
        }
    }

    // The fifth, sixth and seventh character set of OSD_ttFont[] are specially allocated
    //   to save font ROM space for Latin national option sub-sets.
    if ( modifiedCharSet & 0x04 )   // The faster version of if( 4 <= modifiedCharSet <= 6 )
    {
        // This is the lookup table for character sets: A, B and C only.
        //   It looks up the column number of each national option character index.
        col = (cset - 1)%6 + 2; // Plus two because the first column is 2

        // This is the lookup table for character sets: A, B and C only.
        //   It looks up the column number of each national option character index.
        switch ( cindex )
        {
        case 0x23:
            row =  0;
            break;
        case 0x24:
            row =  1;
            break;
        case 0x40:
            row =  2;
            break;
        case 0x5B:
            row =  3;
            break;
        case 0x5C:
            row =  4;
            break;
        case 0x5D:
            row =  5;
            break;
        case 0x5E:
            row =  6;
            break;
        case 0x5F:
            row =  7;
            break;
        case 0x60:
            row =  8;
            break;
        case 0x7B:
            row =  9;
            break;
        case 0x7C:
            row = 10;
            break;
        case 0x7D:
            row = 11;
            break;
        case 0x7E:
            row = 12;
            break;
        default:
            row = 13;   // a dummy number that doesn't equals any of above
            break;
        }
        if ( row != 13 ) // hit!
        {
            modifiedCharIndex = (col << 4) | row;
        }
        else
        {
            // Since originial cindex is not an Latin National Option Sub-sets,
            //   use the original Latin G0 Primary Set (the first char set of OSD_ttFont[].
            modifiedCharSet = 0;
        }
    }

set_CharSet_CharIdx:

    if ( pModifiedCharSet != NULL )
    {
        *pModifiedCharSet = modifiedCharSet;
    }

    if ( pModifiedCharIndex != NULL )
    {
        *pModifiedCharIndex = modifiedCharIndex;
    }
}

#if TTX_VERIFY  /* if enabled verficiation of TTX related IPs */
/*---------------------------------------------------------------------------*/
void
TTX_DISP_FontOffsetToTtAttrCsel  (uint8_t modifiedCharSet, TTX_STD_CharIndex_t modifiedCharIndex, TTX_STD_CharSet_t* pCset, TTX_STD_CharIndex_t* pCindex)
/*****************************************************************************\
 *  @brief   Convert offset of ttFonts[] to teletext standard character set.
 *  @return  the starting offset of the selected font.
\*****************************************************************************/
{
    TTX_STD_CharSet_t   cset   = TTX_CHARSET_G0_LATIN;
    TTX_STD_CharIndex_t cindex = 0x20;

    OSD_REQUIRE( TTX_CHARSET_G0_LATIN_CZECH_SLOVAK               ==  1 );
    OSD_REQUIRE( TTX_CHARSET_G0_LATIN_ENGLISH                    ==  2 );
    OSD_REQUIRE( TTX_CHARSET_G0_LATIN_ESTONIAN                   ==  3 );
    OSD_REQUIRE( TTX_CHARSET_G0_LATIN_FRENCH                     ==  4 );
    OSD_REQUIRE( TTX_CHARSET_G0_LATIN_GERMAN                     ==  5 );
    OSD_REQUIRE( TTX_CHARSET_G0_LATIN_ITALIAN                    ==  6 );
    OSD_REQUIRE( TTX_CHARSET_G0_LATIN_LETTISH_LITHUANIAN         ==  7 );
    OSD_REQUIRE( TTX_CHARSET_G0_LATIN_POLISH                     ==  8 );
    OSD_REQUIRE( TTX_CHARSET_G0_LATIN_PORTUGUESE_SPANISH         ==  9 );
    OSD_REQUIRE( TTX_CHARSET_G0_LATIN_RUMANIAN                   == 10 );
    OSD_REQUIRE( TTX_CHARSET_G0_LATIN_SERBIAN_CROATIAN_SLOVENIAN == 11 );
    OSD_REQUIRE( TTX_CHARSET_G0_LATIN_SWEDISH_FINNISH            == 12 );
    OSD_REQUIRE( TTX_CHARSET_G0_LATIN_TURKISH                    == 13 );

    if ( modifiedCharSet < OSD_FONT_TOTAL_CHARSETS )
    {
        // The fifth, sixth and seventh character set of OSD_ttFont[] are specially allocated
        //   to save font ROM space for Latin national option sub-sets.
        if ( modifiedCharSet & 0x04 )   // The faster version of if( 4 <= modifiedCharSet <= 6 )
        {
            const uint8_t row = modifiedCharIndex & 0x0F;
            const uint8_t col = (modifiedCharIndex >> 4) & 0x0F;

            OSD_REQUIRE( col >= 0x02 );
            OSD_REQUIRE( row <= 0x0C );
            if ( row <= 0x0C )
            {
                const uint8_t ROW_TO_CINDEX_LOOKUP_TABLE[] =
                {
                    0x23,
                    0x24,
                    0x40,
                    0x5B,
                    0x5C,
                    0x5D,
                    0x5E,
                    0x5F,
                    0x60,
                    0x7B,
                    0x7C,
                    0x7D,
                    0x7E
                };

                cset = (modifiedCharSet - 4)*6 + TTX_CHARSET_G0_LATIN_CZECH_SLOVAK + (col - 2);
                cindex = ROW_TO_CINDEX_LOOKUP_TABLE[row];
            }
        }
        else
        {
            const TTX_STD_CharSet_t FONTS_IN_TT_FONT[] =
            {
                TTX_CHARSET_G0_LATIN,
                TTX_CHARSET_G2_LATIN,
                TTX_CHARSET_G1_MOSAICS,
                TTX_CHARSET_G3_MOSAICS
            };

            cset   = FONTS_IN_TT_FONT[ modifiedCharSet ];
            cindex = modifiedCharIndex;
        }

        if ( pCset != NULL )
        {
            *pCset = cset;
        }

        if ( pCindex != NULL )
        {
            *pCindex = cindex;
        }
    }
}

#endif

/*---------------------------------------------------------------------------*/
static void
TTX_DISP_SetClut (void)
/*****************************************************************************\
 *  @brief   Set the content of all color lookup tables to the one of OSD_clutValue.
\*****************************************************************************/
{
    OSD_SetClutAll( OSD_clutValue );
}

#if 0   /* un-used function */

/*---------------------------------------------------------------------------*/
static int32_t
TTX_DISP_SetUpAndDownLimit   (int32_t input_value, int32_t Up_Level, int32_t Down_Level)
/*****************************************************************************\
 *  @brief  Bound a value to an upper bound and a lower bound.
 *  @param  input_value	: [input] the value to be bounded
 *  @param  Up_Level	: [input] the upped bound
 *  @param  Down_Level	: [input] the lower bound
\*****************************************************************************/
{
    if (input_value > Up_Level)
    {
        return Up_Level;
    }
    else if (input_value < Down_Level)
    {
        return Down_Level;
    }
    return input_value;
}

/*---------------------------------------------------------------------------*/
static void
TTX_DISP_Rgb2Yuv (uint8_t clutStartNum, uint8_t clutEndNum)
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in OSD_clutValue[][]
 *             from RGB to YUV
 *  @post    OSD_clutValue[][] will be updated.
 *  @todo    Why this function cannot obtain the following nominal results?
 *          // {Y,U,V,A}
 *			{0x10, 0x80, 0x80, 0xff},
 *			{0x41, 0x64, 0xD4, 0xff},
 *			{0x70, 0x48, 0x3A, 0xff},
 *			{0xA2, 0x2C, 0x8E, 0xff},
 *			{0x23, 0xD4, 0x72, 0xff},
 *			{0x54, 0xB8, 0xC6, 0xff},
 *			{0x83, 0x9C, 0x2C, 0xff},
 *			{0xB4, 0x80, 0x80, 0xff}
 *
\*****************************************************************************/
{
    uint8_t i, j;
    int32_t    temp_YUV[3];

    OSD_REQUIRE( clutStartNum < OSD_TOTAL_CLUT_NUM );

    if ( clutStartNum < OSD_TOTAL_CLUT_NUM )
    {
        for (i = clutStartNum; i <= clutEndNum; i++)
        {
            for (j = 0; j < OSD_MAX_CLUT_ENTRY_NUM; j++)
            {
                // Y
                temp_YUV[0] =  (299L * OSD_clutValue[i][j].r + 587L * OSD_clutValue[i][j].g + 114L *OSD_clutValue[i][j].b)/1000L;
                temp_YUV[0] =  (TTX_DISP_SetUpAndDownLimit(temp_YUV[0], 254, 16));

                // Cb
                temp_YUV[1] =  ((-172) * OSD_clutValue[i][j].r + (-339) * OSD_clutValue[i][j].g + 511L * OSD_clutValue[i][j].b)/1000L;
                temp_YUV[1] += 128L;
                //temp_YUV[1] =  (TTX_DISP_SetUpAndDownLimit(temp_YUV[1], 240, 16));
                temp_YUV[1] =  (TTX_DISP_SetUpAndDownLimit(temp_YUV[1], 210, 16));

                // Cr
                temp_YUV[2] =  (511L * OSD_clutValue[i][j].r - 428L * OSD_clutValue[i][j].g - 83L * OSD_clutValue[i][j].b)/1000L;
                temp_YUV[2] += 128L;
                //temp_YUV[2] =  (TTX_DISP_SetUpAndDownLimit(temp_YUV[2], 240, 16));
                temp_YUV[2] =  (TTX_DISP_SetUpAndDownLimit(temp_YUV[2], 210, 16));



                /* Commented by Janus: This is RTD2638's setting, use our Sparrow's.
                                OSD_clutValue[i][j].r = temp_YUV[2]; // U
                                OSD_clutValue[i][j].g = temp_YUV[0]; // Y
                                OSD_clutValue[i][j].b = temp_YUV[1]; // V
                */
                // Sparrow's YUV
                OSD_clutValue[i][j].r = temp_YUV[0] & 0xFF; // Y
                OSD_clutValue[i][j].g = temp_YUV[1] & 0xFF; // Cb
                OSD_clutValue[i][j].b = temp_YUV[2] & 0xFF; // Cr
            }
        }
    }
}
#endif  /* end of #if 0 */




/* -- Implementations of Private Functions ----------------------------------*/

/*---------------------------------------------------------------------------*/
static OSD_TtAttr_t TTX_MEM*
TTX_DISP_GetAttrMem                 (const TTX_DISP_AttrMem_t attrMem)
/*****************************************************************************\
 *  @brief   Clear output display.
 *  @pre     This __FUNCTION__ must be called after calling TTX_DISP_Open().
 *             Because this __FUNCTION__ calls OSD_GetTtAttr1Address(), and
 *             OSD_GetTtAttr1Address()'s contents is initialized through TTX_DISP_Open().
\*****************************************************************************/
{
    if ( attrMem == TTX_DISP_ATTR_MEM_SHOWING)
    {
        return( (OSD_TtAttr_t TTX_MEM*) OSD_GetTtAttr1Address() );
    }
    else if ( attrMem == TTX_DISP_ATTR_MEM_BG )
    {
        if ( OSD_GetTtAttr1Address() == TTX_DISP_ATTRx_ADDRESS(0) )
        {
            return( (OSD_TtAttr_t TTX_MEM*) TTX_DISP_ATTRx_ADDRESS(1) );
        }
        else
        {
            return( (OSD_TtAttr_t TTX_MEM*) TTX_DISP_ATTRx_ADDRESS(0) );
        }
    }
    else
    {
        TTX_ERROR();    /* ERROR: Unsupported value of attrMem */
    }

    return( NULL );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_DISP_SetAttrHelper       (OSD_TtAttr_t TTX_MEM* pAttrBase,
                              const uint8_t row, const uint8_t column,
                              const TTX_DISP_Attr_t* pAttr)
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
    TTX_REQUIRE( row <= TTX_DISP_LAST_ROW );
    return( TTX_DISP_FillRectAttrHelper( pAttrBase, row, column, row, column, pAttr ) );
}

#if TTX_VERIFY  /* if enabled verficiation of TTX related IPs */

/*---------------------------------------------------------------------------*/
static bool_t
TTX_DISP_GetAttrHelper   (const OSD_TtAttr_t TTX_MEM* pAttrBase,
                          const uint8_t row, const uint8_t col,
                          TTX_DISP_Attr_t* pAttr)
/*****************************************************************************\
 *  @brief   Get the attributes of current display.
 *  @param   row    the row you want to get.    0~TTX_DISP_MAX_ROW_NUM.
 *  @param   col    the column you want to get. 0~TTX_DISP_MAX_ROW_LEN.
\*****************************************************************************/
{
    if ( pAttrBase != NULL )
    {
        TTX_REQUIRE( row <= TTX_DISP_LAST_ROW );
        TTX_REQUIRE( col  < TTX_DISP_MAX_ROW_LEN );
        if ( row  < TTX_DISP_MAX_ROW_NUM )
        {
            if ( col  < TTX_DISP_MAX_ROW_LEN )
            {
                const OSD_Address_t addrAtt = (const OSD_Address_t)( pAttrBase + (row * TTX_DISP_MAX_ROW_LEN + col) );

                // Since attributes are stored in xdata, so are registers,
                //   so we get the attributes through TTX_DISP_AttrRead().
                const volatile OSD_TtAttr_t raw = TTX_DISP_AttrRead( addrAtt );

                if ( pAttr != NULL )
                {
                    return( TTX_DISP_OsdAttrToAttr( &raw, pAttr ) );
                }
            }
        }
    }

    TTX_ERROR();    // ERROR: Shouldn't run here.

    return( FALSE );
}

#endif  /* end of #if TTX_VERIFY */

/*---------------------------------------------------------------------------*/
static bool_t
TTX_DISP_FillRowAttrHelper  (OSD_TtAttr_t TTX_MEM*  pAttrBase,
                             const uint8_t row,
                             const TTX_DISP_Attr_t* pAttr)
/*****************************************************************************\
 *  @brief   Fill a rectangle from (upperLeftRow, upperLeftCol) to (lowerLeftRow,
 *             lowerLeftCol) with the same attribute *pAttr.
 *  @param   *pAttr is the attribute you want to use as fill pattern.
 *  @pre     0 <= row  < TTX_DISP_MAX_ROW_NUM
\*****************************************************************************/
{
    TTX_REQUIRE( row <= TTX_DISP_LAST_ROW );
    return( TTX_DISP_FillRectAttrHelper( pAttrBase, row, 0, row, TTX_DISP_MAX_ROW_LEN-1, pAttr ) );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_DISP_CustermizedCharSet (OSD_TtAttr_t TTX_MEM* pAttrBase,
                             const uint8_t Row , const uint8_t Col, const uint8_t modifiedCharset,
                             const TTX_DISP_Attr_t* pAttr )
{
    OSD_TtAttr_t raw;

    if ( pAttr != NULL )
    {
        if ( TTX_DISP_AttrToOsdAttr( pAttr, &raw ) == TRUE )
        {
            OSD_Address_t addrAtt;

            raw &= ~(_BIT7|_BIT8|_BIT23);

            // modify custermized national code
            raw |= ((modifiedCharset&0x03UL)<<7);
            raw |= ((modifiedCharset&0x04UL)<<21);

            addrAtt = (OSD_Address_t)(pAttrBase + (Row * TTX_DISP_MAX_ROW_LEN + Col));

            // Since attributes are stored in xdata, so are registers,
            //   so we write the attributes through TTX_DISP_AttrWrite().
            TTX_DISP_AttrWrite( addrAtt, raw );
        }
        return( TRUE );
    }
    // end of if( pAttr != NULL )

    TTX_ERROR();    // ERROR: Shouldn't run here.
    return( FALSE );
}




/*---------------------------------------------------------------------------*/
static bool_t
TTX_DISP_FillRectAttrHelper  (OSD_TtAttr_t TTX_MEM* pAttrBase,
                              const uint8_t upperLeftRow , const uint8_t upperLeftCol,
                              const uint8_t lowerRightRow, const uint8_t lowerRightCol,
                              const TTX_DISP_Attr_t* pAttr )
/*****************************************************************************\
 *  @brief   Fill a rectangle from (upperLeftRow, upperLeftCol) to (lowerLeftRow,
 *             lowerLeftCol) with the same attribute *pAttr.
 *  @param   *pAttr is the attribute you want to use as fill pattern.
 *  @pre     0 <= upperLeftRow  < TTX_DISP_MAX_ROW_NUM
 *  @pre     0 <= upperLeftCol  < TTX_DISP_MAX_ROW_LEN
 *  @pre     0 <= lowerRightRow < TTX_DISP_MAX_ROW_NUM
 *  @pre     0 <= lowerRightCol < TTX_DISP_MAX_ROW_LEN
 *  @pre     upperLeftRow <= lowerRightRow
 *  @pre     upperLeftCol <= lowerRightCol
\*****************************************************************************/
{
    OSD_TtAttr_t raw;

    if ( pAttr != NULL )
    {
        if ( TTX_DISP_AttrToOsdAttr( pAttr, &raw ) == TRUE )
        {
            uint8_t row, col;
            OSD_Address_t addrAtt;

            TTX_REQUIRE( upperLeftRow  <= TTX_DISP_LAST_ROW    );
            TTX_REQUIRE( upperLeftCol  <  TTX_DISP_MAX_ROW_LEN );
            TTX_REQUIRE( lowerRightRow <= TTX_DISP_LAST_ROW    );
            TTX_REQUIRE( lowerRightCol <  TTX_DISP_MAX_ROW_LEN );

            TTX_REQUIRE( upperLeftRow <= lowerRightRow );
            TTX_REQUIRE( upperLeftCol <= lowerRightCol );

            if ( ( upperLeftRow <= lowerRightRow ) && ( upperLeftCol <= lowerRightCol ) )
            {
                TTX_REQUIRE( pAttrBase != NULL );
                if ( pAttrBase != NULL )
                {
                    for ( row = upperLeftRow; row <= lowerRightRow; row++ )
                    {
                        for ( col = upperLeftCol; col <= lowerRightCol; col++ )
                        {
                            addrAtt = (OSD_Address_t)(pAttrBase + (row * TTX_DISP_MAX_ROW_LEN + col));

                            // Since attributes are stored in xdata, so are registers,
                            //   so we write the attributes through TTX_DISP_AttrWrite().
                            TTX_DISP_AttrWrite( addrAtt, raw );
                        }
                    }

                    return( TRUE );
                }
            }
        }
    }   // end of if( pAttr != NULL )

    TTX_ERROR();    // ERROR: Shouldn't run here.
    return( FALSE );
}

/*---------------------------------------------------------------------------*/
static void
TTX_DISP_DrawSubPageNavCtrlHelper  (
    const uint8_t row, const TTX_DISP_SubPageNavCtrl_t* pParam)
/*****************************************************************************\
 *  @brief   Renders the "sub-page control".
 *  @note    Mimic vbiSys_ShowSubPage().
 *  @param   [in]  row is the row to be drawn.
 *  @param   [in]  pParam is the drawing parameter that contains the sub-page
 *                   info to be drawn. If you leave (pParam == NULL), this
 *                   __FUNCTION__ will erase existing sub-page nav control.
\*****************************************************************************/
{
    uint8_t col;
    uint8_t modifiedCharset;
    TTX_DISP_Attr_t attr;

    //OSD_TtAttr_t TTX_MEM* const pFGMem = TTX_DISP_GetAttrMem( TTX_DISP_ATTR_MEM_SHOWING );
    OSD_TtAttr_t TTX_MEM* const pBGMem = TTX_DISP_GetAttrMem( TTX_DISP_ATTR_MEM_BG );

    TTX_REQUIRE( UINT8_MAX >= TTX_DISP_LAST_ROW );  /* ERROR: typeof(row) cannot cover full range of displayable rows */
    TTX_REQUIRE( row <= TTX_DISP_LAST_ROW );        /* ERROR: invalid range of row */

    /* No matter pParam->subPageCntTotal >0 or equals zero, erase all sub-page control
     *   before rendering it to avoid un-erased '<<' or '>>'.
     */
    {
        memset( &attr, 0, sizeof( attr ) );

        /* We shall draw transparent SubPageCtrl when showing page is subtitle or news flash */
        attr.conceal = OSD_GetTtBoxEnable() ? TTX_CONCEAL_ON : TTX_CONCEAL_OFF;

        // Erase all
        //TTX_DISP_FillRectAttrHelper( pFGMem, row, 0, row, (TTX_DISP_SUBPAGE_MAX_NUM * 3 + 6) + 1, &attr );
        //TTX_DISP_FillRectAttrHelper( pBGMem, row, 0, row, (TTX_DISP_SUBPAGE_MAX_NUM * 3 + 6) + 1, &attr );
        TTX_DISP_FillRectAttrHelper( pBGMem, row, 0, row, 39, &attr );
    }


    if ( pParam != NULL )
    {
        /* Draw sub-page control */
        if ( pParam->cntTotal > 0 )
        {
            // Draw auto-rotation icon
            {
                memset( &attr, 0, sizeof( attr ) );
                attr.charset = TTX_CHARSET_G0_LATIN;
                if ( ttx_subpage_auto_rotation.rotate )
                {
                    attr.fgColorIdx = TTX_COLOR_BLACK;
                    attr.bgColorIdx = TTX_COLOR_WHITE;
                }
                else
                {
                    attr.fgColorIdx = TTX_COLOR_WHITE;
                    attr.bgColorIdx = TTX_COLOR_BLACK;
                }
                //attr.charIdx = 'A';
                attr.charIdx = 0x2E;
                col = 4;

                //which is corresponding to osd_fonts map in set 6
                //C (TTX_CHARSET_G0_LATIN_TURKISH, small G0 character for diacritical mark)
                modifiedCharset = 6;

                //TTX_DISP_CustermizedCharSet(pFGMem, row, col, 6, &attr);  //mapping auto rotation icon
                TTX_DISP_CustermizedCharSet(pBGMem, row, col, 6, &attr);

                attr.charIdx = 0x2F;
                col = 5;
                modifiedCharset = 6;

                //TTX_DISP_CustermizedCharSet(pFGMem, row, col, 6, &attr);
                TTX_DISP_CustermizedCharSet(pBGMem, row, col, 6, &attr);
            }

            {
                uint8_t i;
                TTX_STD_SubPage_Displayable_Num_t n, t;

                memset( &attr, 0, sizeof( attr ) );

                attr.charset    = TTX_CHARSET_G0_LATIN;
                //attr.fgColorIdx = TTX_COLOR_CYAN;

                col = 7;

                for ( i=0; (i<TTX_DISP_SUBPAGE_MAX_NUM)&& (i<pParam->cnt); i++, col+=3 )
                {
                    t = i%TTX_DISP_SUBPAGE_MAX_NUM;
                    //RTD_Log(LOGGER_INFO, "pParam->idxStart : %d, t : %d \n", (UINT32) pParam->idxStart, (UINT32) t);

                    if ( (!ttx_subpage_auto_rotation.rotate) && (t == pParam->idxHighlight) )
                    {
                        attr.fgColorIdx = TTX_COLOR_BLACK;
                        attr.bgColorIdx = TTX_COLOR_WHITE;
                    }
                    else
                    {
                        attr.fgColorIdx = TTX_COLOR_WHITE;
                        attr.bgColorIdx = TTX_COLOR_BLACK;
                    }
                    // Draw sub-page number tens
                    TTX_REQUIRE( t < TTX_DISP_SUBPAGE_MAX_NUM );
                    if ( t < TTX_DISP_SUBPAGE_MAX_NUM )
                    {
                        if ( ( n = ((pParam->list[t]>>4) & 0xF) ) < 10 )
                        {
                            attr.charIdx = '0' + n;
                        }
                        else
                        {
                            attr.charIdx = 'A' + (n - 10);
                        }

                        //RTD_Log(LOGGER_INFO, "pParam->list[%d] : %x \n", (UINT32) t, (UINT32) pParam->list[t]);
                        //TTX_DISP_SetAttrHelper( pAttrBase, row, col, &attr );
                        //TTX_DISP_SetAttrHelper( pFGMem, row, col, &attr );
                        TTX_DISP_SetAttrHelper( pBGMem, row, col, &attr );


                        // Draw sub-page number units
                        if ( ( n = (pParam->list[t] & 0xF) ) < 10 )
                        {
                            attr.charIdx = '0' + n;
                        }
                        else
                        {
                            attr.charIdx = 'A' + (n - 10);
                        }
                        //TTX_DISP_SetAttrHelper( pFGMem, row, col+1, &attr );
                        TTX_DISP_SetAttrHelper( pBGMem, row, col+1, &attr );

                    }
                }
            }

            // Draw left arrow
            {
                memset( &attr, 0, sizeof( attr ) );
                attr.charset    = TTX_CHARSET_G0_LATIN;
                attr.fgColorIdx = TTX_COLOR_RED;
                attr.bgColorIdx = TTX_COLOR_BLACK;
                attr.charIdx = '<';
                col = 1;
                // Draw red '<<'
                //TTX_DISP_FillRectAttrHelper( pFGMem, row, col, row, col+1, &attr );
                TTX_DISP_FillRectAttrHelper( pBGMem, row, col, row, col+1, &attr );
            }
            // Draw right arrow
            {
                memset( &attr, 0, sizeof( attr ) );
                attr.charset = TTX_CHARSET_G0_LATIN;
                attr.fgColorIdx = TTX_COLOR_RED;
                attr.bgColorIdx = TTX_COLOR_BLACK;
                attr.charIdx = '>';
                col = 37;
                // Draw red '>>'
                //TTX_DISP_FillRectAttrHelper( pFGMem, row, col, row, col+1, &attr );
                TTX_DISP_FillRectAttrHelper( pBGMem, row, col, row, col+1, &attr );
            }
        }   /* end of if( pParam->subPageCntTotal > 0 ) */
    }   /* end of if( pParam != NULL ) */
}

/*---------------------------------------------------------------------------*/
static void
TTX_DISP_ZeroAttrMemHelper               (OSD_TtAttr_t TTX_MEM* pAttrMem)
/*****************************************************************************\
 *  @brief   Dump attr[] to n-th row of addrBaseOsdAttrMem.
 *  @details attr[0] ~ attr[TTX_DISP_MAX_ROW_LEN-1]
\*****************************************************************************/
{
    OSD_REQUIRE( pAttrMem != NULL );
    if ( pAttrMem != NULL )
    {
        memset( pAttrMem, 0, TTX_DISP_ATTR_SIZE ); // Set memory all zero.
        /*
                // Fill all attribute memory as transparent
                {
                    TTX_DISP_Attr_t attr;
                    memset( &attr, 0, sizeof(TTX_DISP_Attr_t) );
                    attr.conceal = TRUE;
                    TTX_DISP_FillRectAttrHelper( pAttrMem, 0, 0, TTX_DISP_MAX_ROW_NUM-1, TTX_DISP_MAX_ROW_LEN-1 , &attr );
                }
        */
    }
}

/*---------------------------------------------------------------------------*/
static void
TTX_DISP_CopyToAttrMemHelper    (OSD_TtAttr_t TTX_MEM* pAttrBase, const TTX_DISP_Attr_t attr[TTX_DISP_MAX_ROW_LEN], uint8_t row)
/*****************************************************************************\
 *  @brief   Dump attr[] to n-th row of addrBaseOsdAttrMem.
 *  @details attr[0] ~ attr[TTX_DISP_MAX_ROW_LEN-1]
\*****************************************************************************/
{
    uint8_t i;
    OSD_TtAttr_t tmp;
    OSD_TtAttr_t TTX_MEM* pDest;

    OSD_REQUIRE( row < TTX_DISP_MAX_ROW_NUM );
    if ( (pAttrBase != NULL)&&(attr != NULL) )
    {
        pDest = (OSD_TtAttr_t TTX_MEM*)((uint8_t TTX_MEM*)pAttrBase + row * OSD_DISP_ATTR_SIZE * TTX_DISP_MAX_ROW_LEN);

        for ( i=0; i < TTX_DISP_MAX_ROW_LEN; i++ )
        {
            if ( TTX_DISP_AttrToOsdAttr( &(attr[i]), &tmp ) == TRUE )
            {
                pDest[i] = tmp;
            }
        }
    }

}

/*---------------------------------------------------------------------------*/
static void
TTX_DISP_AttrWrite    (TTX_Address_t addr, OSD_TtAttr_t raw)
/*****************************************************************************\
 *  @brief   Write val to 32-bit registers.
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
    /* Because WIN32's TTX_Reg32Write() and TTX_Reg32Read() are empty,
     *           and we still needs to read/write attribute memory,
     *           so we independently write TTX_DISP_AttrWrite() and TTX_DISP_AttrRead() here.
     */
#ifdef WIN32
#define         _TTX_Reg8Write(addr,val) (*((volatile uint8_t*) addr) = (val&0xFF))

    /* WIN32 is little endian */
    _TTX_Reg8Write( addr+0, raw>>0*8 );
    _TTX_Reg8Write( addr+1, raw>>1*8 );
    _TTX_Reg8Write( addr+2, raw>>2*8 );
    _TTX_Reg8Write( addr+3, raw>>3*8 );
#else

#if 0
    TTX_Reg8Write( addr+0, raw>>3*8 );
    TTX_Reg8Write( addr+1, raw>>2*8 );
    TTX_Reg8Write( addr+2, raw>>1*8 );
    TTX_Reg8Write( addr+3, raw>>0*8 );
#else
    uint8_t*  pRaw = (UINT8*) &raw;

    TTX_Reg8Write( addr+0, *pRaw);
    TTX_Reg8Write( addr+1, *(pRaw+1) );
    TTX_Reg8Write( addr+2, *(pRaw+2) );
    TTX_Reg8Write( addr+3, *(pRaw+3) );
#endif

#endif  /* end of #ifdef WIN32 */
}

#if TTX_VERIFY  /* if enabled verficiation of TTX related IPs */
/*---------------------------------------------------------------------------*/
static OSD_TtAttr_t
TTX_DISP_AttrRead     (TTX_Address_t addr)
/*****************************************************************************\
 *  @brief   Read value from 32-bit registers.
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
    /* Because WIN32's TTX_Reg32Write() and TTX_Reg32Read() are empty,
     *           and we still needs to read/write attribute memory,
     *           so we independently write TTX_DISP_AttrWrite() and TTX_DISP_AttrRead() here.
     */
#ifdef WIN32

#define         _TTX_Reg8Read(addr)      (*((volatile uint8_t*) addr))

    /* WIN32 is little endian */

    volatile uint32_t val;

    val  =  (uint32_t)(_TTX_Reg8Read(addr+0));
    val |= ((uint32_t)(_TTX_Reg8Read(addr+1)))<<8;
    val |= ((uint32_t)(_TTX_Reg8Read(addr+2)))<<16;
    val |= ((uint32_t)(_TTX_Reg8Read(addr+3)))<<24;

    return( val );
#else
    volatile uint32_t val;

    val  =  (uint32_t)(TTX_Reg8Read(addr+3));
    val |= ((uint32_t)(TTX_Reg8Read(addr+2)))<<8;
    val |= ((uint32_t)(TTX_Reg8Read(addr+1)))<<16;
    val |= ((uint32_t)(TTX_Reg8Read(addr+0)))<<24;

    return( val );
#endif  /* end of #ifdef WIN32 */
}
#endif  /* end of #if TTX_VERIFY */

void TTX_DISP_DrawNoTTMsg(bool_t enable)
/*****************************************************************************\
 *  @brief display no TT signal
 *
 *  @param <enable>    TRUE: display message  FALSE: clear message
\*****************************************************************************/
{
    UINT8 i;
    TTX_DISP_Attr_t attr;
    const uint8_t label[TTX_DISP_MAX_ROW_LEN] = "     No TXT transmission available      ";

    memset( &attr, 0, sizeof(attr) );

    if ( enable == TRUE )
    {
        attr.charset = TTX_CHARSET_G0_LATIN;
        attr.fgColorIdx = TTX_COLOR_BLACK;
        attr.bgColorIdx = TTX_COLOR_CYAN;
        attr.box = TTX_BOX_START; // also display in subtitle mode
        for ( i=0 ; i<TTX_DISP_MAX_ROW_LEN ; ++i )
        {
            attr.charIdx = label[i];
            TTX_DISP_SetAttrHelper( TTX_DISP_GetAttrMem( TTX_DISP_ATTR_MEM_SHOWING), 25, i, &attr );
            TTX_DISP_SetAttrHelper( TTX_DISP_GetAttrMem( TTX_DISP_ATTR_MEM_BG ), 25, i, &attr );
        }
    }
    else
    {
        for ( i=0 ; i<TTX_DISP_MAX_ROW_LEN ; ++i )
        {
            TTX_DISP_SetAttrHelper( TTX_DISP_GetAttrMem( TTX_DISP_ATTR_MEM_SHOWING), 25, i, &attr );
            TTX_DISP_SetAttrHelper( TTX_DISP_GetAttrMem( TTX_DISP_ATTR_MEM_BG ), 25, i, &attr );
        }
    }
}

void TTX_DISP_DrawSelectSubpage (const TTX_STD_Page_AddrWithInputSubCode_t pageAddr)
/*****************************************************************************\
 *  @brief   draw "select subpage" OSD
\*****************************************************************************/
{
    const uint8_t row = TTX_DISP_TOP_CTRL_ROW_NUM;

    TTX_DISP_Attr_t attr;
    uint8_t i;

    { // draw a black space
        memset( &attr, 0, sizeof( attr ) );
        attr.charset = TTX_CHARSET_G0_LATIN;
        attr.fgColorIdx = TTX_COLOR_BLACK;
        attr.bgColorIdx = TTX_COLOR_BLACK;
        attr.box = TTX_BOX_START; // also display in subtitle mode
        attr.charIdx = ' ';
        TTX_DISP_SetAttrHelper( TTX_DISP_GetAttrMem( TTX_DISP_ATTR_MEM_SHOWING ), row, 0, &attr );
        TTX_DISP_SetAttrHelper( TTX_DISP_GetAttrMem( TTX_DISP_ATTR_MEM_BG ), row, 0, &attr );
    }


    { // draw previous subpage
        enum { len = 5 };
        const uint8_t label[len] = "  P- ";

        memset( &attr, 0, sizeof( attr ) );

        attr.charset = TTX_CHARSET_G0_LATIN;
        attr.fgColorIdx = TTX_COLOR_BLACK;
        attr.bgColorIdx = TTX_COLOR_RED;
        attr.box = TTX_BOX_START; // also display in subtitle mode

        for ( i=0 ; i<len ; ++i )
        {
            attr.charIdx = label[i];
            TTX_DISP_SetAttrHelper( TTX_DISP_GetAttrMem( TTX_DISP_ATTR_MEM_SHOWING ), row, 1+i, &attr );
            TTX_DISP_SetAttrHelper( TTX_DISP_GetAttrMem( TTX_DISP_ATTR_MEM_BG ), row, 1+i, &attr );
        }
    }

    { // draw next subpage
        enum { len = 5 };
        const uint8_t label[len] = "  P+ ";

        memset( &attr, 0, sizeof( attr ) );

        attr.charset = TTX_CHARSET_G0_LATIN;
        attr.fgColorIdx = TTX_COLOR_BLACK;
        attr.bgColorIdx = TTX_COLOR_GREEN;
        attr.box = TTX_BOX_START; // also display in subtitle mode

        for ( i=0 ; i<len ; ++i )
        {
            attr.charIdx = label[i];
            TTX_DISP_SetAttrHelper( TTX_DISP_GetAttrMem( TTX_DISP_ATTR_MEM_SHOWING ), row, 6+i, &attr );
            TTX_DISP_SetAttrHelper( TTX_DISP_GetAttrMem( TTX_DISP_ATTR_MEM_BG ), row, 6+i, &attr );
        }
    }

    { // draw yellow bar
        enum { len = 15 };

        memset( &attr, 0, sizeof( attr ) );

        attr.charset = TTX_CHARSET_G0_LATIN;
        attr.fgColorIdx = TTX_COLOR_BLACK;
        attr.bgColorIdx = TTX_COLOR_YELLOW;
        attr.box = TTX_BOX_START; // also display in subtitle mode

        for ( i=0 ; i<len ; ++i )
        {
            attr.charIdx = ' ';
            TTX_DISP_SetAttrHelper( TTX_DISP_GetAttrMem( TTX_DISP_ATTR_MEM_SHOWING ), row, 11+i, &attr );
            TTX_DISP_SetAttrHelper( TTX_DISP_GetAttrMem( TTX_DISP_ATTR_MEM_BG ), row, 11+i, &attr );
        }
    }

    { // draw page
        enum { len = 7 };
        uint8_t label[len+1] = "       "; // +1 for '\0'
        uint8_t magNum = (pageAddr.pageA&0xF00)>>8;
        uint8_t page_tens = (pageAddr.pageA&0xF0)>>4;
        uint8_t page_units = (pageAddr.pageA&0xF);


        //if ( ((pageAddr.magNum&0xF) > 0x8) || (pageAddr.pageNum.tens >= 0xA) || (pageAddr.pageNum.units >= 0xA) )
        if ((magNum>0x8) ||(page_tens>0xA) ||(page_units>0xA))
        {
            //snprintf( label, len+1, "     --- /" );
            label[len+1] = " P---/";
        }
        else
        {
            //snprintf( label, len+1, "     %d%d%d /", magNum, page_tens, page_units);
            label[2] = 'P';
            label[3] = '0' + magNum;
            label[4] = '0' + page_tens;
            label[5] = '0' + page_units;
            label[6] = '/';
        }
        memset( &attr, 0, sizeof( attr ) );

        attr.charset = TTX_CHARSET_G0_LATIN;
        attr.fgColorIdx = TTX_COLOR_BLACK;
        attr.bgColorIdx = TTX_COLOR_CYAN;
        attr.box = TTX_BOX_START; // also display in subtitle mode

        for ( i=0; i<len; ++i )
        {
            attr.charIdx = label[i];
            TTX_DISP_SetAttrHelper( TTX_DISP_GetAttrMem( TTX_DISP_ATTR_MEM_SHOWING ), row, 26+i, &attr );
            TTX_DISP_SetAttrHelper( TTX_DISP_GetAttrMem( TTX_DISP_ATTR_MEM_BG ), row, 26+i, &attr );
        }
    }

    { // draw subpage
        enum { len = 7 };
        uint8_t label[len] = "----   ";
        TTX_STD_Page_Addr_SubCode_t subCode;


        subCode = TTX_STD_SubCodeAToSubcode(pageAddr.subcodeA);

        if ( subCode.s4 < 0xA )
        {
            label[0] = '0' + subCode.s4;
        }

        if ( subCode.s3 < 0xA )
        {
            label[1] = '0' + subCode.s3;
        }

        if ( subCode.s2 < 0xA )
        {
            label[2] = '0' + subCode.s2;
        }

        if ( subCode.s1 < 0xA )
        {
            label[3] = '0' + subCode.s1;
        }

        memset( &attr, 0, sizeof( attr ) );

        attr.charset = TTX_CHARSET_G0_LATIN;
        attr.fgColorIdx = TTX_COLOR_BLACK;
        attr.bgColorIdx = TTX_COLOR_CYAN;
        attr.box = TTX_BOX_START; // also display in subtitle mode

        for ( i=0; i<len; ++i )
        {
            attr.charIdx = label[i];
            TTX_DISP_SetAttrHelper( TTX_DISP_GetAttrMem( TTX_DISP_ATTR_MEM_SHOWING ), row, 33+i, &attr );
            TTX_DISP_SetAttrHelper( TTX_DISP_GetAttrMem( TTX_DISP_ATTR_MEM_BG ), row, 33+i, &attr );
        }
    }
}
/*---------------------------------------------------------------------------*\
 * end of ttx_disp.c                                                         *
\*---------------------------------------------------------------------------*/
#endif
