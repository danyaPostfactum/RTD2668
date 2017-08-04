/*=============================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2007 *
 * All rights reserved.                                       *
 *============================================================*/
/*======================= Description ========================*/
/**
 * @file  audio_host.c
 *
 * @author
 * @date
 * @version 0.1
 * @ingroup drv_adc
 */
/*====================== Include files ========================*/
#include "sparrow.h"
#include "isr_utility.h"
#include "timer_event.h"
#include "ROSPrintf.h"
#include "audio_reg.h"
#include "audio/audio_api.h"
#include "audio_host.h"
#include "audio_tv.h"
#include "audio_if.h"
//#include "audio_hwp.h"
#include "audio_hwp_lv.h"


/*======================= Type Defines ========================*/
extern void * memset(void * s, int c, size_t count);
/*===================== Global Variable =======================*/
static UINT8		m_critical_cntr;

AIO_PATH_INFO	pb_path_info;

static AIO_SPDIF_PLL_TAB code spdif_pll_table[] =
{
    {10, 0, 6, 0, 0},	// 8K		, but LasVegas do not support
    {11, 1, 4,29, 0},	// 11.025K	, but LasVegas do not support
    {10, 0, 4, 0, 0},	// 12K 		, but LasVegas do not support
    {10, 0, 3, 0, 0},	// 16K
    {11, 1, 2,29, 0},	// 22.05K
    {10, 0, 2, 0, 0},	// 24K
    { 4, 0, 3, 0, 0},	// 32K
    {11, 1, 1,29, 0},	// 44.1K
    { 4, 0, 2, 0, 0},	// 48K
    { 1, 0, 3, 0, 0},	// 64K		, but LasVegas do not support
    {11, 1, 1,29, 1},	// 88.2K
    { 4, 0, 1, 0, 0},	// 96K
    { 1, 0, 1, 0, 0},	// 128K		, but LasVegas do not support
    {11, 1, 1,29, 2},	// 176.4K
    { 1, 0, 1, 0, 0}	// 192K
};

static AIO_I2S_PLL_TAB code i2s_pll_table64_for_spdif[] =
{
    { 0, 1, 0,  14, 0, 0},	// special mode for SPDIF_88K
    { 0, 1, 1,  14, 0, 0}	// special mode for SPDIF_176K
};

static AIO_I2S_PLL_TAB code i2s_pll_table64[] =
{
    { 0, 1, 0, 254, 0, 4},	// 8K
    { 0, 1, 0, 254, 0, 4},	// 11.025K
    { 0, 1, 0, 254, 0, 4},	// 12K
    { 0, 1, 0, 254, 0, 4},	// 16K
    { 0, 1, 0, 254, 0, 4},	// 22.05K
    { 0, 1, 0, 254, 0, 4},	// 24K
    { 0, 1, 0, 126, 0, 2},	// 32K
    { 0, 1, 0, 126, 0, 2},	// 44.1K
    { 0, 1, 0, 126, 0, 2},	// 48K
    { 0, 0, 0, 126, 0, 0},	// 64K	, LasVegas do not support
    { 0, 0, 0, 126, 0, 0},	// 88.2K
    { 0, 0, 0, 126, 0, 0},	// 96K
    { 0, 0, 1, 126, 2, 0},	// 128K	, LasVegas do not support
    { 0, 0, 1, 126, 2, 0},	// 176.4K
    { 0, 0, 1, 126, 2, 0}	// 192K
};

static AIO_I2S_PLL_TAB code i2s_pll_table48[] =
{
    { 0, 1, 0, 254, 0, 3},	// 8K
    { 0, 1, 0, 254, 0, 3},	// 11.025K
    { 0, 1, 0, 254, 0, 3},	// 12K
    { 0, 1, 0, 254, 0, 3},	// 16K
    { 0, 1, 0, 254, 0, 3},	// 22.05K
    { 0, 1, 0, 254, 0, 3},	// 24K
    { 0, 1, 0, 126, 0, 3},	// 32K
    { 0, 0, 0, 126, 0, 1},	// 44.1K
    { 0, 1, 0, 126, 0, 1},	// 48K
    { 0, 0, 0, 126, 1, 0},	// 64K	, LasVegas do not support
    { 0, 0, 0, 126, 1, 0},	// 88.2K
    { 0, 0, 0, 126, 1, 0},	// 96K
    { 0, 0, 1, 126, 1, 0},	// 128K	, LasVegas do not support
    { 0, 0, 1, 126, 1, 0},	// 176.4K
    { 0, 0, 1, 126, 1, 0}	// 192K
};

static UINT8 code i2s_map_to_aio_fs_id[14] =
{
    AIO_FS_ID_UNKNOWN,	AIO_FS_ID_8K,	AIO_FS_ID_11K,	AIO_FS_ID_12K,
    AIO_FS_ID_16K,		AIO_FS_ID_22K,	AIO_FS_ID_24K,	AIO_FS_ID_32K,
    AIO_FS_ID_44K,		AIO_FS_ID_48K,	AIO_FS_ID_88K,	AIO_FS_ID_96K,
    AIO_FS_ID_176K,		AIO_FS_ID_192K
};

static UINT8 code spdif_map_to_aio_fs_id[14] =
{
    AIO_FS_ID_UNKNOWN,	AIO_FS_ID_8K,	AIO_FS_ID_11K,	AIO_FS_ID_12K,
    AIO_FS_ID_16K,		AIO_FS_ID_22K,	AIO_FS_ID_24K,	AIO_FS_ID_32K,
    AIO_FS_ID_44K,		AIO_FS_ID_48K,	AIO_FS_ID_88K,	AIO_FS_ID_96K,
    AIO_FS_ID_176K,		AIO_FS_ID_192K
};

/*
static UINT8 code spdif_cs_map_to_aio_fs_id[16] = {
	AIO_FS_ID_44K,	AIO_FS_ID_UNKNOWN,	AIO_FS_ID_48K,	AIO_FS_ID_32K,
	AIO_FS_ID_22K,	AIO_FS_ID_UNKNOWN,	AIO_FS_ID_24K,	AIO_FS_ID_UNKNOWN,
	AIO_FS_ID_88K,	AIO_FS_ID_UNKNOWN,	AIO_FS_ID_96K,	AIO_FS_ID_UNKNOWN,
	AIO_FS_ID_176K,	AIO_FS_ID_UNKNOWN,	AIO_FS_ID_192K,	AIO_FS_ID_UNKNOWN
};
*/

static void EnableAudioClock();
static void InitInterrupt();

//static void Timerevent_Check_SPDIF_LockStatus();
static INT8 SetPLL_I2S(UINT8 fs);
static INT8 SetPLL_SPDIF(UINT8 fs);
static void SetSPDIF_DigitalTracking(BOOL para);
static void SetPbDataSrcSel(UINT8 *src);
static void SetTsDataSrcSel(UINT8 src);
static void SetPeakSrcSel(AIO_PEAK_SRC src);
extern void timerevent_table_audio_atv_freq_scan_event(void);

/*====================== API Routines =========================*/
/*=============================================================*/
/**
 * Audio_Init_Step1
 * Init audio decoder module.
 *
 * @param <void>	{ void }
 * @return 			{ void }
 * @ingroup drv_adc
 */
void Audio_Init_Step1()
{
#ifdef CONFIG_ENABLE_AUDIO
    // init. audio driver internal state info.
    m_critical_cntr = 0;

    memset(&pb_path_info,0,sizeof(AIO_PATH_INFO));
    pb_path_info.max_playback_fs = AIO_FS_ID_48K;
    pb_path_info.curr_ipt_fs = AIO_FS_ID_48K;
    pb_path_info.curr_opt_fs = AIO_FS_ID_48K;
    pb_path_info.prev_opt_fs = AIO_FS_ID_UNKNOWN;
    pb_path_info.spdifo_data_type = AIO_DATA_IS_PCM;
    pb_path_info.spdifo_src_sel = AIO_SPDIFO_DISABLE;
    pb_path_info.beeper_duration = 0;
    pb_path_info.beeper_freq = 0;

    RTD_Log(LOGGER_INFO, "EnableAudioClock\n");
    EnableAudioClock();
    RTD_Log(LOGGER_INFO, "AioInit_Step1\n");
    Audio_AioInit_Step1();
    RTD_Log(LOGGER_INFO, "Init Audio Interrupt\n");
    InitInterrupt();
    RTD_Log(LOGGER_INFO, "AppInit\n");
    Audio_AppInit();
    RTD_Log(LOGGER_INFO, "AtvInit\n");
    #ifdef _VIDEO_TV_SUPPORT
    Audio_AtvInit();
	#endif
#endif // CONFIG_ENABLE_AUDIO
}
/*=============================================================*/
/**
 * Audio_Init_Step2
 * Init audio decoder module.
 *
 * @param <void>	{ void }
 * @return 			{ void }
 * @ingroup drv_adc
 */
void Audio_Init_Step2()
{
#ifdef CONFIG_ENABLE_AUDIO
//	INT16			tick,i,vol,dir;
//	AIO_PATH_CFG	t_path_cfg;
//	APP_BEEP_CFG	t_beep_cfg;

    Audio_AioInit_Step2();

    /* testing

    Audio_AioSetIptSrc_BBADC(AIO_BBADC_AIN5_2);
    //    Audio_AioSetIptSrc_BBADC(AIO_BBADC_AIN2);
    t_path_cfg.id = AIO_PATH_ID_PB;
    t_path_cfg.src[0] = AIO_PATH_SRC_BBADC;
    t_path_cfg.src[1] = AIO_PATH_SRC_UNKNOWN;
    t_path_cfg.src[2] = AIO_PATH_SRC_UNKNOWN;
    t_path_cfg.src[3] = AIO_PATH_SRC_UNKNOWN;
    Audio_AioSetPathSrc(&t_path_cfg);

    Audio_HwpSetVolume(APP_CH_ID_L | APP_CH_ID_R, 0);
    Audio_HwpSetMute(_DISABLE, APP_CH_ID_L | APP_CH_ID_R, AIO_MUTE_ID);

    // mute control testing
    Audio_TimeDelay_ms(1000);
    Audio_HwpSetMute(_ENABLE, APP_CH_ID_L, AIO_MUTE_ID);
    Audio_TimeDelay_ms(1000);
    Audio_HwpSetMute(_ENABLE, APP_CH_ID_R, AIO_MUTE_ID);
    Audio_TimeDelay_ms(1000);
    Audio_HwpSetMute(_DISABLE, APP_CH_ID_L | APP_CH_ID_R, AIO_MUTE_ID);

    // volume control testing
    dir = APP_VOL_STEP_1DB;
    for(i=0,tick=0,vol=0; i<(75*4); i++) {
    	tick = i % 75;	// MIN_VOL = - 72*APP_VOL_STEP_1DB;

    	if(tick == 0) {
    		dir = -dir;
    	}
    	vol += dir;

    	RTD_Log(LOGGER_INFO, "Tick: %d  ,Vol:%d dB\n",(INT32)(tick + 1), (INT32)(vol/APP_VOL_STEP_1DB));
    	Audio_HwpSetVolume(APP_CH_ID_L | APP_CH_ID_R, (INT16)vol);
    	Audio_TimeDelay_ms(250);
    }

    // balance control testing
    dir = APP_VOL_STEP_1DB;
    for(i=0,vol=0; i<(55*4); i++) {

    	if((vol >= APP_MAX_BAL) || (vol <= APP_MIN_BAL)) {
    		dir = -dir;
    	}
    	vol += dir;

    	RTD_Log(LOGGER_INFO, "Tick: %d  ,Bal:%d dB\n",(INT32)(i + 1), (INT32)(vol/APP_VOL_STEP_1DB));
    	Audio_HwpSetBalanceLR(vol, TRUE);
    	Audio_TimeDelay_ms(250);
    }*/

#endif // CONFIG_ENABLE_AUDIO
}

/*==================== System Routines ========================*/
/*=============================================================*/
/**
 * Audio_TimeDelay_us
 *
 * @param <us>		{ time ticks }
 * @return 			{ void }
 * @ingroup drv_adc
 */
void Audio_TimeDelay_us(UINT16 us)
{
#ifdef CONFIG_ENABLE_AUDIO
    UINT16	i,count_value;

    if (us < 100) us = 100;

    if (us)
    {
        count_value = (us/100)*AIO_TIMEDLY_100US;
        if (count_value > 0)
        {
            i= count_value;
            while (i--);
        }
    }
#endif // CONFIG_ENABLE_AUDIO
}

/**
 * Audio_TimeDelay_ms
 *
 * @param <ms>		{ time ticks }
 * @return 			{ void }
 * @ingroup drv_adc
 */
void Audio_TimeDelay_ms(UINT16 ms)
{
#ifdef CONFIG_ENABLE_AUDIO
    UINT16	i = ms;
    // TODO:
    // check if it is possible to replace by fw_timer_event_DelayXms, except to in ISR
    while (i--)
    {
        Audio_TimeDelay_us(1000);
    }
#endif // CONFIG_ENABLE_AUDIO
}
/*=============================================================*/
/**
 * Audio_EnterCriticalSection
 * disable audio interrupt by cleaning the IE bit in Global IE register
 *
 * @param <void>	{ void }
 * @return 			{ void }
 * @ingroup drv_adc
 */
void Audio_EnterCriticalSection()
{
#ifdef CONFIG_ENABLE_AUDIO
    drv_isr_Enable(SYS_ISR_AUDIO, _DISABLE);

    m_critical_cntr++;
#endif // CONFIG_ENABLE_AUDIO
}
/*=============================================================*/
/**
 * Audio_LeaveCriticalSection
 * enable audio interrupt by setting the IE bit in Global IE register
 *
 * @param <void>	{ void }
 * @return 			{ void }
 * @ingroup drv_adc
 */
void Audio_LeaveCriticalSection()
{
#ifdef CONFIG_ENABLE_AUDIO
    if (m_critical_cntr == 0)
        return;

    m_critical_cntr--;

    if (m_critical_cntr == 0)
        drv_isr_Enable(SYS_ISR_AUDIO, _ENABLE);

#endif // CONFIG_ENABLE_AUDIO
}

/*=============================================================*/
/**
 * Audio_ISR_Dispatcher
 * audio interrupt service routine
 *
 * @param <void>	{ void }
 * @return 			{ succeed in handling interrupt or not }
 * @ingroup drv_adc
 */
void Audio_ISR_Dispatcher()
{
#ifdef CONFIG_ENABLE_AUDIO
    system_int_en_RBUS			HWP_INT_ST_REG;
    interrupt_en_RBUS			AIO_INT_EN_REG;
    interrupt_status_RBUS		AIO_INT_ST_REG;

#if 0	// move to other funciton
    if (!drv_isr_GetPending(SYS_ISR_AUDIO))
        return RT_ISR_UNHANDLED;
#endif

    HWP_INT_ST_REG.regValue = rtd_inl(SYSTEM_INT_EN_0x37b4);
    if (HWP_INT_ST_REG.bitField.eq_ovf && HWP_INT_ST_REG.bitField.eq_ovf_en)
    {
        //drv_audio_eq_overflow_int_handler();
    }
    if (HWP_INT_ST_REG.bitField.loudness_ovf && HWP_INT_ST_REG.bitField.loudness_ovf_en)
    {
        //drv_audio_loud_overflow_int_handler();
    }
    if (HWP_INT_ST_REG.bitField.dvol_done && HWP_INT_ST_REG.bitField.dvol_int_en)
    {
        //drv_audio_dvol_done_int_handler();
    }

    AIO_INT_EN_REG.regValue = rtd_inl(INTERRUPT_EN_0x3050);
    AIO_INT_ST_REG.regValue = rtd_inl(INTERRUPT_STATUS_0x3054);

    if (AIO_INT_ST_REG.bitField.int_spi_in_datatype_change && AIO_INT_EN_REG.bitField.int_spi_in_datatype_change_en)
    {
        //RTD_Log(LOGGER_DEBUG,"Audio Interrupt!![1]\n");
        Audio_ISR_SPDIF_DataType_Change();
    }
    if (AIO_INT_ST_REG.bitField.int_spi_in_lock_change && AIO_INT_EN_REG.bitField.int_spi_in_lock_change_en)
    {
        //RTD_Log(LOGGER_DEBUG,"Audio Interrupt!![2]\n");
        Audio_ISR_SPDIF_LockStatus_Change();
    }
    if (AIO_INT_ST_REG.bitField.int_i2s_pri_word_length_change && AIO_INT_EN_REG.bitField.int_i2s_pri_word_length_change_en)
    {
        //RTD_Log(LOGGER_DEBUG,"Audio Interrupt!![7]\n");
        Audio_ISR_I2S_WordLength_Change();
    }
    if (AIO_INT_ST_REG.bitField.int_i2s_pri_sample_rate_chg && AIO_INT_EN_REG.bitField.int_i2s_pri_sample_rate_chg_en)
    {
        //RTD_Log(LOGGER_DEBUG,"Audio Interrupt!![3]\n");
        Audio_ISR_I2S_SampleRate_Change();
    }

#ifdef _VIDEO_TV_SUPPORT
//	if(AIO_INT_ST_REG.bitField.int_sif_nopt) {
//		Audio_ISR_ATV_SIF_Under_Optimize();
//	}
    if (AIO_INT_ST_REG.bitField.int_msd_change && AIO_INT_EN_REG.bitField.int_msd_change_en)
    {
        //RTD_Log(LOGGER_DEBUG,"Audio Interrupt!! Audio_ISR_ATV_AutoStdDetect_Change\n");
        Audio_ISR_ATV_AutoStdDetect_Change();
    }
    if (AIO_INT_ST_REG.bitField.int_btsc_mode_chg && AIO_INT_EN_REG.bitField.int_btsc_mode_chg_en)
    {
        Audio_ISR_ATV_BTSC_ModeStatus_Change();
    }
    if (AIO_INT_ST_REG.bitField.int_mode_det_status_change && AIO_INT_EN_REG.bitField.int_mode_det_status_change_en)
    {
        //RTD_Log(LOGGER_DEBUG,"Audio Interrupt!! Audio_ISR_ATV_ModeStatus_Change\n");
        Audio_ISR_ATV_ModeStatus_Change();
    }
    if (AIO_INT_ST_REG.bitField.int_nicam_hierr_change && AIO_INT_EN_REG.bitField.int_nicam_hierr_change_en)
    {
        Audio_ISR_ATV_NICAM_HiErrRate_Change();
    }
#endif
//	return RT_ISR_HANDLED;

#endif // CONFIG_ENABLE_AUDIO
}


void Audio_AioSetPathSrc(AIO_PATH_CFG *p_path_cfg)
{
#ifndef CONFIG_ENABLE_AUDIO
    p_path_cfg = p_path_cfg;
#else
    spdif_in_RBUS		SPDIFI_CTRL_REG;
    i2s_in_primary_RBUS	I2S_CTRL1_REG;
    UINT32				temp_in_sel;
    UINT8				temp_src[4];
    UINT8				i;

//	RTD_Log(LOGGER_INFO,"[AUDIO][AIO] aio_id:%x\n",(UINT32)p_path_cfg->id);
//	RTD_Log(LOGGER_INFO,"[AUDIO][AIO] src[0]:%x\n",(UINT32)p_path_cfg->src[0]);
//	RTD_Log(LOGGER_INFO,"[AUDIO][AIO] src[1]:%x\n",(UINT32)p_path_cfg->src[1]);
//	RTD_Log(LOGGER_INFO,"[AUDIO][AIO] src[2]:%x\n",(UINT32)p_path_cfg->src[2]);
//	RTD_Log(LOGGER_INFO,"[AUDIO][AIO] src[3]:%x\n",(UINT32)p_path_cfg->src[3]);


    if (p_path_cfg->id == AIO_PATH_ID_PB)
    {
//		RTD_Log(LOGGER_INFO,"[AUDIO][AIO] path is playback\n");
        // disable all input interrupts of the path.
        Audio_AioSetInterrupt(_DISABLE, AIO_INT_ID_ALL);

        Audio_AioSetBackendPlay(_DISABLE);

        Audio_HwpSetMute(_DISABLE, APP_CH_ID_L | APP_CH_ID_R, ATV_MUTE_ID);
        Audio_HwpSetMute(_DISABLE, APP_CH_ID_L | APP_CH_ID_R, AIO_MUTE_ID);
        Audio_HwpSetMute(_DISABLE, APP_CH_ID_L | APP_CH_ID_R, APP_MUTE_ID);


        // reset F/W SPDIF Info
        pb_path_info.spdifi_lock_flag = FALSE;
        pb_path_info.spdifi_prev_fs   = AIO_FS_ID_UNKNOWN;
        //fw_timer_event_CancelTimerEvent(Timerevent_Check_SPDIF_LockStatus);

        pb_path_info.atv_fw_scan_flag = FALSE;
//		fw_timer_event_CancelTimerEvent(Audio_AtvTimerEventFwScan);
#ifdef _VIDEO_TV_SUPPORT
        fw_timer_event_CancelTimerEvent(timerevent_table_audio_atv_freq_scan_event);
#endif
        // TODO: dragon will reset PLL_512FS here


        for (i=0; i<4; i++)
        {
            pb_path_info.src[i] = temp_src[i] = p_path_cfg->src[i];
        }

        switch (pb_path_info.src[0])
        {
        case AIO_PATH_SRC_ATV_NICAM:// shared input(ATV), affect clk setting
        case AIO_PATH_SRC_ATV_HDEV:	// shared input(ATV), affect clk setting
        case AIO_PATH_SRC_ATV:		// real input, mapping to register bit-setting
            if (p_path_cfg->src[0] == AIO_PATH_SRC_ATV_NICAM)
            {
                pb_path_info.curr_ipt_fs = AIO_FS_ID_32K;
            }
            else if (p_path_cfg->src[0] == AIO_PATH_SRC_ATV)
            {
                pb_path_info.curr_ipt_fs = AIO_FS_ID_48K;
            }

            Audio_AioSetPathClock(AIO_PATH_SRC_ATV);
            temp_src[0] = AIO_PATH_SRC_ATV;
            SetPbDataSrcSel(&temp_src[0]);
            Audio_AioSetInterrupt(_ENABLE, (AIO_INT_ID_MSD_CHG  | AIO_INT_ID_MODE_CHG ));
            Audio_AioSetBackendPlay(_ENABLE);
            SetPeakSrcSel(AIO_PEAK_SRC_ATV);
            //Audio_HwpSetMute(_DISABLE, APP_CH_ID_ALL, AUD_DRV_MUTE);
            break;

        case AIO_PATH_SRC_BBADC:	// real input, mapping to register bit-setting
            /*
             * BBADC also can run at 96kHz/192kHz(controled by adc_512fs_sel),
             * but I assume it at 48kHz in this version. (Ivan)
             */
            pb_path_info.curr_ipt_fs = AIO_FS_ID_48K;
            Audio_AioSetPathClock(AIO_PATH_SRC_BBADC);
            SetPbDataSrcSel(&temp_src[0]);
            Audio_AioSetBackendPlay(_ENABLE);
            SetPeakSrcSel(AIO_PEAK_SRC_BBADC);
            Audio_HwpSetMute(_DISABLE, APP_CH_ID_ALL, AUD_DRV_MUTE);
            break;

        case AIO_PATH_SRC_SPDIF:	// real input, mapping to register bit-setting
            // disable SPDIF_IN to trigger lock/unlock again
            SPDIFI_CTRL_REG.regValue = rtd_inl(SPDIF_IN_0x3130);
            temp_in_sel = SPDIFI_CTRL_REG.bitField.source_sel;
            SPDIFI_CTRL_REG.bitField.source_sel = AIO_SPDIFI_DISABLE;
            rtd_outl(SPDIF_IN_0x3130, SPDIFI_CTRL_REG.regValue);
            Audio_TimeDelay_ms(5);		// long enough ?!
            SPDIFI_CTRL_REG.bitField.source_sel = temp_in_sel;
            rtd_outl(SPDIF_IN_0x3130, SPDIFI_CTRL_REG.regValue);

            Audio_AioSetPathClock(AIO_PATH_SRC_SPDIF);
            SetSPDIF_DigitalTracking(_ENABLE);
            SetPbDataSrcSel(&temp_src[0]);
            Audio_AioSetInterrupt(_ENABLE, (AIO_INT_ID_SPDIF_LK_CHG | AIO_INT_ID_SPDIF_DT_CHG | AIO_INT_ID_SPDIF_SR_CHG));
            SetPeakSrcSel(AIO_PEAK_SRC_SPDIF);
            break;

        case AIO_PATH_SRC_PCM:		// real input, mapping to register bit-setting
            SetPeakSrcSel(AIO_PEAK_SRC_PCM);
            break;
        case AIO_PATH_SRC_I2S_CH12:	// real input, mapping to register bit-setting
        case AIO_PATH_SRC_I2S_CH34:	// real input, mapping to register bit-setting
        case AIO_PATH_SRC_I2S_CH56:	// real input, mapping to register bit-setting
        case AIO_PATH_SRC_I2S_CH78:	// real input, mapping to register bit-setting
            // disable I2S_IN to trigger sr_chg/wl_chg again
            I2S_CTRL1_REG.regValue = rtd_inl(I2S_IN_PRIMARY_0x3100);
            temp_in_sel = I2S_CTRL1_REG.bitField.source_sel;
            I2S_CTRL1_REG.bitField.source_sel = AIO_I2SI_DISABLE;
            rtd_outl(I2S_IN_PRIMARY_0x3100, I2S_CTRL1_REG.regValue);
            Audio_TimeDelay_ms(5);		// long enough ?!
            I2S_CTRL1_REG.bitField.source_sel = temp_in_sel;
            rtd_outl(I2S_IN_PRIMARY_0x3100, I2S_CTRL1_REG.regValue);

            // keep clk in pll_audio until sample_rate_int. or word_lenth_int
            pb_path_info.curr_ipt_fs = AIO_FS_ID_48K;
            Audio_AioSetPathClock(AIO_PATH_SRC_BBADC);

            SetPbDataSrcSel(&temp_src[0]);
            SPDIFI_CTRL_REG.regValue = rtd_inl(SPDIF_IN_0x3130);
            if (SPDIFI_CTRL_REG.bitField.source_sel == AIO_SPDIFI_HDMI)
            {
                // disable SPDIF_IN to trigger lock/unlock again
                SPDIFI_CTRL_REG.regValue = rtd_inl(SPDIF_IN_0x3130);
                temp_in_sel = SPDIFI_CTRL_REG.bitField.source_sel;
                SPDIFI_CTRL_REG.bitField.source_sel = AIO_SPDIFI_DISABLE;
                rtd_outl(SPDIF_IN_0x3130, SPDIFI_CTRL_REG.regValue);
                Audio_TimeDelay_us(100);		// long enough ?!
                SPDIFI_CTRL_REG.bitField.source_sel = temp_in_sel;
                rtd_outl(SPDIF_IN_0x3130, SPDIFI_CTRL_REG.regValue);
                // enable DATA_TYPE_CHANGE_INT for NON-PCM detection
                Audio_AioSetInterrupt(_ENABLE,(AIO_INT_ID_I2S1_SR_CHG | AIO_INT_ID_I2S1_WL_CHG | AIO_INT_ID_SPDIF_DT_CHG));
            }
            else
            {
                Audio_AioSetInterrupt(_ENABLE,(AIO_INT_ID_I2S1_SR_CHG | AIO_INT_ID_I2S1_WL_CHG));
            }
            SetPeakSrcSel(AIO_PEAK_SRC_I2S);

            break;
        default:
            break;
        }
    }
    else
    {
        RTD_Log(LOGGER_INFO,"[AUDIO][AIO] path is SCART\n");
        SetTsDataSrcSel(p_path_cfg->src[0]);
    }

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * setup audio clock for playback
 *
 * This function will be called by Task and ISR directly.
 * Therefore,register need to be protected for atom-accessing
 *
 * @return				{ void }
 * @ingroup drv_audio
 */
void Audio_AioSetPathClock(UINT8 src)
{
#ifndef CONFIG_ENABLE_AUDIO
    src = src;
#else
    global_ctrl_RBUS						GLOBAL_CTRL_REG;
    i2s_in_primary_RBUS						I2SI_CTRL1_REG;
    down_sample_control_for_playback_RBUS	DOWN_PB_CTRL_REG;
    INT8	ret;
    UINT8	adc_sel = AIO_ADC_512FS_24576;

    GLOBAL_CTRL_REG.regValue = rtd_inl(GLOBAL_CTRL_0x3004);

    switch (src)
    {
    case AIO_PATH_SRC_I2S_CH12:
    case AIO_PATH_SRC_I2S_CH34:
    case AIO_PATH_SRC_I2S_CH56:
    case AIO_PATH_SRC_I2S_CH78:
        I2SI_CTRL1_REG.regValue = rtd_inl(I2S_IN_PRIMARY_0x3100);
        if (I2SI_CTRL1_REG.bitField.source_sel == AIO_I2SI_HDMI)
        {
            GLOBAL_CTRL_REG.bitField.sel_i2s_hdmi = AIO_I2S_CLK_INT_HDMI;
        }
        else
        {
            GLOBAL_CTRL_REG.bitField.sel_i2s_hdmi = AIO_I2S_CLK_EXT_I2S;
        }
        GLOBAL_CTRL_REG.bitField.sel_i2s_spdif = 0;
        ret = SetPLL_I2S(pb_path_info.curr_ipt_fs);
        if (ret < 0)
            GLOBAL_CTRL_REG.bitField.sel_play_ck_source = AIO_PLL_AUDIO;
        else
            GLOBAL_CTRL_REG.bitField.sel_play_ck_source = AIO_PLL_64_512;
        break;

    case AIO_PATH_SRC_SPDIF:
        ret = SetPLL_SPDIF(pb_path_info.curr_ipt_fs);
        if (ret < 0)
        {
            // unsupport fs, force clk to PLL_AUDIO
            GLOBAL_CTRL_REG.bitField.sel_i2s_spdif = 0;
            GLOBAL_CTRL_REG.bitField.sel_play_ck_source = AIO_PLL_AUDIO;
        }
        else if (ret == 1)
        {
            // special mode, connect PLL_64_512 and PLL_SPDIF together
            GLOBAL_CTRL_REG.bitField.sel_i2s_spdif = 1;
            GLOBAL_CTRL_REG.bitField.sel_play_ck_source = AIO_PLL_64_512;
        }
        else
        {
            // normal mode, use PLL_SPDIF
            GLOBAL_CTRL_REG.bitField.sel_i2s_spdif = 0;
            GLOBAL_CTRL_REG.bitField.sel_play_ck_source = AIO_PLL_SPDIF_TRACKING;
        }

        GLOBAL_CTRL_REG.bitField.spdif_nicam = AIO_SPDIF_CLK_SPDIF;
        break;
    case AIO_PATH_SRC_ATV_NICAM:	// shared input(TV), only affect clk setting
        GLOBAL_CTRL_REG.bitField.sel_play_ck_source = AIO_PLL_NICAM_RESAMPLE;
        break;
    case AIO_PATH_SRC_ATV_HDEV:		// shared input(TV), only affect clk setting
        adc_sel = AIO_ADC_512FS_49152;
    case AIO_PATH_SRC_ATV:
    case AIO_PATH_SRC_BBADC:
    default:
        GLOBAL_CTRL_REG.bitField.sel_play_ck_source = AIO_PLL_AUDIO;
        break;
    }
    GLOBAL_CTRL_REG.bitField.adc_512fs_sel = adc_sel;
    // set down-sample FIFO
    DOWN_PB_CTRL_REG.regValue = rtd_inl(DOWN_SAMPLE_CONTROL_FOR_PLAYBACK_0x3550);
    if ((pb_path_info.max_playback_fs < pb_path_info.curr_ipt_fs) && (pb_path_info.curr_ipt_fs != AIO_FS_ID_UNKNOWN))
    {
        /*
        ROSPrintf("max_fs=%d,curr_ifs=%d,curr_ofs=%d\n",
        		path_cfg->max_playback_fs,
        		path_cfg->curr_ipt_fs,
        		path_cfg->curr_opt_fs
        	);
        */
        DOWN_PB_CTRL_REG.bitField.downsample_en = TRUE;
        DOWN_PB_CTRL_REG.bitField.down_rate = AIO_DOWN_SAMPLE_DIV_2;
        pb_path_info.curr_opt_fs = pb_path_info.curr_ipt_fs - AIO_DOWN_SAMPLE_STEP;
        if (pb_path_info.max_playback_fs < pb_path_info.curr_opt_fs)
        {
            pb_path_info.curr_opt_fs -= AIO_DOWN_SAMPLE_STEP;
            DOWN_PB_CTRL_REG.bitField.down_rate = AIO_DOWN_SAMPLE_DIV_4;
        }
    }
    else
    {
        pb_path_info.curr_opt_fs = pb_path_info.curr_ipt_fs;
        DOWN_PB_CTRL_REG.bitField.downsample_en = FALSE;
    }
    rtd_outl(DOWN_SAMPLE_CONTROL_FOR_PLAYBACK_0x3550, DOWN_PB_CTRL_REG.regValue);

    // set down-conversion for DAC
    if (pb_path_info.curr_opt_fs > AIO_FS_ID_96K)
        GLOBAL_CTRL_REG.bitField.sel_down_conv = AIO_DAAD_DOWN_DIV_4;
    else if (pb_path_info.curr_opt_fs > AIO_FS_ID_48K)
        GLOBAL_CTRL_REG.bitField.sel_down_conv = AIO_DAAD_DOWN_DIV_2;
    else
        GLOBAL_CTRL_REG.bitField.sel_down_conv = AIO_DAAD_DOWN_DIV_1;

    // back_end_play sample-rate
    if (pb_path_info.curr_ipt_fs > AIO_FS_ID_96K)
        GLOBAL_CTRL_REG.bitField.back_end_play_sample_rate = AIO_PNB_FS_DIV_4;
    else if (pb_path_info.curr_ipt_fs > AIO_FS_ID_48K)
        GLOBAL_CTRL_REG.bitField.back_end_play_sample_rate = AIO_PNB_FS_DIV_2;
    else
        GLOBAL_CTRL_REG.bitField.back_end_play_sample_rate = AIO_PNB_FS_DIV_1;

    rtd_outl(GLOBAL_CTRL_0x3004, GLOBAL_CTRL_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}


/**
 * I2S sample-rate change ISR. It is assumed that this interrupt is only enabled when
 * playback path is directed to I2S input.
 *
 * @param 	<void> 	{ void }
 * @return			{ void }
 * @ingroup drv_audio
 */
void Audio_ISR_I2S_SampleRate_Change(void)
{
#ifdef CONFIG_ENABLE_AUDIO
    interrupt_status_RBUS	INT_ST_REG;
    i2s_in_primary_RBUS		I2SI_CTRL1_REG;
    UINT8					curr_fs;

    if (pb_path_info.src[0] != AIO_PATH_SRC_I2S_CH12)
    {
//		printk("Skip int. coz path_src is not I2S\n");
        return;
    }

    Audio_AioSetBackendPlay(_DISABLE);

    // change clock source to PLL_AUDIO
    pb_path_info.curr_ipt_fs = AIO_FS_ID_48K;
    Audio_AioSetPathClock(AIO_PATH_SRC_BBADC);
    Audio_HwpSetMute(_ENABLE, APP_CH_ID_ALL, AUD_DRV_MUTE);

    I2SI_CTRL1_REG.regValue = rtd_inl(I2S_IN_PRIMARY_0x3100);

    curr_fs = i2s_map_to_aio_fs_id[I2SI_CTRL1_REG.bitField.sampling_rate];

    // turn on/off debounce funtion
    if ((curr_fs == AIO_FS_ID_192K) && (I2SI_CTRL1_REG.bitField.pcm_width == 32))
    {
        I2SI_CTRL1_REG.bitField.debounce_en = FALSE;
    }
    else
    {
        I2SI_CTRL1_REG.bitField.debounce_en = TRUE;
    }
    rtd_outl(I2S_IN_PRIMARY_0x3100, I2SI_CTRL1_REG.regValue);

    /*
     * update PLL & clock setting
     * in this version,LasVegas only support two kind of PCM length, 24bit and 32bit.
     */
    if ( (curr_fs <= AIO_FS_ID_192K) &&
            ((I2SI_CTRL1_REG.bitField.pcm_width == 32) || (I2SI_CTRL1_REG.bitField.pcm_width == 24)))
    {
        pb_path_info.curr_ipt_fs = curr_fs;

        Audio_AioSetPathClock(pb_path_info.src[0]);
        Audio_AioSetBackendPlay(_ENABLE);
        Audio_HwpSetMute(_DISABLE, APP_CH_ID_ALL, AUD_DRV_MUTE);
    }

    // clean interrupt pending bit
    INT_ST_REG.regValue = 0;
    INT_ST_REG.bitField.int_i2s_pri_sample_rate_chg = TRUE;	// write one to clear
    rtd_outl(INTERRUPT_STATUS_0x3054, INT_ST_REG.regValue);
#endif // CONFIG_ENABLE_AUDIO
}

/**
 * I2S word-length change ISR. It is assumed that this interrupt is only enabled when
 * playback path is directed to I2S input.
 *
 * @param 	<void> 	{ void }
 * @return			{ void }
 * @ingroup drv_audio
 */
void Audio_ISR_I2S_WordLength_Change(void)
{
#ifdef CONFIG_ENABLE_AUDIO
    interrupt_status_RBUS	INT_ST_REG;
    i2s_in_primary_RBUS		I2SI_CTRL1_REG;
    UINT8					curr_fs;

    if (pb_path_info.src[0] != AIO_PATH_SRC_I2S_CH12)
    {
//		printk("Skip int. coz path_src is not I2S\n");
        return;
    }

    Audio_AioSetBackendPlay(_DISABLE);

    // change clock source to PLL_AUDIO
    pb_path_info.curr_ipt_fs = AIO_FS_ID_48K;
    Audio_AioSetPathClock(AIO_PATH_SRC_BBADC);
    Audio_HwpSetMute(_ENABLE, APP_CH_ID_ALL, AUD_DRV_MUTE);

    I2SI_CTRL1_REG.regValue = rtd_inl(I2S_IN_PRIMARY_0x3100);

    curr_fs = i2s_map_to_aio_fs_id[I2SI_CTRL1_REG.bitField.sampling_rate];

    // turn on/off debounce funtion
    if ((curr_fs == AIO_FS_ID_192K) && (I2SI_CTRL1_REG.bitField.pcm_width == 32))
    {
        I2SI_CTRL1_REG.bitField.debounce_en = FALSE;
    }
    else
    {
        I2SI_CTRL1_REG.bitField.debounce_en = TRUE;
    }
    rtd_outl(I2S_IN_PRIMARY_0x3100, I2SI_CTRL1_REG.regValue);

    /*
     * update PLL & clock setting
     * in this version,LasVegas only support two kind of PCM length, 24bit and 32bit.
     */
    if ( (curr_fs <= AIO_FS_ID_192K) &&
            ((I2SI_CTRL1_REG.bitField.pcm_width == 32) || (I2SI_CTRL1_REG.bitField.pcm_width == 24)))
    {
        pb_path_info.curr_ipt_fs = curr_fs;

        Audio_AioSetPathClock(pb_path_info.src[0]);
        Audio_AioSetBackendPlay(_ENABLE);
        Audio_HwpSetMute(_DISABLE, APP_CH_ID_ALL, AUD_DRV_MUTE);
    }

    // clean interrupt pending bit
    INT_ST_REG.regValue = 0;
    INT_ST_REG.bitField.int_i2s_pri_word_length_change = TRUE;	// write one to clear
    rtd_outl(INTERRUPT_STATUS_0x3054, INT_ST_REG.regValue);

#endif // CONFIG_ENABLE_AUDIO
}

#if 0 // marked by wiehao, because of not used 980727
/**
 * SPDIF sample-rate change ISR. It is assumed that this interrupt is only enabled when
 * playback path is directed to SPDIF input.
 *
 * @param 	<void> 	{ void }
 * @return			{ void }
 * @ingroup drv_audio
 */
void Audio_ISR_SPDIF_SampleRate_Change(void)
{
#ifdef CONFIG_ENABLE_AUDIO
    interrupt_status_RBUS	INT_ST_REG;
    spdif_in_cs_RBUS		SPDIFI_CS_REG;
    spdif_in_RBUS			SPDIFI_CTRL_REG;
    spdif_out_RBUS			SPDIFO_CTRL_REG;
    UINT8					curr_fs;

    if (pb_path_info.src[0] != AIO_PATH_SRC_SPDIF)
    {
//		printk("Skip int. coz path_src is not SPDIF\n");
        return;
    }

    Audio_AioSetBackendPlay(_DISABLE);
    pb_path_info.curr_ipt_fs = AIO_FS_ID_48K;
    Audio_AioSetPathClock(AIO_PATH_SRC_BBADC);
    Audio_HwpSetMute(_ENABLE, APP_CH_ID_ALL, AUD_DRV_MUTE);

    SPDIFI_CS_REG.regValue = rtd_inl(SPDIF_IN_CS_0x3134);
    if (SPDIFI_CS_REG.bitField.hw_locked)
    {
        pb_path_info.spdifi_lock_flag = TRUE;
        SPDIFI_CTRL_REG.regValue = rtd_inl(SPDIF_IN_0x3130);

        curr_fs = spdif_map_to_aio_fs_id[SPDIFI_CTRL_REG.bitField.sampling_rate];

        // this version always trusts h/w tracking result
        // turn on/off debounce function
        if (curr_fs == AIO_FS_ID_192K)
            SPDIFI_CTRL_REG.bitField.debounce_en = FALSE;
        else
            SPDIFI_CTRL_REG.bitField.debounce_en = TRUE;
        rtd_outl(SPDIF_IN_0x3130, SPDIFI_CTRL_REG.regValue);

        // update PLL & clock setting
        pb_path_info.spdifi_prev_fs = curr_fs;
        pb_path_info.curr_ipt_fs = curr_fs;
        Audio_AioSetPathClock(pb_path_info.src[0]);

        if ((curr_fs <= AIO_FS_ID_192K) && SPDIFI_CS_REG.bitField.data_type == AIO_DATA_IS_PCM)
        {
            Audio_AioSetBackendPlay(_ENABLE);

#if 1 // TODO: can we issue a timer event in ISR ??!
            fw_timer_event_ActiveTimerEvent(100, Timerevent_Check_SPDIF_LockStatus);
#else
            Audio_HwpSetMute(_DISABLE, APP_CH_ID_ALL, AUD_DRV_MUTE);
#endif
        }
        else if (SPDIFI_CS_REG.bitField.data_type == AIO_DATA_IS_NONPCM)
        {
            // force bypass NON-PCM according to input selection
            SPDIFI_CTRL_REG.regValue = rtd_inl(SPDIF_IN_0x3130);
            SPDIFO_CTRL_REG.regValue = rtd_inl(SPDIF_OUT_0x3170);
            SPDIFO_CTRL_REG.bitField.output_sel = (SPDIFI_CTRL_REG.bitField.source_sel == AIO_SPDIFI_IN) ? AIO_SPDIFO_IN : AIO_SPDIFO_HDMI;
            rtd_outl(SPDIF_OUT_0x3170, SPDIFO_CTRL_REG.regValue);
        }

    }
    else
    {
        pb_path_info.spdifi_lock_flag = FALSE;
    }

    INT_ST_REG.regValue = 0;
    INT_ST_REG.bitField.int_spi_sample_rate_chg = TRUE;	// write one to clear
    rtd_outl(INTERRUPT_STATUS_0x3054, INT_ST_REG.regValue);
#endif // CONFIG_ENABLE_AUDIO
}
#endif

/**
 * SPDIF data-type change ISR. It is assumed that this interrupt is only enabled when
 * playback path is directed to SPDIF input.
 *
 * @param 	<void> 	{ void }
 * @return			{ void }
 * @ingroup drv_audio
 */
void Audio_ISR_SPDIF_DataType_Change(void)
{
#ifdef CONFIG_ENABLE_AUDIO
    interrupt_status_RBUS	INT_ST_REG;
    spdif_in_cs_RBUS		SPDIFI_CS_REG;
    spdif_in_RBUS			SPDIFI_CTRL_REG;
    spdif_out_RBUS			SPDIFO_CTRL_REG;

    if ((pb_path_info.src[0] != AIO_PATH_SRC_SPDIF) && (pb_path_info.src[0] != AIO_PATH_SRC_I2S_CH12))
    {
//		printk("Skip int. coz path_src is not SPDIF/I2S_CH12\n");
        return;
    }

    SPDIFI_CS_REG.regValue = rtd_inl(SPDIF_IN_CS_0x3134);
    if (SPDIFI_CS_REG.bitField.hw_locked)
    {
        pb_path_info.spdifi_lock_flag = TRUE;
        if (SPDIFI_CS_REG.bitField.data_type == AIO_DATA_IS_NONPCM)
        {
            Audio_HwpSetMute(_ENABLE, APP_CH_ID_ALL, AUD_DRV_MUTE);		// when to unmute ??!
            // force bypass NON-PCM according to input selection
            SPDIFI_CTRL_REG.regValue = rtd_inl(SPDIF_IN_0x3130);
            SPDIFO_CTRL_REG.regValue = rtd_inl(SPDIF_OUT_0x3170);
            SPDIFO_CTRL_REG.bitField.output_sel = (SPDIFI_CTRL_REG.bitField.source_sel == AIO_SPDIFI_IN) ? AIO_SPDIFO_IN : AIO_SPDIFO_HDMI;
            rtd_outl(SPDIF_OUT_0x3170, SPDIFO_CTRL_REG.regValue);

            // patch for SPDIF interrupt debounce
            pb_path_info.spdifi_lock_flag = FALSE;
            pb_path_info.spdifi_prev_fs   = AIO_FS_ID_UNKNOWN;
        }
        else
        {
            // recover to original setting(by user setting or init. configuration)
            SPDIFO_CTRL_REG.regValue = rtd_inl(SPDIF_OUT_0x3170);
            SPDIFO_CTRL_REG.bitField.output_sel = pb_path_info.spdifo_src_sel;
            rtd_outl(SPDIF_OUT_0x3170, SPDIFO_CTRL_REG.regValue);
            // TODO: is it necessary to call isr_spdif_sample_rate_change() ??!
        }
    }
    else
    {
        pb_path_info.spdifi_lock_flag = FALSE;
        pb_path_info.spdifi_prev_fs   = AIO_FS_ID_UNKNOWN;
        // unreasonable register info., reset clk source to audio_pll
        Audio_AioSetBackendPlay(_DISABLE);
        pb_path_info.curr_ipt_fs = AIO_FS_ID_48K;
        Audio_AioSetPathClock(AIO_PATH_SRC_BBADC);
    }

    INT_ST_REG.regValue = 0;
    INT_ST_REG.bitField.int_spi_in_datatype_change = TRUE;	// write one to clear
    rtd_outl(INTERRUPT_STATUS_0x3054, INT_ST_REG.regValue);

#endif // CONFIG_ENABLE_AUDIO
}

/**
 * SPDIF lock-status change ISR. It is assumed that this interrupt is only enabled when
 * playback path is directed to SPDIF input.
 *
 * @param 	<void> 	{ void }
 * @return			{ void }
 * @ingroup drv_audio
 */
void Audio_ISR_SPDIF_LockStatus_Change(void)
{
#ifdef CONFIG_ENABLE_AUDIO
    interrupt_status_RBUS	INT_ST_REG;
    spdif_in_cs_RBUS		SPDIFI_CS_REG;
    spdif_in_RBUS			SPDIFI_CTRL_REG;
    spdif_out_RBUS			SPDIFO_CTRL_REG;
    UINT8					curr_fs;

    if (pb_path_info.src[0] != AIO_PATH_SRC_SPDIF)
    {
//		printk("Skip int. coz path_src is not SPDIF\n");
        return;
    }

    Audio_AioSetBackendPlay(_DISABLE);
    pb_path_info.curr_ipt_fs = AIO_FS_ID_48K;
    Audio_AioSetPathClock(AIO_PATH_SRC_BBADC);
    Audio_HwpSetMute(_ENABLE, APP_CH_ID_ALL, AUD_DRV_MUTE);


    SPDIFI_CS_REG.regValue = rtd_inl(SPDIF_IN_CS_0x3134);
    if (SPDIFI_CS_REG.bitField.hw_locked)
    {
        pb_path_info.spdifi_lock_flag = TRUE;

        SPDIFI_CTRL_REG.regValue = rtd_inl(SPDIF_IN_0x3130);

        curr_fs = spdif_map_to_aio_fs_id[SPDIFI_CTRL_REG.bitField.sampling_rate];

        // this version always trusts h/w tracking result
        // turn on/off debounce function
        if (curr_fs == AIO_FS_ID_192K)
            SPDIFI_CTRL_REG.bitField.debounce_en = FALSE;
        else
            SPDIFI_CTRL_REG.bitField.debounce_en = TRUE;
        rtd_outl(SPDIF_IN_0x3130, SPDIFI_CTRL_REG.regValue);

        // update PLL & clock setting
        pb_path_info.curr_ipt_fs = curr_fs;
        Audio_AioSetPathClock(pb_path_info.src[0]);

        if ((curr_fs <= AIO_FS_ID_192K) && SPDIFI_CS_REG.bitField.data_type == AIO_DATA_IS_PCM)
        {
            Audio_AioSetBackendPlay(_ENABLE);
            Audio_HwpSetMute(_DISABLE, APP_CH_ID_ALL, AUD_DRV_MUTE);
        }
        else if (SPDIFI_CS_REG.bitField.data_type == AIO_DATA_IS_NONPCM)
        {
            // force bypass NON-PCM according to input selection
            SPDIFI_CTRL_REG.regValue = rtd_inl(SPDIF_IN_0x3130);
            SPDIFO_CTRL_REG.regValue = rtd_inl(SPDIF_OUT_0x3170);
            SPDIFO_CTRL_REG.bitField.output_sel = (SPDIFI_CTRL_REG.bitField.source_sel == AIO_SPDIFI_IN) ? AIO_SPDIFO_IN : AIO_SPDIFO_HDMI;
            rtd_outl(SPDIF_OUT_0x3170, SPDIFO_CTRL_REG.regValue);
        }

    }
    else
    {
        pb_path_info.spdifi_lock_flag = FALSE;
    }

    INT_ST_REG.regValue = 0;
    INT_ST_REG.bitField.int_spi_in_lock_change = TRUE;	// write one to clear
    rtd_outl(INTERRUPT_STATUS_0x3054, INT_ST_REG.regValue);
#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * enable/disable ai interrupts
 *
 * This function will be called by Task and ISR directly.
 * Therefore,register need to be protected for atom-accessing
 *
 * @param <para>		{ AIO_ENABLE/AIO_DISABLE }
 * @param <src_sel>		{ which interrupts caller want to enable/disable }
 * @return				{ void }
 * @ingroup drv_audio
 */
void Audio_AioSetInterrupt(BOOL para, UINT32 src_sel)
{
#ifndef CONFIG_ENABLE_AUDIO
    para = para;
    src_sel = src_sel;
#else
    interrupt_en_RBUS		INT_EN_REG;

    INT_EN_REG.regValue = rtd_inl(INTERRUPT_EN_0x3050);

    if (para == _ENABLE)
    {
        INT_EN_REG.regValue |= src_sel;
    }
    else
    {
        INT_EN_REG.regValue &= ~src_sel;
    }

//	printk("[AUDIO_DRV] set int:enable=%d,src_sel=0x%08x,\n",para,src_sel);

    rtd_outl(INTERRUPT_EN_0x3050, INT_EN_REG.regValue);
#endif // CONFIG_ENABLE_AUDIO

}

/**
 * Turn on/off BackEndPlay
 *
 * This function will be called by Task and ISR directly.
 * Therefore,register need to be protected for atom-accessing
 *
 * @para	<para>	{ AIO_ENABLE / AIO_DISABLE }
 * @return			{ void }
 * @ingroup drv_audio
 */
void Audio_AioSetBackendPlay(BOOL para)
{
#ifndef CONFIG_ENABLE_AUDIO
    para = para;
#else
    down_sample_control_for_playback_RBUS	DOWN_PB_CTRL_REG;

    DOWN_PB_CTRL_REG.regValue = rtd_inl(DOWN_SAMPLE_CONTROL_FOR_PLAYBACK_0x3550);
    DOWN_PB_CTRL_REG.bitField.backend_play_en = para;
    rtd_outl(DOWN_SAMPLE_CONTROL_FOR_PLAYBACK_0x3550, DOWN_PB_CTRL_REG.regValue);
#endif // CONFIG_ENABLE_AUDIO
}


/*===================== Local Subroutine ======================*/
#ifdef CONFIG_ENABLE_AUDIO
/*=============================================================*/
/**
 * EnableAudioClock
 * enable all audio-related clock source(audio PLL & 512fs PLL) and ungated them.
 *
 * @param <void>	{ void }
 * @return 			{ void }
 * @ingroup drv_adc
 */
static void EnableAudioClock()
{
    global_ctrl_RBUS	GLOBAL_CTRL_REG;

    // enable audio-related clock
    rtdf_setBitsb(SYS_CLKEN_1_0x2cc1, (SYS_CLKEN_1_AUD_49MHZ_BIT | SYS_CLKEN_1_AUD_98MHZ_BIT));

    rtdf_setBitsb(SYS_CLKEN_2_0x2cc2, (SYS_CLKEN_2_AUD_16MHZ_BIT | SYS_CLKEN_2_AUD_24MHZ_BIT));

    rtdf_setBitsb(SYS_CLKEN_4_0x2cc4,
                  (SYS_CLKEN_4_AUD_PRE_512FS_BIT |
                   SYS_CLKEN_4_AUD_DAAD_256FS_BIT |
                   SYS_CLKEN_4_AUD_DAAD_128FS_BIT |
                   SYS_CLKEN_4_AUD_BIT |
                   SYS_CLKEN_4_AUD2_512FS_BIT |
                   SYS_CLKEN_4_AUD2_DAAD_256FS_BIT |
                   SYS_CLKEN_4_AUD2_DAAD_128FS_BIT));

    // disable audio-related reset status
    rtdf_setBitsb(SYS_SRST_1_0x2ca1,
                  (SYS_SRST_1_AUD_DAC2_BIT |
                   SYS_SRST_1_AUD_DAC1_BIT |
                   SYS_SRST_1_AUD_ADC_BIT |
                   SYS_SRST_1_AUD_BIT));

    // reset PLL_AUDIO flow ---------------------
    // power-on
    rtdf_clearBitsb(SYS_PLL_AUDIO_1_0x2c69, SYS_PLL_AUDIO_1_PWDN_BIT);
    Audio_TimeDelay_us(100);

    // disable PLL_AUDIO output
    rtdf_setBitsb(SYS_PLL_AUDIO_1_0x2c69, (SYS_PLL_AUDIO_1_OEB1_BIT | SYS_PLL_AUDIO_1_OEB2_BIT));

    // disable phase-shift output
    rtdf_clearBitsb(SYS_PLL_AUDIO_0_0x2c68, (SYS_PLL_AUDIO_0_PSAUD_EN_BIT | SYS_PLL_AUDIO_0_PSAUD_PSEN_BIT));

    // set PLL_AUDIO int reset state
    rtdf_clearBitsb(SYS_PLL_AUDIO_1_0x2c69, SYS_PLL_AUDIO_1_RSTB_BIT);
    Audio_TimeDelay_us(100);

    // set PLL_AUDIO into normal state
    rtdf_setBitsb(SYS_PLL_AUDIO_1_0x2c69, SYS_PLL_AUDIO_1_RSTB_BIT);
    Audio_TimeDelay_us(100);

    // enable PLL_AUDIO output
    //RTD_Log(LOGGER_INFO,"\nbf enable PLL_AUDIO : 0x2c69 = %x\n",(UINT32)rtdf_inb(SYS_PLL_AUDIO_1_0x2c69));
    rtdf_clearBitsb(SYS_PLL_AUDIO_1_0x2c69, (SYS_PLL_AUDIO_1_OEB1_BIT | SYS_PLL_AUDIO_1_OEB2_BIT));
    //RTD_Log(LOGGER_INFO,"\naf enable PLL_AUDIO : 0x2c69 = %x\n",(UINT32)rtdf_inb(SYS_PLL_AUDIO_1_0x2c69));

    // enable phase-shift output
    rtdf_setBitsb(SYS_PLL_AUDIO_0_0x2c68, (SYS_PLL_AUDIO_0_PSAUD_EN_BIT | SYS_PLL_AUDIO_0_PSAUD_PSEN_BIT));
    Audio_TimeDelay_us(100);

    // reset PLL_512FS flow ---------------------
    // power-down and disable output
    rtdf_setBitsb(SYS_PLL_512FS_0_0x2c6c, (SYS_PLL_512FS_0_OEB_BIT | SYS_PLL_512FS_0_PWDN_BIT));
    // load M/N code (default to 48KHz)
    // skip...., move to change source

    // power-on
    rtdf_clearBitsb(SYS_PLL_512FS_0_0x2c6c, SYS_PLL_512FS_0_PWDN_BIT);
    Audio_TimeDelay_us(100);

    // set PLL_512FS into reset state
    rtdf_clearBitsb(SYS_PLL_512FS_1_0x2c6d, SYS_PLL_512FS_1_RSTB_BIT);
    Audio_TimeDelay_us(100);

    // set PLL_512FS into normal state
    rtdf_setBitsb(SYS_PLL_512FS_1_0x2c6d, SYS_PLL_512FS_1_RSTB_BIT);
    Audio_TimeDelay_us(100);

    // enable PLL_512FS output
    rtdf_setBitsb(SYS_PLL_512FS_0_0x2c6c, SYS_PLL_512FS_0_OEB_BIT);
    Audio_TimeDelay_us(100);

    // power-on audio sram ----------------------
    GLOBAL_CTRL_REG.regValue = rtd_inl(GLOBAL_CTRL_0x3004);
    GLOBAL_CTRL_REG.bitField.adc_sram_on = _ENABLE;
    GLOBAL_CTRL_REG.bitField.dac_sram_on = _ENABLE;
    GLOBAL_CTRL_REG.bitField.am_sram_on  = _ENABLE;
    GLOBAL_CTRL_REG.bitField.a2_eiaj_sram_on = _ENABLE;
    GLOBAL_CTRL_REG.bitField.audio_rom_on    = _ENABLE;
    GLOBAL_CTRL_REG.bitField.sif_adc_clk_en  = _ENABLE;
    rtd_outl(GLOBAL_CTRL_0x3004, GLOBAL_CTRL_REG.regValue);
}

/*=============================================================*/
/**
 * InitInterrupt
 * setup system interrupt for Audio IP
 * 0. before call this function, make sure that audio ISR has already attach to OS
 * 1. setting route register
 * 2. enable interrupt enable register
 *
 * @param <void>	{ void }
 * @return 			{ void }
 * @ingroup drv_adc
 */
static void InitInterrupt()
{
//	interrupt_en_RBUS		AIO_INT_EN_REG;
//	interrupt_status_RBUS	AIO_INT_ST_REG;

    // disable global audio interrupt
    drv_isr_Enable(SYS_ISR_AUDIO, _DISABLE);

    // route select to 1 for audio interrupts
    drv_isr_SelectRoute(ROUTE_SEL_AUDIO, IRQ_ROUTE_IRQ1);

    // disable all audio interrupts
    rtd_outl(INTERRUPT_EN_0x3050, 0);	// others
    rtd_outl(SYSTEM_INT_EN_0x37b4, 0);	// EQ/Loud/Dvol

    // clear all audio interrupts' pending status(write one to clear)
    rtd_outl(INTERRUPT_STATUS_0x3054, 0x0FFFFFFF);
    rtd_outl(SYSTEM_INT_EN_0x37b4, 0x00000007);

    // enable global audio interrupt
    drv_isr_Enable(SYS_ISR_AUDIO, _ENABLE);
}

#if 0	// not used
static void Timerevent_Check_SPDIF_LockStatus()
{
    if (pb_path_info.spdifi_lock_flag == TRUE)
    {
        Audio_HwpSetMute(_DISABLE, APP_CH_ID_ALL, AUD_DRV_MUTE);
    }
}
#endif

/*
 * return ( 0) : succeed in setting M/N code
 * return (-1) : unsupport fs or error happened
 */
static INT8 SetPLL_I2S(UINT8 fs)
{
    i2s_in_primary_RBUS		I2SI_CTRL1_REG;
    AIO_I2S_PLL_TAB			*p_tab;

    I2SI_CTRL1_REG.regValue = rtd_inl(I2S_IN_PRIMARY_0x3100);
    if (I2SI_CTRL1_REG.bitField.pcm_width == 32)
    {
        p_tab = i2s_pll_table64 + fs;
    }
    else if (I2SI_CTRL1_REG.bitField.pcm_width == 24)
    {
        p_tab = i2s_pll_table48 + fs;
    }
    else
    {
        return (-1);	// unreasonable register value
    }

#if 1 // Sparrow
    rtd_maskb(
        SYS_PLL_512FS_3_0x2c6f,
        ~(SYS_PLL_512FS_3_BPN_BIT|SYS_PLL_512FS_3_BPS_BIT|SYS_PLL_512FS_3_N_MASK),
        ( ((p_tab->bypass_N)<<7) | ((p_tab->bypass_P)<<6) | p_tab->Ncode )
    );

    rtd_outb(SYS_PLL_512FS_2_0x2c6e,p_tab->Mcode);

    rtd_maskb(
        SYS_PLL_512FS_1_0x2c6d,
        ~(SYS_PLL_512FS_1_P_MASK | SYS_PLL_512FS_1_RSTB_BIT),
        ( ((p_tab->Pcode)<<4) | SYS_PLL_512FS_1_RSTB_BIT )
    );

    rtd_maskb(SYS_PLL_512FS_0_0x2c6c, ~(SYS_PLL_512FS_0_OEB_BIT|SYS_PLL_512FS_0_PWDN_BIT), 0);	// power-on, output-enable
#else  // LV PLL setting
    PLL_512FS1_REG.regValue = audio_drv_rd_reg(SYSTEM_PLL512_FS1_VADDR);
    PLL_512FS1_REG.pll512fs_bpn = p_tab->bypass_N;	// PLL_512FS_3
    PLL_512FS1_REG.pll512fs_bps = p_tab->bypass_P;	// PLL_512FS_3
    PLL_512FS1_REG.pll512fs_m = p_tab->Mcode;		// PLL_512FS_2
    PLL_512FS1_REG.pll512fs_n = p_tab->Ncode;		// PLL_512FS_3
    PLL_512FS1_REG.pll512fs_p = p_tab->Pcode;		// PLL_512FS_1
    PLL_512FS1_REG.pll512fs_oeb = 0;				// PLL_512FS_0
    PLL_512FS1_REG.pll512fs_rstb = 1;				// PLL_512FS_1
    PLL_512FS1_REG.pll512fs_pwdn = 0;				// PLL_512FS_0
    audio_drv_wr_reg(SYSTEM_PLL512_FS1_VADDR, PLL_512FS1_REG.regValue);
#endif
    return 0;
}

/*
 * return (ret > 0) : succeed in setting M/N code
 * return (ret < 0) : unsupport fs or error happened
 */
static INT8 SetPLL_SPDIF(UINT8 fs)
{
#if 1 // Sparrow
    pllcg_spdif_gain_cr0_RBUS	SPDIF_CR0_REG;
    INT8	i2s_fs_id;
    INT8	ret = 0;

    SPDIF_CR0_REG.regValue = rtd_inl(PLLCG_SPDIF_GAIN_CR0_0x3008);

    switch (fs)
    {
    case AIO_FS_ID_88K:
    case AIO_FS_ID_176K:
        if (fs == AIO_FS_ID_88K) i2s_fs_id = 0;
        else					i2s_fs_id = 1;
        rtd_maskb(
            SYS_PLL_512FS_3_0x2c6f,
            ~(SYS_PLL_512FS_3_BPN_BIT|SYS_PLL_512FS_3_BPS_BIT|SYS_PLL_512FS_3_N_MASK),
            ( ((i2s_pll_table64_for_spdif[i2s_fs_id].bypass_N)<<7) |
              ((i2s_pll_table64_for_spdif[i2s_fs_id].bypass_P)<<6) |
              ((i2s_pll_table64_for_spdif[i2s_fs_id].Ncode)<<0) )
        );

        rtd_outb(SYS_PLL_512FS_2_0x2c6e,i2s_pll_table64_for_spdif[i2s_fs_id].Mcode);

        rtd_maskb(
            SYS_PLL_512FS_1_0x2c6d,
            ~(SYS_PLL_512FS_1_P_MASK | SYS_PLL_512FS_1_RSTB_BIT),
            ( ((i2s_pll_table64_for_spdif[i2s_fs_id].Pcode)<<4) | SYS_PLL_512FS_1_RSTB_BIT )
        );
        rtd_maskb(SYS_PLL_512FS_0_0x2c6c, ~(SYS_PLL_512FS_0_OEB_BIT|SYS_PLL_512FS_0_PWDN_BIT), 0);	// power-on, output-enable

        ret = 1;	// special mode, need modify global ctrl reg. after return.
        SPDIF_CR0_REG.bitField.div_2fs = spdif_pll_table[fs].div_2fs;
        SPDIF_CR0_REG.bitField.reg_para_a = spdif_pll_table[fs].Acode;
        SPDIF_CR0_REG.bitField.reg_para_d = spdif_pll_table[fs].Dcode;
        SPDIF_CR0_REG.bitField.reg_para_e = spdif_pll_table[fs].Ecode;
        rtd_outl(PLLCG_SPDIF_GAIN_CR0_0x3008, SPDIF_CR0_REG.regValue);

        rtd_maskb(
            SYS_PLL_AUDIO_0_0x2c68,
            ~(SYS_PLL_AUDIO_0_PSAUD_DIV_MASK | SYS_PLL_AUDIO_0_PSAUD_EN_BIT | SYS_PLL_AUDIO_0_PSAUD_PSEN_BIT),
            ( ((spdif_pll_table[fs].Ccode)<<4) | SYS_PLL_AUDIO_0_PSAUD_EN_BIT | SYS_PLL_AUDIO_0_PSAUD_PSEN_BIT )
        );
        break;
    case AIO_FS_ID_16K:
    case AIO_FS_ID_22K:
    case AIO_FS_ID_24K:
    case AIO_FS_ID_32K:
    case AIO_FS_ID_44K:
    case AIO_FS_ID_48K:
    case AIO_FS_ID_96K:
    case AIO_FS_ID_192K:
        ret = 0;	// normal mode
        SPDIF_CR0_REG.bitField.div_2fs = spdif_pll_table[fs].div_2fs;
        SPDIF_CR0_REG.bitField.reg_para_a = spdif_pll_table[fs].Acode;
        SPDIF_CR0_REG.bitField.reg_para_d = spdif_pll_table[fs].Dcode;
        SPDIF_CR0_REG.bitField.reg_para_e = spdif_pll_table[fs].Ecode;
        rtd_outl(PLLCG_SPDIF_GAIN_CR0_0x3008, SPDIF_CR0_REG.regValue);

        rtd_maskb(
            SYS_PLL_AUDIO_0_0x2c68,
            ~(SYS_PLL_AUDIO_0_PSAUD_DIV_MASK | SYS_PLL_AUDIO_0_PSAUD_EN_BIT | SYS_PLL_AUDIO_0_PSAUD_PSEN_BIT),
            ( ((spdif_pll_table[fs].Ccode)<<4) | SYS_PLL_AUDIO_0_PSAUD_EN_BIT | SYS_PLL_AUDIO_0_PSAUD_PSEN_BIT )
        );
        break;

        /*
         * LasVegas do not support the sample-rate listed below.
         * Therefore,force them to 48kHz setting.
         */
    case AIO_FS_ID_8K:
    case AIO_FS_ID_11K:
    case AIO_FS_ID_12K:
    case AIO_FS_ID_64K:
    case AIO_FS_ID_128K:
    default:
        ret = -1;	// unsupport fs
        SPDIF_CR0_REG.bitField.div_2fs = spdif_pll_table[AIO_FS_ID_48K].div_2fs;
        SPDIF_CR0_REG.bitField.reg_para_a = spdif_pll_table[AIO_FS_ID_48K].Acode;
        SPDIF_CR0_REG.bitField.reg_para_d = spdif_pll_table[AIO_FS_ID_48K].Dcode;
        SPDIF_CR0_REG.bitField.reg_para_e = spdif_pll_table[AIO_FS_ID_48K].Ecode;
        rtd_outl(PLLCG_SPDIF_GAIN_CR0_0x3008, SPDIF_CR0_REG.regValue);

        rtd_maskb(
            SYS_PLL_AUDIO_0_0x2c68,
            ~(SYS_PLL_AUDIO_0_PSAUD_DIV_MASK | SYS_PLL_AUDIO_0_PSAUD_EN_BIT | SYS_PLL_AUDIO_0_PSAUD_PSEN_BIT),
            ( ((spdif_pll_table[AIO_FS_ID_48K].Ccode)<<4) | SYS_PLL_AUDIO_0_PSAUD_EN_BIT | SYS_PLL_AUDIO_0_PSAUD_PSEN_BIT )
        );

        break;
    }

    return ret;

#else // LV PLL setting
    pllcg_spdif_gain_cr0_RBUS	SPDIF_CR0_REG;
    pllaud_ps_RBUS				SYS_AUDIO_PLL_REG;
    pll512_fs1_RBUS				PLL_512FS1_REG;
    INT8	i2s_fs_id;
    INT8	ret = 0;

    SPDIF_CR0_REG.regValue = audio_drv_rd_reg(AUDIO_IO_PLLCG_SPDIF_GAIN_CR0_VADDR);
    SYS_AUDIO_PLL_REG.regValue = audio_drv_rd_reg(SYSTEM_PLLAUD_PS_VADDR);

    SYS_AUDIO_PLL_REG.psaud_en = TRUE;
    SYS_AUDIO_PLL_REG.psaud_psen = TRUE;

    switch (fs)
    {
    case AIO_FS_ID_88K:
    case AIO_FS_ID_176K:
        if (fs == AIO_FS_ID_88K) i2s_fs_id = 0;
        else					i2s_fs_id = 1;
        PLL_512FS1_REG.regValue = audio_drv_rd_reg(SYSTEM_PLL512_FS1_VADDR);
        PLL_512FS1_REG.pll512fs_bpn = i2s_pll_table64_for_spdif[i2s_fs_id].bypass_N;
        PLL_512FS1_REG.pll512fs_bps = i2s_pll_table64_for_spdif[i2s_fs_id].bypass_P;
        PLL_512FS1_REG.pll512fs_m = i2s_pll_table64_for_spdif[i2s_fs_id].Mcode;
        PLL_512FS1_REG.pll512fs_n = i2s_pll_table64_for_spdif[i2s_fs_id].Ncode;
        PLL_512FS1_REG.pll512fs_p = i2s_pll_table64_for_spdif[i2s_fs_id].Pcode;
        PLL_512FS1_REG.pll512fs_oeb = 0;
        PLL_512FS1_REG.pll512fs_rstb = 1;
        PLL_512FS1_REG.pll512fs_pwdn = 0;
        audio_drv_wr_reg(SYSTEM_PLL512_FS1_VADDR, PLL_512FS1_REG.regValue);

        ret = 1;	// special mode, need modify global ctrl reg. after return.
        SPDIF_CR0_REG.div_2fs = spdif_pll_table[fs].div_2fs;
        SPDIF_CR0_REG.reg_para_a = spdif_pll_table[fs].Acode;
        SPDIF_CR0_REG.reg_para_d = spdif_pll_table[fs].Dcode;
        SPDIF_CR0_REG.reg_para_e = spdif_pll_table[fs].Ecode;
        SYS_AUDIO_PLL_REG.psaud_div = spdif_pll_table[fs].Ccode;
        break;
    case AIO_FS_ID_16K:
    case AIO_FS_ID_22K:
    case AIO_FS_ID_24K:
    case AIO_FS_ID_32K:
    case AIO_FS_ID_44K:
    case AIO_FS_ID_48K:
    case AIO_FS_ID_96K:
    case AIO_FS_ID_192K:
        ret = 0;	// normal mode
        SPDIF_CR0_REG.div_2fs = spdif_pll_table[fs].div_2fs;
        SPDIF_CR0_REG.reg_para_a = spdif_pll_table[fs].Acode;
        SPDIF_CR0_REG.reg_para_d = spdif_pll_table[fs].Dcode;
        SPDIF_CR0_REG.reg_para_e = spdif_pll_table[fs].Ecode;
        SYS_AUDIO_PLL_REG.psaud_div = spdif_pll_table[fs].Ccode;
        break;

        /*
         * LasVegas do not support the sample-rate listed below.
         * Therefore,force them to 48kHz setting.
         */
    case AIO_FS_ID_8K:
    case AIO_FS_ID_11K:
    case AIO_FS_ID_12K:
    case AIO_FS_ID_64K:
    case AIO_FS_ID_128K:
    default:
        ret = -1;	// unsupport fs
        SPDIF_CR0_REG.reg_para_a = spdif_pll_table[AIO_FS_ID_48K].Acode;
        SPDIF_CR0_REG.reg_para_d = spdif_pll_table[AIO_FS_ID_48K].Dcode;
        SPDIF_CR0_REG.reg_para_e = spdif_pll_table[AIO_FS_ID_48K].Ecode;
        SYS_AUDIO_PLL_REG.psaud_div = spdif_pll_table[AIO_FS_ID_48K].Ccode;
        break;
    }

    audio_drv_wr_reg(AUDIO_IO_PLLCG_SPDIF_GAIN_CR0_VADDR, SPDIF_CR0_REG.regValue);
    audio_drv_wr_reg(SYSTEM_PLLAUD_PS_VADDR, SYS_AUDIO_PLL_REG.regValue);
    return ret;
#endif
}

static void SetSPDIF_DigitalTracking(BOOL para)
{
    pllcg_spdif_gain_cr0_RBUS	SPDIF_CR0_REG;

#if 0 // small code size
    rtd_maskl(PLLCG_SPDIF_GAIN_CR0_0x3008, _BIT15, (para<<_BIT15));
#else
    SPDIF_CR0_REG.regValue = rtd_inl(PLLCG_SPDIF_GAIN_CR0_0x3008);
    SPDIF_CR0_REG.bitField.pll_spdif_dtrack_en = para;
    rtd_outl(PLLCG_SPDIF_GAIN_CR0_0x3008, SPDIF_CR0_REG.regValue);
#endif
}

/**
 * Set data source for Playback path
 */
static void SetPbDataSrcSel(UINT8 *src)
{
    down_sample_fifo_source_sel_RBUS	FIFO_SEL_REG;

    FIFO_SEL_REG.regValue = rtd_inl(DOWN_SAMPLE_FIFO_SOURCE_SEL_0x3554);
//	printk("ch12 src : 0x%08x\n",*src);
    FIFO_SEL_REG.bitField.fifo_ch12_sel = *src++;
//	printk("ch34 src : 0x%08x\n",*src);
    FIFO_SEL_REG.bitField.fifo_ch34_sel = *src++;
//	printk("ch56 src : 0x%08x\n",*src);
    FIFO_SEL_REG.bitField.fifo_ch56_sel = *src++;
//	printk("ch78 src : 0x%08x\n",*src);
    FIFO_SEL_REG.bitField.fifo_ch78_sel = *src;
    rtd_outl(DOWN_SAMPLE_FIFO_SOURCE_SEL_0x3554, FIFO_SEL_REG.regValue);
}

/**
 * Set data source for TimeShift path
 */
static void SetTsDataSrcSel(UINT8 src)
{
    dvol_control_RBUS	TS_PATH_REG;

    TS_PATH_REG.regValue = rtd_inl(DVOL_CONTROL_0x3814);
    switch (src)
    {
    case AIO_PATH_SRC_ATV:
    case AIO_PATH_SRC_ATV_NICAM:
    case AIO_PATH_SRC_ATV_HDEV:
        TS_PATH_REG.bitField.vol_source_sel = 0;
        break;
    case AIO_PATH_SRC_BBADC:
        TS_PATH_REG.bitField.vol_source_sel = 1;
        break;
    case AIO_PATH_SRC_SPDIF:
        TS_PATH_REG.bitField.vol_source_sel = 4;
        break;
    case AIO_PATH_SRC_PCM:
        TS_PATH_REG.bitField.vol_source_sel = 7;
        break;
    case AIO_PATH_SRC_I2S_CH12:
    case AIO_PATH_SRC_I2S_CH34:
    case AIO_PATH_SRC_I2S_CH56:
    case AIO_PATH_SRC_I2S_CH78:
        TS_PATH_REG.bitField.vol_source_sel = 2;
        break;
    default:
        return;
    }

    rtd_outl(DVOL_CONTROL_0x3814, TS_PATH_REG.regValue);
}


static void SetPeakSrcSel(AIO_PEAK_SRC src)
{
    peak_contorl_RBUS	PEAK_CTRL_REG;

#if 0 // small code size
    rtd_maskl(PEAK_CONTORL_0x31a0, (_BIT1|_BIT2|_BIT3), (src<<_BIT1));
#else
    PEAK_CTRL_REG.regValue = rtd_inl(PEAK_CONTORL_0x31a0);
    PEAK_CTRL_REG.bitField.pdetect_enable = TRUE;
    PEAK_CTRL_REG.bitField.detect_source_sel = src;
    rtd_outl(PEAK_CONTORL_0x31a0, PEAK_CTRL_REG.regValue);
#endif
}

void drv_audio_PowerOn(bit on_off)
{
    if (on_off)  	// power on
    {
        rtd_outl(ANALOG_POWER_0x3918, 0x0007ffff);
        rtdf_clearBits(SYS_PLL_AUDIO_1_0x2c69, _BIT1);
        rtdf_clearBits(SYS_PLL_512FS_0_0x2c6c, _BIT1);
    }
    else
    {
        rtd_outl(ANALOG_POWER_0x3918, 0);
        rtdf_setBits(SYS_PLL_AUDIO_1_0x2c69, _BIT1);
        rtdf_setBits(SYS_PLL_512FS_0_0x2c6c, _BIT1);
    }
}

#endif // CONFIG_ENABLE_AUDIO

