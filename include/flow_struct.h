//
//
//
#ifndef _FLOW_STRUCT_H
#define _FLOW_STRUCT_H

#include "sparrow.h"
#include "ui_struct.h"
//#include "pcb\pcb_conf_struct.h"

#define VUZD_BUFFER_NR_MODE					_BIT0
#define VUZD_BUFFER_VUZD_MODE				_BIT1
#define VUZD_BUFFER_VUZD_NR_MODE			_BIT1|_BIT0
#define VUZD_BUFFER_BYPASS_MODE				0

// The number of channel
#define _CHANNEL_NUM    1

// The no. of main/sub display
#define _MAIN_DISPLAY 0
#define _SUB_DISPLAY 0

// The no. of channel1/channel2
#define _CHANNEL1	0
#define _CHANNEL2	1

// The no. of power state
enum POSER_STATE
{
    _POWER_STATE_OFF,
    _POWER_STATE_SAVING,
    _POWER_STATE_ON
};

//add for ATV Channel Status
typedef enum _CHANNEL_STATUS_TYPE
{
    AFC_SKIP = 0x02, // charlie, 20090122, for TV AFC skip
    CHANNEL_FAVORITE=0x03,
    CHILD_LOCK,
    CHANNEL_SKIP,
    PARENTAL_LOCK,
    CHANNEL_LOCK,
} Channel_Status_Type;


typedef enum
{
    RATIO_4_3=0,
    RATIO_16_9=1,
    RATIO_AUTO=2,
    RATIO_PANORAMA=3,
    RATIO_DISABLE = 0x0f,
} _RATIO_TYPE;

typedef enum
{
    IR_OFF=0,
    IR_A=1,
    IR_B=2,
} _IR_TYPE;


#if CONFIG_ENABLE_FM
typedef enum
{
    RADIO_OFF=0,
    RADIO_877=1,
    RADIO_879=2,
    RADIO_881=3,
    RADIO_883=4,
    RADIO_885=5,
    RADIO_887=6,
    RADIO_889=7,
    RADIO_891=8, 
} _RADIO_TYPE;
#endif

typedef struct
{
    int fPowerState:2;
    int fSysState:2;
    int fChannelState:2;
    int fLogLevel:2;
} SYS_STRUCT;


//--------------------------------------------------
// TV information Struct
//--------------------------------------------------
typedef struct
{
    UINT16 CurChn;//for ATV / DTV majorNum
    UINT16 CurMinorNum;//for DTV minorNum
    UINT32 CurFreq;
    UINT8 CurTVType;//0:ATV 1:DTV
    UINT8 PreTVType;//0:ATV 1:DTV
    UINT16 PreChn;//for ATV / DTV majorNum
    UINT16 PreMinorNum;//for DTV minorNum
    UINT32 PreFreq;
} StructChannelDataType;

typedef struct
{
    UINT16 DHTotal;                   // Display Horizontal Total Length
    UINT16 DHWidth;                   // Display Horizontal Width
    UINT16 DVHeight;                  // Display Vertical Height
    UINT16 DVStartPos;                // Display Vertical Start Position

} DisplayTableType;

typedef struct _StructDisplayInfo
{
    UINT8  channel:1;          	// input channel
    UINT8  input_src:4;       	//!  input source of this display
    UINT8  ucMode_Curr;			// the index in mode table

    UINT8  state:4; 		    //! the state of this display (Search, Active ...)

    UINT8  disp_status; 		//! the status of this display
    // Bit0: 1 - 422 format, 0 - 444 format for signal
    // Bit1: 1 - Interlace mode in input, 0 - progressive mode in input

    UINT8 Polarity:4;  			// BIT0: H Polarity, Bit1: V Polarity
    UINT8 CurrZoomMode:2;			//
    UINT8 UserZoomMode:2;
	UINT8 IR_CTRL;
	#if CONFIG_ENABLE_FM
	UINT16 FM_CTRL;
	#endif
    UINT16 ADC_CLOCK;
    UINT16 Hsync;
    UINT16 Vsync;

    UINT16 IHFreq;					// Input Horizontal Frequency
    UINT16 IVFreq;					// Input Vertical Frequency
    UINT16 IHTotal;   				// Input Horizontal Total Length
    UINT16 IVTotal;   				// Input Vertical Total Length
    UINT16 IHCount_PRE;   			// Input Horizontal Count from off-line measure
    UINT16 IVCount_PRE;   			// Input Vertical Count from off-line measure
    UINT16 IHSyncPulseCount;  		// Input Horizontal Sync Pulse Count Numbers

    UINT16 INPUT_ADC_CLOCK;
    UINT16 IPH_SMARTFIT_START;//for keep smart fit ms Hstart
    UINT16 IPV_SMARTFIT_START;//for keep smart fit ms Vstart


    UINT8 ScaleOption:4;		///! scaling-related parameters

    // Active size after measure
    UINT16 IPH_ACT_STA_PRE;		// The input H start after measure
    UINT16 IPV_ACT_STA_PRE;		// The input H size after measure
    UINT16 IPH_ACT_WID_PRE;		// The input V start after measure
    UINT16 IPV_ACT_LEN_PRE;		// The input V size after measure
    UINT16 IPV_DLY_PRE;

    // Active size after VGIP
    UINT16 IPH_ACT_STA;			// The input H start after VGIP
    UINT16 IPV_ACT_STA;			// The input H size after VGIP
    UINT16 IPH_ACT_WID;			// The input V start after VGIP
    UINT16 IPV_ACT_LEN;			// The input V size after VGIP

    UINT16 DispWid;      		// display widnow width
    UINT16 DispLen;            	// display window length

    UINT8	FailureCount:4;	// Recoder failure count
#if 0
    UINT8 	data_array_index;
    UINT8	IsBlock;//for vchip
    UINT8 IPV_DLY_PRE;
    UINT16 ADC_CLOCK;
    UINT16 IPH_SMARTFIT_START;//for keep smart fit ms Hstart
    UINT16 IPV_SMARTFIT_START;//for keep smart fit ms Vstart

    UINT8 ScaleOption;		///! scaling-related parameters
    UINT16 Hsync;
    UINT16 Vsync;
    UINT8 ModeSearchCount;
    UINT8 FailureCount;		// Recoder failure count
    UINT8 FrameSyncFlag;	//_TRUE: Frame Sync Mode, _FALSE: FRC Mode
    StructSourceInput	*SrcInfo;

    UINT8 Polarity;  				// BIT0: H Polarity, Bit1: V Polarity
    UINT16 IHFreq;					// Input Horizontal Frequency
    UINT16 IVFreq;					// Input Vertical Frequency
    UINT16 IHTotal;   				// Input Horizontal Total Length
    UINT16 IVTotal;   				// Input Vertical Total Length
    UINT16 IHCount_PRE;   			// Input Horizontal Count from off-line measure
    UINT16 IVCount_PRE;   			// Input Vertical Count from off-line measure
    UINT16 IHSyncPulseCount;  		// Input Horizontal Sync Pulse Count Numbers
    UINT8  bg_color_r;		// for background color r
    UINT8  bg_color_g;		// for background color g
    UINT8  bg_color_b;		// for background color b
#endif

} StructDisplayInfo;
/*
typedef struct _StructScreenModeInfo
{
      UINT8 unIndex;
      UINT8 H_Position;
      UINT8 V_Position;
      UINT8 Clock;
      UINT8 Phase;
      UINT8 H_Size;
      UINT8 V_Size;
//      UINT8 Display_Range;
//      UINT8 Open_Count; 		//jerry0108 CSW+ 0961123 Run VGA auto-adjust function at first time display
} StructScreenModeInfo;
*/
typedef struct _StructScreenModeInfoSet
{
    UINT8 nModeinfoNum;
    StructScreenModeInfo* ptModeInfo;
} StructScreenModeInfoSet;

#if 0
typedef struct _ADCGainOffset
{
    UINT16 Gain_R;	/*In fact, 10bits */
    UINT16 Gain_G;	/*In fact, 10bits */
    UINT16 Gain_B;	/*In fact, 10bits */
    UINT16 Offset_R;	/*In fact, 10bits */
    UINT16 Offset_G;	/*In fact, 10bits */
    UINT16 Offset_B;	/*In fact, 10bits */
} ADCGainOffset;
#endif

extern SYS_STRUCT xdata sysdata;
extern StructDisplayInfo xdata disp_info;
extern StructDisplayInfo* xdata info;
extern StructScreenModeInfoSet code stVGAModeCurrInfoSet;
extern StructScreenModeInfoSet code stScreenModeCurrInfoSet;
extern StructScreenModeInfo xdata stVGAModeCurrInfo;
extern StructScreenModeInfo xdata stScreenModeCurrInfo;
extern DisplayTableType               stDisplayInfo;


#define SET_422_FORMAT()          				(disp_info.disp_status |= _BIT0)
#define CLR_422_FORMAT()						(disp_info.disp_status &= ~_BIT0)
#define GET_422_FORMAT()						(disp_info.disp_status & _BIT0)

#define SET_INTERLACE_MODE()				(disp_info.disp_status |= _BIT1)
#define CLR_INTERLACE_MODE()				(disp_info.disp_status &= (~_BIT1))
#define GET_INTERLACE_MODE()				(disp_info.disp_status & _BIT1)

#define GET_VUZD_BUFFER_MODE()		(disp_info.disp_status & (_BIT3|_BIT4))
#define SET_VUZD_BUFFER_MODE( x)			(disp_info.disp_status = ((disp_info.disp_status & ~(_BIT3|_BIT4)) | x)) //00: by pass, 01:NR, 02:VUZD, 11:invalid

#define SET_INTERLACE_IN_INPUT_MODE()			(disp_info.disp_status |= _BIT1)
#define CLR_INTERLACE_IN_INPUT_MODE()			(disp_info.disp_status &= (~_BIT1))
#define GET_INTERLACE_IN_INPUT_MODE()			((disp_info.disp_status & _BIT1) >> 1)

#define SET_SHOW_STATUS()			(disp_info.disp_status |= _BIT5)
#define CLR_SHOW_STATUS()			(disp_info.disp_status &= (~_BIT5))
#define GET_SHOW_STATUS()			((disp_info.disp_status & _BIT5) >> 5)


// For Scale down&up
#define SET_VSCALE_UP()							(disp_info.ScaleOption |= _BIT0)
#define CLR_VSCALE_UP()							(disp_info.ScaleOption &= (~_BIT0))
#define GET_VSCALE_UP()							(disp_info.ScaleOption & _BIT0)

#define SET_VSCALE_DOWN()						(disp_info.ScaleOption |= _BIT1)
#define CLR_VSCALE_DOWN()						(disp_info.ScaleOption &= (~_BIT1))
#define GET_VSCALE_DOWN()						(disp_info.ScaleOption & _BIT1)

#define SET_HSCALE_UP()							(disp_info.ScaleOption |= _BIT2)
#define CLR_HSCALE_UP()							(disp_info.ScaleOption &= (~_BIT2))
#define GET_HSCALE_UP()							(disp_info.ScaleOption & _BIT2)

#define SET_HSCALE_DOWN()						(disp_info.ScaleOption |= _BIT3)
#define CLR_HSCALE_DOWN()						(disp_info.ScaleOption &= (~_BIT3))
#define GET_HSCALE_DOWN()						(disp_info.ScaleOption & _BIT3)

// For OSD Display ratio
#define GET_OSD_DISPLAY_RATIO()						(disp_info.UserZoomMode)
#define SET_OSD_DISPLAY_RATIO(x)					(disp_info.UserZoomMode = x)

// For Display ratio
#define GET_DISPLAY_RATIO()						(disp_info.CurrZoomMode)
#define SET_DISPLAY_RATIO(x)					(disp_info.CurrZoomMode = x)

// For OSD Display  IR
#define GET_OSD_DISPLAY_IR()						(disp_info.IR_CTRL)
#define SET_OSD_DISPLAY_IR(x)					(disp_info.IR_CTRL = x)
#if CONFIG_ENABLE_FM
#define GET_OSD_DISPLAY_Radio()						(disp_info.FM_CTRL)
#define SET_OSD_DISPLAY_Radio(x)					(disp_info.FM_CTRL = x)

#endif

// For Display IR 
//#define GET_DISPLAY_IR()						(disp_info.IR_CTRL)
//#define SET_DISPLAY_IR(x)					(disp_info.IR_CTRL = x)


#define IS_RATIO_16_9() (GET_DISPLAY_RATIO() == RATIO_16_9)
#define IS_RATIO_4_3() (GET_DISPLAY_RATIO() == RATIO_4_3)
#define IS_RATIO_PANORAMA() (GET_DISPLAY_RATIO() == RATIO_PANORAMA)
#define IS_RATIO_AUTO() (GET_DISPLAY_RATIO() == RATIO_AUTO)


#define GET_AUTO_ADJUST_DONE() 			(stVGAModeCurrInfo.Status & _BIT0)
#define SET_AUTO_ADJUST_DONE() 			(stVGAModeCurrInfo.Status |= _BIT0)
#define GET_JUMP_PHASE_H_POSITION_COMP() (stVGAModeCurrInfo.Status & _BIT1)
#define SET_JUMP_PHASE_H_POSITION_COMP() (stVGAModeCurrInfo.Status |= _BIT1)
#if 0
#define GET_VGA_USER_MODE() (stVGAModeCurrInfo.Status & _BIT2)
#define SET_VGA_USER_MODE() (stVGAModeCurrInfo.Status |= _BIT2)
#define CLR_VGA_USER_MODE() (stVGAModeCurrInfo.Status &= (~_BIT2))
#endif

#define GET_H_POSITION() (stScreenModeCurrInfo.H_Position)
#define SET_H_POSITION(x) (stScreenModeCurrInfo.H_Position = x)

#define GET_V_POSITION() (stScreenModeCurrInfo.V_Position)
#define SET_V_POSITION(x) (stScreenModeCurrInfo.V_Position = x)

#define GET_CLOCK() (stScreenModeCurrInfo.Clock)
#define SET_CLOCK(x) (stScreenModeCurrInfo.Clock = x)

#define GET_PHASE() (stScreenModeCurrInfo.Phase)
#define SET_PHASE(x) (stScreenModeCurrInfo.Phase = x)


#define GET_CURRENT_MODE()			info->ucMode_Curr//(0)
#define GET_IV_FREQ()				info->IVFreq//(0)
#define GET_ACT_WID()				info->IPH_ACT_WID//(0)
#define GET_ACT_PRE_WID()			info->IPH_ACT_WID_PRE//(0)
#define GET_ACT_PRE_LEN()			info->IPV_ACT_LEN_PRE//(0)

#define GET_HDMI_MEASURE_IVH()		info->IPV_ACT_LEN//(0)

#endif //
