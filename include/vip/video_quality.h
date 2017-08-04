/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2006
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is for video quality related functions.
 *
 * @author 	$Author$
 * @date 	$Date$
 * @version $Revision$
 */

/**
 * @addtogroup color
 * @{
 */

#ifndef _VIDEO_QUALITY_H
#define _VIDEO_QUALITY_H
/*============================ Module dependency  ===========================*/
//#include "global.h"
#include "sparrow.h"
#include "vip\vip_parameter_def.h"
/*===================================  Types ================================*/

/*================================== Variables ==============================*/
/*================================ Definitions ==============================*/


#define VIP_QUALITY_CVBS_NTSC	  	0
#define VIP_QUALITY_CVBS_PAL	  	1
#define VIP_QUALITY_CVBS_S_NTSC  	2
#define VIP_QUALITY_CVBS_S_PAL  	3
#define VIP_QUALITY_TV_NTSC 	 	4
#define VIP_QUALITY_TV_PAL  		5

#define VIP_QUALITY_YPbPr_480I  	6
#define VIP_QUALITY_YPbPr_576I  	7
#define VIP_QUALITY_YPbPr_480P  	8
#define VIP_QUALITY_YPbPr_576P  	9
#define VIP_QUALITY_YPbPr_720P  	10
#define VIP_QUALITY_YPbPr_1080I  	11
#define VIP_QUALITY_YPbPr_1080P  	12

#define VIP_QUALITY_HDMI_480I  		13
#define VIP_QUALITY_HDMI_576I  		14
#define VIP_QUALITY_HDMI_480P  	15
#define VIP_QUALITY_HDMI_576P  	16
#define VIP_QUALITY_HDMI_720P 	 	17
#define VIP_QUALITY_HDMI_1080I  	18
#define VIP_QUALITY_HDMI_1080P  	19

#define VIP_QUALITY_VGA  			20
#define VIP_QUALITY_TOTAL_SOURCE_NUM 21


#define VIP_QUALITY_FUNCTION_DLTi 		0
#define VIP_QUALITY_FUNCTION_DCTi 		1
#define VIP_QUALITY_FUNCTION_Gamma 	2
#define VIP_QUALITY_FUNCTION_PEAK		3
#define VIP_QUALITY_FUNCTION_NRSPAT 	4
#define VIP_QUALITY_FUNCTION_NRTEMP 	5
#define VIP_QUALITY_FUNCTION_CHRER 	6
#define VIP_QUALITY_FUNCTION_DCC 		7
#define VIP_QUALITY_FUNCTION_22 		8
#define VIP_QUALITY_FUNCTION_32 		9
#define VIP_QUALITY_FUNCTION_Intra 	10
#define VIP_QUALITY_FUNCTION_MA 		11
#define VIP_QUALITY_FUNCTION_XC 		12
#define VIP_QUALITY_FUNCTION_SU_H 	13
#define VIP_QUALITY_FUNCTION_SU_V 	14
#define VIP_QUALITY_FUNCTION_S_PK 	15
#define VIP_QUALITY_FUNCTION_VD_Hue 		16
#define VIP_QUALITY_FUNCTION_VD_Sat 		17
#define VIP_QUALITY_FUNCTION_VD_Contrast	18
#define VIP_QUALITY_FUNCTION_VD_Brightness	19
#define VIP_QUALITY_FUNCTION_ICM				20


#define VIP_QUALITY_FUNCTION_TOTAL_CHECK 	21

//extern UINT8 code VIP_QUALITY_Coef [VIP_QUALITY_TOTAL_SOURCE_NUM][VIP_QUALITY_FUNCTION_TOTAL_CHECK];
extern VIP_QUALITY_COEF_TABLE code stVIP_Quality_Coef_Table;
extern SHP_COEF_TABLE code stSHP_Coef_Table;
//extern INT16 code  SHP_Coef[VIP_QUALITY_TOTAL_SOURCE_NUM][15];
//extern SHP_COEF_TABLE code stSHP_Coef_Table;

extern UINT8 code tDCC_CONTROL[5][9];
extern UINT8 code tDCC_USERCURVE[5][23];
extern UINT8 code tICM_ini[6][6][52];
extern UINT8 code SU_PK_Coeff[5] [6];
extern  UINT8 code  tGAMMA_1_R[];
extern  UINT8  code tGAMMA_1_G[];
extern  UINT8 code  tGAMMA_1_B[];
extern  UINT8 code  tGAMMA_2_R[];
extern  UINT8 code  tGAMMA_2_G[];
extern  UINT8 code  tGAMMA_2_B[];

extern UINT8  code tInput_GAMMA_R[];
extern UINT8  code tInput_GAMMA_G[];
extern UINT8  code tInput_GAMMA_B[];

#if 0 // marked by Jerry Wu 20071106
typedef struct //Thur 20070720
{
    INT16 XSrc_Hue;
    INT16 XSrc_Sat;
    INT16 XSrc_HueMin;
    INT16 XSrc_HueMax;
    INT16 XSrc_SatMin;
    INT16 XSrc_SatMax;
    /*UINT8 HDMI1YCbCr_Hue;
    UINT8 HDMI1YCbCr_Sat;
    UINT8 HDMI2YCbCr_Hue;
    UINT8 HDMI2YCbCr_Sat;
    UINT8 AV_Hue;
    UINT8 AV_Sat;
    UINT8 AV1_Hue;
    UINT8 AV1_Sat;
    UINT8 AV2_Hue;
    UINT8 AV2_Sat;
    UINT8 AV3_Hue;
    UINT8 AV3_Sat;
    UINT8 SV_Hue;
    UINT8 SV_Sat;
    UINT8 SV1_Hue;
    UINT8 SV1_Sat;
    UINT8 YPbPr_Hue;
    UINT8 YPbPr_Sat;
    UINT8 TV_Hue;
    UINT8 TV_Sat;
    UINT8 HDMIRGB_Hue;
    UINT8 HDMIRGB_Sat;*/
} StructHueSatDefault;
INT16 GenSat(UINT8 display, UINT8 OSDSAT);
INT16 GenHue(UINT8 display, UINT8 OSDHUE);
#endif

#if 0
extern UINT8  code tGAMMA_1_R[] ;
extern UINT8  code tGAMMA_1_G[] ;
extern UINT8  code tGAMMA_1_B[] ;
extern UINT8  code tGAMMA_2_R[] ;
extern UINT8  code tGAMMA_2_G[] ;
extern UINT8  code tGAMMA_2_B[] ;
#endif

/*============================================================================*/
/**
 * Video_Quality_Handler
 * This function is used to set Video Quality function.
 *
 * @param <void>		{ void }
 * @return 			{ void }
 *
 */
//void Video_Quality_Handler(void);


#endif

