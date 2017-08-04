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
void drv_tuner_initRT810_FM(void);
//UINT8 drv_tuner_SetFreqafc(UINT32 freq);
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
#else

#endif
#if 0
void drv_tuner_isr_TunerAudioStdInit(UINT8 mode);
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
