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
#ifdef CONFIG_ENABLE_HDMI

/*============================ Module dependency  ===========================*/
#include "signal\sig_hdmi.h"
#include "reg_def.h"
#include "flow_struct.h"
#include "hdmi\hdmifun.h"
#include "scaler_idomain\vgip.h"
#include "ui_ctrl.h"
#include "ap_rtirkey.h"
#include "message.h"
#include "audio\audio_api.h"
#include "hdmi\rbushdmiReg.h"
#include "timer\timer.h"
#include "framesync.h"
#include "osdcore.h"
#include "timer_event_sys_table.h"
#include "i2c\i2c.h"


/*===================================  Types ================================*/
/*================================== Variables ==============================*/
/*================================ Definitions ==============================*/
/*================================== Function ===============================*/
//extern UINT8 xdata fUse10msHDMI;
//void drv_sig_hdmi_Init(StructSourceInput *srcinput_pt)
void COsdCtrlSetAudioRemapVolume(UINT8 osd_vol);


void drv_sig_hdmi_Init(void)
{
	//rtd_maskl(DOWN_SAMPLE_FIFO_SOURCE_SEL_0x3554,~(_BIT18|_BIT19),(_BIT18|_BIT19));
	//fw_timer_event_ActiveTimerEvent(SEC(1), timerevent_table_showloading);
	//ISDB_POWER_ENABLE(1);
    // Enable timer operation    
    drv_timer_Enable(TIMER1, _ENABLE);//For SVN 1911 //bHDMI_isr = 1;  
//	fUse10msHDMI = 1;
	rtd_maskb(SYS_SRST_0_0x2ca0,0x7f, 0);
	fw_timer_event_DelayXms(1);
	rtd_maskb(SYS_SRST_0_0x2ca0,0x7f, 0x80);	
	drvif_Hdmi_InitSrc(0);

	InProcHDMI_Init();

	COsdCtrlSetAudioRemapVolume(99);//jokerxie modified ISDB
}

UINT8 drv_sig_hdmi_DetectMode(void)
{
    UINT32 value;
    value = drvif_Hdmi_DetectMode();
    return (UINT8) value;
}

bit drv_sig_hdmi_CheckMode(void)
{
    UINT8 res;
    if (flow_framesync_status())
        return _FALSE;
    if (flow_linebuffer_status())
        return _FALSE;
    if (GET_SLIDER_ON())	// for osd slider adjust
        return _TRUE;
    res = drvif_Hdmi_CheckMode();
    if (res == _FALSE)
    {
        //frank@0425 move from modestate.c ++
        //frank@0425 push exit msg to AP to initial OSD
        fw_ui_message_push( _EXIT_KEY_MESSAGE);
        //frank@0425 move from modestate.c --
    }

    return res;
}

void drv_sig_hdmi_SetupMode(void)
{
	UINT8 ratio;
//  drvif_mode_enableonlinemeasure();
	ratio=ratio;
    InProcHDMI_Setup();
    fw_scaler_set_vgip(VGIP_SRC_TMDS, VGIP_MODE_ANALOG);
    drv_vgip_set_sample(_ENABLE);//for DI bug when do HSD

    //1221 kist avoid color error,frank@0425 move from modestate.c
    drvif_Hdmi_RgbYuvCheck();
    //frank@0530 mark drvif_mode_enableonlinemeasure();
//  hdmi_decide_main_timing_mode();
/*
	if (GET_OSD_DISPLAY_RATIO() == RATIO_AUTO)
	{
		ratio = drv_hdmi_AFD();
		RTD_Log(LOGGER_DEBUG, "HDMI AFD = %d \n",(UINT32)ratio);
		if (ratio == 1)
			SET_DISPLAY_RATIO(RATIO_4_3);
		else if (ratio == 2)
			SET_DISPLAY_RATIO(RATIO_16_9);
	}
*/
}

void drv_sig_hdmi_Release(void)
{
    // Enable timer operation    
    drv_timer_Enable(TIMER1, _DISABLE);//For SVN 1911 //bHDMI_isr = 0;   
//	fUse10msHDMI = 0;

    // set Arbiter Priority start count and end count
//	rtdf_outl(MDOMAIN_IP_PRICNT_STARTEND_CCDINR_VADDR, 0x10008000);
    //hot plug off
//	CHdmi_HpdAllOff();
    //unmute hdmi mute
    Audio_HwpSetMute(_FALSE, ADC_CH_ALL, ADC_HDMI_MUTE);

	fw_timer_event_CancelTimerEvent(timerevent_table_showloading);


    //change uart pin share let uart log appear
//	appcb_sev_ddc2uart();
	//ISDB_POWER_ENABLE(_ON);
    drvif_Hdmi_Release();
}

/*============================================================================*/
#endif
