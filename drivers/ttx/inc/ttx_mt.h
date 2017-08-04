/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_mt.h
 * @brief    Teletext library internal platform-indenpendent multi-threading
 *             utilities.
 * @author   Janus Cheng
 * @date     Feb. 10, 2009
\*****************************************************************************/

/**
 * @addtogroup ttx
 * @{
 */
#if( TTX_SUPPORT_MT )

#ifndef __REALTEK_TELETEXT_INTERNAL_PLATFORM_INDEPENDENT_MULTI_THREADING_UTITLITY_HEADER_INCLUDED__
#define __REALTEK_TELETEXT_INTERNAL_PLATFORM_INDEPENDENT_MULTI_THREADING_UTITLITY_HEADER_INCLUDED__    (1)

#include <ttx_pkg.h>    /* Provides: UINT32_C() */

#ifdef __cplusplus
extern "C"
{
#endif

    /* -- Global typedef Declarations -------------------------------------------*/




    /* -- Public Function Prototypes --------------------------------------------*/
#if defined( WIN32 )
#include <windows.h>
#include <tchar.h>              /* Provides: _T() */

#define Mutex                   HANDLE
#define TTX_MT_Open(m, name)    m = CreateMutex(NULL, FALSE, _T(name));
#define TTX_MT_Lock(m)          WaitForSingleObject(m, INFINITE);
#define TTX_MT_Unlock(m)        ReleaseMutex(m);
#define TTX_MT_Close(m)         {CloseHandle(m); m = NULL;}
#elif defined( __GNUC__ )
#include <pthread.h>

#define Mutex                   pthread_mutex_t
#define TTX_MT_Open(m, name)    pthread_mutex_init(&m, NULL);
#define TTX_MT_Lock(m)          pthread_mutex_lock(&m);
#define TTX_MT_Unlock(m)        pthread_mutex_unlock(&m);
#define TTX_MT_Close(m)         pthread_mutex_destroy(&m);
#else
#error  "ToDo: Add TTX_MT to support your platform."
#endif



#ifdef __cplusplus
}   /* extern "C" */
#endif


#endif /* !__REALTEK_TELETEXT_INTERNAL_PLATFORM_INDEPENDENT_MULTI_THREADING_UTITLITY_HEADER_INCLUDED__ */

#endif /* end of #if( TTX_SUPPORT_MT ) */

/**
 * @}
 */

/*---------------------------------------------------------------------------*\
 * end of ttx_mt.h                                                           *
\*---------------------------------------------------------------------------*/
