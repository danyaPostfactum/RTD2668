/*===========================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2009
 * All rights reserved.
 * ========================================================== */

/*==================== File Description ===================== */
/**
 * @file
 * 	This file is osd flow definition for osd related functions.
 *
 * @author 	$Author: cf.tsai $
 * @date 	$Date: 2009-03-16 $
 * @version $Revision: 10 $
 */

/**
 * @addtogroup osd
 * @{
 */

#define _OSDFLOW_C
/*===================== Module dependency  ================== */
#include "autoconf.h"
#include "rtd_types.h"
#include "flow_struct.h"
#include "osdflow.h"
#include "osdobjectdef.h"
#include "osdparameter.h"
#include "osdcontrol.h"
#include "osdstring.h"
#include "osdcore.h"
#include "ui_struct.h"
#include "pcb_customer.h"
#include "ifd\ifd.h"
#include "tv.h"

extern ItemImmediate code tAutoColor_Item ;

/*======================= Private Types ===================== */

/*======================== Definitions ====================== */


/*========================== Variables ====================== */


ItemSlider code tVolumeSlider = {_SLIDER_POPON, 20/*100*/, 0, MENUITEM_AUDIO_VOLUME};
ItemObject code tVolumeItem = {tOsdVolumnStr, _SLIDER_TYPE, (void*)&tVolumeSlider};

ItemSlider code tChannelScanSlider = {_SLIDER_VBR_RANGE|_SLIDER_POPON, 100, 0, MENUITEM_CHANNEL_SCAN};
ItemObject code tChannelScanItem = {tOsdAutoScanStr, _SLIDER_TYPE, (void*)&tChannelScanSlider};
#if defined(CONFIG_CHANNEL_TABLE_SCAN)
ItemSlider code tTableScanSlider = {_SLIDER_POPON, 100, 0, MENUITEM_TABLE_SCAN};
ItemObject code tTableScanItem = {tOsdTableScanStr, _SLIDER_TYPE, (void*)&tTableScanSlider};
#endif
ItemSlider code tManualScanSlider = {_SLIDER_VBR_RANGE|_SLIDER_POPON, 100, 0, MENUITEM_MANUAL_SCAN};
ItemObject code tManualScanItem = {tStr_ManualScan, _SLIDER_TYPE, (void*)&tManualScanSlider};
#if DISABLE_OPTTION
ItemSelection code tSleepModeSelectionSet[] =
{
    {tOsdSleepNoneStr, 0},
    {tOsdSleep30Str, 1},
    {tOsdSleep60Str, 2},
    {tOsdSleep90Str, 3},
    {tOsdSleep120Str, 4},
    {tOsdSleep150Str, 5}
};

ItemList code tSleepModeList = {tOsdSleepTimerStr, 6, 0, tSleepModeSelectionSet, MENUITEM_SLEEP_MODE};
#endif

ItemSelection code tDisplayRatioSet[] =
{
    {tStr_display_ratio_4x3, RATIO_4_3},
    {tStr_display_ratio_16x9, RATIO_16_9},
//#if defined(CONFIG_WSS_SUPPORT)
    //{tStr_display_ratio_auto, RATIO_AUTO},
//#endif
//		{tStr_display_ratio_panorama, RATIO_PANORAMA},
};
ItemList code tDisplayRatioList = {tOsdDisplayRatioStr, sizeof(tDisplayRatioSet)/sizeof(ItemSelection), 0, tDisplayRatioSet, MENUITEM_DISPLAY_RATIO};

ItemSelection code tDisplayIRSet[] =
{
    {tStr_display_IR_OFF, IR_OFF},
    {tStr_display_IR_A,   IR_A},
//#if defined(CONFIG_WSS_SUPPORT)
    {tStr_display_IR_B,   IR_B}
//#endif
//		{tStr_display_ratio_panorama, RATIO_PANORAMA},
};
ItemList code tDisplayIRList = {tOsdDisplayIrStr, sizeof(tDisplayIRSet)/sizeof(ItemSelection), 0, tDisplayIRSet, MENUITEM_DISPLAY_IR};


#if CONFIG_ENABLE_FM
#if ENABLE_FM_FREQSET
ItemBracket code tDisplayRadioTuneSet = {tOsdDisplayRadioStr, 8, 322, MENUITEM_DISPLAY_RADIO};
#else
ItemSelection code tDisplayRadioSet[] =
{
	{tStr_display_OFF, RADIO_OFF},
    {tStr_display_877, RADIO_877},
    {tStr_display_879, RADIO_879},
	{tStr_display_881, RADIO_881},
	{tStr_display_883, RADIO_883},
	{tStr_display_885, RADIO_885},
	{tStr_display_887, RADIO_887},
	{tStr_display_889, RADIO_889},
	{tStr_display_891, RADIO_891}
//#if defined(CONFIG_WSS_SUPPORT)
   // {tStr_display_IR_B,   IR_B}
//#endif
//		{tStr_display_ratio_panorama, RATIO_PANORAMA},
};
ItemList code tDisplayRadioList = {tOsdDisplayRadioStr, sizeof(tDisplayRadioSet)/sizeof(ItemSelection), 0, tDisplayRadioSet, MENUITEM_DISPLAY_RADIO};
#endif

#endif

/** ========================================================================== */
#define _SOURCE_MENU_ITEMS_
ItemSelection code tSourceSelectionSet[] =
{
#if 0//Tim 0325
    {tStrSignal_VGA,		_SOURCE_INDEX_VGA},
    #ifdef CONFIG_ENABLE_HDMI
    {tStrSignal_HDMI_0,		_SOURCE_INDEX_HDMI0},
    #endif
    //{tStrSignal_HDMI_1,	_SOURCE_INDEX_HDMI1},
    #ifdef _VIDEO_TV_SUPPORT
    {tStrSignal_TV,			_SOURCE_INDEX_TV},
    #endif
    {tStrSignal_CVBS_0,		_SOURCE_INDEX_AV},
    {tStrSignal_CVBS_1,		_SOURCE_INDEX_AV_1},
    {tStrSignal_SV_0,		_SOURCE_INDEX_SV},
    {tStrSignal_YPbPr_0,	_SOURCE_INDEX_YPBPR},
    //{tStrSignal_YPbPr_1, _SOURCE_INDEX_YPbPr1},
#ifdef CONFIG_SCART_AUTO_SWITCH
    {tStrSignal_Scart_0,	_SOURCE_INDEX_SCART_0},
    {tStrSignal_Scart_1,	_SOURCE_INDEX_SCART_1},
#endif
#else
	#ifdef _VIDEO_TV_SUPPORT
    //{tStrSignal_TV,			_SOURCE_INDEX_TV},
	#endif
	//{tStrSignal_YPbPr_0,    _SOURCE_INDEX_YPBPR},
	#if CONFIG_ENABLE_USB
	{tStrSignal_Usb_0,    _SOURCE_INDEX_USB},
	#endif
    {tStrSignal_CVBS_0,		_SOURCE_INDEX_AV},
    {tStrSignal_CVBS_2,		_SOURCE_INDEX_AV_2},
    {tStrSignal_CVBS_3,		_SOURCE_INDEX_AV_3},
    //{tStrSignal_SV_0,       _SOURCE_INDEX_SV}, 
	#if _VIDEO_VGA_SUPPORT
    {tStrSignal_VGA,		_SOURCE_INDEX_VGA},
    #endif
    #ifdef CONFIG_ENABLE_HDMI
    {tStrSignal_HDMI_0,		_SOURCE_INDEX_HDMI0},
    #endif
#if  CONFIG_TU300
	{tStrSignal_CVBS_1,	   _SOURCE_INDEX_AV_1},//DTV
#endif

    //{tStrSignal_YPbPr_1, _SOURCE_INDEX_YPbPr1},
#ifdef CONFIG_SCART_AUTO_SWITCH
    {tStrSignal_Scart_0,	_SOURCE_INDEX_SCART_0},
    {tStrSignal_Scart_1,	_SOURCE_INDEX_SCART_1},
#endif

#endif
};
ItemList code tSourceSelectList = {tStr_source, sizeof(tSourceSelectionSet)/sizeof(ItemSelection), 0, tSourceSelectionSet, MENUITEM_SOURCE_SELECT};

/** ========================================================================== */
#define _PICTURE_SETTING_ITEMS_
#if 1
// Picture Setting Menu Items
ItemSlider code tYBrightnessItem = {_SLIDER_MID_RANGE, 100, 0, MENUITEM_BRIGHTNESS};
ItemSlider code tYConstrastItem = {_SLIDER_MID_RANGE, 100, 0, MENUITEM_CONTRAST};
ItemSlider code tSaturationItem = {_SLIDER_MID_RANGE, 100, 0, MENUITEM_SATURATION};
ItemSlider code tHueItem = {_SLIDER_MID_RANGE, 100, 0, MENUITEM_HUE};
ItemSlider code tSharpnessItem = {_SLIDER_MID_RANGE, 100, 0, MENUITEM_SHARPNESS};
ItemBracket code tDCRItem = {tOsdOnOffStrSet, 0, 2, MENUITEM_DCR};

ItemSelection code tColorTempSet[] =
{
	#if 0
    {tOsdColorTempWarmerStr, 0},
    {tOsdColorTempWarmStr, 1},
    {tOsdColorTempNormalStr, 2},
    {tOsdColorTempCoolStr, 3},
    {tOsdColorTempCoolerStr, 4}
    #else
    {tOsdColorTempWarmStr, 0},
    {tOsdColorTempNormalStr, 1},
    {tOsdColorTempCoolStr, 2}   
    #endif
};
ItemList code tColorTempList = {tOsdColorTempStr, 3, 0, tColorTempSet, MENUITEM_COLOR_TEMPERATURE};

ItemSelection code tDNRSelectionSet[] =
{
    {tOsdDNROffStr, 0},
    {tOsdDNRLowStr, 1},
    {tOsdDNRMidStr, 2},
    {tOsdDNRHighStr, 3}
};
ItemList code tDNRList = {tOsdDNRStr, 4, 0, tDNRSelectionSet, MENUITEM_DNR};

ItemSelection code tPictureModeSelectionSet[] =
{
    {tStr_PictureMode_Vivid, 0},
    {tStr_PictureMode_Standard, 1},
    {tStr_PictureMode_Gentle, 2},
    {tStr_PictureMode_User, 3}
};
ItemList code tPictureModeList = {tStr_PictureMode, 4, 0, tPictureModeSelectionSet, MENUITEM_PICTURE_MODE};

// Image Setting Menu
ItemObject code tMenuImageItemSet[] =
{
    {tOsdBrightnessStr, _SLIDER_TYPE, (void*)&tYBrightnessItem},
    {tOsdContrastStr, _SLIDER_TYPE, (void*)&tYConstrastItem},
    {tOsdHueStr, _SLIDER_TYPE, (void*)&tHueItem},
    {tOsdSaturationStr, _SLIDER_TYPE, (void*)&tSaturationItem},
    {tOsdSharpnessStr, _SLIDER_TYPE, (void*)&tSharpnessItem},
    {tOsdColorTempStr, _LIST_TYPE, (void*)&tColorTempList},
    #if _DISABLE_ITEM_OPTION
    {tOsdDNRStr, _LIST_TYPE, (void*)&tDNRList},
    {tOsdDCRStr, _BRACKET_TYPE, (void*)&tDCRItem},
    #endif
};
MenuItem code tMenuImageSetting = {tStr_picture_title, sizeof(tMenuImageItemSet)/sizeof(ItemObject), MAINMENU_COLOR, tMenuImageItemSet};
#endif

/** ========================================================================== */
#define _VGA_SETTING_ITEMS_
#if 1
// D-Sub Menu Items
//ItemBracket code tDSubAutoAdjustItem = {tOsdOnOffStrSet, 0, 2,MENUITEM_VGA_AUTO_POSITION};
ItemImmediate code tDSubAutoAdjustItem = {0, MENUITEM_VGA_AUTO_POSITION};
ItemSlider code tDSubClockItem = {_SLIDER_MID_RANGE, 100, 0, MENUITEM_VGA_CLOCK};
ItemSlider code tDSubPhaseItem = {_SLIDER_MID_RANGE, 63, 0, MENUITEM_VGA_PHASE};
ItemSlider code tDSubHPositionItem = {_SLIDER_MID_RANGE, 100, 0, MENUITEM_VGA_H_POSITION};
ItemSlider code tDSubVPositionItem = {_SLIDER_MID_RANGE|_SLIDER_VBR_RANGE, 100, 0, MENUITEM_VGA_V_POSITION};
//	ItemSlider code tDSubHSizeItem     = {_SLIDER_POPON, 10, 0, MENUITEM_VGA_H_SIZE};
//	ItemSlider code tDSubVSizeItem     = {_SLIDER_POPON, 10, 0, MENUITEM_VGA_V_SIZE};

// D-Sub Setting Menu
ItemObject code tMenuDSubItemSet[] =
{
    //Tim 0404
    {tOsdAutoAdjustStr, _IMMEDIATE_TYPE, (void*)&tDSubAutoAdjustItem},
    {tOsdHPositionStr, _SLIDER_TYPE, (void*)&tDSubHPositionItem},
    {tOsdVPositionStr, _SLIDER_TYPE, (void*)&tDSubVPositionItem},
    {tOsdClockStr, _SLIDER_TYPE, (void*)&tDSubClockItem},
    {tOsdPhaseStr, _SLIDER_TYPE, (void*)&tDSubPhaseItem},
//		{tOsdHSizeStr, _SLIDER_TYPE, (void*)&tDSubHSizeItem},
//		{tOsdVSizeStr, _SLIDER_TYPE, (void*)&tDSubVSizeItem},
//		{tOsdColorTempStr, _LIST_TYPE, (void*)&tColorTempList},
//		{tOsdAutoColorStr, _IMMEDIATE_TYPE, (void*)&tAutoColor_Item},
};
MenuItem code tMenuDSubSetting = {tStr_screen_title, sizeof(tMenuDSubItemSet)/sizeof(ItemObject), MAINMENU_VGA, tMenuDSubItemSet};
#endif

/** ========================================================================== */
#define _SOUND_SETTING_ITEMS_
#if 1
// Sound setting Items
ItemSlider code tAudioBalanceItem = {_SLIDER_MID_RANGE, _OSD_AUDIO_BALANCE_MAX, _OSD_AUDIO_BALANCE_MIN, MENUITEM_AUDIO_BALANCE};
ItemBracket code tAudioSpatialItem = {tOsdOnOffStrSet, _BIT0, 2, MENUITEM_AUDIO_SPATIAL};

ItemSlider code tAudioBassItem = {_SLIDER_MID_RANGE, _OSD_AUDIO_BASS_MAX, _OSD_AUDIO_BASS_MIN, MENUITEM_AUDIO_BASS};
ItemSlider code tAudioTrebleItem = {_SLIDER_MID_RANGE, _OSD_AUDIO_TREBLE_MAX, _OSD_AUDIO_TREBLE_MIN, MENUITEM_AUDIO_TREBLE};

ItemSlider code tMusicMode_300HzItem = {_SLIDER_MID_RANGE, 12, 0, MENUITEM_MUSIC_MODE_300HZ};
ItemSlider code tMusicMode_1KHzItem  = {_SLIDER_MID_RANGE, 12, 0, MENUITEM_MUSIC_MODE_1KHZ};
ItemSlider code tMusicMode_2KHzItem  = {_SLIDER_MID_RANGE, 12, 0, MENUITEM_MUSIC_MODE_2KHZ};
ItemSlider code tMusicMode_3KHzItem  = {_SLIDER_MID_RANGE, 12, 0, MENUITEM_MUSIC_MODE_3KHZ};
ItemSlider code tMusicMode_8KHzItem  = {_SLIDER_MID_RANGE, 12, 0, MENUITEM_MUSIC_MODE_8KHZ};
ItemSlider code MusicMode_15KHzItem = {_SLIDER_MID_RANGE, 12, 0, MENUITEM_MUSIC_MODE_15KHZ};
ItemObject code tEQModeItems[] =
{
    {tStr_200Hz, _SLIDER_TYPE, (void*)&tMusicMode_300HzItem},
    {tStr_300Hz, _SLIDER_TYPE, (void*)&tMusicMode_1KHzItem},
    {tStr_1KHz, _SLIDER_TYPE, (void*)&tMusicMode_2KHzItem},
    {tStr_3KHz, _SLIDER_TYPE, (void*)&tMusicMode_3KHzItem},
    {tStr_8KHz, _SLIDER_TYPE, (void*)&tMusicMode_8KHzItem},
    {tStr_15KHz, _SLIDER_TYPE, (void*)&MusicMode_15KHzItem},
};
MenuItem code tAudioEqItem = {tStr_Audio_Equalizer, sizeof(tEQModeItems)/sizeof(ItemObject), SUBMENU_MUSIC_MODE, tEQModeItems};
ItemSelection code tEQModeSelectionSet[] =
{
    {tStr_EQ_Disable, 0},
    {tStr_EQ_Club, 1},
    {tStr_EQ_Dance, 2},
    {tStr_EQ_Live, 3},
    {tStr_EQ_Pop, 4},
    {tStr_EQ_Rock, 5},
    {tStr_EQ_User, 6}
};
ItemList code tEQModeList = {tStr_MusicMode, 7, 0, tEQModeSelectionSet, MENUITEM_MUSIC_MODE};

ItemSelection code tMTSModeSelectionSet[] =
{
    {tStr_Mono, 0},
    {tStr_Stereo, 1},
    {tStr_Dual, 2},
};
ItemList code tMTSModeList = {tStr_MTS, 3, 0, tMTSModeSelectionSet, MENUITEM_MTS_MODE};


// Audio Setting Menu
ItemObject code tMenuAudioItemSet[] =
{
    {tOsdAudioBalanceStr, _SLIDER_TYPE, (void*)&tAudioBalanceItem},
    //{tOsdAudioSpatialStr, _BRACKET_TYPE, (void*)&tAudioSpatialItem},
    {tOsdAudioBassStr, _SLIDER_TYPE, (void*)&tAudioBassItem},
    {tOsdAudioTrebleStr, _SLIDER_TYPE, (void*)&tAudioTrebleItem},
    //{tStr_Audio_Equalizer, _SUBPAGE_TYPE, (void*)&tAudioEqItem},//Tim 0413
};
MenuItem code tMenuAudioSetting = {tStr_sound_title, sizeof(tMenuAudioItemSet)/sizeof(ItemObject), MAINMENU_AUDIO, tMenuAudioItemSet};
#endif

/** ========================================================================== */
#define _CHANNEL_SETTING_ITEMS_
#if 1
// Channel setting Items
ItemSelection code tColorSystemSelectionSet[] =
{
    {tStr_AUTO, _SIG_MODE_AUTO},
    #if _TV_PAL_SUPPORT
    {tStr_PAL, _SIG_MODE_PAL},
	#endif

	#if _TV_NTSC_SUPPORT
    {tStr_NTSC, _SIG_MODE_NTSC},
	#endif

	#if _TV_NTSC_443_SUPPORT
    {tStr_NTSC443, _SIG_MODE_NTSC_443},
	#endif
	
	#if defined(CONFIG_APP_EURO)
	#if _TV_SECAM_SUPPORT
	{
        tStr_SECAM, _SIG_MODE_SECAM
		
    }
    #endif
	#endif//Edit SVN53
};
//Tim 0323
//==============================================================================
ItemSelection code tNtscSystemSelectionSet[] =
{
    {tStr_Cable, 0},
    {tStr_AIR, 1},

};

ItemList code tNtscSystemList = {tStr_Ntsc_Type, sizeof(tNtscSystemSelectionSet)/sizeof(ItemSelection), 0, tNtscSystemSelectionSet, MENUITEM_TV_NTSC_STD};

// NTSC FREE Select
ItemSelection code tNtscFreeSelectionSet[] =
{
    {tOsdOffTableStr, 0},
    {tOsdOnTableStr,  1},

};

ItemList code tNtscFreeList     = {tStr_NtscFree, sizeof(tNtscFreeSelectionSet)/sizeof(ItemSelection), 0, tNtscFreeSelectionSet, MENUITEM_NTSC_FREE};

//==============================================================================

ItemList code tColorSystemList = {tStr_ColorSystem, sizeof(tColorSystemSelectionSet)/sizeof(ItemSelection), 0, tColorSystemSelectionSet, MENUITEM_TV_VIDEO_STD};

ItemSelection code tSoundSystemSelectionSet[] =
{	
    {tStr_AUTO, _SOUND_AUTO},
    #if _TV_DK_SUPPORT
    {tStr_DK, _SOUND_DK},
    #endif

    #if _TV_I_SUPPORT
    {tStr_I, _SOUND_I},
    #endif

    #if _TV_BG_SUPPORT
    {tStr_BG, _SOUND_BG},
    #endif

    #if _TV_M_SUPPORT
    {tStr_M, _SOUND_MN},
    #endif

    #if _TV_N_SUPPORT
    {tStr_N, _SOUND_N },
    #endif
    
	#if defined(CONFIG_APP_EURO)
	#if _TV_L_SUPPORT
    {
        tStr_L, _SOUND_L
        
    },
    #endif
	#endif//Edit SVN53

//		{tStr_LL, 5},
};

ItemList code tSoundSystemList = {tStr_SoundSystem, sizeof(tSoundSystemSelectionSet)/sizeof(ItemSelection), 0, tSoundSystemSelectionSet, MENUITEM_TV_AUDIO_STD};
//ItemSlider code tChannelFineTuneSet = {_SLIDER_MID_RANGE, 48, 0, MENUITEM_TV_FREQ_FINE_TUNE};
ItemBracket code tChannelFineTuneSet = {tStr_FineTune, 8, 48, MENUITEM_TV_FREQ_FINE_TUNE};
ItemBracket code tChannelSkipSet = {tOsdOnOffStrSet, 0, 2, MENUITEM_CHANNEL_SKIP};
ItemBracket code tAFCSet = {tOsdOnOffStrSet, 0, 2, MENUITEM_AFC};
ItemImmediate code tTvAutoScanItem = {0, MENUITEM_TV_AUTO_SCAN};
#if defined(CONFIG_CHANNEL_TABLE_SCAN)
ItemImmediate code tTvTableScanItem = {0, MENUITEM_TV_TABLE_SCAN};
#endif
ItemImmediate code tTvManualScanItem = {0, MENUITEM_TV_MANUAL_SCAN};
#if defined(_REALTEK_OSD_USE)
ItemImmediate code tTv_Channel_Edit_Item = {0, MENUITEM_TV_CHANNEL_EDIT};
#endif
//ItemBracket code tTvCountrySelectItem = {tOsdCountryStrSet, 0, 24, MENUITEM_TV_COUNTRY};
//	ItemBracket code tBlueScreenItem = {tOsdOnOffStrSet, _BIT0, 2, MENUITEM_TV_BLUE_SCREEN};
ItemBracket code tTvAVLItem = {tOsdOnOffStrSet, 0, 2, MENUITEM_TV_AVL};
ItemBracket code tTvSpeakerCompensationItem = {tOsdOnOffStrSet, 0, 2, MENUITEM_TV_SPEAKER_COMPENSATION};

// Channel Setting Menu
ItemObject code tMenuTvItemSet[] =
{
    {tStr_ColorSystem, _LIST_TYPE, (void*)&tColorSystemList},
    {tStr_SoundSystem, _LIST_TYPE, (void*)&tSoundSystemList},
    {tStr_FineTune, _BRACKET_TYPE, (void*)&tChannelFineTuneSet},
    //Tim 0323
    //==========================================================================
    {tStr_Ntsc_Type,   _LIST_TYPE, (void*)&tNtscSystemList},
    
    {tStr_NtscFree,	   _LIST_TYPE, (void*)&tNtscFreeList},
	//==========================================================================
	
    {tOsdAutoScanStr, _IMMEDIATE_TYPE, (void*)&tTvAutoScanItem},
#if defined(CONFIG_CHANNEL_TABLE_SCAN)
    {
        tOsdTableScanStr, _IMMEDIATE_TYPE, (void*)&tTvTableScanItem
    },
#endif
    //{tStr_ManualScan, _IMMEDIATE_TYPE, (void*)&tTvManualScanItem},
    {tStr_Skip, _BRACKET_TYPE, (void*)&tChannelSkipSet},
    {tStr_AFC, _BRACKET_TYPE, (void*)&tAFCSet},
    //{tStr_Channel_Edit, _IMMEDIATE_TYPE, (void*)&tTv_Channel_Edit_Item},
    //{tOsdCountrySelectStr, _BRACKET_TYPE, (void*)&tTvCountrySelectItem},
};
MenuItem code tMenuTV = {tStr_channel_title, sizeof(tMenuTvItemSet)/sizeof(ItemObject), MAINMENU_CHANNEL, tMenuTvItemSet};
#endif

/** ========================================================================== */
#define _FUNCTION_SETTING_ITEMS_
#if 1
// Tool Menu Items
ItemSlider code tMiscOsdTimeOutItem = {_SLIDER_NONLINEAR, 60, 5, MENUITEM_OSD_TIMEOUT};
ItemSlider code tMiscOsdBlendItem = {0, 4, 0, MENUITEM_OSD_BLEND};
#if defined(CONFIG_EUROPE_LANGUAGE)
ItemBracket code tMiscLanguageItem = {tOsdLanguageStrSet, 0, 14, MENUITEM_OSD_LANGUAGE};
#endif
ItemImmediate code tOsdResetItem = {0, MENUITEM_OSD_RESET};
#ifdef CONFIG_VBI_CC_SUPPORT
ItemBracket code tCcSelItem = {tOsdCCSelStrSet, 0, 3, MENUITEM_OSD_CC};
#endif

ItemObject code tOsdSettingItems[] =
{
    {tOsdOsdTimeStr, _SLIDER_TYPE, (void*)&tMiscOsdTimeOutItem},
    {tOsdOsdBlendStr, _SLIDER_TYPE, (void*)&tMiscOsdBlendItem},
};
MenuItem code tOsdSettingMenu = {tStr_Osd_Setting, sizeof(tOsdSettingItems)/sizeof(ItemObject), SUBMENU_OSD_SETTING, tOsdSettingItems};

// Tool Menu
ItemObject code tMenuToolItemSet[] =
{
#if defined(CONFIG_EUROPE_LANGUAGE)
    {
        tOsdLanguageStr, _BRACKET_TYPE, (void*)&tMiscLanguageItem
    },
#endif
    {tStr_Osd_Setting, _SUBPAGE_TYPE, (void*)&tOsdSettingMenu},
//		{tOsdBlueScreenStr, _BRACKET_TYPE, (void*)&tBlueScreenItem},
    {tOsdDisplayRatioStr, _LIST_TYPE, (void*)&tDisplayRatioList},
     {tOsdDisplayIrStr, _LIST_TYPE, (void*)&tDisplayIRList},
     #if CONFIG_ENABLE_FM
	#if ENABLE_FM_FREQSET
	 {tOsdDisplayRadioStr, _BRACKET_TYPE, (void*)&tDisplayRadioTuneSet},
	#else
	 {tOsdDisplayRadioStr, _LIST_TYPE, (void*)&tDisplayRadioList},
	 #endif
	 #endif
#ifdef CONFIG_VBI_CC_SUPPORT
        {tStr_Cctype, _BRACKET_TYPE, (void*)&tCcSelItem},
#endif
    {tOsdResetStr, _IMMEDIATE_TYPE, (void*)&tOsdResetItem},
};
MenuItem code tMenuTool = {tStr_setup_title, sizeof(tMenuToolItemSet)/sizeof(ItemObject), MAINMENU_TOOL, tMenuToolItemSet};
#endif


/** ========================================================================== */
/**
 * OSD main menu set.
 */
MenuItem* code stAVMainMenuItems[] = {&tMenuImageSetting, /*&tMenuAudioSetting,*/ &tMenuTool};
MenuItemSet code stAVMainMenuItemSet = {NULL, sizeof(stAVMainMenuItems)/sizeof(MenuItem*), stAVMainMenuItems};

MenuItem* code stVGAMainMenuItems[] = {&tMenuImageSetting, &tMenuAudioSetting, &tMenuDSubSetting, &tMenuTool};
MenuItemSet code stVGAMainMenuItemSet = {NULL, sizeof(stVGAMainMenuItems)/sizeof(MenuItem*), stVGAMainMenuItems};

MenuItem* code stHDMIMainMenuItems[] = {&tMenuImageSetting,/* &tMenuAudioSetting,*/ &tMenuTool};
MenuItemSet code stHDMIMainMenuItemSet = {NULL, sizeof(stHDMIMainMenuItems)/sizeof(MenuItem*), stHDMIMainMenuItems};




/** ========================================================================== */
/**
 * Factory mode items.
 */

/* Factory mode 'Setting' menu */
/* White Balance */
ItemSlider code tFac_WB_R_Gain = {0, 255, 0, MENUITEM_FAC_WB_R_GAIN};
ItemSlider code tFac_WB_G_Gain = {0, 255, 0, MENUITEM_FAC_WB_G_GAIN};
ItemSlider code tFac_WB_B_Gain = {0, 255, 0, MENUITEM_FAC_WB_B_GAIN};
ItemSlider code tFac_WB_R_Offset = {0, 255, 0, MENUITEM_FAC_WB_R_OFFSET};
ItemSlider code tFac_WB_G_Offset = {0, 255, 0, MENUITEM_FAC_WB_G_OFFSET};
ItemSlider code tFac_WB_B_Offset = {0, 255, 0, MENUITEM_FAC_WB_B_OFFSET};
ItemSlider code tFac_WB_HBri = {0, 100, 0, MENUITEM_FAC_WB_H_BRI};
ItemSlider code tFac_WB_HCont   = {0, 100, 0, MENUITEM_FAC_WB_H_CONT};
ItemSlider code tFac_WB_LBri = {0, 100, 0, MENUITEM_FAC_WB_L_BRI};
ItemSlider code tFac_WB_LCont   = {0, 100, 0, MENUITEM_FAC_WB_L_CONT};
ItemObject code tFac_WB_ItemSet[] =
{
    {tStr_WB_R_Gain, _SLIDER_TYPE, (void*)&tFac_WB_R_Gain},
    {tStr_WB_G_Gain, _SLIDER_TYPE, (void*)&tFac_WB_G_Gain},
    {tStr_WB_B_Gain, _SLIDER_TYPE, (void*)&tFac_WB_B_Gain},
    {tStr_WB_R_Offset, _SLIDER_TYPE, (void*)&tFac_WB_R_Offset},
    {tStr_WB_G_Offset, _SLIDER_TYPE, (void*)&tFac_WB_G_Offset},
    {tStr_WB_B_Offset, _SLIDER_TYPE, (void*)&tFac_WB_B_Offset},
    {tStr_WBH_Bri, _SLIDER_TYPE, (void*)&tFac_WB_HBri},
    {tStr_WBH_Cont, _SLIDER_TYPE, (void*)&tFac_WB_HCont},
    {tStr_WBL_Bri, _SLIDER_TYPE, (void*)&tFac_WB_LBri},
    {tStr_WBL_Cont, _SLIDER_TYPE, (void*)&tFac_WB_LCont}
};
MenuItem code tFac_WB_Menu = {tStrWhiteBalance, sizeof(tFac_WB_ItemSet)/sizeof(ItemObject), FAC_SUBMENU_WB, tFac_WB_ItemSet};

/* Color Temperature */
ItemBracket code tFac_ColorTemp_Type = {tColorTempStrSet, 0, 5, MENUITEM_FAC_COLOR_TEM};
ItemSlider code tFac_ColorTemp_R = {0, 255, 0, MENUITEM_FAC_COLOR_TEM_R};
ItemSlider code tFac_ColorTemp_G = {0, 255, 0, MENUITEM_FAC_COLOR_TEM_G};
ItemSlider code tFac_ColorTemp_B = {0, 255, 0, MENUITEM_FAC_COLOR_TEM_B};
ItemObject code tFac_ColorTemp_ItemSet[] =
{
    {tStr_ColorTemp, _BRACKET_TYPE, (void*)&tFac_ColorTemp_Type},
    {tStr_ColorTemp_Red, _SLIDER_TYPE, (void*)&tFac_ColorTemp_R},
    {tStr_ColorTemp_Green, _SLIDER_TYPE, (void*)&tFac_ColorTemp_G},
    {tStr_ColorTemp_Blue, _SLIDER_TYPE, (void*)&tFac_ColorTemp_B}
};
MenuItem code tFac_ColorTemp_Menu = {tStr_ColorTemp, sizeof(tFac_ColorTemp_ItemSet)/sizeof(ItemObject), FAC_SUBMENU_COLORTEMP, tFac_ColorTemp_ItemSet};

/* ADC Calibration */
ItemImmediate code tFac_ADC_Calibration_HD720P = {0, MENUITEM_FAC_ADC_CALIB_HD720P};
ItemImmediate code tFac_ADC_Calibration_VGA = {0, MENUITEM_FAC_ADC_CALIB_VGA};
ItemImmediate code tFac_ADC_Calibration_1BY1 = {0, MENUITEM_FAC_ADC_CALIB_1BY1};
ItemObject code tFac_ADC_Calibration_ItemSet[] =
{
    {tStr_ADC_Calib_HD720P, _IMMEDIATE_TYPE, (void*)&tFac_ADC_Calibration_HD720P},
    {tStr_ADC_Calib_VGA, _IMMEDIATE_TYPE, (void*)&tFac_ADC_Calibration_VGA},
    {tStr_ADC_Calib_1BY1, _IMMEDIATE_TYPE, (void*)&tFac_ADC_Calibration_1BY1}
};
MenuItem code tFac_ADC_Calibration_Menu = {tStr_ADC_Calibration, sizeof(tFac_ADC_Calibration_ItemSet)/sizeof(ItemObject), FAC_SUBMENU_ADC_CALIBRATION, tFac_ADC_Calibration_ItemSet};

/* Option for default setting */
ItemBracket code tFac_Logo = {tDefaultLogo_StrSet, 0, 3, MENUITEM_FAC_LOGO};
#if defined(CONFIG_FAC_DEFAULT_LANG_OPT)
ItemBracket code tFac_Default_Language = {tOsdLanguageStrSet, 0, 13, MENUITEM_FAC_DEFAULT_LANGUAGE};
#endif
ItemObject code tFac_Option_ItemSet[] =
{
    {tStr_Default_Logo, _BRACKET_TYPE, (void*)&tFac_Logo},
#if defined(CONFIG_FAC_DEFAULT_LANG_OPT)
    {
        tStr_Default_Language, _BRACKET_TYPE, (void*)&tFac_Default_Language
    }
#endif
};
MenuItem code tFac_Setting_Option_Menu = {tStr_Option, sizeof(tFac_Option_ItemSet)/sizeof(ItemObject), FAC_SUBMENU_SETTING_OPTION, tFac_Option_ItemSet};

/* Factory Reset */
ItemImmediate code tFac_ClearUserData = {0, MENUITEM_FAC_CLEAR};
ItemImmediate code tFac_ClearAllData = {0, MENUITEM_FAC_CLEAR_ALL};
ItemObject code tFactoryReset_ItemSet[] =
{
    {tStr_Clear, _IMMEDIATE_TYPE, (void*)&tFac_ClearUserData},
    {tStr_ClearAll, _IMMEDIATE_TYPE, (void*)&tFac_ClearAllData}
};
MenuItem code tFac_Reset_Menu = {tStr_Factory_Reset, sizeof(tFactoryReset_ItemSet)/sizeof(ItemObject), FAC_SUBMENU_RESET, tFactoryReset_ItemSet};

/* FW version */
ItemDescription code tFac_OrangeStr = {_DESCRIPTION_COLOR_CTRL, _CP_PURE_YELLOW_ORANGE};
ItemDescription code tFac_WhiteStr  = {_DESCRIPTION_COLOR_CTRL, _CP_WHITE};
ItemDescription code tFac_FwVersion = {_DESCRIPTION_DYNAMIC_STR, MENUITEM_FAC_FW_VERSIOIN};
ItemDescription code tFac_PanelType = {_DESCRIPTION_DYNAMIC_STR, MENUITEM_FAC_PANEL_TYPE};
ItemDescription code tFac_FalshType = {_DESCRIPTION_DYNAMIC_STR, MENUITEM_FAC_FLASH_TYPE};
ItemObject code tFacVersion_ItemSet[] =
{
    {tStr_Version, _DESCRIPTION_TYPE, (void*)&tFac_OrangeStr},
    {tStr_FwVersion, _DESCRIPTION_TYPE, (void*)&tFac_FwVersion},
    {tStr_Empty, _DESCRIPTION_TYPE, (void*)&tFac_WhiteStr},
    {tStr_Panel, _DESCRIPTION_TYPE, (void*)&tFac_OrangeStr},
    {tStr_PanelType, _DESCRIPTION_TYPE, (void*)&tFac_PanelType},
    {tStr_Empty, _DESCRIPTION_TYPE, (void*)&tFac_WhiteStr},
    {tStr_Falsh, _DESCRIPTION_TYPE, (void*)&tFac_OrangeStr},
    {tStr_FalshType, _DESCRIPTION_TYPE, (void*)&tFac_FalshType}
};
MenuItem code tFac_Version_Menu = {tStr_Version, sizeof(tFacVersion_ItemSet)/sizeof(ItemObject), FAC_SUBMENU_VERSION, tFacVersion_ItemSet};

MenuItem* code tFac_Setting_MenuSet[] = {&tFac_WB_Menu, &tFac_ColorTemp_Menu, &tFac_ADC_Calibration_Menu, &tFac_Setting_Option_Menu, &tFac_Reset_Menu, &tFac_Version_Menu};
MenuItemSet code tFac_Setting_MenuItemSet = {tStr_Setting, sizeof(tFac_Setting_MenuSet)/sizeof(MenuItem*), tFac_Setting_MenuSet};


/* Factory mode 'Design' menu */
/* Picture mode */
ItemBracket code tFac_PicMode = {tPictureMode_StrSet, 0, 4, MENUITEM_FAC_PICTURE_MODE};
ItemSlider code tFac_PicMode_Bri = {0, 100, 0, MENUITEM_FAC_PICMODE_BRI};
ItemSlider code tFac_PicMode_Cont = {0, 100, 0, MENUITEM_FAC_PICMODE_CONT};
ItemSlider code tFac_PicMode_Sat = {0, 100, 0, MENUITEM_FAC_PICMODE_SAT};
ItemObject code tFac_PictureMode_ItemSet[] =
{
    {tStr_PictureMode, _BRACKET_TYPE, (void*)&tFac_PicMode},
    {tOsdBrightnessStr, _SLIDER_TYPE, (void*)&tFac_PicMode_Bri},
    {tOsdContrastStr, _SLIDER_TYPE, (void*)&tFac_PicMode_Cont},
    {tOsdSaturationStr, _SLIDER_TYPE, (void*)&tFac_PicMode_Sat}
};
MenuItem code tFac_PicMode_Menu = {tStr_PictureMode, sizeof(tFac_PictureMode_ItemSet)/sizeof(ItemObject), FAC_PIC_MODE_MENU, tFac_PictureMode_ItemSet};

/* Picture Setting */
ItemSlider code tFac_Brightness_0 = {0, 255, 0, MENUITEM_FAC_BRI_0};
ItemSlider code tFac_Brightness_50 = {0, 255, 0, MENUITEM_FAC_BRI_50};
ItemSlider code tFac_Brightness_100 = {0, 255, 0, MENUITEM_FAC_BRI_100};
ItemSlider code tFac_Contrast_0 = {0, 255, 0, MENUITEM_FAC_CONT_0};
ItemSlider code tFac_Contrast_50 = {0, 255, 0, MENUITEM_FAC_CONT_50};
ItemSlider code tFac_Contrast_100 = {0, 255, 0, MENUITEM_FAC_CONT_100};
ItemSlider code tFac_Saturation_0 = {0, 255, 0, MENUITEM_FAC_SAT_0};
ItemSlider code tFac_Saturation_50 = {0, 255, 0, MENUITEM_FAC_SAT_50};
ItemSlider code tFac_Saturation_100 = {0, 255, 0, MENUITEM_FAC_SAT_100};
ItemObject code tFac_PictureSetting_ItemSet[] =
{
    {tStr_source, _LIST_TYPE, (void*)&tSourceSelectList},
    {tStr_Brightness_0, _SLIDER_TYPE, (void*)&tFac_Brightness_0},
    {tStr_Brightness_50, _SLIDER_TYPE, (void*)&tFac_Brightness_50},
    {tStr_Brightness_100, _SLIDER_TYPE, (void*)&tFac_Brightness_100},
    {tStr_Contrast_0, _SLIDER_TYPE, (void*)&tFac_Contrast_0},
    {tStr_Contrast_50, _SLIDER_TYPE, (void*)&tFac_Contrast_50},
    {tStr_Contrast_100, _SLIDER_TYPE, (void*)&tFac_Contrast_100},
    {tStr_Saturation_0, _SLIDER_TYPE, (void*)&tFac_Saturation_0},
    {tStr_Saturation_50, _SLIDER_TYPE, (void*)&tFac_Saturation_50},
    {tStr_Saturation_100, _SLIDER_TYPE, (void*)&tFac_Saturation_100}
};
MenuItem code tFac_PictureSetting_Menu = {tStr_PictureSetting, sizeof(tFac_PictureSetting_ItemSet)/sizeof(ItemObject), tFac_PictureSetting_ItemSet};

/* Music Mode */
ItemBracket code tFac_MusicMode = {tMusicMode_StrSet, 0, 7, MENUITEM_FAC_MUSIC_MODE};
ItemSlider code tFac_MusicMode_300Hz = {_SLIDER_MID_RANGE, 12, 0, MENUITEM_FAC_MUSIC_MODE_300HZ};
ItemSlider code tFac_MusicMode_1KHz  = {_SLIDER_MID_RANGE, 12, 0, MENUITEM_FAC_MUSIC_MODE_1KHZ};
ItemSlider code tFac_MusicMode_2KHz  = {_SLIDER_MID_RANGE, 12, 0, MENUITEM_FAC_MUSIC_MODE_2KHZ};
ItemSlider code tFac_MusicMode_3KHz  = {_SLIDER_MID_RANGE, 12, 0, MENUITEM_FAC_MUSIC_MODE_3KHZ};
ItemSlider code tFac_MusicMode_8KHz  = {_SLIDER_MID_RANGE, 12, 0, MENUITEM_FAC_MUSIC_MODE_8KHZ};
ItemSlider code tFac_MusicMode_15KHz = {_SLIDER_MID_RANGE, 12, 0, MENUITEM_FAC_MUSIC_MODE_15KHZ};
ItemObject code tFac_MusicMode_ItemSet[] =
{
    {tStr_MusicMode, _BRACKET_TYPE, (void*)&tFac_MusicMode},
    {tStr_200Hz, _SLIDER_TYPE, (void*)&tFac_MusicMode_300Hz},
    {tStr_300Hz, _SLIDER_TYPE, (void*)&tFac_MusicMode_1KHz},
    {tStr_1KHz, _SLIDER_TYPE, (void*)&tFac_MusicMode_2KHz},
    {tStr_3KHz, _SLIDER_TYPE, (void*)&tFac_MusicMode_3KHz},
    {tStr_8KHz, _SLIDER_TYPE, (void*)&tFac_MusicMode_8KHz},
    {tStr_15KHz, _SLIDER_TYPE, (void*)&tFac_MusicMode_15KHz}
};
MenuItem code tFac_MusicMode_Menu = {tStr_MusicMode, sizeof(tFac_MusicMode_ItemSet)/sizeof(ItemObject), FAC_MUSIC_MODE_MENU, tFac_MusicMode_ItemSet};

/* Volume Setting */
ItemSlider code tFac_Volume_1 = {0, 100, 0, MENUITEM_FAC_VOLUME_1};
ItemSlider code tFac_Volume_25 = {0, 100, 0, MENUITEM_FAC_VOLUME_25};
ItemSlider code tFac_Volume_50 = {0, 100, 0, MENUITEM_FAC_VOLUME_50};
ItemSlider code tFac_Volume_75 = {0, 100, 0, MENUITEM_FAC_VOLUME_75};
ItemSlider code tFac_Volume_100 = {0, 100, 0, MENUITEM_FAC_VOLUME_100};
ItemSlider code tFac_TV_Volume_1 = {0, 100, 0, MENUITEM_FAC_TV_VOLUME_1};
ItemSlider code tFac_TV_Volume_25 = {0, 100, 0, MENUITEM_FAC_TV_VOLUME_25};
ItemSlider code tFac_TV_Volume_50 = {0, 100, 0, MENUITEM_FAC_TV_VOLUME_50};
ItemSlider code tFac_TV_Volume_75 = {0, 100, 0, MENUITEM_FAC_TV_VOLUME_75};
ItemSlider code tFac_TV_Volume_100 = {0, 100, 0, MENUITEM_FAC_TV_VOLUME_100};
ItemObject code tFac_VolumeSetting_ItemSet[] =
{
    {tStr_Volume_1, _SLIDER_TYPE, (void*)&tFac_Volume_1},
    {tStr_Volume_25, _SLIDER_TYPE, (void*)&tFac_Volume_25},
    {tStr_Volume_50, _SLIDER_TYPE, (void*)&tFac_Volume_50},
    {tStr_Volume_75, _SLIDER_TYPE, (void*)&tFac_Volume_75},
    {tStr_Volume_100, _SLIDER_TYPE, (void*)&tFac_Volume_100},
    {tStr_TV_Volume_1, _SLIDER_TYPE, (void*)&tFac_TV_Volume_1},
    {tStr_TV_Volume_25, _SLIDER_TYPE, (void*)&tFac_TV_Volume_25},
    {tStr_TV_Volume_50, _SLIDER_TYPE, (void*)&tFac_TV_Volume_50},
    {tStr_TV_Volume_75, _SLIDER_TYPE, (void*)&tFac_TV_Volume_75},
    {tStr_TV_Volume_100, _SLIDER_TYPE, (void*)&tFac_TV_Volume_100},
};
MenuItem code tFac_VolumeSetting_Menu = {tStr_VolumeSetting, sizeof(tFac_VolumeSetting_ItemSet)/sizeof(ItemObject), FAC_VOLUME_SETTING_MENU, tFac_VolumeSetting_ItemSet};

/* EMI */
ItemSlider code tFac_DCLKSS_RANGE  = {0,  15, 0, MENUITEM_FAC_DCLK_RANGE};
ItemSlider code tFac_DCLKSS_FMDIV  = {0,   1, 0, MENUITEM_FAC_DCLK_FMDIV};
ItemObject code tFac_EMI_Setting_ItemSet[] =
{
    {tStr_Dclk_RANGE, _SLIDER_TYPE, (void*)&tFac_DCLKSS_RANGE},
    {tStr_Dclk_FMDIV, _SLIDER_TYPE, (void*)&tFac_DCLKSS_FMDIV},
};
MenuItem code tFac_EMI_Setting_Menu = {tStr_EMI, sizeof(tFac_EMI_Setting_ItemSet)/sizeof(ItemObject), FAC_EMI_MENU, tFac_EMI_Setting_ItemSet};

/* Option */
ItemSlider code tFac_RF_StartDb = {0, 31, 0, MENUITEM_FAC_RF_START_DB};
ItemObject code tFac_DesignOption_ItemSet[] =
{
    {tStr_RF_StartDb, _SLIDER_TYPE, (void*)&tFac_RF_StartDb}
};
MenuItem code tFac_Design_Option_Menu = {tStr_Option, sizeof(tFac_DesignOption_ItemSet)/sizeof(ItemObject), FAC_DESIGN_MENU, tFac_DesignOption_ItemSet};

MenuItem* code tFac_Design_MenuSet[] = {&tFac_PicMode_Menu, &tFac_PictureSetting_Menu, &tFac_MusicMode_Menu, &tFac_VolumeSetting_Menu, &tFac_EMI_Setting_Menu, &tFac_Design_Option_Menu};
MenuItemSet code tFac_Design_MenuItemSet = {tStr_Design, sizeof(tFac_Design_MenuSet)/sizeof(MenuItem*), tFac_Design_MenuSet};


/* =========================================================================== */
/**
 * RTK Factory mode Items.
 */

ItemSlider code tPanelBacklight_Item = {0, 10, 0, MENUITEM_PANEL_BACKLIGHT};
ItemSlider code tD_DLti_Item = {0, 10, 0, MENUITEM_D_DLTI};
ItemSlider code tD_DCti_Item = {0, 10, 0, MENUITEM_D_DCTI};
ItemSlider code tI_DLti_Item = {0, 10, 0, MENUITEM_I_DLTI};
ItemSlider code tI_DCti_Item = {0, 10, 0, MENUITEM_I_DCTI};
ItemBracket code tGamma_Item = {tOsdNumberStrSet, 0, 3, MENUITEM_GAMMA};
ItemSlider code tS_Index_Item = {0, 255, 0, MENUITEM_S_Index};
ItemBracket code tS_Low_Item = {tOsdNumberStrSet, 0,10, MENUITEM_S_Low};
ItemBracket code tS_High_Item = {tOsdNumberStrSet, 0, 10, MENUITEM_S_High};
ItemSlider code tSUHCoef_Item = {0, 11, 0, MENUITEM_SUH_COEF};
ItemSlider code tSUVCoef_Item = {0, 10, 0, MENUITEM_SUV_COEF};
ItemSlider code tSUPK_Item = {0, 10, 0, MENUITEM_SUPK};
//ItemSlider code tPeaking_Item = {0, 40, 0, MENUITEM_PEAKING};
//ItemSlider code tSUNR_Item = {0, 16, 0, MENUITEM_NR_SPATIAL};
ItemSlider code tSUNR_Y_Item = {0, 15, 0, MENUITEM_SUNR_Y};
ItemSlider code tSUNR_C_Item = {0, 15, 0, MENUITEM_SUNR_C};
ItemSlider code tDCC_Item = {0, 5, 0, MENUITEM_DCC};

//ItemSlider code tFacSharpness_Item = {0, 100, 0, MENUITEM_FAC_SHARPNESS};
ItemImmediate code tAutoColor_Item = {0, MENUITEM_AUTO_COLOR};
ItemImmediate code tFacReset_Item = {0, MENUITEM_FAC_RESET};
ItemSlider code tYPbPrPhase_Item = {0, 0x3f, 0, MENUITEM_YPBPR_PHASE};
ItemSlider code tSpreadSpectrum_Item = {0, 0x0f, 0, MENUITEM_SPREAD_SPECTRUM};

ItemBracket code tUart_Item = {tOsdUartStrSet, 0, 2, MENUITEM_FAC_UART_PATH};

ItemBracket code tFacDCR_Item = {tOsdNumberStrSet, 0, 3, MENUITEM_FAC_DCR};
ItemBracket code tFacICM_Item = {tOsdNumberStrSet, 0, 7, MENUITEM_FAC_ICM};
ItemBracket code tSUPK_Mask_Item = {tOsdNumberStrSet, 0, 4, MENUITEM_SUPK_MASK};
ItemBracket code tDigitalFilter_Item = {tOsdOnOffStrSet, 0, 2, MENUITEM_DIGITAL_FILTER};
ItemBracket code tDither_Bit_Item = {tOsdNumberStrSet, 0, 3, MENUITEM_DITHER_BIT};
ItemBracket code tVDC_XNR_Item = {tOsdOnOffStrSet, 0, 2, MENUITEM_VDC_XNR};
ItemBracket code tDemo_Item = {tOsdOnOffStrSet, 0, 2, MENUITEM_PQ_DEMO};

ItemObject code tRtkFactoryItemSet[] =
{
    {tOsdBacklightStr, _SLIDER_TYPE, (void*)&tPanelBacklight_Item},
    {tOsdD_DLtiStr, _SLIDER_TYPE, (void*)&tD_DLti_Item},
    {tOsdD_DCtiStr, _SLIDER_TYPE, (void*)&tD_DCti_Item},
    {tOsdI_DLtiStr, _SLIDER_TYPE, (void*)&tI_DLti_Item},
    {tOsdI_DCtiStr, _SLIDER_TYPE, (void*)&tI_DCti_Item},
    {tOsdGammaStr, _BRACKET_TYPE, (void*)&tGamma_Item},
    {tOsdSIndexStr, _SLIDER_TYPE, (void*)&tS_Index_Item},
    {tOsdSHighStr, _BRACKET_TYPE, (void*)&tS_Low_Item},
    {tOsdSLowStr, _BRACKET_TYPE, (void*)&tS_High_Item},
    {tOsdSUHCoefStr, _SLIDER_TYPE, (void*)&tSUHCoef_Item},
    {tOsdSUVCoefStr, _SLIDER_TYPE, (void*)&tSUVCoef_Item},
    {tOsdSUPKStr, _SLIDER_TYPE, (void*)&tSUPK_Item},
    {tOsdSUPK_MaskStr, _BRACKET_TYPE, (void*)&tSUPK_Mask_Item},
    //{tOsdPeakingStr, _SLIDER_TYPE, (void*)&tPeaking_Item},
    {tOsdSUNR_YStr, _SLIDER_TYPE, (void*)&tSUNR_Y_Item},
    {tOsdSUNR_CStr, _SLIDER_TYPE, (void*)&tSUNR_C_Item},
    {tOsdDccStr, _SLIDER_TYPE, (void*)&tDCC_Item},
    {tOsdDCRStr, _BRACKET_TYPE, (void*)&tFacDCR_Item},
    {tOsdIcmStr, _BRACKET_TYPE, (void*)&tFacICM_Item},
    //{tOsdSharpStr, _SLIDER_TYPE, (void*)&tFacSharpness_Item},
    {tOsd_DigitalFilter_Str, _BRACKET_TYPE, (void*)&tDigitalFilter_Item},
    {tOsd_Dither_Bit_Str, _BRACKET_TYPE, (void*)&tDither_Bit_Item},
    {tOsdOsdTimeStr, _SLIDER_TYPE, (void*)&tMiscOsdTimeOutItem},
    {tOsdOsdBlendStr, _SLIDER_TYPE, (void*)&tMiscOsdBlendItem},
    {tOsdAutoColorStr, _IMMEDIATE_TYPE, (void*)&tAutoColor_Item},
    //{tOsdSignalSourceStr, _SELECTDIALOG_TYPE, (void*)&tSignalSourceSet},
    {tOsdFacResetStr, _IMMEDIATE_TYPE, (void*)&tFacReset_Item},
    {tOsdUartStr, _BRACKET_TYPE, (void*)&tUart_Item},
    {tOsdYPbPrPhaseStr, _SLIDER_TYPE, (void*)&tYPbPrPhase_Item},
    {tOsdSpreadSpectrumStr, _SLIDER_TYPE, (void*)&tSpreadSpectrum_Item},
    {tStr_Demo, _BRACKET_TYPE, (void*)&tDemo_Item},
    {tStr_XNR, _BRACKET_TYPE, (void*)&tVDC_XNR_Item},

};

MenuItem code tMenuFactoryMode = {tOsdFactoryModeStr, sizeof(tRtkFactoryItemSet)/sizeof(ItemObject), RTK_FAC_MENU, tRtkFactoryItemSet};





/*=================== Local Functions Phototype ==============*/


/*=========================== Functions ===================== */
/*
*/




/**
 *
 * @}
 */

