/*===========================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2009
 * All rights reserved.
 * ========================================================== */

/*==================== File Description ===================== */
/**
 * @file
 * 	This file is gobal parameter declaration for OSD related functions.
 *
 * @author 	$Author: cf.tsai $
 * @date 	$Date: 2009-03-13 $
 * @version	$Revision: 10 $
 */

/**
 * @addtogroup osd
 * @{
 */

#ifndef _OSDPARAMETER_H
#define _OSDPARAMETER_H
/*===================== Module Dependency  ================== */
#include "osdobjectdef.h"

#if defined(CONFIG_Dragon_F) || defined(CONFIG_Dragon_D) || defined(CONFIG_Dragon_ABC)
#include "scaler_display.h"
#else
// ** for temporary use
//#define CONFIG_DISP_DEN_STA_HPOS 32
//#define CONFIG_DISP_DEN_STA_VPOS 16
#endif

/*========================== Types ========================== */

/*======================== Definitions ====================== */

//
// ** To control all OSD position offset
#define OSD_H_OFFSET 			4
#define OSD_V_OFFSET			0



//
// RGB Settings for Colors
//
#define _COLOR_0       0x9F,0xED,0xAB
#define _COLOR_1 	0xD2,0xC1,0x8B
#define _COLOR_2  	0x2E,0x6D,0xA3
#define _COLOR_3   	0x00,0x00,0x00
#define _COLOR_4  	0xFF,0xFF,0xFF
#define _COLOR_5  	0x48,0x53,0x65
#define _COLOR_6 	0xC0,0xC0,0xC0
#define _COLOR_7  	0x80,0x80,0x80
#define _COLOR_8  	0xFF,0x80,0x00
#define _COLOR_9  	0xC0,0xC0,0xC0
#define _COLOR_10  	0x33,0x6A,0xA5
#define _COLOR_11 	 0x94,0xBD,0x35
#define _COLOR_12 	0xF3,0x94,0x28
#define _COLOR_13 	  0x00,0xFF,0x00
#define _COLOR_14  	 0x00,0x00,0xFF
#define _COLOR_15  	 0xFF,0x00,0x00
#define _RGB_BLACK              0x00,0x00,0x00
#define _RGB_WHITE              255, 255, 255
#define _RGB_RED                255, 0, 0
#define _RGB_GREEN              0, 255, 0
#define _RGB_BLUE               0, 0, 255
#define _RGB_GRAY               127, 127, 127
#define _RGB_YELLOW             255, 255, 0
#define _RGB_MAGENTA            255, 0, 255
#define _RGB_CYAN               0, 255, 255

#define _RGB_SILVER                 193, 193, 193

#define _RGB_LIGHT_MAGENTA			255, 127, 255

#define _RGB_PASTEL_CYAN            109, 207, 246
#define _RGB_LIGHT_CYAN             0, 191, 243
#define _RGB_PURE_CYAN              0, 174, 239
#define _RGB_DARK_CYAN              0, 118, 163
#define _RGB_DARKER_CYAN            0, 91, 127

#define _RGB_PASTEL_CYAN_BLUE       125, 167, 217
#define _RGB_LIGHT_CYAN_BLUE        68, 140, 203
#define _RGB_PURE_CYAN_BLUE         0, 114, 188
#define _RGB_DARK_CYAN_BLUE         0, 74, 128
#define _RGB_DARKER_CYAN_BLUE       0, 54, 99

#define _RGB_PASTEL_GREEN_CYAN      122, 204, 200
#define _RGB_LIGHT_GREEN_CYAN       28, 187, 180
#define _RGB_PURE_GREEN_CYAN        0, 169, 157
#define _RGB_DARK_GREEN_CYAN        0, 116, 107
#define _RGB_DARKER_GREEN_CYAN      0, 89, 82

#define _RGB_PASTEL_BLUE            131, 147, 202
#define _RGB_LIGHT_BLUE             86, 116, 185
#define _RGB_PURE_BLUE              0, 84, 166
#define _RGB_DARK_BLUE              0, 52, 113
#define _RGB_DARKER_BLUE            0, 33, 87

#define _RGB_PASTEL_RED             246, 150, 121
#define _RGB_LIGHT_RED              242, 108, 79
#define _RGB_PURE_RED               237, 28, 36
#define _RGB_DARK_RED               158, 11, 14
#define _RGB_DARKER_RED             121, 0, 0

#define _RGB_PASTEL_YELLOW          255, 247, 153
#define _RGB_LIGHT_YELLOW           255, 245, 104
#define _RGB_PURE_YELLOW            255, 242, 0
#define _RGB_DARK_YELLOW            171, 160, 0
#define _RGB_DARKER_YELLOW          130, 123, 0

#define _RGB_PASTEL_YELLOW_ORANGE   253, 198, 137
#define _RGB_LIGHT_YELLOW_ORANGE    251, 175, 93
#define _RGB_PURE_YELLOW_ORANGE     247, 148, 29
#define _RGB_DARK_YELLOW_ORANGE     163, 98, 9
#define _RGB_DARKER_YELLOW_ORANGE   125, 73, 0

#define _RGB_PASTEL_YELLOW_GREEN    162, 211, 156
#define _RGB_LIGHT_YELLOW_GREEN     124, 197, 118
#define _RGB_PURE_YELLOW_GREEN      57, 181, 74
#define _RGB_DARK_YELLOW_GREEN      25, 123, 48
#define _RGB_DARKER_YELLOW_GREEN    0, 94, 32


#define _RGB_YELLOW_01				171, 160, 0
#define _RGB_YELLOW_02				196, 184, 0
#define _RGB_YELLOW_03				224, 211, 0
#define _RGB_YELLOW_04				255, 242, 0


#define _RGB_COLOR00	0x08, 0x1f, 0x5a
#define _RGB_COLOR01	0x9d, 0x9d, 0xff
#define _RGB_COLOR02	0x34, 0x67, 0x83
#define _RGB_COLOR03	0x6c, 0xce, 0xff
#define _RGB_COLOR04	0x00, 0x7b, 0xbc
#define _RGB_COLOR05	0xff, 0x00, 0x72	//255,0,114 //allen modified 070404
#define _RGB_COLOR06	0xff, 0xa0, 0x00	// 255,160,0	//allen modified  20070402
#define _RGB_COLOR07	0xB6, 0xD2, 0xF6	//allen modified 20070702
#define _RGB_COLOR08	0xdB, 0x64, 0x1c	// 219,100,28	orange

#define _RGB_NORMAL_1	0x8F, 0xBC, 0xF7	//143,188,247
#define _RGB_NORMAL_2	0xA2, 0xC7, 0xF8	//162,199,248
#define _RGB_NORMAL_3	0xBA, 0xD5, 0xF9	//186,213,249
#define _RGB_NORMAL_4	0xD5, 0xE5, 0xFA	//213,229,250
#define _RGB_NORMAL_0	0x7E, 0xA9, 0xE3	//126,169,227

#define _RGB_ACTIVE_1	0xE0, 0x5D, 0x03	//224,93,3
#define _RGB_ACTIVE_2	0xFD, 0x88, 0x39	//253,136,57
#define _RGB_ACTIVE_3	0xFE, 0xB3, 0x81	//254,179,129
#define _RGB_ACTIVE_4	0xFE, 0xD7, 0xBC	//254,215,188
#define _RGB_ACTIVE_0	0xDA, 0x59, 0x03	//218,89,3

#define _RGB_Positon_1	0xf0, 0xf0, 0xf0
#define _RGB_Positon_3	0x10, 0x10, 0x10
#define _RGB_Positon_2	0x60, 0x60, 0x60
#define _RGB_Positon_13	0x2f, 0x67, 0xbc
#define _RGB_Positon_14	0x4e, 0x75, 0xc4
#define _RGB_Positon_15	0x8c, 0xa9, 0xc9



/*======================== CONSTANTS =========================================*/

/**
 * OSD font data start address in SRAM.
 * This value depend on Row map size.
 */
#define _OSD_SRAM_FONT_DATA_START_ADDR		0x300 //(20+1+2*40+18*60+(1+2))	// = 1184

#define _OSD_TITLE_ROW_LENGTH				39
#define _OSD_ITEM_ROW_LENGTH				46







/*======================== MACROS ============================================*/
#if 0 // for Dragon
#define FONTBYTE0(x)				HIBYTE(0x2000|x),LOBYTE(0x2000|x)
#define FONTBYTE1(x)				HIBYTE(0x6000|x),LOBYTE(0x6000|x)
#define FONTBYTE2(x)				HIBYTE(0xA000|x),LOBYTE(0xA000|x)
#define FONTALLBYTE(x)  				HIBYTE(0xE000|x),LOBYTE(0xE000|x)
#else
#define FONTBYTE0(x)                    HIBYTE(0x1000|x),LOBYTE(0x1000|x)
#define FONTBYTE1(x)                    HIBYTE(0x5000|x),LOBYTE(0x5000|x)
#define FONTBYTE2(x)                    HIBYTE(0x9000|x),LOBYTE(0x9000|x)
#define FONTALLBYTE(x)                  HIBYTE(0xD000|x),LOBYTE(0xD000|x)
#endif

#define WINDOWBYTE0(x)                  HIBYTE(0x0000|x),LOBYTE(0x0000|x)
#define WINDOWBYTE1(x)                  HIBYTE(0x4000|x),LOBYTE(0x4000|x)
#define WINDOWBYTE2(x)                  HIBYTE(0x8000|x),LOBYTE(0x8000|x)
#define WINDOWALLBYTE(x)                HIBYTE(0xC000|x),LOBYTE(0xC000|x)


/*======================== TYPES =============================================*/

// Color
typedef enum
{
    _CP_BLACK = 0,
    _CP_WHITE,
    _CP_PASTEL_CYAN_BLUE,
    _CP_DARK_CYAN_BLUE,
    _CP_PURE_YELLOW_ORANGE,
    _CP_DARKER_CYAN_BLUE,
    _CP_GRAY, //_CP_LIGHT_BLUE,
    _CP_SILVER,

    _CP_LIGHT_MAGENTA,			// -- 8
    _CP_GREEN,
    _CP_BLUE,
    _CP_YELLOW,
    _CP_MAGENTA,
    _CP_PURE_RED,
    _CP_DARK_YELLOW_ORANGE,
    _CP_PURE_CYAN_BLUE
} ColorPaletteDef;



// Window Register 0, byte 2
#define _R_GRADIENT_INC			(_BIT7)
#define _R_GRADIENT_DEC			(0)
#define _G_GRADIENT_INC			(_BIT6)
#define _G_GRADIENT_DEC			(0)
#define _B_GRADIENT_INC			(_BIT5)
#define _B_GRADIENT_DEC			(0)
#define _GRADIENT_LEVEL(_gl)		(((_gl)&0x03)<<3)
#define _R_GRADIENT_ENABLE		(_BIT2)
#define _G_GRADIENT_ENABLE		(_BIT1)
#define _B_GRADIENT_ENABLE		(_BIT0)

// Window Register 3, byte 1
#define _LEVEL_PER_GRADIENT(_lpg)	(((_lpg)&0x07) << 5)//4)
#define _WINDOW_COLOR(_wcol)		((_wcol)&0x1f)//0x0f

// Window Register 3, byte 2
#define _BLENDING_ENABLE			(_BIT7)
#define _GRADIENT_ENABLE			(_BIT6)
#define GRADIENT_ENABLE(e)			(((e)&0x01) << 6)
#define _GRADIENT_H					(0)
#define _GRADIENT_V					(_BIT5)
#define _WIN_SHADOW_ENABLE			(_BIT4)
#define _WIN_3DBUTTON_ENABLE		(_BIT4)
#define BORDER_ENABLE(e)			(((e)&0x01) << 4)
#define _SHADOW_TYPE1				(0x00 << 1)
#define _SHADOW_TYPE2				(0x01 << 1)
#define _SHADOW_TYPE3				(0x02 << 1)
#define _SHADOW_TYPE4				(0x03 << 1)
#define _3DBUTTON_TYPE1				(0x04 << 1)
#define _3DBUTTON_TYPE2				(0x05 << 1)
#define _BORDER						(0x07 << 1)
#define WINDOW_ENABLE(e)			((e)&0x01)
#define WINDOW_COLOR(c)				((c)&0x0f)


// Row Command Byte0
#define _ROW_START_CMD				(_BIT7)
//#define BORDER_TYPE(_bt)			((_bt)<<2)
//#define _CHAR_BORDER_NONE			(0<<2)
#define _CHAR_BORDER				(0x01<<2)
#define _CHAR_SHADOW_LEFTTOP		(0x04<<2)
#define _CHAR_SHADOW_LEFTBOTTOM		(0x05<<2)
#define _CHAR_SHADOW_RIGHTTOP		(0x06<<2)
#define _CHAR_SHADOW_RIGHTBOTTOM	(0x07<<2)
#define _CHAR_DOUBLE_WIDTH			(_BIT1)
#define _CHAR_DOUBLE_HEIGHT			(_BIT0)
#define SET_DOUBLE_WIDTH(_dw)		(((_dw)&0x01)<<1)
#define SET_DOUBLE_HEIGHT(_dh)		((_dh)&0x01)

// Row Command Byte1
#define ROW_HEIGHT(_rh)		((((_rh)-1)&0x1f)<<3)
#define SET_COL_SPACE(_cp)		((_cp)&0x07)

// Row Command Byte2
#define ROW_LENGTH(_rl)		(_rl)



// ////////////////////////////////////////////////////////////////////////////
// ** from Dragon OSD
//yyforautoscan  these is set according to bitmap
#define  _AUTO_SCAN_WIDTH 		810//708// 708
#define  _AUTO_SCAN_HEIGHT  	128//108//
#define  _PREBLANK_ARROW1     	65
#define  _PREBLANK_ARROW2     	305 //jacklong 2007/08/14 modify 273
#define  _PREBLANK_ICON_EXIT  	625 //jacklong 2007/08/14 modify 600

#define _VOLUMNBAR_ROW_COUNT	11
#define _VOLUMNBAR_CHAR_COUNT	39



// ////////////////////////////////////////////////////////////////////////////
enum _OSD_MAP_ID
{
    _OSD_MAP0 = 0,
    _OSD_MAP1,
    _OSD_MAP_MAX,
};


enum _OsdLanguageId
{
    _English = 0,
    _French,
    _German,
    _Spanish,
    _Italian,
    _Dutch,
    _Portuguese,
    _Norwegian,
    _Finnish,
    _Danish,
    _Swedish,
    _Polish,
    _Czech,
    _LANGUAGE_TYPE_MAX
};


/*======================== VARIABLES =========================================*/
#if !defined(_OSDPARAMETER_C)

extern UINT8 code tOsd_Palette[];



extern WindowStyle code tPopupMenuWindowStyle;
extern WindowStyle code tMainWindowStyle;
extern WindowStyle code tModeWindowStyle;
extern WindowStyle code tMuteBgWindowStyle;
extern WindowStyle code tModeKeyWindowStyle;


extern WindowStyle code tSourceWindowStyle;
extern ItemPosition code tSourceWindowPos;
extern WindowStyle code tHighLightWindowStyle;
extern WindowStyle code tItemAreaWindowStyle;
extern WindowStyle code tHintBarWindowStyle;
extern WindowStyle code tTitleBarWindowStyle;
extern 	WindowStyle code tMainIconBgWindowStyle;
extern 	WindowStyle code tMainIconHighlightWindowStyle;
extern WindowStyle code tMainIconHighlightWindowStyle2;

extern WindowStyle code tPopon_Slider_Title_WindowStyle;
extern WindowStyle code tPopon_Slider_WindowStyle;
extern WindowStyle code tPopon_Slider_Hint_WindowStyle;

extern WindowStyle code tSource_Select_Title_WindowStyle;
extern WindowStyle code tSource_Select_Window_Style;
extern WindowStyle code tSource_Select_Hint_WindowStyle;
extern WindowStyle code tSource_Select_Highlight_WindowStyle;

extern WindowStyle code tSource_Info_Title_WindowStyle;
extern WindowStyle code tSource_Info_WindowStyle;

extern ItemPosition code tModeInfoHintWindowPos;

extern ItemPosition code tChannel_Edit_TitleWindowPos;
extern ItemPosition code tChannel_Edit_WindowPos;
extern ItemPosition code tChannel_Edit_HintWindowPos;
extern ItemPosition code tChannel_Edit_Highlight_WindowPos;
extern ItemPosition code tChannel_Edit_Name_Highlight_WindowPos;

extern ItemPosition code tFactory_mode_Title_WindowPos;
extern ItemPosition code tFactory_mode_WindowPos;
extern ItemPosition code tFactory_mode_Hint_WindowPos;
extern ItemPosition code tFactory_mode_Highlight_WindowPos;

extern WindowStyle code tFac_Title0_WindowStyle;
extern WindowStyle code tFac_Title1_Window_Style;
extern WindowStyle code tFac_Mainmenu_WindowStyle;
extern WindowStyle code tFac_Submenu_WindowStyle;

extern 	ItemPosition code tMuteIcon_BG_WindowPos;

extern 	ItemPosition code tEqControl_title_window_pos;
extern 	ItemPosition code tEqControl_menu_window_pos;
extern 	ItemPosition code tEqControl_hint_window_pos;
extern 	ItemPosition code tEqControl_highlight_window_pos;

extern UINT8* code tOsdRowCmdSet[];
extern UINT8* code tRowLength_LUT_Set[];

extern UINT8 code tRowLength_LUT0[];
//#ifdef CONFIG_VBI_CC_SUPPORT
extern UINT8 code Palette_VBI[];
//#endif


#endif // !defined(_OSDPARAMETER_C)


/*======================== FUNCTIONS =========================================*/

#endif // #ifndef _OSDPARAMETER_H

/**
 *
 * @}
 */

