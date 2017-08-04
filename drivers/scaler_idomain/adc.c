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
#include "scaler_idomain\adc.h"
#include "scaler_idomain\measure.h"
#include "scaler\scaler_pll.h"
#include "scaler\adjust.h"
#include "timer_event.h"
#include "scaler_idomain\syncproc.h"
#include "signal\sig_ypbpr.h"
#include "rosprintf.h"
#include "flow_scaler.h"
#include "reg_def.h"
#include "mode.h"
#include "mode_customer.h"
#include "osdfunction.h"
#include "flow_parameter.h"
#include "rt_storage.h"
#include "storage_system.h"
#include "source.h"
#include "ui_struct.h"
#include "scaler_idomain\abl.h"
#include "scaler\scaler.h"
#include "scaler_ddomain\scaler_display.h"

//[Code Sync][AlanLi][0980525][1]
//sync auto adjust flow
#define Pulse_Detection_Enable 1
#define Pulse_Detection_Disable 0

#define  Difference_Threshold 	0x02
//[Code Sync][AlanLi][0980525][1][end]
#define _HORIZONTAL_MARGIN   0x58

static UINT16  usVer_Start = 0;    // Vertical Start
static UINT16  usVer_End = 0;    // Vertical End
static UINT16  usH_Start = 0;    // Horizontal Start
static UINT16  usH_End = 0;    // Horizontal End
//static ADCGainOffset *stVGAGainOffset = NULL;

ADCGainOffset xdata stYPbPrGainOffsetData;
ADCGainOffset2 xdata stScartGainOffsetData;

static AUTO_PARAMETER  stAUTO_PARA;
UINT8 ucAutoControl;
void drv_adc_setcolorgainoffset(UINT8 addr, UINT16 parameter, UINT8 bios)
{
    UINT16 result;

    if (bios & 0x80)
    {
        bios &= 0x7f;
        result = (parameter >= bios) ? parameter - bios : 0;
    }
    else
    {
        result = (parameter >= (0x3ff - bios)) ? 0x3ff : parameter + bios;
    }

    // CSW+
    if (addr == 0x00)  // RED_GAIN
        rtdf_writeWord0(ADC_GAIN0_0x0424, result);
    if (addr == 0x01)  // GREEN_GAIN
        rtdf_writeWord1(ADC_GAIN0_0x0424, result);
    if (addr == 0x02)  // BLUE_GAIN
        rtdf_writeWord0(ADC_GAIN1_0x0428, result);

    if (addr == 0x03)  // RED_OFFSET
        rtdf_writeWord0(ADC_OFFSET0_0x042c, result);
    if (addr == 0x04)  // GREEN_OFFSET
        rtdf_writeWord1(ADC_OFFSET0_0x042c, result);
    if (addr == 0x05)  // BLUE_OFFSET
        rtdf_writeWord0(ADC_OFFSET1_0x0430, result);
}

static UINT8 auto_adcgainoffsetadjust(UINT8 color, UINT16 delta,
                                      UINT8 inc, UINT8 saturate, UINT16* Gain_Offset)
{
    UINT16 usTemp;

    usTemp = inc ? *Gain_Offset : (0x3ff - *Gain_Offset);

    if (inc)
    {
        if (usTemp > delta)
            *Gain_Offset -= delta;
        else
        {
            *Gain_Offset = 0;
            saturate |= color;
        }
    }
    else
    {
        if (usTemp > delta)
            *Gain_Offset += delta;
        else
        {
            *Gain_Offset = 0x3ff;
            saturate |= color;
        }
    }
    return saturate;

}

static void auto_adcgainset(void)
{
    ADCGainOffset ptADCGainOffset;
    UINT8   ucRed = 0,ucGreen = 0,ucBlue = 0, ucSwap = 0;

    if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
        ui_get_VGA_ADCGain(&ptADCGainOffset);
    else if (GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR)
        ui_get_YPbPr_ADCGain(&ptADCGainOffset);
    else
        return;

    ucSwap = (rtdf_readByte1(ADC_CLKGEN_RGB_CAPTURE_0x043c)>>4)  & 0x07;

    if ((ucSwap == _ADC_SWAP_PATH2) || (ucSwap == _ADC_SWAP_PATH3) ||
            (ucSwap == _ADC_SWAP_PATH6) || (ucSwap == _ADC_SWAP_PATH7) )  //Bin -> Rout
        drv_adc_setcolorgainoffset(0x02, ptADCGainOffset.Gain_R, ucRed); // BLUE_GAIN
    else if ((ucSwap == _ADC_SWAP_PATH1) || (ucSwap == _ADC_SWAP_PATH5)) //Gin -> Rout
        drv_adc_setcolorgainoffset(0x01, ptADCGainOffset.Gain_R, ucRed); // GREEN_GAIN
    else
        drv_adc_setcolorgainoffset(0x00, ptADCGainOffset.Gain_R, ucRed); // RED_GAIN

    if ((ucSwap == _ADC_SWAP_PATH4) || (ucSwap == _ADC_SWAP_PATH5)) //Bin -> Gout
        drv_adc_setcolorgainoffset(0x02, ptADCGainOffset.Gain_G, ucGreen); // BLUE_GAIN
    else if ((ucSwap == _ADC_SWAP_PATH1) || (ucSwap == _ADC_SWAP_PATH3)  ||
             (ucSwap == _ADC_SWAP_PATH6) ) //Rin -> Gout
        drv_adc_setcolorgainoffset(0x00, ptADCGainOffset.Gain_G, ucGreen); // RED_GAIN
    else
        drv_adc_setcolorgainoffset(0x01, ptADCGainOffset.Gain_G, ucGreen); // GREEN_GAIN

    if ((ucSwap == _ADC_SWAP_PATH3) || (ucSwap == _ADC_SWAP_PATH4) ||
            (ucSwap == _ADC_SWAP_PATH6)) //Gin -> Bout
        drv_adc_setcolorgainoffset(0x01, ptADCGainOffset.Gain_B, ucBlue); // GREEN_GAIN
    else if ((ucSwap == _ADC_SWAP_PATH2) || (ucSwap == _ADC_SWAP_PATH5) ||
             (ucSwap == _ADC_SWAP_PATH7)) //Rin -> Bout
        drv_adc_setcolorgainoffset(0x00, ptADCGainOffset.Gain_B, ucBlue); // RED_GAIN
    else //Bin -> Bout
        drv_adc_setcolorgainoffset(0x02, ptADCGainOffset.Gain_B, ucBlue); // BLUE_GAIN


}

static UINT8 auto_adcgainchange(UINT8 color, UINT16 delta,UINT8 inc)
{

    ADCGainOffset ptADCGainOffset;
    UINT8 ucTemp = 0;

    if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA) 	// weihao 960308
//#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
        ui_get_VGA_ADCGain(&ptADCGainOffset);
//#else
//		LOAD_STORAGE_STRUCT_DATA(ADCGainOffset1, ptADCGainOffset);
//#endif
    //	ptADCGainOffset = stVGAGainOffset;
    else if (GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR) 	// weihao 960308
//#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
        ui_get_YPbPr_ADCGain(&ptADCGainOffset);
//#else
//		LOAD_STORAGE_STRUCT_DATA(ADCGainOffset, ptADCGainOffset);
//#endif
    //ptADCGainOffset =  stYPbPrGainOffsetData; /*ptYPbPrGainOffsetData;*/

    if (color & SELECT_RED)  //forster modified 050728
    {
        ucTemp = auto_adcgainoffsetadjust(SELECT_RED, delta, inc, ucTemp, (UINT16 *)&ptADCGainOffset.Gain_R);  //forster modified 050905
    }

    if (color & SELECT_GREEN)  //forster modified 050728
    {
        ucTemp |= auto_adcgainoffsetadjust(SELECT_GREEN, delta, inc, ucTemp, (UINT16*)&ptADCGainOffset.Gain_G);   //forster modified 050905
    }

    if (color & SELECT_BLUE)  //forster modified 050728
    {
        ucTemp |= auto_adcgainoffsetadjust(SELECT_BLUE, delta, inc, ucTemp, (UINT16*)&ptADCGainOffset.Gain_B);	//forster modified 050905
    }

    RTD_Log(LOGGER_DEBUG,"\nptADCGainOffset.Gain_R = %x\n",(UINT32)ptADCGainOffset.Gain_R );
    RTD_Log(LOGGER_DEBUG,"ptADCGainOffset.Gain_G = %x\n",(UINT32)ptADCGainOffset.Gain_G );
    RTD_Log(LOGGER_DEBUG,"ptADCGainOffset.Gain_B = %x\n",(UINT32)ptADCGainOffset.Gain_B );


    if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)	// weihao 960308
        ui_set_VGA_ADCGain(&ptADCGainOffset);

    else if (GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR)	// weihao 960308
        ui_set_YPbPr_ADCGain(&ptADCGainOffset);


    auto_adcgainset();

    return ucTemp;

}

static void auto_adcoffsetset(void)
{
    ADCGainOffset ptADCGainOffset;
    UINT8   ucRed = 0, ucBlue = 0, ucGreen = 0, ucSwap = 0;

    if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
        ui_get_VGA_ADCGain(&ptADCGainOffset);
    else if (GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR)
        ui_get_YPbPr_ADCGain(&ptADCGainOffset);
    else
        return;



    if ((ucSwap == _ADC_SWAP_PATH2) || (ucSwap == _ADC_SWAP_PATH3) ||
            (ucSwap == _ADC_SWAP_PATH6) || (ucSwap == _ADC_SWAP_PATH7) )  //Bin -> Rout
        drv_adc_setcolorgainoffset(0x05, ptADCGainOffset.Offset_R, ucRed);  // BLUE_OFFSET
    else if ((ucSwap == _ADC_SWAP_PATH1) || (ucSwap == _ADC_SWAP_PATH5)) //Gin -> Rout
        drv_adc_setcolorgainoffset(0x04, ptADCGainOffset.Offset_R, ucRed); // GREEN_OFFSET
    else
        drv_adc_setcolorgainoffset(0x03, ptADCGainOffset.Offset_R, ucRed); // RED_OFFSET


    if ((ucSwap == _ADC_SWAP_PATH4) || (ucSwap == _ADC_SWAP_PATH5)) //Bin -> Gout
        drv_adc_setcolorgainoffset(0x05, ptADCGainOffset.Offset_G, ucGreen);  // BLUE_OFFSET
    else if ((ucSwap == _ADC_SWAP_PATH1) || (ucSwap == _ADC_SWAP_PATH3) ||
             (ucSwap == _ADC_SWAP_PATH6) ) //Rin -> Gout
        drv_adc_setcolorgainoffset(0x03, ptADCGainOffset.Offset_G, ucGreen);  // RED_OFFSET
    else
        drv_adc_setcolorgainoffset(0x04, ptADCGainOffset.Offset_G, ucGreen); // GREEN_OFFSET

    if ((ucSwap == _ADC_SWAP_PATH3) || (ucSwap == _ADC_SWAP_PATH4)||
            (ucSwap == _ADC_SWAP_PATH6)) //Gin -> Bout
        drv_adc_setcolorgainoffset(0x04, ptADCGainOffset.Offset_B, ucBlue);  // GREEN_OFFSET
    else if ((ucSwap == _ADC_SWAP_PATH2) || (ucSwap == _ADC_SWAP_PATH5) ||
             (ucSwap == _ADC_SWAP_PATH7)) //Rin -> Bout
        drv_adc_setcolorgainoffset(0x03, ptADCGainOffset.Offset_B, ucBlue); // RED_OFFSET
    else //Bin -> Bout
        drv_adc_setcolorgainoffset(0x05, ptADCGainOffset.Offset_B, ucBlue); // BLUE_GAIN

}

static UINT8 auto_adcoffsetchange(UINT8 color, UINT16 delta,UINT8 inc)
{

    ADCGainOffset ptADCGainOffset;
    UINT8 ucTemp = 0;

    if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA) 	// weihao 960308
//#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
        ui_get_VGA_ADCGain(&ptADCGainOffset);
//#else
//		LOAD_STORAGE_STRUCT_DATA(ADCGainOffset1, ptADCGainOffset);
//#endif
    //	ptADCGainOffset = stVGAGainOffset;
    else if (GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR) 	// weihao 960308
//#if defined(_DISABLE_RT_STORAGE_FOR_TEST)
        ui_get_YPbPr_ADCGain(&ptADCGainOffset);
//#else
//		LOAD_STORAGE_STRUCT_DATA(ADCGainOffset, ptADCGainOffset);
//#endif
    //ptADCGainOffset =  stYPbPrGainOffsetData; /*ptYPbPrGainOffsetData;*/

    if (color & SELECT_RED)  //forster modified 050728
    {
        ucTemp = auto_adcgainoffsetadjust(SELECT_RED, delta, inc, ucTemp,  (UINT16 *)&ptADCGainOffset.Offset_R);//forster modified 050905
    }

    if (color & SELECT_GREEN)  //forster modified 050728
    {
        ucTemp |= auto_adcgainoffsetadjust(SELECT_GREEN, delta, inc, ucTemp, (UINT16 *)&ptADCGainOffset.Offset_G);//forster modified 050905
    }

    if (color & SELECT_BLUE)  //forster modified 050728		ucTemp |= auto_adcgainoffsetadjust(SELECT_BLUE, delta, inc, ucTemp, (UINT16 *)&ptADCGainOffset.Offset_B);//forster modified 050905
    {
    }

    if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)	// weihao 960308
        ui_set_VGA_ADCGain(&ptADCGainOffset);//SYNC_STORAGE_STRUCT_DATA(ADCGainOffset1, ptADCGainOffset);
    else if (GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR)	// weihao 960308
        ui_set_YPbPr_ADCGain(&ptADCGainOffset);
    //SYNC_STORAGE_STRUCT_DATA(ADCGainOffset, ptADCGainOffset);

    auto_adcoffsetset();

    return ucTemp;

}

static void  auto_setmeasureboundary(void)
{
    rtdf_outl(AUTO_FIELD_0x0600, stAUTO_PARA.ucFieldControl);

    rtdf_outl(AUTO_H_BOUNDARY_0x0604, ((0x0FFF0000&((UINT32)(stAUTO_PARA.usAutoHStart)<<16))
                                       | (0x00000FFF&(stAUTO_PARA.usAutoHEnd))));

    rtdf_outl(AUTO_V_BOUNDARY_0x0608, ((0x0FFF0000&((UINT32)(stAUTO_PARA.usAutoVStart)<<16))
                                       | (0x00000FFF&(stAUTO_PARA.usAutoVEnd))));
}

static void auto_setnm(void)
{
    UINT8 ucArray[10];

    ucArray[0] = (stAUTO_PARA.ucNMR & 0xfc) | (stAUTO_PARA.ucFieldEnable & 0x03);
    ucArray[1] = (stAUTO_PARA.ucNMG & 0xfc) | (stAUTO_PARA.ucVBoundSearch & 0x03);
    ucArray[2] = (stAUTO_PARA.ucNMB & 0xfc) | (stAUTO_PARA.ucColor & 0x03);

    // CSW+
    //Rspi_Write(_P1_A7_AUTO_R_MARGIN, 3, _Y_INC, ucArray);
    rtdf_writeByte1(AUTO_R_G_B_MARGIN_0x060c,ucArray[2]);  // B
    rtdf_writeByte2(AUTO_R_G_B_MARGIN_0x060c,ucArray[1]);  // G
    rtdf_writeByte3(AUTO_R_G_B_MARGIN_0x060c,ucArray[0]);  // R
}


static UINT8 auto_waitfinish(void)
{

    UINT8 Wait_Time_Cnt;
    UINT8 ucResult;
    // CSW+
    rtdf_clearBits(VGIP_CHN1_STATUS_0x0a14, _BIT12|_BIT13|_BIT14);

    flow_timer_Reset_WatchDog();

    // Wait until auto tracking finishes or timeout
    Wait_Time_Cnt = 200; 		  // Auto-Phase timeout 60ms
    do
    {
        fw_timer_event_DelayXms(10);
        // CSW+
        ucResult  = rtdf_readByte0(AUTO_ADJ_0x0610);

    }
    while ((ucResult & _BIT0) && (--Wait_Time_Cnt));


    ucResult = 0x00;

    ucResult = ucResult & (0x03);

    if (ucResult)
    {
        // CSW+
        rtdf_clearBits(VGIP_CHN1_STATUS_0x0a14, _BIT12|_BIT13|_BIT14);
    }


    // Return non-zero value in Data[0] if :
    // 1. IVS or IHS changed
    // 2. Buffer underflow or overflow
    // 3. Auto-Phase Tracking timeout
    /*
    Data[0] = (Data[0] & 0x6f) ?
    	ERROR_INPUT :
    	(0 == Wait_Time_Cnt) ?
    	ERROR_TIMEOUT :
    	ERROR_SUCCEED;
    	*/
    return (ucResult ? ERROR_INPUT : ((0 == Wait_Time_Cnt) ? ERROR_TIMEOUT : ERROR_SUCCEED));


}


static void auto_getboundaryh(void)
{
    stAUTO_PARA.usAutoHEnd  =  disp_info.ADC_CLOCK + stVGAModeCurrInfo.Clock - 0x80;      // Totol Clock Number
    stAUTO_PARA.usAutoHStart   = (UINT32)stAUTO_PARA.usAutoHEnd * disp_info.IHSyncPulseCount  / disp_info.Hsync + 0x10;   // Clock number in HSYNC pulse

    //forster modified 071211
    if (stAUTO_PARA.usAutoHStart > (stAUTO_PARA.usAutoHEnd >> 1)) //the polarity is negtive
        stAUTO_PARA.usAutoHStart = stAUTO_PARA.usAutoHEnd - stAUTO_PARA.usAutoHStart;

    stAUTO_PARA.usAutoHEnd  = stAUTO_PARA.usAutoHEnd + _MEASURE_HDEALY;
    stAUTO_PARA.usAutoHStart   = (((stAUTO_PARA.usAutoHStart  + _MEASURE_HDEALY) < disp_info.IPH_ACT_STA)) ? (stAUTO_PARA.usAutoHStart + _MEASURE_HDEALY) : (disp_info.IPH_ACT_STA - 0x08);
}

static void auto_getboundaryv(void)
{
    stAUTO_PARA.usAutoVStart = 0x02;
    stAUTO_PARA.usAutoVEnd = disp_info.IVTotal;
}

static UINT32 auto_readinfo()
{
    return rtdf_inl(AUTO_RESULT_PHASE_L_0x0620);
}
static UINT8 auto_waitivs(void)
{
    UINT8 flag, ucTimeout = 5;

    flow_timer_Reset_WatchDog();

    do
    {
        // CSW+ 0960607 for 2622 to dragon
        flag = WaitFor_IVS1();
        ucTimeout --;
    }
    while ((!flag) && (ucTimeout>0));

    if (ucTimeout == 0)
        return _FALSE;
    return _TRUE;
}

static UINT32 auto_phasesearch(UINT8 Sel_Mode, UINT8 Sel_Color, UINT8 Sel_Step, UINT8 Sel_Step_Num, UINT8 Sel_Step_Start, UINT8 *Phase_Result)
{

    UINT32 maxsum, maxsum_tmp;
    UINT16 time_cnt = 0x1000;
    UINT8  count, best, Result;

    rtdf_outl(AUTO_ADJ_0x0610, 0x00);  //reset Auto start

    rtdf_maskl(AUTO_R_G_B_MARGIN_0x060c, ~0x00000300, (0x00000300&((UINT32)(Sel_Color)<<8)));

    rtdf_outl(AUTO_PHASE_CTRL0_0x0624, ( _BIT31 |
                                         (0x0000007F&((UINT32)( Sel_Step_Num - 1)<<0)) |
                                         (0x00C00000&((UINT32)(Sel_Step)<<22)) |
                                         (0x003F0000&((UINT32)(Sel_Step_Start)<<16))

                                       ));
#if 0
    RTD_Log(LOGGER_DEBUG,"\nAUTO_FIELD_0x0600_p = %x !\n",(UINT32)rtdf_inl(AUTO_FIELD_0x0600));
    RTD_Log(LOGGER_DEBUG,"AUTO_H_BOUNDARY_0x0604_p = %x !\n",(UINT32)rtdf_inl(AUTO_H_BOUNDARY_0x0604));
    RTD_Log(LOGGER_DEBUG,"AUTO_V_BOUNDARY_0x0608_p = %x !\n",(UINT32)rtdf_inl(AUTO_V_BOUNDARY_0x0608));
    RTD_Log(LOGGER_DEBUG,"AUTO_R_G_B_MARGIN_0x060c_p = %x !\n",(UINT32)rtdf_inl(AUTO_R_G_B_MARGIN_0x060c));
    RTD_Log(LOGGER_DEBUG,"AUTO_PHASE_CTRL0_0x0624_p = %x !\n",(UINT32)rtdf_inl(AUTO_PHASE_CTRL0_0x0624));
#endif

    if (Sel_Step != 0)
    {
        best = 2;
        for (count = 0; count <Sel_Step-1 ; count++)
        {
            best = best * 2;
        }
    }
    else
    {
        best = 1;
    }

    Sel_Step = best;
    maxsum      = 0;
    best        = 0;

    rtdf_setBits(AUTO_PHASE_CTRL0_0x0624, _BIT7);
    rtdf_writeByte0(AUTO_ADJ_0x0610, (Sel_Mode == 0) ? 0x77 : 0x7b);  //reset Auto start

#if 1

    for (count = 0; count <Sel_Step_Num ; count++)
    {
        while ((!(rtdf_inl(AUTO_PHASE_CTRL0_0x0624)&_BIT7))&&(--time_cnt));

        if (time_cnt==0)
            return 0xffffffff ;

        time_cnt = 0x1000;
        maxsum_tmp = auto_readinfo();
        //RTD_Log(LOGGER_DEBUG,"\nmaxsum[%d]=%x\n",(UINT32)count,(UINT32)maxsum_tmp);
        if (maxsum<maxsum_tmp)
        {
            maxsum = maxsum_tmp;
            best = Sel_Step_Start + Sel_Step * count;
            //RTD_Log(LOGGER_DEBUG,"\n^^^^^^^^^best=%x", best);
        }
        rtdf_setBits(AUTO_PHASE_CTRL0_0x0624, _BIT7);
    }

#else
    if (!auto_waitivs())
        return 0xffffffff ;

    for (count = 0; count <Sel_Step_Num ; count++)
    {
        if (!auto_waitivs())
            return 0xffffffff ;

        maxsum_tmp = auto_readinfo();
        //RTD_Log(LOGGER_DEBUG,"\nmaxsum_tmp[%d] = %x !\n",(UINT32)count,(UINT32)maxsum_tmp);
        if (maxsum<maxsum_tmp)
        {
            maxsum = maxsum_tmp;
            best = Sel_Step_Start + Sel_Step * count;
            //RTD_Log(LOGGER_DEBUG,"\n^^^^^^^^^best=%x", best);
        }

    }
#endif
    if (!auto_waitivs())
        return 0xffffffff ;


    Result = auto_waitfinish();
    if (ERROR_SUCCEED != Result)
        return 0xffffffff ;

    // CSW+
    rtdf_writeByte3(AUTO_PHASE_CTRL0_0x0624, 0x00);
    RTD_Log(LOGGER_DEBUG,"\nbest phase = %d !\n",(UINT32)best);

    *Phase_Result = best;
    return maxsum;

}

//[Code Sync][AlanLi][0980525][1]
//sync auto adjust flow
void drv_auto_clock_fail_handler(UINT8 Curr_PosH, UINT8 Curr_Clock)
{
    RTD_Log(LOGGER_DEBUG,"\n\n**********drvif_auto_clockdo_FAIL!!!***********\n\n");

    if (stVGAModeCurrInfo.Clock != Curr_Clock)
    {
        // Fail to find out suitable clock. Restore original clock and H position.
        stVGAModeCurrInfo.H_Position = Curr_PosH;
        stVGAModeCurrInfo.Clock = Curr_Clock;

        // CSW+ 0961004
        SET_H_POSITION(Curr_PosH);
        SET_CLOCK(Curr_Clock);
        //forster modified 041122
        drv_syncproc_Adjust_ADC_Clock( _TRUE);
    }


}

void drv_auto_phase_fail_handler(UINT8 Curr_Phase)
{
    RTD_Log(LOGGER_DEBUG,"\n\n**********drvif_auto_phasedo_FAIL!!!***********\n\n");


    // Restore Phase
    stVGAModeCurrInfo.Phase = Curr_Phase;

    // CSW+ 0961004
    SET_PHASE(Curr_Phase);


    // check later
    drv_syncproc_YPbPr_SetPhase(Curr_Phase, _TRUE);  //forster modified 071211

}

void drv_auto_position_fail_handler(UINT8 Curr_PosH, UINT8 Curr_PosV, UINT8 Curr_Phase, UINT8 Curr_Clock)
{
    RTD_Log(LOGGER_DEBUG,"\n\n**********drvif_auto_positiondo_FAIL!!!***********\n\n");


    stVGAModeCurrInfo.H_Position = Curr_PosH;	   // Save current stMUD.H_POSITION
    stVGAModeCurrInfo.V_Position = Curr_PosV;	   // Save current stMUD.V_POSITION
    stVGAModeCurrInfo.Clock = Curr_Clock;
    stVGAModeCurrInfo.Phase = Curr_Phase;
    SET_CLOCK(Curr_Clock);
    SET_PHASE(Curr_Phase);
    SET_V_POSITION(stVGAModeCurrInfo.V_Position);
    SET_H_POSITION(stVGAModeCurrInfo.H_Position);

    drv_syncproc_Adjust_ADC_Clock( _TRUE);
    drv_syncproc_YPbPr_SetPhase(Curr_Phase, _TRUE);  //forster modified 071211
    drv_syncproc_ScalerSetHPosition();
    drv_syncproc_ScalerSetVPosition();

}


UINT8 drv_auto_onlin_measure_handler(UINT8 Curr_PosH, UINT8 Curr_PosV, UINT8 Curr_Phase, UINT8 Curr_Clock)
{
    if (!drv_measure_onlinemeasure())
    {
        RTD_Log(LOGGER_DEBUG,"\n\n**********drvif_mode_onlinemeasure_error**********\n\n");
        stVGAModeCurrInfo.H_Position = Curr_PosH;	   // Save current stMUD.H_POSITION
        stVGAModeCurrInfo.V_Position = Curr_PosV;	   // Save current stMUD.V_POSITION
        stVGAModeCurrInfo.Clock = Curr_Clock;
        stVGAModeCurrInfo.Phase = Curr_Phase;
        SET_CLOCK(Curr_Clock);
        SET_PHASE(Curr_Phase);
        SET_V_POSITION(stVGAModeCurrInfo.V_Position);
        SET_H_POSITION(stVGAModeCurrInfo.H_Position);

        CLR_AUTO_CONFIG_DOING();
        COsdFxDisableOsd();
        return _FALSE;
    }
    return _TRUE;

}
//[Code Sync][AlanLi][0980624][1]
UINT8 drv_auto_pre_check(UINT8 *select_result)
{
    UINT8 phase_tmp , Result,select_color,color_path=0xFF;
    UINT32 maxsum=0, maxsum_max=0;
#if 0
    UINT8 result;
    rtdf_outl(AUTO_FIELD_0x0600,0x00000001);
    rtdf_outl(AUTO_H_BOUNDARY_0x0604,0x00940423);
    rtdf_outl(AUTO_V_BOUNDARY_0x0608,0x00020273);
    rtdf_outl(AUTO_R_G_B_MARGIN_0x060c,0x010101002);
    result = auto_waitfinish();
    if (ERROR_SUCCEED != result)
    {
        RTD_Log(LOGGER_DEBUG,"Error 1 result = %x",(UINT32)result);
        return result;
    }
    rtdf_outl(AUTO_ADJ_0x0610,0x00000001);
    result = auto_waitfinish();
    if (ERROR_SUCCEED != result)
    {
        RTD_Log(LOGGER_DEBUG,"Error r result = %x",(UINT32)result);
        return result;
    }
    RTD_Log(LOGGER_DEBUG,"AUTO_RESULT_HSTA_END_0x0618_aa = %x !\n",(UINT32)rtdf_inl(AUTO_RESULT_HSTA_END_0x0618));

#endif


    rtdf_writeByte0(AUTO_R_G_B_MARGIN_0x060c, 0x20);
    for (select_color=0; select_color<3; select_color++)
    {
        flow_timer_Reset_WatchDog();

        Result = drv_adc_measurepositionv(0x50, 0x50, 0x50,select_color);
        if ((ERROR_SUCCEED != Result)||(usVer_Start>150)||(usVer_End>2500))
        {
            RTD_Log(LOGGER_DEBUG,"\n****** drvif_auto_measurepositionv Error result = %x*******\n", (UINT32)Result);
            continue;
        }
        RTD_Log(LOGGER_DEBUG,"\nvor_start:%x  vor_end:%x \n", (UINT32)usVer_Start, (UINT32)usVer_End);

        flow_timer_Reset_WatchDog();

        Result = drv_adc_measurepositionh( _HORIZONTAL_MARGIN,select_color);
        if ((ERROR_SUCCEED != Result)||(usH_Start>1000)||(usH_End>4000))
        {
            RTD_Log(LOGGER_DEBUG,"\n****** drvif_auto_measurepositionh Error result = %x*******\n", (UINT32)Result);
            continue;
        }
        RTD_Log(LOGGER_DEBUG,"\nher_start:%x  her_end:%x \n",(UINT32)usH_Start,(UINT32)usH_End);

        if (select_adc_vco_div==4)
            maxsum  = auto_phasesearch(Pulse_Detection_Enable, select_color, HWAUTO_STEP_8, HWAUTOSTEPNUM(1), HWAUTOSTEPSTART(0), &phase_tmp);
        else
            maxsum  = auto_phasesearch(Pulse_Detection_Enable, select_color, HWAUTO_STEP_4, HWAUTOSTEPNUM(1), HWAUTOSTEPSTART(0), &phase_tmp);
        RTD_Log(LOGGER_DEBUG,"\nselect_color_phase:%x\n",(UINT32)maxsum);
        if ((maxsum == 0xfffffff)||(maxsum == 0))
            continue;
        else
        {
            if ((ABS(tINPUTMODE_PRESET_TABLE[info->ucMode_Curr].IHStartPos,usH_Start)>(tINPUTMODE_PRESET_TABLE[info->ucMode_Curr].IHWidth>>2))||
                    (ABS(tINPUTMODE_PRESET_TABLE[info->ucMode_Curr].IVStartPos,usVer_Start)>(tINPUTMODE_PRESET_TABLE[info->ucMode_Curr].IVHeight>>2)))
                continue;

            if (maxsum>maxsum_max)
            {
                maxsum_max = maxsum;
                color_path = select_color;
            }

        }

    }
    if (color_path==0xFF)
        return ERROR_ABORT;
    else
    {
        rtdf_clearBits(ADC_CLOCK_0x0410, _BIT6); //jump phase
        rtdf_setBits(PLLDIV_0x0508, _BIT14);//set phase through Look-Up-Table
        rtdf_maskl(PLLPHASE_CTRL_0x050c, ~0x7F, (0 & 0x7f));//set the phase
        *select_result = color_path;
        return ERROR_SUCCEED;
    }
}
//[Code Sync][AlanLi][0980624][1][end]


void drv_adc_clock_adjust(UINT8 fast_lock_enable)
{
    UINT16 usTempClock;
    //usTempClock = info->ADC_CLOCK + mode_info.Clock - 0x80;
    RTD_Log(LOGGER_DEBUG,"\nADC setting fast_lock = %x, clock=%x in Syncproc clock\n", (UINT32)fast_lock_enable, (UINT32)GET_CLOCK());
    usTempClock = disp_info.ADC_CLOCK + stVGAModeCurrInfo.Clock - 0x80;


    //updated the ADC IH_Total or the on-line measure will error
    disp_info.IHTotal = usTempClock;  //forster modified 071030
    RTD_Log(LOGGER_DEBUG,"usTempClock=%x\n", (UINT32)usTempClock);
    drv_pll_Set_AClk(usTempClock, fast_lock_enable);

}

//------------------------------------------------------------------//
//  						base on 640x480@60Hz   						//
//------------------------------------------------------------------//
UINT8 drv_auto_few_difference_pattern(UINT32 difference)
{
    UINT8 factor;
    if (disp_info.IPV_ACT_LEN<480)
        factor = 1;
    else
        factor = disp_info.IPV_ACT_LEN/480 + ((disp_info.IPV_ACT_LEN%480)?1:0);
    RTD_Log(LOGGER_DEBUG,"\ndifference=%d", difference);
    RTD_Log(LOGGER_DEBUG,"\nfactor=%d\n", factor);

    if (difference<(0x00170000*factor))
        return _FALSE;
    else
        return _TRUE;
}
//[Code Sync][AlanLi][0980525][1][end]


void drv_adc_setadcgainoffset(void)
{
    //if(GET_INPUTSOURCE_FROM() != _SRC_FROM_ADC)
    //return;

    auto_adcgainset();
    auto_adcoffsetset();

}

void drv_adc_ADCPowerControl(UINT8 on_off)
{
    if (_TRUE == on_off)
    {
        rtdf_setBits(ADC_POWER_0x0400, 0xff);
        rtdf_clearBits(PLL_CTRL_0x0504, _BIT0);	 // Enable APLL & dual clk
    }
    else
    {
        rtdf_clearBits(ADC_POWER_0x0400, _BIT7 | _BIT6 | _BIT5 | _BIT4 | _BIT2 | _BIT1 | _BIT0);
        rtdf_setBits(PLL_CTRL_0x0504, _BIT0);	 // Enable APLL & dual clk
    }
}

void drv_adc_ADC_BangGapPowerControl(UINT8 on_off)
{
    if (_TRUE == on_off)
    {
        rtdf_setBits(ADC_POWER_0x0400, _BIT3);
    }

    else
    {
        rtdf_clearBits(ADC_POWER_0x0400, _BIT3);
    }
}

void drv_adc_SOY0_input_select(UINT8 input_index)
{
    UINT8 adc_input_path;
    GET_Init_YPbPrOptions(input_index, &adc_input_path);
    RTD_Log(LOGGER_DEBUG,"drv_adc_SOY0_input_select = %x\n",(UINT32)adc_input_path);

    if (adc_input_path == ADC_DATA_INPUT0)
    {
        rtdf_maskl(ADC_CLAMP_CTRL0_0x041c, ~(_BIT31|_BIT30|_BIT29|_BIT28), _BIT31|_BIT29|_BIT28);
    }
    else if (adc_input_path == ADC_DATA_INPUT1)
    {
        rtdf_maskl(ADC_CLAMP_CTRL0_0x041c, ~(_BIT31|_BIT30|_BIT29|_BIT28), _BIT31|_BIT30|_BIT29);
    }
}

#if 0 // not used
void drv_adc_SOY1_input_select(UINT8 input_index)
{
    UINT8 adc_input_path;
    GET_Init_YPbPrOptions(input_index, &adc_input_path);
    RTD_Log(LOGGER_DEBUG,"drv_adc_SOY0_input_select = %x\n",(UINT32)adc_input_path);

    if (adc_input_path == ADC_DATA_INPUT0)
    {
        rtdf_maskl(ADC_CLAMP_CTRL0_0x041c,  ~(_BIT27|_BIT26|_BIT25|_BIT24), _BIT27|_BIT25|_BIT24);
    }
    else if (adc_input_path == ADC_DATA_INPUT1)
    {
        rtdf_maskl(ADC_CLAMP_CTRL0_0x041c, ~(_BIT27|_BIT26|_BIT25|_BIT24), _BIT27|_BIT26|_BIT25);
    }
}
#endif

void drv_adc_DCRestore_Ctrl(UINT8 enable, UINT8 input_index)
{
    UINT8 adc_input_path;

    GET_Init_YPbPrOptions(input_index, &adc_input_path);

    if (enable)
    {
        if (adc_input_path == ADC_DATA_INPUT0)
            rtdf_maskl(ADC_DCRESTORE_CTRL_0x0418, ~(_BIT2 | _BIT1 | _BIT0), _BIT2 | _BIT1 | _BIT0);
        else if (adc_input_path== ADC_DATA_INPUT1)
            rtdf_maskl(ADC_DCRESTORE_CTRL_0x0418, ~(_BIT8 | _BIT7 | _BIT6), _BIT8 | _BIT7 | _BIT6);
    }
    else
        rtdf_outl(ADC_DCRESTORE_CTRL_0x0418, 0x00000000);
}

void drv_adc_Clamp_Ctrl(UINT8 output_enable, UINT8 input_index)
{
    UINT8 adc_input_path;

    GET_Init_YPbPrOptions(input_index, &adc_input_path);

    if (output_enable)
    {
        if (adc_input_path == ADC_DATA_INPUT0)
            rtdf_maskl(ADC_CLAMP_CTRL1_0x0420, ~(_BIT2 | _BIT1 | _BIT0), _BIT2 | _BIT1 | _BIT0);
        else if (adc_input_path == ADC_DATA_INPUT1)
            rtdf_maskl(ADC_CLAMP_CTRL1_0x0420, ~(_BIT8 | _BIT7 | _BIT6), _BIT8 | _BIT7 | _BIT6);
    }
    else
        rtdf_outl(ADC_CLAMP_CTRL1_0x0420, 0x00000000);
}

void drv_adc_ADCSelectInput(UINT8 input_index)
{
    UINT32 value=0;
    UINT8 adc_path;
    GET_Init_YPbPrOptions(GET_INPUTSOURCE_INDEX(), &adc_path);

    rtdf_clearBits(ADC_CLKGEN_RGB_CAPTURE_0x043c, _BIT12 | _BIT13 | _BIT14);

    if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
    {
        rtdf_outl(ADC_CTL_0x0414, 0x36404040);
        drv_adc_DCRestore_Ctrl(_ENABLE, GET_INPUTSOURCE_INDEX());
        drv_adc_Clamp_Ctrl(_DISABLE, GET_INPUTSOURCE_INDEX());

        if (adc_path == ADC_DATA_INPUT0)
        {
            rtdf_maskl(ADC_VBIAS_0x040c, ~(0x00000300), (0x00000300&((UINT32)(ADC_DATA_INPUT0)<<8)));
        }
        else if (adc_path == ADC_DATA_INPUT1)
        {
            rtdf_maskl(ADC_VBIAS_0x040c, ~(0x00000300), (0x00000300&((UINT32)(ADC_DATA_INPUT1<<1)<<8)));
        }

    }
    else if (GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR)
    {
        rtdf_outl(ADC_CTL_0x0414, 0xaec040c0);//rtdf_outl(ADC_CTL_0x0414, 0x36404040);
        rtdf_maskl(ADC_CLKGEN_RGB_CAPTURE_0x043c, ~(0xffff),(((_BIT14|_BIT13|_BIT12)&((UINT32)(ADC_1_SWAP_INDEX)<<12))|_BIT10|_BIT6|_BIT5|_BIT4));
        drv_adc_DCRestore_Ctrl(_ENABLE, GET_INPUTSOURCE_INDEX());
        drv_adc_Clamp_Ctrl(_DISABLE, GET_INPUTSOURCE_INDEX());  // change to rtdf_outl(IDOMAIN_YPBPRADC_ADC_CLAMP_CTRL_VADDR , 0x0650570f);

        if (adc_path == ADC_DATA_INPUT0)
        {
            rtdf_maskl(ADC_VBIAS_0x040c, ~(0x00000300), (0x00000300&((UINT32)(ADC_DATA_INPUT0)<<8)));
        }
        else if (adc_path == ADC_DATA_INPUT1)
        {
            rtdf_maskl(ADC_VBIAS_0x040c, ~(0x00000300), (0x00000300&((UINT32)(ADC_DATA_INPUT1<<1)<<8)));
        }
    }

    // Remove compiler warning by weihao
    input_index = 0;
}

//--------------------Measure Vertical Position---------------------//
// Return Message => ERROR_SUCCESS   : Success  					//
//  				 ERROR_INPUT	 : 1. IVS or IHS changed		//
//  								   2. underflow or overflow 	//
//  				 ERROR_TIMEOUT   : Measure Time_Out 			//
//  				 ERROR_NOTACTIVE : No Avtive Image  			//
//------------------------------------------------------------------//
UINT8 drv_adc_measurepositionv(UINT8 NMR_V, UINT8 NMG_V, UINT8 NMB_V, UINT8 color)
{
    UINT8 result;
    UINT8  Data[4];

    auto_getboundaryh();
    auto_getboundaryv();
    stAUTO_PARA.ucFieldControl = 0x00;
    auto_setmeasureboundary();
    stAUTO_PARA.ucFieldEnable = 0;
    stAUTO_PARA.ucVBoundSearch = PIXEL_2;
    stAUTO_PARA.ucColor = color;
    stAUTO_PARA.ucNMR = NMR_V;
    stAUTO_PARA.ucNMG = NMG_V;
    stAUTO_PARA.ucNMB = NMB_V;
    auto_setnm();

    // CSW+
    rtdf_writeByte0(AUTO_ADJ_0x0610,0x01);

    result = auto_waitfinish();
    if (ERROR_SUCCEED != result)
    {
        RTD_Log(LOGGER_DEBUG,"Error 1 result = %x",(UINT32)result);
        return result;
    }

    // CSW+
    Data[0] = rtdf_readByte3(AUTO_RESULT_VSTA_END_0x0614);
    Data[1] = rtdf_readByte2(AUTO_RESULT_VSTA_END_0x0614);
    Data[2] = rtdf_readByte1(AUTO_RESULT_VSTA_END_0x0614);
    Data[3] = rtdf_readByte0(AUTO_RESULT_VSTA_END_0x0614);

    usVer_Start = (((UINT16)(Data[0] & 0x0f ) << 8) | (UINT16)Data[1]);
    usVer_End   = (((UINT16)(Data[2] & 0x0f ) << 8) | (UINT16)Data[3]);
    if (usVer_End == 0x0000)
    {
        RTD_Log(LOGGER_DEBUG,"Error 2 usVer_End == 0x0000 ");
        return  ERROR_ABORT;
    }

    stAUTO_PARA.usAutoVStart = usVer_Start;
    stAUTO_PARA.usAutoVEnd = usVer_End;
    auto_setmeasureboundary();
    return ERROR_SUCCEED;
}

//--------------------Measure Horizontal Position-------------------//
// Return Message => ERROR_SUCCESS   : Success  					//
//  				 ERROR_INPUT	 : 1. IVS or IHS changed		//
//  								   2. underflow or overflow 	//
//  				 ERROR_TIMEOUT   : Measure Time_Out 			//
//  				 ERROR_NOTACTIVE : No Avtive Image  			//
//------------------------------------------------------------------//
#define INTERNAL_H_DELAY     2
UINT8 drv_adc_measurepositionh(UINT8 NM_H, UINT8 color)
{

    UINT8 result;

    auto_getboundaryh();
    stAUTO_PARA.ucFieldControl = 0x00;
    auto_setmeasureboundary();
    stAUTO_PARA.ucFieldEnable = 0;
    stAUTO_PARA.ucVBoundSearch = PIXEL_2;
    stAUTO_PARA.ucColor = color;
    stAUTO_PARA.ucNMR = NM_H;
    stAUTO_PARA.ucNMG = NM_H;
    stAUTO_PARA.ucNMB = NM_H;


    auto_setnm();

    result = auto_waitfinish();
    if (ERROR_SUCCEED != result)
    {
//		RTD_Log(LOGGER_DEBUG,"In Measure_PositionH, Fail 1 !\n");
        return result;
    }

    rtdf_outl(AUTO_ADJ_0x0610,0x00000001); // Start auto tracking

    result = auto_waitfinish();
    if (ERROR_SUCCEED != result)
    {
//		RTD_Log(LOGGER_DEBUG,"In Measure_PositionH, Fail 2 !\n");
        return result;
    }

    usH_Start = ((0x0FFF0000&(UINT32)(rtdf_inl(AUTO_RESULT_HSTA_END_0x0618)))>>16) -INTERNAL_H_DELAY;
    usH_End = (0x00000FFF&(rtdf_inl(AUTO_RESULT_HSTA_END_0x0618))) -INTERNAL_H_DELAY;

    if (usH_End == 0x0000)
    {
//		RTD_Log(LOGGER_DEBUG,"In Measure_PositionH, Fail 3 !\n");
        return  ERROR_ABORT;
    }
    return ERROR_SUCCEED;


}

UINT8 drv_adc_auto_phasedo(UINT8 color)
{
    UINT8  Result, phase, best = 0,phase_tmp;
    UINT32 maxsum, temp0, temp1, temp2,temp_sum;
    UINT8 phase_flag;//64 or 128
    RTD_Log(LOGGER_DEBUG,"\n\n**********drvif_auto_phasedo***********\n\n");

    phase_flag = ((0x00100000&(rtdf_inl(PLLPHASE_CTRL_0x050c)))>>20);//64 or 128

    RTD_Log(LOGGER_DEBUG,"\n phase_flag = %x", (UINT32)phase_flag);
//[Code Sync][AlanLi][0980525][1]
//sync auto adjust flow
    rtdf_maskl(PLLPHASE_CTRL_0x050c, ~0x7F, (0 & 0x7f));//set the phase
    Result = drv_adc_measurepositionh(_HORIZONTAL_MARGIN,color);
    if (ERROR_SUCCEED != Result)
        return Result;

    rtdf_writeByte0(AUTO_R_G_B_MARGIN_0x060c, Difference_Threshold);

    if (phase_flag)
        maxsum  = auto_phasesearch(Pulse_Detection_Enable, color, HWAUTO_STEP_4, HWAUTOSTEPNUM(32), HWAUTOSTEPSTART(0), &phase_tmp);
    else
        maxsum  = auto_phasesearch(Pulse_Detection_Enable, color, HWAUTO_STEP_2, HWAUTOSTEPNUM(32), HWAUTOSTEPSTART(0), &phase_tmp);
//[Code Sync][AlanLi][0980525][1][end]

    RTD_Log(LOGGER_DEBUG,"\nbig range search max result=%x\nbig range search phase result=%d\n", (UINT32)maxsum, (UINT32)phase_tmp);

    if (maxsum == 0xffffffff)
        return ERROR_INPUT;
    if (maxsum == 0)
        return ERROR_ABORT;

    //according large range search result, search phase by precision range
    rtdf_writeByte0(AUTO_R_G_B_MARGIN_0x060c, 0x10);

    if (phase_tmp>=2 && phase_tmp<=(phase_flag ? 126 : 62))
        phase = phase_tmp;
    else if (phase_tmp<2)
        phase = phase_tmp + 2;
    else
        phase = phase_tmp - 2;

//[Code Sync][AlanLi][0980525][1]
//sync auto adjust flow
    maxsum  = auto_phasesearch(Pulse_Detection_Enable, color, HWAUTO_STEP_1, HWAUTOSTEPNUM(4), HWAUTOSTEPSTART(phase-2), &phase_tmp);
//[Code Sync][AlanLi][0980525][1][end]

    RTD_Log(LOGGER_DEBUG,"\nsmall range search max result=%x\nsmall range search phase result=%d\n", (UINT32)maxsum, (UINT32)phase_tmp);

    if (maxsum == 0xffffffff)
        return ERROR_INPUT;
    if (maxsum == 0)
        return ERROR_ABORT;
    if (phase_tmp<3 )
        phase_tmp = phase_tmp + 3;
    else if (phase_tmp> (phase_flag ? 125 : 61) )
        phase_tmp = phase_tmp - 3;

    phase = (phase_tmp - 3);
    RTD_Log(LOGGER_DEBUG,"\nphase_1=%d\n", (UINT32)phase);

    stVGAModeCurrInfo.Phase= phase;

//[Code Sync][AlanLi][0980525][1]
//sync auto adjust flow
    //drv_syncproc_YPbPr_SetPhase(phase, _FALSE); //forster modified 071211
    rtdf_maskl(PLLPHASE_CTRL_0x050c, ~0x7F, (phase & 0x7f));//set the phase
//[Code Sync][AlanLi][0980525][1][end]
    rtdf_clearBits(AUTO_PHASE_CTRL0_0x0624, _BIT31);//software auto phase
    rtdf_maskl(AUTO_ADJ_0x0610,~_BIT0,_BIT0); //reset Auto start

    Result = auto_waitfinish();
    if (ERROR_SUCCEED != Result)
        return Result;
    temp0 = auto_readinfo();

    phase ++;

    RTD_Log(LOGGER_DEBUG,"\nphase_2=%d\n", (UINT32)phase);

    stVGAModeCurrInfo.Phase= phase;

//[Code Sync][AlanLi][0980525][1]
//sync auto adjust flow
    rtdf_maskl(PLLPHASE_CTRL_0x050c, ~0x7F, (phase & 0x7f));//set the phase
//[Code Sync][AlanLi][0980525][1][end]
    rtdf_maskl(AUTO_ADJ_0x0610,~_BIT0,_BIT0); //reset Auto start

    Result = auto_waitfinish();
    if (ERROR_SUCCEED != Result)
        return Result;
    temp1 = auto_readinfo();

    phase_tmp = phase_tmp + 3;

    RTD_Log(LOGGER_DEBUG,"\nphase_tmp=%d\n", (UINT32)phase_tmp);
    maxsum  = 0;
    do
    {
        phase ++;

//[Code Sync][AlanLi][0980525][1]
//sync auto adjust flow
        rtdf_maskl(PLLPHASE_CTRL_0x050c, ~0x7F, (phase & 0x7f));//set the phase
//[Code Sync][AlanLi][0980525][1][end]
        // CSW+
        rtdf_maskl(AUTO_ADJ_0x0610,~_BIT0,_BIT0);

        Result = auto_waitfinish();
        if (ERROR_SUCCEED != Result)
            return Result;
        temp2 = auto_readinfo();


        temp_sum = temp2 + temp1 + temp0
                   - ((temp1 > temp0 ? temp1 - temp0 : temp0 - temp1) / 2)
                   - ((temp1 > temp2 ? temp1 - temp2 : temp2 - temp1) / 2);

        if (temp_sum > maxsum)
        {
            maxsum  = temp_sum;
            best    = (phase - 1);
            RTD_Log(LOGGER_DEBUG,"\nBest phase = %d",(UINT32)best);
        }
        temp0   = temp1;
        temp1   = temp2;
    }
    while (phase != phase_tmp);

    if (phase_flag)
    {
        best = (best >> 1);
    }

    // CSW+ 0961004
    stVGAModeCurrInfo.Phase = best;
    RTD_Log(LOGGER_DEBUG,"\n^^^^^^^^^ptVGAModeCurrInfo->Phase=%d\n", (UINT32)stVGAModeCurrInfo.Phase);
    stScreenModeCurrInfo.Phase = best ;
    drv_syncproc_YPbPr_SetPhase(stVGAModeCurrInfo.Phase, _TRUE);
    RTD_Log(LOGGER_DEBUG,"\nAdjust Phase SUCCEED!!!!");
    return ERROR_SUCCEED;

}

UINT8 drv_adc_auto_positiondo(UINT8 color)
{
    UINT8   Result;
    UINT8   temp;
    UINT16 diff_1,diff_2;

    RTD_Log(LOGGER_DEBUG,"\n\n**********drvif_auto_positiondo**********\n\n");
    RTD_Log(LOGGER_DEBUG,"\n\n**********************************\n\n");
    Result = drv_adc_measurepositionv(0x40, 0x40, 0x40,color);
    if (ERROR_SUCCEED != Result)
        return Result;
    Result = drv_adc_measurepositionh(_HORIZONTAL_MARGIN,color);
    if (ERROR_SUCCEED != Result)
        return Result;

    // CSW+ 0961219 Reset VGA IPH/IPV start position to avoid reseting to wrong value
    //Scaler_DispSetInputInfoByDisp(display,SLR_INPUT_IPH_ACT_STA , Scaler_DispGetInputInfoByDisp(display,SLR_INPUT_IPH_ACT_STA_PRE) - _H_POSITION_MOVE_TOLERANCE );
    disp_info.IPH_ACT_STA = disp_info.IPH_ACT_STA_PRE -_H_POSITION_MOVE_TOLERANCE;

    //if (!Scaler_DispGetInputInfoByDisp(display,SLR_INPUT_FRAMESYNC))
    //   	   Scaler_DispSetInputInfoByDisp(display,SLR_INPUT_IPV_ACT_STA,_PROGRAM_VDELAY);


    RTD_Log(LOGGER_DEBUG,"\nmeasure H_start =%d measure H_end  =%d    |||  measure V_start =%d  measure V_end= %d *******\n", (UINT32)usH_Start, (UINT32)usH_End, (UINT32)usVer_Start, (UINT32)usVer_End);
    RTD_Log(LOGGER_DEBUG,"\ntable H_start =%d    table V_start =%d\n",  (UINT32)disp_info.IPH_ACT_STA,  (UINT32)disp_info.IPV_ACT_STA);

    //---------Calculate Vertical Position-----------
    temp = stVGAModeCurrInfo.V_Position;
    if (usVer_Start <= (disp_info.IPV_ACT_STA+(ucIPV_Max_Margin- 128)))//CSW 0970124 To avoid pattern gen 1152*864 70Hz/1366*768 60 Hz  IV start too far  (disp_info[display].IPV_ACT_STA+(ucIPV_Max_Margin -128))
    {
        if (usVer_Start >= disp_info.IPV_ACT_STA-(128-ucIPV_Min_Margin))
            stVGAModeCurrInfo.V_Position = (usVer_Start -disp_info.IPV_ACT_STA) + 128;
        else
            stVGAModeCurrInfo.V_Position = ucIPV_Min_Margin;
    }
    else
    {
        // CSW+ 0970324 Solve VGA 640*350 85 Hz auto adjust position error
        //if (info->ucMode_Curr == _MODE_640x350_85HZ) // CSW+ 0970219 To prevent VGA _MODE_640x350_85HZ iv start too far
        //	stVGAModeCurrInfo[Scaler_DispGetInputInfoByDisp(display,SLR_INPUT_MODE_CURR) - _MODE_640x350_70HZ].V_Position = ucIPV_Max_Margin;
        //else
        stVGAModeCurrInfo.V_Position = ucIPV_Max_Margin;
    }

    SET_V_POSITION(stVGAModeCurrInfo.V_Position);

    //if (info->FrameSyncFlag == _FALSE)
    //		stVGAModeCurrInfo.V_Position -= ((ucIVS2DVSDelay > _V_POSITION_MOVE_TOLERANCE) ? _V_POSITION_MOVE_TOLERANCE :  ucIVS2DVSDelay);

#if 0//(_ADC_INPUT_TEST)
    // CSW+ 0961004
    stScreenModeCurrInfo.V_Position = stVGAModeCurrInfo.V_Position;


#else
    drv_syncproc_ScalerSetVPosition();
#endif
    fw_timer_event_DelayXms(10);


    //---------Calculate Horizontal Position-----------
    if ((usH_Start - ucIHSDelay) <=  disp_info.IPH_ACT_STA+(ucIPH_Max_Margin -128))
    {
        if ((usH_Start - ucIHSDelay) >= disp_info.IPH_ACT_STA-(128-ucIPH_Min_Margin))
            stVGAModeCurrInfo.H_Position = 128+((usH_Start - ucIHSDelay) -disp_info.IPH_ACT_STA) ;
        else
            stVGAModeCurrInfo.H_Position = ucIPH_Min_Margin;
    }
    else
    {
        stVGAModeCurrInfo.H_Position = ucIPH_Max_Margin;
    }

    flow_timer_Reset_WatchDog();

    diff_1 = ((UINT32)rtdf_inl(VGIP_CHN1_DELAY_0x0a20) >> 16);
    diff_1 = ((diff_1 > _H_POSITION_MOVE_TOLERANCE)? (diff_1-_H_POSITION_MOVE_TOLERANCE) : (_H_POSITION_MOVE_TOLERANCE -diff_1));
    diff_2 = stScreenModeCurrInfo.H_Position;
    diff_2 = ((diff_2 > 0x80)? (diff_2-0x80) : (0x80 -diff_2));
    if (diff_1!=diff_2)
    {
        stVGAModeCurrInfo.H_Position --;
        RTD_Log(LOGGER_DEBUG,"POSITION+1\n");
    }

    // CSW+ 0961004
    stScreenModeCurrInfo.H_Position = stVGAModeCurrInfo.H_Position;
    drv_syncproc_ScalerSetHPosition();


//#ifdef _ENLARGE_V_POSITION_ADJUST_BY_VIO_PATH
//	fw_scaler_set_vgip_captureShift(_MAIN_DISPLAY, (int8_t)(stVGAModeCurrInfo[0].H_Position-0x80), (int8_t)(stVGAModeCurrInfo[0].V_Position-0x80));
//#endif //#ifdef _ENLARGE_V_POSITION_ADJUST_BY_VIO_PATH

    //Scaler_AdjustIHSDelay(display, usH_Start-disp_info[display].IPH_ACT_STA);
    RTD_Log(LOGGER_DEBUG,"\nAdjust Position SUCCEED!!!!");
    return ERROR_SUCCEED;

}

UINT8 drv_adc_auto_clockdo(UINT8 color)
{
    UINT16 usTempClock;
    UINT8   count, delta, stop, Result;
    UINT32 max_value  = 0;
    UINT32 max_value_temp = 0;


    RTD_Log(LOGGER_DEBUG,"");
    RTD_Log(LOGGER_DEBUG,"\n\n**********drvif_auto_clockdo***********\n\n\n\n**********************************\n\n");
    stVGAModeCurrInfo.Clock = 128;
    usTempClock = disp_info.ADC_CLOCK;
    usTempClock = usTempClock & 0xfffc;

    // Willy 091215 Disable HS overflow/overrange watchdog for clock adjust
    rtdf_clearBits(SP_MS1IE_0x0910, _BIT21|_BIT17);

    // CSW+ 0961003
    stScreenModeCurrInfo.Clock = stVGAModeCurrInfo.Clock;

//[Code Sync][AlanLi][0980525][1]
//sync auto adjust flow
    //drv_syncproc_Adjust_ADC_Clock(_TRUE);
    drv_adc_clock_adjust(_FALSE);
//[Code Sync][AlanLi][0980525][1][end]

    delta = disp_info.IPH_ACT_WID * 8 / 10;
    delta = (disp_info.IHTotal -delta) / 2;
    stop    = 0;
    count   = 10;

    flow_timer_Reset_WatchDog();

    do
    {

        Result = drv_adc_measurepositionh(_HORIZONTAL_MARGIN,color);
        if (ERROR_SUCCEED != Result)
            return Result;
        //usH_Start = 291;
        //usH_End = 1333;
        //RTD_Log(LOGGER_DEBUG,"\nactive actual end = %d", (UINT32)usH_End);
        usH_End   = usH_End  - usH_Start + 1;
        RTD_Log(LOGGER_DEBUG,"\nactive actual start = %d\nactive actual region = %d\n", (UINT32)usH_Start, (UINT32)usH_End);
        //RTD_Log(LOGGER_DEBUG,"\nactive table region=%x", disp_info.IPH_ACT_WID);
        if (usH_End < disp_info.IPH_ACT_WID)
        {

            if ((disp_info.IPH_ACT_WID - usH_End) >= (2 * delta))
            {
                stop    = 1;
            }
            else
            {
#if 1
                usH_Start = (UINT32)(disp_info.IPH_ACT_WID - usH_End) * (usTempClock)
                            /(UINT32)usH_End;
                RTD_Log(LOGGER_DEBUG,"\nusH_Start=%d", (UINT32)usH_Start);
                if (usH_Start <= 2)    break;
                usH_Start = (usH_Start + 2) & 0xfffc;
                if ((usH_Start + usTempClock) > (delta + disp_info.IHTotal))
                {
                    stop    = 1;
                }
                else
                {
                    usTempClock += usH_Start;
                }
#endif
            }

        }
        else
        {

            if ((usH_End - disp_info.IPH_ACT_WID) >= (2 * delta))
            {
                stop  = 1;
            }
            else
            {
#if 1
                usH_Start = (UINT32)(usH_End - disp_info.IPH_ACT_WID) * (usTempClock)
                            /(UINT32)usH_End;
                RTD_Log(LOGGER_DEBUG,"\nusH_Start=%d", (UINT32)usH_Start);
                if (usH_Start <= 2)    break;
                usH_Start = (usH_Start + 2) & 0xfffc;

                if ((usTempClock - usH_Start ) < (disp_info.IHTotal - delta ))
                {
                    stop  = 1;
                }
                else
                {
                    usTempClock -= usH_Start;
                }
#endif
            }

        }
        RTD_Log(LOGGER_DEBUG,"\nXXXXXXXXXXXX=%x\n",(UINT32)count);
        if (stop)
            break;


        stVGAModeCurrInfo.Clock = usTempClock - info->ADC_CLOCK + 128;
        // CSW+ 0961003
        stScreenModeCurrInfo.Clock = stVGAModeCurrInfo.Clock;
//[Code Sync][AlanLi][0980525][1]
//sync auto adjust flow
        //drv_syncproc_Adjust_ADC_Clock(_TRUE);
        drv_adc_clock_adjust(_FALSE);
//[Code Sync][AlanLi][0980525][1][end]

    }
    while (--count);
    if ((0 == count ) || (1 == stop))
        return ERROR_ABORT;

    RTD_Log(LOGGER_DEBUG,"\n=========================\nptVGAModeCurrInfo.Clock_after=%x", (UINT32)stVGAModeCurrInfo.Clock);

    // Prevent from 1/2-line moire and smear effect.
    if ((usH_End >= disp_info.IPH_ACT_WID && (usTempClock - disp_info.IHTotal) == 4)
            || (usH_End < disp_info.IPH_ACT_WID && (disp_info.IHTotal - usTempClock) == 4))
    {
        stVGAModeCurrInfo.Clock = 128;
        // CSW+ 0961003
        stScreenModeCurrInfo.Clock = stVGAModeCurrInfo.Clock;

//[Code Sync][AlanLi][0980525][1]
//sync auto adjust flow
        //drv_syncproc_Adjust_ADC_Clock(_TRUE);
        drv_adc_clock_adjust(_FALSE);

//[Code Sync][AlanLi][0980525][1][end]
        Result    = drv_adc_measurepositionh(_HORIZONTAL_MARGIN,color);
        if (Result != ERROR_SUCCEED)
            return Result;
        usH_End   = usH_End  - usH_Start + 1;
    }
//[Code Sync][AlanLi][0980525][1]
//sync auto adjust flow


    if (!((usH_End == disp_info.IPH_ACT_WID)&&(stVGAModeCurrInfo.Clock == 128)))
    {
        // Save 4N clock
        RTD_Log(LOGGER_DEBUG,"\n=====MAX=====\n=============");
        stop = stVGAModeCurrInfo.Clock;
        stVGAModeCurrInfo.Clock += (disp_info.IPH_ACT_WID >= usH_End) ? 4 : 2;
        SET_CLOCK(stVGAModeCurrInfo.Clock);

        //Adjust_ADC_Clock(display, _TRUE);
        drv_adc_clock_adjust(_FALSE);

        count   = stop;
        delta   = 6;    // Initial value must be (N * AUTO_CLOCK_STEP)

        rtdf_writeByte0(AUTO_R_G_B_MARGIN_0x060c, Difference_Threshold); // Set threshold for Clock Search

        do
        {
            RTD_Log(LOGGER_DEBUG,"\nnow max measure clock%x", (UINT32)stVGAModeCurrInfo.Clock);
            fw_timer_event_DelayXms(3);
            Result = drv_adc_measurepositionh(_HORIZONTAL_MARGIN,color);
            if (Result != ERROR_SUCCEED)
                return Result;
            usH_End   = usH_End  - usH_Start + 1;
            if (usH_End > (info->IPH_ACT_WID + 2))
            {
                max_value_temp   = 0;
            }
            else
            {
                //WaitFor_IVS1();
                //WaitFor_IVS1();
                //WaitFor_IVS1();
                WaitFor_IVS1();
                WaitFor_IVS1();
                stop = rtdf_readByte1(PLLDIV_0x0508);
                stop = stop & 0x10;//64 or 128
                if (stop)
                    max_value_temp  = auto_phasesearch(Pulse_Detection_Enable, color, HWAUTO_STEP_4, HWAUTOSTEPNUM(32), HWAUTOSTEPSTART(0), &stop);
                else
                    max_value_temp  = auto_phasesearch(Pulse_Detection_Enable, color, HWAUTO_STEP_4, HWAUTOSTEPNUM(16), HWAUTOSTEPSTART(0), &stop);

                if (max_value_temp == 0xffffffff)
                    return ERROR_INPUT;

                RTD_Log(LOGGER_DEBUG,"\nmax_value_temp=%x\n", (UINT32)max_value_temp);
                if (!(drv_auto_few_difference_pattern(max_value_temp)))
                    break;

            }
            if (max_value < max_value_temp)
            {
                max_value = max_value_temp;
                count = stVGAModeCurrInfo.Clock;
                RTD_Log(LOGGER_DEBUG,"\nmax_value=%x\n", (UINT32)count);

            }
            if (delta == 0x00)
            {
#if 0
                if (stVGAModeCurrInfo.Clock > 128)
                {
                    stVGAModeCurrInfo.Clock = 128;
                    // CSW+ 0961003
                    SET_CLOCK(stVGAModeCurrInfo.Clock);
                    //Adjust_ADC_Clock(display, _TRUE);
                    drv_adc_clock_adjust(_FALSE);
                    continue;
                }
#endif
                break;
            }
            delta -= AUTO_CLOCK_PRECISION;


            stVGAModeCurrInfo.Clock -= AUTO_CLOCK_PRECISION;
            // CSW+ 0961003
            SET_CLOCK(stVGAModeCurrInfo.Clock);
            //Adjust_ADC_Clock(display, _TRUE);
            drv_adc_clock_adjust(_FALSE);
        }
        while (_TRUE);
        //maxval = maxval / 3;
        //stModeUserData.Clock    = (maxval > ((DWORD)stModeInfo.IHWidth << 10)) ? count : stop;

        stVGAModeCurrInfo.Clock = count; //lyc update 2009-04-10,for resolve clock overflow
        // CSW+ 0961003
        //SET_CLOCK(disp_info[display].input_src,stVGAModeCurrInfo[disp_info[display].ucMode_Curr - _MODE_640x350_70HZ].Clock);
        //stVGAModeCurrInfo[disp_info[display].ucMode_Curr - _MODE_640x350_70HZ].Center_Clock = (count>0x80)? (count&0x8f):count;  //lyc update 2009-04-10,for resolve clock overflow
        //SET_CLOCK(disp_info[display].input_src,0x80);

    }
//[Code Sync][AlanLi][0980525][1][end]

    //SET_CLOCK(0x80);
    stScreenModeCurrInfo.Clock = stVGAModeCurrInfo.Clock;

//[Code Sync][AlanLi][0980525][1]
//sync auto adjust flow
    //drv_syncproc_Adjust_ADC_Clock(_TRUE);
    drv_adc_clock_adjust(_FALSE);
//[Code Sync][AlanLi][0980525][1][end]
    // Willy 091215 Enable HS overflow/overrange watchdog for clock adjust
    rtdf_setBits(SP_MS1IE_0x0910, _BIT21|_BIT17);
    RTD_Log(LOGGER_DEBUG,"\nAdjust Clock SUCCEED count=%x\n!!!!",(UINT32)count);
    return ERROR_SUCCEED;

}

UINT8 drv_adc_measurecolor(UINT8 color, UINT8 margin, UINT16* rev)
{
    UINT16 ucTemp;


    // CSW+
    UINT8 AutoBMargin;
    AutoBMargin = rtdf_readByte1(AUTO_R_G_B_MARGIN_0x060c) & 0xfc;
    AutoBMargin |= (SELECT_RED == color) ? _COLORS_RED : (SELECT_GREEN == color) ? _COLORS_GREEN : _COLORS_BLUE;
    rtdf_writeByte1(AUTO_R_G_B_MARGIN_0x060c,AutoBMargin);

    stAUTO_PARA.usAutoHStart = disp_info.IPH_ACT_STA + ucIHSDelay;
    stAUTO_PARA.usAutoHEnd = disp_info.IPH_ACT_STA + disp_info.IPH_ACT_WID;

    if (margin == _COLOR_MIN)
    {
        stAUTO_PARA.usAutoVStart = 9;
        stAUTO_PARA.usAutoVEnd = stAUTO_PARA.usAutoVStart + 4;
    }
    else
    {
        stAUTO_PARA.usAutoVStart = disp_info.IPV_ACT_STA + ucIVS2DVSDelay;
        stAUTO_PARA.usAutoVEnd = disp_info.IPV_ACT_STA + disp_info.IPV_ACT_LEN;
    }


    stAUTO_PARA.ucFieldControl = 0;

    auto_setmeasureboundary();

    if (_COLOR_MIN == margin)
        // CSW+
        rtdf_writeByte0(AUTO_ADJ_0x0610,(_COLOR_MIN | (0x41)));
    else
        // CSW+
        rtdf_writeByte0(AUTO_ADJ_0x0610,(_COLOR_MAX | (0x41)));

    //forster modified 041112
    if (auto_waitfinish())
    {
        RTD_Log(LOGGER_DEBUG,"\nMeasure Error Abort%c\n");
        return ERROR_ABORT;
    }

    ucTemp = rtdf_inl(AUTO_RESULT_PHASE_L_0x0620);

    *rev = ucTemp;
    *rev &= 0x03ff;

    if (_COLOR_MIN == margin)
        *rev ^= 0x3ff;

    RTD_Log(LOGGER_DEBUG,"\n Measure Color=%x type=%x Margin = %x",(UINT32)color,(UINT32)margin,(UINT32)*rev);
    return ERROR_SUCCEED;
}

/**
* drv_adc_auto config
*  This function will measure adc signal h,v position and phase and clock
* @param <display>       { 0:MAIN_DISPLAY;1:SUB_DISPLAY}
*@return <result>  	 { 0:ERROR_SUCCEED;not 0:FAIL}
*/
UINT8 drv_adc_auto_config(void)
{
    UINT8 Result, Curr_PosH, Curr_PosV, Curr_Clock, Curr_Phase,select_color;
    // CSW+
    rtdf_outl(AUTO_ADJ_0x0610, 0x00);  //Auto start
    rtdf_outl(AUTO_PHASE_CTRL0_0x0624, 0x00);  // Switch back to software auto phase

//[Code Sync][AlanLi][0980525][1]
//sync auto adjust flow
    if (drv_auto_pre_check(&select_color)!=ERROR_SUCCEED)
        return ERROR_ABORT;

    Curr_PosH = stVGAModeCurrInfo.H_Position;     // Save current stMUD.H_POSITION
    Curr_PosV = stVGAModeCurrInfo.V_Position;     // Save current stMUD.V_POSITION
    Curr_Clock = stVGAModeCurrInfo.Clock;   	   // Save current stMUD.CLOCK
    Curr_Phase = stVGAModeCurrInfo.Phase;   	   // Save current stMUD.PHASE

    drv_scaler_enable_fs_wdg(_FALSE);
    drv_scaler_enable_measure_wdg(_TRUE);
    SET_AUTO_CONFIG_DOING();

    //---------Auto_Clock-----------
    Result = drv_adc_auto_clockdo(select_color);

    flow_timer_Reset_WatchDog();
    rtdf_writeByte0(SP_MS1STUS_0x090c, (_BIT4|_BIT3|_BIT1|_BIT0)); //clear over range, polarity change status
    if (!drv_auto_onlin_measure_handler(Curr_PosH,Curr_PosV,Curr_Phase,Curr_Clock))
    {
    	RTD_Log(LOGGER_LEVEL,"\n Tim bg1 \n");
        drv_scalerdisplay_set_bg_color(0,0,0xff);
        drv_scalerdisplay_ForceBackground(_TRUE);
        drv_scaler_set_display_mode(0);
        return ERROR_INPUT;
    }

    if (ERROR_SUCCEED != Result)
    {
        drv_auto_clock_fail_handler(Curr_PosH,Curr_Clock);
        CLR_AUTO_CONFIG_DOING();
        drv_scaler_enable_fs_wdg(_TRUE);
        return Result;
    }

    //---------Auto_Phase-----------
    Result = drv_adc_auto_phasedo(select_color);

    flow_timer_Reset_WatchDog();

    rtdf_writeByte0(SP_MS1STUS_0x090c, (_BIT4|_BIT3|_BIT1|_BIT0)); //clear over range, polarity change status
    if (!drv_auto_onlin_measure_handler(Curr_PosH,Curr_PosV,Curr_Phase,Curr_Clock))
    {
   		RTD_Log(LOGGER_LEVEL,"\n Tim bg2 \n");
        drv_scalerdisplay_set_bg_color(0,0,0xff);
        drv_scalerdisplay_ForceBackground(_TRUE);
        drv_scaler_set_display_mode(0);
        return ERROR_INPUT;
    }

    if (ERROR_SUCCEED != Result )
    {
        drv_auto_phase_fail_handler(Curr_Phase);
        if (ERROR_NOTACTIVE == Result)
            Result = ERROR_SUCCEED;
        CLR_AUTO_CONFIG_DOING();
        drv_scaler_enable_fs_wdg(_TRUE);
        return Result;
    }


    //---------Auto_Position-----------
    Result = drv_adc_auto_positiondo(select_color);

    flow_timer_Reset_WatchDog();

    rtdf_writeByte0(SP_MS1STUS_0x090c, (_BIT4|_BIT3|_BIT1|_BIT0)); //clear over range, polarity change status
    if (!drv_auto_onlin_measure_handler(Curr_PosH,Curr_PosV,Curr_Phase,Curr_Clock))
    {
    	RTD_Log(LOGGER_LEVEL,"\n Tim bg3 \n");
        drv_scalerdisplay_set_bg_color(0,0,0xff);
        drv_scalerdisplay_ForceBackground(_TRUE);
        drv_scaler_set_display_mode(0);
        return ERROR_INPUT;
    }

    if (ERROR_SUCCEED != Result)
    {
        drv_auto_position_fail_handler(Curr_PosH,Curr_PosV,Curr_Phase,Curr_Clock);
        CLR_AUTO_CONFIG_DOING();
        drv_scaler_enable_fs_wdg(_TRUE);
        return Result;
    }
    else
    {
        rtdf_writeByte0(SP_MS1STUS_0x090c, (_BIT4|_BIT3|_BIT1|_BIT0)); //clear over range, polarity change status
        if (!drv_auto_onlin_measure_handler(Curr_PosH,Curr_PosV,Curr_Phase,Curr_Clock))
            return ERROR_INPUT;
    }

    auto_waitivs();

    drv_scaler_enable_fs_wdg(_TRUE);


    CLR_AUTO_CONFIG_DOING();
    return ERROR_SUCCEED;
//[Code Sync][AlanLi][0980525][1][end]

}

#define _AUTO_VGA_MAX_LEVEL                          (968)//242
#define _AUTO_VGA_MIN_LEVEL                           (8)// 2
#define _AUTO_COMPONENT_Y_MAX_LEVEL         (1016)//250
#define _AUTO_COMPONENT_Y_MIN_LEVEL         (48)//12
#define _AUTO_COMPONENT_PB_MAX_LEVEL       (960)//240
#define _AUTO_COMPONENT_PR_MAX_LEVEL       (960)//240
#define _AUTO_VIDEO8_Y_MAX_LEVEL                 (248)
#define _AUTO_VIDEO8_Y_MIN_LEVEL                 (2)//12
#define _AUTO_VIDEO8_C_MAX_LEVEL                 (236) //this value can't over 240
#define _CHROMA_BLACK_LEVEL             (0x200)

void drv_adc_gettargetvalue(UINT16* max_value, UINT16* min_value, UINT8 color)
{
    flow_timer_Reset_WatchDog();

    switch (GET_INPUTSOURCE_TYPE()) 	// weihao 960308
    {
    case _SOURCE_VGA: 	// weihao 960308
        *max_value = _AUTO_VGA_MAX_LEVEL;
        *min_value = _AUTO_VGA_MIN_LEVEL;
        break;
    case _SOURCE_YPBPR: 	// weihao 960308
        if (color == SELECT_GREEN)//_GREEN)
        {
            *max_value = _AUTO_COMPONENT_Y_MAX_LEVEL;
            *min_value = _AUTO_COMPONENT_Y_MIN_LEVEL;
        }
        else
        {
            //*max_value = (color == _RED) ? _AUTO_COMPONENT_PR_MAX_LEVEL : _AUTO_COMPONENT_PB_MAX_LEVEL;
            *max_value = (color == SELECT_RED) ? _AUTO_COMPONENT_PR_MAX_LEVEL : _AUTO_COMPONENT_PB_MAX_LEVEL;
            *min_value = _CHROMA_BLACK_LEVEL;
        }
        break;

#if(_VIDEO_AUTO_WHITE_BLANCE == _ENABLE)
    case _SOURCE_CVBS: 	// weihao 960308
    case _SOURCE_SV: 	// weihao 960308
//		case _SOURCE_SCART_AV:	//jj 960505
//		case _SOURCE_SCART_SV:	//jj 960505
        if (color == SELECT_GREEN)
        {
            *max_value = _AUTO_VIDEO8_Y_MAX_LEVEL;
            *min_value = _AUTO_VIDEO8_Y_MIN_LEVEL;
        }
        else
        {
            *max_value = _AUTO_VIDEO8_C_MAX_LEVEL;
            *min_value = _CHROMA_BLACK_LEVEL;
        }
        break;
#endif

    }
}

//--------------------------------------------------
// Description  : Adjust ADC gain
// Input Value  : Adjusting setting
// Output Value : Measure status
//--------------------------------------------------
UINT8 drv_adc_tunedevicegain(UINT8 ucColor, UINT16 *pMargin, UINT16 ucMax_Target)
{
    UINT16 temp;

    if (drv_adc_measurecolor(ucColor, _COLOR_MAX, pMargin) != ERROR_SUCCEED)
    {
        RTD_Log(LOGGER_DEBUG,"\nError ABORT 0%c",'!');
        return ERROR_ABORT;
    }

    if (*pMargin > ucMax_Target)
    {
        temp  = *pMargin - ucMax_Target;
        if (auto_adcgainchange(ucColor, temp, 0))                 // Decrease Gain; Decrease Contrast
        {
            if (auto_adcoffsetchange(ucColor, ( (GET_INPUTSOURCE_FROM() == _SRC_FROM_VDC) ? 8 : 16), 0))              // Increase Offset; Decrease Brightness
                return ERROR_FINISH;
        }
    }
    else if (*pMargin < ucMax_Target)
    {
        temp  = ucMax_Target - *pMargin;
        // Non-zero return value of auto_adcgainchange() means ADC gain reaches minimum.
        if (auto_adcgainchange(ucColor, temp, 1))                 // Increase Gain; Increase Contrast
        {
            if (auto_adcoffsetchange(ucColor, ((GET_INPUTSOURCE_FROM()== _SRC_FROM_VDC) ? 8 : 16), 1))              // Increase Offset; Increase Brightness
                return ERROR_FINISH;
        }
    }
    return ERROR_SUCCEED;//_ERROR_SUCCESS;
}



//--------------------------------------------------
// Description  : Adjust ADC offset
// Input Value  : Adjusting setting
// Output Value : Measure status
//--------------------------------------------------
UINT8 drv_adc_tunedeviceoffset(UINT8 ucColor, UINT16 *pMargin, UINT16 ucMin_Target)
{
    UINT16 temp;

    if (drv_adc_measurecolor(ucColor, _COLOR_MIN, pMargin) != ERROR_SUCCEED)
    {
        RTD_Log(LOGGER_DEBUG,"\nError ABORT \n");
        return ERROR_ABORT;
    }

    if ((GET_INPUTSOURCE_TYPE() == _SOURCE_VGA) || 	// weihao 960308
            ((GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR) && (ucColor == SELECT_Y)) ||  	// weihao 960308
            ((GET_INPUTSOURCE_FROM() == _SRC_FROM_VDC)&&(ucColor == SELECT_GREEN)))
    {

        //forster modified 061002
        if (*pMargin > ucMin_Target)
        {

            temp = *pMargin - ucMin_Target;
            RTD_Log(LOGGER_DEBUG,"\nColor= %x, Offset0 BIOS = %x",(UINT32)ucColor, (UINT32)temp);
            auto_adcoffsetchange(ucColor, ((temp > 32) ? 32 : temp), 0);

        }
        else if (*pMargin < ucMin_Target)
        {

            temp = ucMin_Target - *pMargin;
            RTD_Log(LOGGER_DEBUG,"\nColor= %x, Offset1 BIOS = %x",(UINT32)ucColor, (UINT32)temp);

            if (*pMargin == 0)
                auto_adcoffsetchange(ucColor, temp + 16, 1);       // Increase Offset; Increase Brightness //Change_Device_Offset(ucColor, temp + 4, 1);       // Increase Offset; Increase Brightness
            else
                auto_adcoffsetchange(ucColor, temp, 1);           // Increase Offset; Increase Brightness
        }

    }
    else
    {

        if (*pMargin > _CHROMA_BLACK_LEVEL)
        {
            temp = *pMargin - _CHROMA_BLACK_LEVEL;
            auto_adcoffsetchange(ucColor, ((temp > 32) ? 32 : temp), 0);
        }
        else if (*pMargin < _CHROMA_BLACK_LEVEL)
        {

            temp = _CHROMA_BLACK_LEVEL - *pMargin;

            if (*pMargin == 0)
            {
                auto_adcoffsetchange(ucColor, temp + 16, 1);
            }
            else
            {
                auto_adcoffsetchange(ucColor, temp, 1);
            }
        }
    }
    return ERROR_SUCCEED;
}

// [Code Sync][CSW][0980806][1] CSW+ 0980806
#define YPBPR_AUTO_PHASE_SELECT_R 0
#define YPBPR_AUTO_PHASE_SELECT_B 1
#define YPBPR_AUTO_PHASE_SELECT_G 2
#define YPBPR_AUTO_PHASE_THRESHOLD 0x030592fa

UINT8 drv_adc_YPbPr_Auto_Phase(void)
{
    UINT8 phase, best = 0,phase_tmp, Result;
    UINT32 maxsum, temp0, temp1, temp2,temp_sum;
    UINT8 phase_flag;//64 or 128


    if (GET_INPUTSOURCE_TYPE() != _SOURCE_YPBPR)
        return ERROR_ABORT;

    RTD_Log(LOGGER_DEBUG,"\n\n**********YPbPr Auto_Phase_Do***********\n\n");

    flow_timer_Reset_WatchDog();

    Result = drv_adc_measurepositionh(0x80,YPBPR_AUTO_PHASE_SELECT_G);
    Result = drv_adc_measurepositionv(0x80, 0x80, 0x80,YPBPR_AUTO_PHASE_SELECT_G);

    if (ERROR_SUCCEED != Result)
        return Result;

    if ((usH_Start == 0) || (usH_End == 0))
        return ERROR_ABORT;

    //fw_timer_event_DelayXms(20);

    rtdf_clearBits(ADC_CLOCK_0x0410, _BIT6); //jump phase
    rtdf_setBits(PLLDIV_0x0508, _BIT14);//set phase through Look-Up-Table
    rtdf_maskl(PLLPHASE_CTRL_0x050c, ~0x7F, (0 & 0x7f));//set the phase
    WaitFor_DEN_STOP();
    phase_flag = rtdf_readByte1(PLLDIV_0x0508);
    phase_flag = phase_flag & 0x10;//64 or 128

    //ROSPrintf("\n phase_flag = %x", phase_flag);

    rtdf_writeByte0(AUTO_R_G_B_MARGIN_0x060c, Difference_Threshold);

    if (phase_flag)
        maxsum  = auto_phasesearch(Pulse_Detection_Enable, YPBPR_AUTO_PHASE_SELECT_G, HWAUTO_STEP_4, HWAUTOSTEPNUM(32), HWAUTOSTEPSTART(0), &phase_tmp);
    else
        maxsum  = auto_phasesearch(Pulse_Detection_Enable, YPBPR_AUTO_PHASE_SELECT_G, HWAUTO_STEP_4, HWAUTOSTEPNUM(16), HWAUTOSTEPSTART(0), &phase_tmp);


    RTD_Log(LOGGER_DEBUG, "\nbig range search max result=%x", maxsum);
    RTD_Log(LOGGER_DEBUG, "\nbig range search phase result=%d\n", (UINT32)phase_tmp);
    //if((maxsum <= YPBPR_AUTO_PHASE_THRESHOLD) || (maxsum == 0xffffffff)) // not dot morrei pattern(0x08000000)
    //return ERROR_INPUT;
#if 0
    for (phase=0; phase<64; phase++)
    {
        rtdf_maskl(PLLPHASE_CTRL_0x050c, ~0x7F, (phase & 0x7f));
        rtdf_maskl(AUTO_ADJ_0x0610,~_BIT0,_BIT0);
        Result = auto_waitfinish();
        if (ERROR_SUCCEED != Result)
            return Result;
        temp2 = auto_readinfo();
        RTD_Log(LOGGER_DEBUG,"\nmanual[%d]=%x\n",(UINT32)phase, (UINT32)temp2);
    }
#endif

    if (phase_tmp>=2 && phase_tmp<=(phase_flag ? 126 : 62))
        phase = phase_tmp;
    else if (phase_tmp<2)
        phase = phase_tmp + 2;
    else
        phase = phase_tmp - 2;

    maxsum  = auto_phasesearch(Pulse_Detection_Enable, YPBPR_AUTO_PHASE_SELECT_G, HWAUTO_STEP_1, HWAUTOSTEPNUM(4), HWAUTOSTEPSTART(phase-2), &phase_tmp);

    RTD_Log(LOGGER_DEBUG, "\nsmall range search max result=%x", maxsum);
    RTD_Log(LOGGER_DEBUG,"\nsmall range search phase result=%d\n", (UINT32)phase_tmp);

    //if((maxsum <= YPBPR_AUTO_PHASE_THRESHOLD) || (maxsum == 0xffffffff)) // not dot morrei pattern
    //return ERROR_INPUT;

    if (phase_tmp<3 )
        phase_tmp = phase_tmp + 3;
    else if (phase_tmp> (phase_flag ? 125 : 61) )
        phase_tmp = phase_tmp - 3;

    phase = (phase_tmp - 3);
    //ROSPrintf("\nphase_1=%x\n", phase);

    rtdf_maskl(PLLPHASE_CTRL_0x050c, ~0x7F, (phase & 0x7f));//set the phase
    rtdf_clearBits(AUTO_PHASE_CTRL0_0x0624, _BIT31);//software auto phase
    rtdf_maskl(AUTO_ADJ_0x0610,~_BIT0,_BIT0); //reset Auto start

    Result = auto_waitfinish();
    if (ERROR_SUCCEED != Result)
        return Result;
    temp0 = auto_readinfo();

    phase ++;


    //ROSPrintf("\nphase_2=%x\n", phase);

    rtdf_maskl(PLLPHASE_CTRL_0x050c, ~0x7F, (phase & 0x7f));//set the phase

    rtdf_maskl(AUTO_ADJ_0x0610,~_BIT0,_BIT0); //reset Auto start

    Result = auto_waitfinish();
    if (ERROR_SUCCEED != Result)
        return Result;
    temp1 = auto_readinfo();

    phase_tmp = phase_tmp + 3;

    //ROSPrintf("\nphase_tmp=%x\n", phase_tmp);
    maxsum  = 0;
    do
    {
        phase ++;
        rtdf_maskl(PLLPHASE_CTRL_0x050c, ~0x7F, (phase & 0x7f));//set the phase
        rtdf_maskl(AUTO_ADJ_0x0610,~_BIT0,_BIT0);

        Result = auto_waitfinish();
        if (ERROR_SUCCEED != Result)
            return Result;
        temp2 = auto_readinfo();


        temp_sum = temp2 + temp1 + temp0
                   - ((temp1 > temp0 ? temp1 - temp0 : temp0 - temp1) / 2)
                   - ((temp1 > temp2 ? temp1 - temp2 : temp2 - temp1) / 2);

        if (temp_sum > maxsum)
        {
            maxsum  = temp_sum;
            best    = (phase - 1);
            //ROSPrintf("\nBest phase = %x",best);
        }
        temp0   = temp1;
        temp1   = temp2;
    }
    while (phase != phase_tmp);

    if (phase_flag)
    {
        best = (best >> 1);
        //ROSPrintf("\nBest phase1 = %x",best);
    }
    if (GET_INPUT_MODE()<=_MODE_576I)//need debug 090911
    {
        best = best+32;
        if (best>=64)
            best = best-64;
    }
    //stVGAModeCurrInfo.Phase = best;
    //SET_PHASE(best);
    drv_syncproc_YPbPr_SetPhase(best, _TRUE);
    RTD_Log(LOGGER_DEBUG, "\n^^^^^^^^^Ypbpr Adjust Phase=%d\n", (UINT32)best);
    RTD_Log(LOGGER_DEBUG,"\nYpbpr Adjust Phase SUCCEED!!!!");
    return ERROR_SUCCEED;

}


// [Code Sync][CSW][0980806][1] end

/**
* fwif_adc_autobalance
*  This function will Tune ADC gain and offset and do auto balance
* @param <display>       { 0:MAIN_DISPLAY;1:SUB_DISPLAY}
*@return <result>  	 { 0:ERROR_SUCCEED;not 0:FAIL}
*/
UINT8 drv_adc_autobalance(void)
{
    UINT16 rev, result0, result1, color, count;
    UINT16 usMax_Value, usMin_Value;
    ADCGainOffset ptADCGainOffset;

    RTD_Log(LOGGER_DEBUG,"\nAuto color progressing %c",'!');

    drvif_abl_disable();

    ptADCGainOffset.Gain_B = 512;//0x80;
    ptADCGainOffset.Gain_G = 512;//0x80;
    ptADCGainOffset.Gain_R = 512;//0x80;
    ptADCGainOffset.Offset_B = 512;//0x80;
    ptADCGainOffset.Offset_G = 512;//0x80;
    ptADCGainOffset.Offset_R = 512;//0x80;
    ui_set_VGA_ADCGain(&ptADCGainOffset);
    drv_adc_setadcgainoffset();

    if (drv_adc_measurecolor(SELECT_GREEN, _COLOR_MAX, &rev) != ERROR_SUCCEED)
        return ERROR_ABORT;
    if (rev < 0x180)
        return ERROR_ABORT;

    if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
    {
        if (drv_adc_measurecolor(SELECT_BLUE, _COLOR_MAX, &rev) != ERROR_SUCCEED)
            return ERROR_ABORT;

        if (rev < 0x180)
            return ERROR_ABORT;
        if (drv_adc_measurecolor(SELECT_RED, _COLOR_MAX, &rev) != ERROR_SUCCEED)
            return ERROR_ABORT;

        if (rev < 0x180)
            return ERROR_ABORT;
    }

    if (GET_INPUTSOURCE_FROM() == _SRC_FROM_VDC)
        color = SELECT_GREEN;// _GREEN;			  //we don't measure the blue color in VIDEO8 auto color
    else
        color = SELECT_BLUE;//_BLUE;

    while (_TRUE)
    {
        count = 0x30;
        drv_adc_gettargetvalue(&usMax_Value, &usMin_Value, color);
        do
        {
            RTD_Log(LOGGER_DEBUG,"\nauto color count = %d",(UINT32)count);
            rev = drv_adc_tunedevicegain(color, &result0, usMax_Value);

            if (rev == ERROR_ABORT)
                return rev;
            if (rev == ERROR_FINISH)
                break;

            rev = drv_adc_tunedeviceoffset(color, &result1, usMin_Value);

#if(_VIDEO_AUTO_WHITE_BLANCE == _ENABLE)
            if ((GET_INPUTSOURCE_FROM()== _SRC_FROM_VDC)
        {
            //Chroma doesn't need to adjust the offset
            if (color != SELECT_GREEN)//_GREEN)
                    result1 = usMin_Value;
            }
#endif
            if (rev == ERROR_ABORT)
            return rev;
            RTD_Log(LOGGER_DEBUG,"\nresult0 = %x   usMax_Value = %x  result1 = %x  usMin_Value = %x \n",(UINT32)result0,(UINT32)usMax_Value,(UINT32)result1,(UINT32)usMin_Value);

            if ((ABS(result0, usMax_Value) < 2) && (ABS(result1, usMin_Value) < 2))
                break;

                if ((count > 8) && (ABS(result0, usMax_Value) <= 4) && (ABS(result1, usMin_Value)))
                {
                    count = 8;
                }
    }
    while (--count);

        if (color == SELECT_BLUE)//_BLUE)				// 2 : Blue or PB
            color = SELECT_GREEN;//_GREEN;
        else if (color == SELECT_GREEN)//_GREEN)		// 1: Green or Y
            color = SELECT_RED;//_RED;
        else 						// 0: Red or PR
            break;
    }

    drvif_abl_enable();

    if ((ptADCGainOffset.Gain_R == 0x00) || (ptADCGainOffset.Gain_G == 0x00) || (ptADCGainOffset.Gain_B == 0x00))
        return ERROR_ABORT;
    else
        return ERROR_SUCCEED;
}


