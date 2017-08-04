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
#ifndef _PCB_PIN_STRUCT_H
#define _PCB_PIN_STRUCT_H

#include "sparrow.h"

typedef struct
{
// Pad Configure
    // For Port A
    UINT8 Pad_Setting_PA_CFG0;
    UINT8 Pad_Setting_PA_CFG1;
    UINT8 Pad_Setting_PA_CFG2;
    UINT8 Pad_Setting_PA_CFG3;

    // For Port B
    UINT8 Pad_Setting_PB_CFG0;
    UINT8 Pad_Setting_PB_CFG1;
    UINT8 Pad_Setting_PB_CFG2;
    UINT8 Pad_Setting_PB_CFG3;

    // For Port C
    UINT8 Pad_Setting_PC_CFG0;
    UINT8 Pad_Setting_PC_CFG1;
    UINT8 Pad_Setting_PC_CFG2;
    UINT8 Pad_Setting_PC_CFG3;

    UINT8 Pad_Setting_AHS_SMT_CTRL;
    UINT8 Pad_Setting_LVDS_RSDS_TTL_CFG;

    // For Smith Trigger control
    UINT8 Pad_Setting_PA_SMT;
    UINT8 Pad_Setting_PB_SMT;
    UINT8 Pad_Setting_PC_SMT;

// Pin Share
    // For Port A
    UINT8 Pin_share_PA_PS0;
    UINT8 Pin_share_PA_PS1;
    UINT8 Pin_share_PA_PS2;
    UINT8 Pin_share_PA_PS3;

    // For Port B
    UINT8 Pin_share_PB_PS0;
    UINT8 Pin_share_PB_PS1;
    UINT8 Pin_share_PB_PS2;
    UINT8 Pin_share_PB_PS3;

    // For Port C
    UINT8 Pin_share_PC_PS0;
    UINT8 Pin_share_PC_PS1;
    UINT8 Pin_share_PC_PS2;
    UINT8 Pin_share_PC_PS3;

    // For Port D
    UINT8 Pin_share_PD_PS0;
    UINT8 Pin_share_PD_PS1;

    // For Port G
    UINT8 Pin_share_PG_PS0;
    UINT8 Pin_share_PG_PS1;
    UINT8 Pin_share_PG_PS2;
    UINT8 Pin_share_PG_PS3;

    // For Port H
    UINT8 Pin_share_PH_PS0;
    UINT8 Pin_share_PH_PS1;
    UINT8 Pin_share_PH_PS2;
    UINT8 Pin_share_PH_PS3;

    // For Port I
    UINT8 Pin_share_PI_PS0;
    UINT8 Pin_share_PI_PS1;
} _STRUCT_PCB_PARAMETER;

extern _STRUCT_PCB_PARAMETER code stPcbData;

typedef struct
{
    UINT8	GPIO_Directon[5];
    UINT8   	GPIO_POLARITY[5];
    UINT8	GPIO_DEBOUNCE_BASE[5];
} STRUCT_GPIO_CONFIGURATION;

typedef struct
{
    // [6:0]=lsadc_ref,lsddc5(X),lsadc4(X),lsadc3,lsadc2,lsadc1,lsadc0
    UINT8	AGPIO_JD_0_ENABLE;	// Enable output
    UINT8	AGPIO_JD_2_ENABLE;	// Enable Input

    // <1:0> AIO_1R, AIO_1L
    UINT8	AGPIO_AUDIO_0_ENABLE;	// Enable output
    UINT8	AGPIO_AUDIO_2_ENABLE;	// Enable Input

    // PAD: <7:0>: <7>VIN1N, <6>VIN12P <5>VIN02P <4>VIN0N,<3>VIN11P <2>VIN01P <1>VIN10P,<0>VIN00P
    UINT8	AGPIO_VD_0_ENABLE;	// Enable outupt
    UINT8	AGPIO_VD_2_ENABLE;	// Enable input

    // PAD: <15>x   <14>x   <13>AVOUT2, <12>AIN_2R,<11>RGBSW1 <10>AIN_2L,<9>RGBSW0,<8>VIN03P
    UINT8	AGPIO_VD_4_ENABLE;	// Enable Output
    UINT8	AGPIO_VD_6_ENABLE;	// Enable Input

    // PAD:[7:0]=AIN_5R,AIN_5L,R2P,R2N,G2P,G2N,B2P,B2N
    UINT8	AGPIO_ADC_0_ENABLE;	// Enable Output
    UINT8	AGPIO_ADC_2_ENABLE;	// Enable Input
} STRUCT_AGPIO_CONFIGURATION;

extern STRUCT_GPIO_CONFIGURATION code stGPIOData;
extern STRUCT_AGPIO_CONFIGURATION code stAGPIOData;

// Pad configure
//  Slew Rate selection
#define PAD_SLEW_RATE_SELECTION_SLOW 				0
#define PAD_SLEW_RATE_SELECTION_FAST 				1

// Driving Strength selection
#define PAD_DRIVING_STRENGTH_SELECTION_4mA			0
#define PAD_DRIVING_STRENGTH_SELECTION_8mA			1

// Internal Pull Up control
#define PAD_INTERNAL_PULL_UP_CONTROL_DISABLE		0
#define PAD_INTERNAL_PULL_UP_CONTROL_ENABLE		1

// internal Pull Down control
#define PAD_INTERNAL_PULL_DOWN_CONTROL_DISABLE	0
#define PAD_INTERNAL_PULL_DOWN_CONTROL_ENABLE		1

#endif //#ifndef _PCB_CONF_STRUCT_H
