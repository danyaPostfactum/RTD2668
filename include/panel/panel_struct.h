//
//
//
#ifndef _PANEL_STRUCT_H
#define _PANEL_STRUCT_H

#include "sparrow.h"
//--------------------------------------------------
// Definitions for Panel interface Settings
//--------------------------------------------------
#define _PANEL_INTERFACE_TTL                              0
#define _PANEL_INTERFACE_LVDS                             1
#define _PANEL_INTERFACE_RSDS                             2

//--------------------------------------------------
// Set Last Line Finetune Method
//--------------------------------------------------
#define _LAST_LINE_METHOD_NONE                  0
#define _LAST_LINE_METHOD_0                     1                   // Formula: Tne DEN End to DEN Start Length must be (1280*N)
#define _LAST_LINE_METHOD_1                     2                   // Formula: Tne DEN End to DEN Start Length must be (4096*N + 128)
#define _LAST_LINE_METHOD_2                     3                   // Formula: Set to target last line.

//--------------------------------------------------
// Definitions for Panel color bits Settings
//--------------------------------------------------
#define _PANEL_DISPLAY_COLOR_BITS_30			0
#define _PANEL_DISPLAY_COLOR_BITS_24			1
#define _PANEL_DISPLAY_COLOR_BITS_18			2

//--------------------------------------------------
// Definitions for Panel color bits Settings
//--------------------------------------------------
#define _PANEL_DISPLAY_PORT_SINGLE			0
#define _PANEL_DISPLAY_PORT_DOUBLE			1

//--------------------------------------------------
// Definitions for Panel color bits Settings
//--------------------------------------------------
#define _PANEL_DISPLAY_BITMAPPING_TABLE1		0
#define _PANEL_DISPLAY_BITMAPPING_TABLE2		1

typedef struct _STRUCT_PANEL_PARAMETER
{
    char name[16];
#if 0
    UINT16 fOptions;			// bit0 : CONFIG_DISPLAY_PORT 					// 0: single port, 1: double port
    // bit1:bit2 : CONFIG_DISPLAY_COLOR_BITS 		// 0: 30bits, 1:24 bits, 2: 30bits
    // bit3 : CONFIG_DISPLAY_EVEN_RSV1_BIT	 		// 0: Indicate 0, 1: Indicate 1
    // bit4 : CONFIG_DISPLAY_ODD_RSV1_BIT	 		// 0: Indicate 0, 1: Indicate 1
    // bit5 : CONFIG_DISPLAY_BITMAPPING_TABLE		// 0: Table1, 1:Table2
    // bit6 : CONFIG_DISPLAY_PORTAB_SWAP			// 0: No Swap, 1: Swap
    // bit7 : CONFIG_DISPLAY_RED_BLUE_SWAP			// 0: No Swap, 1: Swap
    // bit8 : CONFIG_DISPLAY_MSB_LSB_SWAP			// 0: No Swap, 1: Swap
    // bit9 : CONFIG_DISPLAY_SKEW_DATA_OUTPUT 		// 0: Disable, 1: Skew data output
    // bit10 : CONFIG_DISPLAY_OUTPUT_INVERSE			// 0: No Swap, 1: Swap
    // bit11: CONFIG_DISPLAY_VERTICAL_SYNC 			// 0: normal, 1: Invert
    // bit12: CONFIG_DISPLAY_HORIZONTAL_SYNC 		// 0: normal, 1: Invert
    // bit13: CONFIG_DISPLAY_VERTICAL_SYNC_NORMAL 	// 0: normal, 1: Invert
    // bit14: CONFIG_DISPLAY_HORIZONTAL_SYNC_NORMAL // 0: normal, 1: Invert
    // bit15: CONFIG_DISPLAY_RATIO_4X3 				// 0: 16:9 , 1: 4:3
#else
    UINT8 DISPLAY_PORT; 				// 0: single port, 1: double port
    UINT8 DISPLAY_COLOR_BITS;	 	// 0: 30bits, 1:24 bits, 2: 30bits
    UINT8 DISPLAY_EVEN_RSV1_BIT;	// 0: Indicate 0, 1: Indicate 1
    UINT8 DISPLAY_ODD_RSV1_BIT;	// 0: Indicate 0, 1: Indicate 1
    UINT8 DISPLAY_BITMAPPING_TABLE;		// 0: Table1, 1:Table2
    UINT8 DISPLAY_PORTAB_SWAP;			// 0: No Swap, 1: Swap
    UINT8 DISPLAY_RED_BLUE_SWAP;				// 0: No Swap, 1: Swap
    UINT8 DISPLAY_MSB_LSB_SWAP;			// 0: No Swap, 1: Swap
    UINT8 DISPLAY_SKEW_DATA_OUTPUT; 		// 0: Disable, 1: Skew data output
    UINT8 DISPLAY_OUTPUT_INVERSE;			// 0: No Swap, 1: Swap
    UINT8 DISPLAY_VERTICAL_SYNC; 			// 0: normal, 1: Invert
    UINT8 DISPLAY_HORIZONTAL_SYNC; 		// 0: normal, 1: Invert
    UINT8 DISPLAY_VERTICAL_SYNC_NORMAL;	 	// 0: normal, 1: Invert
    UINT8 DISPLAY_HORIZONTAL_SYNC_NORMAL;					 // 0: normal, 1: Invert
    UINT8 DISPLAY_RATIO_4X3;		// 0: 16:9 , 1: 4:3
#endif
    UINT8 nMaxClock;			// UNIT: MHz CONFIG_DISPLAY_CLOCK_MAX
    UINT8 nRefreshHz;			// UNIT: Hz	 CONFIG_DISPLAY_REFRESH_RATE
    UINT8 nTypicalClk;			// UNIT: MHz CONFIG_DISPLAY_CLOCK_TYPICAL
    /*
     * Display total window setup
     */
    UINT16	nHTotal;			// CONFIG_DISP_HORIZONTAL_TOTAL
    UINT16	nVTotal;			// CONFIG_DISP_VERTICAL_TOTAL

    /*
     * Display Sync Width setup
     */
    UINT8	nHSyncWidth;		// CONFIG_DISP_HSYNC_WIDTH
    UINT8	nVSyncLength;		// CONFIG_DISP_VSYNC_LENGTH 5

    /*
     * Display Enable window setup
     */
    UINT16	nDEHStart;		// CONFIG_DISP_DEN_STA_HPOS
    UINT16	nDEWidth;		//
    UINT16	nDEVStart;		// CONFIG_DISP_DEN_STA_VPOS
    UINT16	nDEVLength;		//

    /*
     * Display Enable window setup
     */
    UINT16	nActiveHStart;		// CONFIG_DISP_ACTIVE_STA_HPOS
    UINT16	nActiveWidth;		//
    UINT16	nActiveVStart;		// CONFIG_DISP_ACTIVE_STA_VPOS
    UINT16	nActiveVLength;		//

    /*
     * Display active window setup
     */
    UINT16	nHSyncLastLine;		// CONFIG_DISP_HSYNC_LASTLINE
    UINT8	nDClkDelay;			// CONFIG_DISP_DCLK_DELAY
    UINT8	nActStartBIOS;		// _CONFIG_DISP_ACT_STA_BIOS

    /*
     * Display PLL setup
     */
    UINT8	nDPLL_M;			// CONFIG_DEFAULT_DPLL_M_DIVIDER
    UINT8	nDPLL_N;			// CONFIG_DEFAULT_DPLL_N_DIVIDER

/////////////////////////////
//Off --> On Sequence
/////////////////////////////
    UINT16 nTimeforPanel2LVDSOn;	//UNIT: ms	PANEL_TO_LVDS_ON_ms 	// Delay(T1+T2): Panel Power --> LVDS Signal
    UINT16 nTimeforLVDS2LightOn; 	//UNIT: ms 	LVDS_TO_LIGHT_ON_ms		// Delay(T3):    Settings: LVDS Signal --> Backlight On

/////////////////////////////
//On --> Off	Sequence
/////////////////////////////
    UINT16 nTimeforLight2LVDSOff;	// UNIT:ms	LIGHT_TO_LDVS_OFF_ms	// Delay(T4):    Turn Off backlight and delay to turn off LVDS signal
    UINT16 nTimeforLVDS2PanelOff;	// UNIT:ms 	LVDS_TO_PANEL_OFF_ms	// Delay(T5+T6): LVDS Signal Off --> Panel Power Off

////////////////////////////
//Panel Off--> Next On
////////////////////////////
    UINT16 nTimePanelOff2On;		// UNIT:ms	PANEL_OFF_TO_ON_ms		// Delay(T7):    Totally Off --> Next On

} STRUCT_PANEL_PARAMETER;

extern STRUCT_PANEL_PARAMETER code stPanelData;

#define _DISP_WID 							(stPanelData.nActiveWidth)
#define _DISP_LEN  							(stPanelData.nActiveVLength)

// The definition of default value
#define CONFIG_PANEL_NAME_DEFAULT				"REALTEK"

#endif //#ifndef _PANEL_STRUCT_H
