/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_bmposd_hel.c
 * @brief    Teletext software emulated BMP OSD hardware graphics engine. This is a
 *             platform-independent Hardware Emulation Layer (HEL).
 * @author   Janus Cheng
 * @date     Jul. 23, 2009
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/

#include <ttx_bmposd.h>     /* Provides: functions to be implemented */

#include <ttx_assert.h>     /* Provides: TTX_DEFINE_THIS_MODULE() */

#include <stdlib.h>         /* Provides: NULL */


#ifdef WIN32
#include <windows.h>
#include <tchar.h>      /* Provides: _T() */
#endif

#if TTX_USE_MALLOC
#include <malloc.h>     /* Provides: malloc() and free() */
#endif  /* end of #if TTX_USE_MALLOC */


TTX_DEFINE_THIS_MODULE( ttx_bmposd_hel.c )





/* -- Prototypes of Private Functions ---------------------------------------*/
static void     TTX_BMPOSD_SendToWindows                    (const UINT msg, const TTX_BMPOSD_PxFmt_t* pBmpToBeDrawn);
static bool_t   TTX_BMPOSD_onRow0NeedsRefreshed             (const TTX_BMPOSD_PxFmt_t* pBmpToBeRefreshed);
static bool_t   TTX_BMPOSD_onPageContentNeedsRefreshed      (const TTX_BMPOSD_PxFmt_t* pBmpToBeRefreshed);
static bool_t   TTX_BMPOSD_onTopCtrlNeedsRefreshed          (const TTX_BMPOSD_PxFmt_t* pBmpToBeRefreshed);
static bool_t   TTX_BMPOSD_onSubPageNavCtrlNeedsRefreshed   (const TTX_BMPOSD_PxFmt_t* pBmpToBeRefreshed);



/* -- Implementations of Public Functions -----------------------------------*/

/*---------------------------------------------------------------------------*/
bool_t
TTX_BMPOSD_Open (void)
/*****************************************************************************\
 *  @brief Open and initialize graphics engine.
 *  @param *pPixels is the total number of pixels in x (width) and y (height),
 *            respectively.
\*****************************************************************************/
{
    return( TRUE );
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_BMPOSD_Close (void)
/*****************************************************************************\
 *  @brief Open and initialize graphics engine.
\*****************************************************************************/
{
    return( TRUE );
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_BMPOSD_Show (const bool_t bEnable)
/*****************************************************************************\
 *  @brief Show or hide graphics engine
\*****************************************************************************/
{
    return( TRUE );
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_BMPOSD_Draw               (const TTX_BMPOSD_PxFmt_t* pBmpToBeDrawn)
/*****************************************************************************\
 *  @brief Send the bitmap *pBmpToBeDrawn to screen
 *  @param *pBmpToBeDrawn is the bitmap to be shown on display.
\*****************************************************************************/
{
    return( TRUE );
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_BMPOSD_ApplyChanges       (void)
/*****************************************************************************\
 *  @brief obtain the bitmap to be output.
\*****************************************************************************/
{
    return( TRUE );
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_BMPOSD_SplitRGB             (TTX_BMPOSD_PxFmt_t* bmp,
                                 const uint8_t alpha    ,
                                 const uint8_t red      ,
                                 const uint8_t green    ,
                                 const uint8_t blue     )
/*****************************************************************************\
 * @brief   Transforms to TTX_BMPOSD_PxFmt_t type from a CLUT entry (OSD_CLUT_t).
\*****************************************************************************/
{
    if ( bmp != NULL )
    {
        bmp->r = red;
        bmp->g = green;
        bmp->b = blue;
        bmp->a = alpha;

        return( TRUE );
    }

    return( FALSE );
}


/*---------------------------------------------------------------------------*/
void
TTX_BMPOSD_Callback   (const TTX_BMPOSD_CallbackEvt_t evt, TTX_BMPOSD_CallbackEvtArgs_t* pArg)
/*****************************************************************************\
 *  @brief   Hook your callbacks here.
 *  @param   evt   the reason for calling back.
 *  @param   *pArg the argument associated with evt..
\*****************************************************************************/
{
    switch ( evt )
    {
    case TTX_BMPOSD_CALLBACK_ON_ROW0_NEEDS_REFRESHED:
    {
        if ( pArg != NULL ) /* this event requires argument */
        {
            TTX_BMPOSD_onRow0NeedsRefreshed( pArg->pBmp );
        }
        break;
    }
    case TTX_BMPOSD_CALLBACK_ON_PAGE_CONTENT_NEEDS_REFRESHED:
    {
        if ( pArg != NULL ) /* this event requires argument */
        {
            TTX_BMPOSD_onPageContentNeedsRefreshed( pArg->pBmp );
        }
        break;
    }

    case TTX_BMPOSD_CALLBACK_ON_TOP_CTRL_NEEDS_REFRESHED:
    {
        if ( pArg != NULL ) /* this event requires argument */
        {
            TTX_BMPOSD_onTopCtrlNeedsRefreshed( pArg->pBmp );
        }
        break;
    }

    case TTX_BMPOSD_CALLBACK_ON_SUBPAGE_NAV_CTRL_NEEDS_REFRESHED:
    {
        if ( pArg != NULL ) /* this event requires argument */
        {
            TTX_BMPOSD_onSubPageNavCtrlNeedsRefreshed( pArg->pBmp );
        }
        break;
    }
    }
}


/* -- Implementations of Private Functions ----------------------------------*/

/*---------------------------------------------------------------------------*/
static void
TTX_BMPOSD_SendToWindows    (const UINT msg, const TTX_BMPOSD_PxFmt_t* pBmpToBeDrawn)
/*****************************************************************************\
 *  @brief   Will be triggered when display is able to be refreshed.
\*****************************************************************************/
{
    static HWND osdWnd = NULL;

    if ( osdWnd == NULL )
        osdWnd = FindWindow( NULL, _T("Taro") );

    if ( osdWnd != NULL )
    {
        if ( pBmpToBeDrawn != NULL )
        {
//            SendMessage(osdWnd, msg, (DWORD)pBmpToBeDrawn, 0);    // block: get the output bitmap and send it to screen
            PostMessage(osdWnd, msg, (DWORD)pBmpToBeDrawn, 0);    // non-block: get the output bitmap and send it to screen
        }
    }
    /*
        // Debug period of drawing
        {
            static size_t cnt = 0;
            TTX_Log( LOGGER_INFO, "[TTX] Draw (%d)\n", cnt++);
        }
    */
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_BMPOSD_onRow0NeedsRefreshed  (const TTX_BMPOSD_PxFmt_t* pBmpToBeRefreshed)
/*****************************************************************************\
 *  @brief   Will be triggered when display is able to be refreshed.
\*****************************************************************************/
{
#if 0
    extern void OSDEMU_PaintRow0 (void);
    OSDEMU_PaintRow0(); /* brutal force calling OSD emulator functions */
#endif
    /* Since every thing has done each state, we now apply the changes to screen */
    TTX_BMPOSD_SendToWindows( 0x12345, pBmpToBeRefreshed );

    return( TRUE );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_BMPOSD_onPageContentNeedsRefreshed      (const TTX_BMPOSD_PxFmt_t* pBmpToBeRefreshed)
/*****************************************************************************\
 *  @brief   Will be triggered when display is able to be refreshed.
\*****************************************************************************/
{
#if 0
    extern void OSDEMU_PaintPageContent (void);
    OSDEMU_PaintPageContent(); /* brutal force calling OSD emulator functions */
#endif

    /* Since every thing has done each state, we now apply the changes to screen */
    TTX_BMPOSD_SendToWindows( 0x12346, pBmpToBeRefreshed );

    return( TRUE );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_BMPOSD_onTopCtrlNeedsRefreshed          (const TTX_BMPOSD_PxFmt_t* pBmpToBeRefreshed)
/*****************************************************************************\
 *  @brief   Will be triggered when display is able to be refreshed.
\*****************************************************************************/
{
#if 0
    extern void OSDEMU_PaintTopCtrl (void);
    OSDEMU_PaintTopCtrl(); /* brutal force calling OSD emulator functions */
#endif
    /* Since every thing has done each state, we now apply the changes to screen */
    TTX_BMPOSD_SendToWindows( 0x12347, pBmpToBeRefreshed );

    return( TRUE );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_BMPOSD_onSubPageNavCtrlNeedsRefreshed   (const TTX_BMPOSD_PxFmt_t* pBmpToBeRefreshed)
/*****************************************************************************\
 *  @brief   Will be triggered when display is able to be refreshed.
\*****************************************************************************/
{
#if 0
    extern void OSDEMU_PaintSubPageNavCtrl (void);
    OSDEMU_PaintSubPageNavCtrl(); /* brutal force calling OSD emulator functions */
#endif
    /* Since every thing has done each state, we now apply the changes to screen */
    TTX_BMPOSD_SendToWindows( 0x12348, pBmpToBeRefreshed );

    return( TRUE );
}


/*---------------------------------------------------------------------------*\
 * end of ttx_bmposd_hel.c                                                   *
\*---------------------------------------------------------------------------*/

