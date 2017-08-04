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

#ifndef  _SPI_FLASH_H_
#define  _SPI_FLASH_H_

#include "sparrow.h"
#include "spi_dma.h"

/*==================== Data structures =======================*/
/**
 *An enum.
 *The SPI Flash controller protocols
 */
typedef enum _SFC_PROTOCOLS
{
    _SPI_MODE_3 = 0,
    _SPI_MODE_1,
    _SPI_ICP_H,
    _SPI_ICP_L

} SFC_PROTOCOLS;

typedef enum _SFC_ERASE_USED
{
    _SPI_PAGE = 0,
    _SPI_SECTOR,
    _SPI_CHIP,

} SFC_ERASE_USED;

/**
 *A struct.
 *The command data structure of SPI Flash controller
 */
typedef struct _SFC_CMD
{
    UINT32 spiFlashCmd;
    UINT32 spiFlashCtrl;

} SFC_CMD;

/**
 *A struct.
 *The specific spi Flash setting data structure.
 */
typedef struct _SFC_SPEC
{

    SFC_CMD	spiSSTWriteStatusEnableCmd;
    SFC_CMD spiWriteStatusCmd;
    SFC_CMD	spiSSTWriteEnableCmd;
    SFC_CMD	spiWriteEnableCmd;
    SFC_CMD spiEraseCmd;


    SFC_CMD	spiSSTFirstWriteCmd;
    SFC_CMD spiWriteDisableCmd;

    SFC_CMD spiReadDataCmd;
    SFC_CMD spiPageProgramCmd;
    SFC_CMD spiReadIDCmd;
    SFC_CMD spiReadStatusCmd;
    UINT32  spiFlashWriteBuf;
    long	spiFlashWriteLength;
    UINT32  spiFlashAddress;
    UINT32	spiFlashID;
    UINT32  spiFlashInProc;
    UINT32	spiWriteStatus;
    UINT32	RebootAddr;
    UINT32 	spiFlashEraseBlockSize;

} SFC_SPEC;

typedef struct _SFC_SPEC_HASH
{

    char *spiFlashName;
    SFC_SPEC spiFlashSpec;
} SFC_SPEC_HASH;

typedef struct
{
    UINT32 ID_90;
    UINT32 Mask_90;
    UINT32 ID_9F;
    UINT32 Mask_9F;
    SFC_SPEC *spec;
    char*	  name;

    UINT32 	  MinBlockSize;
    UINT32 	  MaxBlockSize;
    UINT32    MinBlockStart;	// Block not in MaxBlock_Start between MaxBlock_End then use MaxBlockSize
    UINT32    MinBlockEnd;
    UINT32	  FlashSize;		// 0xFFFFFFFF means FlashSize is unspecified
} FLASH_ID_T;
extern FLASH_ID_T FlashSetting;

typedef enum
{

    TAG_MAGIC_ROS_IMG_END = 0,
    TAG_MAGIC_NULL_ID

} TAG_INDEX_MAGIC_ID_T;

typedef struct
{

    TAG_INDEX_MAGIC_ID_T TAG_INDEX_MAGIC_ID;
    UINT32 TAG_LENGTH;
    UINT32 TAG_POINTER;

} TAG_INDEX_T;


#define drv_flashErease drv_spi_flash_Erease

/*============================================================================*/
/**
 * drv_spi_flash_Read
 * Read from specified address in flash to memory
 *
 * @param <UINT16 dst>		{ Target memory address }
 * @param <UINT32 src>		{ source flash address }
 * @param <UINT16 length>		{ the number of data loaded }
 * @return <BOOL>		{ return _TRUE if success, otherwise _FALSE }
 *
 */
//#define drv_spi_flash_Read(dst, src, length)	drv_spi_dma_memcpy(dst, src, length, SPI_DMA_TRANSFER_MODE_DMEM)
#define drv_spi_flash_Read(dst, bank, src, length)	drv_spi_dma_memcpy(dst, bank, src, length, SPI_DMA_TRANSFER_MODE_DMEM)

int drv_flashWrite(unsigned long dst, unsigned long src, long length);
//void drv_spi_flash_Erease(UINT32 StartAddress, UINT32 Length);
void drv_spi_flash_Erase(UINT8 bankno, UINT16 StartAddress, UINT16 Length);
void drv_spi_isr_flash_Erase(UINT8 bankno, UINT16 StartAddress, UINT16 Length);
//void drv_spi_flash_Write(UINT8 bankno, UINT16 StartAddress, UINT8* sBuf, UINT16 nSize);
void drv_flash_Write(UINT8 bankno, UINT16 StartAddress, UINT8* sBuf, UINT16 nSize);
#define drv_spi_flash_Write(bankno, StartAddress, sBuf, nSize) drv_flash_Write(bankno, StartAddress, sBuf, nSize)
void drv_spi_isr_flash_Write(UINT8 bankno, UINT16 StartAddress, UINT8* sBuf, UINT16 nSize);
//void drv_spi_flash_Write(UINT32 StartAddress, UINT8* sBuf, int nSize);
//BOOL drv_spi_flash_Read(UINT16 dst, UINT32 src, UINT16 length);
void drv_spi_flash_copy(UINT32 srcAddr, UINT32 destAddr, UINT16 count);
BOOL drv_spi_flash_CheckReady(void);
#endif // #ifndef  _IRRC_H_
void drv_flash_protect(bit enable);
