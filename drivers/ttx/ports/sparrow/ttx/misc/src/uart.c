/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     uart.c
 * @brief    Prototypes of public application interface.
 * @details  This header declares the public access application interface for this TtLib library.
 * @author   Janus Cheng
 * @date     Dec. 11, 2007
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/

#include "../inc/uart.h"        /* Provides: functions to be implemented */
#include <ttx_port.h>           /* Provides: TTX_USE_PERI_IO_UART */

#if TTX_USE_PERI_IO_UART
#include <PERIIO_HAL.h>
#include <PERIIO_HAL_TEST.h>
#define BAUD_RATE      (UINT32_C(115200))   // 115,200 bps    (for Peri-IO)
#else
#include <REG52.H>                  /* special function register declarations   */
/* for the intended 8051 derivative         */
#define BAUD_RATE      (UINT32_C( 57600))   //  57,600 bps    (for 8051 on-chip UART)
#endif


/* -- Prototypes of Private Functions ---------------------------------------*/


/* -- Implementations of Public Functions -----------------------------------*/

/*---------------------------------------------------------------------------*/
void
UART_Open (void)
/*****************************************************************************\
 *  @brief   The initialization routine.
 *  @details Use this function to initialize this library.
 *  @note    Must be called in advance of any other functions.
 *
 *  @return  the initialization status.
 *  @retval  TRUE  success
 *  @retval  FALSE failure
 *
 *  @ingroup ttx
 *  @see     TTX_Deinit()
\*****************************************************************************/
{
#if TTX_USE_PERI_IO_UART

    // DLAB == 1 -------------------------------------------------------------

    uint8_t dlh, dll;
    dlh = ((TTX_CPU_CLOCK/(UINT32_C(16)*BAUD_RATE))>>8) & 0xFF;
    dll = ((TTX_CPU_CLOCK/(UINT32_C(16)*BAUD_RATE))     & 0xFF) + 1;

    PERIIO_RSET( U0LCR, PERIIO_U0LCR_RMKS(   1   , /* DLAB */ \
                 DISABLE, /* BRK  */ \
                 EVEN , /* EPS  */ \
                 DISABLE, /* PEN  */ \
                 1BIT , /* STB  */ \
                 5BITS  /* WLS  */ \
                                         ) ); // _SYS_REG_WRITE( 0x260C, 0x90 );

    // Set DLL
    PERIIO_RSET( U0RBR_THR_DLL, PERIIO_U0RBR_THR_DLL_RMK(1)( dll /* DLL */ \
                                                           ) ); // _SYS_REG_WRITE( 0x2600, 0x14 );

    // Set DLH
    PERIIO_RSET( U0IER_DLH, PERIIO_U0IER_DLH_RMK (1)( dlh /* DLH */ \
                                                    ) ); // _SYS_REG_WRITE( 0x2604, 0x00 );



    // DLAB == 0 -------------------------------------------------------------

    PERIIO_RSET( U0LCR, PERIIO_U0LCR_RMKS(   0   , /* DLAB */ \
                 DISABLE, /* BRK  */ \
                 ODD  , /* EPS  */ \
                 DISABLE, /* PEN  */ \
                 1BIT , /* STB  */ \
                 8BITS  /* WLS  */ \
                                         ) ); // _SYS_REG_WRITE( 0x260C, 0x03 );

    PERIIO_RSET( U0IIR_FCR, PERIIO_U0IIR_FCR_RMKS(QUARTER , /* RTRG  */ \
                 ENABLE , /* DMAM  */ \
                 NOP  , /* TFRST */ \
                 NOP  , /* RFRST */ \
                 ENABLE   /* EFIFO */ \
                                                 ) ); // _SYS_REG_WRITE( 0x2608, 0x49 );

    PERIIO_RSET( U0IER_DLH, PERIIO_U0IER_DLH_RMKS(0)(DISABLE, /* EDSSI */ \
                 DISABLE, /* ELSI  */ \
                 ENABLE , /* ETBEI */ \
                 ENABLE   /* ERBFI */ \
                                                    ) ); // _SYS_REG_WRITE( 0x2604, 0x03 );


    PERIIO_RSET( U0MCR, PERIIO_U0MCR_RMKS( DISABLE  , /* LOOP */ \
                                           NO_REQUEST, /* RTS  */ \
                                           NOT_READY   /* DTR  */ \
                                         ) ); // _SYS_REG_WRITE( 0x2610, 0x00 );


#else
    // Disable all of 8051 interrupt
    EA = 0;

    //Stop Timer 2
    TR2 = 0;

//    PCON = 0x80;	// If SMOD = 1 then Y = 2 else Y = 1
    PCON = 0x00;	// If SMOD = 1 then Y = 2 else Y = 1

    //TMOD = 0x21;	//Time 1 Mode 2

    //RCAP2H=0xf2;		//Baud Rtae = clk/32*(65536-RCAP2H,RCAP2L)
    //RCAP2L=0xae;		//{RCAP2H,RCAP2L} = 65536-clk/32*Baud rate

    RCAP2H = ((UINT32_C(65536)-TTX_CPU_CLOCK/(32*BAUD_RATE))&0xFF00)>>8;
    RCAP2L = ((UINT32_C(65536)-TTX_CPU_CLOCK/(32*BAUD_RATE))&0xFF);

    SCON = 0x50;    // Enable UART, Mode-1

    T2CON=0x34;

    TR2=1;

    //Set  Y=2 , CRYSTAL = 27MHz , TH1=239  --> Baud Rate = 9600 bps
//    TH1 = 0x2;      // TH1 = 256 - (Y*CRYSTAL/384*BAUD RATE) -->CRYSTAL = 10MHz

//    TL1 = 0X38;


    IE = 0x00;		// Disable all interrupt
    //    IE = 0x82;   // Enable interrupts of Timer0 and Timer1
    // EA = 1;
    // ET1 = 0;

//    TR1 = 1;

//    ES = 1;
//    PS = 1;
//    bUartCommandSuccess = _FALSE;
//    bUartCommandStart = _FALSE;

//	DebugPrintf("RS232 System Initial Success!!!\n", 0);
//	DebugPrintf("Values == %x", 100);
#endif
}



/* -- Implementations of Private Functions ----------------------------------*/


/*---------------------------------------------------------------------------*\
 * end of uart.c                                                             *
\*---------------------------------------------------------------------------*/
