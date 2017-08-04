/*===========================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2009
 * All rights reserved.
 * ========================================================== */

/*==================== File Description ===================== */
/**
 * @file
 * 	This file is osd control declaration related functions.
 *
 * @author 	$Author: cf.tsai $
 * @date 	$Date: 2009-03-16 $
 * @version $Revision: 10 $
 */

/**
 * @addtogroup osd
 * @{
 */

#ifndef _OSDCONTROL_H
#define _OSDCONTROL_H
/*===================== Module dependency  ================== */
#include "osdobjectdef.h"

/*=======================  Types ===================== */
#define 	OSD_EQ_MAX_VALUE  (6)

/*======================== Definitions ====================== */
#define _OSD_ACC_HWIN        1                    // 1-> enable ACC highlight window

enum _MENU_ITEM_FUNC_ID
{
    MENUITEM_DUMMY = 0,
    MENUITEM_SOURCE_SELECT,

    MENUITEM_BRIGHTNESS,
    MENUITEM_CONTRAST,
    MENUITEM_SATURATION,
    MENUITEM_HUE,
    MENUITEM_SHARPNESS,
    MENUITEM_COLOR_TEMPERATURE,
    MENUITEM_DNR,
    MENUITEM_DCR,

    MENUITEM_VGA_AUTO_POSITION,
    MENUITEM_VGA_CLOCK,
    MENUITEM_VGA_PHASE,
    MENUITEM_VGA_H_POSITION,
    MENUITEM_VGA_V_POSITION,
//	MENUITEM_VGA_H_SIZE,
//	MENUITEM_VGA_V_SIZE,

    MENUITEM_AUDIO_VOLUME,
    MENUITEM_AUDIO_BALANCE,
    MENUITEM_AUDIO_SPATIAL,
    MENUITEM_AUDIO_BASS,
    MENUITEM_AUDIO_TREBLE,
    MENUITEM_AUDIO_EQ_BAR,
    MENUITEM_MUSIC_MODE_300HZ,
    MENUITEM_MUSIC_MODE_1KHZ,
    MENUITEM_MUSIC_MODE_2KHZ,
    MENUITEM_MUSIC_MODE_3KHZ,
    MENUITEM_MUSIC_MODE_8KHZ,
    MENUITEM_MUSIC_MODE_15KHZ,

    MENUITEM_TV_VIDEO_STD,
    MENUITEM_TV_AUDIO_STD,
    MENUITEM_TV_FREQ_FINE_TUNE,
    MENUITEM_TV_NTSC_STD,
    MENUITEM_NTSC_FREE,
    MENUITEM_TV_AUTO_SCAN,
    MENUITEM_TV_TABLE_SCAN,
    MENUITEM_TV_MANUAL_SCAN,
    MENUITEM_TV_COUNTRY,
    MENUITEM_TV_BLUE_SCREEN,
    MENUITEM_TV_CHANNEL_EDIT,
    MENUITEM_TV_AVL,
    MENUITEM_TV_SPEAKER_COMPENSATION,
    MENUITEM_CHANNEL_SCAN,
    MENUITEM_TABLE_SCAN,
    MENUITEM_MANUAL_SCAN,
    MENUITEM_CHANNEL_SKIP,
    MENUITEM_AFC,

    MENUITEM_OSD_TIMEOUT,
    MENUITEM_OSD_BLEND,
    MENUITEM_OSD_LANGUAGE,
    MENUITEM_OSD_RESET,

    MENUITEM_SLEEP_MODE,
    MENUITEM_PICTURE_MODE,
    MENUITEM_MUSIC_MODE,
    MENUITEM_MTS_MODE,
    MENUITEM_DISPLAY_RATIO,
    MENUITEM_DISPLAY_IR,
    #if CONFIG_ENABLE_FM
    MENUITEM_DISPLAY_RADIO,
    #endif

    /* RTK factory mode */
    MENUITEM_PANEL_BACKLIGHT,
    MENUITEM_D_DLTI,
    MENUITEM_D_DCTI,
    MENUITEM_I_DLTI,
    MENUITEM_I_DCTI,
//	MENUITEM_FAC_SHARPNESS,
    MENUITEM_GAMMA,
    //MENUITEM_PEAKING,
    //MENUITEM_NR_SPATIAL,
    MENUITEM_SUNR_Y,
    MENUITEM_SUNR_C,
    MENUITEM_SUH_COEF,
    MENUITEM_SUV_COEF,
    MENUITEM_SUPK,
    MENUITEM_DCC,
    MENUITEM_FAC_DCR,
    MENUITEM_FAC_ICM,
    MENUITEM_SUPK_MASK,
    MENUITEM_DIGITAL_FILTER,
    MENUITEM_DITHER_BIT,
    MENUITEM_AUTO_COLOR,
    MENUITEM_YPBPR_PHASE,
    MENUITEM_SPREAD_SPECTRUM,
    MENUITEM_S_Index,
    MENUITEM_S_Low,
    MENUITEM_S_High,
#if 0 // not used and marked by weihao
    MENUITEM_UART,
#endif
    MENUITEM_FAC_RESET,

    /* factory mode */
    MENUITEM_FAC_WB_R_GAIN,
    MENUITEM_FAC_WB_G_GAIN,
    MENUITEM_FAC_WB_B_GAIN,
    MENUITEM_FAC_WB_R_OFFSET,
    MENUITEM_FAC_WB_G_OFFSET,
    MENUITEM_FAC_WB_B_OFFSET,
    MENUITEM_FAC_WB_H_BRI,
    MENUITEM_FAC_WB_H_CONT,
    MENUITEM_FAC_WB_L_BRI,
    MENUITEM_FAC_WB_L_CONT,

    MENUITEM_FAC_COLOR_TEM,
    MENUITEM_FAC_COLOR_TEM_R,
    MENUITEM_FAC_COLOR_TEM_G,
    MENUITEM_FAC_COLOR_TEM_B,

    MENUITEM_FAC_ADC_CALIB_HD720P,
    MENUITEM_FAC_ADC_CALIB_VGA,
    MENUITEM_FAC_ADC_CALIB_1BY1,

    MENUITEM_FAC_LOGO,
    MENUITEM_FAC_DEFAULT_LANGUAGE,

    MENUITEM_FAC_CLEAR,
    MENUITEM_FAC_CLEAR_ALL,

    MENUITEM_FAC_FW_VERSIOIN,
    MENUITEM_FAC_PANEL_TYPE,
    MENUITEM_FAC_FLASH_TYPE,

    MENUITEM_FAC_PICTURE_MODE,
    MENUITEM_FAC_PICMODE_BRI,
    MENUITEM_FAC_PICMODE_CONT,
    MENUITEM_FAC_PICMODE_SAT,

    MENUITEM_FAC_BRI_0,
    MENUITEM_FAC_BRI_50,
    MENUITEM_FAC_BRI_100,
    MENUITEM_FAC_CONT_0,
    MENUITEM_FAC_CONT_50,
    MENUITEM_FAC_CONT_100,
    MENUITEM_FAC_SAT_0,
    MENUITEM_FAC_SAT_50,
    MENUITEM_FAC_SAT_100,

    MENUITEM_FAC_MUSIC_MODE,
    MENUITEM_FAC_MUSIC_MODE_300HZ,
    MENUITEM_FAC_MUSIC_MODE_1KHZ,
    MENUITEM_FAC_MUSIC_MODE_2KHZ,
    MENUITEM_FAC_MUSIC_MODE_3KHZ,
    MENUITEM_FAC_MUSIC_MODE_8KHZ,
    MENUITEM_FAC_MUSIC_MODE_15KHZ,

    MENUITEM_FAC_VOLUME_1,
    MENUITEM_FAC_VOLUME_25,
    MENUITEM_FAC_VOLUME_50,
    MENUITEM_FAC_VOLUME_75,
    MENUITEM_FAC_VOLUME_100,
    MENUITEM_FAC_TV_VOLUME_1,
    MENUITEM_FAC_TV_VOLUME_25,
    MENUITEM_FAC_TV_VOLUME_50,
    MENUITEM_FAC_TV_VOLUME_75,
    MENUITEM_FAC_TV_VOLUME_100,

    MENUITEM_FAC_DCLK_RANGE,
    MENUITEM_FAC_DCLK_FMDIV,

    MENUITEM_FAC_RF_START_DB,

    MENUITEM_FAC_UART_PATH,

    MENUITEM_VDC_XNR,
    MENUITEM_PQ_DEMO,
#ifdef CONFIG_VBI_CC_SUPPORT
    MENUITEM_OSD_CC,
#endif

    MENUITEM_LAST_ID
};








/*========================== Variables ====================== */
#ifdef _OSDCONTROL_C
#define EXTERN
#else
#define EXTERN   extern
#endif

//EXTERN UINT8 xdata ucSleepTimeSec;
//EXTERN UINT8 xdata ucSleepModeTimes;

#undef EXTERN



typedef enum _AUDIO_MODE
{
    AUDIO_INITIAL=0,
    AUDIO_ACTIVESTATE_INITIAL,
    AUDIO_SEL_MUX,
    AUDIO_SEL_MUX_TO_HDMI,
    AUDIO_SEL_MUX_TO_DVI,
    AUDIO_MUTE,
    AUDIO_UNMUTE,
    AUDIO_SYS_UNMUTE,	// CF 2007 04 01 added, 恢復原始聲音狀態 (mute or unmute by GET_AUDIO_MUTE()
    AUDIO_ORIGINAL_MUTE_SETTING,
    AUDIO_ENABLE_SPATIAL,
    AUDIO_DISABLE_SPATIAL,
    AUDIO_ENABLE_STANDARD,
    AUDIO_DISABLE_STANDARD,
    AUDIO_POLLING_TV_AUDIO_STATUS,
    AUDIO_UNKNOWN
} _AUDIO_MODE;


#define _BACKLIGHT_STRONG			0
#define _BACKLIGHT_NORMAL			1
#define _BACKLIGHT_WEAK				2

/*=================== Local Functions Phototype ==============*/

//Tim 0323
//==============================================================================
UINT8 COsdCtrlGetTVNtscStd(void);

//==============================================================================

/*=========================== Functions ===================== */
void COsdCtrlMenuItemHandler(UINT8 ucMenuItemID, UINT16 ucArg0);
UINT16 COsdCtrlMenuItemGetValue(UINT8 ucMenuItemID);
UINT8 COsdCtrlMenuItemGetMin(UINT8 ucMenuItemID);
UINT8 COsdCtrlMenuItemGetMax(UINT8 ucMenuItemID);
UINT8 COsdCtrlMenuItemGetDefault(UINT8 ucMenuItemID);
UINT8 COsdCtrlMenuItemGetEnable(UINT8 ucMenuItemID);
UINT8* COsdCtrlMenuItemGetString(UINT8 ucMenuItemID);

void COsdCtrlSetStorageValue(void);

UINT8 COsdCtrlGetCurFreq(void);
UINT8 COsdCtrlGetMinFreq(void);
UINT8 COsdCtrlGetMaxFreq(void);

UINT8 COsdCtrlGetPictureMode(void);
void COsdCtrlSetPictureMode(UINT8 message);

UINT8 COsdCtrlGetSleepMode(void);
void COsdCtrlSetSleepMode(void);
void COsdSetSleepMode(void);
void COsdCoreShowSleepType();
void CShowTimer();

UINT8 COsdCtrlGetDNR(void);
void COsdCtrlSetDNR(void);

UINT8 COsdCtrlGetTVVideoStd(void);
void COsdCtrlSetTVVideoStd(void);

UINT8 COsdCtrlGetSoundSystem(void);
void COsdCtrlSetSoundSystem(UINT8 ucMode);

UINT8 COsdCtrlGetTVFineTune();
void COsdCtrlSetTVFineTune(UINT8 ucMode);
//UINT32 GetMinTVFreq(void);
UINT8 COsdCtrlGetUART(void);
void COsdCtrlSetUART(UINT8 ucVal);


void COsdFxChangeInputSrc(UINT8 ucSrcNo);
void ap_osdctrl_scart_change_source(UINT8 newSrc);

void COsdCoreSetBass(INT8 message);
void COsdCoreSetTreble(INT8 message);
void COsdCoreSetEQ(UINT8 message);

#if defined(CONFIG_AUDIO) || defined(CONFIG_EXTERNAL_AUDIO_IC)
void COsdFxChangeAudioMux(UINT8 ucSrcNo);
#endif
void COsdFxAudioFunction(_AUDIO_MODE ucmode);

UINT8 COsdCtrlGetSignalSource();
void COsdCtrlSetSignalSource(UINT8 message);

UINT8 COsdCtrlGetColorTemp();
void COsdCtrlSetColorTemp(void);

void COsdCoreDrawMainMenuFrame(UINT8 ucMainID);
UINT8 COsdCtrlGetNtscFree(void);

UINT8 COsdCtrlGetTvAutoScan();

UINT8 COsdCtrlGetAutoImage();
void COsdCtrlSetAutoImage();
UINT8 COsdCtrlGetBlackExpansion();
void COsdCtrlSetBlackExpansion();

UINT16 ap_osdctrl_get_osd_timeout_ms(void);
UINT8 COsdCtrlGetOsdTimeOutSystem(void);
void COsdCtrlSetOsdTimeOutSystem(void);

UINT8 COsdCtrlGetOsdBlend(void);
void COsdCtrlSetOsdBlend(UINT8 ucLevel);

UINT8 COsdCtrlGetDisplayRatio(void);
void ap_osdctrl_set_display_ratio(UINT8 message);

#if defined(CONFIG_GAMMA_FUNCTION)
UINT8 COsdCtrlGetGamma();
void COsdCtrlSetGamma();
UINT8 COsdCtrlGetSIndex();
void COsdCtrlSetSIndex();
UINT8 COsdCtrlGetSLow();
void COsdCtrlSetSLow();
UINT8 COsdCtrlGetSHigh();
void COsdCtrlSetSHigh();

#endif //defined(CONFIG_GAMMA_FUNCTION)


UINT8 COsdCtrlGetSharpness();
void COsdCtrlSetSharpness(UINT8 message);

UINT8 COsdCtrlGetHue();
void COsdCtrlSetHue(void);

UINT8 COsdCtrlGetSaturation();
void COsdCtrlSetSaturation(UINT8 message);

UINT8 COsdCtrlGetMainYContrast();
void COsdCtrlSetMainYContrast(UINT8 message);

UINT8 COsdCtrlGetMainYBrightness();
void COsdCtrlSetMainYBrightness(UINT8 message);


UINT8 COsdCtrlGetPWM();
void COsdCtrlSetPWM(UINT8 message);

void COsdCtrlSetFacBacklight(UINT8 ucValue);
UINT8 COsdCtrlGetFacBacklight(void);
UINT8 CConverModeProc(UINT8 soundmode);


void COsdCtrlSetSwap();

UINT8 COsdCtrlGetLanguage(void);
void COsdCtrlSetLanguage(void);

UINT8 COsdCtrlGetSubYContrast();
void COsdCtrlSetSubYContrast(UINT8 message);

void ap_osdctrl_audio_mute(UINT8 ucMode);

void ap_osdctrl_audio_setting_init(void);


UINT8 COsdCtrlGetAudioMute();
void COsdCtrlSetAudioMute(void);
UINT8 COsdCtrlGetAudioSpatial();
void COsdCtrlSetAudioSpatial(void);
UINT8 COsdCtrlGetAudioBass(void);
void COsdCtrlSetAudioBass(void);
UINT8 COsdCtrlGetAudioTreble(void);
void COsdCtrlSetAudioTreble(void);
UINT8 COsdCtrlGetAudioBalance(void);
void  COsdCtrlSetAudioBalance(void);
UINT8 COsdCtrlGetAudioVolume();
void COsdCtrlSetAudioVolume(void);

void COsdCtrlSetDualCh(UINT8 ucCh);


void COsdCtrlSetBurnIn(void);

void COsdCtrlSetAudioEQ(UINT8 message);
UINT8 COsdCtrlGetAudioEQ(void);

UINT8 COsdCtrlGetPassword();
void COsdCtrlSetPassword(UINT8 message);

UINT8 COsdCtrlGetVChip();
void COsdCtrlSetVChip(UINT8 message);

UINT8 COsdCtrlGetHPosition();
void COsdCtrlSetHPosition(void);
UINT8 COsdCtrlGetMaxVPosition(void);
UINT8 COsdCtrlGetMinVPosition(void);
UINT8 COsdCtrlGetVPosition(void);
void COsdCtrlSetVPosition(void);

UINT8 COsdCtrlGetClock();
void COsdCtrlSetClock(void);

UINT8 COsdCtrlGetPhase();
void COsdCtrlSetPhase(void);

UINT8 COsdCtrlGetRatingTV(void);
void COsdCtrlSetRatingTV(UINT8 message);

UINT8 COsdCtrlGetRatingUSAMovie(void);
void COsdCtrlSetRatingUSAMovie(UINT8 message);

UINT8 COsdCtrlGetRatingCAFRMovie(void);
void COsdCtrlSetRatingCAFRMovie(UINT8 message);

UINT8 COsdCtrlGetRatingCAENMovie(void);
void COsdCtrlSetRatingCAENMovie(UINT8 message);

UINT8 COsdCtrlGetRatingTV_Option(UINT8 ucRating, UINT8 ucOption);
void COsdCtrlSetRatingTV_Option(UINT8 ucRating, UINT8 ucOption);



INT32 CAdjustColorGammaCal(UINT8 color, UINT8 n);
void CAdjustHueSatSet(UINT8 color, INT16 TempU1, INT16 TempU2, INT16 TempV1, INT16 TempV2);

// ** Sleep mode control API
enum _SLEEP_MODE
{
    _SLEEP_MODE_OFF = 0,
    _SLEEP_MODE_30,
    _SLEEP_MODE_60,
    _SLEEP_MODE_90,
    _SLEEP_MODE_120,
    _SLEEP_MODE_150,
    _SLEEP_MODE_MAX,
};
void ap_osdctrl_set_sleep_mode(UINT8 message);
UINT8 ap_osdctrl_get_sleep_mode(void);


UINT8 COsdCtrlGetRatingTV_OptionLock(UINT8 ucRating, UINT8 ucOption);
void COsdCtrlSetRatingTV_OptionLock(UINT8 ucRating, UINT8 ucOption);

UINT8 COsdCtrlGetCountrySelect(void);
void COsdCtrlSetCountrySelect(void);
void  COsdCtrlSetChanelAFC(void);
UINT8 COsdCtrlGetChanelAFC(void);
void  COsdCtrlSetChanelSkip(void);
UINT8 COsdCtrlGetChanelSkip(void);
void COsdCtrlResetToDefault(void);

// Factory mode menu
UINT8 COsdCtrlGetFacColorTemp(void);
void COsdCtrlSetFacColorTemp(void);
UINT8 COsdCtrlGetRGBOffset(void);
void COsdCtrlSetRGBOffset(UINT8 message);
UINT8 COsdCtrlGetRGain(void);
void COsdCtrlSetRGain(UINT8 message);
UINT8 COsdCtrlGetGGain(void);
void COsdCtrlSetGGain(UINT8 message);
UINT8 COsdCtrlGetBGain(void);
void COsdCtrlSetBGain(UINT8 message);
UINT8 COsdCtrlGetAdcROffset(void);
void COsdCtrlSetAdcROffset(UINT8 message);
UINT8 COsdCtrlGetAdcGOffset(void);
void COsdCtrlSetAdcGOffset(UINT8 message);
UINT8 COsdCtrlGetAdcBOffset(void);
void COsdCtrlSetAdcBOffset(UINT8 message);
UINT8 COsdCtrlGetAdcRGain(void);
void COsdCtrlSetAdcRGain(UINT8 message);
UINT8 COsdCtrlGetAdcGGain(void);
void COsdCtrlSetAdcGGain(UINT8 message);
UINT8 COsdCtrlGetAdcBGain(void);
void COsdCtrlSetAdcBGain(UINT8 message);
void COsdCtrlSetFacAutoColor(void);
UINT8 COsdCtrlGetSpreadSpectrum(void);
void COsdCtrlSetSpreadSpectrum(UINT8 message);



UINT8 COsdCtrlGetBrightness_0(void);
UINT8 COsdCtrlGetBrightness_50(void);
UINT8 COsdCtrlGetBrightness_100(void);
UINT8 COsdCtrlGetContrast_0(void);
UINT8 COsdCtrlGetContrast_50(void);
UINT8 COsdCtrlGetContrast_100(void);
UINT8 COsdCtrlGetFacSaturation0(void);
UINT8 COsdCtrlGetFacSaturation50(void);
UINT8 COsdCtrlGetFacSaturation100(void);






#if defined(CONFIG_CHILD_LOCK)
UINT8 COsdCtrlGetParentalLock(void);
void COsdCtrlSetParentalLock(UINT8 message);
void COsdCtrlEnterChildLockMenu(void);
#endif //defined(CONFIG_CHILD_LOCK)
UINT8 ap_osdctrl_get_childlock(void);


void COsdCtrlSetFactoryReset(UINT8 message);
UINT8 COsdCtrlGetFactoryReset(void);
void COsdCtrlSetReset(void);
UINT8 COsdCtrlGetAutoAdjust(void);

#define _BLUE_SCREEN		1
#define _BLACK_SCREEN		2
void COsdFxScalerForceToBackground(UINT8 ucEnable);

void flow_osdctrl_system_Init(void);


void COsdMenuAutoAdjust(void);
UINT8 COsdCtrlGetTVBlueScreen(void);
void COsdCtrlSetTVBlueScreen(void);
void COsdCtrlChannelScan(void);
void COsdCtrlSetAudioRemapVolume(UINT8 osd_vol);
INT16 COsdCtrlGetAudioRemapVolume(UINT8 osd_vol);


/** Picture Mode */
UINT8 COsdCtrlGetFacPictureMode(void);
void COsdCtrlSetFacPictureMode(void);
void COsdCtrlSetPicture1Brightness(void);
UINT8 COsdCtrlGetPicture1Brightness(void);
void COsdCtrlSetPicture1Contrast(void);
UINT8 COsdCtrlGetPicture1Contrast(void);
void COsdCtrlSetPicture1Color(void);
UINT8 COsdCtrlGetPicture1Color(void);
void COsdCtrlSetBrightness_0(void);
UINT8 COsdCtrlGetBrightness_0(void);
void COsdCtrlSetBrightness_50(void);
UINT8 COsdCtrlGetBrightness_50(void);
void COsdCtrlSetBrightness_100(void);
UINT8 COsdCtrlGetBrightness_100(void);
void COsdCtrlSetContrast_0(void);
UINT8 COsdCtrlGetContrast_0(void);
void COsdCtrlSetContrast_50(void);
UINT8 COsdCtrlGetContrast_50(void);
void COsdCtrlSetContrast_100(void);
UINT8 COsdCtrlGetContrast_100(void);
void COsdCtrlSetFacSaturation0(void);
UINT8 COsdCtrlGetFacSaturation0(void);
void COsdCtrlSetFacSaturation50(void);
UINT8 COsdCtrlGetFacSaturation50(void);
void COsdCtrlSetFacSaturation100(void);
UINT8 COsdCtrlGetFacSaturation100(void);
void COsdCtrlSetFacAudioEQ(void);
UINT8 COsdCtrlGetFacAudioEQ(void);
void COsdCtrlSetFacSoundMode300Hz(void);
UINT8 COsdCtrlGetFacSoundMode300Hz(void);
void COsdCtrlSetFacSoundMode1KHz(void);
UINT8 COsdCtrlGetFacSoundMode1KHz(void);
void COsdCtrlSetFacSoundMode2KHz(void);
UINT8 COsdCtrlGetFacSoundMode2KHz(void);
void COsdCtrlSetFacSoundMode3KHz(void);
UINT8 COsdCtrlGetFacSoundMode3KHz(void);
void COsdCtrlSetFacSoundMode8KHz(void);
UINT8 COsdCtrlGetFacSoundMode8KHz(void);
void COsdCtrlSetFacSoundMode15KHz(void);
UINT8 COsdCtrlGetFacSoundMode15KHz(void);
void COsdCtrlSetFacVolume1(UINT8 ucVal);
UINT8 COsdCtrlGetFacVolume1(void);
void COsdCtrlSetFacVolume25(UINT8 ucVal);
UINT8 COsdCtrlGetFacVolume25(void);
void COsdCtrlSetFacVolume50(UINT8 ucVal);
UINT8 COsdCtrlGetFacVolume50(void);
void COsdCtrlSetFacVolume75(UINT8 ucVal);
UINT8 COsdCtrlGetFacVolume75(void);
void COsdCtrlSetFacVolume100(UINT8 ucVal);
UINT8 COsdCtrlGetFacVolume100(void);
void COsdCtrlSetFacTVVolume1(UINT8 ucVal);
UINT8 COsdCtrlGetFacTVVolume1(void);
void COsdCtrlSetFacTVVolume25(UINT8 ucVal);
UINT8 COsdCtrlGetFacTVVolume25(void);
void COsdCtrlSetFacTVVolume50(UINT8 ucVal);
UINT8 COsdCtrlGetFacTVVolume50(void);
void COsdCtrlSetFacTVVolume75(UINT8 ucVal);
UINT8 COsdCtrlGetFacTVVolume75(void);
void COsdCtrlSetFacTVVolume100(UINT8 ucVal);
UINT8 COsdCtrlGetFacTVVolume100(void);
void COsdSetPWM1(UINT8 level);
UINT8 COsdGetPWM1(void);
void COsdCtrlSetUART(UINT8 ucVal);
UINT8 COsdCtrlGetUART(void);
void COsdCtrlSetFacEmi_DCLKSS_RANGE(UINT8 ucVal);
UINT8 COsdCtrlGetFacEmi_DCLKSS_RANGE(void);
void COsdCtrlSetFacEmi_DCLKSS_FMDIV(UINT8 ucVal);
UINT8 COsdCtrlGetFacEmi_DCLKSS_FMDIV(void);
void COsdCtrlDNR(void);
#ifdef CONFIG_VBI_CC_SUPPORT
UINT8 COsdCtrlGetCCtype(void);
void COsdCtrlSetCCtype(void);
#endif

#endif // #ifndef _OSDCONTROL_H


/**
 *
 * @}
 */

