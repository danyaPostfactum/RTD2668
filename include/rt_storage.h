/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2009
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is for rt-storage related functions.
 *
 * @author:		Weihao Lo
 * @date:		06/18/2009
 * @version:		1.0
 * @ingroup:		rt_storage
 */

/**
 * @addtogroup rt_storage
 * @{
 */

#ifndef RT_STORAGE_H
#define RT_STORAGE_H

#include "rtd_types.h"

// Calculate flash address macro
#define GET_FLASH_ADDRESS(page, address)	(((UINT32)page) << 16 | address)

// The definition of storage type
#define STORAGE_TYPE_INSTANCE 					0
#define STORAGE_TYPE_FLASH_PTR					1
#define STORAGE_TYPE_INSTANCE_ARRAY				2
#define STORAGE_TYPE_INSTANCE_PTR_ARRAY			3
#define STORAGE_TYPE_INSTANCE_ELEMENT_ARRAY	3
#define STORAGE_TYPE_UNKNOW						4

typedef struct _STORAGE_INSTANCE
{
    UINT8 type;					// 0: struct, 1:array
    UINT8 flag;					// 0: system, 1:factory
    UINT16 length;				// element size
    UINT16 INSTANCE;			// pointer to xdata
    UINT16 DEFAULT;				// pointer to default in flash
    UINT16 Array_num;			// specify to the number of array or offset
    UINT8  bank_no;				// pointer to default in flash by bank
} STORAGE_INSTANCE;

typedef enum _STORAGE_STORE_TYPE
{
    STORAGE_STORE_SYSTEM,
    STORAGE_STORE_FACTORY,
    STORAGE_STORE_TOTAL
} _STORAGE_STORE_TYPE;

#define DECLARE_STORAGE_INSTANCE(STRUCT_INSTANCE, STRUCT_DEFAULT, BANK_NO)  \
		UINT16 xdata STRUCT_INSTANCE##ADDRESS;				\
		STORAGE_INSTANCE code STRUCT_INSTANCE##TABLE = {		\
			STORAGE_TYPE_INSTANCE,								\
			0,													\
			sizeof(STRUCT_INSTANCE),							\
			(UINT16)&STRUCT_INSTANCE##ADDRESS,				\
			(UINT16)&STRUCT_DEFAULT,							\
			0,													\
			BANK_NO											\
		}

#define DECLARE_STORAGE_ARRAY(ARRAY_NAME, ARRAY_NUMBER, STRUCT_DEFAULT, BANK_NO) \
		UINT16 xdata ARRAY_NAME##ADDRESS;						\
		STORAGE_INSTANCE code ARRAY_NAME##TABLE = {			\
			STORAGE_TYPE_INSTANCE_ARRAY,						\
			0,													\
			sizeof(ARRAY_NAME),									\
			(UINT16)&ARRAY_NAME##ADDRESS,						\
			(UINT16)&STRUCT_DEFAULT,							\
			ARRAY_NUMBER,										\
			BANK_NO											\
		}

#define DECLARE_STORAGE_ARRAY_DEFAULT_ALL(ARRAY_NAME, ARRAY_NUMBER, STRUCT_DEFAULT, BANK_NO) \
		UINT16 xdata ARRAY_NAME##ADDRESS;						\
		STORAGE_INSTANCE code ARRAY_NAME##TABLE = {			\
			STORAGE_TYPE_INSTANCE_ARRAY,						\
			0x10,												\
			sizeof(ARRAY_NAME),									\
			(UINT16)&ARRAY_NAME##ADDRESS,						\
			(UINT16)&STRUCT_DEFAULT,							\
			ARRAY_NUMBER,										\
			BANK_NO											\
		}

#define DECLARE_FACTORY_STORAGE_INSTANCE(STRUCT_INSTANCE, STRUCT_DEFAULT, BANK_NO)  \
		UINT16 xdata STRUCT_INSTANCE##ADDRESS;				\
		STORAGE_INSTANCE code STRUCT_INSTANCE##TABLE = {		\
			STORAGE_TYPE_INSTANCE,								\
			1,													\
			sizeof(STRUCT_INSTANCE),							\
			(UINT16)&STRUCT_INSTANCE##ADDRESS,				\
			(UINT16)&STRUCT_DEFAULT,							\
			0,													\
			BANK_NO											\
		}

#define DECLARE_FACTORY_STORAGE_ARRAY(ARRAY_NAME, ARRAY_NUMBER, STRUCT_DEFAULT, BANK_NO) \
		UINT16 xdata ARRAY_NAME##ADDRESS;						\
		STORAGE_INSTANCE code ARRAY_NAME##TABLE = {			\
			STORAGE_TYPE_INSTANCE_ARRAY,						\
			1,													\
			sizeof(ARRAY_NAME),									\
			(UINT16)&ARRAY_NAME##ADDRESS,						\
			(UINT16)&STRUCT_DEFAULT,									\
			ARRAY_NUMBER,										\
			BANK_NO											\
		}

#define DECLARE_FACTORY_STORAGE_ARRAY_DEFAULT_ALL(ARRAY_NAME, ARRAY_NUMBER, STRUCT_DEFAULT, BANK_NO) \
		UINT16 xdata ARRAY_NAME##ADDRESS;						\
		STORAGE_INSTANCE code ARRAY_NAME##TABLE = {			\
			STORAGE_TYPE_INSTANCE_ARRAY,						\
			0x11,												\
			sizeof(ARRAY_NAME),									\
			(UINT16)&ARRAY_NAME##ADDRESS,						\
			(UINT16)&STRUCT_DEFAULT,							\
			ARRAY_NUMBER,										\
			BANK_NO											\
		}

#define LOAD_STORAGE_DATA(STRUCT_INSTANCE, ELEMENT, ELEMENT_VAR)	\
		flow_storage_LoadData((STORAGE_INSTANCE code*)&STRUCT_INSTANCE##TABLE, (UINT8 xdata*)&ELEMENT_VAR, OFFSET_STRUCT(STRUCT_INSTANCE, ELEMENT), SIZEOF_STRUCT(STRUCT_INSTANCE, ELEMENT))

#define LOAD_STORAGE_DATA_OFFSET(STRUCT_INSTANCE, ELEMENT, OFFSET, LEN, ELEMENT_VAR)	\
		flow_storage_LoadData((STORAGE_INSTANCE code*)&STRUCT_INSTANCE##TABLE, (UINT8 xdata*)&ELEMENT_VAR, OFFSET_STRUCT(STRUCT_INSTANCE, ELEMENT) + OFFSET, LEN)

#define LOAD_STORAGE_STRUCT_DATA(STRUCT_INSTANCE, ELEMENT_VAR)	\
		flow_storage_LoadData((STORAGE_INSTANCE code*)&STRUCT_INSTANCE##TABLE, (UINT8 xdata*)&ELEMENT_VAR, 0, sizeof(STRUCT_INSTANCE))

#define LOAD_STORAGE_ARRAY_DATA(STRUCT_INSTANCE, INDEX, ELEMENT, ELEMENT_VAR)	\
		flow_storage_LoadArrayData((STORAGE_INSTANCE code*)&STRUCT_INSTANCE##TABLE, (UINT8*)&ELEMENT_VAR, INDEX, OFFSET_STRUCT(STRUCT_INSTANCE, ELEMENT), SIZEOF_STRUCT(STRUCT_INSTANCE, ELEMENT))

#define LOAD_STORAGE_ARRAY_STRUCT_DATA(STRUCT_INSTANCE, INDEX, ELEMENT_VAR)	\
		flow_storage_LoadArrayData((STORAGE_INSTANCE code*)&STRUCT_INSTANCE##TABLE, (UINT8*)&ELEMENT_VAR, INDEX, 0, sizeof(STRUCT_INSTANCE))

#define LOAD_STORAGE_ARRAY_STRUCT_DEFAULT_DATA(STRUCT_INSTANCE, INDEX, ELEMENT_VAR)	\
		flow_storage_LoadArrayDefaultData((STORAGE_INSTANCE code*)&STRUCT_INSTANCE##TABLE, (UINT8*)&ELEMENT_VAR, INDEX, 0, sizeof(STRUCT_INSTANCE))

#define SYNC_STORAGE_DATA(STRUCT_INSTANCE, ELEMENT, ELEMENT_VAR)	\
		flow_storage_SaveData((STORAGE_INSTANCE code*)&STRUCT_INSTANCE##TABLE, (UINT8 xdata*)&ELEMENT_VAR, OFFSET_STRUCT(STRUCT_INSTANCE, ELEMENT), SIZEOF_STRUCT(STRUCT_INSTANCE, ELEMENT))

#define SYNC_STORAGE_DATA_OFFSET(STRUCT_INSTANCE, ELEMENT, OFFSET, LEN,  ELEMENT_VAR)	\
		flow_storage_SaveData((STORAGE_INSTANCE code*)&STRUCT_INSTANCE##TABLE, (UINT8 xdata*)&ELEMENT_VAR, OFFSET_STRUCT(STRUCT_INSTANCE, ELEMENT) + OFFSET, LEN)

#define SYNC_STORAGE_STRUCT_DATA(STRUCT_INSTANCE, ELEMENT_VAR)	\
		flow_storage_SaveData((STORAGE_INSTANCE code*)&STRUCT_INSTANCE##TABLE, (UINT8 xdata*)&ELEMENT_VAR, 0, sizeof(STRUCT_INSTANCE))

#define SYNC_STORAGE_ARRAY_DATA(STRUCT_INSTANCE, INDEX, ELEMENT, ELEMENT_VAR)	\
		flow_storage_SaveArrayData((STORAGE_INSTANCE code*)&STRUCT_INSTANCE##TABLE, (UINT8 xdata*)&ELEMENT_VAR, INDEX, OFFSET_STRUCT(STRUCT_INSTANCE, ELEMENT), SIZEOF_STRUCT(STRUCT_INSTANCE, ELEMENT))

#define SYNC_STORAGE_ARRAY_STRUCT_DATA(STRUCT_INSTANCE, INDEX, ELEMENT_VAR)	\
		flow_storage_SaveArrayData((STORAGE_INSTANCE code*)&STRUCT_INSTANCE##TABLE, (UINT8 xdata*)&ELEMENT_VAR, INDEX, 0, sizeof(STRUCT_INSTANCE))

#define SYNC_STORAGE_DEFAULT_DATA(STRUCT_INSTANCE) \
		flow_storage_SyncDataDefault((STORAGE_INSTANCE code*)&STRUCT_INSTANCE##TABLE)

#define GET_STORAGE_TABLE_ADDRESS(STRUCT_INSTANCE) \
	(UINT16)&STRUCT_INSTANCE##TABLE

#define EXTERN_STORAGE_STANCE(STRUCT_INSTANCE) \
		extern UINT16 xdata STRUCT_INSTANCE##ADDRESS;	\
		extern STORAGE_INSTANCE code STRUCT_INSTANCE##TABLE

/*
#define EXTERN_STORAGE_ARRAY(STRUCT_NAME)   \
		extern UINT32 xdata STRUCT_NAME##ADDRESS;	\
		extern STORAGE_INSTANCE code STRUCT_NAME##TABLE
*/

BOOL flow_storage_Init(void);
BOOL CStorageSync(UINT32 flash_addr, UINT32 data_addr);
BOOL CStorageSyncDefault(UINT32 flash_addr, UINT32 data_addr, int len);

//BOOL flow_storage_LoadData(STORAGE_INSTANCE code* ptStorageInstance, UINT16 data_addr, UINT16 Offset, UINT16 size);
BOOL flow_storage_LoadData(STORAGE_INSTANCE code* ptStorageInstance, UINT8 xdata* data_addr, UINT8 offset, UINT8 size);
//BOOL flow_storage_LoadArrayData(STORAGE_INSTANCE code* ptStorageInstance, UINT16 data_addr, UINT16 index, UINT16 offset, UINT16 size);
BOOL flow_storage_LoadArrayData(STORAGE_INSTANCE code* ptStorageInstance, UINT8 xdata* data_addr, UINT16 index, UINT8 offset, UINT8 size);
BOOL flow_storage_LoadArrayDefaultData(STORAGE_INSTANCE code* ptStorageInstance, UINT8 xdata* data_addr, UINT16 index, UINT8 offset, UINT8 size);
BOOL flow_storage_SaveData(STORAGE_INSTANCE code* ptStorageInstance, UINT8 xdata* data_addr, UINT8 Offset, UINT8 size);
BOOL flow_storage_SaveArrayData(STORAGE_INSTANCE code* ptStorageInstance, UINT8 xdata* data_addr, UINT16 index, UINT8 Offset, UINT8 size);

int flow_storage_SystemInstanceSize(void);
BOOL flow_storage_LoadDefault(UINT8 type);
BOOL flow_storage_SyncDataDefault(STORAGE_INSTANCE code* ptStorageInstance);
BOOL flow_storage_system_StartupCheck(void);
void flow_storage_SyncData(void);
BOOL flow_storage_WriteBackup2Flash();
#endif

