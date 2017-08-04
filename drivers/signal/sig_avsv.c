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

/*============================ Module dependency  ===========================*/
#include "signal\sig_avsv.h"
#include "signal\sig_tv.h"
#include "video\video.h"
#include "rosprintf.h"
#include "reg_def.h"
#include "flow_struct.h"
#include "scaler_idomain\vgip.h"
#include "scaler_idomain\syncproc.h"
#include "scaler_idomain\measure.h"
#include "mode_customer.h"
#include "pcb_customer.h"
#include "pcb\pcb_utility.h"
#include "scaler_idomain\adc.h"
#include "timer_event.h"
#include "message.h"
#include "ifd\ifd.h"
#include "tv.h"
#include "audio/audio_api.h"
#include "framesync.h"

/*===================================  Types ================================*/
/*================================== Variables ==============================*/
/*================================ Definitions ==============================*/
/*================================== Function ===============================*/
#ifdef CONFIG_SCART_AUTO_SWITCH
void drv_sig_scart_adc_Init(void)
{
    //// ADC setting for ADC RGB ////
    rtd_maskl(ADC_VBIAS_0x040c, ~_BIT9, 0);
    rtd_outl(ADC_CLOCK_0x0410, 0x00000c01);				// Set ADC_RBG clock to VD
    // ADC CKOAD clock input select[11:10]=11 => for RGB
    rtd_maskl(ADC_CTL_0x0414, 0xF8FFFFFF, 0x00000000);	// ADC Bandwidth : 10M
//	rtd_outl(ADC_DCRESTORE_CTRL_0x0418, 0x00000000);	// disable DC restore
    rtd_maskl(ADC_DCRESTORE_CTRL_0x0418, 0xFFFFFE38, 0x00000000);	// disable DC restore
//	rtd_outl(ADC_CLAMP_CTRL1_0x0420, 0x00200000);		// disable YPP clamp
    rtd_maskl(ADC_CLAMP_CTRL1_0x0420, 0xFFFFFE38, 0x00000000);	// disable YPP clamp
}
#endif
void drv_sig_avsv_Init(UINT8 vdc_src, void* InitOptions)
{
//	UINT8 vdc_src;

    drv_video_PowerOn(_ON);
	//rtd_maskl(DOWN_SAMPLE_FIFO_SOURCE_SEL_0x3554,~(_BIT18|_BIT19),(_BIT18|_BIT19));
	//RTD_Log(LOGGER_INFO, "[avsv_Init] vdc_src == %x\n", (UINT32)vdc_src);
//	RTD_Log(LOGGER_INFO, "[avsv_Init] InitOptions == %x\n", (UINT32)InitOptions);

    // Get input source type from upper firmware
//	vdc_src = GET_SOURCE_TYPE(srcinput_pt->src_type);
	//RTD_Log(LOGGER_INFO, "[avsv_Init] MELON2222 == %x\n", (UINT32)GET_INPUTSOURCE_INDEX());

    if(GET_INPUTSOURCE_INDEX() == _SOURCE_INDEX_AV)
    	{
    		STB_CTRL_A(0);
			STB_CTRL_B(1);
    	}
	if(GET_INPUTSOURCE_INDEX() == _SOURCE_INDEX_AV_2)
		{
			STB_CTRL_A(1);
			STB_CTRL_B(0);
		}
	if(GET_INPUTSOURCE_INDEX() == _SOURCE_INDEX_AV_3)
		{
			STB_CTRL_A(0);
			STB_CTRL_B(0);
		}
    switch (vdc_src)
    {
    case _SOURCE_CVBS:
        vdc_src = VDC_SOURCE_CVBS;
        break;
    case _SOURCE_SV:
        vdc_src = VDC_SOURCE_SV;
        break;
#ifdef CONFIG_SCART_AUTO_SWITCH
    case _SOURCE_FULL_SCART:
        drv_sig_scart_adc_Init();
        vdc_src = VDC_SOURCE_FULL_SCART;
        break;
    case _SOURCE_HALF_SCART:
        drv_sig_scart_adc_Init();
        vdc_src = VDC_SOURCE_HALF_SCART;
        break;
#endif
//		case _SOURCE_SCART_AV:
//			vdc_src = VDC_SOURCE_SCART_AV;
//			break;
//		case _SOURCE_SCART_SV:
//			vdc_src = VDC_SOURCE_SCART_SV;
//			break;
    default:
        // Get error vdc number
        RTD_Log(LOGGER_ERROR, "Get error value in CAVSVInitial!!\n");
        return;
    };

    // Initial input source path in VDC
//	drv_video_vdc_SelectInput(vdc_src, srcinput_pt->InitOptions);
    drv_video_vdc_SelectInput(vdc_src, InitOptions);
    RTD_Log(LOGGER_DEBUG, "CAVSVInitial done!!\n");
}

UINT8 drv_sig_avsv_DetectMode(void)
{
    UINT8 nRet;
    #ifdef CONFIG_SCART_AUTO_SWITCH
	UINT8 CurChn;
	#endif
    //fw_timer_event_DelayXms(300);
#ifdef CONFIG_SCART_AUTO_SWITCH
    if (GET_INPUTSOURCE_TYPE() == _SOURCE_FULL_SCART)
        fw_timer_event_DelayXms(1300);
#endif
    nRet = drv_video_detectmode(&stModeInfo);
    if (nRet == _MODE_SUCCESS)
    {
        info->ucMode_Curr = stModeInfo.ModeCurr;
        info->disp_status = stModeInfo.status;
        info->Polarity = stModeInfo.Polarity;
        info->IHFreq = stModeInfo.IHFreq;
        info->IVFreq = stModeInfo.IVFreq;
        info->IHTotal = stModeInfo.IHTotal;
        info->IVTotal = stModeInfo.IVTotal;
        info->IPH_ACT_STA_PRE= stModeInfo.IHStartPos;
        info->IPH_ACT_WID_PRE= stModeInfo.IHWidth;
        info->IPV_ACT_STA_PRE= stModeInfo.IVStartPos;
        info->IPV_ACT_LEN_PRE= stModeInfo.IVHeight;
        return _MODE_SUCCESS;
    }
    else
    {
#ifdef CONFIG_SCART_AUTO_SWITCH
		CurChn = GET_TV_CHN_DATA(_CUR_CHN_TV);

        if (GET_INPUTSOURCE_TYPE() == _SOURCE_FULL_SCART)
        {
            nRet = GET_CHANNEL_COLORSYSTEM(CurChn);
            if (nRet == ZAUTO)
            {
#ifndef CONFIG_TUNER_SILICON
                nRet = drv_video_GetTvColorStandard();
                if (nRet == 0)
                    nRet = ucVdc_Curr_Mode;
                else	// mode not found, default set to 480i
                    nRet = ZNTSC;
#else
                nRet = ucVdc_Curr_Mode;
#endif
            }

            if ((nRet == ZNTSC) || (nRet == ZNTSC_443) || (nRet == ZPAL_M))
            { // NTSC
                nRet = VDC_FRAME_RATE_60HZ;
                info->ucMode_Curr = _MODE_480I;
#ifdef CONFIG_ENABLE_FHFVBug
                FieldVTotal = 263; //(525/2)
#endif
                RTD_Log(LOGGER_DEBUG, "480i\n");
            }
            else   // PAL
            {
                nRet = VDC_FRAME_RATE_50HZ;
                info->ucMode_Curr = _MODE_576I;
#ifdef CONFIG_ENABLE_FHFVBug
                FieldVTotal = 313; //(625/2)
#endif
                RTD_Log(LOGGER_DEBUG, "576i\n");
            }
            drv_measure_offlinemeasure(_OFFLINE_MS_SRC_VDC, ANALOG_MODE_MEASUREMENT, &stModeInfo);
            // Get mode parameter from mode table
            flow_Get_Preset_Table_For_VDC(VDC_SOURCE_TV, nRet, &stModeInfo);
            info->IHTotal = stModeInfo.IHTotal;
            info->IPH_ACT_STA_PRE = stModeInfo.IHStartPos;
            info->IPH_ACT_WID_PRE = stModeInfo.IHWidth;
            info->IPV_ACT_STA_PRE = stModeInfo.IVStartPos;
            info->IPV_ACT_LEN_PRE = stModeInfo.IVHeight;
            SET_INTERLACE_IN_INPUT_MODE();
            drv_video_vdc_YcSeparation();
            nRet = _MODE_SUCCESS;
        }
#endif
    }
    return nRet;
}

UINT8 drv_sig_avsv_CheckMode(void)
{
    UINT8 res;
    Audio_HwpInputNoiseReduction();
    res = drv_video_checkmode();
    if ((res != _TRUE)||flow_framesync_status()||flow_linebuffer_status())
    {
#ifdef CONFIG_SCART_AUTO_SWITCH
        if ((GET_INPUTSOURCE_INDEX() == _SOURCE_INDEX_SCART_0)&& (res == 0))
        {
            return _TRUE;
        }
#endif
#ifdef CONFIG_TTX_SUPPORT
        fw_ui_message_push(_TTX_EXIT_MESSAGE);
#endif
        // restore to default
        rtdf_outl(VDC_SERRATION_TH_0x5168, 0xB0);
        drvif_module_vdc_Set_Hstate(VDC_HSTATE_RESET);
        rtdf_outl(VDC_HBLANK_START_0x5128, 0xd6);
        res = _FALSE;
    }

#ifdef CONFIG_ENABLE_FHFVBug
#ifdef CONFIG_SCART_AUTO_SWITCH
    if (drv_video_vdc_CheckHVisLocked()&&(GET_INPUTSOURCE_TYPE() != _SOURCE_FULL_SCART))
#else
    if (drv_video_vdc_CheckHVisLocked())
#endif
    {
        drv_sig_tv_CheckFVBias();
    }
#endif

    return res;
}

void drv_sig_avsv_SetupMode(void)
{
    RTD_Log(LOGGER_DEBUG, "CAVSVInProcSetup !\n");
//	drv_video_inprocvideo_setup();
    drv_vgip_set_vgip(VGIP_SRC_ATV, VGIP_MODE_ANALOG);
    drv_vgip_set_sample(_ENABLE);//for DI bug when do HSD
    drv_video_vdc_user_ckill_mode(VDC_USER_CKILL_AUTO_HW);
    drv_video_vdc_SetCKillValue(0x07);	//reset ckill value to default
    drv_video_vdc_AVoutEnable(VDC_AVOUT_ENALBE);	// Enable AVOUT
#ifdef CONFIG_SCART_AUTO_SWITCH
    drv_video_SCART_RGB_set_gain();
#endif

#if 0 //def CONFIG_ENABLE_FHFVBug
    //frank@0529 fix me why do the below code
    if (GET_INPUTSOURCE_TYPE() == _SOURCE_CVBS)
    {
        FieldVTotal = (((rtd_inl(VDC_MEASURE_FIRST_V_HI_0x53cc) &0x03) <<8) | ((rtd_inl(VDC_MEASURE_FIRST_V_LOW_0x53d0) &0xFF)));
        if ((FieldVTotal < (rtd_inl(VDC_MEASURE_SECOND_V_HI_0x53d4) &0x03) <<8) | (rtd_inl(VDC_MEASURE_SECOND_V_LOW_0x53d8) &0xFF))
            FieldVTotal = (((rtd_inl(VDC_MEASURE_SECOND_V_HI_0x53d4) &0x03) <<8) | ((rtd_inl(VDC_MEASURE_SECOND_V_LOW_0x53d8) &0xFF)));
    }
#endif
}
#define TV_POWER_CODE		0x10

void drv_sig_avsv_Release(void)
{
  #if 0//CONFIG_TU300
		if(GET_INPUTSOURCE_INDEX()== _SOURCE_INDEX_AV_1)
		 Send_IR_DTV((((~TV_POWER_CODE)<<8)|TV_POWER_CODE));
  #endif
    drv_video_vdc_AVoutEnable(VDC_AVOUT_DISABLE); // disable AVout
    drv_video_Release();
}

/*============================================================================*/
