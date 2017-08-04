/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/
/*****************************************************************************\
 * @file     ttx_obj.c
 * @brief    Prototypes of public application interface.
 * @details  This header declares the public access application interface for this TtLib library.
 * @author   Janus Cheng
 * @date     Dec. 11, 2007
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/
#ifdef CONFIG_TTX_SUPPORT

#include <ttx_obj.h>        /* Provides: functions to be implemented */
#include <ttx_assert.h>     /* Provides: TTX_DEFINE_THIS_MODULE() */
#include <ttx_log.h>        /* Provides: TTX_Log(LOGGER_VERBOSE, ) */
#include <ttx_nav.h>        /* Provides: TTX_NAV_ResetSubPageInfo() */
#include <ttx.h>        /* Provides: TTX_NAV_ResetSubPageInfo() */

#include <stdlib.h>         /* Provides: NULL */


TTX_DEFINE_THIS_MODULE( ttx_obj.c )


/* -- Prototypes of Private Functions ---------------------------------------*/


/* -- Implementations of Public Functions -----------------------------------*/


/*---------------------------------------------------------------------------*/
void
TTX_OBJ_ResetAllMemberVariables    ()
/*****************************************************************************\
 *  @brief   The assertion-failure handler callback.
 *  @details Use this function to de-initialize this library.
 *  @note    Must be called in advance of any other functions.
 *
 *  @ingroup ttx
 *  @see     TTApi_DeInit()
\*****************************************************************************/
{
    TTX_FLOF_Key_t k;

    // Clear all fired events
    TTX_EVT_ClearCategory(  SYS  );    /* Clear all events of category TTX_EVTS_SYS. */
    TTX_EVT_ClearCategory(  DEC  );    /* Clear all events of category TTX_EVTS_DEC. */
    TTX_EVT_ClearCategory(  SRC  );    /* Clear all events of category TTX_EVTS_SRC. */
    TTX_EVT_ClearCategory(  BUF  );    /* Clear all events of category TTX_EVTS_BUF. */
    TTX_EVT_ClearCategory(  DISP );    /* Clear all events of category TTX_EVTS_DISP. */

    m_hTtx->statistics.cntPacketsProcessed = 0;
//      m_hTtx->statistics.cntPagesDecoded     = 0;

    m_hTtx->bSubtitleAvailable   = FALSE;

    {
        m_hTtx->pageShowing.navInfo.top.bAvailable = FALSE;
        m_hTtx->pageShowing.navInfo.flof.bAvailable = FALSE;
        m_hTtx->pageShowing.addr.pageA = 0x1FF;
        m_hTtx->pageShowing.addr.subcodeA= 0x3F7F;

        for ( k = 0; k < TTX_FLOF_KEY_NUM; k++ )
        {
            m_hTtx->pageShowing.navInfo.top.flofKeys[k].pageA = 0x100;
            m_hTtx->pageShowing.navInfo.top.flofKeys[k].subcodeA = 0x0;
            m_hTtx->pageShowing.navInfo.flof.flofKeys[k].pageA = 0x100;
            m_hTtx->pageShowing.navInfo.flof.flofKeys[k].subcodeA = 0x0;
        }
        for ( k = 0; k < 100; k++ )
        {
            m_hTtx->pageMap[k] = 0;
        }

    }

    m_hTtx->state = TTX_STATE_INITIAL;

    // Initialize m_hTtx->usrConfig
    {
        m_hTtx->usrConfig.pageToShow.bIsSet   = FALSE;    /* Reset to user hasn't specified the pageToShow address */
        m_hTtx->usrConfig.pageSubtitle.bIsSet = FALSE;    /* Reset to user hasn't specified the subtitle page address */

        for ( k = 0; k < TTX_FLOF_KEY_NUM; k++ )
        {
            m_hTtx->usrConfig.flofKeys[k].pageA = 0x100;
            m_hTtx->usrConfig.flofKeys[k].subcodeA = 0x0;
        }
    }

    // Initialize m_hTtx->infoSubtitle
    {
        m_hTtx->infoSubtitle.bIntendToFind = FALSE;
        m_hTtx->infoSubtitle.bFound        = FALSE;
    }

    m_hTtx->Correct_X0_cnt = 0;

}





/* -- Implementations of Private Functions ----------------------------------*/


/*---------------------------------------------------------------------------*\
 * end of ttx_obj.c                                                          *
\*---------------------------------------------------------------------------*/
#endif
