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
* @{
*/

/*==================== Definitions ================= */
#include "flow_audio.h"
#include "timer_event.h"
#include "pcb\pcb_utility.h"
#include "pcb_customer.h"
#include "ifd\ifd.h"
#include "tv.h"
#include <tuner\tuner.h>
#include "rosprintf.h"
#include "isr_utility.h"
#include "autoconf.h"

UINT8 amp_mute_delay = 0;
UINT8 amp_unmute_delay = 0;
bit bearphone = 0;
//UINT8 drv_tuner_TunerInit(UINT8 mode);
#if 0
/*==================== Functions ================== */
/**
 * flow_audio_setsurround
 * This API is used to setting audio setting Spatial
 *
 * @param [input] :  <mode>		{ 0:Disable, 1:WOW, 2:BBE, 3:Realtek Surround}
  * @param [input] :  <mode>		{ 0:Disable, 1:WOW, 2:BBE, 3:Realtek Surround}
   @param [input] :  <plib_info_bbe>		{ the parameters of BBE method}
 * @return None
 *
 */
void flow_audio_Setsurround(UINT8 mode, void* plib_info_bbe)
{
}
#endif

volatile bit fPollingAudioInterrupt = 0;
extern void Audio_ISR_Dispatcher();
//#ifdef CONFIG_ENABLE_HDMI
bit flow_audio_Interrupt_Proc(void)
{
    if (!drv_isr_GetPending(SYS_ISR_AUDIO))
        return RT_ISR_UNHANDLED;

    fPollingAudioInterrupt = 1;

    drv_isr_isr_Enable(SYS_ISR_AUDIO, _FALSE);

    return RT_ISR_HANDLED;
}
//#endif
void flow_audio_Handler(void)
{
   // static bit  muteFlag =2;
    UINT8 adc_value = 0;
    
    if (fPollingAudioInterrupt)
    {
        Audio_ISR_Dispatcher();
        fPollingAudioInterrupt = 0;
        drv_isr_Enable(SYS_ISR_AUDIO, _TRUE);
    }
/*
    while(!drv_lsadcValRead(PAD_LSADC_2,&adc_value));
    //RTD_Log(LOGGER_DEBUG , "PAD_LSADC_2 is %x--------------------------------\r\n", (UINT32)adc_value);
    if (adc_value>0x20)
    {
        //   RTD_Log(LOGGER_INFO, "ADC_EARPHONE----------------------------------------\n");
        if (!drv_pcb_get_gpio_pin(GPIO_AUDIO_AMP_MUTE))
        {
            flow_audio_Setmute(_ENABLE, SCALER, (ADC_CH_IDX)ADC_CH_C);

            muteFlag=_TRUE;
        }
        bearphone = _TRUE;
    }
    else
    {
        //   RTD_Log(LOGGER_INFO, "-+++++++++++++++++++++++++++++++\n");
        if (muteFlag==_TRUE)
        {
            RTD_Log(LOGGER_INFO, "&&&&&&&&&&&&&&&&&&&&&&&&&&&&-\n");
            flow_audio_Setmute(_DISABLE, SCALER, (ADC_CH_IDX)ADC_CH_C);
            muteFlag =_FALSE;
        }
        bearphone = _FALSE;
    }
*/

}

/**
 * flow_audio_selectSource
 * This API is used to process source select
 *
 * @param [input] :  <StructSourceInput>		{ source table }
 * @return None
 *
 */
void flow_audio_selectSource(StructSourceInput*  ptSourceInput)
//void flow_audio_selectSource(UINT8 sel)
{
#if 1
    STRUCT_AUDIO_INPUT_BBADC* ptstInput_BBAdc;
    STRUCT_AUDIO_INPUT_SPDIF* ptstInput_SPDif;
    STRUCT_AUDIO_INPUT_I2S* ptstInput_I2S;
    AIO_PATH_CFG stPathCfg;

    // Avoid to get null pointer
    if (ptSourceInput == NULL)
        return;

    switch (ptSourceInput->AudioOptions->src_type)
    {
    case AUDIO_TYPE_BBADC:
        ptstInput_BBAdc = ptSourceInput->AudioOptions->cfgOptions;
        Audio_AioSetIptSrc_BBADC(ptstInput_BBAdc->ain);
        stPathCfg.id = ptstInput_BBAdc->aio_path_cfg->id;
        stPathCfg.src[0] = ptstInput_BBAdc->aio_path_cfg->src[0];
        stPathCfg.src[1] = ptstInput_BBAdc->aio_path_cfg->src[1];
        stPathCfg.src[2] = ptstInput_BBAdc->aio_path_cfg->src[2];
        stPathCfg.src[3] = ptstInput_BBAdc->aio_path_cfg->src[3];
        Audio_AioSetPathSrc(&stPathCfg);
#if 0
#if 1
        // for SCART-bypass
        stPathCfg.id = AIO_PATH_ID_TS;
        stPathCfg.src[0] = ptstInput_BBAdc->aio_path_cfg->src[0];
#else
        // for CANAL+
        stPathCfg.id = AIO_PATH_ID_TS;
        stPathCfg.src[0] = AIO_PATH_SRC_ATV;
#endif
        stPathCfg.src[1] = ptstInput_BBAdc->aio_path_cfg->src[1];
        stPathCfg.src[2] = ptstInput_BBAdc->aio_path_cfg->src[2];
        stPathCfg.src[3] = ptstInput_BBAdc->aio_path_cfg->src[3];
        Audio_AioSetPathSrc(&stPathCfg);
#endif
        break;

    case AUDIO_TYPE_TV:
        stPathCfg.id = ((AIO_PATH_CFG*)ptSourceInput->AudioOptions->cfgOptions)->id;
        stPathCfg.src[0] = ((AIO_PATH_CFG*)ptSourceInput->AudioOptions->cfgOptions)->src[0];
        stPathCfg.src[1] = ((AIO_PATH_CFG*)ptSourceInput->AudioOptions->cfgOptions)->src[1];
        stPathCfg.src[2] = ((AIO_PATH_CFG*)ptSourceInput->AudioOptions->cfgOptions)->src[2];
        stPathCfg.src[3] = ((AIO_PATH_CFG*)ptSourceInput->AudioOptions->cfgOptions)->src[3];
        Audio_AioSetPathSrc(&stPathCfg);
//			Audio_AioSetPathSrc(ptSourceInput->AudioOptions->cfgOptions);
//			soundsystem=GET_SOUND_SYSTEM();
//			fwif_audio_setsoundmode(soundsystem);
        RTD_Log(LOGGER_INFO, "AUDIO_TYPE_TV\n");
        break;

    case AUDIO_TYPE_I2S:
        ptstInput_I2S = ptSourceInput->AudioOptions->cfgOptions;
        Audio_AioSetIptSrc_I2SPri(ptstInput_I2S->ain);
        stPathCfg.id = ptstInput_I2S->aio_path_cfg->id;
        stPathCfg.src[0] = ptstInput_I2S->aio_path_cfg->src[0];
        stPathCfg.src[1] = ptstInput_I2S->aio_path_cfg->src[1];
        stPathCfg.src[2] = ptstInput_I2S->aio_path_cfg->src[2];
        stPathCfg.src[3] = ptstInput_I2S->aio_path_cfg->src[3];
        Audio_AioSetPathSrc(&stPathCfg);
//			Audio_AioSetPathSrc(ptstInput_I2S->aio_path_cfg);
        RTD_Log(LOGGER_INFO, "AUDIO_TYPE_I2S = %x\n", (UINT32)ptstInput_I2S->ain);
        break;

    case AUDIO_TYPE_SPDIF:
        ptstInput_SPDif = ptSourceInput->AudioOptions->cfgOptions;
        Audio_AioSetIptSrc_SPDIF(ptstInput_SPDif->ain);
        stPathCfg.id = ptstInput_SPDif->aio_path_cfg->id;
        stPathCfg.src[0] = ptstInput_SPDif->aio_path_cfg->src[0];
        stPathCfg.src[1] = ptstInput_SPDif->aio_path_cfg->src[1];
        stPathCfg.src[2] = ptstInput_SPDif->aio_path_cfg->src[2];
        stPathCfg.src[3] = ptstInput_SPDif->aio_path_cfg->src[3];
        Audio_AioSetPathSrc(&stPathCfg);
//			Audio_AioSetPathSrc(ptstInput_SPDif->aio_path_cfg);
        RTD_Log(LOGGER_INFO, "AUDIO_TYPE_SPDIF = %x\n", (UINT32)ptstInput_SPDif->ain);
        break;

    default:
        // add code dump in future
        break;
    };

#else
    switch (sel)
    {
    case _SOURCE_VGA:
//		case _SOURCE_INDEX_VGA://VGA mode
        Audio_AioSetIptSrc_BBADC(AIO_BBADC_AIN1);
        Audio_AioSetPathSrc(&audio_path_cfg_bbadc);
        break;

    case _SOURCE_CVBS:
//		case _SOURCE_INDEX_AV:
//		case _SOURCE_INDEX_AV1:
//		case _SOURCE_INDEX_AV2:
//		case _SOURCE_INDEX_AV3:
        Audio_AioSetIptSrc_BBADC(AIO_BBADC_AIN3);
        Audio_AioSetPathSrc(&audio_path_cfg_bbadc);
        break;
    case _SOURCE_SV:
//		case _SOURCE_INDEX_SV:
//		case _SOURCE_INDEX_SV1:
        Audio_AioSetIptSrc_SPDIF(AIO_BBADC_AIN3);
        Audio_AioSetPathSrc(&audio_path_cfg_bbadc);
        break;
    case _SOURCE_TV:
//		case _SOURCE_INDEX_TV:
        Audio_AioSetPathSrc(&audio_path_cfg_atv);
//			soundsystem=GET_SOUND_SYSTEM();
        RTD_Log(LOGGER_DEBUG, "Set tv audio stand %x\n",soundsystem);
//			fwif_audio_setsoundmode(soundsystem);
        break;
    case _SOURCE_YPBPR:
//		case _SOURCE_INDEX_YPbPr:
        Audio_AioSetIptSrc_BBADC(AIO_BBADC_AIN2);
        Audio_AioSetPathSrc(&audio_path_cfg_bbadc);
        break;
//		case _SOURCE_INDEX_HDMI:
//		case _SOURCE_INDEX_HDMI1:
    case _SOURCE_HDMI:
        //only set at HDMI driver//drvif_audio_set_source(ADC_SRC_HDMI_I2S);
        Audio_AioSetIptSrc_SPDIF(AIO_SPDIFI_HDMI);
        Audio_AioSetPathSrc(&audio_path_cfg_spdif);
        Audio_AioSetIptSrc_I2SPri(AIO_I2SI_HDMI);
        Audio_AioSetPathSrc(&audio_path_cfg_i2s);
        break;
    default:
        break;
    };
#endif
}

/**
 * fwif_audio_setmute
 * This API is used to process system mute/unmute control
 *
 * @param [input] :  <enable>		{ 1:MUte,0:UNMUTE}
 * @param <flag>	{ SCALER | BLOCK| OSD | VCHIP }
 * @param <ch_idx>	{ CH_L | CH_R | CH_LS | CH_RS | CH_C | CH_SW }
 * @param [output] :
 * @return None
 *
 */
void flow_audio_Setmute(bit enable, MUTE_FLAG flag, ADC_CH_IDX ch_idx)
{
    static UINT8 mute_flag = 0;
    if (enable) //mute
    {
        mute_flag |= flag;
        Audio_HwpSetMute(_ENABLE, ch_idx, ADC_SYS_MUTE);

        if (amp_mute_delay > 0)
            fw_timer_event_DelayXms(amp_mute_delay);

        AUDIO_AMP_MUTE_ENABLE(AUDIO_AMP_MUTE);
    }
    else //unmute
    {
        mute_flag &=~(flag);
        if (!mute_flag)
        {
            AUDIO_AMP_MUTE_ENABLE(AUDIO_AMP_UNMUTE);
            if (amp_unmute_delay > 0)
                fw_timer_event_DelayXms(amp_unmute_delay);

            Audio_HwpSetMute(_FALSE, ch_idx, ADC_SYS_MUTE);
        }
    }
}
#ifdef _VIDEO_TV_SUPPORT
/**
 * flow_audio_setsoundmode
 * This API is used to process TV audio mode control
 *
 * @param <mode>	{ _SOUND_DK,_SOUND_I, _SOUND_BG, _SOUND_MN}
 * @param [output] :
 * @return None
 *
 */
void flow_audio_Setsoundmode(UINT8 mode)
{
    switch (mode)
    {
    case _SOUND_DK:
        //drvif_tuner_init(_SIG_MODE_PAL_DK);
        drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_DK);
        Audio_AtvSetSoundStd(ATV_SOUND_STD_A2_DK1);
        break;
    case _SOUND_I:
        //drvif_tuner_init(_SIG_MODE_PAL_I);
        drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_I);
        Audio_AtvSetSoundStd(ATV_SOUND_STD_FM_MONO_NO_I);
        break;
    case _SOUND_BG:
        //drvif_tuner_init(_SIG_MODE_PAL_BG);
        drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_BG);
        Audio_AtvSetSoundStd(ATV_SOUND_STD_A2_BG);
        break;
    case _SOUND_N:
    case _SOUND_MN:
        //drvif_tuner_init(_SIG_MODE_NTSC);
        drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_MN);
        Audio_AtvSetSoundStd(ATV_SOUND_STD_A2_M);
        break;

    default:
        //drvif_tuner_init(_SIG_MODE_PAL_DK/*_SIG_MODE_PAL_I*/);
        drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_DK);
        Audio_AtvSetSoundStd(ATV_SOUND_STD_DK_MONO);
        break;
    }
}
#endif
#if 0
/**
 * flow_audio_ctrlspatial
 * This API is used to setting audio setting Spatial
 *
 * @param [input] :  <mode>		{ 0:original, 1:stereo, 2:1m effect, 3:2m effect, 4:4m effect}
 * @return None
 *
 */
void flow_audio_Ctrlspatial(UINT8 mode)//yyfor compile12.8
{
}

/**
 * flow_audio_setsoundmode
 * This API is used to process TV audio mode control
 *
 * @param <mode>	{ _SOUND_DK,_SOUND_I, _SOUND_BG, _SOUND_MN}
 * @param [output] :
 * @return None
 *
 */
void flow_audio_Setsoundmode(UINT8 mode)
{
}

/**
 * fw_audio_setmute
 * This API is used to initial config audio setting(Spatial,balance,volume,avl,eq,path)
 *
 * @param [input] :  <enable>		{ 1:MUte,0:UNMUTE}
 * @param <flag>	{ SCALER | BLOCK| OSD }
 * @param <ch_idx>	{ CH_L | CH_R | CH_LS | CH_RS | CH_C | CH_SW }
 * @param [output] :
 * @return None
 *
 */
void fw_audio_Startconfig(void)
{
}

/**
 * flow_audio_setmute
 * This API is used to process system mute/unmute control
 *
 * @param [input] :  <enable>		{ 1:MUte,0:UNMUTE}
 * @param <flag>	{ SCALER | BLOCK| OSD }
 * @param <ch_idx>	{ CH_L | CH_R | CH_LS | CH_RS | CH_C | CH_SW }
 * @param [output] :
 * @return None
 *
 */
//void flow_audio_Setmute(UINT8 enable, MUTE_FLAG flag, ADC_CH_IDX ch_idx)
//{
//}

/**
 * flow_audio_setsoundlanguage
 * This API is used to process TV audio loanuage mode
 *
 * @param <mode>	{ MONO, STEREO, SAP}
 * @param [output] :
 * @return None
 *
 */
void flow_audio_Setsoundlanguage(UINT8 mode)
{
}
#endif

/**
 * @}
 */
