/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2006
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is for rtdaccess related functions.
 *
 * @author 	$Author$
 * @date	$Date$
 * @version	$Revision$
 * @ingroup	rtdaccess
 */

/**
 * @addtogroup rtdaccess
 * @{
 */

#include "storage_system.h"
#include "eepromdefaultcustomer.h"
#include "vip/color.h"
#include "mode_customer.h"

DECLARE_STORAGE_INSTANCE(StructOsdUserDataType, stOsdUserDefaultData, _BANK7);
DECLARE_STORAGE_INSTANCE(StructTvDataType, stTVDataDefaultData, _BANK7);
DECLARE_STORAGE_INSTANCE(StructAudioDataType, stAudioDefaultData, _BANK7);
DECLARE_STORAGE_ARRAY_DEFAULT_ALL(StructColorDataType, 7, stColorDefaultData, _BANK7);

//==============================================================================
DECLARE_STORAGE_ARRAY_DEFAULT_ALL(StructTvCableDataType, 126, stCableData, _BANK7);//Tim 0323
DECLARE_STORAGE_ARRAY_DEFAULT_ALL(StructTvAirDataType,    70, stAirData, _BANK7);//Tim 0323

//==============================================================================

//DECLARE_STORAGE_ARRAY(StructColorDataType, 7, stColorDefaultData, 7);
//DECLARE_FACTORY_STORAGE_INSTANCE(StructFacColorDataType, stFacColorDefaultData);
DECLARE_FACTORY_STORAGE_INSTANCE(StructFacModeDataType, stFacModeDefaultData, _BANK7);
//DECLARE_STORAGE_INSTANCE(StructScreenModeInfo, sizeof(StructScreenModeInfo), stScreenModeInfoDefaultData);
DECLARE_FACTORY_STORAGE_ARRAY_DEFAULT_ALL(ADCGainOffset, 17, stYPbPrGainDefaultData, _BANK7);
DECLARE_FACTORY_STORAGE_ARRAY_DEFAULT_ALL(ADCGainOffset2, 2, stSCARTGainDefaultData, _BANK7);
DECLARE_FACTORY_STORAGE_INSTANCE(ADCGainOffset1, stVGAGainDefaultData, _BANK7);
DECLARE_STORAGE_INSTANCE(StructSystemDataType, stSystemDataDefault, _BANK7);
DECLARE_STORAGE_INSTANCE(StructTVChannelMapTable, stTVChannelMapDefault, _BANK7);

DECLARE_STORAGE_ARRAY(StructUserDataChannelSettingsType, _MAX_PAL_CATV_NUM, tDEFAULT_OSDUSER_CHANNELSETTING, _BANK7);
//DECLARE_STORAGE_ARRAY(StructChannelOffsetType, _MAX_PAL_CATV_NUM, tChannelOffsetDefault);
DECLARE_FACTORY_STORAGE_INSTANCE(StructCustomerFacDataType, stCustomerFacDefaultData, _BANK7);

/* for PQ color temperature setting */
/* 5 types temperature, 7 source */
extern RGB_ColorTemp code stColorTemp[];
DECLARE_FACTORY_STORAGE_ARRAY_DEFAULT_ALL(RGB_ColorTemp, 35, stColorTemp, _BANK2);
DECLARE_STORAGE_ARRAY(StructVGAScreenModeInfo, 100, StructVGAScreenModeInfoDefaultData, _BANK7);

UINT16 code STORAGE_SYSTEM_INSTANCE_MAP[] =
{
    GET_STORAGE_TABLE_ADDRESS(StructOsdUserDataType),
    GET_STORAGE_TABLE_ADDRESS(StructTvDataType),
    GET_STORAGE_TABLE_ADDRESS(StructAudioDataType),
    GET_STORAGE_TABLE_ADDRESS(StructColorDataType),
//	GET_STORAGE_TABLE_ADDRESS(StructFacColorDataType),
//	GET_STORAGE_TABLE_ADDRESS(StructScreenModeInfo),
    GET_STORAGE_TABLE_ADDRESS(StructSystemDataType),
    GET_STORAGE_TABLE_ADDRESS(StructFacModeDataType),
    GET_STORAGE_TABLE_ADDRESS(StructUserDataChannelSettingsType),
//	GET_STORAGE_TABLE_ADDRESS(StructChannelOffsetType),
    GET_STORAGE_TABLE_ADDRESS(StructCustomerFacDataType),
    GET_STORAGE_TABLE_ADDRESS(ADCGainOffset),
    GET_STORAGE_TABLE_ADDRESS(ADCGainOffset1),
    GET_STORAGE_TABLE_ADDRESS(RGB_ColorTemp),
    GET_STORAGE_TABLE_ADDRESS(StructVGAScreenModeInfo),
    GET_STORAGE_TABLE_ADDRESS(StructTVChannelMapTable),
    GET_STORAGE_TABLE_ADDRESS(ADCGainOffset2),
//==============================================================================
    GET_STORAGE_TABLE_ADDRESS(StructTvCableDataType),//Tim 0323
    GET_STORAGE_TABLE_ADDRESS(StructTvAirDataType),//Tim 0323
//==============================================================================
};

int flow_storage_SystemInstanceSize(void)
{
    return (sizeof(STORAGE_SYSTEM_INSTANCE_MAP) / sizeof(UINT16));
}

