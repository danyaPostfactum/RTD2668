/*=============================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2007 *
 * All rights reserved.                                       *
 *============================================================*/
/*======================= Description ========================*/
/**
 * @file  audio_hwp.h
 *
 * @author
 * @date
 * @version 0.1
 * @ingroup drv_adc
 *
 *=============================================================*/
#ifndef	_AUDIO_HWP_H_
#define	_AUDIO_HWP_H_

enum { AUD_SYS_MUTE= 0x00, AUD_HDMI_MUTE = 0x01, AUD_DRV_MUTE = 0x02, AUD_BALANCE_MUTE=0x03, AUD_VOL_MUTE=0x04 };

#define AD_FINE_VOL_LP_A1	(0x03FFF)
#define AD_FINE_VOL_BP_A1	(0x00000)
#define AD_FINE_VOL_BP_A2	(0x0C000)
#define AD_FINE_VOL_TAB_OFFSET	(40)
#define AD_FINE_VOL_STEP_PER_DB	(32)

#define	AD_VOL_P_20DB		(20)
#define	AD_VOL_P_5DB		(5)
#define	AD_VOL_P_10DB		(10)
#define AD_VOL_P_0DB		0
#define	AD_VOL_M_20DB		(-20)
#define	AD_VOL_M_40DB		(-40)

typedef struct _AD_BAND_COEFF
{
    INT16 a1;
    INT16 a2;
    INT16 h0;
} AD_BAND_COEFF;

typedef struct _AD_BASS_COEFF
{
    INT16 a1;
    INT16 h0;
} AD_BASS_COEFF;

typedef struct _AD_TREPLE_COEFF
{
    INT16 a1;
    INT16 a2;
    INT16 h0;
} AD_TREPLE_COEFF;

typedef	struct _EQ_TARGET_COEF
{
    INT16 LP0_A1;
    INT16 BP1_A1;
    INT16 BP2_A1;
    INT16 BP3_A1;
    INT16 BP4_A1;
    INT16 BP5_A1;
//	INT16 HP0_A1; // hign pass can not boost

//	INT16 LP0_A2;
    INT16 BP1_A2;
    INT16 BP2_A2;
    INT16 BP3_A2;
    INT16 BP4_A2;
    INT16 BP5_A2;

    INT16 LP0_H0;
    INT16 BP1_H0;
    INT16 BP2_H0;
    INT16 BP3_H0;
    INT16 BP4_H0;
    INT16 BP5_H0;
} EQ_TARGET_COEF;

typedef	struct _LOUDNESS_TARGET_COEF
{
    INT16 LP0_A1;
    INT16 BP1_A1;
    INT16 BP2_A1;

    INT16 BP1_A2;
    INT16 BP2_A2;

    INT16 LP0_H0;
    INT16 BP1_H0;
    INT16 BP2_H0;
} LOUDNESS_TARGET_COEF;

typedef enum
{
    LP0 = 0,
    BP1,
    BP2,
    BP3,
    BP4,
    BP5,
} EQ_BAND_ID;

typedef enum _AUD_CHANNEL_ID
{
    AUD_CHAN_L = 0x01,
    AUD_CHAN_R = 0x02,
    AUD_CHAN_LS = 0x04,
    AUD_CHAN_RS = 0x08,
    AUD_CHAN_C = 0x10,
    AUD_CHAN_SW = 0x20,
    AUD_CHAN_ALL = 0x3F,
} AUD_CHANNEL_ID;

typedef enum
{
    AUD_BTSC_VOL_COMP_DISABLE = 0,
    AUD_BTSC_VOL_COMP_STEREO,
    AUD_BTSC_VOL_COMP_SAP,
} AUD_BTSC_VOL_COMP;

// Las Vegas ---
#define APP_MUTE_NUM		5

#define APP_VOL_STEP_1DB	8					// 0.125dB per step
#define APP_MAX_VOL	((47.875)*APP_VOL_STEP_1DB)	// (+47.875dB)
#define APP_MIN_VOL (-72*APP_VOL_STEP_1DB)		// (-72dB)
#define APP_MAX_BAL	(50*APP_VOL_STEP_1DB)
#define APP_MIN_BAL (-50*APP_VOL_STEP_1DB)

#define APP_EQ_COEF_NUM		13
#define APP_EQ_SR_NUM		9	// order is identical to struct of AIO_FS_ID in audio_drv.h

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

// H/W-VOL related -------------------------
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


/*============ Prototype of Driver Routines ===================*/
/*=============================================================*/
/**
 * drv_audio_init_hwprocess
 * init global HWP setting and enable HWP block and interrupt
 *
 * @param <void>	{ void }
 * @return 			{ void }
 * @ingroup drv_adc
 */
void drv_audio_init_hwprocess();

/*=============================================================*/
/**
 * drv_audio_set_mute
 * mute the audio output
 *
 * @param <ctrl>		{ mute/unmute }
 * @param <ch_idx>		{ CH_L | CH_R | CH_LS | CH_RS | CH_C | CH_SW }
 * @param <mute_type>	{ system/driver mute flag }
 * @return 				{ void }
 * @ingroup drv_adc
 */
void drv_audio_set_mute(UINT32 ctrl,UINT32 ch_idx, UINT32 mute_type, UINT16 delay_ctrl);

/*=============================================================*/
/**
 * drv_audio_eq_overflow_int_handler
 * 1.disable EQ H/W block
 * 2.clear EQ overflow flag
 * 3.restore original EQ on/off setting
 *
 * @param <void>	{ void }
 * @return 			{ void }
 * @ingroup drv_adc
 */
void drv_audio_eq_overflow_int_handler();

UINT32 drv_audio_write_dbuf_register(UINT32 addr, UINT32 val);

/*=============================================================*/
/**
 * drv_audio_set_fixedvolout
 * enable/disable fixed volume output(0dB on C/SW channel)
 * on SPDIF_OUT and Mono DAC.
 *
 * @param <setting>	{ enable/disable }
 * @return 			{ void }
 * @ingroup drv_adc
 *
 * @note this function will conflict with Muti-Channel I2S output application
 */
void drv_audio_set_fixedvolout(UINT32 setting);
/*=============================================================*/
/**
 * drv_audio_set_btsc_vol_compensation
 *
 * @param <setting>	{ AUD_BTSC_VOL_COMP}
 * @return 			{ void }
 * @ingroup drv_adc
 *
 */
void drv_audio_set_btsc_vol_compensation(AUD_BTSC_VOL_COMP setting);
/*=============================================================*/
/**
 * drv_audio_set_autovolumecontrol
 * to set auto volume control
 *
 * @param <isEnable>	{ disable/enable }
 * @return				{ void }
 * @ingroup drv_adc
 */
void drv_audio_set_autovolumecontrol(UINT32 isEnable, UINT32 avc_level);
/*=============================================================*/
/**
 * drv_audio_get_avc_status
 *
 * @param <void>	{ void}
 * @return 			{ flag }
 * @ingroup drv_adc
 *
 */
UINT8 drv_audio_get_avc_status(void);
#endif // _AUDIO_HWP_H_

