/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2006
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is process key and ir input related functions.
 *
 * @author 	$Author$
 * @date		$Date$
 * @version	$Revision$
 * @ingroup	input
 */

/**
 * @addtogroup input
 * @{
 */

/*============================ Module dependency  ===========================*/
#include "signal\sig_v8.h"

/*===================================  Types ================================*/
/*================================== Variables ==============================*/
/*================================ Definitions ==============================*/
/*================================== Function ===============================*/
void drv_sig_v8_Init(void)
{
}

UINT8 drv_sig_v8_DetectMode(void)
{
    return 0xff;
}

bit drv_sig_v8_CheckMode(void)
{
    return 1;
}

void drv_sig_v8_SetupMode(void)
{
}

void drv_sig_v8_Release(void)
{
}

/*============================================================================*/
