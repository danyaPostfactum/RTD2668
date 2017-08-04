/*-------------------------------------------------------------------------*\
 *               File: osd_hal.h
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
 *   OSD_ADDR   (<REG>)                  -   Register address
 *   OSD_RGET   (<REG>)                  -   Returns the value in the peripheral register
 *   OSD_RSET   (<REG>,x)                -   Register set
 *   OSD_FGET   (<REG>,<FIELD>)          -   Returns the value of the specified field
 *                                             in the peripheral register
 *   OSD_FSET   (<REG>,<FIELD>,fieldval) -   Writes fieldval to the specified field
 *                                             in the peripheral register
 *   OSD_FSETS  (<REG>,<FIELD>,<SYM>)    -   Writes the symbol value to the specified
 *                                             field in the peripheral
 *   OSD_RGETA  (addr,<REG>)             -   Gets register for a given address
 *   OSD_RSETA  (addr,<REG>,x)           -   Sets register for a given address
 *
 *  <NOTE>
 *  Not completed yet!!!!!!!!!!
\*-------------------------------------------------------------------------*/

#ifndef	__C_CHIP_LIB_REALTEK_SPARROW_VBI_TELETEXT_ON_SCREEN_DISPLAY_HARDWARE_ABSTRACT_LAYER_HEADER_INCLUDED__
#define	__C_CHIP_LIB_REALTEK_SPARROW_VBI_TELETEXT_ON_SCREEN_DISPLAY_HARDWARE_ABSTRACT_LAYER_HEADER_INCLUDED__ (1)


/* CCL headers ------------------------------------------------------------*/
#include <ccl/std.h>    /* the standard header of C Chip Library */

/*
 *  Base address of OSD
 */
#define _OSD_ADDR_BASE  (0x4800u)

/*
 *  The function used for read register from this IP. Prototype is
 *     uint32_t _OSD_REG_READ( addr );
 */
#define _OSD_REG_READ(addr)         TTX_Reg32Read(addr)

/*
 *  The function used for read register from this IP. Prototype is
 *     void    _OSD_REG_WRITE( addr, val );
 */
#define _OSD_REG_WRITE(addr,val)    TTX_Reg32Write(addr,val)

/*
 * The shift left of data-bus connection in bits, e.g.,
 *   conventionally, the D0-D7 of OSD are connected to CPU data-bus D0-D7 (_OSD_DB_SHFT = 0).
 *   However, in some cases,
 *     the D0-D7 of 82C52 are connected to CPU data-bus D16-D24 (_OSD_DB_SHFT = 16).
 */
#define _OSD_DB_SHFT     (0)

/*
 * The width of data-bus connection in bits, e.g.,
 *   conventionally, the D0-D7 of 82C52 are connected to CPU data-bus D0-D7 (_OSD_DB_WIDTH = 0x00FFu).
 *   Please note, despite the shift of data-bus, _OSD_DB_WITH starts from LSB.
 *   As a consequence,
 *     if the D0-D7 of 82C52 are connected to CPU data-bus D16-D24 (_OSD_DB_WIDTH = 0x00FFu).
 */
#define _OSD_DB_WIDTH   (0xFFFFFFFFu)

/*
 * The shift left of address-bus connection in bits, e.g.,
 *   conventionally, the A0-A7 of 82C52 are connected to CPU address-bus A0-A7 (_OSD_AB_SHFT = Uint8).
 *   However, in some cases,
 *     the A0-A7 of 82C52 are connected to CPU data-bus A2-A9 (_OSD_DB_SHFT = uint32_t).
 */
#define _OSD_AB_SHFT     uint16_t




/* Field related macros */

#define OSD_FMK(REG,FIELD,x)\
	_CCL_PER_FMK(OSD,REG,FIELD,x)

#define OSD_FMKS(REG,FIELD,SYM)\
	_CCL_PER_FMKS(OSD,REG,FIELD,SYM)

#define OSD_FEXTRACT(REG,FIELD,x)\
	_CCL_PER_FEXTRACT(OSD,REG,FIELD,x)


/* Raw register/field access macros */

#define OSD_ADDR(REG)\
    (_OSD_ADDR_BASE+_OSD_##REG##_OFFSET)

#define OSD_RGET(REG)\
    _OSD_RGETA(OSD_ADDR(REG),REG)

#define OSD_RSET(REG,x)\
    _OSD_RSETA(OSD_ADDR(REG),REG,x)

#define OSD_FGET(REG,FIELD)\
    _OSD_FGETA(OSD_ADDR(REG),REG,FIELD)

#define OSD_FSET(REG,FIELD,x)\
    _OSD_FSETA(OSD_ADDR(REG),REG,FIELD,x)

#define OSD_FSETS(REG,FIELD,SYM)\
    _OSD_FSETSA(OSD_ADDR(REG),REG,FIELD,SYM)


/* Address based register/field access macros */

#define _OSD_RGETA(addr,REG)\
	_CCL_PER_RGET(addr,OSD,REG,_OSD_DB_SHFT,_OSD_DB_WIDTH)

#define _OSD_RSETA(addr,REG,x)\
	_CCL_PER_RSET(addr,OSD,REG,x,_OSD_DB_SHFT,_OSD_DB_WIDTH)

#define _OSD_RSETSA(addr,REG,SYM)\
	_CCL_PER_RSETS(addr,OSD,REG,SYM,_OSD_DB_SHFT,_OSD_DB_WIDTH)

#define _OSD_FGETA(addr,REG,FIELD)\
	_CCL_PER_FGET(addr,OSD,REG,FIELD,_OSD_DB_SHFT,_OSD_DB_WIDTH)

#define _OSD_FSETA(addr,REG,FIELD,x)\
	_CCL_PER_FSET(addr,OSD,REG,FIELD,x,_OSD_DB_SHFT,_OSD_DB_WIDTH)

#define _OSD_FSETSA(addr,REG,FIELD,SYM)\
	_CCL_PER_FSETS(addr,OSD,REG,FIELD,SYM,_OSD_DB_SHFT,_OSD_DB_WIDTH)


/* Handle based register/field access macros */

/* This is for base-address determined by handle h.
#define OSD_ADDRH(h,REG)\
	(_OSD_AB_SHFT)(&((h)->addrBase[_OSD_##REG##_OFFSET]))
*/
#define OSD_ADDRH(h,REG)\
	(_OSD_AB_SHFT)(_OSD_ADDR_BASE+_OSD_##REG##_OFFSET)

#define OSD_RGETH(h,REG)\
	_OSD_RGETA(OSD_ADDRH(h,REG),REG)

#define OSD_RSETH(h,REG,x)\
	_OSD_RSETA(OSD_ADDRH(h,REG),REG,x)

#define OSD_RSETSH(h,REG,SYM)\
	_OSD_RSETSA(OSD_ADDRH(h,REG),REG,SYM)

#define OSD_FGETH(h,REG,FIELD)\
	_OSD_FGETA(OSD_ADDRH(h,REG),REG,FIELD)

#define OSD_FSETH(h,REG,FIELD,x)\
	_OSD_FSETA(OSD_ADDRH(h,REG),REG,FIELD,x)

#define OSD_FSETSH(h,REG,FIELD,SYM)\
	_OSD_FSETSA(OSD_ADDRH(h,REG),REG,FIELD,SYM)



/* Function Prototypes ----------------------------------------------------*/

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  C T R L 1        |
 * |___________________|
 *
 * CTRL1 - OSD Control Register 1
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  PATEN (Pattern Generation Enable) -
 *                  PATEN  |   description
 *                ---------+-------------------
 *                 DISABLE |   disable
 *                 ENABLE  |   enable
 *
 * (rw)  UVTOG (U/V Toggle for video 16)(debug mode) -
 *                  UVTOG  |   description
 *                ---------+-------------------
 *                 DISABLE |   disable
 *                 ENABLE  |   enable
 *
 * (rw)  LPF (Low Pass Filter enable) -
 *                   LPF   |   description
 *                ---------+-------------------
 *                 DISABLE |   disable
 *                 ENABLE  |   enable
 *
 * (rw)  TPCSEL (Select background color when transparent attribute is set) -
 *                 TPCSEL  |   description
 *                ---------+-------------------
 *                  VIDEO  | the same as video
 *                  GRAY   | gray
 *
 * (rw)  PXDLY  (Delay of H-sync of OSD in pixel clocks) - 0x00~0xFF
 *
 * (rw)  FLDTOG (Toggle field value from video decoder) -
 *                 FLDTOG  |   description
 *                ---------+-------------------
 *                 DISABLE |   disable
 *                 ENABLE  |   enable
 *
 * (rw)  CCF2PH (CC fringing is 2 pixel height (in E+S/W+S mode valid))  -
 *                 CCF2PH  |   description
 *                ---------+-------------------
 *                 DISABLE |   disable
 *                 ENABLE  |   enable
 *
 * (rw)  FRINGEE (Character fringe function enable/disable)  -
 *                 FRINGEE |   description
 *                ---------+-------------------
 *                 DISABLE |   disable
 *                 ENABLE  |   enable
 *
 * (rw)  FMODE (Fringe mode)  -
 *                  FMODE  |   description
 *                ---------+-------------------
 *                    ES   |   E + S
 *                    EN   |   E + N
 *                    WS   |   W + S
 *                    WN   |   W + N
 *                   EWSN  |   E + W + S + N
 *
 * (rw)  CCSCUT (CC character cell size cut in width (16 bits --> 12bits) )  -
 *                  CCSCUT |   description
 *                ---------+-------------------
 *                 DISABLE |   disable
 *                 ENABLE  |   enable
 *
 * (rw)  VTYPE (OSD output type)  -
 *                  VTYPE  |   description
 *                ---------+-------------------
 *                  PROG   |   progressive
 *                  INTL   |   interlace
 *
 * (rw)  DLTSEL (Double LATCH register selection)  -
 *                  DLTSEL |   description
 *                ---------+-----------------------------
 *                   1ST   | select 1st latch registers
 *                   2ND   | select 2nd latch registers
 *
 * (rw)  WDDR (With or without SDRAM, must set to 'SD' )  -
 *                  WDDR  |   description
 *                --------+-------------------
 *                   SD   |   with SDRAM
 *                  NOSD  |   without SDRAM
 *
 *
 * (rw)  TTR25E (OSD TT row 25 display enable/disable)  -
 *                 TTR25E  |   description
 *                ---------+-------------------
 *                 DISABLE |   disable
 *                 ENABLE  |   enable
 *
 * (rw)  IDXOUT (Index Out) -
 *                 IDXOUT  |   description
 *                ---------+--------------------------------------
 *                 DISABLE |
 *                 ENABLE  |
 *
 * (rw)  TTMODE - OSD TT display mode
 *                 TTMODE  |   description
 *                ---------+--------------------------------------
 *                   1P    | 1 page mode
 *                   2P    | 2 page mode
 *
 * (rw)  CMODE - Curor Mode
 *                  CMODE  |   description
 *                ---------+--------------------------------------
 *                    0    | cursor bitmap insertion. If OSD
 *                         |   output type is "CC", the bitmap size
 *                         |   of cursor is "16 x 26", otherwise, it is "12 x 10".
 *                    1    | foreground and background color of
 *                         |   characters in the cursor box is inverted
 *
 * (rw)  OTYPE - OSD Output type
 *                  OTYPE  |   description
 *                ---------+--------------------------------------
 *                   TT    | Teletext
 *                   CC    | Closed Caption
 *
 * (rw)  OMODE - OSD Output Mode
 *                  OMODE  |   description
 *                ---------+--------------------------------------
 *                   STD   | VBI standard mode
 *                   MIX   | mixed mode (VBI display data overlay with video)
 *
 * (rw)  OE - Output Enable
 *                   OE    |   description
 *                ---------+--------------------------------------
 *                 DISABLE | Don't display OSD
 *                 ENABLE  | Display OSD
\*-------------------------------------------------------------------------*/
#define _OSD_CTRL1_OFFSET                   (0x0000u)
#define _OSD_CTRL1_ADDRH(h)                 OSD_ADDRH(h,CTRL1)
#define _OSD_CTRL1_LEN                      (32)                 /* 32-bit*/
#define _OSD_CTRL1_TYPE                     uint32_t

#define _OSD_CTRL1_PATEN_MASK               (0x80000000u)
#define _OSD_CTRL1_PATEN_SHIFT              (31)
#define  OSD_CTRL1_PATEN_DEFAULT            (0x00u)
#define  OSD_CTRL1_PATEN_OF(x)              _CCL_VALUEOF(_OSD_CTRL1_TYPE,x)
#define  OSD_CTRL1_PATEN_DISABLE            (0x00u)
#define  OSD_CTRL1_PATEN_ENABLE             (0x01u)

#define _OSD_CTRL1_UVTOG_MASK               (0x40000000u)
#define _OSD_CTRL1_UVTOG_SHIFT              (30)
#define  OSD_CTRL1_UVTOG_DEFAULT            (0x00u)
#define  OSD_CTRL1_UVTOG_OF(x)              _CCL_VALUEOF(_OSD_CTRL1_TYPE,x)
#define  OSD_CTRL1_UVTOG_DISABLE            (0x00u)
#define  OSD_CTRL1_UVTOG_ENABLE             (0x01u)

#define _OSD_CTRL1_LPF_MASK                 (0x20000000u)
#define _OSD_CTRL1_LPF_SHIFT                (29)
#define  OSD_CTRL1_LPF_DEFAULT              (0x00u)
#define  OSD_CTRL1_LPF_OF(x)                _CCL_VALUEOF(_OSD_CTRL1_TYPE,x)
#define  OSD_CTRL1_LPF_DISABLE              (0x00u)
#define  OSD_CTRL1_LPF_ENABLE               (0x01u)

#define _OSD_CTRL1_TPCSEL_MASK              (0x10000000u)
#define _OSD_CTRL1_TPCSEL_SHIFT             (28)
#define  OSD_CTRL1_TPCSEL_DEFAULT           (0x00u)
#define  OSD_CTRL1_TPCSEL_OF(x)             _CCL_VALUEOF(_OSD_CTRL1_TYPE,x)
#define  OSD_CTRL1_TPCSEL_VIDEO             (0x00u)
#define  OSD_CTRL1_TPCSEL_GRAY              (0x01u)

#define _OSD_CTRL1_PXDLY_MASK               (0x0FF00000u)
#define _OSD_CTRL1_PXDLY_SHIFT              (20)
#define  OSD_CTRL1_PXDLY_DEFAULT            (0x00u)
#define  OSD_CTRL1_PXDLY_OF(x)              _CCL_VALUEOF(_OSD_CTRL1_TYPE,x)

#define _OSD_CTRL1_FLDTOG_MASK              (0x00040000u)
#define _OSD_CTRL1_FLDTOG_SHIFT             (18)
#define  OSD_CTRL1_FLDTOG_DEFAULT           (0x00u)
#define  OSD_CTRL1_FLDTOG_OF(x)             _CCL_VALUEOF(_OSD_CTRL1_TYPE,x)
#define  OSD_CTRL1_FLDTOG_DISABLE           (0x00u)
#define  OSD_CTRL1_FLDTOG_ENABLE            (0x01u)

#define _OSD_CTRL1_CCF2PH_MASK              (0x00020000u)
#define _OSD_CTRL1_CCF2PH_SHIFT             (17)
#define  OSD_CTRL1_CCF2PH_DEFAULT           (0x00u)
#define  OSD_CTRL1_CCF2PH_OF(x)             _CCL_VALUEOF(_OSD_CTRL1_TYPE,x)
#define  OSD_CTRL1_CCF2PH_DISABLE           (0x00u)
#define  OSD_CTRL1_CCF2PH_ENABLE            (0x01u)

#define _OSD_CTRL1_FRINGEE_MASK             (0x00010000u)
#define _OSD_CTRL1_FRINGEE_SHIFT            (16)
#define  OSD_CTRL1_FRINGEE_DEFAULT          (0x00u)
#define  OSD_CTRL1_FRINGEE_OF(x)            _CCL_VALUEOF(_OSD_CTRL1_TYPE,x)
#define  OSD_CTRL1_FRINGEE_DISABLE          (0x00u)
#define  OSD_CTRL1_FRINGEE_ENABLE           (0x01u)

#define _OSD_CTRL1_FMODE_MASK               (0x00007000u)
#define _OSD_CTRL1_FMODE_SHIFT              (12)
#define  OSD_CTRL1_FMODE_DEFAULT            (0x00u)
#define  OSD_CTRL1_FMODE_OF(x)              _CCL_VALUEOF(_OSD_CTRL1_TYPE,x)
#define  OSD_CTRL1_FMODE_ES                 (0x00u)
#define  OSD_CTRL1_FMODE_EN                 (0x01u)
#define  OSD_CTRL1_FMODE_WS                 (0x02u)
#define  OSD_CTRL1_FMODE_WN                 (0x03u)
#define  OSD_CTRL1_FMODE_EWSN               (0x04u)

#define _OSD_CTRL1_CCSCUT_MASK              (0x00000400u)
#define _OSD_CTRL1_CCSCUT_SHIFT             (10)
#define  OSD_CTRL1_CCSCUT_DEFAULT           (0x00u)
#define  OSD_CTRL1_CCSCUT_OF(x)             _CCL_VALUEOF(_OSD_CTRL1_TYPE,x)
#define  OSD_CTRL1_CCSCUT_DISABLE           (0x00u)
#define  OSD_CTRL1_CCSCUT_ENABLE            (0x01u)

#define _OSD_CTRL1_VTYPE_MASK               (0x00000200u)
#define _OSD_CTRL1_VTYPE_SHIFT              (9)
#define  OSD_CTRL1_VTYPE_DEFAULT            (0x00u)
#define  OSD_CTRL1_VTYPE_OF(x)              _CCL_VALUEOF(_OSD_CTRL1_TYPE,x)
#define  OSD_CTRL1_VTYPE_PROG               (0x00u)
#define  OSD_CTRL1_VTYPE_INTL               (0x01u)

#define _OSD_CTRL1_DLTSEL_MASK              (0x00000100u)
#define _OSD_CTRL1_DLTSEL_SHIFT             (8)
#define  OSD_CTRL1_DLTSEL_DEFAULT           (0x00u)
#define  OSD_CTRL1_DLTSEL_OF(x)             _CCL_VALUEOF(_OSD_CTRL1_TYPE,x)
#define  OSD_CTRL1_DLTSEL_1ST               (0x00u)
#define  OSD_CTRL1_DLTSEL_2ND               (0x01u)

#define _OSD_CTRL1_WDDR_MASK                (0x00000080u)
#define _OSD_CTRL1_WDDR_SHIFT               (7)
#define  OSD_CTRL1_WDDR_DEFAULT             (0x00u)
#define  OSD_CTRL1_WDDR_OF(x)               _CCL_VALUEOF(_OSD_CTRL1_TYPE,x)
#define  OSD_CTRL1_WDDR_SD                  (0x00u)
#define  OSD_CTRL1_WDDR_NOSD                (0x01u)

#define _OSD_CTRL1_TTR25E_MASK              (0x00000040u)
#define _OSD_CTRL1_TTR25E_SHIFT             (6)
#define  OSD_CTRL1_TTR25E_DEFAULT           (0x00u)
#define  OSD_CTRL1_TTR25E_OF(x)             _CCL_VALUEOF(_OSD_CTRL1_TYPE,x)
#define  OSD_CTRL1_TTR25E_DISABLE           (0x00u)
#define  OSD_CTRL1_TTR25E_ENABLE            (0x01u)

#define _OSD_CTRL1_IDXOUT_MASK              (0x00000020u)
#define _OSD_CTRL1_IDXOUT_SHIFT             (5)
#define  OSD_CTRL1_IDXOUT_DEFAULT           (0x00u)
#define  OSD_CTRL1_IDXOUT_OF(x)             _CCL_VALUEOF(_OSD_CTRL1_TYPE,x)
#define  OSD_CTRL1_IDXOUT_DISABLE           (0x00u)
#define  OSD_CTRL1_IDXOUT_ENABLE            (0x01u)

#define _OSD_CTRL1_TTMODE_MASK              (0x00000010u)
#define _OSD_CTRL1_TTMODE_SHIFT             (4)
#define  OSD_CTRL1_TTMODE_DEFAULT           (0x00u)
#define  OSD_CTRL1_TTMODE_OF(x)             _CCL_VALUEOF(_OSD_CTRL1_TYPE,x)
#define  OSD_CTRL1_TTMODE_1P                (0x00u)
#define  OSD_CTRL1_TTMODE_2P                (0x01u)

#define _OSD_CTRL1_CMODE_MASK               (0x00000008u)
#define _OSD_CTRL1_CMODE_SHIFT              (3)
#define  OSD_CTRL1_CMODE_DEFAULT            (0x00u)
#define  OSD_CTRL1_CMODE_OF(x)              _CCL_VALUEOF(_OSD_CTRL1_TYPE,x)
#define  OSD_CTRL1_CMODE_0                  (0x00u)
#define  OSD_CTRL1_CMODE_1                  (0x01u)

#define _OSD_CTRL1_OTYPE_MASK               (0x00000004u)
#define _OSD_CTRL1_OTYPE_SHIFT              (2)
#define  OSD_CTRL1_OTYPE_DEFAULT            (0x00u)
#define  OSD_CTRL1_OTYPE_OF(x)              _CCL_VALUEOF(_OSD_CTRL1_TYPE,x)
#define  OSD_CTRL1_OTYPE_TT                 (0x00u)
#define  OSD_CTRL1_OTYPE_CC                 (0x01u)

#define _OSD_CTRL1_OMODE_MASK               (0x00000002u)
#define _OSD_CTRL1_OMODE_SHIFT              (1)
#define  OSD_CTRL1_OMODE_DEFAULT            (0x00u)
#define  OSD_CTRL1_OMODE_OF(x)              _CCL_VALUEOF(_OSD_CTRL1_TYPE,x)
#define  OSD_CTRL1_OMODE_STD                (0x00u)
#define  OSD_CTRL1_OMODE_MIX                (0x01u)

#define _OSD_CTRL1_OE_MASK                  (0x00000001u)
#define _OSD_CTRL1_OE_SHIFT                 (0)
#define  OSD_CTRL1_OE_DEFAULT               (0x00u)
#define  OSD_CTRL1_OE_OF(x)                 _CCL_VALUEOF(_OSD_CTRL1_TYPE,x)
#define  OSD_CTRL1_OE_DISABLE               (0x00u)
#define  OSD_CTRL1_OE_ENABLE                (0x01u)

#define  OSD_CTRL1_OF(x)             _CCL_VALUEOF(_OSD_CTRL1_TYPE,x)

#define  OSD_CTRL1_DEFAULT (_OSD_CTRL1_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,CTRL1,PATEN  )      \
	|_CCL_PER_FDEFAULT(OSD,CTRL1,UVTOG  )      \
	|_CCL_PER_FDEFAULT(OSD,CTRL1,LPF    )      \
	|_CCL_PER_FDEFAULT(OSD,CTRL1,TPCSEL )      \
	|_CCL_PER_FDEFAULT(OSD,CTRL1,PXDLY  )      \
	|_CCL_PER_FDEFAULT(OSD,CTRL1,FLDTOG )      \
	|_CCL_PER_FDEFAULT(OSD,CTRL1,CCF2PH )      \
	|_CCL_PER_FDEFAULT(OSD,CTRL1,FRINGEE)      \
	|_CCL_PER_FDEFAULT(OSD,CTRL1,FMODE  )      \
	|_CCL_PER_FDEFAULT(OSD,CTRL1,CCSCUT )      \
	|_CCL_PER_FDEFAULT(OSD,CTRL1,VTYPE  )      \
	|_CCL_PER_FDEFAULT(OSD,CTRL1,DLTSEL )      \
	|_CCL_PER_FDEFAULT(OSD,CTRL1,WDDR   )      \
	|_CCL_PER_FDEFAULT(OSD,CTRL1,TTR25E )      \
	|_CCL_PER_FDEFAULT(OSD,CTRL1,IDXOUT )      \
	|_CCL_PER_FDEFAULT(OSD,CTRL1,TTMODE )      \
	|_CCL_PER_FDEFAULT(OSD,CTRL1,CMODE  )      \
	|_CCL_PER_FDEFAULT(OSD,CTRL1,OTYPE  )      \
	|_CCL_PER_FDEFAULT(OSD,CTRL1,OMODE  )      \
	|_CCL_PER_FDEFAULT(OSD,CTRL1,OE     )      \
	)

#define OSD_CTRL1_RMK(paten, uvtog, lpf, tpcsel, pxdly, fldtog, ccf2ph, fringee, fmode, ccscut, vtype, dltsel, wddr, ttr25e, idxout, ttmode, cmode, otype, omode, oe) (_OSD_CTRL1_TYPE)( \
	 _CCL_PER_FMK  (OSD,CTRL1,PATEN  ,paten  )     \
	|_CCL_PER_FMK  (OSD,CTRL1,UVTOG  ,uvtog  )     \
	|_CCL_PER_FMK  (OSD,CTRL1,LPF    ,lpf    )     \
	|_CCL_PER_FMK  (OSD,CTRL1,TPCSEL ,tpcsel )     \
	|_CCL_PER_FMK  (OSD,CTRL1,PXDLY  ,pxdly  )     \
	|_CCL_PER_FMK  (OSD,CTRL1,FLDTOG ,fldtog )     \
	|_CCL_PER_FMK  (OSD,CTRL1,CCF2PH ,ccf2ph )     \
	|_CCL_PER_FMK  (OSD,CTRL1,FRINGEE,fringee)     \
	|_CCL_PER_FMK  (OSD,CTRL1,FMODE  ,fmode  )     \
	|_CCL_PER_FMK  (OSD,CTRL1,CCSCUT ,ccscut )     \
	|_CCL_PER_FMK  (OSD,CTRL1,VTYPE  ,vtype  )     \
	|_CCL_PER_FMK  (OSD,CTRL1,DLTSEL ,dltsel )     \
	|_CCL_PER_FMK  (OSD,CTRL1,WDDR   ,wddr   )     \
	|_CCL_PER_FMK  (OSD,CTRL1,TTR25E ,ttr25e )     \
	|_CCL_PER_FMK  (OSD,CTRL1,IDXOUT ,idxout )     \
	|_CCL_PER_FMK  (OSD,CTRL1,TTMODE ,ttmode )     \
	|_CCL_PER_FMK  (OSD,CTRL1,CMODE  ,cmode  )     \
	|_CCL_PER_FMK  (OSD,CTRL1,OTYPE  ,otype  )     \
	|_CCL_PER_FMK  (OSD,CTRL1,OMODE  ,omode  )     \
	|_CCL_PER_FMK  (OSD,CTRL1,OE     ,oe     )     \
	)

#define OSD_CTRL1_RMKS(paten, uvtog, lpf, tpcsel, pxdly, fldtog, ccf2ph, fringee, fmode, ccscut, vtype, dltsel, wddr, ttr25e, idxout, ttmode, cmode, otype, omode, oe) (_OSD_CTRL1_TYPE)( \
	 _CCL_PER_FMKS (OSD,CTRL1,PATEN  ,paten  )     \
	|_CCL_PER_FMKS (OSD,CTRL1,UVTOG  ,uvtog  )     \
	|_CCL_PER_FMKS (OSD,CTRL1,LPF    ,lpf    )     \
	|_CCL_PER_FMKS (OSD,CTRL1,TPCSEL ,tpcsel )     \
	|_CCL_PER_FMK  (OSD,CTRL1,PXDLY  ,pxdly  )     \
	|_CCL_PER_FMKS (OSD,CTRL1,FLDTOG ,fldtog )     \
	|_CCL_PER_FMKS (OSD,CTRL1,CCF2PH ,ccf2ph )     \
	|_CCL_PER_FMKS (OSD,CTRL1,FRINGEE,fringee)     \
	|_CCL_PER_FMKS (OSD,CTRL1,FMODE  ,fmode  )     \
	|_CCL_PER_FMKS (OSD,CTRL1,CCSCUT ,ccscut )     \
	|_CCL_PER_FMKS (OSD,CTRL1,VTYPE  ,vtype  )     \
	|_CCL_PER_FMKS (OSD,CTRL1,DLTSEL ,dltsel )     \
	|_CCL_PER_FMKS (OSD,CTRL1,WDDR   ,wddr   )     \
	|_CCL_PER_FMKS (OSD,CTRL1,TTR25E ,ttr25e )     \
	|_CCL_PER_FMKS (OSD,CTRL1,IDXOUT ,idxout )     \
	|_CCL_PER_FMKS (OSD,CTRL1,TTMODE ,ttmode )     \
	|_CCL_PER_FMKS (OSD,CTRL1,CMODE  ,cmode  )     \
	|_CCL_PER_FMKS (OSD,CTRL1,OTYPE  ,otype  )     \
	|_CCL_PER_FMKS (OSD,CTRL1,OMODE  ,omode  )     \
	|_CCL_PER_FMKS (OSD,CTRL1,OE     ,oe     )     \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  C T R L 2        |
 * |___________________|
 *
 * CTRL2 - OSD Control Register 2
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  MBM (Separate mosaic character bit mask) - One bit in this field represents a bit in a character.
 *                   MBM   |   description
 *                ---------+-------------------
 *                  NORM   |   normal display
 *                  MASK   |   display mask
 *
 * (rw)  MLM (Separate mosaic character line mask) - One bit in this field represents a line in a character.
 *                   MLM   |   description
 *                ---------+-------------------
 *                  NORM   |   normal display
 *                  MASK   |   display mask
\*-------------------------------------------------------------------------*/
#define _OSD_CTRL2_OFFSET                   (0x0004u)
#define _OSD_CTRL2_ADDRH(h)                 OSD_ADDRH(h,CTRL2)
#define _OSD_CTRL2_LEN                      (32)                 /* 32-bit*/
#define _OSD_CTRL2_TYPE                     uint32_t

#define _OSD_CTRL2_MBM_MASK                 (0x0FFF0000u)
#define _OSD_CTRL2_MBM_SHIFT                (16)
#define  OSD_CTRL2_MBM_DEFAULT              (0x861u)
#define  OSD_CTRL2_MBM_OF(x)                _CCL_VALUEOF(_OSD_CTRL2_TYPE,x)

#define _OSD_CTRL2_MLM_MASK                 (0x000003FFu)
#define _OSD_CTRL2_MLM_SHIFT                (0)
#define  OSD_CTRL2_MLM_DEFAULT              (0x249u)
#define  OSD_CTRL2_MLM_OF(x)                _CCL_VALUEOF(_OSD_CTRL2_TYPE,x)

#define  OSD_CTRL2_OF(x)             _CCL_VALUEOF(_OSD_CTRL2_TYPE,x)

#define  OSD_CTRL2_DEFAULT (_OSD_CTRL2_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,CTRL2,MBM)          \
	|_CCL_PER_FDEFAULT(OSD,CTRL2,MLM)          \
	)

#define OSD_CTRL2_RMK(mbm, mlm) (_OSD_CTRL2_TYPE)( \
	 _CCL_PER_FMK  (OSD,CTRL2,MBM,mbm)             \
	|_CCL_PER_FMK  (OSD,CTRL2,MLM,mlm)             \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  C T R L 3        |
 * |___________________|
 *
 * CTRL3 - OSD Control Register 3
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  SMEN (TT Character Smooth function enable/disable)  -
 *                  SMEN   |   description
 *                ---------+-------------------
 *                 DISABLE |   disable
 *                 ENABLE  |   enable
 *
 * (rw)  CNCDIS (TT conceal enable)  -
 *                  CNCDIS |   description
 *                ---------+-------------------
 *                 ENABLE  |  conceal enable, conceal attribute of character is effective
 *                 DISABLE |  conceal disable, conceal attribute of character is ignored
 *
 * (rw)  TTVS (OSD TT vertical scaling mode (only valid when row 25 display is disabled))  -
 *                  TTVS   |   description
 *                ---------+-------------------
 *                  MODE0  |   row 0 fixed
 *                  MODE1  |   row 24 fixed
 *
 * (rw)  VSR (Vertical scaling rate (only valid for TT) )  -
 *                   VSR   |   description
 *                ---------+-------------------------------------------
 *                  MODE0  |  1
 *                  MODE1  |  2, only upper half of page is displayed
 *                  MODE2  |  2, only lower half of page is displayed
 *
 * (rw)  VDLY (VSYNC delay)  - 0~511 line
 *                   VDLY  |   description
 *                ---------+-------------------------------------------
 *                   MIN   |  0
 *                   MAX   |  511
 *
 * (rw)  HDLY1 (HSYNC delay 1)  - 0~255 pixel
 *                   HDLY1 |   description
 *                ---------+-------------------------------------------
 *                   MIN   |  0
 *                   MAX   |  255
\*-------------------------------------------------------------------------*/
#define _OSD_CTRL3_OFFSET                   (0x0008u)
#define _OSD_CTRL3_ADDRH(h)                 OSD_ADDRH(h,CTRL3)
#define _OSD_CTRL3_LEN                      (32)                 /* 32-bit*/
#define _OSD_CTRL3_TYPE                     uint32_t

#define _OSD_CTRL3_SMEN_MASK                (0x80000000u)
#define _OSD_CTRL3_SMEN_SHIFT               (31)
#define  OSD_CTRL3_SMEN_DEFAULT             (0x0u)
#define  OSD_CTRL3_SMEN_OF(x)               _CCL_VALUEOF(_OSD_CTRL3_TYPE,x)
#define  OSD_CTRL3_SMEN_DISABLE             (0x00u)
#define  OSD_CTRL3_SMEN_ENABLE              (0x01u)

#define _OSD_CTRL3_CNCDIS_MASK              (0x40000000u)
#define _OSD_CTRL3_CNCDIS_SHIFT             (30)
#define  OSD_CTRL3_CNCDIS_DEFAULT           (0x0u)
#define  OSD_CTRL3_CNCDIS_OF(x)             _CCL_VALUEOF(_OSD_CTRL3_TYPE,x)
#define  OSD_CTRL3_CNCDIS_ENABLE            (0x00u)
#define  OSD_CTRL3_CNCDIS_DISABLE           (0x01u)

#define _OSD_CTRL3_TTVS_MASK                (0x20000000u)
#define _OSD_CTRL3_TTVS_SHIFT               (29)
#define  OSD_CTRL3_TTVS_DEFAULT             (0x0u)
#define  OSD_CTRL3_TTVS_OF(x)               _CCL_VALUEOF(_OSD_CTRL3_TYPE,x)
#define  OSD_CTRL3_TTVS_MODE0               (0x00u)
#define  OSD_CTRL3_TTVS_MODE1               (0x01u)

#define _OSD_CTRL3_VSR_MASK                 (0x03000000u)
#define _OSD_CTRL3_VSR_SHIFT                (24)
#define  OSD_CTRL3_VSR_DEFAULT              (0x0u)
#define  OSD_CTRL3_VSR_OF(x)                _CCL_VALUEOF(_OSD_CTRL3_TYPE,x)
#define  OSD_CTRL3_VSR_MODE0                (0x00u)
#define  OSD_CTRL3_VSR_MODE1                (0x01u)
#define  OSD_CTRL3_VSR_MODE2                (0x02u)

#define _OSD_CTRL3_VDLY_MASK                (0x0001FF00u)
#define _OSD_CTRL3_VDLY_SHIFT               (8)
#define  OSD_CTRL3_VDLY_DEFAULT             (0x0u)
#define  OSD_CTRL3_VDLY_OF(x)               _CCL_VALUEOF(_OSD_CTRL3_TYPE,x)
#define  OSD_CTRL3_VDLY_MIN                 (0)
#define  OSD_CTRL3_VDLY_MAX                 (511)

#define _OSD_CTRL3_HDLY1_MASK               (0x000000FFu)
#define _OSD_CTRL3_HDLY1_SHIFT              (0)
#define  OSD_CTRL3_HDLY1_DEFAULT            (0x0u)
#define  OSD_CTRL3_HDLY1_OF(x)              _CCL_VALUEOF(_OSD_CTRL3_TYPE,x)
#define  OSD_CTRL3_HDLY1_MIN                (0)
#define  OSD_CTRL3_HDLY1_MAX                (255)

#define  OSD_CTRL3_OF(x)             _CCL_VALUEOF(_OSD_CTRL3_TYPE,x)

#define  OSD_CTRL3_DEFAULT (_OSD_CTRL3_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,CTRL3,SMEN  )       \
	|_CCL_PER_FDEFAULT(OSD,CTRL3,CNCDIS)       \
	|_CCL_PER_FDEFAULT(OSD,CTRL3,TTVS  )       \
	|_CCL_PER_FDEFAULT(OSD,CTRL3,VSR   )       \
	|_CCL_PER_FDEFAULT(OSD,CTRL3,VDLY  )       \
	|_CCL_PER_FDEFAULT(OSD,CTRL3,HDLY1 )       \
	)

#define OSD_CTRL3_RMK(smen, cncdis, ttvs, vsr, vdly, hdly1) (_OSD_CTRL3_TYPE)( \
	 _CCL_PER_FMK  (OSD,CTRL3,SMEN  ,smen  )             \
	|_CCL_PER_FMK  (OSD,CTRL3,CNCDIS,cncdis)             \
	|_CCL_PER_FMK  (OSD,CTRL3,TTVS  ,ttvs  )             \
	|_CCL_PER_FMK  (OSD,CTRL3,VSR   ,vsr   )             \
	|_CCL_PER_FMK  (OSD,CTRL3,VDLY  ,vdly  )             \
	|_CCL_PER_FMK  (OSD,CTRL3,HDLY1 ,hdly1 )             \
	)

#define OSD_CTRL3_RMKS(smen, cncdis, ttvs, vsr, vdly, hdly1) (_OSD_CTRL3_TYPE)( \
	 _CCL_PER_FMKS (OSD,CTRL3,SMEN  ,smen  )             \
	|_CCL_PER_FMKS (OSD,CTRL3,CNCDIS,cncdis)             \
	|_CCL_PER_FMKS (OSD,CTRL3,TTVS  ,ttvs  )             \
	|_CCL_PER_FMKS (OSD,CTRL3,VSR   ,vsr   )             \
	|_CCL_PER_FMK  (OSD,CTRL3,VDLY  ,vdly  )             \
	|_CCL_PER_FMK  (OSD,CTRL3,HDLY1 ,hdly1 )             \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  C T R L 4        |
 * |___________________|
 *
 * CTRL4 - OSD Control Register 4
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  HDLY2 (HSYNC delay 2)  - 0~511 pixel
 *                   HDLY2 |   description
 *                ---------+-------------------------------------------
 *                   MIN   |  0
 *                   MAX   |  511
 *
 * (rw)  HDLY3 (HSYNC delay 3)  - 0~511 pixel
 *                   HDLY3 |   description
 *                ---------+-------------------------------------------
 *                   MIN   |  0
 *                   MAX   |  511
\*-------------------------------------------------------------------------*/
#define _OSD_CTRL4_OFFSET                   (0x000Cu)
#define _OSD_CTRL4_ADDRH(h)                 OSD_ADDRH(h,CTRL4)
#define _OSD_CTRL4_LEN                      (32)                 /* 32-bit*/
#define _OSD_CTRL4_TYPE                     uint32_t

#define _OSD_CTRL4_HDLY2_MASK               (0x01FF0000u)
#define _OSD_CTRL4_HDLY2_SHIFT              (16)
#define  OSD_CTRL4_HDLY2_DEFAULT            (0x0u)
#define  OSD_CTRL4_HDLY2_OF(x)              _CCL_VALUEOF(_OSD_CTRL4_TYPE,x)
#define  OSD_CTRL4_HDLY2_MIN                (0)
#define  OSD_CTRL4_HDLY2_MAX                (511)

#define _OSD_CTRL4_HDLY3_MASK               (0x000001FFu)
#define _OSD_CTRL4_HDLY3_SHIFT              (0)
#define  OSD_CTRL4_HDLY3_DEFAULT            (0x0u)
#define  OSD_CTRL4_HDLY3_OF(x)              _CCL_VALUEOF(_OSD_CTRL4_TYPE,x)
#define  OSD_CTRL4_HDLY3_MIN                (0)
#define  OSD_CTRL4_HDLY3_MAX                (511)

#define  OSD_CTRL4_OF(x)             _CCL_VALUEOF(_OSD_CTRL4_TYPE,x)

#define  OSD_CTRL4_DEFAULT (_OSD_CTRL4_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,CTRL4,HDLY2)        \
	|_CCL_PER_FDEFAULT(OSD,CTRL4,HDLY3)        \
	)

#define OSD_CTRL4_RMK(hdly2, hdly3) (_OSD_CTRL4_TYPE)( \
	 _CCL_PER_FMK  (OSD,CTRL4,HDLY2,hdly2)             \
	|_CCL_PER_FMK  (OSD,CTRL4,HDLY3,hdly3)             \
	)

#define OSD_CTRL4_RMKS(hdly2, hdly3) (_OSD_CTRL4_TYPE)( \
	 _CCL_PER_FMKS  (OSD,CTRL4,HDLY2,hdly2)             \
	|_CCL_PER_FMKS  (OSD,CTRL4,HDLY3,hdly3)             \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  C T R L 5        |
 * |___________________|
 *
 * CTRL5 - OSD Control Register 5
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  HDREXT (Header Extensible) - In rolling header / timecode, header can be extended for DH
 *                 HDREXT  |   description
 *                ---------+-------------------
 *                 DISABLE |   can't be extended
 *                 ENABLE  |   can be extended
 *
 * (rw)  HDDWEN (Header Double-Width Enable) - In rolling header / timecode, header allow DW
 *                 HDDWEN  |   description
 *                ---------+-------------------
 *                 DISABLE |   disallow
 *                 ENABLE  |   allow
 *
 * (rw)  HDDHEN (Header Double-Height Enable) - In rolling header / timecode, header allow DH
 *                 HDDHEN  |   description
 *                ---------+-------------------
 *                 DISABLE |   disallow
 *                 ENABLE  |   allow
 *
 * (rw)  HDDWMD (Header Double-Width Mode)  - In rolling header / timecode, header DW mode
 *                 HDDWMD  |   description
 *                ---------+-------------------
 *                 MICRONAS|   Micronas mode
 *                 PHILIPS |   Philips mode
 *
 * (rw)  HDPNSP (Header Page-Number Space) - In rolling header / timecode, row 1 allow be set
 *                                             space below page number when row 0 has a DH
 *                 HDPNSP  |   description
 *                ---------+-------------------
 *                 DISABLE |   disallow
 *                 ENABLE  |   allow
 *
 * (rw)  DHCMDEN (Double-Height Command Enable) - Enable DH command
 *                 DHCMDEN |   description
 *                ---------+-------------------
 *                 DISABLE |   disable
 *                 ENABLE  |   enable
 *
 * (rw)  DWCMDEN (Double-Width Command Enable) - Enable DW command
 *                 DWCMDEN |   description
 *                ---------+-------------------
 *                 DISABLE |   disable
 *                 ENABLE  |   enable
 *
 * (rw)  DSCMDEN (Double-Size Command Enable) - Enable DS command
 *                 DSCMDEN |   description
 *                ---------+-------------------
 *                 DISABLE |   disable
 *                 ENABLE  |   enable
 *
 * (rw)  HDG1CS (Header G1 Character Set) - TT header default G1 character set
 *
 * (rw)  HDG0CS (Header G0 Character Set) - TT header default G0 character set
\*-------------------------------------------------------------------------*/
#define _OSD_CTRL5_OFFSET                   (0x0010u)
#define _OSD_CTRL5_ADDRH(h)                 OSD_ADDRH(h,CTRL5)
#define _OSD_CTRL5_LEN                      (32)                 /* 32-bit*/
#define _OSD_CTRL5_TYPE                     uint32_t

#define _OSD_CTRL5_HDREXT_MASK              (0x80000000u)
#define _OSD_CTRL5_HDREXT_SHIFT             (31)
#define  OSD_CTRL5_HDREXT_DEFAULT           (0x0u)
#define  OSD_CTRL5_HDREXT_OF(x)             _CCL_VALUEOF(_OSD_CTRL5_TYPE,x)
#define  OSD_CTRL5_HDREXT_DISABLE           (0x00u)
#define  OSD_CTRL5_HDREXT_ENABLE            (0x01u)

#define _OSD_CTRL5_HDDWEN_MASK              (0x40000000u)
#define _OSD_CTRL5_HDDWEN_SHIFT             (30)
#define  OSD_CTRL5_HDDWEN_DEFAULT           (0x0u)
#define  OSD_CTRL5_HDDWEN_OF(x)             _CCL_VALUEOF(_OSD_CTRL5_TYPE,x)
#define  OSD_CTRL5_HDDWEN_DISABLE           (0x00u)
#define  OSD_CTRL5_HDDWEN_ENABLE            (0x01u)

#define _OSD_CTRL5_HDDHEN_MASK              (0x20000000u)
#define _OSD_CTRL5_HDDHEN_SHIFT             (29)
#define  OSD_CTRL5_HDDHEN_DEFAULT           (0x0u)
#define  OSD_CTRL5_HDDHEN_OF(x)             _CCL_VALUEOF(_OSD_CTRL5_TYPE,x)
#define  OSD_CTRL5_HDDHEN_DISABLE           (0x00u)
#define  OSD_CTRL5_HDDHEN_ENABLE            (0x01u)

#define _OSD_CTRL5_HDDWMD_MASK              (0x10000000u)
#define _OSD_CTRL5_HDDWMD_SHIFT             (28)
#define  OSD_CTRL5_HDDWMD_DEFAULT           (0x0u)
#define  OSD_CTRL5_HDDWMD_OF(x)             _CCL_VALUEOF(_OSD_CTRL5_TYPE,x)
#define  OSD_CTRL5_HDDWMD_MICRONAS          (0x00u)
#define  OSD_CTRL5_HDDWMD_PHILIPS           (0x01u)

#define _OSD_CTRL5_HDPNSP_MASK              (0x08000000u)
#define _OSD_CTRL5_HDPNSP_SHIFT             (27)
#define  OSD_CTRL5_HDPNSP_DEFAULT           (0x0u)
#define  OSD_CTRL5_HDPNSP_OF(x)             _CCL_VALUEOF(_OSD_CTRL5_TYPE,x)
#define  OSD_CTRL5_HDPNSP_DISABLE           (0x00u)
#define  OSD_CTRL5_HDPNSP_ENABLE            (0x01u)

#define _OSD_CTRL5_DHCMDEN_MASK             (0x04000000u)
#define _OSD_CTRL5_DHCMDEN_SHIFT            (26)
#define  OSD_CTRL5_DHCMDEN_DEFAULT          (0x0u)
#define  OSD_CTRL5_DHCMDEN_OF(x)            _CCL_VALUEOF(_OSD_CTRL5_TYPE,x)
#define  OSD_CTRL5_DHCMDEN_DISABLE          (0x00u)
#define  OSD_CTRL5_DHCMDEN_ENABLE           (0x01u)

#define _OSD_CTRL5_DWCMDEN_MASK             (0x02000000u)
#define _OSD_CTRL5_DWCMDEN_SHIFT            (25)
#define  OSD_CTRL5_DWCMDEN_DEFAULT          (0x0u)
#define  OSD_CTRL5_DWCMDEN_OF(x)            _CCL_VALUEOF(_OSD_CTRL5_TYPE,x)
#define  OSD_CTRL5_DWCMDEN_DISABLE          (0x00u)
#define  OSD_CTRL5_DWCMDEN_ENABLE           (0x01u)

#define _OSD_CTRL5_DSCMDEN_MASK             (0x01000000u)
#define _OSD_CTRL5_DSCMDEN_SHIFT            (24)
#define  OSD_CTRL5_DSCMDEN_DEFAULT          (0x0u)
#define  OSD_CTRL5_DSCMDEN_OF(x)            _CCL_VALUEOF(_OSD_CTRL5_TYPE,x)
#define  OSD_CTRL5_DSCMDEN_DISABLE          (0x00u)
#define  OSD_CTRL5_DSCMDEN_ENABLE           (0x01u)

#define _OSD_CTRL5_HDG1CS_MASK              (0x00001F00u)
#define _OSD_CTRL5_HDG1CS_SHIFT             (8)
#define  OSD_CTRL5_HDG1CS_DEFAULT           (0x0u)
#define  OSD_CTRL5_HDG1CS_OF(x)             _CCL_VALUEOF(_OSD_CTRL5_TYPE,x)

#define _OSD_CTRL5_HDG0CS_MASK              (0x0000001Fu)
#define _OSD_CTRL5_HDG0CS_SHIFT             (0)
#define  OSD_CTRL5_HDG0CS_DEFAULT           (0x0u)
#define  OSD_CTRL5_HDG0CS_OF(x)             _CCL_VALUEOF(_OSD_CTRL5_TYPE,x)

#define  OSD_CTRL5_OF(x)             _CCL_VALUEOF(_OSD_CTRL5_TYPE,x)

#define  OSD_CTRL5_DEFAULT (_OSD_CTRL5_TYPE)(   \
	 _CCL_PER_FDEFAULT(OSD,CTRL5,HDREXT )       \
	|_CCL_PER_FDEFAULT(OSD,CTRL5,HDDWEN )       \
	|_CCL_PER_FDEFAULT(OSD,CTRL5,HDDHEN )       \
	|_CCL_PER_FDEFAULT(OSD,CTRL5,HDDWMD )       \
	|_CCL_PER_FDEFAULT(OSD,CTRL5,HDPNSP )       \
	|_CCL_PER_FDEFAULT(OSD,CTRL5,DHCMDEN)       \
	|_CCL_PER_FDEFAULT(OSD,CTRL5,DWCMDEN)       \
	|_CCL_PER_FDEFAULT(OSD,CTRL5,DSCMDEN)       \
	|_CCL_PER_FDEFAULT(OSD,CTRL5,HDG1CS )       \
	|_CCL_PER_FDEFAULT(OSD,CTRL5,HDG0CS )       \
	)

#define OSD_CTRL5_RMK(hdrext, hddwen, hddhen, hddwmd, hdpnsp, dhcmden, dwcmden, dscmden, hdg1cs, hdg0cs) (_OSD_CTRL5_TYPE)( \
	 _CCL_PER_FMK  (OSD,CTRL5,HDREXT ,hdrext )             \
	|_CCL_PER_FMK  (OSD,CTRL5,HDDWEN ,hddwen )             \
	|_CCL_PER_FMK  (OSD,CTRL5,HDDHEN ,hddhen )             \
	|_CCL_PER_FMK  (OSD,CTRL5,HDDWMD ,hddwmd )             \
	|_CCL_PER_FMK  (OSD,CTRL5,HDPNSP ,hdpnsp )             \
	|_CCL_PER_FMK  (OSD,CTRL5,HDDWEN ,hddwen )             \
	|_CCL_PER_FMK  (OSD,CTRL5,DHCMDEN,dhcmden)             \
	|_CCL_PER_FMK  (OSD,CTRL5,DWCMDEN,dwcmden)             \
	|_CCL_PER_FMK  (OSD,CTRL5,DSCMDEN,dscmden)             \
	|_CCL_PER_FMK  (OSD,CTRL5,HDG1CS ,hdg1cs )             \
	|_CCL_PER_FMK  (OSD,CTRL5,HDG0CS ,hdg0cs )             \
	)

#define OSD_CTRL5_RMKS(hdrext, hddwen, hddhen, hddwmd, hdpnsp, dhcmden, dwcmden, dscmden, hdg1cs, hdg0cs) (_OSD_CTRL5_TYPE)( \
	 _CCL_PER_FMKS (OSD,CTRL5,HDREXT ,hdrext )             \
	|_CCL_PER_FMKS (OSD,CTRL5,HDDWEN ,hddwen )             \
	|_CCL_PER_FMKS (OSD,CTRL5,HDDHEN ,hddhen )             \
	|_CCL_PER_FMKS (OSD,CTRL5,HDDWMD ,hddwmd )             \
	|_CCL_PER_FMKS (OSD,CTRL5,HDPNSP ,hdpnsp )             \
	|_CCL_PER_FMKS (OSD,CTRL5,HDDWEN ,hddwen )             \
	|_CCL_PER_FMKS (OSD,CTRL5,DHCMDEN,dhcmden)             \
	|_CCL_PER_FMKS (OSD,CTRL5,DWCMDEN,dwcmden)             \
	|_CCL_PER_FMKS (OSD,CTRL5,DSCMDEN,dscmden)             \
	|_CCL_PER_FMK  (OSD,CTRL5,HDG1CS ,hdg1cs )             \
	|_CCL_PER_FMK  (OSD,CTRL5,HDG0CS ,hdg0cs )             \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  C S B M P S T    |
 * |___________________|
 *
 * CSBMPST - Character Set Bit Map Start Address Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  CSBMPST (Character Set Bit Map Start Address) -
\*-------------------------------------------------------------------------*/
#define _OSD_CSBMPST_OFFSET                 (0x0014u)
#define _OSD_CSBMPST_ADDRH(h)               OSD_ADDRH(h,CSBMPST)
#define _OSD_CSBMPST_LEN                    (32)                 /* 32-bit*/
#define _OSD_CSBMPST_TYPE                   uint32_t

#define _OSD_CSBMPST_CSBMPST_MASK           (0x1FFFFFFFu)
#define _OSD_CSBMPST_CSBMPST_SHIFT          (0)
#define  OSD_CSBMPST_CSBMPST_DEFAULT        (0x00u)
#define  OSD_CSBMPST_CSBMPST_OF(x)          _CCL_VALUEOF(_OSD_CSBMPST_TYPE,x)

#define  OSD_CSBMPST_OF(x)             _CCL_VALUEOF(_OSD_CSBMPST_TYPE,x)

#define  OSD_CSBMPST_DEFAULT (_OSD_CSBMPST_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,CSBMPST,CSBMPST)        \
	)

#define OSD_CSBMPST_RMK(csbmpst) (_OSD_CSBMPST_TYPE)( \
	 _CCL_PER_FMK  (OSD,CSBMPST,CSBMPST,csbmpst )     \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  A T T S T 1      |
 * |___________________|
 *
 * ATTST1 - Display Attribute Memory Pointer Register 1
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  ATTST (Display Attribute Memory Pointer) -
\*-------------------------------------------------------------------------*/
#define _OSD_ATTST1_OFFSET                 (0x0018u)
#define _OSD_ATTST1_ADDRH(h)               OSD_ADDRH(h,ATTST1)
#define _OSD_ATTST1_LEN                    (32)                 /* 32-bit*/
#define _OSD_ATTST1_TYPE                   uint32_t

#define _OSD_ATTST1_ATTST_MASK             (0x1FFFFFFFu)
#define _OSD_ATTST1_ATTST_SHIFT            (0)
#define  OSD_ATTST1_ATTST_DEFAULT          (0x00u)
#define  OSD_ATTST1_ATTST_OF(x)            _CCL_VALUEOF(_OSD_ATTST1_TYPE,x)

#define  OSD_ATTST1_OF(x)             _CCL_VALUEOF(_OSD_ATTST1_TYPE,x)

#define  OSD_ATTST1_DEFAULT (_OSD_ATTST1_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,ATTST1,ATTST)         \
	)

#define OSD_ATTST1_RMK(attst1) (_OSD_ATTST1_TYPE)( \
	 _CCL_PER_FMK  (OSD,ATTST1,ATTST,attst1)       \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  A T T S T 2      |
 * |___________________|
 *
 * ATTST2 - Display Attribute Memory Pointer Register 2
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  ATTST (Display Attribute Memory Pointer) -
\*-------------------------------------------------------------------------*/
#define _OSD_ATTST2_OFFSET                 (0x001Cu)
#define _OSD_ATTST2_ADDRH(h)               OSD_ADDRH(h,ATTST2)
#define _OSD_ATTST2_LEN                    (32)                 /* 32-bit*/
#define _OSD_ATTST2_TYPE                   uint32_t

#define _OSD_ATTST2_ATTST_MASK             (0x1FFFFFFFu)
#define _OSD_ATTST2_ATTST_SHIFT            (0)
#define  OSD_ATTST2_ATTST_DEFAULT          (0x00u)
#define  OSD_ATTST2_ATTST_OF(x)            _CCL_VALUEOF(_OSD_ATTST2_TYPE,x)

#define  OSD_ATTST2_OF(x)             _CCL_VALUEOF(_OSD_ATTST2_TYPE,x)

#define  OSD_ATTST2_DEFAULT (_OSD_ATTST2_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,ATTST2,ATTST)         \
	)

#define OSD_ATTST2_RMK(attst2) (_OSD_ATTST2_TYPE)( \
	 _CCL_PER_FMK  (OSD,ATTST2,ATTST,attst2)       \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  U P D A T E      |
 * |___________________|
 *
 * UPDATE - Update Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  GRU    (Global Register Update Notify) -
 * (rw)  TTP1RU (TT Page 1 Register Update Notfiy) -
\*-------------------------------------------------------------------------*/
#define _OSD_UPDATE_OFFSET                 (0x0020u)
#define _OSD_UPDATE_ADDRH(h)               OSD_ADDRH(h,UPDATE)
#define _OSD_UPDATE_LEN                    (32)                 /* 32-bit*/
#define _OSD_UPDATE_TYPE                   uint32_t

#define _OSD_UPDATE_GRU_MASK               (0x80000000u)
#define _OSD_UPDATE_GRU_SHIFT              (31)
#define  OSD_UPDATE_GRU_DEFAULT            (0x00u)
#define  OSD_UPDATE_GRU_OF(x)              _CCL_VALUEOF(_OSD_UPDATE_TYPE,x)
#define  OSD_UPDATE_GRU_NORMAL             (0x00u)
#define  OSD_UPDATE_GRU_NOTIFY             (0x01u)

#define _OSD_UPDATE_TTP1RU_MASK            (0x40000000u)
#define _OSD_UPDATE_TTP1RU_SHIFT           (30)
#define  OSD_UPDATE_TTP1RU_DEFAULT         (0x00u)
#define  OSD_UPDATE_TTP1RU_OF(x)           _CCL_VALUEOF(_OSD_UPDATE_TYPE,x)
#define  OSD_UPDATE_TTP1RU_NORMAL          (0x00u)
#define  OSD_UPDATE_TTP1RU_NOTIFY          (0x01u)

#define  OSD_UPDATE_OF(x)             _CCL_VALUEOF(_OSD_UPDATE_TYPE,x)

#define  OSD_UPDATE_DEFAULT (_OSD_UPDATE_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,UPDATE,GRU)           \
	|_CCL_PER_FDEFAULT(OSD,UPDATE,TTP1RU)        \
	)

#define OSD_UPDATE_RMK(gru,ttp1ru) (_OSD_UPDATE_TYPE)( \
	 _CCL_PER_FMK  (OSD,UPDATE,GRU,gru)                \
	|_CCL_PER_FMK  (OSD,UPDATE,TTP1RU,ttp1ru )         \
	)

#define OSD_UPDATE_RMKS(gru,ttp1ru) (_OSD_UPDATE_TYPE)( \
	 _CCL_PER_FMKS (OSD,UPDATE,GRU,gru)                 \
	|_CCL_PER_FMKS (OSD,UPDATE,TTP1RU,ttp1ru )          \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  INTERRUPT        |
 * |___________________|
 *
 * INTERRUPT - Interrupt Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  HSIDLES (HSync Idle Status) - When Idle mode enable, Hsync occurs. Write 1 to clear.
 *                 HSIDLES |   description
 *                ---------+-------------------
 *                  NOP    |   no operation     (read/write)
 *                ASSERTED |   interrupt occurs (read)
 *                 CLEAR   |   clear interrupt  (write)
 *
 * (rw)  SCROLS (CC Start scrolling) - Auto clear after scrolling complete.
 *                 SCROLS  |   description
 *                ---------+-------------------
 *                 STOP    |   Stop scrolling   (read/write)
 *                 START   |   Start scrolling  (write)
 *
 * (rw)  RURIE (Row buffer Under Run Interrupt Enable) -
 *                  RURIE  |   description
 *                ---------+-------------------
 *                   NOP   |   no operation
 *                  ENABLE |   Enable interrupt
 *
 * (rw)  RURID (Row buffer Under Run Interrupt Disable) -
 *                  RURID  |   description
 *                ---------+-------------------
 *                   NOP   |   no operation
 *                 DISABLE |   Disable interrupt
 *
 * (rw)  RURIS (Row buffer Under Run Interrupt Status) -
 *                  RURIS  |   description
 *                ---------+-------------------
 *                   NOP   |   no operation     (read/write)
 *                ASSERTED |   interrupt occurs (read)
 *                  CLEAR  |   Clear interrupt  (write)
 *
 * (rw)  VSYNCIE (VSync occur Interrupt Enable) -
 *                 VSYNCIE |   description
 *                ---------+-------------------
 *                   NOP   |   no operation
 *                  ENABLE |   Enable interrupt
 *
 * (rw)  VSYNCID (VSync occur Interrupt Disable) -
 *                 VSYNCID |   description
 *                ---------+-------------------
 *                   NOP   |   no operation
 *                 DISABLE |   Disable interrupt
 *
 * (rw)  VSYNCIS (VSync occur Interrupt Status) -
 *                 VSYNCIS |   description
 *                ---------+-------------------
 *                   NOP   |   no operation     (read/write)
 *                ASSERTED |   interrupt occurs (read)
 *                  CLEAR  |   Clear interrupt  (write)
\*-------------------------------------------------------------------------*/
#define _OSD_INTERRUPT_OFFSET                 (0x0024u)
#define _OSD_INTERRUPT_ADDRH(h)               OSD_ADDRH(h,INTERRUPT)
#define _OSD_INTERRUPT_LEN                    (32)                 /* 32-bit*/
#define _OSD_INTERRUPT_TYPE                   uint32_t

#define _OSD_INTERRUPT_HSIDLES_MASK           (0x80000000u)
#define _OSD_INTERRUPT_HSIDLES_SHIFT          (31)
#define  OSD_INTERRUPT_HSIDLES_DEFAULT        (0x00u)
#define  OSD_INTERRUPT_HSIDLES_OF(x)          _CCL_VALUEOF(_OSD_INTERRUPT_TYPE,x)
#define  OSD_INTERRUPT_HSIDLES_NOP            (0x00u)
#define  OSD_INTERRUPT_HSIDLES_ASSERTED       (0x01u)
#define  OSD_INTERRUPT_HSIDLES_CLEAR          (0x01u)

#define _OSD_INTERRUPT_SCROLS_MASK            (0x40000000u)
#define _OSD_INTERRUPT_SCROLS_SHIFT           (30)
#define  OSD_INTERRUPT_SCROLS_DEFAULT         (0x00u)
#define  OSD_INTERRUPT_SCROLS_OF(x)           _CCL_VALUEOF(_OSD_INTERRUPT_TYPE,x)
#define  OSD_INTERRUPT_SCROLS_STOP            (0x00u)
#define  OSD_INTERRUPT_SCROLS_START           (0x01u)

#define _OSD_INTERRUPT_RURIE_MASK             (0x00000020u)
#define _OSD_INTERRUPT_RURIE_SHIFT            (5)
#define  OSD_INTERRUPT_RURIE_DEFAULT          (0x00u)
#define  OSD_INTERRUPT_RURIE_OF(x)            _CCL_VALUEOF(_OSD_INTERRUPT_TYPE,x)
#define  OSD_INTERRUPT_RURIE_NOP              (0x00u)
#define  OSD_INTERRUPT_RURIE_ENABLE           (0x01u)

#define _OSD_INTERRUPT_RURID_MASK             (0x00000010u)
#define _OSD_INTERRUPT_RURID_SHIFT            (4)
#define  OSD_INTERRUPT_RURID_DEFAULT          (0x00u)
#define  OSD_INTERRUPT_RURID_OF(x)            _CCL_VALUEOF(_OSD_INTERRUPT_TYPE,x)
#define  OSD_INTERRUPT_RURID_NOP              (0x00u)
#define  OSD_INTERRUPT_RURID_DISABLE          (0x01u)

#define _OSD_INTERRUPT_RURIS_MASK             (0x00000008u)
#define _OSD_INTERRUPT_RURIS_SHIFT            (3)
#define  OSD_INTERRUPT_RURIS_DEFAULT          (0x00u)
#define  OSD_INTERRUPT_RURIS_OF(x)            _CCL_VALUEOF(_OSD_INTERRUPT_TYPE,x)
#define  OSD_INTERRUPT_RURIS_NOP              (0x00u)
#define  OSD_INTERRUPT_RURIS_ASSERTED         (0x01u)
#define  OSD_INTERRUPT_RURIS_CLEAR            (0x01u)

#define _OSD_INTERRUPT_VSYNCIE_MASK           (0x00000004u)
#define _OSD_INTERRUPT_VSYNCIE_SHIFT          (2)
#define  OSD_INTERRUPT_VSYNCIE_DEFAULT        (0x00u)
#define  OSD_INTERRUPT_VSYNCIE_OF(x)          _CCL_VALUEOF(_OSD_INTERRUPT_TYPE,x)
#define  OSD_INTERRUPT_VSYNCIE_NOP            (0x00u)
#define  OSD_INTERRUPT_VSYNCIE_ENABLE         (0x01u)

#define _OSD_INTERRUPT_VSYNCID_MASK           (0x00000002u)
#define _OSD_INTERRUPT_VSYNCID_SHIFT          (1)
#define  OSD_INTERRUPT_VSYNCID_DEFAULT        (0x00u)
#define  OSD_INTERRUPT_VSYNCID_OF(x)          _CCL_VALUEOF(_OSD_INTERRUPT_TYPE,x)
#define  OSD_INTERRUPT_VSYNCID_NOP            (0x00u)
#define  OSD_INTERRUPT_VSYNCID_DISABLE        (0x01u)

#define _OSD_INTERRUPT_VSYNCIS_MASK           (0x00000001u)
#define _OSD_INTERRUPT_VSYNCIS_SHIFT          (0)
#define  OSD_INTERRUPT_VSYNCIS_DEFAULT        (0x00u)
#define  OSD_INTERRUPT_VSYNCIS_OF(x)          _CCL_VALUEOF(_OSD_INTERRUPT_TYPE,x)
#define  OSD_INTERRUPT_VSYNCIS_NOP            (0x00u)
#define  OSD_INTERRUPT_VSYNCIS_ASSERTED       (0x01u)
#define  OSD_INTERRUPT_VSYNCIS_CLEAR          (0x01u)

#define  OSD_INTERRUPT_OF(x)             _CCL_VALUEOF(_OSD_INTERRUPT_TYPE,x)

#define  OSD_INTERRUPT_DEFAULT (_OSD_INTERRUPT_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,INTERRUPT,HSIDLES)          \
	|_CCL_PER_FDEFAULT(OSD,INTERRUPT,SCROLS)           \
	|_CCL_PER_FDEFAULT(OSD,INTERRUPT,RURIE)            \
	|_CCL_PER_FDEFAULT(OSD,INTERRUPT,RURID)            \
	|_CCL_PER_FDEFAULT(OSD,INTERRUPT,RURIS)            \
	|_CCL_PER_FDEFAULT(OSD,INTERRUPT,VSYNCIE)          \
	|_CCL_PER_FDEFAULT(OSD,INTERRUPT,VSYNCID)          \
	|_CCL_PER_FDEFAULT(OSD,INTERRUPT,VSYNCIS)          \
	)

#define OSD_INTERRUPT_RMK(hsidles,scrols,rurie,rurid,ruris,vsyncie,vsyncid,vsyncis) (_OSD_INTERRUPT_TYPE)( \
	 _CCL_PER_FMK  (OSD,INTERRUPT,HSIDLES,hsidles)         \
	|_CCL_PER_FMK  (OSD,INTERRUPT,SCROLS ,scrols )         \
	|_CCL_PER_FMK  (OSD,INTERRUPT,RURIE  ,rurie  )         \
	|_CCL_PER_FMK  (OSD,INTERRUPT,RURID  ,rurid  )         \
	|_CCL_PER_FMK  (OSD,INTERRUPT,RURIS  ,ruris  )         \
	|_CCL_PER_FMK  (OSD,INTERRUPT,VSYNCIE,vsyncie)         \
	|_CCL_PER_FMK  (OSD,INTERRUPT,VSYNCID,vsyncid)         \
	|_CCL_PER_FMK  (OSD,INTERRUPT,VSYNCIS,vsyncis)         \
	)

#define OSD_INTERRUPT_RMKS(hsidles,scrols,rurie,rurid,ruris,vsyncie,vsyncid,vsyncis) (_OSD_INTERRUPT_TYPE)( \
	 _CCL_PER_FMKS (OSD,INTERRUPT,HSIDLES,hsidles)         \
	|_CCL_PER_FMKS (OSD,INTERRUPT,SCROLS ,scrols )         \
	|_CCL_PER_FMKS (OSD,INTERRUPT,RURIE  ,rurie  )         \
	|_CCL_PER_FMKS (OSD,INTERRUPT,RURID  ,rurid  )         \
	|_CCL_PER_FMKS (OSD,INTERRUPT,RURIS  ,ruris  )         \
	|_CCL_PER_FMKS (OSD,INTERRUPT,VSYNCIE,vsyncie)         \
	|_CCL_PER_FMKS (OSD,INTERRUPT,VSYNCID,vsyncid)         \
	|_CCL_PER_FMKS (OSD,INTERRUPT,VSYNCIS,vsyncis)         \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  C R C D A T      |
 * |___________________|
 *
 * CRCDAT - CRC Data Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (r)  CRCDAT (CRC data) -
\*-------------------------------------------------------------------------*/
#define _OSD_CRCDAT_OFFSET                 (0x002Cu)
#define _OSD_CRCDAT_ADDRH(h)               OSD_ADDRH(h,CRCDAT)
#define _OSD_CRCDAT_LEN                    (32)                 /* 32-bit*/
#define _OSD_CRCDAT_TYPE                   uint32_t

#define _OSD_CRCDAT_CRCDAT_MASK            (0xFFFFFFFFu)
#define _OSD_CRCDAT_CRCDAT_SHIFT           (0)
#define  OSD_CRCDAT_CRCDAT_DEFAULT         (0x00u)
#define  OSD_CRCDAT_CRCDAT_OF(x)           _CCL_VALUEOF(_OSD_CRCDAT_TYPE,x)


#define  OSD_CRCDAT_OF(x)             _CCL_VALUEOF(_OSD_CRCDAT_TYPE,x)

#define  OSD_CRCDAT_DEFAULT (_OSD_CRCDAT_TYPE)(    \
	 _CCL_PER_FDEFAULT(OSD,CRCDAT,CRCDAT)          \
	)

#define OSD_CRCDAT_RMK(crcdat) (_OSD_CRCDAT_TYPE)( \
	 _CCL_PER_FMK  (OSD,CRCDAT,CRCDAT,crcdat)      \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  P 1 C T R L      |
 * |___________________|
 *
 * P1CTRL - TT Display Page 1 Parameter Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  BOX    (box or windows) -
 *                   BOX   |   description
 *                ---------+--------------------------------------
 *                  WINDOW |  window area is defined in the page.
 *                   BOX   |  box area is defined in the page.
 *
 * (rw)  SPO    (Side Panel ON) -
 *                   SPO   |   description
 *                ---------+--------------------------------------
 *                   OFF   |  Hide side-panel
 *                   ON    |  Display side-panel.
 *
 * (rw)  LSPNUM (Column number of left side-panel) -
 * (rw)  SCT    (CLUT of page 1 Screen color area) -
 * (rw)  SCE    (CLUT entry of page 1 Screen color area) -
 *
 * (rw)  HSR    (Horizontal Scaling Rate) -
 *                   HSR   |   description
 *                ---------+--------------------------------------
 *                    1    |
 *                 1OVER2  |
 *                 7OVER5  |
 *                 7OVER10 |
 *
 * (rw)  RHDE   (Rolling Header Display Enable) -
 *                  RHDE   |   description
 *                ---------+--------------------------------------
 *                 DISABLE | Don't display rolling header
 *                 ENABLE  | Display rolling header
 *
 * (rw)  TCDE   (Time Code Display Enable) -
 *                  RHDE   |   description
 *                ---------+--------------------------------------
 *                 DISABLE | Don't display time code
 *                 ENABLE  | Display time code
 *
 * (rw)  DE     (P1 Display Enable) -
 *                  RHDE   |   description
 *                ---------+--------------------------------------
 *                 DISABLE | Don't display page-1 (transparent)
 *                 ENABLE  | Display page-1
\*-------------------------------------------------------------------------*/
#define _OSD_P1CTRL_OFFSET                 (0x0030u)
#define _OSD_P1CTRL_ADDRH(h)               OSD_ADDRH(h,P1CTRL)
#define _OSD_P1CTRL_LEN                    (32)                 /* 32-bit*/
#define _OSD_P1CTRL_TYPE                   uint32_t

#define _OSD_P1CTRL_BOX_MASK               (0x80000000u)
#define _OSD_P1CTRL_BOX_SHIFT              (31)
#define  OSD_P1CTRL_BOX_DEFAULT            (0x00u)
#define  OSD_P1CTRL_BOX_OF(x)              _CCL_VALUEOF(_OSD_P1CTRL_TYPE,x)
#define  OSD_P1CTRL_BOX_WINDOW             (0x00u)
#define  OSD_P1CTRL_BOX_BOX                (0x01u)

#define _OSD_P1CTRL_SPO_MASK               (0x40000000u)
#define _OSD_P1CTRL_SPO_SHIFT              (30)
#define  OSD_P1CTRL_SPO_DEFAULT            (0x00u)
#define  OSD_P1CTRL_SPO_OF(x)              _CCL_VALUEOF(_OSD_P1CTRL_TYPE,x)
#define  OSD_P1CTRL_SPO_OFF                (0x00u)
#define  OSD_P1CTRL_SPO_ON                 (0x01u)

#define _OSD_P1CTRL_LSPNUM_MASK            (0x0F000000u)
#define _OSD_P1CTRL_LSPNUM_SHIFT           (24)
#define  OSD_P1CTRL_LSPNUM_DEFAULT         (0x00u)
#define  OSD_P1CTRL_LSPNUM_OF(x)           _CCL_VALUEOF(_OSD_P1CTRL_TYPE,x)

#define _OSD_P1CTRL_SCT_MASK               (0x00001800u)
#define _OSD_P1CTRL_SCT_SHIFT              (11)
#define  OSD_P1CTRL_SCT_DEFAULT            (0x00u)
#define  OSD_P1CTRL_SCT_OF(x)              _CCL_VALUEOF(_OSD_P1CTRL_TYPE,x)

#define _OSD_P1CTRL_SCE_MASK               (0x00000700u)
#define _OSD_P1CTRL_SCE_SHIFT              (8)
#define  OSD_P1CTRL_SCE_DEFAULT            (0x00u)
#define  OSD_P1CTRL_SCE_OF(x)              _CCL_VALUEOF(_OSD_P1CTRL_TYPE,x)

#define _OSD_P1CTRL_HSR_MASK               (0x00000030u)
#define _OSD_P1CTRL_HSR_SHIFT              (4)
#define  OSD_P1CTRL_HSR_DEFAULT            (0x00u)
#define  OSD_P1CTRL_HSR_OF(x)              _CCL_VALUEOF(_OSD_P1CTRL_TYPE,x)
#define  OSD_P1CTRL_HSR_1                  (0x00u)
#define  OSD_P1CTRL_HSR_1OVER2             (0x01u)
#define  OSD_P1CTRL_HSR_7OVER5             (0x02u)
#define  OSD_P1CTRL_HSR_7OVER10            (0x03u)

#define _OSD_P1CTRL_RHDE_MASK              (0x00000008u)
#define _OSD_P1CTRL_RHDE_SHIFT             (3)
#define  OSD_P1CTRL_RHDE_DEFAULT           (0x00u)
#define  OSD_P1CTRL_RHDE_OF(x)             _CCL_VALUEOF(_OSD_P1CTRL_TYPE,x)
#define  OSD_P1CTRL_RHDE_DISABLE           (0x00u)
#define  OSD_P1CTRL_RHDE_ENABLE            (0x01u)

#define _OSD_P1CTRL_TCDE_MASK              (0x00000004u)
#define _OSD_P1CTRL_TCDE_SHIFT             (2)
#define  OSD_P1CTRL_TCDE_DEFAULT           (0x00u)
#define  OSD_P1CTRL_TCDE_OF(x)             _CCL_VALUEOF(_OSD_P1CTRL_TYPE,x)
#define  OSD_P1CTRL_TCDE_DISABLE           (0x00u)
#define  OSD_P1CTRL_TCDE_ENABLE            (0x01u)

#define _OSD_P1CTRL_DE_MASK                (0x00000001u)
#define _OSD_P1CTRL_DE_SHIFT               (0)
#define  OSD_P1CTRL_DE_DEFAULT             (0x00u)
#define  OSD_P1CTRL_DE_OF(x)               _CCL_VALUEOF(_OSD_P1CTRL_TYPE,x)
#define  OSD_P1CTRL_DE_DISABLE             (0x00u)
#define  OSD_P1CTRL_DE_ENABLE              (0x01u)


#define  OSD_P1CTRL_OF(x)             _CCL_VALUEOF(_OSD_P1CTRL_TYPE,x)

#define  OSD_P1CTRL_DEFAULT (_OSD_P1CTRL_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,P1CTRL,BOX)           \
	|_CCL_PER_FDEFAULT(OSD,P1CTRL,SPO)           \
	|_CCL_PER_FDEFAULT(OSD,P1CTRL,LSPNUM)        \
	|_CCL_PER_FDEFAULT(OSD,P1CTRL,SCT)           \
	|_CCL_PER_FDEFAULT(OSD,P1CTRL,SCE)           \
	|_CCL_PER_FDEFAULT(OSD,P1CTRL,HSR)           \
	|_CCL_PER_FDEFAULT(OSD,P1CTRL,RHDE)          \
	|_CCL_PER_FDEFAULT(OSD,P1CTRL,TCDE)          \
	|_CCL_PER_FDEFAULT(OSD,P1CTRL,DE)            \
	)

#define OSD_P1CTRL_RMK(box,spo,lspnum,sct,sce,hsr,rhde,tcde,de) (_OSD_P1CTRL_TYPE)( \
	 _CCL_PER_FMK  (OSD,P1CTRL,BOX,box)                \
	|_CCL_PER_FMK  (OSD,P1CTRL,SPO,spo )               \
	|_CCL_PER_FMK  (OSD,P1CTRL,LSPNUM,lspnum )         \
	|_CCL_PER_FMK  (OSD,P1CTRL,SCT,sct )               \
	|_CCL_PER_FMK  (OSD,P1CTRL,SCE,sce )               \
	|_CCL_PER_FMK  (OSD,P1CTRL,HSR,hsr )               \
	|_CCL_PER_FMK  (OSD,P1CTRL,SPO,spo )               \
	|_CCL_PER_FMK  (OSD,P1CTRL,RHDE,rhde )             \
	|_CCL_PER_FMK  (OSD,P1CTRL,TCDE,tcde )             \
	|_CCL_PER_FMK  (OSD,P1CTRL,DE,de )                 \
	)

#define OSD_P1CTRL_RMKS(box,spo,lspnum,sct,sce,hsr,rhde,tcde,de) (_OSD_P1CTRL_TYPE)( \
	 _CCL_PER_FMKS (OSD,P1CTRL,BOX,box)                 \
	|_CCL_PER_FMKS (OSD,P1CTRL,SPO,spo )                \
	|_CCL_PER_FMK  (OSD,P1CTRL,LSPNUM,lspnum )          \
	|_CCL_PER_FMK  (OSD,P1CTRL,SCT,sct )                \
	|_CCL_PER_FMK  (OSD,P1CTRL,SCE,sce )                \
	|_CCL_PER_FMKS (OSD,P1CTRL,HSR,hsr )                \
	|_CCL_PER_FMKS (OSD,P1CTRL,SPO,spo )                \
	|_CCL_PER_FMKS (OSD,P1CTRL,RHDE,rhde )              \
	|_CCL_PER_FMKS (OSD,P1CTRL,TCDE,tcde )              \
	|_CCL_PER_FMKS (OSD,P1CTRL,DE,de )                  \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  P 1   P A R A 1  |
 * |___________________|
 *
 * P1_PARA1 - OSD TT display page 1 parameter register 1
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  R0DC   (Default row color of Row-0)
 * (rw)  R1DC   (Default row color of Row-1)
 * (rw)  R2DC   (Default row color of Row-2)
 * (rw)  R3DC   (Default row color of Row-3)
\*-------------------------------------------------------------------------*/
#define _OSD_P1_PARA1_OFFSET                 (0x0034u)
#define _OSD_P1_PARA1_ADDRH(h)               OSD_ADDRH(h,P1_PARA1)
#define _OSD_P1_PARA1_LEN                    (32)                 /* 32-bit*/
#define _OSD_P1_PARA1_TYPE                   uint32_t

#define _OSD_P1_PARA1_R0DC_MASK              (0x1F000000u)
#define _OSD_P1_PARA1_R0DC_SHIFT             (24)
#define  OSD_P1_PARA1_R0DC_DEFAULT           (0x00u)
#define  OSD_P1_PARA1_R0DC_OF(x)             _CCL_VALUEOF(_OSD_P1_PARA1_TYPE,x)

#define _OSD_P1_PARA1_R1DC_MASK              (0x001F0000u)
#define _OSD_P1_PARA1_R1DC_SHIFT             (16)
#define  OSD_P1_PARA1_R1DC_DEFAULT           (0x00u)
#define  OSD_P1_PARA1_R1DC_OF(x)             _CCL_VALUEOF(_OSD_P1_PARA1_TYPE,x)

#define _OSD_P1_PARA1_R2DC_MASK              (0x00001F00u)
#define _OSD_P1_PARA1_R2DC_SHIFT             (8)
#define  OSD_P1_PARA1_R2DC_DEFAULT           (0x00u)
#define  OSD_P1_PARA1_R2DC_OF(x)             _CCL_VALUEOF(_OSD_P1_PARA1_TYPE,x)

#define _OSD_P1_PARA1_R3DC_MASK              (0x0000001Fu)
#define _OSD_P1_PARA1_R3DC_SHIFT             (0)
#define  OSD_P1_PARA1_R3DC_DEFAULT           (0x00u)
#define  OSD_P1_PARA1_R3DC_OF(x)             _CCL_VALUEOF(_OSD_P1_PARA1_TYPE,x)


#define  OSD_P1_PARA1_OF(x)             _CCL_VALUEOF(_OSD_P1_PARA1_TYPE,x)

#define  OSD_P1_PARA1_DEFAULT (_OSD_P1_PARA1_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,P1_PARA1,R0DC)            \
	|_CCL_PER_FDEFAULT(OSD,P1_PARA1,R1DC)            \
	|_CCL_PER_FDEFAULT(OSD,P1_PARA1,R2DC)            \
	|_CCL_PER_FDEFAULT(OSD,P1_PARA1,R3DC)            \
	)

#define OSD_P1_PARA1_RMK(r0dc, r1dc, r2dc, r3dc) (_OSD_P1_PARA1_TYPE)( \
	 _CCL_PER_FMK  (OSD,P1_PARA1,R0DC,r0dc)                            \
	|_CCL_PER_FMK  (OSD,P1_PARA1,R1DC,r1dc)                            \
	|_CCL_PER_FMK  (OSD,P1_PARA1,R2DC,r2dc)                            \
	|_CCL_PER_FMK  (OSD,P1_PARA1,R3DC,r3dc)                            \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  P 1   P A R A 2  |
 * |___________________|
 *
 * P1_PARA2 - OSD TT display page 1 parameter register 2
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  R4DC   (Default row color of Row-4)
 * (rw)  R5DC   (Default row color of Row-5)
 * (rw)  R6DC   (Default row color of Row-6)
 * (rw)  R7DC   (Default row color of Row-7)
\*-------------------------------------------------------------------------*/
#define _OSD_P1_PARA2_OFFSET                 (0x0038u)
#define _OSD_P1_PARA2_ADDRH(h)               OSD_ADDRH(h,P1_PARA2)
#define _OSD_P1_PARA2_LEN                    (32)                 /* 32-bit*/
#define _OSD_P1_PARA2_TYPE                   uint32_t

#define _OSD_P1_PARA2_R4DC_MASK              (0x1F000000u)
#define _OSD_P1_PARA2_R4DC_SHIFT             (24)
#define  OSD_P1_PARA2_R4DC_DEFAULT           (0x00u)
#define  OSD_P1_PARA2_R4DC_OF(x)             _CCL_VALUEOF(_OSD_P1_PARA2_TYPE,x)

#define _OSD_P1_PARA2_R5DC_MASK              (0x001F0000u)
#define _OSD_P1_PARA2_R5DC_SHIFT             (16)
#define  OSD_P1_PARA2_R5DC_DEFAULT           (0x00u)
#define  OSD_P1_PARA2_R5DC_OF(x)             _CCL_VALUEOF(_OSD_P1_PARA2_TYPE,x)

#define _OSD_P1_PARA2_R6DC_MASK              (0x00001F00u)
#define _OSD_P1_PARA2_R6DC_SHIFT             (8)
#define  OSD_P1_PARA2_R6DC_DEFAULT           (0x00u)
#define  OSD_P1_PARA2_R6DC_OF(x)             _CCL_VALUEOF(_OSD_P1_PARA2_TYPE,x)

#define _OSD_P1_PARA2_R7DC_MASK              (0x0000001Fu)
#define _OSD_P1_PARA2_R7DC_SHIFT             (0)
#define  OSD_P1_PARA2_R7DC_DEFAULT           (0x00u)
#define  OSD_P1_PARA2_R7DC_OF(x)             _CCL_VALUEOF(_OSD_P1_PARA2_TYPE,x)


#define  OSD_P1_PARA2_OF(x)             _CCL_VALUEOF(_OSD_P1_PARA2_TYPE,x)

#define  OSD_P1_PARA2_DEFAULT (_OSD_P1_PARA2_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,P1_PARA2,R4DC)            \
	|_CCL_PER_FDEFAULT(OSD,P1_PARA2,R5DC)            \
	|_CCL_PER_FDEFAULT(OSD,P1_PARA2,R6DC)            \
	|_CCL_PER_FDEFAULT(OSD,P1_PARA2,R7DC)            \
	)

#define OSD_P1_PARA2_RMK(r4dc, r5dc, r6dc, r7dc) (_OSD_P1_PARA2_TYPE)( \
	 _CCL_PER_FMK  (OSD,P1_PARA2,R4DC,r4dc)                            \
	|_CCL_PER_FMK  (OSD,P1_PARA2,R5DC,r5dc)                            \
	|_CCL_PER_FMK  (OSD,P1_PARA2,R6DC,r6dc)                            \
	|_CCL_PER_FMK  (OSD,P1_PARA2,R7DC,r7dc)                            \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  P 1   P A R A 3  |
 * |___________________|
 *
 * P1_PARA3 - OSD TT display page 1 parameter register 3
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  R8DC    (Default row color of Row-8)
 * (rw)  R9DC    (Default row color of Row-9)
 * (rw)  R10DC   (Default row color of Row-10)
 * (rw)  R11DC   (Default row color of Row-11)
\*-------------------------------------------------------------------------*/
#define _OSD_P1_PARA3_OFFSET                 (0x003Cu)
#define _OSD_P1_PARA3_ADDRH(h)               OSD_ADDRH(h,P1_PARA3)
#define _OSD_P1_PARA3_LEN                    (32)                 /* 32-bit*/
#define _OSD_P1_PARA3_TYPE                   uint32_t

#define _OSD_P1_PARA3_R8DC_MASK              (0x1F000000u)
#define _OSD_P1_PARA3_R8DC_SHIFT             (24)
#define  OSD_P1_PARA3_R8DC_DEFAULT           (0x00u)
#define  OSD_P1_PARA3_R8DC_OF(x)             _CCL_VALUEOF(_OSD_P1_PARA3_TYPE,x)

#define _OSD_P1_PARA3_R9DC_MASK              (0x001F0000u)
#define _OSD_P1_PARA3_R9DC_SHIFT             (16)
#define  OSD_P1_PARA3_R9DC_DEFAULT           (0x00u)
#define  OSD_P1_PARA3_R9DC_OF(x)             _CCL_VALUEOF(_OSD_P1_PARA3_TYPE,x)

#define _OSD_P1_PARA3_R10DC_MASK             (0x00001F00u)
#define _OSD_P1_PARA3_R10DC_SHIFT            (8)
#define  OSD_P1_PARA3_R10DC_DEFAULT          (0x00u)
#define  OSD_P1_PARA3_R10DC_OF(x)            _CCL_VALUEOF(_OSD_P1_PARA3_TYPE,x)

#define _OSD_P1_PARA3_R11DC_MASK             (0x0000001Fu)
#define _OSD_P1_PARA3_R11DC_SHIFT            (0)
#define  OSD_P1_PARA3_R11DC_DEFAULT          (0x00u)
#define  OSD_P1_PARA3_R11DC_OF(x)            _CCL_VALUEOF(_OSD_P1_PARA3_TYPE,x)


#define  OSD_P1_PARA3_OF(x)             _CCL_VALUEOF(_OSD_P1_PARA3_TYPE,x)

#define  OSD_P1_PARA3_DEFAULT (_OSD_P1_PARA3_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,P1_PARA3,R8DC )           \
	|_CCL_PER_FDEFAULT(OSD,P1_PARA3,R9DC )           \
	|_CCL_PER_FDEFAULT(OSD,P1_PARA3,R10DC)           \
	|_CCL_PER_FDEFAULT(OSD,P1_PARA3,R11DC)           \
	)

#define OSD_P1_PARA3_RMK(r8dc, r9dc, r10dc, r11dc) (_OSD_P1_PARA3_TYPE)( \
	 _CCL_PER_FMK  (OSD,P1_PARA3,R8DC ,r8dc )                            \
	|_CCL_PER_FMK  (OSD,P1_PARA3,R9DC ,r9dc )                            \
	|_CCL_PER_FMK  (OSD,P1_PARA3,R10DC,r10dc)                            \
	|_CCL_PER_FMK  (OSD,P1_PARA3,R11DC,r11dc)                            \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  P 1   P A R A 4  |
 * |___________________|
 *
 * P1_PARA4 - OSD TT display page 1 parameter register 4
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  R12DC    (Default row color of Row-12)
 * (rw)  R13DC    (Default row color of Row-13)
 * (rw)  R14DC    (Default row color of Row-14)
 * (rw)  R15DC    (Default row color of Row-15)
\*-------------------------------------------------------------------------*/
#define _OSD_P1_PARA4_OFFSET                 (0x0040u)
#define _OSD_P1_PARA4_ADDRH(h)               OSD_ADDRH(h,P1_PARA4)
#define _OSD_P1_PARA4_LEN                    (32)                 /* 32-bit*/
#define _OSD_P1_PARA4_TYPE                   uint32_t

#define _OSD_P1_PARA4_R12DC_MASK             (0x1F000000u)
#define _OSD_P1_PARA4_R12DC_SHIFT            (24)
#define  OSD_P1_PARA4_R12DC_DEFAULT          (0x00u)
#define  OSD_P1_PARA4_R12DC_OF(x)            _CCL_VALUEOF(_OSD_P1_PARA4_TYPE,x)

#define _OSD_P1_PARA4_R13DC_MASK             (0x001F0000u)
#define _OSD_P1_PARA4_R13DC_SHIFT            (16)
#define  OSD_P1_PARA4_R13DC_DEFAULT          (0x00u)
#define  OSD_P1_PARA4_R13DC_OF(x)            _CCL_VALUEOF(_OSD_P1_PARA4_TYPE,x)

#define _OSD_P1_PARA4_R14DC_MASK             (0x00001F00u)
#define _OSD_P1_PARA4_R14DC_SHIFT            (8)
#define  OSD_P1_PARA4_R14DC_DEFAULT          (0x00u)
#define  OSD_P1_PARA4_R14DC_OF(x)            _CCL_VALUEOF(_OSD_P1_PARA4_TYPE,x)

#define _OSD_P1_PARA4_R15DC_MASK             (0x0000001Fu)
#define _OSD_P1_PARA4_R15DC_SHIFT            (0)
#define  OSD_P1_PARA4_R15DC_DEFAULT          (0x00u)
#define  OSD_P1_PARA4_R15DC_OF(x)            _CCL_VALUEOF(_OSD_P1_PARA4_TYPE,x)


#define  OSD_P1_PARA4_OF(x)             _CCL_VALUEOF(_OSD_P1_PARA4_TYPE,x)

#define  OSD_P1_PARA4_DEFAULT (_OSD_P1_PARA4_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,P1_PARA4,R12DC)           \
	|_CCL_PER_FDEFAULT(OSD,P1_PARA4,R13DC)           \
	|_CCL_PER_FDEFAULT(OSD,P1_PARA4,R14DC)           \
	|_CCL_PER_FDEFAULT(OSD,P1_PARA4,R15DC)           \
	)

#define OSD_P1_PARA4_RMK(r12dc, r13dc, r14dc, r15dc) (_OSD_P1_PARA4_TYPE)( \
	 _CCL_PER_FMK  (OSD,P1_PARA4,R12DC,r12dc)                              \
	|_CCL_PER_FMK  (OSD,P1_PARA4,R13DC,r13dc)                              \
	|_CCL_PER_FMK  (OSD,P1_PARA4,R14DC,r14dc)                              \
	|_CCL_PER_FMK  (OSD,P1_PARA4,R15DC,r15dc)                              \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  P 1   P A R A 5  |
 * |___________________|
 *
 * P1_PARA5 - OSD TT display page 1 parameter register 5
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  R16DC    (Default row color of Row-16)
 * (rw)  R17DC    (Default row color of Row-17)
 * (rw)  R18DC    (Default row color of Row-18)
 * (rw)  R19DC    (Default row color of Row-19)
\*-------------------------------------------------------------------------*/
#define _OSD_P1_PARA5_OFFSET                 (0x0044u)
#define _OSD_P1_PARA5_ADDRH(h)               OSD_ADDRH(h,P1_PARA5)
#define _OSD_P1_PARA5_LEN                    (32)                 /* 32-bit*/
#define _OSD_P1_PARA5_TYPE                   uint32_t

#define _OSD_P1_PARA5_R16DC_MASK             (0x1F000000u)
#define _OSD_P1_PARA5_R16DC_SHIFT            (24)
#define  OSD_P1_PARA5_R16DC_DEFAULT          (0x00u)
#define  OSD_P1_PARA5_R16DC_OF(x)            _CCL_VALUEOF(_OSD_P1_PARA5_TYPE,x)

#define _OSD_P1_PARA5_R17DC_MASK             (0x001F0000u)
#define _OSD_P1_PARA5_R17DC_SHIFT            (16)
#define  OSD_P1_PARA5_R17DC_DEFAULT          (0x00u)
#define  OSD_P1_PARA5_R17DC_OF(x)            _CCL_VALUEOF(_OSD_P1_PARA5_TYPE,x)

#define _OSD_P1_PARA5_R18DC_MASK             (0x00001F00u)
#define _OSD_P1_PARA5_R18DC_SHIFT            (8)
#define  OSD_P1_PARA5_R18DC_DEFAULT          (0x00u)
#define  OSD_P1_PARA5_R18DC_OF(x)            _CCL_VALUEOF(_OSD_P1_PARA5_TYPE,x)

#define _OSD_P1_PARA5_R19DC_MASK             (0x0000001Fu)
#define _OSD_P1_PARA5_R19DC_SHIFT            (0)
#define  OSD_P1_PARA5_R19DC_DEFAULT          (0x00u)
#define  OSD_P1_PARA5_R19DC_OF(x)            _CCL_VALUEOF(_OSD_P1_PARA5_TYPE,x)


#define  OSD_P1_PARA5_OF(x)             _CCL_VALUEOF(_OSD_P1_PARA5_TYPE,x)

#define  OSD_P1_PARA5_DEFAULT (_OSD_P1_PARA5_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,P1_PARA5,R16DC)           \
	|_CCL_PER_FDEFAULT(OSD,P1_PARA5,R17DC)           \
	|_CCL_PER_FDEFAULT(OSD,P1_PARA5,R18DC)           \
	|_CCL_PER_FDEFAULT(OSD,P1_PARA5,R19DC)           \
	)

#define OSD_P1_PARA5_RMK(r16dc, r17dc, r18dc, r19dc) (_OSD_P1_PARA5_TYPE)( \
	 _CCL_PER_FMK  (OSD,P1_PARA5,R16DC,r16dc)                              \
	|_CCL_PER_FMK  (OSD,P1_PARA5,R17DC,r17dc)                              \
	|_CCL_PER_FMK  (OSD,P1_PARA5,R18DC,r18dc)                              \
	|_CCL_PER_FMK  (OSD,P1_PARA5,R19DC,r19dc)                              \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  P 1   P A R A 6  |
 * |___________________|
 *
 * P1_PARA6 - OSD TT display page 1 parameter register 6
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  R20DC    (Default row color of Row-20)
 * (rw)  R21DC    (Default row color of Row-21)
 * (rw)  R22DC    (Default row color of Row-22)
 * (rw)  R23DC    (Default row color of Row-23)
\*-------------------------------------------------------------------------*/
#define _OSD_P1_PARA6_OFFSET                 (0x0048u)
#define _OSD_P1_PARA6_ADDRH(h)               OSD_ADDRH(h,P1_PARA6)
#define _OSD_P1_PARA6_LEN                    (32)                 /* 32-bit*/
#define _OSD_P1_PARA6_TYPE                   uint32_t

#define _OSD_P1_PARA6_R20DC_MASK             (0x1F000000u)
#define _OSD_P1_PARA6_R20DC_SHIFT            (24)
#define  OSD_P1_PARA6_R20DC_DEFAULT          (0x00u)
#define  OSD_P1_PARA6_R20DC_OF(x)            _CCL_VALUEOF(_OSD_P1_PARA6_TYPE,x)

#define _OSD_P1_PARA6_R21DC_MASK             (0x001F0000u)
#define _OSD_P1_PARA6_R21DC_SHIFT            (16)
#define  OSD_P1_PARA6_R21DC_DEFAULT          (0x00u)
#define  OSD_P1_PARA6_R21DC_OF(x)            _CCL_VALUEOF(_OSD_P1_PARA6_TYPE,x)

#define _OSD_P1_PARA6_R22DC_MASK             (0x00001F00u)
#define _OSD_P1_PARA6_R22DC_SHIFT            (8)
#define  OSD_P1_PARA6_R22DC_DEFAULT          (0x00u)
#define  OSD_P1_PARA6_R22DC_OF(x)            _CCL_VALUEOF(_OSD_P1_PARA6_TYPE,x)

#define _OSD_P1_PARA6_R23DC_MASK             (0x0000001Fu)
#define _OSD_P1_PARA6_R23DC_SHIFT            (0)
#define  OSD_P1_PARA6_R23DC_DEFAULT          (0x00u)
#define  OSD_P1_PARA6_R23DC_OF(x)            _CCL_VALUEOF(_OSD_P1_PARA6_TYPE,x)


#define  OSD_P1_PARA6_OF(x)             _CCL_VALUEOF(_OSD_P1_PARA6_TYPE,x)

#define  OSD_P1_PARA6_DEFAULT (_OSD_P1_PARA6_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,P1_PARA6,R20DC)           \
	|_CCL_PER_FDEFAULT(OSD,P1_PARA6,R21DC)           \
	|_CCL_PER_FDEFAULT(OSD,P1_PARA6,R22DC)           \
	|_CCL_PER_FDEFAULT(OSD,P1_PARA6,R23DC)           \
	)

#define OSD_P1_PARA6_RMK(r20dc, r21dc, r22dc, r23dc) (_OSD_P1_PARA6_TYPE)( \
	 _CCL_PER_FMK  (OSD,P1_PARA6,R20DC,r20dc)                              \
	|_CCL_PER_FMK  (OSD,P1_PARA6,R21DC,r21dc)                              \
	|_CCL_PER_FMK  (OSD,P1_PARA6,R22DC,r22dc)                              \
	|_CCL_PER_FMK  (OSD,P1_PARA6,R23DC,r23dc)                              \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  P 1   P A R A 7  |
 * |___________________|
 *
 * P1_PARA7 - OSD TT display page 1 parameter register 7
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  R24DC    (Default row color of Row-24)
 * (rw)  R25DC    (Default row color of Row-25)
\*-------------------------------------------------------------------------*/
#define _OSD_P1_PARA7_OFFSET                 (0x004Cu)
#define _OSD_P1_PARA7_ADDRH(h)               OSD_ADDRH(h,P1_PARA7)
#define _OSD_P1_PARA7_LEN                    (32)                 /* 32-bit*/
#define _OSD_P1_PARA7_TYPE                   uint32_t

#define _OSD_P1_PARA7_R24DC_MASK             (0x1F000000u)
#define _OSD_P1_PARA7_R24DC_SHIFT            (24)
#define  OSD_P1_PARA7_R24DC_DEFAULT          (0x00u)
#define  OSD_P1_PARA7_R24DC_OF(x)            _CCL_VALUEOF(_OSD_P1_PARA7_TYPE,x)

#define _OSD_P1_PARA7_R25DC_MASK             (0x001F0000u)
#define _OSD_P1_PARA7_R25DC_SHIFT            (16)
#define  OSD_P1_PARA7_R25DC_DEFAULT          (0x00u)
#define  OSD_P1_PARA7_R25DC_OF(x)            _CCL_VALUEOF(_OSD_P1_PARA7_TYPE,x)



#define  OSD_P1_PARA7_OF(x)             _CCL_VALUEOF(_OSD_P1_PARA7_TYPE,x)

#define  OSD_P1_PARA7_DEFAULT (_OSD_P1_PARA7_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,P1_PARA7,R24DC)           \
	|_CCL_PER_FDEFAULT(OSD,P1_PARA7,R25DC)           \
	)

#define OSD_P1_PARA7_RMK(r24dc, r25dc) (_OSD_P1_PARA7_TYPE)( \
	 _CCL_PER_FMK  (OSD,P1_PARA7,R24DC,r24dc)                \
	|_CCL_PER_FMK  (OSD,P1_PARA7,R25DC,r25dc)                \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  P1PNUM_CTRL      |
 * |___________________|
 *
 * P1PNUM_CTRL - Display Page-1 Page-Number Control Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  FGC    (Forward Ground Color) -
 * (rw)  BGC    (Back Ground Color) -
 * (rw)  CS     (Character set Selection code) -
 * (rw)  DE     (Display Enable) -
 *                   DE    |   description
 *                ---------+--------------------------------------
 *                 DISABLE | Don't display
 *                 ENABLE  | Display
\*-------------------------------------------------------------------------*/
#define _OSD_P1PNUM_CTRL_OFFSET                 (0x0050u)
#define _OSD_P1PNUM_CTRL_ADDRH(h)               OSD_ADDRH(h,P1PNUM_CTRL)
#define _OSD_P1PNUM_CTRL_LEN                    (32)                 /* 32-bit*/
#define _OSD_P1PNUM_CTRL_TYPE                   uint32_t

#define _OSD_P1PNUM_CTRL_FGC_MASK               (0x1F000000u)
#define _OSD_P1PNUM_CTRL_FGC_SHIFT              (24)
#define  OSD_P1PNUM_CTRL_FGC_DEFAULT            (0x00u)
#define  OSD_P1PNUM_CTRL_FGC_OF(x)              _CCL_VALUEOF(_OSD_P1PNUM_CTRL_TYPE,x)

#define _OSD_P1PNUM_CTRL_BGC_MASK               (0x001F0000u)
#define _OSD_P1PNUM_CTRL_BGC_SHIFT              (16)
#define  OSD_P1PNUM_CTRL_BGC_DEFAULT            (0x00u)
#define  OSD_P1PNUM_CTRL_BGC_OF(x)              _CCL_VALUEOF(_OSD_P1PNUM_CTRL_TYPE,x)

#define _OSD_P1PNUM_CTRL_CS_MASK                (0x00001F00u)
#define _OSD_P1PNUM_CTRL_CS_SHIFT               (8)
#define  OSD_P1PNUM_CTRL_CS_DEFAULT             (0x00u)
#define  OSD_P1PNUM_CTRL_CS_OF(x)               _CCL_VALUEOF(_OSD_P1PNUM_CTRL_TYPE,x)

#define _OSD_P1PNUM_CTRL_DE_MASK                (0x00000001u)
#define _OSD_P1PNUM_CTRL_DE_SHIFT               (0)
#define  OSD_P1PNUM_CTRL_DE_DEFAULT             (0x00u)
#define  OSD_P1PNUM_CTRL_DE_OF(x)               _CCL_VALUEOF(_OSD_P1PNUM_CTRL_TYPE,x)
#define  OSD_P1PNUM_CTRL_DE_DISABLE             (0x00u)
#define  OSD_P1PNUM_CTRL_DE_ENABLE              (0x01u)

#define  OSD_P1PNUM_CTRL_OF(x)             _CCL_VALUEOF(_OSD_P1PNUM_CTRL_TYPE,x)

#define  OSD_P1PNUM_CTRL_DEFAULT (_OSD_P1PNUM_CTRL_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,P1PNUM_CTRL,FGC)                \
	|_CCL_PER_FDEFAULT(OSD,P1PNUM_CTRL,BGC)                \
	|_CCL_PER_FDEFAULT(OSD,P1PNUM_CTRL,CS )                \
	|_CCL_PER_FDEFAULT(OSD,P1PNUM_CTRL,DE )                \
	)

#define OSD_P1PNUM_CTRL_RMK(fgc,bgc,cs,de) (_OSD_P1PNUM_CTRL_TYPE)( \
	 _CCL_PER_FMK  (OSD,P1PNUM_CTRL,FGC,fgc )               \
	|_CCL_PER_FMK  (OSD,P1PNUM_CTRL,BGC,bgc )               \
	|_CCL_PER_FMK  (OSD,P1PNUM_CTRL,CS ,cs  )               \
	|_CCL_PER_FMK  (OSD,P1PNUM_CTRL,DE ,de  )               \
	)
#define OSD_P1PNUM_CTRL_RMKS(fgc,bgc,cs,de) (_OSD_P1PNUM_CTRL_TYPE)( \
	 _CCL_PER_FMK  (OSD,P1PNUM_CTRL,FGC,fgc )               \
	|_CCL_PER_FMK  (OSD,P1PNUM_CTRL,BGC,bgc )               \
	|_CCL_PER_FMK  (OSD,P1PNUM_CTRL,CS ,cs  )               \
	|_CCL_PER_FMKS (OSD,P1PNUM_CTRL,DE ,de  )               \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  P1PNUM_CINDEX1   |
 * |___________________|
 *
 * P1PNUM_CINDEX1 - Display Page-1 Page-Number Character Index Register 1
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  C0   (Row-0 Column-0) -
 * (rw)  C1   (Row-0 Column-1) -
 * (rw)  C2   (Row-0 Column-2) -
 * (rw)  C3   (Row-0 Column-3) -
\*-------------------------------------------------------------------------*/
#define _OSD_P1PNUM_CINDEX1_OFFSET                 (0x0054u)
#define _OSD_P1PNUM_CINDEX1_ADDRH(h)               OSD_ADDRH(h,P1PNUM_CINDEX1)
#define _OSD_P1PNUM_CINDEX1_LEN                    (32)                 /* 32-bit*/
#define _OSD_P1PNUM_CINDEX1_TYPE                   uint32_t

#define _OSD_P1PNUM_CINDEX1_C0_MASK                (0x7F000000u)
#define _OSD_P1PNUM_CINDEX1_C0_SHIFT               (24)
#define  OSD_P1PNUM_CINDEX1_C0_DEFAULT             (0x00u)
#define  OSD_P1PNUM_CINDEX1_C0_OF(x)               _CCL_VALUEOF(_OSD_P1PNUM_CINDEX1_TYPE,x)

#define _OSD_P1PNUM_CINDEX1_C1_MASK                (0x007F0000u)
#define _OSD_P1PNUM_CINDEX1_C1_SHIFT               (16)
#define  OSD_P1PNUM_CINDEX1_C1_DEFAULT             (0x00u)
#define  OSD_P1PNUM_CINDEX1_C1_OF(x)               _CCL_VALUEOF(_OSD_P1PNUM_CINDEX1_TYPE,x)

#define _OSD_P1PNUM_CINDEX1_C2_MASK                (0x00007F00u)
#define _OSD_P1PNUM_CINDEX1_C2_SHIFT               (8)
#define  OSD_P1PNUM_CINDEX1_C2_DEFAULT             (0x00u)
#define  OSD_P1PNUM_CINDEX1_C2_OF(x)               _CCL_VALUEOF(_OSD_P1PNUM_CINDEX1_TYPE,x)

#define _OSD_P1PNUM_CINDEX1_C3_MASK                (0x0000007Fu)
#define _OSD_P1PNUM_CINDEX1_C3_SHIFT               (0)
#define  OSD_P1PNUM_CINDEX1_C3_DEFAULT             (0x00u)
#define  OSD_P1PNUM_CINDEX1_C3_OF(x)               _CCL_VALUEOF(_OSD_P1PNUM_CINDEX1_TYPE,x)

#define  OSD_P1PNUM_CINDEX1_OF(x)             _CCL_VALUEOF(_OSD_P1PNUM_CINDEX1_TYPE,x)

#define  OSD_P1PNUM_CINDEX1_DEFAULT (_OSD_P1PNUM_CINDEX1_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,P1PNUM_CINDEX1,C0)                    \
	|_CCL_PER_FDEFAULT(OSD,P1PNUM_CINDEX1,C1)                    \
	|_CCL_PER_FDEFAULT(OSD,P1PNUM_CINDEX1,C2)                    \
	|_CCL_PER_FDEFAULT(OSD,P1PNUM_CINDEX1,C3)                    \
	)

#define OSD_P1PNUM_CINDEX1_RMK(c0,c1,c2,c3) (_OSD_P1PNUM_CINDEX1_TYPE)( \
	 _CCL_PER_FMK  (OSD,P1PNUM_CINDEX1,C0,c0)               \
	|_CCL_PER_FMK  (OSD,P1PNUM_CINDEX1,C1,c1)               \
	|_CCL_PER_FMK  (OSD,P1PNUM_CINDEX1,C2,c2)               \
	|_CCL_PER_FMK  (OSD,P1PNUM_CINDEX1,C3,c3)               \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  P1PNUM_CINDEX2   |
 * |___________________|
 *
 * P1PNUM_CINDEX2 - Display Page-1 Page-Number Character Index Register 1
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  C4   (Row-0 Column-4) -
 * (rw)  C5   (Row-0 Column-5) -
 * (rw)  C6   (Row-0 Column-6) -
 * (rw)  C7   (Row-0 Column-7) -
\*-------------------------------------------------------------------------*/
#define _OSD_P1PNUM_CINDEX2_OFFSET                 (0x0058u)
#define _OSD_P1PNUM_CINDEX2_ADDRH(h)               OSD_ADDRH(h,P1PNUM_CINDEX2)
#define _OSD_P1PNUM_CINDEX2_LEN                    (32)                 /* 32-bit*/
#define _OSD_P1PNUM_CINDEX2_TYPE                   uint32_t

#define _OSD_P1PNUM_CINDEX2_C4_MASK                (0x7F000000u)
#define _OSD_P1PNUM_CINDEX2_C4_SHIFT               (24)
#define  OSD_P1PNUM_CINDEX2_C4_DEFAULT             (0x00u)
#define  OSD_P1PNUM_CINDEX2_C4_OF(x)               _CCL_VALUEOF(_OSD_P1PNUM_CINDEX2_TYPE,x)

#define _OSD_P1PNUM_CINDEX2_C5_MASK                (0x007F0000u)
#define _OSD_P1PNUM_CINDEX2_C5_SHIFT               (16)
#define  OSD_P1PNUM_CINDEX2_C5_DEFAULT             (0x00u)
#define  OSD_P1PNUM_CINDEX2_C5_OF(x)               _CCL_VALUEOF(_OSD_P1PNUM_CINDEX2_TYPE,x)

#define _OSD_P1PNUM_CINDEX2_C6_MASK                (0x00007F00u)
#define _OSD_P1PNUM_CINDEX2_C6_SHIFT               (8)
#define  OSD_P1PNUM_CINDEX2_C6_DEFAULT             (0x00u)
#define  OSD_P1PNUM_CINDEX2_C6_OF(x)               _CCL_VALUEOF(_OSD_P1PNUM_CINDEX2_TYPE,x)

#define _OSD_P1PNUM_CINDEX2_C7_MASK                (0x0000007Fu)
#define _OSD_P1PNUM_CINDEX2_C7_SHIFT               (0)
#define  OSD_P1PNUM_CINDEX2_C7_DEFAULT             (0x00u)
#define  OSD_P1PNUM_CINDEX2_C7_OF(x)               _CCL_VALUEOF(_OSD_P1PNUM_CINDEX2_TYPE,x)

#define  OSD_P1PNUM_CINDEX2_OF(x)             _CCL_VALUEOF(_OSD_P1PNUM_CINDEX2_TYPE,x)

#define  OSD_P1PNUM_CINDEX2_DEFAULT (_OSD_P1PNUM_CINDEX2_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,P1PNUM_CINDEX2,C4)                    \
	|_CCL_PER_FDEFAULT(OSD,P1PNUM_CINDEX2,C5)                    \
	|_CCL_PER_FDEFAULT(OSD,P1PNUM_CINDEX2,C6)                    \
	|_CCL_PER_FDEFAULT(OSD,P1PNUM_CINDEX2,C7)                    \
	)

#define OSD_P1PNUM_CINDEX2_RMK(c4,c5,c6,c7) (_OSD_P1PNUM_CINDEX2_TYPE)( \
	 _CCL_PER_FMK  (OSD,P1PNUM_CINDEX2,C4,c4)               \
	|_CCL_PER_FMK  (OSD,P1PNUM_CINDEX2,C5,c5)               \
	|_CCL_PER_FMK  (OSD,P1PNUM_CINDEX2,C6,c6)               \
	|_CCL_PER_FMK  (OSD,P1PNUM_CINDEX2,C7,c7)               \
	)



/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  P1MSG_CTRL1      |
 * |___________________|
 *
 * P1MSG_CTRL1 - Display Page-1 Message Control Register 1
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  FGC    (Forward Ground Color) -
 * (rw)  BGC    (Back Ground Color) -
 * (rw)  CS     (Character set Selection code) -
 * (rw)  PS     (Select Page-1 or Page-2) -
 *                   PS    |   description
 *                ---------+--------------------------------------
 *                   P1    | Message displays on Page-1
 *                   P2    | Message displays on Page-2
 *
 * (rw)  OL     (Over Line) -
 *                   OL    |   description
 *                ---------+--------------------------------------
 *                 DISABLE | Don't display over line
 *                 ENABLE  | Display over line
 *
 * (rw)  UL     (Under Line) -
 *                   UL    |   description
 *                ---------+--------------------------------------
 *                 DISABLE | Don't display under line
 *                 ENABLE  | Display under line
 *
 * (rw)  IT     (Italic) -
 *                   IT    |   description
 *                ---------+--------------------------------------
 *                 DISABLE | Don't display in italic font
 *                 ENABLE  | Display italic in italic font
 *
 * (rw)  FL     (Flash) -
 *                   FL    |   description
 *                ---------+--------------------------------------
 *                 DISABLE | Don't display in flashing font
 *                 ENABLE  | Display italic in flashing font
 *
 * (rw)  DE     (Display Enable) -
 *                   DE    |   description
 *                ---------+--------------------------------------
 *                 DISABLE | Don't display
 *                 ENABLE  | Display
\*-------------------------------------------------------------------------*/
#define _OSD_P1MSG_CTRL1_OFFSET                 (0x0064u)
#define _OSD_P1MSG_CTRL1_ADDRH(h)               OSD_ADDRH(h,P1MSG_CTRL1)
#define _OSD_P1MSG_CTRL1_LEN                    (32)                 /* 32-bit*/
#define _OSD_P1MSG_CTRL1_TYPE                   uint32_t

#define _OSD_P1MSG_CTRL1_FGC_MASK               (0x1F000000u)
#define _OSD_P1MSG_CTRL1_FGC_SHIFT              (24)
#define  OSD_P1MSG_CTRL1_FGC_DEFAULT            (0x00u)
#define  OSD_P1MSG_CTRL1_FGC_OF(x)              _CCL_VALUEOF(_OSD_P1MSG_CTRL1_TYPE,x)

#define _OSD_P1MSG_CTRL1_BGC_MASK               (0x001F0000u)
#define _OSD_P1MSG_CTRL1_BGC_SHIFT              (16)
#define  OSD_P1MSG_CTRL1_BGC_DEFAULT            (0x00u)
#define  OSD_P1MSG_CTRL1_BGC_OF(x)              _CCL_VALUEOF(_OSD_P1MSG_CTRL1_TYPE,x)

#define _OSD_P1MSG_CTRL1_CS_MASK                (0x00001F00u)
#define _OSD_P1MSG_CTRL1_CS_SHIFT               (8)
#define  OSD_P1MSG_CTRL1_CS_DEFAULT             (0x00u)
#define  OSD_P1MSG_CTRL1_CS_OF(x)               _CCL_VALUEOF(_OSD_P1MSG_CTRL1_TYPE,x)

#define _OSD_P1MSG_CTRL1_PS_MASK                (0x00000020u)
#define _OSD_P1MSG_CTRL1_PS_SHIFT               (5)
#define  OSD_P1MSG_CTRL1_PS_DEFAULT             (0x00u)
#define  OSD_P1MSG_CTRL1_PS_OF(x)               _CCL_VALUEOF(_OSD_P1MSG_CTRL1_TYPE,x)
#define  OSD_P1MSG_CTRL1_PS_P1                  (0x00u)
#define  OSD_P1MSG_CTRL1_PS_P2                  (0x01u)

#define _OSD_P1MSG_CTRL1_OL_MASK                (0x00000010u)
#define _OSD_P1MSG_CTRL1_OL_SHIFT               (4)
#define  OSD_P1MSG_CTRL1_OL_DEFAULT             (0x00u)
#define  OSD_P1MSG_CTRL1_OL_OF(x)               _CCL_VALUEOF(_OSD_P1MSG_CTRL1_TYPE,x)
#define  OSD_P1MSG_CTRL1_OL_DISABLE             (0x00u)
#define  OSD_P1MSG_CTRL1_OL_ENABLE              (0x01u)

#define _OSD_P1MSG_CTRL1_UL_MASK                (0x00000008u)
#define _OSD_P1MSG_CTRL1_UL_SHIFT               (3)
#define  OSD_P1MSG_CTRL1_UL_DEFAULT             (0x00u)
#define  OSD_P1MSG_CTRL1_UL_OF(x)               _CCL_VALUEOF(_OSD_P1MSG_CTRL1_TYPE,x)
#define  OSD_P1MSG_CTRL1_UL_DISABLE             (0x00u)
#define  OSD_P1MSG_CTRL1_UL_ENABLE              (0x01u)

#define _OSD_P1MSG_CTRL1_IT_MASK                (0x00000004u)
#define _OSD_P1MSG_CTRL1_IT_SHIFT               (2)
#define  OSD_P1MSG_CTRL1_IT_DEFAULT             (0x00u)
#define  OSD_P1MSG_CTRL1_IT_OF(x)               _CCL_VALUEOF(_OSD_P1MSG_CTRL1_TYPE,x)
#define  OSD_P1MSG_CTRL1_IT_DISABLE             (0x00u)
#define  OSD_P1MSG_CTRL1_IT_ENABLE              (0x01u)

#define _OSD_P1MSG_CTRL1_FL_MASK                (0x00000002u)
#define _OSD_P1MSG_CTRL1_FL_SHIFT               (1)
#define  OSD_P1MSG_CTRL1_FL_DEFAULT             (0x00u)
#define  OSD_P1MSG_CTRL1_FL_OF(x)               _CCL_VALUEOF(_OSD_P1MSG_CTRL1_TYPE,x)
#define  OSD_P1MSG_CTRL1_FL_DISABLE             (0x00u)
#define  OSD_P1MSG_CTRL1_FL_ENABLE              (0x01u)

#define _OSD_P1MSG_CTRL1_DE_MASK                (0x00000001u)
#define _OSD_P1MSG_CTRL1_DE_SHIFT               (0)
#define  OSD_P1MSG_CTRL1_DE_DEFAULT             (0x00u)
#define  OSD_P1MSG_CTRL1_DE_OF(x)               _CCL_VALUEOF(_OSD_P1MSG_CTRL1_TYPE,x)
#define  OSD_P1MSG_CTRL1_DE_DISABLE             (0x00u)
#define  OSD_P1MSG_CTRL1_DE_ENABLE              (0x01u)

#define  OSD_P1MSG_CTRL1_OF(x)             _CCL_VALUEOF(_OSD_P1MSG_CTRL1_TYPE,x)

#define  OSD_P1MSG_CTRL1_DEFAULT (_OSD_P1MSG_CTRL1_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,P1MSG_CTRL1,FGC)                \
	|_CCL_PER_FDEFAULT(OSD,P1MSG_CTRL1,BGC)                \
	|_CCL_PER_FDEFAULT(OSD,P1MSG_CTRL1,CS )                \
	|_CCL_PER_FDEFAULT(OSD,P1MSG_CTRL1,PS )                \
	|_CCL_PER_FDEFAULT(OSD,P1MSG_CTRL1,OL )                \
	|_CCL_PER_FDEFAULT(OSD,P1MSG_CTRL1,UL )                \
	|_CCL_PER_FDEFAULT(OSD,P1MSG_CTRL1,IT )                \
	|_CCL_PER_FDEFAULT(OSD,P1MSG_CTRL1,FL )                \
	|_CCL_PER_FDEFAULT(OSD,P1MSG_CTRL1,DE )                \
	)

#define OSD_P1MSG_CTRL1_RMK(fgc,bgc,cs,ps,ol,ul,it,fl,de) (_OSD_P1MSG_CTRL1_TYPE)( \
	 _CCL_PER_FMK  (OSD,P1MSG_CTRL1,FGC,fgc )               \
	|_CCL_PER_FMK  (OSD,P1MSG_CTRL1,BGC,bgc )               \
	|_CCL_PER_FMK  (OSD,P1MSG_CTRL1,CS ,cs  )               \
	|_CCL_PER_FMK  (OSD,P1MSG_CTRL1,PS ,ps  )               \
	|_CCL_PER_FMK  (OSD,P1MSG_CTRL1,OL ,ol  )               \
	|_CCL_PER_FMK  (OSD,P1MSG_CTRL1,UL ,ul  )               \
	|_CCL_PER_FMK  (OSD,P1MSG_CTRL1,IT ,it  )               \
	|_CCL_PER_FMK  (OSD,P1MSG_CTRL1,FL ,fl  )               \
	|_CCL_PER_FMK  (OSD,P1MSG_CTRL1,DE ,de  )               \
	)
#define OSD_P1MSG_CTRL1_RMKS(fgc,bgc,cs,ps,ol,ul,it,fl,de) (_OSD_P1MSG_CTRL1_TYPE)( \
	 _CCL_PER_FMK  (OSD,P1MSG_CTRL1,FGC,fgc )               \
	|_CCL_PER_FMK  (OSD,P1MSG_CTRL1,BGC,bgc )               \
	|_CCL_PER_FMK  (OSD,P1MSG_CTRL1,CS ,cs  )               \
	|_CCL_PER_FMKS (OSD,P1MSG_CTRL1,PS ,ps  )               \
	|_CCL_PER_FMKS (OSD,P1MSG_CTRL1,OL ,ol  )               \
	|_CCL_PER_FMKS (OSD,P1MSG_CTRL1,UL ,ul  )               \
	|_CCL_PER_FMKS (OSD,P1MSG_CTRL1,IT ,it  )               \
	|_CCL_PER_FMKS (OSD,P1MSG_CTRL1,FL ,fl  )               \
	|_CCL_PER_FMKS (OSD,P1MSG_CTRL1,DE ,de  )               \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  P1MSG_CTRL2      |
 * |___________________|
 *
 * P1MSG_CTRL2 - Display Page-1 Message Control Register 2
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  LEN    (Number of characters) - 1 to 10 characters
 * (rw)  ROW     (Row coordinate of message active) -
 * (rw)  COL     (Column coordinate of message active) -
\*-------------------------------------------------------------------------*/
#define _OSD_P1MSG_CTRL2_OFFSET                 (0x0068u)
#define _OSD_P1MSG_CTRL2_ADDRH(h)               OSD_ADDRH(h,P1MSG_CTRL2)
#define _OSD_P1MSG_CTRL2_LEN                    (32)                 /* 32-bit*/
#define _OSD_P1MSG_CTRL2_TYPE                   uint32_t

#define _OSD_P1MSG_CTRL2_LEN_MASK               (0x000F0000u)
#define _OSD_P1MSG_CTRL2_LEN_SHIFT              (16)
#define  OSD_P1MSG_CTRL2_LEN_DEFAULT            (0x00u)
#define  OSD_P1MSG_CTRL2_LEN_OF(x)              _CCL_VALUEOF(_OSD_P1MSG_CTRL2_TYPE,x)

#define _OSD_P1MSG_CTRL2_ROW_MASK               (0x00001F00u)
#define _OSD_P1MSG_CTRL2_ROW_SHIFT              (8)
#define  OSD_P1MSG_CTRL2_ROW_DEFAULT            (0x00u)
#define  OSD_P1MSG_CTRL2_ROW_OF(x)              _CCL_VALUEOF(_OSD_P1MSG_CTRL2_TYPE,x)

#define _OSD_P1MSG_CTRL2_COL_MASK               (0x0000003Fu)
#define _OSD_P1MSG_CTRL2_COL_SHIFT              (0)
#define  OSD_P1MSG_CTRL2_COL_DEFAULT            (0x00u)
#define  OSD_P1MSG_CTRL2_COL_OF(x)              _CCL_VALUEOF(_OSD_P1MSG_CTRL2_TYPE,x)

#define  OSD_P1MSG_CTRL2_OF(x)             _CCL_VALUEOF(_OSD_P1MSG_CTRL2_TYPE,x)

#define  OSD_P1MSG_CTRL2_DEFAULT (_OSD_P1MSG_CTRL2_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,P1MSG_CTRL2,LEN)                \
	|_CCL_PER_FDEFAULT(OSD,P1MSG_CTRL2,ROW )               \
	|_CCL_PER_FDEFAULT(OSD,P1MSG_CTRL2,COL )               \
	)

#define OSD_P1MSG_CTRL2_RMK(len,row,col) (_OSD_P1MSG_CTRL2_TYPE)( \
	 _CCL_PER_FMK  (OSD,P1MSG_CTRL2,LEN,len )                     \
	|_CCL_PER_FMK  (OSD,P1MSG_CTRL2,ROW,row )                     \
	|_CCL_PER_FMK  (OSD,P1MSG_CTRL2,COL,col )                     \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  P1MSG_CINDEX1    |
 * |___________________|
 *
 * P1MSG_CINDEX1 - Display Page-1 Message Character Index Register 1
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  C0   (character 0) -
 * (rw)  C1   (character 1) -
 * (rw)  C2   (character 2) -
 * (rw)  C3   (character 3) -
\*-------------------------------------------------------------------------*/
#define _OSD_P1MSG_CINDEX1_OFFSET                 (0x006Cu)
#define _OSD_P1MSG_CINDEX1_ADDRH(h)               OSD_ADDRH(h,P1MSG_CINDEX1)
#define _OSD_P1MSG_CINDEX1_LEN                    (32)                 /* 32-bit*/
#define _OSD_P1MSG_CINDEX1_TYPE                   uint32_t

#define _OSD_P1MSG_CINDEX1_C0_MASK                (0x7F000000u)
#define _OSD_P1MSG_CINDEX1_C0_SHIFT               (24)
#define  OSD_P1MSG_CINDEX1_C0_DEFAULT             (0x00u)
#define  OSD_P1MSG_CINDEX1_C0_OF(x)               _CCL_VALUEOF(_OSD_P1MSG_CINDEX1_TYPE,x)

#define _OSD_P1MSG_CINDEX1_C1_MASK                (0x007F0000u)
#define _OSD_P1MSG_CINDEX1_C1_SHIFT               (16)
#define  OSD_P1MSG_CINDEX1_C1_DEFAULT             (0x00u)
#define  OSD_P1MSG_CINDEX1_C1_OF(x)               _CCL_VALUEOF(_OSD_P1MSG_CINDEX1_TYPE,x)

#define _OSD_P1MSG_CINDEX1_C2_MASK                (0x00007F00u)
#define _OSD_P1MSG_CINDEX1_C2_SHIFT               (8)
#define  OSD_P1MSG_CINDEX1_C2_DEFAULT             (0x00u)
#define  OSD_P1MSG_CINDEX1_C2_OF(x)               _CCL_VALUEOF(_OSD_P1MSG_CINDEX1_TYPE,x)

#define _OSD_P1MSG_CINDEX1_C3_MASK                (0x0000007Fu)
#define _OSD_P1MSG_CINDEX1_C3_SHIFT               (0)
#define  OSD_P1MSG_CINDEX1_C3_DEFAULT             (0x00u)
#define  OSD_P1MSG_CINDEX1_C3_OF(x)               _CCL_VALUEOF(_OSD_P1MSG_CINDEX1_TYPE,x)

#define  OSD_P1MSG_CINDEX1_OF(x)             _CCL_VALUEOF(_OSD_P1MSG_CINDEX1_TYPE,x)

#define  OSD_P1MSG_CINDEX1_DEFAULT (_OSD_P1MSG_CINDEX1_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,P1MSG_CINDEX1,C0)                   \
	|_CCL_PER_FDEFAULT(OSD,P1MSG_CINDEX1,C1)                   \
	|_CCL_PER_FDEFAULT(OSD,P1MSG_CINDEX1,C2)                   \
	|_CCL_PER_FDEFAULT(OSD,P1MSG_CINDEX1,C3)                   \
	)

#define OSD_P1MSG_CINDEX1_RMK(c0,c1,c2,c3) (_OSD_P1MSG_CINDEX1_TYPE)( \
	 _CCL_PER_FMK  (OSD,P1MSG_CINDEX1,C0,c0)               \
	|_CCL_PER_FMK  (OSD,P1MSG_CINDEX1,C1,c1)               \
	|_CCL_PER_FMK  (OSD,P1MSG_CINDEX1,C2,c2)               \
	|_CCL_PER_FMK  (OSD,P1MSG_CINDEX1,C3,c3)               \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  P1MSG_CINDEX2    |
 * |___________________|
 *
 * P1MSG_CINDEX2 - Display Page-1 Message Character Index Register 2
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  C4   (character 4) -
 * (rw)  C5   (character 5) -
 * (rw)  C6   (character 6) -
 * (rw)  C7   (character 7) -
\*-------------------------------------------------------------------------*/
#define _OSD_P1MSG_CINDEX2_OFFSET                 (0x0070u)
#define _OSD_P1MSG_CINDEX2_ADDRH(h)               OSD_ADDRH(h,P1MSG_CINDEX2)
#define _OSD_P1MSG_CINDEX2_LEN                    (32)                 /* 32-bit*/
#define _OSD_P1MSG_CINDEX2_TYPE                   uint32_t

#define _OSD_P1MSG_CINDEX2_C4_MASK                (0x7F000000u)
#define _OSD_P1MSG_CINDEX2_C4_SHIFT               (24)
#define  OSD_P1MSG_CINDEX2_C4_DEFAULT             (0x00u)
#define  OSD_P1MSG_CINDEX2_C4_OF(x)               _CCL_VALUEOF(_OSD_P1MSG_CINDEX2_TYPE,x)

#define _OSD_P1MSG_CINDEX2_C5_MASK                (0x007F0000u)
#define _OSD_P1MSG_CINDEX2_C5_SHIFT               (16)
#define  OSD_P1MSG_CINDEX2_C5_DEFAULT             (0x00u)
#define  OSD_P1MSG_CINDEX2_C5_OF(x)               _CCL_VALUEOF(_OSD_P1MSG_CINDEX2_TYPE,x)

#define _OSD_P1MSG_CINDEX2_C6_MASK                (0x00007F00u)
#define _OSD_P1MSG_CINDEX2_C6_SHIFT               (8)
#define  OSD_P1MSG_CINDEX2_C6_DEFAULT             (0x00u)
#define  OSD_P1MSG_CINDEX2_C6_OF(x)               _CCL_VALUEOF(_OSD_P1MSG_CINDEX2_TYPE,x)

#define _OSD_P1MSG_CINDEX2_C7_MASK                (0x0000007Fu)
#define _OSD_P1MSG_CINDEX2_C7_SHIFT               (0)
#define  OSD_P1MSG_CINDEX2_C7_DEFAULT             (0x00u)
#define  OSD_P1MSG_CINDEX2_C7_OF(x)               _CCL_VALUEOF(_OSD_P1MSG_CINDEX2_TYPE,x)

#define  OSD_P1MSG_CINDEX2_OF(x)             _CCL_VALUEOF(_OSD_P1MSG_CINDEX2_TYPE,x)

#define  OSD_P1MSG_CINDEX2_DEFAULT (_OSD_P1MSG_CINDEX2_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,P1MSG_CINDEX2,C4)                   \
	|_CCL_PER_FDEFAULT(OSD,P1MSG_CINDEX2,C5)                   \
	|_CCL_PER_FDEFAULT(OSD,P1MSG_CINDEX2,C6)                   \
	|_CCL_PER_FDEFAULT(OSD,P1MSG_CINDEX2,C7)                   \
	)

#define OSD_P1MSG_CINDEX2_RMK(c4,c5,c6,c7) (_OSD_P1MSG_CINDEX2_TYPE)( \
	 _CCL_PER_FMK  (OSD,P1MSG_CINDEX2,C4,c4)               \
	|_CCL_PER_FMK  (OSD,P1MSG_CINDEX2,C5,c5)               \
	|_CCL_PER_FMK  (OSD,P1MSG_CINDEX2,C6,c6)               \
	|_CCL_PER_FMK  (OSD,P1MSG_CINDEX2,C7,c7)               \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  P1MSG_CINDEX3    |
 * |___________________|
 *
 * P1MSG_CINDEX3 - Display Page-1 Message Character Index Register 3
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  C8   (character 8) -
 * (rw)  C9   (character 9) -
\*-------------------------------------------------------------------------*/
#define _OSD_P1MSG_CINDEX3_OFFSET                 (0x0074u)
#define _OSD_P1MSG_CINDEX3_ADDRH(h)               OSD_ADDRH(h,P1MSG_CINDEX3)
#define _OSD_P1MSG_CINDEX3_LEN                    (32)                 /* 32-bit*/
#define _OSD_P1MSG_CINDEX3_TYPE                   uint32_t

#define _OSD_P1MSG_CINDEX3_C8_MASK                (0x7F000000u)
#define _OSD_P1MSG_CINDEX3_C8_SHIFT               (24)
#define  OSD_P1MSG_CINDEX3_C8_DEFAULT             (0x00u)
#define  OSD_P1MSG_CINDEX3_C8_OF(x)               _CCL_VALUEOF(_OSD_P1MSG_CINDEX3_TYPE,x)

#define _OSD_P1MSG_CINDEX3_C9_MASK                (0x007F0000u)
#define _OSD_P1MSG_CINDEX3_C9_SHIFT               (16)
#define  OSD_P1MSG_CINDEX3_C9_DEFAULT             (0x00u)
#define  OSD_P1MSG_CINDEX3_C9_OF(x)               _CCL_VALUEOF(_OSD_P1MSG_CINDEX3_TYPE,x)


#define  OSD_P1MSG_CINDEX3_OF(x)             _CCL_VALUEOF(_OSD_P1MSG_CINDEX3_TYPE,x)

#define  OSD_P1MSG_CINDEX3_DEFAULT (_OSD_P1MSG_CINDEX3_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,P1MSG_CINDEX3,C8)                   \
	|_CCL_PER_FDEFAULT(OSD,P1MSG_CINDEX3,C9)                   \
	)

#define OSD_P1MSG_CINDEX3_RMK(c8,c9) (_OSD_P1MSG_CINDEX3_TYPE)( \
	 _CCL_PER_FMK  (OSD,P1MSG_CINDEX3,C8,c8)               \
	|_CCL_PER_FMK  (OSD,P1MSG_CINDEX3,C9,c9)               \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  I T G 1          |
 * |___________________|
 *
 * ITG1 - OSD internal timing generator configuration register 1
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  VSPRD    (Vsync pulse period) -
 *       Vsync pulse period = (HSPRD+1)*(VSPRD+1) pixel clocks. (DFF)
 *       total VSYNC period = (HSPRD+1) * (VSPRD+1+VSITV+1) pixel clocks.
 *
 * (rw)  VSITV    (Vsync pulse interval) -
 *       Vsync pulse interval in high level = (HSPRD+1)*(VSITV+1) pixel clocks (DFF)
 *
 * (r)   VSOUT    (Vsync pulse in detected) -
 *                  VSOUT  |   description
 *                ---------+--------------------------------------
 *                DETECTED |
 *               UNDETECTED|
 *
 * (rw)  HSIDLEE  (Hsync idle mode enable) - (DFF)
 *                 HSIDLEE |   description
 *                ---------+--------------------------------------
 *                 DISABLE |
 *                 ENABLE  |
 *
 * (rw)  HVSSTOP  - Timing Generator (TG) stops when HVSSTOP=1 (DFF)
 *                 HVSSTOP |   description
 *                ---------+--------------------------------------
 *                 DEASSERT|
 *                 ASSERT  |
 *
 * (rw)  HVSFMEN  - Frame Vsync Signal (DFF), when HVSMODE=1, vs_strobe will be triggered by HVSFMEN.
 *                 HVSFMEN |   description
 *                ---------+--------------------------------------
 *                 DISABLE | Display time-code
 *                 ENABLE  | Don't display time-code
 *
 * (rw)  HVSMODE - (DFF)
 *                 HVSMODE |   description
 *                ---------+--------------------------------------
 *                    0    | vsync is generated by TG or from outside
 *                    1    | vsync is generated by HVSFMEN
 *
 * (rw)  FPGABP  (FPGA clock bypass) - (DFF)
 *                  FPGABP |   description
 *                ---------+--------------------------------------
 *                  ASIC   |
 *                  FPGA   |
 *
 * (rw)  HVSGEN   - Internal hsync,vsync signal generation enable (DFF)
 *                 HVSFMEN |   description
 *                ---------+--------------------------------------
 *                 DISABLE |
 *                 ENABLE  |
\*-------------------------------------------------------------------------*/
#define _OSD_ITG1_OFFSET                 (0x00E0u)
#define _OSD_ITG1_ADDRH(h)               OSD_ADDRH(h,ITG1)
#define _OSD_ITG1_LEN                    (32)                 /* 32-bit*/
#define _OSD_ITG1_TYPE                   uint32_t

#define _OSD_ITG1_VSPRD_MASK             (0x03FF0000u)
#define _OSD_ITG1_VSPRD_SHIFT            (16)
#define  OSD_ITG1_VSPRD_DEFAULT          (0x000u)
#define  OSD_ITG1_VSPRD_OF(x)            _CCL_VALUEOF(_OSD_ITG1_TYPE,x)

#define _OSD_ITG1_VSITV_MASK             (0x00001F00u)
#define _OSD_ITG1_VSITV_SHIFT            (8)
#define  OSD_ITG1_VSITV_DEFAULT          (0x00u)
#define  OSD_ITG1_VSITV_OF(x)            _CCL_VALUEOF(_OSD_ITG1_TYPE,x)

#define _OSD_ITG1_VSOUT_MASK             (0x00000040u)
#define _OSD_ITG1_VSOUT_SHIFT            (6)
#define  OSD_ITG1_VSOUT_DEFAULT          (0x0u)
#define  OSD_ITG1_VSOUT_OF(x)            _CCL_VALUEOF(_OSD_ITG1_TYPE,x)
#define  OSD_ITG1_VSOUT_UNDETECTED       (0x00u)
#define  OSD_ITG1_VSOUT_DETECTED         (0x01u)

#define _OSD_ITG1_HSIDLEE_MASK           (0x00000020u)
#define _OSD_ITG1_HSIDLEE_SHIFT          (5)
#define  OSD_ITG1_HSIDLEE_DEFAULT        (0x00u)
#define  OSD_ITG1_HSIDLEE_OF(x)          _CCL_VALUEOF(_OSD_ITG1_TYPE,x)
#define  OSD_ITG1_HSIDLEE_DISABLE        (0x00u)
#define  OSD_ITG1_HSIDLEE_ENABLE         (0x01u)

#define _OSD_ITG1_HVSSTOP_MASK           (0x00000010u)
#define _OSD_ITG1_HVSSTOP_SHIFT          (4)
#define  OSD_ITG1_HVSSTOP_DEFAULT        (0x00u)
#define  OSD_ITG1_HVSSTOP_OF(x)          _CCL_VALUEOF(_OSD_ITG1_TYPE,x)
#define  OSD_ITG1_HVSSTOP_DEASSERT       (0x00u)
#define  OSD_ITG1_HVSSTOP_ASSERT         (0x01u)

#define _OSD_ITG1_HVSFMEM_MASK           (0x00000008u)
#define _OSD_ITG1_HVSFMEM_SHIFT          (3)
#define  OSD_ITG1_HVSFMEM_DEFAULT        (0x00u)
#define  OSD_ITG1_HVSFMEM_OF(x)          _CCL_VALUEOF(_OSD_ITG1_TYPE,x)
#define  OSD_ITG1_HVSFMEM_DISABLE        (0x00u)
#define  OSD_ITG1_HVSFMEM_ENABLE         (0x01u)

#define _OSD_ITG1_HVSMODE_MASK           (0x00000004u)
#define _OSD_ITG1_HVSMODE_SHIFT          (2)
#define  OSD_ITG1_HVSMODE_DEFAULT        (0x00u)
#define  OSD_ITG1_HVSMODE_OF(x)          _CCL_VALUEOF(_OSD_ITG1_TYPE,x)
#define  OSD_ITG1_HVSMODE_0              (0x00u)
#define  OSD_ITG1_HVSMODE_1              (0x01u)

#define _OSD_ITG1_FPGABP_MASK            (0x00000002u)
#define _OSD_ITG1_FPGABP_SHIFT           (1)
#define  OSD_ITG1_FPGABP_DEFAULT         (0x00u)
#define  OSD_ITG1_FPGABP_OF(x)           _CCL_VALUEOF(_OSD_ITG1_TYPE,x)
#define  OSD_ITG1_FPGABP_ASIC            (0x00u)
#define  OSD_ITG1_FPGABP_FPGA            (0x01u)

#define _OSD_ITG1_HVSGEN_MASK            (0x00000001u)
#define _OSD_ITG1_HVSGEN_SHIFT           (0)
#define  OSD_ITG1_HVSGEN_DEFAULT         (0x00u)
#define  OSD_ITG1_HVSGEN_OF(x)           _CCL_VALUEOF(_OSD_ITG1_TYPE,x)
#define  OSD_ITG1_HVSGEN_DISABLE         (0x00u)
#define  OSD_ITG1_HVSGEN_ENABLE          (0x01u)

#define  OSD_ITG1_OF(x)             _CCL_VALUEOF(_OSD_ITG1_TYPE,x)

#define  OSD_ITG1_DEFAULT (_OSD_ITG1_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,ITG1,VSPRD  )     \
	|_CCL_PER_FDEFAULT(OSD,ITG1,VSITV  )     \
	|_CCL_PER_FDEFAULT(OSD,ITG1,VSOUT  )     \
	|_CCL_PER_FDEFAULT(OSD,ITG1,HSIDLEE)     \
	|_CCL_PER_FDEFAULT(OSD,ITG1,HVSSTOP)     \
	|_CCL_PER_FDEFAULT(OSD,ITG1,HVSFMEM)     \
	|_CCL_PER_FDEFAULT(OSD,ITG1,HVSMODE)     \
	|_CCL_PER_FDEFAULT(OSD,ITG1,FPGABP )     \
	|_CCL_PER_FDEFAULT(OSD,ITG1,HVSGEN )     \
	)

#define OSD_ITG1_RMK(vsprd,vsitv,vsout,hsidlee,hvsstop,hvsfmem,hvsmode,fpgabp,hvsgen) (_OSD_ITG1_TYPE)( \
	 _CCL_PER_FMK  (OSD,ITG1,VSPRD  ,vsprd  )               \
	|_CCL_PER_FMK  (OSD,ITG1,VSITV  ,vsitv  )               \
	|_CCL_PER_FMK  (OSD,ITG1,VSOUT  ,vsout  )               \
	|_CCL_PER_FMK  (OSD,ITG1,HSIDLEE,hsidlee)               \
	|_CCL_PER_FMK  (OSD,ITG1,HVSSTOP,hvsstop)               \
	|_CCL_PER_FMK  (OSD,ITG1,HVSFMEM,hvsfmem)               \
	|_CCL_PER_FMK  (OSD,ITG1,HVSMODE,hvsmode)               \
	|_CCL_PER_FMK  (OSD,ITG1,FPGABP ,fpgabp )               \
	|_CCL_PER_FMK  (OSD,ITG1,HVSGEN ,hvsgen )               \
	)

#define OSD_ITG1_RMKS(vsprd,vsitv,vsout,hsidlee,hvsstop,hvsfmem,hvsmode,fpgabp,hvsgen) (_OSD_ITG1_TYPE)( \
	 _CCL_PER_FMK  (OSD,ITG1,VSPRD  ,vsprd  )               \
	|_CCL_PER_FMK  (OSD,ITG1,VSITV  ,vsitv  )               \
	|_CCL_PER_FMKS (OSD,ITG1,VSOUT  ,vsout  )               \
	|_CCL_PER_FMKS (OSD,ITG1,HSIDLEE,hsidlee)               \
	|_CCL_PER_FMKS (OSD,ITG1,HVSSTOP,hvsstop)               \
	|_CCL_PER_FMKS (OSD,ITG1,HVSFMEM,hvsfmem)               \
	|_CCL_PER_FMKS (OSD,ITG1,HVSMODE,hvsmode)               \
	|_CCL_PER_FMKS (OSD,ITG1,FPGABP ,fpgabp )               \
	|_CCL_PER_FMKS (OSD,ITG1,HVSGEN ,hvsgen )               \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  I T G 2          |
 * |___________________|
 *
 * ITG2 - OSD internal timing generator status register 2
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  HSPRD    (Hsync pulse period) -
 *       Hsync pulse period = (HSPRD+1) pixel clocks. (DFF)
 *
 * (rw)  HSITV    (Hsync pulse interval) -
 *       Hsync pulse interval in high level = (HSITV+1) pixel clocks. (DFF)
\*-------------------------------------------------------------------------*/
#define _OSD_ITG2_OFFSET                 (0x00E4u)
#define _OSD_ITG2_ADDRH(h)               OSD_ADDRH(h,ITG2)
#define _OSD_ITG2_LEN                    (32)                 /* 32-bit*/
#define _OSD_ITG2_TYPE                   uint32_t

#define _OSD_ITG2_HSPRD_MASK             (0xFFFF0000u)
#define _OSD_ITG2_HSPRD_SHIFT            (16)
#define  OSD_ITG2_HSPRD_DEFAULT          (0x0000u)
#define  OSD_ITG2_HSPRD_OF(x)            _CCL_VALUEOF(_OSD_ITG2_TYPE,x)

#define _OSD_ITG2_HSITV_MASK             (0x000000FFu)
#define _OSD_ITG2_HSITV_SHIFT            (0)
#define  OSD_ITG2_HSITV_DEFAULT          (0x00u)
#define  OSD_ITG2_HSITV_OF(x)            _CCL_VALUEOF(_OSD_ITG2_TYPE,x)

#define  OSD_ITG2_OF(x)             _CCL_VALUEOF(_OSD_ITG2_TYPE,x)

#define  OSD_ITG2_DEFAULT (_OSD_ITG2_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,ITG2,HSPRD)       \
	|_CCL_PER_FDEFAULT(OSD,ITG2,HSITV)       \
	)

#define OSD_ITG2_RMK(hsprd,hsitv) (_OSD_ITG2_TYPE)( \
	 _CCL_PER_FMK  (OSD,ITG2,HSPRD,hsprd)           \
	|_CCL_PER_FMK  (OSD,ITG2,HSITV,hsitv)           \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  I T G 3          |
 * |___________________|
 *
 * ITG3 - OSD internal timing generator status register 3
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (r)   LNCNT    (line counter) - Show the current line number in the screen
 *       It will increase by 1 whenever HSync and been reset whenever VSync.
\*-------------------------------------------------------------------------*/
#define _OSD_ITG3_OFFSET                 (0x00E8u)
#define _OSD_ITG3_ADDRH(h)               OSD_ADDRH(h,ITG3)
#define _OSD_ITG3_LEN                    (32)                 /* 32-bit*/
#define _OSD_ITG3_TYPE                   uint32_t

#define _OSD_ITG3_LNCNT_MASK             (0x000003FFu)
#define _OSD_ITG3_LNCNT_SHIFT            (0)
#define  OSD_ITG3_LNCNT_DEFAULT          (0x000u)
#define  OSD_ITG3_LNCNT_OF(x)            _CCL_VALUEOF(_OSD_ITG3_TYPE,x)
#define  OSD_ITG3_OF(x)             _CCL_VALUEOF(_OSD_ITG3_TYPE,x)

#define  OSD_ITG3_DEFAULT (_OSD_ITG3_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,ITG3,LNCNT)       \
	)

#define OSD_ITG3_RMK(lncnt) (_OSD_ITG3_TYPE)( \
	 _CCL_PER_FMK  (OSD,ITG3,LNCNT,lncnt)     \
     )

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  P1TC_INIT        |
 * |___________________|
 *
 * P1TC_INIT - Display Page-1 Time-Code Initialization Register 3
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  CS     (character set selection) -
 *                   CS    |   description
 *                ---------+--------------------------------------
 *                 DISABLE |
 *                 ENABLE  |
 * (rw)  FL     (Flash) -
 *                   FL    |   description
 *                ---------+--------------------------------------
 *                 DISABLE | Don't display in flashing font
 *                 ENABLE  | Display italic in flashing font
 *
 * (rw)  DH     (Double Height) -
 *                   DH    |   description
 *                ---------+--------------------------------------
 *                 DISABLE | Display in normal height
 *                 ENABLE  | Display in double height
 *
 * (rw)  DW     (Double Width) -
 *                   DW    |   description
 *                ---------+--------------------------------------
 *                 DISABLE | Display in normal width
 *                 ENABLE  | Display in double width
 *
 * (rw)  WP     (Width Position) -
 *                   WP    |   description
 *                ---------+--------------------------------------
 *                 DISABLE |
 *                 ENABLE  |
 *
 * (rw)  CN     (Conceal) -
 *                   CN    |   description
 *                ---------+--------------------------------------
 *                 DISABLE | Display time-code
 *                 ENABLE  | Don't display time-code
 *
 * (rw)  SM     (Seperated Mosaic) -
 *                   DW    |   description
 *                ---------+--------------------------------------
 *                 DISABLE | Display in normal mosaic
 *                 ENABLE  | Display in seperated mosaic
 *
 * (rw)  HM     (Hold Mosaic) -
 *                   DW    |   description
 *                ---------+--------------------------------------
 *                 DISABLE | Display in normal mosaic
 *                 ENABLE  | Display in hold mosaic
 *
 * (rw)  FG     (Foreground color) -
 * (rw)  BG     (Background color) -
 * (rw)  HP     (Hold mosaic Pattern) -
\*-------------------------------------------------------------------------*/
#define _OSD_P1TC_INIT_OFFSET                 (0x00F0u)
#define _OSD_P1TC_INIT_ADDRH(h)               OSD_ADDRH(h,P1TC_INIT)
#define _OSD_P1TC_INIT_LEN                    (32)                 /* 32-bit*/
#define _OSD_P1TC_INIT_TYPE                   uint32_t

#define _OSD_P1TC_INIT_CS_MASK                (0x80000000u)
#define _OSD_P1TC_INIT_CS_SHIFT               (31)
#define  OSD_P1TC_INIT_CS_DEFAULT             (0x00u)
#define  OSD_P1TC_INIT_CS_OF(x)               _CCL_VALUEOF(_OSD_P1TC_INIT_TYPE,x)
#define  OSD_P1TC_INIT_CS_DISABLE             (0x00u)
#define  OSD_P1TC_INIT_CS_ENABLE              (0x01u)

#define _OSD_P1TC_INIT_FL_MASK                (0x40000000u)
#define _OSD_P1TC_INIT_FL_SHIFT               (30)
#define  OSD_P1TC_INIT_FL_DEFAULT             (0x00u)
#define  OSD_P1TC_INIT_FL_OF(x)               _CCL_VALUEOF(_OSD_P1TC_INIT_TYPE,x)
#define  OSD_P1TC_INIT_FL_DISABLE             (0x00u)
#define  OSD_P1TC_INIT_FL_ENABLE              (0x01u)

#define _OSD_P1TC_INIT_DH_MASK                (0x20000000u)
#define _OSD_P1TC_INIT_DH_SHIFT               (29)
#define  OSD_P1TC_INIT_DH_DEFAULT             (0x00u)
#define  OSD_P1TC_INIT_DH_OF(x)               _CCL_VALUEOF(_OSD_P1TC_INIT_TYPE,x)
#define  OSD_P1TC_INIT_DH_DISABLE             (0x00u)
#define  OSD_P1TC_INIT_DH_ENABLE              (0x01u)

#define _OSD_P1TC_INIT_DW_MASK                (0x10000000u)
#define _OSD_P1TC_INIT_DW_SHIFT               (28)
#define  OSD_P1TC_INIT_DW_DEFAULT             (0x00u)
#define  OSD_P1TC_INIT_DW_OF(x)               _CCL_VALUEOF(_OSD_P1TC_INIT_TYPE,x)
#define  OSD_P1TC_INIT_DW_DISABLE             (0x00u)
#define  OSD_P1TC_INIT_DW_ENABLE              (0x01u)

#define _OSD_P1TC_INIT_WP_MASK                (0x08000000u)
#define _OSD_P1TC_INIT_WP_SHIFT               (27)
#define  OSD_P1TC_INIT_WP_DEFAULT             (0x00u)
#define  OSD_P1TC_INIT_WP_OF(x)               _CCL_VALUEOF(_OSD_P1TC_INIT_TYPE,x)
#define  OSD_P1TC_INIT_WP_DISABLE             (0x00u)
#define  OSD_P1TC_INIT_WP_ENABLE              (0x01u)

#define _OSD_P1TC_INIT_CN_MASK                (0x04000000u)
#define _OSD_P1TC_INIT_CN_SHIFT               (26)
#define  OSD_P1TC_INIT_CN_DEFAULT             (0x00u)
#define  OSD_P1TC_INIT_CN_OF(x)               _CCL_VALUEOF(_OSD_P1TC_INIT_TYPE,x)
#define  OSD_P1TC_INIT_CN_DISABLE             (0x00u)
#define  OSD_P1TC_INIT_CN_ENABLE              (0x01u)

#define _OSD_P1TC_INIT_SM_MASK                (0x02000000u)
#define _OSD_P1TC_INIT_SM_SHIFT               (25)
#define  OSD_P1TC_INIT_SM_DEFAULT             (0x00u)
#define  OSD_P1TC_INIT_SM_OF(x)               _CCL_VALUEOF(_OSD_P1TC_INIT_TYPE,x)
#define  OSD_P1TC_INIT_SM_DISABLE             (0x00u)
#define  OSD_P1TC_INIT_SM_ENABLE              (0x01u)

#define _OSD_P1TC_INIT_HM_MASK                (0x01000000u)
#define _OSD_P1TC_INIT_HM_SHIFT               (24)
#define  OSD_P1TC_INIT_HM_DEFAULT             (0x00u)
#define  OSD_P1TC_INIT_HM_OF(x)               _CCL_VALUEOF(_OSD_P1TC_INIT_TYPE,x)
#define  OSD_P1TC_INIT_HM_DISABLE             (0x00u)
#define  OSD_P1TC_INIT_HM_ENABLE              (0x01u)

#define _OSD_P1TC_INIT_FG_MASK                (0x001F0000u)
#define _OSD_P1TC_INIT_FG_SHIFT               (16)
#define  OSD_P1TC_INIT_FG_DEFAULT             (0x07u)
#define  OSD_P1TC_INIT_FG_OF(x)               _CCL_VALUEOF(_OSD_P1TC_INIT_TYPE,x)

#define _OSD_P1TC_INIT_BG_MASK                (0x00001F00u)
#define _OSD_P1TC_INIT_BG_SHIFT               (8)
#define  OSD_P1TC_INIT_BG_DEFAULT             (0x00u)
#define  OSD_P1TC_INIT_BG_OF(x)               _CCL_VALUEOF(_OSD_P1TC_INIT_TYPE,x)

#define _OSD_P1TC_INIT_HP_MASK                (0x0000007Fu)
#define _OSD_P1TC_INIT_HP_SHIFT               (0)
#define  OSD_P1TC_INIT_HP_DEFAULT             (0x20u)
#define  OSD_P1TC_INIT_HP_OF(x)               _CCL_VALUEOF(_OSD_P1TC_INIT_TYPE,x)


#define  OSD_P1TC_INIT_OF(x)             _CCL_VALUEOF(_OSD_P1TC_INIT_TYPE,x)

#define  OSD_P1TC_INIT_DEFAULT (_OSD_P1TC_INIT_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,P1TC_INIT,CS)               \
	|_CCL_PER_FDEFAULT(OSD,P1TC_INIT,FL)               \
	|_CCL_PER_FDEFAULT(OSD,P1TC_INIT,DH)               \
	|_CCL_PER_FDEFAULT(OSD,P1TC_INIT,DW)               \
	|_CCL_PER_FDEFAULT(OSD,P1TC_INIT,WP)               \
	|_CCL_PER_FDEFAULT(OSD,P1TC_INIT,CN)               \
	|_CCL_PER_FDEFAULT(OSD,P1TC_INIT,SM)               \
	|_CCL_PER_FDEFAULT(OSD,P1TC_INIT,HM)               \
	|_CCL_PER_FDEFAULT(OSD,P1TC_INIT,FG)               \
	|_CCL_PER_FDEFAULT(OSD,P1TC_INIT,BG)               \
	|_CCL_PER_FDEFAULT(OSD,P1TC_INIT,HP)               \
	)

#define OSD_P1TC_INIT_RMK(cs,fl,dh,dw,wp,cn,sm,hm,fg,bg,hp) (_OSD_P1TC_INIT_TYPE)( \
	 _CCL_PER_FMK  (OSD,P1TC_INIT,CS,cs)               \
	|_CCL_PER_FMK  (OSD,P1TC_INIT,FL,fl)               \
	|_CCL_PER_FMK  (OSD,P1TC_INIT,DH,dh)               \
	|_CCL_PER_FMK  (OSD,P1TC_INIT,DW,dw)               \
	|_CCL_PER_FMK  (OSD,P1TC_INIT,WP,wp)               \
	|_CCL_PER_FMK  (OSD,P1TC_INIT,CN,cn)               \
	|_CCL_PER_FMK  (OSD,P1TC_INIT,SM,sm)               \
	|_CCL_PER_FMK  (OSD,P1TC_INIT,HM,hm)               \
	|_CCL_PER_FMK  (OSD,P1TC_INIT,FG,fg)               \
	|_CCL_PER_FMK  (OSD,P1TC_INIT,BG,bg)               \
	|_CCL_PER_FMK  (OSD,P1TC_INIT,HP,hp)               \
	)

#define OSD_P1TC_INIT_RMKS(cs,fl,dh,dw,wp,cn,sm,hm,fg,bg,hp) (_OSD_P1TC_INIT_TYPE)( \
	 _CCL_PER_FMKS (OSD,P1TC_INIT,CS,cs)               \
	|_CCL_PER_FMKS (OSD,P1TC_INIT,FL,fl)               \
	|_CCL_PER_FMKS (OSD,P1TC_INIT,DH,dh)               \
	|_CCL_PER_FMKS (OSD,P1TC_INIT,DW,dw)               \
	|_CCL_PER_FMKS (OSD,P1TC_INIT,WP,wp)               \
	|_CCL_PER_FMKS (OSD,P1TC_INIT,CN,cn)               \
	|_CCL_PER_FMKS (OSD,P1TC_INIT,SM,sm)               \
	|_CCL_PER_FMKS (OSD,P1TC_INIT,HM,hm)               \
	|_CCL_PER_FMK  (OSD,P1TC_INIT,FG,fg)               \
	|_CCL_PER_FMK  (OSD,P1TC_INIT,BG,bg)               \
	|_CCL_PER_FMK  (OSD,P1TC_INIT,HP,hp)               \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  CLUT0_ENTRY0     |
 * |___________________|
 *
 * CLUT0_ENTRY0 - Configure Register 0 for TT/CC
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  R   (Red) -
 * (rw)  G   (Green) -
 * (rw)  B   (Blue) -
\*-------------------------------------------------------------------------*/
#define _OSD_CLUT0_ENTRY0_OFFSET                 (0x0100u)
#define _OSD_CLUT0_ENTRY0_ADDRH(h)               OSD_ADDRH(h,CLUT0_ENTRY0)
#define _OSD_CLUT0_ENTRY0_LEN                    (32)                 /* 32-bit*/
#define _OSD_CLUT0_ENTRY0_TYPE                   uint32_t

#define _OSD_CLUT0_ENTRY0_R_MASK                 (0x00FF0000u)
#define _OSD_CLUT0_ENTRY0_R_SHIFT                (16)
#define  OSD_CLUT0_ENTRY0_R_DEFAULT              (0x00u)
#define  OSD_CLUT0_ENTRY0_R_OF(x)                _CCL_VALUEOF(_OSD_CLUT0_ENTRY0_TYPE,x)

#define _OSD_CLUT0_ENTRY0_G_MASK                 (0x0000FF00u)
#define _OSD_CLUT0_ENTRY0_G_SHIFT                (8)
#define  OSD_CLUT0_ENTRY0_G_DEFAULT              (0x00u)
#define  OSD_CLUT0_ENTRY0_G_OF(x)                _CCL_VALUEOF(_OSD_CLUT0_ENTRY0_TYPE,x)

#define _OSD_CLUT0_ENTRY0_B_MASK                 (0x000000FFu)
#define _OSD_CLUT0_ENTRY0_B_SHIFT                (0)
#define  OSD_CLUT0_ENTRY0_B_DEFAULT              (0x00u)
#define  OSD_CLUT0_ENTRY0_B_OF(x)                _CCL_VALUEOF(_OSD_CLUT0_ENTRY0_TYPE,x)

#define  OSD_CLUT0_ENTRY0_OF(x)             _CCL_VALUEOF(_OSD_CLUT0_ENTRY0_TYPE,x)

#define  OSD_CLUT0_ENTRY0_DEFAULT (_OSD_CLUT0_ENTRY0_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,CLUT0_ENTRY0,R)                   \
	|_CCL_PER_FDEFAULT(OSD,CLUT0_ENTRY0,G)                   \
	|_CCL_PER_FDEFAULT(OSD,CLUT0_ENTRY0,B)                   \
	)

#define OSD_CLUT0_ENTRY0_RMK(r,g,b) (_OSD_CLUT0_ENTRY0_TYPE)( \
	 _CCL_PER_FMK  (OSD,CLUT0_ENTRY0,R,r)                     \
	|_CCL_PER_FMK  (OSD,CLUT0_ENTRY0,G,g)                     \
	|_CCL_PER_FMK  (OSD,CLUT0_ENTRY0,B,b)                     \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  CLUT0_ENTRY1     |
 * |___________________|
 *
 * CLUT0_ENTRY1 - Configure Register 1 for TT/CC
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  R   (Red) -
 * (rw)  G   (Green) -
 * (rw)  B   (Blue) -
\*-------------------------------------------------------------------------*/
#define _OSD_CLUT0_ENTRY1_OFFSET                 (0x0104u)
#define _OSD_CLUT0_ENTRY1_ADDRH(h)               OSD_ADDRH(h,CLUT0_ENTRY1)
#define _OSD_CLUT0_ENTRY1_LEN                    (32)                 /* 32-bit*/
#define _OSD_CLUT0_ENTRY1_TYPE                   uint32_t

#define _OSD_CLUT0_ENTRY1_R_MASK                 (0x00FF0000u)
#define _OSD_CLUT0_ENTRY1_R_SHIFT                (16)
#define  OSD_CLUT0_ENTRY1_R_DEFAULT              (0x00u)
#define  OSD_CLUT0_ENTRY1_R_OF(x)                _CCL_VALUEOF(_OSD_CLUT0_ENTRY1_TYPE,x)

#define _OSD_CLUT0_ENTRY1_G_MASK                 (0x0000FF00u)
#define _OSD_CLUT0_ENTRY1_G_SHIFT                (8)
#define  OSD_CLUT0_ENTRY1_G_DEFAULT              (0x00u)
#define  OSD_CLUT0_ENTRY1_G_OF(x)                _CCL_VALUEOF(_OSD_CLUT0_ENTRY1_TYPE,x)

#define _OSD_CLUT0_ENTRY1_B_MASK                 (0x000000FFu)
#define _OSD_CLUT0_ENTRY1_B_SHIFT                (0)
#define  OSD_CLUT0_ENTRY1_B_DEFAULT              (0x00u)
#define  OSD_CLUT0_ENTRY1_B_OF(x)                _CCL_VALUEOF(_OSD_CLUT0_ENTRY1_TYPE,x)

#define  OSD_CLUT0_ENTRY1_OF(x)             _CCL_VALUEOF(_OSD_CLUT0_ENTRY1_TYPE,x)

#define  OSD_CLUT0_ENTRY1_DEFAULT (_OSD_CLUT0_ENTRY1_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,CLUT0_ENTRY1,R)                   \
	|_CCL_PER_FDEFAULT(OSD,CLUT0_ENTRY1,G)                   \
	|_CCL_PER_FDEFAULT(OSD,CLUT0_ENTRY1,B)                   \
	)

#define OSD_CLUT0_ENTRY1_RMK(r,g,b) (_OSD_CLUT0_ENTRY1_TYPE)( \
	 _CCL_PER_FMK  (OSD,CLUT0_ENTRY1,R,r)                     \
	|_CCL_PER_FMK  (OSD,CLUT0_ENTRY1,G,g)                     \
	|_CCL_PER_FMK  (OSD,CLUT0_ENTRY1,B,b)                     \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  CLUT0_ENTRY2     |
 * |___________________|
 *
 * CLUT0_ENTRY2 - Configure Register 2 for TT/CC
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  R   (Red) -
 * (rw)  G   (Green) -
 * (rw)  B   (Blue) -
\*-------------------------------------------------------------------------*/
#define _OSD_CLUT0_ENTRY2_OFFSET                 (0x0108u)
#define _OSD_CLUT0_ENTRY2_ADDRH(h)               OSD_ADDRH(h,CLUT0_ENTRY2)
#define _OSD_CLUT0_ENTRY2_LEN                    (32)                 /* 32-bit*/
#define _OSD_CLUT0_ENTRY2_TYPE                   uint32_t

#define _OSD_CLUT0_ENTRY2_R_MASK                 (0x00FF0000u)
#define _OSD_CLUT0_ENTRY2_R_SHIFT                (16)
#define  OSD_CLUT0_ENTRY2_R_DEFAULT              (0x00u)
#define  OSD_CLUT0_ENTRY2_R_OF(x)                _CCL_VALUEOF(_OSD_CLUT0_ENTRY2_TYPE,x)

#define _OSD_CLUT0_ENTRY2_G_MASK                 (0x0000FF00u)
#define _OSD_CLUT0_ENTRY2_G_SHIFT                (8)
#define  OSD_CLUT0_ENTRY2_G_DEFAULT              (0x00u)
#define  OSD_CLUT0_ENTRY2_G_OF(x)                _CCL_VALUEOF(_OSD_CLUT0_ENTRY2_TYPE,x)

#define _OSD_CLUT0_ENTRY2_B_MASK                 (0x000000FFu)
#define _OSD_CLUT0_ENTRY2_B_SHIFT                (0)
#define  OSD_CLUT0_ENTRY2_B_DEFAULT              (0x00u)
#define  OSD_CLUT0_ENTRY2_B_OF(x)                _CCL_VALUEOF(_OSD_CLUT0_ENTRY2_TYPE,x)

#define  OSD_CLUT0_ENTRY2_OF(x)             _CCL_VALUEOF(_OSD_CLUT0_ENTRY2_TYPE,x)

#define  OSD_CLUT0_ENTRY2_DEFAULT (_OSD_CLUT0_ENTRY2_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,CLUT0_ENTRY2,R)                   \
	|_CCL_PER_FDEFAULT(OSD,CLUT0_ENTRY2,G)                   \
	|_CCL_PER_FDEFAULT(OSD,CLUT0_ENTRY2,B)                   \
	)

#define OSD_CLUT0_ENTRY2_RMK(r,g,b) (_OSD_CLUT0_ENTRY2_TYPE)( \
	 _CCL_PER_FMK  (OSD,CLUT0_ENTRY2,R,r)                     \
	|_CCL_PER_FMK  (OSD,CLUT0_ENTRY2,G,g)                     \
	|_CCL_PER_FMK  (OSD,CLUT0_ENTRY2,B,b)                     \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  CLUT0_ENTRY3     |
 * |___________________|
 *
 * CLUT0_ENTRY3 - Configure Register 3 for TT/CC
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  R   (Red) -
 * (rw)  G   (Green) -
 * (rw)  B   (Blue) -
\*-------------------------------------------------------------------------*/
#define _OSD_CLUT0_ENTRY3_OFFSET                 (0x010Cu)
#define _OSD_CLUT0_ENTRY3_ADDRH(h)               OSD_ADDRH(h,CLUT0_ENTRY3)
#define _OSD_CLUT0_ENTRY3_LEN                    (32)                 /* 32-bit*/
#define _OSD_CLUT0_ENTRY3_TYPE                   uint32_t

#define _OSD_CLUT0_ENTRY3_R_MASK                 (0x00FF0000u)
#define _OSD_CLUT0_ENTRY3_R_SHIFT                (16)
#define  OSD_CLUT0_ENTRY3_R_DEFAULT              (0x00u)
#define  OSD_CLUT0_ENTRY3_R_OF(x)                _CCL_VALUEOF(_OSD_CLUT0_ENTRY3_TYPE,x)

#define _OSD_CLUT0_ENTRY3_G_MASK                 (0x0000FF00u)
#define _OSD_CLUT0_ENTRY3_G_SHIFT                (8)
#define  OSD_CLUT0_ENTRY3_G_DEFAULT              (0x00u)
#define  OSD_CLUT0_ENTRY3_G_OF(x)                _CCL_VALUEOF(_OSD_CLUT0_ENTRY3_TYPE,x)

#define _OSD_CLUT0_ENTRY3_B_MASK                 (0x000000FFu)
#define _OSD_CLUT0_ENTRY3_B_SHIFT                (0)
#define  OSD_CLUT0_ENTRY3_B_DEFAULT              (0x00u)
#define  OSD_CLUT0_ENTRY3_B_OF(x)                _CCL_VALUEOF(_OSD_CLUT0_ENTRY3_TYPE,x)

#define  OSD_CLUT0_ENTRY3_OF(x)             _CCL_VALUEOF(_OSD_CLUT0_ENTRY3_TYPE,x)

#define  OSD_CLUT0_ENTRY3_DEFAULT (_OSD_CLUT0_ENTRY3_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,CLUT0_ENTRY3,R)                   \
	|_CCL_PER_FDEFAULT(OSD,CLUT0_ENTRY3,G)                   \
	|_CCL_PER_FDEFAULT(OSD,CLUT0_ENTRY3,B)                   \
	)

#define OSD_CLUT0_ENTRY3_RMK(r,g,b) (_OSD_CLUT0_ENTRY3_TYPE)( \
	 _CCL_PER_FMK  (OSD,CLUT0_ENTRY3,R,r)                     \
	|_CCL_PER_FMK  (OSD,CLUT0_ENTRY3,G,g)                     \
	|_CCL_PER_FMK  (OSD,CLUT0_ENTRY3,B,b)                     \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  CLUT0_ENTRY4     |
 * |___________________|
 *
 * CLUT0_ENTRY4 - Configure Register 1 for TT/CC
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  R   (Red) -
 * (rw)  G   (Green) -
 * (rw)  B   (Blue) -
\*-------------------------------------------------------------------------*/
#define _OSD_CLUT0_ENTRY4_OFFSET                 (0x0110u)
#define _OSD_CLUT0_ENTRY4_ADDRH(h)               OSD_ADDRH(h,CLUT0_ENTRY4)
#define _OSD_CLUT0_ENTRY4_LEN                    (32)                 /* 32-bit*/
#define _OSD_CLUT0_ENTRY4_TYPE                   uint32_t

#define _OSD_CLUT0_ENTRY4_R_MASK                 (0x00FF0000u)
#define _OSD_CLUT0_ENTRY4_R_SHIFT                (16)
#define  OSD_CLUT0_ENTRY4_R_DEFAULT              (0x00u)
#define  OSD_CLUT0_ENTRY4_R_OF(x)                _CCL_VALUEOF(_OSD_CLUT0_ENTRY4_TYPE,x)

#define _OSD_CLUT0_ENTRY4_G_MASK                 (0x0000FF00u)
#define _OSD_CLUT0_ENTRY4_G_SHIFT                (8)
#define  OSD_CLUT0_ENTRY4_G_DEFAULT              (0x00u)
#define  OSD_CLUT0_ENTRY4_G_OF(x)                _CCL_VALUEOF(_OSD_CLUT0_ENTRY4_TYPE,x)

#define _OSD_CLUT0_ENTRY4_B_MASK                 (0x000000FFu)
#define _OSD_CLUT0_ENTRY4_B_SHIFT                (0)
#define  OSD_CLUT0_ENTRY4_B_DEFAULT              (0x00u)
#define  OSD_CLUT0_ENTRY4_B_OF(x)                _CCL_VALUEOF(_OSD_CLUT0_ENTRY4_TYPE,x)

#define  OSD_CLUT0_ENTRY4_OF(x)             _CCL_VALUEOF(_OSD_CLUT0_ENTRY4_TYPE,x)

#define  OSD_CLUT0_ENTRY4_DEFAULT (_OSD_CLUT0_ENTRY4_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,CLUT0_ENTRY4,R)                   \
	|_CCL_PER_FDEFAULT(OSD,CLUT0_ENTRY4,G)                   \
	|_CCL_PER_FDEFAULT(OSD,CLUT0_ENTRY4,B)                   \
	)

#define OSD_CLUT0_ENTRY4_RMK(r,g,b) (_OSD_CLUT0_ENTRY4_TYPE)( \
	 _CCL_PER_FMK  (OSD,CLUT0_ENTRY4,R,r)                     \
	|_CCL_PER_FMK  (OSD,CLUT0_ENTRY4,G,g)                     \
	|_CCL_PER_FMK  (OSD,CLUT0_ENTRY4,B,b)                     \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  CLUT0_ENTRY5     |
 * |___________________|
 *
 * CLUT0_ENTRY5 - Configure Register 5 for TT/CC
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  R   (Red) -
 * (rw)  G   (Green) -
 * (rw)  B   (Blue) -
\*-------------------------------------------------------------------------*/
#define _OSD_CLUT0_ENTRY5_OFFSET                 (0x0114u)
#define _OSD_CLUT0_ENTRY5_ADDRH(h)               OSD_ADDRH(h,CLUT0_ENTRY5)
#define _OSD_CLUT0_ENTRY5_LEN                    (32)                 /* 32-bit*/
#define _OSD_CLUT0_ENTRY5_TYPE                   uint32_t

#define _OSD_CLUT0_ENTRY5_R_MASK                 (0x00FF0000u)
#define _OSD_CLUT0_ENTRY5_R_SHIFT                (16)
#define  OSD_CLUT0_ENTRY5_R_DEFAULT              (0x00u)
#define  OSD_CLUT0_ENTRY5_R_OF(x)                _CCL_VALUEOF(_OSD_CLUT0_ENTRY5_TYPE,x)

#define _OSD_CLUT0_ENTRY5_G_MASK                 (0x0000FF00u)
#define _OSD_CLUT0_ENTRY5_G_SHIFT                (8)
#define  OSD_CLUT0_ENTRY5_G_DEFAULT              (0x00u)
#define  OSD_CLUT0_ENTRY5_G_OF(x)                _CCL_VALUEOF(_OSD_CLUT0_ENTRY5_TYPE,x)

#define _OSD_CLUT0_ENTRY5_B_MASK                 (0x000000FFu)
#define _OSD_CLUT0_ENTRY5_B_SHIFT                (0)
#define  OSD_CLUT0_ENTRY5_B_DEFAULT              (0x00u)
#define  OSD_CLUT0_ENTRY5_B_OF(x)                _CCL_VALUEOF(_OSD_CLUT0_ENTRY5_TYPE,x)

#define  OSD_CLUT0_ENTRY5_OF(x)             _CCL_VALUEOF(_OSD_CLUT0_ENTRY5_TYPE,x)

#define  OSD_CLUT0_ENTRY5_DEFAULT (_OSD_CLUT0_ENTRY5_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,CLUT0_ENTRY5,R)                   \
	|_CCL_PER_FDEFAULT(OSD,CLUT0_ENTRY5,G)                   \
	|_CCL_PER_FDEFAULT(OSD,CLUT0_ENTRY5,B)                   \
	)

#define OSD_CLUT0_ENTRY5_RMK(r,g,b) (_OSD_CLUT0_ENTRY5_TYPE)( \
	 _CCL_PER_FMK  (OSD,CLUT0_ENTRY5,R,r)                     \
	|_CCL_PER_FMK  (OSD,CLUT0_ENTRY5,G,g)                     \
	|_CCL_PER_FMK  (OSD,CLUT0_ENTRY5,B,b)                     \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  CLUT0_ENTRY6     |
 * |___________________|
 *
 * CLUT0_ENTRY6 - Configure Register 6 for TT/CC
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  R   (Red) -
 * (rw)  G   (Green) -
 * (rw)  B   (Blue) -
\*-------------------------------------------------------------------------*/
#define _OSD_CLUT0_ENTRY6_OFFSET                 (0x0118u)
#define _OSD_CLUT0_ENTRY6_ADDRH(h)               OSD_ADDRH(h,CLUT0_ENTRY6)
#define _OSD_CLUT0_ENTRY6_LEN                    (32)                 /* 32-bit*/
#define _OSD_CLUT0_ENTRY6_TYPE                   uint32_t

#define _OSD_CLUT0_ENTRY6_R_MASK                 (0x00FF0000u)
#define _OSD_CLUT0_ENTRY6_R_SHIFT                (16)
#define  OSD_CLUT0_ENTRY6_R_DEFAULT              (0x00u)
#define  OSD_CLUT0_ENTRY6_R_OF(x)                _CCL_VALUEOF(_OSD_CLUT0_ENTRY6_TYPE,x)

#define _OSD_CLUT0_ENTRY6_G_MASK                 (0x0000FF00u)
#define _OSD_CLUT0_ENTRY6_G_SHIFT                (8)
#define  OSD_CLUT0_ENTRY6_G_DEFAULT              (0x00u)
#define  OSD_CLUT0_ENTRY6_G_OF(x)                _CCL_VALUEOF(_OSD_CLUT0_ENTRY6_TYPE,x)

#define _OSD_CLUT0_ENTRY6_B_MASK                 (0x000000FFu)
#define _OSD_CLUT0_ENTRY6_B_SHIFT                (0)
#define  OSD_CLUT0_ENTRY6_B_DEFAULT              (0x00u)
#define  OSD_CLUT0_ENTRY6_B_OF(x)                _CCL_VALUEOF(_OSD_CLUT0_ENTRY6_TYPE,x)

#define  OSD_CLUT0_ENTRY6_OF(x)             _CCL_VALUEOF(_OSD_CLUT0_ENTRY6_TYPE,x)

#define  OSD_CLUT0_ENTRY6_DEFAULT (_OSD_CLUT0_ENTRY6_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,CLUT0_ENTRY6,R)                   \
	|_CCL_PER_FDEFAULT(OSD,CLUT0_ENTRY6,G)                   \
	|_CCL_PER_FDEFAULT(OSD,CLUT0_ENTRY6,B)                   \
	)

#define OSD_CLUT0_ENTRY6_RMK(r,g,b) (_OSD_CLUT0_ENTRY6_TYPE)( \
	 _CCL_PER_FMK  (OSD,CLUT0_ENTRY6,R,r)                     \
	|_CCL_PER_FMK  (OSD,CLUT0_ENTRY6,G,g)                     \
	|_CCL_PER_FMK  (OSD,CLUT0_ENTRY6,B,b)                     \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  CLUT0_ENTRY7     |
 * |___________________|
 *
 * CLUT0_ENTRY7 - Configure Register 7 for TT/CC
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  R   (Red) -
 * (rw)  G   (Green) -
 * (rw)  B   (Blue) -
\*-------------------------------------------------------------------------*/
#define _OSD_CLUT0_ENTRY7_OFFSET                 (0x011Cu)
#define _OSD_CLUT0_ENTRY7_ADDRH(h)               OSD_ADDRH(h,CLUT0_ENTRY7)
#define _OSD_CLUT0_ENTRY7_LEN                    (32)                 /* 32-bit*/
#define _OSD_CLUT0_ENTRY7_TYPE                   uint32_t

#define _OSD_CLUT0_ENTRY7_R_MASK                 (0x00FF0000u)
#define _OSD_CLUT0_ENTRY7_R_SHIFT                (16)
#define  OSD_CLUT0_ENTRY7_R_DEFAULT              (0x00u)
#define  OSD_CLUT0_ENTRY7_R_OF(x)                _CCL_VALUEOF(_OSD_CLUT0_ENTRY7_TYPE,x)

#define _OSD_CLUT0_ENTRY7_G_MASK                 (0x0000FF00u)
#define _OSD_CLUT0_ENTRY7_G_SHIFT                (8)
#define  OSD_CLUT0_ENTRY7_G_DEFAULT              (0x00u)
#define  OSD_CLUT0_ENTRY7_G_OF(x)                _CCL_VALUEOF(_OSD_CLUT0_ENTRY7_TYPE,x)

#define _OSD_CLUT0_ENTRY7_B_MASK                 (0x000000FFu)
#define _OSD_CLUT0_ENTRY7_B_SHIFT                (0)
#define  OSD_CLUT0_ENTRY7_B_DEFAULT              (0x00u)
#define  OSD_CLUT0_ENTRY7_B_OF(x)                _CCL_VALUEOF(_OSD_CLUT0_ENTRY7_TYPE,x)

#define  OSD_CLUT0_ENTRY7_OF(x)             _CCL_VALUEOF(_OSD_CLUT0_ENTRY7_TYPE,x)

#define  OSD_CLUT0_ENTRY7_DEFAULT (_OSD_CLUT0_ENTRY7_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,CLUT0_ENTRY7,R)                   \
	|_CCL_PER_FDEFAULT(OSD,CLUT0_ENTRY7,G)                   \
	|_CCL_PER_FDEFAULT(OSD,CLUT0_ENTRY7,B)                   \
	)

#define OSD_CLUT0_ENTRY7_RMK(r,g,b) (_OSD_CLUT0_ENTRY7_TYPE)( \
	 _CCL_PER_FMK  (OSD,CLUT0_ENTRY7,R,r)                     \
	|_CCL_PER_FMK  (OSD,CLUT0_ENTRY7,G,g)                     \
	|_CCL_PER_FMK  (OSD,CLUT0_ENTRY7,B,b)                     \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  C O E F 0        |
 * |___________________|
 *
 * COEF0 - Scaler Coefficient Register 0
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  COEF0 - coefficient 0 of scaling 28 tap FIR filter (LAT)
 * (rw)  COEF1 - coefficient 1 of scaling 28 tap FIR filter (LAT)
\*-------------------------------------------------------------------------*/
#define _OSD_COEF0_OFFSET                 (0x01C0u)
#define _OSD_COEF0_ADDRH(h)               OSD_ADDRH(h,COEF0)
#define _OSD_COEF0_LEN                    (32)                 /* 32-bit*/
#define _OSD_COEF0_TYPE                   uint32_t

#define _OSD_COEF0_COEF0_MASK             (0x07FF0000u)
#define _OSD_COEF0_COEF0_SHIFT            (16)
#define  OSD_COEF0_COEF0_DEFAULT          (0x00u)
#define  OSD_COEF0_COEF0_OF(x)            _CCL_VALUEOF(_OSD_COEF0_TYPE,x)

#define _OSD_COEF0_COEF1_MASK             (0x000007FFu)
#define _OSD_COEF0_COEF1_SHIFT            (0)
#define  OSD_COEF0_COEF1_DEFAULT          (0x00u)
#define  OSD_COEF0_COEF1_OF(x)            _CCL_VALUEOF(_OSD_COEF0_TYPE,x)

#define  OSD_COEF0_OF(x)             _CCL_VALUEOF(_OSD_COEF0_TYPE,x)

#define  OSD_COEF0_DEFAULT (_OSD_COEF0_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,COEF0,COEF0)        \
	|_CCL_PER_FDEFAULT(OSD,COEF0,COEF1)        \
	)

#define OSD_COEF0_RMK(coef0, coef1) (_OSD_COEF0_TYPE)( \
	 _CCL_PER_FMK  (OSD,COEF0,COEF0,coef0)             \
	|_CCL_PER_FMK  (OSD,COEF0,COEF1,coef1)             \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  C O E F 1        |
 * |___________________|
 *
 * COEF1 - Scaler Coefficient Register 1
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  COEF2 - coefficient 2 of scaling 28 tap FIR filter (LAT)
 * (rw)  COEF3 - coefficient 3 of scaling 28 tap FIR filter (LAT)
\*-------------------------------------------------------------------------*/
#define _OSD_COEF1_OFFSET                 (0x01C4u)
#define _OSD_COEF1_ADDRH(h)               OSD_ADDRH(h,COEF1)
#define _OSD_COEF1_LEN                    (32)                 /* 32-bit*/
#define _OSD_COEF1_TYPE                   uint32_t

#define _OSD_COEF1_COEF2_MASK             (0x07FF0000u)
#define _OSD_COEF1_COEF2_SHIFT            (16)
#define  OSD_COEF1_COEF2_DEFAULT          (0x00u)
#define  OSD_COEF1_COEF2_OF(x)            _CCL_VALUEOF(_OSD_COEF1_TYPE,x)

#define _OSD_COEF1_COEF3_MASK             (0x000007FFu)
#define _OSD_COEF1_COEF3_SHIFT            (0)
#define  OSD_COEF1_COEF3_DEFAULT          (0x00u)
#define  OSD_COEF1_COEF3_OF(x)            _CCL_VALUEOF(_OSD_COEF1_TYPE,x)

#define  OSD_COEF1_OF(x)             _CCL_VALUEOF(_OSD_COEF1_TYPE,x)

#define  OSD_COEF1_DEFAULT (_OSD_COEF1_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,COEF1,COEF2)        \
	|_CCL_PER_FDEFAULT(OSD,COEF1,COEF3)        \
	)

#define OSD_COEF1_RMK(coef2, coef3) (_OSD_COEF0_TYPE)( \
	 _CCL_PER_FMK  (OSD,COEF1,COEF2,coef2)             \
	|_CCL_PER_FMK  (OSD,COEF1,COEF3,coef3)             \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  C O E F 2        |
 * |___________________|
 *
 * COEF2 - Scaler Coefficient Register 2
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  COEF4 - coefficient 4 of scaling 28 tap FIR filter (LAT)
 * (rw)  COEF5 - coefficient 5 of scaling 28 tap FIR filter (LAT)
\*-------------------------------------------------------------------------*/
#define _OSD_COEF2_OFFSET                 (0x01C8u)
#define _OSD_COEF2_ADDRH(h)               OSD_ADDRH(h,COEF2)
#define _OSD_COEF2_LEN                    (32)                 /* 32-bit*/
#define _OSD_COEF2_TYPE                   uint32_t

#define _OSD_COEF2_COEF4_MASK             (0x07FF0000u)
#define _OSD_COEF2_COEF4_SHIFT            (16)
#define  OSD_COEF2_COEF4_DEFAULT          (0x00u)
#define  OSD_COEF2_COEF4_OF(x)            _CCL_VALUEOF(_OSD_COEF2_TYPE,x)

#define _OSD_COEF2_COEF5_MASK             (0x000007FFu)
#define _OSD_COEF2_COEF5_SHIFT            (0)
#define  OSD_COEF2_COEF5_DEFAULT          (0x00u)
#define  OSD_COEF2_COEF5_OF(x)            _CCL_VALUEOF(_OSD_COEF2_TYPE,x)

#define  OSD_COEF2_OF(x)             _CCL_VALUEOF(_OSD_COEF2_TYPE,x)

#define  OSD_COEF2_DEFAULT (_OSD_COEF2_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,COEF2,COEF4)        \
	|_CCL_PER_FDEFAULT(OSD,COEF2,COEF5)        \
	)

#define OSD_COEF2_RMK(coef4, coef5) (_OSD_COEF0_TYPE)( \
	 _CCL_PER_FMK  (OSD,COEF2,COEF4,coef4)             \
	|_CCL_PER_FMK  (OSD,COEF2,COEF5,coef5)             \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  C O E F 3        |
 * |___________________|
 *
 * COEF3 - Scaler Coefficient Register 3
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  COEF6 - coefficient 6 of scaling 28 tap FIR filter (LAT)
 * (rw)  COEF7 - coefficient 7 of scaling 28 tap FIR filter (LAT)
\*-------------------------------------------------------------------------*/
#define _OSD_COEF3_OFFSET                 (0x01CCu)
#define _OSD_COEF3_ADDRH(h)               OSD_ADDRH(h,COEF3)
#define _OSD_COEF3_LEN                    (32)                 /* 32-bit*/
#define _OSD_COEF3_TYPE                   uint32_t

#define _OSD_COEF3_COEF6_MASK             (0x07FF0000u)
#define _OSD_COEF3_COEF6_SHIFT            (16)
#define  OSD_COEF3_COEF6_DEFAULT          (0x00u)
#define  OSD_COEF3_COEF6_OF(x)            _CCL_VALUEOF(_OSD_COEF3_TYPE,x)

#define _OSD_COEF3_COEF7_MASK             (0x000007FFu)
#define _OSD_COEF3_COEF7_SHIFT            (0)
#define  OSD_COEF3_COEF7_DEFAULT          (0x00u)
#define  OSD_COEF3_COEF7_OF(x)            _CCL_VALUEOF(_OSD_COEF3_TYPE,x)

#define  OSD_COEF3_OF(x)             _CCL_VALUEOF(_OSD_COEF3_TYPE,x)

#define  OSD_COEF3_DEFAULT (_OSD_COEF3_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,COEF3,COEF6)        \
	|_CCL_PER_FDEFAULT(OSD,COEF3,COEF7)        \
	)

#define OSD_COEF3_RMK(coef6, coef7) (_OSD_COEF0_TYPE)( \
	 _CCL_PER_FMK  (OSD,COEF3,COEF6,coef6)             \
	|_CCL_PER_FMK  (OSD,COEF3,COEF7,coef7)             \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  C O E F 4        |
 * |___________________|
 *
 * COEF4 - Scaler Coefficient Register 4
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  COEF8 - coefficient 8 of scaling 28 tap FIR filter (LAT)
 * (rw)  COEF9 - coefficient 9 of scaling 28 tap FIR filter (LAT)
\*-------------------------------------------------------------------------*/
#define _OSD_COEF4_OFFSET                 (0x01D0u)
#define _OSD_COEF4_ADDRH(h)               OSD_ADDRH(h,COEF4)
#define _OSD_COEF4_LEN                    (32)                 /* 32-bit*/
#define _OSD_COEF4_TYPE                   uint32_t

#define _OSD_COEF4_COEF8_MASK             (0x07FF0000u)
#define _OSD_COEF4_COEF8_SHIFT            (16)
#define  OSD_COEF4_COEF8_DEFAULT          (0x00u)
#define  OSD_COEF4_COEF8_OF(x)            _CCL_VALUEOF(_OSD_COEF4_TYPE,x)

#define _OSD_COEF4_COEF9_MASK             (0x000007FFu)
#define _OSD_COEF4_COEF9_SHIFT            (0)
#define  OSD_COEF4_COEF9_DEFAULT          (0x00u)
#define  OSD_COEF4_COEF9_OF(x)            _CCL_VALUEOF(_OSD_COEF4_TYPE,x)

#define  OSD_COEF4_OF(x)             _CCL_VALUEOF(_OSD_COEF4_TYPE,x)

#define  OSD_COEF4_DEFAULT (_OSD_COEF4_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,COEF4,COEF8)        \
	|_CCL_PER_FDEFAULT(OSD,COEF4,COEF9)        \
	)

#define OSD_COEF4_RMK(coef8, coef9) (_OSD_COEF0_TYPE)( \
	 _CCL_PER_FMK  (OSD,COEF4,COEF8,coef8)             \
	|_CCL_PER_FMK  (OSD,COEF4,COEF9,coef9)             \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  C O E F 5        |
 * |___________________|
 *
 * COEF5 - Scaler Coefficient Register 5
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  COEF10 - coefficient 10 of scaling 28 tap FIR filter (LAT)
 * (rw)  COEF11 - coefficient 11 of scaling 28 tap FIR filter (LAT)
\*-------------------------------------------------------------------------*/
#define _OSD_COEF5_OFFSET                 (0x01D4u)
#define _OSD_COEF5_ADDRH(h)               OSD_ADDRH(h,COEF5)
#define _OSD_COEF5_LEN                    (32)                 /* 32-bit*/
#define _OSD_COEF5_TYPE                   uint32_t

#define _OSD_COEF5_COEF10_MASK            (0x07FF0000u)
#define _OSD_COEF5_COEF10_SHIFT           (16)
#define  OSD_COEF5_COEF10_DEFAULT         (0x00u)
#define  OSD_COEF5_COEF10_OF(x)           _CCL_VALUEOF(_OSD_COEF5_TYPE,x)

#define _OSD_COEF5_COEF11_MASK            (0x000007FFu)
#define _OSD_COEF5_COEF11_SHIFT           (0)
#define  OSD_COEF5_COEF11_DEFAULT         (0x00u)
#define  OSD_COEF5_COEF11_OF(x)           _CCL_VALUEOF(_OSD_COEF5_TYPE,x)

#define  OSD_COEF5_OF(x)             _CCL_VALUEOF(_OSD_COEF5_TYPE,x)

#define  OSD_COEF5_DEFAULT (_OSD_COEF5_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,COEF5,COEF10)       \
	|_CCL_PER_FDEFAULT(OSD,COEF5,COEF11)       \
	)

#define OSD_COEF5_RMK(coef10, coef11) (_OSD_COEF0_TYPE)( \
	 _CCL_PER_FMK  (OSD,COEF5,COEF10,coef10)             \
	|_CCL_PER_FMK  (OSD,COEF5,COEF11,coef11)             \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  C O E F 6        |
 * |___________________|
 *
 * COEF5 - Scaler Coefficient Register 6
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  COEF12 - coefficient 12 of scaling 28 tap FIR filter (LAT)
 * (rw)  COEF13 - coefficient 13 of scaling 28 tap FIR filter (LAT)
\*-------------------------------------------------------------------------*/
#define _OSD_COEF6_OFFSET                 (0x01D8u)
#define _OSD_COEF6_ADDRH(h)               OSD_ADDRH(h,COEF6)
#define _OSD_COEF6_LEN                    (32)                 /* 32-bit*/
#define _OSD_COEF6_TYPE                   uint32_t

#define _OSD_COEF6_COEF12_MASK            (0x07FF0000u)
#define _OSD_COEF6_COEF12_SHIFT           (16)
#define  OSD_COEF6_COEF12_DEFAULT         (0x00u)
#define  OSD_COEF6_COEF12_OF(x)           _CCL_VALUEOF(_OSD_COEF6_TYPE,x)

#define _OSD_COEF6_COEF13_MASK            (0x000007FFu)
#define _OSD_COEF6_COEF13_SHIFT           (0)
#define  OSD_COEF6_COEF13_DEFAULT         (0x00u)
#define  OSD_COEF6_COEF13_OF(x)           _CCL_VALUEOF(_OSD_COEF6_TYPE,x)

#define  OSD_COEF6_OF(x)             _CCL_VALUEOF(_OSD_COEF6_TYPE,x)

#define  OSD_COEF6_DEFAULT (_OSD_COEF6_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,COEF6,COEF12)       \
	|_CCL_PER_FDEFAULT(OSD,COEF6,COEF13)       \
	)

#define OSD_COEF6_RMK(coef12, coef13) (_OSD_COEF0_TYPE)( \
	 _CCL_PER_FMK  (OSD,COEF6,COEF12,coef12)             \
	|_CCL_PER_FMK  (OSD,COEF6,COEF13,coef13)             \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  D M   M S B 0 0  |
 * |___________________|
 *
 * DM_MSB00 - TT Diacritical Mark Bitmap Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  MSB  - Most Significant 32-bit of Diacritical Mark 00 for TT.
\*-------------------------------------------------------------------------*/
#define _OSD_DM_MSB00_OFFSET                 (0x01E0u)
#define _OSD_DM_MSB00_ADDRH(h)               OSD_ADDRH(h,DM_MSB00)
#define _OSD_DM_MSB00_LEN                    (32)                 /* 32-bit*/
#define _OSD_DM_MSB00_TYPE                   uint32_t

#define  OSD_DM_MSB00_OF(x)                  _CCL_VALUEOF(_OSD_DM_MSB00_TYPE,x)

#define  OSD_DM_MSB00_DEFAULT                (_OSD_DM_MSB00_TYPE)(0x00000000u)

#define  OSD_DM_MSB00_RMK(val)               (_OSD_DM_MSB00_TYPE)(val)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  D M   M S B 0 1  |
 * |___________________|
 *
 * DM_MSB01 - TT Diacritical Mark Bitmap Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  MSB  - Most Significant 32-bit of Diacritical Mark 01 for TT.
\*-------------------------------------------------------------------------*/
#define _OSD_DM_MSB01_OFFSET                 (0x01E4u)
#define _OSD_DM_MSB01_ADDRH(h)               OSD_ADDRH(h,DM_MSB01)
#define _OSD_DM_MSB01_LEN                    (32)                 /* 32-bit*/
#define _OSD_DM_MSB01_TYPE                   uint32_t

#define  OSD_DM_MSB01_OF(x)                  _CCL_VALUEOF(_OSD_DM_MSB01_TYPE,x)

#define  OSD_DM_MSB01_DEFAULT               (_OSD_DM_MSB01_TYPE)(0x00000000u)

#define  OSD_DM_MSB01_RMK(val)              (_OSD_DM_MSB01_TYPE)(val)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  D M   M S B 0 2  |
 * |___________________|
 *
 * DM_MSB02 - TT Diacritical Mark Bitmap Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  MSB  - Most Significant 32-bit of Diacritical Mark 02 for TT.
\*-------------------------------------------------------------------------*/
#define _OSD_DM_MSB02_OFFSET                 (0x01E8u)
#define _OSD_DM_MSB02_ADDRH(h)               OSD_ADDRH(h,DM_MSB02)
#define _OSD_DM_MSB02_LEN                    (32)                 /* 32-bit*/
#define _OSD_DM_MSB02_TYPE                   uint32_t

#define  OSD_DM_MSB02_OF(x)                  _CCL_VALUEOF(_OSD_DM_MSB02_TYPE,x)

#define  OSD_DM_MSB02_DEFAULT               (_OSD_DM_MSB02_TYPE)(0x00000000u)

#define  OSD_DM_MSB02_RMK(val)              (_OSD_DM_MSB02_TYPE)(val)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  D M   M S B 0 3  |
 * |___________________|
 *
 * DM_MSB03 - TT Diacritical Mark Bitmap Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  MSB  - Most Significant 32-bit of Diacritical Mark 03 for TT.
\*-------------------------------------------------------------------------*/
#define _OSD_DM_MSB03_OFFSET                 (0x01ECu)
#define _OSD_DM_MSB03_ADDRH(h)               OSD_ADDRH(h,DM_MSB03)
#define _OSD_DM_MSB03_LEN                    (32)                 /* 32-bit*/
#define _OSD_DM_MSB03_TYPE                   uint32_t

#define  OSD_DM_MSB03_OF(x)                  _CCL_VALUEOF(_OSD_DM_MSB03_TYPE,x)

#define  OSD_DM_MSB03_DEFAULT               (_OSD_DM_MSB03_TYPE)(0x00000000u)

#define  OSD_DM_MSB03_RMK(val)              (_OSD_DM_MSB03_TYPE)(val)



/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  D M   M S B 0 4  |
 * |___________________|
 *
 * DM_MSB04 - TT Diacritical Mark Bitmap Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  MSB  - Most Significant 32-bit of Diacritical Mark 04 for TT.
\*-------------------------------------------------------------------------*/
#define _OSD_DM_MSB04_OFFSET                 (0x01F0u)
#define _OSD_DM_MSB04_ADDRH(h)               OSD_ADDRH(h,DM_MSB04)
#define _OSD_DM_MSB04_LEN                    (32)                 /* 32-bit*/
#define _OSD_DM_MSB04_TYPE                   uint32_t

#define  OSD_DM_MSB04_OF(x)                  _CCL_VALUEOF(_OSD_DM_MSB04_TYPE,x)

#define  OSD_DM_MSB04_DEFAULT               (_OSD_DM_MSB04_TYPE)(0x00000000u)

#define  OSD_DM_MSB04_RMK(val)              (_OSD_DM_MSB04_TYPE)(val)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  D M   M S B 0 5  |
 * |___________________|
 *
 * DM_MSB05 - TT Diacritical Mark Bitmap Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  MSB  - Most Significant 32-bit of Diacritical Mark 05 for TT.
\*-------------------------------------------------------------------------*/
#define _OSD_DM_MSB05_OFFSET                 (0x01F4u)
#define _OSD_DM_MSB05_ADDRH(h)               OSD_ADDRH(h,DM_MSB05)
#define _OSD_DM_MSB05_LEN                    (32)                 /* 32-bit*/
#define _OSD_DM_MSB05_TYPE                   uint32_t

#define  OSD_DM_MSB05_OF(x)                  _CCL_VALUEOF(_OSD_DM_MSB05_TYPE,x)

#define  OSD_DM_MSB05_DEFAULT               (_OSD_DM_MSB05_TYPE)(0x00000000u)

#define  OSD_DM_MSB05_RMK(val)              (_OSD_DM_MSB05_TYPE)(val)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  D M   M S B 0 6  |
 * |___________________|
 *
 * DM_MSB06 - TT Diacritical Mark Bitmap Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  MSB  - Most Significant 32-bit of Diacritical Mark 06 for TT.
\*-------------------------------------------------------------------------*/
#define _OSD_DM_MSB06_OFFSET                 (0x01F8u)
#define _OSD_DM_MSB06_ADDRH(h)               OSD_ADDRH(h,DM_MSB06)
#define _OSD_DM_MSB06_LEN                    (32)                 /* 32-bit*/
#define _OSD_DM_MSB06_TYPE                   uint32_t

#define  OSD_DM_MSB06_OF(x)                  _CCL_VALUEOF(_OSD_DM_MSB06_TYPE,x)

#define  OSD_DM_MSB06_DEFAULT               (_OSD_DM_MSB06_TYPE)(0x00000000u)

#define  OSD_DM_MSB06_RMK(val)              (_OSD_DM_MSB06_TYPE)(val)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  D M   M S B 0 7  |
 * |___________________|
 *
 * DM_MSB07 - TT Diacritical Mark Bitmap Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  MSB  - Most Significant 32-bit of Diacritical Mark 07 for TT.
\*-------------------------------------------------------------------------*/
#define _OSD_DM_MSB07_OFFSET                 (0x01FCu)
#define _OSD_DM_MSB07_ADDRH(h)               OSD_ADDRH(h,DM_MSB07)
#define _OSD_DM_MSB07_LEN                    (32)                 /* 32-bit*/
#define _OSD_DM_MSB07_TYPE                   uint32_t

#define  OSD_DM_MSB07_OF(x)                  _CCL_VALUEOF(_OSD_DM_MSB07_TYPE,x)

#define  OSD_DM_MSB07_DEFAULT                (_OSD_DM_MSB07_TYPE)(0x00000000u)

#define  OSD_DM_MSB07_RMK(val)               (_OSD_DM_MSB07_TYPE)(val)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  D M   M S B 0 8  |
 * |___________________|
 *
 * DM_MSB08 - TT Diacritical Mark Bitmap Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  MSB  - Most Significant 32-bit of Diacritical Mark 08 for TT.
\*-------------------------------------------------------------------------*/
#define _OSD_DM_MSB08_OFFSET                 (0x0200u)
#define _OSD_DM_MSB08_ADDRH(h)               OSD_ADDRH(h,DM_MSB08)
#define _OSD_DM_MSB08_LEN                    (32)                 /* 32-bit*/
#define _OSD_DM_MSB08_TYPE                   uint32_t

#define  OSD_DM_MSB08_OF(x)                  _CCL_VALUEOF(_OSD_DM_MSB08_TYPE,x)

#define  OSD_DM_MSB08_DEFAULT                (_OSD_DM_MSB08_TYPE)(0x00000000u)

#define  OSD_DM_MSB08_RMK(val)               (_OSD_DM_MSB08_TYPE)(val)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  D M   M S B 0 9  |
 * |___________________|
 *
 * DM_MSB09 - TT Diacritical Mark Bitmap Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  MSB  - Most Significant 32-bit of Diacritical Mark 09 for TT.
\*-------------------------------------------------------------------------*/
#define _OSD_DM_MSB09_OFFSET                 (0x0204u)
#define _OSD_DM_MSB09_ADDRH(h)               OSD_ADDRH(h,DM_MSB09)
#define _OSD_DM_MSB09_LEN                    (32)                 /* 32-bit*/
#define _OSD_DM_MSB09_TYPE                   uint32_t

#define  OSD_DM_MSB09_OF(x)                  _CCL_VALUEOF(_OSD_DM_MSB09_TYPE,x)

#define  OSD_DM_MSB09_DEFAULT                (_OSD_DM_MSB09_TYPE)(0x00000000u)

#define  OSD_DM_MSB09_RMK(val)               (_OSD_DM_MSB09_TYPE)(val)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  D M   M S B 1 0  |
 * |___________________|
 *
 * DM_MSB10 - TT Diacritical Mark Bitmap Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  MSB  - Most Significant 32-bit of Diacritical Mark 10 for TT.
\*-------------------------------------------------------------------------*/
#define _OSD_DM_MSB10_OFFSET                 (0x0208u)
#define _OSD_DM_MSB10_ADDRH(h)               OSD_ADDRH(h,DM_MSB10)
#define _OSD_DM_MSB10_LEN                    (32)                 /* 32-bit*/
#define _OSD_DM_MSB10_TYPE                   uint32_t

#define  OSD_DM_MSB10_OF(x)                  _CCL_VALUEOF(_OSD_DM_MSB10_TYPE,x)

#define  OSD_DM_MSB10_DEFAULT                (_OSD_DM_MSB10_TYPE)(0x00000000u)

#define  OSD_DM_MSB10_RMK(val)               (_OSD_DM_MSB10_TYPE)(val)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  D M   M S B 1 1  |
 * |___________________|
 *
 * DM_MSB11 - TT Diacritical Mark Bitmap Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  MSB  - Most Significant 32-bit of Diacritical Mark 11 for TT.
\*-------------------------------------------------------------------------*/
#define _OSD_DM_MSB11_OFFSET                 (0x020Cu)
#define _OSD_DM_MSB11_ADDRH(h)               OSD_ADDRH(h,DM_MSB11)
#define _OSD_DM_MSB11_LEN                    (32)                 /* 32-bit*/
#define _OSD_DM_MSB11_TYPE                   uint32_t

#define  OSD_DM_MSB11_OF(x)                  _CCL_VALUEOF(_OSD_DM_MSB11_TYPE,x)

#define  OSD_DM_MSB11_DEFAULT                (_OSD_DM_MSB11_TYPE)(0x00000000u)

#define  OSD_DM_MSB11_RMK(val)               (_OSD_DM_MSB11_TYPE)(val)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  D M   M S B 1 2  |
 * |___________________|
 *
 * DM_MSB12 - TT Diacritical Mark Bitmap Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  MSB  - Most Significant 32-bit of Diacritical Mark 12 for TT.
\*-------------------------------------------------------------------------*/
#define _OSD_DM_MSB12_OFFSET                 (0x0210u)
#define _OSD_DM_MSB12_ADDRH(h)               OSD_ADDRH(h,DM_MSB12)
#define _OSD_DM_MSB12_LEN                    (32)                 /* 32-bit*/
#define _OSD_DM_MSB12_TYPE                   uint32_t

#define  OSD_DM_MSB12_OF(x)                  _CCL_VALUEOF(_OSD_DM_MSB12_TYPE,x)

#define  OSD_DM_MSB12_DEFAULT                (_OSD_DM_MSB12_TYPE)(0x00000000u)

#define  OSD_DM_MSB12_RMK(val)               (_OSD_DM_MSB12_TYPE)(val)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  D M   M S B 1 3  |
 * |___________________|
 *
 * DM_MSB13 - TT Diacritical Mark Bitmap Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  MSB  - Most Significant 32-bit of Diacritical Mark 13 for TT.
\*-------------------------------------------------------------------------*/
#define _OSD_DM_MSB13_OFFSET                 (0x0214u)
#define _OSD_DM_MSB13_ADDRH(h)               OSD_ADDRH(h,DM_MSB13)
#define _OSD_DM_MSB13_LEN                    (32)                 /* 32-bit*/
#define _OSD_DM_MSB13_TYPE                   uint32_t

#define  OSD_DM_MSB13_OF(x)                  _CCL_VALUEOF(_OSD_DM_MSB13_TYPE,x)

#define  OSD_DM_MSB13_DEFAULT                (_OSD_DM_MSB13_TYPE)(0x00000000u)

#define  OSD_DM_MSB13_RMK(val)               (_OSD_DM_MSB13_TYPE)(val)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  D M   M S B 1 4  |
 * |___________________|
 *
 * DM_MSB14 - TT Diacritical Mark Bitmap Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  MSB  - Most Significant 32-bit of Diacritical Mark 14 for TT.
\*-------------------------------------------------------------------------*/
#define _OSD_DM_MSB14_OFFSET                 (0x0218u)
#define _OSD_DM_MSB14_ADDRH(h)               OSD_ADDRH(h,DM_MSB14)
#define _OSD_DM_MSB14_LEN                    (32)                 /* 32-bit*/
#define _OSD_DM_MSB14_TYPE                   uint32_t

#define  OSD_DM_MSB14_OF(x)                  _CCL_VALUEOF(_OSD_DM_MSB14_TYPE,x)

#define  OSD_DM_MSB14_DEFAULT                (_OSD_DM_MSB14_TYPE)(0x00000000u)

#define  OSD_DM_MSB14_RMK(val)               (_OSD_DM_MSB14_TYPE)(val)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  D M   M S B 1 5  |
 * |___________________|
 *
 * DM_MSB15 - TT Diacritical Mark Bitmap Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  MSB  - Most Significant 32-bit of Diacritical Mark 15 for TT.
\*-------------------------------------------------------------------------*/
#define _OSD_DM_MSB15_OFFSET                 (0x021Cu)
#define _OSD_DM_MSB15_ADDRH(h)               OSD_ADDRH(h,DM_MSB15)
#define _OSD_DM_MSB15_LEN                    (32)                 /* 32-bit*/
#define _OSD_DM_MSB15_TYPE                   uint32_t

#define  OSD_DM_MSB15_OF(x)                  _CCL_VALUEOF(_OSD_DM_MSB15_TYPE,x)

#define  OSD_DM_MSB15_DEFAULT                (_OSD_DM_MSB15_TYPE)(0x00000000u)

#define  OSD_DM_MSB15_RMK(val)               (_OSD_DM_MSB15_TYPE)(val)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  D M   L S B 0    |
 * |___________________|
 *
 * DM_LSB0 - TT Diacritical Mark Bitmap Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  00   - Last four bits of 36 bits bitmap of TT diacritical mark 00
 * (rw)  01   - Last four bits of 36 bits bitmap of TT diacritical mark 01
 * (rw)  02   - Last four bits of 36 bits bitmap of TT diacritical mark 02
 * (rw)  03   - Last four bits of 36 bits bitmap of TT diacritical mark 03
 * (rw)  04   - Last four bits of 36 bits bitmap of TT diacritical mark 04
 * (rw)  05   - Last four bits of 36 bits bitmap of TT diacritical mark 05
 * (rw)  06   - Last four bits of 36 bits bitmap of TT diacritical mark 06
 * (rw)  07   - Last four bits of 36 bits bitmap of TT diacritical mark 07
\*-------------------------------------------------------------------------*/
#define _OSD_DM_LSB0_OFFSET                 (0x0220u)
#define _OSD_DM_LSB0_ADDRH(h)               OSD_ADDRH(h,DM_LSB0)
#define _OSD_DM_LSB0_LEN                    (32)                 /* 32-bit*/
#define _OSD_DM_LSB0_TYPE                   uint32_t

#define _OSD_DM_LSB0_00_MASK                (0xF0000000u)
#define _OSD_DM_LSB0_00_SHIFT               (28)
#define  OSD_DM_LSB0_00_DEFAULT             (0x00u)
#define  OSD_DM_LSB0_00_OF(x)               _CCL_VALUEOF(_OSD_DM_LSB0_TYPE,x)

#define _OSD_DM_LSB0_01_MASK                (0x0F000000u)
#define _OSD_DM_LSB0_01_SHIFT               (24)
#define  OSD_DM_LSB0_01_DEFAULT             (0x00u)
#define  OSD_DM_LSB0_01_OF(x)               _CCL_VALUEOF(_OSD_DM_LSB0_TYPE,x)

#define _OSD_DM_LSB0_02_MASK                (0x00F00000u)
#define _OSD_DM_LSB0_02_SHIFT               (20)
#define  OSD_DM_LSB0_02_DEFAULT             (0x00u)
#define  OSD_DM_LSB0_02_OF(x)               _CCL_VALUEOF(_OSD_DM_LSB0_TYPE,x)

#define _OSD_DM_LSB0_03_MASK                (0x000F0000u)
#define _OSD_DM_LSB0_03_SHIFT               (16)
#define  OSD_DM_LSB0_03_DEFAULT             (0x00u)
#define  OSD_DM_LSB0_03_OF(x)               _CCL_VALUEOF(_OSD_DM_LSB0_TYPE,x)

#define _OSD_DM_LSB0_04_MASK                (0x0000F000u)
#define _OSD_DM_LSB0_04_SHIFT               (12)
#define  OSD_DM_LSB0_04_DEFAULT             (0x00u)
#define  OSD_DM_LSB0_04_OF(x)               _CCL_VALUEOF(_OSD_DM_LSB0_TYPE,x)

#define _OSD_DM_LSB0_05_MASK                (0x00000F00u)
#define _OSD_DM_LSB0_05_SHIFT               (8)
#define  OSD_DM_LSB0_05_DEFAULT             (0x00u)
#define  OSD_DM_LSB0_05_OF(x)               _CCL_VALUEOF(_OSD_DM_LSB0_TYPE,x)

#define _OSD_DM_LSB0_06_MASK                (0x000000F0u)
#define _OSD_DM_LSB0_06_SHIFT               (4)
#define  OSD_DM_LSB0_06_DEFAULT             (0x00u)
#define  OSD_DM_LSB0_06_OF(x)               _CCL_VALUEOF(_OSD_DM_LSB0_TYPE,x)

#define _OSD_DM_LSB0_07_MASK                (0x0000000Fu)
#define _OSD_DM_LSB0_07_SHIFT               (0)
#define  OSD_DM_LSB0_07_DEFAULT             (0x00u)
#define  OSD_DM_LSB0_07_OF(x)               _CCL_VALUEOF(_OSD_DM_LSB0_TYPE,x)

#define  OSD_DM_LSB0_OF(x)             _CCL_VALUEOF(_OSD_DM_LSB0_TYPE,x)

#define  OSD_DM_LSB0_DEFAULT (_OSD_DM_LSB0_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,DM_LSB0,00)                   \
	|_CCL_PER_FDEFAULT(OSD,DM_LSB0,01)                   \
	|_CCL_PER_FDEFAULT(OSD,DM_LSB0,02)                   \
	|_CCL_PER_FDEFAULT(OSD,DM_LSB0,03)                   \
	|_CCL_PER_FDEFAULT(OSD,DM_LSB0,04)                   \
	|_CCL_PER_FDEFAULT(OSD,DM_LSB0,05)                   \
	|_CCL_PER_FDEFAULT(OSD,DM_LSB0,06)                   \
	|_CCL_PER_FDEFAULT(OSD,DM_LSB0,07)                   \
	)

#define OSD_DM_LSB0_RMK(dm00,dm01,dm02,dm03,dm04,dm05,dm06,dm07) (_OSD_DM_LSB0_TYPE)( \
	 _CCL_PER_FMK  (OSD,DM_LSB0,00,dm00)               \
	|_CCL_PER_FMK  (OSD,DM_LSB0,01,dm01)               \
	|_CCL_PER_FMK  (OSD,DM_LSB0,02,dm02)               \
	|_CCL_PER_FMK  (OSD,DM_LSB0,03,dm03)               \
	|_CCL_PER_FMK  (OSD,DM_LSB0,04,dm04)               \
	|_CCL_PER_FMK  (OSD,DM_LSB0,05,dm05)               \
	|_CCL_PER_FMK  (OSD,DM_LSB0,06,dm06)               \
	|_CCL_PER_FMK  (OSD,DM_LSB0,07,dm07)               \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  D M   L S B 1    |
 * |___________________|
 *
 * DM_LSB1 - TT Diacritical Mark Bitmap Register
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  08   - Last four bits of 36 bits bitmap of TT diacritical mark 08
 * (rw)  09   - Last four bits of 36 bits bitmap of TT diacritical mark 09
 * (rw)  10   - Last four bits of 36 bits bitmap of TT diacritical mark 10
 * (rw)  11   - Last four bits of 36 bits bitmap of TT diacritical mark 11
 * (rw)  12   - Last four bits of 36 bits bitmap of TT diacritical mark 12
 * (rw)  13   - Last four bits of 36 bits bitmap of TT diacritical mark 13
 * (rw)  14   - Last four bits of 36 bits bitmap of TT diacritical mark 14
 * (rw)  15   - Last four bits of 36 bits bitmap of TT diacritical mark 15
\*-------------------------------------------------------------------------*/
#define _OSD_DM_LSB1_OFFSET                 (0x0224u)
#define _OSD_DM_LSB1_ADDRH(h)               OSD_ADDRH(h,DM_LSB1)
#define _OSD_DM_LSB1_LEN                    (32)                 /* 32-bit*/
#define _OSD_DM_LSB1_TYPE                   uint32_t

#define _OSD_DM_LSB1_08_MASK                (0xF0000000u)
#define _OSD_DM_LSB1_08_SHIFT               (28)
#define  OSD_DM_LSB1_08_DEFAULT             (0x00u)
#define  OSD_DM_LSB1_08_OF(x)               _CCL_VALUEOF(_OSD_DM_LSB1_TYPE,x)

#define _OSD_DM_LSB1_09_MASK                (0x0F000000u)
#define _OSD_DM_LSB1_09_SHIFT               (24)
#define  OSD_DM_LSB1_09_DEFAULT             (0x00u)
#define  OSD_DM_LSB1_09_OF(x)               _CCL_VALUEOF(_OSD_DM_LSB1_TYPE,x)

#define _OSD_DM_LSB1_10_MASK                (0x00F00000u)
#define _OSD_DM_LSB1_10_SHIFT               (20)
#define  OSD_DM_LSB1_10_DEFAULT             (0x00u)
#define  OSD_DM_LSB1_10_OF(x)               _CCL_VALUEOF(_OSD_DM_LSB1_TYPE,x)

#define _OSD_DM_LSB1_11_MASK                (0x000F0000u)
#define _OSD_DM_LSB1_11_SHIFT               (16)
#define  OSD_DM_LSB1_11_DEFAULT             (0x00u)
#define  OSD_DM_LSB1_11_OF(x)               _CCL_VALUEOF(_OSD_DM_LSB1_TYPE,x)

#define _OSD_DM_LSB1_12_MASK                (0x0000F000u)
#define _OSD_DM_LSB1_12_SHIFT               (12)
#define  OSD_DM_LSB1_12_DEFAULT             (0x00u)
#define  OSD_DM_LSB1_12_OF(x)               _CCL_VALUEOF(_OSD_DM_LSB1_TYPE,x)

#define _OSD_DM_LSB1_13_MASK                (0x00000F00u)
#define _OSD_DM_LSB1_13_SHIFT               (8)
#define  OSD_DM_LSB1_13_DEFAULT             (0x00u)
#define  OSD_DM_LSB1_13_OF(x)               _CCL_VALUEOF(_OSD_DM_LSB1_TYPE,x)

#define _OSD_DM_LSB1_14_MASK                (0x000000F0u)
#define _OSD_DM_LSB1_14_SHIFT               (4)
#define  OSD_DM_LSB1_14_DEFAULT             (0x00u)
#define  OSD_DM_LSB1_14_OF(x)               _CCL_VALUEOF(_OSD_DM_LSB1_TYPE,x)

#define _OSD_DM_LSB1_15_MASK                (0x0000000Fu)
#define _OSD_DM_LSB1_15_SHIFT               (0)
#define  OSD_DM_LSB1_15_DEFAULT             (0x00u)
#define  OSD_DM_LSB1_15_OF(x)               _CCL_VALUEOF(_OSD_DM_LSB1_TYPE,x)

#define  OSD_DM_LSB1_OF(x)             _CCL_VALUEOF(_OSD_DM_LSB1_TYPE,x)

#define  OSD_DM_LSB1_DEFAULT (_OSD_DM_LSB1_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,DM_LSB1,08)                   \
	|_CCL_PER_FDEFAULT(OSD,DM_LSB1,09)                   \
	|_CCL_PER_FDEFAULT(OSD,DM_LSB1,10)                   \
	|_CCL_PER_FDEFAULT(OSD,DM_LSB1,11)                   \
	|_CCL_PER_FDEFAULT(OSD,DM_LSB1,12)                   \
	|_CCL_PER_FDEFAULT(OSD,DM_LSB1,13)                   \
	|_CCL_PER_FDEFAULT(OSD,DM_LSB1,14)                   \
	|_CCL_PER_FDEFAULT(OSD,DM_LSB1,15)                   \
	)

#define OSD_DM_LSB1_RMK(dm08,dm09,dm10,dm11,dm12,dm13,dm14,dm15) (_OSD_DM_LSB1_TYPE)( \
	 _CCL_PER_FMK  (OSD,DM_LSB1,08,dm08)               \
	|_CCL_PER_FMK  (OSD,DM_LSB1,09,dm09)               \
	|_CCL_PER_FMK  (OSD,DM_LSB1,10,dm10)               \
	|_CCL_PER_FMK  (OSD,DM_LSB1,11,dm11)               \
	|_CCL_PER_FMK  (OSD,DM_LSB1,12,dm12)               \
	|_CCL_PER_FMK  (OSD,DM_LSB1,13,dm13)               \
	|_CCL_PER_FMK  (OSD,DM_LSB1,14,dm14)               \
	|_CCL_PER_FMK  (OSD,DM_LSB1,15,dm15)               \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  FMSYNC_CTRL1     |
 * |___________________|
 *
 * FMSYNC_CTRL1 - Control Register 1 in Frame Sync Buffer
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (r)  FE (Fringe Enable) - Character fringe function enable/disable
 *                   FE    |   description
 *                ---------+--------------------------------------
 *                 DISABLE | Don't display fringe
 *                 ENABLE  | Display fringe
 *
 * (r)  FMODE (Fringe Moe) -
 *                  FMODE  |   description
 *                ---------+--------------------------------------
 *                    ES   | E + S
 *                    EN   | E + N
 *                    WS   | W + S
 *                    WN   | W + N
 *                   EWSN  | E + W + S + N
 *
 * (r)  TTR25E - OSD TT row 25 display enable/disable
 *                 TTR25E  |   description
 *                ---------+--------------------------------------
 *                 DISABLE | Don't display row-25
 *                 ENABLE  | Display row-25
 *
 * (r)  IDXOUT (Index Out) -
 *                 IDXOUT  |   description
 *                ---------+--------------------------------------
 *                 DISABLE |
 *                 ENABLE  |
 *
 * (r)  TTMODE - OSD TT display mode
 *                 TTMODE  |   description
 *                ---------+--------------------------------------
 *                   1P    | 1 page mode
 *                   2P    | 2 page mode
 *
 * (r)  CMODE - Curor Mode
 *                  CMODE  |   description
 *                ---------+--------------------------------------
 *                    0    | cursor bitmap insertion. If OSD
 *                         |   output type is "CC", the bitmap size
 *                         |   of cursor is "16 x 26", otherwise, it is "12 x 10".
 *                    1    | foreground and background color of
 *                         |   characters in the cursor box is inverted
 *
 * (r)  OMODE - OSD Output Mode
 *                  OMODE  |   description
 *                ---------+--------------------------------------
 *                   STD   | VBI standard mode
 *                   MIX   | mixed mode (VBI display data overlay with video)
 *
 * (r)  OE - Output Enable
 *                   OE    |   description
 *                ---------+--------------------------------------
 *                 DISABLE | Don't display OSD
 *                 ENABLE  | Display OSD
\*-------------------------------------------------------------------------*/
#define _OSD_FMSYNC_CTRL1_OFFSET                 (0x0250u)
#define _OSD_FMSYNC_CTRL1_ADDRH(h)               OSD_ADDRH(h,FMSYNC_CTRL1)
#define _OSD_FMSYNC_CTRL1_LEN                    (32)                 /* 32-bit*/
#define _OSD_FMSYNC_CTRL1_TYPE                   uint32_t

#define _OSD_FMSYNC_CTRL1_FE_MASK                (0x00010000u)
#define _OSD_FMSYNC_CTRL1_FE_SHIFT               (16)
#define  OSD_FMSYNC_CTRL1_FE_DEFAULT             (0x00u)
#define  OSD_FMSYNC_CTRL1_FE_OF(x)               _CCL_VALUEOF(_OSD_FMSYNC_CTRL1_TYPE,x)
#define  OSD_FMSYNC_CTRL1_FE_DISABLE             (0x00u)
#define  OSD_FMSYNC_CTRL1_FE_ENABLE              (0x01u)

#define _OSD_FMSYNC_CTRL1_FMODE_MASK             (0x00007000u)
#define _OSD_FMSYNC_CTRL1_FMODE_SHIFT            (12)
#define  OSD_FMSYNC_CTRL1_FMODE_DEFAULT          (0x00u)
#define  OSD_FMSYNC_CTRL1_FMODE_OF(x)            _CCL_VALUEOF(_OSD_FMSYNC_CTRL1_TYPE,x)
#define  OSD_FMSYNC_CTRL1_FMODE_ES               (0x00u)
#define  OSD_FMSYNC_CTRL1_FMODE_EN               (0x01u)
#define  OSD_FMSYNC_CTRL1_FMODE_WS               (0x02u)
#define  OSD_FMSYNC_CTRL1_FMODE_WN               (0x03u)
#define  OSD_FMSYNC_CTRL1_FMODE_EWSN             (0x04u)

#define _OSD_FMSYNC_CTRL1_TTR25E_MASK            (0x00000040u)
#define _OSD_FMSYNC_CTRL1_TTR25E_SHIFT           (6)
#define  OSD_FMSYNC_CTRL1_TTR25E_DEFAULT         (0x00u)
#define  OSD_FMSYNC_CTRL1_TTR25E_OF(x)           _CCL_VALUEOF(_OSD_FMSYNC_CTRL1_TYPE,x)
#define  OSD_FMSYNC_CTRL1_TTR25E_DISABLE         (0x00u)
#define  OSD_FMSYNC_CTRL1_TTR25E_ENABLE          (0x01u)

#define _OSD_FMSYNC_CTRL1_IDXOUT_MASK            (0x00000020u)
#define _OSD_FMSYNC_CTRL1_IDXOUT_SHIFT           (5)
#define  OSD_FMSYNC_CTRL1_IDXOUT_DEFAULT         (0x00u)
#define  OSD_FMSYNC_CTRL1_IDXOUT_OF(x)           _CCL_VALUEOF(_OSD_FMSYNC_CTRL1_TYPE,x)
#define  OSD_FMSYNC_CTRL1_IDXOUT_DISABLE         (0x00u)
#define  OSD_FMSYNC_CTRL1_IDXOUT_ENABLE          (0x01u)

#define _OSD_FMSYNC_CTRL1_TTMODE_MASK            (0x00000010u)
#define _OSD_FMSYNC_CTRL1_TTMODE_SHIFT           (4)
#define  OSD_FMSYNC_CTRL1_TTMODE_DEFAULT         (0x00u)
#define  OSD_FMSYNC_CTRL1_TTMODE_OF(x)           _CCL_VALUEOF(_OSD_FMSYNC_CTRL1_TYPE,x)
#define  OSD_FMSYNC_CTRL1_TTMODE_1P              (0x00u)
#define  OSD_FMSYNC_CTRL1_TTMODE_2P              (0x01u)

#define _OSD_FMSYNC_CTRL1_CMODE_MASK             (0x00000008u)
#define _OSD_FMSYNC_CTRL1_CMODE_SHIFT            (3)
#define  OSD_FMSYNC_CTRL1_CMODE_DEFAULT          (0x00u)
#define  OSD_FMSYNC_CTRL1_CMODE_OF(x)            _CCL_VALUEOF(_OSD_FMSYNC_CTRL1_TYPE,x)
#define  OSD_FMSYNC_CTRL1_CMODE_0                (0x00u)
#define  OSD_FMSYNC_CTRL1_CMODE_1                (0x01u)

#define _OSD_FMSYNC_CTRL1_OMODE_MASK             (0x00000002u)
#define _OSD_FMSYNC_CTRL1_OMODE_SHIFT            (1)
#define  OSD_FMSYNC_CTRL1_OMODE_DEFAULT          (0x00u)
#define  OSD_FMSYNC_CTRL1_OMODE_OF(x)            _CCL_VALUEOF(_OSD_FMSYNC_CTRL1_TYPE,x)
#define  OSD_FMSYNC_CTRL1_OMODE_STD              (0x00u)
#define  OSD_FMSYNC_CTRL1_OMODE_MIX              (0x01u)

#define _OSD_FMSYNC_CTRL1_OE_MASK                (0x00000001u)
#define _OSD_FMSYNC_CTRL1_OE_SHIFT               (0)
#define  OSD_FMSYNC_CTRL1_OE_DEFAULT             (0x00u)
#define  OSD_FMSYNC_CTRL1_OE_OF(x)               _CCL_VALUEOF(_OSD_FMSYNC_CTRL1_TYPE,x)
#define  OSD_FMSYNC_CTRL1_OE_DISABLE             (0x00u)
#define  OSD_FMSYNC_CTRL1_OE_ENABLE              (0x01u)

#define  OSD_FMSYNC_CTRL1_OF(x)             _CCL_VALUEOF(_OSD_FMSYNC_CTRL1_TYPE,x)

#define  OSD_FMSYNC_CTRL1_DEFAULT (_OSD_FMSYNC_CTRL1_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,FMSYNC_CTRL1,FE)                  \
	|_CCL_PER_FDEFAULT(OSD,FMSYNC_CTRL1,FMODE)               \
	|_CCL_PER_FDEFAULT(OSD,FMSYNC_CTRL1,TTR25E)              \
	|_CCL_PER_FDEFAULT(OSD,FMSYNC_CTRL1,IDXOUT)              \
	|_CCL_PER_FDEFAULT(OSD,FMSYNC_CTRL1,TTMODE)              \
	|_CCL_PER_FDEFAULT(OSD,FMSYNC_CTRL1,CMODE)               \
	|_CCL_PER_FDEFAULT(OSD,FMSYNC_CTRL1,OMODE)               \
	|_CCL_PER_FDEFAULT(OSD,FMSYNC_CTRL1,OE)                  \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  FMSYNC_CTRL2     |
 * |___________________|
 *
 * FMSYNC_CTRL2 - Control Register 2 in Frame Sync Buffer
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (r)   MBM (Mosaic character Bit Mask) -
 *                   MBM   |   description
 *                ---------+--------------------------------------
 *                     0   | normal display
 *                     1   | display mask
 *
 * (r)   MLM (Mosaic character Line Mask) -
 *                   MLM   |   description
 *                ---------+--------------------------------------
 *                     0   | normal display
 *                     1   | display mask
\*-------------------------------------------------------------------------*/
#define _OSD_FMSYNC_CTRL2_OFFSET                 (0x0254u)
#define _OSD_FMSYNC_CTRL2_ADDRH(h)               OSD_ADDRH(h,FMSYNC_CTRL2)
#define _OSD_FMSYNC_CTRL2_LEN                    (32)                 /* 32-bit*/
#define _OSD_FMSYNC_CTRL2_TYPE                   uint32_t

#define _OSD_FMSYNC_CTRL2_MBM_MASK               (0x0FFF0000u)
#define _OSD_FMSYNC_CTRL2_MBM_SHIFT              (16)
#define  OSD_FMSYNC_CTRL2_MBM_DEFAULT            (0x861u)
#define  OSD_FMSYNC_CTRL2_MBM_OF(x)              _CCL_VALUEOF(_OSD_FMSYNC_CTRL2_TYPE,x)

#define _OSD_FMSYNC_CTRL2_MLM_MASK               (0x000003FFu)
#define _OSD_FMSYNC_CTRL2_MLM_SHIFT              (0)
#define  OSD_FMSYNC_CTRL2_MLM_DEFAULT            (0x249u)
#define  OSD_FMSYNC_CTRL2_MLM_OF(x)              _CCL_VALUEOF(_OSD_FMSYNC_CTRL2_TYPE,x)

#define  OSD_FMSYNC_CTRL2_OF(x)             _CCL_VALUEOF(_OSD_FMSYNC_CTRL2_TYPE,x)

#define  OSD_FMSYNC_CTRL2_DEFAULT (_OSD_FMSYNC_CTRL2_TYPE)(  \
	 _CCL_PER_FDEFAULT(OSD,FMSYNC_CTRL2,MBM)                 \
	|_CCL_PER_FDEFAULT(OSD,FMSYNC_CTRL2,MLM)                 \
	)



#endif	/* !__C_CHIP_LIB_REALTEK_SPARROW_VBI_TELETEXT_ON_SCREEN_DISPLAY_HARDWARE_ABSTRACT_LAYER_HEADER_INCLUDED__ */


/*-------------------------------------------------------------------------*\
 * End of osd_hal.h                                                        *
\*-------------------------------------------------------------------------*/

