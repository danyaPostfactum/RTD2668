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

#ifndef  _SPI_DMA_H_
#define  _SPI_DMA_H_

#include "sparrow.h"

#define SPI_DMA_TRANSFER_MODE_6093 		0		// write 8bit (6093) register only
#define SPI_DMA_TRANSFER_MODE_DMEM 		1		// write dmem only
#define SPI_DMA_TRANSFER_MODE_REGISTER	2		// write 32 bits register / system register
// BOOL drv_spi_dma_memcpy(UINT16 dest_addr, UINT32 src_addr, UINT16 len, UINT8 mode);
BOOL drv_spi_dma_memcpy(UINT16 dest_addr, UINT8 nBank, UINT16 src_addr, UINT16 len, UINT8 mode);
BOOL drv_spi_isr_dma_memcpy(UINT16 dest_addr, UINT8 nBank, UINT16 src_addr, UINT16 len, UINT8 mode);
// mode : 0 inc address
//		  1 non-inc
BOOL drv_spi_dma_6093cpy(UINT8 dest_addr, UINT8 bank, UINT16 src_addr, UINT16 len, UINT8 mode);

#endif // #ifndef  _SPI_DMA_H_
