
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
#include "i2c\i2c.h"
#include "pcb_customer.h"
#include "timer_event.h"
/*================================ Definitions ==============================*/
#define	EEPROM_FIRSTRUN_START_ADDRESS	0x0000
#define	EEPROM_VERSION_START_ADDRESS	(EEPROM_FIRSTRUN_START_ADDRESS + sizeof(UINT16))
#define	EEPROM_STORAGE_HEADER_START_ADDRESS	(EEPROM_VERSION_START_ADDRESS + sizeof(UINT16))
#define	EEPROM_STORAGE_EEPROM_START_ADDRESS	(EEPROM_FIRSTRUN_START_ADDRESS + 32)
#define	EEPROM_STORAGE_XDATA_START_ADDRESS	(0x8000)
#define 	EEPROM_STORAGE_VERSION_CODE 	(_VERSION_CODE + 4)
#define	USE_STORAGE_HEADER_XDATA		1
/*================================== Variables ==============================*/
extern UINT16 code STORAGE_SYSTEM_INSTANCE_MAP[];
void timerevent_table_rt_storage_sync_event(void);
#if 0
void flow_storage_TestEEPROM(void)
{
    UINT8 bufRData[17];
    UINT8 bufWData[18];

    drv_i2c_Set_TargetAddr(I2C_MASTER_TUNER, (0xa0 >> 1));

    // Write
    bufWData[0] = 0x0;
    bufWData[1] = 0x20;
    bufWData[2] = 0xb;	// First
    bufWData[3] = 6;
    bufWData[4] = 7;
    bufWData[5] = 8;
    bufWData[6] = 0x27;
    bufWData[7] = 0x38;
    bufWData[8] = 0x39;
    bufWData[9] = 0x3a;
    bufWData[10] = 0x3b;
    bufWData[11] = 0x3c;
    bufWData[12] = 0x40c;
    bufWData[13] = 0x41;
    bufWData[14] = 0x42;
    bufWData[15] = 0x43;
    bufWData[16] = 0x44;
    bufWData[17] = 0x45;
    drv_i2c_Write(I2C_MASTER_TUNER, 18, bufWData);

    fw_timer_event_DelayXms(20);
    // Read
    drv_i2c_Set_TargetAddr(I2C_MASTER_TUNER, (0xa0 >> 1));
    bufWData[0] = 0x0;
    bufWData[1] = 0x20;
    bufRData[0] = 0;
    bufRData[1] = 0;
    bufRData[2] = 0;
    bufRData[3] = 0;
    bufRData[4] = 0;
    bufRData[5] = 0;

    bufRData[6] = 0;
    bufRData[7] = 0;
    bufRData[8] = 0;
    bufRData[9] = 0;
    drv_i2c_randomRead(I2C_MASTER_TUNER, 2, 16, bufWData, bufRData);

    RTD_Log(LOGGER_INFO, "eeprom: u0:%x, u1=%x, u2=%x, u3=%x, u4=%x\n", (UINT32)bufRData[0], (UINT32)bufRData[1], (UINT32)bufRData[2], (UINT32)bufRData[3], (UINT32)bufRData[4]);
    RTD_Log(LOGGER_INFO, "eeprom: u5:%x, u6=%x, u7=%x, u8=%x, u9=%x\n", (UINT32)bufRData[5], (UINT32)bufRData[6], (UINT32)bufRData[7], (UINT32)bufRData[8], (UINT32)bufRData[9]);
    RTD_Log(LOGGER_INFO, "eeprom: u10:%x, u11=%x, u12=%x, u13=%x, u14=%x, u15=%x\n", (UINT32)bufRData[10], (UINT32)bufRData[11], (UINT32)bufRData[12], (UINT32)bufRData[13], (UINT32)bufRData[14], (UINT32)bufRData[15]);
}
#endif

BOOL flow_storage_WriteBackup2Flash(void)
{
    return _TRUE;
}

// mode:
//	0: eeprom
//	1: xdata
bool flow_eeprom_Read(UINT16 dest_addr, UINT16 src_addr, UINT16 len)
{
    UINT8 wBuf[2];

    if (len == 0)
        return _FALSE;

    drv_i2c_Set_TargetAddr(I2C_MASTER_TUNER, (0xa0 >> 1));
    while (len > 0)
    {
        wBuf[0] = *((UINT8*)&src_addr);
        wBuf[1] = *((UINT8*)&src_addr + 1);
        if (len >= 16)
        {
//			drv_i2c_Set_TargetAddr(I2C_MASTER_TUNER, (0xa0 >> 1));
            drv_i2c_randomRead(I2C_MASTER_TUNER, 2, 16, wBuf, (UINT8 xdata*)dest_addr);
            dest_addr += 16;
            src_addr += 16;
            len -= 16;
        }
        else
        {
//			drv_i2c_Set_TargetAddr(I2C_MASTER_TUNER, (0xa0 >> 1));
            drv_i2c_randomRead(I2C_MASTER_TUNER, 2, len, wBuf, (UINT8 xdata*)dest_addr);
            len = 0;
        }
    }

    return _TRUE;
}
#if 0
// mode:
//	0: eeprom
//	1: xdata
void flow_eeprom_Write(UINT16 StartAddress, UINT8* sBuf, UINT16 nSize, UINT8 mode)
{
    UINT8	wBuf[18];
//	int i;
    if ((sBuf == NULL) || (nSize == 0))
        return;

    while (nSize > 0)
    {
        if (!mode)  	// eeprom
        {
            wBuf[0] = *( (UINT8*)&StartAddress);
            wBuf[1] = *( (UINT8*)&StartAddress + 1);
            if (nSize >= 8)
            {
                flow_storage_memcpy(wBuf + 2, sBuf, 8);
                drv_i2c_Set_TargetAddr(I2C_MASTER_TUNER, (0xa0 >> 1));
                drv_i2c_Write(I2C_MASTER_TUNER, 10, wBuf);
                StartAddress += 8;
                sBuf += 8;
                nSize -= 8;
            }
            else
            {
                flow_storage_memcpy(wBuf + 2, sBuf, nSize);
                drv_i2c_Set_TargetAddr(I2C_MASTER_TUNER, (0xa0 >> 1));
                drv_i2c_Write(I2C_MASTER_TUNER, nSize + 2, wBuf);
                nSize = 0;
            }
        }
        else  		// xdata
        {
            flow_storage_memcpy(sBuf, (UINT8*)StartAddress, nSize);
            nSize = 0;
        }
    }
}
#endif

void flow_eeprom_Write(UINT16 StartAddress, UINT8* sBuf, UINT16 nSize)
{
    UINT8	wBuf[3];
//	UINT8	rBuf[2];

    if ((sBuf == NULL) || (nSize == 0))
        return;

    drv_i2c_Set_TargetAddr(I2C_MASTER_TUNER, (0xa0 >> 1));
    while (nSize > 0)
    {
        wBuf[0] = *( (UINT8*)&StartAddress);
        wBuf[1] = *( (UINT8*)&StartAddress + 1);
        wBuf[2] = *sBuf;
        drv_i2c_Write(I2C_MASTER_TUNER, 3, wBuf);

//		fw_timer_event_DelayXms(10);
//		rBuf[0] = 0xff;
//		drv_i2c_randomRead(I2C_MASTER_TUNER, 2, 1, wBuf, rBuf);
//		if(wBuf[2] != rBuf[0])
//			RTD_Log(LOGGER_INFO, "eeprom write error: dest_addr = %x, wr = %x, rd = %x\n", (UINT32)StartAddress, (UINT32)wBuf[2], (UINT32)rBuf[0]);
        StartAddress++;
        sBuf++;
        nSize--;
    }
}

#if 0
void flow_eeprom_WriteFrmFlash(UINT16 destAddr, UINT8 bankno, UINT16 srcAddr, UINT16 nSize, UINT8 mode)
{
    UINT8 buf_data[16];
    UINT8 buf_wdata[16];
    UINT8 buf_rcommand[2];
    int i;

    while (nSize > 0)
    {
        for (i = 0; i < 16; i++)
            buf_wdata[i] = 0xff;

        if (nSize >= 16)
        {
            drv_spi_flash_Read((UINT16)buf_data, bankno, srcAddr, 16);
            flow_eeprom_Write(destAddr, buf_data, 16, mode);
            if (mode == 0)
            {
                buf_rcommand[0] = *( (UINT8*)&destAddr);
                buf_rcommand[1] = *( (UINT8*)&destAddr + 1);
                drv_i2c_randomRead(I2C_MASTER_TUNER, 2, 16, buf_rcommand, buf_wdata);
            }
            else
            {
                flow_storage_memcpy(buf_wdata, (UINT8*)destAddr, 16);
            }
            for (i = 0; i < 16; i++)
            {
                if (buf_data[i] != buf_wdata[i])
                    RTD_Log(LOGGER_INFO, "eeprom write error: dest_addr = %x, src = %x, dest = %x\n", (UINT32)(destAddr + i), (UINT32)buf_data[i], (UINT32)buf_wdata[i]);
            }

            srcAddr += 16;
            destAddr += 16;
            nSize -= 16;
        }
        else
        {
            drv_spi_flash_Read((UINT16)buf_data, bankno, srcAddr, nSize);
            flow_eeprom_Write(destAddr, buf_data, nSize, mode);
            if (mode == 0)
            {
                buf_rcommand[0] = *( (UINT8*)&destAddr);
                buf_rcommand[1] = *( (UINT8*)&destAddr + 1);
                drv_i2c_randomRead(I2C_MASTER_TUNER, 2, 16, buf_rcommand, buf_wdata);
            }
            else
            {
                flow_storage_memcpy(buf_wdata, (UINT8*)destAddr, nSize);
            }
            for (i = 0; i < nSize; i++)
            {
                if (buf_data[i] != buf_wdata[i])
                    RTD_Log(LOGGER_INFO, "eeprom write error: dest_addr = %x, src = %x, dest = %x\n", (UINT32)(destAddr + i), (UINT32)buf_data[i], (UINT32)buf_wdata[i]);
            }

            nSize = 0;
        }
    }
}
#endif

UINT8 flow_eeprom_GetOffset(STORAGE_INSTANCE code* ptStorageInstance)
{
    int num,i;
    STORAGE_INSTANCE code * ptStorageMap;
    UINT8 offset;

    offset = 0;
    num = flow_storage_SystemInstanceSize();
    for (i = 0; i < num; i++)
    {
        ptStorageMap = (STORAGE_INSTANCE code*)STORAGE_SYSTEM_INSTANCE_MAP[i];
        if (ptStorageMap == ptStorageInstance)
        {
            return offset;
        }
#ifdef CONFIG_FACTORY_MODE_DATA_SAVE
        offset++;
#else
        if (ptStorageInstance->flag & 0x01)		// from xdata
            offset = offset;
        else								// from eeprom
            offset++;
#endif
    }
    RTD_Log(LOGGER_INFO, "Not Found in storage system\n");

    return 0xff;
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

    if (type >= STORAGE_STORE_TOTAL)
        return _FALSE;

    num = flow_storage_SystemInstanceSize();
    for (i = 0; i < num; i++)
    {
        ptStorageMap = (STORAGE_INSTANCE code*)STORAGE_SYSTEM_INSTANCE_MAP[i];

        // Check whether system/factory type
        if ((ptStorageMap->flag & 0x0f) != type)
        {
            continue;
        }
//		RTD_Log(LOGGER_INFO, "sync to default = %x\n", (UINT32)i);
        flow_storage_SyncDataDefault(ptStorageMap);
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
    UINT16 	eeprom_off, flash_off;
    UINT8	mode;
    UINT8 	value;
    UINT8	wbuf[16];

    eeprom_off = *((UINT16 xdata *)ptStorageInstance->INSTANCE);
    if (eeprom_off == 0xffff)
        return _FALSE;

#ifdef CONFIG_FACTORY_MODE_DATA_SAVE
    mode = 0;
#else
    if (ptStorageInstance->flag & 0x01)		// from xdata
        mode = 1;
    else								// from eeprom
        mode = 0;
#endif

    if (mode == 0)
    {
        eeprom_off = flow_eeprom_GetOffset(ptStorageInstance);
        eeprom_off += EEPROM_STORAGE_HEADER_START_ADDRESS;
        flow_eeprom_Read((UINT16)&value, eeprom_off, 1);
        if (value == 0xff)  		// Get default from flash
        {
            flash_off = ptStorageInstance->DEFAULT;
            flash_off	+= offset;
            while (size > 0)
            {
                if (size >= 16)
                {
                    drv_spi_flash_Read((UINT16)wbuf, ptStorageInstance->bank_no, flash_off, 16);
                    flow_storage_memcpy((UINT8*)data_addr, wbuf, 16);
                    size -= 16;
                    flash_off += 16;
                    data_addr += 16;
                }
                else
                {
                    drv_spi_flash_Read((UINT16)wbuf, ptStorageInstance->bank_no, flash_off, size);
                    flow_storage_memcpy((UINT8*)data_addr, wbuf, size);
                    size = 0;
                }
            }
        }
        else
        {
            eeprom_off = *((UINT16 xdata *)ptStorageInstance->INSTANCE);
            eeprom_off += offset;
            flow_eeprom_Read((UINT16)data_addr, eeprom_off, size);
        }
    }
    else
    {
#ifdef USE_STORAGE_HEADER_XDATA
        eeprom_off = *((UINT16 xdata *)ptStorageInstance->INSTANCE);
        if (*((UINT8 xdata*)eeprom_off) == 0xff)
        {
            eeprom_off = ptStorageInstance->DEFAULT;
            eeprom_off += offset;
            while (size > 0)
            {
                if (size >= 16)
                {
                    drv_spi_flash_Read((UINT16)wbuf, ptStorageInstance->bank_no, eeprom_off, 16);
                    flow_storage_memcpy((UINT8 xdata*)data_addr, (UINT8 xdata*)wbuf, 16);
                    size -= 16;
                    eeprom_off += 16;
                }
                else
                {
                    drv_spi_flash_Read((UINT16)wbuf, ptStorageInstance->bank_no, eeprom_off, size);
                    flow_storage_memcpy((UINT8 xdata*)data_addr, (UINT8 xdata*)wbuf, size);
                    size = 0;
                }
            }
        }
        else
        {
            eeprom_off += (offset + 1);
            flow_storage_memcpy((UINT8 xdata*)data_addr, (UINT8 xdata *)eeprom_off, size);
        }
#else
        eeprom_off += offset;
        flow_storage_memcpy((UINT8 xdata*)data_addr, (UINT8 xdata *)eeprom_off, size);
#endif
    }

//->W	RTD_Log(LOGGER_INFO, "INSTANCE = %x, Read address = %x, len = %x\n", (UINT32)ptStorageInstance, (UINT32)eeprom_off, (UINT32)size);

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
    UINT16  eeprom_off, flash_off;
    UINT8	mode;
    UINT8	value;
    UINT8 	wbuf[16];

    eeprom_off = *((UINT16 xdata *)ptStorageInstance->INSTANCE);
    if (eeprom_off == 0xffff)
        return _FALSE;

#ifdef CONFIG_FACTORY_MODE_DATA_SAVE
    mode = 0;
#else
    if (ptStorageInstance->flag & 0x01)		// from xdata
        mode = 1;
    else								// from eeprom
        mode = 0;
#endif
    if (mode == 0)
    {
        eeprom_off = flow_eeprom_GetOffset(ptStorageInstance);
        eeprom_off += EEPROM_STORAGE_HEADER_START_ADDRESS;
        flow_eeprom_Read((UINT16)&value, eeprom_off, 1);
        if (value == 0xff)  		// Get default from flash
        {
            flash_off = ptStorageInstance->DEFAULT;
            flash_off += offset;

            if (ptStorageInstance->flag & 0x10)
                flash_off += (ptStorageInstance->length * index);

            while (size > 0)
            {
                if (size >= 16)
                {
                    drv_spi_flash_Read((UINT16)wbuf, ptStorageInstance->bank_no, flash_off, 16);
                    flow_storage_memcpy((UINT8*)data_addr, (UINT8*)wbuf, 16);
                    size -= 16;
                    flash_off += 16;
                    data_addr += 16;
                }
                else
                {
                    drv_spi_flash_Read((UINT16)wbuf, ptStorageInstance->bank_no, flash_off, size);
                    flow_storage_memcpy((UINT8*)data_addr, (UINT8*)wbuf, size);
                    size = 0;
                }
            }
        }
        else
        {
            eeprom_off = *((UINT16 xdata *)ptStorageInstance->INSTANCE);
            eeprom_off += ((ptStorageInstance->length* index) + offset);
            flow_eeprom_Read((UINT16)data_addr, eeprom_off, size);
        }
    }
    else
    {
#ifdef USE_STORAGE_HEADER_XDATA
        eeprom_off = *((UINT16 xdata *)ptStorageInstance->INSTANCE);
        eeprom_off += ((ptStorageInstance->length + 1) * index);
        if (*((UINT8 xdata*)eeprom_off) == 0xff)
        {
            eeprom_off = ptStorageInstance->DEFAULT;
            if (ptStorageInstance->flag & 0x10)
                eeprom_off += (ptStorageInstance->length * index);

            eeprom_off += offset;
            while (size > 0)
            {
                if (size >= 16)
                {
                    drv_spi_flash_Read((UINT16)wbuf, ptStorageInstance->bank_no, eeprom_off, 16);
                    flow_storage_memcpy((UINT8*)data_addr, (UINT8*)wbuf, 16);
                    size -= 16;
                    eeprom_off += 16;
                }
                else
                {
                    drv_spi_flash_Read((UINT16)wbuf, ptStorageInstance->bank_no, eeprom_off, size);
                    flow_storage_memcpy((UINT8*)data_addr, (UINT8*)wbuf, size);
                    size = 0;
                }
            }
        }
        else
        {
            eeprom_off = *((UINT16 xdata *)ptStorageInstance->INSTANCE);
            eeprom_off += (((ptStorageInstance->length + 1) * index) + offset);
            flow_storage_memcpy((UINT8 xdata*)data_addr, (UINT8 xdata *)eeprom_off, size);
        }
#else
        eeprom_off += ((ptStorageInstance->length* index) + offset);
        flow_storage_memcpy((UINT8 xdata*)data_addr, (UINT8 xdata *)eeprom_off, size);
#endif
    }

//->W	RTD_Log(LOGGER_INFO, "Array INSTANCE = %x, Read address = %x, len = %x\n", (UINT32)ptStorageInstance, (UINT32)eeprom_off, (UINT32)size);

    return _TRUE;
}

/*============================================================================*/
/**
 * flow_storage_LoadArrayDefaultData
 * Initial data structure for rt_storage
 *
 * @param <UINT32>		{ void }
 * @param <UINT32>		{ void }
 * @return <BOOL>		{ void }
 *
 */
BOOL flow_storage_LoadArrayDefaultData(STORAGE_INSTANCE code* ptStorageInstance, UINT8 xdata* data_addr, UINT16 index, UINT8 offset, UINT8 size)
{
    UINT16  eeprom_off, flash_off;
    UINT8	mode;
    UINT8	value;
    UINT8 	wbuf[16];

    flash_off = ptStorageInstance->DEFAULT;
    if (ptStorageInstance->flag & 0x10)
        flash_off += (ptStorageInstance->length * index);

    flash_off += offset;
    drv_spi_flash_Read((UINT16)data_addr, ptStorageInstance->bank_no, eeprom_off, size);

    return _TRUE;
}


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

    UINT16	eeprom_off;
    UINT8	mode;
    UINT8	value;
    UINT16 	count, offset1;
    UINT8	wbuf[16];

    eeprom_off = *((UINT16 xdata *)ptStorageInstance->INSTANCE);
    if (eeprom_off == 0xffff)
        return _FALSE;

#ifdef CONFIG_FACTORY_MODE_DATA_SAVE
    mode = 0;
#else
    if (ptStorageInstance->flag & 0x01)		// from xdata
        mode = 1;
    else								// from eeprom
        mode = 0;
#endif
//	RTD_Log(LOGGER_INFO, "Save INSTANCE = %x, offset = %x, len = %x\n", (UINT32)*((UINT16 xdata *)ptStorageInstance->INSTANCE), (UINT32)Offset,  (UINT32)size);
    if (mode == 0)
    {
        eeprom_off = flow_eeprom_GetOffset(ptStorageInstance);
        eeprom_off += EEPROM_STORAGE_HEADER_START_ADDRESS;
        flow_eeprom_Read((UINT16)&value, eeprom_off, 1);
//		RTD_Log(LOGGER_INFO, "value = %x\n", (UINT32)value);
        if (value == 0xff)  		// Get default from flash
        {
            count = ptStorageInstance->length;
            offset1 = ptStorageInstance->DEFAULT;
            eeprom_off = *((UINT16 xdata *)ptStorageInstance->INSTANCE);
            while (count > 0)
            {
                if (count >= 16)
                {
                    drv_spi_flash_Read((UINT16)wbuf, ptStorageInstance->bank_no, offset1, 16);
                    flow_eeprom_Write(eeprom_off, wbuf, 16);
                    count -= 16;
                    eeprom_off += 16;
                    offset1 += 16;
                }
                else
                {
                    drv_spi_flash_Read((UINT16)wbuf, ptStorageInstance->bank_no, offset1, count);
                    flow_eeprom_Write(eeprom_off, wbuf, count);
                    count = 0;
                }
            }
            eeprom_off = (*((UINT16 xdata *)ptStorageInstance->INSTANCE) + Offset);
            flow_eeprom_Write(eeprom_off, data_addr, size);

            value = 0x00;
            eeprom_off = flow_eeprom_GetOffset(ptStorageInstance);
            eeprom_off += EEPROM_STORAGE_HEADER_START_ADDRESS;
            flow_eeprom_Write((UINT16)eeprom_off, &value, 1);
        }
        else
        {
            eeprom_off = (*((UINT16 xdata *)ptStorageInstance->INSTANCE) + Offset);
            flow_eeprom_Write(eeprom_off, data_addr, size);
        }
    }
    else
    {
#ifdef USE_STORAGE_HEADER_XDATA
        eeprom_off = *((UINT16 xdata *)ptStorageInstance->INSTANCE);
        if (*((UINT8 xdata*)eeprom_off) == 0xff)
        {
            offset1 = ptStorageInstance->DEFAULT;
            count = ptStorageInstance->length;
            eeprom_off += 1;
//			RTD_Log(LOGGER_INFO, "offset = %x, offset1 = %x, num = %x, size = %x\n", (UINT32)offset, (UINT32)offset1, (UINT32)size);
            while (count > 0)
            {
                if (count >= 16)
                {
                    drv_spi_flash_Read((UINT16)wbuf, ptStorageInstance->bank_no, offset1, 16);
                    flow_storage_memcpy((UINT8 xdata*)eeprom_off, (UINT8 xdata*)wbuf, 16);
                    count -= 16;
                    eeprom_off += 16;
                    offset1 += 16;
                }
                else
                {
                    drv_spi_flash_Read((UINT16)wbuf, ptStorageInstance->bank_no, offset1, count);
                    flow_storage_memcpy((UINT8*)eeprom_off, (UINT8*)wbuf, count);
                    count = 0;
                }
            }
            eeprom_off = 	*((UINT16 xdata *)ptStorageInstance->INSTANCE) + Offset + 1;
            flow_storage_memcpy((UINT8 xdata*)eeprom_off, (UINT8 xdata *)data_addr, size);
            eeprom_off = 	*((UINT16 xdata *)ptStorageInstance->INSTANCE);
            *((UINT8 xdata*)eeprom_off) = 0x00;
        }
        else
        {
            eeprom_off = 	*((UINT16 xdata *)ptStorageInstance->INSTANCE) + Offset + 1;
            flow_storage_memcpy((UINT8 xdata*)eeprom_off, (UINT8 xdata *)data_addr, size);
        }
#else
        eeprom_off = 	*((UINT16 xdata *)ptStorageInstance->INSTANCE) + Offset;
        flow_storage_memcpy((UINT8 xdata*)eeprom_off, (UINT8 xdata *)data_addr, size);
#endif
    }

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
    UINT16 eeprom_off;
    UINT8	mode;
    UINT8	value;
    UINT8	wbuf[16];
    UINT16  count, offset1, num;

    eeprom_off = *((UINT16 xdata *)ptStorageInstance->INSTANCE);
    if (eeprom_off == 0xffff)
        return _FALSE;
#ifdef CONFIG_FACTORY_MODE_DATA_SAVE
    mode = 0;
#else
    if (ptStorageInstance->flag & 0x01)		// from xdata
        mode = 1;
    else								// from eeprom
        mode = 0;
#endif

//	RTD_Log(LOGGER_INFO, "Array INSTANCE = %x, index = %x, offset = %x, len = %x\n", (UINT32)*((UINT16 xdata *)ptStorageInstance->INSTANCE), (UINT32)index, (UINT32)offset,  (UINT32)size);
    if (mode == 0)
    {
        eeprom_off = flow_eeprom_GetOffset(ptStorageInstance);
        eeprom_off += EEPROM_STORAGE_HEADER_START_ADDRESS;
        flow_eeprom_Read((UINT16)&value, eeprom_off, 1);
        if (value == 0xff)  		// Get default from flash
        {
            if (ptStorageInstance->flag & 0x10)
            {
                eeprom_off = *((UINT16 xdata *)ptStorageInstance->INSTANCE);
                for (num = 0; num < ptStorageInstance->Array_num; num++)
                {
                    count = ptStorageInstance->length;
                    offset1 = ptStorageInstance->DEFAULT;
                    offset1 += (ptStorageInstance->length * num);
                    // always kick watchdog, even it is not enabled
                    //tick system WD, to avoid system reset
                    flow_timer_Reset_WatchDog();
                    while (count > 0)
                    {
                        if (count >= 16)
                        {
                            drv_spi_flash_Read((UINT16)wbuf, ptStorageInstance->bank_no, offset1, 16);
                            flow_eeprom_Write(eeprom_off, wbuf, 16);
                            count -= 16;
                            eeprom_off += 16;
                            offset1 += 16;
                        }
                        else
                        {
                            drv_spi_flash_Read((UINT16)wbuf, ptStorageInstance->bank_no, offset1, count);
                            flow_eeprom_Write(eeprom_off, wbuf, count);
                            eeprom_off += count;
                            offset1 += count;
                            count = 0;
                        }
                    }
                }
            }
            else
            {
                eeprom_off = (*((UINT16 xdata *)ptStorageInstance->INSTANCE) + (index * ptStorageInstance->length));
                count = ptStorageInstance->length;
                offset1 = ptStorageInstance->DEFAULT;
                while (count > 0)
                {
                    if (count >= 16)
                    {
                        drv_spi_flash_Read((UINT16)wbuf, ptStorageInstance->bank_no, offset1, 16);
                        flow_eeprom_Write(eeprom_off, wbuf, 16);
                        count -= 16;
                        eeprom_off += 16;
                        offset1 += 16;
                    }
                    else
                    {
                        drv_spi_flash_Read((UINT16)wbuf, ptStorageInstance->bank_no, offset1, count);
                        flow_eeprom_Write(eeprom_off, wbuf, count);
                        count = 0;
                    }
                }
            }

            eeprom_off = *((UINT16 xdata *)ptStorageInstance->INSTANCE);
            eeprom_off += ((ptStorageInstance->length * index) + offset);
            flow_eeprom_Write(eeprom_off, data_addr, size);

            value = 0x00;
            eeprom_off = flow_eeprom_GetOffset(ptStorageInstance);
            eeprom_off += EEPROM_STORAGE_HEADER_START_ADDRESS;
            flow_eeprom_Write((UINT16)eeprom_off, &value, 1);

        }
        else
        {
            eeprom_off = *((UINT16 xdata *)ptStorageInstance->INSTANCE);
            eeprom_off += ((ptStorageInstance->length * index) + offset);
            flow_eeprom_Write(eeprom_off, data_addr, size);
        }
    }
    else
    {
#ifdef USE_STORAGE_HEADER_XDATA
        eeprom_off = *((UINT16 xdata *)ptStorageInstance->INSTANCE);
        eeprom_off += ((ptStorageInstance->length + 1) * index);
        if (*((UINT8 xdata*)eeprom_off) == 0xff)
        {
            offset1 = ptStorageInstance->DEFAULT;
            if (ptStorageInstance->flag & 0x10)
                offset1 += (ptStorageInstance->length * index);

            count = ptStorageInstance->length;
            eeprom_off += 1;
            while (count > 0)
            {
                if (count >= 16)
                {
                    drv_spi_flash_Read((UINT16)wbuf, ptStorageInstance->bank_no, offset1, 16);
                    flow_storage_memcpy((UINT8*)eeprom_off, (UINT8*)wbuf, 16);
                    size -= 16;
                    offset1 += 16;
                    eeprom_off += 16;
                }
                else
                {
                    drv_spi_flash_Read((UINT16)wbuf, ptStorageInstance->bank_no, offset1, count);
                    flow_storage_memcpy((UINT8*)eeprom_off, (UINT8*)wbuf, count);
                    size = 0;
                }
            }
            eeprom_off = *((UINT16 xdata *)ptStorageInstance->INSTANCE);
            eeprom_off += (((ptStorageInstance->length  + 1) * index) + offset);
            flow_storage_memcpy((UINT8 xdata*)eeprom_off, (UINT8 xdata *)data_addr, size);

            eeprom_off = *((UINT16 xdata *)ptStorageInstance->INSTANCE);
            eeprom_off += ((ptStorageInstance->length  + 1) * index);
            *((UINT8 xdata*)eeprom_off) = 0x00;
        }
        else
        {
            eeprom_off = *((UINT16 xdata *)ptStorageInstance->INSTANCE);
            eeprom_off += (((ptStorageInstance->length + 1)* index) + offset);
            flow_storage_memcpy((UINT8 xdata*)eeprom_off, (UINT8 xdata *)data_addr, size);
        }
#else
        eeprom_off = *((UINT16 xdata *)ptStorageInstance->INSTANCE);
        eeprom_off += ((ptStorageInstance->length * index) + offset);
        flow_storage_memcpy((UINT8 xdata*)eeprom_off, (UINT8 xdata *)data_addr, size);
//		RTD_Log(LOGGER_INFO, "1.Target address = %x, len = %x\n", (UINT32)eeprom_off, (UINT32)size);
#endif
    }

    return _TRUE;
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
void drv_i2c_Init(void);
BOOL flow_storage_Init(void)
{
    volatile UINT16 fFirstRunVal;
    int i, num;
    BOOL UsedFlag;
    STORAGE_INSTANCE code * ptStorageMap;
    UINT16	offset;
    UINT8 value;

#ifndef FACTORY_MODE_DATA_SAVE
    UINT16 offset1;
    offset1 = EEPROM_STORAGE_XDATA_START_ADDRESS;
#endif

    // Initial I2c
    rtdf_setBitsb(SYS_SRST_4_0x2ca4, _BIT0);
    rtdf_setBitsb(SYS_CLKEN_3_0x2cc3, _BIT0);
    drv_i2c_PowerOn(_ON);
    RTD_Log(LOGGER_INFO, "drv_i2c_Init in ee_storage\n");
    drv_i2c_Init();

    // Initialize instance
    num = flow_storage_SystemInstanceSize();
    offset = EEPROM_STORAGE_EEPROM_START_ADDRESS;
    for (i = 0; i < num; i++)
    {
        ptStorageMap = (STORAGE_INSTANCE code*)STORAGE_SYSTEM_INSTANCE_MAP[i];
#ifdef FACTORY_MODE_DATA_SAVE
        *((UINT16 xdata *)ptStorageMap->INSTANCE) = offset;
#else
        if (ptStorageMap->flag & 0x01)
            *((UINT16 xdata *)ptStorageMap->INSTANCE) = offset1;
        else
            *((UINT16 xdata *)ptStorageMap->INSTANCE) = offset;
#endif
//		RTD_Log(LOGGER_INFO, "%x - INSTANCE=%x\n", (UINT32)i, (UINT32)*((UINT16 xdata *)ptStorageMap->INSTANCE) );

        if (ptStorageMap->type == 0)
        {
#ifdef FACTORY_MODE_DATA_SAVE
            offset += ptStorageMap->length;
#else
            if (ptStorageMap->flag & 0x01)
#ifndef	USE_STORAGE_HEADER_XDATA
                offset1 += ptStorageMap->length;
#else
            offset1 += (ptStorageMap->length + 1);
#endif
            else
                offset += ptStorageMap->length;
#endif
        }
        else
        {
#ifdef FACTORY_MODE_DATA_SAVE
            offset += (ptStorageMap->length * ptStorageMap->Array_num);
#else
            if (ptStorageMap->flag & 0x01)
#ifndef	USE_STORAGE_HEADER_XDATA
                offset1 += (ptStorageMap->length * ptStorageMap->Array_num);
#else
            offset1 += (ptStorageMap->length * (ptStorageMap->Array_num + 1));
#endif
            else
                offset += (ptStorageMap->length * ptStorageMap->Array_num);
#endif
        }
    }


    // Get CStorageFirstRun from flash in order to check whether first run or not
    UsedFlag = 1;
    if (!flow_eeprom_Read((UINT16)((UINT8*)&fFirstRunVal), EEPROM_FIRSTRUN_START_ADDRESS, sizeof(UINT16)))
        return _FALSE;

    RTD_Log(LOGGER_INFO, "Get firstRunVal :%x\n", (UINT32)fFirstRunVal);
    if (fFirstRunVal != 0)
    {
        UsedFlag = 0;
    }
    else
    {
        if (!flow_eeprom_Read((UINT16)((UINT8*)&fFirstRunVal), EEPROM_VERSION_START_ADDRESS, sizeof(UINT16)))
            return _FALSE;

        RTD_Log(LOGGER_INFO, "Get Version :%x\n", (UINT32)fFirstRunVal);
        if (fFirstRunVal != EEPROM_STORAGE_VERSION_CODE)
        {
//		if(fFirstRunVal != (fFirstRunVal + 1)) {
            UsedFlag = 0;
        }
    }
    if (!UsedFlag)
    {
        // Set CStorageFirstRun to 0 specify not first run
        fFirstRunVal = 0;
        flow_eeprom_Write(EEPROM_FIRSTRUN_START_ADDRESS, (UINT8*)&fFirstRunVal, sizeof(UINT16));
//->w		RTD_Log(LOGGER_INFO, "Write FirstRunVal :%x\n", (UINT32)fFirstRunVal);

        fFirstRunVal = 0xff;
        flow_eeprom_Read((UINT16)((UINT8*)&fFirstRunVal), EEPROM_FIRSTRUN_START_ADDRESS, sizeof(UINT16));
//->w		RTD_Log(LOGGER_INFO, "Read FirstRunVal :%x\n", (UINT32)fFirstRunVal);

        fFirstRunVal = EEPROM_STORAGE_VERSION_CODE;
        flow_eeprom_Write(EEPROM_VERSION_START_ADDRESS, (UINT8*)&fFirstRunVal, sizeof(UINT16));
//->w		RTD_Log(LOGGER_INFO, "Write Version :%x\n", (UINT32)fFirstRunVal);

        fFirstRunVal = 0xff;
        flow_eeprom_Read((UINT16)((UINT8*)&fFirstRunVal), EEPROM_FIRSTRUN_START_ADDRESS, sizeof(UINT16));
//->w		RTD_Log(LOGGER_INFO, "Read Version :%x\n", (UINT32)fFirstRunVal);

        offset = EEPROM_STORAGE_HEADER_START_ADDRESS;
        value = 0xff;
        while (offset < EEPROM_STORAGE_EEPROM_START_ADDRESS)
        {
//->w			RTD_Log(LOGGER_INFO, "Write 0xff in addr = %x\n", (UINT32)offset);
            flow_eeprom_Write(offset, (UINT8*)&value, sizeof(UINT8));
            offset++;
        }
#ifndef CONFIG_FACTORY_MODE_DATA_SAVE
        // factory in stoarge all set to default
        flow_storage_LoadDefault(STORAGE_STORE_FACTORY);
#endif
    }
#ifndef CONFIG_FACTORY_MODE_DATA_SAVE
    else
    {
        // factory in stoarge all set to default
        flow_storage_LoadDefault(STORAGE_STORE_FACTORY);
    }
#endif

    RTD_Log(LOGGER_INFO, "flow_storage_Init Complete\n");

    return _TRUE;
}

BOOL flow_storage_SyncDataDefault(STORAGE_INSTANCE code* ptStorageInstance)
{
#ifndef	USE_STORAGE_HEADER_XDATA
    UINT16 offset1;
    UINT8 wbuf[16];
#endif
    UINT16 offset;
    UINT8 value;
    UINT8 mode;
    int i,size;

    if (ptStorageInstance == NULL)
        return _FALSE;

    offset = *((UINT16 xdata *)ptStorageInstance->INSTANCE);
    if (offset  == 0xffff)
        return _FALSE;

#ifdef CONFIG_FACTORY_MODE_DATA_SAVE
    mode = 0;
#else
    if (ptStorageInstance->flag & 0x01)		// from xdata
        mode = 1;
    else								// from eeprom
        mode = 0;
#endif

    if (mode == 0)
    {
        offset = flow_eeprom_GetOffset(ptStorageInstance);
        offset += EEPROM_STORAGE_HEADER_START_ADDRESS;
        value = 0xff;
        flow_eeprom_Write(offset, &value, 1);
    }
    else
    {
        if (ptStorageInstance->type == 0)
        {
            offset = *((UINT16 xdata *)ptStorageInstance->INSTANCE);
#ifdef	USE_STORAGE_HEADER_XDATA
            *((UINT8 xdata*)offset) = 0xff;
#else
            offset1 = ptStorageInstance->DEFAULT;
            size = ptStorageInstance->length;
//			RTD_Log(LOGGER_INFO, "offset = %x, offset1 = %x, num = %x, size = %x\n", (UINT32)offset, (UINT32)offset1, (UINT32)size);
            while (size > 0)
            {
                if (size >= 16)
                {
                    drv_spi_flash_Read((UINT16)wbuf, ptStorageInstance->bank_no, offset1, 16);
                    flow_storage_memcpy((UINT8 xdata*)offset, (UINT8 xdata*)wbuf, 16);
                    size -= 16;
                    offset += 16;
                    offset1 += 16;
                }
                else
                {
                    drv_spi_flash_Read((UINT16)wbuf, ptStorageInstance->bank_no, offset1, size);
                    flow_storage_memcpy((UINT8*)offset, (UINT8*)wbuf, size);
                    size = 0;
                }
            }
#endif
        }
        else
        {
            offset = *((UINT16 xdata *)ptStorageInstance->INSTANCE);
#ifdef	USE_STORAGE_HEADER_XDATA
            size = ptStorageInstance->length;
            for (i = 0; i < ptStorageInstance->Array_num; i++)
            {
                *((UINT8 xdata*)offset) = 0xff;
                offset += (size + 1);
            }
#else
            for (i = 0; i < ptStorageInstance->Array_num; i++)
            {
                size = ptStorageInstance->length;
                offset1 = ptStorageInstance->DEFAULT;
                if (ptStorageInstance->flag & 0x10)
                    offset1 += (ptStorageInstance->length * i);

//				RTD_Log(LOGGER_INFO, "flag = %x, offset = %x, offset1 = %x, num = %x, size = %x\n", (UINT32)ptStorageInstance->flag, (UINT32)offset, (UINT32)offset1, (UINT32)ptStorageInstance->Array_num, (UINT32)size);
                while (size > 0)
                {
                    if (size >= 16)
                    {
                        drv_spi_flash_Read((UINT16)wbuf, ptStorageInstance->bank_no, offset1, 16);
                        flow_storage_memcpy((UINT8*)offset, (UINT8*)wbuf, 16);
                        size -= 16;
                        offset += 16;
                        offset1 += 16;
                    }
                    else
                    {
                        drv_spi_flash_Read((UINT16)wbuf, ptStorageInstance->bank_no, offset1, size);
                        flow_storage_memcpy((UINT8*)offset, (UINT8*)wbuf, size);
                        offset += size;
                        size = 0;
                    }
                }
            }
#endif
        }
    }

    return _TRUE;
}

