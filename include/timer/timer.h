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

/**
 * @addtogroup System
 * @{
 */

#ifndef _TIMER_H
#define _TIMER_H
#include "sparrow.h"

/**
 *An enum.
 *The Timer controller no.
 */
typedef enum  _TimerType
{
    TIMER0 = 0,
    TIMER1,
    TIMER2,
    TIMER_TOTAL
} TimerType;

typedef struct _TIMER_PARAMETER_STRUCT_TABLE
{
    UINT8	bEnable;
    UINT32 	nTargetValue;
    UINT16	nDivider;
} TIMER_PARAMETER_STRUCT_TABLE;

bit drv_timer_Init(void);
void drv_timer_Interrupt_Enable(TimerType nTimer, bit enable);
void drv_timer_Enable(TimerType nTimer, bit enable);
//void drv_timer_Watchdog_Init(void);
void drv_timer_Watchdog_Enable(bit en);
// #define drv_timer_Reset_WatchDog() 					rtdf_setBits(TCWTR_0x2874, _BIT0)
#define drv_timer_Reset_WatchDog()			rtdf_setBits(TCWTR_0x2874, _BIT0)


#endif // #ifdef _TIMER_H

