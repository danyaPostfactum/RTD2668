/*===========================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2009
 * All rights reserved.
 * ========================================================== */

/*==================== File Description ===================== */
/**
 * @file
 * 	This file is osd implement declaration for mode related functions.
 *
 * @author 	$Author: cf.tsai $
 * @date 	$Date: 2009-03-09 $
 * @version $Revision: 10 $
 */

/**
 * @addtogroup osd
 * @{
 */

#ifndef _OSDFUNCTION_H
#define _OSDFUNCTION_H


/*===================== Module dependency  ================== */
#include "osdobjectdef.h"
#include "autoconf.h"

/*=======================  Types ===================== */

/*======================== Definitions ====================== */


//--------------------------------------------------
// Definations of Write Byte
//--------------------------------------------------
#define _WRITE_BYTE0			0
#define _WRITE_BYTE1			1
#define _WRITE_BYTE2			2
#define _WRITE_BYTEALL			3

//--------------------------------------------------
// OSD Global Macros
//--------------------------------------------------
#define LOBYTE(w)                       		((UINT8)(w))
#define HIBYTE(w)                       		((UINT8)(((UINT16)(w) >> 8) & 0x00FF))

#define SET_2BITSCOLOR(_c0, _c1, _c2, _c3)		{uc2BitColor[0]=(_c0);uc2BitColor[1]=(_c1);uc2BitColor[2]=(_c2);uc2BitColor[3]=(_c3);}
//#define MAKE_2BITS_BYTE0(palette_type)			((uc2BitColor[0]&0x04) >> 2 | (uc2BitColor[3]&0x07) << 1 | (uc2BitColor[0]&0x08) << 3 | (palette_type&0x01) << 4 | _BIT7 | _BIT5)
#define MAKE_2BITS_BYTE0()			((uc2BitColor[3]&_BIT8) >> 6 | (uc2BitColor[2]&_BIT8) << 4 | (uc2BitColor[3]&0x07) << 1 |(uc2BitColor[0]&_BIT2)  | _BIT7 | _BIT5)
#define MAKE_2BITS_BYTE2()						((uc2BitColor[0]&0x03) << 6 | (uc2BitColor[2]&0x07) << 3 | (uc2BitColor[1]&0x07))

#define SET_2BITS_FONT_PALETTE_0()				uc2BitColor_Palette = 0
#define SET_2BITS_FONT_PALETTE_1()				uc2BitColor_Palette = 1

/** 為了省 code size, 不做 bits 保護, 使用者自己要注意 */
#define FONT_COLOR(_fg, _bg)					((_fg) << 4 | (_bg))

/*========================== Variables ====================== */
#ifndef _OSDFUNCTION_C
extern UINT8 xdata uc2BitColor[4];
extern UINT8 xdata uwWindowCtrlByte2[10];
extern UINT8 xdata uwWindowCtrlByte1[10];
extern UINT8 xdata uc2BitColor_Palette;
extern void COsdFxDisableOsd(void);//For Keil Building
#endif

/*=================== Local Functions Phototype ==============*/


/*=========================== Functions ===================== */
void ap_osdfx_set_share_memory(void);
void ap_osdfx_power_on(UINT8 on);

// Enable/Disable OSD function
void COsdFxEnableOsd(void);
void COsdFxDisableOsd(void);
void COsdFxBlending(UINT8 ucLevel);

#define _OSD_BLENDING_DISABLE			0x00
#define _OSD_BLENDING_WINDOW_ONLY		0x01
#define _OSD_BLENDING_ALL				0x02
#define _OSD_BLENDING_WIN_AND_CHAR_BG	0x03
void ap_osdfx_set_blending_type(UINT8 type);

void ap_osdfx_font_shadow_color_set(UINT8 color);

void ap_osdfx_write_amount(UINT8 value, UINT8 len);
//void ap_osdfx_write_array(UINT8 *pArray, UINT8 len);
void ap_osdfx_code_write(UINT8 code *pArray);
void ap_osdfx_delayXms(UINT16 usMs);

void ap_osdfx_frame_write(UINT16 usAddr, UINT8 ucData, UINT8 ucIndicate);
UINT8 ap_osdfx_frame_read(UINT16 usAddr, UINT8 ucIndicate);

void COsdFxHLine(UINT8 ucRow, UINT8 ucCol, UINT8 ucLength, UINT8 ucValue, UINT8 ucIndicate);

void ap_osdfx_load_palette(void);
void ap_osdfx_load_palette16(UINT8 ucPaletteTable, UINT8* pColorPaletteArray);

void ap_osdfx_base_addr_set(UINT16 Font_Base, UINT16 Font_Select_Base);
void ap_osdfx_setmap(void);
void COsdFxSetRowCtrl1(UINT8 ucRow, UINT8 ucByte1);
void ap_osdfx_map_setting(UINT8 ucOsdmaptype);

void ap_osdfx_load_vlc_font(UINT8 code *pArray, UINT16 usStart, UINT16 usLength);
void OSDClear(UINT8 row_start, UINT8 height,UINT8 col_start, UINT8 width,UINT8 Value, UINT8 indicate);



// ** OSD window API
void COsdFxDrawWindow(WindowStyle* pStyle);
void COsdFxCloseWindow(UINT8 ucWinNumber);
void COsdFxCloseAllWindow(void);
void COsdFxMoveWindow(UINT8 ucWinNumber, ItemPosition* stPos);
#if 0
UINT8 COsdFxGetWindowEnable(UINT8 ucWindowID);
#else
#define COsdFxGetWindowEnable(_ucWindowID)		(uwWindowCtrlByte2[(_ucWindowID)]&_BIT0)
#endif
void COsdFxShowWindow(UINT8 ucWindowID, UINT8 ucShow);
#if defined(_REALTEK_OSD_USE)
void COsdFxOpenWindowEffect(UINT8 ucWindowID, UINT8 ucEnable);
#endif
//void COsdFxChangeWindowColor(UINT8 ucWindowID, UINT8 ucColor);

void COsdFxSetWinRefPos(UINT16 uiXPos, UINT16 uiYPos);
void ap_osdfx_calc_start_addr(UINT8 ucRow, UINT8 ucCol, UINT8 ucIndicate);

/** OSD text API */
UINT8 COsdFxLoadText(UINT8 ucRow, UINT8 ucCol, UINT16 uwBKLength, UINT8 ucColor, UINT8 ucSelect, UINT8 *pArray);
UINT8 COsdFxCalBlankCmdCount(UINT16 uwBKLength);
UINT8 COsdFxLoadBlankCmd(UINT8 ucRow, UINT8 ucCol, UINT16 uwLength, UINT8 bgColor);
void COsdFxClearText(UINT8 ucRow, UINT8 ucCol, UINT8 ucHeight, UINT8 ucWidth);

void ap_osdfx_change_1bit_color(UINT8 ucRow, UINT8 ucCol, UINT8 ucSizeH, UINT8 ucSizeW, UINT8 ucColor);
void COsdFxChange2bitColor(UINT8 ucRow, UINT8 ucCol, UINT8 ucSizeH, UINT8 ucSizeW);

void COsdFxWrite1BitFont(UINT8 ucRow, UINT8 ucCol, UINT8 ucFontSelect, UINT8 ucColor);
void COsdFxWrite1BitFont_value(UINT8 ucFontSelect,UINT8 ucColor);
#if 0
void COsdFxSet2BitColor(UINT8 ucIndicate, UINT8 ucColor);
#else
#define COsdFxSet2BitColor(_indicate, _ucColor)		uc2BitColor[(_indicate)%4] = (_ucColor)
#endif
void COsdFxWrite2BitFont(UINT8 ucRow, UINT8 ucCol, UINT8 ucCharSelect);
//void COsdFxLoad2BitFontArray(UINT8 ucRow, UINT8 ucCol, UINT8 code *pArray);
void ap_osdfx_set_2bit_font_offset(UINT8 ucOffset);
//void ap_osdfx_write_2bit_font_seq(UINT8 ucChar);


void COsdFxWrite4BitChar(UINT8 ucRow, UINT8 ucCol, UINT8 ucCharSelect, UINT8 ucBgColor, UINT8 ucLUT);

UINT8 COsdFxCalTextCount(UINT8 ucSelect, UINT8 *pArray);
UINT16 COsdFxCalTextWidth(UINT8 ucSelect, UINT8 *pArray);
UINT8 ap_osdfx_font_width(UINT8 ucFont);

UINT8 COsdFxGetNumberwidth(UINT32 ucValue, UINT8 ucPar);
UINT8 COsdFxShowNumber(UINT8 ucRow, UINT8 ucCol, UINT32 ucValue, UINT8 ucPar, UINT8 ucColor);
#ifdef CONFIG_VBI_CC_SUPPORT
void ap_osdfx_load_vbipalette(void);
void ap_osdfx_load_Vbi_vlcfont(UINT8 code *pArray, UINT16 usStart, UINT16 usLength);
void ap_osdfx_reload_osdpalette(void);
#endif
UINT8 CGetOsdRowLength(UINT8 Row);
void SetOSDDouble(UINT8 ucAttr);
void OSDLine(UINT8 row, UINT8 col, UINT8 length, UINT8 value, UINT8 TheByte);
void DrawLeftLine(void);
void Draw2bit3x2Icon(UINT8*tIcon,UINT8 row,UINT8 col);
// ** other OSD API
//void ap_osdfx_str_cpy(UINT8 *destStr, UINT8 *srcStr, UINT8 length);


/**
 *
 * @}
 */

#endif // _OSDFUNCTION_H

