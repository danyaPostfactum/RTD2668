/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2009
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is for rtdaccess related functions.
 *
 * @author 	$Author$
 * @date	$Date$
 * @version	$Revision$
 * @ingroup	rtdaccess
 */

/**
 * @addtogroup rtdaccess
 * @{
 */
#ifndef _FLOW_PARAMETER_H
#define _FLOW_PARAMETER_H

#define DDC_ENABLE	1
#define HDMI_CEC_ENABLE 1
#define SPREAD_SPECTRUM_ENABLE 1
#define DIGITAL_FILTER_INPUTPEAKING 1
#define SEND_MSG_AFTER_DETECT_FAILURE_TIMES 4

#if defined(CONFIG_DEFAULT_POWER_ON)
#define POWERON_AFTERBOOT		1
#else
#define POWERON_AFTERBOOT		0
#endif

#define _VIDEO_AUTO_WHITE_BLANCE 0
#define WATCHDOG_ENABLE			0
#define WATCHDOG_PARAMETER		9

#define KEYCODE_BANK_NO			_BANK0
#define HDMI_HDCP_KEYCODE_ADDR	0xf000
#define HDMI_EDID0_KEYCODE_ADDR	0xf200
#define VGA_EDID0_KEYCODE_ADDR	0xf400

#endif // #ifndef _FLOW_PARAMETER_H
