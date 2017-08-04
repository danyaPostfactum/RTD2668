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

#ifndef _TIMER_EVENT_H_
#define _TIMER_EVENT_H_

/*============================ Module dependency  ===========================*/
#include "sparrow.h"

/*===================================  Types ================================*/
#define SET_ENABLE_TTX_10ms()	{EA = 0;fUse10msTTX = 1; EA = 1;}
#define CLR_ENABLE_TTX_10ms()	{EA = 0;fUse10msTTX = 0; EA = 1;}

#define SET_ENABLE_HDMI_10ms()	{EA = 0; fUse10msHDMI = 1; EA = 1;}
#define CLR_ENABLE_HDMI_10ms()	{EA = 0; fUse10msHDMI = 0; EA = 1;}

#undef fUseCPUSwitchEnable

/*================================== Variables ==============================*/
// jiffies is global variable that holds the number of seconds that have occured after the system has booted.
//extern UINT32 jiffies;
extern bit bHDMI_isr;
//extern UINT32 msTimer;	// debug timer, hsliao 20080909

/*================================ Definitions ==============================*/
/*================================== Function ===============================*/
/*============================================================================*/
/**
 * drv_timer_event_isr_10ms
 * Initial timer event
 *
 * @param <void>	{ void }
 * @return 			{ void }
 *
 */
bit drv_timer_event_isr_10ms(void);
bit drv_timer_event_isr_1ms(void);

/*============================================================================*/
/**
 * drv_timer_event_isr
 * Initial timer event
 *
 * @param <void>	{ void }
 * @return 			{ void }
 *
 */
// bit drv_timer_event_isr_10ms(void);


/*============================================================================*/
/**
 * fw_timerEvent_Initial
 * Initial timer event
 *
 * @param <void>	{ void }
 * @return 			{ void }
 *
 */
void fw_timer_event_Init(void);
void fw_timer2_event_Init(void);
/*============================================================================*/
/**
 * TimerEvent_Handler
 *
 *
 * @param <void>		{ void }
 * @return 			{ void }
 *
 */
void fw_timer_event_Handler(void);

/*============================================================================*/
/**
 * flow_timer_Reset_WatchDog
 * Reset watchdog avoid to system reset
 *
 * @param <void>		{ void }
 * @return 			{ void }
 *
 */
void flow_timer_Reset_WatchDog(void);

/*============================================================================*/
/**
 * fw_timer_tvent_ActiveTimerEvent
 * Add new function into timer event
 *
 * @param <usTime>	{ timer count }
 * @param <Event>	{ function pointer to callback }
 *
 */
UINT8 fw_timer_event_ActiveTimerEvent(UINT16 msTime, void (*Event)(void));
/*============================================================================*/
/**
 * fw_timer_event_ReactiveTimerEvent
 * Renew or add timer event
 *
 * @param <usTime>	{ timer count }
 * @param <Event>	{ function pointer to callback }
 * @return 			{ void }
 *
 */
UINT8 fw_timer_event_ReactiveTimerEvent(UINT16 usTime, void (*Event) (void));

/*============================================================================*/
/**
 * fw_timer_event_CancelTimerEvent
 * Cancel a timer event
 *
 * @param <void>		{ void }
 * @return 			{ void }
 *
 */
void fw_timer_event_CancelTimerEvent(void (*Event) (void));

/*============================================================================*/
/**
 * fw_timer_event_CancelAllTimerEvent
 * Cancel all timer event in internal table
 *
 * @param <void>		{ void }
 * @return 			{ void }
 *
 */
void fw_timer_event_CancelAllTimerEvent(void);

/*============================================================================*/
/**
 * TimerEvent_DelayXms
 * Wait for a time of ms
 *
 * @param <usNum>	{ Delay for ms unit }
 * @return 			{ void }
 *
 */
void fw_timer_event_DelayXms(UINT16 usNum);
void fw_timer_event_DelayXus(UINT16 usNum);
/**
 * TimerEvent_PollingEventProc
 * Polling Event Process
 * @param <ucTimeout --> Timeout number (Max: 255 ms)>
 * @param <Event     --> Polling event. This event has to return _TRUE or _FALSE>
 * @return {_TRUE while polling success, _FALSE for timeout}
 *
*/
UINT8 TimerEvent_PollingEventProc(UINT8 ucTimeout, UINT8 (*Event)(void));

/**
 * TimerEvent_PreciseTimerEvent
 * Set the timer event as precise timer event. The execution timing will more accurate.
 * Remember just send event out or don't do much thing in Precise Timer Event or
 * the system performance will decrease much. Because we call Event in timer ISR.
 *
 * @param <Event>	{ function pointer as more precise }
 *
 */
void TimerEvent_PreciseTimerEvent(void (*Event) (void));

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
void fwif_scalertimer_1s_timer_enable(UINT8 on_off);


#ifdef CONFIG_ENABLE_HDMI
void fw_timer_hdmi_10ms_Init(void);
bit drv_timer_event_hdmi_isr_10ms(void);
#endif

void fw_timer1_10ms_Init(void);

void Ds_DelayXms(UINT16 usNum);

#endif //#ifndef _TIMER_EVENT_H_
/*======================== End of File =======================================*/
/**
*
* @}
*/
