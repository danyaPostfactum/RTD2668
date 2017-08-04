/*
 * Display Setup
 */
#define CONFIG_DISPLAY_PANEL_STYLE                            _PANEL_INTERFACE_LVDS         // _PANEL_INTERFACE_LVDS or _PANEL_INTERFACE_RSDS or _PANEL_INTERFACE_TTL
#define CONFIG_DISPLAY_LAST_LINE_FINETUNE                     _LAST_LINE_METHOD_NONE
#define CONFIG_DISPLAY_PORT					_PANEL_DISPLAY_PORT_SINGLE	// 0: single port, 1: double port
#define CONFIG_DISPLAY_COLOR_BITS			_PANEL_DISPLAY_COLOR_BITS_24	// 0: 30bits, 1:24 bits, 2: 30bits

#define CONFIG_DISPLAY_EVEN_RSV1_BIT		0	// 0: Indicate 0, 1: Indicate 1
#define CONFIG_DISPLAY_ODD_RSV1_BIT			0 	// 0: Indicate 0, 1: Indicate 1
#define CONFIG_DISPLAY_BITMAPPING_TABLE		_PANEL_DISPLAY_BITMAPPING_TABLE2 	// 0: Table1, 1:Table2

#define CONFIG_DISPLAY_PORTAB_SWAP			1	// 0: No Swap, 1: Swap
#define CONFIG_DISPLAY_RED_BLUE_SWAP		0	// 0: No Swap, 1: Swap
#define CONFIG_DISPLAY_MSB_LSB_SWAP			0	// 0: No Swap, 1: Swap
#define CONFIG_DISPLAY_SKEW_DATA_OUTPUT		0	// 0: Disable, 1: Skew data output
#define CONFIG_DISPLAY_OUTPUT_INVERSE		0	// 0: No Swap, 1: Swap
#define CONFIG_DISPLAY_CLOCK_DELAY		0	// 0~7
#define CONFIG_DISPLAY_VERTICAL_SYNC			0		// 0: normal, 1: Invert
#define CONFIG_DISPLAY_HORIZONTAL_SYNC		0 // 0: normal, 1: Invert
/*
 * Display Sync Output polarity
 */
#define CONFIG_DISPLAY_VERTICAL_SYNC_NORMAL 1
#define CONFIG_DISPLAY_HORIZONTAL_SYNC_NORMAL 1
#define CONFIG_DISPLAY_RATIO_4X3 			0	// 0: 16:9 , 1: 4:3 
#define CONFIG_DISPLAY_CLOCK_MAX 			88
#define CONFIG_DISPLAY_REFRESH_RATE 		60	// 75
#define CONFIG_DISPLAY_CLOCK_TYPICAL 76

/*
 * Display total window setup
 */
#define CONFIG_DISP_HORIZONTAL_TOTAL		1560 // 1519
#define CONFIG_DISP_VERTICAL_TOTAL			806 // 790

/*
 * Display Sync Width setup
 */
#define CONFIG_DISP_HSYNC_WIDTH				32
#define CONFIG_DISP_VSYNC_LENGTH			5

/*
 * Display Enable window setup
 */
#define CONFIG_DISP_DEN_STA_HPOS 			49 //111
#define CONFIG_DISP_DEN_END_HPOS 			1415 //1477
#define CONFIG_DISP_DEN_STA_VPOS 			21 //20
#define CONFIG_DISP_DEN_END_VPOS 			789 //788

/*
 * Display active window setup
 */
#define CONFIG_DISP_ACT_STA_HPOS 0
#define CONFIG_DISP_ACT_END_HPOS 1366
#define CONFIG_DISP_ACT_STA_VPOS 0
#define CONFIG_DISP_ACT_END_VPOS 768
#define CONFIG_DISP_HSYNC_LASTLINE 			1606 // 1519
#define CONFIG_DISP_DCLK_DELAY				2

#define _CONFIG_DISP_ACT_STA_BIOS 			0x00
#define CONFIG_DEFAULT_DPLL_M_DIVIDER		0x5c//set default DCLK=(27*92/8)/2/2=77.625MHz
#define CONFIG_DEFAULT_DPLL_N_DIVIDER		0x18

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
    0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x0C,0xF4,0x10,0x22,0x21,0x59,0x54,0x0A,
    0x0F,0x13,0x01,0x03,0x6C,0x00,0x00,0x78,0x20,0xB0,0x64,0xA2,0x55,0x48,0x98,0x27,
    0xD9,0x50,0x54,0xBF,0xEF,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
    0x01,0x01,0x01,0x01,0x01,0x01,0x66,0x21,0x56,0xAA,0x51,0x00,0x1E,0x30,0x46,0x8F,
    0x33,0x00,0x00,0x00,0x00,0x00,0x00,0x1E,0x00,0x00,0x00,0xFF,0x00,0x31,0x37,0x33,
    0x33,0x30,0x30,0x30,0x30,0x31,0x0A,0x0A,0x0A,0x0A,0x00,0x00,0x00,0xFD,0x00,0x37,
    0x4B,0x1E,0x3C,0x08,0x00,0x0A,0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x00,0x00,0xFC,
    0x00,0x4C,0x4D,0x31,0x38,0x31,0x30,0x57,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x00,0xB6,
};

#endif

