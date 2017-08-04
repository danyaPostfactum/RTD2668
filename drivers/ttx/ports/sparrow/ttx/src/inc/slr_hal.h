/*-------------------------------------------------------------------------*\
 *               File: slr_hal.h
 *                Ver: 0.1
 *             Author: Chi-Chun Cheng
 *          Objective: Provides the RTD-2668 VBI slicer
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
 *   SLR_ADDR   (<REG>)                  -   Register address
 *   SLR_RGET   (<REG>)                  -   Returns the value in the peripheral register
 *   SLR_RSET   (<REG>,x)                -   Register set
 *   SLR_FGET   (<REG>,<FIELD>)          -   Returns the value of the specified field
 *                                             in the peripheral register
 *   SLR_FSET   (<REG>,<FIELD>,fieldval) -   Writes fieldval to the specified field
 *                                             in the peripheral register
 *   SLR_FSETS  (<REG>,<FIELD>,<SYM>)    -   Writes the symbol value to the specified
 *                                             field in the peripheral
 *   SLR_RGETA  (addr,<REG>)             -   Gets register for a given address
 *   SLR_RSETA  (addr,<REG>,x)           -   Sets register for a given address
 *
 *  <NOTE>
 *  Not completed yet!!!!!!!!!!
\*-------------------------------------------------------------------------*/

#ifndef	__C_CHIP_LIB_REALTEK_SPARROW_VBI_SLICER_HARDWARE_ABSTRACT_LAYER_HEADER_INCLUDED__
#define	__C_CHIP_LIB_REALTEK_SPARROW_VBI_SLICER_HARDWARE_ABSTRACT_LAYER_HEADER_INCLUDED__ (1)


/* CCL headers ------------------------------------------------------------*/
#include <ccl/std.h>    /* the standard header of C Chip Library */

/*
 *  Base address of SLR
 */
#define _SLR_ADDR_BASE  (0x5800u)

/*
 *  The function used for read register from this IP. Prototype is
 *     uint32_t _SLR_REG_READ( addr );
 */
#define _SLR_REG_READ(addr)         TTX_Reg32Read(addr)

/*
 *  The function used for read register from this IP. Prototype is
 *     void    _SLR_REG_WRITE( addr, val );
 */
#define _SLR_REG_WRITE(addr,val)    TTX_Reg32Write(addr,val)

/*
 * The shift left of data-bus connection in bits, e.g.,
 *   conventionally, the D0-D7 of SLR are connected to CPU data-bus D0-D7 (_SLR_DB_SHFT = 0).
 *   However, in some cases,
 *     the D0-D7 of 82C52 are connected to CPU data-bus D16-D24 (_SLR_DB_SHFT = 16).
 */
#define _SLR_DB_SHFT     (0)

/*
 * The width of data-bus connection in bits, e.g.,
 *   conventionally, the D0-D7 of 82C52 are connected to CPU data-bus D0-D7 (_SLR_DB_WIDTH = 0x00FFu).
 *   Please note, despite the shift of data-bus, _SLR_DB_WITH starts from LSB.
 *   As a consequence,
 *     if the D0-D7 of 82C52 are connected to CPU data-bus D16-D24 (_SLR_DB_WIDTH = 0x00FFu).
 */
#define _SLR_DB_WIDTH   (0xFFFFFFFFu)

/*
 * The shift left of address-bus connection in bits, e.g.,
 *   conventionally, the A0-A7 of 82C52 are connected to CPU address-bus A0-A7 (_SLR_AB_SHFT = Uint8).
 *   However, in some cases,
 *     the A0-A7 of 82C52 are connected to CPU data-bus A2-A9 (_SLR_DB_SHFT = uint32_t).
 */
#define _SLR_AB_SHFT     uint16_t




/* Field related macros */

#define SLR_FMK(REG,FIELD,x)\
	_CCL_PER_FMK(SLR,REG,FIELD,x)

#define SLR_FMKS(REG,FIELD,SYM)\
	_CCL_PER_FMKS(SLR,REG,FIELD,SYM)

#define SLR_FEXTRACT(REG,FIELD,x)\
	_CCL_PER_FEXTRACT(SLR,REG,FIELD,x)


/* Raw register/field access macros */

#define SLR_ADDR(REG)\
    (_SLR_ADDR_BASE+_SLR_##REG##_OFFSET)

#define SLR_RGET(REG)\
    _SLR_RGETA(SLR_ADDR(REG),REG)

#define SLR_RSET(REG,x)\
    _SLR_RSETA(SLR_ADDR(REG),REG,x)

#define SLR_FGET(REG,FIELD)\
    _SLR_FGETA(SLR_ADDR(REG),REG,FIELD)

#define SLR_FSET(REG,FIELD,x)\
    _SLR_FSETA(SLR_ADDR(REG),REG,FIELD,x)

#define SLR_FSETS(REG,FIELD,SYM)\
    _SLR_FSETSA(SLR_ADDR(REG),REG,FIELD,SYM)


/* Address based register/field access macros */

#define _SLR_RGETA(addr,REG)\
	_CCL_PER_RGET(addr,SLR,REG,_SLR_DB_SHFT,_SLR_DB_WIDTH)

#define _SLR_RSETA(addr,REG,x)\
	_CCL_PER_RSET(addr,SLR,REG,x,_SLR_DB_SHFT,_SLR_DB_WIDTH)

#define _SLR_RSETSA(addr,REG,SYM)\
	_CCL_PER_RSETS(addr,SLR,REG,SYM,_SLR_DB_SHFT,_SLR_DB_WIDTH)

#define _SLR_FGETA(addr,REG,FIELD)\
	_CCL_PER_FGET(addr,SLR,REG,FIELD,_SLR_DB_SHFT,_SLR_DB_WIDTH)

#define _SLR_FSETA(addr,REG,FIELD,x)\
	_CCL_PER_FSET(addr,SLR,REG,FIELD,x,_SLR_DB_SHFT,_SLR_DB_WIDTH)

#define _SLR_FSETSA(addr,REG,FIELD,SYM)\
	_CCL_PER_FSETS(addr,SLR,REG,FIELD,SYM,_SLR_DB_SHFT,_SLR_DB_WIDTH)


/* Handle based register/field access macros */

/* This is for base-address determined by handle h.
#define SLR_ADDRH(h,REG)\
	(_SLR_AB_SHFT)(&((h)->addrBase[_SLR_##REG##_OFFSET]))
*/
#define SLR_ADDRH(h,REG)\
	(_SLR_AB_SHFT)(_SLR_ADDR_BASE+_SLR_##REG##_OFFSET)

#define SLR_RGETH(h,REG)\
	_SLR_RGETA(SLR_ADDRH(h,REG),REG)

#define SLR_RSETH(h,REG,x)\
	_SLR_RSETA(SLR_ADDRH(h,REG),REG,x)

#define SLR_RSETSH(h,REG,SYM)\
	_SLR_RSETSA(SLR_ADDRH(h,REG),REG,SYM)

#define SLR_FGETH(h,REG,FIELD)\
	_SLR_FGETA(SLR_ADDRH(h,REG),REG,FIELD)

#define SLR_FSETH(h,REG,FIELD,x)\
	_SLR_FSETA(SLR_ADDRH(h,REG),REG,FIELD,x)

#define SLR_FSETSH(h,REG,FIELD,SYM)\
	_SLR_FSETSA(SLR_ADDRH(h,REG),REG,FIELD,SYM)



/* Function Prototypes ----------------------------------------------------*/

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  C T R L 0        |
 * |___________________|
 *
 * CTRL0 - VBI Slicer Control Register 0
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (r)   DMA - DMA status
 *                 DMASTOP |   description
 *                ---------+-------------------------------------------
 *                 RUNNING |   error not occur yet
 *                 STOPPED |   error occur, dma stop capturing data
 *
 * (rw)  DSE (DMA Stop Enable) -
 *                   DSE   |   description
 *                ---------+-------------------
 *                 DISABLE |   disable
 *                 ENABLE  |   enable
 *
 * (rw)  SRC - ADC source selection
 *                   SRC   |   description
 *                ---------+-------------------
 *                    VD   |   Video Decoder
 *                  YPBPR  |   YPbPr
 *
 * (rw)  SYS - Video System Selection
 *                   SYS   |   description
 *                ---------+-------------------
 *                   NTSC  | NTSC for CC & CGMS Detection
 *                   PAL   | PAL/SECAM for WSS Detection
 *
 * (rw)  FLDIND - Inverse Field Indication or not
 *                  FLDIND |   description
 *                ---------+-------------------
 *                  NORMAL |
 *                 INVERSE | Inverse field indication
 *
 * (rw)  FLDLNCT - Inverse the field signal for V/H Line counter in YpbPr Mode.
 *                  FLDIND |   description
 *                ---------+-------------------
 *                  NORMAL |
 *                 INVERSE | Inverse field indication
\*-------------------------------------------------------------------------*/
#define _SLR_CTRL0_OFFSET                   (0x0000u)
#define _SLR_CTRL0_ADDRH(h)                 SLR_ADDRH(h,CTRL0)
#define _SLR_CTRL0_LEN                      (32)                 /* 32-bit*/
#define _SLR_CTRL0_TYPE                     uint32_t

#define _SLR_CTRL0_DMA_MASK                 (0x08000000u)
#define _SLR_CTRL0_DMA_SHIFT                (27)
#define  SLR_CTRL0_DMA_DEFAULT              (0x00u)
#define  SLR_CTRL0_DMA_OF(x)                _CCL_VALUEOF(_SLR_CTRL0_TYPE,x)
#define  SLR_CTRL0_DMA_RUNNING              (0x00u)
#define  SLR_CTRL0_DMA_STOPPED              (0x01u)

#define _SLR_CTRL0_DSE_MASK                 (0x04000000u)
#define _SLR_CTRL0_DSE_SHIFT                (26)
#define  SLR_CTRL0_DSE_DEFAULT              (0x00u)
#define  SLR_CTRL0_DSE_OF(x)                _CCL_VALUEOF(_SLR_CTRL0_TYPE,x)
#define  SLR_CTRL0_DSE_DISABLE              (0x00u)
#define  SLR_CTRL0_DSE_ENABLE               (0x01u)

#define _SLR_CTRL0_SRC_MASK                 (0x00000080u)
#define _SLR_CTRL0_SRC_SHIFT                (7)
#define  SLR_CTRL0_SRC_DEFAULT              (0x00u)
#define  SLR_CTRL0_SRC_OF(x)                _CCL_VALUEOF(_SLR_CTRL0_TYPE,x)
#define  SLR_CTRL0_SRC_VD                   (0x00u)
#define  SLR_CTRL0_SRC_YPBPR                (0x01u)

#define _SLR_CTRL0_SYS_MASK                 (0x00000040u)
#define _SLR_CTRL0_SYS_SHIFT                (6)
#define  SLR_CTRL0_SYS_DEFAULT              (0x00u)
#define  SLR_CTRL0_SYS_OF(x)                _CCL_VALUEOF(_SLR_CTRL0_TYPE,x)
#define  SLR_CTRL0_SYS_NTSC                 (0x00u)
#define  SLR_CTRL0_SYS_PAL                  (0x01u)

#define _SLR_CTRL0_FLDIND_MASK              (0x00000020u)
#define _SLR_CTRL0_FLDIND_SHIFT             (5)
#define  SLR_CTRL0_FLDIND_DEFAULT           (0x00u)
#define  SLR_CTRL0_FLDIND_OF(x)             _CCL_VALUEOF(_SLR_CTRL0_TYPE,x)
#define  SLR_CTRL0_FLDIND_NORMAL            (0x00u)
#define  SLR_CTRL0_FLDIND_INVERSE           (0x01u)

#define _SLR_CTRL0_FLDLNCNT_MASK            (0x00000010u)
#define _SLR_CTRL0_FLDLNCNT_SHIFT           (4)
#define  SLR_CTRL0_FLDLNCNT_DEFAULT         (0x00u)
#define  SLR_CTRL0_FLDLNCNT_OF(x)           _CCL_VALUEOF(_SLR_CTRL0_TYPE,x)
#define  SLR_CTRL0_FLDLNCNT_NORMAL          (0x00u)
#define  SLR_CTRL0_FLDLNCNT_INVERSE         (0x01u)

#define  SLR_CTRL0_OF(x)             _CCL_VALUEOF(_SLR_CTRL0_TYPE,x)

#define  SLR_CTRL0_DEFAULT (_SLR_CTRL0_TYPE)(   \
	 _CCL_PER_FDEFAULT(SLR,CTRL0,DMA     )      \
	|_CCL_PER_FDEFAULT(SLR,CTRL0,DSE     )      \
	|_CCL_PER_FDEFAULT(SLR,CTRL0,SRC     )      \
	|_CCL_PER_FDEFAULT(SLR,CTRL0,SYS     )      \
	|_CCL_PER_FDEFAULT(SLR,CTRL0,FLDIND  )      \
	|_CCL_PER_FDEFAULT(SLR,CTRL0,FLDLNCNT)      \
	)

#define SLR_CTRL0_RMK(dma, dse, src, sys, fldind, fldlncnt) (_SLR_CTRL0_TYPE)( \
	 _CCL_PER_FMK  (SLR,CTRL0,DMA     ,dma     )     \
	|_CCL_PER_FMK  (SLR,CTRL0,DSE     ,dse     )     \
	|_CCL_PER_FMK  (SLR,CTRL0,SRC     ,src     )     \
	|_CCL_PER_FMK  (SLR,CTRL0,SYS     ,sys     )     \
	|_CCL_PER_FMK  (SLR,CTRL0,FLDIND  ,fldind  )     \
	|_CCL_PER_FMK  (SLR,CTRL0,FLDLNCNT,fldlncnt)     \
	)

#define SLR_CTRL0_RMKS(dma, dse, src, sys, fldind, fldlncnt) (_SLR_CTRL0_TYPE)( \
	 _CCL_PER_FMKS (SLR,CTRL0,DMA     ,dma     )     \
	|_CCL_PER_FMKS (SLR,CTRL0,DSE     ,dse     )     \
	|_CCL_PER_FMKS (SLR,CTRL0,SRC     ,src     )     \
	|_CCL_PER_FMKS (SLR,CTRL0,SYS     ,sys     )     \
	|_CCL_PER_FMKS (SLR,CTRL0,FLDIND  ,fldind  )     \
	|_CCL_PER_FMKS (SLR,CTRL0,FLDLNCNT,fldlncnt)     \
	)



#endif	/* !__C_CHIP_LIB_REALTEK_SPARROW_VBI_SLICER_HARDWARE_ABSTRACT_LAYER_HEADER_INCLUDED__ */


/*-------------------------------------------------------------------------*\
 * end of slr_hal.h                                                        *
\*-------------------------------------------------------------------------*/

