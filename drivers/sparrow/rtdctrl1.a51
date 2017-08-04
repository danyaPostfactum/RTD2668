;/*==========================================================================
;  * Copyright (c)      Realtek Semiconductor Corporation, 2006
;  * All rights reserved.
;  * ========================================================================*/

;/*================= File Description =======================================*/
;/**
; * @file
; * 	This file is for rtdaccess related functions.
; *
; * @author 	$Author$
; * @date	$Date$
; * @version	$Revision$
; * @ingroup	rtdaccess
; */

;/**
; * @addtogroup rtdaccess
; * @{
; */
;#include "char\rtdctrl.h"
;#include "char\uart.h"
;#include <REG52.H>
$NOMOD51

NAME	RTDCTRL1
?PR?_RtdCtrl_Read?RTDCTRL1		SEGMENT CODE
?PR?_RtdCtrl_isr_Read?RTDCTRL1	SEGMENT CODE
?PR?_RtdCtrl_Write?RTDCTRL1		SEGMENT CODE
?XD?_RtdCtrl_Write?RTDCTRL1		SEGMENT XDATA
?PR?_RtdCtrl_isr_Write?RTDCTRL1	SEGMENT CODE
?XD?_RtdCtrl_isr_Write?RTDCTRL1	SEGMENT XDATA
?PR?_RtdCtrl_MaskByte?RTDCTRL1	SEGMENT CODE
?PR?_RtdCtrl_isr_MaskByte?RTDCTRL1	SEGMENT CODE
?PR?_RtdCtrl_Mask?RTDCTRL1              SEGMENT CODE
?XD?_RtdCtrl_Mask?RTDCTRL1              SEGMENT XDATA

	PUBLIC	_RtdCtrl_Write
	PUBLIC	?_RtdCtrl_Write?BYTE
	PUBLIC	_RtdCtrl_Read
	PUBLIC	_RtdCtrl_MaskByte
	PUBLIC	_RtdCtrl_Mask
	PUBLIC	?_RtdCtrl_Mask?BYTE

	PUBLIC	_RtdCtrl_isr_Write
	PUBLIC	?_RtdCtrl_isr_Write?BYTE
	PUBLIC	_RtdCtrl_isr_Read
	PUBLIC	_RtdCtrl_isr_MaskByte

;/*============================================================================*/
;/**
; * RtdCtrl_Read
; * Write 4-byte data from address
; *
; * @param <addr>		{ the start address to write the data }
; * @return 				{ Value in address }
; *
; */
;UINT32 RtdCtrl_Read(UINT16 addr);
	RSEG  ?PR?_RtdCtrl_Read?RTDCTRL1
	USING	0
_RtdCtrl_Read:
; 	Read data from addr
	MOV  	DPL,R7
	MOV  	DPH,R6
	MOVX 	A,@DPTR
	MOV     R7,A

; 	Read data from addr + 1
	INC  	DPTR
	MOVX 	A,@DPTR
	MOV     R6,A

; 	Read data from addr + 2
	INC  	DPTR
	MOVX 	A,@DPTR
	MOV     R5,A

; 	Read data from addr + 3
	INC  	DPTR
	MOVX 	A,@DPTR
	MOV     R4,A

;	Prepare return value
	RET

;/*============================================================================*/
;/**
; * RtdCtrl_Write
; * Write 4-byte data to address
; *
; * @param <addr>		{ the start address to write the data }
; * @param <val>			{ The data to be writeten is put in pData }
; * @return 				{ void }
; *
; */
;void RtdCtrl_Write(UINT16 addr, UINT32 val);
	RSEG  ?XD?_RtdCtrl_Write?RTDCTRL1
?_RtdCtrl_Write?BYTE:
	addr1:   DS   2
	val1:    DS   4

	RSEG  ?PR?_RtdCtrl_Write?RTDCTRL1
	USING	0
_RtdCtrl_Write:
; 	Write data to addr
	MOV  	DPTR,#val1+3
	MOVX 	A,@DPTR
	MOV  	DPL,R7
	MOV  	DPH,R6
	MOVX 	@DPTR,A

; 	Write data to addr + 1
	INC     R7
	MOV  	DPTR,#val1+2
	MOVX 	A,@DPTR
	MOV  	DPL,R7
	MOV  	DPH,R6
	MOVX 	@DPTR,A

; 	Write data to addr + 2
	INC     R7
	MOV  	DPTR,#val1+1
	MOVX 	A,@DPTR
	MOV  	DPL,R7
	MOV  	DPH,R6
	MOVX 	@DPTR,A

; 	Write data to addr + 3
	INC     R7
	MOV  	DPTR,#val1
	MOVX 	A,@DPTR
	MOV  	DPL,R7
	MOV  	DPH,R6
	MOVX 	@DPTR,A
	RET

;/*============================================================================*/
;/**
; * RtdCtrl_isr_Read
; * Write 4-byte data from address
; *
; * @param <addr>		{ the start address to write the data }
; * @return 				{ Value in address }
; *
; */
;UINT32 RtdCtrl_isr_Read(UINT16 addr);
	RSEG  ?PR?_RtdCtrl_isr_Read?RTDCTRL1
	USING	0
_RtdCtrl_isr_Read:
; 	Read data from addr
	MOV  	DPL,R7
	MOV  	DPH,R6
	MOVX 	A,@DPTR
	MOV     R0,A

; 	Read data from addr + 1
	INC  	DPTR
	MOVX 	A,@DPTR
	MOV     R1,A

; 	Read data from addr + 2
	INC  	DPTR
	MOVX 	A,@DPTR
	MOV     R5,A

; 	Read data from addr + 3
	INC  	DPTR
	MOVX 	A,@DPTR
	MOV     R4,A

; 	Prepare return data
	MOV     A, R0
	MOV     R7, A
	MOV     A, R1
	MOV     R6, A
	RET

;/*============================================================================*/
;/**
; * RtdCtrl_isr_Write
; * Write 4-byte data to address
; *
; * @param <addr>		{ the start address to write the data }
; * @param <val>			{ The data to be writeten is put in pData }
; * @return 				{ void }
; *
; */
;void RtdCtrl_isr_Write(UINT16 addr, UINT32 val);
	RSEG  ?XD?_RtdCtrl_isr_Write?RTDCTRL1
?_RtdCtrl_isr_Write?BYTE:
	addr2:  DS   2
	val2:   DS   4

	RSEG  ?PR?_RtdCtrl_isr_Write?RTDCTRL1
	USING	0
_RtdCtrl_isr_Write:
; 	Write data to addr
	MOV  	DPTR,#val2+3
	MOVX 	A,@DPTR
	MOV  	DPL,R7
	MOV  	DPH,R6
	MOVX 	@DPTR,A

; 	Write data to addr + 1
	INC     R7
	MOV  	DPTR,#val2+2
	MOVX 	A,@DPTR
	MOV  	DPL,R7
	MOV  	DPH,R6
	MOVX 	@DPTR,A

; 	Write data to addr + 2
	INC     R7
	MOV  	DPTR,#val2+1
	MOVX 	A,@DPTR
	MOV  	DPL,R7
	MOV  	DPH,R6
	MOVX 	@DPTR,A

; 	Write data to addr + 3
	INC     R7
	MOV  	DPTR,#val2
	MOVX 	A,@DPTR
	MOV  	DPL,R7
	MOV  	DPH,R6
	MOVX 	@DPTR,A
	RET

;/*============================================================================*/
;/**
; * RtdCtrl_MaskByte
; * Do AND and OR operation in byte data
; *
; * @param <addr>		{ Address to do and & or }
; * @param <and>			{ and in the value of address }
; * @param <or>			{ or in the value of address }
; * @return 				{ void }
; *
; */
;void RtdCtrl_MaskByte(UINT16 addr, UINT8 and, UINT8 or);
	RSEG  ?PR?_RtdCtrl_MaskByte?RTDCTRL1
	USING	0
_RtdCtrl_MaskByte:
	MOV  	DPL,R7
	MOV  	DPH,R6
	MOVX 	A,@DPTR

	ANL     A,R5
	ORL     A,R3
	MOVX 	@DPTR,A
	RET

;/*============================================================================*/
;/**
; * RtdCtrl_isr_MaskByte
; * Do AND and OR operation in byte data
; *
; * @param <addr>		{ Address to do and & or }
; * @param <and>			{ and in the value of address }
; * @param <or>			{ or in the value of address }
; * @return 				{ void }
; *
; */
;void RtdCtrl_isr_MaskByte(UINT16 addr, UINT8 and, UINT8 or);
	RSEG  ?PR?_RtdCtrl_isr_MaskByte?RTDCTRL1
	USING	0
_RtdCtrl_isr_MaskByte:
	MOV  	DPL,R7
	MOV  	DPH,R6
	MOVX 	A,@DPTR

	ANL     A,R5
	ORL     A,R3
	MOVX 	@DPTR,A
	RET

;/*============================================================================*/
;/**
; * RtdCtrl_Mask
; * Do AND and OR operation in 4-byte data
; *
; * @param <addr>		{ Address to do and & or }
; * @param <and>			{ and in the value of address }
; * @param <or>			{ or in the value of address }
; * @return 				{ void }
; *
; */
;void RtdCtrl_Mask(UINT16 addr, UINT32 and, UINT32 or)
	RSEG  ?XD?_RtdCtrl_Mask?RTDCTRL1
?_RtdCtrl_Mask?BYTE:
	addr3:   DS   2
	and3:   DS   4
	or3:   DS   4

	RSEG  ?PR?_RtdCtrl_Mask?RTDCTRL1
	USING	0
_RtdCtrl_Mask:
	; Adddress
	MOV		DPTR, #addr3
	MOV		A, R6
	MOVX	@DPTR,A

	INC		DPTR
	MOV		A, R7
	MOVX	@DPTR,A

	MOV		DPL,R7
	MOV		DPH,R6
	MOVX 	A,@DPTR
	MOV		R2,A

	INC		DPTR
	MOVX 	A,@DPTR
	MOV		R3,A

	INC		DPTR
	MOVX 	A,@DPTR
	MOV		R4,A

	INC		DPTR
	MOVX 	A,@DPTR
	MOV		R5,A

	MOV		DPTR, #addr3
	MOVX	A, @DPTR
	MOV		R6,A
	INC		DPTR
	MOVX	A, @DPTR
	MOV		R7,A

	MOV		DPTR, #and3 + 3
	MOVX 	A,@DPTR
	ANL		A, R2
	MOV		R2, A

	MOV		DPTR, #or3 + 3
	MOVX 	A,@DPTR
	ORL		A, R2
	MOV		DPL,R7
	MOV		DPH,R6
	MOVX 	@DPTR, A
	INC		R7

	; Adddress + 1
	MOV		DPTR, #and3 + 2
	MOVX 	A,@DPTR
	ANL		A, R3
	MOV		R3, A

	MOV		DPTR, #or3 + 2
	MOVX 	A,@DPTR
	ORL		A, R3
	MOV		DPL,R7
	MOV		DPH,R6
	MOVX 	@DPTR, A
	INC		R7

	; Adddress + 2
	MOV		DPTR, #and3 + 1
	MOVX 	A,@DPTR
	ANL		A, R4
	MOV		R4, A

	MOV		DPTR, #or3 + 1
	MOVX 	A,@DPTR
	ORL		A, R4
	MOV		DPL,R7
	MOV		DPH,R6
	MOVX 	@DPTR, A
	INC		R7

	; Adddress + 3
	MOV		DPTR, #and3
	MOVX 	A,@DPTR
	ANL		A, R5
	MOV		R5, A

	MOV		DPTR, #or3
	MOVX 	A,@DPTR
	ORL		A, R5
	MOV		DPL,R7
	MOV		DPH,R6
	MOVX 	@DPTR, A
	RET

	END
