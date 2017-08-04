/*-------------------------------------------------------------------------*\
 *               File: ppr_hal.h
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
 *   PPR_ADDR   (<REG>)                  -   Register address
 *   PPR_RGET   (<REG>)                  -   Returns the value in the peripheral register
 *   PPR_RSET   (<REG>,x)                -   Register set
 *   PPR_FGET   (<REG>,<FIELD>)          -   Returns the value of the specified field
 *                                             in the peripheral register
 *   PPR_FSET   (<REG>,<FIELD>,fieldval) -   Writes fieldval to the specified field
 *                                             in the peripheral register
 *   PPR_FSETS  (<REG>,<FIELD>,<SYM>)    -   Writes the symbol value to the specified
 *                                             field in the peripheral
 *   PPR_RGETA  (addr,<REG>)             -   Gets register for a given address
 *   PPR_RSETA  (addr,<REG>,x)           -   Sets register for a given address
 *
 *  <NOTE>
 *  Not completed yet!!!!!!!!!!
\*-------------------------------------------------------------------------*/

#ifndef	__C_CHIP_LIB_REALTEK_SPARROW_VERTICAL_BLANKING_INTERVAL_PRE_PROCESSOR_HARDWARE_ABSTRACT_LAYER_HEADER_INCLUDED__
#define	__C_CHIP_LIB_REALTEK_SPARROW_VERTICAL_BLANKING_INTERVAL_PRE_PROCESSOR_HARDWARE_ABSTRACT_LAYER_HEADER_INCLUDED__ (1)


/* CCL headers ------------------------------------------------------------*/
#include <ccl/std.h>    /* the standard header of C Chip Library */
//#include <sparrow.h>	/* Provides: rtd_isr_inl() and rtd_isr_outl() */

/*
 *  Base address of PPR
 */
#define _PPR_ADDR_BASE  (0x4000u)

/*
 *  The function used for read register from this IP. Prototype is
 *     uint32_t _PPR_REG_READ( addr );
 */
#define _PPR_REG_READ(addr)         TTX_Reg32Read(addr)
//#define _PPR_REG_READ(addr)         rtd_isr_inl(addr)

/*
 *  The function used for read register from this IP. Prototype is
 *     void    _PPR_REG_WRITE( addr, val );
 */
#define _PPR_REG_WRITE(addr,val)    TTX_Reg32Write(addr,val)
//#define _PPR_REG_WRITE(addr,val)    rtd_isr_outl(addr,val)


/*
 * The shift left of data-bus connection in bits, e.g.,
 *   conventionally, the D0-D7 of PPR are connected to CPU data-bus D0-D7 (_PPR_DB_SHFT = 0).
 *   However, in some cases,
 *     the D0-D7 of 82C52 are connected to CPU data-bus D16-D24 (_PPR_DB_SHFT = 16).
 */
#define _PPR_DB_SHFT     (0)

/*
 * The width of data-bus connection in bits, e.g.,
 *   conventionally, the D0-D7 of 82C52 are connected to CPU data-bus D0-D7 (_PPR_DB_WIDTH = 0x00FFu).
 *   Please note, despite the shift of data-bus, _PPR_DB_WITH starts from LSB.
 *   As a consequence,
 *     if the D0-D7 of 82C52 are connected to CPU data-bus D16-D24 (_PPR_DB_WIDTH = 0x00FFu).
 */
#define _PPR_DB_WIDTH   (0xFFFFFFFFu)

/*
 * The shift left of address-bus connection in bits, e.g.,
 *   conventionally, the A0-A7 of 82C52 are connected to CPU address-bus A0-A7 (_PPR_AB_SHFT = Uint8).
 *   However, in some cases,
 *     the A0-A7 of 82C52 are connected to CPU data-bus A2-A9 (_PPR_DB_SHFT = uint32_t).
 */
#define _PPR_AB_SHFT     uint16_t



/* Field related macros */

#define PPR_FMK(REG,FIELD,x)\
	_CCL_PER_FMK(PPR,REG,FIELD,x)

#define PPR_FMKS(REG,FIELD,SYM)\
	_CCL_PER_FMKS(PPR,REG,FIELD,SYM)

#define PPR_FEXTRACT(REG,FIELD,x)\
	_CCL_PER_FEXTRACT(PPR,REG,FIELD,x)



/* Raw register/field access macros */

#define PPR_ADDR(REG)\
    (_PPR_ADDR_BASE+_PPR_##REG##_OFFSET)

#define PPR_RGET(REG)\
    _PPR_RGETA(PPR_ADDR(REG),REG)

#define PPR_RSET(REG,x)\
    _PPR_RSETA(PPR_ADDR(REG),REG,x)

#define PPR_FGET(REG,FIELD)\
    _PPR_FGETA(PPR_ADDR(REG),REG,FIELD)

#define PPR_FSET(REG,FIELD,x)\
    _PPR_FSETA(PPR_ADDR(REG),REG,FIELD,x)

#define PPR_FSETS(REG,FIELD,SYM)\
    _PPR_FSETSA(PPR_ADDR(REG),REG,FIELD,SYM)


/* Address based register/field access macros */

#define _PPR_RGETA(addr,REG)\
	_CCL_PER_RGET(addr,PPR,REG,_PPR_DB_SHFT,_PPR_DB_WIDTH)

#define _PPR_RSETA(addr,REG,x)\
	_CCL_PER_RSET(addr,PPR,REG,x,_PPR_DB_SHFT,_PPR_DB_WIDTH)

#define _PPR_RSETSA(addr,REG,SYM)\
	_CCL_PER_RSETS(addr,PPR,REG,SYM,_PPR_DB_SHFT,_PPR_DB_WIDTH)

#define _PPR_FGETA(addr,REG,FIELD)\
	_CCL_PER_FGET(addr,PPR,REG,FIELD,_PPR_DB_SHFT,_PPR_DB_WIDTH)

#define _PPR_FSETA(addr,REG,FIELD,x)\
	_CCL_PER_FSET(addr,PPR,REG,FIELD,x,_PPR_DB_SHFT,_PPR_DB_WIDTH)

#define _PPR_FSETSA(addr,REG,FIELD,SYM)\
	_CCL_PER_FSETS(addr,PPR,REG,FIELD,SYM,_PPR_DB_SHFT,_PPR_DB_WIDTH)


/* Handle based register/field access macros */

/* This is for base-address determined by handle h.
#define PPR_ADDRH(h,REG)\
	(_PPR_AB_SHFT)(&((h)->addrBase[_PPR_##REG##_OFFSET]))
*/
#define PPR_ADDRH(h,REG)\
	(_PPR_AB_SHFT)(_PPR_ADDR_BASE+_PPR_##REG##_OFFSET)

#define PPR_RGETH(h,REG)\
	_PPR_RGETA(PPR_ADDRH(h,REG),REG)

#define PPR_RSETH(h,REG,x)\
	_PPR_RSETA(PPR_ADDRH(h,REG),REG,x)

#define PPR_RSETSH(h,REG,SYM)\
	_PPR_RSETSA(PPR_ADDRH(h,REG),REG,SYM)

#define PPR_FGETH(h,REG,FIELD)\
	_PPR_FGETA(PPR_ADDRH(h,REG),REG,FIELD)

#define PPR_FSETH(h,REG,FIELD,x)\
	_PPR_FSETA(PPR_ADDRH(h,REG),REG,FIELD,x)

#define PPR_FSETSH(h,REG,FIELD,SYM)\
	_PPR_FSETSA(PPR_ADDRH(h,REG),REG,FIELD,SYM)

/* Function Prototypes ----------------------------------------------------*/

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  C T R L          |
 * |___________________|
 *
 * CTRL  - PPR Control Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  SYS625IND                      -
 *               SYS625IND |   description
 *              -----------+----------------------
 *                    0    | 625 lines per frame
 *                    1    | 525 lines per frame
 * (rw)  DATA_ACQU (Acquisition from VBI Slicer Enable)    -
 *               DATA_ACQU |   description
 *              -----------+-------------------
 *                    0    |   DISABLE
 *                    1    |   ENABLE
\*-------------------------------------------------------------------------*/
#define _PPR_CTRL_OFFSET                    (0x0000u)
#define _PPR_CTRL_ADDRH(h)                  PPR_ADDRH(h,CTRL)
#define _PPR_CTRL_LEN                       (32)                 /* 32-bit*/
#define _PPR_CTRL_TYPE                      uint32_t

#define _PPR_CTRL_SYS625IND_MASK            (0x02u)
#define _PPR_CTRL_SYS625IND_SHIFT           (0x01u)
#define  PPR_CTRL_SYS625IND_DEFAULT         (0x00u)
#define  PPR_CTRL_SYS625IND_OF(x)           _CCL_VALUEOF(_PPR_CTRL_TYPE,x)
#define  PPR_CTRL_SYS625IND_625             (0x00u)
#define  PPR_CTRL_SYS625IND_525             (0x01u)

#define _PPR_CTRL_DATA_ACQU_MASK            (0x01u)
#define _PPR_CTRL_DATA_ACQU_SHIFT           (0x00u)
#define  PPR_CTRL_DATA_ACQU_DEFAULT         (0x00u)
#define  PPR_CTRL_DATA_ACQU_OF(x)           _CCL_VALUEOF(_PPR_CTRL_TYPE,x)
#define  PPR_CTRL_DATA_ACQU_DISABLE         (0x00u)
#define  PPR_CTRL_DATA_ACQU_ENABLE          (0x01u)

#define  PPR_CTRL_OF(x)             _CCL_VALUEOF(_PPR_CTRL_TYPE,x)

#define  PPR_CTRL_DEFAULT (_PPR_CTRL_TYPE)(   \
	 _CCL_PER_FDEFAULT(PPR,CTRL,SYS625IND)    \
	|_CCL_PER_FDEFAULT(PPR,CTRL,DATA_ACQU)    \
	)

#define PPR_CTRL_RMK(sys625ind, data_acqu) (_PPR_CTRL_TYPE)( \
	 _CCL_PER_FMK  (PPR,CTRL,SYS625IND,sys625ind)            \
	|_CCL_PER_FMK  (PPR,CTRL,DATA_ACQU,data_acqu)            \
	)

#define PPR_CTRL_RMKS(sys625ind, data_acqu) (_PPR_CTRL_TYPE)( \
	 _CCL_PER_FMKS (PPR,CTRL,SYS625IND,sys625ind)             \
	|_CCL_PER_FMKS (PPR,CTRL,DATA_ACQU,data_acqu)             \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  V B I I N T  S T |
 * |___________________|
 *
 * VBIINT_ST - VBI Interrupt Status Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (r)  OSD -
 *                   OSD   |   description
 *              -----------+----------------------
 *                INACTIVE | VBIOSD_INT is in-active
 *                  ACTIVE | VBIOSD_INT is active
 *
 * (r)  VTE -
 *                   VTE   |   description
 *              -----------+----------------------
 *                INACTIVE | VBIVTE_INT is in-active
 *                  ACTIVE | VBIVTE_INT is active
 *
 * (r)  PPR -
 *                   PPR   |   description
 *              -----------+----------------------
 *                INACTIVE | VBIOSD_INT is in-active
 *                  ACTIVE | VBIOSD_INT is active
\*-------------------------------------------------------------------------*/
#define _PPR_VBIINT_ST_OFFSET                    (0x0004u)
#define _PPR_VBIINT_ST_ADDRH(h)                  PPR_ADDRH(h,VBIINT_ST)
#define _PPR_VBIINT_ST_LEN                       (32)                 /* 32-bit*/
#define _PPR_VBIINT_ST_TYPE                      uint32_t

#define _PPR_VBIINT_ST_OSD_MASK                  (0x00000004u)
#define _PPR_VBIINT_ST_OSD_SHIFT                 (2)
#define  PPR_VBIINT_ST_OSD_DEFAULT               (0x00u)
#define  PPR_VBIINT_ST_OSD_OF(x)                 _CCL_VALUEOF(_PPR_VBIINT_ST_TYPE,x)
#define  PPR_VBIINT_ST_OSD_INACTIVE              (0x00u)
#define  PPR_VBIINT_ST_OSD_ACTIVE                (0x01u)

#define _PPR_VBIINT_ST_VTE_MASK                  (0x00000002u)
#define _PPR_VBIINT_ST_VTE_SHIFT                 (1)
#define  PPR_VBIINT_ST_VTE_DEFAULT               (0x00u)
#define  PPR_VBIINT_ST_VTE_OF(x)                 _CCL_VALUEOF(_PPR_VBIINT_ST_TYPE,x)
#define  PPR_VBIINT_ST_VTE_INACTIVE              (0x00u)
#define  PPR_VBIINT_ST_VTE_ACTIVE                (0x01u)

#define _PPR_VBIINT_ST_PPR_MASK                  (0x00000001u)
#define _PPR_VBIINT_ST_PPR_SHIFT                 (0)
#define  PPR_VBIINT_ST_PPR_DEFAULT               (0x00u)
#define  PPR_VBIINT_ST_PPR_OF(x)                 _CCL_VALUEOF(_PPR_VBIINT_ST_TYPE,x)
#define  PPR_VBIINT_ST_PPR_INACTIVE              (0x00u)
#define  PPR_VBIINT_ST_PPR_ACTIVE                (0x01u)

#define  PPR_VBIINT_ST_OF(x)             _CCL_VALUEOF(_PPR_VBIINT_ST_TYPE,x)

#define  PPR_VBIINT_ST_DEFAULT (_PPR_VBIINT_ST_TYPE)(   \
	 _CCL_PER_FDEFAULT(PPR,VBIINT_ST,OSD)               \
	|_CCL_PER_FDEFAULT(PPR,VBIINT_ST,VTE)               \
	|_CCL_PER_FDEFAULT(PPR,VBIINT_ST,PPR)               \
	)

#define  PPR_VBIINT_ST_NONE  (_PPR_VBIINT_ST_TYPE)(   \
     PPR_VBIINT_ST_OSD_INACTIVE                       \
    |PPR_VBIINT_ST_VTE_INACTIVE                       \
    |PPR_VBIINT_ST_PPR_INACTIVE                       \
    )

#define PPR_VBIINT_ST_RMK(osd, vte, ppr) (_PPR_VBIINT_ST_TYPE)( \
	 _CCL_PER_FMK  (PPR,VBIINT_ST,OSD,osd)                      \
	|_CCL_PER_FMK  (PPR,VBIINT_ST,VTE,vte)                      \
	|_CCL_PER_FMK  (PPR,VBIINT_ST,PPR,ppr)                      \
	)

#define PPR_VBIINT_ST_RMKS(osd, vte, ppr) (_PPR_VBIINT_ST_TYPE)( \
	 _CCL_PER_FMKS (PPR,VBIINT_ST,OSD,osd)                       \
	|_CCL_PER_FMKS (PPR,VBIINT_ST,VTE,vte)                       \
	|_CCL_PER_FMKS (PPR,VBIINT_ST,PPR,ppr)                       \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  I N T   S T      |
 * |___________________|
 *
 * INT_ST - Interrupt Status Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  DEBUG_INT -
 *               DEBUG_INT |   description
 *              -----------+----------------------
 *                INACTIVE | decoding ok     (read)
 *                  ACTIVE | decoding fail   (read)
 *                  NOP    | no change       (write)
 *                  CLEAR  | clear interrupt (write)
 *
 * (rw)  MEMSET -
 *                 MEMSET  |   description
 *              -----------+--------------------------------------------------
 *                 RUNNING | VBI_MEMSET_DONE_INT is in-active          (read)
 *                  DONE   | VBI_MEMSET_DONE_INT VBIVTE_INT is active  (read)
 *                  NOP    | no change                                 (write)
 *                  CLEAR  | clear interrupt                           (write)
 *
 * (rw)  MEMSAR -
 *                 MEMSAR  |   description
 *              -----------+--------------------------------------------------
 *                 RUNNING | VBI_MEMSAR_DONE_INT is in-active          (read)
 *                  DONE   | VBI_MEMSAR_DONE_INT VBIVTE_INT is active  (read)
 *                  NOP    | no change                                 (write)
 *                  CLEAR  | clear interrupt                           (write)
 *
 * (rw)  PFE -
 *                   PFE   |   description
 *              -----------+--------------------------------------------------
 *                 RUNNING | VBI_PRE_MOV_ENG_DMA_DONE_INT is in-active (read)
 *                  DONE   | VBI_PRE_MOV_ENG_DMA_DONE_INT is active    (read)
 *                  NOP    | no change                                 (write)
 *                  CLEAR  | clear interrupt                           (write)
 *
 * (rw)  X31 -
 *                 PFE_DMA |   description
 *              -----------+--------------------------------------------------
 *                NOT_RCVD | VBI_DATA_RX_INT is in-active              (read)
 *                RECEIVED | VBI_DATA_RX_INT is active                 (read)
 *                  NOP    | no change                                 (write)
 *                  CLEAR  | clear interrupt                           (write)
 *
 * (rw)  FB_FUL -
 *                 PFE_DMA |   description
 *              -----------+--------------------------------------------------
 *                INACTIVE | VBI_DATA_RX_INT is in-active              (read)
 *                  ACTIVE | VBI_DATA_RX_INT is active                 (read)
 *                  NOP    | no change                                 (write)
 *                  CLEAR  | clear interrupt                           (write)
 *
 * (rw)  FB_VALID -
 *                 PFE_DMA |   description
 *              -----------+--------------------------------------------------
 *                INACTIVE | VBI_DATA_RX_INT is in-active              (read)
 *                  ACTIVE | VBI_DATA_RX_INT is active                 (read)
 *                  NOP    | no change                                 (write)
 *                  CLEAR  | clear interrupt                           (write)
 *
 * (rw)  830F2 - 8/30 Format-2
 *                  830F2  |   description
 *              -----------+--------------------------------------------------
 *                NOT_RCVD | VBI_DATA_RX_INT is in-active              (read)
 *                RECEIVED | VBI_DATA_RX_INT is active                 (read)
 *                  NOP    | no change                                 (write)
 *                  CLEAR  | clear interrupt                           (write)
 *
 * (rw)  830F1 - 8/30 Format-1
 *                  830F1  |   description
 *              -----------+--------------------------------------------------
 *                NOT_RCVD | VBI_DATA_RX_INT is in-active              (read)
 *                RECEIVED | VBI_DATA_RX_INT is active                 (read)
 *                  NOP    | no change                                 (write)
 *                  CLEAR  | clear interrupt                           (write)
 *
 * (rw)  VPS -
 *                  VPS    |   description
 *              -----------+--------------------------------------------------
 *                NOT_RCVD | VBI_DATA_RX_INT is in-active              (read)
 *                RECEIVED | VBI_DATA_RX_INT is active                 (read)
 *                  NOP    | no change                                 (write)
 *                  CLEAR  | clear interrupt                           (write)
 *
 * (rw)  CGMS -
 *                  CGMS   |   description
 *              -----------+--------------------------------------------------
 *                NOT_RCVD | VBI_DATA_RX_INT is in-active              (read)
 *                RECEIVED | VBI_DATA_RX_INT is active                 (read)
 *                  NOP    | no change                                 (write)
 *                  CLEAR  | clear interrupt                           (write)
 *
 * (rw)  WSS -
 *                  WSS    |   description
 *              -----------+--------------------------------------------------
 *                NOT_RCVD | VBI_DATA_RX_INT is in-active              (read)
 *                RECEIVED | VBI_DATA_RX_INT is active                 (read)
 *                  NOP    | no change                                 (write)
 *                  CLEAR  | clear interrupt                           (write)
 *
 * (rw)  CC -
 *                   CC    |   description
 *              -----------+--------------------------------------------------
 *                NOT_RCVD | VBI_DATA_RX_INT is in-active              (read)
 *                RECEIVED | VBI_DATA_RX_INT is active                 (read)
 *                  NOP    | no change                                 (write)
 *                  CLEAR  | clear interrupt                           (write)
\*-------------------------------------------------------------------------*/
#define _PPR_INT_ST_OFFSET                    (0x0008u)
#define _PPR_INT_ST_ADDRH(h)                  PPR_ADDRH(h,INT_ST)
#define _PPR_INT_ST_LEN                       (32)                 /* 32-bit*/
#define _PPR_INT_ST_TYPE                      uint32_t

#define _PPR_INT_ST_DEBUG_INT_MASK            (0x80000000u)
#define _PPR_INT_ST_DEBUG_INT_SHIFT           (31)
#define  PPR_INT_ST_DEBUG_INT_DEFAULT         (0x00u)
#define  PPR_INT_ST_DEBUG_INT_OF(x)           _CCL_VALUEOF(_PPR_INT_ST_TYPE,x)
#define  PPR_INT_ST_DEBUG_INT_INACTIVE        (0x00u)
#define  PPR_INT_ST_DEBUG_INT_ACTIVE          (0x01u)
#define  PPR_INT_ST_DEBUG_INT_NOP             (0x00u)
#define  PPR_INT_ST_DEBUG_INT_CLEAR           (0x01u)

#define _PPR_INT_ST_MEMSET_MASK               (0x00040000u)
#define _PPR_INT_ST_MEMSET_SHIFT              (18)
#define  PPR_INT_ST_MEMSET_DEFAULT            (0x00u)
#define  PPR_INT_ST_MEMSET_OF(x)              _CCL_VALUEOF(_PPR_INT_ST_TYPE,x)
#define  PPR_INT_ST_MEMSET_RUNNING            (0x00u)
#define  PPR_INT_ST_MEMSET_DONE               (0x01u)
#define  PPR_INT_ST_MEMSET_NOP                (0x00u)
#define  PPR_INT_ST_MEMSET_CLEAR              (0x01u)

#define _PPR_INT_ST_MEMSAR_MASK               (0x00020000u)
#define _PPR_INT_ST_MEMSAR_SHIFT              (17)
#define  PPR_INT_ST_MEMSAR_DEFAULT            (0x00u)
#define  PPR_INT_ST_MEMSAR_OF(x)              _CCL_VALUEOF(_PPR_INT_ST_TYPE,x)
#define  PPR_INT_ST_MEMSAR_RUNNING            (0x00u)
#define  PPR_INT_ST_MEMSAR_DONE               (0x01u)
#define  PPR_INT_ST_MEMSAR_NOP                (0x00u)
#define  PPR_INT_ST_MEMSAR_CLEAR              (0x01u)

#define _PPR_INT_ST_PFE_MASK                  (0x00010000u)
#define _PPR_INT_ST_PFE_SHIFT                 (16)
#define  PPR_INT_ST_PFE_DEFAULT               (0x00u)
#define  PPR_INT_ST_PFE_OF(x)                 _CCL_VALUEOF(_PPR_INT_ST_TYPE,x)
#define  PPR_INT_ST_PFE_RUNNING               (0x00u)
#define  PPR_INT_ST_PFE_DONE                  (0x01u)
#define  PPR_INT_ST_PFE_NOP                   (0x00u)
#define  PPR_INT_ST_PFE_CLEAR                 (0x01u)

#define _PPR_INT_ST_X31_MASK                  (0x00000100u)
#define _PPR_INT_ST_X31_SHIFT                 (8)
#define  PPR_INT_ST_X31_DEFAULT               (0x00u)
#define  PPR_INT_ST_X31_OF(x)                 _CCL_VALUEOF(_PPR_INT_ST_TYPE,x)
#define  PPR_INT_ST_X31_NOT_RCVD              (0x00u)
#define  PPR_INT_ST_X31_RECEIVED              (0x01u)
#define  PPR_INT_ST_X31_NOP                   (0x00u)
#define  PPR_INT_ST_X31_CLEAR                 (0x01u)

#define _PPR_INT_ST_FB_FUL_MASK               (0x00000080u)
#define _PPR_INT_ST_FB_FUL_SHIFT              (7)
#define  PPR_INT_ST_FB_FUL_DEFAULT            (0x00u)
#define  PPR_INT_ST_FB_FUL_OF(x)              _CCL_VALUEOF(_PPR_INT_ST_TYPE,x)
#define  PPR_INT_ST_FB_FUL_INACTIVE           (0x00u)
#define  PPR_INT_ST_FB_FUL_ACTIVE             (0x01u)
#define  PPR_INT_ST_FB_FUL_NOP                (0x00u)
#define  PPR_INT_ST_FB_FUL_CLEAR              (0x01u)

#define _PPR_INT_ST_FB_VALID_MASK             (0x00000040u)
#define _PPR_INT_ST_FB_VALID_SHIFT            (6)
#define  PPR_INT_ST_FB_VALID_DEFAULT          (0x00u)
#define  PPR_INT_ST_FB_VALID_OF(x)            _CCL_VALUEOF(_PPR_INT_ST_TYPE,x)
#define  PPR_INT_ST_FB_VALID_INACTIVE         (0x00u)
#define  PPR_INT_ST_FB_VALID_ACTIVE           (0x01u)
#define  PPR_INT_ST_FB_VALID_NOP              (0x00u)
#define  PPR_INT_ST_FB_VALID_CLEAR            (0x01u)

#define _PPR_INT_ST_830F2_MASK                (0x00000020u)
#define _PPR_INT_ST_830F2_SHIFT               (5)
#define  PPR_INT_ST_830F2_DEFAULT             (0x00u)
#define  PPR_INT_ST_830F2_OF(x)               _CCL_VALUEOF(_PPR_INT_ST_TYPE,x)
#define  PPR_INT_ST_830F2_NOT_RCVD            (0x00u)
#define  PPR_INT_ST_830F2_RECEIVED            (0x01u)
#define  PPR_INT_ST_830F2_NOP                 (0x00u)
#define  PPR_INT_ST_830F2_CLEAR               (0x01u)

#define _PPR_INT_ST_830F1_MASK                (0x00000010u)
#define _PPR_INT_ST_830F1_SHIFT               (4)
#define  PPR_INT_ST_830F1_DEFAULT             (0x00u)
#define  PPR_INT_ST_830F1_OF(x)               _CCL_VALUEOF(_PPR_INT_ST_TYPE,x)
#define  PPR_INT_ST_830F1_NOT_RCVD            (0x00u)
#define  PPR_INT_ST_830F1_RECEIVED            (0x01u)
#define  PPR_INT_ST_830F1_NOP                 (0x00u)
#define  PPR_INT_ST_830F1_CLEAR               (0x01u)

#define _PPR_INT_ST_VPS_MASK                  (0x00000008u)
#define _PPR_INT_ST_VPS_SHIFT                 (3)
#define  PPR_INT_ST_VPS_DEFAULT               (0x00u)
#define  PPR_INT_ST_VPS_OF(x)                 _CCL_VALUEOF(_PPR_INT_ST_TYPE,x)
#define  PPR_INT_ST_VPS_NOT_RCVD              (0x00u)
#define  PPR_INT_ST_VPS_RECEIVED              (0x01u)
#define  PPR_INT_ST_VPS_NOP                   (0x00u)
#define  PPR_INT_ST_VPS_CLEAR                 (0x01u)

#define _PPR_INT_ST_CGMS_MASK                 (0x00000004u)
#define _PPR_INT_ST_CGMS_SHIFT                (2)
#define  PPR_INT_ST_CGMS_DEFAULT              (0x00u)
#define  PPR_INT_ST_CGMS_OF(x)                _CCL_VALUEOF(_PPR_INT_ST_TYPE,x)
#define  PPR_INT_ST_CGMS_NOT_RCVD             (0x00u)
#define  PPR_INT_ST_CGMS_RECEIVED             (0x01u)
#define  PPR_INT_ST_CGMS_NOP                  (0x00u)
#define  PPR_INT_ST_CGMS_CLEAR                (0x01u)

#define _PPR_INT_ST_WSS_MASK                  (0x00000002u)
#define _PPR_INT_ST_WSS_SHIFT                 (1)
#define  PPR_INT_ST_WSS_DEFAULT               (0x00u)
#define  PPR_INT_ST_WSS_OF(x)                 _CCL_VALUEOF(_PPR_INT_ST_TYPE,x)
#define  PPR_INT_ST_WSS_NOT_RCVD              (0x00u)
#define  PPR_INT_ST_WSS_RECEIVED              (0x01u)
#define  PPR_INT_ST_WSS_NOP                   (0x00u)
#define  PPR_INT_ST_WSS_CLEAR                 (0x01u)

#define _PPR_INT_ST_CC_MASK                   (0x00000001u)
#define _PPR_INT_ST_CC_SHIFT                  (0)
#define  PPR_INT_ST_CC_DEFAULT                (0x00u)
#define  PPR_INT_ST_CC_OF(x)                  _CCL_VALUEOF(_PPR_INT_ST_TYPE,x)
#define  PPR_INT_ST_CC_NOT_RCVD               (0x00u)
#define  PPR_INT_ST_CC_RECEIVED               (0x01u)
#define  PPR_INT_ST_CC_NOP                    (0x00u)
#define  PPR_INT_ST_CC_CLEAR                  (0x01u)


#define  PPR_INT_ST_OF(x)             _CCL_VALUEOF(_PPR_INT_ST_TYPE,x)

#define  PPR_INT_ST_DEFAULT (_PPR_INT_ST_TYPE)(   \
	 _CCL_PER_FDEFAULT(PPR,INT_ST,DEBUG_INT)      \
	|_CCL_PER_FDEFAULT(PPR,INT_ST,MEMSET   )      \
	|_CCL_PER_FDEFAULT(PPR,INT_ST,MEMSAR   )      \
	|_CCL_PER_FDEFAULT(PPR,INT_ST,PFE      )      \
	|_CCL_PER_FDEFAULT(PPR,INT_ST,X31      )      \
	|_CCL_PER_FDEFAULT(PPR,INT_ST,FB_FUL   )      \
	|_CCL_PER_FDEFAULT(PPR,INT_ST,FB_VALID )      \
	|_CCL_PER_FDEFAULT(PPR,INT_ST,830F2    )      \
	|_CCL_PER_FDEFAULT(PPR,INT_ST,830F1    )      \
	|_CCL_PER_FDEFAULT(PPR,INT_ST,VPS      )      \
	|_CCL_PER_FDEFAULT(PPR,INT_ST,CGMS     )      \
	|_CCL_PER_FDEFAULT(PPR,INT_ST,WSS      )      \
	|_CCL_PER_FDEFAULT(PPR,INT_ST,CC       )      \
	)

#define PPR_INT_ST_RMK(debug_int, memset  , memsar, pfe    , x31,   \
                       fb_ful   , fb_valid, f2830 , f1830  , vps,   \
                       cgms     , wss     , cc) (_PPR_INT_ST_TYPE)( \
	 _CCL_PER_FMK  (PPR, INT_ST, DEBUG_INT, debug_int)              \
	|_CCL_PER_FMK  (PPR, INT_ST, MEMSET   , memset   )              \
	|_CCL_PER_FMK  (PPR, INT_ST, MEMSAR   , memsar   )              \
	|_CCL_PER_FMK  (PPR, INT_ST, PFE      , pfe      )              \
	|_CCL_PER_FMK  (PPR, INT_ST, X31      , x31      )              \
	|_CCL_PER_FMK  (PPR, INT_ST, FB_FUL   , fb_ful   )              \
	|_CCL_PER_FMK  (PPR, INT_ST, FB_VALID , fb_valid )              \
	|_CCL_PER_FMK  (PPR, INT_ST, 830F2    , f2830    )              \
	|_CCL_PER_FMK  (PPR, INT_ST, 830F1    , f1830    )              \
	|_CCL_PER_FMK  (PPR, INT_ST, VPS      , vps      )              \
	|_CCL_PER_FMK  (PPR, INT_ST, CGMS     , cgms     )              \
	|_CCL_PER_FMK  (PPR, INT_ST, WSS      , wss      )              \
	|_CCL_PER_FMK  (PPR, INT_ST, CC       , cc       )              \
	)

#define PPR_INT_ST_RMKS(debug_int, memset  , memsar, pfe   , x31,   \
                       fb_ful   , fb_valid, f2830 , f1830  , vps,   \
                       cgms     , wss     , cc) (_PPR_INT_ST_TYPE)( \
	 _CCL_PER_FMKS (PPR, INT_ST, DEBUG_INT, debug_int)              \
	|_CCL_PER_FMKS (PPR, INT_ST, MEMSET   , memset   )              \
	|_CCL_PER_FMKS (PPR, INT_ST, MEMSAR   , memsar   )              \
	|_CCL_PER_FMKS (PPR, INT_ST, PFE      , pfe      )              \
	|_CCL_PER_FMKS (PPR, INT_ST, X31      , x31      )              \
	|_CCL_PER_FMKS (PPR, INT_ST, FB_FUL   , fb_ful   )              \
	|_CCL_PER_FMKS (PPR, INT_ST, FB_VALID , fb_valid )              \
	|_CCL_PER_FMKS (PPR, INT_ST, 830F2    , f2830    )              \
	|_CCL_PER_FMKS (PPR, INT_ST, 830F1    , f1830    )              \
	|_CCL_PER_FMKS (PPR, INT_ST, VPS      , vps      )              \
	|_CCL_PER_FMKS (PPR, INT_ST, CGMS     , cgms     )              \
	|_CCL_PER_FMKS (PPR, INT_ST, WSS      , wss      )              \
	|_CCL_PER_FMKS (PPR, INT_ST, CC       , cc       )              \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  I N T   E N      |
 * |___________________|
 *
 * INT_EN - Interrupt Enable Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  MASTER - Master Interrupt Enable
 *                  MIE    |   description
 *              -----------+-------------------------------------------------------
 *                  NOP    | Ignore
 *                ENABLE   | Enable interrupt generation due
 *                         |  to event specified in the other bits of this register
 *
 * (rw)  DEBUG_INT - Notify the VBIPPR prefetch / memset / memsar operation failed.
 *              DEBUG_INT  |   description
 *           --------------+--------------------------------------------------
 *                  NOP    | Ignore
 *                ENABLE   | Enable interrupt generation due
 *                         |  to VBIVTE transfer operation can’t complete in VBIPPR_DEBUG_CLK_CNT.
 *
 * (rw)  MEMSET - Notify the MEMSET done event by VBI_MEMSET_DONE_INT.
 *                         It is level-trigger method.
 *                MEMSET   |   description
 *          ---------------+--------------------------------------------------
 *                  NOP    | Ignore
 *                ENABLE   | Enable interrupt generation due
 *                         |  to memory set done.
 *
 * (rw)  MEMSAR - Notify the MEMSAR done event by VBI_MEMSAR_DONE_INT.
 *                         It is level-trigger method.
 *                MEMSAR   |   description
 *          ---------------+--------------------------------------------------
 *                  NOP    | Ignore
 *                ENABLE   | Enable interrupt generation due
 *                         |  to memory set done.
 *
 * (rw)  PFE - Notify the DMA done event by VBI_PFE_MOV_ENG_DMA_DONE_INT.
 *                          It is level-trigger method
 *                 PFE     |   description
 *         ----------------+--------------------------------------------------
 *                  NOP    | Ignore
 *                ENABLE   | Enable interrupt generation due to PFEMOVENG DMA is done.
 *
 * (rw)  X31 -
 *                   X31   |   description
 *              -----------+--------------------------------------------------
 *                  NOP    | Ignore
 *                ENABLE   | Enable interrupt generation due to receive a new X/31 data.
 *
 * (rw)  FB_FUL -
 *                 FB_FUL  |   description
 *              -----------+--------------------------------------------------
 *                  NOP    | Ignore
 *                ENABLE   | Enable interrupt generation due to VBI field buffer full.
 *
 * (rw)  FB_VALID -
 *                FB_VALID |   description
 *              -----------+--------------------------------------------------
 *                  NOP    | Ignore
 *                ENABLE   | Enable interrupt generation due to VBI field buffer valid.
 *
 * (rw)  830F2 - 8/30 Format-2
 *                  830F2  |   description
 *              -----------+--------------------------------------------------
 *                  NOP    | Ignore
 *                ENABLE   | Enable interrupt generation due to receive a new 8/30 Format 2 data.
 *
 * (rw)  830F1 - 8/30 Format-1
 *                  830F1  |   description
 *              -----------+--------------------------------------------------
 *                  NOP    | Ignore
 *                ENABLE   | Enable interrupt generation due to receive a new 8/30 Format 1 data.
 *
 * (rw)  VPS -
 *                  VPS    |   description
 *              -----------+--------------------------------------------------
 *                  NOP    | Ignore
 *                ENABLE   | Enable interrupt generation due to receive a new VPS data.
 *
 * (rw)  CGMS -
 *                  CGMS   |   description
 *              -----------+--------------------------------------------------
 *                  NOP    | Ignore
 *                ENABLE   | Enable interrupt generation due to receive a new CGMS data.
 *
 * (rw)  WSS -
 *                  WSS    |   description
 *              -----------+--------------------------------------------------
 *                  NOP    | Ignore
 *                ENABLE   | Enable interrupt generation due to receive a new WSS data.
 *
 * (rw)  CC -
 *                   CC    |   description
 *              -----------+--------------------------------------------------
 *                  NOP    | Ignore
 *                ENABLE   | Enable interrupt generation due to receive a new CC data.
\*-------------------------------------------------------------------------*/
#define _PPR_INT_EN_OFFSET                    (0x000Cu)
#define _PPR_INT_EN_ADDRH(h)                  PPR_ADDRH(h,INT_EN)
#define _PPR_INT_EN_LEN                       (32)                 /* 32-bit*/
#define _PPR_INT_EN_TYPE                      uint32_t

#define _PPR_INT_EN_MASTER_MASK               (0x80000000u)
#define _PPR_INT_EN_MASTER_SHIFT              (31)
#define  PPR_INT_EN_MASTER_DEFAULT            (0x00u)
#define  PPR_INT_EN_MASTER_OF(x)              _CCL_VALUEOF(_PPR_INT_EN_TYPE,x)
#define  PPR_INT_EN_MASTER_NOP                (0x00u)
#define  PPR_INT_EN_MASTER_ENABLE             (0x01u)

#define _PPR_INT_EN_DEBUG_INT_MASK            (0x40000000u)
#define _PPR_INT_EN_DEBUG_INT_SHIFT           (30)
#define  PPR_INT_EN_DEBUG_INT_DEFAULT         (0x00u)
#define  PPR_INT_EN_DEBUG_INT_OF(x)           _CCL_VALUEOF(_PPR_INT_EN_TYPE,x)
#define  PPR_INT_EN_DEBUG_INT_NOP             (0x00u)
#define  PPR_INT_EN_DEBUG_INT_ENABLE          (0x01u)

#define _PPR_INT_EN_MEMSET_MASK               (0x00040000u)
#define _PPR_INT_EN_MEMSET_SHIFT              (18)
#define  PPR_INT_EN_MEMSET_DEFAULT            (0x00u)
#define  PPR_INT_EN_MEMSET_OF(x)              _CCL_VALUEOF(_PPR_INT_EN_TYPE,x)
#define  PPR_INT_EN_MEMSET_NOP                (0x00u)
#define  PPR_INT_EN_MEMSET_ENABLE             (0x01u)

#define _PPR_INT_EN_MEMSAR_MASK               (0x00020000u)
#define _PPR_INT_EN_MEMSAR_SHIFT              (17)
#define  PPR_INT_EN_MEMSAR_DEFAULT            (0x00u)
#define  PPR_INT_EN_MEMSAR_OF(x)              _CCL_VALUEOF(_PPR_INT_EN_TYPE,x)
#define  PPR_INT_EN_MEMSAR_NOP                (0x00u)
#define  PPR_INT_EN_MEMSAR_ENABLE             (0x01u)

#define _PPR_INT_EN_PFE_MASK                  (0x00010000u)
#define _PPR_INT_EN_PFE_SHIFT                 (16)
#define  PPR_INT_EN_PFE_DEFAULT               (0x00u)
#define  PPR_INT_EN_PFE_OF(x)                 _CCL_VALUEOF(_PPR_INT_EN_TYPE,x)
#define  PPR_INT_EN_PFE_NOP                   (0x00u)
#define  PPR_INT_EN_PFE_ENABLE                (0x01u)

#define _PPR_INT_EN_X31_MASK                  (0x00000100u)
#define _PPR_INT_EN_X31_SHIFT                 (8)
#define  PPR_INT_EN_X31_DEFAULT               (0x00u)
#define  PPR_INT_EN_X31_OF(x)                 _CCL_VALUEOF(_PPR_INT_EN_TYPE,x)
#define  PPR_INT_EN_X31_NOP                   (0x00u)
#define  PPR_INT_EN_X31_ENABLE                (0x01u)

#define _PPR_INT_EN_FB_FUL_MASK               (0x00000080u)
#define _PPR_INT_EN_FB_FUL_SHIFT              (7)
#define  PPR_INT_EN_FB_FUL_DEFAULT            (0x00u)
#define  PPR_INT_EN_FB_FUL_OF(x)              _CCL_VALUEOF(_PPR_INT_EN_TYPE,x)
#define  PPR_INT_EN_FB_FUL_NOP                (0x00u)
#define  PPR_INT_EN_FB_FUL_ENABLE             (0x01u)

#define _PPR_INT_EN_FB_VALID_MASK             (0x00000040u)
#define _PPR_INT_EN_FB_VALID_SHIFT            (6)
#define  PPR_INT_EN_FB_VALID_DEFAULT          (0x00u)
#define  PPR_INT_EN_FB_VALID_OF(x)            _CCL_VALUEOF(_PPR_INT_EN_TYPE,x)
#define  PPR_INT_EN_FB_VALID_NOP              (0x00u)
#define  PPR_INT_EN_FB_VALID_ENABLE           (0x01u)

#define _PPR_INT_EN_830F2_MASK                (0x00000020u)
#define _PPR_INT_EN_830F2_SHIFT               (5)
#define  PPR_INT_EN_830F2_DEFAULT             (0x00u)
#define  PPR_INT_EN_830F2_OF(x)               _CCL_VALUEOF(_PPR_INT_EN_TYPE,x)
#define  PPR_INT_EN_830F2_NOP                 (0x00u)
#define  PPR_INT_EN_830F2_ENABLE              (0x01u)

#define _PPR_INT_EN_830F1_MASK                (0x00000010u)
#define _PPR_INT_EN_830F1_SHIFT               (4)
#define  PPR_INT_EN_830F1_DEFAULT             (0x00u)
#define  PPR_INT_EN_830F1_OF(x)               _CCL_VALUEOF(_PPR_INT_EN_TYPE,x)
#define  PPR_INT_EN_830F1_NOP                 (0x00u)
#define  PPR_INT_EN_830F1_ENABLE              (0x01u)

#define _PPR_INT_EN_VPS_MASK                  (0x00000008u)
#define _PPR_INT_EN_VPS_SHIFT                 (3)
#define  PPR_INT_EN_VPS_DEFAULT               (0x00u)
#define  PPR_INT_EN_VPS_OF(x)                 _CCL_VALUEOF(_PPR_INT_EN_TYPE,x)
#define  PPR_INT_EN_VPS_NOP                   (0x00u)
#define  PPR_INT_EN_VPS_ENABLE                (0x01u)

#define _PPR_INT_EN_CGMS_MASK                 (0x00000004u)
#define _PPR_INT_EN_CGMS_SHIFT                (2)
#define  PPR_INT_EN_CGMS_DEFAULT              (0x00u)
#define  PPR_INT_EN_CGMS_OF(x)                _CCL_VALUEOF(_PPR_INT_EN_TYPE,x)
#define  PPR_INT_EN_CGMS_NOP                  (0x00u)
#define  PPR_INT_EN_CGMS_ENABLE               (0x01u)

#define _PPR_INT_EN_WSS_MASK                  (0x00000002u)
#define _PPR_INT_EN_WSS_SHIFT                 (1)
#define  PPR_INT_EN_WSS_DEFAULT               (0x00u)
#define  PPR_INT_EN_WSS_OF(x)                 _CCL_VALUEOF(_PPR_INT_EN_TYPE,x)
#define  PPR_INT_EN_WSS_NOP                   (0x00u)
#define  PPR_INT_EN_WSS_ENABLE                (0x01u)

#define _PPR_INT_EN_CC_MASK                   (0x00000001u)
#define _PPR_INT_EN_CC_SHIFT                  (0)
#define  PPR_INT_EN_CC_DEFAULT                (0x00u)
#define  PPR_INT_EN_CC_OF(x)                  _CCL_VALUEOF(_PPR_INT_EN_TYPE,x)
#define  PPR_INT_EN_CC_NOP                    (0x00u)
#define  PPR_INT_EN_CC_ENABLE                 (0x01u)


#define  PPR_INT_EN_OF(x)             _CCL_VALUEOF(_PPR_INT_EN_TYPE,x)

#define  PPR_INT_EN_DEFAULT (_PPR_INT_EN_TYPE)(   \
	 _CCL_PER_FDEFAULT(PPR,INT_EN,MASTER   )      \
	|_CCL_PER_FDEFAULT(PPR,INT_EN,DEBUG_INT)      \
	|_CCL_PER_FDEFAULT(PPR,INT_EN,MEMSET   )      \
	|_CCL_PER_FDEFAULT(PPR,INT_EN,MEMSAR   )      \
	|_CCL_PER_FDEFAULT(PPR,INT_EN,PFE      )      \
	|_CCL_PER_FDEFAULT(PPR,INT_EN,X31      )      \
	|_CCL_PER_FDEFAULT(PPR,INT_EN,FB_FUL   )      \
	|_CCL_PER_FDEFAULT(PPR,INT_EN,FB_VALID )      \
	|_CCL_PER_FDEFAULT(PPR,INT_EN,830F2    )      \
	|_CCL_PER_FDEFAULT(PPR,INT_EN,830F1    )      \
	|_CCL_PER_FDEFAULT(PPR,INT_EN,VPS      )      \
	|_CCL_PER_FDEFAULT(PPR,INT_EN,CGMS     )      \
	|_CCL_PER_FDEFAULT(PPR,INT_EN,WSS      )      \
	|_CCL_PER_FDEFAULT(PPR,INT_EN,CC       )      \
	)

#define PPR_INT_EN_RMK(master   ,                                   \
                       debug_int, memset  , memsar, pfe    , x31,   \
                       fb_ful   , fb_valid, f2830 , f1830  , vps,   \
                       cgms     , wss     , cc) (_PPR_INT_EN_TYPE)( \
	 _CCL_PER_FMK  (PPR, INT_EN, MASTER   , master   )              \
	|_CCL_PER_FMK  (PPR, INT_EN, DEBUG_INT, debug_int)              \
	|_CCL_PER_FMK  (PPR, INT_EN, MEMSET   , memset   )              \
	|_CCL_PER_FMK  (PPR, INT_EN, MEMSAR   , memsar   )              \
	|_CCL_PER_FMK  (PPR, INT_EN, PFE      , pfe      )              \
	|_CCL_PER_FMK  (PPR, INT_EN, X31      , x31      )              \
	|_CCL_PER_FMK  (PPR, INT_EN, FB_FUL   , fb_ful   )              \
	|_CCL_PER_FMK  (PPR, INT_EN, FB_VALID , fb_valid )              \
	|_CCL_PER_FMK  (PPR, INT_EN, 830F2    , f2830    )              \
	|_CCL_PER_FMK  (PPR, INT_EN, 830F1    , f1830    )              \
	|_CCL_PER_FMK  (PPR, INT_EN, VPS      , vps      )              \
	|_CCL_PER_FMK  (PPR, INT_EN, CGMS     , cgms     )              \
	|_CCL_PER_FMK  (PPR, INT_EN, WSS      , wss      )              \
	|_CCL_PER_FMK  (PPR, INT_EN, CC       , cc       )              \
	)

#define PPR_INT_EN_RMKS(master  ,                                   \
                       debug_int, memset  , memsar, pfe    , x31,   \
                       fb_ful   , fb_valid, f2830 , f1830  , vps,   \
                       cgms     , wss     , cc) (_PPR_INT_EN_TYPE)( \
	 _CCL_PER_FMKS (PPR, INT_EN, MASTER   , master   )              \
	|_CCL_PER_FMKS (PPR, INT_EN, DEBUG_INT, debug_int)              \
	|_CCL_PER_FMKS (PPR, INT_EN, MEMSET   , memset   )              \
	|_CCL_PER_FMKS (PPR, INT_EN, MEMSAR   , memsar   )              \
	|_CCL_PER_FMKS (PPR, INT_EN, PFE      , pfe      )              \
	|_CCL_PER_FMKS (PPR, INT_EN, X31      , x31      )              \
	|_CCL_PER_FMKS (PPR, INT_EN, FB_FUL   , fb_ful   )              \
	|_CCL_PER_FMKS (PPR, INT_EN, FB_VALID , fb_valid )              \
	|_CCL_PER_FMKS (PPR, INT_EN, 830F2    , f2830    )              \
	|_CCL_PER_FMKS (PPR, INT_EN, 830F1    , f1830    )              \
	|_CCL_PER_FMKS (PPR, INT_EN, VPS      , vps      )              \
	|_CCL_PER_FMKS (PPR, INT_EN, CGMS     , cgms     )              \
	|_CCL_PER_FMKS (PPR, INT_EN, WSS      , wss      )              \
	|_CCL_PER_FMKS (PPR, INT_EN, CC       , cc       )              \
	)

#define PPR_INT_EN_ALL                                       \
    PPR_INT_EN_RMKS( ENABLE,                                 \
                     ENABLE, ENABLE, ENABLE, ENABLE, ENABLE, \
                     ENABLE, ENABLE, ENABLE, ENABLE, ENABLE, \
                     ENABLE, ENABLE, ENABLE )

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  I N T   D I S    |
 * |___________________|
 *
 * INT_DIS - Interrupt Disable Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  MASTER - Master Interrupt Enable
 *                  MIE    |   description
 *              -----------+-------------------------------------------------------
 *                  NOP    | Ignore
 *                DISABLE  | Disable interrupt generation due
 *                         |  to event specified in the other bits of this register
 *
 * (rw)  DEBUG_INT - Notify the VBIPPR prefetch / memset / memsar operation failed.
 *              DEBUG_INT  |   description
 *           --------------+--------------------------------------------------
 *                  NOP    | Ignore
 *                DISABLE  | Disable interrupt generation due
 *                         |  to VBIVTE transfer operation can’t complete in VBIPPR_DEBUG_CLK_CNT.
 *
 * (rw)  MEMSET - Notify the MEMSET done event by VBI_MEMSET_DONE_INT.
 *                         It is level-trigger method.
 *                MEMSET   |   description
 *          ---------------+--------------------------------------------------
 *                  NOP    | Ignore
 *                DISABLE  | Disable interrupt generation due
 *                         |  to memory set done.
 *
 * (rw)  MEMSAR - Notify the MEMSAR done event by VBI_MEMSAR_DONE_INT.
 *                         It is level-trigger method.
 *                MEMSAR   |   description
 *          ---------------+--------------------------------------------------
 *                  NOP    | Ignore
 *                DISABLE  | Disable interrupt generation due
 *                         |  to memory set done.
 *
 * (rw)  PFE - Notify the DMA done event by VBI_PFE_MOV_ENG_DMA_DONE_INT.
 *                          It is level-trigger method
 *                 PFE     |   description
 *         ----------------+--------------------------------------------------
 *                  NOP    | Ignore
 *                DISABLE  | Disable interrupt generation due to PFEMOVENG DMA is done.
 *
 * (rw)  X31 -
 *                   X31   |   description
 *              -----------+--------------------------------------------------
 *                  NOP    | Ignore
 *                DISABLE  | Disable interrupt generation due to receive a new X/31 data.
 *
 * (rw)  FB_FUL -
 *                 FB_FUL  |   description
 *              -----------+--------------------------------------------------
 *                  NOP    | Ignore
 *                DISABLE  | Disable interrupt generation due to VBI field buffer full.
 *
 * (rw)  FB_VALID -
 *                FB_VALID |   description
 *              -----------+--------------------------------------------------
 *                  NOP    | Ignore
 *                DISABLE  | Disable interrupt generation due to VBI field buffer valid.
 *
 * (rw)  830F2 - 8/30 Format-2
 *                  830F2  |   description
 *              -----------+--------------------------------------------------
 *                  NOP    | Ignore
 *                DISABLE  | Disable interrupt generation due to receive a new 8/30 Format 2 data.
 *
 * (rw)  830F1 - 8/30 Format-1
 *                  830F1  |   description
 *              -----------+--------------------------------------------------
 *                  NOP    | Ignore
 *                DISABLE  | Disable interrupt generation due to receive a new 8/30 Format 1 data.
 *
 * (rw)  VPS -
 *                  VPS    |   description
 *              -----------+--------------------------------------------------
 *                  NOP    | Ignore
 *                DISABLE  | Disable interrupt generation due to receive a new VPS data.
 *
 * (rw)  CGMS -
 *                  CGMS   |   description
 *              -----------+--------------------------------------------------
 *                  NOP    | Ignore
 *                DISABLE  | Disable interrupt generation due to receive a new CGMS data.
 *
 * (rw)  WSS -
 *                  WSS    |   description
 *              -----------+--------------------------------------------------
 *                  NOP    | Ignore
 *                DISABLE  | Disable interrupt generation due to receive a new WSS data.
 *
 * (rw)  CC -
 *                   CC    |   description
 *              -----------+--------------------------------------------------
 *                  NOP    | Ignore
 *                DISABLE  | Disable interrupt generation due to receive a new CC data.
\*-------------------------------------------------------------------------*/
#define _PPR_INT_DIS_OFFSET                    (0x0010u)
#define _PPR_INT_DIS_ADDRH(h)                  PPR_ADDRH(h,INT_DIS)
#define _PPR_INT_DIS_LEN                       (32)                 /* 32-bit*/
#define _PPR_INT_DIS_TYPE                      uint32_t

#define _PPR_INT_DIS_MASTER_MASK               (0x80000000u)
#define _PPR_INT_DIS_MASTER_SHIFT              (31)
#define  PPR_INT_DIS_MASTER_DEFAULT            (0x00u)
#define  PPR_INT_DIS_MASTER_OF(x)              _CCL_VALUEOF(_PPR_INT_DIS_TYPE,x)
#define  PPR_INT_DIS_MASTER_NOP                (0x00u)
#define  PPR_INT_DIS_MASTER_DISABLE            (0x01u)

#define _PPR_INT_DIS_DEBUG_INT_MASK            (0x40000000u)
#define _PPR_INT_DIS_DEBUG_INT_SHIFT           (30)
#define  PPR_INT_DIS_DEBUG_INT_DEFAULT         (0x00u)
#define  PPR_INT_DIS_DEBUG_INT_OF(x)           _CCL_VALUEOF(_PPR_INT_DIS_TYPE,x)
#define  PPR_INT_DIS_DEBUG_INT_NOP             (0x00u)
#define  PPR_INT_DIS_DEBUG_INT_DISABLE         (0x01u)

#define _PPR_INT_DIS_MEMSET_MASK               (0x00040000u)
#define _PPR_INT_DIS_MEMSET_SHIFT              (18)
#define  PPR_INT_DIS_MEMSET_DEFAULT            (0x00u)
#define  PPR_INT_DIS_MEMSET_OF(x)              _CCL_VALUEOF(_PPR_INT_DIS_TYPE,x)
#define  PPR_INT_DIS_MEMSET_NOP                (0x00u)
#define  PPR_INT_DIS_MEMSET_DISABLE            (0x01u)

#define _PPR_INT_DIS_MEMSAR_MASK               (0x00020000u)
#define _PPR_INT_DIS_MEMSAR_SHIFT              (17)
#define  PPR_INT_DIS_MEMSAR_DEFAULT            (0x00u)
#define  PPR_INT_DIS_MEMSAR_OF(x)              _CCL_VALUEOF(_PPR_INT_DIS_TYPE,x)
#define  PPR_INT_DIS_MEMSAR_NOP                (0x00u)
#define  PPR_INT_DIS_MEMSAR_DISABLE            (0x01u)

#define _PPR_INT_DIS_PFE_MASK                  (0x00010000u)
#define _PPR_INT_DIS_PFE_SHIFT                 (16)
#define  PPR_INT_DIS_PFE_DEFAULT               (0x00u)
#define  PPR_INT_DIS_PFE_OF(x)                 _CCL_VALUEOF(_PPR_INT_DIS_TYPE,x)
#define  PPR_INT_DIS_PFE_NOP                   (0x00u)
#define  PPR_INT_DIS_PFE_DISABLE               (0x01u)

#define _PPR_INT_DIS_X31_MASK                  (0x00000100u)
#define _PPR_INT_DIS_X31_SHIFT                 (8)
#define  PPR_INT_DIS_X31_DEFAULT               (0x00u)
#define  PPR_INT_DIS_X31_OF(x)                 _CCL_VALUEOF(_PPR_INT_DIS_TYPE,x)
#define  PPR_INT_DIS_X31_NOP                   (0x00u)
#define  PPR_INT_DIS_X31_DISABLE               (0x01u)

#define _PPR_INT_DIS_FB_FUL_MASK               (0x00000080u)
#define _PPR_INT_DIS_FB_FUL_SHIFT              (7)
#define  PPR_INT_DIS_FB_FUL_DEFAULT            (0x00u)
#define  PPR_INT_DIS_FB_FUL_OF(x)              _CCL_VALUEOF(_PPR_INT_DIS_TYPE,x)
#define  PPR_INT_DIS_FB_FUL_NOP                (0x00u)
#define  PPR_INT_DIS_FB_FUL_DISABLE            (0x01u)

#define _PPR_INT_DIS_FB_VALID_MASK             (0x00000040u)
#define _PPR_INT_DIS_FB_VALID_SHIFT            (6)
#define  PPR_INT_DIS_FB_VALID_DEFAULT          (0x00u)
#define  PPR_INT_DIS_FB_VALID_OF(x)            _CCL_VALUEOF(_PPR_INT_DIS_TYPE,x)
#define  PPR_INT_DIS_FB_VALID_NOP              (0x00u)
#define  PPR_INT_DIS_FB_VALID_DISABLE          (0x01u)

#define _PPR_INT_DIS_830F2_MASK                (0x00000020u)
#define _PPR_INT_DIS_830F2_SHIFT               (5)
#define  PPR_INT_DIS_830F2_DEFAULT             (0x00u)
#define  PPR_INT_DIS_830F2_OF(x)               _CCL_VALUEOF(_PPR_INT_DIS_TYPE,x)
#define  PPR_INT_DIS_830F2_NOP                 (0x00u)
#define  PPR_INT_DIS_830F2_DISABLE             (0x01u)

#define _PPR_INT_DIS_830F1_MASK                (0x00000010u)
#define _PPR_INT_DIS_830F1_SHIFT               (4)
#define  PPR_INT_DIS_830F1_DEFAULT             (0x00u)
#define  PPR_INT_DIS_830F1_OF(x)               _CCL_VALUEOF(_PPR_INT_DIS_TYPE,x)
#define  PPR_INT_DIS_830F1_NOP                 (0x00u)
#define  PPR_INT_DIS_830F1_DISABLE             (0x01u)

#define _PPR_INT_DIS_VPS_MASK                  (0x00000008u)
#define _PPR_INT_DIS_VPS_SHIFT                 (3)
#define  PPR_INT_DIS_VPS_DEFAULT               (0x00u)
#define  PPR_INT_DIS_VPS_OF(x)                 _CCL_VALUEOF(_PPR_INT_DIS_TYPE,x)
#define  PPR_INT_DIS_VPS_NOP                   (0x00u)
#define  PPR_INT_DIS_VPS_DISABLE               (0x01u)

#define _PPR_INT_DIS_CGMS_MASK                 (0x00000004u)
#define _PPR_INT_DIS_CGMS_SHIFT                (2)
#define  PPR_INT_DIS_CGMS_DEFAULT              (0x00u)
#define  PPR_INT_DIS_CGMS_OF(x)                _CCL_VALUEOF(_PPR_INT_DIS_TYPE,x)
#define  PPR_INT_DIS_CGMS_NOP                  (0x00u)
#define  PPR_INT_DIS_CGMS_DISABLE              (0x01u)

#define _PPR_INT_DIS_WSS_MASK                  (0x00000002u)
#define _PPR_INT_DIS_WSS_SHIFT                 (1)
#define  PPR_INT_DIS_WSS_DEFAULT               (0x00u)
#define  PPR_INT_DIS_WSS_OF(x)                 _CCL_VALUEOF(_PPR_INT_DIS_TYPE,x)
#define  PPR_INT_DIS_WSS_NOP                   (0x00u)
#define  PPR_INT_DIS_WSS_DISABLE               (0x01u)

#define _PPR_INT_DIS_CC_MASK                   (0x00000001u)
#define _PPR_INT_DIS_CC_SHIFT                  (0)
#define  PPR_INT_DIS_CC_DEFAULT                (0x00u)
#define  PPR_INT_DIS_CC_OF(x)                  _CCL_VALUEOF(_PPR_INT_DIS_TYPE,x)
#define  PPR_INT_DIS_CC_NOP                    (0x00u)
#define  PPR_INT_DIS_CC_DISABLE                (0x01u)


#define  PPR_INT_DIS_OF(x)             _CCL_VALUEOF(_PPR_INT_DIS_TYPE,x)

#define  PPR_INT_DIS_DEFAULT (_PPR_INT_DIS_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,INT_DIS,MASTER   )      \
	|_CCL_PER_FDEFAULT(PPR,INT_DIS,DEBUG_INT)      \
	|_CCL_PER_FDEFAULT(PPR,INT_DIS,MEMSET   )      \
	|_CCL_PER_FDEFAULT(PPR,INT_DIS,MEMSAR   )      \
	|_CCL_PER_FDEFAULT(PPR,INT_DIS,PFE      )      \
	|_CCL_PER_FDEFAULT(PPR,INT_DIS,X31      )      \
	|_CCL_PER_FDEFAULT(PPR,INT_DIS,FB_FUL   )      \
	|_CCL_PER_FDEFAULT(PPR,INT_DIS,FB_VALID )      \
	|_CCL_PER_FDEFAULT(PPR,INT_DIS,830F2    )      \
	|_CCL_PER_FDEFAULT(PPR,INT_DIS,830F1    )      \
	|_CCL_PER_FDEFAULT(PPR,INT_DIS,VPS      )      \
	|_CCL_PER_FDEFAULT(PPR,INT_DIS,CGMS     )      \
	|_CCL_PER_FDEFAULT(PPR,INT_DIS,WSS      )      \
	|_CCL_PER_FDEFAULT(PPR,INT_DIS,CC       )      \
	)

#define PPR_INT_DIS_RMK(master   ,                                   \
                       debug_int, memset  , memsar, pfe    , x31,   \
                       fb_ful   , fb_valid, f2830 , f1830  , vps,   \
                       cgms     , wss     , cc) (_PPR_INT_DIS_TYPE)( \
	 _CCL_PER_FMK  (PPR, INT_DIS, MASTER   , master   )              \
	|_CCL_PER_FMK  (PPR, INT_DIS, DEBUG_INT, debug_int)              \
	|_CCL_PER_FMK  (PPR, INT_DIS, MEMSET   , memset   )              \
	|_CCL_PER_FMK  (PPR, INT_DIS, MEMSAR   , memsar   )              \
	|_CCL_PER_FMK  (PPR, INT_DIS, PFE      , pfe      )              \
	|_CCL_PER_FMK  (PPR, INT_DIS, X31      , x31      )              \
	|_CCL_PER_FMK  (PPR, INT_DIS, FB_FUL   , fb_ful   )              \
	|_CCL_PER_FMK  (PPR, INT_DIS, FB_VALID , fb_valid )              \
	|_CCL_PER_FMK  (PPR, INT_DIS, 830F2    , f2830    )              \
	|_CCL_PER_FMK  (PPR, INT_DIS, 830F1    , f1830    )              \
	|_CCL_PER_FMK  (PPR, INT_DIS, VPS      , vps      )              \
	|_CCL_PER_FMK  (PPR, INT_DIS, CGMS     , cgms     )              \
	|_CCL_PER_FMK  (PPR, INT_DIS, WSS      , wss      )              \
	|_CCL_PER_FMK  (PPR, INT_DIS, CC       , cc       )              \
	)

#define PPR_INT_DIS_RMKS(master  ,                                     \
                         debug_int, memset  , memsar, pfe    , x31,    \
                         fb_ful   , fb_valid, f2830 , f1830  , vps,    \
                         cgms     , wss     , cc) (_PPR_INT_DIS_TYPE)( \
	 _CCL_PER_FMKS (PPR, INT_DIS, MASTER   , master   )                \
	|_CCL_PER_FMKS (PPR, INT_DIS, DEBUG_INT, debug_int)                \
	|_CCL_PER_FMKS (PPR, INT_DIS, MEMSET   , memset   )                \
	|_CCL_PER_FMKS (PPR, INT_DIS, MEMSAR   , memsar   )                \
	|_CCL_PER_FMKS (PPR, INT_DIS, PFE      , pfe      )                \
	|_CCL_PER_FMKS (PPR, INT_DIS, X31      , x31      )                \
	|_CCL_PER_FMKS (PPR, INT_DIS, FB_FUL   , fb_ful   )                \
	|_CCL_PER_FMKS (PPR, INT_DIS, FB_VALID , fb_valid )                \
	|_CCL_PER_FMKS (PPR, INT_DIS, 830F2    , f2830    )                \
	|_CCL_PER_FMKS (PPR, INT_DIS, 830F1    , f1830    )                \
	|_CCL_PER_FMKS (PPR, INT_DIS, VPS      , vps      )                \
	|_CCL_PER_FMKS (PPR, INT_DIS, CGMS     , cgms     )                \
	|_CCL_PER_FMKS (PPR, INT_DIS, WSS      , wss      )                \
	|_CCL_PER_FMKS (PPR, INT_DIS, CC       , cc       )                \
	)

#define PPR_INT_DIS_ALL                                            \
    PPR_INT_DIS_RMKS( DISABLE,                                     \
                      DISABLE, DISABLE, DISABLE, DISABLE, DISABLE, \
                      DISABLE, DISABLE, DISABLE, DISABLE, DISABLE, \
                      DISABLE, DISABLE, DISABLE )

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  DATA_ACQU_ST     |
 * |___________________|
 *
 * DATA_ACQU_ST - Data acquisition status register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  FB_FULL - Field buffer full indicator
 *                FB_FULL  |   description
 *              -----------+---------------------------
 *                  FALSE  |  field buffer is not full. (read)
 *                  TRUE   |  field buffer is full.     (read)
 *                  NOP    |  no operation              (write)
 *                  CLEAR  |  clear TRUE status         (write)
 *
 * (rw)  FB_VALID - Field buffer validity indicator
 *                FB_VALID |   description
 *              -----------+---------------------------
 *                  FALSE  |  field buffer has no valid data.          (read)
 *                  TRUE   |  field buffer has buffered valid TT data. (read)
 *                  NOP    |  no operation                             (write)
 *                  CLEAR  |  clear TRUE status                        (write)
\*-------------------------------------------------------------------------*/
#define _PPR_DATA_ACQU_ST_OFFSET                    (0x0014u)
#define _PPR_DATA_ACQU_ST_ADDRH(h)                   PPR_ADDRH(h,DATA_ACQU_ST)
#define _PPR_DATA_ACQU_ST_LEN                       (32)                 /* 32-bit*/
#define _PPR_DATA_ACQU_ST_TYPE                       uint32_t

#define _PPR_DATA_ACQU_ST_FB_FULL_MASK              (0x00000002u)
#define _PPR_DATA_ACQU_ST_FB_FULL_SHIFT             (0x01u)
#define  PPR_DATA_ACQU_ST_FB_FULL_DEFAULT           (0x00u)
#define  PPR_DATA_ACQU_ST_FB_FULL_OF(x)             _CCL_VALUEOF(_PPR_DATA_ACQU_ST_TYPE,x)
#define  PPR_DATA_ACQU_ST_FB_FULL_FALSE             (0x00u)
#define  PPR_DATA_ACQU_ST_FB_FULL_TRUE              (0x01u)
#define  PPR_DATA_ACQU_ST_FB_FULL_NOP               (0x00u)
#define  PPR_DATA_ACQU_ST_FB_FULL_CLEAR             (0x01u)

#define _PPR_DATA_ACQU_ST_FB_VALID_MASK             (0x00000001u)
#define _PPR_DATA_ACQU_ST_FB_VALID_SHIFT            (0x00u)
#define  PPR_DATA_ACQU_ST_FB_VALID_DEFAULT          (0x00u)
#define  PPR_DATA_ACQU_ST_FB_VALID_OF(x)            _CCL_VALUEOF(_PPR_DATA_ACQU_ST_TYPE,x)
#define  PPR_DATA_ACQU_ST_FB_VALID_FALSE            (0x00u)
#define  PPR_DATA_ACQU_ST_FB_VALID_TRUE             (0x01u)
#define  PPR_DATA_ACQU_ST_FB_VALID_NOP              (0x00u)
#define  PPR_DATA_ACQU_ST_FB_VALID_CLEAR            (0x01u)

#define  PPR_DATA_ACQU_ST_OF(x)                     _CCL_VALUEOF(_PPR_DATA_ACQU_ST_TYPE,x)

#define  PPR_DATA_ACQU_ST_DEFAULT       (_PPR_DATA_ACQU_ST_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,DATA_ACQU_ST,FB_FULL)                   \
	|_CCL_PER_FDEFAULT(PPR,DATA_ACQU_ST,FB_VALID)                  \
	)

#define PPR_DATA_ACQU_ST_RMK(fb_full, fb_valid) (_PPR_DATA_ACQU_ST_TYPE)(  \
	 _CCL_PER_FMK  (PPR,DATA_ACQU_ST,FB_FULL, fb_full)                     \
	|_CCL_PER_FMK  (PPR,DATA_ACQU_ST,FB_VALID, fb_valid)                   \
	)

#define PPR_DATA_ACQU_ST_RMKS(fb_full, fb_valid) (_PPR_DATA_ACQU_ST_TYPE)(  \
	 _CCL_PER_FMKS (PPR,DATA_ACQU_ST,FB_FULL, fb_full)                     \
	|_CCL_PER_FMKS (PPR,DATA_ACQU_ST,FB_VALID, fb_valid)                   \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  PKT_RX_CNT       |
 * |___________________|
 *
 * PKT_RX_CNT - Packet Receive Counter register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  RST - Reset
 *                  RST    |   description
 *              -----------+---------------------------
 *                  NOP    |  no operation              (write)
 *                  RST    |  clear receive counter     (write)
 *
 * (r)   CNT - receive counter
\*-------------------------------------------------------------------------*/
#define _PPR_PKT_RX_CNT_OFFSET                    (0x003Cu)
#define _PPR_PKT_RX_CNT_ADDRH(h)                   PPR_ADDRH(h,PKT_RX_CNT)
#define _PPR_PKT_RX_CNT_LEN                       (32)                 /* 32-bit*/
#define _PPR_PKT_RX_CNT_TYPE                       uint32_t

#define _PPR_PKT_RX_CNT_RST_MASK                  (0x80000000u)
#define _PPR_PKT_RX_CNT_RST_SHIFT                 (31)
#define  PPR_PKT_RX_CNT_RST_DEFAULT               (0x0u)
#define  PPR_PKT_RX_CNT_RST_OF(x)                 _CCL_VALUEOF(_PPR_PKT_RX_CNT_TYPE,x)
#define  PPR_PKT_RX_CNT_RST_NOP                   (0x00u)
#define  PPR_PKT_RX_CNT_RST_RST                   (0x01u)

#define _PPR_PKT_RX_CNT_CNT_MASK                  (0x0000FFFFu)
#define _PPR_PKT_RX_CNT_CNT_SHIFT                 (0)
#define  PPR_PKT_RX_CNT_CNT_DEFAULT               (0x0000u)
#define  PPR_PKT_RX_CNT_CNT_OF(x)                 _CCL_VALUEOF(_PPR_PKT_RX_CNT_TYPE,x)

#define  PPR_PKT_RX_CNT_OF(x)                     _CCL_VALUEOF(_PPR_PKT_RX_CNT_TYPE,x)

#define  PPR_PKT_RX_CNT_DEFAULT       (_PPR_PKT_RX_CNT_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,PKT_RX_CNT,RST)                     \
	|_CCL_PER_FDEFAULT(PPR,PKT_RX_CNT,CNT)                     \
	)

#define PPR_PKT_RX_CNT_RMK(rst) (_PPR_PKT_RX_CNT_TYPE)(  \
	 _CCL_PER_FMK  (PPR,PKT_RX_CNT,RST,rst)              \
	)

#define PPR_PKT_RX_CNT_RMKS(rst) (_PPR_PKT_RX_CNT_TYPE)(  \
	 _CCL_PER_FMKS (PPR,PKT_RX_CNT,RST,rst)               \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  PKT_ERR_CNT      |
 * |___________________|
 *
 * PKT_ERR_CNT - Packet Receive Error Counter register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  RST - Reset
 *                  RST    |   description
 *              -----------+---------------------------
 *                  NOP    |  no operation              (write)
 *                  RST    |  clear receive counter     (write)
 *
 * (r)   CNT - receive counter
\*-------------------------------------------------------------------------*/
#define _PPR_PKT_ERR_CNT_OFFSET                    (0x0040u)
#define _PPR_PKT_ERR_CNT_ADDRH(h)                   PPR_ADDRH(h,PKT_ERR_CNT)
#define _PPR_PKT_ERR_CNT_LEN                       (32)                 /* 32-bit*/
#define _PPR_PKT_ERR_CNT_TYPE                       uint32_t

#define _PPR_PKT_ERR_CNT_RST_MASK                  (0x80000000u)
#define _PPR_PKT_ERR_CNT_RST_SHIFT                 (31)
#define  PPR_PKT_ERR_CNT_RST_DEFAULT               (0x0u)
#define  PPR_PKT_ERR_CNT_RST_OF(x)                 _CCL_VALUEOF(_PPR_PKT_ERR_CNT_TYPE,x)
#define  PPR_PKT_ERR_CNT_RST_NOP                   (0x00u)
#define  PPR_PKT_ERR_CNT_RST_RST                   (0x01u)

#define _PPR_PKT_ERR_CNT_CNT_MASK                  (0x0000FFFFu)
#define _PPR_PKT_ERR_CNT_CNT_SHIFT                 (0)
#define  PPR_PKT_ERR_CNT_CNT_DEFAULT               (0x0000u)
#define  PPR_PKT_ERR_CNT_CNT_OF(x)                 _CCL_VALUEOF(_PPR_PKT_ERR_CNT_TYPE,x)

#define  PPR_PKT_ERR_CNT_OF(x)                     _CCL_VALUEOF(_PPR_PKT_ERR_CNT_TYPE,x)

#define  PPR_PKT_ERR_CNT_DEFAULT       (_PPR_PKT_ERR_CNT_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,PKT_ERR_CNT,RST)                     \
	|_CCL_PER_FDEFAULT(PPR,PKT_ERR_CNT,CNT)                     \
	)

#define PPR_PKT_ERR_CNT_RMK(rst) (_PPR_PKT_ERR_CNT_TYPE)(  \
	 _CCL_PER_FMK  (PPR,PKT_ERR_CNT,RST,rst)              \
	)

#define PPR_PKT_ERR_CNT_RMKS(rst) (_PPR_PKT_ERR_CNT_TYPE)(  \
	 _CCL_PER_FMKS (PPR,PKT_ERR_CNT,RST,rst)               \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  BFILTER_CTRL     |
 * |___________________|
 *
 * BFILTER_CTRL - B-filter control register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  MAGPKTDC - Discard magazine packet if hamming error
 *                MAGPKTDC |   description
 *              -----------+-------------------
 *                   KEEP  |  keep error packet
 *                 DISCARD |  discard error packet
\*-------------------------------------------------------------------------*/
#define _PPR_BFILTER_CTRL_OFFSET                    (0x00ACu)
#define _PPR_BFILTER_CTRL_ADDRH(h)                   PPR_ADDRH(h,BFILTER_CTRL)
#define _PPR_BFILTER_CTRL_LEN                       (32)                 /* 32-bit*/
#define _PPR_BFILTER_CTRL_TYPE                       uint32_t

#define _PPR_BFILTER_CTRL_MAGPKTDC_MASK             (0x00000001u)
#define _PPR_BFILTER_CTRL_MAGPKTDC_SHIFT            (0x00u)
#define  PPR_BFILTER_CTRL_MAGPKTDC_DEFAULT          (0x00u)
#define  PPR_BFILTER_CTRL_MAGPKTDC_OF(x)            _CCL_VALUEOF(_PPR_BFILTER_CTRL_TYPE,x)
#define  PPR_BFILTER_CTRL_MAGPKTDC_KEEP             (0x00u)
#define  PPR_BFILTER_CTRL_MAGPKTDC_DISCARD          (0x01u)

#define  PPR_BFILTER_CTRL_OF(x)                     _CCL_VALUEOF(_PPR_BFILTER_CTRL_TYPE,x)

#define  PPR_BFILTER_CTRL_DEFAULT       (_PPR_BFILTER_CTRL_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,BFILTER_CTRL,MAGPKTDC)                  \
	)

#define PPR_BFILTER_CTRL_RMK(magpktdc) (_PPR_BFILTER_CTRL_TYPE)(  \
	 _CCL_PER_FMK  (PPR,BFILTER_CTRL,MAGPKTDC, magpktdc)          \
	)

#define PPR_BFILTER_CTRL_RMKS(magpktdc) (_PPR_BFILTER_CTRL_TYPE)(  \
	 _CCL_PER_FMKS (PPR,BFILTER_CTRL,MAGPKTDC, magpktdc)           \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  F B   I N D      |
 * |___________________|
 *
 * FB_IND - Field buffer indicator register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (r)   LAST_PKT_ADDR - the end address of the last packet in VBI field buffer.
 *                       It is 4-byte basis.
 *
 * (rw)  PKT_X0_IND - Indicates which packet is X/0 in VBI field buffer (18 packets).
\*-------------------------------------------------------------------------*/
#define _PPR_FB_IND_OFFSET                    (0x00B0u)
#define _PPR_FB_IND_ADDRH(h)                   PPR_ADDRH(h,FB_IND)
#define _PPR_FB_IND_LEN                       (32)                 /* 32-bit*/
#define _PPR_FB_IND_TYPE                       uint32_t

#define _PPR_FB_IND_LAST_PKT_ADDR_MASK        (0xFF000000u)
#define _PPR_FB_IND_LAST_PKT_ADDR_SHIFT       (24)
#define  PPR_FB_IND_LAST_PKT_ADDR_DEFAULT     (0x00u)
#define  PPR_FB_IND_LAST_PKT_ADDR_OF(x)       _CCL_VALUEOF(_PPR_FB_IND_TYPE,x)

#define _PPR_FB_IND_PKT_X0_IND_MASK           (0x0003FFFFu)
#define _PPR_FB_IND_PKT_X0_IND_SHIFT          (0x00u)
#define  PPR_FB_IND_PKT_X0_IND_DEFAULT        (0x00u)
#define  PPR_FB_IND_PKT_X0_IND_OF(x)          _CCL_VALUEOF(_PPR_FB_IND_TYPE,x)

#define  PPR_FB_IND_OF(x)                     _CCL_VALUEOF(_PPR_FB_IND_TYPE,x)

#define  PPR_FB_IND_DEFAULT       (_PPR_FB_IND_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,FB_IND,LAST_PKT_ADDR)       \
	|_CCL_PER_FDEFAULT(PPR,FB_IND,PKT_X0_IND   )       \
	)

#define PPR_FB_IND_RMK(last_pkt_addr, pkt_x0_ind) (_PPR_FB_IND_TYPE)(  \
	 _CCL_PER_FMK  (PPR,FB_IND,LAST_PKT_ADDR, last_pkt_addr)           \
	|_CCL_PER_FMK  (PPR,FB_IND,PKT_X0_IND   , pkt_x0_ind   )           \
	)



/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  M A G 1          |
 * |___________________|
 *
 * MAG1 - Magazine-1
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  DES_BADDR - Base Address of Destination (in 8-byte basis)
\*-------------------------------------------------------------------------*/
#define _PPR_MAG1_OFFSET                    (0x00B8u)
#define _PPR_MAG1_ADDRH(h)                   PPR_ADDRH(h,MAG1)
#define _PPR_MAG1_LEN                       (32)                 /* 32-bit*/
#define _PPR_MAG1_TYPE                       uint32_t

#define _PPR_MAG1_DES_BADDR_MASK            (0x003FFFFFu)
#define _PPR_MAG1_DES_BADDR_SHIFT           (0x00u)
#define  PPR_MAG1_DES_BADDR_DEFAULT         (0x00u)
#define  PPR_MAG1_DES_BADDR_OF(x)           _CCL_VALUEOF(_PPR_MAG1_TYPE,x)

#define  PPR_MAG1_OF(x)                     _CCL_VALUEOF(_PPR_MAG1_TYPE,x)

#define  PPR_MAG1_DEFAULT       (_PPR_MAG1_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,MAG1,DES_BADDR)         \
	)

#define PPR_MAG1_RMK(des_baddr) (_PPR_MAG1_TYPE)(  \
	 _CCL_PER_FMK  (PPR,MAG1,DES_BADDR, des_baddr) \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  M A G 2          |
 * |___________________|
 *
 * MAG2 - Magazine-2
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  DES_BADDR - Base Address of Destination (in 8-byte basis)
\*-------------------------------------------------------------------------*/
#define _PPR_MAG2_OFFSET                    (0x00BCu)
#define _PPR_MAG2_ADDRH(h)                   PPR_ADDRH(h,MAG2)
#define _PPR_MAG2_LEN                       (32)                 /* 32-bit*/
#define _PPR_MAG2_TYPE                       uint32_t

#define _PPR_MAG2_DES_BADDR_MASK            (0x003FFFFFu)
#define _PPR_MAG2_DES_BADDR_SHIFT           (0x00u)
#define  PPR_MAG2_DES_BADDR_DEFAULT         (0x00u)
#define  PPR_MAG2_DES_BADDR_OF(x)           _CCL_VALUEOF(_PPR_MAG2_TYPE,x)

#define  PPR_MAG2_OF(x)                     _CCL_VALUEOF(_PPR_MAG2_TYPE,x)

#define  PPR_MAG2_DEFAULT       (_PPR_MAG2_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,MAG2,DES_BADDR)         \
	)

#define PPR_MAG2_RMK(des_baddr) (_PPR_MAG2_TYPE)(  \
	 _CCL_PER_FMK  (PPR,MAG2,DES_BADDR, des_baddr) \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  M A G 3          |
 * |___________________|
 *
 * MAG3 - Magazine-3
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  DES_BADDR - Base Address of Destination (in 8-byte basis)
\*-------------------------------------------------------------------------*/
#define _PPR_MAG3_OFFSET                    (0x00C0u)
#define _PPR_MAG3_ADDRH(h)                   PPR_ADDRH(h,MAG3)
#define _PPR_MAG3_LEN                       (32)                 /* 32-bit*/
#define _PPR_MAG3_TYPE                       uint32_t

#define _PPR_MAG3_DES_BADDR_MASK            (0x003FFFFFu)
#define _PPR_MAG3_DES_BADDR_SHIFT           (0x00u)
#define  PPR_MAG3_DES_BADDR_DEFAULT         (0x00u)
#define  PPR_MAG3_DES_BADDR_OF(x)           _CCL_VALUEOF(_PPR_MAG3_TYPE,x)

#define  PPR_MAG3_OF(x)                     _CCL_VALUEOF(_PPR_MAG3_TYPE,x)

#define  PPR_MAG3_DEFAULT       (_PPR_MAG3_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,MAG3,DES_BADDR)         \
	)

#define PPR_MAG3_RMK(des_baddr) (_PPR_MAG3_TYPE)(  \
	 _CCL_PER_FMK  (PPR,MAG3,DES_BADDR, des_baddr) \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  M A G 4          |
 * |___________________|
 *
 * MAG4 - Magazine-4
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  DES_BADDR - Base Address of Destination (in 8-byte basis)
\*-------------------------------------------------------------------------*/
#define _PPR_MAG4_OFFSET                    (0x00C4u)
#define _PPR_MAG4_ADDRH(h)                   PPR_ADDRH(h,MAG4)
#define _PPR_MAG4_LEN                       (32)                 /* 32-bit*/
#define _PPR_MAG4_TYPE                       uint32_t

#define _PPR_MAG4_DES_BADDR_MASK            (0x003FFFFFu)
#define _PPR_MAG4_DES_BADDR_SHIFT           (0x00u)
#define  PPR_MAG4_DES_BADDR_DEFAULT         (0x00u)
#define  PPR_MAG4_DES_BADDR_OF(x)           _CCL_VALUEOF(_PPR_MAG4_TYPE,x)

#define  PPR_MAG4_OF(x)                     _CCL_VALUEOF(_PPR_MAG4_TYPE,x)

#define  PPR_MAG4_DEFAULT       (_PPR_MAG4_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,MAG4,DES_BADDR)         \
	)

#define PPR_MAG4_RMK(des_baddr) (_PPR_MAG4_TYPE)(  \
	 _CCL_PER_FMK  (PPR,MAG4,DES_BADDR, des_baddr) \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  M A G 5          |
 * |___________________|
 *
 * MAG5 - Magazine-5
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  DES_BADDR - Base Address of Destination (in 8-byte basis)
\*-------------------------------------------------------------------------*/
#define _PPR_MAG5_OFFSET                    (0x00C8u)
#define _PPR_MAG5_ADDRH(h)                   PPR_ADDRH(h,MAG5)
#define _PPR_MAG5_LEN                       (32)                 /* 32-bit*/
#define _PPR_MAG5_TYPE                       uint32_t

#define _PPR_MAG5_DES_BADDR_MASK            (0x003FFFFFu)
#define _PPR_MAG5_DES_BADDR_SHIFT           (0x00u)
#define  PPR_MAG5_DES_BADDR_DEFAULT         (0x00u)
#define  PPR_MAG5_DES_BADDR_OF(x)           _CCL_VALUEOF(_PPR_MAG5_TYPE,x)

#define  PPR_MAG5_OF(x)                     _CCL_VALUEOF(_PPR_MAG5_TYPE,x)

#define  PPR_MAG5_DEFAULT       (_PPR_MAG5_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,MAG5,DES_BADDR)         \
	)

#define PPR_MAG5_RMK(des_baddr) (_PPR_MAG5_TYPE)(  \
	 _CCL_PER_FMK  (PPR,MAG5,DES_BADDR, des_baddr) \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  M A G 6          |
 * |___________________|
 *
 * MAG6 - Magazine-6
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  DES_BADDR - Base Address of Destination (in 8-byte basis)
\*-------------------------------------------------------------------------*/
#define _PPR_MAG6_OFFSET                    (0x00CCu)
#define _PPR_MAG6_ADDRH(h)                   PPR_ADDRH(h,MAG6)
#define _PPR_MAG6_LEN                       (32)                 /* 32-bit*/
#define _PPR_MAG6_TYPE                       uint32_t

#define _PPR_MAG6_DES_BADDR_MASK            (0x003FFFFFu)
#define _PPR_MAG6_DES_BADDR_SHIFT           (0x00u)
#define  PPR_MAG6_DES_BADDR_DEFAULT         (0x00u)
#define  PPR_MAG6_DES_BADDR_OF(x)           _CCL_VALUEOF(_PPR_MAG6_TYPE,x)

#define  PPR_MAG6_OF(x)                     _CCL_VALUEOF(_PPR_MAG6_TYPE,x)

#define  PPR_MAG6_DEFAULT       (_PPR_MAG6_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,MAG6,DES_BADDR)         \
	)

#define PPR_MAG6_RMK(des_baddr) (_PPR_MAG6_TYPE)(  \
	 _CCL_PER_FMK  (PPR,MAG6,DES_BADDR, des_baddr) \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  M A G 7          |
 * |___________________|
 *
 * MAG7 - Magazine-7
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  DES_BADDR - Base Address of Destination (in 8-byte basis)
\*-------------------------------------------------------------------------*/
#define _PPR_MAG7_OFFSET                    (0x00D0u)
#define _PPR_MAG7_ADDRH(h)                   PPR_ADDRH(h,MAG7)
#define _PPR_MAG7_LEN                       (32)                 /* 32-bit*/
#define _PPR_MAG7_TYPE                       uint32_t

#define _PPR_MAG7_DES_BADDR_MASK            (0x003FFFFFu)
#define _PPR_MAG7_DES_BADDR_SHIFT           (0x00u)
#define  PPR_MAG7_DES_BADDR_DEFAULT         (0x00u)
#define  PPR_MAG7_DES_BADDR_OF(x)           _CCL_VALUEOF(_PPR_MAG7_TYPE,x)

#define  PPR_MAG7_OF(x)                     _CCL_VALUEOF(_PPR_MAG7_TYPE,x)

#define  PPR_MAG7_DEFAULT       (_PPR_MAG7_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,MAG7,DES_BADDR)         \
	)

#define PPR_MAG7_RMK(des_baddr) (_PPR_MAG7_TYPE)(  \
	 _CCL_PER_FMK  (PPR,MAG7,DES_BADDR, des_baddr) \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  M A G 8          |
 * |___________________|
 *
 * MAG8 - Magazine-8
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  DES_BADDR - Base Address of Destination (in 8-byte basis)
\*-------------------------------------------------------------------------*/
#define _PPR_MAG8_OFFSET                    (0x00D4u)
#define _PPR_MAG8_ADDRH(h)                   PPR_ADDRH(h,MAG8)
#define _PPR_MAG8_LEN                       (32)                 /* 32-bit*/
#define _PPR_MAG8_TYPE                       uint32_t

#define _PPR_MAG8_DES_BADDR_MASK            (0x003FFFFFu)
#define _PPR_MAG8_DES_BADDR_SHIFT           (0x00u)
#define  PPR_MAG8_DES_BADDR_DEFAULT         (0x00u)
#define  PPR_MAG8_DES_BADDR_OF(x)           _CCL_VALUEOF(_PPR_MAG8_TYPE,x)

#define  PPR_MAG8_OF(x)                     _CCL_VALUEOF(_PPR_MAG8_TYPE,x)

#define  PPR_MAG8_DEFAULT       (_PPR_MAG8_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,MAG8,DES_BADDR)         \
	)

#define PPR_MAG8_RMK(des_baddr) (_PPR_MAG8_TYPE)(  \
	 _CCL_PER_FMK  (PPR,MAG8,DES_BADDR, des_baddr) \
	)


/* MAGx (x=1,2,3,4,5,6,7,8) MACROS */

#define _PPR_MAG_FGET(N,FIELD)\
	_CCL_PER_FGET(_PPR_MAG##N##_ADDR,PPR,MAG##N,FIELD)

#define _PPR_MAG_FSET(N,FIELD,field)\
	_CCL_PER_FSET(_PPR_MAG##N##_ADDR,PPR,MAG##N,FIELD,field)

#define _PPR_MAG1_FGET(FIELD)      _PPR_MAG_FGET(1,FIELD)
#define _PPR_MAG2_FGET(FIELD)      _PPR_MAG_FGET(2,FIELD)
#define _PPR_MAG3_FGET(FIELD)      _PPR_MAG_FGET(3,FIELD)
#define _PPR_MAG4_FGET(FIELD)      _PPR_MAG_FGET(4,FIELD)
#define _PPR_MAG5_FGET(FIELD)      _PPR_MAG_FGET(5,FIELD)
#define _PPR_MAG6_FGET(FIELD)      _PPR_MAG_FGET(6,FIELD)
#define _PPR_MAG7_FGET(FIELD)      _PPR_MAG_FGET(7,FIELD)
#define _PPR_MAG8_FGET(FIELD)      _PPR_MAG_FGET(8,FIELD)

#define _PPR_MAG1_FSET(FIELD,f)    _PPR_MAG_FSET(1,FIELD,f)
#define _PPR_MAG2_FSET(FIELD,f)    _PPR_MAG_FSET(2,FIELD,f)
#define _PPR_MAG3_FSET(FIELD,f)    _PPR_MAG_FSET(3,FIELD,f)
#define _PPR_MAG4_FSET(FIELD,f)    _PPR_MAG_FSET(4,FIELD,f)
#define _PPR_MAG5_FSET(FIELD,f)    _PPR_MAG_FSET(5,FIELD,f)
#define _PPR_MAG6_FSET(FIELD,f)    _PPR_MAG_FSET(6,FIELD,f)
#define _PPR_MAG7_FSET(FIELD,f)    _PPR_MAG_FSET(7,FIELD,f)
#define _PPR_MAG8_FSET(FIELD,f)    _PPR_MAG_FSET(8,FIELD,f)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  B P K T          |
 * |___________________|
 *
 * BPKT - Broadcasting Packet (M/29)
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  DES_BADDR - Base Address of Destination (in 8-byte basis)
\*-------------------------------------------------------------------------*/
#define _PPR_BPKT_OFFSET                    (0x00D8u)
#define _PPR_BPKT_ADDRH(h)                   PPR_ADDRH(h,BPKT)
#define _PPR_BPKT_LEN                       (32)                 /* 32-bit*/
#define _PPR_BPKT_TYPE                       uint32_t

#define _PPR_BPKT_DES_BADDR_MASK            (0x003FFFFFu)
#define _PPR_BPKT_DES_BADDR_SHIFT           (0x00u)
#define  PPR_BPKT_DES_BADDR_DEFAULT         (0x00u)
#define  PPR_BPKT_DES_BADDR_OF(x)           _CCL_VALUEOF(_PPR_BPKT_TYPE,x)

#define  PPR_BPKT_OF(x)                     _CCL_VALUEOF(_PPR_BPKT_TYPE,x)

#define  PPR_BPKT_DEFAULT       (_PPR_BPKT_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,BPKT,DES_BADDR)         \
	)

#define PPR_BPKT_RMK(des_baddr) (_PPR_BPKT_TYPE)(  \
	 _CCL_PER_FMK  (PPR,BPKT,DES_BADDR, des_baddr) \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  PFE  A C T I V E |
 * |___________________|
 *
 * PFE_ACTIVE - Prefetch Engine Activation Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (r)  BPKT - Broadcast Packet Activation
 *                  BPKT   |   description
 *              -----------+----------------------
 *                INACTIVE | M29 is in-active
 *                  ACTIVE | M29 is active
 *
 * (r)  MAG8 -
 *                  MAG8   |   description
 *              -----------+----------------------
 *                INACTIVE | Magazine-8 is in-active
 *                  ACTIVE | Magazine-8 is active
 *
 * (r)  MAG7 -
 *                  MAG7   |   description
 *              -----------+----------------------
 *                INACTIVE | Magazine-7 is in-active
 *                  ACTIVE | Magazine-7 is active
 *
 * (r)  MAG6 -
 *                  MAG6   |   description
 *              -----------+----------------------
 *                INACTIVE | Magazine-6 is in-active
 *                  ACTIVE | Magazine-6 is active
 *
 * (r)  MAG5 -
 *                  MAG5   |   description
 *              -----------+----------------------
 *                INACTIVE | Magazine-5 is in-active
 *                  ACTIVE | Magazine-5 is active
 *
 * (r)  MAG4 -
 *                  MAG4   |   description
 *              -----------+----------------------
 *                INACTIVE | Magazine-4 is in-active
 *                  ACTIVE | Magazine-4 is active
 *
 * (r)  MAG3 -
 *                  MAG3   |   description
 *              -----------+----------------------
 *                INACTIVE | Magazine-3 is in-active
 *                  ACTIVE | Magazine-3 is active
 *
 * (r)  MAG2 -
 *                  MAG2   |   description
 *              -----------+----------------------
 *                INACTIVE | Magazine-2 is in-active
 *                  ACTIVE | Magazine-2 is active
 *
 * (r)  MAG1 -
 *                  MAG1   |   description
 *              -----------+----------------------
 *                INACTIVE | Magazine-1 is in-active
 *                  ACTIVE | Magazine-1 is active
\*-------------------------------------------------------------------------*/
#define _PPR_PFE_ACTIVE_OFFSET                    (0x00DCu)
#define _PPR_PFE_ACTIVE_ADDRH(h)                  PPR_ADDRH(h,PFE_ACTIVE)
#define _PPR_PFE_ACTIVE_LEN                       (32)                 /* 32-bit*/
#define _PPR_PFE_ACTIVE_TYPE                      uint32_t

#define _PPR_PFE_ACTIVE_BPKT_MASK                 (0x00000100u)
#define _PPR_PFE_ACTIVE_BPKT_SHIFT                (8)
#define  PPR_PFE_ACTIVE_BPKT_DEFAULT              (0x00u)
#define  PPR_PFE_ACTIVE_BPKT_OF(x)                _CCL_VALUEOF(_PPR_PFE_ACTIVE_TYPE,x)
#define  PPR_PFE_ACTIVE_BPKT_INACTIVE             (0x00u)
#define  PPR_PFE_ACTIVE_BPKT_ACTIVE               (0x01u)

#define _PPR_PFE_ACTIVE_MAG8_MASK                 (0x00000080u)
#define _PPR_PFE_ACTIVE_MAG8_SHIFT                (7)
#define  PPR_PFE_ACTIVE_MAG8_DEFAULT              (0x00u)
#define  PPR_PFE_ACTIVE_MAG8_OF(x)                _CCL_VALUEOF(_PPR_PFE_ACTIVE_TYPE,x)
#define  PPR_PFE_ACTIVE_MAG8_INACTIVE             (0x00u)
#define  PPR_PFE_ACTIVE_MAG8_ACTIVE               (0x01u)

#define _PPR_PFE_ACTIVE_MAG7_MASK                 (0x00000040u)
#define _PPR_PFE_ACTIVE_MAG7_SHIFT                (6)
#define  PPR_PFE_ACTIVE_MAG7_DEFAULT              (0x00u)
#define  PPR_PFE_ACTIVE_MAG7_OF(x)                _CCL_VALUEOF(_PPR_PFE_ACTIVE_TYPE,x)
#define  PPR_PFE_ACTIVE_MAG7_INACTIVE             (0x00u)
#define  PPR_PFE_ACTIVE_MAG7_ACTIVE               (0x01u)

#define _PPR_PFE_ACTIVE_MAG6_MASK                 (0x00000020u)
#define _PPR_PFE_ACTIVE_MAG6_SHIFT                (5)
#define  PPR_PFE_ACTIVE_MAG6_DEFAULT              (0x00u)
#define  PPR_PFE_ACTIVE_MAG6_OF(x)                _CCL_VALUEOF(_PPR_PFE_ACTIVE_TYPE,x)
#define  PPR_PFE_ACTIVE_MAG6_INACTIVE             (0x00u)
#define  PPR_PFE_ACTIVE_MAG6_ACTIVE               (0x01u)

#define _PPR_PFE_ACTIVE_MAG5_MASK                 (0x00000010u)
#define _PPR_PFE_ACTIVE_MAG5_SHIFT                (4)
#define  PPR_PFE_ACTIVE_MAG5_DEFAULT              (0x00u)
#define  PPR_PFE_ACTIVE_MAG5_OF(x)                _CCL_VALUEOF(_PPR_PFE_ACTIVE_TYPE,x)
#define  PPR_PFE_ACTIVE_MAG5_INACTIVE             (0x00u)
#define  PPR_PFE_ACTIVE_MAG5_ACTIVE               (0x01u)

#define _PPR_PFE_ACTIVE_MAG4_MASK                 (0x00000008u)
#define _PPR_PFE_ACTIVE_MAG4_SHIFT                (3)
#define  PPR_PFE_ACTIVE_MAG4_DEFAULT              (0x00u)
#define  PPR_PFE_ACTIVE_MAG4_OF(x)                _CCL_VALUEOF(_PPR_PFE_ACTIVE_TYPE,x)
#define  PPR_PFE_ACTIVE_MAG4_INACTIVE             (0x00u)
#define  PPR_PFE_ACTIVE_MAG4_ACTIVE               (0x01u)

#define _PPR_PFE_ACTIVE_MAG3_MASK                 (0x00000004u)
#define _PPR_PFE_ACTIVE_MAG3_SHIFT                (2)
#define  PPR_PFE_ACTIVE_MAG3_DEFAULT              (0x00u)
#define  PPR_PFE_ACTIVE_MAG3_OF(x)                _CCL_VALUEOF(_PPR_PFE_ACTIVE_TYPE,x)
#define  PPR_PFE_ACTIVE_MAG3_INACTIVE             (0x00u)
#define  PPR_PFE_ACTIVE_MAG3_ACTIVE               (0x01u)

#define _PPR_PFE_ACTIVE_MAG2_MASK                 (0x00000002u)
#define _PPR_PFE_ACTIVE_MAG2_SHIFT                (1)
#define  PPR_PFE_ACTIVE_MAG2_DEFAULT              (0x00u)
#define  PPR_PFE_ACTIVE_MAG2_OF(x)                _CCL_VALUEOF(_PPR_PFE_ACTIVE_TYPE,x)
#define  PPR_PFE_ACTIVE_MAG2_INACTIVE             (0x00u)
#define  PPR_PFE_ACTIVE_MAG2_ACTIVE               (0x01u)

#define _PPR_PFE_ACTIVE_MAG1_MASK                 (0x00000001u)
#define _PPR_PFE_ACTIVE_MAG1_SHIFT                (0)
#define  PPR_PFE_ACTIVE_MAG1_DEFAULT              (0x00u)
#define  PPR_PFE_ACTIVE_MAG1_OF(x)                _CCL_VALUEOF(_PPR_PFE_ACTIVE_TYPE,x)
#define  PPR_PFE_ACTIVE_MAG1_INACTIVE             (0x00u)
#define  PPR_PFE_ACTIVE_MAG1_ACTIVE               (0x01u)

#define  PPR_PFE_ACTIVE_OF(x)             _CCL_VALUEOF(_PPR_PFE_ACTIVE_TYPE,x)

#define  PPR_PFE_ACTIVE_DEFAULT (_PPR_PFE_ACTIVE_TYPE)(   \
	 _CCL_PER_FDEFAULT(PPR,PFE_ACTIVE,BPKT)               \
	|_CCL_PER_FDEFAULT(PPR,PFE_ACTIVE,MAG8)               \
	|_CCL_PER_FDEFAULT(PPR,PFE_ACTIVE,MAG7)               \
	|_CCL_PER_FDEFAULT(PPR,PFE_ACTIVE,MAG6)               \
	|_CCL_PER_FDEFAULT(PPR,PFE_ACTIVE,MAG5)               \
	|_CCL_PER_FDEFAULT(PPR,PFE_ACTIVE,MAG4)               \
	|_CCL_PER_FDEFAULT(PPR,PFE_ACTIVE,MAG3)               \
	|_CCL_PER_FDEFAULT(PPR,PFE_ACTIVE,MAG2)               \
	|_CCL_PER_FDEFAULT(PPR,PFE_ACTIVE,MAG1)               \
	)

#define PPR_PFE_ACTIVE_RMK(bpkt, mag8, mag7, mag6, mag5, mag4, mag3, mag2, mag1) (_PPR_PFE_ACTIVE_TYPE)( \
	 _CCL_PER_FMK  (PPR,PFE_ACTIVE,BPKT,bpkt)                     \
	|_CCL_PER_FMK  (PPR,PFE_ACTIVE,MAG8,mag8)                     \
	|_CCL_PER_FMK  (PPR,PFE_ACTIVE,MAG7,mag7)                     \
	|_CCL_PER_FMK  (PPR,PFE_ACTIVE,MAG6,mag6)                     \
	|_CCL_PER_FMK  (PPR,PFE_ACTIVE,MAG5,mag5)                     \
	|_CCL_PER_FMK  (PPR,PFE_ACTIVE,MAG4,mag4)                     \
	|_CCL_PER_FMK  (PPR,PFE_ACTIVE,MAG3,mag3)                     \
	|_CCL_PER_FMK  (PPR,PFE_ACTIVE,MAG2,mag2)                     \
	|_CCL_PER_FMK  (PPR,PFE_ACTIVE,MAG1,mag1)                     \
	)

#define PPR_PFE_ACTIVE_RMKS(bpkt, mag8, mag7, mag6, mag5, mag4, mag3, mag2, mag1) (_PPR_PFE_ACTIVE_TYPE)( \
	 _CCL_PER_FMKS (PPR,PFE_ACTIVE,BPKT,bpkt)                     \
	|_CCL_PER_FMKS (PPR,PFE_ACTIVE,MAG8,mag8)                     \
	|_CCL_PER_FMKS (PPR,PFE_ACTIVE,MAG7,mag7)                     \
	|_CCL_PER_FMKS (PPR,PFE_ACTIVE,MAG6,mag6)                     \
	|_CCL_PER_FMKS (PPR,PFE_ACTIVE,MAG5,mag5)                     \
	|_CCL_PER_FMKS (PPR,PFE_ACTIVE,MAG4,mag4)                     \
	|_CCL_PER_FMKS (PPR,PFE_ACTIVE,MAG3,mag3)                     \
	|_CCL_PER_FMKS (PPR,PFE_ACTIVE,MAG2,mag2)                     \
	|_CCL_PER_FMKS (PPR,PFE_ACTIVE,MAG1,mag1)                     \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  PFE_NPROC_ADDR   |
 * |___________________|
 *
 * PFE_NPROC_ADDR - Next Process Address
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  ADDR - next process address of moving DMA of PFEMOVENG. It's 4-byte basis.
\*-------------------------------------------------------------------------*/
#define _PPR_PFE_NPROC_ADDR_OFFSET                    (0x00E0u)
#define _PPR_PFE_NPROC_ADDR_ADDRH(h)                   PPR_ADDRH(h,PFE_NPROC_ADDR)
#define _PPR_PFE_NPROC_ADDR_LEN                       (32)                 /* 32-bit*/
#define _PPR_PFE_NPROC_ADDR_TYPE                       uint32_t

#define _PPR_PFE_NPROC_ADDR_ADDR_MASK                 (0x000000FFu)
#define _PPR_PFE_NPROC_ADDR_ADDR_SHIFT                (0x00u)
#define  PPR_PFE_NPROC_ADDR_ADDR_DEFAULT              (0x00u)
#define  PPR_PFE_NPROC_ADDR_ADDR_OF(x)                _CCL_VALUEOF(_PPR_PFE_NPROC_ADDR_TYPE,x)
#define  PPR_PFE_NPROC_ADDR_ADDR_PKT(x)               (0x00u+10*x)   /* x from 0~17 */

#define  PPR_PFE_NPROC_ADDR_OF(x)                     _CCL_VALUEOF(_PPR_PFE_NPROC_ADDR_TYPE,x)

#define  PPR_PFE_NPROC_ADDR_DEFAULT       (_PPR_PFE_NPROC_ADDR_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,PFE_NPROC_ADDR,ADDR)                        \
	)

#define PPR_PFE_NPROC_ADDR_RMK(addr) (_PPR_PFE_NPROC_ADDR_TYPE)(  \
	 _CCL_PER_FMK  (PPR,PFE_NPROC_ADDR,ADDR, addr)                \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  P F E   C T R L  |
 * |___________________|
 *
 * PFE_CTRL - Prefetch Engine Control Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (w)  VPS -
 *                   VPS   |   description
 *              -----------+----------------------
 *                   NOP   | No function
 *                  RESET  | Clear VBIPPR_VPS_B0B3_REG, VBIPPR_VPS_B4B7_REG, VBIPPR_VPS_B8B11_REG, VBIPPR_VPS_B12_REG
 *
 * (w)  CC -
 *                   CC    |   description
 *              -----------+----------------------
 *                   NOP   | No function
 *                  RESET  | Clear VBIPPR_CC_REG
 *
 * (w)  WSS -
 *                   WSS   |   description
 *              -----------+----------------------
 *                   NOP   | No function
 *                  RESET  | Clear VBIPPR_WSS_REG
 *
 * (w)  CGMS -
 *                   CGMS  |   description
 *              -----------+----------------------
 *                   NOP   | No function
 *                  RESET  | Clear VBIPPR_CGMS_REG
 *
 * (w)  830 -
 *                   830   |   description
 *              -----------+----------------------
 *                   NOP   | No function
 *                  RESET  | Clear VBIPPR_830F1_B0B3_REG,
 *                         |       VBIPPR_830F1_B4B6_REG,
 *                         |       VBIPPR_830F1_B7_REG,
 *                         |       VBIPPR_830F1_B8B11_REG,
 *                         |       VBIPPR_830F1_B12B15_REG,
 *                         |       VBIPPR_830F1_B16B19_REG,
 *                         |       VBIPPR_830F1_B20B23_REG,
 *                         |       VBIPPR_830F1_B24B27_REG,
 *                         |       VBIPPR_830F1_B28B31_REG,
 *                         |       VBIPPR_830F1_B32B35_REG,
 *                         |       VBIPPR_830F1_B36B39_REG,
 *                         |       VBIPPR_830F2_B0B3_REG,
 *                         |       VBIPPR_830F2_B4B6_REG,
 *                         |       VBIPPR_830F2_B7_REG,
 *                         |       VBIPPR_830F2_B8B11_REG,
 *                         |       VBIPPR_830F2_B12B15_REG,
 *                         |       VBIPPR_830F2_B16B19_REG,
 *                         |       VBIPPR_830F2_B20B23_REG,
 *                         |       VBIPPR_830F2_B24B27_REG,
 *                         |       VBIPPR_830F2_B28B31_REG,
 *                         |       VBIPPR_830F2_B32B35_REG,
 *                         |       VBIPPR_830F2_B36B39_REG
 *
 * (rw)  PARITY_CHK -
 *              PARITY_CHK |   description
 *              -----------+----------------------
 *                DISABLE  | Odd-Parity is in-active
 *                 ENABLE  | Odd-Parity is active
 *
 * (rw)  MODE3_IND -
 *               MODE3_IND |   description
 *              -----------+-------------------------------------
 *                 CLEARED | All packets of MODE3 has been moved.         (read)
 *                  LEFT   | There remain packet(s) of MODE3 to be moved. (read)
 *                 CLEAR   | Reset to CLEARED status                      (write)
 *
 * (rw)  MODE3_EN -
 *                MODE3_EN |   description
 *              -----------+----------------------
 *                 DISABLE | Moving X/29 is disabled
 *                  ENABLE | Moving X/29 is enabled
 *
 * (rw)  MODE2_IND -
 *               MODE2_IND |   description
 *              -----------+-------------------------------------
 *                 CLEARED | All packets of MODE2 has been moved.         (read)
 *                  LEFT   | There remain packet(s) of MODE2 to be moved. (read)
 *                 CLEAR   | Reset to CLEARED status                      (write)
 *
 * (rw)  MODE2_EN -
 *                MODE2_EN |   description
 *              -----------+------------------------------------------------------------
 *                 DISABLE | Moving X/26/15, X/27/1~X/27/7 and X/28/2~X/28/4 is disabled
 *                  ENABLE | Moving X/26/15, X/27/1~X/27/7 and X/28/2~X/28/4 is enabled
 *
 * (rw)  MODE1_IND -
 *               MODE1_IND |   description
 *              -----------+-------------------------------------
 *                 CLEARED | All packets of MODE1 has been moved.         (read)
 *                  LEFT   | There remain packet(s) of MODE1 to be moved. (read)
 *                 CLEAR   | Reset to CLEARED status                      (write)
 *
 * (r)  MODE1_EN -
 *                MODE1_EN |   description
 *              -----------+------------------------------------------------------------
 *                 DISABLE | Moving X/26/0~X/26/14, X/27/0, X/28/0 and X/28/1 is disabled
 *                  ENABLE | Moving X/26/0~X/26/14, X/27/0, X/28/0 and X/28/1 is enabled
 *
 *
 * (r)  MODE0_IND -
 *               MODE0_IND |   description
 *              -----------+-------------------------------------
 *                 CLEARED | All packets of MODE0 has been moved.         (read)
 *                  LEFT   | There remain packet(s) of MODE0 to be moved. (read)
 *                 CLEAR   | Reset to CLEARED status                      (write)
 *
 * (r)  MODE0_EN -
 *                MODE0_EN |   description
 *              -----------+---------------------------------
 *                 DISABLE | Moving X/0 to X/25 is disabled
 *                  ENABLE | Moving X/0 to X/25 is enabled
 *
 * (r)  PAGE -
 *                  PAGE   |   description
 *              -----------+-------------------------------------
 *                  MULTI  | multi-page mode (with SDRAM)
 *                   10    | 10-page mode (with SRAM).
 *
 * (r)  START -
 *                  START  |   description
 *              -----------+-------------------------------------
 *             (w) DISABLE | No function
 *             (w)  ENABLE | Enable PFE move engine.
 *             (r)  DONE   | PFE move engine is done.
\*-------------------------------------------------------------------------*/
#define _PPR_PFE_CTRL_OFFSET                    (0x00E4u)
#define _PPR_PFE_CTRL_ADDRH(h)                  PPR_ADDRH(h,PFE_CTRL)
#define _PPR_PFE_CTRL_LEN                       (32)                 /* 32-bit*/
#define _PPR_PFE_CTRL_TYPE                      uint32_t

#define _PPR_PFE_CTRL_VPS_MASK                  (0x00008000u)
#define _PPR_PFE_CTRL_VPS_SHIFT                 (15)
#define  PPR_PFE_CTRL_VPS_DEFAULT               (0x00u)
#define  PPR_PFE_CTRL_VPS_OF(x)                 _CCL_VALUEOF(_PPR_PFE_CTRL_TYPE,x)
#define  PPR_PFE_CTRL_VPS_NOP                   (0x00u)
#define  PPR_PFE_CTRL_VPS_RESET                 (0x01u)

#define _PPR_PFE_CTRL_CC_MASK                   (0x00004000u)
#define _PPR_PFE_CTRL_CC_SHIFT                  (14)
#define  PPR_PFE_CTRL_CC_DEFAULT                (0x00u)
#define  PPR_PFE_CTRL_CC_OF(x)                  _CCL_VALUEOF(_PPR_PFE_CTRL_TYPE,x)
#define  PPR_PFE_CTRL_CC_NOP                    (0x00u)
#define  PPR_PFE_CTRL_CC_RESET                  (0x01u)

#define _PPR_PFE_CTRL_WSS_MASK                  (0x00002000u)
#define _PPR_PFE_CTRL_WSS_SHIFT                 (13)
#define  PPR_PFE_CTRL_WSS_DEFAULT               (0x00u)
#define  PPR_PFE_CTRL_WSS_OF(x)                 _CCL_VALUEOF(_PPR_PFE_CTRL_TYPE,x)
#define  PPR_PFE_CTRL_WSS_NOP                   (0x00u)
#define  PPR_PFE_CTRL_WSS_RESET                 (0x01u)

#define _PPR_PFE_CTRL_CGMS_MASK                 (0x00001000u)
#define _PPR_PFE_CTRL_CGMS_SHIFT                (12)
#define  PPR_PFE_CTRL_CGMS_DEFAULT              (0x00u)
#define  PPR_PFE_CTRL_CGMS_OF(x)                _CCL_VALUEOF(_PPR_PFE_CTRL_TYPE,x)
#define  PPR_PFE_CTRL_CGMS_NOP                  (0x00u)
#define  PPR_PFE_CTRL_CGMS_RESET                (0x01u)

#define _PPR_PFE_CTRL_830_MASK                  (0x00000800u)
#define _PPR_PFE_CTRL_830_SHIFT                 (11)
#define  PPR_PFE_CTRL_830_DEFAULT               (0x00u)
#define  PPR_PFE_CTRL_830_OF(x)                 _CCL_VALUEOF(_PPR_PFE_CTRL_TYPE,x)
#define  PPR_PFE_CTRL_830_NOP                   (0x00u)
#define  PPR_PFE_CTRL_830_RESET                 (0x01u)

#define _PPR_PFE_CTRL_PARITY_CHK_MASK           (0x00000400u)
#define _PPR_PFE_CTRL_PARITY_CHK_SHIFT          (10)
#define  PPR_PFE_CTRL_PARITY_CHK_DEFAULT        (0x00u)
#define  PPR_PFE_CTRL_PARITY_CHK_OF(x)          _CCL_VALUEOF(_PPR_PFE_CTRL_TYPE,x)
#define  PPR_PFE_CTRL_PARITY_CHK_DISABLE        (0x00u)
#define  PPR_PFE_CTRL_PARITY_CHK_ENABLE         (0x01u)

#define _PPR_PFE_CTRL_MODE3_IND_MASK            (0x00000200u)
#define _PPR_PFE_CTRL_MODE3_IND_SHIFT           (9)
#define  PPR_PFE_CTRL_MODE3_IND_DEFAULT         (0x00u)
#define  PPR_PFE_CTRL_MODE3_IND_OF(x)           _CCL_VALUEOF(_PPR_PFE_CTRL_TYPE,x)
#define  PPR_PFE_CTRL_MODE3_IND_CLEARED         (0x00u)
#define  PPR_PFE_CTRL_MODE3_IND_LEFT            (0x01u)
#define  PPR_PFE_CTRL_MODE3_IND_CLEAR           (0x00u)

#define _PPR_PFE_CTRL_MODE3_EN_MASK             (0x00000100u)
#define _PPR_PFE_CTRL_MODE3_EN_SHIFT            (8)
#define  PPR_PFE_CTRL_MODE3_EN_DEFAULT          (0x00u)
#define  PPR_PFE_CTRL_MODE3_EN_OF(x)            _CCL_VALUEOF(_PPR_PFE_CTRL_TYPE,x)
#define  PPR_PFE_CTRL_MODE3_EN_DISABLE          (0x00u)
#define  PPR_PFE_CTRL_MODE3_EN_ENABLE           (0x01u)

#define _PPR_PFE_CTRL_MODE2_IND_MASK            (0x00000080u)
#define _PPR_PFE_CTRL_MODE2_IND_SHIFT           (7)
#define  PPR_PFE_CTRL_MODE2_IND_DEFAULT         (0x00u)
#define  PPR_PFE_CTRL_MODE2_IND_OF(x)           _CCL_VALUEOF(_PPR_PFE_CTRL_TYPE,x)
#define  PPR_PFE_CTRL_MODE2_IND_CLEARED         (0x00u)
#define  PPR_PFE_CTRL_MODE2_IND_LEFT            (0x01u)
#define  PPR_PFE_CTRL_MODE2_IND_CLEAR           (0x00u)

#define _PPR_PFE_CTRL_MODE2_EN_MASK             (0x00000040u)
#define _PPR_PFE_CTRL_MODE2_EN_SHIFT            (6)
#define  PPR_PFE_CTRL_MODE2_EN_DEFAULT          (0x00u)
#define  PPR_PFE_CTRL_MODE2_EN_OF(x)            _CCL_VALUEOF(_PPR_PFE_CTRL_TYPE,x)
#define  PPR_PFE_CTRL_MODE2_EN_DISABLE          (0x00u)
#define  PPR_PFE_CTRL_MODE2_EN_ENABLE           (0x01u)

#define _PPR_PFE_CTRL_MODE1_IND_MASK            (0x00000020u)
#define _PPR_PFE_CTRL_MODE1_IND_SHIFT           (5)
#define  PPR_PFE_CTRL_MODE1_IND_DEFAULT         (0x00u)
#define  PPR_PFE_CTRL_MODE1_IND_OF(x)           _CCL_VALUEOF(_PPR_PFE_CTRL_TYPE,x)
#define  PPR_PFE_CTRL_MODE1_IND_CLEARED         (0x00u)
#define  PPR_PFE_CTRL_MODE1_IND_LEFT            (0x01u)
#define  PPR_PFE_CTRL_MODE1_IND_CLEAR           (0x00u)

#define _PPR_PFE_CTRL_MODE1_EN_MASK             (0x00000010u)
#define _PPR_PFE_CTRL_MODE1_EN_SHIFT            (4)
#define  PPR_PFE_CTRL_MODE1_EN_DEFAULT          (0x00u)
#define  PPR_PFE_CTRL_MODE1_EN_OF(x)            _CCL_VALUEOF(_PPR_PFE_CTRL_TYPE,x)
#define  PPR_PFE_CTRL_MODE1_EN_DISABLE          (0x00u)
#define  PPR_PFE_CTRL_MODE1_EN_ENABLE           (0x01u)

#define _PPR_PFE_CTRL_MODE0_IND_MASK            (0x00000008u)
#define _PPR_PFE_CTRL_MODE0_IND_SHIFT           (3)
#define  PPR_PFE_CTRL_MODE0_IND_DEFAULT         (0x00u)
#define  PPR_PFE_CTRL_MODE0_IND_OF(x)           _CCL_VALUEOF(_PPR_PFE_CTRL_TYPE,x)
#define  PPR_PFE_CTRL_MODE0_IND_CLEARED         (0x00u)
#define  PPR_PFE_CTRL_MODE0_IND_LEFT            (0x01u)
#define  PPR_PFE_CTRL_MODE0_IND_CLEAR           (0x00u)

#define _PPR_PFE_CTRL_MODE0_EN_MASK             (0x00000004u)
#define _PPR_PFE_CTRL_MODE0_EN_SHIFT            (2)
#define  PPR_PFE_CTRL_MODE0_EN_DEFAULT          (0x00u)
#define  PPR_PFE_CTRL_MODE0_EN_OF(x)            _CCL_VALUEOF(_PPR_PFE_CTRL_TYPE,x)
#define  PPR_PFE_CTRL_MODE0_EN_DISABLE          (0x00u)
#define  PPR_PFE_CTRL_MODE0_EN_ENABLE           (0x01u)

#define _PPR_PFE_CTRL_PAGE_MASK                 (0x00000002u)
#define _PPR_PFE_CTRL_PAGE_SHIFT                (1)
#define  PPR_PFE_CTRL_PAGE_DEFAULT              (0x00u)
#define  PPR_PFE_CTRL_PAGE_OF(x)                _CCL_VALUEOF(_PPR_PFE_CTRL_TYPE,x)
#define  PPR_PFE_CTRL_PAGE_MULTI                (0x00u)
#define  PPR_PFE_CTRL_PAGE_10                   (0x01u)

#define _PPR_PFE_CTRL_START_MASK                (0x00000001u)
#define _PPR_PFE_CTRL_START_SHIFT               (0)
#define  PPR_PFE_CTRL_START_DEFAULT             (0x00u)
#define  PPR_PFE_CTRL_START_OF(x)               _CCL_VALUEOF(_PPR_PFE_CTRL_TYPE,x)
#define  PPR_PFE_CTRL_START_DISABLE             (0x00u)
#define  PPR_PFE_CTRL_START_ENABLE              (0x01u)
#define  PPR_PFE_CTRL_START_DONE                (0x00u)

#define  PPR_PFE_CTRL_OF(x)             _CCL_VALUEOF(_PPR_PFE_CTRL_TYPE,x)

#define  PPR_PFE_CTRL_DEFAULT (_PPR_PFE_CTRL_TYPE)(   \
	 _CCL_PER_FDEFAULT(PPR,PFE_CTRL,VPS)              \
	|_CCL_PER_FDEFAULT(PPR,PFE_CTRL,CC)               \
	|_CCL_PER_FDEFAULT(PPR,PFE_CTRL,WSS)              \
	|_CCL_PER_FDEFAULT(PPR,PFE_CTRL,CGMS)             \
	|_CCL_PER_FDEFAULT(PPR,PFE_CTRL,830)              \
	|_CCL_PER_FDEFAULT(PPR,PFE_CTRL,PARITY_CHK)       \
	|_CCL_PER_FDEFAULT(PPR,PFE_CTRL,MODE3_IND)        \
	|_CCL_PER_FDEFAULT(PPR,PFE_CTRL,MODE3_EN)         \
	|_CCL_PER_FDEFAULT(PPR,PFE_CTRL,MODE2_IND)        \
	|_CCL_PER_FDEFAULT(PPR,PFE_CTRL,MODE2_EN)         \
	|_CCL_PER_FDEFAULT(PPR,PFE_CTRL,MODE1_IND)        \
	|_CCL_PER_FDEFAULT(PPR,PFE_CTRL,MODE1_EN)         \
	|_CCL_PER_FDEFAULT(PPR,PFE_CTRL,MODE0_IND)        \
	|_CCL_PER_FDEFAULT(PPR,PFE_CTRL,MODE0_EN)         \
	|_CCL_PER_FDEFAULT(PPR,PFE_CTRL,PAGE)             \
	|_CCL_PER_FDEFAULT(PPR,PFE_CTRL,START)            \
	)

#define PPR_PFE_CTRL_RMK(vps, cc, wss, cgms, m830, parity_chk, mode3_ind, mode3_en, mode2_ind, mode2_en, mode1_ind, mode1_en, mode0_ind, mode0_en, page, start) (_PPR_PFE_CTRL_TYPE)( \
	 _CCL_PER_FMK  (PPR,PFE_CTRL,VPS,vps)                       \
	|_CCL_PER_FMK  (PPR,PFE_CTRL,CC,cc)                         \
	|_CCL_PER_FMK  (PPR,PFE_CTRL,WSS,wss)                       \
	|_CCL_PER_FMK  (PPR,PFE_CTRL,CGMS,cgms)                     \
	|_CCL_PER_FMK  (PPR,PFE_CTRL,830,m830)                      \
	|_CCL_PER_FMK  (PPR,PFE_CTRL,PARITY_CHK,parity_chk)         \
	|_CCL_PER_FMK  (PPR,PFE_CTRL,MODE3_IND,mode3_ind)           \
	|_CCL_PER_FMK  (PPR,PFE_CTRL,MODE3_EN, mode3_en)            \
	|_CCL_PER_FMK  (PPR,PFE_CTRL,MODE2_IND,mode2_ind)           \
	|_CCL_PER_FMK  (PPR,PFE_CTRL,MODE2_EN, mode2_en)            \
	|_CCL_PER_FMK  (PPR,PFE_CTRL,MODE1_IND,mode1_ind)           \
	|_CCL_PER_FMK  (PPR,PFE_CTRL,MODE1_EN, mode1_en)            \
	|_CCL_PER_FMK  (PPR,PFE_CTRL,MODE0_IND,mode0_ind)           \
	|_CCL_PER_FMK  (PPR,PFE_CTRL,MODE0_EN, mode0_en)            \
	|_CCL_PER_FMK  (PPR,PFE_CTRL,PAGE,page)                     \
	|_CCL_PER_FMK  (PPR,PFE_CTRL,START,start)                   \
	)

#define PPR_PFE_CTRL_RMKS(vps, cc, wss, cgms, m830, parity_chk, mode3_ind, mode3_en, mode2_ind, mode2_en, mode1_ind, mode1_en, mode0_ind, mode0_en, page, start) (_PPR_PFE_CTRL_TYPE)( \
	 _CCL_PER_FMKS (PPR,PFE_CTRL,VPS,vps)                       \
	|_CCL_PER_FMKS (PPR,PFE_CTRL,CC,cc)                         \
	|_CCL_PER_FMKS (PPR,PFE_CTRL,WSS,wss)                       \
	|_CCL_PER_FMKS (PPR,PFE_CTRL,CGMS,cgms)                     \
	|_CCL_PER_FMKS (PPR,PFE_CTRL,830,m830)                      \
	|_CCL_PER_FMKS (PPR,PFE_CTRL,PARITY_CHK,parity_chk)         \
	|_CCL_PER_FMKS (PPR,PFE_CTRL,MODE3_IND,mode3_ind)           \
	|_CCL_PER_FMKS (PPR,PFE_CTRL,MODE3_EN, mode3_en)            \
	|_CCL_PER_FMKS (PPR,PFE_CTRL,MODE2_IND,mode2_ind)           \
	|_CCL_PER_FMKS (PPR,PFE_CTRL,MODE2_EN, mode2_en)            \
	|_CCL_PER_FMKS (PPR,PFE_CTRL,MODE1_IND,mode1_ind)           \
	|_CCL_PER_FMKS (PPR,PFE_CTRL,MODE1_EN, mode1_en)            \
	|_CCL_PER_FMKS (PPR,PFE_CTRL,MODE0_IND,mode0_ind)           \
	|_CCL_PER_FMKS (PPR,PFE_CTRL,MODE0_EN, mode0_en)            \
	|_CCL_PER_FMKS (PPR,PFE_CTRL,PAGE,page)                     \
	|_CCL_PER_FMKS (PPR,PFE_CTRL,START,start)                   \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  F B    P K T 1   |
 * |___________________|
 *
 * FB_PKT1 - Packet 1 of field buffer Status Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (r)  DERR - The packet exists odd parity error.
 *             This bit is valid only for packet X/0).
 *                   DERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Odd-parity error
 *
 * (r)  HCERR -  the packet exists hamming error.
 *                  HCERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Hamming error
 *
 * (r)  DCODE -  Page designation code (Hamming 8/4 Decoded).
 *               This field is valid only for packet X/26~X31.
 *
 * (r)  PKT_NUM - Page packet number (Hamming 8/4 Decoded)
 *
 * (r)  MAG_NUM - Page magazine number (Hamming 8/4 Decoded)
\*-------------------------------------------------------------------------*/
#define _PPR_FB_PKT1_OFFSET                    (0x0110u)
#define _PPR_FB_PKT1_ADDRH(h)                  PPR_ADDRH(h,FB_PKT1)
#define _PPR_FB_PKT1_LEN                       (32)                 /* 32-bit*/
#define _PPR_FB_PKT1_TYPE                      uint32_t

#define _PPR_FB_PKT1_DERR_MASK                 (0x00002000u)
#define _PPR_FB_PKT1_DERR_SHIFT                (13)
#define  PPR_FB_PKT1_DERR_DEFAULT              (0x00u)
#define  PPR_FB_PKT1_DERR_OF(x)                _CCL_VALUEOF(_PPR_FB_PKT1_TYPE,x)
#define  PPR_FB_PKT1_DERR_OK                   (0x00u)
#define  PPR_FB_PKT1_DERR_ERROR                (0x01u)

#define _PPR_FB_PKT1_HCERR_MASK                (0x00001000u)
#define _PPR_FB_PKT1_HCERR_SHIFT               (12)
#define  PPR_FB_PKT1_HCERR_DEFAULT             (0x00u)
#define  PPR_FB_PKT1_HCERR_OF(x)               _CCL_VALUEOF(_PPR_FB_PKT1_TYPE,x)
#define  PPR_FB_PKT1_HCERR_OK                  (0x00u)
#define  PPR_FB_PKT1_HCERR_ERROR               (0x01u)

#define _PPR_FB_PKT1_DCODE_MASK                (0x00000F00u)
#define _PPR_FB_PKT1_DCODE_SHIFT               (8)
#define  PPR_FB_PKT1_DCODE_DEFAULT             (0x00u)
#define  PPR_FB_PKT1_DCODE_OF(x)               _DCODEL_VALUEOF(_PPR_FB_PKT1_TYPE,x)

#define _PPR_FB_PKT1_PKT_NUM_MASK              (0x000000F8u)
#define _PPR_FB_PKT1_PKT_NUM_SHIFT             (3)
#define  PPR_FB_PKT1_PKT_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT1_PKT_NUM_OF(x)             _PKT_NUML_VALUEOF(_PPR_FB_PKT1_TYPE,x)

#define _PPR_FB_PKT1_MAG_NUM_MASK              (0x00000007u)
#define _PPR_FB_PKT1_MAG_NUM_SHIFT             (0)
#define  PPR_FB_PKT1_MAG_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT1_MAG_NUM_OF(x)             _MAG_NUML_VALUEOF(_PPR_FB_PKT1_TYPE,x)

#define  PPR_FB_PKT1_OF(x)             _CCL_VALUEOF(_PPR_FB_PKT1_TYPE,x)

#define  PPR_FB_PKT1_DEFAULT (_PPR_FB_PKT1_TYPE)(   \
	 _CCL_PER_FDEFAULT(PPR,FB_PKT1,DERR   )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT1,HCERR  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT1,DCODE  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT1,PKT_NUM)         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT1,MAG_NUM)         \
	)

#define PPR_FB_PKT1_RMK(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT1_TYPE)( \
	 _CCL_PER_FMK  (PPR,FB_PKT1,DERR   ,derr   )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT1,HCERR  ,hcerr  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT1,DCODE  ,dcode  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT1,PKT_NUM,pkt_num)                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT1,MAG_NUM,mag_num)                                   \
	)

#define PPR_FB_PKT1_RMKS(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT1_TYPE)( \
	 _CCL_PER_FMKS (PPR,FB_PKT1,DERR   ,derr   )                                    \
	|_CCL_PER_FMKS (PPR,FB_PKT1,HCERR  ,hcerr  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT1,DCODE  ,dcode  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT1,PKT_NUM,pkt_num)                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT1,MAG_NUM,mag_num)                                    \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  F B    P K T 2   |
 * |___________________|
 *
 * FB_PKT2 - Packet 2 of field buffer Status Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (r)  DERR - The packet exists odd parity error.
 *             This bit is valid only for packet X/0).
 *                   DERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Odd-parity error
 *
 * (r)  HCERR -  the packet exists hamming error.
 *                  HCERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Hamming error
 *
 * (r)  DCODE -  Page designation code (Hamming 8/4 Decoded).
 *               This field is valid only for packet X/26~X31.
 *
 * (r)  PKT_NUM - Page packet number (Hamming 8/4 Decoded)
 *
 * (r)  MAG_NUM - Page magazine number (Hamming 8/4 Decoded)
\*-------------------------------------------------------------------------*/
#define _PPR_FB_PKT2_OFFSET                    (0x0114u)
#define _PPR_FB_PKT2_ADDRH(h)                  PPR_ADDRH(h,FB_PKT2)
#define _PPR_FB_PKT2_LEN                       (32)                 /* 32-bit*/
#define _PPR_FB_PKT2_TYPE                      uint32_t

#define _PPR_FB_PKT2_DERR_MASK                 (0x00002000u)
#define _PPR_FB_PKT2_DERR_SHIFT                (13)
#define  PPR_FB_PKT2_DERR_DEFAULT              (0x00u)
#define  PPR_FB_PKT2_DERR_OF(x)                _CCL_VALUEOF(_PPR_FB_PKT2_TYPE,x)
#define  PPR_FB_PKT2_DERR_OK                   (0x00u)
#define  PPR_FB_PKT2_DERR_ERROR                (0x01u)

#define _PPR_FB_PKT2_HCERR_MASK                (0x00001000u)
#define _PPR_FB_PKT2_HCERR_SHIFT               (12)
#define  PPR_FB_PKT2_HCERR_DEFAULT             (0x00u)
#define  PPR_FB_PKT2_HCERR_OF(x)               _CCL_VALUEOF(_PPR_FB_PKT2_TYPE,x)
#define  PPR_FB_PKT2_HCERR_OK                  (0x00u)
#define  PPR_FB_PKT2_HCERR_ERROR               (0x01u)

#define _PPR_FB_PKT2_DCODE_MASK                (0x00000F00u)
#define _PPR_FB_PKT2_DCODE_SHIFT               (8)
#define  PPR_FB_PKT2_DCODE_DEFAULT             (0x00u)
#define  PPR_FB_PKT2_DCODE_OF(x)               _DCODEL_VALUEOF(_PPR_FB_PKT2_TYPE,x)

#define _PPR_FB_PKT2_PKT_NUM_MASK              (0x000000F8u)
#define _PPR_FB_PKT2_PKT_NUM_SHIFT             (3)
#define  PPR_FB_PKT2_PKT_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT2_PKT_NUM_OF(x)             _PKT_NUML_VALUEOF(_PPR_FB_PKT2_TYPE,x)

#define _PPR_FB_PKT2_MAG_NUM_MASK              (0x00000007u)
#define _PPR_FB_PKT2_MAG_NUM_SHIFT             (0)
#define  PPR_FB_PKT2_MAG_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT2_MAG_NUM_OF(x)             _MAG_NUML_VALUEOF(_PPR_FB_PKT2_TYPE,x)

#define  PPR_FB_PKT2_OF(x)             _CCL_VALUEOF(_PPR_FB_PKT2_TYPE,x)

#define  PPR_FB_PKT2_DEFAULT (_PPR_FB_PKT2_TYPE)(   \
	 _CCL_PER_FDEFAULT(PPR,FB_PKT2,DERR   )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT2,HCERR  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT2,DCODE  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT2,PKT_NUM)         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT2,MAG_NUM)         \
	)

#define PPR_FB_PKT2_RMK(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT2_TYPE)( \
	 _CCL_PER_FMK  (PPR,FB_PKT2,DERR   ,derr   )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT2,HCERR  ,hcerr  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT2,DCODE  ,dcode  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT2,PKT_NUM,pkt_num)                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT2,MAG_NUM,mag_num)                                   \
	)

#define PPR_FB_PKT2_RMKS(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT2_TYPE)( \
	 _CCL_PER_FMKS (PPR,FB_PKT2,DERR   ,derr   )                                    \
	|_CCL_PER_FMKS (PPR,FB_PKT2,HCERR  ,hcerr  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT2,DCODE  ,dcode  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT2,PKT_NUM,pkt_num)                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT2,MAG_NUM,mag_num)                                    \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  F B    P K T 3   |
 * |___________________|
 *
 * FB_PKT3 - Packet 3 of field buffer Status Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (r)  DERR - The packet exists odd parity error.
 *             This bit is valid only for packet X/0).
 *                   DERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Odd-parity error
 *
 * (r)  HCERR -  the packet exists hamming error.
 *                  HCERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Hamming error
 *
 * (r)  DCODE -  Page designation code (Hamming 8/4 Decoded).
 *               This field is valid only for packet X/26~X31.
 *
 * (r)  PKT_NUM - Page packet number (Hamming 8/4 Decoded)
 *
 * (r)  MAG_NUM - Page magazine number (Hamming 8/4 Decoded)
\*-------------------------------------------------------------------------*/
#define _PPR_FB_PKT3_OFFSET                    (0x0118u)
#define _PPR_FB_PKT3_ADDRH(h)                  PPR_ADDRH(h,FB_PKT3)
#define _PPR_FB_PKT3_LEN                       (32)                 /* 32-bit*/
#define _PPR_FB_PKT3_TYPE                      uint32_t

#define _PPR_FB_PKT3_DERR_MASK                 (0x00002000u)
#define _PPR_FB_PKT3_DERR_SHIFT                (13)
#define  PPR_FB_PKT3_DERR_DEFAULT              (0x00u)
#define  PPR_FB_PKT3_DERR_OF(x)                _CCL_VALUEOF(_PPR_FB_PKT3_TYPE,x)
#define  PPR_FB_PKT3_DERR_OK                   (0x00u)
#define  PPR_FB_PKT3_DERR_ERROR                (0x01u)

#define _PPR_FB_PKT3_HCERR_MASK                (0x00001000u)
#define _PPR_FB_PKT3_HCERR_SHIFT               (12)
#define  PPR_FB_PKT3_HCERR_DEFAULT             (0x00u)
#define  PPR_FB_PKT3_HCERR_OF(x)               _CCL_VALUEOF(_PPR_FB_PKT3_TYPE,x)
#define  PPR_FB_PKT3_HCERR_OK                  (0x00u)
#define  PPR_FB_PKT3_HCERR_ERROR               (0x01u)

#define _PPR_FB_PKT3_DCODE_MASK                (0x00000F00u)
#define _PPR_FB_PKT3_DCODE_SHIFT               (8)
#define  PPR_FB_PKT3_DCODE_DEFAULT             (0x00u)
#define  PPR_FB_PKT3_DCODE_OF(x)               _DCODEL_VALUEOF(_PPR_FB_PKT3_TYPE,x)

#define _PPR_FB_PKT3_PKT_NUM_MASK              (0x000000F8u)
#define _PPR_FB_PKT3_PKT_NUM_SHIFT             (3)
#define  PPR_FB_PKT3_PKT_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT3_PKT_NUM_OF(x)             _PKT_NUML_VALUEOF(_PPR_FB_PKT3_TYPE,x)

#define _PPR_FB_PKT3_MAG_NUM_MASK              (0x00000007u)
#define _PPR_FB_PKT3_MAG_NUM_SHIFT             (0)
#define  PPR_FB_PKT3_MAG_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT3_MAG_NUM_OF(x)             _MAG_NUML_VALUEOF(_PPR_FB_PKT3_TYPE,x)

#define  PPR_FB_PKT3_OF(x)             _CCL_VALUEOF(_PPR_FB_PKT3_TYPE,x)

#define  PPR_FB_PKT3_DEFAULT (_PPR_FB_PKT3_TYPE)(   \
	 _CCL_PER_FDEFAULT(PPR,FB_PKT3,DERR   )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT3,HCERR  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT3,DCODE  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT3,PKT_NUM)         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT3,MAG_NUM)         \
	)

#define PPR_FB_PKT3_RMK(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT3_TYPE)( \
	 _CCL_PER_FMK  (PPR,FB_PKT3,DERR   ,derr   )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT3,HCERR  ,hcerr  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT3,DCODE  ,dcode  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT3,PKT_NUM,pkt_num)                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT3,MAG_NUM,mag_num)                                   \
	)

#define PPR_FB_PKT3_RMKS(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT3_TYPE)( \
	 _CCL_PER_FMKS (PPR,FB_PKT3,DERR   ,derr   )                                    \
	|_CCL_PER_FMKS (PPR,FB_PKT3,HCERR  ,hcerr  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT3,DCODE  ,dcode  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT3,PKT_NUM,pkt_num)                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT3,MAG_NUM,mag_num)                                    \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  F B    P K T 4   |
 * |___________________|
 *
 * FB_PKT4 - Packet 4 of field buffer Status Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (r)  DERR - The packet exists odd parity error.
 *             This bit is valid only for packet X/0).
 *                   DERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Odd-parity error
 *
 * (r)  HCERR -  the packet exists hamming error.
 *                  HCERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Hamming error
 *
 * (r)  DCODE -  Page designation code (Hamming 8/4 Decoded).
 *               This field is valid only for packet X/26~X31.
 *
 * (r)  PKT_NUM - Page packet number (Hamming 8/4 Decoded)
 *
 * (r)  MAG_NUM - Page magazine number (Hamming 8/4 Decoded)
\*-------------------------------------------------------------------------*/
#define _PPR_FB_PKT4_OFFSET                    (0x011Cu)
#define _PPR_FB_PKT4_ADDRH(h)                  PPR_ADDRH(h,FB_PKT4)
#define _PPR_FB_PKT4_LEN                       (32)                 /* 32-bit*/
#define _PPR_FB_PKT4_TYPE                      uint32_t

#define _PPR_FB_PKT4_DERR_MASK                 (0x00002000u)
#define _PPR_FB_PKT4_DERR_SHIFT                (13)
#define  PPR_FB_PKT4_DERR_DEFAULT              (0x00u)
#define  PPR_FB_PKT4_DERR_OF(x)                _CCL_VALUEOF(_PPR_FB_PKT4_TYPE,x)
#define  PPR_FB_PKT4_DERR_OK                   (0x00u)
#define  PPR_FB_PKT4_DERR_ERROR                (0x01u)

#define _PPR_FB_PKT4_HCERR_MASK                (0x00001000u)
#define _PPR_FB_PKT4_HCERR_SHIFT               (12)
#define  PPR_FB_PKT4_HCERR_DEFAULT             (0x00u)
#define  PPR_FB_PKT4_HCERR_OF(x)               _CCL_VALUEOF(_PPR_FB_PKT4_TYPE,x)
#define  PPR_FB_PKT4_HCERR_OK                  (0x00u)
#define  PPR_FB_PKT4_HCERR_ERROR               (0x01u)

#define _PPR_FB_PKT4_DCODE_MASK                (0x00000F00u)
#define _PPR_FB_PKT4_DCODE_SHIFT               (8)
#define  PPR_FB_PKT4_DCODE_DEFAULT             (0x00u)
#define  PPR_FB_PKT4_DCODE_OF(x)               _DCODEL_VALUEOF(_PPR_FB_PKT4_TYPE,x)

#define _PPR_FB_PKT4_PKT_NUM_MASK              (0x000000F8u)
#define _PPR_FB_PKT4_PKT_NUM_SHIFT             (3)
#define  PPR_FB_PKT4_PKT_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT4_PKT_NUM_OF(x)             _PKT_NUML_VALUEOF(_PPR_FB_PKT4_TYPE,x)

#define _PPR_FB_PKT4_MAG_NUM_MASK              (0x00000007u)
#define _PPR_FB_PKT4_MAG_NUM_SHIFT             (0)
#define  PPR_FB_PKT4_MAG_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT4_MAG_NUM_OF(x)             _MAG_NUML_VALUEOF(_PPR_FB_PKT4_TYPE,x)

#define  PPR_FB_PKT4_OF(x)             _CCL_VALUEOF(_PPR_FB_PKT4_TYPE,x)

#define  PPR_FB_PKT4_DEFAULT (_PPR_FB_PKT4_TYPE)(   \
	 _CCL_PER_FDEFAULT(PPR,FB_PKT4,DERR   )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT4,HCERR  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT4,DCODE  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT4,PKT_NUM)         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT4,MAG_NUM)         \
	)

#define PPR_FB_PKT4_RMK(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT4_TYPE)( \
	 _CCL_PER_FMK  (PPR,FB_PKT4,DERR   ,derr   )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT4,HCERR  ,hcerr  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT4,DCODE  ,dcode  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT4,PKT_NUM,pkt_num)                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT4,MAG_NUM,mag_num)                                   \
	)

#define PPR_FB_PKT4_RMKS(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT4_TYPE)( \
	 _CCL_PER_FMKS (PPR,FB_PKT4,DERR   ,derr   )                                    \
	|_CCL_PER_FMKS (PPR,FB_PKT4,HCERR  ,hcerr  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT4,DCODE  ,dcode  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT4,PKT_NUM,pkt_num)                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT4,MAG_NUM,mag_num)                                    \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  F B    P K T 5   |
 * |___________________|
 *
 * FB_PKT5 - Packet 5 of field buffer Status Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (r)  DERR - The packet exists odd parity error.
 *             This bit is valid only for packet X/0).
 *                   DERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Odd-parity error
 *
 * (r)  HCERR -  the packet exists hamming error.
 *                  HCERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Hamming error
 *
 * (r)  DCODE -  Page designation code (Hamming 8/4 Decoded).
 *               This field is valid only for packet X/26~X31.
 *
 * (r)  PKT_NUM - Page packet number (Hamming 8/4 Decoded)
 *
 * (r)  MAG_NUM - Page magazine number (Hamming 8/4 Decoded)
\*-------------------------------------------------------------------------*/
#define _PPR_FB_PKT5_OFFSET                    (0x0120u)
#define _PPR_FB_PKT5_ADDRH(h)                  PPR_ADDRH(h,FB_PKT5)
#define _PPR_FB_PKT5_LEN                       (32)                 /* 32-bit*/
#define _PPR_FB_PKT5_TYPE                      uint32_t

#define _PPR_FB_PKT5_DERR_MASK                 (0x00002000u)
#define _PPR_FB_PKT5_DERR_SHIFT                (13)
#define  PPR_FB_PKT5_DERR_DEFAULT              (0x00u)
#define  PPR_FB_PKT5_DERR_OF(x)                _CCL_VALUEOF(_PPR_FB_PKT5_TYPE,x)
#define  PPR_FB_PKT5_DERR_OK                   (0x00u)
#define  PPR_FB_PKT5_DERR_ERROR                (0x01u)

#define _PPR_FB_PKT5_HCERR_MASK                (0x00001000u)
#define _PPR_FB_PKT5_HCERR_SHIFT               (12)
#define  PPR_FB_PKT5_HCERR_DEFAULT             (0x00u)
#define  PPR_FB_PKT5_HCERR_OF(x)               _CCL_VALUEOF(_PPR_FB_PKT5_TYPE,x)
#define  PPR_FB_PKT5_HCERR_OK                  (0x00u)
#define  PPR_FB_PKT5_HCERR_ERROR               (0x01u)

#define _PPR_FB_PKT5_DCODE_MASK                (0x00000F00u)
#define _PPR_FB_PKT5_DCODE_SHIFT               (8)
#define  PPR_FB_PKT5_DCODE_DEFAULT             (0x00u)
#define  PPR_FB_PKT5_DCODE_OF(x)               _DCODEL_VALUEOF(_PPR_FB_PKT5_TYPE,x)

#define _PPR_FB_PKT5_PKT_NUM_MASK              (0x000000F8u)
#define _PPR_FB_PKT5_PKT_NUM_SHIFT             (3)
#define  PPR_FB_PKT5_PKT_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT5_PKT_NUM_OF(x)             _PKT_NUML_VALUEOF(_PPR_FB_PKT5_TYPE,x)

#define _PPR_FB_PKT5_MAG_NUM_MASK              (0x00000007u)
#define _PPR_FB_PKT5_MAG_NUM_SHIFT             (0)
#define  PPR_FB_PKT5_MAG_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT5_MAG_NUM_OF(x)             _MAG_NUML_VALUEOF(_PPR_FB_PKT5_TYPE,x)

#define  PPR_FB_PKT5_OF(x)             _CCL_VALUEOF(_PPR_FB_PKT5_TYPE,x)

#define  PPR_FB_PKT5_DEFAULT (_PPR_FB_PKT5_TYPE)(   \
	 _CCL_PER_FDEFAULT(PPR,FB_PKT5,DERR   )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT5,HCERR  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT5,DCODE  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT5,PKT_NUM)         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT5,MAG_NUM)         \
	)

#define PPR_FB_PKT5_RMK(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT5_TYPE)( \
	 _CCL_PER_FMK  (PPR,FB_PKT5,DERR   ,derr   )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT5,HCERR  ,hcerr  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT5,DCODE  ,dcode  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT5,PKT_NUM,pkt_num)                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT5,MAG_NUM,mag_num)                                   \
	)

#define PPR_FB_PKT5_RMKS(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT5_TYPE)( \
	 _CCL_PER_FMKS (PPR,FB_PKT5,DERR   ,derr   )                                    \
	|_CCL_PER_FMKS (PPR,FB_PKT5,HCERR  ,hcerr  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT5,DCODE  ,dcode  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT5,PKT_NUM,pkt_num)                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT5,MAG_NUM,mag_num)                                    \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  F B    P K T 6   |
 * |___________________|
 *
 * FB_PKT6 - Packet 6 of field buffer Status Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (r)  DERR - The packet exists odd parity error.
 *             This bit is valid only for packet X/0).
 *                   DERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Odd-parity error
 *
 * (r)  HCERR -  the packet exists hamming error.
 *                  HCERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Hamming error
 *
 * (r)  DCODE -  Page designation code (Hamming 8/4 Decoded).
 *               This field is valid only for packet X/26~X31.
 *
 * (r)  PKT_NUM - Page packet number (Hamming 8/4 Decoded)
 *
 * (r)  MAG_NUM - Page magazine number (Hamming 8/4 Decoded)
\*-------------------------------------------------------------------------*/
#define _PPR_FB_PKT6_OFFSET                    (0x0124u)
#define _PPR_FB_PKT6_ADDRH(h)                  PPR_ADDRH(h,FB_PKT6)
#define _PPR_FB_PKT6_LEN                       (32)                 /* 32-bit*/
#define _PPR_FB_PKT6_TYPE                      uint32_t

#define _PPR_FB_PKT6_DERR_MASK                 (0x00002000u)
#define _PPR_FB_PKT6_DERR_SHIFT                (13)
#define  PPR_FB_PKT6_DERR_DEFAULT              (0x00u)
#define  PPR_FB_PKT6_DERR_OF(x)                _CCL_VALUEOF(_PPR_FB_PKT6_TYPE,x)
#define  PPR_FB_PKT6_DERR_OK                   (0x00u)
#define  PPR_FB_PKT6_DERR_ERROR                (0x01u)

#define _PPR_FB_PKT6_HCERR_MASK                (0x00001000u)
#define _PPR_FB_PKT6_HCERR_SHIFT               (12)
#define  PPR_FB_PKT6_HCERR_DEFAULT             (0x00u)
#define  PPR_FB_PKT6_HCERR_OF(x)               _CCL_VALUEOF(_PPR_FB_PKT6_TYPE,x)
#define  PPR_FB_PKT6_HCERR_OK                  (0x00u)
#define  PPR_FB_PKT6_HCERR_ERROR               (0x01u)

#define _PPR_FB_PKT6_DCODE_MASK                (0x00000F00u)
#define _PPR_FB_PKT6_DCODE_SHIFT               (8)
#define  PPR_FB_PKT6_DCODE_DEFAULT             (0x00u)
#define  PPR_FB_PKT6_DCODE_OF(x)               _DCODEL_VALUEOF(_PPR_FB_PKT6_TYPE,x)

#define _PPR_FB_PKT6_PKT_NUM_MASK              (0x000000F8u)
#define _PPR_FB_PKT6_PKT_NUM_SHIFT             (3)
#define  PPR_FB_PKT6_PKT_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT6_PKT_NUM_OF(x)             _PKT_NUML_VALUEOF(_PPR_FB_PKT6_TYPE,x)

#define _PPR_FB_PKT6_MAG_NUM_MASK              (0x00000007u)
#define _PPR_FB_PKT6_MAG_NUM_SHIFT             (0)
#define  PPR_FB_PKT6_MAG_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT6_MAG_NUM_OF(x)             _MAG_NUML_VALUEOF(_PPR_FB_PKT6_TYPE,x)

#define  PPR_FB_PKT6_OF(x)             _CCL_VALUEOF(_PPR_FB_PKT6_TYPE,x)

#define  PPR_FB_PKT6_DEFAULT (_PPR_FB_PKT6_TYPE)(   \
	 _CCL_PER_FDEFAULT(PPR,FB_PKT6,DERR   )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT6,HCERR  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT6,DCODE  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT6,PKT_NUM)         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT6,MAG_NUM)         \
	)

#define PPR_FB_PKT6_RMK(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT6_TYPE)( \
	 _CCL_PER_FMK  (PPR,FB_PKT6,DERR   ,derr   )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT6,HCERR  ,hcerr  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT6,DCODE  ,dcode  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT6,PKT_NUM,pkt_num)                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT6,MAG_NUM,mag_num)                                   \
	)

#define PPR_FB_PKT6_RMKS(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT6_TYPE)( \
	 _CCL_PER_FMKS (PPR,FB_PKT6,DERR   ,derr   )                                    \
	|_CCL_PER_FMKS (PPR,FB_PKT6,HCERR  ,hcerr  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT6,DCODE  ,dcode  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT6,PKT_NUM,pkt_num)                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT6,MAG_NUM,mag_num)                                    \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  F B    P K T 7   |
 * |___________________|
 *
 * FB_PKT7 - Packet 7 of field buffer Status Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (r)  DERR - The packet exists odd parity error.
 *             This bit is valid only for packet X/0).
 *                   DERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Odd-parity error
 *
 * (r)  HCERR -  the packet exists hamming error.
 *                  HCERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Hamming error
 *
 * (r)  DCODE -  Page designation code (Hamming 8/4 Decoded).
 *               This field is valid only for packet X/26~X31.
 *
 * (r)  PKT_NUM - Page packet number (Hamming 8/4 Decoded)
 *
 * (r)  MAG_NUM - Page magazine number (Hamming 8/4 Decoded)
\*-------------------------------------------------------------------------*/
#define _PPR_FB_PKT7_OFFSET                    (0x0128u)
#define _PPR_FB_PKT7_ADDRH(h)                  PPR_ADDRH(h,FB_PKT7)
#define _PPR_FB_PKT7_LEN                       (32)                 /* 32-bit*/
#define _PPR_FB_PKT7_TYPE                      uint32_t

#define _PPR_FB_PKT7_DERR_MASK                 (0x00002000u)
#define _PPR_FB_PKT7_DERR_SHIFT                (13)
#define  PPR_FB_PKT7_DERR_DEFAULT              (0x00u)
#define  PPR_FB_PKT7_DERR_OF(x)                _CCL_VALUEOF(_PPR_FB_PKT7_TYPE,x)
#define  PPR_FB_PKT7_DERR_OK                   (0x00u)
#define  PPR_FB_PKT7_DERR_ERROR                (0x01u)

#define _PPR_FB_PKT7_HCERR_MASK                (0x00001000u)
#define _PPR_FB_PKT7_HCERR_SHIFT               (12)
#define  PPR_FB_PKT7_HCERR_DEFAULT             (0x00u)
#define  PPR_FB_PKT7_HCERR_OF(x)               _CCL_VALUEOF(_PPR_FB_PKT7_TYPE,x)
#define  PPR_FB_PKT7_HCERR_OK                  (0x00u)
#define  PPR_FB_PKT7_HCERR_ERROR               (0x01u)

#define _PPR_FB_PKT7_DCODE_MASK                (0x00000F00u)
#define _PPR_FB_PKT7_DCODE_SHIFT               (8)
#define  PPR_FB_PKT7_DCODE_DEFAULT             (0x00u)
#define  PPR_FB_PKT7_DCODE_OF(x)               _DCODEL_VALUEOF(_PPR_FB_PKT7_TYPE,x)

#define _PPR_FB_PKT7_PKT_NUM_MASK              (0x000000F8u)
#define _PPR_FB_PKT7_PKT_NUM_SHIFT             (3)
#define  PPR_FB_PKT7_PKT_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT7_PKT_NUM_OF(x)             _PKT_NUML_VALUEOF(_PPR_FB_PKT7_TYPE,x)

#define _PPR_FB_PKT7_MAG_NUM_MASK              (0x00000007u)
#define _PPR_FB_PKT7_MAG_NUM_SHIFT             (0)
#define  PPR_FB_PKT7_MAG_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT7_MAG_NUM_OF(x)             _MAG_NUML_VALUEOF(_PPR_FB_PKT7_TYPE,x)

#define  PPR_FB_PKT7_OF(x)             _CCL_VALUEOF(_PPR_FB_PKT7_TYPE,x)

#define  PPR_FB_PKT7_DEFAULT (_PPR_FB_PKT7_TYPE)(   \
	 _CCL_PER_FDEFAULT(PPR,FB_PKT7,DERR   )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT7,HCERR  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT7,DCODE  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT7,PKT_NUM)         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT7,MAG_NUM)         \
	)

#define PPR_FB_PKT7_RMK(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT7_TYPE)( \
	 _CCL_PER_FMK  (PPR,FB_PKT7,DERR   ,derr   )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT7,HCERR  ,hcerr  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT7,DCODE  ,dcode  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT7,PKT_NUM,pkt_num)                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT7,MAG_NUM,mag_num)                                   \
	)

#define PPR_FB_PKT7_RMKS(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT7_TYPE)( \
	 _CCL_PER_FMKS (PPR,FB_PKT7,DERR   ,derr   )                                    \
	|_CCL_PER_FMKS (PPR,FB_PKT7,HCERR  ,hcerr  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT7,DCODE  ,dcode  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT7,PKT_NUM,pkt_num)                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT7,MAG_NUM,mag_num)                                    \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  F B    P K T 8   |
 * |___________________|
 *
 * FB_PKT8 - Packet 8 of field buffer Status Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (r)  DERR - The packet exists odd parity error.
 *             This bit is valid only for packet X/0).
 *                   DERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Odd-parity error
 *
 * (r)  HCERR -  the packet exists hamming error.
 *                  HCERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Hamming error
 *
 * (r)  DCODE -  Page designation code (Hamming 8/4 Decoded).
 *               This field is valid only for packet X/26~X31.
 *
 * (r)  PKT_NUM - Page packet number (Hamming 8/4 Decoded)
 *
 * (r)  MAG_NUM - Page magazine number (Hamming 8/4 Decoded)
\*-------------------------------------------------------------------------*/
#define _PPR_FB_PKT8_OFFSET                    (0x012Cu)
#define _PPR_FB_PKT8_ADDRH(h)                  PPR_ADDRH(h,FB_PKT8)
#define _PPR_FB_PKT8_LEN                       (32)                 /* 32-bit*/
#define _PPR_FB_PKT8_TYPE                      uint32_t

#define _PPR_FB_PKT8_DERR_MASK                 (0x00002000u)
#define _PPR_FB_PKT8_DERR_SHIFT                (13)
#define  PPR_FB_PKT8_DERR_DEFAULT              (0x00u)
#define  PPR_FB_PKT8_DERR_OF(x)                _CCL_VALUEOF(_PPR_FB_PKT8_TYPE,x)
#define  PPR_FB_PKT8_DERR_OK                   (0x00u)
#define  PPR_FB_PKT8_DERR_ERROR                (0x01u)

#define _PPR_FB_PKT8_HCERR_MASK                (0x00001000u)
#define _PPR_FB_PKT8_HCERR_SHIFT               (12)
#define  PPR_FB_PKT8_HCERR_DEFAULT             (0x00u)
#define  PPR_FB_PKT8_HCERR_OF(x)               _CCL_VALUEOF(_PPR_FB_PKT8_TYPE,x)
#define  PPR_FB_PKT8_HCERR_OK                  (0x00u)
#define  PPR_FB_PKT8_HCERR_ERROR               (0x01u)

#define _PPR_FB_PKT8_DCODE_MASK                (0x00000F00u)
#define _PPR_FB_PKT8_DCODE_SHIFT               (8)
#define  PPR_FB_PKT8_DCODE_DEFAULT             (0x00u)
#define  PPR_FB_PKT8_DCODE_OF(x)               _DCODEL_VALUEOF(_PPR_FB_PKT8_TYPE,x)

#define _PPR_FB_PKT8_PKT_NUM_MASK              (0x000000F8u)
#define _PPR_FB_PKT8_PKT_NUM_SHIFT             (3)
#define  PPR_FB_PKT8_PKT_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT8_PKT_NUM_OF(x)             _PKT_NUML_VALUEOF(_PPR_FB_PKT8_TYPE,x)

#define _PPR_FB_PKT8_MAG_NUM_MASK              (0x00000007u)
#define _PPR_FB_PKT8_MAG_NUM_SHIFT             (0)
#define  PPR_FB_PKT8_MAG_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT8_MAG_NUM_OF(x)             _MAG_NUML_VALUEOF(_PPR_FB_PKT8_TYPE,x)

#define  PPR_FB_PKT8_OF(x)             _CCL_VALUEOF(_PPR_FB_PKT8_TYPE,x)

#define  PPR_FB_PKT8_DEFAULT (_PPR_FB_PKT8_TYPE)(   \
	 _CCL_PER_FDEFAULT(PPR,FB_PKT8,DERR   )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT8,HCERR  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT8,DCODE  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT8,PKT_NUM)         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT8,MAG_NUM)         \
	)

#define PPR_FB_PKT8_RMK(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT8_TYPE)( \
	 _CCL_PER_FMK  (PPR,FB_PKT8,DERR   ,derr   )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT8,HCERR  ,hcerr  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT8,DCODE  ,dcode  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT8,PKT_NUM,pkt_num)                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT8,MAG_NUM,mag_num)                                   \
	)

#define PPR_FB_PKT8_RMKS(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT8_TYPE)( \
	 _CCL_PER_FMKS (PPR,FB_PKT8,DERR   ,derr   )                                    \
	|_CCL_PER_FMKS (PPR,FB_PKT8,HCERR  ,hcerr  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT8,DCODE  ,dcode  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT8,PKT_NUM,pkt_num)                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT8,MAG_NUM,mag_num)                                    \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  F B    P K T 9   |
 * |___________________|
 *
 * FB_PKT9 - Packet 9 of field buffer Status Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (r)  DERR - The packet exists odd parity error.
 *             This bit is valid only for packet X/0).
 *                   DERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Odd-parity error
 *
 * (r)  HCERR -  the packet exists hamming error.
 *                  HCERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Hamming error
 *
 * (r)  DCODE -  Page designation code (Hamming 8/4 Decoded).
 *               This field is valid only for packet X/26~X31.
 *
 * (r)  PKT_NUM - Page packet number (Hamming 8/4 Decoded)
 *
 * (r)  MAG_NUM - Page magazine number (Hamming 8/4 Decoded)
\*-------------------------------------------------------------------------*/
#define _PPR_FB_PKT9_OFFSET                    (0x0130u)
#define _PPR_FB_PKT9_ADDRH(h)                  PPR_ADDRH(h,FB_PKT9)
#define _PPR_FB_PKT9_LEN                       (32)                 /* 32-bit*/
#define _PPR_FB_PKT9_TYPE                      uint32_t

#define _PPR_FB_PKT9_DERR_MASK                 (0x00002000u)
#define _PPR_FB_PKT9_DERR_SHIFT                (13)
#define  PPR_FB_PKT9_DERR_DEFAULT              (0x00u)
#define  PPR_FB_PKT9_DERR_OF(x)                _CCL_VALUEOF(_PPR_FB_PKT9_TYPE,x)
#define  PPR_FB_PKT9_DERR_OK                   (0x00u)
#define  PPR_FB_PKT9_DERR_ERROR                (0x01u)

#define _PPR_FB_PKT9_HCERR_MASK                (0x00001000u)
#define _PPR_FB_PKT9_HCERR_SHIFT               (12)
#define  PPR_FB_PKT9_HCERR_DEFAULT             (0x00u)
#define  PPR_FB_PKT9_HCERR_OF(x)               _CCL_VALUEOF(_PPR_FB_PKT9_TYPE,x)
#define  PPR_FB_PKT9_HCERR_OK                  (0x00u)
#define  PPR_FB_PKT9_HCERR_ERROR               (0x01u)

#define _PPR_FB_PKT9_DCODE_MASK                (0x00000F00u)
#define _PPR_FB_PKT9_DCODE_SHIFT               (8)
#define  PPR_FB_PKT9_DCODE_DEFAULT             (0x00u)
#define  PPR_FB_PKT9_DCODE_OF(x)               _DCODEL_VALUEOF(_PPR_FB_PKT9_TYPE,x)

#define _PPR_FB_PKT9_PKT_NUM_MASK              (0x000000F8u)
#define _PPR_FB_PKT9_PKT_NUM_SHIFT             (3)
#define  PPR_FB_PKT9_PKT_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT9_PKT_NUM_OF(x)             _PKT_NUML_VALUEOF(_PPR_FB_PKT9_TYPE,x)

#define _PPR_FB_PKT9_MAG_NUM_MASK              (0x00000007u)
#define _PPR_FB_PKT9_MAG_NUM_SHIFT             (0)
#define  PPR_FB_PKT9_MAG_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT9_MAG_NUM_OF(x)             _MAG_NUML_VALUEOF(_PPR_FB_PKT9_TYPE,x)

#define  PPR_FB_PKT9_OF(x)             _CCL_VALUEOF(_PPR_FB_PKT9_TYPE,x)

#define  PPR_FB_PKT9_DEFAULT (_PPR_FB_PKT9_TYPE)(   \
	 _CCL_PER_FDEFAULT(PPR,FB_PKT9,DERR   )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT9,HCERR  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT9,DCODE  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT9,PKT_NUM)         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT9,MAG_NUM)         \
	)

#define PPR_FB_PKT9_RMK(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT9_TYPE)( \
	 _CCL_PER_FMK  (PPR,FB_PKT9,DERR   ,derr   )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT9,HCERR  ,hcerr  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT9,DCODE  ,dcode  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT9,PKT_NUM,pkt_num)                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT9,MAG_NUM,mag_num)                                   \
	)

#define PPR_FB_PKT9_RMKS(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT9_TYPE)( \
	 _CCL_PER_FMKS (PPR,FB_PKT9,DERR   ,derr   )                                    \
	|_CCL_PER_FMKS (PPR,FB_PKT9,HCERR  ,hcerr  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT9,DCODE  ,dcode  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT9,PKT_NUM,pkt_num)                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT9,MAG_NUM,mag_num)                                    \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  F B    P K T 1 0 |
 * |___________________|
 *
 * FB_PKT10 - Packet 10 of field buffer Status Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (r)  DERR - The packet exists odd parity error.
 *             This bit is valid only for packet X/0).
 *                   DERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Odd-parity error
 *
 * (r)  HCERR -  the packet exists hamming error.
 *                  HCERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Hamming error
 *
 * (r)  DCODE -  Page designation code (Hamming 8/4 Decoded).
 *               This field is valid only for packet X/26~X31.
 *
 * (r)  PKT_NUM - Page packet number (Hamming 8/4 Decoded)
 *
 * (r)  MAG_NUM - Page magazine number (Hamming 8/4 Decoded)
\*-------------------------------------------------------------------------*/
#define _PPR_FB_PKT10_OFFSET                    (0x0134u)
#define _PPR_FB_PKT10_ADDRH(h)                  PPR_ADDRH(h,FB_PKT10)
#define _PPR_FB_PKT10_LEN                       (32)                 /* 32-bit*/
#define _PPR_FB_PKT10_TYPE                      uint32_t

#define _PPR_FB_PKT10_DERR_MASK                 (0x00002000u)
#define _PPR_FB_PKT10_DERR_SHIFT                (13)
#define  PPR_FB_PKT10_DERR_DEFAULT              (0x00u)
#define  PPR_FB_PKT10_DERR_OF(x)                _CCL_VALUEOF(_PPR_FB_PKT10_TYPE,x)
#define  PPR_FB_PKT10_DERR_OK                   (0x00u)
#define  PPR_FB_PKT10_DERR_ERROR                (0x01u)

#define _PPR_FB_PKT10_HCERR_MASK                (0x00001000u)
#define _PPR_FB_PKT10_HCERR_SHIFT               (12)
#define  PPR_FB_PKT10_HCERR_DEFAULT             (0x00u)
#define  PPR_FB_PKT10_HCERR_OF(x)               _CCL_VALUEOF(_PPR_FB_PKT10_TYPE,x)
#define  PPR_FB_PKT10_HCERR_OK                  (0x00u)
#define  PPR_FB_PKT10_HCERR_ERROR               (0x01u)

#define _PPR_FB_PKT10_DCODE_MASK                (0x00000F00u)
#define _PPR_FB_PKT10_DCODE_SHIFT               (8)
#define  PPR_FB_PKT10_DCODE_DEFAULT             (0x00u)
#define  PPR_FB_PKT10_DCODE_OF(x)               _DCODEL_VALUEOF(_PPR_FB_PKT10_TYPE,x)

#define _PPR_FB_PKT10_PKT_NUM_MASK              (0x000000F8u)
#define _PPR_FB_PKT10_PKT_NUM_SHIFT             (3)
#define  PPR_FB_PKT10_PKT_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT10_PKT_NUM_OF(x)             _PKT_NUML_VALUEOF(_PPR_FB_PKT10_TYPE,x)

#define _PPR_FB_PKT10_MAG_NUM_MASK              (0x00000007u)
#define _PPR_FB_PKT10_MAG_NUM_SHIFT             (0)
#define  PPR_FB_PKT10_MAG_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT10_MAG_NUM_OF(x)             _MAG_NUML_VALUEOF(_PPR_FB_PKT10_TYPE,x)

#define  PPR_FB_PKT10_OF(x)             _CCL_VALUEOF(_PPR_FB_PKT10_TYPE,x)

#define  PPR_FB_PKT10_DEFAULT (_PPR_FB_PKT10_TYPE)(   \
	 _CCL_PER_FDEFAULT(PPR,FB_PKT10,DERR   )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT10,HCERR  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT10,DCODE  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT10,PKT_NUM)         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT10,MAG_NUM)         \
	)

#define PPR_FB_PKT10_RMK(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT10_TYPE)( \
	 _CCL_PER_FMK  (PPR,FB_PKT10,DERR   ,derr   )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT10,HCERR  ,hcerr  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT10,DCODE  ,dcode  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT10,PKT_NUM,pkt_num)                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT10,MAG_NUM,mag_num)                                   \
	)

#define PPR_FB_PKT10_RMKS(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT10_TYPE)( \
	 _CCL_PER_FMKS (PPR,FB_PKT10,DERR   ,derr   )                                    \
	|_CCL_PER_FMKS (PPR,FB_PKT10,HCERR  ,hcerr  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT10,DCODE  ,dcode  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT10,PKT_NUM,pkt_num)                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT10,MAG_NUM,mag_num)                                    \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  F B    P K T 1 1 |
 * |___________________|
 *
 * FB_PKT11 - Packet 11 of field buffer Status Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (r)  DERR - The packet exists odd parity error.
 *             This bit is valid only for packet X/0).
 *                   DERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Odd-parity error
 *
 * (r)  HCERR -  the packet exists hamming error.
 *                  HCERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Hamming error
 *
 * (r)  DCODE -  Page designation code (Hamming 8/4 Decoded).
 *               This field is valid only for packet X/26~X31.
 *
 * (r)  PKT_NUM - Page packet number (Hamming 8/4 Decoded)
 *
 * (r)  MAG_NUM - Page magazine number (Hamming 8/4 Decoded)
\*-------------------------------------------------------------------------*/
#define _PPR_FB_PKT11_OFFSET                    (0x0138u)
#define _PPR_FB_PKT11_ADDRH(h)                  PPR_ADDRH(h,FB_PKT11)
#define _PPR_FB_PKT11_LEN                       (32)                 /* 32-bit*/
#define _PPR_FB_PKT11_TYPE                      uint32_t

#define _PPR_FB_PKT11_DERR_MASK                 (0x00002000u)
#define _PPR_FB_PKT11_DERR_SHIFT                (13)
#define  PPR_FB_PKT11_DERR_DEFAULT              (0x00u)
#define  PPR_FB_PKT11_DERR_OF(x)                _CCL_VALUEOF(_PPR_FB_PKT11_TYPE,x)
#define  PPR_FB_PKT11_DERR_OK                   (0x00u)
#define  PPR_FB_PKT11_DERR_ERROR                (0x01u)

#define _PPR_FB_PKT11_HCERR_MASK                (0x00001000u)
#define _PPR_FB_PKT11_HCERR_SHIFT               (12)
#define  PPR_FB_PKT11_HCERR_DEFAULT             (0x00u)
#define  PPR_FB_PKT11_HCERR_OF(x)               _CCL_VALUEOF(_PPR_FB_PKT11_TYPE,x)
#define  PPR_FB_PKT11_HCERR_OK                  (0x00u)
#define  PPR_FB_PKT11_HCERR_ERROR               (0x01u)

#define _PPR_FB_PKT11_DCODE_MASK                (0x00000F00u)
#define _PPR_FB_PKT11_DCODE_SHIFT               (8)
#define  PPR_FB_PKT11_DCODE_DEFAULT             (0x00u)
#define  PPR_FB_PKT11_DCODE_OF(x)               _DCODEL_VALUEOF(_PPR_FB_PKT11_TYPE,x)

#define _PPR_FB_PKT11_PKT_NUM_MASK              (0x000000F8u)
#define _PPR_FB_PKT11_PKT_NUM_SHIFT             (3)
#define  PPR_FB_PKT11_PKT_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT11_PKT_NUM_OF(x)             _PKT_NUML_VALUEOF(_PPR_FB_PKT11_TYPE,x)

#define _PPR_FB_PKT11_MAG_NUM_MASK              (0x00000007u)
#define _PPR_FB_PKT11_MAG_NUM_SHIFT             (0)
#define  PPR_FB_PKT11_MAG_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT11_MAG_NUM_OF(x)             _MAG_NUML_VALUEOF(_PPR_FB_PKT11_TYPE,x)

#define  PPR_FB_PKT11_OF(x)             _CCL_VALUEOF(_PPR_FB_PKT11_TYPE,x)

#define  PPR_FB_PKT11_DEFAULT (_PPR_FB_PKT11_TYPE)(   \
	 _CCL_PER_FDEFAULT(PPR,FB_PKT11,DERR   )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT11,HCERR  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT11,DCODE  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT11,PKT_NUM)         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT11,MAG_NUM)         \
	)

#define PPR_FB_PKT11_RMK(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT11_TYPE)( \
	 _CCL_PER_FMK  (PPR,FB_PKT11,DERR   ,derr   )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT11,HCERR  ,hcerr  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT11,DCODE  ,dcode  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT11,PKT_NUM,pkt_num)                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT11,MAG_NUM,mag_num)                                   \
	)

#define PPR_FB_PKT11_RMKS(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT11_TYPE)( \
	 _CCL_PER_FMKS (PPR,FB_PKT11,DERR   ,derr   )                                    \
	|_CCL_PER_FMKS (PPR,FB_PKT11,HCERR  ,hcerr  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT11,DCODE  ,dcode  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT11,PKT_NUM,pkt_num)                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT11,MAG_NUM,mag_num)                                    \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  F B    P K T 1 2 |
 * |___________________|
 *
 * FB_PKT12 - Packet 12 of field buffer Status Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (r)  DERR - The packet exists odd parity error.
 *             This bit is valid only for packet X/0).
 *                   DERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Odd-parity error
 *
 * (r)  HCERR -  the packet exists hamming error.
 *                  HCERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Hamming error
 *
 * (r)  DCODE -  Page designation code (Hamming 8/4 Decoded).
 *               This field is valid only for packet X/26~X31.
 *
 * (r)  PKT_NUM - Page packet number (Hamming 8/4 Decoded)
 *
 * (r)  MAG_NUM - Page magazine number (Hamming 8/4 Decoded)
\*-------------------------------------------------------------------------*/
#define _PPR_FB_PKT12_OFFSET                    (0x013Cu)
#define _PPR_FB_PKT12_ADDRH(h)                  PPR_ADDRH(h,FB_PKT12)
#define _PPR_FB_PKT12_LEN                       (32)                 /* 32-bit*/
#define _PPR_FB_PKT12_TYPE                      uint32_t

#define _PPR_FB_PKT12_DERR_MASK                 (0x00002000u)
#define _PPR_FB_PKT12_DERR_SHIFT                (13)
#define  PPR_FB_PKT12_DERR_DEFAULT              (0x00u)
#define  PPR_FB_PKT12_DERR_OF(x)                _CCL_VALUEOF(_PPR_FB_PKT12_TYPE,x)
#define  PPR_FB_PKT12_DERR_OK                   (0x00u)
#define  PPR_FB_PKT12_DERR_ERROR                (0x01u)

#define _PPR_FB_PKT12_HCERR_MASK                (0x00001000u)
#define _PPR_FB_PKT12_HCERR_SHIFT               (12)
#define  PPR_FB_PKT12_HCERR_DEFAULT             (0x00u)
#define  PPR_FB_PKT12_HCERR_OF(x)               _CCL_VALUEOF(_PPR_FB_PKT12_TYPE,x)
#define  PPR_FB_PKT12_HCERR_OK                  (0x00u)
#define  PPR_FB_PKT12_HCERR_ERROR               (0x01u)

#define _PPR_FB_PKT12_DCODE_MASK                (0x00000F00u)
#define _PPR_FB_PKT12_DCODE_SHIFT               (8)
#define  PPR_FB_PKT12_DCODE_DEFAULT             (0x00u)
#define  PPR_FB_PKT12_DCODE_OF(x)               _DCODEL_VALUEOF(_PPR_FB_PKT12_TYPE,x)

#define _PPR_FB_PKT12_PKT_NUM_MASK              (0x000000F8u)
#define _PPR_FB_PKT12_PKT_NUM_SHIFT             (3)
#define  PPR_FB_PKT12_PKT_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT12_PKT_NUM_OF(x)             _PKT_NUML_VALUEOF(_PPR_FB_PKT12_TYPE,x)

#define _PPR_FB_PKT12_MAG_NUM_MASK              (0x00000007u)
#define _PPR_FB_PKT12_MAG_NUM_SHIFT             (0)
#define  PPR_FB_PKT12_MAG_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT12_MAG_NUM_OF(x)             _MAG_NUML_VALUEOF(_PPR_FB_PKT12_TYPE,x)

#define  PPR_FB_PKT12_OF(x)             _CCL_VALUEOF(_PPR_FB_PKT12_TYPE,x)

#define  PPR_FB_PKT12_DEFAULT (_PPR_FB_PKT12_TYPE)(   \
	 _CCL_PER_FDEFAULT(PPR,FB_PKT12,DERR   )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT12,HCERR  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT12,DCODE  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT12,PKT_NUM)         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT12,MAG_NUM)         \
	)

#define PPR_FB_PKT12_RMK(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT12_TYPE)( \
	 _CCL_PER_FMK  (PPR,FB_PKT12,DERR   ,derr   )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT12,HCERR  ,hcerr  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT12,DCODE  ,dcode  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT12,PKT_NUM,pkt_num)                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT12,MAG_NUM,mag_num)                                   \
	)

#define PPR_FB_PKT12_RMKS(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT12_TYPE)( \
	 _CCL_PER_FMKS (PPR,FB_PKT12,DERR   ,derr   )                                    \
	|_CCL_PER_FMKS (PPR,FB_PKT12,HCERR  ,hcerr  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT12,DCODE  ,dcode  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT12,PKT_NUM,pkt_num)                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT12,MAG_NUM,mag_num)                                    \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  F B    P K T 1 3 |
 * |___________________|
 *
 * FB_PKT13 - Packet 13 of field buffer Status Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (r)  DERR - The packet exists odd parity error.
 *             This bit is valid only for packet X/0).
 *                   DERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Odd-parity error
 *
 * (r)  HCERR -  the packet exists hamming error.
 *                  HCERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Hamming error
 *
 * (r)  DCODE -  Page designation code (Hamming 8/4 Decoded).
 *               This field is valid only for packet X/26~X31.
 *
 * (r)  PKT_NUM - Page packet number (Hamming 8/4 Decoded)
 *
 * (r)  MAG_NUM - Page magazine number (Hamming 8/4 Decoded)
\*-------------------------------------------------------------------------*/
#define _PPR_FB_PKT13_OFFSET                    (0x0140u)
#define _PPR_FB_PKT13_ADDRH(h)                  PPR_ADDRH(h,FB_PKT13)
#define _PPR_FB_PKT13_LEN                       (32)                 /* 32-bit*/
#define _PPR_FB_PKT13_TYPE                      uint32_t

#define _PPR_FB_PKT13_DERR_MASK                 (0x00002000u)
#define _PPR_FB_PKT13_DERR_SHIFT                (13)
#define  PPR_FB_PKT13_DERR_DEFAULT              (0x00u)
#define  PPR_FB_PKT13_DERR_OF(x)                _CCL_VALUEOF(_PPR_FB_PKT13_TYPE,x)
#define  PPR_FB_PKT13_DERR_OK                   (0x00u)
#define  PPR_FB_PKT13_DERR_ERROR                (0x01u)

#define _PPR_FB_PKT13_HCERR_MASK                (0x00001000u)
#define _PPR_FB_PKT13_HCERR_SHIFT               (12)
#define  PPR_FB_PKT13_HCERR_DEFAULT             (0x00u)
#define  PPR_FB_PKT13_HCERR_OF(x)               _CCL_VALUEOF(_PPR_FB_PKT13_TYPE,x)
#define  PPR_FB_PKT13_HCERR_OK                  (0x00u)
#define  PPR_FB_PKT13_HCERR_ERROR               (0x01u)

#define _PPR_FB_PKT13_DCODE_MASK                (0x00000F00u)
#define _PPR_FB_PKT13_DCODE_SHIFT               (8)
#define  PPR_FB_PKT13_DCODE_DEFAULT             (0x00u)
#define  PPR_FB_PKT13_DCODE_OF(x)               _DCODEL_VALUEOF(_PPR_FB_PKT13_TYPE,x)

#define _PPR_FB_PKT13_PKT_NUM_MASK              (0x000000F8u)
#define _PPR_FB_PKT13_PKT_NUM_SHIFT             (3)
#define  PPR_FB_PKT13_PKT_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT13_PKT_NUM_OF(x)             _PKT_NUML_VALUEOF(_PPR_FB_PKT13_TYPE,x)

#define _PPR_FB_PKT13_MAG_NUM_MASK              (0x00000007u)
#define _PPR_FB_PKT13_MAG_NUM_SHIFT             (0)
#define  PPR_FB_PKT13_MAG_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT13_MAG_NUM_OF(x)             _MAG_NUML_VALUEOF(_PPR_FB_PKT13_TYPE,x)

#define  PPR_FB_PKT13_OF(x)             _CCL_VALUEOF(_PPR_FB_PKT13_TYPE,x)

#define  PPR_FB_PKT13_DEFAULT (_PPR_FB_PKT13_TYPE)(   \
	 _CCL_PER_FDEFAULT(PPR,FB_PKT13,DERR   )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT13,HCERR  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT13,DCODE  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT13,PKT_NUM)         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT13,MAG_NUM)         \
	)

#define PPR_FB_PKT13_RMK(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT13_TYPE)( \
	 _CCL_PER_FMK  (PPR,FB_PKT13,DERR   ,derr   )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT13,HCERR  ,hcerr  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT13,DCODE  ,dcode  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT13,PKT_NUM,pkt_num)                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT13,MAG_NUM,mag_num)                                   \
	)

#define PPR_FB_PKT13_RMKS(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT13_TYPE)( \
	 _CCL_PER_FMKS (PPR,FB_PKT13,DERR   ,derr   )                                    \
	|_CCL_PER_FMKS (PPR,FB_PKT13,HCERR  ,hcerr  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT13,DCODE  ,dcode  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT13,PKT_NUM,pkt_num)                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT13,MAG_NUM,mag_num)                                    \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  F B    P K T 1 4 |
 * |___________________|
 *
 * FB_PKT14 - Packet 14 of field buffer Status Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (r)  DERR - The packet exists odd parity error.
 *             This bit is valid only for packet X/0).
 *                   DERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Odd-parity error
 *
 * (r)  HCERR -  the packet exists hamming error.
 *                  HCERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Hamming error
 *
 * (r)  DCODE -  Page designation code (Hamming 8/4 Decoded).
 *               This field is valid only for packet X/26~X31.
 *
 * (r)  PKT_NUM - Page packet number (Hamming 8/4 Decoded)
 *
 * (r)  MAG_NUM - Page magazine number (Hamming 8/4 Decoded)
\*-------------------------------------------------------------------------*/
#define _PPR_FB_PKT14_OFFSET                    (0x0144u)
#define _PPR_FB_PKT14_ADDRH(h)                  PPR_ADDRH(h,FB_PKT14)
#define _PPR_FB_PKT14_LEN                       (32)                 /* 32-bit*/
#define _PPR_FB_PKT14_TYPE                      uint32_t

#define _PPR_FB_PKT14_DERR_MASK                 (0x00002000u)
#define _PPR_FB_PKT14_DERR_SHIFT                (13)
#define  PPR_FB_PKT14_DERR_DEFAULT              (0x00u)
#define  PPR_FB_PKT14_DERR_OF(x)                _CCL_VALUEOF(_PPR_FB_PKT14_TYPE,x)
#define  PPR_FB_PKT14_DERR_OK                   (0x00u)
#define  PPR_FB_PKT14_DERR_ERROR                (0x01u)

#define _PPR_FB_PKT14_HCERR_MASK                (0x00001000u)
#define _PPR_FB_PKT14_HCERR_SHIFT               (12)
#define  PPR_FB_PKT14_HCERR_DEFAULT             (0x00u)
#define  PPR_FB_PKT14_HCERR_OF(x)               _CCL_VALUEOF(_PPR_FB_PKT14_TYPE,x)
#define  PPR_FB_PKT14_HCERR_OK                  (0x00u)
#define  PPR_FB_PKT14_HCERR_ERROR               (0x01u)

#define _PPR_FB_PKT14_DCODE_MASK                (0x00000F00u)
#define _PPR_FB_PKT14_DCODE_SHIFT               (8)
#define  PPR_FB_PKT14_DCODE_DEFAULT             (0x00u)
#define  PPR_FB_PKT14_DCODE_OF(x)               _DCODEL_VALUEOF(_PPR_FB_PKT14_TYPE,x)

#define _PPR_FB_PKT14_PKT_NUM_MASK              (0x000000F8u)
#define _PPR_FB_PKT14_PKT_NUM_SHIFT             (3)
#define  PPR_FB_PKT14_PKT_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT14_PKT_NUM_OF(x)             _PKT_NUML_VALUEOF(_PPR_FB_PKT14_TYPE,x)

#define _PPR_FB_PKT14_MAG_NUM_MASK              (0x00000007u)
#define _PPR_FB_PKT14_MAG_NUM_SHIFT             (0)
#define  PPR_FB_PKT14_MAG_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT14_MAG_NUM_OF(x)             _MAG_NUML_VALUEOF(_PPR_FB_PKT14_TYPE,x)

#define  PPR_FB_PKT14_OF(x)             _CCL_VALUEOF(_PPR_FB_PKT14_TYPE,x)

#define  PPR_FB_PKT14_DEFAULT (_PPR_FB_PKT14_TYPE)(   \
	 _CCL_PER_FDEFAULT(PPR,FB_PKT14,DERR   )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT14,HCERR  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT14,DCODE  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT14,PKT_NUM)         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT14,MAG_NUM)         \
	)

#define PPR_FB_PKT14_RMK(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT14_TYPE)( \
	 _CCL_PER_FMK  (PPR,FB_PKT14,DERR   ,derr   )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT14,HCERR  ,hcerr  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT14,DCODE  ,dcode  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT14,PKT_NUM,pkt_num)                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT14,MAG_NUM,mag_num)                                   \
	)

#define PPR_FB_PKT14_RMKS(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT14_TYPE)( \
	 _CCL_PER_FMKS (PPR,FB_PKT14,DERR   ,derr   )                                    \
	|_CCL_PER_FMKS (PPR,FB_PKT14,HCERR  ,hcerr  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT14,DCODE  ,dcode  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT14,PKT_NUM,pkt_num)                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT14,MAG_NUM,mag_num)                                    \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  F B    P K T 1 5 |
 * |___________________|
 *
 * FB_PKT15 - Packet 15 of field buffer Status Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (r)  DERR - The packet exists odd parity error.
 *             This bit is valid only for packet X/0).
 *                   DERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Odd-parity error
 *
 * (r)  HCERR -  the packet exists hamming error.
 *                  HCERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Hamming error
 *
 * (r)  DCODE -  Page designation code (Hamming 8/4 Decoded).
 *               This field is valid only for packet X/26~X31.
 *
 * (r)  PKT_NUM - Page packet number (Hamming 8/4 Decoded)
 *
 * (r)  MAG_NUM - Page magazine number (Hamming 8/4 Decoded)
\*-------------------------------------------------------------------------*/
#define _PPR_FB_PKT15_OFFSET                    (0x0148u)
#define _PPR_FB_PKT15_ADDRH(h)                  PPR_ADDRH(h,FB_PKT15)
#define _PPR_FB_PKT15_LEN                       (32)                 /* 32-bit*/
#define _PPR_FB_PKT15_TYPE                      uint32_t

#define _PPR_FB_PKT15_DERR_MASK                 (0x00002000u)
#define _PPR_FB_PKT15_DERR_SHIFT                (13)
#define  PPR_FB_PKT15_DERR_DEFAULT              (0x00u)
#define  PPR_FB_PKT15_DERR_OF(x)                _CCL_VALUEOF(_PPR_FB_PKT15_TYPE,x)
#define  PPR_FB_PKT15_DERR_OK                   (0x00u)
#define  PPR_FB_PKT15_DERR_ERROR                (0x01u)

#define _PPR_FB_PKT15_HCERR_MASK                (0x00001000u)
#define _PPR_FB_PKT15_HCERR_SHIFT               (12)
#define  PPR_FB_PKT15_HCERR_DEFAULT             (0x00u)
#define  PPR_FB_PKT15_HCERR_OF(x)               _CCL_VALUEOF(_PPR_FB_PKT15_TYPE,x)
#define  PPR_FB_PKT15_HCERR_OK                  (0x00u)
#define  PPR_FB_PKT15_HCERR_ERROR               (0x01u)

#define _PPR_FB_PKT15_DCODE_MASK                (0x00000F00u)
#define _PPR_FB_PKT15_DCODE_SHIFT               (8)
#define  PPR_FB_PKT15_DCODE_DEFAULT             (0x00u)
#define  PPR_FB_PKT15_DCODE_OF(x)               _DCODEL_VALUEOF(_PPR_FB_PKT15_TYPE,x)

#define _PPR_FB_PKT15_PKT_NUM_MASK              (0x000000F8u)
#define _PPR_FB_PKT15_PKT_NUM_SHIFT             (3)
#define  PPR_FB_PKT15_PKT_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT15_PKT_NUM_OF(x)             _PKT_NUML_VALUEOF(_PPR_FB_PKT15_TYPE,x)

#define _PPR_FB_PKT15_MAG_NUM_MASK              (0x00000007u)
#define _PPR_FB_PKT15_MAG_NUM_SHIFT             (0)
#define  PPR_FB_PKT15_MAG_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT15_MAG_NUM_OF(x)             _MAG_NUML_VALUEOF(_PPR_FB_PKT15_TYPE,x)

#define  PPR_FB_PKT15_OF(x)             _CCL_VALUEOF(_PPR_FB_PKT15_TYPE,x)

#define  PPR_FB_PKT15_DEFAULT (_PPR_FB_PKT15_TYPE)(   \
	 _CCL_PER_FDEFAULT(PPR,FB_PKT15,DERR   )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT15,HCERR  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT15,DCODE  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT15,PKT_NUM)         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT15,MAG_NUM)         \
	)

#define PPR_FB_PKT15_RMK(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT15_TYPE)( \
	 _CCL_PER_FMK  (PPR,FB_PKT15,DERR   ,derr   )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT15,HCERR  ,hcerr  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT15,DCODE  ,dcode  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT15,PKT_NUM,pkt_num)                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT15,MAG_NUM,mag_num)                                   \
	)

#define PPR_FB_PKT15_RMKS(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT15_TYPE)( \
	 _CCL_PER_FMKS (PPR,FB_PKT15,DERR   ,derr   )                                    \
	|_CCL_PER_FMKS (PPR,FB_PKT15,HCERR  ,hcerr  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT15,DCODE  ,dcode  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT15,PKT_NUM,pkt_num)                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT15,MAG_NUM,mag_num)                                    \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  F B    P K T 1 6 |
 * |___________________|
 *
 * FB_PKT16 - Packet 16 of field buffer Status Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (r)  DERR - The packet exists odd parity error.
 *             This bit is valid only for packet X/0).
 *                   DERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Odd-parity error
 *
 * (r)  HCERR -  the packet exists hamming error.
 *                  HCERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Hamming error
 *
 * (r)  DCODE -  Page designation code (Hamming 8/4 Decoded).
 *               This field is valid only for packet X/26~X31.
 *
 * (r)  PKT_NUM - Page packet number (Hamming 8/4 Decoded)
 *
 * (r)  MAG_NUM - Page magazine number (Hamming 8/4 Decoded)
\*-------------------------------------------------------------------------*/
#define _PPR_FB_PKT16_OFFSET                    (0x014Cu)
#define _PPR_FB_PKT16_ADDRH(h)                  PPR_ADDRH(h,FB_PKT16)
#define _PPR_FB_PKT16_LEN                       (32)                 /* 32-bit*/
#define _PPR_FB_PKT16_TYPE                      uint32_t

#define _PPR_FB_PKT16_DERR_MASK                 (0x00002000u)
#define _PPR_FB_PKT16_DERR_SHIFT                (13)
#define  PPR_FB_PKT16_DERR_DEFAULT              (0x00u)
#define  PPR_FB_PKT16_DERR_OF(x)                _CCL_VALUEOF(_PPR_FB_PKT16_TYPE,x)
#define  PPR_FB_PKT16_DERR_OK                   (0x00u)
#define  PPR_FB_PKT16_DERR_ERROR                (0x01u)

#define _PPR_FB_PKT16_HCERR_MASK                (0x00001000u)
#define _PPR_FB_PKT16_HCERR_SHIFT               (12)
#define  PPR_FB_PKT16_HCERR_DEFAULT             (0x00u)
#define  PPR_FB_PKT16_HCERR_OF(x)               _CCL_VALUEOF(_PPR_FB_PKT16_TYPE,x)
#define  PPR_FB_PKT16_HCERR_OK                  (0x00u)
#define  PPR_FB_PKT16_HCERR_ERROR               (0x01u)

#define _PPR_FB_PKT16_DCODE_MASK                (0x00000F00u)
#define _PPR_FB_PKT16_DCODE_SHIFT               (8)
#define  PPR_FB_PKT16_DCODE_DEFAULT             (0x00u)
#define  PPR_FB_PKT16_DCODE_OF(x)               _DCODEL_VALUEOF(_PPR_FB_PKT16_TYPE,x)

#define _PPR_FB_PKT16_PKT_NUM_MASK              (0x000000F8u)
#define _PPR_FB_PKT16_PKT_NUM_SHIFT             (3)
#define  PPR_FB_PKT16_PKT_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT16_PKT_NUM_OF(x)             _PKT_NUML_VALUEOF(_PPR_FB_PKT16_TYPE,x)

#define _PPR_FB_PKT16_MAG_NUM_MASK              (0x00000007u)
#define _PPR_FB_PKT16_MAG_NUM_SHIFT             (0)
#define  PPR_FB_PKT16_MAG_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT16_MAG_NUM_OF(x)             _MAG_NUML_VALUEOF(_PPR_FB_PKT16_TYPE,x)

#define  PPR_FB_PKT16_OF(x)             _CCL_VALUEOF(_PPR_FB_PKT16_TYPE,x)

#define  PPR_FB_PKT16_DEFAULT (_PPR_FB_PKT16_TYPE)(   \
	 _CCL_PER_FDEFAULT(PPR,FB_PKT16,DERR   )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT16,HCERR  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT16,DCODE  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT16,PKT_NUM)         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT16,MAG_NUM)         \
	)

#define PPR_FB_PKT16_RMK(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT16_TYPE)( \
	 _CCL_PER_FMK  (PPR,FB_PKT16,DERR   ,derr   )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT16,HCERR  ,hcerr  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT16,DCODE  ,dcode  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT16,PKT_NUM,pkt_num)                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT16,MAG_NUM,mag_num)                                   \
	)

#define PPR_FB_PKT16_RMKS(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT16_TYPE)( \
	 _CCL_PER_FMKS (PPR,FB_PKT16,DERR   ,derr   )                                    \
	|_CCL_PER_FMKS (PPR,FB_PKT16,HCERR  ,hcerr  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT16,DCODE  ,dcode  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT16,PKT_NUM,pkt_num)                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT16,MAG_NUM,mag_num)                                    \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  F B    P K T 1 7 |
 * |___________________|
 *
 * FB_PKT17 - Packet 17 of field buffer Status Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (r)  DERR - The packet exists odd parity error.
 *             This bit is valid only for packet X/0).
 *                   DERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Odd-parity error
 *
 * (r)  HCERR -  the packet exists hamming error.
 *                  HCERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Hamming error
 *
 * (r)  DCODE -  Page designation code (Hamming 8/4 Decoded).
 *               This field is valid only for packet X/26~X31.
 *
 * (r)  PKT_NUM - Page packet number (Hamming 8/4 Decoded)
 *
 * (r)  MAG_NUM - Page magazine number (Hamming 8/4 Decoded)
\*-------------------------------------------------------------------------*/
#define _PPR_FB_PKT17_OFFSET                    (0x0150u)
#define _PPR_FB_PKT17_ADDRH(h)                  PPR_ADDRH(h,FB_PKT17)
#define _PPR_FB_PKT17_LEN                       (32)                 /* 32-bit*/
#define _PPR_FB_PKT17_TYPE                      uint32_t

#define _PPR_FB_PKT17_DERR_MASK                 (0x00002000u)
#define _PPR_FB_PKT17_DERR_SHIFT                (13)
#define  PPR_FB_PKT17_DERR_DEFAULT              (0x00u)
#define  PPR_FB_PKT17_DERR_OF(x)                _CCL_VALUEOF(_PPR_FB_PKT17_TYPE,x)
#define  PPR_FB_PKT17_DERR_OK                   (0x00u)
#define  PPR_FB_PKT17_DERR_ERROR                (0x01u)

#define _PPR_FB_PKT17_HCERR_MASK                (0x00001000u)
#define _PPR_FB_PKT17_HCERR_SHIFT               (12)
#define  PPR_FB_PKT17_HCERR_DEFAULT             (0x00u)
#define  PPR_FB_PKT17_HCERR_OF(x)               _CCL_VALUEOF(_PPR_FB_PKT17_TYPE,x)
#define  PPR_FB_PKT17_HCERR_OK                  (0x00u)
#define  PPR_FB_PKT17_HCERR_ERROR               (0x01u)

#define _PPR_FB_PKT17_DCODE_MASK                (0x00000F00u)
#define _PPR_FB_PKT17_DCODE_SHIFT               (8)
#define  PPR_FB_PKT17_DCODE_DEFAULT             (0x00u)
#define  PPR_FB_PKT17_DCODE_OF(x)               _DCODEL_VALUEOF(_PPR_FB_PKT17_TYPE,x)

#define _PPR_FB_PKT17_PKT_NUM_MASK              (0x000000F8u)
#define _PPR_FB_PKT17_PKT_NUM_SHIFT             (3)
#define  PPR_FB_PKT17_PKT_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT17_PKT_NUM_OF(x)             _PKT_NUML_VALUEOF(_PPR_FB_PKT17_TYPE,x)

#define _PPR_FB_PKT17_MAG_NUM_MASK              (0x00000007u)
#define _PPR_FB_PKT17_MAG_NUM_SHIFT             (0)
#define  PPR_FB_PKT17_MAG_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT17_MAG_NUM_OF(x)             _MAG_NUML_VALUEOF(_PPR_FB_PKT17_TYPE,x)

#define  PPR_FB_PKT17_OF(x)             _CCL_VALUEOF(_PPR_FB_PKT17_TYPE,x)

#define  PPR_FB_PKT17_DEFAULT (_PPR_FB_PKT17_TYPE)(   \
	 _CCL_PER_FDEFAULT(PPR,FB_PKT17,DERR   )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT17,HCERR  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT17,DCODE  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT17,PKT_NUM)         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT17,MAG_NUM)         \
	)

#define PPR_FB_PKT17_RMK(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT17_TYPE)( \
	 _CCL_PER_FMK  (PPR,FB_PKT17,DERR   ,derr   )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT17,HCERR  ,hcerr  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT17,DCODE  ,dcode  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT17,PKT_NUM,pkt_num)                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT17,MAG_NUM,mag_num)                                   \
	)

#define PPR_FB_PKT17_RMKS(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT17_TYPE)( \
	 _CCL_PER_FMKS (PPR,FB_PKT17,DERR   ,derr   )                                    \
	|_CCL_PER_FMKS (PPR,FB_PKT17,HCERR  ,hcerr  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT17,DCODE  ,dcode  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT17,PKT_NUM,pkt_num)                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT17,MAG_NUM,mag_num)                                    \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  F B    P K T 1 8 |
 * |___________________|
 *
 * FB_PKT18 - Packet 18 of field buffer Status Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (r)  DERR - The packet exists odd parity error.
 *             This bit is valid only for packet X/0).
 *                   DERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Odd-parity error
 *
 * (r)  HCERR -  the packet exists hamming error.
 *                  HCERR  |   description
 *              -----------+----------------------
 *                   OK    | No error
 *                  ERROR  | Hamming error
 *
 * (r)  DCODE -  Page designation code (Hamming 8/4 Decoded).
 *               This field is valid only for packet X/26~X31.
 *
 * (r)  PKT_NUM - Page packet number (Hamming 8/4 Decoded)
 *
 * (r)  MAG_NUM - Page magazine number (Hamming 8/4 Decoded)
\*-------------------------------------------------------------------------*/
#define _PPR_FB_PKT18_OFFSET                    (0x0154u)
#define _PPR_FB_PKT18_ADDRH(h)                  PPR_ADDRH(h,FB_PKT18)
#define _PPR_FB_PKT18_LEN                       (32)                 /* 32-bit*/
#define _PPR_FB_PKT18_TYPE                      uint32_t

#define _PPR_FB_PKT18_DERR_MASK                 (0x00002000u)
#define _PPR_FB_PKT18_DERR_SHIFT                (13)
#define  PPR_FB_PKT18_DERR_DEFAULT              (0x00u)
#define  PPR_FB_PKT18_DERR_OF(x)                _CCL_VALUEOF(_PPR_FB_PKT18_TYPE,x)
#define  PPR_FB_PKT18_DERR_OK                   (0x00u)
#define  PPR_FB_PKT18_DERR_ERROR                (0x01u)

#define _PPR_FB_PKT18_HCERR_MASK                (0x00001000u)
#define _PPR_FB_PKT18_HCERR_SHIFT               (12)
#define  PPR_FB_PKT18_HCERR_DEFAULT             (0x00u)
#define  PPR_FB_PKT18_HCERR_OF(x)               _CCL_VALUEOF(_PPR_FB_PKT18_TYPE,x)
#define  PPR_FB_PKT18_HCERR_OK                  (0x00u)
#define  PPR_FB_PKT18_HCERR_ERROR               (0x01u)

#define _PPR_FB_PKT18_DCODE_MASK                (0x00000F00u)
#define _PPR_FB_PKT18_DCODE_SHIFT               (8)
#define  PPR_FB_PKT18_DCODE_DEFAULT             (0x00u)
#define  PPR_FB_PKT18_DCODE_OF(x)               _DCODEL_VALUEOF(_PPR_FB_PKT18_TYPE,x)

#define _PPR_FB_PKT18_PKT_NUM_MASK              (0x000000F8u)
#define _PPR_FB_PKT18_PKT_NUM_SHIFT             (3)
#define  PPR_FB_PKT18_PKT_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT18_PKT_NUM_OF(x)             _PKT_NUML_VALUEOF(_PPR_FB_PKT18_TYPE,x)

#define _PPR_FB_PKT18_MAG_NUM_MASK              (0x00000007u)
#define _PPR_FB_PKT18_MAG_NUM_SHIFT             (0)
#define  PPR_FB_PKT18_MAG_NUM_DEFAULT           (0x00u)
#define  PPR_FB_PKT18_MAG_NUM_OF(x)             _MAG_NUML_VALUEOF(_PPR_FB_PKT18_TYPE,x)

#define  PPR_FB_PKT18_OF(x)             _CCL_VALUEOF(_PPR_FB_PKT18_TYPE,x)

#define  PPR_FB_PKT18_DEFAULT (_PPR_FB_PKT18_TYPE)(   \
	 _CCL_PER_FDEFAULT(PPR,FB_PKT18,DERR   )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT18,HCERR  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT18,DCODE  )         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT18,PKT_NUM)         \
	|_CCL_PER_FDEFAULT(PPR,FB_PKT18,MAG_NUM)         \
	)

#define PPR_FB_PKT18_RMK(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT18_TYPE)( \
	 _CCL_PER_FMK  (PPR,FB_PKT18,DERR   ,derr   )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT18,HCERR  ,hcerr  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT18,DCODE  ,dcode  )                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT18,PKT_NUM,pkt_num)                                   \
	|_CCL_PER_FMK  (PPR,FB_PKT18,MAG_NUM,mag_num)                                   \
	)

#define PPR_FB_PKT18_RMKS(derr, hcerr, dcode, pkt_num, mag_num) (_PPR_FB_PKT18_TYPE)( \
	 _CCL_PER_FMKS (PPR,FB_PKT18,DERR   ,derr   )                                    \
	|_CCL_PER_FMKS (PPR,FB_PKT18,HCERR  ,hcerr  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT18,DCODE  ,dcode  )                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT18,PKT_NUM,pkt_num)                                    \
	|_CCL_PER_FMK  (PPR,FB_PKT18,MAG_NUM,mag_num)                                    \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  L V L D   T O P  |
 * |___________________|
 *
 * LVLD_TOP - Live Line Detection
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  END_LINE   - This field specifies end line of live line detection.
 * (rw)  START_LINE - This field specifies start line of live line detection.
\*-------------------------------------------------------------------------*/
#define _PPR_LVLD_TOP_OFFSET                    (0x02E0u)
#define _PPR_LVLD_TOP_ADDRH(h)                   PPR_ADDRH(h,LVLD_TOP)
#define _PPR_LVLD_TOP_LEN                       (32)                 /* 32-bit*/
#define _PPR_LVLD_TOP_TYPE                       uint32_t

#define _PPR_LVLD_TOP_END_LINE_MASK             (0x07FF0000u)
#define _PPR_LVLD_TOP_END_LINE_SHIFT            (16)
#define  PPR_LVLD_TOP_END_LINE_DEFAULT          (0x016u)
#define  PPR_LVLD_TOP_END_LINE_OF(x)            _CCL_VALUEOF(_PPR_LVLD_TOP_TYPE,x)

#define _PPR_LVLD_TOP_START_LINE_MASK           (0x000007FFu)
#define _PPR_LVLD_TOP_START_LINE_SHIFT          (0)
#define  PPR_LVLD_TOP_START_LINE_DEFAULT        (0x005u)
#define  PPR_LVLD_TOP_START_LINE_OF(x)          _CCL_VALUEOF(_PPR_LVLD_TOP_TYPE,x)


#define  PPR_LVLD_TOP_OF(x)                     _CCL_VALUEOF(_PPR_LVLD_TOP_TYPE,x)

#define  PPR_LVLD_TOP_DEFAULT    (_PPR_LVLD_TOP_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,LVLD_TOP,END_LINE)           \
	|_CCL_PER_FDEFAULT(PPR,LVLD_TOP,START_LINE)         \
	)

#define PPR_LVLD_TOP_RMK(end_line, start_line) (_PPR_LVLD_TOP_TYPE)(  \
	 _CCL_PER_FMK  (PPR,LVLD_TOP,END_LINE,end_line  )                 \
	|_CCL_PER_FMK  (PPR,LVLD_TOP,START_LINE,start_line)               \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  L V L D   B T M  |
 * |___________________|
 *
 * LVLD_BTM - Live Line Detection
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  END_LINE   - This field specifies end line of live line detection.
 * (rw)  START_LINE - This field specifies start line of live line detection.
\*-------------------------------------------------------------------------*/
#define _PPR_LVLD_BTM_OFFSET                    (0x02E4u)
#define _PPR_LVLD_BTM_ADDRH(h)                   PPR_ADDRH(h,LVLD_BTM)
#define _PPR_LVLD_BTM_LEN                       (32)                 /* 32-bit*/
#define _PPR_LVLD_BTM_TYPE                       uint32_t

#define _PPR_LVLD_BTM_END_LINE_MASK             (0x07FF0000u)
#define _PPR_LVLD_BTM_END_LINE_SHIFT            (16)
#define  PPR_LVLD_BTM_END_LINE_DEFAULT          (0x016u)
#define  PPR_LVLD_BTM_END_LINE_OF(x)            _CCL_VALUEOF(_PPR_LVLD_BTM_TYPE,x)

#define _PPR_LVLD_BTM_START_LINE_MASK           (0x000007FFu)
#define _PPR_LVLD_BTM_START_LINE_SHIFT          (0)
#define  PPR_LVLD_BTM_START_LINE_DEFAULT        (0x005u)
#define  PPR_LVLD_BTM_START_LINE_OF(x)          _CCL_VALUEOF(_PPR_LVLD_BTM_TYPE,x)


#define  PPR_LVLD_BTM_OF(x)                     _CCL_VALUEOF(_PPR_LVLD_BTM_TYPE,x)

#define  PPR_LVLD_BTM_DEFAULT    (_PPR_LVLD_BTM_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,LVLD_BTM,END_LINE)           \
	|_CCL_PER_FDEFAULT(PPR,LVLD_BTM,START_LINE)         \
	)

#define PPR_LVLD_BTM_RMK(end_line, start_line) (_PPR_LVLD_BTM_TYPE)(  \
	 _CCL_PER_FMK  (PPR,LVLD_BTM,END_LINE,end_line  )                 \
	|_CCL_PER_FMK  (PPR,LVLD_BTM,START_LINE,start_line)               \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  P G   M O D E    |
 * |___________________|
 *
 * PG_MODE - Pattern Generator Mode
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  MODE -
 *                  MODE   |   description
 *              -----------+------------------------------------
 *                   TT    | Teletext
 *                   CC    | Closed Caption
 *                   VPS   | Video Program System
 *                   WSS   | Wide-Screen Signaling
 *                   CGMS  | Copy Generation Management System
 *
 * (rw)  DATA_SRC -
 *                DATA_SRC |   description
 *              -----------+---------------------------------------
 *                 PTNGEN  | VBI slice data from Pattern Generator
 *                 SLICER  | VBI slice data from slicer
 *
 * (rw)  PTNGEN -
 *                  GEN    |   description
 *              -----------+---------------------------------------
 *                 ENABLE  | Enable  VBI Pattern Generator    (write)
 *                 DISABLE | Disable VBI Pattern Generator    (write)
 *                 READY   | Ready for accept data            (read)
 *                 BUSY    | Patten Generator is sending data (read)
\*-------------------------------------------------------------------------*/
#define _PPR_PG_MODE_OFFSET                     (0x0300u)
#define _PPR_PG_MODE_ADDRH(h)                    PPR_ADDRH(h,PG_MODE)
#define _PPR_PG_MODE_LEN                        (32)                 /* 32-bit*/
#define _PPR_PG_MODE_TYPE                        uint32_t

#define _PPR_PG_MODE_MODE_MASK                  (0x00000070u)
#define _PPR_PG_MODE_MODE_SHIFT                 (4)
#define  PPR_PG_MODE_MODE_DEFAULT               (0x00u)
#define  PPR_PG_MODE_MODE_OF(x)                 _CCL_VALUEOF(_PPR_PG_MODE_TYPE,x)
#define  PPR_PG_MODE_MODE_TT                    (0x00u)
#define  PPR_PG_MODE_MODE_CC                    (0x01u)
#define  PPR_PG_MODE_MODE_VPS                   (0x02u)
#define  PPR_PG_MODE_MODE_WSS                   (0x03u)
#define  PPR_PG_MODE_MODE_CGMS                  (0x04u)

#define _PPR_PG_MODE_DATA_SRC_MASK              (0x00000002u)
#define _PPR_PG_MODE_DATA_SRC_SHIFT             (1)
#define  PPR_PG_MODE_DATA_SRC_DEFAULT           (0x00u)
#define  PPR_PG_MODE_DATA_SRC_OF(x)             _CCL_VALUEOF(_PPR_PG_MODE_TYPE,x)
#define  PPR_PG_MODE_DATA_SRC_SLICER            (0x00u)
#define  PPR_PG_MODE_DATA_SRC_PTNGEN            (0x01u)

#define _PPR_PG_MODE_PTNGEN_MASK                (0x00000001u)
#define _PPR_PG_MODE_PTNGEN_SHIFT               (0)
#define  PPR_PG_MODE_PTNGEN_DEFAULT             (0x00u)
#define  PPR_PG_MODE_PTNGEN_OF(x)               _CCL_VALUEOF(_PPR_PG_MODE_TYPE,x)
#define  PPR_PG_MODE_PTNGEN_DISABLE             (0x00u)
#define  PPR_PG_MODE_PTNGEN_ENABLE              (0x01u)
#define  PPR_PG_MODE_PTNGEN_READY               (0x00u)
#define  PPR_PG_MODE_PTNGEN_BUSY                (0x01u)


#define  PPR_PG_MODE_OF(x)             _CCL_VALUEOF(_PPR_PG_MODE_TYPE,x)

#define  PPR_PG_MODE_DEFAULT (_PPR_PG_MODE_TYPE)(   \
	 _CCL_PER_FDEFAULT(PPR,PG_MODE,MODE)            \
	|_CCL_PER_FDEFAULT(PPR,PG_MODE,DATA_SRC)        \
	|_CCL_PER_FDEFAULT(PPR,PG_MODE,PTNGEN)          \
	)

#define PPR_PG_MODE_RMK(mode, data_src, ptngen) (_PPR_PG_MODE_TYPE)(  \
	 _CCL_PER_FMK  (PPR,PG_MODE,MODE,mode)                            \
	|_CCL_PER_FMK  (PPR,PG_MODE,DATA_SRC,data_src)                    \
	|_CCL_PER_FMK  (PPR,PG_MODE,PTNGEN,ptngen)                        \
	)

#define PPR_PG_MODE_RMKS(mode, data_src, ptngen) (_PPR_PG_MODE_TYPE)( \
	 _CCL_PER_FMKS (PPR,PG_MODE,MODE,mode)                            \
	|_CCL_PER_FMKS (PPR,PG_MODE,DATA_SRC,data_src)                    \
	|_CCL_PER_FMKS (PPR,PG_MODE,PTNGEN,ptngen)                        \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  P G   L I N E    |
 * |___________________|
 *
 * PG_LINE - Pattern Generator Line
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  ACT - Active
 *                  ACT    |   description
 *              -----------+---------------------------------------
 *                 DISABLE |
 *                 ENABLE  |
\*-------------------------------------------------------------------------*/
#define _PPR_PG_LINE_OFFSET                     (0x0304u)
#define _PPR_PG_LINE_ADDRH(h)                    PPR_ADDRH(h,PG_LINE)
#define _PPR_PG_LINE_LEN                        (32)                 /* 32-bit*/
#define _PPR_PG_LINE_TYPE                        uint32_t

#define _PPR_PG_LINE_ACT_MASK                   (0x00000001u)
#define _PPR_PG_LINE_ACT_SHIFT                  (0)
#define  PPR_PG_LINE_ACT_DEFAULT                (0x00u)
#define  PPR_PG_LINE_ACT_OF(x)                  _CCL_VALUEOF(_PPR_PG_LINE_TYPE,x)
#define  PPR_PG_LINE_ACT_DISABLE                (0x00u)
#define  PPR_PG_LINE_ACT_ENABLE                 (0x01u)


#define  PPR_PG_LINE_OF(x)             _CCL_VALUEOF(_PPR_PG_LINE_TYPE,x)

#define  PPR_PG_LINE_DEFAULT (_PPR_PG_LINE_TYPE)(   \
	 _CCL_PER_FDEFAULT(PPR,PG_LINE,ACT)             \
	)

#define PPR_PG_LINE_RMK(act) (_PPR_PG_LINE_TYPE)(   \
	 _CCL_PER_FMK  (PPR,PG_LINE,ACT,act)            \
	)

#define PPR_PG_LINE_RMKS(act) (_PPR_PG_LINE_TYPE)(  \
	 _CCL_PER_FMKS (PPR,PG_LINE,ACT,act)            \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  PGDATA B1B4      |
 * |___________________|
 *
 * PGDATA_B1B4 - Byte 1~4 of Pattern Generator Data
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  BYTE1 - 1st byte of output data of VBI header.
 * (rw)  BYTE2 - 2nd byte of output data of VBI header.
 * (rw)  BYTE3 - 3rd byte of output data of VBI header.
 * (rw)  BYTE4 - 4th byte of output data of VBI header.
\*-------------------------------------------------------------------------*/
#define _PPR_PGDATA_B1B4_OFFSET                    (0x0308u)
#define _PPR_PGDATA_B1B4_ADDRH(h)                   PPR_ADDRH(h,PGDATA_B1B4)
#define _PPR_PGDATA_B1B4_LEN                       (32)                 /* 32-bit*/
#define _PPR_PGDATA_B1B4_TYPE                       uint32_t

#define _PPR_PGDATA_B1B4_BYTE1_MASK                (0xFF000000u)
#define _PPR_PGDATA_B1B4_BYTE1_SHIFT               (24)
#define  PPR_PGDATA_B1B4_BYTE1_DEFAULT             (0x00u)
#define  PPR_PGDATA_B1B4_BYTE1_OF(x)               _CCL_VALUEOF(_PPR_PGDATA_B1B4_TYPE,x)

#define _PPR_PGDATA_B1B4_BYTE2_MASK                (0x00FF0000u)
#define _PPR_PGDATA_B1B4_BYTE2_SHIFT               (16)
#define  PPR_PGDATA_B1B4_BYTE2_DEFAULT             (0x00u)
#define  PPR_PGDATA_B1B4_BYTE2_OF(x)               _CCL_VALUEOF(_PPR_PGDATA_B1B4_TYPE,x)

#define _PPR_PGDATA_B1B4_BYTE3_MASK                (0x0000FF00u)
#define _PPR_PGDATA_B1B4_BYTE3_SHIFT               (8)
#define  PPR_PGDATA_B1B4_BYTE3_DEFAULT             (0x00u)
#define  PPR_PGDATA_B1B4_BYTE3_OF(x)               _CCL_VALUEOF(_PPR_PGDATA_B1B4_TYPE,x)

#define _PPR_PGDATA_B1B4_BYTE4_MASK                (0x000000FFu)
#define _PPR_PGDATA_B1B4_BYTE4_SHIFT               (0)
#define  PPR_PGDATA_B1B4_BYTE4_DEFAULT             (0x00u)
#define  PPR_PGDATA_B1B4_BYTE4_OF(x)               _CCL_VALUEOF(_PPR_PGDATA_B1B4_TYPE,x)

#define  PPR_PGDATA_B1B4_OF(x)                     _CCL_VALUEOF(_PPR_PGDATA_B1B4_TYPE,x)

#define  PPR_PGDATA_B1B4_DEFAULT       (_PPR_PGDATA_B1B4_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,PGDATA_B1B4,BYTE1)                    \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B1B4,BYTE2)                    \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B1B4,BYTE3)                    \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B1B4,BYTE4)                    \
	)

#define PPR_PGDATA_B1B4_RMK(byte1, byte2, byte3, byte4) (_PPR_PGDATA_B1B4_TYPE)(  \
	 _CCL_PER_FMK  (PPR,PGDATA_B1B4,BYTE1,byte1)                                  \
	|_CCL_PER_FMK  (PPR,PGDATA_B1B4,BYTE2,byte2)                                  \
	|_CCL_PER_FMK  (PPR,PGDATA_B1B4,BYTE3,byte3)                                  \
	|_CCL_PER_FMK  (PPR,PGDATA_B1B4,BYTE4,byte4)                                  \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  PGDATA B5B8      |
 * |___________________|
 *
 * PGDATA_B5B8 - Byte 5~8 of Pattern Generator Data
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  BYTE5 - 5th byte of output data of VBI header.
 * (rw)  BYTE6 - 6th byte of output data of VBI header.
 * (rw)  BYTE7 - 7th byte of output data of VBI header.
 * (rw)  BYTE8 - 8th byte of output data of VBI header.
\*-------------------------------------------------------------------------*/
#define _PPR_PGDATA_B5B8_OFFSET                    (0x030Cu)
#define _PPR_PGDATA_B5B8_ADDRH(h)                   PPR_ADDRH(h,PGDATA_B5B8)
#define _PPR_PGDATA_B5B8_LEN                       (32)                 /* 32-bit*/
#define _PPR_PGDATA_B5B8_TYPE                       uint32_t

#define _PPR_PGDATA_B5B8_BYTE5_MASK                (0xFF000000u)
#define _PPR_PGDATA_B5B8_BYTE5_SHIFT               (24)
#define  PPR_PGDATA_B5B8_BYTE5_DEFAULT             (0x00u)
#define  PPR_PGDATA_B5B8_BYTE5_OF(x)               _CCL_VALUEOF(_PPR_PGDATA_B5B8_TYPE,x)

#define _PPR_PGDATA_B5B8_BYTE6_MASK                (0x00FF0000u)
#define _PPR_PGDATA_B5B8_BYTE6_SHIFT               (16)
#define  PPR_PGDATA_B5B8_BYTE6_DEFAULT             (0x00u)
#define  PPR_PGDATA_B5B8_BYTE6_OF(x)               _CCL_VALUEOF(_PPR_PGDATA_B5B8_TYPE,x)

#define _PPR_PGDATA_B5B8_BYTE7_MASK                (0x0000FF00u)
#define _PPR_PGDATA_B5B8_BYTE7_SHIFT               (8)
#define  PPR_PGDATA_B5B8_BYTE7_DEFAULT             (0x00u)
#define  PPR_PGDATA_B5B8_BYTE7_OF(x)               _CCL_VALUEOF(_PPR_PGDATA_B5B8_TYPE,x)

#define _PPR_PGDATA_B5B8_BYTE8_MASK                (0x000000FFu)
#define _PPR_PGDATA_B5B8_BYTE8_SHIFT               (0)
#define  PPR_PGDATA_B5B8_BYTE8_DEFAULT             (0x00u)
#define  PPR_PGDATA_B5B8_BYTE8_OF(x)               _CCL_VALUEOF(_PPR_PGDATA_B5B8_TYPE,x)

#define  PPR_PGDATA_B5B8_OF(x)                     _CCL_VALUEOF(_PPR_PGDATA_B5B8_TYPE,x)

#define  PPR_PGDATA_B5B8_DEFAULT       (_PPR_PGDATA_B5B8_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,PGDATA_B5B8,BYTE5)                    \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B5B8,BYTE6)                    \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B5B8,BYTE7)                    \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B5B8,BYTE8)                    \
	)

#define PPR_PGDATA_B5B8_RMK(byte5, byte6, byte7, byte8) (_PPR_PGDATA_B5B8_TYPE)(  \
	 _CCL_PER_FMK  (PPR,PGDATA_B5B8,BYTE5,byte5)                                  \
	|_CCL_PER_FMK  (PPR,PGDATA_B5B8,BYTE6,byte6)                                  \
	|_CCL_PER_FMK  (PPR,PGDATA_B5B8,BYTE7,byte7)                                  \
	|_CCL_PER_FMK  (PPR,PGDATA_B5B8,BYTE8,byte8)                                  \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  PGDATA B9B12     |
 * |___________________|
 *
 * PGDATA_B9B12 - Byte 9~12 of Pattern Generator Data
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  BYTE9  -  9th byte of output data of VBI header.
 * (rw)  BYTE10 - 10th byte of output data of VBI header.
 * (rw)  BYTE11 - 11th byte of output data of VBI header.
 * (rw)  BYTE12 - 12th byte of output data of VBI header.
\*-------------------------------------------------------------------------*/
#define _PPR_PGDATA_B9B12_OFFSET                    (0x0310u)
#define _PPR_PGDATA_B9B12_ADDRH(h)                   PPR_ADDRH(h,PGDATA_B9B12)
#define _PPR_PGDATA_B9B12_LEN                       (32)                 /* 32-bit*/
#define _PPR_PGDATA_B9B12_TYPE                       uint32_t

#define _PPR_PGDATA_B9B12_BYTE9_MASK                (0xFF000000u)
#define _PPR_PGDATA_B9B12_BYTE9_SHIFT               (24)
#define  PPR_PGDATA_B9B12_BYTE9_DEFAULT             (0x00u)
#define  PPR_PGDATA_B9B12_BYTE9_OF(x)               _CCL_VALUEOF(_PPR_PGDATA_B9B12_TYPE,x)

#define _PPR_PGDATA_B9B12_BYTE10_MASK               (0x00FF0000u)
#define _PPR_PGDATA_B9B12_BYTE10_SHIFT              (16)
#define  PPR_PGDATA_B9B12_BYTE10_DEFAULT            (0x00u)
#define  PPR_PGDATA_B9B12_BYTE10_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B9B12_TYPE,x)

#define _PPR_PGDATA_B9B12_BYTE11_MASK               (0x0000FF00u)
#define _PPR_PGDATA_B9B12_BYTE11_SHIFT              (8)
#define  PPR_PGDATA_B9B12_BYTE11_DEFAULT            (0x00u)
#define  PPR_PGDATA_B9B12_BYTE11_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B9B12_TYPE,x)

#define _PPR_PGDATA_B9B12_BYTE12_MASK               (0x000000FFu)
#define _PPR_PGDATA_B9B12_BYTE12_SHIFT              (0)
#define  PPR_PGDATA_B9B12_BYTE12_DEFAULT            (0x00u)
#define  PPR_PGDATA_B9B12_BYTE12_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B9B12_TYPE,x)

#define  PPR_PGDATA_B9B12_OF(x)                     _CCL_VALUEOF(_PPR_PGDATA_B9B12_TYPE,x)

#define  PPR_PGDATA_B9B12_DEFAULT       (_PPR_PGDATA_B9B12_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,PGDATA_B9B12,BYTE9)                     \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B9B12,BYTE10)                    \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B9B12,BYTE11)                    \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B9B12,BYTE12)                    \
	)

#define PPR_PGDATA_B9B12_RMK(byte9, byte10, byte11, byte12) (_PPR_PGDATA_B9B12_TYPE)(  \
	 _CCL_PER_FMK  (PPR,PGDATA_B9B12,BYTE9,byte9)                                      \
	|_CCL_PER_FMK  (PPR,PGDATA_B9B12,BYTE10,byte10)                                    \
	|_CCL_PER_FMK  (PPR,PGDATA_B9B12,BYTE11,byte11)                                    \
	|_CCL_PER_FMK  (PPR,PGDATA_B9B12,BYTE12,byte12)                                    \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  PGDATA B13B16    |
 * |___________________|
 *
 * PGDATA_B13B16 - Byte 13~16 of Pattern Generator Data
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  BYTE13 - 13th byte of output data of VBI header.
 * (rw)  BYTE14 - 14th byte of output data of VBI header.
 * (rw)  BYTE15 - 15th byte of output data of VBI header.
 * (rw)  BYTE16 - 16th byte of output data of VBI header.
\*-------------------------------------------------------------------------*/
#define _PPR_PGDATA_B13B16_OFFSET                    (0x0314u)
#define _PPR_PGDATA_B13B16_ADDRH(h)                   PPR_ADDRH(h,PGDATA_B13B16)
#define _PPR_PGDATA_B13B16_LEN                       (32)                 /* 32-bit*/
#define _PPR_PGDATA_B13B16_TYPE                       uint32_t

#define _PPR_PGDATA_B13B16_BYTE13_MASK               (0xFF000000u)
#define _PPR_PGDATA_B13B16_BYTE13_SHIFT              (24)
#define  PPR_PGDATA_B13B16_BYTE13_DEFAULT            (0x00u)
#define  PPR_PGDATA_B13B16_BYTE13_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B13B16_TYPE,x)

#define _PPR_PGDATA_B13B16_BYTE14_MASK               (0x00FF0000u)
#define _PPR_PGDATA_B13B16_BYTE14_SHIFT              (16)
#define  PPR_PGDATA_B13B16_BYTE14_DEFAULT            (0x00u)
#define  PPR_PGDATA_B13B16_BYTE14_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B13B16_TYPE,x)

#define _PPR_PGDATA_B13B16_BYTE15_MASK               (0x0000FF00u)
#define _PPR_PGDATA_B13B16_BYTE15_SHIFT              (8)
#define  PPR_PGDATA_B13B16_BYTE15_DEFAULT            (0x00u)
#define  PPR_PGDATA_B13B16_BYTE15_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B13B16_TYPE,x)

#define _PPR_PGDATA_B13B16_BYTE16_MASK               (0x000000FFu)
#define _PPR_PGDATA_B13B16_BYTE16_SHIFT              (0)
#define  PPR_PGDATA_B13B16_BYTE16_DEFAULT            (0x00u)
#define  PPR_PGDATA_B13B16_BYTE16_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B13B16_TYPE,x)

#define  PPR_PGDATA_B13B16_OF(x)                     _CCL_VALUEOF(_PPR_PGDATA_B13B16_TYPE,x)

#define  PPR_PGDATA_B13B16_DEFAULT       (_PPR_PGDATA_B13B16_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,PGDATA_B13B16,BYTE13)                     \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B13B16,BYTE14)                     \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B13B16,BYTE15)                     \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B13B16,BYTE16)                     \
	)

#define PPR_PGDATA_B13B16_RMK(byte13, byte14, byte15, byte16) (_PPR_PGDATA_B13B16_TYPE)(  \
	 _CCL_PER_FMK  (PPR,PGDATA_B13B16,BYTE13,byte13)                                      \
	|_CCL_PER_FMK  (PPR,PGDATA_B13B16,BYTE14,byte14)                                      \
	|_CCL_PER_FMK  (PPR,PGDATA_B13B16,BYTE15,byte15)                                      \
	|_CCL_PER_FMK  (PPR,PGDATA_B13B16,BYTE16,byte16)                                      \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  PGDATA B17B20    |
 * |___________________|
 *
 * PGDATA_B17B20 - Byte 17~20 of Pattern Generator Data
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  BYTE17 - 17th byte of output data of VBI header.
 * (rw)  BYTE18 - 18th byte of output data of VBI header.
 * (rw)  BYTE19 - 19th byte of output data of VBI header.
 * (rw)  BYTE20 - 20th byte of output data of VBI header.
\*-------------------------------------------------------------------------*/
#define _PPR_PGDATA_B17B20_OFFSET                    (0x0318u)
#define _PPR_PGDATA_B17B20_ADDRH(h)                   PPR_ADDRH(h,PGDATA_B17B20)
#define _PPR_PGDATA_B17B20_LEN                       (32)                 /* 32-bit*/
#define _PPR_PGDATA_B17B20_TYPE                       uint32_t

#define _PPR_PGDATA_B17B20_BYTE17_MASK               (0xFF000000u)
#define _PPR_PGDATA_B17B20_BYTE17_SHIFT              (24)
#define  PPR_PGDATA_B17B20_BYTE17_DEFAULT            (0x00u)
#define  PPR_PGDATA_B17B20_BYTE17_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B17B20_TYPE,x)

#define _PPR_PGDATA_B17B20_BYTE18_MASK               (0x00FF0000u)
#define _PPR_PGDATA_B17B20_BYTE18_SHIFT              (16)
#define  PPR_PGDATA_B17B20_BYTE18_DEFAULT            (0x00u)
#define  PPR_PGDATA_B17B20_BYTE18_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B17B20_TYPE,x)

#define _PPR_PGDATA_B17B20_BYTE19_MASK               (0x0000FF00u)
#define _PPR_PGDATA_B17B20_BYTE19_SHIFT              (8)
#define  PPR_PGDATA_B17B20_BYTE19_DEFAULT            (0x00u)
#define  PPR_PGDATA_B17B20_BYTE19_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B17B20_TYPE,x)

#define _PPR_PGDATA_B17B20_BYTE20_MASK               (0x000000FFu)
#define _PPR_PGDATA_B17B20_BYTE20_SHIFT              (0)
#define  PPR_PGDATA_B17B20_BYTE20_DEFAULT            (0x00u)
#define  PPR_PGDATA_B17B20_BYTE20_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B17B20_TYPE,x)

#define  PPR_PGDATA_B17B20_OF(x)                     _CCL_VALUEOF(_PPR_PGDATA_B17B20_TYPE,x)

#define  PPR_PGDATA_B17B20_DEFAULT       (_PPR_PGDATA_B17B20_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,PGDATA_B17B20,BYTE17)                     \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B17B20,BYTE18)                     \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B17B20,BYTE19)                     \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B17B20,BYTE20)                     \
	)

#define PPR_PGDATA_B17B20_RMK(byte17, byte18, byte19, byte20) (_PPR_PGDATA_B17B20_TYPE)(  \
	 _CCL_PER_FMK  (PPR,PGDATA_B17B20,BYTE17,byte17)                                      \
	|_CCL_PER_FMK  (PPR,PGDATA_B17B20,BYTE18,byte18)                                      \
	|_CCL_PER_FMK  (PPR,PGDATA_B17B20,BYTE19,byte19)                                      \
	|_CCL_PER_FMK  (PPR,PGDATA_B17B20,BYTE20,byte20)                                      \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  PGDATA B21B24    |
 * |___________________|
 *
 * PGDATA_B21B24 - Byte 21~24 of Pattern Generator Data
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  BYTE21 - 21th byte of output data of VBI header.
 * (rw)  BYTE22 - 22th byte of output data of VBI header.
 * (rw)  BYTE23 - 23th byte of output data of VBI header.
 * (rw)  BYTE24 - 24th byte of output data of VBI header.
\*-------------------------------------------------------------------------*/
#define _PPR_PGDATA_B21B24_OFFSET                    (0x031Cu)
#define _PPR_PGDATA_B21B24_ADDRH(h)                   PPR_ADDRH(h,PGDATA_B21B24)
#define _PPR_PGDATA_B21B24_LEN                       (32)                 /* 32-bit*/
#define _PPR_PGDATA_B21B24_TYPE                       uint32_t

#define _PPR_PGDATA_B21B24_BYTE21_MASK               (0xFF000000u)
#define _PPR_PGDATA_B21B24_BYTE21_SHIFT              (24)
#define  PPR_PGDATA_B21B24_BYTE21_DEFAULT            (0x00u)
#define  PPR_PGDATA_B21B24_BYTE21_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B21B24_TYPE,x)

#define _PPR_PGDATA_B21B24_BYTE22_MASK               (0x00FF0000u)
#define _PPR_PGDATA_B21B24_BYTE22_SHIFT              (16)
#define  PPR_PGDATA_B21B24_BYTE22_DEFAULT            (0x00u)
#define  PPR_PGDATA_B21B24_BYTE22_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B21B24_TYPE,x)

#define _PPR_PGDATA_B21B24_BYTE23_MASK               (0x0000FF00u)
#define _PPR_PGDATA_B21B24_BYTE23_SHIFT              (8)
#define  PPR_PGDATA_B21B24_BYTE23_DEFAULT            (0x00u)
#define  PPR_PGDATA_B21B24_BYTE23_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B21B24_TYPE,x)

#define _PPR_PGDATA_B21B24_BYTE24_MASK               (0x000000FFu)
#define _PPR_PGDATA_B21B24_BYTE24_SHIFT              (0)
#define  PPR_PGDATA_B21B24_BYTE24_DEFAULT            (0x00u)
#define  PPR_PGDATA_B21B24_BYTE24_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B21B24_TYPE,x)

#define  PPR_PGDATA_B21B24_OF(x)                     _CCL_VALUEOF(_PPR_PGDATA_B21B24_TYPE,x)

#define  PPR_PGDATA_B21B24_DEFAULT       (_PPR_PGDATA_B21B24_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,PGDATA_B21B24,BYTE21)                     \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B21B24,BYTE22)                     \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B21B24,BYTE23)                     \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B21B24,BYTE24)                     \
	)

#define PPR_PGDATA_B21B24_RMK(byte21, byte22, byte23, byte24) (_PPR_PGDATA_B21B24_TYPE)(  \
	 _CCL_PER_FMK  (PPR,PGDATA_B21B24,BYTE21,byte21)                                      \
	|_CCL_PER_FMK  (PPR,PGDATA_B21B24,BYTE22,byte22)                                      \
	|_CCL_PER_FMK  (PPR,PGDATA_B21B24,BYTE23,byte23)                                      \
	|_CCL_PER_FMK  (PPR,PGDATA_B21B24,BYTE24,byte24)                                      \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  PGDATA B25B28    |
 * |___________________|
 *
 * PGDATA_B25B28 - Byte 25~28 of Pattern Generator Data
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  BYTE25 - 25th byte of output data of VBI header.
 * (rw)  BYTE26 - 26th byte of output data of VBI header.
 * (rw)  BYTE27 - 27th byte of output data of VBI header.
 * (rw)  BYTE28 - 28th byte of output data of VBI header.
\*-------------------------------------------------------------------------*/
#define _PPR_PGDATA_B25B28_OFFSET                    (0x0320u)
#define _PPR_PGDATA_B25B28_ADDRH(h)                   PPR_ADDRH(h,PGDATA_B25B28)
#define _PPR_PGDATA_B25B28_LEN                       (32)                 /* 32-bit*/
#define _PPR_PGDATA_B25B28_TYPE                       uint32_t

#define _PPR_PGDATA_B25B28_BYTE25_MASK               (0xFF000000u)
#define _PPR_PGDATA_B25B28_BYTE25_SHIFT              (24)
#define  PPR_PGDATA_B25B28_BYTE25_DEFAULT            (0x00u)
#define  PPR_PGDATA_B25B28_BYTE25_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B25B28_TYPE,x)

#define _PPR_PGDATA_B25B28_BYTE26_MASK               (0x00FF0000u)
#define _PPR_PGDATA_B25B28_BYTE26_SHIFT              (16)
#define  PPR_PGDATA_B25B28_BYTE26_DEFAULT            (0x00u)
#define  PPR_PGDATA_B25B28_BYTE26_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B25B28_TYPE,x)

#define _PPR_PGDATA_B25B28_BYTE27_MASK               (0x0000FF00u)
#define _PPR_PGDATA_B25B28_BYTE27_SHIFT              (8)
#define  PPR_PGDATA_B25B28_BYTE27_DEFAULT            (0x00u)
#define  PPR_PGDATA_B25B28_BYTE27_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B25B28_TYPE,x)

#define _PPR_PGDATA_B25B28_BYTE28_MASK               (0x000000FFu)
#define _PPR_PGDATA_B25B28_BYTE28_SHIFT              (0)
#define  PPR_PGDATA_B25B28_BYTE28_DEFAULT            (0x00u)
#define  PPR_PGDATA_B25B28_BYTE28_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B25B28_TYPE,x)

#define  PPR_PGDATA_B25B28_OF(x)                     _CCL_VALUEOF(_PPR_PGDATA_B25B28_TYPE,x)

#define  PPR_PGDATA_B25B28_DEFAULT       (_PPR_PGDATA_B25B28_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,PGDATA_B25B28,BYTE25)                     \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B25B28,BYTE26)                     \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B25B28,BYTE27)                     \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B25B28,BYTE28)                     \
	)

#define PPR_PGDATA_B25B28_RMK(byte25, byte26, byte27, byte28) (_PPR_PGDATA_B25B28_TYPE)(  \
	 _CCL_PER_FMK  (PPR,PGDATA_B25B28,BYTE25,byte25)                                      \
	|_CCL_PER_FMK  (PPR,PGDATA_B25B28,BYTE26,byte26)                                      \
	|_CCL_PER_FMK  (PPR,PGDATA_B25B28,BYTE27,byte27)                                      \
	|_CCL_PER_FMK  (PPR,PGDATA_B25B28,BYTE28,byte28)                                      \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  PGDATA B29B32    |
 * |___________________|
 *
 * PGDATA_B29B32 - Byte 29~32 of Pattern Generator Data
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  BYTE29 - 29th byte of output data of VBI header.
 * (rw)  BYTE30 - 30th byte of output data of VBI header.
 * (rw)  BYTE31 - 31th byte of output data of VBI header.
 * (rw)  BYTE32 - 32th byte of output data of VBI header.
\*-------------------------------------------------------------------------*/
#define _PPR_PGDATA_B29B32_OFFSET                    (0x0324u)
#define _PPR_PGDATA_B29B32_ADDRH(h)                   PPR_ADDRH(h,PGDATA_B29B32)
#define _PPR_PGDATA_B29B32_LEN                       (32)                 /* 32-bit*/
#define _PPR_PGDATA_B29B32_TYPE                       uint32_t

#define _PPR_PGDATA_B29B32_BYTE29_MASK               (0xFF000000u)
#define _PPR_PGDATA_B29B32_BYTE29_SHIFT              (24)
#define  PPR_PGDATA_B29B32_BYTE29_DEFAULT            (0x00u)
#define  PPR_PGDATA_B29B32_BYTE29_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B29B32_TYPE,x)

#define _PPR_PGDATA_B29B32_BYTE30_MASK               (0x00FF0000u)
#define _PPR_PGDATA_B29B32_BYTE30_SHIFT              (16)
#define  PPR_PGDATA_B29B32_BYTE30_DEFAULT            (0x00u)
#define  PPR_PGDATA_B29B32_BYTE30_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B29B32_TYPE,x)

#define _PPR_PGDATA_B29B32_BYTE31_MASK               (0x0000FF00u)
#define _PPR_PGDATA_B29B32_BYTE31_SHIFT              (8)
#define  PPR_PGDATA_B29B32_BYTE31_DEFAULT            (0x00u)
#define  PPR_PGDATA_B29B32_BYTE31_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B29B32_TYPE,x)

#define _PPR_PGDATA_B29B32_BYTE32_MASK               (0x000000FFu)
#define _PPR_PGDATA_B29B32_BYTE32_SHIFT              (0)
#define  PPR_PGDATA_B29B32_BYTE32_DEFAULT            (0x00u)
#define  PPR_PGDATA_B29B32_BYTE32_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B29B32_TYPE,x)

#define  PPR_PGDATA_B29B32_OF(x)                     _CCL_VALUEOF(_PPR_PGDATA_B29B32_TYPE,x)

#define  PPR_PGDATA_B29B32_DEFAULT       (_PPR_PGDATA_B29B32_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,PGDATA_B29B32,BYTE29)                     \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B29B32,BYTE30)                     \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B29B32,BYTE31)                     \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B29B32,BYTE32)                     \
	)

#define PPR_PGDATA_B29B32_RMK(byte29, byte30, byte31, byte32) (_PPR_PGDATA_B29B32_TYPE)(  \
	 _CCL_PER_FMK  (PPR,PGDATA_B29B32,BYTE29,byte29)                                      \
	|_CCL_PER_FMK  (PPR,PGDATA_B29B32,BYTE30,byte30)                                      \
	|_CCL_PER_FMK  (PPR,PGDATA_B29B32,BYTE31,byte31)                                      \
	|_CCL_PER_FMK  (PPR,PGDATA_B29B32,BYTE32,byte32)                                      \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  PGDATA B33B36    |
 * |___________________|
 *
 * PGDATA_B33B36 - Byte 33~36 of Pattern Generator Data
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  BYTE33 - 33th byte of output data of VBI header.
 * (rw)  BYTE34 - 34th byte of output data of VBI header.
 * (rw)  BYTE35 - 35th byte of output data of VBI header.
 * (rw)  BYTE36 - 36th byte of output data of VBI header.
\*-------------------------------------------------------------------------*/
#define _PPR_PGDATA_B33B36_OFFSET                    (0x0328u)
#define _PPR_PGDATA_B33B36_ADDRH(h)                   PPR_ADDRH(h,PGDATA_B33B36)
#define _PPR_PGDATA_B33B36_LEN                       (32)                 /* 32-bit*/
#define _PPR_PGDATA_B33B36_TYPE                       uint32_t

#define _PPR_PGDATA_B33B36_BYTE33_MASK               (0xFF000000u)
#define _PPR_PGDATA_B33B36_BYTE33_SHIFT              (24)
#define  PPR_PGDATA_B33B36_BYTE33_DEFAULT            (0x00u)
#define  PPR_PGDATA_B33B36_BYTE33_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B33B36_TYPE,x)

#define _PPR_PGDATA_B33B36_BYTE34_MASK               (0x00FF0000u)
#define _PPR_PGDATA_B33B36_BYTE34_SHIFT              (16)
#define  PPR_PGDATA_B33B36_BYTE34_DEFAULT            (0x00u)
#define  PPR_PGDATA_B33B36_BYTE34_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B33B36_TYPE,x)

#define _PPR_PGDATA_B33B36_BYTE35_MASK               (0x0000FF00u)
#define _PPR_PGDATA_B33B36_BYTE35_SHIFT              (8)
#define  PPR_PGDATA_B33B36_BYTE35_DEFAULT            (0x00u)
#define  PPR_PGDATA_B33B36_BYTE35_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B33B36_TYPE,x)

#define _PPR_PGDATA_B33B36_BYTE36_MASK               (0x000000FFu)
#define _PPR_PGDATA_B33B36_BYTE36_SHIFT              (0)
#define  PPR_PGDATA_B33B36_BYTE36_DEFAULT            (0x00u)
#define  PPR_PGDATA_B33B36_BYTE36_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B33B36_TYPE,x)

#define  PPR_PGDATA_B33B36_OF(x)                     _CCL_VALUEOF(_PPR_PGDATA_B33B36_TYPE,x)

#define  PPR_PGDATA_B33B36_DEFAULT       (_PPR_PGDATA_B33B36_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,PGDATA_B33B36,BYTE33)                     \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B33B36,BYTE34)                     \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B33B36,BYTE35)                     \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B33B36,BYTE36)                     \
	)

#define PPR_PGDATA_B33B36_RMK(byte33, byte34, byte35, byte36) (_PPR_PGDATA_B33B36_TYPE)(  \
	 _CCL_PER_FMK  (PPR,PGDATA_B33B36,BYTE33,byte33)                                      \
	|_CCL_PER_FMK  (PPR,PGDATA_B33B36,BYTE34,byte34)                                      \
	|_CCL_PER_FMK  (PPR,PGDATA_B33B36,BYTE35,byte35)                                      \
	|_CCL_PER_FMK  (PPR,PGDATA_B33B36,BYTE36,byte36)                                      \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  PGDATA B37B40    |
 * |___________________|
 *
 * PGDATA_B37B40 - Byte 37~40 of Pattern Generator Data
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  BYTE37 - 37th byte of output data of VBI header.
 * (rw)  BYTE38 - 38th byte of output data of VBI header.
 * (rw)  BYTE39 - 39th byte of output data of VBI header.
 * (rw)  BYTE40 - 40th byte of output data of VBI header.
\*-------------------------------------------------------------------------*/
#define _PPR_PGDATA_B37B40_OFFSET                    (0x032Cu)
#define _PPR_PGDATA_B37B40_ADDRH(h)                   PPR_ADDRH(h,PGDATA_B37B40)
#define _PPR_PGDATA_B37B40_LEN                       (32)                 /* 32-bit*/
#define _PPR_PGDATA_B37B40_TYPE                       uint32_t

#define _PPR_PGDATA_B37B40_BYTE37_MASK               (0xFF000000u)
#define _PPR_PGDATA_B37B40_BYTE37_SHIFT              (24)
#define  PPR_PGDATA_B37B40_BYTE37_DEFAULT            (0x00u)
#define  PPR_PGDATA_B37B40_BYTE37_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B37B40_TYPE,x)

#define _PPR_PGDATA_B37B40_BYTE38_MASK               (0x00FF0000u)
#define _PPR_PGDATA_B37B40_BYTE38_SHIFT              (16)
#define  PPR_PGDATA_B37B40_BYTE38_DEFAULT            (0x00u)
#define  PPR_PGDATA_B37B40_BYTE38_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B37B40_TYPE,x)

#define _PPR_PGDATA_B37B40_BYTE39_MASK               (0x0000FF00u)
#define _PPR_PGDATA_B37B40_BYTE39_SHIFT              (8)
#define  PPR_PGDATA_B37B40_BYTE39_DEFAULT            (0x00u)
#define  PPR_PGDATA_B37B40_BYTE39_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B37B40_TYPE,x)

#define _PPR_PGDATA_B37B40_BYTE40_MASK               (0x000000FFu)
#define _PPR_PGDATA_B37B40_BYTE40_SHIFT              (0)
#define  PPR_PGDATA_B37B40_BYTE40_DEFAULT            (0x00u)
#define  PPR_PGDATA_B37B40_BYTE40_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B37B40_TYPE,x)

#define  PPR_PGDATA_B37B40_OF(x)                     _CCL_VALUEOF(_PPR_PGDATA_B37B40_TYPE,x)

#define  PPR_PGDATA_B37B40_DEFAULT       (_PPR_PGDATA_B37B40_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,PGDATA_B37B40,BYTE37)                     \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B37B40,BYTE38)                     \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B37B40,BYTE39)                     \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B37B40,BYTE40)                     \
	)

#define PPR_PGDATA_B37B40_RMK(byte37, byte38, byte39, byte40) (_PPR_PGDATA_B37B40_TYPE)(  \
	 _CCL_PER_FMK  (PPR,PGDATA_B37B40,BYTE37,byte37)                                      \
	|_CCL_PER_FMK  (PPR,PGDATA_B37B40,BYTE38,byte38)                                      \
	|_CCL_PER_FMK  (PPR,PGDATA_B37B40,BYTE39,byte39)                                      \
	|_CCL_PER_FMK  (PPR,PGDATA_B37B40,BYTE40,byte40)                                      \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  PGDATA B41B44    |
 * |___________________|
 *
 * PGDATA_B41B44 - Byte 41~44 of Pattern Generator Data
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  BYTE41 - 41th byte of output data of VBI header.
 * (rw)  BYTE42 - 42th byte of output data of VBI header.
 * (rw)  BYTE43 - 43th byte of output data of VBI header.
 * (rw)  BYTE44 - 44th byte of output data of VBI header.
\*-------------------------------------------------------------------------*/
#define _PPR_PGDATA_B41B44_OFFSET                    (0x0330u)
#define _PPR_PGDATA_B41B44_ADDRH(h)                   PPR_ADDRH(h,PGDATA_B41B44)
#define _PPR_PGDATA_B41B44_LEN                       (32)                 /* 32-bit*/
#define _PPR_PGDATA_B41B44_TYPE                       uint32_t

#define _PPR_PGDATA_B41B44_BYTE41_MASK               (0xFF000000u)
#define _PPR_PGDATA_B41B44_BYTE41_SHIFT              (24)
#define  PPR_PGDATA_B41B44_BYTE41_DEFAULT            (0x00u)
#define  PPR_PGDATA_B41B44_BYTE41_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B41B44_TYPE,x)

#define _PPR_PGDATA_B41B44_BYTE42_MASK               (0x00FF0000u)
#define _PPR_PGDATA_B41B44_BYTE42_SHIFT              (16)
#define  PPR_PGDATA_B41B44_BYTE42_DEFAULT            (0x00u)
#define  PPR_PGDATA_B41B44_BYTE42_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B41B44_TYPE,x)

#define _PPR_PGDATA_B41B44_BYTE43_MASK               (0x0000FF00u)
#define _PPR_PGDATA_B41B44_BYTE43_SHIFT              (8)
#define  PPR_PGDATA_B41B44_BYTE43_DEFAULT            (0x00u)
#define  PPR_PGDATA_B41B44_BYTE43_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B41B44_TYPE,x)

#define _PPR_PGDATA_B41B44_BYTE44_MASK               (0x000000FFu)
#define _PPR_PGDATA_B41B44_BYTE44_SHIFT              (0)
#define  PPR_PGDATA_B41B44_BYTE44_DEFAULT            (0x00u)
#define  PPR_PGDATA_B41B44_BYTE44_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B41B44_TYPE,x)

#define  PPR_PGDATA_B41B44_OF(x)                     _CCL_VALUEOF(_PPR_PGDATA_B41B44_TYPE,x)

#define  PPR_PGDATA_B41B44_DEFAULT       (_PPR_PGDATA_B41B44_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,PGDATA_B41B44,BYTE41)                     \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B41B44,BYTE42)                     \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B41B44,BYTE43)                     \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B41B44,BYTE44)                     \
	)

#define PPR_PGDATA_B41B44_RMK(byte41, byte42, byte43, byte44) (_PPR_PGDATA_B41B44_TYPE)(  \
	 _CCL_PER_FMK  (PPR,PGDATA_B41B44,BYTE41,byte41)                                      \
	|_CCL_PER_FMK  (PPR,PGDATA_B41B44,BYTE42,byte42)                                      \
	|_CCL_PER_FMK  (PPR,PGDATA_B41B44,BYTE43,byte43)                                      \
	|_CCL_PER_FMK  (PPR,PGDATA_B41B44,BYTE44,byte44)                                      \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  PGDATA B45B47    |
 * |___________________|
 *
 * PGDATA_B45B47 - Byte 45~47 of Pattern Generator Data
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  BYTE45 - 45th byte of output data of VBI header.
 * (rw)  BYTE46 - 46th byte of output data of VBI header.
 * (rw)  BYTE47 - 47th byte of output data of VBI header.
\*-------------------------------------------------------------------------*/
#define _PPR_PGDATA_B45B47_OFFSET                    (0x0334u)
#define _PPR_PGDATA_B45B47_ADDRH(h)                   PPR_ADDRH(h,PGDATA_B45B47)
#define _PPR_PGDATA_B45B47_LEN                       (32)                 /* 32-bit*/
#define _PPR_PGDATA_B45B47_TYPE                       uint32_t

#define _PPR_PGDATA_B45B47_BYTE45_MASK               (0xFF000000u)
#define _PPR_PGDATA_B45B47_BYTE45_SHIFT              (24)
#define  PPR_PGDATA_B45B47_BYTE45_DEFAULT            (0x00u)
#define  PPR_PGDATA_B45B47_BYTE45_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B45B47_TYPE,x)

#define _PPR_PGDATA_B45B47_BYTE46_MASK               (0x00FF0000u)
#define _PPR_PGDATA_B45B47_BYTE46_SHIFT              (16)
#define  PPR_PGDATA_B45B47_BYTE46_DEFAULT            (0x00u)
#define  PPR_PGDATA_B45B47_BYTE46_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B45B47_TYPE,x)

#define _PPR_PGDATA_B45B47_BYTE47_MASK               (0x0000FF00u)
#define _PPR_PGDATA_B45B47_BYTE47_SHIFT              (8)
#define  PPR_PGDATA_B45B47_BYTE47_DEFAULT            (0x00u)
#define  PPR_PGDATA_B45B47_BYTE47_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B45B47_TYPE,x)

#define _PPR_PGDATA_B45B47_BYTE44_MASK               (0x000000FFu)
#define _PPR_PGDATA_B45B47_BYTE44_SHIFT              (0)
#define  PPR_PGDATA_B45B47_BYTE44_DEFAULT            (0x00u)
#define  PPR_PGDATA_B45B47_BYTE44_OF(x)              _CCL_VALUEOF(_PPR_PGDATA_B45B47_TYPE,x)

#define  PPR_PGDATA_B45B47_OF(x)                     _CCL_VALUEOF(_PPR_PGDATA_B45B47_TYPE,x)

#define  PPR_PGDATA_B45B47_DEFAULT       (_PPR_PGDATA_B45B47_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,PGDATA_B45B47,BYTE45)                     \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B45B47,BYTE46)                     \
    |_CCL_PER_FDEFAULT(PPR,PGDATA_B45B47,BYTE47)                     \
	)

#define PPR_PGDATA_B45B47_RMK(byte45, byte46, byte47) (_PPR_PGDATA_B45B47_TYPE)(  \
	 _CCL_PER_FMK  (PPR,PGDATA_B45B47,BYTE45,byte45)                              \
	|_CCL_PER_FMK  (PPR,PGDATA_B45B47,BYTE46,byte46)                              \
	|_CCL_PER_FMK  (PPR,PGDATA_B45B47,BYTE47,byte47)                              \
	)


/* PGDATA_BxBy ((x,y)=( 1, 4),( 5, 8),( 9,12),(13,16),
                      (17,20),(21,24),(25,28),(29,32),
                      (33,36),(37,40),(41,44),(45,47) MACROS */

#define _PPR_PGDATA_FGET(x,y,FIELD)\
    _CCL_PER_FGET(_PPR_PGDATA_B##x##B##y##_ADDR,PPR,PGDATA_B##x##B##y##,FIELD)

#define _PPR_PGDATA_FSET(x,y,FIELD,field)\
	_CCL_PER_FSET(_PPR_PGDATA_B##x##B##y##_ADDR,PPR,PGDATA_B##x##B##y##,FIELD,field)

#define _PPR_PGDATA_B1B4_FGET(FIELD)      _PPR_PGDATA_FGET( 1, 4,FIELD)
#define _PPR_PGDATA_B5B8_FGET(FIELD)      _PPR_PGDATA_FGET( 5, 8,FIELD)
#define _PPR_PGDATA_B9B12_FGET(FIELD)     _PPR_PGDATA_FGET( 9,12,FIELD)
#define _PPR_PGDATA_B13B16_FGET(FIELD)    _PPR_PGDATA_FGET(13,16,FIELD)
#define _PPR_PGDATA_B17B20_FGET(FIELD)    _PPR_PGDATA_FGET(17,20,FIELD)
#define _PPR_PGDATA_B21B24_FGET(FIELD)    _PPR_PGDATA_FGET(21,24,FIELD)
#define _PPR_PGDATA_B25B28_FGET(FIELD)    _PPR_PGDATA_FGET(25,28,FIELD)
#define _PPR_PGDATA_B29B32_FGET(FIELD)    _PPR_PGDATA_FGET(29,32,FIELD)
#define _PPR_PGDATA_B33B36_FGET(FIELD)    _PPR_PGDATA_FGET(33,36,FIELD)
#define _PPR_PGDATA_B37B40_FGET(FIELD)    _PPR_PGDATA_FGET(37,40,FIELD)
#define _PPR_PGDATA_B41B44_FGET(FIELD)    _PPR_PGDATA_FGET(41,44,FIELD)
#define _PPR_PGDATA_B45B47_FGET(FIELD)    _PPR_PGDATA_FGET(45,47,FIELD)

#define _PPR_PGDATA_B1B4_FSET(FIELD)      _PPR_PGDATA_FSET( 1, 4,FIELD)
#define _PPR_PGDATA_B5B8_FSET(FIELD)      _PPR_PGDATA_FSET( 5, 8,FIELD)
#define _PPR_PGDATA_B9B12_FSET(FIELD)     _PPR_PGDATA_FSET( 9,12,FIELD)
#define _PPR_PGDATA_B13B16_FSET(FIELD)    _PPR_PGDATA_FSET(13,16,FIELD)
#define _PPR_PGDATA_B17B20_FSET(FIELD)    _PPR_PGDATA_FSET(17,20,FIELD)
#define _PPR_PGDATA_B21B24_FSET(FIELD)    _PPR_PGDATA_FSET(21,24,FIELD)
#define _PPR_PGDATA_B25B28_FSET(FIELD)    _PPR_PGDATA_FSET(25,28,FIELD)
#define _PPR_PGDATA_B29B32_FSET(FIELD)    _PPR_PGDATA_FSET(29,32,FIELD)
#define _PPR_PGDATA_B33B36_FSET(FIELD)    _PPR_PGDATA_FSET(33,36,FIELD)
#define _PPR_PGDATA_B37B40_FSET(FIELD)    _PPR_PGDATA_FSET(37,40,FIELD)
#define _PPR_PGDATA_B41B44_FSET(FIELD)    _PPR_PGDATA_FSET(41,44,FIELD)
#define _PPR_PGDATA_B45B47_FSET(FIELD)    _PPR_PGDATA_FSET(45,47,FIELD)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  D E B U G        |
 * |___________________|
 *
 * DEBUG - Debug Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  CLKCNT - Clock counter for PPR process. When PPR process can’t
 *                   be done in VBIPPR_DEBUG_CLKCNT clocks, the interrupt
 *                   will trigger reset mechanism.
\*-------------------------------------------------------------------------*/
#define _PPR_DEBUG_OFFSET                    (0x0338u)
#define _PPR_DEBUG_ADDRH(h)                   PPR_ADDRH(h,DEBUG)
#define _PPR_DEBUG_LEN                       (32)                 /* 32-bit*/
#define _PPR_DEBUG_TYPE                       uint32_t

#define _PPR_DEBUG_CLKCNT_MASK               (0x01FFFFFFu)
#define _PPR_DEBUG_CLKCNT_SHIFT              (0)
#define  PPR_DEBUG_CLKCNT_DEFAULT            (0x1FFFFFFu)
#define  PPR_DEBUG_CLKCNT_OF(x)              _CCL_VALUEOF(_PPR_DEBUG_TYPE,x)

#define  PPR_DEBUG_OF(x)                     _CCL_VALUEOF(_PPR_DEBUG_TYPE,x)

#define  PPR_DEBUG_DEFAULT       (_PPR_DEBUG_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,DEBUG,CLKCNT)             \
	)

#define PPR_DEBUG_RMK(clkcnt) (_PPR_DEBUG_TYPE)(     \
	 _CCL_PER_FMK  (PPR,DEBUG,CLKCNT,clkcnt)         \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  D B O U N C E 2  |
 * |___________________|
 *
 * DBOUNCE1 - Debounce Control Register 2
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  LIM_VSYNC_CNT - Force to display time code when VBIPPR_DEBOUNCE_EN
 *                         is enabled and the clock counter equals to
 *                         VBIPPR_DEBOUNCE_LIMIT_VSYNC_CNT.
 *                       Reset the clock counter to 0 when time code is displayed.
\*-------------------------------------------------------------------------*/
#define _PPR_DBOUNCE2_OFFSET                    (0x0340u)
#define _PPR_DBOUNCE2_ADDRH(h)                   PPR_ADDRH(h,DEBUG)
#define _PPR_DBOUNCE2_LEN                       (32)                 /* 32-bit*/
#define _PPR_DBOUNCE2_TYPE                       uint32_t

#define _PPR_DBOUNCE2_LIM_VSYNC_CNT_MASK        (0x000000FFu)
#define _PPR_DBOUNCE2_LIM_VSYNC_CNT_SHIFT       (0)
#define  PPR_DBOUNCE2_LIM_VSYNC_CNT_DEFAULT     (0xFFu)
#define  PPR_DBOUNCE2_LIM_VSYNC_CNT_OF(x)       _CCL_VALUEOF(_PPR_DBOUNCE2_TYPE,x)

#define  PPR_DBOUNCE2_OF(x)                     _CCL_VALUEOF(_PPR_DBOUNCE2_TYPE,x)

#define  PPR_DBOUNCE2_DEFAULT       (_PPR_DBOUNCE2_TYPE)(  \
	 _CCL_PER_FDEFAULT(PPR,DBOUNCE2,LIM_VSYNC_CNT)             \
	)

#define PPR_DBOUNCE2_RMK(lim_vsync_cnt) (_PPR_DBOUNCE2_TYPE)(     \
	 _CCL_PER_FMK  (PPR,DBOUNCE2,LIM_VSYNC_CNT,clkcnt)         \
	)



/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  F B   W O R D X  |
 * |___________________|
 *
 * FB_WORDX - Field buffer contents registers
\*-------------------------------------------------------------------------*/
#define _PPR_FB_WORDX_OFFSET                 (0x0400u)



#endif	/* !__C_CHIP_LIB_REALTEK_SPARROW_VERTICAL_BLANKING_INTERVAL_PRE_PROCESSOR_HARDWARE_ABSTRACT_LAYER_HEADER_INCLUDED__ */


/*-------------------------------------------------------------------------*\
 * End of ppr_hal.h
\*-------------------------------------------------------------------------*/

