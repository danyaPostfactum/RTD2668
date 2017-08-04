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
#ifndef _FLOW_COLOR_H
#define _FLOW_COLOR_H

#include "sparrow.h"

void flow_color_DCR();

void flow_color_Transform_Format(void);

void flow_color_ultrazoom_SetScalingDown(void);

void flow_color_ultrazoom_SetScalingUp(void);

void flow_color_Handler(void);

void flow_color_Video_Quality_Handler(void);

UINT8 flow_vip_Get_Input_Mode(void);

#endif // #ifdef _COLOR_H
