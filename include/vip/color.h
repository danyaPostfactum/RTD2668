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

#include "sparrow.h"
#include "vip\vip_parameter_def.h"

#ifndef _COLOR_H
#define _COLOR_H

/*================================ Definitions ==============================*/
#define _RGB2YUV_m11				0x00
#define _RGB2YUV_m12				0x01
#define _RGB2YUV_m13				0x02
#define _RGB2YUV_m21				0x03
#define _RGB2YUV_m22				0x04
#define _RGB2YUV_m23				0x05
#define _RGB2YUV_m31				0x06
#define _RGB2YUV_m32				0x07
#define _RGB2YUV_m33				0x08
#define _RGB2YUV_Yo_even			0x09
#define _RGB2YUV_Yo_odd			0x0a
#define _RGB2YUV_Y_gain				0x0b
#define _RGB2YUV_sel_RGB			0x0c
#define _RGB2YUV_set_Yin_offset		0x0d
#define _RGB2YUV_set_UV_out_offset	0x0e
#define _RGB2YUV_sel_UV_off			0x0f
#define _RGB2YUV_Matrix_bypass		0x10
#define _RGB2YUV_Enable_Y_gain		0x11

#define _YUV2RGB_k11			0x00
#define _YUV2RGB_k13			0x01
#define _YUV2RGB_k22			0x02
#define _YUV2RGB_k23		 	0x03
#define _YUV2RGB_k32			0x04
#define _YUV2RGB_Roffset	 	0x05
#define _YUV2RGB_Goffset		0x06
#define _YUV2RGB_Boffset		0x07
#define _YUV2RGB_Y_Clamp		0x08
#define _YUV2RGB_UV_Clamp		0x09

#define _Gamma_Full				0x01
#define _Gamma_Compact		0x00

#define _ICM_ALL_DATA_HUE 	0
#define _ICM_ALL_DATA_SAT 	2
#define _ICM_ALL_DATA_ITN 	4
#define _ICM_ALL_DATA_SOFTCLAMP_CTRL 	6
#define _ICM_ALL_DATA_SOFTCLAMP_SLOPE 	7
#define _SOFTCLAMP_SAT_SLOP_EN	2
#define _SOFTCLAMP_ITN_SLOP_EN	1

#define _PANEL_BIT_6	0
#define _PANEL_BIT_8	1
#define _PANEL_BIT_10	2

#define DCR_THRESHOLD	150

typedef struct
{
    UINT8 R_Contrast;
    UINT8 G_Contrast;
    UINT8 B_Contrast;
    UINT8 R_Brightness;
    UINT8 G_Brightness;
    UINT8 B_Brightness;
} RGB_ColorTemp;

extern UINT16 xdata g_usHorBoundary;
extern UINT16 xdata g_usVerBoundary;
extern UINT16 xdata g_usRGBInfo;
extern UINT8 xdata g_ucPreBacklightPWM;
extern UINT8 xdata g_ucPreDCCLum;
extern UINT8 xdata g_ucDcrCtrl;
extern UINT8 xdata g_ucDCRPercent;
extern UINT8 xdata g_ucDcrTimer;
extern UINT8 xdata g_ucUserBacklight;

//--------------------------------------------------
#define VIP_I_DOMAIN 0
#define VIP_D_DOMAIN 1

//--------------------------------------------------
// DCR of Power Status
//--------------------------------------------------
#define GET_DCR_STATUS()                        ((bit)(g_ucDcrCtrl & _BIT1))
#define SET_DCR_STATUS()                        (g_ucDcrCtrl |= _BIT1)
#define CLR_DCR_STATUS()                        (g_ucDcrCtrl &= ~_BIT1)

#define GET_DCR_ULTRA_MIN()                     ((bit)(g_ucDcrCtrl & _BIT4))
#define SET_DCR_ULTRA_MIN()                     (g_ucDcrCtrl |= _BIT4)
#define CLR_DCR_ULTRA_MIN()                     (g_ucDcrCtrl &= ~_BIT4)

//--------------------------------------------------
// Definitions of Input Source
//--------------------------------------------------
#define _PWM_MIN            95
#define _PWM_LOW            135
#define _PWM_HIGH           180
#define _PWM_MAX            255
#define _ULTRA_MIN          40
#define _ACER_HIGH_ACM      _ON

//--------------------------------------------------
// Dymanic Contrast Ratio (DCR)
//--------------------------------------------------
#define _DCR_TYPE_OFF                           0
#define _DCR_TYPE_1                             1
#define _DCR_TYPE_2                             2
#define _DCR_TYPE_3                             3
#define _InputGamma_RGB_Same 1
#define _InputGamma_RGB_Sep 0
#define _DCR_MODE                               _DCR_TYPE_OFF

#define _VIVID_COLOR_FUCTION _ON

void drv_color_DcrMeasureStartHD( UINT16 disp_len, UINT16 disp_width, UINT8 osd_backlight);
void drv_color_DcrDcr1( UINT8 OSD_Backlight);
void drv_color_DcrDcr2(void);
void drv_color_DcrDcr3(void);
void drv_color_DcrSlowAdjust(UINT8 ucValue);
void drv_color_AdjustDCRFillDCCTable( UINT8 lamp_value);
void drv_color_AdjustNormalizeFactor(UINT16 usHWidth, UINT16 usVHeight);
void drv_color_DcrInitial(void);
void drv_color_Dcr5MinTimerCount(void);
void drv_color_FillDCCTable(UINT8 ucNum);
void drv_color_EnableDCC(UINT8 ucSelect, UINT8 Enable);
void drv_color_FillICMTable(UINT8 ucNum);
void drv_color_EnableICM(UINT8 ucSelect);
void drv_color_AdjustHue(UINT8  ucYPbPrhue);
void drv_color_AdjustSaturation(UINT8  ucYPbPrSaturation);
void drv_color_PeakingFilter(UINT8 ucPeaking); // 0~15

/*================================== Function ===============================*/
void drv_color_set422to444(bit enable);
void drv_color_colorspacergb2yuvtransfer(UINT8 nSrcType, UINT8 nSD_HD, UINT8 not_HDMIMODE_NEW);
void drv_color_colorspaceyuv2rgbtransfer(UINT8 nSrcType);
void drv_color_setrgbcontrast( UINT8 Red, UINT8 Green, UINT8 Blue);
void drv_color_setrgbbrightness(UINT8 Red, UINT8 Green, UINT8 Blue);
void drv_color_setycontrastbrightness( UINT8 Contrast, UINT8 Brightness);
void drv_color_setydlti(bool domain, UINT8 Level);
void drv_color_setcdcti(bool domain, UINT8 Level);
void drv_color_setrgbcontrast( UINT8 Red, UINT8 Green, UINT8 Blue);
void drv_color_setrgbbrightness(UINT8 Red, UINT8 Green, UINT8 Blue);
//void drvif_color_setdithering(void);
void drv_color_setdithering(UINT8 panel_bit);
void drv_color_setuv_sat( UINT8 Sat);
void drv_color_highlightwindow_config(void);
void drv_color_sethighlightwindow(UINT8 Ena_bit, UINT16 HSTA, UINT16 VSTA, UINT16 HEND, UINT16 VEND);
void drv_color_setgamma( UINT8 Mode);
void drv_color_setgamma_s( UINT8 Mode, UINT8 s_curve_en);

void drvif_color_icm_table_select(UINT8 which_table);
bit drvif_color_VIP_Quality_Coef_GetValue(UINT8 which_source, VIP_QUALITY_COEF code ** pptr);
void drv_color_set_inputgamma( UINT8 enable, UINT8 mode);
UINT16 drv_color_S_Curve(UINT32 input_g_point ,UINT32 g_index);
void drv_color_system_Init(void);
#endif // #ifdef _COLOR_H

