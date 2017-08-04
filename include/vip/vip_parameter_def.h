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
#ifndef _VIP_PARAMETER_DEF_H
#define _VIP_PARAMETER_DEF_H
#include "sparrow.h"
#if 0
typedef struct _VIP_QUALITY_COEF
{
    UINT8	mode;
    UINT8	DLTi:4;
    UINT8	DCTi:4;
    UINT8	Gamma:4;
    UINT8	Peak:4;
    UINT8	NRSpat:4;
    UINT8	NRTemp:4;
    UINT8	ChrEr:4;
    UINT8	DCC:4;
    UINT8	Film_22:4;
    UINT8	Film_32:4;
    UINT8	Intra:4;
    UINT8	MA:4;
    UINT8	XColor:4;
    UINT8	SU_H:4;
    UINT8	SU_V:4;
    UINT8	S_PK:4;
    UINT8	VD_Hue:4;  //Attention this function only valid for NTSC system
    UINT8	VD_Sat:4;
    UINT8	VD_Y_Contrast;
    UINT8	VD_Y_Brightness;
    UINT8	ICM_Table_Apply:4;
} VIP_QUALITY_COEF;
#else

typedef struct _VIP_QUALITY_COEF
{
    UINT8	mode;				// 0		//use system definition
    UINT8	I_DLTi:4;			// 1		//range:0~10
    UINT8	I_DCTi:4;			// 2		//range:0~10
    UINT8	Gamma:4;			// 3		//range:0~2
    UINT8	Digital_Filter:4;		// 4		//range:0~1
    UINT8	SUNR_Y:4;			// 5		//range:0~255
    UINT8	SUNR_C:4;			// 6		//range:0~255
    UINT8	SU_MASK:4;			// 7		//range:0~3
    UINT8	DCC:4;				// 8		//range:0~4
    UINT8	D_DLTi:4;			// 9		//range:0~10
    UINT8	D_DCTi:4;			// 10	//range:0~10
    UINT8	SU_H:4;				// 11	//range:0~9
    UINT8	SU_V:4;				// 12	//range:0~9
    UINT8	SU_PK:4;			// 13	//range:0~10
    UINT8	VD_Hue;				// 14	//range:0~255				  //Attention this function only valid for NTSC system
    UINT8	VD_Sat;				// 15	//range:0~255
    UINT8	VD_Y_Contrast;		// 16	//range:0~255
    UINT8	VD_Y_Brightness;	// 17	//range:0~255
    UINT8	ICM_Table_Apply:4;	// 18	//range:0~5
    UINT8	Diter_Bit:4;			// 19	//range:0~2
//	UINT8 	S_index;
//	UINT8	S_low;
//	UINT8	S_high;
} VIP_QUALITY_COEF;
#endif

typedef struct _VIP_QUALITY_COEF_TABLE
{
    UINT8	num;
    VIP_QUALITY_COEF* ptVIP_QUALITY_COEF;
} VIP_QUALITY_COEF_TABLE;

typedef struct _SHP_COEF
{
    UINT8	mode;// 0
    UINT8	Resolution;// 1
    INT16	C0;// 2
    INT16	C1;// 3
    INT16	C2;// 4
    INT16	C4_Blr;//5
    UINT8	G_Pos_Min;// 6
    UINT8	G_Pos_Max;// 7
    UINT8	G_Neg_Min;// 8
    UINT8	G_Neg_Max;// 9
    UINT16	HV_POS_Min;// 10
    UINT16	HV_POS_Max;// 11
    UINT16	HV_NEG_Min;// 12
    UINT16	HV_NEG_Max;// 13
    UINT8	LV_Min;// 14
    UINT8	LV_Max;// 15
} SHP_COEF;

typedef struct _SHP_COEF_TABLE
{
    UINT8	num;
    SHP_COEF* ptSHP_COEF;
} SHP_COEF_TABLE;


#endif // #ifndef _VIP_PARAMETER_DEF_H


