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


#ifndef _ULTRAZOOM_H_
#define _ULTRAZOOM_H_
/*============================ Module dependency  ===========================*/
#include "sparrow.h"

/*===================================  Types ================================*/
typedef struct _SZIE
{
    UINT16	nWidth;
    UINT16	nLength;
} SIZE;

typedef struct _UZ_Para
{
    UINT8 panorama;
    UINT8 Input422_flag : 1;
    UINT8 VScale_En : 1;
    UINT8 HScale_En : 1;
    UINT8 Video_Com_En : 1;
    SIZE		InSize;
    SIZE		OutSize;
//UINT16 InLen;		move to InSize
//UINT16 InWid;		move to InSize
//UINT16 OutLen;		move to OutSize
//UINT16 OutWid;		move to OutSize
} UZ_Para;


/*================================== Variables ==============================*/
/*================================ Definitions ==============================*/
/*================================== Function ===============================*/
/*============================================================================*/
/**
 * UltraZoom_ConfigScaling
 * Config scaler up/down
 *
 * @param <void>		{ void }
 * @return 			{ void }
 *
 */
void drvif_color_ultrazoom_config_scaling_down(UZ_Para *UZD);
void drvif_color_ultrazoom_config_scaling_up(UZ_Para *UZU);
/*============================================================================*/
/**
 * UltraZoom_SetScaleUpInputSize
 * Enable/disable vertical or horizontal scaling up,
 * including main and sub display. It is to set scale control register.
 * @param <info> {info structure}
 * @nWid
 * @nLen
 * @return { none }
 *
 */
void UltraZoom_SetScaleUpInputSize(UINT8 display, UINT32 nWid, UINT32 nLen);

#ifdef CONFIG_IDTV_US
void UltraZoom_ConfigScaling(void);
#endif

void drvif_color_scaler_setds_pk(/*UINT8 display, */UINT8 factor);
void drvif_color_scaler_setscaleup_vcoef(UINT8 factor);
void drvif_color_scaler_setscaleup_hcoef(UINT8 factor);
void drvif_color_supk_mask( UINT8 type);
void drvif_color_sunr_middleware(UINT8 which_source, UINT8 *level_y, UINT8 *level_c);
void drvif_color_sunr_y( UINT8 level);
void drvif_color_sunr_c( UINT8 level);

#endif
/*======================== End of File =======================================*/
/**
*
* @}
*/

