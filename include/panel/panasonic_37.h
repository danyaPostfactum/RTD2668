/*
 * Display Setup
 */
#define CONFIG_DISPLAY_PORT _PANEL_DISPLAY_PORT_SINGLE				// 0: single port, 1: double port
#define CONFIG_DISPLAY_COLOR_BITS _PANEL_DISPLAY_COLOR_BITS_24			// 0: 30bits, 1:24 bits, 2: 30bits

#define CONFIG_DISPLAY_EVEN_RSV1_BIT 0		// 0: Indicate 0, 1: Indicate 1
#define CONFIG_DISPLAY_ODD_RSV1_BIT	0 	// 0: Indicate 0, 1: Indicate 1

#define CONFIG_DISPLAY_BITMAPPING_TABLE	 _PANEL_DISPLAY_BITMAPPING_TABLE1 	// 0: Table1, 1:Table2


#define CONFIG_DISPLAY_PORTAB_SWAP		0	// 0: No Swap, 1: Swap
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
#define CONFIG_DISPLAY_VERTICAL_SYNC 0		// 0: normal, 1: Invert
#define CONFIG_DISPLAY_HORIZONTAL_SYNC 0 // 0: normal, 1: Invert
#define CONFIG_DISPLAY_RATIO_4X3 0		// 0: 16:9 , 1: 4:3
#define CONFIG_DISPLAY_CLOCK_MAX 80
#define CONFIG_DISPLAY_REFRESH_RATE 75
#if 1
/*
 * Display total window setup
 */
#define CONFIG_DISP_HORIZONTAL_TOTAL 1519
#define CONFIG_DISP_VERTICAL_TOTAL 790

/*
 * Display Sync Width setup
 */
#define CONFIG_DISP_HSYNC_WIDTH 32
#define CONFIG_DISP_VSYNC_LENGTH 5

/*
 * Display Enable window setup
 */
#define CONFIG_DISP_DEN_STA_HPOS 61 //111
#define CONFIG_DISP_DEN_END_HPOS 1477
#define CONFIG_DISP_DEN_STA_VPOS 10
#define CONFIG_DISP_DEN_END_VPOS 788

/*
 * Display active window setup
 */
#define CONFIG_DISP_ACT_STA_HPOS 0
#define CONFIG_DISP_ACT_END_HPOS 1024
#define CONFIG_DISP_ACT_STA_VPOS 0
#define CONFIG_DISP_ACT_END_VPOS 720
#define CONFIG_DISP_HSYNC_LASTLINE 1519
#define CONFIG_DISP_DCLK_DELAY 2
#else//set to 17" parameter for test
#define CONFIG_DISP_HORIZONTAL_TOTAL 1408
#define CONFIG_DISP_VERTICAL_TOTAL 1065

/*
 * Display Sync Width setup
 */
#define CONFIG_DISP_HSYNC_WIDTH 31
#define CONFIG_DISP_VSYNC_LENGTH 5

/*
 * Display Enable window setup
 */

/*
#define CONFIG_DISP_DEN_STA_HPOS 405
#define CONFIG_DISP_DEN_END_HPOS 1685

#define CONFIG_DISP_DEN_STA_VPOS 40
#define CONFIG_DISP_DEN_END_VPOS 1064
*/
#define CONFIG_DISP_DEN_STA_HPOS 32
#define CONFIG_DISP_DEN_END_HPOS 1312
#define CONFIG_DISP_DEN_STA_VPOS 16
#define CONFIG_DISP_DEN_END_VPOS 1040
/*
 * Display active window setup
 */

/*
#define CONFIG_DISP_ACT_STA_HPOS 405
#define CONFIG_DISP_ACT_END_HPOS 1686
#define CONFIG_DISP_ACT_STA_VPOS 40
#define CONFIG_DISP_ACT_END_VPOS 1064
#define CONFIG_DISP_HSYNC_LASTLINE 1704
*/
#define CONFIG_DISP_ACT_STA_HPOS 32
#define CONFIG_DISP_ACT_END_HPOS 1312
#define CONFIG_DISP_ACT_STA_VPOS 16
#define CONFIG_DISP_ACT_END_VPOS 1040
#define CONFIG_DISP_HSYNC_LASTLINE 1408
#define CONFIG_DISP_DCLK_DELAY 0

#endif
#define _CONFIG_DISP_ACT_STA_BIOS 	0x00
#define CONFIG_DEFAULT_DPLL_M_DIVIDER	0x62
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

