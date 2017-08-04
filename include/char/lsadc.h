/*=============================================================
 * Copyright C        Realtek Semiconductor Corporation, 2005 *
 * All rights reserved.                                       *
 *============================================================*/

/*======================= Description ========================*/
/**
 * @file
 *   This header file for RTD2638's registers define.
 * @Author Tzungder Lin
 * @date 2006/06/19
 * @version { 1.0 }
 *
 */

#ifndef _LSADC_H_
#define _LSADC_H_

#include "sparrow.h"

enum PAD_LSADC
{
    PAD_LSADC_0 = 0,
    PAD_LSADC_1,
    PAD_LSADC_2,
    PAD_LSADC_3,
    PAD_LSADC_4,
    PAD_LSADC_5,
    PAD_LSADC_6,
    PAD_LSADC_7,
    PAD_LSADC_TOTAL
};

enum PAD_LSADC_SWITCH_PIN
{
    PAD_LSADC_SWITCH_PIN_0 = 0,
    PAD_LSADC_SWITCH_PIN_1,
    PAD_LSADC_SWITCH_PIN_2,
    PAD_LSADC_SWITCH_PIN_3,
    PAD_LSADC_SWITCH_PIN_4,
    PAD_LSADC_SWITCH_PIN_5,
    PAD_LSADC_SWITCH_PIN_6,
    PAD_LSADC_SWITCH_PIN_7
};

enum PAD_LSADC_MODE_CONTROL
{
    PAD_LSADC_VOLTAGE_MODE = 0,
    PAD_LSADC_CURRENT_MODE,
};
/**
 *A struct.
 *The pad configuration of the low speed ADC.
 */
typedef struct _STRUCT_PAD_LSADC_PARAMETER
{
    UINT8	bEnable:1;
    UINT8	nPad_Switch:4;
    UINT8	bMode_Control:1;
    UINT8	nThreshold_Value;
    UINT8	bCurrentValue;
} STRUCT_PAD_LSADC_PARAMETER;

/**
 *A struct.
 *The configuration of the low speed ADC module.
 */
typedef struct _LSADC_CONFIG
{
    UINT8 	selWait:4;
    UINT8	selClk:4;
    UINT8	debounce:4;
    STRUCT_PAD_LSADC_PARAMETER		stPadLSAdc[PAD_LSADC_TOTAL];
} LSADC_CONFIG_PARAMETER;

void drv_lsadc_Init(void);
bit drv_lsadcValRead(UINT8 pad, UINT8 * buf);
void drv_lsadc_PowerOn(bit on_off);
bit drv_lsadcValRead_isr(UINT8 pad, UINT8 * buf);

#endif
