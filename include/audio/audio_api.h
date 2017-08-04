/*=============================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2006 *
 * All rights reserved.                                       *
 *============================================================*/
/*======================= Description ========================*/
/**
 * @file  audio_api.h
 * The file is a declaration and definition header file
 * @author ivanlee
 * @date 2006.06.14
 * @version 0.1
 * @ingroup drv_adc
 *
 *============================================================*/
#ifndef _ADC_API_H
#define _ADC_API_H

#include "rtd_types.h"

enum
{
    ADC_UNMUTE=0,
    ADC_MUTE
};

typedef enum _ADC_CH_IDX
{
    // bit-mapping setting
    ADC_CH_L = 0x01,
    ADC_CH_R = 0x02,
    ADC_CH_LS = 0x04,
    ADC_CH_RS = 0x08,
    ADC_CH_C = 0x10,
    ADC_CH_SW = 0x20,
    ADC_CH_ALL = 0x3F,
} ADC_CH_IDX;

typedef enum _ADC_EQ_MODE
{
    ADC_EQ_DISABLE=0,
    ADC_EQ_CLUB,
    ADC_EQ_DANCE,
    ADC_EQ_LIVE,
    ADC_EQ_POP,
    ADC_EQ_ROCK,
    ADC_EQ_UNKNOWN
} ADC_EQ_MODE;


typedef enum _ADC_M2S_LEVEL
{
    ADC_M2S_L1 = 0x01F0,
    ADC_M2S_L2 = 0x0160,
    ADC_M2S_L3 = 0x0120,
    ADC_M2S_L4 = 0x0100
} ADC_M2S_LEVEL;

typedef enum _ADC_SPL_LEVEL
{
    ADC_SPL_L1 = 0x003F,
    ADC_SPL_L2 = 0x007F,
    ADC_SPL_L3 = 0x00DE,
    ADC_SPL_L4 = 0x0100
} ADC_SPL_LEVEL;

typedef struct _ADC_EQ_COEFF
{
    INT16 a1;
    INT16 a2;
    INT16 h0;
} ADC_EQ_COEFF;

typedef struct _ADC_EQ_INFO
{
    ADC_EQ_COEFF*	lp0_table;
    ADC_EQ_COEFF*	bp1_table;
    ADC_EQ_COEFF*	bp2_table;
    ADC_EQ_COEFF*	bp3_table;
    ADC_EQ_COEFF*	bp4_table;
    ADC_EQ_COEFF*	bp5_table;
} ADC_EQ_INFO;

typedef struct _ADC_FINE_VOL_COEFF
{
    INT16	LP0_H0;
    INT16	BP1_H0;
    INT16	BP2_H0;
} ADC_FINE_VOL_COEFF;

typedef struct _ADC_FINE_VOL_INFO
{
    ADC_FINE_VOL_COEFF*	fine_vol_table;
} ADC_FINE_VOL_INFO;

typedef struct _EQ_VALUE
{
    INT32	LPValue;
    INT32	BP0Value;
    INT32	BP1Value;
    INT32	BP2Value;
    INT32	BP3Value;
    INT32	BP4Value;
    INT32	BP5Value;
    UINT32	MaxValue;
} EQ_VALUE;

typedef struct _LOUD_VALUE
{
    INT32	LPValue;
    INT32	BP1Value;
    INT32	BP2Value;
    UINT32	MaxValue;
} LOUD_VALUE;

typedef enum _ADC_MUTE_TYPE
{
    ADC_SYS_MUTE= 0x0,
    ADC_HDMI_MUTE = 0x1
} ADC_MUTE_TYPE;
typedef void (*FP_AUDIO) (void);

typedef struct
{
    char *ptr;
    unsigned long len;
} BITSTREAM_FILE;

typedef enum _ADC_BEEPER_VOLUME
{
    ADC_BEEPER_VOLUME_P0DB  = 0x0,
    ADC_BEEPER_VOLUME_P3DB  = 0x1,
    ADC_BEEPER_VOLUME_P6DB,
    ADC_BEEPER_VOLUME_P9DB,
    ADC_BEEPER_VOLUME_P12DB,
    ADC_BEEPER_VOLUME_P15DB,
    ADC_BEEPER_VOLUME_P18DB,
    ADC_BEEPER_VOLUME_P21DB,
    ADC_BEEPER_VOLUME_P24DB,
    ADC_BEEPER_VOLUME_P27DB,
    ADC_BEEPER_VOLUME_P30DB,
    ADC_BEEPER_VOLUME_P33DB,
    ADC_BEEPER_VOLUME_P36DB,
    ADC_BEEPER_VOLUME_P39DB,
    ADC_BEEPER_VOLUME_P42DB,
    ADC_BEEPER_VOLUME_P45DB,
    ADC_BEEPER_VOLUME_MUTE,
} ADC_BEEPER_VOLUME;


#define AUD_ENABLE  (1)
#define AUD_DISABLE (0)

// Las Vegas ---
typedef enum
{
    AIO_PATH_ID_PB,
    AIO_PATH_ID_TS
} AIO_PATH_ID;

typedef enum
{
    AIO_PATH_SRC_ATV       = 0x00,	// real input, mapping to register bit-setting
    AIO_PATH_SRC_BBADC     = 0x01,	// real input, mapping to register bit-setting
    AIO_PATH_SRC_SPDIF     = 0x02,	// real input, mapping to register bit-setting
    AIO_PATH_SRC_PCM       = 0x03,	// real input, mapping to register bit-setting
    AIO_PATH_SRC_I2S_CH12  = 0x04,	// real input, mapping to register bit-setting
    AIO_PATH_SRC_I2S_CH34  = 0x05,	// real input, mapping to register bit-setting
    AIO_PATH_SRC_I2S_CH56  = 0x06,	// real input, mapping to register bit-setting
    AIO_PATH_SRC_I2S_CH78  = 0x07,	// real input, mapping to register bit-setting
    AIO_PATH_SRC_ATV_NICAM = 0x08,	// shared input(ATV), affect clk setting
    AIO_PATH_SRC_ATV_HDEV  = 0x09,	// shared input(ATV), affect clk setting
//	AIO_PATH_SRC_DTV       = 0x10,	// pseudo input, affect clk setting
//	AIO_PATH_SRC_FILE      = 0x20,	// pseudo input, affect clk setting
    AIO_PATH_SRC_UNKNOWN   = 0xFF
} AIO_PATH_SRC;

typedef struct
{
    AIO_PATH_ID		id;
    AIO_PATH_SRC	src[4];
} AIO_PATH_CFG;

#define USR_MUTE_NUM	5
#define LIB_MUTE_NUM	3
#define APP_MUTE_ID		(USR_MUTE_NUM + 0)
#define AIO_MUTE_ID		(USR_MUTE_NUM + 1)
#define ATV_MUTE_ID		(USR_MUTE_NUM + 2)
#define APP_MUTE_NUM	(USR_MUTE_NUM + LIB_MUTE_NUM)

#define APP_VOL_STEP_1DB	8					// 0.125dB per step
#define APP_MAX_VOL	((47.875)*APP_VOL_STEP_1DB)	// (+47.875dB)
#define APP_MIN_VOL (-72*APP_VOL_STEP_1DB)		// (-72dB)
#define APP_MAX_BAL	(50*APP_VOL_STEP_1DB)
#define APP_MIN_BAL (-50*APP_VOL_STEP_1DB)

#define APP_EQ_COEF_NUM		13
#if 0
#define APP_EQ_SR_NUM		9	// order is identical to struct of AIO_FS_ID in audio_drv.h
#else
#define APP_EQ_SR_NUM		1	// reduce code size, only use 48K table in Sparrow
#endif

#define APP_EQ_BAND_NUM		6
#define APP_EQ_BAND_ID0		0
#define APP_EQ_BAND_ID1		1
#define APP_EQ_BAND_ID2		2
#define APP_EQ_BAND_ID3		3
#define APP_EQ_BAND_ID4		4
#define APP_EQ_BAND_ID5		5

#define APP_LOUD_COEF_NUM	11//13
#define APP_LOUD_SR_NUM		9	// order is identical to struct of AIO_FS_ID in audio_drv.h

#define APP_LOUD_BAND_NUM	3
#define APP_LOUD_BAND_ID0	0
#define APP_LOUD_BAND_ID1	1
#define APP_LOUD_BAND_ID2	2
#define APP_BASS_BAND_ID	APP_LOUD_BAND_ID0
#define APP_TREBLE_BAND_ID	APP_LOUD_BAND_ID1

#define APP_MONI_SPECTRUM_NUM	16

typedef enum
{
    // bit-mapping setting
    APP_CH_ID_L	 = (0x1<<0),
    APP_CH_ID_R	 = (0x1<<1),
    APP_CH_ID_LS = (0x1<<2),
    APP_CH_ID_RS = (0x1<<3),
    APP_CH_ID_C  = (0x1<<4),
    APP_CH_ID_SW = (0x1<<5),
    APP_CH_ID_ALL = 0x03F
} APP_CH_ID;


// H/W-AVC related -------------------------
typedef enum
{
    APP_AVC_NOISE_THD_NONE  = 0,
    APP_AVC_NOISE_THD_M90DB = 1,
    APP_AVC_NOISE_THD_M84DB = 2,
    APP_AVC_NOISE_THD_M78DB = 4,
    APP_AVC_NOISE_THD_M72DB = 8,
    APP_AVC_NOISE_THD_M66DB = 16,
    APP_AVC_NOISE_THD_M60DB = 32,
    APP_AVC_NOISE_THD_M54DB = 64,
    APP_AVC_NOISE_THD_M48DB = 128,
    APP_AVC_NOISE_THD_M42DB = 256,
    APP_AVC_NOISE_THD_M36DB = 511	// prevent overflow
} APP_AVC_NOISE_THD;

typedef enum
{
    APP_AVC_SIGNAL_LVL_RSVD	 = 0,
    APP_AVC_SIGNAL_LVL_M42DB = 1,
    APP_AVC_SIGNAL_LVL_M36DB = 2,
    APP_AVC_SIGNAL_LVL_M33DB = 3,	// 32.6dB
    APP_AVC_SIGNAL_LVL_M30DB = 4,
    APP_AVC_SIGNAL_LVL_M27DB = 6,	// 26.6dB
    APP_AVC_SIGNAL_LVL_M24DB = 8,
    APP_AVC_SIGNAL_LVL_M21DB = 11,	// 21.3dB
    APP_AVC_SIGNAL_LVL_M18DB = 16,
    APP_AVC_SIGNAL_LVL_M15DB = 23,	// 14.9dB
    APP_AVC_SIGNAL_LVL_M12DB = 32,
    APP_AVC_SIGNAL_LVL_M9DB  = 46,	// 8.9dB
    APP_AVC_SIGNAL_LVL_M6DB  = 64,
    APP_AVC_SIGNAL_LVL_M3DB  = 91,	// 3dB
} APP_AVC_SIGNAL_LVL;

typedef enum
{
    APP_AVC_NOISE_GAIN_M6DB,
    APP_AVC_NOISE_GAIN_M12DB,
    APP_AVC_NOISE_GAIN_M18DB,
    APP_AVC_NOISE_GAIN_M24DB
} APP_AVC_NOISE_GAIN;

typedef enum
{
    APP_AVC_STEP_GAIN_1P000DB,
    APP_AVC_STEP_GAIN_0P500DB,
    APP_AVC_STEP_GAIN_0P250DB,
    APP_AVC_STEP_GAIN_0P125DB
} APP_AVC_STEP_GAIN;

typedef struct
{
    UINT8	noise_gain;		// refer to enum of APP_AVC_NOISE_GAIN
    UINT8	noise_thd;
    UINT8	level_max;
    UINT8	level_min;
    UINT8	step_up;		// refer to enum of APP_AVC_STEP_GAIN
    UINT8	step_down;		// refer to enum of APP_AVC_STEP_GAIN

    UINT8	thd_cnt;
    UINT16	win_zcross;
    UINT16	win_moniter;
} APP_AVC_CFG;

// H/W-EQ related --------------------------
typedef struct
{
    INT16	val;			// range from mix_val to max_val
    INT16	max_val;		// fill the possible max. value of above band values , must be multiple of 6.
    INT16	min_val;		// always equal to -max_val
} APP_TREBLE_VAL;

typedef struct
{
    INT16	val;			// range from mix_val to max_val
    INT16	max_val;		// fill the possible max. value of above band values , must be multiple of 6.
    INT16	min_val;		// always equal to -max_val
} APP_BASS_VAL;

typedef struct
{
    INT16	val[APP_EQ_BAND_NUM];
    INT16	max_val;		// fill the possible max. value of above band values , must be multiple of 6.
    INT16	min_val;		// always equal to -max_val
} APP_EQ_VAL;

typedef struct
{
    INT16	a1;
    INT16	a2;
    INT16	h0;
} APP_EQ_COEF;

typedef struct
{
    // 9 sample-rate(8k,11k,12k,16k,22k,24k,32k,44k,48k) * 1 band * 13 value
    APP_EQ_COEF	coef[APP_EQ_SR_NUM][APP_EQ_COEF_NUM];
} APP_TREBLE_CFG;

typedef struct
{
    // 9 sample-rate(8k,11k,12k,16k,22k,24k,32k,44k,48k) * 1 band * 13 value
    APP_EQ_COEF	coef[APP_EQ_SR_NUM][APP_EQ_COEF_NUM];
} APP_BASS_CFG;

typedef struct
{
    // 9 sample-rate(8k,11k,12k,16k,22k,24k,32k,44k,48k) * 6 bands * 13 value
    APP_EQ_COEF	coef[APP_EQ_SR_NUM][APP_EQ_BAND_NUM][APP_EQ_COEF_NUM];
} APP_EQ_CFG;


typedef struct
{
    // 9 sample-rate(8k,11k,12k,16k,22k,24k,32k,44k,48k) * 6 bands
    APP_EQ_COEF	coef[APP_EQ_SR_NUM][APP_EQ_BAND_NUM];
} APP_SPK_CFG;
// H/W-SE related --------------------------
typedef struct
{
    UINT16	apf_coef;	// BIT[00:08]
    UINT8	sel_r;		// BIT[10]
    UINT8	sel_l;		// BIT[11]
    UINT8	seb_en;		// BIT[12]
    UINT8	psb_en;		// BIT[13]
    UINT8	apf_en;		// BIT[14]

    UINT16	depth1;		// BIT[16:24]
    UINT16	depth2;		// BIT[00:08]
} APP_SE_CFG;

// H/W-Beeper related ----------------------
typedef struct
{
    UINT16	freq;
    UINT8	vol;
    UINT8	mode;
    UINT8	mute;
    UINT8	duration;
} APP_BEEP_CFG;

// H/W-VOL related -------------------------
typedef enum
{
    APP_DVOL_ID_USR = 0,
    APP_DVOL_ID_ATV = 1,
    APP_DVOL_ID_AIO = 2,
    APP_DVOL_ID_NUM = 3
} APP_DVOL_ID;

typedef enum
{
    APP_DVOL_MODE_NORMAL        = 0,
    APP_DVOL_MODE_ZERO_CROSS    = 1,
    APP_DVOL_MODE_SOFT_RAMP     = 2,
    APP_DVOL_MODE_ZERO_AND_SOFT = 3
} APP_DVOL_MODE;

typedef enum
{
    APP_DVOL_ZCTO_1024x1,
    APP_DVOL_ZCTO_1024x2,
    APP_DVOL_ZCTO_1024x4,
    APP_DVOL_ZCTO_1024x8,
    APP_DVOL_ZCTO_1024x16,
    APP_DVOL_ZCTO_1024x32,
    APP_DVOL_ZCTO_1024x64,
    APP_DVOL_ZCTO_1024x128,
} APP_DVOL_ZERO_TIMEOUT;

typedef enum
{
    APP_DVOL_RAMP_STEP_1_DIV_8_DB,
    APP_DVOL_RAMP_STEP_2_DIV_8_DB,
    APP_DVOL_RAMP_STEP_4_DIV_8_DB,
    APP_DVOL_RAMP_STEP_8_DIV_8_DB,
} APP_DVOL_RAMP_STEP;

typedef enum
{
    APP_MUTE_ZCTO_1024x1,
    APP_MUTE_ZCTO_1024x2,
    APP_MUTE_ZCTO_1024x4,
    APP_MUTE_ZCTO_1024x8,
    APP_MUTE_ZCTO_1024x16,
    APP_MUTE_ZCTO_1024x32,
    APP_MUTE_ZCTO_1024x64,
    APP_MUTE_ZCTO_1024x128,
} APP_MUTE_ZERO_TIMEOUT;

typedef enum
{
    APP_MUTE_RAMP_STEP_1_DIV_8_DB,
    APP_MUTE_RAMP_STEP_2_DIV_8_DB,
    APP_MUTE_RAMP_STEP_4_DIV_8_DB,
    APP_MUTE_RAMP_STEP_8_DIV_8_DB,
} APP_MUTE_RAMP_STEP;

typedef struct
{
    UINT8	mode;
    UINT8	zero_thd;
    UINT8	zero_timeout;
    UINT8	ramp_step;
    UINT16	delay_time;
} APP_DVOL_CFG;

typedef struct
{
    UINT8	on_off;
    UINT8	zero_timeout;
    UINT8	ramp_step;
    UINT16	delay_time;
} APP_MUTE_CFG;


/*============ Prototype of Audio API Routines ================*/
//-------- audio_host.c -----------------------------------------
/*=============================================================*/
/**
 * Audio_Init_Step1
 * Init audio decoder module.
 *
 * @param <void>	{ void }
 * @return 			{ void }
 * @ingroup drv_adc
 */
void Audio_Init_Step1();
/*=============================================================*/
/**
 * Audio_Init_Step2
 * Init audio decoder module.
 *
 * @param <void>	{ void }
 * @return 			{ void }
 * @ingroup drv_adc
 */
void Audio_Init_Step2();

void Audio_AioSetPathSrc(AIO_PATH_CFG *p_path_cfg);
void Audio_AioSetPathClock(UINT8 src);


//-------- audio_hwp.c ------------------------------------------
/*=============================================================*/
/**
 * Init. H/W post-processing module
 * init global HWP setting and enable HWP block and interrupt
 *
 * @param <void>	{ void }
 * @return 			{ void }
 * @ingroup drv_adc
 */
void Audio_AppInit();

/**
 * Set L/R channel balance setting
 * if (balance > 0) means that L channel will be attenuated.
 * if (balance < 0) means that R channel will be attenuated.
 * attenuation level is 1 dB per step.
 *
 * @param <balance>		{ range from APP_MIN_BAL ~ APP_MAX_BAL }
 * @param <auto_mute>	{ when it is set, driver will mute if <balance> is MAX/MIN value }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_HwpSetBalanceLR(INT16  balance, BOOL auto_mute);

/**
 * Set L/R channel balance setting ( TimeShift(SCART) path )
 * if (balance > 0) means that L channel will be attenuated.
 * if (balance < 0) means that R channel will be attenuated.
 * attenuation level is 0.125 dB per step.
 *
 * @param <balance>	{ range from APP_MIN_BAL ~ APP_MAX_BAL }
 * @return			{ void }
 * @ingroup lib_audio
 */
void Audio_HwpSetBalanceTS(INT16  balance, BOOL auto_mute);

/**
 * Set Digital Volume
 *
 * @param <ch_id>	{ APP_CH_ID_L | APP_CH_ID_R | ... ,refer to audio_app.h }
 * @param <vol>		{ range from APP_MIN_VOL ~ APP_MAX_VOL }
 * @return			{ void }
 * @ingroup lib_audio
 */
void Audio_HwpSetVolume(UINT8 ch_id, INT16 vol);

/**
 * Set Digital Volume ( TimeShift(SCART) path )
 *
 * @param <ch_id>	{ APP_CH_ID_L | APP_CH_ID_R | ... ,refer to audio_app.h }
 * @param <vol>		{ range from APP_MIN_VOL ~ APP_MAX_VOL }
 * @return			{ void }
 * @ingroup lib_audio
 */
void Audio_HwpSetVolumeTS(UINT8 ch_id, INT16 vol);

/**
 * Set Digital Volume Compensation
 *
 * @param <ch_id>	{ APP_CH_ID_L | APP_CH_ID_R | ... ,refer to audio_app.h }
 * @param <vol>		{ range from APP_MIN_VOL ~ APP_MAX_VOL }
 * @return			{ void }
 * @ingroup lib_audio
 */
void Audio_HwpSetVolumeCompensation(UINT8 ch_id, INT16 vol, UINT8 comp_id);

/**
 * Set Digital Volume Compensation ( TimeShift(SCART) path )
 *
 * @param <ch_id>	{ APP_CH_ID_L | APP_CH_ID_R | ... ,refer to audio_app.h }
 * @param <vol>		{ range from APP_MIN_VOL ~ APP_MAX_VOL }
 * @param <comp_id>	{ caller id }
 * @return			{ void }
 * @ingroup lib_audio
 */
void Audio_HwpSetVolumeCompensationTS(UINT8 ch_id, INT16 vol, UINT8 comp_id);

/**
 * Mute/Unmute output
 *
 * @param <para>	{ AIO_ENABLE/AIO_DISABLE }
 * @param <ch_id>	{ APP_CH_ID_L | APP_CH_ID_R | ... ,refer to audio_app.h }
 * @param <mute_id>	{ mute_by_who id , now support id 0~4 }
 * @return			{ void }
 * @ingroup lib_audio
 */
void Audio_HwpSetMute(BOOL para, UINT8 ch_id, UINT8 mute_id);

/**
 * Mute/Unmute output ( TimeShift(SCART) path )
 *
 * @param <para>	{ AIO_ENABLE/AIO_DISABLE }
 * @param <ch_id>	{ APP_CH_ID_L | APP_CH_ID_R | ... ,refer to audio_app.h }
 * @param <mute_id>	{ mute_by_who id )
 * @return			{ void }
 * @ingroup lib_audio
 */
void Audio_HwpSetMuteTS(BOOL para, UINT8 ch_id, UINT8 mute_id);

/**
 * setup Digital Volume module
 *
 * @param <p_cfg_dvol>	{ pointer of DVOL control block }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_HwpCfgVolume(APP_DVOL_CFG *p_cfg_dvol);

/**
 * setup Digital Volume module ( TimeShift(SCART) path )
 *
 * @param <p_cfg_dvol>	{ pointer of DVOL control block }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_HwpCfgVolumeTS(APP_DVOL_CFG *p_cfg_dvol);

/**
 * setup Digital Mute module
 *
 * @param <p_cfg_dvol>	{ pointer of DVOL control block }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_HwpCfgMute(APP_MUTE_CFG *p_cfg_mute);

/**
 * setup Digital Mute module ( TimeShift(SCART) path)
 *
 * @param <p_cfg_dvol>	{ pointer of DVOL control block }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_HwpCfgMuteTS(APP_MUTE_CFG *p_cfg_mute);

/**
 * Mute/Unmute output
 * This API is for multi-thread purpose. Audio Library user should use Audio_HwpSetMute()
 * instead of this one.
 *
 * @param <para>	{ AIO_ENABLE/AIO_DISABLE }
 * @param <ch_id>	{ APP_CH_ID_L | APP_CH_ID_R | ... ,refer to audio_app.h }
 * @param <mute_id>	{ mute_by_who id )
 * @return			{ void }
 * @ingroup lib_audio
 */
void Audio_HwpSetMuteInternal(BOOL para, UINT8 ch_id, UINT8 mute_id);

/**
 * Mute/Unmute output ( TimeShift(SCART) path )
 * This API is for multi-thread purpose. Audio Library user should use Audio_HwpSetMute()
 * instead of this one.
 *
 * @param <para>	{ AIO_ENABLE/AIO_DISABLE }
 * @param <ch_id>	{ APP_CH_ID_L | APP_CH_ID_R | ... ,refer to audio_app.h }
 * @param <mute_id>	{ mute_by_who id )
 * @return			{ AIO_OK or AIO_ERROR_CODE }
 * @ingroup lib_audio
 */
void Audio_HwpSetMuteInternalTS(BOOL para, UINT8 ch_id, UINT8 mute_id);

/**
 * Setup H/W Auto Volume Control
 *
 * @param <para>		{ AIO_ENABLE/AIO_DISABLE }
 * @param <p_cfg_avc>	{ pointer of AVC control block }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_HwpSetAVC(BOOL para, APP_AVC_CFG *p_cfg_avc);

/**
 * Setup H/W Treble values
 *
 * @param <p_val_treble>	{ pointer of a set of treble value }
 * @return					{ void }
 * @ingroup lib_audio
 */
void Audio_HwpSetTrebleValue(BOOL para, APP_TREBLE_VAL *p_val_treble);

/**
 * Setup H/W Treble coefficients
 *
 * @param <para>			{ AIO_ENABLE/AIO_DISABLE }
 * @param <p_cfg_treble>	{ pointer of Treble-Band control block }
 * @return					{ void }
 * @note
 *    This function is mutual with Speaker Compensation.
 *
 * @ingroup lib_audio
 */
void Audio_HwpSetTrebleCoef(APP_TREBLE_CFG *p_cfg_treble);

/**
 * Setup H/W Bass values
 *
 * @param <para>		{ AIO_ENABLE/AIO_DISABLE }
 * @param <p_val_bass>	{ pointer of a set of bass value }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_HwpSetBassValue(BOOL para, APP_BASS_VAL *p_val_bass);

/**
 * Setup H/W Bass coefficients
 *
 * @param <p_cfg_bass>	{ pointer of Bass-Band control block }
 * @return				{ void }
 * @note
 *    This function is mutual with Speaker Compensation.
 *
 * @ingroup lib_audio
 */
void Audio_HwpSetBassCoef(APP_BASS_CFG *p_cfg_bass);

/**
 * Setup H/W Equalizer values
 *
 * @param <para>		{ AIO_ENABLE/AIO_DISABLE }
 * @param <p_val_eq>	{ pointer of a set of EQ value }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_HwpSetEQValue(BOOL para, APP_EQ_VAL *p_val_eq);

/**
 * Setup H/W Equalizer coefficients
 *
 * @param <p_cfg_eq>	{ pointer of EQ-Band control block }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_HwpSetEQCoef(APP_EQ_CFG *p_cfg_eq);

/**
 * Setup Speaker Compensation
 *
 * @param <para>		{ AIO_ENABLE/AIO_DISABLE }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_HwpSetSpeakerCompensation(BOOL para);

void Audio_HwpInputNoiseReduction(void);

/**
 * Setup H/W Spatial Effect
 *
 * @param <para>		{ AIO_ENABLE/AIO_DISABLE }
 * @param <p_cfg_avc>	{ pointer of SE control block }
 * @return				{ AIO_OK or AIO_ERROR_CODE }
 * @ingroup lib_audio
 */
void Audio_HwpSetSpatialEffect(BOOL para, APP_SE_CFG *p_cfg_se);

/**
 * Setup H/W Beeper
 *
 * @param <para>			{ _ENABLE/_DISABLE }
 * @param <p_cfg_beeper>	{ pointer of BEEPER control block }
 * @return					{ void }
 * @ingroup lib_audio
 */
void Audio_HwpSetBeeper(BOOL para, APP_BEEP_CFG *p_cfg_beeper);

//-------- audio_tv.c -------------------------------------------



/*=============================================================*/
/**
 * drvif_audio_tv_auto_scan
 *
 * @param <void>
 * @return <soundStd>
 * @ingroup drv_adc
 */
UINT8 drvif_audio_tv_auto_scan(void);

// Las Vegas ---
typedef enum
{
    ATV_DATA_SRC_IFD,
    ATV_DATA_SRC_SIF
} ATV_DATA_SRC;

typedef enum
{
    // identical to the order in register's field
    ATV_SOUND_STD_MN_MONO      = 0x00,
    ATV_SOUND_STD_BTSC         = 0x01,
    ATV_SOUND_STD_A2_M         = 0x02,
    ATV_SOUND_STD_EIAJ         = 0x03,
    ATV_SOUND_STD_BG_MONO      = 0x04,
    ATV_SOUND_STD_A2_BG        = 0x05,
    ATV_SOUND_STD_NICAM_BG     = 0x06,
    ATV_SOUND_STD_DK_MONO      = 0x07,
    ATV_SOUND_STD_A2_DK1       = 0x08,
    ATV_SOUND_STD_A2_DK2       = 0x09,
    ATV_SOUND_STD_A2_DK3       = 0x0A,
    ATV_SOUND_STD_NICAM_DK     = 0x0B,
    ATV_SOUND_STD_AM_MONO      = 0x0C,
    ATV_SOUND_STD_NICAM_L      = 0x0D,
    ATV_SOUND_STD_FM_MONO_NO_I = 0x0E,
    ATV_SOUND_STD_NICAM_I      = 0x0F,    
	ATV_SOUND_STD_PAL_N		   = 0x10,
    ATV_SOUND_STD_UNKNOWN      = 0x1F,
    // f/w only ---
    ATV_SOUND_STD_AUTO         = 0x7F
} ATV_SOUND_STD;

typedef enum
{
    ATV_SOUND_MODE_MONO       = 0,
    ATV_SOUND_MODE_STEREO     = 1,
    ATV_SOUND_MODE_DUAL       = 2,
    ATV_SOUND_MODE_SAP_MONO   = 3,
    ATV_SOUND_MODE_SAP_STEREO = 4,
    ATV_SOUND_MODE_AUTO       = 5
} ATV_SOUND_MODE;

typedef enum
{
    // identical to the order in register's field
    ATV_SOUND_MODE_NICAM_STEREO = 0,
    ATV_SOUND_MODE_NICAM_DUAL   = 2,
    ATV_SOUND_MODE_NICAM_MONO   = 4,
    ATV_SOUND_MODE_NICAM_DATA   = 6,
    // s/w only
    ATV_SOUND_MODE_NICAM_NOISY  = 7
} ATV_SOUND_MODE_NICAM;

typedef enum
{
    ATV_SOUND_SEL_FMAM      = 0,
    ATV_SOUND_SEL_STEREO_AB = 1,
    ATV_SOUND_SEL_STEREO_A  = 2,
    ATV_SOUND_SEL_STEREO_B  = 3
} ATV_SOUND_SEL;

typedef enum
{
    ATV_BTSC_MTS_SEL_MONO         = 0,
    ATV_BTSC_MTS_SEL_STEREO       = 1,
    ATV_BTSC_MTS_SEL_SAP          = 2,
    ATV_BTSC_MTS_SEL_L_MONO_R_SAP = 3
} ATV_BTSC_MTS_SEL;

typedef enum
{
    ATV_PLAY_CH_SEL_L        = 0,
    ATV_PLAY_CH_SEL_R        = 1,
    ATV_PLAY_CH_SEL_L_ADD_R  = 2,
    ATV_PLAY_CH_SEL_L_SUB_R  = 3
} ATV_PLAY_CH_SEL;

typedef enum
{
    ATV_MTS_PRIO_BTSC = 0,
    ATV_MTS_PRIO_EIAJ = 1,
    ATV_MTS_PRIO_MONO = 2,
    ATV_MTS_PRIO_DK   = 3,
    ATV_MTS_PRIO_L    = 4
} ATV_MTS_PRIORITY;

typedef enum
{
    // identical to the order in register's field
    ATV_MN_PRIO_BTSC		= 0,
    ATV_MN_PRIO_EIAJ		= 1,
    ATV_MN_PRIO_MONO1		= 2,
    ATV_MN_PRIO_MONO2		= 3,
} ATV_MN_PRIO;

typedef enum
{
    // identical to the order in register's field
    ATV_DKL_PRIO_DK		= 0,
    ATV_DKL_PRIO_L		= 1
} ATV_DKL_PRIO;

typedef struct
{
    unsigned	rsvd:15;
    unsigned	play_analog:1;
    unsigned	soung_select:2;
    unsigned	sound_std:6;
    unsigned	ana_soundmode:4;
    unsigned	dig_soundmode:4;
} ATV_SOUND_INFO;

typedef enum
{
    // bit-mapping
    ATV_SCAN_OPT_HW_AUTO    = (1<<0),
    ATV_SCAN_OPT_FRQ_OFFSET = (1<<1),
    ATV_SCAN_OPT_2ND_PHASE  = (1<<2),
    ATV_DEV_OPT_HIGH        = (1<<3),
    ATV_DEV_OPT_ULTRA1      = (1<<4),
    ATV_DEV_OPT_AUTO        = (1<<5)
} ATV_MSD_OPT;

typedef struct
{
    int	sound_std;
    int	hdev_mode;
    int	scan_mode;		//[0,1,2] = [normal,frq_offset,2nd_phase]
    int	scan_coef;
} ATV_SCAN_STD;


/**
 * Init. Spaarrow ATV Audio
 *
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AtvInit();

/**
 * Select ATV Sound Standard
 *
 * @param <sound_std>	{ ATV_SOUND_STD_XXX , refer to audio_atv.h }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_AtvSetSoundStd(UINT8 sound_std);

/**
 * Select ATV play channel
 * this function only affect the data into post-processing,
 * it cannot control the ATV audio decoder.
 *
 * @param <sound_sel>	{ ATV_PLAY_CH_SEL_XXX , refer to audio_atv.h }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_AtvSetPlayChannel(UINT8 l_sel, UINT8 r_sel);

/**
 * Select ATV Sound/Language
 *
 * @param <sound_sel>	{ ATV_SOUND_SEL_XXX , refer to audio_atv.h }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_AtvSetSoundSelect(UINT8 sound_sel);

/**
 * Select ATV BTSC Sound/Language
 *
 * @param <mts_sel>	{ ATV_BTSC_MTS_SEL_XXX , refer to audio_atv.h }
 * @return			{ void }
 * @ingroup lib_audio
 */
void Audio_AtvSetBtscMtsSelect(UINT8 mts_sel);

/**
 * Get ATV BTSC Sound/Language Select Status
 *
 * @param <p_mts_sel>	{ ATV_BTSC_MTS_SEL_XXX , refer to audio_atv.h }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_AtvGetBtscMtsSelect(UINT8 *p_mts_sel);

/**
 * Select ATV Sound Mode
 *
 * @param <sound_mode>	{ ATV_SOUND_MODE_XXX , refer to audio_atv.h }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_AtvSetSoundMode(UINT8 sound_mode);

/**
 * Get ATV Sound Mode
 *
 * @param <p_sound_mode>	{ ATV_SOUND_MODE_XXX , refer to audio_atv.h }
 * @return					{ void }
 * @ingroup lib_audio
 */
void Audio_AtvGetSoundMode(UINT8 *p_sound_mode);

/**
 * Select ATV MTS Priority
 *
 * @param <mts_prio>	{ ATV_MTS_PRIO_XXX , refer to audio_atv.h }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_AtvSetMtsPriority(UINT8 mts_prio);

/**
 * Query current ATV Sound Standard
 *
 * @param <p_sound_std>	{ current Sound Standard }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_AtvGetSoundStd(ATV_SOUND_INFO *p_sound_info);

/**
 * Scan/Find current ATV Sound Standard
 *
 * @param <p_sound_std>	{ result of scanning }
 * @prarm <flag>		{ refer to ATV_MSD_OPT in audio_atv.h }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_AtvScanSoundStd(UINT8 *p_sound_std, UINT16 flag);

/**
 * select IF-ADC data source
 *
 * @param 	<para>	{ ATV_DATA_SRC_IFD/ATV_DATA_SRC_SIF }
 * @return			{ void }
 * @ingroup drv_aio
 */
void Audio_AtvSetDataSrc(UINT8 para);

/**
 * get M/N standard status
 *
 * @param <p_status>	{ result of M/N standard status}
 * @param <p_energy>	{ result of M/N standard energy}
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_AtvGetMNStatus(UINT8 *p_status, UINT16 *p_energy);

/**
 * Enable/Disable F/W Scanning
 * This function can suspend/resume F/W scanning when FREQ_SHIFT is enabled.
 * When calling Audio_AtvSetSoundStd, F/W Scaning is enabled automatically.
 *
 * @param 	<para>	{ _ENABLE/_DISABLE }
 * @return			{ void }
 * @ingroup drv_aio
 */
void Audio_AtvSetFwScanCarrierFreqOffset(UINT8 para);

/**
 * ATV TimerEvent : F/W use this event to scan freq. offset of ATV sound carrier.
 *
 * @ingroup lib_audio
 */
bit Audio_AtvTimerEventFwScan(void);

/**
 * ATV TimerEvent : F/W use this event to enable NICAM interrupt
 *
 * @ingroup lib_audio
 */
void Audio_AtvTimerEventNicamCheck(void);

void drv_audio_PowerOn(bit on_off);

#endif // #ifndef _ADC_API_H


