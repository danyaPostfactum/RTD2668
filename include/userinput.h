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

#ifndef _USERINPUT_H_
#define _USERINPUT_H_
/*============================ Module dependency  ===========================*/
#include "sparrow.h"

/*===================================  Types ================================*/
/*================================== Variables ==============================*/
/*================================ Definitions ==============================*/
#define GET_KEY_STATUS(x) (x & keystatus)
#define _POWER_KEY_MASK             	0x80
#define _MENU_KEY_MASK             		0x40
#define _SOURCE_KEY_MASK			0x20
#define _LEFT_KEY_MASK              		0x10
#define _RIGHT_KEY_MASK            		0x08
#define _UP_KEY_MASK             		0x04
#define _DOWN_KEY_MASK          		0x02
#define _ENTER_KEY_MASK             	0x01

typedef struct _KEYCODE_MAP_STRUCT
{
    UINT8  nCode;
    UINT8 nMessage;
} KEYCODE_MAP_STRUCT;

/*================================== Function ===============================*/
/*============================================================================*/
void fw_UserInput_Init(void);
void fw_UserInput_PowerOff(void);
#endif // #ifndef _USER_INPUT_H_
/*======================== End of File =======================================*/
/**
*
* @}
*/

