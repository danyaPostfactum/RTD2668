/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_log.h
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
#ifndef __ENHANCED_TELETEXT_DECODER_LIBRARY_PLATFORM_INDEPENDENT_LOG_INTERFACE_HEADER_INCLUDED__
#define __ENHANCED_TELETEXT_DECODER_LIBRARY_PLATFORM_INDEPENDENT_LOG_INTERFACE_HEADER_INCLUDED__ (1)

#include "ttx_pkg.h"


#ifdef __cplusplus
extern "C"
{
#endif


#define _TTX_LOG_LEVEL_ALL      (  0)
#define _TTX_LOG_LEVEL_TRACE    ( 20)
#define _TTX_LOG_LEVEL_STATUS   ( 40)
#define _TTX_LOG_LEVEL_WARNING  ( 60)
#define _TTX_LOG_LEVEL_ERROR    ( 80)
#define _TTX_LOG_LEVEL_PANIC    (100)
#define _TTX_LOG_LEVEL_NONE     (120)



    /* Transforms content of TTX_CFG_LOG_LEVEL to TTX_LogLevel_t */
#define TTX_LOG_LEVEL(level) (_TTX_LOG_LEVEL_##level)


    /* Defines TTX_Trace() */
#if( TTX_CFG_LOG_LEVEL <= TTX_LOG_LEVEL(TRACE) )
#define TTX_Trace(fmt,arg)    TTX_Log(LOGGER_VERBOSE, fmt, arg)
#else
#define TTX_Trace             // 
#endif

    /* Defines TTX_Status() */
#if( TTX_CFG_LOG_LEVEL <= TTX_LOG_LEVEL(STATUS) )
#define TTX_Status(fmt,arg)   TTX_Log(LOGGER_INFO, fmt, arg)
#else
#define TTX_Status
#endif

    /* Defines TTX_Warning() */
#if( TTX_CFG_LOG_LEVEL <= TTX_LOG_LEVEL(WARNING) )
#define TTX_Warning(fmt,arg)  TTX_Log(LOGGER_WARN, fmt, arg)
#else
#define TTX_Warning
#endif

    /* Defines TTX_Error() */
#if( TTX_CFG_LOG_LEVEL <= TTX_LOG_LEVEL(ERROR) )
#define TTX_Error(fmt,arg)    TTX_Log(LOGGER_ERROR, fmt, arg)
#else
#define TTX_Error
#endif

    /* Defines TTX_Panic() */
#if( TTX_CFG_LOG_LEVEL <= TTX_LOG_LEVEL(PANIC) )
#define TTX_Panic(fmt,arg)    TTX_Log(LOGGER_FATAL, fmt, arg)
#else
#define TTX_Panic
#endif


#ifdef __cplusplus
}   /* extern "C" */
#endif


#endif /* !__ENHANCED_TELETEXT_DECODER_LIBRARY_PLATFORM_INDEPENDENT_LOG_INTERFACE_HEADER_INCLUDED__ */

/**
 * @}
 */

/*---------------------------------------------------------------------------*\
 * end of ttx_log.h                                                          *
\*---------------------------------------------------------------------------*/
