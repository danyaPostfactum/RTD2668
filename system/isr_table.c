/*=============================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2005
  * All rights reserved.
  * ============================================ */

/*================- File Description ================= */
/**
 * @file
 * 	This file is isr function.
 *
 * @author	$Author$
 * @date	$Date$
 * @version 	$Revision$$
 * @ingroup 	isr
 */

/**
* @addtogroup isr
* @{
*/

/*=============================================================*/
/**
 * Audio_ISR_Dispatcher
 * audio interrupt service routine
 *
 * @param <void>	{ void }
 * @return 			{ succeed in handling interrupt or not }
 * @ingroup drv_adc
 */
bit Audio_ISR_Dispatcher();

/*============================ Module dependency  ===========================*/
#include "sparrow.h"
#include "char\monitor.h"
#include "timer_event.h"
#include "isr_utility.h"
#include "irrc\irrc.h"
#include "reg52.h"
#include <reg_def.h>
#include <vbi/ppr.h>	/* Provides: drvif_PPR_Isr() */
#include <vbi/slr.h>
#include "pcb\pcb_utility.h"
#include "autoconf.h"


//extern get_time();
/*===================================  Types ================================*/
extern volatile bit fDebugMode;
extern bit flow_audio_Interrupt_Proc(void);
// Highest prority
void EX0_int(void) interrupt 0
{
    EA = 0;
	#if 1
    if (!drv_timer_event_isr_10ms())
        if (!drv_timer_event_isr_1ms())
            drv_isr_uart_handler();
	#endif
    EA = 1;
}

void EX1_int(void) interrupt 2
{
#if 0
    volatile UINT8 curPage;

    EA = 0;
    if (!fDebugMode)
    {
        curPage = RtdCtrl_isr_ReadScalerPort(Page_Select_0x009f);
    }

    drv_irrc_Isr();

    drv_timer_event_hdmi_isr_10ms();

    if (!fDebugMode)
    {
        drvif_PPR_Isr();
        flow_audio_Interrupt_Proc();
        RtdCtrl_isr_WriteScalerPort(Page_Select_0x009f, curPage);
    }
    EA = 1;
#else
    volatile UINT8 curPage;

    EA = 0;
    if (!fDebugMode)
    {
        curPage = RtdCtrl_isr_ReadScalerPort(Page_Select_0x009f);
    }
#if(IR_DETECTMODE == _IR_DS_SW)
    if (!fw_GPIO_Interrupt_Proc())
    {
        if (!drv_irrc_Isr())
        {
#else
    if (!drv_irrc_Isr())
    {
#endif//#if(IR_DETECTMODE == _IR_DS_SW)

		if(!drv_timer_event_hdmi_isr_10ms()) {//For SVN 1911
            if (!fDebugMode)
            {
#if defined(CONFIG_WSS_SUPPORT)
                drvif_vbi_wss_isr();

#endif
#ifdef CONFIG_VBI_CC_SUPPORT
                if (!drvif_vbi_cc_verify_isr())
                {
#endif

#ifdef CONFIG_TTX_SUPPORT
                    if (!drvif_PPR_Isr())
#endif
                        flow_audio_Interrupt_Proc();
#ifdef CONFIG_VBI_CC_SUPPORT
                }
#endif
            }//if (!fDebugMode)
        }//if(!drv_timer_event_hdmi_isr_10ms())
#if(IR_DETECTMODE == _IR_DS_SW)
        }//if (!drv_irrc_Isr())
       
#endif
    }	
    if (!fDebugMode)
        RtdCtrl_isr_WriteScalerPort(Page_Select_0x009f, curPage);
    EA = 1;
#endif
}

