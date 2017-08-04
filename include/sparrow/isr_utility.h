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

/**
* @addtogroup
* @
*/
#ifndef _ISR_UTILITY_H
#define _ISR_UTILITY_H

#include "sparrow.h"

// Return value for isr
typedef enum _rt_ISR_results
{
    RT_ISR_UNHANDLED  = 0,              /* Interrupt was no handled             */
    RT_ISR_HANDLED  = 1                /* Interrupt was handled             */
} rt_ISR_results;

// Isr block type for interrupt route select
typedef enum _IRQ_ROUTE_PATH
{
    IRQ_ROUTE_IRQ_OUT,
    IRQ_ROUTE_IRQ1,
    IRQ_ROUTE_IRQ0,
    IRQ_ROUTE_TOTAL
} IRQ_ROUTE_PATH;

typedef enum _IRQ_ROUTE_TYPE
{
    ROUTE_SEL_DDC1 = 0,
    ROUTE_SEL_I2C_SLAVE,
    ROUTE_SEL_I2C_MASTER_0,
    ROUTE_SEL_I2C_MASTER_1,
    ROUTE_SEL_TIMER0,
    ROUTE_SEL_TIMER1,
    ROUTE_SEL_TIMER2,
    ROUTE_SEL_LSADC,
    ROUTE_SEL_IRDA,
    ROUTE_SEL_SPI_FLASH,
    ROUTE_SEL_VBI_PPR,
    ROUTE_SEL_VBI_SLICER,
    ROUTE_SEL_AUDIO,
    ROUTE_SEL_VIDEO,
    ROUTE_SEL_IF_DEMOD,
    ROUTE_SEL_CEC,
    ROUTE_SEL_HDMI,
    ROUTE_SEL_UART0,
    ROUTE_SEL_UART1,
    ROUTE_SEL_DDC0,
    ROUTE_SEL_UIOSD,
    ROUTE_SEL_DISPLAYING_GENERATOR,
    ROUTE_SEL_ULTRA_ZOOM,
    ROUTE_SEL_VGIP,
    ROUTE_SEL_OFFLINE_MEASURE,
    ROUTE_SEL_ONLINE_MEASURE,
    ROUTE_SEL_SYNC_PROCESSOR,
    ROUTE_SEL_AUTO_SOY,
    ROUTE_SEL_APLL,
    ROUTE_SEL_VBIOSD,
    ROUTE_SEL_GPIO,
    ROUTE_SEL_SOFTWARE,
    ROUTE_SEL_REGISTER_TIMER_OUT,
    ROUTE_SEL_TOTAL
} IRQ_ROUTE_TYPE;

typedef enum _IRQ_ENABLE_TYPE
{
    SYS_ISR_GPIO = 0,
    SYS_ISR_SOFTWARE,
    SYS_ISR_UIOSD,
    SYS_ISR_DISPLAYTIMING_GENERATOR,
    SYS_ISR_ULTRA_ZOOM,
    SYS_ISR_VGIP,
    SYS_ISR_OFFLINE_MEASURE,
    SYS_ISR_ONLINE_MEASURE,
    SYS_ISR_SYNPROCESSOR,
    SYS_ISR_AUTOSOY,
    SYS_ISR_APLL,
    SYS_ISR_VBIOSD,
    SYS_ISR_VBIPPR,
    SYS_ISR_VBI_SLICER,
    SYS_ISR_AUDIO,
    SYS_ISR_VIDEO,
    SYS_ISR_IF_DEMOD,
    SYS_ISR_CEC,
    SYS_ISR_HDMI,
    SYS_ISR_UART0,
    SYS_ISR_UART1,
    SYS_ISR_DDC0,
    SYS_ISR_DDC1,
    SYS_ISR_I2C_SLAVE,
    SYS_ISR_I2C_MASTER0,
    SYS_ISR_I2C_MASTER1,
    SYS_ISR_TIMER0,
    SYS_ISR_TIMER1,
    SYS_ISR_TIMER2,
    SYS_ISR_LSADC,
    SYS_ISR_IRDA,
    SYS_ISR_SPI_FLASH,
    SYS_ISR_REGISTER_WRAPPER,
    SYS_ISR_TOTAL
} IRQ_ENABLE_TYPE;

bit drv_isr_SelectRoute(IRQ_ROUTE_TYPE type, IRQ_ROUTE_PATH irq_type);
void drv_isr_Enable(IRQ_ENABLE_TYPE type, bit enable);
void drv_isr_isr_Enable(IRQ_ENABLE_TYPE type, bit enable);
bit drv_isr_GetPending(IRQ_ENABLE_TYPE type);
void drv_isr_ClearPending(IRQ_ENABLE_TYPE type);

#endif
