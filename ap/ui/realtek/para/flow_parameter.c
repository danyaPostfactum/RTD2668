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
#include "rtd_types.h"
#include "flow_conf_struct.h"
#include "sys_info.h"
#include "flow_parameter.h"
#include "rt_storage_parameter.h"
#include "eepromdefaultcustomer.h"

// The declartation for sys infomation built after make
extern STRUCT_INFORMATION_ELEMENT code stCompilerVersion;
extern STRUCT_INFORMATION_ELEMENT code stCompilerDateInfo;
extern STRUCT_INFORMATION_ELEMENT code stCompilerClock;
extern STRUCT_INFORMATION_ELEMENT code stCompilerUsername;

STRUCT_FLOW_PARAMETER code stflowbData =
{
    &stCompilerVersion,		// Compiler_Version
    &stCompilerDateInfo,		// Compiler_Date
    &stCompilerClock,			// Compiler_Clock
    &stCompilerUsername,	// Compiler_Username
    DDC_ENABLE,				// fDDC_ENABLE
    HDMI_CEC_ENABLE,		// fHDMI_CEC_ENABLE
    SPREAD_SPECTRUM_ENABLE,	// fSPREAD_SPECTRUM_ENABLE
    DIGITAL_FILTER_INPUTPEAKING,	// fDIGITAL_FILTER_INPUTPEAKING
    SEND_MSG_AFTER_DETECT_FAILURE_TIMES,	// fSEND_MSG_AFTER_DETECT_FAILURE_TIMES
#if defined(CONFIG_ENABLE_RTD_LOG)
    1,							// fEnable_Output_Log
#else
    0,							// fEnable_Output_Log
#endif
    POWERON_AFTERBOOT,		// fBootMode_PowerOn
    WATCHDOG_ENABLE,			// fWatchDog_Enable
    WATCHDOG_PARAMETER,		// nWatchDog_Parameter
    60,							// power saving time parameter
#ifdef CONFIG_FLASH_USE_2BITS_READ
    1,							// nFirstReadBy2Bits_Flash
#else
    0,							// nFirstReadBy2Bits_Flash
#endif
    KEYCODE_BANK_NO,			// KEYCODE_BANK
    HDMI_HDCP_KEYCODE_ADDR,		// KEYCODE_HDCP_ADDR
    HDMI_EDID0_KEYCODE_ADDR,	// KEYCODE_EDID0_ADDR
    VGA_EDID0_KEYCODE_ADDR		// KEYCODE_EDID1_ADDR
};
