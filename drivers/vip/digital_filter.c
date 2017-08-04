/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2006
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is for scaler related functions.
 *
 * @author 	$Author$
 * @date		$Date$
 * @version	$Revision$
 * @ingroup	scaler
 */

/**
 * @addtogroup scaler
 * @{
 */

/*============================ Module dependency  ===========================*/
#include "vip\digital_filter.h"
#include "vip\color.h"
//#include "VIP_video_performance.h"
#include "reg_def.h"
/*===================================  Types ================================*/

/*================================== Variables ==============================*/
extern UINT8 xdata vip_i;
extern UINT8 xdata vip_j;
extern UINT8 xdata vip_8bit_tmp[8];
extern UINT32 xdata vip_16bit_tmp[3];
extern UINT32 xdata vip_32bit_tmp[3];
//extern UINT16 xdata *vip_16bit_table_index;//*vip_table_index;
//extern UINT8 xdata *vip_8bit_table_index;
extern INT32 xdata vip_INT32_tmp[2];
extern INT16 xdata vip_INT16_tmp[5];

/*================================ Definitions ==============================*/

//--------------------------------------------------
// Definitions of Digital Filter
//--------------------------------------------------
#define _DISABLE_PORT				0
#define _PHASE_ACCESS_PORT              1
#define _NEG_SMEAR_ACCESS_PORT	2
#define _POS_SMEAR_ACCESS_PORT	3
#define _NEG_RING_ACCESS_PORT		4
#define _POS_RING_ACCESS_PORT		5
#define _MISMATCH_ACCESS_PORT		6
#define _YPBPR_ACCESS_PORT			7
#define _NOISE_REDUCTION_PORT		8

#define _YPBPR_DISABLE                  0
#define _YPBPR_ENABLE                   7

#define _FUNCTION_DISABLE               0
#define _FUNCTION_ENABLE                1

#define _PHASE_THD_0                    0
#define _PHASE_THD_1                    1
#define _PHASE_THD_2                    2
#define _PHASE_THD_3                    3
#define _PHASE_THD_4                    4
#define _PHASE_THD_5                    5
#define _PHASE_THD_6                    6
#define _PHASE_THD_7                    7

#define _SMEAR_RING_THD_0               0
#define _SMEAR_RING_THD_1               1
#define _SMEAR_RING_THD_2               2
#define _SMEAR_RING_THD_3               3
#define _SMEAR_RING_THD_4               4
#define _SMEAR_RING_THD_5               5
#define _SMEAR_RING_THD_6               6
#define _SMEAR_RING_THD_7               7

#define _MISMATCH_THD_0                 0
#define _MISMATCH_THD_1                 1

#define _NOISE_REDUCTION_THD_0          0
#define _NOISE_REDUCTION_THD_1          1
#define _NOISE_REDUCTION_THD_2          2
#define _NOISE_REDUCTION_THD_3          3
#define _NOISE_REDUCTION_THD_4          4
#define _NOISE_REDUCTION_THD_5          5
#define _NOISE_REDUCTION_THD_6          6
#define _NOISE_REDUCTION_THD_7          7

#define _DIV_VALUE_0                    0
#define _DIV_VALUE_1                    1
#define _DIV_VALUE_2                    2
#define _DIV_VALUE_3                    3

/*================================== Function ===============================*/

/*============================================================================*/
/**
 * Digital Filter
 * This function is used to set Digital Filter registers
 *
 * @param <info> { display-info struecture }
 * @return { none }
 * @note
 * It can be linear or non-linear either, based on the non-linear flag condition:
 */
void drvif_color_digital_filter_set(UINT8 Access, UINT8 Offset, UINT8 Div, UINT8 Enable)
{
//	rtdf_outb(Page_Select_0x009f, _PAGE0);
    rtdf_pageSelect(_PAGE0);

    vip_8bit_tmp[0] = ((Access & 0x0f) << 4) | 0x00;

    if (Access == _YPBPR_ACCESS_PORT)
    {
        vip_8bit_tmp[1] = ((Offset & 0x07) << 5) | 0x10;
    }
    else
    {
        vip_8bit_tmp[1] = ((Enable & 0x01) << 7) | ((Offset & 0x07) << 4) | ((Div & 0x03) << 2);
    }

    rtdf_maskScalerPort(Digital_Filter_Ctr_PAGE0_0x0098, ~(0xff), vip_8bit_tmp[0]);
    rtdf_maskScalerPort(Digital_Filter_Port_PAGE0_0x0099, ~(0xff), vip_8bit_tmp[1]);
    rtdf_writeScalerPort(Digital_Filter_Ctr_PAGE0_0x0098, 0x00);

}
#if 0
//--------------------------------------------------
// Description  : Adjust Digital Filter
// Input Value  : None
// Output Value : None
//--------------------------------------------------
void CAdjustDigitalFilter(BYTE ucAccess, BYTE ucOffset, BYTE ucDiv, BYTE ucEnable)
{
    pData[0] = ((ucAccess & 0x0f) << 4) | 0x00;

    if (ucAccess == _YPBPR_ACCESS_PORT)
    {
        pData[1] = ((ucOffset & 0x07) << 5) | 0x10;
    }
    else
    {
        pData[1] = ((ucEnable & 0x01) << 7) | ((ucOffset & 0x07) << 4) | ((ucDiv & 0x03) << 2);
    }

    CScalerWrite(_DIGITAL_FILTER_CTRL_98, 2, pData, _AUTOINC);

    CScalerSetByte(_DIGITAL_FILTER_CTRL_98, 0x00);

}
#endif

/*============================================================================*/
/**
 * Digital Filter
 * This function is used to set Digital Filter registers
 *
 * @param <info> { display-info struecture }
 * @return { none }
 * @note
 * It can be linear or non-linear either, based on the non-linear flag condition:
 */
void drvif_color_digital_filter_config(UINT8 Enable)
{

    drvif_color_digital_filter_set(_PHASE_ACCESS_PORT, _PHASE_THD_2, _DIV_VALUE_3, Enable);
    drvif_color_digital_filter_set(_NEG_SMEAR_ACCESS_PORT, _SMEAR_RING_THD_4, _DIV_VALUE_1, Enable); //KEN 2005/09/23
    drvif_color_digital_filter_set(_MISMATCH_ACCESS_PORT, _MISMATCH_THD_0, _DIV_VALUE_0, Enable);
    drvif_color_digital_filter_set(_NOISE_REDUCTION_PORT, _NOISE_REDUCTION_THD_7, _DIV_VALUE_0, Enable);

    drvif_color_digital_filter_set(_YPBPR_ACCESS_PORT, _YPBPR_ENABLE, _DIV_VALUE_0, Enable);

}
#if 0
void CModeSetupEtcs(BYTE ucPar)
{
    CMiscEnableDoubleBuffer();
//Anderson 080130 for FRC Support Start
#if(_FRC_SUPPORT == _ON)
    if (GET_FRCSTATUS())
    {
        CScalerSetBit(_WATCH_DOG_CTRL0_0C, ~(_BIT5 | _BIT4 | _BIT3), 0x00);
        CScalerSetBit(_VDISP_CTRL_28, ~_BIT5, 0x00);
    }
    else
#endif
    {
        if ((bit)CScalerGetBit(_VDISP_CTRL_28, _BIT3))
        {
            CScalerSetBit(_VDISP_CTRL_28, ~_BIT5, 0x00);
        }
    }
//Anderson 080130 for FRC Support End

    CAdjustDigitalFilter(_PHASE_ACCESS_PORT, _PHASE_THD_0, _DIV_VALUE_2, ucPar);
    CAdjustDigitalFilter(_NEG_SMEAR_ACCESS_PORT, _SMEAR_RING_THD_4, _DIV_VALUE_1, ucPar); //KEN 2005/09/23
    CAdjustDigitalFilter(_MISMATCH_ACCESS_PORT, _MISMATCH_THD_0, _DIV_VALUE_0, ucPar);
    CAdjustDigitalFilter(_NOISE_REDUCTION_PORT, _NOISE_REDUCTION_THD_7, _DIV_VALUE_0, ucPar);

    CAdjustDigitalFilter(_YPBPR_ACCESS_PORT, _YPBPR_ENABLE, _DIV_VALUE_0, ucPar);
    CMiscClearStatusRegister();
}
#endif

/*======================== End of File =======================================*/
/**
*
* @}
*/


