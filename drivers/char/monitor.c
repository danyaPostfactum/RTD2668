/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2006
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is a program to accept data from uart and operation.
 *
 * @author 	$Author$
 * @date	$Date$
 * @version	$Revision$
 * @ingroup	uart
 */

/**
 * @addtogroup uart
 * @{
 */

/*============================ Module dependency  ===========================*/
#include "char\monitor.h"
#include "char\uart.h"
#include "pcb\pcb_utility.h"
#include "pcb_conf.h"
#include "sparrow\reg_def.h"
#include "isr_utility.h"
#include "pcb_customer.h"
#include "i2c\i2c.h"
#include "scaler\scaler_pll.h"
#include "panel\panel_library.h"
#include "pcb_sys.h"
#include "message.h"
#include "rosprintf.h"
#include "message.h"
#include "reg52.h"
#include "ifd/ifd.h"
#include "spi_dma.h"
#include "flash\spi_flash.h"
#include "flow_conf_struct.h"
#include "imem.h"
#include "osdfunction.h"
#include "timer\timer.h"
/*================================ Definitions ==============================*/
#define UartCMD_OFFSET1					(0x00)
#define UartCMD_OFFSET2					(0xB0)
#define UartCMD_OFFSET					(UartCMD_OFFSET2)
#define UartCMD_DebugModeEnter			(0x10 + UartCMD_OFFSET)
#define UartCMD_DebugModeExit			(0x11 + UartCMD_OFFSET)
#define UartCMD_StopLogModeEnter  	(0x12 + UartCMD_OFFSET)
#define UartCMD_StopLogModeExit  	(0x13 + UartCMD_OFFSET)

#define UartCMD_CScalerRead_UINT8		(0x20 + UartCMD_OFFSET)
#define UartCMD_CScalerRead_UINT32		(0x2c + UartCMD_OFFSET)
#define UartCMD_CScalerWrite_UINT8		(0x30 + UartCMD_OFFSET)
#define UartCMD_CScalerWrite_UINT32		(0x3c + UartCMD_OFFSET)
#define UartCMD_I2CRead					(0x21 + UartCMD_OFFSET)
#define UartCMD_I2CRead_Current			(0x22 + UartCMD_OFFSET)
#define UartCMD_I2CWrite					(0x31 + UartCMD_OFFSET)
#define UartCMD_I2CWrite_NEW			(0x32 + UartCMD_OFFSET)
#define UartCMD_24C32Read				(0x26 + UartCMD_OFFSET)
#define UartCMD_24C32Write				(0x36 + UartCMD_OFFSET)
#define UartCMD_TunerRead				(0x24 + UartCMD_OFFSET)
#define UartCMD_TunerWrite				(0x34 + UartCMD_OFFSET)
#define UartCMD_RTD2638Read			(0x28 + UartCMD_OFFSET)
#define UartCMD_RTD2638Write			(0x38 + UartCMD_OFFSET)
#define UartCMD_AutoTest	 			(0x39 + UartCMD_OFFSET)
#define UartCMD_I2CBus		 			(0x3A + UartCMD_OFFSET)
#define UartCMD_ProgramWrite			(0x37 + UartCMD_OFFSET)
#define UartCMD_ROSINFORMATION		(0x2a + UartCMD_OFFSET)
#define UartCMD_XMODEM_RECEIVE		(0x2b + UartCMD_OFFSET)
#define UartCMD_ScalerReg2Mem			(0x40 + UartCMD_OFFSET)
#define UartCMD_ScalerRegWhenCondition	(0x41 + UartCMD_OFFSET)
#define UartCMD_IRKeyCommand			(0x42 + UartCMD_OFFSET)
#define UartCMD_ROS_ENTER_CRITICAL	(0x43 + UartCMD_OFFSET)
#define UartCMD_ROS_EXIT_CRITICAL   	(0x44+ UartCMD_OFFSET)
#define UartCMD_GET_HDCP_KEY_ADDR   	(0x45 + UartCMD_OFFSET)
#define UartCMD_FLASH_ERASE		   	(0x46+ UartCMD_OFFSET)
#define UartCMD_FLASH_WRITE			(0x37 + UartCMD_OFFSET)
#define UartCMD_FLASH_READ				(0x3b + UartCMD_OFFSET)
#define UartCMD_GET_EDID_KEY_ADDR   	(0x47 + UartCMD_OFFSET)

#define UartCMD_VIRTUALKEY			(0x12)
#define UartCMD_DS_TCON				(0x13)
#define UartCMD_ROMCOMMAND			(0x11)

// For Panel tools
#define UartCMD_ScalerDisplayDomainAccess	(0x4a + UartCMD_OFFSET)
//Sub-Command
#define ScalerDisplayDomainAccess_DCLK				0x00
#define ScalerDisplayDomainAccess_DCLK_M				0x01
#define ScalerDisplayDomainAccess_DCLK_N				0x02
#define ScalerDisplayDomainAccess_DCLK_DIV			0x03
#define ScalerDisplayDomainAccess_DCLK_OFFSET		0x04
#define ScalerDisplayDomainAccess_DDS_ENABLE			0x05
#define ScalerDisplayDomainAccess_DDS_RANGE			0x06
#define ScalerDisplayDomainAccess_FRC_MODE			0x07
#define ScalerDisplayDomainAccess_DATA_PORT			0x08
#define ScalerDisplayDomainAccess_COLOR_BITS			0x09
#define ScalerDisplayDomainAccess_BITMAP_TABLE		0x0a
#define ScalerDisplayDomainAccess_PORTAB_SWAP		0x0b
#define ScalerDisplayDomainAccess_REDBLUE_SWAP		0x0c
#define ScalerDisplayDomainAccess_MSBLSB_SWAP		0x0d
#define ScalerDisplayDomainAccess_OUTPUT_INV		0x0e
#define ScalerDisplayDomainAccess_H_HOTAL			0x0f
#define ScalerDisplayDomainAccess_V_HOTAL			0x10
#define ScalerDisplayDomainAccess_LASTLINE			0x11
#define ScalerDisplayDomainAccess_HSYNC_WIDTH		0x12
#define ScalerDisplayDomainAccess_VSYNC_LENGTH		0x13
#define ScalerDisplayDomainAccess_ENABLE_HSTART		0x14
#define ScalerDisplayDomainAccess_ENABLE_HEND		0x15
#define ScalerDisplayDomainAccess_ENABLE_VSTART		0x16
#define ScalerDisplayDomainAccess_ENABLE_VEND		0x17
#define ScalerDisplayDomainAccess_ACTIVE_HSTART		0x18
#define ScalerDisplayDomainAccess_ACTIVE_HEND		0x19
#define ScalerDisplayDomainAccess_ACTIVE_VSTART		0x1a
#define ScalerDisplayDomainAccess_ACTIVE_VEND		0x1b
#define ScalerDisplayDomainAccess_DCLK_DELAY			0x1c
#define ScalerDisplayDomainAccess_PANEL_SWITCH		0x1d
#define ScalerDisplayDomainAccess_LVDS_SWITCH		0x1e
#define ScalerDisplayDomainAccess_LIGHT_SWITCH		0x1f
#define ScalerDisplayDomainAccess_FS_WATCHDOG_SWITCH		0x20
// ------------------------------------------------------------------

typedef enum UART_CCOMMAND_STATE
{
    UartCMD_None_STATE = 0,
    UartCMD_ACCEPTING_STATE,
    UartCMD_UNPROCESS_STATE,

    UartCMD_DebugModeEnter_STATE,
    UartCMD_DebugModeExit_STATE,
    UartCMD_StopLogModeEnter_STATE,
    UartCMD_StopLogModeExit_STATE,
    UartCMD_FW_PROCESS_STATE,
    UartCMD_CScalerRead_UINT8_STATE,
    UartCMD_CScalerWrite_UINT8_STATE,
    UartCMD_CScalerRead_UINT32_STATE,
    UartCMD_CScalerWrite_UINT32_STATE,
    UartCMD_I2C_BUS_STATE,
    UartCMD_I2C_WRITE_STATE,
    UartCMD_I2C_READ_STATE,
    UartCMD_I2C_READ_CURRENT_STATE,
    UartCMD_ROSINFORMATION_STATE,
    UartCMD_IRKEY_COMMAND_STATE,
    UartCMD_DDOMAIN_ACCESS_STATE,
    UartCMD_TUNER_WRITE_STATE,
    UartCMD_ROS_ENTER_CRITICAL_STATE,
    UartCMD_ROS_EXIT_CRITICAL_STATE,
    UartCMD_RESET_SYSTEM_STATE,
    UartCMD_FLASH_ERASE_STATE,
    UartCMD_ProgramWrite_STATE
};

//
//#define USE_ROMCODE_COMMAND	1
#define USE_ROMCODE_COMMAND 1
static void model_PanelAccessReadProc(UINT8 subcommand);
static void model_PanelAccessWriteProc(UINT8 subcommand, UINT32 val);
static void model_monitor_ROSInformation(UINT8 Cmd);
void drv_flash_Write(UINT8 bankno, UINT16 StartAddress, UINT8* sBuf, UINT16 nSize);
//void rom_monitor_Handler(void);
/*==================== Functions ================== */
/**
 */
volatile UINT8 xdata rcvCount;
volatile UINT8 xdata rcvBuffer[16];
volatile bit fDebugMode;
volatile bit fROMCodeMode;
volatile UINT8 xdata UartCount;
volatile UINT8 xdata fUartState;
volatile UINT8 xdata I2C_MASTER;
volatile UINT8 nCommPort;
sfr SFR_SPC_FNC_0x8e = 0x8e;
volatile UINT8 ucVirtualkey;

void fw_monitor_system_Init(void)
{
    rcvCount = 0;
    UartCount = 0;
    fUartState = 0;
    I2C_MASTER = 0;
    fDebugMode = 0;
    fROMCodeMode = 0;
    nCommPort = 0;
}

void fw_monitor_Init(void)
{
#if(UART_SOURCE_0_ENABLE == _ENABLE)
    drv_isr_Enable(SYS_ISR_UART0, _FALSE);
//#ifdef CONFIG_ENABLE_RTD_LOG//gary for  RTD_LOG=off , code can't run  20091223
    drv_isr_SelectRoute(ROUTE_SEL_UART0, IRQ_ROUTE_IRQ0);

    //set DLAB = 0
    rtdf_clearBits(U0LCR_0x260c, _BIT7);

    // Enable received data available interrupt
    rtdf_setBits(U0IER_DLH_0x2604, _BIT0);

    // Clear Pending status
    rtd_outl(ISR_0x2504, _BIT2);

    drv_isr_Enable(SYS_ISR_UART0, _TRUE);
//#endif
#endif

#if(UART_SOURCE_1_ENABLE == _ENABLE)
    drv_isr_Enable(SYS_ISR_UART1, _FALSE);
//#ifdef CONFIG_ENABLE_RTD_LOG//gary for  RTD_LOG=off , code can't run  20091223
    drv_isr_SelectRoute(ROUTE_SEL_UART1, IRQ_ROUTE_IRQ0);

    //set DLAB = 0
    rtdf_clearBits(U1LCR_0x262c, _BIT7);

    // Enable received data available interrupt
    rtdf_setBits(U1IER_DLH_0x2624, _BIT0);

    // Clear Pending status
    rtd_outl(ISR_0x2504, _BIT3);

    drv_isr_Enable(SYS_ISR_UART1, _TRUE);
//#endif
#endif

    RTD_Log(LOGGER_INFO, "fw_monitor_Init ...\n");
}

/**
 * For kingmice tool comunication protocol.
 * The followings are protocol command.
 *  NEW RS232 COMMAND
 *     1. Enter Deug Mode	  :  0x10,0x01,0x02,0x03,0x04
 *     2. Exit Debug Mode     :  0x11,0x04,0x03,0x02,0x01
 *     3. Read MCU   	  :  0x20,AddressHi,AddressLo,Count,Inc
 *     4. Read CI2c  		   :  0x21,Slave Address,Address,Count
 *     5. Read Micronas3230    :  0x22,Hi-Address,Low-Address,Kind
 *     6. Read VCT49xyl  	  :  0x23,Slave Address,Sub Address,Hi-Address,Low-Address,Kind,Count
 *     7. Read Philips Tuner   :  0x24,Slave Address
 *	 9. Read Flash			: 0x25,
 *	10. Read EEPROM		: 0x26,
 *	11. Read MCU			: 0x27,
 *	12. Read RTD2880		: 0x28, Address3, Address2, Address1, Address0, Count, Option
 *	     Read ROS information: 0x2a, cmd
 *								cmd:00 -- all
 *								cmd:01 -- version
 *	 							cmd:02 -- build date
 *	     Receive XModem data: 0x2b, Address3, Address2, Address1, Address0
 *    13. Write MCU  	  :  0x30,AddressHi,AddressLo,Data
 *    14. Write CI2c 		   :  0x31,Slave Address,Address,Data
 *    15. Write Micronas3230   :  0x32,Slave Address,Hi-Address,Low-Address,Data,Kind (Kind = 0)
 *    						 0x32,Hi-Address,Low-Address,Hi-Data,Low-Data,Kind (Kind = 1)
 *    16. Write VCT49xyl 	  :  0x33,Slave Address,Sub Address,Hi-Address,Low-Address,Hi-Data,Low-Data,Kind
 *    17. Write Philips Tuner  :  0x34,Slave Address,Divider UINT8 1,Divider UINT8 2,Control UINT8,Bandswitch UINT8,Auxiliary UINT8
 *	18. Write Flash			: 0x35,
 *	19. Write EEPROM		: 0x36,
 *	20. Write MCU			: 0x37,
 *    21. Write RTD2880  	  :  0x38,Address3, Address2, Address1, Address0, Data3, Data2, Data1, Data0
 * Extra:
 *    22. Read rtd2622 reg into mem: 0x40, reg address, count, inc, address3, address2, address1, address0
 *	23. Read rtd2522 reg2 when reg1 == value: 0x41, page1, reg1, value1, reg2, value2, page 2, reg3, Count, Inc
 */
bit drv_isr_uart_handler(void)
{
    int i;
//	bit nPort;
//    INT16 nCnt, nCnt1;
    UINT16 nAddr;
    UINT8 UartData;
    UINT8 Data[256];
    UINT32 getVal;
//    UINT8 flash_status;
    UINT8  getTempVal;

    if (drv_isr_GetPending(SYS_ISR_UART0))
        nCommPort = 0;
    else if (drv_isr_GetPending(SYS_ISR_UART1))
        nCommPort = 1;
    else
        return RT_ISR_UNHANDLED;

    if (!drv_isr_uart_Read(nCommPort, &UartData))
        return RT_ISR_UNHANDLED;

    if (fUartState == UartCMD_None_STATE)
    {
        UartCount = 0;
        rcvBuffer[UartCount] = UartData;
        fUartState = UartCMD_ACCEPTING_STATE;
        UartCount++;
        switch (UartData)
        {
        case UartCMD_DebugModeEnter:  	// Enter debug mode
        case UartCMD_DebugModeExit:  		// Exit debug mode
        case UartCMD_CScalerRead_UINT8:
        case UartCMD_CScalerRead_UINT32:
        case UartCMD_FLASH_READ:
            rcvCount = 5;
            break;

        case UartCMD_StopLogModeEnter:
            break;

        case UartCMD_StopLogModeExit:
            break;

        case UartCMD_FLASH_ERASE:
        case UartCMD_CScalerWrite_UINT8:
            rcvCount = 4;
            break;

        case UartCMD_FLASH_WRITE:
            rcvCount = 5;
            break;

        case UartCMD_CScalerWrite_UINT32:
            rcvCount = 7;
            break;

        case UartCMD_I2CBus:
            break;

        case UartCMD_I2CWrite_NEW:
            break;

        case UartCMD_I2CRead:
            break;

        case UartCMD_I2CRead_Current:
            break;

        case UartCMD_ROSINFORMATION:
            rcvCount = 1;
            break;

        case UartCMD_IRKeyCommand:
            rcvCount = 2;
            break;

        case UartCMD_ScalerDisplayDomainAccess:
            break;

        case UartCMD_TunerWrite:
            rcvCount = 6;
            break;

        case UartCMD_ROMCOMMAND:
            rcvCount = 4;
            break;

        case UartCMD_ROS_ENTER_CRITICAL:
        case UartCMD_ROS_EXIT_CRITICAL:
        case UartCMD_GET_HDCP_KEY_ADDR:
        case UartCMD_GET_EDID_KEY_ADDR:
            rcvCount = 2;
            break;

		case UartCMD_VIRTUALKEY:
			rcvCount = 1;
			break;

		case UartCMD_DS_TCON:
			rcvCount = 5;
			break;

        default:
            fUartState = UartCMD_None_STATE;
            rcvBuffer[0] = 0;
            UartCount = 0;
            drv_isr_uart_Write(nCommPort, UartData);
            break;
        };
    }
    else if (fUartState == UartCMD_ACCEPTING_STATE)
    {
        rcvBuffer[UartCount++] = UartData;
        if (UartCount >= rcvCount)
        {
            fUartState = UartCMD_None_STATE;
            switch (rcvBuffer[0])
            {
            case UartCMD_DebugModeEnter:  	// Enter debug mode
                if (rcvBuffer[1] == 0x01)
                    if (rcvBuffer[2] == 0x02)
                        if (rcvBuffer[3]  == 0x03)
                            if (rcvBuffer[4]  == 0x04)
                            {
                                fDebugMode = 1;
                                drv_isr_isr_Enable(SYS_ISR_IF_DEMOD, _DISABLE);
                                drv_isr_isr_Enable(SYS_ISR_TIMER1, _DISABLE);
                                drv_isr_isr_Enable(SYS_ISR_TIMER2, _DISABLE);
                                rtd_isr_outl(TCWCR_0x2870, 0x000000A5);
                                rtd_isr_outl(WTST_0x2878, 0);
                                drv_isr_uart_Write(nCommPort, 0x10);
                            }
                break;

            case UartCMD_DebugModeExit:  		// Exit debug mode
                if (rcvBuffer[1] == 0x04)
                    if (rcvBuffer[2] == 0x03)
                        if (rcvBuffer[3]  == 0x02)
                            if (rcvBuffer[4]  == 0x01)
                            {
                                fDebugMode = 0;
                                drv_isr_isr_Enable(SYS_ISR_IF_DEMOD, _ENABLE);//ROS_ENTER_CRITICAL();
                                drv_isr_isr_Enable(SYS_ISR_TIMER1, _ENABLE);
                                drv_isr_isr_Enable(SYS_ISR_TIMER2, _ENABLE);
                                rtd_isr_maskl(TCWCR_0x2870, 0x00ffff00, ((UINT32)0x3f << 24));
                                rtd_isr_outl(WTST_0x2878, 0xa5a5a5a5);
                                drv_isr_uart_Write(nCommPort, 0x11);
                            }
                break;

            case UartCMD_CScalerRead_UINT8:
                *((UINT8*)&nAddr) = rcvBuffer[1];
                *((UINT8*)&nAddr + 1) = rcvBuffer[2];
                do
                {
                    if (rcvBuffer[3] > 15)
                    {
                        for (i = 0; i <= 15; i++)
                        {
                            Data[i] = rtd_isr_inb(nAddr);

                            // check inc
                            if (!(rcvBuffer[4] & _BIT0))
                                nAddr++;
                        }

                        for (i = 0; i <= 15; i++)
                            drv_isr_uart_Write(nCommPort, Data[i]);

                        rcvBuffer[3] -= 15;
                    }
                    else
                    {

                        for (i = 0; i <= rcvBuffer[3]; i++)
                        {

                            Data[i] = rtd_isr_inb(nAddr);

                            // check inc
                            if (!(rcvBuffer[4] & _BIT0))
                                nAddr++;
                        }

                        for (i = 0; i <= rcvBuffer[3]; i++)
                            drv_isr_uart_Write(nCommPort, Data[i]);

                        rcvBuffer[3] = 0;
                    }
                }
                while (rcvBuffer[3]);
                break;

            case UartCMD_CScalerWrite_UINT8:
                *((UINT8*)&nAddr) = rcvBuffer[1];
                *((UINT8*)&nAddr + 1) = rcvBuffer[2];
                rtd_isr_outb(nAddr, rcvBuffer[3]);
                drv_isr_uart_Write(nCommPort, 0x30);
                break;

            case UartCMD_CScalerRead_UINT32:
                *((UINT8*)&nAddr) = rcvBuffer[1];
                *((UINT8*)&nAddr + 1) = rcvBuffer[2];
                while (rcvBuffer[3])
                {
                    Data[0] = rtd_isr_inb(nAddr);
                    Data[1] = rtd_isr_inb(nAddr + 1);
                    Data[2] = rtd_isr_inb(nAddr + 2);
                    Data[3] = rtd_isr_inb(nAddr + 3);
                    drv_isr_uart_Write(nCommPort, Data[3]);
                    drv_isr_uart_Write(nCommPort, Data[2]);
                    drv_isr_uart_Write(nCommPort, Data[1]);
                    drv_isr_uart_Write(nCommPort, Data[0]);
                    // check inc
                    if (!(rcvBuffer[4] & _BIT0))
                        nAddr+=4;

                    rcvBuffer[3]--;
                };
                break;

            case UartCMD_CScalerWrite_UINT32:
                *((UINT8*)&nAddr) = rcvBuffer[1];
                *((UINT8*)&nAddr + 1) = rcvBuffer[2];
                rtd_isr_outb(nAddr, rcvBuffer[6]);
                rtd_isr_outb(nAddr + 1, rcvBuffer[5]);
                rtd_isr_outb(nAddr + 2, rcvBuffer[4]);
                rtd_isr_outb(nAddr + 3, rcvBuffer[3]);
                drv_isr_uart_Write(nCommPort, 0x3c);
                break;
#if 0
            case UartCMD_FLASH_ERASE:
                getTempVal = SFR_SPC_FNC_0x8e;
                SFR_SPC_FNC_0x8e |= 0x02;
                // Get flash's status
                rtd_outb(COMMON_OP_CODE_0xff61,0x05);
                rtd_outb(COMMON_INST_EN_0xff60,0x42);
                rtd_outb(COMMON_INST_EN_0xff60,0x43);
                // wait for Erase finish
                while (rtd_isr_inb(MCU_SPI_FLASH_COMMON_INST_EN_0xff60)  & _BIT0);
                flash_status = rtd_inb(COMMON_INST_READ_PORT0_0xff67);
                if (flash_status & 0x1c)
                {
                    rtd_outb(COMMON_OP_CODE_0xff61, 0x01);
                    rtd_outb(WREN_OP_CODE_0xff62, 0x06);
                    rtd_outb(EWSR_OP_CODE_0xff63, 0x06);
                    rtd_outb(FLASH_PROG_ISP0_0xff64, 0x00);
                    rtd_outb(COMMON_INST_EN_0xff60, 0x68);
                    rtd_outb(COMMON_INST_EN_0xff60, 0x69);
                    while (rtd_isr_inb(MCU_SPI_FLASH_COMMON_INST_EN_0xff60)  & _BIT0);
                }

                *((UINT8*)&nAddr) = rcvBuffer[2];
                *((UINT8*)&nAddr + 1) = rcvBuffer[3];
                drv_spi_isr_flash_Erase(rcvBuffer[1], nAddr, 4096);
                if (flash_status & 0x1c)
                {
                    rtd_outb(COMMON_OP_CODE_0xff61, 0x01);
                    rtd_outb(WREN_OP_CODE_0xff62, 0x06);
                    rtd_outb(EWSR_OP_CODE_0xff63, 0x06);
                    rtd_outb(FLASH_PROG_ISP0_0xff64, flash_status & 0x1c);
                    rtd_outb(COMMON_INST_EN_0xff60, 0x68);
                    rtd_outb(COMMON_INST_EN_0xff60, 0x69);
                    while (rtd_isr_inb(MCU_SPI_FLASH_COMMON_INST_EN_0xff60)  & _BIT0);
                }
                SFR_SPC_FNC_0x8e = getTempVal;
                rcvBuffer[0] = 0;
                drv_isr_uart_Write(nCommPort, 0x46);
                break;

            case UartCMD_FLASH_WRITE:
                getTempVal = SFR_SPC_FNC_0x8e;
                SFR_SPC_FNC_0x8e |= 0x02;
                // Get flash's status
                rtd_outb(COMMON_OP_CODE_0xff61,0x05);
                rtd_outb(COMMON_INST_EN_0xff60,0x42);
                rtd_outb(COMMON_INST_EN_0xff60,0x43);
                while (rtd_isr_inb(MCU_SPI_FLASH_COMMON_INST_EN_0xff60)  & _BIT0);
                flash_status = rtd_inb(COMMON_INST_READ_PORT0_0xff67);
                if (flash_status & 0x1c)
                {
                    rtd_outb(COMMON_OP_CODE_0xff61, 0x01);
                    rtd_outb(WREN_OP_CODE_0xff62, 0x06);
                    rtd_outb(EWSR_OP_CODE_0xff63, 0x06);
                    rtd_outb(FLASH_PROG_ISP0_0xff64, 0x00);
                    rtd_outb(COMMON_INST_EN_0xff60, 0x68);
                    rtd_outb(COMMON_INST_EN_0xff60, 0x69);
                    while (rtd_isr_inb(MCU_SPI_FLASH_COMMON_INST_EN_0xff60)  & _BIT0);
                }
                SFR_SPC_FNC_0x8e = getTempVal;

                *((UINT8*)&nAddr) = rcvBuffer[2];
                *((UINT8*)&nAddr + 1) = rcvBuffer[3];
                nCnt = rcvBuffer[4] + 1;
#if 1
                nCnt1 = 0;
                while (nCnt)
                {
                    while (!drv_isr_uart_Read(nCommPort, &Data[nCnt1]));
                    nCnt1++;
                    nCnt--;
                }
                drv_flash_Write(rcvBuffer[1], nAddr, Data, rcvBuffer[4] + 1);
#else
                while (nCnt)
                {
                    if (nCnt >= 16)
                    {
                        for (i = 0; i < 16; i++)
                            while (!drv_isr_uart_Read(UART_MONITOR_SOURCE, &Data[i]));

                        drv_flash_Write(rcvBuffer[1], nAddr, Data, 16);
                        nCnt -= 16;
                        nAddr += 16;
                    }
                    else
                    {
                        for (i = 0; i < nCnt; i++)
                            while (!drv_isr_uart_Read(UART_MONITOR_SOURCE, &Data[i]));

                        drv_flash_Write(rcvBuffer[1], nAddr, Data, nCnt);

                        nCnt = 0;
                    }
                }
#endif
                if (flash_status & 0x1c)
                {
                    SFR_SPC_FNC_0x8e |= 0x02;
                    rtd_outb(COMMON_OP_CODE_0xff61, 0x01);
                    rtd_outb(WREN_OP_CODE_0xff62, 0x06);
                    rtd_outb(EWSR_OP_CODE_0xff63, 0x06);
                    rtd_outb(FLASH_PROG_ISP0_0xff64, flash_status & 0x1c);
                    rtd_outb(COMMON_INST_EN_0xff60, 0x68);
                    rtd_outb(COMMON_INST_EN_0xff60, 0x69);
                    while (rtd_isr_inb(MCU_SPI_FLASH_COMMON_INST_EN_0xff60)  & _BIT0);
                    SFR_SPC_FNC_0x8e = getTempVal;
                }
                drv_isr_uart_Write(nCommPort, 0x37);
                rcvBuffer[0] = 0;
                break;

            case UartCMD_FLASH_READ:
                *((UINT8*)&nAddr) = rcvBuffer[2];
                *((UINT8*)&nAddr + 1) = rcvBuffer[3];
                nCnt = rcvBuffer[4] + 1;
                while (nCnt > 0)
                {
                    if (nCnt >= 16)
                    {
                        drv_spi_isr_dma_memcpy((UINT16)Data, rcvBuffer[1], nAddr, 16, SPI_DMA_TRANSFER_MODE_DMEM);
                        for (i = 0; i < 16; i++)
                            drv_isr_uart_Write(nCommPort, Data[i]);
                        nAddr += 16;
                        nCnt -= 16;
                    }
                    else
                    {
                        drv_spi_isr_dma_memcpy((UINT16)Data, rcvBuffer[1], nAddr, nCnt, SPI_DMA_TRANSFER_MODE_DMEM);
                        for (i = 0; i < nCnt; i++)
                            drv_isr_uart_Write(nCommPort, Data[i]);
                        nCnt = 0;
                    }
                }
                rcvBuffer[0] = 0;
                break;
#endif
            case UartCMD_ROS_ENTER_CRITICAL:
                drv_isr_isr_Enable(SYS_ISR_IF_DEMOD, _DISABLE);//ROS_ENTER_CRITICAL();
                drv_isr_isr_Enable(SYS_ISR_TIMER0, _DISABLE);
                drv_isr_isr_Enable(SYS_ISR_TIMER1, _DISABLE);
                drv_isr_isr_Enable(SYS_ISR_TIMER2, _DISABLE);
                drv_isr_uart_Write(nCommPort, 0x43);
                //rtd_isr_outl(0x5c30, 0x1234);
                break;

            case UartCMD_ROS_EXIT_CRITICAL:
                drv_isr_isr_Enable(SYS_ISR_IF_DEMOD, _ENABLE);//ROS_EXIT_CRITICAL();
                drv_isr_isr_Enable(SYS_ISR_TIMER0, _ENABLE);
                drv_isr_isr_Enable(SYS_ISR_TIMER1, _ENABLE);
                drv_isr_isr_Enable(SYS_ISR_TIMER2, _ENABLE);
                drv_isr_uart_Write(nCommPort, 0x44);
                //rtd_isr_outl(0x5c30, 0x5678);
                break;
            case UartCMD_TunerWrite:
                *((UINT8*)&getVal) = rcvBuffer[1];
                *((UINT8*)&getVal + 1) = rcvBuffer[2];
                *((UINT8*)&getVal + 2) = rcvBuffer[3];
                *((UINT8*)&getVal + 3) = rcvBuffer[4];
                if (rcvBuffer[5] == 1)
                {
#ifdef CONFIG_IFD_TUNER_AUTO
                    drvif_ifd_isr_set_standard(1, getVal);
#endif
                }
                else
                {
#ifdef CONFIG_IFD_TUNER_AUTO
                    drvif_ifd_isr_set_standard(0, getVal);
#endif
                }
                drv_isr_uart_Write(nCommPort, 0x34);
                rtd_isr_outl(0x5c30, getVal);
                break;

            case UartCMD_GET_HDCP_KEY_ADDR:
                drv_isr_uart_Write(nCommPort, 0);
                drv_isr_uart_Write(nCommPort, stflowbData.KEYCODE_BANK);
                drv_isr_uart_Write(nCommPort, *((UINT8*)&stflowbData.KEYCODE_HDCP_ADDR));
                drv_isr_uart_Write(nCommPort, *((UINT8*)&stflowbData.KEYCODE_HDCP_ADDR + 1));
                break;

            case UartCMD_GET_EDID_KEY_ADDR:
                drv_isr_uart_Write(nCommPort, 0);
                drv_isr_uart_Write(nCommPort, stflowbData.KEYCODE_BANK);
                drv_isr_uart_Write(nCommPort, *((UINT8*)&stflowbData.KEYCODE_EDID0_ADDR));
                drv_isr_uart_Write(nCommPort, *((UINT8*)&stflowbData.KEYCODE_EDID0_ADDR + 1));
                break;

            case UartCMD_IRKeyCommand:
                fw_isr_ui_message_push(rcvBuffer[1]);
                drv_isr_uart_Write(nCommPort, 0x42);
                break;

            case UartCMD_ROMCOMMAND:
                if (rcvBuffer[1] == UartCMD_ROMCOMMAND)
                    if (rcvBuffer[2] == UartCMD_ROMCOMMAND)
                    {
#ifdef USE_ROMCODE_COMMAND 	// not use ROMCode command
                        drv_isr_uart_Write(nCommPort, 'z');
                        fROMCodeMode = 1;
#else
                        drv_isr_isr_Enable(SYS_ISR_IF_DEMOD, _DISABLE);//ROS_ENTER_CRITICAL();
                        drv_isr_isr_Enable(SYS_ISR_TIMER0, _DISABLE);
                        drv_isr_isr_Enable(SYS_ISR_TIMER1, _DISABLE);
                        drv_isr_isr_Enable(SYS_ISR_TIMER2, _DISABLE);

                        rtd_isr_outl(TCWCR_0x2870, 0x000000A5);
                        rtd_isr_outl(WTST_0x2878, 0);

                        // Boot from flash after reboot
                        rtd_isr_maskb(ROM_CTRL_0x2da0, 0, _BIT1);
//							rtdf_setBitsb(ROM_CTRL_0x2da0, _BIT1);		// 0:flash, 1:romcode

                        // MCU_clock_control to reset MCU
//							rtdf_setBitsb(MCU_CLOCK_CONTROL_0xFFEE, _BIT1 | _BIT0);	// software reset (_BIT1), scaler hardware reset (_BIT0)
                        rtd_isr_maskb(MCU_CLOCK_CONTROL_0xFFEE, 0, _BIT1 | _BIT0);
#endif
                    }
                break;

            case UartCMD_ROSINFORMATION:
                model_monitor_ROSInformation(rcvBuffer[1]);
                break;

			case UartCMD_VIRTUALKEY:
				ucVirtualkey = rcvBuffer[1];
				break;

			case UartCMD_DS_TCON:
				if(rcvBuffer[1] == 0)
				{
					for(i=0;i<0x78;i++)
					{
						getTempVal = RtdCtrl_ReadScalerDataPort(_P0_TCON_ADDR_PORT_8B,i);
						drv_isr_uart_Write(nCommPort,i);
						drv_isr_uart_Write(nCommPort,getTempVal);
					}
				}
				else if(rcvBuffer[1] == 1)
				{
					RtdCtrl_WriteScalerDataPort(_P0_TCON_ADDR_PORT_8B, rcvBuffer[2], rcvBuffer[4]);
				}
				else if(rcvBuffer[1] == 2)
				{
					RtdCtrl_MaskScalerDataPort(_P0_TCON_ADDR_PORT_8B,rcvBuffer[2],rcvBuffer[3],rcvBuffer[4]);
				}
				break;


            };
            UartCount = 0;
        }
    }

    // Clear Pending status
    if (nCommPort)
        rtd_isr_outl(ISR_0x2504, _BIT3);
    else
        rtd_isr_outl(ISR_0x2504, _BIT2);

    return RT_ISR_HANDLED;
}
void drv_sparrow_CPU_Switch_Clock(bit type);
bit drv_fw_uart_handler(void)
{
    int count;
    UINT32 getVal;

#ifdef USE_ROMCODE_COMMAND 	// not use ROMCode command
    if (fROMCodeMode)
    {
        EA = 0;
        COsdFxDisableOsd();
        drv_isr_Enable(SYS_ISR_IF_DEMOD, _DISABLE);//ROS_ENTER_CRITICAL();
        drv_isr_Enable(SYS_ISR_TIMER0, _DISABLE);
        drv_isr_Enable(SYS_ISR_TIMER1, _DISABLE);
        drv_isr_Enable(SYS_ISR_TIMER2, _DISABLE);
        drv_isr_Enable(SYS_ISR_UART0, _DISABLE);
        drv_isr_Enable(SYS_ISR_UART1, _DISABLE);
        drv_isr_Enable(SYS_ISR_IRDA, _DISABLE);
        drv_isr_Enable(SYS_ISR_VBIOSD, _DISABLE);
        drv_isr_Enable(SYS_ISR_VBIPPR, _DISABLE);
        drv_isr_Enable(SYS_ISR_AUDIO, _DISABLE);
        //drv_timer_Watchdog_Enable(_FALSE);
        rtd_outl(WTST_0x2878, 0xa5a5a5a5);
        //drv_sparrow_CPU_Switch_Clock(_FALSE);
        while (1)
        ;
            //rom_monitor_Handler();
        EA = 1;
    }
#endif

    if (fDebugMode && fUartState == UartCMD_UNPROCESS_STATE)
    {
        EA = 0;
        fUartState = UartCMD_None_STATE;
        switch (rcvBuffer[0])
        {
        case UartCMD_I2CBus:
            I2C_MASTER = rcvBuffer[1];
            drv_uart_Write(nCommPort, 0x3a);
            break;

        case UartCMD_I2CWrite:
            drv_i2c_Set_TargetAddr(I2C_MASTER, rcvBuffer[2]);
            drv_i2c_Write(I2C_MASTER, rcvBuffer[1] + 1, &rcvBuffer[3]);
            // Return 0x32
            drv_uart_Write(nCommPort, 0x32);
            break;

        case UartCMD_I2CRead:
            if (rcvBuffer[1] != drv_i2c_Get_TargetAddr(I2C_MASTER))
                drv_i2c_Set_TargetAddr(I2C_MASTER, rcvBuffer[1]);
            drv_i2c_randomRead(I2C_MASTER, 1, rcvBuffer[2], (UINT8*)&rcvBuffer[1], (UINT8*)&rcvBuffer[3]);

            //Return count
            drv_uart_Write(nCommPort, rcvBuffer[1]);

            count = 0;
            while (count <  rcvBuffer[2])
            {
                drv_uart_Write(nCommPort, rcvBuffer[count+3]);
                count++;
            }
            break;

        case UartCMD_I2CRead_Current:
            if (UartCount == 0)  			// get device address
            {

                if (rcvBuffer[1] != drv_i2c_Get_TargetAddr(I2C_MASTER))
                    drv_i2c_Set_TargetAddr(I2C_MASTER, rcvBuffer[1]);

                drv_i2c_currRead(I2C_MASTER, rcvBuffer[2], &rcvBuffer[3]);
                //Return count
                drv_uart_Write(nCommPort, rcvBuffer[2]);

                count = 0;
                while (count < rcvBuffer[2])
                {
                    drv_uart_Write(nCommPort, rcvBuffer[count+3]);
                    count++;
                }
                break;

                /*			case UartCMD_TunerWrite:
                				*((UINT8*)&getVal) = rcvBuffer[1];
                				*((UINT8*)&getVal + 1) = rcvBuffer[2];
                				*((UINT8*)&getVal + 2) = rcvBuffer[3];
                				*((UINT8*)&getVal + 3) = rcvBuffer[4];
                				if(rcvBuffer[5] == 1){
                					drvif_ifd_set_video_standard(ZPAL_I, getVal);
                				}else{
                					drvif_ifd_set_video_standard(ZNTSC, getVal);
                				}
                				rtd_outl(0x5c30, getVal);
                				drv_uart_Write(UART_MONITOR_SOURCE, 0x34);
                				break;
                */
            case UartCMD_ScalerDisplayDomainAccess:
                // Read
                if (rcvBuffer[1] == 0)
                {
                    model_PanelAccessReadProc(rcvBuffer[0]);
                }
                else
                {
                    *((UINT8*)&getVal) = rcvBuffer[2];
                    *((UINT8*)&getVal + 1) = rcvBuffer[3];
                    *((UINT8*)&getVal + 2) = rcvBuffer[4];
                    *((UINT8*)&getVal + 3) = rcvBuffer[5];
                    model_PanelAccessWriteProc(rcvBuffer[0], getVal);
                }
                break;
            };
        }
        EA = 1;
        return _TRUE;
    }

    if (fDebugMode)
        return _TRUE;

    return _FALSE;
}

static void model_PanelAccessReadProc(UINT8 subcommand)
{
#if 1
    UINT32 getVal,tmp;
    UINT16 M_code = 0;
    UINT8 div=0,N_code = 0;
    getVal = 0;
    switch (subcommand)
    {
    case ScalerDisplayDomainAccess_DCLK:
        M_code = (UINT16)( rtdf_inl(PLL_DISP_0_0x2f00)>>20);
        N_code =(UINT8) (rtdf_inl(PLL_DISP_0_0x2f00) >>8);
        div = (UINT8) (rtdf_inl(PLL_DISP_0_0x2f00) >>16)&0x03;
        div = 2<<div;
        getVal = (M_code +2)*CONFIG_CPU_XTAL_FREQ/(N_code+2)/div;
        tmp = getVal >>15;
        tmp = tmp * rtdf_readWord1(DCLKSS_0x2f08);
        getVal = getVal -tmp;
        break;

    case ScalerDisplayDomainAccess_DCLK_M:
        getVal =  rtdf_inl(PLL_DISP_0_0x2f00)>>20;
        break;

    case ScalerDisplayDomainAccess_DCLK_N:
        getVal = (rtdf_inl(PLL_DISP_0_0x2f00) >>8) & 0xFF;
        break;

    case ScalerDisplayDomainAccess_DCLK_DIV:
        getVal = (rtdf_inl(PLL_DISP_0_0x2f00) >>16) & 0x03;
        break;

    case ScalerDisplayDomainAccess_DCLK_OFFSET:
        getVal = rtdf_readWord1(DCLKSS_0x2f08);
        break;

    case ScalerDisplayDomainAccess_DDS_ENABLE:
        getVal = rtdf_readByte0(DCLKSS_0x2f08)&0x01;
        break;

    case ScalerDisplayDomainAccess_DDS_RANGE:
        getVal = rtdf_readByte0(DCLKSS_0x2f08)>>4;
        break;

    case ScalerDisplayDomainAccess_FRC_MODE:
        getVal = (rtdf_inl(DISPLAY_TIMING_CTRL1_0x2000)>>26) & 0x01;
        break;

    case ScalerDisplayDomainAccess_DATA_PORT:
        getVal = (rtdf_inl(DISPLAY_TIMING_CTRL1_0x2000)>>12) & 0x01;
        break;

    case ScalerDisplayDomainAccess_COLOR_BITS:
        getVal = (rtdf_inl(DISPLAY_TIMING_CTRL1_0x2000)>>22) & 0x01;
        break;

    case ScalerDisplayDomainAccess_BITMAP_TABLE:
        rtdf_pageSelect(_PAGE0);
        getVal =	RtdCtrl_ReadScalerDataPort(_P0_TCON_ADDR_PORT_8B, _TCON_LVDS_CTRL3_A3);
        break;

    case ScalerDisplayDomainAccess_PORTAB_SWAP:
        getVal = (rtdf_inl(DHS_FINE_TUNIG_REGISTER_0x2004)>>6) & 0x01;
        break;

    case ScalerDisplayDomainAccess_REDBLUE_SWAP:
        getVal = (rtdf_inl(DISPLAY_TIMING_CTRL1_0x2000)>>7) & 0x01;
        break;

    case ScalerDisplayDomainAccess_MSBLSB_SWAP:
        getVal = (rtdf_inl(DISPLAY_TIMING_CTRL1_0x2000)>>6) & 0x01;
        break;

    case ScalerDisplayDomainAccess_OUTPUT_INV:
        getVal = (rtdf_inl(DISPLAY_TIMING_CTRL1_0x2000)>>4) & 0x01;
        break;

    case ScalerDisplayDomainAccess_H_HOTAL	:
        getVal = rtdf_readWord1(DH_TOTAL_LAST_LINE_LENGTH_0x200c)+1;
        break;

    case ScalerDisplayDomainAccess_V_HOTAL	:
        getVal = rtdf_readWord0(DV_TOTAL_0x2014)+1;
        break;

    case ScalerDisplayDomainAccess_LASTLINE:
        getVal = rtdf_readWord0(DH_TOTAL_LAST_LINE_LENGTH_0x200c)+1;
        break;

    case ScalerDisplayDomainAccess_HSYNC_WIDTH:
        getVal = rtdf_inl(DH_WIDTH_0x2008);
        break;

    case ScalerDisplayDomainAccess_VSYNC_LENGTH:
        getVal = rtdf_inl(DV_LENGTH_0x2010);
        break;

    case ScalerDisplayDomainAccess_ENABLE_HSTART:
        getVal = rtdf_readWord1(DH_DEN_START_END_0x2018);
        break;

    case ScalerDisplayDomainAccess_ENABLE_HEND:
        getVal = rtdf_readWord0(DH_DEN_START_END_0x2018);
        break;

    case ScalerDisplayDomainAccess_ENABLE_VSTART:
        getVal = rtdf_readWord1(DV_DEN_START_END_0x201c);
        break;

    case ScalerDisplayDomainAccess_ENABLE_VEND:
        getVal = rtdf_readWord0(DV_DEN_START_END_0x201c);
        break;

    case ScalerDisplayDomainAccess_ACTIVE_HSTART:
        getVal = rtdf_readWord1(MAIN_ACTIVE_H_START_END_0x2120);
        break;

    case ScalerDisplayDomainAccess_ACTIVE_HEND:
        getVal = rtdf_readWord0(MAIN_ACTIVE_H_START_END_0x2120);
        break;

    case ScalerDisplayDomainAccess_ACTIVE_VSTART:
        getVal = rtdf_readWord1(MAIN_ACTIVE_V_START_END_0x2124);
        break;

    case ScalerDisplayDomainAccess_ACTIVE_VEND:
        getVal = rtdf_readWord0(MAIN_ACTIVE_V_START_END_0x2124);
        break;

    case ScalerDisplayDomainAccess_DCLK_DELAY:
        getVal = rtdf_inl(DISPLAY_TIMING_CTRL1_0x2000)&0x07;
        break;

    case ScalerDisplayDomainAccess_PANEL_SWITCH:
        break;

    case ScalerDisplayDomainAccess_LVDS_SWITCH:
        break;

    case ScalerDisplayDomainAccess_LIGHT_SWITCH:
        break;

    case ScalerDisplayDomainAccess_FS_WATCHDOG_SWITCH:
        break;

    default:
        return;
    };

    drv_uart_Write(nCommPort, (getVal & 0xff000000) >> 24);
    drv_uart_Write(nCommPort, (getVal & 0x00ff0000) >> 16);
    drv_uart_Write(nCommPort, (getVal & 0x0000ff00) >> 8);
    drv_uart_Write(nCommPort, getVal & 0x000000ff);
#endif
}

static void model_PanelAccessWriteProc(UINT8 subcommand, UINT32 val)
{
#if 1
    switch (subcommand)
    {
    case ScalerDisplayDomainAccess_DCLK:
        drv_pll_Set_DClk(0,0, val*1000000);
        break;

    case ScalerDisplayDomainAccess_DCLK_M:
        rtdf_maskl(PLL_DISP_0_0x2f00,0x000FFFFF,(val-2)<<20);
        break;

    case ScalerDisplayDomainAccess_DCLK_N:
        rtdf_maskl(PLL_DISP_0_0x2f00,0xFFFF00FF,(val-2)<<8);
        break;

    case ScalerDisplayDomainAccess_DCLK_DIV:
        if (val == 1)
            rtdf_maskl(PLL_DISP_0_0x2f00,0xFFFCFFFF,0);
        else if (val == 2)
            rtdf_maskl(PLL_DISP_0_0x2f00,0xFFFCFFFF,1<<16);
        else if (val == 4)
            rtdf_maskl(PLL_DISP_0_0x2f00,0xFFFCFFFF,2<<16);
        else if (val == 8)
            rtdf_maskl(PLL_DISP_0_0x2f00,0xFFFCFFFF,3<<16);
        break;

    case ScalerDisplayDomainAccess_DCLK_OFFSET:
        rtdf_maskl(DCLKSS_0x2f08,0xF000FFFF,(val<<16)|0x05);
        break;

    case ScalerDisplayDomainAccess_DDS_ENABLE:
        rtdf_maskl(DCLKSS_0x2f08,0xFFFFFFFE,(val)|0x04);
        break;

    case ScalerDisplayDomainAccess_DDS_RANGE:
        rtdf_maskl(DCLKSS_0x2f08,0xFFFFFF0F,(val<<4)|0x05);
        break;

    case ScalerDisplayDomainAccess_FRC_MODE:
        rtdf_maskl(DISPLAY_TIMING_CTRL1_0x2000,~(_BIT26|_BIT29),val?(_BIT26):0);
        break;

    case ScalerDisplayDomainAccess_DATA_PORT:
        rtdf_maskl(DISPLAY_TIMING_CTRL1_0x2000,~(_BIT12|_BIT13|_BIT14|_BIT15),val<<12|val<<14);
        break;

    case ScalerDisplayDomainAccess_COLOR_BITS:
        rtdf_maskl(DISPLAY_TIMING_CTRL1_0x2000,~(_BIT22|_BIT23),val<<22);
        break;

    case ScalerDisplayDomainAccess_BITMAP_TABLE:
        rtdf_pageSelect(_PAGE0);
        if (val == 0)
            RtdCtrl_WriteScalerDataPort(_P0_TCON_ADDR_PORT_8B, _TCON_LVDS_CTRL3_A3, 0x9c);
        else
            RtdCtrl_WriteScalerDataPort(_P0_TCON_ADDR_PORT_8B, _TCON_LVDS_CTRL3_A3, 0x9d);
        break;

    case ScalerDisplayDomainAccess_PORTAB_SWAP:
        if (val == 0)
            rtdf_maskl(DHS_FINE_TUNIG_REGISTER_0x2004,0xFFFFF03F,0x0200);
        else
            rtdf_maskl(DHS_FINE_TUNIG_REGISTER_0x2004,0xFFFFF03F,0x40);
        break;

    case ScalerDisplayDomainAccess_REDBLUE_SWAP:
        rtdf_maskl(DISPLAY_TIMING_CTRL1_0x2000,~(_BIT7),val<<7);

        break;

    case ScalerDisplayDomainAccess_MSBLSB_SWAP:
        rtdf_maskl(DISPLAY_TIMING_CTRL1_0x2000,~(_BIT6),val<<6);
        break;

    case ScalerDisplayDomainAccess_OUTPUT_INV:
        rtdf_maskl(DISPLAY_TIMING_CTRL1_0x2000,~(_BIT4),val<<4);
        break;

    case ScalerDisplayDomainAccess_H_HOTAL	:
        rtdf_writeWord1(DH_TOTAL_LAST_LINE_LENGTH_0x200c, val-1);
        break;

    case ScalerDisplayDomainAccess_V_HOTAL	:
        rtdf_writeWord0(DV_TOTAL_0x2014, val-1);
        break;

    case ScalerDisplayDomainAccess_LASTLINE:
        rtdf_writeWord0(DH_TOTAL_LAST_LINE_LENGTH_0x200c, val-1);
        break;

    case ScalerDisplayDomainAccess_HSYNC_WIDTH:
        rtdf_outl(DH_WIDTH_0x2008, val);
        break;

    case ScalerDisplayDomainAccess_VSYNC_LENGTH:
        rtdf_outl(DV_LENGTH_0x2010, val);
        break;

    case ScalerDisplayDomainAccess_ENABLE_HSTART:
        rtdf_writeWord1(DH_DEN_START_END_0x2018, val);
        break;

    case ScalerDisplayDomainAccess_ENABLE_HEND:
        rtdf_writeWord0(DH_DEN_START_END_0x2018, val);
        break;

    case ScalerDisplayDomainAccess_ENABLE_VSTART:
        rtdf_writeWord1(DV_DEN_START_END_0x201c, val);
        break;

    case ScalerDisplayDomainAccess_ENABLE_VEND:
        rtdf_writeWord0(DV_DEN_START_END_0x201c, val);
        break;

    case ScalerDisplayDomainAccess_ACTIVE_HSTART:
        rtdf_writeWord1(MAIN_ACTIVE_H_START_END_0x2120, val);
        break;

    case ScalerDisplayDomainAccess_ACTIVE_HEND:
        rtdf_writeWord0(MAIN_ACTIVE_H_START_END_0x2120, val);
        break;

    case ScalerDisplayDomainAccess_ACTIVE_VSTART:
        rtdf_writeWord1(MAIN_ACTIVE_V_START_END_0x2124, val);
        break;

    case ScalerDisplayDomainAccess_ACTIVE_VEND:
        rtdf_writeWord0(MAIN_ACTIVE_V_START_END_0x2124, val);
        break;

    case ScalerDisplayDomainAccess_DCLK_DELAY:
        rtdf_maskl(DISPLAY_TIMING_CTRL1_0x2000,~(_BIT0|_BIT1|_BIT2),val);
        break;

    case ScalerDisplayDomainAccess_PANEL_SWITCH:
        if (val)
            pcb_ctrl_PanelPowerOn();
        else
            pcb_ctrl_PanelPowerOff();
        break;

    case ScalerDisplayDomainAccess_LVDS_SWITCH:
        if (val)
            drv_panel_PowerDisplayPortOn();
        else
            drv_panel_PowerDisplayPortOff();
        break;

    case ScalerDisplayDomainAccess_LIGHT_SWITCH:
        if (val)
            pcb_ctrl_LightPowerOn();
        else
            pcb_ctrl_LightPowerOff();
        break;

    case ScalerDisplayDomainAccess_FS_WATCHDOG_SWITCH:
        break;

    default:
        return;
    };
    val = 0;
    drv_uart_Write(nCommPort, 0x4a);
#endif
}

static void model_monitor_GetRosVersion(void)
{
#if 0
    UINT8 nValue;
    UINT8  strData[8];
    int i;

#ifdef SYS_COMPILER_VERSION
    drv_sparrow_memcpy(strData,
                       strcpy(strData, ROS_COMPILER_VERSION);
#else
    strcpy(strData, "0");
#endif
                       nValue = strlen(strData);
                       Scaler_Putchar(nValue);

                       for (i = 0; i < nValue; i++)
                       Scaler_Putchar(strData[i]);
#endif
                   }

                   static void model_monitor_GetRosBuildDate(void)
{
#if 0
    UINT8 nValue, nValue1;
    UINT8  strData[8], strData1[8];
    int i;

#ifdef SYS_COMPILER_DATE
    strcpy(strData, ROS_COMPILER_DATE);
#else
    strcpy(strData, "1990");
#endif
    nValue = strlen(strData);

#ifdef SYS_COMPILER_CLOCK
    strcpy(strData1, ROS_COMPILER_CLOCK);
#else
    strcpy(strData1, "1200");
#endif
    nValue1 = strlen(strData1);
    Scaler_Putchar(nValue + nValue1);

    for (i = 0; i < nValue; i++)
        Scaler_Putchar(strData[i]);

    for (i = 0; i < nValue1; i++)
        Scaler_Putchar(strData1[i]);
#endif
}

void model_monitor_GetRosUser(void)
{
#if 0
    UINT8 nValue;
    UINT8  strData[32];
    int i;

#ifdef SYS_COMPILER_USERNAME
    strcpy(strData, ROS_COMPILER_USERNAME);
#else
    strcpy(strData, "Realtek");
#endif
    nValue = strlen(strData);
    if (nValue > 32)
        nValue = 32;

    Scaler_Putchar(nValue);

    for (i = 0; i < nValue; i++)
        Scaler_Putchar(strData[i]);
#endif
}

static void model_monitor_ROSInformation(UINT8 Cmd)
{
    switch (Cmd)
    {
    case 0:
        model_monitor_GetRosVersion();
        model_monitor_GetRosBuildDate();
        model_monitor_GetRosUser();
        break;

    case 1:
        model_monitor_GetRosVersion();
        break;

    case 2:
        model_monitor_GetRosBuildDate();
        break;

    case 3:
        model_monitor_GetRosUser();
    };
}

