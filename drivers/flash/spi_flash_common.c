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
#include "reg_def.h"
#include "reg52.h"
#include "timer_event.h"

extern void drv_spi_flash_DisableProtect(void);
extern void timerevent_table_flash_protect(void);

UINT8 Flash_protect_on; //flash 保护标志位
sfr SFR_SPC_FNC_0x8f = 0x8f;
// sfr SFR_SPC_FNC_0x8e = 0x8e;
//void drv_spi_flash_Write(UINT8 bankno, UINT16 StartAddress, UINT8* sBuf, UINT16 nSize)
void drv_flash_Write(UINT8 bankno, UINT16 StartAddress, UINT8* sBuf, UINT16 nSize)
{
    UINT8 tmpBank;
    bit	tmpBit;

    tmpBit = EA;
    EA = 0;

	drv_flash_protect(_FALSE);
    rtd_outb(MCU_SPI_PROGRAM_OP_CODE_0xff6d, 0x02);

    // store old Bank
    tmpBank = rtd_inb(MCU_PBANK_SWITCH_0xFFFF);

    while (nSize > 0)
    {
        // Bank switch to
        rtd_outb(MCU_PBANK_SWITCH_0xFFFF, bankno);

        // Enable Flash write
        SFR_SPC_FNC_0x8f |= _BIT0;

        // Write to Flash
        rtd_outb(StartAddress, *sBuf);

        // Disable Flash write
        SFR_SPC_FNC_0x8f &= ~_BIT0;

        // Restore old bank
        rtd_outb(MCU_PBANK_SWITCH_0xFFFF, tmpBank);

        // Pointer to Next address
        StartAddress++;
        sBuf++;
        nSize--;
    }

    // Restore old bank
    rtd_outb(MCU_PBANK_SWITCH_0xFFFF, tmpBank);

    rtd_outb(MCU_SPI_PROGRAM_OP_CODE_0xff6d, 0xff);

    EA = tmpBit;

    fw_timer_event_ReactiveTimerEvent(SEC(0.5),timerevent_table_flash_protect);
}

/*********************************************************************
** 函数名称: drv_flash_protect(bit enable)
** 功能描述: flash写保护
** 输　  入: bit enable
** 输　  出: 无 
**********************************************************************/

void drv_flash_protect(bit enable)
{
	rtd_outb(MCU_SPI_PROGRAM_OP_CODE_0xff6d,0x02);
	rtd_outb(MCU_SPI_FLASH_COMMON_INST_EN_0xff60,0x68);
	rtd_outb(MCU_SPI_FLASH_COMMON_OP_CODE_0xff61,0x01);
	if(enable == _ON)
	{
		Flash_protect_on = _TRUE;
		rtd_outb(MCU_SPI_FLASH_FLASH_PROG_ISP0_0xff64,0x30);//低512k flash 保护
	}
	else
	{
		Flash_protect_on = _FALSE;
		rtd_outb(MCU_SPI_FLASH_FLASH_PROG_ISP0_0xff64,0x00);//低512k flash 保护
	}
	rtd_outb(MCU_SPI_FLASH_COMMON_INST_EN_0xff60,0x69);

	rtd_outb(MCU_SPI_PROGRAM_OP_CODE_0xff6d,0xff);
	rtd_outb(MCU_SPI_FLASH_COMMON_OP_CODE_0xff61,0xff);
}

#if 0
void drv_spi_common_flash_erase(UINT8 bankno, UINT8 sectAddr)
{
    EA = 0;
    // set start address for Erease
    rtd_outb(MCU_SPI_FLASH_FLASH_PROG_ISP0_0xff64, bankno);
    rtd_outb(MCU_SPI_FLASH_FLASH_PROG_ISP1_0xff65, sectAddr);
    rtd_outb(MCU_SPI_FLASH_FLASH_PROG_ISP2_0xff66, 0);

    //Command Erase
    rtd_outb(MCU_SPI_FLASH_COMMON_INST_EN_0xff60, 0xb8);		// should be customer-defined

    //Blk Er OP code
    rtd_outb(MCU_SPI_FLASH_COMMON_OP_CODE_0xff61, 0x20);		// should be customer-defined

    //Common instruction enable
    rtd_outb(MCU_SPI_FLASH_COMMON_INST_EN_0xff60, 0xb9);		// should be customer-defined

    // wait for Erase finish
    while (rtd_inb(MCU_SPI_FLASH_COMMON_INST_EN_0xff60)  & _BIT0);
    EA = 1;
}
#endif

