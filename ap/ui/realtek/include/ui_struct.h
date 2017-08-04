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
#ifndef _UI_STRUCT_H
#define _UI_STRUCT_H

/*============================ Module dependency  ===========================*/
#include "rtd_types.h"
#include "RT_storage.h"
#include "autoconf.h"
//--------------------------------------------------
// Version Code
//--------------------------------------------------

#define _VERSION_CHAR2		0
#define _VERSION_CHAR1		2
#define _VERSION_CHAR0		8

//#define _VERSION_CODE		(_VERSION_CHAR2*100+_VERSION_CHAR1*10+_VERSION_CHAR0)
//#define _DISABLE_RT_STORAGE_FOR_TEST	1
#define	_MAX_CATV_NUM 	    200//Ding modify 08-01-31 //255

#define _ADC_TYPE_TOTAL         2
#define _COLORTTEMP_SRC_TYPE_TOTAL   4
#define _COLORTTEMP_MODE_TOTAL		 3

#define _DEFAULT_VALUE_USING_BANK		_BANK7
//#define _STORAGE_VALUE_USING_BANK	_BANK7
//#define STORAGE_DEFAULT_BANK			0x00070000UL
//#define STORAGE_BANK					0x00070000UL
//#define FIRSTRUN_VAR_BANK				0x00050000UL

/**
 * Aging mode macro
 */
#define _AGING_MODE_ON			(0x5A)
#define _AGING_MODE_OFF			(0x00)

#define _ADC_INPUT_TEST			1
#define _FORCE_SELECT_YPBPR		0
#define _FORCE_SELECT_VGA			1


//--------------------------------------------------
// Struct Type
//--------------------------------------------------
typedef struct
{
    UINT8 Language;
    UINT8 AgingMode;
    UINT8 OsdTimeout;
    UINT8 OsdBlending;
    UINT8 PictureMode;
    UINT8 ParentalLock;
    UINT8 ucScartFlag;
} StructOsdUserDataType;

typedef struct
{
    UINT8 TVMapIndex[_MAX_CATV_NUM];
} StructTVChannelMapTable;

typedef struct
{
    UINT8 BlueScreen;
    UINT8 CurChn;
    UINT8 PreChn;
    UINT8 ucTotalChn;
    UINT8 CountrySelect;

	UINT8 BlueScreen_Cable;
	UINT8 CurChn_Cable;
	UINT8 PreChn_Cable;
	UINT8 ucTotalChn_Cable;
	UINT8 CountrySelect_Cable;

	UINT8 BlueScreen_Air;
	UINT8 CurChn_Air;
	UINT8 PreChn_Air;
	UINT8 ucTotalChn_Air;
	UINT8 CountrySelect_Air;
	
    UINT8 NtscType;
    UINT8 FreeType;
} StructTvDataType;

enum _eq_mode_id
{
    _EQ_MODE_STANDARD = 0,
    _EQ_MODE_CLUB,
    _EQ_MODE_DANCE,
    _EQ_MODE_LIVE,
    _EQ_MODE_POP,
    _EQ_MODE_ROCK,
    _EQ_MODE_USER,
    _EQ_MODE_COUNT
};

enum _mts_mode_id
{
    _MTS_MONO = 0,
    _MTS_STEREO,
    _MTS_SAP,
    _MTS_COUNT
};

typedef struct
{
    UINT8 EqMode;
    UINT8 Volumn;
    UINT8 Spatial;
    UINT8 Balance;
    UINT8 Bass;
    UINT8 Treble;
    UINT8 AutoVolume;  // AVL
    UINT8 Compensation;  //Speaker Compensation
    UINT8 MTS;
} StructAudioDataType;

enum _picture_mode_id
{
    _PIC_MODE_VIVID = 0,
    _PIC_MODE_STANDARD,
    _PIC_MODE_GENTLE,
    _PIC_MODE_USER,
    _PIC_MODE_COUNT
};
typedef struct
{
    UINT8 Brightness;
    UINT8 Contrast;
    UINT8 Hue;
    UINT8 Saturation;
    UINT8 Sharpness;
    UINT8 DNR;
    UINT8 DCR;
    UINT8 Intensity;
    UINT8 ColorTemperature;
    UINT8 BackLight;
} StructColorDataType;


typedef struct
{
    UINT8 I_DLti;
    UINT8 I_DCti;
    UINT8 Gamma;
    UINT8 Digital_Filter;
    UINT8 SUNR_Y;
    UINT8 SUNR_C;
    UINT8 SUPK_MASK;

    UINT8 DccLevel;
    UINT8 D_DLti;
    UINT8 D_DCti;
    UINT8 ScaleUpH;
    UINT8 ScaleUpV;
    UINT8 ScalePK;
    UINT8 VD_Hue;
    UINT8 VD_Sat;
    UINT8 VD_Y_Contrast;
    UINT8 VD_Y_Brightness;
    UINT8 ICM_Table;
    UINT8 Dither_Bit;
    UINT8 S_Index;
    UINT8 S_Low;
    UINT8 S_High;
//	UINT8 NRSpatial;
//	UINT8 Peaking;
} StructFacColorDataType;


typedef struct
{
    UINT8 AgingMode;
    UINT8 DclkRange;
    UINT8 DclkDiv;
    UINT8 Contrast;
    UINT8 RFStartDb;
    INT16 Volume_Curve[5];
    INT16 TV_Volume_Curve[5];
    UINT8 Carrier_Mute;
    UINT8 BackLight_Control[5];
    UINT8 PQ_Demo;
} StructFacModeDataType;

typedef struct StructScreenModeInfo
{
    UINT8 Status;
    UINT8 H_Position;
    UINT8 V_Position;
    UINT8 Clock;
    UINT8 Phase;
    UINT8 H_Size;
    UINT8 V_Size;
    UINT8 Display_Range;
} StructScreenModeInfo;

typedef StructScreenModeInfo StructVGAScreenModeInfo;


typedef struct _ADCGainOffset
{
    UINT16 Gain_R;	/*In fact, 10bits */
    UINT16 Gain_G;	/*In fact, 10bits */
    UINT16 Gain_B;	/*In fact, 10bits */
    UINT16 Offset_R;	/*In fact, 10bits */
    UINT16 Offset_G;	/*In fact, 10bits */
    UINT16 Offset_B;	/*In fact, 10bits */
} ADCGainOffset;

typedef ADCGainOffset ADCGainOffset1;
typedef ADCGainOffset ADCGainOffset2;

typedef struct
{
    UINT8 Version;

    UINT8 MonitorFlag;	// Bit 7:  Monitor Power Status
    // Bit 6:  Reserved
    // Bit 5:  Factory menu flag
    // Bit 4:  XContrast on/off
    // Bit 1~Bit3: BurnIn Pattern Index
    // Bit 0:  BurnIn Mode on off

    UINT8 InputSource;
    UINT8 UserFIFOMode;
    UINT16 ZoomMode;
    UINT8 Mode;
	UINT16 IrMode;
	#if CONFIG_ENABLE_FM
	UINT16 FMMode;
	#endif
	UINT8 DimmerVaule;
    UINT8 nViviColor;
    UINT8 BootSource;
    UINT8 PictureMode;
    UINT8 ScreenSaver;
    UINT8 AudioAutoDemo;
    UINT8 AudioAutoDemoType;
    UINT8 ColorSystem;
    UINT8 CcSystem;
    UINT8 VChipType;
    UINT8 UartPath;
    UINT8 AcOffPowerStatus;//Hardy20091203
} StructSystemDataType;

typedef  struct
{
    UINT32 freq;	//		[ channel frequency  			]

    UINT8 chnNo;		//

    UINT8 colorSystem;
    // bit 7~0 : Color System
    UINT8 soundSystem;
    // bit 2~0
    //		[Sound System				]
    //          	[ 000  :  PAL BGH / SECAM BG	]
    //          	[ 001  :  PAL DK / SECAM DK	]
    //          	[ 010  :  PAL I		            		]
    //          	[ 011  :  SECAM LL'			]

    UINT8 status;
    //bit 7    [ Auto Scan channel Lock status	]
    //		[ 0:channel scan unlocked   		]
    //		[ 1:channel scan locked       		]
    //bit 6    [ Parental Lock status			]
    //		[ 0:parental unlocked 	     		]
    //		[ 1:parental locked              		]
    //bit 5    	[ Channel skip status           		]
    //		[ 0: not skip channel       			]
    //          	[ 1: skip channel           		]
    //bit 4 ~0 :reserved

    UINT8 offset;

    // modify by linmc, for channel edit, 20080919
#ifdef CONFIG_STORAGE_ACCESS_FLASH	// marked by weihao because of china
    UINT8 name[5];		//channel name
#endif
} StructUserDataChannelSettingsType;

//typedef struct
//{
//	UINT8 offset;
//}StructChannelOffsetType;

//Cable Storage
typedef struct StructTvCableDataType
{
    UINT32 freq;    //      [ channel frequency             ]

    UINT8 chnNo;        //

    UINT8 colorSystem;
    // bit 7~0 : Color System
    UINT8 soundSystem;
    // bit 2~0
    //      [Sound System               ]
    //              [ 000  :  PAL BGH / SECAM BG    ]
    //              [ 001  :  PAL DK / SECAM DK ]
    //              [ 010  :  PAL I                         ]
    //              [ 011  :  SECAM LL'         ]

    UINT8 status;
    //bit 7    [ Auto Scan channel Lock status  ]
    //      [ 0:channel scan unlocked           ]
    //      [ 1:channel scan locked             ]
    //bit 6    [ Parental Lock status           ]
    //      [ 0:parental unlocked               ]
    //      [ 1:parental locked                     ]
    //bit 5     [ Channel skip status                   ]
    //      [ 0: not skip channel                   ]
    //              [ 1: skip channel                   ]
    //bit 4 ~0 :reserved

    UINT8 offset;

    // modify by linmc, for channel edit, 20080919
#ifdef CONFIG_STORAGE_ACCESS_FLASH	// marked by weihao because of china
    UINT8 name[5];      //channel name
#endif
    UINT8 NtscType;

} StructTvCableDataType;

//Air Storage
typedef struct StructTvAirDataType
{
    UINT32 freq;    //      [ channel frequency             ]

    UINT8 chnNo;        //

    UINT8 colorSystem;
    // bit 7~0 : Color System
    UINT8 soundSystem;
    // bit 2~0
    //      [Sound System               ]
    //              [ 000  :  PAL BGH / SECAM BG    ]
    //              [ 001  :  PAL DK / SECAM DK ]
    //              [ 010  :  PAL I                         ]
    //              [ 011  :  SECAM LL'         ]

    UINT8 status;
    //bit 7    [ Auto Scan channel Lock status  ]
    //      [ 0:channel scan unlocked           ]
    //      [ 1:channel scan locked             ]
    //bit 6    [ Parental Lock status           ]
    //      [ 0:parental unlocked               ]
    //      [ 1:parental locked                     ]
    //bit 5     [ Channel skip status                   ]
    //      [ 0: not skip channel                   ]
    //              [ 1: skip channel                   ]
    //bit 4 ~0 :reserved

    UINT8 offset;

    // modify by linmc, for channel edit, 20080919
#ifdef CONFIG_STORAGE_ACCESS_FLASH	// marked by weihao because of china
    UINT8 name[5];      //channel name
#endif
    UINT8 NtscType;

} StructTvAirDataType;

typedef struct
{
    UINT8 WBH_Bri;
    UINT8 WBH_Cont;
    UINT8 WBL_Bri;
    UINT8 WBL_Cont;
    UINT8 LogoType;
    UINT8 DefaultLanguage;
    UINT8 PictureMode_Bri[4];
    UINT8 PictureMode_Cont[4];
    UINT8 PictureMode_Sat[4];
    UINT8 PictureMode_Hue[4];
    UINT8 PictureMode_Sharpness[4];
    UINT8 Brightness_0;
    UINT8 Brightness_50;
    UINT8 Brightness_100;
    UINT8 Contrast_0;
    UINT8 Contrast_50;
    UINT8 Contrast_100;
    UINT8 Saturation_0;
    UINT8 Saturation_50;
    UINT8 Saturation_100;
    UINT8 EQ_300[7];
    UINT8 EQ_1k[7];
    UINT8 EQ_2k[7];
    UINT8 EQ_3k[7];
    UINT8 EQ_8k[7];
    UINT8 EQ_15k[7];
} StructCustomerFacDataType;

enum _Audio_Data_ID
{
    _DATA_EQ_MODE,
    _DATA_VOLUME,
    _DATA_SPATIAL,
    _DATA_BALANCE,
    _DATA_BASS,
    _DATA_TREBLE,
    _DATA_AVL,
    _DATA_COMPENSATION,
    _DATA_MTS,
};

void ui_set_audio_data(UINT8 id, UINT8 ucVal);
UINT8 ui_get_audio_data(UINT8 id);
#define GET_EQ_MODE()				ui_get_audio_data(_DATA_EQ_MODE)
#define SET_EQ_MODE(_val)			ui_set_audio_data(_DATA_EQ_MODE, _val)
#define GET_VOLUME()				ui_get_audio_data(_DATA_VOLUME)
#define SET_VOLUME(_val)			ui_set_audio_data(_DATA_VOLUME, _val)
#define GET_SPATIAL()				ui_get_audio_data(_DATA_SPATIAL)
#define SET_SPATIAL(_val)			ui_set_audio_data(_DATA_SPATIAL, _val)
#define GET_BALANCE()				ui_get_audio_data(_DATA_BALANCE)
#define SET_BALANCE(_val)			ui_set_audio_data(_DATA_BALANCE, _val)
#define GET_BASS()					ui_get_audio_data(_DATA_BASS)
#define SET_BASS(_val)				ui_set_audio_data(_DATA_BASS, _val)
#define GET_TREBLE()				ui_get_audio_data(_DATA_TREBLE)
#define SET_TREBLE(_val)			ui_set_audio_data(_DATA_TREBLE, _val)
#define GET_MTS()				ui_get_audio_data(_DATA_MTS)
#define SET_MTS(_val)			ui_set_audio_data(_DATA_MTS, _val)
enum _PQ_DATA_ID
{
    _PQ_DATA_BRIGHTNESS,
    _PQ_DATA_CONTRAST,
    _PQ_DATA_HUE,
    _PQ_DATA_SATURATION,
    _PQ_DATA_SHARPNESS,
    _PQ_DATA_DNR,
    _PQ_DATA_DCR,
    _PQ_DATA_INTENSTIY,
    _PQ_DATA_COLOR_TEMPERATURE,
    _PQ_DATA_BACKLIGHT,
    _PQ_DATA_FAC_DCR,
    _PQ_DEMO,
};

enum _PQ_FAC_DATA_ID
{
    _PQ_I_DLTI,
    _PQ_I_DCTI,
    _PQ_GAMMA,
    _PQ_DIGI_FILTER,
    _PQ_SUNR_Y,
    _PQ_SUNR_C,
    _PQ_SU_MASK,
    _PQ_DCC_MODE,
    _PQ_D_DLTI,
    _PQ_D_DCTI,
    _PQ_SU_H,
    _PQ_SU_V,
    _PQ_SU_PK,
    _PQ_VD_HUE,
    _PQ_VD_SAT,
    _PQ_VD_Y_CONTRAST,
    _PQ_VD_Y_BRIGHTNESS,
    _PQ_ICM_TABLE,
    _PQ_DITHER_BIT,
    _PQ_S_INDEX,
    _PQ_S_LOW,
    _PQ_S_HIGH,
//	_PQ_NRSPATIAL,
//	_PQ_PEAKING,
};

UINT8 x_get_color_src_id(void);
void ui_set_color_data(UINT8 id, UINT8 val);
UINT8 ui_get_color_data(UINT8 id);
#define GET_BRIGHTNESS()					ui_get_color_data(_PQ_DATA_BRIGHTNESS)
#define SET_BRIGHTNESS(_val)				ui_set_color_data(_PQ_DATA_BRIGHTNESS, _val)
#define GET_CONTRAST()						ui_get_color_data(_PQ_DATA_CONTRAST)
#define SET_CONTRAST(_val)					ui_set_color_data(_PQ_DATA_CONTRAST, _val)
#define GET_HUE()							ui_get_color_data(_PQ_DATA_HUE)
#define SET_HUE(_val)						ui_set_color_data(_PQ_DATA_HUE, _val)
#define GET_SATURATION()					ui_get_color_data(_PQ_DATA_SATURATION)
#define SET_SATURATION(_val)				ui_set_color_data(_PQ_DATA_SATURATION, _val)
#define GET_SHARPNESS()						ui_get_color_data(_PQ_DATA_SHARPNESS)
#define SET_SHARPNESS(_val)					ui_set_color_data(_PQ_DATA_SHARPNESS, _val)
#define GET_DNR()							ui_get_color_data(_PQ_DATA_DNR)
#define SET_DNR(_val)						ui_set_color_data(_PQ_DATA_DNR, _val)
#define GET_DCR()							ui_get_color_data(_PQ_DATA_DCR)
#define SET_DCR(_val)						ui_set_color_data(_PQ_DATA_DCR, _val)
#define GET_INTENSITY()						ui_get_color_data(_PQ_DATA_INTENSTIY)
#define SET_INTENSITY(_val)					ui_set_color_data(_PQ_DATA_INTENSTIY, _val)
#define GET_COLOR_TEMP_TYPE()				ui_get_color_data(_PQ_DATA_COLOR_TEMPERATURE)
#define SET_COLOR_TEMP_TYPE(_colorTemp)		ui_set_color_data(_PQ_DATA_COLOR_TEMPERATURE, _colorTemp)

#define GET_COLOR_BACKLIGHT()				ui_get_color_data(_PQ_DATA_BACKLIGHT)
#define SET_COLOR_BACKLIGHT(_val)			ui_set_color_data(_PQ_DATA_BACKLIGHT, _val)
#define SET_FAC_DCR(_val)					ui_set_color_data(_PQ_DATA_FAC_DCR, _val)
#define GET_FAC_DCR()						ui_get_color_data(_PQ_DATA_FAC_DCR)
#define SET_PQ_DEMO(_val)					ui_set_color_data(_PQ_DEMO, _val)
#define GET_PQ_DEMO()						ui_get_color_data(_PQ_DEMO)


UINT8 ui_get_fac_color_data(UINT8 id);
void ui_set_fac_color_data(UINT8 id, UINT8 val);
#define GET_I_DLTI()						ui_get_fac_color_data(_PQ_I_DLTI)
#define SET_I_DLTI(_val)					ui_set_fac_color_data(_PQ_I_DLTI, _val)
#define GET_I_DCTI()						ui_get_fac_color_data(_PQ_I_DCTI)
#define SET_I_DCTI(_val)					ui_set_fac_color_data(_PQ_I_DCTI, _val)
#define GET_GAMMA()							ui_get_fac_color_data(_PQ_GAMMA)
#define SET_GAMMA(_val)						ui_set_fac_color_data(_PQ_GAMMA, _val)
#define SET_DIGI_FILTER(_val)				ui_set_fac_color_data(_PQ_DIGI_FILTER, _val)
#define GET_DIGI_FILTER()					ui_get_fac_color_data(_PQ_DIGI_FILTER)
#define SET_SUNR_Y(_val)					ui_set_fac_color_data(_PQ_SUNR_Y, _val)
#define GET_SUNR_Y()						ui_get_fac_color_data(_PQ_SUNR_Y)
#define SET_SUNR_C(_val)					ui_set_fac_color_data(_PQ_SUNR_C, _val)
#define GET_SUNR_C()						ui_get_fac_color_data(_PQ_SUNR_C)
#define SET_SUPK_MASK(_val)					ui_set_fac_color_data(_PQ_SU_MASK, _val)
#define GET_SUPK_MASK()						ui_get_fac_color_data(_PQ_SU_MASK)
#define GET_DCC_MODE()						ui_get_fac_color_data(_PQ_DCC_MODE)
#define SET_DCC_MODE(_val)					ui_set_fac_color_data(_PQ_DCC_MODE, _val)
#define GET_DLTI()							ui_get_fac_color_data(_PQ_D_DLTI)
#define SET_DLTI(_val)						ui_set_fac_color_data(_PQ_D_DLTI, _val)
#define GET_DCTI()							ui_get_fac_color_data(_PQ_D_DCTI)
#define SET_DCTI(_val)						ui_set_fac_color_data(_PQ_D_DCTI, _val)
#define GET_SCALEUPH()						ui_get_fac_color_data(_PQ_SU_H)
#define SET_SCALEUPH(_val)					ui_set_fac_color_data(_PQ_SU_H, _val)
#define GET_SCALEUPV()						ui_get_fac_color_data(_PQ_SU_V)
#define SET_SCALEUPV(_val)					ui_set_fac_color_data(_PQ_SU_V, _val)
#define GET_SCALEPK()						ui_get_fac_color_data(_PQ_SU_PK)
#define SET_SCALEPK(_val)					ui_set_fac_color_data(_PQ_SU_PK, _val)
#define GET_VD_HUE()						ui_get_fac_color_data(_PQ_VD_HUE)
#define SET_VD_HUE(_val)					ui_set_fac_color_data(_PQ_VD_HUE, _val)
#define GET_VD_SAT()						ui_get_fac_color_data(_PQ_VD_SAT)
#define SET_VD_SAT(_val)					ui_set_fac_color_data(_PQ_VD_SAT, _val)
#define GET_VD_Y_CONTRAST()					ui_get_fac_color_data(_PQ_VD_Y_CONTRAST)
#define SET_VD_Y_CONTRAST(_val)				ui_set_fac_color_data(_PQ_VD_Y_CONTRAST, _val)
#define GET_VD_Y_BRIGHTNESS()				ui_get_fac_color_data(_PQ_VD_Y_BRIGHTNESS)
#define SET_VD_Y_BRIGHTNESS(_val)			ui_set_fac_color_data(_PQ_VD_Y_BRIGHTNESS, _val)
#define GET_ICM_TABLE()						ui_get_fac_color_data(_PQ_ICM_TABLE)
#define SET_ICM_TABLE(_val)					ui_set_fac_color_data(_PQ_ICM_TABLE, _val)
#define SET_DITHER_BIT(_val)				ui_set_fac_color_data(_PQ_DITHER_BIT, _val)
#define GET_DITHER_BIT()					ui_get_fac_color_data(_PQ_DITHER_BIT)
#define SET_S_INDEX(_val)				ui_set_fac_color_data(_PQ_S_INDEX, _val)
#define GET_S_INDEX()					ui_get_fac_color_data(_PQ_S_INDEX)
#define SET_S_LOW(_val)				ui_set_fac_color_data(_PQ_S_LOW, _val)
#define GET_S_LOW()					ui_get_fac_color_data(_PQ_S_LOW)
#define SET_S_HIGH(_val)				ui_set_fac_color_data(_PQ_S_HIGH, _val)
#define GET_S_HIGH()					ui_get_fac_color_data(_PQ_S_HIGH)
//#define GET_PEAKING()						ui_get_color_data(_PQ_DATA_PEAKING)
//#define SET_PEAKING(_val)					ui_set_color_data(_PQ_DATA_PEAKING, _val)
//#define GET_NRSPATIAL()						ui_get_color_data(_PQ_DATA_NRSPATIAL)
//#define SET_NRSPATIAL(_val)					ui_set_color_data(_PQ_DATA_NRSPATIAL, _val)



enum _color_temperature_id
{
    _COLOR_TEMP_WARMER,
    _COLOR_TEMP_WARM,
    _COLOR_TEMP_NORMAL,
    _COLOR_TEMP_COOL,
    _COLOR_TEMP_COOLER,
    _COLOR_TEMP_USER,
};

#define _R			0
#define _G			1
#define _B			2
void ui_set_color_temperature_gain(UINT8 source, UINT8 colorTemperature, UINT8 channel, UINT8 gain);
UINT8 ui_get_color_temperature_gain(UINT8 source, UINT8 colorTemperature, UINT8 channel);
void ui_set_color_temperature_offset(UINT8 source, UINT8 colorTemperature, UINT8 channel, UINT8 offset);
UINT8 ui_get_color_temperature_offset(UINT8 source, UINT8 colorTemperature, UINT8 channel);
#define GET_COLORTEMP_RGAIN(_src, _colorTemp)				ui_get_color_temperature_gain(_src, _colorTemp, _R)
#define SET_COLORTEMP_RGAIN(_src, _colorTemp, _gain)		ui_set_color_temperature_gain(_src, _colorTemp, _R, _gain)
#define GET_COLORTEMP_GGAIN(_src, _colorTemp)				ui_get_color_temperature_gain(_src, _colorTemp, _G)
#define SET_COLORTEMP_GGAIN(_src, _colorTemp, _gain)		ui_set_color_temperature_gain(_src, _colorTemp, _G, _gain)
#define GET_COLORTEMP_BGAIN(_src, _colorTemp)				ui_get_color_temperature_gain(_src, _colorTemp, _B)
#define SET_COLORTEMP_BGAIN(_src, _colorTemp, _gain)		ui_set_color_temperature_gain(_src, _colorTemp, _B, _gain)

#define GET_COLORTEMP_ROFFSET(_src, _colorTemp)				ui_get_color_temperature_offset(_src, _colorTemp, _R)
#define SET_COLORTEMP_ROFFSET(_src, _colorTemp, _offset)	ui_set_color_temperature_offset(_src, _colorTemp, _R, _offset)
#define GET_COLORTEMP_GOFFSET(_src, _colorTemp)				ui_get_color_temperature_offset(_src, _colorTemp, _G)
#define SET_COLORTEMP_GOFFSET(_src, _colorTemp, _offset)	ui_set_color_temperature_offset(_src, _colorTemp, _G, _offset)
#define GET_COLORTEMP_BOFFSET(_src, _colorTemp)				ui_get_color_temperature_offset(_src, _colorTemp, _B)
#define SET_COLORTEMP_BOFFSET(_src, _colorTemp, _offset)	ui_set_color_temperature_offset(_src, _colorTemp, _B, _offset)


#define _VOLUME_CURVE_1				0
#define _VOLUME_CURVE_25			1
#define _VOLUME_CURVE_50			2
#define _VOLUME_CURVE_75			3
#define _VOLUME_CURVE_100			4
void ui_set_volume_curve(UINT8 level, INT16 ucVal);
INT16 ui_get_volume_curve(UINT8 level);
void ui_set_TV_volume_curve(UINT8 level, INT16 ucVal);
INT16 ui_get_TV_volume_curve(UINT8 level);
#define GET_VOLUME_CURVE(_id)	ui_get_volume_curve(_id)
#define GET_VOLUME_1()			ui_get_volume_curve(_VOLUME_CURVE_1)
#define SET_VOLUME_1(_val)		ui_set_volume_curve(_VOLUME_CURVE_1, _val)
#define GET_VOLUME_25()			ui_get_volume_curve(_VOLUME_CURVE_25)
#define SET_VOLUME_25(_val)		ui_set_volume_curve(_VOLUME_CURVE_25, _val)
#define GET_VOLUME_50()			ui_get_volume_curve(_VOLUME_CURVE_50)
#define SET_VOLUME_50(_val)		ui_set_volume_curve(_VOLUME_CURVE_50, _val)
#define GET_VOLUME_75()			ui_get_volume_curve(_VOLUME_CURVE_75)
#define SET_VOLUME_75(_val)		ui_set_volume_curve(_VOLUME_CURVE_75, _val)
#define GET_VOLUME_100()		ui_get_volume_curve(_VOLUME_CURVE_100)
#define SET_VOLUME_100(_val)	ui_set_volume_curve(_VOLUME_CURVE_100, _val)
#define GET_TV_VOLUME_CURVE(_id)	ui_get_TV_volume_curve(_id)
#define GET_TV_VOLUME_1()			ui_get_TV_volume_curve(_VOLUME_CURVE_1)
#define SET_TV_VOLUME_1(_val)		ui_set_TV_volume_curve(_VOLUME_CURVE_1, _val)
#define GET_TV_VOLUME_25()			ui_get_TV_volume_curve(_VOLUME_CURVE_25)
#define SET_TV_VOLUME_25(_val)		ui_set_TV_volume_curve(_VOLUME_CURVE_25, _val)
#define GET_TV_VOLUME_50()			ui_get_TV_volume_curve(_VOLUME_CURVE_50)
#define SET_TV_VOLUME_50(_val)		ui_set_TV_volume_curve(_VOLUME_CURVE_50, _val)
#define GET_TV_VOLUME_75()			ui_get_TV_volume_curve(_VOLUME_CURVE_75)
#define SET_TV_VOLUME_75(_val)		ui_set_TV_volume_curve(_VOLUME_CURVE_75, _val)
#define GET_TV_VOLUME_100()			ui_get_TV_volume_curve(_VOLUME_CURVE_100)
#define SET_TV_VOLUME_100(_val)		ui_set_TV_volume_curve(_VOLUME_CURVE_100, _val)



enum _Customer_Factory_Data_Id
{
    _WBH_BRI,
    _WBH_CONT,
    _WBL_BRI,
    _WBL_CONT,
    _LOGO_TYPE,
    _DEFAULT_LANGUAGE,
    _BRI_0,
    _BRI_50,
    _BRI_100,
    _CONT_0,
    _CONT_50,
    _CONT_100,
    _SAT_0,
    _SAT_50,
    _SAT_100
};

void ui_set_customer_factory_data(UINT8 id, UINT8 val);
UINT8 ui_get_customer_factory_data(UINT8 id);
#define GET_WBH_BRI()			ui_get_customer_factory_data(_WBH_BRI)
#define SET_WBH_BRI(_val)		ui_set_customer_factory_data(_WBH_BRI, _val)
#define GET_WBH_CONT()			ui_get_customer_factory_data(_WBH_CONT)
#define SET_WBH_CONT(_val)		ui_set_customer_factory_data(_WBH_CONT, _val)
#define GET_WBL_BRI()			ui_get_customer_factory_data(_WBL_BRI)
#define SET_WBL_BRI(_val)		ui_set_customer_factory_data(_WBL_BRI, _val)
#define GET_WBL_CONT()			ui_get_customer_factory_data(_WBL_CONT)
#define SET_WBL_CONT(_val)		ui_set_customer_factory_data(_WBL_CONT, _val)
#define GET_LOGO_TYPE()			ui_get_customer_factory_data(_LOGO_TYPE)
#define SET_LOGO_TYPE(_val)		ui_set_customer_factory_data(_LOGO_TYPE, _val)
#define GET_DEFAULT_LANG()		ui_get_customer_factory_data(_DEFAULT_LANGUAGE)
#define SET_DEFAULT_LANG(_val)	ui_set_customer_factory_data(_DEFAULT_LANGUAGE, _val)
#define GET_BRI_0()				ui_get_customer_factory_data(_BRI_0)
#define SET_BRI_0(_val)			ui_set_customer_factory_data(_BRI_0, _val)
#define GET_BRI_50()			ui_get_customer_factory_data(_BRI_50)
#define SET_BRI_50(_val)		ui_set_customer_factory_data(_BRI_50, _val)
#define GET_BRI_100()			ui_get_customer_factory_data(_BRI_100)
#define SET_BRI_100(_val)		ui_set_customer_factory_data(_BRI_100, _val)
#define GET_CONT_0()			ui_get_customer_factory_data(_CONT_0)
#define SET_CONT_0(_val)		ui_set_customer_factory_data(_CONT_0, _val)
#define GET_CONT_50()			ui_get_customer_factory_data(_CONT_50)
#define SET_CONT_50(_val)		ui_set_customer_factory_data(_CONT_50, _val)
#define GET_CONT_100()			ui_get_customer_factory_data(_CONT_100)
#define SET_CONT_100(_val)		ui_set_customer_factory_data(_CONT_100, _val)
#define GET_SAT_0()				ui_get_customer_factory_data(_SAT_0)
#define SET_SAT_0(_val)			ui_set_customer_factory_data(_SAT_0, _val)
#define GET_SAT_50()			ui_get_customer_factory_data(_SAT_50)
#define SET_SAT_50(_val)		ui_set_customer_factory_data(_SAT_50, _val)
#define GET_SAT_100()			ui_get_customer_factory_data(_SAT_100)
#define SET_SAT_100(_val)		ui_set_customer_factory_data(_SAT_100, _val)

void ui_set_eq_300(UINT8 mode, UINT8 ucVal);
UINT8 ui_get_eq_300(UINT8 mode);
void ui_set_eq_1k(UINT8 mode, UINT8 ucVal);
UINT8 ui_get_eq_1k(UINT8 mode);
void ui_set_eq_2k(UINT8 mode, UINT8 ucVal);
UINT8 ui_get_eq_2k(UINT8 mode);
void ui_set_eq_3k(UINT8 mode, UINT8 ucVal);
UINT8 ui_get_eq_3k(UINT8 mode);
void ui_set_eq_8k(UINT8 mode, UINT8 ucVal);
UINT8 ui_get_eq_8k(UINT8 mode);
void ui_set_eq_15k(UINT8 mode, UINT8 ucVal);
UINT8 ui_get_eq_15k(UINT8 mode);

void ui_set_picture_mode(UINT8 mode);
UINT8 ui_get_picture_mode(void);
#define GET_PICTURE_MODE()		ui_get_picture_mode()
#define SET_PICTURE_MODE(_m)	ui_set_picture_mode(_m)

#define _PICTURE_MODE_BRI			0
#define _PICTURE_MODE_CONT			1
#define _PICTURE_MODE_SAT			2
#define _PICTURE_MODE_HUE			3
#define _PICTURE_MODE_SHARPNESS		4
void ui_set_picture_mode_val(UINT8 pictureMode, UINT8 channel, UINT8 val);
UINT8 ui_get_picture_mode_val(UINT8 pictureMode, UINT8 channel);
#define GET_PICTURE_MODE_BRI(_pictureMode)			ui_get_picture_mode_val(_pictureMode, _PICTURE_MODE_BRI)
#define SET_PICTURE_MODE_BRI(_pictureMode, _val)	ui_set_picture_mode_val(_pictureMode, _PICTURE_MODE_BRI, _val)
#define GET_PICTURE_MODE_CONT(_pictureMode)			ui_get_picture_mode_val(_pictureMode, _PICTURE_MODE_CONT)
#define SET_PICTURE_MODE_CONT(_pictureMode, _val)	ui_set_picture_mode_val(_pictureMode, _PICTURE_MODE_CONT, _val)
#define GET_PICTURE_MODE_SAT(_pictureMode)			ui_get_picture_mode_val(_pictureMode, _PICTURE_MODE_SAT)
#define SET_PICTURE_MODE_SAT(_pictureMode, _val)	ui_set_picture_mode_val(_pictureMode, _PICTURE_MODE_SAT, _val)
#define GET_PICTURE_MODE_HUE(_pictureMode)			ui_get_picture_mode_val(_pictureMode, _PICTURE_MODE_HUE)
#define SET_PICTURE_MODE_HUE(_pictureMode, _val)	ui_set_picture_mode_val(_pictureMode, _PICTURE_MODE_HUE, _val)
#define GET_PICTURE_MODE_SHARPNESS(_pictureMode)			ui_get_picture_mode_val(_pictureMode, _PICTURE_MODE_SHARPNESS)
#define SET_PICTURE_MODE_SHARPNESS(_pictureMode, _val)	ui_set_picture_mode_val(_pictureMode, _PICTURE_MODE_SHARPNESS, _val)



#define _CHN_STS_LOCK					(_BIT0)
#define _CHN_STS_SKIP					(_BIT1)
#define _CHN_STS_AFC_SKIP				(_BIT2)
#define _CHN_STS_CHILD_LOCK				(_BIT3)

enum _channel_ststus_bit
{
    _CHANNEL_EXIST = 0,
    _CHANNEL_SKIP,
    _CHANNEL_CHILD_LOCK,
    _CHANNEL_USER_TUNE,
};
/**
 * TV channel info macro.
 */
//#define GET_CHANNEL_STATUS(chn, m)			((ui_get_channel_data(chn, _DATA_CHN_STATUS) >> (m))&0x01)
//#define SET_CHANNEL_STATUS(chn, m)			ui_set_channel_data(chn, _DATA_CHN_STATUS, ui_get_channel_data(chn, _DATA_CHN_STATUS) | (0x01 << (m)))
//#define CLR_CHANNEL_STATUS(chn, m)			ui_set_channel_data(chn, _DATA_CHN_STATUS, ui_get_channel_data(chn, _DATA_CHN_STATUS) & ~(0x01 << (m)))
#define GET_CHN_POS(chn)					ui_get_channel_idx(chn)
#define GET_CHANNEL_CHNNO(idx)				ui_get_channel_number(idx)
#define SET_CHANNEL_CHNNO(idx, x)			ui_set_channel_number(idx, x)

extern UINT8 xdata stUISignalSource;
extern UINT8 xdata ucUartPath;
extern UINT8 xdata ucStorageTemp;
//==============================================================================
extern StructTvCableDataType code stCableData[];//Tim 0323
extern StructTvAirDataType   code stAirData[];  //Tim 0323

//==============================================================================
extern StructOsdUserDataType  code  stOsdUserDefaultData;
extern StructTvDataType  code stTVDataDefaultData;
extern StructAudioDataType code  stAudioDefaultData;
extern StructColorDataType  code stColorDefaultData[];
//extern StructFacColorDataType code stFacColorDefaultData;
extern StructFacModeDataType  code stFacModeDefaultData;
extern StructScreenModeInfo  code stScreenModeInfoDefaultData;
extern StructSystemDataType  code stSystemDataDefault;
extern StructUserDataChannelSettingsType  code tDEFAULT_OSDUSER_CHANNELSETTING;
//extern StructChannelOffsetType code tChannelOffsetDefault;
extern StructCustomerFacDataType code stCustomerFacDefaultData;
extern ADCGainOffset code stYPbPrGainDefaultData[];
extern ADCGainOffset1 code  stVGAGainDefaultData;
extern StructVGAScreenModeInfo code StructVGAScreenModeInfoDefaultData[];
extern StructTVChannelMapTable code  stTVChannelMapDefault;
extern ADCGainOffset2 code stSCARTGainDefaultData[];

#ifdef _VIDEO_TV_SUPPORT
UINT8 ui_get_channel_idx(UINT8 ucChn);
void ui_set_channel_idx(UINT8 ucChn, UINT8 ucIdx);
UINT8 ui_get_channel_number(UINT8 ucIdx);
void ui_set_channel_number(UINT8 ucIdx, UINT8 ucNumber);

#if defined(_REALTEK_OSD_USE)
void ui_set_chn_data(UINT8 ucIdx, StructUserDataChannelSettingsType* pChannelBuf);
#endif
void ui_get_chn_data(UINT8 ucIdx, StructUserDataChannelSettingsType* pChannelBuf);


void ui_get_Air_chn_data(UINT8 ucIdx, StructTvAirDataType* pChannelBuf);
void ui_get_Cable_chn_data(UINT8 ucIdx, StructTvCableDataType* pChannelBuf);

#endif
#define _BLUE_SCREEN_TV		0x12
#define _CUR_CHN_TV			0x13
#define _PRE_CHN_TV			0x14
#define _TOTAL_CHN_TV       0x15
#define _COUNTRY_SEL_TV     0x16

/**
 * TV channel info macro.
 */
enum _TV_data_id
{
    _TV_BLUE_SCREEN,
    _TV_CUR_CHN,
    _TV_PRE_CHN,
    _TV_TOTAL_CHN,
    _TV_COUNTRY_SEL,


	_TV_BLUE_SCREEN_CABLE,
	_TV_CUR_CHN_CABLE,//6
	_TV_PRE_CHN_CABLE,
	_TV_TOTAL_CHN_CABLE,
	_TV_COUNTRY_SEL_CABLE,

    
	_TV_BLUE_SCREEN_AIR,
	_TV_CUR_CHN_AIR,//b 11
	_TV_PRE_CHN_AIR,
	_TV_TOTAL_CHN_AIR,
	_TV_COUNTRY_SEL_AIR,

	_TV_NTSC_SYSTEM,
	_TV_NTSC_FREE,//0x10 16
    _TV_ID_COUNT,
};
#ifdef _VIDEO_TV_SUPPORT
UINT8 ui_get_tv_data(UINT8 id);
void ui_set_tv_data(UINT8 id, UINT8 ucVal);
//Tim 0323
//==============================================================================
#define GET_TV_CUR_NtscType()			ui_get_tv_data(_TV_NTSC_SYSTEM)
#define SET_TV_CUR_NtscType(_val)		ui_set_tv_data(_TV_NTSC_SYSTEM, _val)
#define GET_TV_CUR_FreeType()			ui_get_tv_data(_TV_NTSC_FREE)
#define SET_TV_CUR_FreeType(_val)		ui_set_tv_data(_TV_NTSC_FREE, _val)

//Tim 0330
#define GET_TV_CUR_CABLE_CHN()			ui_get_tv_data(_TV_CUR_CHN_CABLE)
#define SET_TV_CUR_CABLE_CHN(_val)		ui_set_tv_data(_TV_CUR_CHN_CABLE, _val)
#define GET_TV_CUR_AIR_CHN()			ui_get_tv_data(_TV_CUR_CHN_AIR)
#define SET_TV_CUR_AIR_CHN(_val)		ui_set_tv_data(_TV_CUR_CHN_AIR, _val)

#define GET_TV_PRE_CABLE_CHN()			ui_get_tv_data(_TV_PRE_CHN_CABLE)
#define SET_TV_PRE_CABLE_CHN(_val)		ui_set_tv_data(_TV_PRE_CHN_CABLE, _val)
#define GET_TV_PRE_AIR_CHN()			ui_get_tv_data(_TV_PRE_CHN_AIR)
#define SET_TV_PRE_AIR_CHN(_val)		ui_set_tv_data(_TV_PRE_CHN_AIR, _val)

/*UINT8 ui_get_cable_chn_color_system(UINT8 ucIdx);
void ui_set_cable_chn_color_system(UINT8 ucIdx, UINT8 colorStd);
#define GET_CABLE_CHANNEL_COLORSYSTEM(_chn)			ui_get_cable_chn_color_system(ui_get_channel_idx(_chn))
#define SET_CABLE_CHANNEL_COLORSYSTEM(_chn, _x)		ui_set_cable_chn_color_system(ui_get_channel_idx(_chn), _x)

UINT8 ui_get_air_chn_color_system(UINT8 ucIdx);
void ui_set_air_chn_color_system(UINT8 ucIdx, UINT8 colorStd);
#define GET_AIR_CHANNEL_COLORSYSTEM(_chn)			ui_get_air_chn_color_system(ui_get_channel_idx(_chn))
#define SET_AIR_CHANNEL_COLORSYSTEM(_chn, _x)		ui_set_air_chn_color_system(ui_get_channel_idx(_chn), _x)
*/

//==============================================================================
#define GET_TV_CUR_CHN()				ui_get_tv_data(_TV_CUR_CHN)
#define SET_TV_CUR_CHN(_val)			ui_set_tv_data(_TV_CUR_CHN, _val)
#define GET_TV_PRE_CHN()				ui_get_tv_data(_TV_PRE_CHN)
#define SET_TV_PRE_CHN(_val)			ui_set_tv_data(_TV_PRE_CHN, _val)
#define GET_TV_TOTAL_CHN()				ui_get_tv_data(_TV_TOTAL_CHN)
#define SET_TV_TOTAL_CHN(_val)			ui_set_tv_data(_TV_TOTAL_CHN, _val)
#define GET_TV_BLUE_SCREEN()			ui_get_tv_data(_TV_BLUE_SCREEN)
#define SET_TV_BLUE_SCREEN()			ui_set_tv_data(_TV_BLUE_SCREEN, _ON)	//{ucStorageTemp = _ON;ui_set_tv_data(_TV_BLUE_SCREEN, ucStorageTemp);}
#define CLR_TV_BLUE_SCREEN()			ui_set_tv_data(_TV_BLUE_SCREEN, _OFF)	//{ucStorageTemp = _OFF;ui_set_tv_data(_TV_BLUE_SCREEN, ucStorageTemp);}
#define GET_TV_COUNTRY()				ui_get_tv_data(_TV_COUNTRY_SEL)
#define SET_TV_COUNTRY(_val)			ui_set_tv_data(_TV_COUNTRY_SEL, _val)

//==============================================================================
#define GET_TV_CHN_DATA(DataId)			ui_get_tv_data(DataId)
#define SET_TV_CHN_DATA(DataId,_val)    ui_set_tv_data(DataId, _val)
//==============================================================================

UINT8 ui_get_chn_color_system(UINT8 ucIdx);
void ui_set_chn_color_system(UINT8 ucIdx, UINT8 colorStd);
#define GET_CHANNEL_COLORSYSTEM(_chn)		ui_get_chn_color_system(ui_get_channel_idx(_chn))
#define SET_CHANNEL_COLORSYSTEM(_chn, _x)		ui_set_chn_color_system(ui_get_channel_idx(_chn), _x)

UINT8 ui_get_chn_sound_system(UINT8 ucIdx);
void ui_set_chn_sound_system(UINT8 ucIdx, UINT8 soundStd);
#define GET_CHANNEL_SOUNDSYSTEM(_chn)		ui_get_chn_sound_system(ui_get_channel_idx(_chn))
#define SET_CHANNEL_SOUNDSYSTEM(_chn, _x)	ui_set_chn_sound_system(ui_get_channel_idx(_chn), _x)

UINT32 ui_get_chn_freq(UINT8 ucIdx);
//void ui_set_chn_freq(UINT8 ucIdx, UINT32 uwFreq);
#define GET_CHANNEL_FREQ(_chn)				ui_get_chn_freq(ui_get_channel_idx(_chn))
//Tim 0324
//==============================================================================
//#define SET_CHANNEL_FREQ(_chn, _x)			ui_set_chn_freq(ui_get_channel_idx(_chn), _x)
//==============================================================================
#if defined(_REALTEK_OSD_USE)
void ui_set_chn_name(UINT8 ucIdx, UINT8 *pBuf);
#define SET_CHANNEL_NAME(_chn, _buf)		ui_set_chn_name(ui_get_channel_idx(_chn), _buf)
void ui_get_chn_name(UINT8 ucIdx, UINT8 *pBuf);
#define GET_CHANNEL_NAME(_chn, _buf)		ui_get_chn_name(ui_get_channel_idx(_chn), _buf)
#endif

UINT8 ui_get_chn_freq_offset(UINT8 ucIdx);
void ui_set_chn_freq_offset(UINT8 ucIdx, UINT8 offset);
#define GET_CHANNEL_FREQ_OFFSET(_chn)  		ui_get_chn_freq_offset(ui_get_channel_idx(_chn))
#define SET_CHANNEL_FREQ_OFFSET(_chn, _x)	ui_set_chn_freq_offset(ui_get_channel_idx(_chn), _x)



UINT8 ui_get_chn_status(UINT8 ucIdx);
void ui_set_chn_status(UINT8 ucIdx, UINT8 status);
#define GET_CHANNEL_LOCK(_chn)			(ui_get_chn_status(ui_get_channel_idx(_chn))& _CHN_STS_LOCK)
#define SET_CHANNEL_LOCK(_chn)			ui_set_chn_status(ui_get_channel_idx(_chn), ui_get_chn_status(ui_get_channel_idx(_chn)) | _CHN_STS_LOCK)
#define CLR_CHANNEL_LOCK(_chn)			ui_set_chn_status(ui_get_channel_idx(_chn), ui_get_chn_status(ui_get_channel_idx(_chn)) & ~_CHN_STS_LOCK)

#define GET_CHANNEL_SKIP(_chn)			(ui_get_chn_status(ui_get_channel_idx(_chn)) & _CHN_STS_SKIP)
#define SET_CHANNEL_SKIP(_chn)			ui_set_chn_status(ui_get_channel_idx(_chn), ui_get_chn_status(ui_get_channel_idx(_chn)) | _CHN_STS_SKIP)
#define CLR_CHANNEL_SKIP(_chn)			ui_set_chn_status(ui_get_channel_idx(_chn), ui_get_chn_status(ui_get_channel_idx(_chn)) & ~_CHN_STS_SKIP)

#define GET_AFC_SKIP(_chn)				(ui_get_chn_status(ui_get_channel_idx(_chn)) & _CHN_STS_AFC_SKIP)
#define SET_AFC_SKIP(_chn)				ui_set_chn_status(ui_get_channel_idx(_chn), ui_get_chn_status(ui_get_channel_idx(_chn)) | _CHN_STS_AFC_SKIP)
#define CLR_AFC_SKIP(_chn)				ui_set_chn_status(ui_get_channel_idx(_chn), ui_get_chn_status(ui_get_channel_idx(_chn)) & ~_CHN_STS_AFC_SKIP)
#endif

UINT8 ui_get_osd_language(void);
void ui_set_osd_language(UINT8 ucLang);
#define GET_OSD_LANGUAGE()				ui_get_osd_language()
#define SET_OSD_LANGUAGE(_lang)			ui_set_osd_language(_lang)

UINT8 ui_get_osd_timeout(void);
void ui_set_osd_timeout(UINT8 ucTimeout);
#define GET_OSD_TIMEOUT()				ui_get_osd_timeout()
#define SET_OSD_TIMEOUT(_x)				ui_set_osd_timeout(_x)

UINT8 ui_get_osd_blend(void);
void ui_set_osd_blend(UINT8 ucBlend);
#define GET_OSD_BLENDING()				ui_get_osd_blend()
#define SET_OSD_BLENDING(_x)			ui_set_osd_blend(_x)

UINT8 ui_get_aging_mode(void);
void ui_set_aging_mode(UINT8 ucMode);
#define GET_AGING_MODE()				ui_get_aging_mode()
#define SET_AGING_MODE()				ui_set_aging_mode(_AGING_MODE_ON);
#define CLR_AGING_MODE()				ui_set_aging_mode(_AGING_MODE_OFF);

UINT8 ui_get_input_source(void);
void ui_set_input_source(UINT8 src);
void ui_load_default(void);
void ui_get_VGA_ADCGain(ADCGainOffset1* ptADCGainOffset);
void ui_set_VGA_ADCGain(ADCGainOffset1* ptADCGainOffset);
void ui_get_YPbPr_ADCGain(ADCGainOffset* ptADCGainOffset);
void ui_set_YPbPr_ADCGain(ADCGainOffset* ptADCGainOffset);
void ui_get_SCART_ADCGain(ADCGainOffset2* ptADCGainOffset);
void ui_set_SCART_ADCGain(ADCGainOffset2* ptADCGainOffset);

void ui_get_VGA_info(UINT8 ucIdx);
void ui_set_VGA_info(UINT8 ucIdx);



UINT8 ui_get_source_idx(void);
void ui_set_source_idx(UINT8 srcIdx);
#define GET_SOURCE_IDX()				ui_get_source_idx()
#define SET_SOURCE_IDX(_x)				ui_set_source_idx(_x)

//Hardy20091203 for booting to AC-off power status before
void ui_set_AcOff_powerStatus(UINT8 mode);
BOOL ui_is_AcOffPowerStatus_On();
//end Hardy20091203
#endif //#ifndef _UI_STRUCT_H


