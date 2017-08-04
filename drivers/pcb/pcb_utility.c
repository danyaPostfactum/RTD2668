/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2009
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is for rtdaccess related functions.
 *
 * @author	: Weihao Lo
 * @date		: 06/04/2009
 * @version	: 1.0
 * @ingroup	: pcb
 */

#include "pcb\pcb_utility.h"
#include "sparrow.h"
#include "pcb\pcb_pin_struct.h"
#include "reg_def.h"
#include "rosprintf.h"
#include "ui_struct.h"
#include "pcb_customer.h"
#include "isr_utility.h"

/**
 * drv_pcb_Init
 * Initialize configuration for pcb
 *
 * @param	<void>	{ void }
 * @return	{ none }
 *
 */
void drv_pcb_Init(void)
{
    UINT32 getVal;

//	Add some function to initial when booting
    AUDIO_AMP_MUTE_ENABLE(AUDIO_AMP_MUTE);
//
    rtd_outb(PA_CFG0_0x2d00, stPcbData.Pad_Setting_PA_CFG0);
    rtd_outb(PA_CFG1_0x2d01, stPcbData.Pad_Setting_PA_CFG1);
    rtd_outb(PA_CFG2_0x2d02, stPcbData.Pad_Setting_PA_CFG2);
    rtd_outb(PA_CFG3_0x2d03, stPcbData.Pad_Setting_PA_CFG3);

    rtd_outb(PB_CFG0_0x2d04, stPcbData.Pad_Setting_PB_CFG0);
    rtd_outb(PB_CFG1_0x2d05, stPcbData.Pad_Setting_PB_CFG1);
    rtd_outb(PB_CFG2_0x2d06, stPcbData.Pad_Setting_PB_CFG2);
    rtd_outb(PB_CFG3_0x2d07, stPcbData.Pad_Setting_PB_CFG3);

    rtd_outb(PC_CFG0_0x2d08, stPcbData.Pad_Setting_PC_CFG0);
    rtd_outb(PC_CFG1_0x2d09, stPcbData.Pad_Setting_PC_CFG1);
    rtd_outb(PC_CFG2_0x2d0a, stPcbData.Pad_Setting_PC_CFG2);
    rtd_outb(PC_CFG3_0x2d0b, stPcbData.Pad_Setting_PC_CFG3);

    rtd_outb(AHS_SMT_CTRL_0x2d0c, stPcbData.Pad_Setting_AHS_SMT_CTRL);
    rtd_outb(LVDS_RSDS_TTL_CFG_0x2d0d, stPcbData.Pad_Setting_LVDS_RSDS_TTL_CFG);
    rtd_outb(PA_SMT_0x2d24, stPcbData.Pad_Setting_PA_SMT);
    rtd_outb(PB_SMT_0x2d25, stPcbData.Pad_Setting_PB_SMT);
    rtd_outb(PC_SMT_0x2d26, stPcbData.Pad_Setting_PC_SMT);

    rtd_outb(PA_PS0_0x2d30, stPcbData.Pin_share_PA_PS0);
    rtd_outb(PA_PS1_0x2d31, stPcbData.Pin_share_PA_PS1);
    rtd_outb(PA_PS2_0x2d32, stPcbData.Pin_share_PA_PS2);
    rtd_outb(PA_PS3_0x2d33, stPcbData.Pin_share_PA_PS3);

    rtd_outb(PB_PS0_0x2d34, stPcbData.Pin_share_PB_PS0);
    rtd_outb(PB_PS1_0x2d35, stPcbData.Pin_share_PB_PS1);
    rtd_outb(PB_PS2_0x2d36, stPcbData.Pin_share_PB_PS2);
    rtd_outb(PB_PS3_0x2d37, stPcbData.Pin_share_PB_PS3);

    rtd_outb(PC_PS0_0x2d38, stPcbData.Pin_share_PC_PS0);
    rtd_outb(PC_PS1_0x2d39, stPcbData.Pin_share_PC_PS1);
    rtd_outb(PC_PS2_0x2d3a, stPcbData.Pin_share_PC_PS2);
    rtd_outb(PC_PS3_0x2d3b, stPcbData.Pin_share_PC_PS3);

    rtd_outb(PD_PS0_0x2d3c, stPcbData.Pin_share_PD_PS0);
    rtd_outb(PD_PS1_0x2d40, stPcbData.Pin_share_PD_PS1);

    rtd_outb(PG_PS0_0x2d48, stPcbData.Pin_share_PG_PS0);
    rtd_outb(PG_PS1_0x2d49, stPcbData.Pin_share_PG_PS1);
    rtd_outb(PG_PS2_0x2d4a, stPcbData.Pin_share_PG_PS2);
    rtd_outb(PG_PS3_0x2d4b, stPcbData.Pin_share_PG_PS3);

    rtd_outb(PH_PS0_0x2d4c, stPcbData.Pin_share_PH_PS0);
    rtd_outb(PH_PS1_0x2d4d, stPcbData.Pin_share_PH_PS1);
    rtd_outb(PH_PS2_0x2d4e, stPcbData.Pin_share_PH_PS2);
    rtd_outb(PH_PS3_0x2d4f, stPcbData.Pin_share_PH_PS3);

    rtd_outb(PI_PS0_0x2d50, stPcbData.Pin_share_PI_PS0);
    rtd_outb(PI_PS1_0x2d51, stPcbData.Pin_share_PI_PS1);

    // GPIO configuration
    // Direction (0~31)
#if 1
    *((UINT8*)&getVal) = stGPIOData.GPIO_Directon[3];
    *((UINT8*)&getVal + 1) = stGPIOData.GPIO_Directon[2];
    *((UINT8*)&getVal + 2) = stGPIOData.GPIO_Directon[1];
    *((UINT8*)&getVal + 3) = stGPIOData.GPIO_Directon[0];
#else
    getVal = stGPIOData.GPIO_Directon[3];
    getVal = (getVal << 8) | stGPIOData.GPIO_Directon[2];
    getVal = (getVal << 8) | stGPIOData.GPIO_Directon[1];
    getVal = (getVal << 8) | stGPIOData.GPIO_Directon[0];
#endif
    rtd_outl(GPDIR_0x2580, getVal);

    // Polarity (0~31)
#if 1
    *((UINT8*)&getVal) = stGPIOData.GPIO_POLARITY[3];
    *((UINT8*)&getVal + 1) = stGPIOData.GPIO_POLARITY[2];
    *((UINT8*)&getVal + 2) = stGPIOData.GPIO_POLARITY[1];
    *((UINT8*)&getVal + 3) = stGPIOData.GPIO_POLARITY[0];
#else
    getVal = stGPIOData.GPIO_POLARITY[3];
    getVal = (getVal << 8) | stGPIOData.GPIO_POLARITY[2];
    getVal = (getVal << 8) | stGPIOData.GPIO_POLARITY[1];
    getVal = (getVal << 8) | stGPIOData.GPIO_POLARITY[0];
#endif
    rtd_outl(GPDP_0x2590, getVal);

    // De-bounce clock base.
#if 1
    *((UINT8*)&getVal) = stGPIOData.GPIO_DEBOUNCE_BASE[3];
    *((UINT8*)&getVal + 1) = stGPIOData.GPIO_DEBOUNCE_BASE[2];
    *((UINT8*)&getVal + 2) = stGPIOData.GPIO_DEBOUNCE_BASE[1];
    *((UINT8*)&getVal + 3) = stGPIOData.GPIO_DEBOUNCE_BASE[0];
#else
    getVal = (stGPIOData.GPIO_DEBOUNCE_BASE[4] & 0x7);
    getVal = (getVal << 4) | (stGPIOData.GPIO_DEBOUNCE_BASE[3] & 0x07);
    getVal = (getVal << 4) | (stGPIOData.GPIO_DEBOUNCE_BASE[2] & 0x07);
    getVal = (getVal << 4) | (stGPIOData.GPIO_DEBOUNCE_BASE[2] & 0x07);
    getVal = (getVal << 8) | (stGPIOData.GPIO_DEBOUNCE_BASE[0] & 0x07);
#endif
    rtd_outl(GPDEB_0x2594, getVal);

    // Direction (32~38)
    *((UINT8*)&getVal) = stGPIOData.GPIO_Directon[4];
    *((UINT8*)&getVal + 1) = 0;
    *((UINT8*)&getVal + 2) = 0;
    *((UINT8*)&getVal + 3) = 0;
    rtd_maskl(GPIO_38_32A_0x2598, ~0x7f000000, getVal);

    // Polarity (32~38)
    rtd_maskl(GPIO_38_32B_0x259c, ~0x0000007f, stGPIOData.GPIO_POLARITY[3]);

    // Set AGPO options
    rtd_outb(AGPIO_JD_0_0x2d80, stAGPIOData.AGPIO_JD_0_ENABLE);
    rtd_outb(AGPIO_JD_2_0x2d82, stAGPIOData.AGPIO_JD_2_ENABLE);
    rtd_outb(AGPIO_AUDIO_0_0x2d84, stAGPIOData.AGPIO_AUDIO_0_ENABLE);
    rtd_outb(AGPIO_AUDIO_2_0x2d86, stAGPIOData.AGPIO_AUDIO_2_ENABLE);
    rtd_outb(AGPIO_VD_0_0x2d88, stAGPIOData.AGPIO_VD_0_ENABLE);
    rtd_outb(AGPIO_VD_2_0x2d8a, stAGPIOData.AGPIO_VD_2_ENABLE);
    rtd_outb(AGPIO_VD_4_0x2d8c, stAGPIOData.AGPIO_VD_4_ENABLE);
    rtd_outb(AGPIO_VD_6_0x2d8e, stAGPIOData.AGPIO_VD_6_ENABLE);
    rtd_outb(AGPIO_ADC_0_0x2d90, stAGPIOData.AGPIO_ADC_0_ENABLE);
    rtd_outb(AGPIO_ADC_2_0x2d92, stAGPIOData.AGPIO_ADC_2_ENABLE);

#if 0
    // AGPIO_JD enable ouptut
    rtd_maskb(AGPIO_JD_0_0x2d80, 0xf0, stAGPIOData.AGPIO_JD_0_ENABLE);

    // AGPIO Audio
#ifdef CONFIG_RTD2668_PCB_LONGYUAN_VER1
    rtd_maskb(AGPIO_AUDIO_0_0x2d84, 0xfe, stAGPIOData.AGPIO_Audio_2_ENABLE);
    // AGPIO VD3
    rtd_maskb(AGPIO_VD_2_0x2d8a, 0x90, stAGPIOData.AGPIO_VD_3_ENABLE);
    rtd_maskb(AGPIO_VD_0_0x2d88, ~0x02, 0x02/*stAGPIOData.AGPIO_VD_3_ENABLE*/);
    RTD_Log(LOGGER_INFO, "##AGPIO_VD_2_0x2d8a=%x\n",(UINT32)stAGPIOData.AGPIO_VD_3_ENABLE);

    // AGPIO VD7
    rtd_maskb(AGPIO_VD_4_0x2d8c, 0xc0, stAGPIOData.AGPIO_VD_7_ENABLE);

    // AGPIO ADC3
    rtd_maskb(AGPIO_ADC_0_0x2d90, 0x3f, stAGPIOData.AGPIO_ADC_3_ENABLE);
#else
    rtd_maskb(AGPIO_AUDIO_2_0x2d86, 0xfc, stAGPIOData.AGPIO_Audio_2_ENABLE);
    // AGPIO VD3
    rtd_maskb(AGPIO_VD_2_0x2d8a, 0x90, stAGPIOData.AGPIO_VD_3_ENABLE);

    // AGPIO VD7
    rtd_maskb(AGPIO_VD_2_0x2d8a, 0xc0, stAGPIOData.AGPIO_VD_7_ENABLE);

    // AGPIO ADC3
    rtd_maskb(AGPIO_VD_2_0x2d8a, 0x3f, stAGPIOData.AGPIO_ADC_3_ENABLE);
#endif

// temp to setup
//tommy modify091102 for pin54 Avout2/GPO control
    getVal = (!(AGPO_AVOUT2_ENABLE & _BIT0)) << 5;
    rtd_outb(AGPIO_VD_4_0x2d8c, 0x08|getVal);
//modify 091102
#ifdef CONFIG_RTD2668_PCB_LONGYUAN_VER1
    rtd_outb(AGPIO_VD_5_0x2d8d, 0x00);
#else
    rtd_outb(AGPIO_VD_5_0x2d8d, 0x08);
#endif
#endif
}

/**
 * set_gpio_pin
 * Set GPO in Dragon
 *
 * @param <UINT32> {port_reg :PERI_GPIO_ACR_VADDR,PERI_GPIO_BCR_VADDR...PERI_GPIO_HCR_VADDR}
 * @param <UINT32> {which_bit :_BIT0~_BIT7}
 * @return { none }
 *
 */
void drv_pcb_set_gpio_pin(UINT8 which_bit, bit val)
{
    if (which_bit < AGPO0)  			// GPIO
    {
        if (which_bit < GPIO32)
        {
            if (val)
                rtdf_setBits(GPDATO_0x2584, (1L << which_bit));
            else
                rtdf_clearBits(GPDATO_0x2584, (1L << which_bit));
        }
        else
        {
            if (val)
                rtdf_setBits(GPIO_38_32A_0x2598, ((1L << (which_bit - GPIO32) + 16)));
            else
                rtdf_clearBits(GPIO_38_32A_0x2598, ((1L << (which_bit - GPIO32) + 16)));
        }
    }
    else if (which_bit < GPIO_TOTAL)  	// AGPO
    {
        if (which_bit < AGPO4)  		// GPO0 ~ GPO3
        {
            if (val)
                rtdf_setBitsb(AGPIO_JD_1_0x2d81, (1 << (which_bit - AGPO0)));
            else
                rtdf_clearBitsb(AGPIO_JD_1_0x2d81, (1 <<  (which_bit - AGPO0)));
        }
        else if ((which_bit >= AGPO4) && (which_bit <= AGPO5))
        {
            if (val)
                rtdf_setBitsb(AGPIO_AUDIO_1_0x2d85, (1 << (which_bit - AGPO4)));
            else
                rtdf_clearBitsb(AGPIO_AUDIO_1_0x2d85, (1 << (which_bit - AGPO4)));
        }
        else if ((which_bit >= AGPO6) && (which_bit <= AGPO13))
        {
            //if(which_bit==AGPO7)
            //	RTD_Log(LOGGER_INFO, "##AGPIO_VD_1_0x2d89=%x\n",(UINT32)val);

            if (val)
                rtdf_setBitsb(AGPIO_VD_1_0x2d89, (1L << (which_bit - AGPO6)));
            else
                rtdf_clearBitsb(AGPIO_VD_1_0x2d89, (1 << (which_bit - AGPO6)));
        }
        else if ((which_bit >= AGPO14) && (which_bit <= AGPO19))
        {
            if (val)
                rtdf_setBitsb(AGPIO_VD_5_0x2d8d, (1 << (which_bit - AGPO14)));
            else
                rtdf_clearBitsb(AGPIO_VD_5_0x2d8d, (1 << (which_bit - AGPO14)));
        }
        else
        {
            if (val)
                rtdf_setBitsb(AGPIO_ADC_1_0x2d91, (1 << (which_bit - AGPO20)));
            else
                rtdf_clearBitsb(AGPIO_ADC_1_0x2d91, (1 << (which_bit - AGPO20)));
        }
    }
}

/**
 * get_gpio_pin
 * Get GPI pin in Dragon
 *
 * @param <UINT32> {port_reg :PERI_GPIO_ACR_VADDR,PERI_GPIO_BCR_VADDR...PERI_GPIO_HCR_VADDR}
 * @param <UINT32> {which_bit :_BIT0~_BIT7}
 * @return { none }
 *
 */
bit drv_pcb_get_gpio_pin(UINT8 which_bit)
{
    if (which_bit < AGPO0)
    {
        if (which_bit < GPIO32)
        {
            return (rtdf_inl(GPDATI_0x2588) & (1L << which_bit));
        }
        else
        {
            return (rtdf_inl(GPIO_38_32A_0x2598) & (1L << (which_bit - GPIO32 + 8)));
        }
    }
    else if (which_bit < GPIO_TOTAL)  	// AGPO
    {
        if (which_bit < AGPO4)  		// GPO0 ~ GPO3
        {
            return (rtdf_inb(AGPIO_JD_3_0x2d83) &  (1L << (which_bit - AGPO0)));
        }
        else if ((which_bit >= AGPO4) && (which_bit <= AGPO5))
        {
            return (rtdf_inb(AGPIO_AUDIO_3_0x2d87) &  (1L << (which_bit - AGPO4)));
        }
        else if ((which_bit >= AGPO6) && (which_bit <= AGPO13))
        {
            return (rtd_inb(AGPIO_VD_3_0x2d8b) & (1 << (which_bit - AGPO6)));
        }
        else if ((which_bit >= AGPO14) && (which_bit <= AGPO19))
        {
            return (rtd_inb(AGPIO_VD_7_0x2d8f) & (1 << (which_bit - AGPO14)));
        }
        else
        {
            return (rtd_inb(AGPIO_ADC_3_0x2d93) & (1 << (which_bit - AGPO20)));
        }
    }
    return 0;
}

/*********************************************************************
** 函数名称: bit drv_pcb_get_gpio_pin_isr(UINT8 which_bit)
** 功能描述: 得到GPIO 输入
** 输　  入: UINT8 which_bit : pin脚ID
** 输　  出: pin脚状态
**********************************************************************/

bit drv_pcb_get_gpio_pin_isr(UINT8 which_bit)
{
	volatile UINT8 data which_pin = which_bit;
	
    if (which_pin < AGPO0)
    {
        if (which_pin < GPIO32)
        {
            return (rtdf_inl(GPDATI_0x2588) & (1L << which_pin));
        }
        else
        {
            return (rtdf_inl(GPIO_38_32A_0x2598) & (1L << (which_pin - GPIO32 + 8)));
        }
    }
    else if (which_pin < GPIO_TOTAL)  	// AGPO
    {
        if (which_pin < AGPO4)  		// GPO0 ~ GPO3
        {
            return (rtdf_inb(AGPIO_JD_3_0x2d83) &  (1L << (which_pin - AGPO0)));
        }
        else if ((which_pin >= AGPO4) && (which_pin <= AGPO5))
        {
            return (rtdf_inb(AGPIO_AUDIO_3_0x2d87) &  (1L << (which_pin - AGPO4)));
        }
        else if ((which_pin >= AGPO6) && (which_pin <= AGPO13))
        {
            return (rtd_inb(AGPIO_VD_3_0x2d8b) & (1 << (which_pin - AGPO6)));
        }
        else if ((which_pin >= AGPO14) && (which_pin <= AGPO19))
        {
            return (rtd_inb(AGPIO_VD_7_0x2d8f) & (1 << (which_pin - AGPO14)));
        }
        else
        {
            return (rtd_inb(AGPIO_ADC_3_0x2d93) & (1 << (which_pin - AGPO20)));
        }
    }
    return 0;
}

/*********************************************************************
** 函数名称: void drv_pcb_set_gpio_pin_isr(UINT8 which_bit, bit val)
** 功能描述: 设置GPIO 输出
** 输　  入: UINT8 which_bit : pin脚ID
			 bit val		 : 输出状态
** 输　  出: 寄存器數值
**********************************************************************/

void drv_pcb_set_gpio_pin_isr(UINT8 which_bit, bit val)
{
    if (which_bit < AGPO0)  			// GPIO
    {
        if (which_bit < GPIO32)
        {
            if (val)
                RtdCtrl_isr_Mask(GPDATO_0x2584, 0xffffffff,(1L << which_bit));
            else
                RtdCtrl_isr_Mask(GPDATO_0x2584, ~(UINT32)(1L << which_bit),0);
        }
        else
        {
            if (val)
                RtdCtrl_isr_Mask(GPIO_38_32A_0x2598, 0xffffffff,((1L << (which_bit - GPIO32) + 16)));
            else
                RtdCtrl_isr_Mask(GPIO_38_32A_0x2598, ~(UINT32)((1L << (which_bit - GPIO32) + 16)),0);
        }
    }
    else if (which_bit < GPIO_TOTAL)  	// AGPO
    {
        if (which_bit < AGPO4)  		// GPO0 ~ GPO3
        {
            if (val)
                RtdCtrl_isr_MaskByte(AGPIO_JD_1_0x2d81, 0xff,(1 << (which_bit - AGPO0)));
            else
                RtdCtrl_isr_MaskByte(AGPIO_JD_1_0x2d81, ~(1 <<  (which_bit - AGPO0)), 0);
        }
        else if ((which_bit >= AGPO4) && (which_bit <= AGPO5))
        {
            if (val)
                RtdCtrl_isr_MaskByte(AGPIO_AUDIO_1_0x2d85, 0xff,(1 << (which_bit - AGPO4)));
            else
                RtdCtrl_isr_MaskByte(AGPIO_AUDIO_1_0x2d85, ~(1 << (which_bit - AGPO4)), 0);
        }
        else if ((which_bit >= AGPO6) && (which_bit <= AGPO13))
        {
            //if(which_bit==AGPO7)
            //	RTD_Log(LOGGER_INFO, "##AGPIO_VD_1_0x2d89=%x\n",(UINT32)val);

            if (val)
                RtdCtrl_isr_MaskByte(AGPIO_VD_1_0x2d89, 0xff, (1L << (which_bit - AGPO6)));
            else
                RtdCtrl_isr_MaskByte(AGPIO_VD_1_0x2d89, ~(1 << (which_bit - AGPO6)), 0);
        }
        else if ((which_bit >= AGPO14) && (which_bit <= AGPO19))
        {
            if (val)
                RtdCtrl_isr_MaskByte(AGPIO_VD_5_0x2d8d, 0xff, (1 << (which_bit - AGPO14)));
            else
                RtdCtrl_isr_MaskByte(AGPIO_VD_5_0x2d8d, ~(1 << (which_bit - AGPO14)), 0);
        }
        else
        {
            if (val)
                RtdCtrl_isr_MaskByte(AGPIO_ADC_1_0x2d91, 0xff, (1 << (which_bit - AGPO20)));
            else
                RtdCtrl_isr_MaskByte(AGPIO_ADC_1_0x2d91, ~(1 << (which_bit - AGPO20)), 0);
        }
    }
}

/*********************************************************************
** 函数名称: void fw_GPIO_inputerrupt_Init(void)
** 功能描述: 初始化GPIO中断
** 输　  入: 無
** 输　  出: 無 
**********************************************************************/
void fw_GPIO_inputerrupt_Init(void)
{
    rtdf_outb(SYS_GIR_C_0x2c30, _BIT3);  //GPIO IRQ1
    rtdf_outb(SYS_GIE_0_0x2c34, _BIT0);  //Enable Syatem GPIO interrupt
    rtdf_outl(GPIE0_A_0x25a0, _BIT13);  //Enable GPIO C2 assert interrupt
    rtdf_outl(UMSK_ISR_GPA_0x250c,0xffffffff);  //clean GPIO assert flag
    rtdf_outl(UMSK_ISR_GPDA_0x2510,0xffffffff);  //clean GPIO dis-assert flag
    rtdf_outl(ISR_0x2504, _BIT11);  //Clean flag
}

/*********************************************************************
** 函数名称: bit fw_GPIO_Interrupt_Proc(void)
** 功能描述: GPIO中断处理，可以响应多个IO口中断
** 输　  入: 無
** 输　  出: 無 
**********************************************************************/
bit fw_GPIO_Interrupt_Proc(void)
{
	if (!(rtd_isr_inb(SYS_GIP_0_0x2c38) & _BIT0))
        return RT_ISR_UNHANDLED;

    // Clear pending status for GPIO        
    ds_isr_outl(ISR_0x2504, _BIT11);
    
	if(!(rtdf_inl(GPDATI_0x2588)&(1L<<GPIO_C2)))
    	drv_isr_IR_decode();
    return RT_ISR_HANDLED;
}


