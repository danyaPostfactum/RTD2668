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

#ifndef _SIG_YPBPR_H_
#define _SIG_YPBPR_H_

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

void drv_sig_ypbpr_Init(void);
UINT8 drv_sig_ypbpr_DetectMode(StructSourceInput *srcinput_pt);
bit drv_sig_ypbpr_CheckMode(void);
void drv_sig_ypbpr_SetupMode(void);
void drv_sig_ypbpr_Release(void);



#endif //#ifndef _SIG_YPBPR_H_
/*======================== End of File =======================================*/
