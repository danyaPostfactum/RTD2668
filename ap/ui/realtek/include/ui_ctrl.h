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
#ifndef _UI_CTRL_H
#define _UI_CTRL_H
#include "sparrow.h"
typedef struct _LsADC_Key_Attr
{
    UINT8 keyCode;
    UINT8 keyAttr;
} LsADC_Key_Attr;

//===== Keypad operate type =====//
#define ONE_PUSH		0x01	// Accept one push only
#define DOUBLE_CLICK	0x02	// Double click 
#define LONG_PUSH		0x04	// Accept long push
#define REPEAT_LOW		0x08
#define REPEAT_FAST		0x10
#define REPEAT_NON_LINEAR	0x20

#define COMPOSE_KEY 	0x80
#define SINGAL_KEY		0x00

enum
{
    _NONE_KEY_MESSAGE,
    _POWER_KEY_MESSAGE,
     _MENU_KEY_MESSAGE,
      _OK_KEY_MESSAGE,
    _RIGHT_KEY_MESSAGE,
    
    _LEFT_KEY_MESSAGE,
    _DIMMER_KEY_MESSAGE,
    //_ASPECT_KEY_MESSAGE,
    _SOURCE_KEY_MESSAGE,
    _MENU_IR_MESSAGE,
    _LEFT_IR_MESSAGE,
    
    _RIGHT_IR_MESSAGE,//10
    _SOURCE_IR_MESSAGE,
        _OK_IR_MESSAGE,
    _CHANNEL_INC_MESSAGE,
    _CHANNEL_DEC_MESSAGE,
    
    _VOLUME_INC_MESSAGE,
    _VOLUME_DEC_MESSAGE,
    _PIPMP_KEY_MESSAGE,
    _ENTER_KEY_MESSAGE,
    _UP_KEY_MESSAGE,
    
    _DOWN_KEY_MESSAGE,//20
    _EXIT_KEY_MESSAGE,
    _CHGUIDE_KEY_MESSAGE,
    _MULTICH_KEY_MESSAGE,
    _DISPLAY_KEY_MESSAGE,
    
    _3D_RESET_MESSAGE,
    _RESET_MESSAGE,
    _MUTE_KEY_MESSAGE,
    _SLEEP_KEY_MESSAGE,
    _WIDE_KEY_MESSAGE,
    
    _JUMP_KEY_MESSAGE,//30
    _VIDEO_KEY_MESSAGE,
    _AUDIO_KEY_MESSAGE,
    _FREEZE_KEY_MESSAGE,
    _AUDIO_DEMO_KEY_MESSAGE,
    
    _VOLUMNBAR_KEY_MESSAGE,
    _DASH_KEY_MESSAGE,
    _CC_KEY_MESSAGE,
    _DISPLAY_RATIO_KEY_MESSAGE,
    _DISPLAY_RATIO_IR_MESSAGE,
    _RETURN_KEY_MESSAGE,
    _CAPTION_KEY_MESSAGE,//40
    _TELETEXT_KEY_MESSAGE,
    _SIZE_KEY_MESSAGE,
    _MIXER_KEY_MESSAGE,

    _TTX_REVEAL_KEY_MESSAGE,
    _TTX_HOLD_KEY_MESSAGE,
    _TTX_SUBPAGE_KEY_MESSAGE,
    _TTX_INDEXL_KEY_MESSAGE,

    _REDBUTTON_MESSAGE,
    _BLUEBUTTON_MESSAGE,
    _GREENBUTTON_MESSAGE,//50
    _YELLOWBUTTON_MESSAGE,

	
	
    IR_0_KEY_MESSAGE,
    IR_1_KEY_MESSAGE,
    IR_2_KEY_MESSAGE,
    IR_3_KEY_MESSAGE,
    IR_4_KEY_MESSAGE,
    IR_5_KEY_MESSAGE,
    IR_6_KEY_MESSAGE,
    IR_7_KEY_MESSAGE,
    IR_8_KEY_MESSAGE,//60
    IR_9_KEY_MESSAGE,

    //--------------------IVAN 20090821-------------------------
    #if CONFIG_TU300
    _ISDB_POWER_MESSAGE,
    _ISDB_MUTE_MESSAGE,
    _ISDB_SUB_MESSAGE,
    _ISDB_SEARCH_MESSAGE,//65
    _ISDB_LIST_MESSAGE,
   // _ISDB_RECALL_MESSAGE,
    _ISDB_AUDIO_MESSAGE,
    _ISDB_EPG_MESSAGE,
    _ISDB_UP_MESSAGE,
    _ISDB_DOWN_MESSAGE,
    _ISDB_LEFT_MESSAGE,
    _ISDB_RIGHT_MESSAGE,
    _ISDB_OK_MESSAGE,
    _ISDB_MENU_MESSAGE,
    _ISDB_EXIT_MESSAGE,
    _ISDB_KEY_0_MESSAGE,
    _ISDB_KEY_1_MESSAGE,
	_ISDB_KEY_2_MESSAGE,
    _ISDB_KEY_3_MESSAGE,
    _ISDB_KEY_4_MESSAGE,
    _ISDB_KEY_5_MESSAGE,
	_ISDB_KEY_6_MESSAGE,
    _ISDB_KEY_7_MESSAGE,
    _ISDB_KEY_8_MESSAGE,
    _ISDB_KEY_9_MESSAGE,
    _ISDB_KEY_10_MESSAGE,
    #if 0
    _ISDB_KEY_11_MESSAGE,
    _ISDB_KEY_12_MESSAGE,
    _ISDB_KEY_100_MESSAGE,
    _ISDB_CH_INC_MESSAGE,
    _ISDB_CH_DEC_MESSAGE,
    _ISDB_VOL_INC_MESSAGE,
    _ISDB_VOL_DEC_MESSAGE,
	#endif
    
    //------------------END IVAN 20090821----------------------
	#endif
	
	  _DVD_0_MESSAGE,
	  _DVD_1_MESSAGE,
	  _DVD_2_MESSAGE,
	  _DVD_3_MESSAGE,
	  _DVD_4_MESSAGE,
	  _DVD_5_MESSAGE,
	  _DVD_6_MESSAGE,
	  _DVD_7_MESSAGE,
	  _DVD_8_MESSAGE,
	  _DVD_9_MESSAGE,
	  _DVD_EJECT_MESSAGE,
	  _DVD_DISPLAY_MESSAGE,
	  _DVD_GOTO_MESSAGE,
	  _DVD_TITLE_MESSAGE,
	  _DVD_SETUP_MESSAGE,
	  _DVD_RETURN_MESSAGE,
	  _DVD_AUDIO_MESSAGE,
	  _DVD_ANGLE_MESSAGE,
	  _DVD_SUBTITLE_MESSAGE,
	  _DVD_PAL_NTSC_MESSAGE,
	  _DVD_REPEAT_MESSAGE,
	  _DVD_SLOW_MESSAGE,
	  _DVD_STEP_MESSAGE,
	  _DVD_ZOOM_MESSAGE,
	  _DVD_PLAY_PAUSE_MESSAGE,
	  _DVD_STOP_MESSAGE,
	  _DVD_SET_A_MESSAGE,
	  _DVD_FREW_MESSAGE,
	  _DVD_FFWD_MESSAGE,
	  _DVD_PREV_MESSAGE,
	  _DVD_NEXT_MESSAGE,
	  _DVD_USB_SD_MESSAGE,
	  _DVD_ROOT_MENU_MESSAGE,
	  _DVD_LEFT_MESSAGE,
	  _DVD_RIGHT_MESSAGE,
	  _DVD_UP_MESSAGE,
	  _DVD_DOWN_MESSAGE,
	  _DVD_ENTER_MESSAGE,
	  _DVD_PROGRAM_MESSAGE,
	  _DVD_ADD10_MESSAGE,
    //????
    _HOTKEY_TIMING_SWAP_MESSAGE,
    _HOTKEY_RECALL_MESSAGE,
    _HOTKEY_FACTORY_MODE_MESSAGE

};

#if CONFIG_TU300
#define TV_POWER_CODE		0x10

#define ISDB_POWER_CODE		0x10
#define ISDB_MUTE_CODE		0x06
#define ISDB_SUB_CODE		0x40
#define ISDB_SEARCH_CODE	0x01
#define ISDB_LIST_CODE		0x4c
#define ISDB_AUDIO_CODE		0x0a
#define ISDB_EPG_CODE		0x00

#define ISDB_UP_CODE		0x04

#define ISDB_DOWN_CODE		0x0e
#define ISDB_LEFT_CODE		0x05
#define ISDB_RIGHT_CODE		0x4d

#define ISDB_OK_CODE		0x54
#define ISDB_MENU_CODE		0x1e
#define ISDB_EXIT_CODE		0x1a
#define ISDB_KEY_0_CODE		0x12
#define ISDB_KEY_1_CODE 	0x09
#define ISDB_KEY_2_CODE		0x1d
#define ISDB_KEY_3_CODE		0x1f
#define ISDB_KEY_4_CODE		0x0d
#define ISDB_KEY_5_CODE		0x19
#define ISDB_KEY_6_CODE		0x1b
#define ISDB_KEY_7_CODE		0x11
#define ISDB_KEY_8_CODE		0x15
#define ISDB_KEY_9_CODE		0x17
//no define
#if 0
#define ISDB_KEY_11_CODE	0x66
#define ISDB_KEY_12_CODE	0x67
#define ISDB_KEY_100_CODE	0x65
#define ISDB_CH_INC_CODE	0x92
#define ISDB_CH_DEC_CODE	0x93
#define ISDB_VOL_INC_CODE	0x6a
#define ISDB_VOL_DEC_CODE	0x6b
#define ISDB_RECALL_CODE	0x16
#define ISDB_PAD_MENU_CODE	0x68
#define ISDB_ENTER_CODE		0x60
#endif
#endif


#define  IR_89JDVD_DCT0    		0x10			// IRCODE_DCT0,				// 数字键0
#define  IR_89JDVD_DCT1    		0x4d			// IRCODE_DCT1,				// 数字键1
#define  IR_89JDVD_DCT2    		0x1f	// IRCODE_DCT2,				// 数字键2
#define  IR_89JDVD_DCT3    		0x0f			// IRCODE_DCT3,				// 数字键3
#define  IR_89JDVD_DCT4    		0x5a			// IRCODE_DCT4,				// 数字键4
#define  IR_89JDVD_DCT5    		0x59			// IRCODE_DCT5,				// 数字键5
#define  IR_89JDVD_DCT6    		0x1b			// IRCODE_DCT6,				// 数字键6
#define  IR_89JDVD_DCT7    		0x4a			// IRCODE_DCT7,				// 数字键7
#define  IR_89JDVD_DCT8    		0x46			// IRCODE_DCT8,				// 数字键8
#define  IR_89JDVD_DCT9    		0x11			// IRCODE_DCT9,				// 数字键9	
#define  IR_89JDVD_EJECT        0x1e                        // IRCODE_EJECT
#define  IR_89JDVD_DISPLAY      0x0e                        // IRCODE_EJECT
#define  IR_89JDVD_GOTO         0x1a                        // IRCODE_EJECT
#define  IR_89JDVD_TITLE        0x55                        // IRCODE_EJECT
#define  IR_89JDVD_SETUP        0x51//117                        // IRCODE_EJECT
#define  IR_89JDVD_RETURN       0x15                        // IRCODE_EJECT
#define  IR_89JDVD_AUDIO        0x5b                        // IRCODE_EJECT
#define  IR_89JDVD_ANGLE        0x58                        // IRCODE_EJECT
#define  IR_89JDVD_SUBTITLE     0x19                        // IRCODE_EJECT
#define  IR_89JDVD_PAL_NTSC     0x18                        // IRCODE_EJECT
#define  IR_89JDVD_REPEAT       0x47                        // IRCODE_EJECT
#define  IR_89JDVD_SLOW         0x44                        // IRCODE_EJECT
#define  IR_89JDVD_STEP         0x05                        // IRCODE_EJECT
#define  IR_89JDVD_ZOOM         0x04                        // IRCODE_EJECT
#define  IR_89JDVD_PLAY_PAUSE   0x43                        // IRCODE_EJECT
#define  IR_89JDVD_STOP         0x40                        // IRCODE_EJECT
#define  IR_89JDVD_SET_A        0x01                        // IRCODE_EJECT
#define  IR_89JDVD_FREW			0x4b                        // IRCODE_EJECT
#define  IR_89JDVD_FFWD			0x48                        // IRCODE_EJECT
#define  IR_89JDVD_PREV         0x09                        // IRCODE_EJECT
#define  IR_89JDVD_NEXT         0x08                        // IRCODE_EJECT
#define  IR_89JDVD_USB_SD       0x54                        // IRCODE_SEARCH,
#define  IR_89JDVD_ROOT_MENU    0x12                       // IRCODE_SEARCH,
#define  IR_89JDVD_LEFT    	    0x56			// IRCODE_LEFT,				// 左光标键
#define  IR_89JDVD_RIGHT    	0x4e			// IRCODE_RIGHT,			// 右光标键
#define  IR_89JDVD_UP    	    0x17			// IRCODE_UP,				// 上光标键
#define  IR_89JDVD_DOWN    	    0x52			// IRCODE_DOWN,				// 下光标键
#define  IR_89JDVD_ENTER        0x13                        // KEYCODE_ENTER
//#define  IR_89JDVD_ANGLE        0x58                        // IRCODE_EJECT

#define  IR_89JDVD_DCT10        13                        // KEYCODE_ENTER
#define  IR_89JDVD_PROGRAM      0x5f  
#define  IR_89JDVD_ADD10		0x4f

#define  IR_89JDVD_VOLUP		0X0A
#define IR_89JDVD_VOLDOWN		0X0B
#define IR_89JDVD_ASK_USB		0X5E
#define IR_89JDVD_OPEN_CMD		0X02





#define  IR_89JDVD_NONE         0XFF  


/**
 * ui_ConvertMessage
 * Translate adc value to message
 *
 * @param  <pad> 		{ adc's no. }
 * @param  <adc_Value> 	{ adc'value }
 * @return { message }
 *
 */
UINT8 ui_ReadKeyPad(UINT8 IRcodeCheck);
void  ui_PowerOn_Proc(void);
void  ui_PowerOff_Proc(void);
void ui_PowerSaving_EnterProc(void);
void ui_PowerSaving_ExitProc(void);
#define _SCART_FS_LOW				0x00
#define _SCART_FS_HIGH_4x3			0x01
#define _SCART_FS_HIGH_16x9			0x02
UINT8 ui_SCART_GetVal(UINT8 scartno);
UINT8 ui_CheckPowerKey(void);
void ui_PoweroffOpen(UINT8 cnt); 

UINT8 ui_CheckKeyAttrProcess(UINT8 keycode);
//void Send_IR(UINT16 usCode);
void Send_IR_isr(UINT16 usCode);
bit ui_key_allow_repeat(UINT8 key);

UINT8 ui_ReadKeyPad_isr(void);
void ui_detect_keypad_isr(void);
#if CONFIG_TU300
void DelayBit1(void);
void DelayStartHight(void);
void DelayBit0(void);
void DelayStartLow(void);

#endif
#if CONFIG_TU300
void DelayDtvBit1(void);
void DelayDtvStartHight(void);
void DelayDtvBit0(void);
void DelayDtvStartLow(void);

#endif



#endif //#ifndef _UI_CTRL_H
