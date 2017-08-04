/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2006
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is for power related functions.
 *
 * @author 	$Author$
 * @date	$Date$
 * @version	$Revision$
 * @ingroup	power
 */

/**
 * @addtogroup power
 * @{
 */

#ifndef _POWER_H_
#define _POWER_H_

/*============================ Module dependency  ===========================*/
#include "sparrow.h"
#include "panel\panel.h"

/*===================================  Types ================================*/
extern UINT8 xdata ucPowerControl;
extern BOOL  ucPowersavingdisablelogo;

//--------------------------------------------------
// Macro of Power Switch Flag
//--------------------------------------------------
#define GET_POWERSWITCH()						((bit)(ucPowerControl & _BIT0))
#define SET_POWERSWITCH()						(ucPowerControl |= _BIT0)
#define CLR_POWERSWITCH()						(ucPowerControl &= ~_BIT0)

//--------------------------------------------------
// Macro of Power saving Switch Flag
//--------------------------------------------------
#define GET_POWERSAVING_SWITCH()				((bit)((ucPowerControl & _BIT1)>>1))
#define SET_POWERSAVING_SWITCH()				(ucPowerControl |= _BIT1)
#define CLR_POWERSAVING_SWITCH()				(ucPowerControl &= ~_BIT1)

//--------------------------------------------------
// Macro of Power status Flag
//--------------------------------------------------
typedef enum _POWER_STATUS
{
    POWER_STATUS_POWER_OFF,
    POWER_STATUS_POWER_ON,
    POWER_STATUS_POWER_SAVING
} POWER_STATUS;
#define GET_POWERSTATUS()				((ucPowerControl & (_BIT2 | _BIT3)) >> 2)
#define SET_POWERSTATUS(x)				(ucPowerControl = ((ucPowerControl & ~(_BIT2 | _BIT3)) | ((x & 0x3) << 2)))
//#define CLR_POWERSTATUS(x)                       (ucPowerControl &= ~_BIT1)

//--------------------------------------------------
// Macro of Panel Power Status
//--------------------------------------------------
#define GET_PANELPOWERSTATUS()                  ((bit)((ucPowerControl & _BIT4)>>4))
#define SET_PANELPOWERSTATUS()                  (ucPowerControl |= _BIT4)                   // Power Up
#define CLR_PANELPOWERSTATUS()                  (ucPowerControl &= ~_BIT4)                  // Power Down


//--------------------------------------------------
// Macro of Backlight Power Status
//--------------------------------------------------
#define GET_LIGHTPOWERSTATUS()                  ((bit)((ucPowerControl & _BIT5) >> 5))
#define SET_LIGHTPOWERSTATUS()                  (ucPowerControl |= _BIT5)
#define CLR_LIGHTPOWERSTATUS()                  (ucPowerControl &= ~_BIT5)


//--------------------------------------------------
// Macro of Load Font at First Power Up
//--------------------------------------------------
#define GET_FIRST_LOADFONT()                    ((bit)((ucPowerControl & _BIT6) >> 6))
#define SET_FIRST_LOADFONT()                    (ucPowerControl |= _BIT6)
#define CLR_FIRST_LOADFONT()                    (ucPowerControl &= ~_BIT6)

//--------------------------------------------------
// Macro of Enable Power switch at First Power Up
//--------------------------------------------------
#define GET_FIRST_POWER_SWITCH()                    ((bit)((ucPowerControl & _BIT7) >> 7))
#define SET_FIRST_POWER_SWITCH()                    (ucPowerControl |= _BIT7)
#define CLR_FIRST_POWER_SWITCH()                    (ucPowerControl &= ~_BIT7)

/*================================== Function ================================*/
/*============================================================================*/
/**
 * flow_power_handler
 * Execute power up process
 */
BOOL  flow_power_Handler(void);

/*============================================================================*/
/**
 * drvif_power_on
 * Execute power up/down process
 */
void flow_power_on(void);

/*=============================================================================*/
/**
 * drvif_power_saving
 * Execute power saving process
 */
void flow_power_Saving_off(void);

/*============================================================================*/
/**
 * Power_Off
 * Execute power up/down process
 */
void flow_power_off(void);

/*============================================================================*/
/**
 * Power_PLLEnable
 * Power on/off PLL
 *
 * @param <on_off>	{ power up or down for system }
 * @return 			{ void }
 *
 */
void Power_PLLEnable(UINT8 on_off);


/*============================================================================*/
/**
 * appcb_set_backlight
 * Set brightness for pane.
 *
 * @param <level>	{ set brightness for panel }
 * @return 			{ void }
 *
 */
void appcb_set_backlight(UINT8 level);
extern void appcb_set_backlight(UINT8 level);
void drv_pwm_PowerOff(void);

#endif // #ifndef  _POWER_H_
/*======================== End of File =======================================*/
/**
*
* @}
*/
