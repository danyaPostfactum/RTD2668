/*=============================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2005
  * All rights reserved.
  * ============================================ */

/*================= File Description ================= */
/**
 * @file
 * The file is a declaration and definition header file
 *
 * @author 	$Author:  $
 * @date 	$Date:  $
 * @version 	$Revision:  $
 * @ingroup
 */

/**
* @addtogroup
* @
*/
#ifndef _SPARROW_UTILITY_H
#define _SPARROW_UTILITY_H

void drv_sparrow_boot(void);
void drv_sparrow_PowerOn(void);
void drv_sparrow_PowerOff(void);
#if 0	// not used
#define ROS_ENTER_CRITICAL() drv_sparrow_Interrupt_Enable(1, _DISABLE)
#define ROS_EXIT_CRITICAL() drv_sparrow_Interrupt_Enable(1, _DISABLE)
void drv_sparrow_Interrupt_Enable(bit interrupt_no, bit en);
#endif

//	0: use crystal clock(27MHz)
//	1: use CPU PLL clokc
void drv_sparrow_CPU_Switch_Clock(bit type);
void drv_sparrow_isr_CPU_Switch_Clock(bit type);
// void drv_sparrow_CPU_Switch_Flash_Clock(bit opt);

#endif //#ifndef _SPARROW_UTILITY_H
