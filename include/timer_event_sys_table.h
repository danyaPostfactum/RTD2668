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
#include "sparrow.h"

/*===================================  Types ================================*/
#ifndef _TIMER_EVENT_SYS_TABLE_H
#define _TIMER_EVENT_SYS_TABLE_H
void timerevent_table_power_saving_event(void);
void timerevent_table_no_signal_event(void);
void timerevent_table_no_support_event(void);
void timerevent_table_no_cable_event(void);
void timerevent_table_audio_atv_freq_scan_event(void);
void timerevent_table_audio_atv_nicam_check_event(void);
void timerevent_table_Periodic_signal_event(void);
void timerevent_table_AutoAdjust_event(void);
void timerevent_table_power_first_event(void);
void ResetPowerSavingTime(void);

#if defined(fUseCPUSwitchEnable)
void timerevent_table_Switch_CPU_Clock(void);
#endif

#ifdef CONFIG_ENABLE_FHFVBug
void timerevent_CheckTVFHFVEvent(void);
#endif

void timerevent_table_flash_protect(void);
void timerevent_table_showloading(void);
void timerevent_table_loadmainfont(void);

#endif //#ifndef _TIMER_EVENT_SYS_TABLE_H
