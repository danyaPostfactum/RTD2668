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
#include "sparrow.h"
#include "scaler\scaler.h"
#include "reg52.h"
#include <reg_def.h>	/* Provides: MCU_SCA_INF_ADDR */
#include "autoconf.h"

#if 0
/*============================================================================*/
/**
 * RtdCtrl_Read
 * Write 4-byte data from address
 *
 * @param <addr>		{ the start address to write the data }
 * @return 				{ Value in address }
 *
 */
UINT32 RtdCtrl_Read(UINT16 addr)
{
    UINT8 getVal[4];
    getVal[3] = *((UINT8 xdata*) addr);
    getVal[2] = *((UINT8 xdata*) addr + 1);
    getVal[1] = *((UINT8 xdata*) addr + 2);
    getVal[0] = *((UINT8 xdata*) addr + 3);

    return *((UINT32*)getVal);
}

UINT32 RtdCtrl_isr_Read(UINT16 addr)
{
    UINT8 getVal[4];
    getVal[3] = *((UINT8 xdata*) addr);
    getVal[2] = *((UINT8 xdata*) addr + 1);
    getVal[1] = *((UINT8 xdata*) addr + 2);
    getVal[0] = *((UINT8 xdata*) addr + 3);

    return *((UINT32*)getVal);
}

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
void RtdCtrl_Write(UINT16 addr, UINT32 val)
{
    *((UINT8 xdata*) addr) = *((UINT8 xdata*)&val + 3);
    *((UINT8 xdata*) addr + 1) = *((UINT8 xdata*)&val + 2);
    *((UINT8 xdata*) addr + 2) = *((UINT8 xdata*)&val + 1);
    *((UINT8 xdata*) addr + 3) = *((UINT8 xdata*)&val);
}

void RtdCtrl_isr_Write(UINT16 addr, UINT32 val)
{
    *((UINT8 xdata*) addr) = *((UINT8 xdata*)&val + 3);
    *((UINT8 xdata*) addr + 1) = *((UINT8 xdata*)&val + 2);
    *((UINT8 xdata*) addr + 2) = *((UINT8 xdata*)&val + 1);
    *((UINT8 xdata*) addr + 3) = *((UINT8 xdata*)&val);
}

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
void RtdCtrl_Mask(UINT16 addr, UINT32 and, UINT32 or)
{
#if 0
    UINT32 getVal;
    getVal = RtdCtrl_Read(addr);

    getVal &= and;
    getVal |= or;

    RtdCtrl_Write(addr, getVal);
#else
    UINT8 getVal[4];
    getVal[0] = *((UINT8 xdata*)addr);
    getVal[1] = *((UINT8 xdata*)addr + 1);
    getVal[2] = *((UINT8 xdata*)addr + 2);
    getVal[3] = *((UINT8 xdata*)addr + 3);

    getVal[0] = (getVal[0] & *(((UINT8*)&and + 3))) | *(((UINT8*)&or + 3));
    *((UINT8 xdata*)addr) = getVal[0];

    getVal[1] = (getVal[1] & *(((UINT8*)&and + 2))) | *(((UINT8*)&or + 2));
    *(((UINT8 xdata*)addr + 1)) = getVal[1];

    getVal[2] = (getVal[2] & *(((UINT8*)&and + 1))) | *(((UINT8*)&or + 1));
    *(((UINT8 xdata*)addr + 2)) = getVal[2];

    getVal[3] = (getVal[3] & *((UINT8*)&and)) | *((UINT8*)&or);
    *(((UINT8 xdata*)addr + 3)) = getVal[3];
#endif
}
#endif
void RtdCtrl_isr_Mask(UINT16 addr, UINT32 and, UINT32 or)// reentrant 
{
    UINT8 getVal[4];
    getVal[0] = *((UINT8 xdata*)addr);
    getVal[1] = *((UINT8 xdata*)addr + 1);
    getVal[2] = *((UINT8 xdata*)addr + 2);
    getVal[3] = *((UINT8 xdata*)addr + 3);

    getVal[0] = (getVal[0] & *(((UINT8*)&and + 3))) | *(((UINT8*)&or + 3));
    *((UINT8 xdata*)addr) = getVal[0];

    getVal[1] = (getVal[1] & *(((UINT8*)&and + 2))) | *(((UINT8*)&or + 2));
    *(((UINT8 xdata*)addr + 1)) = getVal[1];

    getVal[2] = (getVal[2] & *(((UINT8*)&and + 1))) | *(((UINT8*)&or + 1));
    *(((UINT8 xdata*)addr + 2)) = getVal[2];

    getVal[3] = (getVal[3] & *((UINT8*)&and)) | *((UINT8*)&or);
    *(((UINT8 xdata*)addr + 3)) = getVal[3];
}
#if 0
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
void RtdCtrl_MaskByte(UINT16 addr, UINT8 and, UINT8 or)
{
    UINT8 getVal;
    getVal = *((volatile UINT8 xdata*)addr);
    getVal &= and;
    getVal |= or;
    *((volatile UINT8 xdata*)addr) = getVal;
}

void RtdCtrl_isr_MaskByte(UINT16 addr, UINT8 and, UINT8 or)
{
    UINT8 getVal;
    getVal = *((volatile UINT8 xdata*)addr);
    getVal &= and;
    getVal |= or;
    *((volatile UINT8 xdata*)addr) = getVal;
}
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
UINT8 RtdCtrl_ReadScalerPort(UINT8 addr)
{
    volatile bit tmpEA = EA;
    UINT8 getVal;

    EA = 0;
    rtd_outb(MCU_SCA_INF_ADDR_0xfff4, addr & 0xff);

    getVal = rtd_inb(MCU_SCA_INF_DATA_0xfff5);

    EA = tmpEA;

    return getVal;
}

/*============================================================================*/
/**
 * RtdCtrl_ReadDataPort
 * Read via data port
 *
 * @param <addr>			{ Address to read }
 * @return 				{ return data }
 *
 */
UINT8 RtdCtrl_isr_ReadScalerPort(UINT8 addr)
{
    rtd_outb(MCU_SCA_INF_ADDR_0xfff4, addr & 0xff);

    return rtd_inb(MCU_SCA_INF_DATA_0xfff5);
}


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
void RtdCtrl_WriteScalerPort(UINT8 addr, UINT8 val)
{
    volatile bit tmpEA = EA;

    EA = 0;
    rtd_outb(MCU_SCA_INF_ADDR_0xfff4, addr & 0xff);

    rtd_outb(MCU_SCA_INF_DATA_0xfff5, val & 0xff);

    EA = tmpEA;
}

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
void RtdCtrl_isr_WriteScalerPort(UINT8 addr, UINT8 val)
{
    rtd_outb(MCU_SCA_INF_ADDR_0xfff4, addr & 0xff);

    rtd_outb(MCU_SCA_INF_DATA_0xfff5, val & 0xff);
}


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
void RtdCtrl_setBitsScalerPort(UINT8 addr, UINT8 val)
{
    UINT8 getVal;
    getVal = RtdCtrl_ReadScalerPort(addr);
    getVal |= val;
    RtdCtrl_WriteScalerPort(addr, getVal);
}

#if 0	// not used
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
void RtdCtrl_isr_setBitsScalerPort(UINT8 addr, UINT8 val)
{
    UINT8 getVal;
    getVal = RtdCtrl_isr_ReadScalerPort(addr);
    getVal |= val;
    RtdCtrl_isr_WriteScalerPort(addr, getVal);
}
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
void RtdCtrl_clearBitsScalerPort(UINT8 addr, UINT8 val)
{
    UINT8 getVal;
    getVal = RtdCtrl_ReadScalerPort(addr);
    getVal &= ~val;
    RtdCtrl_WriteScalerPort(addr, getVal);
}

#if 0	// not used
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
void RtdCtrl_isr_clearBitsScalerPort(UINT8 addr, UINT8 val)
{
    UINT8 getVal;
    getVal = RtdCtrl_isr_ReadScalerPort(addr);
    getVal &= ~val;
    RtdCtrl_isr_WriteScalerPort(addr, getVal);
}
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
void RtdCtrl_maskScalerPort(UINT8 addr, UINT8 and, UINT8 or)
{
    UINT8 getVal;
    getVal = RtdCtrl_ReadScalerPort(addr);

    getVal &= and;
    getVal |= or;
    RtdCtrl_WriteScalerPort(addr, getVal);
}

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
#ifdef CONFIG_ENABLE_HDMI
void RtdCtrl_isr_maskScalerPort(UINT8 addr, UINT8 and, UINT8 or)
{
    UINT8 getVal;
    getVal = RtdCtrl_isr_ReadScalerPort(addr);

    getVal &= and;
    getVal |= or;
    RtdCtrl_isr_WriteScalerPort(addr, getVal);
}
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
UINT8 RtdCtrl_ReadScalerDataPort(UINT8 addr, UINT8 index)
{
    RtdCtrl_WriteScalerPort(addr, index);
    return RtdCtrl_ReadScalerPort(addr + 1);
}


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
void RtdCtrl_WriteScalerDataPort(UINT8 addr, UINT8 index, UINT8 val)
{
    RtdCtrl_WriteScalerPort(addr, index);
    RtdCtrl_WriteScalerPort(addr + 1, val);
}

//--------------------------------------------------
// Description  : Write a table into scaler
// Input Value  : pArray    --> Selected table which contains numbers, address auto increasing information, address of registers and values
// Output Value : None
//--------------------------------------------------
void RtdCtrl_WriteCodeWScalerPort(UINT8 code *pArray)
{
    UINT8 length;
    volatile bit tmpEA = EA;

    do
    {
        if ((*pArray & 0xfc) == 0)
            return;

        length  = *pArray - 3;

        if ((*(pArray + 1)) == _BURST)
        {
            // set address no auto inc
            rtdf_setBitsb(MCU_SCA_INF_CONTROL_0xfff3, _BIT5);

            EA = 0;
            // Write address in scaler
            rtd_outb(MCU_SCA_INF_ADDR_0xfff4, *(pArray + 2) & 0xff);
            pArray += 3;
            // Burst to write
            do
            {
                rtd_outb(MCU_SCA_INF_DATA_0xfff5, *pArray & 0xff);

            }
            while (--length);
            EA = tmpEA;

            pArray++;
        }
        else if ((*(pArray + 1) == _AUTO_INC) || (*(pArray + 1) == _NON_INC))
        {

            if (*(pArray + 1) == _NON_INC)
                rtdf_setBitsb(MCU_SCA_INF_CONTROL_0xfff3, _BIT5);
            else
                rtdf_clearBitsb(MCU_SCA_INF_CONTROL_0xfff3, _BIT5);

            EA = 0;
            rtd_outb(MCU_SCA_INF_ADDR_0xfff4, *(pArray + 2) & 0xff);

            pArray += 3;

            do
            {
                rtd_outb(MCU_SCA_INF_DATA_0xfff5, *pArray++ & 0xff);

            }
            while (--length);

            EA = tmpEA;
        }
    }
    while (_TRUE);
}

void RtdCtrl_CScalerWriteScalerPort(UINT8 ucAddr, UINT16 usLength, UINT8 *pArray, bit bAutoInc)
{
    volatile bit tmpEA = EA;

    // set address auto inc
    if (bAutoInc)
        rtdf_setBitsb(MCU_SCA_INF_CONTROL_0xfff3, _BIT5);
    else
        rtdf_clearBitsb(MCU_SCA_INF_CONTROL_0xfff3, _BIT5);

    EA = 0;
    if (usLength > 0)
    {
        rtd_outb(MCU_SCA_INF_ADDR_0xfff4, ucAddr);
        do
        {
            rtd_outb(MCU_SCA_INF_DATA_0xfff5, *pArray++);
        }
        while (--usLength);
    }
    EA = tmpEA;
}
void RtdCtrl_CScalerSendAddr(UINT8 ucAddr, UINT8 value, bit bAutoInc)
{
    volatile bit tmpEA = EA;

    // set address auto inc
    if (bAutoInc)
        rtdf_setBitsb(MCU_SCA_INF_CONTROL_0xfff3, _BIT5);
    else
        rtdf_clearBitsb(MCU_SCA_INF_CONTROL_0xfff3, _BIT5);

    EA = 0;
    rtd_outb(MCU_SCA_INF_ADDR_0xfff4, ucAddr);
    rtd_outb(MCU_SCA_INF_DATA_0xfff5, value);
    EA = tmpEA;
}

/*********************************************************************
** 函数名称: void RtdCtrl_ReadScalerDataPort(UINT8 addr, UINT8 index)
** 功能描述: 	
** 输　  入: 无
** 输　  出: 无 
**********************************************************************/
void RtdCtrl_MaskScalerDataPort(UINT8 addr, UINT8 index, UINT8 and, UINT8 or)
{
	UINT8 value;

    value = RtdCtrl_ReadScalerDataPort(addr,index);
    value &= and;
    value |= or;
    RtdCtrl_WriteScalerDataPort(addr,index,value);
}

