/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2009
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

#include "spi_dma.h"
#include "reg_def.h"
// mode : 0 inc address
//		  1 non-inc
BOOL drv_spi_dma_6093cpy(UINT8 dest_addr, UINT8 bank, UINT16 src_addr, UINT16 len, UINT8 mode)
{
    if ((mode > 1) || (len == 0))
        return _FALSE;

    if (mode)
        rtdf_setBitsb(0xfff3, _BIT5);
    else
        rtdf_clearBitsb(0xfff3, _BIT5);

    return drv_spi_dma_memcpy(dest_addr, bank, src_addr, len, SPI_DMA_TRANSFER_MODE_6093);
}

#if 1
BOOL drv_spi_dma_memcpy(UINT16 dest_addr, UINT8 nBank, UINT16 src_addr, UINT16 len, UINT8 mode)
{
    if ((mode > 2) || (len == 0))
        return _FALSE;

    // Set bank in flash
    rtd_outb(SPI_DMA_BANK_0x2c0b, nBank);

    // Set address in flash
    rtd_outb(SPI_DMA_FLASH_LSB_0x2c09, *((UINT8*)&src_addr + 1));
    rtd_outb(SPI_DMA_FLASH_MSB_0x2c0a, *(UINT8*)&src_addr);

    // Set length to transfer
    rtd_outb(SPI_DMA_LEN_LSB_0x2c0d, *((UINT8*)&len + 1));
    rtd_outb(SPI_DMA_LEN_MSB_0x2c0e, *(UINT8*)&len);

    // Set dest address
    rtd_outb(SPI_DMA_DEST_LSB_0x2c0f, *((UINT8*)&dest_addr + 1));
    rtd_outb(SPI_DMA_DEST_MSB_0x2c10, *(UINT8*)&dest_addr);

    // Set transfer mode
    rtdf_maskb(SPI_DMA_CTRL_0x2c0c, ~(_BIT2 | _BIT3 | _BIT1 | _BIT0), (UINT8)((mode & 0x3) << 2));

    // Trigger dma function
    rtdf_setBitsb(SPI_DMA_CTRL_0x2c0c, _BIT0);

    // Wait for done
    while (!(rtdf_inb(SPI_DMA_CTRL_0x2c0c) & _BIT1));

    return _TRUE;
}

BOOL drv_spi_isr_dma_memcpy(UINT16 dest_addr, UINT8 nBank, UINT16 src_addr, UINT16 len, UINT8 mode)
{
    if ((mode > 2) || (len == 0))
        return _FALSE;

    // Set bank in flash
    rtd_outb(SPI_DMA_BANK_0x2c0b, nBank);

    // Set address in flash
    rtd_outb(SPI_DMA_FLASH_LSB_0x2c09, *((UINT8*)&src_addr + 1));
    rtd_outb(SPI_DMA_FLASH_MSB_0x2c0a, *(UINT8*)&src_addr);

    // Set length to transfer
    rtd_outb(SPI_DMA_LEN_LSB_0x2c0d, *((UINT8*)&len + 1));
    rtd_outb(SPI_DMA_LEN_MSB_0x2c0e, *(UINT8*)&len);

    // Set dest address
    rtd_outb(SPI_DMA_DEST_LSB_0x2c0f, *((UINT8*)&dest_addr + 1));
    rtd_outb(SPI_DMA_DEST_MSB_0x2c10, *(UINT8*)&dest_addr);

    // Set transfer mode
    rtdf_maskb(SPI_DMA_CTRL_0x2c0c, ~(_BIT2 | _BIT3 | _BIT1 | _BIT0), (UINT8)((mode & 0x3) << 2));

    // Trigger dma function
    rtdf_setBitsb(SPI_DMA_CTRL_0x2c0c, _BIT0);

    // Wait for done
    while (!(rtdf_inb(SPI_DMA_CTRL_0x2c0c) & _BIT1));

    return _TRUE;
}

#else
BOOL drv_spi_dma_memcpy(UINT16 dest_addr, UINT32 src_addr, UINT16 len, UINT8 mode)
{
    if ((mode > 2) || (len == 0))
        return _FALSE;

    // Set bank in flash
    rtd_outb(SPI_DMA_BANK_0x2c0b, (UINT8)((src_addr & 0x00ff0000) >> 16));

    // Set address in flash
    rtd_outb(SPI_DMA_FLASH_LSB_0x2c09, (UINT8)(src_addr & 0x000000ff));
    rtd_outb(SPI_DMA_FLASH_MSB_0x2c0a, (UINT8)((src_addr & 0x0000ff00) >> 8));

    // Set length to transfer
    rtd_outb(SPI_DMA_LEN_LSB_0x2c0d, (UINT8)(len & 0x00ff));
    rtd_outb(SPI_DMA_LEN_MSB_0x2c0e, (UINT8)((len & 0xff00) >> 8));

    // Set dest address
    rtd_outb(SPI_DMA_DEST_LSB_0x2c0f, (UINT8)(dest_addr & 0x00ff));
    rtd_outb(SPI_DMA_DEST_MSB_0x2c10, (UINT8)((dest_addr & 0xff00) >> 8));

    // Set transfer mode
    rtdf_maskb(SPI_DMA_CTRL_0x2c0c, ~(_BIT2 | _BIT3 | _BIT1 | _BIT0), (UINT8)((mode & 0x3) << 2));

    // Trigger dma function
    rtdf_setBitsb(SPI_DMA_CTRL_0x2c0c, _BIT0);

    // Wait for done
    while (!(rtdf_inb(SPI_DMA_CTRL_0x2c0c) & _BIT1));

    return _TRUE;
}
#endif

