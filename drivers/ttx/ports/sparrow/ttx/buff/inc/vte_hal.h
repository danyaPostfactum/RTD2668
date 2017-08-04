/*-------------------------------------------------------------------------*\
 *               File: vte_hal.h
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
 *   VTE_ADDR   (<REG>)                  -   Register address
 *   VTE_RGET   (<REG>)                  -   Returns the value in the peripheral register
 *   VTE_RSET   (<REG>,x)                -   Register set
 *   VTE_FGET   (<REG>,<FIELD>)          -   Returns the value of the specified field
 *                                             in the peripheral register
 *   VTE_FSET   (<REG>,<FIELD>,fieldval) -   Writes fieldval to the specified field
 *                                             in the peripheral register
 *   VTE_FSETS  (<REG>,<FIELD>,<SYM>)    -   Writes the symbol value to the specified
 *                                             field in the peripheral
 *   VTE_RGETA  (addr,<REG>)             -   Gets register for a given address
 *   VTE_RSETA  (addr,<REG>,x)           -   Sets register for a given address
 *
 *  <NOTE>
 *  Not completed yet!!!!!!!!!!
\*-------------------------------------------------------------------------*/

#ifndef	__C_CHIP_LIB_REALTEK_SPARROW_VERTICAL_BLANKING_INTERVAL_TRANSFER_ENGINE_HARDWARE_ABSTRACT_LAYER_HEADER_INCLUDED__
#define	__C_CHIP_LIB_REALTEK_SPARROW_VERTICAL_BLANKING_INTERVAL_TRANSFER_ENGINE_HARDWARE_ABSTRACT_LAYER_HEADER_INCLUDED__ (1)


/* CCL headers ------------------------------------------------------------*/
#include <ccl/std.h>    /* the standard header of C Chip Library */

/*
 *  Base address of VTE
 */
#define _VTE_ADDR_BASE  (0x4700u)

/*
 *  The function used for read register from this IP. Prototype is
 *     uint32_t _VTE_REG_READ( addr );
 */
#define _VTE_REG_READ(addr)         TTX_Reg32Read(addr)

/*
 *  The function used for read register from this IP. Prototype is
 *     void    _VTE_REG_WRITE( addr, val );
 */
#define _VTE_REG_WRITE(addr,val)    TTX_Reg32Write(addr,val)


/*
 * The shift left of data-bus connection in bits, e.g.,
 *   conventionally, the D0-D7 of PPR are connected to CPU data-bus D0-D7 (_VTE_DB_SHFT = 0).
 *   However, in some cases,
 *     the D0-D7 of 82C52 are connected to CPU data-bus D16-D24 (_VTE_DB_SHFT = 16).
 */
#define _VTE_DB_SHFT     (0)

/*
 * The width of data-bus connection in bits, e.g.,
 *   conventionally, the D0-D7 of 82C52 are connected to CPU data-bus D0-D7 (_VTE_DB_WIDTH = 0x00FFu).
 *   Please note, despite the shift of data-bus, _VTE_DB_WITH starts from LSB.
 *   As a consequence,
 *     if the D0-D7 of 82C52 are connected to CPU data-bus D16-D24 (_VTE_DB_WIDTH = 0x00FFu).
 */
#define _VTE_DB_WIDTH   (0xFFFFFFFFu)

/*
 * The shift left of address-bus connection in bits, e.g.,
 *   conventionally, the A0-A7 of 82C52 are connected to CPU address-bus A0-A7 (_VTE_AB_SHFT = Uint8).
 *   However, in some cases,
 *     the A0-A7 of 82C52 are connected to CPU data-bus A2-A9 (_VTE_DB_SHFT = uint32_t).
 */
#define _VTE_AB_SHFT     uint16_t



/* Field related macros */

#define VTE_FMK(REG,FIELD,x)\
	_CCL_PER_FMK(VTE,REG,FIELD,x)

#define VTE_FMKS(REG,FIELD,SYM)\
	_CCL_PER_FMKS(VTE,REG,FIELD,SYM)

#define VTE_FEXTRACT(REG,FIELD,x)\
	_CCL_PER_FEXTRACT(VTE,REG,FIELD,x)


/* Raw register/field access macros */

#define VTE_ADDR(REG)\
    (_VTE_ADDR_BASE+_VTE_##REG##_OFFSET)

#define VTE_RGET(REG)\
    _VTE_RGETA(VTE_ADDR(REG),REG)

#define VTE_RSET(REG,x)\
    _VTE_RSETA(VTE_ADDR(REG),REG,x)

#define VTE_FGET(REG,FIELD)\
    _VTE_FGETA(VTE_ADDR(REG),REG,FIELD)

#define VTE_FSET(REG,FIELD,x)\
    _VTE_FSETA(VTE_ADDR(REG),REG,FIELD,x)

#define VTE_FSETS(REG,FIELD,SYM)\
    _VTE_FSETSA(VTE_ADDR(REG),REG,FIELD,SYM)


/* Address based register/field access macros */

#define _VTE_RGETA(addr,REG)\
	_CCL_PER_RGET(addr,VTE,REG,_VTE_DB_SHFT,_VTE_DB_WIDTH)

#define _VTE_RSETA(addr,REG,x)\
	_CCL_PER_RSET(addr,VTE,REG,x,_VTE_DB_SHFT,_VTE_DB_WIDTH)

#define _VTE_RSETSA(addr,REG,SYM)\
	_CCL_PER_RSETS(addr,VTE,REG,SYM,_VTE_DB_SHFT,_VTE_DB_WIDTH)

#define _VTE_FGETA(addr,REG,FIELD)\
	_CCL_PER_FGET(addr,VTE,REG,FIELD,_VTE_DB_SHFT,_VTE_DB_WIDTH)

#define _VTE_FSETA(addr,REG,FIELD,x)\
	_CCL_PER_FSET(addr,VTE,REG,FIELD,x,_VTE_DB_SHFT,_VTE_DB_WIDTH)

#define _VTE_FSETSA(addr,REG,FIELD,SYM)\
	_CCL_PER_FSETS(addr,VTE,REG,FIELD,SYM,_VTE_DB_SHFT,_VTE_DB_WIDTH)


/* Handle based register/field access macros */

/* This is for base-address determined by handle h.
#define VTE_ADDRH(h,REG)\
	(_VTE_AB_SHFT)(&((h)->addrBase[_VTE_##REG##_OFFSET]))
*/
#define VTE_ADDRH(h,REG)\
	(_VTE_AB_SHFT)(_VTE_ADDR_BASE+_VTE_##REG##_OFFSET)

#define VTE_RGETH(h,REG)\
	_VTE_RGETA(VTE_ADDRH(h,REG),REG)

#define VTE_RSETH(h,REG,x)\
	_VTE_RSETA(VTE_ADDRH(h,REG),REG,x)

#define VTE_RSETSH(h,REG,SYM)\
	_VTE_RSETSA(VTE_ADDRH(h,REG),REG,SYM)

#define VTE_FGETH(h,REG,FIELD)\
	_VTE_FGETA(VTE_ADDRH(h,REG),REG,FIELD)

#define VTE_FSETH(h,REG,FIELD,x)\
	_VTE_FSETA(VTE_ADDRH(h,REG),REG,FIELD,x)

#define VTE_FSETSH(h,REG,FIELD,SYM)\
	_VTE_FSETSA(VTE_ADDRH(h,REG),REG,FIELD,SYM)


/* Function Prototypes ----------------------------------------------------*/

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  I N T S T        |
 * |___________________|
 *
 * INT_ST  - Interrupt Status Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  CD_ST -
 *                 CD_ST   |   description
 *              -----------+------------------------
 *                RUNNING  | channel decode is in-progress (read)
 *                 DONE    | channel decode is done        (read)
 *                CLEARED  | this status has been reset    (read)
 *                 NOP     | Remain the status             (write)
 *                 RESET   | Clear DONE flag               (write)
\*-------------------------------------------------------------------------*/
#define _VTE_INT_ST_OFFSET                    (0x0000u)
#define _VTE_INT_ST_ADDRH(h)                  VTE_ADDRH(h,INT_ST)
#define _VTE_INT_ST_LEN                       (32)                 /* 32-bit*/
#define _VTE_INT_ST_TYPE                      uint32_t

#define _VTE_INT_ST_CD_ST_MASK                (0x00010000u)
#define _VTE_INT_ST_CD_ST_SHIFT               (16)
#define  VTE_INT_ST_CD_ST_DEFAULT             (0x00u)
#define  VTE_INT_ST_CD_ST_OF(x)               _CCL_VALUEOF(_VTE_INT_ST_TYPE,x)
#define  VTE_INT_ST_CD_ST_RUNNING             (0x00u)
#define  VTE_INT_ST_CD_ST_DONE                (0x01u)
#define  VTE_INT_ST_CD_ST_CLEARED             (0x00u)
#define  VTE_INT_ST_CD_ST_NOP                 (0x00u)
#define  VTE_INT_ST_CD_ST_RESET               (0x01u)


#define  VTE_INT_ST_OF(x)             _CCL_VALUEOF(_VTE_INT_ST_TYPE,x)

#define  VTE_INT_ST_DEFAULT (_VTE_INT_ST_TYPE)(    \
	 _CCL_PER_FDEFAULT(VTE,INT_ST,CD_ST)           \
	)

#define VTE_INT_ST_RMK(cd_st) (_VTE_INT_ST_TYPE)(  \
	 _CCL_PER_FMK  (VTE,INT_ST,CD_ST,cd_st)        \
	)

#define VTE_INT_ST_RMKS(cd_st) (_VTE_INT_ST_TYPE)( \
	 _CCL_PER_FMKS (VTE,INT_ST,CD_ST,cd_st)        \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  I N T E N        |
 * |___________________|
 *
 * INT_EN  - Interrupt Enable Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  MIE (Master Interrupt Enable) -
 *                  MIE    |   description
 *              -----------+------------------------
 *                  NOP    | no operation
 *                ASSERT   | Enable interrupt
 *
 * (rw)  CD_INT_EN (Channel Decoder Interrupt Enable) -
 *               CD_INT_EN  |   description
 *              ------------+------------------------
 *                  NOP     | no operation
 *                ASSERT    | Enable interrupt
\*-------------------------------------------------------------------------*/
#define _VTE_INT_EN_OFFSET                    (0x0004u)
#define _VTE_INT_EN_ADDRH(h)                  VTE_ADDRH(h,INT_EN)
#define _VTE_INT_EN_LEN                       (32)                 /* 32-bit*/
#define _VTE_INT_EN_TYPE                      uint32_t

#define _VTE_INT_EN_MIE_MASK                  (0x80000000u)
#define _VTE_INT_EN_MIE_SHIFT                 (31)
#define  VTE_INT_EN_MIE_DEFAULT               (0x00u)
#define  VTE_INT_EN_MIE_OF(x)                 _CCL_VALUEOF(_VTE_INT_EN_TYPE,x)
#define  VTE_INT_EN_MIE_NOP                   (0x00u)
#define  VTE_INT_EN_MIE_ASSERT                (0x01u)

#define _VTE_INT_EN_CD_INT_EN_MASK            (0x00010000u)
#define _VTE_INT_EN_CD_INT_EN_SHIFT           (16)
#define  VTE_INT_EN_CD_INT_EN_DEFAULT         (0x00u)
#define  VTE_INT_EN_CD_INT_EN_OF(x)           _CCL_VALUEOF(_VTE_INT_EN_TYPE,x)
#define  VTE_INT_EN_CD_INT_EN_NOP             (0x00u)
#define  VTE_INT_EN_CD_INT_EN_ASSERT          (0x01u)

#define  VTE_INT_EN_OF(x)             _CCL_VALUEOF(_VTE_INT_EN_TYPE,x)

#define  VTE_INT_EN_DEFAULT (_VTE_INT_EN_TYPE)(    \
	 _CCL_PER_FDEFAULT(VTE,INT_EN,MIE)             \
	|_CCL_PER_FDEFAULT(VTE,INT_EN,CD_INT_EN)       \
	)

#define VTE_INT_EN_RMK(mie, cd_int_en) (_VTE_INT_EN_TYPE)(  \
	 _CCL_PER_FMK  (VTE,INT_EN,MIE,mie)                     \
	|_CCL_PER_FMK  (VTE,INT_EN,CD_INT_EN,cd_int_en)         \
	)

#define VTE_INT_EN_RMKS(mie, cd_int_en) (_VTE_INT_EN_TYPE)( \
	 _CCL_PER_FMKS (VTE,INT_EN,MIE,mie)                     \
	|_CCL_PER_FMKS (VTE,INT_EN,CD_INT_EN,cd_int_en)         \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  I N T D I S      |
 * |___________________|
 *
 * INT_DIS  - Interrupt Disable Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  MID (Master Interrupt Disable) -
 *                  MID    |   description
 *              -----------+------------------------
 *                  NOP    | no operation
 *                ASSERT   | Disable interrupt
 *
 * (rw)  CD_INT_DIS (Channel Decoder Interrupt Disable) -
 *               CD_INT_DIS |   description
 *              ------------+------------------------
 *                  NOP     | no operation
 *                ASSERT    | Disable interrupt
\*-------------------------------------------------------------------------*/
#define _VTE_INT_DIS_OFFSET                    (0x0008u)
#define _VTE_INT_DIS_ADDRH(h)                  VTE_ADDRH(h,INT_DIS)
#define _VTE_INT_DIS_LEN                       (32)                 /* 32-bit*/
#define _VTE_INT_DIS_TYPE                      uint32_t

#define _VTE_INT_DIS_MID_MASK                  (0x80000000u)
#define _VTE_INT_DIS_MID_SHIFT                 (31)
#define  VTE_INT_DIS_MID_DEFAULT               (0x00u)
#define  VTE_INT_DIS_MID_OF(x)                 _CCL_VALUEOF(_VTE_INT_DIS_TYPE,x)
#define  VTE_INT_DIS_MID_NOP                   (0x00u)
#define  VTE_INT_DIS_MID_ASSERT                (0x01u)

#define _VTE_INT_DIS_CD_INT_DIS_MASK           (0x00010000u)
#define _VTE_INT_DIS_CD_INT_DIS_SHIFT          (16)
#define  VTE_INT_DIS_CD_INT_DIS_DEFAULT        (0x00u)
#define  VTE_INT_DIS_CD_INT_DIS_OF(x)           _CCL_VALUEOF(_VTE_INT_DIS_TYPE,x)
#define  VTE_INT_DIS_CD_INT_DIS_NOP            (0x00u)
#define  VTE_INT_DIS_CD_INT_DIS_ASSERT         (0x01u)

#define  VTE_INT_DIS_OF(x)             _CCL_VALUEOF(_VTE_INT_DIS_TYPE,x)

#define  VTE_INT_DIS_DEFAULT (_VTE_INT_DIS_TYPE)(    \
	 _CCL_PER_FDEFAULT(VTE,INT_DIS,MID)              \
	|_CCL_PER_FDEFAULT(VTE,INT_DIS,CD_INT_DIS)       \
	)

#define VTE_INT_DIS_RMK(mid, cd_int_dis) (_VTE_INT_DIS_TYPE)(  \
	 _CCL_PER_FMK  (VTE,INT_DIS,MID,mid)                       \
	|_CCL_PER_FMK  (VTE,INT_DIS,CD_INT_DIS,cd_int_dis)         \
	)

#define VTE_INT_DIS_RMKS(mid, cd_int_dis) (_VTE_INT_DIS_TYPE)( \
	 _CCL_PER_FMKS (VTE,INT_DIS,MID,mid)                       \
	|_CCL_PER_FMKS (VTE,INT_DIS,CD_INT_DIS,cd_int_dis)         \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  C D S A D D R    |
 * |___________________|
 *
 * CD_SADDR - Source Address of encoded data Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  CD_SADDR - Source Address of encoded data
\*-------------------------------------------------------------------------*/
#define _VTE_CD_SADDR_OFFSET                    (0x0020u)
#define _VTE_CD_SADDR_ADDRH(h)                  VTE_ADDRH(h,CD_SADDR)
#define _VTE_CD_SADDR_LEN                       (32)                 /* 32-bit*/
#define _VTE_CD_SADDR_TYPE                      uint32_t

#define _VTE_CD_SADDR_SADDR_MASK                (0x07FFFFFFu)
#define _VTE_CD_SADDR_SADDR_SHIFT               (0)
#define  VTE_CD_SADDR_SADDR_DEFAULT             (0x00u)
#define  VTE_CD_SADDR_SADDR_OF(x)               _CCL_VALUEOF(_VTE_CD_SADDR_TYPE,x)


#define  VTE_CD_SADDR_OF(x)             _CCL_VALUEOF(_VTE_CD_SADDR_TYPE,x)

#define  VTE_CD_SADDR_DEFAULT (_VTE_CD_SADDR_TYPE)(    \
	 _CCL_PER_FDEFAULT(VTE,CD_SADDR,SADDR)             \
	)

#define VTE_CD_SADDR_RMK(saddr) (_VTE_CD_SADDR_TYPE)(  \
	 _CCL_PER_FMK  (VTE,CD_SADDR,SADDR,saddr)          \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  C D D A D D R    |
 * |___________________|
 *
 * CD_DADDR - Destination Address of decoded data Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  CD_DADDR - Source Address of encoded data
\*-------------------------------------------------------------------------*/
#define _VTE_CD_DADDR_OFFSET                    (0x0024u)
#define _VTE_CD_DADDR_ADDRH(h)                  VTE_ADDRH(h,CD_DADDR)
#define _VTE_CD_DADDR_LEN                       (32)                 /* 32-bit*/
#define _VTE_CD_DADDR_TYPE                      uint32_t

#define _VTE_CD_DADDR_DADDR_MASK                (0x07FFFFFFu)
#define _VTE_CD_DADDR_DADDR_SHIFT               (0)
#define  VTE_CD_DADDR_DADDR_DEFAULT             (0x00u)
#define  VTE_CD_DADDR_DADDR_OF(x)               _CCL_VALUEOF(_VTE_CD_DADDR_TYPE,x)


#define  VTE_CD_DADDR_OF(x)             _CCL_VALUEOF(_VTE_CD_DADDR_TYPE,x)

#define  VTE_CD_DADDR_DEFAULT (_VTE_CD_DADDR_TYPE)(    \
	 _CCL_PER_FDEFAULT(VTE,CD_DADDR,DADDR)             \
	)

#define VTE_CD_DADDR_RMK(daddr) (_VTE_CD_DADDR_TYPE)(  \
	 _CCL_PER_FMK  (VTE,CD_DADDR,DADDR,daddr)          \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  C D C T R L      |
 * |___________________|
 *
 * CD_CTRL - Channel Decoder Control Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  DATA_LEN (Data Length)
 *
 * (rw)  DATA_ERR (Data Error) -
 *                DATA_ERR  |   description
 *              ------------+---------------------------------------------
 *                 ERROR    | raw encoded data has error    (read)
 *                   OK     | raw encoded data has no error (read)
 *                 CLEAR    | Clear error or error is cleared (read/write)
 *
 * (rw)  MODE (decode mode) -
 *                 MODE     |   description
 *              ------------+---------------------------------------------
 *                  H84M0   | Hamming 8/4 Mode 0
 *                  H84M1   | Hamming 8/4 Mode 1
 *                  H2418   | Hamming 24/18
 *                  ODDPAR  | Odd Parity
 *                  AIT     | Special coding for Addtional Information Table
 *
 * (rw)  EN (decode enable) -
 *                   EN     |   description
 *              ------------+---------------------------------------------
 *                 DISABLE  | disable decoder
 *                 ENABLE   | enable decoder
\*-------------------------------------------------------------------------*/
#define _VTE_CD_CTRL_OFFSET                    (0x0028u)
#define _VTE_CD_CTRL_ADDRH(h)                  VTE_ADDRH(h,CD_CTRL)
#define _VTE_CD_CTRL_LEN                       (32)                 /* 32-bit*/
#define _VTE_CD_CTRL_TYPE                      uint32_t

#define _VTE_CD_CTRL_DATA_LEN_MASK             (0x01FF0000u)
#define _VTE_CD_CTRL_DATA_LEN_SHIFT            (16)
#define  VTE_CD_CTRL_DATA_LEN_DEFAULT          (0x000u)
#define  VTE_CD_CTRL_DATA_LEN_OF(x)            _CCL_VALUEOF(_VTE_CD_CTRL_TYPE,x)

#define _VTE_CD_CTRL_DATA_ERR_MASK             (0x00000010u)
#define _VTE_CD_CTRL_DATA_ERR_SHIFT            (4)
#define  VTE_CD_CTRL_DATA_ERR_DEFAULT          (0x00u)
#define  VTE_CD_CTRL_DATA_ERR_OF(x)            _CCL_VALUEOF(_VTE_CD_CTRL_TYPE,x)
#define  VTE_CD_CTRL_DATA_ERR_OK               (0x00u)
#define  VTE_CD_CTRL_DATA_ERR_ERROR            (0x01u)
#define  VTE_CD_CTRL_DATA_ERR_CLEAR            (0x00u)

#define _VTE_CD_CTRL_MODE_MASK                 (0x0000000Eu)
#define _VTE_CD_CTRL_MODE_SHIFT                (1)
#define  VTE_CD_CTRL_MODE_DEFAULT              (0x00u)
#define  VTE_CD_CTRL_MODE_OF(x)                _CCL_VALUEOF(_VTE_CD_CTRL_TYPE,x)
#define  VTE_CD_CTRL_MODE_H84M0                (0x00u)
#define  VTE_CD_CTRL_MODE_H84M1                (0x01u)
#define  VTE_CD_CTRL_MODE_H2418                (0x02u)
#define  VTE_CD_CTRL_MODE_ODDPAR               (0x03u)
#define  VTE_CD_CTRL_MODE_AIT                  (0x04u)

#define _VTE_CD_CTRL_EN_MASK                   (0x00000001u)
#define _VTE_CD_CTRL_EN_SHIFT                  (0)
#define  VTE_CD_CTRL_EN_DEFAULT                (0x00u)
#define  VTE_CD_CTRL_EN_OF(x)                  _CCL_VALUEOF(_VTE_CD_CTRL_TYPE,x)
#define  VTE_CD_CTRL_EN_DISABLE                (0x00u)
#define  VTE_CD_CTRL_EN_ENABLE                 (0x01u)


#define  VTE_CD_CTRL_OF(x)             _CCL_VALUEOF(_VTE_CD_CTRL_TYPE,x)

#define  VTE_CD_CTRL_DEFAULT (_VTE_CD_CTRL_TYPE)(    \
	 _CCL_PER_FDEFAULT(VTE,CD_CTRL,DATA_LEN)         \
	|_CCL_PER_FDEFAULT(VTE,CD_CTRL,DATA_ERR)         \
	|_CCL_PER_FDEFAULT(VTE,CD_CTRL,MODE    )         \
	|_CCL_PER_FDEFAULT(VTE,CD_CTRL,EN      )         \
	)

#define VTE_CD_CTRL_RMK(data_len, data_err, mode, en) (_VTE_CD_CTRL_TYPE)(  \
	 _CCL_PER_FMK  (VTE,CD_CTRL,DATA_LEN,data_len)                \
	|_CCL_PER_FMK  (VTE,CD_CTRL,DATA_ERR,data_err)                \
	|_CCL_PER_FMK  (VTE,CD_CTRL,MODE    ,mode    )                \
	|_CCL_PER_FMK  (VTE,CD_CTRL,EN      ,en      )                \
	)

#define VTE_CD_CTRL_RMKS(data_len, data_err, mode, en) (_VTE_CD_CTRL_TYPE)( \
	 _CCL_PER_FMK  (VTE,CD_CTRL,DATA_LEN,data_len)                \
	|_CCL_PER_FMKS (VTE,CD_CTRL,DATA_ERR,data_err)                \
	|_CCL_PER_FMKS (VTE,CD_CTRL,MODE    ,mode    )                \
	|_CCL_PER_FMKS (VTE,CD_CTRL,EN      ,en      )                \
	)



/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  CD ERR RVALUE    |
 * |___________________|
 *
 * CD_ERR_RVALUE  - Error Restore Value Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  ERR_RVALUE (Error Restore Value).
\*-------------------------------------------------------------------------*/
#define _VTE_CD_ERR_RVALUE_OFFSET                    (0x002Cu)
#define _VTE_CD_ERR_RVALUE_ADDRH(h)                  VTE_ADDRH(h,CD_ERR_RVALUE)
#define _VTE_CD_ERR_RVALUE_LEN                       (32)                 /* 32-bit*/
#define _VTE_CD_ERR_RVALUE_TYPE                      uint32_t

#define _VTE_CD_ERR_RVALUE_ERR_RVALUE_MASK           (0x00FFFFFFu)
#define _VTE_CD_ERR_RVALUE_ERR_RVALUE_SHIFT          (0)
#define  VTE_CD_ERR_RVALUE_ERR_RVALUE_DEFAULT        (0x00u)
#define  VTE_CD_ERR_RVALUE_ERR_RVALUE_OF(x)          _CCL_VALUEOF(_VTE_CD_ERR_RVALUE_TYPE,x)

#define  VTE_CD_ERR_RVALUE_OF(x)             _CCL_VALUEOF(_VTE_CD_ERR_RVALUE_TYPE,x)

#define  VTE_CD_ERR_RVALUE_DEFAULT (_VTE_CD_ERR_RVALUE_TYPE)(    \
	 _CCL_PER_FDEFAULT(VTE,CD_ERR_RVALUE,ERR_RVALUE)             \
	)

#define VTE_CD_ERR_RVALUE_RMK(err_rvalue) (_VTE_CD_ERR_RVALUE_TYPE)(  \
	 _CCL_PER_FMK  (VTE,CD_ERR_RVALUE,ERR_RVALUE,err_rvalue)                 \
	)







#endif	/* !__C_CHIP_LIB_REALTEK_SPARROW_VERTICAL_BLANKING_INTERVAL_TRANSFER_ENGINE_HARDWARE_ABSTRACT_LAYER_HEADER_INCLUDED__ */


/*-------------------------------------------------------------------------*\
 * end of vte_hal.h
\*-------------------------------------------------------------------------*/

