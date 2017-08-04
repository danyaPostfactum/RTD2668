/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2006
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is header file for vdc related functions.
 *
 * @author 	$Author$
 * @date		$Date$
 * @version	$Revision$
 * @ingroup	display
 */

/**
 * @addtogroup vdc
 * @{
 */

#ifndef _PCB_SYS_H
#define _PCB_SYS_H
/*============================ Module dependency  ===========================*/
#include "sparrow.h"

//#include "pcb_parameter_def.h"
//#include "pcb\pcb_gpio_def.h"
//#include "pcb\pcb_gpio.h"
/*================================ Definitions ==============================*/

void pcb_ctrl_PanelPowerOn(void);
void pcb_ctrl_PanelPowerOff(void);
//void pcb_ctrl_TunerPoweron(void);
//void pcb_ctrl_TunerPoweroff(void);
void pcb_ctrl_LightPowerOn(void);
void pcb_ctrl_LightPowerOff(void);

//--------------------------------------------------
// Description  : Set panel power off
// Input Value  : None
// Output Value : None
//--------------------------------------------------
void pcb_ctrl_PowerOn(void);
void pcb_ctrl_PowerOff(void);

#if 0
void drvif_Hdmi_HpdOn(UINT8 SOURCE_INDEX);
void CHdmi_HpdOff(UINT8 SOURCE_INDEX);
void CHdmi_HpdAllOff();

void appcb_pin_share_init(void);
void appcb_set_audio_path(UINT8 sel);
void appcb_set_input_src_mux(UINT8 sel);
UINT8 appcb_readkey(void);
void appcb_dev_power_on(void);
void appcb_sev_ddc2uart(void);
UINT8 appcb_readkey(void);
void appcb_ddr_power_ctrl(UINT8 on_off);
#endif



#endif	// #ifndef _PCB_SYS_H
/*======================== End of File =======================================*/

/**
  * @}
  */
