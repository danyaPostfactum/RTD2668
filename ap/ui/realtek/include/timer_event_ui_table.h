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
#ifndef _TIMER_EVENT_UI_TABLE_H
#define _TIMER_EVENT_UI_TABLE_H
void timerevent_ui_table_OSD_timeout_event(void);
void timerevent_ui_table_OK_event(void);


void COsdCoreTimeoutEvent(void);
void COsdCoreIconAnimationTimerEvent(void);
void COsdCoreNoSignalEvent(void);
void COsdCoreNoSupportlEvent(void);
#ifdef CONFIG_TTX_SUPPORT
void timerevent_ui_table_OSD_NoTT_event(void);
#endif

#ifdef CONFIG_VBI_CC_SUPPORT
void COsdEventCloseVbi(void);
#endif

#endif //#ifndef _TIMER_EVENT_UI_TABLE_H
