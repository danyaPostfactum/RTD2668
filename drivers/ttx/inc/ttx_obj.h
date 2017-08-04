/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx.h
 * @brief    Internal (package scope) Ttx/C interface.
 * @details  This header declares the platform-dependent interface for this ttx library.
 * @author   Janus Cheng
 * @date     Dec. 21, 2007
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/

/**
 * @addtogroup ttx
 * @{
 */
#ifndef __REALTEK_ENHANCED_TELETEXT_DECODER_LIBRARY_TELEXT_OBJECT_INTER_PACKAGE_INTERFACE_HEADER_INCLUDED__
#define __REALTEK_ENHANCED_TELETEXT_DECODER_LIBRARY_TELEXT_OBJECT_INTER_PACKAGE_INTERFACE_HEADER_INCLUDED__

#include <ttx_pkg.h>
#include <ttx_std.h>        /* Provides: TTX_STD_Page_AddrWithSubCode_t */
#include <ttx_evt.h>        /* Provides: TTX_EVTS_TYPES */
#include <ttx_assert.h>     /* Provides: TTX_ASSERT_COMPILE() */

#if( TTX_SUPPORT_MT )
#include <ttx_mt.h>     /* Provides: Mutex */
#endif  /* end of #if( TTX_SUPPORT_MT ) */

#ifdef __cplusplus
extern "C"
{
#endif

    /*--- TTX Configurations ----------------------------------------------------*/

    /*--- TTX Constants ---------------------------------------------------------*/

    typedef enum TTX_Cmd_t
    {
        TTX_CMD_NORMAL,         /* decode init page or P100 */
        TTX_CMD_SUBTITLE,       /* jump and decode the sub-title page */
        TTX_CMD_START,          /* start TTX processor from paused */
        TTX_CMD_STOP,           /* stop and hide TTX processor */
        TTX_CMD_PAUSE,          /* pause TTX processor */
        TTX_CMD_SHOW,           /* display decoded page on screen */
        TTX_CMD_HIDE,           /* don't display decoded page screen */
        TTX_CMD_NEXT_PAGE,      /* jump to and decode next page available */
        TTX_CMD_PREV_PAGE,      /* jump to and decode previous page available */
        TTX_CMD_NEXT_SUBPAGE,   /* jump to and decode next sub-page available */
        TTX_CMD_PREV_SUBPAGE,   /* jump to and decode previous sub-page available */
        TTX_CMD_ROTATE_SUBPAGE,
        TTX_CMD_GO_TO_PAGE,     /* jump to and decode a specified page */
        TTX_CMD_BTN_RED,
        TTX_CMD_BTN_GREEN,
        TTX_CMD_BTN_YELLOW,
        TTX_CMD_BTN_CYAN,
        TTX_CMD_SET_SUBTITLE_PAGE_NUM,     /* specify the initial page-address of TTX_CMD_NORMAL or TTX_CMD_SUBTITLE */
        TTX_CMD_SET_PAGE_NUM_INDICATOR,     /* specify the page-address to be shown on the upper-left corner of screen */
        TTX_CMD_VBI_OUT_ON,
        TTX_CMD_VBI_OUT_OFF,
        TTX_CMD_REVEAL, // reveal the concealled content
        TTX_CMD_GO_TO_SUBPAGE, // jump to and decode a specified subpage
        TTX_CMD_HOLD, // hold auto-rotation subpage
        TTX_CMD_ROTATE, // auto-rotate subpage
        TTX_CMD_SUBPAGE,
        TTX_CMD_INDEX_PAGE,
        TTX_CMD_TEST
    } TTX_Cmd_t;

    typedef enum TTX_FLOF_Key_t
    {
        TTX_FLOF_KEY_RED = 0,
        TTX_FLOF_KEY_GREEN,
        TTX_FLOF_KEY_YELLOW,
        TTX_FLOF_KEY_CYAN,
        TTX_FLOF_KEY_RESERVE,
        TTX_FLOF_KEY_INDEX,
        TTX_FLOF_KEY_NUM,
    } TTX_FLOF_Key_t;



    typedef struct TTX_TOP_Service_t
    {
        uint8_t                            bAvailable;
        uint8_t                            bNextPageReady;
        uint8_t                            bPrevPageReady;

        TTX_STD_Page_AddrWithSubCode_t  flofKeys[TTX_FLOF_KEY_NUM];  // use TTX_FLOF_Key_t to index this array.
    } TTX_TOP_Service_t;

    typedef struct TTX_FLOF_Service_t
    {
        uint8_t                            bAvailable;

        TTX_STD_Page_AddrWithSubCode_t  flofKeys[TTX_FLOF_KEY_NUM];  // use TTX_FLOF_Key_t to index this array.
    } TTX_FLOF_Service_t;

    /* Page navigation info of current showing page */
    typedef struct TTX_NAV_Info_t
    {
        TTX_TOP_Service_t      top;
        TTX_FLOF_Service_t     flof;
    } TTX_NAV_Info_t;

    /* Internal statistics */
    typedef struct TTX_Statistics_t
    {
        uint32_t      cntPacketsProcessed;  /* records number of teletext packets that have been processed */
//    uint32_t      cntPagesDecoded;      /* records number of teletext pages   that have been decoded.  */
    } TTX_Statistics_t;

    /* External status */
    typedef struct TTX_Status_t
    {
        TTX_State_t         state;
        TTX_Statistics_t    statistics;
    } TTX_Status_t;


    typedef struct TTX_SubtitlePageInfo_t
    {
        uint8_t bIntendToFind;

        uint8_t bFound;
        TTX_STD_Page_AddrWithSubCode_t addr;    /* Only valid if( bFound == TRUE ) */
    } TTX_SubtitlePageInfo_t;

    typedef struct TTX_UsrCfgPageAddr_t
    {
        uint8_t                        bIsSet;
        TTX_STD_Page_AddrWithSubCode_t pageAddr;
    } TTX_UsrCfgPageAddr_t;

    /* Stores user configurations */
    typedef struct TTX_UsrConfig_t
    {
        TTX_UsrCfgPageAddr_t            pageToShow;                  /* user wants to see this page */
        TTX_UsrCfgPageAddr_t            pageSubtitle;                /* user wants to see this page in subtitle mode */
        TTX_STD_Page_AddrWithSubCode_t  flofKeys[TTX_FLOF_KEY_NUM];  /* use TTX_FLOF_Key_t to index this array. */
    } TTX_UsrConfig_t;


    /* Info of pages */
    typedef struct TTX_PageInfo_t
    {
        TTX_STD_Page_AddrWithSubCode_t  addr;                 // the page is showing
        TTX_NAV_Info_t                  navInfo;              // page navigation info of pageShowing
    } TTX_PageInfo_t;

    typedef uint32_t TTX_Tick_t;

    /* private object, not to be used by application code */
    typedef struct
    {
        TTX_STD_Page_AddrWithSubCode_t pageInitial; // initial page

        TTX_PageInfo_t                  pageShowing;

        uint8_t                         bSubtitleAvailable;   // clone of vbiSys_cmdSubtitlePageFlag
        TTX_STD_Page_Addr_t             pageSubtitle;                // this value can be used only if( m_bSubtitleAvailable == TRUE )

        uint32_t                        evtsFired[TTX_EVTS_NUM];

        TTX_State_t                     state;
        TTX_Statistics_t                statistics;
        TTX_UsrConfig_t                 usrConfig;
        TTX_Cmd_t                       usrCmd;
        TTX_SubtitlePageInfo_t          infoSubtitle;

        uint8_t					 Correct_X0_cnt;

        uint8_t	pageMap[100];

    } TTX_Obj, *TTX_Handle;

    /* -- Function Prototypes ---------------------------------------------------*/
    void            TTX_OBJ_ResetAllMemberVariables ();

    /* -- Private Function Prototypes -------------------------------------------*/
    void            _TTX_STATE_onEntering_INITIAL   ();
    void            _TTX_STATE_onEntering_READY     ();
    void            _TTX_STATE_onEntering_NORMAL    ();
    void            _TTX_STATE_onEntering_SUBTITLE  ();



#ifdef __cplusplus
}   /* extern "C" */
#endif


#endif /* !__REALTEK_ENHANCED_TELETEXT_DECODER_LIBRARY_TELEXT_OBJECT_INTER_PACKAGE_INTERFACE_HEADER_INCLUDED__ */

/**
 * @}
 */

/*---------------------------------------------------------------------------*\
 * end of ttx_obj.h                                                          *
\*---------------------------------------------------------------------------*/
