/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2006
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is for i-doamin Digital Filter related functions.
 *
 * @author 	$Author$
 * @date		$Date$
 * @version	$Revision$
 * @ingroup	clock
 */

/**
 * @addtogroup clock
 * @{
 */


#ifndef _DIGITALFILTER_H_
#define _DIGITALFILTER_H_
/*============================ Module dependency  ===========================*/
#include "sparrow.h"

/*===================================  Types ================================*/


/*================================== Variables ==============================*/
/*================================ Definitions ==============================*/
/*================================== Function ===============================*/
/*============================================================================*/
void drvif_color_digital_filter_set(UINT8 Access, UINT8 Offset, UINT8 Div, UINT8 Enable);
void drvif_color_digital_filter_config(UINT8 Enable);

#endif
/*======================== End of File =======================================*/
/**
*
* @}
*/

