
/*************************************************************************************
*                                 Realtek's ?? Project
*
*    	  		 			  Module: Printf through IO device
*	 		 				(c) Copyright 2001-2007, Realtek Corp.
*                                      All Rights Reserved
*
*                                            V???
*
* File : ROSPrintf.c
*************************************************************************************
*/
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
GENERAL DESCRIPTION
     User printf code through IO device

     The functions in this file are used when enable the option
	 RTDBG_HAL_DEBUG_GDB_INCLUDE_STUBS. It impliments remote printf
	 to user's monitor (maybe Hyperterminal, RCL tool).


Copyright (c) 2001-2003 by Realtek Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

when           who         what, where, why
--------      -----      ----------------------------------------------------------
04/05/2004    Rinoa       Create this file.

===========================================================================*/


/*===========================================================================

                       INCLUDE FILES FOR MODULE

===========================================================================*/
#include "rosprintf.h"
#include "char\uart.h"
#include "pcb_customer.h"
#include "flow_conf_struct.h"
#include <stdarg.h>
#include "ui_struct.h"

#ifdef CONFIG_VBI_CC_SUPPORT
extern BOOL xdata enable_cc_verify;
#endif

extern volatile bit fDebugMode;
volatile UINT8 fEnableLog;
void RTD_Log(_LOGGER_LEVEL level, char code *format, ...)
{
    int xdata  i;
    UINT8  xdata getVal;
    bit fFirstNotZero;
    long xdata argint = 0;
    long xdata  base;
    va_list xdata  arglist;

#ifdef CONFIG_VBI_CC_SUPPORT
    if ( enable_cc_verify )
        return;
#endif

	//UINT8 ttb=0;

    if ((level > LOGGER_LEVEL) || fDebugMode || !fEnableLog||!ucUartPath)
        return;

    va_start(arglist,format);

    while (*format)
    {
        if (*format == '%')
        {
            switch  (*(format+1))
            {
            case 'd':
                argint = va_arg(arglist,long);
                if (argint == 0)
                {
                    drv_uart_Write(UART_MONITOR_SOURCE, '0');
                }
                else
                {
                    if (argint < 0)
                    {
                        drv_uart_Write(UART_MONITOR_SOURCE, '-');
                        argint = -argint;
                    }
                    base = 1;
                    i = 0;
                    while (argint / base)
                    {
                        base *= 10;
                        i++;
                    }
                    base /= 10;
                    while (i > 0)
                    {
                        getVal = argint / base;
                        argint = argint - (base * getVal);
                        getVal += '0';
                        base /= 10;
                        drv_uart_Write(UART_MONITOR_SOURCE, getVal);
                        i--;
                    }
                }
                break;

            case 'X':	// <Janus>2009/08/18: Add support to "%X"
            case 'x':
                argint = va_arg(arglist, long);
                drv_uart_Write(UART_MONITOR_SOURCE, '0');
                drv_uart_Write(UART_MONITOR_SOURCE, 'x');
                if (argint == 0)
                {
                    drv_uart_Write(UART_MONITOR_SOURCE, '0');
                }
                else
                {
                    i = 8;
                    fFirstNotZero = 0;
                    while (i > 0)
                    {
                        if (!fFirstNotZero)
                        {
                            getVal = ((argint & ((UINT32)0x0f) << ((i - 1) << 2)) >> ((i - 1) << 2)) & 0x0f;
                            if (getVal != 0)
                                fFirstNotZero = 1;
                        }

                        if (fFirstNotZero)
                        {
                            getVal = ((argint & ((UINT32)0x0f) << ((i - 1) << 2)) >> ((i - 1) << 2)) & 0x0f;
                            if (getVal > 9)
                                getVal = (getVal - 0x0a) + 'a';
                            else
                                getVal += '0';

                            drv_uart_Write(UART_MONITOR_SOURCE, getVal);
                        }
                        i--;

                    }
                }
                break;
            case 's':	// <Janus>2009/08/06: Add support to "%s"
            {
                char* szStr = va_arg( arglist, char* );
                while ( *szStr != '\0' )
                {
                    drv_uart_Write(UART_MONITOR_SOURCE, *szStr);
                    szStr++;
                }
                break;
            }
            }
            format+=2;
        }
        if (*format == 0x0a)
        {
            drv_uart_Write(UART_MONITOR_SOURCE, 0x0a);
            drv_uart_Write(UART_MONITOR_SOURCE, 0x0d);
            format++;
        }
        else
        {
            if (*format != 0)
                drv_uart_Write(UART_MONITOR_SOURCE, *format);
            format++;
        }
    }
}
