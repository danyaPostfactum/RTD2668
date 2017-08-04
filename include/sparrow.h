/*=============================================================
 * Copyright C        Realtek Semiconductor Corporation, 2005 *
 * All rights reserved.                                       *
 *============================================================*/

/*======================= Description ========================*/
/**
 * @file
 *   This header file for Sparrow's registers define.
 * @Author Tzungder Lin
 * @date 2005/06/30
 * @version { 1.0 }
 *
 */

#ifndef _SPARROW_H
#define _SPARROW_H

#include "autoconf.h"
#include "rtd_types.h"
#include <stddef.h>
#include "rtdctrl.h"
#include "DragonSource.h"

#define CPU_CLOCK           (98000*1000)
#define CRYSTAL_CLOCK       (27000*1000)

#define CONFIG_VGA_SUPPORT_SOG 0
#define CLAMP_NEW_CLAMP_POSITION 1
#define CONFIG_YPBPR_AUTO_COLOR 1
//--------------------------------------------------
// Global Macros
//--------------------------------------------------
#define SIZEOF_STRUCT(strct_name, element)	\
		sizeof(((strct_name*)0)->element)

#define OFFSET_STRUCT(strct_name, element)	\
		offsetof(strct_name, element)

#define LOBYTE(w)                       		((UINT8)(w))
#define HIBYTE(w)                       		((UINT8)(((UINT16)(w) >> 8) & 0x00FF))
#define SEC(x)                          		(1000 * x)//(100 * x)
#define SHL(x,y)			    	    		((x) << (y))
#define SHR(x,y)				        	((x) >> (y))
#define AND(n1,n2)  			        	((n1) & (n2))
#define OR(n1,n2)   			        	((n1) | (n2))
#define ABS(x,y)						((x > y) ? (x-y) : (y-x))
#define abs1(x) 						(x<0)?-(x):(x)

#define BIT(x)	(1 << (x))

#define rtd_inb(offset)			RtdCtrl_ReadByte(offset)
#define rtd_inl(offset)			RtdCtrl_Read(offset)

#define rtd_outb(offset,val)	RtdCtrl_WriteByte(offset,val)
#define rtd_outl(offset,val)		RtdCtrl_Write(offset,val)

#define rtdf_inb(offset)		rtd_inb(offset)
#define rtdf_inl(offset)			rtd_inl(offset)

#define rtdf_outb(offset,val)	rtd_outb(offset,val)
#define rtdf_outl(offset,val)	rtd_outl(offset, (UINT32)val)

#define rtd_maskl(offset, andMask, orMask) RtdCtrl_Mask(offset, andMask, orMask) //rtd_outl(offset, ((rtd_inl(offset) & (andMask)) | (orMask)))
#define rtdf_maskl(offset, andMask, orMask) rtd_maskl(offset, andMask, orMask) //rtd_outl(offset, ((rtd_inl(offset) & (andMask)) | (orMask)))
#define rtd_maskb(offset, andMask, orMask) RtdCtrl_MaskByte(offset, andMask, orMask) // rtd_outb(offset, ((rtd_inb(offset) & (andMask)) | (orMask)))
#define rtdf_maskb(offset, andMask, orMask) rtd_maskb(offset, andMask, orMask) // rtd_outb(offset, ((rtd_inb(offset) & (andMask)) | (orMask)))

#define rtdf_setBitsb(offset, Mask) 		RtdCtrl_MaskByte(offset, 0xff, Mask) // rtd_outb(offset, (rtd_inb(offset) |Mask))
#define rtdf_clearBitsb(offset, Mask) 		RtdCtrl_MaskByte(offset, ~Mask, 0) // rtd_outb(offset, ((rtd_inb(offset) & ~(Mask))))

#define rtdf_setBits(offset, Mask) 			RtdCtrl_Mask(offset, 0xffffffff, (UINT32)Mask) // rtd_outl(offset, (rtd_inl(offset) |Mask))
#define rtdf_clearBits(offset, Mask) 		RtdCtrl_Mask(offset, ~((UINT32)Mask), 0) // rtd_outl(offset, ((rtd_inl(offset) & ~(Mask))))

#define rtdf_readByte0(offset)  (rtd_inl(offset) & 0x000000ff)
#define rtdf_readByte1(offset)  ((rtd_inl(offset) & 0x0000ff00) >> 8)
#define rtdf_readByte2(offset)  ((rtd_inl(offset) & 0x00ff0000) >> 16)
#define rtdf_readByte3(offset)  ((rtd_inl(offset) & 0xff000000) >> 24)
#define rtdf_readWord0(offset)  (rtd_inl(offset) & 0x0000ffff)
#define rtdf_readWord1(offset)  ((rtd_inl(offset) & 0xffff0000) >> 16)

#define rtdf_writeByte0(offset,val)  rtd_outl(offset, (rtd_inl(offset) & 0xffffff00) | (val & 0x000000ff))
#define rtdf_writeByte1(offset,val)  rtd_outl(offset, (rtd_inl(offset) & 0xffff00ff) | ((UINT32)(val & 0x000000ff) << 8))
#define rtdf_writeByte2(offset,val)  rtd_outl(offset, (rtd_inl(offset) & 0xff00ffff) | ((UINT32)(val & 0x000000ff) << 16))
#define rtdf_writeByte3(offset,val)  rtd_outl(offset, (rtd_inl(offset) & 0x00ffffff) | ((UINT32)(val & 0x000000ff) << 24))
#define rtdf_writeWord0(offset,val)  rtd_outl(offset, (rtd_inl(offset) & 0xffff0000) | (val & 0x0000ffff))
#define rtdf_writeWord1(offset,val)  rtd_outl(offset, (rtd_inl(offset) & 0x0000ffff) | ((UINT32)(val & 0x0000ffff) << 16))

// Access function for monitor ip
#define rtdf_pageSelect(page)			RtdCtrl_WriteScalerPort(Page_Select_0x009f, page)

#define rtdf_writeScalerPort(addr, val)	RtdCtrl_WriteScalerPort(addr, val)
#define rtd_writeScalerPort(addr, val)	rtdf_writeScalerPort(addr, val)

#define rtdf_readScalerPort(addr)		RtdCtrl_ReadScalerPort(addr)
#define rtd_readScalerPort(addr)		rtdf_readScalerPort(addr)

#define rtdf_setBitsScalerPort(addr,val)		RtdCtrl_setBitsScalerPort(addr, val)
#define rtd_setBitsScalerPort(addr,val)		rtdf_setBitsScalerPort(addr, val)

#define rtdf_clearBitsScalerPort(addr,val)	RtdCtrl_clearBitsScalerPort(addr, val)
#define rtd_clearBitsScalerPort(addr,val)	rtdf_clearBitsScalerPort(addr, val)

#define rtdf_maskScalerPort(addr, and, or)	RtdCtrl_maskScalerPort(addr, and, or)
#define rtd_maskScalerPort(addr, and, or)	rtdf_maskScalerPort(addr, and, or)

#define rtdf_writeScalerDataPort(addr, index, val)	RtdCtrl_WriteScalerDataPort(addr, index, val)
#define rtd_writeScalerDataPort(addr, index, val)	rtdf_writeScalerDataPort(addr, index, val)

#define rtdf_readScalerDataPort(addr, index)		RtdCtrl_ReadScalerDataPort(addr, index)
//#define rtd_readScalerDataPort(addr)		rtdf_readScalerDataPort(addr)

#define rtdf_writeAmountScalerPort(ucAddr, usLength, ucValue, bAutoInc)		rtdf_CScalerWriteScalerPort(ucAddr, usLength, ucValue, bAutoInc)
#define rtd_writeAmountScalerPort(ucAddr, usLength, ucValue, bAutoInc)		rtdf_writeAmountScalerPort(ucAddr, usLength, ucValue, bAutoInc)

#define rtdf_writeCodeWScalerPort(pArray)		RtdCtrl_WriteCodeWScalerPort(pArray)
#define rtd_writeCodeWScalerPort(pArray)		rtdf_writeCodeWScalerPort(pArray)

#define rtdf_CScalerWriteScalerPort(ucAddr, usLength, pArray, bAutoInc)		RtdCtrl_CScalerWriteScalerPort(ucAddr, usLength, pArray, bAutoInc)
#define rtd_CScalerWriteScalerPort(ucAddr, usLength, pArray, bAutoInc)		rtdf_CScalerWriteScalerPort(ucAddr, usLength, pArray, bAutoInc)
#define CScalerSendAddr( ucAddr, value,  bAutoInc)		 RtdCtrl_CScalerSendAddr( ucAddr, value,  bAutoInc)
// access by single bit
#define rtd_setGPIOPin(bit_offset)			drv_pcb_set_gpio_pin(bit_offset, 1)
#define rtd_clrGPIOPin(bit_offset)			drv_pcb_set_gpio_pin(bit_offset, 0)

// access by value
#define rtd_outGPIO(val)					drv_pcb_set_gpio_pin(bit_offset, 1)
#define rtd_outGPIO32_38(val)				drv_pcb_set_gpio_pin(bit_offset, 1)
#define rtd_inGPIO()						drv_pcb_set_gpio_pin(bit_offset, 0)
#define rtd_inGPIO32_38()

// For Interrupt
#define rtd_isr_inb(offset)			rtd_inb(offset)
#define rtd_isr_inl(offset)			RtdCtrl_isr_Read(offset)

#define rtd_isr_outb(offset,val)		rtd_outb(offset,val)
#define rtd_isr_outl(offset,val)		RtdCtrl_isr_Write(offset,val)

#define rtd_isr_maskl(offset, andMask, orMask) 	RtdCtrl_isr_Mask(offset, andMask, orMask)
#define rtd_isr_maskb(offset, andMask, orMask) RtdCtrl_isr_MaskByte(offset, andMask, orMask)

#define rtdf_isr_pageSelect(page)			RtdCtrl_isr_WriteScalerPort(Page_Select_0x009f, page)

#define rtdf_isr_writeScalerPort(addr, val)	RtdCtrl_isr_WriteScalerPort(addr, val)
#define rtd_isr_writeScalerPort(addr, val)	rtdf_isr_writeScalerPort(addr, val)

#define rtdf_isr_readScalerPort(addr)		RtdCtrl_isr_ReadScalerPort(addr)
#define rtd_isr_readScalerPort(addr)		rtdf_isr_readScalerPort(addr)

#define rtdf_isr_setBitsScalerPort(addr,val)		RtdCtrl_isr_setBitsScalerPort(addr, val)
#define rtd_isr_setBitsScalerPort(addr,val)		rtdf_isr_setBitsScalerPort(addr, val)

#define rtdf_isr_clearBitsScalerPort(addr,val)	RtdCtrl_isr_clearBitsScalerPort(addr, val)
#define rtd_isr_clearBitsScalerPort(addr,val)	rtdf_isr_clearBitsScalerPort(addr, val)

#ifdef CONFIG_ENABLE_HDMI
#define rtdf_isr_maskScalerPort(addr, and, or)	RtdCtrl_isr_maskScalerPort(addr, and, or)
#endif
#define rtd_isr_maskScalerPort(addr, and, or)	rtdf_isr_maskScalerPort(addr, and, or)


#endif	// #ifndef _SPARROW_H

