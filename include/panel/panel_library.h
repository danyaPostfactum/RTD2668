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

#ifndef _PANEL_LIBRARY_H
#define _PANEL_LIBRARY_H

#include "sparrow.h"
#include "panel\panel.h"

void drv_panel_Init(void);
void drv_panel_InitDisplayPort(void);
//void drv_panel_Backlighton(void);
void drv_panel_PowerPanelOn(void);
void drv_panel_PowerPanelOff(void);

void drv_panel_PowerDisplayPortOn(void);
void drv_panel_PowerDisplayPortOff(void);

void drv_panel_PowerLVDSOn(void);
void drv_panel_PowerLVDSOff(void);


//#if(CONFIG_DISPLAY_PANEL_STYLE == _PANEL_INTERFACE_RSDS)
void drv_panel_PowerRSDSOn(void);
void drv_panel_PowerRSDSOff(void);
//#endif

#endif //#ifndef _PANEL_LIBRARY_H
