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
#ifndef _TV_H
#define _TV_H
#include "sparrow.h"
#include "source.h"

#define _TV_TYPE_SUPPORT             1       // Support this type
#define _TV_TYPE_NOT_SUPPORT         0       // Not support this type

#define _TV_NTSC_SUPPORT			_TV_TYPE_SUPPORT
#define _TV_PAL_SUPPORT				_TV_TYPE_SUPPORT
#define _TV_SECAM_SUPPORT			_TV_TYPE_SUPPORT
#define _TV_NTSC_443_SUPPORT		_TV_TYPE_NOT_SUPPORT


#define _TV_M_SUPPORT				_TV_TYPE_SUPPORT
#define _TV_BG_SUPPORT				_TV_TYPE_SUPPORT
#define _TV_DK_SUPPORT				_TV_TYPE_SUPPORT
#define _TV_N_SUPPORT				_TV_TYPE_SUPPORT
#define _TV_I_SUPPORT				_TV_TYPE_SUPPORT
#define _TV_L_SUPPORT				_TV_TYPE_SUPPORT//Edit SVN53
#define _TV_LL_SUPPORT				_TV_TYPE_NOT_SUPPORT

//#define	_MAX_CATV_NUM 	    99//Ding modify 08-01-31 //255
#define _SOUND_AUTO					0
#define _SOUND_DK					1
#define _SOUND_I					2
#define _SOUND_BG					3
#define _SOUND_MN					4
#define _SOUND_N					5//
#define _SOUND_L					6
#define _SOUND_UNKNOW				7//6

#define _TV_NEXT_CHANNEL			0x1
#define _TV_PREV_CHANNEL			0x2
#define _TV_LAST_CHANNEL			0x3
#define _TV_FAV_CHANNEL			0x4
#define _TV_NEXT_ONE_CHANNEL	0x5
#define _TV_PREV_ONE_CHANNEL	0x6
#define _TV_RETURN_CHANNEL		0x07//Tim 0404
#define _DISABLE_SKIP				0x10 //charlie, 20071029

enum
{
    _TV_SYSTEM_AIR = 0,
    _TV_SYSTEM_CABLE,
};

enum
{
    _TV_TYPE_ATV =0,
    _TV_TYPE_DTV,
};


#define _TV_TUNER_MAIN_STEP     (50000)	// 5Mhz
#if 0//def CONFIG_TUNER_SILICON
#define _TV_TUNER_SUB_STEP  	(5000)
#else
#define _TV_TUNER_SUB_STEP  	(2500)
#endif


// charlie, 20090210, In TV Auto Scan 時, 在掃台的時候, 如果有更新的台數, 加在原有台數的後面
#define _EMPTY_FREQ 0//8632500
//extern UINT8 xdata tvScanStopChn;
extern UINT32 xdata tvCoverCurFreq;
extern UINT8 xdata tvScanCurChn;
extern UINT8 xdata tvAutoScanFlag;
extern UINT8 xdata tvManualScanFlag;
//extern UINT8 xdata tvManualScanRun;
extern UINT32 xdata tvScanCurFreq;
extern UINT8 ucVdc_TV_Mode;
/*==================== Definitions ================ */
UINT8 CCheckTVChannelLock(void);
//UINT8 CTvSetChStatus(UINT8 chn,UINT8 status, UINT8 mode);
//UINT8 CTvGetChStatus(UINT8 chn,UINT8 mode);
void TVSetAudioA2MTS(UINT8 mode);
void TVSetAudioNicamMTS(UINT8 mode);
void TVSetAudioMTS(UINT8 mode);
void TVSetAudioStandard(UINT8 soundSystem);
void fw_tv_setvideomode(UINT8 video_mode, UINT32 freq);
void CTVSetVideoManualMode(UINT8 mode);
UINT8 CTvScanGetAudioStd(UINT8 vd_std);
UINT8 TvSetChannel(UINT8 chn);
UINT8 CTvSetChannel(UINT8 chn);
UINT8 CTvSetPvrChannel(UINT16 majornum, UINT16 minornum);
UINT8 CTvSelectChannel(UINT8 operation);
//UINT8 CTVGetMAXChannelNum(void);
void CTvResetUserDataChannelSettings(UINT8 ucStartCh);

void CTvAutoScanStart(void);
void CTvAutoScanStop(void);
void TvAutoScanTask(void);
void CTvManualScanStart(void);
void CTvManualScanStop(void);
void TvManualScanTask(void);
void CTvManualScan(void);

void ap_tv_AFC_Tracking(void);
// Initialize parameter when boot
void fw_tv_system_Init(void);
//void CLoadTableProc(void);

#ifdef CONFIG_IFD_ENABLE
UINT8 fwif_tv_autoscan_PAL_funtion(UINT32 tvAutoScanFreq);
UINT8 fwif_tv_autoscan_PAL_M_funtion(UINT32 tvAutoScanFreq);
UINT8 fwif_tv_autoscan_PAL_N_funtion(UINT32 tvAutoScanFreq);

#if defined(CONFIG_ALL_AUTO_SCAN_FLOW)||defined(CONFIG_EUROPE_AUTO_SCAN_FLOW)
#if 1//defined(CONFIG_APP_EURO)
UINT8 fwif_tv_autoscan_SECAML_funtion(UINT32 tvAutoScanFreq);
UINT8 fwif_tv_autoscan_SECAMLA_funtion(UINT32 tvAutoScanFreq);
UINT8 fwif_tv_auto_SECAML_funtion( UINT32 tvAutoScanFreq);
UINT8 fwif_tv_auto_SECAMLA_funtion( UINT32 tvAutoScanFreq);
#endif
#endif

UINT8 fwif_tv_autoscan_NTSC_funtion(UINT32 tvAutoScanFreq);
#endif



#endif // #ifdef _TV_H
