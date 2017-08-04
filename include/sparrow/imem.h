/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2009
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is for rtdaccess related functions.
 *
 * @author 	$Author$
 * @date	$Date$
 * @version	$Revision$
 * @ingroup	rtdaccess
 */

/**
 * @addtogroup rtdaccess
 * @{
 */

#ifndef  _IMEM_H_
#define  _IMEM_H_

#include "sparrow.h"

void drv_imem4k_Start(UINT8 bank_num, UINT16 imem_addr);
void drv_imem4k_Stop(void);
void drv_imem8k_Start(UINT8 bank_num, UINT16 imem_addr);
void drv_imem8k_Stop(void);

#define MEM_SHARE_MODE_0		0
#define MEM_SHARE_MODE_1		1
#define MEM_SHARE_MODE_2		2
drv_mem_SetShareMode(UINT8 mode);

#endif //#ifndef  _IMEM_H_
