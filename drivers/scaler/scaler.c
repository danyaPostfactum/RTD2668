/*=============================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2005
  * All rights reserved.
  * ============================================ */

/*================= File Description ================= */
/**
 * @file
 * The file is a declaration and definition header file
 *
 * @author 	$Author:  $
 * @date 	$Date:  $
 * @version 	$Revision:  $
 * @ingroup
 */

/**
* @addtogroup
* @
*/
#include "scaler\scaler.h"
#include "scaler\scaler_pll.h"
#include "flow_struct.h"
#include "scaler_idomain\syncproc.h"
#include "scaler_idomain\adc.h"
#include "reg_def.h"
#include "source.h"
#include "flow_scaler.h"
#include "scaler_idomain\measure.h"
#include "isr_utility.h"
#include "sparrow\sparrow_utility.h"
#include "rosprintf.h"
#include "timer_event.h"
#include "pcb_customer.h"
#include "pcb\pcb_utility.h"
#include "reg52.h"
// the parameter all data to zero
#define _ALL_ZERO (1 << 26)



void drv_scaler_WriteTable(UINT32* array)
{
    UINT32	len,index;
    UINT32	inc,all_zero;

    do
    {
        len = *array & (~(_AUTO_INC | _ALL_ZERO));
        if (len==0)
            break;

        inc = (*array & _AUTO_INC ? 1: 0);
        all_zero =  *array & _ALL_ZERO;
        array++;
        index = *array;
        array++;
        if (all_zero)
        {
            while (len--)
            {
                rtdf_outl(index, 0);
                if (!inc)
                    index+=4;
            }
        }
        else
        {
            while (len--)
            {
                rtdf_outl(index, *array);
                if (!inc)
                    index+=4;
                array++;
            }
        }
    }
    while (1);

}

void drv_scaler_enable_fs_wdg(bit enable)
{
    if (enable)
    {
        // frame sync only for main
//		rtdf_setBits(SCALEDOWN_ICH1_IBUFF_STATUS_ADDR, _BIT19|_BIT20);//set the watch dog for frame sync underflow and overflow
        rtdf_setBits(MAIN_DISPLAY_CONTROL_RSV_0x2100, _BIT15);//set Main Watch Dog Enable(force to background)
        rtdf_setBits(DISPLAY_TIMING_CTRL1_0x2000, _BIT20);// set frame sync watch dog Enable(force to freerun)
        rtdf_setBits(DM_UZU_CTRL_0x1604,_BIT24); // set watchdog for underflow
    }
    else
    {
        rtdf_clearBits(DISPLAY_TIMING_CTRL1_0x2000, _BIT20);
        rtdf_clearBits(MAIN_DISPLAY_CONTROL_RSV_0x2100, _BIT15);
        rtdf_clearBits(DM_UZU_CTRL_0x1604,_BIT24); // set watchdog for underflow
//		rtdf_clearBits(SCALEDOWN_ICH1_IBUFF_STATUS_ADDR, _BIT19|_BIT20);//disable watch dog
    }
}

void drv_scaler_enable_measure_wdg(bit enable)
{
    if (enable)
    {
        if ((GET_INPUTSOURCE_TYPE() == _SOURCE_HDMI) ||
                (GET_INPUTSOURCE_FROM() == _SRC_FROM_ADC))
        {
            rtdf_setBits(SP_MS1IE_0x0910, _BIT31|_BIT23|_BIT21|_BIT20  |_BIT17 |_BIT7|_BIT5|_BIT4|_BIT3|_BIT2|_BIT1|_BIT0);
            rtdf_setBits(MAIN_DISPLAY_CONTROL_RSV_0x2100, _BIT15);//set Main Watch Dog Enable(force to background)
            rtdf_setBits(DISPLAY_TIMING_CTRL1_0x2000, _BIT20);// set frame sync watch dog Enable(force to freerun)
            RTD_Log(LOGGER_DEBUG,"\ndrv_scaler_enable_measure_wdg\n");

            //drv_isr_Enable(SYS_ISR_ONLINE_MEASURE, _ENABLE);
            //drv_sparrow_Interrupt_Enable(1, _ENABLE);
        }
    }
    else
    {
        rtdf_clearBits(SP_MS1IE_0x0910, _BIT31|_BIT23|_BIT21|_BIT20  |_BIT17 |_BIT7|_BIT5|_BIT4|_BIT3|_BIT2|_BIT1|_BIT0);
//			rtdf_setBits(DDOMAIN_DISPLAY_TIMING_CTRL1_VADDR, _BIT20);// set frame sync watch dog Enable(force to background)
        rtdf_clearBits(MAIN_DISPLAY_CONTROL_RSV_0x2100, _BIT15);//set Main Watch Dog Enable(force to background)

        //drv_isr_Enable(SYS_ISR_ONLINE_MEASURE, _DISABLE);
    }
}

// Disable/Enable Watchdog  in D-domain
void drv_scaler_enable_ddomain_wdg(bit enable)
{
    if (enable)
        rtdf_setBits(MAIN_DISPLAY_CONTROL_RSV_0x2100,_BIT15);
    else
        rtdf_clearBits(MAIN_DISPLAY_CONTROL_RSV_0x2100,_BIT15);
}
void drv_scaler_set_display_mode(bit mode)
{
    UINT32 timeoutcnt = 0x1ffff;
    UINT32 value,i;
    //0:DISPLAY_MODE_FRC ,1:DISPLAY_MODE_FRAME_SYNC
    ///////////////////////////////////////////
    //Wait for Blank
    ///////////////////////////////////////////
    //WaitFor_DEN_STOP();
    //WaitFor_DEN_STOP();
    //WaitFor_DEN_STOP(); //for fix garbage during mode change
    if (mode)
    {
        EA = 0;
        rtdf_setBits(MAIN_DISPLAY_CONTROL_RSV_0x2100,_BIT11|_BIT10);
        for (i =0 ; i<0x1ffff; i ++)
        {
            value = rtdf_inl(MAIN_DISPLAY_CONTROL_RSV_0x2100)&_BIT10;
            if (value == 0)
                break;
        }

        do
        {
            value = (UINT32)rtdf_inl(MAIN_DISPLAY_CONTROL_RSV_0x2100)&_BIT10;
            if (value)
                break;
        }
        while (timeoutcnt-- != 0);
        /*
        Disp_fsync_en	26
        	Frame Sync Mode Enable
        	0: FRC mode.
        	1: Frame sync mode.
        */
        ///////////////////////////////////////////////////////
        rtdf_maskl(DISPLAY_TIMING_CTRL1_0x2000, ~(_BIT26 | _BIT29), _BIT26);
        EA = 1;
    }
    else
        rtdf_maskl(DISPLAY_TIMING_CTRL1_0x2000, ~(_BIT26 | _BIT29), 0);
//	if (mode)
//		drv_pcb_set_gpio_pin(AGPO_VIN00P,1);
//	else
//		drv_pcb_set_gpio_pin(AGPO_VIN00P,0);

}


void drv_scaler_update_mode_para(void)
{
    //[Code Sync][AlanLi][0980617][1]
    drv_pll_select_VCO_div();
    //[Code Sync][AlanLi][0980617][1][end]
    drv_syncproc_Adjust_ADC_Clock(FALSE);
    //drv_syncproc_ScalerSetVPosition(); //update v position
}

void drv_scaler_idomain_reset(void)
{
    rtdf_clearBitsb(SYS_SRST_1_0x2ca1,_BIT4);
    fw_timer_event_DelayXms(10);
    rtdf_setBitsb(SYS_SRST_1_0x2ca1,_BIT4);
}

/*============================================================================*/
/**
 * Hold until the specified event occurs
 *
 * @param domain: [input] I-domain or D-domain to wait.
 * @param ucEvent: [input] Specified event.
 * @return Wait result.
 * @retval _TRUE while event happened.
 * @retval _FALSE  for timeout.
 */
bit drv_scaler_WaitFor_Event(UINT8 event)
{
    UINT32 timeoutcnt = 0x1ffff;
    UINT32 value;

    rtdf_setBits(VGIP_CHN1_STATUS_0x0a14,_BIT27|_BIT26|_BIT25|_BIT24);//clear status
    rtdf_setBits(MAIN_DISPLAY_CONTROL_RSV_0x2100,_BIT11|_BIT10);
    fw_timer_event_DelayXms(1);

    do
    {
        switch (event)
        {
        case _EVENT_IVS:
            value = ((UINT32)rtdf_inl(VGIP_CHN1_STATUS_0x0a14))& _BIT26;
            //RTD_Log(LOGGER_DEBUG,"drv_scaler_WaitFor_Event_value = %x !\n",(UINT32)rtdf_inl(VGIP_CHN1_STATUS_0x0a14));
            if (value)
                return _TRUE;
            break;

        case _EVENT_IVS_END:
            value = ((UINT32)rtdf_inl(VGIP_CHN1_STATUS_0x0a14))& _BIT27;
            //RTD_Log(LOGGER_DEBUG,"drv_scaler_WaitFor_Event_value = %x !\n",(UINT32)rtdf_inl(VGIP_CHN1_STATUS_0x0a14));
            if (value)
                return _TRUE;
            break;

        case _EVENT_IENSTART1:
            value = (UINT32)rtdf_inl(VGIP_CHN1_STATUS_0x0a14)&_BIT24;
            //RTD_Log(LOGGER_DEBUG,"drv_scaler_WaitFor_Event_value = %x !\n",(UINT32)rtdf_inl(VGIP_CHN1_STATUS_0x0a14));
            if (value)
                return _TRUE;
            break;

        case _EVENT_IENSTOP1:
            value = (UINT32)rtdf_inl(VGIP_CHN1_STATUS_0x0a14)&_BIT25;
            //RTD_Log(LOGGER_DEBUG,"drv_scaler_WaitFor_Event_value = %x !\n",(UINT32)rtdf_inl(VGIP_CHN1_STATUS_0x0a14));
            if (value)
                return _TRUE;
            break;

        case _EVENT_DENSTOP:
            value = (UINT32)rtdf_inl(MAIN_DISPLAY_CONTROL_RSV_0x2100)&_BIT10;
            if (value)
                return _TRUE;
            break;

        case _EVENT_DENMAIN_START:
            value = (UINT32)rtdf_inl(MAIN_DISPLAY_CONTROL_RSV_0x2100)&_BIT11;
            if (value)
                return _TRUE;
            break;
        case _EVENT_DENMAIN_STOP:
            value = (UINT32)rtdf_inl(MAIN_DISPLAY_CONTROL_RSV_0x2100)&_BIT10;
            if (value)
                return _TRUE;
            break;
        }
    }
    while (timeoutcnt-- != 0);
    RTD_Log(LOGGER_DEBUG,"drv_scaler_WaitFor_Event_value timeout!\n");
    return _FALSE;
#if 0
    UINT32 timeoutcnt = 0x01FFFF;

    rtdf_setBits(addr, event);
    rtdf_clearBits(addr, event);
    do
    {
        if (rtdf_inl(addr) & event)
        {
            return _TRUE;
        }
    }
    while (timeoutcnt-- != 0);

    return _FALSE;

    return 1;
#endif
}

#if 0 // temporialy not used
void drv_scaler_poweron_Init(void)
{
    // Enable TV ctrl I/D/M domain
//	rtdf_setBits(PLL_SC_CLKEN1_VADDR, _BIT5 | _BIT4 | _BIT1);
    // Enable ADC power
    drv_adc_ADCPowerControl(_ON);
}
#endif
