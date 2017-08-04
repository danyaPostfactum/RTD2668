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

#ifdef CONFIG_TUNER_TDQ6K

#define _TUNER_WRITE_ADDRESS		0xc0


#define	_TUNER_MIN_FREQ								(440000)//(7000000)//(8500000)//  //irene 497500
#define	_TUNER_MAX_FREQ								(8640000)
#define	_TUNER_PICTURE_INTERMEDIATE_FREQ   			(380000)
#define	_TUNER_VHF_LOW_FREQ							(1442500)
#define 	_TUNER_VHF_HIGH_FREQ   						(4322500)

// Unit: 10Hz
#define	_TUNER_RATIO_IN_STEP_SIZE					(625)//(625)

/**
	This routine will get divider ratio for specific frequency.
	@param freq	frequency
	@return 			Return the status code  _TURE,_FALSE
*/
UINT16 drv_tuner_GeDiverRatio(UINT32 freq)
{
    return (freq + _TUNER_PICTURE_INTERMEDIATE_FREQ) / _TUNER_RATIO_IN_STEP_SIZE;
}

UINT32 drv_tuner_GetRatioInStepSize(void)
{
    return _TUNER_RATIO_IN_STEP_SIZE;
}



void drv_tuner_TunerAudioStdInit(UINT8 mode)
{
    drv_ifd_Set_Audio_BP(mode);
}



/**
	This routine will set TV to specific frequency.
	@param freq	frequency
	@return 			Return the status code  _TURE,_FALSE
*/
UINT8 drv_tuner_SetFreq(UINT32 freq)
{
    UINT8 setting[6];

    //freq = 552500;

//	RTD_Log(LOGGER_DEBUG, "tuner set frq %x\n", freq);

    freq = drv_tuner_GeDiverRatio(freq);
//	setting[0] = _TUNER_WRITE_ADDRESS;
#if 1
    setting[0] = *((UINT8*)&freq + 2);
    setting[1] = *((UINT8*)&freq + 3);
#else
//	setting[0] = (UINT8)((freq>>8)& 0xff);
//	setting[1] = (UINT8)(freq & 0xff);
#endif
    if (_TUNER_RATIO_IN_STEP_SIZE == 500)	// 50kHz
        setting[2] = 0xc8;
    else if (_TUNER_RATIO_IN_STEP_SIZE == 312) // 31.25kHz
        setting[2] = 0xca;
    else	// 62.5kHz
        setting[2] = 0xce ;					//RSA=1,RSB=1(62.5khz)   //for W116H
    setting[3] =(drv_tuner_GeDiverRatio(_TUNER_VHF_LOW_FREQ) > freq) ?0x01 :(drv_tuner_GeDiverRatio(_TUNER_VHF_HIGH_FREQ) > freq) ?0x02 : 0x08;// reynold modified 04 to 08 , for high bend scan lost, 070804
//	setting[5] = 0x00;
    drv_i2c_Set_TargetAddr(I2C_MASTER_TUNER, I2C_MASTER_TUNER_ADDR);
    drv_i2c_Write(I2C_MASTER_TUNER, 4,setting);

    // Wait signal to ready
    //fw_timer_event_DelayXms(25);//?????
    return _TRUE;
}

UINT8 drv_tuner_CheckBand(UINT32 freq)
{
    UINT8 TunerBand;
    TunerBand=(_TUNER_VHF_LOW_FREQ > freq) ?0x01 :(_TUNER_VHF_HIGH_FREQ > freq) ?0x02 :	0x08;

    return TunerBand;
}

UINT32 drv_tuner_GetChanelMaxFreq(void)
{
    return _TUNER_MAX_FREQ;
}

UINT32 drv_tuner_GetChanelMinFreq(void)
{
    return _TUNER_MIN_FREQ;
}


UINT32 drv_tuner_GetPictureInterFreq(void)
{
    return _TUNER_PICTURE_INTERMEDIATE_FREQ;
}

#if 0	// not used
UINT16 drv_tuner_isr_GeDiverRatio(UINT32 freq)
{
    return (freq + _TUNER_PICTURE_INTERMEDIATE_FREQ) / _TUNER_RATIO_IN_STEP_SIZE;
}

void drv_tuner_isr_TunerAudioStdInit(UINT8 mode)
{
    drv_ifd_isr_Set_Audio_BP(mode);
}


UINT8 drv_tuner_isr_SetFreq(UINT32 freq)
{
    UINT8 setting[6];

    //freq = 552500;

//	RTD_Log(LOGGER_DEBUG, "tuner set frq %x\n", freq);

    freq = drv_tuner_isr_GeDiverRatio(freq);
//	setting[0] = _TUNER_WRITE_ADDRESS;
#if 1
    setting[0] = *((UINT8*)&freq + 2);
    setting[1] = *((UINT8*)&freq + 3);
#else
//	setting[0] = (UINT8)((freq>>8)& 0xff);
//	setting[1] = (UINT8)(freq & 0xff);
#endif
    if (_TUNER_RATIO_IN_STEP_SIZE == 500)	// 50kHz
        setting[2] = 0xc8;
    else if (_TUNER_RATIO_IN_STEP_SIZE == 312) // 31.25kHz
        setting[2] = 0xca;
    else	// 62.5kHz
        setting[2] = 0xce ;					//RSA=1,RSB=1(62.5khz)   //for W116H
    setting[3] =(drv_tuner_isr_GeDiverRatio(_TUNER_VHF_LOW_FREQ) > freq) ?0x01 :(drv_tuner_isr_GeDiverRatio(_TUNER_VHF_HIGH_FREQ) > freq) ?0x02 : 0x08;// reynold modified 04 to 08 , for high bend scan lost, 070804
//	setting[5] = 0x00;
    drv_i2c_Set_TargetAddr(I2C_MASTER_TUNER, I2C_MASTER_TUNER_ADDR);
    drv_i2c_Write(I2C_MASTER_TUNER, 4,setting);

    // Wait signal to ready
    //fw_timer_event_DelayXms(25);//?????
    return _TRUE;
}
#endif
#endif

