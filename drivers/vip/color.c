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
#include "rtd_types.h"
#include "source.h"
#include "flow_scaler.h" //•i•H∂‹Â?
#include "reg_def.h"
#include "vip\color.h"
#include "panel\panel_struct.h"
#include "panel\panelapi.h"
#include "ui_struct.h"
#include "flow_struct.h"
#include "vip\video_quality.h"
#include "timer_event.h"
#include "pcb_customer.h"
#include "pcb\pcb_utility.h"
#include "hdmi\rbushdmiReg.h"
#include "mode_customer.h"
#include "rosprintf.h"
#include "flash\spi_flash.h"
#include "autoconf.h"

/*===================================  Types ================================*/
#define _PANEL_BIT_06 0
#define _PANEL_BIT_08 1
#define _PANEL_BIT_10 2
/*
#define HDMI_AP_ADDR 0xB81200C9
#define HDMI_DP_ADDR 0xB81200CA

#define HDMI_PSAP_ADDR 0xB81200CD
#define HDMI_PSDP_ADDR 0xB81200CE
#define HDMI_SR 0xb8020100
*/
#define HDMI_ACRCR_ADDR 0x51

//Highlight Window
#define HLW_FullRegion 0x0	//Full region
#define HLW_InsideWindow 0x1    //01: Inside window
#define HLW_OutsideWindow 0x2    //10: Outside window

#define HLW_DCC_ICM 0x00000c00
#define HLW_PeakingCoring 0x00000300
#define HLW_DCR 0x000000c0
#define HLW_sRGB 0x00000030
#define HLW_Gamma 0x0000000c
#define HLW_ContrastBrightness 0x00000003

#undef	USE_DMA_COPU
/*================================== Variables ==============================*/
//for common use in vip drivers, to save the usage of variable
UINT16 xdata vip_i;
UINT8 xdata vip_j;
UINT8 xdata vip_8bit_tmp[8];
UINT32 xdata vip_16bit_tmp[3];
UINT32 xdata vip_32bit_tmp[3];
//UINT16 xdata *vip_16bit_table_index;//*vip_table_index;
//UINT8 xdata *vip_8bit_table_index;
INT32 xdata vip_INT32_tmp[2];
INT16 xdata vip_INT16_tmp[6];


/*===================================  Tables ================================*/


/*===================================  color sapce  ================================*/
extern UINT16 code tRGB2YUV_COEF_709_YUV_0_255[];
extern UINT16 code tRGB2YUV_COEF_709_YUV_16_235[];
extern UINT16 code tRGB2YUV_COEF_709_RGB_0_255[];
extern UINT16 code tRGB2YUV_COEF_709_RGB_16_235[];
extern UINT16 code tRGB2YUV_COEF_601_RGB_0_255[];
extern UINT16 code tRGB2YUV_COEF_601_RGB_16_235[];
extern UINT16 code tRGB2YUV_COEF_601_YCbCr_0_255[];
extern UINT16 code tRGB2YUV_COEF_601_YCbCr_16_235[];
extern UINT16 code tYUV2RGB_COEF_601_YCbCr[];
extern UINT16 code tYUV2RGB_COEF_709_YPbPr[];
extern UINT8 code tDLTI_COEF[10][9];
extern UINT8 code tDCTI_COEF[10][6];



#if 0
extern UINT8 code tDITHER_SEQUENCE[3][32];
extern UINT8 code tDITHER_TEMPORAL[16];
extern  UINT8 code tDITHER_LUT_06[3][4][4];
extern  UINT8 code tDITHER_LUT_08[3][4][4];
extern  UINT8 code tDITHER_LUT_10[3][4][4];
#else

extern UINT8 code tDITHER_SEQ_TABLE_0[];
extern UINT8 code tDITHER_TABLE_10_TO_6[];
extern UINT8 code tDITHER_TABLE_10_TO_8[];
extern UINT8 code tDITHER_TEMPOFFSET_TABLE[];

#endif


#if 0
#ifdef _PANEL_BIT_06
extern  UINT8 code tDITHER_LUT[3][4][4];
#endif

#ifdef _PANEL_BIT_08
extern  UINT8 code tDITHER_LUT[3][4][4];
#endif

#ifdef _PANEL_BIT_10

extern  UINT8 code tDITHER_LUT[3][4][4] ;
#endif
#endif
/*
UINT8 code PeakingCoeff[][6] =
{
		//Peaking_Coef0,  Peaking_Coef1,  Peaking_Coef2,  Coring_Min,  Coring_Max_Pos,  Coring_Max_Neg ,

		//level=1 :
		0x08, 0x00, 0xFC, 0x00, 0x28, 0xF8,
		//level=2 :
		0x0C, 0x00, 0xFA, 0x00, 0x28, 0xF8,
		//level=3 :
		0x10, 0x00, 0xF8, 0x00, 0x28, 0xF8,
		//level=4 :
	     	0x18, 0x00, 0xF4, 0x00, 0x28, 0xF8,	//3
		//level=5 :
		0x20, 0x00, 0xF0, 0x00, 0x28, 0xF8,
		//level=6 :
		0x28, 0x00, 0xEC, 0x00, 0x28, 0xF8,
		//level=7 :
		0x30, 0x00, 0xE8, 0x00, 0x28, 0xF8,
		//level=8 :
		0x38, 0x00, 0xE4, 0x00, 0x28, 0xF8,
		//level=9 :
		0x40, 0x00, 0xE0, 0x00, 0x28, 0xF8,
		//level=10 :
		0x46, 0x00, 0xDD, 0x00, 0x28, 0xF8,
		//level=11 :
		0x4E, 0x00, 0xD9, 0x00, 0x28, 0xF8,
		//level=12 :
		0x56, 0x00, 0xD5, 0x00, 0x28, 0xF8,
		//level=13 :
		0x5E, 0x00, 0xD1, 0x00, 0x28, 0xF8,
		//level=14 :
		0x66, 0x00, 0xCD, 0x00, 0x28, 0xF8,
		//level=15 :
		0x6E, 0x00, 0xC9, 0x00, 0x28, 0xF8,
		//level=16 :
		0x76, 0x00, 0xC5, 0x00, 0x28, 0xF8,
};


UINT8 code TVPeakingCoeff[][6] =
 {
 		//Peaking_Coef0,  Peaking_Coef1,  Peaking_Coef2,  Coring_Min,  Coring_Max_Pos,  Coring_Max_Neg ,

		//level=1 :
		 0x08, 0x00, 0xFC, 0x12, 0x60, 0xF8,
		//level=2 :
		 0x0A, 0x00, 0xFB,	0x12, 0x60, 0xF8,
		//level=3 :
		 0x0C, 0x00, 0xFA,	0x12, 0x60, 0xF8,
		//level=4 :
		 0x0E, 0x00, 0xF9,	0x12, 0x60, 0xF8,
		//level=5 :
		 0x10, 0x00, 0xF8,	0x12, 0x60, 0xF8,
		//level=6 :
		 0x12, 0x00, 0xF7,	0x12, 0x60, 0xF8,
		//level=7 :
		 0x14, 0x00, 0xF6,	0x12, 0x60, 0xF8,
		//level=8 :
		 0x16, 0x00, 0xF5,	0x12, 0x60, 0xF8,
		//level=9 :
	     	 0x18, 0x00, 0xF4,	0x12, 0x60, 0xF8,
		//level=10 :
		 0x1A, 0x00, 0xF3,	0x12, 0x60, 0xF8,
		//level=11 :
		 0x1C, 0x00, 0xF2,	0x12, 0x60, 0xF8,
		//level=12 :
		 0x1E, 0x00, 0xF1,	0x12, 0x60, 0xF8,
		//level=13 :
		 0x20, 0x00, 0xF0,	0x12, 0x60, 0xF8,
		//level=14 :
		 0x22, 0x00, 0xEF,	0x12, 0x60, 0xF8,
		 //level=15 :
		 0x24, 0x00, 0xEE,	0x12, 0x60, 0xF8,
		//level=16 :
		 0x26, 0x00, 0xED,	0x12, 0x60, 0xF8,
};
*/


/*
//----------------------------------------------------------------------------------------------------
// DCC Table
//----------------------------------------------------------------------------------------------------
UINT8 code tDCC_CONTROL_MOVIE[] =
{
    0x86, 0x66, 0xc3,
    0xb4, 0xce, 0x1e,
    0x10, 0x0c, 0x18,
};

UINT8 code tDCC_CONTROL_PHOTO[] =
{
    0x86, 0x66, 0xc3,
    0xb4, 0xce, 0x1e,
    0x10, 0x0c, 0x18,
};

UINT8 code tDCC_CONTROL_GAME[] =
{
    0x84, 0xaa, 0xc3,
    0xb4, 0xce, 0x1e,
    0x10, 0x0c, 0x18,
};

UINT8 code tDCC_USERCURVE_MOVIE[] =
{
    0x04, 0x0c, 0x1a, 0x2b,
    0x41, 0x57, 0x6d, 0x80,
    0x95, 0xa9, 0xbd, 0xd0,
    0xe2, 0xf2, 0xfc, 0x00,
    0x10, 0x00, 0x00,
    0x80, 0x14, 0x34, 0x00,
};

UINT8 code tDCC_USERCURVE_PHOTO[] =
{
    0x15, 0x1f, 0x2f, 0x3f,
    0x4f, 0x5f, 0x6f, 0x80,
    0x91, 0xa1, 0xb1, 0xc1,
    0xd1, 0xe1, 0xf1, 0x00,
    0x10, 0x00, 0x00,
    0x00, 0x14, 0x00, 0x00,
};

UINT8 code tDCC_USERCURVE_GAME[] =
{
    0x0a, 0x18, 0x26, 0x36,
    0x46, 0x58, 0x6a, 0x80,
    0x96, 0xa8, 0xba, 0xca,
    0xd8, 0xe8, 0xf6, 0x00,
    0x10, 0x00, 0x00,
    0x64, 0x14, 0x32, 0x00
};

//----------------------------------------------------------------------------------------------------
// ICM Table
//----------------------------------------------------------------------------------------------------
UINT8 code tICM_MOVIE0[] =
{
    0x00, 0x00, 0xcc,
    0x1f, 0x00, 0x3f, 0x00, 0x5f, 0x00, 0x7f, 0x00,
    0x5f, 0x00, 0x3f, 0x00, 0x1f, 0x00, 0x00, 0x00,
    0x14, 0xec, 0x2a, 0xd6, 0x3f, 0xc1, 0x54, 0xac,
    0x40, 0xc0, 0x2a, 0xd6, 0x14, 0xec, 0x00, 0x00,
    0x14, 0x14, 0x2a, 0x2a, 0x3f, 0x3f, 0x54, 0x54,
    0x40, 0x40, 0x2a, 0x2a, 0x14, 0x14, 0x00, 0x00,
};

UINT8 code tICM_MOVIE1[] =
{
    0x04, 0x00, 0xcc,
    0x00, 0x1f, 0x00, 0x3f, 0x00, 0x5f, 0x00, 0x7f,
    0x00, 0x5f, 0x00, 0x3f, 0x00, 0x1f, 0x00, 0x00,
    0x14, 0x14, 0x2a, 0x2a, 0x3f, 0x3f, 0x54, 0x54,
    0x40, 0x40, 0x2a, 0x2a, 0x14, 0x14, 0x00, 0x00,
    0xec, 0x14, 0xd6, 0x2a, 0xc1, 0x3f, 0xac, 0x54,
    0xc0, 0x40, 0xd6, 0x2a, 0xec, 0x14, 0x00, 0x00,
};

UINT8 code tICM_MOVIE2[] =
{
    0x08, 0x00, 0xcc,
    0xe1, 0x00, 0xc1, 0x00, 0xa1, 0x00, 0x81, 0x00,
    0xa1, 0x00, 0xc1, 0x00, 0xe1, 0x00, 0x00, 0x00,
    0xec, 0x14, 0xd6, 0x2a, 0xc1, 0x3f, 0xac, 0x54,
    0xc0, 0x40, 0xd6, 0x2a, 0xec, 0x14, 0x00, 0x00,
    0xec, 0xec, 0xd6, 0xd6, 0xc1, 0xc1, 0xac, 0xac,
    0xc0, 0xc0, 0xd6, 0xd6, 0xec, 0xec, 0x00, 0x00,
};

UINT8 code tICM_MOVIE3[] =
{
    0x0c, 0x00, 0xcc,
    0x00, 0xe1, 0x00, 0xc1, 0x00, 0xa1, 0x00, 0x81,
    0x00, 0xa1, 0x00, 0xc1, 0x00, 0xe1, 0x00, 0x00,
    0xec, 0xec, 0xd6, 0xd6, 0xc1, 0xc1, 0xac, 0xac,
    0xc0, 0xc0, 0xd6, 0xd6, 0xec, 0xec, 0x00, 0x00,
    0x14, 0xec, 0x2a, 0xd6, 0x3f, 0xc1, 0x54, 0xac,
    0x40, 0xc0, 0x2a, 0xd6, 0x14, 0xec, 0x00, 0x00,
};

UINT8 code tICM_PHOTO0[] =
{
    0x00, 0x00, 0xcc,
    0x0c, 0x00, 0x19, 0x00, 0x25, 0x00, 0x32, 0x00,
    0x25, 0x00, 0x19, 0x00, 0x0c, 0x00, 0x00, 0x00,
    0x08, 0xf8, 0x11, 0xef, 0x19, 0xe7, 0x21, 0xdf,
    0x19, 0xe7, 0x11, 0xef, 0x08, 0xf8, 0x00, 0x00,
    0x08, 0x08, 0x11, 0x11, 0x19, 0x19, 0x21, 0x21,
    0x19, 0x19, 0x11, 0x11, 0x08, 0x08, 0x00, 0x00,
};

UINT8 code tICM_PHOTO1[] =
{
    0x04, 0x00, 0xcc,
    0x00, 0x0c, 0x00, 0x19, 0x00, 0x25, 0x00, 0x32,
    0x00, 0x25, 0x00, 0x19, 0x00, 0x0c, 0x00, 0x00,
    0x08, 0x08, 0x11, 0x11, 0x19, 0x19, 0x21, 0x21,
    0x19, 0x19, 0x11, 0x11, 0x08, 0x08, 0x00, 0x00,
    0xf8, 0x08, 0xef, 0x11, 0xe7, 0x19, 0xdf, 0x21,
    0xe7, 0x19, 0xef, 0x11, 0xf8, 0x08, 0x00, 0x00,
};

UINT8 code tICM_PHOTO2[] =
{
    0x08, 0x00, 0xcc,
    0xf4, 0x00, 0xe7, 0x00, 0xdb, 0x00, 0xce, 0x00,
    0xdb, 0x00, 0xe7, 0x00, 0xf4, 0x00, 0x00, 0x00,
    0xf8, 0x08, 0xef, 0x11, 0xe7, 0x19, 0xdf, 0x21,
    0xe7, 0x19, 0xef, 0x11, 0xf8, 0x08, 0x00, 0x00,
    0xf8, 0xf8, 0xef, 0xef, 0xe7, 0xe7, 0xdf, 0xdf,
    0xe7, 0xe7, 0xef, 0xef, 0xf8, 0xf8, 0x00, 0x00,
};

UINT8 code tICM_PHOTO3[] =
{
    0x0c, 0x00, 0xcc,
    0x00, 0xf4, 0x00, 0xe7, 0x00, 0xdb, 0x00, 0xce,
    0x00, 0xdb, 0x00, 0xe7, 0x00, 0xf4, 0x00, 0x00,
    0xf8, 0xf8, 0xef, 0xef, 0xe7, 0xe7, 0xdf, 0xdf,
    0xe7, 0xe7, 0xef, 0xef, 0xf8, 0xf8, 0x00, 0x00,
    0x08, 0xf8, 0x11, 0xef, 0x19, 0xe7, 0x21, 0xdf,
    0x19, 0xe7, 0x11, 0xef, 0x08, 0xf8, 0x00, 0x00,
};

UINT8 code tICM_GAME0[] =
{
    0x00, 0x00, 0xcc,
    0xe0, 0x00, 0xc0, 0x00, 0xa0, 0x00, 0x80, 0x00,
    0xa0, 0x00, 0xc0, 0x00, 0xe0, 0x00, 0x00, 0x00,
    0xea, 0x16, 0xd6, 0x2a, 0xc0, 0x40, 0xaa, 0x56,
    0xc0, 0x40, 0xd5, 0x2b, 0xea, 0x16, 0x00, 0x00,
    0xea, 0xea, 0xd6, 0xd6, 0xc0, 0xc0, 0xaa, 0xaa,
    0xc0, 0xc0, 0xd5, 0xd5, 0xea, 0xea, 0x00, 0x00,
};

UINT8 code tICM_GAME1[] =
{
    0x04, 0x00, 0xcc,
    0x00, 0xe0, 0x00, 0xc0, 0x00, 0xa0, 0x00, 0x80,
    0x00, 0xa0, 0x00, 0xc0, 0x00, 0xe0, 0x00, 0x00,
    0xea, 0xea, 0xd6, 0xd6, 0xc0, 0xc0, 0xaa, 0xaa,
    0xc0, 0xc0, 0xd5, 0xd5, 0xea, 0xea, 0x00, 0x00,
    0x16, 0xea, 0x2a, 0xd6, 0x40, 0xc0, 0x56, 0xaa,
    0x40, 0xc0, 0x2b, 0xd5, 0x16, 0xea, 0x00, 0x00,
};

UINT8 code tICM_GAME2[] =
{
    0x08, 0x00, 0xcc,
    0x20, 0x00, 0x40, 0x00, 0x60, 0x00, 0x7f, 0x00,
    0x60, 0x00, 0x40, 0x00, 0x20, 0x00, 0x00, 0x00,
    0x16, 0xea, 0x2a, 0xd6, 0x40, 0xc0, 0x56, 0xaa,
    0x40, 0xc0, 0x2b, 0xd5, 0x16, 0xea, 0x00, 0x00,
    0x16, 0x16, 0x2a, 0x2a, 0x40, 0x40, 0x56, 0x56,
    0x40, 0x40, 0x2b, 0x2b, 0x16, 0x16, 0x00, 0x00,
};

UINT8 code tICM_GAME3[] =
{
    0x0c, 0x00, 0xcc,
    0x00, 0x20, 0x00, 0x40, 0x00, 0x60, 0x00, 0x7f,
    0x00, 0x60, 0x00, 0x40, 0x00, 0x20, 0x00, 0x00,
    0x16, 0x16, 0x2a, 0x2a, 0x40, 0x40, 0x56, 0x56,
    0x40, 0x40, 0x2b, 0x2b, 0x16, 0x16, 0x00, 0x00,
    0xea, 0x16, 0xd6, 0x2a, 0xc0, 0x40, 0xaa, 0x56,
    0xc0, 0x40, 0xd5, 0x2b, 0xea, 0x16, 0x00, 0x00,
};

UINT8 code tEmpty_Table[] =
{
    0x00,
};

code UINT8 *tDCC_CONTROL[] =
{
    tDCC_CONTROL_MOVIE,     tDCC_CONTROL_PHOTO,         tDCC_CONTROL_GAME
};

code UINT8  *tDCC_USERCURVE[] =
{
    tDCC_USERCURVE_MOVIE,   tDCC_USERCURVE_PHOTO,       tDCC_USERCURVE_GAME
};

code UINT8  *tICM_TABLE[] =
{
    tICM_MOVIE0,        tICM_MOVIE1,        tICM_MOVIE2,        tICM_MOVIE3,        tEmpty_Table,       tEmpty_Table,
    tICM_PHOTO0,        tICM_PHOTO1,        tICM_PHOTO2,        tICM_PHOTO0,        tEmpty_Table,       tEmpty_Table,
    tICM_GAME0,         tICM_GAME1,         tICM_GAME2,         tICM_GAME3,         tEmpty_Table,       tEmpty_Table,
};
*/
//--------------------------------------------------
// Vivid Color Data Table
//--------------------------------------------------
code UINT8 tDCC_PAGE0[] =
{
    0x00, 0x00, 0x00, 0x00,
    0x00, 0xFF, 0x00, 0x00,
    0x00,
};

//**********************************************************************************************
//----------------------------------------------------------------------------------------------------
// DCR Table
//----------------------------------------------------------------------------------------------------
UINT16 xdata g_usHorBoundary;
UINT16 xdata g_usVerBoundary;
UINT16 xdata g_usRGBInfo;
UINT8 xdata g_ucPreBacklightPWM;
UINT8 xdata g_ucPreDCCLum;
UINT8 xdata g_ucDcrCtrl;
UINT8 xdata g_ucDCRPercent;  // Max 100
UINT8 xdata g_ucUserBacklight;

#if 0//(_DCR_MODE == _DCR_TYPE_3)
UINT8 xdata g_ucDcrTargetPre = 0;
UINT8 xdata g_ucDcrTargetPre1 = 0;
UINT8 xdata g_ucDcrTemp = 0;
UINT8 xdata g_ucDcrPwm = 0;
UINT8 xdata g_ucDcrCnt = 0;
UINT16 xdata g_usDcrStep = 0;
UINT8 xdata g_ucDcrTimer = 0;
UINT8 xdata g_ucDcrCurrentPWM = 0;
UINT16 idata g_usDCR5MinTimer = 0;
#endif

#if (_VIVID_COLOR_FUCTION == _ON)
code UINT8 tDCR_USERCURVE0[] =
{
    0x0a, 0x20, 0x30, 0x40,
    0x50, 0x60, 0x70, 0x80,
    0x90, 0xa0, 0xb0, 0xc0,
    0xd0, 0xe0, 0xf0,
};

code UINT8 tDCR_USERCURVE255[] =
{
    0x10, 0x20, 0x37, 0x4a,
    0x5a, 0x6a, 0x7a, 0x8a,
    0x9a, 0xaa, 0xba, 0xca,
    0xda, 0xe8, 0xf8,
};
#endif  // End of #if(_VIVID_COLOR_FUCTION == _ON)




#if 0//(_VIVID_COLOR_FUCTION == _ON)
//--------------------------------------------------
// Description    : Fill DCC table , then enable DCC function,DCR Interpolation
// Input Value   : pControlTable, pUserCurveTable, both parameters accroding to Vivid color software.
// Output Value : None
// Return Value : None
//--------------------------------------------------
void drv_color_AdjustDCRFillDCCTable( UINT8 lamp_value)
{
    UINT8 i = 0;

    rtdf_pageSelect(_PAGE7);

//    i = rtdf_inb(_P7_DCC_CTRL0_C7);
//	i = (i & 0xfc) | 0x01;
//	rtdf_outb(_P7_DCC_CTRL0_C7, i);
    rtdf_maskScalerPort(_P7_DCC_CTRL0_C7, 0xfc, 0x01);

    // fill DCC user curve table
//	rtdf_outb(_P7_DCC_ACCESS_PORT_C9, 0x00);
    rtdf_writeScalerPort(_P7_DCC_ACCESS_PORT_C9, 0);

    if (g_usRGBInfo < 48)
    {
        for (i=0; i<15; i++)
        {
            vip_8bit_tmp[0] = tDCR_USERCURVE255[i];
//			rtdf_outb(_P7_DCC_DATA_PORT_CA, vip_8bit_tmp[0]);
            rtdf_writeScalerPort(_P7_DCC_DATA_PORT_CA, vip_8bit_tmp[0]);
        }

        for (i=0; i<5; i++)
        {
//			rtdf_outb(_P7_DCC_DATA_PORT_CA, 0x00);
            rtdf_writeScalerPort(_P7_DCC_DATA_PORT_CA, 0);
        }
    }
    else if (g_usRGBInfo < 336)
    {
        drv_color_FillDCCTable(0);
    }
    else
    {
        vip_8bit_tmp[0] = (UINT16)(tDCR_USERCURVE255[i] - tDCR_USERCURVE0[i]) * lamp_value / 255 + tDCR_USERCURVE0[i];
    }
}
#endif // End of #if(_VIVID_COLOR_FUCTION == _ON)

//--------------------------------------------------
// Description  : DCR Measurement by HW Control
// Input Value  : None
// Output Value : None
// Return Value : None
//--------------------------------------------------
void drv_color_DcrMeasureStartHD( UINT16 disp_len, UINT16 disp_width, UINT8 osd_backlight)
{
    UINT32 ulTempB = 0;

    // get the horizontal boundary
    g_usHorBoundary = disp_width;

    // get the vertical boundary
    g_usVerBoundary = disp_len;

    rtdf_pageSelect(_PAGE7);
    rtdf_writeScalerPort(_P7_DCR_ACCESS_PORT_D8, 0x00);
    rtdf_writeScalerPort(_P7_DCR_DATA_PORT_D9,0x04); // Set the thershold 1 for DCR

//Total sum (R+G+B) of pixel value above threshold1 ===> collect bit0~ bit31
    rtdf_writeScalerPort(_P7_DCR_ACCESS_PORT_D8, 0x17);
    vip_8bit_tmp[0] = rtdf_readScalerPort(_P7_DCR_DATA_PORT_D9);
    rtdf_writeScalerPort(_P7_DCR_ACCESS_PORT_D8, 0x1B);
    vip_8bit_tmp[1] = rtdf_readScalerPort(_P7_DCR_DATA_PORT_D9);
    rtdf_writeScalerPort(_P7_DCR_ACCESS_PORT_D8, 0x1F);
    vip_8bit_tmp[2] = rtdf_readScalerPort(_P7_DCR_DATA_PORT_D9);
    rtdf_writeScalerPort(_P7_DCR_ACCESS_PORT_D8, 0x23);
    vip_8bit_tmp[3] = rtdf_readScalerPort(_P7_DCR_DATA_PORT_D9);

//====

    ulTempB = vip_8bit_tmp[0];
    ulTempB = ulTempB << 8;
    ulTempB = (ulTempB & 0x0000ff00) | vip_8bit_tmp[1];
    ulTempB = ulTempB << 8;
    ulTempB = (ulTempB & 0x00ffff00) | vip_8bit_tmp[2];
    ulTempB = ulTempB << 8;
    ulTempB = (ulTempB & 0xffffff00) | vip_8bit_tmp[3];

    ulTempB = ulTempB / g_usVerBoundary;
    ulTempB = ulTempB / g_usHorBoundary;

    g_usRGBInfo = ulTempB;

//#if(_DCR_MODE == _DCR_TYPE_1)
    drv_color_DcrDcr1(osd_backlight);
//#endif

//#if(_DCR_MODE == _DCR_TYPE_2)
    //drv_color_DcrDcr2();
//#endif
//    	rtdf_clearBitsb(_P7_DCR_ACCESS_PORT_D8, _BIT1);    // Set RESULT_READ to read the result
    rtdf_clearBitsScalerPort(_P7_DCR_ACCESS_PORT_D8, _BIT1);

}


//#if(_DCR_MODE == _DCR_TYPE_1)
//--------------------------------------------------
// Description  : DCR Type 1 process
// Input Value  : None
// Output Value : None
// Return Value : None
//--------------------------------------------------
void drv_color_DcrDcr1( UINT8 OSD_Backlight)
{
//the unit of OSD_Backlight and vip_8bit_tmp[0] are both 0~255

    if (g_usRGBInfo < DCR_THRESHOLD )
    {
        vip_8bit_tmp[0] = (UINT32)255 * g_usRGBInfo / DCR_THRESHOLD;
    }
    else
    {
        vip_8bit_tmp[0] = 255;
    }

    vip_8bit_tmp[0] = ((UINT16)vip_8bit_tmp[0] * g_ucDCRPercent + (UINT16)(OSD_Backlight * (100 - g_ucDCRPercent))) / 100;

    drv_color_DcrSlowAdjust(vip_8bit_tmp[0]);
    g_ucPreDCCLum = 0;
}
//#endif

#if 0 // (_DCR_MODE == _DCR_TYPE_2)
//--------------------------------------------------
// Description  : DCR Type 2 process
// Input Value  : None
// Output Value : None
// Return Value : None
//--------------------------------------------------

void drv_color_DcrDcr2(void)
{
    if (g_usRGBInfo < 48 )        // 16
    {
        vip_8bit_tmp[0] = ((UINT32)50 * 10000 / 48)  * g_usRGBInfo / 10000;

        if (vip_8bit_tmp[0] == 0)
        {
            vip_8bit_tmp[0] = ((((UINT32)50 * 10000 / 48) * g_usRGBInfo) >= 5000) ? 1 : 0;
        }
    }
    else if (g_usRGBInfo < 96)   // 32
    {
        vip_8bit_tmp[0] = ((UINT32)(220 - 50) * 10000 / (96 - 48))  * (g_usRGBInfo - 48) / 10000 + 50;
    }
    else if (g_usRGBInfo < 144)  // 48
    {
        vip_8bit_tmp[0] = ((UINT32)(230 - 220) * 10000 / (144 - 96))  * (g_usRGBInfo - 96) / 10000 + 220;
    }
    else if (g_usRGBInfo < 288)  // 96
    {
        vip_8bit_tmp[0] = 230;
    }
    else if (g_usRGBInfo < 336)  // 112
    {
        vip_8bit_tmp[0] = ((UINT32)(240 - 230) * 10000 / (336 - 288))  * (g_usRGBInfo - 288) / 10000 + 230;
    }
    else if (g_usRGBInfo < 576)  // 192
    {
        vip_8bit_tmp[0] = 240;
    }
    else if (g_usRGBInfo < 672)  // 224
    {
        vip_8bit_tmp[0] = ((UINT32)(255 - 240) * 10000 / (672 - 576))  * (g_usRGBInfo - 576) / 10000 + 240;
    }
    else                      // 240~
    {
        vip_8bit_tmp[0] = 255;
    }

    vip_8bit_tmp[0] = ((UINT16)vip_8bit_tmp[0] * g_ucDcrCtrl + (UINT16)(GET_COLOR_BACKLIGHT() * (100 - g_ucDcrCtrl))) / 100;

    drv_color_DcrSlowAdjust(vip_8bit_tmp[0]);

    vip_8bit_tmp[1] = ((UINT32)255 * 10000 / (765 - 0))  * (g_usRGBInfo - 0) / 10000;   // g_usRGBInfo ratio
    vip_8bit_tmp[1] = 255 - vip_8bit_tmp[1];

    // step by step DCC
    if (g_ucPreDCCLum > vip_8bit_tmp[1])                 // white to black
    {
        if ((g_ucPreDCCLum - vip_8bit_tmp[1]) > 20)
        {
            vip_8bit_tmp[1] = g_ucPreDCCLum - 10;
        }
        else
        {
            vip_8bit_tmp[1] = g_ucPreDCCLum - 1;
        }
    }
    else if (g_ucPreDCCLum < vip_8bit_tmp[1])            // black to white
    {
        if ((vip_8bit_tmp[1] - g_ucPreDCCLum) > 20)
        {
            vip_8bit_tmp[1] = g_ucPreDCCLum + 10;
        }
        else
        {
            vip_8bit_tmp[1] = g_ucPreDCCLum + 1;
        }
    }
    else
    {
        vip_8bit_tmp[1] = g_ucPreDCCLum;
    }
    // End of step by step

#if(_VIVID_COLOR_FUCTION == _ON)
    drv_color_AdjustDCRFillDCCTable(vip_8bit_tmp[1]);           // watch for the pData[1]
#endif

    g_ucPreDCCLum = vip_8bit_tmp[1];
}
#endif

#if 0 // not used
#if((_VIVID_COLOR_FUCTION == _ON) || (_DCR_MODE == _DCR_TYPE_3))
//--------------------------------------------------
// Description  : Adjust Normalize Factor
// Input Value  : usHWidth, usVHeight)
// Output Value : None
//--------------------------------------------------
void drv_color_AdjustNormalizeFactor(UINT16 usHWidth, UINT16 usVHeight)
{
    rtdf_pageSelect(_PAGE7);

    ((UINT32 *)vip_8bit_tmp)[0] = (UINT32)4194304 * 255 / usHWidth / usVHeight;

//    	rtdf_maskb(_P7_DCC_CTRL0_C7, ~(_BIT1 | _BIT0), _PAGE0);
    rtdf_maskScalerPort(_P7_DCC_CTRL0_C7, ~(_BIT1 | _BIT0), _PAGE0);

//    	rtdf_outb(_P7_DCC_ACCESS_PORT_C9, _DCC_P0_NOR_FACTOR_H_00);
    rtdf_writeScalerPort(_P7_DCC_ACCESS_PORT_C9, _DCC_P0_NOR_FACTOR_H_00);

    ((UINT32 *)vip_8bit_tmp)[0] &= 0x3fffff;

//	rtdf_outb(_P7_DCC_DATA_PORT_CA,vip_8bit_tmp[1]);
    rtdf_writeScalerPort(_P7_DCC_DATA_PORT_CA,vip_8bit_tmp[1]);
//	rtdf_outb(_P7_DCC_DATA_PORT_CA,vip_8bit_tmp[2]);
    rtdf_writeScalerPort(_P7_DCC_DATA_PORT_CA,vip_8bit_tmp[2]);
//	rtdf_outb(_P7_DCC_DATA_PORT_CA,vip_8bit_tmp[3]);
    rtdf_writeScalerPort(_P7_DCC_DATA_PORT_CA,vip_8bit_tmp[3]);
    //CScalerWrite(_P7_DCC_DATA_PORT_CA, 3, &pData[1], _NON_AUTOINC);

}
#endif // End of #if((_VIVID_COLOR_FUCTION == _ON) || (_DCR_MODE == _DCR_TYPE_3))
#endif

#if 0//(_DCR_MODE == _DCR_TYPE_3)
//--------------------------------------------------
// Description  : Initialized DCC and DCR function
// Input Value  : None
// Output Value : None
// Return Value : None
//--------------------------------------------------
void drv_color_DcrInitial(void)
{
    UINT8 i = 0;

    drv_color_AdjustNormalizeFactor(stDisplayInfo.DHWidth, stDisplayInfo.DVHeight);  // Normalize factor
    rtdf_pageSelect(_PAGE7);
//	i = rtdf_inb(_P7_DCC_CTRL0_C7);
    i = rtdf_readScalerPort(_P7_DCC_CTRL0_C7);
    if ((i& _BIT7) == 0)  // no use dcc
    {
//        	rtdf_outb(_P7_DCC_CTRL1_C8, 0x00); // disable DCC gain
        rtdf_writeScalerPort(_P7_DCC_CTRL1_C8, 0x00); // disable DCC gain
//        	rtdf_outb(_P7_DCC_CTRL0_C7, 0x40); // Y_formula 1, page0
        rtdf_writeScalerPort(_P7_DCC_CTRL0_C7, 0x40); // Y_formula 1, page0
//        	rtdf_outb(_P7_DCC_ACCESS_PORT_C9, 0x03);
        rtdf_writeScalerPort(_P7_DCC_ACCESS_PORT_C9, 0x03);
        for (i=0; i<9; i++)                       // page0 setting
        {
//            		rtdf_outb(_P7_DCC_DATA_PORT_CA, tDCC_PAGE0[i]);
            rtdf_writeScalerPort(_P7_DCC_DATA_PORT_CA, tDCC_PAGE0[i]);
        }
    }
    else      // use dcc color type
    {
        rtdf_maskScalerPort(_P7_DCC_CTRL0_C7, ~(_BIT2 | _BIT1 | _BIT0), 0x00);    // BWL_EXP off page0

        rtdf_writeScalerPort(_P7_DCC_ACCESS_PORT_C9, 0x03);               // BBE off
        rtdf_writeScalerPort(_P7_DCC_DATA_PORT_CA, 0x00);

        rtdf_writeScalerPort(_P7_DCC_ACCESS_PORT_C9, 0x05);               // RH0/RH1 limiter off
        vip_8bit_tmp[0] = rtdf_readScalerPort(_P7_DCC_DATA_PORT_CA);
        vip_8bit_tmp[0] = vip_8bit_tmp[0] & 0x0f;
        rtdf_writeScalerPort(_P7_DCC_ACCESS_PORT_C9, 0x05);
        rtdf_writeScalerPort(_P7_DCC_DATA_PORT_CA, vip_8bit_tmp[0]);

        rtdf_writeScalerPort(_P7_DCC_ACCESS_PORT_C9, 0x07);
        rtdf_writeScalerPort(_P7_DCC_DATA_PORT_CA, 0x00);         // Y_MAX_LB =0, special detect
        rtdf_writeScalerPort(_P7_DCC_DATA_PORT_CA, 0xFF);         // Y_MIN_HB = 0xff, special detect
    }
    // daniel DCR add 070531
    g_ucDcrCnt = 1;                                         // reset counter
    g_ucDcrTargetPre = _PWM_HIGH + 10;                      // diff from the 4 value
    g_ucDcrTemp = _PWM_LOW + 5;                             // diff from the 4 value
    g_ucDcrPwm = GET_COLOR_BACKLIGHT();
    g_ucDcrTimer = 0;
}

#if(_ACER_HIGH_ACM == _ON)
//--------------------------------------------------
// Description  : A timer for DCR control.
// Input Value  : None
// Output Value : None
// Return Value : None
//--------------------------------------------------
void drv_color_Dcr5MinTimerCount(void)
{
    if (g_usDCR5MinTimer != 15000)
    {
        g_usDCR5MinTimer = g_usDCR5MinTimer + 1;
    }
    else
    {
        if (GET_DCR_ULTRA_MIN())
        {
            g_ucDcrPwm = _ULTRA_MIN;
            CLR_DCR_ULTRA_MIN();
        }
        else
        {
            g_ucDcrPwm = _PWM_MIN;
            SET_DCR_ULTRA_MIN();
        }

        g_usDCR5MinTimer = 0;
    }
}
#endif  // End of #if(_ACER_HIGH_ACM == _ON)

//--------------------------------------------------
// Description  : Main function of DCR type 3.
// Input Value  : None
// Output Value : None
// Return Value : None
//--------------------------------------------------
void drv_color_DcrDcr3(void)
{
    UINT8 S0Percentage = 0;          // daniel 20070626
    UINT8 S0S6Total = 0;     // daniel 20070626
    UINT8 diff = 0;
    bit bTargetChange = 0;

    rtdf_pageSelect(_PAGE7);

    rtdf_maskScalerPort(_P7_DCC_CTRL0_C7, ~(_BIT7 | _BIT1 | _BIT0), _BIT7);   // enable DCC , page0
    rtdf_writeScalerPort(_P7_DCC_ACCESS_PORT_C9, 0x0C);

    for (diff = 0; diff < 11; diff++)
        vip_8bit_tmp[diff] = rtdf_readScalerPort(_P7_DCC_DATA_PORT_CA);
    //CScalerRead(_P7_DCC_DATA_PORT_CA, 11, vip_8bit_tmp, _NON_AUTOINC);

//  Y_MAX_VAL = vip_8bit_tmp[2];   Y_MIN_VAL = vip_8bit_tmp[3];   S0_VALUE  = vip_8bit_tmp[4];

    S0S6Total = vip_8bit_tmp[4] + vip_8bit_tmp[5] + vip_8bit_tmp[6] + vip_8bit_tmp[7] + vip_8bit_tmp[8] + vip_8bit_tmp[9] + vip_8bit_tmp[10];  // daniel 20070626
    S0Percentage = (UINT16)vip_8bit_tmp[4] * 100 / 255;              // S0_VALUE %
    diff = rtdf_readScalerPort(VGIP_CHN1_CTRL_0x0a10);
    diff &= _BIT1;
    diff = diff >>1;
    if (S0Percentage < 80)
    {
        if ((bit)diff)
        {
            if (vip_8bit_tmp[3] > 252)                     // Y_MIN_VAL > 252
            {
                g_ucDcrPwm = _PWM_MAX;
            }
            else
            {
                g_ucDcrPwm = _PWM_HIGH;
            }
        }
        else                        // analog input
        {
            if (vip_8bit_tmp[3] > 215)                 // Y_MIN_VAL > 215
            {
                g_ucDcrPwm = _PWM_MAX;
            }
            else if (S0S6Total <= 1)        // daniel 20070626
            {
                g_ucDcrPwm = _PWM_MAX;          // daniel 20070626
            }
            else
            {
                g_ucDcrPwm = _PWM_HIGH;
            }
        }
    }
    else if (S0Percentage < 90)
    {
        if (vip_8bit_tmp[2] > 220)                         // Y_MAX_VAL > 220
        {
            g_ucDcrPwm = _PWM_HIGH;
        }
        else
        {
            g_ucDcrPwm = _PWM_LOW;
        }
    }
    else
    {
        if ((bit)diff)	// digital
        {
            if (vip_8bit_tmp[2] < 4)                       // Y_MAX_VAL < 4
            {
                g_ucDcrPwm = _PWM_MIN;
            }
            else if (vip_8bit_tmp[2] > 220)                // Y_MAX_VAL > 220
            {
                g_ucDcrPwm = _PWM_HIGH;
            }
            else
            {
                g_ucDcrPwm = _PWM_LOW;
            }
        }
        else                    // analog input
        {
#if(_ACER_HIGH_ACM == _ON)

            if (vip_8bit_tmp[2] < 10)                      // Y_MAX_VAL < 10  test
            {
                if ((g_ucDcrTemp == _PWM_MIN) || (g_ucDcrTemp == _ULTRA_MIN))
                {
                    drv_color_Dcr5MinTimerCount();
                }
                else
                {
                    SET_DCR_ULTRA_MIN();
                    g_ucDcrPwm = _PWM_MIN;
                    g_usDCR5MinTimer = 0;
                }
            }
#else
            if (vip_8bit_tmp[2] < 10)                      // Y_MAX_VAL < 10
            {
                g_ucDcrPwm = _PWM_MIN;
            }
#endif
            else if (vip_8bit_tmp[2] > 220)                // Y_MAX_VAL > 220
            {
                g_ucDcrPwm = _PWM_HIGH;
            }
            else
            {
                g_ucDcrPwm = _PWM_LOW;
            }
        }
    }

    if (!GET_DCR_STATUS())
    {
        g_ucDcrPwm = GET_COLOR_BACKLIGHT();
    }

    // ------- step by step PWM -------------//
    vip_8bit_tmp[1] = g_ucDcrCurrentPWM;

    if (g_ucDcrTemp == g_ucDcrPwm)                      // target_pre = target_now
    {
        bTargetChange = 0;
    }
    else
    {
        bTargetChange = 1;
        g_ucDcrCnt = 1;
    }

    //****************************************************************
    // Suppose 1 -> 400 takes 8sec (DCR3 runs every 20msec).
    // g_usDcrStep inidicate how many g_ucDcrCnt counts, then PWM +/- 1
    //****************************************************************
    if (bTargetChange)
    {
        if (vip_8bit_tmp[1] > g_ucDcrPwm)              // current > target_now
        {
            diff = vip_8bit_tmp[1] - g_ucDcrPwm;
            g_usDcrStep = 4000 / diff;          // how many g_ucDcrCnt, then PWM +/- 1
        }
        else
        {
            diff = g_ucDcrPwm - vip_8bit_tmp[1];
            g_usDcrStep = 4000 / diff;
        }
    }

    if (vip_8bit_tmp[1] == g_ucDcrPwm)
    {
        vip_8bit_tmp[1] = vip_8bit_tmp[1];
        g_ucDcrCnt = 1;
    }
    else if (((g_ucDcrTargetPre == _PWM_MAX) && (g_ucDcrPwm == _PWM_MIN))
             || ((g_ucDcrTargetPre1 == _PWM_MAX) && (g_ucDcrTargetPre != _PWM_MIN) && (g_ucDcrPwm == _PWM_MIN)) )   // test
    {
        if (g_ucDcrCnt % ((g_usDcrStep / 10) * 2) == 0)     // 16s      MAX-MIN
        {
            fw_timer_event_DelayXms((g_usDcrStep % 10)* 2 * 2);
            vip_8bit_tmp[1] = vip_8bit_tmp[1] - 1;
        }
    }
    else if (vip_8bit_tmp[1] > g_ucDcrPwm)
    {
        if (g_ucDcrCnt % (g_usDcrStep / 10) == 0)               // 8s
        {
            fw_timer_event_DelayXms((g_usDcrStep % 10)* 2);
            vip_8bit_tmp[1] = vip_8bit_tmp[1] - 1;
        }
    }
    else if (((g_ucDcrTargetPre == _PWM_MIN) && (g_ucDcrPwm == _PWM_MAX))
             || ((g_ucDcrTargetPre1 == _PWM_MIN) && (g_ucDcrTargetPre != _PWM_MAX) && (g_ucDcrPwm == _PWM_MAX)) )   // test
    {
        if (g_ucDcrCnt % ((g_usDcrStep / 10) * 2) == 0)     // 16s      MIN-MAX
        {
            fw_timer_event_DelayXms((g_usDcrStep % 10) * 2 * 2);
            vip_8bit_tmp[1] = vip_8bit_tmp[1] + 1;
        }
    }
    else                                        // pData[1] < g_ucDcrPwm
    {
        if (g_ucDcrCnt % (g_usDcrStep / 10) == 0)               // 8s
        {
            fw_timer_event_DelayXms((g_usDcrStep % 10)* 2);
            vip_8bit_tmp[1] = vip_8bit_tmp[1] + 1;
        }
    }

    g_ucDcrCurrentPWM  = vip_8bit_tmp[1];
    vip_8bit_tmp[1] = ((UINT16)vip_8bit_tmp[1] * g_ucDCRPercent + (UINT16)(GET_COLOR_BACKLIGHT()* (100 - g_ucDCRPercent))) / 100;
//    CAdjustBacklight(vip_8bit_tmp[1]);
    SET_PANEL_BACKLIGHT(vip_8bit_tmp[1]);

    if (bTargetChange)
    {
        g_ucDcrTargetPre1 = g_ucDcrTargetPre;
        g_ucDcrTargetPre = g_ucDcrTemp;
        g_ucDcrTemp = g_ucDcrPwm;
    }

    g_ucDcrCnt = g_ucDcrCnt + 1;
}

#endif

//--------------------------------------------------
// Description  : Step by step adjust the backlight
//                by control PWM
// Input Value  : ucValue
// Output Value : None
// Return Value : None
//--------------------------------------------------
void drv_color_DcrSlowAdjust(UINT8 ucValue)
{
    vip_8bit_tmp[0] = ucValue;

    // Step by step PWM
    if (g_ucPreBacklightPWM > vip_8bit_tmp[0])             // white to black
    {
        if ((g_ucPreBacklightPWM - vip_8bit_tmp[0]) > 20)
        {
            vip_8bit_tmp[0] = g_ucPreBacklightPWM - 10;
        }
        else
        {
            vip_8bit_tmp[0] = g_ucPreBacklightPWM - 1;
        }
    }
    else if (g_ucPreBacklightPWM < vip_8bit_tmp[0])        // black to white
    {
        if ((vip_8bit_tmp[0] - g_ucPreBacklightPWM) > 20)
        {
            vip_8bit_tmp[0] = g_ucPreBacklightPWM + 10;
        }
        else
        {
            vip_8bit_tmp[0] = g_ucPreBacklightPWM + 1;
        }
    }
    else
    {
        vip_8bit_tmp[0] = g_ucPreBacklightPWM;
    }
    // End of step by step

    g_ucPreBacklightPWM = vip_8bit_tmp[0];

    //    CAdjustBacklight(vip_8bit_tmp[0]);
    SET_PANEL_BACKLIGHT(vip_8bit_tmp[0]);


}
//**********************************************************************************************



INT16 drv_color_Cosine(INT16 fDegree)
{
    //Lewis, the curve value table have multiplied by 1024
    UINT16 code fCosCurveAngle[13] = {   0,   6,  12,  18,  24,  30,  37,  44,  52,  60,  68,  78, 90};
    UINT16 code fCosCurveValue[13] = {1024, 1018, 1002, 974, 935, 887, 818, 737, 630, 512, 384, 213,  0};
    UINT8 i,cQuadrant;
    INT16 fDegreeTemp, fDegreeSpan, fCosValueSpan, fCosValue;

    if (fDegree < 0)
        fDegree += 360;

    fDegree = fDegree%360;

    //locate degree on which quadrant
    cQuadrant = (UINT8)(fDegree / 90);
    cQuadrant++;
    if (cQuadrant==1) //first quadrant
        fDegree = fDegree;
    else if (cQuadrant==2) //second quadrant
        fDegree = 180 - fDegree;
    else if (cQuadrant==3) //third quadrant
        fDegree = fDegree - 180;
    else if (cQuadrant==4) //forth quadrant
        fDegree = 360 -fDegree;
    //locate degree on which quadrant

    if (fDegree<=fCosCurveAngle[0])
        fCosValue = fCosCurveValue[0];
    else
    {
        for (i=0; fCosCurveAngle[i] < fDegree; i++);

        i--;
        fDegreeTemp = fDegree-fCosCurveAngle[i];
        fDegreeSpan = fCosCurveAngle[i+1]-fCosCurveAngle[i];
        fCosValueSpan = fCosCurveValue[i+1]-fCosCurveValue[i];
        fCosValue = fCosCurveValue[i]+fDegreeTemp*fCosValueSpan/fDegreeSpan;
    }

    if (cQuadrant==2 || cQuadrant==3)
        fCosValue = fCosValue*(-1);

    return fCosValue;
}

INT16 drv_color_Sine(INT16 fDegree)
{
    //Lewis, this table have multiplied by 1024
    UINT16 code fSinCurveAngle[13] = {0,   6,  12,  18,  24,  30,  37,  44,  52,  60,  68,  78, 90};
    UINT16 code fSinCurveValue[13] = {0, 107, 213, 316, 416, 512, 616, 711, 807, 887, 949, 1002,  1024};
    UINT8 i,cQuadrant;
    INT16 fDegreeTemp,fDegreeSpan,fSinValueSpan,fSinValue;

    if (fDegree < 0)
        fDegree += 360;

    fDegree = fDegree%360;

    //locate degree on which quadrant
    cQuadrant = (UINT8)(fDegree / 90);
    cQuadrant++;
    if (cQuadrant==1) //first quadrant
        fDegree = fDegree;
    else if (cQuadrant==2) //second quadrant
        fDegree = 180 - fDegree;
    else if (cQuadrant==3) //third quadrant
        fDegree = fDegree - 180;
    else if (cQuadrant==4) //forth quadrant
        fDegree = 360 -fDegree;
    //locate degree on which quadrant

    if (fDegree<=fSinCurveAngle[0])
        fSinValue = fSinCurveValue[0];
    else
    {
        for (i=0; fSinCurveAngle[i] < fDegree; i++);

        i--;
        fDegreeTemp = fDegree-fSinCurveAngle[i];
        fDegreeSpan = fSinCurveAngle[i+1]-fSinCurveAngle[i];
        fSinValueSpan = fSinCurveValue[i+1]-fSinCurveValue[i];
        fSinValue = fSinCurveValue[i]+fDegreeTemp*fSinValueSpan/fDegreeSpan;
    }

    if (cQuadrant==3 || cQuadrant==4)
        fSinValue = fSinValue*(-1);

    return fSinValue;
}

void drv_color_MatrixMultiply(INT16 (*arrayA)[3], INT16 (*arrayB)[3], INT16 (*arrayC)[3])
{
    UINT8 i, j;
    INT32 Temp[3][3];

    for (i=0; i<3; i++)
    {
        for (j=0; j<3; j++)
        {
            Temp[i][j] = ((INT32)arrayA[i][0]*arrayB[0][j]>>2)
                         + ((INT32)arrayA[i][1]*arrayB[1][j]>>2)
                         + ((INT32)arrayA[i][2]*arrayB[2][j]>>2);

            //arrayC[i][j] = (INT16)(Temp[i][j]>>8);//731401
        }
    }
//731401***
    for (i=0; i<3; i++)
        for (j=0; j<3; j++)
            arrayC[i][j] = (INT16)(Temp[i][j]>>8);
//731401###


}

void drv_color_Rhue(void)
{
    rtdf_pageSelect(_PAGE0);
    rtdf_maskScalerPort(_P0_COLOR_CTRL_62,0xc7,0x08);
    rtdf_CScalerWriteScalerPort(_P0_SRGB_ACCESS_PORT_63, 6, vip_8bit_tmp, _NON_INC);
}

void drv_color_Ghue(void)
{
    rtdf_pageSelect(_PAGE0);
    rtdf_maskScalerPort(_P0_COLOR_CTRL_62,0xc7,0x10);
    rtdf_CScalerWriteScalerPort(_P0_SRGB_ACCESS_PORT_63, 6, vip_8bit_tmp, _NON_INC);
}

void drv_color_Bhue(void)
{
    rtdf_pageSelect(_PAGE0);
    rtdf_maskScalerPort(_P0_COLOR_CTRL_62,0xc7,0x18);
    rtdf_CScalerWriteScalerPort(_P0_SRGB_ACCESS_PORT_63, 6, vip_8bit_tmp, _NON_INC);
}

#if 0
void drv_color_SaturationHue(UINT8  ucValue, BOOL bSaturationIndex)
{
//Lewis, this table have multiplied by 1024
    INT16 code YUV2RGB[3][3] =
    {
        {1024,        0,    1436},
        {1024,   -342,   -731},
        {1024,   1815,        0}
    };

//Lewis, this table have multiplied by 1024
    INT16 code RGB2YUV[3][3] =
    {
        {306,     601,   117},
        {-173,   -339,    512},
        {512,   -429,   -83}
    };

    UINT8 i, j;
    INT16 temp_buff0[3][3];
    //731401***
    //INT16 temp_buff1[3][3];
    //INT16 temp_buff2[3][3];
    INT16 MATRIX_GAIN_HUE[3][3];
    //INT16 MATRIX_GAIN[3][3];
    //INT16 MATRIX_HUE[3][3];
    //731401###
    INT16 temp_hue, temp_saturation;

    if (bSaturationIndex == _TRUE) //adjust saturation
    {
        temp_saturation=(INT16)ucValue;
        temp_hue=(INT16)GET_HUE() - 50;
    }
    else
    {
        temp_saturation = (INT16)GET_SATURATION();
        temp_hue=(INT16)ucValue - 50;
    }

    //Lewis 20060717, to prevent adjust CSC matrix, the saturation overflow
    if (temp_saturation>75)
        temp_saturation = 75;
    else if (temp_saturation<25)
        temp_saturation = 25;

    temp_hue = temp_hue*19>>5; //Lewis, 20060720, limited Hue range to prevent grey bar have color

//731401***
//Lewis, this table have multiplied by 1024
    MATRIX_GAIN_HUE[0][0] = 1024;
    MATRIX_GAIN_HUE[0][1] = 0;
    MATRIX_GAIN_HUE[0][2] = 0;
    MATRIX_GAIN_HUE[1][0] = 0;
    MATRIX_GAIN_HUE[1][1] = ((temp_saturation<<7)/25)<<2;
    MATRIX_GAIN_HUE[1][2] = 0;
    MATRIX_GAIN_HUE[2][0] = 0;
    MATRIX_GAIN_HUE[2][1] = 0;
    MATRIX_GAIN_HUE[2][2] = MATRIX_GAIN_HUE[1][1];

    drv_color_MatrixMultiply(MATRIX_GAIN_HUE,RGB2YUV,temp_buff0);

//Lewis, this table have multiplied by 1024
    MATRIX_GAIN_HUE[0][0] = 1024;
    MATRIX_GAIN_HUE[0][1] = 0;
    MATRIX_GAIN_HUE[0][2] = 0;
    MATRIX_GAIN_HUE[1][0] = 0;
    MATRIX_GAIN_HUE[1][1] = drv_color_Cosine(temp_hue);
    MATRIX_GAIN_HUE[1][2] = drv_color_Sine(temp_hue);
    MATRIX_GAIN_HUE[2][0] = 0;
    MATRIX_GAIN_HUE[2][1] = (-1)*MATRIX_GAIN_HUE[1][2];//-CAdjustSine(temp);
    MATRIX_GAIN_HUE[2][2] = MATRIX_GAIN_HUE[1][1]; //CAdjustCosine(temp);


    drv_color_MatrixMultiply(MATRIX_GAIN_HUE,temp_buff0,temp_buff0);
    drv_color_MatrixMultiply(YUV2RGB,temp_buff0,temp_buff0);

    for (i=0; i<3; i++)
        for (j=0; j<3; j++)
        {
            if (i==j)
                temp_buff0[i][j]-=1024;

            temp_buff0[i][j] = temp_buff0[i][j]>>1;

            if (temp_buff0[i][j]>255)
                temp_buff0[i][j] = 255;
            else if (temp_buff0[i][j]<-256)
                temp_buff0[i][j] = -256;
        }

    rtdf_pageSelect(_PAGE0);
    rtdf_setBitsScalerPort(_P0_COLOR_CTRL_62, _BIT2);//sRGB enable

    //Write R
    //frank CScalerSetBit(_COLOR_CTRL_62,0xc7,0x08);
    vip_8bit_tmp[0] =(temp_buff0[0][0]<0?1:0);
    vip_8bit_tmp[1]=(UINT8)(temp_buff0[0][0]);
    vip_8bit_tmp[2] =(temp_buff0[0][1]<0?1:0);
    vip_8bit_tmp[3]=(UINT8)(temp_buff0[0][1]);
    vip_8bit_tmp[4] =(temp_buff0[0][2]<0?1:0);
    vip_8bit_tmp[5]=(UINT8)(temp_buff0[0][2]);

//	CAdjustSetsRGBRhue();
    drv_color_Rhue();

    //Write G
    //frank CScalerSetBit(_COLOR_CTRL_62,0xc7,0x10);

    vip_8bit_tmp[0] =(temp_buff0[1][0]<0?1:0);
    vip_8bit_tmp[1]=(UINT8)(temp_buff0[1][0]);
    vip_8bit_tmp[2] =(temp_buff0[1][1]<0?1:0);
    vip_8bit_tmp[3]=(UINT8)(temp_buff0[1][1]);
    vip_8bit_tmp[4] =(temp_buff0[1][2]<0?1:0);
    vip_8bit_tmp[5]=(UINT8)(temp_buff0[1][2]);

//	CAdjustSetsRGBGhue();
    drv_color_Ghue();

    //Write B

    vip_8bit_tmp[0] =(temp_buff0[2][0]<0?1:0);
    vip_8bit_tmp[1]=(UINT8)(temp_buff0[2][0]);
    vip_8bit_tmp[2] =(temp_buff0[2][1]<0?1:0);
    vip_8bit_tmp[3]=(UINT8)(temp_buff0[2][1]);
    vip_8bit_tmp[4] =(temp_buff0[2][2]<0?1:0);
    vip_8bit_tmp[5]=(UINT8)(temp_buff0[2][2]);
//731401###
//	CAdjustSetsRGBBhue();
    drv_color_Bhue();

    rtdf_maskScalerPort(_P0_COLOR_CTRL_62, 0xff, 0x80);			// cftsai 20061031 added, ready to write

}
#else
void drv_color_Hue_sRGB(UINT8  ucValue)
{
//Lewis, this table have multiplied by 1024
    INT16 code YUV2RGB[3][3] =
    {
        {1024,        0,    1436},
        {1024,   -342,   -731},
        {1024,   1815,        0}
    };

//Lewis, this table have multiplied by 1024
    INT16 code RGB2YUV[3][3] =
    {
        {306,     601,   117},
        {-173,   -339,    512},
        {512,   -429,   -83}
    };

    UINT8 i, j;
    INT16 temp_buff0[3][3];
    //731401***
    //INT16 temp_buff1[3][3];
    //INT16 temp_buff2[3][3];
    INT16 MATRIX_GAIN_HUE[3][3];
    //INT16 MATRIX_GAIN[3][3];
    //INT16 MATRIX_HUE[3][3];
    //731401###
    INT16 temp_hue, temp_saturation;
#if 0
    if (bSaturationIndex == _TRUE) //adjust saturation
    {
        temp_saturation=(INT16)ucValue;
        temp_hue=(INT16)GET_HUE() - 50;
    }
    else
    {
        temp_saturation = (INT16)GET_SATURATION();
        temp_hue=(INT16)ucValue - 50;
    }

    //Lewis 20060717, to prevent adjust CSC matrix, the saturation overflow
    if (temp_saturation>75)
        temp_saturation = 75;
    else if (temp_saturation<25)
        temp_saturation = 25;
#else

temp_saturation = 50;
temp_hue=(INT16)ucValue - 50;

#endif

    temp_hue = temp_hue*19>>5; //Lewis, 20060720, limited Hue range to prevent grey bar have color

//731401***
//Lewis, this table have multiplied by 1024
    MATRIX_GAIN_HUE[0][0] = 1024;
    MATRIX_GAIN_HUE[0][1] = 0;
    MATRIX_GAIN_HUE[0][2] = 0;
    MATRIX_GAIN_HUE[1][0] = 0;
    MATRIX_GAIN_HUE[1][1] = ((temp_saturation<<7)/25)<<2;
    MATRIX_GAIN_HUE[1][2] = 0;
    MATRIX_GAIN_HUE[2][0] = 0;
    MATRIX_GAIN_HUE[2][1] = 0;
    MATRIX_GAIN_HUE[2][2] = MATRIX_GAIN_HUE[1][1];

    drv_color_MatrixMultiply(MATRIX_GAIN_HUE,RGB2YUV,temp_buff0);

//Lewis, this table have multiplied by 1024
    MATRIX_GAIN_HUE[0][0] = 1024;
    MATRIX_GAIN_HUE[0][1] = 0;
    MATRIX_GAIN_HUE[0][2] = 0;
    MATRIX_GAIN_HUE[1][0] = 0;
    MATRIX_GAIN_HUE[1][1] = drv_color_Cosine(temp_hue);
    MATRIX_GAIN_HUE[1][2] = drv_color_Sine(temp_hue);
    MATRIX_GAIN_HUE[2][0] = 0;
    MATRIX_GAIN_HUE[2][1] = (-1)*MATRIX_GAIN_HUE[1][2];//-CAdjustSine(temp);
    MATRIX_GAIN_HUE[2][2] = MATRIX_GAIN_HUE[1][1]; //CAdjustCosine(temp);


    drv_color_MatrixMultiply(MATRIX_GAIN_HUE,temp_buff0,temp_buff0);
    drv_color_MatrixMultiply(YUV2RGB,temp_buff0,temp_buff0);

    for (i=0; i<3; i++)
        for (j=0; j<3; j++)
        {
            if (i==j)
                temp_buff0[i][j]-=1024;

            temp_buff0[i][j] = temp_buff0[i][j]>>1;

            if (temp_buff0[i][j]>255)
                temp_buff0[i][j] = 255;
            else if (temp_buff0[i][j]<-256)
                temp_buff0[i][j] = -256;
        }

    rtdf_pageSelect(_PAGE0);
    rtdf_setBitsScalerPort(_P0_COLOR_CTRL_62, _BIT2);//sRGB enable

    //Write R
    //frank CScalerSetBit(_COLOR_CTRL_62,0xc7,0x08);
    vip_8bit_tmp[0] =(temp_buff0[0][0]<0?1:0);
    vip_8bit_tmp[1]=(UINT8)(temp_buff0[0][0]);
    vip_8bit_tmp[2] =(temp_buff0[0][1]<0?1:0);
    vip_8bit_tmp[3]=(UINT8)(temp_buff0[0][1]);
    vip_8bit_tmp[4] =(temp_buff0[0][2]<0?1:0);
    vip_8bit_tmp[5]=(UINT8)(temp_buff0[0][2]);

//	CAdjustSetsRGBRhue();
    drv_color_Rhue();

    //Write G
    //frank CScalerSetBit(_COLOR_CTRL_62,0xc7,0x10);

    vip_8bit_tmp[0] =(temp_buff0[1][0]<0?1:0);
    vip_8bit_tmp[1]=(UINT8)(temp_buff0[1][0]);
    vip_8bit_tmp[2] =(temp_buff0[1][1]<0?1:0);
    vip_8bit_tmp[3]=(UINT8)(temp_buff0[1][1]);
    vip_8bit_tmp[4] =(temp_buff0[1][2]<0?1:0);
    vip_8bit_tmp[5]=(UINT8)(temp_buff0[1][2]);

//	CAdjustSetsRGBGhue();
    drv_color_Ghue();

    //Write B

    vip_8bit_tmp[0] =(temp_buff0[2][0]<0?1:0);
    vip_8bit_tmp[1]=(UINT8)(temp_buff0[2][0]);
    vip_8bit_tmp[2] =(temp_buff0[2][1]<0?1:0);
    vip_8bit_tmp[3]=(UINT8)(temp_buff0[2][1]);
    vip_8bit_tmp[4] =(temp_buff0[2][2]<0?1:0);
    vip_8bit_tmp[5]=(UINT8)(temp_buff0[2][2]);
//731401###
//	CAdjustSetsRGBBhue();
    drv_color_Bhue();

    rtdf_maskScalerPort(_P0_COLOR_CTRL_62, 0xff, 0x80);			// cftsai 20061031 added, ready to write

}


#endif

void drv_color_AdjustHue(UINT8  ucYPbPrhue)
{
    //drv_color_SaturationHue(ucYPbPrhue, _FALSE);
    drv_color_Hue_sRGB(ucYPbPrhue);//20091214  jason9.ccc, hue adjustemnt: only sRGB
}

void Color_SetUV_Sat(UINT8 Sat)
{
    UINT32 new_coeffs;
    UINT16 *table_index = 0;

    //ROSPrintf("Color_SetUV_Sat%d\n",Sat);
    table_index = tYUV2RGB_COEF_601_YCbCr;
//	Sat = (Sat > 128) ?  75+ (Sat*53 /128) : Sat;
    RTD_Log(LOGGER_INFO, "==>> Color_SetUV_Sat: Sat=%d\n",(UINT32)Sat);

    {
        new_coeffs =  (double)table_index[_YUV2RGB_k13] * Sat;
        new_coeffs = (new_coeffs >> 7) ;
        new_coeffs = (new_coeffs > 0x1ff) ? 0x1ff: new_coeffs;

        //vip_32bit_tmp[0] = table_index[_YUV2RGB_k11];
        //vip_32bit_tmp[0] = (vip_32bit_tmp[0]<<9);
        //vip_32bit_tmp[0]  = vip_32bit_tmp[0]+new_coeffs;
        vip_32bit_tmp[0]  = new_coeffs;
        //rtd_outl(D_YUV2RGB_TAB1_DATA_0_0x1f18, vip_32bit_tmp[0]);
        rtdf_maskl( D_YUV2RGB_TAB1_DATA_0_0x1f18, ~(0x1ff), vip_32bit_tmp[0]);


        new_coeffs = (double)table_index[_YUV2RGB_k32]  * Sat;
        new_coeffs = (new_coeffs >> 7) ;
        vip_32bit_tmp[0]  = (new_coeffs > 0x3ff) ? 0x3ff: new_coeffs;
        vip_32bit_tmp[0] = vip_32bit_tmp[0]<<8;

        new_coeffs = (double)table_index[_YUV2RGB_k23]  * Sat;
        new_coeffs = (new_coeffs >> 7) ;
        vip_32bit_tmp[1]  = (new_coeffs > 0xff) ? 0xff: new_coeffs;
        vip_32bit_tmp[0] += vip_32bit_tmp[1];
        vip_32bit_tmp[0] = (vip_32bit_tmp[0]<<8);

        new_coeffs = (double)table_index[_YUV2RGB_k22]  * Sat;
        new_coeffs = (new_coeffs >> 7) ;
        vip_32bit_tmp[1]  = (new_coeffs > 0xff) ? 0xff: new_coeffs;
        vip_32bit_tmp[0] += vip_32bit_tmp[1];
        rtd_outl(D_YUV2RGB_TAB1_DATA_1_0x1f1c, vip_32bit_tmp[0]);
    }


}

void drv_color_AdjustSaturation(UINT8  ucYPbPrSaturation)
{
    UINT32 Sat;

#if 0	// need to modify below code if input(R/G/B) swap
    WORD    PB, PR ;
    UINT8 temp;

    temp=ucYPbPrSaturation;

    if (GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR)
    {
        PR = (temp <= 50)  ? ((WORD)stYPbPrData.YPbPrGain[_RED] + ((50 - temp) << 2)) :
             ((WORD)stYPbPrData.YPbPrGain[_RED] - (temp - 50));

        PB = (temp <= 50)  ? ((WORD)stYPbPrData.YPbPrGain[_BLUE] + ((50 - temp) << 2)) :
             ((WORD)stYPbPrData.YPbPrGain[_BLUE] - (temp - 50));

        if (HIBYTE(PB)>0)
            PB  = HIBYTE(PB) < 0x02 ? 0x00FF : 0 ;
        if (HIBYTE(PR)>0)
            PR  = HIBYTE(PR) < 0x02 ? 0x00FF : 0 ;

#if(_ADC_INPUT_SWAP_RG == _ON)

        pData[1] = PR;
        pData[0] = stYPbPrData.YPbPrGain[_GREEN];
        pData[2] = PB;

#elif(_ADC_INPUT_SWAP_RB == _ON)

        pData[2] = PR;
        pData[1] = stYPbPrData.YPbPrGain[_GREEN];
        pData[0] = PB;

#elif(_ADC_INPUT_SWAP_GB == _ON)

        pData[0] = PR;
        pData[2] = stYPbPrData.YPbPrGain[_GREEN];
        pData[1] = PB;

#else

        pData[0] = PR;
        pData[1] = stYPbPrData.YPbPrGain[_GREEN];
        pData[2] = PB;

#endif
        CAdjustSetYpbprSaturation();
    }
#endif

    //20091214 use yuv2rgb to adjust saturation: grey
    Sat = ucYPbPrSaturation;
    Sat = Sat*40.95/16;//Sat*43.20/16;
    Color_SetUV_Sat( Sat );

}
/*
//monitor usage
void drv_color_PeakingFilter(UINT8 ucPeaking)
{
	rtdf_pageSelect(_PAGE7);
	if((GET_INPUTSOURCE_TYPE()==_SOURCE_VGA)||(GET_INPUTSOURCE_TYPE()==_SOURCE_DVI)){
		rtdf_writeScalerPort(_P7_PKING_ACCESS_PORT_D6, 0x00);		// disable Peaking and Coring
		return;
	}else{

		if(GET_INPUTSOURCE_TYPE() == _SOURCE_TV){

			vip_8bit_tmp[0] = TVPeakingCoeff[ucPeaking][0];
			vip_8bit_tmp[1] = TVPeakingCoeff[ucPeaking][1];
			vip_8bit_tmp[2] = TVPeakingCoeff[ucPeaking][2];
		    	vip_8bit_tmp[3] = TVPeakingCoeff[ucPeaking][3];
			vip_8bit_tmp[4] = TVPeakingCoeff[ucPeaking][4];
		    	vip_8bit_tmp[5] = TVPeakingCoeff[ucPeaking][5];


		}else{

			vip_8bit_tmp[0] = PeakingCoeff[ucPeaking][0];
			vip_8bit_tmp[1] = PeakingCoeff[ucPeaking][1];
			vip_8bit_tmp[2] = PeakingCoeff[ucPeaking][2];
		    	vip_8bit_tmp[3] = PeakingCoeff[ucPeaking][3];
			vip_8bit_tmp[4] = PeakingCoeff[ucPeaking][4];
		    	vip_8bit_tmp[5] = PeakingCoeff[ucPeaking][5];


		}

	    	rtdf_writeScalerPort(_P7_PKING_ACCESS_PORT_D6, 0x80);
	    	rtdf_CScalerWriteScalerPort(_P7_PKING_DATA_PORT_D7, 6, vip_8bit_tmp, _NON_INC);
	    	rtdf_writeScalerPort(_P7_PKING_ACCESS_PORT_D6, 0x40);


	}

}
*/

#if 0
void drv_color_EnableDCC(UINT8 ucSelect)
{
    rtdf_pageSelect(_PAGE7);
    rtdf_maskScalerPort(_P7_DCC_CTRL1_C8, ~(_BIT7 | _BIT5), ((ucSelect << 7) | (ucSelect << 5)));
    rtdf_maskScalerPort(_P7_DCC_CTRL0_C7, ~(_BIT1 | _BIT0), (_BIT7 | _BIT0));
}

void drv_color_FillDCCTable(UINT8 ucNum)
{
    UINT8 i = 0;

    rtdf_pageSelect(_PAGE7);

//    i = rtdf_inb(_P7_DCC_CTRL0_C7);
//    i = (i & 0xfc) | _PAGE0;
//    rtdf_outb(_P7_DCC_CTRL0_C7, i);
    rtdf_maskScalerPort(_P7_DCC_CTRL0_C7, 0xfc, _PAGE0);

    // fill DCC control table
    rtdf_writeScalerPort(_P7_DCC_ACCESS_PORT_C9, 0x03);
    for (i =0; i<9; i++)
    {
        rtdf_writeScalerPort(_P7_DCC_DATA_PORT_CA, tDCC_CONTROL[ucNum][i]);
    }

    // switch to page 1
//    i = rtdf_inb(_P7_DCC_CTRL0_C7);
//    i = (i & 0xfc) | _PAGE1;
//    rtdf_outb(_P7_DCC_CTRL0_C7, i);
    rtdf_maskScalerPort(_P7_DCC_CTRL0_C7, 0xfc, _PAGE1);

    // fill DCC user curve table
    rtdf_writeScalerPort(_P7_DCC_ACCESS_PORT_C9, 0x00);
    for (i=0; i<23; i++)
    {
        rtdf_writeScalerPort(_P7_DCC_DATA_PORT_CA, tDCC_USERCURVE[ucNum][i]);
    }

    rtdf_writeScalerPort(_P7_DCC_ACCESS_PORT_C9, 0x12);
    rtdf_writeScalerPort(_P7_DCC_DATA_PORT_CA, 0x06);

    // enable DCC func when _EVENT_DEN_STOP
    WaitFor_DEN_STOP();

    rtdf_writeScalerPort(_P7_DCC_CTRL0_C7, 0xac);


#if(_DCR_MODE == _DCR_TYPE_3)
    if (GET_DCR_STATUS())           // dcr initial again after color setting
    {
        drv_color_DcrInitial();
    }
#endif

}
#endif

void drv_color_EnableDCC(UINT8 ucSelect, UINT8 Enable)
{
    rtdf_pageSelect(_PAGE7);
    rtdf_maskScalerPort(_P7_DCC_CTRL1_C8, ~(_BIT7 | _BIT5), ((ucSelect << 7) | (ucSelect << 5)));
    rtdf_maskScalerPort(_P7_DCC_CTRL0_C7, ~(_BIT1 | _BIT0), ( (Enable<<7) | _BIT0));
}

void drv_color_FillDCCTable(UINT8 ucNum)
{
    if (ucNum>4)//range: 0~4
    {
//		RTD_Log(LOGGER_INFO, "VIP DCC table index error!\n");
        return;
    }

    rtdf_pageSelect(_PAGE7);
    rtdf_maskScalerPort(_P7_DCC_CTRL0_C7, 0xfc, _PAGE0);

    // fill DCC control table
    rtdf_writeScalerPort(_P7_DCC_ACCESS_PORT_C9, 0x03);
#ifndef USE_DMA_COPY
    for (vip_i =0; vip_i<9; vip_i++)
        rtdf_writeScalerPort(_P7_DCC_DATA_PORT_CA, tDCC_CONTROL[ucNum][vip_i]);
#else
    drv_spi_dma_6093cpy(_P7_DCC_DATA_PORT_CA, _BANK2 , (UINT32)&tDCC_CONTROL[ucNum][vip_i], 9, 1);
#endif
    // switch to page 1
    rtdf_maskScalerPort(_P7_DCC_CTRL0_C7, 0xfc, _PAGE1);

    // fill DCC user curve table
    rtdf_writeScalerPort(_P7_DCC_ACCESS_PORT_C9, 0x00);
#ifndef USE_DMA_COPY
    for (vip_i=0; vip_i<23; vip_i++)
        rtdf_writeScalerPort(_P7_DCC_DATA_PORT_CA, tDCC_USERCURVE[ucNum][vip_i]);
#else
    drv_spi_dma_6093cpy(_P7_DCC_DATA_PORT_CA, _BANK2 , (UINT32)&tDCC_USERCURVE[ucNum][vip_i], 23, 1);
#endif

    rtdf_writeScalerPort(_P7_DCC_ACCESS_PORT_C9, 0x12);
    rtdf_writeScalerPort(_P7_DCC_DATA_PORT_CA, 0x06);

    // enable DCC func when _EVENT_DEN_STOP
    WaitFor_DEN_STOP();

    rtdf_writeScalerPort(_P7_DCC_CTRL0_C7, 0xac);


}


/*
void drv_color_FillICMTable(UINT8 ucNum)
{
    	UINT8 i = 0;

    	rtdf_pageSelect(_PAGE7);

    	for (i=0; i<6; i++)
    	{
        	rtdf_writeScalerPort(_P7_ICM_SEL_D1, (0x00 + i));
        	rtdf_writeScalerPort(_P7_ICM_ACCESS_PORT_D2, 0x00);
		for (vip_8bit_tmp[0] = 0; vip_8bit_tmp[0]<51;vip_8bit_tmp[0]++)
			rtdf_writeScalerPort(_P7_ICM_DATA_PORT_D3,tICM_TABLE[ucNum * 6 + i][vip_8bit_tmp[0]]);
        	//CScalerWrite(_P7_ICM_DATA_PORT_D3, 51, tICM_TABLE[ucNum * 6 + i], _NON_AUTOINC);
    	}
}

void drv_color_EnableICM(UINT8 ucSelect)
{
    	rtdf_pageSelect(_PAGE7);

    	if(ucSelect != _DISABLE)
    	{
        	rtdf_maskScalerPort(_P7_ICM_CTRL_D0, ~_BIT7, _BIT7);
    	}

    	vip_8bit_tmp[0] = rtdf_readScalerPort(_P7_ICM_CTRL_D0);

    	rtdf_writeScalerPort(_P7_ICM_CTRL_D0, ((vip_8bit_tmp[0] & 0xe0) | ucSelect));
}
*/
void drvif_color_icm_table_select(UINT8 which_table)
{
//		//check table element:
//    	for (vip_i=0; vip_i<=5 ;vip_i++)// CM0 ~ CM5
//    	{
//		for (vip_j=0; vip_j<52;vip_j++)
//			RTD_Log(LOGGER_INFO, "icm[%d][%d][%d]= %d\n", (UINT32)which_table, (UINT32)vip_i, (UINT32)vip_j, (UINT32)(tICM_ini[which_table][vip_i][vip_j]));
//    	}
    rtdf_pageSelect(_PAGE7);
    if (which_table!=0)
    {
        which_table = which_table-1;
        if (which_table>=6)//there're only 6 icm table in vip_paenl_xxx.c (0~5)
        {
            //RTD_Log(LOGGER_INFO, "\nVIP ICM table index error !!\n");
            return;
        }
        //	else
        //		RTD_Log(LOGGER_INFO, "\nVIP ICM which_table=%d\n", (UINT32)which_table);


        //enable ICM
        //	rtdf_setBitsScalerPort(_P7_ICM_CTRL_D0, _BIT7);	//ICM enable

        for (vip_i=0; vip_i<=5 ; vip_i++)// CM0 ~ CM5
        {
            if (tICM_ini[which_table][vip_i][0]==1)//CM enable
            {
                //CM enable bit=================================================================
                if (vip_i==0)//cm0 bit4
                    rtdf_setBitsScalerPort(_P7_ICM_CTRL_D0, _BIT4);
                else if (vip_i==1)//cm1 bit3
                    rtdf_setBitsScalerPort(_P7_ICM_CTRL_D0, _BIT3);
                else if (vip_i==2)//cm2 bit2
                    rtdf_setBitsScalerPort(_P7_ICM_CTRL_D0, _BIT2);
                else if (vip_i==3)//cm3 bit1
                    rtdf_setBitsScalerPort(_P7_ICM_CTRL_D0, _BIT1);
                else if (vip_i==4)//cm4 bit0
                    rtdf_setBitsScalerPort(_P7_ICM_CTRL_D0, _BIT0);
                else// if(i==5)//cm5 bit3
                    rtdf_setBitsScalerPort(_P7_ICM_SEL_D1, _BIT3);
                //=================================================================

                rtdf_writeScalerPort(_P7_ICM_SEL_D1, (0x00 + vip_i));
                rtdf_writeScalerPort(_P7_ICM_ACCESS_PORT_D2, 0x00);
#ifndef USE_DMA_COPY
                for (vip_j=1; vip_j<52; vip_j++)
                    rtdf_writeScalerPort(_P7_ICM_DATA_PORT_D3, tICM_ini[which_table][vip_i][vip_j]);
#else
                drv_spi_dma_6093cpy(_P7_ICM_DATA_PORT_D3, _BANK2 , (UINT32)tICM_ini[which_table][vip_i], 52, 1);
#endif
            }
            else
            {
                if (vip_i==0)//cm0 bit4
                    rtdf_clearBitsScalerPort(_P7_ICM_CTRL_D0, _BIT4);
                else if (vip_i==1)//cm1 bit3
                    rtdf_clearBitsScalerPort(_P7_ICM_CTRL_D0, _BIT3);
                else if (vip_i==2)//cm2 bit2
                    rtdf_clearBitsScalerPort(_P7_ICM_CTRL_D0, _BIT2);
                else if (vip_i==3)//cm3 bit1
                    rtdf_clearBitsScalerPort(_P7_ICM_CTRL_D0, _BIT1);
                else if (vip_i==4)//cm4 bit0
                    rtdf_clearBitsScalerPort(_P7_ICM_CTRL_D0, _BIT0);
                else// if(i==5)//cm5 bit3
                    rtdf_clearBitsScalerPort(_P7_ICM_SEL_D1, _BIT3);
            }

        }

        rtdf_setBitsScalerPort(_P7_ICM_CTRL_D0, _BIT7);	//ICM enable
    }
    else
        rtdf_clearBitsScalerPort(_P7_ICM_CTRL_D0, _BIT7);	//ICM disable

}



/*================================== Function ===============================*/

/*============================================================================*/
/**
 * ColorSet422To444
 * Set 4:2:2 to 4:4:4 interpolation of C path
 * @param <info> {disp-info structure}
 * @return {none}
 *
 */
void drv_color_set422to444(bit enable)
{
    if (enable)
    {
        rtdf_clearBits(ICH2_422TO444_CTRL_0x0e84, _BIT2);//Cr_dly_Cb (at the same pixel )
        rtdf_clearBits(ICH2_422TO444_CTRL_0x0e84, _BIT1);//dup, (Interpolation )
        rtdf_setBits(ICH2_422TO444_CTRL_0x0e84, _BIT0);//enable
    }
    else
    {
        rtdf_clearBits(ICH2_422TO444_CTRL_0x0e84, _BIT0);//disable
    }
}

/*============================================================================*/
/**
 * Color_WriteGamma
 * This function is used to write gamma table in single channel
 *
 * @param <pArray> 	{ gamma table lut}
 * @return 			{ void }
 *
 */

UINT32   bin_num,  G1 , Gz, C_gain0 , C_gain1 , index , scale;
UINT16 drv_color_S_Curve(UINT32 input_g_point ,UINT32 g_index)
{




    if (g_index<=index)
        G1=((g_index*g_index*10/index)*scale+5);
    else
        G1=(((INT32)(g_index-bin_num)*(INT32)(g_index-bin_num)*10/(INT32)(index-bin_num)+bin_num*10)*scale+5);


//RTD_Log(LOGGER_INFO, "%d , %d\n", (UINT32)(G1),(UINT32)g_index);

    if (g_index<=index)
        Gz=((G1/10-g_index*scale)*C_gain0+g_index*scale*10+5);
    else
    {
        Gz=((G1/10-g_index*scale)*C_gain1+g_index*scale*10+5);
    }




//RTD_Log(LOGGER_INFO, "%d , %d\n", (UINT32)(Gz/10),(UINT32)g_index);
    if (g_index ==0)
        g_index = 1;
    return (input_g_point*Gz)/((g_index)*4);
    //	return Gz/10;
}

void drv_color_colorwritegamma_s(UINT8 code *pArray, UINT8 s_curve_en)
{
//step1: transform dragon's gamma table to monitort's gamma table.---------------------------------------------
    for (vip_i=0; vip_i<128; vip_i++)
    {
        //RTD_Log(LOGGER_INFO, "\n[%d](%d): %x, %x, %x\n", (UINT32)(ii), (UINT32)(ii*3), (UINT32)(pArray[ii*3]),  (UINT32)(pArray[ii*3+1]),  (UINT32)(pArray[ii*3+2]));

        for (vip_j=0; vip_j<3; vip_j++)
            vip_16bit_tmp[vip_j] = 0;

        vip_16bit_tmp[0] = pArray[vip_i*3];

        vip_16bit_tmp[0] = (vip_16bit_tmp[0]<<2)+ ((pArray[vip_i*3 + 1] >> 6)&(0x03) ); //gi

        vip_16bit_tmp[1] = (pArray[vip_i*3 + 1] & (0x1f) );//d0

        vip_16bit_tmp[2] =  vip_16bit_tmp[0] +  vip_16bit_tmp[1];//gi_nxt

        if (s_curve_en==1)
        {
            if (index != 0)
            {
                vip_16bit_tmp[0] = drv_color_S_Curve(vip_16bit_tmp[0] ,2*vip_i -1)/10;
                vip_16bit_tmp[2] = drv_color_S_Curve(vip_16bit_tmp[2] ,2*vip_i)/10;

                vip_16bit_tmp[0]=(vip_16bit_tmp[0] >= 1020) ? 1020 : vip_16bit_tmp[0];
                vip_16bit_tmp[2]=(vip_16bit_tmp[2] >= 1020) ? 1020 : vip_16bit_tmp[2];
                //	RTD_Log(LOGGER_INFO, "%d\n", (UINT32)(vip_16bit_tmp[0]));
                //	RTD_Log(LOGGER_INFO, "%d\n", (UINT32)(vip_16bit_tmp[2]));
                //	RTD_Log(LOGGER_INFO, "%d , %d\n", (UINT32)(vip_16bit_tmp[2]),(UINT32)vip_i);
            }
        }

        for (vip_j=0; vip_j<3; vip_j++)
            vip_8bit_tmp[vip_j] = 0;

        vip_8bit_tmp[0] =  ( (vip_16bit_tmp[0]>>4) & (0x3f) );
        vip_8bit_tmp[1] =  ( ( (vip_16bit_tmp[0] & (0xf) )<<4 )+ ( (vip_16bit_tmp[2]>>8) & (0x03)) );
        vip_8bit_tmp[2] =  ( vip_16bit_tmp[2] & (0xff) );

        rtdf_writeScalerPort( _P0_GAMMA_PORT, vip_8bit_tmp[0]);
        rtdf_writeScalerPort( _P0_GAMMA_PORT, vip_8bit_tmp[1]);
        rtdf_writeScalerPort (_P0_GAMMA_PORT, vip_8bit_tmp[2]);

        if (vip_i==127)
            vip_16bit_tmp[1] = vip_16bit_tmp[2];//grid 1020

    }


//step2: for the last data: grid 1023---------------------------------------------
    for (vip_j=0; vip_j<3; vip_j++)
        vip_8bit_tmp[vip_j] = 0;

    vip_8bit_tmp[0] =  ( (vip_16bit_tmp[1]>>4) & (0x3f) );
    vip_8bit_tmp[1] =  ( (vip_16bit_tmp[1] & 0xf)<<4 + 0x0 );
    vip_8bit_tmp[2] =  0x0;

    rtdf_writeScalerPort( _P0_GAMMA_PORT, vip_8bit_tmp[0]);
    rtdf_writeScalerPort( _P0_GAMMA_PORT, vip_8bit_tmp[1]);
    rtdf_writeScalerPort (_P0_GAMMA_PORT, vip_8bit_tmp[2]);
//-------------------------------------------------------------------

}

#if 0
void drv_color_colorwritegamma(UINT8 code *pArray)
{
//step1: transform dragon's gamma table to monitort's gamma table.---------------------------------------------
    for (vip_i=0; vip_i<128; vip_i++)
    {
        //RTD_Log(LOGGER_INFO, "\n[%d](%d): %x, %x, %x\n", (UINT32)(ii), (UINT32)(ii*3), (UINT32)(pArray[ii*3]),  (UINT32)(pArray[ii*3+1]),  (UINT32)(pArray[ii*3+2]));

        for (vip_j=0; vip_j<3; vip_j++)
            vip_16bit_tmp[vip_j] = 0;

        vip_16bit_tmp[0] = pArray[vip_i*3];

        vip_16bit_tmp[0] = (vip_16bit_tmp[0]<<2)+ ((pArray[vip_i*3 + 1] >> 6)&(0x03) ); //gi

        vip_16bit_tmp[1] = (pArray[vip_i*3 + 1] & (0x1f) );//d0

        vip_16bit_tmp[2] =  vip_16bit_tmp[0] +  vip_16bit_tmp[1];//gi_nxt


        if (index != 0)
        {
            vip_16bit_tmp[0] = drv_color_S_Curve(vip_16bit_tmp[0] ,2*vip_i -1)/10;
            vip_16bit_tmp[2] = drv_color_S_Curve(vip_16bit_tmp[2] ,2*vip_i)/10;


            vip_16bit_tmp[0]=(vip_16bit_tmp[0] >= 1020) ? 1020 : vip_16bit_tmp[0];
            vip_16bit_tmp[2]=(vip_16bit_tmp[2] >= 1020) ? 1020 : vip_16bit_tmp[2];
            //RTD_Log(LOGGER_INFO, "%d\n", (UINT32)(vip_16bit_tmp[0]));
            //	RTD_Log(LOGGER_INFO, "%d\n", (UINT32)(vip_16bit_tmp[2]));
            //	RTD_Log(LOGGER_INFO, "%d , %d\n", (UINT32)(vip_16bit_tmp[2]),(UINT32)vip_i);
        }

        for (vip_j=0; vip_j<3; vip_j++)
            vip_8bit_tmp[vip_j] = 0;

        vip_8bit_tmp[0] =  ( (vip_16bit_tmp[0]>>4) & (0x3f) );
        vip_8bit_tmp[1] =  ( ( (vip_16bit_tmp[0] & (0xf) )<<4 )+ ( (vip_16bit_tmp[2]>>8) & (0x03)) );
        vip_8bit_tmp[2] =  ( vip_16bit_tmp[2] & (0xff) );

        rtdf_writeScalerPort( _P0_GAMMA_PORT, vip_8bit_tmp[0]);
        rtdf_writeScalerPort( _P0_GAMMA_PORT, vip_8bit_tmp[1]);
        rtdf_writeScalerPort (_P0_GAMMA_PORT, vip_8bit_tmp[2]);

        if (vip_i==127)
            vip_16bit_tmp[1] = vip_16bit_tmp[2];//grid 1020

    }


//step2: for the last data: grid 1023---------------------------------------------
    for (vip_j=0; vip_j<3; vip_j++)
        vip_8bit_tmp[vip_j] = 0;

    vip_8bit_tmp[0] =  ( (vip_16bit_tmp[1]>>4) & (0x3f) );
    vip_8bit_tmp[1] =  ( (vip_16bit_tmp[1] & 0xf)<<4 + 0x0 );
    vip_8bit_tmp[2] =  0x0;

    rtdf_writeScalerPort( _P0_GAMMA_PORT, vip_8bit_tmp[0]);
    rtdf_writeScalerPort( _P0_GAMMA_PORT, vip_8bit_tmp[1]);
    rtdf_writeScalerPort (_P0_GAMMA_PORT, vip_8bit_tmp[2]);
//-------------------------------------------------------------------

}
#endif

#if 0
void drv_color_set_inputgamma( UINT8 enable, UINT8 mode)
{
    rtdf_pageSelect(_PAGE9);//page 9

    if (enable==1)
    {
        if (mode==_InputGamma_RGB_Same)
        {
            rtdf_writeScalerPort(_P9_INPUT_GAMMA_CTRL , 0xf8);

            for (vip_i=0; vip_i<66; vip_i++)
                rtdf_writeScalerPort( _P9_INPUT_GAMMA_PORT, tInput_GAMMA_R[vip_i]);

            rtdf_writeScalerPort(_P9_INPUT_GAMMA_CTRL, 0x70);
        }
        else//_InputGamma_RGB_Sep
        {
            // R channel
            rtdf_writeScalerPort(_P9_INPUT_GAMMA_CTRL , 0xc0);
            for (vip_i=0; vip_i<66; vip_i++)
                rtdf_writeScalerPort( _P9_INPUT_GAMMA_PORT, tInput_GAMMA_R[vip_i]);

            // G channel
            rtdf_writeScalerPort(_P9_INPUT_GAMMA_CTRL , 0xd0);
            for (vip_i=0; vip_i<66; vip_i++)
                rtdf_writeScalerPort( _P9_INPUT_GAMMA_PORT, tInput_GAMMA_G[vip_i]);

            // B channel
            rtdf_writeScalerPort(_P9_INPUT_GAMMA_CTRL , 0xe0);
            for (vip_i=0; vip_i<66; vip_i++)
                rtdf_writeScalerPort( _P9_INPUT_GAMMA_PORT, tInput_GAMMA_B[vip_i]);

            rtdf_writeScalerPort(_P9_INPUT_GAMMA_CTRL, 0x40);
        }

    }
    else
        rtdf_writeScalerPort(_P9_INPUT_GAMMA_CTRL, 0x00);//disalbe access, and disable gamma

}
#endif

/*============================================================================*/
/**
 * Color_SetGamma
 * This function is used to set gamma table
 *
 * @param <display>	{ specify main/sub to set gamma }
 * @param <mode> 	{ gamma table type}
 * @return 			{ void }

 * Mode = 0 Gamma off
 * Mode = 1 always in Tab-1, rewrite Tab-1 for tGAMMA_1_R, tGAMMA_1_G, tGAMMA_1_B
 * Mode = 2 always in Tab-1, rewrite Tab-1 for tGAMMA_2_R, tGAMMA_2_G, tGAMMA_2_B
 * Mode = 3 always in Tab-2, rewrite Tab-2 for tGAMMA_3_R, tGAMMA_3_G, tGAMMA_3_B

 */
void drv_color_set_inputgamma( UINT8 enable, UINT8 mode)
{
    rtdf_pageSelect(_PAGE9);//page 9

    if (enable==_ENABLE)
    {
        if (mode==_InputGamma_RGB_Same)
        {
            rtdf_writeScalerPort(_P9_INPUT_GAMMA_CTRL , 0xf8);
#ifndef USE_DMA_COPY
            for (vip_i=0; vip_i<66; vip_i++)
                rtdf_writeScalerPort( _P9_INPUT_GAMMA_PORT, tInput_GAMMA_R[vip_i]);
#else
            drv_spi_dma_6093cpy(_P9_INPUT_GAMMA_PORT, _BANK2 , (UINT32)tInput_GAMMA_R, 66, 1);
#endif
            rtdf_writeScalerPort(_P9_INPUT_GAMMA_CTRL, 0x70);
        }
        else//_InputGamma_RGB_Sep
        {
            // R channel
            rtdf_writeScalerPort(_P9_INPUT_GAMMA_CTRL , 0xc0);
#ifndef USE_DMA_COPY
            for (vip_i=0; vip_i<66; vip_i++)
                rtdf_writeScalerPort( _P9_INPUT_GAMMA_PORT, tInput_GAMMA_R[vip_i]);
#else
            drv_spi_dma_6093cpy(_P9_INPUT_GAMMA_PORT, _BANK2 , (UINT32)tInput_GAMMA_R, 66, 1);
            rtd_maskb(0xfff3, ~_BIT5, 1);
            rtdf_writeScalerPort(_P9_INPUT_GAMMA_CTRL , 0xd0);
#endif
            // G channel
            rtdf_writeScalerPort(_P9_INPUT_GAMMA_CTRL , 0xd0);
#ifndef USE_DMA_COPY
            for (vip_i=0; vip_i<66; vip_i++)
                rtdf_writeScalerPort( _P9_INPUT_GAMMA_PORT, tInput_GAMMA_G[vip_i]);
#else
            drv_spi_dma_6093cpy(_P9_INPUT_GAMMA_PORT, _BANK2 , (UINT32)tInput_GAMMA_G, 66, 1);
#endif
            // B channel
            rtdf_writeScalerPort(_P9_INPUT_GAMMA_CTRL , 0xe0);
#ifndef USE_DMA_COPY
            for (vip_i=0; vip_i<66; vip_i++)
                rtdf_writeScalerPort( _P9_INPUT_GAMMA_PORT, tInput_GAMMA_B[vip_i]);
#else
            drv_spi_dma_6093cpy(_P9_INPUT_GAMMA_PORT, _BANK2 , (UINT32)tInput_GAMMA_B, 66, 1);
#endif
            rtdf_writeScalerPort(_P9_INPUT_GAMMA_CTRL, 0x40);
        }

    }
    else
        rtdf_writeScalerPort(_P9_INPUT_GAMMA_CTRL, 0x00);//disalbe access, and disable gamma

}

void drv_color_setgamma_s( UINT8 Mode, UINT8 s_curve_en)//void drvif_color_setgamma(UINT8 display, UINT8 Mode)
{
//	gamma_ctrl_1_RBUS gamma_ctrl_1_REG;
//	gamma_ctrl_2_RBUS gamma_ctrl_2_REG;

    if (Mode > 3) // added by Jerry, for reducing code size
        return;

    if (s_curve_en==1)
    {
        C_gain1=GET_S_LOW();
        C_gain0=GET_S_HIGH();
        index=GET_S_INDEX();
        scale = 4;
        bin_num=256;
    }

//	RTD_Log(LOGGER_INFO,"%d   %d   %d\n",(UINT32)C_gain0,(UINT32)C_gain1,(UINT32)index);

//	gamma_ctrl_1_REG.regValue = rtd_inl(GAMMA_GAMMA_CTRL_1_ADDR);
//	gamma_ctrl_2_REG.regValue = rtd_inl(GAMMA_GAMMA_CTRL_2_ADDR);

//	if(display  == _MAIN_DISPLAY ) {
    //gamma_ctrl_2_REG.main_tab_sel = 0;	// disable main gamma
    // remove "disable gamma" for PIP sub flash when TV scan channel start & end , 20071228 Jerry Wu
//		gamma_ctrl_1_REG.gamma_access_type = _Gamma_Full;
//		gamma_ctrl_1_REG.gamma_table_sel = 0; // tab-1
//		gamma_ctrl_1_REG.enable_access = 1; // writing table

//	}

//	rtd_outl(GAMMA_GAMMA_CTRL_2_ADDR, gamma_ctrl_2_REG.regValue);

    rtdf_pageSelect(_PAGE0);
    if (Mode != 0)
    {
        if (Mode == 1)
        {
//			RTD_Log(LOGGER_INFO,"write red gamma \n");
//			gamma_ctrl_1_REG.color_channel = 0;
//			rtd_outl(GAMMA_GAMMA_CTRL_1_ADDR, gamma_ctrl_1_REG.regValue);
            rtdf_writeScalerPort(_P0_GAMMA_CRTL , 0x80);//enable access
            drv_color_colorwritegamma_s(tGAMMA_1_R, s_curve_en);

//			RTD_Log(LOGGER_INFO,"write green gamma \n");
//			gamma_ctrl_1_REG.color_channel = 1;
//			rtd_outl(GAMMA_GAMMA_CTRL_1_ADDR, gamma_ctrl_1_REG.regValue);
            rtdf_writeScalerPort(_P0_GAMMA_CRTL , 0x90);
            drv_color_colorwritegamma_s(tGAMMA_1_G, s_curve_en);

//			RTD_Log(LOGGER_INFO,"write blue gamma \n");
//			gamma_ctrl_1_REG.color_channel = 2;
//			rtd_outl(GAMMA_GAMMA_CTRL_1_ADDR, gamma_ctrl_1_REG.regValue);
            rtdf_writeScalerPort(_P0_GAMMA_CRTL , 0xa0);
            drv_color_colorwritegamma_s(tGAMMA_1_B, s_curve_en);

            rtdf_writeScalerPort(_P0_GAMMA_CRTL, 0x40);//disalbe access, and enable gamma
        }
        //#if defined( _Second_Gamma) // marked by Jerry Wu 20071106
        else if (Mode == 2)
        {
//			RTD_Log(LOGGER_INFO,"write gamma 2\n");
//			gamma_ctrl_1_REG.color_channel = 0;
//			rtd_outl(GAMMA_GAMMA_CTRL_1_ADDR, gamma_ctrl_1_REG.regValue);
            rtdf_writeScalerPort(_P0_GAMMA_CRTL , 0x80);//enable access
            drv_color_colorwritegamma_s(tGAMMA_2_R, s_curve_en);
//			gamma_ctrl_1_REG.color_channel = 1;
//			rtd_outl(GAMMA_GAMMA_CTRL_1_ADDR, gamma_ctrl_1_REG.regValue);
            rtdf_writeScalerPort(_P0_GAMMA_CRTL , 0x90);
            drv_color_colorwritegamma_s(tGAMMA_2_G, s_curve_en);
//			gamma_ctrl_1_REG.color_channel = 2;
//			rtd_outl(GAMMA_GAMMA_CTRL_1_ADDR, gamma_ctrl_1_REG.regValue);
            rtdf_writeScalerPort(_P0_GAMMA_CRTL , 0xa0);
            drv_color_colorwritegamma_s(tGAMMA_2_B, s_curve_en);

            rtdf_writeScalerPort(_P0_GAMMA_CRTL, 0x40);//disalbe access, and enable gamma
        }
        else
            rtdf_writeScalerPort(_P0_GAMMA_CRTL, 0x00);//disalbe access, and disable gamma

    }
    else
        rtdf_writeScalerPort(_P0_GAMMA_CRTL, 0x00);//disalbe access, and disable gamma


#if 0
    if (Mode != 0)  // not bypass
    {

        if (display  == _MAIN_DISPLAY )
        {
            gamma_ctrl_1_REG.enable_access = 0; // disable writing table
            gamma_ctrl_2_REG.main_tab_sel = 1; // use tab-1
            gamma_ctrl_2_REG.overlay = 0; // use main as overlay
        }

    }
    else   //  bypass
    {
        if (display  == _MAIN_DISPLAY )
        {
            gamma_ctrl_2_REG.main_tab_sel = 0; // use tab-1
            gamma_ctrl_2_REG.overlay = 0; // use main as overlay

        }


    }


    rtd_outl(GAMMA_GAMMA_CTRL_1_ADDR, gamma_ctrl_1_REG.regValue);//20071217 jason for high level (white changes to megenta)
    rtd_outl(GAMMA_GAMMA_CTRL_2_ADDR, gamma_ctrl_2_REG.regValue);
#endif

}
#if 0
void drv_color_setgamma( UINT8 Mode)//void drvif_color_setgamma(UINT8 display, UINT8 Mode)
{
//	gamma_ctrl_1_RBUS gamma_ctrl_1_REG;
//	gamma_ctrl_2_RBUS gamma_ctrl_2_REG;

    if (Mode > 3) // added by Jerry, for reducing code size
        return;

    C_gain1=GET_S_LOW();
    C_gain0=GET_S_HIGH();
    index=GET_S_INDEX();
    scale = 4;
    bin_num=256;

    RTD_Log(LOGGER_INFO,"%d   %d   %d\n",(UINT32)C_gain0,(UINT32)C_gain1,(UINT32)index);

//	gamma_ctrl_1_REG.regValue = rtd_inl(GAMMA_GAMMA_CTRL_1_ADDR);
//	gamma_ctrl_2_REG.regValue = rtd_inl(GAMMA_GAMMA_CTRL_2_ADDR);

//	if(display  == _MAIN_DISPLAY ) {
    //gamma_ctrl_2_REG.main_tab_sel = 0;	// disable main gamma
    // remove "disable gamma" for PIP sub flash when TV scan channel start & end , 20071228 Jerry Wu
//		gamma_ctrl_1_REG.gamma_access_type = _Gamma_Full;
//		gamma_ctrl_1_REG.gamma_table_sel = 0; // tab-1
//		gamma_ctrl_1_REG.enable_access = 1; // writing table

//	}

//	rtd_outl(GAMMA_GAMMA_CTRL_2_ADDR, gamma_ctrl_2_REG.regValue);

    rtdf_pageSelect(_PAGE0);
    if (Mode != 0)
    {
        if (Mode == 1)
        {
//			RTD_Log(LOGGER_INFO,"write red gamma \n");
//			gamma_ctrl_1_REG.color_channel = 0;
//			rtd_outl(GAMMA_GAMMA_CTRL_1_ADDR, gamma_ctrl_1_REG.regValue);
            rtdf_writeScalerPort(_P0_GAMMA_CRTL , 0x80);//enable access
            drv_color_colorwritegamma(tGAMMA_1_R);

//			RTD_Log(LOGGER_INFO,"write green gamma \n");
//			gamma_ctrl_1_REG.color_channel = 1;
//			rtd_outl(GAMMA_GAMMA_CTRL_1_ADDR, gamma_ctrl_1_REG.regValue);
            rtdf_writeScalerPort(_P0_GAMMA_CRTL , 0x90);
            drv_color_colorwritegamma(tGAMMA_1_G);

//			RTD_Log(LOGGER_INFO,"write blue gamma \n");
//			gamma_ctrl_1_REG.color_channel = 2;
//			rtd_outl(GAMMA_GAMMA_CTRL_1_ADDR, gamma_ctrl_1_REG.regValue);
            rtdf_writeScalerPort(_P0_GAMMA_CRTL , 0xa0);
            drv_color_colorwritegamma(tGAMMA_1_B);

            rtdf_writeScalerPort(_P0_GAMMA_CRTL, 0x40);//disalbe access, and enable gamma
        }
        //#if defined( _Second_Gamma) // marked by Jerry Wu 20071106
        else if (Mode == 2)
        {
//			RTD_Log(LOGGER_INFO,"write gamma 2\n");
//			gamma_ctrl_1_REG.color_channel = 0;
//			rtd_outl(GAMMA_GAMMA_CTRL_1_ADDR, gamma_ctrl_1_REG.regValue);
            rtdf_writeScalerPort(_P0_GAMMA_CRTL , 0x80);//enable access
            drv_color_colorwritegamma(tGAMMA_2_R);
//			gamma_ctrl_1_REG.color_channel = 1;
//			rtd_outl(GAMMA_GAMMA_CTRL_1_ADDR, gamma_ctrl_1_REG.regValue);
            rtdf_writeScalerPort(_P0_GAMMA_CRTL , 0x90);
            drv_color_colorwritegamma(tGAMMA_2_G);
//			gamma_ctrl_1_REG.color_channel = 2;
//			rtd_outl(GAMMA_GAMMA_CTRL_1_ADDR, gamma_ctrl_1_REG.regValue);
            rtdf_writeScalerPort(_P0_GAMMA_CRTL , 0xa0);
            drv_color_colorwritegamma(tGAMMA_2_B);

            rtdf_writeScalerPort(_P0_GAMMA_CRTL, 0x40);//disalbe access, and enable gamma
        }
        else
            rtdf_writeScalerPort(_P0_GAMMA_CRTL, 0x00);//disalbe access, and disable gamma

    }
    else
        rtdf_writeScalerPort(_P0_GAMMA_CRTL, 0x00);//disalbe access, and disable gamma


#if 0
    if (Mode != 0)  // not bypass
    {

        if (display  == _MAIN_DISPLAY )
        {
            gamma_ctrl_1_REG.enable_access = 0; // disable writing table
            gamma_ctrl_2_REG.main_tab_sel = 1; // use tab-1
            gamma_ctrl_2_REG.overlay = 0; // use main as overlay
        }

    }
    else   //  bypass
    {
        if (display  == _MAIN_DISPLAY )
        {
            gamma_ctrl_2_REG.main_tab_sel = 0; // use tab-1
            gamma_ctrl_2_REG.overlay = 0; // use main as overlay

        }


    }


    rtd_outl(GAMMA_GAMMA_CTRL_1_ADDR, gamma_ctrl_1_REG.regValue);//20071217 jason for high level (white changes to megenta)
    rtd_outl(GAMMA_GAMMA_CTRL_2_ADDR, gamma_ctrl_2_REG.regValue);
#endif

}
#endif

/*============================================================================*/
/**
 * ColorSpaceTransfer
 * Set Color space rgb2yuv conversion matrix
 * for source CCIR 601 YCbCr: Bypass RGB2YUV
 * for source CCIR 601 RGB: Use Tab-1 as ccir601 RGB2YUV
 * for source CCIR 709 YPbPr: overwrite Tab-2 as ccir709 YPbPr to 601 YCbCr
 * for source CCIR 709 RGB: overwrite Tab-2 as ccir709 RGB to 601 YCbCr
 */
void drv_color_colorspacergb2yuvtransfer(UINT8 nSrcType, UINT8 nSD_HD, UINT8 not_HDMIMODE_NEW)
{
    UINT16 *table_index=0;
//	vip_16bit_table_index =0;
	#ifndef CONFIG_ENABLE_HDMI
    not_HDMIMODE_NEW = not_HDMIMODE_NEW;
    #endif

    vip_8bit_tmp[0]=0;//UINT8 video_format;
    vip_8bit_tmp[1]=0;//UINT8 which_color_space = 0;
    vip_8bit_tmp[2]=0;//UINT8 temp_value = 0;


//	ich1_RGB2YUV_CTRL_RBUS  ich1_RGB2YUV_CTRL_REG;
//	ich2_RGB2YUV_CTRL_RBUS  ich2_RGB2YUV_CTRL_REG;

//	tab1_M1_RBUS tab1_M1_REG;
//	tab1_M2_RBUS tab1_M2_REG;
//	tab1_M3_RBUS tab1_M3_REG;
//	tab1_Yo_and_Y_Gain_RBUS tab1_Yo_and_Y_Gain_REG;

//	tab2_M1_RBUS tab2_M1_REG;
//	tab2_M2_RBUS tab2_M2_REG;
//	tab2_M3_RBUS tab2_M3_REG;
//	tab2_Yo_and_Y_Gain_RBUS tab2_Yo_and_Y_Gain_REG;


    //RTD_Log(LOGGER_INFO,"\nEnter ColorSpaceRGB2YUVTransfer  src_from = %d\n", src_from);

//	if(channel == _CHANNEL1)  { //main always access tab-1

    //ich1_RGB2YUV_CTRL_REG.regValue = rtd_inl(RGB2YUV_ICH1_RGB2YUV_CTRL_ADDR);
    rtdf_clearBits(ICH2_RGB2YUV_CTRL_0x0e04, _BIT1);//ich1_RGB2YUV_CTRL_REG.sel_table = 0;
    rtdf_clearBits(ICH2_RGB2YUV_CTRL_0x0e04, _BIT0);//ich1_RGB2YUV_CTRL_REG.en_rgb2yuv= 0;

    //rtd_outl(RGB2YUV_ICH1_RGB2YUV_CTRL_ADDR, ich1_RGB2YUV_CTRL_REG.regValue);

    //which_tab = 1;
//	}
    /*
    	else { //sub always access tab-2

    		ich2_RGB2YUV_CTRL_REG.regValue = rtd_inl(RGB2YUV_ICH2_RGB2YUV_CTRL_ADDR);

    		ich2_RGB2YUV_CTRL_REG.sel_table = 1;
    		ich2_RGB2YUV_CTRL_REG.en_rgb2yuv= 0;

    		rtd_outl(RGB2YUV_ICH2_RGB2YUV_CTRL_ADDR, ich2_RGB2YUV_CTRL_REG.regValue);

    		//which_tab = 2;
    	}
    */


    if (nSrcType == _SOURCE_VGA)
    {
        table_index = tRGB2YUV_COEF_601_RGB_0_255;

//				RTD_Log(LOGGER_INFO,"\nEnter ColorSpaceRGB2YUVTransfer  src_from = VGA\n", src_from);
    }
    else if (nSrcType == _SOURCE_YPBPR)
    {
        if (!nSD_HD)
            table_index = tRGB2YUV_COEF_601_YCbCr_0_255;
        else
            table_index = tRGB2YUV_COEF_709_YUV_0_255;

    }
    else if (nSrcType == _SOURCE_TV)
    {
        //table_index = tRGB2YUV_COEF_601_YCbCr_16_235;
        table_index = tRGB2YUV_COEF_601_YCbCr_0_255;

//	}  else if((nSrcType == _SOURCE_CVBS) ||(nSrcType == _SOURCE_SV)||(nSrcType == _SOURCE_SCART_AV)
//	||(nSrcType == _SOURCE_SCART_SV)){
    }
#ifdef CONFIG_SCART_AUTO_SWITCH
    else if (  (nSrcType == _SOURCE_FULL_SCART) && (SCART_Type==_SCART_RGB)  )
    {
        table_index = tRGB2YUV_COEF_601_YCbCr_0_255;//tRGB2YUV_COEF_601_RGB_0_255;
        RTD_Log(LOGGER_INFO,"SCART_Type = _SCART_RGB\n");
    }
#endif
    else if ((nSrcType == _SOURCE_CVBS) ||(nSrcType == _SOURCE_SV)
#ifdef CONFIG_SCART_AUTO_SWITCH
             ||(nSrcType == _SOURCE_FULL_SCART)	||(nSrcType == _SOURCE_HALF_SCART)
#endif
            )
    {
        //table_index = tRGB2YUV_COEF_601_YCbCr_16_235;
        table_index = tRGB2YUV_COEF_601_YCbCr_0_255;
        RTD_Log(LOGGER_INFO,"SCART_Type = _HALF_SCART_RGB\n");
    }
    #ifdef CONFIG_ENABLE_HDMI
    else if (nSrcType == _SOURCE_HDMI)
    {
        //read HDMI color space and info
        rtdf_pageSelect(_PAGE2);
        rtdf_writeScalerPort(HDMI_HDMI_AP_VADDR, HDMI_ACRCR_ADDR);
        vip_8bit_tmp[0] = rtdf_readScalerPort(HDMI_HDMI_DP_VADDR);//video_format = rtdf_readByte0(HDMI_HDMI_ACRCR_RO_ADDR);

        rtdf_writeScalerPort(HDMI_HDMI_PSAP_VADDR, 0x05);//≥o√‰´‹©«
        vip_8bit_tmp[3] =  rtdf_readScalerPort(HDMI_HDMI_PSDP_VADDR);//temp_value =  rtdf_readByte0(HDMI_PSDP_ADDR);

        if (((vip_8bit_tmp[3] >> 6)&0x03 )== 0x01) //if(((temp_value >> 6)&0x03 )== 0x01) { // CCIR 601
        {
            vip_8bit_tmp[1] = 1;//which_color_space = 1;
        }
        else if (((vip_8bit_tmp[3] >> 6)&0x03) == 0x02)  //} else if(((temp_value >> 6)&0x03) == 0x02) { // CCIR 709
        {
            vip_8bit_tmp[1] = 2;//which_color_space = 2;
        }
        //RTD_Log(LOGGER_INFO,"RGB2YUV_SRC_HDMI_YUV which_color_space = %d\n", which_color_space);

        if ((rtdf_readScalerPort(HDMI_HDMI_SR_VADDR) & _BIT0) && not_HDMIMODE_NEW )//if((rtd_inl(HDMI_SR) & _BIT0) && (stModeInfo.ModeCurr !=_HDMIMODE_NEW) )//if(drvif_Hdmi_FormatDetect()&&(stModeInfo.ModeCurr !=_HDMIMODE_NEW)) // HDMI Video Mode,  gary 20071227
        {
            if ((vip_8bit_tmp[0]&(_BIT4|_BIT5)) )  //YUV444 or YUV422
            {

                RTD_Log(LOGGER_INFO,"rgb2yuv HDMI Video: YUV\n");

                if (vip_8bit_tmp[1] == 1)//if(which_color_space == 1)
                    table_index = tRGB2YUV_COEF_601_YCbCr_16_235;
                else if (vip_8bit_tmp[1] == 2)//else if(which_color_space == 2)
                    table_index = tRGB2YUV_COEF_709_YUV_16_235;
                else
                {
                    // modified by Jerry Wu 20080516 for solve hdmi color space input detect error
                    if (!nSD_HD)
                        table_index = tRGB2YUV_COEF_601_YCbCr_16_235;
                    else
                        table_index = tRGB2YUV_COEF_709_YUV_16_235;
                }

                //RTD_Log(LOGGER_INFO,"RGB2YUV_SRC_HDMI_YUV\n");
            }
            else  //RGB
            {

                RTD_Log(LOGGER_INFO,"rgb2yuv HDMI Video: RGB\n");

                if (vip_8bit_tmp[1] == 1)//if(which_color_space == 1)
                    table_index =tRGB2YUV_COEF_601_RGB_0_255; //tRGB2YUV_COEF_601_RGB_16_235; modify by Jerry 20080317
                else if (vip_8bit_tmp[1] == 2)//else if(which_color_space == 2)
                    table_index =tRGB2YUV_COEF_709_RGB_0_255; //tRGB2YUV_COEF_709_RGB_16_235; modify by Jerry 20080317
                else
                    //table_index = tRGB2YUV_COEF_709_RGB_0_255;  //tRGB2YUV_COEF_709_RGB_16_235; modify by Jerry 20080317

                { 	 // modified by Jerry Wu 20080516 for solve hdmi color space input detect error

                    if (!nSD_HD)
                        table_index = tRGB2YUV_COEF_601_RGB_0_255;
                    else
                        table_index = tRGB2YUV_COEF_709_RGB_0_255;

                }

                //ROSPrintf("RGB2YUV_SRC_HDMI_RGB\n");
            }

        }
        else // DVI mode
        {
            if ((vip_8bit_tmp[0]&(_BIT4|_BIT5)) )  //YUV444 or YUV422
            {
                RTD_Log(LOGGER_INFO,"rgb2yuv HDMI DVI: YUV\n");

                if (vip_8bit_tmp[1] == 1)//if(which_color_space == 1)//20071107 gary for HDMI gray saturation
                    table_index =tRGB2YUV_COEF_601_YCbCr_0_255;
                else if (vip_8bit_tmp[1] == 2)//else if(which_color_space == 2)
                    table_index = tRGB2YUV_COEF_709_YUV_0_255;
                else
                    table_index = tRGB2YUV_COEF_709_YUV_0_255;

                //RTD_Log(LOGGER_INFO,"RGB2YUV_SRC_HDMI_YUV\n");
            }
            else  //RGB
            {
                RTD_Log(LOGGER_INFO,"rgb2yuv HDMI DVI: RGB\n");

                if (vip_8bit_tmp[1] == 1)//if(which_color_space == 1)
                    table_index =tRGB2YUV_COEF_601_RGB_0_255;
                else if (vip_8bit_tmp[1] == 2)//else if(which_color_space == 2)
                    table_index = tRGB2YUV_COEF_709_RGB_0_255;
                else
                    table_index = tRGB2YUV_COEF_709_RGB_0_255;

                //RTD_Log(LOGGER_INFO,"RGB2YUV_SRC_HDMI_RGB\n");
            }

        }


        //RTD_Log(LOGGER_INFO,"RGB2YUV_SRC_FROM_HDMI\n");
    }
    #endif
    else
    {
        //which_tab = 0;
//		RTD_Log(LOGGER_INFO,"unhandler type%d\n", nSrcType);
        return;
    }

//	if(channel  == _CHANNEL1) {
    //main  all tab-1
//		tab1_M1_REG.m11 = table_index [_RGB2YUV_m11];
//		tab1_M1_REG.m12 = table_index [_RGB2YUV_m12];
//		tab1_M1_REG.m13 = table_index [_RGB2YUV_m13];
//		RTD_Log(LOGGER_INFO,"tab1_M1_REG.regValue = %x\n", tab1_M1_REG.regValue);
    vip_32bit_tmp[0] = table_index [_RGB2YUV_m13];
    vip_32bit_tmp[0] = vip_32bit_tmp[0]<<10;
    vip_32bit_tmp[0] += table_index [_RGB2YUV_m12];
    vip_32bit_tmp[0] = vip_32bit_tmp[0]<<11;
    vip_32bit_tmp[0] += table_index [_RGB2YUV_m11];
    //RTD_Log(LOGGER_INFO,"_m1x=%x\n",(UINT32)vip_32bit_tmp[0]);
    rtd_outl(TAB1_M1_0x0e08, vip_32bit_tmp[0]);
    //rtd_outl(TAB1_M1_0x0e08, (table_index [_RGB2YUV_m13]<<21)+(table_index [_RGB2YUV_m12]<<11)+table_index [_RGB2YUV_m11] );//rtd_outl(RGB2YUV_TAB1_M1_ADDR, tab1_M1_REG.regValue);

//		tab1_M2_REG.m21 = table_index [_RGB2YUV_m21];
//		tab1_M2_REG.m22 = table_index [_RGB2YUV_m22];
//		tab1_M2_REG.m23 = table_index [_RGB2YUV_m23];
//		//RTD_Log(LOGGER_INFO,"tab1_M2_REG.regValue = %x\n", tab1_M2_REG.regValue);
    vip_32bit_tmp[0] = table_index [_RGB2YUV_m23];
    vip_32bit_tmp[0] = vip_32bit_tmp[0]<<10;
    vip_32bit_tmp[0] += table_index [_RGB2YUV_m22];
    vip_32bit_tmp[0] = vip_32bit_tmp[0]<<9;
    vip_32bit_tmp[0] += table_index [_RGB2YUV_m21];
//		RTD_Log(LOGGER_INFO,"_m2x=%x\n",(UINT32)vip_32bit_tmp[0]);
    rtd_outl(TAB1_M2_0x0e0c, vip_32bit_tmp[0]);
    //rtd_outl(TAB1_M2_0x0e0c, (table_index [_RGB2YUV_m23]<<19)+(table_index [_RGB2YUV_m22]<<9)+table_index [_RGB2YUV_m21] );//rtd_outl(RGB2YUV_TAB1_M2_ADDR, tab1_M2_REG.regValue);

//		tab1_M3_REG.m31 = table_index [_RGB2YUV_m31];
//		tab1_M3_REG.m32 = table_index [_RGB2YUV_m32];
//		tab1_M3_REG.m33 = table_index [_RGB2YUV_m33];
//		//RTD_Log(LOGGER_INFO,"tab1_M3_REG.regValue = %x\n", tab1_M3_REG.regValue);
    vip_32bit_tmp[0] = table_index [_RGB2YUV_m33];
    vip_32bit_tmp[0] = vip_32bit_tmp[0]<<9;
    vip_32bit_tmp[0] += table_index [_RGB2YUV_m32];
    vip_32bit_tmp[0] = vip_32bit_tmp[0]<<9;
    vip_32bit_tmp[0] += table_index [_RGB2YUV_m31];
//		RTD_Log(LOGGER_INFO,"_m3x=%x\n",(UINT32)vip_32bit_tmp[0]);
    rtd_outl(TAB1_M3_0x0e10, vip_32bit_tmp[0]);
    //rtd_outl(TAB1_M3_0x0e10, (table_index [_RGB2YUV_m33]<<18)+(table_index [_RGB2YUV_m32]<<9)+table_index [_RGB2YUV_m31] );//rtd_outl(RGB2YUV_TAB1_M3_ADDR, tab1_M3_REG.regValue);

//		tab1_Yo_and_Y_Gain_REG.yo_even= table_index [_RGB2YUV_Yo_even];
//		tab1_Yo_and_Y_Gain_REG.yo_odd= table_index [_RGB2YUV_Yo_odd];
//		tab1_Yo_and_Y_Gain_REG.y_gain= table_index [_RGB2YUV_Y_gain];
//		tab1_Yo_and_Y_Gain_REG.y_gain= table_index [_RGB2YUV_Y_gain];

//RGB2YUV_Y_Gain= vip_16bit_table_index [_RGB2YUV_Y_gain];//jason9.ccc: for what ?
    vip_32bit_tmp[0] = table_index [_RGB2YUV_Y_gain];
    vip_32bit_tmp[0] = vip_32bit_tmp[0]<<10;
    vip_32bit_tmp[0] += table_index [_RGB2YUV_Yo_odd];
    vip_32bit_tmp[0] = vip_32bit_tmp[0]<<10;
    vip_32bit_tmp[0] += table_index [_RGB2YUV_Yo_even];
    rtd_outl(TAB1_YO_AND_Y_GAIN_0x0e14, vip_32bit_tmp[0]);
    //rtd_outl(TAB1_YO_AND_Y_GAIN_0x0e14, (table_index [_RGB2YUV_Y_gain]<<20)+(table_index [_RGB2YUV_Yo_odd]<<10)+table_index [_RGB2YUV_Yo_even] );//rtd_outl(RGB2YUV_TAB1_YO_AND_Y_GAIN_ADDR, tab1_Yo_and_Y_Gain_REG.regValue);

//		ich1_RGB2YUV_CTRL_REG.sel_rgb= table_index [_RGB2YUV_sel_RGB];
//		ich1_RGB2YUV_CTRL_REG.set_r_in_offset= table_index [_RGB2YUV_set_Yin_offset];
//		ich1_RGB2YUV_CTRL_REG.set_uv_out_offset= table_index [_RGB2YUV_set_UV_out_offset];
//		ich1_RGB2YUV_CTRL_REG.sel_uv_off= table_index [_RGB2YUV_sel_UV_off];
//		ich1_RGB2YUV_CTRL_REG.matrix_bypass= table_index [_RGB2YUV_Matrix_bypass];
//		ich1_RGB2YUV_CTRL_REG.sel_y_gain= table_index [_RGB2YUV_Enable_Y_gain];
    vip_32bit_tmp[0] = table_index [_RGB2YUV_Enable_Y_gain];
    vip_32bit_tmp[0] = (vip_32bit_tmp[0]<<1);
    vip_32bit_tmp[0] += table_index [_RGB2YUV_Matrix_bypass];
    vip_32bit_tmp[0] = (vip_32bit_tmp[0]<<1);
    vip_32bit_tmp[0] += table_index [_RGB2YUV_sel_UV_off];
    vip_32bit_tmp[0] = (vip_32bit_tmp[0]<<1);
    vip_32bit_tmp[0] += table_index [_RGB2YUV_set_UV_out_offset];
    vip_32bit_tmp[0] = (vip_32bit_tmp[0]<<1);
    vip_32bit_tmp[0] += table_index [_RGB2YUV_set_Yin_offset];
    vip_32bit_tmp[0] = (vip_32bit_tmp[0]<<1);
    vip_32bit_tmp[0] += table_index [_RGB2YUV_sel_RGB];
    vip_32bit_tmp[0] = (vip_32bit_tmp[0]<<1);
    vip_32bit_tmp[0] += 0x0;//sel_sign??
    vip_32bit_tmp[0] = (vip_32bit_tmp[0]<<1);
    vip_32bit_tmp[0] += 0x0;//sel_table
    vip_32bit_tmp[0] = (vip_32bit_tmp[0]<<1);
//		RTD_Log(LOGGER_INFO,"_selrgb=%x\n",(UINT32)table_index [_RGB2YUV_sel_RGB]);
//		RTD_Log(LOGGER_INFO,"_ctrl=%x\n",(UINT32)vip_32bit_tmp[0]);
    rtd_outl(ICH2_RGB2YUV_CTRL_0x0e04, vip_32bit_tmp[0]);
#if 0
    vip_32bit_tmp[0] =( ((table_index [_RGB2YUV_Enable_Y_gain]<<8)+(table_index [_RGB2YUV_Matrix_bypass]<<7)
                         +(table_index [_RGB2YUV_sel_UV_off]<<6)+(table_index [_RGB2YUV_set_UV_out_offset]<<5)
                         +(table_index [_RGB2YUV_set_Yin_offset]<<4)+(table_index [_RGB2YUV_sel_RGB]<<3)+(0x0<<2) ) & 0xfffffffc );
#endif

//		RTD_Log(LOGGER_INFO,"\n_RGB2YUV Src = %x\n", (UINT32)nSrcType);
//		RTD_Log(LOGGER_INFO,"\nm1x %x, %x, %x\nm2x %x, %x, %x\nm3x %x, %x, %x\n", (UINT32)table_index[_RGB2YUV_m11], (UINT32)table_index[_RGB2YUV_m12], (UINT32)table_index[_RGB2YUV_m13], (UINT32)table_index[_RGB2YUV_m21], (UINT32)table_index[_RGB2YUV_m22], (UINT32)table_index[_RGB2YUV_m23], (UINT32)table_index[_RGB2YUV_m31], (UINT32)table_index[_RGB2YUV_m32], (UINT32)table_index[_RGB2YUV_m33]);

//		RtdCtrl_Mask(ICH2_RGB2YUV_CTRL_0x0e04, ~(0xfffffffc), vip_32bit_tmp[0]);

//		rtdf_clearBits(ICH2_RGB2YUV_CTRL_0x0e04, _BIT1);//ich1_RGB2YUV_CTRL_REG.sel_table= 0;
    rtdf_setBits(ICH2_RGB2YUV_CTRL_0x0e04, _BIT0);//ich1_RGB2YUV_CTRL_REG.en_rgb2yuv = 1;

    //rtd_outl(RGB2YUV_ICH1_RGB2YUV_CTRL_ADDR, ich1_RGB2YUV_CTRL_REG.regValue);

    //RTD_Log(LOGGER_INFO,"_MAIN_DISPLAY %d\n", which_tab);

//	}
    /*
    	#ifdef CONFIG_DUAL_CHANNEL
    	else {

    		 //sub  all tab-2
    		tab2_M1_REG.m11 = table_index [_RGB2YUV_m11];
    		tab2_M1_REG.m12 = table_index [_RGB2YUV_m12];
    		tab2_M1_REG.m13 = table_index [_RGB2YUV_m13];
    		rtd_outl(RGB2YUV_TAB2_M1_ADDR, tab2_M1_REG.regValue);

    		tab2_M2_REG.m21 = table_index [_RGB2YUV_m21];
    		tab2_M2_REG.m22 = table_index [_RGB2YUV_m22];
    		tab2_M2_REG.m23 = table_index [_RGB2YUV_m23];
    		rtd_outl(RGB2YUV_TAB2_M2_ADDR, tab2_M2_REG.regValue);

    		tab2_M3_REG.m31 = table_index [_RGB2YUV_m31];
    		tab2_M3_REG.m32 = table_index [_RGB2YUV_m32];
    		tab2_M3_REG.m33 = table_index [_RGB2YUV_m33];
    		rtd_outl(RGB2YUV_TAB2_M3_ADDR, tab2_M3_REG.regValue);

    		tab2_Yo_and_Y_Gain_REG.yo_even= table_index [_RGB2YUV_Yo_even];
    		tab2_Yo_and_Y_Gain_REG.yo_odd= table_index [_RGB2YUV_Yo_odd];
    		tab2_Yo_and_Y_Gain_REG.y_gain= table_index [_RGB2YUV_Y_gain];
    		tab2_Yo_and_Y_Gain_REG.y_gain= table_index [_RGB2YUV_Y_gain];
    		rtd_outl(RGB2YUV_TAB2_YO_AND_Y_GAIN_ADDR, tab2_Yo_and_Y_Gain_REG.regValue);

    		ich2_RGB2YUV_CTRL_REG.sel_rgb= table_index [_RGB2YUV_sel_RGB];
    		ich2_RGB2YUV_CTRL_REG.set_r_in_offset= table_index [_RGB2YUV_set_Yin_offset];
    		ich2_RGB2YUV_CTRL_REG.set_uv_out_offset= table_index [_RGB2YUV_set_UV_out_offset];
    		ich2_RGB2YUV_CTRL_REG.sel_uv_off= table_index [_RGB2YUV_sel_UV_off];
    		ich2_RGB2YUV_CTRL_REG.matrix_bypass= table_index [_RGB2YUV_Matrix_bypass];
    		ich2_RGB2YUV_CTRL_REG.sel_y_gain= table_index [_RGB2YUV_Enable_Y_gain];

    		ich2_RGB2YUV_CTRL_REG.sel_table= 1;
    		ich2_RGB2YUV_CTRL_REG.en_rgb2yuv = 1;

    		rtd_outl(RGB2YUV_ICH2_RGB2YUV_CTRL_ADDR, ich2_RGB2YUV_CTRL_REG.regValue);

    		//RTD_Log(LOGGER_INFO,"_SUB_DISPLAY %d\n", which_tab);
    	}
    	#endif
    */
}

/*============================================================================*/
/**
 * ColorSpaceTransfer
 * Set Color space yuv2rgb conversion matrix
 * for source CCIR 601 YCbCr: Use Tab-1 as ccir601 YPbPr to 601 RGB
 * for source CCIR 709 YPbPr: Use Tab-2 as ccir709 YPbPr to 601 RGB

 */


void drv_color_colorspaceyuv2rgbtransfer(UINT8 nSrcType)
{
//	StructSourceInput		*srcinput_pt = NULL;
//	UINT8 src_from = 0;
//	UINT8 vdc_src = 0;
//	UINT8 which_tab = 0;

    UINT16 *table_index = 0;
    /*
    	dm_yuv2rgb_control_RBUS dm_yuv2rgb_control_REG;
    	ds_yuv2rgb_control_RBUS ds_yuv2rgb_control_REG;
    	ds_yuv2rgb_dcc_th_RBUS ds_yuv2rgb_dcc_th_REG;
    	ds_yuv2rgb_dcc_gain_RBUS ds_yuv2rgb_dcc_gain_REG;
    	ds_yuv2rgb_dcc_offset_RBUS ds_yuv2rgb_dcc_offset_REG;

    	d_yuv2rgb_tab1_data_clamp_RBUS d_yuv2rgb_tab1_data_clamp_REG;
    	d_yuv2rgb_tab1_data_0_RBUS d_yuv2rgb_tab1_data_0_REG;
    	d_yuv2rgb_tab1_data_1_RBUS d_yuv2rgb_tab1_data_1_REG;
    	d_yuv2rgb_tab1_data_2_RBUS d_yuv2rgb_tab1_data_2_REG;
    	d_yuv2rgb_tab1_data_3_RBUS d_yuv2rgb_tab1_data_3_REG;

    	d_yuv2rgb_tab1_data_clamp_RBUS d_yuv2rgb_tab2_data_clamp_REG;
    	d_yuv2rgb_tab1_data_0_RBUS d_yuv2rgb_tab2_data_0_REG;
    	d_yuv2rgb_tab1_data_1_RBUS d_yuv2rgb_tab2_data_1_REG;
    	d_yuv2rgb_tab1_data_2_RBUS d_yuv2rgb_tab2_data_2_REG;
    	d_yuv2rgb_tab1_data_3_RBUS d_yuv2rgb_tab2_data_3_REG;
    */
    //if(display  == _MAIN_DISPLAY) { //main always access tab-1

//		dm_yuv2rgb_control_REG.regValue = rtd_inl(YUV2RGB_DM_YUV2RGB_CONTROL_ADDR);
//		dm_yuv2rgb_control_REG.main_enable = 0;
//		dm_yuv2rgb_control_REG.main_select = 0;
    rtdf_clearBits(DM_YUV2RGB_CONTROL_0x1f00, _BIT0);//rtd_outl(YUV2RGB_DM_YUV2RGB_CONTROL_ADDR, dm_yuv2rgb_control_REG.regValue);
    rtdf_clearBits(DM_YUV2RGB_CONTROL_0x1f00, _BIT1);

    //	which_tab = 1;
    //}
    /*
    	#ifdef CONFIG_DUAL_CHANNEL
    	else { //sub always access tab-2

    		ds_yuv2rgb_control_REG.regValue = rtd_inl(YUV2RGB_DS_YUV2RGB_CONTROL_ADDR);
    		ds_yuv2rgb_control_REG.sub_enabled= 0;
    		ds_yuv2rgb_control_REG.sub_select= 1;
    		ds_yuv2rgb_control_REG.sub_dcc_enabled = 0;

    		rtd_outl(YUV2RGB_DS_YUV2RGB_CONTROL_ADDR, ds_yuv2rgb_control_REG.regValue);

    	//	which_tab = 2;
    	}
    	#endif
    	RTD_Log(LOGGER_INFO,"Enter ColorSpaceYUV2RGBTransfer \n");
    */

    if (nSrcType == _SOURCE_VGA)
    {

        table_index = tYUV2RGB_COEF_601_YCbCr;

    }
    else if (nSrcType == _SOURCE_YPBPR)
    {

        table_index = tYUV2RGB_COEF_601_YCbCr;

    }
    else if (nSrcType == _SOURCE_TV)
    {

        table_index = tYUV2RGB_COEF_601_YCbCr;

//	} else if((nSrcType == _SOURCE_CVBS) || (nSrcType == _SOURCE_SV)){// || (nSrcType == _SRC_COMPONENT)) {
//	} else if((nSrcType == _SOURCE_CVBS)||(nSrcType == _SOURCE_SV)||(nSrcType == _SOURCE_SCART_AV)
//	||(nSrcType == _SOURCE_SCART_SV)){
    }
    else if ((nSrcType == _SOURCE_CVBS)||(nSrcType == _SOURCE_SV)
#ifdef CONFIG_SCART_AUTO_SWITCH
             ||(nSrcType == _SOURCE_FULL_SCART)	||(nSrcType == _SOURCE_HALF_SCART)
#endif
            )
    {
        table_index = tYUV2RGB_COEF_601_YCbCr;

    }
#ifdef CONFIG_ENABLE_HDMI
    else if (nSrcType == _SOURCE_HDMI)
    {

        table_index = tYUV2RGB_COEF_601_YCbCr;

    }
#endif
    else
    {
        table_index = tYUV2RGB_COEF_601_YCbCr;
        return;
    }

    //if(display  == _MAIN_DISPLAY ) {
    // main always tab-1
    /*
    		RTD_Log(LOGGER_INFO,"\n_YUV2RGB nSrcType = %x\n", (UINT32)nSrcType);
    		RTD_Log(LOGGER_INFO,"table_index[_YUV2RGB_Y_Clamp] = %x\n", (UINT32)table_index[_YUV2RGB_Y_Clamp]);
    	 	RTD_Log(LOGGER_INFO,"table_index[_YUV2RGB_UV_Clamp] = %x\n", (UINT32)table_index[_YUV2RGB_UV_Clamp]);
    		RTD_Log(LOGGER_INFO,"table_index[_YUV2RGB_k11] = %x\n", (UINT32)table_index[_YUV2RGB_k11]);
    		RTD_Log(LOGGER_INFO,"table_index[_YUV2RGB_k13] = %x\n", (UINT32)table_index[_YUV2RGB_k13]);
    		RTD_Log(LOGGER_INFO,"table_index[_YUV2RGB_k22] = %x\n", (UINT32)table_index[_YUV2RGB_k22]);
    		RTD_Log(LOGGER_INFO,"table_index[_YUV2RGB_k23] = %x\n", (UINT32)table_index[_YUV2RGB_k23]);
    		RTD_Log(LOGGER_INFO,"table_index[_YUV2RGB_k32] = %x\n", (UINT32)table_index[_YUV2RGB_k32]);
    		RTD_Log(LOGGER_INFO,"table_index[_YUV2RGB_Roffset] = %x\n", (UINT32)table_index[_YUV2RGB_Roffset]);
    		RTD_Log(LOGGER_INFO,"table_index[_YUV2RGB_Goffset] = %x\n", (UINT32)table_index[_YUV2RGB_Goffset]);
    		RTD_Log(LOGGER_INFO,"table_index[_YUV2RGB_Boffset] = %x\n", (UINT32)table_index[_YUV2RGB_Boffset]);
    */
//		d_yuv2rgb_tab1_data_clamp_REG.y_clamp = table_index[_YUV2RGB_Y_Clamp];
//		d_yuv2rgb_tab1_data_clamp_REG.cb_cr_clamp = table_index[_YUV2RGB_UV_Clamp];
    vip_32bit_tmp[0] = table_index[_YUV2RGB_UV_Clamp];
    vip_32bit_tmp[0] = (vip_32bit_tmp[0]<<1);
    vip_32bit_tmp[0] += table_index[_YUV2RGB_Y_Clamp];
    rtd_outl(D_YUV2RGB_TAB1_DATA_CLAMP_0x1f14, vip_32bit_tmp[0]);//rtd_outl(YUV2RGB_D_YUV2RGB_TAB1_DATA_CLAMP_ADDR, d_yuv2rgb_tab1_data_clamp_REG.regValue);
    //rtd_outl(D_YUV2RGB_TAB1_DATA_CLAMP_0x1f14, (table_index[_YUV2RGB_UV_Clamp]<<1)+table_index[_YUV2RGB_Y_Clamp]);//rtd_outl(YUV2RGB_D_YUV2RGB_TAB1_DATA_CLAMP_ADDR, d_yuv2rgb_tab1_data_clamp_REG.regValue);


//		d_yuv2rgb_tab1_data_0_REG.k11 = table_index[_YUV2RGB_k11];
//		d_yuv2rgb_tab1_data_0_REG.k13 = table_index[_YUV2RGB_k13];
    //RTD_Log(LOGGER_INFO,"k11=%x, k13=%x, d_yuv2rgb_tab1_data_0_REG.regValue %x\n", d_yuv2rgb_tab1_data_0_REG.k11, d_yuv2rgb_tab1_data_0_REG.k13, d_yuv2rgb_tab1_data_0_REG.regValue);
    vip_32bit_tmp[0] = table_index[_YUV2RGB_k11];
    vip_32bit_tmp[0] = (vip_32bit_tmp[0]<<9);
    vip_32bit_tmp[0] += table_index[_YUV2RGB_k13];
    rtd_outl(D_YUV2RGB_TAB1_DATA_0_0x1f18, vip_32bit_tmp[0]);//rtd_outl(YUV2RGB_D_YUV2RGB_TAB1_DATA_0_ADDR, d_yuv2rgb_tab1_data_0_REG.regValue);
//		rtd_outl(D_YUV2RGB_TAB1_DATA_0_0x1f18, (table_index[_YUV2RGB_k11]<<9)+table_index[_YUV2RGB_k13]);//rtd_outl(YUV2RGB_D_YUV2RGB_TAB1_DATA_0_ADDR, d_yuv2rgb_tab1_data_0_REG.regValue);
    //RTD_Log(LOGGER_INFO,"d_yuv2rgb_tab1_data_0_REG.regValue %x\n", d_yuv2rgb_tab1_data_0_REG.regValue);

//		d_yuv2rgb_tab1_data_1_REG.k22 = vip_16bit_table_index[_YUV2RGB_k22];
//		d_yuv2rgb_tab1_data_1_REG.k23 = vip_16bit_table_index[_YUV2RGB_k23];
//		d_yuv2rgb_tab1_data_1_REG.k32 = vip_16bit_table_index[_YUV2RGB_k32];
    vip_32bit_tmp[0] = table_index[_YUV2RGB_k32];
    vip_32bit_tmp[0] = (vip_32bit_tmp[0]<<8);
    vip_32bit_tmp[0] += table_index[_YUV2RGB_k23];
    vip_32bit_tmp[0] = vip_32bit_tmp[0]<<8;
    vip_32bit_tmp[0] += table_index[_YUV2RGB_k22];
#if 0
    vip_32bit_tmp[0] = (vip_32bit_tmp[0]<<16);
    vip_32bit_tmp[1] = table_index[_YUV2RGB_k23];
    vip_32bit_tmp[1] = vip_32bit_tmp[1]<<8;
    vip_32bit_tmp[0] = vip_32bit_tmp[0] + vip_32bit_tmp[1] + table_index[_YUV2RGB_k22];
#endif
    rtd_outl(D_YUV2RGB_TAB1_DATA_1_0x1f1c, vip_32bit_tmp[0]);//rtd_outl(YUV2RGB_D_YUV2RGB_TAB1_DATA_1_ADDR, d_yuv2rgb_tab1_data_1_REG.regValue);
//		rtd_outl(D_YUV2RGB_TAB1_DATA_1_0x1f1c, (table_index[_YUV2RGB_k32]<<16)+(table_index[_YUV2RGB_k23]<<8)+table_index[_YUV2RGB_k22]);//rtd_outl(YUV2RGB_D_YUV2RGB_TAB1_DATA_1_ADDR, d_yuv2rgb_tab1_data_1_REG.regValue);
//		d_yuv2rgb_tab1_data_2_REG.roffset= vip_16bit_table_index[_YUV2RGB_Roffset];
//		d_yuv2rgb_tab1_data_2_REG.goffset= vip_16bit_table_index[_YUV2RGB_Goffset];
    vip_32bit_tmp[0] = table_index[_YUV2RGB_Goffset];
    vip_32bit_tmp[0] = (vip_32bit_tmp[0]<<13);
    vip_32bit_tmp[0] += table_index[_YUV2RGB_Roffset];
    rtd_outl(D_YUV2RGB_TAB1_DATA_2_0x1f20, vip_32bit_tmp[0]);//rtd_outl(YUV2RGB_D_YUV2RGB_TAB1_DATA_2_ADDR, d_yuv2rgb_tab1_data_2_REG.regValue);
//		rtd_outl(D_YUV2RGB_TAB1_DATA_2_0x1f20, (table_index[_YUV2RGB_Goffset]<<13)+table_index[_YUV2RGB_Roffset]);//rtd_outl(YUV2RGB_D_YUV2RGB_TAB1_DATA_2_ADDR, d_yuv2rgb_tab1_data_2_REG.regValue);

//		d_yuv2rgb_tab1_data_3_REG.boffset= vip_16bit_table_index[_YUV2RGB_Boffset];
    vip_32bit_tmp[0] = table_index[_YUV2RGB_Boffset];
    rtd_outl(D_YUV2RGB_TAB1_DATA_3_0x1f24, vip_32bit_tmp[0]);//rtd_outl(YUV2RGB_D_YUV2RGB_TAB1_DATA_3_ADDR, d_yuv2rgb_tab1_data_3_REG.regValue);
//		rtd_outl(D_YUV2RGB_TAB1_DATA_3_0x1f24, table_index[_YUV2RGB_Boffset]);//rtd_outl(YUV2RGB_D_YUV2RGB_TAB1_DATA_3_ADDR, d_yuv2rgb_tab1_data_3_REG.regValue);

//		dm_yuv2rgb_control_REG.main_enable = 1;
//		dm_yuv2rgb_control_REG.main_select = 0;

    rtdf_clearBits(DM_YUV2RGB_CONTROL_0x1f00, _BIT1);
    rtdf_setBits(DM_YUV2RGB_CONTROL_0x1f00, _BIT0);//rtd_outl(YUV2RGB_DM_YUV2RGB_CONTROL_ADDR, dm_yuv2rgb_control_REG.regValue);


    //	RTD_Log(LOGGER_INFO,"d_yuv2rgb_tab1_data_0_REG.regValue %x\n", d_yuv2rgb_tab1_data_0_REG.regValue);
    //	RTD_Log(LOGGER_INFO,"d_yuv2rgb_tab1_data_0_REG.regValue %x\n", d_yuv2rgb_tab1_data_1_REG.regValue);
    //	RTD_Log(LOGGER_INFO,"d_yuv2rgb_tab1_data_0_REG.regValue %x\n", d_yuv2rgb_tab1_data_2_REG.regValue);
    //	RTD_Log(LOGGER_INFO,"d_yuv2rgb_tab1_data_0_REG.regValue %x\n", d_yuv2rgb_tab1_data_3_REG.regValue);

//		RTD_Log(LOGGER_INFO,"_MAIN_DISPLAY\n");
    //}

#if 0
#ifdef CONFIG_DUAL_CHANNEL
    else
    {
        // sub always tab-2
        d_yuv2rgb_tab2_data_clamp_REG.y_clamp = table_index[_YUV2RGB_Y_Clamp];
        d_yuv2rgb_tab2_data_clamp_REG.cb_cr_clamp = table_index[_YUV2RGB_UV_Clamp];
        rtd_outl(YUV2RGB_D_YUV2RGB_TAB2_DATA_CLAMP_ADDR, d_yuv2rgb_tab2_data_clamp_REG.regValue);

        d_yuv2rgb_tab2_data_0_REG.k11 = table_index[_YUV2RGB_k11];
        d_yuv2rgb_tab2_data_0_REG.k13 = table_index[_YUV2RGB_k13];
        rtd_outl(YUV2RGB_D_YUV2RGB_TAB2_DATA_0_ADDR, d_yuv2rgb_tab2_data_0_REG.regValue);

        d_yuv2rgb_tab2_data_1_REG.k22 = table_index[_YUV2RGB_k22];
        d_yuv2rgb_tab2_data_1_REG.k23 = table_index[_YUV2RGB_k23];
        d_yuv2rgb_tab2_data_1_REG.k32 = table_index[_YUV2RGB_k32];
        rtd_outl(YUV2RGB_D_YUV2RGB_TAB2_DATA_1_ADDR, d_yuv2rgb_tab2_data_1_REG.regValue);

        d_yuv2rgb_tab2_data_2_REG.roffset= table_index[_YUV2RGB_Roffset];
        d_yuv2rgb_tab2_data_2_REG.goffset= table_index[_YUV2RGB_Goffset];
        rtd_outl(YUV2RGB_D_YUV2RGB_TAB2_DATA_2_ADDR, d_yuv2rgb_tab2_data_2_REG.regValue);

        d_yuv2rgb_tab2_data_3_REG.boffset= table_index[_YUV2RGB_Boffset];
        rtd_outl(YUV2RGB_D_YUV2RGB_TAB2_DATA_3_ADDR, d_yuv2rgb_tab2_data_3_REG.regValue);

        ds_yuv2rgb_dcc_th_REG.regValue = 0;
        rtd_outl(YUV2RGB_DS_YUV2RGB_DCC_TH_ADDR, ds_yuv2rgb_dcc_th_REG.regValue);

        ds_yuv2rgb_dcc_gain_REG.regValue = 0;
        rtd_outl(YUV2RGB_DS_YUV2RGB_DCC_GAIN_ADDR, ds_yuv2rgb_dcc_gain_REG.regValue);
        ds_yuv2rgb_dcc_offset_REG.regValue = 0;
        rtd_outl(YUV2RGB_DS_YUV2RGB_DCC_OFFSET_ADDR, ds_yuv2rgb_dcc_offset_REG.regValue);

        ds_yuv2rgb_control_REG.sub_enabled = 1;
        ds_yuv2rgb_control_REG.sub_select = 1;
        ds_yuv2rgb_control_REG.sub_dcc_enabled = 0;
        rtd_outl(YUV2RGB_DS_YUV2RGB_CONTROL_ADDR, ds_yuv2rgb_control_REG.regValue);

//		RTD_Log(LOGGER_INFO,"_SUB_DISPLAY\n");
    }
#endif
#endif

}

/*============================================================================*/
/**
 * ColorSetYDLTI
 * Set DLTI of Y path
 *
 * @param <display> {main/sub display}
 * @param <HF_Enable> {HF enable control}
 * @param <HF_TH> {HF threshold value}
 * @param <Gain> {Gain}
 * @param <Gain_th> {Gain_th}
 * @param <AdvanceDelay> {AdvanceDelay control}
 * @param <Long> {Long control}
 * @param <Near> {Near control}
 * @param <Contour_enable> {contour control}
 * @param <Contour_th> {contour threshold value}
 * @return {none}
 *
 */
void drv_color_colorsetydlti(bool domain, bool HF_Enable, UINT8 HF_Th,  UINT8 Gain, UINT8 Gain_th, bool  AdvanceDelay, bool Long, bool Near, bool Contour_enable, UINT8 Contour_th)
{
    //options = (HF_Enable << 15 ) + (HF_Th << 8 ) + (AdvanceDelay << 7) + (Long << 6) + (Near << 5) + (Contour_enable << 4) + (Contour_th&0x0f);
    /*
    	//dm_dlti_options_RBUS dm_dlti_options_REG;
    	dm_dlti_options_REG.hf_th_enabled = HF_Enable;
    	dm_dlti_options_REG.hf_th_value = HF_Th;
    	dm_dlti_options_REG.advance_delay = AdvanceDelay;
    	dm_dlti_options_REG.prevent_long = Long;
    	dm_dlti_options_REG.prevent_near = Near;
    	dm_dlti_options_REG.prevent_contour = Contour_enable;
    	dm_dlti_options_REG.prevent_contour_th= Contour_th;
    */
    //vip_32bit_tmp[0] = (HF_Enable<<15) + (HF_Th<<8) + (AdvanceDelay<<7) + (Long<<6) + (Near<<5) + (Contour_enable<<4) + Contour_th;
    vip_32bit_tmp[0] = HF_Enable;
    vip_32bit_tmp[0] = vip_32bit_tmp[0]<<7;
    vip_32bit_tmp[0] += HF_Th;
    vip_32bit_tmp[0] = vip_32bit_tmp[0]<<1;
    vip_32bit_tmp[0] += AdvanceDelay;
    vip_32bit_tmp[0] = vip_32bit_tmp[0]<<1;
    vip_32bit_tmp[0] += Long;
    vip_32bit_tmp[0] = vip_32bit_tmp[0]<<1;
    vip_32bit_tmp[0] += Near;
    vip_32bit_tmp[0] = vip_32bit_tmp[0]<<1;
    vip_32bit_tmp[0] += Contour_enable;
    vip_32bit_tmp[0] = vip_32bit_tmp[0]<<4;
    vip_32bit_tmp[0] += Contour_th;

    if (domain == VIP_D_DOMAIN)
        rtd_outl( DM_DLTI_OPTIONS_0x1814, vip_32bit_tmp[0]);//rtd_outl(COLOR_DLCTI_DM_DLTI_OPTIONS_ADDR, dm_dlti_options_REG.regValue);
    else
        rtd_outl( DS_DLTI_OPTIONS_0x101c, vip_32bit_tmp[0]);
    /*
    	dm_dlti_value_RBUS dm_dlti_value_REG;
    	dm_dlti_value_REG.gain_value = Gain;
    	dm_dlti_value_REG.th_value = Gain_th;
    */
//	vip_32bit_tmp[0] = (Gain_th<<4) + (Gain);
    vip_32bit_tmp[0] = Gain_th;
    vip_32bit_tmp[0] = vip_32bit_tmp[0]<<4;
    vip_32bit_tmp[0] += Gain;

    if (domain == VIP_D_DOMAIN)
        rtd_outl( DM_DLTI_VALUE_0x1818, vip_32bit_tmp[0]);//rtd_outl(COLOR_DLCTI_DM_DLTI_VALUE_ADDR, dm_dlti_value_REG.regValue);
    else
        rtd_outl( DS_DLTI_VALUE_0x1020, vip_32bit_tmp[0]);

}

/*============================================================================*/
/**
 * ColorSetCDCTI
 * Set DCTI of C path
 *
 * @param <display> {main/sub display}
 * @param <Prevent_PE> {Phase Error Control}
 * @param <S_DCTI_Enable> {2nd DCTi Control}
 * @param <F_Gain> {First_Gain}
 * @param <F_th> {First_th}
 * @param <S_Gain> {Second_Gain}
 * @param <S_th> {Second_th}
 * @return {none}
 *
 */

void drv_color_colorsetcdcti(UINT8 domain, bool Prevent_PE,  UINT8 F_Gain, UINT8 F_th, bool S_DCTI_Enable, UINT8 S_Gain, UINT8 S_Th)
{
//	dm_dcti_1st_value_RBUS dm_dcti_1st_value_REG;
//	dm_dcti_1st_value_REG.th_value = F_th;
//	dm_dcti_1st_value_REG.gain_value = F_Gain;
//	rtd_outl(COLOR_DLCTI_DM_DCTI_1ST_VALUE_ADDR, dm_dcti_1st_value_REG.regValue);
//	vip_32bit_tmp[0] = (F_th<<5) + (F_Gain);
    vip_32bit_tmp[0] = F_th;
    vip_32bit_tmp[0] = vip_32bit_tmp[0]<<5;
    vip_32bit_tmp[0] += F_Gain;

    if (domain==VIP_D_DOMAIN)
        rtd_outl( DM_DCTI_1ST_VALUE_0x1828, vip_32bit_tmp[0]);
    else
        rtd_outl( DS_DCTI_1ST_VALUE_0x1034, vip_32bit_tmp[0]);


//	dm_dcti_2nd_value_RBUS dm_dcti_2nd_value_REG;
//	dm_dcti_2nd_value_REG.th_value = S_Th;
//	dm_dcti_2nd_value_REG.gain_value = S_Gain;
//	rtd_outl(COLOR_DLCTI_DM_DCTI_2ND_VALUE_ADDR, dm_dcti_2nd_value_REG.regValue);
//	vip_32bit_tmp[0] = (S_Th<<5) + (S_Gain);
    vip_32bit_tmp[0] = S_Th;
    vip_32bit_tmp[0] = vip_32bit_tmp[0]<<5;
    vip_32bit_tmp[0] += S_Gain;

    if (domain==VIP_D_DOMAIN)
        rtd_outl( DM_DCTI_2ND_VALUE_0x182c, vip_32bit_tmp[0]);
    else
        rtd_outl( DS_DCTI_2ND_VALUE_0x1038, vip_32bit_tmp[0]);

//	dm_dcti_options_RBUS dm_dcti_options_REG;
//	dm_dcti_options_REG.prevent_pe = Prevent_PE;
//	dm_dcti_options_REG.enabled = S_DCTI_Enable;
//	rtd_outl(COLOR_DLCTI_DM_DCTI_OPTIONS_ADDR, dm_dcti_options_REG.regValue);
//	vip_32bit_tmp[0] = (Prevent_PE<<1) + (S_DCTI_Enable);
    vip_32bit_tmp[0] = Prevent_PE;
    vip_32bit_tmp[0] = vip_32bit_tmp[0]<<1;
    vip_32bit_tmp[0] += S_DCTI_Enable;

    if (domain==VIP_D_DOMAIN)
        rtd_outl( DM_DCTI_OPTIONS_0x1824, vip_32bit_tmp[0]);
    else
        rtd_outl( DS_DCTI_OPTIONS_0x1030, vip_32bit_tmp[0]);

}

/*============================================================================*/
/**
 * ColorSetYDLTI
 * Set DLTI of Y path
 *
 * @param <display> {main/sub display}
 * @param <Level> {Level control 0~11, 0:0ff, 1~10 = level}
 */

void drv_color_setydlti(bool domain, UINT8 Level)
{
    if (Level != 0)
    {
        Level = Level -1;

        drv_color_colorsetydlti(	domain,
                                 tDLTI_COEF[Level][0],
                                 tDLTI_COEF[Level][1],
                                 tDLTI_COEF[Level][2],
                                 tDLTI_COEF[Level][3],
                                 tDLTI_COEF[Level][4],
                                 tDLTI_COEF[Level][5],
                                 tDLTI_COEF[Level][6],
                                 tDLTI_COEF[Level][7],
                                 tDLTI_COEF[Level][8]);

        if (domain==VIP_D_DOMAIN)
        {
            rtdf_setBits( D_VC_GLOBAL_CTRL_0x1800, _BIT30);//vivid color enable
            rtdf_setBits( D_VC_GLOBAL_CTRL_0x1800, _BIT1);//enable
        }
        else//i-domain
            rtdf_setBits( I_DLTI_DCTI_CTRL_0x1000, _BIT9);//enable
    }
    else
    {
        if (domain==VIP_D_DOMAIN)
        {
            rtdf_clearBits( D_VC_GLOBAL_CTRL_0x1800, _BIT30);//vivid color disable
            rtdf_clearBits( D_VC_GLOBAL_CTRL_0x1800, _BIT1);//disable
        }
        else//i-domain
            rtdf_clearBits( I_DLTI_DCTI_CTRL_0x1000, _BIT9);//disable
    }
    ;
}

/*============================================================================*/
/**
 * ColorSetCDCTI
 * Set DCTI of C path
 *
 * @param <display> {main/sub display}
 * @param <Level> {Level control 0~11, 0:0ff, 1~10 = level}
 */

void drv_color_setcdcti(bool domain, UINT8 Level)
{
    if (Level != 0)
    {
        Level = Level -1;

        drv_color_colorsetcdcti(	domain,
                                 tDCTI_COEF[Level][0],
                                 tDCTI_COEF[Level][1],
                                 tDCTI_COEF[Level][2],
                                 tDCTI_COEF[Level][3],
                                 tDCTI_COEF[Level][4],
                                 tDCTI_COEF[Level][5]);

        if (domain==VIP_D_DOMAIN)
        {
            rtdf_setBits( D_VC_GLOBAL_CTRL_0x1800, _BIT30);//vivid color enable
            rtdf_setBits( D_VC_GLOBAL_CTRL_0x1800, _BIT2);//enable
        }
        else//i-domain
            rtdf_setBits( I_DLTI_DCTI_CTRL_0x1000, _BIT10);//enable

    }
    else
    {
        if (domain==VIP_D_DOMAIN)
        {
            rtdf_clearBits( D_VC_GLOBAL_CTRL_0x1800, _BIT30);//vivid color disable
            rtdf_clearBits( D_VC_GLOBAL_CTRL_0x1800, _BIT2);//disable
        }
        else//i-domain
            rtdf_clearBits( I_DLTI_DCTI_CTRL_0x1000, _BIT10);//disable

    }

}

/*============================================================================*/
/**
 * Color_SetYContrastBrightness
 * Set contrast & brightness of Y path
 *
 * @param <Contrast> 	{ Y Contrast}
 * @param <Brightness> 	{ Y brightness}
 * @return 			{ void }
 *
 */
void drv_color_setycontrastbrightness( UINT8 Contrast, UINT8 Brightness)
{
//for user osd set Y contrast and brightness
//input Contrast/Brightness ==> 0~255

//	UINT16 K11;
    vip_32bit_tmp[0]=0;//UINT16 new_K11;
    //UINT8 ii;
    vip_32bit_tmp[1]=0;//INT16 new_RGB_offset;
// 1. access tab-1 & 2
// 2. use both tab-1 & 2's k11 for Y contrast
// 3. use both tab-1 & 2's RGB contrast as Y Brightness.
    //RTD_Log(LOGGER_INFO,"Color_SetYContrastBrightness:%d %d\n",Contrast,Brightness);

//	d_yuv2rgb_tab1_data_0_RBUS d_yuv2rgb_tab1_data_0_REG;
//	d_yuv2rgb_tab2_data_0_RBUS d_yuv2rgb_tab2_data_0_REG;

//	d_yuv2rgb_tab1_data_0_REG.regValue = rtd_inl(YUV2RGB_D_YUV2RGB_TAB1_DATA_0_ADDR);
//	d_yuv2rgb_tab2_data_0_REG.regValue = rtd_inl(YUV2RGB_D_YUV2RGB_TAB2_DATA_0_ADDR);

    vip_32bit_tmp[0] = (2 * Contrast);
    vip_32bit_tmp[0] = vip_32bit_tmp[0] & 0x1ff;//new_K11 = (2 * Contrast) & 0x1ff;

//	d_yuv2rgb_tab1_data_2_RBUS d_yuv2rgb_tab1_data_2_REG;
//	d_yuv2rgb_tab1_data_3_RBUS d_yuv2rgb_tab1_data_3_REG;
//	d_yuv2rgb_tab2_data_2_RBUS d_yuv2rgb_tab2_data_2_REG;
//	d_yuv2rgb_tab2_data_3_RBUS d_yuv2rgb_tab2_data_3_REG;

    WaitFor_DEN_STOP();
//	if(display  == _MAIN_DISPLAY )
//	{
//		d_yuv2rgb_tab1_data_0_REG.k11 = new_K11;
    rtdf_maskl( D_YUV2RGB_TAB1_DATA_0_0x1f18, ~(0x7fe00), vip_32bit_tmp[0]<<9);//rtd_outl(YUV2RGB_D_YUV2RGB_TAB1_DATA_0_ADDR, d_yuv2rgb_tab1_data_0_REG.regValue);
    vip_INT32_tmp[0] = Brightness;
    vip_INT32_tmp[0] = vip_INT32_tmp[0] - 128;
    vip_32bit_tmp[1] = (vip_INT32_tmp[0] << 5)&0x1fff; // old shift:3, modified by Jerry Wu 20071012
    vip_32bit_tmp[2] = (vip_32bit_tmp[1]<<13)+vip_32bit_tmp[1];
    rtd_outl( D_YUV2RGB_TAB1_DATA_2_0x1f20, vip_32bit_tmp[2] );//goffset, roffset
    rtd_outl( D_YUV2RGB_TAB1_DATA_3_0x1f24, vip_32bit_tmp[1]);

//		d_yuv2rgb_tab1_data_2_REG.roffset = new_RGB_offset;
//		d_yuv2rgb_tab1_data_2_REG.goffset = new_RGB_offset;
//		d_yuv2rgb_tab1_data_3_REG.boffset = new_RGB_offset;
//		rtd_outl(YUV2RGB_D_YUV2RGB_TAB1_DATA_2_ADDR, d_yuv2rgb_tab1_data_2_REG.regValue);
//		rtd_outl(YUV2RGB_D_YUV2RGB_TAB1_DATA_3_ADDR, d_yuv2rgb_tab1_data_3_REG.regValue);
//	}

}


#if 0//TV team usage
/*============================================================================*/
/**
 * Color_SetYContrastBrightness
 * Set contrast & brightness of Y path
 *
 * @param <Contrast> 	{ Y Contrast}
 * @param <Brightness> 	{ Y brightness}
 * @return 			{ void }
 *
 */

void drv_color_setuv_sat( UINT8 Sat)
{
    vip_32bit_tmp[0]=0;//	UINT32 new_coeffs;
    vip_16bit_table_index=0;//UINT16 *table_index = 0;
// 1. access tab-1 & 2

//	RTD_Log(LOGGER_INFO,"Color_SetUV_Sat%d\n",Sat);
    vip_16bit_table_index = tYUV2RGB_COEF_601_YCbCr;//table_index = tYUV2RGB_COEF_601_YCbCr;
    /*
    	d_yuv2rgb_tab1_data_0_RBUS d_yuv2rgb_tab1_data_0_REG;
    	d_yuv2rgb_tab1_data_1_RBUS d_yuv2rgb_tab1_data_1_REG;
    	d_yuv2rgb_tab2_data_0_RBUS d_yuv2rgb_tab2_data_0_REG;
    	d_yuv2rgb_tab2_data_1_RBUS d_yuv2rgb_tab2_data_1_REG;
    	d_yuv2rgb_tab1_data_0_REG.regValue = rtd_inl(YUV2RGB_D_YUV2RGB_TAB1_DATA_0_ADDR);
    	d_yuv2rgb_tab1_data_1_REG.regValue = rtd_inl(YUV2RGB_D_YUV2RGB_TAB1_DATA_1_ADDR);
    	d_yuv2rgb_tab2_data_0_REG.regValue = rtd_inl(YUV2RGB_D_YUV2RGB_TAB2_DATA_0_ADDR);
    	d_yuv2rgb_tab2_data_1_REG.regValue = rtd_inl(YUV2RGB_D_YUV2RGB_TAB2_DATA_1_ADDR);
    */
    //Sat = (Sat > 128) ? 128+  ((53)  * (Sat -128) /128) : Sat;
    Sat = (Sat > 128) ?  75+ (Sat*53 /128) : Sat;

//	if(display  == _MAIN_DISPLAY )
//	{
    vip_32bit_tmp[0] =  (double)  vip_16bit_table_index[_YUV2RGB_k13] * Sat;//new_coeffs =  (double)  table_index[_YUV2RGB_k13] * Sat;
    vip_32bit_tmp[0] = (vip_32bit_tmp[0] >> 7) ;//new_coeffs = (new_coeffs >> 7) ;
    vip_32bit_tmp[0] = (vip_32bit_tmp[0] > 0x1ff) ? 0x1ff: vip_32bit_tmp[0];//d_yuv2rgb_tab1_data_0_REG.k13 = (new_coeffs > 0x1ff) ? 0x1ff: new_coeffs;
//		RTD_Log(LOGGER_INFO,"Color_SetUV_Sat new_coeff = %d, k13 = %d\n",new_coeffs, d_yuv2rgb_tab1_data_0_REG.k13 );
    rtdf_maskl( D_YUV2RGB_TAB1_DATA_0_0x1f18, ~(0x000000ff), vip_32bit_tmp[0]);//rtd_outl(YUV2RGB_D_YUV2RGB_TAB1_DATA_0_ADDR, d_yuv2rgb_tab1_data_0_REG.regValue);

    vip_32bit_tmp[0] = (double)   vip_16bit_table_index[_YUV2RGB_k22]  * Sat;//new_coeffs = (double)   table_index[_YUV2RGB_k22]  * Sat;
    vip_32bit_tmp[0] = (vip_32bit_tmp[0] >> 7) ;//new_coeffs = (new_coeffs >> 7) ;
    vip_32bit_tmp[0]  = (vip_32bit_tmp[0]  > 0xff) ? 0xff: vip_32bit_tmp[0] ;//d_yuv2rgb_tab1_data_1_REG.k22 = (new_coeffs > 0xff) ? 0xff: new_coeffs;

//		RTD_Log(LOGGER_INFO,"Color_SetUV_Sat new_coeff = %d, k22 = %d\n",new_coeffs, d_yuv2rgb_tab1_data_1_REG.k22 );

    vip_32bit_tmp[1] = (double)   vip_16bit_table_index[_YUV2RGB_k23]  * Sat;//new_coeffs = (double)   table_index[_YUV2RGB_k23]  * Sat;
    vip_32bit_tmp[1] = (vip_32bit_tmp[1] >> 7) ;//new_coeffs = (new_coeffs >> 7) ;
    vip_32bit_tmp[1] = (vip_32bit_tmp[1] > 0xff) ? 0xff: vip_32bit_tmp[1];//d_yuv2rgb_tab1_data_1_REG.k23 = (new_coeffs > 0xff) ? 0xff: new_coeffs;

//		RTD_Log(LOGGER_INFO,"Color_SetUV_Sat new_coeff = %d, k23 = %d\n",new_coeffs, d_yuv2rgb_tab1_data_1_REG.k23 );

    vip_32bit_tmp[2] = (double)   vip_16bit_table_index[_YUV2RGB_k32]  * Sat;//new_coeffs = (double)   table_index[_YUV2RGB_k32]  * Sat;
    vip_32bit_tmp[2] = (vip_32bit_tmp[2] >> 7) ;//new_coeffs = (new_coeffs >> 7) ;
    vip_32bit_tmp[2] =  (vip_32bit_tmp[2] > 0x3ff) ? 0x3ff: vip_32bit_tmp[2];//d_yuv2rgb_tab1_data_1_REG.k32 = (new_coeffs > 0x3ff) ? 0x3ff: new_coeffs;

//		RTD_Log(LOGGER_INFO,"Color_SetUV_Sat new_coeff = %d, k32 = %d\n",new_coeffs, d_yuv2rgb_tab1_data_1_REG.k32 );

    vip_32bit_tmp[0] = (vip_32bit_tmp[0]<<16)+(vip_32bit_tmp[1]<<8)+vip_32bit_tmp[0];
    rtd_outl( D_YUV2RGB_TAB1_DATA_1_0x1f1c, vip_32bit_tmp[0]);//rtd_outl(YUV2RGB_D_YUV2RGB_TAB1_DATA_1_ADDR, d_yuv2rgb_tab1_data_1_REG.regValue);

    //}

}
#endif

/*============================================================================*/
/**
 * Color_SetRgbContrast
 * Set contrast of RGB path
 * @param <display>	{ specify main/sub to set RgbContrastBrightness }
 * @param <Red> 		{ Red Contrast, Range: 0x00(0)~0x80(1)~0xff(2)}
 * @param <Green> 	{ Green Contrast, Range: 0x00(0)~0x80(1)~0xff(2)}
 * @param <Blue> 	{ Blue Contrast, Range: 0x00(0)~0x80(1)~0xff(2)}
 * @return 			{ void }
*/
void drv_color_setrgbcontrast( UINT8 Red, UINT8 Green, UINT8 Blue)
{
//When highlight window is disable, coefficient set A is used.
//This function is for setA

#if 0
    if (display == _MAIN_DISPLAY)
    {
        dm_color_rgb_ctrl_RBUS	dm_color_rgb_ctrl_REG;
        dm_contrast_a_RBUS		dm_contrast_a_REG;

        dm_color_rgb_ctrl_REG.regValue =  rtd_inl(CON_BRI_DM_COLOR_RGB_CTRL_ADDR);
        dm_contrast_a_REG.regValue	= rtd_inl(CON_BRI_DM_CONTRAST_A_ADDR);

        dm_color_rgb_ctrl_REG.m_con_en = 1;
        dm_contrast_a_REG.m_con_a_r	= Red;
        dm_contrast_a_REG.m_con_a_g	= Green;
        dm_contrast_a_REG.m_con_a_b	= Blue;

        rtd_outl(CON_BRI_DM_COLOR_RGB_CTRL_ADDR, dm_color_rgb_ctrl_REG.regValue);
        rtd_outl(CON_BRI_DM_CONTRAST_A_ADDR, dm_contrast_a_REG.regValue);
    }
#endif
    rtdf_pageSelect(_PAGE0);
    rtdf_maskScalerPort( _P0_COLOR_CTRL_62, ~(0x3), 0x3);//enable contrast/brightness
    rtdf_writeScalerPort( _P0_Cts_Bri_Access_Port_Control, 0x83);//bit7: enable, bit[3:0] access port => 0x3:CTS_RED_COE (Set A)
    rtdf_writeScalerPort( _P0_Cts_Bri_Data_Port, Red);
    rtdf_writeScalerPort( _P0_Cts_Bri_Data_Port, Green);
    rtdf_writeScalerPort( _P0_Cts_Bri_Data_Port, Blue);


}

/*============================================================================*/
/**
 * Color_SetRgbBrightness
 * Set brightness of RGB path
 * @param <display>	{ specify main/sub to set RgbContrastBrightness }
 * @param <Red> 		{ Red brightness, Range: 0x000(-512)~0x200(0)~0x3ff(512)}
 * @param <Green> 	{ Green brightness , Range: 0x000(-512)~0x200(0)~0x3ff(512)}
 * @param <Blue> 	{ Blue brightness , Range: 0x000(-512)~0x200(0)~0x3ff(512)}
 * @return 			{ void }
*/

void drv_color_setrgbbrightness(UINT8 Red, UINT8 Green, UINT8 Blue)
{
    //When highlight window is disable, coefficient set A is used.
//This function is for setA

#if 0
    if (display == _MAIN_DISPLAY)
    {
        dm_color_rgb_ctrl_RBUS	dm_color_rgb_ctrl_REG;
        dm_brightness_1_RBUS		dm_brightness_1_REG;

        dm_color_rgb_ctrl_REG.regValue =  rtd_inl(CON_BRI_DM_COLOR_RGB_CTRL_ADDR);
        dm_brightness_1_REG.regValue	= rtd_inl(CON_BRI_DM_BRIGHTNESS_1_ADDR);

        dm_color_rgb_ctrl_REG.m_bri_en = 1;
        dm_brightness_1_REG.m_bri1_r= Red;
        dm_brightness_1_REG.m_bri1_g= Green;
        dm_brightness_1_REG.m_bri1_b= Blue;

        rtd_outl(CON_BRI_DM_COLOR_RGB_CTRL_ADDR, dm_color_rgb_ctrl_REG.regValue);
        rtd_outl(CON_BRI_DM_BRIGHTNESS_1_ADDR, dm_brightness_1_REG.regValue);

    }
#endif
    rtdf_pageSelect(_PAGE0);
    rtdf_writeScalerPort( _P0_Cts_Bri_Access_Port_Control, 0x80);//bit7: enable, bit[3:0] access port => 0x0:BRI_RED_COE (Set A)
    rtdf_writeScalerPort( _P0_Cts_Bri_Data_Port, Red);
    rtdf_writeScalerPort( _P0_Cts_Bri_Data_Port, Green);
    rtdf_writeScalerPort( _P0_Cts_Bri_Data_Port, Blue);

}



/*============================================================================*/
/**
 * Color_SetDithering
 * This function is used to insert dither tables, including SEQ table & dither table
 *
 * @param <void>		{ void }
 * @return 			{ void }
 *
 */

#if 0
void drv_color_setdithering(UINT8 panel_bit)
{
    /*
    	dithering_ctrl1_RBUS dithering_ctrl1_REG;
    	dithering_SEQUENCE_TABLE_Red_00_15_RBUS dithering_SEQUENCE_TABLE_Red_00_15_REG;
    	dithering_SEQUENCE_TABLE_Red_16_31_RBUS dithering_SEQUENCE_TABLE_Red_16_31_REG;
    	dithering_SEQUENCE_TABLE_Green_00_15_RBUS dithering_SEQUENCE_TABLE_Green_00_15_REG;
    	dithering_SEQUENCE_TABLE_Green_16_31_RBUS dithering_SEQUENCE_TABLE_Green_16_31_REG;
    	dithering_SEQUENCE_TABLE_Blue_00_15_RBUS dithering_SEQUENCE_TABLE_Blue_00_15_REG;
    	dithering_SEQUENCE_TABLE_Blue_16_31_RBUS dithering_SEQUENCE_TABLE_Blue_16_31_REG;
    	dithering_Dither_TABLE_Red_00_RBUS dithering_Dither_TABLE_Red_00_REG;
    	dithering_Dither_TABLE_Red_02_RBUS dithering_Dither_TABLE_Red_02_REG;
    	dithering_Dither_TABLE_Green_00_RBUS dithering_Dither_TABLE_Green_00_REG;
    	dithering_Dither_TABLE_Green_02_RBUS dithering_Dither_TABLE_Green_02_REG;
    	dithering_Dither_TABLE_Blue_00_RBUS dithering_Dither_TABLE_Blue_00_REG;
    	dithering_Dither_TABLE_Blue_02_RBUS dithering_Dither_TABLE_Blue_02_REG;
    	temporal_offset_RBUS temporal_offset_REG;
    */

//access dithering sequence table
    //UINT16 ii, jj, kk, data_shift = 0;
    vip_8bit_tmp[0]=0;//ii
    vip_8bit_tmp[1]=0;//jj
    vip_8bit_tmp[2]=0;//kk
    vip_8bit_tmp[3]=0;//data_shift
    vip_8bit_tmp[4]=0;//UINT32 dither_32BIT_DATA = 0;

    rtdf_maskScalerPort( _P0_DITHERING_CTRL1, ~(0x3f), (0x1)<<6);
    for (vip_8bit_tmp[0] = 0; vip_8bit_tmp[0] < 3; vip_8bit_tmp[0]++) // rgb order//for(ii = 0; ii < 3; ii++) // rgb order
    {
        for (vip_8bit_tmp[1] = 0; vip_8bit_tmp[1] < 32; vip_8bit_tmp[1]++)//for(jj = 0; jj < 32; jj++)
        {
            if (  (vip_8bit_tmp[1]%4)==0  )//if((jj == 0)||(jj == 16))
            {
                vip_8bit_tmp[3]=0;//data_shift = 0;
                vip_8bit_tmp[4]=0;//dither_32BIT_DATA = 0;
            }

            vip_8bit_tmp[4] += tDITHER_SEQUENCE[ vip_8bit_tmp[0] ][ vip_8bit_tmp[1] ] << vip_8bit_tmp[3] ;//dither_32BIT_DATA += tDITHER_SEQUENCE[ii][jj] << data_shift ;
            vip_8bit_tmp[3] +=2; //data_shift += 2;

            if (  (vip_8bit_tmp[1]%4)==3 )
                rtdf_writeScalerPort( _P0_DITHER_PORT, vip_8bit_tmp[4]);
            /*
            			if(jj == 15)
            			{
            				switch(ii)
            				{
            					case 0:// red
            							dithering_SEQUENCE_TABLE_Red_00_15_REG.regValue = dither_32BIT_DATA;
            							rtd_outl(DITHER_DITHERING_SEQUENCE_TABLE_RED_00_15_ADDR, dithering_SEQUENCE_TABLE_Red_00_15_REG.regValue);
            							break;
            					case 1:// green
            							dithering_SEQUENCE_TABLE_Green_00_15_REG.regValue = dither_32BIT_DATA;
            							rtd_outl(DITHER_DITHERING_SEQUENCE_TABLE_GREEN_00_15_ADDR, dithering_SEQUENCE_TABLE_Green_00_15_REG.regValue);
            							break;
            					case 2:// blue
            							dithering_SEQUENCE_TABLE_Blue_00_15_REG.regValue = dither_32BIT_DATA;
            							rtd_outl(DITHER_DITHERING_SEQUENCE_TABLE_BLUE_00_15_ADDR, dithering_SEQUENCE_TABLE_Blue_00_15_REG.regValue);
            							break;
            				}

            			}
            			if(jj == 31)
            			{
            				switch(ii)
            				{
            					case 0:// red
            							dithering_SEQUENCE_TABLE_Red_16_31_REG.regValue = dither_32BIT_DATA;
            							rtd_outl(DITHER_DITHERING_SEQUENCE_TABLE_RED_16_31_ADDR, dithering_SEQUENCE_TABLE_Red_16_31_REG.regValue);
            							break;
            					case 1:// green
            							dithering_SEQUENCE_TABLE_Green_16_31_REG.regValue = dither_32BIT_DATA;
            							rtd_outl(DITHER_DITHERING_SEQUENCE_TABLE_GREEN_16_31_ADDR, dithering_SEQUENCE_TABLE_Green_16_31_REG.regValue);
            							break;
            					case 2:// blue
            							dithering_SEQUENCE_TABLE_Blue_16_31_REG.regValue = dither_32BIT_DATA;
            							rtd_outl(DITHER_DITHERING_SEQUENCE_TABLE_BLUE_16_31_ADDR, dithering_SEQUENCE_TABLE_Blue_16_31_REG.regValue);
            							break;
            				}

            			}
            */

        }
    }




//access dithering table
    vip_8bit_tmp[0]=0;//ii
    vip_8bit_tmp[1]=0;//jj
    vip_8bit_tmp[2]=0;//kk
    vip_8bit_tmp[3]=0;//data_shift
    vip_8bit_tmp[4]=0;//UINT32 dither_32BIT_DATA = 0;

    rtdf_maskScalerPort( _P0_DITHERING_CTRL1, ~(0x3f), (0x2)<<6);
    for (vip_8bit_tmp[0] = 0; vip_8bit_tmp[0] < 3; vip_8bit_tmp[0]++)//for(ii = 0; ii < 3; ii++)
    {
        for (vip_8bit_tmp[1] = 0; vip_8bit_tmp[1] < 4; vip_8bit_tmp[1]++)//for(jj = 0; jj < 4; jj++)
        {
            /*
            			if((jj ==0)||(jj ==2))//if((jj ==0)||(jj ==2))
            			{
            				dither_32BIT_DATA = 0;
            				data_shift = 0;
            			}
            */
            for (vip_8bit_tmp[2] = 0; vip_8bit_tmp[2] < 4; vip_8bit_tmp[2]++)//for(kk = 0; kk < 4; kk++)
            {
                if ( (vip_8bit_tmp[2]==0)  || (vip_8bit_tmp[2]==2))
                {
                    vip_8bit_tmp[3]=0;//data_shift = 0;
                    vip_8bit_tmp[4]=0;//dither_32BIT_DATA = 0;
                }

                if (panel_bit == _PANEL_DISPLAY_COLOR_BITS_18)
                {
                    vip_8bit_tmp[4] += tDITHER_LUT_06[vip_8bit_tmp[0]][vip_8bit_tmp[1]][vip_8bit_tmp[2]] << vip_8bit_tmp[3];//dither_32BIT_DATA += tDITHER_LUT[ii][jj][kk] << data_shift;
                }
                else	if (panel_bit == _PANEL_DISPLAY_COLOR_BITS_30)
                {
                    vip_8bit_tmp[4] += tDITHER_LUT_10[vip_8bit_tmp[0]][vip_8bit_tmp[1]][vip_8bit_tmp[2]] << vip_8bit_tmp[3];//dither_32BIT_DATA += tDITHER_LUT[ii][jj][kk] << data_shift;
                }
                else
                {
                    vip_8bit_tmp[4] += tDITHER_LUT_08[vip_8bit_tmp[0]][vip_8bit_tmp[1]][vip_8bit_tmp[2]] << vip_8bit_tmp[3];//dither_32BIT_DATA += tDITHER_LUT[ii][jj][kk] << data_shift;
                }

                vip_8bit_tmp[3] += 4; // modify by Jerry 20080508, fix index error when 6-bit panel in

                if ( (vip_8bit_tmp[2]==1)  || (vip_8bit_tmp[2]==3))
                    rtdf_writeScalerPort( _P0_DITHER_PORT, vip_8bit_tmp[4]);
            }
            /*
            			if(jj == 1)
            			{
            				switch(ii)
            				{
            					case 0: // red
            							dithering_Dither_TABLE_Red_00_REG.regValue= dither_32BIT_DATA;
            							rtd_outl(DITHER_DITHERING_DITHER_TABLE_RED_00_ADDR, dithering_Dither_TABLE_Red_00_REG.regValue);
            							break;
            					case 1: // green
            							dithering_Dither_TABLE_Green_00_REG.regValue= dither_32BIT_DATA;
            							rtd_outl(DITHER_DITHERING_DITHER_TABLE_GREEN_00_ADDR, dithering_Dither_TABLE_Green_00_REG.regValue);
            							break;
            					case 2: // blue
            							dithering_Dither_TABLE_Blue_00_REG.regValue= dither_32BIT_DATA;
            							rtd_outl(DITHER_DITHERING_DITHER_TABLE_BLUE_00_ADDR, dithering_Dither_TABLE_Blue_00_REG.regValue);
            							break;
            				}
            			}

            			if(jj == 3)
            			{
            				switch(ii)
            				{
            					case 0: // red
            							dithering_Dither_TABLE_Red_02_REG.regValue= dither_32BIT_DATA;
            							rtd_outl(DITHER_DITHERING_DITHER_TABLE_RED_02_ADDR, dithering_Dither_TABLE_Red_02_REG.regValue);
            							break;
            					case 1: // green
            							dithering_Dither_TABLE_Green_02_REG.regValue= dither_32BIT_DATA;
            							rtd_outl(DITHER_DITHERING_DITHER_TABLE_GREEN_02_ADDR, dithering_Dither_TABLE_Green_02_REG.regValue);
            							break;
            					case 2: // blue
            							dithering_Dither_TABLE_Blue_02_REG.regValue= dither_32BIT_DATA;
            							rtd_outl(DITHER_DITHERING_DITHER_TABLE_BLUE_02_ADDR, dithering_Dither_TABLE_Blue_02_REG.regValue);
            							break;
            				}

            			}
            */
        }
    }

//access temporal offset
    vip_8bit_tmp[0]=0;//ii
    vip_8bit_tmp[3]=0;//data_shift
    vip_8bit_tmp[4]=0;//UINT32 dither_32BIT_DATA = 0;

    rtdf_maskScalerPort( _P0_DITHERING_CTRL1, ~(0x3f), (0x3)<<6);
    for (vip_8bit_tmp[0] = 0; vip_8bit_tmp[0] < 16; vip_8bit_tmp[0]++)//for(ii = 0; ii < 16; ii++)
    {
        if (  (vip_8bit_tmp[1]%4)==0  )
        {
            vip_8bit_tmp[3]=0;//data_shift = 0;
            vip_8bit_tmp[4]=0;//dither_32BIT_DATA = 0;
        }

        vip_8bit_tmp[4] += tDITHER_TEMPORAL[vip_8bit_tmp[0]] << vip_8bit_tmp[3] ;//ther_32BIT_DATA += tDITHER_TEMPORAL[ii] << (ii *2);
        vip_8bit_tmp[3] += 2;

        if (  (vip_8bit_tmp[1]%4)==3 )
            rtdf_writeScalerPort( _P0_DITHER_PORT, vip_8bit_tmp[4]);

    }

//	temporal_offset_REG.regValue = dither_32BIT_DATA;
//	rtd_outl(DITHER_TEMPORAL_OFFSET_ADDR, temporal_offset_REG.regValue);

    rtdf_writeScalerPort( _P0_DITHERING_CTRL1, 0x0);//dithering_ctrl1_REG.regValue = 0;
#ifdef _PANEL_BIT_10
    rtdf_clearBitsScalerPort( _P0_DITHERING_CTRL1, _BIT5);//dithering_ctrl1_REG.function_enable = 0;
#else
    rtdf_setBitsScalerPort( _P0_DITHERING_CTRL1, _BIT5);//dithering_ctrl1_REG.function_enable = 1;
#endif
    rtdf_setBitsScalerPort( _P0_DITHERING_CTRL1, _BIT4);//dithering_ctrl1_REG.temporal_enable = 1;
//	rtd_outl(_P0_DITHERING_CTRL1, dithering_ctrl1_REG.regValue);



}
#else
void drv_color_setdithering(UINT8 panel_bit)
{
    rtdf_pageSelect(_PAGE0);

    //sequence table
    rtdf_maskScalerPort( _P0_DITHERING_CTRL1, ~(_BIT7 | _BIT6), _BIT6);
#ifndef USE_DMA_COPY
    for (vip_i=0; vip_i<24; vip_i++)
        rtdf_writeScalerPort( _P0_DITHER_PORT, tDITHER_SEQ_TABLE_0[vip_i]);
#else
drv_spi_dma_6093cpy(_P0_DITHER_PORT, _BANK2 , (UINT32)&tDITHER_SEQ_TABLE_0[vip_i], 24, 1);
#endif
    //dither table
    rtdf_maskScalerPort( _P0_DITHERING_CTRL1, ~(_BIT7 | _BIT6), _BIT7);
    if (panel_bit == _PANEL_BIT_06)
    {
#ifndef USE_DMA_COPY
        for (vip_i=0; vip_i<24; vip_i++)
            rtdf_writeScalerPort( _P0_DITHER_PORT, tDITHER_TABLE_10_TO_6[vip_i]);
#else
drv_spi_dma_6093cpy(_P0_DITHER_PORT, _BANK2 , (UINT32)&tDITHER_TABLE_10_TO_6[vip_i], 24, 1);
#endif
    }
    else
    {
#ifndef USE_DMA_COPY
        for (vip_i=0; vip_i<24; vip_i++)
            rtdf_writeScalerPort( _P0_DITHER_PORT, tDITHER_TABLE_10_TO_8[vip_i]);
#else
drv_spi_dma_6093cpy(_P0_DITHER_PORT, _BANK2 , (UINT32)& tDITHER_TABLE_10_TO_8[vip_i], 24, 1);
#endif
    }

    //dither temporal table
    rtdf_maskScalerPort( _P0_DITHERING_CTRL1, ~(_BIT7 | _BIT6), (_BIT7|_BIT6));
    for (vip_i=0; vip_i<4; vip_i++)
        rtdf_writeScalerPort( _P0_DITHER_PORT, tDITHER_TEMPOFFSET_TABLE[vip_i]);

    //disable access control
    rtdf_clearBitsScalerPort( _P0_DITHERING_CTRL1, _BIT7);
    rtdf_clearBitsScalerPort( _P0_DITHERING_CTRL1, _BIT6);

    //dither enable
    if (panel_bit == _PANEL_BIT_10)
        rtdf_clearBitsScalerPort( _P0_DITHERING_CTRL1, _BIT5);
    else
        rtdf_setBitsScalerPort( _P0_DITHERING_CTRL1, _BIT5);

    //dither temporal enable
//	rtdf_setBitsScalerPort( _P0_DITHERING_CTRL1, _BIT4);
    rtdf_clearBitsScalerPort( _P0_DITHERING_CTRL1, _BIT4);

}
#endif


#if 0
/*============================================================================*/
/**
 * Color_SetHighlightWindow Config
 * Set HighlightWindow
 *
 * @return 			{ void }
 *
 */
void drv_color_highlightwindow_config(void)
{
//decide which function support highlight window, and border size

    rtdf_clearBits(HIGHLIGHT_WINDOW_CONTROL_0x2254, _BIT13);//HighLightWin_inv 0:normal  1: high light window inverted

    //Funcitons that support Highlight Window
    rtdf_maskl(HIGHLIGHT_WINDOW_CONTROL_0x2254, ~(HLW_DCC_ICM), HLW_InsideWindow<<10);		//bit[11:10]
    rtdf_maskl(HIGHLIGHT_WINDOW_CONTROL_0x2254, ~(HLW_PeakingCoring), HLW_InsideWindow<<8);		//bit[9:8]
    rtdf_maskl(HIGHLIGHT_WINDOW_CONTROL_0x2254, ~(HLW_DCR), HLW_InsideWindow<<6);				//bit[7:6]
    rtdf_maskl(HIGHLIGHT_WINDOW_CONTROL_0x2254, ~(HLW_sRGB), HLW_InsideWindow<<4);				//bit[5:4]
    rtdf_maskl(HIGHLIGHT_WINDOW_CONTROL_0x2254, ~(HLW_Gamma), HLW_InsideWindow<<4);			//bit[3:2]
    rtdf_maskl(HIGHLIGHT_WINDOW_CONTROL_0x2254, ~(HLW_ContrastBrightness), HLW_InsideWindow<<4);	//bit[1:0]

    //h, v border width
    rtdf_maskl(HIGHTLIGHT_WINDOW_BORDER_WIDTH_0x2130, ~(0x07FF0000), 0<<16);	//H bit[26:16]
    rtdf_maskl(HIGHTLIGHT_WINDOW_BORDER_WIDTH_0x2130, ~(0x000007FF), 0);		//V bit[10:0]

}

/*============================================================================*/
/**
 * Color_SetHighlightWindow
 * Set HighlightWindow
 *
 * @param <Ena_bit> {Enable bit}
 * @param <HSTA> {H start}
 * @param <VSTA> {V start}
 * @param <HEND> {H end}
 * @param <VEND> {V end}
 *
 * @return 			{ void }
 *
 */
void drv_color_sethighlightwindow(UINT8 Ena_bit, UINT16 HSTA, UINT16 VSTA, UINT16 HEND, UINT16 VEND)
{

    //WaitFor_DEN_STOP();
    if (Ena_bit)
    {
        drv_color_highlightwindow_config();//decide which function support highlight window, and border size

        rtdf_maskl(HIGHTLIGHT_WINDOW_H_START_END_0x2128, ~(0x07ff0000), (HSTA & 0x7ff)<<16);
        rtdf_maskl(HIGHTLIGHT_WINDOW_H_START_END_0x2128, ~(0x000007ff), (HEND& 0x7ff));

        rtdf_maskl(HIGHTLIGHT_WINDOW_V_START_END_0x212c, ~(0x07ff0000), (VSTA & 0x7ff)<<16);
        rtdf_maskl(HIGHTLIGHT_WINDOW_V_START_END_0x212c, ~(0x000007ff), (VEND& 0x7ff));

        rtdf_setBitsb(HIGHLIGHT_WINDOW_CONTROL_0x2254, _BIT12);//highlight window of monitor ip
        rtdf_setBitsb(HIGHLIGHT_WINDOW_CONTROL_0x2254, _BIT14);//highlight window of D-domain DLCTi
    }
    else
    {
        rtdf_clearBitsb(HIGHLIGHT_WINDOW_CONTROL_0x2254, _BIT12);//highlight window of monitor ip
        rtdf_clearBitsb(HIGHLIGHT_WINDOW_CONTROL_0x2254, _BIT14);//highlight window of D-domain DLCTi
    }
}
#endif

bit drvif_color_VIP_Quality_Coef_GetValue(UINT8 which_source, VIP_QUALITY_COEF code ** pptr)
{
    VIP_QUALITY_COEF code * ptr;
    /*
    	UINT8 source;
    	if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
    		source = _VGA_MODE_ALL;
    	else
    		source = which_source;
    */
    vip_i = 0;

//	RTD_Log(LOGGER_INFO, "VIP_Quality_Coef_GetValue: which_source=%d, stVIP_Quality_Coef_Table.num=%d\n", (UINT32)which_source, (UINT32)(stVIP_Quality_Coef_Table.num));

    while (vip_i  < stVIP_Quality_Coef_Table.num)
    {
        ptr = &stVIP_Quality_Coef_Table.ptVIP_QUALITY_COEF[vip_i];
        if (ptr->mode == which_source)
        {
            *pptr = ptr;
            break;
        }
        vip_i++;
    }

    if (vip_i >= stVIP_Quality_Coef_Table.num)// Check whether not found
        return _FALSE;
    else
        return _TRUE;

}


void drv_color_system_Init(void)
{
    g_usHorBoundary = 0;
    g_usVerBoundary = 0;
    g_usRGBInfo = 0;
    g_ucPreBacklightPWM = 255;
    g_ucPreDCCLum = 0;
    g_ucDcrCtrl = 0;
    g_ucDCRPercent = 100;  // Max 100
    g_ucUserBacklight = 0;
}
/*======================== End of File =======================================*/
/**
*
* @}
*/

