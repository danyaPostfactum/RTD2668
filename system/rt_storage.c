
/*============================================================*
 * Copyright (c)      Realtek Semiconductor Corporation, 2005 *
 * All rights reserved.                                       *
 *============================================================*/

/*======================= Description ========================*/
/**
 * @file rt_storage.c
 * To supply a method to store/load variable or structure into flash
 * @author Justin Chung
 * @date 2007/11/20 04:27
 * @version 0.6
 */
#include "rt_storage.h"
#include "rt_storage_parameter.h"
#include "eepromdefaultcustomer.h"
#include "storage_system.h"
#include "rosprintf.h"
#include "flash\spi_flash.h"
#include "flow_struct.h"
#include "mode_customer.h"
#include "spi_dma.h"
#include "flow_conf_struct.h"
#include "sparrow\imem.h"
#include "reg52.h"
#include "timer_event.h"
#include "sparrow_utility.h"
#include "system.h"

/*================================ Definitions ==============================*/

#define STORAGE_SECTOR_TAG_NOTUSED			(0xFF)
#define STORAGE_SECTOR_TAG_USED				(0x3F)
#define STORAGE_SECTOR_TAG_FULL				(0xcF)
#define STORAGE_SECTOR_TAG_NOAVAIL 			(0xF0)
#define STORAGE_SECTOR_TAG_ERASE				(0xF0)
#define STORAGE_SECTOR_TAG_DIRTY				(0xF0)

#define STORAGE_ADDRESS_TAG_NOTUSED			(0xFF)
#define STORAGE_ADDRESS_TAG_USED				(0x3F)
#define STORAGE_ADDRESS_TAG_FULL				(0xCF)
#define STORAGE_ADDRESS_TAG_NOAVAIL			(0xF0)
#define STORAGE_ADDRESS_TAG_ERASE			(0xF0)
#define STORAGE_ADDRESS_TAG_DIRTY			(0xF0)

#define STORAGE_BACKUP_SIZE					256
#define SAME_SECTOR(sector, address)  ((sector & ~(STORAGE_RING_ELEMENT_SIZE - 1)) && (address & ~(STORAGE_RING_ELEMENT_SIZE - 1)))

// The definition for sector
typedef UINT8 RT_STORAGE_SECTORE_TAG;

// The definition for address index
typedef struct _RT_STORAGE_ADDRESS_IDX
{
    UINT8	tag;
    UINT16	storage_address;
} RT_STORAGE_ADDRESS_IDX;

// The definition for storage index
typedef struct _RT_STORAGE_STORAGE_IDX
{
    UINT16	address_idx;
    UINT8	type;
    UINT8	subtype;
    UINT16 	instance;
    UINT16	num_index;
} RT_STORAGE_STORAGE_IDX;

// The definition for rt-storage control block
typedef struct _RT_STORAGE_CONTROL_PARAMETER
{
    UINT16 RingStart;
    UINT16 RingEnd;
    UINT8   Bank_no;
} RT_STORAGE_CONTROL_PARAMETER;

typedef struct _RT_STORAGE_CONTROL_BLOCK
{
    UINT16 StorageLastUpdate;
    UINT16 IndexLastUpdate;
    UINT16 OldestRingPtr;
    UINT16 NewestRingPtr;
} RT_STORAGE_CONTROL_BLOCK;

typedef struct _RT_STORAGE_BACKUP_BLOCK
{
    UINT16 INSTANCE;
    UINT8 status;
    UINT16 num_index;
    UINT8 size;
    UINT8* BackupStorage;
} RT_STORAGE_BACKUP_BLOCK;


/*================================== Variables ==============================*/
RT_STORAGE_CONTROL_BLOCK xdata CTStorage;
RT_STORAGE_CONTROL_PARAMETER code CTStorageRing =
{
    STORAGE_RING_START,						//  UINT32 RingStart;
    (STORAGE_RING_START + (STORAGE_RING_ELEMENT_SIZE * (STORAGE_RING_NUMBER - 1))),						//  UINT32 RingEnd;
    STORAGE_BANK_NO						//  bank no
};

UINT8 xdata BackupStorage[STORAGE_BACKUP_SIZE];
RT_STORAGE_BACKUP_BLOCK xdata stBackupControlBlock;

//#define CStorageFirstRun	(0xfffcUL)
//#define CStorageVersion	(0xfff8UL)
extern UINT16 code STORAGE_SYSTEM_INSTANCE_MAP[];
// UINT16 xdata _stPTRBackup[256]  _at_ 0x8000;
bit CStorageCheckOverflow(UINT16 length);
void CStorageMoveNextSector(void);
void timerevent_table_rt_storage_sync_event(void);
// void drv_spi_flash_Protect(UINT8 enable);

/*============================================================================*/
/**
 * CStorageCheckSameSector
 * check whether included in Sector
 *
 * @param <UINT32>	{ sector }
 * @param <UINT32>	{ Address to check }
 * @return <BOOL>	{ TRUE if same, FALSE otherwise }
 *
 */
BOOL RtStorage_CheckValidAddr(UINT16 nAddress)
{
    if (nAddress < STORAGE_RING_START)
        return _FALSE;

    if (nAddress > ((STORAGE_RING_START - 1) + (STORAGE_RING_NUMBER * STORAGE_RING_ELEMENT_SIZE)))
        return _FALSE;

    return _TRUE;
}
BOOL RtStorage_Write(UINT16 StartAddress, UINT8* sBuf, UINT16 nSize)
{
    UINT32 getVal;
    UINT8 buf_data[8],j;
    UINT16 i;
    if (!RtStorage_CheckValidAddr(StartAddress))
    {
        getVal = StartAddress;
        RTD_Log(LOGGER_FATAL, "Get error write address = %x\n", (UINT32)getVal);
        return _FALSE;
    }

    if (!RtStorage_CheckValidAddr(StartAddress + nSize - 1))
    {
        getVal = StartAddress + nSize;
        RTD_Log(LOGGER_FATAL, "Get error write end address = %x\n", (UINT32)getVal);
        return _FALSE;
    }

    drv_spi_flash_Write(CTStorageRing.Bank_no, StartAddress,  sBuf,  nSize);

    // Double check written data whether correct
    i = nSize;
    while (i > 0)
    {
        if (i > 8)
        {
            drv_spi_flash_Read((UINT16)buf_data, CTStorageRing.Bank_no, StartAddress, 8);
            for (j = 0; j < 8; j++)
            {
                if (buf_data[j] != *sBuf)
                {
                    getVal = StartAddress;
                    RTD_Log(LOGGER_FATAL, "Write Failure address0 = %x\n", (UINT32)getVal);
                    return _FALSE;
                }
                sBuf++;
            }
            StartAddress += 8;
            i -= 8;
        }
        else
        {
            drv_spi_flash_Read((UINT16)buf_data, CTStorageRing.Bank_no, StartAddress, 8);
            for (j = 0; j < i; j++)
            {
                if (buf_data[j] != *sBuf)
                {
                    getVal = StartAddress;
                    RTD_Log(LOGGER_FATAL, "Write Failure address1 = %x\n", (UINT32)getVal);
                    return _FALSE;
                }
                sBuf++;
            }
            i = 0;
        }
    }

    return _TRUE;
}

BOOL RtStorage_Copy(UINT32 srcAddr, UINT32 destAddr, UINT16 count)
{
    UINT8 bankno;
    UINT32 getVal;
    UINT16 nStartAddr;

    bankno = *((UINT8*)&destAddr + 1);
    if (bankno != STORAGE_BANK_NO)
    {
        getVal = bankno;
        RTD_Log(LOGGER_FATAL, "Get error Bankno = %x\n", (UINT32)getVal);
        return _FALSE;
    }

    *(UINT8*)&nStartAddr =  *((UINT8*)&destAddr + 2);
    *((UINT8*)&nStartAddr + 1) =  *((UINT8*)&destAddr + 3);
    if (!RtStorage_CheckValidAddr(nStartAddr))
    {
        getVal = nStartAddr;
        RTD_Log(LOGGER_FATAL, "Get error copy address = %x\n", (UINT32)getVal);
        return _FALSE;
    }

    if (!RtStorage_CheckValidAddr(nStartAddr + count - 1))
    {
        getVal = nStartAddr + count;
        RTD_Log(LOGGER_FATAL, "Get error copy end address = %x\n", (UINT32)getVal);
        return _FALSE;
    }
    drv_spi_flash_copy(srcAddr, destAddr, count);

    return _TRUE;
}

BOOL RtStorage_Erase(UINT16 StartAddress, UINT16 Length)
{
    UINT32 getVal;

    if (!RtStorage_CheckValidAddr(StartAddress))
    {
        getVal = StartAddress;
        RTD_Log(LOGGER_FATAL, "Get error Erase address = %x\n", (UINT32)getVal);
        return _FALSE;
    }

    if (!RtStorage_CheckValidAddr(StartAddress + Length - 1))
    {
        getVal = StartAddress + Length;
        RTD_Log(LOGGER_FATAL, "Get error Erase end address = %x\n", (UINT32)getVal);
        return _FALSE;
    }
    drv_spi_flash_Erase(CTStorageRing.Bank_no, StartAddress, Length);

    return _TRUE;
}

/*============================================================================*/
/**
 * CStorageCheckSameSector
 * check whether included in Sector
 *
 * @param <UINT32>	{ sector }
 * @param <UINT32>	{ Address to check }
 * @return <BOOL>	{ TRUE if same, FALSE otherwise }
 *
 */
BOOL CStorageCheckSameSector(UINT16 sector, UINT16 address)
{
    if ((sector & ~(STORAGE_RING_ELEMENT_SIZE - 1)) == (address & ~(STORAGE_RING_ELEMENT_SIZE - 1)))
        return _TRUE;

    return _FALSE;
}

/*============================================================================*/
/**
 * CStorageMarkSectorTag
 * Marked Sector as TAG_MASK
 *
 * @param <UINT32>	{ p specified the address of  Tag }
 * @param <UINT16>	{ TAG_MASK to be specified to mask }
 * @return 			{ void }
 *
 */
void CStorageMarkSectorTag(UINT16 p, UINT8 TAG_MASK)
{
    RT_STORAGE_SECTORE_TAG TAG;

    TAG = *(RT_STORAGE_SECTORE_TAG code*)p;
    TAG = TAG & ~(TAG & ~TAG_MASK);
    if (p >= CTStorageRing.RingStart)
    {
        if (p <= (CTStorageRing.RingEnd + STORAGE_RING_ELEMENT_SIZE - 1))
        {
//			drv_spi_flash_Write(CTStorageRing.Bank_no, p, (UINT8*)&TAG, sizeof(RT_STORAGE_SECTORE_TAG));
            if (RtStorage_Write(p, (UINT8*)&TAG, sizeof(RT_STORAGE_SECTORE_TAG)))
                return;
        }
    }
    RTD_Log(LOGGER_FATAL, "Get error address = %x, mask = %x in CStorageMarkSectorTag\n", (UINT32)p, (UINT32)TAG_MASK);
}

/*============================================================================*/
/**
 * CStorageCheckSectorTag
 * Check marked flag in sector and return true/false
 *
 * @param <UINT32>	{ Address P }
 * @param <UINT16>	{ TAG_MASK }
 * @return <BOOL>	{ return TRUE if sector marked as TAG_MASK, FALSE otherwise }
 * ok
 */
BOOL CStorageCheckSectorTag(UINT16 p, UINT8 TAG_MASK)
{
    UINT8 TAG;

    TAG = *(UINT8 code*)p;
    TAG = TAG & ~TAG_MASK;
    if (TAG != 0)
        return _FALSE;

    return _TRUE;
}

/*============================================================================*/
/**
 * CStorageStepNextElement
 * Get Next sector in p
 *
 * @param <UINT32>	{ void }
 * @return <UINT32>	{ void }
 *
 */
UINT16 CStorageStepNextSector(UINT16 P)
{
    // Check whether overflow
//	RTD_Log(LOGGER_INFO, "Before CStorageStepNextSector %x\n", (UINT32)P);
    if ((P + STORAGE_RING_ELEMENT_SIZE) < P)
    {
        P = CTStorageRing.RingStart;
    }
    else
    {
        if ((P + STORAGE_RING_ELEMENT_SIZE) > CTStorageRing.RingEnd)
            P = CTStorageRing.RingStart;
        else
            P += STORAGE_RING_ELEMENT_SIZE;
    }

//	RTD_Log(LOGGER_INFO, "After CStorageStepNextSector %x\n", (UINT32)P);
    return P;
}

/*============================================================================*/
/**
 * CStorageStepPrevElement
 * Get Prev sector in p
 *
 * @param <UINT32>	{ void }
 * @return <UINT32>	{ void }
 *
 */
UINT16 CStorageStepPrevSector(UINT16 P)
{
    P -= STORAGE_RING_ELEMENT_SIZE;

    if (P < CTStorageRing.RingStart)
        P = CTStorageRing.RingEnd;

    return P;
}
#if 0
/*============================================================================*/
/**
 * CStorageRemainSector
 * Return remain sector
 *
 * @param <void>			{ void }
 * @return <UINT8>		{ remain number }
 *
 */
UINT8 CStorageRemainSector(void)
{
    UINT16 RemainRing;
    if (CTStorage.OldestRingPtr > CTStorage.NewestRingPtr)
    {
        RemainRing = (CTStorage.OldestRingPtr - CTStorage.NewestRingPtr - STORAGE_RING_ELEMENT_SIZE);
    }
    else
    {
        RemainRing = (CTStorage.NewestRingPtr - CTStorage.OldestRingPtr);
        RemainRing = (CTStorageRing.RingEnd - CTStorageRing.RingStart) - RemainRing;
    }

    RemainRing /= STORAGE_RING_ELEMENT_SIZE;
//	RTD_Log(LOGGER_INFO, "RemainRing = %x\n", (UINT32)RemainRing);
    return RemainRing;
}
#endif

/*============================================================================*/
/**
 * CStorageMarkAddressTag
 * Make address tag to TAG_MASK
 *
 * @param	<UINT32>	{ p Address }
 * @param	<UINT8> 	{ TAG_Mask to be marked }
 * @return	<void> 		{ void }
 *
 */
void CStorageMarkAddressTag(UINT16 p, UINT8 TAG_MASK)
{
    UINT8 tag;
    RT_STORAGE_ADDRESS_IDX code* ptTAG;

    ptTAG = (RT_STORAGE_ADDRESS_IDX code*)p;
    if (p >= CTStorageRing.RingStart)
    {
        ptTAG = (RT_STORAGE_ADDRESS_IDX code*)p;
        tag = ptTAG->tag;
        tag = tag & TAG_MASK;
        if (p <= (CTStorageRing.RingEnd + STORAGE_RING_ELEMENT_SIZE - 1))
        {
            if (RtStorage_Write(p, (UINT8*)&tag, sizeof(UINT8)));
            return;
        }
    }

    RTD_Log(LOGGER_FATAL, "Get error address = %x, mask = %x in CStorageMarkAddressTag\n", (UINT32)p, (UINT32)TAG_MASK);
}

/*============================================================================*/
/**
 * CStorageCheckAddressTag
 * Check whether marked flag in address tag and return true/false
 *
 * @param <UINT32>		{ address TAG }
 * @param <UINT8>		{ TAG_MASK }
 * @return <BOOL>		{ return TRUE if address tag marked as TAG_MASK, FALSE otherwise }
 *
 */
BOOL CStorageCheckAddressTag(UINT16 p, UINT8 TAG_MASK)
{
    RT_STORAGE_ADDRESS_IDX code* TAG;

    TAG = (RT_STORAGE_ADDRESS_IDX code*)p;
    if ((TAG->tag & ~TAG_MASK) != 0)
        return _FALSE;

    return _TRUE;
}

/*============================================================================*/
/**
 * CStorageGetEmptyAddressTag
 * Search avail address tag
 *
 * @param  <UINT32>		{ Search empty address tag and retrun avail address tag }
 * @return <BOOL>		{ TRUE if avail address tag, otherwise FALSE }
 *
 */
BOOL CStorageGetEmptyAddressTag(UINT16* ptIndex)
{
    UINT16 TAG;
    if (ptIndex == NULL)
        return _FALSE;

    TAG = CTStorage.IndexLastUpdate;
//	RTD_Log(LOGGER_INFO, "IndexLastUpdate = %x\n", (UINT32) CTStorage.IndexLastUpdate);
    do
    {
        if (*(UINT8 code*)TAG == 0xff)
            if (*((UINT8 code*)TAG + 1) == 0xff)
                if (*((UINT8 code*)TAG + 2) == 0xff)
                {
                    *ptIndex = TAG;
                    return _TRUE;
                }
        TAG += sizeof(RT_STORAGE_ADDRESS_IDX);
//		RTD_Log(LOGGER_INFO, "TAG = %x\n", (UINT32) TAG);
    }
    while (CTStorage.StorageLastUpdate >= (TAG + sizeof(RT_STORAGE_ADDRESS_IDX)));

    RTD_Log(LOGGER_FATAL, "CStorageGetEmptyAddressTag not found tag\n");

    *ptIndex = 0xffff;

    return _FALSE;
}

/*============================================================================*/
/**
 * CStorageCreateAddressTag
 * Write storage index to Address Tag
 *
 * @param <UINT32>		{ void }
 * @param <UINT32>		{ void }
 * @return <BOOL>		{ void }
 *
 */
BOOL CStorageCreateAddressTag(UINT16 p, UINT16 stAddrIndex)
{
    RT_STORAGE_ADDRESS_IDX xdata stAddressIdxTAG;
    RT_STORAGE_ADDRESS_IDX code* ptAddressIdxTAG;

    if (stAddrIndex >= CTStorageRing.RingStart)
    {
        if (stAddrIndex <= (CTStorageRing.RingEnd + STORAGE_RING_ELEMENT_SIZE - 1))
        {
            ptAddressIdxTAG = (RT_STORAGE_ADDRESS_IDX code*)stAddrIndex;
            if ((ptAddressIdxTAG->tag != 0xff) || (ptAddressIdxTAG->storage_address != 0xffff))
            {
                // Non Empty address index
//				RTD_Log(LOGGER_FATAL, "Get error Tag in CStorageCreateAddressTag\n");
            }
            else
            {
                stAddressIdxTAG.tag = STORAGE_ADDRESS_TAG_USED;
                stAddressIdxTAG.storage_address = p;
                if (RtStorage_Write(stAddrIndex, (UINT8*)&stAddressIdxTAG, sizeof(RT_STORAGE_ADDRESS_IDX)))
                {
                    CTStorage.IndexLastUpdate += sizeof(RT_STORAGE_ADDRESS_IDX);
                    return _TRUE;
                }
            }
        }
    }

    RTD_Log(LOGGER_FATAL, "Get error Tag in CStorageCreateAddressTag %x, stAddrIndex = %x\n", (UINT32)p, (UINT32)stAddrIndex);

    return _FALSE;
}

/*============================================================================*/
/**
 * CStorageFlushOldestSector
 * Initial data structure for rt_storage
 *
 * @param <UINT32>		{ void }
 * @param <UINT32>		{ void }
 * @return <BOOL>		{ void }
 *
 */
void CStorageFlushOldestSector(STORAGE_INSTANCE code* ptStorageInst)
{
    UINT16 num, num1;
    UINT16 dataitem;
    UINT16 count, count1;
    UINT16 element_count;
    UINT16 addrIdxPointer;
    UINT16 addrIdxPointer1;
    UINT16 addrIdxPointer2;
    UINT16 storageIdxPointer1;
    UINT16 code* ptArrayIdx;
    UINT32 srcFlashAddr, dstFlashAddr;
    RT_STORAGE_STORAGE_IDX stStorageIdx;
    RT_STORAGE_STORAGE_IDX code* ptStorageIdx1;
    STORAGE_INSTANCE code* ptStorageMap;
    bit fWriteBackup;
    num = flow_storage_SystemInstanceSize();
    for (count = 0; count < num; count++)
    {
        ptStorageMap = (STORAGE_INSTANCE code*)STORAGE_SYSTEM_INSTANCE_MAP[count];
        if (ptStorageMap->type == STORAGE_TYPE_INSTANCE)
        {
            if (ptStorageInst != ptStorageMap)
            {
                if (*(UINT16 xdata*)ptStorageMap->INSTANCE != 0xffff)
                {
                    if (CStorageCheckSameSector(CTStorage.OldestRingPtr, *(UINT16 xdata*)ptStorageMap->INSTANCE))
                    {

                        flow_timer_Reset_WatchDog();

                        // Get empty tag for address index
                        if (!CStorageGetEmptyAddressTag(&addrIdxPointer))
                        {
                            RTD_Log(LOGGER_INFO, "1.No get Address Tag\n");
                            return;
                        }

                        // Store data contents into storage zone
                        CTStorage.StorageLastUpdate -= ptStorageMap->length;
                        srcFlashAddr = *(UINT16 xdata*)ptStorageMap->INSTANCE;
                        srcFlashAddr += sizeof(RT_STORAGE_STORAGE_IDX);
                        *((UINT8*)&srcFlashAddr + 1) = CTStorageRing.Bank_no;
                        dstFlashAddr = CTStorage.StorageLastUpdate + 1;
                        *((UINT8*)&dstFlashAddr + 1) = CTStorageRing.Bank_no;
//						drv_spi_flash_copy(srcFlashAddr, dstFlashAddr, ptStorageMap->length);
                        if (!RtStorage_Copy(srcFlashAddr, dstFlashAddr, ptStorageMap->length))
                        {
                            RTD_Log(LOGGER_FATAL, "1.No Copy in CStorageFlushOldestSector\n");
                            return;
                        }

                        stStorageIdx.address_idx = addrIdxPointer;
                        stStorageIdx.instance = (UINT16)ptStorageMap;
                        stStorageIdx.subtype = 0;
                        stStorageIdx.num_index = 0;
                        stStorageIdx.type = STORAGE_TYPE_INSTANCE;
                        CTStorage.StorageLastUpdate -= sizeof(RT_STORAGE_STORAGE_IDX);
//						drv_spi_flash_Write(CTStorageRing.Bank_no, CTStorage.StorageLastUpdate + 1, (UINT8*)&stStorageIdx, sizeof(RT_STORAGE_STORAGE_IDX));
                        if (!RtStorage_Write(CTStorage.StorageLastUpdate + 1, (UINT8*)&stStorageIdx, sizeof(RT_STORAGE_STORAGE_IDX)))
                        {
                            RTD_Log(LOGGER_FATAL, "1.No Write in CStorageFlushOldestSector\n");
                            return;
                        }

                        // Store address index in storage zone into index zone
                        CStorageCreateAddressTag(CTStorage.StorageLastUpdate + 1, addrIdxPointer);
                        ptStorageIdx1 = (RT_STORAGE_STORAGE_IDX code*)*(UINT16 xdata*)ptStorageMap->INSTANCE;
                        CStorageMarkAddressTag(ptStorageIdx1->address_idx, STORAGE_ADDRESS_TAG_ERASE);

                        // Store address index in storage zone into variable instance
                        *(UINT16 xdata*)ptStorageMap->INSTANCE = CTStorage.StorageLastUpdate + 1;
                    }
                }
            }
        }
        else if (ptStorageMap->type == STORAGE_TYPE_INSTANCE_ARRAY)
        {
            if (*(UINT16 xdata*)ptStorageMap->INSTANCE != 0xffff)
            {

                flow_timer_Reset_WatchDog();
                fWriteBackup = 0;
#if 0
                if (ptStorageInst == ptStorageMap)
                {
                    if (_stPTRBackup[0] == 0xffff)
                    {
                        _stPTRBackup[0] = ptStorageInst;
                        fWriteBackup = 1;
                    }
                }
#endif
                // Calculate the number of element to store in oldest sector
                element_count = 0;
                ptStorageIdx1 = (RT_STORAGE_STORAGE_IDX code*)*(UINT16 xdata*)ptStorageMap->INSTANCE;
                num1 = ptStorageIdx1->num_index;
                count1 = 0;
                storageIdxPointer1 = *(UINT16 xdata*)ptStorageMap->INSTANCE;
                storageIdxPointer1 += sizeof(RT_STORAGE_STORAGE_IDX);
                while (count1 < num1)
                {
                    ptArrayIdx = (UINT16 code*)storageIdxPointer1;
#if 0
                    if (fWriteBackup)
                        _stPTRBackup[count + 1] = *ptArrayIdx;
#endif
                    if (CStorageCheckSameSector(CTStorage.OldestRingPtr, *ptArrayIdx))
                    {
                        element_count++;
                    }
                    count1++;
                    storageIdxPointer1 += sizeof(UINT16);
                }
//->w				RTD_Log(LOGGER_INFO, "1.Get count to store : %x total : %x\n", (UINT32)element_count, (UINT32)num1);

                // Store array ptr and item in oldest
                count1 = 0;
                storageIdxPointer1 = *(UINT16 xdata*)ptStorageMap->INSTANCE;
                storageIdxPointer1 += sizeof(RT_STORAGE_STORAGE_IDX);

                // Calculate item
                CTStorage.StorageLastUpdate -= (element_count * (ptStorageMap->length + sizeof(RT_STORAGE_STORAGE_IDX)));
                addrIdxPointer1 = CTStorage.StorageLastUpdate + 1;

                // Calculate array ptr
                CTStorage.StorageLastUpdate -= (num1 * sizeof(UINT16));
                addrIdxPointer2 = CTStorage.StorageLastUpdate  + 1;
                while (count1 < num1)
                {
                    ptArrayIdx = (UINT16 code*)storageIdxPointer1;
                    if (CStorageCheckSameSector(CTStorage.OldestRingPtr, *ptArrayIdx))
                    {
                        // Get ptr in the oldest sector
                        ptStorageIdx1 = (RT_STORAGE_STORAGE_IDX code*)*ptArrayIdx;
//						if(((ptStorageInst == ptStorageMap) && (stBackupControlBlock.num_index != ptStorageIdx1->num_index)) || (ptStorageInst != ptStorageMap))  {
                        if (CStorageGetEmptyAddressTag(&addrIdxPointer))
                        {

                            // Copy  data contents into new storage zone
                            srcFlashAddr = *ptArrayIdx;
                            srcFlashAddr += sizeof(RT_STORAGE_STORAGE_IDX);
                            *((UINT8*)&srcFlashAddr + 1) = CTStorageRing.Bank_no;
                            dstFlashAddr = (addrIdxPointer1 + sizeof(RT_STORAGE_STORAGE_IDX));
                            *((UINT8*)&dstFlashAddr + 1) = CTStorageRing.Bank_no;
//								drv_spi_flash_copy(srcFlashAddr, dstFlashAddr, ptStorageMap->length);
                            if (!RtStorage_Copy(srcFlashAddr, dstFlashAddr, ptStorageMap->length))
                            {
                                RTD_Log(LOGGER_FATAL, "2.No Copy in CStorageFlushOldestSector\n");
                                return;
                            }

                            stStorageIdx.address_idx = addrIdxPointer;
                            stStorageIdx.instance = (UINT16)ptStorageMap;
                            stStorageIdx.subtype = 0;
                            stStorageIdx.num_index = ptStorageIdx1->num_index;
                            stStorageIdx.type = STORAGE_TYPE_INSTANCE_PTR_ARRAY;
//								drv_spi_flash_Write(CTStorageRing.Bank_no, addrIdxPointer1, (UINT8*)&stStorageIdx, sizeof(RT_STORAGE_STORAGE_IDX));
                            if (!RtStorage_Write(addrIdxPointer1, (UINT8*)&stStorageIdx, sizeof(RT_STORAGE_STORAGE_IDX)))
                            {
                                RTD_Log(LOGGER_FATAL, "2.No Write in CStorageFlushOldestSector\n");
                                return;
                            }

                            // Store address index in storage zone into index zone
                            CStorageCreateAddressTag(addrIdxPointer1, addrIdxPointer);
                            CStorageMarkAddressTag(ptStorageIdx1->address_idx, STORAGE_ADDRESS_TAG_ERASE);

                        }
                        else
                        {
                            RTD_Log(LOGGER_INFO, "2.No get Address Tag\n");
                        }
                        dataitem = addrIdxPointer1;
                        addrIdxPointer1 += (ptStorageMap->length + sizeof(RT_STORAGE_STORAGE_IDX));
//						}
//						else {
//							dataitem = *ptArrayIdx;
//						}
                    }
                    else
                    {
                        dataitem = *ptArrayIdx;
                    }
//					if(ptStorageInst != ptStorageMap)

//						drv_spi_flash_Write(CTStorageRing.Bank_no, addrIdxPointer2, (UINT8*)&dataitem, sizeof(UINT16));
                    if (!RtStorage_Write(addrIdxPointer2, (UINT8*)&dataitem, sizeof(UINT16)))
                    {
                        RTD_Log(LOGGER_FATAL, "3.No Write in CStorageFlushOldestSector\n");
                        return;
                    }
//					else {
//						_stPTRBackup[count + 1] = dataitem;
//					}

                    count1++;
                    storageIdxPointer1 += sizeof(UINT16);
                    addrIdxPointer2 += sizeof(UINT16);
                }

//				if(ptStorageInst == ptStorageMap)
//					return;

                // Get empty tag for address index
                if (!CStorageGetEmptyAddressTag(&addrIdxPointer))
                {
                    RTD_Log(LOGGER_INFO, "3.No get Address Tag\n");
                    return;
                }

                stStorageIdx.address_idx = addrIdxPointer;
                stStorageIdx.instance = (UINT16)ptStorageMap;
                stStorageIdx.subtype = 0;
                stStorageIdx.num_index = num1;
                stStorageIdx.type = STORAGE_TYPE_INSTANCE_ARRAY;
                CTStorage.StorageLastUpdate -= sizeof(RT_STORAGE_STORAGE_IDX);
//				drv_spi_flash_Write(CTStorageRing.Bank_no, CTStorage.StorageLastUpdate + 1, (UINT8*)&stStorageIdx, sizeof(RT_STORAGE_STORAGE_IDX));
                if (!RtStorage_Write(CTStorage.StorageLastUpdate + 1, (UINT8*)&stStorageIdx, sizeof(RT_STORAGE_STORAGE_IDX)))
                {
                    RTD_Log(LOGGER_FATAL, "4.No Write in CStorageFlushOldestSector\n");
                    return;
                }


                // Store address index in storage zone into index zone
                ptStorageIdx1 = (RT_STORAGE_STORAGE_IDX code*)*(UINT16 xdata*)ptStorageMap->INSTANCE;
                CStorageCreateAddressTag(CTStorage.StorageLastUpdate + 1, addrIdxPointer);
                CStorageMarkAddressTag(ptStorageIdx1->address_idx, STORAGE_ADDRESS_TAG_ERASE);

                // Store address index in storage zone into variable instance
                *(UINT16 xdata*)ptStorageMap->INSTANCE = CTStorage.StorageLastUpdate + 1;

            }
        }
    }
    // Erase oldest sector
//	drv_spi_flash_Erase(CTStorageRing.Bank_no, CTStorage.OldestRingPtr, STORAGE_RING_ELEMENT_SIZE);
    if (!RtStorage_Erase(CTStorage.OldestRingPtr, STORAGE_RING_ELEMENT_SIZE))
    {
        RTD_Log(LOGGER_FATAL, "No Erase in CStorageFlushOldestSector\n");
        return;
    }

    CTStorage.OldestRingPtr = CStorageStepNextSector(CTStorage.OldestRingPtr);
}

/*============================================================================*/
/**
 * flow_storage_LoadDefault(UINT8 type)
 * Load default from code to rt-storage
 *
 * @param <UINT8>		{ 0: for system, 1: for factory }
 * @return <BOOL>		{ _TRUE if success, otherwise _FALSE }
 *
 */
BOOL flow_storage_LoadDefault(UINT8 type)
{
    int num;
    int i;
    STORAGE_INSTANCE code * ptStorageMap;

    // Check flash whether ready is or not
    if (!drv_spi_flash_CheckReady())
        return _FALSE;

    if (type >= STORAGE_STORE_TOTAL)
        return _FALSE;

    num = flow_storage_SystemInstanceSize();
    for (i = 0; i < num; i++)
    {
        ptStorageMap = (STORAGE_INSTANCE code*)STORAGE_SYSTEM_INSTANCE_MAP[i];

        // Check whether system/factory type
        if ((ptStorageMap->flag & 0x0f) != type)
            continue;

        // Store data contents into storaCStorageRemainSectorge zone
        if (*(UINT16 xdata *)ptStorageMap->INSTANCE != 0xffff)
        {
            CStorageMarkAddressTag(*(UINT16*)ptStorageMap->INSTANCE, STORAGE_ADDRESS_TAG_ERASE);
            *(UINT16 xdata *)ptStorageMap->INSTANCE = 0xffff;
        }
    }

    return _TRUE;
}

BOOL CStorageCheckRepeatData(UINT8 bankno, UINT16 flashAddr, UINT8 xdata* data_addr, UINT8 size)
{
    UINT8 rcvBuf[32];
    UINT8 count;
    UINT8 i;
    UINT16 srcAddr;
//	*((UINT8*)&srcAddr + 1) = bankno;
    srcAddr = flashAddr;
    count = size;
    while (count > 0)
    {
        if (count > 32)
        {
            drv_spi_flash_Read((UINT16)rcvBuf, bankno, srcAddr, 32);
            for (i = 0; i  < 32; i++)
            {
                if (*data_addr != rcvBuf[i])
                    return _FALSE;

                data_addr++;
            }
            count -= 32;
        }
        else
        {
            drv_spi_flash_Read((UINT16)rcvBuf, bankno, srcAddr, count);
            for (i = 0; i  < count; i++)
            {
                if (*data_addr != rcvBuf[i])
                    return _FALSE;

                data_addr++;
            }
            count = 0;
        }
    }

    return _TRUE;
}

void CStorageCheckOverflowbySector(STORAGE_INSTANCE code* ptStorageInst, UINT16 len)
{
#if 0
    while ((CTStorage.IndexLastUpdate + sizeof(RT_STORAGE_ADDRESS_IDX)) > (CTStorage.StorageLastUpdate - len))
    {
        // Move to next new sector
        CStorageMoveNextSector();
        CStorageFlushOldestSector(ptStorageInst);

        if ((CTStorage.IndexLastUpdate + sizeof(RT_STORAGE_ADDRESS_IDX)) <= (CTStorage.StorageLastUpdate - len))
        {
            RTD_Log(LOGGER_INFO, "Move next sector ok \n");
            // Add
            return;
        }
        RTD_Log(LOGGER_INFO, "Continue move next sector \n", (UINT32) (CTStorage.IndexLastUpdate & 0xf000));
    }
#else
	UINT16 ptAddr;
	if((CTStorage.IndexLastUpdate + sizeof(RT_STORAGE_ADDRESS_IDX)) >= (CTStorage.StorageLastUpdate - len)) {
		// Move to next new sector
		CStorageMoveNextSector();
		CStorageFlushOldestSector(ptStorageInst);

		if((CTStorage.IndexLastUpdate + sizeof(RT_STORAGE_ADDRESS_IDX)) > (CTStorage.StorageLastUpdate - len)) {
			RTD_Log(LOGGER_INFO, "1. OverFlow \n");
			// Add
		}
	}
//	else if() {	// Check
	else {
		ptAddr = CTStorage.StorageLastUpdate;
		while(len) {
			if(*(UINT8 code*)ptAddr != 0xff) {
				CStorageMoveNextSector();
				CStorageFlushOldestSector(ptStorageInst);
				break;
			}
			len--;
			ptAddr--;
		}
	}
#endif
}

BOOL flow_storage_WriteBackup2Flash(void)
{
    STORAGE_INSTANCE code* ptStorageMap;
    RT_STORAGE_STORAGE_IDX stStorageIdx;
    RT_STORAGE_STORAGE_IDX code* ptStorageIdx;
    RT_STORAGE_STORAGE_IDX code* ptStorageIdx1;
    UINT16 srcPointer;
    UINT16 addrIdxPointer;
    UINT16 storageIdxPointer;
    UINT16 storageIdxPointer1;
    UINT16 storageIdxPointer2;
    UINT16 storageIdxPointer3;
    UINT16 len, len1, count;
    int i,num;
    UINT8 found;

    if (!drv_spi_flash_CheckReady())
        return _FALSE;

    // Check backup block whether empty or not
    if (stBackupControlBlock.INSTANCE == 0xffff)
        return _TRUE;

    // dirty process
    if (stBackupControlBlock.status != 0)
    {
        num = flow_storage_SystemInstanceSize();
        for (i = 0; i < num; i++)
        {
            ptStorageMap = (STORAGE_INSTANCE code*)STORAGE_SYSTEM_INSTANCE_MAP[i];
            if ((UINT16)ptStorageMap == (UINT16)stBackupControlBlock.INSTANCE)
            {

                flow_timer_Reset_WatchDog();

                if (ptStorageMap->type == STORAGE_TYPE_INSTANCE)
                {

                    if (*(UINT16 xdata*)ptStorageMap->INSTANCE == 0xffff)
                    {
                        srcPointer = ptStorageMap->DEFAULT;
                        if (CStorageCheckRepeatData(ptStorageMap->bank_no, srcPointer, stBackupControlBlock.BackupStorage, ptStorageMap->length))
                        {
                            return _TRUE;
                        }
                    }
                    else
                    {
                        // Check whether same with default
                        if (CStorageCheckRepeatData(ptStorageMap->bank_no, ptStorageMap->DEFAULT, stBackupControlBlock.BackupStorage, ptStorageMap->length))
                        {
                            ptStorageIdx1 = (RT_STORAGE_STORAGE_IDX code*)*(UINT16 xdata*)ptStorageMap->INSTANCE;
                            // Marked stored tag as dirty
                            CStorageMarkAddressTag(ptStorageIdx1->address_idx, STORAGE_ADDRESS_TAG_ERASE);
                            *(UINT16 xdata*)ptStorageMap->INSTANCE = 0xffff;
                            return _TRUE;
                        }

                        srcPointer = *((UINT16 xdata *)ptStorageMap->INSTANCE);
                        srcPointer += sizeof(RT_STORAGE_STORAGE_IDX);
                        if (CStorageCheckRepeatData(CTStorageRing.Bank_no, srcPointer, stBackupControlBlock.BackupStorage, ptStorageMap->length))
                        {
                            return _TRUE;
                        }
                    }

                    // Check whether exceed range
                    CStorageCheckOverflowbySector(ptStorageMap, sizeof(RT_STORAGE_STORAGE_IDX) + ptStorageMap->length);

                    // Get empty tag for address index
                    if (!CStorageGetEmptyAddressTag(&addrIdxPointer))
                    {
                        RTD_Log(LOGGER_INFO, "4. GET empty addr\n");
                        return _FALSE;
                    }

                    // Store data contents into storage zone
                    CTStorage.StorageLastUpdate -= ptStorageMap->length;
                    // drv_spi_flash_Write(CTStorageRing.Bank_no, CTStorage.StorageLastUpdate + 1, (UINT8*)stBackupControlBlock.BackupStorage, ptStorageMap->length);
                    if (!RtStorage_Write(CTStorage.StorageLastUpdate + 1, (UINT8*)stBackupControlBlock.BackupStorage, ptStorageMap->length))
                    {
                        RTD_Log(LOGGER_FATAL, "1.No Write in flow_storage_WriteBackup2Flash, lastupdate_addr = %x\n", (UINT32)CTStorage.StorageLastUpdate);
                        return _FALSE;
                    }

                    CTStorage.StorageLastUpdate -= sizeof(RT_STORAGE_STORAGE_IDX);
                    stStorageIdx.address_idx = addrIdxPointer;
                    stStorageIdx.instance = (UINT16)ptStorageMap;
                    stStorageIdx.subtype = 0;
                    stStorageIdx.num_index = 0;
                    stStorageIdx.type = STORAGE_TYPE_INSTANCE;
                    // drv_spi_flash_Write(CTStorageRing.Bank_no, CTStorage.StorageLastUpdate + 1, (UINT8*)&stStorageIdx, sizeof(RT_STORAGE_STORAGE_IDX));
                    if (!RtStorage_Write(CTStorage.StorageLastUpdate + 1, (UINT8*)&stStorageIdx, sizeof(RT_STORAGE_STORAGE_IDX)))
                    {
                        RTD_Log(LOGGER_FATAL, "2.No Write in flow_storage_WriteBackup2Flash\n");
                        return _FALSE;
                    }

                    // Store address index in storage zone into index zone
                    CStorageCreateAddressTag(CTStorage.StorageLastUpdate + 1, addrIdxPointer);

                    if (*(UINT16 xdata*)ptStorageMap->INSTANCE != 0xffff)
                    {
                        ptStorageIdx1 = (RT_STORAGE_STORAGE_IDX code*)*(UINT16 xdata*)ptStorageMap->INSTANCE;
                        if (*(UINT16 xdata*)ptStorageMap->INSTANCE != 0xffff)
                            CStorageMarkAddressTag(ptStorageIdx1->address_idx, STORAGE_ADDRESS_TAG_ERASE);
                    }

                    // Store address index in storage zone into variable instance
                    *(UINT16 xdata*)ptStorageMap->INSTANCE = CTStorage.StorageLastUpdate + 1;
                    ptStorageIdx = (RT_STORAGE_STORAGE_IDX code*)*(UINT16 xdata*)ptStorageMap->INSTANCE;
                    if (ptStorageIdx->type != STORAGE_TYPE_INSTANCE)
                    {
                        RTD_Log(LOGGER_FATAL, "7.WriteBackup2Flash Get error instacnce = %x, index addr = %x\n", (UINT32)ptStorageMap, (UINT32)*(UINT16 xdata*)ptStorageMap->INSTANCE);
                    }
                }
                else if (ptStorageMap->type == STORAGE_TYPE_INSTANCE_ARRAY)
                {
                    if (*(UINT16 xdata*)ptStorageMap->INSTANCE == 0xffff)
                    {
                        // Get ptr from default
                        srcPointer = ptStorageMap->DEFAULT;
                        if (ptStorageMap->flag & 0x10)
                            srcPointer += ptStorageMap->length * stBackupControlBlock.num_index;

                        if (CStorageCheckRepeatData(ptStorageMap->bank_no, srcPointer, stBackupControlBlock.BackupStorage, ptStorageMap->length))
                        {
                            return _TRUE;
                        }

                        found = 0;
                        len = 0;
                        count = 0;
                    }
                    else
                    {
                        srcPointer = *(UINT16 xdata *)ptStorageMap->INSTANCE;
                        ptStorageIdx = (RT_STORAGE_STORAGE_IDX code*)srcPointer;
                        if (ptStorageIdx->num_index == 0xffff)  // Get empty idx
                        {
                            // Get ptr from default
                            srcPointer = ptStorageMap->DEFAULT;
                            if (ptStorageMap->flag & 0x10)
                                srcPointer += ptStorageMap->length * stBackupControlBlock.num_index;

                            if (CStorageCheckRepeatData(ptStorageMap->bank_no, srcPointer, stBackupControlBlock.BackupStorage, ptStorageMap->length))
                            {
                                ptStorageIdx1 = (RT_STORAGE_STORAGE_IDX code*)*(UINT16 xdata*)ptStorageMap->INSTANCE;
                                *(UINT16 xdata*)ptStorageMap->INSTANCE = 0xffff;
                                CStorageMarkAddressTag(ptStorageIdx1->address_idx, STORAGE_ADDRESS_TAG_ERASE);
                                return _TRUE;
                            }
                            found = 0;
                            len = 0;
                            count = 0;
                        }
                        else
                        {
                            // Get the number of storage tag
                            count = ptStorageIdx->num_index;
                            found = 0;
                            i = count;
                            srcPointer += sizeof(RT_STORAGE_STORAGE_IDX);
                            while (i)
                            {

                                // Get ptr
                                ptStorageIdx1 = (RT_STORAGE_STORAGE_IDX code*)*(UINT16 code*)srcPointer;

//->w								RTD_Log(LOGGER_INFO, "4.Write2Backup instance = %x, ptStorageIdx = %x, srcPointer = %x, srcPointer's val = %x\n", (UINT32)ptStorageMap, (UINT32)ptStorageIdx1, (UINT32)srcPointer, (UINT32)*(UINT16 code*)srcPointer);

                                // Get data context
                                if (ptStorageIdx1->num_index == stBackupControlBlock.num_index)
                                {
//									RTD_Log(LOGGER_INFO, "2.found Write2Backup instance = %x, ptStorageIdx = %x, count = %x\n", (UINT32)*(UINT16 xdata *)ptStorageMap->INSTANCE, (UINT32)ptStorageIdx, (UINT32)ptStorageIdx->num_index);
                                    if (CStorageCheckRepeatData(CTStorageRing.Bank_no, srcPointer + sizeof(RT_STORAGE_STORAGE_IDX), stBackupControlBlock.BackupStorage, ptStorageMap->length))
                                    {
                                        return _TRUE;
                                    }
                                    found = 1;
                                    break;
                                }
                                i--;
                                srcPointer += sizeof(UINT16);
                            }

                            if (found)
                            {
                                len = sizeof(RT_STORAGE_STORAGE_IDX) + (2 * ptStorageIdx->num_index);
                            }
                            else
                            {
                                len = sizeof(RT_STORAGE_STORAGE_IDX) + (2 * (ptStorageIdx->num_index + 1));
                            }
                        }
                    }

//					RTD_Log(LOGGER_INFO, "2.Write2Backup instance = %x, found = %x\n", (UINT32)*(UINT16 xdata *)ptStorageMap->INSTANCE, (UINT32)found);
                    // Check whether exceed range

                    len1 = sizeof(RT_STORAGE_STORAGE_IDX) + ptStorageMap->length;
                    CStorageCheckOverflowbySector(ptStorageMap, len1 + len);

                    // Get empty tag for address index
                    if (!CStorageGetEmptyAddressTag(&addrIdxPointer))
                    {
                        RTD_Log(LOGGER_INFO, "5. GET empty addr\n");
                        return _FALSE;
                    }

                    // Check whether INSTANCE be 0xffffffff --> default
//					RTD_Log(LOGGER_INFO, "2.WriteBackup2Flash Get empty index address = %x, StorageLastUpdate = %x\n", (UINT32)addrIdxPointer, (UINT32)CTStorage.StorageLastUpdate);
//					RTD_Log(LOGGER_INFO, "Get empty index address = %x\n", (UINT32)addrIdxPointer);

                    // Store data contents into storage zone
                    CTStorage.StorageLastUpdate -= ptStorageMap->length;
//					RTD_Log(LOGGER_INFO, "Write backupstorage index address = %x, len = %x\n", (UINT32)CTStorage.StorageLastUpdate + 1, (UINT32)ptStorageMap->length);
//					drv_spi_flash_Write(CTStorageRing.Bank_no, CTStorage.StorageLastUpdate + 1, (UINT8*)stBackupControlBlock.BackupStorage, ptStorageMap->length);
                    if (!RtStorage_Write(CTStorage.StorageLastUpdate + 1, (UINT8*)stBackupControlBlock.BackupStorage, ptStorageMap->length))
                    {
                        RTD_Log(LOGGER_FATAL, "3.No Write in flow_storage_WriteBackup2Flash\n");
                        return _FALSE;
                    }


                    stStorageIdx.address_idx = addrIdxPointer;
                    stStorageIdx.instance = (UINT16)ptStorageMap;
                    stStorageIdx.subtype = 0;
                    stStorageIdx.num_index = stBackupControlBlock.num_index;
                    stStorageIdx.type = STORAGE_TYPE_INSTANCE_PTR_ARRAY;
                    CTStorage.StorageLastUpdate -= sizeof(RT_STORAGE_STORAGE_IDX);
//					RTD_Log(LOGGER_INFO, " Write storage index address = %x\n", (UINT32)CTStorage.StorageLastUpdate + 1);
//					drv_spi_flash_Write(CTStorageRing.Bank_no, CTStorage.StorageLastUpdate + 1, (UINT8*)&stStorageIdx, sizeof(RT_STORAGE_STORAGE_IDX));
                    if (!RtStorage_Write(CTStorage.StorageLastUpdate + 1, (UINT8*)&stStorageIdx, sizeof(RT_STORAGE_STORAGE_IDX)))
                    {
                        RTD_Log(LOGGER_FATAL, "4.No Write in flow_storage_WriteBackup2Flash\n");
                        return _FALSE;
                    }

                    // Store address index in storage zone into index zone
//					RTD_Log(LOGGER_INFO, " Write address index address = %x\n", (UINT32)addrIdxPointer);
                    CStorageCreateAddressTag(CTStorage.StorageLastUpdate + 1, addrIdxPointer);

                    storageIdxPointer = CTStorage.StorageLastUpdate + 1;
//					RTD_Log(LOGGER_INFO, " Get Array item = %x\n", (UINT32)storageIdxPointer);
                    // add check whether remain = 0

                    // Get empty tag for address index
                    if (!CStorageGetEmptyAddressTag(&addrIdxPointer))
                    {
                        RTD_Log(LOGGER_INFO, "6. GET empty addr\n");
                        return _FALSE;
                    }

//					RTD_Log(LOGGER_INFO, "Get empty index address = %x\n", (UINT32)addrIdxPointer);
//->w					RTD_Log(LOGGER_INFO, "Get count = %x\n", (UINT32)count);
                    if (count == 0)
                    {
                        CTStorage.StorageLastUpdate -= sizeof(UINT16);
//						drv_spi_flash_Write(CTStorageRing.Bank_no, CTStorage.StorageLastUpdate + 1, (UINT8*)&storageIdxPointer, sizeof(UINT16));
                        if (!RtStorage_Write(CTStorage.StorageLastUpdate + 1, (UINT8*)&storageIdxPointer, sizeof(UINT16)))
                        {
                            RTD_Log(LOGGER_FATAL, "5.No Write in flow_storage_WriteBackup2Flash\n");
                            return _FALSE;
                        }
                    }
                    else
                    {
                        storageIdxPointer2 = *(UINT16 xdata *)ptStorageMap->INSTANCE;
                        storageIdxPointer2 +=  sizeof(RT_STORAGE_STORAGE_IDX);
                        i = 0;
                        CTStorage.StorageLastUpdate -= (sizeof(UINT16) * count);
                        storageIdxPointer3 = CTStorage.StorageLastUpdate + 1;
                        while (i < count)
                        {
                            ptStorageIdx1 = (RT_STORAGE_STORAGE_IDX code*)*(UINT16 code*)storageIdxPointer2;
                            if (ptStorageIdx1->num_index != stBackupControlBlock.num_index)
                            {
                                storageIdxPointer1 = *(UINT16 code*)storageIdxPointer2;
//								drv_spi_flash_Write(CTStorageRing.Bank_no, storageIdxPointer3, (UINT8*)&storageIdxPointer1, sizeof(UINT16));
                                if (!RtStorage_Write(storageIdxPointer3, (UINT8*)&storageIdxPointer1, sizeof(UINT16)))
                                {
                                    RTD_Log(LOGGER_FATAL, "6.No Write in flow_storage_WriteBackup2Flash\n");
                                    return _FALSE;
                                }
                            }
                            else
                            {
//								drv_spi_flash_Write(CTStorageRing.Bank_no, storageIdxPointer3, (UINT8*)&storageIdxPointer, sizeof(UINT16));
                                if (!RtStorage_Write(storageIdxPointer3, (UINT8*)&storageIdxPointer, sizeof(UINT16)))
                                {
                                    RTD_Log(LOGGER_FATAL, "7.No Write in flow_storage_WriteBackup2Flash\n");
                                    return _FALSE;
                                }
                            }
                            i++;
                            storageIdxPointer2 += sizeof(UINT16);
                            storageIdxPointer3 += sizeof(UINT16);
                        }
                        if (!found)
                        {
                            CTStorage.StorageLastUpdate -= sizeof(UINT16);
//							drv_spi_flash_Write(CTStorageRing.Bank_no, CTStorage.StorageLastUpdate + 1, (UINT8*)&storageIdxPointer, sizeof(UINT16));
                            if (!RtStorage_Write(CTStorage.StorageLastUpdate + 1, (UINT8*)&storageIdxPointer, sizeof(UINT16)))
                            {
                                RTD_Log(LOGGER_FATAL, "8.No Write in flow_storage_WriteBackup2Flash\n");
                                return _FALSE;
                            }
                        }
                    }
                    stStorageIdx.address_idx = addrIdxPointer;
                    stStorageIdx.instance = (UINT16)ptStorageMap;
                    stStorageIdx.subtype = 0;
                    if (found)
                        stStorageIdx.num_index = count;
                    else
                        stStorageIdx.num_index = count + 1;
                    stStorageIdx.type = STORAGE_TYPE_INSTANCE_ARRAY;
                    CTStorage.StorageLastUpdate -= sizeof(RT_STORAGE_STORAGE_IDX);
//					drv_spi_flash_Write(CTStorageRing.Bank_no, CTStorage.StorageLastUpdate + 1, (UINT8*)&stStorageIdx, sizeof(RT_STORAGE_STORAGE_IDX));
                    if (!RtStorage_Write(CTStorage.StorageLastUpdate + 1, (UINT8*)&stStorageIdx, sizeof(RT_STORAGE_STORAGE_IDX)))
                    {
                        RTD_Log(LOGGER_FATAL, "9.No Write in flow_storage_WriteBackup2Flash\n");
                        return _FALSE;
                    }


//					RTD_Log(LOGGER_INFO, " Write storage index address = %x\n", (UINT32)CTStorage.StorageLastUpdate + 1);

                    // Store address index in storage zone into index zone
//					RTD_Log(LOGGER_INFO, " Write address index address = %x\n", (UINT32)addrIdxPointer);
                    CStorageCreateAddressTag(CTStorage.StorageLastUpdate + 1, addrIdxPointer);

                    if (*(UINT16 xdata*)ptStorageMap->INSTANCE != 0xffff)
                    {
                        ptStorageIdx = (RT_STORAGE_STORAGE_IDX code*)*(UINT16 xdata*)ptStorageMap->INSTANCE;
                       // RTD_Log(LOGGER_FATAL, "marked as erase %x\n", (UINT32)ptStorageIdx->address_idx);
                        CStorageMarkAddressTag(ptStorageIdx->address_idx, STORAGE_ADDRESS_TAG_ERASE);
                    }

                    // Store address index in storage zone into variable instance
                    *(UINT16 xdata*)ptStorageMap->INSTANCE = CTStorage.StorageLastUpdate + 1;
//->w					RTD_Log(LOGGER_INFO, "9.WriteBackup2Flash Get new instacnce = %x, index addr = %x\n", (UINT32)ptStorageMap, (UINT32)*(UINT16 xdata*)ptStorageMap->INSTANCE);

                    // check instance whether correct
                    ptStorageIdx = (RT_STORAGE_STORAGE_IDX code*)*(UINT16 xdata*)ptStorageMap->INSTANCE;
                    if (ptStorageIdx->type != STORAGE_TYPE_INSTANCE_ARRAY)
                    {
                        RTD_Log(LOGGER_FATAL, "a.WriteBackup2Flash Get error instacnce = %x, index addr = %x\n", (UINT32)ptStorageMap, (UINT32)*(UINT16 xdata*)ptStorageMap->INSTANCE);
                    }
                }
                else
                {

                }
                stBackupControlBlock.status = 0;
                return _TRUE;
            }
        }
    }
    return _TRUE;
}

/*============================================================================*/
/**
 * flow_storage_LoadData
 * Initial data structure for rt_storage
 *
 * @param <UINT32>		{ void }
 * @param <UINT32>		{ void }
 * @return <BOOL>		{ void }
 *
 */
BOOL flow_storage_LoadData(STORAGE_INSTANCE code* ptStorageInstance, UINT8 xdata* data_addr, UINT8 offset, UINT8 size)
{
    UINT16 DstPointer;
    UINT8 nBank;
//	UINT32 val;

    if (ptStorageInstance == NULL)
        return _FALSE;

    if (!drv_spi_flash_CheckReady())
        return _FALSE;

    // Check whether existed in storage from backup memroy
    if (stBackupControlBlock.INSTANCE == (UINT16)ptStorageInstance)
    {
        flow_storage_memcpy(data_addr, &stBackupControlBlock.BackupStorage[offset], size);
        return _TRUE;
    }

    if (*(UINT16 xdata*)ptStorageInstance->INSTANCE == 0xffff)
    {
//		RTD_Log(LOGGER_INFO, "LoadData(0xfffff)\n");
        // Load default value
        DstPointer = ptStorageInstance->DEFAULT;
        nBank = ptStorageInstance->bank_no;
    }
    else
    {
        // Load from storage zone
        DstPointer = *(UINT16 xdata*)ptStorageInstance->INSTANCE;
        DstPointer += sizeof(RT_STORAGE_STORAGE_IDX);
        nBank = CTStorageRing.Bank_no;
    }

    // flash_addr point to address in storage
    DstPointer += offset;
//	RTD_Log(LOGGER_INFO, "0. load data instance = %x, addr = %x, DstPointer = %x, len = %x\n", (UINT32)stBackupControlBlock.INSTANCE, (UINT32)data_addr, (UINT32)DstPointer, (UINT32)size);
    if (!drv_spi_flash_Read((UINT16)data_addr, nBank, DstPointer, size))
        return _FALSE;

    return _TRUE;
}

/*============================================================================*/
/**
 * flow_storage_LoadArrayData
 * Initial data structure for rt_storage
 *
 * @param <UINT32>		{ void }
 * @param <UINT32>		{ void }
 * @return <BOOL>		{ void }
 *
 */
BOOL flow_storage_LoadArrayData(STORAGE_INSTANCE code* ptStorageInstance, UINT8 xdata* data_addr, UINT16 index, UINT8 offset, UINT8 size)
{
    UINT16 srcPointer;
    UINT8 nBank;

    UINT16 srcPointer1;
    UINT8 found;
    RT_STORAGE_STORAGE_IDX code* ptStorageIdx;
    RT_STORAGE_STORAGE_IDX code* ptStorageIdx1;
    int	i,count;

    if (!drv_spi_flash_CheckReady())
        return _FALSE;

    // Check whether null in data pointer
    if (ptStorageInstance == NULL)
        return _FALSE;

    // Check whether storage array or not
    if (ptStorageInstance->type != STORAGE_TYPE_INSTANCE_ARRAY)
        return _FALSE;

    // Check whether existed in storage from backup memroy
    if (stBackupControlBlock.INSTANCE == (UINT16)ptStorageInstance)
    {
        if (stBackupControlBlock.num_index == index)
        {
//			RTD_Log(LOGGER_INFO, "1.LoadArrayData index = %x, offset = %x, size = %x, instance = %x\n", (UINT32)index, (UINT32)offset, (UINT32)size, (UINT32)stBackupControlBlock.INSTANCE);
            flow_storage_memcpy(data_addr, &stBackupControlBlock.BackupStorage[offset], size);
            return _TRUE;
        }
    }

    // Check index whether over range
    if (index >= ptStorageInstance->Array_num)
        return _FALSE;

//	RTD_Log(LOGGER_INFO, "2.LoadArrayData index = %x, offset = %x, size = %x\n", (UINT32)index, (UINT32)offset, (UINT32)size);
    if (*(UINT16 xdata*)ptStorageInstance->INSTANCE == 0xffff)
    {
//		RTD_Log(LOGGER_INFO, "3.LoadArrayData index = %x, offset = %x, size = %x\n", (UINT32)index, (UINT32)offset, (UINT32)size);
        if ((ptStorageInstance->flag & 0x10) == 0)
        {
            srcPointer = ptStorageInstance->DEFAULT;
            nBank = ptStorageInstance->bank_no;
//			RTD_Log(LOGGER_INFO, "1.Get SrcPointer = %x, default = %x \n", (UINT32)srcPointer, (UINT32) ptStorageInstance->DEFAULT);
        }
        else if (ptStorageInstance->flag & 0x10)
        {
//			RTD_Log(LOGGER_INFO, "Array Get instance = %x\n", *(UINT32 xdata*)ptStorageInstance->INSTANCE);
            srcPointer = ptStorageInstance->DEFAULT;
            srcPointer += ptStorageInstance->length * index;
            nBank = ptStorageInstance->bank_no;
//			RTD_Log(LOGGER_INFO, "2.Get SrcPointer = %x, default = %x \n", (UINT32)srcPointer, (UINT32) ptStorageInstance->DEFAULT);
        }
    }
    else
    {
//		RTD_Log(LOGGER_INFO, "4.LoadArrayData index = %x, offset = %x, size = %x\n", (UINT32)index, (UINT32)offset, (UINT32)size);
        // Load from storage zone
        srcPointer1 = *(UINT16 xdata*)ptStorageInstance->INSTANCE;
        ptStorageIdx = (RT_STORAGE_STORAGE_IDX code*)srcPointer1;
//		RTD_Log(LOGGER_INFO, "4.srcPointer1 = %x, ptStorageIdx = %x, count = %x\n", (UINT32)srcPointer1, (UINT32)ptStorageIdx, (UINT32)ptStorageIdx->num_index);
        if (ptStorageIdx->type == STORAGE_TYPE_INSTANCE_ARRAY)
        {
            // search index
            count = ptStorageIdx->num_index;
            i = 0;
            srcPointer1 += sizeof(RT_STORAGE_STORAGE_IDX);
            found = 0;
            while (i < count)
            {
                // Get ptr
                ptStorageIdx1 = (RT_STORAGE_STORAGE_IDX code*)*(UINT16 code*)srcPointer1;
//				RTD_Log(LOGGER_INFO, "5.srcPointer1 = %x, ptStorageIdx = %x, %x st = %x\n", (UINT32)srcPointer1, (UINT32)ptStorageIdx1, (UINT32)i);
                // Get data context
                if (ptStorageIdx1->num_index == index)
                {
                    srcPointer = (UINT32)ptStorageIdx1;
                    srcPointer += sizeof(RT_STORAGE_STORAGE_IDX);
                    nBank = CTStorageRing.Bank_no;
                    found = 1;
//					RTD_Log(LOGGER_INFO, "1.found src = %x in arrray index = %x\n", (UINT32)srcPointer, (UINT32)index);
                    break;
                }
                srcPointer1 += sizeof(UINT16);
                i++;
            }
            if (!found)
            {
                if ((ptStorageInstance->flag & 0x10) == 0)
                {
                    srcPointer = ptStorageInstance->DEFAULT;
                    nBank = ptStorageInstance->bank_no;
//					RTD_Log(LOGGER_INFO, "1.Not found %x in arrray Get instance = %x\n", (UINT32)index, *(UINT32 xdata*)ptStorageInstance->INSTANCE);
                }
                else if (ptStorageInstance->flag & 0x10)
                {
                    srcPointer = ptStorageInstance->DEFAULT;
                    srcPointer += ptStorageInstance->length * index;
                    nBank = ptStorageInstance->bank_no;
//					RTD_Log(LOGGER_INFO, "2.Not found %x in arrray Get instance = %x\n", (UINT32) index, *(UINT32 xdata*)ptStorageInstance->INSTANCE);

                }
            }
        }
        else if (ptStorageIdx->type == STORAGE_TYPE_INSTANCE_PTR_ARRAY)
        {
            // This is not normal pointer and should start rebuild
            if ((ptStorageInstance->flag & 0x10) == 0)
            {
                srcPointer = ptStorageInstance->DEFAULT;
                nBank = ptStorageInstance->bank_no;
            }
            else if (ptStorageInstance->flag & 0x10)
            {
                srcPointer = ptStorageInstance->DEFAULT;
                srcPointer += ptStorageInstance->length * index;
                nBank = ptStorageInstance->bank_no;
            }
            else
                return _FALSE;
        }
    }

    srcPointer += offset;
//	RTD_Log(LOGGER_INFO, "0.load array data  addr = %x, srcPointer = %x, len = %x\n", (UINT32)data_addr, (UINT32)srcPointer, (UINT32)size);
    if (!drv_spi_flash_Read((UINT16)data_addr, nBank, srcPointer, size))
        return _FALSE;

    return _TRUE;
}

void CStorageMoveNextSector(void)
{
    UINT16 DstPointer;
    DstPointer = CStorageStepNextSector(CTStorage.NewestRingPtr);
    RTD_Log(LOGGER_INFO, "Get CTStorage.NewestRingPtr %x next = %x\n", (UINT32)CTStorage.NewestRingPtr, (UINT32)DstPointer);
//	if(CStorageCheckSectorTag(DstPointer, STORAGE_SECTOR_TAG_USED)) {
    if (*(UINT8 code*)DstPointer != 0xff)  	// clear all in sector
    {
        RTD_Log(LOGGER_INFO, "sector %x is not off\n");
//		drv_sparrow_CPU_Switch_Clock(_FALSE);
//		drv_spi_flash_Erase(CTStorageRing.Bank_no, DstPointer, 4096);
//		drv_sparrow_CPU_Switch_Clock(_TRUE);
    }

    CStorageMarkSectorTag(DstPointer, STORAGE_SECTOR_TAG_USED);
    CStorageMarkSectorTag(CTStorage.NewestRingPtr, STORAGE_SECTOR_TAG_FULL);
    CTStorage.NewestRingPtr = DstPointer;
    CTStorage.StorageLastUpdate = DstPointer + STORAGE_RING_ELEMENT_SIZE - 1; //CTStorage.NewestRingPtr + STORAGE_RING_ELEMENT_SIZE;
    CTStorage.IndexLastUpdate = DstPointer + STORAGE_ERASE_BLOCK_BYTE;//CTStorage.NewestRingPtr + STORAGE_ERASE_BLOCK_BYTE;
//	CStorageFlushOldestSector();

//	RTD_Log(LOGGER_INFO, "MoveNext NewestRingPtr:%x, StorageLastUpdate:%x, IndexLastUpdate:%x\n", (UINT32)DstPointer, (UINT32)CTStorage.StorageLastUpdate, (UINT32)CTStorage.IndexLastUpdate);
}

#if 0
bit CStorageCheckOverflow(UINT16 length)
{
//	UINT16 indoff;
//	UINT32 addrOff;
//	indoff = CTStorage.IndexLastUpdate + sizeof(RT_STORAGE_ADDRESS_IDX);
//	addrOff = CTStorage.StorageLastUpdate - (sizeof(RT_STORAGE_STORAGE_IDX) +  length);
//	RTD_Log(LOGGER_INFO, "Check overflow: index = %x, indx = %x\n", (UINT32)CTStorage.IndexLastUpdate, (UINT32)indoff);
//	RTD_Log(LOGGER_INFO, "Check overflow: last = %x, length = %x, addroff= %x\n", (UINT32)CTStorage.StorageLastUpdate , (UINT32)length, (UINT32)addrOff);
//	if((CTStorage.IndexLastUpdate + (sizeof(RT_STORAGE_ADDRESS_IDX) * 2)) >= (CTStorage.StorageLastUpdate - (sizeof(RT_STORAGE_STORAGE_IDX) + length))) {
    if ((CTStorage.IndexLastUpdate + sizeof(RT_STORAGE_ADDRESS_IDX)) > (CTStorage.StorageLastUpdate - length))
    {
//		RTD_Log(LOGGER_INFO, "CStorageCheckOverflow\n");
        return _FALSE;
    }
    else
    {
        // Add check empty
    }
    return _TRUE;
}
#endif

// bit CStorageCheckRepeatData(UINT32 flashAddr, UINT8 xdata* data_addr, UINT16 size)
/*============================================================================*/
/**
 * flow_storage_SaveData
 * Initial data structure for rt_storage
 *
 * @param <UINT32>		{ void }
 * @param <UINT32>		{ void }
 * @return <BOOL>		{ void }
 *
 */
BOOL flow_storage_SaveData(STORAGE_INSTANCE code * ptStorageInstance, UINT8 xdata* data_addr, UINT8 Offset, UINT8 size)
{

    UINT16 i;
    UINT8 xdata* ptBackupStorage;
    UINT16 DstPointer;
    UINT8 nBank;

    if ((ptStorageInstance == NULL) || (data_addr == NULL))
        return _FALSE;

    if (ptStorageInstance->type != STORAGE_TYPE_INSTANCE)
        return _FALSE;

    if (!drv_spi_flash_CheckReady())
        return _FALSE;

    if (stBackupControlBlock.INSTANCE != (UINT16)ptStorageInstance)
    {
        // Check whether same with default
        if (*(UINT16 xdata*)ptStorageInstance->INSTANCE == 0xffff)
        {
            DstPointer = ptStorageInstance->DEFAULT;
            if (CStorageCheckRepeatData(ptStorageInstance->bank_no, DstPointer + Offset, data_addr, size))
            {
//->w				RTD_Log(LOGGER_INFO, "Same data with default in flow_storage_SaveData\n");
                return _TRUE;
            }
        }

        flow_storage_WriteBackup2Flash();

//		RTD_Log(LOGGER_INFO, "flow_storage_SaveData\n");
        if (*(UINT16 xdata*)ptStorageInstance->INSTANCE == 0xffff)
        {
            //RTD_Log(LOGGER_INFO, "LoadData(0xfffff)\n");
            // Load default value
            DstPointer = ptStorageInstance->DEFAULT;
            nBank = ptStorageInstance->bank_no;
        }
        else
        {
            // Load from storage zone
            DstPointer = *(UINT16 xdata*)ptStorageInstance->INSTANCE;
            DstPointer += sizeof(RT_STORAGE_STORAGE_IDX);
            nBank = CTStorageRing.Bank_no;
        }

        // flash_addr point to address in storage
//		RTD_Log(LOGGER_INFO, "0.SaveData backaddr = %x, DstPointer = %x, len = %x\n", (UINT32)stBackupControlBlock.BackupStorage, (UINT32)DstPointer, (UINT32)ptStorageInstance->length);
        if (!drv_spi_flash_Read((UINT16)stBackupControlBlock.BackupStorage, nBank, DstPointer, ptStorageInstance->length))
            return _FALSE;

        stBackupControlBlock.INSTANCE  = (UINT16)ptStorageInstance;
        stBackupControlBlock.status = 0;
        stBackupControlBlock.num_index = 0;
    }
    ptBackupStorage = stBackupControlBlock.BackupStorage;
    ptBackupStorage += Offset;
    for (i = 0; i < size; i++)
    {
        *ptBackupStorage = *data_addr;
        data_addr++;
        ptBackupStorage++;
    }
    stBackupControlBlock.status++;

    fw_timer_event_ReactiveTimerEvent(SEC(2), timerevent_table_rt_storage_sync_event);

    return _TRUE;
}

/*============================================================================*/
/**
 * flow_storage_SaveArrayData
 * Initial data structure for rt_storage
 *
 * @param <UINT32>		{ void }
 * @param <UINT32>		{ void }
 * @return <BOOL>		{ void }
 *
 */
BOOL flow_storage_SaveArrayData(STORAGE_INSTANCE code* ptStorageInstance, UINT8 xdata* data_addr, UINT16 index, UINT8 offset, UINT8 size)
{
    UINT16 count;
    UINT8 found;
    UINT8 xdata* ptBackupStorage;
    UINT8 nBank;
    UINT16 srcPointer;
    UINT16 srcPointer1;
    RT_STORAGE_STORAGE_IDX code* ptStorageIdx;
    RT_STORAGE_STORAGE_IDX code* ptStorageIdx1;

    if ((ptStorageInstance == NULL) || (data_addr == NULL))
        return _FALSE;

    if (ptStorageInstance->type != STORAGE_TYPE_INSTANCE_ARRAY)
        return _FALSE;

    if (!drv_spi_flash_CheckReady())
        return _FALSE;

    if ((stBackupControlBlock.INSTANCE != (UINT16)ptStorageInstance) ||
            ((stBackupControlBlock.INSTANCE == (UINT16)ptStorageInstance) && (stBackupControlBlock.num_index != index)))
    {

        flow_storage_WriteBackup2Flash();

//->w		RTD_Log(LOGGER_INFO, "0.SaveArrayData cur block = %x, index = %x, offset = %x, size = %x\n", (UINT32)stBackupControlBlock.INSTANCE, (UINT32)index, (UINT32)offset, (UINT32)size);

        if (*(UINT16 xdata*)ptStorageInstance->INSTANCE == 0xffff)
        {
            if ((ptStorageInstance->flag & 0x10) == 0)
            {
                srcPointer = ptStorageInstance->DEFAULT;
                nBank = ptStorageInstance->bank_no;
//				RTD_Log(LOGGER_INFO, "1.SaveArrayData srcPointer = %x\n", (UINT32)srcPointer);
            }
            else if (ptStorageInstance->flag & 0x10)
            {
                srcPointer = ptStorageInstance->DEFAULT;
                srcPointer += ptStorageInstance->length * index;
                nBank = ptStorageInstance->bank_no;
//				RTD_Log(LOGGER_INFO, "2.SaveArrayData srcPointer = %x\n", (UINT32)srcPointer);
            }
        }
        else
        {
            // Load from storage zone
            ptStorageIdx = (RT_STORAGE_STORAGE_IDX code*)*(UINT16 xdata*)ptStorageInstance->INSTANCE;
            if (ptStorageIdx->type == STORAGE_TYPE_INSTANCE_ARRAY)
            {
                // search index
                srcPointer1 = *(UINT16 xdata*)ptStorageInstance->INSTANCE;
//				RTD_Log(LOGGER_INFO, "4.0 INSTANCE = %x, instance = %x, srcPointer1 = %x\n", (UINT32)ptStorageInstance, (UINT32)ptStorageInstance->INSTANCE, (UINT32)srcPointer1);
                if (ptStorageIdx->num_index > ptStorageInstance->Array_num)
                    count = ptStorageInstance->Array_num;
                else
                    count = ptStorageIdx->num_index;

                srcPointer1 += sizeof(RT_STORAGE_STORAGE_IDX);
                found = 0;
                while (count)
                {
                    // Get ptr
                    ptStorageIdx1 = (RT_STORAGE_STORAGE_IDX code*)*(UINT16 code*)srcPointer1;

                    // Get data context
                    if (ptStorageIdx1->num_index == index)
                    {
                        srcPointer = (UINT32)ptStorageIdx1;
                        srcPointer += sizeof(RT_STORAGE_STORAGE_IDX);
                        nBank = CTStorageRing.Bank_no;
//						RTD_Log(LOGGER_INFO, "3.SaveArrayData index = %x, offset = %x, size = %x, srcPointer = %x\n", (UINT32)index, (UINT32)offset, (UINT32)size, (UINT32)srcPointer);
                        found = 1;
                        break;
                    }
                    srcPointer1 += sizeof(UINT16);
                    count--;
                }

//->w				RTD_Log(LOGGER_INFO, "4. INSTANCE = %x, found = %x\n", (UINT32)ptStorageInstance, (UINT32)found);

                if (!found)
                {
                    if ((ptStorageInstance->flag & 0x10) == 0)
                    {
                        srcPointer = ptStorageInstance->DEFAULT;
                        nBank = ptStorageInstance->bank_no;
//						RTD_Log(LOGGER_INFO, "4.Not found %x in arrray Get instance = %x\n", (UINT32)index, *(UINT32 xdata*)ptStorageInstance->INSTANCE);
                    }
                    else if (ptStorageInstance->flag & 0x10)
                    {
                        srcPointer = ptStorageInstance->DEFAULT;
                        srcPointer += ptStorageInstance->length * index;
                        nBank = ptStorageInstance->bank_no;
//						RTD_Log(LOGGER_INFO, "5.Not found %x in arrray Get instance = %x\n", (UINT32) index, *(UINT32 xdata*)ptStorageInstance->INSTANCE);

                    }
                    else
                    {
                        RTD_Log(LOGGER_INFO, "a.Not found flag = %x Get instance = %x\n", (UINT32)ptStorageInstance->flag, *(UINT32 xdata*)ptStorageInstance->INSTANCE);
                    }
                }
            }
            else if (ptStorageIdx->type == STORAGE_TYPE_INSTANCE_PTR_ARRAY)
            {
                // This is not normal pointer and should start rebuild
                if ((ptStorageInstance->flag & 0x10) == 0)
                {
                    srcPointer = ptStorageInstance->DEFAULT;
                    nBank = ptStorageInstance->bank_no;
//					RTD_Log(LOGGER_INFO, "6.SaveArrayData srcPointer = %x\n", (UINT32)srcPointer);
                }
                else if (ptStorageInstance->flag & 0x10)
                {
                    srcPointer = ptStorageInstance->DEFAULT;
                    srcPointer += ptStorageInstance->length * index;
                    nBank = ptStorageInstance->bank_no;
//					RTD_Log(LOGGER_INFO, "7.SaveArrayData srcPointer = %x\n", (UINT32)srcPointer);
                }
                else
                {
                    RTD_Log(LOGGER_FATAL, "1.flow_storage_SaveArrayData:Get error flag %x in INSTANCE\n", ptStorageInstance->flag);
                    return _FALSE;
                }
            }
            else
            {

                RTD_Log(LOGGER_FATAL, "2.flow_storage_SaveArrayData:Get Error type %x in INSTANCE\n", ptStorageIdx->type);
            }
        }
        // flash_addr point to address in storage
//		RTD_Log(LOGGER_INFO, "8.SaveArrayData index = %x, offset = %x, size = %x, from addr = %x, len = %x\n", (UINT32)index, (UINT32)offset, (UINT32)size, (UINT32)srcPointer, (UINT32)ptStorageInstance->length);
        if (!drv_spi_flash_Read((UINT16)BackupStorage,  nBank, srcPointer, ptStorageInstance->length))
        {
            return _FALSE;
        }

        stBackupControlBlock.INSTANCE = (UINT16)ptStorageInstance;
        stBackupControlBlock.status = 0;
        stBackupControlBlock.num_index = index;
//		RTD_Log(LOGGER_INFO, "stBackupControlBlockx num_idx = %x, instance = %x\n", (UINT32)stBackupControlBlock.num_index, (UINT32)stBackupControlBlock.INSTANCE);
    }
    ptBackupStorage = stBackupControlBlock.BackupStorage;
    ptBackupStorage += offset;
    for (count = 0; count < size; count++)
    {
        *ptBackupStorage = *data_addr;
        data_addr++;
        ptBackupStorage++;
    }
    stBackupControlBlock.status++;

    fw_timer_event_ReactiveTimerEvent(SEC(2), timerevent_table_rt_storage_sync_event);

    return _TRUE;
}

/*============================================================================*/
/**
 * CStorageUpdateStorageAddress
 * Inialize rt_storage in STORAGE_INSTANCE stored in xdata
 *
 * @param <UINT32>		{ void }
 * @return <BOOL>		{ void }
 *
 */
BOOL CStorageUpdateStorageAddress(UINT16 storageAddr)
{
    STORAGE_INSTANCE code* ptStorageMap;
    RT_STORAGE_STORAGE_IDX code* ptRtStorageIdx;
    RT_STORAGE_ADDRESS_IDX code* ptAddrIdx;
    UINT8  storageType;
    UINT16 instance;
    int i,num;

    if (!drv_spi_flash_CheckReady())
        return _FALSE;

    // check valid address
    if (storageAddr == 0xffff)
        return _FALSE;

    ptRtStorageIdx = (RT_STORAGE_STORAGE_IDX code*)storageAddr;
    storageType = ptRtStorageIdx->type;
    instance = ptRtStorageIdx->instance;
//	RTD_Log(LOGGER_INFO, "addr = %x, Read Type %x...\n", (UINT32)storageAddr, (UINT32)storageType);
//	RTD_Log(LOGGER_INFO, "Read instance %x...\n", (UINT32)instance);

    num = flow_storage_SystemInstanceSize();
    for (i = 0; i < num; i++)
    {
        ptStorageMap = (STORAGE_INSTANCE code*)STORAGE_SYSTEM_INSTANCE_MAP[i];
        if ((UINT16)ptStorageMap == instance)
        {
            if (storageAddr >= CTStorageRing.RingStart)
            {
                if (storageAddr <= (CTStorageRing.RingEnd + STORAGE_RING_ELEMENT_SIZE - 1))
                {
                    if (ptStorageMap->type == STORAGE_TYPE_INSTANCE)
                    {
                        if (storageType == STORAGE_TYPE_INSTANCE)
                        {
                            *((UINT16 xdata *)ptStorageMap->INSTANCE) = storageAddr;
                            ptAddrIdx = (RT_STORAGE_ADDRESS_IDX code*)ptRtStorageIdx->address_idx;
//							RTD_Log(LOGGER_INFO, "STORAGE_TYPE_INSTANCE %x-st INSTANCE =%x addr = %x, marked = %x\n", (UINT32)i, (UINT32)ptStorageMap->INSTANCE, (UINT32)storageAddr, (UINT32)ptAddrIdx->tag);
                            return _TRUE;
                        }
                    }
                    else if (ptStorageMap->type == STORAGE_TYPE_INSTANCE_ARRAY)
                    {
                        if (storageType == STORAGE_TYPE_INSTANCE_ARRAY)
                        {
                            *((UINT16 xdata*)ptStorageMap->INSTANCE) = storageAddr;
                            ptAddrIdx = (RT_STORAGE_ADDRESS_IDX code*)ptRtStorageIdx->address_idx;
//							RTD_Log(LOGGER_INFO, "STORAGE_TYPE_INSTANCE %x-st INSTANCE =%x addr = %x, marked = %x\n", (UINT32)i, (UINT32)ptStorageMap->INSTANCE, (UINT32)storageAddr, (UINT32)ptAddrIdx->tag);
                            return _TRUE;
                        }
                    }
                }
            }
        }
    }

    return _FALSE;
}

/*============================================================================*/
/**
 * flow_storage_InitialScan
 * Initial data structure for rt_storage
 *
 * @param 	<void>		{ void }
 * @return	<BOOL>		{ TRUE if initize ok, otherwise FALSE }
 *
 */
BOOL flow_storage_Init(void)
{

    BOOL UsedFlag;
    volatile UINT16 fFirstRun;
    volatile UINT32 fFirstRunVal;
    UINT16 ScanPointer, DstPointer;
    RT_STORAGE_ADDRESS_IDX code* ptAddrIdx;
    RT_STORAGE_STORAGE_IDX code* ptStorageIdx;
    STORAGE_INSTANCE code* ptStorageMap;
    int i;

    // Check flash whether ready is or not
    if (!drv_spi_flash_CheckReady())
        return _FALSE;

    // Initialize storage system
    ScanPointer= flow_storage_SystemInstanceSize();
    for (i = 0; i < ScanPointer; i++)
    {
        ptStorageMap = (STORAGE_INSTANCE code*)STORAGE_SYSTEM_INSTANCE_MAP[i];
        *((UINT16 xdata *)ptStorageMap->INSTANCE) = 0xffff;
    }

    stBackupControlBlock.INSTANCE = 0xffff;
    stBackupControlBlock.status = 0;
    stBackupControlBlock.num_index = 0;
    stBackupControlBlock.size = sizeof(BackupStorage) / sizeof(UINT8);
    stBackupControlBlock.BackupStorage = BackupStorage;

    // Initialize backup storage
    for (i = 0; i < stBackupControlBlock.size; i++)
        stBackupControlBlock.BackupStorage[i] = 0xff;

    // Initialize index parameter
    CTStorage.StorageLastUpdate = STORAGE_RING_START + (STORAGE_RING_ELEMENT_SIZE - 1);	//  UINT32 StorageLastUpdate;
    CTStorage.IndexLastUpdate = STORAGE_RING_START;								//  UINT32 IndexLastUpdate;
    CTStorage.OldestRingPtr = STORAGE_RING_START;									//  UINT32 OldestRingPtr;
    CTStorage.NewestRingPtr = STORAGE_RING_START;									//  UINT32 NewestRingPtr;

    // Get CStorageFirstRun from flash in order to check whether first run or not
    UsedFlag = 1;
    fFirstRun = (CStorageFirstRun);
    if (!drv_spi_flash_Read((UINT16)&fFirstRunVal, _FIRSTRUN_VAR_BANK, fFirstRun, sizeof(UINT32)))
        return _FALSE;

    if (fFirstRunVal != 0)
    {
        UsedFlag = 0;
    }
    else
    {
        fFirstRun = (CStorageVersion);
        if (!drv_spi_flash_Read((UINT16)&fFirstRunVal, _FIRSTRUN_VAR_BANK, fFirstRun, sizeof(UINT32)))
            return _FALSE;

        if (fFirstRunVal != _VERSION_CODE)
        {
            UsedFlag = 0;
        }
    }

    if (!UsedFlag)
    {
        // erase storage, Must be sure that speed at 27Mhz
        drv_spi_flash_Erase(CTStorageRing.Bank_no, CTStorageRing.RingStart, CTStorageRing.RingEnd - CTStorageRing.RingStart + STORAGE_RING_ELEMENT_SIZE);

        // erase storage, Must be sure that speed at 27Mhz
        drv_spi_flash_Erase(_FIRSTRUN_VAR_BANK, (CStorageFirstRun & 0xf000), 4096);

        // Set CStorageFirstRun to 0 specify not first run
        fFirstRunVal = 0;
        drv_spi_flash_Write(_FIRSTRUN_VAR_BANK, CStorageFirstRun,(UINT8*)&fFirstRunVal, sizeof(UINT32));

        fFirstRunVal = _VERSION_CODE;
        drv_spi_flash_Write(_FIRSTRUN_VAR_BANK, CStorageVersion,(UINT8*)&fFirstRunVal, sizeof(UINT32));

        // system in stoarge all set to default
        flow_storage_LoadDefault(STORAGE_STORE_SYSTEM);

        // factory in stoarge all set to default
        flow_storage_LoadDefault(STORAGE_STORE_FACTORY);

        CTStorage.IndexLastUpdate += STORAGE_ERASE_BLOCK_BYTE;

        RTD_Log(LOGGER_INFO, "CTStorageRing.RingStart %x CTStorageRing.RingEnd %x\n", (UINT32)CTStorageRing.RingStart, (UINT32)CTStorageRing.RingEnd);
        RTD_Log(LOGGER_INFO, "StorageLastUpdate %x...\n", (UINT32)CTStorage.StorageLastUpdate);
        RTD_Log(LOGGER_INFO, "IndexLastUpdate %x...\n", (UINT32)CTStorage.IndexLastUpdate);
        RTD_Log(LOGGER_INFO, "OldestRingPtr %x...\n", (UINT32)CTStorage.OldestRingPtr);
        RTD_Log(LOGGER_INFO, "NewestRingPtr %x...\n", (UINT32)CTStorage.NewestRingPtr);
        RTD_Log(LOGGER_INFO, "flow_storage_Init Complete\n");

        return _TRUE;
    }

    // Find CTStorage.NewestRingPtr
    CTStorage.NewestRingPtr = 0;
    UsedFlag = 0;
    ScanPointer = CTStorageRing.RingStart;
    do
    {
        // Check whether sector sector is in used state
//		RTD_Log(LOGGER_INFO, "1.ScanSectorPointer %x...\n", (UINT32)ScanPointer);
        if (CStorageCheckSectorTag(ScanPointer, STORAGE_SECTOR_TAG_USED))
        {
            UsedFlag = 1;
            // Check whether sector is  not in full state
            if (!CStorageCheckSectorTag(ScanPointer, STORAGE_SECTOR_TAG_FULL))
            {
                CTStorage.NewestRingPtr = ScanPointer;
            }
//			RTD_Log(LOGGER_INFO, "2.ScanSectorPointer %x...\n", (UINT32)ScanPointer);
        }
        else   // not used
        {
            if (UsedFlag == 1)
            {
                if (CStorageCheckSectorTag(CStorageStepPrevSector(ScanPointer), STORAGE_SECTOR_TAG_FULL))
                {
                    CTStorage.NewestRingPtr = ScanPointer;
                }
                else
                {
                    CTStorage.NewestRingPtr = CStorageStepPrevSector(ScanPointer);
                }
//				RTD_Log(LOGGER_INFO, "3.ScanSectorPointer %x...\n", (UINT32)ScanPointer);
                break;
            }
            UsedFlag = 0;
        };
        // avoid Overflow
        if ((ScanPointer + STORAGE_RING_ELEMENT_SIZE) < ScanPointer)
        {
//			RTD_Log(LOGGER_INFO, "4.ScanSectorPointer %x...\n", (UINT32)ScanPointer);
            break;
        }

        ScanPointer += STORAGE_RING_ELEMENT_SIZE;
    }
    while (ScanPointer <= CTStorageRing.RingEnd);

    if (CTStorage.NewestRingPtr == 0)
        CTStorage.NewestRingPtr = CTStorageRing.RingStart;

//	RTD_Log(LOGGER_INFO, "2.NewestRingPtr %x...\n", (UINT32)CTStorage.NewestRingPtr);

    // Find CTStorage->OldestRingPtr
    CTStorage.OldestRingPtr = CTStorage.NewestRingPtr;
    ScanPointer = CTStorage.NewestRingPtr;
    do
    {
//		RTD_Log(LOGGER_INFO, "2.ScanPointer %x...\n", ScanPointer);
        if (ScanPointer != CTStorage.NewestRingPtr)
        {
            if (CStorageCheckSectorTag(ScanPointer, STORAGE_SECTOR_TAG_USED)== 0 /*|| CStorageCheckTag(ScanPointer, STORAGE_TAG_ERASE)*/) // not used	or In Erase
                break;
        }

        CTStorage.OldestRingPtr = ScanPointer;
        ScanPointer = CStorageStepPrevSector(ScanPointer);
    }
    while (ScanPointer != CTStorage.NewestRingPtr);

    // Scan back and load back
    ScanPointer = CTStorage.OldestRingPtr;
    DstPointer = CStorageStepNextSector(CTStorage.NewestRingPtr);
//	RTD_Log(LOGGER_INFO, "ScanPt = %x, DstPt = %x\n", (UINT32)ScanPointer, (UINT32)DstPointer);
    // Reload the saved data from storage
    do
    {
        // return if this is a brand new storage
        if (CStorageCheckSectorTag(ScanPointer, STORAGE_SECTOR_TAG_ERASE))
        {
            //整個storage sector 已經erase 過不檢查, 跳過檢查下一個
//			RTD_Log(LOGGER_INFO, "This erased sector %x\n", (UINT32)ScanPointer);
            ScanPointer += STORAGE_RING_ELEMENT_SIZE;
            continue;
        };

        // Check sector tag whether in used state
        if (CStorageCheckSectorTag(ScanPointer, STORAGE_SECTOR_TAG_USED) == 0)
        {
//			RTD_Log(LOGGER_INFO, "sector %x to be marked as used\n", (UINT32)ScanPointer);
            CStorageMarkSectorTag(ScanPointer, STORAGE_SECTOR_TAG_USED);
        }

//		RTD_Log(LOGGER_INFO, "sector %x\n", (UINT32)ScanPointer);
        // Get start address in index block
        CTStorage.IndexLastUpdate = ScanPointer;
        CTStorage.IndexLastUpdate += STORAGE_ERASE_BLOCK_BYTE;
        CTStorage.StorageLastUpdate = (ScanPointer + STORAGE_RING_ELEMENT_SIZE - 1);
//		RTD_Log(LOGGER_INFO, "StorageLastUpdate %x\n", (UINT32)CTStorage.StorageLastUpdate);
//		RTD_Log(LOGGER_INFO, "IndexLastUpdate %x\n", (UINT32)CTStorage.IndexLastUpdate);
        // Find valid in IndexLastUpdate
        while (CTStorage.StorageLastUpdate > CTStorage.IndexLastUpdate)
        {
            ptAddrIdx = (RT_STORAGE_ADDRESS_IDX code*)CTStorage.IndexLastUpdate;
//			RTD_Log(LOGGER_INFO, "Get ptAddrIdx %x \n", (UINT32)ptAddrIdx);

            // check whether Empty
            if (ptAddrIdx->tag == 0xff)
                if (ptAddrIdx->storage_address == 0xffff)
                    break;

            // data check fail when dirty
            if (CStorageCheckAddressTag(CTStorage.IndexLastUpdate, STORAGE_ADDRESS_TAG_NOAVAIL))
            {
//				RTD_Log(LOGGER_INFO, "Addr %x is noavail from IndexLastUpdate\n", (UINT32)CTStorage.IndexLastUpdate);
                CTStorage.IndexLastUpdate += sizeof(RT_STORAGE_ADDRESS_IDX);
                continue;
            }

            // Check whether valid range
            if (((UINT16)ptAddrIdx< CTStorageRing.RingStart) || ((UINT16)ptAddrIdx > (CTStorageRing.RingEnd + STORAGE_RING_ELEMENT_SIZE - 1)))
            {
                CTStorage.IndexLastUpdate += sizeof(RT_STORAGE_ADDRESS_IDX);
                continue;  // data check fail
            }

//			RTD_Log(LOGGER_INFO, "Get ptAddrIdx %x, address = %x \n", (UINT32)ptAddrIdx, (UINT32) ptAddrIdx->storage_address);
            CTStorage.StorageLastUpdate = ptAddrIdx->storage_address - 1;
            ptStorageIdx = (RT_STORAGE_STORAGE_IDX code*) ptAddrIdx->storage_address ;
            if (ptStorageIdx == 0xffff)
            {
                CTStorage.IndexLastUpdate += sizeof(RT_STORAGE_ADDRESS_IDX);
                continue;  // data check fail
            }

            // Check address whether different with stoarge
            if (ptStorageIdx->address_idx != CTStorage.IndexLastUpdate)
            {
                CTStorage.IndexLastUpdate += sizeof(RT_STORAGE_ADDRESS_IDX);
                continue;  // data check fail
            }

            if (!CStorageCheckAddressTag((UINT16)ptStorageIdx->address_idx, STORAGE_ADDRESS_TAG_ERASE))
            {
                if (CStorageUpdateStorageAddress((UINT16)ptStorageIdx))
                {
                    CTStorage.IndexLastUpdate += sizeof(RT_STORAGE_ADDRESS_IDX);
                    continue;  // data check fail
                }
            }

//			CTStorage.StorageLastUpdate--;
//			RTD_Log(LOGGER_INFO, "StorageLastUpdate %x...\n", (UINT32)CTStorage.StorageLastUpdate);
            CTStorage.IndexLastUpdate += sizeof(RT_STORAGE_ADDRESS_IDX);
        }
        ScanPointer = CStorageStepNextSector(ScanPointer);
//		RTD_Log(LOGGER_INFO, "Get Next ScanPt = %x\n", (UINT32)ScanPointer);

        //
        if ((ScanPointer > CTStorageRing.RingEnd) || (ScanPointer < CTStorageRing.RingStart))
            break;
    }
    while (ScanPointer != DstPointer);

    RTD_Log(LOGGER_INFO, "BANK = %x , CTStorageRing.RingStart %x CTStorageRing.RingEnd %x\n", (UINT32)CTStorageRing.Bank_no, (UINT32)CTStorageRing.RingStart, (UINT32)CTStorageRing.RingEnd);
    RTD_Log(LOGGER_INFO, "StorageLastUpdate %x...\n", (UINT32)CTStorage.StorageLastUpdate);
    RTD_Log(LOGGER_INFO, "IndexLastUpdate %x...\n", (UINT32)CTStorage.IndexLastUpdate);
    RTD_Log(LOGGER_INFO, "OldestRingPtr %x...\n", (UINT32)CTStorage.OldestRingPtr);
    RTD_Log(LOGGER_INFO, "NewestRingPtr %x...\n", (UINT32)CTStorage.NewestRingPtr);

    // Check version
//	if(!flow_storage_system_StartupCheck())
//		return _FALSE;

    RTD_Log(LOGGER_INFO, "flow_storage_Init ...\n");

    return _TRUE;
}

BOOL flow_storage_SyncDataDefault(STORAGE_INSTANCE code* ptStorageInstance)
{
    RT_STORAGE_STORAGE_IDX code* ptStorageIdx;

    if (ptStorageInstance == NULL)
        return _FALSE;

    // Check flash whether ready is or not
    if (!drv_spi_flash_CheckReady())
        return _FALSE;

//	RTD_Log(LOGGER_INFO, "flow_storage_SyncDataDefault\n");

    // Write to default when data in backup zone	990426
    if (stBackupControlBlock.INSTANCE == (UINT32)ptStorageInstance)
        stBackupControlBlock.INSTANCE	 = 0xffff;

    // Store data contents into storaCStorageRemainSectorge zone
    if (*(UINT16 xdata *)ptStorageInstance->INSTANCE != 0xffff)
    {
        ptStorageIdx = (RT_STORAGE_STORAGE_IDX code*)*(UINT16 xdata*)ptStorageInstance->INSTANCE;
        CStorageMarkAddressTag(ptStorageIdx->address_idx, STORAGE_ADDRESS_TAG_ERASE);
        *(UINT16 xdata *)ptStorageInstance->INSTANCE = 0xffff;
    }

    return _TRUE;
}

