/*=============================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2005
  * All rights reserved.
  * ============================================ */

/*================= File Description ================= */
/**
 * @file
 * The file is a declaration and definition header file
 *
 * @author 	$Author:  $
 * @date 	$Date:  $
 * @version 	$Revision:  $
 * @ingroup
 */

/**
* @addtogroup
* @
*/
#ifndef _MODE_H
#define _MODE_H
#include "sparrow.h"
#include "flow_struct.h"
#include "scaler\scaler.h"

//--------------------------------------------------
// Defination of Mode Polarity
//--------------------------------------------------
#define _SYNC_HN_VN                 0x01
#define _SYNC_HP_VN                 0x02
#define _SYNC_HN_VP                 0x04
#define _SYNC_HP_VP                 0x08

#define _OVER_LAP_MODE_DOS  	0x10
#define _OVER_LAP_MODE_CVT	    0x20
#define _OVER_LAP_MODE		    0x40

#define _MAX_VFREQ                   800

#ifdef CONFIG_APSYSTEM_HS
#define _HFREQ_TOLERANCE      10
#else
#define _HFREQ_TOLERANCE      10
#endif
#define _VFREQ_TOLERANCE      10

enum _MODE_RESULT
{
    _MODE_NOSIGNAL	= 0xFF,
    _MODE_NOSUPPORT	= 0xFE,
    _MODE_NOCABLE	= 0xFD,
    _MODE_SUCCESS	= 0xFC,
    _MODE_DETECT	= 0xFB,
    _MDOE_UNDKOWN	= 0xFA,
};

typedef struct
{
    UINT16 ModeCurr;			// Input Mode
    UINT8 PolarityFlag;			//
    UINT16 IHWidth;				// Input Horizontal Width
    UINT16 IVHeight;				// Input Vertical Height
    UINT16 IHFreq;				// Input Horizontal Frequency
    UINT16 IVFreq;				// Input Vertical Frequency
    UINT8 IHFreqTolerance;		// Input Horizontal Frequency Tolerance
    UINT8 IVFreqTolerance;		// Input Vertical Frequency Tolerance
    UINT16 IHTotal;				// Input Horizontal Total Length
    UINT16 IVTotal;				// Input Vertical Total Length
    UINT16 IHStartPos;			// Input Horizontal Start Position
    UINT16 IVStartPos;			// Input Vertical Start Position
    UINT16 IVCount;				// Input Vertical count

} ModeTableType;


typedef struct
{
    UINT8 num;
    ModeTableType*	ModeTableType;
} ModeTableTypeTable;

// Notice only used as mode
typedef struct
{
    UINT8 ModeCurr;  				// Input Mode
    UINT8 Polarity;  					// BIT0: H Polarity, Bit1: V Polarity
    UINT8 status;  					// The status of input source
    //Bit0: 1 - 422 format, 0 - 444 format for signal
    // Bit1: 1 - Interlace mode in input, 0 - progressive mode in input
    UINT16 IHFreq;					// Input Horizontal Frequency
    UINT16 IVFreq;					// Input Vertical Frequency
    UINT16 IHTotal;					// Input Horizontal Total Length
    UINT16 IVTotal;					// Input Vertical Total Length
    UINT16 IHWidth;   				// Input Horizontal Width
    UINT16 IVHeight;  				// Input Vertical Height
    UINT16 IHStartPos;				// Input Horizontal Start Position
    UINT16 IVStartPos;				// Input Vertical Start Position
    UINT16 IHCount;   				// Input Horizontal Count Numbers  (measuement unit: Crystal clock)
    UINT16 IVCount;   				// Input Vertical Count Numbers	 (measuement unit: HS)
    UINT16 IHSyncPulseCount;  		// Input Horizontal Sync Pulse Count Numbers
} ModeInformationType;
extern ModeInformationType xdata stModeInfo;

#define _MAX_HDMI_MODE           	15

extern ModeTableType code tINPUTMODE_PRESET_TABLE[];
extern ModeTableTypeTable stModeTableTypeTable;
//extern UINT8 code EDID_KEY_TABLE[];
extern UINT16 code tVIDEO8_PRESET_TABLE[][5];
extern UINT16 code tAV_PRESET_TABLE[][5];
extern UINT16 code tSV_PRESET_TABLE[][5];


#define flow_mode_Enable_OnMeasure_Watchdog() drv_scaler_enable_measure_wdg(1)
#define flow_mode_Disable_OnMeasure_Watchdog() drv_scaler_enable_measure_wdg(0)

UINT8 mode_SearchModeMap(UINT8 modeno);

/*============================================================================*/
/**
 * flow_mode_DisableChannelProc
 * Do some process for close channel
 *
 * @param <void>	{ void }
 * @return 			{ void }
 *
 */
void flow_mode_DisableChannelProc(void);
void flow_mode_PoweroffProc(void);
/*============================================================================*/
/**
 * flow_mode_ResetMode
 * Do reset process for modestate
 *
 * @param <void>	{ void }
 * @return 			{ void }
 *
 */
void flow_mode_ResetMode(void);

/*============================================================================*/
/**
 * flow_mode_SetVGIP
 * Do reset process for modestate
 *
 * @param <void>	{ void }
 * @return 			{ void }
 *
 */
void flow_mode_SetVGIP(void);

/*============================================================================*/
/**
 * flow_mode_SetIDomain
 * Force to background color
 *
 * @param <void>	{ void }
 * @return 			{ void }
 *
 */
void flow_mode_SetIDomain(void);

/*============================================================================*/
/**
 * flow_mode_ClearOnMeasureflag
 * Clear status in on-line measure
 *
 * @param <void>		{ void }
 * @return 			{ void }
 *
 */
void flow_mode_ClearOnMeasureflag(void);



bit flow_mode_CheckSignalStable(void);

UINT8 flow_mode_detectmode(UINT8 source);

void flow_mode_GetVGAModeCurrInfo(UINT8 modeIndex, StructScreenModeInfo* ptModeInfo);
void flow_mode_GetScreenModeCurrInfo(UINT8 srcIndex, StructScreenModeInfo* ptModeInfo);
UINT8 mode_identifymode(UINT8 modeno);
//UINT8 mode_judge_confuse_mode(UINT8 mode_index,UINT16 usHighPulsePeriod);
void flow_mode_Timing_Change_Init(void);
void mode_setinfofromtable(UINT8 modeno);
UINT16 mode_get_IVTotal(void);


// For mode table
#define GET_MODE_INPUT_MODE(x)	(x & 0x00ff)
#define GET_MODE_SOURCE_TYPE(x)	((x & 0xff00) >> 8)

//UINT16 flow_Get_SrcMode_Parameter(UINT8 idx, UINT16 offset, UINT8 len);
UINT8 flow_Get_Preset_Table_For_VDC(UINT8 type, UINT8 field, ModeInformationType* stModeInfo);

#define FLOW_GET_MODE_INFO_PARAMETER(index, ELEMENT)	\
		flow_Get_SrcMode_Parameter(index, OFFSET_STRUCT(ModeTableType, ELEMENT), SIZEOF_STRUCT(ModeTableType, ELEMENT))

#define GET_ModeCur(idx)		(FLOW_GET_MODE_INFO_PARAMETER(idx, ModeCurr))
#define GET_PolarityFlag(idx)	(FLOW_GET_MODE_INFO_PARAMETER(idx, PolarityFlag))
#define GET_IHWidth(idx)	(FLOW_GET_MODE_INFO_PARAMETER(idx, IHWidth))
#define GET_IVHeight(idx)	(FLOW_GET_MODE_INFO_PARAMETER(idx, IVHeight))
#define GET_IHFreq(idx)	(FLOW_GET_MODE_INFO_PARAMETER(idx, IHFreq))

#define GET_IVFreq(idx)	(FLOW_GET_MODE_INFO_PARAMETER(idx, IVFreq))
#define GET_IHFreqTolerance(idx)	(FLOW_GET_MODE_INFO_PARAMETER(idx, IHFreqTolerance))
#define GET_IVFreqTolerance(idx)	(FLOW_GET_MODE_INFO_PARAMETER(idx, IVFreqTolerance))
#define GET_IHTotal(idx)	(FLOW_GET_MODE_INFO_PARAMETER(idx, IHTotal))
#define GET_IVTotal(idx)	(FLOW_GET_MODE_INFO_PARAMETER(idx, IVTotal))

#define GET_IHStartPos(idx)	(FLOW_GET_MODE_INFO_PARAMETER(idx, IHStartPos))
#define GET_IVStartPos(idx)	(FLOW_GET_MODE_INFO_PARAMETER(idx, IVStartPos))
#define GET_IVCountidx)	(FLOW_GET_MODE_INFO_PARAMETER(idx, IVCount))

#endif // #ifndef _MODE_H
