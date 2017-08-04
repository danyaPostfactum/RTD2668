/*=============================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2005
  * All rights reserved.
  * ============================================ */

/*================= File Description ================= */
/**
 * @file
 * The file is a declaration and definition header file
 *
 * @author 	$Author:  $
 * @date 	$Date:  $
 * @version 	$Revision:  $
 * @ingroup
 */

/**
* @addtogroup
* @
*/
#ifndef _TUNER_H
#define _TUNER_H
#include "sparrow.h"
#include "source.h"

/*==================== Definitions ================ */
/*=============================================================*/
/**
	This routine will set init.
	@param
	@return
*/

//----------------------------------------------------------//
//                   RT810 Parameter                        //
//----------------------------------------------------------//
#ifdef CONFIG_TUNER_SILICON

typedef enum _RT610_ErrCode
{
	RT_Success = _TRUE,
	RT_Fail    =_FALSE
}RT610_ErrCode;

#define RT610_ADDRESS 0x74
#define RT610_Xtal	  16005
#define RT610_Delay_MS	fw_timer_event_DelayXms

typedef enum _RT610_Standard_Type
{
	NTSC_M = 0,
	PAL_G,
	PAL_I,
	PAL_D,
	PAL_B,
	SECAM_L,
	ATV = 0xFF
}RT610_Standard_Type;

typedef enum _RT610_IF_Type
{
	NTSC_5820   = 5820,
	PAL_G_7820  = 7820,
	PAL_I_7820  = 7820,
	PAL_D_7820  = 7820,
	PAL_B_6820  = 6820,
	SECAM_L_7820 = 7820
}RT610_IF_Type;

typedef enum _RT610_BW_Type
{
	BW_6MHz = 0,
	BW_7MHz,
	BW_8MHz
}RT610_BW_Type;

typedef enum _RT610_LoopThrough_Type
{
	LOOP_THROUGH = TRUE,
	SIGLE_IN     = FALSE
}RT610_LoopThrough_Type;

typedef enum _RT610_GPIO_Type
{
	HI_SIG = TRUE,
	LO_SIG = FALSE
}RT610_GPIO_Type;

typedef struct _RT610_Set_Info
{
	UINT32 RF_KHz;
	RT610_IF_Type IF_KHz;
	RT610_BW_Type RT610_BW;
	RT610_Standard_Type RT610_Standard;
	RT610_LoopThrough_Type RT_Input;
}RT610_Set_Info;

#endif
/*================================================= */
/*==================== Definitions ================ */
/*=============================================================*/
/**
	This routine will set init.
	@param			
	@return 			
*/

void CTunerGlobalVarInit(void);
void drv_tuner_TunerAudioStdInit(UINT8 mode);
UINT8 drv_tuner_SetFreq(UINT32 freq);

UINT8 drv_tuner_CheckBand(UINT32 freq);
UINT32 drv_tuner_GetChanelMaxFreq(void);
UINT32 drv_tuner_GetChanelMinFreq(void);

UINT16 drv_tuner_GeDiverRatio(UINT32 freq);
UINT32 drv_tuner_GetRatioInStepSize(void);

UINT32 drv_tuner_GetPictureInterFreq(void);

/*=============================================================*/
/**
	This routine will set TV to specific frequency  in Search mode
	@param			freq					frequency
	@return 			Return the status code	_TURE,_FALSE
*/
UINT8 CTunerSetFreqSearch(UINT32 freq);
/*=============================================================*/
/**
	This routine will get TV to current frequency.
	@param			void
	@return 			Return the freq	frequency
*/

UINT32 CTunerGetFreq(void);

#ifdef CONFIG_TUNER_SILICON
RT610_ErrCode RT610_Init(void);
RT610_ErrCode RT610_SetFrequency(RT610_Set_Info RT610_INFO);
RT610_ErrCode RT610_Standby(RT610_LoopThrough_Type RT610_LoopSwitch);//Edit SVN53

void drv_tuner_initRT810_FM(void);
UINT8 drv_tuner_SetFreqafc(UINT32 freq);
UINT8  ADCRead(UINT8 a, UINT8 b, UINT8 c, UINT8 d);
UINT8  NADCRead(UINT8 a, UINT8 b);
void  writeIQ(UINT8 a, UINT8 b);
int Calli_4_dir(UINT8, UINT8*) ;
void IMRAutoCal(UINT8 skip) ;
void drv_tuner_initRT810(UINT8 mode);
void drv_tuner_write_rt810(UINT8* pRegsMap);//,UINT8 count);
void drv_tuner_write_rt810_cali(UINT8* pRegsMap);//,UINT8 count);
void drv_tuner_write_rt810_vcoonly(UINT8* pRegsMap);
void drv_tuner_RT810_poweroff(void);
//void drv_tuner_RT810_input_selection(UINT8 rf);
#else

#endif
#ifdef CONFIG_IFD_TUNER_AUTO
/*=============================================================*/
/**
	This routine will set TV to specific frequency.
	@param			freq					frequency
	@return 			Return the status code	_TURE,_FALSE
*/
UINT8 drv_tuner_isr_SetFreq(UINT32 freq);
#endif
#endif // #ifdef _TUNER_H
