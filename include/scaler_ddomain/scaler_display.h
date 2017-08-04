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
#include "sparrow.h"
/**
* @addtogroup
* @
*/
#ifndef _SCALER_DISPLAY_H
#define _SCALER_DISPLAY_H
void drv_scalerdisplay_set_bg_color(UINT8 ColorR, UINT8 ColorG, UINT8 ColorB);
void drv_scalerdisplay_set_dbuffer_en(BOOL En);
void drv_scalerdisplay_ForceBackground(BOOL Enable);
void drv_scalerdisplay_Init(void);
void drv_scalerdisplay_set_timing(void);
void drv_scalerdisplay_DVST_reset(void);
void drv_scalerdisplay_set_main_ctrl(void);
//void drv_scalerdisplay_enable_display(BOOL bEnable);
void drv_scalerdisplay_get_display_size(void);
//void drv_scalerdisplay_handler(void);

#endif // #ifndef _SCALER_DISPLAY_H
