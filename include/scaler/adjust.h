/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2006
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is for adjust related functions.
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

#ifndef _ADJUST_H_
#define _ADJUST_H_

/*============================ Module dependency  ===========================*/
#include "sparrow.h"


#define _CAPTURE_HDELAY                 	3 // 2
#define _PROGRAM_HDELAY                 	0 // 10   CSW* modify 0961004 to adjust IHS delay in window H position
#define _PROGRAM_VDELAY                 	4// 2 it must larger than 4 or de-interlace will result in problem *: forster modified 071029
#define _MEASURE_HDEALY                 	7
#define _HSTARTBIAS					200
#define _IVS2DVS_MEASURE_DELAY              1

/*===================================  Types ================================*/
/*================================== Variables ==============================*/
/*================================ Definitions ==============================*/
UINT8 drv_adjust_ivs2dvsdelay(void);
void drv_adjust_spreadspectrumrange(UINT8 ucSpreadRange);
void drv_adjust_syncprocessormeasurestart(void);
UINT8  drv_adjust_measureivs2dvsdelay(void);
UINT16 drv_adjust_fast_get_sync_point(void);
void drv_adjust_setivs2dvslines(UINT8 line);

#endif // _ADJUST_H_
/*======================== End of File =======================================*/
/**
*
* @}
*/

