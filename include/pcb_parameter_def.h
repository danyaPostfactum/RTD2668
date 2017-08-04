/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2006
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is header file for vdc related functions.
 *
 * @author 	$Author$
 * @date		$Date$
 * @version	$Revision$
 * @ingroup	display
 */

/**
 * @addtogroup vdc
 * @{
 */

#ifndef _PCB_PARAMETER_DEF_H
#define _PCB_PARAMETER_DEF_H
/*============================ Module dependency  ===========================*/
#include "sparrow.h"

// Parameter in ADC
#define ADC_SH_GAIN_0	0x00  // 0.95
#define ADC_SH_GAIN_1	0x01  // 1
#define ADC_SH_GAIN_2	0x02  // 1.05
#define ADC_SH_GAIN_3	0x03  // 1.1
#define ADC_SH_GAIN_4	0x04  // 1.15
#define ADC_SH_GAIN_5	0x05  // 1.2
#define ADC_SH_GAIN_6  	0x06  // 1.25
#define ADC_SH_GAIN_7  	0x07  // 1.3
#define ADC_SH_GAIN_8  	0x08  // 1.35
#define ADC_SH_GAIN_9  	0x09  // 1.4
#define ADC_SH_GAIN_a  	0x0a  // 1.45

#define ADC_BANDWIDTH_75M		0x00	// 75M
#define ADC_BANDWIDTH_150M		0x01	// 150M
#define ADC_BANDWIDTH_300M		0x02	// 300M
#define ADC_BANDWIDTH_500M		0x03	// 500M

#define ADC_INPUT_SINGLE_ENDED 0
#define ADC_INPUT_DIFFERENTIAL_ENDED 1

#define ADC_BANDGAP_VOLTAGE_0			0x00	// 1.17
#define ADC_BANDGAP_VOLTAGE_1			0x01	// 1.21
#define ADC_BANDGAP_VOLTAGE_2			0x02	// 1.25
#define ADC_BANDGAP_VOLTAGE_3			0x03	// 1.30

// SOY input MUX in ADC
#define ADC_SOY_INPUT_MUX_VSOG_CAL	0x00
#define ADC_SOY_INPUT_MUX_VADC0		0x01
#define ADC_SOY_INPUT_MUX_VADC1		0x02
#define ADC_SOY_INPUT_MUX_VADC2		0x03
#define ADC_SOY_INPUT_MUX_VADC3		0x04
#define ADC_SOY_INPUT_MUX_VADC4		0x05
#define ADC_SOY_INPUT_MUX_VADC5		0x06
#define ADC_SOY_INPUT_MUX_VADC6		0x07
#define ADC_SOY_INPUT_MUX_VADC7		0x08
#define ADC_SOY_INPUT_MUX_VADC8		0x09
#define ADC_SOY_INPUT_MUX_B0		0x0a
#define ADC_SOY_INPUT_MUX_G0		0x0b
#define ADC_SOY_INPUT_MUX_R0		0x0c
#define ADC_SOY_INPUT_MUX_B1		0x0d
#define ADC_SOY_INPUT_MUX_G2		0x0e
#define ADC_SOY_INPUT_MUX_R3		0x0f

#endif // #ifndef _PCB_PARAMETER_DEF_H
