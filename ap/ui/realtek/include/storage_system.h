//=================================================================================================
// Copyright (c) 2007 Realtek Semiconductor Corp. All Rights Reserved.
//
// Filename: storage_system.h
// Abstract:
//   Related Definitions of Storage System for Dragon Lite
//
// History:
//	 2007-12-07			cyyang			Initial Version
//=================================================================================================

#ifndef _STORAGE_SYSTEM_H_
#define _STORAGE_SYSTEM_H_


#if 1
#include "sparrow.h"
#include "rt_storage.h"
#include "ui_struct.h"

//==============================================================================
EXTERN_STORAGE_STANCE(StructTvCableDataType);//Tim 0323
EXTERN_STORAGE_STANCE(StructTvAirDataType);//Tim 0323

//==============================================================================
EXTERN_STORAGE_STANCE(StructOsdUserDataType);
EXTERN_STORAGE_STANCE(StructTvDataType);
EXTERN_STORAGE_STANCE(StructAudioDataType);
EXTERN_STORAGE_STANCE(StructColorDataType);
//EXTERN_STORAGE_STANCE(StructFacColorDataType);
EXTERN_STORAGE_STANCE(StructFacModeDataType);
//EXTERN_STORAGE_STANCE(StructScreenModeInfo);
EXTERN_STORAGE_STANCE(StructSystemDataType);
EXTERN_STORAGE_STANCE(StructUserDataChannelSettingsType);
EXTERN_STORAGE_STANCE(StructChannelOffsetType);
EXTERN_STORAGE_STANCE(StructCustomerFacDataType);
EXTERN_STORAGE_STANCE(ADCGainOffset);
EXTERN_STORAGE_STANCE(ADCGainOffset1);
EXTERN_STORAGE_STANCE(ADCGainOffset2);
EXTERN_STORAGE_STANCE(RGB_ColorTemp);
EXTERN_STORAGE_STANCE(StructVGAScreenModeInfo);
EXTERN_STORAGE_STANCE(StructTVChannelMapTable);
#else
//=================================================================================================
#ifndef _DRAGON_H
#include "dragon.h"
#endif

#ifndef _RTD_TYPES_H
#include "rtd_types.h"
#endif

#ifndef _MISC_H_
#include "misc.h"
#endif

//=================================================================================================
extern UINT32	_storage_system_start[];
extern UINT32	_storage_system_size[];

extern UINT32 	_quick_save_table_start[];
extern UINT32	_quick_save_table_size[];

extern UINT32 	_channel_table_start[];
extern UINT32 	_channel_table_backup_start[];
extern UINT32	_channel_table_size[];

// mark by linmc temporately, test for storage data are not covered during buring code, 20080131
//extern UINT32 	_user_info_table0_start[];
//exteern UINT32 	_user_info_table1_start[];

extern UINT32 	_user_info_table_start[];
extern UINT32	_user_info_table_size[];

//=================================================================================================

#define	ST_REGION_SIZE							8192

#define ST_USER_INFO_TABLE_VALID_FLAG			(ST_REGION_SIZE - 4)
#define ST_USER_INFO_TABLE_BACKUP_FLAG			(ST_USER_INFO_TABLE_VALID_FLAG - 4)

#define ST_CHANNEL_TABLE_VALID_FLAG				(ST_REGION_SIZE - 4)
#define ST_CHANNEL_TABLE_BACKUP_FLAG			(ST_USER_INFO_TABLE_VALID_FLAG - 4)

#define ST_INFO_END_INDICATOR_0					0xA5
#define ST_INFO_END_INDICATOR_1					0x5A
#define ST_INFO_QUICK_SAVE_INFO_DEFAULT			0x010101A5

#define ST_INFO_QUICK_SAVE_TABLE_FIELD_NUM		(ST_REGION_SIZE >> 2)

#define ST_INFO_BLANK_FLASH_MASK				0xFFFFFFFF

#define ST_INFO_WORD_DATA_BUFFER_SIZE			64
#define ST_INFO_WORD_COPY_NUM					((ST_REGION_SIZE / ST_INFO_WORD_DATA_BUFFER_SIZE) >> 2)

#define ST_INFO_FLAG_DATA_VALID					0x23792379
#define ST_INFO_FLAG_BACKUP						0x5A5AA5A5

#define ST_CMD_CHANNEL_TABLE_PREPARE			0x00
#define ST_CMD_CHANNEL_TABLE_SETINFO			0x01
#define ST_CMD_CHANNEL_TABLE_GETINFO			0x02
#define ST_CMD_CHANNEL_TABLE_UPDATE				0x03
#define ST_CMD_CHANNEL_TABLE_DONE				0x04
#define ST_CMD_CHANNEL_TABLE_SETDEFAULT_FROM	0x05

#define ST_CMD_USER_INFO_TABLE_PREPARE			0x06
#define ST_CMD_USER_INFO_TABLE_SETINFO			0x07
#define ST_CMD_USER_INFO_TABLE_GETINFO			0x08
#define ST_CMD_VGA_TABLE_SETINFO					0x09
#define ST_CMD_VGA_DEFAULTTABLE_SETINFO			0x0a

#define ST_CMD_QUICK_SAVE_TABLE_GETINFO			0x01
#define ST_CMD_QUICK_SAVE_TABLE_SETINFO			0x02

// mark by linmc temporately, test for storage data are not covered during buring code, 20080131
///////////////////////////////////////////////////////////////////////////////////////////
//Range: 底層 flash 在 read write 的 range 是 0x1e000000 ~ 0x1f000000
//													轉成上層: 0x9e000000 ~ 0x9f000000
//Check flash.nm to see which area is used
///////////////////////////////////////////////////////////////////////////////////////////
//<roni 20080215 remove>remove to here for management
#if 0
//#define FLASH_4K
// justin add address define for 64K
#ifdef CONFIG_FLASH_64KB_SECTOR // 64K Config
#define _user_info_table0_start		0x9e190000	// user info start from 768k
#define _user_info_table0_end		0x9e1a0000	// end - start = 64k
#define _user_info_table1_start		0x9e1b0000	// user info start from 832k
#define _user_info_table1_end		0x9e1c0000	// end - start = 64k
#else
/*
#define _user_info_table0_start	0x9e0c0000	// user info start from 768k
#define _user_info_table0_end		0x9e0c1000	// end - start = 4k
#define _user_info_table1_start	0x9e0c1000	// user info start from 772k
#define _user_info_table1_end		0x9e0c2000	// end - start = 4k
*/
#define _user_info_table0_start	0x9e0c0000	// user info start from 768k
#define _user_info_table0_end		0x9e0c2000	// end - start = 4k
#define _user_info_table1_start	0x9e0c2000	// user info start from 772k
#define _user_info_table1_end		0x9e0c4000	// end - start = 4k
#endif
//<roni 20080215 add>
//#ifdef CONFIG_SCALER_HDCP_IN_FLASH_WRITE_BY_UART
#if 1
#ifdef CONFIG_FLASH_64KB_SECTOR // 64K Config
#define _FIXED_HDCP_KEY_START_ADDRESS	0x9e1f0000  //only 320+5 byte  //justin
#else
//#define _FIXED_HDCP_KEY_START_ADDRESS	0x9e0c2000  //only 320+5 byte
#define _FIXED_HDCP_KEY_START_ADDRESS	0x9e0c4000  //only 320+5 byte
#endif
//_FIXED_HDCP_KEY_END_ADDRESS 0x9e0c2150	0x150 = 336 ; 336-325 = 11 <<reserve
#endif
#endif
//=================================================================================================
typedef union _ST_QUICK_SAVE_INFO
{
    UINT32	dwQuickSaveInfo;
    struct
    {
        UINT32	byInputSource: 8;				//The Indicator of Input Source
        UINT32	byChannelNumber: 8;			//User Channel Information
        UINT32	byAudioVolume: 8;			//User Audio Volume Information
        UINT32	byEndIndicator: 8;				//The Indicator of Quick Save Information Structure

    } Fields;

} ST_QUICK_SAVE_INFO;

typedef struct _ST_SYSTEM_INFO
{
    BOOL	bUserInformationTableIndex;
    UINT32	dwQuickSaveTableIndex;

} ST_SYSTEM_INFO;

typedef struct _ST_USER_CMD_INFO
{
    UINT32	dwFlashOffsetAddress;
    UINT32	dwDataBufferAddress;
    UINT32	dwDataLength;

} ST_USER_CMD_INFO;

//=================================================================================================
void StorageSystemChannelTableLoadDefault(UINT8 erase, UINT8 index);
BOOL	CStorageSystemStartupCheck(void);
BOOL	StorageSystemUpdate(UINT8 byUserCommandType, ST_USER_CMD_INFO *pStUserCmdInfo);
BOOL	StorageSystemQuickSaveInfo(UINT8 byUserCommandType, ST_QUICK_SAVE_INFO *pStQuickSaveInfo);

BOOL	StorageSystemMemoryCopy(BOOL bProcUserInfoTable, BOOL bActiveTable0, BOOL bEraseEnable, BOOL bSignatureEnable, UINT32 dwFlashOffsetAddress, UINT32 dwDataLength);

void __inline CStorageSystemUpdateStructData(void);

#if (CONFIG_CUSTOMER_TPV)
void CStorageSystemUpdateVGATimingData(void);
void CStorageSystemUpdateFactoryData(void);
#endif

void CStorageSystemUpdateToFlash(void);
void CStorageSystemEraseATVChannel(void);
void CStorageSystemGetATVChannel(UINT8 channel, UINT32 addr);
void CStorageSystemSetATVChannel(UINT8 channel, UINT32 addr);
void CStorageSystemUpdateATVChannel(UINT8 channel, UINT32 addr);
void CSystemSetSystemReboot(UINT8 magic_number);
#if (CONFIG_CUSTOMER_TPV)
//----------
// check the current flash access offset index
// 2008.11.08
//----------
UINT32 StorageSystemGetFlashOffset(UINT8 numbers);
void CStorageSystemResetVGATiming(void);
#endif
//=================================================================================================
#endif
#endif //_STOR_SYS_H_

//=================================================================================================
// End of File
