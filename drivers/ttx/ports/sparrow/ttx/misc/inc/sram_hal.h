/*-------------------------------------------------------------------------*\
 *               File: sram_hal.h
 *                Ver: 0.1
 *             Author: Chi-Chun Cheng
 *          Objective: Provides the Realtek RTD-2668 Sparrow Video Decoder
 *                       Hardware Abstract Layer functionalities.
 *-------------------------------------------------------------------------
 *          Registers:
 *
 *              TBR  - Transmitter Buffer Register
 *              RBR  - Receiver Buffer Register
 *              UCR  - UART Control Register
 *              USR  - UART Status Register
 *              MCR  - Modem Control Register
 *              BRSR - Baud Rate Select Register
 *              MSR  - Modem Status Register
 *-------------------------------------------------------------------------
 *  Macros that Access Registers and Fields:
 *
 *   SRAM_ADDR   (<REG>)                  -   Register address
 *   SRAM_RGET   (<REG>)                  -   Returns the value in the peripheral register
 *   SRAM_RSET   (<REG>,x)                -   Register set
 *   SRAM_FGET   (<REG>,<FIELD>)          -   Returns the value of the specified field
 *                                             in the peripheral register
 *   SRAM_FSET   (<REG>,<FIELD>,fieldval) -   Writes fieldval to the specified field
 *                                             in the peripheral register
 *   SRAM_FSETS  (<REG>,<FIELD>,<SYM>)    -   Writes the symbol value to the specified
 *                                             field in the peripheral
 *   SRAM_RGETA  (addr,<REG>)             -   Gets register for a given address
 *   SRAM_RSETA  (addr,<REG>,x)           -   Sets register for a given address
 *
 *  <NOTE>
 *  Not completed yet!!!!!!!!!!
\*-------------------------------------------------------------------------*/

#ifndef	__C_CHIP_LIB_REALTEK_RTD_2668_STATIC_RANDOM_ACCESS_MEMORY_HARDWARE_ABSTRACT_LAYER_HEADER_INCLUDED__
#define	__C_CHIP_LIB_REALTEK_RTD_2668_STATIC_RANDOM_ACCESS_MEMORY_HARDWARE_ABSTRACT_LAYER_HEADER_INCLUDED__ (1)


/* CCL headers ------------------------------------------------------------*/
#include <ccl/std.h>    /* the standard header of C Chip Library */

/*
 *  The function used for read register from this IP. Prototype is
 *     uint8_t SRAM_Read( addr );
 */
#define SRAM_Read   TTX_Reg8Read

/*
 *  The function used for read register from this IP. Prototype is
 *     void    SRAM_Write( addr, val );
 */
#define SRAM_Write  TTX_Reg8Write



#endif	/* !__C_CHIP_LIB_REALTEK_RTD_2668_STATIC_RANDOM_ACCESS_MEMORY_HARDWARE_ABSTRACT_LAYER_HEADER_INCLUDED__ */


/*-------------------------------------------------------------------------*\
 * End of sram_hal.h                                                        *
\*-------------------------------------------------------------------------*/

