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
#ifndef _FLOW_DISPLAY_H
#define _FLOW_DISPLAY_H

/*============================ Module dependency  ===========================*/
#include "sparrow.h"
#include "scaler_ddomain/scaler_display.h"
/*
typedef struct {
	UINT8	nR_Value;
	UINT8	nG_Value;
	UINT8	nB_Value;
} _STRUCT_BACKGROUND_COLOR;

extern _STRUCT_BACKGROUND_COLOR xdata stBackgroundColor;*/

#define flow_display_BackgroundColor(r,g,b)	drv_scalerdisplay_set_bg_color(r,g,b)
#define flow_display_ForceBackground(en)	drv_scalerdisplay_ForceBackground(en)
#define flow_display_FreeRun_Mode()		drv_scaler_set_display_mode(0)
#define flow_display_Framesync_Mode()		drv_scaler_set_display_mode(1)
#define flow_display_Disable_FS_Watchdog()	 drv_scaler_enable_fs_wdg(0);
#define flow_display_Enable_FS_Watchdog()	 drv_scaler_enable_fs_wdg(1);
#define flow_display_Disable_Watchdog()	drv_scaler_enable_ddomain_wdg(0);
#define flow_display_Enable_Watchdog()	drv_scaler_enable_ddomain_wdg(1);

/*============================================================================*/
/**
 * flow_display_Handler
 * setup d-domain handler
 *
 * @param <void>	{ void }
 * @return 			{ void }
 *
 */
void flow_display_Handler(void);

/*============================================================================*/
/**
 * flow_display_ForceBackground
 * setup d-domain handler
 *
 * @param <void>	{ void }
 * @return 			{ void }
 *
 */
//void flow_display_ForceBackground(bit enable);

/*============================================================================*/
/**
 * flow_display_Disable_Watchdog
 * Disable Watch enable in D-domain
 *
 * @param <void>	{ void }
 * @return 			{ void }
 *
 */
//void flow_display_Disable_Watchdog();

/*============================================================================*/
/**
 * flow_display_Enable_Watchdog
 * Enable Watch enable in D-domain
 *
 * @param <void>	{ void }
 * @return 			{ void }
 *
 */
//void flow_display_Enable_Watchdog();

/*============================================================================*/
/**
 * flow_display_FreeRun_Mode
 * set Free Run mode
 *
 * @param <void>	{ void }
 * @return 			{ void }
 *
 */
//void flow_display_FreeRun_Mode(void);

/*============================================================================*/
/**
 * flow_display_Framesync_Mode
 * set Framesync mode
 *
 * @param <void>	{ void }
 * @return 			{ void }
 *
 */
//void flow_display_Framesync_Mode(void);

/*============================================================================*/
/**
 * fw_scalerdisplay_get_display_size
 * Set display window size, it must considers aspect ratio and pip.
 * This function will decide the info->DispLen and info->DisdWid
 *
 * @param { none }
 * @return none
 *
 */
void flow_display_Get_DisplaySize(void);



#endif // #ifdef _FLOW_DISPLAY_H
