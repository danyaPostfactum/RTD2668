#include "panel\panel_struct.h"

#define CONFIG_DISPLAY_PANEL_STYLE			_PANEL_INTERFACE_LVDS
#define CONFIG_DISPLAY_LAST_LINE_FINETUNE	_LAST_LINE_METHOD_NONE
#define CONFIG_DISPLAY_PORT 				_PANEL_DISPLAY_PORT_SINGLE
#define CONFIG_DISPLAY_COLOR_BITS			_PANEL_DISPLAY_COLOR_BITS_18

#define CONFIG_DISPLAY_EVEN_RSV1_BIT			0		// 0: Indicate 0, 1: Indicate 1
#define CONFIG_DISPLAY_ODD_RSV1_BIT			0 	// 0: Indicate 0, 1: Indicate 1
#define CONFIG_DISPLAY_BITMAPPING_TABLE		_PANEL_DISPLAY_BITMAPPING_TABLE2

//eric 20071031
#define CONFIG_DISPLAY_PORTAB_SWAP			1	// 0: No Swap, 1: Swap
#define CONFIG_DISPLAY_RED_BLUE_SWAP			0	// 0: No Swap, 1: Swap
#define CONFIG_DISPLAY_MSB_LSB_SWAP			0	// 0: No Swap, 1: Swap
#define CONFIG_DISPLAY_SKEW_DATA_OUTPUT		0	// 0: Disable, 1: Skew data output
#define CONFIG_DISPLAY_OUTPUT_INVERSE		0	// 0: No Swap, 1: Swap
#define CONFIG_DISPLAY_VERTICAL_SYNC			0		// 0: normal, 1: Invert
#define CONFIG_DISPLAY_HORIZONTAL_SYNC		0 // 0: normal, 1: Invert
/*
 * Display Sync Output polarity
 */
#define CONFIG_DISPLAY_VERTICAL_SYNC_NORMAL 1
#define CONFIG_DISPLAY_HORIZONTAL_SYNC_NORMAL 1
#define CONFIG_DISPLAY_RATIO_4X3 0
#define CONFIG_DISPLAY_CLOCK_MAX 110
#define CONFIG_DISPLAY_REFRESH_RATE 60
#define CONFIG_DISPLAY_CLOCK_TYPICAL 85	// hsliao 20081008

/*
 * Display total window setup
 */
#define CONFIG_DISP_HORIZONTAL_TOTAL 1366
#define CONFIG_DISP_VERTICAL_TOTAL 950

/*
 * Display Sync Width setup
 */
#define CONFIG_DISP_HSYNC_WIDTH 31
#define CONFIG_DISP_VSYNC_LENGTH 5

/*
 * Display Enable window setup
 */
#define CONFIG_DISP_DEN_STA_HPOS 32
#define CONFIG_DISP_DEN_END_HPOS 1280+32
#define CONFIG_DISP_DEN_STA_VPOS 12
#define CONFIG_DISP_DEN_END_VPOS 800+12
/*
 * Display active window setup
 */
#define CONFIG_DISP_ACT_STA_HPOS 0
#define CONFIG_DISP_ACT_END_HPOS 1280
#define CONFIG_DISP_ACT_STA_VPOS 0
#define CONFIG_DISP_ACT_END_VPOS 800
#define CONFIG_DISP_HSYNC_LASTLINE 1560
#define CONFIG_DISP_DCLK_DELAY 0


#define _CONFIG_DISP_ACT_STA_BIOS 	0x00
#define CONFIG_DEFAULT_DPLL_M_DIVIDER	0x68  //hpwang 20071119 set default DCLK=(27*104/8)/2/2=87.75MHz
#define CONFIG_DEFAULT_DPLL_N_DIVIDER	0x18

// CSW+ 0970617 For panel power on sequence
/////////////////////////////
//Off --> On Sequence
/////////////////////////////
#define	PANEL_TO_LVDS_ON_ms		10		 // Delay(T1+T2): Panel Power --> LVDS Signal
#define	LVDS_TO_LIGHT_ON_ms		50      // Delay(T3):    Settings: LVDS Signal --> Backlight On
/////////////////////////////
//On --> Off	Sequence
/////////////////////////////
#define	LIGHT_TO_LDVS_OFF_ms		250  // Delay(T4):    Turn Off backlight and delay to turn off LVDS signal
#define	LVDS_TO_PANEL_OFF_ms		50   // Delay(T5+T6): LVDS Signal Off --> Panel Power Off
////////////////////////////
//Panel Off--> Next On
////////////////////////////
#define	PANEL_OFF_TO_ON_ms		550     // Delay(T7):    Totally Off --> Next On

#ifdef _VGAEDID_
#undef _VGAEDID_
UINT8 code EDID_KEY_TABLE[] = 	// EDID for VGA
{
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x20, 0x32, 0x10, 0x19, 0x01, 0x01, 0x01, 0x01,
    0x00, 0x14, 0x01, 0x03, 0x68, 0x29, 0x17, 0x78, 0x2A, 0x3D, 0x85, 0xA6, 0x56, 0x4A, 0x9A, 0x24,
    0x12, 0x50, 0x54, 0xAF, 0xCE, 0x00, 0x81, 0xC0, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x66, 0x21, 0x50, 0xB0, 0x51, 0x00, 0x1B, 0x30, 0x40, 0x70,
    0x36, 0x00, 0x9A, 0xE6, 0x10, 0x00, 0x00, 0x1E, 0xA9, 0x1A, 0x00, 0xA0, 0x50, 0x00, 0x16, 0x30,
    0x30, 0x20, 0x37, 0x00, 0x9A, 0xE6, 0x10, 0x00, 0x00, 0x1A, 0x00, 0x00, 0x00, 0xFD, 0x00, 0x38,
    0x4C, 0x1F, 0x3D, 0x09, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFC,
    0x00, 0x48, 0x4C, 0x31, 0x39, 0x44, 0x32, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x12,
};

#endif






