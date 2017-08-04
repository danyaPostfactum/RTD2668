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

#include "char\lsadc.h"
#include "reg_def.h"

extern LSADC_CONFIG_PARAMETER code lsadcConfig;
/*============================================================================*/
/**
 * drv_lsadc_Init
 * Initial lsadc function referenced from lsadcConfig
 *
 * @param <void> 	{ void }
 * @param <extern>	{ specify lsadc table in lsadcConfig  }
 * @return 			{ void }
 *
 */
void drv_lsadc_Init(void)
{
    UINT8 count;
    UINT8 i;
    UINT32 getVal;
    STRUCT_PAD_LSADC_PARAMETER code *  ptPadConfig;

    //power off
    rtdf_clearBits(PAD_CTRL_0x2660, _BIT0);
    rtdf_clearBits(PAD_ANALOG_CTRL_0x2668, _BIT0);

	for(count = 0; count < 250; count++) 
		for(i = 0; i < 10; i++);
    for (count = 0; count < PAD_LSADC_TOTAL; count++)
    {
        ptPadConfig = &lsadcConfig.stPadLSAdc[count];
        getVal = 	((UINT32)(ptPadConfig->bEnable & 0x01) << 31) |
                  ((UINT32)(ptPadConfig->nThreshold_Value & 0xff) << 16) |
                  ((UINT32)(ptPadConfig->nPad_Switch & 0x0f) << 12) |
                  ((UINT32)(ptPadConfig->bMode_Control & 0x01) << 8) |
                  (ptPadConfig->bCurrentValue & 0x0f);
        rtdf_outl(PAD0_0x2640 + (count << 2), getVal);
    }

    getVal = 	(((UINT32)(lsadcConfig.selWait & 0x0F) << 28) |
               ((UINT32)(lsadcConfig.selClk & 0x0F) << 24)	|
               ((UINT32)(lsadcConfig.debounce & 0x0F) << 20)) | _BIT0;

    rtd_outl(PAD_CTRL_0x2660, getVal);

    //power on
    rtdf_maskl(PAD_ANALOG_CTRL_0x2668, 0xFFFFFFFF, 0x00000101);
}

void drv_lsadc_PowerOn(bit on_off)
{
    if (on_off)  		// power on
    {
        //power on
        rtdf_setBits(PAD_CTRL_0x2660, _BIT0);
        rtdf_setBits(PAD_ANALOG_CTRL_0x2668, _BIT0);
    }
    else  			// power off
    {
        //power off
        rtdf_clearBits(PAD_CTRL_0x2660, _BIT0);
        rtdf_clearBits(PAD_ANALOG_CTRL_0x2668, _BIT0);
    }
}

/*============================================================================*/
/**
 * drv_lsadcValRead
 * Read value from ADC
 *
 * @param <pad> 		{ Pad to Read value from 0~7 }
 * @param <buf>		{ Read value in data buffer }
 * @return <bit>		{ 0: failure; 1:success }
 *
 */
bit drv_lsadcValRead(UINT8 pad, UINT8 * buf)
{
    if (pad > 7 || buf == NULL)
        return _FALSE;

    *buf= (rtd_inl(PAD0_0x2640 + pad * 4) & 0xff);

    return _TRUE;
}

bit drv_lsadcValRead_isr(UINT8 pad, UINT8 * buf)
{
    if (pad > 7 || buf == NULL)
        return _FALSE;

    *buf= (rtd_isr_inl(PAD0_0x2640 + pad * 4) & 0xff);

    return _TRUE;
}



