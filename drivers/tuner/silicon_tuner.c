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
#include "sparrow\spi_dma.h"

#ifdef CONFIG_TUNER_SILICON
#ifdef _SILICON_JIE_CODE

#ifdef CONFIG_TUNER_SILICON_DEBUG_MSG
#define SILICON_DEBUG_MSG RTD_Log
#else
#define SILICON_DEBUG_MSG //
#endif

#define	_TUNER_MIN_FREQ								(450000)
#define	_TUNER_MAX_FREQ								(9000000)
#define	_TUNER_VHF_LOW_FREQ							(1442500)
#define 	_TUNER_VHF_HIGH_FREQ   						(4322500)

// Unit: 10Hz
#define	_TUNER_RATIO_IN_STEP_SIZE					(625)	//(625)
#define	_TUNER_PICTURE_INTERMEDIATE_FREQ   			(78200)//(77500)
#define ANALOG


//Version 2.07
//----------------------------------------------------------//
//                   RT610 Protct Parameter                 //
//----------------------------------------------------------//
#define RingCal	_TRUE
#define LNACal	_FALSE
#define UInt8      UINT8
#define UInt32     UINT32
#define true _TRUE
#define false _FALSE
UINT8 xdata g_curr_tunerband;
UINT32 xdata g_curr_freq;
static UINT32 ChannelIF;
RT610_Set_Info RT610_Data;

UINT8  RT610_Arry[50] = {0xDB, 0x04, 0x92, 0x05, 0x60, 0x06, 0x1F, 0x07, 0x34, 0x08,
	                      //0x03      0x04        0x05        0x06        0x07      
	                      0x40, 0x09, 0x40, 0x0A, 0x4E, 0x0B, 0x02, 0x0C, 0x90, 0x0D,
	                      //0x08      0x09        0x0A        0x0B        0x0C       
	                      0x82, 0x0E, 0x63, 0x0F, 0x88, 0x10, 0x35, 0x11, 0x13, 0x12,
	                      //0x0D      0x0E        0x0F        0x10        0x11      
	                      0x48, 0x13, 0x00, 0x14, 0xC8, 0x15, 0x00, 0x16, 0x00, 0x17,
	                      //0x12      0x13        0x14        0x15        0x16      
	                      0x07, 0x18, 0xC0, 0x19, 0x40, 0x1A, 0x10, 0x1B, 0x00};
	                      //0x17      0x18        0x19        0x1A        0x1B 

typedef struct _RT610_SectType
{
	UInt32 SecFreq;
	UInt32 IMR_Freq;
	UInt8 Phase_Y;
	UInt8 Gain_X;
	UInt8 Value;
}RT610_SectType;
                
typedef struct _I2C_LEN_TYPE
{
	UINT8 RegAddr;
	UINT8 Data[50];
	UINT8 Len;
}I2C_LEN_TYPE;

typedef struct _I2C_TYPE
{
	UINT8 RegAddr;
	UINT8 Data;
	UINT8 Len;
}I2C_TYPE;
//----------------------------------------------------------//
//                   Internal Parameter                     //
//----------------------------------------------------------//
//Channel
#define Channel_Link_Arry	2
typedef struct _RT610_Channel_Link_Type
{
	RT610_Standard_Type RT610_Channel_Standard;
	RT610_IF_Type       RT610_Channel_IF;
	RT610_BW_Type       RT610_Channel_BW;
	UInt32              RT610_Channel_Freq;
}RT610_Channel_Link_Type;
RT610_Channel_Link_Type RT610_Channel_Link_Info[Channel_Link_Arry];

//MUX & VTL & VTH
#define Gain_Set_Arry 4
typedef struct _RT610_Gain_Link_Type
{
	RT610_Standard_Type RT610_Gain_Standard;
	RT610_IF_Type       RT610_Channel_IF;
	RT610_BW_Type       RT610_Channel_BW;
	UInt32              RT610_Gain_Freq;
	UInt32				RT610_Gain_Frey;
	UInt8               RT610_Gain_Reg;
	UInt8               RT610_Set_Mask;
	UInt8               RT610_Set_Mix_Data;
}RT610_Gain_Link_Type;
RT610_Gain_Link_Type RT610_Gain_Link_Info[Gain_Set_Arry];

//HPF & Manual Gain
#define Channel_Control_Array	6
typedef struct _RT610_Channel_Control_Type
{
	RT610_Standard_Type RT610_Control_Standard;
	RT610_IF_Type       RT610_Channel_IF;
	RT610_BW_Type       RT610_Control_BW;
	UInt8               RT610_Control_HP;
	UInt8               RT610_Control_Gain;
}RT610_Channel_Control_Type;
RT610_Channel_Control_Type RT610_Channel_Control[Channel_Control_Array];

//----------------------------------------------------------//
//                   Standard Define                        //
//----------------------------------------------------------//
#ifdef ANALOG
UInt8 Analog_Arry[50] = {0x03, 0xDB, 0x04, 0x92, 0x05, 0x40, 0x06, 0x1F, 0x07, 0x34,
                             //0x03        0x04        0x05        0x06        0x07
						 0x08, 0x40, 0x09, 0x40, 0x0A, 0x4E, 0x0B, 0x02, 0x0C, 0x90,
						     //0x08        0x09        0x0A        0x0B        0x0C
						 0x0D, 0x82, 0x0E, 0x73, 0x0F, 0x88, 0x10, 0x15, 0x11, 0x13,
						     //0x0D        0x0E        0x0F        0x10        0x11
						 0x12, 0x40, 0x13, 0x00, 0x14, 0x49, 0x15, 0x1E, 0x16, 0x49,
						     //0x12        0x13        0x14        0x15        0x16
						 0x17, 0x0E, 0x18, 0xC1, 0x19, 0x40, 0x1A, 0x10, 0x1B, 0x00};
                             //0x17        0x18        0x19        0x1A        0x1B
#endif

/***********************************************************/
I2C_TYPE  RT610_I2C;
I2C_LEN_TYPE RT610_I2C_Len;

bit RT610_I2C_Write_Len(I2C_LEN_TYPE *I2C_Info);
bit RT610_I2C_Read_Len(I2C_LEN_TYPE *I2C_Info);
bit RT610_I2C_Write(I2C_TYPE *I2C_Info);
UInt8 RT610_Convert(UInt8 InvertNum);

RT610_SectType IMR_Data[6] = {{0, 0, 0, 0, 0},
	                            {0, 0, 0, 0, 0},
	                            {0, 0, 0, 0, 0},
	                            {0, 0, 0, 0, 0},
	                            {0, 0, 0, 0, 0},
	                            {0, 0, 0, 0, 0}};//Please keep this array data for standby mode.

RT610_ErrCode RT610_Nine_IQ(RT610_SectType* N_IQ_Pont, UInt8 N_AryNum);
RT610_ErrCode RT610_InitReg(UInt32 RF_KHz, UInt32 IF_KHz);
RT610_ErrCode RT610_MUX(UInt32 RF_KHz, UInt32 IF_KHz, BOOL RingFlg);
RT610_ErrCode RT610_PLL(UInt32 LO_Freq);
RT610_ErrCode RT610_IMR(UInt32 RF_KHz, UInt32 IF_KHz);
RT610_ErrCode RT610_Muti_Read( UInt8* IMR_Data);
RT610_ErrCode RT610_IQ(RT610_SectType* IQ_Pont);
RT610_ErrCode RT610_IQ_Tree(UInt8 FixPot, UInt8 FlucPot, UInt8 PotReg, RT610_SectType* CompareTree);
RT610_ErrCode RT610_CompreCor(RT610_SectType* CorArry);
RT610_ErrCode RT610_CompreStep(RT610_SectType* StepArry, UInt8 Pace);
RT610_ErrCode RT610_Section(RT610_SectType* SectionArry);
RT610_ErrCode RT610_ChannelFilter(RT610_BW_Type ChannelBW, RT610_Standard_Type ChannelStandard, RT610_IF_Type RT8610_IF);

RT610_ErrCode RT610_Standby(RT610_LoopThrough_Type RT610_LoopSwitch);

RT610_ErrCode RT610_Standard(UInt8* StandardArry, RT610_BW_Type ChannelBW, RT610_Standard_Type RT_Standard,
							 RT610_IF_Type RT610_IF);
RT610_ErrCode RT610_SetFrequency(RT610_Set_Info RT610_INFO);

//----------------------------------------------------------//
//                   Internal Function                      //
//----------------------------------------------------------//
void RT610_Channel_Link();
void RT610_Gain_Link();
void RT610_Channel_Control_Link();
RT610_ErrCode RT610_RF_MUX(RT610_Set_Info RT610_INFO);
/************************************************************/
void RT610_Channel_Link()
{
	RT610_Channel_Link_Info[0].RT610_Channel_Standard = NTSC_M;
	RT610_Channel_Link_Info[0].RT610_Channel_BW       = BW_6MHz;
	RT610_Channel_Link_Info[0].RT610_Channel_IF       = NTSC_5820;
	RT610_Channel_Link_Info[0].RT610_Channel_Freq     = 70000;

	RT610_Channel_Link_Info[1].RT610_Channel_Standard = PAL_B;
	RT610_Channel_Link_Info[1].RT610_Channel_BW       = BW_7MHz;
	RT610_Channel_Link_Info[1].RT610_Channel_IF       = PAL_B_6820;
	RT610_Channel_Link_Info[1].RT610_Channel_Freq     = 74000;
}

void RT610_Gain_Link()
{
	RT610_Gain_Link_Info[0].RT610_Gain_Standard = NTSC_M;
	RT610_Gain_Link_Info[0].RT610_Channel_BW    = BW_6MHz;
	RT610_Gain_Link_Info[0].RT610_Channel_IF    = NTSC_5820;
	RT610_Gain_Link_Info[0].RT610_Gain_Freq     = 900000;
	RT610_Gain_Link_Info[0].RT610_Gain_Frey     = 0;
	RT610_Gain_Link_Info[0].RT610_Gain_Reg      = 0x05;
	RT610_Gain_Link_Info[0].RT610_Set_Mask      = 0xFF;
	RT610_Gain_Link_Info[0].RT610_Set_Mix_Data  = 0xA0;

	RT610_Gain_Link_Info[1].RT610_Gain_Standard = ATV;
	RT610_Gain_Link_Info[1].RT610_Channel_BW    = BW_8MHz;
	RT610_Gain_Link_Info[1].RT610_Channel_IF    = NTSC_5820;
	RT610_Gain_Link_Info[1].RT610_Gain_Freq     = 400000;
	RT610_Gain_Link_Info[1].RT610_Gain_Frey     = 0;
	RT610_Gain_Link_Info[1].RT610_Gain_Reg      = 0x04;
	RT610_Gain_Link_Info[1].RT610_Set_Mask      = 0xF8;
	RT610_Gain_Link_Info[1].RT610_Set_Mix_Data  = 0x02;

	RT610_Gain_Link_Info[2].RT610_Gain_Standard = ATV;
	RT610_Gain_Link_Info[2].RT610_Channel_BW    = BW_8MHz;
	RT610_Gain_Link_Info[2].RT610_Channel_IF    = NTSC_5820;
	RT610_Gain_Link_Info[2].RT610_Gain_Freq     = 700000;
	RT610_Gain_Link_Info[2].RT610_Gain_Frey     = 400000;
	RT610_Gain_Link_Info[2].RT610_Gain_Reg      = 0x04;
	RT610_Gain_Link_Info[2].RT610_Set_Mask      = 0xF8;
	RT610_Gain_Link_Info[2].RT610_Set_Mix_Data  = 0x01;

	RT610_Gain_Link_Info[3].RT610_Gain_Standard = ATV;
	RT610_Gain_Link_Info[3].RT610_Channel_BW    = BW_8MHz;
	RT610_Gain_Link_Info[3].RT610_Channel_IF    = NTSC_5820;
	RT610_Gain_Link_Info[3].RT610_Gain_Freq     = 900000;
	RT610_Gain_Link_Info[3].RT610_Gain_Frey     = 700000;
	RT610_Gain_Link_Info[3].RT610_Gain_Reg      = 0x04;
	RT610_Gain_Link_Info[3].RT610_Set_Mask      = 0xF8;
	RT610_Gain_Link_Info[3].RT610_Set_Mix_Data  = 0x00;
}

void RT610_Channel_Control_Link()
{
	RT610_Channel_Control[0].RT610_Control_Standard = NTSC_M;
	RT610_Channel_Control[0].RT610_Channel_IF       = NTSC_5820;
	RT610_Channel_Control[0].RT610_Control_BW       = BW_6MHz;
	RT610_Channel_Control[0].RT610_Control_HP       = 0x10;
	RT610_Channel_Control[0].RT610_Control_Gain     = 0x40;

	RT610_Channel_Control[1].RT610_Control_Standard = PAL_G;
	RT610_Channel_Control[1].RT610_Channel_IF       = PAL_G_7820;
	RT610_Channel_Control[1].RT610_Control_BW       = BW_8MHz;
	RT610_Channel_Control[1].RT610_Control_HP       = 0x10;
	RT610_Channel_Control[1].RT610_Control_Gain     = 0x40;

	RT610_Channel_Control[2].RT610_Control_Standard = PAL_I;
	RT610_Channel_Control[2].RT610_Channel_IF       = PAL_I_7820;
	RT610_Channel_Control[2].RT610_Control_BW       = BW_8MHz;
	RT610_Channel_Control[2].RT610_Control_HP       = 0x10;
	RT610_Channel_Control[2].RT610_Control_Gain     = 0x40;

	RT610_Channel_Control[3].RT610_Control_Standard = PAL_D;
	RT610_Channel_Control[3].RT610_Channel_IF       = PAL_D_7820;
	RT610_Channel_Control[3].RT610_Control_BW       = BW_8MHz;
	RT610_Channel_Control[3].RT610_Control_HP       = 0x10;
	RT610_Channel_Control[3].RT610_Control_Gain     = 0x40;

	RT610_Channel_Control[4].RT610_Control_Standard = PAL_B;
	RT610_Channel_Control[4].RT610_Channel_IF       = PAL_B_6820;
	RT610_Channel_Control[4].RT610_Control_BW       = BW_7MHz;
	RT610_Channel_Control[4].RT610_Control_HP       = 0x10;
	RT610_Channel_Control[4].RT610_Control_Gain     = 0x40;

    RT610_Channel_Control[5].RT610_Control_Standard = SECAM_L;
	RT610_Channel_Control[5].RT610_Channel_IF       = SECAM_L_7820;
	RT610_Channel_Control[5].RT610_Control_BW       = BW_8MHz;
	RT610_Channel_Control[5].RT610_Control_HP       = 0x10;
	RT610_Channel_Control[5].RT610_Control_Gain     = 0x40;
}

RT610_ErrCode RT610_RF_MUX(RT610_Set_Info RT610_INFO)
{
	UInt8 GainCunt = 0;
	UInt8 RT610_Arry_Num = 0;

	for(GainCunt = 0;GainCunt < Gain_Set_Arry;GainCunt ++)
	{
		if((RT610_INFO.RT610_Standard == RT610_Gain_Link_Info[GainCunt].RT610_Gain_Standard) && 
			(RT610_INFO.RT610_BW == RT610_Gain_Link_Info[GainCunt].RT610_Channel_BW) && 
			(RT610_INFO.IF_KHz == RT610_Gain_Link_Info[GainCunt].RT610_Channel_IF))
		{
			RT610_Arry_Num = (RT610_Gain_Link_Info[GainCunt].RT610_Gain_Reg - 3) * 2;
			RT610_Arry[RT610_Arry_Num] &= RT610_Gain_Link_Info[GainCunt].RT610_Set_Mask;
			RT610_I2C.RegAddr = RT610_Gain_Link_Info[GainCunt].RT610_Gain_Reg;

			if((RT610_INFO.RF_KHz < RT610_Gain_Link_Info[GainCunt].RT610_Gain_Freq) && 
				(RT610_INFO.RF_KHz >= RT610_Gain_Link_Info[GainCunt].RT610_Gain_Frey))
			{
				RT610_Arry[RT610_Arry_Num] |= RT610_Gain_Link_Info[GainCunt].RT610_Set_Mix_Data;
				RT610_I2C.Data    = RT610_Arry[RT610_Arry_Num];
				if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
					return RT_Fail;
			}
		}

		if((RT610_INFO.RT610_Standard == NTSC_M) || (RT610_INFO.RT610_Standard == PAL_G) ||
			(RT610_INFO.RT610_Standard == PAL_I) || (RT610_INFO.RT610_Standard == PAL_D) ||
			(RT610_INFO.RT610_Standard == PAL_B) || (RT610_INFO.RT610_Standard == SECAM_L))
		{
			if(RT610_Gain_Link_Info[GainCunt].RT610_Gain_Standard == ATV)
			{
				RT610_Arry_Num = (RT610_Gain_Link_Info[GainCunt].RT610_Gain_Reg - 3) * 2;
				RT610_Arry[RT610_Arry_Num] &= RT610_Gain_Link_Info[GainCunt].RT610_Set_Mask;
				RT610_I2C.RegAddr = RT610_Gain_Link_Info[GainCunt].RT610_Gain_Reg;

				if((RT610_INFO.RF_KHz < RT610_Gain_Link_Info[GainCunt].RT610_Gain_Freq) && 
					(RT610_INFO.RF_KHz >= RT610_Gain_Link_Info[GainCunt].RT610_Gain_Frey))
				{
					RT610_Arry[RT610_Arry_Num] |= RT610_Gain_Link_Info[GainCunt].RT610_Set_Mix_Data;
					RT610_I2C.Data    = RT610_Arry[RT610_Arry_Num];
					if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
						return RT_Fail;
				}
			}
		}
	}

	return RT_Success;
}

RT610_ErrCode RT610_MUX(UInt32 RF_KHz, UInt32 IF_KHz, BOOL RingFlg)
{
	UInt32 LO_KHz = 0;
		
	//Read Register 0x00 ~ 0x03
	RT610_I2C_Len.RegAddr = 0x00;
	RT610_I2C_Len.Len     = 3;
	if(RT610_I2C_Read_Len(&RT610_I2C_Len) != RT_Success)
	{
              SILICON_DEBUG_MSG(LOGGER_INFO, "######RT610_Read Table Fail!!!!!!#################\n");
		return RT_Fail;
	}
	 //SILICON_DEBUG_MSG(LOGGER_INFO, "######RT610_Read Table RT610_I2C_Len.Data[2] = %x!!!!!!#################\n", (UINT8)RT610_I2C_Len.Data[0]);

	//HPF & LPF
	if(~(RT610_I2C_Len.Data[2]) & 0x20)
	{
		RT610_I2C.RegAddr = 0x17;
		RT610_Arry[40] &= 0xFC;
		if((48000 < RF_KHz) && (340000 >= RF_KHz))
			RT610_Arry[40] = (RT610_Arry[40] & 0xF3) | 0x03;
		else if((340000 < RF_KHz) && (440000 >= RF_KHz))
			RT610_Arry[40] = (RT610_Arry[40] & 0xF7) | 0x07;
		else if((440000 < RF_KHz) && (520000 >= RF_KHz))
			RT610_Arry[40] = (RT610_Arry[40] & 0xFC) | 0x0C;
		else if((520000 < RF_KHz) && (700000 >= RF_KHz))
			RT610_Arry[40] = (RT610_Arry[40] & 0xFD) | 0x0D;
		else
			RT610_Arry[40] = (RT610_Arry[40] & 0xFE) | 0x0E;

		RT610_I2C.Data = RT610_Arry[40];
		if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		{
                     SILICON_DEBUG_MSG(LOGGER_INFO, "######RT610_H&L PF Fail!!!!!!#################\n");
			return RT_Fail;
		}
	}
	
	//TF Band
	RT610_I2C.RegAddr = 0x05;
	RT610_Arry[4] &= 0xBF;
	if(95000 >= RF_KHz)
		RT610_Arry[4] |= 0x1F;
	else if((95000 < RF_KHz) && (140000 >= RF_KHz))
		RT610_Arry[4] = (RT610_Arry[4] & 0xEC) | 0x0C;
	else if((140000 < RF_KHz) && (190000 >= RF_KHz))
		RT610_Arry[4] = (RT610_Arry[4] & 0xE5) | 0x05;
	else if((190000 < RF_KHz) && (280000 >= RF_KHz))
		RT610_Arry[4] &= 0xE0;
	else
		RT610_Arry[4] |= 0x40;

	if(RingFlg == RingCal)
	{
		RT610_Arry[4] &= 0x1F;
		if(RF_KHz > 280000)
			RT610_Arry[4] |= 0x40;
	}

	RT610_I2C.Data = RT610_Arry[4];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
	{
             SILICON_DEBUG_MSG(LOGGER_INFO, "######RT610_TF Fail!!!!!!#################\n");
		return RT_Fail;
	}

	//Polymux
	LO_KHz = RF_KHz + IF_KHz;
	RT610_I2C.RegAddr = 0x18;
	if(140000 >= LO_KHz)
		RT610_Arry[42] |= 0x0C;
	else if((140000 < LO_KHz) && (300000 >= LO_KHz))
		RT610_Arry[42] = (RT610_Arry[42] & 0xFB) | 0x08;
	else if((300000 < LO_KHz) && (588000 >= LO_KHz))
		RT610_Arry[42] = (RT610_Arry[42] & 0xF7) | 0x04;
	else
		RT610_Arry[42] &= 0xF3;

	if(RingFlg == RingCal)
	{
		RT610_Arry[42] &= 0x3F;
		if(480 <= LO_KHz)
			RT610_Arry[42] |= 0xC0;
		else if((224000 <= LO_KHz) && (480000 > LO_KHz))
			RT610_Arry[42] |= 0x80;
		else if((96000 <= LO_KHz) && (224000 > LO_KHz))
			RT610_Arry[42] |= 0x40;
		else
			RT610_Arry[42] |= 0x00;
	}
	
	RT610_I2C.Data = RT610_Arry[42];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
	{
              SILICON_DEBUG_MSG(LOGGER_INFO, "######RT610_Polymux Fail!!!!!!#################\n");
		return RT_Fail;
	}
	
	return RT_Success;
}
/*=============================================================*/
/**
	This routine will set init.
	@param
	@return
*/
RT610_ErrCode RT610_Init(void)
{
	//Internal Information Link
	RT610_Channel_Link();
	RT610_Gain_Link();
	RT610_Channel_Control_Link();
	SILICON_DEBUG_MSG(LOGGER_INFO, "######RT610_Channel_Control_Link!---------------------------------start \n");
	if(RT610_InitReg(384000, 4570) != RT_Success)
		return RT_Fail;
		
	if(RT610_IMR(224000, 4570) != RT_Success)
		return RT_Fail;

	/*if(RT610_IMR(64000, 4570) != RT_Success)
		return RT_Fail;
		
	if(RT610_IMR(128000, 4570) != RT_Success)
		return RT_Fail;

	if(RT610_IMR(384000, 4570) != RT_Success)
		return RT_Fail;

	if(RT610_IMR(608000, 4570) != RT_Success)
		return RT_Fail;

	if(RT610_IMR(680000, 4570) != RT_Success)
		return RT_Fail;*/

	//if(RT610_InitReg(384000, 4570) != RT_Success)
	//	return RT_Fail;

	SILICON_DEBUG_MSG(LOGGER_INFO, "######RT610_Init!!!!!!#################\n");

	return RT_Success;
}

RT610_ErrCode RT610_InitReg(UInt32 RF_KHz, UInt32 IF_KHz)
{
	UInt8 InitArryCunt = 0;
	UInt8 InitArryNum  = 49;
	UInt32 LO_KHz      = 0;
	
	//Write Full Table
	RT610_I2C_Len.RegAddr = 0x03;
	RT610_I2C_Len.Len     = InitArryNum;
	for(InitArryCunt = 0;InitArryCunt < InitArryNum;InitArryCunt ++)
	{
		RT610_I2C_Len.Data[InitArryCunt] = RT610_Arry[InitArryCunt];
	}
	if(RT610_I2C_Write_Len(&RT610_I2C_Len) != RT_Success)
		return RT_Fail;
	 
	if(RT610_MUX(RF_KHz, IF_KHz, LNACal) != RT_Success)
		return RT_Fail;

	LO_KHz = RF_KHz + IF_KHz;
	if(RT610_PLL(LO_KHz) != RT_Success)
		return RT_Fail;
	
	return RT_Success;
}

RT610_ErrCode RT610_PLL(UInt32 LO_Freq)
{
	UInt8  MixDiv   = 2;
	UInt8  DivBuf   = 0;
	UInt8  Ni       = 0;
	UInt8  SDM_Cunt = 0;
	UInt8  SDM_B    = 0;
	UInt8  SDM      = 0;
	UInt8  DivNum   = 0;
	UInt8  Judge    = 0;
	UInt32 N        = 0;
	UInt32 Div_1    = 0;
	UInt32 Div_2    = 0;//NF_1
	UInt32 VCO_Min  = 1416000;
	UInt32 VCO_Freq = 0;
	UInt32 Bry_M    = 1;
	UInt32 Bry_L    = 0;
	UInt32 Si       = 0;
	
       //SILICON_DEBUG_MSG(LOGGER_INFO, "######RT610_PLL %d!!!!!!#################\n", LO_Freq);

	//Divider
	while(MixDiv <= 32)
	{
		if(((LO_Freq * MixDiv) >= VCO_Min) && ((LO_Freq * MixDiv) < (VCO_Min * 2)))
		{
			DivBuf = MixDiv;
			while(DivBuf > 1)
			{
				DivBuf /= 2;
				DivNum ++;
			}
			DivNum --;
			
			RT610_I2C.RegAddr = 0x10;
			RT610_Arry[26] &= 0x1F;
			RT610_Arry[26] |= (DivNum << 5);
			RT610_I2C.Data = RT610_Arry[26];
			if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
				return RT_Fail;
			
			break;
		}
		MixDiv *= 2;
	}

	//VCO
	/*LO_Freq *= 10;
	VCO_Freq = LO_Freq * MixDiv;
	Div_1    = VCO_Freq / RT610_Xtal;
	Div_2    = VCO_Freq - (Div_1 * RT610_Xtal);
	Div_2    = (Div_2 * 100000) / RT610_Xtal;
	N        = Div_1 / 2;

	if((Div_1 - (N * 2)) != 0)
		Div_2 += 100000;
	Div_2    /= 2;

	Ni       = (N - 13) / 4;
	Si       = ((N * 1000 - 13000) / 4) - (Ni * 1000);
	Si       = (Si * 4) / 1000;*/


	VCO_Freq = LO_Freq * MixDiv;
	Div_1    = VCO_Freq / RT610_Xtal;
	Div_2    = VCO_Freq - (Div_1 * RT610_Xtal);
	Div_2    = (Div_2 * 100000) / RT610_Xtal;
	N        = Div_1 / 2;

	if((Div_1 - (N * 2)) != 0)
		Div_2 += 100000;
	Div_2    /= 2;

	if(Div_2 < 781)
		Div_2 = 0;
	else if((Div_2 > 49609) && (Div_2 < 50000))
		Div_2 = 49609;
	else if((Div_2 >= 50000) && (Div_2 < 50390))
		Div_2 = 50390;
	else if((99218 < Div_2) && (100000))
	{
		Div_2 = 0;
		N ++;
	}

	Ni       = (UInt8)(N - 13) / 4;
	Si       = ((N * 1000 - 13000) / 4) - (Ni * 1000);
	Si       = (Si * 4) / 1000;
	RT610_I2C.RegAddr = 0x14;
	RT610_Arry[34]  = 0x00;
	RT610_Arry[34] |= (Ni + (Si << 6));
	RT610_I2C.Data = RT610_Arry[34];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;
	
	RT610_I2C.RegAddr = 0x12;
	RT610_Arry[30] &= 0xE7;
	if(Div_2 == 0)
		RT610_Arry[30] |= 0x08;

	RT610_I2C.Data = RT610_Arry[30];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	SDM    = 0;
	SDM_B  = 0x80;
	Bry_M  = 50000;
	Bry_L  = 0;

	if(Div_2 >= Bry_M)
	{
		SDM = SDM_B;
		Div_2 -= Bry_M;
	}
	SDM_B /= 2;
	Bry_M /= 2;

	for(SDM_Cunt = 0;SDM_Cunt < 14;SDM_Cunt ++)
	{
		if(Div_2 >= Bry_M)
		{
			SDM += SDM_B;
			Div_2 -= Bry_M;
		}
		
		if(SDM_Cunt == 6)
		{
			RT610_I2C.RegAddr = 0x16;
			RT610_Arry[38]    = SDM;
			RT610_I2C.Data    = RT610_Arry[38];
			if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
				return RT_Fail;
			
			SDM_B = 0x80;
			SDM   = 0x00;
			Bry_M /= 2;
			if(Div_2 > Bry_M)
			{
				SDM += SDM_B;
				Div_2 -= Bry_M;
			}
		}

		SDM_B /= 2;
		Bry_M /= 2;
	}

	RT610_I2C.RegAddr = 0x15;
	RT610_Arry[36]    = SDM;
	RT610_I2C.Data    = RT610_Arry[36];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	//PLL Lock Judgement
	Judge = 0;
	RT610_Delay_MS(20);

	for(Judge = 0;Judge < 2;Judge ++)
	{
		RT610_I2C_Len.RegAddr = 0x00;
		RT610_I2C_Len.Len     = 3;
		if(RT610_I2C_Read_Len(&RT610_I2C_Len) != RT_Success)
			return RT_Fail;

		if((RT610_I2C_Len.Data[2] == 0x00) || (RT610_I2C_Len.Data[2] == 0x2F))
		{
			RT610_I2C.RegAddr = 0x12;
			if(Judge != 0)
				RT610_Arry[30] &= 0x3F;
			
			RT610_Arry[30] |= 0x02;
			RT610_I2C.Data = RT610_Arry[30];
			if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
				return RT_Fail;

			RT610_Arry[30] &= 0xFD;
			RT610_I2C.Data = RT610_Arry[30];
			if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
				return RT_Fail;
		}

		if((RT610_I2C_Len.Data[2] == 0x00) || (RT610_I2C_Len.Data[2] == 0x2F))
		{
			RT610_I2C.RegAddr = 0x11;
			RT610_Arry[27] &= 0xFC;
			RT610_I2C.Data = RT610_Arry[27];
			if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
				return RT_Fail;
		}
	}

	return RT_Success;
}

RT610_ErrCode RT610_IMR(UInt32 RF_KHz, UInt32 IF_KHz)
{
	UInt32 RingFreq = 0;
	const UInt32 Nine_IMR = 224000;

	RT610_SectType IMR_Pont;
	
	//IMR Preparation
	RT610_I2C.RegAddr = 0x05;
	RT610_Arry[4]     = (RT610_Arry[4] & 0x80) | 0x1F;
	RT610_I2C.Data    = RT610_Arry[4];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr = 0x06;
	RT610_Arry[6]     = (RT610_Arry[6] & 0x20) | 0x0C;
	RT610_I2C.Data    = RT610_Arry[6];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr = 0x04;
	RT610_Arry[2]     = (RT610_Arry[2] & 0x3F) | 0x40;
	RT610_I2C.Data    = RT610_Arry[2];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr = 0x18;
	RT610_Arry[42]    = (RT610_Arry[42] & 0xFC) | 0x01;
	RT610_I2C.Data    = RT610_Arry[42];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr = 0x1A;
	RT610_Arry[46]    = (RT610_Arry[46] & 0x07) | 0xC0;
	RT610_I2C.Data    = RT610_Arry[46];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr = 0x11;
	RT610_Arry[28]    = (RT610_Arry[28] & 0xC7) | 0x10;
	RT610_I2C.Data    = RT610_Arry[28];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr = 0x12;
	RT610_Arry[30]    &= 0xEF;
	RT610_I2C.Data    = RT610_Arry[30];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr = 0x0A;
	RT610_Arry[14]    = (RT610_Arry[14] & 0xE0) | 0x1C;
	RT610_I2C.Data    = RT610_Arry[14];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr = 0x0B;
	RT610_Arry[16]    = (RT610_Arry[16] & 0xE4) | 0x18;
	RT610_I2C.Data    = RT610_Arry[16];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr = 0x0F;
	RT610_Arry[24]   &= 0xF7;
	RT610_I2C.Data    = RT610_Arry[24];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr = 0x0C;
	RT610_Arry[18]    = (RT610_Arry[18] & 0x40) | 0x2C;
	RT610_I2C.Data    = RT610_Arry[18];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	//Debug
	//IMR_I2C.RegAddr = 0x0C;
	//IMR_I2C.Data    = 0x2C;
	//if(RT610_I2C_Write(&IMR_I2C) != RT_Success)
	//	return RT_Fail;

	if(RF_KHz < 88500)
	{
		RT610_I2C.RegAddr = 0x19;
		RT610_Arry[44]    = (RT610_Arry[44] & 0xF7) | 0x08;
		RT610_I2C.Data    = RT610_Arry[44];
		if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
			return RT_Fail;
	}

	if(RT610_MUX(RF_KHz, IF_KHz, RingCal) != RT_Success)
		return RT_Fail;

	//IMR_I2C.RegAddr = 0x06;
	//if((RF_KHz) < 725000)
	//	IMR_I2C.Data    = 0x0C;
	//else
	//	IMR_I2C.Data    = 0x4C;
	//RT610_RingNum.Ring_06 = IMR_I2C.Data;
	//RT610_RingNum.Ring_06 = 0x4C;

	//if(RT610_I2C_Write(&IMR_I2C) != RT_Success)
	//	return RT_Fail;
	
	RT610_Arry[26] &= 0xE0;
	RT610_Arry[42] &= 0x3F;
	RT610_Arry[6]  &= 0x3F;
	RT610_Arry[44] &= 0xF7;
	
	if((RF_KHz + IF_KHz) < 88500)
	{
		RingFreq = 64018;

		RT610_Arry[26] |= 0x02;
		RT610_Arry[42] |= 0x00;
		RT610_Arry[44] |= 0x08;
	}
	else if(((RF_KHz + IF_KHz) >= 88500) && ((RF_KHz + IF_KHz) < 177000))
	{
		RingFreq = 128036;
		
		RT610_Arry[26] |= 0x02;
		RT610_Arry[42] |= 0x40;
	}
	else if(((RF_KHz + IF_KHz) >= 177000) && ((RF_KHz + IF_KHz) < 354000))
	{
		RingFreq = 224063;

		RT610_Arry[26] |= 0x05;
		RT610_Arry[42] |= 0x80;
	}
	else if(((RF_KHz + IF_KHz) >= 354000) && ((RF_KHz + IF_KHz) < 576000))
	{
		RingFreq = 384108;

		RT610_Arry[26] |= 0x0A;
		RT610_Arry[42] |= 0x80;
	}
	else if(((RF_KHz + IF_KHz) >= 576000) && ((RF_KHz + IF_KHz) < 680000))
	{
		RingFreq = 608171;

		RT610_Arry[26] |= 0x11;
		RT610_Arry[42] |= 0xC0;
	}
	else
	{
		RingFreq = 672189;

		RT610_Arry[26] |= 0x13;
		RT610_Arry[42] |= 0xC0;
	}

	RT610_Arry[26] = (UInt8)((RingFreq / 1000) / 32) - 2;
	if(RT610_Arry[26] == 0)
		RT610_Arry[26] = (UInt8)((128036 / 1000) / 32) - 2;
	RT610_Arry[44] |= 0x40;
	
	RT610_I2C.RegAddr = 0x10;
	RT610_I2C.Data    = RT610_Arry[26];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;
	
	RT610_I2C.RegAddr = 0x06;
	RT610_I2C.Data    = RT610_Arry[6];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr = 0x18;
	RT610_I2C.Data    = RT610_Arry[42];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr = 0x19;
	RT610_I2C.Data    = RT610_Arry[44];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr = 0x04;
	RT610_Arry[2]     = (RT610_Arry[2] & 0x3F) | 0xC0;
	RT610_I2C.Data    = RT610_Arry[2];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_Delay_MS(12);

	RT610_I2C.RegAddr = 0x04;
	RT610_I2C.Data    = 0x52;
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;
	
	if(RT610_PLL((RingFreq - 6000)) != RT_Success)
		return RT_Fail;

	RT610_Delay_MS(25);

	if(RF_KHz == Nine_IMR)
	{
		if(RT610_IQ(&IMR_Pont) != RT_Success)
			return RT_Fail;
			
			IMR_Data[0].IMR_Freq = 64000;
			IMR_Data[1].IMR_Freq = 128000;
			IMR_Data[2].IMR_Freq = 224000;
			IMR_Data[3].IMR_Freq = 384000;
			IMR_Data[4].IMR_Freq = 608000;
			IMR_Data[5].IMR_Freq = 680000;
			
			IMR_Data[0].SecFreq = 88500;
			IMR_Data[1].SecFreq = 177000;
			IMR_Data[2].SecFreq = 354000;
			IMR_Data[3].SecFreq = 576000;
			IMR_Data[4].SecFreq = 680000;
	}
	else
	{
		if(RT610_Nine_IQ(&IMR_Pont, 2) != RT_Success)
			return RT_Fail;
	}

	//Save IMR Value
	switch(RingFreq)
	{
	case 64018:
		IMR_Data[0].SecFreq = 88500;
		IMR_Data[0].Gain_X  = IMR_Pont.Gain_X;
		IMR_Data[0].Phase_Y = IMR_Pont.Phase_Y;
		IMR_Data[0].Value   = IMR_Pont.Value;
		break;
	case 128036:
		IMR_Data[1].SecFreq = 177000;
		IMR_Data[1].Gain_X  = IMR_Pont.Gain_X;
		IMR_Data[1].Phase_Y = IMR_Pont.Phase_Y;
		IMR_Data[1].Value   = IMR_Pont.Value;
		break;
	case 224063:
		IMR_Data[2].SecFreq = 354000;
		IMR_Data[2].Gain_X  = IMR_Pont.Gain_X;
		IMR_Data[2].Phase_Y = IMR_Pont.Phase_Y;
		IMR_Data[2].Value   = IMR_Pont.Value;
		break;
	case 384108:
		IMR_Data[3].SecFreq = 576000;
		IMR_Data[3].Gain_X  = IMR_Pont.Gain_X;
		IMR_Data[3].Phase_Y = IMR_Pont.Phase_Y;
		IMR_Data[3].Value   = IMR_Pont.Value;
		break;
	case 608171:
		IMR_Data[4].SecFreq = 680000;
		IMR_Data[4].Gain_X  = IMR_Pont.Gain_X;
		IMR_Data[4].Phase_Y = IMR_Pont.Phase_Y;
		IMR_Data[4].Value   = IMR_Pont.Value;
		break;
	default:
		IMR_Data[5].SecFreq = 680000;
		IMR_Data[5].Gain_X  = IMR_Pont.Gain_X;
		IMR_Data[5].Phase_Y = IMR_Pont.Phase_Y;
		IMR_Data[5].Value   = IMR_Pont.Value;
		break;
	}
	
	return RT_Success;
}

RT610_ErrCode RT610_Nine_IQ(RT610_SectType* N_IQ_Pont, UInt8 N_AryNum)
{
	RT610_SectType N_Compare_IQ[3];
	RT610_SectType N_CmprMax_IQ[3];
	UInt8 IQ_Cunt  = 0;
	UInt8 VGA_Cunt = 0;
	UInt8 Phase_Temp = 0;
		
	//VGA
	for(VGA_Cunt = 12;VGA_Cunt < 16;VGA_Cunt ++)
	{
		RT610_I2C.RegAddr = 0x0C;
		RT610_I2C.Data    = VGA_Cunt * 4;
		if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
			return RT_Fail;

		RT610_Delay_MS(10);
		
		RT610_I2C_Len.RegAddr = 0x00;
		RT610_I2C_Len.Len     = 2;
		if(RT610_Muti_Read(&RT610_I2C_Len.Data[1]) != RT_Success)
			return RT_Fail;
		if(RT610_I2C_Len.Data[1] > 0x28)
			break;
	}

	N_Compare_IQ[IQ_Cunt].Gain_X  = IMR_Data[N_AryNum].Gain_X;  //224
	N_Compare_IQ[IQ_Cunt].Phase_Y = IMR_Data[N_AryNum].Phase_Y;  //224

	//X
	if(RT610_IQ_Tree(N_Compare_IQ[IQ_Cunt].Phase_Y, N_Compare_IQ[IQ_Cunt].Gain_X, 0x09, &N_Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	if(RT610_CompreCor(&N_Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	N_CmprMax_IQ[IQ_Cunt] = N_Compare_IQ[0];

	Phase_Temp = IMR_Data[N_AryNum].Phase_Y;
	if((IMR_Data[N_AryNum].Phase_Y & 0x0F) >= 0x0F)
	{
        Phase_Temp &= 0xF0;
		if(Phase_Temp & 0x20)
            Phase_Temp &= 0xD0;
		else
			Phase_Temp |= 0x20;
	}
	else
		Phase_Temp = IMR_Data[N_AryNum].Phase_Y + 1;

	IQ_Cunt ++;
	N_Compare_IQ[IQ_Cunt].Gain_X  = IMR_Data[N_AryNum].Gain_X;
	N_Compare_IQ[IQ_Cunt].Phase_Y = Phase_Temp;

	//X
	if(RT610_IQ_Tree(N_Compare_IQ[IQ_Cunt].Phase_Y, N_Compare_IQ[IQ_Cunt].Gain_X, 0x09, &N_Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	if(RT610_CompreCor(&N_Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	N_CmprMax_IQ[IQ_Cunt] = N_Compare_IQ[0];

	Phase_Temp = IMR_Data[N_AryNum].Phase_Y;
	if((IMR_Data[N_AryNum].Phase_Y & 0x0F) <= 0x00)
	{
        Phase_Temp &= 0xF1;
		if(Phase_Temp & 0x20)
            Phase_Temp  = (IMR_Data[N_AryNum].Phase_Y  & 0xD0) | 0x01;
		else
			Phase_Temp |= 0x21;
	}
	else
		Phase_Temp -= 1;

	IQ_Cunt ++;
	N_Compare_IQ[IQ_Cunt].Gain_X  = IMR_Data[N_AryNum].Gain_X;
	N_Compare_IQ[IQ_Cunt].Phase_Y = Phase_Temp;

	//X
	if(RT610_IQ_Tree(N_Compare_IQ[IQ_Cunt].Phase_Y, N_Compare_IQ[IQ_Cunt].Gain_X, 0x09, &N_Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	if(RT610_CompreCor(&N_Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	N_CmprMax_IQ[IQ_Cunt] = N_Compare_IQ[0];

	if(RT610_CompreCor(&N_CmprMax_IQ[0]) != RT_Success)
		return RT_Fail;

	N_IQ_Pont[0] = N_CmprMax_IQ[0];

	return RT_Success;
}

RT610_ErrCode RT610_IQ(RT610_SectType* IQ_Pont)
{
	RT610_SectType Compare_IQ[3];
	RT610_SectType CompareTemp;
	UInt8 IQ_Cunt  = 0;
	UInt8 VGA_Cunt = 0;
		
	//VGA
	for(VGA_Cunt = 12;VGA_Cunt < 16;VGA_Cunt ++)
	{
		RT610_I2C.RegAddr = 0x0C;
		RT610_I2C.Data    = VGA_Cunt * 4;
		if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
			return RT_Fail;

		RT610_Delay_MS(10);
		
		RT610_I2C_Len.RegAddr = 0x00;
		RT610_I2C_Len.Len     = 2;
		if(RT610_Muti_Read(&RT610_I2C_Len.Data[1]) != RT_Success)
			return RT_Fail;
		if(RT610_I2C_Len.Data[1] > 0x28)
			break;
	}

	Compare_IQ[0].Gain_X  = 0x40;
	Compare_IQ[0].Phase_Y = 0x40;

	//while(IQ_Cunt < 3)
	//{
		//X
		if(RT610_IQ_Tree(Compare_IQ[IQ_Cunt].Phase_Y, Compare_IQ[IQ_Cunt].Gain_X, 0x09, &Compare_IQ[0]) != RT_Success)
			return RT_Fail;

		if(RT610_CompreCor(&Compare_IQ[0]) != RT_Success)
			return RT_Fail;

		if(RT610_CompreStep(&Compare_IQ[0], 0x08) != RT_Success)
			return RT_Fail;
		
		//Y
		if(RT610_IQ_Tree(Compare_IQ[IQ_Cunt].Gain_X, Compare_IQ[IQ_Cunt].Phase_Y, 0x08, &Compare_IQ[0]) != RT_Success)
			return RT_Fail;

		if(RT610_CompreCor(&Compare_IQ[0]) != RT_Success)
			return RT_Fail;

		if(RT610_CompreStep(&Compare_IQ[0], 0x09) != RT_Success)
			return RT_Fail;

		//CompareTemp = Compare_IQ[0];

		//Check X
		if(RT610_IQ_Tree(Compare_IQ[IQ_Cunt].Phase_Y, Compare_IQ[IQ_Cunt].Gain_X, 0x09, &Compare_IQ[0]) != RT_Success)
			return RT_Fail;

		if(RT610_CompreCor(&Compare_IQ[0]) != RT_Success)
			return RT_Fail;

		//if((CompareTemp.Gain_X == Compare_IQ[0].Gain_X) && (CompareTemp.Phase_Y == Compare_IQ[0].Phase_Y))//Ben Check
		//	break;
		
		//IQ_Cunt ++;
	//}
	//if(IQ_Cunt ==  3)
	//	return RT_Fail;

	//Section Check
	CompareTemp = Compare_IQ[0];
	for(IQ_Cunt = 0;IQ_Cunt < 5;IQ_Cunt ++)
	{
		if(RT610_Section(&Compare_IQ[0]) != RT_Success)
			return RT_Fail;

		if((CompareTemp.Gain_X == Compare_IQ[0].Gain_X) && (CompareTemp.Phase_Y == Compare_IQ[0].Phase_Y))
			break;
	}
	*IQ_Pont = Compare_IQ[0];

	RT610_I2C.RegAddr = 0x08;
	RT610_I2C.Data    = 0x40;
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr = 0x09;
	RT610_I2C.Data    = 0x40;
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	return RT_Success;
}

RT610_ErrCode RT610_IQ_Tree(UInt8 FixPot, UInt8 FlucPot, UInt8 PotReg, RT610_SectType* CompareTree)
{
	UInt8 TreeCunt  = 0;
	UInt8 TreeTimes = 3;
	UInt8 TempPot   = 0;
	UInt8 PntReg    = 0;
		
	if(PotReg == 0x08)
		PntReg = 0x09;
	else
		PntReg = 0x08;

	for(TreeCunt = 0;TreeCunt < TreeTimes;TreeCunt ++)
	{
		RT610_I2C.RegAddr = PotReg;
		RT610_I2C.Data    = FixPot;
		if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
			return RT_Fail;

		RT610_I2C.RegAddr = PntReg;
		RT610_I2C.Data    = FlucPot;
		if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
			return RT_Fail;
	
		if(RT610_Muti_Read( &CompareTree[TreeCunt].Value) != RT_Success)
			return RT_Fail;
		
		if(PotReg == 0x08)
		{
			CompareTree[TreeCunt].Gain_X  = FixPot;
			CompareTree[TreeCunt].Phase_Y = FlucPot;
		}
		else
		{
			CompareTree[TreeCunt].Phase_Y  = FixPot;
			CompareTree[TreeCunt].Gain_X = FlucPot;
		}
		
		if(TreeCunt == 0)
			FlucPot ++;
		else if(TreeCunt == 1)
		{
			if((FlucPot & 0x0F) < 0x02)
			{
				TempPot = 2 - (FlucPot & 0x0F);
				if(FlucPot & 0x20)
				{
					FlucPot &= 0xC0;
					FlucPot |= TempPot;
				}
				else
				{
					FlucPot |= 0x20 | TempPot;
				}
			}
			else
				FlucPot -= 2;
		}
	}

	return RT_Success;
}

RT610_ErrCode RT610_CompreCor(RT610_SectType* CorArry)
{
	UInt8 CompCunt = 0;
	RT610_SectType CorTemp;

	for(CompCunt = 3;CompCunt > 0;CompCunt --)
	{
		if(CorArry[0].Value > CorArry[CompCunt - 1].Value)
		{
			CorTemp = CorArry[0];
			CorArry[0] = CorArry[CompCunt - 1];
			CorArry[CompCunt - 1] = CorTemp;
		}
	}

	return RT_Success;
}

RT610_ErrCode RT610_CompreStep(RT610_SectType* StepArry, UInt8 Pace)
{
	UInt8 StepCunt = 0;
	RT610_SectType StepTemp;
	
	StepTemp.Phase_Y = StepArry[0].Phase_Y;
	StepTemp.Gain_X  = StepArry[0].Gain_X;

	while(((StepTemp.Gain_X & 0x0F) < 5) || ((StepTemp.Phase_Y & 0x0F) < 5))
	{
		if(Pace == 0x08)
			StepTemp.Gain_X ++;
		else
			StepTemp.Phase_Y ++;

		if(((StepTemp.Gain_X & 0x0F) > 4) || ((StepTemp.Phase_Y & 0x0F) > 4))
		{
			StepTemp.Gain_X  = 0x40;
			StepTemp.Phase_Y = 0x40;
			
			RT610_I2C.RegAddr = 0x08;
			RT610_I2C.Data    = StepTemp.Gain_X;
			if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
				return RT_Fail;

			RT610_I2C.RegAddr = 0x09;
			RT610_I2C.Data    = StepTemp.Phase_Y;
			if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
				return RT_Fail;

			if(RT610_Muti_Read(&StepTemp.Value) != RT_Success)
				return RT_Fail;

			StepArry[0].Gain_X  = StepTemp.Gain_X;
			StepArry[0].Phase_Y = StepTemp.Phase_Y;
			StepArry[0].Value   = StepTemp.Value;
			break;
		}
		
		RT610_I2C.RegAddr = 0x08;
		RT610_I2C.Data    = StepTemp.Gain_X ;
		if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
			return RT_Fail;

		RT610_I2C.RegAddr = 0x09;
		RT610_I2C.Data    = StepTemp.Phase_Y;
		if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
			return RT_Fail;

		if(RT610_Muti_Read(&StepTemp.Value) != RT_Success)
			return RT_Fail;

		if(StepTemp.Value <= StepArry[0].Value)
		{
			StepArry[0].Gain_X  = StepTemp.Gain_X;
			StepArry[0].Phase_Y = StepTemp.Phase_Y;
			StepArry[0].Value   = StepTemp.Value;
		}
		else
			break;		
	}
	
	return RT_Success;
}

RT610_ErrCode RT610_Section(RT610_SectType* SectionArry)
{
	UInt8 SectionCunt  = 0;
	UInt8 SectionTimes = 4;
	RT610_SectType SectionTemp;

	SectionTemp.Gain_X  = SectionArry[0].Gain_X;
	SectionTemp.Phase_Y = SectionArry[0].Phase_Y;

	for(SectionCunt = 0;SectionCunt < SectionTimes;SectionCunt ++)
	{
		if(SectionCunt < 2)
		{
			if((SectionArry[0].Gain_X & 0x0F) < 1)
			{
				if(SectionArry[0].Gain_X & 0x20)
					SectionTemp.Gain_X &= 0xDF;
				else
					SectionTemp.Gain_X |= 0x20;

				SectionTemp.Gain_X = (SectionTemp.Gain_X & 0xF0) + 1;
			}
			else
			SectionTemp.Gain_X = SectionArry[0].Gain_X - 1;
		}
		else
			SectionTemp.Gain_X = SectionArry[0].Gain_X + 1;

		if((SectionCunt - ((SectionCunt / 2) * 2)) != 0)
		{
			if((SectionArry[0].Phase_Y & 0x0F) >= 1)
				SectionTemp.Phase_Y = SectionArry[0].Phase_Y - 1;
			else
			{
				SectionTemp.Phase_Y = SectionArry[0].Phase_Y;
				if(SectionArry[0].Phase_Y & 0x20)
					SectionTemp.Phase_Y &= 0xDF;
				else
					SectionTemp.Phase_Y |= 0x20;
				SectionTemp.Phase_Y ++;
			}
		}
		else
			SectionTemp.Phase_Y = SectionArry[0].Phase_Y + 1;

		RT610_I2C.RegAddr = 0x08;
		RT610_I2C.Data    = SectionTemp.Gain_X ;
		if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
			return RT_Fail;

		RT610_I2C.RegAddr = 0x09;
		RT610_I2C.Data    = SectionTemp.Phase_Y;
		if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
			return RT_Fail;

		if(RT610_Muti_Read(&SectionTemp.Value) != RT_Success)
			return RT_Fail;

		if(SectionTemp.Value < SectionArry[0].Value)
		{
			SectionArry[0].Gain_X  = SectionTemp.Gain_X;
			SectionArry[0].Phase_Y = SectionTemp.Phase_Y;
			SectionArry[0].Value   = SectionTemp.Value;
		}
	}

	return RT_Success;
}

RT610_ErrCode RT610_Muti_Read(UInt8* IMR_Data)
{
	UInt8 ReadCunt     = 0;
	UInt8 ReadTimes    = 11;
	UInt8 RepeatedCunt = 0;
	UInt8 CompreCunt   = 0;
	BOOL ReadFlg = FALSE;
	UInt8 ReadArry[12];
	UInt8 ReadRepeated[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	RT610_Delay_MS(10);
	
	RT610_I2C_Len.RegAddr = 0x00;
	RT610_I2C_Len.Len     = 2;
	if(RT610_I2C_Read_Len(&RT610_I2C_Len) != RT_Success)
		return RT_Fail;
	ReadArry[RepeatedCunt] = RT610_I2C_Len.Data[1];
	RepeatedCunt ++;
	ReadRepeated[0] = 1;
	for(ReadCunt = 0;ReadCunt < ReadTimes;ReadCunt ++)
	{
		RT610_I2C_Len.RegAddr = 0x00;
		RT610_I2C_Len.Len     = 2;
		if(RT610_I2C_Read_Len(&RT610_I2C_Len) != RT_Success)
			return RT_Fail;
		
		CompreCunt = 0;
		ReadFlg = FALSE;
		while(CompreCunt <= RepeatedCunt)
		{
			if(ReadArry[CompreCunt] == RT610_I2C_Len.Data[1])
			{
				ReadRepeated[RepeatedCunt - 1] ++;

				if((ReadRepeated[RepeatedCunt - 1] > ReadRepeated[0]) || 
					((ReadRepeated[RepeatedCunt - 1] == ReadRepeated[0]) && 
					ReadArry[RepeatedCunt - 1] < ReadArry[0]))
				{
					ReadRepeated[RepeatedCunt] = ReadRepeated[0];
					ReadRepeated[0] = ReadRepeated[RepeatedCunt - 1];
					ReadRepeated[RepeatedCunt - 1] = ReadRepeated[RepeatedCunt];
					ReadRepeated[RepeatedCunt] = 0x00;

					ReadArry[RepeatedCunt] = ReadArry[0];
					ReadArry[0] = ReadArry[RepeatedCunt - 1];
					ReadArry[RepeatedCunt - 1] = ReadArry[RepeatedCunt];
					ReadArry[RepeatedCunt] = 0x00;
				}
				ReadFlg = TRUE;
			}
			CompreCunt ++;
		}

		if(!ReadFlg)
		{
			ReadArry[RepeatedCunt] =  RT610_I2C_Len.Data[1];
			ReadRepeated[RepeatedCunt] = 1;
			RepeatedCunt ++;
			ReadFlg = FALSE;
		}
	}
	*IMR_Data = ReadArry[0];
	
	return RT_Success;
}

RT610_ErrCode RT610_ChannelFilter(RT610_BW_Type ChannelBW, RT610_Standard_Type ChannelStandard, RT610_IF_Type RT610_IF)
{
	UInt8 ChannelLinkCunt = 0;
	bool ChannelFlg       = false;
	
	RT610_I2C.RegAddr = 0x0A;
	RT610_Arry[14]    = (RT610_Arry[14] & 0x0F) | 0x40;
	RT610_I2C.Data    = RT610_Arry[14];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr = 0x0B;
	RT610_Arry[16]    = (RT610_Arry[16] & 0xE1) | 0x02;
	RT610_I2C.Data    = RT610_Arry[16];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	for(ChannelLinkCunt = 1;ChannelLinkCunt < Channel_Link_Arry;ChannelLinkCunt ++)
	{
		if((ChannelStandard == RT610_Channel_Link_Info[ChannelLinkCunt].RT610_Channel_Standard) &&
			(ChannelBW == RT610_Channel_Link_Info[ChannelLinkCunt].RT610_Channel_BW) && 
			(RT610_IF == RT610_Channel_Link_Info[ChannelLinkCunt].RT610_Channel_IF))
		{
			if(RT610_PLL(RT610_Channel_Link_Info[ChannelLinkCunt].RT610_Channel_Freq) != RT_Success)
				return RT_Fail;

			ChannelFlg = true;
			break;
		}
	}

	if(!ChannelFlg)
	{
		if(RT610_PLL(RT610_Channel_Link_Info[0].RT610_Channel_Freq) != RT_Success)
			return RT_Fail;
	}
	
	RT610_Delay_MS(25);
	
	RT610_I2C.RegAddr = 0x0F;
	RT610_Arry[24]   |= 0x0C;
	RT610_I2C.Data    = RT610_Arry[24];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr = 0x0B;
	RT610_Arry[16]   |= 0x04;
	RT610_I2C.Data    = RT610_Arry[16];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr  = 0x0F;
	RT610_Arry[24]    &= 0xFB;
	RT610_I2C.Data     = RT610_Arry[24];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr = 0x0B;
	RT610_Arry[16]   &= 0xFB;
	RT610_I2C.Data    = RT610_Arry[16];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	//if(RT610_PLL(388570) != RT_Success)
	//	return RT_Fail;

	RT610_I2C.RegAddr = 0x10;
	RT610_Arry[26] &= 0x1F;
	RT610_I2C.Data = RT610_Arry[26] | 0x20;
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	//RT610_Delay_MS(25);

	return RT_Success;
}

RT610_ErrCode RT610_SetFrequency(RT610_Set_Info RT610_INFO)
{
	UInt8 RT_Reg08   = 0;
	UInt8 RT_Reg09   = 0;
	UInt8 IMR_SetNum = 0;
	UInt32 LO_KHz    = 0;
	
	//Set IMR
	for(IMR_SetNum = 0;IMR_SetNum < 5;IMR_SetNum ++)
	{
		/*if(IMR_Data[IMR_SetNum].Value == 0x00)
		{
			//if(RT610_Init() != RT_Success)
			//	return RT_Fail;
			
		}*/
		
		SILICON_DEBUG_MSG(LOGGER_INFO, "######IMR_Data[IMR_SetNum].SecFreq_1=%d#################\n", IMR_Data[IMR_SetNum].SecFreq);
		if(RT610_INFO.RF_KHz < IMR_Data[IMR_SetNum].SecFreq)
		{
			if(IMR_Data[IMR_SetNum].Value == 0x00)
			{
					if(RT610_IMR(IMR_Data[IMR_SetNum].IMR_Freq, 4570) != RT_Success)
						return RT_Fail;
						
					if(RT610_Standard(Analog_Arry, RT610_Data.RT610_BW, RT610_Data.RT610_Standard, RT610_Data.IF_KHz) != RT_Success)
			    	SILICON_DEBUG_MSG(LOGGER_INFO, "#########RT610_Standard() Fail!!!!!!#########\n");
						
					SILICON_DEBUG_MSG(LOGGER_INFO, "######IMR_Data[IMR_SetNum].SecFreq_0=%d#################\n", IMR_Data[IMR_SetNum].SecFreq);
			}
			
			RT_Reg08 = IMR_Data[IMR_SetNum].Gain_X;
			RT_Reg09 = IMR_Data[IMR_SetNum].Phase_Y;
			
			SILICON_DEBUG_MSG(LOGGER_INFO, "######RT_Reg08_0=%x#################\n", RT_Reg08);
  		SILICON_DEBUG_MSG(LOGGER_INFO, "######RT_Reg09_0=%x#################\n", RT_Reg09);
  		SILICON_DEBUG_MSG(LOGGER_INFO, "######IMR_Data[%d].Gain_X=%x#################\n", IMR_SetNum, IMR_Data[IMR_SetNum].Gain_X);
  		SILICON_DEBUG_MSG(LOGGER_INFO, "######IMR_Data[%d].Gain_X=%x#################\n", IMR_SetNum, IMR_Data[IMR_SetNum].Gain_X);
			break;
		}
		else if(RT610_INFO.RF_KHz > IMR_Data[5].SecFreq)
		{
			if(IMR_Data[5].Value == 0x00)
			{
					if(RT610_IMR(IMR_Data[5].IMR_Freq, 4570) != RT_Success)
						return RT_Fail;
						
					SILICON_DEBUG_MSG(LOGGER_INFO, "######IMR_Data[5].SecFreq=%d#################\n", IMR_Data[5].SecFreq);
			}
			
			RT_Reg08 = IMR_Data[5].Gain_X;
			RT_Reg09 = IMR_Data[5].Phase_Y;
			break;
		}
	}
	
	if((RT_Reg08 == 0x00) && (RT_Reg09 == 0x00))
	{
		RT_Reg08 = 0x40;
		RT_Reg09 = 0x40;
	}
	
	SILICON_DEBUG_MSG(LOGGER_INFO, "######RT_Reg08_1=%x#################\n", RT_Reg08);
  SILICON_DEBUG_MSG(LOGGER_INFO, "######RT_Reg09_1=%x#################\n", RT_Reg09);

	RT610_I2C.RegAddr = 0x08;
	RT610_Arry[10] &= 0xC0;
	RT610_I2C.Data    = RT_Reg08 | RT610_Arry[10];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr = 0x09;
	RT610_Arry[12] &= 0xC0;
	RT610_I2C.Data    = RT_Reg09 | RT610_Arry[12];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;
		
	
  //SILICON_DEBUG_MSG(LOGGER_INFO, "######RT610_Array[0]_0=%x#################\n", RT610_Arry[0] );
	if(RT610_MUX(RT610_INFO.RF_KHz, RT610_INFO.IF_KHz, LNACal) != RT_Success)
	{
    SILICON_DEBUG_MSG(LOGGER_INFO, "######RT610_MUX Fail!!!!!!#################\n");
		return RT_Fail;
	}

  //SILICON_DEBUG_MSG(LOGGER_INFO, "######RT610_Array[0]_1=%x#################\n", RT610_Arry[0] );
	if(RT610_RF_MUX(RT610_INFO) != RT_Success)
	{
              SILICON_DEBUG_MSG(LOGGER_INFO, "######RT610_RF_MUX Fail!!!!!!#################\n");
		return RT_Fail;
	}

	//Band Width
	RT610_I2C.RegAddr = 0x0B;
	switch(RT610_INFO.RT610_BW)
	{
	case BW_6MHz:		
		RT610_I2C.Data    = 0x1A;
		break;
	case BW_7MHz:
		RT610_I2C.Data    = 0x12;
		break;
	default:
		RT610_I2C.Data    = 0x02;
		break;
	}
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C_Len.RegAddr = 0x00;
	RT610_I2C_Len.Len     = 4;
	if(RT610_I2C_Read_Len(&RT610_I2C_Len) != RT_Success)
		return RT_Fail;

  //SILICON_DEBUG_MSG(LOGGER_INFO, "######RT610_Array[0]_2=%x#################\n", RT610_Arry[0] );
	RT610_Arry[0]        &= 0xDF;
       /*RT610_Arry[40] |= 0x0F;

	RT610_I2C.RegAddr = 0x17;
	RT610_I2C.Data = RT610_Arry[40];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;*/

	//Loop Through
       RT610_Arry[0]     |= 0x20;
	RT610_Arry[0] &= 0x3F;
	if(RT610_INFO.RT_Input == LOOP_THROUGH)
		RT610_Arry[0] |= 0x40;
	else
		RT610_Arry[0] |= 0xC0;

  //SILICON_DEBUG_MSG(LOGGER_INFO, "######RT610_Array[0]_3=%x#################\n", RT610_Arry[0] );
	RT610_I2C.RegAddr     = 0x03;
	RT610_I2C.Data = RT610_Arry[0];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;
	
	RT610_I2C.RegAddr = 0x05;
	RT610_Arry[4]    |= (RT610_ADDRESS & 0x20);
	RT610_I2C.Data    = RT610_Arry[4];
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	//LO_KHz = RT610_INFO.RF_KHz + RT610_INFO.IF_KHz;
	LO_KHz = RT610_INFO.RF_KHz + RT610_INFO.IF_KHz;
	if(RT610_PLL(LO_KHz) != RT_Success)
	{
    SILICON_DEBUG_MSG(LOGGER_INFO, "######RT610_LO Fail!!!!!!#################\n");
		return RT_Fail;
	}

	SILICON_DEBUG_MSG(LOGGER_INFO, "######RT610_Freq=%d#################\n", RT610_INFO.RF_KHz);
	SILICON_DEBUG_MSG(LOGGER_INFO, "######RT610_IF=%d#################\n", RT610_INFO.IF_KHz);
	
	return RT_Success;
}
/*=============================================================*/
/**
	This routine will set TV to specific frequency  in Search mode
	@param			freq					frequency
	@return 			Return the status code	_TURE,_FALSE
*/
UINT8 drv_tuner_SetFreq(UINT32 freq)
{

	RT610_Data.RF_KHz = freq / 10;
	
	/*if(RT810_Standard(Analog_Arry, RT810_Data.RT810_BW, RT810_Data.RT810_Standard, RT810_Data.IF_KHz) != RT_Success)
		SILICON_DEBUG_MSG(LOGGER_INFO, "#########RT810_Standard() Fail!!!!!!#########\n");*/

	if(RT610_SetFrequency(RT610_Data) != RT_Success)
		SILICON_DEBUG_MSG(LOGGER_INFO, "######RT610_SetFrequency Fail!!!!!!#################\n");
		
	return 0;
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
	if(mode==_SIG_MODE_AUDIO_MN)
	{
		drvif_ifd_set_audio_bp(_SIG_MODE_NTSC);
		//SILICON_DEBUG_MSG(LOGGER_INFO, "RT810_Set_Parameter RT810_Standard_NTSC = %2d\n", (UINT32)mode);
	}
	else
	{
		drvif_ifd_set_audio_bp(_SIG_MODE_PAL);
		//SILICON_DEBUG_MSG(LOGGER_INFO, "RT810_Set_Parameter RT810_Standard_NTSC = %2d\n", (UINT32)mode);
	}
		
	SILICON_DEBUG_MSG(LOGGER_INFO, "######drv_tuner_TunerAudioStdInit()\n");
}
#if 0
UINT8  drv_tuner_SetFreqafc (UINT32 freq)
{
	UINT32 LOFrequency;
	LOFrequency=(freq + ChannelIF)/10;
	//initialreg(LOFrequency);
 	updatereg(12,0x7f,0x80);				// VGA code mode
 	updatereg(12,0x80,40);
/* 	updatereg(3,0xef,0x00);
 	updatereg(3,0xf0,0x08);
 	updatereg(6,0xef,0x1<<4);
 	updatereg(6,0xf0,0x01);

 	updatereg(4,0xf8,0x00);
 	updatereg(7,0xf0,0x08);
 	updatereg(26,0x3f,0x0);			// // +3dB
 	updatereg(23,0xf0,0);
*/
	VCOpll(freq+ChannelIF,1);
	

	//SILICON_DEBUG_MSG(LOGGER_INFO, "Searching AFC channel freq:%d, ChannelIF : %d, LO:%d\n",freq,ChannelIF,LOFrequency);
	return _TRUE;
}

#endif

UINT8  CTunerSetFreqSearch (UINT32 freq)
{
	drv_tuner_SetFreq(freq);

	return _TRUE;
}
/*{
       UINT32 tempfreq;
	   
	RT610_I2C.RegAddr = 0x0B;
	switch(RT610_Data.RT610_BW)
	{
	case BW_6MHz:		
		RT610_I2C.Data    = 0x1A;
		break;
	case BW_7MHz:
		RT610_I2C.Data    = 0x12;
		break;
	default:
		RT610_I2C.Data    = 0x02;
		break;
	}
	    RT610_Data.RF_KHz = freq / 10;
			
	   tempfreq = RT610_Data.RF_KHz+ RT610_Data.IF_KHz;
 	 //RT610_PLL(tempfreq);
	 SILICON_DEBUG_MSG(LOGGER_INFO," freq -----------------------------%d\n",(UINT32)freq);

	 SILICON_DEBUG_MSG(LOGGER_INFO,"temp freq -----------------------------%d\n",(UINT32)tempfreq);
 	return  _TRUE;
  
}*/



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
	//return ChannelIF;//_TUNER_PICTURE_INTERMEDIATE_FREQ;
	return _TUNER_PICTURE_INTERMEDIATE_FREQ;
}

#if 0
void drv_tuner_isr_TunerAudioStdInit(UINT8 mode)
{
	drv_ifd_isr_Set_Audio_BP(mode);
}
#endif

/*void drv_tuner_RT810_poweroff(void)
{
	RT610_Standby(SIGLE_IN);
}*/

RT610_ErrCode RT610_Standby(RT610_LoopThrough_Type RT610_LoopSwitch)
{
	if(RT610_LoopSwitch == LOOP_THROUGH)
	{
		RT610_I2C.RegAddr = 0x03;
		RT610_I2C.Data    = 0x5B;
		if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
			return RT_Fail;

		RT610_I2C.RegAddr = 0x04;
		RT610_I2C.Data    = 0x92;
		if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
			return RT_Fail;
	}
	else
	{
		RT610_I2C.RegAddr = 0x03;
		RT610_I2C.Data    = 0xDB;
		if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
			return RT_Fail;

		RT610_I2C.RegAddr = 0x04;
		RT610_I2C.Data    = 0xD2;
		if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
			return RT_Fail;
	}

	RT610_I2C.RegAddr = 0x05;
	RT610_I2C.Data    = 0x3F;
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr = 0x07;
	RT610_I2C.Data    = 0xB4;
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr = 0x08;
	RT610_I2C.Data    = 0xC0;
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr = 0x09;
	RT610_I2C.Data    = 0xC0;
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr = 0x0A;
	RT610_I2C.Data    = 0xCE;
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr = 0x0B;
	RT610_I2C.Data    = 0x13;
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr = 0x0C;
	RT610_I2C.Data    = 0xCC;
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr = 0x11;
	RT610_I2C.Data    = 0x14;
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr = 0x12;
	RT610_I2C.Data    = 0xE2;
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr = 0x17;
	RT610_I2C.Data    = 0x03;
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr = 0x18;
	RT610_I2C.Data    = 0xCD;
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr = 0x19;
	RT610_I2C.Data    = 0x63;
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;

	RT610_I2C.RegAddr = 0x1A;
	RT610_I2C.Data    = 0x54;
	if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
		return RT_Fail;
	
	return RT_Success;
}

/*
	rf = 0 ,  cable in
	rf = 1, Air in

void drv_tuner_RT810_input_selection(UINT8 rf)
{

	rf_in = rf;
}*/

void drv_tuner_initRT810(UINT8 mode)
{
	static bit InitRT610 = _FALSE;
	static RT610_Standard_Type TempStandard;

	if(InitRT610 != _TRUE)
	{
        if(RT610_Init() == RT_Success)
        {
		        InitRT610 = _TRUE;
		        TempStandard = PAL_G;
		    }
	       else
		   SILICON_DEBUG_MSG(LOGGER_INFO, "RT610_Init Fail!!!\n");	
	}
		
	SILICON_DEBUG_MSG(LOGGER_INFO, "RT610_Set_Parameter RT610_Standard_0 = %d\n", (UINT32)mode);		
		flow_timer_Reset_WatchDog();

		RT610_Data.RT_Input = SIGLE_IN;
		
		
		if(mode == _SIG_MODE_NTSC || mode == _SIG_MODE_NTSC_443)
		{
			SILICON_DEBUG_MSG(LOGGER_INFO, "RT610_Set_Parameter RT610_Standard_1 = %d\n", (UINT32)mode);
			
			RT610_Data.RT610_Standard = NTSC_M;
			RT610_Data.RT610_BW = BW_6MHz;
			RT610_Data.IF_KHz  = NTSC_5820;
		}
		else
		{
			RT610_Data.RT610_Standard = PAL_G;
			RT610_Data.RT610_BW = BW_8MHz;
			RT610_Data.IF_KHz  = PAL_G_7820;
		}
		if(TempStandard == RT610_Data.RT610_Standard)
		{
			
		    SILICON_DEBUG_MSG(LOGGER_INFO, "RT810_Set_Parameter RT810_Standard_2 = %d\n", (UINT32)RT610_Data.RT610_Standard);
		

		    //if(RT810_Standard(Analog_Arry, RT810_Data.RT810_BW, RT810_Data.RT810_Standard, RT810_Data.IF_KHz) != RT_Success)
		    if(RT610_Standard(Analog_Arry, RT610_Data.RT610_BW, RT610_Data.RT610_Standard, RT610_Data.IF_KHz) != RT_Success)
			    SILICON_DEBUG_MSG(LOGGER_INFO, "#########RT610_Standard() Fail!!!!!!#########\n");
		}
		TempStandard = RT610_Data.RT610_Standard;

		//SILICON_DEBUG_MSG(LOGGER_INFO, "drv_tuner_initRT810(),ChannelBW=%x\n",(UINT32)ChannelBW);		
}

RT610_ErrCode RT610_Standard(UInt8* StandardArry, RT610_BW_Type ChannelBW, RT610_Standard_Type RT_Standard,
							 RT610_IF_Type RT610_IF)
{
	UInt8 StardArryCunt = 0;
	UInt8 StardArryNum  = 49;
	
	if(RT610_ChannelFilter(ChannelBW, RT_Standard, RT610_IF) != RT_Success)
		return RT_Fail;

	//Write Full Table
	RT610_I2C_Len.RegAddr = 0x03;
	RT610_I2C_Len.Len     = StardArryNum;
	for(StardArryCunt = 0;StardArryCunt < StardArryNum;StardArryCunt ++)
	{
		RT610_Arry[StardArryCunt]         = StandardArry[StardArryCunt + 1];
		//SILICON_DEBUG_MSG(LOGGER_INFO, "#########RT610_Array[%d]_4=%x#########\n", StardArryCunt, RT610_Arry[StardArryCunt]);
		RT610_I2C_Len.Data[StardArryCunt] = RT610_Arry[StardArryCunt];
	}
	if(RT610_I2C_Write_Len(&RT610_I2C_Len) != RT_Success)
	{
              SILICON_DEBUG_MSG(LOGGER_INFO, "#########RT610_Standard_Write Table Fail_0!!!!!!#########\n");
		return RT_Fail;
	}
	
	//HPF
	RT610_Arry[46] &= 0x07;
	for(StardArryCunt = 0;StardArryCunt < Channel_Control_Array;StardArryCunt ++)
	{
		if((RT_Standard == RT610_Channel_Control[StardArryCunt].RT610_Control_Standard) &&
			(ChannelBW == RT610_Channel_Control[StardArryCunt].RT610_Control_BW) &&
			(RT610_IF == RT610_Channel_Control[StardArryCunt].RT610_Channel_IF))
		{
			RT610_Arry[46] |= (RT610_Channel_Control[StardArryCunt].RT610_Control_HP | 
				               RT610_Channel_Control[StardArryCunt].RT610_Control_Gain);

			RT610_I2C.RegAddr = 0x1A;
			RT610_I2C.Data    = RT610_Arry[46];
			if(RT610_I2C_Write(&RT610_I2C) != RT_Success)
				return RT_Fail;
			break;
		}
	}
	
	return RT_Success;
}

bit RT610_I2C_Write_Len(I2C_LEN_TYPE *I2C_Info)
{
	UINT8 DataCunt = 0;
	UINT8 lWrite   = 0;
//	UINT8 RT810_I2C_Buf[50];
//	UINT8 RT810_I2C_Temp[2];
	I2C_TYPE I2C_Data;

	drv_i2c_Set_TargetAddr(I2C_MASTER_TUNER, I2C_MASTER_TUNER_ADDR);
	
	I2C_Data.RegAddr = I2C_Info->RegAddr;
	for(DataCunt = 0;DataCunt < I2C_Info->Len;DataCunt ++)
	{		
		//RT810_I2C_Buf[DataCunt + 1] = I2C_Info->Data[DataCunt];
		
		I2C_Data.Data = I2C_Info->Data[DataCunt];
		RT610_I2C_Write(&I2C_Data);
		I2C_Data.RegAddr = I2C_Info->Data[DataCunt + 1];
		DataCunt ++;
	}
	
	/*for(DataCunt = 0;DataCunt < (I2C_Info->Len / 2);DataCunt ++)
	{
		RT810_I2C_Temp[0] = RT810_I2C_Buf[lWrite];
		RT810_I2C_Temp[1] = RT810_I2C_Buf[lWrite + 1];
		if(!drv_i2c_Write(I2C_MASTER_TUNER, 2, RT810_I2C_Temp))
		{
			SILICON_DEBUG_MSG(LOGGER_INFO, "######Silicon Access Error!!!!!!\n");
			return _FALSE;
		}
			
		lWrite *= 2;
	}*/
	
	return _TRUE;
}

bit RT610_I2C_Write(I2C_TYPE *I2C_Info)
{
	UINT8 RT810_I2C_Temp[2];
	
	drv_i2c_Set_TargetAddr(I2C_MASTER_TUNER, I2C_MASTER_TUNER_ADDR);
	
	RT810_I2C_Temp[0] = I2C_Info->RegAddr;
	RT810_I2C_Temp[1] = I2C_Info->Data;
	
	if(!drv_i2c_Write(I2C_MASTER_TUNER, 2, RT810_I2C_Temp))
		{
			SILICON_DEBUG_MSG(LOGGER_INFO, "######Silicon Access Error!!!!!!\n");
			return _FALSE;
		}
	return _TRUE;
}

bit RT610_I2C_Read_Len(I2C_LEN_TYPE *I2C_Info)
{
       UINT8 Read_Cunnt = 0;

	   if( drv_i2c_ContRead(I2C_MASTER_TUNER, I2C_Info->Len, I2C_Info->Data) == _FALSE)
	   	return RT_Fail;

	   for(Read_Cunnt = 0;Read_Cunnt < I2C_Info->Len;Read_Cunnt ++)
	   {
	         I2C_Info->Data[Read_Cunnt] = RT610_Convert(I2C_Info->Data[Read_Cunnt]);
	   }
        
	  return RT_Success;
}

UINT8 RT610_Convert(UINT8 InvertNum)
{
	UINT8 ReturnNum = 0;
	UINT8 AddNum    = 0x80;
	UINT8 BitNum    = 0x01;
	UINT8 CuntNum   = 0;

	for(CuntNum = 0;CuntNum < 8;CuntNum ++)
	{
		if(BitNum & InvertNum)
			ReturnNum += AddNum;

		AddNum /= 2;
		BitNum *= 2;
	}

	return ReturnNum;
}

/*
void drv_tuner_write_rt810_cali(UINT8* pRegsMap)
{

	UINT8 RT810RegAccessBuf[2];
	pRegsMap = pRegsMap;
	drv_i2c_Set_TargetAddr(I2C_MASTER_TUNER, I2C_MASTER_TUNER_ADDR);
        	

		RT810RegAccessBuf[0]=11;
		//RT810RegAccessBuf[1]=pRegsMap[0];
		//if(!drv_i2c_Write(I2C_MASTER_TUNER, 2, RT810RegAccessBuf))
		//	SILICON_DEBUG_MSG(LOGGER_INFO, "######Silicon Access Error!!!!!!\n");

		RT810RegAccessBuf[0]=15;
		//RT810RegAccessBuf[1]=pRegsMap[1];
		//if(!drv_i2c_Write(I2C_MASTER_TUNER, 2, RT810RegAccessBuf))
		//	SILICON_DEBUG_MSG(LOGGER_INFO, "######Silicon Access Error!!!!!!\n");
    	
//	fw_timer_event_DelayXms(5);

}
void drv_tuner_write_rt810_vcoonly(UINT8* pRegsMap)
{

	UINT8 RT810RegAccessBuf[2];
		pRegsMap = pRegsMap;
	drv_i2c_Set_TargetAddr(I2C_MASTER_TUNER, I2C_MASTER_TUNER_ADDR);


		RT810RegAccessBuf[0]=22;
		//RT810RegAccessBuf[1]=pRegsMap[22-3];
		//if(!drv_i2c_Write(I2C_MASTER_TUNER, 2, RT810RegAccessBuf))
		//	SILICON_DEBUG_MSG(LOGGER_INFO, "######Silicon Access Error!!!!!!\n");

		RT810RegAccessBuf[0]=21;
		//RT810RegAccessBuf[1]=pRegsMap[21-3];
		//if(!drv_i2c_Write(I2C_MASTER_TUNER, 2, RT810RegAccessBuf))
		//	SILICON_DEBUG_MSG(LOGGER_INFO, "######Silicon Access Error!!!!!!\n");
		
		RT810RegAccessBuf[0]=20;
		//RT810RegAccessBuf[1]=pRegsMap[20-3];
		//if(!drv_i2c_Write(I2C_MASTER_TUNER, 2, RT810RegAccessBuf))
		//	SILICON_DEBUG_MSG(LOGGER_INFO, "######Silicon Access Error!!!!!!\n");

		RT810RegAccessBuf[0]=18;
		//RT810RegAccessBuf[1]=pRegsMap[18-3];
		//if(!drv_i2c_Write(I2C_MASTER_TUNER, 2, RT810RegAccessBuf))
		//	SILICON_DEBUG_MSG(LOGGER_INFO, "######Silicon Access Error!!!!!!\n");

}
*/
#endif
#endif
