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
#include "audio_reg.h"
#include "audio_host.h"
#include "audio_tv.h"
#include "audio_if.h"
#include "audio_hwp.h"
#include "audio/audio_api.h"

/*======================= Type Defines ========================*/
#define	MAXBASSTREBLEVALUE (8)
#define	BASSTREBLEDISABLEVALUE (MAXBASSTREBLEVALUE / 2)
#define	MAXBALANCEVALUE (60)
#define	BALANCEDISABLEVALUE (MAXBALANCEVALUE / 2)
#define	MAXVOLUME2 (20)
#define	MAXVOLUME3 (100)
#define	HL_EQ_REG_NUM2 (20)
#define	MAXEQVALUE (12)
#define	EQ_MODE_NUM	(5)
#define	EQ_ADJUST_STEP_SIZE (20)
#define	HWP_INT_MASK (0x03)
#define	AD_BASS_BP_SET (0x1 << 9)
#define	AD_TREPLE_BP_SET (0x1 << 14)
#define	AD_BASS_TREBLE_SIZE (9)  // this value is at one sample rate

extern UINT16 dragon_mute_delay_ctrl;
extern UINT16 dragon_unmute_delay_ctrl;
extern UINT16 dragon_softvol_delay_ctrl;
extern UINT16 fix_volout_level;
extern UINT8 beeper_duration;
extern UINT8 beeper_frequency;
extern UINT8 beeper_volume;

static void audio_set_mute(UINT32 ch_idx, INT32 isEnable);			// modify reg. only
//static void audio_set_balance_volume(INT32 vol, UINT8 balance);
static void audio_set_volume(UINT32 channel, INT32 volume);		// modify reg. only
#ifdef CONFIG_AUDIO_FINE_VOL
static void audio_set_fine_volume(INT32 volume, ADC_FINE_VOL_INFO* p_fine_vol_info);					// modify reg. only
#endif
static void audio_init_eq();
static void audio_adjust_eqcoef(EQ_BAND_ID id);					// modify reg. only
#ifdef CONFIG_AUDIO_EQ_MODE_2
static void audio_get_eqmode_table(UINT32 type, UINT32 sampleRate);
#endif
/*===================== Global Variable =======================*/
static UINT32 g_BalanceValue;
static INT32 g_VolumeValue;
static UINT32 g_BTSCVolumeValue;
static UINT32 g_MuteStatus[5];
static UINT32 g_FixedVolOut;		// enable/disable fixed volume output on SPDIF_OUT and Mono DAC path
static EQ_TARGET_COEF g_CurrentCoef;
static LOUDNESS_TARGET_COEF g_Loud_CurrentCoef;
static INT32	g_pre_vol=0;

static UINT8 g_avc_flag;
#ifdef CONFIG_AUDIO_EQ_MODE_2
static UINT32 g_CurrentEQMode;
#endif

#ifdef CONFIG_AUDIO_EQ_MODE_1
extern ADC_EQ_COEFF AD_LP_TABLE[];
extern ADC_EQ_COEFF AD_BP1_TABLE[];
extern ADC_EQ_COEFF AD_BP2_TABLE[];
extern ADC_EQ_COEFF AD_BP3_TABLE[];
extern ADC_EQ_COEFF AD_BP4_TABLE[];
extern ADC_EQ_COEFF AD_BP5_TABLE[];
#endif
extern ADC_EQ_COEFF AD_SPEAKERCOMPENSATE[];
#ifndef CONFIG_AUDIO_FINE_VOL
extern ADC_EQ_COEFF AD_LOUD_LP_TABLE[];
extern ADC_EQ_COEFF AD_LOUD_BP1_TABLE[];
extern ADC_EQ_COEFF AD_LOUD_BP2_TABLE[];
#endif
#ifdef CONFIG_AUDIO_EQ_MODE_2
extern UINT16 EQ_CoefTab[];
extern ADC_EQ_COEFF AD_LP_TABLE[];
extern ADC_EQ_COEFF AD_BP5_TABLE[];
#endif
#ifdef CONFIG_BTSC_SOUNDMODE_VOL_COMPENSATION
extern UINT16 sap_vol_gain; //uint: dB
extern UINT16 stereo_vol_gain;
#endif
/*====================== API Routines =========================*/
#ifdef CONFIG_AUDIO
/*=============================================================*/
/**
 * drvif_audio_set_mute
 * mute audio output
 *
 * @param <ctrl>	{ enable/disable }
 * @param <mute_type> {ADC_SYS_MUTE/ADC_HDMI_MUTE}
 * @param <ch_idx>	{ CH_L | CH_R | CH_LS | CH_RS | CH_C | CH_SW }
 * @return 			{ void }
 * @ingroup drv_adc
 */
void drvif_audio_set_mute(UINT8 ctrl, ADC_CH_IDX ch_idx, ADC_MUTE_TYPE mute_type)
{

    if ((!CAudioIsFinishPlayWav())&ctrl) return; //add this function, because the volume of sound will be effected when setting mute or unmute

    HWP_CALLER();
    HWP_MSG(LOGGER_INFO, "%s(%d,%x,%d)\n",__FUNCTION__,ctrl,ch_idx,mute_type);

    // set basic_mode and enable digital volume & EQ
    if (ctrl)
    {
        drv_audio_set_mute(ctrl, ch_idx, mute_type, dragon_mute_delay_ctrl);
    }
    else
    {
        drv_audio_set_mute(ctrl, ch_idx, mute_type, dragon_unmute_delay_ctrl);
    }

}
/*=============================================================*/
/**
 * drvif_audio_enable_speakercompensate
 * enable/disable SpeakerCompensate function with stepping feature
 *
 * @param <isEnale>	{ enable/disable }
 * @return 			{ void }
 * @ingroup drv_adc_eq1
 */
void drvif_audio_enable_speakercompensate(UINT32 isEnable)
{
    UINT32 i;

    HWP_CALLER();
    HWP_MSG(LOGGER_INFO, "%s(%d)\n",__FUNCTION__,isEnable);

    if (isEnable)
    {
        g_CurrentCoef.LP0_A1 = AD_SPEAKERCOMPENSATE[0].a1;
        g_CurrentCoef.LP0_A1 = AD_SPEAKERCOMPENSATE[0].a2; // dummy
        g_CurrentCoef.LP0_H0 = AD_SPEAKERCOMPENSATE[0].h0;
        g_CurrentCoef.BP1_A1 = AD_SPEAKERCOMPENSATE[1].a1;
        g_CurrentCoef.BP1_A2 = AD_SPEAKERCOMPENSATE[1].a2;
        g_CurrentCoef.BP1_H0 = AD_SPEAKERCOMPENSATE[1].h0;
        g_CurrentCoef.BP2_A1 = AD_SPEAKERCOMPENSATE[2].a1;
        g_CurrentCoef.BP2_A2 = AD_SPEAKERCOMPENSATE[2].a2;
        g_CurrentCoef.BP2_H0 = AD_SPEAKERCOMPENSATE[2].h0;
        g_CurrentCoef.BP3_A1 = AD_SPEAKERCOMPENSATE[3].a1;
        g_CurrentCoef.BP3_A2 = AD_SPEAKERCOMPENSATE[3].a2;
        g_CurrentCoef.BP3_H0 = AD_SPEAKERCOMPENSATE[3].h0;
        g_CurrentCoef.BP4_A1 = AD_SPEAKERCOMPENSATE[4].a1;
        g_CurrentCoef.BP4_A2 = AD_SPEAKERCOMPENSATE[4].a2;
        g_CurrentCoef.BP4_H0 = AD_SPEAKERCOMPENSATE[4].h0;
        g_CurrentCoef.BP5_A1 = AD_SPEAKERCOMPENSATE[5].a1;
        g_CurrentCoef.BP5_A2 = AD_SPEAKERCOMPENSATE[5].a2;
        g_CurrentCoef.BP5_H0 = AD_SPEAKERCOMPENSATE[5].h0;
    }
    else
    {
        g_CurrentCoef.LP0_A1 = AD_SPEAKERCOMPENSATE[0].a1;
        g_CurrentCoef.LP0_A1 = AD_SPEAKERCOMPENSATE[0].a2 ; // dummy
        g_CurrentCoef.LP0_H0 = 0;
        g_CurrentCoef.BP1_A1 = AD_SPEAKERCOMPENSATE[1].a1;
        g_CurrentCoef.BP1_A2 = AD_SPEAKERCOMPENSATE[1].a2;
        g_CurrentCoef.BP1_H0 = 0;
        g_CurrentCoef.BP2_A1 = AD_SPEAKERCOMPENSATE[2].a1;
        g_CurrentCoef.BP2_A2 = AD_SPEAKERCOMPENSATE[2].a2;
        g_CurrentCoef.BP2_H0 = 0;
        g_CurrentCoef.BP3_A1 = AD_SPEAKERCOMPENSATE[3].a1;
        g_CurrentCoef.BP3_A2 = AD_SPEAKERCOMPENSATE[3].a2;
        g_CurrentCoef.BP3_H0 = 0;
        g_CurrentCoef.BP4_A1 = AD_SPEAKERCOMPENSATE[4].a1;
        g_CurrentCoef.BP4_A2 = AD_SPEAKERCOMPENSATE[4].a2;
        g_CurrentCoef.BP4_H0 = 0;
        g_CurrentCoef.BP5_A1 = AD_SPEAKERCOMPENSATE[5].a1;
        g_CurrentCoef.BP5_A2 = AD_SPEAKERCOMPENSATE[5].a2;
        g_CurrentCoef.BP5_H0 = 0;
    }

    // update EQ registers LP0,BP1~BP5
    for (i=0; i<6; i++)
    {
        audio_adjust_eqcoef(i);
    }

}
#ifdef CONFIG_AUDIO_EQ_MODE_1
/*=============================================================*/
/**
 * drvif_audio_set_eqvalue
 * load a predefined EQ table to HWP block with stepping feature
 *
 * @param <p_eqvalue>	{ -MaxValue~+MaxValue }
 * @return 				{ void }
 * @ingroup drv_adc_eq1
 */
void drvif_audio_set_eqvalue(EQ_VALUE* p_eqvalue)
{
    INT32	base,interval,input,i;
    UINT8	input_srctype;
    ADC_TV_INFO	scan_tv_info;

    HWP_CALLER();
    HWP_MSG(LOGGER_INFO, "%s(%d,%d,%d)\n",__FUNCTION__,p_eqvalue->MaxValue,p_eqvalue->LPValue,p_eqvalue->BP5Value);

    input_srctype = drv_audio_get_source();
    drvif_audio_get_standard(&scan_tv_info);


    interval = p_eqvalue->MaxValue / (MAXEQVALUE/2);

    input = p_eqvalue->MaxValue - p_eqvalue->LPValue;
    base = 0;
    if (p_eqvalue->LPValue==0)
    {
        input = 0;
        base = 6;
    }
    else
    {
        while (input > interval)
        {
            input-=interval;
            base++;
        }
    }
    if (base >= MAXEQVALUE) input = 0;
    HWP_MSG(LOGGER_INFO, "input=%d,interval=%d,base=%d,a1=%x,h0=%x\n",input,interval,base,AD_LP_TABLE[base+1].a1,AD_LP_TABLE[base+1].h0);
    g_CurrentCoef.LP0_A1 = (INT32)(AD_LP_TABLE[base].a1 * (interval-input) + AD_LP_TABLE[base+1].a1 * input)/interval;
    g_CurrentCoef.LP0_H0 = (INT32)(AD_LP_TABLE[base].h0 * (interval-input) + AD_LP_TABLE[base+1].h0 * input)/interval;

    input = p_eqvalue->MaxValue - p_eqvalue->BP1Value;
    base = 0;
    if (p_eqvalue->BP1Value==0)
    {
        input = 0;
        base = 6;
    }
    else
    {
        while (input > interval)
        {
            input-=interval;
            base++;
        }
    }
    if (base >= MAXEQVALUE) input = 0;
    g_CurrentCoef.BP1_A1 = (INT32)(AD_BP1_TABLE[base].a1 * (interval-input) + AD_BP1_TABLE[base+1].a1 * input)/interval;
    g_CurrentCoef.BP1_A2 = (INT32)(AD_BP1_TABLE[base].a2 * (interval-input) + AD_BP1_TABLE[base+1].a2 * input)/interval;
    g_CurrentCoef.BP1_H0 = (INT32)(AD_BP1_TABLE[base].h0 * (interval-input) + AD_BP1_TABLE[base+1].h0 * input)/interval;


    input = p_eqvalue->MaxValue - p_eqvalue->BP2Value;
    base = 0;
    if (p_eqvalue->BP2Value==0)
    {
        input = 0;
        base = 6;
    }
    else
    {
        while (input > interval)
        {
            input-=interval;
            base++;
        }
    }
    if (base >= MAXEQVALUE) input = 0;
    g_CurrentCoef.BP2_A1 = (INT32)(AD_BP2_TABLE[base].a1 * (interval-input) + AD_BP2_TABLE[base+1].a1 * input)/interval;
    g_CurrentCoef.BP2_A2 = (INT32)(AD_BP2_TABLE[base].a2 * (interval-input) + AD_BP2_TABLE[base+1].a2 * input)/interval;
    g_CurrentCoef.BP2_H0 = (INT32)(AD_BP2_TABLE[base].h0 * (interval-input) + AD_BP2_TABLE[base+1].h0 * input)/interval;

    input = p_eqvalue->MaxValue - p_eqvalue->BP3Value;
    base = 0;
    if (p_eqvalue->BP3Value==0)
    {
        input = 0;
        base = 6;
    }
    else
    {
        while (input > interval)
        {
            input-=interval;
            base++;
        }
    }
    if (base >= MAXEQVALUE) input = 0;
    g_CurrentCoef.BP3_A1 = (INT32)(AD_BP3_TABLE[base].a1 * (interval-input) + AD_BP3_TABLE[base+1].a1 * input)/interval;
    g_CurrentCoef.BP3_A2 = (INT32)(AD_BP3_TABLE[base].a2 * (interval-input) + AD_BP3_TABLE[base+1].a2 * input)/interval;
    g_CurrentCoef.BP3_H0 = (INT32)(AD_BP3_TABLE[base].h0 * (interval-input) + AD_BP3_TABLE[base+1].h0 * input)/interval;

#ifdef CONFIG_ENABLE_AUDIOTV_SUPER_HDEV_MODE_PAL
    if ((drv_audio_read_register(0xb8007000) & 0xffff0000 ) !=0x26710000)
    {
//	if(drv_audio_read_register(0xb8007000)!=0x26710111) {
        if (((input_srctype==AUD_TV_NON_NICAM)||(input_srctype==AUD_TV_NICAM))
                &&((scan_tv_info.std==ADC_BTSC_MN)||(scan_tv_info.std==ADC_A2_MN)||(scan_tv_info.std==ADC_EIAJ)))
        {
            g_CurrentCoef.BP4_A1 = 0x42ED;
            g_CurrentCoef.BP4_A2 = 0x054A;
            g_CurrentCoef.BP4_H0 = 0xFA88;
            g_CurrentCoef.BP5_A1 = 0x1DE2;
            g_CurrentCoef.BP5_A2 = 0xDE25;
            g_CurrentCoef.BP5_H0 = 0xF96D;
        }
        else
        {
            input = p_eqvalue->MaxValue - p_eqvalue->BP4Value;
            base = 0;
            if (p_eqvalue->BP4Value==0)
            {
                input = 0;
                base = 6;
            }
            else
            {
                while (input > interval)
                {
                    input-=interval;
                    base++;
                }
            }
            if (base >= MAXEQVALUE) input = 0;
            g_CurrentCoef.BP4_A1 = (INT32)(AD_BP4_TABLE[base].a1 * (interval-input) + AD_BP4_TABLE[base+1].a1 * input)/interval;
            g_CurrentCoef.BP4_A2 = (INT32)(AD_BP4_TABLE[base].a2 * (interval-input) + AD_BP4_TABLE[base+1].a2 * input)/interval;
            g_CurrentCoef.BP4_H0 = (INT32)(AD_BP4_TABLE[base].h0 * (interval-input) + AD_BP4_TABLE[base+1].h0 * input)/interval;

            input = p_eqvalue->MaxValue - p_eqvalue->BP5Value;
            base = 0;
            if (p_eqvalue->BP5Value==0)
            {
                input = 0;
                base = 6;
            }
            else
            {
                while (input > interval)
                {
                    input-=interval;
                    base++;
                }
            }
            if (base >= MAXEQVALUE) input = 0;
            g_CurrentCoef.BP5_A1 = (INT32)(AD_BP5_TABLE[base].a1 * (interval-input) + AD_BP5_TABLE[base+1].a1 * input)/interval;
            g_CurrentCoef.BP5_A2 = (INT32)(AD_BP5_TABLE[base].a2 * (interval-input) + AD_BP5_TABLE[base+1].a2 * input)/interval;
            g_CurrentCoef.BP5_H0 = (INT32)(AD_BP5_TABLE[base].h0 * (interval-input) + AD_BP5_TABLE[base+1].h0 * input)/interval;
        }
    }
    else
    {
        input = p_eqvalue->MaxValue - p_eqvalue->BP4Value;
        base = 0;
        if (p_eqvalue->BP4Value==0)
        {
            input = 0;
            base = 6;
        }
        else
        {
            while (input > interval)
            {
                input-=interval;
                base++;
            }
        }
        if (base >= MAXEQVALUE) input = 0;
        g_CurrentCoef.BP4_A1 = (INT32)(AD_BP4_TABLE[base].a1 * (interval-input) + AD_BP4_TABLE[base+1].a1 * input)/interval;
        g_CurrentCoef.BP4_A2 = (INT32)(AD_BP4_TABLE[base].a2 * (interval-input) + AD_BP4_TABLE[base+1].a2 * input)/interval;
        g_CurrentCoef.BP4_H0 = (INT32)(AD_BP4_TABLE[base].h0 * (interval-input) + AD_BP4_TABLE[base+1].h0 * input)/interval;

        input = p_eqvalue->MaxValue - p_eqvalue->BP5Value;
        base = 0;
        if (p_eqvalue->BP5Value==0)
        {
            input = 0;
            base = 6;
        }
        else
        {
            while (input > interval)
            {
                input-=interval;
                base++;
            }
        }
        if (base >= MAXEQVALUE) input = 0;
        g_CurrentCoef.BP5_A1 = (INT32)(AD_BP5_TABLE[base].a1 * (interval-input) + AD_BP5_TABLE[base+1].a1 * input)/interval;
        g_CurrentCoef.BP5_A2 = (INT32)(AD_BP5_TABLE[base].a2 * (interval-input) + AD_BP5_TABLE[base+1].a2 * input)/interval;
        g_CurrentCoef.BP5_H0 = (INT32)(AD_BP5_TABLE[base].h0 * (interval-input) + AD_BP5_TABLE[base+1].h0 * input)/interval;
    }
#else
    input = p_eqvalue->MaxValue - p_eqvalue->BP4Value;
    base = 0;
    if (p_eqvalue->BP4Value==0)
    {
        input = 0;
        base = 6;
    }
    else
    {
        while (input > interval)
        {
            input-=interval;
            base++;
        }
    }
    if (base >= MAXEQVALUE) input = 0;
    g_CurrentCoef.BP4_A1 = (INT32)(AD_BP4_TABLE[base].a1 * (interval-input) + AD_BP4_TABLE[base+1].a1 * input)/interval;
    g_CurrentCoef.BP4_A2 = (INT32)(AD_BP4_TABLE[base].a2 * (interval-input) + AD_BP4_TABLE[base+1].a2 * input)/interval;
    g_CurrentCoef.BP4_H0 = (INT32)(AD_BP4_TABLE[base].h0 * (interval-input) + AD_BP4_TABLE[base+1].h0 * input)/interval;

    input = p_eqvalue->MaxValue - p_eqvalue->BP5Value;
    base = 0;
    if (p_eqvalue->BP5Value==0)
    {
        input = 0;
        base = 6;
    }
    else
    {
        while (input > interval)
        {
            input-=interval;
            base++;
        }
    }
    if (base >= MAXEQVALUE) input = 0;
    g_CurrentCoef.BP5_A1 = (INT32)(AD_BP5_TABLE[base].a1 * (interval-input) + AD_BP5_TABLE[base+1].a1 * input)/interval;
    g_CurrentCoef.BP5_A2 = (INT32)(AD_BP5_TABLE[base].a2 * (interval-input) + AD_BP5_TABLE[base+1].a2 * input)/interval;
    g_CurrentCoef.BP5_H0 = (INT32)(AD_BP5_TABLE[base].h0 * (interval-input) + AD_BP5_TABLE[base+1].h0 * input)/interval;
#endif

    // update EQ registers LP0,BP1~BP5
    for (i=0; i<6; i++)
    {
        audio_adjust_eqcoef(i);
    }
}
#endif
#ifdef CONFIG_AUDIO_EQ_MODE_2
/*=============================================================*/
/**
 * drvif_audio_set_eqmode
 * load a predefined EQ table to HWP block with stepping feature
 *
 * @param <eq_mode>	{ DISABLE/CLUB/DANCE/LIVE/POP/ROCK }
 * @return 			{ void }
 * @ingroup drv_adc_eq2
 */
void drvif_audio_set_eqmode(ADC_EQ_MODE eq_mode)
{
    UINT32 i;

    HWP_CALLER();
    HWP_MSG("%s(%d)\n",__FUNCTION__,eq_mode);

    if (eq_mode >= ADC_EQ_UNKNOWN)
        eq_mode = ADC_EQ_DISABLE;

    drv_audio_enter_critical_section();
    g_CurrentEQMode =  eq_mode;
    drv_audio_leave_critical_section();

    if (eq_mode != ADC_EQ_DISABLE)
    {
        audio_get_eqmode_table((eq_mode-1), drv_audio_get_current_samplerate() ); //
    }
    else
    {
        g_CurrentCoef.LP0_H0 = 0;
        g_CurrentCoef.BP1_H0 = 0;
        g_CurrentCoef.BP2_H0 = 0;
        g_CurrentCoef.BP3_H0 = 0;
        g_CurrentCoef.BP4_H0 = 0;
        g_CurrentCoef.BP5_H0 = 0;
    }

    // update EQ registers LP0,BP1~BP5
    for (i=0; i<6; i++)
    {
        audio_adjust_eqcoef(i);
    }
}
/*=============================================================*/
/**
 * drvif_audio_set_eqvalue
 * load a predefined EQ table to HWP block with stepping feature
 *
 * @param <p_eqvalue>	{ -MaxValue~+MaxValue }
 * @return 				{ void }
 * @ingroup drv_adc_eq1
 */
void drvif_audio_set_eqvalue_bass_treble(EQ_VALUE* p_eqvalue)
{
    INT32	base,interval,input,i;

    HWP_CALLER();
    HWP_MSG( "%s(%d,%d,%d)\n",__FUNCTION__,p_eqvalue->MaxValue,p_eqvalue->LPValue,p_eqvalue->BP5Value);

    interval = p_eqvalue->MaxValue / (MAXEQVALUE/2);

    input = p_eqvalue->MaxValue - p_eqvalue->LPValue;
    base = 0;
    if (p_eqvalue->LPValue==0)
    {
        input = 0;
        base = 6;
    }
    else
    {
        while (input > interval)
        {
            input-=interval;
            base++;
        }
    }
    if (base >= MAXEQVALUE) input = 0;
    HWP_MSG( "input=%d,interval=%d,base=%d,a1=%x,h0=%x\n",input,interval,base,AD_LP_TABLE[base+1].a1,AD_LP_TABLE[base+1].h0);
    g_CurrentCoef.LP0_A1 = (INT32)(AD_LP_TABLE[base].a1 * (interval-input) + AD_LP_TABLE[base+1].a1 * input)/interval;
    g_CurrentCoef.LP0_H0 = (INT32)(AD_LP_TABLE[base].h0 * (interval-input) + AD_LP_TABLE[base+1].h0 * input)/interval;

    input = p_eqvalue->MaxValue - p_eqvalue->BP5Value;
    base = 0;
    if (p_eqvalue->BP5Value==0)
    {
        input = 0;
        base = 6;
    }
    else
    {
        while (input > interval)
        {
            input-=interval;
            base++;
        }
    }
    if (base >= MAXEQVALUE) input = 0;
    g_CurrentCoef.BP5_A1 = (INT32)(AD_BP5_TABLE[base].a1 * (interval-input) + AD_BP5_TABLE[base+1].a1 * input)/interval;
    g_CurrentCoef.BP5_A2 = (INT32)(AD_BP5_TABLE[base].a2 * (interval-input) + AD_BP5_TABLE[base+1].a2 * input)/interval;
    g_CurrentCoef.BP5_H0 = (INT32)(AD_BP5_TABLE[base].h0 * (interval-input) + AD_BP5_TABLE[base+1].h0 * input)/interval;

    // update EQ registers LP0,BP1~BP5
    for (i=0; i<6; i++)
    {
        audio_adjust_eqcoef(i);
    }
}
#endif
/*=============================================================*/
/**
 * drvif_audio_setbasstreble
 * load a predefined Bass&Treble table to HWP block with stepping feature
 *
 * @param <p_eqvalue>	{ -MaxValue~+MaxValue }
 * @return 				{ void }
 * @ingroup drv_adc_eq1
 */
void drvif_audio_setbasstreble(LOUD_VALUE* p_loudvalue)
{
#ifndef CONFIG_AUDIO_FINE_VOL
    INT32	base,interval,input,i;

    UINT32 A1Address = 0,A2Address=0 ,H0Address=0;
    INT16 A1TargetData = 0,A2TargetData =0 ,H0TargetData=0;
    INT32 tA1,tA2,tH0,steps;

    HWP_CALLER();
    HWP_MSG(LOGGER_INFO, "%s(%d,%d,%d)\n",__FUNCTION__,p_loudvalue->MaxValue,p_loudvalue->LPValue,p_loudvalue->BP1Value);

    interval = p_loudvalue->MaxValue / (MAXEQVALUE/2);

    input = p_loudvalue->MaxValue - p_loudvalue->LPValue;
    base = 0;
    while (input > interval)
    {
        input-=interval;
        base++;
    }
    if (base >= MAXEQVALUE) input = 0;
    HWP_MSG(LOGGER_INFO, "input=%d,interval=%d,base=%d,a1=%x,h0=%x\n",input,interval,base,AD_LOUD_LP_TABLE[base+1].a1,AD_LOUD_LP_TABLE[base+1].h0);
    g_Loud_CurrentCoef.LP0_A1 = (INT32)(AD_LOUD_LP_TABLE[base].a1 * (interval-input) + AD_LOUD_LP_TABLE[base+1].a1 * input)/interval;
    g_Loud_CurrentCoef.LP0_H0 = (INT32)(AD_LOUD_LP_TABLE[base].h0 * (interval-input) + AD_LOUD_LP_TABLE[base+1].h0 * input)/interval;

    input = p_loudvalue->MaxValue - p_loudvalue->BP1Value;
    base = 0;
    while (input > interval)
    {
        input-=interval;
        base++;
    }
    if (base >= MAXEQVALUE) input = 0;
    g_Loud_CurrentCoef.BP1_A1 = (INT32)(AD_LOUD_BP1_TABLE[base].a1 * (interval-input) + AD_LOUD_BP1_TABLE[base+1].a1 * input)/interval;
    g_Loud_CurrentCoef.BP1_A2 = (INT32)(AD_LOUD_BP1_TABLE[base].a2 * (interval-input) + AD_LOUD_BP1_TABLE[base+1].a2 * input)/interval;
    g_Loud_CurrentCoef.BP1_H0 = (INT32)(AD_LOUD_BP1_TABLE[base].h0 * (interval-input) + AD_LOUD_BP1_TABLE[base+1].h0 * input)/interval;


    input = p_loudvalue->MaxValue - p_loudvalue->BP2Value;
    base = 0;
    while (input > interval)
    {
        input-=interval;
        base++;
    }
    if (base >= MAXEQVALUE) input = 0;
    g_Loud_CurrentCoef.BP2_A1 = (INT32)(AD_LOUD_BP2_TABLE[base].a1 * (interval-input) + AD_LOUD_BP2_TABLE[base+1].a1 * input)/interval;
    g_Loud_CurrentCoef.BP2_A2 = (INT32)(AD_LOUD_BP2_TABLE[base].a2 * (interval-input) + AD_LOUD_BP2_TABLE[base+1].a2 * input)/interval;
    g_Loud_CurrentCoef.BP2_H0 = (INT32)(AD_LOUD_BP2_TABLE[base].h0 * (interval-input) + AD_LOUD_BP2_TABLE[base+1].h0 * input)/interval;


    // update Loudness registers LP0,BP1,BP2
    for (i=0; i<3; i++)
    {
        switch (i)
        {
        case 0:
            A1Address = ADCHWPROCESS_LR_LOUD_LP0_A1_VADDR;
            A2Address = ADCHWPROCESS_LR_LOUD_LP0_A1_VADDR;
            H0Address = ADCHWPROCESS_LR_LOUD_LP0_H0_VADDR;
            A1TargetData = g_Loud_CurrentCoef.LP0_A1;
            A2TargetData = g_Loud_CurrentCoef.LP0_A1;
            H0TargetData = g_Loud_CurrentCoef.LP0_H0;
            break;
        case 1:
            A1Address = ADCHWPROCESS_LR_LOUD_BP1_A1_VADDR;
            A2Address = ADCHWPROCESS_LR_LOUD_BP1_A2_VADDR;
            H0Address = ADCHWPROCESS_LR_LOUD_BP1_H0_VADDR;
            A1TargetData = g_Loud_CurrentCoef.BP1_A1;
            A2TargetData = g_Loud_CurrentCoef.BP1_A2;
            H0TargetData = g_Loud_CurrentCoef.BP1_H0;
            break;
        case 2:
            A1Address = ADCHWPROCESS_LR_LOUD_BP2_A1_VADDR;
            A2Address = ADCHWPROCESS_LR_LOUD_BP2_A2_VADDR;
            H0Address = ADCHWPROCESS_LR_LOUD_BP2_H0_VADDR;
            A1TargetData = g_Loud_CurrentCoef.BP2_A1;
            A2TargetData = g_Loud_CurrentCoef.BP2_A2;
            H0TargetData = g_Loud_CurrentCoef.BP2_H0;
            break;
        default:
            break;
        }
        tA1 = drv_audio_read_register(A1Address);
        tA2 = drv_audio_read_register(A2Address);
        tH0 = drv_audio_read_register(H0Address);

        if (tA1 & 0x8000)	tA1 |= 0xFFFF0000;	// sign extension
        if (tA2 & 0x8000)	tA2 |= 0xFFFF0000;	// sign extension
        if (tH0 & 0x8000)	tH0 |= 0xFFFF0000;	// sign extension

        steps = 0;
        HWP_MSG(LOGGER_INFO, "%s tA1=%x,A1=%x\n",__FUNCTION__,tA1,A1TargetData);
        HWP_MSG(LOGGER_INFO, "%s tA2=%x,A2=%x\n",__FUNCTION__,tA2,A2TargetData);
        HWP_MSG(LOGGER_INFO, "%s tH0=%x,H0=%x",__FUNCTION__,tH0,H0TargetData);

        while ((tA1 != A1TargetData)||(tA2 != A2TargetData)||(tH0 != H0TargetData))
        {
            steps++;
            // update A1 parameter
            if (A1TargetData > tA1)  		// increasing
            {
                tA1 += EQ_ADJUST_STEP_SIZE;
                if (tA1 > A1TargetData) tA1 = A1TargetData;
                drv_audio_write_dbuf_register(A1Address,(0x0FFFF&tA1));
            }
            else if (A1TargetData < tA1) 	// decreasing
            {
                tA1 -= EQ_ADJUST_STEP_SIZE;
                if (A1TargetData > tA1) tA1 = A1TargetData;
                drv_audio_write_dbuf_register(A1Address,(0x0FFFF&tA1));
            }
            // update A2 parameter
            if (A2TargetData > tA2)  		// increasing
            {
                tA2 += EQ_ADJUST_STEP_SIZE;
                if (tA2 > A2TargetData) tA2 = A2TargetData;
                drv_audio_write_dbuf_register(A2Address,(0x0FFFF&tA2));
            }
            else if (A2TargetData < tA2) 	// decreasing
            {
                tA2 -= EQ_ADJUST_STEP_SIZE;
                if (A2TargetData > tA2) tA2 = A2TargetData;
                drv_audio_write_dbuf_register(A2Address,(0x0FFFF&tA2));
            }
            // update H0 parameter
            if (H0TargetData > tH0)  		// increasing
            {
                tH0 += EQ_ADJUST_STEP_SIZE;
                if (tH0 > H0TargetData) tH0 = H0TargetData;
                drv_audio_write_dbuf_register(H0Address,(0x0FFFF&tH0));
            }
            else if (H0TargetData < tH0) 	// decreasing
            {
                tH0 -= EQ_ADJUST_STEP_SIZE;
                if (H0TargetData > tH0) tH0 = H0TargetData;
                drv_audio_write_dbuf_register(H0Address,(0x0FFFF&tH0));
            }
        }
        HWP_MSG(LOGGER_INFO, "steps =%x\n",steps);
    }

    drv_audio_write_register(ADCHWPROCESS_LOUDNESS_CONTROL_REGISTER_VADDR, 0x180);
#endif // CONFIG_AUDIO_FINE_VOL
}


/*=============================================================*/
/**
 * drvif_audio_set_mono2stereo
 * to set mono to stereo with strengh
 *
 * @param <isEnable>	{ disable/enable }
 * @param <m2sLevel>	{ strength of M2S effect }
 * @return				{ void }
 * @ingroup drv_adc
 */
void drvif_audio_set_mono2stereo(UINT32 isEnable, ADC_M2S_LEVEL m2sLevel)
{
    spatial_effect_control_1_RBUS AD_SPATIAL_CTRL1_REG;

    HWP_CALLER();
    HWP_MSG(LOGGER_INFO, "%s(%d,%d)\n",__FUNCTION__,isEnable,m2sLevel);

    AD_SPATIAL_CTRL1_REG.regValue = drv_audio_read_register(ADCHWPROCESS_SPATIAL_EFFECT_CONTROL_1_VADDR);

    if (isEnable)
    {
        //Disable to reset and set para.
        AD_SPATIAL_CTRL1_REG.apf_en = FALSE;
        AD_SPATIAL_CTRL1_REG.f_coef = m2sLevel;
        drv_audio_write_register(ADCHWPROCESS_SPATIAL_EFFECT_CONTROL_1_VADDR,AD_SPATIAL_CTRL1_REG.regValue);
        //Enable
        AD_SPATIAL_CTRL1_REG.apf_en = TRUE;
        drv_audio_write_register(ADCHWPROCESS_SPATIAL_EFFECT_CONTROL_1_VADDR,AD_SPATIAL_CTRL1_REG.regValue);
    }
    else
    {
        AD_SPATIAL_CTRL1_REG.apf_en = FALSE;
        drv_audio_write_register(ADCHWPROCESS_SPATIAL_EFFECT_CONTROL_1_VADDR,AD_SPATIAL_CTRL1_REG.regValue);
    }


}
/*=============================================================*/
/**
 * drvif_audio_set_spatialeffect
 * to set spatial effect
 *
 * @param <isEnable>	{ disable/enable }
 * @param <setting>		{ spatial setting }
 * @return				{ void }
 * @ingroup drv_adc
 */
void drvif_audio_set_spatialeffect(UINT32 isEnable, UINT32 setting)
{
    spatial_effect_control_1_RBUS	AD_SPATIAL_CTRL1_REG;
    hw_processing_control_RBUS		AD_HW_PP_CTRL_REG;

    HWP_CALLER();
    HWP_MSG(LOGGER_INFO, "%s(%d,%d)\n",__FUNCTION__,isEnable,setting);

    AD_SPATIAL_CTRL1_REG.regValue = drv_audio_read_register(ADCHWPROCESS_SPATIAL_EFFECT_CONTROL_1_VADDR);
    if (isEnable)
    {
        // Disable SEB  to reset
        AD_SPATIAL_CTRL1_REG.seb_en = FALSE;
        drv_audio_write_register(ADCHWPROCESS_SPATIAL_EFFECT_CONTROL_1_VADDR,AD_SPATIAL_CTRL1_REG.regValue);
        // Set para.
        drv_audio_write_register(ADCHWPROCESS_SPATIAL_EFFECT_CONTROL_2_VADDR,setting);
        drv_audio_write_register(ADCHWPROCESS_SPATIAL_EFFECT_CONTROL_3_VADDR,setting);
        // Enable SEB
        AD_SPATIAL_CTRL1_REG.seb_en = TRUE;
        drv_audio_write_register(ADCHWPROCESS_SPATIAL_EFFECT_CONTROL_1_VADDR,AD_SPATIAL_CTRL1_REG.regValue);
    }
    else
    {
        // Disable
        AD_SPATIAL_CTRL1_REG.seb_en = FALSE;
        drv_audio_write_register(ADCHWPROCESS_SPATIAL_EFFECT_CONTROL_1_VADDR,AD_SPATIAL_CTRL1_REG.regValue);
    }

    // always enable
    AD_HW_PP_CTRL_REG.regValue = drv_audio_read_register(ADCHWPROCESS_HW_PROCESSING_CONTROL_VADDR);
    AD_HW_PP_CTRL_REG.spatial_ctrlenable = TRUE;
    drv_audio_write_register(ADCHWPROCESS_HW_PROCESSING_CONTROL_VADDR,AD_HW_PP_CTRL_REG.regValue);

}
/*=============================================================*/
/**
 * drvif_audio_set_volume
 * to set digital volume.
 * In this version,the mapping is Ipt(0~100)-->HW(-60dB~40dB)
 *
 * @param <Val>	{ 0~100 }
 * @return		{ void }
 * @ingroup drv_adc
 */
void drvif_audio_set_volume(UINT8 Val, ADC_FINE_VOL_INFO* p_finevol_table)
{
    INT32 realVolume;
#ifdef CONFIG_AUDIO_FINE_VOL
    ADC_FINE_VOL_INFO p_vol_info;
#else
    INT32 realVolumeL;
    INT32 realVolumeR;
#endif

    HWP_CALLER();
    HWP_MSG(LOGGER_INFO, "%s(%d) \n", __FUNCTION__,Val);

    if (Val > MAXVOLUME3)
        Val = MAXVOLUME3;

    realVolume = Val - 60;
//#warning "[audio] is this casing normal"
    g_VolumeValue = (UINT32)realVolume;

    if (g_VolumeValue==-60)
    {
        drv_audio_set_mute(AUD_ENABLE ,AUD_CHAN_L|AUD_CHAN_R, AUD_VOL_MUTE, dragon_mute_delay_ctrl);
    }
    else
    {
        drv_audio_set_mute(AUD_DISABLE ,AUD_CHAN_L|AUD_CHAN_R, AUD_VOL_MUTE, dragon_unmute_delay_ctrl);
    }

    //audio_set_balance_volume(g_VolumeValue, (UINT8) g_BalanceValue);
#ifdef CONFIG_AUDIO_FINE_VOL
    p_vol_info.fine_vol_table = p_finevol_table->fine_vol_table;
    audio_set_fine_volume(g_VolumeValue, &p_vol_info);
#else
    realVolumeL = g_VolumeValue;
    realVolumeR = g_VolumeValue;
    if ( g_BalanceValue >= BALANCEDISABLEVALUE)
    {
        realVolumeL = realVolumeL - (g_BalanceValue - BALANCEDISABLEVALUE);
    }
    else
    {
        realVolumeR = realVolumeR - (BALANCEDISABLEVALUE - g_BalanceValue);
    }
    audio_set_volume(AUD_CHAN_L,  realVolumeL+g_BTSCVolumeValue );
    audio_set_volume(AUD_CHAN_R,  realVolumeR+g_BTSCVolumeValue );
#endif

}
/*=============================================================*/
/**
 * drvif_audio_set_balance
 * to set balance on L/R channel.
 *
 * @param <Val>	{ 0~60 }
 * @return		{ void }
 * @ingroup drv_adc
 */
void drvif_audio_set_balance(UINT8 Val)
{
    INT32 realVolumeL;
    INT32 realVolumeR;

    HWP_CALLER();
    HWP_MSG(LOGGER_INFO, "%s(%d)\n",__FUNCTION__,Val);

    if (Val > MAXBALANCEVALUE)
        Val = MAXBALANCEVALUE;

    g_BalanceValue = (UINT32) (Val);

    if (g_BalanceValue==0)
    {
        drv_audio_set_mute(AUD_ENABLE ,AUD_CHAN_R ,AUD_BALANCE_MUTE, dragon_mute_delay_ctrl);
        drv_audio_set_mute(AUD_DISABLE,AUD_CHAN_L ,AUD_BALANCE_MUTE, dragon_unmute_delay_ctrl);
    }
    else if (g_BalanceValue==60)
    {
        drv_audio_set_mute(AUD_ENABLE ,AUD_CHAN_L ,AUD_BALANCE_MUTE, dragon_mute_delay_ctrl);
        drv_audio_set_mute(AUD_DISABLE,AUD_CHAN_R ,AUD_BALANCE_MUTE, dragon_unmute_delay_ctrl);
    }
    else
    {
        drv_audio_set_mute(AUD_DISABLE ,AUD_CHAN_L|AUD_CHAN_R ,AUD_BALANCE_MUTE, dragon_unmute_delay_ctrl);
    }
    //audio_set_balance_volume(g_VolumeValue, (UINT8) g_BalanceValue);

#ifdef CONFIG_AUDIO_FINE_VOL
    realVolumeL = realVolumeR = 0;
    if ( g_BalanceValue >= BALANCEDISABLEVALUE)
    {
        realVolumeL = BALANCEDISABLEVALUE - g_BalanceValue;
    }
    else
    {
        realVolumeR = g_BalanceValue - BALANCEDISABLEVALUE;
    }
    HWP_MSG(LOGGER_INFO, "realVolumeL=%d,realVolumeR=%d  \n", realVolumeL, realVolumeR);
    audio_set_volume(AUD_CHAN_L,  realVolumeL+g_BTSCVolumeValue );	// for balance only
#else
    realVolumeL = g_VolumeValue;
    realVolumeR = g_VolumeValue;
    if ( g_BalanceValue >= BALANCEDISABLEVALUE)
    {
        realVolumeL = realVolumeL - (g_BalanceValue - BALANCEDISABLEVALUE);
    }
    else
    {
        realVolumeR = realVolumeR - (BALANCEDISABLEVALUE - g_BalanceValue);
    }
#endif
    audio_set_volume(AUD_CHAN_L,  realVolumeL+g_BTSCVolumeValue );
    audio_set_volume(AUD_CHAN_R,  realVolumeR+g_BTSCVolumeValue );

}
/*=============================================================*/
/**
 * drvif_audio_set_fixedvolout
 * enable/disable fixed volume output(0dB on C/SW channel)
 * on SPDIF_OUT and Mono DAC.
 *
 * @param <setting>	{ enable/disable }
 * @return 			{ void }
 * @ingroup drv_adc
 *
 * @note this function will conflict with Muti-Channel I2S output application
 */
void drvif_audio_set_fixedvolout(UINT32 setting)
{
    HWP_CALLER();
    HWP_MSG(LOGGER_INFO, "%s(%d)\n",__FUNCTION__,setting);

    drv_audio_set_fixedvolout(setting);
}

/*=============================================================*/
/**
 * drvif_audio_get_fixedvolout
 * get the status of fixed volume output on SPDIF_OUT and Mono DAC
 *
 * @param <void>	{ void }
 * @return 			{ enable/disable }
 * @ingroup drv_adc
 */
UINT32 drvif_audio_get_fixedvolout()
{
    HWP_CALLER();
    HWP_MSG(LOGGER_INFO, "%s(%d)\n",__FUNCTION__,g_FixedVolOut);

    return g_FixedVolOut;
}
/*=============================================================*/
/**
 * drvif_audio_set_beeper_pulse
 * to set beeper pulse
 *
 * @param <beeper_duration>	{ 0x1~0xFF; 0x0: disable }
 * @param <beeper_vol>		{ADC_BEEPER_VOLUME_P0DB/.../ADC_BEEPER_VOLUME_P45DB/ADC_BEEPER_VOLUME_MUTE}
 * @return					{ void }
 * @ingroup drv_adc
 */
void drvif_audio_set_beeper_pulse(UINT8 beeper_duration, ADC_BEEPER_VOLUME beeper_vol)
{
    programmable_beeper_control_1_RBUS       AD_BEPPER_CONTROL1;
    programmable_beeper_control_2_RBUS       AD_BEPPER_CONTROL2;
    UINT32 druation_time;

    HWP_CALLER();
    HWP_MSG(LOGGER_INFO,  "%s(%d,%d)\n",__FUNCTION__, beeper_duration, beeper_vol);
    //ROSPrintf(LOGGER_INFO,  "%s(%d,%d)\n",__FUNCTION__, beeper_duration, beeper_vol);

    AD_BEPPER_CONTROL1.regValue = drv_audio_read_register(ADCHWPROCESS_PROGRAMMABLE_BEEPER_CONTROL_1_VADDR);
    AD_BEPPER_CONTROL2.regValue = drv_audio_read_register(ADCHWPROCESS_PROGRAMMABLE_BEEPER_CONTROL_2_VADDR);

    AD_BEPPER_CONTROL1.mute_beeper = TRUE;
    drv_audio_write_register(ADCHWPROCESS_PROGRAMMABLE_BEEPER_CONTROL_1_VADDR, AD_BEPPER_CONTROL1.regValue);
    AD_BEPPER_CONTROL1.duration_beeper= 0x0;
    drv_audio_write_register(ADCHWPROCESS_PROGRAMMABLE_BEEPER_CONTROL_1_VADDR, AD_BEPPER_CONTROL1.regValue);
    drv_audio_delaytime_new(500);

    if (beeper_vol!=ADC_BEEPER_VOLUME_MUTE)
    {
        AD_BEPPER_CONTROL2.beeper_volume = beeper_vol;
        drv_audio_write_register(ADCHWPROCESS_PROGRAMMABLE_BEEPER_CONTROL_2_VADDR, AD_BEPPER_CONTROL2.regValue);
        //continute mode  --> HW duration_beeper no use!!
        //AD_BEPPER_CONTROL1.duration_beeper= beeper_duration;
        //drv_audio_write_register(ADCHWPROCESS_PROGRAMMABLE_BEEPER_CONTROL_1_VADDR, AD_BEPPER_CONTROL1.regValue);
        AD_BEPPER_CONTROL1.mute_beeper = FALSE;
        drv_audio_write_register(ADCHWPROCESS_PROGRAMMABLE_BEEPER_CONTROL_1_VADDR, AD_BEPPER_CONTROL1.regValue);
        druation_time = (UINT32)(1000*beeper_duration);
        drv_audio_delaytime_new(druation_time);
        AD_BEPPER_CONTROL1.mute_beeper = TRUE;
        drv_audio_write_register(ADCHWPROCESS_PROGRAMMABLE_BEEPER_CONTROL_1_VADDR, AD_BEPPER_CONTROL1.regValue);
    }
}
/*=============================================================*/
/**
 * drvif_audio_set_autovolumecontrol
 * to set auto volume control
 *
 * @param <isEnable>	{ disable/enable }
 * @return				{ void }
 * @ingroup drv_adc
 */
void drvif_audio_set_autovolumecontrol(UINT32 isEnable, UINT32 avc_level)
{
#ifdef CONFIG_AUDIO_HW_AVC_CTRL
    drv_audio_set_autovolumecontrol(isEnable, avc_level);
#endif
}
#else // !CONFIG_AUDIO
void drvif_audio_set_mute(UINT8 ctrl, ADC_MUTE_TYPE mute_type, ADC_CH_IDX ch_idx)
{
}
void drvif_audio_enable_speakercompensate(UINT32 isEnable)
{
}
void drvif_audio_set_eqvalue(EQ_VALUE* p_eqvalue)
{
}
void drvif_audio_set_eqmode(ADC_EQ_MODE eq_mode)
{
}
void drvif_audio_set_eqvalue_bass_treble(EQ_VALUE* p_eqvalue)
{
}
void drvif_audio_setbasstreble(LOUD_VALUE* p_loudvalue)
{
}
void drvif_audio_set_bass(UINT8 Val)
{
}
void drvif_audio_set_treble(UINT8 Val)
{
}
void drvif_audio_set_mono2stereo(UINT32 isEnable, ADC_M2S_LEVEL m2sLevel)
{
}
void drvif_audio_set_spatialeffect(UINT32 isEnable, UINT32 setting)
{
}
void drvif_audio_set_volume(UINT8 Val)
{
}
void drvif_audio_set_balance(UINT8 Val)
{
}
void drvif_audio_set_fixedvolout(UINT32 setting)
{
}
UINT32 drvif_audio_get_fixedvolout()
{
}
void drvif_audio_set_beeper_pulse(UINT8 beeper_duration, ADC_BEEPER_VOLUME beeper_vol)
{
}
void drvif_audio_set_autovolumecontrol(UINT32 isEnable, UINT32 avc_level)
{
}
#endif // CONFIG_AUDIO
/*==================== Driver Routines ========================*/
/*=============================================================*/
/**
 * drv_audio_init_hwprocess
 * init global HWP setting and enable HWP block and interrupt
 *
 * @param <void>	{ void }
 * @return 			{ void }
 * @ingroup drv_adc
 */
void drv_audio_init_hwprocess()
{
#ifdef CONFIG_AUDIO
    hw_processing_control_RBUS		AD_HW_PP_CTRL_REG;
    sw_digital_volume_control_RBUS	       AD_DVOL_REG;
    programmable_beeper_control_1_RBUS       AD_BEPPER_CONTROL1;
    programmable_beeper_control_2_RBUS       AD_BEPPER_CONTROL2;

    g_BalanceValue = BALANCEDISABLEVALUE;
    g_VolumeValue = -60; //-60 db
    g_MuteStatus[AUD_SYS_MUTE] = 0;
    g_MuteStatus[AUD_HDMI_MUTE] = 0;
    g_MuteStatus[AUD_DRV_MUTE] = 0;
    g_MuteStatus[AUD_BALANCE_MUTE] =0;
    g_MuteStatus[AUD_VOL_MUTE] =0;
    g_pre_vol= -60;
#ifdef CONFIG_BTSC_SOUNDMODE_VOL_COMPENSATION
    g_BTSCVolumeValue = 0;
#endif
    g_avc_flag = 0;
    // disable fixed volume output on SPDIF and Mono DAC
#ifdef CONFIG_AUDIO_FIX_VOL_OUT
    // g_FixedVolOut = AUD_ENABLE;
    // drv_audio_write_register(ADCHWPROCESS_AUDIO_DUMMY_0948_VADDR,1);
    drv_audio_set_fixedvolout(AUD_ENABLE);
#else
    // g_FixedVolOut = AUD_DISABLE;
    // drv_audio_write_register(ADCHWPROCESS_AUDIO_DUMMY_0948_VADDR,0);
    drv_audio_set_fixedvolout(AUD_DISABLE);
#endif
    // stop all hw process
    drv_audio_write_register(ADCHWPROCESS_HW_PROCESSING_CONTROL_VADDR,0);
    // disable interrupt
    drv_audio_write_register(ADCHWPROCESS_HW_PROC_SYSTEM_INT_EN_VADDR,0);

    // digital volume, -60 db, soft mute
    AD_DVOL_REG.regValue = 0;
#ifdef CONFIG_AUDIO_FINE_VOL
    AD_DVOL_REG.vol_sw_a = 0;
#else
    AD_DVOL_REG.vol_sw_a = 0x16;
#endif	// CONFIG_AUDIO_FINE_VOL
    AD_DVOL_REG.mute_sw = TRUE;
    AD_DVOL_REG.soft_mute_sw = TRUE;
    drv_audio_write_register(ADCHWPROCESS_L_DIGITAL_VOLUME_CONTROL_VADDR,AD_DVOL_REG.regValue);
    drv_audio_write_register(ADCHWPROCESS_R_DIGITAL_VOLUME_CONTROL_VADDR,AD_DVOL_REG.regValue);
    drv_audio_write_register(ADCHWPROCESS_C_DIGITAL_VOLUME_CONTROL_VADDR,AD_DVOL_REG.regValue);
    drv_audio_write_register(ADCHWPROCESS_LS_DIGITAL_VOLUME_CONTROL_VADDR,AD_DVOL_REG.regValue);
    drv_audio_write_register(ADCHWPROCESS_RS_DIGITAL_VOLUME_CONTROL_VADDR,AD_DVOL_REG.regValue);
    AD_DVOL_REG.sw_source = TRUE;		// [0:1] = [(L+R)/2 : original input]
    drv_audio_write_register(ADCHWPROCESS_SW_DIGITAL_VOLUME_CONTROL_VADDR,AD_DVOL_REG.regValue);

    // beeper
    AD_BEPPER_CONTROL1.regValue = 0x0;
    AD_BEPPER_CONTROL1.mute_beeper = TRUE;
    AD_BEPPER_CONTROL1.mode_beeper = 0x1; //continue mode, control beeper time by sw
    AD_BEPPER_CONTROL1.duration_beeper = beeper_duration;
    drv_audio_write_register(ADCHWPROCESS_PROGRAMMABLE_BEEPER_CONTROL_1_VADDR, AD_BEPPER_CONTROL1.regValue);
    AD_BEPPER_CONTROL2.regValue = 0x0;
    AD_BEPPER_CONTROL2.freq_beeper = beeper_frequency;
    AD_BEPPER_CONTROL2.beeper_volume = beeper_volume;
    drv_audio_write_register(ADCHWPROCESS_PROGRAMMABLE_BEEPER_CONTROL_2_VADDR, AD_BEPPER_CONTROL2.regValue);

    // spatial - power on default value
    drv_audio_write_register(ADCHWPROCESS_SPATIAL_EFFECT_CONTROL_1_VADDR, 0x574);
    drv_audio_write_register(ADCHWPROCESS_SPATIAL_EFFECT_CONTROL_2_VADDR, 0xDE);
    drv_audio_write_register(ADCHWPROCESS_SPATIAL_EFFECT_CONTROL_3_VADDR, 0xDE);

    // AGC - power on defualt value
    drv_audio_write_register(ADCHWPROCESS_AUTO_VOLUME_CONTROL_1_VADDR, 0x1);
    drv_audio_write_register(ADCHWPROCESS_AUTO_VOLUME_CONTROL_2_VADDR, 0xb0/*0x10*/);
    drv_audio_write_register(ADCHWPROCESS_AVC_MONITOR_WINDOW_VADDR, 0x3FF);
    drv_audio_write_register(ADCHWPROCESS_AVC_OVER_THRESHOLD_COUNT_VADDR, 0x3FE);

    // EQ&Loundness
    audio_init_eq();

    // set basic_mode and enable digital volume & EQ
    AD_HW_PP_CTRL_REG.regValue = 0;
    AD_HW_PP_CTRL_REG.basic_premium_sel = AUD_HWP_PATH;	// alway in H/W path
    AD_HW_PP_CTRL_REG.dvol_enable = TRUE;
    AD_HW_PP_CTRL_REG.deq_enable = TRUE;
    AD_HW_PP_CTRL_REG.loud_enable = TRUE; //Add by yukichen@20070928
    AD_HW_PP_CTRL_REG.delay_ctrl = 0x100;
    drv_audio_write_register(ADCHWPROCESS_HW_PROCESSING_CONTROL_VADDR, AD_HW_PP_CTRL_REG.regValue);

    //#ifdef CONFIG_AUDIO_HW_AVC_CTRL
    drvif_Hdmi_AudioGainContol(1,0x80); //alway enable -6gain for hdmi
    //#else
    //drvif_Hdmi_AudioGainContol(0,0x80);
    //#endif

#endif // CONFIG_AUDIO
}
/*=============================================================*/
/**
 * drv_audio_set_mute
 * mute the audio output
 *
 * @param <ctrl>		{ mute/unmute }
 * @param <ch_idx>		{ CH_L | CH_R | CH_LS | CH_RS | CH_C | CH_SW }
 * @param <mute_type>	{ system/driver mute flag }
  * @param <delay_ctrl>	{ 0~1023 }
 * @return 				{ void }
 * @ingroup drv_adc
 */
void drv_audio_set_mute(UINT32 ctrl,UINT32 ch_idx, UINT32 mute_type, UINT16 delay_ctrl)
{
#ifdef CONFIG_AUDIO
    UINT32	i;
    hw_processing_control_RBUS		AD_HW_PP_CTRL_REG;

    DRV_CALLER();
    DRV_MSG(LOGGER_INFO, "%s(%d,%x,%d)\n",__FUNCTION__,ctrl,ch_idx,mute_type);
    AD_HW_PP_CTRL_REG.regValue = drv_audio_read_register(ADCHWPROCESS_HW_PROCESSING_CONTROL_VADDR);
    if (delay_ctrl<8) delay_ctrl=8;
    AD_HW_PP_CTRL_REG.delay_ctrl = delay_ctrl;
    drv_audio_write_register(ADCHWPROCESS_HW_PROCESSING_CONTROL_VADDR, AD_HW_PP_CTRL_REG.regValue);

    drv_audio_enter_critical_section();
    if (ctrl == AUD_ENABLE)
    {
        g_MuteStatus[mute_type] |= ch_idx;		// set mute record
        audio_set_mute(ch_idx, AUD_ENABLE);
    }
    else
    {
        g_MuteStatus[mute_type] &= ~ch_idx;		// clear mute record
        i = ch_idx & (g_MuteStatus[AUD_SYS_MUTE] |g_MuteStatus[AUD_HDMI_MUTE] | g_MuteStatus[AUD_DRV_MUTE]| g_MuteStatus[AUD_BALANCE_MUTE]|g_MuteStatus[AUD_VOL_MUTE] );
        audio_set_mute(ch_idx-i, AUD_DISABLE);
    }
    DRV_MSG(LOGGER_INFO, "g_MuteStatus[AUD_SYS_MUTE]=%x,g_MuteStatus[AUD_HDMI_MUTE]=%x, g_MuteStatus[AUD_DRV_MUTE]=%x,g_MuteStatus[AUD_BALANCE_MUTE]=%x, g_MuteStatus[AUD_VOL_MUTE] =%x \n",g_MuteStatus[AUD_SYS_MUTE],g_MuteStatus[AUD_HDMI_MUTE],g_MuteStatus[AUD_DRV_MUTE], g_MuteStatus[AUD_BALANCE_MUTE], g_MuteStatus[AUD_VOL_MUTE] );
    DRV_MSG(LOGGER_INFO, "L Vol=%x, R Vol=%x, C Vol=%x, SW Vol=%x\n",rtd_inl(0xb80a0804), rtd_inl(0xb80a0808), rtd_inl(0xb80a0814), rtd_inl(0xb80a0818));
    drv_audio_leave_critical_section();

#endif // CONFIG_AUDIO
}
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
void drv_audio_eq_overflow_int_handler()
{
#ifdef CONFIG_AUDIO
    hw_processing_control_RBUS	AD_HW_PP_CTRL_REG;
    system_int_flag_RBUS		AD_HW_PP_INT_FLAG_REG;
    UINT32						backup_value;

    // backup reg. and disable EQ H/W block
    AD_HW_PP_CTRL_REG.regValue = drv_audio_read_register(ADCHWPROCESS_HW_PROCESSING_CONTROL_VADDR);
    backup_value = AD_HW_PP_CTRL_REG.regValue;
    AD_HW_PP_CTRL_REG.deq_enable = FALSE;
    drv_audio_write_register(ADCHWPROCESS_HW_PROCESSING_CONTROL_VADDR, AD_HW_PP_CTRL_REG.regValue);

    // clean flag
    AD_HW_PP_INT_FLAG_REG.regValue = 0;
    AD_HW_PP_INT_FLAG_REG.eq_ovf = TRUE;	// write 1 to clear
    drv_audio_write_register(ADCHWPROCESS_SYSTEM_INT_FLAG_VADDR, AD_HW_PP_INT_FLAG_REG.regValue);

    // restore original setting
    drv_audio_write_register(ADCHWPROCESS_HW_PROCESSING_CONTROL_VADDR, backup_value);
#endif // CONFIG_AUDIO
}

UINT32 drv_audio_write_dbuf_register(UINT32 addr, UINT32 val)
{
#ifdef CONFIG_AUDIO
    UINT32 i, read_back;
    drv_audio_write_register(addr, val);
    i=1000;
    do
    {
        i--;
        read_back = drv_audio_read_register(addr);
        if (read_back == 0xdeaddead)
        {
            return 0;
        }
    }
    while ((read_back!=val)&&(i!=0));

    return 1;
#endif // CONFIG_AUDIO
}

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
void drv_audio_set_fixedvolout(UINT32 setting)
{
#ifdef CONFIG_AUDIO
    sw_digital_volume_control_RBUS	AD_DVOL_SW_REG;
    c_digital_volume_control_RBUS	AD_DVOL_C_REG;

    DRV_CALLER();
    DRV_MSG(LOGGER_INFO, "%s(%d)\n",__FUNCTION__,setting);

    AD_DVOL_SW_REG.regValue = drv_audio_read_register(ADCHWPROCESS_SW_DIGITAL_VOLUME_CONTROL_VADDR);
    AD_DVOL_C_REG.regValue = drv_audio_read_register(ADCHWPROCESS_C_DIGITAL_VOLUME_CONTROL_VADDR);
    if (setting)
    {
        AD_DVOL_C_REG.soft_mute_c = TRUE;
        AD_DVOL_C_REG.vol_c_a = 0;
        AD_DVOL_C_REG.vol_c_b = 0;

        AD_DVOL_SW_REG.soft_mute_sw = TRUE;
        AD_DVOL_SW_REG.sw_source = TRUE; // [0:1] = [(L+R)/2: original input]
        AD_DVOL_SW_REG.vol_sw_a = fix_volout_level;
        AD_DVOL_SW_REG.vol_sw_b = fix_volout_level;
    }
    else
    {
        AD_DVOL_C_REG.soft_mute_c = TRUE;
        AD_DVOL_C_REG.vol_c_a = 0;
        AD_DVOL_C_REG.vol_c_b = 0;

        AD_DVOL_SW_REG.soft_mute_sw = TRUE;
        AD_DVOL_SW_REG.vol_sw_a = 0;
        AD_DVOL_SW_REG.vol_sw_b = 0;
    }
    drv_audio_write_dbuf_register(ADCHWPROCESS_C_DIGITAL_VOLUME_CONTROL_VADDR, AD_DVOL_C_REG.regValue);
    drv_audio_write_dbuf_register(ADCHWPROCESS_SW_DIGITAL_VOLUME_CONTROL_VADDR, AD_DVOL_SW_REG.regValue);

    // enable/disable SPDIF and Mono DAC output channel 2/3 data
    g_FixedVolOut = setting;
    drv_audio_write_register(ADCHWPROCESS_AUDIO_DUMMY_0948_VADDR,setting);

#endif // CONFIG_AUDIO
}

/*=============================================================*/
/**
 * drv_audio_set_btsc_vol_compensation
 *
 * @param <setting>	{ AUD_BTSC_VOL_COMP}
 * @return 			{ void }
 * @ingroup drv_adc
 *
 */
void drv_audio_set_btsc_vol_compensation(AUD_BTSC_VOL_COMP setting)
{
#ifdef CONFIG_AUDIO
#ifdef CONFIG_BTSC_SOUNDMODE_VOL_COMPENSATION
    INT32 realVolumeL;
    INT32 realVolumeR;

    DRV_CALLER();
    DRV_MSG(LOGGER_INFO, "%s(%d)\n",__FUNCTION__,setting);

    if (setting==AUD_BTSC_VOL_COMP_SAP)
    {
        g_BTSCVolumeValue = sap_vol_gain;
    }
    else if (setting==AUD_BTSC_VOL_COMP_STEREO)
    {
        g_BTSCVolumeValue = stereo_vol_gain;
    }
    else
    {
        g_BTSCVolumeValue = 0;
    }

#ifdef CONFIG_AUDIO_FINE_VOL
    realVolumeL = realVolumeR = 0;
    if ( g_BalanceValue >= BALANCEDISABLEVALUE)
    {
        realVolumeL = BALANCEDISABLEVALUE - g_BalanceValue;
    }
    else
    {
        realVolumeR = g_BalanceValue - BALANCEDISABLEVALUE;
    }
    //no change in loudness.//audio_set_fine_volume(g_VolumeValue);
    audio_set_volume(AUD_CHAN_L,  realVolumeL+g_BTSCVolumeValue );	// for balance only
    audio_set_volume(AUD_CHAN_R,  realVolumeR+g_BTSCVolumeValue );	// for balance only
#else
    realVolumeL = g_VolumeValue;
    realVolumeR = g_VolumeValue;
    if ( g_BalanceValue >= BALANCEDISABLEVALUE)
    {
        realVolumeL = realVolumeL - (g_BalanceValue - BALANCEDISABLEVALUE);
    }
    else
    {
        realVolumeR = realVolumeR - (BALANCEDISABLEVALUE - g_BalanceValue);
    }
    audio_set_volume(AUD_CHAN_L,  realVolumeL+g_BTSCVolumeValue );
    audio_set_volume(AUD_CHAN_R,  realVolumeR+g_BTSCVolumeValue );
#endif
    DRV_MSG(LOGGER_INFO, "g_BTSCVolumeValue=%d\n", g_BTSCVolumeValue);
#else
    g_BTSCVolumeValue=0;
#endif

#endif // CONFIG_AUDIO
}
/*=============================================================*/
/**
 * drv_audio_set_autovolumecontrol
 * to set auto volume control
 *
 * @param <isEnable>	{ disable/enable }
 * @return				{ void }
 * @ingroup drv_adc
 */
void drv_audio_set_autovolumecontrol(UINT32 isEnable, UINT32 avc_level)
{
#ifdef CONFIG_AUDIO
    hw_processing_control_RBUS       AD_HWP_CTRL_REG;
    //audio_adc_gain_control_RBUS	AD_GAIN_CONTROL_REG;

    HWP_CALLER();
    HWP_MSG(LOGGER_DEBUG,"%s(%d)\n",__FUNCTION__,isEnable);

    AD_HWP_CTRL_REG.regValue = drv_audio_read_register(ADCHWPROCESS_HW_PROCESSING_CONTROL_VADDR);
    //AD_GAIN_CONTROL_REG.regValue = drv_audio_read_register(ADCINTERFACE_AUDIO_ADC_GAIN_CONTROL_VADDR);

    drv_audio_set_mute(AUD_ENABLE, AUD_CHAN_L|AUD_CHAN_R, AUD_DRV_MUTE, dragon_mute_delay_ctrl);

    if (isEnable)
    {
        drv_audio_write_register(ADCHWPROCESS_AUTO_VOLUME_CONTROL_2_VADDR, avc_level);
        AD_HWP_CTRL_REG.auto_volenable = TRUE;
        //remove, because it must always fix to -6dB gain
        //AD_GAIN_CONTROL_REG.avol_1_in_left_gain = 0x22;
        //AD_GAIN_CONTROL_REG.avol_1_in_right_gain = 0x22;
        //drvif_Hdmi_AudioGainContol(1,0x80);
        g_avc_flag = 1;
    }
    else
    {
        AD_HWP_CTRL_REG.auto_volenable = FALSE;
        //remove, because it must always fix to -6dB gain
        //AD_GAIN_CONTROL_REG.avol_1_in_left_gain = 0x27;
        //AD_GAIN_CONTROL_REG.avol_1_in_right_gain = 0x27;
        //drvif_Hdmi_AudioGainContol(0,0x80);
        g_avc_flag = 0;
    }
    //drv_audio_write_register(ADCINTERFACE_AUDIO_ADC_GAIN_CONTROL_VADDR, AD_GAIN_CONTROL_REG.regValue);
    drv_audio_write_dbuf_register(ADCHWPROCESS_HW_PROCESSING_CONTROL_VADDR, AD_HWP_CTRL_REG.regValue);

    drv_audio_delaytime_new(1);

    drv_audio_set_mute(AUD_DISABLE, AUD_CHAN_L|AUD_CHAN_R, AUD_DRV_MUTE, dragon_unmute_delay_ctrl);

#endif // CONFIG_AUDIO
}
/*=============================================================*/
/**
 * drv_audio_get_avc_status
 *
 * @param <void>	{ void}
 * @return 			{ flag }
 * @ingroup drv_adc
 *
 */
UINT8 drv_audio_get_avc_status(void)
{
    return g_avc_flag;
}
/*===================== Local Routines ========================*/
/*=============================================================*/
/**
 * audio_set_mute
 * mute/unmute the digital audio output
 *
 * @param <channel>		{ L|R|LS|RS|C|SW }
 * @param <isEnable>	{ enable/disable }
 * @return 				{ void }
 * @ingroup drv_adc
 */
static void audio_set_mute(UINT32 ch_idx, INT32 isEnable)
{
#ifdef CONFIG_AUDIO
    l_digital_volume_control_RBUS AD_DVOL_CTRL_REG;

    if (ch_idx & AUD_CHAN_L)
    {
        AD_DVOL_CTRL_REG.regValue = drv_audio_read_register(ADCHWPROCESS_L_DIGITAL_VOLUME_CONTROL_VADDR);
        AD_DVOL_CTRL_REG.mute_l = isEnable;
        drv_audio_write_register(ADCHWPROCESS_L_DIGITAL_VOLUME_CONTROL_VADDR, AD_DVOL_CTRL_REG.regValue);
    }
    if (ch_idx & AUD_CHAN_R)
    {
        AD_DVOL_CTRL_REG.regValue = drv_audio_read_register(ADCHWPROCESS_R_DIGITAL_VOLUME_CONTROL_VADDR);
        AD_DVOL_CTRL_REG.mute_l = isEnable;
        drv_audio_write_register(ADCHWPROCESS_R_DIGITAL_VOLUME_CONTROL_VADDR, AD_DVOL_CTRL_REG.regValue);
    }
    if (ch_idx & AUD_CHAN_LS)
    {
        AD_DVOL_CTRL_REG.regValue = drv_audio_read_register(ADCHWPROCESS_LS_DIGITAL_VOLUME_CONTROL_VADDR);
        AD_DVOL_CTRL_REG.mute_l = isEnable;
        drv_audio_write_register(ADCHWPROCESS_LS_DIGITAL_VOLUME_CONTROL_VADDR, AD_DVOL_CTRL_REG.regValue);
    }
    if (ch_idx & AUD_CHAN_RS)
    {
        AD_DVOL_CTRL_REG.regValue = drv_audio_read_register(ADCHWPROCESS_RS_DIGITAL_VOLUME_CONTROL_VADDR);
        AD_DVOL_CTRL_REG.mute_l = isEnable;
        drv_audio_write_register(ADCHWPROCESS_RS_DIGITAL_VOLUME_CONTROL_VADDR, AD_DVOL_CTRL_REG.regValue);
    }
    if (ch_idx & AUD_CHAN_SW)
    {
        AD_DVOL_CTRL_REG.regValue = drv_audio_read_register(ADCHWPROCESS_SW_DIGITAL_VOLUME_CONTROL_VADDR);
        AD_DVOL_CTRL_REG.mute_l = isEnable;
        drv_audio_write_register(ADCHWPROCESS_SW_DIGITAL_VOLUME_CONTROL_VADDR, AD_DVOL_CTRL_REG.regValue);
    }
    if (ch_idx & AUD_CHAN_C)
    {
        AD_DVOL_CTRL_REG.regValue = drv_audio_read_register(ADCHWPROCESS_C_DIGITAL_VOLUME_CONTROL_VADDR);
        AD_DVOL_CTRL_REG.mute_l = isEnable;
        drv_audio_write_register(ADCHWPROCESS_C_DIGITAL_VOLUME_CONTROL_VADDR, AD_DVOL_CTRL_REG.regValue);
    }

#endif // CONFIG_AUDIO
}
/*=============================================================*/
/**
 * audio_set_balance_volume
 * cal. the L/R volume according to vol&balance input
 *
 * @param <vol>		{ common volume value }
 * @param <balance>	{ balance setting }
 * @return 			{ void }
 * @ingroup drv_adc
 */
/*static void audio_set_balance_volume(INT32 vol, UINT8 balance)
{
	INT32 realVolumeL;
	INT32 realVolumeR;

	HWP_CALLER();
	HWP_MSG(LOGGER_INFO, "%s(%d, %d) \n", __FUNCTION__,vol, balance);

	realVolumeL = vol;
	realVolumeR = vol;

#ifdef CONFIG_AUDIO_FINE_VOL
	realVolumeL = realVolumeR = 0;
	if( balance >= BALANCEDISABLEVALUE) {
		realVolumeL = BALANCEDISABLEVALUE - balance;
	}else {
		realVolumeR = balance - BALANCEDISABLEVALUE;
	}
	HWP_MSG(LOGGER_INFO, "realVolumeL=%d,realVolumeR=%d  \n", realVolumeL, realVolumeR);
	audio_set_fine_volume(vol);
	audio_set_volume(AUD_CHAN_L,  realVolumeL+g_BTSCVolumeValue );	// for balance only
	audio_set_volume(AUD_CHAN_R,  realVolumeR+g_BTSCVolumeValue );	// for balance only
#else
	if( balance >= BALANCEDISABLEVALUE) {
		realVolumeL = realVolumeL - (balance - BALANCEDISABLEVALUE);
	}else {
		realVolumeR = realVolumeR - (BALANCEDISABLEVALUE - balance);
	}
	audio_set_volume(AUD_CHAN_L,  realVolumeL+g_BTSCVolumeValue );
	audio_set_volume(AUD_CHAN_R,  realVolumeR+g_BTSCVolumeValue );
#endif


}*/
/*=============================================================*/
/**
 * audio_set_volume
 * translate the volume into register's format and store it
 *
 * @param <channel>	{ channel index }
 * @param <volume>	{ volume code }
 * @return 			{ void }
 * @ingroup drv_adc
 */
static void audio_set_volume(UINT32 channel, INT32 volume)
{
#ifdef CONFIG_AUDIO
    l_digital_volume_control_RBUS	AD_DVOL_CTRL_REG;
    hw_processing_control_RBUS	AD_HW_PP_CTRL_REG;
    UINT32	addr;
    INT32	a, b;

    AD_HW_PP_CTRL_REG.regValue = drv_audio_read_register(ADCHWPROCESS_HW_PROCESSING_CONTROL_VADDR);
    AD_HW_PP_CTRL_REG.delay_ctrl = dragon_softvol_delay_ctrl;
    drv_audio_write_register(ADCHWPROCESS_HW_PROCESSING_CONTROL_VADDR, AD_HW_PP_CTRL_REG.regValue);

    switch (channel)
    {
    case AUD_CHAN_L:
        addr = ADCHWPROCESS_L_DIGITAL_VOLUME_CONTROL_VADDR;
        break;
    case AUD_CHAN_R:
        addr = ADCHWPROCESS_R_DIGITAL_VOLUME_CONTROL_VADDR;
        break;
    case AUD_CHAN_LS:
        addr = ADCHWPROCESS_LS_DIGITAL_VOLUME_CONTROL_VADDR;
        break;
    case AUD_CHAN_RS:
        addr = ADCHWPROCESS_RS_DIGITAL_VOLUME_CONTROL_VADDR;
        break;
    case AUD_CHAN_C:
        addr = ADCHWPROCESS_C_DIGITAL_VOLUME_CONTROL_VADDR;
        break;
    case AUD_CHAN_SW:
        addr = ADCHWPROCESS_SW_DIGITAL_VOLUME_CONTROL_VADDR;
        break;
    default:
        addr = ADCHWPROCESS_L_DIGITAL_VOLUME_CONTROL_VADDR;
        break;
    }

    if (volume > 40)	volume = 40;
    if (volume < -60)	volume = -60;

    a = volume / 6;
    b = volume % 6;
    if (b < 0)
    {
        a--;
        b += 6;
    }

    AD_DVOL_CTRL_REG.regValue = drv_audio_read_register(addr);
    AD_DVOL_CTRL_REG.vol_l_a = a;
    AD_DVOL_CTRL_REG.vol_l_b = b;
    drv_audio_write_dbuf_register(addr, AD_DVOL_CTRL_REG.regValue);

#endif // CONFIG_AUDIO
}

#ifdef CONFIG_AUDIO_FINE_VOL
/*=============================================================*/
/**
 * audio_set_fine_volume
 * used loudness moudule to perform volume control, L&R channel only.
 *
 * @param <volume>	{ volume code }
 * @return 			{ void }
 * @ingroup drv_adc
 */
static void audio_set_fine_volume(INT32 volume, ADC_FINE_VOL_INFO* p_fine_vol_info)
{
#ifdef CONFIG_AUDIO
    INT16				step,diff,dir;
    ADC_FINE_VOL_COEFF		target_vol, curr_vol;
    ADC_FINE_VOL_COEFF* p_vol_table;

    diff = ABS(g_pre_vol,volume);
    dir = (g_pre_vol<volume)?1:-1;
    HWP_MSG(LOGGER_INFO, "vol diff=%d,dir=%d\n",diff,dir);

    p_vol_table = p_fine_vol_info->fine_vol_table;

    if ((diff < AD_VOL_P_10DB) & (diff > 0))  	// smooth adjustment
    {
        memcpy(&target_vol,&p_vol_table[AD_FINE_VOL_TAB_OFFSET-volume],sizeof(ADC_FINE_VOL_COEFF));
        memcpy(&curr_vol,&p_vol_table[AD_FINE_VOL_TAB_OFFSET-g_pre_vol],sizeof(ADC_FINE_VOL_COEFF));
        HWP_MSG(LOGGER_INFO, "curr  : LP0=%x,BP1=%x,BP2=%x\n",curr_vol.LP0_H0,curr_vol.BP1_H0,curr_vol.BP2_H0);
        HWP_MSG(LOGGER_INFO, "target: LP0=%x,BP1=%x,BP2=%x\n",target_vol.LP0_H0,target_vol.BP1_H0,target_vol.BP2_H0);

        step = (target_vol.BP2_H0 - curr_vol.BP2_H0)/(diff*AD_FINE_VOL_STEP_PER_DB);
        if (!step)	step=dir;
        HWP_MSG(LOGGER_INFO, "BP2-step:%d\n",step);
        if (curr_vol.BP2_H0 == target_vol.BP2_H0)
        {
            drv_audio_write_dbuf_register(ADCHWPROCESS_LR_LOUD_BP2_H0_VADDR,(0x0FFFF&curr_vol.BP2_H0));
            drv_audio_delaytime_new(AUD_TIMEDLY_1MS);
        }
        while (curr_vol.BP2_H0 != target_vol.BP2_H0)  		// used in -40dB~-60dB
        {
            HWP_MSG(LOGGER_INFO, "curr_vol.BP2_H0=%x, target_vol.BP2_H0=%x\n",curr_vol.BP2_H0, target_vol.BP2_H0);
            if (target_vol.BP2_H0 > curr_vol.BP2_H0)  		// increasing
            {
                curr_vol.BP2_H0 += step;
                if (curr_vol.BP2_H0 > target_vol.BP2_H0) curr_vol.BP2_H0 = target_vol.BP2_H0;
                if (!drv_audio_write_dbuf_register(ADCHWPROCESS_LR_LOUD_BP2_H0_VADDR,(0x0FFFF&curr_vol.BP2_H0)))	break;
                drv_audio_delaytime_new(AUD_TIMEDLY_1MS);
            }
            else if (target_vol.BP2_H0 < curr_vol.BP2_H0) 	// decreasing
            {
                curr_vol.BP2_H0 += step;
                if (target_vol.BP2_H0 > curr_vol.BP2_H0) curr_vol.BP2_H0 = target_vol.BP2_H0;
                if (!drv_audio_write_dbuf_register(ADCHWPROCESS_LR_LOUD_BP2_H0_VADDR,(0x0FFFF&curr_vol.BP2_H0)))	break;
                drv_audio_delaytime_new(AUD_TIMEDLY_1MS);
            }
        }
        step = (target_vol.BP1_H0 - curr_vol.BP1_H0)/(diff*AD_FINE_VOL_STEP_PER_DB);
        if (!step)	step=dir;
        HWP_MSG(LOGGER_INFO, "BP1-step:%d\n",step);
        if (curr_vol.BP2_H0 == target_vol.BP2_H0)
        {
            drv_audio_write_dbuf_register(ADCHWPROCESS_LR_LOUD_BP1_H0_VADDR,(0x0FFFF&curr_vol.BP1_H0));
            drv_audio_delaytime_new(AUD_TIMEDLY_1MS);
        }
        while (curr_vol.BP1_H0 != target_vol.BP1_H0)  		// used in -20db~-40dB & 20dB~40dB
        {
            HWP_MSG(LOGGER_INFO, "curr_vol.BP1_H0=%x, target_vol.BP1_H0=%x\n",curr_vol.BP1_H0, target_vol.BP1_H0);
            if (target_vol.BP1_H0 > curr_vol.BP1_H0)  		// increasing
            {
                curr_vol.BP1_H0 += step;
                if (curr_vol.BP1_H0 > target_vol.BP1_H0) curr_vol.BP1_H0 = target_vol.BP1_H0;
                if (!drv_audio_write_dbuf_register(ADCHWPROCESS_LR_LOUD_BP1_H0_VADDR,(0x0FFFF&curr_vol.BP1_H0)))	break;
                drv_audio_delaytime_new(AUD_TIMEDLY_1MS);
            }
            else if (target_vol.BP1_H0 < curr_vol.BP1_H0) 	// decreasing
            {
                curr_vol.BP1_H0 += step;
                if (target_vol.BP1_H0 > curr_vol.BP1_H0) curr_vol.BP1_H0 = target_vol.BP1_H0;
                if (!drv_audio_write_dbuf_register(ADCHWPROCESS_LR_LOUD_BP1_H0_VADDR,(0x0FFFF&curr_vol.BP1_H0)))	break;
                drv_audio_delaytime_new(AUD_TIMEDLY_1MS);
            }

        }
        step = (target_vol.LP0_H0 - curr_vol.LP0_H0)/(diff*AD_FINE_VOL_STEP_PER_DB);
        if (!step)	step=dir;
        HWP_MSG(LOGGER_INFO, "LP0-step:%d\n",step);
        while (curr_vol.LP0_H0 != target_vol.LP0_H0)  		// used in -20dB~20dB
        {
            HWP_MSG(LOGGER_INFO, "curr_vol.LP0_H0=%x, target_vol.LP0_H0=%x\n",curr_vol.LP0_H0, target_vol.LP0_H0);
            if (target_vol.LP0_H0 > curr_vol.LP0_H0)  		// increasing
            {
                curr_vol.LP0_H0 += step;
                if (curr_vol.LP0_H0 > target_vol.LP0_H0) curr_vol.LP0_H0 = target_vol.LP0_H0;
                if (!drv_audio_write_dbuf_register(ADCHWPROCESS_LR_LOUD_LP0_H0_VADDR,(0x0FFFF&curr_vol.LP0_H0)))	break;
                drv_audio_delaytime_new(AUD_TIMEDLY_1MS);
            }
            else if (target_vol.LP0_H0 < curr_vol.LP0_H0) 	// decreasing
            {
                curr_vol.LP0_H0 += step;
                if (target_vol.LP0_H0 > curr_vol.LP0_H0) curr_vol.LP0_H0 = target_vol.LP0_H0;
                if (!drv_audio_write_dbuf_register(ADCHWPROCESS_LR_LOUD_LP0_H0_VADDR,(0x0FFFF&curr_vol.LP0_H0)))	break;
                drv_audio_delaytime_new(AUD_TIMEDLY_1MS);
            }
        }
    }
    else  	// setting new volume directly
    {
        HWP_MSG(LOGGER_INFO, "LP0=%x,BP1=%x,BP2=%x\n",(0x0FFFF&(p_vol_table[AD_FINE_VOL_TAB_OFFSET-volume].LP0_H0)),(0x0FFFF&(p_vol_table[AD_FINE_VOL_TAB_OFFSET-volume].BP1_H0)),(0x0FFFF&(p_vol_table[AD_FINE_VOL_TAB_OFFSET-volume].BP2_H0)));
        drv_audio_write_dbuf_register(ADCHWPROCESS_LR_LOUD_LP0_H0_VADDR, (0x0FFFF&(p_vol_table[AD_FINE_VOL_TAB_OFFSET-volume].LP0_H0)));
        drv_audio_delaytime_new(AUD_TIMEDLY_1MS);
        drv_audio_write_dbuf_register(ADCHWPROCESS_LR_LOUD_BP1_H0_VADDR, (0x0FFFF&(p_vol_table[AD_FINE_VOL_TAB_OFFSET-volume].BP1_H0)));
        drv_audio_delaytime_new(AUD_TIMEDLY_1MS);
        drv_audio_write_dbuf_register(ADCHWPROCESS_LR_LOUD_BP2_H0_VADDR, (0x0FFFF&(p_vol_table[AD_FINE_VOL_TAB_OFFSET-volume].BP2_H0)));
    }
    g_pre_vol = volume;

#endif // CONFIG_AUDIO	
}
#endif
/*=============================================================*/
/**
 * audio_init_eq
 * init global EQ setting and enable overflow interrupt
 *
 * @param <void>	{ void }
 * @return 			{ void }
 * @ingroup drv_adc
 */
static void audio_init_eq()
{
#ifdef CONFIG_AUDIO
    eq_control_status_register_RBUS		AD_EQ_CTRL_REG;
    hw_proc_system_int_en_RBUS			AD_HW_PP_INT_EN_REG;
    loudness_control_register_RBUS		AD_LOUD_CTRL_REG; //Add by yukichen@20070928


    //disable EQ/Loudness interrupt and clear interrupt pending flag
    drv_audio_write_register(ADCHWPROCESS_HW_PROC_SYSTEM_INT_EN_VADDR,0);
    drv_audio_write_register(ADCHWPROCESS_SYSTEM_INT_FLAG_VADDR,HWP_INT_MASK);

    g_CurrentCoef.LP0_A1 = AD_LP_TABLE[6].a1;//0;
    g_CurrentCoef.BP1_A1 = AD_BP1_TABLE[6].a1;//0;
    g_CurrentCoef.BP2_A1 = AD_BP2_TABLE[6].a1;//0;
    g_CurrentCoef.BP3_A1 = AD_BP3_TABLE[6].a1;//0;
    g_CurrentCoef.BP4_A1 = AD_BP4_TABLE[6].a1;//0;
    g_CurrentCoef.BP5_A1 = AD_BP5_TABLE[6].a1;//0;
    drv_audio_write_register(ADCHWPROCESS_EQ_LP0_A1_VADDR,g_CurrentCoef.LP0_A1);
    drv_audio_write_register(ADCHWPROCESS_EQ_BP1_A1_VADDR,g_CurrentCoef.BP1_A1);
    drv_audio_write_register(ADCHWPROCESS_EQ_BP2_A1_VADDR,g_CurrentCoef.BP2_A1);
    drv_audio_write_register(ADCHWPROCESS_EQ_BP3_A1_VADDR,g_CurrentCoef.BP3_A1);
    drv_audio_write_register(ADCHWPROCESS_EQ_BP4_A1_VADDR,g_CurrentCoef.BP4_A1);
    drv_audio_write_register(ADCHWPROCESS_EQ_BP5_A1_VADDR,g_CurrentCoef.BP5_A1);

    g_CurrentCoef.BP1_A2 = AD_BP1_TABLE[6].a2;//0;
    g_CurrentCoef.BP2_A2 = AD_BP2_TABLE[6].a2;//0;
    g_CurrentCoef.BP3_A2 = AD_BP3_TABLE[6].a2;//0;
    g_CurrentCoef.BP4_A2 = AD_BP4_TABLE[6].a2;//0;
    g_CurrentCoef.BP5_A2 = AD_BP5_TABLE[6].a2;//0;
    drv_audio_write_register(ADCHWPROCESS_EQ_BP1_A2_VADDR,g_CurrentCoef.BP1_A2);
    drv_audio_write_register(ADCHWPROCESS_EQ_BP2_A2_VADDR,g_CurrentCoef.BP2_A2);
    drv_audio_write_register(ADCHWPROCESS_EQ_BP3_A2_VADDR,g_CurrentCoef.BP3_A2);
    drv_audio_write_register(ADCHWPROCESS_EQ_BP4_A2_VADDR,g_CurrentCoef.BP4_A2);
    drv_audio_write_register(ADCHWPROCESS_EQ_BP5_A2_VADDR,g_CurrentCoef.BP5_A2);


    g_CurrentCoef.LP0_H0 = 0;
    g_CurrentCoef.BP1_H0 = 0;
    g_CurrentCoef.BP2_H0 = 0;
    g_CurrentCoef.BP3_H0 = 0;
    g_CurrentCoef.BP4_H0 = 0;
    g_CurrentCoef.BP5_H0 = 0;
    drv_audio_write_register(ADCHWPROCESS_EQ_LP0_H0_VADDR,g_CurrentCoef.LP0_H0);
    drv_audio_write_register(ADCHWPROCESS_EQ_BP1_H0_VADDR,g_CurrentCoef.BP1_H0);
    drv_audio_write_register(ADCHWPROCESS_EQ_BP2_H0_VADDR,g_CurrentCoef.BP2_H0);
    drv_audio_write_register(ADCHWPROCESS_EQ_BP3_H0_VADDR,g_CurrentCoef.BP3_H0);
    drv_audio_write_register(ADCHWPROCESS_EQ_BP4_H0_VADDR,g_CurrentCoef.BP4_H0);
    drv_audio_write_register(ADCHWPROCESS_EQ_BP5_H0_VADDR,g_CurrentCoef.BP5_H0);


    g_Loud_CurrentCoef.LP0_A1=0; //Add by yukichen@20070928
    g_Loud_CurrentCoef.BP1_A1 = 0;
    g_Loud_CurrentCoef.BP2_A1 = 0;

    g_Loud_CurrentCoef.BP1_A2 = 0;
    g_Loud_CurrentCoef.BP2_A2 = 0;

    g_Loud_CurrentCoef.LP0_H0 = 0;
    g_Loud_CurrentCoef.BP1_H0 = 0;
    g_Loud_CurrentCoef.BP2_H0 = 0;


    // enable EQ bands
    AD_EQ_CTRL_REG.regValue = 0;
    AD_EQ_CTRL_REG.eq_band1_enable = TRUE;
    AD_EQ_CTRL_REG.eq_band2_enable = TRUE;
    AD_EQ_CTRL_REG.eq_band3_enable = TRUE;
    AD_EQ_CTRL_REG.eq_band4_enable = TRUE;
    AD_EQ_CTRL_REG.eq_band5_enable = TRUE;
    AD_EQ_CTRL_REG.eq_lp_enable = TRUE;
    AD_EQ_CTRL_REG.eq_hp_enable = FALSE;
    drv_audio_write_register(ADCHWPROCESS_EQ_CONTROL_STATUS_REGISTER_VADDR, AD_EQ_CTRL_REG.regValue);

#ifdef CONFIG_AUDIO_FINE_VOL
    drv_audio_write_register(ADCHWPROCESS_LR_LOUD_LP0_A1_VADDR,AD_FINE_VOL_LP_A1);
    drv_audio_write_register(ADCHWPROCESS_LR_LOUD_LP0_H0_VADDR,0xF8CD);	// -20dB
    drv_audio_write_register(ADCHWPROCESS_LR_LOUD_BP1_A1_VADDR,AD_FINE_VOL_BP_A1);
    drv_audio_write_register(ADCHWPROCESS_LR_LOUD_BP1_A2_VADDR,AD_FINE_VOL_BP_A2);
    drv_audio_write_register(ADCHWPROCESS_LR_LOUD_BP1_H0_VADDR,0xF865);	// -20dB
    drv_audio_write_register(ADCHWPROCESS_LR_LOUD_BP2_A1_VADDR,AD_FINE_VOL_BP_A1);
    drv_audio_write_register(ADCHWPROCESS_LR_LOUD_BP2_A2_VADDR,AD_FINE_VOL_BP_A2);
    drv_audio_write_register(ADCHWPROCESS_LR_LOUD_BP2_H0_VADDR,0xF865);	// -20dB

    AD_LOUD_CTRL_REG.regValue = 0;
    AD_LOUD_CTRL_REG.ld_lpf_en_lr= TRUE;
    AD_LOUD_CTRL_REG.ld_bpf1_en_lr= TRUE;
    AD_LOUD_CTRL_REG.ld_bpf2_en_lr= TRUE;
#else
    // enable Loundness bands
    AD_LOUD_CTRL_REG.regValue = 0;
    AD_LOUD_CTRL_REG.ld_lpf_en_lr= TRUE;
    AD_LOUD_CTRL_REG.ld_bpf1_en_lr= TRUE;
    AD_LOUD_CTRL_REG.ld_bpf2_en_lr= 0;
    drv_audio_write_register(ADCHWPROCESS_LR_LOUD_LP0_A1_VADDR,0);
    drv_audio_write_register(ADCHWPROCESS_LR_LOUD_LP0_H0_VADDR,0);	// -20dB
    drv_audio_write_register(ADCHWPROCESS_LR_LOUD_BP1_A1_VADDR,0);
    drv_audio_write_register(ADCHWPROCESS_LR_LOUD_BP1_A2_VADDR,0);
    drv_audio_write_register(ADCHWPROCESS_LR_LOUD_BP1_H0_VADDR,0);	// -20dB
    drv_audio_write_register(ADCHWPROCESS_LR_LOUD_BP2_A1_VADDR,0);
    drv_audio_write_register(ADCHWPROCESS_LR_LOUD_BP2_A2_VADDR,0);
    drv_audio_write_register(ADCHWPROCESS_LR_LOUD_BP2_H0_VADDR,0);	// -20dB
#endif // CONFIG_AUDIO_FINE_VOL
    drv_audio_write_register(ADCHWPROCESS_LOUDNESS_CONTROL_REGISTER_VADDR, AD_LOUD_CTRL_REG.regValue);


    // enable EQ overflow interrupt
    AD_HW_PP_INT_EN_REG.regValue = 0;
    AD_HW_PP_INT_EN_REG.eq_ovf_en = TRUE;
    drv_audio_write_register(ADCHWPROCESS_HW_PROC_SYSTEM_INT_EN_VADDR,AD_HW_PP_INT_EN_REG.regValue);

#endif // CONFIG_AUDIO
}
/*=============================================================*/
/**
 * audio_adjust_eqcoef
 * update EQ setting according to global EQ control variables
 *
 * @param <id>	{ LP0/BP1/BP2/BP3/BP4/BP5 }
 * @return 		{ void }
 * @ingroup drv_adc
 */
static void audio_adjust_eqcoef(EQ_BAND_ID id)
{
#ifdef CONFIG_AUDIO

    UINT32 A1Address,A2Address,H0Address;
    INT16 A1TargetData,A2TargetData,H0TargetData;
    INT32 tA1,tA2,tH0,steps;

    drv_audio_enter_critical_section();
    switch (id)
    {
    case LP0:
        A1Address = ADCHWPROCESS_EQ_LP0_A1_VADDR;//
        A2Address = ADCHWPROCESS_EQ_LP0_A1_VADDR;// dummy
        H0Address = ADCHWPROCESS_EQ_LP0_H0_VADDR;
        A1TargetData = g_CurrentCoef.LP0_A1;
        A2TargetData = g_CurrentCoef.LP0_A1; // dummy
        H0TargetData = g_CurrentCoef.LP0_H0;
        break;

    case BP1:
        A1Address = ADCHWPROCESS_EQ_BP1_A1_VADDR;
        A2Address = ADCHWPROCESS_EQ_BP1_A2_VADDR;
        H0Address = ADCHWPROCESS_EQ_BP1_H0_VADDR;
        A1TargetData = g_CurrentCoef.BP1_A1;
        A2TargetData = g_CurrentCoef.BP1_A2;
        H0TargetData = g_CurrentCoef.BP1_H0;
        break;

    case BP2:
        A1Address = ADCHWPROCESS_EQ_BP2_A1_VADDR;
        A2Address = ADCHWPROCESS_EQ_BP2_A2_VADDR;
        H0Address = ADCHWPROCESS_EQ_BP2_H0_VADDR;
        A1TargetData = g_CurrentCoef.BP2_A1;
        A2TargetData = g_CurrentCoef.BP2_A2;
        H0TargetData = g_CurrentCoef.BP2_H0;
        break;

    case BP3:
        A1Address = ADCHWPROCESS_EQ_BP3_A1_VADDR;
        A2Address = ADCHWPROCESS_EQ_BP3_A2_VADDR;
        H0Address = ADCHWPROCESS_EQ_BP3_H0_VADDR;
        A1TargetData = g_CurrentCoef.BP3_A1;
        A2TargetData = g_CurrentCoef.BP3_A2;
        H0TargetData = g_CurrentCoef.BP3_H0;
        break;

    case BP4:
        A1Address = ADCHWPROCESS_EQ_BP4_A1_VADDR;
        A2Address = ADCHWPROCESS_EQ_BP4_A2_VADDR;
        H0Address = ADCHWPROCESS_EQ_BP4_H0_VADDR;
        A1TargetData = g_CurrentCoef.BP4_A1;
        A2TargetData = g_CurrentCoef.BP4_A2;
        H0TargetData = g_CurrentCoef.BP4_H0;
        break;

    case BP5:
        A1Address = ADCHWPROCESS_EQ_BP5_A1_VADDR;
        A2Address = ADCHWPROCESS_EQ_BP5_A2_VADDR;
        H0Address = ADCHWPROCESS_EQ_BP5_H0_VADDR;
        A1TargetData = g_CurrentCoef.BP5_A1;
        A2TargetData = g_CurrentCoef.BP5_A2;
        H0TargetData = g_CurrentCoef.BP5_H0;
        break;

    default:
        A1Address = ADCHWPROCESS_EQ_LP0_A1_VADDR;//
        A2Address = ADCHWPROCESS_EQ_LP0_A1_VADDR;// dummy
        H0Address = ADCHWPROCESS_EQ_LP0_H0_VADDR;
        A1TargetData = g_CurrentCoef.LP0_A1;
        A2TargetData = g_CurrentCoef.LP0_A1; // dummy
        H0TargetData = g_CurrentCoef.LP0_H0;
        break;
    }

    tA1 = drv_audio_read_register(A1Address);
    tA2 = drv_audio_read_register(A2Address);
    tH0 = drv_audio_read_register(H0Address);

    if (tA1 & 0x8000)	tA1 |= 0xFFFF0000;	// sign extension
    if (tA2 & 0x8000)	tA2 |= 0xFFFF0000;	// sign extension
    if (tH0 & 0x8000)	tH0 |= 0xFFFF0000;	// sign extension

    steps = 0;
    HWP_MSG(LOGGER_INFO, "%s tA1=%x,A1=%x\n",__FUNCTION__,tA1,A1TargetData);
    HWP_MSG(LOGGER_INFO, "%s tA2=%x,A2=%x\n",__FUNCTION__,tA2,A2TargetData);
    HWP_MSG(LOGGER_INFO, "%s tH0=%x,H0=%x\n",__FUNCTION__,tH0,H0TargetData);

    while ((tA1 != A1TargetData)||(tA2 != A2TargetData)||(tH0 != H0TargetData))
    {
        steps++;
        flow_timer_Reset_WatchDog();

        // update A1 parameter
        if (A1TargetData > tA1)  		// increasing
        {
            tA1 += EQ_ADJUST_STEP_SIZE;
            if (tA1 > A1TargetData) tA1 = A1TargetData;
            if (!drv_audio_write_dbuf_register(A1Address,(0x0FFFF&tA1)))	break;
        }
        else if (A1TargetData < tA1) 	// decreasing
        {
            tA1 -= EQ_ADJUST_STEP_SIZE;
            if (A1TargetData > tA1) tA1 = A1TargetData;
            if (!drv_audio_write_dbuf_register(A1Address,(0x0FFFF&tA1)))	break;
        }
        //drv_audio_delaytime_new(AUD_TIMEDLY_100US);
        // update A2 parameter
        if (A2TargetData > tA2)  		// increasing
        {
            tA2 += EQ_ADJUST_STEP_SIZE;
            if (tA2 > A2TargetData) tA2 = A2TargetData;
            if (!drv_audio_write_dbuf_register(A2Address,(0x0FFFF&tA2)))	break;
        }
        else if (A2TargetData < tA2) 	// decreasing
        {
            tA2 -= EQ_ADJUST_STEP_SIZE;
            if (A2TargetData > tA2) tA2 = A2TargetData;
            if (!drv_audio_write_dbuf_register(A2Address,(0x0FFFF&tA2)))	break;
        }
        //drv_audio_delaytime_new(AUD_TIMEDLY_100US);
        // update H0 parameter
        if (H0TargetData > tH0)  		// increasing
        {
            tH0 += EQ_ADJUST_STEP_SIZE;
            if (tH0 > H0TargetData) tH0 = H0TargetData;
            if (!drv_audio_write_dbuf_register(H0Address,(0x0FFFF&tH0)))	break;
        }
        else if (H0TargetData < tH0) 	// decreasing
        {
            tH0 -= EQ_ADJUST_STEP_SIZE;
            if (H0TargetData > tH0) tH0 = H0TargetData;
            if (!drv_audio_write_dbuf_register(H0Address,(0x0FFFF&tH0)))	break;
        }
        //drv_audio_delaytime_new(AUD_TIMEDLY_100US);
    }
    HWP_MSG(LOGGER_INFO, "steps =%x\n",steps);
    drv_audio_leave_critical_section();

#endif // CONFIG_AUDIO
}
#ifdef CONFIG_AUDIO_EQ_MODE_2
/*=============================================================*/
/**
 * audio_get_eqmode_table
 * load predefined EQ setting to global EQ control variables
 *
 * @param <type>		{ CLUB/DANCE/LIVE/POP/ROCK }
 * @param <sampleRate>	{ 32K/44K/48K/96K/192K }
 * @return 				{ void }
 * @ingroup drv_adc_eq2
 */
static void audio_get_eqmode_table(UINT32 type, UINT32 sampleRate)
{
#ifdef CONFIG_AUDIO
    if (type > EQ_MODE_NUM)
        type = 5;	// ROCK

    g_CurrentCoef.LP0_A1 = EQ_CoefTab[type*HL_EQ_REG_NUM2];
    g_CurrentCoef.BP1_A1 = EQ_CoefTab[type*HL_EQ_REG_NUM2 + 1];
    g_CurrentCoef.BP2_A1 = EQ_CoefTab[type*HL_EQ_REG_NUM2 + 2];
    g_CurrentCoef.BP3_A1 = EQ_CoefTab[type*HL_EQ_REG_NUM2 + 3];
    g_CurrentCoef.BP4_A1 = EQ_CoefTab[type*HL_EQ_REG_NUM2 + 4];
    g_CurrentCoef.BP5_A1 = EQ_CoefTab[type*HL_EQ_REG_NUM2 + 5];

    //g_CurrentCoef.HP6_A1 = EQ_CoefTab[type*HL_EQ_REG_NUM2 + 6];

    g_CurrentCoef.BP1_A2 = EQ_CoefTab[type*HL_EQ_REG_NUM2 + 7];
    g_CurrentCoef.BP2_A2 = EQ_CoefTab[type*HL_EQ_REG_NUM2 + 8];
    g_CurrentCoef.BP3_A2 = EQ_CoefTab[type*HL_EQ_REG_NUM2 + 9];
    g_CurrentCoef.BP4_A2 = EQ_CoefTab[type*HL_EQ_REG_NUM2 + 10];
    g_CurrentCoef.BP5_A2 = EQ_CoefTab[type*HL_EQ_REG_NUM2 + 11];
    //g_CurrentCoef.HP6_A2 = EQ_CoefTab[type*HL_EQ_REG_NUM2 + 12];

    g_CurrentCoef.LP0_H0 = EQ_CoefTab[type*HL_EQ_REG_NUM2 + 13];
    g_CurrentCoef.BP1_H0 = EQ_CoefTab[type*HL_EQ_REG_NUM2 + 14];
    g_CurrentCoef.BP2_H0 = EQ_CoefTab[type*HL_EQ_REG_NUM2 + 15];
    g_CurrentCoef.BP3_H0 = EQ_CoefTab[type*HL_EQ_REG_NUM2 + 16];
    g_CurrentCoef.BP4_H0 = EQ_CoefTab[type*HL_EQ_REG_NUM2 + 17];
    g_CurrentCoef.BP5_H0 = EQ_CoefTab[type*HL_EQ_REG_NUM2 + 18];
#if 0
    if (type > EQ_MODE_NUM)
        type = 5;	// ROCK

    g_CurrentCoef.LP0_A1 = EQ_CoefTab[sampleRate][type*HL_EQ_REG_NUM2];
    g_CurrentCoef.BP1_A1 = EQ_CoefTab[sampleRate][type*HL_EQ_REG_NUM2 + 1];
    g_CurrentCoef.BP2_A1 = EQ_CoefTab[sampleRate][type*HL_EQ_REG_NUM2 + 2];
    g_CurrentCoef.BP3_A1 = EQ_CoefTab[sampleRate][type*HL_EQ_REG_NUM2 + 3];
    g_CurrentCoef.BP4_A1 = EQ_CoefTab[sampleRate][type*HL_EQ_REG_NUM2 + 4];
    g_CurrentCoef.BP5_A1 = EQ_CoefTab[sampleRate][type*HL_EQ_REG_NUM2 + 5];

    //g_CurrentCoef.HP6_A1 = EQ_CoefTab[sampleRate][type*HL_EQ_REG_NUM2 + 6];

    g_CurrentCoef.BP1_A2 = EQ_CoefTab[sampleRate][type*HL_EQ_REG_NUM2 + 7];
    g_CurrentCoef.BP2_A2 = EQ_CoefTab[sampleRate][type*HL_EQ_REG_NUM2 + 8];
    g_CurrentCoef.BP3_A2 = EQ_CoefTab[sampleRate][type*HL_EQ_REG_NUM2 + 9];
    g_CurrentCoef.BP4_A2 = EQ_CoefTab[sampleRate][type*HL_EQ_REG_NUM2 + 10];
    g_CurrentCoef.BP5_A2 = EQ_CoefTab[sampleRate][type*HL_EQ_REG_NUM2 + 11];
    //g_CurrentCoef.HP6_A2 = EQ_CoefTab[sampleRate][type*HL_EQ_REG_NUM2 + 12];

    g_CurrentCoef.LP0_H0 = EQ_CoefTab[sampleRate][type*HL_EQ_REG_NUM2 + 13];
    g_CurrentCoef.BP1_H0 = EQ_CoefTab[sampleRate][type*HL_EQ_REG_NUM2 + 14];
    g_CurrentCoef.BP2_H0 = EQ_CoefTab[sampleRate][type*HL_EQ_REG_NUM2 + 15];
    g_CurrentCoef.BP3_H0 = EQ_CoefTab[sampleRate][type*HL_EQ_REG_NUM2 + 16];
    g_CurrentCoef.BP4_H0 = EQ_CoefTab[sampleRate][type*HL_EQ_REG_NUM2 + 17];
    g_CurrentCoef.BP5_H0 = EQ_CoefTab[sampleRate][type*HL_EQ_REG_NUM2 + 18];
#endif

#endif // CONFIG_AUDIO
}
#endif

