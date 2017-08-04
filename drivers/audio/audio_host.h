/*=============================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2007 *
 * All rights reserved.                                       *
 *============================================================*/
/*======================= Description ========================*/
/**
 * @file  audio_host.h
 *
 * @author
 * @date
 * @version 0.1
 * @ingroup drv_adc
 *
 *=============================================================*/
#ifndef __AUDIO_HOST_H__
#define __AUDIO_HOST_H__

#include <rtd_types.h>


#define	AIO_TIMEDLY_100US	(24)
#define	AIO_INIT_DWN_CNT	(0x0FFFFFF)


#define	AIO_DOWN_SAMPLE_STEP	(3)		// result of power of 2, Ex. 192K-->96K-->48K

typedef enum
{
    AIO_DOWN_SAMPLE_DIV_2,
    AIO_DOWN_SAMPLE_DIV_4
} AIO_DOWN_SAMPLE_DIV;

typedef enum
{
    AIO_DAAD_DOWN_DIV_1,
    AIO_DAAD_DOWN_DIV_2,
    AIO_DAAD_DOWN_DIV_4
} AIO_DAAD_DOWN_DIV;

typedef enum
{
    AIO_PNB_FS_DIV_1,
    AIO_PNB_FS_DIV_2,
    AIO_PNB_FS_DIV_4
} AIO_PNB_FS_DIV;

typedef enum
{
    // identical to the order in register's field
    AIO_PLL_64_512         = 0x00,
    AIO_PLL_SPDIF_TRACKING = 0x01,
    AIO_PLL_AUDIO          = 0x02,
    AIO_PLL_NICAM_RESAMPLE = 0x03
} AIO_PLL_SRC;

typedef enum
{
    // identical to the order in register's field
    AIO_SPDIF_CLK_NICAM = 0x00,
    AIO_SPDIF_CLK_SPDIF = 0x01
} AIO_SPDIF_CLK_SRC;

typedef enum
{
    // identical to the order in register's field
    AIO_I2S_CLK_EXT_I2S  = 0x00,
    AIO_I2S_CLK_INT_HDMI = 0x01,
    AIO_I2S_CLK_INT_DTV  = 0x02,
    AIO_I2S_CLK_EXT_PCM  = 0x03
} AIO_I2S_CLK_SRC;

typedef enum
{
    // identical to the order in register's field
    AIO_ADC_512FS_24576 = 0x00,
    AIO_ADC_512FS_49152 = 0x01,
    AIO_ADC_512FS_98304 = 0x02,
    AIO_ADC_512FS_EXT   = 0x03
} AIO_ADC_512FS_SEL;


typedef struct
{
    unsigned	Ccode:8;
    unsigned	Dcode:8;
    unsigned	Ecode:8;
    unsigned	Acode:6;
    unsigned	div_2fs:2;
} AIO_SPDIF_PLL_TAB;

typedef struct
{
    unsigned	resvd:6;
    unsigned	bypass_N:1;
    unsigned	bypass_P:1;
    unsigned	Mcode:8;
    unsigned	Ncode:4;
    unsigned	Pcode:4;
} AIO_I2S_PLL_TAB;



typedef struct
{
    UINT8	id;
    UINT8	src[4];
    UINT8	hdev_mode;
    UINT8	std_sel;
    UINT8	curr_ipt_fs;
    UINT8	curr_opt_fs;
    UINT8	max_playback_fs;

    UINT8	prev_opt_fs;
    UINT8	spdifi_lock_flag;
    UINT8	spdifi_prev_fs;
    UINT8	spdifo_data_type;		// [0x0,0x1] = [PCM,NON_PCM]
    UINT8	spdifo_src_sel;
    UINT16	beeper_freq;
    UINT16	beeper_duration;

    UINT32	int_mask;

    UINT16	atv_fw_scan_flag;
    UINT16	atv_fw_scan_location;
    UINT16	atv_fw_scan_sc1;
    UINT16	atv_fw_scan_sc2;
    UINT16	atv_fw_scan_std;
    UINT8	atv_fw_scan_cnt;
    UINT8	atv_fw_scan_usr;
    UINT8	atv_fw_scan_nicam_cnt;
    UINT8	atv_fw_scan_noise_cnt;
    UINT8	atv_fw_scan_tick_debounce;
    UINT8	atv_fw_scan_sc1_debounce;

} AIO_PATH_INFO;


typedef enum
{
    AIO_INT_FP_SIF_OV        = 0,
    AIO_INT_FP_MODE_CHG      = 1,
    AIO_INT_FP_MSD_TO        = 2,
    AIO_INT_FP_MSD_CHG       = 3,
    AIO_INT_FP_SIF_NOPT      = 4,
    AIO_INT_FP_I2S2_WL_CHG   = 5,
    AIO_INT_FP_I2S1_WL_CHG   = 6,
    AIO_INT_FP_SPDIF_SR_CHG  = 7,
    AIO_INT_FP_I2S2_SR_CHG   = 8,
    AIO_INT_FP_I2S1_SR_CHG   = 9,
    AIO_INT_FP_NICAM_HIB_CHG = 10,
    AIO_INT_FP_SPDIF_LK_CHG  = 11,
    AIO_INT_FP_SPDIF_DT_CHG  = 12,
    AIO_INT_FP_AO_TIMER      = 13,
    AIO_INT_FP_AI_TIMER      = 14,
    AIO_INT_FP_PCM_SR_CHG    = 15,
    AIO_INT_FP_FIFO0_EMPTY   = 16,
    AIO_INT_FP_FIFO1_EMPTY   = 17,
    AIO_INT_FP_FIFO2_EMPTY   = 18,
    AIO_INT_FP_FIFO3_EMPTY   = 19,
    AIO_INT_FP_FIFO0_FULL    = 20,
    AIO_INT_FP_FIFO1_FULL    = 21,
    AIO_INT_FP_FIFO2_FULL    = 22,
    AIO_INT_FP_FIFO3_FULL    = 23,
    AIO_INT_FP_BTSC_MODE_CHG = 24,
    AIO_INT_FP_RBUS_TO       = 25
} AIO_INT_FP;

typedef enum
{
    // bit-mapping setting
    AIO_INT_ID_SIF_OV        = (0x1<<AIO_INT_FP_SIF_OV),
    AIO_INT_ID_MODE_CHG      = (0x1<<AIO_INT_FP_MODE_CHG),
    AIO_INT_ID_MSD_TO        = (0x1<<AIO_INT_FP_MSD_TO),
    AIO_INT_ID_MSD_CHG       = (0x1<<AIO_INT_FP_MSD_CHG),
    AIO_INT_ID_SIF_NOPT      = (0x1<<AIO_INT_FP_SIF_NOPT),
    AIO_INT_ID_I2S2_WL_CHG   = (0x1<<AIO_INT_FP_I2S2_WL_CHG),
    AIO_INT_ID_I2S1_WL_CHG   = (0x1<<AIO_INT_FP_I2S1_WL_CHG),
    AIO_INT_ID_SPDIF_SR_CHG  = (0x1<<AIO_INT_FP_SPDIF_SR_CHG),
    AIO_INT_ID_I2S2_SR_CHG   = (0x1<<AIO_INT_FP_I2S2_SR_CHG),
    AIO_INT_ID_I2S1_SR_CHG   = (0x1<<AIO_INT_FP_I2S1_SR_CHG),
    AIO_INT_ID_NICAM_HIB_CHG = (0x1<<AIO_INT_FP_NICAM_HIB_CHG),
    AIO_INT_ID_SPDIF_LK_CHG  = (0x1<<AIO_INT_FP_SPDIF_LK_CHG),
    AIO_INT_ID_SPDIF_DT_CHG  = (0x1<<AIO_INT_FP_SPDIF_DT_CHG),
    AIO_INT_ID_AO_TIMER      = (0x1<<AIO_INT_FP_AO_TIMER),
    AIO_INT_ID_AI_TIMER      = (0x1<<AIO_INT_FP_AI_TIMER),
    AIO_INT_ID_PCM_SR_CHG    = (0x1<<AIO_INT_FP_PCM_SR_CHG),
    AIO_INT_ID_FIFO0_EMPTY   = (0x1<<AIO_INT_FP_FIFO0_EMPTY),
    AIO_INT_ID_FIFO1_EMPTY   = (0x1<<AIO_INT_FP_FIFO1_EMPTY),
    AIO_INT_ID_FIFO2_EMPTY   = (0x1<<AIO_INT_FP_FIFO2_EMPTY),
    AIO_INT_ID_FIFO3_EMPTY   = (0x1<<AIO_INT_FP_FIFO3_EMPTY),
    AIO_INT_ID_FIFO0_FULL    = (0x1<<AIO_INT_FP_FIFO0_FULL),
    AIO_INT_ID_FIFO1_FULL    = (0x1<<AIO_INT_FP_FIFO1_FULL),
    AIO_INT_ID_FIFO2_FULL    = (0x1<<AIO_INT_FP_FIFO2_FULL),
    AIO_INT_ID_FIFO3_FULL    = (0x1<<AIO_INT_FP_FIFO3_FULL),
    AIO_INT_ID_BTSC_MODE_CHG = (0x1<<AIO_INT_FP_BTSC_MODE_CHG),
    AIO_INT_ID_RBUS_TO       = (0x1<<AIO_INT_FP_RBUS_TO),
//	AIO_INT_ID_ALL           = 0x03FFFFFF,
} AIO_INT_ID;

#define AIO_INT_ID_ALL	0x03FFFFFF

typedef enum
{
    // identical to AIO_PATH_SRC in audio_aio.h , this is for ROS
    AUDIO_SRC_ATV       = 0x00,	// real input, mapping to register bit-setting
    AUDIO_SRC_BBADC     = 0x01,	// real input, mapping to register bit-setting
    AUDIO_SRC_SPDIF     = 0x02,	// real input, mapping to register bit-setting
    AUDIO_SRC_PCM       = 0x03,	// real input, mapping to register bit-setting
    AUDIO_SRC_I2S_CH12  = 0x04,	// real input, mapping to register bit-setting
    AUDIO_SRC_I2S_CH34  = 0x05,	// real input, mapping to register bit-setting
    AUDIO_SRC_I2S_CH56  = 0x06,	// real input, mapping to register bit-setting
    AUDIO_SRC_I2S_CH78  = 0x07,	// real input, mapping to register bit-setting
    AUDIO_SRC_ATV_NICAM = 0x08,	// shared input(ATV), affect clk setting
    AUDIO_SRC_ATV_HDEV  = 0x09,	// shared input(ATV), affect clk setting
//	AUDIO_SRC_DTV       = 0x10,	// pseudo input, affect clk setting
//	AUDIO_SRC_FILE      = 0x20,	// pseudo input, affect clk setting
    AUDIO_SRC_UNKNOWN   = 0xFF
} AUDIO_SRC;

typedef enum
{
    /*
     * If you want to change this definition. you also need to update all PLL look-up table
     * and the calculation of down-sample divider in Audio_AioSetPathClock()
     * (Ivan)
     */
    AIO_FS_ID_8K   = 0x00,
    AIO_FS_ID_11K  = 0x01,
    AIO_FS_ID_12K  = 0x02,
    AIO_FS_ID_16K  = 0x03,
    AIO_FS_ID_22K  = 0x04,
    AIO_FS_ID_24K  = 0x05,
    AIO_FS_ID_32K  = 0x06,
    AIO_FS_ID_44K  = 0x07,
    AIO_FS_ID_48K  = 0x08,
    AIO_FS_ID_64K  = 0x09,	// LasVegas do not support
    AIO_FS_ID_88K  = 0x0A,
    AIO_FS_ID_96K  = 0x0B,
    AIO_FS_ID_128K = 0x0C,	// LasVegas do not support
    AIO_FS_ID_176K = 0x0D,
    AIO_FS_ID_192K = 0x0E,
    AIO_FS_ID_UNKNOWN = 0x0F
} AIO_FS_ID;

typedef enum
{
    AIO_FIFO_ID_CH12,
    AIO_FIFO_ID_CH34,
    AIO_FIFO_ID_CH56,
    AIO_FIFO_ID_CH78,
} AIO_FIFO_ID;

extern AIO_PATH_INFO	pb_path_info;

/*=============================================================*/
/**
 * Audio_ISR_Dispatcher
 * audio interrupt service routine
 *
 * @param <void>	{ void }
 * @return 			{ succeed in handling interrupt or not }
 * @ingroup drv_adc
 */
void Audio_ISR_Dispatcher();


void Audio_ISR_I2S_WordLength_Change(void);
void Audio_ISR_I2S_SampleRate_Change(void);
void Audio_ISR_SPDIF_SampleRate_Change(void);
void Audio_ISR_SPDIF_DataType_Change(void);
void Audio_ISR_SPDIF_LockStatus_Change(void);
void Audio_ISR_ATV_AutoStdDetect_Change(void);
void Audio_ISR_ATV_ModeStatus_Change(void);
void Audio_ISR_ATV_BTSC_ModeStatus_Change(void);
void Audio_ISR_ATV_NICAM_HiErrRate_Change(void);
void Audio_AioSetInterrupt(BOOL para, UINT32 src_sel);
void Audio_AioSetBackendPlay(BOOL para);



/*============ Prototype of System Routines ===================*/
/*=============================================================*/
/**
 * Audio_TimeDelay_us
 *
 * @param <us>		{ time ticks }
 * @return 			{ void }
 * @ingroup drv_adc
 */
void Audio_TimeDelay_us(UINT16 us);

/*=============================================================*/
/**
 * Audio_TimeDelay_ms
 *
 * @param <ms>		{ time ticks }
 * @return 			{ void }
 * @ingroup drv_adc
 */
void Audio_TimeDelay_ms(UINT16 ms);


/*=============================================================*/
/**
 * Audio_EnterCriticalSection
 * disable audio interrupt by cleaning the IE bit in Global IE register
 *
 * @param <void>	{ void }
 * @return 			{ void }
 * @ingroup drv_adc
 */
void Audio_EnterCriticalSection();

/*=============================================================*/
/**
 * Audio_LeaveCriticalSection
 * enable audio interrupt by setting the IE bit in Global IE register
 *
 * @param <void>	{ void }
 * @return 			{ void }
 * @ingroup drv_adc
 */
void Audio_LeaveCriticalSection();


#endif //#ifndef __AUDIO_HOST_H__

