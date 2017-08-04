/*=============================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2005
  * All rights reserved.
  * ============================================ */

/*================- File Description ================= */
/**
 * @file
 * 	This file mounitor handler via uart.
 *
 * @author	 	$Author$
 * @date	 	$Date$
 * @version 	$Revision$: 2103 $
 * @ingroup 	uart
 */

/**
* @addtogroup monitor
* @{
*/

#ifndef _MONITOR_H_
#define _MONITOR_H_

#include "sparrow.h"

/*============================ Module dependency  ===========================*/

extern volatile bit fDebugMode;

/*================================ Definitions ==============================*/

/*================================== Function ===============================*/
void fw_monitor_system_Init(void);
void fw_monitor_Init(void);
/*============================================================================*/
/**
 * ScalerUart_DebugLoop
 * Init Uart module
 *
 * @param <void>		{ void }
 * @return 			{ void }
 *
 * @Note
 * The followings are protocol command.
 *    1.into  debug mode  0x5a,0x01,0x02,0x03,0x04
 *    2.exit  debug mode  0x5b,0x04,0x03,0x02,0x01
 *    3.read  RTD 		0x5c,0x74,address,count,index
 *    4.read  other   	0x5c,slave address,address,count,index
 *    5.write rtd 		0x5d,0x74,address,value,rever
 *    6.write other 		0x5d,slave address,address,value,rever
 *  NEW RS232 COMMAND
 *     1. Enter Deug Mode	  :  0x10,0x01,0x02,0x03,0x04
 *     2. Exit Debug  Mode     :  0x11,0x04,0x03,0x02,0x01
 *     3. Read RTD2620   	  :  0x20,Address,Count,Inc
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
 *	     Aceept data for Xmodem: 0x2b
 *    13. Write RTD2620  	  :  0x30,Address,Data
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
 */
bit drv_isr_uart_handler(void);
bit drv_fw_uart_handler(void);

#endif // #ifndef _SCALER_UART_H_
/*======================== End of File =======================================*/
/**
*
* @}
*/


