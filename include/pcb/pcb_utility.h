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

#ifndef _PCB_CONF_UTILITY_H
#define _PCB_CONF_UTILIT_H

#include "sparrow.h"
#include "pcb_conf.h"
#include "pcb\pwm.h"



// Gpio control for specified purpose
#define AUDIO_AMP_MUTE_ENABLE(x)		//;drv_pcb_set_gpio_pin(GPIO_AUDIO_AMP_MUTE, x);
#define LED_RED_ENABLE(x) 				//drv_pcb_set_gpio_pin(GPIO_LED_RED, x);
#define LED_GREEN_ENABLE(x) 			//drv_pcb_set_gpio_pin(GPIO_LED_GREEN, x);
#define AV_PVR_SEL(x)					drv_pcb_set_gpio_pin(GPIO_AV_PVR_SEL, x);
//#define ISDB_POWER_ENABLE(x)			drv_pcb_set_gpio_pin(GPIO_ISDB_POWER_ENABLE, x);
#define PVR_SWITCH_ENABLE(x)			drv_pcb_set_gpio_pin(GPIO_PVR_SWITCH_ENABLE, x);
#define BACKLIGHT_POWER_ENABLE(x)		drv_pcb_set_gpio_pin(GPIO_BACKLIGHT_POWER_ENABLE, x);
#define PANEL_POWER_ENABLE(x)			drv_pcb_set_gpio_pin(GPIO_PANEL_POWER_ENABLE, x);
#define HDMI0_HOT_PLUG(x)				drv_pcb_set_gpio_pin(GPIO_HDMI0_HOT_PLUG, x);
#define HDMI1_HOT_PLUG(x)				drv_pcb_set_gpio_pin(GPIO_HDMI1_HOT_PLUG, x);
#define HDMI2_HOT_PLUG(x)				drv_pcb_set_gpio_pin(GPIO_HDMI2_HOT_PLUG, x);
#define YPBPR_SOURCE_SELECT(x)			//drv_pcb_set_gpio_pin(GPIO_YPBPR_SELECT, x);
//#define STB_CTRL(x)						//drv_pcb_set_gpio_pin(GPIO_STB_CTRL, x);
#define STB_CTRL_A(x)					drv_pcb_set_gpio_pin(GPIO_SEL_A, x);                                             
#define STB_CTRL_B(x)					drv_pcb_set_gpio_pin(GPIO_SEL_B, x);

#define STB_IR_EN(x) 					drv_pcb_set_gpio_pin(GPIO_IR_EN, x);
#define STB_IR_CH(x) 			 		drv_pcb_set_gpio_pin(GPIO_IR_CH, x);
#if CONFIG_ENABLE_USB
#define STB_DVDPWR_CTRL(x) 				//drv_pcb_set_gpio_pin(GPIO_DVD_SW, x);
#endif
#if CONFIG_USB_CHECK
#define STB_LOADINGPIN(x) 			 	drv_pcb_set_gpio_pin(GPIO_LOADING_CTRL, x);
#define CLR_MUTE(x) 			 	    drv_pcb_set_gpio_pin(GPIO_MUTE, x);
#endif

#define POWER_ON_CTL(x) 			 	drv_pcb_set_gpio_pin(GPIO_POWER_ON, x);











//
#define SET_PANEL_BACKLIGHT(x)			drv_pwm_SetClockDuty(GPIO_PWM_BACKLIGH, x);//OSD backlight range:0~10, PWM duty range:0~255

/**
 * drv_pcb_Init
 * Initialize configuration for pcb
 *
 * @param	<void>	{ void }
 * @return	{ none }
 *
 */
void drv_pcb_Init(void);
#if 0
/**
 * drv_pcb_set_gpio
 * write value into GPO
 *
 * @param <UINT32> {port_reg :PERI_GPIO_ACR_VADDR,PERI_GPIO_BCR_VADDR...PERI_GPIO_HCR_VADDR}
 * @param <UINT32> {which_bit :_BIT0~_BIT7}
 * @return { none }
 *
 */
void drv_pcb_set_gpio(UINT32 val);

/**
 * drv_pcb_set_gpio32_38
 * write value into GPO32~GPO38
 *
 * @param <UINT32> {port_reg :PERI_GPIO_ACR_VADDR,PERI_GPIO_BCR_VADDR...PERI_GPIO_HCR_VADDR}
 * @param <UINT32> {which_bit :_BIT0~_BIT7}
 * @return { none }
 *
 */
void drv_pcb_set_gpio32_38(UINT8 val);

/**
 * drv_pcb_get_gpio
 * Read value into GPI
 *
 * @param <UINT32> {port_reg :PERI_GPIO_ACR_VADDR,PERI_GPIO_BCR_VADDR...PERI_GPIO_HCR_VADDR}
 * @param <UINT32> {which_bit :_BIT0~_BIT7}
 * @return { none }
 *
 */
UINT32 drv_pcb_get_gpio(void);

/**
 * drv_pcb_set_gpio32_38
 * write value into GPO32~GPO38
 *
 * @param <UINT32> {port_reg :PERI_GPIO_ACR_VADDR,PERI_GPIO_BCR_VADDR...PERI_GPIO_HCR_VADDR}
 * @param <UINT32> {which_bit :_BIT0~_BIT7}
 * @return { none }
 *
 */
UINT8 drv_pcb_get_gpio32_38(void);
#endif

/**
 * set_gpio_pin
 * Set GPO in Dragon
 *
 * @param <UINT32> {port_reg :PERI_GPIO_ACR_VADDR,PERI_GPIO_BCR_VADDR...PERI_GPIO_HCR_VADDR}
 * @param <UINT32> {which_bit :_BIT0~_BIT7}
 * @return { none }
 *
 */
void drv_pcb_set_gpio_pin(UINT8 which_bit, bit val);

/**
 * get_gpio_pin
 * Get GPI pin in Dragon
 *
 * @param <UINT32> {port_reg :PERI_GPIO_ACR_VADDR,PERI_GPIO_BCR_VADDR...PERI_GPIO_HCR_VADDR}
 * @param <UINT32> {which_bit :_BIT0~_BIT7}
 * @return { none }
 *
 */
bit drv_pcb_get_gpio_pin(UINT8 which_bit);
bit drv_pcb_get_gpio_pin_isr(UINT8 which_bit);
void drv_pcb_set_gpio_pin_isr(UINT8 which_bit, bit val);
void fw_GPIO_inputerrupt_Init(void);
bit fw_GPIO_Interrupt_Proc(void);

#endif //#ifndef _PCB_CONF_LIBRARY_H
