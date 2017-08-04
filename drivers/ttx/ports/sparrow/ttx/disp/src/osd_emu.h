/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     osd_emu.h
 * @brief    Internal (package scope) shared interfaces for osd_emu.c and osd_test_emu.c.
 * @author   Janus Cheng
 * @date     Dec. 25, 2008
\*****************************************************************************/

/**
 * @addtogroup ttx
 * @{
 */
#ifndef __REALTEK_SPARROW_ON_SCREEN_DISPLAY_SOFTWARE_EMULATOR_HEADER_INCLUDED__
#define __REALTEK_SPARROW_ON_SCREEN_DISPLAY_SOFTWARE_EMULATOR_HEADER_INCLUDED__ (1)

#include <osd.h>        /* Provides: OSD interfaces to be implemented */
#include <ttx_disp.h>   /* Provides: TTX_DISP_Attr_t */
#include <ttx_std.h>    /* Provides: TTX_STD_PACKET_X0_PAYLOAD_SIZE_BYTES */

#ifdef __cplusplus
extern "C"
{
#endif

    /* -- Global typedef Declarations -------------------------------------------*/

    /* -- Public Function Prototypes --------------------------------------------*/





    /* -- Private Function Prototypes ( Do NOT use ) ----------------------------*/
#define OSDEMU_TIMECODE_LEN         (8)   // the length of time-cdoe "12:59:59", which is total 8-column wide.
#define OSDEMU_HEADER_LEN           (TTX_STD_PACKET_X0_PAYLOAD_SIZE_BYTES-OSDEMU_TIMECODE_LEN)  // the length of rolling header without time-code.

// Configures the zooming factors of horizontal and vertial views.
#define OSDEMU_ZOOM_H               (1)  // Horizontal zoom factor. 1: normal. 2: zoom-in by two.
#define OSDEMU_ZOOM_V               (2)  // Vertical   zoom factor. 1: normal. 2: zoom-in by two.




    typedef struct OSDEMU_Timecode_t
    {
        uint8_t                 bDisplay : 1;
    } OSDEMU_Timecode_t;

    typedef struct OSDEMU_Header_t
    {
        uint8_t                 bDisplay :1;
        uint8_t                 bRoll    :1;
    } OSDEMU_Header_t;

    typedef struct OSDEMU_FlashState_t
    {
        uint8_t stepsTotal;
        uint8_t stepsCnt;                 // There're m_stepsTotal steps to complete a period of flashing.
    } OSDEMU_FlashState_t;

    typedef struct OSDEMU_SidePanel_t
    {
        uint8_t bDisplay : 1;             // display side-panels or not
        uint8_t nLeftColNum;              // number of columns of the left side-panel
    } OSDEMU_SidePanel_t;

    typedef struct OSDEMU_Font_t
    {
        uint8_t           pxHeight;       // number of pixels of height of a character
        uint8_t           pxWidth;        // number of pixels of width  of a character
        uint8_t           bytesPerFont;   // number of bytes of a character
        const uint8_t   (*pBmpFonts)  [][OSD_FONT_CHAR_NUM_PER_SET][OSD_FONT_CHAR_SIZE];   // the character bitmap, should point to OSD_ttFont[]
        uint16_t          bmpDm       [OSD_MAX_DM_NUM][OSD_MAX_DM_LINE_NUM];  // diacritical mark
    } OSDEMU_Font_t;

    /* private object, not to be used by application code */
    typedef struct OSDEMU_Obj
    {
        uint8_t              bIntendToDisplay       :1;   /* user command want to display */
        uint8_t              bIsDisplaying          :1;   /* current status of this OSD */
        uint8_t              bBoxEnable             :1;
        uint8_t              bDisplayRow25          :1;

        uint8_t              bRefreshRow0           :1;   /* when TRUE indicates we need to refresh only row-0 to save time rahter than drawing entire screen */
        uint8_t              bRefreshPageContent    :1;   /* when TRUE indicates we need to refresh row-1 to row-23 */
        uint8_t              bRefreshTopCtrl        :1;   /* when TRUE indicates we need to refresh TOP nav bar (row-24) */
        uint8_t              bRefreshSubPageNavCtrl :1;   /* when TRUE indicates we need to refresh sub-page nav control bar (row-25) */

        uint8_t              latestRow0Raw[40];                 /* Contains latest received and decoded row-0, for implmenting time-code and rolling header */
        OSD_TtAttr_t         latestRow0[TTX_DISP_MAX_ROW_LEN];  /* Contains latest received and decoded row-0, for implmenting time-code and rolling header */

        OSD_PageNum_t        pageNum;
        OSDEMU_Header_t      header;
        OSDEMU_Timecode_t    timecode;
        OSDEMU_FlashState_t  flashState;
        OSDEMU_SidePanel_t   sidePanel;
        OSD_CLUT_t           clut          [OSD_TOTAL_CLUT_NUM][OSD_MAX_CLUT_ENTRY_NUM]; // internal color-lookup table
        OSDEMU_Font_t        font;
        TTX_BMPOSD_PxFmt_t*  pBmpDisp;     /* bitmap to be output */
        OSD_TtAttr_t       (*pAttr1)       [TTX_DISP_MAX_ROW_NUM][TTX_DISP_MAX_ROW_LEN];  // the display attribute memory
    } OSDEMU_Obj, *OSDEMU_Handle;


    extern OSDEMU_Obj osdEmu;





    /* -- Function Prototypes ---------------------------------------------------*/






#ifdef __cplusplus
}   /* extern "C" */
#endif


#endif /* !__REALTEK_SPARROW_ON_SCREEN_DISPLAY_SOFTWARE_EMULATOR_HEADER_INCLUDED__ */

/**
 * @}
 */

/*---------------------------------------------------------------------------*\
 * end of osd_emu.h                                                          *
\*---------------------------------------------------------------------------*/
