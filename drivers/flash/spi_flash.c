/*=============================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2005
  * All rights reserved.
  * ============================================ */

/*================- File Description ================= */
/**
 * @file
 * 	This file is irrc process function.
 *
 * @author	$Author$
 * @date	$Date$
 * @version 	$Revision$$
 * @ingroup 	isr
 */

/**
* @addtogroup spi_flash
* @{
*/
#include "flash/spi_flash.h"
#include "spi_dma.h"
#include "reg_def.h"
#include "rosprintf.h"
#include "imem.h"
#include "sparrow_utility.h"
#include "reg52.h"

// Activate the write operation for flash
void drv_flash_Write(UINT8 bankno, UINT16 StartAddress, UINT8* sBuf, UINT16 nSize);
UINT8 drv_spi_CheckWrite(void);
extern UINT8 fCPUSpeed;
sfr SFR_SPC_FNC_0x8e = 0x8e;

#ifdef CONFIG_STORAGE_ACCESS_FLASH
void drv_spi_flash_Erase(UINT8 bankno, UINT16 StartAddress, UINT16 Length)
{
    UINT16 nAddr;
    UINT8  getTempVal;
    bit	tmpBit;

    tmpBit = EA;
    nAddr = StartAddress & 0xf000;

    RTD_Log(LOGGER_INFO, "ebank = %x, eaddress: %x, elength = %x\n", (UINT32)bankno, (UINT32)StartAddress, (UINT32)Length);
    EA = 0;
    do
    {
//		RTD_Log(LOGGER_INFO, "flash ease bank = %x,  startaddress: %x, length = 4096\n", (UINT32)bankno, (UINT32)nAddr);
        getTempVal = SFR_SPC_FNC_0x8e;
        SFR_SPC_FNC_0x8e = 0x02;

        // set start address for Erease
        rtd_outb(MCU_SPI_FLASH_FLASH_PROG_ISP0_0xff64, bankno);
        rtd_outb(MCU_SPI_FLASH_FLASH_PROG_ISP1_0xff65, *((UINT8*)&nAddr));
        rtd_outb(MCU_SPI_FLASH_FLASH_PROG_ISP2_0xff66, 0);

        //Command Erase
        rtd_outb(MCU_SPI_FLASH_COMMON_INST_EN_0xff60, 0xb8);		// should be customer-defined

        //Blk Er OP code
        rtd_outb(MCU_SPI_FLASH_COMMON_OP_CODE_0xff61, 0x20);		// should be customer-defined

        //Common instruction enable
        rtd_outb(MCU_SPI_FLASH_COMMON_INST_EN_0xff60, 0xb9);		// should be customer-defined

        // wait for Erase finish
        while (rtd_inb(MCU_SPI_FLASH_COMMON_INST_EN_0xff60)  & _BIT0);

        rtd_outb(COMMON_OP_CODE_0xff61, 0xff);

        SFR_SPC_FNC_0x8e = getTempVal;

        // Overflow
        if ((nAddr + 4096) < nAddr)
            break;

        nAddr += 4096;
    }
    while (nAddr < (StartAddress + Length - 1));

    EA = tmpBit;
}
#endif

void drv_spi_isr_flash_Erase(UINT8 bankno, UINT16 StartAddress, UINT16 Length)
{
    UINT16 nAddr;
    UINT8  getTempVal;

    nAddr = StartAddress & 0xf000;
    do
    {
        getTempVal = SFR_SPC_FNC_0x8e;
        SFR_SPC_FNC_0x8e = 0x02;

        // set start address for Erease
        rtd_isr_outb(MCU_SPI_FLASH_FLASH_PROG_ISP0_0xff64, bankno);
        rtd_isr_outb(MCU_SPI_FLASH_FLASH_PROG_ISP1_0xff65, *((UINT8*)&nAddr));
        rtd_isr_outb(MCU_SPI_FLASH_FLASH_PROG_ISP2_0xff66, 0);

        //Command Erase
        rtd_isr_outb(MCU_SPI_FLASH_COMMON_INST_EN_0xff60, 0xb8);		// should be customer-defined

        //Blk Er OP code
        rtd_isr_outb(MCU_SPI_FLASH_COMMON_OP_CODE_0xff61, 0x20);		// should be customer-defined

        //Common instruction enable
        rtd_isr_outb(MCU_SPI_FLASH_COMMON_INST_EN_0xff60, 0xb9);		// should be customer-defined

        // wait for Erase finish
        while (rtd_isr_inb(MCU_SPI_FLASH_COMMON_INST_EN_0xff60)  & _BIT0);

        rtd_outb(COMMON_OP_CODE_0xff61, 0xff);

        SFR_SPC_FNC_0x8e = getTempVal;

        // Overflow
        if ((nAddr + 4096) < nAddr)
            break;

        nAddr += 4096;
    }
    while (nAddr < (StartAddress + Length - 1));
}

#ifdef CONFIG_STORAGE_ACCESS_FLASH
BOOL drv_spi_flash_CheckReady(void)
{
    while (rtd_inb(MCU_SPI_FLASH_COMMON_INST_EN_0xff60) & _BIT0);

    return _TRUE;
}
#endif

#ifdef CONFIG_STORAGE_ACCESS_FLASH
/*============================================================================*/
/**
 * drv_spi_flash_copy
 * copy src in flash to dest in flash
 *
 * @param <UINT32>		{ Source address }
 * @param <UINT32>		{ Destination address }
 * @param <UINT16>		{ the copy number }
 * @return <BOOL>		{ _TRUE if success, otheriwse _FALSE }
 *
 */
void drv_spi_flash_copy(UINT32 srcAddr, UINT32 destAddr, UINT16 count)
{
    UINT16 i;
    UINT8 buf_data[8];
    UINT8 bankno;
    i = count;
//	RTD_Log(LOGGER_INFO, "flash copy internal src = %x, dst = %x, size = %x\n", (UINT32)srcAddr, (UINT32)destAddr, (UINT32)count);
    bankno = *((UINT8*)&srcAddr + 1);
    while (i > 0)
    {
        if (i > 8)
        {
            drv_spi_flash_Read((UINT16)buf_data, bankno, srcAddr, 8);
            drv_spi_flash_Write(bankno, destAddr, buf_data, 8);
            srcAddr += 8;
            destAddr += 8;
            i -= 8;
        }
        else
        {
            drv_spi_flash_Read((UINT16)buf_data, bankno, srcAddr, i);
            drv_spi_flash_Write(bankno, destAddr, buf_data, i);
            srcAddr += i;
            destAddr += i;
            i = 0;
        }
    }
//->ok	RTD_Log(LOGGER_INFO, "flash copy ok\n");
}
#endif

