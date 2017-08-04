/*===========================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2009
 * All rights reserved.
 * ========================================================== */

/*==================== File Description ===================== */
/**
 * @file
 * 	This file is for mode related functions.
 *
 * @author 	$Author: $
 * @date 	$Date: 2009-03-13 $
 * @version $Revision: $
 */

/**
 * @addtogroup osd
 * @{
 */

#ifndef _OSDFONT_H
#define _OSDFONT_H
/*===================== Module Dependency  ================== */
#include "osdobjectdef.h"

/*===================== Module dependency  ================== */


/*======================== CONSTANTS =========================================*/
#define ___ 			0x01

#define _PLUS_ 			0x0B
#define _MINUS_ 		0x0D
#define _DOT_			0x0E
#define _SLASH_			0xDA
#define _COLON_			0x5E
#define _COMMENT_		0x0A
#define _CHECKED_		0xB1	// Heart symbol
#define _LOCK_			0xB4	// Locked symbol
#define _UNLOCK_		0xB5	// Unlock symbol
#define _STAR_			0x68	// symbol : '*'
#define _R_ARROW_		0x57
#define _LR_ARROW_		0x56	// double-direction arrow
#define _CROSS_			0x53
#define _UNDERLINE_		0x5f

#define _L_BRACKET_		0x56
#define _R_BRACKET_		0x57

#define _DOD_			0x5F


#define _FAC_L_ARROW_	0xDB
#define _FAC_R_ARROW_	0xDC
#define _PERCENT_		0xDD
#define _0_ 			0x30
#define _1_ 			0x31
#define _2_ 			0x32
#define _3_ 			0x33
#define _4_ 			0x34
#define _5_ 			0x35
#define _6_ 			0x36
#define _7_ 			0x37
#define _8_ 			0x38
#define _9_ 			0x39

#define _A_ 			0x10,0x11
#define _B_ 			0x12
#define _C_ 			0x13
#define _D_ 			0x14
#define _E_ 			0x15
#define _F_ 			0x16
#define _G_ 			0x17,0x18
#define _H_ 			0x19
#define _I_ 			0x1A
#define _J_ 			0x1B
#define _K_ 			0x1C
#define _L_ 			0x1D
#define _M_ 			0x1E,0x1F
#define _N_ 			0x20
#define _O_ 			0x21,0x22
#define _P_ 			0x23
#define _Q_ 			0x24,0x25
#define _R_ 			0x26
#define _S_ 			0x27
#define _T_ 			0x28
#define _U_ 			0x29
#define _V_ 			0x2A
#define _W_ 			0x2B,0x2C
#define _X_ 			0x2D
#define _Y_ 			0x2E
#define _Z_ 			0x2F

#define _a_ 			0x3A
#define _b_ 			0x3B
#define _c_ 			0x3C
#define _d_ 			0x3D
#define _e_ 			0x3E
#define _f_ 			0x3F
#define _g_ 			0x40
#define _h_ 			0x41
#define _i_ 			0x42
#define _j_ 			0x43
#define _k_ 			0x44
#define _l_ 			0x45
#define _m_ 			0x46,0x47
#define _n_ 			0x48
#define _o_ 			0x49
#define _p_ 			0x4A
#define _q_ 			0x4B
#define _r_ 			0x4C
#define _s_ 			0x4D
#define _t_ 			0x4E
#define _u_ 			0x4F
#define _v_ 			0x50
#define _w_ 			0x51,0x52
#define _x_ 			0x53
#define _y_ 			0x54
#define _z_ 			0x55


/*
#define _Ar_			0x60	//A right dot(0x60)		//jerry0824 start
#define _Er_			0x61	//E right dot(0x61)
#define _Or_			0x62	//O right dot(0x62)
#define _Ur_			0x63	//U right dot(0x63)
#define _Ud_			0x64	//U double dot(0x64)
#define _ud_			0x65	//u double dot(0x65)

#define _Al_			0x70	//A left-dot(0x70)
#define _Au_			0x71	//A upper-dot(0x71)
#define _Cd_			0x72	//C down-dot(0x72)
#define _El_			0x73	//E left-dot(0x73)
#define _Eu_			0x74	//E upper-dot(0x74)
#define _Ed_			0x75	//E double-dot(0x75)
#define _ed_			0x76	//e double-dot(0x76)
#define _Iu_			0x77	//I upper-dot(0x77)
#define _Id_			0x78	//I double-dot(0x78)
#define _id_			0x79	//i double-dot(0x79)
#define _Ou_			0x7A	//O upper-dot(0x7A)
#define _Ul_			0x7B	//U left-dot(0x7B)
#define _ul_			0x7C	//u left-dot(0x7C)
#define _Uu_			0x7D	//U upper-dot(0x7D)

#define _As_			0x80	//A s-dot(0x80)
#define _as_			0x81	//a s-dot(0x81)
#define _Ir_			0x82	//I right-dot(0x82)
#define _Il_			0x83	//I left-dot(0x83)
#define _il_			0x84	//i left-dot(0x84)
#define _Ol_			0x85	//O left-dot(0x85)
#define _ol_			0x86	//o left-dot(0x86)
#define _Os_			0x87	//O s-dot(0x87)
#define _os_			0x88	//o s-dot(0x88)

#define _Ad_			0x90	//A double-dot(0x90)
#define _ad_			0x91	//a double-dot(0x91)
#define _Od_			0x92	//O double-dot(0x92)
#define _od_			0x93	//o double-dot(0x93)
#define _A0_			0x94	//A 0-dot(0x98)
#define _a0_			0x95	//a 0-dot(0x99)

#define _al_			0xA8	//a left-dot(0xA8)
#define _el_			0xAA	//e left-dot(0xAA)
#define _au_			0xAB	//a upper-dot(0xAB)
#define _eu_			0xAC	//e upper-dot(0xAC)
#define _iu_			0xAD	//i upper-dot(0xAD)
#define _ou_			0xAE	//o upper-dot(0xAE)
#define _uu_			0xAF	//u upper-dot(0xAF)
*/

/*
 This flag (_THE_SAME_) means the OSD keyword in the language
     is the same as in English
*/
#define _SPC_			0xfa
#define _THE_SAME_		0xfb
#define _EXT_   		0xfc
#define _REPEAT_		0xfd
#define _NEXT_  		0xfe
#define _END_   		0xff

/*======================== MACROS ============================================*/

/*======================== TYPES =============================================*/

/*======================== VARIABLES =========================================*/

#if !defined(_OSDFONT_C)
extern UINT8 code tOSD_CHARWIDTH_TABLE[];

//extern UINT8 code tFont_MainIcons[];
extern UINT8 code tfont_2biticon[];

extern UINT8 code FntColorEng_Title[];
extern UINT8 code FntColorCHIS_Title[] ;
extern UINT8 code FntColorKOR_Title[] ;
extern UINT8 code FntColorJAPAN_Title[];

extern UINT8 code FntSoundEng_Title[];
extern UINT8 code FntSoundCHI_Title[];
extern UINT8 code FntSoundKOR_Title[];
extern UINT8 code FntSoundJAPAN_Title[];

extern UINT8 code FntAdjustEng_Title[];
extern UINT8 code FntAdjustCHI_Title[];
extern UINT8 code FntAdjustKOR_Title[];


extern UINT8 code FntFunctionEng_Title[];
extern UINT8 code FntFunctionCHI_Title[];
extern UINT8 code FntFunctionKOR_Title[];
extern UINT8 code FntFunctionJAPAN_Title[];

extern UINT8 code FntTVEng_Title[];
extern UINT8 code FntTVCHI_Title[];

extern UINT8 code FntOSDEng_Title[];
extern UINT8 code FntOSDCHI_Title[];

extern STitle code  tMainTitle[][14];
extern UINT8 code tFont_MainIcon_01_4bits[];
extern UINT8 code tFont_MainIcon_02_4bits[];
extern UINT8 code tFont_MainIcon_03_4bits[];
extern UINT8 code tFont_MainIcon_04_4bits[];
extern UINT8 code tFont_MainIcon_05_4bits[];
extern UINT8 code FntJapan_colorPage_S[];
extern UINT8 code FntJAPAN_FunctionPage_S[];
extern UINT8 code FntJAPANTVPage_S[];

extern UINT8 code FntColorPage_S[];
extern UINT8 code FntAdjustPage_S[];

extern UINT8 code FntAudioPage_S[];
extern UINT8 code FntFuncPage_S[];
extern UINT8 code FntTVPage_S[];
extern UINT8 code FntOtherPage_S[];

extern UINT8 code FntKor[];
extern UINT8 code FntRus[];
extern UINT8 code tFont_MuteIcon[];
extern UINT8 code tFont_UnmuteIcon[];
extern UINT8 code FntLangaugeName[];
extern UINT8 code tFONT_GLOBAL[];
extern UINT8 code tFont_OTHERSIGN[];
extern UINT8 code tFont_LOGO[];
extern UINT8 code tFONT_WESTEUR[];
extern UINT8 code tFONT_WESTEUR[];


extern UINT8 code tFont_slider_2bit[];
extern UINT8 code tFont_PoponSlider[];
extern UINT8 code tFont_VolumeSlider[];
extern UINT8 code tFont_EQ_Slider[];

extern UINT8 code tFont_SourcePatch[];

#ifdef CONFIG_VBI_CC_SUPPORT
extern UINT8 code tFONT_GLOBAL_Italic[];
extern UINT8 code tFONT_WESTEUR[];
#endif

//extern UINT8 code tFont_Logo[];


#endif // !defined(_OSDFONT_C)


/*======================== FUNCTIONS =========================================*/

#endif // #ifndef _OSDFONT_H

/**
 *
 * @}
 */

