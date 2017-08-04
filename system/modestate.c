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
* @{
*/
/*============================ Module dependency  ===========================*/
#include "sparrow_utility.h"
#include "modestate.h"
#include "flow_struct.h"
#include "message.h"
#include "osd_sys_msg.h"
#include "source.h"
#include "ui_struct.h"
#include "flow_display.h"
#include "rosprintf.h"
#include "mode.h"
#include "flow_conf_struct.h"
#include "flow_audio.h"
#include "timer_event.h"
#include "timer_event_sys_table.h"
#include "flow_color.h"
#include "framesync.h"
#include "vip\color.h"
#include "flow_scaler.h"
#include "scaler_idomain\syncproc.h"
#include "scaler_idomain\adc.h"
#include "video\video.h"
#include "audio\audio_api.h"
#include "tv.h"
#include "ifd\ifd.h"
#include "osdcontrol.h"
#include "mode_customer.h"
#include "rt_storage.h"
#include "storage_system.h"
#include "osdcore.h"
#include "ap_rtirkey.h"
#include "timer_event_ui_table.h"
#include "scaler_idomain\vgip.h"
#include "power.h"
#include "vbi/slr.h"
#include "ui_ctrl.h"
#include "osdfunction.h"//For Keil Building
#include "pcb_customer.h"
#include "pcb\pcb_utility.h"



UINT16 fModestate_Msg;
UINT8 ucUIScalerSyncInfo;
UINT8 xdata IsActivePowerSavingEvent=0;
UINT8 xdata fAutoscan;
UINT8 xdata fAutoscan1;
UINT8 xdata fFirstPowerOn;

// extern StructUserDataChannelSettingsType xdata _channelData[5];

/**
 * This API is used to init some variables  input signal
 *
 * @param [input]:	<void>
 * @return none
 *
 */
void fw_modestate_Init(void)
{
    info = &disp_info;
    info->input_src = stUISignalSource;
    info->state = _MODE_STATE_SEARCH;
    info->channel = _CHANNEL1;
    info->disp_status = 0;
    fModestate_Msg = 0;
    ucUIScalerSyncInfo = 0;
    fAutoscan = 0;
    fAutoscan1 = 0;
    fFirstPowerOn = 0;
    flow_audio_Setmute(_ENABLE, SCALER, (ADC_CH_IDX)(ADC_CH_L|ADC_CH_R));
	// don't show logo when power saving
	if(ucPowersavingdisablelogo==0)
	{
	ucPowersavingdisablelogo = 0;
#if(_LOGOL_ENABLE)
	//fw_ui_message_push(_FL2UI_SHOW_LOGO);
#endif
    RTD_Log(LOGGER_DEBUG, "Initial modestate is completed. \n");
	}
//	drv_scalerdisplay_Init();
#if defined(fUseCPUSwitchEnable)
//	timerevent_table_Switch_CPU_Clock();
#endif
}

#if defined(CONFIG_WSS_SUPPORT)
/*
PAL WSS GROUP 1 (ASPECT RATIO) DATA BIT ASSIGNMENTS AND USAGE
1000 => 4:3 (576)
0001 => 14:9 (letterbox center; 504)
0010 => 14:9 (letterbox  top; 504)
1011 => 16:9 (letterbox center; 430)
0100 => 16:9 (letterbox top ; 430)
1101 => >16:9 (letterbox center)
1110 => 14:9 (576)
0111 => 16:9 (576)
*/
static void check_wss(BOOL bNeedNotifyFlow, UINT8 stableCnt)
{
    UINT16 wssData = 0;
    static UINT16 preWssData = 0;
    //linmc@20100115, add for debounce checking
    static UINT16 wssDataCnt=0;

    // step1, if src without wss info, drop it (vga/hdmi..etc.
    // in TV020 application, Svideo is DVD src input
    if ((GET_INPUTSOURCE_TYPE() != _SOURCE_TV)
            && (GET_INPUTSOURCE_TYPE() != _SOURCE_CVBS))
        return;

#if defined(CONFIG_TTX_SUPPORT)
    // step2, in TT side-by-side state, it always show 16:9
    //if (IS_RATIO_TT_POP())
    //	return;
#endif

    // step3, it is update wss info, whatever the display ratio is auto/manual
    drvif_vbi_read_wss(&wssData);
    RTD_Log(LOGGER_INFO, "wssData=%d\n", (UINT32)wssData);
    if (preWssData == wssData)
        wssDataCnt = (wssDataCnt > 250) ? 250 : wssDataCnt+1;
    else
    {
        wssDataCnt = 0;
        preWssData = wssData;
    }

    // step4, check if wssdata is stable
    if (wssDataCnt < stableCnt)
        return;
    if (GET_OSD_DISPLAY_RATIO() == RATIO_AUTO)
    {
        switch (wssData)
        {
        case 8:
            if (GET_DISPLAY_RATIO() != RATIO_4_3)
                SET_DISPLAY_RATIO(RATIO_4_3);
            else
                return;
            break;
        case 1:
        case 2:
        case 14:
        case 4:
        case 7:
        case 11:
        case 13:
            if (GET_DISPLAY_RATIO() != RATIO_16_9)
                SET_DISPLAY_RATIO(RATIO_16_9);
            else
                return;
            break;
        default:
            return;
        }

        if (bNeedNotifyFlow)
        {
            RTD_Log(LOGGER_INFO, "wssData=%d\n", (UINT32)wssData);
            RTD_Log(LOGGER_INFO, "wssDataCnt=%d\n", (UINT32)wssDataCnt);
            RTD_Log(LOGGER_INFO, "check_wss, set display ratio=%d\n", (UINT32)GET_DISPLAY_RATIO());
            fw_flow_message_push(_MODE_MSG_MAIN_SET_VGIP_AFTER_FOUND_MODE_SIGNAL);
        }
    }
    else
    {
        wssDataCnt = 0;
    }
}


#endif

/**
 * modestate_global_process_msg
 * This API is used to process dedicated message
 *
 * @param [input]:  <void>		{ void }
 * @return None
 *
 */
bit modestate_global_process_msg(void)
{
    UINT8 result;
    // Clear all same message avoid to accept repeadly.
    fw_flow_message_clearallType(fModestate_Msg);

    // try to handle global messages first
    switch (fModestate_Msg)
    {
    case _UI2FL_MAIN_DISPLAYRATIO_CHANGE:
#ifdef CONFIG_WSS_SUPPORT
        check_wss(_FALSE, 0);//if set aspect auto, need to detect WSS
#endif //#ifdef CONFIG_WSS_ENABLE			
        fModestate_Msg = _MODE_MSG_MAIN_SET_VGIP_AFTER_FOUND_MODE_SIGNAL;
        break;

    case _UI2FL_TV_AUTOSCAN_START:
    #ifdef _VIDEO_TV_SUPPORT
        if (GET_INPUTSOURCE_TYPE() == _SOURCE_TV)
        {
            fw_flow_message_push(_FL2FL_MAIN_STATE_PERIODIC_EVENT);
        }
        info->state = _MODE_STATE_TV_SCAN;
        CTvAutoScanStart();
        fAutoscan = 1;
    #endif
        break;

    case _UI2FL_TV_MENUALSCAN_START:
    #ifdef _VIDEO_TV_SUPPORT
        if (GET_INPUTSOURCE_TYPE() == _SOURCE_TV)
        {
            fw_flow_message_push(_FL2FL_MAIN_STATE_PERIODIC_EVENT);
        }
        info->state = _MODE_STATE_TV_SCAN;
        CTvManualScanStart();
    #endif
        break;

    case _UI2FL_ADC_AUTOADJUST_START:
    case _FL2FL_ADC_AUTOADJUST_START:
        if ((!GET_SCART_MANUAL_MODE())&& (GET_SCART_PLUG_IN()||GET_SCART2_PLUG_IN()))
        {
            RTD_Log(LOGGER_INFO, "\nPASSER!!!\n");
            break;
        }

        flow_display_BackgroundColor(0, 0, 0);
        COsdCoreInitAutoAdjustState(INFORM_ADJUSTING);

        result = drv_adc_auto_config();

        if (result == ERROR_SUCCEED)
        {
            SET_AUTO_ADJUST_DONE();
            ui_set_VGA_info(info->ucMode_Curr);

            //SYNC_STORAGE_ARRAY_STRUCT_DATA(StructUserDataChannelSettingsType, 0, _channelData[0]);
            //SYNC_STORAGE_DATA(StructVGAScreenModeInfo, Status, stVGAModeCurrInfo.Status);
            /*COsdCoreSetState(_MAINMENU_STATE);
            COsdCoreBackToMainMenu(_TRUE);*/
            //fw_timer_event_CancelTimerEvent(timerevent_ui_table_OSD_timeout_event);
            //fw_timer_event_ActiveTimerEvent(1, timerevent_ui_table_OSD_timeout_event);
        }
        else if (result==ERROR_ABORT)
        {
            /*COsdCoreSetState(_MAINMENU_STATE);
            COsdCoreBackToMainMenu(_TRUE);*/
            //fw_timer_event_CancelTimerEvent(timerevent_ui_table_OSD_timeout_event);
            //fw_timer_event_ActiveTimerEvent(0.01, timerevent_ui_table_OSD_timeout_event);
        }
        //send ui task autoadjust end msg
        //fw_ui_message_push(_FL2UI_AUTO_ADJUST_END);
        fw_ui_message_clear();
        fw_ui_message_push(_EXIT_KEY_MESSAGE);
      //  fw_ui_message_push(_DISPLAY_MESSAGE);
        flow_display_BackgroundColor(0, 0, 0xff);
        //fw_timer_event_CancelTimerEvent(timerevent_ui_table_OSD_timeout_event);
        //fw_timer_event_ActiveTimerEvent(1, timerevent_ui_table_OSD_timeout_event);
        break;

    case _UI2FL_ADC_AUTOCOLOR_START:
        if (GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR)  	// weihao 960308
        {
//->w				drvif_color_ypbpr_adjustment_dragon_bartype(_MAIN_DISPLAY, &stYPbPrGainOffsetData[disp_info[_MAIN_DISPLAY].ucMode_Curr - _MODE_480I],ClrBR100);//20080925
//->w				CStorageUpdateStructData();    //here update data immediatly
//->w				CStorageUpdateToFlash();
        }
        else if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
        {
            // CSW+ 0971128 Disable ABL first
//->w				rtdf_clearBits(IDOMAIN_ABL_CTRL_VADDR, _BIT31);
//->w				ScalerTimer_DelayXms(20);

//->w				if(fwif_adc_autobalance(_MAIN_DISPLAY)!=ERROR_SUCCEED)
//->w					fwif_autobalance_BackToDefault(_MAIN_DISPLAY);

            // CSW+ 0971128 Enable ABL first
//->w				rtdf_setBits(IDOMAIN_ABL_CTRL_VADDR, _BIT31);


//->w				CStorageUpdateStructData();    //here update data immediatly
//->w				CStorageUpdateToFlash();
        }
        break;
    };
    return _FALSE;
}

/**
 * modestate_init_src
 * This API is used to initial source
 *
 * @param [input]:  <void>		{ void }
 * @return None
 *
 */
static void modestate_init_src(void)
{
    // Set Background color to Blue
    flow_display_BackgroundColor(0, 0, 0x00);//Tim 0325

    // Force to Background avoid to show dirty and set color to blue
    flow_display_ForceBackground(_TRUE);

    // Disable channel
    flow_mode_DisableChannelProc();

    // Do release fuction in unused source
    flow_source_Release();

    // get the new input source from user
    info->input_src = stUISignalSource;

    RTD_Log(LOGGER_INFO, "\n Initial State: src:%x\n", (UINT32)info->input_src);

    /* Select input source mux in Video */
    pcb_set_Video_src_mux(GET_INPUTSOURCE_TYPE());

    /* Select input source mux in Audio */
    pcb_set_Audio_src_mux(GET_INPUTSOURCE_TYPE());

    // Clear status flag in disp_info
    info->disp_status = 0;

    //flow_audio_Setmute(_ENABLE, SCALER, (ADC_CH_IDX)(ADC_CH_L|ADC_CH_R));

    /* InitSrc callback */
    flow_source_Init();
}

static void modestate_set_audio_unmute(void)
{
#ifdef _VIDEO_TV_SUPPORT
    ATV_SOUND_INFO	t_sound_info;
    UINT8 soundsystem;
    if (!tvAutoScanFlag)
    {
//		if (GET_INPUTSOURCE_TYPE() == _SRC_HDMI){//improve hdmi audio in PIP swap 20071123 kist
//			drvif_Hdmi_EnableAudioOutput();
//		}

        if (GET_INPUTSOURCE_TYPE() ==_SOURCE_TV)  //only set on tv
        {
            Audio_AtvGetSoundStd(&t_sound_info);
            if (t_sound_info.sound_std == ATV_SOUND_STD_UNKNOWN)
            {
//-->W??				soundsystem = GET_SOUND_SYSTEM();
                //frank@0516 mark soundsystem = _SOUND_DK;
                RTD_Log(LOGGER_DEBUG, "Set tv audio stand %x\n",(UINT32)soundsystem);
                flow_audio_Setsoundmode(soundsystem);
            }
        }
        //mark by frank@0520 if(drvif_module_vdc_CheckHVisLocked())
        //if (GET_INPUTSOURCE_TYPE() !=_SOURCE_TV) //unmute in check mode when source is TV
        if (!GET_AUDIO_MUTE())
            flow_audio_Setmute(_DISABLE, SCALER, (ADC_CH_IDX)(ADC_CH_L|ADC_CH_R));

        RTD_Log(LOGGER_INFO, "UnMute in fw_modestate_process...\n");
    }
#else
	if (!GET_AUDIO_MUTE())
	{
		if(!bearphone)
			flow_audio_Setmute(_DISABLE, SCALER, (ADC_CH_IDX)(ADC_CH_L|ADC_CH_R));
		else
		{
			flow_audio_Setmute(_ENABLE, SCALER, (ADC_CH_IDX)ADC_CH_C );
		}
	}

	RTD_Log(LOGGER_INFO, "UnMute in fw_modestate_process...\n");

#endif
}

static bit modestate_set_framesync(void)
{
    UINT8 result=0;

    RTD_Log(LOGGER_INFO, "\n Set DisplayTiming State: state = %x\n", (UINT32)info->state);
    RTD_Log(LOGGER_INFO, " failcount = %x\n", (UINT32)info->FailureCount);

//#ifndef CONFIG_ENABLE_RTD_LOG//tommy add for prevent hdmi error when disable log output
    if (GET_INPUTSOURCE_TYPE() == _SOURCE_HDMI)
    {
#if DEBUG_OFF
			if(ucUartPath == 0)
				fw_timer_event_DelayXms(70);
			else
				fw_timer_event_DelayXms(30);
		
#else
#ifdef CONFIG_ENABLE_RTD_LOG
					fw_timer_event_DelayXms(30);
#else
					fw_timer_event_DelayXms(70);//prevent hdmi detect error when disable log output
#endif
#endif
        flow_timer_Reset_WatchDog();
    }
//#endif

    // CSW+ 0970327 Compare VCount for Off-line & On-line MS to prevent image error
    if (!flow_mode_CheckSignalStable())
    {
        // Force to background
        /*drv_scalerdisplay_set_bg_color(0,0,0xff);
        diable by juney 20110104 for chang timing blue background*/
        RTD_Log(LOGGER_LEVEL,"\n Tim bg4 \n");
        drv_scalerdisplay_set_bg_color(0,0,0x00);
	 flow_display_ForceBackground(_TRUE);

        // Do disable idomain
        flow_mode_DisableChannelProc();

        info->state = _MODE_STATE_SEARCH;

        if (GET_INPUTSOURCE_TYPE()!=_SOURCE_VGA)
            fw_flow_message_push(_FL2FL_MAIN_STATE_PERIODIC_EVENT);
        else// vga
            fw_flow_message_push(_UI2FL_MAIN_INIT_SRC_EVENT);//tommy add 091214,prevent VGA online measure timeout

        info->FailureCount = 0;
        VGADetecSyncTypeMode = SEPARATE_MODE;

        return _FALSE;
    }

    //COsdFxDisableOsd();
    //COsdCoreExitOsd();

    //frank@1011 FRC with frame sync timing so must do calculation IVS2DVS
    flow_display_Handler();

    // Set in the scale up
    flow_color_ultrazoom_SetScalingUp();

    //Do the color related setting for main channel and sub channel
    flow_color_Handler();

    // added by Jerry 20070215 for video quality setting from different source
    flow_color_Video_Quality_Handler();

    // VIP for osd related settings
    COsdCtrlSetStorageValue();

    // Enter FrameSync Setup
    //-->w	if(GET_INPUTSOURCE_TYPE() == _SRC_VGA) {	// weihao 960308
    //-->w		fw_scaler_update_mode_para(); //update mode parameters after adjust IVS2DVSDelay
    //-->w	}

    // set display FIFO for Frame sync
    //-->w	drvif_memory_set_fs_display_fifo();
    result = flow_framesync_do();
    if (result != 0) // Frame sync failed
    {
        // Set Timing mode to Free Run
        flow_display_FreeRun_Mode();
        // For ESD protection,  D domain software reset
        if (result == 3)
        {
            drv_scaler_idomain_reset();
        }
        if (GET_INPUTSOURCE_TYPE() == _SOURCE_TV)
        {
            if (result == 3)
            {
                fw_flow_message_push(_FL2FL_MAIN_STATE_PERIODIC_EVENT);
                return _FALSE;
            }
            else
            {
                for (result = 0; result < 30; result ++)
                {
                    if (!drv_video_vdc_CheckHVisLocked())
                        break;
                    fw_timer_event_DelayXms(10);
                }
            }
        }
        if (!((result < 30)&&(GET_INPUTSOURCE_TYPE() == _SOURCE_TV)))
        {
            if (info->FailureCount > 0)
            {

                // Force to background
                drv_scalerdisplay_set_bg_color(0,0,0xff);
                flow_display_ForceBackground(_TRUE);

                // Do disable idomain
                flow_mode_DisableChannelProc();

                fw_flow_message_push(_FL2FL_MAIN_STATE_PERIODIC_EVENT);

                info->FailureCount = 0;
            }
            else
            {
                fw_flow_message_push(_FL2FL_MAIN_SET_FRAMESYNC_SIGNAL);
                info->FailureCount++;
            }
            return _FALSE;
        }
        /* Why ???
        		if(GET_INPUTSOURCE_FROM() == _SRC_FROM_TMDS){
        			//drvif_Hdmi_SetIRQ(_DISABLE);//disable HDMI interrupt 1021 kist
        			if(HDMI_AVMute_Flag)//detect HDMI, when receive unexpected AV_Mute
        				HDMI_AVMute_Flag = 0;
        		}
        */

    }

    // FrameSync Success
    //flow_framesync_Success_Setting();

    info->FailureCount = 0;
#ifdef CONFIG_SCART_AUTO_SWITCH
    if ((GET_INPUTSOURCE_TYPE() != _SOURCE_TV)&&(GET_INPUTSOURCE_TYPE() != _SOURCE_FULL_SCART))
#else
    if (GET_INPUTSOURCE_TYPE() != _SOURCE_TV)
#endif
        drv_scaler_enable_fs_wdg(_TRUE);
//	flow_scaler_Enable_Framesync_WatchDog();
    /*
    	if(!(GET_INPUTSOURCE_TYPE() == _SRC_TV && GET_TVBLUESCREEN_TYPE()==_FALSE))	// weihao 960308
    		fw_scaler_enable_fs_wdg(info->display, _TRUE);
    	else
    		fw_scaler_enable_fs_wdg(info->display, _FALSE);
    */

    return _TRUE;
}

static void modestate_set_display_on(void)
{
    UINT8 ret=0;
    UINT8 CurChn;

    // CSW+ Reset YPbPr 1080p/576p ICode value
    drv_syncproc_ResetICodeSetting(0x01);

    //clear Frame sync status flag
    flow_framesync_Clear_StatusFlag();
    flow_framesync_Clear_LineBufferFlag();

    /*	why???
    	if(GET_INPUTSOURCE_FROM() == _SRC_FROM_TMDS) {
    		//drvif_Hdmi_SetIRQ(_DISABLE);//disable HDMI interrupt 1021 kist
    		if(drvif_Hdmi_AuthenCheck()) {//detect HDMI hdcp flag change?
    			RTD_Log(LOGGER_INFO, "HDMI_AVMute_Flag_00=%d\n",HDMI_AVMute_Flag);
    		}

    		//When HDMI_AVMute_Flag ON,must open AVMute watchdog again
    		if (HDMI_AVMute_Flag) {
    			RTD_Log(LOGGER_INFO, "HDMI_AVMute_Flag_01=%d\n",HDMI_AVMute_Flag);
    			HDMI_AVMute_Flag = 0;
    			//rtdf_maskl(HDMI_HDMI_WDCR0_ADDR ,~AV_WD_AVMute,AV_WD_AVMute); 1021 kist
    		}
    	}
    */

    //frank@0425 below section to decide FRC timing or Frame sync timing
    // Must switch to framesync in sparrow
    //	modestate_set_display_timing(ret);
#ifdef CONFIG_ENABLE_FHFVBug
    if (GET_INPUTSOURCE_TYPE() == _SOURCE_TV)
        //$$ScalerTimer_ActiveTimerEvent(SEC(0.03), ModeState_CheckTVFHFVEvent);// need to check every 20~30ms
        //------------------
        // modify by linmc, to prevent messageQ overflow
        //------------------
        fw_timer_event_ReactiveTimerEvent(300, timerevent_CheckTVFHFVEvent);// need to check every 20~30ms
#endif

    //flow_scaler_Enable_Measure_Watchdog();
    // Add check on-line measure
#ifdef CONFIG_SCART_AUTO_SWITCH
    if ((GET_INPUTSOURCE_TYPE() != _SOURCE_TV)&&(GET_INPUTSOURCE_TYPE() != _SOURCE_FULL_SCART))
#else
    if (GET_INPUTSOURCE_TYPE() != _SOURCE_TV)
#endif
        drv_scaler_enable_measure_wdg(_TRUE);

    // Send msg info ui task to store color gamma parameter
    //fw_ui_message_push(_FL2UI_SET_MAIN_COLOR_CONFIG_MESSAGE);
    // [Code Sync][CSW][0980806][1] CSW+ 0980806
    //drv_adc_YPbPr_Auto_Phase();

    // [Code Sync][CSW][0980806][1] End
    // setting too fast, so we delay 1 vsync for data ready
    WaitFor_DEN_STOP();
    WaitFor_DEN_STOP();
    drv_adc_YPbPr_Auto_Phase();
    RTD_Log(LOGGER_LEVEL,"\n Tim bg5 \n");

    // Disable background bacground color is black
    flow_display_ForceBackground(_FALSE);
    drv_scalerdisplay_set_bg_color(0,0,0);

    #ifdef _VIDEO_TV_SUPPORT
    CurChn = GET_TV_CHN_DATA(_CUR_CHN_TV);
    if (GET_INPUTSOURCE_TYPE() == _SOURCE_TV)
    {
        if (drv_video_vdc_CheckHVisLocked() )
        {
            if ((ucVdc_Curr_Mode == ZSECAML)&&(GET_CHANNEL_SOUNDSYSTEM(CurChn) == ATV_SOUND_STD_AUTO))
            {
                TVSetAudioStandard(ATV_SOUND_STD_NICAM_L);
            }
            modestate_set_audio_unmute();
        }
    }
    else
    #endif
    {
        //frank@0425 unmute audio
        //if(!bearphone)
        	modestate_set_audio_unmute();
    }


    // when source is sub or TV auto scan, don't display the input source info
    if (!tvAutoScanFlag)
    {

        if (GET_INPUTSOURCE_TYPE()==_SOURCE_VGA)
        {
            //COsdRecallValue_VGA();
            RTD_Log(LOGGER_INFO, "GET_AUTO_ADJUST_DONE() == %x\n",(UINT32)GET_AUTO_ADJUST_DONE());
            if ((GET_AUTO_ADJUST_DONE())||(GET_INPUT_MODE()==_MODE_USER_MODE)
                    ||((GET_INPUT_MODE()>=_MODE_480I)&&(GET_INPUT_MODE()<=_MODE_1080PM60))
                    ||(GET_INPUT_MODE() == _MODE_640x350_70HZ)
                    ||(GET_INPUT_MODE() == _MODE_640x400_70HZ)
                    ||(GET_INPUT_MODE() == _MODE_720x400_70HZ)
                    ||(GET_INPUT_MODE() == _MODE_640x480_60HZ))
            {
               RTD_Log(LOGGER_INFO, "GET_AUTO_ADJUST_DONE() == %x\n",(UINT32)GET_AUTO_ADJUST_DONE());

              // fw_ui_message_push(_DISPLAY_MESSAGE);
              //   fw_ui_message_push(_EXIT_KEY_MESSAGE);
            }
            else
	            fw_timer_event_ActiveTimerEvent(SEC(0.01), timerevent_table_AutoAdjust_event);
			
              //  fw_flow_message_push(_FL2FL_ADC_AUTOADJUST_START);
        }
		
        else
        {
#ifdef MODIFY_ONLYFOR_UICHR
            if (((!GET_MP_ON()&&(info->display ==_MAIN_DISPLAY))||(GET_MP_ON()&&(info->display ==_SUB_DISPLAY)))
                    &&(IsDisplayRatioChange != _TRUE))
                fw_ui_message_push(_DISPLAY_MESSAGE);
            else
                IsDisplayRatioChange = _FALSE;
#else
		if(DisplayRatioFlag)
		{
			fw_ui_message_push(_DISPLAY_RATIO_MESSAGE);
			DisplayRatioFlag = 0;
		}
		else
			;//fw_ui_message_push(_DISPLAY_MESSAGE);
           // fw_ui_message_push(_EXIT_KEY_MESSAGE);
#endif
        }
        

        if ( (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)||(GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR) ) //tommy modify 081230
        {
            info->state = _MODE_STATE_ACTIVE;
        }
    }
}


/**
 * modestate_search_process_msg
 * This API is used to search mode in signal source
 *
 * @param [input]:  <void>		{ void }
 * @return None
 *
 */
static void modestate_search_process_msg(void)
{
    UINT8 xdata detect_res;
    /* for search state */
    switch (fModestate_Msg)
    {
    case _FL2FL_MAIN_STATE_PERIODIC_EVENT:
        // Remove message repeatedly
        fw_flow_message_clearallType(fModestate_Msg);

        // Reset the parameter in mode
        flow_mode_ResetMode();

        // Increment the count of try error
        info->FailureCount = (info->FailureCount + 1) % stflowbData.fSEND_MSG_AFTER_DETECT_FAILURE_TIMES;

        /* Call detect mode function in signal */
        detect_res = flow_source_DetectMode();
         //RTD_Log(LOGGER_INFO, "\n detect_res============================ == %x\n",(UINT32)detect_res);
        // RTD_Log(LOGGER_INFO, "\n detect_res======enter power saving====================== == %x\n",(UINT32)detect_res);
       if ( (detect_res == _MODE_NOSIGNAL) ||(detect_res == _MODE_NOCABLE) )//change by juney 20110106
        {
            if (flow_UsePowerSaving()&&(IsActivePowerSavingEvent == 0))
            {
                IsActivePowerSavingEvent = 1;
				
				//fw_timer_event_CancelTimerEvent(timerevent_table_power_saving_event);
				fw_timer_event_ReactiveTimerEvent(SEC(stflowbData.nPowerSaving_Parameter),timerevent_table_power_saving_event);
            }
        }

        if (detect_res == _MODE_NOSIGNAL)
        {
            if (info->FailureCount > 2)
            {
                //fw_ui_message_push(_FL2UI_MAIN_NO_SIGNAL);//Hardy20091218
              
                if(1)//(GET_INPUTSOURCE_INDEX() != _SOURCE_INDEX_HDMI0)
                	{
                	//RTD_Log(LOGGER_INFO, "\n melon123333============================ == %x\n",(UINT32)detect_res);
                	fw_timer_event_ActiveTimerEvent(SEC(1.8),timerevent_table_no_signal_event);
                	}
			    else
			    	{
						 if(ucOsdState<_MAINMENU_STATE||ucOsdState>_LIST_MENU_STATE)//melon 20120828
				    fw_timer_event_ActiveTimerEvent(SEC(1.8), timerevent_table_showloading);
			    	}
				
            }
            fw_timer_event_ActiveTimerEvent(100, timerevent_table_Periodic_signal_event);
//				fw_flow_message_push(_FL2FL_MAIN_STATE_PERIODIC_EVENT);
            return;
        }
        else if (detect_res == _MODE_NOSUPPORT)
        {
            fw_timer_event_ActiveTimerEvent(SEC(5),timerevent_table_no_support_event);
            fw_timer_event_ActiveTimerEvent(200, timerevent_table_Periodic_signal_event);
	    RTD_Log(LOGGER_INFO,"send the mode un support msg$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
            return;
        }
        else if (detect_res == _MODE_NOCABLE)
        {
            fw_timer_event_ActiveTimerEvent(SEC(5), timerevent_table_no_cable_event);
            fw_timer_event_ActiveTimerEvent(200, timerevent_table_Periodic_signal_event);
            return;
        }
        else if (detect_res == _MODE_DETECT)
        {
            //RTD_Log(LOGGER_INFO, "detect_res == _MODE_DETECT\n");
            fw_timer_event_ActiveTimerEvent(200, timerevent_table_Periodic_signal_event);
            return;
        }
        RTD_Log(LOGGER_INFO, "detect_res == _MODE_SUCCESS\n");

        //Clear all message after detect Mode OK
       // fw_timer_event_CancelTimerEvent(timerevent_table_no_signal_event);
       // fw_timer_event_CancelTimerEvent(timerevent_table_no_support_event);
       // fw_timer_event_CancelTimerEvent(timerevent_table_no_cable_event);
        //fw_timer_event_CancelTimerEvent(timerevent_table_power_saving_event);
		//fw_ui_message_clearallType(_FL2UI_SHOW_LOGO);
	 	//fw_timer_event_CancelTimerEvent(timerevent_table_showloading);
        //IsActivePowerSavingEvent=0x00;

        // send message to info UI that detect mode ok
        fw_ui_message_push(_SIGNAL_DETECT_OK_MESSAGE);

        /* InProcSetup callback */
        flow_source_Inproc_Setup();

        // Store ivsdelay to IPV_DLY_PRE
        info->IPV_DLY_PRE = drv_vgip_GetVSDelay();

        // Turn on/off color format transform(RGB2YUV or YUV2RGB) in data path
        flow_color_Transform_Format();

        info->FailureCount = 0;

        // we have to remember the original iv2dvs delay and other parameters
        //	It seem to ivs delay in register set to 0, why it must from register, and set to register
        //			info->IPV_DLY_PRE = info->display ? rtdf_readByte0(IDOMAIN_VGIP_VGIP_CHN2_DELAY_VADDR) : rtdf_readByte0(IDOMAIN_VGIP_VGIP_CHN1_DELAY_VADDR);
#if 0
        load_table();
        flow_display_ForceBackground(_FALSE);
#else

        flow_mode_SetVGIP();

        flow_mode_SetIDomain();

        //clear online ms error flag
        flow_mode_ClearOnMeasureflag();
#if 0//(_DCR_MODE == _DCR_TYPE_3)
        drvif_color_DcrInitial();
#endif

    case _FL2FL_MAIN_SET_FRAMESYNC_SIGNAL:
        if (!modestate_set_framesync())  // if fs fail, keep in fs state
            break;

    case _FL2FL_MAIN_SET_DISPLAY_ON:  // if ui not yet notify flow go, keep in this state
        modestate_set_display_on();
        RTD_Log(LOGGER_INFO, "Go to check mode!!!\n");
#endif
        fw_timer_event_CancelTimerEvent(timerevent_table_no_signal_event);
        fw_timer_event_CancelTimerEvent(timerevent_table_no_support_event);
        fw_timer_event_CancelTimerEvent(timerevent_table_no_cable_event);
        fw_timer_event_CancelTimerEvent(timerevent_table_power_saving_event);
		fw_ui_message_clearallType(_FL2UI_SHOW_LOGO);
	 	fw_timer_event_CancelTimerEvent(timerevent_table_showloading);
        IsActivePowerSavingEvent=0x00;
		COsdCoreExitOsd();

        info->state = _MODE_STATE_ACTIVE;
        fw_ui_message_push(_SHOW_OSD_MESSAGE);
		 //RTD_Log(LOGGER_INFO, "MELON-------\n");
//			fw_flow_message_push(_FL2FL_MAIN_STATE_PERIODIC_EVENT);
        fw_timer_event_ActiveTimerEvent(500, timerevent_table_Periodic_signal_event);
        break;
    };
}

/**
 * modestate_check_signal_stable
 * This API is used to check signal whether stable or not
 *
 * @param [input]:	<void>		{ void }
 * @return None
 *
 */
static void modestate_check_signal_stable()
{
	//RTD_Log(LOGGER_LEVEL,"\n Tim check mode \n");
    /* check mode stable */
    if (flow_source_CheckMode())
    {
        /* check input src every 500ms */
#if defined(CONFIG_WSS_SUPPORT)
        check_wss(_TRUE, 25);
#endif
//**********************************************************************************************
//DCR routine------------------------------------------------------------------------------------
        flow_color_DCR();//move to flow_color.c
//**********************************************************************************************
//			fw_flow_message_push(_FL2FL_MAIN_STATE_PERIODIC_EVENT);
		 //RTD_Log(LOGGER_INFO, "MELON-------------3\n");
        fw_timer_event_ActiveTimerEvent(500, timerevent_table_Periodic_signal_event);
    }
    else
    {
        /* mode unstable */
        if (!tvAutoScanFlag)
        {
            //reynold added 061214 for auto scan need to show snow
            RTD_Log(LOGGER_INFO, "Mode unstable in check mode\n");
            Audio_AioSetPathClock(AIO_PATH_SRC_ATV); //Change audio clock to pll_aud when source is unstable.
            drv_scaler_set_display_mode(DISPLAY_MODE_FRC); //set FRC mode
        }

        /*Asen 20071019 modify for Osd MENU can't disable when adjust  blue screen end*/
        flow_audio_Setmute(_ENABLE, SCALER, (ADC_CH_IDX)(ADC_CH_L|ADC_CH_R));

        //[Code Sync][AlanLi][0980403][1]
        //VGA support composite sync
        //VGADetecSyncTypeMode = 0;
        //[Code Sync][AlanLi][0980403][1][end]
        /* diable by juney 20110104
        drv_scalerdisplay_set_bg_color(0,0,0xff);*/
        RTD_Log(LOGGER_LEVEL,"\n Tim bg6 \n");
        drv_scalerdisplay_set_bg_color(0,0,0x00);
        drv_scalerdisplay_ForceBackground(_TRUE);
        flow_mode_DisableChannelProc();
//		fw_timer_event_ActiveTimerEvent(500, timerevent_table_Periodic_signal_event);
        fw_flow_message_push(_FL2FL_MAIN_STATE_PERIODIC_EVENT);
#ifdef CONFIG_TTX_SUPPORT
        fw_ui_message_push(_TTX_EXIT_MESSAGE);
#endif
        fw_ui_message_push(_EXIT_KEY_MESSAGE);

        /* active state -> search state */
        info->state = _MODE_STATE_SEARCH;
        info->FailureCount = 0;
        CLR_INTERLACE_MODE();
        flow_mode_Timing_Change_Init();

    }
}

/**
 * fw_modestate_process
 * This API is used to process mode state message
 *
 * @param [input] :  <msg>		{ Accept message from message queues }
 * @param [output] :
 * @return None
 *
 */
static void modestate_active_process_msg(void)
{
    switch (fModestate_Msg)
    {
        /* for search state */
    case _MODE_MSG_MAIN_SET_VGIP_AFTER_FOUND_MODE_SIGNAL:
        // Force to background
       /* if (GET_INPUTSOURCE_TYPE() != _SOURCE_TV)
            drv_scalerdisplay_set_bg_color(0,0,0xff);
          change by juney 20110104  */
          RTD_Log(LOGGER_LEVEL,"\n Tim bg7 \n");
        drv_scalerdisplay_set_bg_color(0,0,0x00);

        flow_display_ForceBackground(_TRUE);
        flow_mode_SetVGIP();
        flow_mode_SetIDomain();
    case _FL2FL_MAIN_SET_FRAMESYNC_SIGNAL:
        if (modestate_set_framesync())
        {
            modestate_set_display_on();
            fw_ui_message_push(_SHOW_OSD_MESSAGE);
        }
        fw_timer_event_ActiveTimerEvent(500, timerevent_table_Periodic_signal_event);
        break;
        /* for active mode */
    case _FL2FL_MAIN_STATE_PERIODIC_EVENT:
        modestate_check_signal_stable();
        break;
    }
}
#ifdef _VIDEO_TV_SUPPORT
/**
 * modestate_TV_Scan_process_msg
 * This API is used to process mode state message
 *
 * @param [input] :  <void>
 * @return None
 *
 */
static void modestate_TV_Scan_process_msg(void)
{
    switch (fModestate_Msg)
    {
    case _UI2FL_TV_AUTOSCAN_CANCEL:
        info->state = _MODE_STATE_SEARCH;
        CTvAutoScanStop();
        fAutoscan = 0;
        break;
    case _UI2FL_TV_MENUALSCAN_CANCEL:
        //info->state = _MODE_STATE_SEARCH;
        info->state = _MODE_STATE_ACTIVE;
        CTvManualScanStop();
        break;
    case _MODE_MSG_MAIN_SET_VGIP_AFTER_FOUND_MODE_SIGNAL:
        // Force to background
        //if (GET_INPUTSOURCE_TYPE() != _SOURCE_TV)
        //	drv_scalerdisplay_set_bg_color(0,0,0xff);
        //flow_display_ForceBackground(_TRUE);
        flow_mode_SetVGIP();
        flow_mode_SetIDomain();
    case _FL2FL_MAIN_SET_FRAMESYNC_SIGNAL:
        if (modestate_set_framesync())
        {
            modestate_set_display_on();
            fw_ui_message_push(_SHOW_OSD_MESSAGE);
        }
        fw_timer_event_ActiveTimerEvent(500, timerevent_table_Periodic_signal_event);
    case _FL2FL_MAIN_STATE_PERIODIC_EVENT:
        if (tvAutoScanFlag)
            TvAutoScanTask();
        else if (tvManualScanFlag)
            TvManualScanTask();

        fw_flow_message_clearallType(_FL2FL_MAIN_STATE_PERIODIC_EVENT);
        break;
    }
}
#endif
/**
 * modestate_Blocking_process_msg
 * This API is used to process mode state message
 *
 * @param [input] :  <void>
 * @return None
 *
 */
static void modestate_Blocking_process_msg(void)
{
}

/**
 * fw_modestate_process
 * This API is used to process mode state message
 *
 * @param [input] :  <msg>		{ Accept message from message queues }
 * @param [output] :
 * @return None
 *
 */
void COsdCtrlChannelScan(void);
void fw_modestate_Process(void)
{
    // Get next message to do
    fModestate_Msg = fw_flow_message_pop();

    // No do if no message
    if (fModestate_Msg == _MODE_MSG_NONE)
        return;

    // Do the Specific purpose's message
    if (modestate_global_process_msg())
        return;

    // Check whether valid message or not
    if (fModestate_Msg >= _MODE_MSG_MAIN_UNKNOWN)
        return;

    // Specify info pointer to disp_info
    info = &disp_info;

//	RTD_Log(LOGGER_INFO, "Accept msg = %x\n", (UINT32)fModestate_Msg);
    // Accept message for inital source
    if (fModestate_Msg == _UI2FL_MAIN_INIT_SRC_EVENT)
    {
        RTD_Log(LOGGER_INFO, "Accept Init Src event msg\n");
        IsActivePowerSavingEvent = 0;
        fw_timer_event_CancelTimerEvent(timerevent_table_power_saving_event);

        // Clear all _UI2FL_MAIN_INIT_SRC_EVENT avoid to accept repeadly.
        fw_timer_event_CancelTimerEvent(timerevent_table_Periodic_signal_event);

        // Mute on audio
        flow_audio_Setmute(_ENABLE, SCALER, (ADC_CH_IDX)(ADC_CH_L|ADC_CH_R));
        //Audio_Init_Step1();
        //Audio_Init_Step2();
        // Enter Search State
        info->state = _MODE_STATE_SEARCH;

        // Do initial process
        modestate_init_src();

        // Clear the count of try error
        info->FailureCount = 0;

        fw_flow_message_clearallType(_UI2FL_MAIN_INIT_SRC_EVENT);
        fw_flow_message_clearallType(_MODE_MSG_MAIN_SET_VGIP_AFTER_FOUND_MODE_SIGNAL);

        // Specify  _UI2FL_MAIN_INIT_SRC_EVENT  to _UI2FL_MAIN_SEARCH_MODE_EVENT in message
        fModestate_Msg = _FL2FL_MAIN_STATE_PERIODIC_EVENT;
       	//fw_ui_message_push(_DISPLAY_MESSAGE);
        //if(flow_UsePowerSaving())
        //fw_timer_event_ActiveTimerEvent(SEC(stflowbData.nPowerSaving_Parameter),timerevent_table_power_saving_event);

    }

    switch (info->state)
    {
    case _MODE_STATE_SEARCH:
        modestate_search_process_msg();
        break;

    case _MODE_STATE_ACTIVE:
        fw_flow_message_clearallType(fModestate_Msg);
        modestate_active_process_msg();
#ifdef ENABLE_AUTO_SCAN_AFTER_POEWR_ON
        if (!fAutoscan && !fAutoscan1)
            COsdCtrlChannelScan();
#endif
        break;

    case _MODE_STATE_FREEZE:
        // do nothing, skip _MODE_MSG_XXX_DETECT_SIGNAL
        break;

    case _MODE_STATE_TV_SCAN:
    #ifdef _VIDEO_TV_SUPPORT
        modestate_TV_Scan_process_msg();
        fw_flow_message_push(_FL2FL_MAIN_STATE_PERIODIC_EVENT);
    #endif
        break;

    case _MODE_STATE_BLOCKING:
        modestate_Blocking_process_msg();
        break;

    default:
        break;
    };
    if (!fFirstPowerOn)
    {
        fFirstPowerOn = 1;
        fw_timer_event_ActiveTimerEvent(2000, timerevent_table_power_first_event);
    }
}





