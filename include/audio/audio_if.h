/*=============================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2007 *
 * All rights reserved.                                       *
 *============================================================*/
/*======================= Description ========================*/
/**
 * @file  audio_if.h
 *
 * @author
 * @date
 * @version 0.1
 * @ingroup drv_adc
 *
 *=============================================================*/
#ifndef	_AUDIO_IF_H_
#define	_AUDIO_IF_H_

typedef struct SPDIF_PLL_TAB_TAG
{
    UINT8 Ccode;
    UINT8 Dcode;
    UINT8 Ecode;
    UINT8 Acode;
} SPDIF_PLL_TAB;

typedef struct I2S_PLL_TAB_TAG
{
    UINT8 bps;
    UINT8 bpn;
    UINT8 p;
    UINT8 n;
    UINT8 m;
} I2S_PLL_TAB;

typedef enum _AUD_BBDAC_MUTE_SRC
{
    AUD_BBDAC_MUTE_AMIX1 = 0x01,
    AUD_BBDAC_MUTE_AMIX2 = 0x02,
    AUD_BBDAC_MUTE_LINEOUT = 0x04,
    AUD_BBDAC_MUTE_HPOUT = 0x08,
} AUD_BBDAC_MUTE_SRC;

typedef enum
{
    // bit-mapping setting
    AUD_BBADC_MUTE_ALL = 0,
    AUD_BBADC_MONOIN = 0x01,
    AUD_BBADC_AIO2 = 0x02,
    AUD_BBADC_AIO1 = 0x04,
    AUD_BBADC_AIN5 = 0x08,
    AUD_BBADC_AIN4 = 0x10,
    AUD_BBADC_AIN3 = 0x20,
    AUD_BBADC_AIN2 = 0x40,
    AUD_BBADC_AIN1 = 0x80
} AUD_BBADC_SRC;

typedef enum _AUD_HPOUT_CFG
{
    AUD_HPOUT_IS_BBDAC = 0x00,
    AUD_HPOUT_IS_WIREIN = 0x01,
} AUD_HPOUT_CFG;

typedef enum _AUD_LINEOUT_CFG
{
    AUD_LINEOUT_IS_BBDAC = 0x00,
    AUD_LINEOUT_IS_WIREIN = 0x01,
} AUD_LINEOUT_CFG;

typedef enum _AUD_INOUT_CFG
{
    AUD_IS_INPUT = 0x00,
    AUD_IS_OUTPUT = 0x01,
} AUDINOUT_CFG;

typedef enum
{
    // bit-mapping setting
    AUD_WIREIN_AIN1 = 0x01,
    AUD_WIREIN_AIN2 = 0x02,
    AUD_WIREIN_MONO_IN = 0x03,
    AUD_WIREIN_DISABLE = 0x4,
} AUD_WIREIN_SRC;

typedef enum
{
    AUD_MONO_PIN_IS_MONO_IO = 0x00,
    AUD_MONO_PIN_IS_STEREO_IN =0x01,
} AUD_MONO_PIN_CFG;

typedef enum _AUD_SPDIFO_CFG
{
    AUD_SPDIFO_IN = 0,
    AUD_SPDIFO_HDMI,
    AUD_SPDIFO_HWPP,
    AUD_SPDIFO_OFF,
} AUD_SPDIFO_CFG;

typedef enum _AUD_SPDIFI_CFG
{
    AUD_SPDIFI_IN = 0,
    AUD_SPDIFI_HDMI,
    AUD_SPDIFI_I2S,
    AUD_SPDIFI_LOOPBACK,
} AUD_SPDIFI_CFG;

typedef enum _AUD_I2SO_CFG
{
    AUD_I2SO_IN = 0,
    AUD_I2SO_HWPP,
    AUD_I2SO_HDMI,
    AUD_I2SO_OFF,
} AUD_I2SO_CFG;

typedef enum _AUD_I2SI_CFG
{
    AUD_I2SI_IN = 0,
    AUD_I2SI_HDMI,
    AUD_I2SI_LOOPBACK,
    AUD_I2SI_DISABLE,
} AUD_I2SI_CFG;

// Las Vegas ---
#define AIO_BBADC_MAX_GAIN	(0x37)
#define AIO_BBADC_MIN_GAIN	(0x00)
#define AIO_BBADC_0DB_GAIN	(0x27)

typedef enum
{
    AIO_DATA_IS_PCM,
    AIO_DATA_IS_NONPCM
} AIO_DATA_TYPE;

typedef enum
{
    // identical to the order in register's field
    AIO_POW_MONO_IN  = 1<<0,
    AIO_POW_AIN5     = 1<<1,
    AIO_POW_AIN4     = 1<<2,
    AIO_POW_AIN3     = 1<<3,
    AIO_POW_AIN2     = 1<<4,
    AIO_POW_AIN1     = 1<<5,
    AIO_POW_AIO_MONO = 1<<6,
    AIO_POW_MONO_OUT = 1<<7,
    AIO_POW_DACHP    = 1<<8,
    AIO_POW_AOUT     = 1<<9,
    AIO_POW_AIO2     = 1<<10,
    AIO_POW_AIO1     = 1<<11,
    AIO_POW_MONO_DAC = 1<<12,
    AIO_POW_DACVREF  = 1<<13,
    AIO_POW_VREF     = 1<<14,
    AIO_POW_MBIAS    = 1<<15,
    AIO_POW_BBDAC1   = 1<<16,
    AIO_POW_BBDAC0   = 1<<17,
    AIO_POW_BBADC    = 1<<18,
    AIO_POW_MASTER   = 1<<19,
    AIO_POW_MIXER    = 1<<20,
    AIO_POW_AFE      = 1<<21,
    AIO_DACVREF_MODE = 1<<22
} AIO_ANA_POWER;

typedef enum
{
    AIO_PEAK_SRC_ATV   = 0x0,
    AIO_PEAK_SRC_BBADC = 0x1,
    AIO_PEAK_SRC_I2S   = 0x2,
    AIO_PEAK_SRC_SPDIF = 0x4,
    AIO_PEAK_SRC_PCM   = 0x5
} AIO_PEAK_SRC;

typedef enum
{
    AIO_FIFO_SEL_CH_12 = 0,
    AIO_FIFO_SEL_CH_34 = 1,
    AIO_FIFO_SEL_CH_56 = 2,
    AIO_FIFO_SEL_CH_78 = 3
} AIO_FIFO_SEL;

typedef enum
{
    AIO_FIFO_MIXING_DISABLE = 0,
    AIO_FIFO_MIXING_CH_34   = 1,
    AIO_FIFO_MIXING_CH_56   = 2,
    AIO_FIFO_MIXING_CH_78   = 3
} AIO_FIFO_MIXING_SEL;

typedef enum
{
    AIO_FIFO_MIXING_FAC_1_DIV_1 = 0,
    AIO_FIFO_MIXING_FAC_1_DIV_2 = 1,
    AIO_FIFO_MIXING_FAC_1_DIV_4 = 2,
    AIO_FIFO_MIXING_FAC_1_DIV_8 = 3
} AIO_FIFO_MIXING_FAC;

typedef struct
{
    UINT8	i2s_out_0_sel;
    UINT8	i2s_out_1_sel;
    UINT8	i2s_out_2_sel;
    UINT8	i2s_out_3_sel;
    UINT8	spdif_out_sel;
    UINT8	dac_out_sel;
    UINT8	pcm_out_sel;
    UINT8	mixing_sel;
    UINT8	mixing_fac_12;
    UINT8	mixing_fac_other;
} AIO_FIFO_CFG;


// BBADC-related --------------------//
typedef enum
{
    // bit-mapping setting
    AIO_BBADC_MUTE_ALL = 0x0000,
    AIO_BBADC_MONOIN   = (0x1<<0),		// this source maybe STEREO_IN or not,depends on pin_mux setting
    AIO_BBADC_AIO2     = (0x1<<1),
    AIO_BBADC_AIO1     = (0x1<<2),
    AIO_BBADC_AIN5_1   = (0x1<<3),				// use bit8 to indicate AIN5 source, 0 means source1
    AIO_BBADC_AIN5_2   = (0x1<<3) | (0x1<<8),	// use bit8 to indicate AIN5 source, 1 means source2
    AIO_BBADC_AIN4     = (0x1<<4),
    AIO_BBADC_AIN3     = (0x1<<5),
    AIO_BBADC_AIN2     = (0x1<<6),
    AIO_BBADC_AIN1     = (0x1<<7),
    AIO_BBADC_UNKNOWN  = (0x1<<9)
} AIO_BBADC_SRC;

typedef enum
{
    // bit-mapping setting
    AIO_WIREIN_MUTE_ALL = 0x00,
    AIO_WIREIN_AIN1     = 1<<0,
    AIO_WIREIN_AIN2     = 1<<1,
    AIO_WIREIN_MONOIN   = 1<<2,
    AIO_WIREIN_BBADC    = 1<<3
} AIO_WIREIN_SRC;

typedef enum
{
    AIO_MONOIN_IN      = 0x0,
    AIO_MONOIN_OUT_T   = 0x1,
    AIO_MONOIN_STEREO  = 0x2,
//	AIO_MONOIN_UNKNOWN = 0x3
} AIO_MONOIN_SRC;


typedef struct
{
    // identical to the order in register's field
    UINT8	vol1_l_gain:6;				// BIT[24:29]
    UINT8	resvd_1:1;					// BIT[30]
    UINT8	vol1_l_smute_enable:1;		// BIT[31]

    UINT8	vol1_r_gain:6;				// BIT[16:21]
    UINT8	resvd_2:1;					// BIT[22]
    UINT8	vol1_r_smute_enable:1;		// BIT[23]

    UINT8	boost_sel:2;				// BIT[08:09]
    UINT8	soft_vol_ctrl:4;			// BIT[10:13]
    UINT8	resvd_3:2;					// BIT[14:15]

    UINT8	vol2_gain:6;				// BIT[00:05]
    UINT8	resvd_4:1;					// BIT[06]
    UINT8	vol2_smute_enable:1;		// BIT[07]
} AIO_BBADC_CFG;


// I2S-related ----------------------//
typedef enum
{
    // identical to the order in register's field
    AIO_I2SI_IN       = 0x0,
    AIO_I2SI_HDMI     = 0x1,
    AIO_I2SI_LOOPBACK = 0x2,
    AIO_I2SI_DISABLE  = 0x3
} AIO_I2SI_SRC;

typedef enum
{
    // identical to the order in register's field
    AIO_I2SI_SYNC_NORMAL  = 0x0,
    AIO_I2SI_SYNC_LEFT    = 0x1,
    AIO_I2SI_SYNC_RGHT    = 0x2,
//	AIO_I2SI_SYNC_NORMAL2 = 0x3
} AIO_I2SI_SYNC;

typedef enum
{
    // identical to the order in register's field
    AIO_I2SI_SLAVE  = 0x0,
    AIO_I2SI_MASTER = 0x1
} AIO_I2SI_MODE;

typedef enum
{
    AIO_I2SI_ID_PRI,
    AIO_I2SI_ID_SEC,
    AIO_I2SI_PLL_DVR
} AIO_I2SI_ID;


typedef struct
{
//	UINT8	resvd:2;
    UINT8	src_sel:2;			// refer to the definition of AIO_I2SI_SRC
    UINT8	sync_type:2;		// refer to the definition of AIO_I2SI_SYNC
    UINT8	mode_sel:1;			// refer to the definition of AIO_I2SI_MODE
    UINT8	update_src_only:1;	// update src_sel only and keep others in default value.
} AIO_I2SI_CFG;


typedef enum
{
    // identical to the order in register's field
    AIO_I2SO_MCLK_128FS   = 0x0,
    AIO_I2SO_MCLK_256FS   = 0x1,
    AIO_I2SO_MCLK_512FS   = 0x2,
    AIO_I2SO_MCLK_DISABLE = 0x3
} AIO_I2SO_MCLK;

typedef enum
{
    // identical to the order in register's field
    AIO_I2SO_FIFO      = 0x0,
    AIO_I2SO_IN_SLAVE  = 0x1,
    AIO_I2SO_HDMI      = 0x2,
    AIO_I2SO_IN_MASTER = 0x3,
    AIO_I2SO_DISABLE   = 0x4
} AIO_I2SO_SRC;


typedef struct
{
//	UINT8	resvd:2;
    UINT8	update_src_only:1;
    UINT8	src_sel:3;
    UINT8	mclk_sel:2;
} AIO_I2SO_CFG;


// SPDIF-related --------------------//
typedef enum
{
    // identical to the order in register's field
    AIO_SPDIFI_IN       = 0x0,
    AIO_SPDIFI_HDMI     = 0x1,
    AIO_SPDIFI_LOOPBACK = 0x2,
    AIO_SPDIFI_DISABLE  = 0x3
} AIO_SPDIFI_SRC;

typedef enum
{
    // identical to the order in register's field
    AIO_SPDIFI_REQ_NO  = 0x0,
    AIO_SPDIFI_REQ_YES = 0x1
} AIO_SPDIFI_REQ_TYPE;


typedef struct
{
//	UINT8	resvd:4;
    UINT8	src_sel:2;			// refer to the definition of AIO_SPDIFI_SRC
    UINT8	req_after_lock:1;	// [0,1] = [No,Yes]
    UINT8	update_src_only:1;	// keep default look&debounce parameter and update src_sel only.
} AIO_SPDIFI_CFG;


typedef enum
{
    // identical to the order in register's field
    AIO_SPDIFO_FIFO,
    AIO_SPDIFO_IN,
    AIO_SPDIFO_HDMI,
    AIO_SPDIFO_DISABLE
} AIO_SPDIFO_SRC;

typedef struct
{
//	UINT8	resvd:3;
    UINT8	data_type:1;
    UINT8	update_src_only:1;
    UINT8	src_sel:2;
    UINT8	valid_ctrl:1;
} AIO_SPDIFO_CFG;


// PCM-related ----------------------//
typedef enum
{
    // identical to the order in register's field
    AIO_PCM_8BITS  = 0x0,
    AIO_PCM_12BITS = 0x1,
    AIO_PCM_16BITS = 0x2,
    AIO_PCM_20BITS = 0x3,
    AIO_PCM_24BITS = 0x4,
    AIO_PCM_28BITS = 0x5,
    AIO_PCM_32BITS = 0x6
} AIO_PCM_BITS;

typedef enum
{
    // identical to the order in register's field
    AIO_PCM_SYNC_TYPE_A = 0x0,
    AIO_PCM_SYNC_TYPE_B = 0x1
} AIO_PCM_SYNC;

typedef enum
{
    // identical to the order in register's field
    AIO_PCM_SCLK_NEG = 0x0,
    AIO_PCM_SCLK_POS = 0x1
} AIO_PCM_SCLK;

typedef enum
{
    // identical to the order in register's field
    AIO_PCMI_MODE_SLAVE  = 0x0,
    AIO_PCMI_MODE_MASTER = 0x1
} AIO_PCMI_MODE;

typedef enum
{
    // identical to the order in register's field
    AIO_PCMI_MONO   = 0x0,
    AIO_PCMI_STEREO = 0x1
} AIO_PCMI_SOUND;

typedef enum
{
    // identical to the order in register's field
    AIO_PCMI_EXT     = 0x0,
    AIO_PCMI_INT     = 0x1,
    AIO_PCMI_DISABLE = 0x2
    //AIO_PCMI_DISABLE2 = 0x3
} AIO_PCMI_SRC;

/*
typedef struct {
	unsigned	resvd:22;
	unsigned	update_src_only:1;
	unsigned	src_sel:2;
	unsigned	pcm_width:3;
	unsigned	mode_sel:1;
	unsigned	sync_mode:1;
	unsigned	sclk_polarity:1;
	unsigned	sound_mode:1;
}AIO_PCMI_CFG;
*/

typedef enum
{
    // identical to the order in register's field
    AIO_PCMO_MCLK_128FS,
    AIO_PCMO_MCLK_256FS,
    AIO_PCMO_MCLK_512FS,
    AIO_PCMO_MCLK_DISABLE
} AIO_PCMO_MCLK;

typedef enum
{
    // identical to the order in register's field
    AIO_PCMO_FIFO      = 0x0,
    AIO_PCMO_IN_SLAVE  = 0x1,
    AIO_PCMO_IN_MSATER = 0x2,
    AIO_PCMO_DISABLE   = 0x3
} AIO_PCMO_SRC;

/*
typedef struct {
	unsigned	resvd:22;
	unsigned	update_src_only:1;
	unsigned	src_sel:2;
	unsigned	pcm_width:3;
	unsigned	mode_sel:1;
	unsigned	sclk_polarity:1;
	unsigned	mclk_sel:2;
}AIO_PCMO_CFG;
*/

// DAC/HP-related -------------------//
typedef enum
{
    // bit-mapping setting
    AIO_HP_MUTE_ALL = 0x00,
    AIO_HP_PB_DAC   = 1<<0,
    AIO_HP_TS_DAC   = 1<<1,
    AIO_HP_WIRE_1   = 1<<2,
    AIO_HP_WIRE_2   = 1<<3
} AIO_HP_SRC;

typedef enum
{
    // identical to the order in register's field
    // bit-mapping setting
    AIO_WIREOUT_MUTE_ALL = 0x07,
    AIO_WIREOUT_TS_DAC   = 1<<0,
    AIO_WIREOUT_WIREIN   = 1<<1,
    AIO_WIREOUT_PB_DAC   = 1<<2
} AIO_WIREOUT_SRC;

typedef enum
{
    // bit-mapping setting
    AIO_DACMIX_MUTE_ALL = 0x00,
    AIO_DACMIX_AMIX_1   = 1<<0,
    AIO_DACMIX_AMIX_2   = 1<<1
} AIO_DACMIX_SRC;

typedef enum
{
    // identical to the order in register's field
    AIO_OPT_CHANNEL_STEREO  = 0x0,
    AIO_OPT_CHANNEL_STEREO1 = 0x1,
    AIO_OPT_CHANNEL_L_TO_R  = 0x2,
    AIO_OPT_CHANNEL_R_TO_L  = 0x3
} AIO_OPT_CHANNEL;


/*============ Prototype of Driver Routines ===================*/
/*=============================================================*/
/**
 * Audio_AioInit_Step2
 * init. audio interface & analog block
 * 1. deal with power-on pop noise
 * 2. config pads setting
 * 3. mute/unmute interface
 *
 * @param <void>	{ void }
 * @return 			{ void }
 * @ingroup drv_adc
 */
void Audio_AioInit_Step1();

/*=============================================================*/
/**
 * Audio_AioInit_Step2
 * init. audio interface & analog block
 * 1. deal with power-on pop noise
 * 2. config pads setting
 * 3. mute/unmute interface
 *
 * @param <void>	{ void }
 * @return 			{ void }
 * @ingroup drv_adc
 */
void Audio_AioInit_Step2();

/**
 * Select BBADC input source.
 * This function is usually used with YPbPr/VGA/CVBS/S-Video sources
 *
 * @param <src>	{ AIO_BBADC_AIO1 | BBADC_AIO2 | ..., refer to audio_aio.h }
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetIptSrc_BBADC(UINT16 src);

/**
 * Select I2S_IN_Primary input source.
 * This function is usually used with HDMI source
 *
 * @param <src>	{ AIO_I2SI_IN / AIO_I2SI_HDMI / AIO_I2S_LOOPBCK / AIO_I2S_DISABLE }
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetIptSrc_I2SPri(UINT8 src);

/**
 * Select I2S_IN_Secondary input source.
 * This function is usually used with HDMI source
 *
 * @param <src>	{ AIO_I2SI_IN / AIO_I2SI_HDMI / AIO_I2S_LOOPBCK / AIO_I2S_DISABLE }
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetIptSrc_I2SSec(UINT8 src);

/**
 * Select SPDIF_IN input source.
 * This function is usually used with HDMI source
 *
 * @param <src>	{ AIO_SPDIFI_IN / AIO_SPDIFI_HDMI / AIO_SPDIFI_LOOPBACK / AIO_SPDIFI_DISABLE }
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetIptSrc_SPDIF(UINT8 src);

/**
 * Select MONO_IN input source.
 * This function is used with BBADC/Wire1/Wire2 input sources.
 *
 * @param <src>	{ AIO_MONOIN_IN / AIO_MONOIN_OUT_T / AIO_MONOIN_STEREO }
 * @return		{ void }
 * @note
 *	This function will affect MONO_OUT configuration!!! (call it after depop-process done.)
 *
 * @ingroup lib_audio
 */
void Audio_AioSetIptSrc_MonoIn(UINT8 src);

/**
 * Select Wire1 input source.
 *
 * @param <src>	{ AIO_WIREIN_AIN1 | AIO_WIREIN_AIN2 | AIO_WIREIN_MONOIN | AIO_WIREIN_BBADC }
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetIptSrc_Wire1(UINT8 mix_src);

/**
 * Select Wire2 input source.
 *
 * @param <src>	{ AIO_WIREIN_AIN1 | AIO_WIREIN_AIN2 | AIO_WIREIN_MONOIN | AIO_WIREIN_BBADC }
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetIptSrc_Wire2(UINT8 mix_src);

/**
 * Setup BBADC input source.
 *
 * @param <p_cfg_bbadc>	{ pointer of BBADC control block }
 * @return				{ AIO_OK or AIO_ERROR_CODE }
 * @ingroup lib_audio
 */
void Audio_AioCfgIptSrc_BBADC(AIO_BBADC_CFG *p_cfg_bbadc);

/**
 * Setup I2S_IN_Primary input source.
 *
 * @param <p_cfg_i2s>	{ pointer of I2S_IN control block }
 * @return				{ AIO_OK or AIO_ERROR_CODE }
 * @ingroup lib_audio
 */
void Audio_AioCfgIptSrc_I2SPri(AIO_I2SI_CFG *p_cfg_i2s);

/**
 * Setup I2S_IN_Secondary input source.
 *
 * @param <p_cfg_i2s>	{ pointer of I2S_IN control block }
 * @return				{ AIO_OK or AIO_ERROR_CODE }
 * @ingroup lib_audio
 */
void Audio_AioCfgIptSrc_I2SSec(AIO_I2SI_CFG *p_cfg_i2s);

/**
 * Setup SPDIF_IN input source.
 *
 * @param <p_cfg_spdif>	{ pointer of SPDIF_IN control block }
 * @return				{ void }
 * @ingroup lib_audio
 */
//void Audio_AioCfgIptSrc_SPDIF(AIO_SPDIFI_CFG *p_cfg_spdif);

/**
 * Setup I2S_OUT source.
 *
 * @param <p_cfg_i2s>	{ pointer of I2S_OUT control block }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_AioCfgOptSrc_I2S(AIO_I2SO_CFG *p_cfg_i2s);

/**
 * Setup SPDIF_OUT source.
 *
 * @param <p_cfg_spdif>	{ pointer of SPDIF_OUT control block }
 * @return				{ void }
 * @ingroup lib_audio
 */
//void Audio_AioCfgOptSrc_SPDIF(AIO_SPDIFO_CFG *p_cfg_spdif);

/**
 * Setup output FIFO.
 *
 * @param <p_cfg_fifo>	{ pointer of AIO_FIFO_CFG control block }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_AioCfgOptFIFO(AIO_FIFO_CFG *p_cfg_fifo);

/**
 * Select I2S_OUT source.
 *
 * @param <src>	{ AIO_I2SO_FIFO / AIO_I2SO_IN_SLAVE / AIO_I2SO_IN_MASTER / AIO_I2SO_IN_HDMI / AIO_I2SO_DISABLE }
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetOptSrc_I2S(UINT8 src);

/**
 * Select SPDIF_OUT source.
 *
 * @param <src>	{ AIO_SPDIFO_FIFO / AIO_SPDIFO_IN / AIO_SPDIFO_HDMI / AIO_SPDIFO_DISABLE }
 * @return		{ void }
 * @ingroup lib_audio
 */
//void Audio_AioSetOptSrc_SPDIF(UINT8 src);

/**
 * Select Headphone output source.
 *
 * @param <src>	{ AIO_HP_SRC_PB_DAC | AIO_HP_SRC_TS_DAC | AIO_HP_SRC_WIRE_1 | AIO_HP_SRC_WIRE_2 }
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetOptSrc_Headphone(UINT8 mix_src);

/**
 * Select PB_DAC_Mixer output.
 * HWP-Post-FIFO always output to Mixer and user can add AMIX_1 and/or AMIX_2 to Mixer output.
 *
 * @param <src>	{ AIO_DACMIX_AMIX_1 | AIO_DACMIX_AMIX_2 }
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetOptSrc_DACMixer(UINT8 mix_src);

/**
 * Select AIO1 output source.
 *
 * @param <src>	{ AIO_WIREOUT_PB_DAC | AIO_WIREOUT_TS_DAC | AIO_WIREOUT_WIREIN }
 * @return		{ AIO_OK or AIO_ERROR_CODE }
 * @ingroup lib_audio
 */
void Audio_AioSetOptSrc_AIO1(UINT8 mix_src);

/**
 * Select AIO2 output source.
 *
 * @param <src>	{ AIO_WIREOUT_PB_DAC | AIO_WIREOUT_TS_DAC | AIO_WIREOUT_WIREIN }
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetOptSrc_AIO2(UINT8 mix_src);

/**
 * Select Headphone output channel configuration.
 *
 * @param <sel>	{ AIO_OPT_CHANNEL_STEREO | AIO_OPT_CHANNEL_L_TO_R | AIO_OPT_CHANNEL_R_TO_L }
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetOptChannel_Headphone(UINT8 sel);

/**
 * Select Lineout output channel configuration.
 *
 * @param <sel>	{ AIO_OPT_CHANNEL_STEREO | AIO_OPT_CHANNEL_L_TO_R | AIO_OPT_CHANNEL_R_TO_L }
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetOptChannel_Lineout(UINT8 sel);

/**
 * Select AIO1 output channel configuration.
 *
 * @param <sel>	{ AIO_OPT_CHANNEL_STEREO | AIO_OPT_CHANNEL_L_TO_R | AIO_OPT_CHANNEL_R_TO_L }
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetOptChannel_AIO1(UINT8 sel);

/**
 * Select AIO2 output channel configuration.
 *
 * @param <sel>	{ AIO_OPT_CHANNEL_STEREO | AIO_OPT_CHANNEL_L_TO_R | AIO_OPT_CHANNEL_R_TO_L }
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetOptChannel_AIO2(UINT8 sel);

/**
 * Mute/Unmute Headphone(Amp) output.
 *
 * @param <para>	{ _ENABLE / _DISABLE }
 * @return			{ void }
 * @ingroup lib_audio
 */
void Audio_AioMute_Headphone(BOOL para);

/**
 * Mute/Unmute Lineout output.
 *
 * @param <para>	{ _ENABLE / _DISABLE }
 * @return			{ void }
 * @ingroup lib_audio
 */
void Audio_AioMute_Lineout(BOOL para);

/**
 * Mute/Unmute AIO1 output.
 *
 * @param <para>	{ _ENABLE / _DISABLE }
 * @return			{ void }
 * @ingroup lib_audio
 */
void Audio_AioMute_AIO1(BOOL para);

/**
 * Mute/Unmute AIO2 output.
 *
 * @param <para>	{ _ENABLE / _DISABLE }
 * @return			{ void }
 * @ingroup lib_audio
 */
void Audio_AioMute_AIO2(BOOL para);

/**
 * Set BBADC volume1
 * range from -58.5dB to +24dB , 1.5dB per step
 * [-58.5dB ~ 0dB ~ +24dB] = [ 0x00 ~ 0x27 ~ 0x37 ]
 *
 * @param <l_gain>	{ volume of left channel }
 * @param <r_gain>	{ volume of right channel }
 * @return			{ AIO_OK or AIO_ERROR_CODE }
 * @ingroup lib_audio
 */
void Audio_AioSetBBADCVol1(UINT8 l_gain, UINT8 r_gain);

/**
 * Set BBADC volume2
 * range from -58.5dB to +24dB , 1.5dB per step
 * [-58.5dB ~ 0dB ~ +24dB] = [ 0x00 ~ 0x27 ~ 0x37 ]
 *
 * @param <gain>	{ volume value }
 * @return			{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetBBADCVol2(UINT8 gain);

/**
 * Enable/Disable Sparrow Audio Analog Modules
 *
 * @param <para>		{ _ENABLE / _DISABLE }
 * @param <module_idx>	{ AIO_POW_XXX, refer to audio_if.h }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetAnalogPower(BOOL para, UINT32 module_idx);

/**
 * Enable/Disable LasVegas Audio HP-Amp output
 *
 * @param <para>		{ _ENABLE / _DISABLE }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetHeadphoneAmp(BOOL para);

#endif // _AUDIO_IF_H_

