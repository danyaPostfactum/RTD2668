/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_evt.h
 * @brief    Teletext library internal event processor.
 * @author   Janus Cheng
 * @date     Feb. 10, 2009
\*****************************************************************************/

/**
 * @addtogroup ttx
 * @{
 */
#ifndef __REALTEK_TELETEXT_INTERNAL_EVENT_PROCESSOR_HEADER_INCLUDED__
#define __REALTEK_TELETEXT_INTERNAL_EVENT_PROCESSOR_HEADER_INCLUDED__    (1)

#include <ttx_pkg.h>    /* Provides: UINT32_C() */

#ifdef __cplusplus
extern "C"
{
#endif

    /* -- Global typedef Declarations -------------------------------------------*/

    /* The event category available */
    enum
    {
        TTX_EVTS_SYS = 0,
        TTX_EVTS_DEC,
        TTX_EVTS_SRC,
        TTX_EVTS_BUF,
        TTX_EVTS_DISP,

        TTX_EVTS_NUM
    };

    /* The events of category TTX_EVTS_SYS */
#define TTX_EVT_SYS_NONE             (UINT32_C(0))
#define TTX_EVT_SYS_NORMAL           (UINT32_C(1) <<  0)    /* Normal Teletext pages with respect to Subtitle pages */
#define TTX_EVT_SYS_SUBTITLE         (UINT32_C(1) <<  1)
#define TTX_EVT_SYS_STOP             (UINT32_C(1) <<  2)
#define TTX_EVT_SYS_SHOW             (UINT32_C(1) <<  3)
#define TTX_EVT_SYS_PAUSE            (UINT32_C(1) <<  4)
#define TTX_EVT_SYS_PREV_PAGE        (UINT32_C(1) <<  5)
#define TTX_EVT_SYS_NEXT_PAGE        (UINT32_C(1) <<  6)
#define TTX_EVT_SYS_PREV_SUBPAGE     (UINT32_C(1) <<  7)
#define TTX_EVT_SYS_NEXT_SUBPAGE     (UINT32_C(1) <<  8)
#define TTX_EVT_SYS_ROTATE_SUBPAGE	(UINT32_C(1) << 9)	// next subpage by auto-rotating
#define TTX_EVT_SYS_FAST_CALL_RED    (UINT32_C(1) <<  10)
#define TTX_EVT_SYS_FAST_CALL_GREEN  (UINT32_C(1) << 11)
#define TTX_EVT_SYS_FAST_CALL_YELLOW (UINT32_C(1) << 12)
#define TTX_EVT_SYS_FAST_CALL_CYAN   (UINT32_C(1) << 13)


    /* The events of category TTX_EVTS_DEC */
#define TTX_EVT_DEC_NONE             (UINT32_C(0))
#define TTX_EVT_DEC_UPDATE_PAGE      (UINT32_C(1) <<  0)
#define TTX_EVT_DEC_LIST_MODE        (UINT32_C(1) <<  1)


    /* The events of category TTX_EVTS_SRC */
#define TTX_EVT_SRC_NONE             (UINT32_C(0))
#define TTX_EVT_SRC_BUF_FULL         (UINT32_C(1) <<  0)
#define TTX_EVT_SRC_BUF_VALID        (UINT32_C(1) <<  1)

    /* The events of category TTX_EVTS_BUF */
#define TTX_EVT_BUF_NONE             (UINT32_C(0))

    /* The events of category TTX_EVTS_DISP */
#define TTX_EVT_DISP_NONE            (UINT32_C(0))
#define TTX_EVT_DISP_UPDATE_NAV_CTRL (UINT32_C(1) <<  0)


    /* Implements internal state-machine of TTX */
    typedef uint8_t TTX_State_t;

    /* Permissible values of type TTX_State_t */
    typedef enum TTX_StateValues_t
    {
        TTX_STATE_INITIAL,
        TTX_STATE_READY,
        TTX_STATE_NORMAL,
        TTX_STATE_SUBTITLE
    } TTX_StateValues_t;




    /* -- Public Function Prototypes --------------------------------------------*/

    /* Returns TRUE if specified evt has been fired */
#define     TTX_EVT_isFired( category, evt)   (m_hTtx->evtsFired[TTX_EVTS_##category] &   TTX_EVT_##category##_##evt)

    /* Fire the specified evt */
#define     TTX_EVT_Fire( category, evt)      m_hTtx->evtsFired[TTX_EVTS_##category] |=  TTX_EVT_##category##_##evt

    /* Stop firing the specified evt */
#define     TTX_EVT_Clear( category, evt)     m_hTtx->evtsFired[TTX_EVTS_##category] &= ~TTX_EVT_##category##_##evt

    /* Stop firing all events of the category */
#define     TTX_EVT_ClearCategory( category)  m_hTtx->evtsFired[TTX_EVTS_##category]  =  TTX_EVT_##category##_NONE

#define     TTX_STATE_TransitTo( newState)    _TTX_STATE_onEntering_##newState(  ); m_hTtx->state = TTX_STATE_##newState



#ifdef __cplusplus
}   /* extern "C" */
#endif


#endif /* !__REALTEK_TELETEXT_INTERNAL_EVENT_PROCESSOR_HEADER_INCLUDED__ */

/**
 * @}
 */

/*---------------------------------------------------------------------------*\
 * end of ttx_evt.h                                                          *
\*---------------------------------------------------------------------------*/
