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

#ifndef  _IRRC_H_
#define  _IRRC_H_
#include "sparrow.h"

/*==================== Data structures =======================*/
#define IRRC_MAX_BUF 	4

/**
 *An enum.
 *The IrDA remote controller protocols.
 */
typedef enum  IrrcProtocols_t
{
    IRDA_NEC = 0,
    IRDA_RC5,
    IRDA_Sharp,
    IRDA_SonySIRC,
    IRDA_Customer
} IrrcProtocols;

/**
 *A struct.
 *The IrDA remote controller settings.
 */
typedef struct _STRUCT_IR_PARAMETER
{
    //  IR_PSR Reg:0x2880
    UINT8 IRRBL;			// Remote burst length
    UINT8 IRRML;			// Remote modulation length
    UINT8 IRRDZL;			// Remote data zero length
    UINT8 IRRDOL;			// Remote data one length

    // IR_PER Reg:0x2884
    UINT8 IRRDL:6;			// Remote de-bounce length
    UINT8 IRRRL;			// Remote repeat length
    UINT8 IRRSL;				// Remote silence length

    // IR_SF Reg:0x2888
    UINT8 IRIT;				// Idle Time
    UINT8 IROCT;			// One Command Time
    UINT16 IRFD;				// Frequency Divisor to generate IR sampling frequency

    // IR_CR  Reg:0x2980
    UINT8 IREDNM:1;			// Extended Data Number Mode
    UINT8 IREDN:6;			// Extended Data Number
    UINT8 MLAE:1;			// Modulation length adjustable enable
    UINT8 IRRES:1;			// IR RX endian select
    UINT8 IRCM:1;			// Coding mode
    UINT8 IRBME:1;			// Burst mode enable
    UINT8 IRDPM:1;			// Data phase mode
    UINT8 IRDN:5;			// Data number, number of bits in one data

    // IR_SR	Reg:0x2898
    UINT8 IRRF:1;				// Repeat flag (NEC only)
} STRUCT_IR_PARAMETER;

/*============================================================================*/
/**
 * drv_irrc_Isr
 * Initial uart, baud rate, data bits..,etc.
 *
 * @param <void>		{ void }
 * @return 				{ void }
 *
 */
bit drv_irrc_Isr(void);

/*============================================================================*/
/**
 * drv_irrc_Init
 * Initial uart, baud rate, data bits..,etc.
 *
 * @param <void>		{ void }
 * @return 				{ void }
 *
 */
void drv_irrc_Init(void);

/*============================================================================*/
/**
 * drv_irrc_Read
 * Read data from uart
 *
 * @param <void>		{ void }
 * @return UINT8		{ data read from uart}
 *
 */
bit drv_irrc_Read(UINT8* uart_data);
void IRRC_isr_Command_Mapping(UINT32 irrc_command, UINT32 IRstate);
void IRRC_Command_Mapping(UINT32 irrc_command, UINT8 *key_message);
void fw_IR_detectmode_change(bit type);

#endif // #ifndef  _IRRC_H_
