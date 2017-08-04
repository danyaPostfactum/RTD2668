
/*=============================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2004 *
 * All rights reserved.                                       *
 *============================================================*/

/*======================= File Description ========================*/
/**
 * @file hdmiFun.h
 * To supply 2670 HDMI RX driver APIs.
 * @author Justin
 * @date 2009/06/30
 * @version 0.1
 * @ingroup drv_hdmi
 */
#include "autoconf.h"

#ifdef CONFIG_ENABLE_HDMI
#ifndef _HDMI_FUN_H
#define _HDMI_FUN_H

/*========================Definitions=============================*/

#define SPARROW_HDMI


/*

	include file

*/

#include <hdmi/hdmiSparrow.h>


/*

	LOG Control Function Definition

*/


#define HDMI_LOG		HDMI_PRINTF
//#define HDMI_LOG		//

//#define HDMI_DEBUG_AUDIO  HDMI_PRINTF
#define HDMI_DEBUG_AUDIO

/********************************************************************************************


				HDMI HDMI_CONSTant definition


********************************************************************************************/

#define CHECK_TIME 2
#define B_CHECK_ZERO_COUNT  10
#define	HDCP_10_DVI 	0x91
#define	HDCP_11_HDMI 	0x93
#ifndef PIN_HDMI_HOTPLUG
#define PIN_HDMI_HOTPLUG 0
#endif
#ifndef _HDMI_HOTPLUG_OFF
#define _HDMI_HOTPLUG_OFF 0
#endif

#ifndef HDMI_HPD_LEVEL
#define HDMI_HPD_LEVEL(x, y)	drv_pcb_set_gpio_pin(x, y)//For SVN 1911
#endif

#define _AUDIO_128_TIMES        1
#define _AUDIO_256_TIMES        2

#define _AUDIO_MCK_32000        _AUDIO_256_TIMES
#define _AUDIO_MCK_44100        _AUDIO_256_TIMES
#define _AUDIO_MCK_48000        _AUDIO_256_TIMES
#define _AUDIO_MCK_88200        _AUDIO_256_TIMES
#define _AUDIO_MCK_96000        _AUDIO_256_TIMES
#define _AUDIO_MCK_176400       _AUDIO_128_TIMES
#define _AUDIO_MCK_192000       _AUDIO_128_TIMES

/********************************************************************************************


	HDMI sturcture definition


********************************************************************************************/

typedef enum
{
    IOCTRL_HDMI_PHY_START = 0,
    IOCTRL_HDMI_PHY_STOP,
    IOCTRL_HDMI_GET_STRUCT,
    IOCTRL_HDMI_AUDIO_PLL_SAMPLE_START,
    IOCTRL_HDMI_AUDIO_PLL_SAMPLE_STOP,
    IOCTRL_HDMI_AUDIO_PLL_SAMPLE_DUMP,
}  HDMIRX_IOCTL_T;

typedef struct
{
    UINT16 IVTotal;
    UINT16 IHTotal;
    UINT16 IVFreq;
    INT16   IVSTA_ADJ;
    INT16   IHSTA_ADJ;
    INT16   IVHEIGHT_ADJ;
    INT16   IHWIDTH_ADJ;
} HDMI_TIMING_COMPENSATION_T;


typedef struct
{
    UINT16 b;
    UINT16 b_pre;
    UINT16 data_b;
    UINT8 measure_source;
    UINT8 b_change;
    UINT16 measure_count;
} HDMIRX_IOCTL_STRUCT_T;


typedef enum
{
    MODE_DVI	= 0x0,
    MODE_HDMI  ,
    MODE_UNKNOW
} HDMI_DVI_MODE_T;

typedef enum
{
    COLOR_RGB 	= 0x00,
    COLOR_YUV422,
    COLOR_YUV444,
    COLOR_UNKNOW
} HDMI_COLOR_SPACE_T;

typedef struct
{
    // for channel selection
    UINT8	DP_PHY_2D_switch_value;
    UINT8     DP_PHY_RESERVED_07_value;
    UINT8     ddc_index;
    // EDID table
    UINT8     EDID[256];   // EDID may stored in Flash and DDR
    // HOTPLUG PIN define
    UINT32	HotPlugPin;
    BOOL      HotPlugOn;
    // channel enable for AP configuration
    BOOL 	DDCEnable;
    BOOL	Enable;

} HDMI_CHANNEL_T;

typedef struct
{
    UINT8 BKsv[5];
    UINT8 Key[320];
} HDCP_KEY_T;

typedef enum
{
    MAIN_FSM_HDMI_SETUP_VEDIO_PLL = 0,
    MAIN_FSM_HDMI_MEASURE
} HDMI_MAIN_FSM_T;


typedef enum
{
    AUDIO_FSM_AUDIO_START = 0,
    AUDIO_FSM_FREQ_DETECT,
    AUDIO_FSM_AUDIO_WAIT_TO_START,
    AUDIO_FSM_AUDIO_START_OUT,
    AUDIO_FSM_AUDIO_WAIT_PLL_READY,
    AUDIO_FSM_AUDIO_CHECK
} HDMI_AUDIO_FSM_T;


typedef enum
{

    AUDIO_FORMAT_LPCM = 0,
    AUDIO_FORMAT_NONLPCM

} HDMI_AUDIO_FORMAT_T;

typedef struct
{
    unsigned long ACR_freq;			// Audio Frequency from ACR
    unsigned long AudioInfo_freq;		// Audio Frequency from Audio Info Frame
    unsigned long SPDIF_freq;			// Audio Frequency from SPDIF Channel Status Info
} HDMI_AUDIO_FREQ_T;

typedef struct
{
    unsigned long 	   freq;
    UINT8 coeff;
    UINT8 rate;
} HDMI_AUDIO_PLL_COEFF_T;

typedef enum _HDMI_TABLE_TYPE
{
    HDMI_CHANNEL0,
    HDMI_CHANNEL1,
    HDMI_CHANNEL2,
    HDMI_HDCP_KEY_TABLE
} HDMI_TABLE_TYPE;

typedef struct
{

    UINT16 Info;     // [0:1] Color Space
    // [2]   IsInterlace
    // [3]    is hdmi/dvi
    // [4:7] color depth
    // [8]    current channel

    UINT8 FSM;	// [0:3] Video FSM
    // [4:7] Audio FSM

    UINT16 b;
    HDCP_KEY_T HDMI_CONST* hdcpkey;
    HDMI_CHANNEL_T * channel[HDMI_MAX_CHANNEL];

    UINT32 timer;
} HDMI_INFO_T;


typedef struct
{

    UINT8 N;
    UINT8 M;
    UINT8 O;
    UINT8 S;
    UINT8 S1;
    UINT32 D_HighByte;
    UINT32 D_LowByte;
    char *sample_rate;

} HDMI_AUDIO_PLL_PARAM_T;

typedef struct
{

    UINT16 b_upper;
    UINT16 b_lower;
    UINT8 O;
    UINT8 S;
    char  *band_name;

} HDMI_VEDIO_PLL_PARAM_T;

typedef enum
{
    MEASURE_TMDS =0,
    MEASURE_GREEN,
    MEASURE_RED,
    MEASURE_BLUE
};

typedef enum
{
    HDMI_AUDIO_N_CTS_TREND_BOUND = 0,
    HDMI_AUDIO_TREND_BOUND,
    HDMI_AUDIO_N_CTS,
} HDMI_AUDIO_TRACK_MODE;


/****************************************************************************


						HDMI Driver Private MARCO


****************************************************************************/

#define GET_HDMI_COLOR_SPACE()			(hdmi.Info & (_BIT0 | _BIT1))
#define GET_HDMI_ISINTERLACE()			((hdmi.Info & (_BIT2)) >> 2)
#define GET_ISHDMI()						((hdmi.Info & _BIT3) >> 3)
#define GET_HDMI_CHANNEL()				((hdmi.Info & _BIT8) >> 8)
#define GET_HDMI_CD()					((hdmi.Info & (0xf0)) >> 4)
#define GET_HDMI_DEONLY_MODE()		((hdmi.Info & _BIT9) >> 8)

#define GET_HDMI_VIDEO_FSM()			(hdmi.FSM & 0x0F)
#define GET_HDMI_AUDIO_FSM()			((hdmi.FSM & 0xF0) >> 4)


#define SET_HDMI_COLOR_SPACE(x)		(hdmi.Info = ((hdmi.Info & ~ (_BIT0 | _BIT1))  | (((UINT16) x & 0x03))))
#define SET_HDMI_CD(x)					(hdmi.Info = ((hdmi.Info & ~(0xf0))  | (((UINT16) x & 0x0f) << 4)))
#define SET_HDMI_ISINTERLACE(x)		(hdmi.Info = ((hdmi.Info & (~_BIT2))  | (((UINT16) x & 0x01) << 2)))
#define SET_ISHDMI(x)					(hdmi.Info = 	((hdmi.Info & ~_BIT3)  | (((UINT16) x & 0x01) << 3)))
#define SET_HDMI_CHANNEL(x)			(hdmi.Info = 	((hdmi.Info & ~_BIT8)  | (((UINT16) x & 0x01) << 8)))
#define SET_HDMI_DEONLY_MODE(x)		(hdmi.Info = ((hdmi.Info & (~_BIT9))  | (((UINT16) x & 0x01) << 9)))

#define SET_HDMI_VIDEO_FSM(x)			(hdmi.FSM = (hdmi.FSM & 0xF0) | ((UINT8) x & 0x0F))
#define SET_HDMI_AUDIO_FSM(x)			(hdmi.FSM = (hdmi.FSM & 0x0F) | (((UINT8) x & 0x0F) << 4))


#if HDMI_SUPPORT_NONPCM
#define HDMI_AUDIO_IS_LPCM()  1
#else
#define HDMI_AUDIO_IS_LPCM()  ((hdmi_in(HDMI_HDMI_SR_VADDR) & _BIT4) == 0)
#endif // ENABLE_HDMIRX_NONPCM


/****************************************************************************


						HDMI Driver Private Function


****************************************************************************/

extern HDMI_INFO_T hdmi;
extern HDMI_CONST HDMI_TIMING_COMPENSATION_T hdmi_timing_compensate[];
#if HDMI_BULD_IN_TABLE
extern HDMI_CONST HDCP_KEY_T default_hdcpkey;
extern HDMI_CONST HDMI_CHANNEL_T default_channel[HDMI_MAX_CHANNEL];
#endif



/****************************************************************************


						HDMI Driver Private Function


****************************************************************************/

/*
	hdmiPhy.c

*/
extern void HdmiGetStruct(HDMIRX_IOCTL_STRUCT_T* ptr);
extern HDMI_BOOL rtd_hdmiRx_cmd(UINT8 cmd,  void * arg);
extern HDMI_BOOL rtd_hdmiPhy_ISR(void);


/*
	hdmiHdmi.c

*/
//extern unsigned long Hdmi_AudioFreqCorrect(unsigned long freq);
extern unsigned long Hdmi_AudioFreqCorrect(unsigned long freq, unsigned long b, HDMI_AUDIO_TRACK_MODE *track_mode);

extern HDMI_BOOL inline Hdmi_StoragePortRead(UINT8 addr);
extern void inline Hdmi_HdcpPortCWrite(UINT8 addr ,UINT8 HDMI_CONST * value ,UINT8 num );
extern void inline Hdmi_HdcpPortWrite(UINT8 addr ,UINT8 value);
extern UINT8 inline Hdmi_HdcpPortRead(UINT8 addr);
extern void 	inline  Hdmi_VideoOutputEnable(void);
extern void 	inline  Hdmi_VideoOutputDisable(void);
extern void 	inline  Hdmi_AudioOutputEnable(void);
extern void 	inline  Hdmi_AudioOutputDisable(void);
extern void 	inline  Hdmi_AudioOutputMute(void);
extern UINT8  inline Hdmi_WaitAudioSample(void);
extern void 	Hdmi_RgbYuvDetect(void);
extern HDMI_COLOR_SPACE_T Hdmi_GetColorSpace(void);
//extern HDMI_BOOL Hdmi_AudioPLLSetting(unsigned long freq, int use_trend_tracking);
extern HDMI_BOOL Hdmi_AudioPLLSetting(unsigned long freq, HDMI_AUDIO_TRACK_MODE track_mode);
extern void HdmiSetScalerColor(void);
extern void Hdmi_CheckTransition(void);
extern HDMI_DVI_MODE_T IsHDMI(void);
extern void Hdmi_Power(char Enable);
extern HDMI_BOOL Hdmi_OnlineMeasure2(void);
extern void Hdmi_WriteHDCPKey(void);
/*

	hdmiPlatform.c

*/
extern void HdmiISREnable(char nEnable);
extern void HdmiISRInit(void);
extern void HdmiISRClose(void);
extern void HdmiAudioPLLSampleStart(void);
extern void HdmiAudioPLLSampleStop(void);
extern void HdmiAudioPLLSampleDump(void);
extern HDMI_BOOL Hdmi_Search_Mode(void);



/****************************************************************************


						HDMI Driver Public Function


****************************************************************************/

extern HDMI_BOOL drvif_IsAVMute(void);
/**
* CHdmi_FormatDetect
*hdmi or dvi mode detect
*@param <none>
*@return _TRUE: hdmi , _FALSE: dvi
*@ingroup drv_hdmi
*/
//UINT8 CHdmi_FormatDetect(void);
extern HDMI_DVI_MODE_T drvif_IsHDMI(void);
#define CHdmi_FormatDetect() 	drvif_IsHDMI()


/**
* drv_Hdmi_TmdsDetect
*tmds signal detect
*@param <ucPar,  _DE_ONLY_OFF or _DE_ONLY_ON>
*@return {_TRUE:success , _FALSE:fail }
*@ingroup drv_hdmi
*/
extern HDMI_BOOL  drv_Hdmi_TmdsDetect(UINT8 ucPar);

#define drvif_Hdmi_TmdsDetect(x)  CHdmi_TmdsDetect(x)

/**
* drvif_Hdmi_IRQInit
*hdmi IRQ init
*@param <none>
*@return {none}
*@ingroup drv_hdmi
*/
extern void drvif_Hdmi_IRQInit(void);
#define CHdmi_IRQInit drvif_Hdmi_IRQInit


/**
* drvif_Hdmi_SetIRQ
*hdmi IRQ set
*@param <enable>
*@return {none}
*@ingroup drv_hdmi
*/
extern void drvif_Hdmi_SetIRQ(UINT8 enable);

#define CHdmi_SetIRQ(x) 	drvif_Hdmi_SetIRQ(x)

extern void drvif_Hdmi_AudioSetIRQ(UINT8 enable);

/**
* drvif_Hdmi_AuthenCheck
*hdcp authen change check
*@param <none>
*@return {_TRUE: change , _FALSE: no change}
*@ingroup drv_hdmi
*/
extern HDMI_BOOL  drvif_Hdmi_AuthenCheck(void);
#define CHdmi_AuthenCheck()  drvif_Hdmi_AuthenCheck()

/**
* drvif_Hdmi_GetInterlace
*hdmi get input signal interlace or progressive mode
*@param <none>
*@return {_TRUE: interlace , _FALSE: progressive}
*@ingroup drv_hdmi
*/
extern HDMI_BOOL  drvif_Hdmi_GetInterlace(void);
#define CHdmi_GetInterlace()       drvif_Hdmi_GetInterlace()

/**
* drvif_Hdmi_HdcpInit
*hdcp init
*@param <none>
*@return {none}
*@ingroup drv_hdmi
*/
extern void drvif_Hdmi_HdcpInit(void);

#define CHdmi_HdcpInit() 	drvif_Hdmi_HdcpInit()

/**
* drvif_Hdmi_Init
*hdmi init
*@param <none>
*@return {none}
*@ingroup drv_hdmi
*/
extern void Hdmi_Src(UINT16 src);
extern void drvif_Hdmi_Init(void);
extern UINT8 drv_hdmi_AFD(void);

#define CHdmi_Init() drvif_Hdmi_Init()

/**
* CHdmi_TmdsInit
*tmds init
*@param <none>
*@return {_TRUE: success}
*@ingroup drv_hdmi
*/
extern HDMI_BOOL  drvif_Hdmi_TmdsInit(void);
#define CHdmi_TmdsInit() 	drvif_Hdmi_TmdsInit()

/**
* drvif_Hdmi_InitSrc
*tmds initSrc
*@param <init_table>
*@return {none}
*@ingroup drv_hdmi
*/
extern void drvif_Hdmi_InitSrc(UINT8 channel);
#define CHdmi_InitSrc(x)   drvif_Hdmi_InitSrc(x)

/**
* drvif_Hdmi_DetectMode
*hdmi detect mode
*@param <none>
*@return { modestate(_MODE_NOCABLE or  _MODE_NOSIGNAL or  _MODE_DETECT or  _MODE_SUCCESS) }
*@ingroup drv_hdmi
*/
extern UINT8  drvif_Hdmi_DetectMode(void);
#define CHdmi_DetectMode()  drvif_Hdmi_DetectMode()

/**
* drvif_Hdmi_CheckMode
*hdmi check mode
*@param <none>
*@return { _TRUE:success , _FALSE:fail }
*@ingroup drv_hdmi
*/
extern HDMI_BOOL  drvif_Hdmi_CheckMode();
#define CHdmi_CheckMode()  drvif_Hdmi_CheckMode()

/**
* drvif_Hdmi_AudioFreqCheck
*drvif_Hdmi_AudioFreqCheck init
*@param <none>
*@return {none}
*@ingroup drv_hdmi
*/

extern HDMI_BOOL  drvif_Hdmi_AudioFreqCheck(void);

#define  CHdmi_AudioFreqCheck() 	drvif_Hdmi_AudioFreqCheck()

/**
* drvif_SetCecGpioHigh
*drvif_SetCecGpioHigh
*@param <none>
*@return {none}
*@ingroup drv_hdmi
*/
extern void drvif_SetCecGpioHigh(void);
#define CSetCecGpioHigh() drvif_SetCecGpioHigh()

/**
* drvif_Hdmi_GetColorSpace
*drvif_Hdmi_GetColorSpace
*@param <none>
*@return {none}
*@ingroup drv_hdmi
*/
extern HDMI_COLOR_SPACE_T drvif_Hdmi_GetColorSpace(void);
/**
* InProcHDMI_Init
*InProcHDMI init
*@param <none>
*@return {none}
*@ingroup drv_hdmi
*/
extern void InProcHDMI_Init(void);
//#define InProcHDMI_Init() InProcHDMI_Init()

/**
* InProcHDMI_Setup
*InProcHDMI set up
*@param <none>
*@return { _TRUE:success }
*@ingroup drv_hdmi
*/
extern HDMI_BOOL   InProcHDMI_Setup(void);
//#define InProcHDMI_Setup() InProcHDMI_Setup()

/**
*drvif_Hdmi_RgbYuvCheck
*drvif_Hdmi_RgbYuvCheck
*@param <none>
*@return { _TRUE:success }
*@ingroup drv_hdmi
*/
//void drvif_Hdmi_RgbYuvCheck(void);

#define drvif_Hdmi_RgbYuvCheck()
#define CHdmi_RgbYuvCheck()
extern HDMI_BOOL drvif_Hdmi_AudioModeDetect(void);


//void drvif_Hdmi_EnableAudioOutput(void);
#define drvif_Hdmi_EnableAudioOutput()	drvif_Hdmi_AudioModeDetect()
#define Hdmi_EnableAudioOutput()  		drvif_Hdmi_AudioModeDetect()
#define drvif_Hdmi_FormatDetect        		drvif_IsHDMI


extern HDMI_BOOL drvif_Hdmi_regTable(HDMI_TABLE_TYPE index, void *ptr);
extern void drvif_Hdmi_Release(void);

void rtd_hdmiPhy_Check_CRC(void);

extern UINT8 AVMute_Timer;

#endif
#endif
