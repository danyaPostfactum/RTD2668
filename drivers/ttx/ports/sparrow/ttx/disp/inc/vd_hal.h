/*-------------------------------------------------------------------------*\
 *               File: vd_hal.h
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
 *   VD_ADDR   (<REG>)                  -   Register address
 *   VD_RGET   (<REG>)                  -   Returns the value in the peripheral register
 *   VD_RSET   (<REG>,x)                -   Register set
 *   VD_FGET   (<REG>,<FIELD>)          -   Returns the value of the specified field
 *                                             in the peripheral register
 *   VD_FSET   (<REG>,<FIELD>,fieldval) -   Writes fieldval to the specified field
 *                                             in the peripheral register
 *   VD_FSETS  (<REG>,<FIELD>,<SYM>)    -   Writes the symbol value to the specified
 *                                             field in the peripheral
 *   VD_RGETA  (addr,<REG>)             -   Gets register for a given address
 *   VD_RSETA  (addr,<REG>,x)           -   Sets register for a given address
 *
 *  <NOTE>
 *  Not completed yet!!!!!!!!!!
\*-------------------------------------------------------------------------*/

#ifndef	__C_CHIP_LIB_REALTEK_RTD_2668_VIDEO_DECODER_HARDWARE_ABSTRACT_LAYER_HEADER_INCLUDED__
#define	__C_CHIP_LIB_REALTEK_RTD_2668_VIDEO_DECODER_HARDWARE_ABSTRACT_LAYER_HEADER_INCLUDED__ (1)


/* CCL headers ------------------------------------------------------------*/
#include <ccl/std.h>    /* the standard header of C Chip Library */

/*
 *  Base address of VD
 */
#define _VD_ADDR_BASE  (0x5000u)

/*
 *  The function used for read register from this IP. Prototype is
 *     uint32_t _VD_REG_READ( addr );
 */
#define _VD_REG_READ   TTX_Reg32Read

/*
 *  The function used for read register from this IP. Prototype is
 *     void    _VD_REG_WRITE( addr, val );
 */
#define _VD_REG_WRITE  TTX_Reg32Write

/*
 * The shift left of data-bus connection in bits, e.g.,
 *   conventionally, the D0-D7 of VD are connected to CPU data-bus D0-D7 (_VD_DB_SHFT = 0).
 *   However, in some cases,
 *     the D0-D7 of 82C52 are connected to CPU data-bus D16-D24 (_VD_DB_SHFT = 16).
 */
#define _VD_DB_SHFT     (0)

/*
 * The width of data-bus connection in bits, e.g.,
 *   conventionally, the D0-D7 of 82C52 are connected to CPU data-bus D0-D7 (_VD_DB_WIDTH = 0x00FFu).
 *   Please note, despite the shift of data-bus, _VD_DB_WITH starts from LSB.
 *   As a consequence,
 *     if the D0-D7 of 82C52 are connected to CPU data-bus D16-D24 (_VD_DB_WIDTH = 0x00FFu).
 */
#define _VD_DB_WIDTH   (0x000000FFu)

/*
 * The shift left of address-bus connection in bits, e.g.,
 *   conventionally, the A0-A7 of 82C52 are connected to CPU address-bus A0-A7 (_VD_AB_SHFT = Uint8).
 *   However, in some cases,
 *     the A0-A7 of 82C52 are connected to CPU data-bus A2-A9 (_VD_DB_SHFT = uint32_t).
 */
#define _VD_AB_SHFT     uint16_t




/* Field related macros */

#define VD_FMK(REG,FIELD,x)\
	_CCL_PER_FMK(VD,REG,FIELD,x)

#define VD_FMKS(REG,FIELD,SYM)\
	_CCL_PER_FMKS(VD,REG,FIELD,SYM)

#define VD_FEXTRACT(REG,FIELD,x)\
	_CCL_PER_FEXTRACT(VD,REG,FIELD,x)


/* Raw register/field access macros */

#define VD_ADDR(REG)\
    (_VD_ADDR_BASE+_VD_##REG##_OFFSET)

#define VD_RGET(REG)\
    _VD_RGETA(VD_ADDR(REG),REG)

#define VD_RSET(REG,x)\
    _VD_RSETA(VD_ADDR(REG),REG,x)

#define VD_FGET(REG,FIELD)\
    _VD_FGETA(VD_ADDR(REG),REG,FIELD)

#define VD_FSET(REG,FIELD,x)\
    _VD_FSETA(VD_ADDR(REG),REG,FIELD,x)

#define VD_FSETS(REG,FIELD,SYM)\
    _VD_FSETSA(VD_ADDR(REG),REG,FIELD,SYM)


/* Address based register/field access macros */

#define _VD_RGETA(addr,REG)\
	_CCL_PER_RGET(addr,VD,REG,_VD_DB_SHFT,_VD_DB_WIDTH)

#define _VD_RSETA(addr,REG,x)\
	_CCL_PER_RSET(addr,VD,REG,x,_VD_DB_SHFT,_VD_DB_WIDTH)

#define _VD_RSETSA(addr,REG,SYM)\
	_CCL_PER_RSETS(addr,VD,REG,SYM,_VD_DB_SHFT,_VD_DB_WIDTH)

#define _VD_FGETA(addr,REG,FIELD)\
	_CCL_PER_FGET(addr,VD,REG,FIELD,_VD_DB_SHFT,_VD_DB_WIDTH)

#define _VD_FSETA(addr,REG,FIELD,x)\
	_CCL_PER_FSET(addr,VD,REG,FIELD,x,_VD_DB_SHFT,_VD_DB_WIDTH)

#define _VD_FSETSA(addr,REG,FIELD,SYM)\
	_CCL_PER_FSETS(addr,VD,REG,FIELD,SYM,_VD_DB_SHFT,_VD_DB_WIDTH)


/* Handle based register/field access macros */

/* This is for base-address determined by handle h.
#define VD_ADDRH(h,REG)\
	(_VD_AB_SHFT)(&((h)->addrBase[_VD_##REG##_OFFSET]))
*/
#define VD_ADDRH(h,REG)\
	(_VD_AB_SHFT)(_VD_ADDR_BASE+_VD_##REG##_OFFSET)

#define VD_RGETH(h,REG)\
	_VD_RGETA(VD_ADDRH(h,REG),REG)

#define VD_RSETH(h,REG,x)\
	_VD_RSETA(VD_ADDRH(h,REG),REG,x)

#define VD_RSETSH(h,REG,SYM)\
	_VD_RSETSA(VD_ADDRH(h,REG),REG,SYM)

#define VD_FGETH(h,REG,FIELD)\
	_VD_FGETA(VD_ADDRH(h,REG),REG,FIELD)

#define VD_FSETH(h,REG,FIELD,x)\
	_VD_FSETA(VD_ADDRH(h,REG),REG,FIELD,x)

#define VD_FSETSH(h,REG,FIELD,SYM)\
	_VD_FSETSA(VD_ADDRH(h,REG),REG,FIELD,SYM)



/* Function Prototypes ----------------------------------------------------*/

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  C T R L 0        |
 * |___________________|
 *
 * CTRL0 - VIDEO_CONTROL0
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  HVDLY   (HV delay) - emulates the HV-delay mode found on Sony studio monitors.
 *                  HVDLY  |   description
 *              -----------+-------------------
 *                 DISABLE | no emulation
 *                 ENABLE  | emulation
 *
 * (rw)  HPXL  (H-Pixel) - select the output display format.
 *                  HPXL   |   description
 *              -----------+------------------------------------
 *                 858PPL  | 858 pixels/line -> NTSC, PAL(M)
 *                 864PPL  | 864 pixels/line -> PAL (B, D, G, H, I, N, CN), SECAM
 *                 780PPL  | 780 pixels/line -> NTSC Square Pixel, PAL (M) Square Pixel
 *                 944PPL  | 944 pixels/line -> PAL (B, D, G, H, I, N) Square Pixel
 *
 * (rw)  VLINE  (V-Line) - select the numbers of scan lines per frame.
 *                  VLINE  |   description
 *                ---------+-------------------
 *                   525   | 525 lines per frame
 *                   625   | 625 lines per frame
 *
 * (rw)  COLOR - select video color standard.
 *                  COLOR  |   description
 *                ---------+-------------------
 *                   NTSC  | NTSC
 *                   PALI  | PAL (I, B, G, H, D, N)
 *                   PALM  | PAL (M)
 *                   PALCN | PAL (CN)
 *                   SECAM | SECAM
 *                 NTSC443A| NTSC443(1127 & 135mode)
 *                 NTSC443B| NTSC443(1126 & 234mode)
 *                   PAL60 | PAL60
\*-------------------------------------------------------------------------*/
#define _VD_CTRL0_OFFSET                   (0x0000u)
#define _VD_CTRL0_ADDRH(h)                 VD_ADDRH(h,CTRL0)
#define _VD_CTRL0_LEN                      (8)                 /* 8-bit*/
#define _VD_CTRL0_TYPE                     uint8_t

#define _VD_CTRL0_HDLY_MASK                (0x80u)
#define _VD_CTRL0_HDLY_SHIFT               (7)
#define  VD_CTRL0_HDLY_DEFAULT             (0x00u)
#define  VD_CTRL0_HDLY_OF(x)               _CCL_VALUEOF(_VD_CTRL0_TYPE,x)
#define  VD_CTRL0_HDLY_DISABLE             (0x00u)
#define  VD_CTRL0_HDLY_ENABLE              (0x01u)

#define _VD_CTRL0_HPXL_MASK                (0x60u)
#define _VD_CTRL0_HPXL_SHIFT               (5)
#define  VD_CTRL0_HPXL_DEFAULT             (0x00u)
#define  VD_CTRL0_HPXL_OF(x)               _CCL_VALUEOF(_VD_CTRL0_TYPE,x)
#define  VD_CTRL0_HPXL_858PPL              (0x00u)
#define  VD_CTRL0_HPXL_864PPL              (0x01u)
#define  VD_CTRL0_HPXL_780PPL              (0x02u)
#define  VD_CTRL0_HPXL_944PPL              (0x03u)

#define _VD_CTRL0_VLINE_MASK               (0x10u)
#define _VD_CTRL0_VLINE_SHIFT              (4)
#define  VD_CTRL0_VLINE_DEFAULT            (0x00u)
#define  VD_CTRL0_VLINE_OF(x)              _CVLINE_VALUEOF(_VD_CTRL0_TYPE,x)
#define  VD_CTRL0_VLINE_525                (0x00u)
#define  VD_CTRL0_VLINE_625                (0x01u)

#define _VD_CTRL0_COLOR_MASK               (0x0Eu)
#define _VD_CTRL0_COLOR_SHIFT              (1)
#define  VD_CTRL0_COLOR_DEFAULT            (0x00u)
#define  VD_CTRL0_COLOR_OF(x)              _CCOLOR_VALUEOF(_VD_CTRL0_TYPE,x)
#define  VD_CTRL0_COLOR_NTSC               (0x00u)
#define  VD_CTRL0_COLOR_PALI               (0x01u)
#define  VD_CTRL0_COLOR_PALM               (0x02u)
#define  VD_CTRL0_COLOR_PALCN              (0x03u)
#define  VD_CTRL0_COLOR_SECAM              (0x04u)
#define  VD_CTRL0_COLOR_NTSC443A           (0x05u)
#define  VD_CTRL0_COLOR_NTSC443B           (0x06u)
#define  VD_CTRL0_COLOR_PAL60              (0x07u)

#define  VD_CTRL0_OF(x)             _CCL_VALUEOF(_VD_CTRL0_TYPE,x)

#define  VD_CTRL0_DEFAULT (_VD_CTRL0_TYPE)(  \
	 _CCL_PER_FDEFAULT(VD,CTRL0,HDLY )       \
	|_CCL_PER_FDEFAULT(VD,CTRL0,HPXL )       \
	|_CCL_PER_FDEFAULT(VD,CTRL0,VLINE)       \
	|_CCL_PER_FDEFAULT(VD,CTRL0,COLOR)       \
	)

#define VD_CTRL0_RMK(hdly, hpxl, vline, color) (_VD_CTRL0_TYPE)( \
	 _CCL_PER_FMK  (VD,CTRL0,HDLY ,hdly )    \
	|_CCL_PER_FMK  (VD,CTRL0,HPXL ,hpxl )    \
	|_CCL_PER_FMK  (VD,CTRL0,VLINE,vline)    \
	|_CCL_PER_FMK  (VD,CTRL0,COLOR,color)    \
	)

#define VD_CTRL0_RMKS(hdly, hpxl, vline, color) (_VD_CTRL0_TYPE)( \
	 _CCL_PER_FMKS (VD,CTRL0,HDLY ,hdly )    \
	|_CCL_PER_FMKS (VD,CTRL0,HPXL ,hpxl )    \
	|_CCL_PER_FMKS (VD,CTRL0,VLINE,vline)    \
	|_CCL_PER_FMKS (VD,CTRL0,COLOR,color)    \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  S T A T U S 1    |
 * |___________________|
 *
 * STATUS1 - VIDEO_STATUS_REG1
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (r)  MVCS   (Macro-Vision Color Stripes) - macro-vision color stripes detected.
 *          The number indicates the number of color stripe lines in each group.
 *
 * (r)  MVVD  (Macro-Vision VBI Detected) - macro-vision VBI pseudo-sync pulses detection
 *                 MVVD    |   description
 *              -----------+-------------------
 *               UNDETECTED| un-detected
 *                 DETECTED| detected
 *
 * (r)  CL  (Chroma Lock) - chroma PLL locked to colour burst.
 *                   CL    |   description
 *                ---------+-------------------
 *                UNLOCKED | unlocked
 *                  LOCKED | locked
 *
 * (r)  VL (Vertical Lock) - vertical lock.
 *                   VL    |   description
 *                ---------+-------------------
 *                UNLOCKED | unlocked
 *                  LOCKED | locked
 *
 * (r)  HL (Horizontal Lock) - horizontal line lock.
 *                   HL    |   description
 *                ---------+-------------------
 *                UNLOCKED | unlocked
 *                  LOCKED | locked
 *
 * (r)  SGL (Signal)
 *                   SGL   |   description
 *                ---------+-------------------
 *                 DETECTED| signal detected
 *               UNDETECTED| no signal detected
\*-------------------------------------------------------------------------*/
#define _VD_STATUS1_OFFSET                   (0x0008u)
#define _VD_STATUS1_ADDRH(h)                 VD_ADDRH(h,STATUS1)
#define _VD_STATUS1_LEN                      (8)                 /* 8-bit*/
#define _VD_STATUS1_TYPE                     uint8_t

#define _VD_STATUS1_MVCS_MASK                (0xE0u)
#define _VD_STATUS1_MVCS_SHIFT               (5)
#define  VD_STATUS1_MVCS_DEFAULT             (0x00u)
#define  VD_STATUS1_MVCS_OF(x)               _CCL_VALUEOF(_VD_STATUS1_TYPE,x)

#define _VD_STATUS1_MVVD_MASK                (0x10u)
#define _VD_STATUS1_MVVD_SHIFT               (4)
#define  VD_STATUS1_MVVD_DEFAULT             (0x00u)
#define  VD_STATUS1_MVVD_OF(x)               _CCL_VALUEOF(_VD_STATUS1_TYPE,x)
#define  VD_STATUS1_MVVD_UNDETECTED          (0x00u)
#define  VD_STATUS1_MVVD_DETECTED            (0x01u)

#define _VD_STATUS1_CL_MASK                  (0x08u)
#define _VD_STATUS1_CL_SHIFT                 (3)
#define  VD_STATUS1_CL_DEFAULT               (0x00u)
#define  VD_STATUS1_CL_OF(x)                 _CCL_VALUEOF(_VD_STATUS1_TYPE,x)
#define  VD_STATUS1_CL_UNLOCKED              (0x00u)
#define  VD_STATUS1_CL_LOCKED                (0x01u)

#define _VD_STATUS1_VL_MASK                  (0x04u)
#define _VD_STATUS1_VL_SHIFT                 (2)
#define  VD_STATUS1_VL_DEFAULT               (0x00u)
#define  VD_STATUS1_VL_OF(x)                 _CVL_VALUEOF(_VD_STATUS1_TYPE,x)
#define  VD_STATUS1_VL_UNLOCKED              (0x00u)
#define  VD_STATUS1_VL_LOCKED                (0x01u)

#define _VD_STATUS1_HL_MASK                  (0x02u)
#define _VD_STATUS1_HL_SHIFT                 (1)
#define  VD_STATUS1_HL_DEFAULT               (0x00u)
#define  VD_STATUS1_HL_OF(x)                 _CHL_VALUEOF(_VD_STATUS1_TYPE,x)
#define  VD_STATUS1_HL_UNLOCKED              (0x00u)
#define  VD_STATUS1_HL_LOCKED                (0x01u)

#define _VD_STATUS1_SGL_MASK                 (0x01u)
#define _VD_STATUS1_SGL_SHIFT                (0)
#define  VD_STATUS1_SGL_DEFAULT              (0x00u)
#define  VD_STATUS1_SGL_OF(x)                _CCL_VALUEOF(_VD_STATUS1_TYPE,x)
#define  VD_STATUS1_SGL_DETECTED             (0x00u)
#define  VD_STATUS1_SGL_UNDETECTED           (0x01u)


#define  VD_STATUS1_OF(x)             _CCL_VALUEOF(_VD_STATUS1_TYPE,x)

#define  VD_STATUS1_DEFAULT (_VD_STATUS1_TYPE)(  \
	 _CCL_PER_FDEFAULT(VD,STATUS1,MVCS)          \
	|_CCL_PER_FDEFAULT(VD,STATUS1,MVVD)          \
	|_CCL_PER_FDEFAULT(VD,STATUS1,CL)            \
	|_CCL_PER_FDEFAULT(VD,STATUS1,VL)            \
	|_CCL_PER_FDEFAULT(VD,STATUS1,HL)            \
	|_CCL_PER_FDEFAULT(VD,STATUS1,SGL)           \
	)


/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  OUTPUT_CTRL      |
 * |___________________|
 *
 * OUTPUT_CTRL - VIDEO_OUTPUT_CTRL_REG
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  SFE   (Snow Freerun Enable)
 *                   SFE   |   description
 *                ---------+-------------------
 *                 DISABLE | disable
 *                  ENABLE | enable
 *
 * (rw)  CSWP  (Cb/Cr Swap)
 *                  CSWP   |   description
 *                ---------+-------------------
 *                 DISABLE | don't swap Cb/Cr
 *                  ENABLE | swap Cb/Cr
 *
 * (rw)  BM  (Blue Mode)
 *                   BM    |   description
 *                ---------+-------------------
 *                 DISABLE | disable
 *                  ENABLE | enable
 *                  AUTO   | auto
 *
 * (rw)  YCD (YC Delay) - 2's complement controls output delay between luma and chroma.
 *                   SFE   |   description
 *                ---------+-------------------
 *                 DISABLE | disable
 *                  ENABLE | enable
 *
\*-------------------------------------------------------------------------*/
#define _VD_OUTPUT_CTRL_OFFSET                   (0x003Cu)
#define _VD_OUTPUT_CTRL_ADDRH(h)                 VD_ADDRH(h,OUTPUT_CTRL)
#define _VD_OUTPUT_CTRL_LEN                      (8)                 /* 8-bit*/
#define _VD_OUTPUT_CTRL_TYPE                     uint8_t

#define _VD_OUTPUT_CTRL_SFE_MASK                 (0x80u)
#define _VD_OUTPUT_CTRL_SFE_SHIFT                (7)
#define  VD_OUTPUT_CTRL_SFE_DEFAULT              (0x01u)
#define  VD_OUTPUT_CTRL_SFE_OF(x)                _CCL_VALUEOF(_VD_OUTPUT_CTRL_TYPE,x)
#define  VD_OUTPUT_CTRL_SFE_DISABLE              (0x00u)
#define  VD_OUTPUT_CTRL_SFE_ENABLE               (0x01u)

#define _VD_OUTPUT_CTRL_CSWP_MASK                (0x40u)
#define _VD_OUTPUT_CTRL_CSWP_SHIFT               (6)
#define  VD_OUTPUT_CTRL_CSWP_DEFAULT             (0x00u)
#define  VD_OUTPUT_CTRL_CSWP_OF(x)               _CCL_VALUEOF(_VD_OUTPUT_CTRL_TYPE,x)
#define  VD_OUTPUT_CTRL_CSWP_DISABLE             (0x00u)
#define  VD_OUTPUT_CTRL_CSWP_ENABLE              (0x01u)

#define _VD_OUTPUT_CTRL_BM_MASK                  (0x30u)
#define _VD_OUTPUT_CTRL_BM_SHIFT                 (4)
#define  VD_OUTPUT_CTRL_BM_DEFAULT               (0x02u)
#define  VD_OUTPUT_CTRL_BM_OF(x)                 _CCL_VALUEOF(_VD_OUTPUT_CTRL_TYPE,x)
#define  VD_OUTPUT_CTRL_BM_DISABLE               (0x00u)
#define  VD_OUTPUT_CTRL_BM_ENABLE                (0x01u)
#define  VD_OUTPUT_CTRL_BM_AUTO                  (0x02u)

#define _VD_OUTPUT_CTRL_YCD_MASK                 (0x0Fu)
#define _VD_OUTPUT_CTRL_YCD_SHIFT                (0)
#define  VD_OUTPUT_CTRL_YCD_DEFAULT              (0x00u)
#define  VD_OUTPUT_CTRL_YCD_OF(x)                _CCL_VALUEOF(_VD_OUTPUT_CTRL_TYPE,x)


#define  VD_OUTPUT_CTRL_OF(x)             _CCL_VALUEOF(_VD_OUTPUT_CTRL_TYPE,x)

#define  VD_OUTPUT_CTRL_DEFAULT (_VD_OUTPUT_CTRL_TYPE)(  \
	 _CCL_PER_FDEFAULT(VD,OUTPUT_CTRL,SFE)               \
	|_CCL_PER_FDEFAULT(VD,OUTPUT_CTRL,CSWP)              \
	|_CCL_PER_FDEFAULT(VD,OUTPUT_CTRL,BM)                \
	|_CCL_PER_FDEFAULT(VD,OUTPUT_CTRL,YCD)               \
	)

#define VD_OUTPUT_CTRL_RMK(sfe, cswp, bm, ycd) (_VD_OUTPUT_CTRL_TYPE)( \
	 _CCL_PER_FMK  (VD,OUTPUT_CTRL,SFE ,sfe )    \
	|_CCL_PER_FMK  (VD,OUTPUT_CTRL,CSWP,cswp)    \
	|_CCL_PER_FMK  (VD,OUTPUT_CTRL,BM  ,bm  )    \
	|_CCL_PER_FMK  (VD,OUTPUT_CTRL,YCD ,ycd )    \
	)

#define VD_OUTPUT_CTRL_RMKS(sfe, cswp, bm, ycd) (_VD_OUTPUT_CTRL_TYPE)( \
	 _CCL_PER_FMKS (VD,OUTPUT_CTRL,SFE ,sfe )    \
	|_CCL_PER_FMKS (VD,OUTPUT_CTRL,CSWP,cswp)    \
	|_CCL_PER_FMKS (VD,OUTPUT_CTRL,BM  ,bm  )    \
	|_CCL_PER_FMK  (VD,OUTPUT_CTRL,YCD ,ycd )    \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * | AGC_GATE_TH_SWAP  |
 * |___________________|
 *
 * AGC_GATE_TH_SWAP -
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  ADC_UPDN_SWAP - swaps the DC clamp up/down controls to the analog front-end
 *                 ADC_UPDN_SWAP |   description
 *                ---------------+-------------------
 *                    DISABLE    | don't swap
 *                    ENABLE     | swap
 *
 * (rw)  ADC_INPUT_SWAP - swaps the MSBs and LSBs from the analog front-end’s ADC.
 *                 ADC_INPUT_SWAP |   description
 *                ----------------+-------------------
 *                    DISABLE     | don't swap
 *                    ENABLE      | swap
 *
 * (rw)  ADC_CBCR_PUMP_SWAP - swaps the Pb/Pr charge pump pairs to analog front-end.
 *                 ADC_CBCR_PUMP_SWAP |   description
 *                --------------------+-------------------
 *                      DISABLE       | don't swap
 *                      ENABLE        | swap
 *
 * (rw)  AGC_GATE_THRESH - threshold at which the rough gate generator creates a sync gate.
\*-------------------------------------------------------------------------*/
#define _VD_AGC_GATE_TH_SWAP_OFFSET                     (0x008Cu)
#define _VD_AGC_GATE_TH_SWAP_ADDRH(h)                   VD_ADDRH(h,AGC_GATE_TH_SWAP)
#define _VD_AGC_GATE_TH_SWAP_LEN                        (8)                 /* 8-bit*/
#define _VD_AGC_GATE_TH_SWAP_TYPE                       uint8_t

#define _VD_AGC_GATE_TH_SWAP_ADC_UPDN_SWAP_MASK         (0x80u)
#define _VD_AGC_GATE_TH_SWAP_ADC_UPDN_SWAP_SHIFT        (7)
#define  VD_AGC_GATE_TH_SWAP_ADC_UPDN_SWAP_DEFAULT      (0x01u)
#define  VD_AGC_GATE_TH_SWAP_ADC_UPDN_SWAP_OF(x)        _CCL_VALUEOF(_VD_AGC_GATE_TH_SWAP_TYPE,x)
#define  VD_AGC_GATE_TH_SWAP_ADC_UPDN_SWAP_DISABLE      (0x00u)
#define  VD_AGC_GATE_TH_SWAP_ADC_UPDN_SWAP_ENABLE       (0x01u)

#define _VD_AGC_GATE_TH_SWAP_ADC_INPUT_SWAP_MASK        (0x40u)
#define _VD_AGC_GATE_TH_SWAP_ADC_INPUT_SWAP_SHIFT       (6)
#define  VD_AGC_GATE_TH_SWAP_ADC_INPUT_SWAP_DEFAULT     (0x00u)
#define  VD_AGC_GATE_TH_SWAP_ADC_INPUT_SWAP_OF(x)       _CCL_VALUEOF(_VD_AGC_GATE_TH_SWAP_TYPE,x)
#define  VD_AGC_GATE_TH_SWAP_ADC_INPUT_SWAP_DISABLE     (0x00u)
#define  VD_AGC_GATE_TH_SWAP_ADC_INPUT_SWAP_ENABLE      (0x01u)

#define _VD_AGC_GATE_TH_SWAP_ADC_CBCR_PUMP_SWAP_MASK    (0x20u)
#define _VD_AGC_GATE_TH_SWAP_ADC_CBCR_PUMP_SWAP_SHIFT   (5)
#define  VD_AGC_GATE_TH_SWAP_ADC_CBCR_PUMP_SWAP_DEFAULT (0x00u)
#define  VD_AGC_GATE_TH_SWAP_ADC_CBCR_PUMP_SWAP_OF(x)   _CCL_VALUEOF(_VD_AGC_GATE_TH_SWAP_TYPE,x)
#define  VD_AGC_GATE_TH_SWAP_ADC_CBCR_PUMP_SWAP_DISABLE (0x00u)
#define  VD_AGC_GATE_TH_SWAP_ADC_CBCR_PUMP_SWAP_ENABLE  (0x01u)

#define _VD_AGC_GATE_TH_SWAP_AGC_GATE_THRESH_MASK       (0x1Fu)
#define _VD_AGC_GATE_TH_SWAP_AGC_GATE_THRESH_SHIFT      (0)
#define  VD_AGC_GATE_TH_SWAP_AGC_GATE_THRESH_DEFAULT    (0x0Au)
#define  VD_AGC_GATE_TH_SWAP_AGC_GATE_THRESH_OF(x)      _CCL_VALUEOF(_VD_AGC_GATE_TH_SWAP_TYPE,x)


#define  VD_AGC_GATE_TH_SWAP_OF(x)             _CCL_VALUEOF(_VD_AGC_GATE_TH_SWAP_TYPE,x)

#define  VD_AGC_GATE_TH_SWAP_DEFAULT (_VD_AGC_GATE_TH_SWAP_TYPE)(  \
	 _CCL_PER_FDEFAULT(VD,AGC_GATE_TH_SWAP,ADC_UPDN_SWAP     )     \
	|_CCL_PER_FDEFAULT(VD,AGC_GATE_TH_SWAP,ADC_INPUT_SWAP    )     \
	|_CCL_PER_FDEFAULT(VD,AGC_GATE_TH_SWAP,ADC_CBCR_PUMP_SWAP)     \
	|_CCL_PER_FDEFAULT(VD,AGC_GATE_TH_SWAP,AGC_GATE_THRESH   )     \
	)

#define VD_AGC_GATE_TH_SWAP_RMK(adc_updn_swap, adc_input_swap, adc_cbcr_pump_swap, agc_gate_thresh) (_VD_AGC_GATE_TH_SWAP_TYPE)( \
	 _CCL_PER_FMK  (VD,AGC_GATE_TH_SWAP,ADC_UPDN_SWAP     ,adc_updn_swap     )    \
	|_CCL_PER_FMK  (VD,AGC_GATE_TH_SWAP,ADC_INPUT_SWAP    ,adc_input_swap    )    \
	|_CCL_PER_FMK  (VD,AGC_GATE_TH_SWAP,ADC_CBCR_PUMP_SWAP,adc_cbcr_pump_swap)    \
	|_CCL_PER_FMK  (VD,AGC_GATE_TH_SWAP,AGC_GATE_THRESH   ,agc_gate_thresh   )    \
	)

#define VD_AGC_GATE_TH_SWAP_RMKS(adc_updn_swap, adc_input_swap, adc_cbcr_pump_swap, agc_gate_thresh) (_VD_AGC_GATE_TH_SWAP_TYPE)( \
	 _CCL_PER_FMKS (VD,AGC_GATE_TH_SWAP,ADC_UPDN_SWAP     ,adc_updn_swap     )    \
	|_CCL_PER_FMKS (VD,AGC_GATE_TH_SWAP,ADC_INPUT_SWAP    ,adc_input_swap    )    \
	|_CCL_PER_FMKS (VD,AGC_GATE_TH_SWAP,ADC_CBCR_PUMP_SWAP,adc_cbcr_pump_swap)    \
	|_CCL_PER_FMK  (VD,AGC_GATE_TH_SWAP,AGC_GATE_THRESH   ,agc_gate_thresh   )    \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  R E S E T        |
 * |___________________|
 *
 * RESET - VIDEO_RESET_REG
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  SOFT  - Software Reset
 *                   SOFT  |   description
 *                ---------+-------------------
 *                 DEASSERT| no reset
 *                  ASSERT | Reset
\*-------------------------------------------------------------------------*/
#define _VD_RESET_OFFSET                   (0x00FCu)
#define _VD_RESET_ADDRH(h)                 VD_ADDRH(h,RESET)
#define _VD_RESET_LEN                      (8)                 /* 8-bit*/
#define _VD_RESET_TYPE                     uint8_t

#define _VD_RESET_SOFT_MASK                (0x01u)
#define _VD_RESET_SOFT_SHIFT               (0)
#define  VD_RESET_SOFT_DEFAULT             (0x00u)
#define  VD_RESET_SOFT_OF(x)               _CCL_VALUEOF(_VD_RESET_TYPE,x)
#define  VD_RESET_SOFT_DEASSERT            (0x00u)
#define  VD_RESET_SOFT_ASSERT              (0x01u)

#define  VD_RESET_OF(x)             _CCL_VALUEOF(_VD_RESET_TYPE,x)

#define  VD_RESET_DEFAULT (_VD_RESET_TYPE)(  \
	 _CCL_PER_FDEFAULT(VD,RESET,SOFT)        \
	)

#define VD_RESET_RMK(soft) (_VD_RESET_TYPE)( \
	 _CCL_PER_FMK  (VD,RESET,SOFT,soft)      \
	)

#define VD_RESET_RMKS(soft) (_VD_RESET_TYPE)( \
	 _CCL_PER_FMKS (VD,RESET,SOFT,soft)       \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  HSYNC_RISING     |
 * |___________________|
 *
 * HSYNC_RISING - VIDEO_HSYNC_RISING_REG
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  VALUE  - Value of HSync Rising
\*-------------------------------------------------------------------------*/
#define _VD_HSYNC_RISING_OFFSET                   (0x0100u)
#define _VD_HSYNC_RISING_ADDRH(h)                 VD_ADDRH(h,HSYNC_RISING)
#define _VD_HSYNC_RISING_LEN                      (8)                 /* 8-bit*/
#define _VD_HSYNC_RISING_TYPE                     uint8_t

#define _VD_HSYNC_RISING_VALUE_MASK               (0xFFu)
#define _VD_HSYNC_RISING_VALUE_SHIFT              (0)
#define  VD_HSYNC_RISING_VALUE_DEFAULT            (0x3Eu)
#define  VD_HSYNC_RISING_VALUE_OF(x)              _CCL_VALUEOF(_VD_HSYNC_RISING_TYPE,x)

#define  VD_HSYNC_RISING_OF(x)             _CCL_VALUEOF(_VD_HSYNC_RISING_TYPE,x)

#define  VD_HSYNC_RISING_DEFAULT (_VD_HSYNC_RISING_TYPE)(  \
	 _CCL_PER_FDEFAULT(VD,HSYNC_RISING,VALUE)              \
	)

#define VD_HSYNC_RISING_RMK(value) (_VD_HSYNC_RISING_TYPE)( \
	 _CCL_PER_FMK  (VD,HSYNC_RISING,VALUE,value)            \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  HSYNC_GATE_END   |
 * |___________________|
 *
 * HSYNC_GATE_END - VIDEO_HSYNC_GATE_END_REG
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  VALUE  - Value of HSync Gate End
\*-------------------------------------------------------------------------*/
#define _VD_HSYNC_GATE_END_OFFSET                   (0x010Cu)
#define _VD_HSYNC_GATE_END_ADDRH(h)                 VD_ADDRH(h,HSYNC_GATE_END)
#define _VD_HSYNC_GATE_END_LEN                      (8)                 /* 8-bit*/
#define _VD_HSYNC_GATE_END_TYPE                     uint8_t

#define _VD_HSYNC_GATE_END_VALUE_MASK               (0xFFu)
#define _VD_HSYNC_GATE_END_VALUE_SHIFT              (0)
#define  VD_HSYNC_GATE_END_VALUE_DEFAULT            (0x80u)
#define  VD_HSYNC_GATE_END_VALUE_OF(x)              _CCL_VALUEOF(_VD_HSYNC_GATE_END_TYPE,x)

#define  VD_HSYNC_GATE_END_OF(x)             _CCL_VALUEOF(_VD_HSYNC_GATE_END_TYPE,x)

#define  VD_HSYNC_GATE_END_DEFAULT (_VD_HSYNC_GATE_END_TYPE)(  \
	 _CCL_PER_FDEFAULT(VD,HSYNC_GATE_END,VALUE)                \
	)

#define VD_HSYNC_GATE_END_RMK(value) (_VD_HSYNC_GATE_END_TYPE)( \
	 _CCL_PER_FMK  (VD,HSYNC_GATE_END,VALUE,value)              \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  FIFO_CTRL        |
 * |___________________|
 *
 * FIFO_CTRL - VIDEO_FIFO_CTRL_REG
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  V8FMT  - Ccir656 output format select.
 *                  V8FMT  |   description
 *                ---------+---------------------------------
 *                   ORIG  | original (follow 656 spec)
 *                  DIRECT | decode directly from hactive/vactive.
 *
 * (rw)  PSWAP  - phase up/down swap
 *                  PSWAP  |   description
 *                ---------+---------------------------------
 *                 NO_SWAP | don't swap
 *                   SWAP  | swap
 *
 * (rw)  INIT  - when set, initial FIFO control state to initial state.
 *                  INIT   |   description
 *                ---------+---------------------------------
 *                DEASSERT | when overflow/underflow happens, FIFO R/W flag reset and return state to 0
 *                 ASSERT  | when overflow/underflow happens, minus/plus FIFO R/W flag to escape overflow/underflow status
\*-------------------------------------------------------------------------*/
#define _VD_FIFO_CTRL_OFFSET                   (0x0390u)
#define _VD_FIFO_CTRL_ADDRH(h)                 VD_ADDRH(h,FIFO_CTRL)
#define _VD_FIFO_CTRL_LEN                      (8)                 /* 8-bit*/
#define _VD_FIFO_CTRL_TYPE                     uint8_t

#define _VD_FIFO_CTRL_V8FMT_MASK               (0x80u)
#define _VD_FIFO_CTRL_V8FMT_SHIFT              (7)
#define  VD_FIFO_CTRL_V8FMT_DEFAULT            (0x1u)
#define  VD_FIFO_CTRL_V8FMT_OF(x)              _CCL_VALUEOF(_VD_FIFO_CTRL_TYPE,x)
#define  VD_FIFO_CTRL_V8FMT_ORIG               (0x0u)
#define  VD_FIFO_CTRL_V8FMT_DIRECT             (0x1u)

#define _VD_FIFO_CTRL_PSWAP_MASK               (0x04u)
#define _VD_FIFO_CTRL_PSWAP_SHIFT              (2)
#define  VD_FIFO_CTRL_PSWAP_DEFAULT            (0x00u)
#define  VD_FIFO_CTRL_PSWAP_OF(x)              _CCL_VALUEOF(_VD_FIFO_CTRL_TYPE,x)
#define  VD_FIFO_CTRL_PSWAP_NO_SWAP            (0x0u)
#define  VD_FIFO_CTRL_PSWAP_SWAP               (0x1u)

#define _VD_FIFO_CTRL_INIT_MASK               (0x01u)
#define _VD_FIFO_CTRL_INIT_SHIFT              (0)
#define  VD_FIFO_CTRL_INIT_DEFAULT            (0x00u)
#define  VD_FIFO_CTRL_INIT_OF(x)              _CCL_VALUEOF(_VD_FIFO_CTRL_TYPE,x)
#define  VD_FIFO_CTRL_INIT_DEASSERT           (0x0u)
#define  VD_FIFO_CTRL_INIT_ASSERT             (0x1u)

#define  VD_FIFO_CTRL_OF(x)             _CCL_VALUEOF(_VD_FIFO_CTRL_TYPE,x)

#define  VD_FIFO_CTRL_DEFAULT (_VD_FIFO_CTRL_TYPE)(  \
	 _CCL_PER_FDEFAULT(VD,FIFO_CTRL,V8FMT)           \
	|_CCL_PER_FDEFAULT(VD,FIFO_CTRL,PSWAP)           \
	|_CCL_PER_FDEFAULT(VD,FIFO_CTRL,INIT )           \
	)

#define VD_FIFO_CTRL_RMK(v8fmt, pswap, init) (_VD_FIFO_CTRL_TYPE)( \
	 _CCL_PER_FMK  (VD,FIFO_CTRL,V8FMT,v8fmt)                      \
	|_CCL_PER_FMK  (VD,FIFO_CTRL,PSWAP,pswap)                      \
	|_CCL_PER_FMK  (VD,FIFO_CTRL,INIT ,init )                      \
	)

#define VD_FIFO_CTRL_RMKS(v8fmt, pswap, init) (_VD_FIFO_CTRL_TYPE)( \
	 _CCL_PER_FMKS (VD,FIFO_CTRL,V8FMT,v8fmt)                       \
	|_CCL_PER_FMKS (VD,FIFO_CTRL,PSWAP,pswap)                       \
	|_CCL_PER_FMKS (VD,FIFO_CTRL,INIT ,init )                       \
	)

/*-------------------------------------------------------------------------*\
 * _____________________
 * |                   |
 * |  FIFO_STATUS      |
 * |___________________|
 *
 * FIFO_STATUS - VIDEO_FIFO_STATUS_REG
 *
 *-------------------------------------------------------------------------
 * FIELDS (msb -> lsb)
 *
 * (rw)  FULL  - FIFO full status
 *                   FULL  |   description
 *                ---------+---------------------------------
 *                   FULL  | FIFO is full             (read)
 *                 NOT_FULL| FIFO is not full         (read)
 *                   NOP   | no operation             (write)
 *                   CLEAR | reset status to NOT_FULL (write)
 *
 * (rw)  EMPTY  - FIFO empty status
 *                  EMPTY  |   description
 *                ---------+---------------------------------
 *                   EMPTY | FIFO is empty             (read)
 *                NOT_EMPTY| FIFO is not empty         (read)
 *                   NOP   | no operation             (write)
 *                   CLEAR | reset status to NOT_EMPTY (write)
\*-------------------------------------------------------------------------*/
#define _VD_FIFO_STATUS_OFFSET                   (0x0394u)
#define _VD_FIFO_STATUS_ADDRH(h)                 VD_ADDRH(h,FIFO_STATUS)
#define _VD_FIFO_STATUS_LEN                      (8)                 /* 8-bit*/
#define _VD_FIFO_STATUS_TYPE                     uint8_t

#define _VD_FIFO_STATUS_FULL_MASK                (0x02u)
#define _VD_FIFO_STATUS_FULL_SHIFT               (1)
#define  VD_FIFO_STATUS_FULL_DEFAULT             (0x00u)
#define  VD_FIFO_STATUS_FULL_OF(x)               _CCL_VALUEOF(_VD_FIFO_STATUS_TYPE,x)
#define  VD_FIFO_STATUS_FULL_FULL                (0x0u)
#define  VD_FIFO_STATUS_FULL_NOT_FULL            (0x1u)
#define  VD_FIFO_STATUS_FULL_NOP                 (0x0u)
#define  VD_FIFO_STATUS_FULL_CLEAR               (0x1u)

#define _VD_FIFO_STATUS_EMPTY_MASK               (0x01u)
#define _VD_FIFO_STATUS_EMPTY_SHIFT              (0)
#define  VD_FIFO_STATUS_EMPTY_DEFAULT            (0x00u)
#define  VD_FIFO_STATUS_EMPTY_OF(x)              _CCL_VALUEOF(_VD_FIFO_STATUS_TYPE,x)
#define  VD_FIFO_STATUS_EMPTY_EMPTY              (0x0u)
#define  VD_FIFO_STATUS_EMPTY_NOT_EMPTY          (0x1u)
#define  VD_FIFO_STATUS_EMPTY_NOP                (0x0u)
#define  VD_FIFO_STATUS_EMPTY_CLEAR              (0x1u)

#define  VD_FIFO_STATUS_OF(x)             _CCL_VALUEOF(_VD_FIFO_STATUS_TYPE,x)

#define  VD_FIFO_STATUS_DEFAULT (_VD_FIFO_STATUS_TYPE)(  \
	 _CCL_PER_FDEFAULT(VD,FIFO_STATUS,FULL )             \
	|_CCL_PER_FDEFAULT(VD,FIFO_STATUS,EMPTY)             \
	)

#define VD_FIFO_STATUS_RMK(full, empty) (_VD_FIFO_STATUS_TYPE)( \
	 _CCL_PER_FMK  (VD,FIFO_STATUS,FULL ,full )                 \
	|_CCL_PER_FMK  (VD,FIFO_STATUS,EMPTY,empty)                 \
	)

#define VD_FIFO_STATUS_RMKS(full, empty) (_VD_FIFO_STATUS_TYPE)( \
	 _CCL_PER_FMKS (VD,FIFO_STATUS,FULL ,full )                  \
	|_CCL_PER_FMKS (VD,FIFO_STATUS,EMPTY,empty)                  \
	)



#endif	/* !__C_CHIP_LIB_REALTEK_RTD_2668_VIDEO_DECODER_HARDWARE_ABSTRACT_LAYER_HEADER_INCLUDED__ */


/*-------------------------------------------------------------------------*\
 * End of vd_hal.h                                                        *
\*-------------------------------------------------------------------------*/

