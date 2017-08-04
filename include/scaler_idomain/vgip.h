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
#ifndef _VGIP_H
#define _VGIP_H
#include "sparrow.h"

#define _PROGRAM_HDELAY                 	0 // 10   CSW* modify 0961004 to adjust IHS delay in window H position

enum
{
    VGIP_SRC_TMDS = 0,	//HDMI
    VGIP_SRC_ADC,		//ADC/YPbPr/Pattern Gen
    VGIP_SRC_ATV,		//From Dragon
    VGIP_SRC_V10,		//From plug-in vdc1
    VGIP_SRC_Reserved,		//From plug-in vdc2
    VGIP_SRC_JPEG,		//From plug-in vdc
};

enum
{
    VGIP_MODE_ANALOG = 0,
    VGIP_MODE_DIGITAL
};
// set default to vgip
void drv_vgip_Set_Default(void);
void drv_vgip_set_vgip_capture_shift(INT8 shiftx, INT8 shifty);
void drv_vgip_set_vgip(UINT8 src, UINT8 mode);
void drv_vgip_set_vgip_capture(void);
//void drv_vgip_disablevgip(void);

void drv_vgip_set_sample(UINT8 enable);

// Set delay value in VGIP
void drv_vgip_SetVSDelay(UINT16 val);
UINT16 drv_vgip_GetVSDelay(void);

void drv_vgip_adjust_ihs_delay(UINT16 usIHSDelay);
void drv_vgip_adjust_ivs_delay(UINT16 usIVSDelay);
void drv_vgip_cal_vgip_capture(void);

#endif // #ifndef _VGIP_H
