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

#include "rtd_types.h"
/**
* @addtogroup
* @
*/
#ifndef _AUTO_SOY_H
#define _AUTO_SOY_H

#define	SOY_CH0_COMPARE_LEVEL		0x1A
#define	_SOY_MANUAL_COMPARE_LEVEL	0x16

//#define SOY_CH1_COMPARE_LEVEL		0x1E
void drv_auto_soy_AutoSOY(void);
void drv_auto_soy_SetSOYCompareLevel(UINT8 soy_path, UINT8 value);
UINT8 drv_auto_soy_GetSOYCompareLevel(UINT8 soy_path);


#endif // #ifndef _AUTO_SOY_H
