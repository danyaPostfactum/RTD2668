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
#ifndef _ABL_H
#define _ABL_H

#define _TARGET_VALUE  (8 >> 2) // 4~64 and value should be the times of 4

//void drv_abl_VGA_Setting_On(UINT16 Hstart);
void drv_abl_YPbPr_Setting_On(void);

void drv_abl_YPbPr_Setting_Off(void);
void drv_abl_YPbPr_Setting_Finetune(void);
void drv_abl_YPbPr_Control(UINT8 h_start, UINT8 h_width, UINT8 v_start, UINT16 v_end);
void drvif_abl_disable(void);
void drvif_abl_setting_on(void);
void drvif_abl_enable(void);



#endif // #ifndef _ABL_H
