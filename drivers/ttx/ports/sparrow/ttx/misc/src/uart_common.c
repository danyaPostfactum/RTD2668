/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     uart_common.c
 * @brief    Prototypes of public application interface.
 * @details  This header declares the public access application interface for this TtLib library.
 * @author   Janus Cheng
 * @date     Dec. 11, 2007
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/

#include "../inc/uart.h"         /* Provides: functions to be implemented */
#include <ttx_port.h>               /* Provides: TTX_USE_PERI_IO_UART */
#include <stdlib.h>                 /* Provides: size_t */

#if TTX_USE_PERI_IO_UART
#include <PERIIO_HAL.h>
#include <sys.h>
#else
#include <REG52.H>                  /* special function register declarations   */
/* for the intended 8051 derivative         */
#endif

/* -- Prototypes of Private Functions ---------------------------------------*/


/* -- Implementations of Public Functions -----------------------------------*/

/*---------------------------------------------------------------------------*/
uint8_t
UART_RxData                     (uint8_t* dataRcvd)
/*****************************************************************************\
 *  @brief   The de-initialization routine.
 *  @details Use this function to de-initialize this library.
 *  @note    Must be called in advance of any other functions.
 *
 *  @return  the initialization status.
 *  @retval  TRUE  success
 *  @retval  FALSE failure
 *
 *  @ingroup ttx
 *  @see     TTX_Init()
\*****************************************************************************/
{
#if TTX_USE_PERI_IO_UART
    if (   (PERIIO_FGET(U0LSR, DR ) == PERIIO_U0LSR_DR_READY)  \
            &&(PERIIO_FGET(U0LSR, RFE) == PERIIO_U0LSR_RFE_OK) )
    {
        *dataRcvd = PERIIO_RGET( U0RBR_THR_DLL );

        return 1;
    }
#else
    if ( RI )
    {
        RI = 0;
        *dataRcvd = SBUF;
        return 1;
    }
#endif
    return 0;
}

/*---------------------------------------------------------------------------*/
void
UART_TxData                     (uint32_t ch, uint8_t mode)
/*****************************************************************************\
 *  @brief   The de-initialization routine.
 *  @details Use this function to de-initialize this library.
 *  @note    Must be called in advance of any other functions.
 *
 *  @return  the initialization status.
 *  @retval  TRUE  success
 *  @retval  FALSE failure
 *
 *  @ingroup ttx
 *  @see     TTX_Init()
\*****************************************************************************/
{
    uint8_t NO1[8], i, time;

//	ES = 0;
    if (mode == 0)
    {
#if TTX_USE_PERI_IO_UART
        PERIIO_RSET( U0RBR_THR_DLL, PERIIO_U0RBR_THR_DLL_RMK(0)( ch /* THD */ \
                                                               ) );
#else
        TI = 0;
        SBUF = ch; // transfer UART
        while (!TI);		// wait buffer completing.
#endif
    }
    else if (mode == 1) 	//dec
    {
        NO1[4] =  ch%10;
        NO1[3] = (ch%100)/10;
        NO1[2] = (ch%1000)/100;
        NO1[1] = (ch%10000)/1000;
        NO1[0] =  ch/10000;
        time = 0;
        for (i = 0; i < 5; i++)
        {
            if (NO1[i] != 0)
            {
#if TTX_USE_PERI_IO_UART
                PERIIO_RSET( U0RBR_THR_DLL, PERIIO_U0RBR_THR_DLL_RMK(0)( NO1[i]+'0' /* THD */ \
                                                                       ) );
#else
                TI = 0;
                SBUF = NO1[i] + '0'; // transfer UART
                while (!TI) ;	// wait buffer completing.
#endif
                time=1;
            }
            else if (time)
            {
#if TTX_USE_PERI_IO_UART
                PERIIO_RSET( U0RBR_THR_DLL, PERIIO_U0RBR_THR_DLL_RMK(0)( NO1[i]+'0' /* THD */ \
                                                                       ) );
#else
                TI = 0;
                SBUF = NO1[i] + '0'; // transfer UART
                while (!TI) ;	// wait buffer completing.
#endif
            }
        }
    }
    else if (mode == 2) 	//hex
    {
        const size_t MAX_DIGITS = sizeof(uint32_t)*2;    // each bytes is two hex
        size_t nValidDigits = MAX_DIGITS;
        /*
        		NO1[3] = ( ch & 0x0F);
        		NO1[2] = ((ch >> 1*4) & 0x0F);
        		NO1[1] = ((ch >> 2*4) & 0x0F);
        		NO1[0] = ((ch >> 3*4) & 0x0F);
        */
        NO1[7] = ( ch & 0x0F);
        NO1[6] = ((ch >> 1*4) & 0x0F);
        NO1[5] = ((ch >> 2*4) & 0x0F);
        NO1[4] = ((ch >> 3*4) & 0x0F);
        NO1[3] = ((ch >> 4*4) & 0x0F);
        NO1[2] = ((ch >> 5*4) & 0x0F);
        NO1[1] = ((ch >> 6*4) & 0x0F);
        NO1[0] = ((ch >> 7*4) & 0x0F);

        // Trim most significat zeros
        for (i = 0; i < MAX_DIGITS; i++)
        {
            if ( NO1[i] == 0 )
                nValidDigits--;
            else
                break;
        }

        // In case of all zeros, show at least one zero.
        if ( nValidDigits == 0 )
        {
            nValidDigits = 1;
        }

        for (i = MAX_DIGITS-nValidDigits; i < MAX_DIGITS; i++)
        {
#if TTX_USE_PERI_IO_UART
            if ( NO1[i] > 9 )
            {
                PERIIO_RSET( U0RBR_THR_DLL, PERIIO_U0RBR_THR_DLL_RMK(0)( NO1[i] - 10 + 'A' /* THD */ \
                                                                       ) );
            }
            else
            {
                PERIIO_RSET( U0RBR_THR_DLL, PERIIO_U0RBR_THR_DLL_RMK(0)( NO1[i] + '0' /* THD */ \
                                                                       ) );
            }

#else
            TI = 0;
            if (NO1[i] > 9)
                SBUF = NO1[i] - 10 + 'A';
            else
                SBUF = NO1[i] + '0';	 	// transfer UART

            while (!TI);					// wait buffer completing.
#endif
        }
    }
//	ES = 1;
}


/* -- Implementations of Private Functions ----------------------------------*/


/*---------------------------------------------------------------------------*\
 * end of uart_common.c                                                      *
\*---------------------------------------------------------------------------*/
