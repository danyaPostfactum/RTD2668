/*=============================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2007 *
 * All rights reserved.                                       *
 *============================================================*/
/*======================= Description ========================*/
/**
 * @file  audio_if.c
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
#include "audio/audio_api.h"
#include "audio_host.h"
#include "audio_tv.h"
#include "audio_if.h"
#include "audio_hwp_lv.h"


/*======================= Type Defines ========================*/

/*===================== Global Variable =======================*/

/*====================== API Routines =========================*/


/*==================== Driver Routines ========================*/
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
void Audio_AioInit_Step1()
{
#ifdef CONFIG_ENABLE_AUDIO
    rtd_outl(ADDA_LOOPBACK_0x3900, 0xc0004440);	// [31:30] asynchronous FIFO enable
    rtd_outl(ANALOG_DEPOP2_0x3938, 0x02a2302a);	// [6:5] mode 2 : DEPOP2 In --> Out + Switch
    rtd_outl(ANALOG_DEPOP2_0x3938, 0x02a2303a);	// [4] SW controls VCM ready
    rtd_outl(ANALOG_DEPOP2_0x3938, 0x02a23032);	// [3] VCM ready false
    rtd_outl(ANALOG_DEPOP2_0x3938, 0x02a27032);	// [14] IO output power on
    rtd_outl(ANALOG_DEPOP2_0x3938, 0x02a270b2);	// [7] analog non-overlap clock on
    rtd_outl(ANALOG_DEPOP1_0x3934, 0x0000000d);	// [3:0] depop clock enable and select : 16kHz
    rtd_outl(ANALOG_DEPOP2_0x3938, 0x02a270b2);	// [14:12] select depop Cap. : 24pF+24pF
    rtd_outl(ANALOG_POWER_0x3918, 0x003FF300);	// turn-on analog power except to pin-shared and set DAC reference from Int.
    rtd_outl(ANALOG_AIO_MASTER_HPOUT_LINEOUT_0x3910, 0x000FFFFF);	// hp AMP power on
    rtd_outl(ANALOG_DEPOP2_0x3938, 0xa2a270b2);	// [31] [29] depop core op power on
    Audio_TimeDelay_ms(1);		// at least delay 1ms
    rtd_outl(ANALOG_DEPOP2_0x3938, 0xe2a270b2);	// [30] depop core ckt power on

#if 0 // move to init_io_step2() for speed-up booting flow.
    Audio_TimeDelay_ms(200);	// at least delay 200ms
    rtd_outl(ANALOG_DEPOP2_0x3938, 0xe2a270ba);	// [3] VCM ready true
    Audio_TimeDelay_ms(400);	// at least delay 400ms
    rtd_outl(ANALOG_DEPOP2_0x3938, 0xe2a276be);	// [10] [9] [2] depop mode --> normal mode
    rtd_outl(ANALOG_DEPOP1_0x3934, 0x00000005);	// [3] stop depop clock
    rtd_outl(ANALOG_DEPOP2_0x3938, 0x02a276be);	// [31:29] stop depop ckt
#endif //

#endif // CONFIG_ENABLE_AUDIO
}
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
void Audio_AioInit_Step2()
{
#ifdef CONFIG_ENABLE_AUDIO
    Audio_TimeDelay_ms(200);	// at least delay 200ms
    rtd_outl(ANALOG_DEPOP2_0x3938, 0xe2a270ba);	// [3] VCM ready true
    Audio_TimeDelay_ms(400);	// at least delay 400ms
    rtd_outl(ANALOG_DEPOP2_0x3938, 0xe2a276be);	// [10] [9] [2] depop mode --> normal mode
    rtd_outl(ANALOG_DEPOP1_0x3934, 0x00000005);	// [3] stop depop clock
    rtd_outl(ANALOG_DEPOP2_0x3938, 0x02a276be);	// [31:29] stop depop ckt
    rtd_outl(ANALOG_DEPOP_DUMMY_0x393c, 0x007A0010);

#if 0 // TODO: add other io init flow here
    //Modified the flow for de-pop by yukichen@20071008
    drv_audio_config_hpout_inout(AUD_IS_OUTPUT);	// aio1 as HP_out
    drv_audio_config_lineout_inout(AUD_IS_OUTPUT);	// aio2 as Line_out

    drv_audio_delaytime_new(400);

#endif //

#ifdef CONFIG_SPARROW_VER_A
    // H/W bug in IC_VERSION_A
    Audio_AioSetOptChannel_Headphone(AIO_OPT_CHANNEL_L_TO_R);
    Audio_AioSetOptChannel_Lineout(AIO_OPT_CHANNEL_L_TO_R);
    Audio_AioSetOptChannel_AIO1(AIO_OPT_CHANNEL_L_TO_R);
    Audio_AioSetOptChannel_AIO2(AIO_OPT_CHANNEL_L_TO_R);
#else
    Audio_AioSetOptChannel_Headphone(AIO_OPT_CHANNEL_STEREO);
    Audio_AioSetOptChannel_Lineout(AIO_OPT_CHANNEL_STEREO);
    Audio_AioSetOptChannel_AIO1(AIO_OPT_CHANNEL_STEREO);
    Audio_AioSetOptChannel_AIO2(AIO_OPT_CHANNEL_STEREO);
#endif // CONFIG_SPARROW_VER_X

    // TODO: pop-noise on HP still happened

    //unmute hp_out&line_out
    Audio_AioSetHeadphoneAmp(_ENABLE);
    Audio_AioSetOptSrc_Headphone(AIO_HP_PB_DAC);	// this should move to pcb layer in the future
    Audio_AioMute_Headphone(_DISABLE);

    Audio_AioMute_Lineout(_DISABLE);


#endif // CONFIG_ENABLE_AUDIO
}

/**
 * Select BBADC input source.
 * This function is usually used with YPbPr/VGA/CVBS/S-Video sources
 *
 * @param <src>	{ AIO_BBADC_AIO1 | BBADC_AIO2 | ..., refer to audio_aio.h }
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetIptSrc_BBADC(UINT16 src)
{
#ifndef CONFIG_ENABLE_AUDIO
    src = src;
#else
    analog_frontend_mute_RBUS	BBADC_MUTE_CTRL_REG;
    analog_in_out_sel_RBUS		AIO_SEL_REG;
    UINT32						ana_pow_flag;

//	RTD_Log(LOGGER_INFO,"[AUDIO][AIO] BBADC to %x\n",(UINT32)src);

    ana_pow_flag = 0;

    BBADC_MUTE_CTRL_REG.regValue = 0x03FFFF;		// mute all input of BBADC
    AIO_SEL_REG.regValue = rtd_inl(ANALOG_IN_OUT_SEL_0x3904);

    if (src)
    {
        BBADC_MUTE_CTRL_REG.bitField.mute_avol1_l = FALSE;
        BBADC_MUTE_CTRL_REG.bitField.mute_avol1_r = FALSE;

        if (src & AIO_BBADC_MONOIN)
        {
            BBADC_MUTE_CTRL_REG.bitField.mute_monoin_l_adc = FALSE;
            BBADC_MUTE_CTRL_REG.bitField.mute_monoin_r_adc = FALSE;
            ana_pow_flag |= (AIO_POW_MONO_IN | AIO_POW_MONO_OUT | AIO_POW_AIO_MONO);
            // TODO: use aio_flag to enable or disable in the future
            Audio_AioSetIptSrc_MonoIn(AIO_MONOIN_STEREO);	// testing
        }
        if (src & AIO_BBADC_AIO1)
        {
            BBADC_MUTE_CTRL_REG.bitField.mute_l_aio1_adc = FALSE;
            BBADC_MUTE_CTRL_REG.bitField.mute_r_aio1_adc = FALSE;
            ana_pow_flag |= AIO_POW_AIO1;
            AIO_SEL_REG.bitField.reg_bb_aio1_io = 0;
            rtd_outl(ANALOG_IN_OUT_SEL_0x3904, AIO_SEL_REG.regValue);
        }
        if (src & AIO_BBADC_AIO2)
        {
            BBADC_MUTE_CTRL_REG.bitField.mute_l_aio2_adc = FALSE;
            BBADC_MUTE_CTRL_REG.bitField.mute_r_aio2_adc = FALSE;
            ana_pow_flag |= AIO_POW_AIO2;
            AIO_SEL_REG.bitField.reg_bb_aio2_io = 0;
            rtd_outl(ANALOG_IN_OUT_SEL_0x3904, AIO_SEL_REG.regValue);
        }
        if (src & AIO_BBADC_AIN1)
        {
            BBADC_MUTE_CTRL_REG.bitField.mute_l_ain1_adc = FALSE;
            BBADC_MUTE_CTRL_REG.bitField.mute_r_ain1_adc = FALSE;
            ana_pow_flag |= AIO_POW_AIN1;
        }
        if (src & AIO_BBADC_AIN2)
        {
            BBADC_MUTE_CTRL_REG.bitField.mute_l_ain2_adc = FALSE;
            BBADC_MUTE_CTRL_REG.bitField.mute_r_ain2_adc = FALSE;
            ana_pow_flag |= AIO_POW_AIN2;
        }
        if (src & AIO_BBADC_AIN3)
        {
            BBADC_MUTE_CTRL_REG.bitField.mute_l_ain3_adc = FALSE;
            BBADC_MUTE_CTRL_REG.bitField.mute_r_ain3_adc = FALSE;
            ana_pow_flag |= AIO_POW_AIN3;
        }
        if (src & AIO_BBADC_AIN4)
        {
            BBADC_MUTE_CTRL_REG.bitField.mute_l_ain4_adc = FALSE;
            BBADC_MUTE_CTRL_REG.bitField.mute_r_ain4_adc = FALSE;
            ana_pow_flag |= AIO_POW_AIN4;
        }
        if (src & AIO_BBADC_AIN5_2)
        {
            AIO_SEL_REG.bitField.reg_bb_sw_in = (src>>8);
            rtd_outl(ANALOG_IN_OUT_SEL_0x3904, AIO_SEL_REG.regValue);

            BBADC_MUTE_CTRL_REG.bitField.mute_l_ain5_adc = FALSE;
            BBADC_MUTE_CTRL_REG.bitField.mute_r_ain5_adc = FALSE;
            ana_pow_flag |= AIO_POW_AIN5;
        }
    }

    rtd_outl(ANALOG_FRONTEND_MUTE_0x3908, BBADC_MUTE_CTRL_REG.regValue);

    Audio_AioSetAnalogPower(_ENABLE, ana_pow_flag);

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Select I2S_IN_Primary input source.
 * This function is usually used with HDMI source
 *
 * @param <src>	{ AIO_I2SI_IN / AIO_I2SI_HDMI / AIO_I2S_LOOPBCK / AIO_I2S_DISABLE }
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetIptSrc_I2SPri(UINT8 src)
{
#ifndef CONFIG_ENABLE_AUDIO
    src = src;
#else
    i2s_in_primary_RBUS I2SI_CTRL_REG;

    I2SI_CTRL_REG.regValue = rtd_inl(I2S_IN_PRIMARY_0x3100);

    I2SI_CTRL_REG.bitField.source_sel = src;

    rtd_outl(I2S_IN_PRIMARY_0x3100, I2SI_CTRL_REG.regValue);
#endif // !CONFIG_ENABLE_AUDIO
}
#if 0	// marked by weihao because of uncalled
/**
 * Select I2S_IN_Secondary input source.
 * This function is usually used with HDMI source
 *
 * @param <src>	{ AIO_I2SI_IN / AIO_I2SI_HDMI / AIO_I2S_LOOPBCK / AIO_I2S_DISABLE }
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetIptSrc_I2SSec(UINT8 src)
{
#ifndef CONFIG_ENABLE_AUDIO
    src = src;
#else
    i2s_in_secondary_RBUS I2SI_CTRL_REG;

    I2SI_CTRL_REG.regValue = rtd_inl(I2S_IN_SECONDARY_0x3104);

    I2SI_CTRL_REG.bitField.source_sel = src;

    rtd_outl(I2S_IN_SECONDARY_0x3104, I2SI_CTRL_REG.regValue);
#endif // !CONFIG_ENABLE_AUDIO
}
#endif


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
void Audio_AioSetIptSrc_MonoIn(UINT8 src)
{
#ifndef CONFIG_ENABLE_AUDIO
    src = src;
#else
    analog_in_out_sel_RBUS AIO_SEL_REG;

    AIO_SEL_REG.regValue = rtd_inl(ANALOG_IN_OUT_SEL_0x3904);

#if 1
    AIO_SEL_REG.bitField.reg_bb_mono_stereo = (src>>1)&0x1;
    AIO_SEL_REG.bitField.reg_bb_mono_in_sel = src&0x1;
#else
    switch (src)
    {
    case AIO_MONOIN_IN:
        AIO_SEL_REG.reg_bb_mono_stereo = 0;
        AIO_SEL_REG.reg_bb_mono_in_sel = 0;
        break;
    case AIO_MONOIN_OUT_T:
        AIO_SEL_REG.reg_bb_mono_stereo = 0;
        AIO_SEL_REG.reg_bb_mono_in_sel = 1;
        break;
    case AIO_MONOIN_STEREO:
    default:
        AIO_SEL_REG.reg_bb_mono_stereo = 1;
        break;
    }
#endif

    rtd_outl(ANALOG_IN_OUT_SEL_0x3904, AIO_SEL_REG.regValue);

    Audio_AioSetAnalogPower(_ENABLE, AIO_POW_MONO_IN | AIO_POW_MONO_OUT | AIO_POW_AIO_MONO);

#endif // !CONFIG_ENABLE_AUDIO
}

#if 0	// marked by weihao because of uncalled
/**
 * Select Wire1 input source.
 *
 * @param <src>	{ AIO_WIREIN_AIN1 | AIO_WIREIN_AIN2 | AIO_WIREIN_MONOIN | AIO_WIREIN_BBADC }
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetIptSrc_Wire1(UINT8 mix_src)
{
#ifndef CONFIG_ENABLE_AUDIO
    mix_src = mix_src;
#else
    analog_mixer_mute_RBUS	MIXIN_MUTE_CTRL_REG;

    MIXIN_MUTE_CTRL_REG.regValue = rtd_inl(ANALOG_MIXER_MUTE_0x390c);

    MIXIN_MUTE_CTRL_REG.regValue |= 0x0FF00;	// mute all WireIn1 inputs

    if (mix_src & AIO_WIREIN_MONOIN)
    {
        MIXIN_MUTE_CTRL_REG.bitField.mute_l_monoin_wireio1 = FALSE;
        MIXIN_MUTE_CTRL_REG.bitField.mute_r_monoin_wireio1 = FALSE;
    }
    if (mix_src & AIO_WIREIN_AIN1)
    {
        MIXIN_MUTE_CTRL_REG.bitField.mute_l_ain1_wireio1 = FALSE;
        MIXIN_MUTE_CTRL_REG.bitField.mute_r_ain1_wireio1 = FALSE;
    }
    if (mix_src & AIO_WIREIN_AIN2)
    {
        MIXIN_MUTE_CTRL_REG.bitField.mute_l_ain2_wireio1 = FALSE;
        MIXIN_MUTE_CTRL_REG.bitField.mute_r_ain2_wireio1 = FALSE;
    }
    if (mix_src & AIO_WIREIN_BBADC)
    {
        MIXIN_MUTE_CTRL_REG.bitField.mute_l_adc_in_wireio1 = FALSE;
        MIXIN_MUTE_CTRL_REG.bitField.mute_r_adc_in_wireio1 = FALSE;
    }

    rtd_outl(ANALOG_MIXER_MUTE_0x390c, MIXIN_MUTE_CTRL_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Select Wire2 input source.
 *
 * @param <src>	{ AIO_WIREIN_AIN1 | AIO_WIREIN_AIN2 | AIO_WIREIN_MONOIN | AIO_WIREIN_BBADC }
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetIptSrc_Wire2(UINT8 mix_src)
{
#ifndef CONFIG_ENABLE_AUDIO
    mix_src = mix_src;
#else
    analog_mixer_mute_RBUS	MIXIN_MUTE_CTRL_REG;

    MIXIN_MUTE_CTRL_REG.regValue = rtd_inl(ANALOG_MIXER_MUTE_0x390c);

    MIXIN_MUTE_CTRL_REG.regValue |= 0x000FF;	// mute all WireIn2 inputs

    if (mix_src & AIO_WIREIN_MONOIN)
    {
        MIXIN_MUTE_CTRL_REG.bitField.mute_l_monoin_wireio2 = FALSE;
        MIXIN_MUTE_CTRL_REG.bitField.mute_r_monoin_wireio2 = FALSE;
    }
    if (mix_src & AIO_WIREIN_AIN1)
    {
        MIXIN_MUTE_CTRL_REG.bitField.mute_l_ain1_wireio2 = FALSE;
        MIXIN_MUTE_CTRL_REG.bitField.mute_r_ain1_wireio2 = FALSE;
    }
    if (mix_src & AIO_WIREIN_AIN2)
    {
        MIXIN_MUTE_CTRL_REG.bitField.mute_l_ain2_wireio2 = FALSE;
        MIXIN_MUTE_CTRL_REG.bitField.mute_r_ain2_wireio2 = FALSE;
    }
    if (mix_src & AIO_WIREIN_BBADC)
    {
        MIXIN_MUTE_CTRL_REG.bitField.mute_l_adc_in_wireio2 = FALSE;
        MIXIN_MUTE_CTRL_REG.bitField.mute_r_adc_in_wireio2 = FALSE;
    }

    rtd_outl(ANALOG_MIXER_MUTE_0x390c, MIXIN_MUTE_CTRL_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Setup BBADC input source.
 *
 * @param <p_cfg_bbadc>	{ pointer of BBADC control block }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_AioCfgIptSrc_BBADC(AIO_BBADC_CFG *p_cfg_bbadc)
{
#ifndef CONFIG_ENABLE_AUDIO
    p_cfg_bbadc = p_cfg_bbadc;
#else
    adc_volume_gain_control_RBUS	BBADC_VOL_CTRL_REG;

    // TODO: make sure the bit-order
    BBADC_VOL_CTRL_REG.regValue = (UINT32)&p_cfg_bbadc;

    rtd_outl(ADC_VOLUME_GAIN_CONTROL_0x3914, BBADC_VOL_CTRL_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Setup I2S_IN_Primary input source.
 *
 * @param <p_cfg_i2s>	{ pointer of I2S_IN control block }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_AioCfgIptSrc_I2SPri(AIO_I2SI_CFG *p_cfg_i2s)
{
#ifndef CONFIG_ENABLE_AUDIO
    p_cfg_i2s = p_cfg_i2s;
#else
    i2s_in_primary_RBUS I2SI_CTRL_REG;

    I2SI_CTRL_REG.regValue = rtd_inl(I2S_IN_PRIMARY_0x3100);

    if (p_cfg_i2s->update_src_only == FALSE)
    {
        I2SI_CTRL_REG.bitField.left_right = p_cfg_i2s->sync_type;
        I2SI_CTRL_REG.bitField.mode_sel = p_cfg_i2s->mode_sel;
    }
    I2SI_CTRL_REG.bitField.source_sel = p_cfg_i2s->src_sel;

    rtd_outl(I2S_IN_PRIMARY_0x3100, I2SI_CTRL_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Setup I2S_IN_Secondary input source.
 *
 * @param <p_cfg_i2s>	{ pointer of I2S_IN control block }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_AioCfgIptSrc_I2SSec(AIO_I2SI_CFG *p_cfg_i2s)
{
#ifndef CONFIG_ENABLE_AUDIO
    p_cfg_i2s = p_cfg_i2s;
#else
    i2s_in_secondary_RBUS I2SI_CTRL_REG;

    I2SI_CTRL_REG.regValue = rtd_inl(I2S_IN_SECONDARY_0x3104);

    if (p_cfg_i2s->update_src_only == FALSE)
    {
        I2SI_CTRL_REG.bitField.left_right = p_cfg_i2s->sync_type;
        I2SI_CTRL_REG.bitField.mode_sel = p_cfg_i2s->mode_sel;
    }
    I2SI_CTRL_REG.bitField.source_sel = p_cfg_i2s->src_sel;

    rtd_outl(I2S_IN_SECONDARY_0x3104, I2SI_CTRL_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Setup I2S_OUT source.
 *
 * @param <p_cfg_i2s>	{ pointer of I2S_OUT control block }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_AioCfgOptSrc_I2S(AIO_I2SO_CFG *p_cfg_i2s)
{
#ifndef CONFIG_ENABLE_AUDIO
    p_cfg_i2s = p_cfg_i2s;
#else
    i2s_out_RBUS	I2SO_CTRL_REG;

    I2SO_CTRL_REG.regValue = rtd_inl(I2S_OUT_0x3150);

    if (p_cfg_i2s->update_src_only == FALSE)
    {
        I2SO_CTRL_REG.bitField.mclk_sel = p_cfg_i2s->mclk_sel;
    }
    I2SO_CTRL_REG.bitField.output_sel = p_cfg_i2s->src_sel;

    rtd_outl(I2S_OUT_0x3150, I2SO_CTRL_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Setup output FIFO.
 *
 * @param <p_cfg_fifo>	{ pointer of AIO_FIFO_CFG control block }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_AioCfgOptFIFO(AIO_FIFO_CFG *p_cfg_fifo)
{
#ifndef CONFIG_ENABLE_AUDIO
    p_cfg_fifo = p_cfg_fifo;
#else
    hw_backplay_transfer_RBUS	FIFO_CTRL_REG;

    FIFO_CTRL_REG.regValue = 0;// rtd_inl(HW_BACKPLAY_TRANSFER_0x37bc);
    FIFO_CTRL_REG.bitField.hw_i2s_out_0_sel = p_cfg_fifo->i2s_out_0_sel;
    FIFO_CTRL_REG.bitField.hw_i2s_out_1_sel = p_cfg_fifo->i2s_out_1_sel;
    FIFO_CTRL_REG.bitField.hw_i2s_out_2_sel = p_cfg_fifo->i2s_out_2_sel;
    FIFO_CTRL_REG.bitField.hw_i2s_out_3_sel = p_cfg_fifo->i2s_out_3_sel;
    FIFO_CTRL_REG.bitField.hw_spdif_out_sel = p_cfg_fifo->spdif_out_sel;
    FIFO_CTRL_REG.bitField.hw_dac_out_sel   = p_cfg_fifo->dac_out_sel;
    FIFO_CTRL_REG.bitField.hw_pcm_out_sel   = p_cfg_fifo->pcm_out_sel;
    FIFO_CTRL_REG.bitField.hw_mixing_sel    = p_cfg_fifo->mixing_sel;
    FIFO_CTRL_REG.bitField.w_ch_12_factor   = p_cfg_fifo->mixing_fac_12;
    FIFO_CTRL_REG.bitField.w_ch_mix_factor  = p_cfg_fifo->mixing_fac_other;
    rtd_outl(HW_BACKPLAY_TRANSFER_0x37bc, FIFO_CTRL_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}


/**
 * Select I2S_OUT source.
 *
 * @param <src>	{ AIO_I2SO_FIFO / AIO_I2SO_IN_SLAVE / AIO_I2SO_IN_MASTER / AIO_I2SO_IN_HDMI / AIO_I2SO_DISABLE }
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetOptSrc_I2S(UINT8 src)
{
#ifndef CONFIG_ENABLE_AUDIO
    src = src;
#else
    i2s_out_RBUS	I2SO_CTRL_REG;

    I2SO_CTRL_REG.regValue = rtd_inl(I2S_OUT_0x3150);

    I2SO_CTRL_REG.bitField.output_sel = src;

    rtd_outl(I2S_OUT_0x3150, I2SO_CTRL_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}

#endif
/**
 * Setup SPDIF_IN input source.
 *
 * @param <p_cfg_spdif>	{ pointer of SPDIF_IN control block }
 * @return				{ void }
 * @ingroup lib_audio
 */
 /*
void Audio_AioCfgIptSrc_SPDIF(AIO_SPDIFI_CFG *p_cfg_spdif)
{
#ifndef CONFIG_ENABLE_AUDIO
    p_cfg_spdif = p_cfg_spdif;
#else
    spdif_in_RBUS SPDIFI_CTRL_REG;

    SPDIFI_CTRL_REG.regValue = rtd_inl(SPDIF_IN_0x3130);

    if (p_cfg_spdif->update_src_only == FALSE)
    {
        SPDIFI_CTRL_REG.bitField.req_after_lock = p_cfg_spdif->req_after_lock;
    }
    SPDIFI_CTRL_REG.bitField.source_sel = p_cfg_spdif->src_sel;

    rtd_outl(SPDIF_IN_0x3130,SPDIFI_CTRL_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}
*/
/**
 * Select SPDIF_IN input source.
 * This function is usually used with HDMI source
 *
 * @param <src>	{ AIO_SPDIFI_IN / AIO_SPDIFI_HDMI / AIO_SPDIFI_LOOPBACK / AIO_SPDIFI_DISABLE }
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetIptSrc_SPDIF(UINT8 src)
{
#ifndef CONFIG_ENABLE_AUDIO
    src = src;
#else
    spdif_in_RBUS SPDIFI_CTRL_REG;

    SPDIFI_CTRL_REG.regValue = rtd_inl(SPDIF_IN_0x3130);

    SPDIFI_CTRL_REG.bitField.source_sel = src;

    rtd_outl(SPDIF_IN_0x3130,SPDIFI_CTRL_REG.regValue);
#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Setup SPDIF_OUT source.
 *
 * @param <p_cfg_spdif>	{ pointer of SPDIF_OUT control block }
 * @return				{ void }
 * @ingroup lib_audio
 */
/*void Audio_AioCfgOptSrc_SPDIF(AIO_SPDIFO_CFG *p_cfg_spdif)
{
#ifndef CONFIG_ENABLE_AUDIO
    p_cfg_spdif = p_cfg_spdif;
#else
    spdif_out_RBUS		SPDIFO_CTRL_REG;
    spdif_out_cs_RBUS	SPDIFO_CS_REG;

    SPDIFO_CTRL_REG.regValue = rtd_inl(SPDIF_OUT_0x3170);

    if (p_cfg_spdif->update_src_only == FALSE)
    {
        SPDIFO_CTRL_REG.bitField.validity_ctrl = p_cfg_spdif->valid_ctrl;
    }

    SPDIFO_CS_REG.regValue = rtd_inl(SPDIF_OUT_CS_0x3174);

#if 0 //defined(ENABLE_AUDIO_AIO_SPDIF_NONPCM)
    SPDIFO_CTRL_REG.output_sel = AIO_SPDIFO_DISABLE;
    rtd_outl(SPDIF_OUT, SPDIFO_CTRL_REG.regValue);

    ioctl(audio_fd, AIO_CMD_SPDIFO_DT_CHG, p_cfg_spdif->data_type);

    usleep(50*1000);	// temp patch, delay 50ms to make sure that ROS already apply the setting

    SPDIFO_CTRL_REG.output_sel = p_cfg_spdif->src_sel;
    rtd_outl(SPDIF_OUT, SPDIFO_CTRL_REG.regValue);

    SPDIFO_CS_REG.regValue = rtd_inl(SPDIF_OUT_CS);
    SPDIFO_CS_REG.data_type = p_cfg_spdif->data_type;
#else
    SPDIFO_CTRL_REG.bitField.output_sel = p_cfg_spdif->src_sel;
    rtd_outl(SPDIF_OUT_0x3170, SPDIFO_CTRL_REG.regValue);

    SPDIFO_CS_REG.regValue = rtd_inl(SPDIF_OUT_CS_0x3174);
    SPDIFO_CS_REG.bitField.data_type = AIO_DATA_IS_PCM;
#endif // ENABLE_AUDIO_AIO_SPDIF_NONPCM

    rtd_outl(SPDIF_OUT_CS_0x3174, SPDIFO_CS_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}
*/
/**
 * Select SPDIF_OUT source.
 *
 * @param <src>	{ AIO_SPDIFO_FIFO / AIO_SPDIFO_IN / AIO_SPDIFO_HDMI / AIO_SPDIFO_DISABLE }
 * @return		{ void }
 * @ingroup lib_audio
 */
/*void Audio_AioSetOptSrc_SPDIF(UINT8 src)
{
#ifndef CONFIG_ENABLE_AUDIO
    src = src;
#else
    spdif_out_RBUS		SPDIFO_CTRL_REG;

    SPDIFO_CTRL_REG.regValue = rtd_inl(SPDIF_OUT_0x3170);

    SPDIFO_CTRL_REG.bitField.output_sel = src;

    rtd_outl(SPDIF_OUT_0x3170, SPDIFO_CTRL_REG.regValue);
#endif // !CONFIG_ENABLE_AUDIO
}*/


/**
 * Select Headphone output source.
 *
 * @param <src>	{ AIO_HP_SRC_PB_DAC | AIO_HP_SRC_TS_DAC | AIO_HP_SRC_WIRE_1 | AIO_HP_SRC_WIRE_2 }
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetOptSrc_Headphone(UINT8 mix_src)
{
#ifndef CONFIG_ENABLE_AUDIO
    mix_src = mix_src;
#else
    analog_aio_master_hpout_lineout_RBUS HP_LINEOUT_MIX_REG;

    HP_LINEOUT_MIX_REG.regValue = rtd_inl(ANALOG_AIO_MASTER_HPOUT_LINEOUT_0x3910);

    // not all mute
    if (mix_src & AIO_HP_PB_DAC)
    {
        HP_LINEOUT_MIX_REG.bitField.mute_left_dac0_out = FALSE;
        HP_LINEOUT_MIX_REG.bitField.mute_right_dac0_out = FALSE;
    }
    else
    {
        HP_LINEOUT_MIX_REG.bitField.mute_left_dac0_out = TRUE;
        HP_LINEOUT_MIX_REG.bitField.mute_right_dac0_out = TRUE;
    }

    if (mix_src & AIO_HP_WIRE_1)
    {
        HP_LINEOUT_MIX_REG.bitField.mute_left_wire1_out = FALSE;
        HP_LINEOUT_MIX_REG.bitField.mute_right_wire1_out = FALSE;
    }
    else
    {
        HP_LINEOUT_MIX_REG.bitField.mute_left_wire1_out = TRUE;
        HP_LINEOUT_MIX_REG.bitField.mute_right_wire1_out = TRUE;
    }

    if (mix_src & AIO_HP_WIRE_2)
    {
        HP_LINEOUT_MIX_REG.bitField.mute_left_wire2_out = FALSE;
        HP_LINEOUT_MIX_REG.bitField.mute_right_wire2_out = FALSE;
    }
    else
    {
        HP_LINEOUT_MIX_REG.bitField.mute_left_wire2_out = TRUE;
        HP_LINEOUT_MIX_REG.bitField.mute_right_wire2_out = TRUE;
    }

    if (mix_src & AIO_HP_TS_DAC)
    {
        HP_LINEOUT_MIX_REG.bitField.mute_left_dac1_out = FALSE;
        HP_LINEOUT_MIX_REG.bitField.mute_right_dac1_out = FALSE;
    }
    else
    {
        HP_LINEOUT_MIX_REG.bitField.mute_left_dac1_out = TRUE;
        HP_LINEOUT_MIX_REG.bitField.mute_right_dac1_out = TRUE;
    }

    if (mix_src)
    {
        HP_LINEOUT_MIX_REG.bitField.mute_left_amp = FALSE;
        HP_LINEOUT_MIX_REG.bitField.mute_right_amp = FALSE;
    }
    else
    {
        HP_LINEOUT_MIX_REG.bitField.mute_left_amp = TRUE;
        HP_LINEOUT_MIX_REG.bitField.mute_right_amp = TRUE;
    }

    rtd_outl(ANALOG_AIO_MASTER_HPOUT_LINEOUT_0x3910, HP_LINEOUT_MIX_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}

#if 0	// not used
/**
 * Select PB_DAC_Mixer output.
 * HWP-Post-FIFO always output to Mixer and user can add AMIX_1 and/or AMIX_2 to Mixer output.
 *
 * @param <src>	{ AIO_DACMIX_AMIX_1 | AIO_DACMIX_AMIX_2 }
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetOptSrc_DACMixer(UINT8 mix_src)
{
#ifndef CONFIG_ENABLE_AUDIO
    mix_src = mix_src;
#else
    analog_aio_master_hpout_lineout_RBUS HP_LINEOUT_MIX_REG;

    HP_LINEOUT_MIX_REG.regValue = rtd_inl(ANALOG_AIO_MASTER_HPOUT_LINEOUT_0x3910);

    HP_LINEOUT_MIX_REG.regValue &= ~0x0F0000;	// mute AIO1 & AIO2

    if (mix_src & AIO_DACMIX_AMIX_1)
    {
        HP_LINEOUT_MIX_REG.bitField.reg_bb_m_aio1_l = FALSE;
        HP_LINEOUT_MIX_REG.bitField.reg_bb_m_aio1_r = FALSE;
    }

    if (mix_src & AIO_DACMIX_AMIX_2)
    {
        HP_LINEOUT_MIX_REG.bitField.reg_bb_m_aio2_l = FALSE;
        HP_LINEOUT_MIX_REG.bitField.reg_bb_m_aio2_r = FALSE;
    }

    rtd_outl(ANALOG_AIO_MASTER_HPOUT_LINEOUT_0x3910, HP_LINEOUT_MIX_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}
#endif

#ifdef CONFIG_SCART_AUTO_SWITCH
/**
 * Select AIO1 output source.
 *
 * @param <src>	{ AIO_WIREOUT_PB_DAC | AIO_WIREOUT_TS_DAC | AIO_WIREOUT_WIREIN }
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetOptSrc_AIO1(UINT8 mix_src)
{
#ifndef CONFIG_ENABLE_AUDIO
    mix_src = mix_src;
#else
    analog_in_out_sel_RBUS AIO_SEL_REG;

    AIO_SEL_REG.regValue = rtd_inl(ANALOG_IN_OUT_SEL_0x3904);

    AIO_SEL_REG.bitField.reg_bb_aio1_mux |= AIO_WIREOUT_MUTE_ALL;

    if (mix_src & AIO_WIREOUT_PB_DAC)
    {
        AIO_SEL_REG.bitField.reg_bb_aio1_mux &= ~AIO_WIREOUT_PB_DAC;
    }
    if (mix_src & AIO_WIREOUT_TS_DAC)
    {
        AIO_SEL_REG.bitField.reg_bb_aio1_mux &= ~AIO_WIREOUT_TS_DAC;
    }
    if (mix_src & AIO_WIREOUT_WIREIN)
    {
        AIO_SEL_REG.bitField.reg_bb_aio1_mux &= ~AIO_WIREOUT_WIREIN;
    }

    AIO_SEL_REG.bitField.reg_bb_aio1_io = 1;
    rtd_outl(ANALOG_IN_OUT_SEL_0x3904, AIO_SEL_REG.regValue);

    Audio_AioSetAnalogPower(_ENABLE, AIO_POW_AIO1);

#endif // !CONFIG_ENABLE_AUDIO
}
#endif

#ifdef CONFIG_SCART_AUTO_SWITCH
/**
 * Select AIO2 output source.
 *
 * @param <src>	{ AIO_WIREOUT_PB_DAC | AIO_WIREOUT_TS_DAC | AIO_WIREOUT_WIREIN }
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetOptSrc_AIO2(UINT8 mix_src)
{
#ifndef CONFIG_ENABLE_AUDIO
    mix_src = mix_src;
#else
    analog_in_out_sel_RBUS AIO_SEL_REG;

    AIO_SEL_REG.regValue = rtd_inl(ANALOG_IN_OUT_SEL_0x3904);

    AIO_SEL_REG.bitField.reg_bb_aio2_mux |= AIO_WIREOUT_MUTE_ALL;

    if (mix_src & AIO_WIREOUT_PB_DAC)
    {
        AIO_SEL_REG.bitField.reg_bb_aio2_mux &= ~AIO_WIREOUT_PB_DAC;
    }
    if (mix_src & AIO_WIREOUT_TS_DAC)
    {
        AIO_SEL_REG.bitField.reg_bb_aio2_mux &= ~AIO_WIREOUT_TS_DAC;
    }
    if (mix_src & AIO_WIREOUT_WIREIN)
    {
        AIO_SEL_REG.bitField.reg_bb_aio2_mux &= ~AIO_WIREOUT_WIREIN;
    }

    AIO_SEL_REG.bitField.reg_bb_aio2_io = 1;
    rtd_outl(ANALOG_IN_OUT_SEL_0x3904, AIO_SEL_REG.regValue);

    Audio_AioSetAnalogPower(_ENABLE, AIO_POW_AIO2);

#endif // !CONFIG_ENABLE_AUDIO
}
#endif

/**
 * Select Headphone output channel configuration.
 *
 * @param <sel>	{ AIO_OPT_CHANNEL_STEREO | AIO_OPT_CHANNEL_L_TO_R | AIO_OPT_CHANNEL_R_TO_L }
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetOptChannel_Headphone(UINT8 sel)
{
#ifndef CONFIG_ENABLE_AUDIO
    sel = sel;
#else
    analog_in_out_sel_RBUS AIO_SEL_REG;

    AIO_SEL_REG.regValue = rtd_inl(ANALOG_IN_OUT_SEL_0x3904);

    AIO_SEL_REG.bitField.hpout_source = sel;

    rtd_outl(ANALOG_IN_OUT_SEL_0x3904, AIO_SEL_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Select Lineout output channel configuration.
 *
 * @param <sel>	{ AIO_OPT_CHANNEL_STEREO | AIO_OPT_CHANNEL_L_TO_R | AIO_OPT_CHANNEL_R_TO_L }
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetOptChannel_Lineout(UINT8 sel)
{
#ifndef CONFIG_ENABLE_AUDIO
    sel = sel;
#else
    analog_in_out_sel_RBUS AIO_SEL_REG;

    AIO_SEL_REG.regValue = rtd_inl(ANALOG_IN_OUT_SEL_0x3904);

    AIO_SEL_REG.bitField.lineout_source = sel;

    rtd_outl(ANALOG_IN_OUT_SEL_0x3904, AIO_SEL_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Select AIO1 output channel configuration.
 *
 * @param <sel>	{ AIO_OPT_CHANNEL_STEREO | AIO_OPT_CHANNEL_L_TO_R | AIO_OPT_CHANNEL_R_TO_L }
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetOptChannel_AIO1(AIO_OPT_CHANNEL sel)
{
#ifndef CONFIG_ENABLE_AUDIO
    sel = sel;
#else
    analog_in_out_sel_RBUS AIO_SEL_REG;

    AIO_SEL_REG.regValue = rtd_inl(ANALOG_IN_OUT_SEL_0x3904);

    AIO_SEL_REG.bitField.aio1out_source = sel;

    rtd_outl(ANALOG_IN_OUT_SEL_0x3904, AIO_SEL_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Select AIO2 output channel configuration.
 *
 * @param <sel>	{ AIO_OPT_CHANNEL_STEREO | AIO_OPT_CHANNEL_L_TO_R | AIO_OPT_CHANNEL_R_TO_L }
 * @return		{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetOptChannel_AIO2(UINT8 sel)
{
#ifndef CONFIG_ENABLE_AUDIO
    sel = sel;
#else
    analog_in_out_sel_RBUS AIO_SEL_REG;

    AIO_SEL_REG.regValue = rtd_inl(ANALOG_IN_OUT_SEL_0x3904);

    AIO_SEL_REG.bitField.aio2out_source = sel;

    rtd_outl(ANALOG_IN_OUT_SEL_0x3904, AIO_SEL_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Mute/Unmute Headphone(Amp) output.
 *
 * @param <para>	{ _ENABLE / _DISABLE }
 * @return			{ void }
 * @ingroup lib_audio
 */
void Audio_AioMute_Headphone(BOOL para)
{
#ifndef CONFIG_ENABLE_AUDIO
    para = para;
#else
    analog_aio_master_hpout_lineout_RBUS HP_LINEOUT_MIX_REG;

    HP_LINEOUT_MIX_REG.regValue = rtd_inl(ANALOG_AIO_MASTER_HPOUT_LINEOUT_0x3910);

    HP_LINEOUT_MIX_REG.bitField.mute_left_amp = para;
    HP_LINEOUT_MIX_REG.bitField.mute_right_amp = para;

    rtd_outl(ANALOG_AIO_MASTER_HPOUT_LINEOUT_0x3910, HP_LINEOUT_MIX_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Mute/Unmute Lineout output.
 *
 * @param <para>	{ _ENABLE / _DISABLE }
 * @return			{ void }
 * @ingroup lib_audio
 */
void Audio_AioMute_Lineout(BOOL para)
{
#ifndef CONFIG_ENABLE_AUDIO
    para = para;
#else
    analog_aio_master_hpout_lineout_RBUS HP_LINEOUT_MIX_REG;

    HP_LINEOUT_MIX_REG.regValue = rtd_inl(ANALOG_AIO_MASTER_HPOUT_LINEOUT_0x3910);

    HP_LINEOUT_MIX_REG.bitField.mute_left_lineout = para;
    HP_LINEOUT_MIX_REG.bitField.mute_right_lineout = para;

    rtd_outl(ANALOG_AIO_MASTER_HPOUT_LINEOUT_0x3910, HP_LINEOUT_MIX_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}

#ifdef CONFIG_SCART_AUTO_SWITCH
/**
 * Mute/Unmute AIO1 output.
 *
 * @param <para>	{ _ENABLE / _DISABLE }
 * @return			{ void }
 * @ingroup lib_audio
 */
void Audio_AioMute_AIO1(BOOL para)
{
#ifndef CONFIG_ENABLE_AUDIO
    para = para;
#else
    analog_aio_master_hpout_lineout_RBUS HP_LINEOUT_MIX_REG;

    HP_LINEOUT_MIX_REG.regValue = rtd_inl(ANALOG_AIO_MASTER_HPOUT_LINEOUT_0x3910);

    HP_LINEOUT_MIX_REG.bitField.reg_bb_m_aio1_l = para;
    HP_LINEOUT_MIX_REG.bitField.reg_bb_m_aio1_r = para;

    rtd_outl(ANALOG_AIO_MASTER_HPOUT_LINEOUT_0x3910, HP_LINEOUT_MIX_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Mute/Unmute AIO2 output.
 *
 * @param <para>	{ _ENABLE / _DISABLE }
 * @return			{ void }
 * @ingroup lib_audio
 */
void Audio_AioMute_AIO2(BOOL para)
{
#ifndef CONFIG_ENABLE_AUDIO
    para = para;
#else
    analog_aio_master_hpout_lineout_RBUS HP_LINEOUT_MIX_REG;

    HP_LINEOUT_MIX_REG.regValue = rtd_inl(ANALOG_AIO_MASTER_HPOUT_LINEOUT_0x3910);

    HP_LINEOUT_MIX_REG.bitField.reg_bb_m_aio2_l = para;
    HP_LINEOUT_MIX_REG.bitField.reg_bb_m_aio2_r = para;

    rtd_outl(ANALOG_AIO_MASTER_HPOUT_LINEOUT_0x3910, HP_LINEOUT_MIX_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}
#endif
#if 0	// marked by weihao because of uncalled
/**
 * Set BBADC volume1
 * range from -58.5dB to +24dB , 1.5dB per step
 * [-58.5dB ~ 0dB ~ +24dB] = [ 0x00 ~ 0x27 ~ 0x37 ]
 *
 * @param <l_gain>	{ volume of left channel }
 * @param <r_gain>	{ volume of right channel }
 * @return			{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetBBADCVol1(UINT8 l_gain, UINT8 r_gain)
{
#ifndef CONFIG_ENABLE_AUDIO
    l_gain = l_gain;
    r_gain = r_gain;
#else
    adc_volume_gain_control_RBUS	BBADC_VOL_CTRL_REG;

    BBADC_VOL_CTRL_REG.regValue = rtd_inl(ADC_VOLUME_GAIN_CONTROL_0x3914);

    if (l_gain > AIO_BBADC_MAX_GAIN)
        l_gain  = AIO_BBADC_MAX_GAIN;
    if (r_gain > AIO_BBADC_MAX_GAIN)
        r_gain  = AIO_BBADC_MAX_GAIN;

    BBADC_VOL_CTRL_REG.bitField.avol_1_in_left_gain = l_gain;
    BBADC_VOL_CTRL_REG.bitField.avol_1_in_right_gain = r_gain;

    rtd_outl(ADC_VOLUME_GAIN_CONTROL_0x3914, BBADC_VOL_CTRL_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Set BBADC volume2
 * range from -58.5dB to +24dB , 1.5dB per step
 * [-58.5dB ~ 0dB ~ +24dB] = [ 0x00 ~ 0x27 ~ 0x37 ]
 *
 * @param <gain>	{ volume value }
 * @return			{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetBBADCVol2(UINT8 gain)
{
#ifndef CONFIG_ENABLE_AUDIO
    gain = gain;
#else
    adc_volume_gain_control_RBUS	BBADC_VOL_CTRL_REG;

    BBADC_VOL_CTRL_REG.regValue = rtd_inl(ADC_VOLUME_GAIN_CONTROL_0x3914);

    if (gain > AIO_BBADC_MAX_GAIN)
        gain  = AIO_BBADC_MAX_GAIN;

    BBADC_VOL_CTRL_REG.bitField.avol_2_in_gain = gain;

    rtd_outl(ADC_VOLUME_GAIN_CONTROL_0x3914, BBADC_VOL_CTRL_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}
#endif

/**
 * Enable/Disable Sparrow Audio Analog Modules
 *
 * @param <para>		{ _ENABLE / _DISABLE }
 * @param <module_idx>	{ AIO_POW_XXX, refer to audio_if.h }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetAnalogPower(BOOL para, UINT32 module_idx)
{
#ifndef CONFIG_ENABLE_AUDIO
    para = para;
    module_idx = module_idx;
#else
    analog_power_RBUS ANALOG_POW_REG;

    ANALOG_POW_REG.regValue = rtd_inl(ANALOG_POWER_0x3918);

    if (para == _ENABLE)
    {
        ANALOG_POW_REG.regValue |= module_idx;
    }
    else
    {
        ANALOG_POW_REG.regValue &= ~module_idx;
    }

    rtd_outl(ANALOG_POWER_0x3918, ANALOG_POW_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}

/**
 * Enable/Disable LasVegas Audio HP-Amp output
 *
 * @param <para>		{ _ENABLE / _DISABLE }
 * @return				{ void }
 * @ingroup lib_audio
 */
void Audio_AioSetHeadphoneAmp(BOOL para)
{
#ifndef CONFIG_ENABLE_AUDIO
    para = para;
#else
    analog_aio_master_hpout_lineout_RBUS HP_LINEOUT_MIX_REG;

    HP_LINEOUT_MIX_REG.regValue = rtd_inl(ANALOG_AIO_MASTER_HPOUT_LINEOUT_0x3910);

    HP_LINEOUT_MIX_REG.bitField.reg_bb_en_amp = para;

    rtd_outl(ANALOG_AIO_MASTER_HPOUT_LINEOUT_0x3910, HP_LINEOUT_MIX_REG.regValue);

#endif // !CONFIG_ENABLE_AUDIO
}

/*===================== Local Routines ========================*/


