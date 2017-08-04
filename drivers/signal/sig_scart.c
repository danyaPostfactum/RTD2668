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

/*============================ Module dependency  ===========================*/
#include "signal\sig_avsv.h"
#include "video\video.h"
#include "rosprintf.h"
#include "reg_def.h"
#include "flow_struct.h"
#include "scaler_idomain\vgip.h"
#include "pcb_customer.h"
#include "message.h"
#include "pcb\pcb_utility.h"

/*===================================  Types ================================*/
/*================================== Variables ==============================*/
/*================================ Definitions ==============================*/
/*================================== Function ===============================*/
#ifdef CONFIG_SCART_AUTO_SWITCH
UINT8 drv_sig_scart_DetectMode(void)
{
#if 0
    if (drv_pcb_get_gpio_pin(GPIO_SCART_FUNCTION_SELECT))
        fw_ui_message_push(_SCART_FS_HIGH_MESSAGE);
    else
        fw_ui_message_push(_SCART_FS_LOW_MESSAGE);
#endif
    return drv_sig_avsv_DetectMode();
}

UINT8 drv_sig_scart_CheckMode(void)
{
    UINT8 res = drv_sig_avsv_CheckMode();
//	RTD_Log(LOGGER_INFO, "scart checkmode:%x\n",(UINT32)res);
#if 0
    if (drv_pcb_get_gpio_pin(GPIO_SCART_FUNCTION_SELECT))
        fw_ui_message_push(_SCART_FS_HIGH_MESSAGE);
    else
        fw_ui_message_push(_SCART_FS_LOW_MESSAGE);
#endif
    return res;
}
#endif
/*============================================================================*/
