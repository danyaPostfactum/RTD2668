/*===========================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2009
 * All rights reserved.
 * ========================================================== */

/*==================== File Description ===================== */
/**
 * @file
 * 	This file is string declaration for OSD related functions.
 *
 * @author 	$Author: cf.tsai $
 * @date 	$Date: 2009-03-16 $
 * @version	$Revision: 10 $
 */

/**
 * @addtogroup osd
 * @{
 */

#ifndef _OSDSTRING_H
#define _OSDSTRING_H
/*===================== Module Dependency  ================== */
#include "osdobjectdef.h"


/*======================== CONSTANTS =========================================*/


/*======================== MACROS ============================================*/


/*======================== TYPES =============================================*/
enum LanguageType
{
    _ENGLISH,
    _FRENCH,
    _GERMAN,
    _SPANISH,
    _CHINESE_T,
//    _CHINESE_S,
//    _JAPAN,
//    _ITALIAN,
    _LANG_TOTAL,
};

/*======================== VARIABLES =========================================*/
#if !defined(_OSDSTRING_C)
#define EXTERN	extern

//color
EXTERN UINT8 code tOsdContrastStr[];
EXTERN UINT8 code tOsdBrightnessStr[];
EXTERN UINT8 code tOsdHueStr[];
EXTERN UINT8 code tOsdSaturationStr[];
EXTERN UINT8 code tOsdSharpnessStr[];

EXTERN UINT8 code tOsdColorTempStr[];

EXTERN UINT8 code tOsdBacklightStr[];

EXTERN UINT8 code tOsdColorTempWarmerStr[];
EXTERN UINT8 code tOsdColorTempWarmStr[];
EXTERN UINT8 code tOsdColorTempNormalStr[];
EXTERN UINT8 code tOsdColorTempCoolStr[];
EXTERN UINT8 code tOsdColorTempCoolerStr[];
EXTERN UINT8* code tColorTempStrSet[];

EXTERN UINT8 code tOsdDCRStr[];

EXTERN UINT8 code tOsdDNRStr[];
EXTERN UINT8 code tOsdDNRAutoStr[];
EXTERN UINT8 code tOsdDNRLowStr[];
EXTERN UINT8 code tOsdDNRMidStr[];
EXTERN UINT8 code tOsdDNRHighStr[];
EXTERN UINT8 code tOsdDNROffStr[];


EXTERN UINT8 code tStr_PictureMode_Vivid[];
EXTERN UINT8 code tStr_PictureMode_Standard[];
EXTERN UINT8 code tStr_PictureMode_Gentle[];
EXTERN UINT8 code tStr_PictureMode_User[];
EXTERN UINT8 code tStr_PictureMode[];
extern UINT8* code tPictureMode_StrSet[];


EXTERN UINT8 code tStr_EQ_Disable[];
EXTERN UINT8 code tStr_EQ_Club[];
EXTERN UINT8 code tStr_EQ_Dance[];
EXTERN UINT8 code tStr_EQ_Live[];
EXTERN UINT8 code tStr_EQ_Pop[];
EXTERN UINT8 code tStr_EQ_Rock[];
EXTERN UINT8 code tStr_EQ_User[];
EXTERN UINT8 code tStr_MusicMode[];
extern UINT8* code tMusicMode_StrSet[];


EXTERN UINT8 code tStr_MTS[];
EXTERN UINT8 code tStr_Mono[];
EXTERN UINT8 code tStr_Stereo[];
EXTERN UINT8 code tStr_Dual[];
extern UINT8* code tMTS_StrSet[];


//image
EXTERN UINT8 code tOsdClockStr[];
EXTERN UINT8 code tOsdPhaseStr[];

// Position
EXTERN UINT8 code tOsdHPositionStr[];
EXTERN UINT8 code tOsdVPositionStr[];
extern UINT8 code tOsdHSizeStr[];
extern UINT8 code tOsdVSizeStr[];

// Osd Menu
#if defined(CONFIG_EUROPE_LANGUAGE)
EXTERN UINT8 code tOsdLanguageStr[];
#endif
EXTERN UINT8 code tOsdOsdTimeStr[];
EXTERN UINT8 code tStr_Osd_Setting[];
EXTERN UINT8 code tOsdOsdBlendStr[];

// Function
EXTERN UINT8 code tOsdDisplayRatioStr[];
EXTERN UINT8 code tOsdAutoAdjustStr[];
EXTERN UINT8 code tOsdAutoColorStr[];
EXTERN UINT8 code tOsdDisplayIrStr[];

#if CONFIG_ENABLE_FM
EXTERN	UINT8 code tOsdDisplayRadioStr[];
#if (!ENABLE_FM_FREQSET)
EXTERN UINT8 code tStr_display_OFF[];
EXTERN UINT8 code tStr_display_877[];
EXTERN UINT8 code tStr_display_879[];
EXTERN UINT8 code tStr_display_881[];
EXTERN UINT8 code tStr_display_883[];
EXTERN UINT8 code tStr_display_885[];
EXTERN UINT8 code tStr_display_887[];
EXTERN UINT8 code tStr_display_889[];
EXTERN UINT8 code tStr_display_891[];
#endif
#endif


// Misc
EXTERN UINT8 code tStr_source[];
EXTERN UINT8 code tOsdResetStr[];
EXTERN UINT8 code tOsdSleepStr[];
EXTERN UINT8 code tOsdSleepTimerStr[];


EXTERN UINT8 code tOsdNumber0Str[];
EXTERN UINT8 code tOsdNumber1Str[];
EXTERN UINT8 code tOsdNumber2Str[];
EXTERN UINT8 code tOsdNumber3Str[];
EXTERN UINT8 code tOsdNumber4Str[];
EXTERN UINT8 code tOsdNumber5Str[];
EXTERN UINT8 code tOsdNumber6Str[];
EXTERN UINT8 code tOsdNumber7Str[];
EXTERN UINT8 code tOsdNumber8Str[];
EXTERN UINT8 code tOsdNumber9Str[];


// TV
extern UINT8 code tOsdAutoScanStr[];
extern UINT8 code tOsdTableScanStr[];
extern UINT8 code tStr_ManualScan[];
extern UINT8 code tOsdBlueScreenStr[];

// Port name string set
extern UINT8* code tOsdSourceStrSet[];

//Signal Source
extern UINT8 code tStrSignal_VGA[];
extern UINT8 code tStrSignal_DVI[];
extern UINT8 code tStrSignal_HDMI_0[];
extern UINT8 code tStrSignal_CVBS_0[];
extern UINT8 code tStrSignal_CVBS_1[];
extern UINT8 code tStrSignal_CVBS_2[];
extern UINT8 code tStrSignal_CVBS_3[];
extern UINT8 code tStrSignal_SV_0[];
extern UINT8 code tStrSignal_SV_1[];
extern UINT8 code tStrSignal_TV[];
extern UINT8 code tStrSignal_YPbPr_0[];
extern UINT8 code tStrSignal_YPbPr_1[];
extern UINT8 code tStrSignal_AvSv[];
extern UINT8 code tStrSignal_Scart_0[];
extern UINT8 code tStrSignal_Scart_1[];
#if CONFIG_ENABLE_USB
extern UINT8 code tStrSignal_Usb_0[];
#endif







// No Signal / No Support / No Cable
EXTERN UINT8 code tOsdNoSignalStr[];
EXTERN UINT8 code tOsdNoSupportStr[];
EXTERN UINT8 code tOsdNoCableStr[];
EXTERN UINT8 code tOsdNoTTStr[];


// VBI
EXTERN UINT8 code tOsdTTLabelStr[];
EXTERN UINT8 code tOsdTTMuteStr[];
EXTERN UINT8 code tOsdTTOnlyStr[];
EXTERN UINT8 code tOsdTTVideoStr[];


// Audio
EXTERN UINT8 code tOsdAudioMuteStr[];
EXTERN UINT8 code tOsdAudioBassStr[];
EXTERN UINT8 code tOsdAudioTrebleStr[];
EXTERN UINT8 code tOsdVolumnStr[];
EXTERN UINT8 code tOsdAudioBalanceStr[];
EXTERN UINT8 code tOsdAudioSpatialStr[];
extern UINT8 code tStr_Audio_Equalizer[];

EXTERN UINT8* code tOsdSelectSet[];
EXTERN UINT8 code tOsdOnStr[];
EXTERN UINT8 code tOsdOffStr[];
EXTERN UINT8* code tOsdOnOffStrSet[];

// Color temperature
EXTERN UINT8* code tOsdNumberStrSet[];
#if defined(CONFIG_EUROPE_LANGUAGE)
EXTERN UINT8* code tOsdLanguageStrSet[];
#endif

// Color Adjust
EXTERN UINT8* code tOsdColorAdjustStrSet[3];

// Deinterlace
EXTERN UINT8* code tOsdDeinterlaceStrSet[5];

// MTS
EXTERN UINT8* code tOsdMTSStrSet[];

//EQ
EXTERN UINT8* code tOsdEQStrSet[6];

EXTERN UINT8 code tOsdIcmStr[];

EXTERN UINT8 code tOsdRStr[];
EXTERN UINT8 code tOsdGStr[];
EXTERN UINT8 code tOsdBStr[];

EXTERN UINT8 code tOsdRHueStr[];
EXTERN UINT8 code tOsdGHueStr[];
EXTERN UINT8 code tOsdBHueStr[];
EXTERN UINT8 code tOsdYHueStr[];
EXTERN UINT8 code tOsdCHueStr[];
EXTERN UINT8 code tOsdVStr[];

EXTERN UINT8 code tOsdRSatStr[];
EXTERN UINT8 code tOsdGSatStr[];
EXTERN UINT8 code tOsdBSatStr[];
EXTERN UINT8 code tOsdYSatStr[];
EXTERN UINT8 code tOsdCSatStr[];


#if DISABLE_OPTTION

EXTERN UINT8 code tOsdSleepNoneStr[];
EXTERN UINT8 code tOsdSleep30Str[];
EXTERN UINT8 code tOsdSleep60Str[];
EXTERN UINT8 code tOsdSleep90Str[];
EXTERN UINT8 code tOsdSleep120Str[];
EXTERN UINT8 code tOsdSleep150Str[];

EXTERN UINT8* code tOsdSleepOptionStrSet[6];
#endif

EXTERN UINT8 code tStr_Channel_Edit[];
EXTERN UINT8 code tOsdScartDetectStr[];

EXTERN UINT8* code tOsdShowStatusStrSet[];

EXTERN UINT8 code tStr_CH[];
EXTERN UINT8 code tStr_Name[];
EXTERN UINT8 code tStr_Skip[];
EXTERN UINT8 code tStr_AFC[];

EXTERN UINT8 code tStr_MHz[];
//EXTERN UINT8 code tOsdChannelTotalStr[];
//EXTERN UINT8 code tOsdScanningStr[];
//EXTERN UINT8 code tOsdSearchNameStr[];
//EXTERN UINT8 code tOsdCountrySelectStr[];
//EXTERN UINT8 code tOsdMenuColorStr[];

//EXTERN UINT8* code tOsdCountryStrSet[];
EXTERN UINT8 code tOSdCanalPlusStr[];


// cftsai 20061101, Channel Setting
//EXTERN UINT8* code tOsdSystemSelectSet[];
// end of cftsai 20061101 added

//EXTERN UINT8* code tOsdBackLightStrSet[];


EXTERN UINT8 code tOsdAdjustingStr[];


EXTERN UINT8 code tOsdFactoryModeStr[];
EXTERN UINT8 code tStr_svn[];

//EXTERN UINT8* code tOsdPictureStrSet[];
EXTERN UINT8 code tOsdNicamStr[];


EXTERN UINT8 code tOsdAVStr[];
EXTERN UINT8 code tOsdSVStr[];
EXTERN UINT8 code tOsdRGBStr[];

//Tim 0323
//==============================================================================
EXTERN UINT8 code tStr_Cable[];
EXTERN UINT8 code tStr_AIR[];
EXTERN UINT8 code tOsdOffTableStr[];
EXTERN UINT8 code tOsdOnTableStr[];
EXTERN UINT8 code tStr_NtscFree[];

//==============================================================================

EXTERN UINT8 code tStr_AUTO[];
EXTERN UINT8 code tStr_NTSC443[];
EXTERN UINT8 code tStr_NTSC[];
EXTERN UINT8 code tStr_PAL[];
EXTERN UINT8 code tStr_SECAM[];
EXTERN UINT8 code tStr_ColorSystem[];
#ifdef CONFIG_VBI_CC_SUPPORT

EXTERN UINT8 code tOsdCC1Str[];
EXTERN UINT8 code tOsdCC2Str[];
#endif
EXTERN UINT8 code tStr_BG[];
EXTERN UINT8 code tStr_DK[];
EXTERN UINT8 code tStr_I[];
EXTERN UINT8 code tStr_L[];
EXTERN UINT8 code tStr_LL[];
EXTERN UINT8 code tStr_M[];
EXTERN UINT8 code tStr_SoundSystem[];

//Tim 0407
//==============================================================================
EXTERN UINT8 code tStr_N[];

//==============================================================================
EXTERN UINT8 code tStr_FineTune[];

EXTERN UINT8 code tStr_Ntsc_Type[];



//EXTERN UINT8 code tOsd_COLOR_Str[];
//EXTERN UINT8 code tOsd_PC_Str[];
//EXTERN UINT8 code tOsd_AUDIO_Str[];
//EXTERN UINT8 code tOsd_TV_Str[];
//EXTERN UINT8 code tOsd_OSD_Str[];
//EXTERN UINT8* code tOsdMainMenuHintStrSet[];
extern UINT8 code tStr_display_ratio_auto[];
extern UINT8 code tStr_display_ratio_4x3[];
extern UINT8 code tStr_display_ratio_16x9[];
extern UINT8 code tStr_display_ratio_14x9[];
extern UINT8 code tStr_display_ratio_letterbox[];
extern UINT8 code tStr_display_ratio_panorama[];

extern UINT8 code tStr_display_IR_OFF[];
extern UINT8 code tStr_display_IR_A[];
extern UINT8 code tStr_display_IR_B[];
extern UINT8 code tStr_display_Dimmer_on[];
extern UINT8 code tStr_display_Dimmer_off[];




//EXTERN UINT8 code tOsdMinStr[];
//EXTERN UINT8* code tOsdTtEventStrSet[];
//EXTERN UINT8 code tOsdNonPCMStr[];


EXTERN UINT8 code tOsdResolution480iStr[];
EXTERN UINT8 code tOsdResolution480pStr[];

EXTERN UINT8 code tOsdResolution576iStr[];
EXTERN UINT8 code tOsdResolution576pStr[];

EXTERN UINT8 code tOsdResolution240pStr[];
EXTERN UINT8 code tOsdResolution288pStr[];
EXTERN UINT8 code tOsdResolution720pStr[];
EXTERN UINT8 code tOsdResolution1080iStr[];
EXTERN UINT8 code tOsdResolution1080pStr[];

EXTERN UINT8* code tOsdResolutionStrSet[];
EXTERN UINT8* code tOsdHdmiResolutionStrSet[];
EXTERN UINT8* code tOsdVGAResolutionStrSet[];
EXTERN UINT8 code tOsdFreq24Str[];
EXTERN UINT8 code tOsdFreq25Str[];
EXTERN UINT8 code tOsdFreq30Str[];
EXTERN UINT8 code tOsdFreq50Str[];
EXTERN UINT8 code tOsdFreq60Str[];
EXTERN UINT8 code tOsdFreq100Str[];
EXTERN UINT8 code tOsdFreq120Str[];

#if CONFIG_ENABLE_INFO
EXTERN UINT8 code tOsdResolutionStr[];
#endif

EXTERN UINT8* code tOsdUartStrSet[2];

EXTERN UINT8 code tOsdD_DLtiStr[];
EXTERN UINT8 code tOsdD_DCtiStr[];
EXTERN UINT8 code tOsdI_DLtiStr[];
EXTERN UINT8 code tOsdI_DCtiStr[];
EXTERN UINT8 code tOsdSUHCoefStr[];
EXTERN UINT8 code tOsdSUVCoefStr[];
EXTERN UINT8 code tOsdSUPKStr[];
EXTERN UINT8 code tOsdSUPK_MaskStr[];
EXTERN UINT8 code tOsdPeakingStr[];
EXTERN UINT8 code tOsdSUNR_YStr[];
EXTERN UINT8 code tOsdSUNR_CStr[];
EXTERN UINT8 code tOsdSharpStr[];
EXTERN UINT8 code tOsdUartStr[];
EXTERN UINT8 code tOsdYPbPrPhaseStr[];
EXTERN UINT8 code tOsdSpreadSpectrumStr[];
EXTERN UINT8 code tOsdFacResetStr[];
EXTERN UINT8 code tOsdGammaStr[];
EXTERN UINT8 code tOsdSIndexStr[];
EXTERN UINT8 code tOsdSLowStr[];
EXTERN UINT8 code tOsdSHighStr[];
EXTERN UINT8 code tOsdDccStr[];
EXTERN UINT8 code tOsd_DigitalFilter_Str[];
EXTERN UINT8 code tOsd_Dither_Bit_Str[];

EXTERN UINT8 code mainIcons[10][4];
EXTERN UINT8 code Icons_normale_av[3][3];
EXTERN UINT8 code Icons_Select_av[3][3];
EXTERN UINT8 code Icons_normale_vga[4][3];
EXTERN UINT8 code Icons_Select_vga[4][3];
EXTERN UINT8 code Icons_normale_hdmi[3][3];
EXTERN UINT8 code Icons_Select_hdmi[3][3];
EXTERN UINT8 code Icons_disable[5][3];
/**
* Menu Hint strings
*/
EXTERN UINT8 code tMenuHint_MainIcon[];
EXTERN UINT8 code tSourceMenuHint[];

EXTERN UINT8 code tOsdUpDownButtonStr[];
EXTERN UINT8 code tOsdLeftButtonStr[];
EXTERN UINT8 code tOsdLeftRightButtonStr[];
EXTERN UINT8 code tOsdOkButtonStr[];
EXTERN UINT8 code tOsdOkRightButtonStr[];
EXTERN UINT8 code tOsdMenuButtonStr[];
EXTERN UINT8 code tOsdSelectStr[];
EXTERN UINT8 code tOsdEnterStr[];
EXTERN UINT8 code tOsdAdjustStr[];
EXTERN UINT8 code tOsdConfirmStr[];
EXTERN UINT8 code tOsdReturnStr[];
EXTERN UINT8 code tOsdExitStr[];



/**
 * Factory menu OSD strings.
 */
extern UINT8 code tStr_WB_R_Gain[];
extern UINT8 code tStr_WB_G_Gain[];
extern UINT8 code tStr_WB_B_Gain[];
extern UINT8 code tStr_WB_R_Offset[];
extern UINT8 code tStr_WB_G_Offset[];
extern UINT8 code tStr_WB_B_Offset[];
extern UINT8 code tStr_WBH_Bri[];
extern UINT8 code tStr_WBH_Cont[];
extern UINT8 code tStr_WBL_Bri[];
extern UINT8 code tStr_WBL_Cont[];
extern UINT8 code tStrWhiteBalance[];

extern UINT8 code tStr_ColorTemp[];
extern UINT8 code tStr_ColorTemp_Red[];
extern UINT8 code tStr_ColorTemp_Green[];
extern UINT8 code tStr_ColorTemp_Blue[];

extern UINT8 code tStr_ADC_Calib_HD720P[];
extern UINT8 code tStr_ADC_Calib_VGA[];
extern UINT8 code tStr_ADC_Calib_1BY1[];
extern UINT8 code tStr_ADC_Calibration[];

extern UINT8* code tDefaultLogo_StrSet[];

extern UINT8 code tStr_Default_Logo[];
#if defined(CONFIG_FAC_DEFAULT_LANG_OPT)
extern UINT8 code tStr_Default_Language[];
#endif
extern UINT8 code tStr_Option[];

extern UINT8 code tStr_Clear[];
extern UINT8 code tStr_ClearAll[];
extern UINT8 code tStr_Factory_Reset[];

extern UINT8 code tStr_Version[];
extern UINT8 code tStr_FwVersion[];
extern UINT8 code tStr_Empty[];
extern UINT8 code tStr_Panel[];
extern UINT8 code tStr_PanelType[];
extern UINT8 code tStr_Falsh[];
extern UINT8 code tStr_FalshType[];

extern UINT8 code tStr_Brightness_0[];
extern UINT8 code tStr_Brightness_50[];
extern UINT8 code tStr_Brightness_100[];
extern UINT8 code tStr_Contrast_0[];
extern UINT8 code tStr_Contrast_50[];
extern UINT8 code tStr_Contrast_100[];
extern UINT8 code tStr_Saturation_0[];
extern UINT8 code tStr_Saturation_50[];
extern UINT8 code tStr_Saturation_100[];
extern UINT8 code tStr_PictureSetting[];

extern UINT8 code tStr_200Hz[];
extern UINT8 code tStr_300Hz[];
extern UINT8 code tStr_1KHz[];
extern UINT8 code tStr_3KHz[];
extern UINT8 code tStr_8KHz[];
extern UINT8 code tStr_15KHz[];
extern UINT8 code tStr_Hz[];

extern UINT8 code tStr_Volume_1[];
extern UINT8 code tStr_Volume_25[];
extern UINT8 code tStr_Volume_50[];
extern UINT8 code tStr_Volume_75[];
extern UINT8 code tStr_Volume_100[];
extern UINT8 code tStr_TV_Volume_1[];
extern UINT8 code tStr_TV_Volume_25[];
extern UINT8 code tStr_TV_Volume_50[];
extern UINT8 code tStr_TV_Volume_75[];
extern UINT8 code tStr_TV_Volume_100[];
extern UINT8 code tStr_VolumeSetting[];

extern UINT8 code tStr_Dclk_RANGE[];
extern UINT8 code tStr_Dclk_FMDIV[];
extern UINT8 code tStr_EMI[];

extern UINT8 code tStr_RF_StartDb[];

extern UINT8 code tStr_Setting[];
extern UINT8 code tStr_Design[];

extern UINT8 code tStr_XNR[];
extern UINT8 code tStr_Set_Skip[];
extern UINT8 code tStr_Remove[];
extern UINT8 code tStr_Demo[];
extern UINT8 code tStr_Copy[];
extern UINT8 code tStr_Insert[];




/**
 * Menu title strings
 */
EXTERN UINT8 code tStr_picture_title[];
EXTERN UINT8 code tStr_sound_title[];
EXTERN UINT8 code tStr_screen_title[];
EXTERN UINT8 code tStr_setup_title[];
EXTERN UINT8 code tStr_channel_title[];
#ifdef CONFIG_VBI_CC_SUPPORT
extern UINT8 code tStr_Cctype[];
extern UINT8* code tOsdCCSelStrSet[];
#endif

#undef EXTERN
#endif // !defined(_OSDSTRING_C)


/*======================== FUNCTIONS =========================================*/

#endif // #ifndef _OSDSTRING_H

/**
 *
 * @}
 */

