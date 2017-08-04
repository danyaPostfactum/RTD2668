/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2006
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is for message related functions.
 *
 * @author 	$Author$
 * @date	$Date$
 * @version	$Revision$
 * @ingroup	message
 */

/**
 * @addtogroup message
 * @{
 */
#include "reg52.h"
#include "message.h"
#include "rosprintf.h"
#include "modestate.h"
#include "power.h"
#include "ui_ctrl.h"
/*================================ Definitions ==============================*/
#define CONFIG_STATE_MESSAGE_QUEUE_SIZE 	8 // Define the size of message queue
#define CONFIG_UI_MESSAGE_QUEUE_SIZE 		8 // Define the size of message queue
#define CONFIG_IR_MESSAGE_QUEUE_SIZE 		8 // Define the size of message queue

/*================================== Variables ==============================*/
// message queue in State
volatile UINT16 xdata STATE_MESSAGEQUEUES_BUF[CONFIG_STATE_MESSAGE_QUEUE_SIZE];
volatile UINT8 xdata InStateQueue, OutStateQueue;

// message queue in UI
volatile UINT16 xdata UI_MESSAGEQUEUES_BUF[CONFIG_STATE_MESSAGE_QUEUE_SIZE];
volatile UINT8 xdata InUIMsgQueue =0, OutUIMsgQueue =0;

// message queue in Remote
volatile UINT8 xdata IR_MESSAGEQUEUES_BUF[CONFIG_IR_MESSAGE_QUEUE_SIZE];
volatile UINT8 xdata InIRMsgQueue, OutIRMsgQueue;

/*================================== Function ===============================*/


/*============================================================================*/
/**
 * fw_flow_message_init
 * Initial message queue for state
 *
 * @param <void>		{ void }
 * @return 			{ void }
 *
 */

void fw_message_para_Init(void)
{
    OutIRMsgQueue = 0;
    for (InIRMsgQueue = 0; InIRMsgQueue <= CONFIG_IR_MESSAGE_QUEUE_SIZE; InIRMsgQueue++)
        IR_MESSAGEQUEUES_BUF[InIRMsgQueue] = 0;
    InIRMsgQueue = 0;

    // Clear state message queue for sclaer system
    fw_flow_message_clearall();

    // Clear ui message queue for ui system
    fw_ui_message_clear();
}

void fw_message_Init(void)
{
    fw_message_para_Init();

    RTD_Log(LOGGER_INFO, "\nIt had created  message queues for state!***** \n");
}

/*============================================================================*/
/**
 * fw_flow_message_push
 * Push message into state message queues
 *
 * @param <message>	{ message to be placed in messasge queues }
 * @return 			{ void }
 *
 */
void fw_flow_message_push(UINT16 message)
{
    if (message != _FL2FL_POWER_KEY_MESSAGE)
    {
        STATE_MESSAGEQUEUES_BUF[InStateQueue] = message;
        InStateQueue = (InStateQueue + 1) % CONFIG_STATE_MESSAGE_QUEUE_SIZE;
        //frank@0506 add avoid message queue full and can not be pop
        if (InStateQueue == OutStateQueue) //Queue is full
        {
            if (InStateQueue == 0)
                InStateQueue = CONFIG_STATE_MESSAGE_QUEUE_SIZE-1;
            else
                InStateQueue = InStateQueue-1;
        }
    }
    else
    {
        // The highest priority to proccess
        if (!GET_FIRST_POWER_SWITCH())
            SET_POWERSWITCH();
        fw_flow_message_clearall();
        STATE_MESSAGEQUEUES_BUF[InStateQueue] = message;
        InStateQueue = (InStateQueue + 1) % CONFIG_STATE_MESSAGE_QUEUE_SIZE;
    }
}

/*============================================================================*/
/**
 * fw_flow_message_pop
 * Get message in State message queue
 *
 * @param <void>		{ void }
 * @return 			{ Get a message in message queues }
 *
 */
UINT16 fw_flow_message_pop(void)
{
    UINT16 xdata msg;

    if (OutStateQueue == InStateQueue)
        return 0;

    msg = STATE_MESSAGEQUEUES_BUF[OutStateQueue];
    STATE_MESSAGEQUEUES_BUF[OutStateQueue] = 0;
    OutStateQueue = (OutStateQueue + 1) % CONFIG_STATE_MESSAGE_QUEUE_SIZE;

    return msg;
}

void fw_flow_message_clearall(void)
{
    for (InStateQueue = 0; InStateQueue < CONFIG_STATE_MESSAGE_QUEUE_SIZE; InStateQueue++)
        STATE_MESSAGEQUEUES_BUF[InStateQueue] = 0;

    InStateQueue = OutStateQueue = 0;
}

void fw_flow_message_clearallType(UINT16 msg)
{
#if 0
    UINT8 xdata count;
    UINT16 Tmp_BUF[CONFIG_STATE_MESSAGE_QUEUE_SIZE];
    UINT16 tmp_Msg;

    if (InStateQueue == OutStateQueue)
        return;

    count = 0;
    tmp_Msg = fw_flow_message_pop();
    while (tmp_Msg != 0)
    {
        if (tmp_Msg != msg)
            Tmp_BUF	[count] = tmp_Msg;

        count++;
        tmp_Msg = fw_flow_message_pop();
    }

    InStateQueue = OutStateQueue = 0;
    for (InStateQueue = 0; InStateQueue < count; InStateQueue++)
    {
        STATE_MESSAGEQUEUES_BUF[InStateQueue] = Tmp_BUF[InStateQueue];
    }

#else
    UINT8 xdata count;
    for (count = 0; count < CONFIG_STATE_MESSAGE_QUEUE_SIZE; count++)
    {
        if (STATE_MESSAGEQUEUES_BUF[count] == msg)
            STATE_MESSAGEQUEUES_BUF[count] = 0;
    }

    InStateQueue = OutStateQueue = 0;
    for (count = 0; count < CONFIG_STATE_MESSAGE_QUEUE_SIZE; count++)
    {
        if (STATE_MESSAGEQUEUES_BUF[count] != 0)
        {
            STATE_MESSAGEQUEUES_BUF[InStateQueue] = STATE_MESSAGEQUEUES_BUF[count];
            InStateQueue++;
        }
    }
#endif
}

/*============================================================================*/
/**
 * fw_ui_message_push
 * Push message into message queues
 *
 * @param <message>	{ message to be placed in messasge queues }
 * @return 			{ void }
 *
 */

void fw_isr_IR_message_push(UINT8 message)
{
    UINT8 tmp;
#if 1
    // Check whether repeat
    if (InIRMsgQueue != OutIRMsgQueue)
    {
        for (tmp = 0; tmp < CONFIG_IR_MESSAGE_QUEUE_SIZE; tmp++)
            if (IR_MESSAGEQUEUES_BUF[tmp] == message)
                return;
    }

    tmp = (InIRMsgQueue + 1) % CONFIG_IR_MESSAGE_QUEUE_SIZE;
    // Check whether Queue not full
    if (tmp  != OutIRMsgQueue)
    {
        IR_MESSAGEQUEUES_BUF[InIRMsgQueue] = message;
        InIRMsgQueue = tmp;
    }
#else
#if 1
    if (InIRMsgQueue > OutIRMsgQueue)
    {
        for (tmp = OutIRMsgQueue; tmp < InIRMsgQueue; tmp++)
            if (IR_MESSAGEQUEUES_BUF[tmp] == message);
        return;
    }
    else if (InIRMsgQueue < OutIRMsgQueue)
    {
        for (tmp = InIRMsgQueue + 1; tmp <= OutIRMsgQueue; tmp++)
            if (IR_MESSAGEQUEUES_BUF[tmp] == message);
        return;
    }
#else
    for (tmp = 0; tmp <= CONFIG_IR_MESSAGE_QUEUE_SIZE; tmp++)
        if (IR_MESSAGEQUEUES_BUF[tmp] == message);
    return;
#endif

    tmp = (InIRMsgQueue + 1) % CONFIG_IR_MESSAGE_QUEUE_SIZE;

    if (tmp  != OutIRMsgQueue)  //Queue not full
    {
        IR_MESSAGEQUEUES_BUF[InIRMsgQueue] = message;
        InIRMsgQueue = tmp;
    } // if queue full, skip this key event
#endif
}

void fw_isr_ui_message_push(UINT16 message)
{
#if 1 //modified by weihao
    UINT8 tmp;
//	RTD_Log(LOGGER_INFO, "\nPush message = %x \n", (UINT32)message);
    if (InUIMsgQueue != OutUIMsgQueue)
    {
        for (tmp = 0; tmp < CONFIG_UI_MESSAGE_QUEUE_SIZE; tmp++)
            if (UI_MESSAGEQUEUES_BUF[tmp] == message)
                return;
    }

    UI_MESSAGEQUEUES_BUF[InUIMsgQueue] = message;
    tmp = (InUIMsgQueue + 1) % CONFIG_UI_MESSAGE_QUEUE_SIZE;
    if (tmp  != OutUIMsgQueue)  //Queue not full
    {
        InUIMsgQueue = tmp;
    } // if queue full, skip this key event
#else
    UINT8 tmp;
#if 1
    // Check whether repeat
    if (InUIMsgQueue != OutUIMsgQueue)
    {
        for (tmp = 0; tmp < CONFIG_UI_MESSAGE_QUEUE_SIZE; tmp++)
            if (UI_MESSAGEQUEUES_BUF[tmp] == message);
        return;
    }

#else
    if (InUIMsgQueue > OutUIMsgQueue)
    {
        for (tmp = OutUIMsgQueue; tmp < InUIMsgQueue; tmp++)
            if (UI_MESSAGEQUEUES_BUF[tmp] == message);
        return;
    }
    else if (InUIMsgQueue < OutUIMsgQueue)
    {
        for (tmp = InUIMsgQueue + 1; tmp <= OutUIMsgQueue; tmp++)
            if (UI_MESSAGEQUEUES_BUF[tmp] == message);
        return;
    }
#endif
    tmp = (InUIMsgQueue + 1) % CONFIG_UI_MESSAGE_QUEUE_SIZE;

    if (tmp  != OutUIMsgQueue)  //Queue not full
    {
        UI_MESSAGEQUEUES_BUF[tmp] = message;
        InUIMsgQueue = tmp;
    } // if queue full, skip this key event

#endif
}

void fw_ui_message_push(UINT16 message)
{
    UINT8 tmp;
    EA = 0;
#if 0
    //STATE_MESSAGEQUEUES_BUF[InUIMsgQueue] = message;
    tmp = (InUIMsgQueue + 1) % CONFIG_UI_MESSAGE_QUEUE_SIZE;
    //frank@0506 add avoid message queue full and can not be pop
    if (tmp != OutUIMsgQueue) //Queue is full
    {
        UI_MESSAGEQUEUES_BUF[tmp] = message;
        InUIMsgQueue = tmp;
    }
#else
    UI_MESSAGEQUEUES_BUF[InUIMsgQueue] = message;
    tmp = (InUIMsgQueue + 1) % CONFIG_UI_MESSAGE_QUEUE_SIZE;
    if (tmp  != OutUIMsgQueue)  //Queue not full
    {
        InUIMsgQueue = tmp;
    } // if queue full, skip this key event
#if 0
    InUIMsgQueue = (InUIMsgQueue + 1) % CONFIG_UI_MESSAGE_QUEUE_SIZE;
    //frank@0506 add avoid message queue full and can not be pop
    if (InUIMsgQueue == OutUIMsgQueue) //Queue is full
    {
        if (InUIMsgQueue == 0)
            InUIMsgQueue = CONFIG_UI_MESSAGE_QUEUE_SIZE - 1;
        else
            InUIMsgQueue = InUIMsgQueue - 1;
    }
#endif
#endif
    EA = 1;
}

UINT16 fw_ui_message_pop(void)
{
    UINT16 msg;
    EA = 0;
    // queue empty
    if (OutUIMsgQueue == InUIMsgQueue)
    {
        EA = 1;
        return 0;
    }

    msg = UI_MESSAGEQUEUES_BUF[OutUIMsgQueue];
    UI_MESSAGEQUEUES_BUF[OutUIMsgQueue] = 0;
    OutUIMsgQueue = (OutUIMsgQueue + 1) % CONFIG_UI_MESSAGE_QUEUE_SIZE;

    EA = 1;
    return msg;
}
void fw_ir_message_clear(void)
{
    for (InUIMsgQueue = 0; InUIMsgQueue < CONFIG_UI_MESSAGE_QUEUE_SIZE; InUIMsgQueue++)
        IR_MESSAGEQUEUES_BUF[InUIMsgQueue] = 0;

    InUIMsgQueue = OutUIMsgQueue = 0;
}

UINT16 fw_ir_message_pop(void)
{
    UINT16 msg;

    EA = 0;
    if (OutIRMsgQueue == InIRMsgQueue)  // queue empty
    {
        EA = 1;
        return 0;
    }

    *((UINT8*)&msg) = 0;
    *((UINT8*)&msg + 1) = IR_MESSAGEQUEUES_BUF[OutIRMsgQueue];
    IR_MESSAGEQUEUES_BUF[OutIRMsgQueue] = 0;
#if 0
    // pop all the same type message at once
    for (tmp = 0; tmp < CONFIG_IR_MESSAGE_QUEUE_SIZE; tmp++)
    {
        if (IR_MESSAGEQUEUES_BUF[tmp] == msg)
            IR_MESSAGEQUEUES_BUF[tmp] = 0;
    }
    UI_MESSAGEQUEUES_BUF[OutIRMsgQueue] = 0;
#endif

    OutIRMsgQueue = (OutIRMsgQueue + 1) % CONFIG_IR_MESSAGE_QUEUE_SIZE;
    EA = 1;
    return msg;
}

/*============================================================================*/
/**
 * fw_ui_message_clear
 * Clear all message in message queue
 *
 * @param <void>		{ void }
 * @return			{ void }
 *
 */
void fw_ui_message_clear(void)
{
    for (InUIMsgQueue = 0; InUIMsgQueue < CONFIG_UI_MESSAGE_QUEUE_SIZE; InUIMsgQueue++)
        UI_MESSAGEQUEUES_BUF[InUIMsgQueue] = 0;

    InUIMsgQueue = OutUIMsgQueue = 0;
}

void fw_ui_message_clearallType(UINT16 msg)
{
#if 1
    UINT8 xdata count;
    UINT16 Tmp_BUF[CONFIG_UI_MESSAGE_QUEUE_SIZE];
    UINT16 tmp_Msg;
    EA = 0;

    if (InUIMsgQueue == OutUIMsgQueue)
    {
        EA = 1;
        return;
    }

    count = 0;
    do
    {
        tmp_Msg = UI_MESSAGEQUEUES_BUF[OutUIMsgQueue];
        UI_MESSAGEQUEUES_BUF[OutUIMsgQueue] = 0;
        if (tmp_Msg != msg)
            Tmp_BUF	[count] = tmp_Msg;
        else
            Tmp_BUF	[count] = 0;
        count++;
        OutUIMsgQueue = (OutUIMsgQueue + 1) % CONFIG_UI_MESSAGE_QUEUE_SIZE;
    }
    while ((OutUIMsgQueue != InUIMsgQueue) && (count < CONFIG_UI_MESSAGE_QUEUE_SIZE));

    InUIMsgQueue = OutUIMsgQueue = 0;
    for (InUIMsgQueue = 0; InUIMsgQueue < count; InUIMsgQueue++)
    {
        UI_MESSAGEQUEUES_BUF[InUIMsgQueue] = Tmp_BUF[InUIMsgQueue];
    }
    EA = 1;
#else
    UINT8 xdata count;
    EA = 0;
    for (count = 0; count < CONFIG_UI_MESSAGE_QUEUE_SIZE; count++)
    {
        if (UI_MESSAGEQUEUES_BUF[count] == msg)
            UI_MESSAGEQUEUES_BUF[count] = 0;
    }

    InUIMsgQueue = OutUIMsgQueue = 0;
    for (count = 0; count < CONFIG_UI_MESSAGE_QUEUE_SIZE; count++)
    {
        if (UI_MESSAGEQUEUES_BUF[count] != 0)
        {
            UI_MESSAGEQUEUES_BUF[InUIMsgQueue] = UI_MESSAGEQUEUES_BUF[count];
            InUIMsgQueue++;
        }
    }
    EA = 1;
#endif
}

/*********************************************************************
** 函数名称: void fw_isr_IR_message_push(UINT8 message)
** 功能描述: IR message 进栈
** 输　  入: 消息
** 输　  出: 无 
**********************************************************************/
void fw_IR_message_push(UINT8 message)
{
    UINT8 tmp;
    EA = 0;
    // Check whether repeat
    if (InIRMsgQueue != OutIRMsgQueue)
    {
        for (tmp = 0; tmp < CONFIG_IR_MESSAGE_QUEUE_SIZE; tmp++)
            if (IR_MESSAGEQUEUES_BUF[tmp] == message)
                return;
    }

    tmp = (InIRMsgQueue + 1) % CONFIG_IR_MESSAGE_QUEUE_SIZE;
    // Check whether Queue not full
    if (tmp  != OutIRMsgQueue)
    {
        IR_MESSAGEQUEUES_BUF[InIRMsgQueue] = message;
        InIRMsgQueue = tmp;
    }

    EA = 1;
}

