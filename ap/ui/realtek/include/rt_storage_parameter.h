/*=============================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2009 *
 * All rights reserved.                                       *
 *============================================================*/
/*======================= Description ========================*/
/**
  * @file
 * @Author Weihao Lo
 * @date  2009/06/01
 * @version 1.0
 */

/*=============================================================*/
#include "autoconf.h"

#ifndef _RT_STORAGE_PARAMETER_H
#define _RT_STORAGE_PARAMETER_H

#ifdef CONFIG_FLASH_8M_SIZE
#define STORAGE_RING_ELEMENT_SIZE			( 4 * 1024 )
#define STORAGE_RING_NUMBER					( 6 )
#define STORAGE_RING_START					( 0xa000 )
#define STORAGE_BANK_NO						_BANK8
#define STORAGE_ERASE_BLOCK_BYTE				( 1 )
#define _FIRSTRUN_VAR_BANK					_BANK8
#define CStorageFirstRun							(0x9ffcUL)
#define CStorageVersion							(0x9ff8UL)
#else
#define STORAGE_RING_ELEMENT_SIZE			( 4 * 1024 )
#define STORAGE_RING_NUMBER					( 4 )
#define STORAGE_RING_START					( 0xc000 )
#define STORAGE_BANK_NO						_BANK7
#define STORAGE_ERASE_BLOCK_BYTE				( 1 )
#define _FIRSTRUN_VAR_BANK					_BANK7
#define CStorageFirstRun							(0xbffcUL)
#define CStorageVersion							(0xbff8UL)
#endif

#endif // #ifndef _RT_STORAGE_PARAMETER_H

