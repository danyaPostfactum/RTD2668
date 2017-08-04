/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_evt.c
 * @brief    Teletext event processor.
 * @author   Janus Cheng
 * @date     Feb. 10, 2009
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/
#ifdef CONFIG_TTX_SUPPORT

#include <ttx_evt.h>        /* Provides: functions to be implemented */
#include <ttx.h>            /* Provides: functions to be implemented */
#include <ttx_assert.h>     /* Provides: TTX_DEFINE_THIS_MODULE() */
#include <ttx_log.h>        /* Provides: TTX_Log(LOGGER_VERBOSE, ) */
#include <ttx_source.h>     /* Provides: TTX_SRC_FetchRawPkt() */
#include <ttx_buff.h>       /* Provides: TTX_BUF_FindPage() */
#include <ttx_dec.h>        /* Provides: TTX_DecodePage() */
#include <ttx_disp.h>       /* Provides: TTX_DISP_PostRefresh() */
#include <ttx_nav.h>        /* Provides: TTX_NAV_GetNextPage() */
#include <ttx_dec_top.h>    /* Provides: TTX_TOP_Decode() */
#include <vbi/ppr.h>

#if (defined(CONFIG_PLATFORM_VIKING)||defined(CONFIG_PLATFORM_PACIFIC))
#include <ttx_bmposd.h>     /* Provides: TTX_BMPOSD_Show() */
#endif


#include <stdlib.h>         /* Provides: NULL */

#if defined(CONFIG_PLATFORM_VIKING)||defined(CONFIG_PLATFORM_PACIFIC)
#include <vbi/vbi.h>
#endif

TTX_DEFINE_THIS_MODULE( ttx_evt.c )


/* -- Prototypes of Private Functions ---------------------------------------*/
static bool_t   TTX_EVT_onCmdNormal                     ();
static bool_t   TTX_EVT_onCmdSubtitle                   ();
static bool_t   TTX_EVT_onCmdStart                      ();
static bool_t   TTX_EVT_onCmdPause                      ();
static bool_t   TTX_EVT_onCmdStop                       ();
static bool_t   TTX_EVT_onCmdShow                       ();
static bool_t   TTX_EVT_onCmdHide                       ();
static bool_t   TTX_EVT_onCmdNextPage                   ();
static bool_t   TTX_EVT_onCmdPrevPage                   ();
static bool_t   TTX_EVT_onCmdNextSubPage                ();
static bool_t   TTX_EVT_onCmdPrevSubPage                ();
static bool_t   TTX_EVT_onCmdGoToPage                   ( const TTX_STD_Page_AddrWithSubCode_t* pPageAddr);
static bool_t   TTX_EVT_onCmdGoToSubPage   	(const TTX_STD_Page_AddrWithSubCode_t* pPageAddr);

static bool_t   TTX_EVT_onCmdBtnRed                     ();
static bool_t   TTX_EVT_onCmdBtnGreen                   ();
static bool_t   TTX_EVT_onCmdBtnYellow                  ();
static bool_t   TTX_EVT_onCmdBtnCyan                    ();
static bool_t   TTX_EVT_onCmdSetSubtitlePageAddr        ( const TTX_STD_Page_AddrWithSubCode_t* pPageAddr);
static bool_t   TTX_EVT_onCmdSetPageNumIndicator        ( const TTX_STD_Page_AddrWithSubCode_t* pPageAddr);

void   TTX_EVT_onDecode                        ();
static bool_t   TTX_EVT_onDecodingBufferedPage          ( const TTX_STD_Page_AddrWithSubCode_t* pPageToDecode, uint8_t  pPageBuf );
static bool_t   TTX_EVT_onDecodedBufferedPage           ( const TTX_STD_Page_AddrWithSubCode_t* pPageDecoded, const bool_t changePageNum );
static void   TTX_EVT_onShowingPageChanged            (       TTX_STD_Page_AddrWithSubCode_t* pPageToShow );
static bool_t   TTX_EVT_onScreenCanBeRefreshed          ();

static void     TTX_EVT_Dispatch                        ();
static bool_t   TTX_EVT_DispatchBeforeEveryState        ();
static bool_t   TTX_EVT_DispatchAfterEveryState         ();
static bool_t   TTX_EVT_DispatchInInitialState          ();
static bool_t   TTX_EVT_DispatchInReadyState            ();
static bool_t   TTX_EVT_DispatchInNormalState           ();
static bool_t   TTX_EVT_DispatchInSubtitleState         ();
static bool_t   TTX_EVT_DispatchInNormalOrSubtitleState ();

static void     TTX_SET_Reveal(bool_t isOn);
static bool_t   TTX_EVT_onCmdRotate();
static bool_t   TTX_EVT_onCmdHold();
static bool_t TTX_EVT_onCmdRotateSubPage();
static bool_t TTX_SET_First_Subpage();
static bool_t TTX_SET_Last_Subpage();
static bool_t TTX_EVT_onCmdIndexPage();

bool_t user_reveal = FALSE; // User chooses to reveal the concealed content.
int ttx_user_cursor;
extern void * memset(void * s, int c, size_t count);

/* -- Implementations of Public Functions -----------------------------------*/

/*---------------------------------------------------------------------------*/
bool_t
TTX_Control     ( TTX_Cmd_t cmd)
/*****************************************************************************\
 *  @brief   Use THIS_FUNCTION to control TTX library.
\*****************************************************************************/
{
    bool_t bOk;

    if ( m_hTtx != NULL )
    {
        bOk = FALSE;

#if( TTX_SUPPORT_MT )
        TTX_MT_Lock( m_hTtx->hMutex );
#endif  /* end of #if( TTX_SUPPORT_MT ) */

        switch ( cmd )
        {
        case TTX_CMD_NORMAL:
        {
            bOk = TTX_EVT_onCmdNormal();
            break;
        }
        case TTX_CMD_SUBTITLE:
        {
            bOk = TTX_EVT_onCmdSubtitle();
            break;
        }
        case TTX_CMD_START:
        {
            bOk = TTX_EVT_onCmdStart();
            break;
        }
        case TTX_CMD_PAUSE:
        {
            bOk = TTX_EVT_onCmdPause();
            break;
        }
        case TTX_CMD_STOP:
        {
            bOk = TTX_EVT_onCmdStop();
            break;
        }
        case TTX_CMD_SHOW:
        {
            bOk = TTX_EVT_onCmdShow();
            break;
        }
        case TTX_CMD_HIDE:
        {
            bOk = TTX_EVT_onCmdHide();
            break;
        }
        case TTX_CMD_NEXT_PAGE:
        {
            bOk = TTX_EVT_onCmdNextPage();
            break;
        }
        case TTX_CMD_PREV_PAGE:
        {
            bOk = TTX_EVT_onCmdPrevPage();
            break;
        }
        case TTX_CMD_NEXT_SUBPAGE:
        {
            bOk = TTX_EVT_onCmdNextSubPage();
            break;
        }
        case TTX_CMD_PREV_SUBPAGE:
        {
            bOk = TTX_EVT_onCmdPrevSubPage();
            break;
        }
        case TTX_CMD_ROTATE_SUBPAGE:
        {
            bOk = TTX_EVT_onCmdRotateSubPage();
            break;
        }
        case TTX_CMD_INDEX_PAGE:
        {
            bOk = TTX_EVT_onCmdIndexPage();
            break;
        }
        case TTX_CMD_BTN_RED:
        {
            bOk = TTX_EVT_onCmdBtnRed();
            break;
        }
        case TTX_CMD_BTN_GREEN:
        {
            bOk = TTX_EVT_onCmdBtnGreen();
            break;
        }
        case TTX_CMD_BTN_YELLOW:
        {
            bOk = TTX_EVT_onCmdBtnYellow();
            break;
        }
        case TTX_CMD_BTN_CYAN:
        {
            bOk = TTX_EVT_onCmdBtnCyan();
            break;
        }
        case TTX_CMD_VBI_OUT_ON:
        {
            break;
        }
        case TTX_CMD_VBI_OUT_OFF:
        {
            break;
        }
        case TTX_CMD_REVEAL: // reveal the concealed characters (toggle between reveal and conceal)
        {
            user_reveal = !user_reveal;
            TTX_SET_Reveal(user_reveal);
            break;
        }
        case TTX_CMD_TEST:
        {
            TTX_BUF_Dump();
            break;
        }
        case TTX_CMD_HOLD: // display 'HOLD' icon; 'HOLD' behavior will do in AP layer
        {
            bOk = TTX_EVT_onCmdHold();
            break;
        }
        case TTX_CMD_ROTATE: // hide 'HOLD' icon (display page number); 'ROTATE' bahavior will do in AP layer
        {
            bOk = TTX_EVT_onCmdRotate();
            break;
        }
        default:
        {
            bOk = FALSE;
            break;
        }
        }


        if ( bOk )
        {
            m_hTtx->usrCmd = cmd;
        }

#if( TTX_SUPPORT_MT )
        TTX_MT_Unlock( m_hTtx->hMutex );
#endif  /* end of #if( TTX_SUPPORT_MT ) */

        return( bOk );
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_Control1     ( TTX_Cmd_t cmd, TTX_CmdArg_t* pArg)
/*****************************************************************************\
 *  @brief   Use THIS_FUNCTION to control TTX library.
\*****************************************************************************/
{
    bool_t bOk;

    if ( m_hTtx != NULL )
    {
        bOk = FALSE;

        switch ( cmd )
        {

        case TTX_CMD_GO_TO_PAGE:
        {
            TTX_REQUIRE( pArg != NULL );
            if ( pArg != NULL )
            {
                TTX_STD_Page_AddrWithSubCode_t pageAddr;

                pageAddr.pageA = (pArg->pageNum.magNum <<8) |
                                 (pArg->pageNum.pageNumTens<<4) |
                                 pArg->pageNum.pageNumUnits;
                pageAddr.subcodeA = 0;


                bOk = TTX_EVT_onCmdGoToPage(  &pageAddr );
            }
            break;
        }
        case TTX_CMD_GO_TO_SUBPAGE:
        {
            if ( pArg != NULL )
            {
                if (pArg->pageAddr.subcodeA > 0x79)
                    pArg->pageAddr.subcodeA = 0;

                RTD_Log(LOGGER_INFO, "TTX_CMD_GO_TO_SUBPAGE, PageA : %x, subpageA : %x \n", (UINT32) pArg->pageAddr.pageA, (UINT32) pArg->pageAddr.subcodeA);

                bOk = TTX_EVT_onCmdGoToSubPage(&pArg->pageAddr);
            }
            break;
        }
        case TTX_CMD_SET_SUBTITLE_PAGE_NUM:
        {
            TTX_REQUIRE( pArg != NULL );
            if ( pArg != NULL )
            {
                TTX_STD_Page_AddrWithSubCode_t pageAddr;

//                    TTX_REQUIRE( sizeof( pArg->pageNum.magNum ) >= sizeof( pageAddr.magNum ) ); // ERROR: the type of pArg->pageAddr.magNum cannot express full range of magazine number.

                pageAddr.pageA = (pArg->pageNum.magNum <<8) |
                                 (pArg->pageNum.pageNumTens<<4) |
                                 pArg->pageNum.pageNumUnits;
                pageAddr.subcodeA = 0;

                bOk = TTX_EVT_onCmdSetSubtitlePageAddr(  &pageAddr );
            }
            break;
        }
        case TTX_CMD_SET_PAGE_NUM_INDICATOR:
        {
            TTX_REQUIRE( pArg != NULL );

            if ( pArg != NULL )
            {
                TTX_STD_Page_AddrWithSubCode_t pageAddr;

                TTX_REQUIRE( sizeof( pArg->pageNum.magNum ) >= sizeof( pageAddr.magNum ) ); // ERROR: the type of pArg->pageAddr.magNum cannot express full range of magazine number.

                pageAddr.pageA = (pArg->pageNum.magNum<<8) |
                                 ((pArg->pageNum.pageNumTens&0x0F)<<4) |
                                 ((pArg->pageNum.pageNumUnits&0x0F)); //uint8 transfer to uint4, filter high 4bits
                pageAddr.subcodeA = 0;

                bOk = TTX_EVT_onCmdSetPageNumIndicator(  &pageAddr );

            }

            break;
        }
        case TTX_CMD_VBI_OUT_ON:
        {
            break;
        }
        case TTX_CMD_VBI_OUT_OFF:
        {
            break;
        }
        default:
        {
            bOk = FALSE;
            break;
        }
        }


        if ( bOk )
        {
            m_hTtx->usrCmd = cmd;
        }

#if( TTX_SUPPORT_MT )
        TTX_MT_Unlock( m_hTtx->hMutex );
#endif  /* end of #if( TTX_SUPPORT_MT ) */

        return( bOk );
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
void
TTX_Tick  ()
/*****************************************************************************\
 *  @brief   test programming errors
\*****************************************************************************/
{
    TTX_EVT_Dispatch();
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_WaitForCompletion       ()
/*****************************************************************************\
 *  @brief   A blocking call to wait for the previous TTX_Control() to be completed.
 *  @pre     1. Must be called immediately after TTX_Control().
 *  @retval  TRUE if success.
 *  @retval  FALSE if timeout or error.
\*****************************************************************************/
{
    uint8_t i, n;

    if ( m_hTtx == NULL )
        return( FALSE );

    /* find out number of iteration according to m_hTtx->usrCmd */
    switch ( m_hTtx->usrCmd )
    {
    case TTX_CMD_STOP:
    {
        n = 2;
        break;
    }
    default:
    {
        n = 1;
        break;
    }
    }

    for ( i=0; i<n; i++ )
    {
        TTX_Tick();
    }

    return( TRUE );
}


/*---------------------------------------------------------------------------*/
void
_TTX_STATE_onEntering_INITIAL       ()
/*****************************************************************************\
 *  @brief   A blocking call to be invoked before entering TTX_STATE_INITIAL.
 *  @pre     m_hTtx->state is the state transited from.
\*****************************************************************************/
{

}

/*---------------------------------------------------------------------------*/
void
_TTX_STATE_onEntering_READY       ()
/*****************************************************************************\
 *  @brief   A blocking call to be invoked before entering TTX_STATE_READY.
 *  @pre     m_hTtx->state is the state transited from.
\*****************************************************************************/
{
    TTX_STD_Page_AddrWithSubCode_t PageAddr;
    uint16_t Addr;

    if (drvif_PPR_GetInitialPage(&Addr))
    {
        PageAddr.pageA = Addr;
        PageAddr.subcodeA = 0;
        RTD_Log(LOGGER_INFO, "Read initial page (%x)\n", (UINT32)PageAddr.pageA);
    }
    else
    {
        // reset initial page to P100
        PageAddr.pageA = 0x100;
        PageAddr.subcodeA = 0;
        RTD_Log(LOGGER_INFO, "Reset initial page (%x)\n", (UINT32) (PageAddr.pageA));
    }

    TTX_SetInitPage(&PageAddr);

    /* Don't display when stopped. */
    TTX_EVT_Clear(  SYS, SHOW );

}

/*---------------------------------------------------------------------------*/
void
_TTX_STATE_onEntering_NORMAL       ()
/*****************************************************************************\
 *  @brief   A blocking call to be invoked before entering TTX_STATE_NORMAL.
 *  @pre     m_hTtx->state is the state transited from.
\*****************************************************************************/
{
    if ( m_hTtx == NULL )
        return ;

    /* Returns back to initial page */
    TTX_NAV_SetPageToShow(&m_hTtx->pageInitial);

    // Initialize PPR to accept data
    //  TTX_SRC_PrepareToAcceptNextField();

    TTX_Callback( TTX_CALLBACK_ON_ENTERED_NORMAL, NULL );    /* callback to platform-dependent implementations */

}

/*---------------------------------------------------------------------------*/
void
_TTX_STATE_onEntering_SUBTITLE       ()
/*****************************************************************************\
 *  @brief   A blocking call to be invoked before entering TTX_STATE_SUBTITLE.
 *  @pre     m_hTtx->state is the state transited from.
\*****************************************************************************/
{
    if ( m_hTtx == NULL )
        return;

    /* When entering subtitle, we should hide all display. This is because
         some streams contain no subtitle page, but contain normal P100. In this case,
         when we turn on subtitle mode in P100, we should hide all display.
     */
    TTX_DISP_Show( FALSE );

    // Initialize PPR to accept data
    TTX_SRC_PrepareToAcceptNextField();


    if ( m_hTtx->usrConfig.pageSubtitle.bIsSet )  /* if user has specified the subtitle page, use it */
    {
        /* Signaling TTX_SRC that we don't want to find the subtitle page */
        m_hTtx->infoSubtitle.bIntendToFind = FALSE;

        TTX_NAV_SetPageToShow(  &(m_hTtx->usrConfig.pageSubtitle.pageAddr) );
    }
    else    /* Get subtitle page address specified by broadcaster */
    {
        /* Signaling TTX_SRC that we want to find the subtitle page */
        m_hTtx->infoSubtitle.bIntendToFind = TRUE;

        TTX_Log( LOGGER_INFO, "[TTX] Finding subtitle page...\n");
    }

    TTX_Callback( TTX_CALLBACK_ON_ENTERED_SUBTITLE, NULL );    /* callback to platform-dependent implementations */


}

/* -- Implementations of Private Functions ----------------------------------*/

/*---------------------------------------------------------------------------*/
static void
TTX_EVT_Dispatch              ()
/*****************************************************************************\
 *  @brief   Dispatch fired events of m_hTtx to corresponding event handlers.
\*****************************************************************************/
{
    /* Stop dispatching events when paused */
    // if( ! TTX_EVT_isFired(  SYS, PAUSE ) )  // not use now in sparrow
    // {

    /* Dispatch state-independent events */
    // TTX_EVT_DispatchBeforeEveryState(); // not use now in sparrow

    /* Dispatch state-dependent events */
    if ( m_hTtx == NULL )
        return;

    /* Stop dispatching events when paused */
    if ( TTX_EVT_isFired(SYS, PAUSE ) )
        return;

    switch ( m_hTtx->state )
    {
    case TTX_STATE_NORMAL:
    {
        TTX_EVT_DispatchInNormalOrSubtitleState();
        TTX_EVT_DispatchInNormalState();
        break;
    }
    case TTX_STATE_SUBTITLE:
    {
        TTX_EVT_DispatchInNormalOrSubtitleState();
        TTX_EVT_DispatchInSubtitleState();
        break;
    }
    case TTX_STATE_INITIAL:
    {
        TTX_EVT_DispatchInInitialState();
        break;
    }
    case TTX_STATE_READY:
    {
        TTX_EVT_DispatchInReadyState();
        m_hTtx->pageShowing.addr.pageA = 0;
        break;
    }
    default:
    {
        TTX_ERROR();    /* ERROR: Unknown state */
        break;
    }
    }

    /* Dispatch state-independent events */
    TTX_EVT_DispatchAfterEveryState();

    // }
}

#if 0
/*---------------------------------------------------------------------------*/
static bool_t
TTX_EVT_DispatchBeforeEveryState  ()
/*****************************************************************************\
 *  @brief   Dispatch events that common in every state.
\*****************************************************************************/
{
    if ( m_hTtx != NULL )
    {
        return( TRUE );
    }   /* end of if( m_hTtx != NULL ) */

    return( FALSE );
}
#endif

/*---------------------------------------------------------------------------*/
static bool_t
TTX_EVT_DispatchAfterEveryState  ()
/*****************************************************************************\
 *  @brief   Dispatch events that common in every state.
\*****************************************************************************/
{
    if ( m_hTtx == NULL )
        return (FALSE);

    TTX_EVT_onScreenCanBeRefreshed();

    return( TRUE );
}


/*---------------------------------------------------------------------------*/
static bool_t
TTX_EVT_DispatchInInitialState  ()
/*****************************************************************************\
 *  @brief   event handler of TTX_NOTIFY_EVT_100MS
\*****************************************************************************/
{
    if ( m_hTtx == NULL )
        return (FALSE);

    TTX_REQUIRE( m_hTtx->state == TTX_STATE_INITIAL );

    TTX_Reset();

    TTX_STATE_TransitTo(  READY ); /* Transit to TTX_STATE_READY state */
    TTX_Log( LOGGER_INFO, "[TTX] state: INIT -> READY\n");

    return( TRUE );
}
/*---------------------------------------------------------------------------*/
static bool_t
TTX_EVT_DispatchInReadyState  ()
/*****************************************************************************\
 *  @brief   event handler of TTX_NOTIFY_EVT_100MS
\*****************************************************************************/
{
    if ( m_hTtx == NULL )
        return (FALSE);

    TTX_REQUIRE( m_hTtx->state == TTX_STATE_READY );


    if ( TTX_EVT_isFired(  SYS, STOP ) )
    {
        TTX_STATE_TransitTo(  INITIAL ); /* Transit to TTX_STATE_INITIAL state */
        TTX_Log(LOGGER_INFO, "[TTX] state: READY -> INIT\n");
        TTX_EVT_Clear(  SYS, STOP );   /* This event has been handled */
    }
    else if ( TTX_EVT_isFired(  SYS, NORMAL ) )
    {
        TTX_STATE_TransitTo(  NORMAL ); /* Transit to TTX_STATE_NORMAL state */
        TTX_Log(LOGGER_INFO, "[TTX] state: READY -> NORMAL\n");
        TTX_EVT_Clear(  SYS, NORMAL );   /* This event has been handled */
    }
    else if ( TTX_EVT_isFired(  SYS, SUBTITLE ) )
    {
        TTX_STATE_TransitTo(  SUBTITLE ); /* Transit to TTX_STATE_SUBTITLE state */
        TTX_Log(LOGGER_INFO, "[TTX] state: READY -> SUBTITLE\n");
        TTX_EVT_Clear(  SYS, SUBTITLE );   /* This event has been handled */
    }

    // reset showing page
    m_hTtx->pageShowing.addr.pageA = 0;

    return( TRUE );
}

/*---------------------------------------------------------------------------*/

static void TTX_EVT_TOPFastColor (UINT8 color_key)
/*****************************************************************************\
 * @brief		apply TOP fast color key
\*****************************************************************************/
{
    if ( m_hTtx == NULL )
        return;

    if ( color_key >= 4 )
        return;

    switch (color_key)
    {
    case 0: // red
        m_hTtx->usrConfig.pageToShow.pageAddr = m_hTtx->pageShowing.navInfo.top.flofKeys[TTX_FLOF_KEY_RED];
        m_hTtx->usrConfig.pageToShow.bIsSet   = TRUE; /* Set hTtx->usrConfig.pageToShow.pageAddr is valid */
        //RTD_Log(LOGGER_INFO, "TOP Red : %x \n", (UINT32) m_hTtx->usrConfig.pageToShow.pageAddr.pageA);
        TTX_EVT_Clear( SYS, FAST_CALL_RED );  /* this event has been processed. */
        break;
    case 1: // green
        m_hTtx->usrConfig.pageToShow.pageAddr = m_hTtx->pageShowing.navInfo.top.flofKeys[TTX_FLOF_KEY_GREEN];
        m_hTtx->usrConfig.pageToShow.bIsSet   = TRUE; /* Set hTtx->usrConfig.pageToShow.pageAddr is valid */
        //RTD_Log(LOGGER_INFO, "TOP Green : %x \n", (UINT32) m_hTtx->usrConfig.pageToShow.pageAddr.pageA);
        TTX_EVT_Clear( SYS, FAST_CALL_GREEN );  /* this event has been processed. */
        break;
    case 2: // yellow
        m_hTtx->usrConfig.pageToShow.pageAddr = m_hTtx->pageShowing.navInfo.top.flofKeys[TTX_FLOF_KEY_YELLOW];
        m_hTtx->usrConfig.pageToShow.bIsSet   = TRUE; /* Set hTtx->usrConfig.pageToShow.pageAddr is valid */
        //RTD_Log(LOGGER_INFO, "TOP Yellow : %x \n", (UINT32) m_hTtx->usrConfig.pageToShow.pageAddr.pageA);
        break;
    case 3: // blue
        m_hTtx->usrConfig.pageToShow.pageAddr = m_hTtx->pageShowing.navInfo.top.flofKeys[TTX_FLOF_KEY_CYAN];
        m_hTtx->usrConfig.pageToShow.bIsSet = TRUE; /* Set hTtx->usrConfig.pageToShow.pageAddr is valid */
        //RTD_Log(LOGGER_INFO, "TOP Cyan : %x \n", (UINT32) m_hTtx->usrConfig.pageToShow.pageAddr.pageA);
        break;
    }
}

/*---------------------------------------------------------------------------*/
static void TTX_EVT_FLOFFastColor (UINT8 color_key)
/*****************************************************************************\
 * @brief		apply FLOF fast color key
\*****************************************************************************/
{
    if ( m_hTtx == NULL )
        return;

    if ( color_key >= 4 )
        return;

    switch (color_key)
    {
    case 0: // red
        if ((m_hTtx->pageShowing.navInfo.flof.flofKeys[TTX_FLOF_KEY_RED].pageA&0xFF)!=0xFF)
        {
            m_hTtx->usrConfig.pageToShow.pageAddr = m_hTtx->pageShowing.navInfo.flof.flofKeys[TTX_FLOF_KEY_RED];
            m_hTtx->usrConfig.pageToShow.bIsSet = TRUE; /* Set hTtx->usrConfig.pageToShow.pageAddr is valid */
        }
        TTX_EVT_Clear( SYS, FAST_CALL_RED );  /* this event has been processed. */
        break;
    case 1: // green
        if ((m_hTtx->pageShowing.navInfo.flof.flofKeys[TTX_FLOF_KEY_GREEN].pageA&0xFF)!=0xFF)
        {
            m_hTtx->usrConfig.pageToShow.pageAddr = m_hTtx->pageShowing.navInfo.flof.flofKeys[TTX_FLOF_KEY_GREEN];
            m_hTtx->usrConfig.pageToShow.bIsSet   = TRUE; /* Set hTtx->usrConfig.pageToShow.pageAddr is valid */
        }
        break;
    case 2: // yellow
        if ((m_hTtx->pageShowing.navInfo.flof.flofKeys[TTX_FLOF_KEY_YELLOW].pageA&0xFF)!=0xFF)
        {
            m_hTtx->usrConfig.pageToShow.pageAddr = m_hTtx->pageShowing.navInfo.flof.flofKeys[TTX_FLOF_KEY_YELLOW];
            m_hTtx->usrConfig.pageToShow.bIsSet   = TRUE; /* Set hTtx->usrConfig.pageToShow.pageAddr is valid */
        }
        break;
    case 3: // blue
        if ((m_hTtx->pageShowing.navInfo.flof.flofKeys[TTX_FLOF_KEY_CYAN].pageA&0xFF)!=0xFF)
        {
            m_hTtx->usrConfig.pageToShow.pageAddr = m_hTtx->pageShowing.navInfo.flof.flofKeys[TTX_FLOF_KEY_CYAN];
            m_hTtx->usrConfig.pageToShow.bIsSet   = TRUE; /* Set hTtx->usrConfig.pageToShow.pageAddr is valid */
        }
        TTX_EVT_Clear( SYS, FAST_CALL_CYAN );  /* this event has been processed. */
        break;
    }
}


static bool_t
TTX_EVT_DispatchInNormalState  ()
/*****************************************************************************\
 *  @brief   event handler of TTX_NOTIFY_EVT_100MS
\*****************************************************************************/
{
    if ( m_hTtx == NULL )
        return FALSE;

    TTX_REQUIRE( m_hTtx->state == TTX_STATE_NORMAL );


    if ( TTX_EVT_isFired(  SYS, SUBTITLE ) )
    {
        TTX_STATE_TransitTo(  SUBTITLE );      /* Transit to TTX_STATE_SUBTITLE state */
        TTX_Log(LOGGER_INFO, "[TTX] state: NORMAL -> SUBTITLE\n");
        TTX_EVT_Clear(  SYS, SUBTITLE );
    }
    else if ( TTX_EVT_isFired(  SYS, NEXT_PAGE ) )
    {
        if ( m_hTtx->usrConfig.pageToShow.bIsSet ) /* if m_hTtx->usrConfig.pageToShow.pageAddr is valid */
        {
            if ( TTX_NAV_GetNextPage(  &(m_hTtx->usrConfig.pageToShow.pageAddr) ) )
            {
                TTX_EVT_Clear(  SYS, NEXT_PAGE );  /* this event has been processed. */
            }
        }
    }
    else if ( TTX_EVT_isFired(  SYS, PREV_PAGE ) )
    {
        if ( m_hTtx->usrConfig.pageToShow.bIsSet ) /* if m_hTtx->usrConfig.pageToShow.pageAddr is valid */
        {
            if ( TTX_NAV_GetPrevPage(  &(m_hTtx->usrConfig.pageToShow.pageAddr) ) )
            {
                TTX_EVT_Clear(  SYS, PREV_PAGE );  /* this event has been processed. */
            }
        }
    }
    else if ( TTX_EVT_isFired(  SYS, NEXT_SUBPAGE ) )
    {
        if ( m_hTtx->usrConfig.pageToShow.bIsSet ) /* if m_hTtx->usrConfig.pageToShow.pageAddr is valid */
        {

            /* Don't check the return of TTX_NAV_GetNextSubPage() here.
             *   Because when we already at the the last sub-page,
             *   TTX_NAV_GetNextSubPage() will return FALSE.
             */
            if (TTX_NAV_GetNextSubPage(  &(m_hTtx->usrConfig.pageToShow.pageAddr) ))
                TTX_EVT_Fire(DISP, UPDATE_NAV_CTRL);

            TTX_EVT_Clear(  SYS, NEXT_SUBPAGE );  /* this event has been processed. */
        }

    }
    else if ( TTX_EVT_isFired(  SYS, PREV_SUBPAGE ) )
    {
        if ( m_hTtx->usrConfig.pageToShow.bIsSet ) /* if m_hTtx->usrConfig.pageToShow.pageAddr is valid */
        {

            /* Don't check the return of TTX_NAV_GetPrevSubPage() here.
             *   Because when we already at the the first sub-page,
             *   TTX_NAV_GetPrevSubPage() will return FALSE.
             */
            if (TTX_NAV_GetPrevSubPage(  &(m_hTtx->usrConfig.pageToShow.pageAddr) ))
                TTX_EVT_Fire(DISP, UPDATE_NAV_CTRL);

            TTX_EVT_Clear(  SYS, PREV_SUBPAGE );  /* this event has been processed. */
        }
    }
    else if ( TTX_EVT_isFired(SYS, ROTATE_SUBPAGE ) )
    {
        if ( m_hTtx->usrConfig.pageToShow.bIsSet ) /* if hTtx->usrConfig.pageToShow.pageAddr is valid */
        {
            TTX_NAV_GetNextSubPage(  &(m_hTtx->usrConfig.pageToShow.pageAddr));

            /* Don't check the return of TTX_NAV_GetNextSubPage2() here.
            *   Because when we already at the the last sub-page,
            *   TTX_NAV_GetNextSubPage2() will return FALSE.
            */
            TTX_EVT_Clear( SYS, ROTATE_SUBPAGE );  /* this event has been processed. */
        }
    }

    else if ( TTX_EVT_isFired(  SYS, FAST_CALL_RED ) )
    {
#if defined(CONFIG_TTX_TOP_TOSHIBA) // TOP > FLOF
        if ( m_hTtx->pageShowing.navInfo.top.bAvailable )
        {
            TTX_EVT_TOPFastColor(0);
        }
        else if ( m_hTtx->pageShowing.navInfo.flof.bAvailable )
        {
            TTX_EVT_FLOFFastColor(0);
        }
#else // FLOF > TOP
        if ( m_hTtx->pageShowing.navInfo.flof.bAvailable )
        {
            TTX_EVT_FLOFFastColor(0);
        }
        else if ( m_hTtx->pageShowing.navInfo.top.bAvailable )
        {
            TTX_EVT_TOPFastColor(0);
        }
#endif
        else
        {
            if ( m_hTtx->usrConfig.pageToShow.bIsSet ) /* if m_hTtx->usrConfig.pageToShow.pageAddr is valid */
            {
                if ( TTX_NAV_GetPrevPage(  &(m_hTtx->usrConfig.pageToShow.pageAddr) ) )
                {
                    TTX_EVT_Clear(  SYS, FAST_CALL_RED );  /* this event has been processed. */
                }
            }
        }
    }
    else if ( TTX_EVT_isFired(  SYS, FAST_CALL_GREEN ) )
    {
#if defined(CONFIG_TTX_TOP_TOSHIBA) // TOP > FLOF
        if ( m_hTtx->pageShowing.navInfo.top.bAvailable )
        {
            TTX_EVT_TOPFastColor(1);
        }
        else if ( m_hTtx->pageShowing.navInfo.flof.bAvailable )
        {
            TTX_EVT_FLOFFastColor(1);
            TTX_EVT_Clear(  SYS, FAST_CALL_GREEN );  /* this event has been processed. */
        }
#else // FLOF > TOP
        if ( m_hTtx->pageShowing.navInfo.flof.bAvailable )
        {
            TTX_EVT_FLOFFastColor(1);
            TTX_EVT_Clear(  SYS, FAST_CALL_GREEN );  /* this event has been processed. */
        }
        else if ( m_hTtx->pageShowing.navInfo.top.bAvailable )
        {
            TTX_EVT_TOPFastColor(1);
        }
#endif
        else
        {
            if ( m_hTtx->usrConfig.pageToShow.bIsSet ) /* if hTtx->usrConfig.pageToShow.pageAddr is valid */
            {
                if ( TTX_NAV_GetNextPage( &(m_hTtx->usrConfig.pageToShow.pageAddr) ) )
                {
                    TTX_EVT_Clear( SYS, FAST_CALL_GREEN );  /* this event has been processed. */
                }
            }
        }
    }
    else if ( TTX_EVT_isFired(  SYS, FAST_CALL_YELLOW ) )
    {
#if defined(CONFIG_TTX_TOP_TOSHIBA) // TOP > FLOF
        if ( m_hTtx->pageShowing.navInfo.top.bAvailable )
        {
            TTX_EVT_TOPFastColor(2);
        }
        else if ( m_hTtx->pageShowing.navInfo.flof.bAvailable )
        {
            TTX_EVT_FLOFFastColor(2);
        }
#else // FLOF > TOP
        if ( m_hTtx->pageShowing.navInfo.flof.bAvailable )
        {
            TTX_EVT_FLOFFastColor(2);
        }
        else if ( m_hTtx->pageShowing.navInfo.top.bAvailable )
        {
            TTX_EVT_TOPFastColor(2);
        }
#endif

        TTX_EVT_Clear(  SYS, FAST_CALL_YELLOW );  /* this event has been processed. */
    }
    else if ( TTX_EVT_isFired(  SYS, FAST_CALL_CYAN ) )
    {
#if defined(CONFIG_TTX_TOP_TOSHIBA) // TOP > FLOF
        if ( m_hTtx->pageShowing.navInfo.top.bAvailable )
        {
            TTX_EVT_TOPFastColor(3);
        }
        else if ( m_hTtx->pageShowing.navInfo.flof.bAvailable )
        {
            TTX_EVT_FLOFFastColor(3);
        }
#else // FLOF > TOP
        if ( m_hTtx->pageShowing.navInfo.flof.bAvailable )
        {
            TTX_EVT_FLOFFastColor(3);
        }
        else if ( m_hTtx->pageShowing.navInfo.top.bAvailable )
        {
            TTX_EVT_TOPFastColor(3);
        }
#endif
        TTX_EVT_Clear( SYS, FAST_CALL_CYAN );  /* this event has been processed. */
    }
    else if ( TTX_EVT_isFired(  SYS, STOP ) )
    {
        TTX_Reset();  /* Reset all stored data of TTX */

        TTX_STATE_TransitTo(  READY ); /* Transit to TTX_STATE_READY state */
        TTX_Log(LOGGER_INFO, "[TTX] state: NORMAL -> READY\n");
        TTX_Callback( TTX_CALLBACK_ON_STOPPED, NULL );    /* callback to platform-dependent implementations */
        TTX_EVT_Clear(  SYS, STOP );   /* This event has been handled */
    }

    return( TRUE );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_EVT_DispatchInSubtitleState  ()
/*****************************************************************************\
 *  @brief   event handler of TTX_NOTIFY_EVT_100MS
\*****************************************************************************/
{
    if ( m_hTtx == NULL )
        return (FALSE);

    TTX_REQUIRE( m_hTtx->state == TTX_STATE_SUBTITLE );

    {
        if ( m_hTtx->infoSubtitle.bIntendToFind )
        {
            if ( m_hTtx->infoSubtitle.bFound )
            {
                TTX_NAV_SetPageToShow(  &(m_hTtx->infoSubtitle.addr) );
                m_hTtx->infoSubtitle.bIntendToFind = FALSE;

                // Print debug log
//                    {
                //                     const TTX_STD_Page_AddrWithSubCode_Hex_t pageAddrHex =
                //                      m_hTtx->infoSubtitle.addr;
                //               TTX_Log(LOGGER_INFO, "[TTX] " "Found subtitle P%X.%X\n", (pageAddrHex>>16), (pageAddrHex & 0x0000FFFF) );
                //        }
            }
        }
    }



    {
        if ( TTX_EVT_isFired(  SYS, NORMAL ) )
        {
            TTX_STATE_TransitTo(  NORMAL );      /* Transit to TTX_STATE_NORMAL state */
            TTX_Log(LOGGER_INFO, "[TTX] state: SUBTITLE -> NORMAL\n");
            TTX_EVT_Clear(  SYS, NORMAL ); /* This event has been handled */
        }
        else if ( TTX_EVT_isFired(  SYS, STOP ) )
        {
            TTX_Reset();  /* Reset all stored data of TTX */

            TTX_STATE_TransitTo(  READY ); /* Transit to TTX_STATE_READY state */
            TTX_Log(LOGGER_INFO, "[TTX] state: SUBTITLE -> READY\n");
            TTX_Callback( TTX_CALLBACK_ON_STOPPED, NULL );    /* callback to platform-dependent implementations */
            TTX_EVT_Clear(  SYS, STOP );   /* This event has been handled */
        }
    }

    return( TRUE );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_EVT_DispatchInNormalOrSubtitleState ( )
/*****************************************************************************\
 *  @brief   Dispatch events that common in in TTX_STATE_NORMAL or TTX_STATE_SUBTITLE.
 *  @note    There shouldn't occur any state transition in this helper function.
 *           State transition should be implemented in TTX_EVT_DispatchInNormalState() or
 *             TTX_EVT_DispatchInSubtitleState().
 *  @pre     m_hTtx->state == TTX_STATE_NORMAL or TTX_STATE_SUBTITLE.
\*****************************************************************************/
{
    if ( m_hTtx == NULL )
        return( FALSE );

//        const TTX_StateValues_t origState = m_hTtx->state;

    // TTX_Log(LOGGER_INFO, "[TTX] state: TTX_EVT_onDecode\n");
    TTX_EVT_onDecode();

//        TTX_ENSURE( origState == m_hTtx->state ); /* ERROR: State transition occurs within this __FUNCTION__. */

    return( TRUE );

}


/*---------------------------------------------------------------------------*/
static bool_t
TTX_EVT_onCmdNormal()
/*****************************************************************************\
 *  @brief   event handler of TTX_CMD_SUBTITLE_OFF
\*****************************************************************************/
{
    if ( m_hTtx == NULL )
        return( FALSE );

    TTX_EVT_Fire(  SYS, NORMAL );

    //Reset Reveal
    user_reveal = FALSE;
    TTX_SET_Reveal(user_reveal);
    ttx_user_cursor = 0;
    memset(&ttx_select_subpage, 0, sizeof(ttx_select_subpage)); // exit selecting subpage mode

    return( TRUE );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_EVT_onCmdSubtitle   ()
/*****************************************************************************\
 *  @brief   event handler of TTX_CMD_SUBTITLE_ON
\*****************************************************************************/
{
    if ( m_hTtx == NULL )
        return( FALSE );

    TTX_EVT_Fire(  SYS, SUBTITLE );

    //Reset Reveal
    user_reveal = FALSE;
    TTX_SET_Reveal(user_reveal);
    ttx_user_cursor = 0;
    memset(&ttx_select_subpage, 0, sizeof(ttx_select_subpage)); // exit selecting subpage mode

    return( TRUE );

}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_EVT_onCmdStart ()
/*****************************************************************************\
 *  @brief   Recover from paused state.
\*****************************************************************************/
{
    if ( m_hTtx != NULL )
        return (FALSE);

    TTX_EVT_Clear(  SYS, PAUSE );
    return( TRUE );

}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_EVT_onCmdPause ()
/*****************************************************************************\
 *  @brief   event handler of TTX_NOTIFY_EVT_100MS
\*****************************************************************************/
{
    if ( m_hTtx == NULL )
        return( FALSE );

    TTX_EVT_Fire(  SYS, PAUSE );

    return( TRUE );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_EVT_onCmdStop ()
/*****************************************************************************\
 *  @brief   event handler of TTX_NOTIFY_EVT_100MS
\*****************************************************************************/
{
    if ( m_hTtx == NULL )
        return (FALSE);

    TTX_EVT_Fire(  SYS, STOP );
    return( TRUE );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_EVT_onCmdShow ()
/*****************************************************************************\
 *  @brief   event handler of TTX_NOTIFY_EVT_100MS
\*****************************************************************************/
{
    if ( m_hTtx == NULL )
        return( FALSE );

    TTX_EVT_Fire(  SYS, SHOW );
    return( TRUE );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_EVT_onCmdHide ()
/*****************************************************************************\
 *  @brief   event handler of TTX_NOTIFY_EVT_100MS
\*****************************************************************************/
{
    if ( m_hTtx == NULL )
        return( FALSE );
    TTX_EVT_Clear(  SYS, SHOW );
    return( TRUE );

}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_EVT_onCmdNextPage   ()
/*****************************************************************************\
 *  @brief   event handler of TTX_NOTIFY_EVT_100MS
\*****************************************************************************/
{
    if ( m_hTtx == NULL )
        return( FALSE );
    if ( m_hTtx->state != TTX_STATE_NORMAL )  /* this command is only valid in TTX_STATE_NORMAL */
        return( FALSE );
    TTX_EVT_Fire(  SYS, NEXT_PAGE );

    //Reset Reveal
    user_reveal = FALSE;
    TTX_SET_Reveal(user_reveal);
    ttx_user_cursor = 0;
    return( TRUE );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_EVT_onCmdPrevPage   ()
/*****************************************************************************\
 *  @brief   event handler of TTX_NOTIFY_EVT_100MS
\*****************************************************************************/
{
    if ( m_hTtx == NULL )
        return( FALSE );
    if ( m_hTtx->state != TTX_STATE_NORMAL )  /* this command is only valid in TTX_STATE_NORMAL */
        return( FALSE );

    TTX_EVT_Fire(  SYS, PREV_PAGE );

    //Reset Reveal
    user_reveal = FALSE;
    TTX_SET_Reveal(user_reveal);
    ttx_user_cursor = 0;
    return( TRUE );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_EVT_onCmdNextSubPage   ()
/*****************************************************************************\
 *  @brief   event handler of TTX_NOTIFY_EVT_100MS
\*****************************************************************************/
{
    if ( m_hTtx == NULL )
        return( FALSE );
    if ( m_hTtx->state != TTX_STATE_NORMAL )  /* this command is only valid in TTX_STATE_NORMAL */
        return( FALSE );

    //Reset Reveal
    user_reveal = FALSE;
    TTX_SET_Reveal(user_reveal);

    if (ttx_user_cursor ==0)
    {
        //when the auto rotate is enable, you can't make sure which the next page you got.
        //Therefore we need to try to get first page in current list.
        //(That's why we need to do "TTX_BUF_Sync_Cursor_Page" in the later)
        if (!TTX_SET_First_Subpage())
            return FALSE;
        ttx_subpage_auto_rotation.rotate = FALSE;
    }
    else
        TTX_EVT_Fire(  SYS, NEXT_SUBPAGE );

    ttx_user_cursor++;

    RTD_Log(LOGGER_INFO, "TTX_EVT_onCmdNextSubPage : user_cursor = %x \n", (UINT32) ttx_user_cursor);
    return( TRUE );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_EVT_onCmdPrevSubPage   ()
/*****************************************************************************\
 *  @brief   event handler of TTX_NOTIFY_EVT_100MS
\*****************************************************************************/
{
    if ( m_hTtx == NULL )
        return( FALSE );
    if ( m_hTtx->state != TTX_STATE_NORMAL )  /* this command is only valid in TTX_STATE_NORMAL */
        return( FALSE );

    //Reset Reveal
    user_reveal = FALSE;
    TTX_SET_Reveal(user_reveal);

    if (ttx_user_cursor ==0)
    {
        if (!TTX_SET_Last_Subpage())
            return FALSE;
        //when the auto rotate is enable, you can't make sure which the prev page you got.
        //Therefore we need to try to get last page in current list.
        //(That's why we need to do "TTX_BUF_Sync_Cursor_Page" in the later)
        ttx_subpage_auto_rotation.rotate = FALSE;
    }
    else
        TTX_EVT_Fire(  SYS, PREV_SUBPAGE );

    ttx_user_cursor--;

    RTD_Log(LOGGER_INFO, "TTX_EVT_onCmdPrevSubPage : user_cursor = %x \n", (UINT32) ttx_user_cursor);
    return( TRUE );
}

static bool_t TTX_EVT_onCmdRotateSubPage ()
/*****************************************************************************\
 * @brief		rotate to next subpage automatically
 * @param	<hTtx>	teletext handler
 * @return	SUCCESS or FAIL
\*****************************************************************************/
{
    if ( m_hTtx == NULL )
        return( FALSE );

    if ( m_hTtx->state != TTX_STATE_NORMAL )   /* this command is only valid in TTX_STATE_NORMAL */
        return( FALSE );

    TTX_EVT_Fire( SYS, ROTATE_SUBPAGE );

    //Reset Reveal
    user_reveal = FALSE;
    TTX_SET_Reveal(user_reveal);


    return( TRUE );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_EVT_onCmdGoToPage   (
    const TTX_STD_Page_AddrWithSubCode_t* pPageAddr)
/*****************************************************************************\
 *  @brief   event handler of TTX_NOTIFY_EVT_100MS
\*****************************************************************************/
{
    if ( m_hTtx == NULL )
        return( FALSE );

    if ( m_hTtx->state != TTX_STATE_NORMAL )  /* this command is only valid in TTX_STATE_NORMAL */
        return( FALSE );

    if ( pPageAddr == NULL )
        return( FALSE );

    TTX_NAV_SetPageToShow(  pPageAddr );

    //Reset Reveal
    user_reveal = FALSE;
    TTX_SET_Reveal(user_reveal);
    ttx_user_cursor = 0;
    return( TRUE );
}


/*---------------------------------------------------------------------------*/
static bool_t
TTX_EVT_onCmdGoToSubPage   (
    const TTX_STD_Page_AddrWithSubCode_t* pPageAddr)
/*****************************************************************************\
 *  @brief   event handler of TTX_NOTIFY_EVT_100MS
\*****************************************************************************/
{
    uint16_t val = 0;
    bool_t result = TRUE;

    if ( m_hTtx == NULL )
        return( FALSE );

    if ( m_hTtx->state != TTX_STATE_NORMAL )  /* this command is only valid in TTX_STATE_NORMAL */
        return( FALSE );

    if ( pPageAddr == NULL )
        return( FALSE );

    val = TTX_BUF_Get_Index_By_Subcode(pPageAddr->subcodeA);

    if (val == 0xFF)
    {
        ttx_user_cursor = 0;
        pPageAddr->subcodeA = 0x0;
        result = FALSE;
    }
    else
        ttx_user_cursor = val+1;

    TTX_NAV_SetPageToShow(  pPageAddr );

    //Reset Reveal
    user_reveal = FALSE;
    TTX_SET_Reveal(user_reveal);


    return( result );
}

static bool_t TTX_EVT_onCmdIndexPage()
{
    TTX_STD_Page_AddrWithSubCode_t pageAddr;

    if ( m_hTtx == NULL )
        return( FALSE );

    if ( m_hTtx->state != TTX_STATE_NORMAL )  /* this command is only valid in TTX_STATE_NORMAL */
        return( FALSE );

    if ((m_hTtx->pageShowing.navInfo.flof.bAvailable == TRUE) &&
            ((m_hTtx->pageShowing.navInfo.flof.flofKeys[TTX_FLOF_KEY_INDEX].pageA&0xFF)!=0xFF))
    {
        pageAddr = m_hTtx->pageShowing.navInfo.flof.flofKeys[TTX_FLOF_KEY_INDEX];
        RTD_Log(LOGGER_INFO, "TTX_EVT_onCmdIndexPage : Get Index Page ( %x ) \n", (UINT32) pageAddr.pageA);
    }
    else
    {
        TTX_GetInitPage(&pageAddr);
        RTD_Log(LOGGER_INFO, "TTX_EVT_onCmdIndexPage : Can't Get Index Page, Get Initial Page ( %x ) \n", (UINT32) pageAddr.pageA);
    }
    TTX_NAV_SetPageToShow(  &pageAddr );

    //Reset Reveal
    user_reveal = FALSE;
    TTX_SET_Reveal(user_reveal);
    ttx_user_cursor = 0;
    return( TRUE );

}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_EVT_onCmdBtnRed   ()
/*****************************************************************************\
 *  @brief   event handler of TTX_NOTIFY_EVT_100MS
\*****************************************************************************/
{
    if ( m_hTtx != NULL )
    {
        if ( m_hTtx->state == TTX_STATE_NORMAL )  /* this command is only valid in TTX_STATE_NORMAL */
        {
            TTX_EVT_Fire(  SYS, FAST_CALL_RED );

            //Reset Reveal
            user_reveal = FALSE;
            TTX_SET_Reveal(user_reveal);
            ttx_user_cursor = 0;
            return( TRUE );
        }
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_EVT_onCmdBtnGreen   ()
/*****************************************************************************\
 *  @brief   event handler of TTX_NOTIFY_EVT_100MS
\*****************************************************************************/
{
    if ( m_hTtx != NULL )
    {
        if ( m_hTtx->state == TTX_STATE_NORMAL )  /* this command is only valid in TTX_STATE_NORMAL */
        {
            TTX_EVT_Fire(  SYS, FAST_CALL_GREEN );

            //Reset Reveal
            user_reveal = FALSE;
            TTX_SET_Reveal(user_reveal);
            ttx_user_cursor = 0;
            return( TRUE );
        }
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_EVT_onCmdBtnYellow   ()
/*****************************************************************************\
 *  @brief   event handler of TTX_NOTIFY_EVT_100MS
\*****************************************************************************/
{
    if ( m_hTtx != NULL )
    {
        if ( m_hTtx->state == TTX_STATE_NORMAL )  /* this command is only valid in TTX_STATE_NORMAL */
        {
            TTX_EVT_Fire(  SYS, FAST_CALL_YELLOW );

            //Reset Reveal
            user_reveal = FALSE;
            TTX_SET_Reveal(user_reveal);
            ttx_user_cursor = 0;
            return( TRUE );
        }
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_EVT_onCmdBtnCyan   ()
/*****************************************************************************\
 *  @brief   event handler of TTX_NOTIFY_EVT_100MS
\*****************************************************************************/
{
    if ( m_hTtx != NULL )
    {
        if ( m_hTtx->state == TTX_STATE_NORMAL )  /* this command is only valid in TTX_STATE_NORMAL */
        {
            TTX_EVT_Fire(  SYS, FAST_CALL_CYAN );

            //Reset Reveal
            user_reveal = FALSE;
            TTX_SET_Reveal(user_reveal);
            ttx_user_cursor = 0;
            return( TRUE );
        }
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_EVT_onCmdSetSubtitlePageAddr   (
    const TTX_STD_Page_AddrWithSubCode_t* pPageAddr)
/*****************************************************************************\
 *  @brief   event handler of TTX_CMD_SET_SUBTITLE_PAGE_NUM
\*****************************************************************************/
{
    if ( m_hTtx != NULL )
    {
        if ( pPageAddr != NULL )
        {
            m_hTtx->usrConfig.pageSubtitle.pageAddr = *pPageAddr;
            m_hTtx->usrConfig.pageToShow.bIsSet     = TRUE;

            return( TRUE );
        }
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_EVT_onCmdSetPageNumIndicator   (
    const TTX_STD_Page_AddrWithSubCode_t* pPageAddr)
/*****************************************************************************\
 *  @brief   event handler of TTX_CMD_SET_PAGE_NUM_INDICATOR
\*****************************************************************************/
{
    if ( pPageAddr != NULL )
    {
        TTX_DISP_PageNumAttr_t PageNumAttr;
        uint8_t mag = (pPageAddr->pageA)>>8;
        uint8_t page_tens = ((pPageAddr->pageA&0xF0)>>4);
        uint8_t page_units = (pPageAddr->pageA&0x0F);

        if ( page_units >= 0xA ) // input number by remote control will not >= 0xA
            PageNumAttr.bEnablePageUnits = FALSE;  // display '-'
        else
            PageNumAttr.bEnablePageUnits = TRUE; // display page units
        if ( page_tens >= 0xA ) // input number by remote control will not >= 0xA
            PageNumAttr.bEnablePageTens = FALSE; // display '-'
        else
            PageNumAttr.bEnablePageTens = TRUE; // display page tens
        if ( mag >= 0xA )   // input number by remote control will not >= 0xA
        {
            PageNumAttr.bEnableMag = FALSE;
            mag = '-'; // display '-'
        }
        else if ( (mag == 0x0) || (mag == 0x9) )
        {
            PageNumAttr.bEnableMag = FALSE;
            mag = '?'; // display '?'
        }
        else
        {
            PageNumAttr.bEnableMag = TRUE; // display magazine
        }
        PageNumAttr.bEnableP = TRUE; // display 'P'

        PageNumAttr.ForegroundColor = TTX_COLOR_WHITE; // white foreground color
        PageNumAttr.ValidBackgroundColor = TTX_COLOR_BLACK; // black background color for valid magazine
        PageNumAttr.InvalidBackgroundColor = TTX_COLOR_BLACK; // black background color for invalid magazine

        if ((mag == '-')||(mag=='?'))
        {
            pPageAddr->pageA = ((pPageAddr->pageA&0x00FF) | (mag<<8));
        }

        TTX_DISP_SetPageNum( pPageAddr, PageNumAttr );

        return( TRUE );
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
void
TTX_EVT_onDecode   ()
/*****************************************************************************\
 *  @brief   This event will be triggered whenever a complete page has been
 *             received.
 *  @param   *pPageAddr is the page address of received page.
 *  @note    Mimic vbiSys_SetTtPageNum().
\*****************************************************************************/
{
    static uint8_t flash_count = 0;

    TTX_STD_Page_AddrWithSubCode_t* pPageToDecode;

    if ( !(m_hTtx->usrConfig.pageToShow.bIsSet))
        return;

    /* if m_hTtx->usrConfig.pageToShow.pageAddr is valid */
    pPageToDecode =  &( m_hTtx->usrConfig.pageToShow.pageAddr );

//		if (pPageToDecode->pageA == 0x101)
//			return TRUE;

    if ( !(TTX_STD_Page_AddrIsValid( pPageToDecode ) ))
        return;

    if ( !(TTX_STD_PageAddrSubCodeIsValid( pPageToDecode->subcodeA) ))
    {
        TTX_BUF_ResetSubPageList();
    }

    /* Move UPDATE_NAV_CTRL in front of page decode which is a work around of the screen broken
        of subpage auto-rotate (Mantis 25074)*/
    if ( TTX_EVT_isFired(  DISP, UPDATE_NAV_CTRL ) )
    {
        if ( m_hTtx->state == TTX_STATE_NORMAL )    /* SubPage nav control is only renderned in TTX_STATE_NORMAL */
        {
            const TTX_STD_Page_AddrWithSubCode_t* pPageShowing = &(m_hTtx->pageShowing.addr);

            TTX_DISP_SubPageNavCtrl_t subPageNavCtrl;

            if (TTX_BUF_GetSubPageNavCtrlInfo( &ttx_user_cursor, &subPageNavCtrl ))
            {
                TTX_DISP_DrawSubPageNavCtrl( &subPageNavCtrl );
            }
            /* Signals display should be refrehsed */
            TTX_DISP_PostRefreshSubPageNavCtrl();
        }
        //TTX_EVT_Clear(  DISP, UPDATE_NAV_CTRL );
    }

    /* Compare both "main page numbers" and "sub-codes" of "showing page" and "page to decode" */
    if ( ! TTX_STD_PageAddrWithSubCodeAreEqual( &(m_hTtx->pageShowing.addr),
            pPageToDecode, TRUE ) )
    {
        TTX_EVT_onShowingPageChanged(  pPageToDecode );
        flash_count = 0;
        // reset the UPDATE_PAGE event
        TTX_EVT_Clear(  DEC, UPDATE_PAGE );
    }
    else if ( TTX_EVT_isFired(  DEC, UPDATE_PAGE ) )  /* m_hTtx->pageShowing needs update */
    {
        uint8_t buf_idx;
        const bool_t bFound = TTX_BUF_FindPage( &(m_hTtx->pageShowing.addr), &buf_idx, TRUE );


        if (bFound)   // if not found, it means the buffer is on collecting
        {
            //RTD_Log(LOGGER_INFO, "TTX_EVT_onDecode ------ found\n");
            if ( TTX_EVT_onDecodingBufferedPage(  &(m_hTtx->pageShowing.addr), buf_idx ) )
            {
                TTX_EVT_onDecodedBufferedPage(  &(m_hTtx->pageShowing.addr), FALSE );
            }
        }
        else
        {
            //RTD_Log(LOGGER_INFO, "TTX_EVT_onDecode ------ not found \n");
            if ( ttx_select_subpage.pageA& _BIT15 ) // OSD of selecting subpage
            {
                TTX_DISP_DrawSelectSubpage(ttx_select_subpage);
                TTX_DISP_PostRefreshSubPageNavCtrl();
            }
        }

        TTX_EVT_Clear(  DEC, UPDATE_PAGE );
        flash_count = 0;
//			TTX_Log(LOGGER_VERBOSE, "UP!! i:%x %x\n",  (uint32_t)buf_idx, (uint32_t)bFound);
    }
    else
    {
        if ( flash_count >= 25 )   // added by hsliao 20090911, update every 500ms (500/20=25) for flash and auto update screen
        {
            TTX_EVT_Fire(DEC, UPDATE_PAGE);
            flash_count = 0;
        }
        else
        {
            flash_count++;
        }
    }

    if ( TTX_EVT_isFired(  DISP, UPDATE_NAV_CTRL ) )
    {

        // when the first time to enter the subpage, indicator is not parsing ready;
        // the cursor and showing page won't sync.
        if (ttx_subpage_auto_rotation.rotate == FALSE)
        {
            TTX_BUF_Sync_Cursor_Page(ttx_user_cursor);
        }

        TTX_EVT_Clear(  DISP, UPDATE_NAV_CTRL );
    }
}


/*---------------------------------------------------------------------------*/
static bool_t TTX_EVT_onDecodingBufferedPage (
    const TTX_STD_Page_AddrWithSubCode_t* pPageToDecode,
    uint8_t idx )
/*****************************************************************************\
 *  @brief   This event will be triggered whenever a complete page has been
 *             received.
 *  @param   [in] *pPageToDecode is the page address of decoding page.
 *  @param   [in] *pPageBuf is the page data stored in page-buffer.
\*****************************************************************************/
{
    uint16_t X0_Header_6_7 = 0;
    TTX_STD_Page_CtrlBits_t ctrlBits;
    TTX_LopParam_t param;
    uint8_t TTX_MEM* pPageBuf = TTX_BUF_GetMemByIdx(idx);

    /* Since we have found the page to decode, the header shall stop rolling. */
    TTX_DISP_EnableRolling( FALSE );

#if 1
    {
//        const TTX_STD_Page_AddrWithSubCode_Hex_t hexPageAddr =
//            TTX_STD_PageAddrWithSubCodeToHex( pPageToDecode );

        // Dragon's RTD_Log() does NOT support %X, but supports %x.
        // TTX_Log( LOGGER_INFO, "[TTX]" "[Trace] Decoding P%x.%x\n", (hexPageAddr>>16), (hexPageAddr & 0x0000FFFF) );
        /*
                if( (hexPageAddr>>16) == 0x697 )
                {
                    // int a = 3;
                }
        */
    }
#endif

#if 0
    // Copy a complete page data to *pPageBuf to test TTX_DecodePage().
    {
        // These are undescrambled X/0 to X/23 (in order) of  P100.2 of 7_Melb_151106.ts
        uint8_t const TTX_ROM TTX_test7_Melb_151106_P100_2_pkts [24][40] =
        {
            { 0x15, 0x15, 0x49, 0x15, 0x15, 0x15, 0x38, 0x15, 0x01, 0xC1, 0x75, 0x73, 0xF4, 0xE5, 0xF8, 0xF4, 0x02, 0x31, 0xB0, 0xB0, 0x07, 0x57, 0xE5, 0x64, 0x20, 0x31, 0xB5, 0x20, 0xCE, 0xEF, 0x76, 0x83, 0xB0, 0xB9, 0xBA, 0xB5, 0xB5, 0xBA, 0xB3, 0xB5 }, \
            { 0x97, 0x9D, 0x01, 0x0D, 0xC1, 0x20, 0xD5, 0x20, 0xD3, 0x20, 0x54, 0x20, 0x45, 0x20, 0x58, 0x20, 0x54, 0x20, 0x20, 0x20, 0xCD, 0x20, 0xC1, 0x20, 0x49, 0x20, 0xCE, 0x20, 0x20, 0x20, 0x49, 0x20, 0xCE, 0x20, 0xC4, 0x20, 0x45, 0x20, 0x58, 0x20 }, \
            { 0x16, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20 }, \
            { 0x97, 0x9D, 0x04, 0x20, 0x20, 0x20, 0x43, 0x4F, 0xD0, 0xD9, 0x52, 0x49, 0xC7, 0xC8, 0x54, 0x20, 0x49, 0xCE, 0x46, 0x4F, 0x52, 0xCD, 0xC1, 0x54, 0x49, 0x4F, 0xCE, 0x20, 0x4F, 0xCE, 0x20, 0x31, 0xB0, 0x34, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20 }, \
            { 0x91, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C }, \
            { 0x04, 0x9D, 0x20, 0x83, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x49, 0xCE, 0xC4, 0x45, 0x58, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x49, 0xCE, 0x46, 0x4F, 0x52, 0xCD, 0xC1, 0x54, 0x49, 0x4F, 0xCE, 0x20, 0x20, 0x20, 0x20 }, \
            { 0x94, 0x9D, 0x86, 0xCD, 0xC1, 0x49, 0xCE, 0x20, 0x49, 0xCE, 0xC4, 0x45, 0x58, 0xAE, 0xAE, 0xAE, 0x31, 0xB0, 0xB0, 0x13, 0x9D, 0x01, 0xCE, 0x45, 0x57, 0xD3, 0x20, 0x49, 0xCE, 0xC4, 0x45, 0x58, 0xAE, 0xAE, 0xAE, 0xAE, 0xAE, 0x31, 0x31, 0xB9 }, \
            { 0x04, 0x9D, 0x86, 0xC2, 0xD5, 0xD3, 0x49, 0xCE, 0x45, 0xD3, 0xD3, 0xAE, 0xAE, 0xAE, 0xAE, 0xAE, 0x31, 0x31, 0x31, 0x13, 0x9D, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20 }, \
            { 0x83, 0x9D, 0x04, 0xCE, 0x45, 0x57, 0xD3, 0xAE, 0xAE, 0xAE, 0xAE, 0xAE, 0xAE, 0xAE, 0xAE, 0xAE, 0x31, 0x31, 0xB9, 0x13, 0x9D, 0x01, 0x2A, 0x04, 0xCE, 0xC1, 0x54, 0x49, 0x4F, 0xCE, 0xC1, 0x4C, 0x20, 0xCE, 0x45, 0x57, 0xD3, 0x20, 0x20, 0x20 }, \
            { 0x04, 0x9D, 0x86, 0xD3, 0xD0, 0x4F, 0x52, 0x54, 0xAE, 0xAE, 0xAE, 0xAE, 0xAE, 0xAE, 0xAE, 0xAE, 0x31, 0xB6, 0xB0, 0x13, 0x9D, 0x04, 0x04, 0x20, 0x49, 0xCE, 0xC4, 0x45, 0x58, 0xAE, 0xAE, 0xAE, 0xAE, 0xAE, 0xAE, 0xAE, 0xAE, 0x31, 0x32, 0xB0 }, \
            { 0x04, 0x9D, 0x86, 0x04, 0x49, 0xCE, 0xC1, 0xCE, 0x43, 0x45, 0xAE, 0xAE, 0xAE, 0xAE, 0xAE, 0xAE, 0x32, 0xB0, 0xB0, 0x13, 0x9D, 0x01, 0x2A, 0x04, 0xC4, 0x45, 0x54, 0xC1, 0x49, 0x4C, 0xD3, 0x20, 0x31, 0x32, 0x31, 0x20, 0x5D, 0x31, 0xB3, 0xB0 }, \
            { 0x94, 0x9D, 0x86, 0x57, 0x45, 0xC1, 0x54, 0xC8, 0x45, 0x52, 0xAE, 0xAE, 0xAE, 0xAE, 0xAE, 0xAE, 0xB3, 0xB0, 0xB0, 0x13, 0x9D, 0x01, 0x2A, 0x04, 0xCE, 0xC1, 0x54, 0x49, 0x4F, 0xCE, 0xC1, 0x4C, 0x20, 0xCE, 0x45, 0x57, 0xD3, 0x20, 0x20, 0x20 }, \
            { 0x94, 0x9D, 0x86, 0x54, 0xC1, 0xC2, 0x20, 0xD3, 0x45, 0x52, 0xD6, 0x49, 0x43, 0x45, 0xAE, 0xAE, 0xB5, 0xB0, 0xB0, 0x13, 0x9D, 0x04, 0x20, 0x20, 0x52, 0x45, 0xD6, 0x49, 0x45, 0x57, 0xAE, 0xAE, 0xAE, 0xAE, 0xAE, 0xAE, 0xAE, 0x31, 0xB3, 0x37 }, \
            { 0x94, 0x9D, 0x86, 0xC7, 0x45, 0xCE, 0x45, 0x52, 0xC1, 0x4C, 0x20, 0x49, 0xCE, 0x46, 0x4F, 0xAE, 0xB6, 0xB0, 0xB0, 0x13, 0x9D, 0x01, 0x2A, 0x04, 0x49, 0xCE, 0x54, 0x45, 0x52, 0xCE, 0xC1, 0x54, 0x49, 0x4F, 0xCE, 0xC1, 0x4C, 0x20, 0x20, 0x20 }, \
            { 0x94, 0x9D, 0x86, 0x54, 0xD6, 0x20, 0xD0, 0x52, 0x4F, 0xC7, 0x52, 0xC1, 0xCD, 0xD3, 0xAE, 0xAE, 0xB6, 0xB5, 0xB0, 0x13, 0x9D, 0x04, 0x20, 0x20, 0xCE, 0x45, 0x57, 0xD3, 0x20, 0x52, 0x45, 0xD6, 0x49, 0x45, 0x57, 0xAE, 0xAE, 0x31, 0xB3, 0x38 }, \
            { 0x04, 0x9D, 0x86, 0x43, 0xC1, 0xD0, 0x54, 0x49, 0x4F, 0xCE, 0xD3, 0xAE, 0xAE, 0xAE, 0xAE, 0xAE, 0x38, 0xB0, 0x31, 0x13, 0x9D, 0x01, 0x2A, 0x04, 0xCE, 0x45, 0x57, 0xD3, 0x20, 0x4C, 0xC1, 0x54, 0x45, 0xD3, 0x54, 0xAE, 0xAE, 0x31, 0xB3, 0xB9 }, \
            { 0x94, 0x9D, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x13, 0x9D, 0x01, 0x2A, 0x04, 0x49, 0xCE, 0x54, 0x45, 0x52, 0xCE, 0xC1, 0x54, 0x49, 0x4F, 0xCE, 0xC1, 0x4C, 0x20, 0x20, 0x20 }, \
            { 0x94, 0x9D, 0x86, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x13, 0x9D, 0x04, 0x20, 0x04, 0xCE, 0x45, 0x57, 0xD3, 0x20, 0x49, 0xCE, 0xC4, 0x45, 0x58, 0xAE, 0xAE, 0xAE, 0x31, 0x34, 0xB0 }, \
            { 0x94, 0x9D, 0x86, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x13, 0x9D, 0x01, 0x2A, 0x04, 0xC4, 0x45, 0x54, 0xC1, 0x49, 0x4C, 0xD3, 0x20, 0x31, 0x34, 0x31, 0x20, 0x5D, 0x31, 0xB5, 0xB0 }, \
            { 0x94, 0x9D, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x13, 0x9D, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20 }, \
            { 0x94, 0x9D, 0x86, 0x20, 0x20, 0x04, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20 }, \
            { 0x07, 0x9D, 0x20, 0x20, 0x01, 0xCD, 0xC1, 0xCB, 0x45, 0x20, 0xC1, 0xD5, 0xD3, 0x54, 0x45, 0x58, 0x54, 0x20, 0xD9, 0x4F, 0xD5, 0x52, 0x20, 0x46, 0x49, 0x52, 0xD3, 0x54, 0x20, 0x43, 0xC8, 0x4F, 0x49, 0x43, 0x45, 0x20, 0x20, 0x20, 0x20, 0x20 }, \
            { 0x07, 0x9D, 0x01, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x46, 0x4F, 0x52, 0x20, 0x4C, 0x4F, 0x43, 0xC1, 0x4C, 0x20, 0xCE, 0x45, 0x57, 0xD3, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20 }, \
            { 0x01, 0x9D, 0x07, 0x20, 0x20, 0x20, 0xCE, 0xC1, 0x54, 0x49, 0x4F, 0xCE, 0xC1, 0x4C, 0x20, 0xCE, 0x45, 0x57, 0xD3, 0x20, 0x49, 0xCE, 0xC4, 0x45, 0x58, 0x20, 0xD0, 0xC1, 0xC7, 0x45, 0x20, 0x31, 0x32, 0xB0, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20 }  \
        };
        memcpy( pPageBuf, TTX_test7_Melb_151106_P100_2_pkts, 24*40 );
    }

#endif

//	TTX_Log(LOGGER_VERBOSE, "\nh6 %x %x\n",(uint32_t) pPageToDecode->pageA, (uint32_t) pPageBuf);

    X0_Header_6_7 = ( (uint16_t)pPageBuf[6] << 8 ) | ( (uint16_t)pPageBuf[7] << 0 );
    TTX_STD_ParseCtrlBits( X0_Header_6_7, &ctrlBits );

    // Decode Page: page buffer -> OSD attr -> OSD
    if ( TTX_DecodePage( idx, ctrlBits, &param ) )
    {
        TTX_DISP_PostRefreshPageContent();  /* row-0 to 23 need to be refreshed */
    }

    {
        uint8_t bSubtitleAvailable = FALSE;
#if 0
        /* Obtain TOP service info to m_hTtx->pageShowing.navInfo.top and bSubtitleAvailable */
        if ( ctrlBits.bInhibitDisplay ) // inhibit display: clear X/24
        {
            TTX_TOP_Hide();
        }
        else if ( ttx_select_subpage.pageA& _BIT15 ) // OSD of selecting subpage
#endif
            if ( ttx_select_subpage.pageA& _BIT15 ) // OSD of selecting subpage
            {
                //RTD_Log(LOGGER_INFO, "TTX_EVT_onDecodingBufferedPage : ttx_select_subpage.pageA& _BIT15 (%x) \n", (UINT32) ttx_select_subpage.subcodeA);
                TTX_DISP_DrawSelectSubpage(ttx_select_subpage);
                TTX_DISP_PostRefreshSubPageNavCtrl();
            }
#if defined(CONFIG_TTX_TOP_TOSHIBA) // TOP > FLOF
            else if ( TTX_TOP_Decode( pPageToDecode, &(m_hTtx->pageShowing.navInfo.top), &(bSubtitleAvailable), &m_hTtx->pageSubtitle, &param ) )
            {
                m_hTtx->bSubtitleAvailable = bSubtitleAvailable;
                TTX_DISP_PostRefreshTopCtrl();
            }
            else if ( TTX_FLOF_Decode( pPageToDecode, &(m_hTtx->pageShowing.navInfo.flof), idx ) )
            {

                TTX_DecodeX24( idx, &param ); // show FLOF
                TTX_DISP_PostRefreshTopCtrl();

            }
#elif defined(CONFIG_TTX_TOP_PHILIPS) // FLOF > TOP
            else if ( TTX_FLOF_Decode( pPageToDecode, &(m_hTtx->pageShowing.navInfo.flof), idx ) )
            {

                TTX_DecodeX24( idx, &param ); // show FLOF
                TTX_DISP_PostRefreshTopCtrl();

            }
            else if ( TTX_TOP_Decode( pPageToDecode, &(m_hTtx->pageShowing.navInfo.top), &(bSubtitleAvailable), &m_hTtx->pageSubtitle, &param ) )
            {
                m_hTtx->bSubtitleAvailable = bSubtitleAvailable;
                TTX_DISP_PostRefreshTopCtrl();
            }
#else
            else if ( TTX_FLOF_Decode( pPageToDecode, &(m_hTtx->pageShowing.navInfo.flof), idx ) )
            {
                TTX_DecodeX24( pPageBuf, &param ); // show FLOF
                TTX_DISP_PostRefreshTopCtrl();
            }
            else if ( TTX_TOP_Decode( pPageToDecode, &(m_hTtx->pageShowing.navInfo.top), &(bSubtitleAvailable), &m_hTtx->pageSubtitle, &param ) )
            {
                m_hTtx->bSubtitleAvailable = bSubtitleAvailable;
                TTX_DISP_PostRefreshTopCtrl();
            }
#endif
    }

    return( TRUE );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_EVT_onDecodedBufferedPage (
    const TTX_STD_Page_AddrWithSubCode_t* pPageDecoded,
    const bool_t changePageNum )
/*****************************************************************************\
 *  @brief   This event will be triggered whenever a complete page has been
 *             received.
 *  @param   [in] *pPageToDecode is the page address of decoding page.
 *  @param   [in] *pPageBuf is the page data stored in page-buffer.
\*****************************************************************************/
{
    TTX_DISP_PageNumAttr_t PageNumAttr;

    // before set buffer to displayed, we change pageShowing first
    m_hTtx->pageShowing.addr = *pPageDecoded;

    if (pPageDecoded->subcodeA == 0)
        TTX_DISP_ClrNavCtrl();

    if ( changePageNum ) // Update page number when user changes the showing page; do not update page number when source updates the showing page
    {
        PageNumAttr.bEnablePageUnits = TRUE; // display page units
        PageNumAttr.bEnablePageTens = TRUE; // display page tens
        PageNumAttr.bEnableMag = TRUE; // display magazine
        PageNumAttr.bEnableP = TRUE; // display 'P'

        PageNumAttr.ForegroundColor = TTX_COLOR_WHITE; // white foreground color
        PageNumAttr.ValidBackgroundColor = TTX_COLOR_BLACK; // black background color for valid magazine
        PageNumAttr.InvalidBackgroundColor = TTX_COLOR_BLACK; // black background color for invalid magazine
        TTX_DISP_SetPageNum( pPageDecoded, PageNumAttr );
    }


    /* Set *pPageDecoded as the current showing page */
    TTX_NAV_SetShowingPage(  pPageDecoded );

//	m_hTtx->statistics.cntPagesDecoded++;


#if DV_SIM // Contract of DV team
//          *(volatile uint32_t*)(0xA0203000) = m_hTtx->status.cntPagesDecoded;
#else
//            TTX_Log(LOGGER_VERBOSE, "%10d", m_hTtx->statistics.cntPagesDecoded );
//            TTX_Log(LOGGER_VERBOSE, "\b\b\b\b\b\b\b\b\b\b");
#endif

    /* Decoded attributes are placed in background memory, we shall switch
     *   the showing and background attributes whenever attributes are
     *   decoded or updated.
     */
    TTX_DISP_SwitchAttrMem();

    TTX_DISP_PostRefreshRow0(); // signal screen to display this decoded page
    TTX_DISP_PostRefreshPageContent();

    return( TRUE );
}

/*---------------------------------------------------------------------------*/
static void
TTX_EVT_onShowingPageChanged    (
    TTX_STD_Page_AddrWithSubCode_t* pPageToShow)
/*****************************************************************************\
 *  @brief   This event will be triggered whenever a complete page has been
 *             received.
 *  @param   [in] *pPageToShow is the page address of decoding page.
 *  @param   [in] *pPageBuf is the page data stored in page-buffer.
\*****************************************************************************/
{
//    void TTX_MEM* pPageBuf = NULL; /* the page data of *pPageToShow */
    uint8_t buf_idx = 0xff;

    TTX_REQUIRE( TTX_STD_isDisplayablePage2( pPageToShow ) );
    TTX_BUF_SetPageToShow( pPageToShow );    /* Start page-buffer to collect *pPageToShow. */

//		TTX_Log(LOGGER_VERBOSE, "h4 %x",(uint32_t) pPageToShow->pageA);

    /* Try to find exact matched *pPageToShow. If not, try to find its main page,
     *   and update *pPageToShow.
     */
    if (  TTX_BUF_FindPage( pPageToShow, &buf_idx, TRUE ) )
    {

        if ( m_hTtx->state == TTX_STATE_NORMAL )    /* SubPage nav control is only renderned in TTX_STATE_NORMAL */
        {
            /* signals highlight of page navigation control changes */
            TTX_EVT_Fire(  DISP, UPDATE_NAV_CTRL );
        }

        if ( TTX_EVT_onDecodingBufferedPage(  pPageToShow, buf_idx ) )
        {
            //RTD_Log(LOGGER_INFO, "TTX_EVT_onShowingPageChanged ... TTX_EVT_onDecodingBufferedPage\n");
            TTX_EVT_onDecodedBufferedPage(  pPageToShow, TRUE );

            // Update Page Number of OSD
            //TTX_DISP_SetPageNum( pPageToShow );
        }
        //RTD_Log(LOGGER_INFO, "TTX_EVT_onShowingPageChanged ... TTX_EVT_onDecodingBufferedPage == FALSE\n");

    }
    else
    {
        ttx_select_subpage.subcodeA = 0xFFFF; // display '----'
        //RTD_Log(LOGGER_INFO, "TTX_EVT_onShowingPageChanged ... TTX_BUF_FindPage == FALSE\n");
    }


}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_EVT_onScreenCanBeRefreshed  ()
/*****************************************************************************\
 *  @brief   This event will be triggered whenever display attribute is safe
 *             to be painted on screen.
\*****************************************************************************/
{
    if ( m_hTtx != NULL )
    {
        /* Because we have run TTX_DISP_Show( FALSE ) in _TTX_STATE_onEntering_SUBTITLE(),
         *   we have to re-show if user intend to show.
         */
        if ( TTX_EVT_isFired(  SYS, SHOW ) )
        {
            if ( ! TTX_DISP_isShowing() )
            {
                TTX_DISP_Show( TRUE );
            }
        }
        else
        {
            if ( TTX_DISP_isShowing() )
            {
                TTX_DISP_Show( FALSE );
            }
        }

        TTX_DISP_onScreenCanBeRefreshed();

        return( TRUE );
    }

    return( FALSE );
}

void TTX_SET_Reveal(bool_t isOn)
{
    if (isOn)
        rtdf_maskl(VBIOSD_CTRL3_REG_0x4808, ~_BIT30, _BIT30);
    else
        rtdf_maskl(VBIOSD_CTRL3_REG_0x4808, ~_BIT30, 0);
}

bool_t TTX_SET_First_Subpage()
{
    TTX_STD_Page_AddrWithSubCode_t PageAddr;
    uint8_t subCode;

    subCode = TTX_BUF_Get_First_Subcode();

    RTD_Log(LOGGER_INFO, "TTX_SET_First_Subpage : subCode %x !!! \n", (UINT32) subCode);


    if (subCode == 0xFF)
    {
        RTD_Log(LOGGER_INFO, "TTX_SET_First_Subpage : Get First Subpage Error !!! \n");
        return FALSE;
    }

    PageAddr.pageA = m_hTtx->pageShowing.addr.pageA;
    PageAddr.subcodeA = subCode;

    if (ttx_select_subpage.pageA&_BIT15)
        ttx_select_subpage.subcodeA = subCode;

    TTX_NAV_SetPageToShow(&PageAddr);

    return TRUE;
}

bool_t TTX_SET_Last_Subpage()
{
    TTX_STD_Page_AddrWithSubCode_t PageAddr;
    uint8_t subCode;

    subCode = TTX_BUF_Get_Last_Subcode();

    if (subCode == 0xFF)
    {
        RTD_Log(LOGGER_INFO, "TTX_SET_First_Subpage : Get First Subpage Error !!! \n");
        return FALSE;
    }

    PageAddr.pageA = m_hTtx->pageShowing.addr.pageA;
    PageAddr.subcodeA = subCode;

    if (ttx_select_subpage.pageA&_BIT15)
        ttx_select_subpage.subcodeA = subCode;

    TTX_NAV_SetPageToShow(&PageAddr);
    return TRUE;
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_EVT_onCmdHold()
/*****************************************************************************\
 *  @brief   event handler of TTX_CMD_HOLD
\*****************************************************************************/
{
    if ( m_hTtx != NULL )
    {
        TTX_DISP_SubpageHold();
        return( TRUE );
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_EVT_onCmdRotate()
/*****************************************************************************\
 *  @brief   event handler of TTX_CMD_ROTATE
\*****************************************************************************/
{
    if ( m_hTtx != NULL )
    {
        TTX_DISP_SubpageRotate();
        return( TRUE );
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*\
 * end of ttx_evt.c                                                          *
\*---------------------------------------------------------------------------*/
#endif
