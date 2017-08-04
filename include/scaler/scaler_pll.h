/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2006
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is for scaler clock related functions.
 *
 * @author 	$Author$
 * @date		$Date$
 * @version	$Revision$
 * @ingroup	clock
 */

/**
 * @addtogroup clock
 * @{
 */

#ifndef _SCALER_PLL_H_
#define _SCALER_PLL_H_

/*============================ Module dependency  ===========================*/
#include "sparrow.h"

/*===================================  Types ================================*/
/*================================== Variables ==============================*/
/*================================ Definitions ==============================*/
#define _USER_DPLL_NCODE    10//12//16
#define _USER_APLL_NCODE 	3		///! NEVER change this setting !!!
#define _USER_APLL_PE 		100
#define _USER_APLL_n 		8
#define _MCLK_NCODE  		5		///! Do not exceed 20!
#define _DCLK_SPRED_RANGE	0		///! DCLK Spreading range : 0~15

#define _PLL_D 0x00
#define _PLL_M 0x01
//new parameter
#define _APLL_N_VALUE_FOR_P_CODE 8
#define _APLL_N_VALUE_FOR_I_CODE 9 // 512 = 2^9

//[Code Sync][AlanLi][0980617][1]
extern UINT8 select_adc_vco_div;
//[Code Sync][AlanLi][0980617][1][end]
/*================================== Function ===============================*/
void drv_pll_SetPLL(UINT8 M_Code, UINT8 N_Code, UINT8 div, UINT8 CPC);

/*============================================================================*/
/**
 * drvif_clock_set_dclk
 * Set display clock setup
 *
 * @param <void>		{ void }
 * @return 			{ void }
 *
 */
bit drv_pll_Set_DClk(UINT16 niHFreq, UINT16 niVActLen, UINT32 default_freq);


//--------------------------------------------------
// Description  : Adjust Dclk offset
// Input Value  : Dclk offset
// Output Value : None
//--------------------------------------------------
void drv_pll_Set_DClk_offset(UINT16 usDclkOffset);

/*============================================================================*/
/**
 * drvif_clock_set_aclk
 * Set ADC clock setup
 *
 * @param <UINT16>	{ freq in KHz}
 * @return 			{ void }
 *
 */
bit drv_pll_Set_AClk(UINT32 usClock, UINT8 fast_lock_enable);
void drv_pll_pfd_calibration(void);
UINT8 drv_pll_cal_cpc(UINT8 M_Code,UINT8 N_Code); // CPC : charge pump current

//void drv_pll_power_AdcApllEnable(UINT8 on_off);
//[Code Sync][AlanLi][0980617][1]
void drv_pll_select_VCO_div(void);
//[Code Sync][AlanLi][0980617][1][end]

#ifdef APLL_NEW_MODE
void drv_pll_set_aclk_newmode(UINT8 NewModeEnable);
#endif

#endif // _SCALER_PLL_H_
/*======================== End of File =======================================*/
/**
*
* @}
*/

