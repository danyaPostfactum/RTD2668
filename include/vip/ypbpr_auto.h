/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2006
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is for color related functions.
 *
 * @author 	$Author$
 * @date 	$Date$
 * @version $Revision$
 */

/**
 * @addtogroup color
 * @{
 */

#ifndef _YPBPR_AUTO_H
#define _YPBPR_AUTO_H

#include "sparrow.h"
#include "source.h"
#include "flow_struct.h"
/*============================ Module dependency  ===========================*/


/*===================================  Types ================================*/

/*================================== Variables ==============================*/
#define AutoColor_Y  1
#define AutoColor_U  0
#define AutoColor_V  2
#define R_ADC_Dragon 0
#define G_ADC_Dragon 1
#define B_ADC_Dragon 2
#define imoffset 1
#define imgain 0
#define ClrBR75 75
#define ClrBR100 100
/*================================ Definitions ==============================*/

typedef struct
{
    UINT8   Y_RGB_idx;//R=0,G=1,B=2
    UINT8   U_RGB_idx;//R=0,G=1,B=2
    UINT8   V_RGB_idx;//R=0,G=1,B=2
} YUV_RGB_ADC;

typedef struct
{
    UINT16   h_start;
    UINT16   h_end;
    UINT16   v_start;
    UINT16   v_end;
} TVColorBar;

#define _SCART_R 0
#define _SCART_G 1
#define _SCART_B 2


/*================================== Function ===============================*/
void drvif_color_write_adc_dragon(UINT16 ADC_Data, UINT8 if_offset, UINT8 RGB_ADC);
//void HistPos( UINT16 *Active_Region, UINT16 *Offset_Pos, UINT16 *Y_Gain_Pos, UINT16 *PbPr_Gain_Pos);
void  drvif_color_smartfit_windows( UINT16 *Active_Region, UINT16 *Offset_Pos, UINT16 *Y_Gain_Pos, UINT16 *PbPr_Gain_Pos);
void drvif_color_adc_swap_check(YUV_RGB_ADC *Swap);
void drvif_color_write_smartfit_dragon( UINT16 Hstart, UINT16 Hend, UINT16 Vstart,UINT16 Vend, UINT8 Ch);
#if 0
void drvif_color_adjust_loop(ADCGainOffset *pt_info,UINT16 OffsetTarget, UINT16 GainTarget, UINT16 *OffsetPos, UINT16 *GainPos, UINT16 OffsetData, UINT16 GainData, UINT8 RGB_idx, UINT8 Channel);
#endif
void drvif_color_adjust_loop_offset_off(ADCGainOffset *pt_info,UINT16 OffsetTarget, UINT16 GainTarget, UINT16 *OffsetPos, UINT16 *GainPos, UINT16 GainData, UINT8 RGB_idx, UINT8 Channel);
UINT8 drvif_color_activeregion_dragon(UINT16 *Region);
UINT16 drvif_color_ypbpr_adc_init(ADCGainOffset * ADC);
UINT16 drvif_color_adc_adjust_dragon(UINT16 StartH,UINT16 EndH, UINT16 StartV, UINT16 EndV, UINT16 Target, UINT16 ADC_Data, UINT8 if_Offset, UINT8 RGB_ADC, UINT8 HistCh);
UINT16 drvif_color_finddominant_smartfit_dragon(UINT16 HStart, UINT16 HEnd, UINT16 VStart, UINT16 VEnd, UINT8 HistCh);
UINT16 drvif_color_ypbpr_adjustment_dragon(ADCGainOffset * ptADC_info);
UINT16 drvif_color_ypbpr_adjustment_dragon_bartype(ADCGainOffset * ptADC_info, UINT8 bartype);
void drvif_color_cypbprporch_levelcheck(ADCGainOffset * ptADC_info);
UINT8 drvif_color_dragon_colobar_dectector(UINT8 Block_Vsize);
UINT8 drvif_color_dragon_autocolor_colobar_dectector(UINT8 Block_Vsize);
UINT8 drvif_color_dragon_profile_dectector(UINT8 Block_Vsize,  UINT16 *H_Block, UINT16 *V_Block, UINT16 *Profile_transition_sum);
/*============================================================================*/
UINT16 drvif_color_SCARTrgb_adjustment(ADCGainOffset * ptADC_info);
void drv_video_SCART_ADC_adjust_gain( UINT8 RGB_idx, UINT16 ADC_Data);

//UINT16 drvif_color_SCARTrgb_adjustment_mayberight(ADCGainOffset * ptADC_info);
//UINT16 drvif_color_adc_SCARTrgb_adjust_inverse(UINT16 *POS, UINT16 *Target, UINT16 ADC_Data, UINT8 RGB_ADC, UINT8 HistCh);

/**
 *
 *
 *
 * @param <???> {comment}
 * @param <???> {comment}
 */
#endif	//#ifndef _YPBPR_AUTO_H

