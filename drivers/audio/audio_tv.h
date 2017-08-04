/*=============================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2007 *
 * All rights reserved.                                       *
 *============================================================*/
/*======================= Description ========================*/
/**
 * @file  audio_tv.h
 *
 * @author
 * @date
 * @version 0.1
 * @ingroup drv_adc
 *
 *=============================================================*/
#ifndef _AUDIO_TV_H_
#define _AUDIO_TV_H_

// internal configuration of ATV driver
#define ATV_CFG_FW_SCAN_BY_TIMER_EVENT
#define ATV_CFG_NICAM_SWITCH_BY_TIMER_EVENT
//#define ATV_CFG_FW_SCAN_ALGO_DRAGON	// dragon
#define ATV_CFG_FW_SCAN_ALGO_NEW	// columbus/lasvegas/sparrow



// IFD resigters
#define ATV_IFD_AUDIO_BP_SEL_VADDR		(AUDIO_BP_SEL_0x5cc0)
#define IFD_BP_SEL_BYPASS				(0x00<<4)
#define IFD_BP_SEL_4P5MHZ_200K			(0x01<<4)
#define IFD_BP_SEL_4P5MHZ_540K			(0x02<<4)
#define IFD_BP_SEL_4P5MHZ_400K			(0x03<<4)
#define IFD_BP_SEL_5P30MHZ_6P7MHZ		(0x04<<4)
#define IFD_BP_SEL_5P25MHZ_6P75MHZ		(0x05<<4)
#define IFD_BP_SEL_5P2MHZ_6P8MHZ		(0x06<<4)
#define IFD_BP_SEL_6P5MHZ_200K			(0x07<<4)
#define IFD_BP_SEL_6P5MHZ_300K			(0x08<<4)
#define IFD_BP_SEL_6P5MHZ_400K			(0x09<<4)
#define IFD_BP_SEL_4P5MHZ_688K			(0x0a<<4)
#define IFD_BP_SEL_4P5MHZ_600K			(0x0b<<4)
#define IFD_BP_SEL_4P5MHZ_450K			(0x0c<<4)
#define IFD_BP_SEL_6P126MHZ_2P021MHZ	(0x10<<4)
#define IFD_BP_SEL_6P126MHZ_2P012MHZ	(0x11<<4)
#define IFD_BP_SEL_6P126MHZ_2P003MHZ	(0x12<<4)
#define IFD_BP_SEL_6P126MHZ_1P987MHZ	(0x13<<4)
#define IFD_BP_SEL_6P126MHZ_1P862MHZ	(0x14<<4)
#define IFD_BP_SEL_6P126MHZ_1P853MHZ	(0x15<<4)
#define IFD_BP_SEL_6P126MHZ_1P838MHZ	(0x16<<4)
#define IFD_BP_SEL_4P5MHZ_1MHZ			(0x17<<4)
#define IFD_BP_SEL_4P5MHZ_1P4MHZ		(0x18<<4)
#define IFD_BP_SEL_MASK_BITS			(0xff<<4)
#define IFD_LP_SEL_BYPASS				(0x0<<0)
#define IFD_LP_SEL_NTSC					(0x1<<0)
#define IFD_LP_SEL_PAL					(0x2<<0)
#define IFD_LP_SEL_MASK_BITS			(0x3<<0)

#define ATV_IFD_AIN_SEL_VADDR			(AIN_SEL_0x5fc0)
#define IFD_AIN_SEL_SIFPGA				(0x0<<0)
#define IFD_AIN_SEL_IFDPGA				(0x1<<0)
#define IFD_AIN_SEL_BYPASS_DIF			(0x2<<0)
#define IFD_AIN_SEL_HIZ					(0x3<<0)
#define IFD_AIN_SEL_MASK_BITS			(0x3<<0)

#define ATV_IFD_ADC_CK_VADDR			(ADC_CK_0x5fd0)
#define IFD_ADC_CK_FOR_SIF				(0x0<<7)	// 24.576MHZ
#define IFD_ADC_CK_FOR_IFD				(0x1<<7)	// 54MHZ
#define IFD_ADC_CK_MASK_BIT				(0x1<<7)


//#define ATV_FW_SCAN_FREQ_RANGE			(20)
#define ATV_FW_SCAN_MAG_THRES			(0x400)		// according to dragon's experience
#define ATV_FW_SCAN_TICK_DEBOUNCE_LEVEL	(8)
#define ATV_FW_SCAN_NOISE_CNT_LEVEL		(2)


#define ATV_FW_SCAN_FREQ_OFFSET			CONFIG_AUDIO_ATV_FREQ_SHIFT_RANGE
#define ATV_FW_SCAN_FREQ_STEP			(50)	//(2*ATV_FW_SCAN_FREQ_OFFSET/ATV_FW_SCAN_FREQ_RANGE)
#define ATV_FW_SCAN_FREQ_RANGE			(2*ATV_FW_SCAN_FREQ_OFFSET/ATV_FW_SCAN_FREQ_STEP)

typedef enum _AUD_TV_SRC
{
    AUD_TV_SRC_IFD=0,
    AUD_TV_SRC_SIF
} AUD_TV_SRC;

typedef enum _AUD_PIN_SRC
{
    AUD_PIN_SRC_IFD=0,
    AUD_PIN_SRC_SIF
} AUD_PIN_SRC;

typedef enum
{
    AUD_MTS_MONO = 0,
    AUD_MTS_STEREO,
    AUD_MTS_SAP,
    AUD_MTS_DUAL,
    AUD_MTS_AUTO,
    AUD_MTS_UNKNOWN
} AUD_MTS;

typedef enum
{
    AUD_SOUND_SELECT_FMAM = 0,
    AUD_SOUND_SELECT_STEREO_AB,
    AUD_SOUND_SELECT_STEREO_A,
    AUD_SOUND_SELECT_STEREO_B,
} AUD_SOUND_SELECT;

// Las Vegas ---
typedef enum
{
    ATV_HW_SCAN_FRQ_OFFSET_M50K,
    ATV_HW_SCAN_FRQ_OFFSET_P50K,
    ATV_HW_SCAN_FRQ_OFFSET_M100K,
    ATV_HW_SCAN_FRQ_OFFSET_P100K,
    ATV_HW_SCAN_FRQ_OFFSET_M150K,
    ATV_HW_SCAN_FRQ_OFFSET_P150K,
    ATV_HW_SCAN_FRQ_OFFSET_M200K,
    ATV_HW_SCAN_FRQ_OFFSET_P200K
} ATV_HW_SCAN_FRQ_OFFSET;

typedef enum
{
    // identical to the order in register's field
    ATV_PHASE_DELTA_M50K  = 0x00,
    ATV_PHASE_DELTA_P50K  = 0x01,
    ATV_PHASE_DELTA_M100K = 0x02,
    ATV_PHASE_DELTA_P100K = 0x03,
    ATV_PHASE_DELTA_M150K = 0x04,
    ATV_PHASE_DELTA_P150K = 0x05,
    ATV_PHASE_DELTA_M200K = 0x06,
    ATV_PHASE_DELTA_P200K = 0x07
} ATV_PHASE_DELTA;

typedef enum
{
    // identical to the order in register's field
    ATV_A2_BW_SEL_50KHZ  = 0x00,
    ATV_A2_BW_SEL_70KHZ  = 0x01,
    ATV_A2_BW_SEL_100KHZ = 0x02,
    ATV_A2_BW_SEL_150KHZ = 0x03
} ATV_A2_BW_SEL;

typedef enum
{
    // identical to the order in register's field
    ATV_DEEMPHASIS_50US    = 0x00,
    ATV_DEEMPHASIS_75US    = 0x01,
    ATV_DEEMPHASIS_DISABLE = 0x02
} ATV_DEEMPHASIS_TIME;

typedef enum
{
    // identical to the order in register's field
    ATV_FM1_DEVIATION_16KHZ  = 0x00,
    ATV_FM1_DEVIATION_25KHZ  = 0x01,
    ATV_FM1_DEVIATION_28KHZ  = 0x02,
    ATV_FM1_DEVIATION_50KHZ  = 0x03,
    ATV_FM1_DEVIATION_100KHZ = 0x04,
    ATV_FM1_DEVIATION_150KHZ = 0x05,
    ATV_FM1_DEVIATION_180KHZ = 0x06
} ATV_FM1_DEVIATION_GAIN;

typedef enum
{
    // identical to the order in register's field
    ATV_FM2_DEVIATION_18KHZ  = 0x00,
    ATV_FM2_DEVIATION_25KHZ  = 0x01,
    ATV_FM2_DEVIATION_28KHZ  = 0x02,
    ATV_FM2_DEVIATION_50KHZ  = 0x03,
    ATV_FM2_DEVIATION_100KHZ = 0x04,
    ATV_FM2_DEVIATION_150KHZ = 0x05,
    ATV_FM2_DEVIATION_180KHZ = 0x06
} ATV_FM2_DEVIATION_GAIN;

typedef enum
{
    // identical to the order in register's field
    ATV_EIAJ_DEVIATION_10KHZ = 0x00,
    ATV_EIAJ_DEVIATION_20KHZ = 0x01
} ATV_EIAJ_DEVIATION;

typedef enum
{
    // identical to the order in register's field
    ATV_AM_PRESCALE_0DB  = 0x00,
    ATV_AM_PRESCALE_6DB  = 0x01,
    ATV_AM_PRESCALE_12DB = 0x02,
    ATV_AM_PRESCALE_18DB = 0x03
} ATV_AM_PRESCALE;

typedef enum
{
    // identical to the order in register's field
    ATV_NICAM_BW_250KHZ = 0x00,
    ATV_NICAM_BW_350KHZ = 0x01
} ATV_NICAM_BW;

typedef enum
{
    // identical to the order in register's field
    ATV_SCAN_FRQ_OFFSET_M50K  = 0x00,
    ATV_SCAN_FRQ_OFFSET_P50K  = 0x01,
    ATV_SCAN_FRQ_OFFSET_M100K = 0x02,
    ATV_SCAN_FRQ_OFFSET_P100K = 0x03,
    ATV_SCAN_FRQ_OFFSET_M150K = 0x04,
    ATV_SCAN_FRQ_OFFSET_P150K = 0x05,
    ATV_SCAN_FRQ_OFFSET_M200K = 0x06,
    ATV_SCAN_FRQ_OFFSET_P200K = 0x07
} ATV_SCAN_FRQ_OFFSET;

typedef enum
{
    // identical to the order in register's field
    ATV_MODE_HI_TH_1P25T = 0x00,
    ATV_MODE_HI_TH_1P50T = 0x01,
    ATV_MODE_HI_TH_2P00T = 0x02,
    ATV_MODE_HI_TH_2P50T = 0x03
} ATV_MODE_HI_TH;

typedef enum
{
    // identical to the order in register's field
    ATV_MODE_LO_TH_1P00T = 0x00,
    ATV_MODE_LO_TH_1P25T = 0x01,
    ATV_MODE_LO_TH_1P50T = 0x02,
    ATV_MODE_LO_TH_2P00T = 0x03
} ATV_MODE_LO_TH;

typedef enum
{
    // identical to the order in register's field
    ATV_MODE_ACCU_16  = 0x00,
    ATV_MODE_ACCU_32  = 0x01,
    ATV_MODE_ACCU_64  = 0x02,
    ATV_MODE_ACCU_128 = 0x03
} ATV_MODE_ACCU;

typedef struct
{
    UINT8 debounce_cnt;
    UINT8 hi_threshold;
    UINT8 lo_threshold;
    UINT8 accu_num;
} ATV_MODE_TAB;

typedef struct
{
    UINT8 resvd:2;
    UINT8 nicam_psf:1;
    UINT8 eiaj_sub:1;
    UINT8 a2_bw_sel:2;
    UINT8 de_emphasis:2;

    UINT8 fm1_dev_gain:3;
    UINT8 fm2_dev_gain:3;
    UINT8 hw_am_prescale:2;
} ATV_PSDE_TAB;


typedef enum
{
    // this is for ATV_CFG_FW_SCAN_ALGO_DRAGON only
    ATV_FW_SCAN_OFFSET_P500K = (0x01<<0),
    ATV_FW_SCAN_OFFSET_P400K = (0x01<<1),
    ATV_FW_SCAN_OFFSET_P300K = (0x01<<2),
    ATV_FW_SCAN_OFFSET_P200K = (0x01<<3),
    ATV_FW_SCAN_OFFSET_P100K = (0x01<<4),
    ATV_FW_SCAN_OFFSET_ZERO  = (0x01<<5),
    ATV_FW_SCAN_OFFSET_M100K = (0x01<<6),
    ATV_FW_SCAN_OFFSET_M200K = (0x01<<7),
    ATV_FW_SCAN_OFFSET_M300K = (0x01<<8),
    ATV_FW_SCAN_OFFSET_M400K = (0x01<<9),
    ATV_FW_SCAN_OFFSET_M500K = (0x01<<10),
} ATV_FW_SCAN_OFFSET;

typedef enum
{
    ATV_FW_SCAN_SC1_M  = 4500,
    ATV_FW_SCAN_SC1_DK = 6500,
    ATV_FW_SCAN_SC1_BG = 5500,
    ATV_FW_SCAN_SC1_I  = 6000,
    ATV_FW_SCAN_SC1_L  = 6000,
} ATV_FW_SCAN_SC1;

typedef enum
{
    ATV_FW_SCAN_SC2_M           = 4724,
    ATV_FW_SCAN_SC2_DK1         = 6257,
    ATV_FW_SCAN_SC2_DK2         = 6742,
    ATV_FW_SCAN_SC2_DK3         = 6742,
    ATV_FW_SCAN_SC2_BG          = 5742,
    ATV_FW_SCAN_SC2_NICAM_I     = 6552,
    ATV_FW_SCAN_SC2_NICAM_BGDKL = 5850,
} ATV_FW_SCAN_SC2;

/*============ Prototype of Driver Routines ===================*/
/*=============================================================*/
/**
 * drv_audio_enable_tvaudio
 * enable/disable TV audio auto standard detection mechanism and related interrupts
 *
 * @param <isEnable>	{ enable/disable }
 * @param <std>			{ TV sound standard }
 * @return				{ void }
 * @ingroup drv_adc
 */
void drv_audio_enable_tvaudio(UINT32 isEnable, UINT32 std);

/*=============================================================*/
/**
 * drv_audio_tvstandard_change_int_handler
 * TV audio standard change interrupt service routine
 *
 * @param <void>	{ void }
 * @return 			{ void }
 * @ingroup drv_adc
 */
void drv_audio_tvstandard_change_int_handler();

/*=============================================================*/
/**
 * drv_audio_tvmode_change_int_handler
 * TV audio sound mode change interrupt service routine
 * In this version, handler only clear the interrupt pending flag
 *
 * @param <void>	{ void }
 * @return 			{ void }
 * @ingroup drv_adc
 */
void drv_audio_tvmode_change_int_handler();

/*=============================================================*/
/**
 * drv_audio_init_tvaudio
 * init. TV audio module(registers, variables, interrupts)
 *
 * @param <void>	{ void }
 * @return 			{ void }
 * @ingroup drv_adc
 */
void drv_audio_init_tvaudio();


// Las Vegas ---
// move to audio_api.h

#endif // _AUDIO_TV_H_


