/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_cb.c
 * @brief    Teletext callback functions.
 * @note     You should overwrite this implementation if you need these callbacks
 *             on your platform/projects.
 * @author   Janus Cheng
 * @date     Aug. 20, 2009
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/
#ifdef CONFIG_TTX_SUPPORT

#include <ttx.h>            /* Provides: functions to be implemented */

#include <stdlib.h>         /* Provides: NULL */




/* -- Prototypes of Private Functions ---------------------------------------*/


/* -- Implementations of Public Functions -----------------------------------*/

/*---------------------------------------------------------------------------*/
void
TTX_Callback   (const TTX_CallbackEvt_t evt, TTX_CallbackEvtArgs_t* pArg)
/*****************************************************************************\
 *  @brief   Hook your callbacks here.
 *  @param   evt   the reason for calling back.
 *  @param   *pArg the argument associated with evt..
\*****************************************************************************/
{
    switch ( evt )
    {
    case TTX_CALLBACK_ON_ENTERED_NORMAL:
    {
        TTX_Log( LOGGER_VERBOSE, "[TTX] normal\n");
        break;
    }
    case TTX_CALLBACK_ON_ENTERED_SUBTITLE:
    {
        TTX_Log( LOGGER_VERBOSE, "[TTX] subtitle\n");
        break;
    }
    case TTX_CALLBACK_ON_STOPPED:
    {
        TTX_Log( LOGGER_VERBOSE, "[TTX] stopped\n");
        break;
    }

    if ( pArg != NULL ) /* to eliminate compiler warning */
    {
    }
    }
}


/*---------------------------------------------------------------------------*\
 * end of ttx_cb.c                                                           *
\*---------------------------------------------------------------------------*/
#endif
