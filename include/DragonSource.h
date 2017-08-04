#ifndef  _DRAGONSOURCE_H_
#define  _DRAGONSOURCE_H_

#define ID692_EN		_OFF

#define _IR_RTD_HW		0
#define _IR_DS_SW		1		//只是适合NEC编码
#define IR_DETECTMODE	_IR_RTD_HW
#define   VFREQ_MAX        860
#define   VFREQ_MIN       550

#define _OVERSACN_100                 0
#define _OVERSCAN_93             	7	//93.75%    //PAL
#define _OVERSCAN_95             	5	//95.00%   //NTSC
#define _OVERSCAN_97            	3	//97.00%   //HD
#define _HDMI_OVERSCAN_PERCENT   _OVERSCAN_95
#define _YPBPR_OVERSCAN_PERCENT  _OVERSCAN_95

//龙源代码寄存器专用变量，请不要作其他用途。
extern UINT16 data reg_w_addr;						
extern UINT32 data reg_and;
extern UINT32 data reg_or;
extern UINT16 data isr_reg_w_addr;
extern UINT32 data isr_reg_and;
extern UINT32 data isr_reg_or;

extern UINT32 IR_Raw_data;
extern bit bIR_firstcode;
extern bit bIR_Receive;

void RtdCtrl_WriteDS(UINT32 val);
UINT32 RtdCtrl_ReadDS(UINT16 addr);
void RtdCtrl_MaskByteDS(UINT16 addr, UINT8 and, UINT8 or);
void RtdCtrl_MaskDS(UINT16 addr);
void RtdCtrl_isr_WriteDS(UINT32 val);
UINT32 RtdCtrl_isr_ReadDS(UINT16 addr);
void RtdCtrl_isr_MaskByteDS(UINT16 addr, UINT8 and, UINT8 or);
void RtdCtrl_isr_MaskDS(UINT16 addr);

#define ds_maskl(offset, andMask, orMask) 				{reg_and = andMask; reg_or = orMask; RtdCtrl_MaskDS(offset);}
#define ds_outl(offset,val)								{reg_w_addr = offset; RtdCtrl_WriteDS(val);}
#define ds_isr_outl(offset,val)							{isr_reg_w_addr=offset; RtdCtrl_isr_WriteDS(val);}
#define ds_isr_maskl(offset, andMask, orMask) 			{isr_reg_and = andMask; isr_reg_or = orMask; RtdCtrl_isr_MaskDS(offset);}
#define ds_setBits(offset, Mask)						ds_maskl(offset, 0xffffffff, Mask)						
#define ds_clearBits(offset, Mask) 						ds_maskl(offset, ~((UINT32)Mask), 0) 

extern void delay_1ms(UINT16 ms);
extern bit bReadID(void);
extern void DS_Init(void);
extern void drv_isr_IR_decode(void);
#endif	// #ifndef  _DRAGONSOURCE_H_
