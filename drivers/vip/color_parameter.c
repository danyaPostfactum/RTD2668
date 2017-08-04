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
#include "sparrow.h"
//#include "VIP_video_performance.h"
#include "vip\color.h"

/*===================================  Types ================================*/


/*================================== Variables ==============================*/
//for common use in vip drivers

/*===================================  Tables ================================*/



/*===================================  Dither Tables  ================================*/

UINT8 code tDITHER_SEQ_TABLE_0[] =
{
    0xe4,0xa2,0x05,0x37,0xf6,0x31,0x69,0xcb,0x1f,0xd2,0xb0,0xe6,
    0x45,0x1b,0x87,0xc6,0x9e,0xb4,0xc6,0x38,0xd4,0xdb,0x12,0x1b,
};


UINT8 code tDITHER_TABLE_10_TO_6[] =
{
    0x07,0xf8,0x69,0x1e,0xad,0x52,0xc3,0xb4,
    0xad,0x52,0xc3,0xb4,0x07,0xf8,0x69,0x1e,
    0xad,0x52,0x69,0x1e,0xc3,0xb4,0x07,0xf8,
};

UINT8 code tDITHER_TABLE_10_TO_8[] =
{
    0x01,0x32,0x12,0x03,0x23,0x10,0x30,0x21,
    0x23,0x10,0x30,0x21,0x01,0x32,0x12,0x03,
    0x30,0x21,0x01,0x32,0x12,0x03,0x23,0x10,

};

UINT8 code tDITHER_TEMPOFFSET_TABLE[] =
{
    0xe4,0xe4,0xe4,0xe4,  //0xe4 can be a default setting
};


#if 0
UINT8 code tDITHER_SEQUENCE[3][32] =
{
    {  0,  1,  2,  3,  2,  0,  2,  2,  1,  1,  0,  0,  3,  1,  3,  0,  2,  1,  3,  3,  1,  0,  3,  0,  1,  2,  2,  1,  3,  2,  0,  3 },
    {  3,  3,  1,  0,  2,  0,  1,  3,  0,  0,  3,  2,  2,  1,  2,  3,  1,  1,  0,  1,  3,  2,  1,  0,  3,  1,  0,  2,  2,  1,  0,  3 },
    {  2,  3,  1,  2,  0,  1,  3,  2,  2,  1,  0,  3,  0,  2,  3,  0,  0,  1,  1,  3,  3,  2,  1,  3,  2,  0,  1,  0,  3,  2,  1,  0 }
};


UINT8 code tDITHER_TEMPORAL[16] =
{
    0, 3, 2, 1, 2, 1, 3, 0, 3, 0, 1, 2, 1, 2, 0, 3
};

UINT8 code tDITHER_LUT_06[3][4][4] =
{
    { { 0, 7,15, 8 }, { 6, 9, 1,14 }, {10,13, 5, 2 }, {12, 3,11, 4 } },
    { {10,13, 5, 2 }, {12, 3,11, 4 }, { 0, 7,15, 8 }, { 6, 9, 1,14 } },
    { {10,13, 5, 2 }, { 6, 9, 1,14 }, {12, 3,11, 4 }, { 0, 7,15, 8 } }
};

UINT8 code tDITHER_LUT_08[3][4][4] =
{
    { { 0, 3, 2, 1 }, { 2, 1, 3, 0 }, { 3, 0, 1, 2 }, { 1, 2, 0, 3 } },
    { { 2, 1, 3, 0 }, { 3, 0, 1, 2 }, { 1, 2, 0, 3 }, { 0, 3, 2, 1 } },
    { { 3, 0, 1, 2 }, { 1, 2, 0, 3 }, { 0, 3, 2, 1 }, { 2, 1, 3, 0 } }
};

UINT8 code tDITHER_LUT_10[3][4][4] =
{
    { { 0, 3, 2, 1 }, { 2, 1, 3, 0 }, { 3, 0, 1, 2 }, { 1, 2, 0, 3 } },
    { { 2, 1, 3, 0 }, { 3, 0, 1, 2 }, { 1, 2, 0, 3 }, { 0, 3, 2, 1 } },
    { { 3, 0, 1, 2 }, { 1, 2, 0, 3 }, { 0, 3, 2, 1 }, { 2, 1, 3, 0 } }
};
#endif

/*===================================  color sapce  ================================*/
UINT16 code tRGB2YUV_COEF_709_YUV_0_255[] =
{
    // CCIR 709 YPbPr
    0x0400, 	// m11
    0x0066, 	// m12
    0x00c4,	// m13
    0x0000, 	// m21 >> 2
    0x01fb, 	// m22 >> 1
    0x01e4, 	// m23 >> 2
    0x0000, 	// m31 >> 2
    0x01ed, 	// m32 >> 2
    0x01f7, 	// m33 >> 1

    0x0000,  	// Yo_even
    0x0000,  	// Yo_odd
    0x0100,  	// Y_gain
    0x0000,  // sel_RGB
    0x0000,  // sel_Yin_offset
    0x0001,  // sel_UV_out_offset
    0x0000,  // sel_UV_off
    0x0000,  // Matrix_bypass
    0x0001,  // Enable_Y_gain

};

UINT16 code tRGB2YUV_COEF_709_YUV_16_235[] =
{
    // CCIR 709 HDMI 16~235
    0x0400, 	// m11
    0x0066, 	// m12
    0x00c4,	// m13
    0x0000, 	// m21 >> 2
    0x01fb, 	// m22 >> 1
    0x01e4, 	// m23 >> 2
    0x0000, 	// m31 >> 2
    0x01ed, 	// m32 >> 2
    0x01f7, 	// m33 >> 1

    0x0000,  	// Yo_even
    0x0000,  	// Yo_odd
    0x0129,  	// Y_gain
    0x0000,  // sel_RGB
    0x0001,  // sel_Yin_offset
    0x0001,  // sel_UV_out_offset
    0x0000,  // sel_UV_off
    0x0000,  // Matrix_bypass
    0x0001,  // Enable_Y_gain

};

UINT16 code tRGB2YUV_COEF_709_RGB_0_255[] =
{
    // CCIR 709 RGB
#if 1 // for 0~255
    0x0131,	// m11
    0x025c,  // m12
    0x0074,  // m13
#else // for 16~235
    0x0163,	// m11
    0x02bf,   // m12
    0x0087,  // m13

#endif

    0x01D5,   // m21 >> 2
    0x0357,   // m22 >> 1
    0x0080,   // m23 >> 2
    0x0080,   // m31 >> 2
    0x0195,   // m32 >> 2
    0x03D7,   // m33 >> 1

    0x0000,  	// Yo_even
    0x0000,  	// Yo_odd
    0x0100,  	// Y_gain
    0x0001,  // sel_RGB
    0x0000,  // sel_Yin_offset
    0x0001,  // sel_UV_out_offset
    0x0000,  // sel_UV_off
    0x0000,  // Matrix_bypass
    0x0001,  // Enable_Y_gain
};


UINT16 code tRGB2YUV_COEF_709_RGB_16_235[] =
{
    // CCIR 709 RGB
#if 1 // for 0~255
    0x0131,	// m11
    0x025c,   // m12
    0x0074,   // m13
#else // for 16~235
    0x0163,	// m11
    0x02bf,   // m12
    0x0087,   // m13

#endif

    0x01D5,   // m21 >> 2
    0x0357,   // m22 >> 1
    0x0080,   // m23 >> 2
    0x0080,   // m31 >> 2
    0x0195,   // m32 >> 2
    0x03D7,   // m33 >> 1

    0x03E0,  	// Yo_even
    0x03E0,  	// Yo_odd
    0x0129,  	// Y_gain
    0x0001,  // sel_RGB
    0x0000,  // sel_Yin_offset
    0x0001,  // sel_UV_out_offset
    0x0000,  // sel_UV_off
    0x0000,  // Matrix_bypass
    0x0001,  // Enable_Y_gain

};

UINT16 code tRGB2YUV_COEF_601_RGB_0_255[] =
{
    // CCIR 601 RGB

    0x0132, 	// m11
    0x0259,   // m12
    0x0075,   // m13
    0x01D5,   // m21 >> 2
    0x0356,   // m22 >> 1
    0x0080,   // m23 >> 2
    0x0080,   // m31 >> 2
    0x0195,   // m32 >> 2
    0x03d6,   // m33 >> 1

    0x0000,  	// Yo_even
    0x0000,  	// Yo_odd
    0x0100,  	// Y_gain
    0x0001,  // sel_RGB
    0x0000,  // sel_Yin_offset
    0x0001,  // sel_UV_out_offset
    0x0000,  // sel_UV_off
    0x0000,  // Matrix_bypass
    0x0001,  // Enable_Y_gain

};

UINT16 code tRGB2YUV_COEF_601_RGB_16_235[] =
{
    // CCIR 601 RGB
    0x0132, 	// m11
    0x0259,   // m12
    0x0075,   // m13
    0x01D5,   // m21 >> 2
    0x0356,   // m22 >> 1
    0x0080,   // m23 >> 2
    0x0080,   // m31 >> 2
    0x0195,   // m32 >> 2
    0x03d6,   // m33 >> 1

    0x03E0,  	// Yo_even
    0x03E0,  	// Yo_odd
    0x0129,  	// Y_gain
    0x0001,  // sel_RGB
    0x0000,  // sel_Yin_offset
    0x0001,  // sel_UV_out_offset
    0x0000,  // sel_UV_off
    0x0000,  // Matrix_bypass
    0x0001,  // Enable_Y_gain

};

UINT16 code tRGB2YUV_COEF_601_YCbCr_0_255[] =
{
    // CCIR 601 YCbCr
    0x0000, 	// m11
    0x0000,   // m12
    0x0000,   // m13
    0x0000,   // m21 >> 2
    0x0000,   // m22 >> 1
    0x0000,   // m23 >> 2
    0x0000,   // m31 >> 2
    0x0000,   // m32 >> 2
    0x0000,   // m33 >> 1

    0x0000,  	// Yo_even
    0x0000,  	// Yo_odd
    0x0100,  	// Y_gain
    0x0000,  // sel_RGB
    0x0000,  // sel_Yin_offset
    0x0001,  // sel_UV_out_offset
    0x0000,  // sel_UV_off
    0x0001,  // Matrix_bypass
    0x0001,  // Enable_Y_gain

};

UINT16 code tRGB2YUV_COEF_601_YCbCr_16_235[] =
{
    // CCIR 601 YCbCr
    0x0000, 	// m11
    0x0000,   // m12
    0x0000,   // m13
    0x0000,   // m21 >> 2
    0x0000,   // m22 >> 1
    0x0000,   // m23 >> 2
    0x0000,   // m31 >> 2
    0x0000,   // m32 >> 2
    0x0000,   // m33 >> 1

    0x0000,  	// Yo_even
    0x0000,  	// Yo_odd
    0x0129,  	// Y_gain
    0x0000,  // sel_RGB
    0x0001,  // sel_Yin_offset
    0x0001,  // sel_UV_out_offset
    0x0000,  // sel_UV_off
    0x0001,  // Matrix_bypass
    0x0001,  // Enable_Y_gain
};


UINT16 code tYUV2RGB_COEF_601_YCbCr[] =
{

    0x0100, // k11
    0x0166, // k13
    0x0058, // k22
    0x00b6, // k23
    0x01c5, // k32
    0x0000, // R-offset
    0x0000, // G-offset
    0x0000, // B-offset

    0x0000, // Y Clamp (Y-16)
    0x0001, // UV Clamp (UV-512)

};

UINT16 code tYUV2RGB_COEF_709_YPbPr[] =
{
    0x0100, // k11
    0x0193, // k13
    0x002f, // k22
    0x0077, // k23
    0x01db, // k32
    0x0000, // R-offset
    0x0000, // G-offset
    0x0000, // B-offset

    0x0000, // Y Clamp (Y-16)
    0x0001, // UV Clamp (UV-512)
};

UINT8 code tDLTI_COEF[10][9] =
{
// bool HF_Enable, UINT8 HF_Th,  UINT8 Gain, UINT8 Gain_th, bool  AdvanceDelay, bool Long, bool Near, bool Contour_enable, UINT8 Contour_th
    {0x01, 0x2A, 0x02, 0x04, 0x00, 0x01, 0x01, 0x01, 0x0A},// 1
    {0x01, 0x2A, 0x02, 0x04, 0x00, 0x01, 0x01, 0x01, 0x0A},	// 2
    {0x01, 0x2A, 0x03, 0x04, 0x00, 0x01, 0x01, 0x01, 0x08},	// 3
    {0x01, 0x2A, 0x04, 0x01, 0x00, 0x01, 0x01, 0x01, 0x04},	// 4
    {0x01, 0x2A, 0x0A, 0x01, 0x00, 0x01, 0x01, 0x01, 0x06},	// 5
    {0x01, 0x2A, 0x0C, 0x04, 0x00, 0x01, 0x01, 0x01, 0x06},	// 6
    {0x01, 0x2A, 0x0D, 0x04, 0x01, 0x01, 0x01, 0x01, 0x03},	// 7
    {0x01, 0x2A, 0x0D, 0x05, 0x01, 0x01, 0x01, 0x01, 0x02},	// 8
    {0x01, 0x2A, 0x0D, 0x06, 0x01, 0x01, 0x01, 0x01, 0x02},	// 9
    {0x01, 0x2A, 0x0D, 0x07, 0x01, 0x01, 0x01, 0x01, 0x02},	// 10

};


UINT8 code tDCTI_COEF[10][6] =
{
//	bool Prevent_PE,  UINT8 F_Gain, UINT8 F_th, bool S_DCTI_Enable, UINT8 S_Gain, UINT8 S_Th
    {0x01, 0x00, 0x00, 0x00, 0x00, 0x00},	// 1
    {0x01, 0x03, 0x01, 0x00, 0x00, 0x00},	// 2
    {0x00, 0x06, 0x01, 0x00, 0x00, 0x00},	// 3
    {0x00, 0x09, 0x01, 0x00, 0x00, 0x00},	// 4
    {0x00, 0x0C, 0x01, 0x00, 0x00, 0x00},	// 5
    {0x00, 0x0F, 0x02, 0x00, 0x00, 0x00},	// 6
    {0x00, 0x03, 0x02, 0x01, 0x03, 0x10},	// 7
    {0x00, 0x06, 0x02, 0x01, 0x06, 0x20},	// 8
    {0x00, 0x0A, 0x10, 0x01, 0x0A, 0x30},	// 9
    {0x00, 0x0F, 0x1F, 0x01, 0x0F, 0x3F},	// 10

};

/*======================== End of File =======================================*/
/**
*
* @}
*/

