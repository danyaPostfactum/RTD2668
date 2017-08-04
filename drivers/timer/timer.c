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

#include "timer\timer.h"
#include "reg_def.h"
#include "isr_utility.h"
#include "flow_conf_struct.h"

extern TIMER_PARAMETER_STRUCT_TABLE  code stTimerParameter[TIMER_TOTAL];

bit drv_timer_Init(void)
{
    UINT8 count;
    TIMER_PARAMETER_STRUCT_TABLE* ptTimerParameter;
    for (count = 0; count < TIMER_TOTAL; count++)
    {
        ptTimerParameter = &stTimerParameter[count];

        // Disable interrupt for timer
        drv_isr_Enable(SYS_ISR_TIMER0 + count, _DISABLE);

        // assert in target time
        rtd_outl(TC0TVR_0x2840 + count * 4, ptTimerParameter->nTargetValue);

        // use timer mode | not use source divider
        rtd_maskl(TC0CR_0x2858+ count * 4, _BIT30, (_BIT30 | ptTimerParameter->nDivider));
    }

    return _TRUE;
}

void drv_timer_Interrupt_Enable(TimerType nTimer, bit enable)
{
    if (enable)
        rtdf_setBits(TC0ICR_0x2864 + nTimer * 4, _BIT31);
    else
        rtdf_clearBits(TC0ICR_0x2864 + nTimer * 4, _BIT31);
}

void drv_timer_Enable(TimerType nTimer, bit enable)
{
    if (enable)
        rtdf_setBits(TC0CR_0x2858+ nTimer * 4, _BIT31);
    else
        rtdf_clearBits(TC0CR_0x2858+ nTimer * 4, _BIT31);
}

void drv_timer_Watchdog_Enable(bit en)
{
//	if(stflowbData.fWatchDog_Enable) {
    if (en)   // Enable Watchdog
    {
//			rtdf_outl(TCWCR_0x2870, 0x000000A5);
//			rtd_outl(WTST_0x2878, 0);

        rtd_maskl(TCWCR_0x2870, 0x00ffff00, ((UINT32)0x4f << 24));
        rtd_outl(WTST_0x2878, 0xa5a5a5a5);
        //enable sysetm WD, scaler task must tick during max tolerance: 9.7(ms)x2^9 =4.9(sec)
        //note. bottleneck: when FrameSync Fail
//		rtd_outl(PERI_WR_CR_VADDR, 9<<28);
//->w		rtdf_maskl(TCWCR_0x2870, 0xf0000000, (GET_WATCHDOG_PARAMETER() & 0xf) << 28);

//		rtd_outl(PERI_WR_CR_VADDR, 9<<28|_BIT0);
//->w		rtdf_maskl(TCWCR_0x2870, 0xffffff00, 0x01);

        //kick watchdog
        //		drv_timer_Reset_WatchDog();
    }
    else   // Disable Watchdog
    {
        // disable watchdog
        rtdf_outl(TCWCR_0x2870, 0x000000A5);
        rtd_outl(WTST_0x2878, 0);
    }
//	}
}

