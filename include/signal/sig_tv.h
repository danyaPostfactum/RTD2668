/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2006
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is for state related functions.
 *
 * @author 	$Author$
 * @date	$Date$
 * @version	$Revision$
 * @ingroup	state
 */

/**
 * @addtogroup state
 * @{
 */

#ifndef _SIG_TV_H_
#define _SIG_TV_H_

/*============================ Module dependency  ===========================*/
#include "sparrow.h"
#include "source.h"
/*===================================  Types ================================*/

/*================================== Variables ==============================*/

/*================================ Definitions ==============================*/

/*================================== Function ===============================*/

/*============================================================================*/
/**
 *
 * @param <void>		{ void }
 * @return 			{ void }
 *
 */
//void drv_sig_tv_Init(StructSourceInput *srcinput_pt);
void drv_sig_tv_Init(void* InitOptions);
UINT8 drv_sig_tv_DetectMode(void);
bit drv_sig_tv_CheckMode(void);
void drv_sig_tv_SetupMode(void);
void drv_sig_tv_Release(void);
#ifdef CONFIG_ENABLE_FHFVBug
UINT8 drv_sig_tv_CheckFVBias(void);
#endif
void drv_sig_tv_ReSetVGIP(void);
void drv_sig_tv_SetVGIP(UINT8 mode);

#endif //#ifndef _SIG_TV_H_
/*======================== End of File =======================================*/

