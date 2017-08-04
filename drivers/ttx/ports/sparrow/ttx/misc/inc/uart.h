/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     UART.h
 * @brief    Internal (package scope) Ttx/C interface.
 * @details  This header declares the platform-dependent interface for this ttx library.
 * @author   Janus Cheng
 * @date     Dec. 21, 2007
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/

/**
 * @addtogroup ttx
 * @{
 */
#ifndef __8051_UNIVERSAL_ASNYC_RECEIVER_AND_TRANSMITTER_HEADER_INCLUDED__
#define __8051_UNIVERSAL_ASNYC_RECEIVER_AND_TRANSMITTER_HEADER_INCLUDED__ (1)

#include "periio_hal.h" /* Provides: Peripheral I/O HAL */
#include <ttx_port.h>  /* Provides: uint8_t */

#ifdef __cplusplus
extern "C"
{
#endif

    /* -- Global Macro Definitions ----------------------------------------------*/

    /* -- Global typedef Declarations -------------------------------------------*/

    /* Initialization configuration structure */


    /* -- Function Prototypes ---------------------------------------------------*/

    void                    UART_Open                       (void);
    void                    UART_TxData                     (uint32_t d, uint8_t mode);
    uint8_t                 UART_RxData                     (uint8_t* dataRcvd);


#ifdef __cplusplus
}   /* extern "C" */
#endif



#endif /* !__8051_UNIVERSAL_ASNYC_RECEIVER_AND_TRANSMITTER_HEADER_INCLUDED__ */

/**
 * @}
 */

/*---------------------------------------------------------------------------*\
 * End of UART.h                                                          *
\*---------------------------------------------------------------------------*/
