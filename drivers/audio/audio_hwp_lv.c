/*=============================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2007 *
 * All rights reserved.                                       *
 *============================================================*/
/*======================= Description ========================*/
/**
 * @file  audio_hwp.c
 *
 * @author
 * @date
 * @version 0.1
 * @ingroup drv_adc
 *
 */
/*====================== Include files ========================*/
#include "sparrow.h"
#include "ROSPrintf.h"
#include "audio_reg.h"
#include "audio_host.h"
#include "audio_tv.h"
#include "audio_if.h"
//#include "audio_hwp.h"
#include "audio_hwp_lv.h"
#include "audio/audio_api.h"

/*======================= Type Defines ========================*/
#define MAX_EQ_VALUE			(APP_EQ_COEF_NUM-1)
#define ZERO_EQ_VALUE			(MAX_EQ_VALUE/2)
#define EQ_ADJUST_STEP_SIZE		(20)	// columbus use 200 ?!, but dragon use 20
#define MAX_LOUD_VALUE			(APP_LOUD_COEF_NUM-1)
#define ZERO_LOUD_VALUE			(MAX_LOUD_VALUE/2)
#define LOUD_ADJUST_STEP_SIZE	(10)	// too large gap will cause pop noise during changing parameters.
#define INPUT_NOISE_VALUE		0x120

#define APP_CH_NUM	6
#define CH_L		0
#define CH_R		1
#define CH_LS		2
#define CH_RS		3
#define CH_C		4
#define CH_SW		5

/*===================== Global Variable =======================*/
static INT16	m_volume_comp[APP_DVOL_ID_NUM][APP_CH_NUM];
static INT16	m_balance_lr = 0;
static INT16	m_balance_lsrs = 0;
static INT16	m_balance_csw = 0;
static UINT8	m_mute[APP_MUTE_NUM];
static INT16	m_volume_comp_ts[APP_DVOL_ID_NUM][2];
static INT16	m_balance_ts_lr = 0;
static UINT8	m_mute_ts[APP_MUTE_NUM];
static UINT8	m_mute_cnt = 0;
static BOOL	m_nr_mute_flag = _FALSE;

#if 0
static UINT8 code audio_sample_per_ms[16] =
{
    8,  11,  12,
    16,  22,  24,
    32,  44,  48,
    64,  88,  96,
    128, 176, 192,
    0
};
#endif

/*
 * default table is in driver/audio/audio_table_default.c
 */
extern APP_DVOL_CFG code m_cfg_dvol;
extern APP_DVOL_CFG code m_cfg_dvol_ts;
extern APP_MUTE_CFG code m_cfg_mute;
extern APP_MUTE_CFG code m_cfg_mute_ts;
extern APP_AVC_CFG code m_cfg_avc;
extern APP_AVC_CFG code m_cfg_avc_off;
extern APP_BEEP_CFG	code m_cfg_beeper;
extern APP_EQ_COEF code m_coef_eq[APP_EQ_SR_NUM][APP_EQ_BAND_NUM][APP_EQ_COEF_NUM];
extern APP_EQ_COEF code m_coef_spk[APP_EQ_SR_NUM][APP_EQ_BAND_NUM];
extern APP_EQ_COEF code m_coef_treble[APP_EQ_SR_NUM][APP_EQ_COEF_NUM];
extern APP_EQ_COEF code m_coef_bass[APP_EQ_SR_NUM][APP_EQ_COEF_NUM];

static APP_EQ_COEF	m_curr_coef_treble;
static APP_EQ_COEF	m_curr_coef_bass;
static APP_EQ_COEF	m_curr_coef_eq[APP_EQ_BAND_NUM];


static void HwpSetVolume(UINT8 ch_id, INT16 vol);
static void HwpSetVolumeTS(UINT8 ch_id, INT16 vol);
static void HwpSetMute(BOOL para, UINT8 ch_id);
static void HwpSetMuteTS(BOOL para, UINT8 ch_id);

static INT8 HwpSetVal_Treble(APP_TREBLE_VAL *p_val_treble);
static INT8 HwpSetVal_Bass(APP_BASS_VAL *p_val_bass);
static INT8 HwpSetVal_EQ(APP_EQ_VAL *p_val_eq);

static INT8 HwpSetCoef_Loudness(UINT8 band_id, UINT8 smooth);
static INT8 HwpSetCoef_EQ(UINT8 band_id, UINT8 smooth);
static INT8 HwpSetReg(UINT32 address, INT32 value, BOOL shift);	// only for Equalizer and Loudness

/*====================== API Routines =========================*/
/*==================== Driver Routines ========================*/
/*=============================================================*/
/**
 * Init. H/W post-processing module
 * init global HWP setting and enable HWP block and interrupt
 *
 * @param <void>	{ void }
 * @return 			{ void }
 * @ingroup drv_adc
 */
void Audio_AppInit()
{
#ifdef CONFIG_ENABLE_AUDIO
    hw_process_control_RBUS		HWP_CTRL_REG;
    UINT8						i,j;

    // disable all hw process
    HWP_CTRL_REG.regValue = rtd_inl(HW_PROCESS_CONTROL_0x3700);
    HWP_CTRL_REG.bitField.basic_premium_sel  = 0;	// always set to HWP_PATH
    HWP_CTRL_REG.bitField.auto_volenable     = FALSE;
    HWP_CTRL_REG.bitField.spatial_ctrlenable = FALSE;
    HWP_CTRL_REG.bitField.deq_enable         = FALSE;
    HWP_CTRL_REG.bitField.loud_enable        = FALSE;
    HWP_CTRL_REG.bitField.dvol_enable        = FALSE;
    rtd_outl(HW_PROCESS_CONTROL_0x3700, HWP_CTRL_REG.regValue);

    // disable hwp-related interrupts and clear pending status
    rtd_outl(SYSTEM_INT_EN_0x37b4,0x7);

    // init Digital Volume
    Audio_HwpCfgVolume(&m_cfg_dvol);
    Audio_HwpCfgVolumeTS(&m_cfg_dvol_ts);
    Audio_HwpCfgMute(&m_cfg_mute);
    Audio_HwpCfgMuteTS(&m_cfg_mute_ts);

    m_balance_lr    = 0;		// center point
    m_balance_lsrs  = 0;		// center point
    m_balance_csw   = 0;		// center point
    m_balance_ts_lr = 0;		// center point


    for (i=0; i<APP_CH_NUM; i++)
    {
        for (j=0; j<APP_DVOL_ID_NUM; j++)
        {
            m_volume_comp[j][i] = 0;
        }
    }

    for (i=0; i<2; i++)
    {
        for (j=0; j<APP_DVOL_ID_NUM; j++)
        {
            m_volume_comp_ts[j][i] = 0;
        }
    }

    for (i=0; i<APP_MUTE_NUM; i++)
    {
        m_mute[i] = m_mute_ts[i] = 0;	// unmute
    }

    Audio_HwpSetVolume(APP_CH_ID_ALL, APP_MIN_VOL);
    Audio_HwpSetVolumeTS(APP_CH_ID_ALL, 0/*APP_MIN_VOL*/);

    // init Beeper

    // init Spatial Effect

    // init Auto Volume Control

    // init Equalizer & Loundness
    Audio_HwpSetEQCoef((APP_EQ_CFG*)&m_coef_eq);
    Audio_HwpSetBassCoef((APP_BASS_CFG*)&m_coef_bass);
    Audio_HwpSetTrebleCoef((APP_TREBLE_CFG*)&m_coef_treble);

    // enable Digital Volume , Equalizer & Loudness
    HWP_CTRL_REG.regValue = rtd_inl(HW_PROCESS_CONTROL_0x3700);
    HWP_CTRL_REG.bitField.dvol_enable = TRUE;
    HWP_CTRL_REG.bitField.deq_enable  = TRUE;
    HWP_CTRL_REG.bitField.loud_enable = TRUE;
    rtd_outl(HW_PROCESS_CONTROL_0x3700, HWP_CTRL_REG.regValue);

    // TODO: is this function still available in Sparrow ?
    /* from dragon
    drvif_Hdmi_AudioGainContol(1,0x80); //alway enable -6gain for hdmi
    */

#endif // CONFIG_ENABLE_AUDIO
}


/**
 * Set L/R channel balance setting
 * if (balance > 0) means that L channel will be attenuated.
 * if (balance < 0) means that R channel will be attenuated.
 * attenuation level is 0.125 dB per step.
 *
 * @param <balance>	{ range from APP_MIN_BAL ~ APP_MAX_BAL }
 * @return			{ void }
 * @ingroup lib_audio
 */
void Audio_HwpSetBalanceLR(INT16  balance, BOOL auto_mute)
{
#ifndef CONFIG_ENABLE_AUDIO
    balance = balance;
    auto_mute = auto_mute;
#else
    UINT8	i;
    INT16 	comp_vol;

    if (balance >= APP_MAX_BAL)
    {
        balance = APP_MAX_BAL;
        if (auto_mute == TRUE)
        {
            Audio_HwpSetMute(_ENABLE, APP_CH_ID_L, AUD_BALANCE_MUTE);
        }
    }
    else if (balance <= APP_MIN_BAL)
    {
        balance = APP_MIN_BAL;
        if (auto_mute == TRUE)
        {
            Audio_HwpSetMute(_ENABLE, APP_CH_ID_R, AUD_BALANCE_MUTE);
        }
    }
    else
    {
        Audio_HwpSetMute(_DISABLE, APP_CH_ID_L | APP_CH_ID_R, AUD_BALANCE_MUTE);
    }

    //RTD_Log(LOGGER_INFO,"[AUDIO][APP] set balance L/R : %d\n",(INT32)balance);

    m_balance_lr = balance;

    if (m_balance_lr > 0)  	// attenuate L channel
    {
        for (i=0,comp_vol=0; i<APP_DVOL_ID_NUM; i++)
        {
            comp_vol += m_volume_comp[i][CH_L];
        }

        HwpSetVolume(APP_CH_ID_L, comp_vol - m_balance_lr);
    }
    else  					// attenuate R channel
    {
        for (i=0,comp_vol=0; i<APP_DVOL_ID_NUM; i++)
        {
            comp_vol += m_volume_comp[i][CH_R];
        }

        HwpSetVolume(APP_CH_ID_R, comp_vol + m_balance_lr);
    }

    //RTD_Log(LOGGER_INFO,"[AUDIO][APP] set balance L/R done!\n");
#endif // !CONFIG_ENABLE_AUDIO
}

#if 0	// not used
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
void Audio_HwpSetBalanceTS(INT16  balance, BOOL auto_mute)
{
#ifndef CONFIG_ENABLE_AUDIO
    balance = balance;
    auto_mute = auto_mute;
#else
    UINT8	i;
    INT16 	comp_vol;

    if (balance >= APP_MAX_BAL)
    {
        balance = APP_MAX_BAL;
        if (auto_mute == TRUE)
        {
            Audio_HwpSetMuteTS(_ENABLE, APP_CH_ID_L, AUD_BALANCE_MUTE);
        }
    }
    else if (balance <= APP_MIN_BAL)
    {
        balance = APP_MIN_BAL;
        if (auto_mute == TRUE)
        {
            Audio_HwpSetMuteTS(_ENABLE, APP_CH_ID_R, AUD_BALANCE_MUTE);
        }
    }
    else
    {
        Audio_HwpSetMuteTS(_DISABLE, APP_CH_ID_L | APP_CH_ID_R, AUD_BALANCE_MUTE);
    }

    RTD_Log(LOGGER_INFO,"[AUDIO][APP] set ts_balance L/R : %d\n",(INT32)balance);

    m_balance_ts_lr = balance;

    if (m_balance_ts_lr > 0)  	// attenuate L channel
    {
        for (i=0,comp_vol=0; i<APP_DVOL_ID_NUM; i++)
        {
            comp_vol += m_volume_comp_ts[i][CH_L];
        }

        HwpSetVolumeTS(APP_CH_ID_L, comp_vol - m_balance_ts_lr);
    }
    else  					// attenuate R channel
    {
        for (i=0,comp_vol=0; i<APP_DVOL_ID_NUM; i++)
        {
            comp_vol += m_volume_comp_ts[i][CH_R];
        }

        HwpSetVolumeTS(APP_CH_ID_R, comp_vol + m_balance_ts_lr);
    }

#endif // !CONFIG_ENABLE_AUDIO
}
#endif

/**
 * Set Digital Volume
 *
 * @param <ch_id>	{ APP_CH_ID_L | APP_CH_ID_R | ... ,refer to audio_app.h }
 * @param <vol>		{ range from APP_MIN_VOL ~ APP_MAX_VOL }
 * @return			{ void }
 * @ingroup lib_audio
 */
void Audio_HwpSetVolume(UINT8 ch_id, INT16 vol)
{
#ifndef CONFIG_ENABLE_AUDIO
    ch_id = ch_id;
    vol = vol;
#else
	RTD_Log(LOGGER_INFO, "[AUDIO][APP] set volume ch=%d,vol=%d -----------------------\r\n",ch_id,vol);
    Audio_EnterCriticalSection();
    Audio_HwpSetVolumeCompensation(ch_id, vol, APP_DVOL_ID_USR);
    Audio_LeaveCriticalSection();

#endif // !CONFIG_ENABLE_AUDIO
}

void Audio_HwpInputNoiseReduction(void)
{
    UINT16 StatusL,StatusR;
    StatusL = rtdf_readWord0(PEAK_STATUS_L_0x31b0);
    StatusR = rtdf_readWord0(PEAK_STATUS_R_0x31bc);
    if (StatusL <= INPUT_NOISE_VALUE && StatusR <= INPUT_NOISE_VALUE)
    {
        m_mute_cnt ++;
        if (m_mute_cnt > 5)
        {
            m_nr_mute_flag = _TRUE;
            Audio_HwpSetMute(_ENABLE, APP_CH_ID_ALL, AIO_MUTE_ID);
        }
    }
    else
    {
        m_mute_cnt = 0;
        if (m_nr_mute_flag == _TRUE)
        {
            m_nr_mute_flag = _FALSE;
            Audio_HwpSetMute(_DISABLE, APP_CH_ID_ALL, AIO_MUTE_ID);
        }
    }
}

/**
 * Set Digital Volume ( TimeShift(SCART) path )
 *
 * @param <ch_id>	{ APP_CH_ID_L | APP_CH_ID_R | ... ,refer to audio_app.h }
 * @param <vol>		{ range from APP_MIN_VOL ~ APP_MAX_VOL }
 * @return			{ void }
 * @ingroup lib_audio
 */
void Audio_HwpSetVolumeTS(UINT8 ch_id, INT16 vol)
{
#ifndef CONFIG_ENABLE_AUDIO
    ch_id = ch_id;
    vol = vol;
#else
//	APP_LOG("[AUDIO][APP] set volume ch=%d,vol=%d\n",ch_id,vol);
    Audio_EnterCriticalSection();
    Audio_HwpSetVolumeCompensationTS(ch_id, vol, APP_DVOL_ID_USR);
    Audio_LeaveCriticalSection();

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Set Digital Volume Compensation
 *
 * @param <ch_id>	{ APP_CH_ID_L | APP_CH_ID_R | ... ,refer to audio_app.h }
 * @param <vol>		{ range from APP_MIN_VOL ~ APP_MAX_VOL }
 * @param <comp_id>	{ caller id }
 * @return			{ void }
 * @ingroup lib_audio
 */
void Audio_HwpSetVolumeCompensation(UINT8 ch_id, INT16 vol, UINT8 comp_id)
{
#ifndef CONFIG_ENABLE_AUDIO
    ch_id = ch_id;
    vol = vol;
#else
    UINT8	i;
    INT16 	comp_vol;

    RTD_Log(LOGGER_INFO,"[AUDIO][APP] set comp_volume ch=%x,vol=%d,id=%d\n",(UINT32)ch_id,(INT32)vol,(UINT32)comp_id);

    if (comp_id >= APP_DVOL_ID_NUM) return;

    if (vol > APP_MAX_VOL)	vol = APP_MAX_VOL;
    if (vol < APP_MIN_VOL)	vol = APP_MIN_VOL;

    if (ch_id & APP_CH_ID_L)
    {
        m_volume_comp[comp_id][CH_L] = vol;

        for (i=0,comp_vol=0; i<APP_DVOL_ID_NUM; i++)
        {
            comp_vol += m_volume_comp[i][CH_L];
        }

        if (vol != APP_MIN_VOL)
        {
            if (m_balance_lr > 0)	// attenuate L channel
                HwpSetVolume(APP_CH_ID_L, comp_vol - m_balance_lr);
            else
                HwpSetVolume(APP_CH_ID_L, comp_vol);
        }
    }
    if (ch_id & APP_CH_ID_R)
    {
        m_volume_comp[comp_id][CH_R] = vol;

        for (i=0,comp_vol=0; i<APP_DVOL_ID_NUM; i++)
        {
            comp_vol += m_volume_comp[i][CH_R];
        }

        if (vol != APP_MIN_VOL)
        {
            if (m_balance_lr < 0)	// attenuate R channel
                HwpSetVolume(APP_CH_ID_R, comp_vol + m_balance_lr);
            else
                HwpSetVolume(APP_CH_ID_R, comp_vol);
        }
    }
    if (ch_id & APP_CH_ID_LS)
    {
        m_volume_comp[comp_id][CH_LS] = vol;

        for (i=0,comp_vol=0; i<APP_DVOL_ID_NUM; i++)
        {
            comp_vol += m_volume_comp[i][CH_LS];
        }

        if (vol != APP_MIN_VOL)
        {
            if (m_balance_lsrs > 0)	// attenuate Ls channel
                HwpSetVolume(APP_CH_ID_LS, comp_vol - m_balance_lsrs);
            else
                HwpSetVolume(APP_CH_ID_LS, comp_vol);
        }
    }
    if (ch_id & APP_CH_ID_RS)
    {
        m_volume_comp[comp_id][CH_RS] = vol;

        for (i=0,comp_vol=0; i<APP_DVOL_ID_NUM; i++)
        {
            comp_vol += m_volume_comp[i][CH_RS];
        }

        if (vol != APP_MIN_VOL)
        {
            if (m_balance_lsrs < 0)	// attenuate Rs channel
                HwpSetVolume(APP_CH_ID_RS, comp_vol + m_balance_lsrs);
            else
                HwpSetVolume(APP_CH_ID_RS, comp_vol);
        }
    }
    if (ch_id & APP_CH_ID_C)
    {
        m_volume_comp[comp_id][CH_C] = vol;

        for (i=0,comp_vol=0; i<APP_DVOL_ID_NUM; i++)
        {
            comp_vol += m_volume_comp[i][CH_C];
        }

        if (vol != APP_MIN_VOL)
        {
            if (m_balance_csw> 0)	// attenuate C channel
                HwpSetVolume(APP_CH_ID_C, comp_vol - m_balance_csw);
            else
                HwpSetVolume(APP_CH_ID_C, comp_vol);
        }
    }
    if (ch_id & APP_CH_ID_SW)
    {
        m_volume_comp[comp_id][CH_SW] = vol;

        for (i=0,comp_vol=0; i<APP_DVOL_ID_NUM; i++)
        {
            comp_vol += m_volume_comp[i][CH_SW];
        }

        if (vol != APP_MIN_VOL)
        {
            if (m_balance_csw < 0)	// attenuate SW channel
                HwpSetVolume(APP_CH_ID_SW, comp_vol + m_balance_csw);
            else
                HwpSetVolume(APP_CH_ID_SW, comp_vol);
        }
    }

    // if user choose the min_vol, library will mute the output
    if (vol == APP_MIN_VOL)
    {
        Audio_HwpSetMuteInternal(_ENABLE, ch_id, APP_MUTE_ID);
    }
    else if (m_mute[APP_MUTE_ID])
    {
        Audio_HwpSetMuteInternal(_DISABLE, ch_id, APP_MUTE_ID);
    }


#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Set Digital Volume Compensation ( TimeShift(SCART) path )
 *
 * @param <ch_id>	{ APP_CH_ID_L | APP_CH_ID_R | ... ,refer to audio_app.h }
 * @param <vol>		{ range from APP_MIN_VOL ~ APP_MAX_VOL }
 * @param <comp_id>	{ caller id }
 * @return			{ void }
 * @ingroup lib_audio
 */
void Audio_HwpSetVolumeCompensationTS(UINT8 ch_id, INT16 vol, UINT8 comp_id)
{
#ifndef CONFIG_ENABLE_AUDIO
    ch_id = ch_id;
    vol = vol;
#else
    UINT8	i;
    INT16 	comp_vol;

    RTD_Log(LOGGER_INFO,"[AUDIO][APP] set ts_scomp_volume ch=%x,vol=%d,id=%d\n",(UINT32)ch_id,(INT32)vol,(UINT32)comp_id);

    if (comp_id >= APP_DVOL_ID_NUM) return;

    if (vol > APP_MAX_VOL)	vol = APP_MAX_VOL;
    if (vol < APP_MIN_VOL)	vol = APP_MIN_VOL;

    if (ch_id & APP_CH_ID_L)
    {
        m_volume_comp_ts[comp_id][CH_L] = vol;

        for (i=0,comp_vol=0; i<APP_DVOL_ID_NUM; i++)
        {
            comp_vol += m_volume_comp_ts[i][CH_L];
        }

        if (vol != APP_MIN_VOL)
        {
            if (m_balance_ts_lr > 0)	// attenuate L channel
                HwpSetVolumeTS(APP_CH_ID_L, comp_vol - m_balance_ts_lr);
            else
                HwpSetVolumeTS(APP_CH_ID_L, comp_vol);
        }
    }
    if (ch_id & APP_CH_ID_R)
    {
        m_volume_comp_ts[comp_id][CH_R] = vol;

        for (i=0,comp_vol=0; i<APP_DVOL_ID_NUM; i++)
        {
            comp_vol += m_volume_comp_ts[i][CH_R];
        }

        if (vol != APP_MIN_VOL)
        {
            if (m_balance_ts_lr < 0)	// attenuate R channel
                HwpSetVolumeTS(APP_CH_ID_R, comp_vol + m_balance_ts_lr);
            else
                HwpSetVolumeTS(APP_CH_ID_R, comp_vol);
        }
    }

    // if user choose the min_vol, library will mute the output
    if (vol == APP_MIN_VOL)
    {
        Audio_HwpSetMuteInternalTS(_ENABLE, ch_id, APP_MUTE_ID);
    }
    else if (m_mute_ts[APP_MUTE_ID])
    {
        Audio_HwpSetMuteInternalTS(_DISABLE, ch_id, APP_MUTE_ID);
    }


#endif // !CONFIG_ENABLE_AUDIO
}


/**
 * Mute/Unmute output
 *
 * @param <para>	{ AIO_ENABLE/AIO_DISABLE }
 * @param <ch_id>	{ APP_CH_ID_L | APP_CH_ID_R | ... ,refer to audio_app.h }
 * @param <mute_id>	{ mute_by_who id )
 * @return			{ void }
 * @ingroup lib_audio
 */
void Audio_HwpSetMute(BOOL para, UINT8 ch_id, UINT8 mute_id)
{
#ifndef CONFIG_ENABLE_AUDIO
    para = para;
    ch_id = ch_id;
    mute_id = mute_id;
#else

    Audio_EnterCriticalSection();
    Audio_HwpSetMuteInternal(para, ch_id, mute_id);
    Audio_LeaveCriticalSection();

#endif // !CONFIG_ENABLE_AUDIO
}

#ifdef _VIDEO_TV_SUPPORT
/**
 * Mute/Unmute output ( TimeShift(SCART) path )
 *
 * @param <para>	{ AIO_ENABLE/AIO_DISABLE }
 * @param <ch_id>	{ APP_CH_ID_L | APP_CH_ID_R | ... ,refer to audio_app.h }
 * @param <mute_id>	{ mute_by_who id )
 * @return			{ void }
 * @ingroup lib_audio
 */
void Audio_HwpSetMuteTS(BOOL para, UINT8 ch_id, UINT8 mute_id)
{
#ifndef CONFIG_ENABLE_AUDIO
    para = para;
    ch_id = ch_id;
    mute_id = mute_id;
#else

    Audio_EnterCriticalSection();
    Audio_HwpSetMuteInternalTS(para, ch_id, mute_id);
    Audio_LeaveCriticalSection();

#endif // !CONFIG_ENABLE_AUDIO
}
#endif

/**
 * setup Digital Volume module
 *
 * @param <p_cfg_dvol>	{ pointer of DVOL control block }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_HwpCfgVolume(APP_DVOL_CFG *p_cfg_dvol)
{
#ifndef CONFIG_ENABLE_AUDIO
    p_cfg_dvol = p_cfg_dvol;
#else
    hw_process_control_RBUS			HWP_CTRL_REG;
    digital_volume_control_0_RBUS	DVOL_CTRL_REG;

    DVOL_CTRL_REG.regValue = rtd_inl(DIGITAL_VOLUME_CONTROL_0_0x3704);
    DVOL_CTRL_REG.bitField.zcto       = p_cfg_dvol->zero_timeout;
    DVOL_CTRL_REG.bitField.ramp_step  = p_cfg_dvol->ramp_step;
    DVOL_CTRL_REG.bitField.delay_ctrl = p_cfg_dvol->delay_time;
    rtd_outl(DIGITAL_VOLUME_CONTROL_0_0x3704, DVOL_CTRL_REG.regValue);

    HWP_CTRL_REG.regValue = rtd_inl(HW_PROCESS_CONTROL_0x3700);
    HWP_CTRL_REG.bitField.zcth = p_cfg_dvol->zero_thd;
    HWP_CTRL_REG.bitField.dvcm = p_cfg_dvol->mode;
    rtd_outl(HW_PROCESS_CONTROL_0x3700, HWP_CTRL_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * setup Digital Volume module ( TimeShift(SCART) path )
 *
 * @param <p_cfg_dvol>	{ pointer of DVOL control block }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_HwpCfgVolumeTS(APP_DVOL_CFG *p_cfg_dvol)
{
#ifndef CONFIG_ENABLE_AUDIO
    p_cfg_dvol = p_cfg_dvol;
#else
    lrrr_digital_volume_control_1_RBUS	HWP_CTRL_REG;
    lrrr_digital_volume_control_0_RBUS	DVOL_CTRL_REG;

    DVOL_CTRL_REG.regValue = rtd_inl(LRRR_DIGITAL_VOLUME_CONTROL_0_0x3800);
    DVOL_CTRL_REG.bitField.lrrr_zcto       = p_cfg_dvol->zero_timeout;
    DVOL_CTRL_REG.bitField.lrrr_ramp_step  = p_cfg_dvol->ramp_step;
    DVOL_CTRL_REG.bitField.lrrr_delay_ctrl = p_cfg_dvol->delay_time;
    rtd_outl(LRRR_DIGITAL_VOLUME_CONTROL_0_0x3800, DVOL_CTRL_REG.regValue);

    HWP_CTRL_REG.regValue = rtd_inl(LRRR_DIGITAL_VOLUME_CONTROL_1_0x3804);
    HWP_CTRL_REG.bitField.lrrr_dvol_enable = TRUE;
    HWP_CTRL_REG.bitField.lrrr_zcth = p_cfg_dvol->zero_thd;
    HWP_CTRL_REG.bitField.lrrr_dvcm = p_cfg_dvol->mode;
    rtd_outl(LRRR_DIGITAL_VOLUME_CONTROL_1_0x3804, HWP_CTRL_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}


/**
 * setup Digital Mute module
 *
 * @param <p_cfg_dvol>	{ pointer of DVOL control block }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_HwpCfgMute(APP_MUTE_CFG *p_cfg_mute)
{
#ifndef CONFIG_ENABLE_AUDIO
    p_cfg_mute = p_cfg_mute;
#else
    digital_volume_control_0_RBUS	DVOL_CTRL_REG;

    DVOL_CTRL_REG.regValue = rtd_inl(DIGITAL_VOLUME_CONTROL_0_0x3704);
    DVOL_CTRL_REG.bitField.zcto_mute       = p_cfg_mute->zero_timeout;
    DVOL_CTRL_REG.bitField.ramp_step_mute  = p_cfg_mute->ramp_step;
    DVOL_CTRL_REG.bitField.delay_ctrl_mute = p_cfg_mute->delay_time;
    rtd_outl(DIGITAL_VOLUME_CONTROL_0_0x3704, DVOL_CTRL_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * setup Digital Mute module ( TimeShift(SCART) path)
 *
 * @param <p_cfg_dvol>	{ pointer of DVOL control block }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_HwpCfgMuteTS(APP_MUTE_CFG *p_cfg_mute)
{
#ifndef CONFIG_ENABLE_AUDIO
    p_cfg_mute = p_cfg_mute;
#else
    lrrr_digital_volume_control_0_RBUS	DVOL_CTRL_REG;

    DVOL_CTRL_REG.regValue = rtd_inl(LRRR_DIGITAL_VOLUME_CONTROL_0_0x3800);
    DVOL_CTRL_REG.bitField.lrrr_zcto_mute       = p_cfg_mute->zero_timeout;
    DVOL_CTRL_REG.bitField.lrrr_ramp_step_mute  = p_cfg_mute->ramp_step;
    DVOL_CTRL_REG.bitField.lrrr_delay_ctrl_mute = p_cfg_mute->delay_time;
    rtd_outl(LRRR_DIGITAL_VOLUME_CONTROL_0_0x3800, DVOL_CTRL_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Mute/Unmute output
 * This API is for multi-thread purpose. Audio Library user should use Audio_HwpSetMute()
 * instead of this one.
 *
 * @param <para>	{ AIO_ENABLE/AIO_DISABLE }
 * @param <ch_id>	{ APP_CH_ID_L | APP_CH_ID_R | ... ,refer to audio_app.h }
 * @param <mute_id>	{ mute_by_who id )
 * @return			{ AIO_OK or AIO_ERROR_CODE }
 * @ingroup lib_audio
 */
void Audio_HwpSetMuteInternal(BOOL para, UINT8 ch_id, UINT8 mute_id)
{
#ifndef CONFIG_ENABLE_AUDIO
    para = para;
    ch_id = ch_id;
    mute_id = mute_id;
#else
    UINT32 i,mask;

    //RTD_Log(LOGGER_INFO,"Audio_HwpSetMuteInternal(%d,%x,%x)\n",(UINT32)para,(UINT32)ch_id,(UINT32)mute_id);

    if (mute_id > APP_MUTE_NUM)
    {
        RTD_Log(LOGGER_INFO,"[AUDIO][APP] Error MUTE_ID(%d)\n",mute_id);
        return;
    }

    if (para == _ENABLE)
    {
        m_mute[mute_id] |= ch_id;	// store mute record
        //RTD_Log(LOGGER_INFO,"[AUDIO][APP] Mute[%d] CH:%x\n",(UINT32)mute_id,(UINT32)ch_id);
        //mute
        HwpSetMute(_ENABLE, ch_id);
        //for(i=0; i<APP_MUTE_NUM; i++) {
        //	RTD_Log(LOGGER_INFO,"[AUDIO][APP] Mute[%d] CH:%x\n",(UINT32)i,(UINT32)m_mute[i]);
        //}
    }
    else
    {
        m_mute[mute_id] = m_mute[mute_id] & ~ch_id;	// clear mute record
        mask = 0;
        for (i=0; i<APP_MUTE_NUM; i++)
            mask |= m_mute[i];

        mask &= (UINT32)ch_id;

        mask = (UINT32)ch_id - mask;
        //RTD_Log(LOGGER_INFO,"[AUDIO][APP] Unmute[%d] CH:%x\n",(UINT32)mute_id,(UINT32)mask);
        if (mask)
        {
            HwpSetMute(_DISABLE, mask);
            //for(i=0; i<APP_MUTE_NUM; i++) {
            //	RTD_Log(LOGGER_INFO,"[AUDIO][APP] Mute[%d] CH:%x\n",(UINT32)i,(UINT32)m_mute[i]);
            //}
        }
    }

#endif // !CONFIG_ENABLE_AUDIO
}

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
void Audio_HwpSetMuteInternalTS(BOOL para, UINT8 ch_id, UINT8 mute_id)
{
#ifndef CONFIG_ENABLE_AUDIO
    para = para;
    ch_id = ch_id;
    mute_id = mute_id;
#else
    UINT32 i,mask;

    //RTD_Log(LOGGER_INFO,"Audio_HwpSetMuteInternalTS(%d,%x,%x)\n",(UINT32)para,(UINT32)ch_id,(UINT32)mute_id);

    if (mute_id > APP_MUTE_NUM)
    {
        RTD_Log(LOGGER_INFO,"[AUDIO][APP] Error MUTE_ID(%d)\n",mute_id);
        return;
    }

    if (para == _ENABLE)
    {
        m_mute_ts[mute_id] |= ch_id;	// store mute record
        //RTD_Log(LOGGER_INFO,"[AUDIO][APP] TS_Mute[%d] CH:%x\n",(UINT32)mute_id,(UINT32)ch_id);
        //mute
        HwpSetMuteTS(_ENABLE, ch_id);
        //for(i=0; i<APP_MUTE_NUM; i++) {
        //	RTD_Log(LOGGER_INFO,"[AUDIO][APP] TS_Mute[%d] CH:%x\n",(UINT32)i,(UINT32)m_mute[i]);
        //}
    }
    else
    {
        m_mute_ts[mute_id] = m_mute_ts[mute_id] & ~ch_id;	// clear mute record
        mask = 0;
        for (i=0; i<APP_MUTE_NUM; i++)
            mask |= m_mute_ts[i];

        mask &= (UINT32)ch_id;

        mask = (UINT32)ch_id - mask;
        //RTD_Log(LOGGER_INFO,"[AUDIO][APP] TS_Unmute[%d] CH:%x\n",(UINT32)mute_id,(UINT32)mask);
        if (mask)
        {
            HwpSetMuteTS(_DISABLE, mask);
            //for(i=0; i<APP_MUTE_NUM; i++) {
            //	RTD_Log(LOGGER_INFO,"[AUDIO][APP] TS_Mute[%d] CH:%x\n",(UINT32)i,(UINT32)m_mute[i]);
            //}
        }
    }

#endif // !CONFIG_ENABLE_AUDIO
}
#if 0
/**
 * Setup H/W Auto Volume Control
 *
 * @param <para>		{ AIO_ENABLE/AIO_DISABLE }
 * @param <p_cfg_avc>	{ pointer of AVC control block }
 * @return				{ AIO_OK or AIO_ERROR_CODE }
 * @ingroup lib_audio
 */
void Audio_HwpSetAVC(BOOL para, APP_AVC_CFG *p_cfg_avc)
{
#ifndef CONFIG_ENABLE_AUDIO
    para = para;
    p_cfg_avc = p_cfg_avc;
#else
    hw_process_control_RBUS		HWP_CTRL_REG;
    auto_volume_control_0_RBUS	AVC_CTRL0_REG;
    avc_volume_control_1_RBUS	AVC_CTRL1_REG;
    APP_AVC_CFG 				*t_cfg_avc;


    if (p_cfg_avc == NULL)
    {
        //apply default setting
        if (para)
            t_cfg_avc = &m_cfg_avc;
        else
            t_cfg_avc = &m_cfg_avc_off;

    }
    else
    {
        t_cfg_avc = p_cfg_avc;
    }

    if (para == _DISABLE)
    {
        // configure H/W-AVC
        AVC_CTRL0_REG.regValue = 0;
        AVC_CTRL0_REG.regValue |= ((t_cfg_avc->noise_gain)<<28);
        AVC_CTRL0_REG.regValue |= ((t_cfg_avc->noise_thd)<<20);
        AVC_CTRL0_REG.regValue |= ((t_cfg_avc->level_max)<<12);
        AVC_CTRL0_REG.regValue |= ((t_cfg_avc->level_min)<<4);
        AVC_CTRL0_REG.regValue |= ((t_cfg_avc->step_down)<<2);
        AVC_CTRL0_REG.regValue |= ((t_cfg_avc->step_up)<<0);
        rtd_outl(AUTO_VOLUME_CONTROL_0_0x3738, AVC_CTRL0_REG.regValue);

        AVC_CTRL1_REG.regValue = 0;
        AVC_CTRL1_REG.regValue |= ((t_cfg_avc->thd_cnt)<<24);
        AVC_CTRL1_REG.regValue |= ((t_cfg_avc->win_moniter)<<12);
        AVC_CTRL1_REG.regValue |= ((t_cfg_avc->win_zcross)<<0);
        rtd_outl(AVC_VOLUME_CONTROL_1_0x373c, AVC_CTRL1_REG.regValue);
    }

    /*
     * disable H/W-AVC before configure it.
     * This register is a double buffer register,we should check it after setting,
     * but this version of code, we just assume that H/W reaction is always fast enough.
     */
    HWP_CTRL_REG.regValue = rtd_inl(HW_PROCESS_CONTROL_0x3700);
    HWP_CTRL_REG.bitField.auto_volenable = _DISABLE;
    rtd_outl(HW_PROCESS_CONTROL_0x3700, HWP_CTRL_REG.regValue);

    if (para == _ENABLE)
    {
        // configure H/W-AVC
        AVC_CTRL0_REG.regValue = 0;
        AVC_CTRL0_REG.regValue |= ((t_cfg_avc->noise_gain)<<28);
        AVC_CTRL0_REG.regValue |= ((t_cfg_avc->noise_thd)<<20);
        AVC_CTRL0_REG.regValue |= ((t_cfg_avc->level_max)<<12);
        AVC_CTRL0_REG.regValue |= ((t_cfg_avc->level_min)<<4);
        AVC_CTRL0_REG.regValue |= ((t_cfg_avc->step_down)<<2);
        AVC_CTRL0_REG.regValue |= ((t_cfg_avc->step_up)<<0);
        rtd_outl(AUTO_VOLUME_CONTROL_0_0x3738, AVC_CTRL0_REG.regValue);

        AVC_CTRL1_REG.regValue = 0;
        AVC_CTRL1_REG.regValue |= ((t_cfg_avc->thd_cnt)<<24);
        AVC_CTRL1_REG.regValue |= ((t_cfg_avc->win_moniter)<<12);
        AVC_CTRL1_REG.regValue |= ((t_cfg_avc->win_zcross)<<0);
        rtd_outl(AVC_VOLUME_CONTROL_1_0x373c, AVC_CTRL1_REG.regValue);

        // enable H/W-AVC again.
        HWP_CTRL_REG.regValue = rtd_inl(HW_PROCESS_CONTROL_0x3700);
        HWP_CTRL_REG.bitField.auto_volenable = _ENABLE;
        rtd_outl(HW_PROCESS_CONTROL_0x3700, HWP_CTRL_REG.regValue);
    }

#endif // !CONFIG_ENABLE_AUDIO
}
#endif
/**
 * Setup H/W Treble values
 *
 * @param <para>			{ AIO_ENABLE/AIO_DISABLE }
 * @param <p_val_treble>	{ pointer of a set of treble value }
 * @return					{ AIO_OK or AIO_ERROR_CODE }
 * @ingroup lib_audio
 */
void Audio_HwpSetTrebleValue(BOOL para, APP_TREBLE_VAL *p_val_treble)
{
#ifndef CONFIG_ENABLE_AUDIO
    para = para;
    p_val_treble = p_val_treble;
#else
    loudness_control_register_RBUS	LOUD_CTRL_REG;
    APP_TREBLE_VAL	u_val_treble;

    if (p_val_treble == NULL)
    {
        return;
    }

    if (para == _ENABLE)
    {
        LOUD_CTRL_REG.regValue = rtd_inl(LOUDNESS_CONTROL_REGISTER_0x37ac);
        LOUD_CTRL_REG.bitField.ld_bpf1_en_lr  = TRUE;
        rtd_outl(LOUDNESS_CONTROL_REGISTER_0x37ac, LOUD_CTRL_REG.regValue);

        HwpSetVal_Treble(p_val_treble);
    }
    else
    {
        u_val_treble.max_val = 6;
        u_val_treble.min_val = -6;
        u_val_treble.val = 0;

        HwpSetVal_Treble(&u_val_treble);

        LOUD_CTRL_REG.regValue = rtd_inl(LOUDNESS_CONTROL_REGISTER_0x37ac);
        LOUD_CTRL_REG.bitField.ld_bpf1_en_lr  = FALSE;
        rtd_outl(LOUDNESS_CONTROL_REGISTER_0x37ac, LOUD_CTRL_REG.regValue);
    }

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Setup H/W Treble coefficients
 *
 * @param <p_cfg_treble>	{ pointer of Treble-Band control block }
 * @return					{ AIO_OK or AIO_ERROR_CODE }
 * @note
 *    This function is mutual with Speaker Compensation.
 *
 * @ingroup lib_audio
 */
void Audio_HwpSetTrebleCoef(APP_TREBLE_CFG *p_cfg_treble)
{
#ifdef CONFIG_ENABLE_AUDIO
//	lr_loud_a1_0_RBUS	LOUD_BP1_A1_REG;
//	lr_loud_a2_RBUS		LOUD_BP1_A2_REG;
//	lr_loud_h0_0_RBUS	LOUD_BP1_H0_REG;

    if (p_cfg_treble== NULL)
    {
        return;
    }

    RTD_Log(LOGGER_INFO,"Init Treble Coef.\n");
    //RTD_Log(LOGGER_INFO,"bf [0x3770] = %x\n",(UINT32)rtd_inl(LR_LOUD_A1_0_0x3770));
    //RTD_Log(LOGGER_INFO,"bf [0x3778] = %x\n",(UINT32)rtd_inl(LR_LOUD_A2_0x3778));
    //RTD_Log(LOGGER_INFO,"bf [0x377c] = %x\n",(UINT32)rtd_inl(LR_LOUD_H0_0_0x377c));

    // assume only 48K sample table & Treble band is LP0 of Loudness Module
    HwpSetReg(LR_LOUD_A1_0_0x3770, (INT32)p_cfg_treble->coef[0][ZERO_LOUD_VALUE].a1, FALSE);

    HwpSetReg(LR_LOUD_A2_0x3778, (INT32)p_cfg_treble->coef[0][ZERO_LOUD_VALUE].a2, TRUE);

    HwpSetReg(LR_LOUD_H0_0_0x377c, (INT32)p_cfg_treble->coef[0][ZERO_LOUD_VALUE].h0, FALSE);

    //RTD_Log(LOGGER_INFO,"af [0x3770] = %x\n",(UINT32)rtd_inl(LR_LOUD_A1_0_0x3770));
    //RTD_Log(LOGGER_INFO,"af [0x3778] = %x\n",(UINT32)rtd_inl(LR_LOUD_A2_0x3778));
    //RTD_Log(LOGGER_INFO,"af [0x377c] = %x\n",(UINT32)rtd_inl(LR_LOUD_H0_0_0x377c));
#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Setup H/W Bass values
 *
 * @param <para>		{ AIO_ENABLE/AIO_DISABLE }
 * @param <p_val_bass>	{ pointer of a set of bass value }
 * @return				{ AIO_OK or AIO_ERROR_CODE }
 * @ingroup lib_audio
 */
void Audio_HwpSetBassValue(BOOL para, APP_BASS_VAL *p_val_bass)
{
#ifndef CONFIG_ENABLE_AUDIO
    para = para;
    p_val_bass = p_val_bass;
#else
    loudness_control_register_RBUS	LOUD_CTRL_REG;
    APP_BASS_VAL	u_val_bass;

    if (p_val_bass == NULL)
    {
        return;
    }

    if (para == _ENABLE)
    {
        LOUD_CTRL_REG.regValue = rtd_inl(LOUDNESS_CONTROL_REGISTER_0x37ac);
        LOUD_CTRL_REG.bitField.ld_lpf_en_lr  = TRUE;
        rtd_outl(LOUDNESS_CONTROL_REGISTER_0x37ac, LOUD_CTRL_REG.regValue);

        HwpSetVal_Bass(p_val_bass);
    }
    else
    {
        u_val_bass.max_val = 6;
        u_val_bass.min_val = -6;
        u_val_bass.val = 0;

        HwpSetVal_Bass(&u_val_bass);

        LOUD_CTRL_REG.regValue = rtd_inl(LOUDNESS_CONTROL_REGISTER_0x37ac);
        LOUD_CTRL_REG.bitField.ld_lpf_en_lr  = FALSE;
        rtd_outl(LOUDNESS_CONTROL_REGISTER_0x37ac, LOUD_CTRL_REG.regValue);
    }

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Setup H/W Bass coefficients
 *
 * @param <p_cfg_bass>	{ pointer of Bass-Band control block }
 * @return				{ AIO_OK or AIO_ERROR_CODE }
 * @note
 *    This function is mutual with Speaker Compensation.
 *
 * @ingroup lib_audio
 */
void Audio_HwpSetBassCoef(APP_BASS_CFG *p_cfg_bass)
{
#ifdef CONFIG_ENABLE_AUDIO
//	lr_loud_a1_0_RBUS	LOUD_LP0_A1_REG;
//	lr_loud_h0_0_RBUS	LOUD_LP0_H0_REG;

    if (p_cfg_bass == NULL)
    {
        return;
    }

    RTD_Log(LOGGER_INFO,"Init Bass Coef.\n");
    //RTD_Log(LOGGER_INFO,"bf [0x3770] = %x\n",(UINT32)rtd_inl(LR_LOUD_A1_0_0x3770));
    //RTD_Log(LOGGER_INFO,"bf [0x377c] = %x\n",(UINT32)rtd_inl(LR_LOUD_H0_0_0x377c));

    // assume only 48K sample table & Bass band is LP0 of Loudness Module
    HwpSetReg(LR_LOUD_A1_0_0x3770, (INT32)p_cfg_bass->coef[0][ZERO_LOUD_VALUE].a1, TRUE);

    //LOUD_LP0_H0_REG.regValue = rtd_inl(LR_LOUD_H0_0_0x377c);
    HwpSetReg(LR_LOUD_H0_0_0x377c, (INT32)p_cfg_bass->coef[0][ZERO_LOUD_VALUE].h0, TRUE);

    //RTD_Log(LOGGER_INFO,"af [0x3770] = %x\n",(UINT32)rtd_inl(LR_LOUD_A1_0_0x3770));
    //RTD_Log(LOGGER_INFO,"af [0x377c] = %x\n",(UINT32)rtd_inl(LR_LOUD_H0_0_0x377c));

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Setup H/W Equalizer values
 *
 * @param <para>		{ AIO_ENABLE/AIO_DISABLE }
 * @param <p_val_eq>	{ pointer of a set of EQ value }
 * @return				{ AIO_OK or AIO_ERROR_CODE }
 * @ingroup lib_audio
 */
void Audio_HwpSetEQValue(BOOL para, APP_EQ_VAL *p_val_eq)
{
#ifndef CONFIG_ENABLE_AUDIO
    para = para;
    p_val_eq = p_val_eq;
#else
    eq_control_status_register_RBUS	EQ_CTRL_REG;
    APP_EQ_VAL	u_val_eq;
    UINT32		i;

    if (p_val_eq == NULL)
    {
        return;
    }

    if (para == _ENABLE)
    {
        EQ_CTRL_REG.regValue = rtd_inl(EQ_CONTROL_STATUS_REGISTER_0x376c);
        EQ_CTRL_REG.bitField.eq_lp_enable    = TRUE;
        EQ_CTRL_REG.bitField.eq_band1_enable = TRUE;
        EQ_CTRL_REG.bitField.eq_band2_enable = TRUE;
        EQ_CTRL_REG.bitField.eq_band3_enable = TRUE;
        EQ_CTRL_REG.bitField.eq_band4_enable = TRUE;
        EQ_CTRL_REG.bitField.eq_band5_enable = TRUE;
        /*
        EQ_CTRL_REG.eq_lp_of        = TRUE;
        EQ_CTRL_REG.eq_band1_of     = TRUE;
        EQ_CTRL_REG.eq_band2_of     = TRUE;
        EQ_CTRL_REG.eq_band3_of     = TRUE;
        EQ_CTRL_REG.eq_band4_of     = TRUE;
        EQ_CTRL_REG.eq_band5_of     = TRUE;
        */
        rtd_outl(EQ_CONTROL_STATUS_REGISTER_0x376c, EQ_CTRL_REG.regValue);
        //RTD_Log(LOGGER_INFO,"[EQ] Enable(%x)\n",(UINT32)EQ_CTRL_REG.regValue);

        HwpSetVal_EQ(p_val_eq);
    }
    else
    {
        u_val_eq.max_val = 6;
        u_val_eq.min_val = -6;
        for (i=0; i<APP_EQ_BAND_NUM; i++)
            u_val_eq.val[i] = 0;

        HwpSetVal_EQ(&u_val_eq);

        EQ_CTRL_REG.regValue = rtd_inl(EQ_CONTROL_STATUS_REGISTER_0x376c);
        EQ_CTRL_REG.bitField.eq_lp_enable    = FALSE;
        EQ_CTRL_REG.bitField.eq_band1_enable = FALSE;
        EQ_CTRL_REG.bitField.eq_band2_enable = FALSE;
        EQ_CTRL_REG.bitField.eq_band3_enable = FALSE;
        EQ_CTRL_REG.bitField.eq_band4_enable = FALSE;
        EQ_CTRL_REG.bitField.eq_band5_enable = FALSE;
        /*
        EQ_CTRL_REG.eq_lp_of        = TRUE;
        EQ_CTRL_REG.eq_band1_of     = TRUE;
        EQ_CTRL_REG.eq_band2_of     = TRUE;
        EQ_CTRL_REG.eq_band3_of     = TRUE;
        EQ_CTRL_REG.eq_band4_of     = TRUE;
        EQ_CTRL_REG.eq_band5_of     = TRUE;
        */
        rtd_outl(EQ_CONTROL_STATUS_REGISTER_0x376c, EQ_CTRL_REG.regValue);
        //RTD_Log(LOGGER_INFO,"[EQ] Disable(%x)\n",EQ_CTRL_REG.regValue);
    }

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Setup H/W Equalizer coefficients
 *
 * @param <p_cfg_eq>	{ pointer of EQ-Band control block }
 * @return				{ void }
 * @note
 *    This function is mutual with Speaker Compensation.
 *
 * @ingroup lib_audio
 */
void Audio_HwpSetEQCoef(APP_EQ_CFG *p_cfg_eq)
{
#ifdef CONFIG_ENABLE_AUDIO
    if (p_cfg_eq == NULL)
    {
        return;
    }

    RTD_Log(LOGGER_INFO,"Init Equalizer Coef.\n");

    // assume only 48K sample table
    // LP0
    HwpSetReg(EQ_A1_0_0x3740, (INT32)p_cfg_eq->coef[0][APP_EQ_BAND_ID0][ZERO_LOUD_VALUE].a1, TRUE);
    HwpSetReg(EQ_H0_1_0x3760, (INT32)p_cfg_eq->coef[0][APP_EQ_BAND_ID0][ZERO_LOUD_VALUE].h0, TRUE);
    // BP1
    HwpSetReg(EQ_A1_0_0x3740, (INT32)p_cfg_eq->coef[0][APP_EQ_BAND_ID1][ZERO_LOUD_VALUE].a1, FALSE);
    HwpSetReg(EQ_A2_1_0x3750, (INT32)p_cfg_eq->coef[0][APP_EQ_BAND_ID1][ZERO_LOUD_VALUE].a2, TRUE);
    HwpSetReg(EQ_H0_1_0x3760, (INT32)p_cfg_eq->coef[0][APP_EQ_BAND_ID1][ZERO_LOUD_VALUE].h0, FALSE);
    // BP2
    HwpSetReg(EQ_A1_1_0x3744, (INT32)p_cfg_eq->coef[0][APP_EQ_BAND_ID2][ZERO_LOUD_VALUE].a1, TRUE);
    HwpSetReg(EQ_A2_1_0x3750, (INT32)p_cfg_eq->coef[0][APP_EQ_BAND_ID2][ZERO_LOUD_VALUE].a2, FALSE);
    HwpSetReg(EQ_H0_2_0x3764, (INT32)p_cfg_eq->coef[0][APP_EQ_BAND_ID2][ZERO_LOUD_VALUE].h0, TRUE);
    // BP3
    HwpSetReg(EQ_A1_1_0x3744, (INT32)p_cfg_eq->coef[0][APP_EQ_BAND_ID3][ZERO_LOUD_VALUE].a1, FALSE);
    HwpSetReg(EQ_A2_2_0x3754, (INT32)p_cfg_eq->coef[0][APP_EQ_BAND_ID3][ZERO_LOUD_VALUE].a2, TRUE);
    HwpSetReg(EQ_H0_2_0x3764, (INT32)p_cfg_eq->coef[0][APP_EQ_BAND_ID3][ZERO_LOUD_VALUE].h0, FALSE);
    // BP4
    HwpSetReg(EQ_A1_2_0x3748, (INT32)p_cfg_eq->coef[0][APP_EQ_BAND_ID4][ZERO_LOUD_VALUE].a1, TRUE);
    HwpSetReg(EQ_A2_2_0x3754, (INT32)p_cfg_eq->coef[0][APP_EQ_BAND_ID4][ZERO_LOUD_VALUE].a2, FALSE);
    HwpSetReg(EQ_H0_3_0x3768, (INT32)p_cfg_eq->coef[0][APP_EQ_BAND_ID4][ZERO_LOUD_VALUE].h0, TRUE);
    // BP5
    HwpSetReg(EQ_A1_2_0x3748, (INT32)p_cfg_eq->coef[0][APP_EQ_BAND_ID4][ZERO_LOUD_VALUE].a1, FALSE);
    HwpSetReg(EQ_A2_3_0x3758, (INT32)p_cfg_eq->coef[0][APP_EQ_BAND_ID4][ZERO_LOUD_VALUE].a2, FALSE);
    HwpSetReg(EQ_H0_3_0x3768, (INT32)p_cfg_eq->coef[0][APP_EQ_BAND_ID4][ZERO_LOUD_VALUE].h0, FALSE);


#endif // !CONFIG_ENABLE_AUDIO
}

#if 0
/**
 * Setup Speaker Compensation
 *
 * @param <para>		{ _ENABLE/_DISABLE }
 * @return				{ void }
 * @note
 *    This function is mutual with EQ.
 *
 * @ingroup lib_audio
 */
void Audio_HwpSetSpeakerCompensation(BOOL para)
{
#ifndef CONFIG_ENABLE_AUDIO
    para = para;
#else
    eq_control_status_register_RBUS	EQ_CTRL_REG;
    UINT8	i,curr_fs;

    // reduce coef. size, 'coz there are no 8K/11K/12K/24K/22K cases in Sparrow project
    curr_fs = 0;//pb_path_info.curr_opt_fs;

//	printf("Audio_HwpSetSpeakerCompensation(%d)\n",para);

    if (para == _ENABLE)
    {
        EQ_CTRL_REG.regValue = rtd_inl(EQ_CONTROL_STATUS_REGISTER_0x376c);
        EQ_CTRL_REG.bitField.eq_lp_enable    = TRUE;
        EQ_CTRL_REG.bitField.eq_band1_enable = TRUE;
        EQ_CTRL_REG.bitField.eq_band2_enable = TRUE;
        EQ_CTRL_REG.bitField.eq_band3_enable = TRUE;
        EQ_CTRL_REG.bitField.eq_band4_enable = TRUE;
        EQ_CTRL_REG.bitField.eq_band5_enable = TRUE;
        /*
        EQ_CTRL_REG.eq_lp_of        = TRUE;
        EQ_CTRL_REG.eq_band1_of     = TRUE;
        EQ_CTRL_REG.eq_band2_of     = TRUE;
        EQ_CTRL_REG.eq_band3_of     = TRUE;
        EQ_CTRL_REG.eq_band4_of     = TRUE;
        EQ_CTRL_REG.eq_band5_of     = TRUE;
        */
        rtd_outl(EQ_CONTROL_STATUS_REGISTER_0x376c, EQ_CTRL_REG.regValue);

        for (i=0; i<APP_EQ_BAND_NUM; i++)
        {
            m_curr_coef_eq[i].a1 = m_coef_spk[curr_fs][i].a1;
            m_curr_coef_eq[i].a2 = m_coef_spk[curr_fs][i].a2;
            m_curr_coef_eq[i].h0 = m_coef_spk[curr_fs][i].h0;
        }

        // update EQ registers
        for (i=0; i<APP_EQ_BAND_NUM; i++)
        {
            if (HwpSetCoef_EQ(i) < 0)
            {
//				printk("[AUDIO_DRV] fail to set eq value!!!\n");
                break;
            }
        }

    }
    else
    {

        for (i=0; i<APP_EQ_BAND_NUM; i++)
        {
            m_curr_coef_eq[i].a1 = m_coef_spk[curr_fs][i].a1;
            m_curr_coef_eq[i].a2 = m_coef_spk[curr_fs][i].a2;
            m_curr_coef_eq[i].h0 = 0;
        }

        // update EQ registers
        for (i=0; i<APP_EQ_BAND_NUM; i++)
        {
            if (HwpSetCoef_EQ(i) < 0)
            {
//				printk("[AUDIO_DRV] fail to set eq value!!!\n");
                break;
            }
        }

        EQ_CTRL_REG.regValue = rtd_inl(EQ_CONTROL_STATUS_REGISTER_0x376c);
        EQ_CTRL_REG.bitField.eq_lp_enable    = FALSE;
        EQ_CTRL_REG.bitField.eq_band1_enable = FALSE;
        EQ_CTRL_REG.bitField.eq_band2_enable = FALSE;
        EQ_CTRL_REG.bitField.eq_band3_enable = FALSE;
        EQ_CTRL_REG.bitField.eq_band4_enable = FALSE;
        EQ_CTRL_REG.bitField.eq_band5_enable = FALSE;
        /*
        EQ_CTRL_REG.eq_lp_of        = TRUE;
        EQ_CTRL_REG.eq_band1_of     = TRUE;
        EQ_CTRL_REG.eq_band2_of     = TRUE;
        EQ_CTRL_REG.eq_band3_of     = TRUE;
        EQ_CTRL_REG.eq_band4_of     = TRUE;
        EQ_CTRL_REG.eq_band5_of     = TRUE;
        */
        rtd_outl(EQ_CONTROL_STATUS_REGISTER_0x376c, EQ_CTRL_REG.regValue);

    }

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Setup Speaker Compensation coefficients
 *
 * @param <p_cfg_spk>	{ pointer of Speaker Compensation control block }
 * @return				{ AIO_OK or AIO_ERROR_CODE }
 * @ingroup lib_audio
 */
void Audio_HwpSetSpeakerCompensationCoef(APP_SPK_CFG *p_cfg_spk)
{

    if (p_cfg_spk == NULL)
    {
        return;
    }

//	ioctl(audio_fd, HWP_CMD_SET_SPK_COEF, (void*)p_cfg_spk);

}
#endif
/**
 * Setup H/W Spatial Effect
 *
 * @param <para>		{ _ENABLE/_DISABLE }
 * @param <p_cfg_avc>	{ pointer of SE control block }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_HwpSetSpatialEffect(BOOL para, APP_SE_CFG *p_cfg_se)
{
#ifndef CONFIG_ENABLE_AUDIO
    para = para;
    p_cfg_se = p_cfg_se;
#else
    hw_process_control_RBUS			HWP_CTRL_REG;
    spatial_effect_control_0_RBUS	SPL_CTRL0_REG;
    spatial_effect_control_1_RBUS	SPL_CTRL1_REG;

    if (p_cfg_se == NULL)
    {
        return;
    }

    /*
     * disable H/W-SE before configure it.
     * This register is a double buffer register,we should check it after setting,
     * but this version of code, we just assume that H/W reaction is always fast enough.
     */
    HWP_CTRL_REG.regValue = rtd_inl(HW_PROCESS_CONTROL_0x3700);
    HWP_CTRL_REG.bitField.spatial_ctrlenable = _DISABLE;
    rtd_outl(HW_PROCESS_CONTROL_0x3700, HWP_CTRL_REG.regValue);

    if (para == _ENABLE)
    {
        // combine spatial & mono-to-stereo together.
        SPL_CTRL0_REG.regValue = rtd_inl(SPATIAL_EFFECT_CONTROL_0_0x3730);
        SPL_CTRL0_REG.bitField.apf_en = p_cfg_se->apf_en;
        SPL_CTRL0_REG.bitField.f_coef = p_cfg_se->apf_coef;
        SPL_CTRL0_REG.bitField.psb_en = p_cfg_se->psb_en;
        SPL_CTRL0_REG.bitField.seb_en = p_cfg_se->seb_en;
        SPL_CTRL0_REG.bitField.spatial_in_l_sel = p_cfg_se->sel_l;
        SPL_CTRL0_REG.bitField.spatial_in_r_sel = p_cfg_se->sel_r;
        rtd_outl(SPATIAL_EFFECT_CONTROL_0_0x3730, SPL_CTRL0_REG.regValue);

        SPL_CTRL1_REG.regValue = rtd_inl(SPATIAL_EFFECT_CONTROL_1_0x3734);
        SPL_CTRL1_REG.bitField.depth1 = p_cfg_se->depth1;
        SPL_CTRL1_REG.bitField.depth2 = p_cfg_se->depth2;
        rtd_outl(SPATIAL_EFFECT_CONTROL_1_0x3734, SPL_CTRL1_REG.regValue);

        // enable H/W-Spatial Effect
        HWP_CTRL_REG.regValue = rtd_inl(HW_PROCESS_CONTROL_0x3700);
        HWP_CTRL_REG.bitField.spatial_ctrlenable = _ENABLE;
        rtd_outl(HW_PROCESS_CONTROL_0x3700, HWP_CTRL_REG.regValue);
    }

#endif // CONFIG_ENABLE_AUDIO
}
#if 0
/**
 * Setup H/W Beeper
 *
 * @param <para>			{ _ENABLE/_DISABLE }
 * @param <p_cfg_beeper>	{ pointer of BEEPER control block }
 * @return					{ void }
 * @ingroup lib_audio
 */
void Audio_HwpSetBeeper(BOOL para, APP_BEEP_CFG *p_cfg_beeper)
{
#ifndef CONFIG_ENABLE_AUDIO
    para = para;
    p_cfg_beeper = p_cfg_beeper;
#else
    programmable_beeper_control_RBUS	BEEP_CTRL_REG;
    UINT8	curr_fs;
    UINT32	ui32;

    BEEP_CTRL_REG.regValue = rtd_inl(PROGRAMMABLE_BEEPER_CONTROL_0x372c);

    if (para == _ENABLE)
    {
        // reduec table size
        curr_fs = 0;//pb_path_info.curr_opt_fs;

        pb_path_info.beeper_freq     = p_cfg_beeper->freq;	// backup for sample rate change issue
        pb_path_info.beeper_duration = p_cfg_beeper->duration;

        ui32 = (p_cfg_beeper->freq * audio_sample_per_ms[curr_fs])/4000;
        if (ui32 > 0x0FF)
            ui32 = 0x0FF;

        BEEP_CTRL_REG.bitField.freq_beeper = (UINT8)ui32;
        BEEP_CTRL_REG.bitField.mode_beeper = p_cfg_beeper->mode;
        BEEP_CTRL_REG.bitField.mute_beeper = p_cfg_beeper->mute;
        BEEP_CTRL_REG.bitField.beeper_volume = p_cfg_beeper->vol;

        ui32 = (p_cfg_beeper->duration * audio_sample_per_ms[curr_fs])/8;
        if (ui32 > 0x0FF)
            ui32 = 0x0FF;

        BEEP_CTRL_REG.bitField.duration_beeper = (UINT8)ui32;
    }
    else
    {
        BEEP_CTRL_REG.bitField.freq_beeper = 0;	// disable beeper generator
        BEEP_CTRL_REG.bitField.mute_beeper = TRUE;
    }

    rtd_outl(PROGRAMMABLE_BEEPER_CONTROL_0x372c, BEEP_CTRL_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}
#endif

/*===================== Local Subroutine ======================*/
#ifdef CONFIG_ENABLE_AUDIO
/**
 * Set Digital Volume
 *
 * @param <ch_id>	{ APP_CH_ID_L / APP_CH_ID_R / ... ,refer to audio_app.h }
 * @param <vol>		{ range from APP_MIN_VOL ~ APP_MAX_VOL }
 * @return			{ AIO_OK or AIO_ERROR_CODE }
 * @ingroup lib_audio
 */
static void HwpSetVolume(UINT8 ch_id, INT16 vol)
{
    digital_volume_control_0_RBUS	DVOL_CTRL_REG;
    lr_digital_volume_control_RBUS	DVOL_SET_REG;	// assume register of L/R , Ls/Rs and C/SW is identical.
    INT32	a,b,c,lr_idx=0;
    UINT32	reg_addr_set;
    UINT16	time_out = 0;

    while (1)
    {
        DVOL_CTRL_REG.regValue = rtd_inl(DIGITAL_VOLUME_CONTROL_0_0x3704);
        if (DVOL_CTRL_REG.bitField.dvol_apply == 1)
        {
            //RTD_Log(LOGGER_INFO,"[AUDIO][APP] dvol_apply = 1\n");
        }
        else
        {
            break;
        }
        time_out++;

        if (time_out > 0x8000)
        {
            RTD_Log(LOGGER_INFO,"[AUDIO][APP] HwpSetVolume() TimeOut(%x)!!!\n",(UINT32)time_out);
            return;
        }
    }
    if (time_out)
    {
        //RTD_Log(LOGGER_INFO,"[AUDIO][APP] TimeOut = %d\n",(UINT32)time_out);
    }

    // change dvol setting to DVOL mode
    DVOL_CTRL_REG.regValue = rtd_inl(DIGITAL_VOLUME_CONTROL_0_0x3704);
    DVOL_CTRL_REG.bitField.mute_apply = FALSE;
    rtd_outl(DIGITAL_VOLUME_CONTROL_0_0x3704, DVOL_CTRL_REG.regValue);


    if (vol > APP_MAX_VOL)	vol = APP_MAX_VOL;
    if (vol < APP_MIN_VOL)	vol = APP_MIN_VOL;

    c = vol & 0x7;

    a = (vol>>3) / 6;
    b = (vol>>3) % 6;
    if (b < 0)
    {
        a--;
        b += 6;
    }

//	RTD_Log(LOGGER_INFO,"[AUDIO][APP] vol=%d, a=%d,b=%d,c=%d\n",vol,a,b,c);

    switch (ch_id)
    {
    case APP_CH_ID_L:
        lr_idx = 1;
        reg_addr_set = LR_DIGITAL_VOLUME_CONTROL_0x3708;
        break;
    case APP_CH_ID_R:
        lr_idx = 0;
        reg_addr_set = LR_DIGITAL_VOLUME_CONTROL_0x3708;
        break;
    case APP_CH_ID_LS:
        lr_idx = 1;
        reg_addr_set = LS_DIGITAL_VOLUME_CONTROL_0x370c;
        break;
    case APP_CH_ID_RS:
        lr_idx = 0;
        reg_addr_set = LS_DIGITAL_VOLUME_CONTROL_0x370c;
        break;
    case APP_CH_ID_C:
        lr_idx = 1;
        reg_addr_set = C_SW_DIGITAL_VOLUME_CONTROL_0x3710;
        break;
    case APP_CH_ID_SW:
        // ignore sw_source setting
        lr_idx = 0;
        reg_addr_set = C_SW_DIGITAL_VOLUME_CONTROL_0x3710;
        break;
    default:
        return;
    }

    DVOL_SET_REG.regValue = rtd_inl(reg_addr_set);

    if (lr_idx)
    {
        DVOL_SET_REG.bitField.rep_l_status = TRUE;
        DVOL_SET_REG.bitField.hpf_l_enable = TRUE;
        DVOL_SET_REG.bitField.vol_l_a = a;
        DVOL_SET_REG.bitField.vol_l_b = (b<<3) | c;
    }
    else
    {
        DVOL_SET_REG.bitField.rep_r_status = TRUE;
        DVOL_SET_REG.bitField.hpf_r_enable = TRUE;
        DVOL_SET_REG.bitField.vol_r_a = a;
        DVOL_SET_REG.bitField.vol_r_b = (b<<3) | c;
    }

//	APP_LOG("rw : [0x%08x] = 0x%08x\n",reg_addr_set,DVOL_SET_REG.regValue);
    rtd_outl(reg_addr_set, DVOL_SET_REG.regValue);

    DVOL_CTRL_REG.regValue = rtd_inl(DIGITAL_VOLUME_CONTROL_0_0x3704);
    DVOL_CTRL_REG.bitField.dvol_apply = TRUE;
    rtd_outl(DIGITAL_VOLUME_CONTROL_0_0x3704, DVOL_CTRL_REG.regValue);

}

/**
 * Set Digital Volume ( TimeShift(SCART) path )
 *
 * @param <ch_id>	{ APP_CH_ID_L / APP_CH_ID_R / ... ,refer to audio_app.h }
 * @param <vol>		{ range from APP_MIN_VOL ~ APP_MAX_VOL }
 * @return			{ AIO_OK or AIO_ERROR_CODE }
 * @ingroup lib_audio
 */
static void HwpSetVolumeTS(UINT8 ch_id, INT16 vol)
{
    lrrr_digital_volume_control_0_RBUS	DVOL_CTRL_REG;
    lrrr_digital_volume_control_2_RBUS	DVOL_SET_REG;
    INT32	a,b,c,lr_idx=0;
    UINT32	reg_addr_set;
    UINT16	time_out = 0;

    while (1)
    {
        DVOL_CTRL_REG.regValue = rtd_inl(LRRR_DIGITAL_VOLUME_CONTROL_0_0x3800);
        if (DVOL_CTRL_REG.bitField.lrrr_dvol_apply == 1)
        {
            //RTD_Log(LOGGER_INFO,"[AUDIO][APP] lrrr_dvol_apply = 1\n");
        }
        else
        {
            break;
        }
        time_out++;

        if (time_out > 0x8000)
        {
            RTD_Log(LOGGER_INFO,"[AUDIO][APP] HwpSetVolumeTS() TimeOut(%x)!!!\n",(UINT32)time_out);
            return;
        }
    }
    if (time_out)
    {
        //RTD_Log(LOGGER_INFO,"[AUDIO][APP] TimeOut = %d\n",(UINT32)time_out);
    }

    // change dvol setting to DVOL mode
    DVOL_CTRL_REG.regValue = rtd_inl(LRRR_DIGITAL_VOLUME_CONTROL_0_0x3800);
    DVOL_CTRL_REG.bitField.lrrr_mute_apply = FALSE;
    rtd_outl(LRRR_DIGITAL_VOLUME_CONTROL_0_0x3800, DVOL_CTRL_REG.regValue);


    if (vol > APP_MAX_VOL)	vol = APP_MAX_VOL;
    if (vol < APP_MIN_VOL)	vol = APP_MIN_VOL;

    c = vol & 0x7;

    a = (vol>>3) / 6;
    b = (vol>>3) % 6;
    if (b < 0)
    {
        a--;
        b += 6;
    }

//	RTD_Log(LOGGER_INFO,"[AUDIO][APP] ts_vol=%d, a=%d,b=%d,c=%d\n",vol,a,b,c);

    switch (ch_id)
    {
    case APP_CH_ID_L:
        lr_idx = 1;
        reg_addr_set = LRRR_DIGITAL_VOLUME_CONTROL_2_0x3808;
        break;
    case APP_CH_ID_R:
        lr_idx = 0;
        reg_addr_set = LRRR_DIGITAL_VOLUME_CONTROL_2_0x3808;
        break;
    default:
        return;
    }

    DVOL_SET_REG.regValue = rtd_inl(reg_addr_set);

    if (lr_idx)
    {
        DVOL_SET_REG.bitField.hpf_lr_enable = TRUE;
        DVOL_SET_REG.bitField.vol_lr_a = a;
        DVOL_SET_REG.bitField.vol_lr_b = (b<<3) | c;
    }
    else
    {
        DVOL_SET_REG.bitField.hpf_rr_enable = TRUE;
        DVOL_SET_REG.bitField.vol_rr_a = a;
        DVOL_SET_REG.bitField.vol_rr_b = (b<<3) | c;
    }

//	RTD_Log(LOGGER_INFO,"rw : [0x%08x] = 0x%08x\n",reg_addr_set,DVOL_SET_REG.regValue);
    rtd_outl(reg_addr_set, DVOL_SET_REG.regValue);

    DVOL_CTRL_REG.regValue = rtd_inl(LRRR_DIGITAL_VOLUME_CONTROL_0_0x3800);
    DVOL_CTRL_REG.bitField.lrrr_dvol_apply = TRUE;
    rtd_outl(LRRR_DIGITAL_VOLUME_CONTROL_0_0x3800, DVOL_CTRL_REG.regValue);

}


/**
 * mute/unmute the digital audio output
 *
 * @param <para>		{ AIO_ENABLE / AIO_DISABLE }
 * @param <ch_id>		{ APP_CH_ID_L | APP_CH_ID_R |... , refrer to audio_app.h }
 * @return 				{ void }
 * @ingroup drv_audio
 */
static void HwpSetMute(BOOL para, UINT8 ch_id)
{
    lr_digital_volume_control_RBUS		DVOL_CH_LR_REG;
    ls_digital_volume_control_RBUS		DVOL_CH_LSRS_REG;
    c_sw_digital_volume_control_RBUS	DVOL_CH_CSW_REG;
    digital_volume_control_0_RBUS		DVOL_CTRL_REG;
    UINT16	time_out = 0;

    while (1)
    {
        DVOL_CTRL_REG.regValue = rtd_inl(DIGITAL_VOLUME_CONTROL_0_0x3704);
        if (DVOL_CTRL_REG.bitField.dvol_apply == 1)
        {
            //printk("[AUDIO][DRV] dvol_apply = 1\n");
        }
        else
        {
            break;
        }
        time_out++;

        if (time_out > 0x8000)
        {
            RTD_Log(LOGGER_INFO,"[AUDIO][APP] HwpSetMute() TimeOut(%x)!!!\n",(UINT32)time_out);
            return;
        }
    }
    if (time_out)
    {
        //RTD_Log(LOGGER_INFO,"[AUDIO][APP] TimeOut = %d\n",(UINT32)time_out);
    }

    // change dvol setting to MUTE mode
    DVOL_CTRL_REG.regValue = rtd_inl(DIGITAL_VOLUME_CONTROL_0_0x3704);
    DVOL_CTRL_REG.bitField.mute_apply = TRUE;
    rtd_outl(DIGITAL_VOLUME_CONTROL_0_0x3704, DVOL_CTRL_REG.regValue);


    if (ch_id & APP_CH_ID_L)
    {
        DVOL_CH_LR_REG.regValue = rtd_inl(LR_DIGITAL_VOLUME_CONTROL_0x3708);
        DVOL_CH_LR_REG.bitField.mute_l = para;
        DVOL_CH_LR_REG.bitField.rep_l_status = _ENABLE;
        DVOL_CH_LR_REG.bitField.hpf_l_enable = _ENABLE;
        rtd_outl(LR_DIGITAL_VOLUME_CONTROL_0x3708, DVOL_CH_LR_REG.regValue);
    }
    if (ch_id & APP_CH_ID_R)
    {
        DVOL_CH_LR_REG.regValue = rtd_inl(LR_DIGITAL_VOLUME_CONTROL_0x3708);
        DVOL_CH_LR_REG.bitField.mute_r = para;
        DVOL_CH_LR_REG.bitField.rep_r_status = _ENABLE;
        DVOL_CH_LR_REG.bitField.hpf_r_enable = _ENABLE;
        rtd_outl(LR_DIGITAL_VOLUME_CONTROL_0x3708, DVOL_CH_LR_REG.regValue);
    }
    if (ch_id & APP_CH_ID_LS)
    {
        DVOL_CH_LSRS_REG.regValue = rtd_inl(LS_DIGITAL_VOLUME_CONTROL_0x370c);
        DVOL_CH_LSRS_REG.bitField.mute_ls = para;
        DVOL_CH_LSRS_REG.bitField.rep_ls_status = _ENABLE;
        DVOL_CH_LSRS_REG.bitField.hpf_ls_enable = _ENABLE;
        rtd_outl(LS_DIGITAL_VOLUME_CONTROL_0x370c, DVOL_CH_LSRS_REG.regValue);
    }
    if (ch_id & APP_CH_ID_RS)
    {
        DVOL_CH_LSRS_REG.regValue = rtd_inl(LS_DIGITAL_VOLUME_CONTROL_0x370c);
        DVOL_CH_LSRS_REG.bitField.mute_rs = para;
        DVOL_CH_LSRS_REG.bitField.rep_rs_status = _ENABLE;
        DVOL_CH_LSRS_REG.bitField.hpf_rs_enable = _ENABLE;
        rtd_outl(LS_DIGITAL_VOLUME_CONTROL_0x370c, DVOL_CH_LSRS_REG.regValue);
    }
    if (ch_id & APP_CH_ID_C)
    {
        DVOL_CH_CSW_REG.regValue = rtd_inl(C_SW_DIGITAL_VOLUME_CONTROL_0x3710);
        DVOL_CH_CSW_REG.bitField.mute_c = para;
        DVOL_CH_CSW_REG.bitField.rep_c_status = _ENABLE;
        DVOL_CH_CSW_REG.bitField.hpf_c_enable = _ENABLE;
        rtd_outl(C_SW_DIGITAL_VOLUME_CONTROL_0x3710, DVOL_CH_CSW_REG.regValue);
    }
    if (ch_id & APP_CH_ID_SW)
    {
        DVOL_CH_CSW_REG.regValue = rtd_inl(C_SW_DIGITAL_VOLUME_CONTROL_0x3710);
        DVOL_CH_CSW_REG.bitField.mute_sw = para;
        DVOL_CH_CSW_REG.bitField.rep_sw_status = _ENABLE;
        DVOL_CH_CSW_REG.bitField.hpf_sw_enable = _ENABLE;
        rtd_outl(C_SW_DIGITAL_VOLUME_CONTROL_0x3710, DVOL_CH_CSW_REG.regValue);
    }

    if (ch_id)
    {
        DVOL_CTRL_REG.regValue = rtd_inl(DIGITAL_VOLUME_CONTROL_0_0x3704);
        DVOL_CTRL_REG.bitField.dvol_apply = _ENABLE;
        rtd_outl(DIGITAL_VOLUME_CONTROL_0_0x3704, DVOL_CTRL_REG.regValue);
    }

}

/**
 * mute/unmute the digital audio output ( TimeShift(SCART) path )
 *
 * @param <para>		{ AIO_ENABLE / AIO_DISABLE }
 * @param <ch_id>		{ APP_CH_ID_L | APP_CH_ID_R |... , refrer to audio_app.h }
 * @return 				{ void }
 * @ingroup drv_audio
 */
static void HwpSetMuteTS(BOOL para, UINT8 ch_id)
{
    lrrr_digital_volume_control_2_RBUS	DVOL_CH_LR_REG;
    lrrr_digital_volume_control_0_RBUS	DVOL_CTRL_REG;
    UINT16	time_out = 0;

    while (1)
    {
        DVOL_CTRL_REG.regValue = rtd_inl(LRRR_DIGITAL_VOLUME_CONTROL_0_0x3800);
        if (DVOL_CTRL_REG.bitField.lrrr_dvol_apply == 1)
        {
            //RTD_Log(LOGGER_INFO,"[AUDIO][DRV] lrrr_dvol_apply = 1\n");
        }
        else
        {
            break;
        }
        time_out++;

        if (time_out > 0x8000)
        {
            RTD_Log(LOGGER_INFO,"[AUDIO][APP] HwpSetMuteTS() TimeOut(%x)!!!\n",(UINT32)time_out);
            return;
        }
    }
    if (time_out)
    {
        //RTD_Log(LOGGER_INFO,"[AUDIO][APP] TimeOut = %d\n",(UINT32)time_out);
    }

    // change dvol setting to MUTE mode
    DVOL_CTRL_REG.regValue = rtd_inl(LRRR_DIGITAL_VOLUME_CONTROL_0_0x3800);
    DVOL_CTRL_REG.bitField.lrrr_mute_apply = TRUE;
    rtd_outl(LRRR_DIGITAL_VOLUME_CONTROL_0_0x3800, DVOL_CTRL_REG.regValue);


    if (ch_id & APP_CH_ID_L)
    {
        DVOL_CH_LR_REG.regValue = rtd_inl(LRRR_DIGITAL_VOLUME_CONTROL_2_0x3808);
        DVOL_CH_LR_REG.bitField.mute_lr = para;
        DVOL_CH_LR_REG.bitField.hpf_lr_enable = _ENABLE;
        rtd_outl(LRRR_DIGITAL_VOLUME_CONTROL_2_0x3808, DVOL_CH_LR_REG.regValue);
    }
    if (ch_id & APP_CH_ID_R)
    {
        DVOL_CH_LR_REG.regValue = rtd_inl(LRRR_DIGITAL_VOLUME_CONTROL_2_0x3808);
        DVOL_CH_LR_REG.bitField.mute_rr = para;
        DVOL_CH_LR_REG.bitField.hpf_rr_enable = _ENABLE;
        rtd_outl(LRRR_DIGITAL_VOLUME_CONTROL_2_0x3808, DVOL_CH_LR_REG.regValue);
    }

}


static INT8 HwpSetVal_Treble(APP_TREBLE_VAL *p_val_treble)
{
    INT32	interval,input;
    UINT8	base,curr_fs;

    //RTD_Log(LOGGER_INFO,"[AUDIO][APP] Treble Val = %d\n",(UINT32)p_val_treble->val);
    //RTD_Log(LOGGER_INFO,"[AUDIO][APP] Treble Max = %d\n",(UINT32)p_val_treble->max_val);
    //RTD_Log(LOGGER_INFO,"[AUDIO][APP] Treble Min = %d\n",(UINT32)p_val_treble->min_val);

    if (p_val_treble->max_val <= 0)
    {
//		printk("[AUDIO_DRV] Treble max_val can't be less than ZERO\n");
        return -1;
    }

    // reduce coef. size, 'coz there are no 8K/11K/12K/24K/22K cases in Sparrow project
    curr_fs = 0;//pb_path_info.curr_opt_fs;

    interval = p_val_treble->max_val / (MAX_LOUD_VALUE/2);

    // band 0 : LPF
    input = p_val_treble->max_val + p_val_treble->val;
    if (input < 0) input = 0;

    base = 0;
    if (p_val_treble->val == 0)
    {
        input = 0;
        base = ZERO_LOUD_VALUE;
    }
    else
    {
        while (input > interval)
        {
            input-=interval;
            base++;
        }
    }
    if (base >= MAX_LOUD_VALUE) input = 0;
//	printk("BPF: base=%d,offset=%d\n",base,input);

    m_curr_coef_treble.a1 = (int)(m_coef_treble[curr_fs][base].a1 * (interval-input) + m_coef_treble[curr_fs][base+1].a1 * input)/interval;
    m_curr_coef_treble.a2 = (int)(m_coef_treble[curr_fs][base].a2 * (interval-input) + m_coef_treble[curr_fs][base+1].a2 * input)/interval;
    m_curr_coef_treble.h0 = (int)(m_coef_treble[curr_fs][base].h0 * (interval-input) + m_coef_treble[curr_fs][base+1].h0 * input)/interval;

    if (HwpSetCoef_Loudness(APP_TREBLE_BAND_ID, TRUE) < 0)
    {
//		printk("[AUDIO_DRV] fail to set treble value!!!\n");
        return -1;
    }

    return 0;
}

static INT8 HwpSetVal_Bass(APP_BASS_VAL *p_val_bass)
{
    INT32	interval,input;
    UINT8	base,curr_fs;

    //RTD_Log(LOGGER_INFO,"[AUDIO[APP] Bass Val = %d\n",(UINT32)p_val_bass->val);
    //RTD_Log(LOGGER_INFO,"[AUDIO[APP] Bass Max = %d\n",(UINT32)p_val_bass->max_val);
    //RTD_Log(LOGGER_INFO,"[AUDIO[APP] Bass Min = %d\n",(UINT32)p_val_bass->min_val);

    if (p_val_bass->max_val <= 0)
    {
//		printk("[AUDIO_DRV] Bass max_val can't be less than ZERO\n");
        return -1;
    }

    // reduce coef. size, 'coz there are no 8K/11K/12K/24K/22K cases in Sparrow project
    curr_fs = 0;//pb_path_info.curr_opt_fs;

    interval = p_val_bass->max_val / (MAX_LOUD_VALUE/2);

    // band 0 : LPF
    input = p_val_bass->max_val + p_val_bass->val;
    if (input < 0) input = 0;

    base = 0;
    if (p_val_bass->val == 0)
    {
        input = 0;
        base = ZERO_LOUD_VALUE;
    }
    else
    {
        while (input > interval)
        {
            input-=interval;
            base++;
        }
    }
    if (base >= MAX_LOUD_VALUE) input = 0;
//	printk("LPF: base=%d,offset=%d\n",base,input);

    m_curr_coef_bass.a1 = (int)(m_coef_bass[curr_fs][base].a1 * (interval-input) + m_coef_bass[curr_fs][base+1].a1 * input)/interval;
    m_curr_coef_bass.a2 = m_curr_coef_bass.a1; // dummy
    m_curr_coef_bass.h0 = (int)(m_coef_bass[curr_fs][base].h0 * (interval-input) + m_coef_bass[curr_fs][base+1].h0 * input)/interval;

    if (HwpSetCoef_Loudness(APP_BASS_BAND_ID, TRUE) < 0)
    {
//		printk("[AUDIO_DRV] fail to set bass value!!!\n");
        return -1;
    }

    return 0;
}


static INT8 HwpSetVal_EQ(APP_EQ_VAL *p_val_eq)
{
    INT32	interval,input;
    UINT8	base,curr_fs,i;

    //RTD_Log(LOGGER_INFO,"LPF_0 = %d\n",(UINT32)p_val_eq->val[0]);
    //RTD_Log(LOGGER_INFO,"BPF_1 = %d\n",(UINT32)p_val_eq->val[1]);
    //RTD_Log(LOGGER_INFO,"BPF_2 = %d\n",(UINT32)p_val_eq->val[2]);
    //RTD_Log(LOGGER_INFO,"BPF_3 = %d\n",(UINT32)p_val_eq->val[3]);
    //RTD_Log(LOGGER_INFO,"BPF_4 = %d\n",(UINT32)p_val_eq->val[4]);
    //RTD_Log(LOGGER_INFO,"BPF_5 = %d\n",(UINT32)p_val_eq->val[5]);
    //RTD_Log(LOGGER_INFO,"MAX_V = %d\n",(UINT32)p_val_eq->max_val);
    //RTD_Log(LOGGER_INFO,"MIN_V = %d\n",(UINT32)p_val_eq->min_val);

    if (p_val_eq->max_val <= 0)
    {
//		printk("[AUDIO_DRV] EQ max_val can't be less than ZERO\n");
        return -1;
    }

    // reduce coef. size, 'coz there are no 8K/11K/12K/24K/22K cases in Sparrow project
    curr_fs = 0;//pb_path_info.curr_opt_fs;
    //RTD_Log(LOGGER_INFO,"HwpSetVal_EQ --- curr_fs = %d\n",(UINT32)curr_fs);

    interval = p_val_eq->max_val / (MAX_EQ_VALUE/2);

    // band 0 : LPF
    input = p_val_eq->max_val + p_val_eq->val[APP_EQ_BAND_ID0];
    if (input < 0) input = 0;

    base = 0;
    if (p_val_eq->val[APP_EQ_BAND_ID0] == 0)
    {
        input = 0;
        base = ZERO_EQ_VALUE;
    }
    else
    {
        while (input > interval)
        {
            input-=interval;
            base++;
        }
    }
    if (base >= MAX_EQ_VALUE) input = 0;
    //RTD_Log(LOGGER_INFO,"LPF_0: base=%d,offset=%d\n",(UINT32)base,(INT32)input);

    m_curr_coef_eq[APP_EQ_BAND_ID0].a1 = (int)(m_coef_eq[curr_fs][APP_EQ_BAND_ID0][base].a1 * (interval-input) + m_coef_eq[curr_fs][APP_EQ_BAND_ID0][base+1].a1 * input)/interval;
    m_curr_coef_eq[APP_EQ_BAND_ID0].a2 = m_curr_coef_eq[APP_EQ_BAND_ID0].a1; // dummy
    m_curr_coef_eq[APP_EQ_BAND_ID0].h0 = (int)(m_coef_eq[curr_fs][APP_EQ_BAND_ID0][base].h0 * (interval-input) + m_coef_eq[curr_fs][APP_EQ_BAND_ID0][base+1].h0 * input)/interval;

    // band 1 : BPF
    input = p_val_eq->max_val + p_val_eq->val[APP_EQ_BAND_ID1];
    if (input < 0) input = 0;

    base = 0;
    if (p_val_eq->val[APP_EQ_BAND_ID1] == 0)
    {
        input = 0;
        base = ZERO_EQ_VALUE;
    }
    else
    {
        while (input > interval)
        {
            input-=interval;
            base++;
        }
    }
    if (base >= MAX_EQ_VALUE) input = 0;
    //RTD_Log(LOGGER_INFO,"BPF_1: base=%d,offset=%d\n",(UINT32)base,(INT32)input);

    m_curr_coef_eq[APP_EQ_BAND_ID1].a1 = (int)(m_coef_eq[curr_fs][APP_EQ_BAND_ID1][base].a1 * (interval-input) + m_coef_eq[curr_fs][APP_EQ_BAND_ID1][base+1].a1 * input)/interval;
    m_curr_coef_eq[APP_EQ_BAND_ID1].a2 = (int)(m_coef_eq[curr_fs][APP_EQ_BAND_ID1][base].a2 * (interval-input) + m_coef_eq[curr_fs][APP_EQ_BAND_ID1][base+1].a2 * input)/interval;
    m_curr_coef_eq[APP_EQ_BAND_ID1].h0 = (int)(m_coef_eq[curr_fs][APP_EQ_BAND_ID1][base].h0 * (interval-input) + m_coef_eq[curr_fs][APP_EQ_BAND_ID1][base+1].h0 * input)/interval;

    // band 2 : BPF
    input = p_val_eq->max_val + p_val_eq->val[APP_EQ_BAND_ID2];
    if (input < 0) input = 0;

    base = 0;
    if (p_val_eq->val[APP_EQ_BAND_ID2] == 0)
    {
        input = 0;
        base = ZERO_EQ_VALUE;
    }
    else
    {
        while (input > interval)
        {
            input-=interval;
            base++;
        }
    }
    if (base >= MAX_EQ_VALUE) input = 0;
    //RTD_Log(LOGGER_INFO,"BPF_2: base=%d,offset=%d\n",(UINT32)base,(INT32)input);

    m_curr_coef_eq[APP_EQ_BAND_ID2].a1 = (int)(m_coef_eq[curr_fs][APP_EQ_BAND_ID2][base].a1 * (interval-input) + m_coef_eq[curr_fs][APP_EQ_BAND_ID2][base+1].a1 * input)/interval;
    m_curr_coef_eq[APP_EQ_BAND_ID2].a2 = (int)(m_coef_eq[curr_fs][APP_EQ_BAND_ID2][base].a2 * (interval-input) + m_coef_eq[curr_fs][APP_EQ_BAND_ID2][base+1].a2 * input)/interval;
    m_curr_coef_eq[APP_EQ_BAND_ID2].h0 = (int)(m_coef_eq[curr_fs][APP_EQ_BAND_ID2][base].h0 * (interval-input) + m_coef_eq[curr_fs][APP_EQ_BAND_ID2][base+1].h0 * input)/interval;

    // band 3 : BPF
    input = p_val_eq->max_val + p_val_eq->val[APP_EQ_BAND_ID3];
    if (input < 0) input = 0;

    base = 0;
    if (p_val_eq->val[APP_EQ_BAND_ID3] == 0)
    {
        input = 0;
        base = ZERO_EQ_VALUE;
    }
    else
    {
        while (input > interval)
        {
            input-=interval;
            base++;
        }
    }
    if (base >= MAX_EQ_VALUE) input = 0;
    //RTD_Log(LOGGER_INFO,"BPF_3: base=%d,offset=%d\n",(UINT32)base,(INT32)input);

    m_curr_coef_eq[APP_EQ_BAND_ID3].a1 = (int)(m_coef_eq[curr_fs][APP_EQ_BAND_ID3][base].a1 * (interval-input) + m_coef_eq[curr_fs][APP_EQ_BAND_ID3][base+1].a1 * input)/interval;
    m_curr_coef_eq[APP_EQ_BAND_ID3].a2 = (int)(m_coef_eq[curr_fs][APP_EQ_BAND_ID3][base].a2 * (interval-input) + m_coef_eq[curr_fs][APP_EQ_BAND_ID3][base+1].a2 * input)/interval;
    m_curr_coef_eq[APP_EQ_BAND_ID3].h0 = (int)(m_coef_eq[curr_fs][APP_EQ_BAND_ID3][base].h0 * (interval-input) + m_coef_eq[curr_fs][APP_EQ_BAND_ID3][base+1].h0 * input)/interval;

    // band 4 : BPF
    input = p_val_eq->max_val + p_val_eq->val[APP_EQ_BAND_ID4];
    if (input < 0) input = 0;

    base = 0;
    if (p_val_eq->val[APP_EQ_BAND_ID4] == 0)
    {
        input = 0;
        base = ZERO_EQ_VALUE;
    }
    else
    {
        while (input > interval)
        {
            input-=interval;
            base++;
        }
    }
    if (base >= MAX_EQ_VALUE) input = 0;
    //RTD_Log(LOGGER_INFO,"BPF_4: base=%d,offset=%d\n",(UINT32)base,(INT32)input);

    m_curr_coef_eq[APP_EQ_BAND_ID4].a1 = (int)(m_coef_eq[curr_fs][APP_EQ_BAND_ID4][base].a1 * (interval-input) + m_coef_eq[curr_fs][APP_EQ_BAND_ID4][base+1].a1 * input)/interval;
    m_curr_coef_eq[APP_EQ_BAND_ID4].a2 = (int)(m_coef_eq[curr_fs][APP_EQ_BAND_ID4][base].a2 * (interval-input) + m_coef_eq[curr_fs][APP_EQ_BAND_ID4][base+1].a2 * input)/interval;
    m_curr_coef_eq[APP_EQ_BAND_ID4].h0 = (int)(m_coef_eq[curr_fs][APP_EQ_BAND_ID4][base].h0 * (interval-input) + m_coef_eq[curr_fs][APP_EQ_BAND_ID4][base+1].h0 * input)/interval;

    // band 5 : BPF
    input = p_val_eq->max_val + p_val_eq->val[APP_EQ_BAND_ID5];
    if (input < 0) input = 0;

    base = 0;
    if (p_val_eq->val[APP_EQ_BAND_ID5] == 0)
    {
        input = 0;
        base = ZERO_EQ_VALUE;
    }
    else
    {
        while (input > interval)
        {
            input-=interval;
            base++;
        }
    }
    if (base >= MAX_EQ_VALUE) input = 0;
    //RTD_Log(LOGGER_INFO,"BPF_5: base=%d,offset=%d\n",(UINT32)base,(INT32)input);

    m_curr_coef_eq[APP_EQ_BAND_ID5].a1 = (int)(m_coef_eq[curr_fs][APP_EQ_BAND_ID5][base].a1 * (interval-input) + m_coef_eq[curr_fs][APP_EQ_BAND_ID5][base+1].a1 * input)/interval;
    m_curr_coef_eq[APP_EQ_BAND_ID5].a2 = (int)(m_coef_eq[curr_fs][APP_EQ_BAND_ID5][base].a2 * (interval-input) + m_coef_eq[curr_fs][APP_EQ_BAND_ID5][base+1].a2 * input)/interval;
    m_curr_coef_eq[APP_EQ_BAND_ID5].h0 = (int)(m_coef_eq[curr_fs][APP_EQ_BAND_ID5][base].h0 * (interval-input) + m_coef_eq[curr_fs][APP_EQ_BAND_ID5][base+1].h0 * input)/interval;

    // update EQ registers LP0,BP1~BP5
    for (i=0; i<APP_EQ_BAND_NUM; i++)
    {
        if (HwpSetCoef_EQ(i,TRUE) < 0)
        {
            RTD_Log(LOGGER_INFO,"[AUDIO][APP] fail to set eq value!!!\n");
            return -1;
        }
    }

    return 0;
}


static INT8 HwpSetCoef_Loudness(UINT8 band_id, UINT8 smooth)
{
    UINT32	A1Address,A2Address,H0Address;
    INT32	A1TargetData,A2TargetData,H0TargetData;
    INT32	tA1,tA2,tH0,steps;
    BOOL	a1shift,a2shift,h0shift;

    if (band_id >= APP_LOUD_BAND_NUM)
        return -1;

    Audio_EnterCriticalSection();

    switch (band_id)
    {
    case APP_LOUD_BAND_ID0:
        A1TargetData = (INT32)m_curr_coef_bass.a1;
        A2TargetData = (INT32)m_curr_coef_bass.a2;	// dummy for LPF
        H0TargetData = (INT32)m_curr_coef_bass.h0;
        A1Address = LR_LOUD_A1_0_0x3770;	//
        A2Address = LR_LOUD_A1_0_0x3770;	// dummy
        H0Address = LR_LOUD_H0_0_0x377c;
        a1shift = TRUE;
        a2shift = TRUE;
        h0shift = TRUE;
        break;
    case APP_LOUD_BAND_ID1:
        A1TargetData = (INT32)m_curr_coef_treble.a1;
        A2TargetData = (INT32)m_curr_coef_treble.a2;
        H0TargetData = (INT32)m_curr_coef_treble.h0;
        A1Address = LR_LOUD_A1_0_0x3770;
        A2Address = LR_LOUD_A2_0x3778;
        H0Address = LR_LOUD_H0_0_0x377c;
        a1shift = FALSE;
        a2shift = TRUE;
        h0shift = FALSE;
        break;
        /*
        case APP_LOUD_BAND_ID2:
        	A1Address = AUDIO_IO_LR_LOUD_A1_1_VADDR;
        	A2Address = AUDIO_IO_LR_LOUD_A2_VADDR;
        	H0Address = AUDIO_IO_LR_LOUD_H0_1_VADDR;
        	a1shift = 0;
        	a2shift = 0;
        	h0shift = 0;
        */
    default:
        return -1;
    }

    tA1 = rtd_inl(A1Address);
    if (a1shift == TRUE)
        tA1 = (tA1 & 0xFFFF0000) >> 16;
    else
        tA1 = (tA1 & 0xFFFF);

    tA2 = rtd_inl(A2Address);
    if (a2shift == TRUE)
        tA2 = (tA2 & 0xFFFF0000) >> 16;
    else
        tA2 = (tA2 & 0xFFFF);

    tH0 = rtd_inl(H0Address);
    if (h0shift == TRUE)
        tH0 = (tH0 & 0xFFFF0000) >> 16;
    else
        tH0 = (tH0 & 0xFFFF);

    if (tA1 & 0x8000)	tA1 |= 0xFFFF0000;	// sign extension
    if (tA2 & 0x8000)	tA2 |= 0xFFFF0000;	// sign extension
    if (tH0 & 0x8000)	tH0 |= 0xFFFF0000;	// sign extension

    steps = 0;
    //RTD_Log(LOGGER_INFO,"HwpSetCoef_Loudness: currA1 =%x,nextA1 =%x\n",(UINT32)tA1,(UINT32)A1TargetData);
    //RTD_Log(LOGGER_INFO,"HwpSetCoef_Loudness: currA2 =%x,nextA2 =%x\n",(UINT32)tA2,(UINT32)A2TargetData);
    //RTD_Log(LOGGER_INFO,"HwpSetCoef_Loudness: currH0 =%x,nextH0 =%x\n",(UINT32)tH0,(UINT32)H0TargetData);

    if (smooth == TRUE)
    {
        while ((tA1 != A1TargetData)||(tA2 != A2TargetData)||(tH0 != H0TargetData))
        {
            steps++;
            // update A1 parameter
            if (A1TargetData > tA1)  		// increasing
            {
                tA1 += LOUD_ADJUST_STEP_SIZE;
                if (tA1 > A1TargetData) tA1 = A1TargetData;
                if (HwpSetReg(A1Address, tA1, a1shift) < 0)	return -1;
            }
            else if (A1TargetData < tA1) 	// decreasing
            {
                tA1 -= LOUD_ADJUST_STEP_SIZE;
                if (A1TargetData > tA1) tA1 = A1TargetData;
                if (HwpSetReg(A1Address, tA1, a1shift) < 0)	return -1;
            }

            // update A2 parameter
            if (A2TargetData > tA2)  		// increasing
            {
                tA2 += LOUD_ADJUST_STEP_SIZE;
                if (tA2 > A2TargetData) tA2 = A2TargetData;
                if (HwpSetReg(A2Address, tA2, a2shift) < 0)	return -1;
            }
            else if (A2TargetData < tA2) 	// decreasing
            {
                tA2 -= LOUD_ADJUST_STEP_SIZE;
                if (A2TargetData > tA2) tA2 = A2TargetData;
                if (HwpSetReg(A2Address, tA2, a2shift) < 0)	return -1;
            }

            // update H0 parameter
            if (H0TargetData > tH0)  		// increasing
            {
                tH0 += LOUD_ADJUST_STEP_SIZE;
                if (tH0 > H0TargetData) tH0 = H0TargetData;
                if (HwpSetReg(H0Address, tH0, h0shift) < 0)	return -1;
            }
            else if (H0TargetData < tH0) 	// decreasing
            {
                tH0 -= LOUD_ADJUST_STEP_SIZE;
                if (H0TargetData > tH0) tH0 = H0TargetData;
                if (HwpSetReg(H0Address, tH0, h0shift) < 0)	return -1;
            }
        }
    }
    else
    {
        if (HwpSetReg(A1Address, A1TargetData, a1shift) < 0)	return -1;
        if (HwpSetReg(A2Address, A2TargetData, a2shift) < 0)	return -1;
        if (HwpSetReg(H0Address, H0TargetData, h0shift) < 0)	return -1;
    }

    Audio_LeaveCriticalSection();

    //RTD_Log(LOGGER_INFO,"steps = %d\n",(UINT32)steps);

    return 0;
}

static INT8 HwpSetCoef_EQ(UINT8 band_id, UINT8 smooth)
{
    UINT32	A1Address,A2Address,H0Address;
    INT32	A1TargetData,A2TargetData,H0TargetData;
    INT32	tA1,tA2,tH0,steps;
    BOOL	a1shift,a2shift,h0shift;

    if (band_id >= APP_EQ_BAND_NUM)
        return -1;

    Audio_EnterCriticalSection();

    A1TargetData = (int)m_curr_coef_eq[band_id].a1;
    A2TargetData = (int)m_curr_coef_eq[band_id].a2;	// dummy for LPF
    H0TargetData = (int)m_curr_coef_eq[band_id].h0;

    switch (band_id)
    {
    case APP_EQ_BAND_ID0:
        A1Address = EQ_A1_0_0x3740;	//
        A2Address = EQ_A1_0_0x3740;	// dummy
        H0Address = EQ_H0_1_0x3760;
        a1shift = TRUE;
        a2shift = TRUE;
        h0shift = TRUE;
        break;
    case APP_EQ_BAND_ID1:
        A1Address = EQ_A1_0_0x3740;
        A2Address = EQ_A2_1_0x3750;
        H0Address = EQ_H0_1_0x3760;
        a1shift = FALSE;
        a2shift = TRUE;
        h0shift = FALSE;
        break;
    case APP_EQ_BAND_ID2:
        A1Address = EQ_A1_1_0x3744;
        A2Address = EQ_A2_1_0x3750;
        H0Address = EQ_H0_2_0x3764;
        a1shift = TRUE;
        a2shift = FALSE;
        h0shift = TRUE;
        break;
    case APP_EQ_BAND_ID3:
        A1Address = EQ_A1_1_0x3744;
        A2Address = EQ_A2_2_0x3754;
        H0Address = EQ_H0_2_0x3764;
        a1shift = FALSE;
        a2shift = TRUE;
        h0shift = FALSE;
        break;
    case APP_EQ_BAND_ID4:
        A1Address = EQ_A1_2_0x3748;
        A2Address = EQ_A2_2_0x3754;
        H0Address = EQ_H0_3_0x3768;
        a1shift = TRUE;
        a2shift = FALSE;
        h0shift = TRUE;
        break;
    case APP_EQ_BAND_ID5:
        A1Address = EQ_A1_2_0x3748;
        A2Address = EQ_A2_3_0x3758;
        H0Address = EQ_H0_3_0x3768;
        a1shift = FALSE;
        a2shift = FALSE;
        h0shift = FALSE;
        break;
    default:
        return -1;
    }

    tA1 = rtd_inl(A1Address);
    if (a1shift == TRUE)
        tA1 = (tA1 & 0xFFFF0000) >> 16;
    else
        tA1 = (tA1 & 0xFFFF);

    tA2 = rtd_inl(A2Address);
    if (a2shift == TRUE)
        tA2 = (tA2 & 0xFFFF0000) >> 16;
    else
        tA2 = (tA2 & 0xFFFF);

    tH0 = rtd_inl(H0Address);
    if (h0shift == TRUE)
        tH0 = (tH0 & 0xFFFF0000) >> 16;
    else
        tH0 = (tH0 & 0xFFFF);

    if (tA1 & 0x8000)	tA1 |= 0xFFFF0000;	// sign extension
    if (tA2 & 0x8000)	tA2 |= 0xFFFF0000;	// sign extension
    if (tH0 & 0x8000)	tH0 |= 0xFFFF0000;	// sign extension

    steps = 0;
    //RTD_Log(LOGGER_INFO,"HwpSetCoef_EQ: currA1 =%x,nextA1 =%x\n",(UINT32)tA1,(UINT32)A1TargetData);
    //RTD_Log(LOGGER_INFO,"HwpSetCoef_EQ: currA2 =%x,nextA2 =%x\n",(UINT32)tA2,(UINT32)A2TargetData);
    //RTD_Log(LOGGER_INFO,"HwpSetCoef_EQ: currH0 =%x,nextH0 =%x\n",(UINT32)tH0,(UINT32)H0TargetData);

    if (smooth == TRUE)
    {
        while ((tA1 != A1TargetData)||(tA2 != A2TargetData)||(tH0 != H0TargetData))
        {
            steps++;
            // update A1 parameter
            if (A1TargetData > tA1)  		// increasing
            {
                tA1 += EQ_ADJUST_STEP_SIZE;
                if (tA1 > A1TargetData) tA1 = A1TargetData;
                if (HwpSetReg(A1Address, tA1, a1shift) < 0)	return -1;
            }
            else if (A1TargetData < tA1) 	// decreasing
            {
                tA1 -= EQ_ADJUST_STEP_SIZE;
                if (A1TargetData > tA1) tA1 = A1TargetData;
                if (HwpSetReg(A1Address, tA1, a1shift) < 0)	return -1;
            }

            // update A2 parameter
            if (A2TargetData > tA2)  		// increasing
            {
                tA2 += EQ_ADJUST_STEP_SIZE;
                if (tA2 > A2TargetData) tA2 = A2TargetData;
                if (HwpSetReg(A2Address, tA2, a2shift) < 0)	return -1;
            }
            else if (A2TargetData < tA2) 	// decreasing
            {
                tA2 -= EQ_ADJUST_STEP_SIZE;
                if (A2TargetData > tA2) tA2 = A2TargetData;
                if (HwpSetReg(A2Address, tA2, a2shift) < 0)	return -1;
            }

            // update H0 parameter
            if (H0TargetData > tH0)  		// increasing
            {
                tH0 += EQ_ADJUST_STEP_SIZE;
                if (tH0 > H0TargetData) tH0 = H0TargetData;
                if (HwpSetReg(H0Address, tH0, h0shift) < 0)	return -1;
            }
            else if (H0TargetData < tH0) 	// decreasing
            {
                tH0 -= EQ_ADJUST_STEP_SIZE;
                if (H0TargetData > tH0) tH0 = H0TargetData;
                if (HwpSetReg(H0Address, tH0, h0shift) < 0)	return -1;
            }
        }
    }
    else
    {
        if (HwpSetReg(A1Address, A1TargetData, a1shift) < 0)	return -1;
        if (HwpSetReg(A2Address, A2TargetData, a2shift) < 0)	return -1;
        if (HwpSetReg(H0Address, H0TargetData, h0shift) < 0)	return -1;
    }

    Audio_LeaveCriticalSection();

    //RTD_Log(LOGGER_INFO,"steps = %d\n",(INT32)steps);

    return 0;

}

static INT8 HwpSetReg(UINT32 address, INT32 value, BOOL shift)
{
    UINT32	oridata,read_back,i;

    oridata = rtd_inl(address);

    if (shift == TRUE)
    {
        value = (oridata & 0xFFFF) | ((value &0xFFFF) << 16);
    }
    else
    {
        value = (oridata & 0xFFFF0000) | (value &0xFFFF) ;
    }
    rtd_outl(address, value);

    // double buffer issue
    i=1000;
    do
    {
        i--;
        read_back = rtd_inl(address);
        if (read_back == 0xdeaddead)
        {
            return -1;
        }
    }
    while ((read_back!=value)&&(i!=0));

    return 0;

}

#endif // CONFIG_ENABLE_AUDIO

