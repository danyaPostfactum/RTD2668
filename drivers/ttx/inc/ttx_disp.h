/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_disp.h
 * @brief    Teletext display interface.
 * @author   Janus Cheng
 * @date     Mar. 05, 2009
 * @version  0.1
 * @ingroup  osd
\*****************************************************************************/

#ifndef __ENHANCED_TELETEXT_DECODER_LIBRARY_OUTPUT_DISPLAY_INTERFACE_HEADER_INCLUDED__
#define __ENHANCED_TELETEXT_DECODER_LIBRARY_OUTPUT_DISPLAY_INTERFACE_HEADER_INCLUDED__ (1)

/* -- TTX headers -----------------------------------------------------------*/
#include <ttx_std.h>    /* Provides: TTX_STD_CharSet_t */
#include <ttx_obj.h>    /* Provides: TTX_NAV_Info_t */


#ifdef __cplusplus
extern "C"
{
#endif

    /* -- Global Macro Definitions ----------------------------------------------*/
#if TTX_CFG_PRESENT_LEVEL >= TTX_STD_PRESENT_LEVEL(25)
#define TTX_DISP_MAX_ROW_LEN            (56)    // the maxium length of a row
#else
#define TTX_DISP_MAX_ROW_LEN            (40)    // the maxium length of a row
#endif
#define TTX_DISP_MAX_ROW_LEN_WITHOUT_SP     (40)    // the maxium length of a row without side panel
#define TTX_DISP_TOP_CTRL_ROW_NUM           (24)    // TOP control bar is located at row-24
#define TTX_DISP_SUBPAGE_NAV_CTRL_ROW_NUM   (25)    // sub-page navigation control bar is located at row-25
#define TTX_DISP_MAX_ROW_NUM                (26)    // the maxium number of rows
#define TTX_DISP_SUBPAGE_MAX_NUM            (5)     // at most 5 subpages will be shown
//#define TTX_DISP_SUBPAGE_MAX_NUM            (10)     // at most 5 subpages will be shown

#define TTX_DISP_LAST_ROW                   (TTX_DISP_MAX_ROW_NUM-1)


    /* -- Global typedef Declarations -------------------------------------------*/

    /* TTX_SrcOpen() flags */
    typedef enum
    {
        TTX_DISP_OPEN_NOP   = 0x00000000u,
        TTX_DISP_OPEN_RESET = 0x00000001u
    } TTX_DISP_OpenFlag_t;


// Display attribute of a character
    typedef struct TTX_DISP_Attr_t
    {
        TTX_STD_CharSet_t       charset    :6;  // @todo verify why this be 5-bit will cause decode error?
        TTX_STD_CharIndex_t     charIdx    :9;  // only 9 bits used

        TTX_STD_Color_t         bgColorIdx :3;
        TTX_STD_Color_t         fgColorIdx :3;
        TTX_STD_ColorTable_t    bgColorTab :2;
        TTX_STD_ColorTable_t    fgColorTab :2;


        TTX_STD_DiacMark_t      dmIdx      :5;
        TTX_STD_Mosaics_t       mosaics    :1;
        TTX_STD_ConcealFlag_t   conceal    :1;
        TTX_STD_Graphic_t       graphic    :1;
        TTX_STD_Box_t           box        :1;
        TTX_STD_SizeCfg_t       size       :4;

        uint8_t                 bOverline  :1;
        uint8_t                 bUnderline :1;

#if TTX_CFG_PRESENT_LEVEL >= TTX_STD_PRESENT_LEVEL(25)
        TTX_STD_FlashMode_t     flashMode  :1;
        TTX_STD_FlashRate_t     flashRate  :3;
#else
        TTX_STD_FlashMode_t     flashMode  :2;
#endif


    } TTX_DISP_Attr_t;

    typedef struct TTX_DISP_SubPageRotateCtrl_t
    {
        uint8_t	hold : 1;
        uint8_t	rotate : 1;
    } TTX_DISP_SubPageRotateCtrl_t;

    extern TTX_DISP_SubPageRotateCtrl_t ttx_subpage_auto_rotation; // auto rotate subpage
    extern TTX_STD_Page_AddrWithInputSubCode_t ttx_select_subpage;



    typedef struct TTX_DISP_SubPageNavCtrl_t
    {
        /* list[] is the computed subpage-list */
        TTX_STD_SubPage_Displayable_Num_t  list[TTX_DISP_SUBPAGE_MAX_NUM];

        /* idxStart: subPageList[ idxStart ] is the first element */
        TTX_STD_SubPage_Displayable_Num_t  idxStart;

        /* cnt: subPageList[(cnt-1)%TTX_DISP_SUBPAGE_MAX_NUM] is the last element. */
        TTX_STD_SubPage_Displayable_Num_t  cnt;

        /* idxHighlight is the index of subPageList[] to be highlighted. */
        TTX_STD_SubPage_Displayable_Num_t  idxHighlight;

        /* cntTotal is the total amount of subpages of the showing page.
         *                   If you don't know the number of subPageCntTotal, set it
         *                   to TTX_STD_SUBPAGE_DISPLAYABLE_MAX_NUM.
         */
        TTX_STD_SubPage_Displayable_Num_t  cntTotal;
    } TTX_DISP_SubPageNavCtrl_t;

    typedef struct TTX_DISP_PageNumAttr_t
    {
        uint8_t	bEnablePageUnits : 1; // display page units or '-'
        uint8_t	bEnablePageTens : 1; // display page tens or '-'
        uint8_t	bEnableMag : 1; // display magazine or '-'
        uint8_t	bEnableP : 1; // display 'P' or ' '
        uint8_t	ForegroundColor : 4; // foreground color (see TTX_STD_ColorValues_t)
        uint8_t	ValidBackgroundColor : 4; // background color for valid magazine
        uint8_t	InvalidBackgroundColor : 4; // background color for invalid magazine
    } TTX_DISP_PageNumAttr_t;

    /* -- Function Prototypes ---------------------------------------------------*/

    /* Initialize Teletext output interface */
    bool_t                  TTX_DISP_Open                       (const TTX_DISP_OpenFlag_t flags);

    /* Close Teletext output interface */
    bool_t                  TTX_DISP_Close                      (void);

    bool_t                  TTX_DISP_Reset                      (void);

    bool_t                  TTX_DISP_Test                       (void);

    void  			TTX_DISP_ClrNavCtrl(void);


    /* Attribute processing */
    void                    TTX_DISP_ZeroAttrMem                (void);
    void                    TTX_DISP_CopyToAttrMem              (const TTX_DISP_Attr_t attrDecoded[TTX_DISP_MAX_ROW_LEN], uint8_t row);

    bool_t                  TTX_DISP_SetAttr                    (const uint8_t row         , const uint8_t column, const TTX_DISP_Attr_t* pAttr);
    bool_t                  TTX_DISP_GetAttr                    (const uint8_t row         , const uint8_t column,       TTX_DISP_Attr_t* pAttr);
    bool_t                  TTX_DISP_FillRowAttr                (const uint8_t row                               , const TTX_DISP_Attr_t* pAttr);
    bool_t                  TTX_DISP_FillRectAttr               (const uint8_t upperLeftRow, const uint8_t upperLeftCol, const uint8_t lowerRightRow, const uint8_t lowerRightCol, const TTX_DISP_Attr_t* pAttr );

    void                    TTX_DISP_SwitchAttrMem              (void); /* toggle showing attribute memory between TTX_DISP_AttrMemValues_t */

    void                    TTX_DISP_DrawSubPageNavCtrl         (const TTX_DISP_SubPageNavCtrl_t* pParam);

    /* Global processing */
    void                    TTX_DISP_Show                       (const bool_t bShow);
    bool_t                  TTX_DISP_isShowing                  (void);
    void                    TTX_DISP_ShowRow0                   (const bool_t bShow);
    void 		     TTX_DISP_SetPageNum (
        const TTX_STD_Page_AddrWithSubCode_t* pPageAddr,
        const TTX_DISP_PageNumAttr_t PageNumAttr );
    void                    TTX_DISP_EnableTimecode             (const bool_t bEnable);
    void                    TTX_DISP_EnableRolling              (const bool_t bEnable);
    void                    TTX_DISP_EnableBox                  (const bool_t bEnable);
    void 		     TTX_DISP_SubpageHold (void);
    void 	            TTX_DISP_SubpageRotate (void);


    /* Refresh output display */
    void                    TTX_DISP_PostRefreshEntireScreen    (void);     /* Signals the entire screen needs to be updated (non-blocking). */
    void                    TTX_DISP_PostRefreshRow0            (void);     /* Signals the row-0 needs to be refreshed (non-blocking). */
    void                    TTX_DISP_PostRefreshPageContent     (void);     /* Signals the content of row-0 to row-23 needs to be updated (non-blocking). */
    void                    TTX_DISP_PostRefreshTopCtrl         (void);     /* signals the TOP control bar (row-24) needs to be refreshed (non-blocking). */
    void                    TTX_DISP_PostRefreshSubPageNavCtrl  (void);     /* signals the sub-page navigation control (row-25) needs to be refreshed (non-blocking). */

    void                    TTX_DISP_DoRefresh                  (void);     /* Actually do refreshing screen if someone did TTX_DISP_PostRefresh(). (blocking) */
    void                    TTX_DISP_onScreenCanBeRefreshed     (void);     /* event handler of signaling display to TTX_DISP_DoRefresh() */

    /* inter-module functions */
    bool_t                  TTX_DISP_AttrToOsdAttr              (const TTX_DISP_Attr_t*  pAttr, uint32_t*      pUint32);
    bool_t                  TTX_DISP_OsdAttrToAttr              (const volatile uint32_t* pRaw, TTX_DISP_Attr_t* pAttr);
    void                    TTX_DISP_TtAttrCselToFontOffset     (TTX_STD_DiacMark_t dm_index, TTX_STD_CharSet_t cset, TTX_STD_CharIndex_t cindex, uint8_t* pModifiedCharSet, TTX_STD_CharIndex_t* pModifiedCharIndex);
    void                    TTX_DISP_FontOffsetToTtAttrCsel     (uint8_t modifiedCharSet, TTX_STD_CharIndex_t modifiedCharIndex, TTX_STD_CharSet_t* pCset, TTX_STD_CharIndex_t* pCindex);

    /* draw no TT message */
    void TTX_DISP_DrawNoTTMsg(bool_t enable);
    void TTX_DISP_DrawSelectSubpage (const TTX_STD_Page_AddrWithInputSubCode_t pageAddr); // draw "select subpage" OSD

#ifdef __cplusplus
}   /* extern "C" */
#endif


#endif /* !__ENHANCED_TELETEXT_DECODER_LIBRARY_OUTPUT_DISPLAY_INTERFACE_HEADER_INCLUDED__ */


/*---------------------------------------------------------------------------*\
 * end of ttx_disp.h                                                         *
\*---------------------------------------------------------------------------*/

