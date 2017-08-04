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
#include "system.h"
#include "char\uart.h"
#include "panel\panel_library.h"
#include "pcb\pcb_utility.h"
#include "irrc\irrc.h"
#include "pcb_conf.h"
#include "pcb_customer.h"
#include "scaler_ddomain\scaler_display.h"
#include "char\lsadc.h"
#include "i2c\i2c.h"
#include "vip\color.h"
#include "pcb\pwm.h"
#include "timer\timer.h"
#include "sparrow_utility.h"
#include "audio/audio_api.h"
#include "rosprintf.h"
#include "flow_conf_struct.h"
#include "flash\spi_flash.h"
#include "video\video.h"
#include "tv.h"
#include "ifd\ifd.h"
#include "char\monitor.h"
#include "message.h"

extern char code compiler_version_info[];
extern char code compiler_date_info[];
extern char code compiler_clock_info[];
extern char code compiler_username_info[];

extern char code compiler_lib_version_info[];
extern char code compiler_lib_date_info[];
extern char code compiler_lib_clock_info[];
extern char code compiler_lib_username_info[];
UINT8	fCPUSpeed;
/*============================================================================*/
/**
 * flow_system_parameter_Init
 * Initial system parameter when boot
 *
 * @param <void> 	{ void }
 * @return 			{ void }
 *
 */
// Initialize parameter when boot
void flow_system_parameter_Init()
{
    // RTD_Log
    fw_rosprintf_para_Init();

    // Message
    fw_message_para_Init();

    // Monitor
    fw_monitor_system_Init();

    // Video decoder driver
    drv_video_system_Init();

    // IFD
    drvif_ifd_system_init();

#ifdef _VIDEO_TV_SUPPORT
    // TV
    fw_tv_system_Init();
#endif

    // color
    drv_color_system_Init();
}

/*============================================================================*/
/**
 * flow_system_Init
 * Initial hardware-related function
 *
 * @param <void> 	{ void }
 * @return 		{ void }
 *
 */
void flow_system_Init(void)
{
    // Initialize cpu speed to default value
    fCPUSpeed = 0;

    // Set essential hardware options, for example PLL, clock enable etc.,.
    drv_sparrow_boot();

    //  Start to initial hardware-related funciton
    // Initial pcb configuration
    drv_pcb_Init();
    RTD_Log(LOGGER_INFO, "drv_pcb_Init ...\n");

    // Initial Uart for monitor handler
#if(UART_SOURCE_0_ENABLE == _ENABLE)
    drv_uart_Init(0);
#endif
#if(UART_SOURCE_1_ENABLE == _ENABLE)
    drv_uart_Init(1);
#endif
    RTD_Log(LOGGER_INFO, "drv_uart_Init ...\n");

    // Initial Timer
    RTD_Log(LOGGER_INFO, "drv_timer_Init ...\n");
    drv_timer_Init();

    // Initial IRRC   
    RTD_Log(LOGGER_INFO, "drv_irrc_Init ...\n");
    drv_irrc_Init();    
#if 0 //(_DCR_MODE == _DCR_TYPE_3)
    drvif_color_DcrInitial();
#endif

    // Initial interrupt function
    drv_flash_protect(_ON);   
}

/*============================================================================*/
/**
 * flow_system_ShowInfo
 * Show system informate via UART
 *
 * @param <void> 	{ void }
 * @return 			{ void }
 *
 */
void flow_system_ShowInfo(void)
{
    RTD_Log(LOGGER_INFO, "\nVersion:");
    RTD_Log(LOGGER_INFO, compiler_version_info);
    RTD_Log(LOGGER_INFO, "\nDate:");
    RTD_Log(LOGGER_INFO, compiler_date_info);
    RTD_Log(LOGGER_INFO, "\nClock:");
    RTD_Log(LOGGER_INFO, compiler_clock_info);
    RTD_Log(LOGGER_INFO, "\nUsername:");
    RTD_Log(LOGGER_INFO, compiler_username_info);
    RTD_Log(LOGGER_INFO, "\n");

    RTD_Log(LOGGER_INFO, "\nLibrary Version:");
    RTD_Log(LOGGER_INFO, compiler_lib_version_info);
    RTD_Log(LOGGER_INFO, "\nLibrary Date:");
    RTD_Log(LOGGER_INFO, compiler_lib_date_info);
    RTD_Log(LOGGER_INFO, "\nLibrary Clock:");
    RTD_Log(LOGGER_INFO, compiler_lib_clock_info);
    RTD_Log(LOGGER_INFO, "\nLibrary Username:");
    RTD_Log(LOGGER_INFO, compiler_lib_username_info);
    RTD_Log(LOGGER_INFO, "\n");
}

void flow_storage_memcpy(UINT8 xdata* dst, UINT8 xdata* src, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        *dst = *src;
        dst ++;
        src ++;
    }
}

void flow_system_LoadParameter(UINT8 xdata* ptBuf, UINT8 offset, UINT8 size)
{
    UINT16 DstPointer;

    if (ptBuf == NULL)
        return;

    // Load from storage zone
    DstPointer = (UINT16)&stflowbData;

    // flash_addr point to address in storage
    DstPointer += offset;

    drv_spi_flash_Read((UINT16)ptBuf, _BANK0, DstPointer, size);
}



