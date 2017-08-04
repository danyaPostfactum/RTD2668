/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2006
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is for timer related functions.
 *
 * @author 	$Author$
 * @date		$Date$
 * @version	$Revision$
 * @ingroup	timer
 */

/**
 * @addtogroup timer
 * @{
 */

/*============================ Module dependency  ===========================*/
#include "timer_event.h"
#include "timer_event_ui_table.h"
#include "message.h"
#include "osdcore.h"
#include "osd_sys_msg.h"
#include "timer_event.h"
#include "ui_ctrl.h"
#include "ap_rtirkey.h"
#ifdef CONFIG_VBI_CC_SUPPORT
#include "osdvbi.h"
#include "closecaption.h"
#endif
#include "rosprintf.h"

/*===================================  Types ================================*/
extern volatile UINT16 xdata fUI_Msg;

/*=================================== function ================================*/

/**
 * This function is for internal message to notify closing OSD.
 * @return none.
 */
void timerevent_ui_table_OSD_timeout_event(void)
{
    fw_ui_message_push(_OSD_TIMEOUT_MESSAGE);
}
void timerevent_ui_table_OK_event(void)
{
	fw_ui_message_push(_OK_KEY_MESSAGE);
}

void timerevent_ui_table_ReadPowerKey(void)
{
	extern UINT8 ucVirtualkey;
	
    if (ui_CheckPowerKey())
    {
        if (ui_CheckPowerKey())
        {
        	
            fw_ui_message_push(_POWER_KEY_MESSAGE);
        }
    }
    else
    {
    	if(ucVirtualkey!= 0)
    		fw_ui_message_push(ucVirtualkey);
    	ucVirtualkey = 0;
	 	
	 	
	 	
    }
    fw_timer_event_ActiveTimerEvent(SEC(0.10), timerevent_ui_table_ReadPowerKey);
}


/**
 *
 * @return none.
 */
void COsdCoreIconAnimationTimerEvent(void)
{
    if (ucOsdState == _MAINMENU_STATE)
    {
        //fw_ui_message_push(_MAINICON_ANIME_STEP1);
        // need to modify otherwise irda's operation is too slow
    }
    fw_timer_event_ReactiveTimerEvent(SEC(4), COsdCoreIconAnimationTimerEvent);
}

void COsdCoreNoSignalEvent(void)
{
    switch (fUI_Msg)
    {
    case _FL2UI_MAIN_NO_SIGNAL:
    case _FREEZE_KEY_MESSAGE:
        break;

    default:
        COsdCoreDispatchMsg();
        break;
    }
}

void COsdCoreNoSupportlEvent(void)
{
    switch (fUI_Msg)
    {
    case _FL2UI_MAIN_NO_SUPPORT:
		break;
    case _FL2UI_MAIN_NO_SIGNAL:
    case _SLEEP_KEY_MESSAGE:
    case _MUTE_KEY_MESSAGE:
    case _SOURCE_KEY_MESSAGE:
    case _DISPLAY_KEY_MESSAGE:
    case _DISPLAY_MESSAGE:
    case _WIDE_KEY_MESSAGE:
    case _VIDEO_KEY_MESSAGE:
    case _AUDIO_KEY_MESSAGE:
    case _MENU_KEY_MESSAGE:
    case _VOLUMNBAR_KEY_MESSAGE:
    case _LEFT_KEY_MESSAGE:
    case _RIGHT_KEY_MESSAGE:
    case _LEFT_IR_MESSAGE:
    case _RIGHT_IR_MESSAGE:
    case _VOLUME_DEC_MESSAGE:
    case _VOLUME_INC_MESSAGE:
    case _FREEZE_KEY_MESSAGE:
    case _CAPTION_KEY_MESSAGE:
    case _TELETEXT_KEY_MESSAGE:
    case _EXIT_KEY_MESSAGE:	
    case _CC_KEY_MESSAGE:
    case _DISPLAY_RATIO_KEY_MESSAGE:
	//case _DISPLAY_RATIO_IR_MESSAGE:	
    case _RETURN_KEY_MESSAGE:
    //RTD_Log(LOGGER_LEVEL,"\n _SHOW_INFO_STATE bCheckCCStatus=%d \n",(UINT32)bCheckCCStatus);
        COsdCoreExitOsd();
        fw_ui_message_push(fUI_Msg);
        //ucOsdState = _IDLE_STATE;
        break;

    case _JUMP_KEY_MESSAGE:
    case _UP_KEY_MESSAGE:
    case _DOWN_KEY_MESSAGE:

    case _CHANNEL_DEC_MESSAGE:
    case _CHANNEL_INC_MESSAGE:
    case _0_KEY_MESSAGE:
    case _1_KEY_MESSAGE:
    case _2_KEY_MESSAGE:
    case _3_KEY_MESSAGE:
    case _4_KEY_MESSAGE:
    case _5_KEY_MESSAGE:
    case _6_KEY_MESSAGE:
    case _7_KEY_MESSAGE:
    case _8_KEY_MESSAGE:
    case _9_KEY_MESSAGE:
    case _100_KEY_MESSAGE:
    
    //RTD_Log(LOGGER_LEVEL,"\n _SHOW_INFO_STATE bCheckCCStatus=%d \n",(UINT32)bCheckCCStatus);
            COsdCoreExitOsd();
        fw_ui_message_push(fUI_Msg);
        //COsdCoreInitMainMenu();
        break;

    default:
        break;
    }
}

#ifdef _VIDEO_TV_SUPPORT
/**
 * Channel input timeout event.
 * @return none.
 */
void COsdChnInputTimeoutEvent(void)
{
    fw_ui_message_push(_CHN_INPUT_TIMEOUT);
}
#endif

#ifdef CONFIG_TTX_SUPPORT
void timerevent_ui_table_OSD_NoTT_event(void)
{
    fw_ui_message_push(_TTX_SIGNAL_NG_MESSAGE);
}
#endif

#ifdef CONFIG_VBI_CC_SUPPORT
void COsdEventCloseVbi(void)
{
    CosdVbiDisableShow();
}
#endif

