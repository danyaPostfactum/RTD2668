/*=============================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2005
  * All rights reserved.
  * ============================================ */

/*================- File Description ================= */
/**
 * @file
 * 	This file is irrc process function.
 *
 * @author	$Author$
 * @date	$Date$
 * @version 	$Revision$$
 * @ingroup 	isr
 */

/**
* @addtogroup irrc
* @{
*/

#include "irrc\irrc.h"
#include "isr_utility.h"
#include "reg_def.h"
#include "message.h"
#include "rosprintf.h"
#define _IRKEY_
#include "ui_ctrl.h"
#include "ap_rtirkey.h"
#include "pcb_customer.h"
#include "source.h"
#include "flow_struct.h"
#include "osdcore.h"
#include "power.h"

#undef	_IRKEY_
#define IRRC_MAX_BUF 	4
#define _IRRC_REAPET_COUNT	IRRC_MAX_BUF-1

#define CR_IRSR_RESET_EN    (1<<31)
#define CR_IREDNM_EN        (1<<23)
#define CR_RAW_FIFO_OV_EN   (1<<14)
#define CR_RAW_FIFO_VAL_EN  (1<<13)
#define CR_RAW_EN           (1<<12)
#define CR_MLAE_EN          (1<<11)
#define CR_IRIE_EN          (1<<10)
#define CR_IRRES_EN         (1<<9)
#define CR_IRUE_EN          (1<<8)
#define CR_IRCM_EN          (1<<7)
#define CR_IRBME_EN         (1<<6)
#define CR_IRDMP_EN         (1<<5)
#define SR_RMT_BIG          (1<<4)
#define SR_RAW_FIFO_OV      (1<<3)
#define SR_RAW_FIFO_VAL     (1<<2)
#define SR_IRRF             (1<<1)
#define SR_IRDVF            (1<<0)


#if CONFIG_TU300
code UINT8  ISDBIrKeyMsg[][2]=
{  
	//转发码值
	//{ISDB_POWER_CODE			    ,_POWER_KEY_MESSAGE},  
	{ISDB_MUTE_CODE			       ,_MUTE_KEY_MESSAGE},  
	{ISDB_SUB_CODE       			,_DVD_SUBTITLE_MESSAGE},  
	{ISDB_SEARCH_CODE      			,_DVD_GOTO_MESSAGE},  
	{ISDB_LIST_CODE      			,_ISDB_LIST_MESSAGE},  
	{ISDB_AUDIO_CODE     			,_DVD_AUDIO_MESSAGE},  
	{ISDB_EPG_CODE      			,_ISDB_EPG_MESSAGE},  
	{ISDB_UP_CODE			      	,_UP_KEY_MESSAGE},  
	{ISDB_DOWN_CODE   			  	,_DOWN_KEY_MESSAGE},  
	{ISDB_LEFT_CODE   				,_LEFT_IR_MESSAGE},  
	{ISDB_RIGHT_CODE     			,_RIGHT_IR_MESSAGE},  
	{ISDB_OK_CODE       			,_OK_IR_MESSAGE},  
	{ISDB_MENU_CODE       			,_DVD_SETUP_MESSAGE},  
	{ISDB_EXIT_CODE       			,_EXIT_KEY_MESSAGE},  
	{ISDB_KEY_0_CODE 				,IR_0_KEY_MESSAGE},  
	{ISDB_KEY_1_CODE       			,IR_1_KEY_MESSAGE},  
	{ISDB_KEY_2_CODE      			,IR_2_KEY_MESSAGE},  
	{ISDB_KEY_3_CODE		  		,IR_3_KEY_MESSAGE}, 
	{ISDB_KEY_4_CODE       			,IR_4_KEY_MESSAGE},  
	{ISDB_KEY_5_CODE       			,IR_5_KEY_MESSAGE},  
	{ISDB_KEY_6_CODE       			,IR_6_KEY_MESSAGE},  
	{ISDB_KEY_7_CODE     			,IR_7_KEY_MESSAGE},  
	{ISDB_KEY_8_CODE  				,IR_8_KEY_MESSAGE},  
	{ISDB_KEY_9_CODE    	  		,IR_9_KEY_MESSAGE},
	#if 0
	{ISDB_KEY_11_CODE				,_ISDB_KEY_11_MESSAGE},
	{ISDB_KEY_12_CODE				,_ISDB_KEY_12_MESSAGE},
	{ISDB_RECALL_CODE				,_ISDB_RECALL_MESSAGE},
	{ISDB_KEY_100_CODE			,_ISDB_KEY_100_MESSAGE},
	{ISDB_CH_INC_CODE				,_CHANNEL_INC_MESSAGE},
	{ISDB_CH_DEC_CODE				,_CHANNEL_DEC_MESSAGE},
	{ISDB_VOL_INC_CODE			,_VOLUME_INC_MESSAGE},
	{ISDB_VOL_DEC_CODE			,_VOLUME_DEC_MESSAGE}
	#endif
};
#endif
code UINT8  DVDIrKeyMsg[][2]=
{  
	//转发码值 
    {IR_89JDVD_DCT0       		,IR_0_KEY_MESSAGE},  
    {IR_89JDVD_DCT1       		,IR_1_KEY_MESSAGE},
    {IR_89JDVD_DCT2       		,IR_2_KEY_MESSAGE},  
    {IR_89JDVD_DCT3       		,IR_3_KEY_MESSAGE},
    {IR_89JDVD_DCT4       		,IR_4_KEY_MESSAGE},  
    {IR_89JDVD_DCT5       		,IR_5_KEY_MESSAGE},
    {IR_89JDVD_DCT6       		,IR_6_KEY_MESSAGE},  
    {IR_89JDVD_DCT7       		,IR_7_KEY_MESSAGE},
    {IR_89JDVD_DCT8       		,IR_8_KEY_MESSAGE},  
    {IR_89JDVD_DCT9       		,IR_9_KEY_MESSAGE},
    {IR_89JDVD_EJECT       		,_DVD_EJECT_MESSAGE},  
    {IR_89JDVD_DISPLAY       	,_DVD_DISPLAY_MESSAGE},
    {IR_89JDVD_GOTO       		,_DVD_GOTO_MESSAGE},  
    {IR_89JDVD_TITLE       		,_DVD_TITLE_MESSAGE},
    {IR_89JDVD_SETUP       		,_DVD_SETUP_MESSAGE},  
    {IR_89JDVD_RETURN       	,_EXIT_KEY_MESSAGE},
    {IR_89JDVD_AUDIO       		,_DVD_AUDIO_MESSAGE},  
    {IR_89JDVD_ANGLE       		,_DVD_ANGLE_MESSAGE},
    {IR_89JDVD_SUBTITLE       	,_DVD_SUBTITLE_MESSAGE},  
    {IR_89JDVD_PAL_NTSC       	,_DVD_PAL_NTSC_MESSAGE},
    {IR_89JDVD_REPEAT       	,_DVD_REPEAT_MESSAGE},  
    {IR_89JDVD_SLOW       		,_DVD_SLOW_MESSAGE},
    {IR_89JDVD_STEP       		,_DVD_STEP_MESSAGE},
    {IR_89JDVD_ZOOM       		,_DVD_ZOOM_MESSAGE},  
    {IR_89JDVD_PLAY_PAUSE       ,_DVD_PLAY_PAUSE_MESSAGE},
    {IR_89JDVD_STOP       		,_DVD_STOP_MESSAGE},  
    {IR_89JDVD_SET_A       		,_DVD_SET_A_MESSAGE},
    {IR_89JDVD_FREW       		,_DVD_FREW_MESSAGE},  
    {IR_89JDVD_FFWD       		,_DVD_FFWD_MESSAGE},
    {IR_89JDVD_PREV       		,_DVD_PREV_MESSAGE},  
    {IR_89JDVD_NEXT       		,_DVD_NEXT_MESSAGE},
    {IR_89JDVD_USB_SD       	,_DVD_USB_SD_MESSAGE},  
    {IR_89JDVD_ROOT_MENU        ,_DVD_ROOT_MENU_MESSAGE},
    {IR_89JDVD_LEFT       		,_LEFT_IR_MESSAGE},
    {IR_89JDVD_RIGHT       		,_RIGHT_IR_MESSAGE},
    {IR_89JDVD_UP       		,_UP_KEY_MESSAGE},
    {IR_89JDVD_DOWN       		,_DOWN_KEY_MESSAGE},
    {IR_89JDVD_ENTER       		,_OK_IR_MESSAGE},//_DVD_ENTER_MESSAGE},
    {IR_89JDVD_PROGRAM       	,_DVD_PROGRAM_MESSAGE},
    {IR_89JDVD_ADD10       		,_DVD_ADD10_MESSAGE},
    

};
typedef struct _IRRC_QUEUE
{
    UINT8 IRRC_Repeat_Count;
    UINT32 buf;
} IRRC_QUEUE;

static IRRC_QUEUE irrc_queue;
extern STRUCT_IR_PARAMETER code stIrrcSettings;
bit drv_irrc_Decoder(void);
UINT32 PCB_GetIrdaCustomeCode();

/*============================================================================*/
/**
 * drv_irrc_Isr
 * Interrupt process for irda
 *
 * @param <void>		{ void }
 * @return 				{ void }
 *
 */
bit drv_irrc_Isr(void)
{
    if (!drv_isr_GetPending(SYS_ISR_IRDA))
        return RT_ISR_UNHANDLED;

    if (!drv_irrc_Decoder())
    {
        //clean Pending
        rtd_isr_outl(ISR_0x2504, _BIT4);

        return RT_ISR_UNHANDLED;
    }

    //clean Pending
    rtd_isr_outl(ISR_0x2504, _BIT4);

    return RT_ISR_HANDLED;
}
void RtdCtrl_Write1(UINT16 addr, UINT32 val);
UINT32 RtdCtrl_Read1(UINT16 addr);

/*============================================================================*/
/**
 * drv_irrc_Init
 * Initial irda referenced from stIrrcSettings
 *
 * @param <void>			{ void }
 * @return 				{ void }
 *
 */
void drv_irrc_Init(void)
{
    UINT32 getVal;

    // Initial irrc from stIrrcSettings's parameter
    // Reset
    rtdf_setBits(IR_CR_0x2890, CR_IRSR_RESET_EN);

    //  Disable IR HW interrupt
    drv_isr_Enable(SYS_ISR_IRDA, _DISABLE);

    /* reset irda */
    rtd_outl(IR_CR_0x2890, rtd_inl(IR_CR_0x2890) | CR_IRSR_RESET_EN);
#if 1
    //  IR_PSR Reg:0x2880
    getVal = (stIrrcSettings.IRRBL & 0xff);
    getVal = (getVal << 8);
    getVal |= (stIrrcSettings.IRRML & 0xff);
    getVal = (getVal << 8);
    getVal |= (stIrrcSettings.IRRDZL & 0xff);
    getVal = (getVal << 8);
    getVal |= (stIrrcSettings.IRRDOL & 0xff);
    rtd_outl(IR_PSR_0x2880, getVal);
    RTD_Log(LOGGER_INFO, "drv_irrc_Init:2880 = %x\n", (UINT32)rtd_inl(IR_PSR_0x2880));

    // IR_PER Reg:0x2884
    getVal = (stIrrcSettings.IRRDL & 0x3f);
    getVal = (getVal << 8);
    getVal |= (stIrrcSettings.IRRRL & 0xff);
    getVal = (getVal << 8);
    getVal |= (stIrrcSettings.IRRSL & 0xff);
    rtdf_maskl(IR_PER_0x2884,0xffc00000, getVal);
//	RTD_Log(LOGGER_INFO, "drv_irrc_Init:2884 = %x\n", (UINT32)rtd_inl(IR_PER_0x2884));

    // IR_SF Reg:0x2888
    getVal = (stIrrcSettings.IRIT & 0xff);
    getVal = (getVal << 8);
    getVal |= (stIrrcSettings.IROCT & 0xff);
    getVal = (getVal << 16);
    getVal |= (stIrrcSettings.IRFD & 0xffff);
//	RTD_Log(LOGGER_INFO, "1.drv_irrc_Init:2888 = %x\n", (UINT32)getVal);
//	RTD_Log(LOGGER_INFO, "1.drv_irrc_Init:IRFD = %x\n", (UINT32)stIrrcSettings.IRFD);
//	RtdCtrl_Write1(IR_SF_0x2888, getVal);
//	RTD_Log(LOGGER_INFO, "drv_irrc_Init:28881 = %x\n", (UINT32)rtd_inl(IR_SF_0x2888));
//	RTD_Log(LOGGER_INFO, "drv_irrc_Init:2888.1 = %x\n", (UINT32)rtd_inl(IR_SF_0x2888));

    rtd_outl(IR_SF_0x2888, getVal);
    RTD_Log(LOGGER_INFO, "drv_irrc_Init:28882 = %x\n", (UINT32)rtd_inl(IR_SF_0x2888));

    // IR_CR Reg:0x2890
    getVal = (stIrrcSettings.IREDNM & 0x01);
    getVal = (getVal << 1);
    getVal |= (stIrrcSettings.IREDN & 0x3f);
    getVal = (getVal << 7);
    getVal |= (stIrrcSettings.MLAE & 0x01);
    getVal = (getVal << 4);

    getVal = (stIrrcSettings.IRRES & 0x01);
    getVal = (getVal << 2);
    getVal |= (stIrrcSettings.IRCM & 0x01);
    getVal = (getVal << 2);
    getVal |= (stIrrcSettings.IRBME & 0x01);
    getVal = (getVal << 1);

    getVal = (stIrrcSettings.IRDPM & 0x01);
    getVal = (getVal << 4);
    getVal |= (stIrrcSettings.IRDN & 0x0f);
    rtd_outl(IR_CR_0x2890, getVal);
//	RTD_Log(LOGGER_INFO, "drv_irrc_Init:2890 = %x\n", (UINT32)rtd_inl(IR_CR_0x2890));

    // IR_SR Reg:0x2898
    getVal = stIrrcSettings.IRRF & 0x01;
    getVal |= (getVal << 1);
    rtdf_setBits(IR_SR_0x2898, getVal) ;
//	RTD_Log(LOGGER_INFO, "drv_irrc_Init:2898 = %x\n", (UINT32)rtd_inl(IR_SR_0x2898));
#else	// from fan
    rtd_outl(0x2890, 0x80000000);							//Reset
    rtd_outl(0x2880, 0x4e13133b);							//set GPIOA5 Hi
    rtd_outl(0x2884, 0x11327);							//set 0~3 out clk
    rtd_outl(0x2888, 0xffff021b);							//set 4~5 out clk
#endif
    rtd_outl(0x2898, 0x02);							//set 0~3 duty
    rtd_outl(0x2890, 0x5df);							//set 4~5 duty

    // IRDA interrupt route select to IRQ1
    drv_isr_SelectRoute(ROUTE_SEL_IRDA, IRQ_ROUTE_IRQ1);

    // SYS_GIE_3 enable irda interrupt
    drv_isr_Enable(SYS_ISR_IRDA, _ENABLE);

    //clean Pending
    rtd_outl(UMSK_ISR_0x2500, _BIT4);

    // Irrc_Enable
    rtdf_setBits(IR_CR_0x2890, CR_IRUE_EN);

    // IR HW interrupt enable
    rtdf_setBits(IR_CR_0x2890 /*IR_CR*/, _BIT10);
}

#if CONFIG_USB_CHECK
extern UINT8 xdata stUsbFlag;
extern UINT16 g_cnt_1;

#endif

void IRRC_isr_Command_Mapping(UINT32 irrc_command, UINT32 IRstate)
{
    UINT32 Custom_Code;
    UINT8 temp = 0,tempmessage = 0,cnt;
    UINT8 ir_command = 0;
	cnt=cnt;
    Custom_Code = PCB_GetIrdaCustomeCode();

//	UINT8 getVal, getVal1;
//	getVal = ((irrc_command & 0x00ff0000) >> 16);
//	getVal1 = ((irrc_command & 0xff000000) >> 24);
//	getVal = *((UINT8*)&irrc_command  + 1); // ((irrc_command & 0x00ff0000) >> 16);
//	getVal1 =*(UINT8*)&irrc_command;
 //RTD_Log(LOGGER_INFO, "Accept error data in irrc = %x\n", (UINT32)irrc_command);

    if (*((UINT8*)&irrc_command  + 1) != ~*(UINT8*)&irrc_command)
    {
        // Accept abnormal command
        //RTD_Log(LOGGER_INFO, "Accept error data in irrc = %x\n", (UINT32)irrc_command);
        return;
    }

    //RTD_Log(LOGGER_INFO, "Accept correct data in irrc = %x\n", (UINT32)irrc_command);

    if (((UINT32)irrc_command & 0x0000ffff)!= Custom_Code)
        return;

    if (*(UINT8*)&irrc_command != 0x00)
    {
        //fw_isr_IR_message_push(*((UINT8*)&irrc_command  + 1));
        ir_command = *((UINT8*)&irrc_command  + 1);
        for (temp = 0; temp <= ((sizeof(IRKeyMsg))/2); temp ++)
        {
        	if (IRKeyMsg[temp][0] == ir_command)
        	{
        	//RTD_Log(LOGGER_INFO,"IR Code %x ",(UINT32)ir_command);
			//RTD_Log(LOGGER_INFO,"GET_INPUTSOURCE_INDEX() %x ",(UINT32)GET_INPUTSOURCE_INDEX());
			 #if CONFIG_USB_CHECK
			  if(IRKeyMsg[temp][0]==0xaa)
			  	{
			  		stUsbFlag=1;
			  		break;
			  	}
			  else if(IRKeyMsg[temp][0]==0xbb)
			  	{
			  		stUsbFlag=2;
					g_cnt_1=500;
					break;
			  	}
			 #endif
        		if(!(IRstate & SR_IRRF) || ((IRstate & SR_IRRF) && ui_key_allow_repeat(IRKeyMsg[temp][1])))
        		{
        			#if 1
					//RTD_Log(LOGGER_INFO,"IR GET_POWERSTATUS() =%x ",(UINT32)GET_POWERSTATUS());
					//RTD_Log(LOGGER_INFO,"ucOsdState =%x ",(UINT32)ucOsdState);
					#if 0//CONFIG_TU300
					if(IRKeyMsg[temp][0]==0x14)
					Send_IR_isr((((~TV_POWER_CODE)<<8)|TV_POWER_CODE));
					#endif
					#if CONFIG_ENABLE_USB
	        		if(((GET_INPUTSOURCE_INDEX()== _SOURCE_INDEX_YPBPR)||(GET_INPUTSOURCE_INDEX()== _SOURCE_INDEX_USB)) && (GET_POWERSTATUS() == POWER_STATUS_POWER_ON)&&(ucOsdState==_IDLE_STATE || ucOsdState==_SHOW_INFO_STATE || ucOsdState==_VOLUME_STATE || ucOsdState==_HOTKEY_STATE))
					{
						tempmessage = IRKeyMsg[temp][1];
						for(cnt=0;cnt<sizeof(DVDIrKeyMsg)/2;cnt++)
						{
							if(DVDIrKeyMsg[cnt][1] == tempmessage)
							{
								//RTD_Log(LOGGER_INFO,"DVD IR Code %x ",(UINT32)DVDIrKeyMsg[cnt][0]);
								ir_command = DVDIrKeyMsg[cnt][0];
								Send_IR_isr(((~ir_command)<<8)|ir_command);
								break;
							}	
						}
						if((cnt == sizeof(DVDIrKeyMsg)/2)/*||(tempmessage==_LEFT_KEY_MESSAGE)||(tempmessage==_RIGHT_KEY_MESSAGE)*/)
						{
							fw_isr_IR_message_push(IRKeyMsg[temp][1]);
						}
					}
					else 
					#endif
					#if CONFIG_TU300
					if(((GET_INPUTSOURCE_INDEX()== _SOURCE_INDEX_AV_1)) && (GET_POWERSTATUS() == POWER_STATUS_POWER_ON)&&(ucOsdState==_IDLE_STATE || ucOsdState==_SHOW_INFO_STATE|| ucOsdState==_VOLUME_STATE || ucOsdState==_HOTKEY_STATE))
					{
						tempmessage = IRKeyMsg[temp][1];
						//RTD_Log(LOGGER_INFO,"DTV IR tempmessage %x ",(UINT32)tempmessage);
						for(cnt=0;cnt<sizeof(ISDBIrKeyMsg)/2;cnt++)
						{
							if(ISDBIrKeyMsg[cnt][1] == tempmessage)
							{
								//RTD_Log(LOGGER_INFO,"DTV IR Code %x ",(UINT32)ISDBIrKeyMsg[cnt][0]);
								ir_command = ISDBIrKeyMsg[cnt][0];
								Send_IR_isr(((~ir_command)<<8)|ir_command);
								break;
							}	
						}
						if((cnt == sizeof(ISDBIrKeyMsg)/2)/*||(tempmessage==_LEFT_KEY_MESSAGE)||(tempmessage==_RIGHT_KEY_MESSAGE)*/)
						{
							fw_isr_IR_message_push(IRKeyMsg[temp][1]);
						}
					}
					#endif
					#endif
	        		{
	        			fw_isr_IR_message_push(IRKeyMsg[temp][1]);
	        		}
        		}
        		break;
        		
        	}
        }
        //fw_isr_ui_message_push(getVal);
    }

}

/*********************************************************************
** 函数名称: void IRRC_Command_Mapping(UINT32 irrc_command, UINT8 *key_message)
** 功能描述: IR码解析
** 输　  入: 完整IR码,返回key_message
** 输　  出: 无 
**********************************************************************/
void IRRC_Command_Mapping(UINT32 irrc_command, UINT8 *key_message)
{
    UINT32 Custom_Code;
    UINT8 temp = 0;
    UINT8 ir_command = 0;
    Custom_Code = PCB_GetIrdaCustomeCode();

//	UINT8 getVal, getVal1;
//	getVal = ((irrc_command & 0x00ff0000) >> 16);
//	getVal1 = ((irrc_command & 0xff000000) >> 24);
//	getVal = *((UINT8*)&irrc_command  + 1); // ((irrc_command & 0x00ff0000) >> 16);
//	getVal1 =*(UINT8*)&irrc_command;
// RTD_Log(LOGGER_INFO, "Accept error data in irrc = %x\n", (UINT32)irrc_command);

    if (*((UINT8*)&irrc_command  + 1) != ~*(UINT8*)&irrc_command)
    {
        // Accept abnormal command
        //RTD_Log(LOGGER_INFO, "Accept error data in irrc = %x\n", (UINT32)irrc_command);
        return;
    }

    //RTD_Log(LOGGER_INFO, "Accept correct data in irrc = %x\n", (UINT32)irrc_command);

    if (((UINT32)irrc_command & 0x0000ffff)!= Custom_Code)
        return;

    if (*(UINT8*)&irrc_command != 0x00)
    {
        //fw_isr_IR_message_push(*((UINT8*)&irrc_command  + 1));
        ir_command = *((UINT8*)&irrc_command  + 1);
        for (temp = 0; temp <= ((sizeof(IRKeyMsg))/2); temp ++)
        {
            if (IRKeyMsg[temp][0] == ir_command)
                (*key_message) = IRKeyMsg[temp][1];//fw_IR_message_push(IRKeyMsg[temp][1]);
        }
        //fw_isr_ui_message_push(getVal);
    }
}

bit drv_irrc_Decoder(void)
{
    volatile UINT32 status;
    volatile UINT32 acceptdata;

    status = rtd_isr_inl(IR_SR_0x2898);
    if ((status&(SR_IRDVF|SR_IRRF)) == 0)
        return RT_ISR_UNHANDLED;

    /* Write clear out the pending status*/
    // Clear repeat flag
    if (status & SR_IRRF)
        rtd_isr_outl(IR_SR_0x2898, SR_IRRF | (stIrrcSettings.IRRF << 1));

    // Clear valid data flag
    if (status & SR_IRDVF)
        rtd_isr_outl(IR_SR_0x2898, SR_IRDVF | (stIrrcSettings.IRRF << 1));

    // Get valid data
    acceptdata = rtd_isr_inl(IR_RP_0x2894);
#if 0
    IRRC_isr_Command_Mapping(acceptdata);
#else
    if ((status & SR_IRRF) && (status & SR_IRDVF))
    {
        if (irrc_queue.buf == acceptdata)
        {
            irrc_queue.IRRC_Repeat_Count++;
            if (irrc_queue.IRRC_Repeat_Count >= _IRRC_REAPET_COUNT)
            {
                irrc_queue.IRRC_Repeat_Count--;
                IRRC_isr_Command_Mapping(acceptdata, status);
            }
        }
        else
        {
            irrc_queue.IRRC_Repeat_Count = 0;
            irrc_queue.buf = acceptdata;
            IRRC_isr_Command_Mapping(acceptdata, status);
        }
    }
    else
    {
        irrc_queue.IRRC_Repeat_Count = 0;
        irrc_queue.buf = acceptdata;
        IRRC_isr_Command_Mapping(acceptdata, status);
    }
#endif
    return RT_ISR_HANDLED;
}

#if(IR_DETECTMODE == _IR_DS_SW)
/*********************************************************************
** 函数名称: void fw_IR_detectmode_change(bit type)
** 功能描述: 改变IR侦测模式
** 输　  入: bit type: HW or SW
** 输　  出: 无 
**********************************************************************/

void fw_IR_detectmode_change(bit type)
{
	if(type)
		RtdCtrl_MaskByte(PC_PS2_0x2d3a,~(_BIT0|_BIT1|_BIT2),0x01);//将IR变为SW侦测
	else
		RtdCtrl_MaskByte(PC_PS2_0x2d3a,~(_BIT0|_BIT1|_BIT2),0x00);//将IR变为HW侦测
}
#endif//#if(IR_DETECTMODE == _IR_DS_SW)
