/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2006
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is a main fuction and entrance for firmwae.
 *
 * @author 	$Author$
 * @date	$Date$
 * @version	$Revision$
 * @ingroup	System
 */

#ifndef _PWM_H
#define _PWM_H

#include "sparrow.h"

typedef struct _STRUCT_PWM_PARAMETER
{
    UINT8 enable:1;
    UINT8 no:7;
    UINT8 gpio_no;
    UINT8 SourceDivisor;
    UINT8 OutputDivisor;
} STRUCT_PWM_PARAMETER;


//  The real PWM clock frequency is crystal clock(27MHz) / PWM clock source divisor / PWM output clock divisor / 2
//
void drv_pwm_Init(void);
void drv_pwm_SetClockDuty(UINT8 no, UINT8 duty);
void drv_pwm_PowerOn(bit on_off);

#endif //  #ifndef _PWM_H

