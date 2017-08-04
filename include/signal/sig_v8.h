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

#ifndef _SIG_V8_H_
#define _SIG_V8_H_

/*============================ Module dependency  ===========================*/
#include "rtd_types.h"
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
void drv_sig_v8_Init(void);
UINT8 drv_sig_v8_DetectMode(void);
bit drv_sig_v8_CheckMode(void);
void drv_sig_v8_SetupMode(void);
void drv_Sig_v8_Release(void);


#endif //#ifndef _SIG_V8_H_
/*======================== End of File =======================================*/
/**
*
* @}
