/*=============================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2007 *
 * All rights reserved.                                       *
 *============================================================*/
/*======================= Description ========================*/
/**
 * @file  audio_tv.c
 *
 * @author
 * @date
 * @version 0.1
 * @ingroup drv_adc
 *
 */
  #include "autoconf.h"
 #ifdef _VIDEO_TV_SUPPORT
/*====================== Include files ========================*/
#include "sparrow.h"
#include "timer_event.h"
#include "ROSPrintf.h"
#include "audio/audio_api.h"
#include "audio_reg.h"
#include "audio_host.h"
#include "audio_tv.h"
#include "audio_if.h"
//#include "audio_hwp.h"
#include "audio_hwp_lv.h"



/*======================= Type Defines ========================*/
// bit0 - bit6, bit-mapping
#define ATV_INIT_SUPPORT_STD_MASK		(0x2FUL<<0)
#define ATV_INIT_SUPPORT_STD_BTSC		(0x01UL<<0)
#define ATV_INIT_SUPPORT_STD_A2M		(0x01UL<<1)
#define ATV_INIT_SUPPORT_STD_BG			(0x01UL<<2)
#define ATV_INIT_SUPPORT_STD_DK			(0x01UL<<3)
#define ATV_INIT_SUPPORT_STD_L			(0x01UL<<4)
#define ATV_INIT_SUPPORT_STD_I			(0x01UL<<5)
#define ATV_INIT_SUPPORT_STD_NICAM		(0x01UL<<6)
#define ATV_INIT_SUPPORT_STD_ALL		(0x3FUL<<0)
// bit7 - bit8
#define ATV_INIT_DEV_M_MASK				(0x03UL<<7)
#define ATV_INIT_DEV_M_NORMAL			(0x00UL<<7)
#define ATV_INIT_DEV_M_HIGH				(0x01UL<<7)
#define ATV_INIT_DEV_M_SUPER			(0x02UL<<7)
#define ATV_INIT_DEV_M_ULTRA			(0x03UL<<7)
// bit9 - bit10
#define ATV_INIT_DEV_BGDKI_MASK			(0x03UL<<9)
#define ATV_INIT_DEV_BGDKI_NORMAL		(0x00UL<<9)
#define ATV_INIT_DEV_BGDKI_HIGH			(0x01UL<<9)
#define ATV_INIT_DEV_BGDKI_SUPER		(0x02UL<<9)
#define ATV_INIT_DEV_BGDKI_ULTRA		(0x03UL<<9)
// bit11
#define ATV_INIT_HDEV_CARRIER_DETECT	(0x01UL<<11)
// bit12 - bit13
#define ATV_INIT_FREQ_SHIFT_MASK		(0x03UL<<12)
#define ATV_INIT_FREQ_SHIFT_DISABLE		(0x00UL<<12)
#define ATV_INIT_FREQ_SHIFT_150K		(0x01UL<<12)
#define ATV_INIT_FREQ_SHIFT_300K		(0x02UL<<12)
#define ATV_INIT_FREQ_SHIFT_500K		(0x03UL<<12)
// bit14
#define ATV_INIT_SOUND_MODE_AUTO_DETECT		(0x01UL<<14)
// bit15
#define ATV_INIT_SOUND_MODE_BGDKI_VOL_COMP	(0x01UL<<15)
// bit16
#define ATV_INIT_SOUND_MODE_M_VOL_COMP		(0x01UL<<16)
// bit17
#define ATV_INIT_SOUND_STD_AUTO_SCAN		(0x01UL<<17)
// bit18
#define ATV_INIT_SOUND_STD_HW_DETECT		(0x01UL<<18)
// bit19 - bit22
#define ATV_INIT_ADAPATIVE_SC1_THD_4P5MHZ	(0x01UL<<19)
#define ATV_INIT_ADAPATIVE_SC1_THD_5P5MHZ	(0x01UL<<20)
#define ATV_INIT_ADAPATIVE_SC1_THD_6P0MHZ	(0x01UL<<21)
#define ATV_INIT_ADAPATIVE_SC1_THD_6P5MHZ	(0x01UL<<22)
// bit23
#define ATV_INIT_DATA_SOURCE_MASK			(0x01UL<<23)
#define ATV_INIT_DATA_SOURCE_IFD			(0x00UL<<23)
#define ATV_INIT_DATA_SOURCE_SIF			(0x01UL<<23)
// bit24
#define ATV_INIT_PRESCAN_STD_MASK			(0x01UL<<24)
#define ATV_INIT_PRESCAN_NONE				(0x00UL<<24)
#define ATV_INIT_PRESCAN_400K				(0x01UL<<24)
// bit25 - bit30, reserved
// bit31
#define ATV_INIT_BTSC_LANG_SELECT			(0x01UL<<31)

#ifdef CONFIG_AUDIO_ENABLE_ATV_LOG
#define AUDIO_LOG	RTD_Log
#else
#define AUDIO_LOG	//
#endif

/*===================== Global Variable =======================*/
UINT8 code m_sound_select_btsc_map[4][4] =
{
    {	// Mono
        ATV_BTSC_MTS_SEL_MONO,
        ATV_BTSC_MTS_SEL_MONO,
        ATV_BTSC_MTS_SEL_MONO,
        ATV_BTSC_MTS_SEL_MONO
    },
    {	// Stereo
        ATV_BTSC_MTS_SEL_MONO,
        ATV_BTSC_MTS_SEL_STEREO,
        ATV_BTSC_MTS_SEL_STEREO,
        ATV_BTSC_MTS_SEL_STEREO
    },
    {	// Mono+SAP
        ATV_BTSC_MTS_SEL_L_MONO_R_SAP,
        ATV_BTSC_MTS_SEL_L_MONO_R_SAP,
        ATV_BTSC_MTS_SEL_MONO,
        ATV_BTSC_MTS_SEL_SAP
    },
    {	// Stereo+SAP
        ATV_BTSC_MTS_SEL_SAP,
        ATV_BTSC_MTS_SEL_MONO/*ATV_BTSC_MTS_SEL_STEREO*/,
        ATV_BTSC_MTS_SEL_STEREO,
        ATV_BTSC_MTS_SEL_SAP
    }
};

ATV_MODE_TAB code atv_mode_tab[16] =
{
    /*
     * LasVegas can detect 16 different standards.
     * In the following setting,BTSC and MONO standards are useless
     * but we keep these fields for being easy searching algo.
     */
    {	0,	0,	0,	0},// AUDIO_SOUND_STD_MN_MONO
    {	0,	0,	0,	0},// AUDIO_SOUND_STD_BTSC
    {	4,	ATV_MODE_HI_TH_2P00T,	ATV_MODE_LO_TH_1P50T,	ATV_MODE_ACCU_64},// AUDIO_SOUND_STD_A2_M
    {	4,	ATV_MODE_HI_TH_2P00T,	ATV_MODE_LO_TH_1P50T,	ATV_MODE_ACCU_64},// AUDIO_SOUND_STD_EIAJ
    {	0,	0,	0,	0},// AUDIO_SOUND_STD_BG_MONO
    {	4,	ATV_MODE_HI_TH_2P00T,	ATV_MODE_LO_TH_1P50T,	ATV_MODE_ACCU_64},// AUDIO_SOUND_STD_A2_BG
    {	4,	0,	0,	0},// AUDIO_SOUND_STD_NICAM_BG
    {	0,	0,	0,	0},// AUDIO_SOUND_STD_DK_MONO
    {	4,	ATV_MODE_HI_TH_2P00T,	ATV_MODE_LO_TH_1P50T,	ATV_MODE_ACCU_64},// AUDIO_SOUND_STD_A2_DK1
    {	4,	ATV_MODE_HI_TH_2P00T,	ATV_MODE_LO_TH_1P50T,	ATV_MODE_ACCU_64},// AUDIO_SOUND_STD_A2_DK2
    {	4,	ATV_MODE_HI_TH_2P00T,	ATV_MODE_LO_TH_1P50T,	ATV_MODE_ACCU_64},// AUDIO_SOUND_STD_A2_DK3
    {	4,	0,	0,	0},// AUDIO_SOUND_STD_NICAM_DK
    {	0,	0,	0,	0},// AUDIO_SOUND_STD_AM_MONO
    {	4,	0,	0,	0},// AUDIO_SOUND_STD_NICAM_L
    {	0,	0,	0,	0},// AUDIO_SOUND_STD_FM_MONO_NO_I
    {	4,	0,	0,	0} // AUDIO_SOUND_STD_NICAM_I
};

ATV_PSDE_TAB code atv_psde_tab[2][16] =
{
    {	// hdev_mode == 0
        // AUDIO_SOUND_STD_MN_MONO
        {                  0, ATV_NICAM_BW_250KHZ,      ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_70KHZ,
            ATV_DEEMPHASIS_75US, ATV_FM1_DEVIATION_50KHZ,  ATV_FM2_DEVIATION_50KHZ,  ATV_AM_PRESCALE_0DB },
        // AUDIO_SOUND_STD_BTSC
        {                  0, ATV_NICAM_BW_250KHZ,      ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_70KHZ,
                           ATV_DEEMPHASIS_75US, ATV_FM1_DEVIATION_50KHZ,  ATV_FM2_DEVIATION_50KHZ,  ATV_AM_PRESCALE_0DB },
        // AUDIO_SOUND_STD_A2_M
        {                  0, ATV_NICAM_BW_250KHZ,      ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_70KHZ,
                           ATV_DEEMPHASIS_75US, ATV_FM1_DEVIATION_50KHZ,  ATV_FM2_DEVIATION_50KHZ,  ATV_AM_PRESCALE_0DB },
        // AUDIO_SOUND_STD_EIAJ
        {                  0, ATV_NICAM_BW_250KHZ,      ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_70KHZ,
                           ATV_DEEMPHASIS_75US, ATV_FM1_DEVIATION_50KHZ,  ATV_FM2_DEVIATION_50KHZ,  ATV_AM_PRESCALE_0DB },
        // AUDIO_SOUND_STD_BG_MONO
        {                  0, ATV_NICAM_BW_250KHZ,      ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_100KHZ,
                           ATV_DEEMPHASIS_50US, ATV_FM1_DEVIATION_100KHZ, ATV_FM2_DEVIATION_100KHZ, ATV_AM_PRESCALE_0DB },
        // AUDIO_SOUND_STD_A2_BG
        {                  0, ATV_NICAM_BW_250KHZ,      ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_100KHZ,
                           ATV_DEEMPHASIS_50US, ATV_FM1_DEVIATION_100KHZ, ATV_FM2_DEVIATION_100KHZ, ATV_AM_PRESCALE_0DB },
        // AUDIO_SOUND_STD_NICAM_BG
        {                  0, ATV_NICAM_BW_250KHZ,      ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_100KHZ,
                           ATV_DEEMPHASIS_50US, ATV_FM1_DEVIATION_100KHZ, ATV_FM2_DEVIATION_100KHZ, ATV_AM_PRESCALE_0DB },
        // AUDIO_SOUND_STD_DK_MONO
        {                  0, ATV_NICAM_BW_250KHZ,      ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_100KHZ,
                           ATV_DEEMPHASIS_50US, ATV_FM1_DEVIATION_100KHZ, ATV_FM2_DEVIATION_100KHZ, ATV_AM_PRESCALE_0DB },
        // AUDIO_SOUND_STD_A2_DK1
        {                  0, ATV_NICAM_BW_250KHZ,      ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_100KHZ,
                           ATV_DEEMPHASIS_50US, ATV_FM1_DEVIATION_100KHZ, ATV_FM2_DEVIATION_100KHZ, ATV_AM_PRESCALE_0DB },
        // AUDIO_SOUND_STD_A2_DK2
        {                  0, ATV_NICAM_BW_250KHZ,      ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_100KHZ,
                           ATV_DEEMPHASIS_50US, ATV_FM1_DEVIATION_100KHZ, ATV_FM2_DEVIATION_100KHZ, ATV_AM_PRESCALE_0DB },
        // AUDIO_SOUND_STD_A2_DK3
        {                  0, ATV_NICAM_BW_250KHZ,      ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_100KHZ,
                           ATV_DEEMPHASIS_50US, ATV_FM1_DEVIATION_100KHZ, ATV_FM2_DEVIATION_100KHZ, ATV_AM_PRESCALE_0DB },
        // AUDIO_SOUND_STD_NICAM_DK
        {                  0, ATV_NICAM_BW_250KHZ,      ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_100KHZ,
                           ATV_DEEMPHASIS_50US, ATV_FM1_DEVIATION_100KHZ, ATV_FM2_DEVIATION_100KHZ, ATV_AM_PRESCALE_0DB },
        // AUDIO_SOUND_STD_AM_MONO
        {                  0, ATV_NICAM_BW_250KHZ,      ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_70KHZ,
                           ATV_DEEMPHASIS_75US, ATV_FM1_DEVIATION_50KHZ,  ATV_FM2_DEVIATION_50KHZ,  ATV_AM_PRESCALE_6DB },
        // AUDIO_SOUND_STD_NICAM_L
        {                  0, ATV_NICAM_BW_250KHZ,      ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_70KHZ,
                           ATV_DEEMPHASIS_75US, ATV_FM1_DEVIATION_50KHZ,  ATV_FM2_DEVIATION_50KHZ,  ATV_AM_PRESCALE_6DB },
        // AUDIO_SOUND_STD_FM_MONO_NO_I
        {                  0, ATV_NICAM_BW_250KHZ,      ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_100KHZ,
                           ATV_DEEMPHASIS_50US, ATV_FM1_DEVIATION_100KHZ, ATV_FM2_DEVIATION_100KHZ, ATV_AM_PRESCALE_0DB },
        // AUDIO_SOUND_STD_NICAM_I
        {                  0, ATV_NICAM_BW_350KHZ,      ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_100KHZ,
                           ATV_DEEMPHASIS_50US, ATV_FM1_DEVIATION_100KHZ, ATV_FM2_DEVIATION_100KHZ, ATV_AM_PRESCALE_0DB },
    },
    {	// hedv_mode == 1
        // AUDIO_SOUND_STD_MN_MONO
        {                  0, ATV_NICAM_BW_250KHZ,       ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_100KHZ,
            ATV_DEEMPHASIS_75US, ATV_FM1_DEVIATION_100KHZ,  ATV_FM2_DEVIATION_100KHZ, ATV_AM_PRESCALE_0DB },
        // AUDIO_SOUND_STD_BTSC
        {                  0, ATV_NICAM_BW_250KHZ,       ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_100KHZ,
                           ATV_DEEMPHASIS_75US, ATV_FM1_DEVIATION_100KHZ,  ATV_FM2_DEVIATION_100KHZ, ATV_AM_PRESCALE_0DB },
        // AUDIO_SOUND_STD_A2_M
        {                  0, ATV_NICAM_BW_250KHZ,       ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_100KHZ,
                           ATV_DEEMPHASIS_75US, ATV_FM1_DEVIATION_100KHZ,  ATV_FM2_DEVIATION_100KHZ, ATV_AM_PRESCALE_0DB },
        // AUDIO_SOUND_STD_EIAJ
        {                  0, ATV_NICAM_BW_250KHZ,       ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_100KHZ,
                           ATV_DEEMPHASIS_75US, ATV_FM1_DEVIATION_100KHZ,  ATV_FM2_DEVIATION_100KHZ, ATV_AM_PRESCALE_0DB },
        // AUDIO_SOUND_STD_BG_MONO
        {                  0, ATV_NICAM_BW_250KHZ,       ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_150KHZ,
                           ATV_DEEMPHASIS_50US, ATV_FM1_DEVIATION_180KHZ,  ATV_FM2_DEVIATION_180KHZ, ATV_AM_PRESCALE_0DB },
        // AUDIO_SOUND_STD_A2_BG
        {                  0, ATV_NICAM_BW_250KHZ,       ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_150KHZ,
                           ATV_DEEMPHASIS_50US, ATV_FM1_DEVIATION_180KHZ,  ATV_FM2_DEVIATION_180KHZ, ATV_AM_PRESCALE_0DB },
        // AUDIO_SOUND_STD_NICAM_BG
        {                  0, ATV_NICAM_BW_250KHZ,       ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_150KHZ,
                           ATV_DEEMPHASIS_50US, ATV_FM1_DEVIATION_180KHZ,  ATV_FM2_DEVIATION_180KHZ, ATV_AM_PRESCALE_0DB },
        // AUDIO_SOUND_STD_DK_MONO
        {                  0, ATV_NICAM_BW_250KHZ,       ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_150KHZ,
                           ATV_DEEMPHASIS_50US, ATV_FM1_DEVIATION_180KHZ,  ATV_FM2_DEVIATION_180KHZ, ATV_AM_PRESCALE_0DB },
        // AUDIO_SOUND_STD_A2_DK1
        {                  0, ATV_NICAM_BW_250KHZ,       ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_150KHZ,
                           ATV_DEEMPHASIS_50US, ATV_FM1_DEVIATION_180KHZ,  ATV_FM2_DEVIATION_180KHZ, ATV_AM_PRESCALE_0DB },
        // AUDIO_SOUND_STD_A2_DK2
        {                  0, ATV_NICAM_BW_250KHZ,       ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_150KHZ,
                           ATV_DEEMPHASIS_50US, ATV_FM1_DEVIATION_180KHZ,  ATV_FM2_DEVIATION_180KHZ, ATV_AM_PRESCALE_0DB },
        // AUDIO_SOUND_STD_A2_DK3
        {                  0, ATV_NICAM_BW_250KHZ,       ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_150KHZ,
                           ATV_DEEMPHASIS_50US, ATV_FM1_DEVIATION_180KHZ,  ATV_FM2_DEVIATION_180KHZ, ATV_AM_PRESCALE_0DB },
        // AUDIO_SOUND_STD_NICAM_DK
        {                  0, ATV_NICAM_BW_250KHZ,       ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_150KHZ,
                           ATV_DEEMPHASIS_50US, ATV_FM1_DEVIATION_180KHZ,  ATV_FM2_DEVIATION_180KHZ, ATV_AM_PRESCALE_0DB },
        // AUDIO_SOUND_STD_AM_MONO
        {                  0, ATV_NICAM_BW_250KHZ,       ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_70KHZ,
                           ATV_DEEMPHASIS_75US, ATV_FM1_DEVIATION_50KHZ,   ATV_FM2_DEVIATION_50KHZ,  ATV_AM_PRESCALE_6DB },
        // AUDIO_SOUND_STD_NICAM_L
        {                  0, ATV_NICAM_BW_250KHZ,       ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_70KHZ,
                           ATV_DEEMPHASIS_75US, ATV_FM1_DEVIATION_50KHZ,   ATV_FM2_DEVIATION_50KHZ,  ATV_AM_PRESCALE_6DB },
        // AUDIO_SOUND_STD_FM_MONO_NO_I
        {                  0, ATV_NICAM_BW_250KHZ,       ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_100KHZ,
                           ATV_DEEMPHASIS_50US, ATV_FM1_DEVIATION_100KHZ,  ATV_FM2_DEVIATION_100KHZ, ATV_AM_PRESCALE_0DB },
        // AUDIO_SOUND_STD_NICAM_I
        {                  0, ATV_NICAM_BW_350KHZ,       ATV_EIAJ_DEVIATION_20KHZ, ATV_A2_BW_SEL_100KHZ,
                           ATV_DEEMPHASIS_50US, ATV_FM1_DEVIATION_100KHZ,  ATV_FM2_DEVIATION_100KHZ, ATV_AM_PRESCALE_0DB },
    }
};

//INT8 code	atv_fw_scan_freq_tab[ATV_FW_SCAN_FREQ_RANGE] = {-5, 5, 0, -4, 4, -5, 5, 0, -3, 3, -5, 5, 0, -2, 2, -5, 5, 0, -1, 1};

UINT32	atv_flag;
UINT16	atv_fw_scan_mag[ATV_FW_SCAN_FREQ_RANGE];

UINT16	m_c6p5mhz_threshold_high;	// DK/L
UINT16	m_c6p0mhz_threshold_high;	// I
UINT16	m_c5p5mhz_threshold_high;	// BG
UINT16	m_c4p5mhz_threshold_high;	// MN
UINT16	m_c6p5mhz_threshold_low;
UINT16	m_c6p0mhz_threshold_low;
UINT16	m_c5p5mhz_threshold_low;
UINT16	m_c4p5mhz_threshold_low;

extern UINT16 code m_c6p5mhz_threshold_high_tab[3];
extern UINT16 code m_c6p0mhz_threshold_high_tab[3];
extern UINT16 code m_c5p5mhz_threshold_high_tab[3];
extern UINT16 code m_c4p5mhz_threshold_high_tab[3];
extern UINT16 code m_c6p5mhz_threshold_low_tab[3];
extern UINT16 code m_c6p0mhz_threshold_low_tab[3];
extern UINT16 code m_c5p5mhz_threshold_low_tab[3];
extern UINT16 code m_c4p5mhz_threshold_low_tab[3];

extern UINT16 code m_c4p72mhz_threshold;	// dragon use 0xFFFF for mono sound
extern UINT16 code m_c5p74mhz_threshold;
extern UINT16 code m_c5p85mhz_threshold;
extern UINT16 code m_c6p25mhz_threshold;
extern UINT16 code m_c6p74mhz_threshold;
extern UINT16 code m_c6p55mhz_threshold; // dragon use 0xFFFF for mono sound
extern UINT32 code m_c5p0mhz_threshold;
extern UINT16 code m_c6p8mhz_threshold;

extern UINT16 code m_btsc_pilot_threshold_high;
extern UINT16 code m_btsc_pilot_threshold_low;
extern UINT16 code m_btsc_sap_threshold_high;
extern UINT16 code m_btsc_sap_threshold_low;
extern UINT16 code m_btsc_sap_noise_threshold_high;
extern UINT16 code m_btsc_sap_noise_threshold_low;

extern INT16 code m_vol_comp_btsc_mono;
extern INT16 code m_vol_comp_btsc_sap;
extern INT16 code m_vol_comp_btsc_stereo;

static void AtvSetIfAdcPower(BOOL para);
static void AtvSetHwAGC(BOOL para);
static void AtvSetHighDeviation(UINT8 sound_std);
static UINT8 AtvGetHighDeviation(void);
static void AtvSetPrescaleDeemphasis(UINT8 sound_std);
static void AtvSetModeDebounceCoef(UINT8 sound_std);
static void AtvSetFwCarrierFreq(BOOL para, UINT16 fc_main, UINT16 fc_sub);
static void AtvFwScanInit(BOOL para);
static void AtvFwScanCarrierOffset(void);
static UINT8 AtvFwScanNicam(void);
static void AtvEnableNicamHiBitErrInt(void);
static void AtvSetSoundStd(UINT8 sound_std);
static void AtvSetAdaptiveCarrierThreshold(BOOL para);
static void AtvSetMute(BOOL para, UINT8 ch_id);
/*====================== API Routines =========================*/
extern void timerevent_table_audio_atv_freq_scan_event(void);
extern void timerevent_table_audio_atv_nicam_check_event(void);


/*==================== Driver Routines ========================*/

/*===================== Local Routines ========================*/


/*=============================================================*/
/**
 * Init. Spaarrow ATV Audio
 *
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AtvInit()
{
#ifdef CONFIG_ENABLE_AUDIO
    UINT32 NICAM_AUTO_REG;

    Audio_EnterCriticalSection();

    atv_flag = 0;

#if defined(CONFIG_AUDIO_ATV_SUPPORT_BGDKI)
    atv_flag |= ATV_INIT_SUPPORT_STD_BG|ATV_INIT_SUPPORT_STD_DK|ATV_INIT_SUPPORT_STD_I;
    atv_flag &= ~ATV_INIT_DEV_BGDKI_MASK;
#if defined(CONFIG_AUDIO_ATV_SUPPORT_BGDKI_DEV_MODE_HIGH)
    atv_flag |= ATV_INIT_DEV_BGDKI_HIGH;
#elif defined(CONFIG_AUDIO_ATV_SUPPORT_BGDKI_DEV_MODE_SUPER)
    atv_flag |= ATV_INIT_DEV_BGDKI_SUPER;
#endif
#endif

#if defined(CONFIG_AUDIO_ATV_SUPPORT_L)
    atv_flag |= ATV_INIT_SUPPORT_STD_L;
#endif

#if defined(CONFIG_AUDIO_ATV_SUPPORT_NICAM)
    atv_flag |= ATV_INIT_SUPPORT_STD_NICAM;
#endif

#if defined(CONFIG_AUDIO_ATV_SUPPORT_M)
    atv_flag |= ATV_INIT_SUPPORT_STD_A2M;
    atv_flag &= ~ATV_INIT_DEV_M_MASK;
#if defined(CONFIG_AUDIO_ATV_SUPPORT_M_DEV_MODE_HIGH)
    atv_flag |= ATV_INIT_DEV_M_HIGH;
#elif defined(CONFIG_AUDIO_ATV_SUPPORT_M_DEV_MODE_SUPER)
    atv_flag |= ATV_INIT_DEV_M_SUPER;
#endif
#endif

#if defined(CONFIG_AUDIO_ATV_SUPPORT_BTSC)
    atv_flag |= ATV_INIT_SUPPORT_STD_BTSC;
#if defined(CONFIG_AUDIO_ATV_SUPPORT_BTSC_LANGUAGE_SELECT)
    atv_flag |= ATV_INIT_BTSC_LANG_SELECT;
#endif
#if defined(CONFIG_AUDIO_ATV_SUPPORT_BTSC_VOL_COMP)
    atv_flag |= ATV_INIT_SOUND_MODE_M_VOL_COMP;
#endif
#endif

#if defined(CONFIG_AUDIO_ATV_SUPPORT_AUTO_SOUND_MODE)
    atv_flag |= ATV_INIT_SOUND_MODE_AUTO_DETECT;
#endif


#if defined(CONFIG_AUDIO_ATV_SUPPORT_FREQ_SHIFT)
    atv_flag |= ATV_INIT_FREQ_SHIFT_500K;
    atv_flag &= ~ATV_INIT_PRESCAN_STD_MASK;

    if ((CONFIG_AUDIO_ATV_FREQ_SHIFT_RANGE < 50) || ((CONFIG_AUDIO_ATV_FREQ_SHIFT_RANGE%50) != 0))
    {
        RTD_Log(LOGGER_INFO,"Audio ATV Fatal Error!!!\n");
        while (1);
    }

#if defined(CONFIG_AUDIO_ATV_FREQ_SHIFT_PRESCAN)
    atv_flag |= ATV_INIT_PRESCAN_400K;
#endif
#endif

    // stop clk and enable
//	drv_audio_enable_tvaudio(AUD_DISABLE,0x1F);


    // TODO: all these threshold setting is according to Dragon project
    // BTSC-related
    rtd_outl(BTSC_PILOT_THRES_0x3400, ((UINT32)m_btsc_pilot_threshold_high<<16) | m_btsc_pilot_threshold_low);
    rtd_outl(BTSC_SAP_THRES_0x3408, ((UINT32)m_btsc_sap_threshold_high<<16) | m_btsc_sap_threshold_low);
    rtd_outl(BTSC_SAP_NOISE_TH_0x3410, ((UINT32)m_btsc_sap_noise_threshold_high<<16) | m_btsc_sap_noise_threshold_low);

    // threshold setting
    AtvSetAdaptiveCarrierThreshold(_DISABLE);
    rtd_outl(MSD_CARRIER_4_72_0x3210, ((UINT32)m_c4p72mhz_threshold<<16));	// sub-carrier of A2-M
#if defined( CONFIG_ENABLE_AUDIOTV_HDEV_MODE_PAL) && defined(CONFIG_ENABLE_AUDIOTV_SATUS_DETECT_HDEVMODE_PAL)
    rtd_outl(MSD_CARRIER_5_74_0x321c, ((UINT32)m_c5p74mhz_threshold<<16));	// sub-carrier of A2-B/G or A2-D/K3
    rtd_outl(MSD_CARRIER_5_85_0x3220, ((UINT32)m_c5p85mhz_threshold<<16));	// sub-carrier of NICAM(B/G or L)
    rtd_outl(MSD_CARRIER_6_25_0x322c, ((UINT32)m_c6p25mhz_threshold<<16));	// sub-carrier of A2-D/K1
    rtd_outl(MSD_CARRIER_6_74_0x3248, ((UINT32)m_c6p74mhz_threshold<<16));	// sub-carrier of A2-D/K2
    rtd_outl(MSD_CARRIER_6_55_0x3240, ((UINT32)m_c6p55mhz_threshold<<16));	// sub-carrier of NICAM(I)
#else
    /* set these only when all analog & mono sound area
    rtd_outl(MSD_CARRIER_5_74_0x321c, ((UINT32)0xFFFF<<16));	// sub-carrier of A2-B/G or A2-D/K3
    rtd_outl(MSD_CARRIER_5_85_0x3220, ((UINT32)0xFFFF<<16));	// sub-carrier of NICAM(B/G or L)
    rtd_outl(MSD_CARRIER_6_25_0x322c, ((UINT32)0xFFFF<<16));	// sub-carrier of A2-D/K1
    rtd_outl(MSD_CARRIER_6_74_0x3248, ((UINT32)0xFFFF<<16));	// sub-carrier of A2-D/K2
    rtd_outl(MSD_CARRIER_6_55_0x3240, ((UINT32)0xFFFF<<16));	// sub-carrier of NICAM(I)
    */
#endif
    rtd_outl(MSD_CARRIER_6_35_0x3230, ((UINT32)0xFFFF<<16));
    rtd_outl(MSD_CARRIER_6_45_0x3234, ((UINT32)0xFFFF<<16));
    rtd_outl(MSD_CARRIER_6_62_0x3244, ((UINT32)0xFFFF<<16));
    rtd_outl(MSD_CARRIER_6_80_0x324c, ((UINT32)0xFFFF<<16));
    /* set these only when all analog & mono sound area
    rtd_outl(MSD_574_585_0x3258, ((UINT32)0xFFFF<<16));			// this is for A2-BG or NIACM-BG/BG-Mono(5.5M)
    rtd_outl(MSD_585_635_0x3260, ((UINT32)0xFFFF<<16));			// this is for NIACM-BG or BG-Mono(5.5M)
    rtd_outl(MSD_585_550_0x325c, ((UINT32)0xFFFF<<16));			// this is for NICAM-DKL or DKL-Mono(6.5M)
    rtd_outl(MSD_625_550_0x3264, ((UINT32)0xFFFF<<16));			// this is for A2-DK1 or NICAM-DKL/DKL-Mono(6.5M)
    rtd_outl(MSD_655_680_0x3268, ((UINT32)0xFFFF<<16));			// this is for NICAM-I or FM-Mono(6.0M)
    */

    // NICAM-related
    NICAM_AUTO_REG = rtd_inl(NICAM_AUTO_SWITCH_0x3518);
    NICAM_AUTO_REG |= _BIT31;	// disable auto-switch
    rtd_outl(NICAM_AUTO_SWITCH_0x3518, NICAM_AUTO_REG);

    // SIF-AGC-related
    AtvSetHwAGC(TRUE);

    // MSD-related
    if ((atv_flag & ATV_INIT_SUPPORT_STD_NICAM) || (atv_flag & ATV_INIT_SUPPORT_STD_BTSC))
    {
        Audio_AtvSetSoundSelect(ATV_SOUND_SEL_STEREO_A);
    }
    else
    {
        Audio_AtvSetSoundSelect(ATV_SOUND_SEL_FMAM);	// force to mono
    }


    if (atv_flag & ATV_INIT_SOUND_STD_HW_DETECT)
    {
        Audio_AtvSetSoundStd(ATV_SOUND_STD_AUTO);
        AtvSetAdaptiveCarrierThreshold(_ENABLE);

        if ((atv_flag & ATV_INIT_DEV_M_MASK) != ATV_INIT_DEV_M_NORMAL)
        {
            Audio_AtvSetMtsPriority(ATV_MTS_PRIO_MONO);
        }
        else
        {
            if (atv_flag & ATV_INIT_SUPPORT_STD_BTSC)
            {
                Audio_AtvSetMtsPriority(ATV_MTS_PRIO_BTSC);
            }
            else
            {
                Audio_AtvSetMtsPriority(ATV_MTS_PRIO_EIAJ);
            }
        }

        if (atv_flag & ATV_INIT_SUPPORT_STD_DK)
        {
            Audio_AtvSetMtsPriority(ATV_MTS_PRIO_DK);
        }
        else
        {
            Audio_AtvSetMtsPriority(ATV_MTS_PRIO_L);
        }
    }
    else
    {
        Audio_AtvSetSoundStd(ATV_SOUND_STD_UNKNOWN);
    }


    if ((atv_flag & ATV_INIT_DATA_SOURCE_MASK) == ATV_INIT_DATA_SOURCE_SIF)
    {
        Audio_AtvSetDataSrc(ATV_DATA_SRC_SIF);
    }
    else
    {
        Audio_AtvSetDataSrc(ATV_DATA_SRC_IFD);
    }

    if (atv_flag & ATV_INIT_SOUND_MODE_AUTO_DETECT)
    {
        Audio_AtvSetSoundMode(ATV_SOUND_MODE_AUTO);
    }
    else
    {
        Audio_AtvSetSoundMode(ATV_SOUND_MODE_MONO);
    }


    AtvSetFwCarrierFreq(_DISABLE, 0, 0);

    Audio_LeaveCriticalSection();

    AUDIO_LOG(LOGGER_INFO,"audio atv_flag = %x\n",(UINT32)atv_flag);

#endif // CONFIG_ENABLE_AUDIO
}

/*========================= Routines ==========================*/
/**
 * Select ATV Sound Standard
 *
 * @param <sound_std>	{ ATV_SOUND_STD_XXX , refer to audio_atv.h }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_AtvSetSoundStd(UINT8 sound_std)
{
#ifndef CONFIG_ENABLE_AUDIO
    sound_std = sound_std;
#else
    msd_ctrl_word_RBUS			MSD_CTRL_REG;
    msd_result_RBUS				MSD_RESULT_REG;
    carrier_freq_deviatio_RBUS	FRQ_DEV_CTRL_REG;
    UINT8						i,j,k;

    AUDIO_LOG(LOGGER_INFO,"Audio_AtvSetSoundStd(%x)\n",(UINT32)sound_std);

    // always disable F/W scaning sound standard task before setting new sound standard
#ifdef ATV_CFG_FW_SCAN_BY_TIMER_EVENT
//	fw_timer_event_CancelTimerEvent(Audio_AtvTimerEventFwScan);
    fw_timer_event_CancelTimerEvent(timerevent_table_audio_atv_freq_scan_event);
#endif

    AtvSetMute(_ENABLE, APP_CH_ID_L | APP_CH_ID_R);

    // turn-off F/W carrier setting
    AtvSetFwCarrierFreq(_DISABLE, 0, 0);

    Audio_AtvSetPlayChannel(ATV_PLAY_CH_SEL_L, ATV_PLAY_CH_SEL_R);	// reset to stereo


    MSD_CTRL_REG.regValue = rtd_inl(MSD_CTRL_WORD_0x3200);

    if (sound_std == ATV_SOUND_STD_AUTO)
    {
        // disable F/W scan anyway.
        AtvFwScanInit(_DISABLE);

        // others msd setting should be set by configuration
        k = MSD_CTRL_REG.bitField.msd_debounce_cnt;
        MSD_CTRL_REG.bitField.msd_debounce_cnt = 0;		// no debounce for trigger
        MSD_CTRL_REG.bitField.msd_en     = TRUE;
        MSD_CTRL_REG.bitField.update_en  = TRUE;		// change MSD_RESULT_REG.reg_std will issue interrupt.
        MSD_CTRL_REG.bitField.fw_scan_en = FALSE;
        rtd_outl(MSD_CTRL_WORD_0x3200, MSD_CTRL_REG.regValue);

        // TODO: check if this will try to trigger an interrupt correctly.
        /*
         * 1. trigger an interrupt by toggling sif_data_src temporarily
         * 2. in the ISR, output will be unmuted and bandend_play will be enable as well.
         */
        FRQ_DEV_CTRL_REG.regValue = rtd_inl(CARRIER_FREQ_DEVIATIO_0x3350);
        i = FRQ_DEV_CTRL_REG.bitField.sif_data_source;
        FRQ_DEV_CTRL_REG.bitField.sif_data_source = (~i)&0x1;
        //AUDIO_LOG(LOGGER_INFO,"sif_data_src = %x\n",(UINT32)FRQ_DEV_CTRL_REG.bitField.sif_data_source);
        rtd_outl(CARRIER_FREQ_DEVIATIO_0x3350, FRQ_DEV_CTRL_REG.regValue);
        for (j=0; j<8; j++)
        {
            AUDIO_LOG(LOGGER_INFO,"wait msd_unknown cnt %x\n",(UINT32)j);
            Audio_TimeDelay_ms(50);
            MSD_RESULT_REG.regValue = rtd_inl(MSD_RESULT_0x3204);
            if (MSD_RESULT_REG.bitField.hw_detected_std == ATV_SOUND_STD_UNKNOWN)
                break;
        }
        MSD_CTRL_REG.bitField.msd_debounce_cnt = k;	// restore origianl debounce cnt
        rtd_outl(MSD_CTRL_WORD_0x3200, MSD_CTRL_REG.regValue);

        FRQ_DEV_CTRL_REG.bitField.sif_data_source = i;
        //AUDIO_LOG(LOGGER_INFO,"sif_data_src = %x\n",(UINT32)FRQ_DEV_CTRL_REG.bitField.sif_data_source);
        rtd_outl(CARRIER_FREQ_DEVIATIO_0x3350, FRQ_DEV_CTRL_REG.regValue);

    }
    else
    {
        /*
         * step 1. use MSD to check if standard exist.
         * step 2. use freq_offset or 2nd_phase to lock standard (move to sub-routine in the future)
         */

        MSD_CTRL_REG.regValue = rtd_inl(MSD_CTRL_WORD_0x3200);
        MSD_CTRL_REG.bitField.msd_en     = TRUE;
        MSD_CTRL_REG.bitField.update_en  = FALSE;		// change MSD_RESULT_REG.reg_std won't issue interrupt.
        MSD_CTRL_REG.bitField.fw_scan_en = FALSE;
        rtd_outl(MSD_CTRL_WORD_0x3200, MSD_CTRL_REG.regValue);

        AtvSetSoundStd(sound_std);

        MSD_RESULT_REG.regValue = rtd_inl(MSD_RESULT_0x3204);

        if (MSD_RESULT_REG.bitField.reg_std != ATV_SOUND_STD_UNKNOWN)
        {
            AtvSetHighDeviation(MSD_RESULT_REG.bitField.reg_std);
            AtvSetPrescaleDeemphasis(MSD_RESULT_REG.bitField.reg_std);
            AtvSetModeDebounceCoef(MSD_RESULT_REG.bitField.reg_std);

            if ( (pb_path_info.src[0] == AIO_PATH_SRC_ATV_NICAM) ||
                    (pb_path_info.src[0] == AIO_PATH_SRC_ATV_HDEV) ||
                    (pb_path_info.src[0] == AIO_PATH_SRC_ATV) )
            {

                if ( (MSD_RESULT_REG.bitField.reg_std == ATV_SOUND_STD_NICAM_BG) ||
                        (MSD_RESULT_REG.bitField.reg_std == ATV_SOUND_STD_NICAM_DK) ||
                        (MSD_RESULT_REG.bitField.reg_std == ATV_SOUND_STD_NICAM_I) ||
                        (MSD_RESULT_REG.bitField.reg_std == ATV_SOUND_STD_NICAM_L) )
                {
                    pb_path_info.curr_ipt_fs = AIO_FS_ID_32K;
                    pb_path_info.src[0] = AIO_PATH_SRC_ATV_NICAM;
                }
                else
                {
                    if (AtvGetHighDeviation() == TRUE)
                    {
                        pb_path_info.curr_ipt_fs = AIO_FS_ID_96K;
                        pb_path_info.src[0] = AIO_PATH_SRC_ATV_HDEV;
                    }
                    else
                    {
                        pb_path_info.curr_ipt_fs = AIO_FS_ID_48K;
                        pb_path_info.src[0] = AIO_PATH_SRC_ATV;
                    }
                }

                Audio_AioSetPathClock(pb_path_info.src[0]);
                Audio_AioSetBackendPlay(_ENABLE);
            }
            else
            {
                // Canal+ application, do not modify clock/PLL setting
            }

            // TODO: check the correction of the result of F/W Scanning mechanism
            if ((atv_flag & ATV_INIT_FREQ_SHIFT_MASK) != ATV_INIT_FREQ_SHIFT_DISABLE)
            {
                AtvFwScanInit(_ENABLE);
            }
            else
            {
                AtvFwScanInit(_DISABLE);
            }

            AtvSetMute(_DISABLE, APP_CH_ID_L | APP_CH_ID_R);

        }

    }

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Select ATV play channel
 * this function only affect the data into post-processing,
 * it cannot control the ATV audio decoder.
 *
 * @param <sound_sel>	{ ATV_PLAY_CH_SEL_XXX , refer to audio_atv.h }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_AtvSetPlayChannel(UINT8 l_sel, UINT8 r_sel)
{
#ifndef CONFIG_ENABLE_AUDIO
    l_sel = l_sel;
    r_sel = r_sel;
#else
    msd_ctrl_word_RBUS		MSD_CTRL_REG;

    MSD_CTRL_REG.regValue = rtd_inl(MSD_CTRL_WORD_0x3200);
    MSD_CTRL_REG.bitField.play_sel_a = l_sel;
    MSD_CTRL_REG.bitField.play_sel_b = r_sel;
    rtd_outl(MSD_CTRL_WORD_0x3200, MSD_CTRL_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Select ATV Sound/Language
 *
 * @param <sound_sel>	{ ATV_SOUND_SEL_XXX , refer to audio_atv.h }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_AtvSetSoundSelect(UINT8 sound_sel)
{
#ifndef CONFIG_ENABLE_AUDIO
    sound_sel = sound_sel;
#else
    msd_ctrl_word_RBUS	MSD_CTRL_REG;

    AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] change SoundSelect to %d\n",(UINT32)sound_sel);

    MSD_CTRL_REG.regValue = rtd_inl(MSD_CTRL_WORD_0x3200);
    MSD_CTRL_REG.bitField.sound_select = sound_sel;
    rtd_outl(MSD_CTRL_WORD_0x3200, MSD_CTRL_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}

#if 0
/**
 * Select ATV BTSC Sound/Language
 *
 * @param <mts_sel>	{ ATV_BTSC_MTS_SEL_XXX , refer to audio_atv.h }
 * @return			{ void }
 * @ingroup lib_audio
 */
void Audio_AtvSetBtscMtsSelect(UINT8 mts_sel)
{
#ifndef CONFIG_ENABLE_AUDIO
    mts_sel = mts_sel;
#else
    mode_detect_RBUS	MODE_DETECT_REG;
    UINT32				sound_sel;

    sound_sel = ATV_SOUND_SEL_STEREO_A;

    MODE_DETECT_REG.regValue = rtd_inl(MODE_DETECT_0x3300);
    switch (MODE_DETECT_REG.bitField.mode_result)
    {
    case 0: // Mono
        break;
    case 1: // Stereo
        if (mts_sel == ATV_BTSC_MTS_SEL_MONO)
            sound_sel = ATV_SOUND_SEL_FMAM;
        break;
    case 2:	// Mono+SAP
        if (mts_sel == ATV_BTSC_MTS_SEL_SAP)
            sound_sel = ATV_SOUND_SEL_STEREO_B;
        else if (mts_sel == ATV_BTSC_MTS_SEL_L_MONO_R_SAP)
            sound_sel = ATV_SOUND_SEL_FMAM;
        break;
    case 3: // Stereo+SAP
        if (mts_sel == ATV_BTSC_MTS_SEL_SAP)
            sound_sel = ATV_SOUND_SEL_STEREO_B;
        else if (mts_sel == ATV_BTSC_MTS_SEL_MONO)
            sound_sel = ATV_SOUND_SEL_STEREO_AB;
        break;
    }

    // TODO: BTSC volume compensation ??!

    if (mts_sel == ATV_BTSC_MTS_SEL_MONO)
        Audio_AtvSetPlayChannel(ATV_PLAY_CH_SEL_L_ADD_R, ATV_PLAY_CH_SEL_L_ADD_R);
    else
        Audio_AtvSetPlayChannel(ATV_PLAY_CH_SEL_L, ATV_PLAY_CH_SEL_R);

    Audio_AtvSetSoundSelect(sound_sel);

#endif // !CONFIG_ENABLE_AUDIO
}
#endif
/**
 * Get ATV BTSC Sound/Language Select Status
 *
 * @param <p_mts_sel>	{ ATV_BTSC_MTS_SEL_XXX , refer to audio_atv.h }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_AtvGetBtscMtsSelect(UINT8 *p_mts_sel)
{
#ifndef CONFIG_ENABLE_AUDIO
    p_mts_sel = p_mts_sel;
#else
    mode_detect_RBUS	MODE_DETECT_REG;
    msd_ctrl_word_RBUS	MSD_CTRL_REG;

    MODE_DETECT_REG.regValue = rtd_inl(MODE_DETECT_0x3300);
    MSD_CTRL_REG.regValue = rtd_inl(MSD_CTRL_WORD_0x3200);

    *p_mts_sel = m_sound_select_btsc_map[MODE_DETECT_REG.bitField.mode_result][MSD_CTRL_REG.bitField.sound_select];

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Select ATV Sound Mode
 *
 * @param <sound_mode>	{ ATV_SOUND_MODE_XXX , refer to audio_atv.h }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_AtvSetSoundMode(UINT8 sound_mode)
{
#ifndef CONFIG_ENABLE_AUDIO
    sound_mode = sound_mode;
#else
    mode_detect_RBUS	MODE_DETECT_REG;

    MODE_DETECT_REG.regValue = rtd_inl(MODE_DETECT_0x3300);

    if (sound_mode == ATV_SOUND_MODE_AUTO)
    {
        MODE_DETECT_REG.bitField.hw_mode_en = TRUE;
    }
    else
    {
        MODE_DETECT_REG.bitField.hw_mode_en = FALSE;
        switch (sound_mode)
        {
        case ATV_SOUND_MODE_STEREO:
            MODE_DETECT_REG.bitField.mode_result = 1;
            break;
        case ATV_SOUND_MODE_DUAL:
        case ATV_SOUND_MODE_SAP_MONO:
            MODE_DETECT_REG.bitField.mode_result = 2;
            break;
        case ATV_SOUND_MODE_SAP_STEREO:
            MODE_DETECT_REG.bitField.mode_result = 3;
            break;
        case ATV_SOUND_MODE_MONO:
        default:
            MODE_DETECT_REG.bitField.mode_result = 0;
            break;
        }
    }

    rtd_outl(MODE_DETECT_0x3300, MODE_DETECT_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Get ATV Sound Mode
 *
 * @param <p_sound_mode>	{ ATV_SOUND_MODE_XXX , refer to audio_atv.h }
 * @return					{ void }
 * @ingroup lib_audio
 */
void Audio_AtvGetSoundMode(UINT8 *p_sound_mode)
{
#ifndef CONFIG_ENABLE_AUDIO
    p_sound_mode = p_sound_mode;
#else
    mode_detect_RBUS	MODE_DETECT_REG;

    MODE_DETECT_REG.regValue = rtd_inl(MODE_DETECT_0x3300);

    *p_sound_mode = MODE_DETECT_REG.bitField.mode_result;
#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Select ATV MTS Priority
 *
 * @param <mts_prio>	{ ATV_MTS_PRIO_XXX , refer to audio_atv.h }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_AtvSetMtsPriority(UINT8 mts_prio)
{
#ifndef CONFIG_ENABLE_AUDIO
    mts_prio = mts_prio;
#else

    msd_ctrl_word_RBUS	MSD_CTRL_REG;

    MSD_CTRL_REG.regValue = rtd_inl(MSD_CTRL_WORD_0x3200);

    switch (mts_prio)
    {
    case ATV_MTS_PRIO_BTSC:
        MSD_CTRL_REG.bitField.mn_priority = ATV_MN_PRIO_BTSC;
        break;
    case ATV_MTS_PRIO_EIAJ:
        MSD_CTRL_REG.bitField.mn_priority = ATV_MN_PRIO_EIAJ;
        break;
    case ATV_MTS_PRIO_MONO:
        MSD_CTRL_REG.bitField.mn_priority = ATV_MN_PRIO_MONO1;
        break;
    case ATV_MTS_PRIO_DK:
        MSD_CTRL_REG.bitField.dkl_priority = ATV_DKL_PRIO_DK;
        break;
    case ATV_MTS_PRIO_L:
        MSD_CTRL_REG.bitField.dkl_priority = ATV_DKL_PRIO_L;
        break;
    default:
        MSD_CTRL_REG.bitField.mn_priority = ATV_MN_PRIO_MONO2;
        MSD_CTRL_REG.bitField.dkl_priority = ATV_DKL_PRIO_DK;
        break;
    }

    rtd_outl(MSD_CTRL_WORD_0x3200, MSD_CTRL_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Query current ATV Sound Standard
 *
 * @param <p_sound_std>	{ current Sound Standard }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_AtvGetSoundStd(ATV_SOUND_INFO *p_sound_info)
{
#ifndef CONFIG_ENABLE_AUDIO
    p_sound_info = p_sound_info;
#else

    msd_ctrl_word_RBUS			MSD_CTRL_REG;
    msd_result_RBUS				MSD_RESULT_REG;
    mode_detect_RBUS			MODE_DETECT_REG;
    nicam_decode_status_RBUS	MODE_NICAM_REG;
    UINT32	mode;

    // get sound standard
    AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] atv_fw_scan_flag = %x\n",(UINT32)pb_path_info.atv_fw_scan_flag);
    AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] atv_fw_scan_std  = %x\n",(UINT32)pb_path_info.atv_fw_scan_std);
    if (pb_path_info.atv_fw_scan_flag == TRUE)
    {
        p_sound_info->sound_std = pb_path_info.atv_fw_scan_std;
    }
    else
    {
        // TODO:
        /*
         * If want to get more accurate standard, the following registers can be applied.
         *    msd_std_list_RBUS			MSD_LIST_REG;
         *    msd_second_reg_std_RBUS	MSD_SEC_RESULT_REG;
         */
        MSD_RESULT_REG.regValue = rtd_inl(MSD_RESULT_0x3204);
        //p_sound_info->sound_std = MSD_RESULT_REG.bitField.reg_std;
        p_sound_info->sound_std = MSD_RESULT_REG.bitField.hw_detected_std;
        AUDIO_LOG(LOGGER_INFO,"[msd_control] %x\n",(UINT32)rtd_inl(MSD_CTRL_WORD_0x3200));
        AUDIO_LOG(LOGGER_INFO,"[msd_thr_4_5] %x\n",(UINT32)rtd_inl(MSD_THRESH_4_5_0x3208));
        AUDIO_LOG(LOGGER_INFO,"[msd_mag_4_5] %x\n",(UINT32)rtd_inl(MSD_MAG_4_5_0x320c));
        AUDIO_LOG(LOGGER_INFO,"[msd_thr_5_5] %x\n",(UINT32)rtd_inl(MSD_THRESH_5_5_0x3214));
        AUDIO_LOG(LOGGER_INFO,"[msd_mag_5_5] %x\n",(UINT32)rtd_inl(MSD_MAG_5_5_0x3218));
        AUDIO_LOG(LOGGER_INFO,"[msd_thr_6_0] %x\n",(UINT32)rtd_inl(MSD_THRESH_6_0_0x3224));
        AUDIO_LOG(LOGGER_INFO,"[msd_mag_6_0] %x\n",(UINT32)rtd_inl(MSD_MAG_6_0_0x3228));
        AUDIO_LOG(LOGGER_INFO,"[msd_thr_6_5] %x\n",(UINT32)rtd_inl(MSD_THRESH_6_5_0x3238));
        AUDIO_LOG(LOGGER_INFO,"[msd_mag_6_5] %x\n",(UINT32)rtd_inl(MSD_MAG_6_5_0x323c));
        AUDIO_LOG(LOGGER_INFO,"[msd_det_flg] %x\n",(UINT32)rtd_inl(MSD_FLAG_0x326c));
        AUDIO_LOG(LOGGER_INFO,"[msd_larg_st] %x\n",(UINT32)rtd_inl(MSD_LARGE_STATUS_0x3270));
        AUDIO_LOG(LOGGER_INFO,"[msd_result ] %x\n",(UINT32)rtd_inl(MSD_RESULT_0x3204));
    }
    // get sound select
    MSD_CTRL_REG.regValue = rtd_inl(MSD_CTRL_WORD_0x3200);
    p_sound_info->soung_select = MSD_CTRL_REG.bitField.sound_select;

    // get analog sound_mode
    MODE_DETECT_REG.regValue = rtd_inl(MODE_DETECT_0x3300);
    switch (MODE_DETECT_REG.bitField.mode_result)
    {
    case 2:	// Dual/Bilingual(A2) or Mono+SAP(BTSC)
        if (p_sound_info->sound_std == ATV_SOUND_STD_BTSC)
            mode = ATV_SOUND_MODE_SAP_MONO;
        else
            mode = ATV_SOUND_MODE_DUAL;
        break;
    case 3: // Mono(A2) or Stereo+SAP(BTSC)
        if (p_sound_info->sound_std == ATV_SOUND_STD_BTSC)
            mode = ATV_SOUND_MODE_SAP_STEREO;
        else
            mode = ATV_SOUND_MODE_MONO;
        break;
    default:
        mode = MODE_DETECT_REG.bitField.mode_result;
        break;
    }
    p_sound_info->ana_soundmode = mode;

    // get digital sound_mode
    MODE_NICAM_REG.regValue = rtd_inl(NICAM_DECODE_STATUS_0x3514);
    p_sound_info->dig_soundmode = MODE_NICAM_REG.bitField.c1c2c3;

    AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] get sound_standard = %d\n",(UINT32)p_sound_info->sound_std);
    AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] get sound_select   = %d\n",(UINT32)p_sound_info->soung_select);
    AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] get ana_soundmode  = %d\n",(UINT32)p_sound_info->ana_soundmode);
    AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] get dig_soundmode  = %d\n",(UINT32)MODE_NICAM_REG.bitField.c1c2c3);

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Scan/Find current ATV Sound Standard
 *
 * @param <p_sound_std>	{ result of scanning }
 * @prarm <flag>		{ refer to ATV_MSD_OPT in audio_atv.h }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_AtvScanSoundStd(UINT8 *p_sound_std, UINT16 flag)
{
#ifndef CONFIG_ENABLE_AUDIO
    p_sound_std = p_sound_std;
    flag = flag;
#else
    msd_ctrl_word_RBUS		MSD_CTRL_REG;
    msd_result_RBUS			MSD_RESULT_REG;
    msd_second_reg_std_RBUS MSD_SEC_STD_REG;
    interrupt_en_RBUS		INT_EN_REG;
    interrupt_status_RBUS	INT_ST_REG;
    ATV_SCAN_STD			scan_std;
    unsigned int			i;
    UINT32					BK_INT_EN_REG;

    scan_std.sound_std = ATV_SOUND_STD_UNKNOWN;
    scan_std.hdev_mode = 0;
    scan_std.scan_mode = 0;
    scan_std.scan_coef = 0;

    AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] Audio_AtvScanSoundStd(flag=%x)\n",(UINT32)flag);
    AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] IFD Audio BW Setting = %x\n",(UINT32)rtd_inl(AUDIO_BP_SEL_0x5cc0));

    // disable auto standard detection
    MSD_CTRL_REG.regValue = rtd_inl(MSD_CTRL_WORD_0x3200);
    MSD_CTRL_REG.bitField.msd_en = FALSE;
    rtd_outl(MSD_CTRL_WORD_0x3200, MSD_CTRL_REG.regValue);

#ifdef ATV_CFG_FW_SCAN_BY_TIMER_EVENT
//	fw_timer_event_CancelTimerEvent(Audio_AtvTimerEventFwScan);
    fw_timer_event_CancelTimerEvent(timerevent_table_audio_atv_freq_scan_event);
#endif // ATV_CFG_FW_SCAN_BY_TIMER_EVENT
    pb_path_info.atv_fw_scan_flag = FALSE;
    AtvSetFwCarrierFreq(_DISABLE, 0, 0);

    BK_INT_EN_REG = INT_EN_REG.regValue = rtd_inl(INTERRUPT_EN_0x3050);

    // disable TV-related interrupts temporarily.
    INT_EN_REG.bitField.int_mode_det_status_change_en = FALSE;
    INT_EN_REG.bitField.int_msd_change_en = FALSE;
    INT_EN_REG.bitField.int_msd_timeout_en = FALSE;
    INT_EN_REG.bitField.int_nicam_hierr_change_en = FALSE;
    INT_EN_REG.bitField.int_sif_nopt_en = FALSE;
    INT_EN_REG.bitField.int_sif_ovf_en = FALSE;
    rtd_outl(INTERRUPT_EN_0x3050, INT_EN_REG.regValue);

    // clean all interrupt pending status.
    INT_ST_REG.regValue = 0;
    INT_ST_REG.bitField.int_mode_det_status_change = TRUE;
    INT_ST_REG.bitField.int_msd_change = TRUE;
    INT_ST_REG.bitField.int_msd_timeout = TRUE;
    INT_ST_REG.bitField.int_nicam_hierr_change = TRUE;
    INT_ST_REG.bitField.int_sif_nopt = TRUE;
    INT_ST_REG.bitField.int_sif_ovf = TRUE;
    rtd_outl(INTERRUPT_STATUS_0x3054, INT_ST_REG.regValue);


    // adjust IFD output BPF/LPF in PAL mode to allow wider BW
    //AIO_WriteRegister(IFD_AUDIO_BP_SEL_VADDR, IFD_BP_SEL_BYPASS|IFD_LP_SEL_PAL);
    //ATV_LOG("[IFD-BW] %x\n",AIO_ReadRegister(IFD_AUDIO_BP_SEL_VADDR));

    // init. MSD-related registers
    MSD_SEC_STD_REG.regValue = 0;
    MSD_SEC_STD_REG.bitField.mono_std_sel = FALSE;
    rtd_outl(MSD_SECOND_REG_STD_0x3280, MSD_SEC_STD_REG.regValue);

    MSD_RESULT_REG.regValue = rtd_inl(MSD_RESULT_0x3204);
    MSD_RESULT_REG.bitField.reg_std = ATV_SOUND_STD_UNKNOWN;
    rtd_outl(MSD_RESULT_0x3204, MSD_RESULT_REG.regValue);

    MSD_CTRL_REG.regValue = rtd_inl(MSD_CTRL_WORD_0x3200);
    MSD_CTRL_REG.bitField.msd_en     = TRUE;
    MSD_CTRL_REG.bitField.update_en  = TRUE;
    MSD_CTRL_REG.bitField.fw_scan_en = FALSE;
    MSD_CTRL_REG.bitField.carrier_shift_scan_en = FALSE;
    if ((atv_flag & ATV_INIT_SUPPORT_STD_NICAM) || (atv_flag & ATV_INIT_SUPPORT_STD_BTSC))
    {
        MSD_CTRL_REG.bitField.sound_select = ATV_SOUND_SEL_STEREO_A;
    }
    else
    {
        MSD_CTRL_REG.bitField.sound_select = ATV_SOUND_SEL_FMAM;
    }
    rtd_outl(MSD_CTRL_WORD_0x3200, MSD_CTRL_REG.regValue);

    /*
    ATV_LOG("start to scan sound std(%x,%x,%x)\n",
    	rtd_inl(MSD_CTRL_WORD_0x3200),
    	rtd_inl(MSD_RESULT_0x3204),
    	rtd_inl(MSD_SECOND_REG_STD_0x3280));
    */

    // according to user's configuration to scan standard
    if (flag & ATV_SCAN_OPT_HW_AUTO)
    {
        for (i=0; i<10; i++)
        {
            //usleep(100*1000);	// 100ms
            Audio_TimeDelay_ms(100);

            AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] scan(%d) tick %d\n",(UINT32)scan_std.scan_mode,(UINT32)i);
            AUDIO_LOG(LOGGER_INFO,"[msd_thr_4_5] %x\n",(UINT32)rtd_inl(MSD_THRESH_4_5_0x3208));
            AUDIO_LOG(LOGGER_INFO,"[msd_mag_4_5] %x\n",(UINT32)rtd_inl(MSD_MAG_4_5_0x320c));
            AUDIO_LOG(LOGGER_INFO,"[msd_thr_5_5] %x\n",(UINT32)rtd_inl(MSD_THRESH_5_5_0x3214));
            AUDIO_LOG(LOGGER_INFO,"[msd_mag_5_5] %x\n",(UINT32)rtd_inl(MSD_MAG_5_5_0x3218));
            AUDIO_LOG(LOGGER_INFO,"[msd_thr_6_0] %x\n",(UINT32)rtd_inl(MSD_THRESH_6_0_0x3224));
            AUDIO_LOG(LOGGER_INFO,"[msd_mag_6_0] %x\n",(UINT32)rtd_inl(MSD_MAG_6_0_0x3228));
            AUDIO_LOG(LOGGER_INFO,"[msd_thr_6_5] %x\n",(UINT32)rtd_inl(MSD_THRESH_6_5_0x3238));
            AUDIO_LOG(LOGGER_INFO,"[msd_mag_6_5] %x\n",(UINT32)rtd_inl(MSD_MAG_6_5_0x323c));
            AUDIO_LOG(LOGGER_INFO,"[msd_det_flg] %x\n",(UINT32)rtd_inl(MSD_FLAG_0x326c));
            AUDIO_LOG(LOGGER_INFO,"[msd_larg_st] %x\n",(UINT32)rtd_inl(MSD_LARGE_STATUS_0x3270));
            AUDIO_LOG(LOGGER_INFO,"[msd_result ] %x\n",(UINT32)rtd_inl(MSD_RESULT_0x3204));

            MSD_RESULT_REG.regValue = rtd_inl(MSD_RESULT_0x3204);
            scan_std.sound_std = MSD_RESULT_REG.bitField.hw_detected_std;
            if (scan_std.sound_std != ATV_SOUND_STD_UNKNOWN)
            {
                scan_std.scan_coef = i;
                break;
            }
        }
    }

    if ((flag & ATV_SCAN_OPT_FRQ_OFFSET) && (scan_std.sound_std == ATV_SOUND_STD_UNKNOWN))
    {
        scan_std.scan_mode = ATV_SCAN_OPT_FRQ_OFFSET;
        MSD_SEC_STD_REG.regValue = 0;
        MSD_SEC_STD_REG.bitField.mono_std_sel = FALSE;
        rtd_outl(MSD_SECOND_REG_STD_0x3280, MSD_SEC_STD_REG.regValue);
        MSD_CTRL_REG.bitField.carrier_shift_scan_en = TRUE;	// this is 'HW' , naming is worng
        for (i=0; i<=ATV_HW_SCAN_FRQ_OFFSET_P200K; i++)
        {
            MSD_CTRL_REG.bitField.carrier_phase_delta = i;
            rtd_outl(MSD_CTRL_WORD_0x3200, MSD_CTRL_REG.regValue);
            //usleep(500*1000);	// 500ms
            Audio_TimeDelay_ms(500);
            scan_std.sound_std = rtd_inl(MSD_RESULT_0x3204) & 0x1F;

            AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] scan(%d) tick %d\n",(UINT32)scan_std.scan_mode,(UINT32)i);
            AUDIO_LOG(LOGGER_INFO,"[msd_thr_4_5] %x\n",(UINT32)rtd_inl(MSD_THRESH_4_5_0x3208));
            AUDIO_LOG(LOGGER_INFO,"[msd_mag_4_5] %x\n",(UINT32)rtd_inl(MSD_MAG_4_5_0x320c));
            AUDIO_LOG(LOGGER_INFO,"[msd_thr_5_5] %x\n",(UINT32)rtd_inl(MSD_THRESH_5_5_0x3214));
            AUDIO_LOG(LOGGER_INFO,"[msd_mag_5_5] %x\n",(UINT32)rtd_inl(MSD_MAG_5_5_0x3218));
            AUDIO_LOG(LOGGER_INFO,"[msd_thr_6_5] %x\n",(UINT32)rtd_inl(MSD_THRESH_6_0_0x3224));
            AUDIO_LOG(LOGGER_INFO,"[msd_mag_6_5] %x\n",(UINT32)rtd_inl(MSD_MAG_6_0_0x3228));
            AUDIO_LOG(LOGGER_INFO,"[msd_thr_6_5] %x\n",(UINT32)rtd_inl(MSD_THRESH_6_5_0x3238));
            AUDIO_LOG(LOGGER_INFO,"[msd_mag_6_5] %x\n",(UINT32)rtd_inl(MSD_MAG_6_5_0x323c));
            AUDIO_LOG(LOGGER_INFO,"[msd_det_flg] %x\n",(UINT32)rtd_inl(MSD_FLAG_0x326c));
            AUDIO_LOG(LOGGER_INFO,"[msd_larg_st] %x\n",(UINT32)rtd_inl(MSD_LARGE_STATUS_0x3270));

            if (scan_std.sound_std != ATV_SOUND_STD_UNKNOWN)
            {
                scan_std.scan_coef = i;
                break;
            }
        }
    }

    if ((flag & ATV_SCAN_OPT_2ND_PHASE) && (scan_std.sound_std == ATV_SOUND_STD_UNKNOWN))
    {
        scan_std.scan_mode = ATV_SCAN_OPT_2ND_PHASE;
        MSD_CTRL_REG.bitField.carrier_shift_scan_en = FALSE;
        MSD_CTRL_REG.bitField.carrier_phase_delta = 0;
        rtd_outl(MSD_CTRL_WORD_0x3200, MSD_CTRL_REG.regValue);
        MSD_SEC_STD_REG.regValue = 0;
        MSD_SEC_STD_REG.bitField.mono_std_sel = TRUE;
        rtd_outl(MSD_SECOND_REG_STD_0x3280, MSD_SEC_STD_REG.regValue);
        //usleep(500*1000);
        Audio_TimeDelay_ms(500);
        //ATV_LOG("scan(%d) tick \n",scan_std.scan_mode);
        scan_std.sound_std = rtd_inl(MSD_SECOND_REG_STD_0x3280);
    }


    AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] end of scan sound std(%x) in scan_mode(%x,%x)\n",
              (UINT32)scan_std.sound_std,
              (UINT32)scan_std.scan_mode,
              (UINT32)scan_std.scan_coef);


    *p_sound_std = (UINT8)scan_std.sound_std;

    // turn-on interrupts which are enabled before calling this function.
    rtd_outl(INTERRUPT_EN_0x3050, BK_INT_EN_REG);

    /*
     * force to trigger MSD change interrupt to update STD-related setting.
     * it is beacuse MSD interrupt pending bit will lose when MSD interrupt is disabled.
     */
    MSD_RESULT_REG.regValue = rtd_inl(MSD_RESULT_0x3204);
    MSD_RESULT_REG.bitField.reg_std = ATV_SOUND_STD_UNKNOWN;
    rtd_outl(MSD_RESULT_0x3204, MSD_RESULT_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * select IF-ADC data source
 *
 * @param 	<para>	{ ATV_DATA_SRC_IFD/ATV_DATA_SRC_SIF }
 * @return			{ void }
 * @ingroup drv_aio
 */
void Audio_AtvSetDataSrc(ATV_DATA_SRC para)
{
#ifndef CONFIG_ENABLE_AUDIO
    para = para;
#else
    carrier_freq_deviatio_RBUS	FRQ_DEV_CTRL_REG;
    UINT32						IFD_ADC_CK_REG,IFD_AIN_SEL_REG;

    IFD_ADC_CK_REG = rtd_inl(ADC_CK_0x5fd0);
    IFD_AIN_SEL_REG = rtd_inl(AIN_SEL_0x5fc0);

    if (para == ATV_DATA_SRC_IFD)
    {
//		IFD_ADC_CK_REG &= ~IFD_ADC_CK_MASK_BIT;
//		IFD_ADC_CK_REG |= IFD_ADC_CK_FOR_IFD;

//		IFD_AIN_SEL_REG &= ~IFD_AIN_SEL_MASK_BITS;
//		IFD_AIN_SEL_REG |= IFD_AIN_SEL_IFDPGA;
//		IFD_AIN_SEL_REG |= IFD_AIN_SEL_BYPASS_DIF;
    }
    else
    {
        IFD_ADC_CK_REG &= ~IFD_ADC_CK_MASK_BIT;
        IFD_ADC_CK_REG |= IFD_ADC_CK_FOR_SIF;

        IFD_AIN_SEL_REG &= ~IFD_AIN_SEL_MASK_BITS;
        IFD_AIN_SEL_REG |= IFD_AIN_SEL_SIFPGA;
    }

    // step 1
    rtd_outl(ATV_IFD_AIN_SEL_VADDR, IFD_AIN_SEL_REG);

    // step 2
    FRQ_DEV_CTRL_REG.regValue = rtd_inl(CARRIER_FREQ_DEVIATIO_0x3350);
    FRQ_DEV_CTRL_REG.bitField.sif_data_source = para;
    rtd_outl(CARRIER_FREQ_DEVIATIO_0x3350, FRQ_DEV_CTRL_REG.regValue);

    // step 3
    rtd_outl(ATV_IFD_ADC_CK_VADDR, IFD_ADC_CK_REG);

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * get M/N standard status
 *
 * @param <p_status>	{ result of M/N standard status}
 * @param <p_energy>	{ result of M/N standard energy}
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_AtvGetMNStatus(UINT8 *p_status, UINT16 *p_energy)
{
#ifndef CONFIG_ENABLE_AUDIO
    p_status = p_status;
    p_energy = p_energy;
#else
    msd_mag_4_5_RBUS	MSD_MAG_4P5_REG;
    msd_flag_RBUS		MSD_FLAG_REG;
//	msd_ctrl_word_RBUS	MSD_CTRL_REG;
    UINT8				threshold;
    UINT8				retry,t_status;
    UINT16				max_energy,t_energy;

    max_energy = t_energy = 0;

    for (retry=0,threshold=0; retry<5; retry++)
    {
        AUDIO_LOG(LOGGER_INFO,"Audio_AtvGetMNStatus retry %d\n",(UINT32)retry);
        MSD_MAG_4P5_REG.regValue = rtd_inl(MSD_MAG_4_5_0x320c);
        t_energy = MSD_MAG_4P5_REG.bitField.reg_mag_4_5;
        MSD_FLAG_REG.regValue = rtd_inl(MSD_FLAG_0x326c);
        t_status = MSD_FLAG_REG.bitField.flag_4_5;

        if (t_energy > max_energy)
            max_energy = t_energy;

        if (t_status == 1)
        {
#if 0
            threshold++;
#else
    max_energy = t_energy;
    break;
#endif
        }
        Audio_TimeDelay_ms(50);
    }

//	MSD_CTRL_REG.regValue = rtd_inl(MSD_CTRL_WORD_0x3200);
//	AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] MSD_CTRL=%x, MN_THD=%d\n",(UINT32)MSD_CTRL_REG.regValue,(UINT32)threshold);

#if 0
    if (threshold > 5)
    {
        if (p_status != NULL)	*p_status = 1;
        if (p_energy != NULL)	*p_energy = max_energy;
    }
    else
    {
        if (p_status != NULL)	*p_status = 0;
        if (p_energy != NULL)	*p_energy = max_energy;
    }
#else
    *p_status = t_status;
    *p_energy = max_energy;
#endif

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Enable/Disable F/W Scanning
 * This function can suspend/resume F/W scanning when FREQ_SHIFT is enabled.
 * When calling Audio_AtvSetSoundStd, F/W Scaning is enabled automatically.
 *
 * @param 	<para>	{ _ENABLE/_DISABLE }
 * @return			{ void }
 * @ingroup drv_aio
 */
void Audio_AtvSetFwScanCarrierFreqOffset(UINT8 para)
{

    AIO_PATH_INFO 	*path_info = &pb_path_info;

    path_info->atv_fw_scan_usr = para & 0x1;	// _ENABLE/_DISABLE

}

/**
 * ATV TimerEvent : F/W use this event to scan freq. offset of ATV sound carrier.
 *
 * @ingroup lib_audio
 */
//void Audio_AtvTimerEventFwScan(void)
bit Audio_AtvTimerEventFwScan(void)
{
    AIO_PATH_INFO 	*path_info = &pb_path_info;
    UINT8			nicam_status;

#ifndef ATV_CFG_FW_SCAN_BY_TIMER_EVENT
    // invoke this function in Task rountine
    path_info->atv_fw_scan_tick_debounce++;
    if (path_info->atv_fw_scan_tick_debounce >= ATV_FW_SCAN_TICK_DEBOUNCE_LEVEL)
    {
        path_info->atv_fw_scan_tick_debounce = 0;
    }
    else
    {
        return _FALSE;
    }
#else
    if (path_info->atv_fw_scan_flag == FALSE)
    {
//		fw_timer_event_CancelTimerEvent(Audio_AtvTimerEventFwScan);
//		fw_timer_event_CancelTimerEvent(timerevent_table_audio_atv_freq_scan_event);
        return _FALSE;
    }
#endif // !ATV_CFG_FW_SCAN_BY_TIMER_EVENT

#ifdef ATV_CFG_FW_SCAN_ALGO_NEW
    if (path_info->atv_fw_scan_cnt >= ATV_FW_SCAN_FREQ_RANGE)
    {
        if (atv_flag & ATV_INIT_SUPPORT_STD_NICAM)
            nicam_status = AtvFwScanNicam();
        else
            nicam_status = 0;

        if (!nicam_status)
        {
            // when finish NICAM check process,reset fw_scan_cnt to scan carrier offest again
            path_info->atv_fw_scan_cnt = 0;
        }
    }
    else
    {
        AtvFwScanCarrierOffset();
        path_info->atv_fw_scan_cnt++;
    }
#else
    AtvFwScanCarrierOffset();
#endif


#ifdef ATV_CFG_FW_SCAN_BY_TIMER_EVENT
//	fw_timer_event_ActiveTimerEvent(50, Audio_AtvTimerEventFwScan);
//	fw_timer_event_ActiveTimerEvent(50, timerevent_table_audio_atv_freq_scan_event);
#endif // ATV_CFG_FW_SCAN_BY_TIMER_EVENT
    return _TRUE;
}

/**
 * ATV TimerEvent : F/W use this event to enable NICAM interrupt
 *
 * @ingroup lib_audio
 */
void Audio_AtvTimerEventNicamCheck(void)
{
    AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] TimerEvent to Enable NICAM HiBitErr Interrupt \n");
    AtvEnableNicamHiBitErrInt();
}


/**
 * Auto Standard Detection ISR. It is assumed that this interrupt is only enabled when
 * playback path is directed to TV Audio input.
 *
 * @param 	<void> 	{ void }
 * @return			{ void }
 * @ingroup drv_audio
 */
void Audio_ISR_ATV_AutoStdDetect_Change(void)
{
#ifdef CONFIG_ENABLE_AUDIO
    interrupt_status_RBUS	INT_ST_REG;
    msd_result_RBUS			MSD_RESULT_REG;
    UINT8					sound_std;
    AIO_PATH_INFO	 	*path_info = &pb_path_info;

    if (path_info->atv_fw_scan_flag == TRUE)
    {
        // when enable F/W scan, we USE MSD to detect NICAM only
        // do nothing...
        //AUDIO_LOG(LOGGER_INFO,"clear pending bit only\n");
    }
    else
    {
        AtvSetMute(_ENABLE, APP_CH_ID_L | APP_CH_ID_R);

        Audio_AioSetBackendPlay(_DISABLE);

        // get sound standard
        MSD_RESULT_REG.regValue = rtd_inl(MSD_RESULT_0x3204);
        sound_std = MSD_RESULT_REG.bitField.reg_std;

        if (sound_std != ATV_SOUND_STD_UNKNOWN)
        {
            AtvSetHighDeviation(sound_std);
            AtvSetPrescaleDeemphasis(sound_std);
            AtvSetModeDebounceCoef(sound_std);
            switch (sound_std)
            {
            case ATV_SOUND_STD_NICAM_L:
            case ATV_SOUND_STD_NICAM_DK:
            case ATV_SOUND_STD_NICAM_I:
            case ATV_SOUND_STD_NICAM_BG:
                if (atv_flag & ATV_INIT_SUPPORT_STD_NICAM)
                {
                    Audio_AtvSetSoundSelect(ATV_SOUND_SEL_STEREO_A);	// force to NICAM digital mode select
                    pb_path_info.curr_ipt_fs = AIO_FS_ID_32K;
                    pb_path_info.src[0] = AIO_PATH_SRC_ATV_NICAM;		// k_path_cfg.src[0] is still AIO_PATH_SRC_ATV
                    Audio_AioSetPathClock(AIO_PATH_SRC_ATV_NICAM);
                    /*
                     * NICAM HiErrRate interrupt will be enabled after an amount of delay time.
                     * Usually, it will be enabled in a periodic task.
                     */
                    rtd_outl(INTERRUPT_STATUS_0x3054, AIO_INT_ID_NICAM_HIB_CHG);
#ifdef ATV_CFG_NICAM_SWITCH_BY_TIMER_EVENT
                    fw_timer_event_ActiveTimerEvent(500, timerevent_table_audio_atv_nicam_check_event);
#endif // ATV_CFG_NICAM_SWITCH_BY_TIMER_EVENT
                }
                else
                {
                    Audio_AtvSetSoundSelect(ATV_SOUND_SEL_FMAM);
                    if (AtvGetHighDeviation() == TRUE)
                    {
                        pb_path_info.curr_ipt_fs = AIO_FS_ID_96K;
                        pb_path_info.src[0] = AIO_PATH_SRC_ATV_HDEV;	// k_path_cfg.src[0] is still AIO_PATH_SRC_ATV
                    }
                    else
                    {
                        pb_path_info.curr_ipt_fs = AIO_FS_ID_48K;
                        pb_path_info.src[0] = AIO_PATH_SRC_ATV;			// k_path_cfg.src[0] is still AIO_PATH_SRC_ATV
                    }
                    Audio_AioSetPathClock(pb_path_info.src[0]);
                }
                break;

            case ATV_SOUND_STD_EIAJ:
            case ATV_SOUND_STD_BTSC:
            case ATV_SOUND_STD_MN_MONO:
            case ATV_SOUND_STD_A2_M:
            case ATV_SOUND_STD_BG_MONO:
            case ATV_SOUND_STD_A2_BG:
            case ATV_SOUND_STD_DK_MONO:
            case ATV_SOUND_STD_A2_DK1:
            case ATV_SOUND_STD_A2_DK2:
            case ATV_SOUND_STD_A2_DK3:
            case ATV_SOUND_STD_FM_MONO_NO_I:
            case ATV_SOUND_STD_AM_MONO:
            case ATV_SOUND_STD_PAL_N:
            default:
                if (AtvGetHighDeviation() == TRUE)
                {
                    pb_path_info.curr_ipt_fs = AIO_FS_ID_96K;
                    pb_path_info.src[0] = AIO_PATH_SRC_ATV_HDEV;	// k_path_cfg.src[0] is still AIO_PATH_SRC_ATV
                }
                else
                {
                    pb_path_info.curr_ipt_fs = AIO_FS_ID_48K;
                    pb_path_info.src[0] = AIO_PATH_SRC_ATV;			// k_path_cfg.src[0] is still AIO_PATH_SRC_ATV
                }
                Audio_AioSetPathClock(pb_path_info.src[0]);
                break;
            }
            Audio_AioSetBackendPlay(_ENABLE);

            AtvSetMute(_DISABLE, APP_CH_ID_L | APP_CH_ID_R);

        }

    }
    INT_ST_REG.regValue = 0;
    INT_ST_REG.bitField.int_msd_change = TRUE;	// write one to clear
    rtd_outl(INTERRUPT_STATUS_0x3054, INT_ST_REG.regValue);

#endif // CONFIG_ENABLE_AUDIO
}

/**
 * ModeStatus Change ISR. It is assumed that this interrupt is only enabled when
 * playback path is directed to TV Audio input.
 *
 * @param 	<void> 	{ void }
 * @return			{ void }
 * @ingroup drv_audio
 */
void Audio_ISR_ATV_ModeStatus_Change(void)
{
#ifdef CONFIG_ENABLE_AUDIO
    interrupt_status_RBUS	INT_ST_REG;

    INT_ST_REG.regValue = 0;
    INT_ST_REG.bitField.int_mode_det_status_change = TRUE;	// write one to clear
    rtd_outl(INTERRUPT_STATUS_0x3054, INT_ST_REG.regValue);
#endif // CONFIG_ENABLE_AUDIO
}

/**
 * BTSC-ModeStatus Change ISR. It is assumed that this interrupt is only enabled when
 * playback path is directed to TV Audio input.
 *
 * @param 	<void> 	{ void }
 * @return			{ void }
 * @ingroup drv_audio
 */
void Audio_ISR_ATV_BTSC_ModeStatus_Change(void)
{
#ifdef CONFIG_ENABLE_AUDIO
    interrupt_status_RBUS	INT_ST_REG;
    msd_result_RBUS			MSD_RESULT_REG;
    UINT8					mts_sel;

    MSD_RESULT_REG.regValue = rtd_inl(MSD_RESULT_0x3204);

    if (MSD_RESULT_REG.bitField.reg_std == ATV_SOUND_STD_BTSC)
    {

        Audio_AtvGetBtscMtsSelect(&mts_sel);

        switch (mts_sel)
        {
        case ATV_BTSC_MTS_SEL_SAP:
            Audio_HwpSetVolumeCompensation(APP_CH_ID_L|APP_CH_ID_R, m_vol_comp_btsc_sap, APP_DVOL_ID_ATV);
            break;
        case ATV_BTSC_MTS_SEL_STEREO:
            Audio_HwpSetVolumeCompensation(APP_CH_ID_L|APP_CH_ID_R, m_vol_comp_btsc_stereo, APP_DVOL_ID_ATV);
            break;
        case ATV_BTSC_MTS_SEL_MONO:
        case ATV_BTSC_MTS_SEL_L_MONO_R_SAP:
        default:
            Audio_HwpSetVolumeCompensation(APP_CH_ID_L|APP_CH_ID_R, m_vol_comp_btsc_mono, APP_DVOL_ID_ATV);
            break;
        }
    }

    INT_ST_REG.regValue = 0;
    INT_ST_REG.bitField.int_btsc_mode_chg = TRUE;	// write one to clear
    rtd_outl(INTERRUPT_STATUS_0x3054, INT_ST_REG.regValue);

#endif // CONFIG_ENABLE_AUDIO
}

/**
 * AutoStandard Detection Timeout ISR. It is assumed that this interrupt is only enabled when
 * playback path is directed to TV Audio input.
 *
 * @param 	<void> 	{ void }
 * @return			{ void }
 * @ingroup drv_audio
 */
/* not used Willy
void Audio_ISR_ATV_AutoStdDetect_Timeout(void)
{
#ifdef CONFIG_ENABLE_AUDIO
   interrupt_status_RBUS	INT_ST_REG;

   INT_ST_REG.regValue = 0;
   INT_ST_REG.bitField.int_msd_timeout = TRUE;	// write one to clear
   rtd_outl(INTERRUPT_STATUS_0x3054, INT_ST_REG.regValue);

#endif // CONFIG_ENABLE_AUDIO
}
*/
/**
 * SIF Vpp Overflow ISR. It is assumed that this interrupt is only enabled when
 * playback path is directed to TV Audio input.
 *
 * @param 	<void> 	{ void }
 * @return			{ void }
 * @ingroup drv_audio
 */
/* not used Willy
void Audio_ISR_ATV_SIF_Overflow(void)
{
#ifdef CONFIG_ENABLE_AUDIO
   interrupt_status_RBUS	INT_ST_REG;

   INT_ST_REG.regValue = 0;
   INT_ST_REG.bitField.int_sif_ovf = TRUE;	// write one to clear
   rtd_outl(INTERRUPT_STATUS_0x3054, INT_ST_REG.regValue);

#endif // CONFIG_ENABLE_AUDIO
}
*/
/**
 * SIF Under Optimized ISR. It is assumed that this interrupt is only enabled when
 * playback path is directed to TV Audio input.
 *
 * @param 	<void> 	{ void }
 * @return			{ void }
 * @ingroup drv_audio
 */
/* not used Willy
void Audio_ISR_ATV_SIF_Under_Optimize(void)
{
#ifdef CONFIG_ENABLE_AUDIO
   interrupt_status_RBUS	INT_ST_REG;

   INT_ST_REG.regValue = 0;
   INT_ST_REG.bitField.int_sif_nopt = TRUE;	// write one to clear
   rtd_outl(INTERRUPT_STATUS_0x3054, INT_ST_REG.regValue);

#endif // CONFIG_ENABLE_AUDIO
}
*/
/**
 * NICAM Hi BitErrorRate ISR. It is assumed that this interrupt is only enabled when
 * playback path is directed to TV Audio input.
 *
 * @param 	<void> 	{ void }
 * @return			{ void }
 * @ingroup drv_audio
 */
void Audio_ISR_ATV_NICAM_HiErrRate_Change(void)
{
#ifdef CONFIG_ENABLE_AUDIO
    interrupt_status_RBUS	INT_ST_REG;
//	nicam_auto_switch_RBUS	NICAM_AUTO_REG;
    UINT32					NICAM_AUTO_REG;
    global_ctrl_RBUS		GLOBAL_CTRL_REG;
    msd_result_RBUS			MSD_RESULT_REG;
    unsigned int			clk_src;

    /*
     * once HiErrRate happened, turn-off interrupt.
     * interrupt will be enabled while switching to NICAM std next time.
     */
    Audio_AioSetInterrupt(_DISABLE, AIO_INT_ID_NICAM_HIB_CHG);
    rtd_outl(INTERRUPT_STATUS_0x3054, AIO_INT_ID_NICAM_HIB_CHG);

    NICAM_AUTO_REG = rtd_inl(NICAM_AUTO_SWITCH_0x3518);

    if (NICAM_AUTO_REG & _BIT31)  	// [0,1] = [enable,disable]
    {
        GLOBAL_CTRL_REG.regValue = rtd_inl(GLOBAL_CTRL_0x3004);

        clk_src = GLOBAL_CTRL_REG.bitField.sel_play_ck_source;

        MSD_RESULT_REG.regValue = rtd_inl(MSD_RESULT_0x3204);

        if ((clk_src == AIO_PLL_NICAM_RESAMPLE) && (MSD_RESULT_REG.bitField.reg_std != ATV_SOUND_STD_UNKNOWN))
        {
            // change to analog mode
            Audio_AioSetBackendPlay(_DISABLE);
            Audio_AtvSetSoundSelect(ATV_SOUND_SEL_FMAM);
            pb_path_info.curr_ipt_fs = AIO_FS_ID_48K;
            pb_path_info.src[0] = AIO_PATH_SRC_ATV;
            Audio_AioSetPathClock(AIO_PATH_SRC_ATV);
            Audio_AioSetBackendPlay(_ENABLE);
        }
    }

    INT_ST_REG.regValue = 0;
    INT_ST_REG.bitField.int_nicam_hierr_change = TRUE;	// write one to clear
    rtd_outl(INTERRUPT_STATUS_0x3054, INT_ST_REG.regValue);
#endif // CONFIG_ENABLE_AUDIO
}


/*===================== Local Subroutine ======================*/
#ifdef CONFIG_ENABLE_AUDIO
/**
 * turn on/off IF-ADC PGA power
 *
 * @param 	<para>	{ AIO_ENABLE/AIO_DISABLE }
 * @return			{ void }
 * @ingroup drv_aio
 */
/*	not used Willy
static void AtvSetIfAdcPower(BOOL para)
{
   analog_pga_RBUS PGA_CTRL_REG;

   PGA_CTRL_REG.regValue = rtd_inl(ANALOG_PGA_0x391c);
   PGA_CTRL_REG.bitField.if_adc_pgapow = para;
   rtd_outl(ANALOG_PGA_0x391c, PGA_CTRL_REG.regValue);
}
*/
/**
 * turn on/off AGC
 *
 * @param 	<para>	{ AIO_ENABLE/AIO_DISABLE }
 * @return			{ void }
 * @ingroup drv_aio
 */
static void AtvSetHwAGC(BOOL para)
{
    sif_agc_eval_cnt_RBUS AGC_EVAL_REG;

    AGC_EVAL_REG.regValue = rtd_inl(SIF_AGC_EVAL_CNT_0x3190);
    AGC_EVAL_REG.bitField.hw_agc_en = para;
    rtd_outl(SIF_AGC_EVAL_CNT_0x3190, AGC_EVAL_REG.regValue);
}

/**
 * set high deviation depends on standards and flags(add in the future)
 *
 * @param 	<std>	{ refer to ATV_SOUND_STD in audio_atv.h }
 * @return			{ void }
 * @ingroup drv_aio
 */
static void AtvSetHighDeviation(UINT8 sound_std)
{
    global_ctrl_RBUS	GLOBAL_CTRL_REG;

    GLOBAL_CTRL_REG.regValue = rtd_inl(GLOBAL_CTRL_0x3004);

    switch (sound_std)
    {
    case ATV_SOUND_STD_BG_MONO:
    case ATV_SOUND_STD_DK_MONO:
    case ATV_SOUND_STD_FM_MONO_NO_I:
        if ((atv_flag & ATV_INIT_DEV_BGDKI_MASK) == ATV_INIT_DEV_BGDKI_NORMAL)
        {
            GLOBAL_CTRL_REG.bitField.hdv_mode_sel = FALSE;
        }
        else
        {
            GLOBAL_CTRL_REG.bitField.hdv_mode_sel = TRUE;
        }
        break;

    case ATV_SOUND_STD_MN_MONO:
        if ((atv_flag & ATV_INIT_DEV_M_MASK) == ATV_INIT_DEV_M_NORMAL)
        {
            GLOBAL_CTRL_REG.bitField.hdv_mode_sel = FALSE;
        }
        else
        {
            GLOBAL_CTRL_REG.bitField.hdv_mode_sel = TRUE;
        }
        break;

    case ATV_SOUND_STD_NICAM_I:
    case ATV_SOUND_STD_A2_BG:
    case ATV_SOUND_STD_NICAM_BG:
    case ATV_SOUND_STD_A2_DK1:
    case ATV_SOUND_STD_A2_DK2:
    case ATV_SOUND_STD_A2_DK3:
    case ATV_SOUND_STD_NICAM_DK:
    case ATV_SOUND_STD_A2_M:
    case ATV_SOUND_STD_EIAJ:
    case ATV_SOUND_STD_BTSC:
    case ATV_SOUND_STD_AM_MONO:
    case ATV_SOUND_STD_NICAM_L:
    case ATV_SOUND_STD_PAL_N:
    case ATV_SOUND_STD_UNKNOWN:
    default:
        GLOBAL_CTRL_REG.bitField.hdv_mode_sel = FALSE;
        break;
    }

    rtd_outl(GLOBAL_CTRL_0x3004, GLOBAL_CTRL_REG.regValue);
}

static UINT8 AtvGetHighDeviation(void)
{
    global_ctrl_RBUS	GLOBAL_CTRL_REG;

    GLOBAL_CTRL_REG.regValue = rtd_inl(GLOBAL_CTRL_0x3004);

    return GLOBAL_CTRL_REG.bitField.hdv_mode_sel;
}

static void AtvSetPrescaleDeemphasis(UINT8 sound_std)
{
    a2_eiaj_demod_RBUS		FM_CTRL_REG;
    am_RBUS					AM_CTRL_REG;
//	nicam_demod_RBUS		NICAM_DEMO_REG;
    UINT32					NICAM_DEMO_REG;
    UINT8					hdev_mode = 0;
    UINT8					m_hdev_mode = 0;

    FM_CTRL_REG.regValue = rtd_inl(A2_EIAJ_DEMOD_0x3450);
    AM_CTRL_REG.regValue = rtd_inl(AM_0x3360);

    NICAM_DEMO_REG = rtd_inl(NICAM_DEMOD_0x3500);
    NICAM_DEMO_REG &= ~_BIT31;	// psf_sel
//	NICAM_DEMO_REG |= (ATV_NICAM_BW_250KHZ<<31);

    if ((atv_flag & ATV_INIT_DEV_BGDKI_MASK) != ATV_INIT_DEV_BGDKI_NORMAL)
    {
        hdev_mode = AtvGetHighDeviation();
    }

    if ((atv_flag & ATV_INIT_DEV_M_MASK) != ATV_INIT_DEV_M_NORMAL)
    {
        m_hdev_mode = AtvGetHighDeviation();
    }

    switch (sound_std)
    {
    case ATV_SOUND_STD_EIAJ:
        FM_CTRL_REG.bitField.eiaj_sub_deviation = atv_psde_tab[m_hdev_mode][sound_std].eiaj_sub;
    case ATV_SOUND_STD_BTSC:
    case ATV_SOUND_STD_MN_MONO:
    case ATV_SOUND_STD_A2_M:
        FM_CTRL_REG.bitField.a2_bw_sel          = atv_psde_tab[m_hdev_mode][sound_std].a2_bw_sel;
        FM_CTRL_REG.bitField.de_emphasis_time   = atv_psde_tab[m_hdev_mode][sound_std].de_emphasis;
        FM_CTRL_REG.bitField.fm1_deviation_gain = atv_psde_tab[m_hdev_mode][sound_std].fm1_dev_gain;
        FM_CTRL_REG.bitField.fm2_deviation_gain = atv_psde_tab[m_hdev_mode][sound_std].fm2_dev_gain;
        break;

    case ATV_SOUND_STD_NICAM_I:
        NICAM_DEMO_REG |= ((UINT32)atv_psde_tab[hdev_mode][sound_std].nicam_psf<<31);
    case ATV_SOUND_STD_FM_MONO_NO_I:
        FM_CTRL_REG.bitField.a2_bw_sel          = atv_psde_tab[hdev_mode][sound_std].a2_bw_sel;
        FM_CTRL_REG.bitField.de_emphasis_time   = atv_psde_tab[hdev_mode][sound_std].de_emphasis;
        FM_CTRL_REG.bitField.fm1_deviation_gain = atv_psde_tab[hdev_mode][sound_std].fm1_dev_gain;
        FM_CTRL_REG.bitField.fm2_deviation_gain = atv_psde_tab[hdev_mode][sound_std].fm2_dev_gain;
        break;

    case ATV_SOUND_STD_NICAM_BG:
        NICAM_DEMO_REG |= ((UINT32)atv_psde_tab[hdev_mode][sound_std].nicam_psf<<31);
    case ATV_SOUND_STD_BG_MONO:
    case ATV_SOUND_STD_A2_BG:
        FM_CTRL_REG.bitField.a2_bw_sel          = atv_psde_tab[hdev_mode][sound_std].a2_bw_sel;
        FM_CTRL_REG.bitField.de_emphasis_time   = atv_psde_tab[hdev_mode][sound_std].de_emphasis;
        FM_CTRL_REG.bitField.fm1_deviation_gain = atv_psde_tab[hdev_mode][sound_std].fm1_dev_gain;
        FM_CTRL_REG.bitField.fm2_deviation_gain = atv_psde_tab[hdev_mode][sound_std].fm2_dev_gain;
        break;

    case ATV_SOUND_STD_NICAM_DK:
        NICAM_DEMO_REG |= ((UINT32)atv_psde_tab[hdev_mode][sound_std].nicam_psf<<31);
    case ATV_SOUND_STD_DK_MONO:
    case ATV_SOUND_STD_A2_DK1:
    case ATV_SOUND_STD_A2_DK2:
    case ATV_SOUND_STD_A2_DK3:
        FM_CTRL_REG.bitField.a2_bw_sel          = atv_psde_tab[hdev_mode][sound_std].a2_bw_sel;
        FM_CTRL_REG.bitField.de_emphasis_time   = atv_psde_tab[hdev_mode][sound_std].de_emphasis;
        FM_CTRL_REG.bitField.fm1_deviation_gain = atv_psde_tab[hdev_mode][sound_std].fm1_dev_gain;
        FM_CTRL_REG.bitField.fm2_deviation_gain = atv_psde_tab[hdev_mode][sound_std].fm2_dev_gain;
        break;

	case ATV_SOUND_STD_PAL_N:
        FM_CTRL_REG.bitField.a2_bw_sel          = atv_psde_tab[m_hdev_mode][ATV_SOUND_STD_A2_M].a2_bw_sel;
        FM_CTRL_REG.bitField.de_emphasis_time   = atv_psde_tab[m_hdev_mode][ATV_SOUND_STD_A2_M].de_emphasis;
        FM_CTRL_REG.bitField.fm1_deviation_gain = atv_psde_tab[m_hdev_mode][ATV_SOUND_STD_A2_M].fm1_dev_gain;
        FM_CTRL_REG.bitField.fm2_deviation_gain = atv_psde_tab[m_hdev_mode][ATV_SOUND_STD_A2_M].fm2_dev_gain;
		break;
    case ATV_SOUND_STD_AM_MONO:
    case ATV_SOUND_STD_NICAM_L:
        AM_CTRL_REG.bitField.hw_am_prescale = atv_psde_tab[hdev_mode][sound_std].hw_am_prescale;
    default:
        break;
    }

    rtd_outl(A2_EIAJ_DEMOD_0x3450, FM_CTRL_REG.regValue);
    rtd_outl(AM_0x3360, AM_CTRL_REG.regValue);
    rtd_outl(NICAM_DEMOD_0x3500, NICAM_DEMO_REG);
}

static void AtvSetModeDebounceCoef(UINT8 sound_std)
{
    md_ctrl_RBUS			MD_CTRL_REG;
    nicam_decode_ctrl_RBUS	NICAM_DEC_REG;

    if (sound_std != ATV_SOUND_STD_UNKNOWN)
    {
        switch (sound_std)
        {
        case ATV_SOUND_STD_NICAM_L:
        case ATV_SOUND_STD_NICAM_DK:
        case ATV_SOUND_STD_NICAM_I:
        case ATV_SOUND_STD_NICAM_BG:
            NICAM_DEC_REG.regValue = rtd_inl(NICAM_DECODE_CTRL_0x3510);
            NICAM_DEC_REG.bitField.status_debounce_cnt = atv_mode_tab[sound_std].debounce_cnt;
            rtd_outl(NICAM_DECODE_CTRL_0x3510, NICAM_DEC_REG.regValue);
            break;
        case ATV_SOUND_STD_PAL_N:
            MD_CTRL_REG.regValue = rtd_inl(MD_CTRL_0x3304);
            MD_CTRL_REG.bitField.enable = TRUE;
            MD_CTRL_REG.bitField.debounce_cnt = atv_mode_tab[ATV_SOUND_STD_A2_M].debounce_cnt;
            MD_CTRL_REG.bitField.high_threshold = atv_mode_tab[ATV_SOUND_STD_A2_M].hi_threshold;
            MD_CTRL_REG.bitField.low_threshold = atv_mode_tab[ATV_SOUND_STD_A2_M].lo_threshold;
            MD_CTRL_REG.bitField.accu_num = atv_mode_tab[ATV_SOUND_STD_A2_M].accu_num;
            rtd_outl(MD_CTRL_0x3304, MD_CTRL_REG.regValue);
            break;
        	
        case ATV_SOUND_STD_EIAJ:
        case ATV_SOUND_STD_A2_M:
        case ATV_SOUND_STD_A2_BG:
        case ATV_SOUND_STD_A2_DK1:
        case ATV_SOUND_STD_A2_DK2:
        case ATV_SOUND_STD_A2_DK3:
            MD_CTRL_REG.regValue = rtd_inl(MD_CTRL_0x3304);
            MD_CTRL_REG.bitField.enable = TRUE;
            MD_CTRL_REG.bitField.debounce_cnt = atv_mode_tab[sound_std].debounce_cnt;
            MD_CTRL_REG.bitField.high_threshold = atv_mode_tab[sound_std].hi_threshold;
            MD_CTRL_REG.bitField.low_threshold = atv_mode_tab[sound_std].lo_threshold;
            MD_CTRL_REG.bitField.accu_num = atv_mode_tab[sound_std].accu_num;
            rtd_outl(MD_CTRL_0x3304, MD_CTRL_REG.regValue);
            break;
        case ATV_SOUND_STD_BTSC:
            /* configure BTSC debounce in init. reg stage */
        case ATV_SOUND_STD_MN_MONO:
        case ATV_SOUND_STD_BG_MONO:
        case ATV_SOUND_STD_DK_MONO:
        case ATV_SOUND_STD_FM_MONO_NO_I:
        case ATV_SOUND_STD_AM_MONO:
        default:
            break;
        }
    }

}

static void AtvSetFwCarrierFreq(BOOL para, UINT16 fc_main, UINT16 fc_sub)
{
    carrier_freq_deviatio_RBUS	FW_CARRIER_CTRL_REG;
    carrier_phase_delta_RBUS	FW_CARRIER_FREQ_REG;

    if (para == _DISABLE)
    {
        FW_CARRIER_CTRL_REG.regValue = rtd_inl(CARRIER_FREQ_DEVIATIO_0x3350);
        FW_CARRIER_CTRL_REG.bitField.carrier_freq_correct_en = FALSE;
        rtd_outl(CARRIER_FREQ_DEVIATIO_0x3350, FW_CARRIER_CTRL_REG.regValue);
    }
    else
    {
        FW_CARRIER_FREQ_REG.bitField.main_phase_delta = ((UINT32)fc_main<<14)/24576;
        FW_CARRIER_FREQ_REG.bitField.sub_phase_delta = ((UINT32)fc_sub<<14)/24576;
        rtd_outl(CARRIER_PHASE_DELTA_0x3354, FW_CARRIER_FREQ_REG.regValue);

        FW_CARRIER_CTRL_REG.regValue = rtd_inl(CARRIER_FREQ_DEVIATIO_0x3350);
        FW_CARRIER_CTRL_REG.bitField.carrier_freq_correct_en = TRUE;
        rtd_outl(CARRIER_FREQ_DEVIATIO_0x3350, FW_CARRIER_CTRL_REG.regValue);
    }
}

static void AtvFwScanInit(BOOL para)
{
    msd_ctrl_word_RBUS				MSD_CTRL_REG;
    msd_result_RBUS					MSD_RESULT_REG;
    msd_prog_freq_threshold_RBUS	FW_CARRIER_THD_REG;
    msd_prog_freq_RBUS				FW_CARRIER_CTRL_REG;
    msd_thresh_4_5_RBUS				SC1_THRES_REG;	// 4.5M,5.5M,6.0M,6.5M are all the same order in register
    msd_mag_4_5_RBUS				SC1_MAG_REG;	// 4.5M,5.5M,6.0M,6.5M are all the same order in register
    AIO_PATH_INFO 					*path_info = &pb_path_info;
    UINT16		sc1_freq,sc1_mag_addr,sc1_thres_addr;
    UINT16		sc2_freq;
    UINT16		test_freq;
    UINT16		scan_i,scan_step;
    BOOL		test_freq_check,nicam_status;
    INT16		scan_freq_tab[4] = {-450,450,-350,350};

    MSD_RESULT_REG.regValue = rtd_inl(MSD_RESULT_0x3204);
    path_info->atv_fw_scan_std       = MSD_RESULT_REG.bitField.reg_std;
    path_info->atv_fw_scan_flag      = 0;
    path_info->atv_fw_scan_cnt       = 0;
    path_info->atv_fw_scan_usr       = 1;	// defualt to scan
    path_info->atv_fw_scan_nicam_cnt = 0;
    path_info->atv_fw_scan_noise_cnt = 0;

    if (para == _DISABLE)
    {
        MSD_CTRL_REG.regValue = rtd_inl(MSD_CTRL_WORD_0x3200);
        MSD_CTRL_REG.bitField.fw_scan_en = FALSE;
        rtd_outl(MSD_CTRL_WORD_0x3200, MSD_CTRL_REG.regValue);
        return;
    }
    else
    {
        MSD_CTRL_REG.regValue = rtd_inl(MSD_CTRL_WORD_0x3200);
        MSD_CTRL_REG.bitField.fw_scan_en = TRUE;
        MSD_CTRL_REG.bitField.msd_en     = FALSE;
        rtd_outl(MSD_CTRL_WORD_0x3200, MSD_CTRL_REG.regValue);
    }


    sc2_freq = sc1_freq = sc1_mag_addr = sc1_thres_addr = 0;
    nicam_status = FALSE;

    switch (path_info->atv_fw_scan_std)
    {
    case ATV_SOUND_STD_BG_MONO:
        scan_i         = 0;
        scan_step      = 1;
        sc2_freq       = 0;
        sc1_freq       = ATV_FW_SCAN_SC1_BG;
        sc1_mag_addr   = MSD_MAG_5_5_0x3218;
        sc1_thres_addr = MSD_THRESH_5_5_0x3214;
        break;
    case ATV_SOUND_STD_A2_BG:
        scan_i         = 0;
        scan_step      = 1;
        sc2_freq       = ATV_FW_SCAN_SC2_BG;
        sc1_freq       = ATV_FW_SCAN_SC1_BG;
        sc1_mag_addr   = MSD_MAG_5_5_0x3218;
        sc1_thres_addr = MSD_THRESH_5_5_0x3214;
        break;
    case ATV_SOUND_STD_NICAM_BG:
        nicam_status   = TRUE;
        scan_i         = 0;
        scan_step      = 1;
        sc2_freq       = ATV_FW_SCAN_SC2_NICAM_BGDKL;
        sc1_freq       = ATV_FW_SCAN_SC1_BG;
        sc1_mag_addr   = MSD_MAG_5_5_0x3218;
        sc1_thres_addr = MSD_THRESH_5_5_0x3214;
        break;
    case ATV_SOUND_STD_DK_MONO:
        scan_i         = 0;
        scan_step      = 1;
        sc2_freq       = 0;
        sc1_freq       = ATV_FW_SCAN_SC1_DK;
        sc1_mag_addr   = MSD_MAG_6_5_0x323c;
        sc1_thres_addr = MSD_THRESH_6_5_0x3238;
        break;
    case ATV_SOUND_STD_A2_DK1:
        scan_i         = 0;
        scan_step      = 1;
        sc2_freq       = ATV_FW_SCAN_SC2_DK1;
        sc1_freq       = ATV_FW_SCAN_SC1_DK;
        sc1_mag_addr   = MSD_MAG_6_5_0x323c;
        sc1_thres_addr = MSD_THRESH_6_5_0x3238;
        break;
    case ATV_SOUND_STD_A2_DK2:
        scan_i         = 0;
        scan_step      = 1;
        sc2_freq       = ATV_FW_SCAN_SC2_DK2;
        sc1_freq       = ATV_FW_SCAN_SC1_DK;
        sc1_mag_addr   = MSD_MAG_6_5_0x323c;
        sc1_thres_addr = MSD_THRESH_6_5_0x3238;
        break;
    case ATV_SOUND_STD_A2_DK3:
        scan_i         = 0;
        scan_step      = 1;
        sc2_freq       = ATV_FW_SCAN_SC2_DK3;
        sc1_freq       = ATV_FW_SCAN_SC1_DK;
        sc1_mag_addr   = MSD_MAG_6_5_0x323c;
        sc1_thres_addr = MSD_THRESH_6_5_0x3238;
        break;
    case ATV_SOUND_STD_NICAM_DK:
        nicam_status   = TRUE;
        scan_i         = 0;
        scan_step      = 1;
        sc2_freq       = ATV_FW_SCAN_SC2_NICAM_BGDKL;
        sc1_freq       = ATV_FW_SCAN_SC1_DK;
        sc1_mag_addr   = MSD_MAG_6_5_0x323c;
        sc1_thres_addr = MSD_THRESH_6_5_0x3238;
        break;
    case ATV_SOUND_STD_NICAM_I:
        nicam_status   = TRUE;
        sc2_freq       = ATV_FW_SCAN_SC2_NICAM_I;
    case ATV_SOUND_STD_FM_MONO_NO_I:
        scan_i         = 0;
        scan_step      = 1;
        sc1_freq       = ATV_FW_SCAN_SC1_I;
        sc1_mag_addr   = MSD_MAG_6_0_0x3228;
        sc1_thres_addr = MSD_THRESH_6_0_0x3224;
        break;
    case ATV_SOUND_STD_PAL_N:
    case ATV_SOUND_STD_A2_M:
    case ATV_SOUND_STD_EIAJ:
        sc2_freq       = ATV_FW_SCAN_SC2_M;
    case ATV_SOUND_STD_MN_MONO:
        scan_i         = 1;
        scan_step      = 2;
        sc1_freq       = ATV_FW_SCAN_SC1_M;
        sc1_mag_addr   = MSD_MAG_4_5_0x320c;
        sc1_thres_addr = MSD_THRESH_4_5_0x3208;
        break;
    case ATV_SOUND_STD_BTSC:
#ifdef CONFIG_SPARROW_VER_B
        scan_i         = 1;
        scan_step      = 2;
        sc1_freq       = ATV_FW_SCAN_SC1_M;
        sc1_mag_addr   = MSD_MAG_4_5_0x320c;
        sc1_thres_addr = MSD_THRESH_4_5_0x3208;
#endif // CONFIG_SPARROW_VER_B
        break;
    case ATV_SOUND_STD_AM_MONO:
    case ATV_SOUND_STD_NICAM_L:
    case ATV_SOUND_STD_UNKNOWN:
    default:
        // disable fw_scan 'coz this standard can't be scan
        MSD_CTRL_REG.regValue = rtd_inl(MSD_CTRL_WORD_0x3200);
        MSD_CTRL_REG.bitField.fw_scan_en = FALSE;
        MSD_CTRL_REG.bitField.msd_en       = TRUE;
        rtd_outl(MSD_CTRL_WORD_0x3200, MSD_CTRL_REG.regValue);
        break;
    }

    path_info->atv_fw_scan_location = ATV_FW_SCAN_OFFSET_ZERO;
    path_info->atv_fw_scan_sc1 = sc1_freq;
    path_info->atv_fw_scan_sc2 = sc2_freq;



    if (sc1_freq != 0)
    {
        SC1_THRES_REG.regValue = rtd_inl(sc1_thres_addr);
        FW_CARRIER_THD_REG.regValue = 0;
        FW_CARRIER_THD_REG.bitField.thresh_fw = SC1_THRES_REG.bitField.thresh_4_5;
        rtd_outl(MSD_PROGRAMMABLE_FREQ_THRESHOLD_0x3254, FW_CARRIER_THD_REG.regValue);
        FW_CARRIER_CTRL_REG.bitField.thresh_fw_l = SC1_THRES_REG.bitField.thresh_4_5_l;
        FW_CARRIER_CTRL_REG.bitField.fw_phase_delta = ((UINT32)sc1_freq<<14)/24576;
        rtd_outl(MSD_PROGRAMMABLE_FREQ_0x3250, FW_CARRIER_CTRL_REG.regValue);
        Audio_TimeDelay_ms(50);

        SC1_MAG_REG.regValue = rtd_inl(sc1_mag_addr);
        FW_CARRIER_THD_REG.regValue = rtd_inl(MSD_PROGRAMMABLE_FREQ_THRESHOLD_0x3254);
        AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] init scan(mag/thr:%x/%x)\n",
                  (UINT32)FW_CARRIER_THD_REG.bitField.reg_mag_fw,
                  (UINT32)SC1_THRES_REG.bitField.thresh_4_5);

        if (FW_CARRIER_THD_REG.bitField.reg_mag_fw > SC1_THRES_REG.bitField.thresh_4_5)
        {
            scan_i = 10; // ignore init scan
            AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] ingore init. scan\n");
        }
        if ((atv_flag & ATV_INIT_PRESCAN_STD_MASK) != ATV_INIT_PRESCAN_NONE)
        {
            for (; scan_i<4; scan_i+=scan_step)
            {
                test_freq = sc1_freq + scan_freq_tab[scan_i];
                FW_CARRIER_THD_REG.regValue = 0;
                FW_CARRIER_THD_REG.bitField.thresh_fw = SC1_THRES_REG.bitField.thresh_4_5;
                rtd_outl(MSD_PROGRAMMABLE_FREQ_THRESHOLD_0x3254, FW_CARRIER_THD_REG.regValue);
                FW_CARRIER_CTRL_REG.bitField.thresh_fw_l = SC1_THRES_REG.bitField.thresh_4_5_l;
                FW_CARRIER_CTRL_REG.bitField.fw_phase_delta = ((UINT32)test_freq<<14)/24576;
                rtd_outl(MSD_PROGRAMMABLE_FREQ_0x3250, FW_CARRIER_CTRL_REG.regValue);

                Audio_TimeDelay_ms(50);
                FW_CARRIER_THD_REG.regValue = rtd_inl(MSD_PROGRAMMABLE_FREQ_THRESHOLD_0x3254);
                test_freq_check = rtd_inl(MSD_FLAG_0x326c) & 0x01;
                SC1_MAG_REG.regValue = rtd_inl(sc1_mag_addr);

                AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] scan fw_sc1_freq=%d,flag:%x\n",
                          (UINT32)test_freq,
                          (UINT32)rtd_inl(MSD_FLAG_0x326c));
                AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] scan fw_mag/sc_mag:%x/%x\n",
                          (UINT32)FW_CARRIER_THD_REG.bitField.reg_mag_fw,
                          (UINT32)SC1_MAG_REG.bitField.reg_mag_4_5);

                if ( test_freq_check && (FW_CARRIER_THD_REG.bitField.reg_mag_fw > (ATV_FW_SCAN_MAG_THRES+SC1_MAG_REG.bitField.reg_mag_4_5)) )
                {
                    if (test_freq < sc1_freq)
                    {
                        path_info->atv_fw_scan_sc1 = sc1_freq - 400;
                        path_info->atv_fw_scan_location = ATV_FW_SCAN_OFFSET_M400K;
                        if (sc2_freq)
                        {
                            path_info->atv_fw_scan_sc2 = sc2_freq - 400;
                        }
                    }
                    else
                    {
                        path_info->atv_fw_scan_sc1 = sc1_freq + 400;
                        path_info->atv_fw_scan_location = ATV_FW_SCAN_OFFSET_P400K;
                        if (sc2_freq)
                        {
                            path_info->atv_fw_scan_sc2 = sc2_freq + 400;
                        }
                    }
                    break;
                }
            }
        }

        if (!nicam_status)
        {
            // NICAM decoding can't be active with F/W carrier in the same time.
            AtvSetFwCarrierFreq(_ENABLE, path_info->atv_fw_scan_sc1, path_info->atv_fw_scan_sc2);
            FW_CARRIER_THD_REG.regValue = rtd_inl(MSD_PROGRAMMABLE_FREQ_THRESHOLD_0x3254);
            RTD_Log(LOGGER_INFO,"[AUDIO][ATV] init fw_sc1_freq=%d,fw_thres=%x\n",(UINT32)path_info->atv_fw_scan_sc1, (UINT32)FW_CARRIER_THD_REG.regValue);
            RTD_Log(LOGGER_INFO,"[AUDIO][ATV] init fw_sc2_freq=%d\n",(UINT32)path_info->atv_fw_scan_sc2);
        }

        // reset MSD-FW carrier setting
        FW_CARRIER_THD_REG.bitField.thresh_fw = 0xFFFF;
        rtd_outl(MSD_PROGRAMMABLE_FREQ_THRESHOLD_0x3254, FW_CARRIER_THD_REG.regValue);
        rtd_outl(MSD_PROGRAMMABLE_FREQ_0x3250, 0);

        path_info->atv_fw_scan_flag = TRUE;
        path_info->atv_fw_scan_tick_debounce = 0;

#ifdef ATV_CFG_FW_SCAN_ALGO_NEW
        path_info->atv_fw_scan_sc1_debounce = ATV_FW_SCAN_FREQ_RANGE/2;	// set to std. carrier freq.
        path_info->atv_fw_scan_location = 0;							// set to low-bound of scanning range

        test_freq = (sc1_freq-ATV_FW_SCAN_FREQ_OFFSET);
        FW_CARRIER_THD_REG.bitField.thresh_fw = SC1_THRES_REG.bitField.thresh_4_5;
        rtd_outl(MSD_PROGRAMMABLE_FREQ_THRESHOLD_0x3254, FW_CARRIER_THD_REG.regValue);
        FW_CARRIER_CTRL_REG.bitField.thresh_fw_l = SC1_THRES_REG.bitField.thresh_4_5_l;
        FW_CARRIER_CTRL_REG.bitField.fw_phase_delta = ((UINT32)test_freq<<14)/24576;
        rtd_outl(MSD_PROGRAMMABLE_FREQ_0x3250, FW_CARRIER_CTRL_REG.regValue);
#endif // ATV_CFG_FW_SCAN_ALGO_NEW


#ifdef ATV_CFG_FW_SCAN_BY_TIMER_EVENT
//		fw_timer_event_ActiveTimerEvent(100, Audio_AtvTimerEventFwScan);
        fw_timer_event_ActiveTimerEvent(100, timerevent_table_audio_atv_freq_scan_event);
#endif // ATV_CFG_FW_SCAN_BY_TIMER_EVENT


    }

}

static void AtvFwScanCarrierOffset(void)
{
    msd_prog_freq_threshold_RBUS	FW_CARRIER_THD_REG;
    msd_prog_freq_RBUS				FW_CARRIER_CTRL_REG;
    msd_thresh_4_5_RBUS				SC1_THRES_REG;
    msd_mag_4_5_RBUS				SC1_MAG_REG;
    AIO_PATH_INFO 					*path_info = &pb_path_info;
    UINT16		sc1_freq,sc1_mag_addr,sc1_thres_addr;
    UINT16		sc2_freq;
    UINT16		test_freq;
//	UINT16		freq_location;
    UINT16		i,j,l_bound,h_bound,bound_thresh;
    UINT8		sound_mode;
    BOOL		test_freq_check;

    if (path_info->atv_fw_scan_flag == FALSE)
        return;

    sc2_freq = sc1_freq = sc1_mag_addr = sc1_thres_addr = 0;

    switch (path_info->atv_fw_scan_std)
    {
    case ATV_SOUND_STD_BG_MONO:
        sc2_freq       = 0;
        sc1_freq       = ATV_FW_SCAN_SC1_BG;
        sc1_mag_addr   = MSD_MAG_5_5_0x3218;
        sc1_thres_addr = MSD_THRESH_5_5_0x3214;
        break;
    case ATV_SOUND_STD_A2_BG:
        sc2_freq       = ATV_FW_SCAN_SC2_BG;
        sc1_freq       = ATV_FW_SCAN_SC1_BG;
        sc1_mag_addr   = MSD_MAG_5_5_0x3218;
        sc1_thres_addr = MSD_THRESH_5_5_0x3214;
        break;
    case ATV_SOUND_STD_NICAM_BG:
        sc2_freq       = ATV_FW_SCAN_SC2_NICAM_BGDKL;
        sc1_freq       = ATV_FW_SCAN_SC1_BG;
        sc1_mag_addr   = MSD_MAG_5_5_0x3218;
        sc1_thres_addr = MSD_THRESH_5_5_0x3214;
        break;
    case ATV_SOUND_STD_DK_MONO:
        sc2_freq       = 0;
        sc1_freq       = ATV_FW_SCAN_SC1_DK;
        sc1_mag_addr   = MSD_MAG_6_5_0x323c;
        sc1_thres_addr = MSD_THRESH_6_5_0x3238;
        break;
    case ATV_SOUND_STD_A2_DK1:
        sc2_freq       = ATV_FW_SCAN_SC2_DK1;
        sc1_freq       = ATV_FW_SCAN_SC1_DK;
        sc1_mag_addr   = MSD_MAG_6_5_0x323c;
        sc1_thres_addr = MSD_THRESH_6_5_0x3238;
        break;
    case ATV_SOUND_STD_A2_DK2:
        sc2_freq       = ATV_FW_SCAN_SC2_DK2;
        sc1_freq       = ATV_FW_SCAN_SC1_DK;
        sc1_mag_addr   = MSD_MAG_6_5_0x323c;
        sc1_thres_addr = MSD_THRESH_6_5_0x3238;
        break;
    case ATV_SOUND_STD_A2_DK3:
        sc2_freq       = ATV_FW_SCAN_SC2_DK3;
        sc1_freq       = ATV_FW_SCAN_SC1_DK;
        sc1_mag_addr   = MSD_MAG_6_5_0x323c;
        sc1_thres_addr = MSD_THRESH_6_5_0x3238;
        break;
    case ATV_SOUND_STD_NICAM_DK:
        sc2_freq       = ATV_FW_SCAN_SC2_NICAM_BGDKL;
        sc1_freq       = ATV_FW_SCAN_SC1_DK;
        sc1_mag_addr   = MSD_MAG_6_5_0x323c;
        sc1_thres_addr = MSD_THRESH_6_5_0x3238;
        break;
    case ATV_SOUND_STD_NICAM_I:
        sc2_freq       = ATV_FW_SCAN_SC2_NICAM_I;
    case ATV_SOUND_STD_FM_MONO_NO_I:
        sc1_freq       = ATV_FW_SCAN_SC1_I;
        sc1_mag_addr   = MSD_MAG_6_0_0x3228;
        sc1_thres_addr = MSD_THRESH_6_0_0x3224;
        break;
    case ATV_SOUND_STD_PAL_N:
    case ATV_SOUND_STD_A2_M:
    case ATV_SOUND_STD_EIAJ:
        sc2_freq       = ATV_FW_SCAN_SC2_M;
    case ATV_SOUND_STD_MN_MONO:
        sc1_freq       = ATV_FW_SCAN_SC1_M;
        sc1_mag_addr   = MSD_MAG_4_5_0x320c;
        sc1_thres_addr = MSD_THRESH_4_5_0x3208;
        break;
    case ATV_SOUND_STD_BTSC:
#ifdef CONFIG_SPARROW_VER_B
        sc1_freq       = ATV_FW_SCAN_SC1_M;
        sc1_mag_addr   = MSD_MAG_4_5_0x320c;
        sc1_thres_addr = MSD_THRESH_4_5_0x3208;
#endif // CONFIG_SPARROW_VER_B
        break;
    case ATV_SOUND_STD_AM_MONO:
    case ATV_SOUND_STD_NICAM_L:
    case ATV_SOUND_STD_UNKNOWN:
    default:
        break;
    }

    if (sc1_freq != 0)
    {
#ifdef ATV_CFG_FW_SCAN_ALGO_DRAGON // dragon algo.
        FW_CARRIER_THD_REG.regValue = rtd_inl(MSD_PROGRAMMABLE_FREQ_THRESHOLD_0x3254);
        SC1_MAG_REG.regValue = rtd_inl(sc1_mag_addr);
        SC1_THRES_REG.regValue = rtd_inl(sc1_thres_addr);
        test_freq_check = rtd_inl(MSD_FLAG_0x326c) & 0x01;

        /*
        ROSPrintf("[AIN] scan_freq=%d,scan_energy=%x,status=%d\n",
        	(sc1_freq+atv_fw_scan_freq_tab[path_info->atv_fw_scan_cnt]*100),
        	FW_CARRIER_THD_REG.bitField.reg_mag_fw,
        	test_freq_check);
        ROSPrintf("[AIN] sc1_freq=%d,sc1_energy=%x,sc1_thres=%x\n",
        	sc1_freq,
        	SC1_MAG_REG.bitField.reg_mag_4_5,
        	SC1_THRES_REG.bitField.thresh_4_5);
        */

        if ( test_freq_check && ((atv_fw_scan_freq_tab[path_info->atv_fw_scan_cnt] == 0) || (FW_CARRIER_THD_REG.bitField.reg_mag_fw > (ATV_FW_SCAN_MAG_THRES+SC1_MAG_REG.bitField.reg_mag_4_5))) )
        {
            freq_location = ((UINT32)0x1<<(5 - atv_fw_scan_freq_tab[path_info->atv_fw_scan_cnt]));
            ROSPrintf("[AUDIO][ATV] fw_scan_loc=%x, test_loc=%x\n",path_info->atv_fw_scan_location,freq_location);
            if (freq_location >= ((UINT32)path_info->atv_fw_scan_location << 8))
            {
                path_info->atv_fw_scan_sc1 -= 800;
                path_info->atv_fw_scan_location <<= 8;
            }
            else if (freq_location >= ((UINT32)path_info->atv_fw_scan_location << 6))
            {
                path_info->atv_fw_scan_sc1 -= 600;
                path_info->atv_fw_scan_location <<= 6;
            }
            else if (freq_location >= ((UINT32)path_info->atv_fw_scan_location << 4))
            {
                path_info->atv_fw_scan_sc1 -= 400;
                path_info->atv_fw_scan_location <<= 4;
            }
            else if (freq_location >= ((UINT32)path_info->atv_fw_scan_location << 2))
            {
                path_info->atv_fw_scan_sc1 -= 200;
                path_info->atv_fw_scan_location <<= 2;
            }
            else if (freq_location <= ((UINT32)path_info->atv_fw_scan_location >> 8))
            {
                path_info->atv_fw_scan_sc1 += 800;
                path_info->atv_fw_scan_location >>= 8;
            }
            else if (freq_location <= ((UINT32)path_info->atv_fw_scan_location >> 6))
            {
                path_info->atv_fw_scan_sc1 += 600;
                path_info->atv_fw_scan_location >>= 6;
            }
            else if (freq_location <= ((UINT32)path_info->atv_fw_scan_location >> 4))
            {
                path_info->atv_fw_scan_sc1 += 400;
                path_info->atv_fw_scan_location >>= 4;
            }
            else if (freq_location <= ((UINT32)path_info->atv_fw_scan_location >> 2))
            {
                path_info->atv_fw_scan_sc1 += 200;
                path_info->atv_fw_scan_location >>= 2;
            }

            if (path_info->atv_fw_scan_sc1 < (sc1_freq-500))
            {
                path_info->atv_fw_scan_sc1 = sc1_freq-500;
                path_info->atv_fw_scan_location = ATV_FW_SCAN_OFFSET_M500K;
            }
            else if (path_info->atv_fw_scan_sc1 > (sc1_freq+500))
            {
                path_info->atv_fw_scan_sc1 = sc1_freq+500;
                path_info->atv_fw_scan_location = ATV_FW_SCAN_OFFSET_P500K;
            }

            AtvSetFwCarrierFreq(_ENABLE, path_info->atv_fw_scan_sc1, 0);
            ROSPrintf("[AUDIO][ATV] set fw_scan_sc1=%d\n",path_info->atv_fw_scan_sc1);
        }

        do
        {
            path_info->atv_fw_scan_cnt++;
            path_info->atv_fw_scan_cnt %= ATV_FW_SCAN_FREQ_RANGE;
            test_freq = sc1_freq + atv_fw_scan_freq_tab[path_info->atv_fw_scan_cnt]*100;
        }
        while (test_freq == path_info->atv_fw_scan_sc1);

        FW_CARRIER_THD_REG.bitField.thresh_fw = SC1_THRES_REG.bitField.thresh_4_5;
        rtd_outl(MSD_PROGRAMMABLE_FREQ_THRESHOLD_0x3254, FW_CARRIER_THD_REG.regValue);
        rtd_outl(MSD_PROGRAMMABLE_FREQ_0x3250, ((UINT32)test_freq<<14)/24576);
#else	// statistic method, testing
        FW_CARRIER_THD_REG.regValue = rtd_inl(MSD_PROGRAMMABLE_FREQ_THRESHOLD_0x3254);
        SC1_MAG_REG.regValue    = rtd_inl(sc1_mag_addr);
        SC1_THRES_REG.regValue  = rtd_inl(sc1_thres_addr);
        test_freq_check = rtd_inl(MSD_FLAG_0x326c) & 0x01;

        /*
        AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] Curr Fs:%d,MAG/THD:%x/%x\n",
        	(UINT32)((sc1_freq-ATV_FW_SCAN_FREQ_OFFSET) + path_info->atv_fw_scan_location*ATV_FW_SCAN_FREQ_STEP),
        	(UINT32)FW_CARRIER_THD_REG.bitField.reg_mag_fw,
        	(UINT32)FW_CARRIER_THD_REG.bitField.thresh_fw);
        */

        if (path_info->atv_fw_scan_location == 0)
        {
            Audio_AtvGetSoundMode(&sound_mode);
            AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] F/W SoundMode %d @SC1:%dkHz ,SC2:%dkHz\n",
                      (UINT32)sound_mode,
                      (UINT32)path_info->atv_fw_scan_sc1,
                      (UINT32)path_info->atv_fw_scan_sc2
                     );
        }

        atv_fw_scan_mag[path_info->atv_fw_scan_location++] = FW_CARRIER_THD_REG.bitField.reg_mag_fw;

        if (path_info->atv_fw_scan_location >= ATV_FW_SCAN_FREQ_RANGE)
        {
            path_info->atv_fw_scan_location = 0;
            // find out the max mag. between the scan range (except 1st address)
            AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] F/W Scan Range %d ~ %d\n",(UINT32)(sc1_freq-(ATV_FW_SCAN_FREQ_OFFSET-ATV_FW_SCAN_FREQ_STEP)),(UINT32)(sc1_freq+(ATV_FW_SCAN_FREQ_OFFSET-ATV_FW_SCAN_FREQ_STEP)));
            for (i=1,j=1; i<ATV_FW_SCAN_FREQ_RANGE; i++)
            {
                AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] [%d]%x\n",(UINT32)((sc1_freq-ATV_FW_SCAN_FREQ_OFFSET)+i*ATV_FW_SCAN_FREQ_STEP),(UINT32)atv_fw_scan_mag[i]);
                if (atv_fw_scan_mag[i] > atv_fw_scan_mag[j])
                    j = i;
            }

            /*
             * when signal is in high deviation, Energy may not contrast at the SC freq. but spread on the edge.
             * therefore, we need sophisticated method to identify SC.
             */
            if (1/*AtvGetHighDeviation() == TRUE*/)
            {
                l_bound = j;
                h_bound = j;
                bound_thresh = atv_fw_scan_mag[j]>>2;	// assume max dev. is 4 times than normal
                while ((l_bound>0) && (atv_fw_scan_mag[l_bound-1]>bound_thresh))
                {
                    l_bound--;
                }
                while ((h_bound<(ATV_FW_SCAN_FREQ_RANGE-1)) && (atv_fw_scan_mag[h_bound+1]>bound_thresh))
                {
                    h_bound++;
                }
                AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] max_loc=%d,new_loc=%d\n",(UINT32)j,(UINT32)((l_bound+h_bound+1)/2));
                AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] l_bound=%d,h_bound=%d\n",(UINT32)l_bound,(UINT32)h_bound);
                AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] dev= %dk ~ %dk\n",(UINT32)((h_bound-l_bound)*ATV_FW_SCAN_FREQ_STEP/2),(UINT32)((h_bound-l_bound+1)*ATV_FW_SCAN_FREQ_STEP/2));
                j = (l_bound+h_bound+1)/2;
            }

            if (path_info->atv_fw_scan_usr == _ENABLE)
            {
                if (atv_fw_scan_mag[j] >= SC1_THRES_REG.bitField.thresh_4_5)
                {
                    test_freq = ((sc1_freq-ATV_FW_SCAN_FREQ_OFFSET)+j*ATV_FW_SCAN_FREQ_STEP);
                    if ((path_info->atv_fw_scan_sc1_debounce == j) && (path_info->atv_fw_scan_sc1 != test_freq))
                    {
                        path_info->atv_fw_scan_sc1_debounce = 0xFF;	// reset debounce
                        AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] update F/W Freq. from %d to %d\n",(UINT32)path_info->atv_fw_scan_sc1,(UINT32)test_freq);
                        path_info->atv_fw_scan_sc1 = test_freq;
                        path_info->atv_fw_scan_sc2 = ((sc2_freq-ATV_FW_SCAN_FREQ_OFFSET)+j*ATV_FW_SCAN_FREQ_STEP);
                        AtvSetFwCarrierFreq(_ENABLE, path_info->atv_fw_scan_sc1, path_info->atv_fw_scan_sc2);
                    }
                    else
                    {
                        // change F/W freq. only when new freq. shifts more 2 step than old freq.
                        if (ABS(j,path_info->atv_fw_scan_sc1_debounce) >= 2)
                        {
                            AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] found new F/W Freq. %d(now:%d) with Mag(%x)\n",(UINT32)test_freq,(UINT32)path_info->atv_fw_scan_sc1,(UINT32)atv_fw_scan_mag[j]);
                            path_info->atv_fw_scan_sc1_debounce = j;
                        }
                    }
                    path_info->atv_fw_scan_noise_cnt = 0;		//reset
                }
                else
                {
                    path_info->atv_fw_scan_noise_cnt++;
                    AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] weak signal ?!!\n");
                    if (path_info->atv_fw_scan_noise_cnt >= ATV_FW_SCAN_NOISE_CNT_LEVEL)
                    {
                        path_info->atv_fw_scan_noise_cnt = 0;
                        if (path_info->atv_fw_scan_sc1 != sc1_freq)
                        {
                            AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] fixed F/W Freq. to %d\n",(UINT32)sc1_freq);
                            path_info->atv_fw_scan_sc1 = sc1_freq;
                            path_info->atv_fw_scan_sc2 = sc2_freq;
                            AtvSetFwCarrierFreq(_ENABLE, path_info->atv_fw_scan_sc1, path_info->atv_fw_scan_sc2);
                        }
                    }
                }
            }
            else
            {
                if (path_info->atv_fw_scan_sc1 != sc1_freq)
                {
                    AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] fixed F/W Freq. to %d\n",(UINT32)sc1_freq);
                    path_info->atv_fw_scan_sc1 = sc1_freq;
                    path_info->atv_fw_scan_sc2 = sc2_freq;
                    AtvSetFwCarrierFreq(_ENABLE, path_info->atv_fw_scan_sc1, path_info->atv_fw_scan_sc2);
                }
            }

        }
        test_freq = (sc1_freq-ATV_FW_SCAN_FREQ_OFFSET) + path_info->atv_fw_scan_location*ATV_FW_SCAN_FREQ_STEP;

        FW_CARRIER_THD_REG.bitField.thresh_fw = SC1_THRES_REG.bitField.thresh_4_5;
        rtd_outl(MSD_PROGRAMMABLE_FREQ_THRESHOLD_0x3254, FW_CARRIER_THD_REG.regValue);
        FW_CARRIER_CTRL_REG.bitField.thresh_fw_l = SC1_THRES_REG.bitField.thresh_4_5_l;
        FW_CARRIER_CTRL_REG.bitField.fw_phase_delta = ((UINT32)test_freq<<14)/24576;
        rtd_outl(MSD_PROGRAMMABLE_FREQ_0x3250, FW_CARRIER_CTRL_REG.regValue);
//		AUDIO_LOG(LOGGER_INFO,"[AUDIO]AtvFwScanCarrierOffset\n");
#endif // dragon or statistic method
    }
}

static UINT8 AtvFwScanNicam(void)
{
    msd_ctrl_word_RBUS	MSD_CTRL_REG;
    msd_result_RBUS		MSD_RESULT_REG;
    AIO_PATH_INFO	 	*path_info = &pb_path_info;
    UINT8				nicam_status = TRUE;

    if (path_info->atv_fw_scan_flag == FALSE)
        return FALSE;

    if (path_info->atv_fw_scan_nicam_cnt <= 2/*wait state*/)
    {
        if (path_info->atv_fw_scan_nicam_cnt == 0)
        {
            /*
             * disable FW_SCAN and enable MSD
             */
            MSD_CTRL_REG.regValue = rtd_inl(MSD_CTRL_WORD_0x3200);
            MSD_CTRL_REG.bitField.fw_scan_en = FALSE;
            MSD_CTRL_REG.bitField.msd_en     = TRUE;
            rtd_outl(MSD_CTRL_WORD_0x3200, MSD_CTRL_REG.regValue);
            AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] use MSD to check NICAM\n");
        }
        path_info->atv_fw_scan_nicam_cnt++;
        MSD_RESULT_REG.regValue = rtd_inl(MSD_RESULT_0x3204);
        AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] MSD_HW:%x\n",(UINT32)MSD_RESULT_REG.bitField.hw_detected_std);
    }
    else
    {
        path_info->atv_fw_scan_nicam_cnt = 0;

        MSD_RESULT_REG.regValue = rtd_inl(MSD_RESULT_0x3204);
        if ( (MSD_RESULT_REG.bitField.hw_detected_std == ATV_SOUND_STD_NICAM_BG) ||
                (MSD_RESULT_REG.bitField.hw_detected_std == ATV_SOUND_STD_NICAM_DK) ||
                (MSD_RESULT_REG.bitField.hw_detected_std == ATV_SOUND_STD_NICAM_I) ||
                (MSD_RESULT_REG.bitField.hw_detected_std == ATV_SOUND_STD_NICAM_L) )
        {
            // NICAM decoding can't be active with F/W carrier in the same time.
            if ((path_info->src[0] == AIO_PATH_SRC_ATV) || (path_info->src[0] == AIO_PATH_SRC_ATV_HDEV))
            {
                //AtvSetFwCarrierFreq(_DISABLE, 0, 0);
                Audio_AtvSetSoundStd(MSD_RESULT_REG.bitField.hw_detected_std);
                Audio_AtvSetSoundSelect(ATV_SOUND_SEL_STEREO_A);	// set to NICAM digital
#ifdef ATV_CFG_NICAM_SWITCH_BY_TIMER_EVENT
//				fw_timer_event_ActiveTimerEvent(500, Audio_AtvTimerEventNicamCheck);
                fw_timer_event_ActiveTimerEvent(500, timerevent_table_audio_atv_nicam_check_event);
#endif
            }
            path_info->atv_fw_scan_cnt = ATV_FW_SCAN_FREQ_RANGE;	// test, ignore next fw/scan

        }
        else
        {
            nicam_status = FALSE;
            /*
             * enable FW_SCAN and disable MSD
             */
            MSD_CTRL_REG.regValue = rtd_inl(MSD_CTRL_WORD_0x3200);
            MSD_CTRL_REG.bitField.fw_scan_en = TRUE;
            MSD_CTRL_REG.bitField.msd_en     = FALSE;
            rtd_outl(MSD_CTRL_WORD_0x3200, MSD_CTRL_REG.regValue);
        }
        AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] MSD_HW:%x,NICAM status(%d)\n",(UINT32)MSD_RESULT_REG.bitField.hw_detected_std,(UINT32)nicam_status);

    }


    return nicam_status;
}

/*
 * check if it's necessary to switch NICAM from digital to analog mode.
 * In the first 500ms after standard changes, HiBitErrRate still unstable.
 * So we check it after that period.
 */
static void AtvEnableNicamHiBitErrInt(void)
{
    if (pb_path_info.src[0] == AIO_PATH_SRC_ATV_NICAM)
    {
        AUDIO_LOG(LOGGER_INFO,"[AUDIO][ATV] Enable NICAM HiBitErr Interrupt 2\n");
        rtd_outl(INTERRUPT_STATUS_0x3054, AIO_INT_ID_NICAM_HIB_CHG);		// clear pending bit
        Audio_AioSetInterrupt(_ENABLE, AIO_INT_ID_NICAM_HIB_CHG);
    }
}

static void AtvSetSoundStd(UINT8 sound_std)
{
    msd_result_RBUS			MSD_RESULT_REG;
    UINT8					sound_mode;

    if (sound_std > ATV_SOUND_STD_PAL_N)//ATV_SOUND_STD_NICAM_I)
    {
        rtd_outl(MSD_RESULT_0x3204, ATV_SOUND_STD_UNKNOWN);
        return;
    }

    MSD_RESULT_REG.regValue = rtd_inl(MSD_RESULT_0x3204);

    // check if support BTSC only
    if ((atv_flag & ATV_INIT_SUPPORT_STD_MASK) == ATV_INIT_SUPPORT_STD_BTSC)
    {
        if ((atv_flag & ATV_INIT_DEV_M_MASK) == ATV_INIT_DEV_M_NORMAL)
        {
            MSD_RESULT_REG.bitField.reg_std = ATV_SOUND_STD_BTSC;
        }
        else
        {
            Audio_AtvSetSoundMode(ATV_SOUND_MODE_AUTO);
            Audio_TimeDelay_ms(250);
            Audio_AtvGetSoundMode(&sound_mode);
            if (sound_mode == ATV_SOUND_MODE_MONO)
            {
                // deal high-deviation signal with FM_MONO
                MSD_RESULT_REG.bitField.reg_std = ATV_SOUND_STD_MN_MONO;
            }
            else
            {
                MSD_RESULT_REG.bitField.reg_std = ATV_SOUND_STD_BTSC;
            }
        }
    }
    else
    {

        switch (sound_std)
        {
        case ATV_SOUND_STD_A2_BG:
            if ((atv_flag & ATV_INIT_DEV_BGDKI_MASK) != ATV_INIT_DEV_BGDKI_NORMAL)
            {
                Audio_AtvSetSoundMode(ATV_SOUND_MODE_AUTO);
                Audio_TimeDelay_ms(250);
                Audio_AtvGetSoundMode(&sound_mode);
                if (sound_mode == ATV_SOUND_MODE_MONO)
                {
                    // deal high-deviation signal with FM_MONO
                    MSD_RESULT_REG.bitField.reg_std = ATV_SOUND_STD_BG_MONO;
                }
                else
                {
                    MSD_RESULT_REG.bitField.reg_std = sound_std;
                }
            }
            else
            {
                MSD_RESULT_REG.bitField.reg_std = sound_std;
            }
            break;

        case ATV_SOUND_STD_A2_DK1:
        case ATV_SOUND_STD_A2_DK2:
        case ATV_SOUND_STD_A2_DK3:
            if ((atv_flag & ATV_INIT_DEV_BGDKI_MASK) != ATV_INIT_DEV_BGDKI_NORMAL)
            {
                Audio_AtvSetSoundMode(ATV_SOUND_MODE_AUTO);
                Audio_TimeDelay_ms(250);
                Audio_AtvGetSoundMode(&sound_mode);
                if (sound_mode == ATV_SOUND_MODE_MONO)
                {
                    // deal high-deviation signal with FM_MONO
                    MSD_RESULT_REG.bitField.reg_std = ATV_SOUND_STD_DK_MONO;
                }
                else
                {
                    MSD_RESULT_REG.bitField.reg_std = sound_std;
                }
            }
            else
            {
                MSD_RESULT_REG.bitField.reg_std = sound_std;
            }
            break;
        case ATV_SOUND_STD_PAL_N:
            if ((atv_flag & ATV_INIT_DEV_M_MASK) != ATV_INIT_DEV_M_NORMAL)
            {
                Audio_AtvSetSoundMode(ATV_SOUND_MODE_AUTO);
                Audio_TimeDelay_ms(250);
                Audio_AtvGetSoundMode(&sound_mode);
                if (sound_mode == ATV_SOUND_MODE_MONO)
                {
                    // deal high-deviation signal with FM_MONO
                    MSD_RESULT_REG.bitField.reg_std = ATV_SOUND_STD_MN_MONO;
                }
                else
                {
                    MSD_RESULT_REG.bitField.reg_std = ATV_SOUND_STD_A2_M;
                }
            }
            else
            {
                MSD_RESULT_REG.bitField.reg_std = ATV_SOUND_STD_A2_M;
            }
            break;
            
        case ATV_SOUND_STD_A2_M:
        case ATV_SOUND_STD_EIAJ:
        case ATV_SOUND_STD_BTSC:
            if ((atv_flag & ATV_INIT_DEV_M_MASK) != ATV_INIT_DEV_M_NORMAL)
            {
                Audio_AtvSetSoundMode(ATV_SOUND_MODE_AUTO);
                Audio_TimeDelay_ms(250);
                Audio_AtvGetSoundMode(&sound_mode);
                if (sound_mode == ATV_SOUND_MODE_MONO)
                {
                    // deal high-deviation signal with FM_MONO
                    MSD_RESULT_REG.bitField.reg_std = ATV_SOUND_STD_MN_MONO;
                }
                else
                {
                    MSD_RESULT_REG.bitField.reg_std = sound_std;
                }
            }
            else
            {
                MSD_RESULT_REG.bitField.reg_std = sound_std;
            }
            break;

        case ATV_SOUND_STD_MN_MONO:
        case ATV_SOUND_STD_BG_MONO:
        case ATV_SOUND_STD_DK_MONO:
        case ATV_SOUND_STD_AM_MONO:
        case ATV_SOUND_STD_FM_MONO_NO_I:
            MSD_RESULT_REG.bitField.reg_std = sound_std;
            break;

        case ATV_SOUND_STD_NICAM_BG:	//(analog is FM-Mono)
            if (atv_flag & ATV_INIT_SUPPORT_STD_NICAM)
            {
                // TODO: get NICAM mode to decide if we should force to analog here
                MSD_RESULT_REG.bitField.reg_std = sound_std;
            }
            else
            {
                MSD_RESULT_REG.bitField.reg_std = ATV_SOUND_STD_BG_MONO;
            }
            break;
        case ATV_SOUND_STD_NICAM_DK:	//(analog is FM-Mono)
            if (atv_flag & ATV_INIT_SUPPORT_STD_NICAM)
            {
                // TODO: get NICAM mode to decide if we should force to analog here
                MSD_RESULT_REG.bitField.reg_std = sound_std;
            }
            else
            {
                MSD_RESULT_REG.bitField.reg_std = ATV_SOUND_STD_DK_MONO;
            }
            break;
        case ATV_SOUND_STD_NICAM_I:		//(analog is FM-Mono)
            if (atv_flag & ATV_INIT_SUPPORT_STD_NICAM)
            {
                // TODO: get NICAM mode to decide if we should force to analog here
                MSD_RESULT_REG.bitField.reg_std = sound_std;
            }
            else
            {
                MSD_RESULT_REG.bitField.reg_std = ATV_SOUND_STD_FM_MONO_NO_I;
            }
            break;
        case ATV_SOUND_STD_NICAM_L:		//(analog is AM Mono)
            if (atv_flag & ATV_INIT_SUPPORT_STD_NICAM)
            {
                // TODO: get NICAM mode to decide if we should force to analog here
                MSD_RESULT_REG.bitField.reg_std = sound_std;
            }
            else
            {
                MSD_RESULT_REG.bitField.reg_std = ATV_SOUND_STD_AM_MONO;
            }
            break;
        default : // unknown
            break;
        }

    }

    rtd_outl(MSD_RESULT_0x3204, MSD_RESULT_REG.regValue);

}

static void AtvSetAdaptiveCarrierThreshold(BOOL para)
{
    // TODO: if carrier has a lot of freq. shift, max. energy will shift too.
    UINT16	c4p5_eng = rtd_inl(MSD_MAG_4_5_0x320c);
    UINT16	c5p5_eng = rtd_inl(MSD_MAG_5_5_0x3218);
    UINT16	c6p0_eng = rtd_inl(MSD_MAG_6_0_0x3228);
    UINT16	c6p5_eng = rtd_inl(MSD_MAG_6_5_0x323c);

    // only change carrier that has max energy between 4.5MHz and 6.5MHz
    if (c4p5_eng > c5p5_eng)
    {
        c5p5_eng = 0;
        if (c4p5_eng > c6p0_eng)
        {
            c6p0_eng = 0;
            if (c4p5_eng > c6p5_eng) c6p5_eng = 0;
            else					c4p5_eng = 0;
        }
        else
        {
            c4p5_eng = 0;
            if (c6p0_eng > c6p5_eng) c6p5_eng = 0;
            else					c6p0_eng = 0;
        }
    }
    else
    {
        c4p5_eng = 0;
        if (c5p5_eng > c6p0_eng)
        {
            c6p0_eng = 0;
            if (c5p5_eng > c6p5_eng) c6p5_eng = 0;
            else					c5p5_eng = 0;
        }
        else
        {
            c5p5_eng = 0;
            if (c6p0_eng > c6p5_eng) c6p5_eng = 0;
            else					c6p0_eng = 0;
        }
    }



    if (para == _ENABLE)
    {
        if ((atv_flag & ATV_INIT_ADAPATIVE_SC1_THD_4P5MHZ) && c4p5_eng)
        {
            if (c4p5_eng > (m_c4p5mhz_threshold_high_tab[0] + 0x200))
            {
                m_c4p5mhz_threshold_high = m_c4p5mhz_threshold_high_tab[0];
                m_c4p5mhz_threshold_low  = m_c4p5mhz_threshold_low_tab[0];
            }
            else if (c4p5_eng > (m_c4p5mhz_threshold_high_tab[1] + 0x200))
            {
                m_c4p5mhz_threshold_high = m_c4p5mhz_threshold_high_tab[1];
                m_c4p5mhz_threshold_low  = m_c4p5mhz_threshold_low_tab[1];
            }
            else
            {
                m_c4p5mhz_threshold_high = m_c4p5mhz_threshold_high_tab[2];
                m_c4p5mhz_threshold_low  = m_c4p5mhz_threshold_low_tab[2];
            }
        }
        if ((atv_flag & ATV_INIT_ADAPATIVE_SC1_THD_5P5MHZ) && c5p5_eng)
        {
            if (c5p5_eng > (m_c5p5mhz_threshold_high_tab[0] + 0x200))
            {
                m_c5p5mhz_threshold_high = m_c5p5mhz_threshold_high_tab[0];
                m_c5p5mhz_threshold_low  = m_c5p5mhz_threshold_low_tab[0];
            }
            else if (c5p5_eng > (m_c5p5mhz_threshold_high_tab[1] + 0x200))
            {
                m_c5p5mhz_threshold_high = m_c5p5mhz_threshold_high_tab[1];
                m_c5p5mhz_threshold_low  = m_c5p5mhz_threshold_low_tab[1];
            }
            else
            {
                m_c5p5mhz_threshold_high = m_c5p5mhz_threshold_high_tab[2];
                m_c5p5mhz_threshold_low  = m_c5p5mhz_threshold_low_tab[2];
            }
        }
        if ((atv_flag & ATV_INIT_ADAPATIVE_SC1_THD_6P0MHZ) && c6p0_eng)
        {
            if (c6p0_eng > (m_c6p0mhz_threshold_high_tab[0] + 0x200))
            {
                m_c6p0mhz_threshold_high = m_c6p0mhz_threshold_high_tab[0];
                m_c6p0mhz_threshold_low  = m_c6p0mhz_threshold_low_tab[0];
            }
            else if (c6p0_eng > (m_c6p0mhz_threshold_high_tab[1] + 0x200))
            {
                m_c6p0mhz_threshold_high = m_c6p0mhz_threshold_high_tab[1];
                m_c6p0mhz_threshold_low  = m_c6p0mhz_threshold_low_tab[1];
            }
            else
            {
                m_c6p0mhz_threshold_high = m_c6p0mhz_threshold_high_tab[2];
                m_c6p0mhz_threshold_low  = m_c6p0mhz_threshold_low_tab[2];
            }
        }
        if ((atv_flag & ATV_INIT_ADAPATIVE_SC1_THD_6P5MHZ) && c6p5_eng)
        {
            if (c6p5_eng > (m_c6p5mhz_threshold_high_tab[0] + 0x200))
            {
                m_c6p5mhz_threshold_high = m_c6p5mhz_threshold_high_tab[0];
                m_c6p5mhz_threshold_low  = m_c6p5mhz_threshold_low_tab[0];
            }
            else if (c6p5_eng > (m_c6p5mhz_threshold_high_tab[1] + 0x200))
            {
                m_c6p5mhz_threshold_high = m_c6p5mhz_threshold_high_tab[1];
                m_c6p5mhz_threshold_low  = m_c6p5mhz_threshold_low_tab[1];
            }
            else
            {
                m_c6p5mhz_threshold_high = m_c6p5mhz_threshold_high_tab[2];
                m_c6p5mhz_threshold_low  = m_c6p5mhz_threshold_low_tab[2];
            }
        }

    }
    else
    {
        m_c4p5mhz_threshold_high = m_c4p5mhz_threshold_high_tab[0];
        m_c5p5mhz_threshold_high = m_c5p5mhz_threshold_high_tab[0];
        m_c6p0mhz_threshold_high = m_c6p0mhz_threshold_high_tab[0];
        m_c6p5mhz_threshold_high = m_c6p5mhz_threshold_high_tab[0];

        m_c4p5mhz_threshold_low = m_c4p5mhz_threshold_low_tab[0];
        m_c5p5mhz_threshold_low = m_c5p5mhz_threshold_low_tab[0];
        m_c6p0mhz_threshold_low = m_c6p0mhz_threshold_low_tab[0];
        m_c6p5mhz_threshold_low = m_c6p5mhz_threshold_low_tab[0];
    }

    rtd_outl(MSD_THRESH_4_5_0x3208, ((UINT32)m_c4p5mhz_threshold_high<<16) | m_c4p5mhz_threshold_low);
    rtd_outl(MSD_THRESH_5_5_0x3214, ((UINT32)m_c5p5mhz_threshold_high<<16) | m_c5p5mhz_threshold_low);
    rtd_outl(MSD_THRESH_6_0_0x3224, ((UINT32)m_c6p0mhz_threshold_high<<16) | m_c6p0mhz_threshold_low);
    rtd_outl(MSD_THRESH_6_5_0x3238, ((UINT32)m_c6p5mhz_threshold_high<<16) | m_c6p5mhz_threshold_low);

    AUDIO_LOG(LOGGER_INFO,"[msd_thr_4_5] = %x\n",(UINT32)rtd_inl(MSD_THRESH_4_5_0x3208));
    AUDIO_LOG(LOGGER_INFO,"[msd_thr_5_5] = %x\n",(UINT32)rtd_inl(MSD_THRESH_5_5_0x3214));
    AUDIO_LOG(LOGGER_INFO,"[msd_thr_6_0] = %x\n",(UINT32)rtd_inl(MSD_THRESH_6_0_0x3224));
    AUDIO_LOG(LOGGER_INFO,"[msd_thr_6_5] = %x\n",(UINT32)rtd_inl(MSD_THRESH_6_5_0x3238));

}

static void AtvSetMute(BOOL para, UINT8 ch_id)
{
    dvol_control_RBUS	TS_PATH_REG;

    if ( (pb_path_info.src[0] == AIO_PATH_SRC_ATV) ||
            (pb_path_info.src[0] == AIO_PATH_SRC_ATV_HDEV) ||
            (pb_path_info.src[0] == AIO_PATH_SRC_ATV_NICAM) )
    {
        Audio_HwpSetMute(para, ch_id, ATV_MUTE_ID);
    }

    TS_PATH_REG.regValue = rtd_inl(DVOL_CONTROL_0x3814);

    if (TS_PATH_REG.bitField.vol_source_sel == 0/*TV_AUDIO_IN*/)
    {
        Audio_HwpSetMuteTS(para, ch_id, ATV_MUTE_ID);
    }
}

#endif // CONFIG_ENABLE_AUDIO
#endif
