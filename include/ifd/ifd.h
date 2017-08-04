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
#ifndef _IFD_H
#define _IFD_H

//include file
#ifdef CONFIG_PLATFORM_PACIFIC //pacific
#include "if_demod/ifd_factorymode.h"
#include "if_demod/ifd_pacific.h"
#elif defined(CONFIG_ATLANTIC)
#include "libs/frontend/ifd_atlantic.h"
#elif defined(CONFIG_LAS_VEGAS)
#include "libs/frontend/ifd_atlantic.h"
#else //sparrow
#include "ifd\ifd_sparrow.h"
#endif



//define parameter
#ifdef CONFIG_PLATFORM_PACIFIC
#define   AGC_VAR_TIMER                                    20
#else
#define   AGC_VAR_TIMER                                    40
#endif
#ifdef CONFIG_PLATFORM_PACIFIC
#define	IFD_REG_LOG_TIMER					100
#endif
#define   AFC_FREQ_STEP                                    5000

#define IFD_ENABLE  (1)
#define IFD_DISABLE (0)
#define CR_BPF_TABLE_NUM (5)
#define CR_BPF_TABLE_SIZE (41)
#define EQ_TABLE_SIZE (8)
#define PGA_TABLE_SIZE (132)
#define IFD_FLASH_MAGIC_CODE					0x69666421

#define	IFD_AGC_KRF						0x400UL
#define	IFD_CR_OFFSET_SAT					0x0000A000UL
#define	IFD_CR_PHASE_BAD_THD				0x7FUL
#define	IFD_CLAMP_MAX_THD				0x1fffUL
#define	IFD_CLAMP_MIN_THD				0x3500UL
#define	IFD_CLAMP_MID_THD_NEG			0x3a00UL
#define	IFD_CLAMP_MID_THD_POS			0x0100UL
#define	IFD_UNLOCK_MAX_DEC_CNT			0x20UL
#define	IFD_LOCK_MAX_DEC_CNT				0x20UL
#define	IFD_UNLOCK_MAX2_DEC_CNT			0x10UL
#define	IFD_LOCK_MAX2_DEC_CNT			0x10UL
#define	IFD_PEAK_DEC_CNT					0xdb0UL
#define	IFD_PEAK2_DEC_CNT					0xdb0UL
#define	IFD_RF_AGC_PORT_INV				IFD_DISABLE // IFD_ENABLE
#define	IFD_IF_AGC_PORT_INV				IFD_DISABLE // IFD_ENABLE
#define	IFD_RF_AGC_MAX					0x1fffUL
#define	IFD_RF_AGC_MIN					0x0UL
#define	IFD_IF_AGC_MAX					0x1fffUL
#define	IFD_IF_AGC_MIN					0x0UL
#define	IFD_LOOP1_AGC_LOCK_IN_THD		0x10UL
#define	IFD_LOOP1_AGC_LOCK_OUT_THD		0x18UL
#define	IFD_LOOP2_AGC_LOCK_IN_THD		0x10UL
#define	IFD_LOOP2_AGC_LOCK_OUT_THD		0x18UL
#define	IFD_AGC_2STEP_DGAIN_LOW_THD	0x7f00UL
#define	IFD_AGC_2STEP_DGAIN_HIGH_THD	0xfff0UL
#define	IFD_AGC_2STEP_PGA_STEP              	0x04UL
#define	IFD_VSMODE_EN						IFD_DISABLE // IFD_ENABLE
#define	SECAMLA_FREQ_SHIFT				58000UL
#define	IFD_VSmode1_Hstart_NTSC			0xb5UL
#define	IFD_VSmode1_Hend_NTSC			0xc5UL
#define	IFD_VSmode1_Vstart_NTSC			0x8080UL
#define	IFD_VSmode1_Vend_NTSC			0x0993UL
#define	IFD_VSmode2_Hstart_NTSC			0xb8UL  //irene ok// irene has to check why w/o vsync_start2/hsync_start2
#define	IFD_VSmode2_Hend_NTSC			0xc8UL  //irene ok// irene has to check why w/o vsync_start2/hsync_start2
#define	IFD_VSmode2_Vstart_NTSC			0x0200UL
#define	IFD_VSmode2_Vend_NTSC			0x0400UL
#define	IFD_VSmode1_Hstart_PAL			0xb5UL
#define	IFD_VSmode1_Hend_PAL				0xc5UL
#define	IFD_VSmode1_Vstart_PAL			0x8080UL
#define	IFD_VSmode1_Vend_PAL				0x0993UL
#define	IFD_VSmode2_Hstart_PAL			0xb8UL  //irene ok// irene has to check why w/o vsync_start2/hsync_start2
#define	IFD_VSmode2_Hend_PAL				0xc8UL  //irene ok// irene has to check why w/o vsync_start2/hsync_start2
#define	IFD_VSmode2_Vstart_PAL			0x0200UL
#define	IFD_VSmode2_Vend_PAL				0x0400UL
#define	IFD_VSmode1_Hstart_SECAM			0xb8UL  //modify by wjchen, after check dma debug mode, 20091218
#define	IFD_VSmode1_Hend_SECAM			0xc8UL  //modify by wjchen, after check dma debug mode, 20091218
#define	IFD_VSmode1_Vstart_SECAM			0x8000UL
#define	IFD_VSmode1_Vend_SECAM			0x0a00UL
#define	IFD_VSmode2_Hstart_SECAM			0xb8UL  //modify by wjchen, after check dma debug mode, 20091218
#define	IFD_VSmode2_Hend_SECAM			0xcaUL  //modify by wjchen, after check dma debug mode, 20091218
#define	IFD_VSmode2_Vstart_SECAM			0x8000UL
#define	IFD_VSmode2_Vend_SECAM			0x0a00UL

/*extern UINT16  group_delay_table1[];
extern UINT16  group_delay_table2[];
extern UINT16  group_delay_table3[];
extern UINT16  group_delay_table4[];
extern UINT16  group_delay_table5[];
*/
typedef enum _TUNER_AGCTYPE_SEL
{
    TUNER_NONE_IC_RFAGC = 0,
    TUNER_RFAGC_IC_IFAGC,
    TUNER_RFAGC_IFAGC_IC_NONE,
    TUNER_NONE_IC_RFAGC_IFAGC,
} TUNER_AGCTYPE_SEL;

typedef enum _TUNER_TYPE_SEL
{
    TUNER_NORMAL = 0,
    TUNER_LOW_IF,
    TUNER_ZERO_IF,
} TUNER_TYPE_SEL;

typedef enum
{
    IFD_MODE_CH_SCAN=0,
    IFD_MODE_CH_SET,
} IFD_MODE_OPERATE;

typedef enum
{
    IFD_CR_STATE_SET_TUNER=0,
    IFD_CR_STATE_CHECK,
    IFD_CR_FLOW_STATE_ENABLE_BY_FACTORYMODE,
    IFD_CR_FLOW_STATE_DISABLE_BY_FACTORYMODE,
} IFD_CR_FLOW_STATE;

typedef enum _IFD_CR_LOOP_CR_LOOP_STATE
{
    IFD_CR_LOOP_FAST,
    IFD_CR_LOOP_SLOW,
} IFD_CR_LOOP_STATE;

typedef enum
{
    IFD_CR_WEIGHT_CR_LOCK_ENABLE = 0 ,
    IFD_CR_WEIGHT_CR_LOCK_DISABLE,
} IFD_CR_WEIGHT_CR_LOCK;

typedef enum
{
    IFD_CR_WEIGHT_MAX_DECREASE_1CYCLE = 0 ,
    IFD_CR_WEIGHT_MAX_DECREASE_4CYCLE,
    IFD_CR_WEIGHT_MAX_DECREASE_8CYCLE,
    IFD_CR_WEIGHT_MAX_DECREASE_16CYCLE,
    IFD_CR_WEIGHT_MAX_DECREASE_32CYCLE,
    IFD_CR_WEIGHT_MAX_DECREASE_64CYCLE,
    IFD_CR_WEIGHT_MAX_DECREASE_128CYCLE,
    IFD_CR_WEIGHT_MAX_DECREASE_256CYCLE,
    IFD_CR_WEIGHT_MAX_DECREASE_512CYCLE,
    IFD_CR_WEIGHT_MAX_DECREASE_1024CYCLE,
} IFD_CR_WEIGHT_MAX_DECREASE;

typedef enum
{
    IFD_CR_WEIGHT_MAX_SHIFT_RIGHT_0BIT = 0 ,
    IFD_CR_WEIGHT_MAX_SHIFT_RIGHT_1BIT,
    IFD_CR_WEIGHT_MAX_SHIFT_RIGHT_2BIT,
    IFD_CR_WEIGHT_MAX_SHIFT_RIGHT_3BIT,
    IFD_CR_WEIGHT_MAX_SHIFT_RIGHT_4BIT,
    IFD_CR_WEIGHT_MAX_SHIFT_RIGHT_5BIT,
    IFD_CR_WEIGHT_MAX_SHIFT_RIGHT_6BIT,
    IFD_CR_WEIGHT_MAX_SHIFT_RIGHT_7BIT,
    IFD_CR_WEIGHT_MAX_SHIFT_RIGHT_8BIT,
    IFD_CR_WEIGHT_MAX_SHIFT_RIGHT_9BIT,
    IFD_CR_WEIGHT_MAX_SHIFT_RIGHT_10BIT,
    IFD_CR_WEIGHT_MAX_SHIFT_RIGHT_11BIT,
    IFD_CR_WEIGHT_MAX_SHIFT_RIGHT_12BIT,
    IFD_CR_WEIGHT_MAX_SHIFT_RIGHT_13BIT,
    IFD_CR_WEIGHT_MAX_SHIFT_RIGHT_14BIT,
    IFD_CR_WEIGHT_MAX_SHIFT_RIGHT_15BIT,
} IFD_CR_WEIGHT_SHIFT;

typedef enum
{
    IFD_VIDEOPATH_OUTPUT_GAIN_1 = 0 ,
    IFD_VIDEOPATH_OUTPUT_GAIN_2 = 1 ,
    IFD_VIDEOPATH_OUTPUT_GAIN_4 = 2 ,
} IFD_VIDEOPATH_OUTPUT_GAIN;

typedef enum
{
    IFD_SHAPING_BW_SEL_BYPASS = 0 ,
    IFD_SHAPING_BW_SEL_0P86M,
    IFD_SHAPING_BW_SEL_1P18M,
    IFD_SHAPING_BW_SEL_1P25M,
    IFD_SHAPING_BW_SEL_1P36M,
    IFD_SHAPING_BW_SEL_1P50M,
    IFD_SHAPING_BW_SEL_2P18M,
    IFD_SHAPING_BW_SEL_4P07M,
    IFD_SHAPING_BW_SEL_5P45M,
    IFD_SHAPING_BW_SEL_7P67M,
    IFD_SHAPING_BW_SEL_9P04M,
    IFD_SHAPING_BW_SEL_10P36M,
    IFD_SHAPING_BW_SEL_11P91M,
} IFD_SHAPING_BW_SEL;

typedef enum
{
    IFD_SHAPING_IMG_NEG = 0 ,
    IFD_SHAPING_IMG_POS,
} IFD_SHAPING_IMG_SEL;

typedef enum
{
    IFD_MODULATION_SEL_POS = 0 ,
    IFD_MODULATION_SEL_NEG,
} IFD_MODULATION_SEL;

typedef enum
{
    IFD_VIDEO_BW_SEL_BYPASS = 0x0,
    IFD_VIDEO_BW_SEL_3P97M = 0x1,
    IFD_VIDEO_BW_SEL_4P04M = 0x2,
    IFD_VIDEO_BW_SEL_4P1M = 0x3 ,
    IFD_VIDEO_BW_SEL_4P14M = 0x4,
    IFD_VIDEO_BW_SEL_4P18M = 0x5,
    IFD_VIDEO_BW_SEL_5P1M = 0x6 ,
    IFD_VIDEO_BW_SEL_5P14M = 0x7,
    IFD_VIDEO_BW_SEL_5P15M = 0x8,
    IFD_VIDEO_BW_SEL_5P16M = 0x9,
    IFD_VIDEO_BW_SEL_5P17M = 0xa,
    IFD_VIDEO_BW_SEL_5P32M = 0xb,
} IFD_VIDEO_BW_SEL;

typedef enum
{
    SOUND_NOTCH_SEL2_BYPASS   ,
    SOUND_NOTCH_SEL2_4P72M135K,
    SOUND_NOTCH_SEL2_4P72M240K,
    SOUND_NOTCH_SEL2_4P72M322K,
    SOUND_NOTCH_SEL2_5P74M174K,
    SOUND_NOTCH_SEL2_5P74M291K,
    SOUND_NOTCH_SEL2_5P74M417K,
    SOUND_NOTCH_SEL2_6P25M195K,
    SOUND_NOTCH_SEL2_6P25M310K,
    SOUND_NOTCH_SEL2_6P25M505K,
    SOUND_NOTCH_SEL2_6P74M225K,
    SOUND_NOTCH_SEL2_6P74M288K,
    SOUND_NOTCH_SEL2_6P74M441K,
} IFD_SOUND_NOTCH_SEL2;

typedef enum
{
    SOUND_NOTCH_SEL_BYPASS = 0  ,
    SOUND_NOTCH_SEL_4P5M200k,
    SOUND_NOTCH_SEL_4P5M300k,
    SOUND_NOTCH_SEL_4P5M400k,
    SOUND_NOTCH_SEL_5P5M162k,
    SOUND_NOTCH_SEL_5P5M262k,
    SOUND_NOTCH_SEL_5P5M392k,
    SOUND_NOTCH_SEL_6P0M176k,
    SOUND_NOTCH_SEL_6P0M330k,
    SOUND_NOTCH_SEL_6P0M450k,
    SOUND_NOTCH_SEL_6P5M215k,
    SOUND_NOTCH_SEL_6P5M323k,
    SOUND_NOTCH_SEL_6P5M391k,
} IFD_SOUND_NOTCH_SEL;

typedef enum
{
    IFD_UPSAMPLE_LP_SEL_BYPASS = 0  ,
    IFD_UPSAMPLE_LP_SEL_NTSC,
    IFD_UPSAMPLE_LP_SEL_NTSC2,
    IFD_UPSAMPLE_LP_SEL_PAL,
} IFD_UPSAMPLE_LP_SEL;

typedef enum
{
    IFD_CLAMP_RESET_NEG = 0  ,
    IFD_CLAMP_RESET_POS,
} IFD_CLAMP_RESET_POLARITY;

typedef enum
{
    IFD_CLAMP_OFFSET_NORMAL = 0  ,
    IFD_CLAMP_OFFSET_HOLD,
} IFD_CLAMP_OFFSET;

typedef enum
{
    IFD_CLAMP_SCALE_1D64 = 0  ,
    IFD_CLAMP_SCALE_1D32,
    IFD_CLAMP_SCALE_1D16,
    IFD_CLAMP_SCALE_1D8,
    IFD_CLAMP_SCALE_1D4,
    IFD_CLAMP_SCALE_1D2,
    IFD_CLAMP_SCALE_1,
    IFD_CLAMP_SCALE_2,
    IFD_CLAMP_SCALE_4,
} IFD_CLAMP_SCALE;

typedef enum
{
    IFD_AUDIO_BP_SEL_BYPASS = 0            ,
    IFD_AUDIO_BP_SEL_4P300M_4P700M =   1,
    IFD_AUDIO_BP_SEL_3P960M_5P040M =   2,
    IFD_AUDIO_BP_SEL_4P100M_4P900M =   3,
    IFD_AUDIO_BP_SEL_5P300M_6P700M =   4,
    IFD_AUDIO_BP_SEL_5P250M_6P750M =   5,
    IFD_AUDIO_BP_SEL_5P200M_6P800M =   6,
    IFD_AUDIO_BP_SEL_6P300M_6P700M =   7,
    IFD_AUDIO_BP_SEL_6P200M_6P800M =   8,
    IFD_AUDIO_BP_SEL_6P100M_6P900M =   9,
    IFD_AUDIO_BP_SEL_4P090M_4P780M =  10,
    IFD_AUDIO_BP_SEL_4P180M_4P780M =  11,
    IFD_AUDIO_BP_SEL_4P210M_4P750M =  12,
    IFD_AUDIO_BP_SEL_5P115M_7P136M =  16,	//0x10
    IFD_AUDIO_BP_SEL_5P120M_7P132M =  17,	//0x11
    IFD_AUDIO_BP_SEL_5P124M_7P127M =  18,	//0x12
    IFD_AUDIO_BP_SEL_5P132M_7P119M =  19,	//0x13
    IFD_AUDIO_BP_SEL_5P170M_7P032M =  20,	//0x14
    IFD_AUDIO_BP_SEL_5P174M_7P027M =  21,	//0x15
    IFD_AUDIO_BP_SEL_5P182M_7P020M =  22,	//0x16
    IFD_AUDIO_BP_SEL_4P000M_5P000M =  23,	//0x17
    IFD_AUDIO_BP_SEL_3P800M_5P200M =  24,	//0x18
} IFD_AUDIO_BP_SEL;

typedef enum
{
    IFD_AUDIO_UPSAMPLE_LP_SEL_BYPASS = 0  ,
    IFD_AUDIO_UPSAMPLE_LP_SEL_PAL = 2,
} IFD_AUDIO_UPSAMPLE_LP_SEL;

typedef enum
{
    IFD_ADC_INPUT_SIFPGA = 0  ,
    IFD_ADC_INPUT_IFPGA,
    IFD_ADC_INPUT_BYPASS_PGA,
    IFD_ADC_INPUT_HZ,
} IFD_ADC_INPUT;

typedef enum
{
    IFD_CLK_RATE_108M = 0  ,
    IFD_CLK_RATE_54M,
    IFD_CLK_RATE_24P576M,
} IFD_CLK_RATE;

typedef enum
{
    IFD_AGC_MODE_ORIGINAL = 0  ,
    IFD_AGC_MODE_2LOOP,
    IFD_AGC_MODE_2STEP,
} IFD_AGC_MODE;


typedef enum
{
    AUTO,
    FREQ_184P25MHZ,
    FREQ_264P25MHZ,
    FREQ_487P25MHZ,
    FREQ_503P25MHZ,
    FREQ_807P25MHZ,
} DEFAULT_CHANNEL;

typedef enum
{
    IFD_AGC_SINGLE_NOIFAMP_PGA = 0  ,
    IFD_AGC_SINGLE_IFAMP_NOPGA,
    IFD_AGC_DOUBLE_NOIFAMP_PGA,
    IFD_AGC_DOUBLE_IFAMP_NOPGA,
    IFD_AGC_2STEP_NOIFAMP_PGA,
    IFD_AGC_2STEP_IFAMP_NOPGA,
} IFD_MODE_AGC_PGA_CONFIG;

extern UINT8 IFD_XDATA g_afc_stage_count;
extern UINT32 IFD_XDATA g_afc_curr_freq;
extern UINT32 IFD_XDATA g_afc_center_freq;
extern UINT8 IFD_XDATA g_afc_direction;
extern UINT8 IFD_XDATA g_curr_std;
extern UINT8 IFD_XDATA g_afc_hvunlock_debounce;

#ifndef CONFIG_TUNER_SILICON
#define AFC_HV_UNLOCK_DEBOUNCE_COUNT 25
#else
#define AFC_HV_UNLOCK_DEBOUNCE_COUNT 3
#endif

#ifdef _VIDEO_TV_SUPPORT
//=================================================================
//IFD driver
//=================================================================
//#ifdef CONFIG_PLATFORM_PACIFIC
//=================================================================
/**
 * drvif_tuner_is_locked
 *
 *
 * @param <afc>
 * @return <locked>
 */
UINT8 drvif_tuner_is_locked(UINT8 *afc);
//#endif
//=================================================================
/**
 * drvif_tuner_init
 *
 *
 * @param <mode>
 * @return { void }
 */
void drvif_tuner_init(UINT8 mode);
//=================================================================
/**
 * drvif_tuner_get_afcoffset
 *
 *
 * @param <void>
 * @return <offset>
 */
INT32 drvif_tuner_get_afcoffset(UINT8 afc);
#endif
//=================================================================
/**
 * drvif_ifd_system_init
 *
 *
 * @param <void>
 * @return { void }
 */
void drvif_ifd_system_init(void);
//=================================================================
/**
 * drvif_ifd_init
 *
 *
 * @param <void>
 * @return { void }
 */
void drvif_ifd_init(void);
//=================================================================
/**
 * drvif_ifd_power_on
 *
 *
 * @param <enable>
 * @return { void }
 */
void drvif_ifd_power_on(bool enable);
//=================================================================
/**
 * drvif_ifd_set_iftuner_setting
 *
 *
 * @param <mode><freq><usnum>
 * @return {void}
 */
void drvif_ifd_set_iftuner_setting(UINT8 mode, UINT32 freq,UINT16 usNum);
//=================================================================
/**
 * drvif_ifd_set_video_standard
 *
 *
 * @param <video_mode>
 * @return { void }
 */
#if 1//ndef CONFIG_TUNER_SILICON
void drvif_ifd_set_video_standard(UINT8 video_mode, UINT32 freq);
#endif
//=================================================================
/**
 * drvif_ifd_set_standard
 *
 *
 * @param <video_mode>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drvif_iftuner_finetune_setting(UINT32 freq);
#endif
//=================================================================
/**
 * drvif_ifd_scan_start_setting
 *
 *
 * @param <video_mode>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drvif_ifd_scan_start_setting(void);
#endif
//=================================================================
/**
 * drvif_ifd_get_agc_lock
 *
 *
 * @param {void}
 * @return <lock>
 */
bool drvif_ifd_get_agc_lock();
//=================================================================
/**
 * drvif_ifd_get_cr_lock
 *
 *
 * @param {void}
 * @return <lock>
 */
bool drvif_ifd_get_cr_lock();
//=================================================================
/**
 * drvif_ifd_set_audio_bp
 *
 *
 * @param <mode>
 * @return <newset>
 */
UINT32 drvif_ifd_set_audio_bp(UINT8 mode);
//=================================================================
/**
 * drvif_ifd_clamp_reset
 *
 *
 * @param {mode}
 * @return {void}
 */
void drvif_ifd_clamp_reset(IFD_MODE_OPERATE mode);
//=================================================================
/**
 * drvif_ifd_cr_freq_status_check
 *
 *
 * @param <status>
 * @return {void}
 */
void drvif_ifd_cr_freq_status_check(IFD_CR_FLOW_STATE status);
//=================================================================
/**
 * drvif_ifd_softreset
 *
 *
 * @param {void}
 * @return {void}
 */
void drvif_ifd_softreset(void);
//=================================================================
/**
 * drvif_ifd_set_agc_variance_off
 *
 *
 * @param <agc_speed>
 * @return {void}
 */
void drvif_ifd_set_agc_variance_off(UINT8 agc_speed);
//=================================================================
/**
 * drvif_ifd_set_agc_variance_on
 *
 *
 * @param {void}
 * @return {void}
 */
void drvif_ifd_set_agc_variance_on(void);
//=================================================================
/**
 * drvif_ifd_agc_peak_en
 *
 *
 * @param <enable>
 * @return {void}
 */
void drvif_ifd_agc_peak_en(bool enable);
//=================================================================
/**
 * drvif_ifd_vtop_change_with_tunerband
 *
 *
 * @param {void}
 * @return {void}
 */
#ifndef CONFIG_TUNER_SILICON
void drvif_ifd_vtop_change_with_tunerband();
#endif
//=================================================================
/**
 * drvif_ifd_cr_freq_offset_in_100hz_get_info
 *
 *
 * @param {void}
 * @return {shift_freq}
 */
INT32 drvif_ifd_cr_freq_offset_in_100hz_get_info();
#ifdef CONFIG_PLATFORM_PACIFIC
//=================================================================
/**
 * drvif_ifd_get_signal_strength
 *
 *
 * @param {void}
 * @return {signal_strength}
 */
UINT32 drvif_ifd_get_signal_strength();
#endif
//=================================================================
/**
 * drvif_ifd_afc_auto_tracking
 *
 *
 * @param {curr_freq/afc_finetune_en}
 * @return {status}
 */
#ifndef CONFIG_TUNER_SILICON
bool drvif_ifd_afc_auto_tracking(UINT32* curr_freq);
#endif
//=================================================================
/**
 * drvif_ifd_set_cr_state
 *
 *
 * @param {swap}
 * @return {void}
 */
void drvif_ifd_set_cr_state(UINT8 mode);
#ifdef CONFIG_IFD_NONSTD_3P10_DETECT
//=================================================================
/**
 * drvif_ifd_non_std_3p10_detection
 *
 *
 * @param {void}
 * @return {void}
 */
void drvif_ifd_non_std_3p10_detection(void);
#endif
/*=============================================================*/
/**
 * drvif_ifd_set_videopath_output_gain
 *
 *
 * @param <data_gain>
 * @return { void }
 */
void drvif_ifd_set_videopath_output_gain(IFD_VIDEOPATH_OUTPUT_GAIN data_gain);
//=================================================================
//IFD local driver
//=================================================================
//=================================================================
/**
 * drv_ifd_analog_preset
 *
 *
 * @param <void>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_analog_preset(void);
#endif
//=================================================================
/**
 * drv_ifd_power_on
 *
 *
 * @param <enable>
 * @return {void}
 */
void drv_ifd_power_on(bool enable);
//=================================================================
/**
 * drv_ifd_clamp_reset
 *
 *
 * @param {polarity}
 * @return {void}
 */
void drv_ifd_clamp_reset(IFD_CLAMP_RESET_POLARITY polarity);
//=================================================================
/**
 * drv_ifd_set_video_ntsc
 *
 *
 * @param {void}
 * @return {void}
 */
void drv_ifd_set_video_ntsc(void);
//=================================================================
/**
 * drv_ifd_set_video_pal
 *
 *
 * @param {void}
 * @return {void}
 */
void drv_ifd_set_video_pal(void/*UINT8 mode*/);
//=================================================================
/**
 * drv_ifd_set_video_secaml
 *
 *
 * @param {void}
 * @return {void}
 */
void drv_ifd_set_video_secaml(void);
//=================================================================
/**
 * drv_ifd_set_video_secamla
 *
 *
 * @param {void}
 * @return {void}
 */
void drv_ifd_set_video_secamla();
//=================================================================
/**
 * drv_ifd_group_delay_coe_table
 *
 *
 * @param <table>
 * @return {void}
 */
void drv_ifd_group_delay_coe_table(UINT8 table);
//=================================================================
/**
 * drv_ifd_set_adc_init
 *
 *
 * @param {void}
 * @return {void}
 */
 #ifdef _VIDEO_TV_SUPPORT
void drv_ifd_set_adc_init(void);
//=================================================================
/**
 * drv_ifd_agc_mode_set
 *
 *
 * @param <ifdAGCmode>
 * @return { void }
 */
 
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_mode_set(IFD_MODE_AGC_PGA_CONFIG ifd_agc_mode);
#endif
#endif
/*=============================================================*/
/**
 * drv_ifd_cr_phase_error_variance_get_info
 *
 *
 * @param <void>
 * @return { cr_phase_error_variance }
 */
UINT16 drv_ifd_cr_phase_error_variance_get_info(void);
#ifdef _VIDEO_TV_SUPPORT

//=================================================================
/**
 * drv_ifd_initdata_by_tuner_if
 *
 *
 * @param {void}
 * @return {void}
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_initdata_by_tuner_if(void);
#endif
#endif
//=================================================================
/**
 * drv_ifd_cr_loop_settings
 *
 *
 * @param {state}
 * @return {void}
 */
void drv_ifd_cr_loop_settings(IFD_CR_LOOP_STATE state);
#ifdef CONFIG_PLATFORM_PACIFIC
//=================================================================
/**
 * drv_ifd_log_reg
 *
 *
 * @param { void }
 * @return { void }
 */
void drv_ifd_log_reg(void);
//=================================================================
/**
 * drv_ifd_periodic_log_reg
 *
 *
 * @param { void }
 * @return { void }
 */
void drv_ifd_periodic_log_reg(void);
#endif

#ifndef CONFIG_TUNER_SILICON
//=================================================================
/**
 * tuner_data_init
 *
 *
 * @param {void}
 * @return {void}
 */
void tuner_data_init(void);
#endif
//=================================================================
/**
 * drv_ifd_user_data_init
 *
 *
 * @param {void}
 * @return {void}
 */
void drv_ifd_user_data_init(void);
//=================================================================
/**
 * ifd_active_tuner_assign
 *
 *
 * @param {tunerID}
 * @return {void}
 */
void ifd_active_tuner_assign(UINT8 tunerID);
//=================================================================
/**
 * drv_ifd_set_cr_bpf_coef
 *
 *
 * @param <cr_bpf_en><table>
 * @return { void }
 */
void drv_ifd_set_cr_bpf_coef(bool cr_bpf_en, UINT16 *table);
/*=============================================================*/
/**
 * drv_ifd_cr_bpf_en
 *
 *
 * @param <cr_bpf_en>
 * @return { void }
 */
void drv_ifd_cr_bpf_en(bool cr_bpf_en);
/*=============================================================*/
/**
 * drv_ifd_cr_bpf_en_get_info
 *
 *
 * @param <void>
 * @return { cr_bpf_en }
 */
UINT8 drv_ifd_cr_bpf_en_get_info(void);
/*=============================================================*/
/**
 * drv_ifd_cr_inner_lpf_en
 *
 *
 * @param <cr_inner_lpf_en>
 * @return { void }
 */
void drv_ifd_cr_inner_lpf_en(bool cr_inner_lpf_en);
/*=============================================================*/
/**
 * drv_ifd_dma_data_sel_en
 *
 *
 * @param <dma_data_sel_en>
 * @return { void }
 */
void drv_ifd_dma_data_sel_en(bool dma_data_sel_en);
/*=============================================================*/
/**
 * drv_ifd_cr_inner_lpf_en_get_info
 *
 *
 * @param <void>
 * @return { cr_inner_lpf_en }
 */
UINT8 drv_ifd_cr_inner_lpf_en_get_info(void);
/*=============================================================*/
/**
 * drv_ifd_cr_loop_filter_kp
 *
 *
 * @param <cr_pgain>
 * @return { void }
 */
void drv_ifd_cr_loop_filter_kp( UINT8 cr_pgain);
/*=============================================================*/
/**
 * drv_ifd_cr_kp_get_info
 *
 *
 * @param <void>
 * @return { cr_pgain }
 */
UINT8 drv_ifd_cr_kp_get_info(void);
/*=============================================================*/
/**
 * drv_ifd_cr_loop_filter_ki
 *
 *
 * @param <cr_igain>
 * @return { void }
 */
void drv_ifd_cr_loop_filter_ki(UINT8 cr_igain);
/*=============================================================*/
/**
 * drv_ifd_cr_ki_get_info
 *
 *
 * @param <void>
 * @return { cr_igain }
 */
UINT8 drv_ifd_cr_ki_get_info(void);
/*=============================================================*/
/**
 * drv_ifd_get_cr_lock_status
 *
 *
 * @param <void>
 * @return <lock_status>
 */
bool drv_ifd_get_cr_lock_status();
/*=============================================================*/
/**
 * drv_ifd_set_cr_nco
 *
 *
 * @param <enable><para>
 * @return { void }
 */
void drv_ifd_set_cr_nco (bool enable, UINT32 para);
/*=============================================================*/
/**
 * drv_ifd_get_cr_freq_offset
 *
 *
 * @param <void>
 * @return <cr_freq_offset>
 */
UINT32 drv_ifd_get_cr_freq_offset(void);
/*=============================================================*/
/**
 * drv_ifd_set_cr_weighting_en
 *
 *
 * @param <cr_weight_en>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_set_cr_weighting_en (bool cr_weight_en);
#endif
/*=============================================================*/
/**
 * drv_ifd_cr_weighting_en_get_info
 *
 *
 * @param <void>
 * @return { cr_weight_en }
 */
UINT8 drv_ifd_cr_weighting_en_get_info(void);
/*=============================================================*/
/**
 * drv_ifd_set_cr_weighting_settings
 *
 *
 * @param <cr_weighting_cr_lock_dis><cr_weight_max_decrease><cr_weight_shift>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_set_cr_weighting_settings (IFD_CR_WEIGHT_CR_LOCK cr_weighting_cr_lock_dis, IFD_CR_WEIGHT_MAX_DECREASE cr_weight_max_decrease, IFD_CR_WEIGHT_SHIFT cr_weight_shift);
#endif
/*=============================================================*/
/**
 * drv_ifd_set_cr_weighting_speed
 *
 *
 * @param <cr_weight_max_decrease>
 * @return { void }
 */
void drv_ifd_set_cr_weighting_speed (IFD_CR_WEIGHT_MAX_DECREASE cr_weight_max_decrease);
/*=============================================================*/
/**
 * drv_ifd_set_cr_weighting_speed_get_info
 *
 *
 * @param <void>
 * @return { cr_weight_max_decrease }
 */
IFD_CR_WEIGHT_MAX_DECREASE drv_ifd_set_cr_weighting_speed_get_info (void);
/*=============================================================*/
/**
 * drv_ifd_set_cr_phase_bad_en
 *
 *
 * @param <cr_phase_bad_check_en>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void  drv_ifd_set_cr_phase_bad_en(bool cr_phase_bad_check_en);
#endif
/*=============================================================*/
/**
 * drv_ifd_cr_phase_bad_en_get_info
 *
 *
 * @param <void>
 * @return { cr_phase_bad_check_en }
 */
UINT8 drv_ifd_cr_phase_bad_en_get_info(void);
/*=============================================================*/
/**
 * drv_ifd_set_slow_charge_en_for_max
 *
 *
 * @param <enable>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void  drv_ifd_set_slow_charge_en_for_max(bool enable);
#endif
/*=============================================================*/
/**
 * drv_ifd_set_slow_charge_en_for_max_get_info
 *
 *
 * @param <void>
 * @return { enable }
 */
UINT8 drv_ifd_set_slow_charge_en_for_max_get_info(void);
/*=============================================================*/
/**
 * drv_ifd_set_slow_charge_en_for_peak
 *
 *
 * @param <enable>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void  drv_ifd_set_slow_charge_en_for_peak(bool enable);
#endif
/*=============================================================*/
/**
 * drv_ifd_set_slow_charge_en_for_peak_get_info
 *
 *
 * @param <void>
 * @return { enable }
 */
UINT8 drv_ifd_set_slow_charge_en_for_peak_get_info(void);
/*=============================================================*/
/**
 * drv_ifd_set_videopath_output_gain
 *
 *
 * @param <data_gain>
 * @return { void }
 */
void drv_ifd_set_videopath_output_gain(IFD_VIDEOPATH_OUTPUT_GAIN data_gain);
/*=============================================================*/
/**
 * drv_ifd_set_shaping_filter
 *
 *
 * @param <shaping_bw_sel>
 * @return { void }
 */
void drv_ifd_set_shaping_filter(IFD_SHAPING_BW_SEL shaping_bw_sel);
/*=============================================================*/
/**
 * drv_ifd_set_shaping_filter_get_info
 *
 *
 * @param <void>
 * @return { shaping_bw_sel }
 */
IFD_SHAPING_BW_SEL drv_ifd_set_shaping_filter_get_info();
/*=============================================================*/
/**
 * drv_ifd_set_shaping_img
 *
 *
 * @param <shaping_image_sel>
 * @return { void }
 */
void drv_ifd_set_shaping_img( IFD_SHAPING_IMG_SEL shaping_image_sel);
/*=============================================================*/
/**
 * drv_ifd_set_shaping_filter_img_get_info
 *
 *
 * @param <void>
 * @return <shaping_image_sel>
 */
bool drv_ifd_set_shaping_filter_img_get_info();
/*=============================================================*/
/**
 * drv_ifd_soft_reset
 *
 *
 * @param <reset>
 * @return { void }
 */
void drv_ifd_soft_reset(bool reset);
/*=============================================================*/
/**
 * drv_ifd_cr_error_thd
 *
 *
 * @param <err_thd>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_cr_error_thd(UINT8 err_thd);
#endif
/*=============================================================*/
/**
 * drv_ifd_cr_error_thd_get_info
 *
 *
 * @param <void>
 * @return { err_thd }
 */
#ifndef CONFIG_TUNER_SILICON
UINT8 drv_ifd_cr_error_thd_get_info(void);
#endif
/*=============================================================*/
/**
 * drv_ifd_cr_auto_reset_en
 *
 *
 * @param <enable>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_cr_auto_reset_en (bool enable);
#endif
/*=============================================================*/
/**
 * drv_ifd_cr_auto_reset_en_get_info
 *
 *
 * @param <void>
 * @return { enable }
 */
bool drv_ifd_cr_auto_reset_en_get_info(void);
/*=============================================================*/
/**
 * drv_ifd_cr_phase_inverse_en
 *
 *
 * @param <enable>
 * @return { void }
 */
void drv_ifd_cr_phase_inverse_en(bool enable);
/*=============================================================*/
/**
 * drv_ifd_cr_phase_inverse_en_get_info
 *
 *
 * @param <void>
 * @return { enable }
 */
bool drv_ifd_cr_phase_inverse_en_get_info(void);
#if 0//!(IFD_CHIP_VER_A)
/*=============================================================*/
/**
 * drv_ifd_cr_phase_inverse_en_depend_crlock_diable
 *
 *
 * @param <enable>
 * @return { void }
 */
void drv_ifd_cr_phase_inverse_en_depend_crlock_diable(bool enable);
/*=============================================================*/
/**
 * drv_ifd_cr_phase_inverse_en_depend_crlock_diable_get_info
 *
 *
 * @param <void>
 * @return { enable }
 */
bool drv_ifd_cr_phase_inverse_en_depend_crlock_diable_get_info(void);
#endif
/*=============================================================*/
/**
 * drv_ifd_eq_en
 *
 *
 * @param <enable>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_eq_en(bool enable);
#endif
/*=============================================================*/
/**
 * drv_ifd_eq_en_get_info
 *
 *
 * @param <void>
 * @return { enable }
 */
#ifndef CONFIG_TUNER_SILICON
bool drv_ifd_eq_en_get_info(void);
#endif
/*=============================================================*/
/**
 * drv_ifd_set_eq_coef
 *
 *
 * @param <table>
 * @return { void }
 */
//#ifndef CONFIG_TUNER_SILICON
void drv_ifd_set_eq_coef(UINT16 *table);
//#endif
/*=============================================================*/
/**
 * drv_ifd_set_54to27_decimation_enable
 *
 *
 * @param <enable>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_set_54to27_decimation_enable(bool enable);
#endif
/*=============================================================*/
/**
 * drv_ifd_set_pos_neg_mod
 *
 *
 * @param <mod_sel>
 * @return { void }
 */
void drv_ifd_set_pos_neg_mod(IFD_MODULATION_SEL mod_sel);
/*=============================================================*/
/**
 * drv_ifd_set_video_audio_nf_bw
 *
 *
 * @param <video_bw_sel><sound_notch_sel2><sound_notch_sel><video_upsample_lp_sel>
 * @return { void }
 */
void drv_ifd_set_video_audio_nf_bw(IFD_VIDEO_BW_SEL video_bw_sel, IFD_SOUND_NOTCH_SEL2 sound_notch_sel2, IFD_SOUND_NOTCH_SEL sound_notch_sel, IFD_UPSAMPLE_LP_SEL video_upsample_lp_sel);
/*=============================================================*/
/**
 * drv_ifd_set_video_bw
 *
 *
 * @param <video_bw_sel>
 * @return { void }
 */
void drv_ifd_set_video_bw(IFD_VIDEO_BW_SEL video_bw_sel);
/*=============================================================*/
/**
 * drv_ifd_get_video_bw_info
 *
 *
 * @param <void>
 * @return { video_bw_sel }
 */
IFD_VIDEO_BW_SEL drv_ifd_get_video_bw_info(void);
/*=============================================================*/
/**
 * drv_ifd_grp_delay_en
 *
 *
 * @param <enable><table>
 * @return { void }
 */
void drv_ifd_grp_delay_en (bool enable,  UINT16 *table);
/*=============================================================*/
/**
 * drv_ifd_clamp_offset_hold
 *
 *
 * @param <clamp_offset_hold>
 * @return { void }
 */
void drv_ifd_clamp_offset_hold (IFD_CLAMP_OFFSET clamp_offset_hold);
/*=============================================================*/
/**
 * drv_ifd_set_clamping_scale
 *
 *
 * @param <clamping_scale>
 * @return { void }
 */
void drv_ifd_set_clamping_scale (IFD_CLAMP_SCALE clamping_scale);
/*=============================================================*/
/**
 * drv_ifd_set_clamping_scale_get_info
 *
 *
 * @param <void>
 * @return { clamping_scale }
 */
IFD_CLAMP_SCALE drv_ifd_set_clamping_scale_get_info (void);
/*=============================================================*/
/**
 * drv_ifd_set_clamping_thr_max
 *
 *
 * @param <clamping_thr_max>
 * @return <void>
 */
void drv_ifd_set_clamping_thr_max (UINT16 clamping_thr_max);
/*=============================================================*/
/**
 * drv_ifd_set_clamping_thr_max_get_info
 *
 *
 * @param <void>
 * @return <clamping_thr_max>
 */
UINT16 drv_ifd_set_clamping_thr_max_get_info (void);
/*=============================================================*/
/**
 * drv_ifd_set_clamping_thr_min
 *
 *
 * @param <clamping_thr_min>
 * @return <void>
 */
void drv_ifd_set_clamping_thr_min (UINT16 clamping_thr_min);
/*=============================================================*/
/**
 * drv_ifd_set_clamping_thr_min_get_info
 *
 *
 * @param <void>
 * @return <clamping_thr_min>
 */
UINT16 drv_ifd_set_clamping_thr_min_get_info (void);
/*=============================================================*/
/**
 * drv_ifd_get_clamping_offset_status
 *
 *
 * @param <void>
 * @return <clamping_offset_status>
 */
UINT16 drv_ifd_get_clamping_offset_status (void);
/*=============================================================*/
/**
 * drv_ifd_set_audio_bp_up_lp
 *
 *
 * @param <audio_bp_sel><audio_upsample_lp_sel>
 * @return { void }
 */
void drv_ifd_set_audio_bp_up_lp (IFD_AUDIO_BP_SEL audio_bp_sel, IFD_AUDIO_UPSAMPLE_LP_SEL audio_upsample_lp_sel);
/*=============================================================*/
/**
 * drv_ifd_set_audio_bp
 *
 *
 * @param <audio_bp_sel>
 * @return { void }
 */
void drv_ifd_set_audio_bp (IFD_AUDIO_BP_SEL audio_bp_sel);
/*=============================================================*/
/**
 * drv_ifd_get_audio_bp_info
 *
 *
 * @param <void>
 * @return { audio_bp_sel }
 */
IFD_AUDIO_BP_SEL drv_ifd_get_audio_bp_info (void);
/*=============================================================*/
/**
 * drv_ifd_audio_gain_en
 *
 *
 * @param <enable>
 * @return { void }
 */
void drv_ifd_audio_gain_en(bool enable);
/*=============================================================*/
/**
 * drv_ifd_audio_gain_en_get_info
 *
 *
 * @param <void>
 * @return { enable }
 */
bool drv_ifd_audio_gain_en_get_info(void);
/*=============================================================*/
/**
 * drv_ifd_agc_dgain_en
 *
 *
 * @param <enable>
 * @return { void }
 */
void drv_ifd_agc_dgain_en(bool enable);
/*=============================================================*/
/**
 * drv_ifd_agc_dgain_en
 *
 *
 * @param <enable>
 * @return { void }
 */
void drv_ifd_agc_agc_rf_en(bool enable);
/*=============================================================*/
/**
 * drv_ifd_agc_loop1_fine_tune_en
 *
 *
 * @param <enable>
 * @return { void }
 */
void drv_ifd_agc_loop1_fine_tune_en(bool enable);
/*=============================================================*/
/**
 * drv_ifd_agc_loop1_fine_tune_en_get_info
 *
 *
 * @param <void>
 * @return { enable }
 */
bool drv_ifd_agc_loop1_fine_tune_en_get_info();
/*=============================================================*/
/**
 * drv_ifd_agc_bpf_en
 *
 *
 * @param <enable>
 * @return { void }
 */
void drv_ifd_agc_bpf_en(bool enable);
/*=============================================================*/
/**
 * drv_ifd_agc_bpf_en_get_info
 *
 *
 * @param <void>
 * @return { enable }
 */
bool drv_ifd_agc_bpf_en_get_info(void);
/*=============================================================*/
/**
 * drv_ifd_agc_diff_targ
 *
 *
 * @param <agc_diff_targ>
 * @return { void }
 */
void drv_ifd_agc_diff_targ(UINT16 agc_diff_targ);
/*=============================================================*/
/**
 * drv_ifd_agc_diff_target_get_info
 *
 *
 * @param <void>
 * @return { agc_diff_targ }
 */
UINT16  drv_ifd_agc_diff_target_get_info(void);
/*=============================================================*/
/**
 * drv_ifd_agc_loop1_pos_en
 *
 *
 * @param <enable>
 * @return { void }
 */
void drv_ifd_agc_loop1_pos_en (bool enable);
/*=============================================================*/
/**
 * drv_ifd_agc_pos_en_get_info
 *
 *
 * @param <void>
 * @return { bool }
 */
bool drv_ifd_agc_pos_en_get_info (void);
/*=============================================================*/
/**
 * drv_ifd_agc_targ_val
 *
 *
 * @param <agc_targ_val>
 * @return { void }
 */
void drv_ifd_agc_targ_val(UINT16 agc_targ_val);
/*=============================================================*/
/**
 * drv_ifd_agc_targ_val_get_info
 *
 *
 * @param <void>
 * @return { agc_targ_val }
 */
UINT16 drv_ifd_agc_targ_val_get_info(void);
/*=============================================================*/
/**
 * drv_ifd_agc_out_inv
 *
 *
 * @param <agc_out_inv_rf><agc_out_inv_if>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_out_inv(bool agc_out_inv_rf, bool agc_out_inv_if);
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_if_manual_en
 *
 *
 * @param <agc_if_manu_en>
 * @return { void }
 */
void drv_ifd_agc_if_manual_en(bool agc_if_manu_en);
/*=============================================================*/
/**
 * drv_ifd_agc_if_manual_en_get_info
 *
 *
 * @param <void>
 * @return { agc_if_manu_en }
 */
UINT16 drv_ifd_agc_if_manual_en_get_info(void);
/*=============================================================*/
/**
 * drv_ifd_agc_if_manual_gain
 *
 *
 * @param <agc_if_manu_value>
 * @return { void }
 */
void drv_ifd_agc_if_manual_gain(UINT16 agc_if_manu_value);
/*=============================================================*/
/**
 * drv_ifd_agc_if_manual_gain_get_info
 *
 *
 * @param <void>
 * @return { agc_if_manu_value }
 */
UINT16 drv_ifd_agc_if_manual_gain_get_info(void);
/*=============================================================*/
/**
 * drv_ifd_agc_rf_manual_en
 *
 *
 * @param <agc_rf_manu_en>
 * @return { void }
 */
void drv_ifd_agc_rf_manual_en(bool agc_rf_manu_en);
/*=============================================================*/
/**
 * drv_ifd_agc_rf_manual_en_get_info
 *
 *
 * @param <void>
 * @return { agc_rf_manu_en }
 */
bool drv_ifd_agc_rf_manual_en_get_info(void);
/*=============================================================*/
/**
 * drv_ifd_agc_manual_rf_gain
 *
 *
 * @param <agc_rf_manu_value>
 * @return { void }
 */
void drv_ifd_agc_manual_rf_gain(UINT16 agc_rf_manu_value);
/*=============================================================*/
/**
 * drv_ifd_agc_manual_rf_gain
 *
 *
 * @param <void>
 * @return { agc_rf_manu_value }
 */
UINT16 drv_ifd_agc_manual_rf_gain_get_info(void);
/*=============================================================*/
/**
 * drv_ifd_agc_manual_en
 *
 *
 * @param <agc_rf_manu_en><agc_rf_manu_value>
 * @return { void }
 */
void drv_ifd_agc_manual_en(bool agc_manual_en, UINT16 agc_manual_value);
/*=============================================================*/
/**
 * drv_ifd_agc_loop1_kp_settings
 *
 *
 * @param <agc_kp>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_loop1_kp_settings(UINT8 agc_kp);
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_loop1_ki_settings
 *
 *
 * @param <agc_ki>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_loop1_ki_settings(UINT8 agc_ki);
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_loop1_pishift_settings
 *
 *
 * @param <agc_kpd_add_method_shift>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_loop1_pishift_settings( UINT8 agc_kpd_add_method_shift);
#endif

/*=============================================================*/
/**
 * drv_ifd_get_agc_loop1_kp_settings
 *
 *
 * @param <void>
 * @return <agc_kp>
 */
UINT8 drv_ifd_get_agc_loop1_kp_settings();
/*=============================================================*/
/**
 * drv_ifd_get_agc_loop1_ki_settings
 *
 *
 * @param <void>
 * @return <agc_ki >
 */
UINT8 drv_ifd_get_agc_loop1_ki_settings();
/*=============================================================*/
/**
 * drv_ifd_get_agc_loop1_pishift_settings
 *
 *
 * @param <void>
 * @return <agc_kpd_add_method_shift >
 */
UINT8 drv_ifd_get_agc_loop1_pishift_settings();
/*=============================================================*/
/**
 * drv_ifd_agc_rf_max
 *
 *
 * @param <rf_max>
 * @return <void >
 */
void drv_ifd_agc_rf_max(UINT16 rf_max);
/*=============================================================*/
/**
 * drv_ifd_agc_rf_max_get_info
 *
 *
 * @param <void>
 * @return <rf_max >
 */
UINT16 drv_ifd_agc_rf_max_get_info();
/*=============================================================*/
/**
 * drv_ifd_agc_get_loop1_lock
 *
 *
 * @param <void>
 * @return <lock_status>
 */
bool drv_ifd_agc_get_loop1_lock();
/*=============================================================*/
/**
 * drv_ifd_agc_get_loop1_fine_lock
 *
 *
 * @param <void>
 * @return <fine_lock_status>
 */
bool drv_ifd_agc_get_loop1_fine_lock();
/*=============================================================*/
/**
 * drv_ifd_agc_get_loop1_peak_status
 *
 *
 * @param <void>
 * @return <peak_status>
 */
bool drv_ifd_agc_get_loop1_peak_status();
/*=============================================================*/
/**
 * drv_ifd_agc_loop1_peak_en
 *
 *
 * @param <agc_peak_en>
 * @return { void }
 */
void drv_ifd_agc_loop1_peak_en(bool agc_peak_en);
/*=============================================================*/
/**
 * drv_ifd_get_agc_loop1_peak_en
 *
 *
 * @param <void>
 * @return <agc_peak_en>
 */
bool drv_ifd_get_agc_loop1_peak_en();
/*=============================================================*/
/**
 * drv_ifd_agc_peak_target
 *
 *
 * @param <agc_peak_value>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_peak_target(UINT16 agc_peak_value);
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_peak_target_get_info
 *
 *
 * @param <void>
 * @return { agc_peak_value }
 */
UINT16  drv_ifd_agc_peak_target_get_info(void);
/*=============================================================*/
/**
 * drv_ifd_agc_get_loop1_err0
 *
 *
 * @param <void>
 * @return <agc_err>
 */
UINT16 drv_ifd_agc_get_loop1_err0();
/*=============================================================*/
/**
 * drv_ifd_agc_get_if_output
 *
 *
 * @param <void>
 * @return <agc_if_mode>
 */
UINT16 drv_ifd_agc_get_if_output();
/*=============================================================*/
/**
 * drv_ifd_agc_get_if_output
 *
 *
 * @param <void>
 * @return <agc_if_mode>
 */
UINT16 drv_ifd_agc_get_if_output();
/*=============================================================*/
/**
 * drv_ifd_agc_get_rf_output
 *
 *
 * @param <void>
 * @return <agc_rf_mode>
 */
UINT16 drv_ifd_agc_get_rf_output();
/*=============================================================*/
/**
 * drv_ifd_agc_pga_manual_en
 *
 *
 * @param <agc_pga_fix_en><agc_pga_fix>
 * @return { void }
 */
void drv_ifd_agc_pga_manual_en(bool agc_pga_fix_en, UINT16 agc_pga_fix);
/*=============================================================*/
/**
 * drv_ifd_agc_pga_manual_en_get_info
 *
 *
 * @param { void }
 * @return { agc_pga_fix_en }
 */
bool drv_ifd_agc_pga_manual_en_get_info();
/*=============================================================*/
/**
 * drv_ifd_agc_pga_manual_index_get_info
 *
 *
 * @param { void }
 * @return { index_output }
 */
UINT8 drv_ifd_agc_pga_manual_index_get_info();
/*=============================================================*/
/**
 * drv_ifd_agc_vtop
 *
 *
 * @param <agc_vtop>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_vtop(UINT32 agc_vtop);
#endif
/*=============================================================*/
/**
 * drv_ifd_get_agc_vtop
 *
 *
 * @param <void>
 * @return <agc_vtop>
 */
UINT32 drv_ifd_get_agc_vtop();
/*=============================================================*/
/**
 * drv_ifd_agc_krf
 *
 *
 * @param <agc_krf>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_krf(UINT32 agc_krf);
#endif
/*=============================================================*/
/**
 * drv_ifd_get_agc_krf
 *
 *
 * @param <void>
 * @return <agc_krf>
 */
UINT32 drv_ifd_get_agc_krf();
/*=============================================================*/
/**
 * drv_ifd_agc_get_pga_code
 *
 *
 * @param <void>
 * @return <pga_set_read>
 */
UINT16 drv_ifd_agc_get_pga_code();
/*=============================================================*/
/**
 * drv_ifd_agc_pga_index_get_info
 *
 *
 * @param { void }
 * @return { index_output }
 */
UINT8 drv_ifd_agc_pga_index_get_info();
/*=============================================================*/
/**
 * drv_ifd_agc_original_mode
 *
 *
 * @param <agc_original>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_original_mode(bool agc_original);
#endif
/*=============================================================*/
/**
 * drv_ifd_get_agc_original_mode
 *
 *
 * @param <void>
 * @return <agc_original>
 */
bool drv_ifd_get_agc_original_mode();
/*=============================================================*/
/**
 * drv_ifd_agc_vsync_mode_en
 *
 *
 * @param <agc_vsync_mode_en>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_vsync_mode_en(bool agc_vsync_mode_en);
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_hold
 *
 *
 * @param <agc_loop2_hold>
 * @return { void }
 */
void drv_ifd_agc_loop2_hold(bool agc_loop2_hold);
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_hold_get_info
 *
 *
 * @param <void>
 * @return { agc_loop2_hold }
 */
bool drv_ifd_agc_loop2_hold_get_info();
/*=============================================================*/
/**
 * drv_ifd_loop2_agcin_kpki_settings
 *
 *
 * @param <agc_in_kp><agc_in_ki><agc_in_kp_shift><agc_in_ki_shift>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_loop2_agcin_kpki_settings(UINT8 agc_in_kp, UINT8 agc_in_ki, UINT8 agc_in_kp_shift, UINT8 agc_in_ki_shift);
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_kp_settings
 *
 *
 * @param <agc_in_kp>
 * @return { void }
 */
void drv_ifd_agc_loop2_kp_settings(UINT8 agc_in_kp);
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_kp_settings_get_info
 *
 *
 * @param <void>
 * @return { agc_in_kp }
 */
UINT8 drv_ifd_agc_loop2_kp_settings_get_info(void);
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_ki_settings
 *
 *
 * @param <agc_in_ki>
 * @return { void }
 */
void drv_ifd_agc_loop2_ki_settings(UINT8 agc_in_ki);
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_ki_settings_get_info
 *
 *
 * @param <void>
 * @return { agc_in_ki }
 */
UINT8 drv_ifd_agc_loop2_ki_settings_get_info(void);
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_kishift_settings
 *
 *
 * @param <agc_in_ki_shift>
 * @return { void }
 */
void drv_ifd_agc_loop2_kishift_settings(UINT8 agc_in_ki_shift);
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_kishift_settings_get_info
 *
 *
 * @param <void>
 * @return { agc_in_ki_shift }
 */
UINT8 drv_ifd_agc_loop2_kishift_settings_get_info(void);
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_kpshift_settings
 *
 *
 * @param <agc_in_kp_shift>
 * @return { void }
 */
void drv_ifd_agc_loop2_kpshift_settings(UINT8 agc_in_kp_shift);
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_kpshift_settings_get_info
 *
 *
 * @param <void>
 * @return { agc_in_kp_shift }
 */
UINT8 drv_ifd_agc_loop2_kpshift_settings_get_info(void);
/*=============================================================*/
/**
 * drv_ifd_agc_dgain_boundary
 *
 *
 * @param <agc_in_max><agc_in_min>
 * @return { void }
 */
void drv_ifd_agc_dgain_boundary(UINT16 agc_in_max, UINT16 agc_in_min);
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_target
 *
 *
 * @param <agc_targ_val_loop2>
 * @return { void }
 */
void drv_ifd_agc_loop2_target(UINT16 agc_targ_val_loop2);
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_target_get_info
 *
 *
 * @param <void>
 * @return { target }
 */
UINT16 drv_ifd_agc_loop2_target_get_info();
/*=============================================================*/
/**
 * drv_ifd_agc_get_loop2_err
 *
 *
 * @param {void}
 * @return  <loop2_err>
 */
UINT16 drv_ifd_agc_get_loop2_err();
/*=============================================================*/
/**
 * drv_ifd_agc_get_loop1_dgain_output
 *
 *
 * @param {void}
 * @return  <agc_inner1>
 */
UINT16 drv_ifd_agc_get_loop1_dgain_output();
/*=============================================================*/
/**
 * drv_ifd_agc_get_loop2_dgain_output
 *
 *
 * @param {void}
 * @return  <agc_inner2>
 */
UINT16 drv_ifd_agc_get_loop2_dgain_output();
/*=============================================================*/
/**
 * drv_ifd_agc_adaptive_peak2_en
 *
 *
 * @param <enable>
 * @return { void }
 */
void drv_ifd_agc_adaptive_peak2_en(bool enable);
/*=============================================================*/
/**
 * drv_ifd_agc_get_loop2_peak_status
 *
 *
 * @param {void}
 * @return  <agc_peak2_status>
 */
bool drv_ifd_agc_get_loop2_peak_status();
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_peak_en
 *
 *
 * @param <enable>
 * @return { void }
 */
void drv_ifd_agc_loop2_peak_en(bool enable);
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_peak_cnt
 *
 *
 * @param <agc_peak2_cnt>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_loop2_peak_cnt(UINT16 agc_peak2_cnt);
#endif
/*=============================================================*/
/**
 * drv_ifd_get_agc_loop2_peak_en
 *
 *
 * @param <void>
 * @return <agc_peak2_en>
 */
bool drv_ifd_get_agc_loop2_peak_en();
/*=============================================================*/
/**
 * drv_ifd_agc_peak2_target
 *
 *
 * @param <agc_peak2_value>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_peak2_target(UINT16 agc_peak2_value);
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_get_loop2_peak_error
 *
 *
 * @param {void}
 * @return  <error>
 */
UINT16 drv_ifd_agc_get_loop2_peak_error();
/*=============================================================*/
/**
 * drv_ifd_agc_get_loop2_peak_max
 *
 *
 * @param {void}
 * @return  <max>
 */
UINT16 drv_ifd_agc_get_loop2_peak_max();
/*=============================================================*/
/**
 * drv_ifd_agc_get_loop2_lock
 *
 *
 * @param {void}
 * @return  <agc_lock2>
 */
bool drv_ifd_agc_get_loop2_lock();
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_fine_tune_en
 *
 *
 * @param <enable>
 * @return { void }
 */
void drv_ifd_agc_loop2_fine_tune_en(bool enable);
/*=============================================================*/
/**
 * drv_ifd_agc_2step_en
 *
 *
 * @param <enable>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_2step_en(bool enable);
#endif
/*=============================================================*/
/**
 * drv_ifd_get_agc_2step_en
 *
 *
 * @param <void>
 * @return <enable>
 */
bool drv_ifd_get_agc_2step_en();
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_pos_en
 *
 *
 * @param <enable>
 * @return { void }
 */
void drv_ifd_agc_loop2_pos_en(bool enable);
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_diff_target
 *
 *
 * @param <agc_diff_targ2>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_loop2_diff_target(UINT16 agc_diff_targ2);
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_dgain_up_dn_step
 *
 *
 * @param <agc_dgain_up_step><agc_dgain_down_step>
 * @return { void }
 */
void drv_ifd_agc_dgain_up_dn_step(UINT16 agc_dgain_up_step, UINT16 agc_dgain_down_step);
/*=============================================================*/
/**
 * drv_ifd_agc_dgain_up_dn_boundary
 *
 *
 * @param <agc_dgain_low_thr><agc_dgain_up_thr>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_dgain_up_dn_boundary(UINT16 agc_dgain_low_thr, UINT16 agc_dgain_up_thr);
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_2step_pga_step
 *
 *
 * @param <agc_step_num_pga>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_2step_pga_step(UINT8 agc_step_num_pga);
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_2step_pga_minimum
 *
 *
 * @param <agc_dgain_min_pga>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_2step_pga_minimum(UINT32 agc_dgain_min_pga);
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_2step_pga_minimum_get_info
 *
 *
 * @param <void>
 * @return < agc_dgain_min_pga >
 */
UINT32 drv_ifd_agc_2step_pga_minimum_get_info();
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_fine_lock
 *
 *
 * @param {void}
 * @return  <fine_lock2>
 */
bool drv_ifd_agc_loop2_fine_lock();
/*=============================================================*/
/**
 * drv_ifd_bypass_demod
 *
 *
 * @param <bypass>
 * @return { void }
 */
void drv_ifd_bypass_demod(bool bypass);
/*=============================================================*/
/**
 * drv_ifd_adc_input_sel
 *
 *
 * @param <input_source>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_adc_input_sel(IFD_ADC_INPUT input_source);
#endif
/*=============================================================*/
/**
 * drv_ifd_adc_clk_sel
 *
 *
 * @param <clk_rate>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_adc_clk_sel(IFD_CLK_RATE clk_rate);
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_mode_sel
 *
 *
 * @param <agc_mode>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_mode_sel(IFD_AGC_MODE agc_mode);
#endif
/*=============================================================*/
/**
 * drv_ifd_get_agc_mode_sel
 *
 *
 * @param <void>
 * @return <agc_mode>
 */
IFD_AGC_MODE drv_ifd_get_agc_mode_sel();
/*=============================================================*/
/**
 * drv_ifd_agc_vsync_start1
 *
 *
 * @param <start><end>
 * @return { void }
 */
void drv_ifd_agc_vsync_start1(UINT16 start, UINT16 end);
/*=============================================================*/
/**
 * drv_ifd_agc_hsync_start1
 *
 *
 * @param <start><end>
 * @return { void }
 */
void drv_ifd_agc_hsync_start1(UINT16 start, UINT16 end);
/*=============================================================*/
/**
 * drv_ifd_agc_vsync_start2
 *
 *
 * @param <start><end>
 * @return { void }
 */
void drv_ifd_agc_vsync_start2(UINT16 start, UINT16 end);
/*=============================================================*/
/**
 * drv_ifd_agc_hsync_start2
 *
 *
 * @param <start><end>
 * @return { void }
 */
void drv_ifd_agc_hsync_start2(UINT16 start, UINT16 end);
/*=============================================================*/
/**
 * drv_ifd_agc_loop1_finetune_dn_sample
 *
 *
 * @param <dn_sample>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_loop1_finetune_dn_sample(UINT16 dn_sample);
#endif
/*=============================================================*/
/**
 * drvif_ifd_afc_auto_tracking
 *
 *
 * @param <UINT32 *curr_freq>
 * @return { bool }
 */
#ifdef CONFIG_TUNER_SILICON
bool drvif_ifd_afc_auto_tracking(UINT32* curr_freq);
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_loop1_finetune_dn_sample_get_info
 *
 *
 * @param <void>
 * @return { dn_sample }
 */
UINT16 drv_ifd_agc_loop1_finetune_dn_sample_get_info(void);
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_finetune_dn_sample
 *
 *
 * @param <dn_sample>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_loop2_finetune_dn_sample(UINT16 dn_sample);
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_finetune_dn_sample_get_info
 *
 *
 * @param <void>
 * @return { dn_sample }
 */
UINT16 drv_ifd_agc_loop2_finetune_dn_sample_get_info(void);
/*=============================================================*/
/**
 * drv_ifd_agc_loop1_dn_sample
 *
 *
 * @param <dnsample>
 * @return { void }
 */
void drv_ifd_agc_loop1_dn_sample (UINT16 dnsample);
/*=============================================================*/
/**
 * drv_ifd_agc_loop1_dn_sample_get_info
 *
 *
 * @param <void>
 * @return { dnsample }
 */
UINT16 drv_ifd_agc_loop1_dn_sample_get_info(void);
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_dn_sample
 *
 *
 * @param <dnsample>
 * @return { void }
 */
void drv_ifd_agc_loop2_dn_sample(UINT16 dnsample);
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_dn_sample_get_info
 *
 *
 * @param <void>
 * @return { dnsample }
 */
UINT16 drv_ifd_agc_loop2_dn_sample_get_info(void);
/*=============================================================*/
/**
 * drv_ifd_rf_full_get_info
 *
 *
 * @param <void>
 * @return { dnsample }
 */
bool drv_ifd_rf_full_get_info(void);

UINT16 drv_ifd_agc_get_loop2_max_out();
/*=============================================================*/
/**
 * drv_ifd_set_vd_path
 *
 *
 * @param <void>
 * @return {void}
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_set_vd_path(void);
#endif

#ifdef CONFIG_SPARROW
#ifdef CONFIG_IFD_TUNER_AUTO
//=================================================================
/**
 * drvif_ifd_isr_set_standard
 *
 *
 * @param <video_mode><freq>
 * @return { void }
 */
void drvif_ifd_isr_set_standard(UINT8 video_mode, UINT32 freq);
#endif
#endif

#endif // #ifdef _IFD_H



