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
#include "imem.h"
#include "reg_def.h"
#include "reg52.h"

drv_mem_SetShareMode(UINT8 mode)
{
    UINT8 val;
    mode &= 0x03;
    val = mode;
    val = val << 2;
    val |= mode;
    rtd_outb(SYS_SHARE_MEM_0x2da1, val);
}

#ifdef CONFIG_TTX_SUPPORT

void drv_imem4k_Start(UINT8 bank_num, UINT16 imem_addr)
{
//	rtd_outb( SYS_SHARE_MEM_0x2da1, 0x00 );
    // Set sys share memory to mode - 0
    drv_mem_SetShareMode(MEM_SHARE_MODE_0);

    // Disable 4k-iMem map
    drv_imem4k_Stop();

    // Set start address in 4k-Imem
    rtdf_maskb(IMEM4K_ADDR_0x2c00, ~(_BIT3 | _BIT2 | _BIT1 | _BIT0),  (UINT8)((imem_addr & 0xf000) >> 12));

    // Set bank in 4k-Imem
    rtdf_outb(IMEM4K_BANK_0x2c01, bank_num);

    // Disable global mapping of 4k-Imem
//	rtdf_clearBitsb(GLOBAL_MAPPING_0x2c08, _BIT1);
    rtdf_setBitsb(GLOBAL_MAPPING_0x2c08, _BIT1);

    // Enable 4k-iMem map
    rtdf_setBitsb(IMEM4K_REMAP_0x2c02, _BIT0);
}

void drv_imem4k_Stop(void)
{
    // Disable global mapping of 4k-Imem
    rtdf_maskb(GLOBAL_MAPPING_0x2c08, ~_BIT1, 0);


    // Disable 4k-iMem map
    rtdf_clearBitsb(IMEM4K_REMAP_0x2c02, _BIT0);
}

void drv_imem8k_Start(UINT8 bank_num, UINT16 imem_addr)
{
//	rtd_outb( SYS_SHARE_MEM_0x2da1, 0x00 );
    // Set sys share memory to mode - 0
    drv_mem_SetShareMode(MEM_SHARE_MODE_0);

    // Disable 8k-iMem map
    drv_imem8k_Stop();

    // Set start address in 8k-Imem
    rtdf_outb(IMEM8K_ADDR_0x2c03, (UINT8)((imem_addr & 0xf000) >> 12));

    // Set bank in 8k-Imem
    rtdf_outb(IMEM8K_BANK_0x2c04, bank_num);

    // Disable global mapping of 8k-Imem
    rtdf_clearBitsb(GLOBAL_MAPPING_0x2c08, _BIT0);

    // Enable 8k-iMem map
    rtdf_setBitsb(IMEM8K_REMAP_0x2c05, _BIT0);
}

void drv_imem8k_Stop(void)
{
    // Disable 8k-iMem map
    rtdf_outb(IMEM8K_REMAP_0x2c05, 0);

    // Disable global mapping of 8k-Imem
    rtdf_clearBitsb(GLOBAL_MAPPING_0x2c08, _BIT0);
}
#endif
