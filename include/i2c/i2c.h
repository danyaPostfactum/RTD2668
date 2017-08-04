/*=============================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2004 *
 * All rights reserved.                                       *
 *============================================================*/
#ifndef _DEV_I2C_H
#define _DEV_I2C_H

/*====================== Include files ========================*/
#include "sparrow.h"

/*===================== Global Constant =======================*/
/* for linux driver */
#define IRDEV_ADAPS_MAX		1
#define I2C_IRQ 		    8
#define I2C_NUM             CHIP_I2C_TOTAL
#define I2C_RX_BIG_BUG      (2*1024)
#define I2C_1_Local_Addr   0x55
#define I2C_EXCESS_MALLOC  32
#define I2C_MAX_READ_ONCE  8
/*======================== Type Defines =======================*/


/*==================== Macro Definition =======================*/
#define I2C_BASE_ADDRESS(x)		(IC_CON_0x2900 + ((UINT16)x << 8))
#define I2C_EE_WWAIT_TIME            (100000)
#define I2C_EE_RWAIT_TIME            (100)
#define I2C1_BASE					 0x2900
#define I2C2_BASE					 0x2a00
#define I2C_CLK                      27000000
//#define I2C_NUM                      CHIP_I2C_TOTAL
#define I2C_MAX_FIFO_SIZE            8
#define I2C_IC_COMP_VERSION_STR      0x3130332a
#define I2C_IC_COMP_TYPE_STR         0x44570140
#define I2C_STD_SCL_HCNT             0x6d
#define I2C_STD_SCL_LCNT             0x80
#define I2C_FAST_SCL_HCNT            0x10
#define I2C_FAST_SCL_LCNT            0x23
#define I2C_TX_FIFO_TL               0x3
#define I2C_RX_FIFO_TL               0x2

#define I2C_IC_CON                   0x0
#define I2C_IC_TAR                   0x4
#define I2C_IC_SAR                   0x8
#define I2C_IC_HS_MADDR              0xC
#define I2C_IC_DATA_CMD              0x10
#define I2C_IC_SS_SCL_HCNT           0x14
#define I2C_IC_SS_SCL_LCNT           0x18
#define I2C_IC_FS_SCL_HCNT           0x1C
#define I2C_IC_FS_SCL_LCNT           0x20
#define I2C_IC_INTR_STAT             0x2C
#define I2C_IC_INTR_MASK             0x30
#define I2C_IC_RAW_INTR_STAT         0x34

#define I2C_IC_RX_TL                 0x38
#define I2C_IC_TX_TL                 0x3c
#define I2C_IC_CLR_INTR              0x40
#define I2C_IC_CLR_RX_UNDER          0x44
#define I2C_IC_CLR_RX_OVER           0x48
#define I2C_IC_CLR_TX_OVER           0x4c
#define I2C_IC_CLR_RD_REQ            0x50
#define I2C_IC_CLR_TX_ABRT           0x54
#define I2C_IC_CLR_RX_DONE           0x58
#define I2C_IC_CLR_ACTIVITY          0x5c
#define I2C_IC_CLR_STOP_DET          0x60
#define I2C_IC_CLR_START_DET         0x64
#define I2C_IC_CLR_GEN_CALL          0x68
#define I2C_IC_ENABLE                0x6c
#define I2C_IC_STATUS                0x70
#define I2C_IC_TXFLR                 0x74
#define I2C_IC_RXFLR                 0x78
#define I2C_TX_ABRT_SOURCE           0x80
#define I2C_IC_DMA_CR                0x88
#define I2C_IC_DMA_TDLR              0x8c
#define I2C_IC_DMA_RDLR              0x90
#define I2C_IC_COMP_PARAM_1          0xf4
#define I2C_IC_COMP_VERSION          0xf8
#define I2C_IC_COMP_TYPE             0xfc

#define IC_CON_MASTER_MODE_ENABLE   (1UL<<0)
#define IC_CON_SPEED(x)             (x<<1)
#define IC_CON_SPEED_MASK           (3UL<<1)
#define IC_CON_10BITADDR_MASTER     (1UL<<4)
#define IC_CON_10BITADDR_SLAVE      (1UL<<3)

#define IC_ENABLE_ENABLE            (1UL<<0)

#define MIS_IC_TAR(x)               (x<<0)
#define MIS_IC_TAR_MASK             (0x000003FFUL)
#define MIS_IC_SAR(x)               (x<<0)
#define MIS_IC_SAR_MASK             (0x000003FFUL)
#define CLR_INTR_CLR(x)             (rtd_inw(x+I2C_IC_CLR_INTR))
#define CLR_RX_UNDER_CLR(x)         (rtd_inw(x+I2C_IC_CLR_RX_UNDER))
#define CLR_RX_OVER_CLR(x)          (rtd_inw(x+I2C_IC_CLR_RX_OVER))
#define CLR_RD_REQ_CLR(x)           (rtd_inw(x+I2C_IC_CLR_RD_REQ))
#define CLR_TX_ABRT_CLR(x)          (rtd_inw(x+I2C_IC_CLR_TX_ABRT))
#define CLR_RX_DONE_CLR(x)          (rtd_inw(x+I2C_IC_CLR_RX_DONE))
#define CLR_ACTIVITY_CLR(x)         (rtd_inw(x+I2C_IC_CLR_ACTIVITY))
#define CLR_STOP_DET_CLR(x)         (rtd_inw(x+I2C_IC_CLR_STOP_DET))
#define CLR_START_DET_CLR(x)        (rtd_inw(x+I2C_IC_CLR_START_DET))
#define CLR_GEN_CALL_CLR(x)         (rtd_inw(x+I2C_IC_CLR_GEN_CALL))

#define RAW_INTR_STAT_GEN_CALL      (1UL<<11)
#define RAW_INTR_STAT_START_DET     (1UL<<10)
#define RAW_INTR_STAT_STOP_DET      (1UL<<9)
#define RAW_INTR_STAT_ACTIVITY      (1UL<<8)
#define RAW_INTR_STAT_RX_DONE       (1UL<<7)
#define RAW_INTR_STAT_TX_ABRT       (1UL<<6)
#define RAW_INTR_STAT_RD_REQ        (1UL<<5)
#define RAW_INTR_STAT_TX_EMPTY      (1Ul<<4)
#define RAW_INTR_STAT_TX_OVER       (1UL<<3)
#define RAW_INTR_STAT_RX_FULL       (1UL<<2)
#define RAW_INTR_STAT_RX_OVER       (1UL<<1)
#define RAW_INTR_STAT_RX_UNDER      (1UL<<0)


#define RAW_INTR_MASK_GEN_CALL      (1UL<<11)
#define RAW_INTR_MASK_START_DET     (1UL<<10)
#define RAW_INTR_MASK_STOP_DET      (1UL<<9)
#define RAW_INTR_MASK_ACTIVITY      (1UL<<8)
#define RAW_INTR_MASK_RX_DONE       (1UL<<7)
#define RAW_INTR_MASK_TX_ABRT       (1UL<<6)
#define RAW_INTR_MASK_RD_REQ        (1UL<<5)
#define RAW_INTR_MASK_TX_EMPTY      (1UL<<4)
#define RAW_INTR_MASK_TX_OVER       (1UL<<3)
#define RAW_INTR_MASK_RX_FULL       (1UL<<2)
#define RAW_INTR_MASK_RX_OVER       (1UL<<1)
#define RAW_INTR_MASK_RX_UNDER      (1UL<<0)
#define RAW_INTR_MASK_CLEAR_ALL     (0)

#define IC_RESTART_EN               (1UL<<5)
#define IC_DATA_CMD_DATA_MASK       (0xFF)
#define IC_DATA_CMD_WRITE_ENABLE      (~0x00000100UL)
#define IC_DATA_CMD_READ_ENABLE       (0x00000100UL)


#define IC_STAT_RX_FULL       (1UL<<4)
#define IC_STAT_RX_NOT_EMPTY  (1UL<<3)
#define IC_STAT_TX_EMPTY      (1UL<<2)
#define IC_STAT_TX_FULL       (1UL<<1)
#define IC_STAT_ACT           (1UL<<0)

/*
 *  I2C register offset definitions
 */

/*
 * Enumeration for i2c
 */
typedef enum
{
    MASTER =0,
    SLAVE =1
} I2C_ROLE_T;

typedef enum
{
    CHIP_I2C1=0,
    CHIP_I2C2,
    CHIP_I2C3,
    CHIP_I2C_TOTAL
} I2C_CHIP_T;

typedef enum
{
    I2C_ADDR_7=0,
    I2C_ADDR_10=1
} I2C_ADDR_T;

typedef enum
{
    I2C_SPEED_STD=1,
    I2C_SPEED_FAST=2
} I2C_SPEED_T;

/*
 *  data structure
 */
typedef struct _STRUCT_I2C_PARAMETER
{
    UINT8	enable:1;		// enable
    UINT8	mode:1;			// master or slave
    UINT8	addrtype: 1;		// ADDR_7 or I2C_ADDR_10
    UINT8	speed:1;			// SPEED_STD or SPEED_FAST
    UINT8	rx_tl:2;			//
    UINT8	tx_tl:2;			//
    UINT16	addr;			// address
} STRUCT_I2C_PARAMETER;

typedef struct _i2c_info_
{
    UINT8   role:4;
    UINT8    chip:4;
//	I2C_ADDR_T      addrtype;
//	I2C_SPEED_T     speed;
    UINT16		io_base;
//	unsigned long   addr;
//	unsigned long   tar_addr;
    UINT8   scl_hcnt;
    UINT8   scl_lcnt;
    UINT8   rx_tl;
    UINT8   tx_tl;
//	UINT32          buf_index;
//	struct completion   complete;
//	UINT8           *buf;
} I2C_INFO_T;

typedef struct _i2c_info_table
{
    UINT8    addrtype:4;
    UINT8   speed:4;
    UINT8  addr;
    UINT8  tar_addr;
    UINT8          read_len;
    I2C_INFO_T*   ptsysI2C;
} I2C_INFO_TABLE;

/*==================== Function  Prototypes ==================*/

void drv_i2c_Init(void);
void drv_i2c_PowerOn(bit on_off);


bit drv_i2c_Set_rx_tx_tl(UINT8 i2c_master, UINT8 rx_tl, UINT8 tx_tl);
bit drv_i2c_currRead(UINT32 i2c_master,UINT8 r_len,UINT8* read_buf);
bit drv_i2c_randomRead(UINT32 i2c_master,UINT8 w_len, UINT8 r_len, UINT8* write_buf, UINT8* read_buf);
bit drv_i2c_Write(UINT8 i2c_master, UINT8 w_len, UINT8* write_buf);
bit drv_i2c_Set_speed(UINT8 i2c_master,I2C_SPEED_T speed);
//bit drv_i2c_Set_addr(UINT8 i2c_master,UINT8 local_addr,UINT8 target_addr,I2C_ADDR_T addr_t);
bit drv_i2c_Set_MasterMode(UINT8 i2c_master, UINT8 master_type);
bit drv_i2c_Set_AddressType(UINT8 i2c_master, UINT8 addr_type);
UINT16 drv_i2c_Get_TargetAddr(UINT8 i2c_master);
bit drv_i2c_Set_TargetAddr(UINT8 i2c_master, UINT16 Target_addr);
#if 0 // Temporily not use
UINT16 drv_i2c_Get_LocalAddr(UINT8 i2c_master);
#endif
bit drv_i2c_Set_LocalAddr(UINT8 i2c_master, UINT16 local_addr);
#ifdef CONFIG_TUNER_SILICON
bit drv_i2c_ContRead(UINT32 i2c_master,UINT8 r_len,UINT8* read_buf);
void drv_tuner_read_rt810(UINT8 r_len);
#endif


#if CONFIG_ENABLE_FM
void QND_Init();
BOOL AdjustFMfun(UINT16 action);
void Delay5ms(UINT16 time);												  

#endif


#ifdef CONFIG_IFD_TUNER_AUTO
bit drv_i2c_isr_Set_TargetAddr(UINT8 i2c_master, UINT16 Target_addr);
bit drv_i2c_isr_Write(UINT8 i2c_master, UINT8 w_len, UINT8* write_buf);
#endif

#endif
