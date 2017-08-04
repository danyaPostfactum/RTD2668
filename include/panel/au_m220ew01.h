/*
 * Display Setup
 */
#define CONFIG_DISPLAY_PANEL_STYLE                            _PANEL_INTERFACE_LVDS         // _PANEL_INTERFACE_LVDS or _PANEL_INTERFACE_RSDS or _PANEL_INTERFACE_TTL
#define CONFIG_DISPLAY_LAST_LINE_FINETUNE                     _LAST_LINE_METHOD_NONE
#define CONFIG_DISPLAY_PORT _PANEL_DISPLAY_PORT_DOUBLE				// 0: single port, 1: double port
#define CONFIG_DISPLAY_COLOR_BITS _PANEL_DISPLAY_COLOR_BITS_24		// 0: 30bits, 1:24 bits, 2: 18bits

#define CONFIG_DISPLAY_EVEN_RSV1_BIT 0		// 0: Indicate 0, 1: Indicate 1
#define CONFIG_DISPLAY_ODD_RSV1_BIT	0 	// 0: Indicate 0, 1: Indicate 1
#define CONFIG_DISPLAY_BITMAPPING_TABLE	_PANEL_DISPLAY_BITMAPPING_TABLE1// 0: Table1, 1:Table2
#define CONFIG_DISPLAY_PORTAB_SWAP		1	// 0: No Swap, 1: Swap
#define CONFIG_DISPLAY_RED_BLUE_SWAP	0	// 0: No Swap, 1: Swap
#define CONFIG_DISPLAY_MSB_LSB_SWAP		0	// 0: No Swap, 1: Swap
#define CONFIG_DISPLAY_SKEW_DATA_OUTPUT 0	// 0: Disable, 1: Skew data output
#define CONFIG_DISPLAY_OUTPUT_INVERSE	0	// 0: No Swap, 1: Swap
#define CONFIG_DISPLAY_CLOCK_DELAY		0	// 0~7
#define CONFIG_DISPLAY_VERTICAL_SYNC			0		// 0: normal, 1: Invert
#define CONFIG_DISPLAY_HORIZONTAL_SYNC		0 // 0: normal, 1: Invert

/*
 * Display Sync Output polarity
 */
#define CONFIG_DISPLAY_VERTICAL_SYNC_NORMAL 1
#define CONFIG_DISPLAY_HORIZONTAL_SYNC_NORMAL 1
#define CONFIG_DISPLAY_RATIO_4X3  0//yy  1
#define CONFIG_DISPLAY_CLOCK_MAX   170//yy50
#define CONFIG_DISPLAY_REFRESH_RATE 75
#define CONFIG_DISPLAY_CLOCK_TYPICAL 125

/*
 * Display total window setup
 */
//#define CONFIG_DISP_HORIZONTAL_TOTAL 1704
#define CONFIG_DISP_HORIZONTAL_TOTAL 1790//1128//1408
#define CONFIG_DISP_VERTICAL_TOTAL 1150//1066//1065

/*
 * Display Sync Width setup
 */
#define CONFIG_DISP_HSYNC_WIDTH   16//31
#define CONFIG_DISP_VSYNC_LENGTH 3//5

/*
 * Display Enable window setup
 */


#define CONFIG_DISP_DEN_STA_HPOS  40//jacklong 32
#define CONFIG_DISP_DEN_END_HPOS  1680+40//yy1312
#define CONFIG_DISP_DEN_STA_VPOS  18//16
#define CONFIG_DISP_DEN_END_VPOS  1050+18//1040
/*
 * Display active window setup
 */

#define CONFIG_DISP_ACT_STA_HPOS 0//jacklong 32
#define CONFIG_DISP_ACT_END_HPOS 1680//1312
#define CONFIG_DISP_ACT_STA_VPOS 0 //jacklong //16
#define CONFIG_DISP_ACT_END_VPOS  1050//1040
#define CONFIG_DISP_HSYNC_LASTLINE 1790//2256//yy   modify1408
#define CONFIG_DISP_DCLK_DELAY 0


#define _CONFIG_DISP_ACT_STA_BIOS 	0x00
#define CONFIG_DEFAULT_DPLL_M_DIVIDER	0xb2
#define CONFIG_DEFAULT_DPLL_N_DIVIDER	0x18

// CSW+ 0970617 For panel power on sequence
/////////////////////////////
//Off --> On Sequence
/////////////////////////////
#define	PANEL_TO_LVDS_ON_ms		50		 // Delay(T1+T2): Panel Power --> LVDS Signal
#define	LVDS_TO_LIGHT_ON_ms		200      // Delay(T3):    Settings: LVDS Signal --> Backlight On
/////////////////////////////
//On --> Off	Sequence
/////////////////////////////
#define	LIGHT_TO_LDVS_OFF_ms		200  // Delay(T4):    Turn Off backlight and delay to turn off LVDS signal
#define	LVDS_TO_PANEL_OFF_ms		40   // Delay(T5+T6): LVDS Signal Off --> Panel Power Off
////////////////////////////
//Panel Off--> Next On
////////////////////////////
#define	PANEL_OFF_TO_ON_ms		1000     // Delay(T7):    Totally Off --> Next On

////////////////////////////
// For VIP
////////////////////////////
#define	PANEL_BIT_FOR_VIP	1				// Panle 8btis


#ifdef _VGAEDID_
#undef _VGAEDID_
UINT8 code EDID_KEY_TABLE[] = 	// EDID for VGA
{
    0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x0C,0xF4,0x10,0x19,0x21,0x59,0x54,0x0A,
    0x24,0x13,0x01,0x03,0x6C,0x2F,0x1E,0x78,0xEA,0xF0,0x65,0x98,0x57,0x51,0x91,0x27,
    0x21,0x50,0x54,0xBF,0xEF,0x80,0x71,0x4A,0x71,0x4F,0x81,0x40,0x81,0x80,0xA9,0x40,
    0xB3,0x00,0x01,0x01,0x01,0x01,0x08,0x39,0x90,0x30,0x62,0x1A,0x27,0x40,0x68,0xB0,
    0x36,0x00,0xB1,0x0E,0x11,0x00,0x00,0x1C,0x00,0x00,0x00,0xFD,0x00,0x38,0x4B,0x1F,
    0x51,0x0E,0x00,0x0A,0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x00,0x00,0xFC,0x00,0x4C,
    0x43,0x44,0x31,0x39,0x31,0x30,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x00,0x00,0x00,0xFE,
    0x00,0x31,0x37,0x33,0x33,0x30,0x30,0x30,0x30,0x31,0x0A,0x20,0x20,0x20,0x00,0xAF,
};

#endif

