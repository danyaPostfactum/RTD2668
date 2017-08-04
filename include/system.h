/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2006
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is a main fuction and entrance for firmwae.
 *
 * @author 	$Author$
 * @date	$Date$
 * @version	$Revision$
 * @ingroup	System
 */

/**
 * @addtogroup System
 * @{
 */
#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "sparrow.h"

/*============================================================================*/
/**
 * flow_system_Init
 * Initial hardware-related function
 *
 * @param <void> 	{ void }
 * @return 			{ void }
 *
 */
void flow_system_Init(void);

/*============================================================================*/
/**
 * flow_system_ShowInfo
 * Show system informate via UART
 *
 * @param <void> 	{ void }
 * @return 			{ void }
 *
 */
void flow_system_ShowInfo(void);

/*============================================================================*/
/**
 * flow_system_parameter_Init
 * Initial system parameter
 *
 * @param <void> 	{ void }
 * @return 			{ void }
 *
 */
void flow_system_parameter_Init(void);

/*============================================================================*/
/**
 * flow_storage_memcpy
 * Copy from src to dst in xdata address
 *
 * @param <void> 	{ void }
 * @return 			{ void }
 *
 */
void flow_storage_memcpy(UINT8 xdata* dst, UINT8 xdata* src, int size);




#endif // #ifndef _SYSTEM_H_

