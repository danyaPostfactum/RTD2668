/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_nav.h
 * @brief    Teletext page navigation routines.
 * @author   Janus Cheng
 * @date     Feb. 13, 2009
\*****************************************************************************/

/**
 * @addtogroup ttx
 * @{
 */
#ifndef __REALTEK_TELETEXT_PAGE_NAVIGATOR_HEADER_INCLUDED__
#define __REALTEK_TELETEXT_PAGE_NAVIGATOR_HEADER_INCLUDED__    (1)

#include <ttx_obj.h>    /* Provides: TTX_Handle */
#include <ttx_std.h>    /* Provides: TTX_STD_Page_AddrWithSubCode_t */

#include <stdlib.h>     /* Provides: size_t */

#ifdef __cplusplus
extern "C"
{
#endif

    /* -- Global typedef Declarations -------------------------------------------*/


    /* -- Public Function Prototypes --------------------------------------------*/

    bool_t          TTX_NAV_GetNextPage             (       TTX_STD_Page_AddrWithSubCode_t* pNextPage   );
    bool_t          TTX_NAV_GetPrevPage             (       TTX_STD_Page_AddrWithSubCode_t* pPrevPage   );
    bool_t          TTX_NAV_GetNextTopPageInBlock   (       TTX_STD_Page_AddrWithSubCode_t* pNextPage   );
    bool_t          TTX_NAV_GetPrevTopPage          (       TTX_STD_Page_AddrWithSubCode_t* pPrevPage   );
    bool_t          TTX_NAV_GetNextSubPage          (       TTX_STD_Page_AddrWithSubCode_t* pNextSubPage);
    bool_t          TTX_NAV_GetPrevSubPage          (       TTX_STD_Page_AddrWithSubCode_t* pPrevSubPage);
    void            TTX_NAV_SetPageToShow           (       const TTX_STD_Page_AddrWithSubCode_t* pPageToShow );
    void            TTX_NAV_SetShowingPage          (       const TTX_STD_Page_AddrWithSubCode_t* pShowingPage);

#ifdef __cplusplus
}   /* extern "C" */
#endif


#endif /* !__REALTEK_TELETEXT_PAGE_NAVIGATOR_HEADER_INCLUDED__ */

/**
 * @}
 */

/*---------------------------------------------------------------------------*\
 * end of ttx_nav.h                                                          *
\*---------------------------------------------------------------------------*/
