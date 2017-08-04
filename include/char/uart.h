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

#ifndef  _UART_H_
#define  _UART_H_

#include "sparrow.h"

// define source index for uart
enum UART_SOURCE
{
    UART_SOURCE_0 = 0,
    UART_SOURCE_1,
    UART_SOURCE_TOTAL
};

// define baudrate for uart
#define UART_BAUDRATE_9600			9600
#define UART_BAUDRATE_19200			19200
#define UART_BAUDRATE_38400			38400
#define UART_BAUDRATE_57600			57600
#define UART_BAUDRATE_115200		115200
#define UART_BAUDRATE_230400		230400

// define parity SELECT
#define UART_PARITY_SELECT_ODD		00
#define UART_PARITY_SELECT_EVEN		01
#define UART_PARITY_SELECT_MARK		02
#define UART_PARITY_SELECT_SPACE	03

// define Enable/Disable parity
#define UART_PARITY_DISABLE			0
#define UART_PARITY_ENABLE			1

// define stop bit
#define UART_STOP_BITS_1			0
#define UART_STOP_BITS_2			1

// define data bit
#define UART_DATA_BITS_5			0
#define UART_DATA_BITS_6			1
#define UART_DATA_BITS_7			2
#define UART_DATA_BITS_8			3

#define UART_SOURCE_0_BAUDRATE			UART_BAUDRATE_115200
#define UART_SOURCE_0_PARITY_SELECT		UART_PARITY_SELECT_ODD
#define UART_SOURCE_0_PARITY_ENABLE		UART_PARITY_DISABLE
#define UART_SOURCE_0_STOP_BITS			UART_STOP_BITS_1
#define UART_SOURCE_0_DATA_BITS			UART_DATA_BITS_8


typedef struct _STRUCT_UART_PARAMETER
{
    UINT32 BaudRate;
    UINT8 Parity_enable:1;
    UINT8 Parity:3;
    UINT8 StopBits:1;
    UINT8 DataBits:2;
} STRUCT_UART_PARAMETER;

/*============================================================================*/
/**
 * drv_uart_Init
 * Initial uart, baud rate, data bits..,etc.
 *
 * @param <void>		{ void }
 * @return 				{ void }
 *
 */
void drv_uart_Init(bit nUart);

/*============================================================================*/
/**
 * drv_uart_Read
 * Read data from uart
 *
 * @param <void>		{ void }
 * @return UINT8		{ data read from uart}
 *
 */
bit drv_uart_Read(bit nUart, UINT8* uart_data);
bit drv_isr_uart_Read(bit nUart, UINT8* uart_data);

/*============================================================================*/
/**
 * drv_uart_Write
 * Write data from uart
 *
 * @param <void>		{ void }
 * @return 				{ void }
 *
 */
void drv_uart_Write(bit nUart, UINT8 uart_data);
void drv_isr_uart_Write(bit nUart, UINT8 uart_data);

/*============================================================================*/
/**
 * drv_uart_Enable
 * Enable the operation of uart
 *
 * @param <void>		{ void }
 * @return 				{ void }
 *
 */
void drv_uart_Enable(bit nUart);

/*============================================================================*/
/**
 * drv_uart_Disable
 * Disable the operation of uart
 *
 * @param <void>		{ void }
 * @return 				{ void }
 *
 */
void drv_uart_Disable(bit nUart);

/*============================================================================*/
/**
 * drv_uart_Disable
 * Reset uart
 *
 * @param <void>		{ void }
 * @return 				{ void }
 *
 */
void drv_uart_Reset(bit nUart);

/*============================================================================*/
/**
 * drv_uart_IsEmpty
 * Enable the operation of uart
 *
 * @param <void>		{ void }
 * @return 				{ void }
 *
 */
bit drv_uart_IsEmpty(bit nUart);

#endif // #ifndef  _UART_H_
