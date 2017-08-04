/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2006
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is for scaler related functions.
 *
 * @author 	$Author$
 * @date		$Date$
 * @version	$Revision$
 * @ingroup	scaler
 */

/**
 * @addtogroup scaler
 * @{
 */

/*============================ Module dependency  ===========================*/
#include "ui_ctrl.h"
#include "osdcore.h"
#include "char\lsadc.h"
#include "rosprintf.h"
#include "ui_ctrl.h"
#include "ap_rtirkey.h"
#include "pcb_customer.h"
#include "pcb\pcb_utility.h"
#include "message.h"
#include "osdcontrol.h"
#include "rt_storage.h"
#include "ui_struct.h"
#include "storage_system.h"
#include "flow_struct.h"
#include "source.h"
#include "timer_event.h"
#include "timer\timer.h"
#include "intrins.h"
#include "reg52.h"
#include "power.h"
#include "i2c\i2c.h"


UINT8 key_state_from_isr;
//***************************************************
// Key Pad
// lsadc 0
#define _SOURCE_KEY_THRESHOLD 0x41
#define _RIGHT_KEY_THRESHOLD          0x82
#define _COMPOSE1_KEY_THRESHOLD 0xe6// Right + Left
// ls adc 1
#define _MENU_KEY_THRESHOLD 0xc1
#define _DOWN_KEY_THRESHOLD 0x83
#define _UP_KEY_THRESHOLD          0x43
#define _LEFT_KEY_THRESHOLD           0xc
#define _COMPOSE2_KEY_THRESHOLD 0xeb// Menu + Down
#define _AUDIO_IN_THRESHOLD 0xe0
#define _AUDIO_HEAD_IN_THRESHOLD 0xFD
#define _AUDIO_HEAD_OUT_THRESHOLD 0x30
#define _KEY_THRESHOLD 0x3
#define _KEYPRESSED_THRESHOLD 0x3
//***************************************************

UINT8 code KeyCodeADC0Msg[][2] =
{
	{0x47,_DIMMER_KEY_MESSAGE/*_SOURCE_KEY_MESSAGE*/},
	{0x7a,_SOURCE_KEY_MESSAGE/*_OK_KEY_MESSAGE*/},
	{0x99,_MENU_KEY_MESSAGE/*_RIGHT_KEY_MESSAGE*/},
	{0xB0,_LEFT_KEY_MESSAGE/*_LEFT_KEY_MESSAGE*/},
	//{0xC5,_RIGHT_KEY_MESSAGE/*_LEFT_KEY_MESSAGE*/},
	{0xC5 ,_RIGHT_KEY_MESSAGE/*_LEFT_KEY_MESSAGE*/},
	{0xD0 ,_DISPLAY_RATIO_KEY_MESSAGE/*_LEFT_KEY_MESSAGE*/},
	{0xDD ,_MUTE_KEY_MESSAGE/*_LEFT_KEY_MESSAGE*/},
	//{0xDC,_MUTE_KEY_MESSAGE/*_LEFT_KEY_MESSAGE*/}
};
#if 1
 #if CONFIG_ENABLE_USB
UINT8 code KeyCodeADC1Msg[][2] =
{
	{0xa,_POWER_KEY_MESSAGE/*_SOURCE_KEY_MESSAGE*/},
	//{0x3F,_DVD_FFWD_MESSAGE/*_OK_KEY_MESSAGE*/},
	//{0x67,_DVD_FREW_MESSAGE/*_RIGHT_KEY_MESSAGE*/},
	//{0x7F,_DVD_PLAY_PAUSE_MESSAGE/*_LEFT_KEY_MESSAGE*/},
	//{0xC5,_RIGHT_KEY_MESSAGE/*_LEFT_KEY_MESSAGE*/},
	//{0x99 ,_DVD_STOP_MESSAGE/*_LEFT_KEY_MESSAGE*/},
	//{0xDC,_MUTE_KEY_MESSAGE/*_LEFT_KEY_MESSAGE*/}
};

UINT8 code Keypad2DVD[][2] = 
{
	{IR_89JDVD_RETURN			   	,_EXIT_KEY_MESSAGE},  
	{IR_89JDVD_EJECT   			   	,_DVD_EJECT_MESSAGE},  
	{IR_89JDVD_NEXT					,_DVD_NEXT_MESSAGE},  
	{IR_89JDVD_PREV					,_DVD_PREV_MESSAGE},    
	{IR_89JDVD_PLAY_PAUSE       	,_DVD_PLAY_PAUSE_MESSAGE},  
	{IR_89JDVD_STOP       			,_DVD_STOP_MESSAGE},
};
#endif

#if CONFIG_TU300
UINT8 code Keypad2ISDB[][2] = 
{
	{ISDB_UP_CODE			      ,_LEFT_KEY_MESSAGE},  
	{ISDB_DOWN_CODE   			  ,_RIGHT_KEY_MESSAGE},    
	//{ISDB_EXIT_CODE       		  ,_EXIT_KEY_MESSAGE},  
	//{ISDB_OK_CODE       		  ,_DISPLAY_RATIO_KEY_MESSAGE},
	{ISDB_MUTE_CODE       		  ,_MUTE_KEY_MESSAGE}
	
};
UINT8 code Keylongpad2ISDB[][2] = 
{
	{ISDB_LIST_CODE			      ,_LEFT_KEY_MESSAGE},  
	{ISDB_SEARCH_CODE   		  ,_RIGHT_KEY_MESSAGE},      
	{ISDB_EPG_CODE       		  ,_DISPLAY_RATIO_KEY_MESSAGE},
};

#endif
#endif
UINT8 code KeyRepeat[] =
{
	_LEFT_KEY_MESSAGE,
	_RIGHT_KEY_MESSAGE,
	_LEFT_IR_MESSAGE,
	_RIGHT_IR_MESSAGE,
	_VOLUME_INC_MESSAGE,
	_VOLUME_DEC_MESSAGE
};

#define BIG_ENDIAN_ENABLE				0  		//big endian
#define LITTLE_ENDIAN_ENABLE			1

#define ENDIAN_STYLE					LITTLE_ENDIAN_ENABLE

bit ui_key_allow_repeat(UINT8 key)
{
	UINT8 cnt;
	for(cnt=0;cnt<sizeof(KeyRepeat);cnt++)
	{
		if(key == KeyRepeat[cnt])
		{
			if(Repeatkey_en(key))
				return _TRUE;
		}
	}
	return _FALSE;
}
#if 1
bit ui_key_is_ISDN(UINT8 key)
{
	
	UINT8 cnt;
	UINT8 ir_command;
#if CONFIG_ENABLE_USB

	if((GET_INPUTSOURCE_INDEX()== _SOURCE_INDEX_YPBPR||GET_INPUTSOURCE_INDEX()== _SOURCE_INDEX_USB)&& (ucOsdState==_IDLE_STATE || ucOsdState==_SHOW_INFO_STATE || ucOsdState==_VOLUME_STATE || ucOsdState==_HOTKEY_STATE))
	{
		for(cnt=0;cnt<sizeof(Keypad2DVD)/2;cnt++)
		{
			if(Keypad2DVD[cnt][1] == key)
			{
				//RTD_Log(LOGGER_INFO,"ISDB IR Code %x ",(UINT32)ir_command);
				ir_command = Keypad2DVD[cnt][0];
				Send_IR_isr(((~ir_command)<<8)|ir_command);
				return _TRUE;
			}	
		}		
	}
	#endif
	#if CONFIG_TU300
	if((GET_INPUTSOURCE_INDEX()== _SOURCE_INDEX_AV_1)&& (ucOsdState==_IDLE_STATE || ucOsdState==_SHOW_INFO_STATE || ucOsdState==_VOLUME_STATE || ucOsdState==_HOTKEY_STATE))
	{
		for(cnt=0;cnt<sizeof(Keypad2ISDB)/2;cnt++)
		{
			if(Keypad2ISDB[cnt][1] == key)
			{
				//RTD_Log(LOGGER_INFO,"ISDB IR Code %x ",(UINT32)ir_command);
				ir_command = Keypad2ISDB[cnt][0];
				Send_IR_isr(((~ir_command)<<8)|ir_command);
				return _TRUE;
			}	
		}		
	}
	#endif
	return _FALSE;
}
#endif
#if CONFIG_TU300
bit ui_LONG_key_is_ISDN(UINT8 key)
{
	UINT8 cnt;
	UINT8 ir_command;
	if((GET_INPUTSOURCE_INDEX()== _SOURCE_INDEX_AV_1)&& (ucOsdState==_IDLE_STATE || ucOsdState==_SHOW_INFO_STATE || ucOsdState==_VOLUME_STATE || ucOsdState==_HOTKEY_STATE))
	{
		for(cnt=0;cnt<sizeof(Keylongpad2ISDB)/2;cnt++)
		{
			if(Keylongpad2ISDB[cnt][1] == key)
			{
				//RTD_Log(LOGGER_INFO,"ISDB IR Code %x ",(UINT32)ir_command);
				ir_command = Keylongpad2ISDB[cnt][0];
				Send_IR_isr(((~ir_command)<<8)|ir_command);
				return _TRUE;
			}	
		}		
	}
	return _FALSE;


}

#endif

UINT8 ui_ReadKeyPad(UINT8 IRcodeCheck)
{
#if 1
	IRcodeCheck = IRcodeCheck;
	return _NONE_KEY_MESSAGE;
#elif 0
	static UINT8 pre_keystate = 0; 
	UINT8 keystate;
	bit bkeyisdb = 0;
	bit tmpbit = EA;

	EA = 0;
	
	keystate = key_state_from_isr;

	key_state_from_isr = 0;
	
	EA = tmpbit;

	IRcodeCheck = IRcodeCheck;

	if(pre_keystate != keystate)
	{
		pre_keystate = keystate;
		bkeyisdb = ui_key_is_ISDN(keystate);
	}
	else if(ui_key_allow_repeat(keystate) && pre_keystate == keystate)
	{
		pre_keystate = keystate;
		bkeyisdb = ui_key_is_ISDN(keystate);
	}
	else if(!ui_key_allow_repeat(keystate) && pre_keystate == keystate)
	{
		keystate = _NONE_KEY_MESSAGE;
	}
	
	if(bkeyisdb)
		keystate = _NONE_KEY_MESSAGE;
		
	return keystate;
#else
    UINT8 value, composekeyDB=0, keystate = 0,prevalue,count= 0 ;
    static UINT8 checkIRDB=0;
    static UINT8 pre_keystate = 0;
    bit bkeyisdb = 0,brepeatkey = 0;
//<Stanley K91013> Check IR code first.
	RTD_Log(LOGGER_DEBUG, "EE");
    if (IRcodeCheck!=0)
    {
        checkIRDB=0;
        keystate = ui_CheckKeyAttrProcess(IRcodeCheck);
//RTD_Log(LOGGER_INFO, "IRcodeCheck=%x\n", (UINT32)IRcodeCheck);
//RTD_Log(LOGGER_INFO, "keystate=%x\n", (UINT32)keystate);
        return keystate;
    }
    else
    {
        checkIRDB+=1;
    }
// LSADC0 (UP/Down/Left/Right)
    drv_lsadcValRead(PAD_LSADC_0,&value);
	//RTD_Log(LOGGER_INFO,"---PAD_LSADC_0-------%x\n",(UINT32)value);
	if ((value>_KEYPRESSED_THRESHOLD)&&(value< 0xf0))
    {
	    prevalue = value;
	    do
	    {
	        drv_lsadcValRead(PAD_LSADC_0,&value);
	        if (ABS(value, prevalue)<_KEY_THRESHOLD)
	        {
	            count++;
	        }
	        else
	        {
	            keystate = 0;
	            RTD_Log(LOGGER_INFO,"--keystate- wrong value--------%x\n",(UINT32)value);
	            return keystate;
	        }
	    }
	    while (count<3);

	    for(count=0;count<sizeof(KeyCodeADC0Msg)/2;count++)
	    {
	    	if(ABS(value,KeyCodeADC0Msg[count][0])<_KEYPRESSED_THRESHOLD)
	    	{
	    		keystate = KeyCodeADC0Msg[count][1];
	    		if(pre_keystate == keystate)
	    		{
	    			if(!ui_key_allow_repeat(keystate))
	    				brepeatkey = _TRUE;
				}
				if(!brepeatkey)
					bkeyisdb = ui_key_is_ISDN(keystate);
	    		break;
	    	}
	    }
    }
    
    drv_lsadcValRead(PAD_LSADC_1,&value);
	//RTD_Log(LOGGER_INFO,"---PAD_LSADC_1-------%x\n",(UINT32)value);
	if ((value>_KEYPRESSED_THRESHOLD)&&(value< 0xf0))
    {
	    prevalue = value;
	    do
	    {
	        drv_lsadcValRead(PAD_LSADC_1,&value);
	        if (ABS(value, prevalue)<_KEY_THRESHOLD)
	        {
	            count++;
	        }
	        else
	        {
	            keystate = 0;
	            RTD_Log(LOGGER_INFO,"--keystate- wrong value--------%x\n",(UINT32)value);
	            return keystate;
	        }
	    }
	    while (count<3);

	    for(count=0;count<sizeof(KeyCodeADC1Msg)/2;count++)
	    {
	    	if(ABS(value,KeyCodeADC1Msg[count][0])<_KEYPRESSED_THRESHOLD)
	    	{
	    		keystate = KeyCodeADC1Msg[count][1];
	    		if(pre_keystate == keystate)
	    		{
	    			if(!ui_key_allow_repeat(keystate))
	    				brepeatkey = _TRUE;
				}
				if(!brepeatkey)
					bkeyisdb = ui_key_is_ISDN(keystate);
	    		break;
	    	}

	    }
    }

	pre_keystate = keystate;
	if(bkeyisdb || brepeatkey)
		keystate = 0;
		
    if (!drv_pcb_get_gpio_pin(GPIO_POWER_KEY))
    {
    	fw_timer_event_DelayXms(5);
		if(!drv_pcb_get_gpio_pin(GPIO_POWER_KEY))  
		{
        	keystate = _POWER_KEY_MESSAGE;
        	RTD_Log(LOGGER_INFO, "Accept Powre key\n");
		}
    }
    if (IRcodeCheck==0 && keystate==0)
    {
        if (checkIRDB<7)
            return keystate;
        checkIRDB=0;
    }
    
    keystate=ui_CheckKeyAttrProcess(keystate);
    return keystate;
#endif    
}

#ifdef CONFIG_SCART_AUTO_SWITCH
#define _SCART_THRESHOLD			0x0a
#define _SCART_16x9_THRESHOLD		0x5d
#define _SCART_4x3_THRESHOLD		0xd5
UINT8 ui_SCART_GetVal(UINT8 scartno)
{
    UINT8 value;
    drv_lsadcValRead(scartno, &value);
    //RTD_Log(LOGGER_INFO, "SCART FS LSADC%d = %x\n",(UINT32)scartno, (UINT32)value);

    //if(ABS(value, _SCART_16x9_THRESHOLD)< _SCART_THRESHOLD)
    //	return _SCART_FS_HIGH_16x9;
    //else if(ABS(value, _SCART_4x3_THRESHOLD)< _SCART_THRESHOLD)
    if (value > (_SCART_4x3_THRESHOLD - _SCART_THRESHOLD))
        return _SCART_FS_HIGH_4x3;
    else if ( value > (_SCART_16x9_THRESHOLD - _SCART_THRESHOLD))
        return _SCART_FS_HIGH_16x9;

    return _SCART_FS_LOW;
}
#endif
void	ui_PowerOn_Proc(void)
{
    //COsdCtrlSetAudioVolume();
    UINT16 val;
	UINT16 val_2;
	UINT16 val_1;
    ap_osdctrl_audio_setting_init();
    LOAD_STORAGE_DATA(StructSystemDataType,  ZoomMode, val /*disp_info.CurrZoomMode*/);
	
	LOAD_STORAGE_DATA(StructSystemDataType,  IrMode, val_1);
	
	SET_OSD_DISPLAY_IR(val_1);
	if (val_1 == 0) 
        {
			STB_IR_EN(1);
           
        }
        else
    	{
    		STB_IR_EN(0);
			
    	  		 if(val_1==1)
    	   		{
		   		STB_IR_CH(0);
    	   		}
		   	else
	   		{
				STB_IR_CH(1);	
	   		}
	   		
		  	
    	}
	#if CONFIG_ENABLE_FM
	QND_Init();
	Delay5ms(10);
	LOAD_STORAGE_DATA(StructSystemDataType,  FMMode, val_2);
	SET_OSD_DISPLAY_Radio(val_2);
	AdjustFMfun(val_2);
	#endif
    //RTD_Log(LOGGER_LEVEL,"\n Tim ZoomMode = %d  \n",(UINT32)val);
    disp_info.UserZoomMode = val;
    if (val == 2)
        disp_info.CurrZoomMode = 1;
    else
        disp_info.CurrZoomMode = val;
    //Low Voltage Reset set to 0x11
    rtd_outb(SYS_POR_0x2c78, 0x00);//0x03 change for avoid 3.3v panel always reset
    //Set uart path
    COsdCtrlSetUART(ucUartPath);
}

void	ui_PowerOff_Proc(void)
{
    //Low Voltage Reset set to 0
    rtd_outb(SYS_POR_0x2c78, 0x00);
}
//extern void Beep_Process(void);



UINT8 ui_CheckPowerKey(void)
{
	/*

    UINT8 value;
	
    //drv_lsadcValRead(PAD_LSADC_0,&value);
	drv_lsadcValRead(PAD_LSADC_1,&value);
	
    if(ABS(value,0x20)<_KEYPRESSED_THRESHOLD)
	{
		RTD_Log(LOGGER_LEVEL,"\n melon value = %d  \n",(UINT32)value);
		Ds_DelayXms(5);
		drv_lsadcValRead(PAD_LSADC_1,&value);
	    if(ABS(value,0x20)<_KEYPRESSED_THRESHOLD)
		{
			return _TRUE;	
		}
	}
	*/
    return _FALSE;
}


 

//----------------------------------
void DelayStartHight(void)
{
	// Delay 4.5ms 
	// Insert Delay code to here.
	UINT16 idata i;


	//for(i=0;i<0x465;i++)
	for(i=0;i<1220;i++)//1152
	//for(i=0;i<1235;i++)
	{
		_nop_();
		_nop_();
		_nop_();
		_nop_();

		_nop_();
		_nop_();
		_nop_();
		_nop_();

		_nop_();
	}	
}
//----------------------------------
void DelayBit1(void)
{
	// Delay 1680us
	// Insert Delay code to here.
	UINT16 idata i;


	//for(i=0;i<0x17f;i++)
	for(i=0;i<410;i++)
	//for(i=0;i<430;i++)
	{
		_nop_();
		_nop_();
		_nop_();
		_nop_();

		_nop_();
		_nop_();
		_nop_();
		_nop_();

		_nop_();
		//add
		//_nop_();
	}
}

//----------------------------------
void DelayBit0(void)
{
	// Delay 560us
	// Insert Delay code to here.
	UINT16 idata i;


	//for(i=0;i<0x7a;i++)//122
	for(i=0;i<108;i++)
	{
		_nop_();
		_nop_();
		_nop_();
		_nop_();

		_nop_();
		_nop_();
		_nop_();
		_nop_();

		_nop_();
	}   
}
//----------------------------------
void DelayStartLow(void)
{
	// Delay 4.5ms 

	DelayStartHight();
	DelayStartHight();
}
#if CONFIG_TU300
//----------------------------------
void DelayDtvStartHight(void)
{
	// Delay 4.5ms 
	// Insert Delay code to here.
	UINT16 idata i;


	//for(i=0;i<0x465;i++)
	//for(i=0;i<1220;i++)//1152
	for(i=0;i<1235;i++)
	{
		_nop_();
		_nop_();
		_nop_();
		_nop_();

		_nop_();
		_nop_();
		_nop_();
		_nop_();

		_nop_();
	}	
}
//----------------------------------
void DelayDtvBit1(void)
{
	// Delay 1680us
	// Insert Delay code to here.
	UINT16 idata i;


	//for(i=0;i<0x17f;i++)
	//for(i=0;i<410;i++)
	for(i=0;i<430;i++)
	{
		_nop_();
		_nop_();
		_nop_();
		_nop_();

		_nop_();
		_nop_();
		_nop_();
		_nop_();

		_nop_();
		//add
		_nop_();
	}
}

//----------------------------------
void DelayDtvBit0(void)
{
	// Delay 560us
	// Insert Delay code to here.
	UINT16 idata i;


	//for(i=0;i<0x7a;i++)//122
	for(i=0;i<108;i++)
	{
		_nop_();
		_nop_();
		_nop_();
		_nop_();

		_nop_();
		_nop_();
		_nop_();
		_nop_();

		_nop_();
	}   
}
//----------------------------------
void DelayDtvStartLow(void)
{
	// Delay 4.5ms 

	DelayStartHight();
	DelayStartHight();
}
#endif
#if 0
void Send_IR(UINT16 usCode)
{
	UINT8 idata i;
	UINT16 idata usCC = DVD_CUSTOM_CODE;//客户码
	#if CONFIG_ENABLE_USB
	static UINT32 temp;
	#endif
	//UINT16 idata usTemp = (usCC & 0xFF) << 8;

	//usCC = (usCC >>8) | usTemp;
	
	temp=rtd_inl(GPDATO_0x2584);
	//rtd_outl(GPDATO_0x2584, temp|0x0020);
	//Delay5ms(200);
	//rtd_outl(GPDATO_0x2584, temp&(~0x0020));

	EA = 0;
	// IR Start
	rtd_outl(GPDATO_0x2584, temp&(~0x0020));
	DelayStartLow();
	rtd_outl(GPDATO_0x2584, temp|0x0020);
	DelayStartHight();

	// Send Custom Code
	for( i = 0; i< 16; i++)
	{
		rtd_outl(GPDATO_0x2584, temp&(~0x0020));
		DelayBit0();
		rtd_outl(GPDATO_0x2584, temp|0x0020);

		if(usCC & 0x0001)    
			DelayBit1();
		else                 
			DelayBit0();

		usCC = usCC >> 1;
	}
	
	// Send Key Code
	for(i=0;i<16;i++)
	{
		rtd_outl(GPDATO_0x2584, temp&(~0x0020));
		DelayBit0();

		rtd_outl(GPDATO_0x2584, temp|0x0020);
#if(ENDIAN_STYLE == LITTLE_ENDIAN_ENABLE)
		if(usCode & 0x0001)
			DelayBit1();
		else
			DelayBit0();

		usCode = usCode >> 1;
#elif(ENDIAN_STYLE == BIG_ENDIAN_ENABLE)	
		if(usCode & 0x8000)
			DelayBit1();
		else
			DelayBit0();

		usCode = usCode << 1;
#endif
	}

	rtd_outl(GPDATO_0x2584, temp&(~0x0020));
	DelayBit0();

	rtd_outl(GPDATO_0x2584, temp|0x0020);

	EA = 1;
}
#endif
void Send_IR_isr(UINT16 usCode)
{
	UINT8 idata i,GPIO_IR;
	
	UINT16 idata usCC = DVD_CUSTOM_CODE;//客户码
	//if(usCode==0xe00f)
	//usCC = IC_CUSTOM_CODE;	
	//UINT16 idata usTemp = (usCC & 0xFF) << 8;
	#if CONFIG_TU300
	if(GET_INPUTSOURCE_INDEX()== _SOURCE_INDEX_AV_1)
	{
		GPIO_IR=GPIO_DTV_IR_SEND;
	}
	else 
	;//GPIO_IR=GPIO_IR_SEND;	
	#endif
	//usCC = (usCC >>8) | usTemp;

	//EA = 0;
	// IR Start
	drv_pcb_set_gpio_pin_isr(GPIO_IR,0);
	DelayStartLow();
	drv_pcb_set_gpio_pin_isr(GPIO_IR,1);
	DelayStartHight();

	// Send Custom Code
	for( i = 0; i< 16; i++)
	{
		drv_pcb_set_gpio_pin_isr(GPIO_IR,0);
		DelayBit0();
		drv_pcb_set_gpio_pin_isr(GPIO_IR,1);

		if(usCC & 0x0001)    
			DelayBit1();
		else                 
			DelayBit0();

		usCC = usCC >> 1;
	}
	
	// Send Key Code
	for(i=0;i<16;i++)
	{
		drv_pcb_set_gpio_pin_isr(GPIO_IR,0);
		DelayBit0();

		drv_pcb_set_gpio_pin_isr(GPIO_IR,1);
#if(ENDIAN_STYLE == LITTLE_ENDIAN_ENABLE)
		if(usCode & 0x0001)
			DelayBit1();
		else
			DelayBit0();

		usCode = usCode >> 1;
#elif(ENDIAN_STYLE == BIG_ENDIAN_ENABLE)	
		if(usCode & 0x8000)
			DelayBit1();
		else
			DelayBit0();

		usCode = usCode << 1;
#endif
	}

	drv_pcb_set_gpio_pin_isr(GPIO_IR,0);
	DelayBit0();

	drv_pcb_set_gpio_pin_isr(GPIO_IR,1);

	//EA = 1;
}
extern volatile UINT16 xdata fUI_Msg;
#if 1

UINT8 ui_ReadKeyPad_isr(void)
{
	UINT8 value, composekeyDB=0, keystate = 0,prevalue,count= 0 ;
	static UINT8 pre_keystate = 0;
	//static UINT16 keyhold = 1;

	keystate = _NONE_KEY_MESSAGE;
#if 0
	drv_lsadcValRead_isr(PAD_LSADC_1,&value);
	if (value< 0xf0)
	{
		RTD_Log(LOGGER_DEBUG , "value is %x--------------------------------\r\n", (UINT32)value);
		prevalue = value;
		do
		{
			drv_lsadcValRead_isr(PAD_LSADC_1,&value);
			if (ABS(value, prevalue)<_KEY_THRESHOLD)
			{
				count++;
			}
			else
			{
				keystate = 0;
				return keystate;
			}
		}
		while (count<3);
		

		for(count=0;count<sizeof(KeyCodeADC1Msg)/2;count++)
		{
			if(ABS(value,KeyCodeADC1Msg[count][0])<_KEYPRESSED_THRESHOLD)
			{
				keystate = KeyCodeADC1Msg[count][1];
			}
		}
	RTD_Log(LOGGER_DEBUG , "melon_keystate is %x--------------------------------\r\n", (UINT32)keystate);
		if(keyhold>25)
		{
			keyhold = 0;
			pre_keystate = _NONE_KEY_MESSAGE;
			return _POWER_KEY_MESSAGE;
		}
		else if(keystate == _POWER_KEY_MESSAGE)
		{
		
			if(keyhold!=0)
			pre_keystate = _POWER_KEY_MESSAGE;
			if(keyhold)
				keyhold++;
			return _NONE_KEY_MESSAGE;
		}
	}
	else
	{
	
		keyhold = 1;	
		{
			if(pre_keystate == _POWER_KEY_MESSAGE)
			{
				pre_keystate = _NONE_KEY_MESSAGE;
				if(GET_POWERSTATUS() == POWER_STATUS_POWER_OFF)
				{
					
					return _POWER_KEY_MESSAGE;
				}
				else
					return _OK_KEY_MESSAGE;
			}
		}
	}
#endif
#if 1
	drv_lsadcValRead_isr(PAD_LSADC_0,&value);
	if (value< 0xf0)
	{
		//RTD_Log(LOGGER_DEBUG , "value is %x--------------------------------\r\n", (UINT32)value);
		prevalue = value;
		do
		{
			drv_lsadcValRead_isr(PAD_LSADC_0,&value);
			if (ABS(value, prevalue)<_KEY_THRESHOLD)
			{
				count++;
			}
			else
			{
				keystate = 0;
				return keystate;
			}
		}
		while (count<55);

		for(count=0;count<sizeof(KeyCodeADC0Msg)/2;count++)
		{
			if(ABS(value,KeyCodeADC0Msg[count][0])<_KEYPRESSED_THRESHOLD)
			{
				if(ABS(value,KeyCodeADC0Msg[count][0])<_KEYPRESSED_THRESHOLD)
				keystate = KeyCodeADC0Msg[count][1];
			}
		}
	}
		
	if (!drv_pcb_get_gpio_pin_isr(GPIO_POWER_KEY))
	{
		if(pre_keystate == _POWER_KEY_MESSAGE)
			keystate = _POWER_KEY_MESSAGE;
		else
			pre_keystate = _POWER_KEY_MESSAGE;	
	}
	else
	#endif
	pre_keystate = keystate;
		
	return keystate;
}


void ui_detect_keypad_isr(void)
{
	static UINT8 pre_keystate = 0; 
	UINT8 keystate;
	bit bkeyisdb = 0;
	
	keystate = ui_ReadKeyPad_isr();
	

	if(keystate == 0)
	{
		pre_keystate = 0;
		return;
	}	
	#if 1
	if(pre_keystate != keystate)
	{
		pre_keystate = keystate;
		//bkeyisdb = ui_key_is_ISDN(keystate);
		
	}
	else if(ui_key_allow_repeat(keystate) && pre_keystate == keystate)
	{
		pre_keystate = keystate;
		//bkeyisdb = ui_key_is_ISDN(keystate);
	}
	else 
	#endif
	#if 1
	if(!ui_key_allow_repeat(keystate) && pre_keystate == keystate )
	{

	     //bkeyisdb=ui_LONG_key_is_ISDN(keystate); 
		 //if(keystate!=_POWER_KEY_MESSAGE)
		 keystate = _NONE_KEY_MESSAGE;
	}
	#if CONFIG_TU300
	if(bkeyisdb&&(keystate!=_POWER_KEY_MESSAGE))
		keystate = _NONE_KEY_MESSAGE;
	#endif
	#endif
	if(keystate!=_NONE_KEY_MESSAGE)
	{
		//RTD_Log(LOGGER_INFO,"keystate %x ",(UINT32)keystate);
		if(keystate==_DVD_PLAY_PAUSE_MESSAGE)
		{
			keystate=_OK_IR_MESSAGE;
		}
		fw_isr_ui_message_push(keystate);
	}
}
#endif
