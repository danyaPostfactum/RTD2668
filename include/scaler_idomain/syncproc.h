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
#include "rtd_types.h"
#include "source.h"

/**
* @addtogroup
* @
*/
#ifndef _SYNCPROC_H
#define _SYNCPROC_H
#define SOG1                              0
#define SOG2                              1
#define  SOG_SOURCE              SOG1
#define  SOY_SOURCE              SOG2

#define SOY0_PATH             0
#define SOY1_PATH             1

#ifdef CONFIG_YPBPR_USE_AUTO_SOY
#define YPBPR_SOY_PATH_SELECT  SOY0_PATH
#define VGA_SOG_PATH_SELECT    SOY1_PATH
#else
#define YPBPR_SOY_PATH_SELECT  SOY0_PATH
#define VGA_SOG_PATH_SELECT    SOY0_PATH
#endif


#define SYNC_PROCESSOR1         0
#define SYNC_PROCESSOR2         1

#define CLAMP_TO_BLACK          0
#define CLAMP_TO_MIDDLE        1
#define CLAMP_TO_SYNC            2
#define Y_CLAMP_METHOD      CLAMP_TO_BLACK

#define MEASURE_BLOCK1           0
#define MEASURE_BLOCK2           1

#define YPBPR_ABL_ENABLE       _DISABLE
#define YPBPR_AGC_ENABLE      _DISABLE

#define DISP_1080P_CLAMP_Y_START	0x08
#define DISP_1080P_CLAMP_Y_END	0x15
#define DISP_1080P_CLAMP_NY_START	0x08
#define DISP_1080P_CLAMP_NY_END	0x15

#define DISP_HD_CLAMP_Y_START		0x15
#define DISP_HD_CLAMP_Y_END		0x30
#define DISP_HD_CLAMP_NY_START	0x15
#define DISP_HD_CLAMP_NY_END		0x30

#define DISP_SD_CLAMP_Y_START		0x10
#define DISP_SD_CLAMP_Y_END		0x30
#define DISP_SD_CLAMP_NY_START	0x10
#define DISP_SD_CLAMP_NY_END		0x30

#define CLAMP_Y      0
#define CLAMP_NY    1
#define CLAMP_ALL	2

#define CLAMP_TRAILING_EDGE	1 //Back Porch
#define CLAMP_LEADING_EDGE	0 //front Porch

#define CLAMP_Y_REF_EDGE	CLAMP_LEADING_EDGE
#define CLAMP_NY_REF_EDGE	CLAMP_LEADING_EDGE

#define CLAMP_CLKSRC_ADC		1
#define CLAMP_CLKSRC_CRYSTAL	0

#define CLAMP_CLKSRC			CLAMP_CLKSRC_ADC

#define CLAMP_CNT_UNIT_1CLK	0
#define CLAMP_CNT_UNIT_2CLK	1

#define CLAMP_CNT_UNIT			CLAMP_CNT_UNIT_2CLK

#if (CLAMP_CNT_UNIT	== CLAMP_CNT_UNIT_2CLK)
#define CLAMP_POSITION_TOLERANCE	2	//in 2x clock it is 4
#else //#if (CLAMP_CNT_UNIT	== CLAMP_CNT_UNIT_2CLK)
#define CLAMP_POSITION_TOLERANCE	5
#endif //#if (CLAMP_CNT_UNIT	== CLAMP_CNT_UNIT_2CLK)
#define CLAMP_POSITION_CHECK_CNT	10


//SP1_CTRL[2:0], S1_vhs_source_sel
#define SP_VHS_SOURCE_PTG_CS_HS1VS1	0  //ptg_adc_cshs,csvs (PatternGen Composite Signal)
#define SP_VHS_SOURCE_PTG_HS1VS1		1 //ptg_adc_hs,vs (PatternGen Separate  Signal)
#define SP_VHS_SOURCE_PAD_HS1VS1		2 //pad_hs1,pad_vs1
#define SP_VHS_SOURCE_JITTER_PTG		3 //jitter PTG (From APLL Pattern Gen)
#define SP_VHS_SOURCE_SOY_0			4 //SOY0
#define SP_VHS_SOURCE_SOY_1			5 //SOY1

#define _IN_HS_SEL_HSRAW	0
#define _IN_HS_SEL_SOY		1	// SOY/SOG

#define ANALOG_MODE_MEASUREMENT      0
#define DIGITAL_MODE_MEASUREMENT     1

#define ADC_SWAP_RGB		0
#define ADC_SWAP_GRB		1
#define ADC_SWAP_BGR		2
#define ADC_SWAP_RBG		3
#define ADC_SWAP_BRG		4
//?#define ADC_SWAP_BRG		5
#define ADC_SWAP_GBR		6
//?#define ADC_SWAP_BGR		7

#define ADC_0_SWAP_INDEX	ADC_SWAP_RGB
#define ADC_1_SWAP_INDEX	ADC_SWAP_RGB
#define ADC_2_SWAP_INDEX	ADC_SWAP_RGB

#define ADC_BW_10M		0
#define ADC_BW_25M		1
#define ADC_BW_37_5M	2
#define ADC_BW_50M		3
#define ADC_BW_75M		4
#define ADC_BW_150M	5
#define ADC_BW_300M	6
#define ADC_BW_500M	7

enum _OFFLINE_MS_SRC
{
    _OFFLINE_MS_SRC_PATTERN = 0,
    _OFFLINE_MS_SRC_ADCHV1,
    _OFFLINE_MS_SRC_RESERVE1,
    _OFFLINE_MS_SRC_RESERVE2,
    _OFFLINE_MS_SRC_RAWHV1,
    _OFFLINE_MS_SRC_DEHV1,	//decomposite
    _OFFLINE_MS_SRC_RAWHV2,
    _OFFLINE_MS_SRC_DEHV2,
    _OFFLINE_MS_SRC_HDMI,
    _OFFLINE_MS_SRC_VDC,
    _OFFLINE_MS_SRC_VIDEO8_1,
    _OFFLINE_MS_SRC_VIDEO8_2,
    _OFFLINE_MS_SRC_VIDEO16,
};

#define VGA_SYNC_DETECT_MODE	3
//VGA support composite sync
enum
{
    SEPARATE_MODE = 0x00,
    CS_ON_H_MODE ,
    SOG_MODE ,
};
#define NO_SIGNAL									(0)
#define NOT_SUPPORT								(1)
#define YPBPR_SYNC									(2)
#define SERRATION_COMPOSITE_SYNC					(3)
#define XOR_OR_COMPOSITE_WITH_EQUALIZER			(4)
#define XOR_OR_COMPOSITE_WITHOUT_EQUALIZER		(5)
#define SEPARATE_SYNC								(6)
#define HSYNC_ONLY 									(7)
#define SYNC_CHANGE								(8)


UINT32 code YPBPR_INIT[];
UINT32 code VGA_Init[];

extern UINT8 VGADetecSyncTypeMode;

UINT8 drv_syncproc_Get_Sync_Type(void);

void drv_syncproc_VGA_sync_input(void);
//void drv_syncproc_VGA_SOG_Init(void);

UINT8 drv_syncproc_DetectMode(StructSourceInput *srcinput_pt);
UINT8 drv_syncproc_CheckMode(void);
void	drv_syncproc_InitSrc(UINT32 *init_table);	// Init input source setting(ex.VGA,YPbPr...)
void drv_syncproc_SOYSourceSelect(UINT8 soy_source_index);
UINT8 drv_syncproc_ModeHandler(StructSourceInput *srcinput_pt);
void drv_syncproc_YPbPr_ADCSetting(void);

void drv_syncproc_FunctionDisable(void);
void drv_syncproc_FunctionPowerDown(void);
void drv_syncproc_ScalerSetHPosition(void);
void drv_syncproc_ScalerSetVPosition(void);
void drv_syncproc_Adjust_ADC_Clock(UINT8 fast_lock_enable);
void drv_syncproc_YPbPr_SetPhase(UINT8 phase, UINT8 uc64phase_limited);
UINT8 drv_syncproc_YPbPr_GetPhase(UINT8 Mode_Curr);
void drv_syncproc_YPbPr_Init(void);
void drv_syncproc_ResetICodeSetting(UINT8 icode);
void drv_syncproc_ClampPositionAdjust(void);
UINT8 drv_syncproc_SetClampPulsePos(UINT8 clamp_start, UINT8 clamp_end, UINT8 select); //select for Y or nY

void drv_syncproc_YPbPr_ModeSetting(StructSourceInput *srcinput_pt);
//void drv_syncproc_VGA_ModeSetting(StructSourceInput *srcinput_pt);
UINT8 drv_syncproc_inprocypbpr_setup(void);
UINT8 drv_syncproc_inprocvga_setup(void);
void drv_syncproc_PLL27X_Enable(UINT8 enable);
BOOL drv_hsyncdetect_powersaving(void);

#endif // #ifndef _SYNCPROC_H
