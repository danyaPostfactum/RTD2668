/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2006
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is for message related functions.
 *
 * @author 	$Author$
 * @date	$Date$
 * @version	$Revision$
 * @ingroup	message
 */

/**
 * @addtogroup message
 * @{
 */

#include "framesync.h"
#include "hdmi\hdmiFun.h"
//#include "mode_customer.h"
#include "scaler\scaler_pll.h"
#include "scaler\adjust.h"
#include "panel\panel.h"
#include "reg_def.h"
#include "mode.h"
#include "timer_event.h"
#include "source.h"
#include "rosprintf.h"
#include "flow_struct.h"
#include "flow_scaler.h"
#include "scaler_idomain\syncproc.h"
#include "rtd_types.h"
#include "tv.h"
#include "osdcontrol.h"


#define _DCLK_SPREAD_RANGE (0)

#if(CONFIG_DISPLAY_LAST_LINE_FINETUNE == _LAST_LINE_METHOD_0)
static BOOL framesync_adjustlastLine(BOOL bUp,UINT16 Offset, UINT16 LastLine)
{
    UINT16 DVTotal,DHTotal,DVTotal1,DHTotal1,Refer,Refer_Progress ;
    UINT8 timeout = 100;
    Refer_Progress = 100;
/////////////////////////////////////////////////////////////////////////////
/////////////Patch for Sparrow
    if ((Get_DISP_ACT_H_WID() == 1920)||(Get_DISP_ACT_H_WID() == 1680))
    {
        if (info->IPH_ACT_WID > 1300) // 1080p
        {
            Refer_Progress = 130;
            if ((Get_DISP_ACT_H_WID() == 1680)&&(info->IVFreq >550))
                bUp = _TRUE;
        }
        if ((Get_DISP_ACT_H_WID() == 1920) &&(info->IPH_ACT_WID == 1440))
            bUp = _TRUE;
    }
///////////////////////////////////////////////////////////////////////////////

    while (timeout--)
    {
        if (bUp)
            Offset ++;
        else
            Offset --;


        RTD_Log(LOGGER_DEBUG,"Dclk offset after adjust = %x\n",(UINT32)Offset);
        rtdf_writeWord1(DCLKSS_0x2f08, Offset);

        rtdf_setBits(DCLKSS_0x2f08, _BIT2|_BIT0);

        WaitFor_IVS1();
        WaitFor_IVS1();
        WaitFor_IVS1();
        WaitFor_IVS1();

        DVTotal = rtdf_readWord1(MEASURE_DVTOTAL_LAST_LINE_PIXELS_RESULT1_0x203c);
        DHTotal = rtdf_readWord0(MEASURE_DVTOTAL_LAST_LINE_PIXELS_RESULT1_0x203c);
        RTD_Log(LOGGER_DEBUG,"Measure after adjust DV total = %x\n",(UINT32)DVTotal);
        RTD_Log(LOGGER_DEBUG,"Measure after adjust last line = %x\n",(UINT32)DHTotal);
        DVTotal1 = rtdf_readWord1(MEASURE_DVTOTAL_LAST_LINE_PIXELS_RESULT2_0x2040);
        DHTotal1 = rtdf_readWord0(MEASURE_DVTOTAL_LAST_LINE_PIXELS_RESULT2_0x2040);
        RTD_Log(LOGGER_DEBUG,"Measure after adjust second DV total = %x\n",(UINT32)DVTotal1);
        RTD_Log(LOGGER_DEBUG,"Measure after adjust second last line = %x\n",(UINT32)DHTotal1);
        if (timeout == 99)	// calculate reference
        {
            Refer = ABS(DHTotal,DHTotal1);
            DHTotal1 += DHTotal;
            DHTotal1 = DHTotal1>>1;
            Refer += ABS(DHTotal1,LastLine);
            Refer += 30;
            if (Refer > 230)
                Refer = 230;
        }
        else
        {
            DHTotal1 += DHTotal;
            DHTotal1 = DHTotal1>>1;
        }

        RTD_Log(LOGGER_DEBUG,"last line reference= %x\n",(UINT32)Refer);
        if ((GET_INTERLACE_MODE()&&(DHTotal1< Refer))||(DHTotal1< Refer_Progress))
        {
            rtd_outl(DV_TOTAL_0x2014,DVTotal-1);
            if (DHTotal1 > 200)
                rtdf_maskl(DCLKSS_0x2f08, ~(_BIT8 |_BIT9|_BIT10|_BIT11|_BIT12|_BIT13),_BIT10 |_BIT2|_BIT0);
            else
                rtdf_maskl(DCLKSS_0x2f08, ~(_BIT8 |_BIT9|_BIT10|_BIT11|_BIT12|_BIT13),_BIT8|_BIT9|_BIT2|_BIT0);
            rtdf_setBits(DISPLAY_TIMING_CTRL1_0x2000, _BIT27);
            return _TRUE;
        }
    }	//while (1)

}

#if 0
static void framesync_iv2dvadjust(void)
{
    UINT32 iv2dv,pixels,value;
    UINT8 i;
    iv2dv = rtdf_inl(FS_IV_DV_FINE_TUNING_0x2020);
    if (GET_INTERLACE_MODE())
        return;
    RTD_Log(LOGGER_DEBUG,"\n framesync_iv2dvadjust = %x\n",iv2dv);
    for (i = 1; i<5; i++)
    {
        // disable last line
        rtdf_clearBits(DISPLAY_TIMING_CTRL1_0x2000, _BIT27);
        pixels = (iv2dv>>14)&0xff;
        pixels += i*5;
        value = iv2dv & 0xffc03fff;
        value |= pixels << 14;
        RTD_Log(LOGGER_DEBUG,"\n adjust iv2dv value = %x\n",value);
        rtdf_outl(FS_IV_DV_FINE_TUNING_0x2020,value);
        rtdf_setBits(DISPLAY_TIMING_CTRL1_0x2000, _BIT27);
        fw_timer_event_DelayXms(40);
        if (flow_framesync_status() == 0)
            return;
    }

    // restore to original value
    rtdf_outl(FS_IV_DV_FINE_TUNING_0x2020,iv2dv);
}
#endif
#endif
/**
* framesync_synclastlinefinetune
*  This function will Finetune last line problem
* @param <pDelta>       { Dclk offset}
*@return <result>  	 { Return TRUE if there is no timing change}
*/
#define _MEASURE_2FRAME_AT_ONCE
#define CONTINUE_MODE	0
#define SINGLE_MODE		1
#define _SELECT_MEASURE_MODE	SINGLE_MODE
static UINT8 framesync_synclastlinefinetune(void)
{
#if(CONFIG_DISPLAY_LAST_LINE_FINETUNE == _LAST_LINE_METHOD_NONE)

    return _TRUE;

#elif(CONFIG_DISPLAY_LAST_LINE_FINETUNE == _LAST_LINE_METHOD_0)


    UINT16 DclkOffset;
    UINT16 DVTotal,DHTotal,DHTotal1;
    rtdf_outl(MEASURE_DVTOTAL_LAST_LINE_PIXELS_RESULT1_0x203c,0xa0000000);
    WaitFor_IVS1();
    WaitFor_IVS1();
    WaitFor_IVS1();
    WaitFor_IVS1();
    DVTotal = rtdf_readWord1(MEASURE_DVTOTAL_LAST_LINE_PIXELS_RESULT1_0x203c);
    DHTotal = rtdf_readWord0(MEASURE_DVTOTAL_LAST_LINE_PIXELS_RESULT1_0x203c);
    RTD_Log(LOGGER_DEBUG,"\nMeasure first time DV total = %x\n",(UINT32)DVTotal);
    RTD_Log(LOGGER_DEBUG,"Measure first time last line = %x\n",(UINT32)DHTotal);
    DHTotal1 = rtdf_readWord0(MEASURE_DVTOTAL_LAST_LINE_PIXELS_RESULT2_0x2040);
    RTD_Log(LOGGER_DEBUG,"Measure after adjust second last line = %x\n",(UINT32)DHTotal1);

    DHTotal += DHTotal1;
    DHTotal = DHTotal >>1;
    RTD_Log(LOGGER_DEBUG,"Measure average last line = %x\n",(UINT32)DHTotal);

    rtd_outl(DV_TOTAL_0x2014,DVTotal);
    RTD_Log(LOGGER_DEBUG,"Last line comparison for interlace = %x\n",(UINT32)_LAST_LINE_FOR_INTERLACE);
    RTD_Log(LOGGER_DEBUG,"Last line comparison for progress= %x\n",(UINT32)_LAST_LINE_FOR_PROGRESS);
    if ((GET_INTERLACE_MODE()&&(DHTotal < _LAST_LINE_FOR_INTERLACE))||(DHTotal < _LAST_LINE_FOR_PROGRESS))
    {
        rtd_outl(DV_TOTAL_0x2014,DVTotal-1);
        rtdf_setBits(DCLKSS_0x2f08, _BIT9|_BIT8|_BIT2|_BIT0);
        rtdf_setBits(DISPLAY_TIMING_CTRL1_0x2000, _BIT27);
        return _TRUE;
    }	//if (DHTotal <100)


    RTD_Log(LOGGER_DEBUG,"Need to adjust offset reference HTotal= %x\n",(UINT32)(Get_DISP_HORIZONTAL_TOTAL()/3));
    //tick system WD, to avoid system reset
    flow_timer_Reset_WatchDog();
    DclkOffset = rtdf_readWord1(DCLKSS_0x2f08);
    if (DHTotal < (Get_DISP_HORIZONTAL_TOTAL()/3))
    {
        if (framesync_adjustlastLine(_TRUE,DclkOffset,DHTotal))
        {
            fw_timer_event_DelayXms(20);
            if (flow_framesync_status() == 0)
                return _TRUE;
            //framesync_iv2dvadjust();
        }
        RTD_Log(LOGGER_DEBUG,"Measure completed but frame sync error!!\n",(UINT32)DHTotal);
        rtdf_clearBits(DISPLAY_TIMING_CTRL1_0x2000, _BIT27);
        drv_pll_Set_DClk_offset(DclkOffset);
        fw_timer_event_DelayXms(30);

    }
    //tick system WD, to avoid system reset
    flow_timer_Reset_WatchDog();
    if (framesync_adjustlastLine(_FALSE,DclkOffset,DHTotal))
    {
        fw_timer_event_DelayXms(20);
        if (flow_framesync_status() == 0)
            return _TRUE;
        //framesync_iv2dvadjust();
        RTD_Log(LOGGER_DEBUG,"Frame sync error, Restore to frame sync\n",(UINT32)DHTotal);
        rtdf_clearBits(DISPLAY_TIMING_CTRL1_0x2000, _BIT27);
        drv_pll_Set_DClk_offset(DclkOffset);
        fw_timer_event_DelayXms(30);
        //tick system WD, to avoid system reset
        flow_timer_Reset_WatchDog();
        if (DHTotal > (Get_DISP_HORIZONTAL_TOTAL()/3))
        {
            if (framesync_adjustlastLine(_TRUE,DclkOffset,DHTotal))
            {
                fw_timer_event_DelayXms(20);
                if (flow_framesync_status() == 0)
                    return _TRUE;
                //framesync_iv2dvadjust();
            }
        }

    }
    RTD_Log(LOGGER_DEBUG,"Frame sync error, Restore to frame sync\n",(UINT32)DHTotal);
    rtdf_clearBits(DISPLAY_TIMING_CTRL1_0x2000, _BIT27);
    drv_pll_Set_DClk_offset(DclkOffset);
    return _FALSE;
#endif
}

/**
* framesync_testsync
*  This function will Apply Dclk frequency and get the overflow/underflow information
* @param <usOffset>  {Dclk offset}
* @param <retValue>  {under/over flow status}
*@return <result>  	 {Return FALSE if there is no timing change}
*/
static UINT8 framesync_testsync(UINT16 usOffset, UINT8* retValue)
{
    // Apply Dclk frequency setting
    fw_timer_event_DelayXms(40);//RtSystem_Sleep(40);
    drv_pll_Set_DClk_offset(usOffset);

    fw_timer_event_DelayXms(10);//RtSystem_Sleep(40);
    *retValue = flow_framesync_status();
    return FALSE;
}

UINT8 flow_linebuffer_status(void)
{
    UINT8 value;
    value = rtdf_readByte3(DM_UZU_CTRL_0x1604);
    value = (value & _BIT1);
    return value;
}

UINT8 flow_framesync_status(void)
{
    UINT8 value;
    rtdf_setBits(MAIN_DISPLAY_CONTROL_RSV_0x2100,  _BIT18|_BIT19);//write status
    fw_timer_event_DelayXms(40);//test by tyan
    // Save underflow/overflow information into pData[0]
    value = rtdf_readByte2(MAIN_DISPLAY_CONTROL_RSV_0x2100)>>2;
    // printf("ICH1_IBUFF_STATUS: %x\n",*retValue); // for debug
    value = (value & (_BIT0 | _BIT1 ));//frank@0625 add BIT2 to check frame sync error
    return value;
}
/**
* framesync_spreadspectrumfinetune
*  This function will Finetune Dclk offset for spread spectrum
* @param <pDelta>       { Dclk offset}
*@return <result>  	 { Return FALSE if there is no timing change}
*/
static UINT8 framesync_spreadspectrumfinetune(UINT16 *pDelta)
{
    UINT8 Data= 0;
    drv_adjust_spreadspectrumrange(_DCLK_SPREAD_RANGE);

    if (framesync_testsync(*pDelta, &Data))
        return 2;

    /*	if(Data) {
    		//Fine-tune DCLK offset
    		if(framesync_testsync((Data & 0x02) ? (*pDelta - 1) : (*pDelta + 1), &Data))
    			return 2;

    		if(Data & 0x03)
    			return 1;   //Frame sync fail!
    	}
    */
    return 0;
}

/**
* framesync_update_ivs_position
*  This function will reset IV start position for V position move
* @param <ucIV2DVDelay>  {send iv2dvs delay value}
*@return <none>
*/
static void framesync_update_ivs_position(UINT8 ucIV2DVDelay)
{
    UINT32 ivs2dvs_offset = 0; //Lewis 20081020
#ifdef CONFIG_YPBPR_OPEN_H_V_POSITION
    if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA || GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR) // CSW+ Just for VGA mode temporarily
#else
    if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
#endif
    {
        /////////////////////////////////////////////////////
        //update Minimum margin
        ////////////////////////////////////////////////////
        ucIPV_Min_Margin = 0x80 -((ucIV2DVDelay > _V_POSITION_MOVE_TOLERANCE) ?
        (GET_INTERLACE_MODE() ? (_V_POSITION_MOVE_TOLERANCE >> 1) : _V_POSITION_MOVE_TOLERANCE) :
                (GET_INTERLACE_MODE() ? (ucIV2DVDelay >> 1) : ucIV2DVDelay));

        ucIPV_Max_Margin = 0x80 +((ucIV2DVDelay > _V_POSITION_MOVE_TOLERANCE) ?
                                  (GET_INTERLACE_MODE() ? (_V_POSITION_MOVE_TOLERANCE >> 1) : _V_POSITION_MOVE_TOLERANCE) :
                                          (GET_INTERLACE_MODE() ? (ucIV2DVDelay >> 1) : ucIV2DVDelay));

        //update input vertical position
        // CSW+ 0961220 Separate frame sync & FRC mode
//		if (info->FrameSyncFlag)
{
            if (GET_INTERLACE_MODE())
            {
                //Lewis 20081020 modify, if VGIP Vstart < 4, we need add Vstart to prevent framesync error
                //we need adjust IVS2DVSdelay too
                if ((info->IPV_ACT_STA- (ucIV2DVDelay >> 1)) < 4)
                {
                    ivs2dvs_offset = (4 - (info->IPV_ACT_STA - (ucIV2DVDelay >> 1)))<<1;
                    rtdf_maskl(VGIP_CHN1_ACT_VSTA_LENGTH_0x0a1c, 0xf000f000, (((UINT32)((4) & 0x00000fff) << 16) | (info->IPV_ACT_LEN& 0x00000fff)));
                }
                else
                {
                    rtdf_maskl(VGIP_CHN1_ACT_VSTA_LENGTH_0x0a1c, 0xf000f000, (((UINT32)((info->IPV_ACT_STA - (ucIV2DVDelay >> 1)) & 0x00000fff) << 16) | (info->IPV_ACT_LEN & 0x00000fff)));
                }
            }
            else
            {
                //Lewis 20081020 modify, if VGIP Vstart < 4, we need add Vstart to prevent framesync error
                //we need adjust IVS2DVSdelay too
                if ((info->IPV_ACT_STA - (ucIV2DVDelay)) < 4)
                {
                    ivs2dvs_offset = 4 - (info->IPV_ACT_STA- ucIV2DVDelay);
                    rtdf_maskl(VGIP_CHN1_ACT_VSTA_LENGTH_0x0a1c, 0xf000f000, (((UINT32)((4) & 0x00000fff) << 16) | (info->IPV_ACT_LEN & 0x00000fff)));
                }
                else
                {
                    rtdf_maskl(VGIP_CHN1_ACT_VSTA_LENGTH_0x0a1c, 0xf000f000, (((UINT32)((info->IPV_ACT_STA - (ucIV2DVDelay)) & 0x00000fff) << 16) | (info->IPV_ACT_LEN & 0x00000fff)));
                }
            }

        }
//		else
//			rtdf_maskl(VGIP_CHN1_ACT_VSTA_LENGTH_0x0a1c, 0xf000f000, ((((info->IPV_ACT_STA - (ucIV2DVDelay + _PROGRAM_VDELAY)) & 0x00000fff) << 16) | (info->IPV_ACT_LEN & 0x00000fff)));
        ///////////////////////////////////////////////////////////////////
        //transfer the IVS2DVS_delay into IVS_delay
        //#define DDOMAIN_FS_IV_DV_FINE_TUNING_VADDR         (0xb8070020)
        /*
        iv2dv_line	31:24	R/W/DB	0
        	IV to DV Line count
        	The number of input HS from input IVS to DVS.
        	Determine the delay line numbers between IVS and DVS.
        	Double buffer by double buffer write enable (load at input data enable stop point ) of i_domain, (Double buffer by I domain)
        */
        ///////////////////////////////////////////////////////////////////
        //rtdf_maskl(DDOMAIN_FS_IV_DV_FINE_TUNING_VADDR, 0x02ffffff, 0);
        //Lewis 20081020 modify, if VGIP Vstart < 4, we need add Vstart to prevent framesync error
        //we need adjust IVS2DVSdelay too
        if (ivs2dvs_offset)
        {
            rtdf_maskl(FS_IV_DV_FINE_TUNING_0x2020, 0x003fffff, (UINT32)(ivs2dvs_offset&0x3ff)<<22);
        }
        else
        {
            rtdf_maskl(FS_IV_DV_FINE_TUNING_0x2020, 0x003fffff, (UINT32)_IVS2DVS_MEASURE_DELAY<<22);
        }

        ///////////////////////////////////////////////////////////////////
        //update IVS delay
        ///////////////////////////////////////////////////////////////////
        drv_syncproc_ScalerSetVPosition(); //update v position
        /////////////////////////////////////////////////////

        drv_scaler_enable_fs_wdg(_FALSE);//tommy add .for prevent frame sync error
    }


}

void flow_framesync_Clear_StatusFlag(void)
{
    rtdf_clearBits(MAIN_DISPLAY_CONTROL_RSV_0x2100, _BIT18|_BIT19);
}

void flow_framesync_Clear_LineBufferFlag(void)
{
    rtdf_clearBits(DM_UZU_CTRL_0x1604, _BIT25);
}

void adjust_Iv2dvDelay(UINT16 offset)
{
    UINT8	usData=0;

    if (framesync_testsync(offset, &usData))
    {
        RTD_Log(LOGGER_DEBUG, "FrameSyncTestSync error1:  HS/VS wrong assert(b8052414)\n");
    }

    if (usData !=0x00)
    {
        RTD_Log(LOGGER_DEBUG, " ret=%x after test0 : Frame sync not OK after FrameSyncFastDo!!\n", (UINT32)usData);
        drv_adjust_setivs2dvslines(ucIVS2DVSDelay-1);
        if (framesync_testsync(offset, &usData))
        {
            RTD_Log(LOGGER_DEBUG, "FrameSyncTestSync error1:  HS/VS wrong assert(b8052414)\n");
        }
        if (usData == 0)
        {
            ucIVS2DVSDelay = ucIVS2DVSDelay -1;
            return;
        }
        drv_adjust_setivs2dvslines(ucIVS2DVSDelay+1);
        usData = 0;
        if (framesync_testsync(offset, &usData))
        {
            RTD_Log(LOGGER_DEBUG, "FrameSyncTestSync error1:  HS/VS wrong assert(b8052414)\n");
        }
        if (usData == 0)
        {
            ucIVS2DVSDelay = ucIVS2DVSDelay +1;
            return;
        }
        // back to original setting if search not ok
        drv_adjust_setivs2dvslines(ucIVS2DVSDelay);
    }

}

#if 0// not used
void Search_Iv2dvDelay(void)
{
    UINT8 start,end,pass = 0;
    UINT16 i,j,pixel;
    UINT32 value;
    end = ucIVS2DVSDelay +2;
    RTD_Log(LOGGER_DEBUG, "ucIVS2DVSDelay in FrameSync_do = %x\n",(UINT32)ucIVS2DVSDelay);
    for (start = ucIVS2DVSDelay -1; start <= end; start ++)
    {
        //drv_adjust_setivs2dvslines(start);
        flow_timer_Reset_WatchDog();
        pixel = info->IHTotal >>4;
        for (i = 0; i<pixel; i+= 4)
        {
            if (GET_INTERLACE_MODE())
            {
                value = (start+_IVS2DVS_MEASURE_DELAY);
                value = value <<8;
                value = value |i;
                value = value <<8;
                if (i > (info->IHTotal >>5))
                    j = i -(info->IHTotal >>5);
                else
                    j = 0;
                value = value |j;
                value = value <<6;
                value = value | _BIT5;
                rtdf_outl(FS_IV_DV_FINE_TUNING_0x2020, value);
                //rtdf_maskl(FS_IV_DV_FINE_TUNING_0x2020, 0x0000007f, ((iv2dv_line+1)<< 22) | ((iv2dv_16p1+ (info->IHTotal>>4))<< 14) |(iv2dv_16p1<< 6) | _BIT5);
            }
            else
            {

                value =(start+_IVS2DVS_MEASURE_DELAY);
                value = value <<8;
                value = value |i;
                value = value <<14;
                rtdf_maskl(FS_IV_DV_FINE_TUNING_0x2020, 0x0003ffff, value);
            }
            flow_framesync_Clear_StatusFlag();
            fw_timer_event_DelayXms(40);//RtSystem_Sleep(40);
            if (flow_framesync_status()== 0)
            {
                RTD_Log(LOGGER_DEBUG, "ucIVS2DVSDelay = %x , pixel = %x can Frame sync\n",(UINT32)start,(UINT32)i);
            }
            else
            {
                RTD_Log(LOGGER_DEBUG, "ucIVS2DVSDelay = %x , pixel = %x cannot Frame sync\n",(UINT32)start,(UINT32)i);
            }
        }
    }
    // Restore to orignal delay
    drv_adjust_setivs2dvslines(ucIVS2DVSDelay);
}
#endif

//--------------------------------------------------
// Description  : Adjust Dclk offset to meet frame sync
// Input Value  : None
// Output Value : Return 0 if sucess
//--------------------------------------------------
/**
* drvif_framesync_do
*  This function will djust Dclk offset to meet frame sync
* @param <None>
*@return <UINT8>  	{Return 0 if sucess}
*/
UINT8 flow_framesync_do(void)
{

    UINT16 delta;
    UINT16 result = 0;
    UINT8	usData;
	UINT8 CurChn;
    drv_scaler_enable_fs_wdg(_FALSE);

    //drv_adjust_ivs2dvsdelay();
    //rtd_outl(MDOMAIN_DISP_DDR_MAINCTRL_VADDR, _BIT1 | _BIT2 | _BIT0);
    // write data & enable double buffer
    // due to mantis bug:10311, we have to write 3 times to enable double buffer register write
    //rtd_outl(MDOMAIN_DISP_DDR_MAINSTATUS_VADDR , _BIT2 | _BIT1|(GET_10BIT_FORMAT(info->display)?_BIT0:0));
    //rtd_outl(MDOMAIN_DISP_DDR_MAINSTATUS_VADDR , _BIT2 | _BIT1|(GET_10BIT_FORMAT(info->display)?_BIT0:0));
    //rtd_outl(MDOMAIN_DISP_DDR_MAINSTATUS_VADDR , _BIT2 | _BIT1|(GET_10BIT_FORMAT(info->display)?_BIT0:0));

//	drvif_memory_set_dbuffer_write();
//	WaitFor_DEN_STOP();
//	rtdf_setBits(info->channel ? MDOMAIN_DISP_DDR_SUBCTRL_VADDR : MDOMAIN_DISP_DDR_MAINCTRL_VADDR, _BIT0);
    // enable double buffer
//	rtd_outl(info->channel ? MDOMAIN_DISP_DDR_SUBSTATUS_VADDR : MDOMAIN_DISP_DDR_MAINSTATUS_VADDR , _BIT2 | _BIT1|(GET_10BIT_FORMAT(info->display)?_BIT0:0));

//	drvif_memory_set_dbuffer_write();
    // Disable spread spectrum
    drv_adjust_spreadspectrumrange(0);
    //tick system WD, to avoid system reset
    flow_timer_Reset_WatchDog();
// CSW+ 0961225 Disable Fix last line before open it
#if(CONFIG_DISPLAY_LAST_LINE_FINETUNE != _LAST_LINE_METHOD_NONE)
    rtdf_clearBits(DISPLAY_TIMING_CTRL1_0x2000, _BIT27);
#endif
    // Fast Framesync method
    delta = drv_adjust_fast_get_sync_point();
    if (delta == 0xfff)
    {
        RTD_Log(LOGGER_DEBUG, "\nFrame sync Aclkcnt measure timeout !!!!!!!!!!!!!!!\n");
        return 2;
    }
//	drv_pll_Set_DClk_offset(delta);
    // always kick watchdog, even it is not enabled
    //tick system WD, to avoid system reset
    flow_timer_Reset_WatchDog();
    // CSW+ 0980116 [Code sync] Power down DPLL and then power on DPLL to prevent IC DPLL fail
    //rtdf_setBits(PLL_DISP_0_0x2f00, _BIT19); // power down DPLL
    //fw_timer_event_DelayXms(5);
    //rtdf_clearBits(PLL_DISP_0_0x2f00, _BIT19);  // power on DPLL

    //fw_timer_event_DelayXms(10);
    ucIVS2DVSDelay = drv_adjust_measureivs2dvsdelay();
    if (ucIVS2DVSDelay == 0)
    {
        RTD_Log(LOGGER_DEBUG, "\nFrame sync IV2DV delay measure timeout !!!!!!!!!!!!!!!\n");
        return 3;
    }

    adjust_Iv2dvDelay(delta);
    //fw_timer_event_DelayXms(40);
    //Search_Iv2dvDelay();

    RTD_Log(LOGGER_DEBUG, "ucIVS2DVSDelay in FrameSync_do = %x\n",(UINT32)ucIVS2DVSDelay);
    //forster modified 071002 to reset IV start position for V position move in frame sync mode
    framesync_update_ivs_position(ucIVS2DVSDelay);

    RTD_Log(LOGGER_DEBUG, "fast do :ret offset =%x\n", (UINT32)delta);
    // move to measureivs2dvsdelay()
//	rtdf_clearBits(DISPLAY_TIMING_CTRL1_0x2000, _BIT29);
//	drv_scaler_set_display_mode(DISPLAY_MODE_FRAME_SYNC);
//	fw_timer_event_DelayXms(40);

    if (framesync_testsync(delta, &usData))
    {
        RTD_Log(LOGGER_DEBUG, "FrameSyncTestSync error1:  HS/VS wrong assert(b8052414)\n");
    }

#ifdef CONFIG_SCART_AUTO_SWITCH
    if ((usData !=0x00)&&(GET_INPUTSOURCE_TYPE() != _SOURCE_TV)&&(GET_INPUTSOURCE_TYPE() != _SOURCE_FULL_SCART))
#else
    if ((usData !=0x00)&&(GET_INPUTSOURCE_TYPE() != _SOURCE_TV))
#endif
    {
        RTD_Log(LOGGER_DEBUG, " ret=%x after test0 : Frame sync not OK after FrameSyncFastDo!!\n", (UINT32)usData);
        return 1;
    }

    if (usData == 0)
    {
        RTD_Log(LOGGER_DEBUG, " ret=%x after test0 : Frame sync OK after FrameSyncFastDo!!\n", (UINT32)usData);
#if(CONFIG_DISPLAY_LAST_LINE_FINETUNE == _LAST_LINE_METHOD_NONE)
        result |= _BIT0;
        if (framesync_testsync(delta + _OFFSET_STEP, &usData))
        {
            RTD_Log(LOGGER_DEBUG, "FrameSyncTestSync error2:  HS/VS wrong assert(b8052414)\n");
            return 1;
        }

        RTD_Log(LOGGER_DEBUG, "\n ret=%x after test1 : offset + _OFFSET_STEP!!\n", (UINT32)usData);
        if (usData == 0)   result |= _BIT1;

        if (framesync_testsync(delta - _OFFSET_STEP, &usData))
        {
            RTD_Log(LOGGER_DEBUG, "FrameSyncTestSync error3:  HS/VS wrong assert(b8052414)\n");
            return 1;
        }

        RTD_Log(LOGGER_DEBUG, "\n ret=%x after test2 :  offset  - _OFFSET_STEP!!\n", (UINT32)usData);
        if (usData == 0)   result |= _BIT2;

        if ((result & 0x07) != 0x07)
        {
            if (result == (_BIT0 | _BIT1))
            {
                delta += (_OFFSET_STEP / 2);
                RTD_Log(LOGGER_DEBUG, "\n FrameSync test3 : FrameSync Success(offset + _OFFSET_STEP)==>offset value=%x\n",(UINT32)delta);
            }
            else if (result == (_BIT0 | _BIT2))
            {
                delta -= (_OFFSET_STEP / 2);
                RTD_Log(LOGGER_DEBUG, "\n FrameSync test3 : FrameSync Success(offset -_OFFSET_STEP)==>offset value=%x\n",(UINT32)delta);
            }
            drv_pll_Set_DClk_offset(delta);
        }
#endif
    }

#ifdef _VIDEO_TV_SUPPORT
    if (GET_INPUTSOURCE_TYPE() == _SOURCE_TV)
    {
        CurChn = GET_TV_CHN_DATA(_CUR_CHN_TV);
        CTVSetVideoManualMode(GET_CHANNEL_COLORSYSTEM(CurChn)); // reset vd mode after frame sync
        rtdf_clearBits(VDC_VCR_OPTION_0x53c4,_BIT3);
    }
#endif

    RTD_Log(LOGGER_DEBUG, "FrameSync test7 : FrameSync SpreadSpectrum Finetune \n");
    result = framesync_spreadspectrumfinetune(&delta);
    if (result != 0)     return result;

    RTD_Log(LOGGER_DEBUG, "FrameSync test8 : FrameSync LastLine Finetune \n");
    if (!framesync_synclastlinefinetune())
    {
        return 1;
    }

    return 0;
}


// CSW+ 0970306 For fine tunr DVI interlace mode V position in frame sync mode
/*
void flow_framesync_Success_Setting(void)
{
	UINT8 IVS_Delay;

	if ((GET_INPUTSOURCE_FROM() == _SRC_FROM_TMDS) && (!drvif_Hdmi_FormatDetect())){ // DVI mode
		// adjust IVS delay
		IVS_Delay = rtdf_readByte0(VGIP_CHN1_DELAY_0x0a20) + 2; // To compensate the V position to the center
		rtdf_writeByte0(VGIP_CHN1_DELAY_0x0a20, IVS_Delay);
	}

}
*/


