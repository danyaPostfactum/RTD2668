/*
 * Display Setup
 */
#define CONFIG_DISPLAY_PANEL_STYLE                            _PANEL_INTERFACE_LVDS         // _PANEL_INTERFACE_LVDS or _PANEL_INTERFACE_RSDS or _PANEL_INTERFACE_TTL
#define CONFIG_DISPLAY_LAST_LINE_FINETUNE                     _LAST_LINE_METHOD_NONE
#define CONFIG_DISPLAY_PORT _PANEL_DISPLAY_PORT_DOUBLE				// 0: single port, 1: double port
#define CONFIG_DISPLAY_COLOR_BITS _PANEL_DISPLAY_COLOR_BITS_24		// 0: 30bits, 1:24 bits, 2: 18bits

#define CONFIG_DISPLAY_EVEN_RSV1_BIT 0		// 0: Indicate 0, 1: Indicate 1
#define CONFIG_DISPLAY_ODD_RSV1_BIT	0 	// 0: Indicate 0, 1: Indicate 1
#define CONFIG_DISPLAY_BITMAPPING_TABLE	_PANEL_DISPLAY_BITMAPPING_TABLE1 // 0: Table1, 1:Table2

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
#define CONFIG_DISPLAY_CLOCK_MAX   170 //yy50
#define CONFIG_DISPLAY_CLOCK_TYPICAL   154 //yy50
#define CONFIG_DISPLAY_REFRESH_RATE 60

/*
 * Display total window setup
 */
//#define CONFIG_DISP_HORIZONTAL_TOTAL 1704
#define CONFIG_DISP_HORIZONTAL_TOTAL 2180//1128//1408
#define CONFIG_DISP_VERTICAL_TOTAL 1120//1066//1065

/*
 * Display Sync Width setup
 */
#define CONFIG_DISP_HSYNC_WIDTH		176//16
#define CONFIG_DISP_VSYNC_LENGTH	6// 3

/*
 * Display Enable window setup
 */

//<roni 20080305 modify>start postion can't even
#define CONFIG_DISP_DEN_STA_HPOS  200	//jacklong 32
#define CONFIG_DISP_DEN_END_HPOS  CONFIG_DISP_ACT_END_HPOS+CONFIG_DISP_DEN_STA_HPOS	//1680+280//yy1312
#define CONFIG_DISP_DEN_STA_VPOS  16	//jacklong 18//16
#define CONFIG_DISP_DEN_END_VPOS  CONFIG_DISP_ACT_END_VPOS+CONFIG_DISP_DEN_STA_VPOS	//1050+36//1040
/*
 * Display active window setup
 */

#define CONFIG_DISP_ACT_STA_HPOS	0//jacklong 32
#define CONFIG_DISP_ACT_END_HPOS	1920//1312
#define CONFIG_DISP_ACT_STA_VPOS	0 //jacklong //16
#define CONFIG_DISP_ACT_END_VPOS 	1080//1040
#define CONFIG_DISP_HSYNC_LASTLINE	2180//yy   modify1408
#define CONFIG_DISP_DCLK_DELAY		0


#define _CONFIG_DISP_ACT_STA_BIOS		0x00
#define CONFIG_DEFAULT_DPLL_M_DIVIDER	178//152	//0xb2 // 0xb2 = 178
#define CONFIG_DEFAULT_DPLL_N_DIVIDER	24//28	//0x18 // 0x18 = 24

/////////////////////////////
//Off --> On Sequence	// 20080218
/////////////////////////////
#define	PANEL_TO_LVDS_ON_ms		50
#define	LVDS_TO_LIGHT_ON_ms		250
/////////////////////////////
//On --> Off	Sequence
/////////////////////////////
#define	LIGHT_TO_LDVS_OFF_ms		200
#define	LVDS_TO_PANEL_OFF_ms		16
////////////////////////////
//Panel Off--> Next On
////////////////////////////
#define	PANEL_OFF_TO_ON_ms		1000

////////////////////////////
// For VIP
////////////////////////////
#define	PANEL_BIT_FOR_VIP	1				// Panle 8btis

#ifdef _VGAEDID_
#undef _VGAEDID_
UINT8 code EDID_KEY_TABLE[] = 	// EDID for VGA
{
    0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x0C,0xF4,0x60,0x21,0x21,0x59,0x54,0x0A,
    0x20,0x13,0x01,0x03,0x6C,0x2F,0x1A,0x78,0x2E,0x35,0x85,0xA6,0x56,0x48,0x9A,0x24,
    0x12,0x50,0x54,0x2F,0x6F,0x00,0x71,0x4F,0x81,0x80,0x81,0x8A,0x95,0x00,0x95,0x0F,
    0xB3,0x00,0xD1,0xC0,0x01,0x01,0x02,0x3A,0x80,0x18,0x71,0x38,0x2D,0x40,0x58,0x2C,
    0x45,0x00,0xDC,0x0C,0x11,0x00,0x00,0x1E,0x00,0x00,0x00,0xFF,0x00,0x31,0x37,0x33,
    0x33,0x30,0x30,0x30,0x30,0x31,0x0A,0x20,0x20,0x20,0x00,0x00,0x00,0xFD,0x00,0x38,
    0x4B,0x1E,0x53,0x15,0x00,0x0A,0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x00,0x00,0xFC,
    0x00,0x4C,0x43,0x44,0x32,0x31,0x36,0x30,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x00,0x2A,
};

#endif


