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
#ifndef _FLOW_CONF_STRUCT_H
#define _FLOW_CONF_STRUCT_H

#include "sparrow.h"

typedef struct _STRUCT_INFORMATION_ELEMENT
{
    UINT8	num;
    char*	char_info;
} STRUCT_INFORMATION_ELEMENT;

typedef struct _STRUCT_FLOW_PARAMETER
{
    STRUCT_INFORMATION_ELEMENT*	Compiler_Version;
    STRUCT_INFORMATION_ELEMENT*	Compiler_Date;
    STRUCT_INFORMATION_ELEMENT*	Compiler_Clock;
    STRUCT_INFORMATION_ELEMENT*	Compiler_Username;
    UINT8 	fDDC_ENABLE;
    UINT8 	fHDMI_CEC_ENABLE;
    UINT8	fSPREAD_SPECTRUM_ENABLE;
    UINT8	fDIGITAL_FILTER_INPUTPEAKING;
    UINT8	fSEND_MSG_AFTER_DETECT_FAILURE_TIMES;
    UINT8	fEnable_Output_Log;
    UINT8	fBootMode_PowerOn;
    UINT8	fWatchDog_Enable;
    UINT8	nWatchDog_Parameter;
    UINT8	nPowerSaving_Parameter;
    UINT8	nFirstReadBy2Bits_Flash;
    UINT8	KEYCODE_BANK;
    UINT16	KEYCODE_HDCP_ADDR;
    UINT16	KEYCODE_EDID0_ADDR;			// For HDMI
    UINT16	KEYCODE_EDID1_ADDR;			// For VGA
} STRUCT_FLOW_PARAMETER;

extern STRUCT_FLOW_PARAMETER code stflowbData;

void flow_system_LoadParameter(UINT8 xdata* ptBuf, UINT8 offset, UINT8 size);
#define GET_POWER_ON_AFTER_BOOT()	stflowbData.fBootMode_PowerOn
#define GET_WATCHDOG_ENABLE()			stflowbData.fWatchDog_Enable
#define GET_WATCHDOG_PARAMETER()	stflowbData.nWatchDog_Parameter
#define GET_FLOW_PARAMETER_DATA(ELEMENT, ELEMENT_VAR)	\
		flow_system_LoadParameter((UINT8 xdata*)&ELEMENT_VAR, OFFSET_STRUCT(STRUCT_FLOW_PARAMETER, ELEMENT), SIZEOF_STRUCT(STRUCT_FLOW_PARAMETER, ELEMENT))

#endif // #ifndef _FLOW_CONF_STRUCT_H
