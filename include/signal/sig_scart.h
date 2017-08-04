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

#ifndef _SIG_SCART_H_
#define _SIG_SCART_H_

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
void drv_sig_scart_Init(StructSourceInput *srcinput_pt);
UINT8 drv_sig_scart_DetectMode(void);
UINT8 drv_sig_scart_CheckMode(void);
void drv_sig_scart_SetupMode(void);
void drv_sig_scart_Release(void);

#endif //#ifndef _SIG_AVSV_H_
/*======================== End of File =======================================*/

