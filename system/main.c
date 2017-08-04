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
#include "sparrow.h"
#include "system.h"
#include "message.h"
#include "rosprintf.h"
#include "osdcore.h"
#include "modestate.h"
#include "timer_event.h"
#include "userinput.h"
#include "char\monitor.h"
#include "rt_storage.h"
#include "power.h"
#include "flow_conf_struct.h"
#include "reg52.h"
#include "flow_audio.h"
#include "ap_rtirkey.h"
#include "ui_struct.h"
#include "ui_ctrl.h"
#include "pcb\pcb_utility.h"
#include "pcb_customer.h"

#ifdef CONFIG_TTX_SUPPORT
#include <ttx/ttx.h>	// Provides: TTX_Handle
#endif
#include "autoconf.h"

#ifdef CONFIG_VBI_CC_SUPPORT
#include "closecaption.h"
#endif

//#include "i2c\i2c.h"

#if  CONFIG_ENABLE_USB

#include "flow_struct.h"

extern UINT16 g_cnt_2,g_cnt_1;


#endif

#if CONFIG_USB_CHECK
extern UINT8 xdata stUsbFlag;
UINT8 xdata stUsbFlag_NO=0;

#endif

void main(void)
{

  //  Initialize system parameter when before boot
    flow_system_parameter_Init();

    // Initial hardware-related function
    flow_system_Init();
	//BACKLIGHT_POWER_ENABLE(1);
    RTD_Log(LOGGER_INFO, "Sparrow Booting ...\n");

    // Show System's information
    flow_system_ShowInfo();

    // Start to initial system-related function
    // Initial message queue
    fw_message_Init();

    // Initial Timer module
    fw_timer_event_Init();

    // Initial Timer2 module for delay
    fw_timer2_event_Init();

#ifdef CONFIG_ENABLE_HDMI
    // Initial Timer for HDMI
    fw_timer_hdmi_10ms_Init();
#endif

#if(IR_DETECTMODE == _IR_DS_SW)
	//fw_timer1_10ms_Init();
	fw_GPIO_inputerrupt_Init();
#endif
    // Initial monitor handler
    fw_monitor_Init();
	
#if ID692_EN   
	if(!bReadID())
		goto _mainend;
#endif	    

	DS_Init();
    // Restore osd data to system
    flow_storage_Init();
//	SET_POWERSWITCH();
    // Power on if GET_POWER_ON_AFTER_BOOT() true
    if (GET_POWER_ON_AFTER_BOOT())
    {
        SET_POWERSWITCH();
    }
    else
    {
        //if(rtd_inl(WTST_0x2878) == 0xa5a5a5a5)
        //if (ui_is_AcOffPowerStatus_On())//Hardy20091203 for booting to AC-off power status before
        //    SET_POWERSWITCH();
        //else
            flow_power_off();
    }  
	// Initial LSADC
	
    drv_lsadc_Init();
	#if 1
	LED_RED_ENABLE(LED_ON);
    LED_GREEN_ENABLE(LED_OFF);
   // ISDB_POWER_ENABLE(_OFF);	
    fw_UserInput_PowerOff();
	#endif
	//ISDB_POWER_ENABLE(_OFF);
	#if CONFIG_ENABLE_USB
	drv_pcb_set_gpio_pin_isr(GPIO_IR_SEND,1);
	
	#endif
	#if CONFIG_TU300
	drv_pcb_set_gpio_pin_isr(GPIO_DTV_IR_SEND,1);
	#endif
	#if CONFIG_USB_CHECK
	STB_LOADINGPIN(1);
	CLR_MUTE(0);
	#endif
    //RTD_Log(LOGGER_INFO, "Sparrow Enter normal state,AcOffPowerStatus=%x\n",(UINT32)ui_is_AcOffPowerStatus_On());  
    do
    {
#ifdef CONFIG_TTX_SUPPORT
        extern xdata TTX_Handle m_hTtx;
#endif
        // No do when enter debug mode
        if (drv_fw_uart_handler())
        {
            continue;
        }
        // Check whether power switch to power on/off/saving
        if (!flow_power_Handler())
        {
            // Do timer event send from user
            //RTD_Log(LOGGER_INFO, "Sparrow fw_timer_event_Handler\n");
            fw_timer_event_Handler();
            continue;
        }

        // always kick watchdog, even it is not enabled
        //tick system WD, to avoid system reset
        flow_timer_Reset_WatchDog();

        // Do timer event send from user
        // RTD_Log(LOGGER_INFO, "Sparrow fw_timer_event_Handler\n");
        fw_timer_event_Handler();

#ifdef CONFIG_VBI_CC_SUPPORT
        CCcDecoder();
#endif

        // Do UI operation
        fw_osdcore_Handler();

        // Do mode state
        // RTD_Log(LOGGER_INFO, "Sparrow fw_modestate_Process\n");
        fw_modestate_Process();
		#if  CONFIG_ENABLE_USB
		 if(info->state == _MODE_STATE_ACTIVE)
	 	{
	 	     
			 if(GET_INPUTSOURCE_INDEX()== _SOURCE_INDEX_USB)
			 	{
			 		if(g_cnt_2)
		 			{
				 		 g_cnt_2--;	
						 #if ENABLE_OPENDISPLAY_CTL
						 if(g_cnt_2==4900)//4840
							Send_IR((((~IR_89JDVD_OPEN_CMD)<<8)|IR_89JDVD_OPEN_CMD));
						 #endif
						 if((g_cnt_2==0))
						 	{
			    		     Send_IR((((~IR_89JDVD_USB_SD)<<8)|IR_89JDVD_USB_SD));	
							 stUsbFlag_NO=1;
						 	}
		 			}
			 	}
			
	 	}
		
		if (g_cnt_1&&stUsbFlag_NO)
		{
			g_cnt_1--;
			if(g_cnt_1==0)
				{
			  		STB_LOADINGPIN(1);
					stUsbFlag_NO=0;
					
				}
				
		}
		
		 #endif
#ifdef CONFIG_TTX_SUPPORT
        if ( m_hTtx != NULL )	// TT is not opened.
            TTX_Tick();
#endif
        flow_audio_Handler();
	



    }
    while (1);
#if ID692_EN     
_mainend: 
	return;
#endif	
}

