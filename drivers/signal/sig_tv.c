/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2006
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is process key and ir input related functions.
 *
 * @author 	$Author$
 * @date		$Date$
 * @version	$Revision$
 * @ingroup	input
 */

/**
 * @addtogroup input
 * @{
 */
 #include "autoconf.h"
#ifdef _VIDEO_TV_SUPPORT
/*============================ Module dependency  ===========================*/
#include "signal\sig_tv.h"
#include "ifd\ifd.h"
#include "tuner\tuner.h"
#include "video\video.h"
#include "scaler\scaler_pll.h"
#include "rosprintf.h"
#include "reg_def.h"
#include "flow_struct.h"
#include "mode.h"
#include "mode_customer.h"
#include "scaler_idomain\vgip.h"
#include "scaler_idomain\syncproc.h"
#include "scaler_idomain\measure.h"
#include "osdcore.h"
#include "osdcontrol.h"
#include "scaler\adjust.h"
#include "modestate.h"
#include "message.h"
#include "rt_storage.h"
#include "storage_system.h"
#include "tv.h"
#include "timer_event.h"
#include "audio\audio_api.h"
#include "flow_audio.h"
#include "timer_event_sys_table.h"
#include "source.h"
//#include "Pcb_ctrl.h"//For Keil Building
#include "pcb\pcb_utility.h"
#include "osdcore.h"
#include "pcb_customer.h"

extern UINT8 xdata IsActivePowerSavingEvent;
extern UINT8 drv_video_GetTvColorStandard(void);//For Keil Building

/*===================================  Types ================================*/
/*================================== Variables ==============================*/
/*================================ Definitions ==============================*/
/*================================== Function ===============================*/
//void drv_sig_tv_Init(StructSourceInput *srcinput_pt)
void drv_sig_tv_Init(void* InitOptions)
{
    // Get input source type from upper firmware
//	if(GET_SOURCE_TYPE(srcinput_pt->src_type) != _SOURCE_TV) {
    // Get error vdc number
//		RTD_Log(LOGGER_ERROR, "Get error value in CAVSVInitial!!\n");
//		return;

//	}
//	UINT32 getVal;
    //pcb_ctrl_TunerPoweron();
    UINT8 CurChn;
	CurChn = GET_TV_CHN_DATA(_CUR_CHN_TV);
	
	//rtd_maskl(DOWN_SAMPLE_FIFO_SOURCE_SEL_0x3554,~(_BIT18|_BIT19),0x00);
    drv_video_PowerOn(_ON);
    drv_video_vdc_SelectInput(VDC_SOURCE_TV, InitOptions);
    drvif_ifd_init();
    drvif_tuner_init(_SIG_MODE_PAL);
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
//	ui_set_channel_number(idx, getVal);
#else
//	LOAD_STORAGE_ARRAY_DATA(StructUserDataChannelSettingsType, 0, freq, getVal);
#endif
//	RTD_Log(LOGGER_ERROR, "Get freqr value : %x\n", (UINT32)getVal);

    //     drv_tuner_SetFreq(0);
    //TvSetChannel(GET_TV_CUR_CHN());
    CTvSetChannel(CurChn);

//	printf( "Set TV channel = %x\n", stTvInfo.CurChn);
//	drv_video_inprocvideo_init();
    tvAutoScanFlag = _FALSE;

//	fw_timer_event_DelayXms(500);		//delay 800mS for test
    RTD_Log(LOGGER_DEBUG,"[CTVInitial]\n");
}

UINT8 drv_sig_tv_DetectMode(void)
{
    UINT8 nMode;
	UINT8 CurChn;

	CurChn = GET_TV_CHN_DATA(_CUR_CHN_TV);
	
    nMode = GET_CHANNEL_COLORSYSTEM(CurChn);

#ifdef CONFIG_IFD_ENABLE
    drvif_ifd_cr_freq_status_check(0);
#endif
    /*	nRet = drv_video_detectmode(&stModeInfo);
    	if (nRet == _MODE_SUCCESS) {
    		info->ucMode_Curr = stModeInfo.ModeCurr;
    		info->disp_status = stModeInfo.status;
    		info->Polarity = stModeInfo.Polarity;
    		info->IHFreq = stModeInfo.IHFreq;
    		info->IVFreq = stModeInfo.IVFreq;
    		info->IHTotal = stModeInfo.IHTotal;
    		info->IVTotal = stModeInfo.IVTotal;
    		info->IPH_ACT_STA_PRE = stModeInfo.IHStartPos;
    		info->IPH_ACT_WID_PRE = stModeInfo.IHWidth;
    		info->IPV_ACT_STA_PRE = stModeInfo.IVStartPos;
    		info->IPV_ACT_LEN_PRE = stModeInfo.IVHeight;
    		RTD_Log(LOGGER_ERROR, "sig_tv.info->IPH_ACT_WID_PRE = %x\n", (UINT32)info->IPH_ACT_WID_PRE);
    		RTD_Log(LOGGER_DEBUG,"sig_tv.info->IPV_ACT_LEN_PRE=%x\n", (UINT32)info->IPV_ACT_LEN_PRE);

    		return _MODE_SUCCESS;
    	}
    */
//	if(nRet != _MODE_SUCCESS && GET_TV_BLUE_SCREEN() == _OFF) {
    if (nMode == ZAUTO)
    {
        fw_timer_event_DelayXms(400);
#ifndef CONFIG_TUNER_SILICON
        nMode = drv_video_GetTvColorStandard();
#else
        nMode = ucVdc_Curr_Mode;
#endif
        RTD_Log(LOGGER_DEBUG,"\nTVColorStandard = %x\n", (UINT32)nMode);
        if (nMode == 0xff)// mode not found, default set to 480i
            nMode = ZNTSC;
        else	 if (nMode == 0)
        {
            nMode = ucVdc_TV_Mode;
            //RTD_Log(LOGGER_DEBUG,"\n[auto]TVColorStandard111 = %x\n", (UINT32)nMode);
        }
    }
    else
    {
        rtdf_setBits(VDC_VCR_OPTION_0x53c4,_BIT3);	// For frame sync
    }
    if ((nMode == ZNTSC) || (nMode == ZNTSC_443) || (nMode == ZPAL_M))
    { // NTSC
        info->ucMode_Curr = _MODE_480I;
        switch (nMode)
        {
        case ZNTSC:
            rtdf_outl(VDC_MANUAL_MODE_0x51c0,VDC_MODE_NTSC);	// Force 525
            break;
        case ZNTSC_443:
            rtdf_outl(VDC_MANUAL_MODE_0x51c0,VDC_MODE_NTSC443);	// Force 525
            break;
        case ZPAL_M:
            rtdf_outl(VDC_MANUAL_MODE_0x51c0,VDC_MODE_PALM);	// Force 525
            break;
        default:
            rtdf_outl(VDC_MANUAL_MODE_0x51c0,VDC_MODE_NTSC);	// Force 525
            break;
        }
        nMode = VDC_FRAME_RATE_60HZ;
        rtdf_clearBits(VDC_VIDEO_CONTROL0_0x5000,_BIT4);		// Force 525
#ifdef CONFIG_ENABLE_FHFVBug
        FieldVTotal = 263; //(525/2)
#endif
        RTD_Log(LOGGER_DEBUG, "480i\n");
    }
    else
    { // PAL
        info->ucMode_Curr = _MODE_576I;
        switch (nMode)
        {
        case ZPAL_I:
            rtdf_outl(VDC_MANUAL_MODE_0x51c0,VDC_MODE_PALI);	// Force 525
            break;
        case ZSECAM:
        case ZSECAML:
        case ZSECAMLA:
            rtdf_outl(VDC_MANUAL_MODE_0x51c0,VDC_MODE_SECAM);	// Force 525
            break;
        case ZPAL_N:
            rtdf_outl(VDC_MANUAL_MODE_0x51c0,VDC_MODE_PALN);	// Force 525
            break;
        default:
            rtdf_outl(VDC_MANUAL_MODE_0x51c0,VDC_MODE_PALI);	// Force 525
            break;
        }
        nMode = VDC_FRAME_RATE_50HZ;
        rtdf_setBits(VDC_VIDEO_CONTROL0_0x5000,_BIT4);	// Force 625
#ifdef CONFIG_ENABLE_FHFVBug
        FieldVTotal = 313; //(625/2)
#endif
        RTD_Log(LOGGER_DEBUG, "576i\n");
    }
    //RTD_Log(LOGGER_DEBUG, "VDC_MANUAL_MODE_0x51c0 = %x\n",rtdf_inl(VDC_MANUAL_MODE_0x51c0));
    //RTD_Log(LOGGER_DEBUG, "VDC_MODE_DETECTION_STATUS_0x51c4 = %x\n",rtdf_inl(VDC_MODE_DETECTION_STATUS_0x51c4));
    //RTD_Log(LOGGER_DEBUG, "VDC_VCR_OPTION_0x53c4 = %x\n",rtdf_inl(VDC_VCR_OPTION_0x53c4));

    fw_timer_event_DelayXms(400);
    drv_measure_offlinemeasure(_OFFLINE_MS_SRC_VDC, ANALOG_MODE_MEASUREMENT, &stModeInfo);
    // Get mode parameter from mode table
    flow_Get_Preset_Table_For_VDC(VDC_SOURCE_TV, nMode, &stModeInfo);
    info->IHTotal = stModeInfo.IHTotal;
    info->IVTotal = stModeInfo.IVTotal;
    info->IPH_ACT_STA_PRE = stModeInfo.IHStartPos;
    info->IPH_ACT_WID_PRE = stModeInfo.IHWidth;
    info->IPV_ACT_STA_PRE = stModeInfo.IVStartPos;
    info->IPV_ACT_LEN_PRE = stModeInfo.IVHeight;
    SET_INTERLACE_IN_INPUT_MODE();
    //drv_video_vdc_YcSeparation();
    // Show snow
    return _MODE_SUCCESS;
//	}
//	return nRet;
}

bit drv_sig_tv_CheckMode(void)
{
#define UNLOCKDB	5
    UINT8 colorSys;
    UINT8 ucCstate;
#ifdef CONFIG_ENABLE_FHFVBug
    UINT8 set_mode_status;
    UINT8 decision_mode_status;
#endif
	UINT8 CurChn;

	CurChn = GET_TV_CHN_DATA(_CUR_CHN_TV);
	
#ifdef CONFIG_IFD_ENABLE
    if (cvideo_tv_channel_scan_is_running)  	//disable for scan
    {
        // Auto Scan 正在執行的時候, 跑下面的流程
        drvif_ifd_cr_freq_status_check(0);
    }
    else
    {
		if (drv_video_vdc_CheckHVisLocked())
		{

            if (!GET_AUDIO_MUTE())
            {
                AUDIO_AMP_MUTE_ENABLE(0);
                Audio_HwpSetMute(_DISABLE, ADC_CH_ALL, ADC_SYS_MUTE);
            }
            else
            {
                Audio_HwpSetMute(_ENABLE, ADC_CH_ALL, ADC_SYS_MUTE);
                fw_timer_event_DelayXms(20);
                AUDIO_AMP_MUTE_ENABLE(1);  // CF 20071023 added, mute AMP
            }
            //rtdf_maskl(DISPLAY_TIMING_CTRL1_0x2000, ~(_BIT26 | _BIT29), _BIT26);

		}
		else 
		{		
			AUDIO_AMP_MUTE_ENABLE(1);
			Audio_HwpSetMute(_ENABLE, ADC_CH_ALL, ADC_SYS_MUTE);
			
			  // CF 20071023 added, mute AMP
			//RTD_Log(LOGGER_LEVEL,"\n Tim unlock \n");
			//flow_audio_Setmute(ADC_MUTE, SCALER, ADC_CH_ALL);
            //rtdf_maskl(DISPLAY_TIMING_CTRL1_0x2000, ~(_BIT26 | _BIT29), 0);
		}
		
        drvif_ifd_cr_freq_status_check(1);
        drvif_ifd_set_cr_state(drv_video_vdc_CheckHVisLocked());
//==============================================================================
//Tim 0324

//==============================================================================
    }
#endif

    //Hardy20091208
    if (GET_AFC_SKIP(CurChn) == 0)
        return _TRUE;

    ap_tv_AFC_Tracking();

    if (drv_video_vdc_CheckHVisLocked())
    {
        Audio_AtvSetFwScanCarrierFreqOffset(_ENABLE);
#ifndef CONFIG_ENABLE_FHFVBug
        drv_video_vdc_YcSeparation();
#endif
        // check c state to 3
        ucCstate = (rtdf_readByte0(VDC_CHROMA_LOOPFILTER_STATE_0x50a0) & 0x0e)>>1;
        if (ucCstate != 3)
        {
            rtdf_maskl(VDC_CHROMA_LOOPFILTER_STATE_0x50a0,0xf1,0x06);	// set cstate to 3
            //VDC_PRINTF(LOGGER_ERROR, "Set C state to 3 @TV\n");
        }
        colorSys = GET_CHANNEL_COLORSYSTEM(CurChn);
        if ( colorSys == ZSECAM)
            rtd_outl(VDC_CHROMA_KILL_0x5094, 0x83);

#ifdef CONFIG_IFD_ENABLE
        if ((colorSys == ZAUTO)&&(!GET_SLIDER_ON()))  // auto mode
        {

            colorSys = drv_video_GetTvColorStandard();
            RTD_Log(LOGGER_DEBUG, "\nColor std = %x\n",(UINT32)colorSys);
            if (colorSys != 0)
            {
                flow_audio_Setmute(ADC_MUTE, SCALER, (ADC_CH_IDX)(ADC_CH_L|ADC_CH_R));
                fw_tv_setvideomode(colorSys, 0);
                if (GET_CHANNEL_SOUNDSYSTEM(CurChn) == ATV_SOUND_STD_AUTO)
                    TVSetAudioStandard(ATV_SOUND_STD_AUTO);
                YcSeparationStatus = VDC_YCSEPARATE_DEFAULT;
                ucVdc_Curr_Mode = colorSys;
                disp_info.state = _MODE_STATE_SEARCH;
                flow_mode_DisableChannelProc();
            }
        }
#endif

#ifdef CONFIG_ENABLE_FHFVBug
        set_mode_status=(UINT8)((rtdf_inl(VDC_MODE_DETECTION_STATUS_0x51c4)&0x70)>>4);
        decision_mode_status=(UINT8)(rtdf_inl(VDC_MODE_DETECTION_STATUS_0x51c4)&0x07);
        if ((ucOsdState != _AUTO_SCAN_STATE)&&(ucOsdState != _MANUAL_SCAN_STATE)&&(set_mode_status==decision_mode_status))
        {
            drv_sig_tv_CheckFVBias();
        }
#endif
        if (!GET_AUDIO_MUTE())
            flow_audio_Setmute(_DISABLE, SCALER, (ADC_CH_IDX)(ADC_CH_L|ADC_CH_R));//Hardy20091204 for mute audio when tv no signal

        if (flow_UsePowerSaving())
        {
            if (IsActivePowerSavingEvent==1)
            {
                IsActivePowerSavingEvent=0;
                //RTD_Log(LOGGER_INFO, "\n++++++++Cancelpower_saving_checkmode000", 0);
                fw_timer_event_CancelTimerEvent(timerevent_table_power_saving_event);
            }
        }

    }
    else   // TV video h,v unlock
    {
        Audio_AtvSetFwScanCarrierFreqOffset(_DISABLE);
        rtd_outl(VDC_CHROMA_KILL_0x5094, 0x07); // for secam(no signal)  c kill setting
#ifdef CONFIG_TTX_SUPPORT
        fw_ui_message_push(_TTX_EXIT_MESSAGE);
#endif
        if (!GET_AUDIO_MUTE() && !drvif_ifd_get_cr_lock()) //<Stanley 2K100107> checking TV status by ifd lock
        {
            flow_audio_Setmute(_ENABLE, SCALER, (ADC_CH_IDX)(ADC_CH_L|ADC_CH_R));//Hardy20091204 for mute audio when tv no signal
        }
        //return _FALSE;//reynold reopen 071020; if here not return false, the TvBlueScreen function will fail

        if (flow_UsePowerSaving())
        {
            if ((IsActivePowerSavingEvent==0)&&(ucOsdState == _IDLE_STATE))
            {
                IsActivePowerSavingEvent=1;
                //RTD_Log(LOGGER_INFO, "\n++++++++Enablepower_saving_checkmode000", 0);
                ResetPowerSavingTime();
                fw_timer_event_ReactiveTimerEvent(SEC(30),timerevent_table_power_saving_event);
            }
        }

    }
    return _TRUE;
}

#ifdef CONFIG_ENABLE_FHFVBug
UINT8 drv_sig_tv_CheckFVBias(void)
{
    UINT16 VHalfTotal;
    UINT8 cur_fs_status;
	UINT8 CurChn;

    //static UINT8 fs_status = 0;
    //static UINT8 fs_count = 0;
    UINT32 Frequency;

    if (!drv_video_vdc_CheckHVisLocked())
        return _TRUE;

    rtdf_setBits(MAIN_DISPLAY_CONTROL_RSV_0x2100,  _BIT18|_BIT19);//write status
    fw_timer_event_DelayXms(40);//test by tyan
    cur_fs_status = rtdf_readByte2(MAIN_DISPLAY_CONTROL_RSV_0x2100)>>2;
    RTD_Log(LOGGER_DEBUG, "\n cur_fs_status:%x\n",(UINT32)cur_fs_status);
    cur_fs_status &= _BIT0|_BIT1;
    if (cur_fs_status >0)
        drv_adjust_fast_get_sync_point();

    //if((cur_fs_status > 0) && (fs_status == cur_fs_status)) {
    //	fs_count++;
    //}
    //else {
    //	fs_status = cur_fs_status;
    //	fs_count = 0;
    //}

    //if(fs_count>20) {
    //	drv_adjust_measureivs2dvsdelay();
    //	fs_count = 0;
    //	fs_status = 0;
    //}

    if (drv_video_vdc_GetVHalfTotalLen(&VHalfTotal))
    {
        //avoid force change color standard, input line(ex. NTSC) < setting(ex.PAL)
        if (FieldVTotal <= (info->IPV_ACT_STA + info->IPV_ACT_LEN))
        {
            return _TRUE;
        }

        if (VHalfTotal >= 0x161) // for fixed video decoder line unexpected bug
            rtdf_maskl(VDC_VSYNC_CNTL_0x5158, ~(_BIT6|_BIT7), _BIT6);
        else
            rtdf_maskl(VDC_VSYNC_CNTL_0x5158, ~(_BIT6|_BIT7), 0);

        if (((info->ucMode_Curr == _MODE_480I) && ((VHalfTotal == 263)||(VHalfTotal == 262))) ||
                ((info->ucMode_Curr == _MODE_576I) && ((VHalfTotal == 313)||(VHalfTotal == 312))))
        {
            if ((FieldVTotal == 263)||(FieldVTotal == 262) ||
                    ((FieldVTotal == 313)||(FieldVTotal == 312)))
            {
                //----------
                // add by linmc for tv judgement,
                // avsv will call this function, 20090105
                //----------
				CurChn = GET_TV_CHN_DATA(_CUR_CHN_TV);
				
#if defined(CONFIG_ENABLE_AV_FHFV)
                if (GET_INPUTSOURCE_TYPE() == _SOURCE_TV)
                {
#ifdef CONFIG_ENABLE_BurstBug
                    Frequency = GET_CHANNEL_FREQ(CurChn);
                    drv_video_vdc_CheckBurst(Frequency);
#endif
                }
                return _TRUE;
#else
#ifdef CONFIG_ENABLE_BurstBug
                Frequency = GET_CHANNEL_FREQ(CurChn);
                drv_video_vdc_CheckBurst(Frequency);
#endif
                return _TRUE;
#endif
            }

            RTD_Log(LOGGER_DEBUG, "\n reset1....................FieldVTotal:%x\n",FieldVTotal);
            info->IPV_ACT_LEN_PRE = tAV_PRESET_TABLE[info->ucMode_Curr - _MODE_480I][4];
            drv_scaler_set_display_mode(DISPLAY_MODE_FRC);
            fw_flow_message_push(_MODE_MSG_MAIN_SET_VGIP_AFTER_FOUND_MODE_SIGNAL);
            FieldVTotal = VHalfTotal;

            drv_measure_offlinemeasure(_OFFLINE_MS_SRC_VDC, ANALOG_MODE_MEASUREMENT, &stModeInfo);
            //FHFV bias dclk
            info->IHFreq = stModeInfo.IHFreq;

            RTD_Log(LOGGER_DEBUG,"info->IHFreq_1:%d\n",info->IHFreq);
            return _FALSE;
        }

        // decrease lines
        if (VHalfTotal <= (FieldVTotal - (FieldVTotal - info->IPV_ACT_STA - info->IPV_ACT_LEN) +2))
        {
            RTD_Log(LOGGER_DEBUG, "\n reset2....................FieldVTotal:%x\n",FieldVTotal);
            info->IPV_ACT_LEN_PRE = VHalfTotal - (FieldVTotal - info->IPV_ACT_LEN)-3; //pre-subtraction
            drv_scaler_set_display_mode(DISPLAY_MODE_FRC);
            fw_flow_message_push(_MODE_MSG_MAIN_SET_VGIP_AFTER_FOUND_MODE_SIGNAL);
            FieldVTotal = VHalfTotal;

            drv_measure_offlinemeasure(_OFFLINE_MS_SRC_VDC, ANALOG_MODE_MEASUREMENT, &stModeInfo);
            //FHFV bias dclk
            info->IHFreq = stModeInfo.IHFreq;

            RTD_Log(LOGGER_DEBUG,"info->IHFreq_2:%d\n",info->IHFreq);
            return _FALSE;
        }

        // when lines increase, redo redo flow for UZU/UZD
        if ((((info->ucMode_Curr == _MODE_576I) &&(VHalfTotal < 313))
                ||((info->ucMode_Curr == _MODE_480I) &&(VHalfTotal < 263)))
                &&(VHalfTotal >= FieldVTotal + (FieldVTotal - info->IPV_ACT_STA - info->IPV_ACT_LEN)))
        {
            info->IPV_ACT_LEN_PRE = VHalfTotal - (FieldVTotal - info->IPV_ACT_LEN)+3;
            drv_scaler_set_display_mode(DISPLAY_MODE_FRC);
            fw_flow_message_push(_MODE_MSG_MAIN_SET_VGIP_AFTER_FOUND_MODE_SIGNAL);
            FieldVTotal = VHalfTotal;
            drv_measure_offlinemeasure(_OFFLINE_MS_SRC_VDC, ANALOG_MODE_MEASUREMENT, &stModeInfo);
            //FHFV bias dclk
            info->IHFreq = stModeInfo.IHFreq;

            RTD_Log(LOGGER_DEBUG,"\n Bias increase reset..................FieldVTotal:%x\n",FieldVTotal);
            return _FALSE;
        }
    }
    return _TRUE;
}
#endif

void drv_sig_tv_SetupMode(void)
{
    rtdf_setBits(VGIP_CHN1_CTRL_0x0a10, _BIT3);

    drv_vgip_set_vgip(VGIP_SRC_ATV, VGIP_MODE_ANALOG);
    drv_vgip_set_sample(_ENABLE);//for DI bug when do HSD

    drv_video_vdc_AVoutEnable(VDC_AVOUT_ENALBE);	// Enable AVOUT
}

void drv_sig_tv_Release(void)
{
    drv_video_vdc_AVoutEnable(VDC_AVOUT_DISABLE);
    drv_video_Release();
    drv_video_reset_HPLL_default();
#ifdef CONFIG_IFD_ENABLE
    fw_timer_event_CancelTimerEvent(timerevent_atv_ifd_chk_agc_var);
#endif
    //pcb_ctrl_TunerPoweroff();
}

void drv_sig_tv_ReSetVGIP(void)
{
    flow_mode_DisableChannelProc();
    SET_COLOR_SYSTEM_CHANGE();
    RTD_Log(LOGGER_DEBUG, "[drv_sig_tv_ReSetVGIP] color system change\n");
}

void drv_sig_tv_SetVGIP(UINT8 mode)
{
    UINT8 ucMode;
    if (drv_measure_offlinemeasure(_OFFLINE_MS_SRC_VDC, ANALOG_MODE_MEASUREMENT, &stModeInfo) == _MODE_NOSIGNAL)
    {	// offlinemeasure fail, init source again
        //fw_flow_message_push(_UI2FL_MAIN_INIT_SRC_EVENT);
        //return;
    }
    if (mode == _MODE_480I)
        ucMode = VDC_FRAME_RATE_60HZ;
    else
        ucMode = VDC_FRAME_RATE_50HZ;
    // Get mode parameter from mode table
    flow_Get_Preset_Table_For_VDC(VDC_SOURCE_TV, ucMode, &stModeInfo);
    info->IHTotal = stModeInfo.IHTotal;
    info->IVTotal = stModeInfo.IVTotal;
    info->IPH_ACT_STA_PRE = stModeInfo.IHStartPos;
    info->IPH_ACT_WID_PRE = stModeInfo.IHWidth;
    info->IPV_ACT_STA_PRE = stModeInfo.IVStartPos;
    info->IPV_ACT_LEN_PRE = stModeInfo.IVHeight;
    disp_info.IHFreq = stModeInfo.IHFreq;
    disp_info.IVFreq = stModeInfo.IVFreq;
    disp_info.ucMode_Curr = mode;
    disp_info.Polarity = stModeInfo.Polarity;  //jj 20080421 HDMI 切 TV 會黑屏
    SET_INTERLACE_IN_INPUT_MODE();
}

/*============================================================================*/
#endif
