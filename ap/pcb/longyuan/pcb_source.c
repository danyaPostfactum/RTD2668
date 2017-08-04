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
* @{
*/

#include "source.h"
#include "pcb\pcb_utility.h"
#include "pcb_customer.h"
#include "autoconf.h"

//Switch ADC Data input path (input0 or input1)
VGA_ADC_DATA_PATH code tVGAPath = {ADC_DATA_INPUT1};					//VGA
YPBPR_ADC_DATA_PATH code tYPbPrPath = {ADC_DATA_INPUT0};				//YPbPr

// Set some options for Video, include video path
AV_VDC_PATH code tAvPath	= {VIDEO_INPUT12,VIDEO_GND1, VIDEO_AVOUT_NONE};
AV_VDC_PATH code tAv1Path	= {VIDEO_INPUT02,VIDEO_GND0, VIDEO_AVOUT_NONE};
SV_VDC_PATH code tSvPath	= {VIDEO_INPUT11,VIDEO_GND1,VIDEO_INPUT01,VIDEO_GND0, VIDEO_AVOUT_NONE};
TV_VDC_PATH code tTvPath	= {TV_INPUT,TV_INPUT, VIDEO_AVOUT_NONE};

SCART_VDC_PATH code tFullScart	= {VIDEO_INPUT03,VIDEO_GND0,VIDEO_RGB0,VIDEO_GND0, VIDEO_AVOUT1};
SCART_VDC_PATH code tHalfScart	= {VIDEO_INPUT02,VIDEO_GND0,VIDEO_INPUT12,VIDEO_GND1, VIDEO_AVOUT2};

// Set some options for HDMI


/*==================== object of HDMI ================== */

/*==================== object of Video Decoder ================== */

/*==================== object of Tuner ================== */

/*==================== object of Sync Processor ================== */

/*==================== object of Audio ================== */
AIO_PATH_CFG code audio_path_cfg_bbadc =
{
    AIO_PATH_ID_PB,
    {AIO_PATH_SRC_BBADC, AIO_PATH_SRC_UNKNOWN, AIO_PATH_SRC_UNKNOWN, AIO_PATH_SRC_UNKNOWN},
};

AIO_PATH_CFG code audio_path_cfg_spdif =
{
    AIO_PATH_ID_PB,
    {AIO_PATH_SRC_SPDIF, AIO_PATH_SRC_UNKNOWN, AIO_PATH_SRC_UNKNOWN, AIO_PATH_SRC_UNKNOWN},
};

AIO_PATH_CFG code audio_path_cfg_i2s =
{
    AIO_PATH_ID_PB,
    {AIO_PATH_SRC_I2S_CH12, AIO_PATH_SRC_UNKNOWN, AIO_PATH_SRC_UNKNOWN, AIO_PATH_SRC_UNKNOWN},
};

AIO_PATH_CFG code audio_path_cfg_atv =
{
    AIO_PATH_ID_PB,
    {AIO_PATH_SRC_ATV, AIO_PATH_SRC_UNKNOWN, AIO_PATH_SRC_UNKNOWN, AIO_PATH_SRC_UNKNOWN},
};

// The defintition for src_info and link between video and audio
STRUCT_AUDIO_INPUT_BBADC code audio_cfg_path_vga =
{
    AIO_BBADC_AIN2,
    &audio_path_cfg_bbadc
};

STRUCT_AUDIO_INPUT code audio_path_vga =
{
    AUDIO_TYPE_BBADC,
    (void*)&audio_cfg_path_vga
};

STRUCT_AUDIO_INPUT_BBADC code audio_cfg_path_ypbpr =
{
    AIO_BBADC_AIN5_2,
    &audio_path_cfg_bbadc
};

STRUCT_AUDIO_INPUT code audio_path_ypbpr =
{
    AUDIO_TYPE_BBADC,
    (void*)&audio_cfg_path_ypbpr
};

STRUCT_AUDIO_INPUT_BBADC code audio_cfg_path_av_sv =
{
    AIO_BBADC_AIN1,
    &audio_path_cfg_bbadc
};

STRUCT_AUDIO_INPUT_BBADC code audio_cfg_path_av3_sv =
{
    AIO_BBADC_AIN2,
    &audio_path_cfg_bbadc
};
STRUCT_AUDIO_INPUT_BBADC code audio_cfg_path_av2_sv =
{
    AIO_BBADC_AIN5_2,
    &audio_path_cfg_bbadc
};
STRUCT_AUDIO_INPUT_BBADC code audio_cfg_path_av4_sv =
{
    AIO_BBADC_AIN3,
    &audio_path_cfg_bbadc
};


STRUCT_AUDIO_INPUT_BBADC code audio_cfg_path_full_scart =
{
    AIO_BBADC_AIO1,
    &audio_path_cfg_bbadc
};

STRUCT_AUDIO_INPUT_BBADC code audio_cfg_path_half_scart =
{
    AIO_BBADC_AIN1,
    &audio_path_cfg_bbadc
};

STRUCT_AUDIO_INPUT code audio_path_av_sv =
{
    AUDIO_TYPE_BBADC,
    (void*)&audio_cfg_path_av_sv
};

STRUCT_AUDIO_INPUT code audio_path_av3_sv =
{
    AUDIO_TYPE_BBADC,
    (void*)&audio_cfg_path_av3_sv
};
STRUCT_AUDIO_INPUT code audio_path_av2_sv =
{
    AUDIO_TYPE_BBADC,
    (void*)&audio_cfg_path_av2_sv
};

STRUCT_AUDIO_INPUT code audio_path_av4_sv =
{
    AUDIO_TYPE_BBADC,
    (void*)&audio_cfg_path_av4_sv
};

STRUCT_AUDIO_INPUT code audio_path_atv =
{
    AUDIO_TYPE_TV,
    (void*)&audio_path_cfg_atv
};

STRUCT_AUDIO_INPUT code audio_path_full_scart =
{
    AUDIO_TYPE_BBADC,
    (void*)&audio_cfg_path_full_scart
};

STRUCT_AUDIO_INPUT code audio_path_half_scart =
{
    AUDIO_TYPE_BBADC,
    (void*)&audio_cfg_path_half_scart
};

STRUCT_AUDIO_INPUT_SPDIF code audio_cfg_path_hdmi =
{
    AIO_SPDIFI_HDMI,
    (void*)&audio_path_cfg_spdif
};

STRUCT_AUDIO_INPUT code audio_path_hdmi =
{
    AUDIO_TYPE_SPDIF,
    (void*)&audio_cfg_path_hdmi
};

UINT8 code audio_full_scart_sel = 1;	// [1,2] = [AIO1,AIO2]
UINT8 code audio_half_scart_sel = 2;	// [1,2] = [AIO1,AIO2]
/*==================== All input source ================== */
/// This structure must corresponds input source index
StructSourceInput code src_info[] =
{
#if 0
    { SET_SOURCE(_SOURCE_VGA,0), (void *)&tVGAPath, &audio_path_vga},
    #ifdef CONFIG_ENABLE_HDMI
    { SET_SOURCE(_SOURCE_HDMI,0), (void *)NULL, &audio_path_hdmi},
    #endif
    #ifdef _VIDEO_TV_SUPPORT
    { SET_SOURCE(_SOURCE_TV,0), (void *)&tTvPath, &audio_path_atv},
    #endif
    //{ SET_SOURCE(_SOURCE_CVBS,0), (void *)&tAvPath, &audio_path_av1_sv},
    { SET_SOURCE(_SOURCE_CVBS,1), (void *)&tAv1Path, &audio_path_av_sv},
    { SET_SOURCE(_SOURCE_SV,0), (void *)&tSvPath, &audio_path_av_sv},
    { SET_SOURCE(_SOURCE_YPBPR,0), (void *)&tYPbPrPath, &audio_path_av_sv},
#ifdef CONFIG_SCART_AUTO_SWITCH
    { SET_SOURCE(_SOURCE_FULL_SCART,0), (void *)&tFullScart, &audio_path_full_scart},
    { SET_SOURCE(_SOURCE_HALF_SCART,0), (void *)&tHalfScart, &audio_path_half_scart},
#endif
#else//Tim 0325
	#ifdef _VIDEO_TV_SUPPORT
    { SET_SOURCE(_SOURCE_TV,0), (void *)&tTvPath, &audio_path_atv},
	#endif
    { SET_SOURCE(_SOURCE_CVBS,0), (void *)&tAvPath, &audio_path_av_sv},
    { SET_SOURCE(_SOURCE_CVBS,1), (void *)&tAv1Path, &audio_path_av2_sv},
    { SET_SOURCE(_SOURCE_CVBS,2), (void *)&tAvPath, &audio_path_av3_sv},
    { SET_SOURCE(_SOURCE_CVBS,3), (void *)&tAvPath, &audio_path_av4_sv},
    { SET_SOURCE(_SOURCE_SV,0), (void *)&tSvPath, &audio_path_av_sv},
    { SET_SOURCE(_SOURCE_YPBPR,0), (void *)&tYPbPrPath, &audio_path_ypbpr},
    #if CONFIG_ENABLE_USB
	{ SET_SOURCE(_SOURCE_YPBPR,0), (void *)&tYPbPrPath, &audio_path_ypbpr},
	#endif


    { SET_SOURCE(_SOURCE_VGA,0), (void *)&tVGAPath, &audio_path_vga},
    #ifdef CONFIG_ENABLE_HDMI
    { SET_SOURCE(_SOURCE_HDMI,0), (void *)NULL, &audio_path_hdmi},
    #endif
#ifdef CONFIG_SCART_AUTO_SWITCH
    { SET_SOURCE(_SOURCE_FULL_SCART,0), (void *)&tFullScart, &audio_path_full_scart},
    { SET_SOURCE(_SOURCE_HALF_SCART,0), (void *)&tHalfScart, &audio_path_half_scart},
#endif

#endif
};

StructSourceInputSet code src_infoSet =
{
    sizeof(src_info) / sizeof(StructSourceInput),
    src_info
};

/* Select input source mux in Video */
void pcb_set_Video_src_mux(UINT8 src)
{
    switch (src)
    {
    case _SOURCE_YPBPR:
        YPBPR_SOURCE_SELECT(ADC0_MUX_YPBPR);
        break;
#ifdef CONFIG_SCART_AUTO_SWITCH
    case _SOURCE_FULL_SCART:
        YPBPR_SOURCE_SELECT(ADC0_MUX_SCART_RGB);		// ±NADC0
        break;
#endif
    default:
        break;
    };
}

/* Select input source mux in Audio */
void pcb_set_Audio_src_mux(UINT8 src)
{
    switch (src)
    {
#ifdef CONFIG_SCART_AUTO_SWITCH
    case _SOURCE_FULL_SCART:
        if (audio_full_scart_sel == 1)
        {
            Audio_AioSetOptSrc_AIO1(AIO_WIREOUT_TS_DAC);
        }
        else
        {
            Audio_AioSetOptSrc_AIO2(AIO_WIREOUT_TS_DAC);
        }
        break;

    case _SOURCE_HALF_SCART:
        if (audio_half_scart_sel == 1)
        {
            Audio_AioSetOptSrc_AIO1(AIO_WIREOUT_TS_DAC);
        }
        else
        {
            Audio_AioSetOptSrc_AIO2(AIO_WIREOUT_TS_DAC);
        }
#endif //CONFIG_SCART_AUTO_SWITCH
    default:
        break;
    };
}

/**
 *
 * @}
 */
