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
#include "video\video.h"
#include "ifd\ifd.h"
#include "tuner\tuner.h"
#include "audio\audio_api.h"
#include "flow_struct.h"
#include "ui_struct.h"
#include "scaler_idomain\syncproc.h"
#include "scaler_idomain\measure.h"
#include "scaler\scaler_pll.h"
#include "reg_def.h"
#include "flow_scaler.h"
#include "rosprintf.h"
#include "timer_event.h"
#include "mode.h"
#include "mode_customer.h"
#include "isr_utility.h"
#include "tv.h"
#include "vip\ypbpr_auto.h"
#include "osdcore.h"
#include "osdcontrol.h"

#include "pcb_conf.h"

#ifdef CONFIG_VBI_CC_SUPPORT
#include "vbi/slr.h"
#endif

#define _VIDEO_INDEX	0x5000
#if 0
static UINT8 vdc_InputSrc = 0xff;
UINT8 vdc_dcti_talbe_value = 0;
UINT8 vdc_factory_mod_enable = 0;
UINT8 ucVdc_Curr_Mode = 0x00;
UINT8 ucVdc_Pre_Mode = 0x11;
static VDC_FRAME_RATE field = VDC_FRAME_RATE_50HZ;

//// Y/C separation Function ////
UINT8 YcSeparationStatus = VDC_YCSEPARATE_DEFAULT;
bool cvideo_tv_channel_scan_is_running = 0;
UINT8 SCART_Type = _SCART_AV;	// 0: is scart av, 2: is scart sv, 1: is scart rgb
UINT8 pre_SCART_Type = 0xff;
UINT8 SCART_Counter = 0x00;
bool SvNoBurstMark = 1;
#define cSCART_constant     0x05

#ifdef CONFIG_ENABLE_FHFVBug
bool    cvideo_fvfh_set_1d = _OFF;
UINT16 FieldVTotal;
UINT8 ucFHFVBug_Counter = 5;
//UINT16 OriginalIPHeriod;
//UINT16 OriginalDclkOffset;
#endif

#ifdef CONFIG_ENABLE_NTSC_No_Burst_Check
UINT8 xdata bNTSCnoBurst = 0;
#endif
#else
UINT8 vdc_InputSrc;
UINT8 vdc_dcti_talbe_value;
UINT8 vdc_factory_mod_enable;
UINT8 ucVdc_Curr_Mode;
UINT8 ucVdc_Pre_Mode;
VDC_FRAME_RATE field;

//// Y/C separation Function ////
UINT8 YcSeparationStatus;
bool cvideo_tv_channel_scan_is_running;
UINT8 SCART_Type;	// 0: is scart av, 2: is scart sv, 1: is scart rgb
UINT8 pre_SCART_Type;
UINT8 SCART_Counter;
bool SvNoBurstMark;
#define cSCART_constant     0x02

#ifdef CONFIG_ENABLE_FHFVBug
bool    cvideo_fvfh_set_1d;
UINT16 FieldVTotal;
UINT8 ucFHFVBug_Counter;
//UINT16 OriginalIPHeriod;
//UINT16 OriginalDclkOffset;
#endif

#ifdef CONFIG_ENABLE_NTSC_No_Burst_Check
UINT8 xdata bNTSCnoBurst ;
#endif
#endif

//#if CONFIG_ENABLE_RTD_LOG_VIDEO
#define VDC_PRINTF	RTD_Log
//#else
//#define VDC_PRINTF
//#endif

#define tv_AUTO			0x80
#define tv_ZNTSC			0x00
#define tv_ZPAL_I		0x06
#define tv_ZPAL_M		0x01
#define tv_ZPAL_N		0x03
#define tv_ZNTSC_443	0x04
#define tv_ZSECAM		0x07
#define tv_ZPAL_60		0x05
#define tv_ZNTSC_50		0x02


#ifdef CONFIG_ENABLE_TV_NOISE_BOX_ENALBE
bit bTvNoiseBox = 0;
#define GET_NoiseBoxStatus()		(bTvNoiseBox)
#define SET_NoiseBoxStatus(x)		(bTvNoiseBox = x)
#endif

static UINT8  code VDC_1D_SET1[]=
//no_signal, NTSC, NTSC_443, NTSC_50, SECAM
//modify to LV table 20090624
{
#if 0
    4, HIGH_BYTE(VDC_YC_SEP_CONTROL_0x5280), LOW_BYTE(VDC_YC_SEP_CONTROL_0x5280), 0x03,
    4, HIGH_BYTE(VDC_BPF_BW_SEL_Y_0x5284), LOW_BYTE(VDC_BPF_BW_SEL_Y_0x5284), 0x43,
    4, HIGH_BYTE(VDC_BPF_BW_SEL_C_0x5288), LOW_BYTE(VDC_BPF_BW_SEL_C_0x5288), 0x60,
    4, HIGH_BYTE(VDC_ADAP_BPF_C_TH1_0x528c), LOW_BYTE(VDC_ADAP_BPF_C_TH1_0x528c), 0x0c,
    4, HIGH_BYTE(VDC_ADAP_BPF_C_TH2_0x5290), LOW_BYTE(VDC_ADAP_BPF_C_TH2_0x5290), 0x14,
#else
    8, HIGH_BYTE(VDC_YC_SEP_CONTROL_0x5280), LOW_BYTE(VDC_YC_SEP_CONTROL_0x5280), 0x03,0x43,0x60,0x0c,0x14,
#endif
    4, HIGH_BYTE(VDC_ADAP_BPF_Y_TH2_0x52a0), LOW_BYTE(VDC_ADAP_BPF_Y_TH2_0x52a0), 0x03,
    4, HIGH_BYTE(VDC_YC_BW_CTRL_0x52a8), LOW_BYTE(VDC_YC_BW_CTRL_0x52a8), 0x01,
    4, HIGH_BYTE(VDC_BW_DETECTION_0x52b0), LOW_BYTE(VDC_BW_DETECTION_0x52b0), 0x60,
    4, HIGH_BYTE(VDC_H2V_Y_NOISE_THR_0x52c8), LOW_BYTE(VDC_H2V_Y_NOISE_THR_0x52c8), 0x03,
    4, HIGH_BYTE(VDC_H2V_NOISE_MAX_HDY_0x52d0), LOW_BYTE(VDC_H2V_NOISE_MAX_HDY_0x52d0), 0x0f,
    _TBL_END
};

static UINT8 code DRAGON_1D_SET2[]=
//PAL_I, PAL_M, PAL_N
//modify to LV table 20090626
{
#if 0
    4, HIGH_BYTE(VDC_YC_SEP_CONTROL_0x5280),     LOW_BYTE(VDC_YC_SEP_CONTROL_0x5280),     0x03,
    4, HIGH_BYTE(VDC_BPF_BW_SEL_Y_0x5284),       LOW_BYTE(VDC_BPF_BW_SEL_Y_0x5284),       0x43,
    4, HIGH_BYTE(VDC_BPF_BW_SEL_C_0x5288),       LOW_BYTE(VDC_BPF_BW_SEL_C_0x5288),       0x60,
    4, HIGH_BYTE(VDC_ADAP_BPF_C_TH1_0x528c),     LOW_BYTE(VDC_ADAP_BPF_C_TH1_0x528c),     0x03,
    4, HIGH_BYTE(VDC_ADAP_BPF_C_TH2_0x5290),     LOW_BYTE(VDC_ADAP_BPF_C_TH2_0x5290),     0x0c,
#else
    8, HIGH_BYTE(VDC_YC_SEP_CONTROL_0x5280), LOW_BYTE(VDC_YC_SEP_CONTROL_0x5280), 0x03,0x43,0x60,0x03,0x0c,
#endif
    4, HIGH_BYTE(VDC_ADAP_BPF_Y_TH2_0x52a0),     LOW_BYTE(VDC_ADAP_BPF_Y_TH2_0x52a0),     0x13,
    4, HIGH_BYTE(VDC_YC_BW_CTRL_0x52a8),         LOW_BYTE(VDC_YC_BW_CTRL_0x52a8),         0x49,
    4, HIGH_BYTE(VDC_BW_DETECTION_0x52b0 ),        LOW_BYTE(VDC_BW_DETECTION_0x52b0),         0x60,
    4, HIGH_BYTE(VDC_H2V_Y_NOISE_THR_0x52c8 ),   LOW_BYTE(VDC_H2V_Y_NOISE_THR_0x52c8),    0x03,
    4, HIGH_BYTE(VDC_H2V_NOISE_MAX_HDY_0x52d0 ),   LOW_BYTE(VDC_H2V_NOISE_MAX_HDY_0x52d0),    0x0f,
    _TBL_END
};

static UINT8 code DRAGON_1D_SET3[]=
//PAL_60
//modify to LV table 20090626
{
    4, HIGH_BYTE(VDC_VIDEO_CONTROL0_0x5000),     LOW_BYTE(VDC_VIDEO_CONTROL0_0x5000),     0x0e,
#if 0
    4, HIGH_BYTE(VDC_YC_SEP_CONTROL_0x5280),     LOW_BYTE(VDC_YC_SEP_CONTROL_0x5280),     0x03,
    4, HIGH_BYTE(VDC_BPF_BW_SEL_Y_0x5284),       LOW_BYTE(VDC_BPF_BW_SEL_Y_0x5284),       0x43,
    4, HIGH_BYTE(VDC_BPF_BW_SEL_C_0x5288),       LOW_BYTE(VDC_BPF_BW_SEL_C_0x5288),       0x60,
#else
    6, HIGH_BYTE(VDC_YC_SEP_CONTROL_0x5280),     LOW_BYTE(VDC_YC_SEP_CONTROL_0x5280),     0x03,0x43,0x60,
#endif
    4, HIGH_BYTE(VDC_YC_BW_CTRL_0x52a8),         LOW_BYTE(VDC_YC_BW_CTRL_0x52a8),         0x49,
    4, HIGH_BYTE(VDC_BW_DETECTION_0x52b0 ),        LOW_BYTE(VDC_BW_DETECTION_0x52b0),         0x60,
    4, HIGH_BYTE(VDC_H2V_Y_NOISE_THR_0x52c8 ),   LOW_BYTE(VDC_H2V_Y_NOISE_THR_0x52c8),    0x03,
    4, HIGH_BYTE(VDC_H2V_NOISE_MAX_HDY_0x52d0 ),   LOW_BYTE(VDC_H2V_NOISE_MAX_HDY_0x52d0),    0x0f,
    _TBL_END
};

static UINT8 code DRAGON_2D_SET1[]=
//Mode 0 : NTSC
//modify to LV table 20090626
{
    4, HIGH_BYTE(VDC_VIDEO_CONTROL0_0x5000),     LOW_BYTE(VDC_VIDEO_CONTROL0_0x5000),     0x00,
#if 0
    4, HIGH_BYTE(VDC_YC_SEP_CONTROL_0x5280),     LOW_BYTE(VDC_YC_SEP_CONTROL_0x5280),     0x00,
    4, HIGH_BYTE(VDC_BPF_BW_SEL_Y_0x5284),       LOW_BYTE(VDC_BPF_BW_SEL_Y_0x5284),       0x65,
    4, HIGH_BYTE(VDC_BPF_BW_SEL_C_0x5288),       LOW_BYTE(VDC_BPF_BW_SEL_C_0x5288),       0x00,
#else
    6, HIGH_BYTE(VDC_YC_SEP_CONTROL_0x5280),     LOW_BYTE(VDC_YC_SEP_CONTROL_0x5280),     0x00,0x65,0x30,
#endif
    4, HIGH_BYTE(VDC_YC_BW_CTRL_0x52a8),        LOW_BYTE(VDC_YC_BW_CTRL_0x52a8),         0x01,
    4, HIGH_BYTE(VDC_BW_DETECTION_0x52b0),        LOW_BYTE(VDC_BW_DETECTION_0x52b0),         0x10,
    4, HIGH_BYTE(VDC_H2V_Y_NOISE_THR_0x52c8),    LOW_BYTE(VDC_H2V_Y_NOISE_THR_0x52c8),    0x02,
    4, HIGH_BYTE(VDC_H2V_NOISE_MAX_HDY_0x52d0),   LOW_BYTE(VDC_H2V_NOISE_MAX_HDY_0x52d0),    0x2f,
    4, HIGH_BYTE(VDC_H2V_BLEND_RATIO_0x52d8),   LOW_BYTE(VDC_H2V_BLEND_RATIO_0x52d8),    0x04,
    4, HIGH_BYTE(VDC_COMB_FILTER_THRESHOLD1_0x52e0),   LOW_BYTE(VDC_COMB_FILTER_THRESHOLD1_0x52e0),    0x02,
    4, HIGH_BYTE(VDC_CHROMA_DELAY_CTRL_0x50fc),   LOW_BYTE(VDC_CHROMA_DELAY_CTRL_0x50fc),    0x20,
    4, HIGH_BYTE(VDC_XNR_CTRL_0x54f0),   LOW_BYTE(VDC_XNR_CTRL_0x54f0),    0xc1,
    4, HIGH_BYTE(VDC_XNR_THR_0x54f4),   LOW_BYTE(VDC_XNR_THR_0x54f4),    0x58,
    4, HIGH_BYTE(VDC_XNR_IDX_THR_0x54f8),   LOW_BYTE(VDC_XNR_IDX_THR_0x54f8),    0x33,
    4, HIGH_BYTE(VDC_XNR_RATIO_CTRL_0x54fc),   LOW_BYTE(VDC_XNR_RATIO_CTRL_0x54fc),    0x11,
    _TBL_END
};

static UINT8 code DRAGON_2D_SET2[]=
//Mode 2 : PAL
//modify to LV table 20090626
{
    4, HIGH_BYTE(VDC_VIDEO_CONTROL0_0x5000),	LOW_BYTE(VDC_VIDEO_CONTROL0_0x5000),	0x00,
#if 0
    4, HIGH_BYTE(VDC_YC_SEP_CONTROL_0x5280),	LOW_BYTE(VDC_YC_SEP_CONTROL_0x5280),	0x02,
    4, HIGH_BYTE(VDC_BPF_BW_SEL_Y_0x5284),		LOW_BYTE(VDC_BPF_BW_SEL_Y_0x5284),	0x43,
    4, HIGH_BYTE(VDC_BPF_BW_SEL_C_0x5288),		LOW_BYTE(VDC_BPF_BW_SEL_C_0x5288),	0x11,
#else
    6, HIGH_BYTE(VDC_YC_SEP_CONTROL_0x5280),     LOW_BYTE(VDC_YC_SEP_CONTROL_0x5280),     0x02,0x65,0x31,
#endif
    4, HIGH_BYTE(VDC_YC_BW_CTRL_0x52a8 ),		LOW_BYTE(VDC_YC_BW_CTRL_0x52a8),		0x49,
    4, HIGH_BYTE(VDC_BW_DETECTION_0x52b0 ),	LOW_BYTE(VDC_BW_DETECTION_0x52b0),	0x10,
    4, HIGH_BYTE(VDC_H2V_Y_NOISE_THR_0x52c8),	LOW_BYTE(VDC_H2V_Y_NOISE_THR_0x52c8),	0x03,
#if 0
    4, HIGH_BYTE(VDC_H2V_NOISE_MAX_HDY_0x52d0 ),	LOW_BYTE(VDC_H2V_NOISE_MAX_HDY_0x52d0),	0x06,
    4, HIGH_BYTE(VDC_H2V_NOISE_Y_ADD_DC_0x52d4),	LOW_BYTE(VDC_H2V_NOISE_Y_ADD_DC_0x52d4),	0x05,
    4, HIGH_BYTE(VDC_H2V_BLEND_RATIO_0x52d8 ),	LOW_BYTE(VDC_H2V_BLEND_RATIO_0x52d8),	0x07,
#else
    6, HIGH_BYTE(VDC_H2V_NOISE_MAX_HDY_0x52d0),     LOW_BYTE(VDC_H2V_NOISE_MAX_HDY_0x52d0),     0x0f,0x0a,0x07,
#endif
    4, HIGH_BYTE(VDC_COMB_FILTER_THRESHOLD1_0x52e0 ),	LOW_BYTE(VDC_COMB_FILTER_THRESHOLD1_0x52e0),	0x02,
    4, HIGH_BYTE(VDC_CHROMA_DELAY_CTRL_0x50fc),   LOW_BYTE(VDC_CHROMA_DELAY_CTRL_0x50fc),    0x20,
    4, HIGH_BYTE(VDC_XNR_CTRL_0x54f0),   LOW_BYTE(VDC_XNR_CTRL_0x54f0),    0xc1,
    4, HIGH_BYTE(VDC_XNR_THR_0x54f4),   LOW_BYTE(VDC_XNR_THR_0x54f4),    0x58,
    4, HIGH_BYTE(VDC_XNR_IDX_THR_0x54f8),   LOW_BYTE(VDC_XNR_IDX_THR_0x54f8),    0x13,
    4, HIGH_BYTE(VDC_XNR_RATIO_CTRL_0x54fc),   LOW_BYTE(VDC_XNR_RATIO_CTRL_0x54fc),    0x12,
    _TBL_END
};

static UINT8  code VDC_DCTI_SET1[]=
{
    //PAL M ,PALN NTSC50,NTSC443,PAL60   disable the function
#if 0
    4, HIGH_BYTE(VDC_CADAP_ENABLE_CTRL_0x5600), LOW_BYTE(VDC_CADAP_ENABLE_CTRL_0x5600), 0x1e,
    4, HIGH_BYTE(VDC_CADAP_ENABLE_CTRL2_0x5604), LOW_BYTE(VDC_CADAP_ENABLE_CTRL2_0x5604), 0x00,
    4, HIGH_BYTE(VDC_CADAP_GAIN_CTRL_0x5608), LOW_BYTE(VDC_CADAP_GAIN_CTRL_0x5608), 0x25,
#else
    6, HIGH_BYTE(VDC_CADAP_ENABLE_CTRL_0x5600),     LOW_BYTE(VDC_CADAP_ENABLE_CTRL_0x5600),     0x1e,0x00,0x15,
#endif
#if 0
    4, HIGH_BYTE(VDC_CADAP_LUT_LEVEL_TH_0x561c), LOW_BYTE(VDC_CADAP_LUT_LEVEL_TH_0x561c), 0x27,
    4, HIGH_BYTE(VDC_CADAP_LUT_GAIN_TH_0x5620), LOW_BYTE(VDC_CADAP_LUT_GAIN_TH_0x5620), 0x23,
#else
    5, HIGH_BYTE(VDC_CADAP_LUT_LEVEL_TH_0x561c),     LOW_BYTE(VDC_CADAP_LUT_LEVEL_TH_0x561c),     0x27,0x23,
#endif
    _TBL_END
};

static UINT8  code VDC_DCTI_SET2[]=
{
    //ntsc pal i  , av setting  // ives 20090324
#if 0
    4, HIGH_BYTE(VDC_CADAP_ENABLE_CTRL_0x5600), LOW_BYTE(VDC_CADAP_ENABLE_CTRL_0x5600), 0x1f,
    4, HIGH_BYTE(VDC_CADAP_ENABLE_CTRL2_0x5604), LOW_BYTE(VDC_CADAP_ENABLE_CTRL2_0x5604), 0x07,
    4, HIGH_BYTE(VDC_CADAP_GAIN_CTRL_0x5608), LOW_BYTE(VDC_CADAP_GAIN_CTRL_0x5608), 0x25,
    4, HIGH_BYTE(VDC_CADAP_ZC_TH_0x560c), LOW_BYTE(   VDC_CADAP_ZC_TH_0x560c), 0x74,// 0x26
#else
    7, HIGH_BYTE(VDC_CADAP_ENABLE_CTRL_0x5600),     LOW_BYTE(VDC_CADAP_ENABLE_CTRL_0x5600),     0x1f,0x07,0x58,0x45,
    7, HIGH_BYTE(VDC_CADAP_ZC_TH2_0x5610),     LOW_BYTE(VDC_CADAP_ZC_TH2_0x5610),     0x74,0x85,0x00,0x12,

#endif
#if 0
    4, HIGH_BYTE(VDC_CADAP_SYNC_UNIFORMITY_TH_0x5618), LOW_BYTE(VDC_CADAP_SYNC_UNIFORMITY_TH_0x5618), 0x10,// 0x50
    4, HIGH_BYTE(VDC_CADAP_LUT_LEVEL_TH_0x561c), LOW_BYTE(VDC_CADAP_LUT_LEVEL_TH_0x561c), 0x45,
    4, HIGH_BYTE(VDC_CADAP_LUT_GAIN_TH_0x5620), LOW_BYTE(VDC_CADAP_LUT_GAIN_TH_0x5620), 0x34,
    6, HIGH_BYTE(VDC_CADAP_SYNC_UNIFORMITY_TH_0x5618),     LOW_BYTE(VDC_CADAP_SYNC_UNIFORMITY_TH_0x5618),     0x10,0x45,0x34,
#else

#endif
    _TBL_END
};

static UINT8  code VDC_DCTI_SET3[]=
{
    //ntsc pal i  , tv setting
#if 0
    4, HIGH_BYTE(VDC_CADAP_ENABLE_CTRL_0x5600), LOW_BYTE(VDC_CADAP_ENABLE_CTRL_0x5600), 0x1f,
    4, HIGH_BYTE(VDC_CADAP_ENABLE_CTRL2_0x5604), LOW_BYTE(VDC_CADAP_ENABLE_CTRL2_0x5604), 0x07,
    4, HIGH_BYTE(VDC_CADAP_GAIN_CTRL_0x5608), LOW_BYTE(VDC_CADAP_GAIN_CTRL_0x5608), 0x22,
    4, HIGH_BYTE(VDC_CADAP_ZC_TH_0x560c), LOW_BYTE(   VDC_CADAP_ZC_TH_0x560c), 0x35,
#else
    7, HIGH_BYTE(VDC_CADAP_ENABLE_CTRL_0x5600),     LOW_BYTE(VDC_CADAP_ENABLE_CTRL_0x5600),     0x1f,0x07,0x22,0x35,
#endif
#if 0
    4, HIGH_BYTE(VDC_CADAP_SYNC_UNIFORMITY_TH_0x5618), LOW_BYTE(VDC_CADAP_SYNC_UNIFORMITY_TH_0x5618), 0x50,
    4, HIGH_BYTE(VDC_CADAP_LUT_LEVEL_TH_0x561c), LOW_BYTE(VDC_CADAP_LUT_LEVEL_TH_0x561c), 0x45,
    4, HIGH_BYTE(VDC_CADAP_LUT_GAIN_TH_0x5620), LOW_BYTE(VDC_CADAP_LUT_GAIN_TH_0x5620), 0x34,
#else
    6, HIGH_BYTE(VDC_CADAP_SYNC_UNIFORMITY_TH_0x5618),     LOW_BYTE(VDC_CADAP_SYNC_UNIFORMITY_TH_0x5618),     0x50,0x45,0x12,
#endif
    _TBL_END
};

//==========  Input path setting functions  =============================//
/**
	module_vdc_SetADCPath
	ADC path set, no extern
	@param{ none }
	@return uint8

	//Signal
		#define VIDEO_INPUT00	0
		#define VIDEO_INPUT01	1
		#define VIDEO_INPUT02	2
		#define VIDEO_INPUT03	3
		#define VIDEO_INPUT10	4
		#define VIDEO_INPUT11	5
		#define VIDEO_INPUT12	6
		#define VIDEO_RGB0		7
		#define VIDEO_RGB1		8
		#define TV_INPUT			9
		#define VIDEO_INPUT_R0	a
		#define VIDEO_INPUT_G0	b
		#define VIDEO_INPUT_B0	c
		#define VIDEO_INPUT_R1	d
		#define VIDEO_INPUT_G1	e
		#define VIDEO_INPUT_B1	f
 	//GND
		#define VIDEO_GND0		0	//AD0 GND input
		#define VIDEO_GND1		1	//AD1 GND input
*/
static UINT8 module_vdc_SetADCPath(UINT8 path_signal, UINT8 path_gnd)
{
    if (path_signal<=VIDEO_INPUT12)
    {
        if ((path_signal&0x0c)==0x00)
        {	// set AD0
            rtdf_maskl(VDC_VD_INSEL_0x5744,0xcf,((path_signal&0x03)<<4));	//ADC0 P input select
            rtdf_maskl(VDC_VD_INSEL_0x5744,0xfe,(path_gnd&0x01));	//ADC0 N input select
//			VDC_PRINTF(LOGGER_INFO, "[AD0]path_signal:%x,5744:%x\n",(UINT32)path_signal,(UINT32)rtdf_inl(0x5744));
            return 0;
        }
        else
        {	// set AD1
            rtdf_maskl(VDC_VD_INSEL_0x5744,0x3f,((path_signal&0x03)<<6));	//ADC1 P input select
            rtdf_maskl(VDC_VD_INSEL_0x5744,0xfd,((path_gnd&0x01)<<1));	//ADC1 N input select
//			VDC_PRINTF(LOGGER_INFO, "[AD1]path_signal:%x,5744:%x\n",(UINT32)path_signal,(UINT32)rtdf_inl(0x5744));
            return 1;
        }
    }
    else
    {	// set AD RGB
        rtdf_maskl(VDC_VD_SCT_0x5794,0xf7,(path_signal&0x08));	//AD RGB input select
        rtdf_outl(VDC_CLAMP_SW_CTRL_0x530c, 0x0000002D);			// set clamp RGB
        rtdf_outl(VDC_CLAMP_RGB_UPDN_EN_0x5310, 0x0000003f);		// RGB clamp enable
//		VDC_PRINTF(LOGGER_INFO, "[AD RGB]path_signal:%x,5794:%x\n",(UINT32)path_signal,(UINT32)rtdf_inl(0x5794));
        return 2;
    }
}

/**
   module_vdc_TableWrite
   Video table write
   If write to VD, registers can be write auto-inc the address
   If write the other part, it just can write the only one address by 32bits

   @param <pArray>   { registers table }
   @return                  { void }

*/
static void module_vdc_TableWrite(UINT8 code *pArray)
{
    UINT16 data_len = 0, data_num = 0;
    UINT32 Addr_Index = 0, Reg_data = 0;

    do
    {
        if (pArray[0] == _TBL_END)
            return;

        data_len = pArray[0];
        data_num = 3;

//		Addr_Index = (((UINT32)pArray[1])<<8) + (UINT32)pArray[2];
        Addr_Index = pArray[1];
        Addr_Index = Addr_Index << 8;
        Addr_Index |= pArray[2];

        for ( ; data_num<data_len ; ++data_num, Addr_Index+=4 )
        {
            Reg_data = pArray[data_num];
            rtdf_outl(Addr_Index, Reg_data);
        }

        pArray += pArray[0];
    }
    while (1);
}

/**
     module_vdc_SetInput_Y
    if the input is the av ,the function is setting the y registers
    @param<SrcType> {uint8_t Y_path, uint8_t Y_Gnd_path}
    return { none }
    @ingroup dragon_video
*/
static void module_vdc_SetInput_Y(UINT8 Y_path, UINT8 Gnd_path)
{
    UINT8 ADCselect;

//	VDC_PRINTF(LOGGER_INFO, "Ypath:%x, GND:%x\n",(UINT32)Y_path,(UINT32)Gnd_path);
    ADCselect = module_vdc_SetADCPath(Y_path, Gnd_path);	// set 5744
//	VDC_PRINTF(LOGGER_INFO, "ADCselect:%x\n",(UINT32)ADCselect);
    rtdf_maskl(VDC_AD_SW_CTRL0_0x5300,0x0f,(ADCselect<<4));	// set Y switch
//	VDC_PRINTF(LOGGER_INFO, "5300:%x\n",(UINT32)rtdf_inl(0x5300));

    if ((Y_path&0x0c)==0x00)
        rtdf_maskl(VDC_CLAMP_UPDN_EN_0x5308,0x33,0x0c);
    else
        rtdf_maskl(VDC_CLAMP_UPDN_EN_0x5308,0xcc,0x03);
//	VDC_PRINTF(LOGGER_INFO, "5308:%x\n",(UINT32)rtdf_inl(0x5308));

    drv_video_vdc_SoftReset();
    rtdf_maskl(VDC_VIDEO_CONTROL1_0x5004,0xdf,0x00);	//set to AV
}

/**
     module_vdc_SetInput_YC
    if the input is the  s-video ,the function is setting the y and c path registers
    @param<SrcType> { uint8_t Y_path, uint8_t Y_Gnd_path, uint8_t C_path, uint8_t C_Gnd_path }
    @return { none }
    @ingroup dragon_video
*/
static void module_vdc_SetInput_YC(UINT8 Y_path, UINT8 Y_Gnd_path, UINT8 C_path, UINT8 C_Gnd_path)
{
    UINT8   Y_ADCselect;
    UINT8   C_ADCselect;

//	VDC_PRINTF(LOGGER_INFO, "Ypath:%x, GND:%x\n",(UINT32)Y_path,(UINT32)Y_Gnd_path);
//	VDC_PRINTF(LOGGER_INFO, "Cpath:%x, GND:%x\n",(UINT32)C_path,(UINT32)C_Gnd_path);

    Y_ADCselect = module_vdc_SetADCPath(Y_path, Y_Gnd_path);	// set 5744
    C_ADCselect = module_vdc_SetADCPath(C_path, C_Gnd_path);	// set 5744
//	VDC_PRINTF(LOGGER_INFO, "Y_ADCselect:%x\n",(UINT32)Y_ADCselect);
//	VDC_PRINTF(LOGGER_INFO, "C_ADCselect:%x\n",(UINT32)C_ADCselect);
    rtdf_maskl(VDC_AD_SW_CTRL0_0x5300,0x0f,(Y_ADCselect<<4));	// set Y switch
    rtdf_maskl(VDC_AD_SW_CTRL0_0x5300,0xf0,C_ADCselect);		// set C switch
//	VDC_PRINTF(LOGGER_INFO, "5300:%x\n",(UINT32)rtdf_inl(0x5300));

    if ((Y_path&0x0c)==0x00)
        rtdf_maskl(VDC_CLAMP_UPDN_EN_0x5308,0x33,0x0c);
    else
        rtdf_maskl(VDC_CLAMP_UPDN_EN_0x5308,0xcc,0x03);
//	VDC_PRINTF(LOGGER_INFO, "[Y]5308:%x\n",(UINT32)rtdf_inl(0x5308));

    if ((C_path&0x0c)==0x00)
        rtdf_maskl(VDC_CLAMP_UPDN_EN_0x5308,0x33,0x8c);
    else
        rtdf_maskl(VDC_CLAMP_UPDN_EN_0x5308,0xcc,0x23);
//	VDC_PRINTF(LOGGER_INFO, "[C]5308:%x\n",(UINT32)rtdf_inl(0x5308));

    drv_video_vdc_SoftReset();
    rtdf_maskl(VDC_VIDEO_CONTROL1_0x5004,0xdf,0x20);
}

#ifdef CONFIG_SCART_AUTO_SWITCH
/**
     module_vdc_SetInput_YRGB
    if the input is the  s-video ,the function is setting the y and c path registers
    @param<SrcType> { uint8_t Y_path, uint8_t Y_Gnd_path, uint8_t C_path, uint8_t C_Gnd_path }
    @return { none }
    @ingroup dragon_video
*/
static void module_vdc_SetInput_YRGB(UINT8 Y_path, UINT8 Y_Gnd_path, UINT8 RGB_path, UINT8 RGB_Gnd_path)
{
    UINT8   Y_ADCselect;
    UINT8   RGB_ADCselect;

//	VDC_PRINTF(LOGGER_INFO, "Ypath:%x, GND:%x\n",(UINT32)Y_path,(UINT32)Y_Gnd_path);
//	VDC_PRINTF(LOGGER_INFO, "RGBpath:%x, GND:%x\n",(UINT32)RGB_path,(UINT32)RGB_Gnd_path);

    Y_ADCselect = module_vdc_SetADCPath(Y_path, Y_Gnd_path);	// set 5744
    RGB_ADCselect = module_vdc_SetADCPath(RGB_path, RGB_Gnd_path);
//	VDC_PRINTF(LOGGER_INFO, "Y_ADCselect:%x\n",(UINT32)Y_ADCselect);
//	VDC_PRINTF(LOGGER_INFO, "RGB_ADCselect:%x\n",(UINT32)RGB_ADCselect);

    rtdf_maskl(VDC_AD_SW_CTRL0_0x5300,0x0f,(Y_ADCselect<<4));	// set Y switch

    if (RGB_path<=VIDEO_INPUT12)
    {	// is C setting
        rtdf_maskl(VDC_AD_SW_CTRL0_0x5300,0xf0,RGB_ADCselect);	// set C switch
        if ((RGB_path&0x0c)==0x00)
            rtdf_maskl(VDC_CLAMP_UPDN_EN_0x5308,0x33,0x8c);
        else
            rtdf_maskl(VDC_CLAMP_UPDN_EN_0x5308,0xcc,0x23);
//		VDC_PRINTF(LOGGER_INFO, "[C]5308:%x\n",(UINT32)rtdf_inl(0x5308));
    }
    else
    {
        if (RGB_path == VIDEO_RGB0)
            rtd_maskl(DUMMY_0x0438, 0xFFFFFFF8, 0x00000007);	// enable RGB0 clamp up/dn
        else
            rtd_maskl(DUMMY_0x0438, 0xFFFFFE3F, 0x000001C0);	// enable RGB1 clamp up/dn

        rtdf_maskl(VDC_AD_SW_CTRL0_0x5300,0xf0,0x02);		// set Red switch
        rtdf_outl(VDC_AD_SW_CTRL1_0x5304, 0x00000009);		// set Blue/Green switch
        rtdf_outl(VDC_CLAMP_RGB_UPDN_EN_0x5310,0x3f);	// enable RGB clamp up/dn
    }
    VDC_PRINTF(LOGGER_INFO, "5300:%x\n",(UINT32)rtdf_inl(0x5300));
//	VDC_PRINTF(LOGGER_INFO, "5304:%x\n",(UINT32)rtdf_inl(0x5304));

    if ((Y_path&0x0c)==0x00)
        rtdf_maskl(VDC_CLAMP_UPDN_EN_0x5308,0x33,0x0c);
    else
        rtdf_maskl(VDC_CLAMP_UPDN_EN_0x5308,0xcc,0x03);
//	VDC_PRINTF(LOGGER_INFO, "[Y]5308:%x\n",(UINT32)rtdf_inl(0x5308));

    drv_video_vdc_SoftReset();
    rtdf_maskl(VDC_VIDEO_CONTROL1_0x5004,0xffffffdf,0x00);
}
#endif

/**
   module_vdc_FrontDigitalFilterEnable
   Video funtion set, extern

   @param <void>       { void }
   @return                  { void }

*/
static void module_vdc_FrontDigitalFilterEnable(VDC_VIDEO_FrontDigitalFilter mode)
{
    rtdf_maskl(VDC_INOUT_CTRL_0x5018, 0x3f, ((mode&0x03)<<6));
}

/**
   module_vdc_PathSelect
   Video input path set, no extern

   @param <src>        { Select input signal}
   @return                  { void }

*/
static void module_vdc_PathSelect(void* stPath)
{
    rtdf_maskl(VDC_INOUT_CTRL_0x5018,0xF0,0x00);
    VDC_PRINTF(LOGGER_DEBUG,"[PathSelect] %x\n",(UINT32)vdc_InputSrc);

    switch (vdc_InputSrc)
    {
    case  VDC_SOURCE_TV:
        if ((((TV_VDC_PATH*)stPath)->VideoIn)== TV_INPUT)
        {
            VDC_PRINTF(LOGGER_DEBUG,"path is ifd.\n");
            rtdf_maskl(VDC_VD_ADC2X_POW_0x57a8, 0x00, 0x03); 	// adc power
            rtdf_maskl(VDC_INOUT_CTRL_0x5018, 0x00, 0x0b);		// vd clamp enable
            drv_video_vdc_SoftReset();
        }
        else
        {
            rtdf_maskl(VDC_AGC_GATE_TH_SWAP_0x508c,0x7f,0x80);
            module_vdc_SetInput_Y(((TV_VDC_PATH*)stPath)->VideoIn, ((TV_VDC_PATH*)stPath)->GroundIn);
        }
        drv_video_vdc_AvOutMode(((TV_VDC_PATH*)stPath)->AVOutPath,VDC_AVOUT_SOURCE_FOR_TV);
        rtdf_outl(VDC_VDAC_SIB_0x57c0,VDC_AVOUT_SIB_FOR_TV);
        drvif_ifd_set_videopath_output_gain(IFD_VIDEO_OUTPUT_GAIN);
        break;
    case VDC_SOURCE_SV:
        module_vdc_SetInput_YC(((SV_VDC_PATH*)stPath)->Y_VideoIn, ((SV_VDC_PATH*)stPath)->Y_GroundIn,
                               ((SV_VDC_PATH*)stPath)->C_VideoIn, ((SV_VDC_PATH*)stPath)->C_GroundIn);
        drv_video_vdc_AvOutMode(((SV_VDC_PATH*)stPath)->AVOutPath,VDC_AVOUT_YC_WITH_FIL_WITH_AGC);
        rtdf_outl(VDC_VDAC_SIB_0x57c0,VDC_AVOUT_SIB);
        drvif_ifd_set_videopath_output_gain(0);
        break;
    case VDC_SOURCE_CVBS:
        module_vdc_SetInput_Y(((AV_VDC_PATH*)stPath)->VideoIn, ((AV_VDC_PATH*)stPath)->GroundIn);
        drv_video_vdc_AvOutMode(((AV_VDC_PATH*)stPath)->AVOutPath,VDC_AVOUT_CVBS_WITH_FIL_WITH_AGC);
        rtdf_outl(VDC_VDAC_SIB_0x57c0,VDC_AVOUT_SIB);
        drvif_ifd_set_videopath_output_gain(0);
        break;
#ifdef CONFIG_SCART_AUTO_SWITCH
    case VDC_SOURCE_HALF_SCART:
//		case VDC_SOURCE_SCART_AV:
//		case VDC_SOURCE_SCART_SV:
        module_vdc_SetInput_YRGB(((SCART_VDC_PATH*)stPath)->Y_VideoIn, ((SCART_VDC_PATH*)stPath)->Y_GroundIn,
                                 ((SCART_VDC_PATH*)stPath)->RGB_VideoIn, ((SCART_VDC_PATH*)stPath)->RGB_GroundIn);
        drv_video_vdc_AvOutMode(((SCART_VDC_PATH*)stPath)->AVOutPath,VDC_AVOUT_CVBS_WITH_FIL_WITH_AGC);
        rtdf_maskl(VDC_VDAC_AVOUTSEL_0x57b0,0xf0,0x04);// AVOUT1 set to VIDEO_INPUT02
        SCART_Type = _SCART_AV;
        rtdf_outl(VDC_VDAC_SIB_0x57c0,VDC_AVOUT_SIB);
        drvif_ifd_set_videopath_output_gain(0);
        break;
    case VDC_SOURCE_FULL_SCART:
        drv_video_FullSCART_func_initial();
        module_vdc_SetInput_YRGB(((SCART_VDC_PATH*)stPath)->Y_VideoIn, ((SCART_VDC_PATH*)stPath)->Y_GroundIn,
                                 ((SCART_VDC_PATH*)stPath)->RGB_VideoIn, ((SCART_VDC_PATH*)stPath)->RGB_GroundIn);
//			drv_video_vdc_AvOutMode(((SCART_VDC_PATH*)stPath)->AVOutPath,VDC_AVOUT_DAC);
//////////// For CANAL+, don't moved //////////////////////////////////////////////////////////////////////////
        rtd_outl(CLAMPING_THR_MAX_0x5c90, 0x000037a0);
        rtd_outl(CLAMPING_THR_MIN_0x5c94, 0x000037a0);
        drv_video_vdc_AvOutMode(((TV_VDC_PATH*)stPath)->AVOutPath,VDC_AVOUT_NO_CLAMP_IFD_DATA);	// for CANAL+
//////////// For CANAL+, don't moved //////////////////////////////////////////////////////////////////////////
        rtdf_maskl(VDC_VDAC_AVOUTSEL_0x57b0,0x0f,0x67);// AVOUT2 set to VIDEO_INPUT03
        if ((rtdf_inl(VDC_FSW_INPUT_STATUS_0x533c)&0x0f)>=0x04)
        {
            drv_video_SCART_RGB_set(_ON);
            SCART_Type = _SCART_RGB;
        }
        else
        {
            SCART_Type = _SCART_AV;
        }
        rtdf_outl(VDC_VDAC_SIB_0x57c0,VDC_AVOUT_SIB);
        drvif_ifd_set_videopath_output_gain(0);
        break;
#endif
    default:
        rtdf_outl(VDC_VDAC_SIB_0x57c0, VDC_AVOUT_SIB);
        drvif_ifd_set_videopath_output_gain(0);
        // Get error vdc number
        VDC_PRINTF(LOGGER_ERROR, "Get error vdc_InputSrc in module_vdc_PathSelect!!\n");
        break;
    }
}

static void module_vdc_cresample_lpf_sel(_VDC_VIDEO_Cresample_Lpf_MODE MODE)
{
    if (MODE == VDC_VIDEO_Cresample_Lpf_ENABLE)
        rtdf_maskl(VDC_CHROMA_LOOPFILTER_STATE_0x50a0, 0xcf, 0x10);
    else
        rtdf_maskl(VDC_CHROMA_LOOPFILTER_STATE_0x50a0, 0xcf, 0x00);
}

/**
	  Video1DSetting
	   Video 1D setting

	   @param                      { void }
	   @return                      { void }

*/
static void module_vdc_1DSet(void)
{
    UINT8 VD_mode = VDC_MODE_NTSC;

    if (rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008)&_No_Sig)
        VD_mode = VDC_MODE_NO_SIGNAL;
    else
        VD_mode = drv_video_vdc_ReadMode(VDC_SET);

    if (YcSeparationStatus == VDC_YCSEPARATE_1D)
    {
        return;	// 1D is already setting
    }

    // reset SECAM setting
    drv_video_vdc_YcDelay(0);
    rtdf_outl(VDC_SECAM_CTRL_0x5340, 0x82);
    rtdf_outl(VDC_Y_GAIN_TABLE_CTRL_0x535c, 0x08);		//reset Y threshold
    rtdf_maskl(VDC_VD_BASIC_REGISTER_ENABLE_0x51d4, 0xdf, 0x00);
    // reset NTSC443 setting
    rtdf_maskl(VDC_VIDEO_CONTROL0_0x5000, 0xf1, 0x00);
    module_vdc_FrontDigitalFilterEnable(VDC_FDF_BYPASS);

    switch (VD_mode)
    {
    default:
    case VDC_MODE_NTSC:
        module_vdc_cresample_lpf_sel(VDC_VIDEO_Cresample_Lpf_DISABLE);
        module_vdc_TableWrite(VDC_1D_SET1);
        module_vdc_TableWrite(VDC_DCTI_SET2);
        drv_video_vdc_YcDelay(1); // ives 20090826, for Y/U/V match
        rtdf_outl(VDC_CHROMA_DELAY_CTRL_0x50fc, 0x20);//novia091111
        break;
    case VDC_MODE_NO_SIGNAL:
    case VDC_MODE_NTSC443:
    case VDC_MODE_SECAM:
    case VDC_MODE_NTSC50:
        module_vdc_cresample_lpf_sel(VDC_VIDEO_Cresample_Lpf_DISABLE);
        module_vdc_TableWrite(VDC_1D_SET1);
        module_vdc_TableWrite(VDC_DCTI_SET1);
        if (VD_mode == VDC_MODE_SECAM)
        {
//			rtdf_outl(VDC_SECAM_CTRL_0x5340, 0xc2);
            rtdf_outl(VDC_SECAM_CTRL_0x5340, 0xc0);	//novia091111
//				drv_video_vdc_YcDelay(3);	//novia091111
            rtdf_outl(VDC_Y_GAIN_TABLE_CTRL_0x535c, 0x00);		//reset Y threshold
            rtdf_outl(VDC_YC_BW_CTRL_0x52a8, 0x07);
            rtdf_outl(VDC_CHROMA_DELAY_CTRL_0x50fc, 0x20);	//novia091111
        }
        else if (VD_mode == VDC_MODE_NTSC443)
        {
            rtdf_maskl(VDC_VD_BASIC_REGISTER_ENABLE_0x51d4, 0xdf, 0x20);
            rtdf_maskl(VDC_VIDEO_CONTROL0_0x5000, 0xf1, 0x0c);
            drv_video_vdc_YcDelay(1); //novia091111
            rtdf_outl(VDC_CHROMA_DELAY_CTRL_0x50fc, 0x20);	//novia091111
        }
        break;
    case VDC_MODE_PALI:
        module_vdc_cresample_lpf_sel(VDC_VIDEO_Cresample_Lpf_ENABLE);
        module_vdc_TableWrite(DRAGON_1D_SET2);
        module_vdc_TableWrite(VDC_DCTI_SET2);
        drv_video_vdc_YcDelay(1); // ives 20090826, for Y/U/V match
        rtdf_outl(VDC_CHROMA_DELAY_CTRL_0x50fc, 0x10);	//novia091111
        break;
    case VDC_MODE_PALM:
    case VDC_MODE_PALN:
        module_vdc_cresample_lpf_sel(VDC_VIDEO_Cresample_Lpf_ENABLE);
        module_vdc_TableWrite(DRAGON_1D_SET2);
        module_vdc_TableWrite(VDC_DCTI_SET1);
        rtdf_outl(VDC_CHROMA_DELAY_CTRL_0x50fc, 0x10);//novia091111
        break;
    case VDC_MODE_PAL60:
        module_vdc_cresample_lpf_sel(VDC_VIDEO_Cresample_Lpf_ENABLE);
        module_vdc_TableWrite(DRAGON_1D_SET3);
        module_vdc_TableWrite(VDC_DCTI_SET1);
        break;
    }
    YcSeparationStatus = VDC_YCSEPARATE_1D;
    VDC_PRINTF(LOGGER_DEBUG,"[vdc] 1D Set : %x\n",(UINT32)VD_mode);
}

static void module_vdc_2DSet(void)
{
    UINT8 VD_mode = VDC_MODE_NTSC;

    if (rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008)&_No_Sig)
        VD_mode = VDC_MODE_NO_SIGNAL;
    else
        VD_mode = drv_video_vdc_ReadMode(VDC_SET);

    //VDC_PRINTF(LOGGER_DEBUG,"[vdc] 2D checked : %x\n",(UINT32)YcSeparationStatus);
    if (YcSeparationStatus == VDC_YCSEPARATE_2D)
    {
        return;	// 2D is already setting
    }

    module_vdc_FrontDigitalFilterEnable(VDC_FDF_BYPASS);
    // reset SECAM setting
    rtdf_outl(VDC_SECAM_CTRL_0x5340, 0x82);
    drv_video_vdc_YcDelay(0);
    rtdf_outl(VDC_Y_GAIN_TABLE_CTRL_0x535c, 0x08);  //reset Y threshold
    rtdf_outl(VDC_YC_SEP_CONTROL_0x5280, 0x00000000);//Tim 0620
    rtdf_maskl(VDC_VD_BASIC_REGISTER_ENABLE_0x51d4, 0xdf, 0x00);
    rtdf_maskl(VDC_VIDEO_CONTROL0_0x5000, 0xf1, 0x00);

    switch (VD_mode)
    {
    default:
    case VDC_MODE_NTSC:
    case VDC_MODE_NO_SIGNAL:
        module_vdc_FrontDigitalFilterEnable(VDC_FDF_60MHZ);
        module_vdc_cresample_lpf_sel(VDC_VIDEO_Cresample_Lpf_DISABLE);
        module_vdc_TableWrite(DRAGON_2D_SET1);
        drv_video_vdc_YcDelay(1); // ives 20090826, for Y/U/V match
        switch (vdc_InputSrc)
        {
        case  VDC_SOURCE_CVBS:
            module_vdc_TableWrite(VDC_DCTI_SET2);
            break;
        case  VDC_SOURCE_TV:
            module_vdc_TableWrite(VDC_DCTI_SET3);
            break;
        }
        break;
    case VDC_MODE_NTSC443:
        module_vdc_cresample_lpf_sel(VDC_VIDEO_Cresample_Lpf_DISABLE);
        module_vdc_TableWrite(DRAGON_2D_SET2);
        module_vdc_TableWrite(VDC_DCTI_SET1);
        rtdf_maskl(VDC_VD_BASIC_REGISTER_ENABLE_0x51d4, 0xdf, 0x20);
        rtdf_maskl(VDC_VIDEO_CONTROL0_0x5000, 0xf1, 0x0a);
        break;
    case VDC_MODE_PALM:
        module_vdc_cresample_lpf_sel(VDC_VIDEO_Cresample_Lpf_ENABLE);
        module_vdc_TableWrite(DRAGON_2D_SET2);
        module_vdc_TableWrite(VDC_DCTI_SET1);
        break;
    case VDC_MODE_PALN:
        module_vdc_cresample_lpf_sel(VDC_VIDEO_Cresample_Lpf_ENABLE);
        module_vdc_TableWrite(DRAGON_2D_SET2);
        module_vdc_TableWrite(VDC_DCTI_SET1);
        break;
    case VDC_MODE_PALI:
        module_vdc_cresample_lpf_sel(VDC_VIDEO_Cresample_Lpf_ENABLE);
        module_vdc_TableWrite(DRAGON_2D_SET2);
        drv_video_vdc_YcDelay(1); // ives 20090826, for Y/U/V match
        switch (vdc_InputSrc)
        {
        case  VDC_SOURCE_CVBS:
            module_vdc_TableWrite(VDC_DCTI_SET2);
            break;
        case  VDC_SOURCE_TV:
            module_vdc_TableWrite(VDC_DCTI_SET3);
            break;
        }
        break;
    case VDC_MODE_SECAM:
        module_vdc_TableWrite(VDC_DCTI_SET1);
        module_vdc_cresample_lpf_sel(VDC_VIDEO_Cresample_Lpf_DISABLE);
//			rtdf_outl(VDC_SECAM_CTRL_0x5340, 0xc2);
        rtdf_outl(VDC_SECAM_CTRL_0x5340, 0xc0);	//novia091111
        drv_video_vdc_YcDelay(3);
        rtdf_outl(VDC_Y_GAIN_TABLE_CTRL_0x535c, 0x00);  //reset Y threshold
        rtdf_outl(VDC_YC_BW_CTRL_0x52a8, 0x07);
        break;
    case VDC_MODE_NTSC50:
        module_vdc_cresample_lpf_sel(VDC_VIDEO_Cresample_Lpf_DISABLE);
        module_vdc_TableWrite(VDC_1D_SET1);
        module_vdc_TableWrite(VDC_DCTI_SET1);
        break;
    case VDC_MODE_PAL60:
        module_vdc_cresample_lpf_sel(VDC_VIDEO_Cresample_Lpf_ENABLE);
        rtdf_maskl(VDC_INOUT_CTRL_0x5018, ~_BIT6, _ZERO);
        module_vdc_TableWrite(DRAGON_2D_SET2);
        module_vdc_TableWrite(VDC_DCTI_SET1);
        rtdf_maskl(VDC_VD_BASIC_REGISTER_ENABLE_0x51d4, 0xdf, 0x20);
        rtdf_maskl(VDC_VIDEO_CONTROL0_0x5000, 0xf1, 0x0e);
        break;
    }
    YcSeparationStatus = VDC_YCSEPARATE_2D;
    VDC_PRINTF(LOGGER_DEBUG,"[vdc] 2D Set : %x\n",(UINT32)VD_mode);
}

void drv_video_vdc_YcSeparation(void)
{
    if (vdc_factory_mod_enable==1)
        return ;

//	VDC_PRINTF(LOGGER_DEBUG,"[vdc] YC Separation\n");
    switch (vdc_InputSrc)
    {
    case VDC_SOURCE_SV:
//		case VDC_SOURCE_SCART_SV:
        module_vdc_1DSet();
        drvif_module_vdc_XNREnable(VDC_XNR_DISABLE);
        break;
    case  VDC_SOURCE_TV:
    case  VDC_SOURCE_CVBS:
        drvif_module_vdc_XNREnable(VDC_XNR_ENABLE);
        if ((rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008)&_VD_AllLock)==_VD_AllLock)
        {
            module_vdc_2DSet();
        }
        else
        {
            module_vdc_1DSet();
        }
        break;
#ifdef CONFIG_SCART_AUTO_SWITCH
    case VDC_SOURCE_FULL_SCART:
    case VDC_SOURCE_HALF_SCART:
//		case VDC_SOURCE_SCART_AV:
        VDC_PRINTF(LOGGER_DEBUG,"[vdc]SCART_Type : %x\n",(UINT32)SCART_Type);
        if (SCART_Type != _SCART_AV)
        {	// Is SCART_SV or SCART_RGB
//				VDC_PRINTF(LOGGER_ERROR, "SCART SV/RGB set\n");
            module_vdc_1DSet();
        }
        else
        {
            if ((rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008)&_VD_AllLock)==_VD_AllLock)
            {
                VDC_PRINTF(LOGGER_ERROR, "SCART AV set\n");
                module_vdc_2DSet();
            }
            else
            {
                VDC_PRINTF(LOGGER_ERROR, "SCART AV no burst set\n");
                module_vdc_1DSet();
            }
        }
        break;
#endif
    default:
        // Get error vdc number
        VDC_PRINTF(LOGGER_ERROR, "[1]Get error vdc_InputSrc in drv_video_detectmode!!\n");
        break;
    }
}
#ifdef _VIDEO_TV_SUPPORT

void drv_video_reset_HPLL_default(void)
{
    rtdf_outl(VDC_H_LOOP_MAXSTATE_0x50b0, 0xCB);
}
#endif
/**
   drvif_module_vdc_AvOutMode
   select the av out mode

   @param <uselect> {}
   @return                 { void }

*/

void drv_video_vdc_AvOutMode(_VDC_VIDEO_AVOUT_PATH  uselect, _VDC_VIDEO_AVOUT_MODE mode)
{
    if (mode >= VDC_AVOUT_BYPASS_VIN00P)	// bypass mode
    {
        if (uselect == VDC_AVOUT1)
            rtdf_maskl(VDC_VDAC_AVOUTSEL_0x57b0,0xf0,(mode&0x07));
        else		// VDC_AVOUT2
            rtdf_maskl(VDC_VDAC_AVOUTSEL_0x57b0,0x0f,((mode&0x07)<<4));
    }
    else
    {
        rtdf_maskl(VDC_AVOUT_FIFO_STATUS_0x501c,0x1f,((mode&0x07)<<5));

        if (uselect==VDC_AVOUT1)
            rtdf_maskl(VDC_VDAC_AVOUTSEL_0x57b0,0xf0,0x07);
        else
            rtdf_maskl(VDC_VDAC_AVOUTSEL_0x57b0,0x0f,0x70);
    }
}

/**
   drvif_module_vdc_SoftReset
   Reset video parameter

   @param <void>       { void }
   @return                  { void }

*/
void drv_video_vdc_SoftReset(void)
{
    rtdf_maskb(SYS_SRST_0_0x2ca0,~_BIT3, _ZERO);
    fw_timer_event_DelayXms(10);
    rtdf_maskb(SYS_SRST_0_0x2ca0,~_BIT3, _BIT3);
}

/*UINT8 drvif_module_vdc_Read_Hstate(void)
{
    return (rtdf_inl(VDC_H_LOOP_MAXSTATE_0x50b0)&0x07);
}*/

void drvif_module_vdc_Set_Hstate(_VDC_VIDEO_FIXHSTATE_MODE value)
{
    if (vdc_factory_mod_enable==_ON)
        return ;

    switch (value)
    {
    default:
    case VDC_HSTATE_RESET:  //reset the h state
        rtdf_outl(VDC_H_LOOP_MAXSTATE_0x50b0, 0xcd);
        break;
    case VDC_NOT_FIX_HSTATE0: // not fix hstate
    case VDC_NOT_FIX_HSTATE1:
    case VDC_NOT_FIX_HSTATE2:
    case VDC_NOT_FIX_HSTATE3:
    case VDC_NOT_FIX_HSTATE4:
    case VDC_NOT_FIX_HSTATE5:
        value = (value-VDC_NOT_FIX_HSTATE0);
        rtdf_maskl(VDC_H_LOOP_MAXSTATE_0x50b0, ~_BIT5,_ZERO);
        rtdf_maskl(VDC_H_LOOP_MAXSTATE_0x50b0, 0xfffffff8,value);
        break;
    case VDC_FIX_HSTATE0: //fix hstate
    case VDC_FIX_HSTATE1:
    case VDC_FIX_HSTATE2:
    case VDC_FIX_HSTATE3:
    case VDC_FIX_HSTATE4:
    case VDC_FIX_HSTATE5:
        value = (value-VDC_FIX_HSTATE0);
        rtdf_maskl(VDC_H_LOOP_MAXSTATE_0x50b0, ~_BIT5,_BIT5);
        rtdf_maskl(VDC_H_LOOP_MAXSTATE_0x50b0, 0xfffffff8,value);
        break;
    }
}



/**
   drvif_module_vdc_SetCstateToDefault
   Set Cstate to Default (= 3)

 This function need to execute when source is changed.

   @param <void>       { void }
   @return                  { void }
*/
void drvif_module_vdc_SetCstateToDefault(void)
{
//leo 20100126 for NTSC4.43 Cstate default set to 5
#if defined (CONFIG_PROJECT_TV010_1) || defined (CONFIG_PROJECT_TV015_2)
    UINT8 VD_mode = 0;
    VD_mode = drv_video_vdc_ReadMode(VDC_DETECT);
    if (VD_mode == VDC_MODE_NTSC443)
    {
//		if((rtdf_inl(VDC_CHROMA_LOOPFILTER_STATE_VADDR)&0x0e) != 0x05)
        if (drvif_module_vdc_Read_Cstate() != VDC_FIX_CSTATE2)
        {
//			rtdf_maskl(VDC_CHROMA_LOOPFILTER_STATE_VADDR, 0xf0, 0x05);
            drvif_module_vdc_Set_Cstate(VDC_FIX_CSTATE2);
            RTD_Log(LOGGER_INFO,"[cstate] NTSC4.43 default \n");
        }
    }
    else
    {
//		if((rtdf_inl(VDC_CHROMA_LOOPFILTER_STATE_VADDR)&0x0e) != 0x06)
        if (drvif_module_vdc_Read_Cstate() != VDC_CSTATE_DEFAULT)
        {
//			rtdf_maskl(VDC_CHROMA_LOOPFILTER_STATE_VADDR, 0xf0, 0x06);
            drvif_module_vdc_Set_Cstate(VDC_CSTATE_DEFAULT);
            RTD_Log(LOGGER_INFO,"[cstate] default \n");
        }
    }
#else
//	if((rtdf_inl(VDC_CHROMA_LOOPFILTER_STATE_VADDR)&0x0e) != 0x06)	// c state is not 3
    if (drvif_module_vdc_Read_Cstate() != VDC_CSTATE_DEFAULT)	// c state is not 3
    {
//		rtdf_maskl(VDC_CHROMA_LOOPFILTER_STATE_VADDR, 0xf0, 0x06);
        drvif_module_vdc_Set_Cstate(VDC_CSTATE_DEFAULT);
        RTD_Log(LOGGER_INFO,"[cstate] default \n");
    }
#endif
}


UINT8 drvif_module_vdc_Read_Cstate(void)
{
    return (rtdf_inl(VDC_CHROMA_LOOPFILTER_STATE_0x50a0)&0x0f);
}

void drvif_module_vdc_Set_Cstate(_VDC_VIDEO_FIXCSTATE_MODE value)
{
    if (vdc_factory_mod_enable==_ON)
        return ;

#if 0
    switch (value)
    {
    default:
    case VDC_CSTATE_RESET:  //reset the c state
        drvif_module_vdc_SetCstateToDefault();
        break;
    case VDC_NOT_FIX_CSTATE0: // not fix cstate
    case VDC_NOT_FIX_CSTATE1:
    case VDC_NOT_FIX_CSTATE2:
    case VDC_NOT_FIX_CSTATE3:
    case VDC_NOT_FIX_CSTATE4:
    case VDC_NOT_FIX_CSTATE5:
        value = (value-VDC_NOT_FIX_CSTATE0)<<1;
        rtdf_maskl(VDC_CHROMA_LOOPFILTER_STATE_VADDR, ~_BIT0,_ZERO);
        rtdf_maskl(VDC_CHROMA_LOOPFILTER_STATE_VADDR, 0xfffffff1,value);
        break;
    case VDC_FIX_CSTATE0: //fix cstate
    case VDC_FIX_CSTATE1:
    case VDC_FIX_CSTATE2:
    case VDC_FIX_CSTATE3:
    case VDC_FIX_CSTATE4:
    case VDC_FIX_CSTATE5:
        value = (value-VDC_FIX_CSTATE0)<<1;
        rtdf_maskl(VDC_CHROMA_LOOPFILTER_STATE_VADDR, ~_BIT0,_BIT0);
        rtdf_maskl(VDC_CHROMA_LOOPFILTER_STATE_VADDR, 0xfffffff1,value);
        break;
    }
#else
    if (value == VDC_CSTATE_RESET)
    {
        drvif_module_vdc_SetCstateToDefault();
    }
    else
    {
        rtdf_maskl(VDC_CHROMA_LOOPFILTER_STATE_0x50a0,0xf0,(value&0x0f));
    }
#endif
}
#ifdef _VIDEO_TV_SUPPORT

UINT8 drvif_module_vdc_AVG_BURST_MAG_STATUS(void)
{
    return (UINT8)rtdf_inl(VDC_AVG_BURST_MAG_STATUS_0x523c);
}
#endif
/**
   module_vdc_YcDelay
   Set contrast

   @param

   @return                  { void }

*/
void drv_video_vdc_YcDelay(UINT8 value)
{
    if ((rtdf_inl(VDC_FSW_CTRL2_0x531c)&_BIT3) == _BIT3)	//when set to 444 output (~Sparrow), value need to decrease 1
        value--;
    rtdf_maskl(VDC_OUTPUT_CONTROL_0x503c, 0xf0, (value&0x0f));
}

/**
   drv_module_vdc_SetContrast
   Set contrast

   @param <ucValue>  { Set contrast }
               <num>       { contrast number 1,2      }
                                { =1; adjust contrast       }
                                { =2; adjust contrast2 (component/SCART_RGB) }
   @return                  { void }

*/
void drv_video_vdc_SetContrast(UINT8 ucValue)
{
    rtdf_outl(VDC_LUMA_CONTRAST_ADJ_0x5020,ucValue);
}

/**
   drv_module_vdc_SetBrightness
   Set contrast

   @param <ucValue>  { Set contrast }
               <num>       { contrast number 1,2      }
                                { =1; adjust contrast       }
                                { =2; adjust contrast2 (component/SCART_RGB) }
   @return                  { void }

*/
void drv_video_vdc_SetBrightness(UINT8 ucValue)
{
    rtdf_outl(VDC_LUMA_BRIGHTNESS_ADJ_0x5024,ucValue);
}

/**
  drv_module_vdc_SetHue
   Set hue
   @param <ucValue>  { Set hue }
  @return                  { void }
*/

void drv_video_vdc_SetHue(UINT8 ucValue)
{
    rtdf_outl(VDC_CHROMA_HUE_PHASE_ADJ_0x502c,ucValue);
}
#ifdef _VIDEO_TV_SUPPORT
void drv_module_vdc_Set_CKill_Mode(VDC_CKill_MODE ucMode)
{
    rtdf_maskl(VDC_CHROMA_KILL_0x5094, 0x3f, ucMode);
}
#endif
/*
   drv_module_vdc_SetSaturation
   Set saturation

   @param <ucValue>  { Set saturation }
               <num>       { saturation number 0,1   }
                                { =0; adjust saturation    }
                                { =1; adjust saturation2 (component/SCART_RGB) }
   @return                  { void }

*/
void drv_video_vdc_SetSaturation(UINT8 ucValue)
{
    rtdf_outl(VDC_CHROMA_SATURATION_ADJ_0x5028,ucValue);
}

void drv_video_vdc_AVoutEnable(_VDC_VIDEO_AVOUT_ENABLE on_off)
{
    if (on_off == VDC_AVOUT_ENALBE)
        rtdf_maskl(VDC_VDAC_ABIQ1_0x57bc,0x30, 0xc0);
    else
        rtdf_maskl(VDC_VDAC_ABIQ1_0x57bc,0x30, 0x00);
}

void drv_video_vdc_user_ckill_mode(_VDC_VIDEO_USER_CKILL_MODE mode)
{
    if (vdc_factory_mod_enable==1)
        return ;

    switch (mode)
    {
    case   VDC_USER_CKILL_AUTO_HW:
        rtdf_maskl(VDC_CHROMA_KILL_0x5094, 0x3f,0x00);
        break;
    case   VDC_USER_CKILL_FORCE_ON:
        rtdf_maskl(VDC_CHROMA_KILL_0x5094, 0x3f,0x40);
        break;
    case   VDC_USER_CKILL_FORCE_OFF:
        rtdf_maskl(VDC_CHROMA_KILL_0x5094, 0x3f,0x80);
        break;
    }
}

/**
   drvif_module_vdc_PowerCtrl
   Power on/off vdc and tv(ifd) module

   @param <on_off>   { _ON/_OFF }
   @return                  { void }
*/
void drv_video_vdc_Dcti_Enable(_VDC_DCTI_ENABLE on_off)
{
    return;
    if (on_off == DCTI_ENABLE)
        rtdf_maskl(VDC_DCTI_FILTER_EN_0x54e8, ~_BIT0, _BIT0);
    else
        rtdf_maskl(VDC_DCTI_FILTER_EN_0x54e8, ~_BIT0, _ZERO);
}

/*
   drv_module_vdc_SetCKillValue
   drv_module_vdc_ReadCKillValue
   Set / read saturation

   @param <ucValue>  { Set chroma kill value }
   @return                   { chroma kill value }

*/
void drv_video_vdc_SetCKillValue(UINT8 ucValue)
{
    rtdf_maskl(VDC_CHROMA_KILL_0x5094,0xf0,(ucValue&0x0f));
}

/**
     module_vdc_Set_DCTI_Setting
    @param<SrcType> {}
    return { none }
    @ingroup dragon_video
*/
static UINT8  code VDC_DCTI_3db_1level[]=
{
    13, HIGH_BYTE(VDC_DCTI_FILTER_COEF_B1_MSB_0x54c0), LOW_BYTE(VDC_DCTI_FILTER_COEF_B1_MSB_0x54c0),
    0x01,0x6a,0x0e,0x95,0x00,0x00,0x0f,0xca,0x00,0x00,
    _TBL_END
};

static UINT8  code VDC_DCTI_3db_2level[]=
{
    13, HIGH_BYTE(VDC_DCTI_FILTER_COEF_B1_MSB_0x54c0), LOW_BYTE(VDC_DCTI_FILTER_COEF_B1_MSB_0x54c0),
    0x00,0xde,0x0e,0x44,0x00,0xdc,0x0f,0x67,0x00,0x5f,
    _TBL_END
};

static UINT8  code VDC_DCTI_9db_1level[]=
{
    13, HIGH_BYTE(VDC_DCTI_FILTER_COEF_B1_MSB_0x54c0), LOW_BYTE(VDC_DCTI_FILTER_COEF_B1_MSB_0x54c0),
    0x01,0x88,0x0e,0x76,0x00,0x00,0x00,0xd9,0x00,0x00,
    _TBL_END
};

static UINT8  code VDC_DCTI_9db_2level[]=
{
    13, HIGH_BYTE(VDC_DCTI_FILTER_COEF_B1_MSB_0x54c0), LOW_BYTE(VDC_DCTI_FILTER_COEF_B1_MSB_0x54c0),
    0x00,0xd1,0x0e,0x5e,0x00,0xd1,0x00,0x83,0x00,0x5d,
    _TBL_END
};

static UINT8  code VDC_DCTI_6db_1level[]=
{
    13, HIGH_BYTE(VDC_DCTI_FILTER_COEF_B1_MSB_0x54c0), LOW_BYTE(VDC_DCTI_FILTER_COEF_B1_MSB_0x54c0),
    0x01,0x74,0x0e,0x8a,0x00,0x00,0x0f,0x80,0x00,0x00,
    _TBL_END
};

static UINT8  code VDC_DCTI_6db_2level[]=
{
    13, HIGH_BYTE(VDC_DCTI_FILTER_COEF_B1_MSB_0x54c0), LOW_BYTE(VDC_DCTI_FILTER_COEF_B1_MSB_0x54c0),
    0x00,0xd3,0x0e,0x5a,0x00,0xd3,0x00,0x00,0x00,0x59,
    _TBL_END
};

static UINT8  code VDC_DCTI_12db_1level[]=
{
    13, HIGH_BYTE(VDC_DCTI_FILTER_COEF_B1_MSB_0x54c0), LOW_BYTE(VDC_DCTI_FILTER_COEF_B1_MSB_0x54c0),
    0x01,0x00,0x06,0x00,0x01,0x00,0x02,0xa0,0x03,0x7d,
    _TBL_END
};

static UINT8  code VDC_DCTI_12db_2level[]=
{
    13, HIGH_BYTE(VDC_DCTI_FILTER_COEF_B1_MSB_0x54c0), LOW_BYTE(VDC_DCTI_FILTER_COEF_B1_MSB_0x54c0),
    0x00,0xec,0x0e,0x27,0x00,0xec,0x01,0x01,0x00,0x6e,
    _TBL_END
};

void drv_video_vdc_Set_DCTI_Setting(UINT8 level)
{
    vdc_dcti_talbe_value = level;
//	VDC_PRINTF(LOGGER_DEBUG,"[DCTI] set:%x\n",(UINT32)level);

    switch (level)
    {
    case 0:		// disable
    default:
        break;
    case 1:
        module_vdc_TableWrite(VDC_DCTI_3db_1level);
        break;
    case 2:
        module_vdc_TableWrite(VDC_DCTI_3db_2level);
        break;
    case 3:
        module_vdc_TableWrite(VDC_DCTI_9db_1level);
        break;
    case 4:
        module_vdc_TableWrite(VDC_DCTI_9db_2level);
        break;
    case 5:
        module_vdc_TableWrite(VDC_DCTI_6db_1level);
        break;
    case 6:
        module_vdc_TableWrite(VDC_DCTI_6db_2level);
        break;
    case 7:
        module_vdc_TableWrite(VDC_DCTI_12db_1level);
        break;
    case 8:
        module_vdc_TableWrite(VDC_DCTI_12db_2level);
        break;
    }
}

/**
    drv_video_vdc_NoiseStatus
    Read noise status
    @param              { void }
    @return              { noise status }
*/
UINT8 drv_video_vdc_NoiseStatus(void)
{
    return (UINT8)rtdf_inl(VDC_NOISE_STATUS_0x527c);
}

/**
   drvif_module_vdc_tv_Reset
   Initial video decoder module in tv

   @param <void>       { void }
   @return                  { void }

*/
void drv_video_vdc_tv_Reset(void)
{
    drvif_module_vdc_Set_Hstate(VDC_HSTATE_RESET);
    /*secam flag settin */
    rtdf_outl(VDC_FREQ_VALID_INNER_0x5220,0xc2);
    rtdf_outl(VDC_SECAM_FLAG_COUNTER_MAX_0x5228,0xE0);
    rtdf_outl(VDC_SECAM_FLAG_COUNTER_MIN_0x522c,0x05);
    rtdf_outl(VDC_AUTO_MODE_S_M_STATISTIC_0x51d8,0x04);
    /*443flag th*/
    rtdf_outl(VDC_NO_BURST_MAG_FILTER_TH_0x5688,0x00);
    rtdf_maskl(VDC_NO_BURST_443_CTRL_0x568c, 0x0f, 0x00);
    drv_video_vdc_Dcti_Enable(_ENABLE);
}

/**
    drv_video_vdc_RegSetDefault
    Read noise status
    @param              { void }
    @return              { noise status }
*/
void drv_video_vdc_RegSetDefault(void)
{
    rtdf_maskl(VDC_HDETECT_PHASE_SEL_0x50c8,~_BIT5,_BIT5);
    rtdf_outl(VDC_CHROMA_LOCK_CONFIG_0x5098,0x6b);
    rtdf_outl(VDC_VIDEO_CONTROL1_0x5004,0x00);				//diable YPbPr(VDC) function in VDC IP
    rtdf_outl(VDC_VD_BASIC_REGISTER_ENABLE_0x51d4, 0x01);	// Auto Mode: basic register enable (default)
    rtdf_outl(VDC_VSYNC_TIME_CONST_0x515c, 0x0a);		// VSYNC_TIME_CONST: set field polarity
    rtdf_outl(VDC_NO_SIGNAL_DEC_0x516c, 0x03);	//speed up no-signal status
    //SECAM FLAG SETTING
    rtdf_outl(VDC_FREQ_VALID_INNER_0x5220,0x42);
    rtdf_outl(VDC_SECAM_FLAG_COUNTER_MAX_0x5228,0xA0);
    rtdf_outl(VDC_SECAM_FLAG_COUNTER_MIN_0x522c,0x0D);
    rtdf_outl(VDC_AUTO_MODE_S_M_STATISTIC_0x51d8,0x0B);
    //443flag th
    rtdf_outl(VDC_NO_BURST_MAG_FILTER_TH_0x5688,0x12);
    rtdf_maskl(VDC_NO_BURST_443_CTRL_0x568c, 0x0f, 0x4);
    rtdf_maskl(VDC_CSTATE_CTRL_0x5670, 0x1f, 0x00);
    //av out setting
    rtdf_maskl(VDC_VDAC_ABIQ2_0x57b8, 0x3F, 0x40);
    //rtdf_outl(VDC_VDAC_SIB_0x57c0,0x20);
    rtdf_outl(VDC_YC_SEP_CONTROL_0x5280,0x00);
    rtdf_setBits(VDC_INOUT_CTRL_0x5018, _BIT1);

    /* default : for channel switch vsync time const */
    rtdf_maskl(VDC_VSYNC_TIME_CONST2_0x5160,0xffffff00,0x04);
    rtdf_maskl(VDC_VSYNC_TIME_CONST1_0x5164,0xffffff00,0x20);

    rtdf_maskl(VDC_AGC_GATE_TH_SWAP_0x508c,0xffffff7f,0x80);
    rtdf_maskl(VDC_CLAMPAGC_COMPLUMA_NEW_CONTROL_0x506c,0xfffffffC,0x03);
    rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x80);	// Set to Auto mode
}

void drv_video_vdc_FreeRun(bit on_off)
{
    if (on_off)
        rtdf_setBits(VDC_OUTPUT_CONTROL_0x503c, _BIT7);
    else
        rtdf_clearBits(VDC_OUTPUT_CONTROL_0x503c, _BIT7);
}

void drv_video_vdc_Initial(void)
{
    rtdf_outl(VDC_VD_POW_CTRL0_0x5780, 0x3f);		// global power on
    rtdf_outl(VDC_VD_VDCCLAMPPOW_0x5784, 0x7f);		// vdc clamp power on
    rtdf_outl(VDC_PATTERN_CONTROL_0x5418, 0x00);		// disable vd pattern generator
    rtdf_outl(VDC_FSW_CTRL2_0x531c,0x08);			// set 444 output
    rtdf_outl(VDC_CHROMA_DELAY_CTRL_0x50fc, 0x00);	// when 444 output, set cb delay initial
    rtdf_setBits(VDC_VSYNC_TIME_CONST_0x515c, _BIT7);	// Set even/odd swap

    rtdf_outl(VDC_COMB_FILTER_THRESHOLD1_0x52e0,0x07);	// for ives's request of yc2d
    rtdf_outl(VDC_MISC_CONTROL_0x539c, 0x7f);			//enable VDC phase swallow (default)
//	rtdf_outl(VDC_VIDEO_CONTROL1_0x5004, 0x00);			//diable YPbPr(VDC) function in VDC IP
    rtdf_outl(VDC_VD_SGAIN_0x5754, 0x11);			// Set SGain
    rtdf_maskl(VDC_CHROMA_LOOPFILTER_STATE_0x50a0,0xf1,0x04);	// set cstate to 2
    rtdf_setBits(ADC_ANALOG_RESERVE_0x5ff4,_BIT30);	//DAC setting for ver.B

    drv_video_vdc_RegSetDefault();
    drv_video_vdc_Set_DCTI_Setting(3);
    drv_video_vdc_Dcti_Enable(_DISABLE);
    drv_video_vdc_YcDelay(0x00); // do not move , the function need to excute after the set the vdc out444
    drvif_module_vdc_Set_Hstate(VDC_HSTATE_RESET);

    VDC_PRINTF(LOGGER_DEBUG,"video reset done\n");
}

/**
	drv_video_Release
	Release VDC
*/
void	drv_video_Release(void)
{
    vdc_InputSrc = 0xff;
    drv_video_PowerOn(_OFF);
}

/**
   drvif_module_vdc_BlueScreenEnable
   enable/disable blue screen

   @param <uselect> { _ENABLE/_DISABLE /_AUTO}
   @return                 { void }

*/
void drv_video_vdc_BlueScreenEnable(VDC_BLUE_SCREEN_MODE uselect)
{
    switch (uselect)
    {
    case BLUE_SCREEN_ENABLE:
        rtdf_maskl(VDC_OUTPUT_CONTROL_0x503c, ~(_BIT5|_BIT4), _BIT4);
        break;
    case BLUE_SCREEN_DISABLE:
        rtdf_maskl(VDC_OUTPUT_CONTROL_0x503c, ~(_BIT5|_BIT4), _ZERO);
        break;
    case BLUE_SCREEN_AUTO:
        rtdf_maskl(VDC_OUTPUT_CONTROL_0x503c, ~(_BIT5|_BIT4), _BIT5);
        break;
    }
}


/**
   drvif_module_vdc_XNREnable
   enable/disable blue screen

   @param <uselect> { _ENABLE/_DISABLE }
   @return                 { void }

*/

void drvif_module_vdc_XNREnable(_VDC_XNR_MODE uselect)
{
    switch (uselect)
    {
    case VDC_XNR_ENABLE:
        rtdf_maskl(VDC_XNR_CTRL_0x54f0, ~_BIT0, _BIT0);
        break;
    case VDC_XNR_DISABLE:
        rtdf_maskl(VDC_XNR_CTRL_0x54f0, ~_BIT0, _ZERO);
        break;
    }
}


/**
   drvif_module_vdc_SelectInput
   Select signal source

 Depending on the VDC's different input source, set the corresponding setting.
 For different input source of VDC, we need to config first the correct
 setting, for example the data path select for TV, AV or SV. Beside
 the VDC's setting, we also put the corresponding rtd2620 setting here
 just for convenience. Because for different input source, mabye we
 need to tune the suitable color setting.

   @param <src>        { Select signal to show }
   @return                  { void }

*/
void drv_video_vdc_SelectInput(UINT8 ucInputSrc, VDC_INITIAL_OPTIONS  *ptOptions)
{
//	VDC_PRINTF(LOGGER_ERROR, "[drv_video_vdc_SelectInput] ucInputSrc :%x\n",(UINT32)ucInputSrc);
    if (ucInputSrc >= VDC_SOURCE_TOTAL)
    {
        // Get error vdc number
        VDC_PRINTF(LOGGER_ERROR, "[0]Get error ucInputSrc in drv_video_vdc_SelectInput!!\n");
        return;
    }

    // Store input src and firmware will not pass ucInputSrc in future
    // Split video driver from firmware, and can't use info variable
    vdc_InputSrc = ucInputSrc;
//	VDC_PRINTF(LOGGER_ERROR, "[vdc_InputSrc] vd source in :%x   %x \n",(UINT32)vdc_InputSrc,VDC_SOURCE_TV);

    drv_video_vdc_Initial();
    module_vdc_PathSelect(ptOptions);

    if (vdc_InputSrc == VDC_SOURCE_TV)
    {
        rtdf_maskl(VDC_CLAMPAGC_COMPLUMA_NEW_CONTROL_0x506c,0xfffffffd,0x00);//wjchen, 20091111, turn off bottom gain on TV source
        rtdf_maskl(VDC_AGC_PEAK_AND_GATE_CTRL_0x5044,0xfffffff7,0x08);//wjchen, 20091111, turn on peak on TV source

    }
    else
    {
        rtdf_maskl(VDC_CLAMPAGC_COMPLUMA_NEW_CONTROL_0x506c,0xfffffffd,0x02);//wjchen, 20091111, turn on bottom gain on TV source
        rtdf_maskl(VDC_AGC_PEAK_AND_GATE_CTRL_0x5044,0xfffffff7,0x08);//wjchen, 20091111, turn on peak on TV source
    }
#ifdef CONFIG_SCART_AUTO_SWITCH
    if ((vdc_InputSrc == VDC_SOURCE_TV)||(vdc_InputSrc == VDC_SOURCE_FULL_SCART))
#else
    if (vdc_InputSrc == VDC_SOURCE_TV)
#endif
    {
        drv_video_vdc_tv_Reset();
        drv_video_vdc_BlueScreenEnable(BLUE_SCREEN_DISABLE);
        drv_video_vdc_FreeRun(_TRUE);
    }
    else
    {
//		drv_video_vdc_Initial();	// for TV test
//		module_vdc_PathSelect(ptOptions);	//for TV test
        drv_video_vdc_BlueScreenEnable(BLUE_SCREEN_AUTO);
    }
}


#ifdef _VIDEO_TV_SUPPORT

UINT8 drvif_module_vdc_flag358_detect(void)
{
    UINT8  i;
    UINT8  flag_358;
    UINT8  flag;

    flag_358 = 0;
    for (i=1; i<=4; i++)
    {
        fw_timer_event_DelayXms(5);
        flag = (rtdf_inl(VDC_VIDEO_STATUS_REG2_0x500c)&_BIT7);
        if (flag == _BIT7)
            flag_358++;
        else
        {
            if (flag_358 <= 0)
                flag_358 = 0;
            else
                flag_358--;
        }
    }
    if (flag_358 >= 2)
        return 1;
    else
        return 0;
}
#endif
/**
 Check the TV video mode type in auto color system.
 This function will query the video decoder's mode type register.
 Differ which type it is now.
   @param                               { void }
   @return <ucVdc_Curr_Mode> { The result it detected }

 		BIT 0~1
 		 : 1 60 Hz
 		 : 2 50 Hz
 		BIT 4~7
 		 : 00 PAL B,G,H,I
 		 : 10 NTSC M
 		 : 20 SECAM
 		 : 30 NTSC44
 		 : 40 PAL M
 		 : 50 PAL N
 		 : 60 PAL 60
*/
#ifdef _VIDEO_TV_SUPPORT
#if 1//ndef CONFIG_TUNER_SILICON
UINT8 drv_video_GetTvColorStandard(void)
{
#ifndef CONFIG_IFD_ENABLE
    UINT8 afc;
#endif
    UINT8 mn_status;
    UINT8 ucMenuMode,ucAutoMode,checkntsc50;
    UINT8 noise_value;
    UINT8 flag625;
    UINT8 color_burst_detect;
    UINT8 color_magnitude;
    UINT8 average_color_magnitude;
    UINT8 PAL_M_FLAG,PAL_FLAG;
    UINT8 auto_flag;
    UINT8 secam_flag, flg358, new_v_lock, noisy;



	UINT8 CurChn;

#ifdef CONFIG_IFD_ENABLE
    UINT8 if_mode;
    UINT32 curr_Frequency;
    UINT8 video_bw_sel = 0;
    if_mode = 0;
#endif
	CurChn = GET_TV_CHN_DATA(_CUR_CHN_TV);
	
    /*
    auto_flag : 選擇AUTO的時候 一定會強制讓FLOW跑一次,因為第一次切過來系統會將AUTO設起來,所以用AUTO FLAG來判斷是否為第一次
    ucMenuMode:現在VD 被FIX 的MODE
    ucAutoMode:讀取現在AUTO MODE判斷的結?
    color_magnitude:看是否有COLOR BURST
    PAL_FLAG:看是否 PAL FLAG有舉起來
    color_burst_detect:偵測 有無COLOR BURST
    average_color_magnitude:可以量測目前的COLOR BURST大小
    noise_value:可以觀察現在的NOISE LEVEL是多少

    */
    auto_flag = (UINT8)(rtdf_inl(VDC_MANUAL_MODE_0x51c0)&0x80);
    //VDC_PRINTF(LOGGER_DEBUG,"[auto]101vd tv auto  auto_flag=%x.......... \n",(UINT32)auto_flag);
    ucMenuMode = (UINT8)(rtdf_inl(VDC_MANUAL_MODE_0x51c0)&0x7);
    ucAutoMode = (UINT8)(rtdf_inl(VDC_MODE_DETECTION_STATUS_0x51c4)&0x7);
    noise_value = drv_video_vdc_NoiseStatus();
    //VDC_PRINTF(LOGGER_DEBUG,"[auto]102vd tv auto  test ucMenuMode=%x.......... \n",(UINT32)ucMenuMode);
    //VDC_PRINTF(LOGGER_DEBUG,"[auto]103vd tv auto  test ucAutoMode=%x.......... \n",(UINT32)ucAutoMode);
    color_magnitude=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG2_0x500c);
    PAL_FLAG=color_magnitude&0X06;
    //VDC_PRINTF(LOGGER_DEBUG,"[auto]104VDC_VIDEO_STATUS_REG2_VADDR=%x.......... \n",color_magnitude);
    color_magnitude=color_magnitude&0x08;
    average_color_magnitude=(UINT8)rtdf_inl(VDC_AVG_BURST_MAG_STATUS_0x523c);
    //VDC_PRINTF(LOGGER_DEBUG,"[auto]161VDC_AVG_BURST_MAG_STATUS_VADDR=%x ??????........... \n",average_color_magnitude);
    color_burst_detect=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
    color_burst_detect=color_burst_detect&0x08;
    //VDC_PRINTF(LOGGER_DEBUG,"[auto]105VDC_VIDEO_STATUS_REG1_0x5008=%x.......... \n",color_burst_detect);
    PAL_M_FLAG=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG3_0x5010);
    //VDC_PRINTF(LOGGER_DEBUG,"[auto]106VDC_VIDEO_STATUS_REG3_0x5010=%x.......... \n",PAL_M_FLAG);
    Audio_AtvGetMNStatus(&mn_status, NULL);
    //VDC_PRINTF(LOGGER_DEBUG,"[auto]107the audio detect 4.5mhz tone=%x.......... \n",mn_status);
    //VDC_PRINTF(LOGGER_DEBUG,"[auto]108vdc_TV_Mode = %x\n",(UINT32)ucVdc_TV_Mode);

    secam_flag = (PAL_M_FLAG&0x02) ? 1 : 0;
    flg358=drvif_module_vdc_flag358_detect();
    new_v_lock=drvif_module_vdc_New_Vlock();
    noisy=rtdf_inl(VDC_VIDEO_STATUS_REG3_0x5010)&0x08;

#ifdef CONFIG_IFD_ENABLE  //ifd flow


#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
    if ((ucAutoMode !=  ucMenuMode)||((noise_value>0X50)&&(ucAutoMode ==  ucMenuMode))
            ||((noise_value<0X10)&&(color_burst_detect==0x00)&&(ucMenuMode==tv_ZNTSC)&&(ucAutoMode==tv_ZNTSC))
            ||((color_burst_detect==0x00)&&(ucMenuMode==tv_ZNTSC_443)&&(ucAutoMode==tv_ZNTSC_443))
            ||((ucMenuMode==tv_ZPAL_I)&&(ucAutoMode==tv_ZPAL_I)&&(flg358==0x01)&&(noisy==0x08))
            ||((ucMenuMode==tv_ZPAL_N)&&(ucAutoMode==tv_ZPAL_N)&&(flg358==0x00))
            ||((ucMenuMode==tv_ZPAL_M)&&(ucAutoMode==tv_ZPAL_M)&&(flg358==0x00))//for palm==>pal 60 detect
            ||((ucMenuMode==tv_ZSECAM)&&(ucAutoMode==tv_ZSECAM)&&(new_v_lock==0x00))//for secaml==>pal
            ||((ucMenuMode==tv_ZSECAM)&&(ucAutoMode==tv_ZSECAM)&&(!secam_flag))//for secam dk==>pal
       )
#else

#ifdef  CONFIG_SUPPORT_PAL60
    /*
    判斷是否是該進入讓AUTO 重做的條件
    */
    /*
    if((ucAutoMode !=  ucMenuMode)||((noise_value>0X50)&&(ucAutoMode ==  ucMenuMode))
            ||((noise_value<0X10)&&(color_burst_detect==0x00)&&(ucMenuMode==tv_ZNTSC)&&(ucAutoMode==tv_ZNTSC))
            ||((color_burst_detect==0x00)&&(ucMenuMode==tv_ZNTSC_443)&&(ucAutoMode==tv_ZNTSC_443))||(auto_flag==tv_AUTO)
            ||((ucMenuMode==tv_ZPAL_I)&&(ucAutoMode==tv_ZPAL_I)&&(((PAL_M_FLAG&0X03)==0X00)||(PAL_FLAG==0X00))&&(mn_status==1))
            ||((ucMenuMode==tv_ZPAL_N)&&(ucAutoMode==tv_ZPAL_N)&&(average_color_magnitude<=0x0a))
            ||((ucMenuMode==tv_ZPAL_M)&&(ucAutoMode==tv_ZPAL_M)&&(color_magnitude==0x00))//for palm==>pal 60 detect

    )*/
#if (defined(CONFIG_PROJECT_TV024)||defined(CONFIG_PROJECT_TV024_1))
    if ((ucAutoMode !=  ucMenuMode)
#else
        if ((ucAutoMode !=  ucMenuMode)||((noise_value>0X50)&&(ucAutoMode ==  ucMenuMode))
#endif
            ||((noise_value<0X10)&&(color_burst_detect==0x00)&&(ucMenuMode==tv_ZNTSC)&&(ucAutoMode==tv_ZNTSC))
            ||((color_burst_detect==0x00)&&(ucMenuMode==tv_ZNTSC_443)&&(ucAutoMode==tv_ZNTSC_443))||(auto_flag==tv_AUTO)
            ||((ucMenuMode==tv_ZPAL_I)&&(ucAutoMode==tv_ZPAL_I)&&(flg358==0x01)&&(noisy==0x08))//leo 20100119 for cross pattern
            ||((ucMenuMode==tv_ZPAL_N)&&(ucAutoMode==tv_ZPAL_N)&&(flg358==0x00))
            ||((ucMenuMode==tv_ZPAL_M)&&(ucAutoMode==tv_ZPAL_M)&&(flg358==0x00))//for palm==>pal 60 detect
            ||((ucMenuMode==tv_ZSECAM)&&(ucAutoMode==tv_ZSECAM)&&(new_v_lock==0x00))//for secaml==>pal
       )
#else
//VDC_PRINTF(LOGGER_DEBUG,"the audio detect 4.5mhz tone=%x.......... \n",mn_status);
        /*
        if((ucAutoMode !=  ucMenuMode)||((noise_value>0X50)&&(ucAutoMode ==  ucMenuMode))
                ||((noise_value<0X10)&&(color_burst_detect==0x00)&&(ucMenuMode==tv_ZNTSC)&&(ucAutoMode==tv_ZNTSC))
                ||((color_burst_detect==0x00)&&(ucMenuMode==tv_ZNTSC_443)&&(ucAutoMode==tv_ZNTSC_443))||(auto_flag==tv_AUTO)
                ||((ucMenuMode==tv_ZPAL_I)&&(ucAutoMode==tv_ZPAL_I)&&(((PAL_M_FLAG&0X03)==0X00)||(PAL_FLAG==0X00))&&(mn_status==1))
                ||((ucMenuMode==tv_ZPAL_N)&&(ucAutoMode==tv_ZPAL_N)&&(average_color_magnitude<=0x0a)))
        */
        if ((ucAutoMode !=  ucMenuMode)||((noise_value>0X50)&&(ucAutoMode ==  ucMenuMode))
                ||((noise_value<0X10)&&(color_burst_detect==0x00)&&(ucMenuMode==tv_ZNTSC)&&(ucAutoMode==tv_ZNTSC))
                ||((color_burst_detect==0x00)&&(ucMenuMode==tv_ZNTSC_443)&&(ucAutoMode==tv_ZNTSC_443))||(auto_flag==tv_AUTO)
                ||((ucMenuMode==tv_ZPAL_I)&&(ucAutoMode==tv_ZPAL_I)&&(flg358==0x01)&&(noisy==0x08))//leo 20100119 for cross pattern
                ||((ucMenuMode==tv_ZPAL_N)&&(ucAutoMode==tv_ZPAL_N)&&(flg358==0x00))
                ||((ucMenuMode==tv_ZPAL_M)&&(ucAutoMode==tv_ZPAL_M)&&(flg358==0x00))//for palm==>pal 60 detect
           )
           //if(0)

#endif //CONFIG_SUPPORT_PAL60
#endif //#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
    {
        //if_mode=drvif_module_ifd_Read_mode();//一開始確認 IF MODE 的制式
        //VDC_PRINTF(LOGGER_DEBUG,"the if demed mode== %x.......... \n",if_mode);

#ifdef CONFIG_IFD_ENABLE
        video_bw_sel = (rtdf_inl(IFD_VIDEO_BW_SEL_VADDR)&0x0000f000)>>12;
        //VDC_PRINTF(LOGGER_DEBUG,"[auto]109video_bw_sel=%x.......... \n",(UINT32)video_bw_sel);
#endif //#ifdef CONFIG_IFD_ENABLE	

        curr_Frequency = GET_CHANNEL_FREQ(CurChn);	//將現在的頻率讀出來
        /********************************ERROR CASE:*****************************************/
        /*
        #ifdef HISENSE_TV_SECAM_SIGNAL
        		if((ucMenuMode==tv_ZSECAM)&&(ucAutoMode==tv_ZPAL_I)&&(average_color_magnitude<0x13)
        			&&(noise_value<0x20)&&(auto_flag!=tv_AUTO))
        		{
        			if(CTvGetBackupVideoMode(stTvInfo.CurChn)==ZSECAM)
        			{
        				VDC_PRINTF(LOGGER_INFO,"the bug is the secam  the no color burst ,do not  mode detect  ........... \n");
        				//~~return 0;//ZSECAM;//ok//reynold return to 0, or it will reset while color system set to AUTO mode
        				if(ucVdc_Curr_Mode == ZSECAM)
        					return 0;
        				else
        				{
        					ucVdc_Curr_Mode = ZSECAM;
        					return ZSECAM;
        				}
        			}
        		}
        #endif
        */

// linmc20091229, help sky_yeh to add auto scan enhance function
#ifdef CONFIG_EUROPE_AUTO_SCAN_FLOW
        if ((ucAutoMode==tv_ZSECAM)&&(ucMenuMode==tv_ZPAL_I)&&(average_color_magnitude<0x08)&&(noise_value<0x08)&&(auto_flag!=tv_AUTO))
        {
            //VDC_PRINTF(LOGGER_DEBUG,"[auto]110manu = pal, auto=zecam, but no color burst, mode detect is pal i \n");
            if (ucVdc_TV_Mode == ZPAL_I)
                return 0;
            else
            {
                ucVdc_TV_Mode = ZPAL_I;
                return ZPAL_I;
            }
        }
        if ((ucAutoMode==tv_ZPAL_I)&&(ucMenuMode==tv_ZSECAM)&&(secam_flag==1)&&(noise_value<0x20)&&(PAL_FLAG==0X00)&&(auto_flag!=tv_AUTO))
        {
            //VDC_PRINTF(LOGGER_DEBUG,"[auto]111manu = SECAM, auto=PAL, the secam flag is on ,the pal flag is off then the mode is secam \n");
            if (ucVdc_TV_Mode == ZSECAM)
                return 0;
            else
            {
                ucVdc_TV_Mode = ZSECAM;
                return ZSECAM;
            }
        }

#endif
        if ((ucAutoMode==tv_ZNTSC_443)&&(ucMenuMode==tv_ZNTSC_443)&&(color_burst_detect==0x00)&&(auto_flag!=tv_AUTO))
        {
            rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000000);
            //drvif_module_ifd_IF_AUTO_FRQ_AND_IF_MODE_Setting(if_mode,_SIG_MODE_NTSC,curr_Frequency,500);
            drvif_ifd_set_video_standard(ZNTSC, curr_Frequency);//CTunerInit(_SIG_MODE_NTSC);
            //VDC_PRINTF(LOGGER_DEBUG,"[auto]112the mode detect is ntsc 443 the bug is the ntsc the no color burst ,mode detect is ntsc ........... \n");

            if (ucVdc_TV_Mode == ZNTSC)
                return 0;
            else
            {
                ucVdc_TV_Mode = ZNTSC;
                return ZNTSC;
            }
        }

        if ((ucMenuMode==tv_ZPAL_I)&&(ucAutoMode==tv_ZNTSC_50)&&(auto_flag!=tv_AUTO))
        {
            //VDC_PRINTF(LOGGER_DEBUG,"[auto]113the bug is the PAL  the no color burst  mode is NTSC50 ,do not  mode detect  ........... \n");
            if (ucVdc_TV_Mode == ZPAL_I)
                return 0;
            else
            {
                ucVdc_TV_Mode = ZPAL_I;
                //drvif_module_ifd_IF_SAVE_AUTO_FRQ(if_mode,_SIG_MODE_PAL_I,curr_Frequency);
                return ZPAL_I;
            }
        }

        if ((ucMenuMode==tv_ZPAL_I)&&(ucAutoMode==tv_ZPAL_I)&&(((PAL_M_FLAG&0X03)==0X01)||(PAL_FLAG==0X06))/*&&(mn_status==0)*/&&(auto_flag!=tv_AUTO))
        {
            //VDC_PRINTF(LOGGER_DEBUG,"[auto]114the bug is the PAL ,BUT THE PAL AND SECAM FLAG NO DETECT......... \n");
            if (ucVdc_TV_Mode == ZPAL_I)
                return 0;
            else
            {
                ucVdc_TV_Mode = ZPAL_I;
                //drvif_module_ifd_IF_SAVE_AUTO_FRQ(if_mode,_SIG_MODE_PAL_I,curr_Frequency);
                return ZPAL_I;
            }
        }

        if ((ucAutoMode==tv_ZNTSC)&&(ucMenuMode==tv_ZNTSC)&&(color_burst_detect==0x00)&&(noise_value>0x20)&&(noise_value<0x70)&&(auto_flag!=tv_AUTO))
        {
            //VDC_PRINTF(LOGGER_DEBUG,"[auto]115the bug is the ntsc the no color burst ,do not  mode detect ........... \n");
            if (ucVdc_TV_Mode == ZNTSC)
                return 0;
            else
            {
                ucVdc_TV_Mode = ZNTSC;
                return ZNTSC;
            }
        }

        if ((ucAutoMode==tv_ZNTSC)&&(ucMenuMode==tv_ZNTSC)&&(color_burst_detect==0x00)&&(average_color_magnitude<0x10)&&(noise_value<0x10)&&(auto_flag!=tv_AUTO))
        {
            //VDC_PRINTF(LOGGER_DEBUG,"[auto]116the bug is the ntsc the no color burst ,do not  mode detect 111111........... \n");
            if (ucVdc_TV_Mode == ZNTSC)
                return 0;
            else
            {
                ucVdc_TV_Mode = ZNTSC;
                return ZNTSC;
            }
        }

        if ((ucMenuMode==tv_ZNTSC)&&(ucAutoMode==tv_ZNTSC_443)&&(mn_status==1)&&(auto_flag!=tv_AUTO))
        {
            drvif_ifd_set_video_standard(ZNTSC, curr_Frequency);//drvif_ifd_set_iftuner_setting(ZNTSC,curr_Frequency,500); //irene mark
            fw_timer_event_DelayXms(500);
            //No TunerInit in IFD flow-->CTunerInit(_SIG_MODE_NTSC);//irene
            //VDC_PRINTF(LOGGER_DEBUG,"[auto]117the bug is the ntsc ==>ntsc443,do not  mode detect 00000........... \n");

            if (ucVdc_TV_Mode == ZNTSC)
                return 0;
            else
            {
                ucVdc_TV_Mode = ZNTSC;
                return ZNTSC;
            }
        }

        if ((ucMenuMode==tv_ZNTSC)&&(ucAutoMode==tv_ZNTSC_443)&&(auto_flag!=tv_AUTO))
        {
            fw_timer_event_DelayXms(500);  //wait the color of vd stable
            fw_timer_event_DelayXms(500);  // wait the color of vd stable
            color_burst_detect=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
            //VDC_PRINTF(LOGGER_DEBUG,"[auto]118the vd curr mode=ntsc the auto mode=ntsc443VDC_VIDEO_STATUS_REG1_VADDR=%x.......... \n",(UINT32)color_burst_detect);
            color_burst_detect=color_burst_detect&0x08;
            //tick system WD, to avoid system reset
            flow_timer_Reset_WatchDog();
            if ((color_burst_detect!=0x08))
            {
                rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000004);
                drvif_ifd_set_video_standard(ZNTSC, curr_Frequency);//drvif_ifd_set_iftuner_setting(ZNTSC,curr_Frequency,500);  //irene mark
                ScalerTimer_DelayXms(500);
                //No TunerInit in IFD flow-->CTunerInit(_SIG_MODE_NTSC);//irene
                color_burst_detect=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
                //VDC_PRINTF(LOGGER_DEBUG,"[auto]119the vd fix ntsc443 VDC_VIDEO_STATUS_REG1_VADDR=%x.......... \n",(UINT32)color_burst_detect);
                color_burst_detect=color_burst_detect&0x08;
                Audio_AtvGetMNStatus(&mn_status, NULL);

                if (/*(color_burst_detect==0x08)&&*/(mn_status==0))
                {
                    //VDC_PRINTF(LOGGER_DEBUG,"[auto]120the bug is the ntsc ==>ntsc443 ........... \n");
                    if (ucVdc_TV_Mode == ZNTSC_443)
                        return 0;
                    else
                    {
                        ucVdc_TV_Mode = ZNTSC_443;
                        return ZNTSC_443;
                    }
                }
                else
                {
                    //VDC_PRINTF(LOGGER_DEBUG,"[auto]159the tv auto mode is on  the  vd fix palm ,the mode detect is ntsc443........\n");
                    rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000000);
                    drvif_ifd_set_video_standard(ZNTSC, curr_Frequency);//CTunerInit(_SIG_MODE_NTSC);

                    if (ucVdc_TV_Mode == ZNTSC)
                        return 0;
                    else
                    {
                        ucVdc_TV_Mode = ZNTSC;
                        return ZNTSC;
                    }
                }
            }
            else
            {
                if (ucVdc_TV_Mode == ZNTSC)
                    return 0;
                else
                {
                    ucVdc_TV_Mode = ZNTSC;
                    return ZNTSC;
                }
            }
        }

//*************************AUTO DETECT FINE THE MODE****************************
#if defined(CONFIG_PROJECT_TV020)||defined(CONFIG_PROJECT_TV024_2)
        // linmc@20100107, reset burst gate value, prevent pal dete[ct error
        if ((ucMenuMode==tv_ZPAL_I)&&(ucAutoMode==tv_ZSECAM))
        {
            rtdf_outl(VDC_BURST_GATE_START_0x5130,0x50);
            rtdf_outl(VDC_BURST_GATE_END_0x5134,0x64);
        }
        else
        {
            rtdf_outl(VDC_BURST_GATE_START_0x5130,0x32);
            rtdf_outl(VDC_BURST_GATE_END_0x5134,0x46);
        }


// irene@20100108 fix this driver bug
        /*
        	// linmc@20100107, noticed when secam dk -> pal bg, it will not reset video_bw_sel,
        	// then it will cause noise level becomes large
        	if (video_bw_sel == GET_IFD_VIDEOBPF_PALDKI())
        		rtdf_maskl(IFD_VIDEO_BW_SEL_VADDR, 0xffff0fff, GET_IFD_VIDEOBPF_PALBG()<<12);
        	else if (video_bw_sel == GET_IFD_VIDEOBPF_SECAM())
        		rtdf_maskl(IFD_VIDEO_BW_SEL_VADDR, 0xffff0fff, GET_IFD_VIDEOBPF_PALBG()<<12);
        	RTD_Log(tv_log_level,"IFD_VIDEO_BW_SEL_VADDR=%x\n",rtdf_inl(IFD_VIDEO_BW_SEL_VADDR));
        */
#endif


        drvif_ifd_set_video_standard(ZPAL_I, curr_Frequency);//drvif_ifd_set_iftuner_setting(ZPAL_I,curr_Frequency,10);

        fw_timer_event_DelayXms(10);

        //VDC_PRINTF(LOGGER_DEBUG,"[auto]121the tv auto mode is on  the tuner set the pal i,the mode detect ??????........... \n");
        //VDC_PRINTF(LOGGER_DEBUG,"[auto]122the tv auto mode is on and  tuner set pal i ,the vd and the tuner is lock........... \n");
        rtd_outl(VDC_MANUAL_MODE_0x51c0, 0x06);
        //tick system WD, to avoid system reset
        flow_timer_Reset_WatchDog();
        fw_timer_event_DelayXms(600);//wait  the vd  stable
        noise_value = drv_video_vdc_NoiseStatus();
        flag625=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG3_0x5010);
        PAL_M_FLAG=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG3_0x5010);
        //VDC_PRINTF(LOGGER_DEBUG,"[auto]123VDC_VIDEO_STATUS_REG3_VADDR=%x.......... \n",PAL_M_FLAG);
        //VDC_PRINTF(LOGGER_DEBUG,"[auto]124the tuner set the pal  the mode detect=%x ??????........... \n",ucAutoMode);
        //VDC_PRINTF(LOGGER_DEBUG,"[auto]125the tuner set the pal  the mode detect noise=%x ??????........... \n",noise_value);

        if (drv_video_vdc_CheckHVisLocked())
        {
#if 0
            //ucAutoMode = (UINT8)(rtdf_inl(VDC_MODE_DETECTION_STATUS_VADDR)&0x7);
#else
            //ucAutoMode =  drv_video_vdc_IdentifyColorStandard();
            ucAutoMode = (UINT8)(rtdf_inl(VDC_MODE_DETECTION_STATUS_0x51c4)&0x7);
#endif

            if ((flag625&0x04)==0x04)
            {

                if ((ucAutoMode == tv_ZPAL_I)&&(noise_value<=0x30))
                {//PAL I
                    //VDC_PRINTF(LOGGER_DEBUG,"[auto]126auto mode is on ,the mode detect is pal i\n");
                    ucVdc_TV_Mode = ZPAL_I;
                    return ZPAL_I;
                }
                else
                {
                    Audio_AtvGetMNStatus(&mn_status, NULL);
                    if ((ucAutoMode == tv_ZPAL_I)&&(noise_value>=0x31)&&(noise_value<=0x80))
                    {//PAL I or others
                        color_burst_detect=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
                        if (((mn_status==0)||((color_burst_detect&0x08)==0x08))&&((PAL_M_FLAG&0X03)==0X01))
                        {
                            //VDC_PRINTF(LOGGER_DEBUG,"[auto]127auto mode is on ,the mode detect is pal i . noise is big ,the color detect\n");
                            drvif_ifd_set_video_standard(ZPAL_I, curr_Frequency);//drvif_ifd_set_iftuner_setting(ZPAL_I,curr_Frequency,50);
                            fw_timer_event_DelayXms(50);
                            ucVdc_TV_Mode = ZPAL_I;
                            return ZPAL_I;
                        }

                    }
                }


                if ((ucAutoMode == tv_ZSECAM)&&(noise_value<=0x30)) //secam
                {

                    if ((average_color_magnitude<0x08)&&(noise_value<0x08)) // maybe pali monoscope with no color
                    {
                        if ((ucVdc_TV_Mode == ZSECAM) && (!secam_flag))
                        {
                            //VDC_PRINTF(LOGGER_DEBUG,"[auto]128avg_color=%d, noise=%d, secam_flag=%d, mode is pali\n", (UINT32)average_color_magnitude, (UINT32)noise_value, (UINT32)secam_flag);
                            ucVdc_TV_Mode = ZPAL_I;
                            return ZPAL_I;
                        }
                        else
                        {
                            //VDC_PRINTF(LOGGER_DEBUG,"[auto]129avg_color=%d, noise=%d, secam_flag=%d mode is secam\n", (UINT32)average_color_magnitude, (UINT32)noise_value, (UINT32)secam_flag);
                            if (ucVdc_TV_Mode == ZSECAM)
                                return 0;
                            else
                            {
                                rtd_outl(VDC_MANUAL_MODE_0x51c0, 0x07);
                                //VDC_PRINTF(LOGGER_DEBUG,"[auto]130the tv auto mode is on ,the mode detect is secam1........... \n");
                                drvif_ifd_set_video_standard(ZPAL_I, curr_Frequency);////CTunerInit(_SIG_MODE_PAL_I);//irene
                                ucVdc_TV_Mode = ZSECAM;
                                return ZSECAM;
                            }
                        }
                    }
                    else
                    {
                        rtd_outl(VDC_MANUAL_MODE_0x51c0, 0x07);
                        //VDC_PRINTF(LOGGER_DEBUG,"[auto]131the tv auto mode is on ,the mode detect is secam1........... \n");
                        drvif_ifd_set_video_standard(ZPAL_I, curr_Frequency);//CTunerInit(_SIG_MODE_PAL_I);//irene
                        ucVdc_TV_Mode = ZSECAM;
                        return ZSECAM;
                    }
                }

                if (ucAutoMode == tv_ZNTSC_50)
                {//ntsc50
                    noise_value= drv_video_vdc_NoiseStatus();
                    color_magnitude=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG2_0x500c);

                    if (((color_magnitude&0x08)==0x00)&&(noise_value<0x12))
                    {
                        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000006);
                        //VDC_PRINTF(LOGGER_DEBUG,"[auto]132ntsc 50 error and no color_magnitude ,the tuner set pal . the mode detect fix pal.......... \n");
                        drvif_ifd_set_video_standard(ZPAL_I, curr_Frequency);//CTunerInit(_SIG_MODE_PAL_I);//irene
                        ucVdc_TV_Mode= ZPAL_I;
                        return ZPAL_I;
                    }

                    // linmc@20100123, add it
                    if ((!color_burst_detect) && (!secam_flag))// treat as pali
                    {
                        if (ucVdc_TV_Mode == ZPAL_I)
                            return 0;
                        else
                        {
                            drvif_ifd_set_video_standard(ZPAL_I, curr_Frequency);//CTunerInit(_SIG_MODE_PAL_I);
                            ucVdc_TV_Mode = ZPAL_I;
                            return ZPAL_I;
                        }
                    }
                }
            }
            else
            {
                if (ucAutoMode == tv_ZNTSC_443)
                {//ntsc443
                    Audio_AtvGetMNStatus(&mn_status, NULL);
                    fw_timer_event_DelayXms(300); //wait  the vd stable and  detect the color burst  stable
                    rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000004);
                    color_burst_detect=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
                    color_magnitude=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG2_0x500c);
                    //VDC_PRINTF(LOGGER_DEBUG,"[auto]133VDC_VIDEO_STATUS_REG1_VADDR=%x.......... \n",(UINT32)color_burst_detect);
                    //VDC_PRINTF(LOGGER_DEBUG,"[auto]134VDC_VIDEO_STATUS_REG2_VADDR=%x.......... \n",color_magnitude);
                    checkntsc50= (UINT8)(rtdf_inl(VDC_MODE_DETECTION_STATUS_0x51c4)&0x7);
                    //VDC_PRINTF(LOGGER_DEBUG,"[auto]135the tuners set pal the mode detect the ntsc443 ,after set the ntsc443 and mode detect=%x ........... \n",checkntsc50);
                    noise_value= drv_video_vdc_NoiseStatus();
                    //VDC_PRINTF(LOGGER_DEBUG,"[auto]136checek the  ntsc  AUDIO TONE=%x.......... \n",mn_status);
                    //VDC_PRINTF(LOGGER_DEBUG,"[auto]137the ntsc443 noise =%x.......... \n",noise_value);

                    if (((color_burst_detect&0x08)==0x08)&&(mn_status==0)&&(noise_value<0x80))
                    {
                        //VDC_PRINTF(LOGGER_DEBUG,"[auto]138ntsc443 detect is ok ok ok ok ok ok1111111111111........... \n");
                        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000004);
                        //drvif_ifd_set_iftuner_setting(ZPAL_I,curr_Frequency,50);
                        drvif_ifd_set_video_standard(ZPAL_I, curr_Frequency);//CTunerInit(_SIG_MODE_PAL_I);//irene
                        ucVdc_TV_Mode = ZNTSC_443;
                        return ZNTSC_443;
                    }
                }

#ifdef  CONFIG_SUPPORT_PAL60

                if (ucAutoMode == tv_ZPAL_60)
                {//pal60
                    rtd_outl(VDC_MANUAL_MODE_0x51c0, 0x00000005);
                    fw_timer_event_DelayXms(300);   //wait  the vd stable  and detect the noise level
                    noise_value= drv_video_vdc_NoiseStatus();
                    ucAutoMode = (UINT8)(rtdf_inl(VDC_MODE_DETECTION_STATUS_0x51c4)&0x7);
                    Audio_AtvGetMNStatus(&mn_status, NULL);
                    //VDC_PRINTF(LOGGER_DEBUG,"[auto]139MN carrier lock = %x \n",(UINT32)mn_status);

                    if ((ucAutoMode == tv_ZPAL_60)&&(mn_status==0))
                    {//PAL-60
                        //VDC_PRINTF(LOGGER_DEBUG,"[auto]140PAL60 DETECT.........\n");
                        drvif_ifd_set_video_standard(ZPAL_I, curr_Frequency);//CTunerInit(_SIG_MODE_PAL_I);//irene
                        ucVdc_TV_Mode = ZPAL_60;
                        return ZPAL_60;
                    }
                }

#endif
                goto _ifd_set_ntsc;

            }//if((flag625&0x04)==0x04)


#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
#if defined(CONFIG_PROJECT_TV020)||defined(CONFIG_PROJECT_TV024_2)
            if (GET_CHANNEL_COLORSYSTEM(CurChn) != ZAUTO)
            {
                //tick system WD, to avoid system reset
                flow_timer_Reset_WatchDog();
                if (fwif_tv_auto_SECAML_funtion(curr_Frequency)==ZSECAML)
                {
                    if (ucVdc_TV_Mode == ZSECAML)
                        return 0;
                    ucVdc_TV_Mode = ZSECAML;
                    return ZSECAML;
                }

                if (fwif_tv_auto_SECAMLA_funtion(curr_Frequency)==ZSECAMLA)
                {
                    ucVdc_TV_Mode = ZSECAMLA;
                    return ZSECAMLA;
                }
            }

            return  0xff; //unknow
#else
            //tick system WD, to avoid system reset
            flow_timer_Reset_WatchDog();
#ifndef CONFIG_TUNER_SILICON
            if (fwif_tv_auto_SECAML_funtion(curr_Frequency)==ZSECAML)
            {
                if (ucVdc_TV_Mode == ZSECAML)
                    return 0;
                ucVdc_TV_Mode = ZSECAML;
                return ZSECAML;
            }
            if (fwif_tv_auto_SECAMLA_funtion(curr_Frequency)==ZSECAMLA)
            {
                ucVdc_TV_Mode = ZSECAMLA;
                return ZSECAMLA;
            }
#endif
#endif
#endif

        }
        else //if(drv_video_vdc_CheckHVisLocked()) in PAL
        {
#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
            //tick system WD, to avoid system reset
            flow_timer_Reset_WatchDog();
#ifndef CONFIG_TUNER_SILICON
            if (fwif_tv_auto_SECAML_funtion(curr_Frequency)==ZSECAML)
            {
                if (ucVdc_TV_Mode == ZSECAML)
                    return 0;
                ucVdc_TV_Mode = ZSECAML;
                return ZSECAML;
            }

            if (fwif_tv_auto_SECAMLA_funtion(curr_Frequency)==ZSECAMLA)
            {
                ucVdc_TV_Mode = ZSECAMLA;
                return ZSECAMLA;
            }
#endif
#endif
        }

//start of goto _ifd_set_ntsc
_ifd_set_ntsc:


        drvif_ifd_set_video_standard(ZNTSC, curr_Frequency);//drvif_ifd_set_iftuner_setting(ZNTSC,curr_Frequency,10);
        //VDC_PRINTF(LOGGER_DEBUG,"[auto]141the tv auto mode is on and  tuner set ntsc,the vd and the tuner is lock........... \n");
        rtd_outl(VDC_MANUAL_MODE_0x51c0, 0x00);
        fw_timer_event_DelayXms(300);//wait  the vd stable
        noise_value= drv_video_vdc_NoiseStatus();
        flag625=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG3_0x5010);
        //VDC_PRINTF(LOGGER_DEBUG,"[auto]142the tuner set the ntsc  the mode detect=%x ??????........... \n",ucAutoMode);
        //VDC_PRINTF(LOGGER_DEBUG,"[auto]143the tuner set the ntsc the mode detect noise=%x ??????........... \n",noise_value);

        if (drv_video_vdc_CheckHVisLocked())
        {
            ucAutoMode = (UINT8)(rtdf_inl(VDC_MODE_DETECTION_STATUS_0x51c4)&0x7);
            if ((flag625&0x04)==0x00)
            {
                if ((ucAutoMode == tv_ZNTSC)&&(noise_value<=0x30))
                {//NTSC
                    fw_timer_event_DelayXms(50);//wait  the vd's color  stable
                    color_magnitude=drvif_module_vdc_AVG_BURST_MAG_STATUS();
                    //VDC_PRINTF(LOGGER_DEBUG,"[auto]144the tv auto mode is on ,the mode detect is NTSC and the VDC_VIDEO_STATUS_REG2_VADDR=%x.......... \n",color_magnitude);
                    //VDC_PRINTF(LOGGER_DEBUG,"[auto]145the tv auto mode is on ,the mode detect is NTSC......... \n");
                    //drvif_ifd_set_iftuner_setting(ZNTSC,curr_Frequency,500);
                    drvif_ifd_set_video_standard(ZNTSC, curr_Frequency);//CTunerInit(_SIG_MODE_NTSC);//irene
                    ucVdc_TV_Mode = ZNTSC;
                    return ZNTSC;
                }

                rtd_outl(VDC_MANUAL_MODE_0x51c0, 0x01);
                fw_timer_event_DelayXms(300);//wait  the  noise of vd  stable
                noise_value= drv_video_vdc_NoiseStatus();
                flg358=drvif_module_vdc_flag358_detect();
                ucAutoMode = (UINT8)(rtdf_inl(VDC_MODE_DETECTION_STATUS_0x51c4)&0x7);

                if ((ucAutoMode == tv_ZPAL_M)&&(flg358==1))
                {//PAL-M
                    //VDC_PRINTF(LOGGER_DEBUG,"[auto]146the tv auto mode is on ,the mode detect is PAL-M.........\n");
                    //drvif_ifd_set_iftuner_setting(_SIG_MODE_NTSC,curr_Frequency,500);
                    drvif_ifd_set_video_standard(ZNTSC, curr_Frequency);//CTunerInit(_SIG_MODE_NTSC);//irene
                    ucVdc_TV_Mode = ZPAL_M;
                    return ZPAL_M;
                }
                else
                {
                    if ((ucAutoMode == tv_ZNTSC_443)&&(mn_status==1)) //ntsc443
                    {
                        rtd_outl(VDC_MANUAL_MODE_0x51c0, 0x00);
                        //VDC_PRINTF(LOGGER_DEBUG,"[auto]147the tv auto mode is on  the  vd fix palm ,the mode detect is ntsc443........\n");
                        //drvif_ifd_set_iftuner_setting(ZNTSC,curr_Frequency,500);
                        drvif_ifd_set_video_standard(ZNTSC, curr_Frequency);//CTunerInit(_SIG_MODE_NTSC);//irene
                        ucVdc_TV_Mode = ZNTSC;
                        return ZNTSC;
                    }
                    else
                    {
                        rtd_outl(VDC_MANUAL_MODE_0x51c0, 0x00);
                        //VDC_PRINTF(LOGGER_DEBUG,"[auto]152the tv auto mode is on  the  vd fix palm ,the mode detect is ntsc443........\n");
                    }
                }
            }
            else
            {
                rtd_outl(VDC_MANUAL_MODE_0x51c0, 0x03);
                fw_timer_event_DelayXms(300); //wait  the nosie of  stable
                noise_value= drv_video_vdc_NoiseStatus();
                flg358=drvif_module_vdc_flag358_detect();
                ucAutoMode = (UINT8)(rtdf_inl(VDC_MODE_DETECTION_STATUS_0x51c4)&0x7);
                //if((ucAutoMode == tv_ZPAL_N )&&(mn_info.mn_status==1)){//PAL-N
                if ((ucAutoMode == tv_ZPAL_N )&&(flg358==1)) //PAL-N
                {
                    //VDC_PRINTF(LOGGER_DEBUG,"[auto]148the tv auto mode is on ,the mode detect is PAL-N.........\n");
                    //drvif_ifd_set_iftuner_setting(ZNTSC,curr_Frequency,500);
                    drvif_ifd_set_video_standard(ZNTSC, curr_Frequency);//CTunerInit(_SIG_MODE_NTSC);//irene
                    ucVdc_TV_Mode = ZPAL_N;
                    return ZPAL_N;
                }
            }
        }
        else // if(drv_video_vdc_CheckHVisLocked()) in NTSC
        {
            //VDC_PRINTF(LOGGER_DEBUG,"\n[auto]149************* return 0xff _1\n");
            return  0xff; //unknow
        }
//end of goto _ifd_set_ntsc

    }
    else //if((ucAutoMode !=  ucMenuMode)
    {
        //VDC_PRINTF(LOGGER_DEBUG,"\n[auto]150************* return 0 _2\n");
        return 0;//ok
    }

    return 0;//ok


#else  //CONFIG_IFD_ENABLE
    /*
    判斷是否是該進入讓AUTO 重做的條件
    */
    if ((ucAutoMode !=  ucMenuMode)
            ||(auto_flag==tv_AUTO)
            ||((noise_value>0X50)&&(ucAutoMode ==  ucMenuMode))
            ||((noise_value<0X10)&&(color_burst_detect==0x00)&&(ucMenuMode==tv_ZNTSC)&&(ucAutoMode==tv_ZNTSC))
            ||((color_burst_detect==0x00)&&(ucMenuMode==tv_ZNTSC_443)&&(ucAutoMode==tv_ZNTSC_443))
            ||((ucMenuMode==tv_ZPAL_I)&&(ucAutoMode==tv_ZPAL_I)&&(((PAL_M_FLAG&0X03)==0X00)||(PAL_FLAG==0X00))&&(mn_status==1)))
    {

//********************************ERROR CASE:*****************************************
        if ((ucAutoMode==tv_ZNTSC_443)&&(ucMenuMode==tv_ZNTSC_443)&&(color_burst_detect==0x00)&&(auto_flag!=tv_AUTO))
        {
            rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000000);
            drvif_tuner_init(_SIG_MODE_NTSC);
            drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_MN);
            VDC_PRINTF(LOGGER_DEBUG,"mode detect is ntsc 443 the bug is the ntsc the no color burst ,mode detect is ntsc\n");

            if (ucVdc_Curr_Mode == ZNTSC)
                return 0;
            else
            {
                ucVdc_Curr_Mode = ZNTSC;
                return ZNTSC;
            }
        }

        if ((ucMenuMode==tv_ZPAL_I)&&(ucAutoMode==tv_ZNTSC_50)&&(auto_flag!=tv_AUTO))
        {
            VDC_PRINTF(LOGGER_DEBUG,"bug is the PAL  the no color burst  mode is NTSC50 ,do not  mode detect\n");
            if (ucVdc_Curr_Mode == ZPAL_I)
                return 0;
            else
            {
                ucVdc_Curr_Mode = ZPAL_I;
                return ZPAL_I;
            }
        }

        if ((ucMenuMode==tv_ZPAL_I)&&(ucAutoMode==tv_ZPAL_I)&&(((PAL_M_FLAG&0X03)==0X01)
                ||(PAL_FLAG==0X06))&&(mn_status==0)&&(auto_flag!=tv_AUTO))
        {
            VDC_PRINTF(LOGGER_DEBUG,"the bug is the PAL ,BUT THE PAL AND SECAM FLAG NO DETECT\n");
            if (ucVdc_Curr_Mode == ZPAL_I)
                return 0;
            else
            {
                ucVdc_Curr_Mode = ZPAL_I;
                return ZPAL_I;
            }
        }

        if ((ucAutoMode==tv_ZNTSC)&&(ucMenuMode==tv_ZNTSC)&&(color_burst_detect==0x00)
                &&(noise_value>0x20)&&(noise_value<0x70)&&(auto_flag!=tv_AUTO))
        {
            VDC_PRINTF(LOGGER_DEBUG,"the bug is the ntsc the no color burst ,do not  mode detect\n");
            if (ucVdc_Curr_Mode == ZNTSC)
                return 0;
            else
            {
                ucVdc_Curr_Mode = ZNTSC;
                return ZNTSC;
            }
        }

        if ((ucAutoMode==tv_ZNTSC)&&(ucMenuMode==tv_ZNTSC)&&(color_burst_detect==0x00)
                &&(average_color_magnitude<0x10)&&(noise_value<0x10)&&(auto_flag!=tv_AUTO))
        {
            VDC_PRINTF(LOGGER_DEBUG,"the bug is the ntsc the no color burst ,do not  mode detect 111111\n");
            if (ucVdc_Curr_Mode == ZNTSC)
                return 0;
            else
            {
                ucVdc_Curr_Mode = ZNTSC;
                return ZNTSC;
            }
        }

        if ((ucMenuMode==tv_ZNTSC)&&(ucAutoMode==tv_ZNTSC_443)&&(mn_status==1)&&(auto_flag!=tv_AUTO))
        {
            drvif_tuner_init(_SIG_MODE_NTSC);
            drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_MN);
            fw_timer_event_DelayXms(2);
            VDC_PRINTF(LOGGER_DEBUG,"the bug is the ntsc ==>ntsc443,do not  mode detect 00000\n");
            if (ucVdc_Curr_Mode == ZNTSC)
                return 0;
            else
            {
                ucVdc_Curr_Mode = ZNTSC;
                return ZNTSC;
            }
        }

        if ((ucMenuMode==tv_ZNTSC)&&(ucAutoMode==tv_ZNTSC_443)&&(auto_flag!=tv_AUTO))
        {
            fw_timer_event_DelayXms(20);  //wait the color of vd stable
            color_burst_detect=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
            VDC_PRINTF(LOGGER_DEBUG,"the vd curr mode=ntsc the auto mode=ntsc443VDC_VIDEO_STATUS_REG1_0x5008=%x\n",color_burst_detect);
            color_burst_detect=color_burst_detect&0x08;
            if ((color_burst_detect!=0x08))
            {
                rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000004);
                drvif_tuner_init(_SIG_MODE_PAL);
                drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_BG);
                fw_timer_event_DelayXms(6); //wait the color of vd stable
                color_burst_detect=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
                VDC_PRINTF(LOGGER_DEBUG,"the vd fix ntsc443 VDC_VIDEO_STATUS_REG1_0x5008=%x\n",color_burst_detect);
                color_burst_detect=color_burst_detect&0x08;
                Audio_AtvGetMNStatus(&mn_status, NULL);

                if ((color_burst_detect==0x08)&&(mn_status==0))
                {
                    VDC_PRINTF(LOGGER_DEBUG,"the bug is the ntsc ==>ntsc443\n");
                    if (ucVdc_Curr_Mode == ZNTSC_443)
                        return 0;
                    else
                    {
                        ucVdc_Curr_Mode = ZNTSC_443;
                        return ZNTSC_443;
                    }
                }
                else
                {
                    rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000000);
                    drvif_tuner_init(_SIG_MODE_NTSC);
                    drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_MN);

                    if (ucVdc_Curr_Mode == ZNTSC)
                        return 0;
                    else
                    {
                        ucVdc_Curr_Mode = ZNTSC;
                        return ZNTSC;
                    }
                }
            }
            else
            {
                if (ucVdc_Curr_Mode == ZNTSC)
                    return 0;
                else
                {
                    ucVdc_Curr_Mode = ZNTSC;
                    return ZNTSC;
                }
            }
        }

//*************************AUTO DETECT FINE THE MODE****************************
        drvif_tuner_init(_SIG_MODE_PAL);
        drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_DK);
        fw_timer_event_DelayXms(1);  // wait the vd stable
        VDC_PRINTF(LOGGER_DEBUG,"auto mode is on  the tuner set the pal i,the mode detect ???\n");

        if ((drvif_tuner_is_locked(&afc)==_TRUE) && (drv_video_vdc_CheckHVisLocked()))
        {
            VDC_PRINTF(LOGGER_DEBUG,"auto mode is on and  tuner set pal i ,the vd and the tuner is lock\n");
            rtdf_outl(VDC_MANUAL_MODE_0x51c0, 0x06);
            fw_timer_event_DelayXms(1);// wait the vd stable
            noise_value= drv_video_vdc_NoiseStatus();
            ucAutoMode = (UINT8)(rtdf_inl(VDC_MODE_DETECTION_STATUS_0x51c4)&0x7);
            flag625=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG3_0x5010);
            PAL_M_FLAG=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG3_0x5010);
            VDC_PRINTF(LOGGER_DEBUG,"VDC_VIDEO_STATUS_REG3_0x5010=%x\n",PAL_M_FLAG);
            VDC_PRINTF(LOGGER_DEBUG,"tuner set the pal  the mode detect=%x?\n",ucAutoMode);
            VDC_PRINTF(LOGGER_DEBUG,"tuner set the pal  the mode detect noise=%x?\n",noise_value);

            if ((flag625&0x04)==0x04)
            {
                if ((ucAutoMode == tv_ZPAL_I)&&(noise_value<=0x30))
                {	//PAL I
                    VDC_PRINTF(LOGGER_DEBUG,"auto mode is on ,the mode detect is pal i\n");
                    return ZPAL_I;
                }
                else
                {
                    fw_timer_event_DelayXms(4);// wait the audio stable
                    Audio_AtvGetMNStatus(&mn_status, NULL);

                    if ((ucAutoMode == tv_ZPAL_I)&&(noise_value>0x31)&&(noise_value<=0x80))
                    {	//PAL I or others
                        color_burst_detect=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);

                        if (((mn_status==0)||((color_burst_detect&0x08)==0x08))&&((PAL_M_FLAG&0X03)==0X01))
                        {
                            VDC_PRINTF(LOGGER_DEBUG,"auto mode is on ,the mode detect is pal i . noise is big ,the color detect\n");
                            return ZPAL_I;
                        }
                        else
                        {
                            rtdf_outl(VDC_MANUAL_MODE_0x51c0, 0x03);
                            drvif_tuner_init(_SIG_MODE_NTSC);
                            drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_MN);
                            fw_timer_event_DelayXms(4); // wait the noise of vd  stable
                            noise_value = drv_video_vdc_NoiseStatus();
                            ucAutoMode = (UINT8)(rtdf_inl(VDC_MODE_DETECTION_STATUS_0x51c4)&0x7);
                            Audio_AtvGetMNStatus(&mn_status, NULL);
                            VDC_PRINTF(LOGGER_DEBUG,"checek the  ZPAL N AUDIO TONE=%x\n",mn_status);

                            if ((ucAutoMode == tv_ZPAL_N)&&(mn_status==1))
                            {	//PAL-N
                                VDC_PRINTF(LOGGER_DEBUG,"auto mode is on ,the mode detect is PAL-N\n");
                                return ZPAL_N;
                            }
                        }
                    }
                }

                if ((ucAutoMode == tv_ZSECAM)&&(noise_value<=0x30))
                {	//secam
                    rtdf_outl(VDC_MANUAL_MODE_0x51c0, 0x07);
                    VDC_PRINTF(LOGGER_DEBUG,"auto mode is on ,the mode detect is secam-1\n");
                    return ZSECAM;
                }

                if (ucAutoMode == tv_ZNTSC_50)
                {	//ntsc50
                    noise_value = drv_video_vdc_NoiseStatus();
                    color_magnitude=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG2_0x500c);

                    if (((color_magnitude&0x08)==0x00)&&(noise_value<0x12))
                    {
                        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000006);
                        VDC_PRINTF(LOGGER_DEBUG,"ntsc 50 error and no color_magnitude ,the tuner set pal . the mode detect fix pal\n");
                        return ZPAL_I;
                    }

                    rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000000);
                    drvif_tuner_init(_SIG_MODE_NTSC);
                    drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_MN);
                    fw_timer_event_DelayXms(2); // wait the mode of vd stable
                    checkntsc50= (UINT8)(rtdf_inl(VDC_MODE_DETECTION_STATUS_0x51c4)&0x07);
                    VDC_PRINTF(LOGGER_DEBUG,"check ntsc50 value=%x\n",checkntsc50);

                    if (checkntsc50==tv_ZNTSC_50)
                    {
                        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000006);
                        drvif_tuner_init(_SIG_MODE_PAL);
                        drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_DK);
                        fw_timer_event_DelayXms(2); //set the  tuner mode
                        VDC_PRINTF(LOGGER_DEBUG,"ntsc 50 error ,the tuner set ntsc50\n");
                        return ZPAL_I;
                    }

                    if (checkntsc50==tv_ZPAL_I)
                    {
                        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000006);
                        drvif_tuner_init(_SIG_MODE_PAL);
                        drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_DK);
                        fw_timer_event_DelayXms(2);//set the  tuner mode
                        VDC_PRINTF(LOGGER_DEBUG,"ntsc 50 error ,the tuner set pal\n");
                        return ZPAL_I;
                    }

                    if (checkntsc50==tv_ZSECAM )
                    {
                        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000007);
                        drvif_tuner_init(_SIG_MODE_PAL);
                        drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_DK);
                        fw_timer_event_DelayXms(2);//set the  tuner mode
                        VDC_PRINTF(LOGGER_DEBUG,"ntsc 50 error ,the tuner set secam\n");
                        return ZSECAM;
                    }
                }

                rtdf_outl(VDC_MANUAL_MODE_0x51c0, 0x07);
                fw_timer_event_DelayXms(1);
                noise_value = drv_video_vdc_NoiseStatus();
                ucAutoMode = (UINT8)(rtdf_inl(VDC_MODE_DETECTION_STATUS_0x51c4)&0x7);

                if ((ucAutoMode == tv_ZSECAM )&&(noise_value<=0x30))
                {	//SECAM
                    VDC_PRINTF(LOGGER_DEBUG,"auto mode is on ,the mode detect is secam-2\n");
                    return ZSECAM;
                }
            }
            else
            {
                if (ucAutoMode == tv_ZNTSC)
                {
                    rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000000);
                    drvif_tuner_init(_SIG_MODE_NTSC);
                    drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_MN);
                    fw_timer_event_DelayXms(2); //set the  tuner mode
                    VDC_PRINTF(LOGGER_DEBUG,"auto mode is on ,the mode detect is pal but mode=ntsc  and the VDC_VIDEO_STATUS_REG2_VADDR=%x\n",color_magnitude);
                    VDC_PRINTF(LOGGER_DEBUG,"auto mode is on the tuner set pal i the mode=ntsc ,the mode detect is NTSC\n");
                    return ZNTSC;
                }

                if (ucAutoMode == tv_ZNTSC_443)
                {	//ntsc443
                    drvif_tuner_init(_SIG_MODE_PAL);
                    drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_DK);
                    fw_timer_event_DelayXms(8);//wait the color of vd stable
                    rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000004);
                    color_burst_detect=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
                    color_magnitude=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG2_0x500c);
                    VDC_PRINTF(LOGGER_DEBUG,"VDC_VIDEO_STATUS_REG1_0x5008=%x\n",color_burst_detect);
                    VDC_PRINTF(LOGGER_DEBUG,"VDC_VIDEO_STATUS_REG2_VADDR=%x\n",color_magnitude);
                    rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000000);
                    drvif_tuner_init(_SIG_MODE_NTSC);
                    drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_MN);
                    fw_timer_event_DelayXms(2);
                    checkntsc50= (UINT8)(rtdf_inl(VDC_MODE_DETECTION_STATUS_0x51c4)&0x7);
                    VDC_PRINTF(LOGGER_DEBUG,"tuners set pal the mode detect the ntsc443 ,after set the ntsc and mode detect=%x\n",checkntsc50);

                    if (checkntsc50==tv_ZNTSC)
                    {
                        VDC_PRINTF(LOGGER_DEBUG,"ntsc443 error the mode detect set the ntscl\n");
                        return ZNTSC;
                    }

                    if ((color_burst_detect&0x08)==0x08)
                    {
                        VDC_PRINTF(LOGGER_DEBUG,"ntsc443 detect is ok ok ok\n");
                        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000004);
                        drvif_tuner_init(_SIG_MODE_PAL);
                        drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_DK);
                        fw_timer_event_DelayXms(8);//set the  tuner mode
                        return ZNTSC_443;
                    }
                }
                VDC_PRINTF(LOGGER_DEBUG,"auto test ucAutoMode =%x.......... irene\n",ucAutoMode);

#ifdef  CONFIG_SUPPORT_PAL60
                if (ucAutoMode == tv_ZPAL_60)
                {	//pal60
                    rtdf_outl(VDC_MANUAL_MODE_0x51c0, 0x00000005);
                    fw_timer_event_DelayXms(6); //wait the noise of vd stable
                    noise_value= drv_video_vdc_NoiseStatus();
                    ucAutoMode = (UINT8)(rtdf_inl(VDC_MODE_DETECTION_STATUS_0x51c4)&0x7);
                    Audio_AtvGetMNStatus(&mn_status, NULL);
                    VDC_PRINTF(LOGGER_DEBUG,"MN carrier lock = %x \n",mn_status);

                    if ((ucAutoMode == tv_ZPAL_60)&&(mn_status==0))
                    {	//PAL-60
                        VDC_PRINTF(LOGGER_DEBUG," PAL60 DETECT\n");
                        return ZPAL_60;
                    }
                }
#endif
            }

            rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000000);
            drvif_tuner_init(_SIG_MODE_NTSC);
            drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_MN);
            fw_timer_event_DelayXms(1); //wait the noise of vd stable
            VDC_PRINTF(LOGGER_DEBUG," auto mode is on  the tuner set the ntsc,the mode detect?\n");
            VDC_PRINTF(LOGGER_DEBUG," auto mode is on and  tuner set ntsc,the vd and the tuner is lock\n");
            rtdf_outl(VDC_MANUAL_MODE_0x51c0, 0x00);
            fw_timer_event_DelayXms(1); //wait the noise of vd stable
            noise_value = drv_video_vdc_NoiseStatus();
            ucAutoMode = (UINT8)(rtdf_inl(VDC_MODE_DETECTION_STATUS_0x51c4)&0x7);
            flag625=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG3_0x5010);
            VDC_PRINTF(LOGGER_DEBUG," tuner set the ntsc  the mode detect=%x ? \n",ucAutoMode);
            VDC_PRINTF(LOGGER_DEBUG," tuner set the ntsc the mode detect noise=%x ? \n",noise_value);

            if ((flag625&0x04)==0x00)
            {
                if ((ucAutoMode == tv_ZNTSC)&&(noise_value<=0x30))
                {	//NTSC
                    fw_timer_event_DelayXms(1); //wait the color of vd stable
                    color_magnitude=(UINT8)rtdf_inl(VDC_AVG_BURST_MAG_STATUS_0x523c);
                    VDC_PRINTF(LOGGER_DEBUG," auto mode is on ,the mode detect is NTSC and the VDC_VIDEO_STATUS_REG2_VADDR=%x\n",color_magnitude);
                    VDC_PRINTF(LOGGER_DEBUG," auto mode is on ,the mode detect is NTSC\n");
                    return ZNTSC;
                }

                rtdf_outl(VDC_MANUAL_MODE_0x51c0, 0x01);
                fw_timer_event_DelayXms(1);  //wait the noise of vd stable
                noise_value = drv_video_vdc_NoiseStatus();
                ucAutoMode = (UINT8)(rtdf_inl(VDC_MODE_DETECTION_STATUS_0x51c4)&0x7);

                if ((ucAutoMode == tv_ZPAL_M)&&(noise_value<=0x70))
                {	//PAL-M
                    VDC_PRINTF(LOGGER_DEBUG,"auto mode is on ,the mode detect is PAL-M\n");
                    return ZPAL_M;
                }
                else
                {
                    if ((ucAutoMode == tv_ZNTSC_443)&&(noise_value<=0x30))
                    {	//ntsc443
                        rtdf_outl(VDC_MANUAL_MODE_0x51c0, 0x00);
                        VDC_PRINTF(LOGGER_DEBUG,"auto mode is on  the  vd fix palm ,the mode detect is ntsc443\n");
                        return ZNTSC;
                    }
                    else
                        rtdf_outl(VDC_MANUAL_MODE_0x51c0, 0x00);
                }
            }
            else
            {
                if (ucAutoMode == tv_ZPAL_I)
                {
                    rtdf_outl(VDC_MANUAL_MODE_0x51c0, 0x06);
                    //drvif_tuner_init(_SIG_MODE_PAL_DK);
                    drvif_tuner_init(_SIG_MODE_PAL);
                    drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_DK);
                    fw_timer_event_DelayXms(1);	//set the  tuner mode
                    VDC_PRINTF(LOGGER_DEBUG,"tuner set the pal but the mode is pai........... \n");
                    return ZPAL_I;
                }
                rtdf_outl(VDC_MANUAL_MODE_0x51c0, 0x03);
                fw_timer_event_DelayXms(1);	//set the  tuner mode
                noise_value= drv_video_vdc_NoiseStatus();
                ucAutoMode = (UINT8)(rtdf_inl(VDC_MODE_DETECTION_STATUS_0x51c4)&0x7);

                if ((ucAutoMode == tv_ZPAL_N)&&(noise_value<=0x30))
                {	//PAL-N
                    VDC_PRINTF(LOGGER_DEBUG,"auto mode is on ,the mode detect is PAL-N.........\n");
                    return ZPAL_N;
                }
            }
        }
        VDC_PRINTF(LOGGER_DEBUG,"\n *** return 0xff _1\n");
        return  0xff;	//unknow
    }
    else
    {
        VDC_PRINTF(LOGGER_DEBUG,"\n *** return 0 _2\n");
        return 0;	//ok
    }
    return 0;
#endif
}

#endif //#ifndef CONFIG_TUNER_SILICON
#endif

#ifdef _VIDEO_TV_SUPPORT
//#ifdef CONFIG_ENABLE_NTSC_No_Burst_Check
//******************************************************************************
//
// FUNCTION       :  void drvif_module_vdc_SetMode(UINT8 ucMode)
//
// USAGE          	:  Set Video Mode
//
// INPUT         	 	:  	< ucMode > :
//                                { 0 : ntsc
//                                   1 : palm
//                                   2 : ntsc50
//                                   3 : paln
//                                   4 : ntsc443
//                                   5 : pal60
//                                   6 : pali
//                                   7 : secam
//                                   8 : auto mode }
//
// OUTPUT         	:	{ none }
//
// GLOBALS        	:
//
// USED_REGS      :
//
//******************************************************************************
void drv_video_vdc_SetMode(UINT8 ucMode)
{
    ucMode = ucMode & 0x0F;
    /////////////////////////////////////////////////////
    //#define VDC_MANUAL_MODE_VADDR                      (0xb80e21c0)
    /*
    manual_mode_select
    	2:0 R/W Select a mode when bit3 is 0.
    		0: ntsc.
    		1: palm.
    		2: ntsc50.
    		3: paln.
    		4: ntsc443.
    		5: pal60.
    		6: pali.
    		7: secam.
    */
    ////////////////////////////////////////////////////////
    if (ucMode == 0x08)
    {
        //////////////////////////////////
        //No Signal
        //////////////////////////////////
        rtdf_maskl(VDC_MANUAL_MODE_0x51c0, 0x78, 0x80);
        //VDC_PRINTF(LOGGER_DEBUG,"[auto]161vd tv ucMode  ucMode=%x.......... \n",(UINT32)ucMode);
    }
    else
    {
        rtdf_maskl(VDC_MANUAL_MODE_0x51c0, 0x78, ucMode);
        //VDC_PRINTF(LOGGER_DEBUG,"[auto]160vd tv ucMode  ucMode=%x.......... \n",(UINT32)ucMode);
    }
    rtdf_maskl(VDC_VD_BASIC_REGISTER_ENABLE_0x51d4, 0xdf, 0x00);

    switch (ucMode)
    {
    default:
    case 0x08://auto mode
        rtdf_maskl(VDC_VIDEO_CONTROL0_0x5000, 0xf1, 0x00);

#ifdef CONFIG_ENABLE_FHFVBug
#if 0
        fw_timer_event_DelayXms(100);
        mode = drv_video_vdc_ReadMode(VDC_SET);
        if ((mode == VDC_MODE_NTSC)||(mode == VDC_MODE_PALM)
                ||(mode == VDC_MODE_NTSC443)||(mode == VDC_MODE_PAL60))   //input lines = 525
#else
        if (field == VDC_FRAME_RATE_60HZ)
#endif
            FieldVTotal = 263; //(525/2)
        else
            FieldVTotal = 313; //(625/2)
#endif
        break;
    case VDC_MODE_NTSC:
    case VDC_MODE_PALM:
        rtdf_maskl(VDC_VIDEO_CONTROL0_0x5000, 0xf1, 0x00);
#ifdef CONFIG_ENABLE_FHFVBug
        FieldVTotal = 263; //(525/2)
#endif
        break;
    case VDC_MODE_PALN:
    case VDC_MODE_PALI:
    case VDC_MODE_SECAM:
    case VDC_MODE_NTSC50:
        rtdf_maskl(VDC_VIDEO_CONTROL0_0x5000, 0xf1, 0x00);
#ifdef CONFIG_ENABLE_FHFVBug
        FieldVTotal = 313; //(625/2)
#endif
        break;
    case VDC_MODE_NTSC443:
        rtdf_maskl(VDC_VD_BASIC_REGISTER_ENABLE_0x51d4, 0xdf, 0x20);
        rtdf_maskl(VDC_VIDEO_CONTROL0_0x5000, 0xf1, 0x0a);
#ifdef CONFIG_ENABLE_FHFVBug
        FieldVTotal = 263; //(525/2)
#endif
        break;
    case VDC_MODE_PAL60:
        rtdf_maskl(VDC_VD_BASIC_REGISTER_ENABLE_0x51d4, 0xdf, 0x20);
        rtdf_maskl(VDC_VIDEO_CONTROL0_0x5000, 0xf1, 0x0e);
#ifdef CONFIG_ENABLE_FHFVBug
        FieldVTotal = 263; //(525/2)
#endif
        break;
    }
}
//#endif
#endif
//============================//
#ifdef CONFIG_ENABLE_FHFVBug
//============================//
UINT8 drv_video_vdc_GetVHalfTotalLen(UINT16 *VActLen)
{
    UINT16 val1, val2, vrange1, vrange2, temp;
    //UINT8 real_standard;

    val1 = (((rtdf_inl(VDC_MEASURE_FIRST_V_HI_0x53cc) &0x03) <<8) | ((rtdf_inl(VDC_MEASURE_FIRST_V_LOW_0x53d0) &0xFF)));
    val2 = (((rtdf_inl(VDC_MEASURE_SECOND_V_HI_0x53d4) &0x03) <<8) | ((rtdf_inl(VDC_MEASURE_SECOND_V_LOW_0x53d8) &0xFF)));

    if (val2 > val1)
        temp = val2 - val1;
    else
        temp = val1 - val2;
    if (temp >1)
    {
        return _FALSE;
    }

    if (((val2 +val1) < 425)||((val2 +val1) > 725))	// avoid non-standard signal error working..
    {
        return _FALSE;
    }

#if 0
    real_standard = rtdf_inl(VDC_MODE_DETECTION_STATUS_0x51c4) & 0x07;
    if ((real_standard == VDC_MODE_NTSC) ||(real_standard == VDC_MODE_PALM)
            || (real_standard == VDC_MODE_NTSC443) ||(real_standard == VDC_MODE_PAL60))	//input lines = 525
#else
    if (field == VDC_FRAME_RATE_60HZ)
#endif
    {
        vrange1 = 0x0119;	//(564 / 2) lines
        vrange2 = 0x00f7;	//(496 / 2) lines
    }
    else		//input lines = 625
    {
        vrange1 = 0x014c;	//(664 / 2) lines
        vrange2 = 0x012a;	//(596 / 2) lines
    }

    *VActLen = ((val1>val2) ? val1 : val2);
    if ((*VActLen)>vrange1 || (*VActLen<vrange2))
    {
        //set VDC_VSYNC_VBI_LOCKOUT_END_VADDR bit7:1
        if ((rtdf_inl(VDC_VSYNC_VBI_LOCKOUT_END_0x5154) & _BIT7) == 0)
            rtdf_setBits(VDC_VSYNC_VBI_LOCKOUT_END_0x5154, _BIT7);
    }
    else
    {
        //set VDC_VSYNC_VBI_LOCKOUT_END_VADDR bit7:0
        if ((rtdf_inl(VDC_VSYNC_VBI_LOCKOUT_END_0x5154) & _BIT7) == _BIT7)
            rtdf_clearBits(VDC_VSYNC_VBI_LOCKOUT_END_0x5154, _BIT7);
    }
    return TRUE;
}


/**
   CVideoFHFVBug

   For  H /V frequency offset adjust +- 6%, please
   call this function every 20mSec

   @param <void>       { void }
   @return  <void>      { void }

*/
#ifdef CONFIG_ENABLE_FHFVBug
void drv_video_FhBug(void)
{
    UINT8 Data[4];
    UINT32 vd_hdto = 0;
    UINT32 vd_cdto = 0;
    UINT32 Temp_diff = 0;
    UINT8 VD2638_mode = 0;
    UINT32 vd_temp_diff_max = 0;
    UINT32 vd_temp_diff_min = 0;

    VD2638_mode=drv_video_vdc_ReadMode(VDC_SET);
    if (VD2638_mode==0x00)//NTSC
    {
        vd_temp_diff_max = 0x020f6000;
        vd_temp_diff_min = 0x01d18000;

    }
    else		//PAL
    {
        vd_temp_diff_max = 0x0A208000;
        vd_temp_diff_min =0x09F08EAC;	//0x09EA8EAC;
    }

    Data[0] = (UINT8)rtdf_inl(VDC_NOISE_STATUS_0x527c);
#if 1
    if ((Data[0]<0x12 )&&((VD2638_mode==0x00)||(VD2638_mode==0x06)))
#else
    if ((Data[0]<0x12 )&&(field == VDC_FRAME_RATE_60HZ))
#endif
    {
        Data[0] = (UINT8)rtdf_inl(VDC_HDTO_INC_STATUS4_0x5240);
        Data[1] = (UINT8)rtdf_inl(VDC_HDTO_INC_STATUS3_0x5244);
        Data[2] = (UINT8)rtdf_inl(VDC_HDTO_INC_STATUS2_0x5248);
        Data[3] = (UINT8)rtdf_inl(VDC_HDTO_INC_STATUS1_0x524c);
        vd_hdto = (((UINT32)Data[0]) << 24) | (((UINT32)Data[1]) << 16) | (((UINT32)Data[2]) << 8) | Data[3];

        Data[0] = (UINT8)rtdf_inl(VDC_CDTO_INC_STATUS4_0x5250);
        Data[1] = (UINT8)rtdf_inl(VDC_CDTO_INC_STATUS3_0x5254);
        Data[2] = (UINT8)rtdf_inl(VDC_CDTO_INC_STATUS2_0x5258);
        Data[3] = (UINT8)rtdf_inl(VDC_CDTO_INC_STATUS1_0x525c);
        vd_cdto = (((UINT32)Data[0]) << 24) | (((UINT32)Data[1]) << 16) | (((UINT32)Data[2]) << 8) | Data[3];

        Temp_diff = vd_cdto - vd_hdto;

        if ((Temp_diff <= vd_temp_diff_min)||(Temp_diff >= vd_temp_diff_max))
        {
            ucFHFVBug_Counter++;
            if (ucFHFVBug_Counter >= 10)
            {
                ucFHFVBug_Counter = 8;
                cvideo_fvfh_set_1d = _ON;
//				YcSeparationStatus = VDC_YCSEPARATE_DEFAULT;
                module_vdc_1DSet();
                VDC_PRINTF(LOGGER_INFO, "[dto] Set to 1D \n");
            }
        }
        else
        {
            ucFHFVBug_Counter--;
            if (ucFHFVBug_Counter == 0)
            {
                ucFHFVBug_Counter = 3;
                cvideo_fvfh_set_1d = _OFF;
//				YcSeparationStatus = VDC_YCSEPARATE_DEFAULT;
                module_vdc_2DSet();//wjchen, 20091119
                VDC_PRINTF(LOGGER_INFO, "[dto] Set to 2D \n");
            }
        }
        // ROSPrintf("[ucFHFVBug_Counter] %d \n",ucFHFVBug_Counter);

        //ROSPrintf("Temp_diff==%x \n",Temp_diff);
    }
}
#endif
//============================//
#endif

/**
   drvif_module_vdc_ReadSettingMode

   Read VD setting mode or auto mode detection result

   @param                 { result
   					VDC_SET / VDC_DETECT }
   @return   Mode       {  0 : ntsc
                                   1 : palm
                                   2 : ntsc50
                                   3 : paln
                                   4 : ntsc443
                                   5 : pal60
                                   6 : pali
                                   7 : secam }
*/
UINT8 drv_video_vdc_ReadMode(GET_VIDEO_MODE_METHOD result)
{

    if (result == VDC_SET)
        return ((((UINT8)rtdf_inl(VDC_MODE_DETECTION_STATUS_0x51c4))&0x70)>>4);
    else		// VDC_DETECT
        return (((UINT8)rtdf_inl(VDC_MODE_DETECTION_STATUS_0x51c4))&0x07);
}

/**
 Check the video mode type.
 This function will query the video decoder's mode type register.
 Differ which type it is now.
   @param <ucInputSrc>  { Input source }
   @return <ucVdc_Curr_Mode> { The result it detected }

 		BIT 0~1
 		 : 1 60 Hz
 		 : 2 50 Hz
 		BIT 4~7
 		 : 00 PAL B,G,H,I
 		 : 10 NTSC M
 		 : 20 SECAM
 		 : 30 NTSC44
 		 : 40 PAL M
 		 : 50 PAL N
 		 : 60 PAL 60
*/
UINT8 drv_video_vdc_IdentifyColorStandard(void)
{
    UINT8 VD_mode = VDC_MODE_NTSC;
    UINT8 VD_mode_try = VDC_MODE_NTSC;
    UINT8 Data[1];
    UINT8 i;
    UINT8 check_mode_count = 9;
    UINT8 check_mode_threshold = 6;
    UINT8 Flag_NoSignal = 0;
    UINT8 Flag_NTSC = 0;
    UINT8 Flag_PAL_M = 0;
    UINT8 Flag_NTSC_443 = 0;
    UINT8 Flag_PAL_60 = 0;
    UINT8 Flag_PAL_I = 0;
    UINT8 Flag_PAL_N = 0;
    UINT8 Flag_SECAM = 0;
    UINT8 Flag_NTSC_50 = 0;
    UINT8 VCR_counter = 10;
    BOOL VCR_Flag = 0;

    if (vdc_InputSrc >= VDC_SOURCE_TOTAL)
    {
        // Get error vdc number
        VDC_PRINTF(LOGGER_INFO, "[2]Get error vdc_InputSrc in drv_video_detectmode!!\n");
//		VDC_PRINTF(LOGGER_ERROR, "[vdc_InputSrc]%x\n",vdc_InputSrc);
        ucVdc_Curr_Mode = _MODE_NOSIGNAL;
        return ucVdc_Curr_Mode;
    }

    if (vdc_InputSrc != VDC_SOURCE_TV)
    {
        check_mode_count = 9;
        check_mode_threshold = 6;
    }
    else
    {
        check_mode_count = 6;
        check_mode_threshold = 4;
    }

    if (vdc_InputSrc != VDC_SOURCE_TV)
    {
        for (i=0; i<10; i++)
        {
            Data[0] =(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG3_0x5010);
            if (Data[0]&0x10)
                VCR_counter +=2;
            else
                VCR_counter --;
        }
        if (VCR_counter >16)
            VCR_Flag =1;
    }

    Data[0] = drv_video_vdc_NoiseStatus();

    if (((Data[0] >7)||VCR_Flag)&&(vdc_InputSrc == VDC_SOURCE_CVBS))
    {
        rtdf_outl(VDC_FREQ_FLAG_THRESHOLD_0x5218, 0xF0);
        rtdf_outl(VDC_PREFER_MODE_0x51cc, 0x02);
    }
    else
    {
        rtdf_outl(VDC_FREQ_FLAG_THRESHOLD_0x5218, 0x00);
        rtdf_outl(VDC_PREFER_MODE_0x51cc, 0x00);
    }
    rtdf_maskl(VDC_MANUAL_MODE_0x51c0, 0x78, 0x80);

    fw_timer_event_DelayXms(50);

    for (i=0; i<check_mode_count; i++)
    {
        fw_timer_event_DelayXms(20);
        Data[0] = (UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
        if ((Data[0]&_No_Sig) == _No_Sig)
        {
            VD_mode = VDC_MODE_NO_SIGNAL;
        }
        else
        {
            if ((vdc_InputSrc != VDC_SOURCE_TV)||(tvAutoScanFlag==_TRUE)||(tvManualScanFlag==1))
            {
                VD_mode_try = drv_video_vdc_ReadMode(VDC_DETECT);
//				VDC_PRINTF(LOGGER_DEBUG,"[VDC mode %d] detect:%x\n",(UINT32)i,(UINT32)VD_mode_try);
            }
            else
            {
                VD_mode_try = drv_video_vdc_ReadMode(VDC_SET);
                //VDC_PRINTF(LOGGER_DEBUG,"[VDC mode %d] set:%x\n",(UINT32)i,(UINT32)VD_mode_try);
            }
        }
		//RTD_Log(LOGGER_LEVEL,"\n Tim VD_mode_try = %d \n",(UINT32)VD_mode_try);
        switch (VD_mode_try)
        {
        case VDC_MODE_NO_SIGNAL:
            Flag_NoSignal ++;
            break;
        default:
        case VDC_MODE_NTSC:
            Flag_NTSC ++;
            break;
        case VDC_MODE_PALM:
            Flag_PAL_M ++;
            break;
        case VDC_MODE_NTSC443:
            Flag_NTSC_443 ++;
            break;
        case VDC_MODE_PAL60:
            Flag_PAL_60 ++;
            break;
        case VDC_MODE_PALI:
            Flag_PAL_I ++;
            break;
        case VDC_MODE_PALN:
            Flag_PAL_N ++;
            break;
        case VDC_MODE_SECAM:
            Flag_SECAM ++;
            break;
        case VDC_MODE_NTSC50:
            Flag_NTSC_50 ++;
            break;
        }
        
		//RTD_Log(LOGGER_LEVEL,"\n Tim  VD_mode_try = %d ,Flag_PAL_M = %d, Flag_NTSC = %d \n",(UINT32)VD_mode_try,(UINT32)Flag_PAL_M,(UINT32)Flag_NTSC);
    }

    //RTD_Log(LOGGER_LEVEL,"\n Tim1 Flag_PAL_M = %d, Flag_NTSC = %d \n",(UINT32)Flag_PAL_M,(UINT32)Flag_NTSC);

    if (Flag_NoSignal >=check_mode_threshold)
    {
        VD_mode = VDC_MODE_NO_SIGNAL;
    }
    else if (Flag_NTSC >= check_mode_threshold)
    {
        VD_mode = VDC_MODE_NTSC;
    }
    else if (Flag_PAL_M >= check_mode_threshold)
    {
        VD_mode = VDC_MODE_PALM;
    }
    else if (Flag_NTSC_443 >= check_mode_threshold)
    {
        VD_mode = VDC_MODE_NTSC443;
    }
    else if (Flag_PAL_60 >= check_mode_threshold)
    {
        VD_mode = VDC_MODE_PAL60;
    }
    else if (Flag_PAL_I >= check_mode_threshold)
    {
        VD_mode = VDC_MODE_PALI;
    }
    else if (Flag_PAL_N >= check_mode_threshold)
    {
        VD_mode = VDC_MODE_PALN;
    }
    else if (Flag_SECAM >= check_mode_threshold)
    {
        VD_mode = VDC_MODE_SECAM;
    }
    else if (Flag_NTSC_50 >= check_mode_threshold)
    {
        VD_mode = VDC_MODE_NTSC50;
    }
    else
    {
        return  ucVdc_Curr_Mode;;
    }

    Data[0] = (UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
    if ((Data[0]&_No_Sig) == _No_Sig)
    {
        VD_mode = VDC_MODE_NO_SIGNAL;   //set to no signal
    }

    rtdf_maskl(VDC_VD_BASIC_REGISTER_ENABLE_0x51d4, 0xFD,0x00);

    switch (VD_mode)
    {
    case VDC_MODE_NO_SIGNAL:
        ucVdc_Curr_Mode = _MODE_NOSIGNAL;
        rtdf_maskl(VDC_VD_BASIC_REGISTER_ENABLE_0x51d4, 0xFD,0x00);
        rtdf_outl(VDC_DC_VACTIVE_START_0x5484, 0x22);
        rtdf_outl(VDC_VACTIVE_HEIGHT_0x5144, 0x61);
        field = VDC_FRAME_RATE_60HZ;
        break;
    case VDC_MODE_NTSC:
        ucVdc_Curr_Mode = ZNTSC;
        rtdf_maskl(VDC_VD_BASIC_REGISTER_ENABLE_0x51d4, 0xFD,0x00);
        rtdf_outl(VDC_DC_VACTIVE_START_0x5484, 0x22);
        rtdf_outl(VDC_VACTIVE_HEIGHT_0x5144, 0x61);
        field = VDC_FRAME_RATE_60HZ;
        break;
    case VDC_MODE_PALM:
        ucVdc_Curr_Mode = ZPAL_M;
        rtdf_maskl(VDC_VD_BASIC_REGISTER_ENABLE_0x51d4, 0xFD,0x00);
        rtdf_outl(VDC_DC_VACTIVE_START_0x5484, 0x22);
        rtdf_outl(VDC_VACTIVE_HEIGHT_0x5144, 0x61);
        field = VDC_FRAME_RATE_60HZ;
        break;
    case VDC_MODE_NTSC443:
        ucVdc_Curr_Mode = ZNTSC_443;
        rtdf_maskl(VDC_VD_BASIC_REGISTER_ENABLE_0x51d4, 0xFD,0x00);
        rtdf_outl(VDC_DC_VACTIVE_START_0x5484, 0x22);
        rtdf_outl(VDC_VACTIVE_HEIGHT_0x5144, 0x61);
        field = VDC_FRAME_RATE_60HZ;
        break;
    case VDC_MODE_PAL60:
        ucVdc_Curr_Mode = ZPAL_60;
        rtdf_maskl(VDC_VD_BASIC_REGISTER_ENABLE_0x51d4, 0xFD,0x00);
        rtdf_outl(VDC_DC_VACTIVE_START_0x5484, 0x22);
        rtdf_outl(VDC_VACTIVE_HEIGHT_0x5144, 0x61);
        field = VDC_FRAME_RATE_60HZ;
        break;
    case VDC_MODE_PALI:
        ucVdc_Curr_Mode = ZPAL_I;
        rtdf_maskl(VDC_VD_BASIC_REGISTER_ENABLE_0x51d4, 0xFD,0x02);
        rtdf_outl(VDC_DC_VACTIVE_START_0x5484, 0x27);
        rtdf_outl(VDC_VACTIVE_HEIGHT_0x5144, 0xC1);
        field = VDC_FRAME_RATE_50HZ;
        break;
    case VDC_MODE_PALN:
        ucVdc_Curr_Mode = ZPAL_N;
        rtdf_maskl(VDC_VD_BASIC_REGISTER_ENABLE_0x51d4, 0xFD,0x02);
        rtdf_outl(VDC_DC_VACTIVE_START_0x5484, 0x27);
        rtdf_outl(VDC_VACTIVE_HEIGHT_0x5144, 0xC1);
        field = VDC_FRAME_RATE_50HZ;
        break;
    case VDC_MODE_SECAM:
        ucVdc_Curr_Mode = ZSECAM;
        rtdf_maskl(VDC_VD_BASIC_REGISTER_ENABLE_0x51d4, 0xFD,0x00);
        rtdf_outl(VDC_DC_VACTIVE_START_0x5484, 0x2A);
        rtdf_outl(VDC_VACTIVE_HEIGHT_0x5144, 0xC1);
        field = VDC_FRAME_RATE_50HZ;
        break;
    case VDC_MODE_NTSC50:
        ucVdc_Curr_Mode = ZNTSC_50;
        rtdf_maskl(VDC_VD_BASIC_REGISTER_ENABLE_0x51d4, 0xFD,0x00);
        rtdf_outl(VDC_DC_VACTIVE_START_0x5484, 0x2A);
        rtdf_outl(VDC_VACTIVE_HEIGHT_0x5144, 0xC1);
        field = VDC_FRAME_RATE_50HZ;
        break;
    }

//#ifdef CONFIG_SUPPORT_PAL60
    if (VD_mode==VDC_MODE_PAL60)
        rtdf_maskl(VDC_VSYNC_TIME_CONST_0x515c,~_BIT6,_BIT6);
    else
        rtdf_maskl(VDC_VSYNC_TIME_CONST_0x515c,~_BIT6,0x00);
//#endif

    return  ucVdc_Curr_Mode;
}

/**
   drvif_module_vdc_CheckHVisLocked
   Check whether signal locked or not
   Check video decoder's if input signal stable when TV input.
   This function will query the video decoder's status register.
  If TV input is stable return TRUE, else return FALSE.

   @param <void>       { void }
   @return <UINT8>    { return SLR_SUCCESS if mode is stable,
                                  return SLR_FAIL if mode changed }

*/
UINT8 drv_video_vdc_CheckHVisLocked(void)
{
    UINT8 Status;

    Status = (UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
    if (!(Status & _No_Sig))
    {
        if ((Status & _H_Lock) && (Status & _V_Lock))
        {
            return _VDC_SUCCESS;
        }
    }
    return _VDC_FAIL;
}
#ifdef _VIDEO_TV_SUPPORT
/**
    drvif_module_vdc_new_vlock
    Read noise status
    @param              { void }
    @return              { noise status }
*/
UINT8 drvif_module_vdc_New_Vlock(void)
{
    UINT8  i;
    UINT8  new_vlock_count = 0;
    UINT8  flag;

//	new_vlock_count=0;
    for (i=1; i<=4; i++)
    {
        fw_timer_event_DelayXms(5);
        flag = rtdf_inl(VDC_VIDEO_STATUS_REG2_0x500c);
        flag = flag & _BIT6;
        if (flag == _BIT6)
            new_vlock_count++;
        else
        {
            if (new_vlock_count <= 0)
                new_vlock_count = 0;
            else
                new_vlock_count--;
        }
    }

    if (new_vlock_count >= 2)
        return 1;
    else
        return 0;
}
#endif
/**
   drv_video_vdc_Exist
   Check VDC signal is the same or not

   @param <ucInputSrc>   { Input source }
   @return <UINT8>          { return TRUE if mode is stable,
                                         return FALSE if mode changed }

*/
UINT8 drv_video_vdc_Exist(UINT8* ucMode)
{
    UINT8 status;
    UINT8 firstIN;

    if (vdc_InputSrc >= VDC_SOURCE_TOTAL)
    {
        // Get error vdc number
        VDC_PRINTF(LOGGER_ERROR, "[3]Get error vdc_InputSrc in drv_video_detectmode!!\n");
//		VDC_PRINTF(LOGGER_ERROR, "[vdc_InputSrc]%x\n",(UINT32)vdc_InputSrc);
        return _FALSE;
    }

    status = (UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
    firstIN = 1;
    while ((status & _H_Lock) && (status & _V_Lock))
    {
        fw_timer_event_DelayXms(300); // wait for signal stable
        drv_video_vdc_IdentifyColorStandard();
        if (firstIN)
        {
            ucVdc_Pre_Mode = ucVdc_Curr_Mode;
            firstIN = 0;
            continue;
        }

        if (ucVdc_Pre_Mode == ucVdc_Curr_Mode)
        {
            *ucMode = ucVdc_Curr_Mode;
            return _TRUE;
        }
        else
        {
            ucVdc_Pre_Mode = ucVdc_Curr_Mode;
            drv_video_vdc_IdentifyColorStandard();
            if (ucVdc_Pre_Mode == ucVdc_Curr_Mode)
            {
                *ucMode = ucVdc_Curr_Mode;
                return _TRUE;
            }
            return _FALSE;	// no signal
        }
        status = (UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
    }
    return _FALSE;
}
#ifdef CONFIG_SCART_AUTO_SWITCH
void drv_video_FullSCART_func_initial(void)
{
    SCART_Type = _SCART_AV;
    pre_SCART_Type = 0xff;
    SCART_Counter = 0x00;
    SvNoBurstMark = 1;
    ///// initial RGB adc gain = 0.5 /////
    rtdf_maskl(ADC_GAIN0_0x0424,0xfc00fc00,0x03000300);
    rtdf_maskl(ADC_GAIN1_0x0428,0xfffffc00,0x00000300);
    ///// initial RGB adc gain = 0.5 /////
    rtdf_outl(VDC_FSW_GAIN_CTRL_0x5338, 0x80);		// Enable FSW full swing
    rtdf_maskl(VDC_VD_SCT_0x5794, ~_BIT0, _BIT0);	// Enable 3bit ADC
    rtdf_outl(VDC_CHROMA_SATURATION2_ADJ_0x532c, 0x96);	// Cr default setting
    rtdf_outl(VDC_CB_SAT_ADJ_0x5330, 0x60);				// Cb default setting
    rtdf_maskl(VDC_FSW_CTRL2_0x531c, ~_BIT0, _BIT0);	// set SCART display depends on FSW
}

void drv_video_SCART_RGB_set(bit on_off)
{
    if (on_off)
        rtdf_setBits(VDC_FSW_CTRL2_0x531c, _BIT7);	// Enable SCART function
    else
        rtdf_clearBits(VDC_FSW_CTRL2_0x531c,_BIT7);	// Disable SCART function
}

void drv_video_SCART_func_check(void)
{
    // Check AV or RGB
//	VDC_PRINTF(LOGGER_DEBUG,"[SCART][SCART_Type:%x] \n",(UINT32)SCART_Type);
//	VDC_PRINTF(LOGGER_DEBUG,"[SCART][pre_SCART_Type:%x] \n",(UINT32)pre_SCART_Type);
//	VDC_PRINTF(LOGGER_DEBUG,"[SCART][SCART_Counter:%x] \n",(UINT32)SCART_Counter);
//	VDC_PRINTF(LOGGER_DEBUG,"[SCART][SvNoBurstMark:%x] \n",(UINT32)SvNoBurstMark);
    if ((rtdf_inl(VDC_FSW_INPUT_STATUS_0x533c)&0x0f)>=0x04)
    {
        if (SCART_Type != _SCART_RGB)
        {
            VDC_PRINTF(LOGGER_DEBUG, "[SCART RGB]\n");
            drv_video_SCART_RGB_set(_ON);	// Enable SCART function
            SCART_Type = _SCART_RGB;
        }
    }
    else
    {
        if (SCART_Type == _SCART_RGB)
            drv_video_SCART_RGB_set(_OFF);		// Disable SCART function

        if (SCART_Type != pre_SCART_Type)
            SCART_Counter = 0x00;

        if ((SCART_Type == _SCART_AV) && (pre_SCART_Type == _SCART_AV))
        {
//			VDC_PRINTF(LOGGER_DEBUG, "check is AV and C is locked = %x\n",(UINT32)(rtdf_inl(VDC_AVG_BURST_MAG_STATUS_0x523c)));
            if ((UINT8)rtdf_inl(VDC_AVG_BURST_MAG_STATUS_0x523c) < 4)	// burst unlocked
            {
                SCART_Counter ++;
//				VDC_PRINTF(LOGGER_DEBUG, "[AV]SCART_Counter:%x\n",(UINT32)SCART_Counter);
            }
        }

        if (rtdf_inl(VDC_VIDEO_CONTROL1_0x5004)&_BIT5)	//now is set SV
        {
//			VDC_PRINTF(LOGGER_DEBUG, "Is SV, burst mag.:%x\n",(UINT32)rtdf_inl(VDC_AVG_BURST_MAG_STATUS_0x523c));
            if ((UINT8)rtdf_inl(VDC_AVG_BURST_MAG_STATUS_0x523c) > 4)	// burst locked
            {
                SvNoBurstMark = 0;
                SCART_Counter = 0x00;
                SCART_Type = _SCART_SV;
//				VDC_PRINTF(LOGGER_DEBUG, "[SV] reset all\n");
            }
            else
            {
                SvNoBurstMark = 1;
                SCART_Type = _SCART_AV;
                rtdf_maskl(VDC_VIDEO_CONTROL1_0x5004, ~_BIT5, _ZERO);	// set AV
                rtdf_maskl(VDC_CHROMA_LOOPFILTER_STATE_0x50a0, 0xf1, 0x04);	//cstate set 2
                VDC_PRINTF(LOGGER_DEBUG, "[SV --> AV] reset\n");
            }
        }
        else    //now is set AV
        {
//			VDC_PRINTF(LOGGER_DEBUG, "Is AV, burst mag.:%x\n",(UINT32)rtdf_inl(VDC_AVG_BURST_MAG_STATUS_0x523c));
            if ((UINT8)rtdf_inl(VDC_AVG_BURST_MAG_STATUS_0x523c) > 4)	// burst locked
            {
                if (SvNoBurstMark == 0)
                {
//					VDC_PRINTF(LOGGER_DEBUG, "[AV] change mode, SvNoBurstMark:%x\n",(UINT32)SvNoBurstMark);
                    pre_SCART_Type = 0xff;      // need to change mode
                }
                SvNoBurstMark = 1;
                SCART_Counter = 0x00;
                SCART_Type = _SCART_AV;
//				VDC_PRINTF(LOGGER_DEBUG, "[AV], c locked, SvNoBurstMark:%x\n",(UINT32)SvNoBurstMark);
            }
            else
            {
//				VDC_PRINTF(LOGGER_DEBUG, "[AV], c Un locked !!!! SvNoBurstMark:%x\n",(UINT32)SvNoBurstMark);
                if (SCART_Counter >= cSCART_constant)
                {
                    SCART_Counter = 0x00;
                    WaitFor_DEN_STOP();
                    rtdf_maskl(VDC_VIDEO_CONTROL1_0x5004, ~_BIT5, _BIT5);	// set SV
                    VDC_PRINTF(LOGGER_DEBUG, "[AV --> SV] check 1, reg.5004:%x\n",(UINT32)rtdf_inl(VDC_VIDEO_CONTROL1_0x5004));
                    fw_timer_event_DelayXms(10);
                    if ((UINT8)rtdf_inl(VDC_AVG_BURST_MAG_STATUS_0x523c) > 4)
                    {
                        SCART_Type = _SCART_SV;
                        SvNoBurstMark = 0;
                        drv_video_SCART_RGB_set(_OFF);		// Disable SCART function
                        rtdf_maskl(VDC_YC_SEP_CONTROL_0x5280, 0xfc, 0x03);		// set 1D
                        rtdf_maskl(VDC_CHROMA_LOOPFILTER_STATE_0x50a0, 0xf1, 0x04);	//cstate set 2
                        VDC_PRINTF(LOGGER_DEBUG, "[AV --> SV]\n");
                    }
                    else
                    {
                        WaitFor_DEN_STOP();
                        fw_timer_event_DelayXms(10);
                        rtdf_maskl(VDC_VIDEO_CONTROL1_0x5004, ~_BIT5, _ZERO);	// set AV
//						VDC_PRINTF(LOGGER_DEBUG, "reset to AV, reg.5004:%x\n",(UINT32)rtdf_inl(VDC_VIDEO_CONTROL1_0x5004));
                    }
                }
                if (SvNoBurstMark == 1)
                {
                    SCART_Type = _SCART_AV;
//					VDC_PRINTF(LOGGER_DEBUG, "Still set to AV\n");
                }
                else
                {
                    drv_video_SCART_RGB_set(_OFF);		// Disable SCART function
                    SCART_Type = _SCART_SV;
//					VDC_PRINTF(LOGGER_DEBUG, "[1][AV --> SV]\n");
                }
            }
        }
    }
//	VDC_PRINTF(LOGGER_DEBUG, "[SCART_Type]%x, preset:%x\n",(UINT32)SCART_Type,(UINT32)pre_SCART_Type);
}
#endif
/**
  fw_video_checkmode
   Check mode whether changed or not
 Check video decoder's input signal stable.
 This function will query the video decoder's status register.
 If the signal is stable return _SUCCESS, else return FALSE.

   @param <src>        { Select signal to show }
   @return <UINT8>    { return 1 if mode is changed, otherwise 0 }
   @return <UINT8>    { return _SUCCESS if mode is stable,
                                  return FALSE if mode changed }
*/
UINT8 drv_video_checkmode(void)
{
    UINT8 ucLocked = 0;
    UINT8 ucCstate = 0;
    UINT8 VD_mode = 0;

//	RTD_Log(LOGGER_DEBUG, "drv_video_checkmode !\n");
    if (vdc_InputSrc >= VDC_SOURCE_TOTAL)
    {
        // Get error vdc number
        VDC_PRINTF(LOGGER_ERROR, "Get error vdc_InputSrc in drv_video_checkmode!!\n");
        return _FALSE;
    }

    ucLocked = rtdf_readByte0(VDC_VIDEO_STATUS_REG1_0x5008);
    if (ucLocked & _No_Sig)
        return _FALSE; //no signal

    // check c state to 3
    ucCstate = (rtdf_readByte0(VDC_CHROMA_LOOPFILTER_STATE_0x50a0) & 0x0e)>>1;
    if (ucCstate != 3)
    {
        rtdf_maskl(VDC_CHROMA_LOOPFILTER_STATE_0x50a0,0xf1,0x06);	// set cstate to 3
        YcSeparationStatus = VDC_YCSEPARATE_DEFAULT;
        VDC_PRINTF(LOGGER_ERROR, "Set C state to 3\n");
    }

#if 0
    // Progressive scan detected
    Data[0] = rtdf_readByte0(VDC_VIDEO_STATUS_REG2_0x500c);
    if (Data[0] & _BIT0)
    {
        Data[0] = rtdf_readByte0(VDC_VSYNC_TIME_CONST_0x515c);
        if ( (Data[0]&0x0a) != 0x0a )
            rtdf_maskl(VDC_VSYNC_TIME_CONST_0x515c, 0xf5, 0x0a);

        rtdf_outl(VDC_SIGNAL_STM_CONTROL_0x53e4, 0x30);
    }
    else
    {
        rtdf_outl(VDC_SIGNAL_STM_CONTROL_0x53e4, 0x38);
    }

    // Check field rate
    Data[0] = rtdf_readByte0(VDC_VIDEO_STATUS_REG1_0x5008);
#endif

    if ((ucLocked & _HSLCK) && (ucLocked & _VLINF))
    {
        // For osd adjust
        if (GET_SLIDER_ON())
            return _TRUE;
        VD_mode = drv_video_vdc_IdentifyColorStandard();
//		VDC_PRINTF(LOGGER_ERROR, "Curr:%x , Pr:%x\n",(UINT32)ucVdc_Curr_Mode,(UINT32)ucVdc_Pre_Mode);

        if (ucVdc_Pre_Mode != VD_mode)
        {
            VDC_PRINTF(LOGGER_ERROR, "[mode changed]Curr:%x , Pr:%x\n",(UINT32)ucVdc_Curr_Mode,(UINT32)ucVdc_Pre_Mode);
            return 2;	// mode changed
        }
        else
        {
#ifdef CONFIG_SCART_AUTO_SWITCH
            if ((vdc_InputSrc == VDC_SOURCE_FULL_SCART)||(vdc_InputSrc == VDC_SOURCE_HALF_SCART))
            {
                drv_video_SCART_func_check();
                if (pre_SCART_Type != SCART_Type)
                {
                    fw_timer_event_DelayXms(50);
                    YcSeparationStatus = VDC_YCSEPARATE_DEFAULT;
                    drv_video_vdc_YcSeparation();
                    pre_SCART_Type = SCART_Type;
                }
            }
#endif
            return _TRUE;	// pass check mode while VDC locked
        }
    }

    return _FALSE;
}

UINT8 drv_video_detectmode(ModeInformationType* stModeInfo)
{
    if (vdc_InputSrc >= VDC_SOURCE_TOTAL)
    {
        // Get error vdc number
        VDC_PRINTF(LOGGER_ERROR, "[4]Get error vdc_InputSrc in drv_video_detectmode!!\n");
//		VDC_PRINTF(LOGGER_ERROR, "[vdc_InputSrc]%x, total:%x\n",(UINT32)vdc_InputSrc,(UINT32)VDC_SOURCE_TOTAL);
        return _MODE_NOSIGNAL;
    }

    if (drv_video_vdc_Exist(&stModeInfo->ModeCurr))
    {
        // Get IHFreq, IVFreq
        drv_measure_offlinemeasure(_OFFLINE_MS_SRC_VDC, ANALOG_MODE_MEASUREMENT, stModeInfo);
        flow_Get_Preset_Table_For_VDC(vdc_InputSrc, field, stModeInfo);

        if (field == VDC_FRAME_RATE_60HZ)
        {
            stModeInfo->ModeCurr = _MODE_480I;
            VDC_PRINTF(LOGGER_DEBUG,"[VDC] 480i \n");
#ifdef CONFIG_ENABLE_FHFVBug
            //========================//
            FieldVTotal = 263; //(525/2)
            //========================//
#endif
        }
        else
        {
            stModeInfo->ModeCurr = _MODE_576I;
            VDC_PRINTF(LOGGER_DEBUG,"[VDC] 576i \n");
#ifdef CONFIG_ENABLE_FHFVBug
            //========================//
            FieldVTotal = 313; //(625/2)
            //========================//
#endif
        }

        YcSeparationStatus = VDC_YCSEPARATE_DEFAULT;
        stModeInfo->status = _BIT1;

        VDC_PRINTF(LOGGER_DEBUG,"!! VDC_Detect Done !![reg.5008:%x] \n",(UINT32)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008));
#ifdef CONFIG_SCART_AUTO_SWITCH
        if ((vdc_InputSrc == VDC_SOURCE_FULL_SCART)||(vdc_InputSrc == VDC_SOURCE_HALF_SCART))
            drv_video_SCART_func_check();
#endif
        drv_video_vdc_YcSeparation();

#ifdef CONFIG_VBI_CC_SUPPORT
        drvif_vbi_cc_verify_play();
#endif

        return _MODE_SUCCESS;
    }
    else
    {
//		VDC_PRINTF(LOGGER_DEBUG,"!! VDC fail to lock !![reg.5008:%x] \n",(UINT32)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008));
#ifdef CONFIG_SCART_AUTO_SWITCH
        if (vdc_InputSrc == VDC_SOURCE_FULL_SCART)
            drv_video_FullSCART_func_initial();
#endif
        drv_vdc_ResetReg();
        return _MODE_NOSIGNAL;
    }
}

void drv_video_PowerOn(bit on_off)
{
    if (on_off)
    {
        rtdf_outl(VDC_VD_POW_CTRL0_0x5780, 0xff);
        rtdf_setBits(VDC_VDAC_ABIQ1_0x57bc, _BIT6|_BIT7);
        rtdf_setBits(SYS_PLL_27X_0_0x2c64, _BIT6);
        rtdf_setBits(VDC_VD_BANDGAP_CTRL_0x5790, _BIT0); //HPWang
        rtdf_setBits(VDC_VD_SCT_0x5794,_BIT0); //HPWang
    }
    else
    {
        rtdf_outl(VDC_VD_POW_CTRL0_0x5780, 0x00);
        rtdf_clearBits(VDC_VDAC_ABIQ1_0x57bc, (_BIT6|_BIT7));
        rtdf_clearBits(SYS_PLL_27X_0_0x2c64, _BIT6);
        rtdf_clearBits(VDC_VD_BANDGAP_CTRL_0x5790, _BIT0); //HPWang
        rtdf_clearBits(VDC_VD_SCT_0x5794,_BIT0); //HPWang
    }
}

#if 0 // not used
void drv_video_Interrupt_Init(void)
{
    drv_isr_Enable(SYS_ISR_VIDEO, _FALSE);

    // Route select to 0 for video decoder
    drv_isr_SelectRoute(ROUTE_SEL_VIDEO, IRQ_ROUTE_IRQ1);
}

bit drv_video_isr(void)
{
    if (!drv_isr_GetPending(SYS_ISR_VIDEO))
        return RT_ISR_UNHANDLED;

    return RT_ISR_HANDLED;
}
#endif

#ifdef CONFIG_ENABLE_BurstBug
void drv_video_vdc_CheckBurst(UINT32 freq)
{
    if ((freq==497500)||(freq==1842500)||(freq==5032500)||(freq==8072500)||(freq==552500)||((freq>=4710000)&&(freq<=4715000))||((freq>=5370000)&&(freq<=5375000))) //irene@20090527 add for palm
    {
        UINT32 BurstMag;
        UINT32 CagcTarget;
        UINT8 NoiseStatus;
        bit CLockFlag;
        UINT8 VD2638_mode = 0;
        VD2638_mode = drv_video_vdc_ReadMode(VDC_SET);

        //ROSPrintf( "[check burst]  current mode = %x\n", VD2638_mode);
        if ((VD2638_mode!=0x06) && (VD2638_mode!=0x00) && (VD2638_mode!=0x01) && (VD2638_mode!=0x03)) //irene@20090527 add for palm, //wjchen@20090605 add for pal-n
            return;

        BurstMag=((UINT32)rtdf_inl(VDC_AVG_BURST_MAG_STATUS_0x523c) + (UINT32)rtdf_inl(VDC_AVG_BURST_MAG_STATUS_0x523c) +
                  (UINT32)rtdf_inl(VDC_AVG_BURST_MAG_STATUS_0x523c) + (UINT32)rtdf_inl(VDC_AVG_BURST_MAG_STATUS_0x523c) ) >>2;

        //ROSPrintf( "******** BurstMag = : %x***************\n", BurstMag);
        NoiseStatus = drv_video_vdc_NoiseStatus();
        CLockFlag = (bit)(rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008)&0x08
                          || rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008)&0x08
                          || rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008)&0x08
                          || rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008)&0x08);


        //*********************************
        //* set CAGC.. **********************
        //*********************************
        if (BurstMag <= 0x14) //0x10
        {
            if ( CLockFlag) //RF level > 40dB, & //burst level = 40%
            {
                //if(BurstMag >= 2) // ives modified from 2 to 3
                if (BurstMag >= 4) // rayzen 20090105
                {
                    //CagcTarget = ((BurstMag - 3) * 8) + 0x10;
                    //CagcTarget = ((BurstMag - 2) * 8) + 0x20;
                    //CagcTarget = ((BurstMag - 1) * 6) + 0x30; // ives 20080410
                    //CagcTarget = ((BurstMag - 2) * 6) + 0x30; // ives 20081015
                    //CagcTarget = ((BurstMag - 5) * 2) + 0x2c; // rayzen 20090105
                    CagcTarget = (BurstMag * 5) + 0x10; // rayzen 20090109
                    VDC_PRINTF(LOGGER_DEBUG, " low BurstMag.... = : %x***************\n", (UINT32)BurstMag);
                }
                else
                {
                    //CagcTarget = 0x30; // fix cagc for low burst mag.
                    if ((VD2638_mode==0x01) || (VD2638_mode==0x03))//wjchen@20090605 add for pal-n
                        CagcTarget = 0x26; //irene@20090527 add for palm
                    else
                        CagcTarget = 0x2c; // rayzen 20090105??
                }
                rtdf_outl(VDC_CHROMA_AGC_0x5090, CagcTarget);
            }
            else// c unlock
            {
                VDC_PRINTF(LOGGER_DEBUG, "...c unlock\n");
                //rtdf_outl(VDC_CHROMA_AGC_VADDR, 0x30); // ives 20071213
                if ((VD2638_mode==0x01) || (VD2638_mode==0x03))//wjchen@20090605 add for pal-n
                    rtdf_outl(VDC_CHROMA_AGC_0x5090, 0x50); //irene@20090527 add for palm
                else
                    rtdf_outl(VDC_CHROMA_AGC_0x5090, 0x30); // rayzen 20090105, must bigger than 0x2a
            }
        }
        else if (BurstMag > 0x16) //burst> 0x10,  reset cagc 8A  //0x12
        {
            if (rtdf_inl(VDC_CHROMA_AGC_0x5090) != 0x8A)
                rtdf_outl(VDC_CHROMA_AGC_0x5090, 0x8A); // ives added 20071214
            //ROSPrintf("[Burst Mode] reset cagc.........................  \n");
        }
        /*
        else  //burst> 0x10,  reset cagc 8A
        {        if(rtdf_inl(VDC_CHROMA_AGC_VADDR) != 0x8A)
                    rtdf_outl(VDC_CHROMA_AGC_VADDR, 0x8A); // ives added 20071214
             ROSPrintf("[Burst Mode] reset cagc.........................  \n");
        }
        */


        //*********************************************
        // C kill, c new_gate, igain, lose chroma_cnt, colour mode
        //*********************************************

        //if((BurstMag <= 0x10)&&(NoiseStatus>=0x05)) //  ives 20080104
        if ((BurstMag <= 0x10)&&(NoiseStatus>=0x08)) //  rayzen 20090105
        {
            //ROSPrintf("[Burst & Noise Mode] setting.............................  \n");
            //set color mode
            if ((rtdf_inl(VDC_VD_BASIC_REGISTER_ENABLE_0x51d4)&0x20) != 0x20)
            {
                rtdf_maskl(VDC_VD_BASIC_REGISTER_ENABLE_0x51d4, 0xff, 0x20);  //enable reg_colour_mode
                if (VD2638_mode==0x06)
                {
                    rtdf_maskl(VDC_VIDEO_CONTROL0_0x5000, 0xf0, 0x02);  //set Colour_mode = pal
                }
                else if (VD2638_mode==0x00)
                {
                    rtdf_maskl(VDC_VIDEO_CONTROL0_0x5000, 0xf0, 0x00);  //set Colour_mode = ntsc
                }
            }
            // jj stop 20071212

            //set lose_chromalock_cnt
            if (rtdf_inl(VDC_CHROMA_LOCK_CONFIG_0x5098) != 0x00)
                rtdf_outl(VDC_CHROMA_LOCK_CONFIG_0x5098, 0x00);

            if ((rtdf_inl(VDC_CHROMA_KILL_0x5094)&0x0f) != 0x0f)
                rtdf_maskl(VDC_CHROMA_KILL_0x5094, 0xf0, 0x0f);

            // disable cnew_gate & set cagc_igain slower
            rtdf_maskl(VDC_CHROMA_LOOPFILTER_GAIN_0x509c, 0x7c, 0x02); // ives lose chromalock set to 0.
        }
        else // reset C kill, c new_gate, igain, lose chroma_cnt, colour mode
        {
            //ROSPrintf("[Burst & Noise Mode] reset.............................  \n");
            //set color mode
            if ((rtdf_inl(VDC_VD_BASIC_REGISTER_ENABLE_0x51d4)&0x20) != 0x00)
            {
                rtdf_maskl(VDC_VD_BASIC_REGISTER_ENABLE_0x51d4, 0xdf, 0x00);  //enable reg_colour_mode
                rtdf_maskl(VDC_VIDEO_CONTROL0_0x5000, 0xf0, 0x00);  //clear Colour_mode = pal
            }
            // enable cnew_gate & set cagc_igain	faster
            rtdf_maskl(VDC_CHROMA_LOOPFILTER_GAIN_0x509c, 0x7c, 0x80);

            rtdf_maskl(VDC_CHROMA_LOCK_CONFIG_0x5098, 0x00, 0xbb); // modify for Ckill on 30dBuv, 0xbb has color, 0x6b doesn't

            //if(rtdf_inl(VDC_CHROMA_AGC_VADDR) != 0x8A)
            //       rtdf_outl(VDC_CHROMA_AGC_VADDR, 0x8A); // rayzen add reset flow 20090105

            if ((rtdf_inl(VDC_CHROMA_KILL_0x5094)&0x0f) != 0x07)
                rtdf_maskl(VDC_CHROMA_KILL_0x5094, 0xf0, 0x07);
        }// end reset
    }
    else // Other freq. no burst check*********************
    {
        rtdf_maskl(VDC_CHROMA_LOOPFILTER_GAIN_0x509c, 0x7c, 0x80); // set loopgain to default. ives 20071209
        rtdf_maskl(VDC_CHROMA_LOCK_CONFIG_0x5098, 0x00, 0x6b); // set chromalock to default. ives 20071209

        if (rtdf_inl(VDC_CHROMA_AGC_0x5090) != 0x8A)
            rtdf_outl(VDC_CHROMA_AGC_0x5090, 0x8A); // ives added 20071214
        // ROSPrintf("[Burst Mode] reset.............................  \n");
        if ((rtdf_inl(VDC_VD_BASIC_REGISTER_ENABLE_0x51d4)&0x20) != 0x00)
        {
            rtdf_maskl(VDC_VD_BASIC_REGISTER_ENABLE_0x51d4, 0xdf, 0x00);  //enable reg_colour_mode
            rtdf_maskl(VDC_VIDEO_CONTROL0_0x5000, 0xf0, 0x00);  //clear Colour_mode = pal
        }
        if ((rtdf_inl(VDC_CHROMA_KILL_0x5094)&0x0f) != 0x07)
            rtdf_maskl(VDC_CHROMA_KILL_0x5094, 0xf0, 0x07);
    }
}
#endif


#ifdef CONFIG_ENABLE_TV_NOISE_BOX_ENALBE
void drv_vdc_TV_noise_box_delay(UINT16 usNum)
{
    UINT16 count;

    for (count=0; count<usNum; count++)
    {
        flow_timer_Reset_WatchDog();

        fw_timer_event_DelayXms(100);
    }
}

void drvif_module_vdc_noisebox_avoutfunction(bit on_off)
{
    UINT8 Data[1];
    UINT8 mode;
    UINT8 vcr;
    bit  unstable_flag;
    bit  pal_flag;
    UINT8 pal_flag_p_count;
    UINT8 pal_flag_n_count;
    UINT8 i;

    if (on_off)
    {
        pal_flag_p_count = 0x00;
        pal_flag_n_count = 0x00;
        drv_video_vdc_AvOutMode(VDC_AVOUT1, VDC_AVOUT_CVBS_WITH_FIL_WITH_AGC)

        drv_vdc_TV_noise_box_delay(5);		// delay 500 ms
        Data[0]=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
        vcr = (UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG3_0x5010)&0x10;
        mode = drv_video_vdc_ReadMode();
//		VDC_PRINTF(LOGGER_DEBUG, "\n noise box function is eanble ..the vd lock=%x,\n", Data[0]);
//		VDC_PRINTF(LOGGER_DEBUG, "\n noise box function is eanble ..the mode is =%x,\n", mode);
//		VDC_PRINTF(LOGGER_DEBUG, "\n noise box function is eanble ..the vcr flag=%x,\n", vcr);
        switch (mode)
        {
        case 0x07://zsecam
        case 0x06://pal
            if ( vcr==0x10 && (Data[0]&0x06)==0x06)
            {
                for (i=0; i<10; i++)
                {
                    fw_timer_event_DelayXms(10);
                    pal_flag = rtdf_inl(VDC_VIDEO_STATUS_REG3_0x5010)&0x01;
                    if (pal_flag==0x01)
                        pal_flag_p_count++;
                    else
                        pal_flag_n_count++;
                }
//					VDC_PRINTF(LOGGER_DEBUG, "\n noise box function is eanble ..the mode is pal  the pal_flag_p_count=%x,\n",pal_flag_p_count);
//					VDC_PRINTF(LOGGER_DEBUG, "\n noise box function is eanble ..the mode is pal  the pal_flag_n_count =%x,\n",pal_flag_n_count);

                if ( pal_flag_p_count > 8)
                    unstable_flag = 0;
                else
                    unstable_flag=1;
            }
            else
            {
                unstable_flag=0;
            }
            break;

        case 0x00://ntsc
            if ( vcr==0x10 && (Data[0] &0x06)==0x06)
            {
                for (i=0; i<10; i++)
                {
                    fw_timer_event_DelayXms(10);
                    pal_flag = rtdf_inl(VDC_VIDEO_STATUS_REG3_0x5010)&0x01;
                    if (pal_flag == 0x01)
                        pal_flag_p_count++;
                    else
                        pal_flag_n_count++;
                }
//					VDC_PRINTF(LOGGER_DEBUG, "\n noise box function is eanble ..the mode is ntsc  the pal_flag_p_count=%x,\n",pal_flag_p_count);
//					VDC_PRINTF(LOGGER_DEBUG, "\n noise box function is eanble ..the mode is ntsc  the pal_flag_n_count =%x,\n",pal_flag_n_count);

                if ( pal_flag_n_count > 8)
                    unstable_flag=0;
                else
                    unstable_flag=1;
            }
            else
            {
                unstable_flag=0;
            }
            break;

        default:
            unstable_flag=0;
            break;
        }

        if ( (Data[0]&0x06)==0x02 || (Data[0] &0x06)==0x04 || (Data[0] &0x06)==0x00 || unstable_flag == 1 )
        {
            //?? module_vdc_AVoutMode(VDC_AVOUT_TV_NOISE_BOX);
            drv_video_vdc_AvOutMode(VDC_AVOUT1, VDC_AVOUT_CVBS_NO_FIL_NO_AGC);
            flow_timer_Reset_WatchDog();
            SET_NoiseBoxStatus(1);
//			VDC_PRINTF(LOGGER_DEBUG, "\n GET_NoiseBoxStatus =%x \n",GET_NoiseBoxStatus(1));
//			VDC_PRINTF(LOGGER_DEBUG, "\n noise box function is eanble ...the hv is  lock, use the tv noise box av out \n");
        }
        else
        {
            SET_NoiseBoxStatus(0);
//			VDC_PRINTF(LOGGER_DEBUG, "\n noise box function is eanble ...the hv is  lock,  do not use the tv noise box av out \n");
        }
    }
    else
    {
        SET_NoiseBoxStatus(0);
    }
}
#endif


/**
   drv_vdc_ResetReg
   Reset video registers for TV channel change

 This function only execute when TV channel change

   @param <void>       { void }
   @return                  { void }

*/
void drv_vdc_ResetReg(void)
{
    YcSeparationStatus = VDC_YCSEPARATE_DEFAULT;
#ifdef CONFIG_ENABLE_NTSC_No_Burst_Check
    bNTSCnoBurst = 0;
    rtdf_outl(VDC_FLAG_443_COUNTER_0x5204, 0x32);
    rtdf_outl(VDC_FREQ_FLAG_THRESHOLD_0x5218, 0x00);
    rtdf_outl(VDC_FREQ_VALID_MAX_0x521c, 0xc0);
    rtdf_outl(VDC_FREQ_VALID_INNER_0x5220, 0x42);
    rtdf_outl(VDC_FREQ_VALID_MIN_0x5224, 0x50);
#endif
}


/**
    drvif_module_vdc_NTSCnoBurstCheck
    When NTSC have no burst, it could be shake.
    Need to change mode to NTSC443 to avoid this condition.
    Thise function can execute in check mode.

    @param                      { void }
    @return                       { void }
*/
#ifdef CONFIG_ENABLE_NTSC_No_Burst_Check
void drvif_module_vdc_NTSCnoBurstCheck(void)
{
    UINT8 Data[2];

    Data[0] = rtdf_inl(VDC_MANUAL_MODE_0x51c0)&0x87;   // double check mode is set to NTSC
    if (Data[0]==0x00)  // is NTSC
    {
        Data[0] = (UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG2_0x500c)&0x10; // Detect_443_flag
        Data[1] = (UINT8)rtdf_inl(VDC_AVG_BURST_MAG_STATUS_0x523c);
//            VDC_PRINTF(LOGGER_DEBUG,"[jj check_1] Detect 443:%x, burst_meg:%x\n",Data[0],Data[1]);
        if ( bNTSCnoBurst==0 && Data[0]==0x10 && Data[1]<0x03 )
        {
            rtdf_outl(VDC_FLAG_443_COUNTER_0x5204, 0xff);
            rtdf_outl(VDC_FREQ_FLAG_THRESHOLD_0x5218, 0xff);
            rtdf_outl(VDC_FREQ_VALID_MAX_0x521c, 0x00);
            rtdf_outl(VDC_FREQ_VALID_INNER_0x5220, 0xff);
            rtdf_outl(VDC_FREQ_VALID_MIN_0x5224, 0xff);
            bNTSCnoBurst = 1;
            VDC_PRINTF(LOGGER_DEBUG, "[jj set_1] Set no NTSC443 condition\n");
        }
        else if ( bNTSCnoBurst==1 && Data[1]>0x10 )
        {
            rtdf_outl(VDC_FLAG_443_COUNTER_0x5204, 0x32);
            rtdf_outl(VDC_FREQ_FLAG_THRESHOLD_0x5218, 0x00);
            rtdf_outl(VDC_FREQ_VALID_MAX_0x521c, 0xc0);
            rtdf_outl(VDC_FREQ_VALID_INNER_0x5220, 0x42);
            rtdf_outl(VDC_FREQ_VALID_MIN_0x5224, 0x50);
            VDC_PRINTF(LOGGER_DEBUG, "[jj set_2] Reset NTSC443 condition\n");
            bNTSCnoBurst = 0;
        }
        else if ( bNTSCnoBurst==2 && Data[1]>0x10 )
        {
            drv_video_vdc_SetMode(8);   // set to auto mode
            rtdf_outl(VDC_FLAG_443_COUNTER_0x5204, 0x32);
            rtdf_outl(VDC_FREQ_FLAG_THRESHOLD_0x5218, 0x00);
            rtdf_outl(VDC_FREQ_VALID_MAX_0x521c, 0xc0);
            rtdf_outl(VDC_FREQ_VALID_INNER_0x5220, 0x42);
            rtdf_outl(VDC_FREQ_VALID_MIN_0x5224, 0x50);
            VDC_PRINTF(LOGGER_DEBUG, "[jj set_3] Reset NTSC443 condition and set to auto mode\n");
            bNTSCnoBurst = 0;
        }
    }
    else if (Data[0]==0x80)  // is auto mode
    {
        Data[0] = (UINT8)rtdf_inl(VDC_MODE_DETECTION_STATUS_0x51c4);   // double check mode is set to NTSC
        if (Data[0]==0x00)  // is NTSC
        {
            Data[0] = ((UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG2_0x500c)&0x10); // Detect_443_flag
            Data[1] = (UINT8)rtdf_inl(VDC_AVG_BURST_MAG_STATUS_0x523c);
//                VDC_PRINTF(LOGGER_DEBUG,"[jj check_2] Detect 443:%x, burst_meg:%x\n",Data[0],Data[1]);
            if ((bNTSCnoBurst==0)&&(Data[0]==0x10)&&(Data[1]<0x03))
            {
                drv_video_vdc_SetMode(0);   // set to NTSC
                rtdf_outl(VDC_FLAG_443_COUNTER_0x5204, 0xff);
                rtdf_outl(VDC_FREQ_FLAG_THRESHOLD_0x5218, 0xff);
                rtdf_outl(VDC_FREQ_VALID_MAX_0x521c, 0x00);
                rtdf_outl(VDC_FREQ_VALID_INNER_0x5220, 0xff);
                rtdf_outl(VDC_FREQ_VALID_MIN_0x5224, 0xff);
                bNTSCnoBurst = 2;
                VDC_PRINTF(LOGGER_DEBUG, "[jj set_4] Set no NTSC443 condition\n");
            }
        }
    }
}
#endif


#ifdef _VIDEO_TV_SUPPORT

// FHFV bias dclk
void drvif_module_vdc_TV_FHFV_Bias_DCLK_Adjust(void)
{
#if 0 // CF 20090612 removed, need to refine
    UINT64   	offset=0;
    UINT32 		aclkcnt=0,ulFreq;
    UINT16 		nMCode,nNCode;
    UINT8 		div=1,cpc,cnt=0;
//	UINT16 currentIPHeriod;

    rtdf_maskl(IDOMAIN_OFFMS_SP_MS3ENATVCTRL_, ~(_BIT6 | _BIT7), _BIT6);
    rtdf_writeByte1(IDOMAIN_OFFMS_REG18051100_VADDR, 1);
    rtdf_setBits(SYSTEM_SC_PLLDISP0_VADDR, _BIT1 | _BIT2);
    rtdf_setBits(SYSTEM_SC_DCLKSS_VADDR, _BIT0);
    drv_adjust_syncprocessormeasurestart();

    if (ScalerTimer_PollingEventProc(50, drvif_framesync_offlinemeasureendevent))
    {
        rtdf_setBits(IDOMAIN_OFFMS_REG18051100_VADDR, _BIT12);  // off-line measure pop-up
        ScalerTimer_DelayXms(10);
        aclkcnt = rtdf_inl(IDOMAIN_OFFMS_REG1805111C_VADDR) & 0x00ffffff;
        //	currentIPHeriod =  rtdf_readWord0(IDOMAIN_OFFMS_REG18051104_VADDR) >> 4;  // Get Input HSYNC Period Measurement Result
#ifdef CONFIG_DEINTERLACE_CUT_LINE//<Fred 20090210>//for delete 4 lines (Up 2 line and down 2 line)
        if (GET_INTERLACE_MODE(info->display))
        {
            aclkcnt=(aclkcnt)/((info->IPV_ACT_LEN-1)+info->IPH_ACT_WID)*((info->IPV_ACT_LEN-3)+info->IPH_ACT_WID);
            //aclkcnt = aclkcnt /info->IPV_ACT_LEN *(info->IPV_ACT_LEN -2);// for delete 4 lines (Up 2 line and down 2 line)
        }
#endif
        //ROSPrintf("Read DPLL_M=%x, div=%x in FastDo\n", mcode, div);

        if ((aclkcnt == 0)||(_DISP_LEN ==0))
        {
            offset = 0xfff;
            //DEBUG_PRINT("\n aclkcnt =%x  _DISP_LEN = %x !!\n", aclkcnt,_DISP_LEN);
            //DEBUG_PRINT("Fast frame sync OffLineMeasure ERROR : off-line measure status polling time-out!!\n");
            return offset;
        }
    }
    ROSPrintf(" [ framesync_fast_get_sync_point ] aclkcnt =%x !!\n", aclkcnt);

    rtdf_clearBits(IDOMAIN_OFFMS_REG18051100_VADDR, _BIT9|_BIT10);  // clear off-line measureing flag
    ulFreq = ((Get_DISP_HORIZONTAL_TOTAL() +1)*(_DISP_LEN-1)+_DISP_WID)*(CONFIG_CPU_XTAL_FREQ/aclkcnt);

    if ((ulFreq<=234000000) && (ulFreq>=60000000))
        div = 1;
    else if ((ulFreq<=117000000) && (ulFreq>=30000000))
        div=2;
    else if ((ulFreq<=58000000) && (ulFreq>=15000000))
        div=4;
    else if ((ulFreq<=29200000) && (ulFreq>=7500000))
        div=8;

    offset=(UINT64)2*16*div*1000*((Get_DISP_HORIZONTAL_TOTAL() +1)*(_DISP_LEN-1)+_DISP_WID)/(15*aclkcnt);

    for (cnt=3; cnt<=15; cnt++)
    {
        nNCode=cnt;
        nMCode=nNCode*2*offset/1000;
        ////////////////////////////////////
        //四捨五入
        /////////////////////////////////////
        nMCode = ((nMCode >> 1) + (nMCode & 0x01));
        if ((nMCode<=255)&&(nMCode>=3))
            break;
    }
    cpc = drvif_clock_cal_cpc(nMCode,nNCode);

    drvif_clock_set_pll(nMCode, nNCode, div, cpc);
    if (rtdf_readByte0(SYSTEM_SC_PLLDISP0_VADDR)  & _BIT7)
    {
        rtdf_setBits(SYSTEM_SC_PLLDISP0_VADDR, _BIT23);
        ScalerTimer_DelayXms(5);		// I don't know why to delay?
        rtdf_clearBits(SYSTEM_SC_PLLDISP0_VADDR, _BIT23);
    }
    offset =(UINT64)32768-(UINT64)32768*2*div*nNCode*((Get_DISP_HORIZONTAL_TOTAL() +1)*(_DISP_LEN-1)+_DISP_WID)/(nMCode*aclkcnt);

    ROSPrintf("\n [ framesync_fast_get_sync_point ] @@DClock offset1 = %x\n",(UINT16) offset);
    drvif_clock_offset((UINT16) offset);

    rtdf_clearBits(IDOMAIN_OFFMS_SP_MS3ENATVCTRL_VADDR, _BIT6);
    /*if (currentIPHeriod != OriginalIPHeriod){
    	OriginalIPHeriod = currentIPHeriod;
    	return _TRUE;
    }else
    	return _FALSE;*/
#endif
}
#endif
#ifdef CONFIG_SCART_AUTO_SWITCH
void drv_video_SCART_RGB_set_gain(void)
{
    ADCGainOffset ptADCGainOffset;
    RTD_Log(LOGGER_DEBUG, "GET_INPUTSOURCE_TYPE=%d,SCART_Type=%d!\n",(UINT32)GET_INPUTSOURCE_TYPE(),(UINT32)SCART_Type);

    if ((GET_INPUTSOURCE_TYPE()==_SOURCE_FULL_SCART) && (SCART_Type ==_SCART_RGB))
    {
        RTD_Log(LOGGER_DEBUG, "AAAAAAAAAAAAAAAAAA !!\n");
        ui_get_SCART_ADCGain(&ptADCGainOffset);
        drv_video_SCART_ADC_adjust_gain(_SCART_R, ptADCGainOffset.Gain_R);
        drv_video_SCART_ADC_adjust_gain(_SCART_G, ptADCGainOffset.Gain_G);
        drv_video_SCART_ADC_adjust_gain(_SCART_B, ptADCGainOffset.Gain_B);
    }
}
#endif

#if 0	// not used
/**
	drv_video_SCART_ADC_adjust
	ADC R/G/B calibration function

	@param			{ source : VDC_R/VDC_G/VDC_B
				 	   gain : range 10bit (0~0x3FF), default = 0x200
				 	   offset : range 8bit (0~0xFF), default = 0x80	}
	@return                       { void }
*/
void drv_video_SCART_ADC_adjust(_ADC_CHANNEL source, UINT16 gain, UINT8 offset)
{
    UINT8 Gain_H;
    UINT8 Gain_L;

    Gain_H = (UINT16)(gain & 0x0300) >> 8;
    Gain_L = (UINT16)(gain & 0x00FF);
    if (source == VDC_R)
    {
        rtdf_outl(VDC_ADC_GAIN_R1_0x56e4, Gain_H);
        rtdf_outl(VDC_ADC_GAIN_R0_0x56e8, Gain_L);
        rtdf_outl(VDC_ADC_OFFSET_R_0x56fc, offset);
    }
    else if (source == VDC_G)
    {
        rtdf_outl(VDC_ADC_GAIN_G1_0x56ec, Gain_H);
        rtdf_outl(VDC_ADC_GAIN_G0_0x56f0, Gain_L);
        rtdf_outl(VDC_ADC_OFFSET_G_0x5700, offset);
    }
    else		// VDC_B
    {
        rtdf_outl(VDC_ADC_GAIN_B1_0x56f4, Gain_H);
        rtdf_outl(VDC_ADC_GAIN_B0_0x56f8, Gain_L);
        rtdf_outl(VDC_ADC_OFFSET_B_0x5704, offset);
    }
}

#endif

void drv_video_system_Init(void)
{
    vdc_InputSrc = 0xff;
    vdc_dcti_talbe_value = 0;
    vdc_factory_mod_enable = 0;
    ucVdc_Curr_Mode = 0x00;
    ucVdc_Pre_Mode = 0x11;
    field = VDC_FRAME_RATE_50HZ;

//// Y/C separation Function ////
    YcSeparationStatus = VDC_YCSEPARATE_DEFAULT;
    cvideo_tv_channel_scan_is_running = 0;
    SCART_Type = _SCART_AV;	// 0: is scart av, 2: is scart sv, 1: is scart rgb
    pre_SCART_Type = 0xff;
    SCART_Counter = 0x00;
    SvNoBurstMark = 1;

#ifdef CONFIG_ENABLE_FHFVBug
    cvideo_fvfh_set_1d = _OFF;
    ucFHFVBug_Counter = 5;
//UINT16 OriginalIPHeriod;
//UINT16 OriginalDclkOffset;
#endif

#ifdef CONFIG_ENABLE_NTSC_No_Burst_Check
    bNTSCnoBurst = 0;
#endif
}

