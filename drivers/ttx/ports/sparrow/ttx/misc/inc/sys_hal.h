/*-------------------------------------------------------------------------*\
 *               File: sys_hal.h
 *                Ver: 0.1
 *             Author: Chi-Chun Cheng
 *          Objective: Provides the Realtek RTD-2668 Sparrow System
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
 *   SYS_ADDR   (<REG>)                  -   Register address
 *   SYS_RGET   (<REG>)                  -   Returns the value in the peripheral register
 *   SYS_RSET   (<REG>,x)                -   Register set
 *   SYS_FGET   (<REG>,<FIELD>)          -   Returns the value of the specified field
 *                                             in the peripheral register
 *   SYS_FSET   (<REG>,<FIELD>,fieldval) -   Writes fieldval to the specified field
 *                                             in the peripheral register
 *   SYS_FSETS  (<REG>,<FIELD>,<SYM>)    -   Writes the symbol value to the specified
 *                                             field in the peripheral
 *   SYS_RGETA  (addr,<REG>)             -   Gets register for a given address
 *   SYS_RSETA  (addr,<REG>,x)           -   Sets register for a given address
 *
 *  <NOTE>
 *  Not completed yet!!!!!!!!!!
\*-------------------------------------------------------------------------*/

#ifndef	__C_CHIP_LIB_REALTEK_RTD_2668_SYSTEM_HARDWARE_ABSTRACT_LAYER_HEADER_INCLUDED__
#define	__C_CHIP_LIB_REALTEK_RTD_2668_SYSTEM_HARDWARE_ABSTRACT_LAYER_HEADER_INCLUDED__


/* CCL headers ------------------------------------------------------------*/
#include <ccl/std.h>    /* the standard header of C Chip Library */

/*
 *  Base address of SYS
 */
#define _SYS_ADDR_BASE  (0x2C00u)

/*
 *  The function used for read register from this IP. Prototype is
 *     uint32_t _SYS_REG_READ( addr );
 */
#define _SYS_REG_READ(addr)   TTX_Reg8Read(addr)

/*
 *  The function used for read register from this IP. Prototype is
 *     void    _SYS_REG_WRITE( addr, val );
 */
#define _SYS_REG_WRITE(addr,val)  TTX_Reg8Write(addr,val)

/*
 * The shift left of data-bus connection in bits, e.g.,
 *   conventionally, the D0-D7 of SYS are connected to CPU data-bus D0-D7 (_SYS_DB_SHFT = 0).
 *   However, in some cases,
 *     the D0-D7 of 82C52 are connected to CPU data-bus D16-D24 (_SYS_DB_SHFT = 16).
 */
#define _SYS_DB_SHFT     (0)

/*
 * The width of data-bus connection in bits, e.g.,
 *   conventionally, the D0-D7 of 82C52 are connected to CPU data-bus D0-D7 (_SYS_DB_WIDTH = 0x00FFu).
 *   Please note, despite the shift of data-bus, _SYS_DB_WITH starts from LSB.
 *   As a consequence,
 *     if the D0-D7 of 82C52 are connected to CPU data-bus D16-D24 (_SYS_DB_WIDTH = 0x00FFu).
 */
#define _SYS_DB_WIDTH   (0x000000FFu)

/*
 * The shift left of address-bus connection in bits, e.g.,
 *   conventionally, the A0-A7 of 82C52 are connected to CPU address-bus A0-A7 (_SYS_AB_SHFT = Uint8).
 *   However, in some cases,
 *     the A0-A7 of 82C52 are connected to CPU data-bus A2-A9 (_SYS_DB_SHFT = uint32_t).
 */
#define _SYS_AB_SHFT     uint16_t




/* Field related macros */

#define SYS_FMK(REG,FIELD,x)\
	_CCL_PER_FMK(SYS,REG,FIELD,x)

#define SYS_FMKS(REG,FIELD,SYM)\
	_CCL_PER_FMKS(SYS,REG,FIELD,SYM)

#define SYS_FEXTRACT(REG,FIELD,x)\
	_CCL_PER_FEXTRACT(SYS,REG,FIELD,x)


/* Raw register/field access macros */

#define SYS_ADDR(REG)\
    (_SYS_ADDR_BASE+_SYS_##REG##_OFFSET)

#define SYS_RGET(REG)\
    _SYS_RGETA(SYS_ADDR(REG),REG)

#define SYS_RSET(REG,x)\
    _SYS_RSETA(SYS_ADDR(REG),REG,x)

#define SYS_FGET(REG,FIELD)\
    _SYS_FGETA(SYS_ADDR(REG),REG,FIELD)

#define SYS_FSET(REG,FIELD,x)\
    _SYS_FSETA(SYS_ADDR(REG),REG,FIELD,x)

#define SYS_FSETS(REG,FIELD,SYM)\
    _SYS_FSETSA(SYS_ADDR(REG),REG,FIELD,SYM)


/* Address based register/field access macros */

#define _SYS_RGETA(addr,REG)\
	_CCL_PER_RGET(addr,SYS,REG,_SYS_DB_SHFT,_SYS_DB_WIDTH)

#define _SYS_RSETA(addr,REG,x)\
	_CCL_PER_RSET(addr,SYS,REG,x,_SYS_DB_SHFT,_SYS_DB_WIDTH)

#define _SYS_RSETSA(addr,REG,SYM)\
	_CCL_PER_RSETS(addr,SYS,REG,SYM,_SYS_DB_SHFT,_SYS_DB_WIDTH)

#define _SYS_FGETA(addr,REG,FIELD)\
	_CCL_PER_FGET(addr,SYS,REG,FIELD,_SYS_DB_SHFT,_SYS_DB_WIDTH)

#define _SYS_FSETA(addr,REG,FIELD,x)\
	_CCL_PER_FSET(addr,SYS,REG,FIELD,x,_SYS_DB_SHFT,_SYS_DB_WIDTH)

#define _SYS_FSETSA(addr,REG,FIELD,SYM)\
	_CCL_PER_FSETS(addr,SYS,REG,FIELD,SYM,_SYS_DB_SHFT,_SYS_DB_WIDTH)


/* Handle based register/field access macros */

/* This is for base-address determined by handle h.
#define SYS_ADDRH(h,REG)\
	(_SYS_AB_SHFT)(&((h)->addrBase[_SYS_##REG##_OFFSET]))
*/
#define SYS_ADDRH(h,REG)\
	(_SYS_AB_SHFT)(_SYS_ADDR_BASE+_SYS_##REG##_OFFSET)

#define SYS_RGETH(h,REG)\
	_SYS_RGETA(SYS_ADDRH(h,REG),REG)

#define SYS_RSETH(h,REG,x)\
	_SYS_RSETA(SYS_ADDRH(h,REG),REG,x)

#define SYS_RSETSH(h,REG,SYM)\
	_SYS_RSETSA(SYS_ADDRH(h,REG),REG,SYM)

#define SYS_FGETH(h,REG,FIELD)\
	_SYS_FGETA(SYS_ADDRH(h,REG),REG,FIELD)

#define SYS_FSETH(h,REG,FIELD,x)\
	_SYS_FSETA(SYS_ADDRH(h,REG),REG,FIELD,x)

#define SYS_FSETSH(h,REG,FIELD,SYM)\
	_SYS_FSETSA(SYS_ADDRH(h,REG),REG,FIELD,SYM)



/* Function Prototypes ----------------------------------------------------*/

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  IMEM4K ADDR      |
 * |___________________|
 *
 * IMEM4K_ADDR - Address of 4K bytes I-Memory
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  ADDR  - Start address [15:12] of IMEM, start address should be 4K boundary.
\*-------------------------------------------------------------------------*/
#define _SYS_IMEM4K_ADDR_OFFSET                   (0x0000u)
#define _SYS_IMEM4K_ADDR_ADDRH(h)                 SYS_ADDRH(h,IMEM4K_ADDR)
#define _SYS_IMEM4K_ADDR_LEN                      (8)                 /* 8-bit*/
#define _SYS_IMEM4K_ADDR_TYPE                     uint8_t

#define _SYS_IMEM4K_ADDR_ADDR_MASK                (0x0Fu)
#define _SYS_IMEM4K_ADDR_ADDR_SHIFT               (0)
#define  SYS_IMEM4K_ADDR_ADDR_DEFAULT             (0x00u)
#define  SYS_IMEM4K_ADDR_ADDR_OF(x)               _CCL_VALUEOF(_SYS_IMEM4K_ADDR_TYPE,x)

#define  SYS_IMEM4K_ADDR_OF(x)             _CCL_VALUEOF(_SYS_IMEM4K_ADDR_TYPE,x)

#define  SYS_IMEM4K_ADDR_DEFAULT (_SYS_IMEM4K_ADDR_TYPE)(  \
	 _CCL_PER_FDEFAULT(SYS,IMEM4K_ADDR,ADDR)          \
	)

#define SYS_IMEM4K_ADDR_RMK(addr) (_SYS_IMEM4K_ADDR_TYPE)( \
	 _CCL_PER_FMK  (SYS,IMEM4K_ADDR,ADDR,addr)        \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  IMEM4K BANK      |
 * |___________________|
 *
 * IMEM4K_BANK - Bank of 4K bytes I-Memory
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  BANK  - 5bit bank info.
\*-------------------------------------------------------------------------*/
#define _SYS_IMEM4K_BANK_OFFSET                   (0x0001u)
#define _SYS_IMEM4K_BANK_ADDRH(h)                 SYS_ADDRH(h,IMEM4K_BANK)
#define _SYS_IMEM4K_BANK_LEN                      (8)                 /* 8-bit*/
#define _SYS_IMEM4K_BANK_TYPE                     uint8_t

#define _SYS_IMEM4K_BANK_BANK_MASK                (0xFFu)
#define _SYS_IMEM4K_BANK_BANK_SHIFT               (0)
#define  SYS_IMEM4K_BANK_BANK_DEFAULT             (0x00u)
#define  SYS_IMEM4K_BANK_BANK_OF(x)               _CCL_VALUEOF(_SYS_IMEM4K_BANK_TYPE,x)

#define  SYS_IMEM4K_BANK_OF(x)             _CCL_VALUEOF(_SYS_IMEM4K_BANK_TYPE,x)

#define  SYS_IMEM4K_BANK_DEFAULT (_SYS_IMEM4K_BANK_TYPE)(  \
	 _CCL_PER_FDEFAULT(SYS,IMEM4K_BANK,BANK)               \
	)

#define SYS_IMEM4K_BANK_RMK(bank) (_SYS_IMEM4K_BANK_TYPE)( \
	 _CCL_PER_FMK  (SYS,IMEM4K_BANK,BANK,bank)             \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  IMEM4K REMAP     |
 * |___________________|
 *
 * IMEM4K_REMAP - Bank of 4K bytes I-Memory
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (r)   DONE   - remap done
 *                  DONE   |   description
 *                ---------+-------------------
 *                 ASSERTED| remap is done.
 *               DEASSERTED| remap is not done.
 *
 * (rw)  REMAP  - 5bit bank info.
 *                  REMAP  |   description
 *                ---------+-------------------
 *                  STOP   | stop mapping (Don't clear remap in IMEM region)
 *                  START  | map program from serial flash to IMEM.
\*-------------------------------------------------------------------------*/
#define _SYS_IMEM4K_REMAP_OFFSET                   (0x0002u)
#define _SYS_IMEM4K_REMAP_ADDRH(h)                 SYS_ADDRH(h,IMEM4K_REMAP)
#define _SYS_IMEM4K_REMAP_LEN                      (8)                 /* 8-bit*/
#define _SYS_IMEM4K_REMAP_TYPE                     uint8_t

#define _SYS_IMEM4K_REMAP_DONE_MASK                (0x02u)
#define _SYS_IMEM4K_REMAP_DONE_SHIFT               (1)
#define  SYS_IMEM4K_REMAP_DONE_DEFAULT             (0x00u)
#define  SYS_IMEM4K_REMAP_DONE_OF(x)               _CCL_VALUEOF(_SYS_IMEM4K_REMAP_TYPE,x)
#define  SYS_IMEM4K_REMAP_DONE_DEASSERTED          (0x00u)
#define  SYS_IMEM4K_REMAP_DONE_ASSERTED            (0x01u)

#define _SYS_IMEM4K_REMAP_REMAP_MASK               (0x01u)
#define _SYS_IMEM4K_REMAP_REMAP_SHIFT              (0)
#define  SYS_IMEM4K_REMAP_REMAP_DEFAULT            (0x00u)
#define  SYS_IMEM4K_REMAP_REMAP_OF(x)               _CCL_VALUEOF(_SYS_IMEM4K_REMAP_TYPE,x)
#define  SYS_IMEM4K_REMAP_REMAP_STOP               (0x00u)
#define  SYS_IMEM4K_REMAP_REMAP_START              (0x01u)

#define  SYS_IMEM4K_REMAP_OF(x)             _CCL_VALUEOF(_SYS_IMEM4K_REMAP_TYPE,x)

#define  SYS_IMEM4K_REMAP_DEFAULT (_SYS_IMEM4K_REMAP_TYPE)(  \
	 _CCL_PER_FDEFAULT(SYS,IMEM4K_REMAP,REMAP)               \
	)

#define SYS_IMEM4K_REMAP_RMK(remap) (_SYS_IMEM4K_REMAP_TYPE)( \
	 _CCL_PER_FMK  (SYS,IMEM4K_REMAP,REMAP,remap)             \
	)

#define SYS_IMEM4K_REMAP_RMKS(remap) (_SYS_IMEM4K_REMAP_TYPE)( \
	 _CCL_PER_FMKS (SYS,IMEM4K_REMAP,REMAP,remap)             \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  IMEM8K ADDR      |
 * |___________________|
 *
 * IMEM8K_ADDR - Address of 8K bytes I-Memory
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  ADDR  - Start address [15:12] of IMEM, start address should be 8K boundary.
\*-------------------------------------------------------------------------*/
#define _SYS_IMEM8K_ADDR_OFFSET                   (0x0003u)
#define _SYS_IMEM8K_ADDR_ADDRH(h)                 SYS_ADDRH(h,IMEM8K_ADDR)
#define _SYS_IMEM8K_ADDR_LEN                      (8)                 /* 8-bit*/
#define _SYS_IMEM8K_ADDR_TYPE                     uint8_t

#define _SYS_IMEM8K_ADDR_ADDR_MASK                (0x0Fu)
#define _SYS_IMEM8K_ADDR_ADDR_SHIFT               (0)
#define  SYS_IMEM8K_ADDR_ADDR_DEFAULT             (0x00u)
#define  SYS_IMEM8K_ADDR_ADDR_OF(x)               _CCL_VALUEOF(_SYS_IMEM8K_ADDR_TYPE,x)

#define  SYS_IMEM8K_ADDR_OF(x)             _CCL_VALUEOF(_SYS_IMEM8K_ADDR_TYPE,x)

#define  SYS_IMEM8K_ADDR_DEFAULT (_SYS_IMEM8K_ADDR_TYPE)(  \
	 _CCL_PER_FDEFAULT(SYS,IMEM8K_ADDR,ADDR)          \
	)

#define SYS_IMEM8K_ADDR_RMK(addr) (_SYS_IMEM8K_ADDR_TYPE)( \
	 _CCL_PER_FMK  (SYS,IMEM8K_ADDR,ADDR,addr)        \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  IMEM8K BANK      |
 * |___________________|
 *
 * IMEM8K_BANK - Bank of 8K bytes I-Memory
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  BANK  - 5bit bank info.
\*-------------------------------------------------------------------------*/
#define _SYS_IMEM8K_BANK_OFFSET                   (0x0004u)
#define _SYS_IMEM8K_BANK_ADDRH(h)                 SYS_ADDRH(h,IMEM8K_BANK)
#define _SYS_IMEM8K_BANK_LEN                      (8)                 /* 8-bit*/
#define _SYS_IMEM8K_BANK_TYPE                     uint8_t

#define _SYS_IMEM8K_BANK_BANK_MASK                (0xFFu)
#define _SYS_IMEM8K_BANK_BANK_SHIFT               (0)
#define  SYS_IMEM8K_BANK_BANK_DEFAULT             (0x00u)
#define  SYS_IMEM8K_BANK_BANK_OF(x)               _CCL_VALUEOF(_SYS_IMEM8K_BANK_TYPE,x)

#define  SYS_IMEM8K_BANK_OF(x)             _CCL_VALUEOF(_SYS_IMEM8K_BANK_TYPE,x)

#define  SYS_IMEM8K_BANK_DEFAULT (_SYS_IMEM8K_BANK_TYPE)(  \
	 _CCL_PER_FDEFAULT(SYS,IMEM8K_BANK,BANK)               \
	)

#define SYS_IMEM8K_BANK_RMK(bank) (_SYS_IMEM8K_BANK_TYPE)( \
	 _CCL_PER_FMK  (SYS,IMEM8K_BANK,BANK,bank)             \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  IMEM8K REMAP     |
 * |___________________|
 *
 * IMEM8K_REMAP - Bank of 8K bytes I-Memory
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (r)   DONE   - remap done
 *                  DONE   |   description
 *                ---------+-------------------
 *                 ASSERTED| remap is done.
 *               DEASSERTED| remap is not done.
 *
 * (rw)  REMAP  - 5bit bank info.
 *                  REMAP  |   description
 *                ---------+-------------------
 *                  STOP   | stop mapping (Don't clear remap in IMEM region)
 *                  START  | map program from serial flash to IMEM.
\*-------------------------------------------------------------------------*/
#define _SYS_IMEM8K_REMAP_OFFSET                   (0x0005u)
#define _SYS_IMEM8K_REMAP_ADDRH(h)                 SYS_ADDRH(h,IMEM8K_REMAP)
#define _SYS_IMEM8K_REMAP_LEN                      (8)                 /* 8-bit*/
#define _SYS_IMEM8K_REMAP_TYPE                     uint8_t

#define _SYS_IMEM8K_REMAP_DONE_MASK                (0x02u)
#define _SYS_IMEM8K_REMAP_DONE_SHIFT               (1)
#define  SYS_IMEM8K_REMAP_DONE_DEFAULT             (0x00u)
#define  SYS_IMEM8K_REMAP_DONE_OF(x)               _CCL_VALUEOF(_SYS_IMEM8K_REMAP_TYPE,x)
#define  SYS_IMEM8K_REMAP_DONE_DEASSERTED          (0x00u)
#define  SYS_IMEM8K_REMAP_DONE_ASSERTED            (0x01u)

#define _SYS_IMEM8K_REMAP_REMAP_MASK               (0x01u)
#define _SYS_IMEM8K_REMAP_REMAP_SHIFT              (0)
#define  SYS_IMEM8K_REMAP_REMAP_DEFAULT            (0x00u)
#define  SYS_IMEM8K_REMAP_REMAP_OF(x)              _CCL_VALUEOF(_SYS_IMEM8K_REMAP_TYPE,x)
#define  SYS_IMEM8K_REMAP_REMAP_STOP               (0x00u)
#define  SYS_IMEM8K_REMAP_REMAP_START              (0x01u)

#define  SYS_IMEM8K_REMAP_OF(x)             _CCL_VALUEOF(_SYS_IMEM8K_REMAP_TYPE,x)

#define  SYS_IMEM8K_REMAP_DEFAULT (_SYS_IMEM8K_REMAP_TYPE)(  \
	 _CCL_PER_FDEFAULT(SYS,IMEM8K_REMAP,REMAP)               \
	)

#define SYS_IMEM8K_REMAP_RMK(remap) (_SYS_IMEM8K_REMAP_TYPE)( \
	 _CCL_PER_FMK  (SYS,IMEM8K_REMAP,REMAP,remap)             \
	)

#define SYS_IMEM8K_REMAP_RMKS(remap) (_SYS_IMEM8K_REMAP_TYPE)( \
	 _CCL_PER_FMKS (SYS,IMEM8K_REMAP,REMAP,remap)             \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  R E G E N D      |
 * |___________________|
 *
 * REGEND - Register Endianess
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  SWAP  - we separate a 32-bit register as 4 "8-bit" register, 03, 02, 01 and 00.
 *                 Swap can change the arrangement like below.
 *                   SWAP  |   description
 *                ---------+-------------------
 *                   BIG   | reg_32 = {03,02,01,00}
 *                  LITTLE | reg_32 = {00,01,02,03}
\*-------------------------------------------------------------------------*/
#define _SYS_REGEND_OFFSET                   (0x0007u)
#define _SYS_REGEND_ADDRH(h)                 SYS_ADDRH(h,REGEND)
#define _SYS_REGEND_LEN                      (8)                 /* 8-bit*/
#define _SYS_REGEND_TYPE                     uint8_t

#define _SYS_REGEND_SWAP_MASK               (0x01u)
#define _SYS_REGEND_SWAP_SHIFT              (0)
#define  SYS_REGEND_SWAP_DEFAULT            (0x00u)
#define  SYS_REGEND_SWAP_OF(x)              _CCL_VALUEOF(_SYS_REGEND_TYPE,x)
#define  SYS_REGEND_SWAP_BIG                (0x00u)
#define  SYS_REGEND_SWAP_LITTLE             (0x01u)

#define  SYS_REGEND_OF(x)             _CCL_VALUEOF(_SYS_REGEND_TYPE,x)

#define  SYS_REGEND_DEFAULT (_SYS_REGEND_TYPE)(  \
	 _CCL_PER_FDEFAULT(SYS,REGEND,SWAP)          \
	)

#define SYS_REGEND_RMK(swap) (_SYS_REGEND_TYPE)( \
	 _CCL_PER_FMK  (SYS,REGEND,SWAP,swap)        \
	)

#define SYS_REGEND_RMKS(swap) (_SYS_REGEND_TYPE)( \
	 _CCL_PER_FMKS (SYS,REGEND,SWAP,swap)         \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  G M A P          |
 * |___________________|
 *
 * GMAP - Global Mapping Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  IMEM4K  - 5bit bank info.
 *                  IMEM4K  |   description
 *                ----------+-------------------
 *                  STOP    | stop mapping (Don't clear remap in IMEM region)
 *                  START   | map program of IMEM4k_ADDR in all bank to 4k IMEM.
 *                          |   Source of 4k IMEM will be choosed by register IMEM4K_BANK.
 *
 * (rw)  IMEM8K  - 5bit bank info.
 *                  IMEM8K  |   description
 *                ----------+-------------------
 *                  STOP    | stop mapping (Don't clear remap in IMEM region)
 *                  START   | map program of IMEM8K_ADDR in all bank 8k IMEM.
 *                          |   Source of 8k IMEM will be choosed by register IMEM8K_BANK.
\*-------------------------------------------------------------------------*/
#define _SYS_GMAP_OFFSET                   (0x0008u)
#define _SYS_GMAP_ADDRH(h)                 SYS_ADDRH(h,GMAP)
#define _SYS_GMAP_LEN                      (8)                 /* 8-bit*/
#define _SYS_GMAP_TYPE                     uint8_t

#define _SYS_GMAP_IMEM4K_MASK              (0x02u)
#define _SYS_GMAP_IMEM4K_SHIFT             (1)
#define  SYS_GMAP_IMEM4K_DEFAULT           (0x00u)
#define  SYS_GMAP_IMEM4K_OF(x)             _CCL_VALUEOF(_SYS_GMAP_TYPE,x)
#define  SYS_GMAP_IMEM4K_STOP              (0x00u)
#define  SYS_GMAP_IMEM4K_START             (0x01u)

#define _SYS_GMAP_IMEM8K_MASK              (0x01u)
#define _SYS_GMAP_IMEM8K_SHIFT             (0)
#define  SYS_GMAP_IMEM8K_DEFAULT           (0x00u)
#define  SYS_GMAP_IMEM8K_OF(x)             _CCL_VALUEOF(_SYS_GMAP_TYPE,x)
#define  SYS_GMAP_IMEM8K_STOP              (0x00u)
#define  SYS_GMAP_IMEM8K_START             (0x01u)

#define  SYS_GMAP_OF(x)             _CCL_VALUEOF(_SYS_GMAP_TYPE,x)

#define  SYS_GMAP_DEFAULT (_SYS_GMAP_TYPE)(  \
	 _CCL_PER_FDEFAULT(SYS,GMAP,IMEM4K)      \
	|_CCL_PER_FDEFAULT(SYS,GMAP,IMEM8K)      \
	)

#define SYS_GMAP_RMK(imem4k, imem8k) (_SYS_GMAP_TYPE)( \
	 _CCL_PER_FMK  (SYS,GMAP,IMEM4K,imem4k)            \
	|_CCL_PER_FMK  (SYS,GMAP,IMEM8K,imem8k)            \
	)

#define SYS_GMAP_RMKS(imem4k, imem8k) (_SYS_GMAP_TYPE)( \
	 _CCL_PER_FMKS (SYS,GMAP,IMEM4K,imem4k)             \
	|_CCL_PER_FMKS (SYS,GMAP,IMEM8K,imem8k)             \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  SDMA_FLASHLSB    |
 * |___________________|
 *
 * SDMA_FLASHLSB - SPI DMA flash addres least significant byte Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  ADDR  - least significatn byte of FLASH address.
\*-------------------------------------------------------------------------*/
#define _SYS_SDMA_FLASHLSB_OFFSET                   (0x0009u)
#define _SYS_SDMA_FLASHLSB_ADDRH(h)                 SYS_ADDRH(h,SDMA_FLASHLSB)
#define _SYS_SDMA_FLASHLSB_LEN                      (8)                 /* 8-bit*/
#define _SYS_SDMA_FLASHLSB_TYPE                     uint8_t

#define _SYS_SDMA_FLASHLSB_ADDR_MASK                (0xFFu)
#define _SYS_SDMA_FLASHLSB_ADDR_SHIFT               (0)
#define  SYS_SDMA_FLASHLSB_ADDR_DEFAULT             (0x00u)
#define  SYS_SDMA_FLASHLSB_ADDR_OF(x)               _CCL_VALUEOF(_SYS_SDMA_FLASHLSB_TYPE,x)


#define  SYS_SDMA_FLASHLSB_OF(x)             _CCL_VALUEOF(_SYS_SDMA_FLASHLSB_TYPE,x)

#define  SYS_SDMA_FLASHLSB_DEFAULT (_SYS_SDMA_FLASHLSB_TYPE)(  \
	 _CCL_PER_FDEFAULT(SYS,SDMA_FLASHLSB,ADDR)                \
	)

#define SYS_SDMA_FLASHLSB_RMK(addr) (_SYS_SDMA_FLASHLSB_TYPE)( \
	 _CCL_PER_FMK  (SYS,SDMA_FLASHLSB,ADDR,addr)               \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  SDMA_FLASHMSB    |
 * |___________________|
 *
 * SDMA_FLASHMSB - SPI DMA flash addres most significant byte Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  ADDR  - least significatn byte of FLASH address.
\*-------------------------------------------------------------------------*/
#define _SYS_SDMA_FLASHMSB_OFFSET                   (0x000Au)
#define _SYS_SDMA_FLASHMSB_ADDRH(h)                 SYS_ADDRH(h,SDMA_FLASHMSB)
#define _SYS_SDMA_FLASHMSB_LEN                      (8)                 /* 8-bit*/
#define _SYS_SDMA_FLASHMSB_TYPE                     uint8_t

#define _SYS_SDMA_FLASHMSB_ADDR_MASK                (0xFFu)
#define _SYS_SDMA_FLASHMSB_ADDR_SHIFT               (0)
#define  SYS_SDMA_FLASHMSB_ADDR_DEFAULT             (0x00u)
#define  SYS_SDMA_FLASHMSB_ADDR_OF(x)               _CCL_VALUEOF(_SYS_SDMA_FLASHMSB_TYPE,x)


#define  SYS_SDMA_FLASHMSB_OF(x)             _CCL_VALUEOF(_SYS_SDMA_FLASHMSB_TYPE,x)

#define  SYS_SDMA_FLASHMSB_DEFAULT (_SYS_SDMA_FLASHMSB_TYPE)(  \
	 _CCL_PER_FDEFAULT(SYS,SDMA_FLASHMSB,ADDR)                \
	)

#define SYS_SDMA_FLASHMSB_RMK(addr) (_SYS_SDMA_FLASHMSB_TYPE)( \
	 _CCL_PER_FMK  (SYS,SDMA_FLASHMSB,ADDR,addr)               \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  SDMA_BANK        |
 * |___________________|
 *
 * SDMA_BANK - SPI DMA flash bank Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  BANK  - bank of FLASH (0~31).
\*-------------------------------------------------------------------------*/
#define _SYS_SDMA_BANK_OFFSET                   (0x000Bu)
#define _SYS_SDMA_BANK_ADDRH(h)                 SYS_ADDRH(h,SDMA_BANK)
#define _SYS_SDMA_BANK_LEN                      (8)                 /* 8-bit*/
#define _SYS_SDMA_BANK_TYPE                     uint8_t

#define _SYS_SDMA_BANK_BANK_MASK                (0x1Fu)
#define _SYS_SDMA_BANK_BANK_SHIFT               (0)
#define  SYS_SDMA_BANK_BANK_DEFAULT             (0x00u)
#define  SYS_SDMA_BANK_BANK_OF(x)               _CCL_VALUEOF(_SYS_SDMA_BANK_TYPE,x)

#define  SYS_SDMA_BANK_OF(x)             _CCL_VALUEOF(_SYS_SDMA_BANK_TYPE,x)

#define  SYS_SDMA_BANK_DEFAULT (_SYS_SDMA_BANK_TYPE)(  \
	 _CCL_PER_FDEFAULT(SYS,SDMA_BANK,BANK)             \
	)

#define SYS_SDMA_BANK_RMK(bank) (_SYS_SDMA_BANK_TYPE)( \
	 _CCL_PER_FMK  (SYS,SDMA_BANK,BANK,bank)           \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  SDMA_CTRL        |
 * |___________________|
 *
 * SDMA_CTRL - SPI DMA flash bank Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  MODE -
 *                   MODE   |   description
 *                ----------+-------------------
 *                     0    | write 8bit (6093) registers (address won't increase)
 *                     1    | write dmem (address increases)
 *                     2    | write 32 bit register / system register (address won't increase)
 *
 * (rw)  GO  - bank of FLASH (0~31).
 *                     GO   |   description
 *                ----------+-------------------
 *                   STOP   | Stop DMA
 *                   START  | Start DMA
\*-------------------------------------------------------------------------*/
#define _SYS_SDMA_CTRL_OFFSET                   (0x000Cu)
#define _SYS_SDMA_CTRL_ADDRH(h)                 SYS_ADDRH(h,SDMA_CTRL)
#define _SYS_SDMA_CTRL_LEN                      (8)                 /* 8-bit*/
#define _SYS_SDMA_CTRL_TYPE                     uint8_t

#define _SYS_SDMA_CTRL_MODE_MASK                (0x0Cu)
#define _SYS_SDMA_CTRL_MODE_SHIFT               (2)
#define  SYS_SDMA_CTRL_MODE_DEFAULT             (0x00u)
#define  SYS_SDMA_CTRL_MODE_OF(x)               _CCL_VALUEOF(_SYS_SDMA_CTRL_TYPE,x)
#define  SYS_SDMA_CTRL_MODE_0                   (0x00u)
#define  SYS_SDMA_CTRL_MODE_1                   (0x01u)
#define  SYS_SDMA_CTRL_MODE_2                   (0x02u)

#define _SYS_SDMA_CTRL_GO_MASK                  (0x01u)
#define _SYS_SDMA_CTRL_GO_SHIFT                 (0)
#define  SYS_SDMA_CTRL_GO_DEFAULT               (0x00u)
#define  SYS_SDMA_CTRL_GO_OF(x)                 _CCL_VALUEOF(_SYS_SDMA_CTRL_TYPE,x)
#define  SYS_SDMA_CTRL_GO_STOP                  (0x00u)
#define  SYS_SDMA_CTRL_GO_START                 (0x01u)

#define  SYS_SDMA_CTRL_OF(x)             _CCL_VALUEOF(_SYS_SDMA_CTRL_TYPE,x)

#define  SYS_SDMA_CTRL_DEFAULT (_SYS_SDMA_CTRL_TYPE)(  \
	 _CCL_PER_FDEFAULT(SYS,SDMA_CTRL,MODE)             \
	|_CCL_PER_FDEFAULT(SYS,SDMA_CTRL,GO)               \
	)

#define SYS_SDMA_CTRL_RMK(mode,go) (_SYS_SDMA_CTRL_TYPE)( \
	 _CCL_PER_FMK  (SYS,SDMA_CTRL,MODE,mode)              \
	|_CCL_PER_FMK  (SYS,SDMA_CTRL,GO  ,go  )              \
	)

#define SYS_SDMA_CTRL_RMKS(mode,go) (_SYS_SDMA_CTRL_TYPE)( \
	 _CCL_PER_FMKS  (SYS,SDMA_CTRL,MODE,mode)              \
	|_CCL_PER_FMKS  (SYS,SDMA_CTRL,GO  ,go  )              \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  SDMA_LENLSB      |
 * |___________________|
 *
 * SDMA_LENLSB - length least significant byte Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  SIZE  - for SYS_SDMA_CTRL_MODE_0, unit is 8-bit
 *               for SYS_SDMA_CTRL_MODE_1, unit is 8-bit
 *               for SYS_SDMA_CTRL_MODE_2, unit is 32-bit
\*-------------------------------------------------------------------------*/
#define _SYS_SDMA_LENLSB_OFFSET                   (0x000Du)
#define _SYS_SDMA_LENLSB_SIZEH(h)                 SYS_SIZEH(h,SDMA_LENLSB)
#define _SYS_SDMA_LENLSB_LEN                      (8)                 /* 8-bit*/
#define _SYS_SDMA_LENLSB_TYPE                     uint8_t

#define _SYS_SDMA_LENLSB_SIZE_MASK                (0xFFu)
#define _SYS_SDMA_LENLSB_SIZE_SHIFT               (0)
#define  SYS_SDMA_LENLSB_SIZE_DEFAULT             (0x00u)
#define  SYS_SDMA_LENLSB_SIZE_OF(x)               _CCL_VALUEOF(_SYS_SDMA_LENLSB_TYPE,x)


#define  SYS_SDMA_LENLSB_OF(x)             _CCL_VALUEOF(_SYS_SDMA_LENLSB_TYPE,x)

#define  SYS_SDMA_LENLSB_DEFAULT (_SYS_SDMA_LENLSB_TYPE)(  \
	 _CCL_PER_FDEFAULT(SYS,SDMA_LENLSB,SIZE)               \
	)

#define SYS_SDMA_LENLSB_RMK(addr) (_SYS_SDMA_LENLSB_TYPE)( \
	 _CCL_PER_FMK  (SYS,SDMA_LENLSB,SIZE,addr)             \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  SDMA_LENMSB      |
 * |___________________|
 *
 * SDMA_LENMSB - length most significant byte Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  SIZE  - for SYS_SDMA_CTRL_MODE_0, unit is 8-bit
 *               for SYS_SDMA_CTRL_MODE_1, unit is 8-bit
 *               for SYS_SDMA_CTRL_MODE_2, unit is 32-bit
\*-------------------------------------------------------------------------*/
#define _SYS_SDMA_LENMSB_OFFSET                   (0x000Eu)
#define _SYS_SDMA_LENMSB_SIZEH(h)                 SYS_SIZEH(h,SDMA_LENMSB)
#define _SYS_SDMA_LENMSB_LEN                      (8)                 /* 8-bit*/
#define _SYS_SDMA_LENMSB_TYPE                     uint8_t

#define _SYS_SDMA_LENMSB_SIZE_MASK                (0xFFu)
#define _SYS_SDMA_LENMSB_SIZE_SHIFT               (0)
#define  SYS_SDMA_LENMSB_SIZE_DEFAULT             (0x00u)
#define  SYS_SDMA_LENMSB_SIZE_OF(x)               _CCL_VALUEOF(_SYS_SDMA_LENMSB_TYPE,x)


#define  SYS_SDMA_LENMSB_OF(x)             _CCL_VALUEOF(_SYS_SDMA_LENMSB_TYPE,x)

#define  SYS_SDMA_LENMSB_DEFAULT (_SYS_SDMA_LENMSB_TYPE)(  \
	 _CCL_PER_FDEFAULT(SYS,SDMA_LENMSB,SIZE)               \
	)

#define SYS_SDMA_LENMSB_RMK(addr) (_SYS_SDMA_LENMSB_TYPE)( \
	 _CCL_PER_FMK  (SYS,SDMA_LENMSB,SIZE,addr)             \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  SDMA_DESTLSB     |
 * |___________________|
 *
 * SDMA_DESTLSB - destination address least significant byte Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  ADDR  - destination address (DMEM or Registers)
\*-------------------------------------------------------------------------*/
#define _SYS_SDMA_DESTLSB_OFFSET                   (0x000Fu)
#define _SYS_SDMA_DESTLSB_ADDRH(h)                 SYS_ADDRH(h,SDMA_DESTLSB)
#define _SYS_SDMA_DESTLSB_LEN                      (8)                 /* 8-bit*/
#define _SYS_SDMA_DESTLSB_TYPE                     uint8_t

#define _SYS_SDMA_DESTLSB_ADDR_MASK                (0xFFu)
#define _SYS_SDMA_DESTLSB_ADDR_SHIFT               (0)
#define  SYS_SDMA_DESTLSB_ADDR_DEFAULT             (0x00u)
#define  SYS_SDMA_DESTLSB_ADDR_OF(x)               _CCL_VALUEOF(_SYS_SDMA_DESTLSB_TYPE,x)


#define  SYS_SDMA_DESTLSB_OF(x)             _CCL_VALUEOF(_SYS_SDMA_DESTLSB_TYPE,x)

#define  SYS_SDMA_DESTLSB_DEFAULT (_SYS_SDMA_DESTLSB_TYPE)(  \
	 _CCL_PER_FDEFAULT(SYS,SDMA_DESTLSB,ADDR)                \
	)

#define SYS_SDMA_DESTLSB_RMK(addr) (_SYS_SDMA_DESTLSB_TYPE)( \
	 _CCL_PER_FMK  (SYS,SDMA_DESTLSB,ADDR,addr)               \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  SDMA_DESTMSB     |
 * |___________________|
 *
 * SDMA_DESTMSB - destination address most significant byte Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  ADDR  - destination address (DMEM or Registers)
\*-------------------------------------------------------------------------*/
#define _SYS_SDMA_DESTMSB_OFFSET                   (0x0010u)
#define _SYS_SDMA_DESTMSB_ADDRH(h)                 SYS_ADDRH(h,SDMA_DESTMSB)
#define _SYS_SDMA_DESTMSB_LEN                      (8)                 /* 8-bit*/
#define _SYS_SDMA_DESTMSB_TYPE                     uint8_t

#define _SYS_SDMA_DESTMSB_ADDR_MASK                (0xFFu)
#define _SYS_SDMA_DESTMSB_ADDR_SHIFT               (0)
#define  SYS_SDMA_DESTMSB_ADDR_DEFAULT             (0x00u)
#define  SYS_SDMA_DESTMSB_ADDR_OF(x)               _CCL_VALUEOF(_SYS_SDMA_DESTMSB_TYPE,x)


#define  SYS_SDMA_DESTMSB_OF(x)             _CCL_VALUEOF(_SYS_SDMA_DESTMSB_TYPE,x)

#define  SYS_SDMA_DESTMSB_DEFAULT (_SYS_SDMA_DESTMSB_TYPE)(  \
	 _CCL_PER_FDEFAULT(SYS,SDMA_DESTMSB,ADDR)                \
	)

#define SYS_SDMA_DESTMSB_RMK(addr) (_SYS_SDMA_DESTMSB_TYPE)( \
	 _CCL_PER_FMK  (SYS,SDMA_DESTMSB,ADDR,addr)               \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  S Y N C  I E N   |
 * |___________________|
 *
 * SYNC_IEN - Sync Interrupt Enable Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  DV - D-Domain V-Sync
 *                    DV   |   description
 *                ---------+-------------------
 *                 DISABLE | disable interrupt
 *                 ENABLE  | enable interrupt
 *
 * (rw)  IV - I-Domain V-Sync
 *                    IV   |   description
 *                ---------+-------------------
 *                 DISABLE | disable interrupt
 *                 ENABLE  | enable interrupt
\*-------------------------------------------------------------------------*/
#define _SYS_SYNC_IEN_OFFSET                  (0x0020u)
#define _SYS_SYNC_IEN_ADDRH(h)                SYS_ADDRH(h,SYNC_IEN)
#define _SYS_SYNC_IEN_LEN                     (8)                 /* 8-bit*/
#define _SYS_SYNC_IEN_TYPE                    uint8_t

#define _SYS_SYNC_IEN_DV_MASK                 (0x02u)
#define _SYS_SYNC_IEN_DV_SHIFT                (1)
#define  SYS_SYNC_IEN_DV_DEFAULT              (0x00u)
#define  SYS_SYNC_IEN_DV_OF(x)                _CCL_VALUEOF(_SYS_SYNC_IEN_TYPE,x)
#define  SYS_SYNC_IEN_DV_DISABLE              (0x00u)
#define  SYS_SYNC_IEN_DV_ENABLE               (0x01u)

#define _SYS_SYNC_IEN_IV_MASK                 (0x01u)
#define _SYS_SYNC_IEN_IV_SHIFT                (0)
#define  SYS_SYNC_IEN_IV_DEFAULT              (0x00u)
#define  SYS_SYNC_IEN_IV_OF(x)                _CCL_VALUEOF(_SYS_SYNC_IEN_TYPE,x)
#define  SYS_SYNC_IEN_IV_DISABLE              (0x00u)
#define  SYS_SYNC_IEN_IV_ENABLE               (0x01u)

#define  SYS_SYNC_IEN_OF(x)             _CCL_VALUEOF(_SYS_SYNC_IEN_TYPE,x)

#define  SYS_SYNC_IEN_DEFAULT (_SYS_SYNC_IEN_TYPE)(  \
	 _CCL_PER_FDEFAULT(SYS,SYNC_IEN,DV)              \
	|_CCL_PER_FDEFAULT(SYS,SYNC_IEN,IV)              \
	)

#define SYS_SYNC_IEN_RMK(dv, iv) (_SYS_SYNC_IEN_TYPE)( \
	 _CCL_PER_FMK  (SYS,SYNC_IEN,DV,dv)                \
	|_CCL_PER_FMK  (SYS,SYNC_IEN,IV,iv)                \
	)

#define SYS_SYNC_IEN_RMKS(dv, iv) (_SYS_SYNC_IEN_TYPE)( \
	 _CCL_PER_FMKS (SYS,SYNC_IEN,DV,dv)                 \
	|_CCL_PER_FMKS (SYS,SYNC_IEN,IV,iv)                 \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  S Y N C  I N T   |
 * |___________________|
 *
 * SYNC_INT - Sync Interrupt Status Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  DV - D-Domain V-Sync
 *                    DV   |   description
 *                ---------+-------------------
 *                   NOP   | no interrupt occurs
 *                 ASSERTED| interrupt occurs
 *
 * (rw)  IV - I-Domain V-Sync
 *                    IV   |   description
 *                ---------+-------------------
 *                   NOP   | no interrupt occurs
 *                 ASSERTED| interrupt occurs
\*-------------------------------------------------------------------------*/
#define _SYS_SYNC_INT_OFFSET                  (0x0021u)
#define _SYS_SYNC_INT_ADDRH(h)                SYS_ADDRH(h,SYNC_INT)
#define _SYS_SYNC_INT_LEN                     (8)                 /* 8-bit*/
#define _SYS_SYNC_INT_TYPE                    uint8_t

#define _SYS_SYNC_INT_DV_MASK                 (0x02u)
#define _SYS_SYNC_INT_DV_SHIFT                (1)
#define  SYS_SYNC_INT_DV_DEFAULT              (0x00u)
#define  SYS_SYNC_INT_DV_OF(x)                _CCL_VALUEOF(_SYS_SYNC_INT_TYPE,x)
#define  SYS_SYNC_INT_DV_NOP                  (0x00u)
#define  SYS_SYNC_INT_DV_ASSERTED             (0x01u)

#define _SYS_SYNC_INT_IV_MASK                 (0x01u)
#define _SYS_SYNC_INT_IV_SHIFT                (0)
#define  SYS_SYNC_INT_IV_DEFAULT              (0x00u)
#define  SYS_SYNC_INT_IV_OF(x)                _CCL_VALUEOF(_SYS_SYNC_INT_TYPE,x)
#define  SYS_SYNC_INT_IV_NOP                  (0x00u)
#define  SYS_SYNC_INT_IV_ASSERTED             (0x01u)

#define  SYS_SYNC_INT_OF(x)             _CCL_VALUEOF(_SYS_SYNC_INT_TYPE,x)

#define  SYS_SYNC_INT_DEFAULT (_SYS_SYNC_INT_TYPE)(  \
	 _CCL_PER_FDEFAULT(SYS,SYNC_INT,DV)              \
	|_CCL_PER_FDEFAULT(SYS,SYNC_INT,IV)              \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |     G I E 1       |
 * |___________________|
 *
 * GIE_1 - Global Interrupt Enable Register 1
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  VDIE - Video Decoder Interrupt Enable
 *                  VDIE   |   description
 *                ---------+-------------------
 *                 DISABLE | disable interrupt
 *                  ENABLE | enable interrupt
 *
 * (rw)  ADIE - Audio Decoder Interrupt Enable
 *                  ADIE   |   description
 *                ---------+-------------------
 *                 DISABLE | disable interrupt
 *                  ENABLE | enable interrupt
 *
 * (rw)  VBISLIIE - VBI Slicer Interrupt Enable
 *                VBISLIIE |   description
 *                ---------+-------------------
 *                 DISABLE | disable interrupt
 *                  ENABLE | enable interrupt
 *
 * (rw)  VBIPPRIE - VBI PPR Interrupt Enable
 *                VBIPPRIE |   description
 *                ---------+-------------------
 *                 DISABLE | disable interrupt
 *                  ENABLE | enable interrupt
 *
 * (rw)  VBIOSDIE - VBI OSD Interrupt Enable
 *                VBIOSDIE |   description
 *                ---------+-------------------
 *                 DISABLE | disable interrupt
 *                  ENABLE | enable interrupt
 *
 * (rw)  APLLIE - APLL/DDS Interrupt Enable
 *                  APLLIE |   description
 *                ---------+-------------------
 *                 DISABLE | disable interrupt
 *                  ENABLE | enable interrupt
 *
 * (rw)  AUTOSOYIE - AUTOSOY Interrupt Enable
 *                AUTOSOYIE|   description
 *                ---------+-------------------
 *                 DISABLE | disable interrupt
 *                  ENABLE | enable interrupt
 *
 * (rw)  SPIE - Sync Processor Interrupt Enable
 *                  SPIE   |   description
 *                ---------+-------------------
 *                 DISABLE | disable interrupt
 *                  ENABLE | enable interrupt
\*-------------------------------------------------------------------------*/
#define _SYS_GIE_1_OFFSET                  (0x0035u)
#define _SYS_GIE_1_ADDRH(h)                SYS_ADDRH(h,GIE_1)
#define _SYS_GIE_1_LEN                     (8)                 /* 8-bit*/
#define _SYS_GIE_1_TYPE                    uint8_t

#define _SYS_GIE_1_VDIE_MASK               (0x80u)
#define _SYS_GIE_1_VDIE_SHIFT              (7)
#define  SYS_GIE_1_VDIE_DEFAULT            (0x00u)
#define  SYS_GIE_1_VDIE_OF(x)              _CCL_VALUEOF(_SYS_GIE_1_TYPE,x)
#define  SYS_GIE_1_VDIE_DISABLE            (0x00u)
#define  SYS_GIE_1_VDIE_ENABLE             (0x01u)

#define _SYS_GIE_1_ADIE_MASK               (0x40u)
#define _SYS_GIE_1_ADIE_SHIFT              (6)
#define  SYS_GIE_1_ADIE_DEFAULT            (0x00u)
#define  SYS_GIE_1_ADIE_OF(x)              _CCL_VALUEOF(_SYS_GIE_1_TYPE,x)
#define  SYS_GIE_1_ADIE_DISABLE            (0x00u)
#define  SYS_GIE_1_ADIE_ENABLE             (0x01u)

#define _SYS_GIE_1_VBISLIIE_MASK           (0x20u)
#define _SYS_GIE_1_VBISLIIE_SHIFT          (5)
#define  SYS_GIE_1_VBISLIIE_DEFAULT        (0x00u)
#define  SYS_GIE_1_VBISLIIE_OF(x)          _CCL_VALUEOF(_SYS_GIE_1_TYPE,x)
#define  SYS_GIE_1_VBISLIIE_DISABLE        (0x00u)
#define  SYS_GIE_1_VBISLIIE_ENABLE         (0x01u)

#define _SYS_GIE_1_VBIPPRIE_MASK           (0x10u)
#define _SYS_GIE_1_VBIPPRIE_SHIFT          (4)
#define  SYS_GIE_1_VBIPPRIE_DEFAULT        (0x00u)
#define  SYS_GIE_1_VBIPPRIE_OF(x)          _CCL_VALUEOF(_SYS_GIE_1_TYPE,x)
#define  SYS_GIE_1_VBIPPRIE_DISABLE        (0x00u)
#define  SYS_GIE_1_VBIPPRIE_ENABLE         (0x01u)

#define _SYS_GIE_1_VBIOSDIE_MASK           (0x08u)
#define _SYS_GIE_1_VBIOSDIE_SHIFT          (3)
#define  SYS_GIE_1_VBIOSDIE_DEFAULT        (0x00u)
#define  SYS_GIE_1_VBIOSDIE_OF(x)          _CCL_VALUEOF(_SYS_GIE_1_TYPE,x)
#define  SYS_GIE_1_VBIOSDIE_DISABLE        (0x00u)
#define  SYS_GIE_1_VBIOSDIE_ENABLE         (0x01u)

#define _SYS_GIE_1_APLLIE_MASK             (0x04u)
#define _SYS_GIE_1_APLLIE_SHIFT            (2)
#define  SYS_GIE_1_APLLIE_DEFAULT          (0x00u)
#define  SYS_GIE_1_APLLIE_OF(x)            _CCL_VALUEOF(_SYS_GIE_1_TYPE,x)
#define  SYS_GIE_1_APLLIE_DISABLE          (0x00u)
#define  SYS_GIE_1_APLLIE_ENABLE           (0x01u)

#define _SYS_GIE_1_AUTOSOYIE_MASK          (0x02u)
#define _SYS_GIE_1_AUTOSOYIE_SHIFT         (1)
#define  SYS_GIE_1_AUTOSOYIE_DEFAULT       (0x00u)
#define  SYS_GIE_1_AUTOSOYIE_OF(x)         _CCL_VALUEOF(_SYS_GIE_1_TYPE,x)
#define  SYS_GIE_1_AUTOSOYIE_DISABLE       (0x00u)
#define  SYS_GIE_1_AUTOSOYIE_ENABLE        (0x01u)

#define _SYS_GIE_1_SPIE_MASK               (0x01u)
#define _SYS_GIE_1_SPIE_SHIFT              (0)
#define  SYS_GIE_1_SPIE_DEFAULT            (0x00u)
#define  SYS_GIE_1_SPIE_OF(x)              _CCL_VALUEOF(_SYS_GIE_1_TYPE,x)
#define  SYS_GIE_1_SPIE_DISABLE            (0x00u)
#define  SYS_GIE_1_SPIE_ENABLE             (0x01u)

#define  SYS_GIE_1_OF(x)             _CCL_VALUEOF(_SYS_GIE_1_TYPE,x)

#define  SYS_GIE_1_DEFAULT (_SYS_GIE_1_TYPE)(  \
	 _CCL_PER_FDEFAULT(SYS,GIE_1,VDIE)         \
	|_CCL_PER_FDEFAULT(SYS,GIE_1,ADIE)         \
	|_CCL_PER_FDEFAULT(SYS,GIE_1,VBISLIIE)     \
	|_CCL_PER_FDEFAULT(SYS,GIE_1,VBIPPRIE)     \
	|_CCL_PER_FDEFAULT(SYS,GIE_1,VBIOSDIE)     \
	|_CCL_PER_FDEFAULT(SYS,GIE_1,APLLIE)       \
	|_CCL_PER_FDEFAULT(SYS,GIE_1,AUTOSOYIE)    \
	|_CCL_PER_FDEFAULT(SYS,GIE_1,SPIE)         \
	)

#define SYS_GIE_1_RMK(vdie, adie, vbisliie, vbipprie, vbiosdie, apllie, autosoyie, spie) (_SYS_GIE_1_TYPE)( \
	 _CCL_PER_FMK  (SYS,GIE_1,VDIE,vdie)                \
	|_CCL_PER_FMK  (SYS,GIE_1,ADIE,adie)                \
	|_CCL_PER_FMK  (SYS,GIE_1,VBISLIIE,vbisliie)        \
	|_CCL_PER_FMK  (SYS,GIE_1,VBIPPRIE,vbipprie)        \
	|_CCL_PER_FMK  (SYS,GIE_1,VBIOSDIE,vbiosdie)        \
	|_CCL_PER_FMK  (SYS,GIE_1,APLLIE,apllie)            \
	|_CCL_PER_FMK  (SYS,GIE_1,AUTOSOYIE,autosoyie)      \
	|_CCL_PER_FMK  (SYS,GIE_1,SPIE,spie)                \
	)

#define SYS_GIE_1_RMKS(vdie, adie, vbisliie, vbipprie, vbiosdie, apllie, autosoyie, spie) (_SYS_GIE_1_TYPE)( \
	 _CCL_PER_FMKS (SYS,GIE_1,VDIE,vdie)                \
	|_CCL_PER_FMKS (SYS,GIE_1,ADIE,adie)                \
	|_CCL_PER_FMKS (SYS,GIE_1,VBISLIIE,vbisliie)        \
	|_CCL_PER_FMKS (SYS,GIE_1,VBIPPRIE,vbipprie)        \
	|_CCL_PER_FMKS (SYS,GIE_1,VBIOSDIE,vbiosdie)        \
	|_CCL_PER_FMKS (SYS,GIE_1,APLLIE,apllie)            \
	|_CCL_PER_FMKS (SYS,GIE_1,AUTOSOYIE,autosoyie)      \
	|_CCL_PER_FMKS (SYS,GIE_1,SPIE,spie)                \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |     G I P 1       |
 * |___________________|
 *
 * GIP_1 - Global Interrupt Pending Register 1
 *
 *-------------------------------------------------------------------------
 * FIPLDS (msb -> lsb)
 *
 * (rw)  VDIP - Video Decoder Interrupt Pending
 *                  VDIP   |   description
 *              -----------+---------------------------
 *              NOT_PENDING| interrupt is not pending
 *                 PENDING | interrupt pending
 *
 * (rw)  ADIP - Audio Decoder Interrupt Pending
 *                  ADIP   |   description
 *              -----------+---------------------------
 *              NOT_PENDING| interrupt is not pending
 *                 PENDING | interrupt pending
 *
 * (rw)  VBISLIIP - VBI Slicer Interrupt Pending
 *                VBISLIIP |   description
 *              -----------+---------------------------
 *              NOT_PENDING| interrupt is not pending
 *                 PENDING | interrupt pending
 *
 * (rw)  VBIPPRIP - VBI PPR Interrupt Pending
 *                VBIPPRIP |   description
 *              -----------+---------------------------
 *              NOT_PENDING| interrupt is not pending
 *                 PENDING | interrupt pending
 *
 * (rw)  VBIOSDIP - VBI OSD Interrupt Pending
 *                VBIOSDIP |   description
 *              -----------+---------------------------
 *              NOT_PENDING| interrupt is not pending
 *                 PENDING | interrupt pending
 *
 * (rw)  APLLIP - APLL/DDS Interrupt Pending
 *                  APLLIP |   description
 *              -----------+---------------------------
 *              NOT_PENDING| interrupt is not pending
 *                 PENDING | interrupt pending
 *
 * (rw)  AUTOSOYIP - AUTOSOY Interrupt Pending
 *                AUTOSOYIP|   description
 *              -----------+---------------------------
 *              NOT_PENDING| interrupt is not pending
 *                 PENDING | interrupt pending
 *
 * (rw)  SPIP - Sync Processor Interrupt Pending
 *                  SPIP   |   description
 *              -----------+---------------------------
 *              NOT_PENDING| interrupt is not pending
 *                 PENDING | interrupt pending
\*-------------------------------------------------------------------------*/
#define _SYS_GIP_1_OFFSET                  (0x0039u)
#define _SYS_GIP_1_ADDRH(h)                SYS_ADDRH(h,GIP_1)
#define _SYS_GIP_1_LEN                     (8)                 /* 8-bit*/
#define _SYS_GIP_1_TYPE                    uint8_t

#define _SYS_GIP_1_VDIP_MASK               (0x80u)
#define _SYS_GIP_1_VDIP_SHIFT              (7)
#define  SYS_GIP_1_VDIP_DEFAULT            (0x00u)
#define  SYS_GIP_1_VDIP_OF(x)              _CCL_VALUEOF(_SYS_GIP_1_TYPE,x)
#define  SYS_GIP_1_VDIP_NOT_PENDING        (0x00u)
#define  SYS_GIP_1_VDIP_PENDING            (0x01u)

#define _SYS_GIP_1_ADIP_MASK               (0x40u)
#define _SYS_GIP_1_ADIP_SHIFT              (6)
#define  SYS_GIP_1_ADIP_DEFAULT            (0x00u)
#define  SYS_GIP_1_ADIP_OF(x)              _CCL_VALUEOF(_SYS_GIP_1_TYPE,x)
#define  SYS_GIP_1_ADIP_NOT_PENDING        (0x00u)
#define  SYS_GIP_1_ADIP_PENDING            (0x01u)

#define _SYS_GIP_1_VBISLIIP_MASK           (0x20u)
#define _SYS_GIP_1_VBISLIIP_SHIFT          (5)
#define  SYS_GIP_1_VBISLIIP_DEFAULT        (0x00u)
#define  SYS_GIP_1_VBISLIIP_OF(x)          _CCL_VALUEOF(_SYS_GIP_1_TYPE,x)
#define  SYS_GIP_1_VBISLIIP_NOT_PENDING    (0x00u)
#define  SYS_GIP_1_VBISLIIP_PENDING        (0x01u)

#define _SYS_GIP_1_VBIPPRIP_MASK           (0x10u)
#define _SYS_GIP_1_VBIPPRIP_SHIFT          (4)
#define  SYS_GIP_1_VBIPPRIP_DEFAULT        (0x00u)
#define  SYS_GIP_1_VBIPPRIP_OF(x)          _CCL_VALUEOF(_SYS_GIP_1_TYPE,x)
#define  SYS_GIP_1_VBIPPRIP_NOT_PENDING    (0x00u)
#define  SYS_GIP_1_VBIPPRIP_PENDING        (0x01u)

#define _SYS_GIP_1_VBIOSDIP_MASK           (0x08u)
#define _SYS_GIP_1_VBIOSDIP_SHIFT          (3)
#define  SYS_GIP_1_VBIOSDIP_DEFAULT        (0x00u)
#define  SYS_GIP_1_VBIOSDIP_OF(x)          _CCL_VALUEOF(_SYS_GIP_1_TYPE,x)
#define  SYS_GIP_1_VBIOSDIP_NOT_PENDING    (0x00u)
#define  SYS_GIP_1_VBIOSDIP_PENDING        (0x01u)

#define _SYS_GIP_1_APLLIP_MASK             (0x04u)
#define _SYS_GIP_1_APLLIP_SHIFT            (2)
#define  SYS_GIP_1_APLLIP_DEFAULT          (0x00u)
#define  SYS_GIP_1_APLLIP_OF(x)            _CCL_VALUEOF(_SYS_GIP_1_TYPE,x)
#define  SYS_GIP_1_APLLIP_NOT_PENDING      (0x00u)
#define  SYS_GIP_1_APLLIP_PENDING          (0x01u)

#define _SYS_GIP_1_AUTOSOYIP_MASK          (0x02u)
#define _SYS_GIP_1_AUTOSOYIP_SHIFT         (1)
#define  SYS_GIP_1_AUTOSOYIP_DEFAULT       (0x00u)
#define  SYS_GIP_1_AUTOSOYIP_OF(x)         _CCL_VALUEOF(_SYS_GIP_1_TYPE,x)
#define  SYS_GIP_1_AUTOSOYIP_NOT_PENDING   (0x00u)
#define  SYS_GIP_1_AUTOSOYIP_PENDING       (0x01u)

#define _SYS_GIP_1_SPIP_MASK               (0x01u)
#define _SYS_GIP_1_SPIP_SHIFT              (0)
#define  SYS_GIP_1_SPIP_DEFAULT            (0x00u)
#define  SYS_GIP_1_SPIP_OF(x)              _CCL_VALUEOF(_SYS_GIP_1_TYPE,x)
#define  SYS_GIP_1_SPIP_NOT_PENDING        (0x00u)
#define  SYS_GIP_1_SPIP_PENDING            (0x01u)

#define  SYS_GIP_1_OF(x)             _CCL_VALUEOF(_SYS_GIP_1_TYPE,x)

#define  SYS_GIP_1_DEFAULT (_SYS_GIP_1_TYPE)(  \
	 _CCL_PER_FDEFAULT(SYS,GIP_1,VDIP)         \
	|_CCL_PER_FDEFAULT(SYS,GIP_1,ADIP)         \
	|_CCL_PER_FDEFAULT(SYS,GIP_1,VBISLIIP)     \
	|_CCL_PER_FDEFAULT(SYS,GIP_1,VBIPPRIP)     \
	|_CCL_PER_FDEFAULT(SYS,GIP_1,VBIOSDIP)     \
	|_CCL_PER_FDEFAULT(SYS,GIP_1,APLLIP)       \
	|_CCL_PER_FDEFAULT(SYS,GIP_1,AUTOSOYIP)    \
	|_CCL_PER_FDEFAULT(SYS,GIP_1,SPIP)         \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  S R S T 0        |
 * |___________________|
 *
 * SRST0 - Software Reset Register 0
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  HDMI (High Definition Multimedia Interface) - HDMI Rx Software Reset
 *                  HDMI   |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
 *
 * (rw)  SLCR (VBI Slicer) -
 *                  SLCR   |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
 *
 * (rw)  IFD (IF Demodulator) -
 *                   IFD   |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
 *
 * (rw)  VDEC (Video Decoder) -
 *                   VDEC  |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
 *
 * (rw)  FLASH (Flash Controller) -
 *                  FLASH  |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
 *
 * (rw)  SPIDMA (Serial Port Interface Direct Memory Access) -
 *                 SPIDMA  |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
 *
 * (rw)  RBUS (Register Bus) -
 *                  RBUS   |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
\*-------------------------------------------------------------------------*/
#define _SYS_SRST0_OFFSET                   (0x00A0u)
#define _SYS_SRST0_ADDRH(h)                 SYS_ADDRH(h,SRST0)
#define _SYS_SRST0_LEN                      (8)                 /* 8-bit*/
#define _SYS_SRST0_TYPE                     uint8_t

#define _SYS_SRST0_HDMI_MASK                (0x80u)
#define _SYS_SRST0_HDMI_SHIFT               (7)
#define  SYS_SRST0_HDMI_DEFAULT             (0x00u)
#define  SYS_SRST0_HDMI_OF(x)               _CCL_VALUEOF(_SYS_SRST0_TYPE,x)
#define  SYS_SRST0_HDMI_RESET               (0x00u)
#define  SYS_SRST0_HDMI_NOP                 (0x01u)

#define _SYS_SRST0_SLCR_MASK                (0x40u)
#define _SYS_SRST0_SLCR_SHIFT               (6)
#define  SYS_SRST0_SLCR_DEFAULT             (0x00u)
#define  SYS_SRST0_SLCR_OF(x)               _CCL_VALUEOF(_SYS_SRST0_TYPE,x)
#define  SYS_SRST0_SLCR_RESET               (0x00u)
#define  SYS_SRST0_SLCR_NOP                 (0x01u)

#define _SYS_SRST0_IFD_MASK                 (0x20u)
#define _SYS_SRST0_IFD_SHIFT                (5)
#define  SYS_SRST0_IFD_DEFAULT              (0x00u)
#define  SYS_SRST0_IFD_OF(x)                _CCL_VALUEOF(_SYS_SRST0_TYPE,x)
#define  SYS_SRST0_IFD_RESET                (0x00u)
#define  SYS_SRST0_IFD_NOP                  (0x01u)

#define _SYS_SRST0_VDEC_MASK                (0x10u)
#define _SYS_SRST0_VDEC_SHIFT               (4)
#define  SYS_SRST0_VDEC_DEFAULT             (0x00u)
#define  SYS_SRST0_VDEC_OF(x)               _CCL_VALUEOF(_SYS_SRST0_TYPE,x)
#define  SYS_SRST0_VDEC_RESET               (0x00u)
#define  SYS_SRST0_VDEC_NOP                 (0x01u)

#define _SYS_SRST0_FLASH_MASK               (0x04u)
#define _SYS_SRST0_FLASH_SHIFT              (2)
#define  SYS_SRST0_FLASH_DEFAULT            (0x00u)
#define  SYS_SRST0_FLASH_OF(x)              _CCL_VALUEOF(_SYS_SRST0_TYPE,x)
#define  SYS_SRST0_FLASH_RESET              (0x00u)
#define  SYS_SRST0_FLASH_NOP                (0x01u)

#define _SYS_SRST0_SPIDMA_MASK              (0x02u)
#define _SYS_SRST0_SPIDMA_SHIFT             (1)
#define  SYS_SRST0_SPIDMA_DEFAULT           (0x00u)
#define  SYS_SRST0_SPIDMA_OF(x)             _CCL_VALUEOF(_SYS_SRST0_TYPE,x)
#define  SYS_SRST0_SPIDMA_RESET             (0x00u)
#define  SYS_SRST0_SPIDMA_NOP               (0x01u)

#define _SYS_SRST0_RBUS_MASK                (0x01u)
#define _SYS_SRST0_RBUS_SHIFT               (0)
#define  SYS_SRST0_RBUS_DEFAULT             (0x00u)
#define  SYS_SRST0_RBUS_OF(x)               _CCL_VALUEOF(_SYS_SRST0_TYPE,x)
#define  SYS_SRST0_RBUS_RESET               (0x00u)
#define  SYS_SRST0_RBUS_NOP                 (0x01u)

#define  SYS_SRST0_OF(x)             _CCL_VALUEOF(_SYS_SRST0_TYPE,x)

#define  SYS_SRST0_DEFAULT (_SYS_SRST0_TYPE)(  \
	 _CCL_PER_FDEFAULT(SYS,SRST0,HDMI  )       \
	|_CCL_PER_FDEFAULT(SYS,SRST0,SLCR  )       \
	|_CCL_PER_FDEFAULT(SYS,SRST0,IFD   )       \
	|_CCL_PER_FDEFAULT(SYS,SRST0,VDEC  )       \
	|_CCL_PER_FDEFAULT(SYS,SRST0,FLASH )       \
	|_CCL_PER_FDEFAULT(SYS,SRST0,SPIDMA)       \
	|_CCL_PER_FDEFAULT(SYS,SRST0,RBUS  )       \
	)

#define SYS_SRST0_RMK(hdmi, slcr, ifd, vdec, flash, spidma, rbus) (_SYS_SRST0_TYPE)( \
	 _CCL_PER_FMK  (SYS,SRST0,HDMI  ,hdmi  )     \
	|_CCL_PER_FMK  (SYS,SRST0,SLCR  ,slcr  )     \
	|_CCL_PER_FMK  (SYS,SRST0,IFD   ,ifd   )     \
	|_CCL_PER_FMK  (SYS,SRST0,VDEC  ,vdec  )     \
	|_CCL_PER_FMK  (SYS,SRST0,FLASH ,flash )     \
	|_CCL_PER_FMK  (SYS,SRST0,SPIDMA,spidma)     \
	|_CCL_PER_FMK  (SYS,SRST0,RBUS  ,rbus  )     \
	)

#define SYS_SRST0_RMKS(hdmi, slcr, ifd, vdec, flash, spidma, rbus) (_SYS_SRST0_TYPE)( \
	 _CCL_PER_FMKS (SYS,SRST0,HDMI  ,hdmi  )     \
	|_CCL_PER_FMKS (SYS,SRST0,SLCR  ,slcr  )     \
	|_CCL_PER_FMKS (SYS,SRST0,IFD   ,ifd   )     \
	|_CCL_PER_FMKS (SYS,SRST0,VDEC  ,vdec  )     \
	|_CCL_PER_FMKS (SYS,SRST0,FLASH ,flash )     \
	|_CCL_PER_FMKS (SYS,SRST0,SPIDMA,spidma)     \
	|_CCL_PER_FMKS (SYS,SRST0,RBUS  ,rbus  )     \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  S R S T 1        |
 * |___________________|
 *
 * SRST1 - Software Reset Register 1
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  DISPD (Display D-Domain) -
 *                  DISPD  |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
 *
 * (rw)  DISPI (Display I-Domain) -
 *                  DISPI  |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
 *
 * (rw)  ADAC2 (Audio DAC-2) -
 *                  ADAC2  |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
 *
 * (rw)  ADAC1 (Audio DAC-1) -
 *                  ADAC1  |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
 *
 * (rw)  AADC (Audio ADC) -
 *                  AADC   |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
 *
 * (rw)  AUD -
 *                   AUD   |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
\*-------------------------------------------------------------------------*/
#define _SYS_SRST1_OFFSET                   (0x00A1u)
#define _SYS_SRST1_ADDRH(h)                 SYS_ADDRH(h,SRST1)
#define _SYS_SRST1_LEN                      (8)                 /* 8-bit*/
#define _SYS_SRST1_TYPE                     uint8_t

#define _SYS_SRST1_DISPD_MASK               (0x20u)
#define _SYS_SRST1_DISPD_SHIFT              (5)
#define  SYS_SRST1_DISPD_DEFAULT            (0x00u)
#define  SYS_SRST1_DISPD_OF(x)              _CCL_VALUEOF(_SYS_SRST1_TYPE,x)
#define  SYS_SRST1_DISPD_RESET              (0x00u)
#define  SYS_SRST1_DISPD_NOP                (0x01u)

#define _SYS_SRST1_DISPI_MASK               (0x10u)
#define _SYS_SRST1_DISPI_SHIFT              (4)
#define  SYS_SRST1_DISPI_DEFAULT            (0x00u)
#define  SYS_SRST1_DISPI_OF(x)              _CCL_VALUEOF(_SYS_SRST1_TYPE,x)
#define  SYS_SRST1_DISPI_RESET              (0x00u)
#define  SYS_SRST1_DISPI_NOP                (0x01u)

#define _SYS_SRST1_ADAC2_MASK               (0x08u)
#define _SYS_SRST1_ADAC2_SHIFT              (3)
#define  SYS_SRST1_ADAC2_DEFAULT            (0x00u)
#define  SYS_SRST1_ADAC2_OF(x)              _CCL_VALUEOF(_SYS_SRST1_TYPE,x)
#define  SYS_SRST1_ADAC2_RESET              (0x00u)
#define  SYS_SRST1_ADAC2_NOP                (0x01u)

#define _SYS_SRST1_ADAC1_MASK               (0x04u)
#define _SYS_SRST1_ADAC1_SHIFT              (2)
#define  SYS_SRST1_ADAC1_DEFAULT            (0x00u)
#define  SYS_SRST1_ADAC1_OF(x)              _CCL_VALUEOF(_SYS_SRST1_TYPE,x)
#define  SYS_SRST1_ADAC1_RESET              (0x00u)
#define  SYS_SRST1_ADAC1_NOP                (0x01u)

#define _SYS_SRST1_AADC_MASK                (0x02u)
#define _SYS_SRST1_AADC_SHIFT               (1)
#define  SYS_SRST1_AADC_DEFAULT             (0x00u)
#define  SYS_SRST1_AADC_OF(x)               _CCL_VALUEOF(_SYS_SRST1_TYPE,x)
#define  SYS_SRST1_AADC_RESET               (0x00u)
#define  SYS_SRST1_AADC_NOP                 (0x01u)

#define _SYS_SRST1_AUD_MASK                 (0x01u)
#define _SYS_SRST1_AUD_SHIFT                (0)
#define  SYS_SRST1_AUD_DEFAULT              (0x00u)
#define  SYS_SRST1_AUD_OF(x)                _CCL_VALUEOF(_SYS_SRST1_TYPE,x)
#define  SYS_SRST1_AUD_RESET                (0x00u)
#define  SYS_SRST1_AUD_NOP                  (0x01u)

#define  SYS_SRST1_OF(x)             _CCL_VALUEOF(_SYS_SRST1_TYPE,x)

#define  SYS_SRST1_DEFAULT (_SYS_SRST1_TYPE)(  \
	 _CCL_PER_FDEFAULT(SYS,SRST1,DISPD)        \
	|_CCL_PER_FDEFAULT(SYS,SRST1,DISPI)        \
	|_CCL_PER_FDEFAULT(SYS,SRST1,ADAC2)        \
	|_CCL_PER_FDEFAULT(SYS,SRST1,ADAC1)        \
	|_CCL_PER_FDEFAULT(SYS,SRST1,AADC )        \
	|_CCL_PER_FDEFAULT(SYS,SRST1,AUD  )        \
	)

#define SYS_SRST1_RMK(dispd, dispi, adac2, adac1, aadc, aud) (_SYS_SRST1_TYPE)( \
	 _CCL_PER_FMK  (SYS,SRST1,DISPD,dispd)     \
	|_CCL_PER_FMK  (SYS,SRST1,DISPI,dispi)     \
	|_CCL_PER_FMK  (SYS,SRST1,ADAC2,adac2)     \
	|_CCL_PER_FMK  (SYS,SRST1,ADAC1,adac1)     \
	|_CCL_PER_FMK  (SYS,SRST1,AADC ,aadc )     \
	|_CCL_PER_FMK  (SYS,SRST1,AUD  ,aud  )     \
	)

#define SYS_SRST1_RMKS(dispd, dispi, adac2, adac1, aadc, aud) (_SYS_SRST1_TYPE)( \
	 _CCL_PER_FMKS (SYS,SRST1,DISPD,dispd)     \
	|_CCL_PER_FMKS (SYS,SRST1,DISPI,dispi)     \
	|_CCL_PER_FMKS (SYS,SRST1,ADAC2,adac2)     \
	|_CCL_PER_FMKS (SYS,SRST1,ADAC1,adac1)     \
	|_CCL_PER_FMKS (SYS,SRST1,AADC ,aadc )     \
	|_CCL_PER_FMKS (SYS,SRST1,AUD  ,aud  )     \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  S R S T 2        |
 * |___________________|
 *
 * SRST2 - Software Reset Register 2
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  OSD (VBI OSD) -
 *                   OSD   |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
 *
 * (rw)  VTE (VBI VTE) -
 *                   VTE   |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
 *
 * (rw)  PPR (VBI PPR) -
 *                   PPR   |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
 *
 * (rw)  VBIBUS (VBI RBUS/DBUS) -
 *                  VBIBUS |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
 *
 * (rw)  SM_UI12K (Share-Mem UI OSD-12KB function software reset) -
 *                SM_UI12K |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
 *
 * (rw)  SM_UI8K (Share-Mem UI OSD-8KB function software reset) -
 *                 SM_UI8K |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
 *
 * (rw)  SM_TT13K (Share-Mem TT-13KB function software reset) -
 *                SM_TT13K |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
\*-------------------------------------------------------------------------*/
#define _SYS_SRST2_OFFSET                   (0x00A2u)
#define _SYS_SRST2_ADDRH(h)                 SYS_ADDRH(h,SRST2)
#define _SYS_SRST2_LEN                      (8)                 /* 8-bit*/
#define _SYS_SRST2_TYPE                     uint8_t

#define _SYS_SRST2_OSD_MASK                 (0x80u)
#define _SYS_SRST2_OSD_SHIFT                (7)
#define  SYS_SRST2_OSD_DEFAULT              (0x00u)
#define  SYS_SRST2_OSD_OF(x)                _CCL_VALUEOF(_SYS_SRST2_TYPE,x)
#define  SYS_SRST2_OSD_RESET                (0x00u)
#define  SYS_SRST2_OSD_NOP                  (0x01u)

#define _SYS_SRST2_VTE_MASK                 (0x40u)
#define _SYS_SRST2_VTE_SHIFT                (6)
#define  SYS_SRST2_VTE_DEFAULT              (0x00u)
#define  SYS_SRST2_VTE_OF(x)                _CCL_VALUEOF(_SYS_SRST2_TYPE,x)
#define  SYS_SRST2_VTE_RESET                (0x00u)
#define  SYS_SRST2_VTE_NOP                  (0x01u)

#define _SYS_SRST2_PPR_MASK                 (0x20u)
#define _SYS_SRST2_PPR_SHIFT                (5)
#define  SYS_SRST2_PPR_DEFAULT              (0x00u)
#define  SYS_SRST2_PPR_OF(x)                _CCL_VALUEOF(_SYS_SRST2_TYPE,x)
#define  SYS_SRST2_PPR_RESET                (0x00u)
#define  SYS_SRST2_PPR_NOP                  (0x01u)

#define _SYS_SRST2_VBIBUS_MASK              (0x10u)
#define _SYS_SRST2_VBIBUS_SHIFT             (4)
#define  SYS_SRST2_VBIBUS_DEFAULT           (0x00u)
#define  SYS_SRST2_VBIBUS_OF(x)             _CCL_VALUEOF(_SYS_SRST2_TYPE,x)
#define  SYS_SRST2_VBIBUS_RESET             (0x00u)
#define  SYS_SRST2_VBIBUS_NOP               (0x01u)

#define _SYS_SRST2_SM_UI12K_MASK            (0x04u)
#define _SYS_SRST2_SM_UI12K_SHIFT           (2)
#define  SYS_SRST2_SM_UI12K_DEFAULT         (0x00u)
#define  SYS_SRST2_SM_UI12K_OF(x)           _CCL_VALUEOF(_SYS_SRST2_TYPE,x)
#define  SYS_SRST2_SM_UI12K_RESET           (0x00u)
#define  SYS_SRST2_SM_UI12K_NOP             (0x01u)

#define _SYS_SRST2_SM_UI8K_MASK             (0x02u)
#define _SYS_SRST2_SM_UI8K_SHIFT            (1)
#define  SYS_SRST2_SM_UI8K_DEFAULT          (0x00u)
#define  SYS_SRST2_SM_UI8K_OF(x)            _CCL_VALUEOF(_SYS_SRST2_TYPE,x)
#define  SYS_SRST2_SM_UI8K_RESET            (0x00u)
#define  SYS_SRST2_SM_UI8K_NOP              (0x01u)

#define _SYS_SRST2_SM_TT13K_MASK            (0x01u)
#define _SYS_SRST2_SM_TT13K_SHIFT           (0)
#define  SYS_SRST2_SM_TT13K_DEFAULT         (0x00u)
#define  SYS_SRST2_SM_TT13K_OF(x)           _CCL_VALUEOF(_SYS_SRST2_TYPE,x)
#define  SYS_SRST2_SM_TT13K_RESET           (0x00u)
#define  SYS_SRST2_SM_TT13K_NOP             (0x01u)

#define  SYS_SRST2_OF(x)             _CCL_VALUEOF(_SYS_SRST2_TYPE,x)

#define  SYS_SRST2_DEFAULT (_SYS_SRST2_TYPE)(     \
	 _CCL_PER_FDEFAULT(SYS,SRST2,OSD     )        \
	|_CCL_PER_FDEFAULT(SYS,SRST2,VTE     )        \
	|_CCL_PER_FDEFAULT(SYS,SRST2,PPR     )        \
	|_CCL_PER_FDEFAULT(SYS,SRST2,VBIBUS  )        \
	|_CCL_PER_FDEFAULT(SYS,SRST2,SM_UI12K)        \
	|_CCL_PER_FDEFAULT(SYS,SRST2,SM_UI8K )        \
	|_CCL_PER_FDEFAULT(SYS,SRST2,SM_TT13K)        \
	)

#define SYS_SRST2_RMK(osd, vte, ppr, vbibus, sm_ui12k, sm_ui8k, sm_tt13k) (_SYS_SRST2_TYPE)( \
	 _CCL_PER_FMK  (SYS,SRST2,OSD     ,osd     )     \
	|_CCL_PER_FMK  (SYS,SRST2,VTE     ,vte     )     \
	|_CCL_PER_FMK  (SYS,SRST2,PPR     ,ppr     )     \
	|_CCL_PER_FMK  (SYS,SRST2,VBIBUS  ,vbibus  )     \
	|_CCL_PER_FMK  (SYS,SRST2,SM_UI12K,sm_ui12k)     \
	|_CCL_PER_FMK  (SYS,SRST2,SM_UI8K ,sm_ui8k )     \
	|_CCL_PER_FMK  (SYS,SRST2,SM_TT13K,sm_tt13k)     \
	)

#define SYS_SRST2_RMKS(osd, vte, ppr, vbibus, sm_ui12k, sm_ui8k, sm_tt13k) (_SYS_SRST2_TYPE)( \
	 _CCL_PER_FMKS (SYS,SRST2,OSD     ,osd     )     \
	|_CCL_PER_FMKS (SYS,SRST2,VTE     ,vte     )     \
	|_CCL_PER_FMKS (SYS,SRST2,PPR     ,ppr     )     \
	|_CCL_PER_FMKS (SYS,SRST2,VBIBUS  ,vbibus  )     \
	|_CCL_PER_FMKS (SYS,SRST2,SM_UI12K,sm_ui12k)     \
	|_CCL_PER_FMKS (SYS,SRST2,SM_UI8K ,sm_ui8k )     \
	|_CCL_PER_FMKS (SYS,SRST2,SM_TT13K,sm_tt13k)     \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  S R S T 3        |
 * |___________________|
 *
 * SRST3 - Software Reset Register 3
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  PWM (Pulse-Width Modulation) -
 *                   PWM   |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
 *
 * (rw)  WDT (Watch-Dog Timer) -
 *                   WDT   |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
 *
 * (rw)  TMR2 (Timer 2) -
 *                  TMR2   |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
 *
 * (rw)  TMR1 (Timer 1) -
 *                   TMR1  |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
 *
 * (rw)  TMR0 (Timer 0) -
 *                   TMR0  |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
 *
 * (rw)  UART1 (UART-1) -
 *                  UART1  |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
 *
 * (rw)  UART0 (UART-0) -
 *                  UART0  |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
 *
 * (rw)  LSADC (LSADC) -
 *                  LSADC  |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
\*-------------------------------------------------------------------------*/
#define _SYS_SRST3_OFFSET                   (0x00A3u)
#define _SYS_SRST3_ADDRH(h)                 SYS_ADDRH(h,SRST3)
#define _SYS_SRST3_LEN                      (8)                 /* 8-bit*/
#define _SYS_SRST3_TYPE                     uint8_t

#define _SYS_SRST3_PWM_MASK                 (0x80u)
#define _SYS_SRST3_PWM_SHIFT                (7)
#define  SYS_SRST3_PWM_DEFAULT              (0x00u)
#define  SYS_SRST3_PWM_OF(x)                _CCL_VALUEOF(_SYS_SRST3_TYPE,x)
#define  SYS_SRST3_PWM_RESET                (0x00u)
#define  SYS_SRST3_PWM_NOP                  (0x01u)

#define _SYS_SRST3_WDT_MASK                 (0x40u)
#define _SYS_SRST3_WDT_SHIFT                (6)
#define  SYS_SRST3_WDT_DEFAULT              (0x00u)
#define  SYS_SRST3_WDT_OF(x)                _CCL_VALUEOF(_SYS_SRST3_TYPE,x)
#define  SYS_SRST3_WDT_RESET                (0x00u)
#define  SYS_SRST3_WDT_NOP                  (0x01u)

#define _SYS_SRST3_TMR2_MASK                (0x20u)
#define _SYS_SRST3_TMR2_SHIFT               (5)
#define  SYS_SRST3_TMR2_DEFAULT             (0x00u)
#define  SYS_SRST3_TMR2_OF(x)               _CCL_VALUEOF(_SYS_SRST3_TYPE,x)
#define  SYS_SRST3_TMR2_RESET               (0x00u)
#define  SYS_SRST3_TMR2_NOP                 (0x01u)

#define _SYS_SRST3_TMR1_MASK                (0x10u)
#define _SYS_SRST3_TMR1_SHIFT               (4)
#define  SYS_SRST3_TMR1_DEFAULT             (0x00u)
#define  SYS_SRST3_TMR1_OF(x)               _CCL_VALUEOF(_SYS_SRST3_TYPE,x)
#define  SYS_SRST3_TMR1_RESET               (0x00u)
#define  SYS_SRST3_TMR1_NOP                 (0x01u)

#define _SYS_SRST3_TMR0_MASK                (0x08u)
#define _SYS_SRST3_TMR0_SHIFT               (3)
#define  SYS_SRST3_TMR0_DEFAULT             (0x00u)
#define  SYS_SRST3_TMR0_OF(x)               _CCL_VALUEOF(_SYS_SRST3_TYPE,x)
#define  SYS_SRST3_TMR0_RESET               (0x00u)
#define  SYS_SRST3_TMR0_NOP                 (0x01u)

#define _SYS_SRST3_UART1_MASK               (0x04u)
#define _SYS_SRST3_UART1_SHIFT              (2)
#define  SYS_SRST3_UART1_DEFAULT            (0x00u)
#define  SYS_SRST3_UART1_OF(x)              _CCL_VALUEOF(_SYS_SRST3_TYPE,x)
#define  SYS_SRST3_UART1_RESET              (0x00u)
#define  SYS_SRST3_UART1_NOP                (0x01u)

#define _SYS_SRST3_UART0_MASK               (0x02u)
#define _SYS_SRST3_UART0_SHIFT              (1)
#define  SYS_SRST3_UART0_DEFAULT            (0x00u)
#define  SYS_SRST3_UART0_OF(x)              _CCL_VALUEOF(_SYS_SRST3_TYPE,x)
#define  SYS_SRST3_UART0_RESET              (0x00u)
#define  SYS_SRST3_UART0_NOP                (0x01u)

#define _SYS_SRST3_LSADC_MASK               (0x01u)
#define _SYS_SRST3_LSADC_SHIFT              (0)
#define  SYS_SRST3_LSADC_DEFAULT            (0x00u)
#define  SYS_SRST3_LSADC_OF(x)              _CCL_VALUEOF(_SYS_SRST3_TYPE,x)
#define  SYS_SRST3_LSADC_RESET              (0x00u)
#define  SYS_SRST3_LSADC_NOP                (0x01u)

#define  SYS_SRST3_OF(x)             _CCL_VALUEOF(_SYS_SRST3_TYPE,x)

#define  SYS_SRST3_DEFAULT (_SYS_SRST3_TYPE)(  \
	 _CCL_PER_FDEFAULT(SYS,SRST3,PWM  )        \
	|_CCL_PER_FDEFAULT(SYS,SRST3,WDT  )        \
	|_CCL_PER_FDEFAULT(SYS,SRST3,TMR2 )        \
	|_CCL_PER_FDEFAULT(SYS,SRST3,TMR1 )        \
	|_CCL_PER_FDEFAULT(SYS,SRST3,TMR0 )        \
	|_CCL_PER_FDEFAULT(SYS,SRST3,UART1)        \
	|_CCL_PER_FDEFAULT(SYS,SRST3,UART0)        \
	|_CCL_PER_FDEFAULT(SYS,SRST3,LSADC)        \
	)

#define SYS_SRST3_RMK(pwm, wdt, tmr2, tmr1, tmr0, uart1, uart0, lsadc) (_SYS_SRST3_TYPE)( \
	 _CCL_PER_FMK  (SYS,SRST3,PWM  ,pwm  )     \
	|_CCL_PER_FMK  (SYS,SRST3,WDT  ,wdt  )     \
	|_CCL_PER_FMK  (SYS,SRST3,TMR2 ,tmr2 )     \
	|_CCL_PER_FMK  (SYS,SRST3,TMR1 ,tmr1 )     \
	|_CCL_PER_FMK  (SYS,SRST3,TMR0 ,tmr0 )     \
	|_CCL_PER_FMK  (SYS,SRST3,UART1,uart1)     \
	|_CCL_PER_FMK  (SYS,SRST3,UART0,uart0)     \
	|_CCL_PER_FMK  (SYS,SRST3,LSADC,lsadc)     \
	)

#define SYS_SRST3_RMKS(pwm, wdt, tmr2, tmr1, tmr0, uart1, uart0, lsadc) (_SYS_SRST3_TYPE)( \
	 _CCL_PER_FMKS (SYS,SRST3,PWM  ,pwm  )     \
	|_CCL_PER_FMKS (SYS,SRST3,WDT  ,wdt  )     \
	|_CCL_PER_FMKS (SYS,SRST3,TMR2 ,tmr2 )     \
	|_CCL_PER_FMKS (SYS,SRST3,TMR1 ,tmr1 )     \
	|_CCL_PER_FMKS (SYS,SRST3,TMR0 ,tmr0 )     \
	|_CCL_PER_FMKS (SYS,SRST3,UART1,uart1)     \
	|_CCL_PER_FMKS (SYS,SRST3,UART0,uart0)     \
	|_CCL_PER_FMKS (SYS,SRST3,LSADC,lsadc)     \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  S R S T 4        |
 * |___________________|
 *
 * SRST4 - Software Reset Register 4
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  GPIO (General Purpose I/O) -
 *                  GPIO   |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
 *
 * (rw)  IRDA (IrDA) -
 *                  IRDA   |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
 *
 * (rw)  I2C2 (I2C 2) -
 *                  I2C2   |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
 *
 * (rw)  I2C1 (I2C 1) -
 *                  I2C1   |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
 *
 * (rw)  I2C0 (I2C 0) -
 *                  I2C0   |   description
 *                ---------+-------------------
 *                  RESET  |   Reset
 *                   NOP   |   No Operation
\*-------------------------------------------------------------------------*/
#define _SYS_SRST4_OFFSET                   (0x00A4u)
#define _SYS_SRST4_ADDRH(h)                 SYS_ADDRH(h,SRST4)
#define _SYS_SRST4_LEN                      (8)                 /* 8-bit*/
#define _SYS_SRST4_TYPE                     uint8_t

#define _SYS_SRST4_GPIO_MASK                (0x10u)
#define _SYS_SRST4_GPIO_SHIFT               (4)
#define  SYS_SRST4_GPIO_DEFAULT             (0x00u)
#define  SYS_SRST4_GPIO_OF(x)               _CCL_VALUEOF(_SYS_SRST4_TYPE,x)
#define  SYS_SRST4_GPIO_RESET               (0x00u)
#define  SYS_SRST4_GPIO_NOP                 (0x01u)

#define _SYS_SRST4_IRDA_MASK                (0x08u)
#define _SYS_SRST4_IRDA_SHIFT               (3)
#define  SYS_SRST4_IRDA_DEFAULT             (0x00u)
#define  SYS_SRST4_IRDA_OF(x)               _CCL_VALUEOF(_SYS_SRST4_TYPE,x)
#define  SYS_SRST4_IRDA_RESET               (0x00u)
#define  SYS_SRST4_IRDA_NOP                 (0x01u)

#define _SYS_SRST4_I2C2_MASK                (0x04u)
#define _SYS_SRST4_I2C2_SHIFT               (2)
#define  SYS_SRST4_I2C2_DEFAULT             (0x00u)
#define  SYS_SRST4_I2C2_OF(x)               _CCL_VALUEOF(_SYS_SRST4_TYPE,x)
#define  SYS_SRST4_I2C2_RESET               (0x00u)
#define  SYS_SRST4_I2C2_NOP                 (0x01u)

#define _SYS_SRST4_I2C1_MASK                (0x02u)
#define _SYS_SRST4_I2C1_SHIFT               (1)
#define  SYS_SRST4_I2C1_DEFAULT             (0x00u)
#define  SYS_SRST4_I2C1_OF(x)               _CCL_VALUEOF(_SYS_SRST4_TYPE,x)
#define  SYS_SRST4_I2C1_RESET               (0x00u)
#define  SYS_SRST4_I2C1_NOP                 (0x01u)

#define _SYS_SRST4_I2C0_MASK                (0x01u)
#define _SYS_SRST4_I2C0_SHIFT               (0)
#define  SYS_SRST4_I2C0_DEFAULT             (0x00u)
#define  SYS_SRST4_I2C0_OF(x)               _CCL_VALUEOF(_SYS_SRST4_TYPE,x)
#define  SYS_SRST4_I2C0_RESET               (0x00u)
#define  SYS_SRST4_I2C0_NOP                 (0x01u)

#define  SYS_SRST4_OF(x)             _CCL_VALUEOF(_SYS_SRST4_TYPE,x)

#define  SYS_SRST4_DEFAULT (_SYS_SRST4_TYPE)(  \
	 _CCL_PER_FDEFAULT(SYS,SRST4,GPIO)         \
	|_CCL_PER_FDEFAULT(SYS,SRST4,IRDA)         \
	|_CCL_PER_FDEFAULT(SYS,SRST4,I2C2)         \
	|_CCL_PER_FDEFAULT(SYS,SRST4,I2C1)         \
	|_CCL_PER_FDEFAULT(SYS,SRST4,I2C0)         \
	)

#define SYS_SRST4_RMK(gpio, irda, i2c2, i2c1, i2c0) (_SYS_SRST4_TYPE)( \
	 _CCL_PER_FMK  (SYS,SRST4,GPIO,gpio)     \
	|_CCL_PER_FMK  (SYS,SRST4,IRDA,irda)     \
	|_CCL_PER_FMK  (SYS,SRST4,I2C2,i2c2)     \
	|_CCL_PER_FMK  (SYS,SRST4,I2C1,i2c1)     \
	|_CCL_PER_FMK  (SYS,SRST4,I2C0,i2c0)     \
	)

#define SYS_SRST4_RMKS(gpio, irda, i2c2, i2c1, i2c0) (_SYS_SRST4_TYPE)( \
	 _CCL_PER_FMKS (SYS,SRST4,GPIO,gpio)     \
	|_CCL_PER_FMKS (SYS,SRST4,IRDA,irda)     \
	|_CCL_PER_FMKS (SYS,SRST4,I2C2,i2c2)     \
	|_CCL_PER_FMKS (SYS,SRST4,I2C1,i2c1)     \
	|_CCL_PER_FMKS (SYS,SRST4,I2C0,i2c0)     \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  C L K E N 0      |
 * |___________________|
 *
 * CLKEN0 - Clock Enable Register 0
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  LSADC -
 *                  LSADC  |   description
 *                ---------+-------------------
 *                 DISABLE | disable clock
 *                 ENABLE  | enable clock
 *
 * (rw)  TMR -
 *                   TMR   |   description
 *                ---------+-------------------
 *                 DISABLE | disable clock
 *                 ENABLE  | enable clock
 *
 * (rw)  I2CS -
 *                  I2CS   |   description
 *                ---------+-------------------
 *                 DISABLE | disable clock
 *                 ENABLE  | enable clock
 *
 * (rw)  DDC1 -
 *                  DDC1   |   description
 *                ---------+-------------------
 *                 DISABLE | disable clock
 *                 ENABLE  | enable clock
 *
 * (rw)  DDC0 -
 *                  DDC0   |   description
 *                ---------+-------------------
 *                 DISABLE | disable clock
 *                 ENABLE  | enable clock
 *
 * (rw)  HDMI -
 *                  HDMI   |   description
 *                ---------+-------------------
 *                 DISABLE | disable clock
 *                 ENABLE  | enable clock
 *
 * (rw)  VDEC -
 *                  VDEC   |   description
 *                ---------+-------------------
 *                 DISABLE | disable clock
 *                 ENABLE  | enable clock
 *
 * (rw)  VBI -
 *                   VBI   |   description
 *                ---------+-------------------
 *                 DISABLE | disable clock
 *                 ENABLE  | enable clock
\*-------------------------------------------------------------------------*/
#define _SYS_CLKEN0_OFFSET                   (0x00C0u)
#define _SYS_CLKEN0_ADDRH(h)                 SYS_ADDRH(h,CLKEN0)
#define _SYS_CLKEN0_LEN                      (8)                 /* 8-bit*/
#define _SYS_CLKEN0_TYPE                     uint8_t

#define _SYS_CLKEN0_LSADC_MASK               (0x80u)
#define _SYS_CLKEN0_LSADC_SHIFT              (7)
#define  SYS_CLKEN0_LSADC_DEFAULT            (0x00u)
#define  SYS_CLKEN0_LSADC_OF(x)              _CCL_VALUEOF(_SYS_CLKEN0_TYPE,x)
#define  SYS_CLKEN0_LSADC_DISABLE            (0x00u)
#define  SYS_CLKEN0_LSADC_ENABLE             (0x01u)

#define _SYS_CLKEN0_TMR_MASK                 (0x40u)
#define _SYS_CLKEN0_TMR_SHIFT                (6)
#define  SYS_CLKEN0_TMR_DEFAULT              (0x00u)
#define  SYS_CLKEN0_TMR_OF(x)                _CCL_VALUEOF(_SYS_CLKEN0_TYPE,x)
#define  SYS_CLKEN0_TMR_DISABLE              (0x00u)
#define  SYS_CLKEN0_TMR_ENABLE               (0x01u)

#define _SYS_CLKEN0_I2CS_MASK                (0x20u)
#define _SYS_CLKEN0_I2CS_SHIFT               (5)
#define  SYS_CLKEN0_I2CS_DEFAULT             (0x00u)
#define  SYS_CLKEN0_I2CS_OF(x)               _CCL_VALUEOF(_SYS_CLKEN0_TYPE,x)
#define  SYS_CLKEN0_I2CS_DISABLE             (0x00u)
#define  SYS_CLKEN0_I2CS_ENABLE              (0x01u)

#define _SYS_CLKEN0_DDC1_MASK                (0x10u)
#define _SYS_CLKEN0_DDC1_SHIFT               (4)
#define  SYS_CLKEN0_DDC1_DEFAULT             (0x00u)
#define  SYS_CLKEN0_DDC1_OF(x)               _CCL_VALUEOF(_SYS_CLKEN0_TYPE,x)
#define  SYS_CLKEN0_DDC1_DISABLE             (0x00u)
#define  SYS_CLKEN0_DDC1_ENABLE              (0x01u)

#define _SYS_CLKEN0_DDC0_MASK                (0x08u)
#define _SYS_CLKEN0_DDC0_SHIFT               (3)
#define  SYS_CLKEN0_DDC0_DEFAULT             (0x00u)
#define  SYS_CLKEN0_DDC0_OF(x)               _CCL_VALUEOF(_SYS_CLKEN0_TYPE,x)
#define  SYS_CLKEN0_DDC0_DISABLE             (0x00u)
#define  SYS_CLKEN0_DDC0_ENABLE              (0x01u)

#define _SYS_CLKEN0_HDMI_MASK                (0x04u)
#define _SYS_CLKEN0_HDMI_SHIFT               (2)
#define  SYS_CLKEN0_HDMI_DEFAULT             (0x00u)
#define  SYS_CLKEN0_HDMI_OF(x)               _CCL_VALUEOF(_SYS_CLKEN0_TYPE,x)
#define  SYS_CLKEN0_HDMI_DISABLE             (0x00u)
#define  SYS_CLKEN0_HDMI_ENABLE              (0x01u)

#define _SYS_CLKEN0_VDEC_MASK                (0x02u)
#define _SYS_CLKEN0_VDEC_SHIFT               (1)
#define  SYS_CLKEN0_VDEC_DEFAULT             (0x00u)
#define  SYS_CLKEN0_VDEC_OF(x)               _CCL_VALUEOF(_SYS_CLKEN0_TYPE,x)
#define  SYS_CLKEN0_VDEC_DISABLE             (0x00u)
#define  SYS_CLKEN0_VDEC_ENABLE              (0x01u)

#define _SYS_CLKEN0_VBI_MASK                 (0x01u)
#define _SYS_CLKEN0_VBI_SHIFT                (0)
#define  SYS_CLKEN0_VBI_DEFAULT              (0x00u)
#define  SYS_CLKEN0_VBI_OF(x)                _CCL_VALUEOF(_SYS_CLKEN0_TYPE,x)
#define  SYS_CLKEN0_VBI_DISABLE              (0x00u)
#define  SYS_CLKEN0_VBI_ENABLE               (0x01u)

#define  SYS_CLKEN0_OF(x)             _CCL_VALUEOF(_SYS_CLKEN0_TYPE,x)

#define  SYS_CLKEN0_DEFAULT (_SYS_CLKEN0_TYPE)(  \
	 _CCL_PER_FDEFAULT(SYS,CLKEN0,LSADC)        \
	|_CCL_PER_FDEFAULT(SYS,CLKEN0,TMR  )        \
	|_CCL_PER_FDEFAULT(SYS,CLKEN0,I2CS )        \
	|_CCL_PER_FDEFAULT(SYS,CLKEN0,DDC1 )        \
	|_CCL_PER_FDEFAULT(SYS,CLKEN0,DDC0 )        \
	|_CCL_PER_FDEFAULT(SYS,CLKEN0,HDMI )        \
	|_CCL_PER_FDEFAULT(SYS,CLKEN0,VDEC )        \
	|_CCL_PER_FDEFAULT(SYS,CLKEN0,VBI  )        \
	)

#define SYS_CLKEN0_RMK(lsadc, tmr, i2cs, ddc1, ddc0, hdmi, vdec, vbi) (_SYS_CLKEN0_TYPE)( \
	 _CCL_PER_FMK  (SYS,CLKEN0,LSADC,lsadc)     \
	|_CCL_PER_FMK  (SYS,CLKEN0,TMR  ,tmr  )     \
	|_CCL_PER_FMK  (SYS,CLKEN0,I2CS ,i2cs )     \
	|_CCL_PER_FMK  (SYS,CLKEN0,DDC1 ,ddc1 )     \
	|_CCL_PER_FMK  (SYS,CLKEN0,DDC0 ,ddc0 )     \
	|_CCL_PER_FMK  (SYS,CLKEN0,HDMI ,hdmi )     \
	|_CCL_PER_FMK  (SYS,CLKEN0,VDEC ,vdec )     \
	|_CCL_PER_FMK  (SYS,CLKEN0,VBI  ,vbi  )     \
	)

#define SYS_CLKEN0_RMKS(lsadc, tmr, i2cs, ddc1, ddc0, hdmi, vdec, vbi) (_SYS_CLKEN0_TYPE)( \
	 _CCL_PER_FMKS (SYS,CLKEN0,LSADC,lsadc)     \
	|_CCL_PER_FMKS (SYS,CLKEN0,TMR  ,tmr  )     \
	|_CCL_PER_FMKS (SYS,CLKEN0,I2CS ,i2cs )     \
	|_CCL_PER_FMKS (SYS,CLKEN0,DDC1 ,ddc1 )     \
	|_CCL_PER_FMKS (SYS,CLKEN0,DDC0 ,ddc0 )     \
	|_CCL_PER_FMKS (SYS,CLKEN0,HDMI ,hdmi )     \
	|_CCL_PER_FMKS (SYS,CLKEN0,VDEC ,vdec )     \
	|_CCL_PER_FMKS (SYS,CLKEN0,VBI  ,vbi  )     \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  C L K E N 1      |
 * |___________________|
 *
 * CLKEN1 - Clock Enable Register 1
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  I2CM1 -
 *                  I2CM1  |   description
 *                ---------+-------------------
 *                 DISABLE | disable clock
 *                 ENABLE  | enable clock
 *
 * (rw)  I2CM0 -
 *                  I2CM0  |   description
 *                ---------+-------------------
 *                 DISABLE | disable clock
 *                 ENABLE  | enable clock
 *
 * (rw)  SPI -
 *                   SPI   |   description
 *                ---------+-------------------
 *                 DISABLE | disable clock
 *                 ENABLE  | enable clock
 *
 * (rw)  DISPM - Display M-Domain
 *                  DISPM  |   description
 *                ---------+-------------------
 *                 DISABLE | disable clock
 *                 ENABLE  | enable clock
 *
 * (rw)  UART98M - UART 98 MHz
 *                 UART98M |   description
 *                ---------+-------------------
 *                 DISABLE | disable clock
 *                 ENABLE  | enable clock
 *
 * (rw)  IFD49M - IF Demodulator 49 MHz
 *                  IFD49M |   description
 *                ---------+-------------------
 *                 DISABLE | disable clock
 *                 ENABLE  | enable clock
 *
 * (rw)  AUD49M - Audio 49 MHz
 *                  AUD49M |   description
 *                ---------+-------------------
 *                 DISABLE | disable clock
 *                 ENABLE  | enable clock
 *
 * (rw)  AUD98M - Audio 98 MHz
 *                  AUD98M |   description
 *                ---------+-------------------
 *                 DISABLE | disable clock
 *                 ENABLE  | enable clock
\*-------------------------------------------------------------------------*/
#define _SYS_CLKEN1_OFFSET                   (0x00C1u)
#define _SYS_CLKEN1_ADDRH(h)                 SYS_ADDRH(h,CLKEN1)
#define _SYS_CLKEN1_LEN                      (8)                 /* 8-bit*/
#define _SYS_CLKEN1_TYPE                     uint8_t

#define _SYS_CLKEN1_I2CM1_MASK               (0x80u)
#define _SYS_CLKEN1_I2CM1_SHIFT              (7)
#define  SYS_CLKEN1_I2CM1_DEFAULT            (0x00u)
#define  SYS_CLKEN1_I2CM1_OF(x)              _CCL_VALUEOF(_SYS_CLKEN1_TYPE,x)
#define  SYS_CLKEN1_I2CM1_DISABLE            (0x00u)
#define  SYS_CLKEN1_I2CM1_ENABLE             (0x01u)

#define _SYS_CLKEN1_I2CM0_MASK               (0x40u)
#define _SYS_CLKEN1_I2CM0_SHIFT              (6)
#define  SYS_CLKEN1_I2CM0_DEFAULT            (0x00u)
#define  SYS_CLKEN1_I2CM0_OF(x)              _CCL_VALUEOF(_SYS_CLKEN1_TYPE,x)
#define  SYS_CLKEN1_I2CM0_DISABLE            (0x00u)
#define  SYS_CLKEN1_I2CM0_ENABLE             (0x01u)

#define _SYS_CLKEN1_SPI_MASK                 (0x20u)
#define _SYS_CLKEN1_SPI_SHIFT                (5)
#define  SYS_CLKEN1_SPI_DEFAULT              (0x00u)
#define  SYS_CLKEN1_SPI_OF(x)                _CCL_VALUEOF(_SYS_CLKEN1_TYPE,x)
#define  SYS_CLKEN1_SPI_DISABLE              (0x00u)
#define  SYS_CLKEN1_SPI_ENABLE               (0x01u)

#define _SYS_CLKEN1_DISPM_MASK               (0x10u)
#define _SYS_CLKEN1_DISPM_SHIFT              (4)
#define  SYS_CLKEN1_DISPM_DEFAULT            (0x00u)
#define  SYS_CLKEN1_DISPM_OF(x)              _CCL_VALUEOF(_SYS_CLKEN1_TYPE,x)
#define  SYS_CLKEN1_DISPM_DISABLE            (0x00u)
#define  SYS_CLKEN1_DISPM_ENABLE             (0x01u)

#define _SYS_CLKEN1_UART98M_MASK             (0x08u)
#define _SYS_CLKEN1_UART98M_SHIFT            (3)
#define  SYS_CLKEN1_UART98M_DEFAULT          (0x00u)
#define  SYS_CLKEN1_UART98M_OF(x)            _CCL_VALUEOF(_SYS_CLKEN1_TYPE,x)
#define  SYS_CLKEN1_UART98M_DISABLE          (0x00u)
#define  SYS_CLKEN1_UART98M_ENABLE           (0x01u)

#define _SYS_CLKEN1_IFD49M_MASK              (0x04u)
#define _SYS_CLKEN1_IFD49M_SHIFT             (2)
#define  SYS_CLKEN1_IFD49M_DEFAULT           (0x00u)
#define  SYS_CLKEN1_IFD49M_OF(x)             _CCL_VALUEOF(_SYS_CLKEN1_TYPE,x)
#define  SYS_CLKEN1_IFD49M_DISABLE           (0x00u)
#define  SYS_CLKEN1_IFD49M_ENABLE            (0x01u)

#define _SYS_CLKEN1_AUD49M_MASK              (0x02u)
#define _SYS_CLKEN1_AUD49M_SHIFT             (1)
#define  SYS_CLKEN1_AUD49M_DEFAULT           (0x00u)
#define  SYS_CLKEN1_AUD49M_OF(x)             _CCL_VALUEOF(_SYS_CLKEN1_TYPE,x)
#define  SYS_CLKEN1_AUD49M_DISABLE           (0x00u)
#define  SYS_CLKEN1_AUD49M_ENABLE            (0x01u)

#define _SYS_CLKEN1_AUD98M_MASK              (0x01u)
#define _SYS_CLKEN1_AUD98M_SHIFT             (0)
#define  SYS_CLKEN1_AUD98M_DEFAULT           (0x00u)
#define  SYS_CLKEN1_AUD98M_OF(x)             _CCL_VALUEOF(_SYS_CLKEN1_TYPE,x)
#define  SYS_CLKEN1_AUD98M_DISABLE           (0x00u)
#define  SYS_CLKEN1_AUD98M_ENABLE            (0x01u)

#define  SYS_CLKEN1_OF(x)             _CCL_VALUEOF(_SYS_CLKEN1_TYPE,x)

#define  SYS_CLKEN1_DEFAULT (_SYS_CLKEN1_TYPE)(   \
	 _CCL_PER_FDEFAULT(SYS,CLKEN1,I2CM1  )        \
	|_CCL_PER_FDEFAULT(SYS,CLKEN1,I2CM0  )        \
	|_CCL_PER_FDEFAULT(SYS,CLKEN1,SPI    )        \
	|_CCL_PER_FDEFAULT(SYS,CLKEN1,DISPM  )        \
	|_CCL_PER_FDEFAULT(SYS,CLKEN1,UART98M)        \
	|_CCL_PER_FDEFAULT(SYS,CLKEN1,IFD49M )        \
	|_CCL_PER_FDEFAULT(SYS,CLKEN1,AUD49M )        \
	|_CCL_PER_FDEFAULT(SYS,CLKEN1,AUD98M )        \
	)

#define SYS_CLKEN1_RMK(i2cm1, i2cm0, spi, dispm, uart98m, ifd49m, aud49m, aud98m) (_SYS_CLKEN1_TYPE)( \
	 _CCL_PER_FMK  (SYS,CLKEN1,I2CM1  ,i2cm1  )     \
	|_CCL_PER_FMK  (SYS,CLKEN1,I2CM0  ,i2cm0  )     \
	|_CCL_PER_FMK  (SYS,CLKEN1,SPI    ,spi    )     \
	|_CCL_PER_FMK  (SYS,CLKEN1,DISPM  ,dispm  )     \
	|_CCL_PER_FMK  (SYS,CLKEN1,UART98M,uart98m)     \
	|_CCL_PER_FMK  (SYS,CLKEN1,IFD49M ,ifd49m )     \
	|_CCL_PER_FMK  (SYS,CLKEN1,AUD49M ,aud49m )     \
	|_CCL_PER_FMK  (SYS,CLKEN1,AUD98M ,aud98m )     \
	)

#define SYS_CLKEN1_RMKS(i2cm1, i2cm0, spi, dispm, uart98m, ifd49m, aud49m, aud98m) (_SYS_CLKEN1_TYPE)( \
	 _CCL_PER_FMKS (SYS,CLKEN1,I2CM1  ,i2cm1  )     \
	|_CCL_PER_FMKS (SYS,CLKEN1,I2CM0  ,i2cm0  )     \
	|_CCL_PER_FMKS (SYS,CLKEN1,SPI    ,spi    )     \
	|_CCL_PER_FMKS (SYS,CLKEN1,DISPM  ,dispm  )     \
	|_CCL_PER_FMKS (SYS,CLKEN1,UART98M,uart98m)     \
	|_CCL_PER_FMKS (SYS,CLKEN1,IFD49M ,ifd49m )     \
	|_CCL_PER_FMKS (SYS,CLKEN1,AUD49M ,aud49m )     \
	|_CCL_PER_FMKS (SYS,CLKEN1,AUD98M ,aud98m )     \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  C L K E N 2      |
 * |___________________|
 *
 * CLKEN2 - Clock Enable Register 2
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  PWM_XTAL -
 *                 PWM_XTAL|   description
 *                ---------+-------------------
 *                 DISABLE | disable clock
 *                 ENABLE  | enable clock
 *
 * (rw)  IR_XTAL -
 *                  IR_XTAL|   description
 *                ---------+-------------------
 *                 DISABLE | disable clock
 *                 ENABLE  | enable clock
 *
 * (rw)  UR1_XTAL -
 *                 UR1_XTAL|   description
 *                ---------+-------------------
 *                 DISABLE | disable clock
 *                 ENABLE  | enable clock
 *
 * (rw)  UR0_XTAL - Display M-Domain
 *                 UR0_XTAL|   description
 *                ---------+-------------------
 *                 DISABLE | disable clock
 *                 ENABLE  | enable clock
 *
 * (rw)  AUD16M - UART 98 MHz
 *                 AUD16M  |   description
 *                ---------+-------------------
 *                 DISABLE | disable clock
 *                 ENABLE  | enable clock
 *
 * (rw)  AUD24M - IF Demodulator 49 MHz
 *                  AUD24M |   description
 *                ---------+-------------------
 *                 DISABLE | disable clock
 *                 ENABLE  | enable clock
 *
 * (rw)  DISPD - D-domain
 *                  DISPD  |   description
 *                ---------+-------------------
 *                 DISABLE | disable clock
 *                 ENABLE  | enable clock
 *
 * (rw)  DISPI - I-domain
 *                  DISPI  |   description
 *                ---------+-------------------
 *                 DISABLE | disable clock
 *                 ENABLE  | enable clock
\*-------------------------------------------------------------------------*/
#define _SYS_CLKEN2_OFFSET                   (0x00C2u)
#define _SYS_CLKEN2_ADDRH(h)                 SYS_ADDRH(h,CLKEN2)
#define _SYS_CLKEN2_LEN                      (8)                 /* 8-bit*/
#define _SYS_CLKEN2_TYPE                     uint8_t

#define _SYS_CLKEN2_PWM_XTAL_MASK            (0x80u)
#define _SYS_CLKEN2_PWM_XTAL_SHIFT           (7)
#define  SYS_CLKEN2_PWM_XTAL_DEFAULT         (0x00u)
#define  SYS_CLKEN2_PWM_XTAL_OF(x)           _CCL_VALUEOF(_SYS_CLKEN2_TYPE,x)
#define  SYS_CLKEN2_PWM_XTAL_DISABLE         (0x00u)
#define  SYS_CLKEN2_PWM_XTAL_ENABLE          (0x01u)

#define _SYS_CLKEN2_IR_XTAL_MASK             (0x40u)
#define _SYS_CLKEN2_IR_XTAL_SHIFT            (6)
#define  SYS_CLKEN2_IR_XTAL_DEFAULT          (0x00u)
#define  SYS_CLKEN2_IR_XTAL_OF(x)            _CCL_VALUEOF(_SYS_CLKEN2_TYPE,x)
#define  SYS_CLKEN2_IR_XTAL_DISABLE          (0x00u)
#define  SYS_CLKEN2_IR_XTAL_ENABLE           (0x01u)

#define _SYS_CLKEN2_UR1_XTAL_MASK            (0x20u)
#define _SYS_CLKEN2_UR1_XTAL_SHIFT           (5)
#define  SYS_CLKEN2_UR1_XTAL_DEFAULT         (0x00u)
#define  SYS_CLKEN2_UR1_XTAL_OF(x)           _CCL_VALUEOF(_SYS_CLKEN2_TYPE,x)
#define  SYS_CLKEN2_UR1_XTAL_DISABLE         (0x00u)
#define  SYS_CLKEN2_UR1_XTAL_ENABLE          (0x01u)

#define _SYS_CLKEN2_UR0_XTAL_MASK            (0x10u)
#define _SYS_CLKEN2_UR0_XTAL_SHIFT           (4)
#define  SYS_CLKEN2_UR0_XTAL_DEFAULT         (0x00u)
#define  SYS_CLKEN2_UR0_XTAL_OF(x)           _CCL_VALUEOF(_SYS_CLKEN2_TYPE,x)
#define  SYS_CLKEN2_UR0_XTAL_DISABLE         (0x00u)
#define  SYS_CLKEN2_UR0_XTAL_ENABLE          (0x01u)

#define _SYS_CLKEN2_AUD16M_MASK              (0x08u)
#define _SYS_CLKEN2_AUD16M_SHIFT             (3)
#define  SYS_CLKEN2_AUD16M_DEFAULT           (0x00u)
#define  SYS_CLKEN2_AUD16M_OF(x)             _CCL_VALUEOF(_SYS_CLKEN2_TYPE,x)
#define  SYS_CLKEN2_AUD16M_DISABLE           (0x00u)
#define  SYS_CLKEN2_AUD16M_ENABLE            (0x01u)

#define _SYS_CLKEN2_AUD24M_MASK              (0x04u)
#define _SYS_CLKEN2_AUD24M_SHIFT             (2)
#define  SYS_CLKEN2_AUD24M_DEFAULT           (0x00u)
#define  SYS_CLKEN2_AUD24M_OF(x)             _CCL_VALUEOF(_SYS_CLKEN2_TYPE,x)
#define  SYS_CLKEN2_AUD24M_DISABLE           (0x00u)
#define  SYS_CLKEN2_AUD24M_ENABLE            (0x01u)

#define _SYS_CLKEN2_DISPD_MASK               (0x02u)
#define _SYS_CLKEN2_DISPD_SHIFT              (1)
#define  SYS_CLKEN2_DISPD_DEFAULT            (0x00u)
#define  SYS_CLKEN2_DISPD_OF(x)              _CCL_VALUEOF(_SYS_CLKEN2_TYPE,x)
#define  SYS_CLKEN2_DISPD_DISABLE            (0x00u)
#define  SYS_CLKEN2_DISPD_ENABLE             (0x01u)

#define _SYS_CLKEN2_DISPI_MASK               (0x01u)
#define _SYS_CLKEN2_DISPI_SHIFT              (0)
#define  SYS_CLKEN2_DISPI_DEFAULT            (0x00u)
#define  SYS_CLKEN2_DISPI_OF(x)              _CCL_VALUEOF(_SYS_CLKEN2_TYPE,x)
#define  SYS_CLKEN2_DISPI_DISABLE            (0x00u)
#define  SYS_CLKEN2_DISPI_ENABLE             (0x01u)

#define  SYS_CLKEN2_OF(x)             _CCL_VALUEOF(_SYS_CLKEN2_TYPE,x)

#define  SYS_CLKEN2_DEFAULT (_SYS_CLKEN2_TYPE)(     \
	 _CCL_PER_FDEFAULT(SYS,CLKEN2,PWM_XTAL )        \
	|_CCL_PER_FDEFAULT(SYS,CLKEN2,IR_XTAL  )        \
	|_CCL_PER_FDEFAULT(SYS,CLKEN2,UR1_XTAL )        \
	|_CCL_PER_FDEFAULT(SYS,CLKEN2,UR0_XTAL )        \
	|_CCL_PER_FDEFAULT(SYS,CLKEN2,AUD16M   )        \
	|_CCL_PER_FDEFAULT(SYS,CLKEN2,AUD24M   )        \
	|_CCL_PER_FDEFAULT(SYS,CLKEN2,DISPD    )        \
	|_CCL_PER_FDEFAULT(SYS,CLKEN2,DISPI    )        \
	)

#define SYS_CLKEN2_RMK(pwm_xtal, ir_xtal, ur1_xtal, ur0_xtal, aud16m, aud24m, dispd, dispi) (_SYS_CLKEN2_TYPE)( \
	 _CCL_PER_FMK  (SYS,CLKEN2,PWM_XTAL,pwm_xtal  )     \
	|_CCL_PER_FMK  (SYS,CLKEN2,IR_XTAL ,ir_xtal   )     \
	|_CCL_PER_FMK  (SYS,CLKEN2,UR1_XTAL,ur1_xtal  )     \
	|_CCL_PER_FMK  (SYS,CLKEN2,UR0_XTAL,ur0_xtal  )     \
	|_CCL_PER_FMK  (SYS,CLKEN2,AUD16M  ,aud16m    )     \
	|_CCL_PER_FMK  (SYS,CLKEN2,AUD24M  ,aud24m    )     \
	|_CCL_PER_FMK  (SYS,CLKEN2,DISPD   ,dispd     )     \
	|_CCL_PER_FMK  (SYS,CLKEN2,DISPI   ,dispi     )     \
	)

#define SYS_CLKEN2_RMKS(pwm_xtal, ir_xtal, ur1_xtal, ur0_xtal, aud16m, aud24m, dispd, dispi) (_SYS_CLKEN2_TYPE)( \
	 _CCL_PER_FMKS (SYS,CLKEN2,PWM_XTAL,pwm_xtal  )     \
	|_CCL_PER_FMKS (SYS,CLKEN2,IR_XTAL ,ir_xtal   )     \
	|_CCL_PER_FMKS (SYS,CLKEN2,UR1_XTAL,ur1_xtal  )     \
	|_CCL_PER_FMKS (SYS,CLKEN2,UR0_XTAL,ur0_xtal  )     \
	|_CCL_PER_FMKS (SYS,CLKEN2,AUD16M  ,aud16m    )     \
	|_CCL_PER_FMKS (SYS,CLKEN2,AUD24M  ,aud24m    )     \
	|_CCL_PER_FMKS (SYS,CLKEN2,DISPD   ,dispd     )     \
	|_CCL_PER_FMKS (SYS,CLKEN2,DISPI   ,dispi     )     \
	)




/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  S H M            |
 * |___________________|
 *
 * SHM - Share Memory
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  CK_SEL  - Clock Selection
 *                  CK_SEL |   description
 *                ---------+-------------------
 *                    0    |   TT, Display TT (TT Page 13KB, VBIOSD 8.32KB, I-MEM 12KB)
 *                    1    |   TT, Display UI OSD (TT Page 13KB, UI OSD 20KB)
 *                    2    |   TT, Display TT + simple OSD (TT Page 13KB, VBIOSD 8.32KB,  UI OSD 12KB)
 *
 * (rw)  MODE  -
 *                  MODE   |   description
 *                ---------+-------------------
 *                    0    |   TT, Display TT (TT Page 13KB, VBIOSD 8.32KB, I-MEM 12KB)
 *                    1    |   TT, Display UI OSD (TT Page 13KB, UI OSD 20KB)
 *                    2    |   TT, Display TT + simple OSD (TT Page 13KB, VBIOSD 8.32KB,  UI OSD 12KB)
\*-------------------------------------------------------------------------*/
#define _SYS_SHM_OFFSET                   (0x01A1u)
#define _SYS_SHM_ADDRH(h)                 SYS_ADDRH(h,SHM)
#define _SYS_SHM_LEN                      (8)                 /* 8-bit*/
#define _SYS_SHM_TYPE                     uint8_t

#define _SYS_SHM_CK_SEL_MASK              (0x0Cu)
#define _SYS_SHM_CK_SEL_SHIFT             (2)
#define  SYS_SHM_CK_SEL_DEFAULT           (0x00u)
#define  SYS_SHM_CK_SEL_OF(x)             _CCL_VALUEOF(_SYS_SHM_TYPE,x)
#define  SYS_SHM_CK_SEL_0                 (0x00u)
#define  SYS_SHM_CK_SEL_1                 (0x01u)
#define  SYS_SHM_CK_SEL_2                 (0x02u)

#define _SYS_SHM_MODE_MASK                (0x03u)
#define _SYS_SHM_MODE_SHIFT               (0)
#define  SYS_SHM_MODE_DEFAULT             (0x00u)
#define  SYS_SHM_MODE_OF(x)               _CCL_VALUEOF(_SYS_SHM_TYPE,x)
#define  SYS_SHM_MODE_0                   (0x00u)
#define  SYS_SHM_MODE_1                   (0x01u)
#define  SYS_SHM_MODE_2                   (0x02u)

#define  SYS_SHM_OF(x)             _CCL_VALUEOF(_SYS_SHM_TYPE,x)

#define  SYS_SHM_DEFAULT (_SYS_SHM_TYPE)(  \
	 _CCL_PER_FDEFAULT(SYS,SHM,CK_SEL)     \
	|_CCL_PER_FDEFAULT(SYS,SHM,MODE)       \
	)

#define SYS_SHM_RMK(ck_sel, mode) (_SYS_SHM_TYPE)( \
	 _CCL_PER_FMK  (SYS,SHM,CK_SEL,ck_sel)         \
	|_CCL_PER_FMK  (SYS,SHM,MODE  ,mode  )         \
	)

#define SYS_SHM_RMKS(ck_sel, mode) (_SYS_SHM_TYPE)( \
	 _CCL_PER_FMKS (SYS,SHM,CK_SEL,ck_sel)          \
	|_CCL_PER_FMKS (SYS,SHM,MODE  ,mode  )          \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  N S D M A        |
 * |___________________|
 *
 * NSDMA - Non-Stop SPI-DMA
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  ENABLE  -
 *                  ENABLE |   description
 *                ---------+---------------------------------------------------
 *                  DISABLE|   we always gated CPU clk, while DMA is triggered
 *                  ENABLE |   while CPU process IMEM code, then we could do DMA without stop CPU
\*-------------------------------------------------------------------------*/
#define _SYS_NSDMA_OFFSET                   (0x01A1u)
#define _SYS_NSDMA_ADDRH(h)                 SYS_ADDRH(h,NSDMA)
#define _SYS_NSDMA_LEN                      (8)                 /* 8-bit*/
#define _SYS_NSDMA_TYPE                     uint8_t

#define _SYS_NSDMA_ENABLE_MASK              (0x01u)
#define _SYS_NSDMA_ENABLE_SHIFT             (0)
#define  SYS_NSDMA_ENABLE_DEFAULT           (0x00u)
#define  SYS_NSDMA_ENABLE_OF(x)             _CCL_VALUEOF(_SYS_NSDMA_TYPE,x)
#define  SYS_NSDMA_ENABLE_DISABLE           (0x00u)
#define  SYS_NSDMA_ENABLE_ENABLE            (0x01u)

#define  SYS_NSDMA_OF(x)             _CCL_VALUEOF(_SYS_NSDMA_TYPE,x)

#define  SYS_NSDMA_DEFAULT (_SYS_NSDMA_TYPE)(  \
	 _CCL_PER_FDEFAULT(SYS,NSDMA,ENABLE)       \
	)

#define SYS_NSDMA_RMK(enable) (_SYS_NSDMA_TYPE)( \
	 _CCL_PER_FMK  (SYS,NSDMA,ENABLE,enable)     \
	)

#define SYS_NSDMA_RMKS(enable) (_SYS_NSDMA_TYPE)( \
	 _CCL_PER_FMKS (SYS,NSDMA,ENABLE,enable)      \
	)

#endif	/* !__C_CHIP_LIB_REALTEK_RTD_2668_SYSTEM_HARDWARE_ABSTRACT_LAYER_HEADER_INCLUDED__ */


/*-------------------------------------------------------------------------*\
 * end of sys_hal.h                                                        *
\*-------------------------------------------------------------------------*/

