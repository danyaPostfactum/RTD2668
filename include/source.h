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

/*==================== Definitions ================= */


#ifndef  _SOURCE_H
#define  _SOURCE_H

#include "sparrow.h"
#include "audio\audio_api.h"
#include "audio\audio_if.h"
#include "ui_struct.h"
#include "autoconf.h"

/*==================== Definitions ================ */
//--------------------------------------------------
//  input source index(Total Num of Input Source)
//--------------------------------------------------
//--------------------------------------------------
// where the source from in VGIP
//--------------------------------------------------
#define _SRC_FROM_ADC		0x01
#define _SRC_FROM_VDC		0x02
#define _SRC_FROM_TMDS		0x03
#define _SRC_FROM_VIDEO8	0x04
#define _SRC_FROM_VIDMA	0x05
#define _SRC_FROM_SCART	0x06

//--------------------------------------------------
// source
//--------------------------------------------------
// For ADC input
#define _SOURCE_YPBPR		((_SRC_FROM_ADC & 0x0f) << 4 | 0x01)
#define _SOURCE_VGA		((_SRC_FROM_ADC & 0x0f) << 4 | 0x02)
#define _SOURCE_PVR			((_SRC_FROM_ADC & 0x0f) << 4 | 0x08)

// For VDC input
#define _SOURCE_TV   		((_SRC_FROM_VDC & 0x0f) << 4 | 0x01)
#define _SOURCE_CVBS 		((_SRC_FROM_VDC & 0x0f) << 4 | 0x02)
#define _SOURCE_SV   		((_SRC_FROM_VDC & 0x0f) << 4 | 0x03)
// For SCART
#ifdef CONFIG_SCART_AUTO_SWITCH
#define _SOURCE_FULL_SCART		((_SRC_FROM_VDC & 0x0f) << 4 | 0x04)
#define _SOURCE_HALF_SCART	((_SRC_FROM_VDC & 0x0f) << 4 | 0x05)
//#define _SOURCE_SCART_AV 	((_SRC_FROM_VDC & 0x0f) << 4 | 0x06)
//#define _SOURCE_SCART_SV 	((_SRC_FROM_VDC & 0x0f) << 4 | 0x07)
#endif

//#ifdef CONFIG_ENABLE_HDMI
// For HDMI
#define _SOURCE_HDMI  		((_SRC_FROM_TMDS & 0x0f) << 4 | 0x01)
#define _SOURCE_DVI			((_SRC_FROM_TMDS & 0x0f) << 4 | 0x02)
//#endif

#define _SOURCE_SCART		((_SRC_FROM_SCART & 0x0f) << 4 | 0x01)

/*====================== Types ================== */
#define FLOW_GET_SRC_INFO_PARAMETER(index, ELEMENT)	\
		flow_Get_SrcInput_Parameter(index, OFFSET_STRUCT(StructSourceInput, ELEMENT), SIZEOF_STRUCT(StructSourceInput, ELEMENT))

#define GET_SRC_TYPE(idx)	(FLOW_GET_SRC_INFO_PARAMETER(idx, src_type))

#define GET_Init_AVOptions(idx, Data)		(flow_Get_SrcInput_InitOptions(idx, Data, sizeof(AV_VDC_PATH)))
#define GET_Init_SVOptions(idx, Data)		(flow_Get_SrcInput_InitOptions(idx, Data, sizeof(SV_VDC_PATH)))
#define GET_Init_TVOptions(idx, Data)		(flow_Get_SrcInput_InitOptions(idx, Data, sizeof(TV_VDC_PATH)))
#define GET_Init_YPbPrOptions(idx, Data)	(flow_Get_SrcInput_InitOptions(idx, Data, sizeof(YPBPR_ADC_DATA_PATH)))
#define GET_Init_VGAOptions(idx, Data)		(flow_Get_SrcInput_InitOptions(idx, Data, sizeof(VGA_ADC_DATA_PATH)))
#define GET_Init_SCARTOptions(idx, Data)	(flow_Get_SrcInput_InitOptions(idx, Data, sizeof(SCART_VDC_PATH)))


#define SET_SOURCE(src, index)				(((src & 0xff) << 8 | (index & 0x0f)))
#define GET_NUMBER_OF_SOURCE()			(src_infoSet.nSrcNum)

#define GET_INPUTSOURCE_INDEX()					(info->input_src)

#define GET_SOURCE_FROM(src_type)			(((src_type & 0xf000) >> 12))
#define GET_SOURCE_TYPE(src_type)			((((UINT32)src_type & 0xf700) >> 8))
#define GET_SOURCE_INDEX(src_type)			((src_type & 0x000f))

#define GET_INPUTSOURCE_TYPE()			((UINT32)GET_SRC_TYPE(GET_INPUTSOURCE_INDEX()))>>8

#define GET_INPUTSOURCE_FROM()			((UINT32)GET_SRC_TYPE(GET_INPUTSOURCE_INDEX()))>>12//GET_SOURCE_FROM(src_info[info->input_src].src_type)

#define GET_INPUT_MODE()					(info->ucMode_Curr)

////////////////////////////////////////////////////////////////////////////////
//ADC SIGNAL POSITIVE PATH
typedef enum
{
    _SCART_AV = 0,
    _SCART_SV,
    _SCART_RGB,
} SCACT_DATA_TYPE;
extern UINT8 SCART_Type;	// 0: is scart av, 2: is scart sv, 1: is scart rgb

typedef enum
{
    ADC_DATA_INPUT0 = 0,
    ADC_DATA_INPUT1,
    ADC_DATA_INPUT2,
} ADC_DATA_PATH;

//ADC SOY SIGNAL ATH
typedef enum
{
    ADC_SOY_INPUT0 = 0,
    ADC_SOY_INPUT1,
} ADC_SOY_INPUT_PATH;

//ADC SOY SIGNAL ATH
typedef enum
{
    ADC_SOY_R_INPUT = 0,
    ADC_SOY_G_INPUT,
    ADC_SOY_B_INPUT,
} ADC_SOY_CONNECT_TO_PATH;

typedef struct
{
    ADC_DATA_PATH DATA_INPUT;
//	ADC_SOY_INPUT_PATH USE_SOY;
//	ADC_SOY_CONNECT_TO_PATH SOY_PATH;
} VGA_ADC_DATA_PATH;

typedef struct
{
    ADC_DATA_PATH DATA_INPUT;
//	ADC_SOY_INPUT_PATH USE_SOY_INPUT;
//	ADC_SOY_CONNECT_TO_PATH SOY_PATH;
}  YPBPR_ADC_DATA_PATH;

typedef struct
{
    void*		path;
} ADC_INITIAL_OPTIONS;

//VDC SIGNAL POSITIVE PATH
typedef enum
{
    VIDEO_INPUT00 = 0,
    VIDEO_INPUT01,
    VIDEO_INPUT02,
    VIDEO_INPUT03,
    VIDEO_INPUT10,
    VIDEO_INPUT11,
    VIDEO_INPUT12,
    VIDEO_RGB0,
    VIDEO_RGB1,
    TV_INPUT,
    VIDEO_INPUT_R0,
    VIDEO_INPUT_G0,
    VIDEO_INPUT_B0,
    VIDEO_INPUT_R1,
    VIDEO_INPUT_G1,
    VIDEO_INPUT_B1,
} VIDEO_PATH;

//VDC SIGNAL OUTPUT PATH
typedef enum
{
    VIDEO_AVOUT_NONE = 0,
    VIDEO_AVOUT1 = 1,
    VIDEO_AVOUT2 = 2,
} VIDEO_AVOUT_PATH;

typedef enum
{
    VIDEO_GND0 = 0,
    VIDEO_GND1,
} VIDEO_GND_PATH;

typedef struct
{
    VIDEO_PATH VideoIn;
    VIDEO_GND_PATH GroundIn;
    VIDEO_AVOUT_PATH AVOutPath;
}  AV_VDC_PATH;

typedef struct
{
    VIDEO_PATH Y_VideoIn;
    VIDEO_GND_PATH Y_GroundIn;
    VIDEO_PATH C_VideoIn;
    VIDEO_GND_PATH C_GroundIn;
    VIDEO_AVOUT_PATH AVOutPath;
}  SV_VDC_PATH;

typedef struct
{
    VIDEO_PATH Y_VideoIn;
    VIDEO_GND_PATH Y_GroundIn;
    VIDEO_PATH RGB_VideoIn;
    VIDEO_GND_PATH RGB_GroundIn;
    VIDEO_AVOUT_PATH AVOutPath;
}  SCART_VDC_PATH;

typedef AV_VDC_PATH TV_VDC_PATH;

typedef struct
{
    void*		path;
} VDC_INITIAL_OPTIONS;

// For aduio
//
typedef enum _AUDIO_INPUT_TYPE
{
    AUDIO_TYPE_BBADC = 0,
    AUDIO_TYPE_TV,
    AUDIO_TYPE_I2S,
    AUDIO_TYPE_SPDIF,
    AUDIO_TYPE_UNKNOWN
} AUDIO_INPUT_TYPE;

typedef struct _STRUCT_AUDIO_INPUT_BBADC
{
    UINT16	ain;
    AIO_PATH_CFG  * aio_path_cfg;
} STRUCT_AUDIO_INPUT_BBADC;

typedef struct _STRUCT_AUDIO_INPUT_TV
{
    AIO_PATH_CFG  * aio_path_cfg;
} STRUCT_AUDIO_INPUT_TV;

typedef struct _STRUCT_AUDIO_INPUT_I2S
{
    UINT16	ain;
    AIO_PATH_CFG  * aio_path_cfg;
} STRUCT_AUDIO_INPUT_I2S;

typedef struct _STRUCT_AUDIO_INPUT_SPDIF
{
    UINT16	ain;
    AIO_PATH_CFG  * aio_path_cfg;
} STRUCT_AUDIO_INPUT_SPDIF;

typedef struct _STRUCT_AUDIO_INPUT
{
    AUDIO_INPUT_TYPE	src_type;
    void*				cfgOptions;	// Audio path & options
} STRUCT_AUDIO_INPUT;

typedef struct
{
    UINT16					src_type;		// Bit0~Bit7: RTD2622 input from, Bit8~Bit15: VDC input type
    void*					InitOptions;		// different initial setting according to individual source(just for video decoder)
    STRUCT_AUDIO_INPUT*	AudioOptions;	// Audio path & options
} StructSourceInput;

typedef struct
{
    UINT8				nSrcNum;			// the number of input source
    StructSourceInput*		SourceInputSet;		// point to SourceInput array
} StructSourceInputSet;

extern StructSourceInput code src_info[];
extern StructSourceInputSet code src_infoSet;

void flow_source_Init(void);
UINT8 flow_source_DetectMode(void);
bit flow_source_CheckMode(void);
void flow_source_Inproc_Setup(void);
void flow_source_Release(void);
UINT8 flow_UsePowerSaving(void);
/* Select input source mux in Video */
void pcb_set_Video_src_mux(UINT8 src);

/* Select input source mux in Audio */
void pcb_set_Audio_src_mux(UINT8 src);
#if CONFIG_ENABLE_USB
void Send_IR(UINT16 usCode);
#endif
#if CONFIG_TU300
void Send_IR_DTV(UINT16 usCode);
#endif

UINT16 flow_Get_SrcInput_Parameter(UINT8 idx, UINT16 offset, UINT8 len);
bit flow_Get_SrcInput_InitOptions(UINT8 idx, UINT8* ptData, UINT8 len);


#define FLOW_GET_SRC_INFO_PARAMETER(index, ELEMENT)	\
		flow_Get_SrcInput_Parameter(index, OFFSET_STRUCT(StructSourceInput, ELEMENT), SIZEOF_STRUCT(StructSourceInput, ELEMENT))

extern StructSourceInput  *ptSrcInput;

#endif /* _SOURCE_H */

/**
 *
 * @}
 */
