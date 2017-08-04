/*===========================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2005
 * All rights reserved.
 * ========================================================== */

/*==================== File Description ===================== */
/**
 * @file
 * 	This file is for mode related functions.
 *
 * @author 	$Author: hillwang $
 * @date 	$Date: 2006-05-18 10:06:06 +0800 (星期四, 18 五月 2006) $
 * @version 	$Revision: 753 $
 * @ingroup vbi
 */

/**
 * @addtogroup vbi
 * @{
 */

#ifndef _CLOSECAPTION_H_
#define _CLOSECAPTION_H_

/*===================== Module dependency  ================== */
#include "sparrow.h"
#include "osdobjectdef.h"


#ifdef CONFIG_VBI_CC_SUPPORT

//#include "vbi.h"

/*======================== CONSTANTS =========================================*/
#define _Cc1Mode _BIT0
#define _Cc2Mode _BIT1
#define _Cc3Mode _BIT4
#define _Cc4Mode _BIT5
#define _T1Mode _BIT2
#define _T2Mode _BIT3
#define _T3Mode _BIT6
#define _T4Mode _BIT7


#define _ROWS_16			0
#define _ROWS_32			1
#define _ROWS_NO_TYPE		_ROWS_16	//jerry0413A
#define _DER_COMMAND		_FALSE	//  _TRUE  : Enable "delete to end of row(der)" command
//  _FALSE : Disable "delete to end of row(der)" command
#define _TEXT_RESTART		_TRUE	//  _TRUE  : Enable "text restart/resume text display" function
//  _FALSE : Disable "text restart/resume text display" function(Dell-Type)
#define _LR_COL_MORE_PIXEL	_TRUE	//	_TRUE  : Enable Left/Right Column More Pixel
//	_FALSE : Disable Left/Right Column More Pixel
//#define _TRANSPARENT_ENABLE 1
/*======================== MACROS ============================================*/
#define Variable_BitValue(VARIABLE, BIT_POSITION) ((VARIABLE >> BIT_POSITION) & 0x01)
#define Variable_BitSet(VARIABLE, BIT_POSITION)  (VARIABLE |= (0x01 << BIT_POSITION))
#define Variable_BitClear(VARIABLE, BIT_POSITION) (VARIABLE &= (~(0x01 << BIT_POSITION)))

/*======================== TYPES =============================================*/

/*======================== VARIABLES =========================================*/
#ifdef _CLOSECAPTION_C
#define EXTERN
#else
#define EXTERN   extern
#endif

EXTERN UINT8 CCSystemState;
EXTERN UINT8 CaptionMode;
extern UINT8 xdata CC_System_Mode;		//VBI_ENABLE(osd)
//EXTERN unsigned char xdata	CC_Mode;			//@<Stanley> main & hot key osd using
//EXTERN UINT8 bCCInterrupt;						//VBI_INTERRUPT
extern UINT8 xdata bCcStartFlag;
extern UINT8 xdata bCheckCCStatus;
#define CCBuffer_Size 75//--4//10//80
extern UINT8 xdata  CCBuffer[CCBuffer_Size][2];	//VBI_INTERRUPT
EXTERN UINT8 xdata	ucCCWriteCnt;		//VBI_INTERRUPT
EXTERN UINT8 xdata 	ucCCReadCnt;		//VBI_INTERRUPT
extern UINT8 xdata bCCReceiveOK;						//VBI_INTERRUPT
//EXTERN unsigned char xdata 	ucCC_VSyncCnt;
extern UINT8 xdata bVBI_INT_Enable;
//EXTERN UINT8 bVideoStable;
//EXTERN UINT8 bYPbPrStable;
//EXTERN UINT8 bVCRStalbe;
extern UINT8 xdata bCCDecodeStalbe;
//EXTERN UINT8 bNotify_Timer1_VBI_Int;

#undef EXTERN



/*======================== FUNCTIONS =========================================*/
UINT8 CVbiIsrDisable(void);
void CCcSetSystemMode(void);

/*==================== Function  Prototypes ==================*/
/**
 * This function shall initialize the related VBI variables, including color palette,
 * screen memory, display memory.
 *
 *
 * @param 	None
 *
 * @return	None
 *
 * @ingroup 	vbi
 */
void CCcInitVariables(void);


/*==================== Function  Prototypes ==================*/
/**
 * This function shall initialize the VBI module including screen and display memories.
 *
 *
 * @param 	None
 *
 * @return	None
 *
 * @ingroup 	vbi
 */
void CCcInitial(void);

/*==================== Function  Prototypes ==================*/
/**
 * This function shall execute the VBI decode engine to parsing closecaption or teltext
 * control words and strings..
 *
 *
 * @param 	None
 *
 * @return	None
 *
 * @ingroup 	vbi
 */
void CCcDecoder(void);

/*==================== Function  Prototypes ==================*/
/**
 * This function enables the VBI module.
 *
 *
 * @param 	None
 *
 * @return	None
 *
 * @ingroup 	vbi
 */
void CCcEnable(void);

/*==================== Function  Prototypes ==================*/
/**
 * This function disables the VBI module.
 *
 *
 * @param 	None
 *
 * @return	None
 *
 * @ingroup 	vbi
 */
void CCcDisable(void);
#endif // VBI_ENABLE
#endif // _CLOSECAPTION_H_


/**
*
* @}
*/



