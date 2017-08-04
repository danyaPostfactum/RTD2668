/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2009
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is for rtdaccess related functions.
 *
 * @author 	$Author$
 * @date	$Date$
 * @version	$Revision$
 * @ingroup	rtdaccess
 */

/**
 * @addtogroup rtdaccess
 * @{
 */

#ifndef _MESSAGE_H
#define _MESSAGE_H

/*============================ Module dependency  ===========================*/
#include "sparrow.h"

/*===================================  Types ================================*/
enum SYSTEMESSAGE
{
    _SYSTEM_NO_MSG = 0,
    _SYSTEM_POWERSWITCH_MSG = 1,	// Power switch
    _SYSTEM_ENTER_DEBUG_MODE_MSG,	// Enter debug mode
    _SYSTEM_EXIT_DEBUG_MODE_MSG,	// Exit debug mode
    _SYSTEM_TEST_MSG,					// test message
    _SYSTEM_TEST_MSG1,				// test message1
#ifdef CONFIG_CUSTOMER_VERIFY
    _SYSTEM_CHECK_VIP_CHECK1,
    _SYSTEM_CHECK_VIP_CHECK2,
#endif
    _SYSTEM_UNKNOWN_MSG			// Unknown state
};



enum OSDMESSAGE
{
    _OSD_TIMEOUT_MESSAGE = 0x200,
    _SLEEP_TIMEOUT_MESSAGE,
    _SLEEP_MIN_MESSAGE,
    _SLEEP_10SEC_MESSAGE,
    _POWER_SAVING_MESSAGE,
    
    _0_KEY_MESSAGE,
    _1_KEY_MESSAGE,
    _2_KEY_MESSAGE,
    _3_KEY_MESSAGE,
    _4_KEY_MESSAGE,
    
    _5_KEY_MESSAGE,
    _6_KEY_MESSAGE,
    _7_KEY_MESSAGE,
    _8_KEY_MESSAGE,
    _9_KEY_MESSAGE,
    
    _100_KEY_MESSAGE,
    _REPEAT_KEY_MESSAGE,

    _SHOW_BOOT_MESSAGE,	//0x210
//	_SHOW_NOSIGNAL_MESSAGE,
    _DISPLAY_MESSAGE,

    _CHN_INPUT_TIMEOUT,		// 0x212

//#ifdef CONFIG_TTX_SUPPORT
    //_TTX_10MS_NOTIFY_MESSAGE,
    _TTX_EXIT_MESSAGE,
    _TTX_SIGNAL_OK_MESSAGE,
    _TTX_SIGNAL_NG_MESSAGE,
    _TTX_NEXT_SUBPAGE,
//#endif

//	_SCART_FS_NO_SIGNAL_MESSAGE,
//	_SCART_FS_4x3_MESSAGE,
//	_SCART_FS_16x9_MESSAGE,
//	_SCART_FS_HIGH_MESSAGE,
//	_SCART_FS_LOW_MESSAGE,
    _MAINICON_ANIME_STEP1,
    _DISPLAY_RATIO_MESSAGE,
    _SHOW_OSD_MESSAGE,
};

/*================================== Variables ==============================*/

/*================================ Definitions ==============================*/

/*================================== Function ===============================*/

/*============================================================================*/
/**
 * fw_message_init
 * Initial message queues
 *
 * @param <message>	{ initial message queue }
 * @return 		{ void }
 *
 */
void fw_message_Init(void);
void fw_message_para_Init(void);


/*============================================================================*/
/**
 * fw_flow_message_push
 * Push message into state message queues
 *
 * @param <message>	{ message to be placed in messasge queues }
 * @return 			{ void }
 *
 */
void fw_flow_message_push(UINT16 message);

/*============================================================================*/
/**
 * fw_flow_message_pop
 * Get message in state message queue
 *
 * @param <void>		{ void }
 * @return 			{ Get a message in message queues }
 *
 */
UINT16 fw_flow_message_pop(void);

/*============================================================================*/
/**
 * fw_flow_message_clearallType
 * Clear message in State message queue
 *
 * @param <UINT32>		{ message }
 * @return 			{ Get a message in message queues }
 *
 */
void fw_flow_message_clearallType(UINT16 msg);
void fw_flow_message_clearall(void);

/*============================================================================*/
/**
 * ui_message_push
 * Push message into message queues
 *
 * @param <message>	{ message to be placed in messasge queues }
 * @return 			{ void }
 *
 */
void fw_ui_message_push(UINT16 message);
void fw_isr_ui_message_push(UINT16 message);
void fw_isr_IR_message_push(UINT8 message);
void fw_IR_message_push(UINT8 message);
UINT16 fw_ir_message_pop(void);
UINT16 fw_ui_message_pop(void);
void fw_ui_message_clear(void);
void fw_ui_message_clearallType(UINT16 msg);
void fw_ir_message_clear(void);

#endif // #ifndef _MESSAGE_H
