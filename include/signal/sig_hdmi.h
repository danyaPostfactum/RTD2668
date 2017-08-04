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
#include "autoconf.h"
#ifdef CONFIG_ENABLE_HDMI

#ifndef _SIG_HDMI_H_
#define _SIG_HDMI_H

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
 */

//void drv_sig_hdmi_Init(StructSourceInput *srcinput_pt);
void drv_sig_hdmi_Init(void);
UINT8 drv_sig_hdmi_DetectMode(void);
bit drv_sig_hdmi_CheckMode(void);
void drv_sig_hdmi_SetupMode(void);
void drv_sig_hdmi_Release(void);

#endif //#ifndef _SIG_YPBPR_H_
/*======================== End of File =======================================*/
#endif
