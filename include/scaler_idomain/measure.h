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
#ifndef _MEASURE_H
#define _MEASURE_H

#include "sparrow.h"
#include "mode.h"

UINT8 drv_measure_onlinemeasure(void);
void debug_temp(void);

UINT8 drv_measure_offlinemeasure(UINT8 source, UINT8 mode, ModeInformationType* stModeInfo);
void drv_measure_enableonlinemeasure(void);
void drv_measure_Disable_OnMeasure(void);
//void drv_measure_Disable_OnMeasure_Watchdog(void);
UINT16 drv_measure_HighPulsePeriod(void);
//void drv_measure_Clear_OnMeasure_Status(void);
bit drv_measure_onlinemeasure_Isr(void);
void drv_measure_onlinemeasure_Interrupt_Init(void);
UINT8 mode_identifymode(UINT8 modeno);
#ifdef CONFIG_ENABLE_HDMI
void mode_setHDMIinfo(UINT8 modeno);
#endif
UINT8 mode_searchmode(void);
UINT8 flow_mode_detectmode(UINT8 source);
void mode_setinfofromtable(UINT8 modeno);
UINT8 mode_judge_confuse_mode(UINT8 mode_index,UINT16 usHighPulsePeriod);

#endif // #ifndef _MEASURE_H
