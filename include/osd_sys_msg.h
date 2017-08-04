/*===========================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2005
 * All rights reserved.
 * ========================================================== */

/*==================== File Description ===================== */
/**
 * @file
 * 	This file is osd event declaration file for osd related functions.
 *
 * @author 	$Author: forster $
 * @date 	$Date: 2006-02-06 14:03:22 +0800 (?Ÿæ?ä¸€, 06 äºŒæ? 2006) $
 * @version $Revision: 2 $
 */

/**
 * @addtogroup osd
 * @{
 */

#ifndef _OSD_SYS_MSG_H
#define _OSD_SYS_MSG_H
/*===================== Module Dependency  ================== */

/*========================== Types ========================== */

typedef enum
{
    _FL2UI_NONE_MESSAGE = 0x0000,
    _AUDIO_START_CONFIG_MESSAGE = 0x0100,  //frank@0428 When video signal active,set audio initial setting
    _FL2UI_SET_MAIN_COLOR_CONFIG_MESSAGE, //frank@0428 after color handler must store some color control parameter
    _FL2UI_SET_SUB_COLOR_CONFIG_MESSAGE, //frank@0428 after color handler must store some color control parameter
    _SIGNAL_DETECT_OK_MESSAGE, //frank@0428 after color handler must store some color control parameter
    _TIME_TO_SLEEP_MESSAGE, //frank@0428 after color handler must store some color control parameter
    _SHOW_NOSIGNAL_POWER_OFF_MESSAGE, // CSW 081006
    _NoSignal_SaverlogoProcess_Message, // CSW 081006
    _FL2UI_TASK_READY, //frank@0513 when flow task wake up,flow send the msg to ui task
    _FL2UI_TASK_ACK_GO, //frank@0513 when flow task receive flow go msg,flow send the msg to ui task
    _FL2UI_AUTO_ADJUST_START, //frank@0513 when flow task receive flow  auto adjust start msg,flow send the msg to ui task
    _FL2UI_AUTO_ADJUST_END, //0x0130//frank@0513 when flow task receive flow auto adjust end msg,flow send the msg to ui task
    _UI2UI_FAKE_EVENT, // for ui to active itself
    _FL2UI_MAIN_NO_SIGNAL,			//0x0113
    _FL2UI_MAIN_NO_CABLE,			//0x0114
    _FL2UI_MAIN_NO_SUPPORT,		//0x0115
    _FL2UI_1S_TIMER_EVENT,		//0x0115
    _FL2UI_VCHIP_BLOCK, // hsliao 20080908, when vchip block
    _FL2UI_VCHIP_UNBLOCK, // hsliao 20080908, when vchip unblock
    _NO_TT_SIGNAL,		// Willy , when tt not support
    _FL2UI_SHOW_LOGO,	//joker show logo
    _FL2UI_LOAD_MAINFONT,	//joker load font
    // It's last message in system osd message, and can't delete
    _OSD_SYS_MESSAGE_END
    //--
} SYS_OSDMESSAGETYPE;

/*======================== Definitions ====================== */


/*=====================  Extern Variables =================== */

/*======================= Extern Functions ================== */

#endif //#ifndef _OSD_SYS_MSG_H
/**
 *
 * @}
 */