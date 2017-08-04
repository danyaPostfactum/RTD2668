/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     PERIIO_HAL_TEST.h
 * @brief    Compile-time tests for PERIIO HAL.
 * @details  Include this file to test CCL. No obj code will be generated.
 *             This file to be included in private CPP file.
 * @author   Janus Cheng
 * @date     Oct. 17, 2008
 * @version  0.1
\*****************************************************************************/

#include "PERIIO_HAL.h"             /* Provides: PERIIO HAL macros to be tested */
#include "../../inc/ttx_assert.h"   /* Provides: TTX_ASSERT_COMPILE() */


/* Test PERIIO_U0RBR_THR_DLL_REG */
TTX_ASSERT_COMPILE( PERIIO_U0RBR_THR_DLL_DEFAULT(0) == 0x00 );
TTX_ASSERT_COMPILE( PERIIO_U0RBR_THR_DLL_DEFAULT(1) == 0x00 );

TTX_ASSERT_COMPILE( PERIIO_U0RBR_THR_DLL_RMK(0)( 0x00 /* THD */ \
                                               ) == 0x00 );

TTX_ASSERT_COMPILE( PERIIO_U0RBR_THR_DLL_RMK(0)( 0x5A /* THD */ \
                                               ) == 0x5A );

TTX_ASSERT_COMPILE( PERIIO_U0RBR_THR_DLL_RMK(0)( 0xA5 /* THD */ \
                                               ) == 0xA5 );

TTX_ASSERT_COMPILE( PERIIO_U0RBR_THR_DLL_RMK(0)( 0xF0 /* THD */ \
                                               ) == 0xF0 );

TTX_ASSERT_COMPILE( PERIIO_U0RBR_THR_DLL_RMK(0)( 0x0F /* THD */ \
                                               ) == 0x0F );

TTX_ASSERT_COMPILE( PERIIO_U0RBR_THR_DLL_RMK(0)( 0xFF /* THD */ \
                                               ) == 0xFF );

TTX_ASSERT_COMPILE( PERIIO_U0RBR_THR_DLL_RMK(1)( 0x00 /* DLL */ \
                                               ) == 0x00 );

TTX_ASSERT_COMPILE( PERIIO_U0RBR_THR_DLL_RMK(1)( 0x5A /* DLL */ \
                                               ) == 0x5A );

TTX_ASSERT_COMPILE( PERIIO_U0RBR_THR_DLL_RMK(1)( 0xA5 /* DLL */ \
                                               ) == 0xA5 );

TTX_ASSERT_COMPILE( PERIIO_U0RBR_THR_DLL_RMK(1)( 0xF0 /* DLL */ \
                                               ) == 0xF0 );

TTX_ASSERT_COMPILE( PERIIO_U0RBR_THR_DLL_RMK(1)( 0x0F /* DLL */ \
                                               ) == 0x0F );

TTX_ASSERT_COMPILE( PERIIO_U0RBR_THR_DLL_RMK(1)( 0xFF /* DLL */ \
                                               ) == 0xFF );


/* Test PERIIO_U0IER_DLH_REG */
TTX_ASSERT_COMPILE( PERIIO_U0IER_DLH_DEFAULT(0) == 0x00 );
TTX_ASSERT_COMPILE( PERIIO_U0IER_DLH_DEFAULT(1) == 0x00 );

TTX_ASSERT_COMPILE( PERIIO_U0IER_DLH_RMKS(0)(DISABLE, /* EDSSI */ \
                    DISABLE, /* ELSI  */ \
                    DISABLE, /* ETBEI */ \
                    DISABLE  /* ERBFI */ \
                                            ) == 0x00 );

TTX_ASSERT_COMPILE( PERIIO_U0IER_DLH_RMKS(0)(ENABLE , /* EDSSI */ \
                    ENABLE , /* ELSI  */ \
                    ENABLE , /* ETBEI */ \
                    ENABLE   /* ERBFI */ \
                                            ) == 0x0F );


TTX_ASSERT_COMPILE( PERIIO_U0IER_DLH_RMKS(0)(ENABLE , /* EDSSI */ \
                    DISABLE, /* ELSI  */ \
                    DISABLE, /* ETBEI */ \
                    DISABLE  /* ERBFI */ \
                                            ) == 0x08 );

TTX_ASSERT_COMPILE( PERIIO_U0IER_DLH_RMKS(0)(DISABLE, /* EDSSI */ \
                    ENABLE , /* ELSI  */ \
                    DISABLE, /* ETBEI */ \
                    DISABLE  /* ERBFI */ \
                                            ) == 0x04 );

TTX_ASSERT_COMPILE( PERIIO_U0IER_DLH_RMKS(0)(DISABLE, /* EDSSI */ \
                    DISABLE, /* ELSI  */ \
                    ENABLE , /* ETBEI */ \
                    ENABLE   /* ERBFI */ \
                                            ) == 0x03 );

TTX_ASSERT_COMPILE( PERIIO_U0IER_DLH_RMKS(0)(DISABLE, /* EDSSI */ \
                    DISABLE, /* ELSI  */ \
                    ENABLE , /* ETBEI */ \
                    DISABLE  /* ERBFI */ \
                                            ) == 0x02 );

TTX_ASSERT_COMPILE( PERIIO_U0IER_DLH_RMKS(0)(DISABLE, /* EDSSI */ \
                    DISABLE, /* ELSI  */ \
                    DISABLE, /* ETBEI */ \
                    ENABLE   /* ERBFI */ \
                                            ) == 0x01 );

TTX_ASSERT_COMPILE( PERIIO_U0IER_DLH_RMK (1)( 0x00 /* DLH */ \
                                            ) == 0x00 );

TTX_ASSERT_COMPILE( PERIIO_U0IER_DLH_RMK (1)( 0xFF /* DLH */ \
                                            ) == 0xFF );

TTX_ASSERT_COMPILE( PERIIO_U0IER_DLH_RMK (1)( 0x5A /* DLH */ \
                                            ) == 0x5A );

TTX_ASSERT_COMPILE( PERIIO_U0IER_DLH_RMK (1)( 0xA5 /* DLH */ \
                                            ) == 0xA5 );

TTX_ASSERT_COMPILE( PERIIO_U0IER_DLH_RMK (1)( 0x0F /* DLH */ \
                                            ) == 0x0F );

TTX_ASSERT_COMPILE( PERIIO_U0IER_DLH_RMK (1)( 0xF0 /* DLH */ \
                                            ) == 0xF0 );


/* Test PERIIO_U0IIR_FCR_REG */
TTX_ASSERT_COMPILE( PERIIO_U0IIR_FCR_DEFAULT == 0x00 );

TTX_ASSERT_COMPILE( PERIIO_U0IIR_FCR_RMKS(ONE_CHAR, /* RTRG  */ \
                    DISABLE , /* DMAM  */ \
                    NOP  , /* TFRST */ \
                    NOP  , /* RFRST */ \
                    DISABLE   /* EFIFO */ \
                                         ) == 0x00 );

TTX_ASSERT_COMPILE( PERIIO_U0IIR_FCR_RMKS(  FULL  , /* RTRG  */ \
                    ENABLE , /* DMAM  */ \
                    RESET  , /* TFRST */ \
                    RESET  , /* RFRST */ \
                    ENABLE   /* EFIFO */ \
                                         ) == 0xCF );

TTX_ASSERT_COMPILE( PERIIO_U0IIR_FCR_RMKS(  HALF  , /* RTRG  */ \
                    DISABLE , /* DMAM  */ \
                    RESET  , /* TFRST */ \
                    NOP  , /* RFRST */ \
                    ENABLE   /* EFIFO */ \
                                         ) == 0x85 );

TTX_ASSERT_COMPILE( PERIIO_U0IIR_FCR_RMKS(QUARTER , /* RTRG  */ \
                    ENABLE , /* DMAM  */ \
                    NOP  , /* TFRST */ \
                    RESET  , /* RFRST */ \
                    DISABLE   /* EFIFO */ \
                                         ) == 0x4A );

TTX_ASSERT_COMPILE( PERIIO_U0IIR_FCR_RMKS(QUARTER , /* RTRG  */ \
                    ENABLE , /* DMAM  */ \
                    NOP  , /* TFRST */ \
                    NOP  , /* RFRST */ \
                    ENABLE   /* EFIFO */ \
                                         ) == 0x49 );



/* Test PERIIO_U0LCR_REG */
TTX_ASSERT_COMPILE( PERIIO_U0LCR_DEFAULT == 0x00 );

TTX_ASSERT_COMPILE( PERIIO_U0LCR_RMKS(   0   , /* DLAB */ \
                    DISABLE, /* BRK  */ \
                    ODD  , /* EPS  */ \
                    DISABLE, /* PEN  */ \
                    1BIT , /* STB  */ \
                    5BITS  /* WLS  */ \
                                     ) == 0x00 );

TTX_ASSERT_COMPILE( PERIIO_U0LCR_RMKS(   1   , /* DLAB */ \
                    ENABLE, /* BRK  */ \
                    SPACE , /* EPS  */ \
                    ENABLE, /* PEN  */ \
                    2BITS, /* STB  */ \
                    8BITS  /* WLS  */ \
                                     ) == 0xFF );

TTX_ASSERT_COMPILE( PERIIO_U0LCR_RMKS(   0   , /* DLAB */ \
                    ENABLE , /* BRK  */ \
                    EVEN  , /* EPS  */ \
                    ENABLE , /* PEN  */ \
                    1BIT , /* STB  */ \
                    7BITS  /* WLS  */ \
                                     ) == 0x5A );

TTX_ASSERT_COMPILE( PERIIO_U0LCR_RMKS(   1   , /* DLAB */ \
                    DISABLE, /* BRK  */ \
                    MARK , /* EPS  */ \
                    DISABLE, /* PEN  */ \
                    2BITS, /* STB  */ \
                    6BITS  /* WLS  */ \
                                     ) == 0xA5 );

TTX_ASSERT_COMPILE( PERIIO_U0LCR_RMKS(   1   , /* DLAB */ \
                    DISABLE, /* BRK  */ \
                    EVEN , /* EPS  */ \
                    DISABLE, /* PEN  */ \
                    1BIT , /* STB  */ \
                    5BITS  /* WLS  */ \
                                     ) == 0x90 );

TTX_ASSERT_COMPILE( PERIIO_U0LCR_RMKS(   0   , /* DLAB */ \
                    DISABLE, /* BRK  */ \
                    ODD  , /* EPS  */ \
                    DISABLE, /* PEN  */ \
                    1BIT , /* STB  */ \
                    8BITS  /* WLS  */ \
                                     ) == 0x03 );


/* Test PERIIO_U0MCR_REG */
TTX_ASSERT_COMPILE( PERIIO_U0MCR_DEFAULT == 0x00 );

TTX_ASSERT_COMPILE( PERIIO_U0MCR_RMKS( DISABLE  , /* LOOP */ \
                                       NO_REQUEST, /* RTS  */ \
                                       NOT_READY   /* DTR  */ \
                                     ) == 0x00 );

TTX_ASSERT_COMPILE( PERIIO_U0MCR_RMKS(  ENABLE  , /* LOOP */ \
                                        NO_REQUEST, /* RTS  */ \
                                        NOT_READY   /* DTR  */ \
                                     ) == 0x10 );


/* Test PERIIO_U0LSR_REG */
TTX_ASSERT_COMPILE( PERIIO_U0LSR_DEFAULT == 0x60 );


/*-------------------------------------------------------------------------*\
 * End of PERIIO_HAL_TEST.h                                                *
\*-------------------------------------------------------------------------*/

