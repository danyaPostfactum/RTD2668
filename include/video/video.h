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
#ifndef _VIDEO_H
#define _VIDEO_H
#include "sparrow.h"
#include "source.h"
#include "mode.h"

/*==================== Definitions ================ */
#define _VIDEO_50HZ 0
#define _VIDEO_60HZ 1
#define _No_Sig     _BIT0
#define _H_Lock     _BIT1
#define _V_Lock     _BIT2
#define _C_Lock     _BIT3
#define _VD_AllLock	0x0E
#define _VDC_SUCCESS		1
#define _VDC_FAIL			0

#define    AVOUT1 0
#define    AVOUT2 1

#define    by_pass_enable  1
#define    by_pass_disable  0

// use enum replace magic number, hsliao 20081107
enum VDC_SOURCE_MODE
{
    VDC_SOURCE_MODE_480i,
    VDC_SOURCE_MODE_576i
};

enum VDC_MODE
{
    VDC_MODE_NTSC = 0,
    VDC_MODE_PALM = 1,
    VDC_MODE_NTSC50 = 2,
    VDC_MODE_PALN = 3,
    VDC_MODE_NTSC443 = 4,
    VDC_MODE_PAL60 = 5,
    VDC_MODE_PALI = 6,
    VDC_MODE_SECAM = 7,
    VDC_MODE_NO_SIGNAL = 8,
};

#define _VIDEO_NOT_EXIST    		_MODE_NOSIGNAL
//#define _VIDEO_EXIST        		_MODE_EXIST
#define _HSLCK      				_BIT1
#define _VLINF      				_BIT2

#define ZAUTO       				0x10
#define ZNTSC       				0x11
#define ZPAL_I     				0x02
#define ZPAL_M      				0x41
#define ZPAL_N      				0x52
#define ZNTSC_443   				0x31
#define ZSECAM      				0x22
#define ZSECAML					0x23
#define ZSECAMLA				0x24
#define ZPAL_60     				0x71
#define ZNTSC_50    				0x74

#define ZMODE_ERROR			0x19
#define ZCR_NOT_LOCK			0x1A
#define ZHV_NOT_LOCK			0x1B
#define ZFRQ_NOT_IN_BAND    	0x1C
#define ZMODE_525_STD			0x1D
/*==================== Variables ================= */
#define HIGH_BYTE(x)             ((x & 0xFF00)>>8)
#define LOW_BYTE(x)              (x & 0x00FF)


typedef enum _GET_VIDEO_MODE_METHOD
{
    VDC_SET = 0,
    VDC_DETECT = 1,
} GET_VIDEO_MODE_METHOD;


typedef enum _VDC_BLUE_SCREEN_MODE
{
    BLUE_SCREEN_DISABLE = 0,
    BLUE_SCREEN_ENABLE = 1,
    BLUE_SCREEN_AUTO = 2
} VDC_BLUE_SCREEN_MODE;

typedef enum _VDC_XNR_MODE
{
    VDC_XNR_DISABLE = 0,
    VDC_XNR_ENABLE = 1,
} _VDC_XNR_MODE;

typedef enum VDC_VIDEO_FrontDigitalFilter
{
    VDC_FDF_BYPASS=0,
    VDC_FDF_55MHZ,
    VDC_FDF_65MHZ,
    VDC_FDF_60MHZ,
} VDC_VIDEO_FrontDigitalFilter;

typedef enum _VDC_FRAME_RATE
{
    VDC_FRAME_RATE_60HZ = 0,
    VDC_FRAME_RATE_50HZ
} VDC_FRAME_RATE;

typedef enum _VDC_VIDEO_AVOUT_PATH
{
    VDC_AVOUT1=0,
    VDC_AVOUT2,

} _VDC_VIDEO_AVOUT_PATH;

typedef enum _VDC_VIDEO_Cresample_Lpf_MODE
{
    VDC_VIDEO_Cresample_Lpf_DISABLE=0,
    VDC_VIDEO_Cresample_Lpf_ENABLE,
} _VDC_VIDEO_Cresample_Lpf_MODE;

typedef enum _VDC_VIDEO_USER_CKILL_MODE
{
    VDC_USER_CKILL_AUTO_HW=0,
    VDC_USER_CKILL_FORCE_ON,
    VDC_USER_CKILL_FORCE_OFF,
} _VDC_VIDEO_USER_CKILL_MODE;

typedef enum _VDC_VIDEO_AVOUT_BY_PASS_ENABLE
{
    VDC_AVOUT_BY_PASS_ENABLE=0,
    VDC_AVOUT_BY_PASS_DISABLE,

} _VDC_VIDEO_AVOUT_BY_PASS_ENABLE;


typedef enum _VDC_VIDEO_AVOUT_MODE
{
    VDC_AVOUT_CVBS_NO_FIL_NO_AGC =0,
    VDC_AVOUT_CVBS_WITH_FIL_NO_AGC,
    VDC_AVOUT_CVBS_WITH_FIL_WITH_AGC,
    VDC_AVOUT_YC_WITH_FIL_NO_AGC,
    VDC_AVOUT_YC_WITH_FIL_WITH_AGC,
    VDC_AVOUT_NO_CLAMP_IFD_DATA,
    VDC_AVOUT_AUDIO_SIF_DATA,
    VDC_AVOUT_IFD_DATA,
    VDC_AVOUT_BYPASS_VIN00P,	//8
    VDC_AVOUT_BYPASS_VIN10P,
    VDC_AVOUT_BYPASS_VIN01P,
    VDC_AVOUT_BYPASS_VIN11P,
    VDC_AVOUT_BYPASS_VIN02P,
    VDC_AVOUT_BYPASS_VIN12P,
    VDC_AVOUT_BYPASS_VIN03P,
    VDC_AVOUT_DAC,
} _VDC_VIDEO_AVOUT_MODE;

typedef enum _VDC_YCSEPARATE_MODE
{
    VDC_YCSEPARATE_DEFAULT = 0,
    VDC_YCSEPARATE_1D,
    VDC_YCSEPARATE_2D,
    VDC_YCSEPARATE_3D,
} VDC_YCSEPARATE_MODE;

typedef enum _ADC_CHANNEL
{
    VDC_R = 0,
    VDC_G,
    VDC_B,
} _ADC_CHANNEL;

typedef enum _VDC_DCTI_ENABLE
{
    DCTI_DISABLE = 0,
    DCTI_ENABLE = 1,
} _VDC_DCTI_ENABLE;

typedef enum _VDC_CKill_MODE
{
    CKill_AUTO = 0x00,
    CKill_ON = 0x40,
    CKill_OFF = 0x80
} VDC_CKill_MODE;

typedef enum _VDC_VIDEO_AVOUT_ENABLE
{
    VDC_AVOUT_DISABLE=0,
    VDC_AVOUT_ENALBE,
} _VDC_VIDEO_AVOUT_ENABLE;

typedef enum _VDC_VIDEO_FIXCSTATE_MODE
{
    VDC_NOT_FIX_CSTATE0 = 0,
    VDC_FIX_CSTATE0,
    VDC_NOT_FIX_CSTATE1,
    VDC_FIX_CSTATE1,
    VDC_NOT_FIX_CSTATE2,
    VDC_FIX_CSTATE2,
    VDC_NOT_FIX_CSTATE3,
    VDC_FIX_CSTATE3,
    VDC_NOT_FIX_CSTATE4,
    VDC_FIX_CSTATE4,
    VDC_NOT_FIX_CSTATE5,
    VDC_FIX_CSTATE5,
    VDC_CSTATE_RESET,
} _VDC_VIDEO_FIXCSTATE_MODE;
#define VDC_CSTATE_DEFAULT	VDC_NOT_FIX_CSTATE3

typedef enum _VDC_VIDEO_FIXHSTATE_MODE
{
    VDC_FIX_HSTATE0=0,
    VDC_FIX_HSTATE1,
    VDC_FIX_HSTATE2,
    VDC_FIX_HSTATE3,
    VDC_FIX_HSTATE4,
    VDC_FIX_HSTATE5,
    VDC_NOT_FIX_HSTATE0,
    VDC_NOT_FIX_HSTATE1,
    VDC_NOT_FIX_HSTATE2,
    VDC_NOT_FIX_HSTATE3,
    VDC_NOT_FIX_HSTATE4,
    VDC_NOT_FIX_HSTATE5,
    VDC_HSTATE_RESET,
} _VDC_VIDEO_FIXHSTATE_MODE;

typedef enum _VDC_SOURCE_INDEX
{
    VDC_SOURCE_CVBS = 0,
    VDC_SOURCE_SV,
    VDC_SOURCE_TV,
#ifdef CONFIG_SCART_AUTO_SWITCH
    VDC_SOURCE_FULL_SCART,
    VDC_SOURCE_HALF_SCART,
#endif
//	VDC_SOURCE_SCART_AV,
//	VDC_SOURCE_SCART_RGB,
//	VDC_SOURCE_SCART_SV,
    VDC_SOURCE_TOTAL
};

//extern UINT16 code tVIDEO_TABLE_INPUT_INFO[][5];
extern bool    cvideo_tv_channel_scan_is_running;
extern UINT8 YcSeparationStatus;
extern UINT8 ucVdc_Curr_Mode;
extern UINT8 ucVdc_Pre_Mode;

#ifdef CONFIG_ENABLE_FHFVBug
extern bool    cvideo_fvfh_set_1d ;
extern UINT16 FieldVTotal;
//FHFV bias dclk
//extern UINT16 OriginalIPHeriod;
//extern UINT16 OriginalDclkOffset;
#endif

UINT8 drvif_module_vdc_AVG_BURST_MAG_STATUS(void);
//UINT8 drvif_module_vdc_Read_Hstate(void);
void drvif_module_vdc_Set_Hstate(_VDC_VIDEO_FIXHSTATE_MODE value);
void drvif_module_vdc_SetCstateToDefault(void);
UINT8 drvif_module_vdc_Read_Cstate(void);
void drvif_module_vdc_Set_Cstate(_VDC_VIDEO_FIXCSTATE_MODE value);
void drv_video_vdc_TV_FHFV_Bias_DCLK_Adjust(void);
void drv_video_vdc_YcDelay(UINT8 value);
void drv_video_vdc_Set_DCTI_Setting(UINT8 level);
void drv_video_vdc_tv_Reset(void);
void drv_video_vdc_RegSetDefault(void);
void drv_video_vdc_Initial(void);
void drv_video_FullSCART_func_initial(void);
void drv_video_SCART_RGB_set(bit on_off);
void drv_video_vdc_FreeRun(bit on_off);
//void drv_video_scart_Initial(void);
void drv_video_vdc_SelectInput(UINT8 ucInputSrc, VDC_INITIAL_OPTIONS  *ptOptions);
void drv_video_vdc_SoftReset(void);
void drv_video_vdc_AvOutMode(_VDC_VIDEO_AVOUT_PATH  uselect, _VDC_VIDEO_AVOUT_MODE mode);
void drv_video_vdc_AVoutEnable(_VDC_VIDEO_AVOUT_ENABLE on_off);
void drv_video_vdc_BlueScreenEnable(VDC_BLUE_SCREEN_MODE uselect);
void drv_video_vdc_SetContrast(UINT8 ucValue);
void drv_video_vdc_SetBrightness(UINT8 ucValue);
void drv_video_vdc_SetHue(UINT8 ucValue);
void drv_module_vdc_Set_CKill_Mode(VDC_CKill_MODE ucMode);
void drv_video_vdc_SetSaturation(UINT8 ucValue);
void drv_video_vdc_SetCKillValue(UINT8 ucValue);
void drv_video_vdc_user_ckill_mode(_VDC_VIDEO_USER_CKILL_MODE mode);
void drv_video_vdc_Dcti_Enable(_VDC_DCTI_ENABLE on_off);
void drv_video_vdc_YcSeparation(void);
void drv_video_reset_HPLL_default(void);
UINT8 drvif_module_vdc_flag358_detect(void);

UINT8 drv_video_vdc_NoiseStatus(void);
UINT8 drv_video_vdc_flag358_detect(void);
void drv_video_vdc_SetMode(UINT8 ucMode);
UINT8 drv_video_vdc_ReadMode(GET_VIDEO_MODE_METHOD result);
UINT8 drv_video_vdc_IdentifyColorStandard(void);
UINT8 drv_video_vdc_Exist(UINT8* ucMode);
UINT8 drv_video_vdc_CheckHVisLocked(void);
UINT8 drvif_module_vdc_New_Vlock(void);
UINT8 drv_video_detectmode(ModeInformationType* stModeInfo);
UINT8 drv_video_checkmode(void);
void	drv_video_Release(void);
void drv_video_inprocvideo_config_capture(UINT8 modecnt);
void drv_video_PowerOn(bit on_off);
void drv_video_Interrupt_Init(void);
bit drv_video_isr(void);

#ifdef CONFIG_ENABLE_FHFVBug
void drv_video_FhBug(void);
UINT8 drv_video_vdc_GetVHalfTotalLen(UINT16 *VActLen);
void drv_video_vdc_CheckBurst(UINT32 freq);
#endif

#ifndef CONFIG_TUNER_SILICON
UINT8 drv_video_GetTvColorStandard(void);
#endif
void drvif_module_vdc_TV_FHFV_Bias_DCLK_Adjust(void);
void drv_vdc_ResetReg(void);
void drvif_module_vdc_XNREnable(_VDC_XNR_MODE uselect);
#ifdef CONFIG_ENABLE_TV_NOISE_BOX_ENALBE
extern bit bTvNoiseBox;
#define GET_NoiseBoxStatus()		(bTvNoiseBox)
#define SET_NoiseBoxStatus(x)		bTvNoiseBox = x
void drv_vdc_TV_noise_box_delay(UINT16 usNum);
void drvif_module_vdc_noisebox_avoutfunction(bit on_off);
#endif
void drv_video_SCART_ADC_adjust(_ADC_CHANNEL source, UINT16 gain, UINT8 offset);
void drv_video_SCART_RGB_set_gain(void);
void drv_video_system_Init(void);

#endif // #ifdef _VIDEO_H
