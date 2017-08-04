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


#ifndef _PEAKING_H_
#define _PEAKING_H_
/*============================ Module dependency  ===========================*/
#include "sparrow.h"

/*===================================  Types ================================*/


/*================================== Variables ==============================*/
/*================================ Definitions ==============================*/
/*================================== Function ===============================*/
/*============================================================================*/
void drvif_color_peaking_config(UINT8 which_source, UINT8 level);
void drvif_color_peaking_resolution( UINT8 enable);
#endif
/*======================== End of File =======================================*/
/**
*
* @}
*/

