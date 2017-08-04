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
 * @ingroup osd
 */

/**
 * @addtogroup osd
 * @{
 */

#define _OSDCONTROL_C
/*===================== Module dependency  ================== */
#include "autoconf.h"
#include "reg52.h"
#include "sparrow.h"
#include "rosprintf.h"
#include "rtd_types.h"
#include "ui_struct.h"
#include "mode.h"
#include "timer_event.h"
#include "timer_event_ui_table.h"
#include "tuner\tuner.h"
#include "ifd\ifd.h"
#include "tv.h"
#include "mode_customer.h"
#include "scaler_idomain\syncproc.h"
#include "scaler_idomain\adc.h"
#include "scaler_idomain\measure.h"
#include "scaler_ddomain\scaler_display.h"
#include "flow_audio.h"
#include "video\video.h"
#include "modestate.h"
#include "message.h"
#include "flow_color.h"
#include "vip\peaking.h"
#include "vip\color.h"
#include "vip\ultrazoom.h"
#include "vip\digital_filter.h"
#include "vip\ypbpr_auto.h"
#include "osdregdef.h"
#include "osdobjectdef.h"
#include "osdfont.h"
#include "osdparameter.h"
#include "osdfunction.h"
#include "osdflow.h"
#include "osdstring.h"
#include "osdcore.h"
#include "osdcontrol.h"
#include "rt_storage.h"
#include "storage_system.h"
#include "cni_table.h"
#include "ui_ctrl.h"
#include "ap_rtirkey.h"
#include "pcb\pcb_utility.h"
#include "pcb_customer.h"
#include "rosprintf.h"
#include "timer_event_ui_table.h"
#include "scaler\scaler_pll.h"
#include "flow_display.h"
#include "flow_scaler.h"
#include "vip\ypbpr_auto.h"
#include "framesync.h"
#include "timer_event_sys_table.h"
#include "power.h"
#ifdef CONFIG_VBI_CC_SUPPORT
#include "osdvbi.h"
#include "closecaption.h"
#endif
#include <hdmi\hdmiFun.h>
#include "i2c\i2c.h"

extern UINT8 xdata ucListSelectedItem;

extern ItemList* xdata pListItem;
UINT16 xdata usSoundMode_MASK =  1                              | 
                    			 (_TV_DK_SUPPORT  << _SOUND_DK) |
                    			 (_TV_I_SUPPORT   << _SOUND_I)  |
                    			 (_TV_BG_SUPPORT  << _SOUND_BG) |
                    			 (_TV_M_SUPPORT   << _SOUND_MN) |
                    			 (_TV_N_SUPPORT   << _SOUND_N)  |
                    			 (_TV_L_SUPPORT   << _SOUND_L);

UINT16 xdata usColorSystemMode_MASK =  1                              	  		| 
                    			  (_TV_PAL_SUPPORT  	<< _SIG_MODE_PAL) 		|
                    			  (_TV_NTSC_SUPPORT 	<< _SIG_MODE_NTSC)		|
                    			  (_TV_NTSC_443_SUPPORT	<< _SIG_MODE_NTSC_443)	|
                    			  (_TV_SECAM_SUPPORT  	<< _SIG_MODE_SECAM);


/*======================= Private Types ===================== */

/*======================== Definitions ====================== */
#define _DISABLE_OSD_CTRL_FUNC_

/*========================== Variables ====================== */
UINT8 xdata ucSleepType;
UINT16 xdata ucCtrlArg0;

static INT8 VPositionTemp;//VPOSITION
static UINT8 ClockTemp;//CLOCK
UINT8 xdata VDC_XNR;

extern ItemSlider code tAudioBalanceItem;
extern volatile UINT8 xdata ucLayerIndex;
extern volatile LayerStack xdata stLayerStack[];

/*=================== Local Functions Phototype ==============*/
void flow_osdctrl_system_Init(void)
{
    ucSleepType = 0;
    VPositionTemp = 0;//VPOSITION
    ClockTemp = 0;//CLOCK
    VDC_XNR = 0;
}

void COsdCtrlDNR(void);
// void timerevent_table_rt_storage_sync_event(void);

/*=========================== Functions ===================== */
/**
 * To change input source by specified source index.
 * @return none.
 */
void COsdFxChangeInputSrc(UINT8 ucSrcNo)
{
    if ( ucSrcNo > _MAX_SOURCE_INDEX )
        return;

#ifdef CONFIG_ENABLE_FHFVBug
    if (GET_INPUTSOURCE_TYPE() == _SOURCE_TV)
    {
        fw_timer_event_CancelTimerEvent(timerevent_CheckTVFHFVEvent);
    }
#endif
//	fw_timer_event_CancelTimerEvent(timerevent_table_rt_storage_sync_event);

    flow_storage_WriteBackup2Flash();
#if 0//CONFIG_USB_CHECK
	if(ucSrcNo==_SOURCE_INDEX_USB&&1)
		ucSrcNo=_SOURCE_INDEX_YPBPR;
#endif
    stUISignalSource = ucSrcNo;

    //Write input source to flash
//	SYNC_STORAGE_DATA(StructSystemDataType, InputSource, ucSrcNo);
    ui_set_input_source(ucSrcNo);
#if defined(CONFIG_WSS_SUPPORT)
    if ((GET_INPUTSOURCE_TYPE() != _SOURCE_TV)
            && (GET_INPUTSOURCE_TYPE() != _SOURCE_CVBS))
    {
        if (GET_OSD_DISPLAY_RATIO() == 2)
            SET_DISPLAY_RATIO(0); // default 4:3
    }
#endif

    fw_flow_message_push(_UI2FL_MAIN_INIT_SRC_EVENT);

}
#if CONFIG_USB_CHECK
extern UINT8 xdata stUsbFlag;
#endif
#if  CONFIG_ENABLE_USB
extern UINT8 g_EnterUsb;
#endif
UINT8 COsdCtrlGetSignalSource()
{
#if CONFIG_USB_CHECK
	 //RTD_Log(LOGGER_INFO, "MELON123=%x\n",(UINT32)stUISignalSource);  
	// RTD_Log(LOGGER_INFO, "MELON456=%x\n",(UINT32)stUsbFlag);	

	if(stUISignalSource== _SOURCE_INDEX_YPBPR&&stUsbFlag==1)//脣奻USB赻雄黍USB
		{
			//RTD_Log(LOGGER_INFO, "[SOURCE] melon2222222222222\n"); 
			stUISignalSource= _SOURCE_INDEX_USB;
			g_EnterUsb=1;
			//flow_storage_WriteBackup2Flash();
			//ui_set_input_source(stUISignalSource);
		}
	else if(stUISignalSource== _SOURCE_INDEX_USB&&(stUsbFlag==2))//з遙善USB
		{
			//RTD_Log(LOGGER_INFO, "[SOURCE] melon3333333333333\n");
			stUISignalSource= _SOURCE_INDEX_YPBPR;
			//flow_storage_WriteBackup2Flash();
			//ui_set_input_source(stUISignalSource);
		}
	stUsbFlag=0;
#endif
#if CONFIG_ENABLE_USB
if(stUISignalSource== _SOURCE_INDEX_USB||stUISignalSource== _SOURCE_INDEX_YPBPR)
	STB_DVDPWR_CTRL(1);
#endif
    return stUISignalSource;
}

/**
 * To change signal source.
 * @param	index of new source
 * @return	none
 */
void COsdCtrlSetSignalSource(UINT8 newSrc)
{
    UINT8 src = COsdCtrlGetSignalSource();
	#if 0
	if((newSrc == _SOURCE_INDEX_HDMI0)&&(info->state != _MODE_STATE_ACTIVE))//joker xie 瓚剿HDMI
	{
			
		  fw_timer_event_ActiveTimerEvent(SEC(0.1), timerevent_table_showloading);
	}
	//else
	//fw_timer_event_ActiveTimerEvent(SEC(1.8),timerevent_table_no_signal_event);	
	#endif
    // if the same source, don't need to change.
    if ( newSrc == src)
        return;
	//RTD_Log(LOGGER_LEVEL,"\n setsignalsource bCheckCCStatus=%d \n",(UINT32)bCheckCCStatus);
    COsdCoreExitOsd();
#if 1//black_backcolor//Tim 0325
    flow_display_BackgroundColor(0, 0, 0x00);
	

	
#else
    flow_display_BackgroundColor(0, 0, 0xff);
#endif
    drv_scalerdisplay_ForceBackground(_TRUE);//reynld added 070214 to improve "FRC switch to Frame sync has an un-stable reset "

    flow_audio_Setmute(_ENABLE, SCALER, (ADC_CH_IDX)(ADC_CH_L|ADC_CH_R|ADC_CH_C|ADC_CH_SW));

    CLR_OSD_SOURCE_INFO();
#ifdef CONFIG_SCART_AUTO_SWITCH
    if ((src == _SOURCE_INDEX_SCART_0)||(src == _SOURCE_INDEX_SCART_1))
        SET_SCART_MANUAL_MODE();		// Disable SCART FS function
#endif
    COsdFxChangeInputSrc(newSrc);



	
    //fw_osdcore_Initialze();
    #if 0
    if (GET_FACTORY_MODE_MENU())
    {
        fw_ui_message_push(_DISPLAY_MESSAGE);
    }
    else
    {
        if ( GET_SOURCE_IDX() == _SOURCE_INDEX_TV)
            COsdCoreShowTVChannel(GET_TV_CUR_CHN(), 1);
        else
            COsdCoreSourceInfo(1);
    }
#endif
}
#ifdef CONFIG_SCART_AUTO_SWITCH
/**
 * To change source depends on SCART FS.
 * @return		none
 */
void ap_osdctrl_scart_change_source(UINT8 newSrc)
{
    UINT8 src = COsdCtrlGetSignalSource();

    // if the same source, don't need to change.
    if ( newSrc == src)
        return;
	//RTD_Log(LOGGER_LEVEL,"\n hange source bCheckCCStatus=%d \n",(UINT32)bCheckCCStatus);

    //RTD_Log(LOGGER_INFO, "[OSD] change source to %d\n", (UINT32)newSrc);
    COsdCoreExitOsd();

    drv_scalerdisplay_ForceBackground(_TRUE);

    flow_audio_Setmute(_ENABLE, SCALER, (ADC_CH_IDX)(ADC_CH_L|ADC_CH_R|ADC_CH_C|ADC_CH_SW));
    fw_timer_event_DelayXms(20);
    AUDIO_AMP_MUTE_ENABLE(AUDIO_AMP_MUTE);

    CLR_OSD_SOURCE_INFO();
    COsdFxChangeInputSrc(newSrc);
}
#endif

/**
 * Color temperature control functions.
 * @return none.
 */
void COsdCtrlColorTemp(void)
{
    UINT8 R_val,G_val,B_val, colorTemp;

    colorTemp = GET_COLOR_TEMP_TYPE();
    RTD_Log(LOGGER_INFO, "VIP: GET_COLOR_TEMP_TYPE()=%d, source type = %x\n",(UINT32)colorTemp,(UINT32)GET_INPUTSOURCE_TYPE());
    if ( colorTemp == _COLOR_TEMP_USER)
    {
    }
    else
    {
        R_val = GET_COLORTEMP_RGAIN(GET_INPUTSOURCE_TYPE(), colorTemp);
        G_val = GET_COLORTEMP_GGAIN(GET_INPUTSOURCE_TYPE(), colorTemp);
        B_val = GET_COLORTEMP_BGAIN(GET_INPUTSOURCE_TYPE(), colorTemp);
        drv_color_setrgbcontrast(R_val, G_val, B_val);
        RTD_Log(LOGGER_INFO, "VIP: ColorTemp gain=%d, %d, %d\n", (UINT32)R_val, (UINT32)G_val, (UINT32)B_val);

        R_val = GET_COLORTEMP_ROFFSET(GET_INPUTSOURCE_TYPE(), colorTemp);
        G_val = GET_COLORTEMP_GOFFSET(GET_INPUTSOURCE_TYPE(), colorTemp);
        B_val = GET_COLORTEMP_BOFFSET(GET_INPUTSOURCE_TYPE(), colorTemp);
        drv_color_setrgbbrightness(R_val, G_val, B_val);
        RTD_Log(LOGGER_INFO, "VIP: ColorTemp offset=%d, %d, %d\n", (UINT32)R_val, (UINT32)G_val, (UINT32)B_val);
    }
    /*
    	switch(colorTemp)
    	{
    	case _COLOR_TEMP_WARMER:
    		Color_SetRgbContrast(_MAIN_DISPLAY, GET_COLORTEMP_RGAIN(WARMER), GET_COLORTEMP_GGAIN(WARMER), GET_COLORTEMP_BGAIN(WARMER), GET_PIP_BLENDING());
    		break;

    	case _COLOR_TEMP_WARM:
    		Color_SetRgbContrast(_MAIN_DISPLAY, GET_COLORTEMP_RGAIN(WARM), GET_COLORTEMP_GGAIN(WARM), GET_COLORTEMP_BGAIN(WARM), GET_PIP_BLENDING());
    		break;

    	case _COLOR_TEMP_NORMAL:
    		Color_SetRgbContrast(_MAIN_DISPLAY, GET_COLORTEMP_RGAIN(NORMAL), GET_COLORTEMP_GGAIN(NORMAL), GET_COLORTEMP_BGAIN(NORMAL), GET_PIP_BLENDING());
    		break;

    	case _COLOR_TEMP_COOL:
    		Color_SetRgbContrast(_MAIN_DISPLAY, GET_COLORTEMP_RGAIN(COOL), GET_COLORTEMP_GGAIN(COOL), GET_COLORTEMP_BGAIN(COOL), GET_PIP_BLENDING());
    		break;

    	case _COLOR_TEMP_COOLER:
    		Color_SetRgbContrast(_MAIN_DISPLAY, GET_COLORTEMP_RGAIN(COOLER), GET_COLORTEMP_GGAIN(COOLER), GET_COLORTEMP_BGAIN(COOLER), GET_PIP_BLENDING());
    		break;

    	case _COLOR_TEMP_USER:
    		R_val = GET_USER_COLORTEMP_USER_R(GET_INPUT_SRC(_MAIN_DISPLAY));
    		G_val = GET_USER_COLORTEMP_USER_G(GET_INPUT_SRC(_MAIN_DISPLAY));
    		B_val = GET_USER_COLORTEMP_USER_B(GET_INPUT_SRC(_MAIN_DISPLAY));
    		R_val *= 2.55;  // mapping to the register
    		G_val *= 2.55;
    		B_val *= 2.55;
    		Color_SetRgbContrast(_MAIN_DISPLAY, R_val, G_val, B_val, GET_PIP_BLENDING());
    		break;

    	default:
    		Color_SetRgbContrast(_MAIN_DISPLAY, _COLOR_TEMP_NORMAL_R_CVBS, _COLOR_TEMP_NORMAL_G_CVBS, _COLOR_TEMP_NORMAL_B_CVBS, GET_PIP_BLENDING());
    		break;
    	}
    */
}

UINT8 COsdCtrlGetColorTemp()
{
    return GET_COLOR_TEMP_TYPE();
}

void COsdCtrlSetColorTemp(void)
{
    UINT8 mode;

    mode = ucCtrlArg0;
    SET_COLOR_TEMP_TYPE(mode);
    COsdCtrlColorTemp();
}

#ifdef _VIDEO_TV_SUPPORT
void COsdCtrlChannelScan(void)
{
    // mute audio
#if 0
    CAudioSetMute(ADC_MUTE,ADC_CH_L|ADC_CH_R);
    fw_timer_event_DelayXms(20);
    AUDIO_AMP_MUTE_ENABLE(AUDIO_AMP_MUTE);
#endif
    COsdCoreInitChannelScanState();

    fw_timer_event_CancelTimerEvent(timerevent_ui_table_OSD_timeout_event);

    if (GET_TV_MANUAL_SCAN() )
    {
        //CTvManualScanStart();
        fw_flow_message_push(_UI2FL_TV_MENUALSCAN_START);
    }
    else
    {
        //CTvAutoScanStart();
        //cvideo_tv_channel_scan_is_running = _TRUE;
        Audio_HwpSetMute(_ENABLE, ADC_CH_ALL, ADC_SYS_MUTE);
        fw_flow_message_push(_UI2FL_TV_AUTOSCAN_START);
    }
}
#endif


void COsdMenuAutoColor()
{
    if ( !(GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR || GET_INPUTSOURCE_TYPE() == _SOURCE_VGA
#ifdef CONFIG_SCART_AUTO_SWITCH
            ||GET_INPUTSOURCE_TYPE() == _SOURCE_FULL_SCART
#endif
          ) )
        return;
//	if ( GET_INPUTSOURCE_TYPE() != _SOURCE_YPBPR
//		&& GET_INPUTSOURCE_TYPE() != _SOURCE_VGA)
//		return;

    COsdCoreInitAutoAdjustState(INFORM_AUTO_COLOR);
    if (GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR) // weihao 960308
    {
#ifdef CONFIG_YPBPR_AUTO_COLOR
        drvif_color_ypbpr_adjustment_dragon_bartype(&stYPbPrGainOffsetData, ClrBR100);
//#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
        ui_set_YPbPr_ADCGain(&stYPbPrGainOffsetData);
//#else
//		SYNC_STORAGE_STRUCT_DATA(ADCGainOffset, stYPbPrGainOffsetData);
//#endif
#else
        drv_adc_autobalance();
#endif
    }
    else if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
    {
        drv_adc_autobalance();
    }
#ifdef CONFIG_SCART_AUTO_SWITCH
    else if ( GET_INPUTSOURCE_TYPE() == _SOURCE_FULL_SCART)
    {
        //drv_adc_autobalance();
        drvif_color_SCARTrgb_adjustment(&stScartGainOffsetData);//march test!!
        ui_set_SCART_ADCGain(&stScartGainOffsetData);
    }
#endif
    fw_ui_message_clear();
    fw_ui_message_push(_EXIT_KEY_MESSAGE);
}

void COsdMenuAutoAdjust(void)
{
    UINT8 auto_config_result;

    if ((info->state != _MODE_STATE_ACTIVE) || (GET_INPUTSOURCE_TYPE() != _SOURCE_VGA))
    {
        //fw_ui_message_push(_EXIT_KEY_MESSAGE);
        return;
    }
    flow_display_BackgroundColor(0, 0, 0);
    COsdCoreInitAutoAdjustState(INFORM_ADJUSTING);
    auto_config_result = drv_adc_auto_config();

    if (auto_config_result == ERROR_SUCCEED)
    {
        SET_AUTO_ADJUST_DONE();
        ui_set_VGA_info(info->ucMode_Curr);
        /*CStorageSystemUpdateStructData(_flash_storage_vga_info);
        CStorageSystemUpdateToFlash();
        COsdCoreSetState(_MAINMENU_STATE);
        COsdCoreBackToMainMenu(_TRUE);*/
        //fw_timer_event_CancelTimerEvent(timerevent_ui_table_OSD_timeout_event);
        //fw_timer_event_ActiveTimerEvent(0.01, timerevent_ui_table_OSD_timeout_event);
    }
    else if (auto_config_result==ERROR_ABORT)
    {
        /*COsdCoreSetState(_MAINMENU_STATE);
        COsdCoreBackToMainMenu(_TRUE);*/
        //fw_timer_event_CancelTimerEvent(timerevent_ui_table_OSD_timeout_event);
        //fw_timer_event_ActiveTimerEvent(0.01, timerevent_ui_table_OSD_timeout_event);

    }
    fw_ui_message_clear();
    fw_ui_message_push(_EXIT_KEY_MESSAGE);
#if 0 //black_backcolor
    flow_display_BackgroundColor(0, 0, 0x00);
#else
    flow_display_BackgroundColor(0, 0, 0xff);
#endif
    //fw_timer_event_CancelTimerEvent(timerevent_ui_table_OSD_timeout_event);
    //fw_timer_event_ActiveTimerEvent(1, timerevent_ui_table_OSD_timeout_event);
#if 0
    COsdFxDisableOsd();
    //COsdCoreSetState(_USER_DEFINED_STATE);
    fw_timer_event_CancelTimerEvent(timerevent_ui_table_OSD_timeout_event);
    fw_timer_event_ActiveTimerEvent(0.01, timerevent_ui_table_OSD_timeout_event);
#endif

}

/**
 * OSD timeout control functions.
 */

UINT16 ap_osdctrl_get_osd_timeout_ms(void)
{
    UINT16 uwTimeout;

    uwTimeout = GET_OSD_TIMEOUT();
    if (uwTimeout < 5)
    {
        //	RTD_Log(LOGGER_INFO, "Invalid OSD timeout (%d)\n", (UINT32)uwTimeout);
        uwTimeout = 5;
    }
    uwTimeout = uwTimeout * 1000;

    return uwTimeout;
}

UINT8 COsdCtrlGetOsdTimeOutSystem()
{
    return GET_OSD_TIMEOUT();
}

void COsdCtrlSetOsdTimeOutSystem(void)
{
    UINT8 ucVal;

    ucVal = GET_OSD_TIMEOUT();
    if (ucCtrlArg0 == _RIGHT_KEY_MESSAGE||ucCtrlArg0 == _RIGHT_IR_MESSAGE)
    {
        if (ucVal < 60)
            ucVal = ucVal + 5;
        if (ucVal > 60)
            ucVal = 60;
    }
    else if (ucCtrlArg0 == _LEFT_KEY_MESSAGE||ucCtrlArg0 == _LEFT_IR_MESSAGE)
    {
        if (ucVal > 5)
            ucVal = ucVal - 5;
        if ( ucVal < 5)
            ucVal = 5;
    }

    SET_OSD_TIMEOUT(ucVal);
}

/**
 * OSD blending control functions.
 */
UINT8 COsdCtrlGetOsdBlend(void)
{
    return GET_OSD_BLENDING();
}

void COsdCtrlSetOsdBlend(UINT8 ucLevel)
{
    SET_OSD_BLENDING(ucLevel);
    COsdFxBlending(ucLevel);
}

#ifdef _VIDEO_TV_SUPPORT
/**
 * OSD country control functions.
 *
 */
void COsdCtrlSetCountrySelect(void)
{
    UINT8 type;
    type = ucCtrlArg0;

    SET_TV_COUNTRY(type);
}

UINT8 COsdCtrlGetCountrySelect(void)
{
    return GET_TV_COUNTRY();
}

void  COsdCtrlSetChanelAFC(void)
{
    UINT8  val;
	UINT8 CurChn;
	
    CurChn = GET_TV_CHN_DATA(_CUR_CHN_TV);

    val = ucCtrlArg0;
    if (val)
        SET_AFC_SKIP(CurChn);
    else
        CLR_AFC_SKIP(CurChn);
}
UINT8 COsdCtrlGetChanelAFC(void)
{
	//RTD_Log(LOGGER_LEVEL,"\n tim afc = %d \n",GET_AFC_SKIP(GET_TV_CUR_CHN()));
	UINT8 CurChn;
	CurChn = GET_TV_CHN_DATA(_CUR_CHN_TV);
	//RTD_Log(LOGGER_LEVEL,"\n Tim chn skip =%d \n",(UINT32)GET_AFC_SKIP(CurChn));
    if (GET_AFC_SKIP(CurChn)==_CHN_STS_AFC_SKIP)
    {
        return 1;
    }
    else
        return 0;

}
void  COsdCtrlSetChanelSkip(void)
{
    UINT8  val;
	UINT8 CurChn;

	CurChn = GET_TV_CHN_DATA(_CUR_CHN_TV);
	
    val = ucCtrlArg0;
    if (val)
        SET_CHANNEL_SKIP(CurChn);
    else
        CLR_CHANNEL_SKIP(CurChn);
}
UINT8 COsdCtrlGetChanelSkip(void)
{
	UINT8 CurChn;

	CurChn = GET_TV_CHN_DATA(_CUR_CHN_TV);

    if (GET_CHANNEL_SKIP(CurChn)==_CHN_STS_SKIP)
        return 1;
    else
        return 0;
}
#endif
#if 0
/**
 * Display ration control functions.
 */
UINT8 COsdCtrlGetDisplayRatioEnable(UINT8 idx)
{
    UINT8 getVal;
    LOAD_STORAGE_DATA(StructSystemDataType, InputSource, getVal);
    switch (getVal)
    {
    case _SOURCE_INDEX_VGA:
        if ( idx == RATIO_14_9 || idx == RATIO_LETTERBOX || idx == RATIO_PANORAMA)
            return _FALSE;
        break;

    case _SOURCE_INDEX_HDMI0:
    case _SOURCE_INDEX_HDMI1:
        if (idx == RATIO_PANORAMA)
            return _FALSE;
        break;

    case _SOURCE_INDEX_YPBPR:
        if ( idx == RATIO_PANORAMA)
            return _FALSE;
        break;

        //leo 20071113 disable 1:1 at video8 source
    case _SOURCE_INDEX_TV:
    case _SOURCE_INDEX_AV:
    case _SOURCE_INDEX_SV:
        if ( idx == RATIO_POINTTOPOINT)
            return _FALSE;
    default:
        break;
    }

    return _TRUE;
}


/**
 * PQ sharpness control functions.
 */
UINT8 COsdCtrlGetSharpness()
{
    return GET_SHARPNESS();
}

void COsdCtrlSetSharpness(UINT8 message)
{
    SET_SHARPNESS(message);

    // call PQ sharpness API here
    drv_color_PeakingFilter(message);
    //Sharpness_Adjust(pInfoOsdSrc->display, message);
}
#endif

#if 0
/**
 * ICM control functions.
 */
void COsdCtrlSetICMSHI(void)
{
    INT16 Hue, Sat, Intensity;
    UINT8 OSDSat,OSDHue, StartPoint,EndPoint;
    INT16  Temp_value;

    OSDSat = GET_SATURATION();
    if ( OSDSat < 50)
    {
        StartPoint = COsdCtrlGetFacSaturation0();
        EndPoint = COsdCtrlGetFacSaturation50();
    }
    else
    {
        StartPoint = COsdCtrlGetFacSaturation50();
        EndPoint = COsdCtrlGetFacSaturation100();
        OSDSat -= 50;
    }
    OSDSat = (UINT8)(StartPoint + ABS(EndPoint,StartPoint)*OSDSat/50);
    Sat =  (OSDSat * 40.95) - 2048; // map 0~100 to -2048~2047

    OSDHue = GET_HUE();
    Temp_value = (INT16)(OSDHue) - 50;
    //Hue = (Temp_value * 2.5) ; // map -50~50 to -128~128
    Hue = Temp_value * 5 / 2; // map -50~50 to -128~128

    Intensity = GET_INTENSITY();
    Intensity = (Intensity * 40.95) - 2048;//map 0~100 to -2048~2047

    //Color_SetICMHSI(display, Hue, Sat, Intensity);
}
#endif


UINT8 COsdCtrlGetHue()
{
    return GET_HUE();
}

void COsdCtrlSetHue(void)
{
    SET_HUE(ucCtrlArg0);
    if ( COsdCoreGetState() == _SLIDER_STATE && GET_PICTURE_MODE() != _PIC_MODE_USER) // Bug PR00038: cf.tsai resolved 071003
    {
        SET_PICTURE_MODE(_PIC_MODE_USER);
    }
    if ( GET_PICTURE_MODE() == _PIC_MODE_USER)
    {
        SET_PICTURE_MODE_HUE(_PIC_MODE_USER, ucCtrlArg0);
    }

    // call PQ Hue API here
//	if(GET_INPUTSOURCE_FROM() == _SRC_FROM_VDC)
//	   	drv_video_vdc_SetHue(ucCtrlArg0);
//	else
    drv_color_AdjustHue(ucCtrlArg0);

}

UINT8 COsd_remapping( UINT8 osd_value, UINT8 mid_point)
{
//range of osd value: 0-100
//mid_point: original 50 map to the level u want

    UINT32 new_value=0;

    new_value =osd_value;

//	RTD_Log(LOGGER_INFO, "==>>COsd_remapping input: osd_value=%d, mid_point=%d\n",(UINT32)osd_value, (UINT32)mid_point);

    if ( (mid_point>100 || mid_point<0) || (osd_value>100 || osd_value<0) )
    {
        //	RTD_Log(LOGGER_INFO, "==>>OSD-Driver mapping error\n");

        return(osd_value);
    }
    else
    {
        if (osd_value>=60)
        {
            new_value = (mid_point*50 + ( (osd_value-50)*(100-mid_point) ) );
            new_value = new_value/50;
        }
        else
        {
            new_value = (mid_point*50 - ( (50-osd_value)*mid_point ));
            new_value = new_value/50;
        }

//		RTD_Log(LOGGER_INFO, "==>>COsd_remapping: new_value=%d\n", (UINT32)new_value);

        return(new_value);
    }
}


UINT8 COsdCtrlGetSaturation()
{
    return GET_SATURATION();
}

void COsdCtrlSetSaturation(UINT8 message)
{
    UINT8 new_sat=0;

    SET_SATURATION(message);
    if ( COsdCoreGetState() == _SLIDER_STATE && GET_PICTURE_MODE() != _PIC_MODE_USER) // Bug PR00038: cf.tsai resolved 071003
    {
        SET_PICTURE_MODE(_PIC_MODE_USER);
    }
    if ( GET_PICTURE_MODE() == _PIC_MODE_USER)
    {
        SET_PICTURE_MODE_SAT(_PIC_MODE_USER, message);
    }

    // call PQ Saturation API here
//	if(GET_INPUTSOURCE_FROM() == _SRC_FROM_VDC)
//	   	drv_video_vdc_SetSaturation(message);
//	else
    //drv_color_AdjustSaturation(message);
    new_sat = COsd_remapping( message, 60);
    drv_color_AdjustSaturation(new_sat);


}

#if 0
UINT8 COsdCtrlGetIntensity()
{
    return GET_INTENSITY();
}

void COsdCtrlSetIntensity(UINT8 message)
{
    SET_INTENSITY(message);
    //COsdCtrlSetICMSHI();
}
#endif
#if 1//New
#define BRI_MAX         144//120//80//105//128
#define BRI_MIN         60

#define CON_MAX			186//232//(194)//(255)//128//Tim 0322
#define CON_MIN			60//Tim 0322

#define BRI_MAX_HDMI    112//  136//105
#define BRI_MIN_HDMI	60

#define CON_MAX_HDMI	154//194//255
#define CON_MIN_HDMI    60//30

void COsdCtrlSetYCB(void)
{
    UINT8 Contrast, Brightness,ageMode;
    UINT8 BriConIdx;
    UINT8 ConMin,ConMax,BriMin,BriMax;
//	INT16 StartPoint, EndPoint;
//	RTD_Log(LOGGER_INFO, "VIP: GET_CONTRAST()=%d, GET_BRIGHTNESS()=%d\n",(UINT32)GET_CONTRAST(),(UINT32)GET_BRIGHTNESS());
    ageMode = GET_AGING_MODE();
    if (ageMode == _AGING_MODE_ON)
        BriConIdx = 100;
    else
    {
        BriConIdx = GET_CONTRAST();
    }
#ifdef CONFIG_ENABLE_HDMI
	if ( COsdCtrlGetSignalSource() == _SOURCE_INDEX_HDMI0)
	{
        ConMin = CON_MIN_HDMI;
        ConMax = CON_MAX_HDMI;
        BriMin = BRI_MIN_HDMI;
        BriMax = BRI_MAX_HDMI;

	}
	else
#endif
	{
        ConMin = CON_MIN;
        ConMax = CON_MAX;
        BriMin = BRI_MIN;
        BriMax = BRI_MAX;
	}

	//RTD_Log(LOGGER_LEVEL,"\n Tim Con max = %d,min = %d,bri max = %d,bri min = %d \n",(UINT32)ConMax,(UINT32)ConMin,(UINT32)BriMax,(UINT32)BriMin);
#if 0
    if (BriConIdx < 50)
    {
//?		StartPoint = GET_FAC_CONT_0();
//?		EndPoint = GET_FAC_CONT_50();
    }
    else
    {
//?		StartPoint = GET_FAC_CONT_50();
//?		EndPoint = GET_FAC_CONT_100();
        BriConIdx -= 50;
    }
    Contrast = (UINT8)(StartPoint + (EndPoint-StartPoint)*BriConIdx/50);
#else
    // mapping from 100 to 255
    //Contrast = (UINT16)BriConIdx * 255 / 100;    
	Contrast = ConMin+(UINT16)BriConIdx * ConMax / 100;//Tim 0322
 if(Contrast>255)
  Contrast=255;
	
#endif

    if (ageMode == _AGING_MODE_ON)
        BriConIdx = 100;
    else
    {
        BriConIdx = GET_BRIGHTNESS();
    }

#if 0
    if (BriConIdx < 50)
    {
//?		StartPoint = GET_FAC_BRI_0();
//?		EndPoint = GET_FAC_BRI_50();
    }
    else
    {
//?		StartPoint = GET_FAC_BRI_50();
//?		EndPoint = GET_FAC_BRI_100();
        BriConIdx -= 50;
    }
    Brightness = (UINT8)(StartPoint + (EndPoint-StartPoint)*BriConIdx/50);
#else
    // mapping from 100 to 255
   //Brightness = (UINT16)BriConIdx * 255 / 100;

  Brightness = BriMin+(UINT16)BriConIdx * BriMax / 100;
   //RTD_Log(LOGGER_INFO,"BRIGHTNESSS---------------------------%d\n",(UINT32)Brightness);
   
  // RTD_Log(LOGGER_INFO,"Contrast---------------------------%d\n",(UINT32)Contrast);
#endif

    // call PQ contrast API here
    drv_color_setycontrastbrightness(Contrast, Brightness);
}
#else
void COsdCtrlSetYCB(void)
{
#define BRI_MAX          128
#define BRI_MIN         60
    UINT8 Contrast, Brightness,ageMode;
    UINT8 BriConIdx;
//	INT16 StartPoint, EndPoint;
//	RTD_Log(LOGGER_INFO, "VIP: GET_CONTRAST()=%d, GET_BRIGHTNESS()=%d\n",(UINT32)GET_CONTRAST(),(UINT32)GET_BRIGHTNESS());
    ageMode = GET_AGING_MODE();
    if (ageMode == _AGING_MODE_ON)
        BriConIdx = 100;
    else
    {
        BriConIdx = GET_CONTRAST();
    }

#if 0
    if (BriConIdx < 50)
    {
//?		StartPoint = GET_FAC_CONT_0();
//?		EndPoint = GET_FAC_CONT_50();
    }
    else
    {
//?		StartPoint = GET_FAC_CONT_50();
//?		EndPoint = GET_FAC_CONT_100();
        BriConIdx -= 50;
    }
    Contrast = (UINT8)(StartPoint + (EndPoint-StartPoint)*BriConIdx/50);
#else
    // mapping from 100 to 255
    Contrast = (UINT16)BriConIdx * 255 / 100;
#endif

    if (ageMode == _AGING_MODE_ON)
        BriConIdx = 100;
    else
    {
        BriConIdx = GET_BRIGHTNESS();
    }

#if 0
    if (BriConIdx < 50)
    {
//?		StartPoint = GET_FAC_BRI_0();
//?		EndPoint = GET_FAC_BRI_50();
    }
    else
    {
//?		StartPoint = GET_FAC_BRI_50();
//?		EndPoint = GET_FAC_BRI_100();
        BriConIdx -= 50;
    }
    Brightness = (UINT8)(StartPoint + (EndPoint-StartPoint)*BriConIdx/50);
#else
    // mapping from 100 to 255
   //Brightness = (UINT16)BriConIdx * 255 / 100;

  Brightness = BRI_MIN+(UINT16)BriConIdx * BRI_MAX / 100;
   RTD_Log(LOGGER_INFO,"BRIGHTNESSS---------------------------%d\n",(UINT32)Brightness);
#endif

    // call PQ contrast API here
    drv_color_setycontrastbrightness(Contrast, Brightness);
}

#endif


UINT8 COsdCtrlGetMainYContrast()
{
    return GET_CONTRAST();
}

void COsdCtrlSetMainYContrast(UINT8 message)
{
    SET_CONTRAST(message);
    if ( COsdCoreGetState() == _SLIDER_STATE && GET_PICTURE_MODE() != _PIC_MODE_USER) // Bug PR00038: cf.tsai resolved 071003
    {
        SET_PICTURE_MODE(_PIC_MODE_USER);
//		SET_USER_BRIGHTNESS(pInfoOsdSrc->input_src, GET_BRIGHTNESS(pInfoOsdSrc->input_src));
//		SET_USER_SATURATION(pInfoOsdSrc->input_src, GET_SATURATION(pInfoOsdSrc->input_src));
    }

    if (GET_PICTURE_MODE() == _PIC_MODE_USER)
        SET_PICTURE_MODE_CONT(_PIC_MODE_USER, message);

    COsdCtrlSetYCB();
}

UINT8 COsdCtrlGetMainYBrightness()
{
    return GET_BRIGHTNESS();
}

void COsdCtrlSetMainYBrightness(UINT8 message)
{
    SET_BRIGHTNESS(message);
    if ( COsdCoreGetState() == _SLIDER_STATE && GET_PICTURE_MODE() != _PIC_MODE_USER) // Bug PR00038: cf.tsai resolved 071003
    {
        SET_PICTURE_MODE(_PIC_MODE_USER);
//		SET_USER_CONTRAST(pInfoOsdSrc->input_src, GET_CONTRAST(pInfoOsdSrc->input_src));
//		SET_USER_SATURATION(pInfoOsdSrc->input_src, GET_SATURATION(pInfoOsdSrc->input_src));
    }

    if (GET_PICTURE_MODE() == _PIC_MODE_USER)
        SET_PICTURE_MODE_BRI(_PIC_MODE_USER, message);

	SYNC_STORAGE_DATA(StructSystemDataType,  DimmerVaule, message);
    COsdCtrlSetYCB();
}

#if 0
UINT8 COsdCtrlGetRgbContrast()
{
    return GET_CONTRAST();
}
#endif

void COsdCtrlSetRgbContrast(UINT8 message)
{
    UINT8 value;

    value = message * 2.55;//map 0~100 to 0~255
    //Color_SetRgbContrast(pInfoOsdSrc->display, value, value, value, GET_PIP_BLENDING());
    drv_color_setrgbcontrast(value, value, value);
}

void COsdCtrlSetReset(void)
{	
	//Tim 0404
	UINT16 ucTempRatio;
	UINT16 ucTempIr;
	UINT16 ucTempfm;
	//UINT8 xdata ucVolTemp;
	//UINT8 cursource;
    COsdCoreExitOsd();
    //flow_audio_Setmute(_ENABLE, SCALER, (ADC_CH_IDX)(ADC_CH_L|ADC_CH_R|ADC_CH_C|ADC_CH_SW));//hardy20091208 for avoid reboot when osd reset

    /*if (GET_INPUTSOURCE_TYPE() == _SOURCE_TV)
    {
        SET_TV_CUR_CHN(1);
        SET_TV_PRE_CHN(1);
    }
    */
	LOAD_STORAGE_DATA(StructSystemDataType, InputSource, stUISignalSource); 



	//ucVolTemp=COsdCtrlGetAudioVolume();//Vol not reset;
    SYNC_STORAGE_DEFAULT_DATA(StructOsdUserDataType);
    SYNC_STORAGE_DEFAULT_DATA(StructAudioDataType);
    SYNC_STORAGE_DEFAULT_DATA(StructColorDataType);
    
    SYNC_STORAGE_DEFAULT_DATA(StructSystemDataType);//,  ZoomMode, ucCtrlArg0);//Tim 0404
//	SYNC_STORAGE_DEFAULT_DATA(StructSystemDataType);
 //   SYNC_STORAGE_DEFAULT_DATA(StructTvDataType);
    //Tim 0404
    //==========================================================================
    LOAD_STORAGE_DATA(StructSystemDataType, ZoomMode, ucTempRatio);
    SET_OSD_DISPLAY_RATIO(ucTempRatio);
    if (ucTempRatio == 2) // auto default set to 4:3
        SET_DISPLAY_RATIO(1);
    else
        SET_DISPLAY_RATIO(ucTempRatio);
	LOAD_STORAGE_DATA(StructSystemDataType,  IrMode, ucTempIr);
	SET_OSD_DISPLAY_IR(ucTempIr);
	if (ucTempIr == 0) 
        {
			STB_IR_EN(1);
           
        }
        else
    	{
    		STB_IR_EN(0);
			
    	  	if(ucTempIr==1)
    	   		{
		   		STB_IR_CH(0);
    	   		}
		   	else
	   		{
				STB_IR_CH(1);	
	   		}
	   		
		  	
    	}
		LOAD_STORAGE_DATA(StructSystemDataType,  FMMode, ucTempfm);
		SET_OSD_DISPLAY_Radio(ucTempfm);
		AdjustFMfun(ucTempfm);

    //RTD_Log(LOGGER_LEVEL,"\n Tim1 ZoomMode = %d  \n",(UINT32)GET_OSD_DISPLAY_RATIO());
    SYNC_STORAGE_DEFAULT_DATA(StructSystemDataType);//,  ZoomMode, ucCtrlArg0);
    //RTD_Log(LOGGER_LEVEL,"\n Tim2 ZoomMode = %d  \n",(UINT32)GET_OSD_DISPLAY_RATIO());
    SYNC_STORAGE_DATA(StructSystemDataType, InputSource, stUISignalSource); 
	//==========================================================================
    if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
    {
        //#define GET_AUTO_ADJUST_DONE() 			(stVGAModeCurrInfo.Status & _BIT0)
        stVGAModeCurrInfo.Status = stVGAModeCurrInfo.Status&0xFE;
        stVGAModeCurrInfo.Clock = 0x80;
        stVGAModeCurrInfo.H_Position = 0x80;
        stVGAModeCurrInfo.V_Position = 0x80;
        stVGAModeCurrInfo.Phase = 0x00;
        ui_set_VGA_info(info->ucMode_Curr);
    }

    SYNC_STORAGE_DEFAULT_DATA(StructVGAScreenModeInfo);

    ui_set_AcOff_powerStatus(POWER_STATUS_POWER_ON);//Hardy20091203 for booting to AC-off power status before

	//SET_VOLUME(ucVolTemp);//Vol not reset;
    
    fw_osdcore_Initialze(); // osd reset
    ap_osdctrl_audio_setting_init(); // audio reset
    // VIP for osd related settings
    COsdCtrlSetStorageValue();
    //osd reset don't change source by brighty modify
    //if(GET_INPUTSOURCE_TYPE() == _SOURCE_TV)
    //{
    //fw_flow_message_push(_UI2FL_MAIN_INIT_SRC_EVENT);
    //}
    //else
    //COsdCtrlSetSignalSource(_SOURCE_INDEX_TV);
    //RTD_Log(LOGGER_LEVEL,"\n Tim3 ZoomMode = %d  \n",(UINT32)GET_OSD_DISPLAY_RATIO());
    // DisplayRatioFlag = _ON; 
    if(!IS_RATIO_16_9()) 
	 fw_flow_message_push(_UI2FL_MAIN_DISPLAYRATIO_CHANGE);  

}

/**
 * OSD Language control functions.
 */
UINT8 COsdCtrlGetLanguage(void)
{
    return GET_OSD_LANGUAGE();
}
extern volatile UINT16 xdata fUI_Msg;

void COsdCtrlSetLanguage(void)
{
    UINT8 ucLanguage;

    ucLanguage = ucCtrlArg0;
	if(fUI_Msg == _RIGHT_KEY_MESSAGE||fUI_Msg == _RIGHT_IR_MESSAGE)
	{
		if(ucCtrlArg0==1)
		ucLanguage=4;
		if(ucCtrlArg0==6)
		ucLanguage=2;
		if(ucCtrlArg0==4)
		ucLanguage=1;
		if(ucCtrlArg0==2)
		ucLanguage=6;
	}
	else
	{
		if(ucCtrlArg0==4)
		ucLanguage=1;
		if(ucCtrlArg0==0)
		ucLanguage=2;
		if(ucCtrlArg0==1)
		ucLanguage=4;
		if(ucCtrlArg0==2)
		ucLanguage=0;
	}
    SET_OSD_LANGUAGE(ucLanguage);
    UpdateLanguagePage();
}

/**
 * TV blue screen control functions.
 */
/*
UINT8 COsdCtrlGetTVBlueScreen(void)
{
   return GET_TV_BLUE_SCREEN();
}

void COsdCtrlSetTVBlueScreen(void)
{
   if ( ucCtrlArg0 == _OFF ) {
   	CLR_TV_BLUE_SCREEN();
   	if ( GET_INPUTSOURCE_TYPE() == _SOURCE_TV){
   		drv_scalerdisplay_ForceBackground(FALSE);
   		if ( !drv_video_vdc_CheckHVisLocked() ) {	// no signal ?
   		}
   	}
   }
   else {
   	SET_TV_BLUE_SCREEN();
   	if ( GET_INPUTSOURCE_TYPE() == _SOURCE_TV) {
   		if ( !drv_video_vdc_CheckHVisLocked() ) {
   			drv_scalerdisplay_ForceBackground(TRUE);
   			Audio_HwpSetMute(_ENABLE, ADC_CH_ALL, ADC_SYS_MUTE);
   			fw_timer_event_DelayXms(20);
   			AUDIO_AMP_MUTE_ENABLE(AUDIO_AMP_MUTE);	// CF 20071023 added, mute AMP
   		}
   	}
   }
#if 0
   if ( GET_TV_BLUE_SCREEN() )
   {
   	if ( !CVideoIsLocked() )
   	{
   		CAudioSetMute(ADC_MUTE, ADC_CH_L|ADC_CH_R);
   		fw_timer_event_DelayXms(20);	//sms 2008-01-09
   		AUDIO_AMP_MUTE_ENABLE(AUDIO_AMP_MUTE);
   	}
   	TvBlueScreenFlag=1; // reynold, 080712, 子面下部瞬拉伸，持大概1s
   }
   else
   	TvBlueScreenFlag=0; // reynold, 080712, 子面下部瞬拉伸，持大概1s
#endif
}
*/

void COsdCoreSetBass(INT8 message)
{
    APP_BASS_VAL bass_value;
    bass_value.max_val = 6;
    bass_value.min_val = -6;
    bass_value.val = message;
    Audio_HwpSetBassValue(_TRUE,&bass_value);
}

void COsdCoreSetTreble(INT8 message)
{
    APP_TREBLE_VAL treble_value;
    treble_value.max_val = 6;
    treble_value.min_val = -6;
    treble_value.val = message;
    Audio_HwpSetTrebleValue(_TRUE,&treble_value);
}

void COsdCoreSetEQ(UINT8 EQ_mode)
{
#define MAX_EQVALUE_SET 6
    APP_EQ_VAL eq_value;

    eq_value.max_val = 6;
    eq_value.min_val = -6;

    eq_value.val[0] = ui_get_eq_300(EQ_mode)-MAX_EQVALUE_SET;
    eq_value.val[1] = ui_get_eq_1k(EQ_mode) -MAX_EQVALUE_SET;
    eq_value.val[2] = ui_get_eq_2k(EQ_mode) -MAX_EQVALUE_SET;
    eq_value.val[3] = ui_get_eq_3k(EQ_mode) - MAX_EQVALUE_SET;
    eq_value.val[4] = ui_get_eq_8k(EQ_mode) - MAX_EQVALUE_SET;
    eq_value.val[5] = ui_get_eq_15k(EQ_mode)-MAX_EQVALUE_SET;

    Audio_HwpSetEQValue(_ENABLE,&eq_value);
#undef  MAX_EQVALUE_SET
}


/**
 *
 *
 */
void ap_osdctrl_audio_mute(UINT8 ucMode)
{
    bit bUnmuteFlag = _FALSE;

    if ( ucMode == AUDIO_MUTE )
    {
        SET_AUDIO_MUTE();
        Audio_HwpSetMute(_ENABLE, ADC_CH_ALL, ADC_SYS_MUTE);
        fw_timer_event_DelayXms(20);
        AUDIO_AMP_MUTE_ENABLE(AUDIO_AMP_MUTE);
    }
    else if ( ucMode == AUDIO_UNMUTE)
    {
        CLR_AUDIO_MUTE();
        if ( GET_INPUTSOURCE_TYPE() == _SOURCE_TV && drv_video_vdc_CheckHVisLocked() )
        {
            bUnmuteFlag = _TRUE;
        }
        else if (( info->state == _MODE_STATE_ACTIVE )&&(GET_INPUTSOURCE_TYPE() != _SOURCE_TV))
        {
            bUnmuteFlag = _TRUE;
        }

        if ( bUnmuteFlag)
        {
            AUDIO_AMP_MUTE_ENABLE(AUDIO_AMP_UNMUTE);
            Audio_HwpSetMute(_DISABLE, ADC_CH_ALL, ADC_SYS_MUTE);
        }
    }
}


/**
 *
 *
 */
void COsdCtrlSetAudioMute(void)
{
    if (!GET_AUDIO_MUTE())
    {
        Audio_HwpSetMute(_ENABLE, ADC_CH_ALL, ADC_SYS_MUTE);
        fw_timer_event_DelayXms(20);
        AUDIO_AMP_MUTE_ENABLE(AUDIO_AMP_MUTE);	// CF 20071023 added, mute AMP
        SET_AUDIO_MUTE();
    }
    else
    {
    	if(!bearphone)
        	AUDIO_AMP_MUTE_ENABLE(AUDIO_AMP_UNMUTE);	// CF 20071023 added, unmute AMP
        fw_timer_event_DelayXms(20);
        Audio_HwpSetMute(_DISABLE, ADC_CH_ALL, ADC_SYS_MUTE);
        CLR_AUDIO_MUTE();
    }
}

#if 0
/**
 * Audio mute control functions.
 */
UINT8 COsdCtrlGetAudioMute()
{
    return GET_AUDIO_MUTE();
}

void COsdCtrlAudioSurround(UINT8 mode)
{
    switch (mode)
    {
    case 1:
        CAudioEnableWOW(AUD_ENABLE);
        break;
    case 2:
        CAudioEnableWOW(AUD_DISABLE);
        CAudioEnableBBE(AUD_ENABLE,3,(void*) NULL);	// (1-3)BBE_I/II/ViVA
        break;
    case 3:
        CAudioEnableBBE(AUD_DISABLE,2,(void*) NULL);	// (1-3)BBE_I/II/ViVA
        CAudioEnableBBE(AUD_ENABLE,3,(void*) NULL);	// (1-3)BBE_I/II/ViVA
        break;
    default:
        CAudioEnableWOW(AUD_DISABLE);
        break;
    }
}

UINT8 COsdCtrlGetAudioSurround()
{
    return (UINT8)(GET_AUDIO_SURROUND()%3);
}

void COsdCtrlSetAudioSurround(UINT8 message)
{
    UINT8 mode=GET_AUDIO_SURROUND();

    mode = message % 3;
    SET_AUDIO_SURROUND(mode);
    CAudioSetMute(ADC_MUTE,ADC_CH_L|ADC_CH_R);
    fw_timer_event_DelayXms(60);            	//delay 60ms
    COsdCtrlAudioSurround(mode);

    CAudioClearFIFO();
    if ( GET_INPUTSOURCE_INDEX() == _SOURCE_INDEX_TV && !(/*CTunerIsLocked(&afc) && */CVideoIsLocked()))
        ;
    else if ( (rtdf_readByte0(DDOMAIN_MAIN_DISPLAY_CONTROL_RSV_VADDR) & _BIT1) != 0)	// force to background ?
        ;
    else
    {
        if (!GET_AUDIO_MUTE())
            CAudioSetMute(ADC_UNMUTE,ADC_CH_L|ADC_CH_R);
    }
    COsdCoreSetEQValue();

    if (0 == mode)
    {
        if (0 == GET_BALANCE())
        {
            CAudioSetMute(ADC_MUTE, ADC_CH_R);
        }
        else if (20 == GET_BALANCE())
        {
            CAudioSetMute(ADC_MUTE, ADC_CH_L);
        }
    }
}

void COsdCtrlAudioSpatial(UINT8 mode)
{
    switch (mode)
    {
    case 0:	// original
        CAudioSetMono2Stereo(AUD_DISABLE,ADC_M2S_L1);
        CAudioSetSpatialEffect(AUD_DISABLE, ADC_SPL_L1);
        break;
    case 1:	// stereo
        CAudioSetMono2Stereo(AUD_ENABLE,ADC_M2S_L2);
        CAudioSetSpatialEffect(AUD_DISABLE, ADC_SPL_L1);
        break;
    case 2:	// 1m
        CAudioSetMono2Stereo(AUD_ENABLE,ADC_M2S_L4);
        CAudioSetSpatialEffect(AUD_DISABLE, ADC_SPL_L1);
        break;
    case 3:	// 2m
        CAudioSetMono2Stereo(AUD_ENABLE,ADC_M2S_L4);
        CAudioSetSpatialEffect(AUD_ENABLE, ADC_SPL_L2);
        break;
    case 4:	// 4m
        CAudioSetMono2Stereo(AUD_ENABLE,ADC_M2S_L3);
        CAudioSetSpatialEffect(AUD_ENABLE, ADC_SPL_L3);
        break;
    default:
        CAudioSetMono2Stereo(AUD_DISABLE,ADC_M2S_L1);
        CAudioSetSpatialEffect(AUD_DISABLE, ADC_SPL_L1);
        break;
    }
}
#endif

UINT8 COsdCtrlGetAudioSpatial()
{
    return GET_SPATIAL() % 2;
}

void COsdCtrlSetAudioSpatial(void)
{
    UINT8 mode;
    APP_SE_CFG code m_cfg_se =
    {
        0x01F0,
        0,
        1,
        1,
        1,
        1,
        0x003F,
        0x003F
    };

    mode = ucCtrlArg0;
    SET_SPATIAL(mode);
    Audio_HwpSetMute(_ENABLE, ADC_CH_ALL, ADC_SYS_MUTE);
    Audio_HwpSetSpatialEffect(mode,&m_cfg_se);
    Audio_HwpSetMute(_DISABLE, ADC_CH_ALL, ADC_SYS_MUTE);
}


UINT8 COsdCtrlGetAudioBass(void)
{
    return GET_BASS();
}

void COsdCtrlSetAudioBass(void)
{
    UINT8 ucMid;
    UINT8 iVal2;//jokerxie
    INT8 iVal;

#if 1
    ucMid = (_OSD_AUDIO_BASS_MAX - _OSD_AUDIO_BASS_MIN) / 2;
    if ( ucCtrlArg0 > _OSD_AUDIO_BASS_MAX)
        ucCtrlArg0 = ucMid;
#else
    ucMid = (tAudioBassItem.ucMax - tAudioBassItem.ucMin) / 2;
    if ( ucCtrlArg0 > tAudioBassItem.ucMax)
        ucCtrlArg0 = ucMid;
#endif

    SET_BASS(ucCtrlArg0);

	iVal2 = ((UINT16)ucCtrlArg0*6)/(_OSD_AUDIO_BASS_MAX - _OSD_AUDIO_BASS_MIN);
    //iVal = ucCtrlArg0 - ucMid;
    iVal = iVal2 - ucMid;

    COsdCoreSetBass(iVal);
}

UINT8 COsdCtrlGetAudioTreble(void)
{
    return GET_TREBLE();
}

void COsdCtrlSetAudioTreble(void)
{
    UINT8 ucMid;
    INT8 iVal;

#if 1
    ucMid = (_OSD_AUDIO_TREBLE_MAX - _OSD_AUDIO_TREBLE_MIN)/2;
    if ( ucCtrlArg0 > _OSD_AUDIO_TREBLE_MAX)
        ucCtrlArg0 = ucMid;
#else
    ucMid = (tAudioTrebleItem.ucMax - tAudioTrebleItem.ucMin)/2;
    if ( ucCtrlArg0 > tAudioTrebleItem.ucMax)
        ucCtrlArg0 = ucMid;
#endif

    SET_TREBLE(ucCtrlArg0);

    iVal = ucCtrlArg0 - ucMid;

    COsdCoreSetTreble(iVal);
}

UINT8 COsdCtrlGetAudioBalance(void)
{
    return GET_BALANCE();
}

void  COsdCtrlSetAudioBalance(void)
{
    UINT8	ucMid;
    INT16	bal,scale;
    //ucMax = tAudioBalanceItem.ucMax;
    //ucMin = tAudioBalanceItem.ucMin;
    ucMid = (_OSD_AUDIO_BALANCE_MAX - _OSD_AUDIO_BALANCE_MIN)/2;

    if ( ucCtrlArg0 > _OSD_AUDIO_BALANCE_MAX)
        ucCtrlArg0 = ucMid;

    SET_BALANCE(ucCtrlArg0);

    bal = ucCtrlArg0;				// ucCtrlArg0 range from ucMin to ucMax
    bal -= ucMid;					// remap bal to -ucMid to +ucMid

    scale = APP_MAX_BAL/ucMid;
    bal = bal*scale;				// scale bal to MinBal to MaxBal

    //RTD_Log(LOGGER_INFO,"COsdCtrlSetAudioBalance\n");
    //RTD_Log(LOGGER_INFO,"ucMax=%d ,ucMin=%d\n",(UINT32)ucMax,(UINT32)ucMin);
    //RTD_Log(LOGGER_INFO,"ucMid=%d ,scale=%d\n",(UINT32)ucMid,(UINT32)scale);

    Audio_HwpSetBalanceLR(bal,_TRUE);
}

INT16 COsdCtrlGetAudioRemapVolume(UINT8 osd_vol)
{
    UINT8 i;
    INT16 j,k;
    INT16 int_vol=0;
    INT16 RemapVolumeTable[7];
    UINT8 code VolumeOsdTable[7] = {0, 1, 25, 50, 75, 100, 255};
#define REMAP_VOL_SIZE 7

    RemapVolumeTable[0] = APP_MIN_VOL;
    RemapVolumeTable[6] = APP_MAX_VOL;
    
    RTD_Log(LOGGER_LEVEL,"\n RemapVolumeTable[0]= %d ,RemapVolumeTable[6]= %d\n",(UINT32)RemapVolumeTable[0],(UINT32)RemapVolumeTable[6]);
#if ENABLE_AUDIO_SEP
if (GET_INPUTSOURCE_TYPE() == _SOURCE_HDMI)
#else
    if (GET_INPUTSOURCE_TYPE() == _SOURCE_TV)
#endif		
    {
        for (i=0; i<5; i++)
        {
            RemapVolumeTable[i+1] = GET_TV_VOLUME_CURVE(i);
           
            RTD_Log(LOGGER_LEVEL,"\n i=%d,RemapVolumeTable[i+1] = %d \n",(UINT32)i,(UINT32)RemapVolumeTable[i+1]);
        }
    }
    else
    {
        for (i=0; i<5; i++)
        {
            RemapVolumeTable[i+1] = GET_VOLUME_CURVE(i);
        }
    }

    // debug only --
    /*
    RemapVolumeTable[0] = APP_MIN_VOL;
    RemapVolumeTable[1] = -60*APP_VOL_STEP_1DB;
    RemapVolumeTable[2] = -30*APP_VOL_STEP_1DB;
    RemapVolumeTable[3] = -15*APP_VOL_STEP_1DB;
    RemapVolumeTable[4] = 0;
    RemapVolumeTable[5] = 10*APP_VOL_STEP_1DB;
    RemapVolumeTable[6] = APP_MAX_VOL;

    for(i=0;i<REMAP_VOL_SIZE;i++)
    {
    	RTD_Log(LOGGER_INFO,"MAP_VOL_TAB[%d] = %d\n",(UINT32)i,(UINT32)RemapVolumeTable[i]);
    }
    */
    // -------------

    for (i=0; i<REMAP_VOL_SIZE; i++)
    {
    
	    RTD_Log(LOGGER_LEVEL,"\n i = %d,VolumeOsdTable[i] = %d \n",(UINT32)i,(UINT32)VolumeOsdTable[i]);
        if (VolumeOsdTable[i] > osd_vol)
        {
            j = (RemapVolumeTable[i] - RemapVolumeTable[i-1]) * (osd_vol - VolumeOsdTable[i-1]);
            k = VolumeOsdTable[i] - VolumeOsdTable[i-1];
            int_vol = (k * RemapVolumeTable[i-1] + j) / k;
            break;
        }
    }
    //RTD_Log(LOGGER_LEVEL,"\n i = %d,VolumeOsdTable[i] = %d \n",(UINT32)i,(UINT32)VolumeOsdTable[i]);

    return (INT16)int_vol;
}

void COsdCtrlSetAudioRemapVolume(UINT8 osd_vol)
{
    INT16 int_vol;

	//RTD_Log(LOGGER_INFO, "Joker flag 222222222222222222");
	//RTD_Log(LOGGER_LEVEL,"\n Tim osd_vol = %d \n",(UINT32)osd_vol);
    int_vol = COsdCtrlGetAudioRemapVolume(osd_vol);
	//RTD_Log(LOGGER_LEVEL,"\n Tim int_vol = %d \n",(UINT32)int_vol);

//->w	RTD_Log(LOGGER_INFO,"OSD_Vol=%d,MAP_Vol=%d\n",(UINT32)osd_vol,(UINT32)int_vol);
    Audio_HwpSetVolume(APP_CH_ID_L | APP_CH_ID_R,int_vol);

}

UINT8 COsdCtrlGetAudioVolume()
{
    return GET_VOLUME();
}

void COsdCtrlSetAudioVolume(void)
{
    //RTD_Log(LOGGER_INFO, "Volume = %d\n", (UINT32)ucCtrlArg0);
	//RTD_Log(LOGGER_INFO, "Joker flag 111111111111111");
    SET_VOLUME(ucCtrlArg0);

    //if ( GET_INPUTSOURCE_TYPE() == _SOURCE_TV /*&& !(drv_ifd_TunerIsLocked(&afc)*/ && !drv_video_vdc_CheckHVisLocked())
    //	;
    //else if ( (rtdf_readByte0(DDOMAIN_MAIN_DISPLAY_CONTROL_RSV_VADDR) & _BIT1) != 0)	// force to background ?
    //	;
    //else
    COsdCtrlSetAudioRemapVolume(ucCtrlArg0*5);
}
UINT8 COsdCtrlGetAudioEQ(void)
{
    return GET_EQ_MODE() % _EQ_MODE_COUNT;
}

void COsdCtrlSetAudioEQ(UINT8 message)
{
    UINT8 mode = GET_EQ_MODE();
    if ( message == _RIGHT_KEY_MESSAGE)
        mode = (mode + 1) % _EQ_MODE_COUNT;
    else if ( message == _LEFT_KEY_MESSAGE)
    {
        if (mode == 0)
            mode = _EQ_MODE_COUNT-1;
        else
            mode--;
    }
    else
        return;

    SET_EQ_MODE(mode);
    COsdCoreSetEQ(mode);
}

UINT8 COsdCtrlGetAudioMTS(void)
{
    return GET_MTS() % _MTS_COUNT;
}

void COsdCtrlSetAudioMTS(UINT8 message)
{
    UINT8 mode = GET_MTS();
    if ( message == _RIGHT_KEY_MESSAGE)
        mode = (mode + 1) % _MTS_COUNT;
    else if ( message == _LEFT_KEY_MESSAGE)
    {
        if (mode == 0)
            mode = _MTS_COUNT-1;
        else
            mode--;
    }
    else
        return;

    SET_MTS(mode);
    #ifdef _VIDEO_TV_SUPPORT
    TVSetAudioMTS(mode);
    #endif
    //COsdCoreSetEQ(mode);
}


UINT8 COsdCtrlGetHPosition()
{
    if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
    {
        //return (50 + GET_H_POSITION() - 0x80);
        return (50 + 0x80 - GET_H_POSITION());
    }
    /*	else
    	{
    		ROSPrintf("GET_H_POSITION()=%x in COsdCtrlGetHPosition\n",GET_H_POSITION(pInfoOsdSrc->input_src));
    	 	HPositionTemp = GET_H_POSITION(pInfoOsdSrc->input_src) -0x80;
    	 	HPositionTemp +=_H_V_POSITION_MOVE_TOLERANCE;
    		//sms 2008-02-28
    		if(HPositionTemp < 0 || HPositionTemp >10)
    		{
    			HPositionTemp = _H_V_POSITION_MOVE_TOLERANCE;
    		}

    	 	//ROSPrintf("HPositionTemp=%d in COsdCtrlGetHPosition\n",HPositionTemp);
    	}
    	 return HPositionTemp;
    */
}

void COsdCtrlSetHPosition(void)
{
    if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
    {
        ucCtrlArg0 = 0x80+50- ucCtrlArg0;

        if (ucCtrlArg0 > ucIPH_Max_Margin)
            ucCtrlArg0 = ucIPH_Max_Margin;

        if (ucCtrlArg0 < ucIPH_Min_Margin)
            ucCtrlArg0 = ucIPH_Min_Margin;

        SET_H_POSITION(ucCtrlArg0);
        drv_syncproc_ScalerSetHPosition();

        stVGAModeCurrInfo.H_Position = ucCtrlArg0;
        ui_set_VGA_info(info->ucMode_Curr);
    }
    /*	else
    	{
          		UINT8 temp = GET_H_POSITION(info->input_src);
    		temp = temp + (message - HPositionTemp);
    		ROSPrintf("[Chengying] %d %d %d\n",message,temp,HPositionTemp);

    		if (pInfoOsdSrc->FrameSyncFlag || Scaler_SetCaptureWindowShift(pInfoOsdSrc->display, message - 5,COsdCtrlGetVPosition() - 5))
    			return;
    		SET_H_POSITION(Info->input_src,temp);
    	}
    */

}
#if 0
UINT8 COsdCtrlGetMaxVPosition(void)
{
    return 50 + (ucIPV_Max_Margin - 0x80);
}

UINT8 COsdCtrlGetMinVPosition(void)
{
    return 50 - (0x80 - ucIPV_Min_Margin);
}
#endif

UINT8 COsdCtrlGetVPosition()
{
#if 0
    VPositionTemp = 50;
#else
    UINT16 temp2;

    if (GET_INPUTSOURCE_TYPE()  == _SOURCE_VGA)
    {

        VPositionTemp = ABS(GET_V_POSITION(),0x80);
        //RTD_Log(LOGGER_INFO, "\nGET_V_POSITION_start=%d\nVPositionTemp_start=%d\n", (UINT32)GET_V_POSITION(),(UINT32)VPositionTemp);
        if (GET_V_POSITION()>= 0x80)
        {
            temp2 = VPositionTemp*50;
            VPositionTemp = (temp2/(ucIPV_Max_Margin -0x80))+50;
        }
        else
        {
            temp2 = VPositionTemp*50;
            VPositionTemp = 50 - (temp2/(0x80 - ucIPV_Min_Margin));
        }
    }
#endif
    //RTD_Log(LOGGER_INFO, "\nVPositionTemp=%d\n", (UINT32)VPositionTemp);

    return VPositionTemp;

}

void COsdCtrlSetVPosition(void)
{
#if 0
    return;
#else
    UINT8 temp;

    if (GET_INPUTSOURCE_TYPE()  == _SOURCE_VGA)
    {
        temp = GET_V_POSITION();
        temp =  temp + (ucCtrlArg0  - VPositionTemp);
//->w		RTD_Log(LOGGER_INFO, "\ntemp=%d\n", (UINT32)temp);

        SET_V_POSITION(temp);
        drv_syncproc_ScalerSetVPosition();

        stVGAModeCurrInfo.V_Position = temp;
        ui_set_VGA_info(info->ucMode_Curr);
    }
#endif

}

#if 0
UINT8 COsdCtrlGetHSize()
{
    return 0;
#if !defined(_DISABLE_OSD_CTRL_FUNC_)
    INT8 HSizeTemp = 0;

    if (GET_OSD_INPUTSOURCE_TYPE()  == _ADC_SRC_VGA ||GET_OSD_INPUTSOURCE_TYPE() == _TMDS_SRC_DVI)
    {
#ifdef PDP_PANEL //reynold  added 080904 for PDP model H size  problem
        if ((pInfoOsdSrc->ucMode_Curr>= _MODE_640x350_85HZ) && (pInfoOsdSrc->ucMode_Curr<= _MODE_640x480_85HZ))
            HSizeTemp=(GET_H_SIZE(pInfoOsdSrc->input_src) -75) / 5;
        else  if ((pInfoOsdSrc->ucMode_Curr>= _MODE_800x600_56HZ) && (pInfoOsdSrc->ucMode_Curr<= _MODE_832x624_75HZ))
            HSizeTemp=(GET_H_SIZE(pInfoOsdSrc->input_src) -80) / 4;
        else
#endif
            HSizeTemp=(GET_H_SIZE(pInfoOsdSrc->input_src) -85) / 3;

    }
    else if (GET_OSD_INPUTSOURCE_TYPE()  == _ADC_SRC_YPBPR || GET_OSD_INPUTSOURCE_TYPE() == _TMDS_SRC_HDMI)
    {
        switch (GET_H_SIZE(pInfoOsdSrc->input_src))
        {
        case 88:
            HSizeTemp = 0;
            break;
        case 92:
            HSizeTemp = 3;
            break;
        case 96:
        case 97:
            HSizeTemp = 5;
            break;
        case 100:
            HSizeTemp = 7;
            break;
        case 104:
            HSizeTemp = 10;
            break;
        }
    }
    else
        HSizeTemp= GET_H_SIZE(pInfoOsdSrc->input_src) - 90 ;

    if ((HSizeTemp < 0 ) || (HSizeTemp > 10))
        HSizeTemp = 5;

    return HSizeTemp;
#endif
}
#endif

#if 0
void COsdCtrlSetHSize(UINT8 message)
{
    message = 0;
#if !defined(_DISABLE_OSD_CTRL_FUNC_)
    UINT8 Osdhsize = 0;

    if ( pInfoOsdSrc->FrameSyncFlag || ((GET_INPUT_TYPE(pInfoOsdSrc->display)==_TMDS_SRC_HDMI) && (COsdCtrlGetHDMIDisplayRange() == 0)))

        return;

    if (COsdCtrlGetFreeze())
    {
        bFreeeEnable = 0;
        ScalerDisplay_SetFreeze(pInfoOsdSrc->channel, 0);
    }

    if (message ==0&& COsdCtrlGetHSize()==0)//reynold added 080728 , not to re-entry set H_size while H_size = 0
        return;

#ifdef DISPLAYASPECT_SEPARATE
    if (!IS_RATIO_16_9( GET_INPUTSOURCE_INDEX() ) )
#else
    if ( !(IS_RATIO_16_9() || IS_RATIO_4_3()) ) // 071228 charlie Modify 高清4:3，zoom 水平幅度不作用
#endif
    {
        if (GET_INPUT_TYPE(pInfoOsdSrc->display) == _ADC_SRC_VGA ||GET_INPUT_TYPE(pInfoOsdSrc->display) == _TMDS_SRC_DVI)
            Scaler_DisplayRatio_SetWindowSize(100,100);
        else if (GET_INPUT_TYPE(pInfoOsdSrc->display) == _ADC_SRC_YPBPR || GET_INPUT_TYPE(pInfoOsdSrc->display) == _TMDS_SRC_HDMI)
        {
            //leo 20080113 set the size to default value when the display range is full at HDMI
            if ((GET_INPUT_TYPE(pInfoOsdSrc->display)==_TMDS_SRC_HDMI) && (COsdCtrlGetHDMIDisplayRange() == 0))
                Scaler_DisplayRatio_SetWindowSize(100,100);
            else
                Scaler_DisplayRatio_SetWindowSize(96,97);
        }
        else
            Scaler_DisplayRatio_SetWindowSize(96,95);
    }
    else
    {
        if (GET_INPUT_TYPE(pInfoOsdSrc->display) == _ADC_SRC_VGA ||GET_INPUT_TYPE(pInfoOsdSrc->display) == _TMDS_SRC_DVI)
        {
#ifdef PDP_PANEL //reynold  added 080904 for PDP model H size problem
            if ((pInfoOsdSrc->ucMode_Curr>=_MODE_640x350_85HZ)&&(pInfoOsdSrc->ucMode_Curr<=_MODE_640x480_85HZ))
            {
                Osdhsize=message * 5 + 75;
                HSizeTemp = (10-message) * 5 + 75;
            }
            else if ((pInfoOsdSrc->ucMode_Curr>=_MODE_800x600_56HZ)&&(pInfoOsdSrc->ucMode_Curr<=_MODE_832x624_75HZ))
            {
                Osdhsize=message * 4 + 80;
                HSizeTemp = (10-message) * 4 + 80;
            }
            else
#endif
            {
                Osdhsize=message * 3 + 85;
                HSizeTemp = (10-message) * 3 + 85;
            }

            VSizeTemp = (10-COsdCtrlGetVSize()) * 3 + 85;
        }
        else if (GET_INPUT_TYPE(pInfoOsdSrc->display) == _ADC_SRC_YPBPR || GET_INPUT_TYPE(pInfoOsdSrc->display) == _TMDS_SRC_HDMI)
        {
            switch (message)
            {
            case 10:
                HSizeTemp = 88;
                Osdhsize= 104;
                break;
            case 7:

                HSizeTemp = 92;
                Osdhsize= 100;
                break;

            case 5:

                HSizeTemp = 96;
                Osdhsize= 96;
                break;
            case 3:

                HSizeTemp = 100;
                Osdhsize= 92;
                break;
            case 0:

                HSizeTemp = 104;
                Osdhsize= 88;
                break;
            }
            VSizeTemp = (10-COsdCtrlGetVSize()) + 92;

        }
        else
        {
            Osdhsize=message + 90;
            HSizeTemp = (10-message) + 90;
            HSizeTemp = 100 - ((100 - HSizeTemp) / 5) * 4;

            VSizeTemp = (10-COsdCtrlGetVSize()) + 90;
        }

        if ( ((HSizeTemp > 100) || (VSizeTemp > 100)) && (GET_SCREENSAVER() == SCREEN_SAVER_OFF) )
            ScalerDisplay_SetBgColor(pInfoOsdSrc->display, _DISPLAY_BG, 0x00, 0x00, 0x00);

        {

            if (Scaler_DisplayRatio_SetWindowSize(HSizeTemp,VSizeTemp))
            {
                return;
            }
            SET_H_SIZE(pInfoOsdSrc->input_src, Osdhsize);
            //allen add to save stScreenModeCurrInfo of VGA to stVGAModeCurrInfo[mode] by VGA mode
            if (GET_OSD_INPUTSOURCE_TYPE()==_ADC_SRC_VGA )
            {
                stVGAModeCurrInfo[disp_info[_CHANNEL1].ucMode_Curr] = stScreenModeCurrInfo[GET_INPUTSOURCE_INDEX()];
            }
            else if ( GET_OSD_INPUTSOURCE_TYPE()==_ADC_SRC_YPBPR)
            {
                stYPbPrModeCurrInfo[(disp_info[_CHANNEL1].ucMode_Curr -(_MAX_PRESET_MODE - _MAX_PRESET_YPBPR_MODE))] = stScreenModeCurrInfo[GET_INPUTSOURCE_INDEX()];
            }
        }
    }
#endif
}
#endif

#if 0
UINT8 COsdCtrlGetVSize()
{
    return 0;
#if !defined(_DISABLE_OSD_CTRL_FUNC_)
    INT8 VSizeTemp = 0;

    if (GET_OSD_INPUTSOURCE_TYPE() == _ADC_SRC_VGA ||GET_OSD_INPUTSOURCE_TYPE() == _TMDS_SRC_DVI)
        VSizeTemp = (GET_V_SIZE(pInfoOsdSrc->input_src)-85) / 3;
    else if (GET_OSD_INPUTSOURCE_TYPE() == _ADC_SRC_YPBPR || GET_OSD_INPUTSOURCE_TYPE() == _TMDS_SRC_HDMI)
        VSizeTemp=GET_V_SIZE(pInfoOsdSrc->input_src)-92;
    else
        VSizeTemp=GET_V_SIZE(pInfoOsdSrc->input_src)-90;

    if ((VSizeTemp < 0 ) || (VSizeTemp > 10))
        VSizeTemp = 5;

    return VSizeTemp;
#endif
}
#endif

#if 0
void COsdCtrlSetVSize(UINT8 message)
{
    message = 0;
#if !defined(_DISABLE_OSD_CTRL_FUNC_)
    UINT8 OsdVsize = 0;//Asen 20071026 add for H size increase with OSD slider

    if (IS_RATIO_PANORAMA() || pInfoOsdSrc->FrameSyncFlag ||  ((GET_INPUT_TYPE(pInfoOsdSrc->display)==_TMDS_SRC_HDMI) && (COsdCtrlGetHDMIDisplayRange() == 0)) )
        return;

    if (COsdCtrlGetFreeze())
    {
        bFreeeEnable = 0;
        ScalerDisplay_SetFreeze(pInfoOsdSrc->channel, 0);
    }

    if (GET_INPUT_TYPE(pInfoOsdSrc->display) == _ADC_SRC_VGA ||GET_INPUT_TYPE(pInfoOsdSrc->display) == _TMDS_SRC_DVI)
    {
        OsdVsize=message * 3 + 85;
        message=10-message;
        VSizeTemp = message * 3 + 85;

#ifdef PDP_PANEL //qeejee  added 080904 for PDP model H size problem
        if ((pInfoOsdSrc->ucMode_Curr>= _MODE_640x350_85HZ) && (pInfoOsdSrc->ucMode_Curr<= _MODE_640x480_85HZ))
            HSizeTemp = (10-COsdCtrlGetHSize()) * 5 + 75;
        else  if ((pInfoOsdSrc->ucMode_Curr>= _MODE_800x600_56HZ) && (pInfoOsdSrc->ucMode_Curr<= _MODE_832x624_75HZ))
            HSizeTemp = (10-COsdCtrlGetHSize()) * 4 + 80;
        else
#endif
            HSizeTemp = (10-COsdCtrlGetHSize()) * 3 + 85;
    }
    else if (GET_INPUT_TYPE(pInfoOsdSrc->display) == _ADC_SRC_YPBPR || GET_INPUT_TYPE(pInfoOsdSrc->display) == _TMDS_SRC_HDMI)
    {
        OsdVsize=message + 92;
        message=10-message;
        VSizeTemp =  message + 92; // move 3% for each in 90 - 100%

        switch (COsdCtrlGetHSize())
        {
        case 10:
            HSizeTemp = 88;
            break;
        case 7:
            HSizeTemp = 92;
            break;
        case 5:
            HSizeTemp = 96;
            break;
        case 3:
            HSizeTemp = 100;
            break;
        case 0:
            HSizeTemp = 104;
            break;

        }
    }
    else
    {
        OsdVsize=message + 90;
        message=10-message;
        VSizeTemp = message + 90;

        HSizeTemp = (10-COsdCtrlGetHSize()) + 90;
        HSizeTemp = 100 - ((100 - HSizeTemp) / 5) * 4; // move 5 for each
    }

    if ((HSizeTemp > 100) || (VSizeTemp > 100))
        ScalerDisplay_SetBgColor(pInfoOsdSrc->display, _DISPLAY_BG, 0x00, 0x00, 0x00);

    if (Scaler_DisplayRatio_SetWindowSize(HSizeTemp,VSizeTemp))
    {
        return;
    }

    SET_V_SIZE(pInfoOsdSrc->input_src , OsdVsize);

    if (GET_OSD_INPUTSOURCE_TYPE()==_ADC_SRC_VGA )
    {
        stVGAModeCurrInfo[disp_info[_CHANNEL1].ucMode_Curr] = stScreenModeCurrInfo[GET_INPUTSOURCE_INDEX()];
    }
    else if ( GET_OSD_INPUTSOURCE_TYPE()==_ADC_SRC_YPBPR)
    {
        stYPbPrModeCurrInfo[(disp_info[_CHANNEL1].ucMode_Curr -(_MAX_PRESET_MODE - _MAX_PRESET_YPBPR_MODE))] = stScreenModeCurrInfo[GET_INPUTSOURCE_INDEX()];
    }
#endif
}
#endif

#if 0
UINT8 COsdCtrlGetHDMIDisplayRange(void)
{
    if (GET_OSD_INPUTSOURCE_TYPE() ==_TMDS_SRC_HDMI)
        return (GET_Display_Range(pInfoOsdSrc->input_src)%2);
    else
        return 0;
}

void COsdCtrlSetHDMIDisplayRange(UINT8 message)
{
    if (pInfoOsdSrc->FrameSyncFlag)
        return;

    if (GET_OSD_INPUTSOURCE_TYPE() ==_TMDS_SRC_HDMI)
    {
        SET_Display_Range(pInfoOsdSrc->input_src,(GET_Display_Range(pInfoOsdSrc->input_src)+1)%2);

#if 1
        if (GET_Display_Range(pInfoOsdSrc->input_src) == 0)
        {
            //leo 20080113 set the position to default value when the display range is full
            Scaler_SetCaptureWindowShift(pInfoOsdSrc->display, 0, 0);

            Scaler_DisplayRatio_SetWindowSize(100,100);
        }
        else
        {
            //leo 20080113 set the position to the previously stored value when the display range is zoom
            Scaler_SetCaptureWindowShift(info->display, GET_H_POSITION(info->input_src)  -0x80, GET_V_POSITION(info->input_src)  -0x80);
#if 0
            //Scaler_DisplayRatio_SetWindowSize(96,96);
            COsdCtrlSetHSize(COsdCtrlGetHSize());
#endif
        }
#endif

    }
    else if (GET_OSD_INPUTSOURCE_TYPE() ==_TMDS_SRC_DVI)
    {
#if 1
        Scaler_DisplayRatio_SetWindowSize(100,100);
#endif
        SET_Display_Range(pInfoOsdSrc->input_src,0);
    }
    else
        return;
}
#endif

UINT8 COsdCtrlGetClock()
{
    if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
    {
        //ClockTemp = (GET_CLOCK() + 50 - 0x80);
        //return ClockTemp;
        return (GET_CLOCK() + 50 - 0x80);
    }
}

void COsdCtrlSetClock(void)
{
    drv_scaler_enable_measure_wdg(_DISABLE);
    drv_scaler_enable_fs_wdg(_FALSE);
    //// Disable On-Line Measure
    //rtdf_clearBits(SP_MS1CTRL_0x0900, (_BIT16|_BIT9 |_BIT10)); //disable online measure

    /*COsdCtrlGetClock();

    	if(ucCtrlArg0-ClockTemp >1)
    	ucCtrlArg0 = ClockTemp + 1;
    else if(ucCtrlArg0-ClockTemp <-1)
    	ucCtrlArg0 = ClockTemp - 1;*/

    SET_CLOCK(0x80 + ucCtrlArg0 - 50);
    drv_syncproc_Adjust_ADC_Clock(_FALSE);



    flow_mode_ClearOnMeasureflag();
    //fw_timer_event_DelayXms(60);
    //drv_measure_enableonlinemeasure();
    drv_scaler_enable_measure_wdg(_ENABLE);
    drv_scaler_enable_fs_wdg(_TRUE);

    stVGAModeCurrInfo.Clock = GET_CLOCK();
    ui_set_VGA_info(info->ucMode_Curr);
    //Message_ClearMessage(_REPEAT_KEY_MESSAGE);	// CF 20080529 added, to stop clock adjustment if user release RC key
}

UINT8 COsdCtrlGetPhase()
{
    return GET_PHASE();
}

void COsdCtrlSetPhase(void)
{
    SET_PHASE(ucCtrlArg0);
    drv_syncproc_YPbPr_SetPhase(ucCtrlArg0, _TRUE);

    stVGAModeCurrInfo.Phase = GET_PHASE();
    ui_set_VGA_info(info->ucMode_Curr);
}

#if 0
void COsdCtrlSetColorRed(UINT8 message)
{
    UINT8 R_val, G_val = GET_USER_COLORTEMP_USER_G(pInfoOsdSrc->input_src), B_val= GET_USER_COLORTEMP_USER_B(pInfoOsdSrc->input_src);
    R_val = message*2.55;//map 0~100 to 0~255
    G_val*=2.55;
    B_val*=2.55;
    SET_USER_COLORTEMP_USER_R(pInfoOsdSrc->input_src, message);
    Color_SetRgbContrast(pInfoOsdSrc->display, R_val, G_val, B_val, GET_PIP_BLENDING());
}

void COsdCtrlSetColorGreen(UINT8 message)
{
    UINT8 R_val = GET_USER_COLORTEMP_USER_R(pInfoOsdSrc->input_src), G_val, B_val= GET_USER_COLORTEMP_USER_B(pInfoOsdSrc->input_src);

    R_val*=2.55;//map 0~100 to 0~255
    G_val=message*2.55;
    B_val*=2.55;
    SET_USER_COLORTEMP_USER_G(pInfoOsdSrc->input_src, message);
    Color_SetRgbContrast(pInfoOsdSrc->display, R_val, G_val, B_val, GET_PIP_BLENDING());
}

void COsdCtrlSetColorBlue(UINT8 message)
{
    UINT8 R_val= GET_USER_COLORTEMP_USER_R(pInfoOsdSrc->input_src), G_val = GET_USER_COLORTEMP_USER_G(pInfoOsdSrc->input_src), B_val;

    R_val*=2.55;//map 0~100 to 0~255
    G_val*=2.55;
    B_val=message*2.55;
    SET_USER_COLORTEMP_USER_B(pInfoOsdSrc->input_src, message);
    Color_SetRgbContrast(pInfoOsdSrc->display, R_val, G_val, B_val, GET_PIP_BLENDING());
}
#endif
extern UINT8 xdata sleep_10ms;
extern UINT8 xdata sleep_sec;
extern UINT8 xdata sleep_min;
extern UINT8 xdata ttse ;
void CShowTimer()
{
    ttse = sleep_sec;

	if(!ucShowSleepFlag)
	{
	OSDLine(ROW(3), COL(1), LENGTH(37), 0xA6, _WRITE_BYTE0);
	OSDLine(ROW(3), COL(1), LENGTH(37), 0xA4, _WRITE_BYTE1);
	OSDLine(ROW(3), COL(1), LENGTH(37), 0x11, _WRITE_BYTE2);

	OSDLine(ROW(16), COL(1), LENGTH(37), 0xA6, _WRITE_BYTE0);
	OSDLine(ROW(16), COL(1), LENGTH(37), 0xA4, _WRITE_BYTE1);
	OSDLine(ROW(16), COL(1), LENGTH(37), 0x11, _WRITE_BYTE2);
    
    ap_osdfx_map_setting(_OSD_MAP0);
    SetOSDDouble(3);
    // display channel number
    OSDLine(ROW(1), COL(1), LENGTH(8), 0xF0, _WRITE_BYTE2);
    OSDLine(ROW(2), COL(1), LENGTH(8), 0xF0, _WRITE_BYTE2);
    COsdFxSetWinRefPos(_OSD_TVCHANNEL, 10);
    COsdFxEnableOsd();
    ucShowSleepFlag = 1;
    
	}
	//CGetSleepString();
    //COsdFxLoadText(2, 2, 0, FONT_COLOR(15, _CP_BLACK), 0, pStr);

    COsdFxShowNumber(2, 2, sleep_sec, 0x23, 15);
    
    fw_timer_event_CancelTimerEvent(timerevent_ui_table_OSD_timeout_event);
    //RTD_Log(LOGGER_LEVEL,"\n Tim sleep \n");

}
#if DISABLE_OPTTION

void COsdCoreShowSleepType()
{
	OSDLine(ROW(3), COL(1), LENGTH(37), 0xA6, _WRITE_BYTE0);
	OSDLine(ROW(3), COL(1), LENGTH(37), 0xA4, _WRITE_BYTE1);
	OSDLine(ROW(3), COL(1), LENGTH(37), 0x11, _WRITE_BYTE2);

	OSDLine(ROW(16), COL(1), LENGTH(37), 0xA6, _WRITE_BYTE0);
	OSDLine(ROW(16), COL(1), LENGTH(37), 0xA4, _WRITE_BYTE1);
	OSDLine(ROW(16), COL(1), LENGTH(37), 0x11, _WRITE_BYTE2);
    
    ap_osdfx_map_setting(_OSD_MAP0);
    SetOSDDouble(3);
    // display channel number
    OSDLine(ROW(1), COL(1), LENGTH(8), 0xF0, _WRITE_BYTE2);
    OSDLine(ROW(2), COL(1), LENGTH(8), 0xF0, _WRITE_BYTE2);

	CGetSleepString();


    COsdFxEnableOsd();
    fw_timer_event_ReactiveTimerEvent(SEC(2), timerevent_ui_table_OSD_timeout_event);
}
#endif
void COsdSetSleepMode(void)
{
     ucSleepType = (ucSleepType + 1) % 6;
     
     if ( ucSleepType == 0)
     {
         EA = 0;
         // disable sleep timer
         sleep_sec = 0;
         sleep_min = 0;
         sleep_10ms = 0;
         EA = 1;
         return;
     }
     else
     {
         EA = 0;
         sleep_10ms = 0;
         sleep_sec = ucSleepType * 30;
         sleep_min = 0;//ucSleepType * 30;
         EA = 1;
     }

}

/**
 *
 *
 */
void COsdCtrlSetSleepMode(void)
{
    if ( ucCtrlArg0 == _RIGHT_KEY_MESSAGE)
        ucSleepType = (ucSleepType + 1) % 6;
    else if ( ucCtrlArg0 == _LEFT_KEY_MESSAGE)
    {
        if (ucSleepType == 0)
            ucSleepType = 5;
        else
            ucSleepType--;
    }
    else
        return;

    if ( ucSleepType == 0)
    {
        EA = 0;
        // disable sleep timer
        sleep_sec = 0;
        sleep_min = 0;
        sleep_10ms = 0;
        EA = 1;
        return;
    }
    else
    {
        EA = 0;
        sleep_10ms = 0;
        sleep_sec = 0;
        sleep_min = ucSleepType * 30;
        EA = 1;
    }
    //fw_timer_event_CancelTimerEvent(COsdReckonTimeEvent);
}


UINT8 COsdCtrlGetSleepMode(void)
{
    return (ucSleepType%6);
}

//For Picture
UINT8 COsdCtrlGetSharpness1(void)
{
    return GET_SHARPNESS();
}

void COsdCtrlSetSharpness1(void)
{
    SET_SHARPNESS(ucCtrlArg0);
//->w	RTD_Log(LOGGER_INFO, "VIP: Sharpness1 flow_vip_Get_Input_Mode=%d\n",(UINT32)(flow_vip_Get_Input_Mode()));
    drvif_color_peaking_config(flow_vip_Get_Input_Mode(), ucCtrlArg0);
//	drvif_color_peaking_config(GET_INPUTSOURCE_TYPE(), ucCtrlArg0);
}

UINT8 COsdCtrlGetPictureMode(void)
{
    return (GET_PICTURE_MODE() % _PIC_MODE_COUNT);
}

void COsdCtrlSetPictureMode(UINT8 message)
{
    UINT8 new_sat=0;
    UINT8 mode = GET_PICTURE_MODE();
    if ( message == _RIGHT_KEY_MESSAGE)
        mode = (mode + 1) % _PIC_MODE_COUNT;
    else if ( message == _LEFT_KEY_MESSAGE)
    {
        if (mode == 0)
            mode = _PIC_MODE_COUNT-1;
        else
            mode--;
    }
    else
        return;

    SET_PICTURE_MODE(mode);

    SET_BRIGHTNESS(GET_PICTURE_MODE_BRI(mode));
    SET_CONTRAST(GET_PICTURE_MODE_CONT(mode));
    SET_SATURATION(GET_PICTURE_MODE_SAT(mode));
    SET_HUE(GET_PICTURE_MODE_HUE(mode));
    SET_SHARPNESS(GET_PICTURE_MODE_SHARPNESS(mode));

    /*
    	switch(mode)
    	{
    	case _PIC_MODE_VIVID:
    		#ifdef CONFIG_USE_CUSTOMER1_FACTORY_MENU
    		SET_BRIGHTNESS(pInfoOsdSrc->input_src, GET_PICTUREMODE_BRIGHTNESS(pInfoOsdSrc->input_src,_PICTURE_MODE_BRIGHT));
    		SET_CONTRAST(pInfoOsdSrc->input_src, GET_PICTUREMODE_CONTRAST(pInfoOsdSrc->input_src,_PICTURE_MODE_BRIGHT));
    		SET_SATURATION(pInfoOsdSrc->input_src, GET_PICTUREMODE_COLOR(pInfoOsdSrc->input_src,_PICTURE_MODE_BRIGHT));
    		#else
    		SET_BRIGHTNESS(42);
    		SET_CONTRAST(55);
    		SET_SATURATION(78);
    		#endif
    //		if  (!GET_VUZD_BUFFER_NR_MODE(pInfoOsdSrc->display))		//HCount >1440,DNR is Off
    //			SET_NRSPATIAL(0);			//off
    //		else
    //			SET_NRSPATIAL(2);			//mid
    		break;

    	case _PIC_MODE_STANDARD:
    		#ifdef CONFIG_USE_CUSTOMER1_FACTORY_MENU
    		SET_BRIGHTNESS(pInfoOsdSrc->input_src, GET_PICTUREMODE_BRIGHTNESS(pInfoOsdSrc->input_src,_PICTURE_MODE_NORMAL));
    		SET_CONTRAST(pInfoOsdSrc->input_src, GET_PICTUREMODE_CONTRAST(pInfoOsdSrc->input_src,_PICTURE_MODE_NORMAL));
    		SET_SATURATION(pInfoOsdSrc->input_src, GET_PICTUREMODE_COLOR(pInfoOsdSrc->input_src,_PICTURE_MODE_NORMAL));
    		#else
    //		SET_BRIGHTNESS(pInfoOsdSrc->input_src,GET_STD_BRIGHTNESS(pInfoOsdSrc->input_src));	//50
    //		SET_CONTRAST(pInfoOsdSrc->input_src,GET_STD_CONTRAST(pInfoOsdSrc->input_src));		//50
    //		SET_SATURATION(pInfoOsdSrc->input_src,GET_STD_SATURATION(pInfoOsdSrc->input_src));	//70
    		#endif
    //		if  (!GET_VUZD_BUFFER_NR_MODE(pInfoOsdSrc->display))		//HCount >1440,DNR is Off
    //			SET_NRSPATIAL(pInfoOsdSrc->input_src, 0);			//off
    //		else
    //			SET_NRSPATIAL(pInfoOsdSrc->input_src, GET_STD_NRSPATIAL(pInfoOsdSrc->input_src));		//off
    		break;

    	case _PIC_MODE_GENTLE:
    		#ifdef CONFIG_USE_CUSTOMER1_FACTORY_MENU
    		SET_BRIGHTNESS(pInfoOsdSrc->input_src, GET_PICTUREMODE_BRIGHTNESS(pInfoOsdSrc->input_src,_PICTURE_MODE_SOFT));
    		SET_CONTRAST(pInfoOsdSrc->input_src, GET_PICTUREMODE_CONTRAST(pInfoOsdSrc->input_src,_PICTURE_MODE_SOFT));
    		SET_SATURATION(pInfoOsdSrc->input_src, GET_PICTUREMODE_COLOR(pInfoOsdSrc->input_src,_PICTURE_MODE_SOFT));
    		#else
    		SET_BRIGHTNESS(50);
    		SET_CONTRAST(44);
    		SET_SATURATION(65);
    		#endif
    //		if  (!GET_VUZD_BUFFER_NR_MODE(pInfoOsdSrc->display))		//HCount >1440,DNR is Off
    //			SET_NRSPATIAL(pInfoOsdSrc->input_src, 0);		//off
    //		else
    //			SET_NRSPATIAL(pInfoOsdSrc->input_src, 3);		//high
    		break;

    	case _PIC_MODE_USER:// user
    		SET_BRIGHTNESS(GET_PICTURE_MODE_BRI(_PIC_MODE_USER));
    		SET_CONTRAST(GET_PICTURE_MODE_CONT(_PIC_MODE_USER));
    		SET_SATURATION(GET_PICTURE_MODE_SAT(_PIC_MODE_USER));
    //		if  (!GET_VUZD_BUFFER_NR_MODE(pInfoOsdSrc->display))		//HCount >1440,DNR is Off
    //			SET_NRSPATIAL(pInfoOsdSrc->input_src, 0);			//off
    //		else
    //			SET_NRSPATIAL(pInfoOsdSrc->input_src, GET_USER_NRSPATIAL(pInfoOsdSrc->input_src));
    		break;

    	default:
    		break;
    	}
    */

    // apply PQ Saturation setting
    new_sat = COsd_remapping( GET_SATURATION(), 60);
    drv_color_AdjustSaturation(new_sat);
    // apply PQ Contrast/Brightness setting
    COsdCtrlSetYCB();
    // apply PQ Saturation setting
    //new_sat = COsd_remapping( GET_SATURATION(), 50);
    //drv_color_AdjustSaturation(new_sat);
    // apply PQ Hue setting
    if (GET_INPUTSOURCE_FROM()==_SRC_FROM_VDC)
    {
        if (GET_CURRENT_MODE() == _MODE_480I)
            drv_color_AdjustHue(GET_HUE());
        else
            drv_color_AdjustHue(50);
    }
    else
        drv_color_AdjustHue(GET_HUE());

    // apply PQ sharpness setting
    drvif_color_peaking_config(flow_vip_Get_Input_Mode(), GET_SHARPNESS());
//	drvif_color_peaking_config(GET_INPUTSOURCE_TYPE(), GET_SHARPNESS());

    //COsdCtrlSetICMSHI();
    //COsdCtrlColorTemp();
    COsdCtrlDNR();
}

/**
 * Display Ratio Control functions
 */
UINT8 COsdCtrlGetDisplayRatio(void)
{
    return GET_OSD_DISPLAY_RATIO();
}

void COsdCtrlSetDisplayRatio(void)
{
	//UINT8 ratio;

    if (ucCtrlArg0 !=GET_OSD_DISPLAY_RATIO())
    {
        SET_OSD_DISPLAY_RATIO(ucCtrlArg0);

        SYNC_STORAGE_DATA(StructSystemDataType,  ZoomMode, ucCtrlArg0);
        if (ucCtrlArg0 == 2) // auto default set to 4:3
        {
            /*if (GET_INPUTSOURCE_INDEX() == _SOURCE_INDEX_HDMI0)
            {
                ratio = drv_hdmi_AFD();
                RTD_Log(LOGGER_DEBUG, "HDMI AFD = %d \n",(UINT32)ratio);
                if (ratio == 1)
                    SET_DISPLAY_RATIO(0);
                else if (ratio == 2)
                    SET_DISPLAY_RATIO(1);           
            }
            else
                SET_DISPLAY_RATIO(0);*/
            SET_DISPLAY_RATIO(1);
        }
        else
            SET_DISPLAY_RATIO(ucCtrlArg0);
			COsdFxDisableOsd();//melon 20120217
           // RTD_Log(LOGGER_INFO, "MELON--------------------------------...\n");

        //Scaler_EnableFrameSyncWatchDog(_MAIN_DISPLAY, _FALSE);
        fw_flow_message_push(_UI2FL_MAIN_DISPLAYRATIO_CHANGE);
    }
}

UINT8 COsdCtrlGetDisplayIR(void)
{
	
    return GET_OSD_DISPLAY_IR();
}
void COsdCtrlSetDisplayIR(void)
{

	//RTD_Log(LOGGER_LEVEL,"\n *****melon***********ucCtrlArg0=%d ,disp_info.IR_CTRL=%d\n",(UINT32)ucCtrlArg0,(UINT32)disp_info.IR_CTRL);

    if (ucCtrlArg0 !=GET_OSD_DISPLAY_IR())
    {
        SET_OSD_DISPLAY_IR(ucCtrlArg0);
		SYNC_STORAGE_DATA(StructSystemDataType,  IrMode, ucCtrlArg0);
       
        if (ucCtrlArg0 == 0) 
        {
			STB_IR_EN(1);
           
        }
        else
    	{
    		STB_IR_EN(0);
			
    	  		 if(ucCtrlArg0==1)
    	   		{
		   		STB_IR_CH(0);
    	   		}
		   	else
	   		{
				STB_IR_CH(1);	
	   		}
	   		
		  	
    	}	
    }

}
#if CONFIG_ENABLE_FM
UINT16 COsdCtrlGetDisplayRadio(void)
{
	
    return GET_OSD_DISPLAY_Radio();
}
void COsdCtrlSetDisplayRadio(void)
{

	//RTD_Log(LOGGER_LEVEL,"\n ucCtrlArg0=%d ,disp_info.IR_CTRL=%d\n",(UINT32)ucCtrlArg0,(UINT32)disp_info.IR_CTRL);

    if (ucCtrlArg0 !=GET_OSD_DISPLAY_Radio())
    {
        SET_OSD_DISPLAY_Radio(ucCtrlArg0);
		
		SYNC_STORAGE_DATA(StructSystemDataType,  FMMode, ucCtrlArg0);
       
        AdjustFMfun(ucCtrlArg0);
		
		
    }

}

#endif

/* =========================================================================== */
//For Audio

#if 0
void COsdCtrlAudioAVL(UINT8 mode)
{
    // Sam, Jun-25-2008
// Playing fixed freq. audio, when switch AVC on/off, output POP noise.
    UINT8 ucFlag = 0;
    if (0 == ((ADC_CH_L|ADC_CH_R) & GetSysMuteStatus()))
    {
        ucFlag = 1;
        CAudioSetMute(ADC_MUTE,ADC_CH_L|ADC_CH_R);
        fw_timer_event_DelayXms(50);
    }
    switch (mode)
    {
    case 0: //off
        CAudioEnableAVC(AUD_DISABLE, &avc_cfg_osd); // turn on/off AVC
        // charlie, 080701, Debug for spdif output controled by volume
        ///CAudioSetFixedVolOut(AUD_ENABLE);//fixed AVC enable cause SPDIF no Fixed volume
        break;
    case 1: //on
        //ivan+ for debug only
        ROSPrintf("[OSD-AVC] sl=%d, bo=%d,th=%d,ti=%d\n",
                  avc_cfg_osd.slope,
                  avc_cfg_osd.boost,
                  avc_cfg_osd.comp_threshold,
                  avc_cfg_osd.rel_time
                 );
        CAudioEnableAVC(AUD_ENABLE, &avc_cfg_osd); // turn on/off AVC
        // charlie, 080701, Debug for spdif output controled by volume
        ///CAudioSetFixedVolOut(AUD_DISABLE);//fixed AVC enable cause SPDIF no output
        break;
    }

    // Sam, Jun-25-2008
    // Playing fixed freq. audio, when switch AVC on/off, output POP noise.
    if (ucFlag)
    {
        CAudioSetMute(ADC_UNMUTE,ADC_CH_L|ADC_CH_R);
        fw_timer_event_DelayXms(0x50);
    }

}



UINT8 COsdCtrlGetAudioAVL(void)
{
    return 0;
    //return (GET_AUDIO_AVL()%2);
}

void COsdCtrlSetAudioAVL(UINT8 message)
{
    message = 0;
#if !defined(_DISABLE_OSD_CTRL_FUNC_)
#ifndef  CONFIG_AUDIO_AVC //reynold modified 080508, AVC can process in all source
    if (GET_OSD_INPUTSOURCE_TYPE() != _VDC_SRC_TV)
        return;
#endif
    UINT8 mode = (GET_AUDIO_AVL()+1)%2;
    SET_AUDIO_AVL(mode);
    COsdCtrlAudioAVL(mode);
#endif
}
#endif
#ifdef _VIDEO_TV_SUPPORT
//For TV Tune
UINT8 CConverColorModeProc(UINT8 soundmode)
{
	UINT8 i,count = 0;
	UINT8 temp;
    //RTD_Log(LOGGER_LEVEL,"\n Tim soundmode = %d \n",(UINT32)soundmode);
	for(i = 1;i <= 5;i++)//Edit SVN53
	{
		temp = usColorSystemMode_MASK >> i;
        //RTD_Log(LOGGER_LEVEL,"\n Tim i = %d,usSoundMode_MASK =%d \n",(UINT32)i,(UINT32)temp);
		if(temp&0x01)
			count ++;
		//RTD_Log(LOGGER_LEVEL,"\n Tim count = %d \n",(UINT32)count);
		if(i == soundmode)
			return count;
	}
	return 0;
}

UINT8 COsdCtrlGetTVVideoStd(void)  //jacklong 2007/09/20 modify
{
    UINT8 ColorStd, type=0;
    UINT8 CurChn;
	CurChn = GET_TV_CHN_DATA(_CUR_CHN_TV);
	
    ColorStd = GET_CHANNEL_COLORSYSTEM(CurChn);

    if (ColorStd==ZNTSC)
        type= _SIG_MODE_NTSC;
    else if ((ColorStd==ZPAL_I)||(ColorStd==ZPAL_60)||(ColorStd==ZPAL_M)||(ColorStd==ZPAL_N))
        type= _SIG_MODE_PAL;
    else if ((ColorStd==ZSECAM)||(ColorStd==ZSECAML)||(ColorStd==ZSECAMLA))
        type = _SIG_MODE_SECAM;
    else if (ColorStd==ZNTSC_443)
        type = _SIG_MODE_NTSC_443;
    else
        type = _SIG_MODE_AUTO;
        
    //type = CConverColorModeProc(type);

    return type;
}

void COsdCtrlSetTVVideoStd(void)  //jacklong 2007/09/20 modify
{
    UINT8 type,std;
    UINT8 CurChn;
    switch (ucCtrlArg0)
    {
    case _SIG_MODE_AUTO:
        std = ZAUTO;
        break;

    default:
    case _SIG_MODE_PAL:
        std = ZPAL_I;
        break;

    case _SIG_MODE_NTSC:
        std = ZNTSC;
        break;

    case _SIG_MODE_NTSC_443:
        std = ZNTSC_443;
        break;

    case _SIG_MODE_SECAM:
        std = ZSECAM;
        break;
    }
	CurChn = GET_TV_CHN_DATA(_CUR_CHN_TV);
	
    type = GET_CHANNEL_COLORSYSTEM(CurChn);

    /*-------------------
    when soundsystem is _SOUND_MN and colorsystem is pal_i,correct color_sytem to pal m,
    prevent set  error videostd
       tommy modify20100407
       -------------------*/
    if (std==ZPAL_I)
    {
        if (COsdCtrlGetSoundSystem()==_SOUND_MN)
        	std =ZPAL_M;
        else if (COsdCtrlGetSoundSystem()==_SOUND_N)
            std =ZPAL_N;
    }

    if (std==ZSECAM)
    {
        if (COsdCtrlGetSoundSystem()==_SOUND_L)
        {
            std =ZSECAML;
            drvif_tuner_init(_SIG_MODE_SECAM_L);
        }
		else
        drvif_tuner_init(_SIG_MODE_PAL);
        fw_timer_event_DelayXms(100);
        //if (!drv_video_vdc_CheckHVisLocked())//Edit SVN53
          //  std =ZSECAML;
    }


    if (type == std)	// the same color system do nothing
        return;
    SET_CHANNEL_COLORSYSTEM(CurChn, std);
    if(std == ZNTSC)
    {	
	    if (GET_CHANNEL_SOUNDSYSTEM(CurChn)==ATV_SOUND_STD_PAL_N)
	    	SET_CHANNEL_SOUNDSYSTEM(CurChn, ATV_SOUND_STD_A2_M);
	}
    Audio_HwpSetMute(_TRUE,ADC_CH_ALL, ADC_SYS_MUTE);//Tim 0404

        //fw_timer_event_DelayXms(400);
    flow_audio_Setmute(ADC_MUTE, SCALER, (ADC_CH_IDX)(ADC_CH_L|ADC_CH_R));
    fw_tv_setvideomode(std, 0);//Edit SVN53
    
    if (std != ZAUTO)  	//if not auto mode, reset again
    {
        disp_info.state = _MODE_STATE_SEARCH;
        //RTD_Log(LOGGER_LEVEL,"\n Tim bg8 \n");
        //drv_scalerdisplay_set_bg_color(0,0,0xff);
        //drv_scalerdisplay_ForceBackground(_TRUE);
        flow_mode_DisableChannelProc();
        COsdFxDisableOsd(); // Prevent OSD flick

#ifdef CONFIG_TUNER_SILICON
        //tommy modify 20100702 ,reset set freq and init silicon tuner ,prevent tv snow noise
        if (std ==  ZSECAMLA)
        {
            drv_tuner_initRT810(_SIG_MODE_PAL);
            drv_tuner_SetFreq(tvScanCurFreq -SECAMLA_FREQ_SHIFT);
        }
        else
        {
            if ((std ==  ZNTSC)||(std == ZPAL_M)||(std == ZPAL_N))
                drv_tuner_initRT810(_SIG_MODE_NTSC);
            else
                drv_tuner_initRT810(_SIG_MODE_PAL);
            drv_tuner_SetFreq(tvScanCurFreq);
        }
#endif

    }

    //Edit SVN53

    /*-------------------
    when soundsystem is _SOUND_L and colorsystem is ZAUTO,
    init tuner parameter by _SIG_MODE_SECAM_L,no by _SIG_MODE_PAL
       tommy modify20100407
       -------------------*/
    if (std==ZAUTO)
    {
    //rtdf_maskl(VDC_MANUAL_MODE_0x51c0, 0x78, 0x80);
        if (COsdCtrlGetSoundSystem()==_SOUND_L)
        {
            drvif_tuner_init(_SIG_MODE_SECAM_L);
            drv_tuner_SetFreq(tvScanCurFreq);
        }

    }

}
//==============================================================================
UINT8 CConverModeProc(UINT8 soundmode)
{
	UINT8 i,count = 0;
	UINT8 temp;
    //RTD_Log(LOGGER_LEVEL,"\n Tim soundmode = %d \n",(UINT32)soundmode);
	for(i = 1;i <= 6;i++)
	{
		temp = usSoundMode_MASK >> i;
        //RTD_Log(LOGGER_LEVEL,"\n Tim i = %d,usSoundMode_MASK =%d \n",(UINT32)i,(UINT32)temp);
		if(temp&0x01)
			count ++;
		//RTD_Log(LOGGER_LEVEL,"\n Tim count = %d \n",(UINT32)count);
		if(i == soundmode)
			return count;
	}
	return 0;
}
//==============================================================================

UINT8 COsdSoundSystemSwitch(UINT8 std)
{
    UINT8 type;
    switch (std)
    {
    case ATV_SOUND_STD_DK_MONO:
    case ATV_SOUND_STD_A2_DK1:
    case ATV_SOUND_STD_A2_DK2:
    case ATV_SOUND_STD_A2_DK3:
    case ATV_SOUND_STD_NICAM_DK:
        type=_SOUND_DK;
        break;
    case ATV_SOUND_STD_FM_MONO_NO_I:
    case ATV_SOUND_STD_NICAM_I:
        type=_SOUND_I;
        break;
    case ATV_SOUND_STD_BG_MONO:
    case ATV_SOUND_STD_A2_BG:
    case ATV_SOUND_STD_NICAM_BG:
        type=_SOUND_BG;
        break;
    case ATV_SOUND_STD_MN_MONO:
    case ATV_SOUND_STD_BTSC:
    case ATV_SOUND_STD_A2_M:
    case ATV_SOUND_STD_EIAJ:
        type=_SOUND_MN;
        break;
    case ATV_SOUND_STD_AM_MONO:
    case ATV_SOUND_STD_NICAM_L:
        type = _SOUND_L;
        break;

    case ATV_SOUND_STD_PAL_N:
        type = _SOUND_N;
        break;
        

    default:
        type=_SOUND_AUTO;
        break;
    }
    //RTD_Log(LOGGER_LEVEL,"\n Tim 0000000\n");
    //type = CConverModeProc(type);
    //RTD_Log(LOGGER_LEVEL,"\n Tim type=%d \n",(UINT32)type);
    return type;
}

UINT8 COsdCtrlGetSoundSystem(void)
{
    UINT8 soundStd;
    UINT8 CurChn;
	CurChn = GET_TV_CHN_DATA(_CUR_CHN_TV);
	
    soundStd = GET_CHANNEL_SOUNDSYSTEM(CurChn);
    //RTD_Log(LOGGER_LEVEL,"\n Tim soundStd = %d\n",(UINT32)soundStd);
    return COsdSoundSystemSwitch(soundStd);

}
/*
void COsdSetSoundMode(UINT8 mode)
{
	switch (mode)
	{
	case _SOUND_DK:
		drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_DK);
		fw_timer_event_DelayXms(300);	// wait Tuner stable
		Audio_AtvSetSoundStd(ATV_SOUND_STD_A2_DK1);
  		break;
	case _SOUND_I:
		drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_I);
		fw_timer_event_DelayXms(300);	// wait Tuner stable
		Audio_AtvSetSoundStd(ATV_SOUND_STD_FM_MONO_NO_I);
  		break;
	case _SOUND_BG:
		drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_BG);
		fw_timer_event_DelayXms(300);	// wait Tuner stable
		Audio_AtvSetSoundStd(ATV_SOUND_STD_A2_BG);
  		break;
	case _SOUND_MN:
		drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_MN);
		Audio_AtvSetSoundStd(ATV_SOUND_STD_A2_M);
  		break;
	case _SOUND_L:
		drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_L);
		fw_timer_event_DelayXms(300);	// wait Tuner stable
		Audio_AtvSetSoundStd(ATV_SOUND_STD_NICAM_L);
		break;

	default:
		Audio_AtvSetSoundStd(ATV_SOUND_STD_A2_DK1);
  		break;
	}
}
*/

void COsdCtrlSetSoundSystem(UINT8 ucMode)
{
    UINT8 resetflag =0;
	UINT8 CurChn;

	CurChn = GET_TV_CHN_DATA(_CUR_CHN_TV);
	
	
    Audio_HwpSetMute(_ENABLE, ADC_CH_ALL, ADC_SYS_MUTE);
    fw_timer_event_DelayXms(60);	//sms 2008-04-01
    //RTD_Log(LOGGER_LEVEL,"\nTim ucMode=%d \n",(UINT32)ucMode);
    //Mark by frank fw_timer_event_DelayXms(500);		//delay 500ms
    //ucMode = CConverModeProc(ucMode);

    switch (ucMode)
    {
    case _SOUND_DK:
        drvif_tuner_init(_SIG_MODE_PAL_DK);
        TVSetAudioStandard(ATV_SOUND_STD_A2_DK1);
        SET_CHANNEL_SOUNDSYSTEM(CurChn, ATV_SOUND_STD_A2_DK1);
        break;
    case _SOUND_I:
        drvif_tuner_init(_SIG_MODE_PAL_I);
        TVSetAudioStandard(ATV_SOUND_STD_FM_MONO_NO_I);
        SET_CHANNEL_SOUNDSYSTEM(CurChn, ATV_SOUND_STD_FM_MONO_NO_I);
        break;
    case _SOUND_BG:
        drvif_tuner_init(_SIG_MODE_PAL_BG);
        TVSetAudioStandard(ATV_SOUND_STD_A2_BG);
        SET_CHANNEL_SOUNDSYSTEM(CurChn, ATV_SOUND_STD_A2_BG);
        break;
    case _SOUND_N:
	    drvif_tuner_init(_SIG_MODE_PAL);
	    TVSetAudioStandard(ATV_SOUND_STD_MN_MONO);
	    SET_CHANNEL_SOUNDSYSTEM(CurChn, ATV_SOUND_STD_PAL_N);
	    break;

    case _SOUND_MN:
        drvif_tuner_init(_SIG_MODE_NTSC);
        TVSetAudioStandard(ATV_SOUND_STD_MN_MONO);
        SET_CHANNEL_SOUNDSYSTEM(CurChn, ATV_SOUND_STD_A2_M);
        break;
    case _SOUND_L:
        drvif_tuner_init(_SIG_MODE_SECAM_L);
        TVSetAudioStandard(ATV_SOUND_STD_NICAM_L);
        SET_CHANNEL_SOUNDSYSTEM(CurChn, ATV_SOUND_STD_NICAM_L);

        drv_tuner_SetFreq(tvScanCurFreq);
        break;
    case _SOUND_AUTO:
    default:
        if (COsdCtrlGetTVVideoStd() == _SIG_MODE_SECAM)
        {
            drvif_tuner_init(_SIG_MODE_PAL);
            fw_timer_event_DelayXms(100);
            if (!drv_video_vdc_CheckHVisLocked())
                drvif_tuner_init(_SIG_MODE_SECAM_L);
        }
        else
            drvif_tuner_init(COsdCtrlGetTVVideoStd());
        //ucMode = COsdSoundSystemSwitch(CTvScanGetAudioStd(vd_std));
        TVSetAudioStandard(ATV_SOUND_STD_AUTO);
        SET_CHANNEL_SOUNDSYSTEM(CurChn, ATV_SOUND_STD_AUTO);

        break;
    }

    /*-------------------
    when soundsystem is _SOUND_MN and colorsystem is pal_i,correct color_sytem to pal m,
    prevent set  error videostd
       tommy modify20100407
       -------------------*/
    switch (ucMode)
    {
    case _SOUND_DK:
    case _SOUND_I:
    case _SOUND_BG:
        if (GET_CHANNEL_COLORSYSTEM(CurChn)==ZPAL_M)
        {
            SET_CHANNEL_COLORSYSTEM(CurChn, ZPAL_I);
            RTD_Log(LOGGER_LEVEL,"\n Tim bg9 \n");
            drv_scalerdisplay_set_bg_color(0,0,0);
            drv_scalerdisplay_ForceBackground(_TRUE);
            fw_tv_setvideomode(ZPAL_I,0);
            resetflag =1;

#ifdef CONFIG_TUNER_SILICON
            //tommy modify20100702 ,reset set freq and init silicon tuner
            drv_tuner_initRT810(_SIG_MODE_PAL);
            drv_tuner_SetFreq(tvScanCurFreq);
#endif
        }
        else
            resetflag=0;
        break;
        
	case _SOUND_N:
        if (1)//GET_CHANNEL_COLORSYSTEM(CurChn)==ZPAL_M)
        {
            SET_CHANNEL_COLORSYSTEM(CurChn, ZPAL_N);
            RTD_Log(LOGGER_LEVEL,"\n Tim bg101 \n");
            drv_scalerdisplay_set_bg_color(0,0,0);
            drv_scalerdisplay_ForceBackground(_TRUE);
            fw_tv_setvideomode(ZPAL_N,0);
            resetflag =1;

		#ifdef CONFIG_TUNER_SILICON
            //tommy modify20100702 ,reset set freq and init silicon tuner
            drv_tuner_initRT810(_SIG_MODE_NTSC);
            drv_tuner_SetFreq(tvScanCurFreq);
		#endif
        }
        else
            resetflag=0;

		break;
		
    case _SOUND_MN:
        if ((GET_CHANNEL_COLORSYSTEM(CurChn)==ZPAL_N)||(GET_CHANNEL_COLORSYSTEM(CurChn)==ZPAL_I))
        {
            SET_CHANNEL_COLORSYSTEM(CurChn, ZPAL_M);
            RTD_Log(LOGGER_LEVEL,"\n Tim bg103 \n");
            drv_scalerdisplay_set_bg_color(0,0,0);
            drv_scalerdisplay_ForceBackground(_TRUE);
            fw_tv_setvideomode(ZPAL_M,0);
            resetflag =1;

			#ifdef CONFIG_TUNER_SILICON
            //tommy modify20100702 ,reset set freq and init silicon tuner
            drv_tuner_initRT810(_SIG_MODE_NTSC);
            drv_tuner_SetFreq(tvScanCurFreq);
			#endif
        }
        else	if (GET_CHANNEL_COLORSYSTEM(CurChn)==ZNTSC)
        {
            SET_CHANNEL_COLORSYSTEM(CurChn, ZNTSC);
            RTD_Log(LOGGER_LEVEL,"\n Tim bg102 \n");
            drv_scalerdisplay_set_bg_color(0,0,0);
            drv_scalerdisplay_ForceBackground(_TRUE);
            fw_tv_setvideomode(ZNTSC,0);
            resetflag =1;

			#ifdef CONFIG_TUNER_SILICON
            //tommy modify20100702 ,reset set freq and init silicon tuner
            drv_tuner_initRT810(_SIG_MODE_NTSC);
            drv_tuner_SetFreq(tvScanCurFreq);
			#endif
        }
        else
            resetflag=0;
        break;
    default:
        resetflag=0;
        break;
    }

    //RTK00190 leo 20071106 open audio when no mute
    if (!GET_AUDIO_MUTE())
    {
        if (CCheckTVChannelLock())	//sms 2008-04-11
            Audio_HwpSetMute(_DISABLE, ADC_CH_ALL, ADC_SYS_MUTE);
    }

    if (resetflag)
    {
        disp_info.state = _MODE_STATE_SEARCH;
        //drv_scalerdisplay_set_bg_color(0,0,0);
        //drv_scalerdisplay_ForceBackground(_TRUE);
        flow_mode_DisableChannelProc();
        COsdFxDisableOsd(); // Prevent OSD flick
        flow_audio_Setmute(ADC_MUTE, SCALER, (ADC_CH_IDX)(ADC_CH_L|ADC_CH_R));
    }

}
//Tim 0323
//==============================================================================
UINT8 COsdCtrlGetTVNtscStd(void)  //jacklong 2007/09/20 modify
{
    UINT8 ntscStd;
    
    //ntscStd = GET_CHANNEL_NTSCSYSTEM(GET_TV_CUR_NtscType());
    ntscStd = GET_TV_CUR_NtscType();
    return ntscStd;
    
}
//******************************************************************************
void COsdCtrlSetTVNtscStd(UINT8 ucMode)  //jacklong 2007/09/20 modify
{	
    UINT8 CurChn;

    SET_TV_CUR_NtscType(ucMode);
//Tim 0404
	CurChn = GET_TV_CHN_DATA(_CUR_CHN_TV);
	
    CTvSetChannel(CurChn);
    //CLoadTableProc();
}
//******************************************************************************
UINT8 COsdCtrlGetNtscFree(void)
{
    if (GET_TV_CUR_FreeType()== 1)
    {
        return 1;
    }
    else
        return 0;

}
//******************************************************************************
void  COsdCtrlSetNtscFree(void)
{
    UINT8  val;
    UINT8 CurChn;
    val = ucCtrlArg0;
    SET_TV_CUR_FreeType(val);

	CurChn = GET_TV_CHN_DATA(_CUR_CHN_TV);
	disp_info.state = _MODE_STATE_SEARCH;
	flow_mode_DisableChannelProc();
	CTvSetChannel(CurChn);

}

//==============================================================================

/* ** Channel Frequency Fine Tune Control functions */
UINT8 COsdCtrlGetTVFineTune()
{
    UINT8 ucOffset;
    UINT8 curChn;
	
	curChn = GET_TV_CHN_DATA(_CUR_CHN_TV);
    ucOffset = GET_CHANNEL_FREQ_OFFSET(curChn);
    return ucOffset;
}
/*UINT32 GetMinTVFreq(void)
{
    return drv_tuner_GetChanelMinFreq();
}*/
void COsdCtrlSetTVFineTune(UINT8 ucMode)
{
    UINT32 freq/*,step*/;//For Keil Building
    UINT8 offset;
    UINT8 curChn;
    UINT32 ucCurFreq;

    ucMode = ucMode;//For Keil Building
    offset = ucCtrlArg0;
    if ( offset > 48)
        offset = 48;
    curChn = GET_TV_CUR_CHN();
#if 1
    	SET_CHANNEL_FREQ_OFFSET(curChn, offset);
	freq = GET_CHANNEL_FREQ_OFFSET(curChn) + (UINT32)offset * drv_tuner_GetRatioInStepSize() - 24UL * drv_tuner_GetRatioInStepSize();
	ucCurFreq = GET_CHANNEL_FREQ(curChn);
	RTD_Log(LOGGER_INFO," New Freq = %d \n",(UINT32)freq);
	RTD_Log(LOGGER_INFO," ucCurFreq = %d \n",(UINT32)ucCurFreq);
	 freq= freq + ucCurFreq;
	RTD_Log(LOGGER_INFO," New Freq 111= %d \n",(UINT32)freq);
#endif
    WaitFor_DEN_STOP();//hardy20091208
    WaitFor_DEN_STOP();//hardy20091208

    drv_tuner_SetFreq(freq);


    if (drv_video_vdc_CheckHVisLocked() == _TRUE)
    {
        if ( !GET_AUDIO_MUTE() )
        {	// unmute audio
            if ( GET_BALANCE() == 0)
                Audio_HwpSetMute(_ENABLE, ADC_CH_R, ADC_SYS_MUTE);
            else if ( GET_BALANCE() == 20)
                Audio_HwpSetMute(_ENABLE, ADC_CH_L, ADC_SYS_MUTE);
            else
                Audio_HwpSetMute(_DISABLE, ADC_CH_ALL, ADC_SYS_MUTE);
            fw_timer_event_DelayXms(20);	//sms 2008-01-09
            AUDIO_AMP_MUTE_ENABLE(AUDIO_AMP_UNMUTE);	// CF 20071023 added, mute AMP
        }
    }

}
#endif


/* =========================================================================== */
/**
 * RTK factory mode control functions.
 */
#define _RTK_FACTORY_MODE_CONTROL_FUNCTIONS_

/** DLTi */
UINT8 COsdCtrlGetDLti()
{
    return GET_DLTI();
}

void COsdCtrlSetDLti(UINT8 message)
{
    SET_DLTI(message);
//->w	RTD_Log(LOGGER_INFO, "VIP: COsdCtrlSetDLti=%d\n", (UINT32)message);
    // call PQ D-Domain DLTi API here
    drv_color_setydlti(VIP_D_DOMAIN, message);
}

/** DCTi */
UINT8 COsdCtrlGetDCti()
{
    return GET_DCTI();//osd show backlight range 0~10
}

void COsdCtrlSetDCti(UINT8 message)
{
    SET_DCTI(message);
//->w	RTD_Log(LOGGER_INFO, "VIP: COsdCtrlSetDCti=%d\n", (UINT32)message);
    // call PQ D-Domain DCti API here
    drv_color_setcdcti(VIP_D_DOMAIN, message);
}

/** I-Domain DLTi */
UINT8 COsdCtrlGetIDomainDLti(void)
{
    return GET_I_DLTI();
}

void COsdCtrlSetIDomainDLti(UINT8 message)
{
    SET_I_DLTI(message);
//->w	RTD_Log(LOGGER_INFO, "VIP: COsdCtrlSetIDomainDLti=%d\n", (UINT32)message);
    // call PQ I-Domain DLTI API here
    drv_color_setydlti(VIP_I_DOMAIN, message);
}

/** I-Domain DCTi */
UINT8 COsdCtrlGetIDomainDCti(void)
{
    return GET_I_DCTI();
}

void COsdCtrlSetIDomainDCti(UINT8 message)
{
    SET_I_DCTI(message);
//->w	RTD_Log(LOGGER_INFO, "VIP: COsdCtrlSetIDomainDCti=%d\n", (UINT32)message);
    // call PQ I-Domain DCTI API here
    drv_color_setcdcti(VIP_I_DOMAIN, message);
}

extern UINT8 g_power_flag;
void COsdCtrlSetStorageValue(void)
{
//->w	RTD_Log(LOGGER_INFO, "VIP: GET_CONTRAST()=%d, GET_BRIGHTNESS()=%d\n",(UINT32)GET_CONTRAST(),(UINT32)GET_BRIGHTNESS());
//->w	RTD_Log(LOGGER_INFO, "VIP: GET_HUE()=%d, GET_SATURATION()=%d\n",(UINT32)GET_HUE(),(UINT32)GET_SATURATION());
//->w	RTD_Log(LOGGER_INFO, "VIP: GET_COLOR_BACKLIGHT()=%d, GET_SHARPNESS()=%d, GET_DNR()=%d\n",(UINT32)GET_COLOR_BACKLIGHT(),(UINT32)GET_SHARPNESS(), (UINT32)GET_DNR());
    UINT8 new_sat=0;
	UINT8 Dimmertemp=0;

    g_ucUserBacklight = GET_COLOR_BACKLIGHT();//for the use of DCR, DCR func. is always called in active state! (need backlight level)

    COsdCtrlColorTemp();//reload color temp setting
   if( g_power_flag)
   	{
    	LOAD_STORAGE_DATA(StructSystemDataType,  DimmerVaule, Dimmertemp);
   	    SET_BRIGHTNESS(Dimmertemp);
		g_power_flag=0;
   	}
    COsdCtrlSetYCB();

    //RTD_Log(LOGGER_INFO, "Joker flag 333333333333333333");
    if(GET_INPUTSOURCE_INDEX() == _SOURCE_INDEX_HDMI0)
    	COsdCtrlSetAudioRemapVolume(GET_VOLUME()*5);
    else
    	COsdCtrlSetAudioRemapVolume(GET_VOLUME()*5);

    if (GET_INPUTSOURCE_FROM()==_SRC_FROM_VDC)
    {
        if (GET_CURRENT_MODE() == _MODE_480I)
            drv_color_AdjustHue(GET_HUE());
        else
            drv_color_AdjustHue(50);
    }
    else
        drv_color_AdjustHue(GET_HUE());

    new_sat = COsd_remapping( GET_SATURATION(), 60);
    drv_color_AdjustSaturation(new_sat);
    COsdCtrlDNR();
    drvif_color_peaking_config(flow_vip_Get_Input_Mode(), GET_SHARPNESS());

#ifdef CONFIG_SCART_AUTO_SWITCH
    if (GET_INPUTSOURCE_TYPE() == _SOURCE_FULL_SCART)
    {
        ui_get_SCART_ADCGain(&stScartGainOffsetData);
        drv_video_SCART_ADC_adjust_gain(_SCART_R, stScartGainOffsetData.Gain_R);
        drv_video_SCART_ADC_adjust_gain(_SCART_G, stScartGainOffsetData.Gain_G);
        drv_video_SCART_ADC_adjust_gain(_SCART_B, stScartGainOffsetData.Gain_B);
    }
#endif

    //COsdCtrlSetOsdBlend(GET_OSD_BLENDING());
//	drvif_color_peaking_config(GET_INPUTSOURCE_TYPE(), GET_SHARPNESS());    //    drv_color_PeakingFilter(GET_SHARPNESS()); //monitor usage
}


/** Gamma */
UINT8 COsdCtrlGetGamma()
{
    return GET_GAMMA();
}

void COsdCtrlSetGamma(void)
{
    SET_GAMMA(ucCtrlArg0);
//->w	RTD_Log(LOGGER_INFO, "VIP: COsdCtrlSetGamma=%d\n", (UINT32)ucCtrlArg0);
    // call PQ Gamma setting API
    drv_color_setgamma_s(ucCtrlArg0, 0);
}

UINT8 COsdCtrlGetSIndex()
{
    return GET_S_INDEX();
}

void COsdCtrlSetSIndex(void)
{

    static UINT8 buffer_index=0; // set for step 8
    if (ucCtrlArg0 > buffer_index)
    {
        if (ucCtrlArg0 <= 248)
        {
            ucCtrlArg0+=7;
        }
        else
        {
            ucCtrlArg0=255;
        }
    }
    else if (ucCtrlArg0 < buffer_index)
    {
        if (ucCtrlArg0 >= 7)
        {
            ucCtrlArg0-=7;
        }
        else
        {
            ucCtrlArg0=0;
        }
    }
    buffer_index = ucCtrlArg0;

    SET_S_INDEX(ucCtrlArg0);
//->w	RTD_Log(LOGGER_INFO, "VIP: COsdCtrlSetGamma=%d\n", (UINT32)ucCtrlArg0);
    // call PQ Gamma setting API
    drv_color_setgamma_s(GET_GAMMA(), 1);
}

UINT8 COsdCtrlGetSLow()
{
    return GET_S_LOW();
}

void COsdCtrlSetSLow(void)
{
    SET_S_LOW(ucCtrlArg0);
//->w	RTD_Log(LOGGER_INFO, "VIP: COsdCtrlSetGamma=%d\n", (UINT32)ucCtrlArg0);
    // call PQ Gamma setting API
    drv_color_setgamma_s(GET_GAMMA(), 1);
}

UINT8 COsdCtrlGetSHigh()
{
    return GET_S_HIGH();
}

void COsdCtrlSetSHigh(void)
{
    SET_S_HIGH(ucCtrlArg0);
//->w	RTD_Log(LOGGER_INFO, "VIP: COsdCtrlSetGamma=%d\n", (UINT32)ucCtrlArg0);
    // call PQ Gamma setting API
    drv_color_setgamma_s(GET_GAMMA(), 1);
}

/** ScaleUP H */
UINT8 COsdCtrlGetScaleUPH()
{
    return GET_SCALEUPH();
}

void COsdCtrlSetScaleUPH(UINT8 message)
{
    SET_SCALEUPH(message);
//->w	RTD_Log(LOGGER_INFO, "VIP: COsdCtrlSetScaleUPH=%d\n", (UINT32)message);
    // call PQ scale up hcoef API
    drvif_color_scaler_setscaleup_hcoef(message);
}

/** ScaleUP V */
UINT8 COsdCtrlGetScaleUPV()
{
    return GET_SCALEUPV();
}

void COsdCtrlSetScaleUPV(UINT8 message)
{
    SET_SCALEUPV(message);
//->w	RTD_Log(LOGGER_INFO, "VIP: COsdCtrlSetScaleUPV=%d\n", (UINT32)message);
    drvif_color_scaler_setscaleup_vcoef(message);
}

/** Scale PK */
UINT8 COsdCtrlGetScalePK()
{
    return GET_SCALEPK();
}

void COsdCtrlSetScalePK(UINT8 message)
{
    SET_SCALEPK(message);
//->w	RTD_Log(LOGGER_INFO, "VIP: COsdCtrlSetScalePK=%d\n", (UINT32)message);
    drvif_color_scaler_setds_pk(message);
}

#if 0
/** Peaking */
UINT8 COsdCtrlGetPeaking()
{
    return GET_PEAKING();
}

void COsdCtrlSetPeaking(UINT8 message)
{
    SET_PEAKING(message);
    drvif_color_peaking_config(_MAIN_DISPLAY, message);
}
#endif

/** Digital Filter */
UINT8 COsdCtrlGet_DigitalFilter(void)
{
    return GET_DIGI_FILTER()%2;
}

void COsdCtrlSet_DigitalFilter(void)
{
    SET_DIGI_FILTER(ucCtrlArg0);
//->w	RTD_Log(LOGGER_INFO, "VIP: COsdCtrlSet_DigitalFilter=%d\n", (UINT32)ucCtrlArg0);
    // call PQ digital filter API here
    drvif_color_digital_filter_config(ucCtrlArg0);
}


#if 0
/** NR Spatial */
UINT8 COsdCtrlGetNRSpatial()
{
    return GET_NRSPATIAL();
}
void COsdCtrlSetNRSpatial(UINT8 message)
{
    SET_NRSPATIAL(message);

    // call PQ SUNR API here
    //NoiseReduction_Spatial(pInfoOsdSrc->channel, message, GET_VUZD_BUFFER_NR_MODE(pInfoOsdSrc->display));
}
#endif

/** SUNR **/
UINT8 COsdCtrlGetSUNR_Y(void)
{
    return GET_SUNR_Y();
}

void COsdCtrlSetSUNR_Y(UINT8 ucValue)
{
    SET_SUNR_Y(ucValue);
//->w	RTD_Log(LOGGER_INFO, "VIP: COsdCtrlSetSUNR_Y=%d\n", (UINT32)ucValue);
    // call PQ SUNR Y API here
    drvif_color_sunr_y( ucValue);
}

UINT8 COsdCtrlGetSUNR_C(void)
{
    return GET_SUNR_C();
}

void COsdCtrlSetSUNR_C(UINT8 ucValue)
{
    SET_SUNR_C(ucValue);
//->w	RTD_Log(LOGGER_INFO, "VIP: COsdCtrlSetSUNR_C=%d\n", (UINT32)ucValue);
    // call PQ SUNR C API here
    drvif_color_sunr_c( ucValue);
}


/** DCC */
UINT8 COsdCtrlGetDcc(void)
{
    return GET_DCC_MODE();
}

void COsdCtrlSetDcc(UINT8 message)
{
    SET_DCC_MODE(message);
//->w	RTD_Log(LOGGER_INFO, "VIP: COsdCtrlSetDcc=%d\n", (UINT32)message);
    // call PQ DCC API here
    //DCC_setLevel(pInfoOsdSrc->input_src, message);
    if ( message )
    {
        drv_color_FillDCCTable( message-1 );
        drv_color_EnableDCC( 1, _ENABLE);
    }
    else
        drv_color_EnableDCC( 1, _DISABLE);
}

/** ICM */
UINT8 COsdCtrlGetFacICM(void)
{
    //return ucFacICM;
    return GET_ICM_TABLE();
}

void COsdCtrlSetFacICM(void)
{
    SET_ICM_TABLE(ucCtrlArg0);
//->w	RTD_Log(LOGGER_INFO, "VIP: COsdCtrlSetFacICM=%d\n", (UINT32)ucCtrlArg0);
    // call PQ ICM setting API here
    drvif_color_icm_table_select( ucCtrlArg0 );
}

/** DCR */
UINT8 COsdCtrlGetFacDCR(void)
{
    return GET_FAC_DCR();
}

void COsdCtrlSetFacDCR(void)
{
    SET_FAC_DCR(ucCtrlArg0);

    // call PQ DCR API here
}

/** DNR */
void COsdCtrlDNR(void)
{
    UINT8 OSD_Level = 0;
    UINT8 spatial_y = 0;
    UINT8 spatial_c = 0;

    // need to modify in future
    if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
        return;

    OSD_Level = GET_DNR();

    switch (OSD_Level)
    {
    case 0:// off
        break;

    case 1:// low
        spatial_y = 1;
        spatial_c = 1;
        break;

    case 2:// Mid
        spatial_y = 3;
        spatial_c = 3;
        break;

    case 3:// High
        spatial_y = 10;
        spatial_c = 10;
        break;

    default:
        spatial_y = 1;
        spatial_c = 1;
        break;
    }

//->w	RTD_Log(LOGGER_INFO, "VIP: DNR-Level=%d, spatial_y=%d, spatial_c=%d\n", (UINT32)GET_DNR(), (UINT32)spatial_y, (UINT32)spatial_c);

    //SET_USER_NRSPATIAL(pInfoOsdSrc->input_src, spatial);
    drvif_color_sunr_middleware( flow_vip_Get_Input_Mode(), &spatial_y, &spatial_c);
    drvif_color_sunr_y(spatial_y);
    drvif_color_sunr_c(spatial_c);

}

UINT8 COsdCtrlGetDNR(void)
{
    return GET_DNR();
}

void COsdCtrlSetDNR(void)
{
    SET_DNR(ucCtrlArg0);
    COsdCtrlDNR();
}

UINT8 COsdCtrlGetDCR(void)
{
    return GET_DCR();
}

void COsdCtrlSetDCR(void)
{
    SET_DCR(ucCtrlArg0);

    if ( ucCtrlArg0 == _ON)
        SET_DCR_STATUS();
    else
        CLR_DCR_STATUS();

    // call PQ DCR API here
}

UINT8 COsdCtrlGetSUPK_Mask(void)
{
    return GET_SUPK_MASK();
}

void COsdCtrlSetSUPK_Mask(void)
{
    SET_SUPK_MASK(ucCtrlArg0);
//->w	RTD_Log(LOGGER_INFO, "VIP: COsdCtrlSetSUPK_Mask=%d\n", (UINT32)ucCtrlArg0);
    // call PQ SUPK Mask API here
    drvif_color_supk_mask( ucCtrlArg0);
}


/** PQ Dither Bit */
UINT8 COsdCtrlGetDitherBit(void)
{
    return GET_DITHER_BIT()%3;
}

void COsdCtrlSetDitherBit(void)
{
    SET_DITHER_BIT(ucCtrlArg0);
    // call PQ Dither Bit setting API here
    drv_color_setdithering( ucCtrlArg0 );//0: 6bit, 1:8bit, 2:10bit
}


UINT8 COsdCtrlGetPQDemo(void)
{
    return GET_PQ_DEMO()%2;
}

void COsdCtrlSetPQDemo(void)
{
    SET_PQ_DEMO(ucCtrlArg0);

    // call PQ demo API here

    drvif_color_peaking_resolution(ucCtrlArg0);
}


/** Factory Reset */
void COsdCtrlReset(UINT8 message)
{
    if (message == 0)
    {
        COsdFxDisableOsd();
        return;
    }
    // when user press osd's reset, cause the whole system reset.
    // Some customer may request to keep some data when OSD reset,
    // so, we save these data to DDR, and when boot, load them to
    // structure for restore to flash!

//	flow_storage_LoadDefault(STORAGE_STORE_SYSTEM);
    //fw_flow_message_push(_MODE_MSG_MAIN_SET_VGIP_AFTER_FOUND_MODE_SIGNAL);
    CLR_FACTORY_MODE_MENU();
    CLR_RTK_FACTORY_MODE();
    COsdCoreExitOsd();
    flow_audio_Setmute(_ENABLE, SCALER, (ADC_CH_IDX)(ADC_CH_L|ADC_CH_R|ADC_CH_C|ADC_CH_SW));
#if 1
    flow_storage_LoadDefault(STORAGE_STORE_SYSTEM);
    SYNC_STORAGE_DEFAULT_DATA(StructFacModeDataType);
    SYNC_STORAGE_DEFAULT_DATA(StructCustomerFacDataType);
    SYNC_STORAGE_DEFAULT_DATA(ADCGainOffset);
    SYNC_STORAGE_DEFAULT_DATA(ADCGainOffset1);
    SYNC_STORAGE_DEFAULT_DATA(ADCGainOffset2),

#ifdef _VIDEO_TV_SUPPORT
    //if(GET_INPUTSOURCE_TYPE() == _SOURCE_TV)
    //{
    SET_TV_CUR_CHN(1);
    SET_TV_PRE_CHN(1);
    //}
#endif
#else
    SYNC_STORAGE_DEFAULT_DATA(StructOsdUserDataType);
    SYNC_STORAGE_DEFAULT_DATA(StructAudioDataType);
    SYNC_STORAGE_DEFAULT_DATA(StructColorDataType);
    SYNC_STORAGE_DEFAULT_DATA(StructSystemDataType);
    SYNC_STORAGE_DEFAULT_DATA(StructTvDataType);
    SYNC_STORAGE_DEFAULT_DATA(StructVGAScreenModeInfo);
    SYNC_STORAGE_DEFAULT_DATA(StructUserDataChannelSettingsType);
    SYNC_STORAGE_DEFAULT_DATA(StructTVChannelMapTable);
    SYNC_STORAGE_DEFAULT_DATA(StructFacModeDataType);
    SYNC_STORAGE_DEFAULT_DATA(StructCustomerFacDataType),
    //SYNC_STORAGE_DEFAULT_DATA(ADCGainOffset),
    //SYNC_STORAGE_DEFAULT_DATA(ADCGainOffset1),
    //SYNC_STORAGE_DEFAULT_DATA(RGB_ColorTemp),
    //SYNC_STORAGE_DEFAULT_DATA(ADCGainOffset2),
#endif

    fw_osdcore_Initialze(); // osd reset
    ap_osdctrl_audio_setting_init(); // audio reset
    fw_flow_message_push(_UI2FL_MAIN_INIT_SRC_EVENT);

}

#if 0		// not used and marked by weihao
/** UART setting */
UINT8 Uart_Set(UINT32  baudrate)
{// 20080104 charlie
    return baudrate;
#if !defined(_DISABLE_OSD_CTRL_FUNC_)
    bool bSucceed=0;
    UINT32 tmp;

    bSucceed = serial_config_port(baudrate); //cchh

//tdl for CLE
//UART0 IE
    tmp = rtd_inl(CNL_UART + 0x04);
    tmp |= 0x01;
    rtd_outl(CNL_UART + 0x04, tmp);

    rtdf_maskl(SC_GIR15, 0xffff0000, _BIT5);//UART0 select IRQ5
    rtd_outl(SC_GIE0, rtd_inl(SC_GIE0) | (1<<0));//Enable UART0 Interrpt

    return bSucceed;
#endif
}

void COsdCtrlSetUart(UINT8 message)
{
    message = 0;
#if !defined(_DISABLE_OSD_CTRL_FUNC_)
    UINT8 Comb=GET_FILTER_TYPE();

    if (message == _RIGHT_KEY_MESSAGE)
        Comb = (Comb + 1)%2;
    else if (message == _LEFT_KEY_MESSAGE)
    {
        if (Comb==0)
            Comb = 1;
        else
            Comb = 0;
    }
    SET_FILTER_TYPE(Comb);
    switch (Comb)
    {
    case 0:
        // Tim modified , serial_config_port(115200);
        Uart_Set(115200);
        break;
    case 1:
        // Tim modified , serial_config_port(9600);
        Uart_Set(9600);
        break;
    }
#endif
}

UINT8 COsdCtrlGetUart(void)
{
    return 0;
    //return GET_FILTER_TYPE();
}
#endif

/** Ypbpr phase */
UINT8 COsdCtrlGetYPbPrPhase(void)
{
    UINT32 regVal = 0;

    regVal = ((UINT32)rtdf_inl(PLLPHASE_CTRL_0x050c)) & 0x0000007f;

    if (select_adc_vco_div == 4)
        regVal = regVal>>1;

    return (regVal&0x3f);
}

void COsdCtrlSetYPbPrPhase(UINT8 value)
{
    //rtdf_maskl(PLLPHASE_CTRL_0x050c, 0xffffffc0, (value&0x3f));
    drv_syncproc_YPbPr_SetPhase(value, _TRUE);
}

UINT8 COsdCtrlGetSpreadSpectrum(void)
{
    UINT32 regVal = 0;

    regVal = ((UINT32)rtdf_inl(DCLKSS_0x2f08)) & 0x000000F0;

    regVal = regVal>>4;

    return (regVal&0x0f);
}

void COsdCtrlSetSpreadSpectrum(UINT8 value)
{
    UINT32 regVal = value;
    regVal = regVal <<4;
    regVal |= 0x05;
    rtdf_maskl(DCLKSS_0x2f08,0xFFFFFF01,regVal);
}



/* =========================================================================== */
/**
 * Factory mode 'Setting' menu control functions.
 *
 */
#define _FAC_SETTING_MENU_CONTROL_FUNCTIONS_

void COsdCtrlSetWB_RGain(void)
{
    UINT8 r,g,b;

    SET_COLORTEMP_RGAIN(GET_INPUTSOURCE_TYPE(), _COLOR_TEMP_NORMAL, ucCtrlArg0);
    //Color_SetRgbContrast(pInfoOsdSrc->display, GET_COLORTEMP_RGAIN(NORMAL), GET_COLORTEMP_GGAIN(NORMAL), GET_COLORTEMP_BGAIN(NORMAL), GET_PIP_BLENDING());
    r = GET_COLORTEMP_RGAIN(GET_INPUTSOURCE_TYPE(), _COLOR_TEMP_NORMAL);
    g = GET_COLORTEMP_GGAIN(GET_INPUTSOURCE_TYPE(), _COLOR_TEMP_NORMAL);
    b = GET_COLORTEMP_BGAIN(GET_INPUTSOURCE_TYPE(), _COLOR_TEMP_NORMAL);
    drv_color_setrgbcontrast(r, g, b);
}

UINT8 COsdCtrlGetWB_RGain(void)
{
    return GET_COLORTEMP_RGAIN(GET_INPUTSOURCE_TYPE(), _COLOR_TEMP_NORMAL);
}

void COsdCtrlSetWB_GGain(void)
{
    UINT8 r,g,b;

    SET_COLORTEMP_GGAIN(GET_INPUTSOURCE_TYPE(), _COLOR_TEMP_NORMAL, ucCtrlArg0);
    //Color_SetRgbContrast(pInfoOsdSrc->display, GET_COLORTEMP_RGAIN(NORMAL), GET_COLORTEMP_GGAIN(NORMAL), GET_COLORTEMP_BGAIN(NORMAL), GET_PIP_BLENDING());
    r = GET_COLORTEMP_RGAIN(GET_INPUTSOURCE_TYPE(), _COLOR_TEMP_NORMAL);
    g = GET_COLORTEMP_GGAIN(GET_INPUTSOURCE_TYPE(), _COLOR_TEMP_NORMAL);
    b = GET_COLORTEMP_BGAIN(GET_INPUTSOURCE_TYPE(), _COLOR_TEMP_NORMAL);
    drv_color_setrgbcontrast(r, g, b);
}

UINT8 COsdCtrlGetWB_GGain(void)
{
    return GET_COLORTEMP_GGAIN(GET_INPUTSOURCE_TYPE(), _COLOR_TEMP_NORMAL);
}

void COsdCtrlSetWB_BGain(void)
{
    UINT8 r,g,b;

    SET_COLORTEMP_BGAIN(GET_INPUTSOURCE_TYPE(), _COLOR_TEMP_NORMAL, ucCtrlArg0);
    //Color_SetRgbContrast(pInfoOsdSrc->display, GET_COLORTEMP_RGAIN(NORMAL), GET_COLORTEMP_GGAIN(NORMAL), GET_COLORTEMP_BGAIN(NORMAL), GET_PIP_BLENDING());
    r = GET_COLORTEMP_RGAIN(GET_INPUTSOURCE_TYPE(), _COLOR_TEMP_NORMAL);
    g = GET_COLORTEMP_GGAIN(GET_INPUTSOURCE_TYPE(), _COLOR_TEMP_NORMAL);
    b = GET_COLORTEMP_BGAIN(GET_INPUTSOURCE_TYPE(), _COLOR_TEMP_NORMAL);
    drv_color_setrgbcontrast(r, g, b);
}

UINT8 COsdCtrlGetWB_BGain(void)
{
    return GET_COLORTEMP_BGAIN(GET_INPUTSOURCE_TYPE(), _COLOR_TEMP_NORMAL);
}

void COsdCtrlSetWB_ROffset(void)
{
    SET_COLORTEMP_ROFFSET(GET_INPUTSOURCE_TYPE(), _COLOR_TEMP_NORMAL, ucCtrlArg0);
    //Color_SetRgbBrightness(pInfoOsdSrc->display, GET_COLORTEMP_ROFFSET()<<2, GET_COLORTEMP_GOFFSET()<<2, GET_COLORTEMP_BOFFSET()<<2);
}

UINT8 COsdCtrlGetWB_ROffset(void)
{
    return GET_COLORTEMP_ROFFSET(GET_INPUTSOURCE_TYPE(), _COLOR_TEMP_NORMAL);
}

void COsdCtrlSetWB_GOffset(void)
{
    SET_COLORTEMP_GOFFSET(GET_INPUTSOURCE_TYPE(), _COLOR_TEMP_NORMAL, ucCtrlArg0);
    //Color_SetRgbBrightness(pInfoOsdSrc->display, GET_COLORTEMP_ROFFSET()<<2, GET_COLORTEMP_GOFFSET()<<2, GET_COLORTEMP_BOFFSET()<<2);
}

UINT8 COsdCtrlGetWB_GOffset(void)
{
    return GET_COLORTEMP_GOFFSET(GET_INPUTSOURCE_TYPE(), _COLOR_TEMP_NORMAL);
}

void COsdCtrlSetWB_BOffset(void)
{
    SET_COLORTEMP_BOFFSET(GET_INPUTSOURCE_TYPE(), _COLOR_TEMP_NORMAL, ucCtrlArg0);
    //Color_SetRgbBrightness(pInfoOsdSrc->display, GET_COLORTEMP_ROFFSET()<<2, GET_COLORTEMP_GOFFSET()<<2, GET_COLORTEMP_BOFFSET()<<2);
}

UINT8 COsdCtrlGetWB_BOffset(void)
{
    return GET_COLORTEMP_BOFFSET(GET_INPUTSOURCE_TYPE(), _COLOR_TEMP_NORMAL);
}

void  COsdCtrlSetWBHBrightness(void)
{
    SET_WBH_BRI(ucCtrlArg0);
    COsdCtrlSetMainYBrightness(GET_WBH_BRI());
}

UINT8 COsdCtrlGetWBHBrightness(void)
{
    return GET_WBH_BRI();
}

void COsdCtrlSetWBLBrightness(void)
{
    SET_WBL_BRI(ucCtrlArg0);
    COsdCtrlSetMainYBrightness(GET_WBL_BRI());
}

UINT8 COsdCtrlGetWBLBrightness(void)
{
    return GET_WBL_BRI();
}

void  COsdCtrlSetWBHContrast(void)
{
    SET_WBH_CONT(ucCtrlArg0);
    COsdCtrlSetRgbContrast(GET_WBH_CONT());
}

UINT8 COsdCtrlGetWBHContrast(void)
{
    return GET_WBH_CONT();
}

void  COsdCtrlSetWBLContrast(void)
{
    SET_WBL_CONT(ucCtrlArg0);
    COsdCtrlSetRgbContrast(GET_WBL_CONT());
}

UINT8 COsdCtrlGetWBLContrast(void)
{
    return GET_WBL_CONT();
}

/**
 * Factory Color Temperature
 */
void COsdCtrlSetFacColorTemp(void)
{
    SET_COLOR_TEMP_TYPE(ucCtrlArg0);
    COsdCtrlColorTemp();
}

UINT8 COsdCtrlGetFacColorTemp(void)
{
    return GET_COLOR_TEMP_TYPE()%5;
}
#if 0
UINT8 COsdCtrlGetWBType(void)
{
    return 0;
#if !defined(_DISABLE_OSD_CTRL_FUNC_)
    switch (COsdCtrlGetSignalSource())
    {
    case _SOURCE_INDEX_TV:
        return _WB_TYPE_CVBS;
        break;
    case _SOURCE_INDEX_AV:
        return _WB_TYPE_CVBS;
        break;
    case _SOURCE_INDEX_AV1:
        return _WB_TYPE_CVBS;
        break;
    case _SOURCE_INDEX_SV:
        return _WB_TYPE_CVBS;
        break;
    case _SOURCE_INDEX_YPbPr:
        return _WB_TYPE_COMP;
        break;
#ifdef CONFIG_PCB_CUSTOMER1_32V68
    case _SOURCE_INDEX_YPbPr1:
        return _WB_TYPE_COMP;
        break;
#endif
    case _SOURCE_INDEX_VGA:
        return _WB_TYPE_VGA;
        break;
    case _SOURCE_INDEX_HDMI:
        return _WB_TYPE_HDMI;
        break;
    case _SOURCE_INDEX_HDMI1:
        return _WB_TYPE_HDMI;
        break;
    default:
        return _WB_TYPE_CVBS;
    }
#endif
}
#endif
void COsdCtrlSetFacColorTempRgain(void)
{
    SET_COLORTEMP_RGAIN(GET_INPUTSOURCE_TYPE(), GET_COLOR_TEMP_TYPE(), ucCtrlArg0);
    COsdCtrlColorTemp();
}

UINT8 COsdCtrlGetFacColorTempRgain(void)
{
    return GET_COLORTEMP_RGAIN(GET_INPUTSOURCE_TYPE(), GET_COLOR_TEMP_TYPE());
}

void COsdCtrlSetFacColorTempGgain(void)
{
    SET_COLORTEMP_GGAIN(GET_INPUTSOURCE_TYPE(), GET_COLOR_TEMP_TYPE(), ucCtrlArg0);
    COsdCtrlColorTemp();
}

UINT8 COsdCtrlGetFacColorTempGgain(void)
{
    return GET_COLORTEMP_GGAIN(GET_INPUTSOURCE_TYPE(), GET_COLOR_TEMP_TYPE());
}

void COsdCtrlSetFacColorTempBgain(void)
{
    SET_COLORTEMP_BGAIN(GET_INPUTSOURCE_TYPE(), GET_COLOR_TEMP_TYPE(), ucCtrlArg0);
    COsdCtrlColorTemp();
}

UINT8 COsdCtrlGetFacColorTempBgain(void)
{
    return GET_COLORTEMP_BGAIN(GET_INPUTSOURCE_TYPE(), GET_COLOR_TEMP_TYPE());
}

/**
 * Factory mode ADC calibration.
 */
void COsdCtrlFacAutoColor(void)
{
    if ( ! (GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR
            || GET_INPUTSOURCE_TYPE() == _SOURCE_VGA
#ifdef CONFIG_SCART_AUTO_SWITCH
            || GET_INPUTSOURCE_TYPE() == _SOURCE_FULL_SCART
#endif
           ))
        return;

    COsdCoreInitAutoAdjustState(INFORM_ADJUSTING);

    if (GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR) // weihao 960308
    {
#ifdef CONFIG_YPBPR_AUTO_COLOR
        drvif_color_ypbpr_adjustment_dragon_bartype(&stYPbPrGainOffsetData, ClrBR100);
//#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
        ui_set_YPbPr_ADCGain(&stYPbPrGainOffsetData);
//#else
//		SYNC_STORAGE_STRUCT_DATA(ADCGainOffset, stYPbPrGainOffsetData);
//#endif

#else
        drv_adc_autobalance();
#endif
    }
    else if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
    {
        drv_adc_autobalance();
    }
#ifdef CONFIG_SCART_AUTO_SWITCH
    else if ( GET_INPUTSOURCE_TYPE() == _SOURCE_FULL_SCART)
    {
        drvif_color_SCARTrgb_adjustment(&stScartGainOffsetData);//march test!!
        ui_set_SCART_ADCGain(&stScartGainOffsetData);

    }
#endif
    fw_ui_message_push(_EXIT_KEY_MESSAGE);
}

#if 0
void COsdCtrlFacAdcCalibration480i(void)
{
    if ( GET_OSD_INPUTSOURCE_TYPE() != _ADC_SRC_YPBPR)
        return;
    if ( pInfoOsdSrc->ucMode_Curr != _MODE_480I)
        return;
    COsdCtrlFacAutoColor();
}

void COsdCtrlFacAdcCalibration480p(void)
{
    if ( GET_OSD_INPUTSOURCE_TYPE() != _ADC_SRC_YPBPR)
        return;
    if ( pInfoOsdSrc->ucMode_Curr != _MODE_480P)
        return;
    COsdCtrlFacAutoColor();
}

void COsdCtrlFacAdcCalibration576i(void)
{
    if ( GET_OSD_INPUTSOURCE_TYPE() != _ADC_SRC_YPBPR)
        return;
    if ( pInfoOsdSrc->ucMode_Curr != _MODE_576I)
        return;
    COsdCtrlFacAutoColor();
}

void COsdCtrlFacAdcCalibration576p(void)
{
    if ( GET_OSD_INPUTSOURCE_TYPE() != _ADC_SRC_YPBPR)
        return;
    if ( pInfoOsdSrc->ucMode_Curr != _MODE_576P)
        return;
    COsdCtrlFacAutoColor();
}

void COsdCtrlFacAdcCalibration720p50(void)
{
    if ( GET_OSD_INPUTSOURCE_TYPE() != _ADC_SRC_YPBPR)
        return;
    if ( pInfoOsdSrc->ucMode_Curr != _MODE_720P50)
        return;
    COsdCtrlFacAutoColor();
}
#endif

void COsdCtrlFacAdcCalibration720p60(void)
{
    if ( GET_INPUTSOURCE_TYPE() != _SOURCE_YPBPR)
        return;
    if ( info->ucMode_Curr != _MODE_720P60)
        return;
    COsdCtrlFacAutoColor();
}

#if 0
void COsdCtrlFacAdcCalibration1080i50(void)
{
    if ( GET_OSD_INPUTSOURCE_TYPE() != _ADC_SRC_YPBPR)
        return;
    if ( pInfoOsdSrc->ucMode_Curr != _MODE_1080I25)
        return;
    COsdCtrlFacAutoColor();
}

void COsdCtrlFacAdcCalibration1080i60(void)
{
    if ( GET_OSD_INPUTSOURCE_TYPE() != _ADC_SRC_YPBPR)
        return;
    if ( pInfoOsdSrc->ucMode_Curr != _MODE_1080I30)
        return;
    COsdCtrlFacAutoColor();
}

void COsdCtrlFacAdcCalibration1080p50(void)
{
    if ( GET_OSD_INPUTSOURCE_TYPE() != _ADC_SRC_YPBPR)
        return;
    if ( pInfoOsdSrc->ucMode_Curr != _MODE_1080P50)
        return;
    COsdCtrlFacAutoColor();
}

void COsdCtrlFacAdcCalibration1080p60(void)
{
    if ( GET_OSD_INPUTSOURCE_TYPE() != _ADC_SRC_YPBPR)
        return;
    if ( pInfoOsdSrc->ucMode_Curr != _MODE_1080P60)
        return;
    COsdCtrlFacAutoColor();
}
#endif

/**
 * Factory mode Options.
 */
void COsdCtrlSetLogo(void)
{
    SET_LOGO_TYPE(ucCtrlArg0);
}

UINT8 COsdCtrlGetLogo(void)
{
    return GET_LOGO_TYPE();
}

void COsdCtrlSetFacLanguage(void)
{
    SET_DEFAULT_LANG(ucCtrlArg0);
}

UINT8 COsdCtrlGetFacLanguage(void)
{
    return GET_DEFAULT_LANG();
}

#if 0
void COsdCtrlSetTOFACValue(UINT8 val)
{
    val = 0;
    UINT8 mode;
    mode =GET_TOFAC_MODE();

    if ( val == _RIGHT_KEY_MESSAGE)
        mode = (mode+1)%2;
    else if ( val == _LEFT_KEY_MESSAGE)
        mode = (mode+1)%2;
    else
        return;

    SET_TOFAC_MODE(mode);
}
#endif

#if 0
UINT8 COsdCtrlGetTOFACValue(void)
{
    return (GET_TOFAC_MODE()%2);
}
#endif

void COsdFactoryClear()
{
    // need to modify
//	flow_storage_LoadDefault(STORAGE_STORE_SYSTEM);
}

void COsdFactoryClearAll()
{
    // need to modify
//	flow_storage_LoadDefault(STORAGE_STORE_SYSTEM);
}

/* =========================================================================== */
/**
 * Factory Mode 'Design' menu control Functions.
 *
 */
#define _FAC_DESIGN_MENU_CONTROL_FUNCTIONS_

/** Picture Mode */
UINT8 COsdCtrlGetFacPictureMode(void)
{
    UINT8 mode = GET_PICTURE_MODE();
    if ( mode > _PIC_MODE_GENTLE)
        COsdCtrlSetPictureMode(_PIC_MODE_VIVID);

    return (GET_PICTURE_MODE()%3);
}

void COsdCtrlSetFacPictureMode(void)
{
    COsdCtrlSetPictureMode(ucCtrlArg0);
}

void COsdCtrlSetPicture1Brightness(void)
{
    SET_PICTURE_MODE_BRI(GET_PICTURE_MODE(), ucCtrlArg0);

    SET_BRIGHTNESS(ucCtrlArg0);
    COsdCtrlSetYCB();
}

UINT8 COsdCtrlGetPicture1Brightness(void)
{
    return GET_PICTURE_MODE_BRI(GET_PICTURE_MODE());
}

void COsdCtrlSetPicture1Contrast(void)
{
    SET_PICTURE_MODE_CONT(GET_PICTURE_MODE(), ucCtrlArg0);

    SET_CONTRAST(ucCtrlArg0);
    COsdCtrlSetYCB();
}

UINT8 COsdCtrlGetPicture1Contrast(void)
{
    return GET_PICTURE_MODE_CONT(GET_PICTURE_MODE());
}

void COsdCtrlSetPicture1Color(void)
{
    SET_PICTURE_MODE_SAT(GET_PICTURE_MODE(), ucCtrlArg0);

    SET_SATURATION(ucCtrlArg0);
    //COsdCtrlSetICMSHI(pInfoOsdSrc->display);
}

UINT8 COsdCtrlGetPicture1Color(void)
{
    return GET_PICTURE_MODE_SAT(GET_PICTURE_MODE());
}

/** Picture Setting */
/**
 * Brightness curve control functions.
 */
void COsdCtrlSetBrightness_0(void)
{
    SET_BRI_0(ucCtrlArg0);
    COsdCtrlSetMainYBrightness(0);
}

UINT8 COsdCtrlGetBrightness_0(void)
{
    return GET_BRI_0();
}

void COsdCtrlSetBrightness_50(void)
{
    SET_BRI_50(ucCtrlArg0);
    COsdCtrlSetMainYBrightness(50);
}

UINT8 COsdCtrlGetBrightness_50(void)
{
    return GET_BRI_50();
}

void COsdCtrlSetBrightness_100(void)
{
    SET_BRI_100(ucCtrlArg0);
    COsdCtrlSetMainYBrightness(100);
}

UINT8 COsdCtrlGetBrightness_100(void)
{
    return GET_BRI_100();
}

/**
 * Contrast curve control functions.
 */
void COsdCtrlSetContrast_0(void)
{
    SET_CONT_0(ucCtrlArg0);
    COsdCtrlSetMainYContrast(0);
}

UINT8 COsdCtrlGetContrast_0(void)
{
    return GET_CONT_0();
}

void COsdCtrlSetContrast_50(void)
{
    SET_CONT_50(ucCtrlArg0);
    COsdCtrlSetMainYContrast(50);
}

UINT8 COsdCtrlGetContrast_50(void)
{
    return GET_CONT_50();
}

void COsdCtrlSetContrast_100(void)
{
    SET_CONT_100(ucCtrlArg0);
    COsdCtrlSetMainYContrast(100);
}

UINT8 COsdCtrlGetContrast_100(void)
{
    return GET_CONT_100();
}

void COsdCtrlSetFacSaturation0(void)
{
    if (ucCtrlArg0 > COsdCtrlGetFacSaturation50())
        return;
    SET_SAT_0(ucCtrlArg0);
    COsdCtrlSetSaturation(0);
}

UINT8 COsdCtrlGetFacSaturation0(void)
{
    return GET_SAT_0();
}

void COsdCtrlSetFacSaturation50(void)
{
    if (ucCtrlArg0 > COsdCtrlGetFacSaturation100() || ucCtrlArg0 < COsdCtrlGetFacSaturation0())
        return;
    SET_SAT_50(ucCtrlArg0);
    COsdCtrlSetSaturation(50);
}

UINT8 COsdCtrlGetFacSaturation50(void)
{
    return GET_SAT_50();
}

void COsdCtrlSetFacSaturation100(void)
{
    if (ucCtrlArg0 > 100 || ucCtrlArg0 < COsdCtrlGetFacSaturation50())
        return;
    SET_SAT_100(ucCtrlArg0);
    COsdCtrlSetSaturation(100);
}

UINT8 COsdCtrlGetFacSaturation100(void)
{
    return GET_SAT_100();
}

/** Music mode */
void COsdCtrlSetFacAudioEQ(void)
{
    SET_EQ_MODE(ucCtrlArg0);
    COsdCoreSetEQ(ucCtrlArg0);
}

UINT8 COsdCtrlGetFacAudioEQ(void)
{
    return (GET_EQ_MODE()%_EQ_MODE_COUNT);
}

void COsdCtrlSetFacSoundMode300Hz(void)
{
    ui_set_eq_300(GET_EQ_MODE(), ucCtrlArg0);
    COsdCoreSetEQ(GET_EQ_MODE());

}

UINT8 COsdCtrlGetFacSoundMode300Hz(void)
{
    return ui_get_eq_300(GET_EQ_MODE());
}

void COsdCtrlSetFacSoundMode1KHz(void)
{
    ui_set_eq_1k(GET_EQ_MODE(), ucCtrlArg0);
    COsdCoreSetEQ(GET_EQ_MODE());
}

UINT8 COsdCtrlGetFacSoundMode1KHz(void)
{
    return ui_get_eq_1k(GET_EQ_MODE());
}

void COsdCtrlSetFacSoundMode2KHz(void)
{
    ui_set_eq_2k(GET_EQ_MODE(), ucCtrlArg0);
    COsdCoreSetEQ(GET_EQ_MODE());
}

UINT8 COsdCtrlGetFacSoundMode2KHz(void)
{
    return ui_get_eq_2k(GET_EQ_MODE());
}

void COsdCtrlSetFacSoundMode3KHz(void)
{
    ui_set_eq_3k(GET_EQ_MODE(), ucCtrlArg0);
    COsdCoreSetEQ(GET_EQ_MODE());
}

UINT8 COsdCtrlGetFacSoundMode3KHz(void)
{
    return ui_get_eq_3k(GET_EQ_MODE());
}

void COsdCtrlSetFacSoundMode8KHz(void)
{
    ui_set_eq_8k(GET_EQ_MODE(), ucCtrlArg0);
    COsdCoreSetEQ(GET_EQ_MODE());

}

UINT8 COsdCtrlGetFacSoundMode8KHz(void)
{
    return ui_get_eq_8k(GET_EQ_MODE());
}

void COsdCtrlSetFacSoundMode15KHz(void)
{
    ui_set_eq_15k(GET_EQ_MODE(), ucCtrlArg0);
    COsdCoreSetEQ(GET_EQ_MODE());

}

UINT8 COsdCtrlGetFacSoundMode15KHz(void)
{
    return ui_get_eq_15k(GET_EQ_MODE());
}

/** Volume setting */
void COsdCtrlSetFacVolume1(UINT8 ucVal)
{
    SET_VOLUME_1(ucVal);
    COsdCtrlSetAudioRemapVolume(1);
}

UINT8 COsdCtrlGetFacVolume1(void)
{
    return GET_VOLUME_1();
}

void COsdCtrlSetFacVolume25(UINT8 ucVal)
{
    SET_VOLUME_25(ucVal);
    COsdCtrlSetAudioRemapVolume(25);
}

UINT8 COsdCtrlGetFacVolume25(void)
{
    return GET_VOLUME_25();
}

void COsdCtrlSetFacVolume50(UINT8 ucVal)
{
    SET_VOLUME_50(ucVal);
    COsdCtrlSetAudioRemapVolume(50);
}

UINT8 COsdCtrlGetFacVolume50(void)
{
    return GET_VOLUME_50();
}

void COsdCtrlSetFacVolume75(UINT8 ucVal)
{
    SET_VOLUME_75(ucVal);
    COsdCtrlSetAudioRemapVolume(75);
}

UINT8 COsdCtrlGetFacVolume75(void)
{
    return GET_VOLUME_75();
}

void COsdCtrlSetFacVolume100(UINT8 ucVal)
{
    SET_VOLUME_100(ucVal);
    COsdCtrlSetAudioRemapVolume(100);
}

UINT8 COsdCtrlGetFacVolume100(void)
{
    return GET_VOLUME_100();
}

void COsdCtrlSetFacTVVolume1(UINT8 ucVal)
{
    SET_TV_VOLUME_1(ucVal);
    COsdCtrlSetAudioRemapVolume(1);
}

UINT8 COsdCtrlGetFacTVVolume1(void)
{
    return GET_TV_VOLUME_1();
}

void COsdCtrlSetFacTVVolume25(UINT8 ucVal)
{
    SET_TV_VOLUME_25(ucVal);
    COsdCtrlSetAudioRemapVolume(25);
}

UINT8 COsdCtrlGetFacTVVolume25(void)
{
    return GET_TV_VOLUME_25();
}

void COsdCtrlSetFacTVVolume50(UINT8 ucVal)
{
    SET_TV_VOLUME_50(ucVal);
    COsdCtrlSetAudioRemapVolume(50);
}

UINT8 COsdCtrlGetFacTVVolume50(void)
{
    return GET_TV_VOLUME_50();
}

void COsdCtrlSetFacTVVolume75(UINT8 ucVal)
{
    SET_TV_VOLUME_75(ucVal);
    COsdCtrlSetAudioRemapVolume(75);
}

UINT8 COsdCtrlGetFacTVVolume75(void)
{
    return GET_TV_VOLUME_75();
}

void COsdCtrlSetFacTVVolume100(UINT8 ucVal)
{
    SET_TV_VOLUME_100(ucVal);
    COsdCtrlSetAudioRemapVolume(100);
}

UINT8 COsdCtrlGetFacTVVolume100(void)
{
    return GET_TV_VOLUME_100();
}

void COsdSetPWM1(UINT8 level)
{
    SET_PANEL_BACKLIGHT( (UINT16)(254*level)/10);
    g_ucUserBacklight = level;
    SET_COLOR_BACKLIGHT(level );
}

UINT8 COsdGetPWM1(void)
{
    return GET_COLOR_BACKLIGHT();
}

void COsdCtrlSetUART(UINT8 ucVal)
{
    if (ucVal == 0)// Uart off, set pinshare to DDC0
    {
        rtd_outb(PB_PS2_0x2d36, 0);
        ucUartPath = 0;
    }
    else // Uart path set to DDC0
    {
        rtd_outb(PB_PS2_0x2d36, 0x55);
        ucUartPath = 1;
    }

    SYNC_STORAGE_DATA(StructSystemDataType,  UartPath, ucUartPath);

}

UINT8 COsdCtrlGetUART(void)
{
    return ucUartPath;
}

void COsdCtrlSetFacEmi_DCLKSS_RANGE(UINT8 ucVal)
{
    UINT32 reg;
    reg=rtd_inl(DCLKSS_0x2f08)&0xFFFFFF0F;//QWS20080729
    reg|= (UINT32)(ucVal<<4)|(_BIT2|_BIT0);//set dclk ss range and enable
    rtd_outl(DCLKSS_0x2f08, reg);
}

UINT8 COsdCtrlGetFacEmi_DCLKSS_RANGE(void)
{
    UINT32 reg;
    UINT8 DCLK_ss_rag;
    reg=rtd_inl(DCLKSS_0x2f08);
    DCLK_ss_rag=(UINT8) (( reg&(_BIT4|_BIT5|_BIT6|_BIT7))>>4);
    return DCLK_ss_rag;
}


void COsdCtrlSetFacEmi_DCLKSS_FMDIV(UINT8 ucVal)
{
    UINT32 reg;
    ucVal&=~(_BIT1|_BIT2|_BIT3|_BIT4|_BIT5|_BIT6|_BIT7);
    reg = rtdf_inl(DCLKSS_0x2f08)&~_BIT3; //QWS20080729
    reg|= (UINT32)(ucVal<<3)|(_BIT2|_BIT0);//set dclk ss range and enable
    rtdf_outl(DCLKSS_0x2f08, reg);
}

UINT8 COsdCtrlGetFacEmi_DCLKSS_FMDIV(void)
{
    UINT32 reg;
    UINT8 DCLK_ss_FMDIV;
    reg=rtd_inl(DCLKSS_0x2f08);
    DCLK_ss_FMDIV=(UINT8) (( reg&_BIT3)>>3);
    return DCLK_ss_FMDIV;

}



#if 0
UINT8 COsdCtrlGetTunerTop(void)
{
    return 0;
#if !defined(_DISABLE_OSD_CTRL_FUNC_)
    if ( GET_FAC_TUNER_TOP() > 31)	//sms 2008-01-04 the bigest is 31
        SET_FAC_TUNER_TOP(31);

    return GET_FAC_TUNER_TOP();
#endif
}

#ifdef CONFIG_TUNER_XUGUANG_TDQ6PD
void CTunerSetTopValue(UINT8 topValue);
#endif
void COsdCtrlSetTunerTop(UINT8 value)
{
    value = 0;
#if !defined(_DISABLE_OSD_CTRL_FUNC_)
    if ( value > 31)	//sms 2008-01-04 the bigest is 31
        value = 31;
    SET_FAC_TUNER_TOP(value);
#ifdef CONFIG_TUNER_XUGUANG_TDQ6PD
    if ( GET_FAC_TUNER_TOP() > 0x1f)
        CTunerSetTopValue(0x1f);
    else
        CTunerSetTopValue(GET_FAC_TUNER_TOP());
#endif
#endif
}
#endif

#ifdef _VIDEO_TV_SUPPORT
UINT8 COsdCtrlGetCurScanProgress(void)
{
    extern UINT8 xdata ucOsdCoreBuf[8];

    return ucOsdCoreBuf[7];
}
#endif

/**
 * To initial audio setting.
 *
 */
void ap_osdctrl_audio_setting_init(void)
{
    //RTD_Log(LOGGER_INFO,"----ap_osdctrl_audio_setting_init--------------\n");
    // apply audio volume
    //RTD_Log(LOGGER_INFO,"COsdCtrlSetAudioVolume------------------------\n");
    ucCtrlArg0 = COsdCtrlGetAudioVolume();
    COsdCtrlSetAudioVolume();

    // apply audio balance
    //RTD_Log(LOGGER_INFO,"COsdCtrlSetAudioBalance-----------------------\n");
    ucCtrlArg0 = COsdCtrlGetAudioBalance();
    COsdCtrlSetAudioBalance();

    // apply audio treble
    //RTD_Log(LOGGER_INFO,"COsdCtrlSetAudioTreble------------------------\n");
    ucCtrlArg0 = COsdCtrlGetAudioTreble();
    COsdCtrlSetAudioTreble();

    // apply audio bass
    //	RTD_Log(LOGGER_INFO,"COsdCtrlSetAudioBass--------------------------\n");
    ucCtrlArg0 = COsdCtrlGetAudioBass();
    COsdCtrlSetAudioBass();

    // apply EQ mode
    // RTD_Log(LOGGER_INFO,"COsdCtrlSetAudioEQ----------------------------\n");
    //COsdCtrlSetAudioEQ(COsdCtrlGetAudioEQ());
    COsdCoreSetEQ(COsdCtrlGetAudioEQ());
    //RTD_Log(LOGGER_INFO,"----------------------------------------------\n");
}










/* =========================================================================== */
#define _CONTROL_HANDLER_
#ifdef _VIDEO_TV_SUPPORT
/**
 * Channel frequency status functions.
 * @return frequency.
 */
UINT8 COsdCtrlGetCurFreq(void)
{
    UINT32 uwFreq;

    uwFreq = tvScanCurFreq - drv_tuner_GetChanelMinFreq();
    uwFreq = uwFreq * 100 / (drv_tuner_GetChanelMaxFreq() - drv_tuner_GetChanelMinFreq())+1;
    return (UINT8)uwFreq;
}

UINT8 COsdCtrlGetMinFreq(void)
{
    return 0;
}

UINT8 COsdCtrlGetMaxFreq(void)
{
    return 100;
}
#endif

/**
 * OSD control function handler.
 * @return none.
 */
void COsdCtrlMenuItemHandler(UINT8 ucMenuItemID, UINT16 ucArg0)
{
    UINT8 temp;
    ucCtrlArg0 = ucArg0;
	//RTD_Log(LOGGER_LEVEL,"\n melon ucMenuItemID=%d,ucArg0 = %d \n",(UINT32)ucMenuItemID,(UINT32)ucArg0);

    switch (ucMenuItemID)
    {
    case MENUITEM_AUDIO_VOLUME:
        COsdCtrlSetAudioVolume();
        break;

    case MENUITEM_AUDIO_BALANCE:
        COsdCtrlSetAudioBalance();
        break;

    case MENUITEM_AUDIO_BASS:
        COsdCtrlSetAudioBass();
        break;

    case MENUITEM_AUDIO_TREBLE:
        COsdCtrlSetAudioTreble();
        break;

    case MENUITEM_AUDIO_SPATIAL:
        COsdCtrlSetAudioSpatial();
        break;

    case MENUITEM_MUSIC_MODE_300HZ:
        temp = MENUITEM_MUSIC_MODE_300HZ-MENUITEM_MUSIC_MODE_300HZ;
        ui_set_eq_300(GET_EQ_MODE(), ucCtrlArg0);
        COsdCtrlSetEQ(temp, ucCtrlArg0);
        break;
    case MENUITEM_MUSIC_MODE_1KHZ:
        temp = MENUITEM_MUSIC_MODE_1KHZ-MENUITEM_MUSIC_MODE_300HZ;
        ui_set_eq_1k(GET_EQ_MODE(), ucCtrlArg0);
        COsdCtrlSetEQ(temp ,ucCtrlArg0);
        break;
    case MENUITEM_MUSIC_MODE_2KHZ:
        temp = MENUITEM_MUSIC_MODE_2KHZ-MENUITEM_MUSIC_MODE_300HZ;
        ui_set_eq_2k(GET_EQ_MODE(), ucCtrlArg0);
        COsdCtrlSetEQ(temp ,ucCtrlArg0);
        break;
    case MENUITEM_MUSIC_MODE_3KHZ:
        temp = MENUITEM_MUSIC_MODE_3KHZ-MENUITEM_MUSIC_MODE_300HZ;
        ui_set_eq_3k(GET_EQ_MODE(),ucCtrlArg0);
        COsdCtrlSetEQ(temp, ucCtrlArg0);
        break;
    case MENUITEM_MUSIC_MODE_8KHZ:
        temp = MENUITEM_MUSIC_MODE_8KHZ-MENUITEM_MUSIC_MODE_300HZ;
        ui_set_eq_8k(GET_EQ_MODE(), ucCtrlArg0);
        COsdCtrlSetEQ(temp, ucCtrlArg0);
        break;
    case MENUITEM_MUSIC_MODE_15KHZ:
        temp = MENUITEM_MUSIC_MODE_15KHZ-MENUITEM_MUSIC_MODE_300HZ;
        ui_set_eq_15k(GET_EQ_MODE(), ucCtrlArg0);
        COsdCtrlSetEQ(temp ,ucCtrlArg0);
        break;

    case MENUITEM_BRIGHTNESS:
        COsdCtrlSetMainYBrightness(ucArg0);
        break;

    case MENUITEM_CONTRAST:
        COsdCtrlSetMainYContrast(ucArg0);
        break;

    case MENUITEM_SATURATION:
        COsdCtrlSetSaturation(ucArg0);
        break;

    case MENUITEM_HUE:
        COsdCtrlSetHue();
        break;

    case MENUITEM_SHARPNESS:
        COsdCtrlSetSharpness1();
        break;

    case MENUITEM_COLOR_TEMPERATURE:
        COsdCtrlSetColorTemp();
        break;

    case MENUITEM_DNR:
        COsdCtrlSetDNR();
        break;

    case MENUITEM_DCR:
        COsdCtrlSetDCR();
        break;

    case MENUITEM_VGA_AUTO_POSITION:
        COsdMenuAutoAdjust();
        break;

    case MENUITEM_VGA_CLOCK:
        COsdCtrlSetClock();
        break;

    case MENUITEM_VGA_PHASE:
        COsdCtrlSetPhase();
        break;

    case MENUITEM_VGA_H_POSITION:
        COsdCtrlSetHPosition();
        break;

    case MENUITEM_VGA_V_POSITION:
        COsdCtrlSetVPosition();
        break;

//	case MENUITEM_VGA_H_SIZE:
//		COsdCtrlSetHSize(ucArg0);
//		break;

//	case MENUITEM_VGA_V_SIZE:
//		COsdCtrlSetVSize(ucArg0);
//		break;
#ifdef _VIDEO_TV_SUPPORT
    case MENUITEM_TV_VIDEO_STD:
        COsdCtrlSetTVVideoStd();
        break;

    case MENUITEM_TV_AUDIO_STD:
    	//ucListSelectedItem
        COsdCtrlSetSoundSystem(ucArg0);
        break;

    case MENUITEM_TV_FREQ_FINE_TUNE:
        COsdCtrlSetTVFineTune(0);//For Keil Building
        break;

//Tim 0323
//==============================================================================
    case MENUITEM_TV_NTSC_STD:
        COsdCtrlSetTVNtscStd(ucArg0);
        break;

	case MENUITEM_NTSC_FREE:
    	COsdCtrlSetNtscFree();
    	break;

//==============================================================================

    case MENUITEM_TV_AUTO_SCAN:
        CLR_TV_TABLE_SCAN();
        CLR_TV_MANUAL_SCAN();
        COsdCtrlChannelScan();
        break;

    case MENUITEM_TV_TABLE_SCAN:
        CLR_TV_MANUAL_SCAN();
        SET_TV_TABLE_SCAN();
        COsdCtrlChannelScan();
        break;

    case MENUITEM_TV_MANUAL_SCAN:
        SET_TV_MANUAL_SCAN();
        COsdCtrlChannelScan();
        break;

    case MENUITEM_TV_COUNTRY:
        COsdCtrlSetCountrySelect();
        break;
    case MENUITEM_CHANNEL_SKIP:
        COsdCtrlSetChanelSkip();
        break;
    case MENUITEM_AFC:
        COsdCtrlSetChanelAFC();
        break;
//	case MENUITEM_TV_BLUE_SCREEN:
//		COsdCtrlSetTVBlueScreen();
//		break;
	#if defined(_REALTEK_OSD_USE)
    case MENUITEM_TV_CHANNEL_EDIT:
        COsdCoreInitChannelEditState();
        break;
    #endif
#endif

    case MENUITEM_SOURCE_SELECT:
        COsdCtrlSetSignalSource(ucArg0);
        break;

    case MENUITEM_OSD_TIMEOUT:
        COsdCtrlSetOsdTimeOutSystem();
        break;

    case MENUITEM_OSD_BLEND:
//->w		RTD_Log(LOGGER_INFO, "\n!!!!!!!COsdCtrlSetOsdBlend = %d\n", (UINT32)ucArg0);
        COsdCtrlSetOsdBlend(ucArg0);
        break;

    case MENUITEM_OSD_LANGUAGE:
        COsdCtrlSetLanguage();
        break;

    case MENUITEM_OSD_RESET:
    
	    //RTD_Log(LOGGER_LEVEL,"\n Tim0 ZoomMode = %d  \n",(UINT32)GET_OSD_DISPLAY_RATIO());
        COsdCtrlSetReset();
        
	    //RTD_Log(LOGGER_LEVEL,"\n Tim4 ZoomMode = %d  \n",(UINT32)GET_OSD_DISPLAY_RATIO());
        break;

    case MENUITEM_SLEEP_MODE:
        COsdCtrlSetSleepMode();
        break;

    case MENUITEM_PICTURE_MODE:
        COsdCtrlSetPictureMode(ucArg0);
        break;

    case MENUITEM_MUSIC_MODE:
        COsdCtrlSetAudioEQ(ucArg0);
        break;

    case MENUITEM_MTS_MODE:
        COsdCtrlSetAudioMTS(ucArg0);
        break;

    case MENUITEM_DISPLAY_RATIO:
        COsdCtrlSetDisplayRatio();
        break;
	case MENUITEM_DISPLAY_IR:
	     COsdCtrlSetDisplayIR();
        break;
	#if CONFIG_ENABLE_FM
	case MENUITEM_DISPLAY_RADIO:
		COsdCtrlSetDisplayRadio();
		 break;
	#endif
		
    case MENUITEM_FAC_WB_R_GAIN:
        COsdCtrlSetWB_RGain();
        break;

    case MENUITEM_FAC_WB_G_GAIN:
        COsdCtrlSetWB_GGain();
        break;

    case MENUITEM_FAC_WB_B_GAIN:
        COsdCtrlSetWB_BGain();
        break;

    case MENUITEM_FAC_WB_R_OFFSET:
        COsdCtrlSetWB_ROffset();
        break;

    case MENUITEM_FAC_WB_G_OFFSET:
        COsdCtrlSetWB_GOffset();
        break;

    case MENUITEM_FAC_WB_B_OFFSET:
        COsdCtrlSetWB_BOffset();
        break;

    case MENUITEM_FAC_WB_H_BRI:
        COsdCtrlSetWBHBrightness();
        break;

    case MENUITEM_FAC_WB_H_CONT:
        COsdCtrlSetWBHContrast();
        break;

    case MENUITEM_FAC_WB_L_BRI:
        COsdCtrlSetWBLBrightness();
        break;

    case MENUITEM_FAC_WB_L_CONT:
        COsdCtrlSetWBLContrast();
        break;

    case MENUITEM_FAC_COLOR_TEM:
        COsdCtrlSetFacColorTemp();
        break;

    case MENUITEM_FAC_COLOR_TEM_R:
        COsdCtrlSetFacColorTempRgain();
        break;

    case MENUITEM_FAC_COLOR_TEM_G:
        COsdCtrlSetFacColorTempGgain();
        break;

    case MENUITEM_FAC_COLOR_TEM_B:
        COsdCtrlSetFacColorTempBgain();
        break;

    case MENUITEM_FAC_ADC_CALIB_HD720P:
        COsdCtrlFacAdcCalibration720p60();
        break;

    case MENUITEM_FAC_ADC_CALIB_VGA:
        COsdCtrlFacAutoColor();
        break;

    case MENUITEM_FAC_ADC_CALIB_1BY1:
        COsdCtrlFacAutoColor();
        break;

    case MENUITEM_FAC_LOGO:
        COsdCtrlSetLogo();
        break;

    case MENUITEM_FAC_DEFAULT_LANGUAGE:
        COsdCtrlSetFacLanguage();
        break;

    case MENUITEM_FAC_CLEAR:
        COsdFactoryClear();
        break;

    case MENUITEM_FAC_CLEAR_ALL:
        COsdFactoryClearAll();
        break;

    case MENUITEM_FAC_PICTURE_MODE:
        COsdCtrlSetFacPictureMode();
        break;

    case MENUITEM_FAC_PICMODE_BRI:
        COsdCtrlSetPicture1Brightness();
        break;

    case MENUITEM_FAC_PICMODE_CONT:
        COsdCtrlSetPicture1Contrast();
        break;

    case MENUITEM_FAC_PICMODE_SAT:
        COsdCtrlSetPicture1Color();
        break;

    case MENUITEM_FAC_BRI_0:
        COsdCtrlSetBrightness_0();
        break;

    case MENUITEM_FAC_BRI_50:
        COsdCtrlSetBrightness_50();
        break;

    case MENUITEM_FAC_BRI_100:
        COsdCtrlSetBrightness_100();
        break;

    case MENUITEM_FAC_CONT_0:
        COsdCtrlSetContrast_0();
        break;

    case MENUITEM_FAC_CONT_50:
        COsdCtrlSetContrast_50();
        break;

    case MENUITEM_FAC_CONT_100:
        COsdCtrlSetContrast_100();
        break;

    case MENUITEM_FAC_SAT_0:
        COsdCtrlSetFacSaturation0();
        break;

    case MENUITEM_FAC_SAT_50:
        COsdCtrlSetFacSaturation50();
        break;

    case MENUITEM_FAC_SAT_100:
        COsdCtrlSetFacSaturation100();
        break;

    case MENUITEM_FAC_MUSIC_MODE:
        COsdCtrlSetFacAudioEQ();
        break;

    case MENUITEM_FAC_MUSIC_MODE_300HZ:
        COsdCtrlSetFacSoundMode300Hz();
        break;

    case MENUITEM_FAC_MUSIC_MODE_1KHZ:
        COsdCtrlSetFacSoundMode1KHz();
        break;

    case MENUITEM_FAC_MUSIC_MODE_2KHZ:
        COsdCtrlSetFacSoundMode2KHz();
        break;

    case MENUITEM_FAC_MUSIC_MODE_3KHZ:
        COsdCtrlSetFacSoundMode3KHz();
        break;

    case MENUITEM_FAC_MUSIC_MODE_8KHZ:
        COsdCtrlSetFacSoundMode8KHz();
        break;

    case MENUITEM_FAC_MUSIC_MODE_15KHZ:
        COsdCtrlSetFacSoundMode15KHz();
        break;

    case MENUITEM_FAC_VOLUME_1:
        COsdCtrlSetFacVolume1(ucArg0);
        break;

    case MENUITEM_FAC_VOLUME_25:
        COsdCtrlSetFacVolume25(ucArg0);
        break;

    case MENUITEM_FAC_VOLUME_50:
        COsdCtrlSetFacVolume50(ucArg0);
        break;

    case MENUITEM_FAC_VOLUME_75:
        COsdCtrlSetFacVolume75(ucArg0);
        break;

    case MENUITEM_FAC_VOLUME_100:
        COsdCtrlSetFacVolume100(ucArg0);
        break;

    case MENUITEM_FAC_TV_VOLUME_1:
        COsdCtrlSetFacTVVolume1(ucArg0);
        break;

    case MENUITEM_FAC_TV_VOLUME_25:
        COsdCtrlSetFacTVVolume25(ucArg0);
        break;

    case MENUITEM_FAC_TV_VOLUME_50:
        COsdCtrlSetFacTVVolume50(ucArg0);
        break;

    case MENUITEM_FAC_TV_VOLUME_75:
        COsdCtrlSetFacTVVolume75(ucArg0);
        break;

    case MENUITEM_FAC_TV_VOLUME_100:
        COsdCtrlSetFacTVVolume100(ucArg0);
        break;

    case MENUITEM_FAC_DCLK_RANGE:
        COsdCtrlSetFacEmi_DCLKSS_RANGE(ucArg0);
        break;

    case MENUITEM_FAC_DCLK_FMDIV:
        COsdCtrlSetFacEmi_DCLKSS_FMDIV(ucArg0);
        break;

        //case MENUITEM_FAC_RF_START_DB:
        //	COsdCtrlSetTunerTop(ucArg0);
        //	break;

    case MENUITEM_PANEL_BACKLIGHT:
        COsdSetPWM1(ucArg0);
        break;

    case MENUITEM_D_DLTI:
        COsdCtrlSetDLti(ucArg0);
        break;

    case MENUITEM_D_DCTI:
        COsdCtrlSetDCti(ucArg0);
        break;

    case MENUITEM_I_DLTI:
        COsdCtrlSetIDomainDLti(ucArg0);
        break;

    case MENUITEM_I_DCTI:
        COsdCtrlSetIDomainDCti(ucArg0);
        break;

        //case MENUITEM_FAC_SHARPNESS:
        //	COsdCtrlSetSharpness(ucArg0);
        //	break;

    case MENUITEM_GAMMA:
        COsdCtrlSetGamma();
        break;

    case MENUITEM_S_Index:
        COsdCtrlSetSIndex();
        break;

    case MENUITEM_S_Low:
        COsdCtrlSetSLow();
        break;

    case MENUITEM_S_High:
        COsdCtrlSetSHigh();
        break;

        //case MENUITEM_PEAKING:
        //	COsdCtrlSetPeaking(ucArg0);
        //	break;

        //case MENUITEM_NR_SPATIAL:
        //	COsdCtrlSetNRSpatial(ucArg0);
        //	break;

    case MENUITEM_DIGITAL_FILTER:
        COsdCtrlSet_DigitalFilter();
        break;

    case MENUITEM_SUNR_Y:
        COsdCtrlSetSUNR_Y(ucArg0);
        break;

    case MENUITEM_SUNR_C:
        COsdCtrlSetSUNR_C(ucArg0);
        break;

    case MENUITEM_SUH_COEF:
        COsdCtrlSetScaleUPH(ucArg0);
        break;

    case MENUITEM_SUV_COEF:
        COsdCtrlSetScaleUPV(ucArg0);
        break;

    case MENUITEM_SUPK:
        COsdCtrlSetScalePK(ucArg0);
        break;

    case MENUITEM_DCC:
        COsdCtrlSetDcc(ucArg0);
        break;

    case MENUITEM_FAC_DCR:
        COsdCtrlSetFacDCR();
        break;

    case MENUITEM_FAC_ICM:
        COsdCtrlSetFacICM();
        break;

    case MENUITEM_SUPK_MASK:
        COsdCtrlSetSUPK_Mask();
        break;

    case MENUITEM_DITHER_BIT:
        COsdCtrlSetDitherBit();
        break;

    case MENUITEM_AUTO_COLOR:
        COsdMenuAutoColor();
        break;

    case MENUITEM_YPBPR_PHASE:
        COsdCtrlSetYPbPrPhase(ucArg0);
        break;
    case MENUITEM_SPREAD_SPECTRUM:
        COsdCtrlSetSpreadSpectrum(ucArg0);
        break;

    case MENUITEM_FAC_UART_PATH:
        COsdCtrlSetUART(ucArg0);
        break;

    case MENUITEM_FAC_RESET:
        COsdCtrlReset(ucArg0);
        break;

    case MENUITEM_VDC_XNR:
        VDC_XNR = ( ucArg0 == _ON)? VDC_XNR_ENABLE : VDC_XNR_DISABLE;
        drvif_module_vdc_XNREnable(VDC_XNR);
        break;

    case MENUITEM_PQ_DEMO:
        COsdCtrlSetPQDemo();
        break;

#ifdef CONFIG_VBI_CC_SUPPORT
    case MENUITEM_OSD_CC:
        COsdCtrlSetCCtype();
        break;
#endif

    default:
        break;
    }
}

/**
 * To get OSD control value.
 * @return control value.
 */
UINT16 COsdCtrlMenuItemGetValue(UINT8 ucMenuItemID)
{
    UINT32 tmp32;
    UINT8  type;
	//RTD_Log(LOGGER_LEVEL,"\n Ttsetdfdfdf \n");
    switch (ucMenuItemID)
    {
    case MENUITEM_SOURCE_SELECT:
        return COsdCtrlGetSignalSource();

#ifdef _VIDEO_TV_SUPPORT
    case MENUITEM_CHANNEL_SCAN:
        return COsdCtrlGetCurFreq();

    case MENUITEM_TABLE_SCAN:
        return COsdCtrlGetCurScanProgress();

    case MENUITEM_MANUAL_SCAN:
        return COsdCtrlGetCurFreq();
#endif

    case MENUITEM_BRIGHTNESS:
        return COsdCtrlGetMainYBrightness();

    case MENUITEM_CONTRAST:
        return COsdCtrlGetMainYContrast();

    case MENUITEM_SATURATION:
        return COsdCtrlGetSaturation();

    case MENUITEM_HUE:
        return COsdCtrlGetHue();

    case MENUITEM_SHARPNESS:
        return COsdCtrlGetSharpness1();

    case MENUITEM_COLOR_TEMPERATURE:
        return COsdCtrlGetColorTemp();

    case MENUITEM_DNR:
        return COsdCtrlGetDNR();

    case MENUITEM_DCR:
        return COsdCtrlGetDCR();

    case MENUITEM_VGA_CLOCK:
        return COsdCtrlGetClock();

    case MENUITEM_VGA_PHASE:
        return COsdCtrlGetPhase();

    case MENUITEM_VGA_H_POSITION:
        return COsdCtrlGetHPosition();

    case MENUITEM_VGA_V_POSITION:
        return COsdCtrlGetVPosition();

//	case MENUITEM_VGA_H_SIZE:
//		return COsdCtrlGetHSize();

//	case MENUITEM_VGA_V_SIZE:
//		return COsdCtrlGetVSize();

    case MENUITEM_AUDIO_BALANCE:
        return COsdCtrlGetAudioBalance();

    case MENUITEM_AUDIO_SPATIAL:
        return COsdCtrlGetAudioSpatial();

    case MENUITEM_AUDIO_BASS:
        return COsdCtrlGetAudioBass();

    case MENUITEM_AUDIO_TREBLE:
        return COsdCtrlGetAudioTreble();

    case MENUITEM_MUSIC_MODE_300HZ:
        return ui_get_eq_300(GET_EQ_MODE());

    case MENUITEM_MUSIC_MODE_1KHZ:
        return ui_get_eq_1k(GET_EQ_MODE());

    case MENUITEM_MUSIC_MODE_2KHZ:
        return ui_get_eq_2k(GET_EQ_MODE());

    case MENUITEM_MUSIC_MODE_3KHZ:
        return ui_get_eq_3k(GET_EQ_MODE());

    case MENUITEM_MUSIC_MODE_8KHZ:
        return ui_get_eq_8k(GET_EQ_MODE());

    case MENUITEM_MUSIC_MODE_15KHZ:
        return ui_get_eq_15k(GET_EQ_MODE());

    case MENUITEM_AUDIO_VOLUME:
        return COsdCtrlGetAudioVolume();

#ifdef _VIDEO_TV_SUPPORT
    case MENUITEM_TV_VIDEO_STD:
		type = COsdCtrlGetTVVideoStd();
	    type = CConverColorModeProc(type);
        return type;

    case MENUITEM_TV_AUDIO_STD:
	    type = COsdCtrlGetSoundSystem();
	    type = CConverModeProc(type);
        return type;

    case MENUITEM_TV_FREQ_FINE_TUNE:
        return COsdCtrlGetTVFineTune();

//Tim 0323
//==============================================================================
	case MENUITEM_TV_NTSC_STD:
    	return COsdCtrlGetTVNtscStd();
    	
    case MENUITEM_NTSC_FREE:
        return COsdCtrlGetNtscFree();
//==============================================================================
    case MENUITEM_TV_COUNTRY:
        return COsdCtrlGetCountrySelect();

    case MENUITEM_CHANNEL_SKIP:
        return COsdCtrlGetChanelSkip();
    case MENUITEM_AFC:
        return COsdCtrlGetChanelAFC();
#endif
//	case MENUITEM_TV_BLUE_SCREEN:
//		return COsdCtrlGetTVBlueScreen();

    case MENUITEM_OSD_TIMEOUT:
        return COsdCtrlGetOsdTimeOutSystem();

    case MENUITEM_OSD_BLEND:
        return COsdCtrlGetOsdBlend();

    case MENUITEM_OSD_LANGUAGE:
        return COsdCtrlGetLanguage();

    case MENUITEM_SLEEP_MODE:
        return COsdCtrlGetSleepMode();

    case MENUITEM_PICTURE_MODE:
        return COsdCtrlGetPictureMode();

    case MENUITEM_MUSIC_MODE:
        return COsdCtrlGetAudioEQ();

    case MENUITEM_MTS_MODE:
        return COsdCtrlGetAudioMTS();

    case MENUITEM_DISPLAY_RATIO:
        return COsdCtrlGetDisplayRatio();
	case MENUITEM_DISPLAY_IR:
		 return COsdCtrlGetDisplayIR();
	#if CONFIG_ENABLE_FM
	case MENUITEM_DISPLAY_RADIO:
		 return  COsdCtrlGetDisplayRadio();			   
	#endif
    case MENUITEM_FAC_WB_R_GAIN:
        return COsdCtrlGetWB_RGain();

    case MENUITEM_FAC_WB_G_GAIN:
        return COsdCtrlGetWB_GGain();

    case MENUITEM_FAC_WB_B_GAIN:
        return COsdCtrlGetWB_BGain();

    case MENUITEM_FAC_WB_R_OFFSET:
        return COsdCtrlGetWB_ROffset();

    case MENUITEM_FAC_WB_G_OFFSET:
        return COsdCtrlGetWB_GOffset();

    case MENUITEM_FAC_WB_B_OFFSET:
        return COsdCtrlGetWB_BOffset();

    case MENUITEM_FAC_WB_H_BRI:
        return COsdCtrlGetWBHBrightness();

    case MENUITEM_FAC_WB_H_CONT:
        return COsdCtrlGetWBHContrast();

    case MENUITEM_FAC_WB_L_BRI:
        return COsdCtrlGetWBLBrightness();

    case MENUITEM_FAC_WB_L_CONT:
        return COsdCtrlGetWBLContrast();

    case MENUITEM_FAC_COLOR_TEM:
        return COsdCtrlGetFacColorTemp();

    case MENUITEM_FAC_COLOR_TEM_R:
        return COsdCtrlGetFacColorTempRgain();

    case MENUITEM_FAC_COLOR_TEM_G:
        return COsdCtrlGetFacColorTempGgain();

    case MENUITEM_FAC_COLOR_TEM_B:
        return COsdCtrlGetFacColorTempBgain();

    case MENUITEM_FAC_LOGO:
        return COsdCtrlGetLogo();

    case MENUITEM_FAC_DEFAULT_LANGUAGE:
        return COsdCtrlGetFacLanguage();

    case MENUITEM_FAC_PICTURE_MODE:
        return COsdCtrlGetFacPictureMode();

    case MENUITEM_FAC_PICMODE_BRI:
        return COsdCtrlGetPicture1Brightness();

    case MENUITEM_FAC_PICMODE_CONT:
        return COsdCtrlGetPicture1Contrast();

    case MENUITEM_FAC_PICMODE_SAT:
        return COsdCtrlGetPicture1Color();

    case MENUITEM_FAC_BRI_0:
        return COsdCtrlGetBrightness_0();

    case MENUITEM_FAC_BRI_50:
        return COsdCtrlGetBrightness_50();

    case MENUITEM_FAC_BRI_100:
        return COsdCtrlGetBrightness_100();

    case MENUITEM_FAC_CONT_0:
        return COsdCtrlGetContrast_0();

    case MENUITEM_FAC_CONT_50:
        return COsdCtrlGetContrast_50();

    case MENUITEM_FAC_CONT_100:
        return COsdCtrlGetContrast_100();

    case MENUITEM_FAC_SAT_0:
        return COsdCtrlGetFacSaturation0();

    case MENUITEM_FAC_SAT_50:
        return COsdCtrlGetFacSaturation50();

    case MENUITEM_FAC_SAT_100:
        return COsdCtrlGetFacSaturation100();

    case MENUITEM_FAC_MUSIC_MODE:
        return COsdCtrlGetFacAudioEQ();

    case MENUITEM_FAC_MUSIC_MODE_300HZ:
        return COsdCtrlGetFacSoundMode300Hz();

    case MENUITEM_FAC_MUSIC_MODE_1KHZ:
        return COsdCtrlGetFacSoundMode1KHz();

    case MENUITEM_FAC_MUSIC_MODE_2KHZ:
        return COsdCtrlGetFacSoundMode2KHz();

    case MENUITEM_FAC_MUSIC_MODE_3KHZ:
        return COsdCtrlGetFacSoundMode3KHz();

    case MENUITEM_FAC_MUSIC_MODE_8KHZ:
        return COsdCtrlGetFacSoundMode8KHz();

    case MENUITEM_FAC_MUSIC_MODE_15KHZ:
        return COsdCtrlGetFacSoundMode15KHz();

    case MENUITEM_FAC_VOLUME_1:
        return COsdCtrlGetFacVolume1();

    case MENUITEM_FAC_VOLUME_25:
        return COsdCtrlGetFacVolume25();

    case MENUITEM_FAC_VOLUME_50:
        return COsdCtrlGetFacVolume50();

    case MENUITEM_FAC_VOLUME_75:
        return COsdCtrlGetFacVolume75();

    case MENUITEM_FAC_VOLUME_100:
        return COsdCtrlGetFacVolume100();

    case MENUITEM_FAC_TV_VOLUME_1:
        return COsdCtrlGetFacTVVolume1();

    case MENUITEM_FAC_TV_VOLUME_25:
        return COsdCtrlGetFacTVVolume25();

    case MENUITEM_FAC_TV_VOLUME_50:
        return COsdCtrlGetFacTVVolume50();

    case MENUITEM_FAC_TV_VOLUME_75:
        return COsdCtrlGetFacTVVolume75();

    case MENUITEM_FAC_TV_VOLUME_100:
        return COsdCtrlGetFacTVVolume100();

    case MENUITEM_FAC_DCLK_RANGE:
        return COsdCtrlGetFacEmi_DCLKSS_RANGE();

    case MENUITEM_FAC_DCLK_FMDIV:
        return COsdCtrlGetFacEmi_DCLKSS_FMDIV();

    case MENUITEM_DITHER_BIT:
        return COsdCtrlGetDitherBit();

        //case MENUITEM_FAC_RF_START_DB:
        //	return COsdCtrlGetTunerTop();

    case MENUITEM_PANEL_BACKLIGHT:
        return COsdGetPWM1();

    case MENUITEM_D_DLTI:
        return COsdCtrlGetDLti();

    case MENUITEM_D_DCTI:
        return COsdCtrlGetDCti();

    case MENUITEM_I_DLTI:
        return COsdCtrlGetIDomainDLti();

    case MENUITEM_I_DCTI:
        return COsdCtrlGetIDomainDCti();

        //case MENUITEM_FAC_SHARPNESS:
        //	return COsdCtrlGetSharpness();

    case MENUITEM_GAMMA:
        return COsdCtrlGetGamma();

    case MENUITEM_S_Index:
        return COsdCtrlGetSIndex();
        break;

    case MENUITEM_S_Low:
        return COsdCtrlGetSLow();
        break;

    case MENUITEM_S_High:
        return COsdCtrlGetSHigh();
        break;
        //case MENUITEM_PEAKING:
        //	return COsdCtrlGetPeaking();

        //case MENUITEM_NR_SPATIAL:
        //	return COsdCtrlGetNRSpatial();

    case MENUITEM_DIGITAL_FILTER:
        return COsdCtrlGet_DigitalFilter();

    case MENUITEM_SUNR_Y:
        return COsdCtrlGetSUNR_Y();

    case MENUITEM_SUNR_C:
        return COsdCtrlGetSUNR_C();

    case MENUITEM_SUH_COEF:
        return COsdCtrlGetScaleUPH();

    case MENUITEM_SUV_COEF:
        return COsdCtrlGetScaleUPV();

    case MENUITEM_SUPK:
        return COsdCtrlGetScalePK();

    case MENUITEM_DCC:
        return COsdCtrlGetDcc();

    case MENUITEM_FAC_DCR:
        return COsdCtrlGetFacDCR();

    case MENUITEM_FAC_ICM:
        return COsdCtrlGetFacICM();

    case MENUITEM_SUPK_MASK:
        return COsdCtrlGetSUPK_Mask();

    case MENUITEM_AUTO_COLOR:
        return 0;

    case MENUITEM_YPBPR_PHASE:
        return COsdCtrlGetYPbPrPhase();
    case MENUITEM_SPREAD_SPECTRUM:
        return COsdCtrlGetSpreadSpectrum();

    case MENUITEM_FAC_UART_PATH:
        return COsdCtrlGetUART();

    case MENUITEM_FAC_RESET:
        return 0;

    case MENUITEM_PQ_DEMO:
        return COsdCtrlGetPQDemo();

    case MENUITEM_VDC_XNR:
        tmp32 = rtdf_inl(VDC_XNR_CTRL_0x54f0);
        VDC_XNR = (tmp32 & _BIT0);
        return VDC_XNR;

#ifdef CONFIG_VBI_CC_SUPPORT
    case MENUITEM_OSD_CC:
        return COsdCtrlGetCCtype();
#endif

    default:
        return 0;
    }
}


/**
 * To get min value of specified item.
 * @return min value of slider.
 */
UINT8 COsdCtrlMenuItemGetMin(UINT8 ucMenuItemID)
{
    switch (ucMenuItemID)
    {
#ifdef _VIDEO_TV_SUPPORT
    case MENUITEM_CHANNEL_SCAN:
        return COsdCtrlGetMinFreq();

    case MENUITEM_MANUAL_SCAN:
        return COsdCtrlGetMinFreq();
#endif

    default:
        return 0;
    }
}

/**
 * To get max value of specified item.
 * @return max value of slider.
 */
UINT8 COsdCtrlMenuItemGetMax(UINT8 ucMenuItemID)
{
    switch (ucMenuItemID)
    {
    #ifdef _VIDEO_TV_SUPPORT
    case MENUITEM_CHANNEL_SCAN:
        return COsdCtrlGetMaxFreq();

    case MENUITEM_MANUAL_SCAN:
        return COsdCtrlGetMaxFreq();
	#endif
    case MENUITEM_YPBPR_PHASE:
        return 63;
    case MENUITEM_SPREAD_SPECTRUM:
        return 15;
    case MENUITEM_SHARPNESS:
        return 20;
    default:
        return 100;
    }
}

/**
 * To get default value of specified item.
 * @return default value.
 */
UINT8 COsdCtrlMenuItemGetDefault(UINT8 ucMenuItemID)
{
    UINT8 src;

    src = x_get_color_src_id();
    switch (ucMenuItemID)
    {
    case MENUITEM_BRIGHTNESS:
        SET_BRIGHTNESS(stColorDefaultData[src].Brightness);
        return stColorDefaultData[src].Brightness;

    case MENUITEM_CONTRAST:
        SET_CONTRAST(stColorDefaultData[src].Contrast);
        return stColorDefaultData[src].Contrast;

    case MENUITEM_SATURATION:
        SET_SATURATION(stColorDefaultData[src].Saturation);
        return stColorDefaultData[src].Saturation;

    case MENUITEM_HUE:
        SET_HUE(stColorDefaultData[src].Hue);
        return stColorDefaultData[src].Hue;

    case MENUITEM_SHARPNESS:
        SET_SHARPNESS(stColorDefaultData[src].Sharpness);
        return stColorDefaultData[src].Sharpness;

    case MENUITEM_COLOR_TEMPERATURE:
        SET_COLOR_TEMP_TYPE(stColorDefaultData[src].ColorTemperature);
        return stColorDefaultData[src].ColorTemperature;
		
    case MENUITEM_DNR:
        SET_DNR(stColorDefaultData[src].DNR);
        return stColorDefaultData[src].DNR;

    case MENUITEM_VGA_CLOCK:
        SET_CLOCK(stScreenModeInfoDefaultData.Clock);
        return stScreenModeInfoDefaultData.Clock;

    case MENUITEM_VGA_PHASE:
        SET_PHASE(stScreenModeInfoDefaultData.Phase);
        return stScreenModeInfoDefaultData.Phase;

    case MENUITEM_VGA_H_POSITION:
        SET_H_POSITION(stScreenModeInfoDefaultData.H_Position);
        return stScreenModeInfoDefaultData.H_Position;

    case MENUITEM_VGA_V_POSITION:
        SET_V_POSITION(stScreenModeInfoDefaultData.V_Position);
        return stScreenModeInfoDefaultData.V_Position;

    case MENUITEM_OSD_TIMEOUT:
        SET_OSD_TIMEOUT(stOsdUserDefaultData.OsdTimeout);
        return stOsdUserDefaultData.OsdTimeout;

    case MENUITEM_OSD_BLEND:
        SET_OSD_BLENDING(stOsdUserDefaultData.OsdBlending);
        return stOsdUserDefaultData.OsdBlending;

    case MENUITEM_OSD_LANGUAGE:
        SET_OSD_LANGUAGE(stOsdUserDefaultData.Language);
        return stOsdUserDefaultData.Language;

    case MENUITEM_PICTURE_MODE:
        SET_PICTURE_MODE(stOsdUserDefaultData.PictureMode);
        return stOsdUserDefaultData.PictureMode;

    case MENUITEM_DISPLAY_RATIO:
        SET_DISPLAY_RATIO(stSystemDataDefault.Mode);
        return stSystemDataDefault.Mode;
	case MENUITEM_DISPLAY_IR:
		SYNC_STORAGE_DATA(StructSystemDataType,  IrMode, stSystemDataDefault.IrMode);
	    //SET_DISPLAY_IR(stSystemDataDefault.IrMode);
        return stSystemDataDefault.IrMode;
#if CONFIG_ENABLE_FM
		case MENUITEM_DISPLAY_RADIO:
		SYNC_STORAGE_DATA(StructSystemDataType,  FMMode, stSystemDataDefault.FMMode);	
		return stSystemDataDefault.FMMode;
#endif

    case MENUITEM_PANEL_BACKLIGHT:
        SET_PANEL_BACKLIGHT( (UINT16)(254*stColorDefaultData[src].BackLight)/10);
        return stColorDefaultData[src].BackLight;

    case MENUITEM_AUDIO_VOLUME:
        SET_VOLUME(stAudioDefaultData.Volumn);
        return stAudioDefaultData.Volumn;

    case MENUITEM_AUDIO_BALANCE:
        SET_BALANCE(stAudioDefaultData.Balance);
        return stAudioDefaultData.Balance;

    case MENUITEM_AUDIO_SPATIAL:
        SET_SPATIAL(stAudioDefaultData.Spatial);
        return stAudioDefaultData.Spatial;

    case MENUITEM_AUDIO_BASS:
        SET_BASS(stAudioDefaultData.Bass);
        return stAudioDefaultData.Bass;

    case MENUITEM_AUDIO_TREBLE:
        SET_TREBLE(stAudioDefaultData.Treble);
        return stAudioDefaultData.Treble;

    case MENUITEM_AUDIO_EQ_BAR:
        SET_EQ_MODE(stAudioDefaultData.EqMode);
        return stAudioDefaultData.EqMode;

    case MENUITEM_TV_AVL:
        return stAudioDefaultData.AutoVolume;

    case MENUITEM_TV_SPEAKER_COMPENSATION:
        return stAudioDefaultData.Compensation;

//	case MENUITEM_TV_BLUE_SCREEN:
//		if ( stTVDataDefaultData.BlueScreen)
//			SET_TV_BLUE_SCREEN();
//		else
//			CLR_TV_BLUE_SCREEN();
//		return stTVDataDefaultData.BlueScreen;

    default:
        return 0;
    }
}

/**
 * To get menu item enable statue.
 * @return enable statue.
 */
UINT8 COsdCtrlMenuItemGetEnable(UINT8 ucMenuItemID)
{
#ifdef _VIDEO_TV_SUPPORT
	UINT8 CurChn;

	CurChn = GET_TV_CHN_DATA(_CUR_CHN_TV);
#endif
    switch (ucMenuItemID)
    {
#if 0
    case MENUITEM_BRIGHTNESS:
    case MENUITEM_CONTRAST:
    case MENUITEM_SATURATION:
    case MENUITEM_HUE:
    case MENUITEM_SHARPNESS:
    case MENUITEM_COLOR_TEMPERATURE:
    case MENUITEM_DNR:
    case MENUITEM_DCR:
    case MENUITEM_AUDIO_BALANCE:
    case MENUITEM_AUDIO_BASS:
    case MENUITEM_AUDIO_TREBLE:
    case MENUITEM_AUDIO_EQ_BAR:
#endif
//	case MENUITEM_TV_BLUE_SCREEN:
//		if ( GET_INPUTSOURCE_TYPE() != _SOURCE_TV)
//			return _FALSE;
//		break;
    case MENUITEM_SATURATION:
        if (GET_INPUTSOURCE_TYPE()==_SOURCE_VGA)
            return _FALSE;
        break;
    #if defined(_REALTEK_OSD_USE)
    case MENUITEM_TV_CHANNEL_EDIT:
        return _FALSE;
        break;
    #endif
    case MENUITEM_HUE:
        if ((GET_INPUTSOURCE_FROM()==_SRC_FROM_VDC)&&(GET_CURRENT_MODE() != _MODE_480I))
            return _FALSE;
        if (GET_INPUTSOURCE_TYPE()==_SOURCE_VGA)
            return _FALSE;
        break;

    case MENUITEM_VGA_AUTO_POSITION:
    case MENUITEM_VGA_CLOCK:
    case MENUITEM_VGA_PHASE:
    case MENUITEM_VGA_H_POSITION:
    case MENUITEM_VGA_V_POSITION:
        if ( info->state != _MODE_STATE_ACTIVE)	// no signal !!
            return _FALSE;
        break;
//Tim 0323
//==============================================================================
	/*#ifdef _ONLY_NTSC_SYSTEM
	case MENUITEM_NTSC_FREE:
		if(GET_CHANNEL_COLORSYSTEM(CurChn) == ZPAL_I)
			return _FALSE;
		break;
	#endif*/
	
	case MENUITEM_TV_AUTO_SCAN:
		if(COsdCtrlGetNtscFree())//Table
			return _FALSE;			
		break;

    case MENUITEM_TV_NTSC_STD:
        if(COsdCtrlGetNtscFree())//Table
			return _TRUE;
		else
			return _FALSE;
    	break;
    	

    case MENUITEM_TV_TABLE_SCAN:
        //if((GET_CHANNEL_COLORSYSTEM(GET_TV_CUR_CHN()) != ZNTSC) || (GET_TV_CUR_FreeType() == _DISABLE))//table
        //return _FALSE;
        if(COsdCtrlGetNtscFree())//Table
        	return _TRUE;
        else
        	return _FALSE;
        break;

	    	

//==============================================================================
	//Tim 0404
//==============================================================================
	#ifdef CONFIG_VBI_CC_SUPPORT
	case MENUITEM_OSD_CC:
		if((GET_INPUTSOURCE_TYPE() == _SOURCE_TV) || (GET_INPUTSOURCE_TYPE() == _SOURCE_CVBS) || (GET_INPUTSOURCE_TYPE() == _SOURCE_SV))
			return _TRUE;
		else
			return _FALSE;
		break;
	#endif
//==============================================================================
    default:
        break;
    }

    return _TRUE;
}


/**
 * To get label string of specified bracket item.
 * @return label string.
 */
UINT8 code tBracket1DummyStr[] = {_MINUS_, _MINUS_, _MINUS_, _END_};
UINT8* COsdCtrlMenuItemGetString(UINT8 ucMenuItemID)
{
    switch (ucMenuItemID)
    {
    case MENUITEM_FAC_FW_VERSIOIN:
    case MENUITEM_FAC_PANEL_TYPE:
    case MENUITEM_FAC_FLASH_TYPE:
        return tBracket1DummyStr;

    default:
        return tBracket1DummyStr;
    }
}
//Tim 0406 For Move Function From Bank3
#if 1
void COsdCoreDrawMainMenuFrame(UINT8 ucMainID)
{
    UINT8 i,row;
    UINT8 *tIcon;
    UINT8 maxindex;
    
    OSDLine(ROW(3), COL(1), LENGTH(37), 0xA6, _WRITE_BYTE0);
    OSDLine(ROW(3), COL(1), LENGTH(37), 0xA4, _WRITE_BYTE1);
    OSDLine(ROW(3), COL(1), LENGTH(37), 0x11, _WRITE_BYTE2);

    OSDLine(ROW(16), COL(1), LENGTH(37), 0xA6, _WRITE_BYTE0);
    OSDLine(ROW(16), COL(1), LENGTH(37), 0xA4, _WRITE_BYTE1);
    OSDLine(ROW(16), COL(1), LENGTH(37), 0x11, _WRITE_BYTE2);

    DrawLeftLine();

    for (i=0; i<6; i++)
    {
        SETCOLOR_SUBMENU_SELLINE(SHOWINDEX_TO_LINE(i) + 1);
    }
	if((GET_INPUTSOURCE_INDEX() == _SOURCE_INDEX_AV_3)||(GET_INPUTSOURCE_INDEX() == _SOURCE_INDEX_AV)||(GET_INPUTSOURCE_INDEX() == _SOURCE_INDEX_AV_1)||(GET_INPUTSOURCE_INDEX() == _SOURCE_INDEX_AV_2))
		maxindex = 2;
	else if(GET_INPUTSOURCE_INDEX() == _SOURCE_INDEX_VGA)
		maxindex = 4;
	else
		maxindex = 2;
	
    for (i=0; i</*stLayerStack[ucLayerIndex].pMenuItemSet->ucItemCount*/maxindex; i++)
    {
    	if((GET_INPUTSOURCE_INDEX() == _SOURCE_INDEX_AV)||(GET_INPUTSOURCE_INDEX() == _SOURCE_INDEX_AV_1)||(GET_INPUTSOURCE_INDEX() == _SOURCE_INDEX_AV_2)||(GET_INPUTSOURCE_INDEX() == _SOURCE_INDEX_AV_3))
    	{
	        if (ucMainID==i)
	            tIcon = Icons_Select_av[i];
	        else
	            tIcon = Icons_normale_av[i];
	    }
	    else if(GET_INPUTSOURCE_INDEX() == _SOURCE_INDEX_VGA)
	    {
	    	if (ucMainID==i)
	            tIcon = Icons_Select_vga[i];
	        else
	            tIcon = Icons_normale_vga[i];	
	    }
	    else
	    {
	    	if (ucMainID==i)
	            tIcon = Icons_Select_hdmi[i];
	        else
	            tIcon = Icons_normale_hdmi[i];		
	    }
        row = 4+i*2;
     
        Draw2bit3x2Icon(tIcon,  row,  1);
    }

    COsdFxDrawWindow(&tMainWindowStyle);

    // set osd position
    COsdFxSetWinRefPos(_OSD_MAINMENU_H_POS, _OSD_MAINMENU_V_POS);
}

void COsdCoreDispModeBox(UINT8 ucOpt)
{
    UINT8 ucCol;
    UINT8 idx;

    if ( (ucOpt&_BIT0) != 0)
    {
        COsdCoreDrawInfoBoxFrame(pListItem->pString, _INFOBOX_HINT_BAR | _MODEBOX_TYPE);
    }

    // display mode status
    idx = COsdCoreListItemValueToIdx(COsdCtrlMenuItemGetValue(pListItem->ucMenuItemID));
    ucCol = COsdFxLoadText(2, 0, 34, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, pListItem->pItemList[idx].pString);
    if ( ucCol < _OSD_ITEM_ROW_LENGTH)
        COsdFxClearText(2, ucCol, 1, _OSD_ITEM_ROW_LENGTH-ucCol);

    if ( (ucOpt&_BIT0) != 0)
        COsdFxEnableOsd();

}

#endif

#ifdef CONFIG_VBI_CC_SUPPORT
UINT8 COsdCtrlGetCCtype(void)
{
    return GET_CLOSEDCAPTION();
}

void COsdCtrlSetCCtype(void)
{
    if (ucCtrlArg0!=GET_CLOSEDCAPTION())
    {
        SET_CLOSEDCAPTION(ucCtrlArg0);
        SYNC_STORAGE_DATA(StructSystemDataType,  CcSystem, ucCtrlArg0);
    }
}
#endif


/**
 *
 * @}
 */

