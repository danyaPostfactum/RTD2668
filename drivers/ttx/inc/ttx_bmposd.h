/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_bmposd.h
 * @brief    Teletext platform-independent bitmap OSD graphics engine declarations.
 * @note     These functions should be overwridden if your platform supports
 *             hardware BMP OSD.
 * @author   Janus Cheng
 * @date     Jul. 22, 2009
\*****************************************************************************/

/**
 * @addtogroup ttx
 * @{
 */
#ifndef __REALTEK_TELETEXT_BITMAP_ON_SCREEN_DISPLAY_HEADER_INCLUDED__
#define __REALTEK_TELETEXT_BITMAP_ON_SCREEN_DISPLAY_HEADER_INCLUDED__    (1)

#include <ttx_pkg.h>    /* Provides: TTX_BMPOSD_PxFmt_t */

#ifdef __cplusplus
extern "C"
{
#endif

    /* -- Global typedef Declarations -------------------------------------------*/

    typedef enum TTX_BMPOSD_CallbackEvt_t
    {
        TTX_BMPOSD_CALLBACK_ON_ROW0_NEEDS_REFRESHED,
        TTX_BMPOSD_CALLBACK_ON_PAGE_CONTENT_NEEDS_REFRESHED,
        TTX_BMPOSD_CALLBACK_ON_TOP_CTRL_NEEDS_REFRESHED,
        TTX_BMPOSD_CALLBACK_ON_SUBPAGE_NAV_CTRL_NEEDS_REFRESHED
    } TTX_BMPOSD_CallbackEvt_t;

    typedef union TTX_BMPOSD_CallbackEvtArgs_t
    {
        const TTX_BMPOSD_PxFmt_t* pBmp; /* the bitmap to be refreshed */
    } TTX_BMPOSD_CallbackEvtArgs_t;

    /* The POINT structure defines the x- and y-coordinates of a point. */
    typedef struct TTX_BMPOSD_POINT_t
    {
        int32_t x;    /* Specifies the x-coordinate of a point. */
        int32_t y;    /* Specifies the y-coordinate of a point. */
    } TTX_BMPOSD_POINT_t;

    /* The SIZE structure specifies the width and height of a rectangle. */
    typedef struct TTX_BMPOSD_SIZE_t
    {
        int32_t cx;   /* Specifies the rectangle's width. The units depend on which function uses this. */
        int32_t cy;   /* Specifies the rectangle's height. The units depend on which function uses this. */
    } TTX_BMPOSD_SIZE_t;




    /* -- Public Function Prototypes --------------------------------------------*/
    bool_t   TTX_BMPOSD_Open                    (void);
    bool_t   TTX_BMPOSD_Close                   (void);
    bool_t   TTX_BMPOSD_Show                    (const bool_t bEnable);
    bool_t   TTX_BMPOSD_Draw                    (const TTX_BMPOSD_PxFmt_t* pBmpToBeDrawn);
    bool_t   TTX_BMPOSD_ApplyChanges            (void);
    bool_t   TTX_BMPOSD_SplitRGB                (TTX_BMPOSD_PxFmt_t* bmp, const uint8_t a, const uint8_t r, const uint8_t g, const uint8_t b);

    /* event callbacks */
    void     TTX_BMPOSD_Callback                (const TTX_BMPOSD_CallbackEvt_t evt, TTX_BMPOSD_CallbackEvtArgs_t* pArg);


#ifdef __cplusplus
}   /* extern "C" */
#endif


#endif /* !__REALTEK_TELETEXT_BITMAP_ON_SCREEN_DISPLAY_HEADER_INCLUDED__ */

/**
 * @}
 */

/*---------------------------------------------------------------------------*\
 * end of ttx_bmposd.h                                                       *
\*---------------------------------------------------------------------------*/
