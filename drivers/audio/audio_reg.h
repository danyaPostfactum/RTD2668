/*=============================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2009 *
 * All rights reserved.                                       *
 *============================================================*/
/*======================= Description ========================*/
/**
 * @file  audio_regs.h
 * The file is a declaration and definition header file
 * @author ivanlee
 * @date 2009.06.14
 * @version 0.1
 * @ingroup drv_audio
 */

#ifndef _AUDIO_REG_H_
#define _AUDIO_REG_H_

#include "reg_def.h"		// this header file includes all system registers
#include "rbusAudioReg.h"	// this header file includes all details of audio registers

// regsiter defined in RTD2668-DesignSpec-Memory and Register Wrapper.doc
#define SYS_SRST_1_AUD_DAC2_BIT			_BIT3
#define SYS_SRST_1_AUD_DAC1_BIT			_BIT2
#define SYS_SRST_1_AUD_ADC_BIT			_BIT1
#define SYS_SRST_1_AUD_BIT				_BIT0
#define SYS_CLKEN_1_AUD_49MHZ_BIT		_BIT1
#define SYS_CLKEN_1_AUD_98MHZ_BIT		_BIT0
#define SYS_CLKEN_2_AUD_16MHZ_BIT		_BIT3
#define SYS_CLKEN_2_AUD_24MHZ_BIT		_BIT2
#define SYS_CLKEN_4_AUD_PRE_512FS_BIT	_BIT7
#define SYS_CLKEN_4_AUD_DAAD_256FS_BIT	_BIT6
#define SYS_CLKEN_4_AUD_DAAD_128FS_BIT	_BIT5
#define SYS_CLKEN_4_AUD_BIT				_BIT4
#define SYS_CLKEN_4_AUD2_512FS_BIT		_BIT2
#define SYS_CLKEN_4_AUD2_DAAD_256FS_BIT	_BIT1
#define SYS_CLKEN_4_AUD2_DAAD_128FS_BIT	_BIT0
#define SYS_PLL_AUDIO_0_PSAUD_DIV_MASK	(_BIT7|_BIT6|_BIT5|_BIT4)
#define SYS_PLL_AUDIO_0_PSAUD_PSEN_BIT	_BIT3
#define SYS_PLL_AUDIO_0_PSAUD_EN_BIT	_BIT2
#define SYS_PLL_AUDIO_0_PSAUD_TST_BIT	_BIT1
#define SYS_PLL_AUDIO_0_PSAUD_CTRL_BIT	_BIT0
#define SYS_PLL_AUDIO_1_RSTB_BIT		_BIT7
#define SYS_PLL_AUDIO_1_OEB1_BIT		_BIT3
#define SYS_PLL_AUDIO_1_OEB2_BIT		_BIT2
#define SYS_PLL_AUDIO_1_PWDN_BIT		_BIT1
#define SYS_PLL_AUDIO_1_TST_BIT			_BIT0
#define SYS_PLL_AUDIO_2_RS_MASK			(_BIT6|_BIT5|_BIT4)
#define SYS_PLL_AUDIO_2_CS_MASK			(_BIT3|_BIT2)
#define SYS_PLL_AUDIO_2_WDRST			_BIT1
#define SYS_PLL_AUDIO_2_WDSET			_BIT0
#define SYS_PLL_AUDIO_3_IP_MASK			(_BIT2|_BIT1|_BIT0)
#define SYS_PLL_512FS_0_OEB_BIT			_BIT7
#define SYS_PLL_512FS_0_RS_MASK			(_BIT6|_BIT5|_BIT4)
#define SYS_PLL_512FS_0_WDRST_BIT		_BIT3
#define SYS_PLL_512FS_0_WDSET_BIT		_BIT2
#define SYS_PLL_512FS_0_PWDN_BIT		_BIT1
#define SYS_PLL_512FS_0_TST_BIT			_BIT0
#define SYS_PLL_512FS_1_RSTB_BIT		_BIT7
#define SYS_PLL_512FS_1_P_MASK			(_BIT6|_BIT5|_BIT4)
#define SYS_PLL_512FS_1_CS_MASK			(_BIT1|_BIT0)
#define SYS_PLL_512FS_2_M_MASK			(_BIT7|_BIT6|_BIT5|_BIT4|_BIT3|_BIT2|_BIT1|_BIT0)
#define SYS_PLL_512FS_3_BPN_BIT			_BIT7
#define SYS_PLL_512FS_3_BPS_BIT			_BIT6
#define SYS_PLL_512FS_3_2048FS_TST_BIT	_BIT5
#define SYS_PLL_512FS_3_WDVSET_BIT		_BIT4
#define SYS_PLL_512FS_3_N_MASK			(_BIT3|_BIT2|_BIT1|_BIT0)
#define SYS_PLL_512FS_4_IP_MASK			(_BIT2|_BIT1|_BIT0)

/*
 * bit-field definition of Audio Registers
 */
//---- Global Registers Group ----//
// AD_GLOBAL_CTRL_REG
enum
{
    AD_PLL_64_512	= 0x00,
    AD_PLL_SPDIF_TRACKING = 0x01,
    AD_PLL_AUDIO	= 0x02,
    AD_PLL_RESAMPLE = 0x03
};
// AD_DAIO_SEL_REG
enum
{
    AD_IPT_IS_TV	= 0x00,
    AD_IPT_IS_ADC	= 0x01,
    AD_IPT_IS_I2S	= 0x02,
    AD_IPT_IS_SPDIF	= 0x03,
    AD_IPT_IS_HDMI	= 0x04,
    AD_IPT_IS_CANEL	= 0x05,
    AD_IPT_IS_RSD1	= 0x06,
    AD_IPT_IS_RSD2	= 0x07
};
// AD_AAIO_SEL_REG
enum
{
    AD_HPOUT_IS_BBDAC = 0x00,
    AD_HPOUT_IS_GND = 0x01,			// IC verison : A~D
//	AD_HPOUT_IS_WIREIN = 0x01,		// IC verison : in the feature
};
enum
{
    AD_LINEOUT_IS_BBDAC = 0x00,
    AD_LINEOUT_IS_WIREIN = 0x01,
};
// AD_SPDIF_IO_SEL_REG
enum
{
    AD_SPDIF_IN_SPDIF	= 0x00,
    AD_SPDIF_IN_HDMI	= 0x01,
    AD_SPDIF_IN_I2S		= 0x02,
    AD_SPDIF_IN_LOOP	= 0x03
};
enum
{
    AD_SPDIF_OUT_SPDIF	= 0x00,
    AD_SPDIF_OUT_HDMI	= 0x01,
    AD_SPDIF_OUT_HWPP	= 0x02,
    AD_SPDIF_OUT_FLOAT	= 0x03
};
// AD_SPDIF_ICS2_REG
enum
{
    AD_SPDIF_FRQ_24k = 0x06,
    AD_SPDIF_FRQ_32k = 0x03,
    AD_SPDIF_FRQ_44_1k =	0x0,
    AD_SPDIF_FRQ_48k = 0x02,
    AD_SPDIF_FRQ_96k = 0x0A,
    AD_SPDIF_FRQ_192k = 0x0E
};
//---- TVSound Registers Group ----//
// AD_AUTO_STD_DETECT_REG
enum { AD_MN_IS_BTSC = 0x0,	AD_MN_IS_EIAJ};
enum { AD_DKL_IS_DK = 0x0,	AD_DKL_IS_L};
enum
{
    AD_TV_SOUND_SEL_FMAM = 0x0,
    AD_TV_SOUND_SEL_SAB = 0x01,
    AD_TV_SOUND_SEL_SA = 0x02,
    AD_TV_SOUND_SEL_SB = 0x03
};
enum
{
    AD_TV_STD_A2_BG = 0x00,
    AD_TV_STD_NICAM_BG = 0x01,
    AD_TV_STD_A2_DK1 = 0x02,
    AD_TV_STD_A2_DK2 = 0x03,
    AD_TV_STD_A2_DK3 = 0x04,
    AD_TV_STD_NICAM_DK = 0x05,
    AD_TV_STD_A2_MN = 0x06,
    AD_TV_STD_EIAJ = 0x07,
    AD_TV_STD_BTSC = 0x08,
    AD_TV_STD_NICAM_L = 0x09,
    AD_TV_STD_NICAM_I = 0x0A,
    AD_TV_STD_A2_HDEV = 0x0B,
    AD_TV_STD_NICAM_L_WO_DIGI = 0x0C,
    AD_TV_STD_NICAM_I_WO_DIGI = 0x0D,
    AD_TV_STD_NICAM_BG_WO_DIGI = 0x0E,
    AD_TV_STD_NICAM_DK_WO_DIGI = 0x0F,
    AD_TV_STD_UNKNOWN = 0x1F
};

// AD_CONST_PRESCALE_REG
enum
{
    AD_HW_AM_PRESCALE_0DB = 0x00,
    AD_HW_AM_PRESCALE_6DB = 0x01,
    AD_HW_AM_PRESCALE_12DB =	0x02,
    AD_HW_AM_PRESCALE_18DB =	0x03
};
enum
{
    AD_FM1_DEVIATION_18K = 0x00,
    AD_FM1_DEVIATION_25K = 0x01,
    AD_FM1_DEVIATION_28K = 0x02,
    AD_FM1_DEVIATION_50K = 0x03,
    AD_FM1_DEVIATION_100K = 0x04,
    AD_FM1_DEVIATION_150K = 0x05,
    AD_FM1_DEVIATION_180K = 0x06,
    AD_FM1_DEVIATION_300K = 0x07
};
enum
{
    AD_FM2_DEVIATION_16K = 0x00,
    AD_FM2_DEVIATION_25K = 0x01,
    AD_FM2_DEVIATION_28K = 0x02,
    AD_FM2_DEVIATION_50K = 0x03,
    AD_FM2_DEVIATION_100K = 0x04,
    AD_FM2_DEVIATION_150K = 0x05,
    AD_FM2_DEVIATION_180K = 0x06,
    AD_FM2_DEVIATION_300K = 0x07
};
enum { AD_FM_SAP2_CONST_10K = 0x00, AD_FM_SAP2_CONST_20K = 0x01};
enum
{
    AD_A2_BW_SEL_50K = 0x00,
    AD_A2_BW_SEL_70K = 0x01,
    AD_A2_BW_SEL_100K = 0x02,
    AD_A2_BW_SEL_150K = 0x03
};

// AD_DEMO_CTRL1_REG
enum
{
    AD_DE_EMPHASIS_50US = 0x00,
    AD_DE_EMPHASIS_75US = 0x01,
    AD_DE_EMPHASIS_J17 = 0x02,
    AD_DE_EMPHASIS_OFF = 0x03
};
// HWProcessing Registers Group
// AD_HW_PP_CTRL_REG
enum { AD_BASIC_MODE = 0x00, AD_PREMIUM_MODE };

enum
{
    AD_MSD_FREQ_OFFSET_N50 = 0x00,
    AD_MSD_FREQ_OFFSET_P50 = 0x01,
    AD_MSD_FREQ_OFFSET_N100 = 0x02,
    AD_MSD_FREQ_OFFSET_P100 = 0x03,
    AD_MSD_FREQ_OFFSET_N150 = 0x04,
    AD_MSD_FREQ_OFFSET_P150 = 0x05,
    AD_MSD_FREQ_OFFSET_N200 = 0x06,
    AD_MSD_FREQ_OFFSET_P200 = 0x07,
};
// DMA Registers Group
// Analog Registers Group
// Interface Registers Group
// TestMode Registers Group


#endif // end of _AUDIO_REG_H_

