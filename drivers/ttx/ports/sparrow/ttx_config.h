/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_config.h
 * @brief    Configure the behavior of this ttx library.
 * @details  This header declares the public access application interface for this TtLib library.
 * @author   Janus Cheng
 * @date     Dec. 12, 2007
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/

/**
 * @addtogroup ttx
 * @{
 */
#ifndef __ENHANCED_TELETEXT_DECODER_LIBRARY_CONFIGURATION_HEADER_INCLUDED__
#define __ENHANCED_TELETEXT_DECODER_LIBRARY_CONFIGURATION_HEADER_INCLUDED__ (1)

/**
 * @defgroup TtxConfig Enhanced Teletext Decoder Library Configuration
 */

#define TTX_VER            "0.0.01"    /* Version number of this library, should be used in TTX_GetVersion() only. */

//#define TTX_NASSERT  /** define this macro to disable assertion checking */

/** define the minimum log level. Any log severe than this will be issued.
    Must be one of: ALL, TRACE, STATUS, WARNING, ERROR, PANIC, NONE */
#define TTX_CFG_LOG_LEVEL  TTX_LOG_LEVEL(ALL)


/** Select one of the following presentation level:
 *  (10): level 1.0
 *  (15): level 1.5
 *  (25): level 2.5
 *  (35): level 3.5
 *
 *  so that you can code like this:
 *
 *  #if TTX_CFG_PRESENT_LEVEL >= TTX_STD_PRESENT_LEVEL(25)
 *     do some thing special for presentation level 2.5 or higher
 *  #endif
 */
#define TTX_CFG_PRESENT_LEVEL       TTX_STD_PRESENT_LEVEL(15)



#endif /* !__ENHANCED_TELETEXT_DECODER_LIBRARY_CONFIGURATION_HEADER_INCLUDED__ */

/**
 * @}
 */

/*---------------------------------------------------------------------------*\
 * end of ttx_config.h                                                       *
\*---------------------------------------------------------------------------*/
