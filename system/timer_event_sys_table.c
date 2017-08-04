/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2006
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is for timer related functions.
 *
 * @author 	$Author$
 * @date		$Date$
 * @version	$Revision$
 * @ingroup	timer
 */

/**
 * @addtogroup timer
 * @{
 */

/*============================ Module dependency  ===========================*/
#include "timer_event_sys_table.h"
#include "timer_event.h"
#include "modestate.h"
#include "message.h"
#include "osd_sys_msg.h"
#include "audio\audio_api.h"
#include "rosprintf.h"
#include "sparrow\sparrow_utility.h"
#include "rt_storage.h"
#include "video\video.h"
#include "power.h"
#include "source.h"
#include "autoconf.h"
#include "flash/spi_flash.h"

/*=================================== Types ================================*/
UINT8 PowerSavingTime = 10;

void ResetPowerSavingTime(void)
{
    PowerSavingTime = 10;
}

void power_saving_event(void)
{
    fw_ui_message_clear();
    fw_flow_message_clearall();
    if (GET_INPUTSOURCE_TYPE() != _SOURCE_TV)
        ;//SET_POWERSAVING_SWITCH();//MELON
 //   else
   //     SET_POWERSWITCH();
}

/*=================================== function ================================*/
void timerevent_table_power_saving_event(void)
{
#if 1
    if (GET_INPUTSOURCE_TYPE() != _SOURCE_TV)
        power_saving_event();
    else
    {
        --PowerSavingTime;

        if (PowerSavingTime<=1)
        {
            ResetPowerSavingTime();
            power_saving_event();
        }
        else
            fw_timer_event_ReactiveTimerEvent(SEC(30),timerevent_table_power_saving_event);
    }
#else
    fw_ui_message_clear();
    fw_flow_message_clearall();
    if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
        SET_POWERSAVING_SWITCH();
    else
        SET_POWERSWITCH();
#endif
}

void timerevent_table_no_signal_event(void)
{
    fw_ui_message_push(_FL2UI_MAIN_NO_SIGNAL);
}

void timerevent_table_no_support_event(void)
{
    fw_ui_message_push( _FL2UI_MAIN_NO_SUPPORT);
}

void timerevent_table_no_cable_event(void)
{
    fw_ui_message_push(_FL2UI_MAIN_NO_CABLE);
}

void timerevent_table_Periodic_signal_event(void)
{
    fw_flow_message_push(_FL2FL_MAIN_STATE_PERIODIC_EVENT);
}
void timerevent_table_AutoAdjust_event(void)
{
    fw_flow_message_push(_FL2FL_ADC_AUTOADJUST_START);
}

#ifdef _VIDEO_TV_SUPPORT
void timerevent_table_audio_atv_freq_scan_event(void)
{
    if (Audio_AtvTimerEventFwScan())
        fw_timer_event_ActiveTimerEvent(50, timerevent_table_audio_atv_freq_scan_event);
}
void timerevent_table_audio_atv_nicam_check_event(void)
{
    Audio_AtvTimerEventNicamCheck();
}
#endif
#ifdef CONFIG_STORAGE_ACCESS_FLASH
void timerevent_table_rt_storage_sync_event(void)
{
    flow_storage_WriteBackup2Flash();
}
#endif
void timerevent_table_power_first_event(void)
{
    CLR_FIRST_POWER_SWITCH();
}
#ifdef _VIDEO_TV_SUPPORT
void drvif_ifd_set_agc_variance_on(void);
void timerevent_atv_ifd_chk_agc_var(void)
{
    drvif_ifd_set_agc_variance_on();
}
#endif
#ifdef CONFIG_ENABLE_FHFVBug
void timerevent_CheckTVFHFVEvent(void)
{
    drv_video_FhBug();
    //$$ScalerTimer_ActiveTimerEvent(SEC(0.03), ModeState_CheckTVFHFVEvent);// need to check every 20~30ms
    fw_timer_event_ReactiveTimerEvent(300, timerevent_CheckTVFHFVEvent);// need to check every 20~30ms
}
#endif

/*********************************************************************
** 函数名称: void timerevent_table_flash_protect(void)
** 功能描述: flash写保护
** 输　  入: 无
** 输　  出: 无 
**********************************************************************/
void timerevent_table_flash_protect(void)
{
	drv_flash_protect(_ON);
}

void timerevent_table_showloading(void)
{
	fw_ui_message_push(_FL2UI_SHOW_LOGO);
}

void timerevent_table_loadmainfont(void)
{
	fw_ui_message_push(_FL2UI_LOAD_MAINFONT);
}

