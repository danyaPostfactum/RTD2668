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


#include "pcb_parameter_def.h"
#include "i2c\i2c.h"
#include "ifd\ifd.h"
#include "tuner\tuner.h"
#include "video\video.h"
#include "flow_struct.h"
#include "scaler_idomain\syncproc.h"
#include "scaler_idomain\measure.h"
#include "reg_def.h"
#include "flow_scaler.h"
#include "rosprintf.h"
#include "timer_event.h"
#include "mode.h"
#include "mode_customer.h"
#include "pcb_customer.h"

#include "ifd/ifd_factorymode.h"

#ifdef CONFIG_TUNER_DEFAULT
extern StructTunerDataType IFD_XDATA gTunerData[_Tuner_MAX_NO];

UINT8 xdata g_curr_tunerband;
UINT32 xdata g_curr_freq;

/*=============================================================*/
/**
	This routine will set init.
	@param
	@return
*/

void CTunerGlobalVarInit(void)
{
    g_curr_freq=0;
}

/*=============================================================*/
/**
	This routine will set TV to specific frequency.
	@param			freq					frequency
	@return 			Return the status code	_TURE,_FALSE
*/
UINT8 drv_tuner_SetFreq(UINT32 freq)
{
    UINT8 setting[6], i;
    UINT8 tunerBANDno;
    UINT8 tunerRegno;
    UINT32 divider_freq;

    divider_freq = drv_tuner_GeDiverRatio(freq);
    tunerBANDno = gTunerData[0].tunerBandNo;
    tunerRegno = gTunerData[0].tunerRegNoPerBand;

    g_curr_tunerband = drv_tuner_CheckBand(freq);
    g_curr_freq= freq;

    setting[0] = (UINT8)((divider_freq>>8)& 0xff);
    setting[1] = (UINT8)(divider_freq & 0xff);
    for (i=0; i<tunerRegno; i++)
        setting[i+2] = gTunerData[0].tunerRegisterByte[i+tunerRegno*drv_tuner_CheckBand(freq)];

    drv_i2c_Set_TargetAddr(I2C_MASTER_TUNER, (gTunerData[0].tunerI2C_ADD>>1));
    drv_i2c_Write(I2C_MASTER_TUNER, tunerRegno+2 ,setting);
#ifdef _VIDEO_TV_SUPPORT
#ifndef CONFIG_TUNER_SILICON
    drvif_ifd_vtop_change_with_tunerband();
#endif
#endif
    return _TRUE;

}
/*=============================================================*/
/**
	This routine will set TV to specific frequency  in Search mode
	@param			freq					frequency
	@return 			Return the status code	_TURE,_FALSE
*/
UINT8 CTunerSetFreqSearch(UINT32 freq)
{
    UINT8 setting[6], i;
    UINT8 tunerBANDno;
    UINT8 tunerRegno;
    UINT32 divider_freq;

    divider_freq = drv_tuner_GeDiverRatio(freq);
    tunerBANDno = gTunerData[0].tunerBandNo;
    tunerRegno = gTunerData[0].tunerRegNoPerBand;

    g_curr_tunerband = drv_tuner_CheckBand(freq);
    g_curr_freq= freq;

    setting[0] = (UINT8)((divider_freq>>8)& 0xff);
    setting[1] = (UINT8)(divider_freq & 0xff);
    for (i=0; i<tunerRegno; i++)
        setting[i+2] = gTunerData[0].tunerRegisterByteSearch[i+tunerRegno*drv_tuner_CheckBand(freq)];

    drv_i2c_Set_TargetAddr(I2C_MASTER_TUNER, (gTunerData[0].tunerI2C_ADD>>1));
    drv_i2c_Write(I2C_MASTER_TUNER, tunerRegno+2 ,setting);
#ifdef _VIDEO_TV_SUPPORT
#ifndef CONFIG_TUNER_SILICON
    drvif_ifd_vtop_change_with_tunerband();
#endif
#endif
    return _TRUE;
}
/*=============================================================*/
/**
	This routine will get TV to current frequency.
	@param			void
	@return 			Return the freq	frequency
*/

UINT32 CTunerGetFreq(void)
{
    return g_curr_freq;
}
/*=============================================================*/
/**
	This routine will get divider ratio for specific frequency.
	@param			freq					frequency
	@return 			Return the status code	_TURE,_FALSE
*/
UINT16 drv_tuner_GeDiverRatio(UINT32 freq)
{
	/*UINT32 TEMP1,TEMP2,TEMP3,TEMP4,TEMP5;
	TEMP4 = freq;
	TEMP5 = gTunerData[0].tunerPIF;
	TEMP1 = (freq + gTunerData[0].tunerPIF);
	TEMP2 = gTunerData[0].tunerRATIOStepSize;
	TEMP3 = TEMP1 / TEMP2;*/
	
	//RTD_Log(LOGGER_LEVEL,"\n Tim freq = %d,tunerPIF = %d,StepSize = %d,temp1 = %d ,temp2 = %d,temp3 = %d,temp4 = %d,temp5 = %d\n",(UINT32)freq,(UINT32)gTunerData[0].tunerPIF,(UINT32)gTunerData[0].tunerRATIOStepSize,(UINT32)TEMP1,(UINT32)TEMP2,(UINT32)TEMP3,(UINT32)TEMP4,(UINT32)TEMP5);
    return (freq + gTunerData[0].tunerPIF) / gTunerData[0].tunerRATIOStepSize;
}
/*=============================================================*/
/**
	This routine will check tuner receiving frequency band(VHF Low¡BVHF High¡BUHF).
	@param 			freq	       					tuner receiving frequency(divider)
	@return 			Return tuner receiving frequency band(01¡B02 or 04)
*/
UINT8 drv_tuner_CheckBand(UINT32 freq)
{
    UINT8 i;
    UINT8 tunerBANDno;

    tunerBANDno = gTunerData[0].tunerBandNo;
    for (i=0; i<tunerBANDno; )
    {
        if (freq <= gTunerData[0].tunerBandFreq[i])
            return (i);
        i++;
    }
    return 0x00;
}
/*=============================================================*/
UINT32 drv_tuner_GetRatioInStepSize(void)
{
    return gTunerData[0].tunerRATIOStepSize;
}
/*=============================================================*/
UINT32 drv_tuner_GetChanelMaxFreq(void)
{
    return  gTunerData[0].tuner_MaxFreq;
}
/*=============================================================*/
UINT32 drv_tuner_GetChanelMinFreq(void)
{
    return  gTunerData[0].tunerMinFreq;
}
/*=============================================================*/
UINT32 drv_tuner_GetPictureInterFreq(void)
{
    return gTunerData[0].tunerPIF;
}
void drv_tuner_TunerAudioStdInit(UINT8 mode)
{
    if (mode==_SIG_MODE_AUDIO_MN)
        drvif_ifd_set_audio_bp(_SIG_MODE_NTSC);
    else
        drvif_ifd_set_audio_bp(_SIG_MODE_PAL);
}

/*=============================================================*/
// ISR
/*=============================================================*/
#if 0
void drv_tuner_isr_TunerAudioStdInit(UINT8 mode)
{
    drv_ifd_isr_Set_Audio_BP(mode);
}
#endif

#ifdef CONFIG_IFD_TUNER_AUTO
/*=============================================================*/
/**
	This routine will set TV to specific frequency.
	@param			freq					frequency
	@return 			Return the status code	_TURE,_FALSE
*/
UINT8 drv_tuner_isr_SetFreq(UINT32 freq)
{
    UINT8 setting[6], i;
    UINT8 tunerBANDno;
    UINT8 tunerRegno;
    UINT32 divider_freq;

    divider_freq = (freq + gTunerData[0].tunerPIF) / gTunerData[0].tunerRATIOStepSize;
    tunerBANDno = gTunerData[0].tunerBandNo;
    tunerRegno = gTunerData[0].tunerRegNoPerBand;

    for (i=0; i<tunerBANDno; )
    {
        if (freq <= gTunerData[0].tunerBandFreq[i])
        {
            g_curr_tunerband = i;
            i=tunerBANDno;
            break;
        }
        i++;
    }

    g_curr_freq= freq;

    setting[0] = (UINT8)((divider_freq>>8)& 0xff);
    setting[1] = (UINT8)(divider_freq & 0xff);
    for (i=0; i<tunerRegno; i++)
        setting[i+2] = gTunerData[0].tunerRegisterByte[i+tunerRegno*g_curr_tunerband];

    drv_i2c_isr_Set_TargetAddr(I2C_MASTER_TUNER, (gTunerData[0].tunerI2C_ADD>>1));
    drv_i2c_isr_Write(I2C_MASTER_TUNER, tunerRegno+2 ,setting);

    return _TRUE;

}
#endif
#endif
