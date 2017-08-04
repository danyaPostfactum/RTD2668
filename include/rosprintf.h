/*=============================================================
 * Copyright C        Realtek Semiconductor Corporation, 2005 *
 * All rights reserved.                                       *
 *============================================================*/

/*======================= Description ========================*/
/**
 * @file
 *   This header file for RTD2638's registers define.
 * @Author Tzungder Lin
 * @date 2006/06/19
 * @version { 1.0 }
 *
 */

#ifndef _ROSPRINTF_H_
#define _ROSPRINTF_H_
#include "autoconf.h"
#include "rtd_types.h"

#if !defined(CONFIG_ENABLE_RTD_LOG)
#define CONFIG_LOGGER_FATAL 1
#endif

typedef enum
{
    LOGGER_FATAL=0,
    LOGGER_DEBUG=5,
    LOGGER_ERROR=10,
    LOGGER_WARN=20,
    LOGGER_INFO=30,
    LOGGER_VERBOSE=40,
    LOGGER_OFF=100
} _LOGGER_LEVEL;

#if defined(CONFIG_LOGGER_FATAL)
#define LOGGER_LEVEL LOGGER_FATAL
#elif defined(CONFIG_LOGGER_DEBUG)
#define LOGGER_LEVEL LOGGER_DEBUG
#elif defined(CONFIG_LOGGER_ERROR)
#define LOGGER_LEVEL LOGGER_ERROR
#elif defined(CONFIG_LOGGER_WARN)
#define LOGGER_LEVEL LOGGER_WARN
#elif defined(CONFIG_LOGGER_INFO)
#define LOGGER_LEVEL LOGGER_INFO
#elif defined(CONFIG_LOGGER_VERBOSE)
#define LOGGER_LEVEL LOGGER_VERBOSE
#else
#define LOGGER_LEVEL LOGGER_OFF
#endif
UINT32 Log_trans_UINT32(UINT32 value);
void fw_rosprintf_para_Init(void);
void  RTD_Log(_LOGGER_LEVEL level, char code *format , ...);

#endif // #ifndef _ROSPRINTF_H_
