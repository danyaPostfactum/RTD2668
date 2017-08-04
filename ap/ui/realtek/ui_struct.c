/*=============================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2009
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

#define _UI_STRUCT_C

/*============================ Module dependency  ===========================*/
#include "ui_struct.h"
#include "rosprintf.h"
#include "eepromdefaultcustomer.h"
#include "tv.h"
#include "osdparameter.h"
#include "rt_storage.h"
#include "storage_system.h"
#include "vip/color.h"
#include <tuner\tuner.h>
#include "sparrow\imem.h"
#include "flow_struct.h"
#include "mode_customer.h"
#include "power.h"
#include "osdcontrol.h"
#include "autoconf.h"

#if 1
UINT8 xdata stUISignalSource = 4;	// 0: VGA  // 2: TV	// 4:AV	//5:SV	// 6: YPBPR  //7:Full SCART	//8:Half SCART
UINT8 xdata ucUartPath = 1;
UINT8 xdata ucStorageTemp = 0;
#if CONFIG_USB_CHECK
UINT8 xdata stUsbFlag=0;
#endif
StructFacColorDataType xdata stFacColorData;

StructUserDataChannelSettingsType xdata stTempChannelData;
StructTvCableDataType			  xdata stTempCableChannelData;
StructTvAirDataType				  xdata stTempAirChannelData;


// StructUserDataChannelSettingsType xdata stChannelTempBuffer[_MAX_PAL_CATV_NUM] _at_ 0x8200;

/** for PQ setting */
UINT8 xdata ucFacDCR = 0;

#if 0	// not used
UINT8 ui_get_source_idx(void)
{
    UINT8 getVal;
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    getVal = _stSystemData.InputSource;
#else
    LOAD_STORAGE_DATA(StructSystemDataType, InputSource, getVal);
#endif
    return getVal;
}

void ui_set_source_idx(UINT8 srcIdx)
{
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    _stSystemData.InputSource = srcIdx;
#else
    SYNC_STORAGE_DATA(StructSystemDataType, InputSource, srcIdx);
#endif
}

UINT8 ui_get_channel_number(UINT8 ucIdx)
{
    UINT8 getVal;

#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    getVal = _stchannelData[ucIdx].chnNo;
#else
    LOAD_STORAGE_ARRAY_DATA(StructUserDataChannelSettingsType, ucIdx, chnNo, getVal);
#endif
    return getVal;
}

void ui_set_channel_number(UINT8 ucIdx, UINT8 ucNumber)
{
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    _stchannelData[ucIdx].chnNo = ucNumber;
#else
    SYNC_STORAGE_ARRAY_DATA(StructUserDataChannelSettingsType, ucIdx, chnNo, ucNumber);
#endif
}
#endif
#ifdef _VIDEO_TV_SUPPORT

UINT8 ui_get_channel_idx(UINT8 ucChn)
{
    UINT8 ucIdx;



    if ( ucChn < 1)
        ucChn = 1;
	//RTD_Log(LOGGER_LEVEL,"\n Tim ucChn get=%d \n",(UINT32)ucChn);

	if(COsdCtrlGetNtscFree())//Table
	{
        if(GET_TV_CUR_NtscType() == 0)//Cable
            LOAD_STORAGE_DATA_OFFSET(StructTVChannelMapTable, TVMapIndex, ucChn, 1, ucIdx);
        else        
            LOAD_STORAGE_DATA_OFFSET(StructTVChannelMapTable, TVMapIndex, ucChn, 1, ucIdx);
	}
	else
    LOAD_STORAGE_DATA_OFFSET(StructTVChannelMapTable, TVMapIndex, ucChn, 1, ucIdx);
	//RTD_Log(LOGGER_LEVEL,"\n Tim ucidx0=%d \n",(UINT32)ucIdx);

    if (ucIdx > _MAX_CATV_NUM - 1)
        ucIdx = 0;
	//RTD_Log(LOGGER_LEVEL,"\n Tim ucidx=%d \n",(UINT32)ucIdx);
    return ucIdx;
}

void ui_set_channel_idx(UINT8 ucChn, UINT8 ucIdx)
{
    if ( ucIdx > _MAX_CATV_NUM - 1)
        ucIdx = _MAX_CATV_NUM - 1;
    if ( ucChn  < 1)
        ucChn = 1;
//	stTvChannelMap.TVMapIndex[ucChn-1] = ucIdx;
    SYNC_STORAGE_DATA_OFFSET(StructTVChannelMapTable, TVMapIndex, ucChn, 1, ucIdx);
}


UINT8 ui_get_tv_data(UINT8 id)
{
    UINT8 ucVal;
    UINT8 TableType,FreeType;

	if(id>=_BLUE_SCREEN_TV)
	{
        LOAD_STORAGE_DATA(StructTvDataType, NtscType, TableType);
        LOAD_STORAGE_DATA(StructTvDataType, FreeType, FreeType);
        if(TableType)//Table
		{
			if(FreeType == 0)//Cable	
                id = id - _BLUE_SCREEN_TV + 5;
			else
				id = id - _BLUE_SCREEN_TV + 10;
		}
		else
		{
			id = id - _BLUE_SCREEN_TV;
		}
	}

    switch (id)
    {
    case _TV_BLUE_SCREEN:
        LOAD_STORAGE_DATA(StructTvDataType, BlueScreen, ucVal);
        break;

    case _TV_CUR_CHN:
        LOAD_STORAGE_DATA(StructTvDataType, CurChn, ucVal);
        break;

    case _TV_PRE_CHN:
        LOAD_STORAGE_DATA(StructTvDataType, PreChn, ucVal);
        break;

    case _TV_TOTAL_CHN:
        LOAD_STORAGE_DATA(StructTvDataType, ucTotalChn, ucVal);
        break;

    case _TV_COUNTRY_SEL:
        LOAD_STORAGE_DATA(StructTvDataType, CountrySelect, ucVal);
        break;

//Tim 0330
//==============================================================================
	case _TV_BLUE_SCREEN_CABLE:
	    LOAD_STORAGE_DATA(StructTvDataType, BlueScreen_Cable, ucVal);
	    break;

	case _TV_CUR_CHN_CABLE:
	    LOAD_STORAGE_DATA(StructTvDataType, CurChn_Cable, ucVal);
	    break;

	case _TV_PRE_CHN_CABLE:
	    LOAD_STORAGE_DATA(StructTvDataType, PreChn_Cable, ucVal);
	    break;

	case _TV_TOTAL_CHN_CABLE:
	    LOAD_STORAGE_DATA(StructTvDataType, ucTotalChn_Cable, ucVal);
	    break;

	case _TV_COUNTRY_SEL_CABLE:
	    LOAD_STORAGE_DATA(StructTvDataType, CountrySelect_Cable, ucVal);
	    break;

	case _TV_BLUE_SCREEN_AIR:
	    LOAD_STORAGE_DATA(StructTvDataType, BlueScreen_Air, ucVal);
	    break;

	case _TV_CUR_CHN_AIR:
	    LOAD_STORAGE_DATA(StructTvDataType, CurChn_Air, ucVal);
	    break;

	case _TV_PRE_CHN_AIR:
	    LOAD_STORAGE_DATA(StructTvDataType, PreChn_Air, ucVal);
	    break;

	case _TV_TOTAL_CHN_AIR:
	    LOAD_STORAGE_DATA(StructTvDataType, ucTotalChn_Air, ucVal);
	    break;

	case _TV_COUNTRY_SEL_AIR:
	    LOAD_STORAGE_DATA(StructTvDataType, CountrySelect_Air, ucVal);
	    break;
	    

//==============================================================================
//Tim 0323
//==============================================================================
	case _TV_NTSC_SYSTEM:
    	LOAD_STORAGE_DATA(StructTvDataType, NtscType, ucVal);
    	break;

	case _TV_NTSC_FREE:
	    LOAD_STORAGE_DATA(StructTvDataType, FreeType, ucVal);
	    break;

//==============================================================================
    default:
        ucVal = 0;
        break;
    }
    return ucVal;
}

void ui_set_tv_data(UINT8 id, UINT8 ucVal)
{
    UINT8 TableType,FreeType;

	if(id>=_BLUE_SCREEN_TV)
	{
	    LOAD_STORAGE_DATA(StructTvDataType, NtscType, TableType);
	    LOAD_STORAGE_DATA(StructTvDataType, FreeType, FreeType);
        if(TableType)//Table
		{
			if(FreeType == 0)//Cable	
                id = id - _BLUE_SCREEN_TV + 5;
			else
				id = id - _BLUE_SCREEN_TV + 10;
		}
		else
		{
			id = id - _BLUE_SCREEN_TV;
		}
	
	}

    switch (id)
    {
    case _TV_BLUE_SCREEN:
        SYNC_STORAGE_DATA(StructTvDataType, BlueScreen, ucVal);
        break;

    case _TV_CUR_CHN:
        SYNC_STORAGE_DATA(StructTvDataType, CurChn, ucVal);
        break;

    case _TV_PRE_CHN:
        SYNC_STORAGE_DATA(StructTvDataType, PreChn, ucVal);
        break;

    case _TV_TOTAL_CHN:
        SYNC_STORAGE_DATA(StructTvDataType, ucTotalChn, ucVal);
        break;

    case _TV_COUNTRY_SEL:
        SYNC_STORAGE_DATA(StructTvDataType, CountrySelect, ucVal);
        break;

//Tim 0330
//==============================================================================
	case _TV_BLUE_SCREEN_CABLE:
	    SYNC_STORAGE_DATA(StructTvDataType, BlueScreen_Cable, ucVal);
	    break;

	case _TV_CUR_CHN_CABLE:
	    SYNC_STORAGE_DATA(StructTvDataType, CurChn_Cable, ucVal);
	    break;

	case _TV_PRE_CHN_CABLE:
	    SYNC_STORAGE_DATA(StructTvDataType, PreChn_Cable, ucVal);
	    break;

	case _TV_TOTAL_CHN_CABLE:
	    SYNC_STORAGE_DATA(StructTvDataType, ucTotalChn_Cable, ucVal);
	    break;

	case _TV_COUNTRY_SEL_CABLE:
	    SYNC_STORAGE_DATA(StructTvDataType, CountrySelect_Cable, ucVal);
	    break;

	case _TV_BLUE_SCREEN_AIR:
	    SYNC_STORAGE_DATA(StructTvDataType, BlueScreen_Air, ucVal);
	    break;

	case _TV_CUR_CHN_AIR:
	    SYNC_STORAGE_DATA(StructTvDataType, CurChn_Air, ucVal);
	    break;

	case _TV_PRE_CHN_AIR:
	    SYNC_STORAGE_DATA(StructTvDataType, PreChn_Air, ucVal);
	    break;

	case _TV_TOTAL_CHN_AIR:
	    SYNC_STORAGE_DATA(StructTvDataType, ucTotalChn_Air, ucVal);
	    break;

	case _TV_COUNTRY_SEL_AIR:
	    SYNC_STORAGE_DATA(StructTvDataType, CountrySelect_Air, ucVal);
	    break;

//==============================================================================
//Tim 0323
//==============================================================================
	case _TV_NTSC_SYSTEM:
    	SYNC_STORAGE_DATA(StructTvDataType, NtscType, ucVal);
    	break;
    	
    case _TV_NTSC_FREE:
        SYNC_STORAGE_DATA(StructTvDataType, FreeType, ucVal);
        break;

//==============================================================================
    default:
        break;
    }
}
//Tim 0330
//==============================================================================
/*UINT8 ui_get_cable_chn_color_system(UINT8 ucIdx)
{
    UINT8 getVal;
    LOAD_STORAGE_ARRAY_DATA(StructTvCableDataType, ucIdx, colorSystem, getVal);
    return getVal;
}
void ui_set_cable_chn_color_system(UINT8 ucIdx, UINT8 colorStd)
{
    LOAD_STORAGE_ARRAY_STRUCT_DATA(StructTvCableDataType, ucIdx, stTempCableChannelData);
    stTempCableChannelData.colorSystem = colorStd;
    SYNC_STORAGE_ARRAY_STRUCT_DATA(StructTvCableDataType, ucIdx, stTempCableChannelData);
}

UINT8 ui_get_air_chn_color_system(UINT8 ucIdx)
{
    UINT8 getVal;
    LOAD_STORAGE_ARRAY_DATA(StructTvAirDataType, ucIdx, colorSystem, getVal);
    return getVal;
}
void ui_set_air_chn_color_system(UINT8 ucIdx, UINT8 colorStd)
{
    LOAD_STORAGE_ARRAY_STRUCT_DATA(StructTvAirDataType, ucIdx, stTempAirChannelData);
    stTempAirChannelData.colorSystem = colorStd;
    SYNC_STORAGE_ARRAY_STRUCT_DATA(StructTvAirDataType, ucIdx, stTempAirChannelData);
}*/

//==============================================================================
UINT8 ui_get_chn_color_system(UINT8 ucIdx)
{

    UINT8 getVal;

    if(COsdCtrlGetNtscFree())//Table
	{
        if(GET_TV_CUR_NtscType() == 0)//Cable
        {
            LOAD_STORAGE_ARRAY_DATA(StructTvCableDataType, ucIdx, colorSystem, getVal);
        
        }
        else
        {
            LOAD_STORAGE_ARRAY_DATA(StructTvAirDataType, ucIdx, colorSystem, getVal);
        
        }
	}
	else
	    LOAD_STORAGE_ARRAY_DATA(StructUserDataChannelSettingsType, ucIdx, colorSystem, getVal);
    return getVal;
}

void ui_set_chn_color_system(UINT8 ucIdx, UINT8 colorStd)
{
	if(COsdCtrlGetNtscFree())//Table
	{
        if(GET_TV_CUR_NtscType() == 0)//Cable
        {
            LOAD_STORAGE_ARRAY_STRUCT_DATA(StructTvCableDataType, ucIdx, stTempCableChannelData);
            stTempCableChannelData.colorSystem = colorStd;
            SYNC_STORAGE_ARRAY_STRUCT_DATA(StructTvCableDataType, ucIdx, stTempCableChannelData);
        
        }
        else
        {
            LOAD_STORAGE_ARRAY_STRUCT_DATA(StructTvAirDataType, ucIdx, stTempAirChannelData);
            stTempAirChannelData.colorSystem = colorStd;
            SYNC_STORAGE_ARRAY_STRUCT_DATA(StructTvAirDataType, ucIdx, stTempAirChannelData);
        
        }

	}
	else
	{
	    LOAD_STORAGE_ARRAY_STRUCT_DATA(StructUserDataChannelSettingsType, ucIdx, stTempChannelData);
	    stTempChannelData.colorSystem = colorStd;
	    SYNC_STORAGE_ARRAY_STRUCT_DATA(StructUserDataChannelSettingsType, ucIdx, stTempChannelData);
	}
}

UINT8 ui_get_chn_sound_system(UINT8 ucIdx)
{
    UINT8 getVal;
	if(COsdCtrlGetNtscFree())//Table
	{
        if(GET_TV_CUR_NtscType() == 0)//Cable
            LOAD_STORAGE_ARRAY_DATA(StructTvCableDataType, ucIdx, soundSystem, getVal);
        else
            LOAD_STORAGE_ARRAY_DATA(StructTvAirDataType, ucIdx, soundSystem, getVal);       
	}
	else
    	LOAD_STORAGE_ARRAY_DATA(StructUserDataChannelSettingsType, ucIdx, soundSystem, getVal);
    return getVal;
}

void ui_set_chn_sound_system(UINT8 ucIdx, UINT8 soundStd)
{

	if(COsdCtrlGetNtscFree())//Table
	{
        if(GET_TV_CUR_NtscType() == 0)//Cable
        {
            LOAD_STORAGE_ARRAY_STRUCT_DATA(StructTvCableDataType, ucIdx, stTempCableChannelData);
            stTempCableChannelData.soundSystem = soundStd;
            SYNC_STORAGE_ARRAY_STRUCT_DATA(StructTvCableDataType, ucIdx, stTempCableChannelData);
        
        }
        else
        {
            LOAD_STORAGE_ARRAY_STRUCT_DATA(StructTvAirDataType, ucIdx, stTempAirChannelData);
            stTempAirChannelData.soundSystem = soundStd;
            SYNC_STORAGE_ARRAY_STRUCT_DATA(StructTvAirDataType, ucIdx, stTempAirChannelData);
        
        }
	}
	else
	{
    	LOAD_STORAGE_ARRAY_STRUCT_DATA(StructUserDataChannelSettingsType, ucIdx, stTempChannelData);
    	stTempChannelData.soundSystem = soundStd;
    	SYNC_STORAGE_ARRAY_STRUCT_DATA(StructUserDataChannelSettingsType, ucIdx, stTempChannelData);
	}
    //SYNC_STORAGE_ARRAY_DATA(StructUserDataChannelSettingsType, ucIdx, soundSystem, soundStd);
}

UINT32 ui_get_chn_freq(UINT8 ucIdx)
{
    UINT32 getVal;
	//RTD_Log(LOGGER_LEVEL,"\n Tim ucIdx = %d\n",(UINT32)ucIdx);
	if(COsdCtrlGetNtscFree())//Table
	{
        if(GET_TV_CUR_NtscType() == 0)//Cable
        {
            LOAD_STORAGE_ARRAY_STRUCT_DATA(StructTvCableDataType, ucIdx, stTempCableChannelData);
            getVal = stTempCableChannelData.freq;
        
        }else
        {
            LOAD_STORAGE_ARRAY_STRUCT_DATA(StructTvAirDataType, ucIdx, stTempAirChannelData);
            getVal = stTempAirChannelData.freq;
        
        }

	}
	else
	{
	    LOAD_STORAGE_ARRAY_STRUCT_DATA(StructUserDataChannelSettingsType, ucIdx, stTempChannelData);
	    getVal = stTempChannelData.freq;
	}
    //LOAD_STORAGE_ARRAY_DATA(StructUserDataChannelSettingsType, ucIdx, freq, getVal);

    if ( getVal < drv_tuner_GetChanelMinFreq())
        getVal = drv_tuner_GetChanelMinFreq();
    if ( getVal > drv_tuner_GetChanelMaxFreq())
        getVal = drv_tuner_GetChanelMaxFreq();

    return getVal;
}

//Tim 0324
//==============================================================================
#if 0
void ui_set_chn_freq(UINT8 ucIdx, UINT32 uwFreq)
{
	#ifdef _ONLY_NTSC_SYSTEM
		if(GET_TV_CUR_NtscType() == 0)//Cable
            SYNC_STORAGE_ARRAY_DATA(StructTvCableDataType, ucIdx, freq, uwFreq);
		else
	        SYNC_STORAGE_ARRAY_DATA(StructTvAirDataType, ucIdx, freq, uwFreq);
		
	#else
		SYNC_STORAGE_ARRAY_DATA(StructUserDataChannelSettingsType, ucIdx, freq, uwFreq);
	#endif
}
#endif

void ui_get_Air_chn_data(UINT8 ucIdx, StructTvAirDataType* pChannelBuf)
{
    LOAD_STORAGE_ARRAY_STRUCT_DATA(StructTvAirDataType, ucIdx, *pChannelBuf);
}
void ui_get_Cable_chn_data(UINT8 ucIdx, StructTvCableDataType* pChannelBuf)
{
    LOAD_STORAGE_ARRAY_STRUCT_DATA(StructTvCableDataType, ucIdx, *pChannelBuf);
}

//==============================================================================
void ui_get_chn_data(UINT8 ucIdx, StructUserDataChannelSettingsType* pChannelBuf)
{
    LOAD_STORAGE_ARRAY_STRUCT_DATA(StructUserDataChannelSettingsType, ucIdx, *pChannelBuf);
}
#if defined(_REALTEK_OSD_USE)

void ui_set_chn_data(UINT8 ucIdx, StructUserDataChannelSettingsType* pChannelBuf)
{
    SYNC_STORAGE_ARRAY_STRUCT_DATA(StructUserDataChannelSettingsType, ucIdx, *pChannelBuf);
}
void ui_get_chn_name(UINT8 ucIdx, UINT8 *pBuf)
{
    UINT8 j;
#ifdef CONFIG_STORAGE_ACCESS_FLASH	// marked by weihao because of china
    LOAD_STORAGE_ARRAY_STRUCT_DATA(StructUserDataChannelSettingsType, ucIdx, stTempChannelData);
#endif
    for (j=0; j<5; j++)
#ifdef CONFIG_STORAGE_ACCESS_FLASH	// marked by weihao because of china
        pBuf[j] = stTempChannelData.name[j];
#else
        pBuf[j] = '-';
#endif
}
void ui_set_chn_name(UINT8 ucIdx, UINT8 *pBuf)
{
#ifdef CONFIG_STORAGE_ACCESS_FLASH	// marked by weihao because of china
    UINT8 j;
    LOAD_STORAGE_ARRAY_STRUCT_DATA(StructUserDataChannelSettingsType, ucIdx, stTempChannelData);
    for (j=0; j<5; j++)
        stTempChannelData.name[j] = pBuf[j];
    SYNC_STORAGE_ARRAY_STRUCT_DATA(StructUserDataChannelSettingsType, ucIdx, stTempChannelData);
    //SYNC_STORAGE_ARRAY_DATA(StructUserDataChannelSettingsType, ucIdx, name, *pBuf);
#endif
}
#endif
UINT8 ui_get_chn_freq_offset(UINT8 ucIdx)
{
    UINT8 getVal;
    
	if(COsdCtrlGetNtscFree())//Table
	{
        if(GET_TV_CUR_NtscType() == 0)//Cable
            LOAD_STORAGE_ARRAY_DATA(StructTvCableDataType, ucIdx, offset, getVal);
        else
            LOAD_STORAGE_ARRAY_DATA(StructTvAirDataType, ucIdx, offset, getVal);
	}
	else
    LOAD_STORAGE_ARRAY_DATA(StructUserDataChannelSettingsType, ucIdx, offset, getVal);
    return getVal;
}

void ui_set_chn_freq_offset(UINT8 ucIdx, UINT8 ucOffset)
{
	if(COsdCtrlGetNtscFree())//Table
	{
        if(GET_TV_CUR_NtscType() == 0)//Cable
        {
            LOAD_STORAGE_ARRAY_STRUCT_DATA(StructTvCableDataType, ucIdx, stTempCableChannelData);
            stTempCableChannelData.offset = ucOffset;
            SYNC_STORAGE_ARRAY_STRUCT_DATA(StructTvCableDataType, ucIdx, stTempCableChannelData);
        }
        else
        {
            LOAD_STORAGE_ARRAY_STRUCT_DATA(StructTvAirDataType, ucIdx, stTempAirChannelData);
            stTempAirChannelData.offset = ucOffset;
            SYNC_STORAGE_ARRAY_STRUCT_DATA(StructTvAirDataType, ucIdx, stTempAirChannelData);
        }

	}
	else
	{
	    LOAD_STORAGE_ARRAY_STRUCT_DATA(StructUserDataChannelSettingsType, ucIdx, stTempChannelData);
	    stTempChannelData.offset = ucOffset;
	    SYNC_STORAGE_ARRAY_STRUCT_DATA(StructUserDataChannelSettingsType, ucIdx, stTempChannelData);
	}
//SYNC_STORAGE_ARRAY_DATA(StructUserDataChannelSettingsType, ucIdx, offset, ucOffset);
}

UINT8 ui_get_chn_status(UINT8 ucIdx)
{
    UINT8 getVal;
	if(COsdCtrlGetNtscFree())//Table
	{
        if(GET_TV_CUR_NtscType() == 0)//Cable
            LOAD_STORAGE_ARRAY_DATA(StructTvCableDataType, ucIdx, status, getVal);
        else
            LOAD_STORAGE_ARRAY_DATA(StructTvAirDataType, ucIdx, status, getVal);
	}
	else
    LOAD_STORAGE_ARRAY_DATA(StructUserDataChannelSettingsType, ucIdx, status, getVal);
	
    return getVal;
}

void ui_set_chn_status(UINT8 ucIdx, UINT8 ucSts)
{
	if(COsdCtrlGetNtscFree())//Table
	{
        if(GET_TV_CUR_NtscType() == 0)//Cable
        {
            LOAD_STORAGE_ARRAY_STRUCT_DATA(StructTvCableDataType, ucIdx, stTempCableChannelData);
            stTempCableChannelData.status = ucSts;
            SYNC_STORAGE_ARRAY_STRUCT_DATA(StructTvCableDataType, ucIdx, stTempCableChannelData);
        }
        else
        {
            LOAD_STORAGE_ARRAY_STRUCT_DATA(StructTvAirDataType, ucIdx, stTempAirChannelData);
            stTempAirChannelData.status = ucSts;
            SYNC_STORAGE_ARRAY_STRUCT_DATA(StructTvAirDataType, ucIdx, stTempAirChannelData);
        }

	}
	else
	{
	    LOAD_STORAGE_ARRAY_STRUCT_DATA(StructUserDataChannelSettingsType, ucIdx, stTempChannelData);
	    stTempChannelData.status = ucSts;
	    SYNC_STORAGE_ARRAY_STRUCT_DATA(StructUserDataChannelSettingsType, ucIdx, stTempChannelData);
	}
//SYNC_STORAGE_ARRAY_DATA(StructUserDataChannelSettingsType, ucIdx, status, ucSts);
}
#endif
/* ***** OSD setting data access API ***** */
UINT8 ui_get_osd_language(void)
{
    UINT8 getVal;
    LOAD_STORAGE_DATA(StructOsdUserDataType, Language, getVal);
    return getVal;
}

void ui_set_osd_language(UINT8 ucLang)
{
    SYNC_STORAGE_DATA(StructOsdUserDataType, Language, ucLang);
}

UINT8 ui_get_osd_timeout(void)
{
    UINT8 getVal;
    LOAD_STORAGE_DATA(StructOsdUserDataType, OsdTimeout, getVal);
    return getVal;
}

void ui_set_osd_timeout(UINT8 ucTimeout)
{
    SYNC_STORAGE_DATA(StructOsdUserDataType, OsdTimeout, ucTimeout);
}

UINT8 ui_get_osd_blend(void)
{
    UINT8 getVal;
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    getVal = _stOsdUserData.OsdBlending;
#else
    LOAD_STORAGE_DATA(StructOsdUserDataType, OsdBlending, getVal);
#endif
    return getVal;
}

void ui_set_osd_blend(UINT8 ucBlend)
{
    SYNC_STORAGE_DATA(StructOsdUserDataType, OsdBlending, ucBlend);
}

UINT8 ui_get_aging_mode(void)
{
    UINT8 getVal;
    LOAD_STORAGE_DATA(StructOsdUserDataType, AgingMode, getVal);
    return getVal;
}
#if 0
void ui_set_aging_mode(UINT8 ucMode)
{
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    _stOsdUserData.AgingMode = ucMode;
#else
    SYNC_STORAGE_DATA(StructOsdUserDataType, AgingMode, ucMode);
#endif
}
#endif

/* ***** color storage access API ***** */

void ui_set_color_temperature_gain(UINT8 source, UINT8 colorTemperature, UINT8 channel, UINT8 gain)
{
    // boundary check
    colorTemperature = colorTemperature % 5;

    switch (source)
    {
    case _SOURCE_TV:
        break;

    case _SOURCE_CVBS:
        colorTemperature += 5;
        break;

    case _SOURCE_SV:
        colorTemperature += 5*2;
        break;
#ifdef CONFIG_SCART_AUTO_SWITCH
    case _SOURCE_SCART:
    case _SOURCE_FULL_SCART:
    case _SOURCE_HALF_SCART:
        colorTemperature += 5*3;
        break;
#endif
    case _SOURCE_YPBPR:
        colorTemperature += 5*4;
        break;

#ifdef CONFIG_ENABLE_HDMI
    case _SOURCE_HDMI:
    case _SOURCE_DVI:
        colorTemperature += 5*5;
        break;
#endif

    case _SOURCE_VGA:
        colorTemperature += 5*6;
        break;

    default:
        return;
    }

    RTD_Log(LOGGER_INFO, "Set color temp =%x, source type = %x, gain= %x\n",(UINT32)colorTemperature,(UINT32)source, (UINT32)gain);

    if ( channel == _R)
        SYNC_STORAGE_ARRAY_DATA(RGB_ColorTemp, colorTemperature, R_Contrast, gain);
    else if ( channel == _G)
        SYNC_STORAGE_ARRAY_DATA(RGB_ColorTemp, colorTemperature, G_Contrast, gain);
    else
        SYNC_STORAGE_ARRAY_DATA(RGB_ColorTemp, colorTemperature, B_Contrast, gain);
}

UINT8 ui_get_color_temperature_gain(UINT8 source, UINT8 colorTemperature, UINT8 channel)
{
    RGB_ColorTemp getstRGBColorTemp;
    // boundary check
    colorTemperature = colorTemperature % 5;

    switch (source)
    {
    case _SOURCE_TV:
        break;

    case _SOURCE_CVBS:
        colorTemperature += 5;
        break;

    case _SOURCE_SV:
        colorTemperature += 5*2;
        break;
#ifdef CONFIG_SCART_AUTO_SWITCH
    case _SOURCE_SCART:
    case _SOURCE_FULL_SCART:
    case _SOURCE_HALF_SCART:
        colorTemperature += 5*3;
        break;
#endif
    case _SOURCE_YPBPR:
        colorTemperature += 5*4;
        break;

#ifdef CONFIG_ENABLE_HDMI
    case _SOURCE_HDMI:
    case _SOURCE_DVI:
        colorTemperature += 5*5;
        break;
#endif

    case _SOURCE_VGA:
        colorTemperature += 5*6;
        break;

    default:
        return 0x80;
    }

    LOAD_STORAGE_ARRAY_STRUCT_DATA(RGB_ColorTemp, colorTemperature, getstRGBColorTemp);
	//RTD_Log(LOGGER_INFO, "Get color temp =%x, source type = %x, gain= %x\n",(UINT32)colorTemperature,(UINT32)source, (UINT32)getstRGBColorTemp.R_Contrast);

    if ( channel == _R)
        return getstRGBColorTemp.R_Contrast;
//			LOAD_STORAGE_ARRAY_DATA(RGB_ColorTemp, colorTemperature, R_Contrast, getVal);
    else if ( channel == _G)
        return getstRGBColorTemp.G_Contrast;
//			LOAD_STORAGE_ARRAY_DATA(RGB_ColorTemp, colorTemperature, G_Contrast, getVal);
    else
        return getstRGBColorTemp.B_Contrast;
//			LOAD_STORAGE_ARRAY_DATA(RGB_ColorTemp, colorTemperature, B_Contrast, getVal);
}


void ui_set_color_temperature_offset(UINT8 source, UINT8 colorTemperature, UINT8 channel, UINT8 offset)
{
    // boundary check
    colorTemperature = colorTemperature % 5;

    switch (source)
    {
    case _SOURCE_TV:
        break;

    case _SOURCE_CVBS:
        colorTemperature += 5;
        break;

    case _SOURCE_SV:
        colorTemperature += 5*2;
        break;
#ifdef CONFIG_SCART_AUTO_SWITCH
    case _SOURCE_SCART:
    case _SOURCE_FULL_SCART:
    case _SOURCE_HALF_SCART:
        colorTemperature += 5*3;
        break;
#endif
    case _SOURCE_YPBPR:
        colorTemperature += 5*4;
        break;

#ifdef CONFIG_ENABLE_HDMI
    case _SOURCE_HDMI:
    case _SOURCE_DVI:
        colorTemperature += 5*5;
        break;
#endif

    case _SOURCE_VGA:
        colorTemperature += 5*6;
        break;

    default:
        return;
    }

    if ( channel == _R)
        SYNC_STORAGE_ARRAY_DATA(RGB_ColorTemp, colorTemperature, R_Brightness, offset);
    else if ( channel == _G)
        SYNC_STORAGE_ARRAY_DATA(RGB_ColorTemp, colorTemperature, G_Brightness, offset);
    else
        SYNC_STORAGE_ARRAY_DATA(RGB_ColorTemp, colorTemperature, B_Brightness, offset);
}

UINT8 ui_get_color_temperature_offset(UINT8 source, UINT8 colorTemperature, UINT8 channel)
{
    UINT8 getVal;
    // boundary check
    colorTemperature = colorTemperature % 5;

    switch (source)
    {
    case _SOURCE_TV:
        break;

    case _SOURCE_CVBS:
        colorTemperature += 5;
        break;

    case _SOURCE_SV:
        colorTemperature += 5*2;
        break;
#ifdef CONFIG_SCART_AUTO_SWITCH
    case _SOURCE_SCART:
    case _SOURCE_FULL_SCART:
    case _SOURCE_HALF_SCART:
        colorTemperature += 5*3;
        break;
#endif
    case _SOURCE_YPBPR:
        colorTemperature += 5*4;
        break;

#ifdef CONFIG_ENABLE_HDMI
    case _SOURCE_HDMI:
    case _SOURCE_DVI:
        colorTemperature += 5*5;
        break;
#endif

    case _SOURCE_VGA:
        colorTemperature += 5*6;
        break;

    default:
        return 0x80;
    }

    if ( channel == _R)
        LOAD_STORAGE_ARRAY_DATA(RGB_ColorTemp, colorTemperature, R_Brightness, getVal);
    else if ( channel == _G)
        LOAD_STORAGE_ARRAY_DATA(RGB_ColorTemp, colorTemperature, G_Brightness, getVal);
    else
        LOAD_STORAGE_ARRAY_DATA(RGB_ColorTemp, colorTemperature, B_Brightness, getVal);

    return getVal;
}

void ui_set_customer_factory_data(UINT8 id, UINT8 val)
{
    switch (id)
    {
    case _WBH_BRI:
        SYNC_STORAGE_DATA(StructCustomerFacDataType, WBH_Bri, val);
        break;

    case _WBH_CONT:
        SYNC_STORAGE_DATA(StructCustomerFacDataType, WBH_Cont, val);
        break;

    case _WBL_BRI:
        SYNC_STORAGE_DATA(StructCustomerFacDataType, WBL_Bri, val);
        break;

    case _WBL_CONT:
        SYNC_STORAGE_DATA(StructCustomerFacDataType, WBL_Cont, val);
        break;

    case _LOGO_TYPE:
        SYNC_STORAGE_DATA(StructCustomerFacDataType, LogoType, val);
        break;

    case _DEFAULT_LANGUAGE:
        SYNC_STORAGE_DATA(StructCustomerFacDataType, DefaultLanguage, val);
        break;

    case _BRI_0:
        SYNC_STORAGE_DATA(StructCustomerFacDataType, Brightness_0, val);
        break;

    case _BRI_50:
        SYNC_STORAGE_DATA(StructCustomerFacDataType, Brightness_50, val);
        break;

    case _BRI_100:
        SYNC_STORAGE_DATA(StructCustomerFacDataType, Brightness_100, val);
        break;

    case _CONT_0:
        SYNC_STORAGE_DATA(StructCustomerFacDataType, Contrast_0, val);
        break;

    case _CONT_50:
        SYNC_STORAGE_DATA(StructCustomerFacDataType, Contrast_50, val);
        break;

    case _CONT_100:
        SYNC_STORAGE_DATA(StructCustomerFacDataType, Contrast_100, val);
        break;

    case _SAT_0:
        SYNC_STORAGE_DATA(StructCustomerFacDataType, Saturation_0, val);
        break;

    case _SAT_50:
        SYNC_STORAGE_DATA(StructCustomerFacDataType, Saturation_50, val);
        break;

    case _SAT_100:
        SYNC_STORAGE_DATA(StructCustomerFacDataType, Saturation_100, val);
        break;

    default:
        break;
    }
}

UINT8 ui_get_customer_factory_data(UINT8 id)
{
    UINT8 getVal;
    switch (id)
    {
    case _WBH_BRI:
        LOAD_STORAGE_DATA(StructCustomerFacDataType, WBH_Bri, getVal);
        break;

    case _WBH_CONT:
        LOAD_STORAGE_DATA(StructCustomerFacDataType, WBH_Cont, getVal);
        break;

    case _WBL_BRI:
        LOAD_STORAGE_DATA(StructCustomerFacDataType, WBL_Bri, getVal);
        break;

    case _WBL_CONT:
        LOAD_STORAGE_DATA(StructCustomerFacDataType, WBL_Cont, getVal);
        break;

    case _LOGO_TYPE:
        LOAD_STORAGE_DATA(StructCustomerFacDataType, LogoType, getVal);
        break;

    case _DEFAULT_LANGUAGE:
        LOAD_STORAGE_DATA(StructCustomerFacDataType, DefaultLanguage, getVal);
        break;

    case _BRI_0:
        LOAD_STORAGE_DATA(StructCustomerFacDataType, Brightness_0, getVal);
        break;

    case _BRI_50:
        LOAD_STORAGE_DATA(StructCustomerFacDataType, Brightness_50, getVal);
        break;

    case _BRI_100:
        LOAD_STORAGE_DATA(StructCustomerFacDataType, Brightness_100, getVal);
        break;

    case _CONT_0:
        LOAD_STORAGE_DATA(StructCustomerFacDataType, Contrast_0, getVal);
        break;

    case _CONT_50:
        LOAD_STORAGE_DATA(StructCustomerFacDataType, Contrast_50, getVal);
        break;

    case _CONT_100:
        LOAD_STORAGE_DATA(StructCustomerFacDataType, Contrast_100, getVal);
        break;

    case _SAT_0:
        LOAD_STORAGE_DATA(StructCustomerFacDataType, Saturation_0, getVal);
        break;

    case _SAT_50:
        LOAD_STORAGE_DATA(StructCustomerFacDataType, Saturation_50, getVal);
        break;

    case _SAT_100:
        LOAD_STORAGE_DATA(StructCustomerFacDataType, Saturation_100, getVal);
        break;

    default:
        getVal = 0;
        break;
    }

    return getVal;
}

void ui_set_input_source(UINT8 src)
{
    SYNC_STORAGE_DATA(StructSystemDataType, InputSource, src);
}
#if 0
UINT8 ui_get_input_source(void)
{
    UINT8 getVal;
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    getVal = _stSystemData.InputSource;
#else
    SYNC_STORAGE_DATA(StructSystemDataType, InputSource, getVal);
#endif
    return getVal;
}
#endif

void ui_set_picture_mode(UINT8 mode)
{
//	RTD_Log(LOGGER_INFO, "sync picture mode:%x\n", (UINT32)mode);
    SYNC_STORAGE_DATA(StructSystemDataType, PictureMode, mode);
}

UINT8 ui_get_picture_mode(void)
{
    UINT8 getVal;
    LOAD_STORAGE_DATA(StructSystemDataType, PictureMode, getVal);
//	RTD_Log(LOGGER_INFO, "load picture mode:%x\n", (UINT32)getVal);
    return getVal;
}

void ui_set_picture_mode_val(UINT8 pictureMode, UINT8 channel, UINT8 val)
{
    UINT8 getVal[4];

    pictureMode = pictureMode%4;

    if ( channel == _PICTURE_MODE_BRI)
    {
        LOAD_STORAGE_DATA(StructCustomerFacDataType, PictureMode_Bri, *getVal);
        getVal[pictureMode] = val;
        SYNC_STORAGE_DATA(StructCustomerFacDataType, PictureMode_Bri, *getVal);
    }
    else if ( channel == _PICTURE_MODE_CONT)
    {
        LOAD_STORAGE_DATA(StructCustomerFacDataType, PictureMode_Cont, *getVal);
        getVal[pictureMode] = val;
        SYNC_STORAGE_DATA(StructCustomerFacDataType, PictureMode_Cont, *getVal);
    }
    else if ( channel == _PICTURE_MODE_SAT)
    {
        LOAD_STORAGE_DATA(StructCustomerFacDataType, PictureMode_Sat, *getVal);
        getVal[pictureMode] = val;
        SYNC_STORAGE_DATA(StructCustomerFacDataType, PictureMode_Sat, *getVal);
    }
    else if ( channel == _PICTURE_MODE_HUE)
    {
        LOAD_STORAGE_DATA(StructCustomerFacDataType, PictureMode_Hue, *getVal);
        getVal[pictureMode] = val;
        SYNC_STORAGE_DATA(StructCustomerFacDataType, PictureMode_Hue, *getVal);
    }
    else
    {
        LOAD_STORAGE_DATA(StructCustomerFacDataType, PictureMode_Sharpness, *getVal);
        getVal[pictureMode] = val;
        SYNC_STORAGE_DATA(StructCustomerFacDataType, PictureMode_Sharpness, *getVal);
    }

//	RTD_Log(LOGGER_INFO, "sync picture mode :%x, channel = %x, val = %x\n", (UINT32)pictureMode, (UINT32)channel, (UINT32)getVal);
}

UINT8 ui_get_picture_mode_val(UINT8 pictureMode, UINT8 channel)
{
    UINT8 getVal[4];

    pictureMode = pictureMode % 4;

    if ( channel == _PICTURE_MODE_BRI)
    {
        LOAD_STORAGE_DATA(StructCustomerFacDataType, PictureMode_Bri, *getVal);
    }
    else if ( channel == _PICTURE_MODE_CONT)
    {
        LOAD_STORAGE_DATA(StructCustomerFacDataType, PictureMode_Cont, *getVal);
    }
    else if ( channel == _PICTURE_MODE_SAT)
    {
        LOAD_STORAGE_DATA(StructCustomerFacDataType, PictureMode_Sat, *getVal);
    }
    else if ( channel == _PICTURE_MODE_HUE)
    {
        LOAD_STORAGE_DATA(StructCustomerFacDataType, PictureMode_Hue, *getVal);
    }
    else
    {
        LOAD_STORAGE_DATA(StructCustomerFacDataType, PictureMode_Sharpness, *getVal);
    }

//	RTD_Log(LOGGER_INFO, "load picture mode :%x, channel = %x, val = %x\n", (UINT32)pictureMode, (UINT32)channel, (UINT32)getVal);
    return getVal[pictureMode];
}

UINT8 x_get_color_src_id(void)
{
    switch (GET_INPUTSOURCE_TYPE())
    {
    case _SOURCE_TV:
        return 0;

    case _SOURCE_CVBS:
        return 1;

    case _SOURCE_SV:
        return 2;
#ifdef CONFIG_SCART_AUTO_SWITCH
    case _SOURCE_SCART:
    case _SOURCE_HALF_SCART:
    case _SOURCE_FULL_SCART:
        return 3;
#endif
    case _SOURCE_YPBPR:
        return 4;

#ifdef CONFIG_ENABLE_HDMI
    case _SOURCE_HDMI:
    case _SOURCE_DVI:
        return 5;
#endif

    case _SOURCE_VGA:
        return 6;

    default:
        return 0;
    }
}

void ui_set_color_data(UINT8 id, UINT8 val)
{
    UINT8 src;

    src = x_get_color_src_id();

    switch (id)
    {
    case _PQ_DATA_BACKLIGHT:
        SYNC_STORAGE_ARRAY_DATA(StructColorDataType, src, BackLight, val);
        break;

    case _PQ_DATA_COLOR_TEMPERATURE:
        SYNC_STORAGE_ARRAY_DATA(StructColorDataType, src, ColorTemperature, val);
        break;

    case _PQ_DATA_BRIGHTNESS:
        SYNC_STORAGE_ARRAY_DATA(StructColorDataType, src, Brightness, val);
        break;

    case _PQ_DATA_CONTRAST:
        SYNC_STORAGE_ARRAY_DATA(StructColorDataType, src, Contrast, val);
        break;

    case _PQ_DATA_SATURATION:
        SYNC_STORAGE_ARRAY_DATA(StructColorDataType, src, Saturation, val);
        break;

    case _PQ_DATA_HUE:
        SYNC_STORAGE_ARRAY_DATA(StructColorDataType, src, Hue, val);
        break;

    case _PQ_DATA_SHARPNESS:
        SYNC_STORAGE_ARRAY_DATA(StructColorDataType, src, Sharpness, val);
        break;

    case _PQ_DATA_INTENSTIY:
        SYNC_STORAGE_ARRAY_DATA(StructColorDataType, src, Intensity, val);
        break;

    case _PQ_DATA_DNR:
        SYNC_STORAGE_ARRAY_DATA(StructColorDataType, src, DNR, val);
        break;

    case _PQ_DATA_DCR:
        SYNC_STORAGE_ARRAY_DATA(StructColorDataType, src, DCR, val);
        break;

    case _PQ_DATA_FAC_DCR:
        ucFacDCR = val;
        break;

    case _PQ_DEMO:
        SYNC_STORAGE_DATA(StructFacModeDataType, PQ_Demo, val);
        break;

    default:
        break;
    }
}

UINT8 ui_get_color_data(UINT8 id)
{
    UINT8 getVal;
    UINT8 src;

    src = x_get_color_src_id();

    switch (id)
    {
    case _PQ_DATA_BACKLIGHT:
        LOAD_STORAGE_ARRAY_DATA(StructColorDataType, src, BackLight, getVal);
        break;

    case _PQ_DATA_COLOR_TEMPERATURE:
        LOAD_STORAGE_ARRAY_DATA(StructColorDataType, src, ColorTemperature, getVal);
        break;

    case _PQ_DATA_BRIGHTNESS:
        LOAD_STORAGE_ARRAY_DATA(StructColorDataType, src, Brightness, getVal);
        break;

    case _PQ_DATA_CONTRAST:
        LOAD_STORAGE_ARRAY_DATA(StructColorDataType, src, Contrast, getVal);
        break;

    case _PQ_DATA_SATURATION:
        LOAD_STORAGE_ARRAY_DATA(StructColorDataType, src, Saturation, getVal);
        break;

    case _PQ_DATA_HUE:
        LOAD_STORAGE_ARRAY_DATA(StructColorDataType, src, Hue, getVal);
        break;

    case _PQ_DATA_SHARPNESS:
        LOAD_STORAGE_ARRAY_DATA(StructColorDataType, src, Sharpness, getVal);
        break;

    case _PQ_DATA_INTENSTIY:
        LOAD_STORAGE_ARRAY_DATA(StructColorDataType, src, Intensity, getVal);
        break;

    case _PQ_DATA_DNR:
        LOAD_STORAGE_ARRAY_DATA(StructColorDataType, src, DNR, getVal);
        break;

    case _PQ_DATA_DCR:
        LOAD_STORAGE_ARRAY_DATA(StructColorDataType, src, DCR, getVal);
        break;

    case _PQ_DATA_FAC_DCR:
        getVal = ucFacDCR;
        break;

    case _PQ_DEMO:
        LOAD_STORAGE_DATA(StructFacModeDataType, PQ_Demo, getVal);
        break;

    default:
        getVal = 0;
        break;
    }
    return getVal;
}


UINT8 ui_get_fac_color_data(UINT8 id)
{
    UINT8 getVal;

    switch (id)
    {
    case _PQ_I_DLTI:
        getVal = stFacColorData.I_DLti;
        break;

    case _PQ_I_DCTI:
        getVal = stFacColorData.I_DCti;
        break;

    case _PQ_GAMMA:
        getVal = stFacColorData.Gamma;
        break;

    case _PQ_S_INDEX:
        getVal = stFacColorData.S_Index;
        break;

    case _PQ_S_LOW:
        getVal = stFacColorData.S_Low;
        break;

    case _PQ_S_HIGH:
        getVal = stFacColorData.S_High;
        break;

    case _PQ_DIGI_FILTER:
        getVal = stFacColorData.Digital_Filter;
        break;

    case _PQ_SUNR_Y:
        getVal = stFacColorData.SUNR_Y;
        break;

    case _PQ_SUNR_C:
        getVal = stFacColorData.SUNR_C;
        break;

    case _PQ_SU_MASK:
        getVal = stFacColorData.SUPK_MASK;
        break;

    case _PQ_DCC_MODE:
        getVal = stFacColorData.DccLevel;
        break;

    case _PQ_D_DLTI:
        getVal = stFacColorData.D_DLti;
        break;

    case _PQ_D_DCTI:
        getVal = stFacColorData.D_DCti;
        break;

    case _PQ_SU_H:
        getVal = stFacColorData.ScaleUpH;
        break;

    case _PQ_SU_V:
        getVal = stFacColorData.ScaleUpV;
        break;

    case _PQ_SU_PK:
        getVal = stFacColorData.ScalePK;
        break;

    case _PQ_VD_HUE:
        getVal = stFacColorData.VD_Hue;
        break;

    case _PQ_VD_SAT:
        getVal = stFacColorData.VD_Sat;
        break;

    case _PQ_VD_Y_CONTRAST:
        getVal = stFacColorData.VD_Y_Contrast;
        break;

    case _PQ_VD_Y_BRIGHTNESS:
        getVal = stFacColorData.VD_Y_Brightness;
        break;

    case _PQ_ICM_TABLE:
        getVal = stFacColorData.ICM_Table;
        break;

    case _PQ_DITHER_BIT:
        getVal = stFacColorData.Dither_Bit;
        break;

    default:
        getVal = 0;
        break;
    }

    return getVal;
}

void ui_set_fac_color_data(UINT8 id, UINT8 val)
{
    switch (id)
    {
    case _PQ_I_DLTI:
        stFacColorData.I_DLti = val;
        break;

    case _PQ_I_DCTI:
        stFacColorData.I_DCti = val;
        break;

    case _PQ_GAMMA:
        stFacColorData.Gamma = val;
        break;

    case _PQ_S_INDEX:
        stFacColorData.S_Index = val;
        break;

    case _PQ_S_LOW:
        stFacColorData.S_Low = val;
        break;

    case _PQ_S_HIGH:
        stFacColorData.S_High = val;
        break;


    case _PQ_DIGI_FILTER:
        stFacColorData.Digital_Filter = val;
        break;

    case _PQ_SUNR_Y:
        stFacColorData.SUNR_Y = val;
        break;

    case _PQ_SUNR_C:
        stFacColorData.SUNR_C = val;
        break;

    case _PQ_SU_MASK:
        stFacColorData.SUPK_MASK = val;
        break;

    case _PQ_DCC_MODE:
        stFacColorData.DccLevel = val;
        break;

    case _PQ_D_DLTI:
        stFacColorData.D_DLti = val;
        break;

    case _PQ_D_DCTI:
        stFacColorData.D_DCti = val;
        break;

    case _PQ_SU_H:
        stFacColorData.ScaleUpH = val;
        break;

    case _PQ_SU_V:
        stFacColorData.ScaleUpV = val;
        break;

    case _PQ_SU_PK:
        stFacColorData.ScalePK = val;
        break;

    case _PQ_VD_HUE:
        stFacColorData.VD_Hue = val;
        break;

    case _PQ_VD_SAT:
        stFacColorData.VD_Sat = val;
        break;

    case _PQ_VD_Y_CONTRAST:
        stFacColorData.VD_Y_Contrast = val;
        break;

    case _PQ_VD_Y_BRIGHTNESS:
        stFacColorData.VD_Y_Brightness = val;
        break;

    case _PQ_ICM_TABLE:
        stFacColorData.ICM_Table = val;
        break;

    case _PQ_DITHER_BIT:
        stFacColorData.Dither_Bit = val;
        break;

    default:
        break;
    }
}

void ui_set_volume_curve(UINT8 level, INT16 ucVal)
{
    INT16 getVal[5];

    level = level % 5;
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    _stFacModeDataType.Volume_Curve[level] = ucVal;
#else
    LOAD_STORAGE_DATA(StructFacModeDataType, Volume_Curve, *getVal);
    getVal[level] = ucVal;
    SYNC_STORAGE_DATA(StructFacModeDataType, Volume_Curve, *getVal);
#endif
}

INT16 ui_get_volume_curve(UINT8 level)
{
    INT16 getVal[5];

    level = level % 5;
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    getVal[0] = _stFacModeDataType.Volume_Curve[level];
    return getVal[0];
#else
    LOAD_STORAGE_DATA(StructFacModeDataType, Volume_Curve, *getVal);
    return getVal[level];
#endif
}

void ui_set_eq_300(UINT8 mode, UINT8 ucVal)
{
    UINT8 getVal[7];

    mode = mode % 7;
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    _stCustomerFactoryDataType.EQ_300[mode] = ucVal;
#else
    LOAD_STORAGE_DATA(StructCustomerFacDataType, EQ_300, *getVal);
    getVal[mode] = ucVal;
    SYNC_STORAGE_DATA(StructCustomerFacDataType, EQ_300, *getVal);
#endif
}

UINT8 ui_get_eq_300(UINT8 mode)
{
    UINT8 getVal[7];

    mode = mode % 7;
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    getVal[0] = _stCustomerFactoryDataType.EQ_300[mode];
    return getVal[0];
#else
    LOAD_STORAGE_DATA(StructCustomerFacDataType, EQ_300, *getVal);
    return getVal[mode];
#endif
}

void ui_set_eq_1k(UINT8 mode, UINT8 ucVal)
{
    UINT8 getVal[7];

    mode = mode % 7;
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    _stCustomerFactoryDataType.EQ_1k[mode] = ucVal;
#else
    LOAD_STORAGE_DATA(StructCustomerFacDataType, EQ_1k, *getVal);
    getVal[mode] = ucVal;
    SYNC_STORAGE_DATA(StructCustomerFacDataType, EQ_1k, *getVal);
#endif
}

UINT8 ui_get_eq_1k(UINT8 mode)
{
    UINT8 getVal[7];

    mode = mode % 7;
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    getVal[0] = _stCustomerFactoryDataType.EQ_1k[mode];
    return getVal[0];
#else
    LOAD_STORAGE_DATA(StructCustomerFacDataType, EQ_1k, *getVal);
    return getVal[mode];
#endif
}

void ui_set_eq_2k(UINT8 mode, UINT8 ucVal)
{
    UINT8 getVal[7];

    mode = mode % 7;
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    _stCustomerFactoryDataType.EQ_2k[mode] = ucVal;
#else
    LOAD_STORAGE_DATA(StructCustomerFacDataType, EQ_2k, *getVal);
    getVal[mode] = ucVal;
    SYNC_STORAGE_DATA(StructCustomerFacDataType, EQ_2k, *getVal);
#endif
}

UINT8 ui_get_eq_2k(UINT8 mode)
{
    UINT8 getVal[7];

    mode = mode % 7;
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    getVal[0] = _stCustomerFactoryDataType.EQ_2k[mode];
    return getVal[0];
#else
    LOAD_STORAGE_DATA(StructCustomerFacDataType, EQ_2k, *getVal);
    return getVal[mode];
#endif
}

void ui_set_eq_3k(UINT8 mode, UINT8 ucVal)
{
    UINT8 getVal[7];

    mode = mode % 7;
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    _stCustomerFactoryDataType.EQ_3k[mode] = ucVal;
#else
    LOAD_STORAGE_DATA(StructCustomerFacDataType, EQ_3k, *getVal);
    getVal[mode] = ucVal;
    SYNC_STORAGE_DATA(StructCustomerFacDataType, EQ_3k, *getVal);
#endif
}

UINT8 ui_get_eq_3k(UINT8 mode)
{
    UINT8 getVal[7];

    mode = mode % 7;
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    getVal[0] = _stCustomerFactoryDataType.EQ_3k[mode];
    return getVal[0];
#else
    LOAD_STORAGE_DATA(StructCustomerFacDataType, EQ_3k, *getVal);
    return getVal[mode];
#endif
}

void ui_set_eq_8k(UINT8 mode, UINT8 ucVal)
{
    UINT8 getVal[7];

    mode = mode % 7;
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    _stCustomerFactoryDataType.EQ_8k[mode] = ucVal;
#else
    LOAD_STORAGE_DATA(StructCustomerFacDataType, EQ_8k, *getVal);
    getVal[mode] = ucVal;
    SYNC_STORAGE_DATA(StructCustomerFacDataType, EQ_8k, *getVal);
#endif
}

UINT8 ui_get_eq_8k(UINT8 mode)
{
    UINT8 getVal[7];

    mode = mode % 7;
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    getVal[0] =_stCustomerFactoryDataType.EQ_8k[mode];
    return getVal[0];
#else
    LOAD_STORAGE_DATA(StructCustomerFacDataType, EQ_8k, *getVal);
    return getVal[mode];
#endif
}

void ui_set_eq_15k(UINT8 mode, UINT8 ucVal)
{
    UINT8 getVal[7];

    mode = mode % 7;
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    _stCustomerFactoryDataType.EQ_15k[mode] = ucVal;
#else
    LOAD_STORAGE_DATA(StructCustomerFacDataType, EQ_15k, *getVal);
    getVal[mode] = ucVal;
    SYNC_STORAGE_DATA(StructCustomerFacDataType, EQ_15k, *getVal);
#endif
}

UINT8 ui_get_eq_15k(UINT8 mode)
{
    UINT8 getVal[7];

    mode = mode % 7;
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    getVal[0] =_stCustomerFactoryDataType.EQ_15k[mode];
    return getVal[0];
#else
    LOAD_STORAGE_DATA(StructCustomerFacDataType, EQ_15k, *getVal);
    return getVal[mode];
#endif
}

void ui_set_TV_volume_curve(UINT8 level, INT16 ucVal)
{
    INT16 getVal[5];

    level = level % 5;
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    _stFacModeDataType.TV_Volume_Curve[level] = ucVal;
#else
    LOAD_STORAGE_DATA(StructFacModeDataType, TV_Volume_Curve, *getVal);
    getVal[level] = ucVal;
    SYNC_STORAGE_DATA(StructFacModeDataType, TV_Volume_Curve, *getVal);
#endif
}

INT16 ui_get_TV_volume_curve(UINT8 level)
{
    INT16 getVal[5];

    level = level % 5;
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    getVal[0] =_stFacModeDataType.TV_Volume_Curve[level];
    return getVal[0];
#else
    LOAD_STORAGE_DATA(StructFacModeDataType, TV_Volume_Curve, *getVal);
    return getVal[level];
#endif
}


void ui_set_audio_data(UINT8 id, UINT8 ucVal)
{
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    switch (id)
    {
    case _DATA_EQ_MODE:
        _stAudioDataType.EqMode = ucVal;
        break;

    case _DATA_VOLUME:
        _stAudioDataType.Volumn = ucVal;
        break;
    case _DATA_SPATIAL:
        _stAudioDataType.Spatial = ucVal;
        break;

    case _DATA_BALANCE:
        _stAudioDataType.Balance = ucVal;
        break;

    case _DATA_BASS:
        _stAudioDataType.Bass = ucVal;
        break;

    case _DATA_TREBLE:
        _stAudioDataType.Treble = ucVal;
        break;

    case _DATA_AVL:
        _stAudioDataType.AutoVolume = ucVal;
        break;

    case _DATA_COMPENSATION:
        _stAudioDataType.Compensation = ucVal;
        break;

    default:
        break;
    }
#else
    switch (id)
    {
    case _DATA_EQ_MODE:
        SYNC_STORAGE_DATA(StructAudioDataType, EqMode, ucVal);
        break;

    case _DATA_VOLUME:
        SYNC_STORAGE_DATA(StructAudioDataType, Volumn, ucVal);
        break;
    case _DATA_SPATIAL:
        SYNC_STORAGE_DATA(StructAudioDataType, Spatial, ucVal);
        break;

    case _DATA_BALANCE:
        SYNC_STORAGE_DATA(StructAudioDataType, Balance, ucVal);
        break;

    case _DATA_BASS:
        SYNC_STORAGE_DATA(StructAudioDataType, Bass, ucVal);
        break;

    case _DATA_TREBLE:
        SYNC_STORAGE_DATA(StructAudioDataType, Treble, ucVal);
        break;

    case _DATA_AVL:
        SYNC_STORAGE_DATA(StructAudioDataType, AutoVolume, ucVal);
        break;

    case _DATA_COMPENSATION:
        SYNC_STORAGE_DATA(StructAudioDataType, Compensation, ucVal);
        break;

    case _DATA_MTS:
        SYNC_STORAGE_DATA(StructAudioDataType, MTS, ucVal);
        break;

    default:
        break;
    }
#endif
}

UINT8 ui_get_audio_data(UINT8 id)
{
    UINT8 ucVal;
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    switch (id)
    {
    case _DATA_EQ_MODE:
        ucVal = _stAudioDataType.EqMode;
        break;

    case _DATA_VOLUME:
        ucVal = _stAudioDataType.Volumn;
        break;
    case _DATA_SPATIAL:
        ucVal = _stAudioDataType.Spatial;
        break;

    case _DATA_BALANCE:
        ucVal = _stAudioDataType.Balance;
        break;

    case _DATA_BASS:
        ucVal = _stAudioDataType.Bass;
        break;

    case _DATA_TREBLE:
        ucVal = _stAudioDataType.Treble;
        break;

    case _DATA_AVL:
        ucVal = _stAudioDataType.AutoVolume;
        break;

    case _DATA_COMPENSATION:
        ucVal = _stAudioDataType.Compensation;
        break;

    default:
        ucVal = 0;
        break;
    }
#else
    switch (id)
    {
    case _DATA_EQ_MODE:
        LOAD_STORAGE_DATA(StructAudioDataType, EqMode, ucVal);
        break;

    case _DATA_VOLUME:
        LOAD_STORAGE_DATA(StructAudioDataType, Volumn, ucVal);
        break;

    case _DATA_SPATIAL:
        LOAD_STORAGE_DATA(StructAudioDataType, Spatial, ucVal);
        break;

    case _DATA_BALANCE:
        LOAD_STORAGE_DATA(StructAudioDataType, Balance, ucVal);
        break;

    case _DATA_BASS:
        LOAD_STORAGE_DATA(StructAudioDataType, Bass, ucVal);
        break;

    case _DATA_TREBLE:
        LOAD_STORAGE_DATA(StructAudioDataType, Treble, ucVal);
        break;

    case _DATA_AVL:
        LOAD_STORAGE_DATA(StructAudioDataType, AutoVolume, ucVal);
        break;

    case _DATA_COMPENSATION:
        LOAD_STORAGE_DATA(StructAudioDataType, Compensation, ucVal);
        break;

    case _DATA_MTS:
        LOAD_STORAGE_DATA(StructAudioDataType, MTS, ucVal);
        break;

    default:
        ucVal = 0;
        break;
    }
#endif
    return ucVal;
}

//#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
#include "sparrow\spi_dma.h"

void ui_get_VGA_ADCGain(ADCGainOffset1* ptADCGainOffset)
{
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    ptADCGainOffset->Gain_R = _stADCGainOffset1.Gain_R;
    ptADCGainOffset->Gain_G = _stADCGainOffset1.Gain_G;
    ptADCGainOffset->Gain_B = _stADCGainOffset1.Gain_B;
    ptADCGainOffset->Offset_R =  _stADCGainOffset1.Offset_R ;
    ptADCGainOffset->Offset_G = _stADCGainOffset1.Offset_G;
    ptADCGainOffset->Offset_B = _stADCGainOffset1.Offset_B;
#else
    LOAD_STORAGE_STRUCT_DATA(ADCGainOffset1, *ptADCGainOffset);
#endif
}


void ui_set_VGA_ADCGain(ADCGainOffset1* ptADCGainOffset)
{
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    _stADCGainOffset1.Gain_R = ptADCGainOffset->Gain_R;
    _stADCGainOffset1.Gain_G = ptADCGainOffset->Gain_G;
    _stADCGainOffset1.Gain_B = ptADCGainOffset->Gain_B;
    _stADCGainOffset1.Offset_R = ptADCGainOffset->Offset_R;
    _stADCGainOffset1.Offset_G = ptADCGainOffset->Offset_G;
    _stADCGainOffset1.Offset_B = ptADCGainOffset->Offset_B;
#else
    SYNC_STORAGE_STRUCT_DATA(ADCGainOffset1, *ptADCGainOffset);
#endif
}


void ui_get_YPbPr_ADCGain(ADCGainOffset* ptADCGainOffset)
{
    UINT8 index;
    index = info->ucMode_Curr-_MODE_480I;
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    ptADCGainOffset->Gain_R = _stADCGainOffset[index].Gain_R;
    ptADCGainOffset->Gain_G = _stADCGainOffset[index].Gain_G;
    ptADCGainOffset->Gain_B = _stADCGainOffset[index].Gain_B;
    ptADCGainOffset->Offset_R =  _stADCGainOffset[index].Offset_R ;
    ptADCGainOffset->Offset_G = _stADCGainOffset[index].Offset_G;
    ptADCGainOffset->Offset_B = _stADCGainOffset[index].Offset_B;
#else
    LOAD_STORAGE_ARRAY_STRUCT_DATA(ADCGainOffset, index, *ptADCGainOffset);
#endif
}

void ui_set_YPbPr_ADCGain(ADCGainOffset* ptADCGainOffset)
{
    UINT8 index;
    index = info->ucMode_Curr-_MODE_480I;
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    _stADCGainOffset[index].Gain_R = ptADCGainOffset->Gain_R;
    _stADCGainOffset[index].Gain_G = ptADCGainOffset->Gain_G;
    _stADCGainOffset[index].Gain_B = ptADCGainOffset->Gain_B;
    _stADCGainOffset[index].Offset_R = ptADCGainOffset->Offset_R;
    _stADCGainOffset[index].Offset_G = ptADCGainOffset->Offset_G;
    _stADCGainOffset[index].Offset_B = ptADCGainOffset->Offset_B;
#else
    SYNC_STORAGE_ARRAY_STRUCT_DATA(ADCGainOffset, index, *ptADCGainOffset);
#endif
}
#if defined(CONFIG_SCART_AUTO_SWITCH)

void ui_get_SCART_ADCGain(ADCGainOffset2* ptADCGainOffset)
{
    UINT8 index;
    index = info->ucMode_Curr -_MODE_480I;
    LOAD_STORAGE_ARRAY_STRUCT_DATA(ADCGainOffset2, index, *ptADCGainOffset);
}


void ui_set_SCART_ADCGain(ADCGainOffset2* ptADCGainOffset)
{
    UINT8 index;
    index = info->ucMode_Curr-_MODE_480I;
    SYNC_STORAGE_ARRAY_STRUCT_DATA(ADCGainOffset2, index, *ptADCGainOffset);
}
#endif

void ui_get_VGA_info(UINT8 ucIdx)
{
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    stVGAModeCurrInfo.Status = _stVGAScreenModeInfo[ucIdx].Status;
    stVGAModeCurrInfo.H_Position = _stVGAScreenModeInfo[ucIdx].H_Position;
    stVGAModeCurrInfo.V_Position = _stVGAScreenModeInfo[ucIdx].V_Position;
    stVGAModeCurrInfo.Clock = _stVGAScreenModeInfo[ucIdx].Clock;
    stVGAModeCurrInfo.Phase = _stVGAScreenModeInfo[ucIdx].Phase;
    stVGAModeCurrInfo.H_Size = _stVGAScreenModeInfo[ucIdx].H_Size;
    stVGAModeCurrInfo.V_Size = _stVGAScreenModeInfo[ucIdx].V_Size;
    stVGAModeCurrInfo.Display_Range = _stVGAScreenModeInfo[ucIdx].Display_Range;
#else
    if (ucIdx < 100)
        if (ucIdx > _VGA_MODE_ALL)
            if (ucIdx < _YPBPR_MODE_ALL)
                LOAD_STORAGE_ARRAY_STRUCT_DATA(StructVGAScreenModeInfo, ucIdx - 1, stVGAModeCurrInfo);
#endif
}

//Hardy20091203 for booting to AC-off power status before
void ui_set_AcOff_powerStatus(UINT8 mode)
{
//	RTD_Log(LOGGER_INFO, "sync AcOffPowerStatus:%x\n", (UINT32)mode);
    SYNC_STORAGE_DATA(StructSystemDataType,  AcOffPowerStatus, mode);
}

BOOL ui_is_AcOffPowerStatus_On()
{
    UINT8 mode;
    LOAD_STORAGE_DATA(StructSystemDataType, AcOffPowerStatus, mode);
    return (mode);
}
//end Hardy20091203
void ui_set_VGA_info(UINT8 ucIdx)
{
#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
    _stVGAScreenModeInfo[ucIdx].Status= stVGAModeCurrInfo.Status;
    _stVGAScreenModeInfo[ucIdx].H_Position= stVGAModeCurrInfo.H_Position;
    _stVGAScreenModeInfo[ucIdx].V_Position= stVGAModeCurrInfo.V_Position;
    _stVGAScreenModeInfo[ucIdx].Clock= stVGAModeCurrInfo.Clock;
    _stVGAScreenModeInfo[ucIdx].Phase= stVGAModeCurrInfo.Phase;
    _stVGAScreenModeInfo[ucIdx].H_Size= stVGAModeCurrInfo.H_Size;
    _stVGAScreenModeInfo[ucIdx].V_Size= stVGAModeCurrInfo.V_Size;
    _stVGAScreenModeInfo[ucIdx].Display_Range =stVGAModeCurrInfo.Display_Range;
#else
    if (ucIdx < 100)
        if (ucIdx > _VGA_MODE_ALL)
            if (ucIdx < _YPBPR_MODE_ALL)
                SYNC_STORAGE_ARRAY_STRUCT_DATA(StructVGAScreenModeInfo, ucIdx - 1, stVGAModeCurrInfo);
#endif
}


#else
UINT8 xdata stUISignalSource = 0;

//UINT8 code stOsdUserData[_USER_DATA_COUNT];
//UINT8 code stPqData[_PQ_DATA_COUNT];
//UINT8 code stWbData[5][_WB_DATA_COUNT];
//UINT8 code stFacData[_FAC_DATA_COUNT];
UINT8 xdata stChannelPos[_MAX_PAL_CATV_NUM];

StructUserDataChannelSettingsType *ptUserDataChannelSettings;
//StructUserDataChannelSettingsType code stCableChannelSettings[_MAX_PAL_CATV_NUM];
//UINT8 code stChannelFreqOffsetSettings[_MAX_PAL_CATV_NUM+1];
StructUserDataChannelSettingsType code tDEFAULT_OSDUSER_CHANNELSETTING =
{
    _EMPTY_FREQ,		// freq
//	0x01,				// Chn
    0x00,				// ColorSystem
    0x00,				// SoundSystem
    0x00,				// Status
    {0, 0, 0, 0, 0}		// name[5]
};


#if 0
StructSystemDataType code stSystemDataBlock =
{
    _VERSION_CODE,
//    _INPUTSOURCE,
//    _BOOTSOURCE,
//    _PICTUREMODE,
//    0,
    _PWR_SAVE_MODE,
};
#endif

#if 0
StructOsdUserDataType xdata stOsdUserData =
{
    _English,		// Language
    15,				// OSD Timeout
    0,				// OsdBlending;
    15,				// Volumn;
    0,				// CountrySelect;
    _OFF, 			// ParentalLock;
    0, 				// ucScartFlag;
};
#endif

#if 0
StructTvDataType xdata stTvInfo =
{
    1,		// current channel
    1,		// previous channel
    1,		// total channel numbers
    0,		// FAV page id
};
#endif

UINT8 xdata stSourceZoomMode[] =
{
    1,		// VGA
    1,		// HDMI 0
    1,		// HDMI 1
    1,		// TV
    1, 		// AV
    1,		// SV
    1, 		// Ypbpr
};


/**
 *
 *
 */
UINT8 ui_get_osd_user_data(UINT8 ucIdx)
{
    // 之後要改成直接對 flash 讀寫，不要使用 xdata ...
    //return stOsdUserData[ucIdx];
    ucIdx = 0;
    return 0;
}

void ui_set_osd_user_data(UINT8 ucIdx, UINT8 ucValue)
{
    // 之後要改成直接對 flash 讀寫，不要使用 xdata ...
    //stOsdUserData[ucIdx] = ucValue;
    ucIdx = ucValue;
}


/**
 * PQ Data read/write functions.
 *
 */
UINT8 ui_get_pq_data(UINT8 ucIdx)
{
    //return stPqData[ucIdx];
    ucIdx = 0;
    return 0;
}

void ui_set_pq_data(UINT8 ucIdx, UINT8 ucValue)
{
    // 之後要改成直接對 flash 讀寫，不要使用 xdata ...
    ucIdx = ucValue;
}

/**
 * WB data read/write functions.
 */
UINT8 ui_get_wb_data(UINT8 ucColorTemp, UINT8 ucIdx)
{
    // Color temperature R G B gain 需要改成依照 source 分別儲存
    //return stWbData[ucColorTemp][ucIdx];
    ucColorTemp = ucIdx;
    return 0;
}

void ui_set_wb_data(UINT8 ucColorTemp, UINT8 ucIdx, UINT8 ucValue)
{
    // 之後要改成直接對 flash 讀寫，不要使用 xdata ...
    ucColorTemp = 0;
    ucIdx = ucValue;
}

/**
 * Factory mode data read/write functions.
 */
UINT8 ui_get_fac_data(UINT8 ucIdx)
{
    //return stFacData[ucIdx];
    ucIdx = 0;
    return 0;
}

void ui_set_fac_data(UINT8 ucIdx, UINT8 ucValue)
{
    // 之後要改成直接對 flash 讀寫，不要使用 xdata ...
    ucIdx = ucValue;
}



UINT8 ui_get_channel_number(UINT8 ucIdx)
{
    return stChannelPos[ucIdx];
}

void ui_set_channel_number(UINT8 ucIdx, UINT8 ucNumber)
{
    stChannelPos[ucIdx] = ucNumber;
}

UINT8 ui_get_channel_idx(UINT8 ucChn)
{
    UINT8 j;
    for (j=0; j<_MAX_PAL_CATV_NUM; j++)
    {
        if ( stChannelPos[j] == ucChn)
            return j;
    }

    return 0;
}

/**
 * TV channel data control functions.
 *
 */
UINT16 ui_get_channel_freq(UINT8 ucChn)
{
    ucChn = ui_get_channel_idx(ucChn);
    //return stCableChannelSettings[ucChn].freq;
    return 0;
}

void ui_set_channel_freq(UINT8 ucChn, UINT16 uwFreq)
{
    // 之後要改成直接對 flash 讀寫，不要使用 xdata ...
    ucChn = 0;
    uwFreq = 0;
}

UINT8 ui_get_channel_data(UINT8 ucChn, UINT8 ucIdx)
{
    ucChn = ui_get_channel_idx(ucChn);

    if (ucIdx == _DATA_CHN_VIDEO_STD)
        //return stCableChannelSettings[ucChn].colorSystem;
        return 0;
    else if (ucIdx == _DATA_CHN_AUDIO_STD)
        //return stCableChannelSettings[ucChn].soundSystem;
        return 0;
    else if (ucIdx == _DATA_CHN_STATUS)
        //return stCableChannelSettings[ucChn].status;
        return 0;
    else
        return 0;
}

void ui_set_channel_data(UINT8 ucChn, UINT8 ucIdx, UINT8 ucValue)
{
    // 之後要改成直接對 flash 讀寫，不要使用 xdata ...
    ucChn = 0;
    ucIdx = 0;
    ucValue = 0;
}

void ui_get_channel_name(UINT8 ucChn, UINT8* pBuf)
{
    UINT8 j;

    ucChn = ui_get_channel_idx(ucChn);

    for (j=0; j<5; j++)
    {
        //pBuf[j] = stCableChannelSettings[ucChn].name[j];
        pBuf[j] = 0x20;
    }
}

void ui_set_channel_name(UINT8 ucChn, UINT8* pName)
{
    // 之後要改成直接對 flash 讀寫，不要使用 xdata ...
    ucChn = *pName;
}

UINT8 ui_get_channel_freq_offset(UINT8 ucChn)
{
    ucChn = ui_get_channel_idx(ucChn);

    //return (stChannelFreqOffsetSettings[ucChn]&0x7f);
    return 0;
}

void ui_set_channel_freq_offset(UINT8 ucChn, UINT8 ucOffset)
{
#if 0	// 之後要改成直接對 flash 讀寫，不要使用 xdata ...
    stChannelFreqOffsetSettings[ucChn] = ucOffset&0x7f;
#endif
    ucChn = ucOffset;
}

UINT8 ui_get_channel_finetune(UINT8 ucChn)
{
    ucChn = ui_get_channel_idx(ucChn);

    //return ((stChannelFreqOffsetSettings[ucChn]&_BIT7) >> 7);
    return 0;
}

void ui_set_channel_finetune(UINT8 ucChn, UINT8 ucOpt)
{
#if 0	// 之後要改成直接對 flash 讀寫，不要使用 xdata ...
    if (ucOpt)
        stChannelFreqOffsetSettings[ucChn] |= _BIT7;
    else
        stChannelFreqOffsetSettings[ucChn] &= ~_BIT7;
#endif
    ucChn = ucOpt;
}
#endif

