/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2006
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

#ifndef  _RTDCTRL_H_
#define  _RTDCTRL_H_

#include "rtd_types.h"
#include "reg_def.h"
#include "autoconf.h"

/*============================================================================*/
/**
 * RtdCtrl_ReadByte
 * Write byte data from address
 *
 * @param <addr>		{ the start address to write the data }
 * @return 				{ Value in address }
 *
 */
// UINT8 RtdCtrl_ReadByte(UINT16 addr);
#define RtdCtrl_ReadByte(addr) (*((volatile UINT8 xdata*) addr))

/*============================================================================*/
/**
 * RtdCtrl_Read
 * Write 4-byte data from address
 *
 * @param <addr>		{ the start address to write the data }
 * @return 				{ Value in address }
 *
 */
UINT32 RtdCtrl_Read(UINT16 addr);
UINT32 RtdCtrl_isr_Read(UINT16 addr);

/*============================================================================*/
/**
 * RtdCtrl_WriteByte
 * Write byte data to address
 *
 * @param <addr>		{ the start address to write the data }
 * @param <val>			{ The data to be writeten is put in pData }
 * @return 				{ void }
 *
 */
// void RtdCtrl_WriteByte(UINT16 addr, UINT8 val);
#define RtdCtrl_WriteByte(addr, val) {*((volatile UINT8 xdata*) addr) = val;}

/*============================================================================*/
/**
 * RtdCtrl_Write
 * Write 4-byte data to address
 *
 * @param <addr>		{ the start address to write the data }
 * @param <val>			{ The data to be writeten is put in pData }
 * @return 				{ void }
 *
 */
void RtdCtrl_Write(UINT16 addr, UINT32 val);
void RtdCtrl_isr_Write(UINT16 addr, UINT32 val);

/*============================================================================*/
/**
 * RtdCtrl_Mask
 * Do AND and OR operation in 4-byte data
 *
 * @param <addr>		{ Address to do and & or }
 * @param <and>			{ and in the value of address }
 * @param <or>			{ or in the value of address }
 * @return 				{ void }
 *
 */
void RtdCtrl_Mask(UINT16 addr, UINT32 and, UINT32 or);
void RtdCtrl_isr_Mask(UINT16 addr, UINT32 and, UINT32 or);

/*============================================================================*/
/**
 * RtdCtrl_MaskByte
 * Do AND and OR operation in byte data
 *
 * @param <addr>		{ Address to do and & or }
 * @param <and>			{ and in the value of address }
 * @param <or>			{ or in the value of address }
 * @return 				{ void }
 *
 */
void RtdCtrl_MaskByte(UINT16 addr, UINT8 and, UINT8 or);
void RtdCtrl_isr_MaskByte(UINT16 addr, UINT8 and, UINT8 or);

/*============================================================================*/
/**
 * RtdCtrl_ReadDataPort
 * Read via data port
 *
 * @param <addr>			{ Address to read }
 * @return 				{ return data }
 *
 */
UINT8 RtdCtrl_ReadScalerPort(UINT8 addr);
UINT8 RtdCtrl_isr_ReadScalerPort(UINT8 addr);

/*============================================================================*/
/**
 * RtdCtrl_WriteDataPort
 * Write data via data port
 *
 * @param <addr>			{ Address to write}
 * @param <data>			{ data  to write }
 * @return 				{ void }
 *
 */
void RtdCtrl_WriteScalerPort(UINT8 addr, UINT8 val);
void RtdCtrl_isr_WriteScalerPort(UINT8 addr, UINT8 val);

/*============================================================================*/
/**
 * RtdCtrl_setbitsDataPort
 * mask  data via data port
 *
 * @param <addr>			{ Address to write}
 * @param <data>			{ data  to write }
 * @return 				{ void }
 *
 */
void RtdCtrl_setBitsScalerPort(UINT8 addr, UINT8 val);
#if 0	// not used
void RtdCtrl_isr_setBitsScalerPort(UINT8 addr, UINT8 val);
#endif
/*============================================================================*/
/**
 * RtdCtrl_setbitsDataPort
 * mask  data via data port
 *
 * @param <addr>			{ Address to write}
 * @param <data>			{ data  to write }
 * @return 				{ void }
 *
 */
void RtdCtrl_clearBitsScalerPort(UINT8 addr, UINT8 val);
#if 0	// not used
void RtdCtrl_isr_clearBitsScalerPort(UINT8 addr, UINT8 val);
#endif
/*============================================================================*/
/**
 * RtdCtrl_maskDataPort
 * mask  data via data port
 *
 * @param <addr>			{ Address to write}
 * @param <data>			{ data  to write }
 * @return 				{ void }
 *
 */
void RtdCtrl_maskScalerPort(UINT8 addr, UINT8 and, UINT8 or);
#ifdef CONFIG_ENABLE_HDMI
void RtdCtrl_isr_maskScalerPort(UINT8 addr, UINT8 and, UINT8 or);
#endif
/*============================================================================*/
/**
 * RtdCtrl_ReadDataPort
 * Read via data port
 *
 * @param <addr>			{ Address to read }
 * @return 				{ return data }
 *
 */
UINT8 RtdCtrl_ReadScalerDataPort(UINT8 addr, UINT8 index);

/*============================================================================*/
/**
 * RtdCtrl_WriteDataPort
 * Write data via data port
 *
 * @param <addr>			{ Address to write}
 * @param <data>			{ data  to write }
 * @return 				{ void }
 *
 */
void RtdCtrl_WriteScalerDataPort(UINT8 addr, UINT8 index, UINT8 val);

//--------------------------------------------------
// Description  : Write a table into scaler
// Input Value  : pArray    --> Selected table which contains numbers, address auto increasing information, address of registers and values
// Output Value : None
//--------------------------------------------------
void RtdCtrl_WriteCodeWScalerPort(UINT8 code *pArray);

//--------------------------------------------------
// Description  : Write a data array into registers of scaler
// Input Value  : ucAddr    --> Start address of register
//                ucLength  --> Numbers of data we want to write
//                pValue    --> Value we want to write
//                bAutoInc  --> Address auto increasing select
// Output Value : None
//--------------------------------------------------
void RtdCtrl_CScalerWriteScalerPort(UINT8 ucAddr, UINT16 usLength, UINT8 *pArray, bit bAutoInc);
void RtdCtrl_CScalerSendAddr(UINT8 ucAddr, UINT8 value, bit bAutoInc);
#if 0	// not used
//
void RtdCtrl_WriteHostPort(UINT32 ucAddr, UINT8 val);
UINT8 RtdCtrl_ReadHostPort(UINT32 ucAddr);
#endif
void RtdCtrl_MaskScalerDataPort(UINT8 addr, UINT8 index, UINT8 and, UINT8 or);
#endif // #ifndef  _RTDCTRL_H_
