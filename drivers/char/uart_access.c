//**********************************************************************************************************
//  The  Software  is  proprietary,  confidential,  and  valuable to Realtek Semiconductor
//  Corporation  ("Realtek").  All  rights, including but not limited  to  copyrights,
//  patents,  trademarks, trade secrets, mask work rights, and other similar rights and interests,
//  are reserved to Realtek. Without  prior  written  consent  from  Realtek,  copying, reproduction,
//  modification,  distribution,  or  otherwise  is strictly prohibited. The Software  shall  be
//  kept  strictly  in  confidence,  and  shall  not be  disclosed to or otherwise accessed by
//  any third party. @ <2003> - <2008>   The Software is provided "AS IS" without any warranty of any kind,
//  express, implied, statutory or otherwise.
//**********************************************************************************************************
//----------------------------------------------------------------------------------------------------
// ID Code      : Uart.c No.0000
// Update Note  :
//----------------------------------------------------------------------------------------------------
//#include <reg320.h>
#include <REG52.H>
#include "sparrow.h"
#include "sparrow\reg_def.h"
#include "char\uart.h"
#include "pcb_conf.h"
#include "rosprintf.h"

#define BAUDRATE_DIVISOR(x)		(_XTAL_CLOCK/(16*x))
extern STRUCT_UART_PARAMETER code stUartParameter[UART_SOURCE_TOTAL];
/*============================================================================*/
/**
 * drv_uart_Init
 * Initial uart, baud rate, data bits..,etc.	referenced from stUartParameter
 *
 * @param <bit>         { 0: use uart0, 1: uart1}
 * @return              	{ void }
 *
 */
void drv_uart_Init(bit nUart)
{
    UINT8 offset;
    STRUCT_UART_PARAMETER* ptUartPart;

    // Enable Clock & Release Reset
    if (!nUart)
    {
        // Enable Clock
        rtdf_setBitsb(SYS_CLKEN_2_0x2cc2, _BIT4);

        // Disable reset for Uart0
        rtdf_setBitsb(SYS_SRST_3_0x2ca3, _BIT1);

        offset = 0;
    }
    else
    {
        // Enable Clock
        rtdf_setBitsb(SYS_CLKEN_2_0x2cc2, _BIT5);

        // Disable reset for Uart1
        rtdf_setBitsb(SYS_SRST_3_0x2ca3, _BIT2);

        offset = 0x20;
    }
    ptUartPart = &stUartParameter[nUart];

    //set DLAB = 1
    rtdf_setBits(U0LCR_0x260c + offset, _BIT7);

    // Set Baudrate
//	rtdf_maskl(U0RBR_THR_DLL_0x2600 + offset, ~0xff, LOBYTE(BAUDRATE_DIVISOR(ptUartPart->BaudRate)));
//	rtdf_maskl(U0IER_DLH_0x2604 + offset, ~0xff, HIBYTE(BAUDRATE_DIVISOR(ptUartPart->BaudRate)));
    rtdf_maskl(U0RBR_THR_DLL_0x2600 + offset, ~0xff, 0x0f);
    rtdf_maskl(U0IER_DLH_0x2604 + offset, ~0xff, 00);

    // Set data bits, stop bits & parity bits
    rtdf_maskl(U0LCR_0x260c + offset, 0x00, (ptUartPart->Parity << 4) | (ptUartPart->Parity_enable << 3) | (ptUartPart->StopBits << 2) | ptUartPart->DataBits);

    // Set Receiver trigger levelTrigger level: 1bytes
    // DMA mode enable
    // Enable FIFO
    rtdf_maskl(U0IIR_FCR_0x2608 + offset, ~0xcf, 0x0f);

    //Disable all interrupt
    rtdf_maskl(U0IER_DLH_0x2604 + offset, ~0xff, 0x00);

    // R: MIS_U0MCR
    rtdf_maskl(U0MCR_0x2610 + offset, ~0xff, 0x00);

    // Write byte to buffer
    rtdf_maskl(U0RBR_THR_DLL_0x2600 + offset, ~0xff, 00);

//	RTD_Log(LOGGER_INFO, "drv_uart_Init baudrate %x ...\n", BAUDRATE_DIVISOR(ptUartPart->BaudRate));
}

/*============================================================================*/
/**
 * uart_Write
 * Write char data into Uart'buffer
 *
 * @param <ch>		{ data to write into uart'buffer }
 * @return 			{ void }
 *
 */
void drv_uart_Write(bit nUart, UINT8 ch)
{
    EA = 0;
    if (!nUart)
    {

        // Wait for empty
        while (!(rtdf_inl(U0LSR_0x2614) & _BIT6));

        // Write byte to buffer
        rtdf_maskl(U0RBR_THR_DLL_0x2600, ~0xff, ch);
    }
    else
    {
        // Wait for empty
        while (!(rtdf_inl(U1LSR_0x2634) & _BIT6));

        // Write byte to buffer
        rtdf_maskl(U1RBR_THR_DLL_0x2620, ~0xff, ch);
    }
    EA =1;
}

void drv_isr_uart_Write(bit nUart, UINT8 ch)
{
    if (!nUart)
    {
        // Wait for empty
        while (!(rtd_isr_inl(U0LSR_0x2614) & _BIT6));

        // Write byte to buffer
        rtd_isr_maskl(U0RBR_THR_DLL_0x2600, ~0xff, ch);
    }
    else
    {
        // Wait for empty
        while (!(rtd_isr_inl(U1LSR_0x2634) & _BIT6));

        // Write byte to buffer
        rtd_isr_maskl(U1RBR_THR_DLL_0x2620, ~0xff, ch);
    }
}
#if 0 // not used
/*============================================================================*/
/**
 * uart_Read
 * Read char data from Uart'buffer
 *
 * @param <ch>		{ data read from uart'buffer }
 * @return 			{ true if read valid data, otherwise 0 }
 *
 */
bit drv_uart_Read(bit nUart, UINT8* ch)
{
    if (!nUart)
    {
        if (rtdf_inl(U0LSR_0x2614) & _BIT0)
        {
            // Read byte from buffer
            *ch = (UINT8)(rtdf_inl(U0RBR_THR_DLL_0x2600) & 0xff);

            return _TRUE;
        }
    }
    else
    {
        if (rtdf_inl(U1LSR_0x2634) & _BIT0)
        {
            // Read byte from buffer
            *ch = (UINT8)(rtdf_inl(U1RBR_THR_DLL_0x2620) & 0xff);

            return _TRUE;
        }
    }
    return _FALSE;
}
#endif

bit drv_isr_uart_Read(bit nUart, UINT8* ch)
{
    if (!nUart)
    {
        if (rtd_isr_inl(U0LSR_0x2614) & _BIT0)
        {
            // Read byte from buffer
            *ch = (UINT8)(rtd_isr_inl(U0RBR_THR_DLL_0x2600) & 0xff);

            return _TRUE;
        }
    }
    else
    {
        if (rtd_isr_inl(U1LSR_0x2634) & _BIT0)
        {
            // Read byte from buffer
            *ch = (UINT8)(rtd_isr_inl(U1RBR_THR_DLL_0x2620) & 0xff);

            return _TRUE;
        }
    }
    return _FALSE;
}

