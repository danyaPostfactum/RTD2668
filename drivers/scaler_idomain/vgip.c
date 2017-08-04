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
#include "scaler_idomain\vgip.h"
#include "scaler\scaler.h"
#include "scaler\adjust.h"
#include "autoconf.h"
#include "sparrow.h"
#include "reg_def.h"
#include "mode.h"
#include "timer_event.h"
#include "source.h"
#include "rosprintf.h"
#include "flow_struct.h"
#include "flow_scaler.h"
#include "mode_customer.h"
#include "rtd_types.h"


// set default to vgip
void drv_vgip_Set_Default(void)
{
    // Disable SRC_Clock_Enbale bit
    rtdf_clearBits(VGIP_CHN1_CTRL_0x0a10, _BIT0);

    //BIT2,BIT3=>no invert H/V signal polarity
    //BIT15 => active window is according to IHS signal.
    //_BIT16=>ODD signal produce by VGIP or external signal
    //_BIT18=>Invert ODD signal
    rtdf_clearBits(VGIP_CHN1_CTRL_0x0a10, _BIT16|_BIT18|_BIT3|_BIT2);

    // CSW+ 0961120 Reset to positive edge to latch the stable data
    rtdf_clearBits(VGIP_CHN1_CTRL_0x0a10, _BIT6);

    //restore Hsync/Vsync delay to zero
    rtdf_maskl(VGIP_CHN1_DELAY_0x0a20, 0xf000f000, 0);

}

void drv_vgip_set_vgip_capture_shift(INT8 shiftx, INT8 shifty)
{
    //return;//frank@0211 mark for correct image
    INT16 HStart,VStart;
    UINT32 tmp;
    //////////////////////////////////////////////
    //Shift H/V Start Position
    /////////////////////////////////////////////
    HStart = disp_info.IPH_ACT_STA -(shiftx * 2); // must even
    VStart = disp_info.IPV_ACT_STA -(shifty * 1);
    tmp = HStart&0x0FFF;
    tmp = tmp <<16;
    rtdf_maskl(VGIP_CHN1_ACT_HSTA_WIDTH_0x0a18,0xf000ffff, tmp);
    tmp = VStart&0x0FFF;
    tmp = tmp << 16;
    rtdf_maskl(VGIP_CHN1_ACT_VSTA_LENGTH_0x0a1c,0xf000ffff,tmp);

#if 0
    rtdf_maskl(VGIP_CHN1_ACT_HSTA_WIDTH_0x0a18,0xf000ffff, ((UINT32)HStart & 0x00000fff) << 16);
    rtdf_maskl(VGIP_CHN1_ACT_VSTA_LENGTH_0x0a1c,0xf000ffff,((UINT32)VStart & 0x00000fff) << 16);
#endif

}
#ifdef CONFIG_YPBPR_OPEN_H_V_POSITION
#define SRC_CHECK()	(GET_INPUTSOURCE_FROM() == _SRC_FROM_ADC)
#else
#define SRC_CHECK() 	 (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
#endif

/*============================================================================*/
/**
 * fw_scaler_set_vgip_capture
 * Set capture window of input source.
 *
 * @param <void>		{ void }
 * @return 			{ void }
 *
 */
void drv_vgip_set_vgip_capture(void)
{
    UINT16 usV_Porch;
    UINT32 tmp;

    ucIVS2DVSDelay = 0;
    ucIHSDelay= 0;
    if (SRC_CHECK())   	// weihao 960308
    {

        //RTD_Log(LOGGER_DEBUG,"V Postion = %x\n", GET_V_POSITION(info->data_array_index));
        //RTD_Log(LOGGER_DEBUG,"H Postion = %x\n", GET_H_POSITION(info->data_array_index));
        ucIVS2DVSDelay = drv_adjust_ivs2dvsdelay(); // CSW+ 0961002 Set IVS2DVS delay
        //RTD_Log(LOGGER_DEBUG,"\n SetCapture IHStart = %x in fw_scaler_set_vgip_capture\n", info->IPH_ACT_STA);
        ucIHSDelay= _H_POSITION_MOVE_TOLERANCE;

        info->IPH_ACT_STA -= _H_POSITION_MOVE_TOLERANCE;
        //RTD_Log(LOGGER_DEBUG,"\n SetCapture IHStart = %x in fw_scaler_set_vgip_capture\n", info->IPH_ACT_STA);
        usV_Porch = (UINT8)(info->IVTotal - info->IPV_ACT_LEN - 4);
        //CSW 0970121 Add protection o avoid front porch exceed 255
        if ((info->IVTotal - info->IPV_ACT_LEN - 4) > 255)
            usV_Porch = 255;
        else
            usV_Porch = (UINT8)(info->IVTotal - info->IPV_ACT_LEN - 4);//Get the porch
        RTD_Log(LOGGER_DEBUG," usV_Porch = %d\n",usV_Porch);
        ucIPH_Max_Margin = 0x80 + ucIHSDelay;
        ucIPH_Min_Margin = 0x80 - ucIHSDelay;
        ucIPV_Max_Margin = 0x80 + ((usV_Porch > _V_POSITION_MOVE_TOLERANCE) ? _V_POSITION_MOVE_TOLERANCE : usV_Porch);

        ucIPV_Min_Margin = 0x80 -((ucIVS2DVSDelay > _V_POSITION_MOVE_TOLERANCE) ? _V_POSITION_MOVE_TOLERANCE :  ucIVS2DVSDelay);

//		RTD_Log(LOGGER_DEBUG,"\n ucIPV_Max_Margin = %x", ucIPV_Max_Margin);
//		RTD_Log(LOGGER_DEBUG,"\n ucIPV_Min_Margin = %x", ucIPV_Min_Margin);
        RTD_Log(LOGGER_DEBUG,"ucIVS2DVSDelay = %d\n", ucIVS2DVSDelay);
        IVS2DVSDelay_VGA = ucIVS2DVSDelay;
    }

#if 0
    rtdf_maskl(VGIP_CHN1_ACT_HSTA_WIDTH_0x0a18, 0xf000f000, (((UINT32)(info->IPH_ACT_STA & 0x00000fff) << 16)|(info->IPH_ACT_WID & 0x00000fff)));
    rtdf_maskl(VGIP_CHN1_ACT_VSTA_LENGTH_0x0a1c, 0xf000f000,((UINT32)((info->IPV_ACT_STA - ucIVS2DVSDelay /*+ _PROGRAM_VDELAY*/) & 0x00000fff) << 16)|(info->IPV_ACT_LEN & 0x00000fff));
    RTD_Log(LOGGER_DEBUG,"VGIP_CHN1_ACT_HSTA_WIDTH_0x0a18 = %x\n", Log_trans_UINT32(rtdf_inl(VGIP_CHN1_ACT_HSTA_WIDTH_0x0a18)));
    RTD_Log(LOGGER_DEBUG,"VGIP_CHN1_ACT_VSTA_LENGTH_0x0a1c = %x\n", Log_trans_UINT32(rtdf_inl(VGIP_CHN1_ACT_VSTA_LENGTH_0x0a1c)));
#endif
    tmp = info->IPH_ACT_STA;
    tmp = tmp << 16 | info->IPH_ACT_WID;
    RTD_Log(LOGGER_DEBUG,"H VGIP_CHN1_ACT_HSTA_WIDTH_0x0a18= %x\n", tmp);
    rtdf_maskl(VGIP_CHN1_ACT_HSTA_WIDTH_0x0a18, 0xf000f000, tmp);
    tmp = info->IPV_ACT_STA - ucIVS2DVSDelay;
    tmp = tmp <<16 | info->IPV_ACT_LEN;
    RTD_Log(LOGGER_DEBUG,"H VGIP_CHN1_ACT_VSTA_LENGTH_0x0a1c= %x\n", tmp);
    rtdf_maskl(VGIP_CHN1_ACT_VSTA_LENGTH_0x0a1c, 0xf000f000, tmp);


    if (SRC_CHECK())
    {
        drv_vgip_set_vgip_capture_shift(0  , 0);
    }
    else
    {
        RTD_Log(LOGGER_DEBUG,"H position= %x\n", (UINT32)GET_H_POSITION());
        RTD_Log(LOGGER_DEBUG,"V position= %x\n", (UINT32)GET_V_POSITION());
        //drv_vgip_set_vgip_capture_shift(GET_H_POSITION()  -0x80, GET_V_POSITION()  -0x80);
    }

    // CSW+ 0961011 to set VGIP ctrl bits
    if (GET_INPUTSOURCE_TYPE() == _SOURCE_HDMI)
    {
        if (info->ucMode_Curr == _HDMI_MODE_1080I25 || info->ucMode_Curr == _HDMI_MODE_1080I30) //set odd signal inverse
            rtdf_maskl(VGIP_CHN1_CTRL_0x0a10, ~(/*_BIT18 |_BIT16 | */_BIT8 | _BIT6 | _BIT5 | _BIT4 ), 0); // CSW+  _BIT3 0960824
        else
            rtdf_maskl(VGIP_CHN1_CTRL_0x0a10, ~(/*_BIT18 |_BIT16 | */_BIT8 | _BIT6 | _BIT5 | _BIT4 ), _BIT5);

        //if ((info->Polarity== _SYNC_HP_VN) || ( info->Polarity== _SYNC_HN_VN))	//VS Negative
        //	rtdf_maskl(VGIP_CHN1_CTRL_0x0a10, ~(_BIT3), _BIT3); // Let VS negtive polarity to positive polarity

    }
}

void drv_vgip_set_vgip(UINT8 src, UINT8 mode)
{
    // Disable SRC_Clock_Enbale bit
    rtdf_maskl(VGIP_CHN1_CTRL_0x0a10, ~_BIT0,0x0);

    // 2nd determine the input source
    rtdf_maskl(VGIP_CHN1_CTRL_0x0a10, 0x8ffffffd, ((UINT32)src<<28) | (((UINT32)mode & 0x01)<<1));

    // 3rd enable SRC_Clock_Enbale bit
    rtdf_maskl(VGIP_CHN1_CTRL_0x0a10, ~(_BIT0), _BIT0);

    RTD_Log(LOGGER_DEBUG,"VGIP_CHN1_CTRL_0x0a10 = %x\n", (UINT32)rtdf_inl(VGIP_CHN1_CTRL_0x0a10));

    rtdf_maskl(DATA_PATH_SELECT_0x0a84,~(_BIT5|_BIT4|_BIT3|_BIT2|_BIT1|_BIT0),_BIT3|_BIT2);

    rtdf_setBits(VGIP_CHN1_CTRL_0x0a10, _BIT16); //field toggled by VGIP
}

void drv_vgip_set_sample(UINT8 enable)
{
    rtdf_maskl(VGIP_CHN1_CTRL_0x0a10, ~_BIT31,(enable ? _BIT31 : 0));
}
/*
void drv_vgip_disablevgip(void)
{
	// Disable Sample Clock in VGIP
	drv_vgip_set_sample(_DISABLE);
}
*/

void drv_vgip_SetVSDelay(UINT16 val)
{
    rtdf_writeWord0(VGIP_CHN1_DELAY_0x0a20, val & 0x0fff);
    rtdf_writeWord1(VGIP_VBISLI_0x0a88,val & 0x0fff);
}

UINT16 drv_vgip_GetVSDelay(void)
{
    return (UINT16)(rtdf_inl(VGIP_CHN1_DELAY_0x0a20) & 0xfffff000);
}

void drv_vgip_adjust_ihs_delay(UINT16 usIHSDelay)
{
    rtdf_maskl( VGIP_CHN1_DELAY_0x0a20, 0xf000ffff, (((UINT32)(_PROGRAM_HDELAY + usIHSDelay) & 0x00000fff) << 16));
    //usIHSDelay = (IoReg_Read32(IDOMAIN_VGIP_VGIP_CHN1_DELAY_VADDR) >> 16);
    rtdf_writeWord0(VGIP_VBISLI_0x0a88,usIHSDelay);
    RTD_Log(LOGGER_DEBUG,"IHSDelay in drv_scaler_adjust_ihs_delay=%x\n", (UINT32)usIHSDelay);
}

void drv_vgip_adjust_ivs_delay(UINT16 usIVSDelay)
{
    rtdf_maskl(VGIP_CHN1_DELAY_0x0a20,  ~0x00000fff, ( usIVSDelay /*+_PROGRAM_VDELAY */) & 0x00000fff);
    rtdf_writeWord1(VGIP_VBISLI_0x0a88,usIVSDelay);
    RTD_Log(LOGGER_DEBUG,"IVSDelay=%x\nVGIP_CHN1_DELAY_0x0a20=%x\n", (UINT32)usIVSDelay,(UINT32)rtdf_inl(VGIP_CHN1_DELAY_0x0a20));
}

//******************************************************************************
//
// FUNCTION       :  void fw_scaler_cal_vgip_capture(void)
//
// USAGE          	:  	This function is used to handle some auto aspect ratio, including movies & subtitle */
//
// INPUT         	 	:  	{ none }
//
// OUTPUT         	:	{ none }
//
// GLOBALS        	:
//
// USED_REGS      :
//
//******************************************************************************
void drv_vgip_cal_vgip_capture(void)
{
    ////////////////////////////////////////////////////
    //Transfer Orignal H/V Start , Size to needed overscan variables(info->IPH_ACT_STA,....)
    /////////////////////////////////////////////////////
    info->IPH_ACT_STA = info->IPH_ACT_STA_PRE;
    // [Code Sync][CSW][0980421][1] Add protection to prevent image shaking when
    // enabling de-interlace setting 0x1806_104c(DragonLite spec)for odd field delay even field 1 line
    info->IPH_ACT_WID = info->IPH_ACT_WID_PRE = info->IPH_ACT_WID_PRE & (~_BIT0);
    info->IPV_ACT_STA = info->IPV_ACT_STA_PRE;
    info->IPV_ACT_LEN = info->IPV_ACT_LEN_PRE;

//=============================//
/////////////////////////////////////////////////////////////
//=============================//
//#ifdef CONFIG_DISPLAY_RATIO_ENABLE
//=============================//

    ///////////////////////////////////////////////////////////////
    //Check Aspect Ratio
    ///////////////////////////////////////////////////////////////
    /*	if (IS_RATIO_LETTERBOX() && (GET_INPUTSOURCE_TYPE() != _SOURCE_VGA)) //letter box display
    	{
    		///////////////////////////////
    		//letter box display
    		//////////////////////////////
    		//
    		/////////////////////////////////////////////////////
    		//Calculate new V Start Position: Original + (V Size / 8)
    		//					   V Size = ((Original / 8) * 6)
    		/////////////////////////////////////////////////////
    		info->IPV_ACT_STA = info->IPV_ACT_STA + (info->IPV_ACT_LEN >> 3);
    		info->IPV_ACT_LEN = ((info->IPV_ACT_LEN >> 2)*3) & ~_BIT0;

    		///////////////////////////////
    		//for astro VG848/859 T972 1080i 30Hz frame sync error
    		//////////////////////////////
    		if((GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR) && (info->ucMode_Curr == _MODE_1080I30))
    		{
    			info->IPV_ACT_STA = info->IPV_ACT_STA - 1;
    			info->IPV_ACT_LEN = info->IPV_ACT_LEN + 2;
    		}
    		//stModeInfo.IVStartPos = info->IPV_ACT_STA;
    		//stModeInfo.IVHeight = info->IPV_ACT_LEN;
    	}
    */
//#endif	//CONFIG_DISPLAY_RATIO_ENABLE
//=============================//

}




