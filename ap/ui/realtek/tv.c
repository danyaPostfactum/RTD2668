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
#include <string.h>
#include "autoconf.h"

#include "flow_struct.h"
#include "flow_audio.h"
#include "mode.h"
#include "mode_customer.h"
#include "scaler_idomain\syncproc.h"
#include "scaler_idomain\measure.h"
#include "scaler_ddomain\scaler_display.h"
#include "signal\sig_tv.h"
#include "audio\audio_api.h"
#include "rosprintf.h"
#include "modestate.h"
#include "timer_event.h"
#include "reg_def.h"
#include "video\video.h"
#include "audio\audio_api.h"
#include "ifd\ifd.h"
#include "tuner\tuner.h"
#include "message.h"
#include "tv.h"
#include "eepromdefaultcustomer.h"
#include "ui_struct.h"
#include "cni_table.h"
#include "osdfunction.h"
#include "osdcore.h"
#include "osdcontrol.h"
#include "osdfont.h"
#include "rt_storage.h"
#include "storage_system.h"
#include "closecaption.h"
#include "flow_display.h"

#ifndef _VIDEO_TV_SUPPORT
UINT8 xdata tvAutoScanFlag = 0;
UINT32 xdata tvScanCurFreq = 0;
UINT8 xdata tvManualScanFlag = 0;


#endif
#ifdef _VIDEO_TV_SUPPORT
UINT8 xdata colortype;
UINT8 xdata soundtype;

/**
* @addtogroup
* @
*/

#define _TV_FineTuneBar_ZeroPoint	24
#define _TUNER_MANUAL_SCAN_MIN_SETP  (2500)
#define _TUNER_MANUAL_SCAN_MAX_SETP  (7500)
#define SECAM_FRQ                    (3750)
//auto scan flow
#define TV_PAL_NTSC_TH 1

typedef enum
{
    _TV_SCAN_NONE = 0,
    _TV_SCAN_ATV_AUTO,
    _TV_SCAN_DTV_AUTO,
} TV_SCAN_FLAG;

// Initialize parameter move void drv_ifd_system_Init(void)
#if 0
//UINT8 tvAutoScanFlag = _TV_SCAN_NONE;
// charlie, 20090210, In TV Auto Scan , 苯, 狦Τ穝计, Τ计
//UINT8 xdata tvScanStopChn = 0;
UINT8 xdata tvScanCurChn;
UINT32 xdata tvScanCurFreq;
static UINT32 xdata usAfcCurFreq = 0;
UINT8 xdata tvAutoScanFlag = 0;
UINT8 xdata tvManualScanFlag = 0;
//UINT8 xdata tvManualScanRun = 0;
UINT8 ucAfcDirection = 0;
//UINT8 xdata ucFreqInc = 0;
#ifdef CONFIG_ENABLE_TV_NOISE_BOX_ENALBE
bit xdata auto_scan_avout_for_noise_box_enable = 0;
#endif
#else
//UINT8 tvAutoScanFlag = _TV_SCAN_NONE;
// charlie, 20090210, In TV Auto Scan , 苯, 狦Τ穝计, Τ计
//UINT8 xdata tvScanStopChn = 0;
UINT8 xdata tvScanCurChn;
UINT8 xdata tvTableScanFirstChn;

UINT32 xdata tvScanCurFreq;
UINT32 xdata tvCoverCurFreq;

static UINT32 xdata usAfcCurFreq;
UINT8 xdata tvAutoScanFlag;
UINT8 xdata tvManualScanFlag;
//UINT8 xdata tvManualScanRun = 0;
UINT8 ucAfcDirection;
UINT8 AFC_stage_count;
UINT8 AScan_Noise_stas1;
UINT8 AScan_Noise_stas2;
//UINT8 xdata ucFreqInc = 0;
UINT8 ucVdc_TV_Mode;
//lewis 20100113, use 358 flag to judge NTSC mode
//#define _VDC_DETECT_358_FLAG	1
extern UINT8 drv_video_GetTvColorStandard(void);//For Keil Building

#ifdef CONFIG_ENABLE_TV_NOISE_BOX_ENALBE
bit xdata auto_scan_avout_for_noise_box_enable;
#endif
#endif
// い繵38 MHz, divider ratio = 62.5 KHz
/*
UINT16 code tNTSC_CATV_FREQ[] =
{
	0x06f4, 0x05d4, 0x0634, 0x0694, 0x0734, 0x0794, 0x0d54, 0x0db4, 0x0e14, 0x0e74,
	0x0ed4, 0x0f34, 0x0f94, 0x09f4, 0x0a54, 0x0ab4, 0x0b14, 0x0b74, 0x0bd4, 0x0c34,
	0x0c94, 0x0cf4, 0x0ff4, 0x1054, 0x10b4, 0x1114, 0x1174, 0x11d4, 0x1234, 0x1294,
	0x12f4, 0x1354, 0x13b4, 0x1414, 0x1474, 0x14d4, 0x1534, 0x1594, 0x15f4, 0x1654,
	0x16b4, 0x1714, 0x1774, 0x17d4, 0x1834, 0x1894, 0x18f4, 0x1954, 0x19b4, 0x1a14,
	0x1a74, 0x1ad4, 0x1b34, 0x1b94, 0x1bf4, 0x1c54, 0x1cb4, 0x1d14, 0x1d74, 0x1dd4,
	0x1e34, 0x1e94, 0x1ef4, 0x1f54, 0x1fb4, 0x2014, 0x2074, 0x20d4, 0x2134, 0x2194,
	0x21f4, 0x2254, 0x22b4, 0x2314, 0x2374, 0x23d4, 0x2434, 0x2494, 0x24f4, 0x2554,
	0x25b4, 0x2614, 0x2674, 0x26d4, 0x2734, 0x2794, 0x27f4, 0x2854, 0x28b4, 0x2914,
	0x2974, 0x29d4, 0x2a34, 0x2a94, 0x0814, 0x0874, 0x08d4, 0x0934, 0x0994, 0x2af4,
	0x2b54, 0x2bb4, 0x2c14, 0x2c74, 0x2cd4, 0x2d34, 0x2d94, 0x2df4, 0x2e54, 0x2eb4,
	0x2f14, 0x2f74, 0x2fd4, 0x3034, 0x3094, 0x30f4, 0x3154, 0x31b4, 0x3214, 0x3274,
	0x32d4, 0x3334, 0x3394, 0x33f4, 0x3454,
};
*/
//Tim 0324
//==============================================================================
UINT32 code tNTSC_AIR_FREQ[] =
{
#if 0
    552500,
    552500,
    552500,// Channel start
    612500,
    672500,
    772500,
    832500,
    1752500,                
    1812500,                
    1872500,                
    1932500,                
    1992500,                
    2052500,                
    2112500,                
    4712500,                
    4772500,                
    4832500,                
    4892500,                
    4952500,                
    5012500,                
    5072500,                
    5132500,                
    5192500,                
    5252500,                
    5312500,                
    5372500,                
    5432500,                
    5492500,                
    5552500,                
    5612500,                
    5672500,                
    5732500,                
    5792500,                
    5852500,                
    5912500,                
    5972500,                
    6032500,                
    6092500,                
    6152500,                
    6212500,                
    6272500,                
    6332500,                
    6392500,                
    6452500,                
    6512500,                
    6572500,                
    6632500,                
    6692500,                
    6752500,                
    6812500,                
    6872500,                
    6932500,                
    6992500,                
    7052500,                
    7112500,                
    7172500,                
    7232500,                
    7292500,                
    7352500,                
    7412500,                
    7472500,                
    7532500,                
    7592500,                
    7652500,                
    7712500,                
    7772500,                
    7832500,                
    7892500,                
    7952500,                
    8012500,
#else
	 322500, 
	 322500, 
	 552500, 
	 612500, 
	 672500, // 000 - 004
	 772500, 
	 832500, 
	1752500, 
	1812500, 
	1872500, // 005 - 009
	1932500, 
	1992500, 
	2052500, 
	2112500, 
	4712500, // 010 - 014
	4772500, 
	4832500, 
	4892500, 
	4952500, 
	5012500, // 015 - 019
	5072500, 
	5132500, 
	5192500, 
	5252500, 
	5312500, // 020 - 024
	5372500, 
	5432500, 
	5492500, 
	5552500, 
	5612500, // 025 - 029
	5672500, 
	5732500, 
	5792500, 
	5852500, 
	5912500, // 030 - 034
	5972500, 
	6032500, 
	6092500, 
	6152500, 
	6212500, // 035 - 039
	6272500, 
	6332500, 
	6392500, 
	6452500, 
	6512500, // 040 - 044
	6572500, 
	6632500, 
	6692500, 
	6752500, 
	6812500, // 045 - 049
	6872500, 
	6932500, 
	6992500, 
	7052500, 
	7112500, // 050 - 054
	7172500, 
	7232500, 
	7292500, 
	7352500, 
	7412500, // 055 - 059
	7472500, 
	7532500, 
	7592500, 
	7652500, 
	7712500, // 060 - 064
	7772500, 
	7832500, 
	7892500, 
	7952500, 
	8012500, // 065 - 069

#endif

};

//==============================================================================
UINT32 code tNTSC_CATV_FREQ[] =
{
#if 0
	 552500,
	 782500,
	 1092500,
	 1422500,
	 2242500,
	 2862500,
	 3362500,
	4122500,
	4652500,
	5032500,
	5612500,
	6052500,
	6872500,
	7242500,
	7862500,
	552500,
	1752500,
	2452500,
	3252500,
	4472500,
	5352500,
	6472500,
	7552500,
	8252500,
	1692500,
	2172500,
	2232500,
	2292500,
	2352500,
	2412500,
	2472500,
	2532500,
	2592500,
	2652500,
	2712500,
	2772500,
	2832500,
	2892500,
	2952500,
	3012500,
	3072500,
	3132500,
	3192500,
	3252500,
	3312500,
	3372500,
	3432500,
	3492500,
	3552500,
	3612500,
	3672500,
	3732500,
	3792500,
	3852500,
	3912500,
	3972500,
	4032500,
	4092500,
	4152500,
	4212500,
	4272500,
	4332500,
	4392500,
	4452500,
	4512500,
	4572500,
	4632500,
	4692500,
	4752500,
	4812500,
	4872500,
	4932500,
	4992500,
	5052500,
	5112500,
	5172500,
	5232500,
	5292500,
	5352500,
	5412500,
	5472500,
	5532500,
	5592500,
	5652500,
	5712500,
	5772500,
	5832500,
	5892500,
	5952500,
	6012500,
	6072500,
	6132500,
	6192500,
	6252500,
	6312500,
	6372500,
	6432500,
	 912500,
	 972500,
	1032500,
	1092500,
	1152500,
	6492500,
	6552500,
	6612500,
	6672500,
	6732500,
	6792500,
	6852500,
	6912500,
	6972500,
	7032500,
	7092500,
	7152500,
	7212500,
	7272500,
	7332500,
	7392500,
	7452500,
	7512500,
	7572500,
	7632500,
	7692500,
	7752500,
	7812500,
	7872500,
	7932500,
	7992500,
#else
	732500, 
	732500, 
	552500, 
	612500, 
	672500, // 000 - 004
	772500, 
	832500, 
	1752500, 
	1812500, 
	1872500, // 005 - 009
	1932500, 
	1992500, 
	2052500, 
	2112500, 
	1212500, // 010 - 014
	1272500, 
	1332500, 
	1392500, 
	1452500, 
	1512500, // 015 - 019
	1572500, 
	1632500, 
	1692500, 
	2172500, 
	2232500, // 020 - 024
	2292500, 
	2352500, 
	2412500, 
	2472500, 
	2532500, // 025 - 029
	2592500, 
	2652500, 
	2712500, 
	2772500, 
	2832500, // 030 - 034
	2892500, 
	2952500, 
	3012500, 
	3072500, 
	3132500, // 035 - 039
	3192500, 
	3252500, 
	3312500, 
	3372500, 
	3432500, // 040 - 044
	3492500, 
	3552500, 
	3612500, 
	3672500, 
	3732500, // 045 - 049
	3792500, 
	3852500, 
	3912500, 
	3972500, 
	4032500, // 050 - 054
	4092500, 
	4152500, 
	4212500, 
	4272500, 
	4332500, // 055 - 059
	4392500, 
	4452500, 
	4512500, 
	4572500, 
	4632500, // 060 - 064
	4692500, 
	4752500, 
	4812500, 
	4872500, 
	4932500, // 065 - 069
	4992500, 
	5052500, 
	5112500, 
	5172500, 
	5232500, // 070 - 074
	5292500, 
	5352500, 
	5412500, 
	5472500, 
	5532500, // 075 - 079
	5592500, 
	5652500, 
	5712500, 
	5772500, 
	5832500, // 080 - 084
	5892500, 
	5952500, 
	6012500, 
	6072500, 
	6132500, // 085 - 089
	6192500, 
	6252500, 
	6312500, 
	6372500, 
	6432500, // 090 - 094
	912500 ,
	972500 ,
	1032500, 
	1092500, 
	1152500,    // 095 - 099
	6492500, 
	6552500, 
	6612500, 
	6672500, 
	6732500, // 100 - 104
	6792500, 
	6852500, 
	6912500, 
	6972500, 
	7032500, // 105 - 109
	7092500, 
	7152500, 
	7212500, 
	7272500, 
	7332500, // 110 - 114
	7392500, 
	7452500, 
	7512500, 
	7572500, 
	7632500, // 115 - 119
	7692500, 
	7752500, 
	7812500, 
	7872500, 
	7932500, // 120 - 124                                                          // 125
	7992500, 


#endif
};
UINT8 code ucNtsc_CATV_Freq_Tbl_Count = sizeof(tNTSC_CATV_FREQ) / sizeof(UINT32);
UINT8 code ucNtsc_AIR_Freq_Tbl_Count = sizeof(tNTSC_AIR_FREQ) / sizeof(UINT32);

UINT8 xdata ucTableProgress;
#ifdef CONFIG_CHANNEL_TABLE_SHIFT_SCAN
UINT8 xdata ucTableProgress_freqshift;
#endif

extern bool g_next_time_dont_check_variance;// -> no existed extern bool next_time_dont_check_variance;
/*void CLoadTableProc(void)
{
#if 0
	UINT8 i;
	UINT8 ChnStart;

    StructUserDataChannelSettingsType xdata stTVScanData;
    UINT8  Count;// = ucNtsc_CATV_Freq_Tbl_Count;//sizeof(tNTSC_AIR_FREQ) / sizeof(UINT32);
    ChnStart = (GET_TV_CUR_NtscType() ? 1 : 2);
    ucTableFlag = 1;

	if(GET_TV_COUNTRY())//Japan
        Count = (GET_TV_CUR_NtscType() ? ucNtsc_JAPAN_AIR_Freq_Tbl_Count : ucNtsc_JAPAN_CATV_Freq_Tbl_Count);
	else
		Count = (GET_TV_CUR_NtscType() ? ucNtsc_AIR_Freq_Tbl_Count : ucNtsc_CATV_Freq_Tbl_Count);
	for(i = ChnStart;i < Count;i++)
	{
	
		if(GET_TV_COUNTRY())//Japan
			tvScanCurFreq = (GET_TV_CUR_NtscType() ? tNTSC_JAPAN_AIR_FREQ[i] : tNTSC_JAPAN_CATV_FREQ[i]);
		else
			tvScanCurFreq = (GET_TV_CUR_NtscType() ? tNTSC_AIR_FREQ[i] : tNTSC_CATV_FREQ[i]);
		//tvScanCurFreq = 1812500;
        //CSaveChannelData((_GET_TV_TYPE() == _TV_CATV) ? tNTSC_CATV[i] : tNTSC_AIR[i], 0x00, i);
	    //tvScanCurFreq = tNTSC_AIR_FREQ[ucTableProgress];
		RTD_Log(LOGGER_DEBUG,"\n Freq = %d \n",(UINT32)tvScanCurFreq);
	    tvScanCurChn = i;
		RTD_Log(LOGGER_INFO,"\n i = %d \n",(UINT32)tvScanCurChn);
	    stTVScanData.chnNo = tvScanCurChn;
	    stTVScanData.colorSystem = ZNTSC;
	    stTVScanData.freq = tvScanCurFreq;
	    stTVScanData.offset = 24;
	    stTVScanData.status = _CHN_STS_LOCK;
	    stTVScanData.soundSystem = ATV_SOUND_STD_A2_M;//CTvScanGetAudioStd(ZNTSC);
	    stTVScanData.name[0] = _C_;
	    stTVScanData.name[1] = _MINUS_;
	    stTVScanData.name[2] = (tvScanCurChn / 100) % 10 + _0_;
	    stTVScanData.name[3] = (tvScanCurChn / 10) % 10 + _0_;
	    stTVScanData.name[4] = tvScanCurChn % 10 + _0_;
	    //CTStorage.StorageLastUpdate = 0xff;
        SYNC_STORAGE_ARRAY_STRUCT_DATA(StructUserDataChannelSettingsType, tvScanCurChn - 1, stTVScanData);
	}
#else
    UINT8  i,Count;// = ucNtsc_CATV_Freq_Tbl_Count;//sizeof(tNTSC_AIR_FREQ) / sizeof(UINT32);

	Count = (GET_TV_CUR_NtscType() ? ucNtsc_AIR_Freq_Tbl_Count : ucNtsc_CATV_Freq_Tbl_Count);


	for(i = 0;i<Count;i++)
	{
	    SET_CHANNEL_FREQ(i, tNTSC_CATV_FREQ[i]);
	    RTD_Log(LOGGER_LEVEL,"\n tNTSC_CATV_FREQ[i] = %d \n",(UINT32)tNTSC_CATV_FREQ[i]);
	}

#endif
}*/

#ifdef CONFIG_ENABLE_TV_NOISE_BOX_ENALBE
UINT8 Auto_Scan_videoislocked_Tv_noise_box()
{
    UINT8  times=10;

    do
    {
        fw_timer_event_DelayXms(100);
        if (drv_video_vdc_CheckHVisLocked())
            return _TRUE;
    }
    while (--times);
    return _FAIL;
}
#endif

#ifdef CONFIG_ENABLE_TV_NOISE_BOX_ENALBE
void Tv_noise_box_for_setchannel(UINT8 chn)
{
    if (auto_scan_avout_for_noise_box_enable == 1)
    {
        drvif_module_vdc_noisebox_avoutfunction(1);

        if (GET_NoiseBoxStatus(1) == 1)
        {
            RTD_Log(LOGGER_DEBUG, "\n a____1__noise =1_________ sound sys=%x \n", GET_CHANNEL_COLORSYSTEM(chn));
            RTD_Log(LOGGER_DEBUG, "\n a____1__noise =1_________clorsye =%x \n",GET_CHANNEL_COLORSYSTEM(chn));

            if (COsdCtrlGetSoundSystem()!=_SOUND_DK|| GET_CHANNEL_COLORSYSTEM(chn)!=ZPAL_I)
            {
                SET_CHANNEL_COLORSYSTEM(chn,ZPAL_I);
                SET_CHANNEL_SOUNDSYSTEM(chn,ATV_SOUND_STD_DK_MONO);
                RTD_Log(LOGGER_DEBUG, "\n a______reset d/k__1_______ sound sys=%x \n",GET_CHANNEL_SOUNDSYSTEM(chn));
            }
        }
        auto_scan_avout_for_noise_box_enable=0;
    }
    else
    {
        RTD_Log(LOGGER_DEBUG, "\n auto_scan_avout_for_noise_box_enable=0 \n");
        RTD_Log(LOGGER_DEBUG, "GET_NoiseBoxStatus= %x\n",GET_NoiseBoxStatus(1));
        if (GET_NoiseBoxStatus(1) == 1)
        {
            //module_vdc_AVoutMode(VDC_AVOUT_TV_NOISE_BOX);
            drv_video_vdc_AvOutMode(VDC_AVOUT1, VDC_AVOUT_CVBS_NO_FIL_NO_AGC)
            RTD_Log(LOGGER_DEBUG, "\n the avout for tv noise box \n");
        }
        else
        {
            //module_vdc_AVoutMode(VDC_AVOUT_CVBS_WITH_FIL_WITH_AGC);
            drv_video_vdc_AvOutMode(VDC_AVOUT1, VDC_AVOUT_CVBS_WITH_FIL_WITH_AGC)
            RTD_Log(LOGGER_DEBUG, "\n the avout is normail tv av out \n");
        }
    }
}
#endif

// Initialize parameter when boot
void fw_tv_system_Init(void)
{
    usAfcCurFreq = 0;
    tvAutoScanFlag = 0;
    tvManualScanFlag = 0;
    ucAfcDirection = 0;
    AFC_stage_count = 0;
#ifdef CONFIG_ENABLE_TV_NOISE_BOX_ENALBE
    auto_scan_avout_for_noise_box_enable = 0;
#endif
}
#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
//-------------------------------------------------------------
// Local Function
//-------------------------------------------------------------
UINT8 fwif_tv_autoscan_check_new_vlock(UINT8 check_count)
{
    UINT8 new_v_lock_status;
    UINT8 i;

    if (check_count <= 0)
        return _FALSE;

    for (i=0; i<check_count; i++)
    {
        new_v_lock_status = drvif_module_vdc_New_Vlock();
        RTD_Log(LOGGER_DEBUG,"TV AUTO SCAN mode is ,the new v lock=... %x\n", (UINT32)new_v_lock_status);

        if (new_v_lock_status == 1)
            return _TRUE;

        if (i < (check_count - 1))
            fw_timer_event_DelayXms(300);
    }

    return _FALSE;
}
#endif
#ifdef CONFIG_IFD_ENABLE
#if 0
UINT8 fwif_tv_single_scan_PAL_N_funtion(UINT32 tvAutoScanFreq)
{
    UINT8 AScan_Noise_stas;
    UINT8 flag358;
    UINT16 Ratio;
    INT32 offset;
    UINT8 Status;

  // RTD_Log(LOGGER_DEBUG,"tuner set frq %d\n",(UINT32) tvAutoScanFreq);
    drvif_ifd_set_iftuner_setting(ZPAL_N, (UINT32)tvAutoScanFreq, 5);	//suggest delay here should > 5
	if(GET_TV_TABLE_SCAN()) 
		fw_timer_event_DelayXms(150);

    Ratio = drv_tuner_GetRatioInStepSize();
    offset = drvif_tuner_get_afcoffset(0);
    //RTD_Log(LOGGER_LEVEL,"\n Tim Ratio= %d,offset = %d \n",(UINT32)Ratio,(UINT32)offset);
	if (( drvif_ifd_get_cr_lock() == _TRUE )&&((UINT32)(abs1(offset)) <= (UINT32)(10*Ratio)))//Tim 0402 //Tim 0404
    {
        //RTD_Log(LOGGER_DEBUG,"\nIFD_CRLock %d\n",(UINT32) tvAutoScanFreq);
        //VDC initial code ++
		#if defined(CONFIG_CHINA_AUTO_SCAN_FLOW)
        /*SECAM FLAG SETTING*/
        rtdf_outl(VDC_FREQ_VALID_INNER_0x5220,0x42);
        rtdf_outl(VDC_SECAM_FLAG_COUNTER_MAX_0x5228,0xA0);
        rtdf_outl(VDC_SECAM_FLAG_COUNTER_MIN_0x522c,0x0D);
        rtdf_outl(VDC_AUTO_MODE_S_M_STATISTIC_0x51d8,0x0B);
       // RTD_Log(LOGGER_INFO,"\n[autoscan PAL 1] 51d8=0x0B \n");
		#endif
        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000006);
        //VDC initial code --
        //audio initial code
        drvif_ifd_set_audio_bp(_SIG_MODE_AUTO);
		/*
		#if 1
			fw_timer_event_DelayXms(110);
			Status = (UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
			if(Status & _No_Sig)
			{
				if((Status&0x06)==0x00) // h&v unlock,prvent weak signal
					return  ZMODE_ERROR;
			}
			fw_timer_event_DelayXms(200);
		#else
			fw_timer_event_DelayXms(400);
		#endif
		*/
        fw_timer_event_DelayXms(20);
        if ( !drvif_ifd_get_agc_lock() )
			return 0;//ZCR_NOT_LOCK;
		RTD_Log(LOGGER_LEVEL,"\n Tim agc lock \n");
        fw_timer_event_DelayXms(20);
        Status = (UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
        if(Status & _No_Sig)
        {
            if((Status&0x06)==0x00) // h&v unlock,prvent weak signal
                return  0;//ZCR_NOT_LOCK;
        }

		fw_timer_event_DelayXms(140);
		
        RTD_Log(LOGGER_DEBUG,"tuner set frq %d\n",(UINT32) tvAutoScanFreq);
        //CHECKLSADCKEY();
        //RTD_Log(LOGGER_DEBUG, "AGC LOCK  FLAG2 %x \n", rtdf_inl(IFD_AGC_LOCK_STATUS_VADDR));
        RTD_Log(LOGGER_DEBUG, "RF AGC  %x \n", (UINT32)(rtdf_inl(IFD_AGC_RF_STATUS_VADDR)));
        RTD_Log(LOGGER_DEBUG, "IF PGA  %x \n", (UINT32)(rtdf_inl(IFD_READ_PGA_VADDR)));
        RTD_Log(LOGGER_DEBUG, "IF AGC  %x \n", (UINT32)(rtdf_inl(IFD_AGC_IF_STATUS_VADDR)));
        RTD_Log(LOGGER_DEBUG, "clamp offset  %x \n", (UINT32)(rtdf_inl(IFD_OFFSET_VADDR)));
        RTD_Log(LOGGER_DEBUG, "dgain status 2 %x \n", (UINT32)(rtdf_inl(VDC_AGC_DGAIN_STATUS2_0x5260)));
        RTD_Log(LOGGER_DEBUG, "dgain status 1 %x \n", (UINT32)(rtdf_inl(VDC_AGC_DGAIN_STATUS1_0x5264)));
        RTD_Log(LOGGER_DEBUG, "pal tuner is lock\n");
        RTD_Log(LOGGER_DEBUG, "vd status = %x\n", (UINT32)(rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008)));

        if (drv_video_vdc_CheckHVisLocked() )
        {
            RTD_Log(LOGGER_DEBUG,"\n Pal N tuner and vd is lock ok %d\n", (UINT32) tvAutoScanFreq);
            
            ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
            if ( (ucVdc_TV_Mode==ZPAL_N))//Tim For Low Level Lock//&&(AScan_Noise_stas<=0x06))
		    {
				AScan_Noise_stas = drv_video_vdc_NoiseStatus();
				RTD_Log(LOGGER_DEBUG,"the pal n noise  ... %x\n", (UINT32) AScan_Noise_stas);
            
		        flag358=rtdf_inl(VDC_VIDEO_STATUS_REG2_0x500c)&0x80;
		        RTD_Log(LOGGER_DEBUG,"the vd mode is pal n the flag358=%x ??????........... \n",(UINT32) flag358);
		        if (flag358==0x80)
		        {		        	
                    if ((AScan_Noise_stas<0xa5))//&&(pal_audio_detect==1))//0x70
		            return ZPAL_N;
		        }
		    }
			
            return  0;//ZMODE_ERROR;
        }
        else
        {
            return 0;//ZHV_NOT_LOCK;
        }
    }
    else
    {
    	//RTD_Log(LOGGER_LEVEL,"\n Tim not lock\n");
        return 0;//ZCR_NOT_LOCK;
    }
}

UINT8 fwif_tv_autoscan_PAL_N_funtion(UINT32 tvAutoScanFreq)
{
    UINT8 mn_status;
    UINT8 pal_audio_detect = 0;
    UINT8 AScan_Noise_stas;
    UINT8 RetryCntPAL = 0;
    UINT8 RetryCnt358;
    UINT8 color_detect;
    UINT8 afc;
    UINT8 lock = 0;
    UINT8 new_v_lock;
    UINT8 flag358;
    UINT32 secamdk_tvScanCurFreq;
    UINT8 cnt_358;
    UINT16 Ratio;
    INT32 offset;
// linmc20091229, help sky_yeh to add auto scan enhance function
#ifdef CONFIG_EUROPE_AUTO_SCAN_FLOW
    UINT8 pal_new_v_lock_count=0;
    UINT32 average_burst;
    UINT32 secam_flag;
#endif
    UINT8 Status;

    RetryCntPAL = 0;
    color_detect=0;
    new_v_lock=0;
    afc=0;
    RetryCnt358 = 0;
    cnt_358 = 0;


  // RTD_Log(LOGGER_DEBUG,"tuner set frq %d\n",(UINT32) tvAutoScanFreq);
    drvif_ifd_set_iftuner_setting(ZPAL_N, (UINT32)tvAutoScanFreq, 5);	//suggest delay here should > 5
	if(GET_TV_TABLE_SCAN()) 
		fw_timer_event_DelayXms(150);

    Ratio = drv_tuner_GetRatioInStepSize();
    offset = drvif_tuner_get_afcoffset(0);
    //RTD_Log(LOGGER_LEVEL,"\n Tim Ratio= %d,offset = %d \n",(UINT32)Ratio,(UINT32)offset);
	if (( drvif_ifd_get_cr_lock() == _TRUE )&&((UINT32)(abs1(offset)) <= (UINT32)(10*Ratio)))//Tim 0402 //Tim 0404
    {
        //RTD_Log(LOGGER_DEBUG,"\nIFD_CRLock %d\n",(UINT32) tvAutoScanFreq);
        //VDC initial code ++
#if defined(CONFIG_CHINA_AUTO_SCAN_FLOW)
        /*SECAM FLAG SETTING*/
        rtdf_outl(VDC_FREQ_VALID_INNER_0x5220,0x42);
        rtdf_outl(VDC_SECAM_FLAG_COUNTER_MAX_0x5228,0xA0);
        rtdf_outl(VDC_SECAM_FLAG_COUNTER_MIN_0x522c,0x0D);
        rtdf_outl(VDC_AUTO_MODE_S_M_STATISTIC_0x51d8,0x0B);
       // RTD_Log(LOGGER_INFO,"\n[autoscan PAL 1] 51d8=0x0B \n");
#endif
// linmc20091229, help sky_yeh to add auto scan enhance function
#ifdef CONFIG_EUROPE_AUTO_SCAN_FLOW
        rtdf_maskl(VDC_VSYNC_TIME_CONST_0x515c,0x00, 0x0a);
        rtdf_outl(VDC_VSYNC_TIME_CONST2_0x5160, 0x04);
        rtdf_outl(VDC_VSYNC_TIME_CONST1_0x5164, 0x20);
        drvif_module_vdc_Set_Hstate(VDC_HSTATE_RESET);
        drvif_module_vdc_Set_Cstate(VDC_NOT_FIX_CSTATE2);
#endif
#if defined(CONFIG_PROJECT_TV020)||defined(CONFIG_PROJECT_TV024_2)
        rtdf_outl(VDC_NO_BURST_MAG_FILTER_TH_0x5688,0x00);
        rtdf_outl(VDC_AUTO_MODE_S_M_STATISTIC_0x51d8,0x24);
        RTD_Log(LOGGER_INFO,"\n[autoscan PAL 2] 51d8=0x24 \n");
        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000007);
        rtdf_outl(VDC_BURST_GATE_START_0x5130,0x32);
        rtdf_outl(VDC_BURST_GATE_END_0x5134,0x46);
        rtdf_maskl(VDC_CHROMA_AUTOPOS_0x50a4, ~_BIT5, _BIT5);
        rtdf_maskl(VDC_CHROMA_AUTOPOS_0x50a4, 0xe0, 0x0c);
#else
        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000006);
#endif
        //VDC initial code --
        //audio initial code
        drvif_ifd_set_audio_bp(_SIG_MODE_AUTO);

		/*
		#if 1
			fw_timer_event_DelayXms(110);
			Status = (UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
			if(Status & _No_Sig)
			{
				if((Status&0x06)==0x00) // h&v unlock,prvent weak signal
					return  ZMODE_ERROR;
			}
			fw_timer_event_DelayXms(200);
		#else
			fw_timer_event_DelayXms(400);
		#endif
		*/
		fw_timer_event_DelayXms(20);
        if ( !drvif_ifd_get_agc_lock() )
			return ZCR_NOT_LOCK;
		RTD_Log(LOGGER_LEVEL,"\n Tim agc lock \n");
        fw_timer_event_DelayXms(20);
        Status = (UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
        if(Status & _No_Sig)
        {
            if((Status&0x06)==0x00) // h&v unlock,prvent weak signal
                return  ZCR_NOT_LOCK;//ZCR_NOT_LOCK;
        }
		
		fw_timer_event_DelayXms(140);
		
        RTD_Log(LOGGER_DEBUG,"tuner set frq %d\n",(UINT32) tvAutoScanFreq);
        //CHECKLSADCKEY();
        //RTD_Log(LOGGER_DEBUG, "AGC LOCK  FLAG2 %x \n", rtdf_inl(IFD_AGC_LOCK_STATUS_VADDR));
        RTD_Log(LOGGER_DEBUG, "RF AGC  %x \n", (UINT32)(rtdf_inl(IFD_AGC_RF_STATUS_VADDR)));
        RTD_Log(LOGGER_DEBUG, "IF PGA  %x \n", (UINT32)(rtdf_inl(IFD_READ_PGA_VADDR)));
        RTD_Log(LOGGER_DEBUG, "IF AGC  %x \n", (UINT32)(rtdf_inl(IFD_AGC_IF_STATUS_VADDR)));
        RTD_Log(LOGGER_DEBUG, "clamp offset  %x \n", (UINT32)(rtdf_inl(IFD_OFFSET_VADDR)));
        RTD_Log(LOGGER_DEBUG, "dgain status 2 %x \n", (UINT32)(rtdf_inl(VDC_AGC_DGAIN_STATUS2_0x5260)));
        RTD_Log(LOGGER_DEBUG, "dgain status 1 %x \n", (UINT32)(rtdf_inl(VDC_AGC_DGAIN_STATUS1_0x5264)));
        RTD_Log(LOGGER_DEBUG, "pal tuner is lock\n");
        RTD_Log(LOGGER_DEBUG, "vd status = %x\n", (UINT32)(rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008)));

        //CHECKLSADCKEY();
        if (drv_video_vdc_CheckHVisLocked() )
        {
            lock=1;
            RTD_Log(LOGGER_DEBUG,"tuner and vd is lock ok %d\n", (UINT32) tvAutoScanFreq);
            RetryCntPAL =1;
            do
            {
                ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
                RTD_Log(LOGGER_DEBUG,"TV AUTO SCAN mode is NTSC443 or SECAM 1=... %x\n", (UINT32)ucVdc_TV_Mode);
                AScan_Noise_stas = drv_video_vdc_NoiseStatus();
                AScan_Noise_stas1 = AScan_Noise_stas;
                RTD_Log(LOGGER_DEBUG,"TV AUTO SCAN mode is NTSC443 or SECAM noise ... %x\n",(UINT32) AScan_Noise_stas);
                RTD_Log(LOGGER_DEBUG, "VDC_VIDEO_STATUS_REG2_VADDR = %x\n", (UINT32) (rtdf_inl(VDC_VIDEO_STATUS_REG2_0x500c)));
                RTD_Log(LOGGER_DEBUG, "VDC_VIDEO_STATUS_REG3_VADDR = %x\n", (UINT32) (rtdf_inl(VDC_VIDEO_STATUS_REG3_0x5010)));
#if defined(CONFIG_CHINA_AUTO_SCAN_FLOW)

                if ( ucVdc_TV_Mode == ZSECAM )
                {
                    fw_timer_event_DelayXms (500);
                    ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
                    RTD_Log(LOGGER_DEBUG,"TV AUTO SCAN mode is NTSC443 or SECAM 2= ... %x\n", (UINT32)ucVdc_TV_Mode);
                }
#endif
                // linmc20091229, help sky_yeh to add auto scan enhance function
#ifdef CONFIG_EUROPE_AUTO_SCAN_FLOW
                if (( ucVdc_TV_Mode == ZSECAM )&&(AScan_Noise_stas<0x10))
                {

                    fw_timer_event_DelayXms(100);
                    color_detect=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
                    if ((color_detect&0x08)== 0x00)
                    {
                        fw_timer_event_DelayXms(100);
                        ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();

                    }
                }
#endif

                if ( ucVdc_TV_Mode == ZNTSC )
                {
#ifdef CONFIG_EUROPE_AUTO_SCAN_FLOW
                    return ZNTSC;
#else
                    return ZMODE_525_STD;
#endif
                }
#ifdef CONFIG_EUROPE_AUTO_SCAN_FLOW
                else if (( ucVdc_TV_Mode == ZNTSC_443 )&&(AScan_Noise_stas<=0x06))
#else
                else if ( ucVdc_TV_Mode == ZNTSC_443 )
#endif
                {
                    //CHECKLSADCKEY();
#ifndef CONFIG_TUNER_SILICON
                    fw_timer_event_DelayXms(200); // wait the color of vd and audio are stable
                    //frank@1102 in front have 400ms dalay	ScalerTimer_DelayXms(200);  // wait the color of vd and audio are stable
                    color_detect = (UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
                    RTD_Log(LOGGER_DEBUG,"VDC_VIDEO_STATUS_REG1_VADDR=%x ??????........... \n", (UINT32) color_detect);
                    if ((color_detect & 0x08) == 0x08)
                    {
#ifdef _VDC_DETECT_358_FLAG
                        flag358=rtdf_inl(VDC_VIDEO_STATUS_REG2_0x500c)&0x80;
                        RTD_Log(LOGGER_DEBUG,"the vd flag358=%x ??????........... \n", (UINT32) flag358);
                        if (flag358 != 0x80)
                            return ZNTSC_443;
#else //#ifdef _VDC_DETECT_358_FLAG
                        Audio_AtvGetMNStatus(&mn_status, NULL);

                        if (mn_status == 0)
                        {
                            return ZNTSC_443;
                        }
                        else
                            return ZMODE_525_STD;
#endif //#ifdef _VDC_DETECT_358_FLAG
                    }
                    else
                        return ZMODE_525_STD;
#else //CONFIG_TUNER_SILICON
                    return ZMODE_525_STD;
#endif //CONFIG_TUNER_SILICON
                }
                else if ( (ucVdc_TV_Mode==ZPAL_N))//Tim For Low Level Lock//&&(AScan_Noise_stas<=0x06))
                {
#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
                    if (_TRUE == fwif_tv_autoscan_check_new_vlock(2))
                        return ZPAL_N;
#else
                    flag358=rtdf_inl(VDC_VIDEO_STATUS_REG2_0x500c)&0x80;
                    RTD_Log(LOGGER_DEBUG,"the vd mode is pal n the flag358=%x ??????........... \n",(UINT32) flag358);
                    if (flag358==0x80)
                        return ZPAL_N;
#endif
                }
#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
                else if ( (ucVdc_TV_Mode==ZSECAM)&&(AScan_Noise_stas<=0xa0))
#else
                else if ( (ucVdc_TV_Mode==ZSECAM)&&(AScan_Noise_stas<=0x1F))
#endif
                {
#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
                    if (_TRUE == fwif_tv_autoscan_check_new_vlock(2))
                    {
                        // linmc20091229, help sky_yeh to add auto scan enhance function
#ifdef CONFIG_EUROPE_AUTO_SCAN_FLOW
#if defined(CONFIG_PROJECT_TV020)||defined(CONFIG_PROJECT_TV024_2)
                        rtdf_outl(VDC_BURST_GATE_START_0x5130,0x50);
                        rtdf_outl(VDC_BURST_GATE_END_0x5134,0x64);
                        fw_timer_event_DelayXms(40);
#endif
                        average_burst=drvif_module_vdc_AVG_BURST_MAG_STATUS();
                        secam_flag=rtdf_inl(VDC_VIDEO_STATUS_REG3_0x5010)&0x02;
                        RTD_Log(LOGGER_DEBUG,"the secam detect ,average_burst=%x ??????........... \n", (UINT32) average_burst);
                        RTD_Log(LOGGER_DEBUG,"the secam detect ,secam_flag=%x ??????........... \n", (UINT32) secam_flag);
                        RTD_Log(LOGGER_DEBUG,"the secam detect ,AScan_Noise_stas=%x ??????........... \n", (UINT32) AScan_Noise_stas);
                        if ((average_burst<0x08)&&(secam_flag==0x00)&&(AScan_Noise_stas<0x15))
                            return ZPAL_I;
                        else
                        {
                            if ((secam_flag==0x00)&&(AScan_Noise_stas<0x20))
                            {
                                RTD_Log(LOGGER_DEBUG,"the pal i detect  the secam flag is off\n");
                                return ZPAL_I;
                            }
                            else
                            {
                                if ((secam_flag==0x02)&&(AScan_Noise_stas<0x30))
                                {
                                    RTD_Log(LOGGER_DEBUG,"the secam  detect  the secam flag is on\n");
                                    return ZSECAM;
                                }
                            }
                        }
#else
                        return ZSECAM;
#endif
                    }
#else //#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
                    return ZSECAM;
#endif //#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)

#if defined(CONFIG_PROJECT_TV020)||defined(CONFIG_PROJECT_TV024_2)
                    rtdf_outl(VDC_BURST_GATE_START_0x5130,0x32);
                    rtdf_outl(VDC_BURST_GATE_END_0x5134,0x46);

#endif
                }
                /*
                				drvif_ifd_set_iftuner_setting(ZPAL_I,tvAutoScanFreq,300);
                				ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
                				RTD_Log(LOGGER_DEBUG,"TV AUTO SCAN mode is  ... %x\n", (UINT32) ucVdc_TV_Mode);
                				AScan_Noise_stas = drv_video_vdc_NoiseStatus();
                				RTD_Log(LOGGER_DEBUG,"the pal noise  ... %x\n", (UINT32) AScan_Noise_stas);
                //				new_v_lock=drvif_module_vdc_New_Vlock();
                //				RTD_Log(LOGGER_DEBUG,"secam TV AUTO SCAN mode is ,the new v lock=... %x\n",new_v_lock);
                */
                else if ( ucVdc_TV_Mode==ZPAL_I )
                {
                    rtdf_outl(VDC_MANUAL_MODE_0x51c0, 0x00000006);
                    AScan_Noise_stas = drv_video_vdc_NoiseStatus();
                    RTD_Log(LOGGER_DEBUG,"the pal noise  ... %x\n", (UINT32) AScan_Noise_stas);
#ifdef TV_PAL_NTSC_TH
                    if (AScan_Noise_stas > 0xb6)
                        break;
#endif //#ifdef TV_PAL_NTSC_TH

#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
#if 0//tommy delete 20100830 for prevent adjust error when pal
                    if (_FALSE == fwif_tv_autoscan_check_new_vlock(2))
                    {
                        // linmc20091229, help sky_yeh to add auto scan enhance function
#ifdef CONFIG_EUROPE_AUTO_SCAN_FLOW
                        RTD_Log(LOGGER_DEBUG,"the pal detect but the soft  reset is on  ...\n");
                        drv_video_vdc_SoftReset();
                        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000006);
                        fw_timer_event_DelayXms(400);
                        if (AScan_Noise_stas<0x11)
                            pal_new_v_lock_count=5;
                        else
                            pal_new_v_lock_count=3;
                        if (_FALSE == fwif_tv_autoscan_check_new_vlock(pal_new_v_lock_count))
                        {
                            RTD_Log(LOGGER_DEBUG,"the pal detect but the soft  reset is on , the new vlock is not lock ...\n");
                            break;
                        }
#else
                        break;
#endif
                    }
#endif
#endif //#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)

                    Audio_AtvGetMNStatus(&mn_status, NULL);

                    if (mn_status == 0 )
                    {
                        pal_audio_detect=1;
                    }

                    if (AScan_Noise_stas >= 0x50 )
                    {
                        //CHECKLSADCKEY();
                        fw_timer_event_DelayXms(200); // wait noise of vd is stable
                        AScan_Noise_stas = drv_video_vdc_NoiseStatus();
                        RTD_Log(LOGGER_DEBUG,"the pal noise is too big so to do one  ... %x\n", (UINT32) AScan_Noise_stas);
                        cnt_358 = 0;
                        for (RetryCnt358 = 0; RetryCnt358 < 3; RetryCnt358++)
                        {
                            color_detect = (UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG2_0x500c);
                            RTD_Log(LOGGER_DEBUG,"VDC_VIDEO_STATUS_REG2_VADDR=%x ??????........... \n", (UINT32) color_detect);
                            if ((color_detect & 0x70) == 0x70)//0x90
                            {
                                cnt_358++;
                            }
                            fw_timer_event_DelayXms(5);
                        }
                        if (cnt_358 >=2)
                            return ZMODE_525_STD;
                    }

                    if ((AScan_Noise_stas<0xa5)&&(pal_audio_detect==1))//0x70
                    {
                        RTD_Log(LOGGER_DEBUG,"pal detect \n");
                        return ZPAL_I;
                    }
                }

#ifdef CONFIG_SUPPORT_PAL60
                else if ( ucVdc_TV_Mode == ZPAL_60 )
                {
                    //	CHECKLSADCKEY();
                    //frank@1102 in front have 400ms dalay	ScalerTimer_DelayXms(500);//wait the the audio  is stable
                    //CHECKLSADCKEY();
                    fw_timer_event_DelayXms(500);//wait the the v sync and h sync is lock
                    //CHECKLSADCKEY();

                    Audio_AtvGetMNStatus(&mn_status, NULL);

                    if (mn_status == 0 )
                    {
                        RTD_Log(LOGGER_DEBUG,"pal60 detect \n");
                        return  ZPAL_60;
                    }
                    else
                        return ZMODE_525_STD;
                }
#endif //#ifdef CONFIG_SUPPORT_PAL60

                else if ( (ucVdc_TV_Mode == ZNTSC_50) && ((AScan_Noise_stas = drv_video_vdc_NoiseStatus())<=0x05) )
                {
#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
                    if ( _TRUE == fwif_tv_autoscan_check_new_vlock(2))
                    {
                        RTD_Log(LOGGER_DEBUG,"ntsc50 detect \n");
                        return  ZPAL_I;
                    }
#else
                    RTD_Log(LOGGER_DEBUG,"ntsc50 detect \n");
                    return  ZPAL_I;
#endif
                }
                else if ( (ucVdc_TV_Mode==ZPAL_N) && ((AScan_Noise_stas = drv_video_vdc_NoiseStatus())<=0x13))
                {
#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
                    if ( _FALSE == fwif_tv_autoscan_check_new_vlock(2))
                        break;
#endif //#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
                    flag358=rtdf_inl(VDC_VIDEO_STATUS_REG2_0x500c)&0x80;
                    RTD_Log(LOGGER_DEBUG,"the vd mode is pal n the flag358=%x ??????........... \n", (UINT32) flag358);
                    if (flag358==0x80)
                    {
                        secamdk_tvScanCurFreq = tvAutoScanFreq +_TUNER_MANUAL_SCAN_MIN_SETP;
                        //					drvif_ifd_set_iftuner_setting(ZPAL_I,secamdk_tvScanCurFreq,165);
                        drvif_ifd_set_iftuner_setting(ZPAL_I,secamdk_tvScanCurFreq,20);
                        ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
                        AScan_Noise_stas = drv_video_vdc_NoiseStatus();
                        RTD_Log(LOGGER_DEBUG,"the tuner set pal the frq is +4  but may be the pal i  mode is ... %x\n", (UINT32) ucVdc_TV_Mode);
                        RTD_Log(LOGGER_DEBUG,"the tuner set pal but may be the pal n  ,the noise... %x\n", (UINT32)AScan_Noise_stas);

                        if (ucVdc_TV_Mode==ZPAL_I)
                        {
                            new_v_lock=drvif_module_vdc_New_Vlock();
                            RTD_Log(LOGGER_DEBUG,"he tuner set pal but may be the pal n ,the new v lock=... %x\n", (UINT32)new_v_lock);

                            if (1 == new_v_lock)
                            {
                                RTD_Log(LOGGER_DEBUG,"pal the frq +4 detect \n");
                                return  ZPAL_I;
                            }
                        }
                    }
                }
                else if (ucVdc_TV_Mode==ZPAL_M)
                {
                    return ZPAL_M;//ZMODE_525_STD; //Tim 0428
                }

            }
            while (--RetryCntPAL);

            return  ZMODE_ERROR;
        }
        else
        {
            return ZHV_NOT_LOCK;
        }
    }
    else
    {
    	//RTD_Log(LOGGER_LEVEL,"\n Tim not lock\n");
        return ZCR_NOT_LOCK;
    }
}
#endif

//Tim 0517
//==============================================================================
UINT8 fwif_tv_single_scan_PAL_funtion(UINT32 tvAutoScanFreq)
{
    UINT8 AScan_Noise_stas;
    UINT16 Ratio;
    INT32 offset;
    UINT8 Status;

   //RTD_Log(LOGGER_DEBUG,"tuner set frq123 %d\n",(UINT32) tvAutoScanFreq);
    drvif_ifd_set_iftuner_setting(ZPAL_I, (UINT32)tvAutoScanFreq, 5);	//suggest delay here should > 5
	if(GET_TV_TABLE_SCAN()) 
		fw_timer_event_DelayXms(150);

    Ratio = drv_tuner_GetRatioInStepSize();
    offset = drvif_tuner_get_afcoffset(0);
    //RTD_Log(LOGGER_LEVEL,"\n Tim Ratio= %d,offset = %d \n",(UINT32)Ratio,(UINT32)offset);
	if (( drvif_ifd_get_cr_lock() == _TRUE )&&((UINT32)(abs1(offset)) <= (UINT32)(10*Ratio)))//Tim 0402 //Tim 0404
    {
		#if defined(CONFIG_CHINA_AUTO_SCAN_FLOW)
        rtdf_outl(VDC_FREQ_VALID_INNER_0x5220,0x42);
        rtdf_outl(VDC_SECAM_FLAG_COUNTER_MAX_0x5228,0xA0);
        rtdf_outl(VDC_SECAM_FLAG_COUNTER_MIN_0x522c,0x0D);
        rtdf_outl(VDC_AUTO_MODE_S_M_STATISTIC_0x51d8,0x0B);
		#endif
        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000006);
        //VDC initial code --
        //audio initial code
        //drvif_ifd_set_audio_bp(_SIG_MODE_AUTO);
        fw_timer_event_DelayXms(20);
        if ( !drvif_ifd_get_agc_lock() )
			return 0;//ZCR_NOT_LOCK;
		RTD_Log(LOGGER_LEVEL,"\n Tim agc lock \n");
        fw_timer_event_DelayXms(20);
        Status = (UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
        if(Status & _No_Sig)
        {
            if((Status&0x06)==0x00) // h&v unlock,prvent weak signal
                return  0;//ZCR_NOT_LOCK;
        }

		fw_timer_event_DelayXms(140);
        RTD_Log(LOGGER_DEBUG,"tuner set frq %d\n",(UINT32) tvAutoScanFreq);

        RTD_Log(LOGGER_DEBUG, "RF AGC  %x \n", (UINT32)(rtdf_inl(IFD_AGC_RF_STATUS_VADDR)));
        RTD_Log(LOGGER_DEBUG, "IF PGA  %x \n", (UINT32)(rtdf_inl(IFD_READ_PGA_VADDR)));
        RTD_Log(LOGGER_DEBUG, "IF AGC  %x \n", (UINT32)(rtdf_inl(IFD_AGC_IF_STATUS_VADDR)));
        RTD_Log(LOGGER_DEBUG, "clamp offset  %x \n", (UINT32)(rtdf_inl(IFD_OFFSET_VADDR)));
        RTD_Log(LOGGER_DEBUG, "dgain status 2 %x \n", (UINT32)(rtdf_inl(VDC_AGC_DGAIN_STATUS2_0x5260)));
        RTD_Log(LOGGER_DEBUG, "dgain status 1 %x \n", (UINT32)(rtdf_inl(VDC_AGC_DGAIN_STATUS1_0x5264)));
        RTD_Log(LOGGER_DEBUG, "pal tuner is lock\n");
        RTD_Log(LOGGER_DEBUG, "vd status = %x\n", (UINT32)(rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008)));

        if (drv_video_vdc_CheckHVisLocked() )
        {
            RTD_Log(LOGGER_DEBUG,"tuner and vd is lock ok %d\n", (UINT32) tvAutoScanFreq);

            ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
			if(ucVdc_TV_Mode == ZPAL_I)
  			{
                rtdf_outl(VDC_MANUAL_MODE_0x51c0, 0x00000006);
                AScan_Noise_stas = drv_video_vdc_NoiseStatus();
                RTD_Log(LOGGER_DEBUG,"the pal noise  ... %x\n", (UINT32) AScan_Noise_stas);
                //if (AScan_Noise_stas > 0xb6)
                    //return 0;

                if ((AScan_Noise_stas<0xa5))//&&(pal_audio_detect==1))//0x70
                {
                    RTD_Log(LOGGER_DEBUG,"pal detect \n");
                    return ZPAL_I;
                }
                return 0;

  			}
  			//==================================================================
            else if ( (ucVdc_TV_Mode==ZSECAM))
            {
                if (_TRUE == fwif_tv_autoscan_check_new_vlock(2))
                    return ZSECAM;
			}//Edit SVN53
  			//==================================================================
			else
	            return 0;//ZPAL_I;

            return  0;//ZMODE_ERROR;
        }
        else
        {
            return 0;//ZHV_NOT_LOCK;
        }
    }
    else
    {
    	//RTD_Log(LOGGER_LEVEL,"\n Tim not lock\n");
        return 0;//ZCR_NOT_LOCK;
    }
}

//==============================================================================
UINT8 fwif_tv_autoscan_PAL_funtion(UINT32 tvAutoScanFreq)
{
    UINT8 mn_status;
    UINT8 pal_audio_detect = 0;
    UINT8 AScan_Noise_stas;
    UINT8 RetryCntPAL = 0;
    UINT8 RetryCnt358;
    UINT8 color_detect;
    UINT8 afc;
    UINT8 lock = 0;
    UINT8 new_v_lock;
    UINT8 flag358;
    UINT32 secamdk_tvScanCurFreq;
    UINT8 cnt_358;
    UINT16 Ratio;
    INT32 offset;
// linmc20091229, help sky_yeh to add auto scan enhance function
#ifdef CONFIG_EUROPE_AUTO_SCAN_FLOW
    UINT8 pal_new_v_lock_count=0;
    UINT32 average_burst;
    UINT32 secam_flag;
#endif
    UINT8 Status;

    RetryCntPAL = 0;
    color_detect=0;
    new_v_lock=0;
    afc=0;
    RetryCnt358 = 0;
    cnt_358 = 0;


   //RTD_Log(LOGGER_DEBUG,"tuner set frq123 %d\n",(UINT32) tvAutoScanFreq);
    drvif_ifd_set_iftuner_setting(ZPAL_I, (UINT32)tvAutoScanFreq, 5);	//suggest delay here should > 5
	if(GET_TV_TABLE_SCAN()) 
		fw_timer_event_DelayXms(150);

    Ratio = drv_tuner_GetRatioInStepSize();
    offset = drvif_tuner_get_afcoffset(0);
    //RTD_Log(LOGGER_LEVEL,"\n Tim Ratio= %d,offset = %d \n",(UINT32)Ratio,(UINT32)offset);
	if (( drvif_ifd_get_cr_lock() == _TRUE )&&((UINT32)(abs1(offset)) <= (UINT32)(10*Ratio)))//Tim 0402 //Tim 0404
    {
    //rtdf_maskl(DISPLAY_TIMING_CTRL1_0x2000, ~(_BIT26 ), 0);
        //RTD_Log(LOGGER_DEBUG,"\nIFD_CRLock %d\n",(UINT32) tvAutoScanFreq);
        //VDC initial code ++
#if defined(CONFIG_CHINA_AUTO_SCAN_FLOW)
        /*SECAM FLAG SETTING*/
        rtdf_outl(VDC_FREQ_VALID_INNER_0x5220,0x42);
        rtdf_outl(VDC_SECAM_FLAG_COUNTER_MAX_0x5228,0xA0);
        rtdf_outl(VDC_SECAM_FLAG_COUNTER_MIN_0x522c,0x0D);
        rtdf_outl(VDC_AUTO_MODE_S_M_STATISTIC_0x51d8,0x0B);
       // RTD_Log(LOGGER_INFO,"\n[autoscan PAL 1] 51d8=0x0B \n");
#endif
// linmc20091229, help sky_yeh to add auto scan enhance function
#ifdef CONFIG_EUROPE_AUTO_SCAN_FLOW
        rtdf_maskl(VDC_VSYNC_TIME_CONST_0x515c,0x00, 0x0a);
        rtdf_outl(VDC_VSYNC_TIME_CONST2_0x5160, 0x04);
        rtdf_outl(VDC_VSYNC_TIME_CONST1_0x5164, 0x20);
        drvif_module_vdc_Set_Hstate(VDC_HSTATE_RESET);
        drvif_module_vdc_Set_Cstate(VDC_NOT_FIX_CSTATE2);
#endif
#if defined(CONFIG_PROJECT_TV020)||defined(CONFIG_PROJECT_TV024_2)
        rtdf_outl(VDC_NO_BURST_MAG_FILTER_TH_0x5688,0x00);
        rtdf_outl(VDC_AUTO_MODE_S_M_STATISTIC_0x51d8,0x24);
        RTD_Log(LOGGER_INFO,"\n[autoscan PAL 2] 51d8=0x24 \n");
        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000007);
        rtdf_outl(VDC_BURST_GATE_START_0x5130,0x32);
        rtdf_outl(VDC_BURST_GATE_END_0x5134,0x46);
        rtdf_maskl(VDC_CHROMA_AUTOPOS_0x50a4, ~_BIT5, _BIT5);
        rtdf_maskl(VDC_CHROMA_AUTOPOS_0x50a4, 0xe0, 0x0c);
#else
        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000006);
#endif
        //VDC initial code --
        //audio initial code
        drvif_ifd_set_audio_bp(_SIG_MODE_AUTO);
		/*
		#if 1
			fw_timer_event_DelayXms(110);
			Status = (UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
			if(Status & _No_Sig)
			{
				if((Status&0x06)==0x00) // h&v unlock,prvent weak signal
					return  ZMODE_ERROR;
			}
			fw_timer_event_DelayXms(200);
		#else
			fw_timer_event_DelayXms(400);
		#endif
		*/
		fw_timer_event_DelayXms(20);
        if ( !drvif_ifd_get_agc_lock() )
			return ZCR_NOT_LOCK;
		RTD_Log(LOGGER_LEVEL,"\n Tim agc lock \n");
        fw_timer_event_DelayXms(20);
        Status = (UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
        if(Status & _No_Sig)
        {
            if((Status&0x06)==0x00) // h&v unlock,prvent weak signal
                return  ZCR_NOT_LOCK;//ZCR_NOT_LOCK;
        }
		
		fw_timer_event_DelayXms(140);
		
        RTD_Log(LOGGER_DEBUG,"tuner set frq %d\n",(UINT32) tvAutoScanFreq);
        //CHECKLSADCKEY();
        //RTD_Log(LOGGER_DEBUG, "AGC LOCK  FLAG2 %x \n", rtdf_inl(IFD_AGC_LOCK_STATUS_VADDR));
        RTD_Log(LOGGER_DEBUG, "RF AGC  %x \n", (UINT32)(rtdf_inl(IFD_AGC_RF_STATUS_VADDR)));
        RTD_Log(LOGGER_DEBUG, "IF PGA  %x \n", (UINT32)(rtdf_inl(IFD_READ_PGA_VADDR)));
        RTD_Log(LOGGER_DEBUG, "IF AGC  %x \n", (UINT32)(rtdf_inl(IFD_AGC_IF_STATUS_VADDR)));
        RTD_Log(LOGGER_DEBUG, "clamp offset  %x \n", (UINT32)(rtdf_inl(IFD_OFFSET_VADDR)));
        RTD_Log(LOGGER_DEBUG, "dgain status 2 %x \n", (UINT32)(rtdf_inl(VDC_AGC_DGAIN_STATUS2_0x5260)));
        RTD_Log(LOGGER_DEBUG, "dgain status 1 %x \n", (UINT32)(rtdf_inl(VDC_AGC_DGAIN_STATUS1_0x5264)));
        RTD_Log(LOGGER_DEBUG, "pal tuner is lock\n");
        RTD_Log(LOGGER_DEBUG, "vd status = %x\n", (UINT32)(rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008)));

        //CHECKLSADCKEY();
        if (drv_video_vdc_CheckHVisLocked() )
        {
            lock=1;
            RTD_Log(LOGGER_DEBUG,"tuner and vd is lock ok %d\n", (UINT32) tvAutoScanFreq);
            RetryCntPAL =1;
            do
            {
                ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
                RTD_Log(LOGGER_DEBUG,"TV AUTO SCAN mode is NTSC443 or SECAM 1=... %x\n", (UINT32)ucVdc_TV_Mode);
                AScan_Noise_stas = drv_video_vdc_NoiseStatus();
                AScan_Noise_stas1 = AScan_Noise_stas;
                RTD_Log(LOGGER_DEBUG,"TV AUTO SCAN mode is NTSC443 or SECAM noise ... %x\n",(UINT32) AScan_Noise_stas);
                RTD_Log(LOGGER_DEBUG, "VDC_VIDEO_STATUS_REG2_VADDR = %x\n", (UINT32) (rtdf_inl(VDC_VIDEO_STATUS_REG2_0x500c)));
                RTD_Log(LOGGER_DEBUG, "VDC_VIDEO_STATUS_REG3_VADDR = %x\n", (UINT32) (rtdf_inl(VDC_VIDEO_STATUS_REG3_0x5010)));
#if defined(CONFIG_CHINA_AUTO_SCAN_FLOW)

                if ( ucVdc_TV_Mode == ZSECAM )
                {
                    fw_timer_event_DelayXms (500);
                    ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
                    RTD_Log(LOGGER_DEBUG,"TV AUTO SCAN mode is NTSC443 or SECAM 2= ... %x\n", (UINT32)ucVdc_TV_Mode);
                }
#endif
                // linmc20091229, help sky_yeh to add auto scan enhance function
#ifdef CONFIG_EUROPE_AUTO_SCAN_FLOW
                if (( ucVdc_TV_Mode == ZSECAM )&&(AScan_Noise_stas<0x10))
                {

                    fw_timer_event_DelayXms(100);
                    color_detect=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
                    if ((color_detect&0x08)== 0x00)
                    {
                        fw_timer_event_DelayXms(100);
                        ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();

                    }
                }
#endif

                if ( ucVdc_TV_Mode == ZNTSC )
                {
#ifdef CONFIG_EUROPE_AUTO_SCAN_FLOW
                    return ZNTSC;
#else
                    return ZMODE_525_STD;
#endif
                }
#ifdef CONFIG_EUROPE_AUTO_SCAN_FLOW
                else if (( ucVdc_TV_Mode == ZNTSC_443 )&&(AScan_Noise_stas<=0x06))
#else
                else if ( ucVdc_TV_Mode == ZNTSC_443 )
#endif
                {
                    //CHECKLSADCKEY();
#ifndef CONFIG_TUNER_SILICON
                    fw_timer_event_DelayXms(200); // wait the color of vd and audio are stable
                    //frank@1102 in front have 400ms dalay	ScalerTimer_DelayXms(200);  // wait the color of vd and audio are stable
                    color_detect = (UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
                    RTD_Log(LOGGER_DEBUG,"VDC_VIDEO_STATUS_REG1_VADDR=%x ??????........... \n", (UINT32) color_detect);
                    if ((color_detect & 0x08) == 0x08)
                    {
#ifdef _VDC_DETECT_358_FLAG
                        flag358=rtdf_inl(VDC_VIDEO_STATUS_REG2_0x500c)&0x80;
                        RTD_Log(LOGGER_DEBUG,"the vd flag358=%x ??????........... \n", (UINT32) flag358);
                        if (flag358 != 0x80)
                            return ZNTSC_443;
#else //#ifdef _VDC_DETECT_358_FLAG
                        Audio_AtvGetMNStatus(&mn_status, NULL);

                        if (mn_status == 0)
                        {
                            return ZNTSC_443;
                        }
                        else
                            return ZMODE_525_STD;
#endif //#ifdef _VDC_DETECT_358_FLAG
                    }
                    else
                        return ZMODE_525_STD;
#else //CONFIG_TUNER_SILICON
                    return ZMODE_525_STD;
#endif //CONFIG_TUNER_SILICON
                }
                else if ( (ucVdc_TV_Mode==ZPAL_N)&&(AScan_Noise_stas<=0x06))
                {
#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
                    if (_TRUE == fwif_tv_autoscan_check_new_vlock(2))
                        return ZPAL_N;
#else
                    flag358=rtdf_inl(VDC_VIDEO_STATUS_REG2_0x500c)&0x80;
                    RTD_Log(LOGGER_DEBUG,"the vd mode is pal n the flag358=%x ??????........... \n",(UINT32) flag358);
                    if (flag358==0x80)
                        return ZPAL_N;
#endif
                }
#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
                else if ( (ucVdc_TV_Mode==ZSECAM)&&(AScan_Noise_stas<=0xa0))
#else
                else if ( (ucVdc_TV_Mode==ZSECAM)&&(AScan_Noise_stas<=0x1F))
#endif
                {
#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
                    if (_TRUE == fwif_tv_autoscan_check_new_vlock(2))
                    {
                        // linmc20091229, help sky_yeh to add auto scan enhance function
#ifdef CONFIG_EUROPE_AUTO_SCAN_FLOW
#if defined(CONFIG_PROJECT_TV020)||defined(CONFIG_PROJECT_TV024_2)
                        rtdf_outl(VDC_BURST_GATE_START_0x5130,0x50);
                        rtdf_outl(VDC_BURST_GATE_END_0x5134,0x64);
                        fw_timer_event_DelayXms(40);
#endif
                        average_burst=drvif_module_vdc_AVG_BURST_MAG_STATUS();
                        secam_flag=rtdf_inl(VDC_VIDEO_STATUS_REG3_0x5010)&0x02;
                        RTD_Log(LOGGER_DEBUG,"the secam detect ,average_burst=%x ??????........... \n", (UINT32) average_burst);
                        RTD_Log(LOGGER_DEBUG,"the secam detect ,secam_flag=%x ??????........... \n", (UINT32) secam_flag);
                        RTD_Log(LOGGER_DEBUG,"the secam detect ,AScan_Noise_stas=%x ??????........... \n", (UINT32) AScan_Noise_stas);
                        if ((average_burst<0x08)&&(secam_flag==0x00)&&(AScan_Noise_stas<0x15))
                            return ZPAL_I;
                        else
                        {
                            if ((secam_flag==0x00)&&(AScan_Noise_stas<0x20))
                            {
                                RTD_Log(LOGGER_DEBUG,"the pal i detect  the secam flag is off\n");
                                return ZPAL_I;
                            }
                            else
                            {
                                if ((secam_flag==0x02)&&(AScan_Noise_stas<0x30))
                                {
                                    RTD_Log(LOGGER_DEBUG,"the secam  detect  the secam flag is on\n");
                                    return ZSECAM;
                                }
                            }
                        }
#else
                        return ZSECAM;
#endif
                    }
#else //#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
                    return ZSECAM;
#endif //#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)

#if defined(CONFIG_PROJECT_TV020)||defined(CONFIG_PROJECT_TV024_2)
                    rtdf_outl(VDC_BURST_GATE_START_0x5130,0x32);
                    rtdf_outl(VDC_BURST_GATE_END_0x5134,0x46);

#endif
                }
                /*
                				drvif_ifd_set_iftuner_setting(ZPAL_I,tvAutoScanFreq,300);
                				ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
                				RTD_Log(LOGGER_DEBUG,"TV AUTO SCAN mode is  ... %x\n", (UINT32) ucVdc_TV_Mode);
                				AScan_Noise_stas = drv_video_vdc_NoiseStatus();
                				RTD_Log(LOGGER_DEBUG,"the pal noise  ... %x\n", (UINT32) AScan_Noise_stas);
                //				new_v_lock=drvif_module_vdc_New_Vlock();
                //				RTD_Log(LOGGER_DEBUG,"secam TV AUTO SCAN mode is ,the new v lock=... %x\n",new_v_lock);
                */
                else if ( ucVdc_TV_Mode==ZPAL_I )
                {
                    rtdf_outl(VDC_MANUAL_MODE_0x51c0, 0x00000006);
                    AScan_Noise_stas = drv_video_vdc_NoiseStatus();
                    RTD_Log(LOGGER_DEBUG,"the pal noise  ... %x\n", (UINT32) AScan_Noise_stas);

				//#ifdef TV_PAL_NTSC_TH
                    //if (AScan_Noise_stas > 0xb6)
                        //break;
				//#endif //#ifdef TV_PAL_NTSC_TH

                    Audio_AtvGetMNStatus(&mn_status, NULL);

                    if (mn_status == 0 )
                    {
                        pal_audio_detect=1;
                    }

                    if (AScan_Noise_stas >= 0x50 )
                    {
                        //CHECKLSADCKEY();
                        fw_timer_event_DelayXms(200); // wait noise of vd is stable
                        AScan_Noise_stas = drv_video_vdc_NoiseStatus();
                        RTD_Log(LOGGER_DEBUG,"the pal noise is too big so to do one  ... %x\n", (UINT32) AScan_Noise_stas);
                        cnt_358 = 0;
                        for (RetryCnt358 = 0; RetryCnt358 < 3; RetryCnt358++)
                        {
                            color_detect = (UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG2_0x500c);
                            RTD_Log(LOGGER_DEBUG,"VDC_VIDEO_STATUS_REG2_VADDR=%x ??????........... \n", (UINT32) color_detect);
                            if ((color_detect & 0x90) == 0x90)
                            {
                                cnt_358++;
                            }
                            fw_timer_event_DelayXms(5);
                        }
                        if (cnt_358 >=2)
                            return ZMODE_525_STD;
                    }

                    if ((AScan_Noise_stas<0xa5)&&(pal_audio_detect==1))//0x70
                    {
                        RTD_Log(LOGGER_DEBUG,"pal detect \n");
                        return ZPAL_I;
                    }
                }

#ifdef CONFIG_SUPPORT_PAL60
                else if ( ucVdc_TV_Mode == ZPAL_60 )
                {
                    //	CHECKLSADCKEY();
                    //frank@1102 in front have 400ms dalay	ScalerTimer_DelayXms(500);//wait the the audio  is stable
                    //CHECKLSADCKEY();
                    fw_timer_event_DelayXms(500);//wait the the v sync and h sync is lock
                    //CHECKLSADCKEY();

                    Audio_AtvGetMNStatus(&mn_status, NULL);

                    if (mn_status == 0 )
                    {
                        RTD_Log(LOGGER_DEBUG,"pal60 detect \n");
                        return  ZPAL_60;
                    }
                    else
                        return ZMODE_525_STD;
                }
#endif //#ifdef CONFIG_SUPPORT_PAL60

                else if ( (ucVdc_TV_Mode == ZNTSC_50) && ((AScan_Noise_stas = drv_video_vdc_NoiseStatus())<=0x05) )
                {
#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
                    if ( _TRUE == fwif_tv_autoscan_check_new_vlock(2))
                    {
                        RTD_Log(LOGGER_DEBUG,"ntsc50 detect \n");
                        return  ZPAL_I;
                    }
#else
                    RTD_Log(LOGGER_DEBUG,"ntsc50 detect \n");
                    return  ZPAL_I;
#endif
                }
                else if ( (ucVdc_TV_Mode==ZPAL_N) && ((AScan_Noise_stas = drv_video_vdc_NoiseStatus())<=0x13))
                {
#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
                    if ( _FALSE == fwif_tv_autoscan_check_new_vlock(2))
                        break;
#endif //#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
                    flag358=rtdf_inl(VDC_VIDEO_STATUS_REG2_0x500c)&0x80;
                    RTD_Log(LOGGER_DEBUG,"the vd mode is pal n the flag358=%x ??????........... \n", (UINT32) flag358);
                    if (flag358==0x80)
                    {
                        secamdk_tvScanCurFreq = tvAutoScanFreq +_TUNER_MANUAL_SCAN_MIN_SETP;
                        //					drvif_ifd_set_iftuner_setting(ZPAL_I,secamdk_tvScanCurFreq,165);
                        drvif_ifd_set_iftuner_setting(ZPAL_I,secamdk_tvScanCurFreq,20);
                        ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
                        AScan_Noise_stas = drv_video_vdc_NoiseStatus();
                        RTD_Log(LOGGER_DEBUG,"the tuner set pal the frq is +4  but may be the pal i  mode is ... %x\n", (UINT32) ucVdc_TV_Mode);
                        RTD_Log(LOGGER_DEBUG,"the tuner set pal but may be the pal n  ,the noise... %x\n", (UINT32)AScan_Noise_stas);

                        if (ucVdc_TV_Mode==ZPAL_I)
                        {
                            new_v_lock=drvif_module_vdc_New_Vlock();
                            RTD_Log(LOGGER_DEBUG,"he tuner set pal but may be the pal n ,the new v lock=... %x\n", (UINT32)new_v_lock);

                            if (1 == new_v_lock)
                            {
                                RTD_Log(LOGGER_DEBUG,"pal the frq +4 detect \n");
                                return  ZPAL_I;
                            }
                        }
                    }
                }
                else if (ucVdc_TV_Mode==ZPAL_M)
                {
                    return ZMODE_525_STD;
                }

            }
            while (--RetryCntPAL);

            return  ZMODE_ERROR;
        }
        else
        {
            return ZHV_NOT_LOCK;
        }
    }
    else
    {
    	//RTD_Log(LOGGER_LEVEL,"\n Tim not lock\n");
        return ZCR_NOT_LOCK;
    }
}

#if 0
UINT8 fwif_tv_autoscan_PAL_funtion(UINT32 tvAutoScanFreq)
{
    UINT8 RetryCntPAL = 0;
    UINT8 hv_lock = 0;
    UINT8 RetryCntNTSC = 0;
    UINT8 lock = 0;
    UINT32 flag625 = 0;
    UINT8 AScan_Noise_stas;
    UINT8 hv_lock_mode=0;  //irene

    UINT8 agc_lock1 = 0;
    UINT8 agc_lock2 = 0;
    UINT16 clamp_lv = 0;


#ifdef AUDIO_USE_DISABLE
    UINT8 color_detect;
    UINT8 ntsc443_color;
#else
    UINT8 mn_status;
    UINT8 pal_audio_detect = 0;
    UINT8 color_detect;
#endif
    RetryCntNTSC = 0;
    RetryCntPAL = 0;
    lock = 0;
    flag625 = 0;
    hv_lock = 0;
    hv_lock_mode=0;

    drvif_ifd_set_iftuner_setting(ZAPL_I, tvAutoScanFreq, 16);

    rtdf_outl(VDC_MANUAL_MODE_0x51c0, 0x00000006);
    //RTD_Log(LOGGER_DEBUG, "tuner set frq %d\n", tvScanCurFreq);

    if (drvif_ifd_get_cr_lock()==_TRUE)
    {
        fw_timer_event_DelayXms(250);
        agc_lock1 = (UINT8)(rtd_inl(AGC_LOCK_STATUS_0x5d40) & 0x1);
        agc_lock2 = drvif_ifd_get_agc_lock();
        clamp_lv = (UINT16)(rtd_inl(OFFSET_0x5c98) & 0x3fff);
        RTD_Log(LOGGER_DEBUG, "pal tuner is lock @ freq = %d, agc_lock1 = %d, agc_lock2 = %d, clamp=%x\n", (UINT32)tvAutoScanFreq, (UINT32)agc_lock1, (UINT32)agc_lock2, (UINT32)clamp_lv);
#ifdef CONFIG_ENABLE_TV_NOISE_BOX_ENALBE
        if (Auto_Scan_videoislocked_Tv_noise_box())
#else
        if (drv_video_vdc_CheckHVisLocked())
#endif
        {
            //AScan_Noise_stas = drv_video_vdc_NoiseStatus();
            //if (AScan_Noise_stas > 0xd8)
            //{
            //	RTD_Log(LOGGER_DEBUG,"noise status = %x\n",(UINT32)AScan_Noise_stas);
            //	return 0;
            //}
#ifdef CONFIG_ENABLE_TV_NOISE_BOX_ENALBE
            fw_timer_event_DelayXms(500);
#else
            fw_timer_event_DelayXms(150);
#endif

            lock=1;
            RTD_Log(LOGGER_DEBUG,"tuner and vd is lock ok %d\n", (UINT32)tvAutoScanFreq);
            RetryCntPAL =1;
            do
            {
                RTD_Log(LOGGER_DEBUG,"reg.5c80=%x\n",(UINT32)rtdf_inl(VIDEO_BW_SEL_0x5c80));
                ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
                RTD_Log(LOGGER_DEBUG,"TV AUTO SCAN mode is NTSC443 or SECAM ... %x\n", (UINT32) ucVdc_TV_Mode);
                AScan_Noise_stas = drv_video_vdc_NoiseStatus();
                RTD_Log(LOGGER_DEBUG,"TV AUTO SCAN mode is NTSC443 or SECAM noise ... %x\n", (UINT32)AScan_Noise_stas);
#if 0
#ifdef AUDIO_USE_DISABLE
                if ((ucVdc_TV_Mode==ZNTSC_443)&&(AScan_Noise_stas<=0x12))
                {
                    SET_CHANNEL_COLORSYSTEM(tvScanCurChn, ZNTSC_443);
                    SET_CHANNEL_SOUNDSYSTEM(tvScanCurChn, _SOUND_MN);
                    //stUserDataChannelSettings[tvScanCurChn-1].colorSystem=ZNTSC_443;
                    //stUserDataChannelSettings[tvScanCurChn-1].soundSystem= _SOUND_MN;
                    break;
                }
                if ((ucVdc_TV_Mode==ZNTSC_443)&&(AScan_Noise_stas>=0x13))
                {
                    color_detect=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
                    if ((color_detect&0x08)== 0x08)
                    {
                        ntsc443_color=1;
                    }
                    else
                        ntsc443_color=0;
                }
#else//use the audio  to select the mode
                if ((ucVdc_TV_Mode==ZNTSC_443)&&(AScan_Noise_stas<=0x20))
                {
                    fw_timer_event_DelayXms(200);  // wait the color of vd and audio are stable
                    color_detect=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
                    RTD_Log(LOGGER_DEBUG,"?? VDC_VIDEO_STATUS_REG1_VADDR = %x\n", (UINT32)color_detect);
                    if ((color_detect&0x08)== 0x08)
                    {
                        Audio_AtvGetMNStatus(&mn_status, NULL);
                        if (mn_status == 0)
                        {
                            RTD_Log(LOGGER_DEBUG,"ntsc443 is ok\n");
                            return ZNTSC_443;
                        }
                    }
                }
#endif
#endif
                if ( ucVdc_TV_Mode == ZNTSC )
                {
#ifdef CONFIG_EUROPE_AUTO_SCAN_FLOW
                    return ZNTSC;
#else
                    RTD_Log(LOGGER_DEBUG,"ZZZNNNTTTSSSCCC\n");
                    return ZMODE_ERROR;
#endif
                }

                if ((ucVdc_TV_Mode == ZNTSC_443))
                {
                    //CHECKLSADCKEY();
                    //frank@1102 in front have 400ms dalay	ScalerTimer_DelayXms(200);  // wait the color of vd and audio are stable
                    color_detect=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
                    RTD_Log(LOGGER_DEBUG,"VDC_VIDEO_STATUS_REG1_VADDR=%x ??????........... \n", (UINT32) color_detect);
                    if ((color_detect & 0x08) == 0x08)
                    {
                        Audio_AtvGetMNStatus(&mn_status, NULL);
                        if (mn_status == 0)
                            return ZNTSC_443;
                        else
                            return ZMODE_ERROR;
                    }
                    else
                        return ZMODE_ERROR;
                }

                if ((ucVdc_TV_Mode==ZPAL_N)&&(AScan_Noise_stas<=0x06))
                {
#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
                    if (_TRUE == fwif_tv_autoscan_check_new_vlock(2))
                        return ZPAL_N;
#else
                    return ZPAL_N;
#endif
                }

                if ((ucVdc_TV_Mode==ZSECAM)&&(AScan_Noise_stas<=0x1F))
                {
#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
                    if (_TRUE == fwif_tv_autoscan_check_new_vlock(2))
                        return ZSECAM;
#else
                    return ZSECAM;
#endif

                }

#if 0
                secamdk_tvScanCurFreq = tvScanCurFreq + SECAM_FRQ;
                drvif_ifd_set_iftuner_setting(ZPAL_I,secamdk_tvScanCurFreq,165);
                ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
                AScan_Noise_stas = drv_video_vdc_NoiseStatus();
                if (ucVdc_TV_Mode == ZSECAM)
                {
                    return ZSECAM;
                }
#endif

                //drvif_ifd_set_iftuner_setting(ZPAL_I,tvScanCurFreq,300);
                //ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
                //RTD_Log(LOGGER_DEBUG,"TV AUTO SCAN mode is  ... %x\n", (UINT32)ucVdc_TV_Mode);
#ifdef TV_PAL_NTSC_TH
                AScan_Noise_stas = drv_video_vdc_NoiseStatus();
                RTD_Log(LOGGER_DEBUG,"the pal noise  ... %x\n", (UINT32) AScan_Noise_stas);
                if ((ucVdc_TV_Mode==ZPAL_I)&&(AScan_Noise_stas<=0xb6))
#else
                if (ucVdc_TV_Mode==ZPAL_I)
#endif
                {
#ifndef AUDIO_USE_DISABLE
                    Audio_AtvGetMNStatus(&mn_status, NULL);
                    if (mn_status == 0)
                    {
                        pal_audio_detect=1;
                    }
#endif
                    if (AScan_Noise_stas>=0x30)
                    {
                        fw_timer_event_DelayXms(200);
                        AScan_Noise_stas = drv_video_vdc_NoiseStatus();
                        //leo 20090217 add for audio tone be identified as video
                        RTD_Log(LOGGER_DEBUG,"the pal noise is too big so to do one  ... %x\n", (UINT32)AScan_Noise_stas);
                    }
#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
                    if (_FALSE == fwif_tv_autoscan_check_new_vlock(2))
                        break;
#endif

#ifdef AUDIO_USE_DISABLE
                    if (AScan_Noise_stas<=0x30)
#else//use the audio  to select the mode
                    if ((AScan_Noise_stas<=0x30)&&(pal_audio_detect==1))
#endif
                    {
                        RTD_Log(LOGGER_DEBUG,"pal detect \n");
                        return ZPAL_I;
                        break;
                    }
#ifdef HISENSE_TV_SECAM_SIGNAL
                    else
                    {
                        secamdk_tvScanCurFreq = tvAutoScanFreq - SECAM_FRQ;
                        drvif_ifd_set_iftuner_setting(ZPAL_I, secamdk_tvScanCurFreq, 300);
//						ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
                        ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
                        AScan_Noise_stas = drv_video_vdc_NoiseStatus();
                        RTD_Log(LOGGER_DEBUG,"the tuner set pal but may be the pal n  mode is ... %x\n", (UINT32)ucVdc_TV_Mode);
                        RTD_Log(LOGGER_DEBUG,"the tuner set pal but may be the pal n  ... %x\n", (UINT32)AScan_Noise_stas);

                        if ((ucVdc_TV_Mode==ZPAL_N)&&(AScan_Noise_stas<=0x10))
                        {
#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
                            if (_TRUE == fwif_tv_autoscan_check_new_vlock(2))
                                return  ZPAL_N;
#else
                            return  ZPAL_N;
#endif
                        }
                        else if ((ucVdc_TV_Mode==ZNTSC)&&(AScan_Noise_stas<=0x10))
                        {
                            return ZNTSC;
                        }
                        else
                        {

                            if ((ucVdc_TV_Mode==ZPAL_I)&&(AScan_Noise_stas<=0x15))
                            {
#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
                                if (_TRUE == fwif_tv_autoscan_check_new_vlock(2))
                                    return  ZPAL_I;
#else
                                return  ZPAL_I;
#endif
                            }

                        }
                    }
#endif
                }

#ifdef CONFIG_SUPPORT_PAL60
                if (ucVdc_TV_Mode == ZPAL_60)
                {
                    fw_timer_event_DelayXms(500);//wait the the audio  is stable
                    fw_timer_event_DelayXms(500);//wait the the v sync and h sync is lock
                    Audio_AtvGetMNStatus(&mn_status, NULL);
                    if (mn_status == 0)
                    {
                        RTD_Log(LOGGER_DEBUG,"pal60 detect \n");
                        return  ZPAL_60;
                    }
                }
#endif

#if 0
                if ((ucVdc_TV_Mode == ZNTSC_50)&&(AScan_Noise_stas<=0x05))
                {
                    RTD_Log(LOGGER_DEBUG,"ntsc50 detect \n");
                    return	ZPAL_I;
                }
                if ((ucVdc_TV_Mode==ZPAL_N)&&(AScan_Noise_stas<=0x13))
                {
                    secamdk_tvScanCurFreq=tvScanCurFreq +_TUNER_MANUAL_SCAN_MIN_SETP;

                    drvif_ifd_set_iftuner_setting(ZPAL_I,secamdk_tvScanCurFreq,165);
                    ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
                    AScan_Noise_stas = drv_video_vdc_NoiseStatus();
                    RTD_Log(LOGGER_DEBUG,"the tuner set pal the frq is +4	but may be the pal i  mode is ... %x\n",(UINT32)ucVdc_TV_Mode);
                    RTD_Log(LOGGER_DEBUG,"the tuner set pal but may be the pal n  ... %x\n",(UINT32)AScan_Noise_stas);
                    if (ucVdc_TV_Mode==ZPAL_I)
                    {
                        RTD_Log(LOGGER_DEBUG,"pal the frq +4 detect \n");
                        return	ZPAL_I;
                    }
                }
#endif
            }
            while (--RetryCntPAL);
            return  ZMODE_ERROR;
        }
        else
            return ZHV_NOT_LOCK;
    }
    else
        return ZCR_NOT_LOCK;
}

#endif
#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
#if defined(CONFIG_APP_EURO)
UINT8 fwif_tv_autoscan_SECAML_funtion( UINT32 tvAutoScanFreq)
{
    UINT8 AScan_Noise_stas;
    UINT8 agc_lock1 = 0;
    UINT8 agc_lock2 = 0;
    UINT16 clamp_lv = 0;
    UINT16 Ratio;
    INT32 offset;
    UINT8 secam_flag,flag358,pal_flag,Status;


//	RTD_Log(LOGGER_DEBUG, "Tuner change to secam l \n");
    drvif_ifd_set_iftuner_setting(ZSECAML, tvAutoScanFreq, 5);	//suggest delay here should > 5
    

    
    Ratio = drv_tuner_GetRatioInStepSize();
    offset = drvif_tuner_get_afcoffset(0);
    //RTD_Log(LOGGER_LEVEL,"\n Tim Ratio= %d,offset = %d \n",(UINT32)Ratio,(UINT32)offset);
	if (( drvif_ifd_get_cr_lock() == _TRUE )&&((UINT32)(abs1(offset)) <= (UINT32)(10*Ratio)))//Tim 0402 //Tim 0404

    //if ( drvif_ifd_get_cr_lock() == _TRUE )
    {
        //RTD_Log(LOGGER_DEBUG, "secam l tuener is lock\n");
		rtdf_outl(VDC_FREQ_VALID_INNER_0x5220,0x42);
		rtdf_outl(VDC_SECAM_FLAG_COUNTER_MAX_0x5228,0xA0);
		rtdf_outl(VDC_SECAM_FLAG_COUNTER_MIN_0x522c,0x0D);
		rtdf_outl(VDC_AUTO_MODE_S_M_STATISTIC_0x51d8,0x0B);
        rtdf_outl(VDC_MANUAL_MODE_0x51c0, 0x00000007);

		//VDC initial code --
		//audio initial code
		drvif_ifd_set_audio_bp(_SIG_MODE_AUTO);


		#if 1
		fw_timer_event_DelayXms(20);
        if ( !drvif_ifd_get_agc_lock() )
			return ZCR_NOT_LOCK;
		//RTD_Log(LOGGER_LEVEL,"\n Tim agc lock \n");
        fw_timer_event_DelayXms(20);
        Status = (UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
        if(Status & _No_Sig)
        {
            if((Status&0x06)==0x00) // h&v unlock,prvent weak signal
                return  ZCR_NOT_LOCK;//ZCR_NOT_LOCK;
        }
		
		fw_timer_event_DelayXms(140);


		#else
        fw_timer_event_DelayXms(400);
#endif
        agc_lock1 = (UINT8)(rtd_inl(AGC_LOCK_STATUS_0x5d40) & 0x1);
        agc_lock2 = drvif_ifd_get_agc_lock();
        clamp_lv = (UINT16)(rtd_inl(OFFSET_0x5c98) & 0x3fff);
        RTD_Log(LOGGER_DEBUG, "secam l tuner is lock @ freq = %d, agc_lock1 = %d, agc_lock2 = %d, clamp=%x\n", (UINT32)tvScanCurFreq, (UINT32)agc_lock1, (UINT32)agc_lock2, (UINT32)clamp_lv);
        if (drv_video_vdc_CheckHVisLocked())
        {
            //fw_timer_event_DelayXms(300);
            RTD_Log(LOGGER_DEBUG,"tuner is secam l and vd is lock ok %d\n", (UINT32)tvAutoScanFreq);
            ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
            RTD_Log(LOGGER_DEBUG,"tuner is secam l  the vd detect mode1= %x...\n", (UINT32)ucVdc_TV_Mode);
            secam_flag=rtdf_inl(VDC_VIDEO_STATUS_REG3_0x5010)&0x02;
            RTD_Log(LOGGER_DEBUG,"tuner is secam l  ,secam_flag=%x ??????........... \n",(UINT32) secam_flag);
            AScan_Noise_stas = drv_video_vdc_NoiseStatus();
            RTD_Log(LOGGER_DEBUG,"tuner is secam l  the vd  noise level=  %x....\n", (UINT32) AScan_Noise_stas);
            flag358 = rtdf_inl(VDC_VIDEO_STATUS_REG2_0x500c)&0x80;
            pal_flag = rtdf_inl(VDC_VIDEO_STATUS_REG2_0x500c)&0x06;
            RTD_Log(LOGGER_DEBUG,"tuner is secam l  the vd  flag358=  %x....\n",(UINT32) flag358);
            RTD_Log(LOGGER_DEBUG,"tuner is secam l  the vd pal_flag=  %x....\n",(UINT32) pal_flag);

            if ( (ucVdc_TV_Mode ==ZSECAM)&&(AScan_Noise_stas<=0x25)&&(secam_flag==0x02) )
            {
                RTD_Log(LOGGER_DEBUG,"secam l detect \n");
                return ZSECAML;

            }
            return  ZMODE_ERROR;
        }
        else
        {
            RTD_Log(LOGGER_DEBUG,"secam l VDC h/v no lock \n");
            return ZHV_NOT_LOCK;
        }
    }
    else
        return ZCR_NOT_LOCK;
}
UINT8 fwif_tv_autoscan_single_SECAMBG_DK_funtion( UINT32 tvAutoScanFreq)
{
    UINT8 AScan_Noise_stas;
    UINT8 agc_lock1 = 0;
    UINT8 agc_lock2 = 0;
    UINT16 clamp_lv = 0;
    UINT16 Ratio;
    INT32 offset;
    UINT8 secam_flag,flag358,pal_flag,Status;


//	RTD_Log(LOGGER_DEBUG, "Tuner change to secam l \n");
    drvif_ifd_set_iftuner_setting(ZSECAM, tvAutoScanFreq, 5);	//suggest delay here should > 5
    

    
    Ratio = drv_tuner_GetRatioInStepSize();
    offset = drvif_tuner_get_afcoffset(0);
    //RTD_Log(LOGGER_LEVEL,"\n Tim Ratio= %d,offset = %d \n",(UINT32)Ratio,(UINT32)offset);
	if (( drvif_ifd_get_cr_lock() == _TRUE )&&((UINT32)(abs1(offset)) <= (UINT32)(10*Ratio)))//Tim 0402 //Tim 0404

    //if ( drvif_ifd_get_cr_lock() == _TRUE )
    {
        RTD_Log(LOGGER_DEBUG, "secam l tuener is lock\n");

		rtdf_outl(VDC_FREQ_VALID_INNER_0x5220,0x42);
		rtdf_outl(VDC_SECAM_FLAG_COUNTER_MAX_0x5228,0xA0);
		rtdf_outl(VDC_SECAM_FLAG_COUNTER_MIN_0x522c,0x0D);
		rtdf_outl(VDC_AUTO_MODE_S_M_STATISTIC_0x51d8,0x0B);
        rtdf_outl(VDC_MANUAL_MODE_0x51c0, 0x00000007);

		//VDC initial code --
		//audio initial code
		drvif_ifd_set_audio_bp(_SIG_MODE_AUTO);


		#if 1
		fw_timer_event_DelayXms(20);
        if ( !drvif_ifd_get_agc_lock() )
			return 0;
		RTD_Log(LOGGER_LEVEL,"\n Tim agc lock \n");
        fw_timer_event_DelayXms(20);
        Status = (UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
        if(Status & _No_Sig)
        {
            if((Status&0x06)==0x00) // h&v unlock,prvent weak signal
                return  0;//ZCR_NOT_LOCK;
        }
		
		fw_timer_event_DelayXms(140);


		#else
        fw_timer_event_DelayXms(400);
		#endif
        agc_lock1 = (UINT8)(rtd_inl(AGC_LOCK_STATUS_0x5d40) & 0x1);
        agc_lock2 = drvif_ifd_get_agc_lock();
        clamp_lv = (UINT16)(rtd_inl(OFFSET_0x5c98) & 0x3fff);
        RTD_Log(LOGGER_DEBUG, "secam l tuner is lock @ freq = %d, agc_lock1 = %d, agc_lock2 = %d, clamp=%x\n", (UINT32)tvScanCurFreq, (UINT32)agc_lock1, (UINT32)agc_lock2, (UINT32)clamp_lv);
        if (drv_video_vdc_CheckHVisLocked())
        {
            //fw_timer_event_DelayXms(300);
            RTD_Log(LOGGER_DEBUG,"tuner is secam l and vd is lock ok %d\n", (UINT32)tvAutoScanFreq);
            ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
            RTD_Log(LOGGER_DEBUG,"tuner is secam l  the vd detect mode1= %x...\n", (UINT32)ucVdc_TV_Mode);
            secam_flag=rtdf_inl(VDC_VIDEO_STATUS_REG3_0x5010)&0x02;
            RTD_Log(LOGGER_DEBUG,"tuner is secam l  ,secam_flag=%x ??????........... \n",(UINT32) secam_flag);
            AScan_Noise_stas = drv_video_vdc_NoiseStatus();
            RTD_Log(LOGGER_DEBUG,"tuner is secam l  the vd  noise level=  %x....\n", (UINT32) AScan_Noise_stas);
            flag358 = rtdf_inl(VDC_VIDEO_STATUS_REG2_0x500c)&0x80;
            pal_flag = rtdf_inl(VDC_VIDEO_STATUS_REG2_0x500c)&0x06;
            RTD_Log(LOGGER_DEBUG,"tuner is secam l  the vd  flag358=  %x....\n",(UINT32) flag358);
            RTD_Log(LOGGER_DEBUG,"tuner is secam l  the vd pal_flag=  %x....\n",(UINT32) pal_flag);

/*
            ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
            RTD_Log(LOGGER_DEBUG,"TV AUTO SCAN mode is NTSC443 or SECAM 1=... %x\n", (UINT32)ucVdc_TV_Mode);
            AScan_Noise_stas = drv_video_vdc_NoiseStatus();
            AScan_Noise_stas1 = AScan_Noise_stas;
            RTD_Log(LOGGER_DEBUG,"TV AUTO SCAN mode is NTSC443 or SECAM noise ... %x\n",(UINT32) AScan_Noise_stas);
            RTD_Log(LOGGER_DEBUG, "VDC_VIDEO_STATUS_REG2_VADDR = %x\n", (UINT32) (rtdf_inl(VDC_VIDEO_STATUS_REG2_0x500c)));
            RTD_Log(LOGGER_DEBUG, "VDC_VIDEO_STATUS_REG3_VADDR = %x\n", (UINT32) (rtdf_inl(VDC_VIDEO_STATUS_REG3_0x5010)));
*/

            if ( (ucVdc_TV_Mode ==ZSECAM)&&(AScan_Noise_stas<=0x25)&&(secam_flag==0x02) )
            {
                RTD_Log(LOGGER_DEBUG,"secam l detect \n");
                return ZSECAML;

            }
            return  0;
        }
        else
        {
            RTD_Log(LOGGER_DEBUG,"secam l VDC h/v no lock \n");
            return 0;
        }
    }
    else
        return 0;
}

UINT8 fwif_tv_autoscan_single_SECAML_funtion( UINT32 tvAutoScanFreq)
{
    UINT8 AScan_Noise_stas;
    UINT8 agc_lock1 = 0;
    UINT8 agc_lock2 = 0;
    UINT16 clamp_lv = 0;
    UINT16 Ratio;
    INT32 offset;
    UINT8 secam_flag,flag358,pal_flag,Status;


//	RTD_Log(LOGGER_DEBUG, "Tuner change to secam l \n");
    drvif_ifd_set_iftuner_setting(ZSECAML, tvAutoScanFreq, 5);	//suggest delay here should > 5
    

    
    Ratio = drv_tuner_GetRatioInStepSize();
    offset = drvif_tuner_get_afcoffset(0);
    //RTD_Log(LOGGER_LEVEL,"\n Tim Ratio= %d,offset = %d \n",(UINT32)Ratio,(UINT32)offset);
	if (( drvif_ifd_get_cr_lock() == _TRUE )&&((UINT32)(abs1(offset)) <= (UINT32)(10*Ratio)))//Tim 0402 //Tim 0404

    //if ( drvif_ifd_get_cr_lock() == _TRUE )
    {
        RTD_Log(LOGGER_DEBUG, "secam l tuener is lock\n");

		rtdf_outl(VDC_FREQ_VALID_INNER_0x5220,0x42);
		rtdf_outl(VDC_SECAM_FLAG_COUNTER_MAX_0x5228,0xA0);
		rtdf_outl(VDC_SECAM_FLAG_COUNTER_MIN_0x522c,0x0D);
		rtdf_outl(VDC_AUTO_MODE_S_M_STATISTIC_0x51d8,0x0B);
        rtdf_outl(VDC_MANUAL_MODE_0x51c0, 0x00000007);

		//VDC initial code --
		//audio initial code
		drvif_ifd_set_audio_bp(_SIG_MODE_AUTO);


		#if 1
		fw_timer_event_DelayXms(20);
        if ( !drvif_ifd_get_agc_lock() )
			return 0;
		RTD_Log(LOGGER_LEVEL,"\n Tim agc lock \n");
        fw_timer_event_DelayXms(20);
        Status = (UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
        if(Status & _No_Sig)
        {
            if((Status&0x06)==0x00) // h&v unlock,prvent weak signal
                return  0;//ZCR_NOT_LOCK;
        }
		
		fw_timer_event_DelayXms(140);


		#else
        fw_timer_event_DelayXms(400);
		#endif
        agc_lock1 = (UINT8)(rtd_inl(AGC_LOCK_STATUS_0x5d40) & 0x1);
        agc_lock2 = drvif_ifd_get_agc_lock();
        clamp_lv = (UINT16)(rtd_inl(OFFSET_0x5c98) & 0x3fff);
        RTD_Log(LOGGER_DEBUG, "secam l tuner is lock @ freq = %d, agc_lock1 = %d, agc_lock2 = %d, clamp=%x\n", (UINT32)tvScanCurFreq, (UINT32)agc_lock1, (UINT32)agc_lock2, (UINT32)clamp_lv);
        if (drv_video_vdc_CheckHVisLocked())
        {
            //fw_timer_event_DelayXms(300);
            RTD_Log(LOGGER_DEBUG,"tuner is secam l and vd is lock ok %d\n", (UINT32)tvAutoScanFreq);
            ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
            RTD_Log(LOGGER_DEBUG,"tuner is secam l  the vd detect mode1= %x...\n", (UINT32)ucVdc_TV_Mode);
            secam_flag=rtdf_inl(VDC_VIDEO_STATUS_REG3_0x5010)&0x02;
            RTD_Log(LOGGER_DEBUG,"tuner is secam l  ,secam_flag=%x ??????........... \n",(UINT32) secam_flag);
            AScan_Noise_stas = drv_video_vdc_NoiseStatus();
            RTD_Log(LOGGER_DEBUG,"tuner is secam l  the vd  noise level=  %x....\n", (UINT32) AScan_Noise_stas);
            flag358 = rtdf_inl(VDC_VIDEO_STATUS_REG2_0x500c)&0x80;
            pal_flag = rtdf_inl(VDC_VIDEO_STATUS_REG2_0x500c)&0x06;
            RTD_Log(LOGGER_DEBUG,"tuner is secam l  the vd  flag358=  %x....\n",(UINT32) flag358);
            RTD_Log(LOGGER_DEBUG,"tuner is secam l  the vd pal_flag=  %x....\n",(UINT32) pal_flag);

/*
            ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
            RTD_Log(LOGGER_DEBUG,"TV AUTO SCAN mode is NTSC443 or SECAM 1=... %x\n", (UINT32)ucVdc_TV_Mode);
            AScan_Noise_stas = drv_video_vdc_NoiseStatus();
            AScan_Noise_stas1 = AScan_Noise_stas;
            RTD_Log(LOGGER_DEBUG,"TV AUTO SCAN mode is NTSC443 or SECAM noise ... %x\n",(UINT32) AScan_Noise_stas);
            RTD_Log(LOGGER_DEBUG, "VDC_VIDEO_STATUS_REG2_VADDR = %x\n", (UINT32) (rtdf_inl(VDC_VIDEO_STATUS_REG2_0x500c)));
            RTD_Log(LOGGER_DEBUG, "VDC_VIDEO_STATUS_REG3_VADDR = %x\n", (UINT32) (rtdf_inl(VDC_VIDEO_STATUS_REG3_0x5010)));
*/

            if ( (ucVdc_TV_Mode ==ZSECAM)&&(AScan_Noise_stas<=0x25)&&(secam_flag==0x02) )
            {
                RTD_Log(LOGGER_DEBUG,"secam l detect \n");
                return ZSECAML;

            }
            return  0;
        }
        else
        {
            RTD_Log(LOGGER_DEBUG,"secam l VDC h/v no lock \n");
            return 0;
        }
    }
    else
        return 0;
}
#if 0
UINT8 fwif_tv_autoscan_SECAMLA_funtion( UINT32 tvAutoScanFreq)
{
    UINT8 AScan_Noise_stas,Status;
    UINT8 agc_lock1 = 0;
    UINT8 agc_lock2 = 0;
    UINT16 clamp_lv = 0;

//	RTD_Log(LOGGER_DEBUG, "Tuner change to secam la \n");
    drvif_ifd_set_iftuner_setting(ZSECAMLA, tvAutoScanFreq, 5);	//suggest delay here should > 5
    rtdf_outl(VDC_MANUAL_MODE_0x51c0, 0x00000006);
    if ( drvif_ifd_get_cr_lock() == _TRUE )
    {
        RTD_Log(LOGGER_DEBUG, "secam la  tuener is lock\n");
#if 0
        if ( !drvif_ifd_get_agc_lock() )
            RTD_Log(LOGGER_DEBUG, "secam la agc no lock!! \n");
        RTD_Log(LOGGER_DEBUG, "secam la  RF AGC  %x \n", (UINT32)(rtdf_inl(IFD_AGC_RF_STATUS_VADDR)));
        RTD_Log(LOGGER_DEBUG, "secam la IF AGC  %x \n", (UINT32)(rtdf_inl(IFD_AGC_IF_STATUS_VADDR)));
        RTD_Log(LOGGER_DEBUG, "secam la dgain status 2 %x \n", (UINT32)(rtdf_inl(VDC_AGC_DGAIN_STATUS2_0x5260)));
        RTD_Log(LOGGER_DEBUG, "secam la dgain status 1 %x \n", (UINT32)(rtdf_inl(VDC_AGC_DGAIN_STATUS1_0x5264)));
        RTD_Log(LOGGER_DEBUG, "secam la  pal tuner is lock\n");
        RTD_Log(LOGGER_DEBUG, "secam la vd status = %x\n", (UINT32)(rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008)));
#endif

#if 1
        fw_timer_event_DelayXms(110);
        Status = (UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
        if (Status & _No_Sig)
            return  ZMODE_ERROR;

        fw_timer_event_DelayXms(200);
#else
        fw_timer_event_DelayXms(400);
#endif

        agc_lock1 = (UINT8)(rtd_inl(AGC_LOCK_STATUS_0x5d40) & 0x1);
        agc_lock2 = drvif_ifd_get_agc_lock();
        clamp_lv = (UINT16)(rtd_inl(OFFSET_0x5c98) & 0x3fff);
        RTD_Log(LOGGER_DEBUG, "secam la tuner is lock @ freq = %d, agc_lock1 = %d, agc_lock2 = %d, clamp=%x\n", (UINT32) tvAutoScanFreq, (UINT32)agc_lock1, (UINT32)agc_lock2, (UINT32)clamp_lv);

        if (drv_video_vdc_CheckHVisLocked())
        {
            fw_timer_event_DelayXms(100);
            RTD_Log(LOGGER_DEBUG,"tuner is secam la and vd is lock ok %d\n", (UINT32) tvAutoScanFreq);
            ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
            RTD_Log(LOGGER_DEBUG,"tuner is secam la  the vd detect mode1= %x...\n", (UINT32) ucVdc_TV_Mode);
            AScan_Noise_stas = drv_video_vdc_NoiseStatus();
            RTD_Log(LOGGER_DEBUG,"tuner is secam la  the vd  noise level=  %x....\n", (UINT32) AScan_Noise_stas);
            if ( (ucVdc_TV_Mode == ZSECAM)&&(AScan_Noise_stas<=0x25) )
            {
                RTD_Log(LOGGER_DEBUG,"secam la detect \n");
                return ZSECAMLA;

            }
            return  ZMODE_ERROR;
        }
        else
            return ZHV_NOT_LOCK;
    }
    else
        return ZCR_NOT_LOCK;
}
#endif
#endif
#endif
UINT8 fwif_tv_single_scan_NTSC_funtion( UINT32 tvAutoScanFreq)
{
    UINT8 AScan_Noise_stas;
    UINT16 Ratio;
    INT32 offset;
    UINT8 Status;
    UINT8 mn_status;

    RTD_Log(LOGGER_DEBUG,"single NTSC SCAN \n");
    drvif_ifd_set_iftuner_setting(ZNTSC, tvAutoScanFreq,5);
    rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000000);
    RTD_Log(LOGGER_DEBUG,"the tuner set ntsc frq ... %d\n", (UINT32) tvAutoScanFreq);
	if(GET_TV_TABLE_SCAN()) 
		fw_timer_event_DelayXms(150);
    Ratio = drv_tuner_GetRatioInStepSize();
    offset = drvif_tuner_get_afcoffset(0);

    //RTD_Log(LOGGER_LEVEL,"\n Tim1 Ratio= %d,offset = %d \n",(UINT32)Ratio,(UINT32)offset);
	if (( drvif_ifd_get_cr_lock() == _TRUE )&&((UINT32)(abs1(offset)) <= (UINT32)(10*Ratio)))//Tim 0402 //Tim 0404
	{
		drvif_ifd_set_audio_bp(_SIG_MODE_AUTO);
		/*
		#if 1
			fw_timer_event_DelayXms(110);
			Status = (UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
			if(Status & _No_Sig)
			{
				if((Status&0x06)==0x00) // h&v unlock,prvent weak signal
					return  ZMODE_ERROR;
			}
			fw_timer_event_DelayXms(200);
		#else
        fw_timer_event_DelayXms(400); // wait the the v sync and h sync is lock
		#endif
		*/
        fw_timer_event_DelayXms(20);
        if ( !drvif_ifd_get_agc_lock() )
			return 0;//ZCR_NOT_LOCK;
		RTD_Log(LOGGER_LEVEL,"\n Tim agc lock \n");
        fw_timer_event_DelayXms(20);
        Status = (UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
        if(Status & _No_Sig)
        {
            if((Status&0x06)==0x00) // h&v unlock,prvent weak signal
                return  0;//ZCR_NOT_LOCK;
        }

		fw_timer_event_DelayXms(140);
		
        RTD_Log(LOGGER_DEBUG,"ntsc tuner is lock\n");
        if ( drv_video_vdc_CheckHVisLocked() )
        {

            ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
            AScan_Noise_stas = drv_video_vdc_NoiseStatus();
            RTD_Log(LOGGER_DEBUG,"the ntsc noise  ... %x\n", (UINT32) AScan_Noise_stas);

            if (ucVdc_TV_Mode == ZNTSC)
            {
	            if (AScan_Noise_stas<=0xb6)
	                return ZNTSC;
            }

            if ( ucVdc_TV_Mode == ZPAL_M )
            {
                if (drvif_ifd_set_audio_bp(_SIG_MODE_NTSC) != 1)
                    fw_timer_event_DelayXms(200);
                Audio_AtvGetMNStatus(&mn_status, NULL);
                RTD_Log(LOGGER_DEBUG,"the pal m audio  tone=%x ??????........... \n", (UINT32) mn_status);
                if (mn_status == 1)
                {
                    return ZPAL_M;
                }
            }

            if (ucVdc_TV_Mode==ZPAL_N)
            {
                if (drvif_ifd_set_audio_bp(_SIG_MODE_NTSC) != 1)
                    fw_timer_event_DelayXms(100);

                Audio_AtvGetMNStatus(&mn_status, NULL);
                if (mn_status == 1)
                {
                    RTD_Log(LOGGER_DEBUG,"PAL N is ok ..\n");
                    return ZPAL_N;
                }
            }
            return 0;

        }
        else
            return 0;//ZHV_NOT_LOCK;
    }
    else
        return 0;//ZCR_NOT_LOCK;
}

UINT8 fwif_tv_autoscan_NTSC_funtion( UINT32 tvAutoScanFreq)
{
    UINT8 mn_status;
    UINT8 RetryCntNTSC = 0;
    UINT8 AScan_Noise_stas;
    UINT16 Ratio;
    INT32 offset;
    UINT8 Status;

    RTD_Log(LOGGER_DEBUG,"ZZZNNNTTTSSSCCC1111\n");
    drvif_ifd_set_iftuner_setting(ZNTSC, tvAutoScanFreq,5);
    rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000000);
    RTD_Log(LOGGER_DEBUG,"the tuner set ntsc frq ... %d\n", (UINT32) tvAutoScanFreq);
	if(GET_TV_TABLE_SCAN()) 
		fw_timer_event_DelayXms(150);
    Ratio = drv_tuner_GetRatioInStepSize();
    offset = drvif_tuner_get_afcoffset(0);
    //rtdf_maskl(DISPLAY_TIMING_CTRL1_0x2000, ~(_BIT26 ), 0);
    //RTD_Log(LOGGER_LEVEL,"\n Tim1 Ratio= %d,offset = %d \n",(UINT32)Ratio,(UINT32)offset);
	if (( drvif_ifd_get_cr_lock() == _TRUE )&&((UINT32)(abs1(offset)) <= (UINT32)(10*Ratio)))//Tim 0402 //Tim 0404
	{
		drvif_ifd_set_audio_bp(_SIG_MODE_AUTO);
		/*
		#if 1
			fw_timer_event_DelayXms(110);
			Status = (UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
			if(Status & _No_Sig)
			{
				if((Status&0x06)==0x00) // h&v unlock,prvent weak signal
					return  ZMODE_ERROR;
			}
			fw_timer_event_DelayXms(200);
		#else
        fw_timer_event_DelayXms(400); // wait the the v sync and h sync is lock
		#endif
		*/
		fw_timer_event_DelayXms(20);
        if ( !drvif_ifd_get_agc_lock() )
			return ZCR_NOT_LOCK;
		RTD_Log(LOGGER_LEVEL,"\n Tim agc lock \n");
        fw_timer_event_DelayXms(20);
        Status = (UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
        if(Status & _No_Sig)
        {
            if((Status&0x06)==0x00) // h&v unlock,prvent weak signal
                return  ZCR_NOT_LOCK;//ZCR_NOT_LOCK;
        }
		
		fw_timer_event_DelayXms(140);
		
        RTD_Log(LOGGER_DEBUG,"ntsc tuner is lock\n");
        if ( drv_video_vdc_CheckHVisLocked() )
        {
        //rtdf_maskl(DISPLAY_TIMING_CTRL1_0x2000, ~(_BIT26 ), 0);
            RTD_Log(LOGGER_DEBUG,"the tuner  ntsc mode  and the vd are  lock\n");
            RetryCntNTSC =1;
            //ScalerTimer_DelayXms(200); // wait the vd  mode,  noise of vd are stable
            do
            {
                ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
                RTD_Log(LOGGER_DEBUG,"the tuner set ntsc  the mode1= ... %x\n", (UINT32) ucVdc_TV_Mode);
                AScan_Noise_stas = drv_video_vdc_NoiseStatus();
                AScan_Noise_stas2 = AScan_Noise_stas;
                RTD_Log(LOGGER_DEBUG,"the ntsc noise  ... %x\n", (UINT32) AScan_Noise_stas);
                if (ucVdc_TV_Mode == ZPAL_60)
                {
                    fw_timer_event_DelayXms(300);
                    ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
                    RTD_Log(LOGGER_DEBUG,"the tuner set ntsc  the mode2= ... %x\n", (UINT32) ucVdc_TV_Mode);
                }
                if (ucVdc_TV_Mode == ZNTSC)
                {
#ifdef TV_PAL_NTSC_TH
                    if (AScan_Noise_stas<=0xb6)
                        return ZNTSC;
#else //#ifdef TV_PAL_NTSC_TH
                    return ZNTSC;
#endif //#ifdef TV_PAL_NTSC_TH
                }
                if (ucVdc_TV_Mode == ZNTSC_443)
                {
#ifdef TV_PAL_NTSC_TH
                    if (AScan_Noise_stas<=0xb6)
                        return ZNTSC_443;
#else //#ifdef TV_PAL_NTSC_TH
                    return ZNTSC_443;
#endif //#ifdef TV_PAL_NTSC_TH
                }
                if (ucVdc_TV_Mode==ZPAL_N)
                {
                	
                    //rtdf_maskl(DISPLAY_TIMING_CTRL1_0x2000, ~(_BIT26 ), 0);
                    //fw_timer_event_DelayXms(20);
                    //rtdf_maskl(DISPLAY_TIMING_CTRL1_0x2000, ~(_BIT26 ), 0);rtdf_maskl(DISPLAY_TIMING_CTRL1_0x2000, ~(_BIT26 ), 0);
                    //rtdf_maskl(DISPLAY_TIMING_CTRL1_0x2000, ~(_BIT26 ), 0);

                    if (drvif_ifd_set_audio_bp(_SIG_MODE_NTSC) != 1)
                        fw_timer_event_DelayXms(200);
                                        //rtdf_maskl(DISPLAY_TIMING_CTRL1_0x2000, ~(_BIT26 ), 0);
                    //fw_timer_event_DelayXms(20);

                    Audio_AtvGetMNStatus(&mn_status, NULL);
                    if (mn_status == 1)
                    {

                        RTD_Log(LOGGER_DEBUG,"PAL N is ok ..\n");
                        return ZPAL_N;
                    }
                }

                if ( ucVdc_TV_Mode == ZPAL_M )
                {
                    if (drvif_ifd_set_audio_bp(_SIG_MODE_NTSC) != 1)
                        fw_timer_event_DelayXms(200);
                    Audio_AtvGetMNStatus(&mn_status, NULL);
                    RTD_Log(LOGGER_DEBUG,"the pal m audio  tone=%x ??????........... \n", (UINT32) mn_status);
                    if (mn_status == 1)
                    {
                        return ZPAL_M;
                    }
                }
            }
            while (--RetryCntNTSC);

            return  ZMODE_ERROR;
        }
        else
            return ZHV_NOT_LOCK;
    }
    else
        return ZCR_NOT_LOCK;
}


#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
#if defined(CONFIG_APP_EURO)
UINT8 fwif_tv_auto_SECAML_funtion( UINT32 tvAutoScanFreq)
{
    UINT8 afc = 0;
    UINT8 AScan_Noise_stas;
    UINT8 ucAutoMode;

    drvif_ifd_set_video_standard(ZSECAML, tvAutoScanFreq);//drvif_ifd_set_iftuner_setting(ZSECAML, tvAutoScanFreq, 10);	//suggest delay here should > 5
    fw_timer_event_DelayXms(10);
    rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000006);
    if ( drvif_ifd_get_cr_lock() == _TRUE )
    {
        RTD_Log(LOGGER_DEBUG, "secam l tuener is lock\n");
        if ( !drvif_ifd_get_agc_lock() )
            RTD_Log(LOGGER_DEBUG, "secam l agc no lock!! \n");
        RTD_Log(LOGGER_DEBUG, "secam l RF AGC  %x \n", (UINT32)rtdf_inl(IFD_AGC_RF_STATUS_VADDR));
        RTD_Log(LOGGER_DEBUG, "secam l IF AGC  %x \n", (UINT32)rtdf_inl(IFD_AGC_IF_STATUS_VADDR));
        RTD_Log(LOGGER_DEBUG, "secam l dgain status 2 %x \n", (UINT32)rtdf_inl(VDC_AGC_DGAIN_STATUS2_VADDR));
        RTD_Log(LOGGER_DEBUG, "secam l dgain status 1 %x \n", (UINT32)rtdf_inl(VDC_AGC_DGAIN_STATUS1_VADDR));
        RTD_Log(LOGGER_DEBUG, "secam l pal tuner is lock\n");
        RTD_Log(LOGGER_DEBUG, "secam l vd status = %x\n", (UINT32)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008));
        fw_timer_event_DelayXms(400);
        if (drv_video_vdc_CheckHVisLocked())
        {
            fw_timer_event_DelayXms(100);
            RTD_Log(LOGGER_DEBUG,"tuner is secam l and vd is lock ok %d\n",(UINT32)tvAutoScanFreq);
            ucAutoMode = (UINT8)(rtdf_inl(VDC_MODE_DETECTION_STATUS_0x51c4)&0x7);
            RTD_Log(LOGGER_DEBUG,"tuner is secam l  the vd detect mode= %x...\n", (UINT32)ucVdc_TV_Mode);
            AScan_Noise_stas = drv_video_vdc_NoiseStatus();
            RTD_Log(LOGGER_DEBUG,"tuner is secam l  the vd  noise level=  %x....\n",(UINT32)AScan_Noise_stas);
            if ( (ucAutoMode==0x07)&&(AScan_Noise_stas<=0x25) )
            {
                rtd_outl(VDC_MANUAL_MODE_0x51c0, 0x00000007);
                RTD_Log(LOGGER_DEBUG,"secam l detect \n");
                return ZSECAML;

            }
            return  ZMODE_ERROR;
        }
        else
            return ZHV_NOT_LOCK;

    }
    else
        return ZCR_NOT_LOCK;


}

UINT8 fwif_tv_auto_SECAMLA_funtion( UINT32 tvAutoScanFreq)
{
    INT8 afc = 0;
    UINT8 AScan_Noise_stas;
    UINT8 ucAutoMode;

    if ((tvAutoScanFreq>420000)&&(tvAutoScanFreq<1050000))
    {	// linmc@20100106, recovery frequency checking in secam
        drvif_ifd_set_video_standard(ZSECAMLA,tvAutoScanFreq);//drvif_ifd_set_iftuner_setting(ZSECAMLA, tvAutoScanFreq, 10);	//suggest delay here should > 5
        fw_timer_event_DelayXms(10);
        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000006);
        if ( drvif_ifd_get_cr_lock() == _TRUE )
        {
            RTD_Log(LOGGER_DEBUG, "secam la  tuener is lock\n");
            if ( !drvif_ifd_get_agc_lock() )
                RTD_Log(LOGGER_DEBUG, "secam la agc no lock!! \n");
            RTD_Log(LOGGER_DEBUG, "secam la  RF AGC  %x \n", (UINT32)rtdf_inl(IFD_AGC_RF_STATUS_VADDR));
            RTD_Log(LOGGER_DEBUG, "secam la IF AGC  %x \n", (UINT32)rtdf_inl(IFD_AGC_IF_STATUS_VADDR));
            RTD_Log(LOGGER_DEBUG, "secam la dgain status 2 %x \n", (UINT32)rtdf_inl(VDC_AGC_DGAIN_STATUS2_VADDR));
            RTD_Log(LOGGER_DEBUG, "secam la dgain status 1 %x \n", (UINT32)rtdf_inl(VDC_AGC_DGAIN_STATUS1_VADDR));
            RTD_Log(LOGGER_DEBUG, "secam la  pal tuner is lock\n");
            RTD_Log(LOGGER_DEBUG, "secam la vd status = %x\n", (UINT32)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008));
            fw_timer_event_DelayXms(400);
            if ( drv_video_vdc_CheckHVisLocked() )
            {
                fw_timer_event_DelayXms(100);
                RTD_Log(LOGGER_DEBUG,"tuner is secam la and vd is lock ok %d\n",(UINT32)tvAutoScanFreq);
                ucAutoMode = (UINT8)(rtdf_inl(VDC_MODE_DETECTION_STATUS_0x51c4)&0x7);
                RTD_Log(LOGGER_DEBUG,"tuner is secam la  the vd detect mode= %x...\n", (UINT32) ucVdc_TV_Mode);
                AScan_Noise_stas = drv_video_vdc_NoiseStatus();
                RTD_Log(LOGGER_DEBUG,"tuner is secam la  the vd  noise level=  %x....\n", (UINT32) AScan_Noise_stas);

                if ( (ucAutoMode==0x07)&&(AScan_Noise_stas<=0x25) )
                {
                    RTD_Log(LOGGER_DEBUG,"secam la  detect \n");
                    rtd_outl(VDC_MANUAL_MODE_0x51c0, 0x00000007);
                    return ZSECAMLA;

                }
                return  ZMODE_ERROR;
            }
            else
                return ZHV_NOT_LOCK;

        }
        else
            return ZCR_NOT_LOCK;
    }
    return ZFRQ_NOT_IN_BAND;

}
#endif //#ifndef CONFIG_TUNER_SILICON
#endif //#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)

#endif  //CONFIG_IFD_ENABLE


UINT8 fwif_tv_autoscanmodedetect(void)
{

#ifdef CONFIG_IFD_ENABLE  //ifd flow
////////////////////////   all  std //////////////////////////////////////////
#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
#if 1//defined(CONFIG_APP_EURO)
    UINT8 ifd_secaml_vd_mode;
    //UINT8 ifd_secamla_vd_mode;
#endif
#endif
    UINT8 ifd_pal_vd_mode;
    UINT8 ifd_ntsc_vd_mode;
    UINT8 hv_lock = 0;
    UINT8 AScan_Noise_stas;
    UINT32 flag625 = 0;

    drv_module_vdc_Set_CKill_Mode(CKill_AUTO);	//jj 20091210
    

    ifd_pal_vd_mode = fwif_tv_autoscan_PAL_funtion(tvScanCurFreq);
    //RTD_Log(LOGGER_LEVEL,"\nTim ifd_pad_vd_mode=%d \n",(UINT32)ifd_pal_vd_mode);
    if (ifd_pal_vd_mode !=ZCR_NOT_LOCK)
    {
        if (ifd_pal_vd_mode !=ZHV_NOT_LOCK)
        {
            if ((ifd_pal_vd_mode !=ZMODE_ERROR) && (ifd_pal_vd_mode != ZMODE_525_STD))
            {
                return ifd_pal_vd_mode;
            }
            else
            {
                if (ifd_pal_vd_mode == ZMODE_525_STD)
                {
                    ifd_ntsc_vd_mode = fwif_tv_autoscan_NTSC_funtion(tvScanCurFreq);
                    if ((ifd_ntsc_vd_mode != ZMODE_ERROR)&&(ifd_ntsc_vd_mode != ZCR_NOT_LOCK)&&(ifd_ntsc_vd_mode != ZHV_NOT_LOCK))
                        return ifd_ntsc_vd_mode;
                }
                else
                {
					#if defined(CONFIG_APP_EURO)
					#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
	                {
	                    //RTD_Log(LOGGER_DEBUG, "the tuner set pal ,h,v,cr=lock,but mode is error,change the secam l \n");
	                    ifd_secaml_vd_mode = fwif_tv_autoscan_SECAML_funtion(tvScanCurFreq);
	                    if (ifd_secaml_vd_mode == ZSECAML)
	                        return ZSECAML;
	                    //RTD_Log(LOGGER_DEBUG, "the tuner set pal ,h,v,cr=lock,but mode is error,change the secam la \n");
	                    /*if (tvScanCurFreq<=1422500)//tommy, secam lA seq  under 142.25Mhz
	                    {
	                        ifd_secamla_vd_mode = fwif_tv_autoscan_SECAMLA_funtion(tvScanCurFreq);
	                        if (ifd_secamla_vd_mode == ZSECAMLA)
	                            return ZSECAMLA;
	                    }*/
	                }
					#endif
					#endif
				}
            }
        }
        else
        {
		/*#if defined(CONFIG_APP_EURO)
		#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
            //RTD_Log(LOGGER_DEBUG, "the tuner set pal cr=lock,but h,v are not lock,change the secam l \n");
            ifd_secaml_vd_mode = fwif_tv_autoscan_SECAML_funtion(tvScanCurFreq);
            if (ifd_secaml_vd_mode == ZSECAML)
                return ZSECAML;
            //RTD_Log(LOGGER_DEBUG, "the tuner set pal cr=lock,but h,v are not lock,change the secam la \n");
            if (tvScanCurFreq<=1422500)//tommy ,secam lA seq  under 142.25Mhz
            {
                ifd_secamla_vd_mode = fwif_tv_autoscan_SECAMLA_funtion(tvScanCurFreq);
                if (ifd_secamla_vd_mode == ZSECAMLA)
                    return ZSECAMLA;
            }
		#endif
            return 0;
		#endif*/
        }
    }
    else
    {
        //RTD_Log(LOGGER_DEBUG, "the tuner set pal cr  h,v  are  not lock,change the secam l \n");
        /*ifd_secaml_vd_mode = fwif_tv_autoscan_SECAML_funtion(tvScanCurFreq);
        if(ifd_secaml_vd_mode == ZSECAML)
        	return ZSECAML;
        RTD_Log(LOGGER_DEBUG, "the tuner set pal cr  h,v  are  not lock,change the secam la \n");
        ifd_secamla_vd_mode = fwif_tv_autoscan_SECAMLA_funtion(tvScanCurFreq);
        if(ifd_secamla_vd_mode == ZSECAMLA)
        	return ZSECAMLA;*/
        return 0;
    }

    //if((ifd_pal_vd_mode ==ZMODE_ERROR) || ((ifd_pal_vd_mode ==ZMODE_525_STD)&&(ifd_ntsc_vd_mode == ZMODE_ERROR)))
    if ((ifd_pal_vd_mode ==ZMODE_ERROR) &&(ifd_ntsc_vd_mode == ZMODE_ERROR))
    {
        AScan_Noise_stas = drv_video_vdc_NoiseStatus();
        RTD_Log(LOGGER_DEBUG, "the final noise  ... %x,%x,%x\n", (UINT32) AScan_Noise_stas);
        hv_lock = 1;
        flag625 = (UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG3_0x5010);
        flag625 = flag625 & 0x04;
        if (flag625 == 0x04)
        {
            RTD_Log(LOGGER_DEBUG, "h v lock  is pal ..\n");
            if (AScan_Noise_stas > 0xf0)
            {
                RTD_Log(LOGGER_DEBUG, "the final noise level is 0xff, so ignore  ..\n");
                return 0;
            }
            return ZPAL_I;
        }
        else if (ifd_pal_vd_mode ==ZMODE_525_STD)
        {
            RTD_Log(LOGGER_DEBUG, "h v lock  is ntsc ..\n");
            if (AScan_Noise_stas > 0xf0) //irene@090312
            {
                RTD_Log(LOGGER_DEBUG, "the final noise level is 0xff, so ignore  ..\n");
                return 0;
            }
            return ZNTSC;
        }
        else
            return 0;
    }
    else
        return 0;

#endif //#ifdef  CONFIG_IFD_ENABLE

}

void TVSetAudioStandard(UINT8 soundSystem)
{
    //ATV_SOUND_INFO	t_sound_info;
    UINT8 CurChn;
    UINT8 video_std;
	CurChn = GET_TV_CHN_DATA(_CUR_CHN_TV);
	
     video_std = GET_CHANNEL_COLORSYSTEM(CurChn);
    switch (soundSystem)
    {
    case ATV_SOUND_STD_BG_MONO:
    case ATV_SOUND_STD_A2_BG:
    case ATV_SOUND_STD_NICAM_BG:
        drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_BG);
        fw_timer_event_DelayXms(300);	// wait Tuner stable
        Audio_AtvSetSoundStd(soundSystem);
        break;
    case ATV_SOUND_STD_DK_MONO:
    case ATV_SOUND_STD_A2_DK1:
    case ATV_SOUND_STD_A2_DK2:
    case ATV_SOUND_STD_A2_DK3:
    case ATV_SOUND_STD_NICAM_DK:
        drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_DK);
        fw_timer_event_DelayXms(300);	// wait Tuner stable
        Audio_AtvSetSoundStd(soundSystem);
        break;
    case ATV_SOUND_STD_FM_MONO_NO_I:
    case ATV_SOUND_STD_NICAM_I:
        drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_I);
        fw_timer_event_DelayXms(300);	// wait Tuner stable
        Audio_AtvSetSoundStd(soundSystem);
        break;
    case ATV_SOUND_STD_AM_MONO:
    case ATV_SOUND_STD_NICAM_L:
        drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_L);
        fw_timer_event_DelayXms(300);	// wait Tuner stable
        Audio_AtvSetSoundStd(soundSystem);
        break;
        
    case ATV_SOUND_STD_PAL_N:
    case ATV_SOUND_STD_MN_MONO:
    case ATV_SOUND_STD_BTSC:
    case ATV_SOUND_STD_A2_M:
    case ATV_SOUND_STD_EIAJ:
        drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_MN);
        Audio_AtvSetSoundStd(soundSystem);
        break;
    case ATV_SOUND_STD_AUTO:
    default:
        if (video_std == ZAUTO)
            video_std = ucVdc_TV_Mode;
        RTD_Log(LOGGER_DEBUG, "TVSetAudioStandard video_std = %x\n", (UINT32)video_std);
        if ((video_std == ZNTSC)||(video_std == ZPAL_M)||(video_std == ZPAL_N))
            drvif_ifd_set_audio_bp(_SIG_MODE_NTSC);
        else
            drvif_ifd_set_audio_bp(_SIG_MODE_PAL);
        Audio_AtvSetSoundStd(ATV_SOUND_STD_AUTO);
        break;
    }
}

void TVSetAudioA2MTS(UINT8 mode)
{
    UINT8 sound_mode;

    Audio_AtvGetSoundMode(&sound_mode);
    RTD_Log(LOGGER_DEBUG, "Get sound mode = %x\n", (UINT32)sound_mode);

    if (mode == ATV_SOUND_MODE_MONO)
    {
        if (sound_mode & _BIT1) // Dual  detected
            Audio_AtvSetSoundSelect(ATV_SOUND_SEL_STEREO_A);
        else
            Audio_AtvSetSoundSelect(mode);
    }
    else if (mode == ATV_SOUND_MODE_STEREO)
    {
        if (sound_mode & _BIT0)
        {
            Audio_AtvSetSoundSelect(mode);
        }
        else
        {
            Audio_AtvSetSoundSelect(ATV_SOUND_SEL_STEREO_A);
        }
    }
    else 	// Dual
    {
        if (sound_mode & _BIT1)
        {
            Audio_AtvSetSoundSelect(ATV_SOUND_SEL_STEREO_B);
        }
        else
        {
            Audio_AtvSetSoundSelect(ATV_SOUND_SEL_FMAM);
        }
    }

}

void TVSetAudioNicamMTS(UINT8 mode)
{
    UINT8 sound_mode;

    sound_mode = rtdf_readByte3(NICAM_DECODE_STATUS_0x3514);
    sound_mode = (sound_mode >>4)& 0x07;
    RTD_Log(LOGGER_DEBUG, "Get sound mode = %x\n", (UINT32)sound_mode);

    if (mode == ATV_SOUND_MODE_MONO)
    {
        if (sound_mode & _BIT1) // Dual  detected
            Audio_AtvSetSoundSelect(ATV_SOUND_SEL_STEREO_A);
        else if (sound_mode & _BIT2) // Mono  detected
            Audio_AtvSetSoundSelect(ATV_SOUND_SEL_STEREO_A); //Nicam mono
        else	// Stereo
            Audio_AtvSetSoundSelect(mode);	// analog mono
    }
    else if (mode == ATV_SOUND_MODE_STEREO)
    {
        if (sound_mode & _BIT1) // dual detected
        {
            Audio_AtvSetSoundSelect(ATV_SOUND_SEL_STEREO_A);
        }
        else
        {
            Audio_AtvSetSoundSelect(mode);
        }
    }
    else 	// Dual
    {
        if (sound_mode & _BIT1) // dual detected
        {
            Audio_AtvSetSoundSelect(ATV_SOUND_SEL_STEREO_B);
        }
        else if (sound_mode & _BIT2) // Mono  detected
        {
            Audio_AtvSetSoundSelect(ATV_SOUND_SEL_STEREO_A); //Nicam mono
        }
        else
        {
            Audio_AtvSetSoundSelect(ATV_SOUND_SEL_FMAM);
        }
    }
}

void TVSetAudioMTS(UINT8 mode)
{
    UINT8 sound_std;
    UINT8 CurChn;
	CurChn = GET_TV_CHN_DATA(_CUR_CHN_TV);
	
    sound_std = GET_CHANNEL_SOUNDSYSTEM(CurChn);
    switch (sound_std)
    {
    case ATV_SOUND_STD_PAL_N:
    case ATV_SOUND_STD_A2_BG:
    case ATV_SOUND_STD_A2_DK1:
    case ATV_SOUND_STD_A2_DK2:
    case ATV_SOUND_STD_A2_DK3:
    case ATV_SOUND_STD_A2_M:
    case ATV_SOUND_STD_EIAJ:
        TVSetAudioA2MTS(mode);
        break;

    case ATV_SOUND_STD_NICAM_I:
    case ATV_SOUND_STD_NICAM_DK:
    case ATV_SOUND_STD_NICAM_L:
    case ATV_SOUND_STD_NICAM_BG:
        TVSetAudioNicamMTS(mode);
        break;
    case ATV_SOUND_STD_AM_MONO:
    case ATV_SOUND_STD_MN_MONO:
    case ATV_SOUND_STD_BG_MONO:
    case ATV_SOUND_STD_DK_MONO:
    case ATV_SOUND_STD_FM_MONO_NO_I:
    case ATV_SOUND_STD_BTSC:
    default:
        TVSetAudioA2MTS(ATV_SOUND_MODE_MONO);
        break;


    }

}

#if 0
UINT8 fwif_tv_autoscanmodedetect(void)
{
    UINT8 RetryCntPAL = 0;
    UINT8 hv_lock = 0;
    UINT8 RetryCntNTSC = 0;
    UINT8 lock = 0;
    UINT32 flag625 = 0;
    UINT8 AScan_Noise_stas;
    UINT8 AScan_Noise_stas1;
    UINT8 AScan_Noise_stas2;
    UINT32 secamdk_tvScanCurFreq;
    UINT8 hv_lock_mode=0;  //irene

    UINT8 agc_lock1 = 0;
    UINT8 agc_lock2 = 0;
    UINT16 clamp_lv = 0;


#ifdef AUDIO_USE_DISABLE
    UINT8 color_detect;
    UINT8 ntsc443_color;
#else
    UINT8 mn_status;
    UINT8 pal_audio_detect = 0;
    UINT8 color_detect;
#endif
    RetryCntNTSC = 0;
    RetryCntPAL = 0;
    lock = 0;
    flag625 = 0;
    hv_lock = 0;
    hv_lock_mode=0;

    drvif_ifd_set_iftuner_setting(ZPAL_I, tvScanCurFreq, 16);

    rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000006);
    //RTD_Log(LOGGER_DEBUG, "tuner set frq %d\n", tvScanCurFreq);

    if (drvif_ifd_get_cr_lock()==_TRUE)
    {
        fw_timer_event_DelayXms(250);
        agc_lock1 = (UINT8)(rtd_inl(AGC_LOCK_STATUS_0x5d40) & 0x1);
        agc_lock2 = drvif_ifd_get_agc_lock();
        clamp_lv = (UINT16)(rtd_inl(OFFSET_0x5c98) & 0x3fff);
        RTD_Log(LOGGER_DEBUG, "pal tuner is lock @ freq = %d, agc_lock1 = %d, agc_lock2 = %d, clamp=%x\n", (UINT32)tvScanCurFreq, (UINT32)agc_lock1, (UINT32)agc_lock2, (UINT32)clamp_lv);
#ifdef CONFIG_ENABLE_TV_NOISE_BOX_ENALBE
        if (Auto_Scan_videoislocked_Tv_noise_box())
#else
        if (drv_video_vdc_CheckHVisLocked())
#endif
        {
            //AScan_Noise_stas = drv_video_vdc_NoiseStatus();
            //if (AScan_Noise_stas > 0xd8)
            //{
            //	RTD_Log(LOGGER_DEBUG,"noise status = %x\n",(UINT32)AScan_Noise_stas);
            //	return 0;
            //}
#ifdef CONFIG_ENABLE_TV_NOISE_BOX_ENALBE
            fw_timer_event_DelayXms(500);
#else
            fw_timer_event_DelayXms(150);
#endif

            lock=1;
            RTD_Log(LOGGER_DEBUG,"tuner and vd is lock ok %d\n",tvScanCurFreq);
            RetryCntPAL =1;
            do
            {
                RTD_Log(LOGGER_DEBUG,"reg.5c80=%x\n",(UINT32)rtdf_inl(VIDEO_BW_SEL_0x5c80));
                ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
                RTD_Log(LOGGER_DEBUG,"TV AUTO SCAN mode is NTSC443 or SECAM ... %x\n", (UINT32)ucVdc_TV_Mode);
                AScan_Noise_stas = drv_video_vdc_NoiseStatus();
                RTD_Log(LOGGER_DEBUG,"TV AUTO SCAN mode is NTSC443 or SECAM noise ... %x\n", (UINT32)AScan_Noise_stas);

#ifdef AUDIO_USE_DISABLE
                if ((ucVdc_TV_Mode==ZNTSC_443)&&(AScan_Noise_stas<=0x12))
                {
                    SET_CHANNEL_COLORSYSTEM(tvScanCurChn, ZNTSC_443);
                    SET_CHANNEL_SOUNDSYSTEM(tvScanCurChn, _SOUND_MN);
                    //stUserDataChannelSettings[tvScanCurChn-1].colorSystem=ZNTSC_443;
                    //stUserDataChannelSettings[tvScanCurChn-1].soundSystem= _SOUND_MN;
                    break;
                }
                if ((ucVdc_TV_Mode==ZNTSC_443)&&(AScan_Noise_stas>=0x13))
                {
                    color_detect=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
                    if ((color_detect&0x08)== 0x08)
                    {
                        ntsc443_color=1;
                    }
                    else
                        ntsc443_color=0;
                }
#else//use the audio  to select the mode
                if ((ucVdc_TV_Mode==ZNTSC_443)&&(AScan_Noise_stas<=0x20))
                {
                    fw_timer_event_DelayXms(200);  // wait the color of vd and audio are stable
                    color_detect=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
                    RTD_Log(LOGGER_DEBUG,"?? VDC_VIDEO_STATUS_REG1_VADDR = %x\n", (UINT32)color_detect);
                    if ((color_detect&0x08)== 0x08)
                    {
                        Audio_AtvGetMNStatus(&mn_status, NULL);
                        if (mn_status == 0)
                        {
                            RTD_Log(LOGGER_DEBUG,"ntsc443 is ok\n");
                            return ZNTSC_443;
                        }
                    }
                }
#endif
                if ((ucVdc_TV_Mode==ZPAL_N)&&(AScan_Noise_stas<=0x06))
                {
#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
                    if (_TRUE == fwif_tv_autoscan_check_new_vlock(2))
                        return ZPAL_N;
#else
                    return ZPAL_N;
#endif
                }

                if ((ucVdc_TV_Mode==ZSECAM)&&(AScan_Noise_stas<=0x1F))
                {
#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
                    if (_TRUE == fwif_tv_autoscan_check_new_vlock(2))
                        return ZSECAM;
#else
                    return ZSECAM;
#endif

                }

#if 0
                secamdk_tvScanCurFreq = tvScanCurFreq + SECAM_FRQ;
                drvif_ifd_set_iftuner_setting(ZPAL_I,secamdk_tvScanCurFreq,165);
                ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
                AScan_Noise_stas = drv_video_vdc_NoiseStatus();
                if (ucVdc_TV_Mode == ZSECAM)
                {
                    return ZSECAM;
                }
#endif

                //drvif_ifd_set_iftuner_setting(ZPAL_I,tvScanCurFreq,300);
                //ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
                //RTD_Log(LOGGER_DEBUG,"TV AUTO SCAN mode is  ... %x\n", (UINT32)ucVdc_TV_Mode);
#ifdef TV_PAL_NTSC_TH
                AScan_Noise_stas = drv_video_vdc_NoiseStatus();
                RTD_Log(LOGGER_DEBUG,"the pal noise  ... %x\n",AScan_Noise_stas);
                if ((ucVdc_TV_Mode==ZPAL_I)&&(AScan_Noise_stas<=0xb6))
#else
                if (ucVdc_TV_Mode==ZPAL_I)
#endif
                {
#ifndef AUDIO_USE_DISABLE
                    Audio_AtvGetMNStatus(&mn_status, NULL);
                    if (mn_status == 0)
                    {
                        pal_audio_detect=1;
                    }
#endif
                    if (AScan_Noise_stas>=0x30)
                    {
                        fw_timer_event_DelayXms(200);
                        AScan_Noise_stas = drv_video_vdc_NoiseStatus();
                        //leo 20090217 add for audio tone be identified as video
                        AScan_Noise_stas1 = AScan_Noise_stas;
                        RTD_Log(LOGGER_DEBUG,"the pal noise is too big so to do one  ... %x\n", (UINT32)AScan_Noise_stas);
                    }
#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
                    if (_FALSE == fwif_tv_autoscan_check_new_vlock(2))
                        break;
#endif

#ifdef AUDIO_USE_DISABLE
                    if (AScan_Noise_stas<=0x30)
#else//use the audio  to select the mode
                    if ((AScan_Noise_stas<=0x30)&&(pal_audio_detect==1))
#endif
                    {
                        RTD_Log(LOGGER_DEBUG,"pal detect \n");
                        return ZPAL_I;
                        break;
                    }
#ifdef HISENSE_TV_SECAM_SIGNAL
                    else
                    {
                        secamdk_tvScanCurFreq = tvScanCurFreq - SECAM_FRQ;
                        drvif_ifd_set_iftuner_setting(ZPAL_I, secamdk_tvScanCurFreq, 300);
//						ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
                        ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
                        AScan_Noise_stas = drv_video_vdc_NoiseStatus();
                        RTD_Log(LOGGER_DEBUG,"the tuner set pal but may be the pal n  mode is ... %x\n", (UINT32)ucVdc_TV_Mode);
                        RTD_Log(LOGGER_DEBUG,"the tuner set pal but may be the pal n  ... %x\n", (UINT32)AScan_Noise_stas);

                        if ((ucVdc_TV_Mode==ZPAL_N)&&(AScan_Noise_stas<=0x10))
                        {
#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
                            if (_TRUE == fwif_tv_autoscan_check_new_vlock(2))
                                return  ZPAL_N;
#else
                            return  ZPAL_N;
#endif
                        }
                        else if ((ucVdc_TV_Mode==ZNTSC)&&(AScan_Noise_stas<=0x10))
                        {
                            return ZNTSC;
                        }
                        else
                        {

                            if ((ucVdc_TV_Mode==ZPAL_I)&&(AScan_Noise_stas<=0x15))
                            {
#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
                                if (_TRUE == fwif_tv_autoscan_check_new_vlock(2))
                                    return  ZPAL_I;
#else
                                return  ZPAL_I;
#endif
                            }

                        }
                    }
#endif
                }

#ifdef CONFIG_SUPPORT_PAL60
                if (ucVdc_TV_Mode == ZPAL_60)
                {
                    fw_timer_event_DelayXms(500);//wait the the audio  is stable
                    fw_timer_event_DelayXms(500);//wait the the v sync and h sync is lock
                    Audio_AtvGetMNStatus(&mn_status, NULL);
                    if (mn_status == 0)
                    {
                        RTD_Log(LOGGER_DEBUG,"pal60 detect \n");
                        return  ZPAL_60;
                    }
                }
#endif

#if 0
                if ((ucVdc_TV_Mode == ZNTSC_50)&&(AScan_Noise_stas<=0x05))
                {
                    RTD_Log(LOGGER_DEBUG,"ntsc50 detect \n");
                    return	ZPAL_I;
                }
                if ((ucVdc_TV_Mode==ZPAL_N)&&(AScan_Noise_stas<=0x13))
                {
                    secamdk_tvScanCurFreq=tvScanCurFreq +_TUNER_MANUAL_SCAN_MIN_SETP;

                    drvif_ifd_set_iftuner_setting(ZPAL_I,secamdk_tvScanCurFreq,165);
                    ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
                    AScan_Noise_stas = drv_video_vdc_NoiseStatus();
                    RTD_Log(LOGGER_DEBUG,"the tuner set pal the frq is +4	but may be the pal i  mode is ... %x\n",(UINT32)ucVdc_TV_Mode);
                    RTD_Log(LOGGER_DEBUG,"the tuner set pal but may be the pal n  ... %x\n",(UINT32)AScan_Noise_stas);
                    if (ucVdc_TV_Mode==ZPAL_I)
                    {
                        RTD_Log(LOGGER_DEBUG,"pal the frq +4 detect \n");
                        return	ZPAL_I;
                    }
                }
#endif
            }
            while (--RetryCntPAL);
        }
        else
            return 0;
    }
    else
        return 0;

    if ((RetryCntPAL==0)&&(lock==1)) //Ntsc unlocked or NTSC lock but mode error ,chihwei@070722
    {
        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000000);
        drvif_ifd_set_iftuner_setting(ZNTSC, tvScanCurFreq, 30);
        RTD_Log(LOGGER_DEBUG,"the tuner set ntsc frq ... %d\n", tvScanCurFreq);

        if (drvif_ifd_get_cr_lock()==_TRUE)
        {
            fw_timer_event_DelayXms(280);// wait the the v sync and h sync is lock
            RTD_Log(LOGGER_DEBUG,"ntsc tuner is lock\n");
            if (drv_video_vdc_CheckHVisLocked())
            {
                RTD_Log(LOGGER_DEBUG,"the tuner  ntsc mode  and the vd are  lock\n");
                RetryCntNTSC =1;
                fw_timer_event_DelayXms(100);
                do
                {
//					ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
                    ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
                    RTD_Log(LOGGER_DEBUG,"the tuner set ntsc  the mode ... %x\n", (UINT32)ucVdc_TV_Mode);

                    AScan_Noise_stas = drv_video_vdc_NoiseStatus();
                    //leo 20090217 add for audio tone be identified as video
                    AScan_Noise_stas2 = AScan_Noise_stas;
                    RTD_Log(LOGGER_DEBUG,"the ntsc noise  ... %x\n", (UINT32)AScan_Noise_stas);
#ifdef TV_PAL_NTSC_TH

                    if ((ucVdc_TV_Mode==ZNTSC)&&(AScan_Noise_stas<=0xb6))
#else
                    if (ucVdc_TV_Mode==ZNTSC)
#endif
                    {
                        return ZNTSC;
                    }
#ifdef AUDIO_USE_DISABLE
                    if ((ucVdc_TV_Mode==ZNTSC_443)&&(AScan_Noise_stas<=0x27))
                    {
                        fw_timer_event_DelayXms(200);
                        color_detect=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG1_0x5008);
                        RTD_Log(LOGGER_DEBUG,"VDC_VIDEO_STATUS_REG1_VADDR=%x ??????........... \n",color_detect);
                        if (ntsc443_color==0)
                        {
                            return ZNTSC;
                        }
                        else
                        {
                            RTD_Log(LOGGER_DEBUG,"ntsc443 is ok ..\n");
                            return ZNTSC_443;
                        }
                    }
#endif
#ifdef AUDIO_USE_DISABLE
                    if ((ucVdc_TV_Mode==ZPAL_N)&&(AScan_Noise_stas<=0x15))
                    {
                        return ZPAL_N;
                    }
                    if ((ucVdc_TV_Mode==ZPAL_M)&&(AScan_Noise_stas<=0x15))
                    {
                        return ZPAL_M;
                    }
#else
                    if (ucVdc_TV_Mode==ZPAL_N)
                    {
                        Audio_AtvGetMNStatus(&mn_status, NULL);
                        if (mn_status == 1)
                        {
                            RTD_Log(LOGGER_DEBUG,"PAL N is ok ..\n");
                            return ZPAL_N;
                        }
                    }
                    /////////////////////////
                    if (ucVdc_TV_Mode==ZPAL_M)
                    {
                        Audio_AtvGetMNStatus(&mn_status, NULL);
                        RTD_Log(LOGGER_DEBUG,"the pal m audio  tone=%x ??????........... \n", (UINT32)mn_status);
                        if (mn_status == 1)
                        {
                            RTD_Log(LOGGER_DEBUG,"PAL M is ok ..\n");
                            return ZPAL_M;
                        }
                    }

                    //////////////////////////////
#endif
                }
                while (--RetryCntNTSC);
            }
        }
    }

    if (RetryCntPAL == 0 && RetryCntNTSC == 0&&lock==1) //for weak signal
    {
        /////////////////////////////////////////////////////////////////
        ///// For detect Secam L only
        drvif_ifd_set_iftuner_setting(ZSECAML,tvScanCurFreq,160);
        if (drvif_ifd_get_cr_lock()==_TRUE)
        {
            RTD_Log(LOGGER_DEBUG,"\n\n_SIG_MODE_SECAM ifd lock\n\n");
            fw_timer_event_DelayXms(500);
            if (drv_video_vdc_CheckHVisLocked())
            {
                RTD_Log(LOGGER_DEBUG,"\n\n_SIG_MODE_SECAM vd lock\n\n");
                ucVdc_TV_Mode = drv_video_vdc_IdentifyColorStandard();
                if (ucVdc_TV_Mode == ZSECAM)
                {
                    return ZSECAML;
                }
            }
        }

        ////////////////////////////////////////////////////////////////

        //leo 20090217 add for audio tone be identified as video
        AScan_Noise_stas = drv_video_vdc_NoiseStatus();
        RTD_Log(LOGGER_DEBUG,"the final noise  ... %x,%x,%x\n", (UINT32)AScan_Noise_stas, (UINT32)AScan_Noise_stas1, (UINT32)AScan_Noise_stas2);
        /* //irene move@090312
             if (AScan_Noise_stas == 0xff || AScan_Noise_stas1 == 0xff || AScan_Noise_stas2 == 0xff)
             	{
             		RTD_Log(LOGGER_DEBUG,"the final noise level is 0xff, so ignore  ..\n");
        	 	return 0;
             	}
        	 */
        hv_lock=1;
        flag625=(UINT8)rtdf_inl(VDC_VIDEO_STATUS_REG3_0x5010);
        flag625=flag625&0x04;
        if (flag625==0x04)
        {
            RTD_Log(LOGGER_DEBUG,"h v lock  is pal ..\n");
            hv_lock_mode=1;
            if (AScan_Noise_stas1 == 0xff) //irene@090312
            {
                RTD_Log(LOGGER_DEBUG,"the final noise level is 0xff, so ignore  ..\n");
                return 0;
            }
            //SET_CHANNEL_COLORSYSTEM(tvScanCurChn, ZPAL_I);
            //stUserDataChannelSettings[tvScanCurChn-1].colorSystem = ZPAL_I;
            return ZPAL_I;
        }
        else
        {
            RTD_Log(LOGGER_DEBUG,"h v lock  is ntsc ..\n");

            hv_lock_mode=0;
            if (AScan_Noise_stas2 == 0xff) //irene@090312
            {
                RTD_Log(LOGGER_DEBUG,"the final noise level is 0xff, so ignore  ..\n");
                return 0;
            }
            //SET_CHANNEL_COLORSYSTEM(tvScanCurChn, ZNTSC);
            //stUserDataChannelSettings[tvScanCurChn-1].colorSystem = ZNTSC;
            return ZNTSC;
        }
    }
    return 0;

}
#endif

void CTVSetVideoManualMode(UINT8 mode)
{
    switch (mode)
    {
    case ZNTSC:
        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000000);
        break;
    case ZNTSC_443:
        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000004);
        rtdf_outl(VDC_STATE_MACHINE_FIX_MODE_0x51d0,0x00000000);
        break;
    case ZPAL_N:
        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000003);
        rtdf_outl(VDC_STATE_MACHINE_FIX_MODE_0x51d0,0x00000000);
        break;
    case ZPAL_M:
        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000001);
        rtdf_outl(VDC_STATE_MACHINE_FIX_MODE_0x51d0,0x00000000);
        break;
    case ZPAL_I:
        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000006);
        rtdf_outl(VDC_STATE_MACHINE_FIX_MODE_0x51d0,0x00000000);
        break;
    case ZSECAM:
        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000007);
        rtdf_outl(VDC_STATE_MACHINE_FIX_MODE_0x51d0,0x00000000);
        break;
    case ZSECAML:
        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000007);
        rtdf_outl(VDC_STATE_MACHINE_FIX_MODE_0x51d0,0x00000000);
        break;
    case ZSECAMLA:
        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000007);
        rtdf_outl(VDC_STATE_MACHINE_FIX_MODE_0x51d0,0x00000000);
        break;
#ifdef CONFIG_SUPPORT_PAL60
    case ZPAL_60:
        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000005);
        rtdf_outl(VDC_STATE_MACHINE_FIX_MODE_0x51d0,0x00000000);
        break;
#endif
    default:
        break;
    }
}

void fw_tv_setvideomode(UINT8 video_mode, UINT32 freq)
{
    //drv_scalerdisplay_set_bg_color(0,0,0);
    //drv_scalerdisplay_ForceBackground(_TRUE);

    //  0 : ntsc
    //  1 : palm
    //  2 : ntsc50
    //  3 : paln
    //  4 : ntsc443
    //  5 : pal60
    //  6 : pali
    //  7 : secam
    //  8 : auto mode
    UINT32 tuner_freq;
	UINT8 CurChn;

	CurChn = GET_TV_CHN_DATA(_CUR_CHN_TV);
	
    if (freq != 0)
    {
        if (video_mode ==  ZSECAMLA)
        {
            tuner_freq= freq -SECAMLA_FREQ_SHIFT;
            g_afc_curr_freq = freq-SECAMLA_FREQ_SHIFT;
            g_afc_center_freq = freq-SECAMLA_FREQ_SHIFT;
        }
        else
        {
            tuner_freq = freq ;
            g_afc_curr_freq = freq;
            g_afc_center_freq = freq;
        }

        g_afc_stage_count = 0; //reset afc
        g_afc_hvunlock_debounce = AFC_HV_UNLOCK_DEBOUNCE_COUNT; //reset afc
    }
    g_curr_std = video_mode;

    switch (video_mode)
    {
    case ZNTSC:
        drvif_tuner_init(_SIG_MODE_NTSC);
        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000000);
        break;
    case ZNTSC_443:
        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000004);
        rtdf_outl(VDC_STATE_MACHINE_FIX_MODE_0x51d0,0x00000000);
        drvif_tuner_init(_SIG_MODE_PAL);
        break;
    case ZPAL_N:
        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000003);
        rtdf_outl(VDC_STATE_MACHINE_FIX_MODE_0x51d0,0x00000000);
        drvif_tuner_init(_SIG_MODE_NTSC);
        break;
    case ZPAL_M:
        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000001);
        rtdf_outl(VDC_STATE_MACHINE_FIX_MODE_0x51d0,0x00000000);
        drvif_tuner_init(_SIG_MODE_NTSC);
        break;
    case ZPAL_I:
        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000006);
        rtdf_outl(VDC_STATE_MACHINE_FIX_MODE_0x51d0,0x00000000);
        drvif_tuner_init(_SIG_MODE_PAL);
        break;
    case ZSECAM:
        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000007);
        rtdf_outl(VDC_STATE_MACHINE_FIX_MODE_0x51d0,0x00000000);
        drvif_tuner_init(_SIG_MODE_PAL);
        break;
    case ZSECAML:
        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000007);
        rtdf_outl(VDC_STATE_MACHINE_FIX_MODE_0x51d0,0x00000000);
        drvif_tuner_init(_SIG_MODE_SECAM_L);
        break;
    case ZSECAMLA:
        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000007);
        rtdf_outl(VDC_STATE_MACHINE_FIX_MODE_0x51d0,0x00000000);
        drvif_tuner_init(_SIG_MODE_SECAM_LA);
        break;
#ifdef CONFIG_SUPPORT_PAL60
    case ZPAL_60:
        rtdf_outl(VDC_MANUAL_MODE_0x51c0,0x00000005);
        rtdf_outl(VDC_STATE_MACHINE_FIX_MODE_0x51d0,0x00000000);
        drvif_tuner_init(_SIG_MODE_PAL);
        break;
#endif
    default:
        //drvif_tuner_init(_SIG_MODE_PAL);
        //drv_video_vdc_SetMode(8);
        break;
    }

#ifdef CONFIG_SUPPORT_PAL60
    if (std==ZPAL_60)
        rtdf_maskl(VDC_VSYNC_TIME_CONST_0x515c,~_BIT6,_BIT6);
    else
        rtdf_maskl(VDC_VSYNC_TIME_CONST_0x515c,~_BIT6,0x00);
#endif

    // charlie 080125 debug for change TV system, the color kill need to be set
    //if(stUserDataChannelSettings[stTvInfo.CurChn-1].colorSystem==ZSECAM)
    if (GET_CHANNEL_COLORSYSTEM(CurChn) == ZSECAM)
        rtdf_maskl(VDC_CHROMA_KILL_0x5094,~(_BIT7|_BIT6),_BIT7);
    else
        rtdf_maskl(VDC_CHROMA_KILL_0x5094,~(_BIT7|_BIT6),0x00);
    drv_video_vdc_YcSeparation();
}
extern UINT32 code tNTSC_CATV_FREQ[];
//******************************************************************************
//
// FUNCTION       : UINT8 TvSetChannel(UINT8 disp, UINT8 chn)
//
// USAGE          	: This routine will set the specific channel as current channel.
//
// INPUT         	 	:	< ch > { current channel	}
//
// OUTPUT         	:	Return the status code  _TURE,_FALSE
//
// GLOBALS        	:
//
// USED_REGS      :
//
//******************************************************************************
UINT8 TvSetChannel(UINT8 chn)
{
#if 1
	//#ifndef _ONLY_NTSC_SYSTEM
    StructUserDataChannelSettingsType stNormalChannelData;    
	//#else
    StructTvCableDataType           stChannelData;    
    StructTvAirDataType             stChannelData_Air;
	//#endif
#ifdef CONFIG_IFD_ENABLE
    //static UINT8 first = 1;
    //UINT8 PreBand,CurBand;
#endif
    UINT8 colorSystem;//,colortype;
    drv_video_vdc_AVoutEnable(VDC_AVOUT_DISABLE);


#ifdef CONFIG_IFD_ENABLE
    drvif_ifd_set_agc_variance_off(1);
    drvif_ifd_agc_peak_en(0);
#endif
    //ui_get_chn_data(chn, &stChannelData);
    if(COsdCtrlGetNtscFree())//Table
    {
        if(GET_TV_CUR_NtscType())
        ui_get_Air_chn_data(chn, &stChannelData_Air);
        else
        ui_get_Cable_chn_data(chn, &stChannelData);
    }
    else
	ui_get_chn_data(ui_get_channel_idx(chn), &stNormalChannelData);


    if(COsdCtrlGetNtscFree())//Table
    {
        if(GET_TV_CUR_NtscType())
        {
            colorSystem = stChannelData_Air.colorSystem;
            RTD_Log(LOGGER_INFO,"\n FREQ=%d, Air_colorSystem = %d \n",(UINT32)stChannelData_Air.freq,(UINT32)colorSystem);
        
        }else
        {
            colorSystem = stChannelData.colorSystem;
            RTD_Log(LOGGER_INFO,"\n Cable_colorSystem = %d \n",(UINT32)stChannelData.freq);
        
        }

    }
    else
    colorSystem = stNormalChannelData.colorSystem;
    //RTD_Log(LOGGER_LEVEL,"\n Tim colorSystem=%d \n",(UINT32)colorSystem);

    flow_audio_Setmute(_ENABLE, SCALER, (ADC_CH_IDX)(ADC_CH_L|ADC_CH_R));

#if 1 //defined(CONFIG_FINE_TUNE_SEPARATED_FROM_FREQ)
//-->w  tvScanCurFreq = GET_CHANNEL_FREQ(chn);
//-->w  tvScanCurFreq += ((GET_CHANNEL_FREQ_OFFSET(chn) - _TV_FineTuneBar_ZeroPoint) * drv_tuner_GetRatioInStepSize()); // charlie, 20090122, for TV AFC skip

	if(COsdCtrlGetNtscFree())//Table
	{
        if(GET_TV_CUR_NtscType())
        {
            tvScanCurFreq = stChannelData_Air.freq;
            tvScanCurFreq += ((stChannelData_Air.offset - _TV_FineTuneBar_ZeroPoint) * drv_tuner_GetRatioInStepSize()); // charlie, 20090122, for TV AFC skip
        }
        else{
            tvScanCurFreq = stChannelData.freq;
            tvScanCurFreq += ((stChannelData.offset - _TV_FineTuneBar_ZeroPoint) * drv_tuner_GetRatioInStepSize()); // charlie, 20090122, for TV AFC skip
        }
	}
	else
	{
		tvScanCurFreq = stNormalChannelData.freq;
		tvScanCurFreq += ((stNormalChannelData.offset - _TV_FineTuneBar_ZeroPoint) * drv_tuner_GetRatioInStepSize()); // charlie, 20090122, for TV AFC skip
	}


#else
    tvScanCurFreq = GET_CHANNEL_FREQ(chn);
#endif
    // video setting
    //fw_tv_setvideomode(colorSystem, tvScanCurFreq);//Edit SVN53
    //disp_info.state = _MODE_STATE_SEARCH;
    //drv_scalerdisplay_set_bg_color(0,0,0);
    //drv_scalerdisplay_ForceBackground(_TRUE);
    //flow_mode_DisableChannelProc();
    //COsdFxDisableOsd(); // Prevent OSD flick


    /*if (colorSystem==ZNTSC)
        colortype= _SIG_MODE_NTSC;
    else if ((colorSystem==ZPAL_I)||(colorSystem==ZPAL_60)||(colorSystem==ZPAL_M)||(colorSystem==ZPAL_N))
        colortype= _SIG_MODE_PAL;
    else if ((colorSystem==ZSECAM)||(colorSystem==ZSECAML)||(colorSystem==ZSECAMLA))
        colortype = _SIG_MODE_SECAM;
    else if (colorSystem==ZNTSC_443)
        colortype = _SIG_MODE_NTSC_443;
    else
        colortype = _SIG_MODE_AUTO;
        
    drvif_ifd_init();
    drvif_tuner_init(colortype);*/

    flow_audio_Setmute(ADC_MUTE, SCALER, (ADC_CH_IDX)(ADC_CH_L|ADC_CH_R));
    fw_tv_setvideomode(colorSystem, tvScanCurFreq);

    usAfcCurFreq = tvScanCurFreq;
    if (colorSystem ==  ZSECAMLA)
    {
		#ifdef CONFIG_TUNER_SILICON
        drv_tuner_initRT810(_SIG_MODE_PAL);
		#endif
        drv_tuner_SetFreq(tvScanCurFreq -SECAMLA_FREQ_SHIFT);
    }
    else
    {
		#ifdef CONFIG_TUNER_SILICON
        if ((colorSystem ==  ZNTSC)||(colorSystem ==  ZPAL_M)||(colorSystem ==  ZPAL_N))
            drv_tuner_initRT810(_SIG_MODE_NTSC);
        else
            drv_tuner_initRT810(_SIG_MODE_PAL);
		#endif

        drv_tuner_SetFreq(tvScanCurFreq);
    }

//-->w  RTD_Log(LOGGER_DEBUG, "Tuner Freq ... %x\n", (UINT32)tvScanCurFreq);

#ifdef CONFIG_IFD_ENABLE
    drvif_ifd_cr_freq_status_check(0);
    g_next_time_dont_check_variance = _TRUE;//->w not existed   next_time_dont_check_variance = _TRUE;
    drvif_ifd_set_agc_variance_on();
    fw_timer_event_DelayXms(150); // chihwei modify for "default TV is un-lock"
    fw_timer_event_DelayXms(100);//Band

#endif
	SET_TV_CHN_DATA(_CUR_CHN_TV, chn);
	
    //RTD_Log(LOGGER_DEBUG,"Tim111 Pre air=%d\n", (UINT32)GET_TV_PRE_AIR_CHN());
    //RTD_Log(LOGGER_DEBUG,"Tim222 Cur cable Chn %d,air=%d\n", (UINT32)GET_TV_CUR_CABLE_CHN(),(UINT32)GET_TV_CUR_AIR_CHN());
    rtdf_maskl(VDC_VSYNC_TIME_CONST_0x515c,0xfffffffC,0x02);
#ifndef CONFIG_IFD_ENABLE
    rtd_outl(VDC_VSYNC_TIME_CONST2_0x5160, 0x04);
    rtd_outl(VDC_VSYNC_TIME_CONST1_0x5164, 0x20);
#endif

    //audio setting
#ifdef CONFIG_ENABLE_AUDIOTV_HW_ASD
    Audio_AtvSetSoundStd(ATV_SOUND_STD_AUTO);
#else
	//RTD_Log(LOGGER_LEVEL,"\n Tim soundset = %d \n",(UINT32)stChannelData_Air.soundSystem);
	if(COsdCtrlGetNtscFree())//Table
	{
        if(GET_TV_CUR_NtscType())
            TVSetAudioStandard(stChannelData_Air.soundSystem);
        else
            TVSetAudioStandard(stChannelData.soundSystem);
	}
	else
    TVSetAudioStandard(stNormalChannelData.soundSystem);
#endif
    RTD_Log(LOGGER_LEVEL,"\n tim stNormalChannelData.soundSystem=%d \n",(UINT32)stNormalChannelData.soundSystem);

	//RTD_Log(LOGGER_LEVEL,"\n tim chn=%d,cable chn = %d,air chn=%d,colorSystem=%d \n",(UINT32)GET_TV_CUR_CHN(),(UINT32)GET_TV_CUR_CABLE_CHN(),(UINT32)GET_TV_CUR_AIR_CHN(),(UINT32)colorSystem);
    SET_CHANNEL_COLORSYSTEM(GET_TV_CHN_DATA(_CUR_CHN_TV), colorSystem);
    
    flow_audio_Setmute(ADC_MUTE, SCALER, (ADC_CH_IDX)(ADC_CH_L|ADC_CH_R));

    //set to tuner for test
    //RTD_Log(LOGGER_DEBUG,"colorSystem = %x soundSystem = %x in Tv\n", (UINT32)colorSystem, (UINT32)stChannelData.soundSystem);

    // kevin added 080312
    if ((colorSystem==ZSECAM) && CCheckTVChannelLock())
        rtd_outl(VDC_CHROMA_KILL_0x5094,0x83);
    else
        rtd_outl(VDC_CHROMA_KILL_0x5094,0x07);

    drv_video_vdc_AVoutEnable(VDC_AVOUT_ENALBE);
#endif
    return _TRUE;
}


UINT8 CTvScanGetAudioStd(UINT8 vd_std)
{
    UINT8 mn_status;
    UINT8 ucTemp;
    switch (vd_std)
    {
    case ZPAL_N:
        drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_MN);
        //fw_timer_event_DelayXms(500); // chihwei modify for "default TV is un-lock"
        Audio_AtvGetMNStatus(&mn_status, NULL);
        RTD_Log(LOGGER_DEBUG,"mn_status=%x\n", (UINT32)mn_status);
        if (mn_status)
        {
            ucTemp = ATV_SOUND_STD_PAL_N;
        }
        else
        {
            drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_BG);
            Audio_AtvScanSoundStd(&ucTemp,ATV_SCAN_OPT_HW_AUTO);
        }
        RTD_Log(LOGGER_DEBUG, "[auto scan]soundStd = %x\n", (UINT32)ucTemp);
        break;

    case ZNTSC :
    case ZPAL_M:
        drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_MN);
        //fw_timer_event_DelayXms(500); // chihwei modify for "default TV is un-lock"
        Audio_AtvGetMNStatus(&mn_status, NULL);
        RTD_Log(LOGGER_DEBUG,"mn_status=%x\n", (UINT32)mn_status);
        if (mn_status)
        {
            ucTemp = ATV_SOUND_STD_MN_MONO;
        }
        else
        {
            drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_BG);
            Audio_AtvScanSoundStd(&ucTemp,ATV_SCAN_OPT_HW_AUTO);
        }
        RTD_Log(LOGGER_DEBUG, "[auto scan]soundStd = %x\n", (UINT32)ucTemp);
        break;
    case ZSECAML:
    case ZSECAMLA:
        Audio_AtvSetMtsPriority(ATV_MTS_PRIO_L);
        drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_L);
        Audio_AtvScanSoundStd(&ucTemp,ATV_SCAN_OPT_HW_AUTO);
        RTD_Log(LOGGER_DEBUG, "[auto scan]soundStd = %x\n", (UINT32)ucTemp);
        break;
    default:
        Audio_AtvSetMtsPriority(ATV_MTS_PRIO_DK);
        drv_tuner_TunerAudioStdInit(_SIG_MODE_AUDIO_BG);
        Audio_AtvScanSoundStd(&ucTemp,ATV_SCAN_OPT_HW_AUTO);
        RTD_Log(LOGGER_DEBUG, "[auto scan]soundStd = %x\n", (UINT32)ucTemp);
        break;
    }
    return ucTemp;
}


UINT8 CTvSetChannel(UINT8 chn)
{
    UINT8 ucColorStandard;
    UINT8 CurColorSys, PrevColorSys;

    RTD_Log(LOGGER_INFO,"CTvSetChannel(%d)\n", (UINT32)chn);
    //RTD_Log(LOGGER_DEBUG,"Tim000 Cur cable Chn %d,air=%d\n", (UINT32)GET_TV_CUR_CABLE_CHN(),(UINT32)GET_TV_CUR_AIR_CHN());

    drv_video_reset_HPLL_default();
    if ((disp_info.state != _MODE_STATE_SEARCH)/*&&(CAudioIsFinishPlayWav())*/)
    {
        Audio_HwpSetMute(_TRUE,ADC_CH_ALL, ADC_SYS_MUTE);

        SET_TV_CHN_DATA(_PRE_CHN_TV,GET_TV_CHN_DATA(_CUR_CHN_TV));
        SET_TV_CHN_DATA(_CUR_CHN_TV,chn);
        //RTD_Log(LOGGER_DEBUG,"Tim111 Cur cable Chn %d,air=%d\n", (UINT32)GET_TV_CUR_CABLE_CHN(),(UINT32)GET_TV_CUR_AIR_CHN());
    }

    drv_vdc_ResetReg();


//	RTD_Log(LOGGER_INFO,"2.Get cur = %x, Get prev = %x\n", (UINT32)GET_TV_CUR_CHN(), (UINT32)GET_TV_PRE_CHN());
	
    PrevColorSys = GET_CHANNEL_COLORSYSTEM(GET_TV_CHN_DATA(_PRE_CHN_TV));
    CurColorSys = GET_CHANNEL_COLORSYSTEM(GET_TV_CHN_DATA(_CUR_CHN_TV));

    RTD_Log(LOGGER_INFO,"CurColorSys(%x),PrevColorSys(%x) \n", (UINT32)CurColorSys,(UINT32)PrevColorSys);
	
    if ((PrevColorSys != CurColorSys) || (CurColorSys == ZAUTO))
    {
        if ((info->state == _MODE_STATE_ACTIVE)&&(tvAutoScanFlag == _FALSE))
            drv_scalerdisplay_set_bg_color(0,0,0);
        drv_scalerdisplay_ForceBackground(_ENABLE);
        if ( PrevColorSys == ZAUTO)
        {
            drv_video_vdc_SetMode(8);
#ifndef CONFIG_TUNER_SILICON
            ucColorStandard = drv_video_GetTvColorStandard();
#else
            ucColorStandard = ucVdc_Curr_Mode;
#endif
            if (ucColorStandard == 0)
                ucColorStandard = ucVdc_TV_Mode;
        }
        TvSetChannel(chn);	//set channel

#ifdef CONFIG_IFD_ENABLE //auto mode
        if ( CurColorSys == ZAUTO)
        {
            fw_timer_event_DelayXms(400);
            drv_video_vdc_SetMode(8);
#if 1//ndef CONFIG_TUNER_SILICON				
            ucColorStandard = drv_video_GetTvColorStandard();
#else
            ucColorStandard = ucVdc_Curr_Mode;
#endif
            //RTD_Log(LOGGER_INFO,"ucColorStandard(%d)\n", (UINT32)ucColorStandard);
            if ((ucColorStandard == PrevColorSys)||(ucColorStandard == 0))
            {
                goto SetTVProcess;
            }
            else
            {
                CurColorSys = ucColorStandard;
            }

        }
        else if ( PrevColorSys == ZAUTO)
        {
            if (ucColorStandard == CurColorSys)
            {
                goto SetTVProcess;
            }
        }
#endif

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        drv_sig_tv_ReSetVGIP();
        fw_timer_event_DelayXms(200);
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        if ((CurColorSys == ZNTSC) || (CurColorSys == ZNTSC_443) || (CurColorSys == ZPAL_M))
        {
            drv_sig_tv_SetVGIP(_MODE_480I);
            RTD_Log(LOGGER_DEBUG,"[TV]Set VGIP = 480i, colorSystem=%x\n", CurColorSys);
        }
        else // if((CurColorSys == ZPAL_I) || (CurColorSys == ZNTSC_50) || (CurColorSys == ZPAL_N) || (CurColorSys == ZSECAM))
        {//end yy1.8 	//TV mode=PAL
            drv_sig_tv_SetVGIP(_MODE_576I);
            RTD_Log(LOGGER_DEBUG,"[TV]Set VGIP = 576i, colorSystem=%x\n", CurColorSys);
        }//yy 1.8 end
        fw_flow_message_clearall();//prevent do check mode at the same time
        fw_flow_message_push(_MODE_MSG_MAIN_SET_VGIP_AFTER_FOUND_MODE_SIGNAL);
    }
    else
    {
        RTD_Log(LOGGER_DEBUG,"@@@@color system no change\n");
        if ((info->state == _MODE_STATE_ACTIVE)&&(tvAutoScanFlag == _FALSE))
            drv_scalerdisplay_set_bg_color(0,0,0);
        drv_scalerdisplay_ForceBackground(_ENABLE);
        TvSetChannel(chn);//set channel
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
SetTVProcess:
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        if ((info->state == _MODE_STATE_ACTIVE)&&(tvAutoScanFlag == _FALSE))
        {
            drv_scalerdisplay_ForceBackground(_FALSE);
            drv_scalerdisplay_set_bg_color(0,0,0xff);
        }
        if ( (disp_info.state != _MODE_STATE_SEARCH))
        {
            //RTD_Log(LOGGER_INFO,"GET_AUDIO_MUTE() = %x\n",(UINT32)GET_AUDIO_MUTE());
            if (!GET_AUDIO_MUTE())
            {
                //-----------------
                // modify by linmc,
                // 2009.03.13
                //-----------------
#if defined(_TEST_0313)
                ModeSate_ModeStateMuteControl(_FALSE);
#else
                if (drv_video_vdc_CheckHVisLocked())
                {
                    flow_audio_Setmute(ADC_UNMUTE, SCALER, (ADC_CH_IDX)(ADC_CH_L|ADC_CH_R));
					//flow_display_Framesync_Mode();//Tim 0426
					//rtdf_maskl(DISPLAY_TIMING_CTRL1_0x2000, ~(_BIT26 | _BIT29), _BIT26);
                }//else
                	//rtdf_maskl(DISPLAY_TIMING_CTRL1_0x2000, ~(_BIT26 | _BIT29), 0);
                	//flow_display_FreeRun_Mode();
	                //flow_mode_DisableChannelProc();//Tim 0426
	           //rtdf_maskl(DISPLAY_TIMING_CTRL1_0x2000, ~(_BIT26), _BIT26);
#endif
            }
            // TODO:
            /*
             * MTS policy should be defined by customer.
             * In this version, we force MTS to STEREO if it is possible.
             */
#if defined(CONFIG_AUDIO_ATV_SUPPORT_NICAM)||defined(CONFIG_AUDIO_ATV_SUPPORT_BTSC)
            Audio_AtvSetSoundSelect(ATV_SOUND_SEL_STEREO_A);
#else
            Audio_AtvSetSoundSelect(ATV_SOUND_SEL_FMAM);
#endif
        }
        drvif_module_vdc_TV_FHFV_Bias_DCLK_Adjust();
    }

#ifdef CONFIG_DCC_ENABLE
    drvif_color_dcc_debug_mode(0); // add by Jerry Wu 20080820
#endif

#ifdef CONFIG_ENABLE_NTSC_No_Burst_Check		//lzf_dbg080518
    drvif_module_vdc_NTSCnoBurstCheck();
#endif

    //if (!GET_COLOR_SYSTEM_CHANGE())
    //{
    //	drv_scalerdisplay_ForceBackground(_DISABLE);
    //}
    return _TRUE;
}

//******************************************************************************
//
// FUNCTION       : UINT8 CTvSelectChannel(UINT8 display, UINT8 operation, UINT8 Osd)
//
// USAGE          	: This routine will change channel to next/prev channel.
//
// INPUT         	 	:	< operation > {	_TV_PREV_CHANNEL, change to previous channel
//														_TV_NEXT_CHANNEL,change to next channel
//														_TV_LAST_CHANNEL,jump to last watched channel
//
// OUTPUT         	:	Return the status code  _TURE,_FALSE
//
// GLOBALS        	:
//
// USED_REGS      :
//
//******************************************************************************
UINT8 CTvSelectChannel(UINT8 operation)
{
    UINT8 chn;
    UINT8 check;
    UINT8 MaxChn;

	if(COsdCtrlGetNtscFree())//Table
	{
        if(GET_TV_CUR_NtscType() == 0)//Cable
        {
            chn = GET_TV_CHN_DATA(_CUR_CHN_TV);//GET_TV_CUR_CABLE_CHN();
            MaxChn = 125;
        }
        else
        {
            chn = GET_TV_CHN_DATA(_CUR_CHN_TV);//GET_TV_CUR_AIR_CHN();
            MaxChn = 69;
        }

	}
	else
	{
	    chn = GET_TV_CHN_DATA(_CUR_CHN_TV);//GET_TV_CUR_CHN();
	    MaxChn = _MAX_CATV_NUM;
	}

	RTD_Log(LOGGER_LEVEL,"\n MaxChn = %d,GET_TV_CUR_CABLE_CHN()=%d,GET_TV_PRE_AIR_CHN() = %d \n",(UINT32)MaxChn,(UINT32)GET_TV_CUR_CABLE_CHN(),(UINT32)GET_TV_PRE_AIR_CHN());
    switch (operation)
    {
    case _TV_PREV_CHANNEL:
        //previous channel
        check = 0;
        do
        {
            if ( (2 == chn && GET_TV_CUR_NtscType() && COsdCtrlGetNtscFree()) || (1  == chn && !GET_TV_CUR_NtscType()&& COsdCtrlGetNtscFree()) || (!COsdCtrlGetNtscFree() && ( 1	== chn )))
			//if ( 1	== chn )
                chn = MaxChn;
            else
                chn--;
	                
            check++;
            //}while(check < _MAX_CATV_NUM && (CTvGetChStatus(chn, CHANNEL_SKIP)  == _ENABLE));
        }
        while (check < MaxChn && GET_CHANNEL_SKIP(chn));
        break;
    case _TV_NEXT_CHANNEL:
        check = 0;
        do
        {
            if ( MaxChn == chn )//_MAX_CATV_NUM
            {
            
	            if(COsdCtrlGetNtscFree())//Table
	            {
                    if(MaxChn == 125)
                        chn = 1;
                    else
                        chn = 2;
	            }
	            else
            	chn = 1;
            }else
                chn++;
            check++;
            //}while(check < _MAX_CATV_NUM && (CTvGetChStatus(chn, CHANNEL_SKIP)  == _ENABLE));
        }
        while (check < MaxChn && GET_CHANNEL_SKIP(chn));
        break;
    case _TV_LAST_CHANNEL:
        //chn = GET_TV_PRE_CHN();//Tim 0404
        break;

	//Tim 0404
//==============================================================================
    case _TV_RETURN_CHANNEL:
		chn = GET_TV_CHN_DATA(_PRE_CHN_TV);
		
    	break;
//==============================================================================
        //=====================//
        //Ch Down by 1
        //=====================//
    case _TV_PREV_ONE_CHANNEL:
        //////////////////////////////////////
        // Check if ATV source is doing cycle
        //////////////////////////////////////
        if (chn == 1)
            chn = _MAX_CATV_NUM;
        else
            chn--;
        if (chn > _MAX_CATV_NUM)
            chn = _MAX_CATV_NUM;
        break;
        //=====================//
        //Ch Up by 1
        //=====================//
    case _TV_NEXT_ONE_CHANNEL:
        //////////////////////////////////////
        // Check if ATV source is doing cycle
        //////////////////////////////////////
        chn++;
        if (chn > _MAX_CATV_NUM)
            chn = 1;
        break;
    }

    //------------
    // modify by linmc
    // to select channel just get next channel number
    // 2009.01.10
    //------------
#if 1
	RTD_Log(LOGGER_LEVEL,"\n Chn = %d \n",(UINT32)chn);

    return chn;
#else
    CTvSetChannel(chn);

    return _TRUE;
#endif
}

UINT8 CCheckTVChannelLock()
{
    if (drvif_ifd_get_cr_lock()==_TRUE)
    {
        if (drv_video_vdc_CheckHVisLocked())
        {
//			DebugPrintf("\n VDC is Locked !",0);
            return  _TRUE;
        }
        else
        {
//			DebugPrintf("\n VDC is unLocked !",0);
            return _FALSE;
        }
    }
    return _FALSE;
}

//******************************************************************************
//
// FUNCTION       : void CTvResetUserDataChannelSettings(void)
//
// USAGE          	: Reset Channel Data
//
// INPUT         	 	: { None }
//
// OUTPUT         	: { None }
//
// GLOBALS        	:
//
// USED_REGS      :
//
//******************************************************************************
void CTvResetUserDataChannelSettings(UINT8 ucStartCh)
{
    UINT8	Size,i;
    //
    ////////////////////////////////////////
    //Get Structure Size
    ///////////////////////////////////////
    Size = sizeof (StructUserDataChannelSettingsType);

    for (i=ucStartCh; i<_MAX_PAL_CATV_NUM; i++)
    {
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
		#if defined(_REALTEK_OSD_USE)
        ui_set_chn_data(i, &tDEFAULT_OSDUSER_CHANNELSETTING);
        #endif
#else
        //SYNC_STORAGE_ARRAY_STRUCT_DATA(StructUserDataChannelSettingsType, 0, stTVScanData);
#endif
        //memcpy((UINT8 *) & stCableChannelSettings[i], (UINT8 *) &tDEFAULT_OSDUSER_CHANNELSETTING, Size);
        // charlie, 20090211, Fine Tune Range: эΘ -0.7~2.0M
        //stChannelFreqOffsetSettings[i] = 24;
        //stChannelFreqOffsetSettings[i] = 14;
    }
}

#if 0
// charlie, 20090210, In TV Auto Scan , 苯, 狦Τ穝计, Τ计
UINT8 GetAutoScanStopChn(void)
{
    UINT8 tmp, MaxChn;
    UINT8 idx;
    UINT16 freq;

    MaxChn = CTVGetMAXChannelNum();

    for (tmp=1; tmp<=MaxChn; tmp++)
    {
        idx = ui_get_channel_idx(tmp);
        LOAD_STORAGE_ARRAY_DATA(StructUserDataChannelSettingsType, idx, freq, freq);
        if (freq == _EMPTY_FREQ)
            break;

//		if(GET_CHANNEL_FREQ(tmp)==_EMPTY_FREQ)
//			break;
    }

    return (tmp-1);
}
#endif

#if 0
// charlie, 20090210, In TV Auto Scan , 苯, 狦Τ穝计, Τ计
bool ChkStoreFreg(INT32 ufreq)
{

    UINT8 TmpFreq2, TmpFreq1, ich;
    UINT8 uFreq2, uFreq1;
    UINT8 idx;
    UINT16 freq;


    TmpFreq2 = (UINT8)(ufreq / 1000000);
    TmpFreq1 = (UINT8)(ufreq / 100000);

    for (ich=1; ich<=tvScanStopChn; ich++)
    {
        idx = ui_get_channel_idx(ich);
        LOAD_STORAGE_ARRAY_DATA(StructUserDataChannelSettingsType, idx, freq, freq);

        uFreq2 = (UINT8)(freq/1000000);
        if ( uFreq2 == TmpFreq2)
        {
            uFreq1 = (UINT8)(freq/100000);
            if (ABS(uFreq1,TmpFreq1)<2)
            {
                if ( ABS(freq,ufreq)<50000 ) // same
                    return _FALSE; // same
            }
        }
    }
    return _TRUE; // not same
}
#endif

//******************************************************************************
//
// FUNCTION       : void CTvAutoScanStart(void)
//
// USAGE          	: Auto scan start
//
// INPUT         	 	:	{ None	}
//
// OUTPUT         	:	{ None	}
//
// GLOBALS        	:
//
// USED_REGS      :
//
//******************************************************************************
#if defined(CONFIG_STORAGE_ACCESS_EEPROM)
UINT8 stInitialNum;
#endif
void CTvAutoScanStart(void)
{
    UINT8 CurChn;

	UINT8 NtscType;
#ifdef CONFIG_STORAGE_ACCESS_EEPROM
//	UINT8 i;
    UINT8 val;
    StructUserDataChannelSettingsType  xdata tDEFAULT_OSDUSER_CHANNELSETTING1;
#endif

#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    UINT8 i;
    StructUserDataChannelSettingsType  xdata tDEFAULT_OSDUSER_CHANNELSETTING1 =
    {
        440000,		// freq
        0,			// channel number
        0x02,		// ColorSystem, default = PAL_I
        0x00,		// SoundSystem, default = DK
        2,			// Status
        14,			// offset
#ifdef CONFIG_STORAGE_ACCESS_FLASH	// marked by weihao because of china
        {0x0D, 0x0D, 0x0D, 0x0D, 0x0D},	// name[5]
#endif
    };
#endif
#ifdef CONFIG_DCC_ENABLE
    drvif_color_dcc_debug_mode(1); // add by Jerry Wu 20080820
#endif
    drv_video_reset_HPLL_default();

    CTvResetUserDataChannelSettings(0);
#if defined(CONFIG_CHANNEL_ATS)
    drv_CNI_Init();
#endif
    RTD_Log(LOGGER_DEBUG,"....................CTvAutoScanStart\n");

    cvideo_tv_channel_scan_is_running = _ON;
    tvAutoScanFlag = _TRUE;
    //scan start from channel 1
    
	#ifdef CONFIG_TUNER_SILICON
    tvScanCurChn = 0;//Tim 0328  //1
    #else    
    tvScanCurChn = 0;//Tim 0328  //1
	#endif
	
    #ifndef CONFIG_TUNER_SILICON
    tvTableScanFirstChn = 2;
	#else
	tvTableScanFirstChn = 1;
	#endif
	if ( GET_TV_TABLE_SCAN()) {	
	    NtscType = GET_TV_CUR_NtscType();
        if(NtscType == 0)
			ucTableProgress = 1;
		else
            ucTableProgress = 2;
		
#ifdef CONFIG_CHANNEL_TABLE_SHIFT_SCAN
		ucTableProgress_freqshift=0;
#endif
	}

	if ( GET_TV_TABLE_SCAN()) {	
	    NtscType = GET_TV_CUR_NtscType();
        if(NtscType == 0)//Cable
    	{
            tvScanCurFreq = tNTSC_CATV_FREQ[1];
    	}
    	else
    	{
            tvScanCurFreq = tNTSC_AIR_FREQ[2];

    	}

	}
	else
	    tvScanCurFreq = drv_tuner_GetChanelMinFreq();

    RTD_Log(LOGGER_DEBUG,"\n The First Freq = %d \n",(UINT32)tvScanCurFreq);
#ifdef CONFIG_IFD_ENABLE  //irene for pga bug
    drvif_ifd_cr_freq_status_check(0);
    drvif_ifd_set_agc_variance_off(0xf);
    drvif_ifd_agc_peak_en(1);
#endif

#ifdef CONFIG_ENABLE_TV_NOISE_BOX_ENALBE
    auto_scan_avout_for_noise_box_enable=1;
    //module_vdc_AVoutMode(VDC_AVOUT_TV_NOISE_BOX);
    drv_video_vdc_AvOutMode(VDC_AVOUT1, VDC_AVOUT_CVBS_NO_FIL_NO_AGC)
#endif
//	 ModeSate_ShortTimeMuteAutoUnmute(_TRUE);	 // the mode stat will detect scan flag to mute
    //$$AUDIO_AMP_MUTE_ENABLE(AUDIO_AMP_MUTE);

    //avoid audio noise
#if defined(CONFIG_STORAGE_ACCESS_EEPROM)
//	for (i = 0; i < _MAX_PAL_CATV_NUM;i++) {
//		flow_timer_Reset_WatchDog();
    val = 0x02;
    stInitialNum = _MAX_PAL_CATV_NUM;
//		SYNC_STORAGE_ARRAY_STRUCT_DATA(StructUserDataChannelSettingsType, i, tDEFAULT_OSDUSER_CHANNELSETTING1);
    LOAD_STORAGE_ARRAY_STRUCT_DEFAULT_DATA(StructUserDataChannelSettingsType, 0, tDEFAULT_OSDUSER_CHANNELSETTING1);
    SYNC_STORAGE_ARRAY_STRUCT_DATA(StructUserDataChannelSettingsType, 0, tDEFAULT_OSDUSER_CHANNELSETTING1);
    SYNC_STORAGE_ARRAY_STRUCT_DATA(StructUserDataChannelSettingsType, stInitialNum - 1, tDEFAULT_OSDUSER_CHANNELSETTING1);
    stInitialNum--;
//		SYNC_STORAGE_ARRAY_STRUCT_DATA(StructUserDataChannelSettingsType, _MAX_PAL_CATV_NUM - 1, tDEFAULT_OSDUSER_CHANNELSETTING1);
//	}
#else
    fw_timer_event_DelayXms(150);
#endif


    Audio_AtvSetSoundStd(ATV_SOUND_STD_AUTO);

    drv_video_vdc_BlueScreenEnable(_DISABLE);

	CurChn = GET_TV_CHN_DATA(_CUR_CHN_TV);
	colortype = GET_CHANNEL_COLORSYSTEM(CurChn);//COsdCtrlGetTVVideoStd();
	//colortype = CConverColorModeProc(colortype);
	RTD_Log(LOGGER_LEVEL,"\n Tim colortype=%d \n",(UINT32)colortype);

	CurChn = GET_TV_CHN_DATA(_CUR_CHN_TV);
	soundtype = GET_CHANNEL_SOUNDSYSTEM(CurChn);

	//soundtype = COsdCtrlGetSoundSystem();
	//soundtype = CConverModeProc(soundtype);
	RTD_Log(LOGGER_LEVEL,"\n Tim soundtype=%d \n",(UINT32)soundtype);


#ifdef CONFIG_STORAGE_ACCESS_FLASH
#if !defined(_DISABLE_RT_STORAGE_FOR_TEST)
    // Set default to channel data

	if(COsdCtrlGetNtscFree())//Table
	{
        if(GET_TV_CUR_NtscType() == 0)//Cable
        {
            SYNC_STORAGE_DEFAULT_DATA(StructTvCableDataType);
        }
        else
        SYNC_STORAGE_DEFAULT_DATA(StructTvAirDataType);

	}
	else
    SYNC_STORAGE_DEFAULT_DATA(StructUserDataChannelSettingsType); 
    SYNC_STORAGE_DEFAULT_DATA(StructTVChannelMapTable);
#endif
#endif
    rtdf_maskl(VDC_VD_BASIC_REGISTER_ENABLE_0x51d4, 0xdf, 0x00);  //reset
    rtdf_maskl(VDC_VIDEO_CONTROL0_0x5000, 0xf0, 0x00);   //reset color mode

    g_curr_std=0x10;


    //fw_flow_message_push(_UI2FL_TV_AUTOSCAN_START);
}


/**
 *
 * @return none.
 */
void CTvAutoScanStop(void)
{
#ifdef CONFIG_STORAGE_ACCESS_EEPROM
    int i;
    StructUserDataChannelSettingsType tDEFAULT_OSDUSER_CHANNELSETTING1;
    LOAD_STORAGE_ARRAY_STRUCT_DEFAULT_DATA(StructUserDataChannelSettingsType, 0, tDEFAULT_OSDUSER_CHANNELSETTING1);
    for (i = tvScanCurChn; i <= stInitialNum; i++)
        SYNC_STORAGE_ARRAY_STRUCT_DATA(StructUserDataChannelSettingsType, i - 1, tDEFAULT_OSDUSER_CHANNELSETTING1);
    flow_timer_Reset_WatchDog();
#endif

    cvideo_tv_channel_scan_is_running = _OFF;

    //fw_flow_message_push(_UI2FL_TV_AUTOSCAN_CANCEL);	// inform flow to cancel channel scan

    tvScanCurFreq = GET_CHANNEL_FREQ(tvTableScanFirstChn);//for draw OSD
    //RTD_Log(LOGGER_DEBUG,"\n Tim tvScanCurFreq=%d \n",(UINT32)tvScanCurFreq);

    RTD_Log(LOGGER_DEBUG,"CTvAutoScanStop..............\n");
    //SET_TV_CUR_CHN(1);
    drv_scalerdisplay_set_bg_color(0,0,0);
    drv_scalerdisplay_ForceBackground(_ENABLE);
    flow_storage_WriteBackup2Flash();
    tvAutoScanFlag = _FALSE;
	//RTD_Log(LOGGER_LEVEL,"\n tvTableScanFirstChn=%d \n",(UINT32)tvTableScanFirstChn);
    TvSetChannel(tvTableScanFirstChn);
    drv_video_vdc_YcSeparation();
    /*SET_CHANNEL_COLORSYSTEM(tvTableScanFirstChn, ZPAL_M);
    //RTD_Log(LOGGER_LEVEL,"\n Tim bg103 \n");
    drv_scalerdisplay_set_bg_color(0,0,0);
    drv_scalerdisplay_ForceBackground(_TRUE);
    fw_tv_setvideomode(ZPAL_M,0);*/

#if 0
    //------------------------------------------------------
    // add by linmc,
    // force to Reset Color Std Setting during detect mode
    // the releated function is [ module_vdc_2D3Dset ] ->
    // [ drvif_module_vdc_YcSeparation ] -> [ fw_video_detectmode ]
    // 2008.10.21
    //------------------------------------------------------
    YcSeparationStatus = 0;
#endif
//	State_MessagePush(_MODE_MSG_MAIN_DETECT_SIGNAL);
    //RTD_Log(LOGGER_LEVEL,"\n _SHOW_INFO_STATE bCheckCCStatus=%d \n",(UINT32)bCheckCCStatus);

    COsdCoreExitOsd();
}

//******************************************************************************
//
// FUNCTION       : void TvAutoScanTask(void)
//
// USAGE          	: using table mapping: ATV table scan
//
// INPUT         	 	:	{ None	}
//
// OUTPUT         	:	{ None	}
//
// GLOBALS        	:
//
// USED_REGS      :
//
//******************************************************************************
void TvAutoScanTask(void)
{
    UINT32 tmp32;
    UINT8 autoscan_mode;
    INT32 offset;    
    UINT8 NtscType;//Tim 0324
    StructUserDataChannelSettingsType xdata stTVScanData;
	#if defined(CONFIG_CHANNEL_ATS)
    UINT8 ucAts;
	//	UINT8 nameBuf[5];
	#endif
    tvCoverCurFreq = tvScanCurFreq / 100;
    
	if(GET_TV_TABLE_SCAN()) 
	{
	
    NtscType = GET_TV_CUR_NtscType();
	#ifdef CONFIG_CHANNEL_TABLE_SHIFT_SCAN
		if (ucTableProgress_freqshift==0)
		{
			if(NtscType == 0)
				COsdCoreUpdateTableScanSlider(tvScanCurChn, (ucTableProgress - 1) * (UINT16)100 / (ucNtsc_CATV_Freq_Tbl_Count - 1), tvScanCurFreq );
			else
				COsdCoreUpdateTableScanSlider(tvScanCurChn ,(ucTableProgress - 2) * (UINT16)100 / (ucNtsc_AIR_Freq_Tbl_Count - 2), tvScanCurFreq );//Tim 0325
		
		#ifdef CONFIG_TUNER_SILICON
	        tvScanCurFreq = tNTSC_CATV_FREQ[ucTableProgress] ;
		#else
//******************************************************************************
        //Tim 0324

	    	NtscType = GET_TV_CUR_NtscType();

	        if(NtscType == 0)//Cable
	            tvScanCurFreq = tNTSC_CATV_FREQ[ucTableProgress];
	    	else
	            tvScanCurFreq = tNTSC_AIR_FREQ[ucTableProgress];

//******************************************************************************    	
		#endif
		}//if (ucTableProgress_freqshift==0)
	#else//#ifdef CONFIG_CHANNEL_TABLE_SHIFT_SCAN
		COsdCoreUpdateTableScanSlider(tvScanCurChn, ucTableProgress * (UINT16)100 / ucNtsc_CATV_Freq_Tbl_Count);//Tim 0325
		#if CONFIG_TUNER_SILICON
	        tvScanCurFreq = tNTSC_CATV_FREQ[ucTableProgress] ;
		#else
//******************************************************************************        
//Tim 0324
    	NtscType = GET_TV_CUR_NtscType();

        if(NtscType == 0)//Cable
            tvScanCurFreq = tNTSC_CATV_FREQ[ucTableProgress];
    	else
            tvScanCurFreq = tNTSC_AIR_FREQ[ucTableProgress] ;

//******************************************************************************        

		#endif//#if CONFIG_TUNER_SILICON

	#endif//#ifdef CONFIG_CHANNEL_TABLE_SHIFT_SCAN
	}
	else //if(GET_TV_TABLE_SCAN())
		COsdCoreUpdateChannelScanSlider(tvScanCurChn);//, tvCoverCurFreq);//drv_tuner_GeDiverRatio(tvScanCurFreq));//drv_tuner_GeDiverRatio(tvScanCurFreq));//drv_tuner_GeDiverRatio(tvScanCurFreq));//drv_tuner_GeDiverRatio(tvScanCurFreq));//Tim 0323

	if(colortype == ZAUTO)
	{
        //RTD_Log(LOGGER_LEVEL,"\nTim autoscan_mode \n");
	    autoscan_mode = fwif_tv_autoscanmodedetect();
	}
	else if((colortype == ZSECAM) && (soundtype == ATV_SOUND_STD_NICAM_L))
    	autoscan_mode = fwif_tv_autoscan_single_SECAML_funtion(tvScanCurFreq);	
    else if(colortype == ZSECAM)
        autoscan_mode = fwif_tv_autoscan_single_SECAMBG_DK_funtion(tvScanCurFreq);
    else if((colortype == ZPAL_I)  )//|| (colortype==ZSECAM))//Edit SVN53
    {
	    //RTD_Log(LOGGER_LEVEL,"\nTim single scan \n");
		autoscan_mode = fwif_tv_single_scan_PAL_funtion(tvScanCurFreq);
    }
    //else if((colortype == ZPAL_N) )
	  //  autoscan_mode = fwif_tv_single_scan_PAL_N_funtion(tvScanCurFreq);
	else
	    autoscan_mode = fwif_tv_single_scan_NTSC_funtion(tvScanCurFreq);

    // set tuner, set IFD and check if VD locked...
    //autoscan_mode = fwif_tv_autoscanmodedetect();
	//RTD_Log(LOGGER_LEVEL,"\nTim autoscan_mode=%d \n",(UINT32)autoscan_mode);
    if (autoscan_mode != 0)  // ok, we got signal...
    {
		if(tvScanCurChn == 0)
		{
			if(COsdCtrlGetNtscFree())//Table
                tvTableScanFirstChn = ucTableProgress;
			else
				tvTableScanFirstChn = tvScanCurChn+1;
		}
		//RTD_Log(LOGGER_LEVEL,"\n Tim tvTablescanfirstchn = %d ,uctableprogress=%d\n",(UINT32)tvTableScanFirstChn,(UINT32)ucTableProgress);
		// update chnNum, frequency with MainStep
		tvScanCurChn += 1;
		
	    if (GET_TV_TABLE_SCAN())
			offset = 0;
		else
	        offset = drvif_tuner_get_afcoffset(0);
	
        RTD_Log(LOGGER_DEBUG,"the auto scan offset ... %x\n", (UINT32)offset);
        RTD_Log(LOGGER_DEBUG,"the auto scan mode ... %x\n", (UINT32)autoscan_mode);
        RTD_Log(LOGGER_DEBUG,"0xb80d10c0~10c4 %x,%x\n", rtdf_inl(0xb80d10c0), rtdf_inl(0xb80d10c4));
        //RTD_Log(LOGGER_DEBUG,"ucTableProgress ... %d\n", (UINT32)ucTableProgress);

        //exact freq
        tvScanCurFreq += offset;

        if ( GET_TV_TABLE_SCAN())
        {
	        NtscType = GET_TV_CUR_NtscType();
	        if(NtscType == 0)//Cable
	            COsdCoreUpdateTableScanSlider(tvScanCurChn,(ucTableProgress - 1) * (UINT16)100 / (ucNtsc_CATV_Freq_Tbl_Count - 1), tvScanCurFreq );
	        else	        
	            COsdCoreUpdateTableScanSlider(tvScanCurChn, (ucTableProgress - 2) * (UINT16)100 / (ucNtsc_AIR_Freq_Tbl_Count - 2), tvScanCurFreq );//Tim 0325
        }else
            COsdCoreUpdateChannelScanSlider(tvScanCurChn);//, drv_tuner_GeDiverRatio(tvScanCurFreq));

		#ifndef CONFIG_TUNER_SILICON		
	        stTVScanData.chnNo = ucTableProgress;//Tim 0324
        #else
	        stTVScanData.chnNo = tvScanCurChn;//Tim 0324
        #endif
        //RTD_Log(LOGGER_LEVEL,"\n Tim first chn=%d,freq=%d\n",(UINT32)tvScanCurChn,(UINT32)tvScanCurFreq);
        //RTD_Log(LOGGER_LEVEL,"\nTim autoscan_mode=%d \n",(UINT32)autoscan_mode);
        if(colortype == ZAUTO)
        stTVScanData.colorSystem = autoscan_mode;
        else if((colortype == ZSECAM) && (soundtype == ATV_SOUND_STD_NICAM_L))
        stTVScanData.colorSystem = ZSECAML;//autoscan_mode;
        else
        stTVScanData.colorSystem = colortype;//autoscan_mode;
        stTVScanData.freq = tvScanCurFreq;
        stTVScanData.offset = _TV_FineTuneBar_ZeroPoint;
        stTVScanData.status = _CHN_STS_LOCK|_CHN_STS_AFC_SKIP;//Tim 0404
        if((colortype == ZAUTO) || (colortype == ZPAL_M) || (colortype == ZPAL_N))
        stTVScanData.soundSystem = CTvScanGetAudioStd(autoscan_mode);
        else
        stTVScanData.soundSystem = soundtype;//CTvScanGetAudioStd(autoscan_mode);

        flow_timer_Reset_WatchDog();
		#if defined(CONFIG_CHANNEL_ATS)
	        if ( stTVScanData.colorSystem != ZNTSC)
				#ifdef CONFIG_STORAGE_ACCESS_FLASH	// marked by weihao because of china
	            ucAts = CGetNetworkInfo(stTVScanData.name);
				#else
	        	ucAts = 0;
				#endif
	        else
		#endif
	        {
			#ifdef CONFIG_STORAGE_ACCESS_FLASH	// marked by weihao because of china
	            stTVScanData.name[0] = _C_;
	            stTVScanData.name[1] = _MINUS_;
	            stTVScanData.name[2] = (tvScanCurChn / 100) % 10 + _0_;
	            stTVScanData.name[3] = (tvScanCurChn / 10) % 10 + _0_;
	            stTVScanData.name[4] = tvScanCurChn % 10 + _0_;
			#endif
	        }

        // Write chanel data to flash
		#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
			#if defined(_REALTEK_OSD_USE)
	        ui_set_chn_data(tvScanCurChn - 1, &stTVScanData);
	        #endif
		#else
	        RTD_Log(LOGGER_DEBUG,"ucTableProgress ... %d\n", (UINT32)ucTableProgress);
			//Tim 0324
			if(COsdCtrlGetNtscFree())//Table
			{
                if(GET_TV_CUR_NtscType() == 0)//Cable
                    SYNC_STORAGE_ARRAY_STRUCT_DATA(StructTvCableDataType, /*tvScanCurChn - 1*/ucTableProgress, stTVScanData);
                else
                    SYNC_STORAGE_ARRAY_STRUCT_DATA(StructTvAirDataType, /*tvScanCurChn - 1*/ucTableProgress, stTVScanData);
			}
			else
			//RTD_Log(LOGGER_LEVEL,"\n Tim Save Chn = %d,freq=%d \n",(UINT32)tvScanCurChn,(UINT32)tvScanCurFreq);
	        SYNC_STORAGE_ARRAY_STRUCT_DATA(StructUserDataChannelSettingsType, /*tvScanCurChn - 1*/tvScanCurChn, stTVScanData);
		#endif//#if defined(_DISABLE_RT_STORAGE_FOR_TEST)

        // connect channel number
        ui_set_channel_idx(tvScanCurChn, tvScanCurChn);//Tim 0419  tvScanCurChn-1

        tmp32 = GET_CHANNEL_FREQ(ucTableProgress);

		if ( GET_TV_TABLE_SCAN()) 
		{
			ucTableProgress += 1;
			#ifdef CONFIG_CHANNEL_TABLE_SHIFT_SCAN
				ucTableProgress_freqshift = 0;
			#endif
		}
		else
			tvScanCurFreq += _TV_TUNER_MAIN_STEP;
		//flow_display_FreeRun_Mode();//Tim 0426
		//fw_timer_event_DelayXms(10);
		//flow_display_Framesync_Mode();
        //rtdf_maskl(DISPLAY_TIMING_CTRL1_0x2000, ~(_BIT26 ), _BIT26);

	}
	else //if (autoscan_mode != 0) 
	{
		if ( GET_TV_TABLE_SCAN()) 
		{
		#ifdef CONFIG_CHANNEL_TABLE_SHIFT_SCAN
			if(ucTableProgress_freqshift==0)
			{
//******************************************************************************
				//Tim 0324
		        NtscType = GET_TV_CUR_NtscType();
		        if(NtscType == 0)//Cable
					tvScanCurFreq = tNTSC_CATV_FREQ[ucTableProgress];// + (_TV_TUNER_SUB_STEP<<1);
				else
	                tvScanCurFreq = tNTSC_AIR_FREQ[ucTableProgress];// + (_TV_TUNER_SUB_STEP<<1);
				ucTableProgress_freqshift = 1;
			}
			else if(ucTableProgress_freqshift==1)
			{
	            if(NtscType == 0)//Cable
					tvScanCurFreq = tNTSC_CATV_FREQ[ucTableProgress];// - (_TV_TUNER_SUB_STEP<<1);
				else
					tvScanCurFreq = tNTSC_AIR_FREQ[ucTableProgress];// - (_TV_TUNER_SUB_STEP<<1);
				ucTableProgress_freqshift = 2;
			}
			else 
			{
				ucTableProgress += 1;
				ucTableProgress_freqshift = 0;
			}
//******************************************************************************

		#else//#ifdef CONFIG_CHANNEL_TABLE_SHIFT_SCAN
			ucTableProgress += 1;
		#endif
		}
        else//if ( GET_TV_TABLE_SCAN())
            tvScanCurFreq += _TV_TUNER_SUB_STEP;

		#ifdef CONFIG_STORAGE_ACCESS_EEPROM
	        if ((tvScanCurChn < stInitialNum) && (stInitialNum > 0))
	        {
	            LOAD_STORAGE_ARRAY_STRUCT_DEFAULT_DATA(StructUserDataChannelSettingsType, 0, stTVScanData);
	            SYNC_STORAGE_ARRAY_STRUCT_DATA(StructUserDataChannelSettingsType, stInitialNum - 1, stTVScanData);
	            stInitialNum--;
	        }
		#endif
    }

    if ( GET_TV_TABLE_SCAN())
    {
//******************************************************************************
		//Tim 0324
	    NtscType = GET_TV_CUR_NtscType();
	    if(NtscType == 0)//Cable
		{
	        if (ucTableProgress>ucNtsc_CATV_Freq_Tbl_Count)
	            fw_flow_message_push(_UI2FL_TV_AUTOSCAN_CANCEL);	// inform flow to cancel channel scan
		}
		else
		{
	        if (ucTableProgress>ucNtsc_AIR_Freq_Tbl_Count)
	            fw_flow_message_push(_UI2FL_TV_AUTOSCAN_CANCEL);	// inform flow to cancel channel scan

		}
    }//if ( GET_TV_TABLE_SCAN())
    else
    {
        if (tvScanCurFreq > drv_tuner_GetChanelMaxFreq())
            fw_flow_message_push(_UI2FL_TV_AUTOSCAN_CANCEL);	// inform flow to cancel channel scan
    }

//******************************************************************************

}

#if 0
UINT8 CTVGetMAXChannelNum(void)
{
    return _MAX_PAL_CATV_NUM;
}
#endif
/**
 * To start manual channel scan.
 * @return none.
 */
void CTvManualScanStart(void)
{
    UINT8 curChn;

    drv_video_reset_HPLL_default();
#if defined(CONFIG_CHANNEL_ATS)
    drv_CNI_Init();
#endif


    curChn = GET_TV_CUR_CHN();
    tvScanCurChn = curChn;
    tvScanCurFreq = GET_CHANNEL_FREQ(tvScanCurChn);
    // prevent when finetune set to -13, the snow screen will become "white screen"
    // add by linmc, 2009.04.24
    if (GET_CHANNEL_FREQ_OFFSET(curChn)  >  _TV_FineTuneBar_ZeroPoint)
        tvScanCurFreq += ((GET_CHANNEL_FREQ_OFFSET(curChn) - _TV_FineTuneBar_ZeroPoint) * drv_tuner_GetRatioInStepSize()); // charlie, 20090122, for TV AFC skip
    else
        tvScanCurFreq -= ((_TV_FineTuneBar_ZeroPoint - GET_CHANNEL_FREQ_OFFSET(curChn) ) * drv_tuner_GetRatioInStepSize()); // charlie, 20090122, for TV AFC skip


    // <Stanley K90326> Added, if entry manual scan function, but do manual scan then exit manual function,  just change fine tune value.
    //SET_CHANNEL_FREQ_OFFSET(tvScanCurChn, _TV_FineTuneBar_ZeroPoint); // reset the offset value, range is from 0~48


    if (tvScanCurFreq < drv_tuner_GetChanelMinFreq())
    {
        tvScanCurFreq = drv_tuner_GetChanelMinFreq() + _TUNER_MANUAL_SCAN_MIN_SETP;
        if (GET_TV_MANUAL_SCAN_DIRECTION() == _MANUAL_SCAN_DEC)
            tvScanCurFreq = drv_tuner_GetChanelMaxFreq() - _TUNER_MANUAL_SCAN_MIN_SETP;
    }
    else if (tvScanCurFreq > drv_tuner_GetChanelMaxFreq())
    {
        tvScanCurFreq = drv_tuner_GetChanelMaxFreq() - _TUNER_MANUAL_SCAN_MIN_SETP;
        if (GET_TV_MANUAL_SCAN_DIRECTION() == _MANUAL_SCAN_INC)
            tvScanCurFreq = drv_tuner_GetChanelMinFreq() + _TUNER_MANUAL_SCAN_MIN_SETP;
    }
    //RTD_Log(LOGGER_DEBUG,"[Manual SCAN]  is start: %d\n", tvScanCurFreq);
    COsdCoreUpdateChannelScanSlider(tvScanCurChn);//, drv_tuner_GeDiverRatio(tvScanCurFreq));

#ifdef CONFIG_IFD_ENABLE
    drvif_ifd_cr_freq_status_check(0);
    drvif_ifd_set_agc_variance_off(0xf);
    drvif_ifd_agc_peak_en(1);
#endif
    //Audio_AtvSetSoundStd(ATV_SOUND_STD_AUTO);

    drv_video_vdc_BlueScreenEnable(_DISABLE);

    if (GET_TV_MANUAL_SCAN_DIRECTION() == _MANUAL_SCAN_INC)
    {
		tvScanCurFreq += _TUNER_MANUAL_SCAN_MIN_SETP*6;
    }
    else
    {
		tvScanCurFreq -= _TUNER_MANUAL_SCAN_MIN_SETP*6;
    }
    //drvif_scaler_set_display_mode(DISPLAY_MODE_FRC);//when scan channel, change timing to ref. FRC

#if 0 // CONFIG_ENABLE_3D_SETTING
    drvif_module_vdc_Disable3DFunction();
    cvideo_tv_check_3d_is_running = _OFF;
    ScalerTimer_CancelTimerEvent(drvif_module_vdc_TVCheck3D);
#endif

#ifdef CONFIG_ENABLE_TV_NOISE_BOX_ENALBE
    auto_scan_avout_for_noise_box_enable=1;
    //module_vdc_AVoutMode(VDC_AVOUT_TV_NOISE_BOX);
    drv_video_vdc_AvOutMode(VDC_AVOUT1, VDC_AVOUT_CVBS_NO_FIL_NO_AGC)
#endif

    rtdf_maskl(VDC_VD_BASIC_REGISTER_ENABLE_0x51d4, 0xdf, 0x00);
    rtdf_maskl(VDC_VIDEO_CONTROL0_0x5000, 0xf0, 0x00);

    //tvManualScanFlag = 1;   // start to scan

}

void CTvManualScanStop(void)
{
#if defined(CONFIG_CHANNEL_ATS)
    UINT8 ucAts;
#endif
    StructUserDataChannelSettingsType  stTVScanData;

    //drv_scalerdisplay_set_bg_color(0,0,0);
    //drv_scalerdisplay_ForceBackground(_ENABLE);
    usAfcCurFreq = tvScanCurFreq;
    LOAD_STORAGE_ARRAY_STRUCT_DATA(StructUserDataChannelSettingsType, tvScanCurChn-1, stTVScanData);

    RTD_Log(LOGGER_DEBUG,"TV color system =%x\n", (UINT32)stTVScanData.colorSystem);


    tvManualScanFlag = 0;   //jj 20080424
#if defined(CONFIG_CHANNEL_ATS)
    flow_timer_Reset_WatchDog();
    if (( stTVScanData.colorSystem != ZNTSC)&&(stTVScanData.status & _BIT7))
#ifdef CONFIG_STORAGE_ACCESS_FLASH	// marked by weihao because of china
        ucAts = CGetNetworkInfo(stTVScanData.name);
#else
        ucAts = 0;
#endif

    SYNC_STORAGE_ARRAY_STRUCT_DATA(StructUserDataChannelSettingsType, tvScanCurChn-1, stTVScanData);
#endif

    //drvif_scaler_set_display_mode(DISPLAY_MODE_FRAME_SYNC);//after scan channel, change timing to ref. FrameSync

    if (!GET_AUDIO_MUTE())
        flow_audio_Setmute(_DISABLE, SCALER, (ADC_CH_IDX)(ADC_CH_L|ADC_CH_R|ADC_CH_C|ADC_CH_SW));

    //SET_CHANNEL_FREQ(tvScanCurChn, tvScanCurFreq);
    CLR_AFC_SKIP(tvScanCurChn); // charlie, 20090122, for TV AFC skip
    //drv_scalerdisplay_set_bg_color(0,0,0);
    //drv_scalerdisplay_ForceBackground(_ENABLE);
    //TvSetChannel(GET_TV_CUR_CHN());

    cvideo_tv_channel_scan_is_running = _OFF;

}

void TvManualScanTask(void)
{
    INT32 offset;
    UINT32 	freq = 0 ;
    StructUserDataChannelSettingsType  stTVScanData;

    UINT8 autoscan_mode;


    /*wjchen 20070813, start for TV set 3D function*/
    cvideo_tv_channel_scan_is_running = _ON;

    Audio_AtvSetSoundStd(ATV_SOUND_STD_AUTO);
    autoscan_mode = fwif_tv_autoscanmodedetect();

    if (autoscan_mode != 0)
    {	// OK, we got signal...
        tvManualScanFlag = 0;

        // It need to set up scaler to display correctly
        //CTVQuickChangeTiming();

        // get offset
        offset = drvif_tuner_get_afcoffset(0);
        // exact freq
        freq = tvScanCurFreq + offset;


        tvScanCurFreq = freq;
        RTD_Log(LOGGER_DEBUG,"tuner set frq %d\n",(UINT32) tvScanCurFreq);

        fw_tv_setvideomode(autoscan_mode, tvScanCurFreq);
        drv_tuner_SetFreq(freq );
        COsdCoreUpdateChannelScanSlider(tvScanCurChn);//, drv_tuner_GeDiverRatio(tvScanCurFreq));

        stTVScanData.freq = freq;
        stTVScanData.offset = _TV_FineTuneBar_ZeroPoint;
        stTVScanData.colorSystem = autoscan_mode;
        stTVScanData.status = _CHN_STS_LOCK;
        stTVScanData.soundSystem = CTvScanGetAudioStd(autoscan_mode);

        TVSetAudioStandard(stTVScanData.soundSystem);

        if (autoscan_mode != GET_CHANNEL_COLORSYSTEM(GET_TV_CUR_AIR_CHN()))
        {
            if ((autoscan_mode == ZNTSC) || (autoscan_mode == ZNTSC_443) || (autoscan_mode == ZPAL_M))
            {
                drv_sig_tv_SetVGIP(_MODE_480I);
                RTD_Log(LOGGER_DEBUG,"[TV]Set VGIP = 480i, colorSystem=%x\n", autoscan_mode);
            }
            else
            {//end yy1.8 	//TV mode=PAL
                drv_sig_tv_SetVGIP(_MODE_576I);
                RTD_Log(LOGGER_DEBUG,"[TV]Set VGIP = 576i, colorSystem=%x\n", autoscan_mode);
            }//yy 1.8 end
            fw_flow_message_push(_MODE_MSG_MAIN_SET_VGIP_AFTER_FOUND_MODE_SIGNAL);
        }
        else
        {
            if (!GET_AUDIO_MUTE())
                flow_audio_Setmute(_DISABLE, SCALER, (ADC_CH_IDX)(ADC_CH_L|ADC_CH_R|ADC_CH_C|ADC_CH_SW));

        }

        SYNC_STORAGE_ARRAY_STRUCT_DATA(StructUserDataChannelSettingsType, tvScanCurChn-1, stTVScanData);

        // connect channel number
        ui_set_channel_idx(tvScanCurChn, tvScanCurChn-1);

        // unmute audio
#if 0
        if (!GET_AUDIO_MUTE())
            fwif_audio_setmute(_DISABLE, SCALER, ADC_CH_L|ADC_CH_R);
#endif
    }
    else
    {
        if (GET_TV_MANUAL_SCAN_DIRECTION() == _MANUAL_SCAN_INC)
        {
            tvScanCurFreq += _TV_TUNER_SUB_STEP;
            if (tvScanCurFreq > drv_tuner_GetChanelMaxFreq())
                tvScanCurFreq = drv_tuner_GetChanelMinFreq(); // + _TUNER_MANUAL_SCAN_MAX_SETP;
        }
        else
        {
            tvScanCurFreq -= _TV_TUNER_SUB_STEP;
            if (tvScanCurFreq < drv_tuner_GetChanelMinFreq())
                tvScanCurFreq = drv_tuner_GetChanelMaxFreq(); // - _TUNER_MANUAL_SCAN_MAX_SETP;
        }

    }
    COsdCoreUpdateChannelScanSlider(tvScanCurChn);//, drv_tuner_GeDiverRatio(tvScanCurFreq));

}

void ap_tv_AFC_Tracking(void)
{
    //UINT8 afc;
#ifdef CONFIG_TUNER_SILICON

    UINT32 usFreq;
    bool afc_status=_TRUE;
    StructUserDataChannelSettingsType stChannelData;
    UINT8 chn;

	chn = GET_TV_CHN_DATA(_CUR_CHN_TV);
	
    //chn = CurChn;
    //LOAD_STORAGE_ARRAY_STRUCT_DATA(StructUserDataChannelSettingsType, chn - 1, stChannelData);
    ui_get_chn_data(ui_get_channel_idx(chn), &stChannelData);
    if ( cvideo_tv_channel_scan_is_running)
        return;
    usFreq = stChannelData.freq;
//	RTD_Log(LOGGER_DEBUG,"[AFC] usFreq = %d \n",usFreq);
    afc_status=drvif_ifd_afc_auto_tracking(&usFreq);
    if (afc_status==_TRUE)
    {
        usAfcCurFreq = usFreq;
    }

#else
    UINT8 ucTunerLocked = 0;
#ifdef CONFIG_IFD_ENABLE
    UINT32 usFreq;
    INT32 iAfcOffset;
    UINT32 ucSubStep = 10*drv_tuner_GetRatioInStepSize();
#endif
    UINT8 chn;

	chn = GET_TV_CHN_DATA(_CUR_CHN_TV);
	
    //RTD_Log(LOGGER_DEBUG,"[AFC] usAfcCurFreq = %d \n",(UINT32)usAfcCurFreq);

    if ( cvideo_tv_channel_scan_is_running)
        return;
    usFreq = GET_CHANNEL_FREQ(chn);
    flow_timer_Reset_WatchDog();
#ifdef CONFIG_IFD_ENABLE

    if (AFC_stage_count<3)
    {
        AFC_stage_count ++;
        return;
    }
    else
    {
        AFC_stage_count = 0;
    }

    if ((drv_video_vdc_CheckHVisLocked())&&(drvif_ifd_get_cr_lock()==_TRUE))
    {
        // CF 20080114 added, new version of AFC tracking
        if (drv_ifd_Freq_Offset_Small()!= _TRUE)
        {	// RF Tuner mislock, AFC tracking action...
            usFreq = usAfcCurFreq;
            RTD_Log(LOGGER_DEBUG,"[AFC]HV lock but Tuner mislock...\n");
            iAfcOffset = drvif_tuner_get_afcoffset(0);
            RTD_Log(LOGGER_DEBUG,"[afc]AFC offset = %d\n", (UINT32)iAfcOffset);
            usFreq += iAfcOffset;
            drvif_ifd_cr_freq_status_check(0);
            drv_tuner_SetFreq(usFreq);
            fw_timer_event_DelayXms(15);
            usAfcCurFreq = usFreq;
            AFC_stage_count = 0;
        }
        ucTunerLocked = _TRUE;
        ucAfcDirection = 0;
        //marked by Hardy20081230 for no dectect change on plug in/out tv cable
        //if(GET_TVBLUESCREEN_TYPE()==_TRUE) 	// BUG PR00060:reynold_jao 071003 fixed
        //fw_control_force_bg(info->display, _FALSE);
        //end Hardy20081230
    }
    else
    {
        AFC_stage_count =3;
        //RTD_Log(LOGGER_DEBUG,"[afc]hv lock= %d\n", drvif_module_vdc_CheckHVisLocked());
        usAfcCurFreq = usFreq;

        //RTD_Log(LOGGER_DEBUG,"[AFC]HV mislock...\n");
        switch (ucAfcDirection)
        {
        case 0:
            usFreq += 0;
            break;
        case 1:
            usFreq += ucSubStep;
            break;
        case 2:
            usFreq -= 0;
            break;
        case 3:
            usFreq -= ucSubStep;
            break;
        default:
            usFreq += 0;
            break;
        }
        //RTD_Log(LOGGER_DEBUG,"[AFC] direction = %d, usFreq = %d \n",(UINT32)ucAfcDirection,(UINT32)usFreq);
        //RTD_Log(LOGGER_DEBUG,"Freq = %d, AFC Freq = (%d)\n", stUserDataChannelSettings[stTvInfo.CurChn-1].freq, usFreq);
        ucAfcDirection++;
        if ( ucAfcDirection> 3)
            ucAfcDirection= 0;
        drvif_ifd_cr_freq_status_check(0);
        drv_tuner_SetFreq(usFreq);
        fw_timer_event_DelayXms(30);
        //~~drvif_module_ifd_Reset_nco();
        //ScalerTimer_DelayXms(50);
        //RTD_Log(LOGGER_DEBUG,"[afc]cr lock 50= %d\n", drvif_module_ifd_CRLock());
        //RTD_Log(LOGGER_DEBUG,"[afc]agc lock 50= %d\n", drvif_module_ifd_AGCLock());
        //RTD_Log(LOGGER_DEBUG,"[afc]hv lock= %d\n", drvif_module_vdc_CheckHVisLocked());
        if ((drvif_ifd_get_cr_lock() == _TRUE) && (drvif_ifd_get_agc_lock() == _TRUE))
//		if (drvif_module_vdc_CheckHVisLocked() == _TRUE)
        {

            iAfcOffset = drvif_tuner_get_afcoffset(0);
//->w			RTD_Log(LOGGER_DEBUG,"[afc] AFC  find signal !!  usFreq= %d, AFC offset = %d\n", (UINT32)usFreq, (UINT32)iAfcOffset);
            usFreq += iAfcOffset;
            drv_tuner_SetFreq(usFreq);
            fw_timer_event_DelayXms(15);
            //~~drvif_module_ifd_Reset_nco();
            fw_timer_event_DelayXms(50);
            //~~drvif_module_ifd_Clamping_Reset();	// irene@081126 for white pattern
            usAfcCurFreq = usFreq;
            ucAfcDirection= 0;
            AFC_stage_count = 0;
        }
#endif
        //marked by Hardy20081230 for no dectect change on plug in/out tv cable

        /*
        		if(GET_TVBLUESCREEN_TYPE()==_TRUE) 	// BUG PR00060:reynold_jao 071003 fixed
        			fw_control_force_bg(info->display, _ENABLE);
        		else

        			RTD_Log(LOGGER_INFO,"no fore to background\n");
        			fw_control_force_bg(info->display, _FALSE);
        			*/
        //end Hardy20081230
    }
#endif
}
#endif
