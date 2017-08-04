/*===========================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2005
 * All rights reserved.
 * ========================================================== */

/*==================== File Description ===================== */
/**
 * @file
 * 	This file is a osd message definition file.
 *
 * @author 	$Author: leo_liu $
 * @date 	$Date: 2006-05-23 18:15:17 +0800 (星期�? 23 五月 2006) $
 * @version	$Revision: 812 $
 */

/**
 * @addtogroup vbi
 * @{
 */
#ifndef _OSDVBI_H
#define _OSDVBI_H

/*===================== Module Dependency  ================== */
#include "sparrow.h"
#include "osdobjectdef.h"
#include "panel\panelapi.h"

#ifdef CONFIG_VBI_CC_SUPPORT
/*========================== Types ========================== */

/*======================== Definitions ====================== */
#define SET_OSDVBI_INITIAL() (ucOsdVbiControl |= _BIT0)
#define GET_OSDVBI_INITIAL() (ucOsdVbiControl & _BIT0)
#define CLR_OSDVBI_INITIAL() (ucOsdVbiControl &= (~_BIT0))

#define SET_OSDVBI_HADSHOWED() (ucOsdVbiControl |= _BIT2)
#define CLR_OSDVBI_HADSHOWED() (ucOsdVbiControl &= (~_BIT2))
#define GET_OSDVBI_HADSHOWED() ((ucOsdVbiControl & _BIT2) >> 2)
#define GET_CLOSEDCAPTION()  (Ccsel)
#define SET_CLOSEDCAPTION(x)  (Ccsel=x)

#define _OSDVBI_CHARACTER_DOUBLE_WIDTH 1
#define _OSDVBI_CHARACTER_DOUBLE_HEIGHT 1

#ifndef _OSDVBI_ROWHEIGNT
#define _OSDVBI_ROWHEIGNT 24
#endif

#ifndef _OSDVBI_CHARACTERSPACE
#define _OSDVBI_CHARACTERSPACE 2
#endif

#ifndef _OSDVBI_ROWCOUNT
#define _OSDVBI_ROWCOUNT	17
#endif

#ifndef _OSDVBI_ROW_NUMBER_INCOLUMNSPACE
#define _OSDVBI_ROW_NUMBER_INCOLUMNSPACE 32
#endif

#ifndef _OSDVBI_ROW_LEN_INCOLUMNSPACE
#define _OSDVBI_ROW_LEN_INCOLUMNSPACE 34
#endif

#ifndef _OSDVBI_FONTTABLE_STARTADDRESS
#define _OSDVBI_FONTTABLE_STARTADDRESS (_OSDVBI_ROWCOUNT + 1 + _OSDVBI_ROW_NUMBER_INCOLUMNSPACE * _OSDVBI_ROW_LEN_INCOLUMNSPACE)
#endif

#ifndef _OSDVBI_ROLLUP_NUM
#define _OSDVBI_ROLLUP_NUM  12
#endif

#ifndef _OSDVBI_ROLLUP_STEP
#define _OSDVBI_ROLLUP_STEP (_OSDVBI_ROWHEIGNT / _OSDVBI_ROLLUP_NUM)
#endif

#ifndef _OSDVBI_DOUBLEBUFFER_ROW
#define _OSDVBI_DOUBLEBUFFER_ROW	 16
#endif

#ifndef _OSDVBI_FIRSTBLOCK_DOUBLEBUFFER
#define _OSDVBI_FIRSTBLOCK_DOUBLEBUFFER	 (_OSDVBI_ROWCOUNT + 1)
#endif

#ifndef _OSDVBI_SECOND_DOUBLEBUFFER
#define _OSDVBI_SECOND_DOUBLEBUFFER	 ( _OSDVBI_ROWCOUNT + 1 + _OSDVBI_DOUBLEBUFFER_ROW * _OSDVBI_ROW_LEN_INCOLUMNSPACE)
#endif

#ifndef _OSDVBI_CLOSESHOW_TIME
#define _OSDVBI_CLOSESHOW_TIME 10
#endif

#define _OSD_VBI_H_POS (Get_DISP_ACT_H_WID() - 14 * 34 * 2) /2 + 144
#define _OSD_VBI_V_POS (Get_DISP_ACT_V_LEN() - 24 * 15 * 2) / 2

/*=====================  Extern Variables =================== */
#if 0
#ifdef _OSDMESSAGE_C
#define EXTERN
#else
#define EXTERN   extern
#endif
#endif

extern UINT8  xdata ucOsdVbiControl;
// Bit0: whether initialize or not
// Bit1: Show Memory zone for double buffer (0 or 1)
// Bit2: Show or not
extern UINT8 xdata ucRollupTimerCnt;
extern UINT8 xdata Ccsel;

//#undef EXTERN

/*======================= Extern Functions ================== */
void COsdVbiInitial(void);
void COsdVbiInitialVariable(void);
void COsdVbiBlendingOperation(UINT8 on_off);
void COsdVbiShowData(UINT8 on_off);
void	COsdVbiSetDisplayMemory(UINT8 ucRow, UINT8 ucCol, UINT8 ucAttr, UINT8 ucColor, UINT8 ucData);
void	COsdVbiSetNonDisplayMemory(UINT8 ucRow, UINT8 ucCol, UINT8 ucAttr, UINT8 ucColor, UINT8 ucData);
void COsdVbiResetDisplayMemory(void);
void COsdVbiResetScrollUpEnv(void);
void COsdVbiSetScrollRow(void);
void COsdVbiResetScrollRow(void);
void COsdVbiRullUpProc(void);
void COsdVbiStopRullUpProc(void);
void COsdVbiSetRullUpStartRow(UINT8 ucStartRow, UINT8 ucRollNum);
void	COsdVbiClearDisplayMemory(void);
void	COsdVbiClearNonDisplayMemory(void);
void COsdVbiSwitchDisplayMemory(void);
void	COsdVbiClearAllMemory(void);
void COsdVbiClearRow(UINT8 row);
void COsdVbiSetShowRow(UINT8 ucStartRow, UINT8 ucNumber);
void COsdVbiStartAddress(UINT8 ucRow, UINT8 ucCol, UINT8 ucIndicate);
void CosdVbiDisableShow(void);
#endif

#endif
