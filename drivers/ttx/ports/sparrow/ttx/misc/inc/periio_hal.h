/*-------------------------------------------------------------------------*\
 *               File: PERIIO_HAL.h
 *                Ver: 0.1
 *             Author: Chi-Chun Cheng
 *          Objective: Provides the 82C52 Programmable UART
 *                       (Universal Asynchronous Receiver/Transmitter)
 *                       Controller and Baud Rate Generator (BRG)
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
 *   PERIIO_ADDR   (<REG>)                  -   Register address
 *   PERIIO_RGET   (<REG>)                  -   Returns the value in the peripheral register
 *   PERIIO_RSET   (<REG>,x)                -   Register set
 *   PERIIO_FGET   (<REG>,<FIELD>)          -   Returns the value of the specified field
 *                                             in the peripheral register
 *   PERIIO_FSET   (<REG>,<FIELD>,fieldval) -   Writes fieldval to the specified field
 *                                             in the peripheral register
 *   PERIIO_FSETS  (<REG>,<FIELD>,<SYM>)    -   Writes the symbol value to the specified
 *                                             field in the peripheral
 *   PERIIO_RGETA  (addr,<REG>)             -   Gets register for a given address
 *   PERIIO_RSETA  (addr,<REG>,x)           -   Sets register for a given address
 *
 *  <NOTE>
 *  Not completed yet!!!!!!!!!!
\*-------------------------------------------------------------------------*/

#ifndef	__C_CHIP_LIB_REALTEK_PERIPHERAL_INPUT_OUTPUT_HARDWARE_ABSTRACT_LAYER_HEADER_INCLUDED__
#define	__C_CHIP_LIB_REALTEK_PERIPHERAL_INPUT_OUTPUT_HARDWARE_ABSTRACT_LAYER_HEADER_INCLUDED__ (1)


/* CCL headers ------------------------------------------------------------*/
#include <ccl/std.h>    /* the standard header of C Chip Library */

/*
 *  Base address of PERIIO
 */
#define _PERIIO_ADDR_BASE  (0x2500u)

/*
 *  The function used for read register from this IP. Prototype is
 *     uint32_t _PERIIO_REG_READ( addr );
 */
#define _PERIIO_REG_READ(addr)         TTX_Reg32Read(addr)

/*
 *  The function used for read register from this IP. Prototype is
 *     void    _PERIIO_REG_WRITE( addr, val );
 */
#define _PERIIO_REG_WRITE(addr,val)    TTX_Reg32Write(addr,val)

/*
 * The shift left of data-bus connection in bits, e.g.,
 *   conventionally, the D0-D7 of PERIIO are connected to CPU data-bus D0-D7 (_PERIIO_DB_SHFT = 0).
 *   However, in some cases,
 *     the D0-D7 of 82C52 are connected to CPU data-bus D16-D24 (_PERIIO_DB_SHFT = 16).
 */
#define _PERIIO_DB_SHFT     (0)

/*
 * The width of data-bus connection in bits, e.g.,
 *   conventionally, the D0-D7 of 82C52 are connected to CPU data-bus D0-D7 (_PERIIO_DB_WIDTH = 0x00FFu).
 *   Please note, despite the shift of data-bus, _PERIIO_DB_WITH starts from LSB.
 *   As a consequence,
 *     if the D0-D7 of 82C52 are connected to CPU data-bus D16-D24 (_PERIIO_DB_WIDTH = 0x00FFu).
 */
#define _PERIIO_DB_WIDTH   (0x000000FFu)

/*
 * The shift left of address-bus connection in bits, e.g.,
 *   conventionally, the A0-A7 of 82C52 are connected to CPU address-bus A0-A7 (_PERIIO_AB_SHFT = Uint8).
 *   However, in some cases,
 *     the A0-A7 of 82C52 are connected to CPU data-bus A2-A9 (_PERIIO_DB_SHFT = uint32_t).
 */
#define _PERIIO_AB_SHFT     uint16_t




/* Field related macros */

#define PERIIO_FMK(REG,FIELD,x)\
	_CCL_PER_FMK(PERIIO,REG,FIELD,x)

#define PERIIO_FMKS(REG,FIELD,SYM)\
	_CCL_PER_FMKS(PERIIO,REG,FIELD,SYM)

#define PERIIO_FEXTRACT(REG,FIELD,x)\
	_CCL_PER_FEXTRACT(PERIIO,REG,FIELD,x)


/* Raw register/field access macros */

#define PERIIO_ADDR(REG)\
    (_PERIIO_ADDR_BASE+_PERIIO_##REG##_OFFSET)

#define PERIIO_RGET(REG)\
    _PERIIO_RGETA(PERIIO_ADDR(REG),REG)

#define PERIIO_RSET(REG,x)\
    _PERIIO_RSETA(PERIIO_ADDR(REG),REG,x)

#define PERIIO_FGET(REG,FIELD)\
    _PERIIO_FGETA(PERIIO_ADDR(REG),REG,FIELD)

#define PERIIO_FSET(REG,FIELD,x)\
    _PERIIO_FSETA(PERIIO_ADDR(REG),REG,FIELD,x)

#define PERIIO_FSETS(REG,FIELD,SYM)\
    _PERIIO_FSETSA(PERIIO_ADDR(REG),REG,FIELD,SYM)


/* Address based register/field access macros */

#define _PERIIO_RGETA(addr,REG)\
	_CCL_PER_RGET(addr,PERIIO,REG,_PERIIO_DB_SHFT,_PERIIO_DB_WIDTH)

#define _PERIIO_RSETA(addr,REG,x)\
	_CCL_PER_RSET(addr,PERIIO,REG,x,_PERIIO_DB_SHFT,_PERIIO_DB_WIDTH)

#define _PERIIO_RSETSA(addr,REG,SYM)\
	_CCL_PER_RSETS(addr,PERIIO,REG,SYM,_PERIIO_DB_SHFT,_PERIIO_DB_WIDTH)

#define _PERIIO_FGETA(addr,REG,FIELD)\
	_CCL_PER_FGET(addr,PERIIO,REG,FIELD,_PERIIO_DB_SHFT,_PERIIO_DB_WIDTH)

#define _PERIIO_FSETA(addr,REG,FIELD,x)\
	_CCL_PER_FSET(addr,PERIIO,REG,FIELD,x,_PERIIO_DB_SHFT,_PERIIO_DB_WIDTH)

#define _PERIIO_FSETSA(addr,REG,FIELD,SYM)\
	_CCL_PER_FSETS(addr,PERIIO,REG,FIELD,SYM,_PERIIO_DB_SHFT,_PERIIO_DB_WIDTH)


/* Handle based register/field access macros */

/* This is for base-address determined by handle h.
#define PERIIO_ADDRH(h,REG)\
	(_PERIIO_AB_SHFT)(&((h)->addrBase[_PERIIO_##REG##_OFFSET]))
*/
#define PERIIO_ADDRH(h,REG)\
	(_PERIIO_AB_SHFT)(_PERIIO_ADDR_BASE+_PERIIO_##REG##_OFFSET)

#define PERIIO_RGETH(h,REG)\
	_PERIIO_RGETA(PERIIO_ADDRH(h,REG),REG)

#define PERIIO_RSETH(h,REG,x)\
	_PERIIO_RSETA(PERIIO_ADDRH(h,REG),REG,x)

#define PERIIO_RSETSH(h,REG,SYM)\
	_PERIIO_RSETSA(PERIIO_ADDRH(h,REG),REG,SYM)

#define PERIIO_FGETH(h,REG,FIELD)\
	_PERIIO_FGETA(PERIIO_ADDRH(h,REG),REG,FIELD)

#define PERIIO_FSETH(h,REG,FIELD,x)\
	_PERIIO_FSETA(PERIIO_ADDRH(h,REG),REG,FIELD,x)

#define PERIIO_FSETSH(h,REG,FIELD,SYM)\
	_PERIIO_FSETSA(PERIIO_ADDRH(h,REG),REG,FIELD,SYM)



/* Function Prototypes ----------------------------------------------------*/

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  U0RBR_THR_DLL    |
 * |___________________|
 *
 * U0RBR_THR_DLL - UART0 Receiver Buffer Register/Transmitter Holding Data/Divisor Latch LSB
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * When DLAB=0 ------------------------------------------------------------
 * (r)   RBD (Receiver Buffer Data)
 * (w)   THD (Transmitter Holding Data)
 *
 * When DLAB=1 ------------------------------------------------------------
 * (rw)  DLL (Divisor Latch LSB)
\*-------------------------------------------------------------------------*/
#define _PERIIO_U0RBR_THR_DLL_OFFSET                 (0x0100u)
#define _PERIIO_U0RBR_THR_DLL_ADDRH(h)               PERIIO_ADDRH(h,U0RBR_THR_DLL)
#define _PERIIO_U0RBR_THR_DLL_LEN                    (8)                 /* 8-bit*/
#define _PERIIO_U0RBR_THR_DLL_TYPE                    uint8_t

#define _PERIIO_U0RBR_THR_DLL_RBD_MASK               (0xFFu)
#define _PERIIO_U0RBR_THR_DLL_RBD_SHIFT              (0)
#define  PERIIO_U0RBR_THR_DLL_RBD_DEFAULT            (0x00u)
#define  PERIIO_U0RBR_THR_DLL_RBD_OF(x)              _CCL_VALUEOF(_PERIIO_U0RBR_THR_DLL_TYPE,x)

#define _PERIIO_U0RBR_THR_DLL_THD_MASK               (0xFFu)
#define _PERIIO_U0RBR_THR_DLL_THD_SHIFT              (0)
#define  PERIIO_U0RBR_THR_DLL_THD_DEFAULT            (0x00u)
#define  PERIIO_U0RBR_THR_DLL_THD_OF(x)              _CCL_VALUEOF(_PERIIO_U0RBR_THR_DLL_TYPE,x)

#define _PERIIO_U0RBR_THR_DLL_DLL_MASK               (0xFFu)
#define _PERIIO_U0RBR_THR_DLL_DLL_SHIFT              (0)
#define  PERIIO_U0RBR_THR_DLL_DLL_DEFAULT            (0x00u)
#define  PERIIO_U0RBR_THR_DLL_DLL_OF(x)              _CCL_VALUEOF(_PERIIO_U0RBR_THR_DLL_TYPE,x)

#define  PERIIO_U0RBR_THR_DLL_OF(x)             _CCL_VALUEOF(_PERIIO_U0RBR_THR_DLL_TYPE,x)

/* default value of U0IER_DLH register when DLAB == 0 */
#define  PERIIO_U0RBR_THR_DLL_DEFAULT_DLAB0 (_PERIIO_U0RBR_THR_DLL_TYPE)(  \
	 _CCL_PER_FDEFAULT(PERIIO,U0RBR_THR_DLL,RBD)                           \
	|_CCL_PER_FDEFAULT(PERIIO,U0RBR_THR_DLL,THD)                           \
	)

/* default value of U0IER_DLH register when DLAB == 1 */
#define  PERIIO_U0RBR_THR_DLL_DEFAULT_DLAB1 (_PERIIO_U0RBR_THR_DLL_TYPE)(  \
	 _CCL_PER_FDEFAULT(PERIIO,U0RBR_THR_DLL,DLL)                           \
	)

#define PERIIO_U0RBR_THR_DLL_RMK_DLAB0(thd) (_PERIIO_U0RBR_THR_DLL_TYPE)( \
	 _CCL_PER_FMK  (PERIIO,U0RBR_THR_DLL,THD,thd)                         \
	)

#define PERIIO_U0RBR_THR_DLL_RMK_DLAB1(dll) (_PERIIO_U0RBR_THR_DLL_TYPE)( \
	 _CCL_PER_FMK  (PERIIO,U0RBR_THR_DLL,DLL,dll)                         \
	)

#define PERIIO_U0RBR_THR_DLL_DEFAULT(dlab)  PERIIO_U0RBR_THR_DLL_DEFAULT_DLAB##dlab
#define PERIIO_U0RBR_THR_DLL_RMK(dlab)      PERIIO_U0RBR_THR_DLL_RMK_DLAB##dlab


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  U0IER_DLH        |
 * |___________________|
 *
 * U0IER_DLH - UART0 Interrupt Enable/Divisor Latch High-byte Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * When DLAB=0 ------------------------------------------------------------
 * (rw)  EDSSI - Enable modem status register interrupt
 *                  EDSSI  |   description
 *                ---------+-------------------
 *                 DISABLE |   disable
 *                 ENABLE  |   enable
 *
 * (rw)  ELSI - Enable receiver line status interrupt
 *                  ELSI   |   description
 *                ---------+-------------------
 *                 DISABLE |   disable
 *                 ENABLE  |   enable
 *
 * (rw)  ETBEI - Enable transmitter holding register empty interrupt
 *                  ETBEI  |   description
 *                ---------+-------------------
 *                 DISABLE |   disable
 *                 ENABLE  |   enable
 *
 * (rw)  ERBFI - Enable received data available interrupt
 *                  ERBFI  |   description
 *                ---------+-------------------
 *                 DISABLE |   disable
 *                 ENABLE  |   enable
 *
 * When DLAB=1 ------------------------------------------------------------
 * (rw)  DLH (Divisor Latch High-byte)
\*-------------------------------------------------------------------------*/
#define _PERIIO_U0IER_DLH_OFFSET                   (0x0104u)
#define _PERIIO_U0IER_DLH_ADDRH(h)                 PERIIO_ADDRH(h,U0IER_DLH)
#define _PERIIO_U0IER_DLH_LEN                      (8)                 /* 8-bit*/
#define _PERIIO_U0IER_DLH_TYPE                     uint8_t

#define _PERIIO_U0IER_DLH_EDSSI_MASK               (0x08u)
#define _PERIIO_U0IER_DLH_EDSSI_SHIFT              (3)
#define  PERIIO_U0IER_DLH_EDSSI_DEFAULT            (0x0u)
#define  PERIIO_U0IER_DLH_EDSSI_OF(x)              _CCL_VALUEOF(_PERIIO_U0IER_DLH_TYPE,x)
#define  PERIIO_U0IER_DLH_EDSSI_DISABLE            (0x00u)
#define  PERIIO_U0IER_DLH_EDSSI_ENABLE             (0x01u)

#define _PERIIO_U0IER_DLH_ELSI_MASK                (0x04u)
#define _PERIIO_U0IER_DLH_ELSI_SHIFT               (2)
#define  PERIIO_U0IER_DLH_ELSI_DEFAULT             (0x0u)
#define  PERIIO_U0IER_DLH_ELSI_OF(x)               _CCL_VALUEOF(_PERIIO_U0IER_DLH_TYPE,x)
#define  PERIIO_U0IER_DLH_ELSI_DISABLE             (0x00u)
#define  PERIIO_U0IER_DLH_ELSI_ENABLE              (0x01u)

#define _PERIIO_U0IER_DLH_ETBEI_MASK               (0x02u)
#define _PERIIO_U0IER_DLH_ETBEI_SHIFT              (1)
#define  PERIIO_U0IER_DLH_ETBEI_DEFAULT            (0x0u)
#define  PERIIO_U0IER_DLH_ETBEI_OF(x)              _CCL_VALUEOF(_PERIIO_U0IER_DLH_TYPE,x)
#define  PERIIO_U0IER_DLH_ETBEI_DISABLE            (0x00u)
#define  PERIIO_U0IER_DLH_ETBEI_ENABLE             (0x01u)

#define _PERIIO_U0IER_DLH_ERBFI_MASK               (0x01u)
#define _PERIIO_U0IER_DLH_ERBFI_SHIFT              (0)
#define  PERIIO_U0IER_DLH_ERBFI_DEFAULT            (0x0u)
#define  PERIIO_U0IER_DLH_ERBFI_OF(x)              _CCL_VALUEOF(_PERIIO_U0IER_DLH_TYPE,x)
#define  PERIIO_U0IER_DLH_ERBFI_DISABLE            (0x00u)
#define  PERIIO_U0IER_DLH_ERBFI_ENABLE             (0x01u)

#define _PERIIO_U0IER_DLH_DLH_MASK                 (0xFFu)
#define _PERIIO_U0IER_DLH_DLH_SHIFT                (0)
#define  PERIIO_U0IER_DLH_DLH_DEFAULT              (0x00u)
#define  PERIIO_U0IER_DLH_DLH_OF(x)                _CCL_VALUEOF(_PERIIO_U0IER_DLH_TYPE,x)

#define  PERIIO_U0IER_DLH_OF(x)             _CCL_VALUEOF(_PERIIO_U0IER_DLH_TYPE,x)


/* default value of U0IER_DLH register when DLAB == 0 */
#define  PERIIO_U0IER_DLH_DEFAULT_DLAB0 (_PERIIO_U0IER_DLH_TYPE)(  \
	 _CCL_PER_FDEFAULT(PERIIO,U0IER_DLH,EDSSI)                     \
	|_CCL_PER_FDEFAULT(PERIIO,U0IER_DLH,ELSI )                     \
	|_CCL_PER_FDEFAULT(PERIIO,U0IER_DLH,ETBEI)                     \
	|_CCL_PER_FDEFAULT(PERIIO,U0IER_DLH,ERBFI)                     \
	)

/* default value of U0IER_DLH register when DLAB == 1 */
#define  PERIIO_U0IER_DLH_DEFAULT_DLAB1 (_PERIIO_U0IER_DLH_TYPE)(  \
	 _CCL_PER_FDEFAULT(PERIIO,U0IER_DLH,DLH)                       \
	)

#define PERIIO_U0IER_DLH_RMK_DLAB0(edssi,elsi,etbei,erbfi) (_PERIIO_U0IER_DLH_TYPE)( \
	 _CCL_PER_FMK  (PERIIO,U0IER_DLH,EDSSI,edssi)                        \
	|_CCL_PER_FMK  (PERIIO,U0IER_DLH,ELSI ,elsi )                        \
	|_CCL_PER_FMK  (PERIIO,U0IER_DLH,ETBEI,etbei)                        \
	|_CCL_PER_FMK  (PERIIO,U0IER_DLH,ERBFI,erbfi)                        \
	)

#define PERIIO_U0IER_DLH_RMKS_DLAB0(edssi,elsi,etbei,erbfi) (_PERIIO_U0IER_DLH_TYPE)( \
	 _CCL_PER_FMKS (PERIIO,U0IER_DLH,EDSSI,edssi)                        \
	|_CCL_PER_FMKS (PERIIO,U0IER_DLH,ELSI ,elsi )                        \
	|_CCL_PER_FMKS (PERIIO,U0IER_DLH,ETBEI,etbei)                        \
	|_CCL_PER_FMKS (PERIIO,U0IER_DLH,ERBFI,erbfi)                        \
	)

#define PERIIO_U0IER_DLH_RMK_DLAB1(dlh) (_PERIIO_U0IER_DLH_TYPE)( \
	 _CCL_PER_FMK  (PERIIO,U0IER_DLH,DLH,dlh)                     \
	)

#define PERIIO_U0IER_DLH_RMKS_DLAB1(dlh) (_PERIIO_U0IER_DLH_TYPE)( \
	 _CCL_PER_FMK  (PERIIO,U0IER_DLH,DLH,dlh)                      \
	)

#define PERIIO_U0IER_DLH_DEFAULT(dlab)  PERIIO_U0IER_DLH_DEFAULT_DLAB##dlab
#define PERIIO_U0IER_DLH_RMK(dlab)      PERIIO_U0IER_DLH_RMK_DLAB##dlab
#define PERIIO_U0IER_DLH_RMKS(dlab)     PERIIO_U0IER_DLH_RMKS_DLAB##dlab


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  U0IIR_FCR        |
 * |___________________|
 *
 * U0IIR_FCR - UART0 Interrupt Identification Register / FIFO Control Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * When Read --------------------------------------------------------------
 * (r)   FIFO16 - FIFO enable
 *                 FIFO16  |   description
 *                ---------+-------------------
 *                 DISABLE |   no FIFO
 *                 ENABLE  |   16-byte FIFO
 *
 * (r)   IID - Interrupt ID
 *                   IID          |   description
 *           ---------------------+-----------------------------------------
 *           MODEM_STATUS_CHANGED |
 *           THR_EMPTY            | transmit holding register (THR) is empty
 *           RCVD_DATA_AVAIL      | received data available
 *           RCVR_STATUS          | receiver status
 *           CHAR_TIME_OUT        | character time out
 *
 * (r)   IPND - Interrupt Pending
 *                   IPND  |   description
 *                ---------+-------------------
 *                  ASSERT | interrupt pending
 *                DEASSERT |
 *
 * When Write -------------------------------------------------------------
 * (w)   RTRG (Receiver Trigger Level)
 *                  RTRG   |   description
 *                ---------+-------------------
 *                ONE_CHAR | one character in FIFO
 *                QUARTER  | four bytes in FIFO
 *                  HALF   | eight bytes in FIFO
 *                  FULL   | sixteen bytes in FIFO
 *
 * (w)   DMAM (DMA Mode)
 *                  DMAM   |   description
 *                ---------+-------------------
 *                 DISABLE | disable DMA
 *                  ENABLE | enable DMA
 *
 * (w)   TFRST (Transmitter FIFO Reset)
 *                  TFRST  |   description
 *                ---------+-------------------
 *                   NOP   | No operation
 *                  RESET  | Clear transmitter FIFO.
 *
 * (w)   RFRST (Receiver FIFO Reset)
 *                  RFRST  |   description
 *                ---------+-------------------
 *                   NOP   | No operation
 *                  RESET  | Clear receiver FIFO.
 *
 * (w)   EFIFO (Enable FIFO) - Changing this bit clears the FIFOs.
 *                  EFIFO  |   description
 *                ---------+-------------------
 *                 DISABLE | disable FIFOs
 *                  ENABLE | enable the transmitter and receiver FIFOs.
\*-------------------------------------------------------------------------*/
#define _PERIIO_U0IIR_FCR_OFFSET                   (0x0108u)
#define _PERIIO_U0IIR_FCR_ADDRH(h)                 PERIIO_ADDRH(h,U0IIR_FCR)
#define _PERIIO_U0IIR_FCR_LEN                      (8)                 /* 8-bit*/
#define _PERIIO_U0IIR_FCR_TYPE                     uint8_t

// When Read --------------------------------------------------------------
#define _PERIIO_U0IIR_FCR_FIFO16_MASK              (0xC0u)
#define _PERIIO_U0IIR_FCR_FIFO16_SHIFT             (6)
#define  PERIIO_U0IIR_FCR_FIFO16_DEFAULT           (0x0u)
#define  PERIIO_U0IIR_FCR_FIFO16_OF(x)             _CCL_VALUEOF(_PERIIO_U0IIR_FCR_TYPE,x)
#define  PERIIO_U0IIR_FCR_FIFO16_DISABLE           (0x0u)
#define  PERIIO_U0IIR_FCR_FIFO16_ENABLE            (0x3u)

#define _PERIIO_U0IIR_FCR_IID_MASK                 (0x0Eu)
#define _PERIIO_U0IIR_FCR_IID_SHIFT                (1)
#define  PERIIO_U0IIR_FCR_IID_DEFAULT              (0x0u)
#define  PERIIO_U0IIR_FCR_IID_OF(x)                _CCL_VALUEOF(_PERIIO_U0IIR_FCR_TYPE,x)
#define  PERIIO_U0IIR_FCR_IID_MODEM_STATUS_CHANGED (0x0u)
#define  PERIIO_U0IIR_FCR_IID_THR_EMPTY            (0x1u)
#define  PERIIO_U0IIR_FCR_IID_RCVD_DATA_AVAIL      (0x2u)
#define  PERIIO_U0IIR_FCR_IID_RCVR_STATUS          (0x3u)
#define  PERIIO_U0IIR_FCR_IID_THR_CHAR_TIME_OUT    (0x6u)

#define _PERIIO_U0IIR_FCR_IPND_MASK                (0x01u)
#define _PERIIO_U0IIR_FCR_IPND_SHIFT               (0)
#define  PERIIO_U0IIR_FCR_IPND_DEFAULT             (0x0u)
#define  PERIIO_U0IIR_FCR_IPND_OF(x)               _CCL_VALUEOF(_PERIIO_U0IIR_FCR_TYPE,x)
#define  PERIIO_U0IIR_FCR_IPND_ASSERT              (0x00u)
#define  PERIIO_U0IIR_FCR_IPND_DEASSERT            (0x01u)

// When Write -------------------------------------------------------------
#define _PERIIO_U0IIR_FCR_RTRG_MASK                (0xC0u)
#define _PERIIO_U0IIR_FCR_RTRG_SHIFT               (6)
#define  PERIIO_U0IIR_FCR_RTRG_DEFAULT             (0x3u)
#define  PERIIO_U0IIR_FCR_RTRG_OF(x)               _CCL_VALUEOF(_PERIIO_U0IIR_FCR_TYPE,x)
#define  PERIIO_U0IIR_FCR_RTRG_ONE_CHAR            (0x0u)
#define  PERIIO_U0IIR_FCR_RTRG_QUARTER             (0x1u)
#define  PERIIO_U0IIR_FCR_RTRG_HALF                (0x2u)
#define  PERIIO_U0IIR_FCR_RTRG_FULL                (0x3u)

#define _PERIIO_U0IIR_FCR_DMAM_MASK                (0x08u)
#define _PERIIO_U0IIR_FCR_DMAM_SHIFT               (3)
#define  PERIIO_U0IIR_FCR_DMAM_DEFAULT             (0x00u)
#define  PERIIO_U0IIR_FCR_DMAM_OF(x)               _CCL_VALUEOF(_PERIIO_U0IIR_FCR_TYPE,x)
#define  PERIIO_U0IIR_FCR_DMAM_DISABLE             (0x0u)
#define  PERIIO_U0IIR_FCR_DMAM_ENABLE              (0x1u)

#define _PERIIO_U0IIR_FCR_TFRST_MASK               (0x04u)
#define _PERIIO_U0IIR_FCR_TFRST_SHIFT              (2)
#define  PERIIO_U0IIR_FCR_TFRST_DEFAULT            (0x00u)
#define  PERIIO_U0IIR_FCR_TFRST_OF(x)              _CCL_VALUEOF(_PERIIO_U0IIR_FCR_TYPE,x)
#define  PERIIO_U0IIR_FCR_TFRST_NOP                (0x0u)
#define  PERIIO_U0IIR_FCR_TFRST_RESET              (0x1u)

#define _PERIIO_U0IIR_FCR_RFRST_MASK               (0x02u)
#define _PERIIO_U0IIR_FCR_RFRST_SHIFT              (1)
#define  PERIIO_U0IIR_FCR_RFRST_DEFAULT            (0x00u)
#define  PERIIO_U0IIR_FCR_RFRST_OF(x)              _CCL_VALUEOF(_PERIIO_U0IIR_FCR_TYPE,x)
#define  PERIIO_U0IIR_FCR_RFRST_NOP                (0x0u)
#define  PERIIO_U0IIR_FCR_RFRST_RESET              (0x1u)

#define _PERIIO_U0IIR_FCR_EFIFO_MASK               (0x01u)
#define _PERIIO_U0IIR_FCR_EFIFO_SHIFT              (0)
#define  PERIIO_U0IIR_FCR_EFIFO_DEFAULT            (0x00u)
#define  PERIIO_U0IIR_FCR_EFIFO_OF(x)              _CCL_VALUEOF(_PERIIO_U0IIR_FCR_TYPE,x)
#define  PERIIO_U0IIR_FCR_EFIFO_DISABLE            (0x0u)
#define  PERIIO_U0IIR_FCR_EFIFO_ENABLE             (0x1u)

#define  PERIIO_U0IIR_FCR_OF(x)             _CCL_VALUEOF(_PERIIO_U0IIR_FCR_TYPE,x)


/* default value of U0IIR_FCR register when READ */
#define  PERIIO_U0IIR_FCR_DEFAULT (_PERIIO_U0IIR_FCR_TYPE)(  \
	 _CCL_PER_FDEFAULT(PERIIO,U0IIR_FCR,FIFO16)              \
	|_CCL_PER_FDEFAULT(PERIIO,U0IIR_FCR,IID   )              \
	|_CCL_PER_FDEFAULT(PERIIO,U0IIR_FCR,IPND  )              \
	)

#define PERIIO_U0IIR_FCR_RMK(rtrg, dmam, tfrst, rfrst, efifo) (_PERIIO_U0IIR_FCR_TYPE)( \
	 _CCL_PER_FMK  (PERIIO,U0IIR_FCR,RTRG ,rtrg )                        \
	|_CCL_PER_FMK  (PERIIO,U0IIR_FCR,DMAM ,dmam )                        \
	|_CCL_PER_FMK  (PERIIO,U0IIR_FCR,TFRST,tfrst)                        \
	|_CCL_PER_FMK  (PERIIO,U0IIR_FCR,RFRST,rfrst)                        \
	|_CCL_PER_FMK  (PERIIO,U0IIR_FCR,EFIFO,efifo)                        \
	)

#define PERIIO_U0IIR_FCR_RMKS(rtrg, dmam, tfrst, rfrst, efifo) (_PERIIO_U0IIR_FCR_TYPE)( \
	 _CCL_PER_FMKS (PERIIO,U0IIR_FCR,RTRG ,rtrg )                        \
	|_CCL_PER_FMKS (PERIIO,U0IIR_FCR,DMAM ,dmam )                        \
	|_CCL_PER_FMKS (PERIIO,U0IIR_FCR,TFRST,tfrst)                        \
	|_CCL_PER_FMKS (PERIIO,U0IIR_FCR,RFRST,rfrst)                        \
	|_CCL_PER_FMKS (PERIIO,U0IIR_FCR,EFIFO,efifo)                        \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  U 0 L C R        |
 * |___________________|
 *
 * U0LCR - UART0 Line Control Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  DLAB (Divisor Latch Access Bit)
 *
 * (rw)  BRK (Break control) - This is used to cause a break condition to be transmitted
 *                               to the receiving device.
 *                   BRK   |   description
 *                ---------+-------------------
 *                 DISABLE |
 *                 ENABLE  | the serial output is forced to the spacing (logic 0) state
 *
 * (rw)  EPS (Even Parity Select) -
 *                   EPS   |   description
 *                ---------+-------------------
 *                   ODD   |   odd parity
 *                  EVEN   |   even parity
 *                  MARK   |   mark parity
 *                  SPACE  |   space parity
 *
 * (rw)  PEN (Parity Enable)
 *                   PEN   |   description
 *                ---------+-------------------
 *                 DISABLE |   disable
 *                 ENABLE  |   enable
 *
 * (rw)  STB (Stop Bits) - Number of Stop Bits
 *                   STB   |   description
 *                ---------+-------------------
 *                   1BIT  |   one stop bit
 *                   2BITS |   two stop bits
 *
 * (rw)  WLS (Word Length Select)
 *                   WLS   |   description
 *                ---------+-------------------
 *                  5BITS  |
 *                  6BITS  |
 *                  7BITS  |
 *                  8BITS  |
\*-------------------------------------------------------------------------*/
#define _PERIIO_U0LCR_OFFSET                   (0x010Cu)
#define _PERIIO_U0LCR_ADDRH(h)                 PERIIO_ADDRH(h,U0LCR)
#define _PERIIO_U0LCR_LEN                      (8)                 /* 8-bit*/
#define _PERIIO_U0LCR_TYPE                     uint8_t

#define _PERIIO_U0LCR_DLAB_MASK                (0x80u)
#define _PERIIO_U0LCR_DLAB_SHIFT               (7)
#define  PERIIO_U0LCR_DLAB_DEFAULT             (0x0u)
#define  PERIIO_U0LCR_DLAB_OF(x)               _CCL_VALUEOF(_PERIIO_U0LCR_TYPE,x)

#define _PERIIO_U0LCR_BRK_MASK                 (0x40u)
#define _PERIIO_U0LCR_BRK_SHIFT                (6)
#define  PERIIO_U0LCR_BRK_DEFAULT              (0x0u)
#define  PERIIO_U0LCR_BRK_OF(x)                _CCL_VALUEOF(_PERIIO_U0LCR_TYPE,x)
#define  PERIIO_U0LCR_BRK_DISABLE              (0x00u)
#define  PERIIO_U0LCR_BRK_ENABLE               (0x01u)

#define _PERIIO_U0LCR_EPS_MASK                 (0x30u)
#define _PERIIO_U0LCR_EPS_SHIFT                (4)
#define  PERIIO_U0LCR_EPS_DEFAULT              (0x0u)
#define  PERIIO_U0LCR_EPS_OF(x)                _CCL_VALUEOF(_PERIIO_U0LCR_TYPE,x)
#define  PERIIO_U0LCR_EPS_ODD                  (0x00u)
#define  PERIIO_U0LCR_EPS_EVEN                 (0x01u)
#define  PERIIO_U0LCR_EPS_MARK                 (0x02u)
#define  PERIIO_U0LCR_EPS_SPACE                (0x03u)

#define _PERIIO_U0LCR_PEN_MASK                 (0x08u)
#define _PERIIO_U0LCR_PEN_SHIFT                (3)
#define  PERIIO_U0LCR_PEN_DEFAULT              (0x0u)
#define  PERIIO_U0LCR_PEN_OF(x)                _CCL_VALUEOF(_PERIIO_U0LCR_TYPE,x)
#define  PERIIO_U0LCR_PEN_DISABLE              (0x00u)
#define  PERIIO_U0LCR_PEN_ENABLE               (0x01u)

#define _PERIIO_U0LCR_STB_MASK                 (0x04u)
#define _PERIIO_U0LCR_STB_SHIFT                (2)
#define  PERIIO_U0LCR_STB_DEFAULT              (0x0u)
#define  PERIIO_U0LCR_STB_OF(x)                _CCL_VALUEOF(_PERIIO_U0LCR_TYPE,x)
#define  PERIIO_U0LCR_STB_1BIT                 (0x00u)
#define  PERIIO_U0LCR_STB_2BITS                (0x01u)

#define _PERIIO_U0LCR_WLS_MASK                 (0x03u)
#define _PERIIO_U0LCR_WLS_SHIFT                (0)
#define  PERIIO_U0LCR_WLS_DEFAULT              (0x0u)
#define  PERIIO_U0LCR_WLS_OF(x)                _CCL_VALUEOF(_PERIIO_U0LCR_TYPE,x)
#define  PERIIO_U0LCR_WLS_5BITS                (0x0u)
#define  PERIIO_U0LCR_WLS_6BITS                (0x1u)
#define  PERIIO_U0LCR_WLS_7BITS                (0x2u)
#define  PERIIO_U0LCR_WLS_8BITS                (0x3u)

#define  PERIIO_U0LCR_OF(x)             _CCL_VALUEOF(_PERIIO_U0LCR_TYPE,x)


#define  PERIIO_U0LCR_DEFAULT (_PERIIO_U0LCR_TYPE)(  \
	 _CCL_PER_FDEFAULT(PERIIO,U0LCR,DLAB)            \
	|_CCL_PER_FDEFAULT(PERIIO,U0LCR,BRK )            \
	|_CCL_PER_FDEFAULT(PERIIO,U0LCR,EPS )            \
	|_CCL_PER_FDEFAULT(PERIIO,U0LCR,PEN )            \
	|_CCL_PER_FDEFAULT(PERIIO,U0LCR,STB )            \
	|_CCL_PER_FDEFAULT(PERIIO,U0LCR,WLS )            \
	)


#define PERIIO_U0LCR_RMK(dlab, brk, eps, pen, stb, wls) (_PERIIO_U0LCR_TYPE)( \
	 _CCL_PER_FMK  (PERIIO,U0LCR,DLAB,dlab)                        \
	|_CCL_PER_FMK  (PERIIO,U0LCR,BRK ,brk )                        \
	|_CCL_PER_FMK  (PERIIO,U0LCR,EPS ,eps )                        \
	|_CCL_PER_FMK  (PERIIO,U0LCR,PEN ,pen )                        \
	|_CCL_PER_FMK  (PERIIO,U0LCR,STB ,stb )                        \
	|_CCL_PER_FMK  (PERIIO,U0LCR,WLS ,wls )                        \
	)

#define PERIIO_U0LCR_RMKS(dlab, brk, eps, pen, stb, wls) (_PERIIO_U0LCR_TYPE)( \
	 _CCL_PER_FMK  (PERIIO,U0LCR,DLAB,dlab)                        \
	|_CCL_PER_FMKS (PERIIO,U0LCR,BRK ,brk )                        \
	|_CCL_PER_FMKS (PERIIO,U0LCR,EPS ,eps )                        \
	|_CCL_PER_FMKS (PERIIO,U0LCR,PEN ,pen )                        \
	|_CCL_PER_FMKS (PERIIO,U0LCR,STB ,stb )                        \
	|_CCL_PER_FMKS (PERIIO,U0LCR,WLS ,wls )                        \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  U 0 M C R        |
 * |___________________|
 *
 * U0MCR - UART0 Modem Control Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  LOOP (Loopback) -
 *                  LOOP   |   description
 *                ---------+-------------------
 *                 DISABLE |
 *                 ENABLE  |
 *
 * (rw)  RTS (Request to Send) -
 *                   RTS   |   description
 *                ---------+-------------------
 *               NO_REQUEST| Set RTS# high
 *                 REQUEST | Set RTS# low
 *
 * (rw)  DTR (Data Terminal Ready) -
 *                   DTR   |   description
 *                ---------+-------------------
 *                NOT_READY| Set DTR# high
 *                  READY  | Set DTR# low
\*-------------------------------------------------------------------------*/
#define _PERIIO_U0MCR_OFFSET                   (0x0110u)
#define _PERIIO_U0MCR_ADDRH(h)                 PERIIO_ADDRH(h,U0MCR)
#define _PERIIO_U0MCR_LEN                      (8)                 /* 8-bit*/
#define _PERIIO_U0MCR_TYPE                     uint8_t

#define _PERIIO_U0MCR_LOOP_MASK                (0x10u)
#define _PERIIO_U0MCR_LOOP_SHIFT               (4)
#define  PERIIO_U0MCR_LOOP_DEFAULT             (0x0u)
#define  PERIIO_U0MCR_LOOP_OF(x)               _CCL_VALUEOF(_PERIIO_U0MCR_TYPE,x)
#define  PERIIO_U0MCR_LOOP_DISABLE             (0x00u)
#define  PERIIO_U0MCR_LOOP_ENABLE              (0x01u)

#define _PERIIO_U0MCR_RTS_MASK                 (0x02u)
#define _PERIIO_U0MCR_RTS_SHIFT                (1)
#define  PERIIO_U0MCR_RTS_DEFAULT              (0x0u)
#define  PERIIO_U0MCR_RTS_OF(x)                _CCL_VALUEOF(_PERIIO_U0MCR_TYPE,x)
#define  PERIIO_U0MCR_RTS_NO_REQUEST           (0x00u)
#define  PERIIO_U0MCR_RTS_REQUEST              (0x01u)

#define _PERIIO_U0MCR_DTR_MASK                 (0x01u)
#define _PERIIO_U0MCR_DTR_SHIFT                (0)
#define  PERIIO_U0MCR_DTR_DEFAULT              (0x0u)
#define  PERIIO_U0MCR_DTR_OF(x)                _CCL_VALUEOF(_PERIIO_U0MCR_TYPE,x)
#define  PERIIO_U0MCR_DTR_NOT_READY            (0x00u)
#define  PERIIO_U0MCR_DTR_READY                (0x01u)

#define  PERIIO_U0MCR_OF(x)             _CCL_VALUEOF(_PERIIO_U0MCR_TYPE,x)

#define  PERIIO_U0MCR_DEFAULT (_PERIIO_U0MCR_TYPE)(  \
	 _CCL_PER_FDEFAULT(PERIIO,U0MCR,LOOP)            \
	|_CCL_PER_FDEFAULT(PERIIO,U0MCR,RTS )            \
	|_CCL_PER_FDEFAULT(PERIIO,U0MCR,DTR )            \
	)


#define PERIIO_U0MCR_RMK(loop, rts, dtr) (_PERIIO_U0MCR_TYPE)( \
	 _CCL_PER_FMK  (PERIIO,U0MCR,LOOP,loop)                    \
	|_CCL_PER_FMK  (PERIIO,U0MCR,RTS ,rts )                    \
	|_CCL_PER_FMK  (PERIIO,U0MCR,DTR ,dtr )                    \
	)

#define PERIIO_U0MCR_RMKS(loop, rts, dtr) (_PERIIO_U0MCR_TYPE)( \
	 _CCL_PER_FMKS (PERIIO,U0MCR,LOOP,loop)                     \
	|_CCL_PER_FMKS (PERIIO,U0MCR,RTS ,rts )                     \
	|_CCL_PER_FMKS (PERIIO,U0MCR,DTR ,dtr )                     \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  U 0 L S R        |
 * |___________________|
 *
 * U0LSR - UART0 Line Status Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (r)  RFE (Receiver FIFO Error) - This bit is used to indicate if there is at
 *         least one parity error, framing error, or break indication in the FIFO.
 *         This bit is only relevant when FIFO_MODE != NONE AND FIFOs are enabled
 *         (FCR[0] set to one).
 *                   RFE   |   description
 *                ---------+-------------------
 *                    OK   | no error in RX FIFO
 *                   ERROR | error in RX FIFO
 *
 * (r)  TEMT (Transmitter Empty) -
 *                  TEMT   |   description
 *                ---------+-------------------
 *                NOT_EMPTY|
 *                  EMPTY  |
 *
 * (r)  THRE (Transmit Holding Register Empty) -
 *                  THRE   |   description
 *                ---------+-------------------
 *                NOT_EMPTY|
 *                  EMPTY  |
 *
 * (r)  BI (Break Interrupt) - This bit is used to indicate the detection of
 *          a break sequence on the serial input data
 *                   BI    |   description
 *                ---------+-------------------
 *               NOT_BROKEN|
 *                  BROKEN |
 *
 * (r)  FE (Framing Error) - This bit is used to indicate the occurrence of
 *          a framing error in the receiver. A framing error occurs when the
 *          receiver does not detect a valid STOP bit in the received data.
 *                    FE   |   description
 *                ---------+-------------------
 *                    OK   | no framing error
 *                   ERROR | framing error
 *
 * (r)  PE (Parity Error) - This bit is used to indicate the occurrence of a
 *          parity error in the receiver if the Parity Enable (PEN) bit (LCR[3]) is set.
 *                    PE   |   description
 *                ---------+-------------------
 *                    OK   | no parity error
 *                   ERROR | parity error
 *
 * (r)  OE (Overrun Error) - This bit is used to indicate the occurrence of
 *          an overrun error. This occurs if a new data character was received
 *          before the previous data was read.
 *                    OE   |   description
 *                ---------+-------------------
 *                    OK   | no overrun error
 *                   ERROR | overrun error
 *
 * (r)  DR (Data Ready) - This bit is used to indicate that the receiver
 *          contains at least one character in the RBR or the receiver FIFO.
 *                    DR   |   description
 *                ---------+-----------------------------
 *                NOT_READY| there's no data to be read.
 *                   READY | there's data ready to be read.
\*-------------------------------------------------------------------------*/
#define _PERIIO_U0LSR_OFFSET                   (0x0114u)
#define _PERIIO_U0LSR_ADDRH(h)                 PERIIO_ADDRH(h,U0LSR)
#define _PERIIO_U0LSR_LEN                      (8)                 /* 8-bit*/
#define _PERIIO_U0LSR_TYPE                     uint8_t

#define _PERIIO_U0LSR_RFE_MASK                 (0x80u)
#define _PERIIO_U0LSR_RFE_SHIFT                (7)
#define  PERIIO_U0LSR_RFE_DEFAULT              (0x0u)
#define  PERIIO_U0LSR_RFE_OF(x)                _CCL_VALUEOF(_PERIIO_U0LSR_TYPE,x)
#define  PERIIO_U0LSR_RFE_OK                   (0x00u)
#define  PERIIO_U0LSR_RFE_ERROR                (0x01u)

#define _PERIIO_U0LSR_TEMT_MASK                (0x40u)
#define _PERIIO_U0LSR_TEMT_SHIFT               (6)
#define  PERIIO_U0LSR_TEMT_DEFAULT             (0x1u)
#define  PERIIO_U0LSR_TEMT_OF(x)               _CCL_VALUEOF(_PERIIO_U0LSR_TYPE,x)
#define  PERIIO_U0LSR_TEMT_NOT_EMPTY           (0x00u)
#define  PERIIO_U0LSR_TEMT_EMPTY               (0x01u)

#define _PERIIO_U0LSR_THRE_MASK                (0x20u)
#define _PERIIO_U0LSR_THRE_SHIFT               (5)
#define  PERIIO_U0LSR_THRE_DEFAULT             (0x1u)
#define  PERIIO_U0LSR_THRE_OF(x)               _CCL_VALUEOF(_PERIIO_U0LSR_TYPE,x)
#define  PERIIO_U0LSR_THRE_NOT_EMPTY           (0x00u)
#define  PERIIO_U0LSR_THRE_EMPTY               (0x01u)

#define _PERIIO_U0LSR_BI_MASK                  (0x10u)
#define _PERIIO_U0LSR_BI_SHIFT                 (4)
#define  PERIIO_U0LSR_BI_DEFAULT               (0x0u)
#define  PERIIO_U0LSR_BI_OF(x)                 _CCL_VALUEOF(_PERIIO_U0LSR_TYPE,x)
#define  PERIIO_U0LSR_BI_NOT_BROKEN            (0x00u)
#define  PERIIO_U0LSR_BI_BROKEN                (0x01u)

#define _PERIIO_U0LSR_FE_MASK                  (0x08u)
#define _PERIIO_U0LSR_FE_SHIFT                 (3)
#define  PERIIO_U0LSR_FE_DEFAULT               (0x0u)
#define  PERIIO_U0LSR_FE_OF(x)                 _CCL_VALUEOF(_PERIIO_U0LSR_TYPE,x)
#define  PERIIO_U0LSR_FE_OK                    (0x00u)
#define  PERIIO_U0LSR_FE_ERROR                 (0x01u)

#define _PERIIO_U0LSR_PE_MASK                  (0x04u)
#define _PERIIO_U0LSR_PE_SHIFT                 (2)
#define  PERIIO_U0LSR_PE_DEFAULT               (0x0u)
#define  PERIIO_U0LSR_PE_OF(x)                 _CCL_VALUEOF(_PERIIO_U0LSR_TYPE,x)
#define  PERIIO_U0LSR_PE_OK                    (0x00u)
#define  PERIIO_U0LSR_PE_ERROR                 (0x01u)

#define _PERIIO_U0LSR_OE_MASK                  (0x02u)
#define _PERIIO_U0LSR_OE_SHIFT                 (1)
#define  PERIIO_U0LSR_OE_DEFAULT               (0x0u)
#define  PERIIO_U0LSR_OE_OF(x)                 _CCL_VALUEOF(_OERIIO_U0LSR_TYOE,x)
#define  PERIIO_U0LSR_OE_OK                    (0x00u)
#define  PERIIO_U0LSR_OE_ERROR                 (0x01u)

#define _PERIIO_U0LSR_DR_MASK                  (0x01u)
#define _PERIIO_U0LSR_DR_SHIFT                 (0)
#define  PERIIO_U0LSR_DR_DEFAULT               (0x0u)
#define  PERIIO_U0LSR_DR_OF(x)                 _CCL_VALUEOF(_DRRIIO_U0LSR_TYDR,x)
#define  PERIIO_U0LSR_DR_NOT_READY             (0x00u)
#define  PERIIO_U0LSR_DR_READY                 (0x01u)


#define  PERIIO_U0LSR_OF(x)             _CCL_VALUEOF(_PERIIO_U0LSR_TYPE,x)

#define  PERIIO_U0LSR_DEFAULT (_PERIIO_U0LSR_TYPE)(  \
	 _CCL_PER_FDEFAULT(PERIIO,U0LSR,RFE )            \
	|_CCL_PER_FDEFAULT(PERIIO,U0LSR,TEMT)            \
	|_CCL_PER_FDEFAULT(PERIIO,U0LSR,THRE)            \
	|_CCL_PER_FDEFAULT(PERIIO,U0LSR, BI )            \
	|_CCL_PER_FDEFAULT(PERIIO,U0LSR, FE )            \
	|_CCL_PER_FDEFAULT(PERIIO,U0LSR, PE )            \
	|_CCL_PER_FDEFAULT(PERIIO,U0LSR, OE )            \
	|_CCL_PER_FDEFAULT(PERIIO,U0LSR, DR )            \
	)


#endif	/* !__C_CHIP_LIB_REALTEK_PERIPHERAL_INPUT_OUTPUT_HARDWARE_ABSTRACT_LAYER_HEADER_INCLUDED__ */


/*-------------------------------------------------------------------------*\
 * End of PERIIO_HAL.h                                                        *
\*-------------------------------------------------------------------------*/

