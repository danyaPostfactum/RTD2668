/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2006
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is for power related functions.
 *
 * @author 	$Author$
 * @date	$Date$
 * @version	$Revision$
 * @ingroup	power
 */

/**
 * @addtogroup power
 * @{
 */

/*============================ Module dependency  ===========================*/
#include "power.h"
#include "ui_ctrl.h"
#include "timer_event.h"
#include "timer\timer.h"
#include "sparrow.h"
#include "panel\panel_struct.h"
#include "panel\panel_library.h"
#include "message.h"
#include "ui_ctrl.h"
#include "ap_rtirkey.h"
#include "pcb\pcb_utility.h"
#include "pcb_sys.h"
#include "pcb_customer.h"
#include "video\video.h"
#include "scaler\scaler.h"
#include "scaler_ddomain\scaler_display.h"
#include "scaler_idomain\adc.h"
#include "osdcore.h"
#include "hdmi\hdmifun.h"
#include "audio\audio_api.h"
#include "modestate.h"
#include "sparrow_utility.h"
#include "ui_struct.h"
#include "rt_storage.h"
#include "i2c\i2c.h"
#include "audio\audio_api.h"
#include "ifd\ifd.h"
#include "userinput.h"
#include "signal\sig_vga.h"
#include "autoconf.h"
#include "signal\sig_avsv.h"
#include "irrc\irrc.h"
#include "flash/spi_flash.h"

#ifdef CONFIG_TTX_SUPPORT
#include "ttx.h"
#endif

#ifdef CONFIG_VBI_CC_SUPPORT
#include "vbi/slr.h"
#include "closecaption.h"
#endif

UINT8 xdata ucPowerControl = 0x00;
 BOOL  ucPowersavingdisablelogo = 0;
extern UINT8 xdata fFirstPowerOn;
extern UINT8 Flash_protect_on;
extern	UINT8 xdata ucSleepType;
extern UINT8 g_dimmer;

extern UINT8 g_power_flag;
extern UINT8 g_power_dtvflag;

/*===================================  Types ================================*/
void drv_spi_flash_Protect(UINT8 enable);
void drv_sparrow_power_off_misc(bit on_off);
/*================================== Function ================================*/
/**
 * flow_power_handler
 * Execute power up process
 */

BOOL  flow_power_Handler(void)
{
    UINT16 getKeyVal=0;
	UINT8 xdata detect_res111;
    if (GET_POWERSTATUS() == POWER_STATUS_POWER_ON)
    {
//		RTD_Log(LOGGER_INFO, "POWER_STATUS_POWER_ON\n");
        // Not yet accept power switch and when power on status
        if (!GET_POWERSWITCH() && !GET_POWERSAVING_SWITCH())
            return _TRUE;

        // Accept power switch and enter power off
        if (GET_POWERSWITCH())
        {
            // set power off in status
            SET_POWERSTATUS(POWER_STATUS_POWER_OFF);

            RTD_Log(LOGGER_INFO, "Enter the process in Power Off ...\n");

            flow_power_off();
            ucSleepType = 0;
			//fw_timer_event_DelayXms(5000);
            RTD_Log(LOGGER_INFO, "Power Off is complete...\n");

            // Complete power switch process
            CLR_POWERSWITCH();
            fw_ir_message_clear();

            fw_ui_message_clearallType(_POWER_KEY_MESSAGE);
        }
        else if (GET_POWERSAVING_SWITCH())
        {
            // set power saving in status
            SET_POWERSTATUS(POWER_STATUS_POWER_SAVING);

            RTD_Log(LOGGER_INFO, "Enter the process in Power Saving ...\n");

            flow_power_Saving_off();

            // Change HS/VS to GPIOC4/GPIOC5
            rtd_outb(PC_PS1_0x2d39, 0x11);

            // Complete power saving switch process
            CLR_POWERSAVING_SWITCH();

            RTD_Log(LOGGER_INFO, "the process of Power Saving is complete\n");
        }

        return _FALSE;
    }

    if (GET_POWERSTATUS() == POWER_STATUS_POWER_OFF)
    {
        getKeyVal = fw_ir_message_pop();
			
        if (getKeyVal==0 || (getKeyVal != _POWER_KEY_MESSAGE))
          	  getKeyVal = fw_ui_message_pop();
        	
        if ((getKeyVal == _POWER_KEY_MESSAGE) || GET_POWERSWITCH())
        {


			 //RTD_Log(LOGGER_INFO, "melon22222\n");
            // set power on in status
            SET_POWERSTATUS(POWER_STATUS_POWER_ON);

            flow_power_on();

            // Complete power switch process
            CLR_POWERSWITCH();

            fw_ui_message_clearallType(_POWER_KEY_MESSAGE);

            return _TRUE;
        }
		
		
    }
    else if (GET_POWERSTATUS() == POWER_STATUS_POWER_SAVING)  		// POWER_STATUS_POWER_SAVING
    {
        getKeyVal = fw_ir_message_pop();
        if (getKeyVal==0)// || (getKeyVal != _POWER_KEY_MESSAGE) || (getKeyVal != _POWER_KEY_MESSAGE))
            getKeyVal = fw_ui_message_pop();

        if ((getKeyVal == _POWER_KEY_MESSAGE) || GET_POWERSWITCH())
        {
            // Change GPIOC4/GPIOC5 to HS/VS
            rtd_outb(PC_PS1_0x2d39, 0x00);

            SET_POWERSTATUS(POWER_STATUS_POWER_OFF);

            RTD_Log(LOGGER_INFO, "Enter from Power Saving to Power Off\n");

            return _FALSE;
        }
		detect_res111 = flow_source_DetectMode();
       // RTD_Log(LOGGER_INFO, "detect_res111 = %x\n",(UINT32)detect_res111);
		if(drv_hsyncdetect_powersaving() || getKeyVal || (detect_res111 == _MODE_SUCCESS))
		{
            RTD_Log(LOGGER_INFO, "GET HS/VS signal in power saving\n");

            // set power on in status
            SET_POWERSTATUS(POWER_STATUS_POWER_ON);

            flow_power_on();

            // Change GPIOC4/GPIOC5 to HS/VS
            rtd_outb(PC_PS1_0x2d39, 0x00);

            // Complete power saving switch process
            CLR_POWERSAVING_SWITCH();

            fw_ui_message_clearallType(_POWER_SAVING_MESSAGE);

            return _TRUE;
        }
    }
    return _FALSE;
}
/**
 * drvif_power_on
 * Execute power up process
 */
void  flow_power_on(void)
{
    RTD_Log(LOGGER_INFO, "Power On ...\n");
    #if defined(CONFIG_VBI_CC_SUPPORT)
    bCheckCCStatus = 0;
	#endif
    // Release reset & Enable clock for each ip
    drv_sparrow_PowerOn();

    // Turn on watchdog
    drv_timer_Watchdog_Enable(_TRUE);

    drv_adc_ADC_BangGapPowerControl(_ON); //HPWang add

    drv_sparrow_power_off_misc(_ON); //HPWang add

    // Initial pwm configuration
    RTD_Log(LOGGER_INFO, "drv_pwm_Init ...\n");
    drv_pwm_Init();

    // Initial I2c
    RTD_Log(LOGGER_INFO, "drv_i2c_Init ...\n");
    drv_i2c_Init();

    // Initial LSADC
    RTD_Log(LOGGER_INFO, "drv_lsadc_Init ...\n");
    drv_lsadc_Init();

    // Write HDCP key into HDMI ip
//	RTD_Log(LOGGER_INFO, "Write HDCP Key to hdmi module...\n");
//	Hdmi_WriteHDCPKey();

    // use cpu PLL
    drv_sparrow_CPU_Switch_Clock(_TRUE);

    LED_RED_ENABLE(LED_OFF);
    LED_GREEN_ENABLE(LED_ON);
    //STB_CTRL(1);
    #if CONFIG_ENABLE_USB
	STB_DVDPWR_CTRL(1);
	#endif
   // RTD_Log(LOGGER_LEVEL,"\n poweron Tim bCheckCCStatus =%d \n",(UINT32)bCheckCCStatus);

    //frank@0416 Power on setting according to customer's PCB
    pcb_ctrl_PowerOn();

    SET_FIRST_POWER_SWITCH();
    fFirstPowerOn = 0;
    // Initial scaler settings
//	drv_scaler_poweron_Init();

    // Init display domain
    drv_scalerdisplay_Init();

#if defined( CONFIG_PVR_ENABLE)
    //Set PVR_ON_OFF(HDMI_CEC) pin to power on PVR Module
    PVR_PowerOnOffModule(PVR_PowerOn);
#endif

#if defined (CONFIG_USB_SOURCE_ENABLE)
    rtd_outl(0xb8020800,0x80);
    //	rtd_outl(0xb802080c,0x1f);	//set to 3.3V
#ifdef CONFIG_2670_PCB_HANNSTAR_VER1

#else
    PVR_SWITCH_ENABLE(_TRUE);
#endif
#endif
    fw_timer_event_CancelAllTimerEvent();

    // Initial modestate
    fw_modestate_Init();

    fw_flow_message_clearall();

    // Send Init message
    fw_flow_message_push(_UI2FL_MAIN_INIT_SRC_EVENT);

    fw_osdcore_Initialze();
    #ifdef CONFIG_ENABLE_HDMI
    drvif_Hdmi_Init();
	#endif
    drv_sig_vga_edid_init();

    // Initial Audio
    Audio_Init_Step1();
    Audio_Init_Step2();
	
    drv_panel_PowerPanelOn();

    // Reset Watchdog avoid to triggle
    flow_timer_Reset_WatchDog();
    ui_PowerOn_Proc();

    drv_adc_ADCPowerControl(_TRUE);

#if !defined(CONFIG_RTD2668_PCB1_QC_VER1)
    // Initial user input
    fw_UserInput_Init();
#endif
    // Turn on watchdog
    // drv_timer_Watchdog_Enable(_TRUE);
    flow_timer_Reset_WatchDog();
  

#ifdef CONFIG_VBI_CC_SUPPORT
    drvif_vbi_cc_verify_init();
#endif
    ui_set_AcOff_powerStatus(POWER_STATUS_POWER_ON);//Hardy20091203 for booting to AC-off power status before


#ifdef CONFIG_WSS_SUPPORT
    drvif_vbi_init_wss();
#endif

#if(IR_DETECTMODE == _IR_DS_SW)
	fw_IR_detectmode_change(_IR_DS_SW);
#endif

	if(Flash_protect_on == _FALSE)
		drv_flash_protect(_ON);

    fw_ui_message_push(_DISPLAY_MESSAGE);//jokerxie power 起来的时候显示一次通道
	g_dimmer=0;
	g_power_flag=1;
	g_power_dtvflag=1;
    RTD_Log(LOGGER_INFO, "Power On Init success...\n");
    #if 0//CONFIG_TU300
   if(GET_INPUTSOURCE_INDEX()== _SOURCE_INDEX_AV_1)
   	Send_IR_DTV((((~TV_POWER_CODE)<<8)|TV_POWER_CODE));
    #endif
}

/*============================================================================*/
/**
 * drvif_power_off
 * Execute power down process
 */
#ifdef CONFIG_TTX_SUPPORT
extern void TTX_PowerOff();
#endif

extern void COsdFxDisableOsd(void);

void flow_power_off(void)
{
#if CONFIG_TU300
	 Send_IR_DTV((((~TV_POWER_CODE)<<8)|TV_POWER_CODE));
#endif
    // Turn off watchdog
    drv_timer_Watchdog_Enable(_FALSE);
    

#ifdef CONFIG_STORAGE_ACCESS_FLASH
    // Enable Protect from write for flash
    // drv_spi_flash_Protect(_TRUE);
#endif

#ifdef CONFIG_TTX_SUPPORT
    TTX_PowerOff();
#endif

    // Power off For customer
    ui_PowerOff_Proc();

    // MUTE function
    Audio_HwpSetMute(_ENABLE, ADC_CH_ALL, ADC_SYS_MUTE);
	AUDIO_AMP_MUTE_ENABLE(AUDIO_AMP_MUTE);


    fw_timer_event_DelayXms(600);//Hardy20091207 for avoid pop noise when power off


#ifndef CONFIG_RT_STORAGE
//  CStorageUpdateStructData();
#endif
    COsdFxDisableOsd();
    // Power down process
    drv_panel_PowerPanelOff();

    LED_RED_ENABLE(LED_ON);
    LED_GREEN_ENABLE(LED_OFF);
    //ISDB_POWER_ENABLE(_OFF);
    //STB_CTRL(0);
	STB_IR_EN(1);
	#if CONFIG_ENABLE_USB
	STB_DVDPWR_CTRL(0);
	#endif

    #ifdef CONFIG_VBI_CC_SUPPORT
	CCcDisable();
	#endif
    //RTD_Log(LOGGER_LEVEL,"\n poweroffdf Tim bCheckCCStatus =%d \n",(UINT32)bCheckCCStatus);
	//COsdCoreExitOsd();

    // Force write rt-storage to flash
    flow_storage_WriteBackup2Flash();

    flow_mode_PoweroffProc();

    // Release source
    flow_source_Release();

    // Remove all event
    fw_timer_event_CancelAllTimerEvent();

    // Remove all message
    fw_ui_message_clear();

    // disable ADCAPLL
    drv_adc_ADCPowerControl(_OFF);

    // Disable VDC
    drv_video_PowerOn(_OFF);

    // disable pwm module
    drv_pwm_PowerOn(_OFF);

#ifdef CONFIG_STORAGE_ACCESS_FLASH	// marked by weihao because of china
    // disable i2c module
    drv_i2c_PowerOn(_OFF);

	//WT5700_init();

#endif
#ifdef CONFIG_ENABLE_HDMI
    // disable hdmi module
    Hdmi_Power(_OFF);
#endif
    // disable lsadc module
    drv_lsadc_PowerOn(_ON);

    // disable audio module
    drv_audio_PowerOn(_OFF);

    // disable IFD module
    drvif_ifd_power_on(_OFF);

    // Switch cpu clock to crystal
    drv_sparrow_CPU_Switch_Clock(_FALSE);

    drv_sparrow_power_off_misc(_OFF); //HPWang add

    pcb_ctrl_PowerOff();
	    

#if 1//!defined(CONFIG_RTD2668_PCB1_QC_VER1)
    fw_UserInput_PowerOff();
#endif

#if(IR_DETECTMODE == _IR_DS_SW)
	fw_IR_detectmode_change(_IR_RTD_HW);
#endif
//  drv_adc_ADC_BangGapPowerControl(_OFF); //HPWang add
    drv_sparrow_PowerOff();
    ui_set_AcOff_powerStatus(POWER_STATUS_POWER_OFF);//Hardy20091203 for booting to AC-off power status before

	if(Flash_protect_on == _FALSE)
		drv_flash_protect(_ON);
    RTD_Log(LOGGER_INFO, "Power Off Init success...\n");

}
void flow_power_Saving_off(void)
{
    // Turn off watchdog
    drv_timer_Watchdog_Enable(_FALSE);
   ucPowersavingdisablelogo = 1;
#ifdef CONFIG_STORAGE_ACCESS_FLASH
    // Enable Protect from write for flash
    // drv_spi_flash_Protect(_TRUE);
#endif

#ifdef CONFIG_TTX_SUPPORT
    TTX_PowerOff();
#endif

    // Power off For customer
    ui_PowerOff_Proc();

    // MUTE function
    Audio_HwpSetMute(_ENABLE, ADC_CH_ALL, ADC_SYS_MUTE);
    fw_timer_event_DelayXms(200);//Hardy20091207 for avoid pop noise when power off
    AUDIO_AMP_MUTE_ENABLE(AUDIO_AMP_MUTE);

#ifndef CONFIG_RT_STORAGE
//	CStorageUpdateStructData();
#endif
    // Power down process
    drv_panel_PowerPanelOff();

    //LED_RED_ENABLE(LED_OFF);
    //LED_GREEN_ENABLE(LED_OFF);
	
 //if (GET_INPUTSOURCE_TYPE() != _SOURCE_TV)

    //TUNER_POWER_ENABLE(_OFF);


    flow_storage_WriteBackup2Flash();

    fw_timer_event_CancelAllTimerEvent();

    // Remove all message
    fw_ui_message_clear();




#if !defined(CONFIG_RTD2668_PCB1_QC_VER1)
    fw_UserInput_PowerOff();
#endif

    ui_set_AcOff_powerStatus(POWER_STATUS_POWER_SAVING);//Hardy20091203 for booting to AC-off power status before

	if(Flash_protect_on == _FALSE)
		drv_flash_protect(_ON);
    RTD_Log(LOGGER_INFO, "Power Saving Init success...\n");

}



/*======================== End of File =======================================*/
/**
*
* @}
*/

