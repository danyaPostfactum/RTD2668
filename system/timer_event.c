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
#include "autoconf.h"

/*============================ Module dependency  ===========================*/
#include "timer_event.h"
#include "timer\timer.h"
#ifdef CONFIG_ENABLE_HDMI
#include "hdmi/hdmifun.h"
#endif
#include "vip\color.h"
#include "isr_utility.h"
#include "message.h"
#include "osdcore.h"
#include "reg52.h"
#include "rosprintf.h"
#include "intrins.h"
#include "ui_ctrl.h"


/*===================================  Types ================================*/
typedef struct TIMEREVENT_STRUCT
{
    UINT16 Time;				///! The rest time of this event
    void (*Event)(void);		///! The event handler's function pointer of this event
} _TIMEREVENT_STRUCT;

/*================================ Definitions ==============================*/
#define _INACTIVE_TIMER_EVENT   	(0xFFFF)
#define _MAX_EVENT_AMOUNT   		(20)		///! The ammount of all soft timer events.

/*================================== Variables ==============================*/
struct TIMEREVENT_STRUCT xdata TimerEvent[_MAX_EVENT_AMOUNT];
// UINT16 xdata jitter_time;
bit jitter_ustime;
UINT8 xdata sleep_10ms;
UINT8 xdata sleep_sec;
UINT8 xdata sleep_min;
UINT32 xdata profile_time;
UINT8 xdata keypad_time;

bit bHDMI_isr;
extern	UINT8 xdata ucSleepType;
extern UINT8 key_state_from_isr;

/*================================== Function ===============================*/
/*============================================================================*/
/**
 * drv_timer_event_isr
 * Initial timer event
 *
 * @param <void>	{ void }
 * @return 			{ void }
 *
 */
extern  volatile bit fDebugMode;
#ifdef CONFIG_ENABLE_HDMI
HDMI_BOOL rtd_hdmiPhy_ISR(void);
#endif
bit drv_timer_event_isr_1ms(void)
{
    if (!(rtd_isr_inb(SYS_GIP_3_0x2c3b) & 0x10))
        return RT_ISR_UNHANDLED;

    // Disable timer2 operation
    rtdf_clearBits(TC2CR_0x2860, _BIT31);
    //RtdCtrl_isr_Mask(TC2CR_0x2860, ~((UINT32)_BIT31), 0);

    // Clear timer0 interrupt's pending status
    rtd_isr_outl(ISR_0x2504, _BIT7);

    jitter_ustime = 0;

    return RT_ISR_HANDLED;
}
//For SVN 1911
bit drv_timer_event_hdmi_isr_10ms(void)
{
	if(!(rtd_isr_inb(SYS_GIP_3_0x2c3b) & 0x08))
		return RT_ISR_UNHANDLED;

	if(!fDebugMode) {
		rtd_hdmiPhy_ISR();
	}

	// Clear pending status for timer1
	rtd_isr_outl(ISR_0x2504, _BIT6);

	return RT_ISR_HANDLED;
}

bit drv_timer_event_isr_10ms(void)
{
    UINT8 xdata timereventcnt;
    if (!(rtd_isr_inb(SYS_GIP_3_0x2c3b) & 0x04))
        return RT_ISR_UNHANDLED;

#if 0
    if (jitter_time)
        jitter_time--;
#endif
	keypad_time ++;
	if(keypad_time%10 == 0)
	{
	#if 1
		keypad_time=0;
		ui_detect_keypad_isr();
	#endif
		//if(key_state_from_isr == 0)
			//key_state_from_isr = ui_ReadKeyPad_isr();
	}
		
    profile_time ++;
    if ((sleep_min + sleep_sec))
    {
        sleep_10ms++;
        if (sleep_10ms >= 100)  // 1 sec
        {
            sleep_10ms = 0;
            if (sleep_sec > 0)
                sleep_sec--;

            if (sleep_sec == 0)
            {  
	            fw_isr_ui_message_push(_SLEEP_TIMEOUT_MESSAGE);
	            ucSleepType = 0;
                /*if (sleep_min == 0)  	// sleep time out
                {
                    fw_isr_ui_message_push(_SLEEP_TIMEOUT_MESSAGE);
                }
                else
                {
                    sleep_sec = 60;
                    //sleep_min--;
                }*/
            }
            else
            {
                //if ( sleep_sec % 10 == 0)
                //	fw_isr_ui_message_push(_SLEEP_10SEC_MESSAGE);
            }
        }
    }

    for (timereventcnt = 0; timereventcnt < _MAX_EVENT_AMOUNT; timereventcnt++)
    {
        if (TimerEvent[timereventcnt].Time != _INACTIVE_TIMER_EVENT)
        {
            if (TimerEvent[timereventcnt].Time > 0)
            {
                TimerEvent[timereventcnt].Time--;
            }
        }
    }
    
/*#ifdef CONFIG_ENABLE_HDMI   
    if(bHDMI_isr)
    {
    	if (!fDebugMode)
        	rtd_hdmiPhy_ISR();
    }
#endif//#ifdef CONFIG_ENABLE_HDMI  *///For SVN 1911

    // Clear pending status for timer0
    rtd_isr_outl(ISR_0x2504, _BIT5);


    return RT_ISR_HANDLED;
}

/*============================================================================*/
/**
 * flow_timer_Reset_WatchDog
 * Reset watchdog avoid to system reset
 *
 * @param <void>		{ void }
 * @return 			{ void }
 *
 */
void flow_timer_Reset_WatchDog(void)
{
    drv_timer_Reset_WatchDog();
}
#ifdef CONFIG_ENABLE_HDMI

void fw_timer_hdmi_10ms_Init(void)
{
	//bHDMI_isr = 0;
	//For SVN 1911
	// Enable timer operation
	drv_timer_Enable(TIMER1, _DISABLE);

	// Set interrupt route select to IRQ0
	drv_isr_SelectRoute(ROUTE_SEL_TIMER1, IRQ_ROUTE_IRQ1);

	// Enable Timer interrupt
	drv_timer_Interrupt_Enable(TIMER1, _ENABLE);

	drv_isr_Enable(SYS_ISR_TIMER1, _TRUE);

	// Clear timer0 interrupt's pending status
	rtd_outl(ISR_0x2504, _BIT6);
	
    RTD_Log(LOGGER_INFO, "fw_timer_hdmi_10ms_Init complete\n");
}

#endif//#ifdef CONFIG_ENABLE_HDMI

/*============================================================================*/
/**
 * TimerEvent_Initial
 * Initial timer event
 *
 * @param <void>	{ void }
 * @return 			{ void }
 *
 */
void fw_timer_event_Init(void)
{
//	jitter_time = 0;
    sleep_10ms = 0;
    sleep_sec = 0;
    sleep_min = 0;

    // Enable timer operation
    drv_timer_Enable(TIMER0, _DISABLE);

    fw_timer_event_CancelAllTimerEvent();

    // Set interrupt route select to IRQ0
    drv_isr_SelectRoute(ROUTE_SEL_TIMER0, IRQ_ROUTE_IRQ0);

    // Enable Timer interrupt
    drv_timer_Interrupt_Enable(TIMER0, _ENABLE);

    drv_isr_Enable(SYS_ISR_TIMER0, _TRUE);

    // Clear timer0 interrupt's pending status
    rtd_outl(ISR_0x2504, _BIT5);

    // Enable timer operation
    drv_timer_Enable(TIMER0, _ENABLE);

    RTD_Log(LOGGER_INFO, "fw_timer_event_Init complete\n");
}


/*============================================================================*/
/**
 * Timer2Event_Initial
 * Initial timer event
 *
 * @param <void>	{ void }
 * @return 			{ void }
 *
 */
void fw_timer2_event_Init(void)
{
    jitter_ustime = 0;

    // Enable timer operation
    drv_timer_Enable(TIMER2, _DISABLE);

    // Set interrupt route select to IRQ0
    drv_isr_SelectRoute(ROUTE_SEL_TIMER2, IRQ_ROUTE_IRQ0);

    drv_isr_Enable(SYS_ISR_TIMER2, _TRUE);

    // Clear timer0 interrupt's pending status
    rtd_outl(ISR_0x2504, _BIT7);

    RTD_Log(LOGGER_INFO, "fw_timer2_event_Init complete\n");
}

/*============================================================================*/
/**
 * TimerEvent_Handler
 *
 *
 * @param <void>		{ void }
 * @return 			{ void }
 *
 */
void fw_timer_event_Handler(void)
{
    UINT8 xdata timereventcnt;
    for (timereventcnt = 0; timereventcnt < _MAX_EVENT_AMOUNT; timereventcnt++)
    {
        if (TimerEvent[timereventcnt].Time == 0)
        {
            EA = 0;
            TimerEvent[timereventcnt].Time = _INACTIVE_TIMER_EVENT;
            EA= 1;
//			RTD_Log(LOGGER_INFO, "event = %x\n", (UINT32)TimerEvent[timereventcnt].Event);
            (*TimerEvent[timereventcnt].Event) ();

        }
    }
}

/*============================================================================*/
/**
 * TimerEvent_ActiveTimerEvent
 * Add new function into timer event
 *
 * @param <usTime>	{ timer count }
 * @param <Event>	{ function pointer to callback }
 *
 */
UINT8 fw_timer_event_ActiveTimerEvent(UINT16 msTime, void (*Event)(void))
{
    UINT8 xdata timereventcnt;
    UINT16 xdata stTimeInterval;
    stTimeInterval =  (msTime / 10);
    if (!stTimeInterval)
        stTimeInterval = 1;

    EA = 0;
    for (timereventcnt = 0;
            timereventcnt < _MAX_EVENT_AMOUNT;
            timereventcnt++)
    {
        if ((TimerEvent[timereventcnt].Time != _INACTIVE_TIMER_EVENT) &&
                (TimerEvent[timereventcnt].Event == Event))
        {
            EA = 1;
            return _FALSE;
        }
    }

    for (timereventcnt = 0;
            timereventcnt < _MAX_EVENT_AMOUNT;
            timereventcnt++)
    {
        if (TimerEvent[timereventcnt].Time == _INACTIVE_TIMER_EVENT)
        {
            TimerEvent[timereventcnt].Time = stTimeInterval;
            TimerEvent[timereventcnt].Event = Event;
            EA = 1;
            return _TRUE;
        }
    }
    EA = 1;
    return _FALSE;
}

/*============================================================================*/
/**
 * TimerEvent_ReactiveTimerEvent
 * Renew or add timer event
 *
 * @param <usTime>	{ timer count }
 * @param <Event>	{ function pointer to callback }
 * @return 			{ void }
 *
 */
UINT8 fw_timer_event_ReactiveTimerEvent(UINT16 msTime, void (*Event) (void))
{
    UINT8 xdata timereventcnt;
    UINT16 xdata stTimeInterval;
    stTimeInterval =  (msTime / 10);

    if (!stTimeInterval)
        stTimeInterval = 1;

    EA = 0;
    for (timereventcnt = 0;
            timereventcnt < _MAX_EVENT_AMOUNT;
            timereventcnt++)
    {
        if ((TimerEvent[timereventcnt].Time != _INACTIVE_TIMER_EVENT) &&
                (TimerEvent[timereventcnt].Event == Event))
        {
            TimerEvent[timereventcnt].Time = stTimeInterval;
            EA = 1;
            return _FALSE;
        }
    }

    for (timereventcnt = 0;
            timereventcnt < _MAX_EVENT_AMOUNT;
            timereventcnt++)
    {
        if (TimerEvent[timereventcnt].Time == _INACTIVE_TIMER_EVENT)
        {
//			RTD_Log(LOGGER_INFO, "cnt = %x, readd timer_event = %x\n",(UINT32)timereventcnt, (UINT32)TimerEvent[timereventcnt].Event);
            TimerEvent[timereventcnt].Time = stTimeInterval;
            TimerEvent[timereventcnt].Event = Event;
            EA = 1;
            return _TRUE;
        }
    }
    EA = 1;
    return _FALSE;
}

/*============================================================================*/
/**
 * TimerEvent_CancelTimerEvent
 * Cancel a timer event
 *
 * @param <void>		{ void }
 * @return 			{ void }
 *
 */
void fw_timer_event_CancelTimerEvent(void (*Event) (void))
{
    UINT8 xdata timereventcnt;
    EA = 0;
    // Disable interrupt in future
    for (timereventcnt = 0;
            timereventcnt < _MAX_EVENT_AMOUNT;
            timereventcnt++)
    {
        if (TimerEvent[timereventcnt].Event == Event)
        {
//			RTD_Log(LOGGER_INFO, "cnt = %x, cancel timer_event = %x\n",(UINT32)timereventcnt, (UINT32)TimerEvent[timereventcnt].Event);
            TimerEvent[timereventcnt].Time = _INACTIVE_TIMER_EVENT;
//			TimerEvent[timereventcnt].precise_timer = _FALSE;
        }
    }
    EA = 1;
}

/*============================================================================*/
/**
 * TimerEvent_CancelAllTimerEvent
 * Cancel all timer event in internal table
 *
 * @param <void>		{ void }
 * @return 			{ void }
 *
 */
void fw_timer_event_CancelAllTimerEvent(void)
{
    UINT8 xdata timereventcnt;
    EA = 0;
    for (timereventcnt = 0; timereventcnt < _MAX_EVENT_AMOUNT; timereventcnt++)
    {
        TimerEvent[timereventcnt].Time = _INACTIVE_TIMER_EVENT;
        TimerEvent[timereventcnt].Event= 0;
//		TimerEvent[timereventcnt].precise_timer = _FALSE;
    }
    EA = 1;
}

/*============================================================================*/
/**
 * fw_timer_event_DelayXms
 * Wait for a time of ms
 *
 * @param <usNum>	{ Delay for ms unit }
 * @return 			{ void }
 *
 */
void fw_timer_event_DelayXms(UINT16 usNum)
{
    EA = 0;
#if 1
    // Disable timer2 operation
    rtd_maskl(TC2CR_0x2860, 0x0fffffff, 0);

    if (usNum > 1)
        rtd_outl(TC2TVR_0x2848, usNum * 10);		// 1msec
    else
        rtd_outl(TC2TVR_0x2848, 10);	// usNum msec

    // Clear timer2 interrupt's pending status
    rtd_outl(ISR_0x2504, _BIT7);

    // Enable Timer2 interrupt
    rtdf_setBits(TC2ICR_0x286c, _BIT31);

    // Enable time2r operation
    rtdf_setBits(TC2CR_0x2860, _BIT31);

    jitter_ustime = 1;

    EA = 1;

    // Wait time out
    while (jitter_ustime);

#else
    if (!(usNum / 10))
        jitter_time = 1;
    else
        jitter_time = usNum / 10;

    EA = 1;

    // Wait time out
    while (jitter_time);
#endif
}

void fw_timer_event_DelayXus(UINT16 usNum)
{
    EA = 0;

    // Disable timer2 operation
    rtd_maskl(TC2CR_0x2860, 0x0fffffff, 0);

    if (usNum > 1)
        rtd_outl(TC2TVR_0x2848, usNum);		// 1msec
    else
        rtd_outl(TC2TVR_0x2848, 2);	// usNum msec

    // Clear timer2 interrupt's pending status
    rtd_outl(ISR_0x2504, _BIT7);

    // Enable Timer2 interrupt
    rtdf_setBits(TC2ICR_0x286c, _BIT31);

    // Enable time2r operation
    rtdf_setBits(TC2CR_0x2860, _BIT31);

    jitter_ustime = 1;

    EA = 1;

    // Wait time out
    while (jitter_ustime);
}

void Ds_DelayXms(UINT16 usNum)
{
	UINT16 idata i;

	while(usNum!=0)
	{
		for(i=0;i<200;i++)
		{
			_nop_();
			_nop_();
			_nop_();
			_nop_();

			_nop_();
			_nop_();
			_nop_();
			_nop_();

			_nop_();
		}
		usNum--;
	}
}


/**
 * TimerEvent_PollingEventProc
 * Polling Event Process
 * @param <ucTimeout --> Timeout number (Max: 255 ms)>
 * @param <Event     --> Polling event. This event has to return _TRUE or _FALSE>
 * @return {_TRUE while polling success, _FALSE for timeout}
 *
*/
//UINT8 TimerEvent_PollingEventProc(UINT8 ucTimeout, UINT8 (*Event)(void));

/**
 * TimerEvent_PreciseTimerEvent
 * Set the timer event as precise timer event. The execution timing will more accurate.
 * Remember just send event out or don't do much thing in Precise Timer Event or
 * the system performance will decrease much. Because we call Event in timer ISR.
 *
 * @param <Event>	{ function pointer as more precise }
 *
 */
//void TimerEvent_PreciseTimerEvent(void (*Event) (void));

/**
 * Set if trigger _FL2UI_1S_TIMER_EVENT event each seconds.
 * AP may need 1 sec event to do something, for example, system time accumulat or
 * 1 sec to check pvr module status or 60 seconds later to power off or others.
 * Call this function to turn on/off _FL2UI_1S_TIMER_EVENT event triggle.
 *
 * @param <void>		{ void }
 * @return 			{ void }
 *
 */
//void fwif_scalertimer_1s_timer_enable(UINT8 on_off);


/*======================== End of File =======================================*/
/**
*
* @}
*/
