/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2006
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is for color related functions.
 *
 * @author 	$Author$
 * @date 	$Date$
 * @version 	$Revision$
 */

/**
 * @addtogroup color
 * @{
 */

/*============================ Module dependency  ===========================*/

#include "vip\ypbpr_auto.h"
#include "timer_event.h"
#include "scaler_ddomain\scaler_display.h"
#include "mode.h"
#include "mode_customer.h"
#include "rosprintf.h"

/*===================================  Types ================================*/
#define auto_clr_test 		(0)
#define auto_clr_acitve_Y (0)
#define auto_clr_acitve_Y_pvr (0)
// #define _DISABLE_RT_STORAGE_FOR_TEST	1
/*================================== Variables ==============================*/

#ifdef CONFIG_YPBPR_AUTO_COLOR
static UINT8 exit_autocolor_flag = 0;

/*================================ Definitions ==============================*/
void drvif_color_write_adc_dragon(UINT16 ADC_Data, UINT8 if_offset, UINT8 RGB_ADC)
{
//	UINT32 RegValue,value;
    UINT32 RegValue;
    if (if_offset==1)//if adjust ADC offset
    {
        switch (RGB_ADC)
        {
        case R_ADC_Dragon://R_ADC
            RegValue = rtdf_inl(ADC_OFFSET0_0x042c) & 0xffff0000;
            RegValue |= ADC_Data;
            rtdf_outl(ADC_OFFSET0_0x042c, RegValue);
            break;
        case G_ADC_Dragon://G_ADC
            RegValue = rtdf_inl(ADC_OFFSET0_0x042c) & 0x0000ffff;
            RegValue |= ((UINT32)ADC_Data << 16);
            rtdf_outl(ADC_OFFSET0_0x042c, RegValue);
            break;
        case B_ADC_Dragon://B_ADC
            RegValue = rtdf_inl(ADC_OFFSET1_0x0430) & 0xffff0000;
            RegValue |= ADC_Data;
            rtdf_outl(ADC_OFFSET1_0x0430, RegValue);
            break;
        }
    }
    else//if adjust ADC gain
    {
        switch (RGB_ADC)
        {
        case R_ADC_Dragon://R_ADC
            RegValue = rtdf_inl(ADC_GAIN0_0x0424) & 0xffff0000;
            RegValue |= ADC_Data;
            rtdf_outl(ADC_GAIN0_0x0424, RegValue);
            break;
        case G_ADC_Dragon://G_ADC
            RegValue = rtdf_inl(ADC_GAIN0_0x0424) & 0x0000ffff;
            RegValue |= ((UINT32)ADC_Data << 16);
            rtdf_outl(ADC_GAIN0_0x0424, RegValue);
            break;
        case B_ADC_Dragon://B_ADC
            RegValue = rtdf_inl(ADC_GAIN1_0x0428) & 0xffff0000;
            RegValue |= ADC_Data;
            rtdf_outl(ADC_GAIN1_0x0428, RegValue);
            break;
        }
    }

}


UINT8 drvif_color_activeregion_dragon(UINT16 *Region)
{
    UINT16 HDelay;
    UINT16 VDelay;

    VDelay = rtdf_readWord0(VGIP_CHN1_DELAY_0x0a20)& 0x0fff;
    HDelay = rtdf_readWord1(VGIP_CHN1_DELAY_0x0a20)& 0x0fff;

    Region[0] = rtdf_readWord1(VGIP_CHN1_ACT_HSTA_WIDTH_0x0a18)&0x0fff;//Active_HStart
    Region[0] += HDelay;
    Region[1] = rtdf_readWord0(VGIP_CHN1_ACT_HSTA_WIDTH_0x0a18)&0x0fff;//Active_HWidtht
    Region[2] = rtdf_readWord1(VGIP_CHN1_ACT_VSTA_LENGTH_0x0a1c)&0x0fff;//Active_VStart
    Region[2] += VDelay;
    Region[3] = rtdf_readWord0(VGIP_CHN1_ACT_VSTA_LENGTH_0x0a1c)&0x0fff;//Active_VLines


    return(1);
}

UINT16 drvif_color_adc_adjust_dragon(UINT16 StartH,UINT16 EndH, UINT16 StartV, UINT16 EndV, UINT16 Target, UINT16 ADC_Data, UINT8 if_offset, UINT8 RGB_ADC, UINT8 HistCh)
{

    //UINT8 DCC_status;
    UINT16 counter = 0;
    UINT16 max = 0;
    UINT16 min = 1023;
    UINT16 PEAK_Level;
    UINT16 diff;
    UINT16 diff_temp = 0;
    UINT8 same_diff=0;

    //RTD_Log(LOGGER_DEBUG,"ADC_adjust:  ADC_Data=%d\n",ADC_Data);
    drvif_color_write_adc_dragon(ADC_Data, if_offset, RGB_ADC);

    do
    {
#ifdef CONFIG_WATCHDOG_ENABLE
        rtdf_outl(PERI_WR_TR_VADDR, 1);//tick system WD, to avoid system reset
#endif

        PEAK_Level = drvif_color_finddominant_smartfit_dragon(StartH, EndH, StartV, EndV,HistCh);

        if ((Target - PEAK_Level)<0)
            diff = -(Target - PEAK_Level);
        else
            diff = (Target - PEAK_Level);

        if (same_diff>4)
            break;

        //RTD_Log(LOGGER_DEBUG,"Target=%d,ADC_Data=%d,PEAK_Level=%d,diff=%d\n",Target,ADC_Data,PEAK_Level,diff);

        if (diff == 0)
        {
            break;
        }
        else if (diff != 0 && counter == 10)
        {
            break;
        }
        else if (PEAK_Level >Target)
        {
            max = ADC_Data;
            ADC_Data = ( max + min ) / 2;
            drvif_color_write_adc_dragon(ADC_Data, if_offset, RGB_ADC);
        }
        else if (PEAK_Level < Target)
        {
            min = ADC_Data;
            ADC_Data = ( max + min) / 2;
            drvif_color_write_adc_dragon(ADC_Data, if_offset, RGB_ADC);
        }

        if (diff==diff_temp)
            same_diff++;
        counter++;

        diff_temp=diff;
    }
    while (exit_autocolor_flag==0);
    /*
    	d_vc_global_ctrl_reg.m_dcc_en = 1;//Disable DCC
    	rtdf_outl(COLOR_D_VC_GLOBAL_CTRL_VADDR, d_vc_global_ctrl_reg.regValue);//recover DCC
    */

    return( ADC_Data);
}

UINT16 drvif_color_finddominant_smartfit_dragon( UINT16 HStart, UINT16 HEnd, UINT16 VStart, UINT16 VEnd, UINT8 HistCh)
{
    static UINT8 exit_loop = 0;
    UINT32 auto_result_phase_m_reg,auto_result_phase_l_reg;
    UINT32 PxlNum;
    UINT32 PEAK = 0;
    UINT8 cnt=0;
    if (exit_loop)
    {
        exit_loop = 0;
        return 0;
    }

    rtdf_clearBits(AUTO_FIELD_0x0600, _BIT5);

    //auto_r_g_b_margin_reg.diff_th = 0;
    //rtdf_outl(AUTO_R_G_B_MARGIN_0x060c,auto_r_g_b_margin_reg.regValue);
    rtdf_writeByte0(AUTO_R_G_B_MARGIN_0x060c, 0);

    drvif_color_write_smartfit_dragon(HStart, HEnd, VStart, VEnd, HistCh);

    do
    {
        flow_timer_Reset_WatchDog();


        //RTD_Log(LOGGER_DEBUG,"auto_adj_reg.now_af=%d\n",auto_adj_reg.now_af);

        if ((rtdf_readByte0(AUTO_ADJ_0x0610)& _BIT0)==0)//if smart-fit done
        {
            auto_result_phase_m_reg=  rtdf_inl(AUTO_RESULT_PHASE_M_0x061c);
            auto_result_phase_l_reg =  rtdf_inl(AUTO_RESULT_PHASE_L_0x0620);
            //RTD_Log(LOGGER_DEBUG,"auto_result_phase_m_reg.acc_35_32=%x\n",auto_result_phase_m_reg.acc_35_32);
            //RTD_Log(LOGGER_DEBUG,"auto_result_phase_l_reg.acc_31_0=%x\n",auto_result_phase_l_reg.acc_31_0);
            //SUM = ( ( (UINT64)(auto_result_phase_m_reg.acc_35_32) ) <<32) + (UINT64)(auto_result_phase_l_reg.acc_31_0);
            PxlNum = (HEnd-HStart+1)*(VEnd-VStart+1);

            PEAK = (0xffffffff)/(PxlNum);
            PEAK = auto_result_phase_m_reg*PEAK;
            PEAK += auto_result_phase_l_reg/PxlNum;

#if 0
            RTD_Log(LOGGER_DEBUG,"HStart=%d, HEnd=%d\n",HStart,HEnd);
            RTD_Log(LOGGER_DEBUG,"VStart=%d, VEnd=%d\n",VStart,VEnd);
            RTD_Log(LOGGER_DEBUG,"PEAK=%d\n",(UINT16)PEAK);
#endif

            break;
        };

        //RTD_Log(LOGGER_DEBUG,"FindDominant_SmartFit_Dragon cnt = %d\n",cnt);
        fw_timer_event_DelayXms(11); // CSW+ 0961023 This delay should not be removed, or the mode
        cnt++;
        if (cnt>50) // CSW+ 0961023 force loop out to avoid infinite looping
        {
            drv_scalerdisplay_ForceBackground(TRUE);
            flow_mode_DisableChannelProc();
//Mark by frank@1126 avoid IC ahng	drvif_mode_disablevgip(info->display);
            // should check if need do something else. by ghyu
//			flow_message_push(info->display ? _FL2FL_SUB_STATE_PERIODIC_EVENT : _FL2FL_MAIN_STATE_PERIODIC_EVENT);
            info->FailureCount = 0;
            exit_loop = 1;
            return 0;
        }
    }
    while (cnt<50 || exit_autocolor_flag==0);

    return((UINT16)PEAK);

}

void drvif_color_write_smartfit_dragon( UINT16 Hstart, UINT16 Hend, UINT16 Vstart,UINT16 Vend, UINT8 Ch)
{
    //RTD_Log(LOGGER_DEBUG,"=========In Write_SmatFif=========\n");
    UINT32 auto_r_g_b_margin_reg;

    auto_r_g_b_margin_reg=rtdf_inl(AUTO_R_G_B_MARGIN_0x060c);
    auto_r_g_b_margin_reg &=~ (_BIT8|_BIT9);

    //H & V boundary setting
    rtdf_writeWord0(AUTO_H_BOUNDARY_0x0604, Hend);
    rtdf_writeWord1(AUTO_H_BOUNDARY_0x0604, Hstart);
    rtdf_writeWord0(AUTO_V_BOUNDARY_0x0608, Vend);
    rtdf_writeWord1(AUTO_V_BOUNDARY_0x0608, Vstart);

    //Color Source Select for Detection //20070713 SmartFit YUV definition's wrong on ver.FIB !! Notice whether spec. been changed?
    if (Ch==AutoColor_U)//U
        auto_r_g_b_margin_reg |= _BIT8; //auto_r_g_b_margin_reg.color_sel=0x0;
    else if (Ch==AutoColor_Y)//Y
        auto_r_g_b_margin_reg |= _BIT9;//auto_r_g_b_margin_reg.color_sel=0x1;
    else if (Ch==AutoColor_V)//V
        auto_r_g_b_margin_reg |= 0x0;//auto_r_g_b_margin_reg.color_sel=0x2;

    rtdf_outl(AUTO_R_G_B_MARGIN_0x060c,auto_r_g_b_margin_reg);

    //MIN color measured, Accumulation Mode, Start Auto-Function Tracking Function
    //rtdf_setBits(AUTO_ADJ_0x0610, _BIT0|_BIT4 |_BIT5);
    // [Code Sync][CSW][0980806][1] CSW+ 0980806
    rtdf_maskl(AUTO_ADJ_0x0610,~(_BIT0|_BIT1|_BIT4 |_BIT5),_BIT0|_BIT4 |_BIT5);
    // [Code Sync][CSW][0980806][1] end
}

//void HistPos( UINT16 *Active_Region, UINT16 *Offset_Pos, UINT16 *Y_Gain_Pos, UINT16 *PbPr_Gain_Pos)
void drvif_color_smartfit_windows( UINT16 *Active_Region, UINT16 *Offset_Pos, UINT16 *Y_Gain_Pos, UINT16 *PbPr_Gain_Pos)
{
//VideoBook-Chapter#2_colorbar
    //offsetY(Black)
    Offset_Pos[0]	= (Active_Region[0]+Active_Region[1]) - Active_Region[1]/16;
    Offset_Pos[1]	= Offset_Pos[0] + Active_Region[1]/64;

    //GainY(White)
    Y_Gain_Pos[0]	= Active_Region[0] + Active_Region[1]/32;
    Y_Gain_Pos[1]	= Y_Gain_Pos[0] + Active_Region[1]/64;

    //GainPbPr(Magenta)
    PbPr_Gain_Pos[0] = (Active_Region[0]+Active_Region[1]) - 3.5*Active_Region[1]/8;
    PbPr_Gain_Pos[1] = PbPr_Gain_Pos[0] + Active_Region[1]/64;


    Offset_Pos[2]	= Active_Region[2] + Active_Region[3]/20;
    Offset_Pos[3]	= Offset_Pos[2] + Active_Region[3]/20;
    Y_Gain_Pos[2]  = Offset_Pos[2];
    Y_Gain_Pos[3]  = Offset_Pos[3];
    PbPr_Gain_Pos[2]  = Offset_Pos[2];
    PbPr_Gain_Pos[3]  = Offset_Pos[3];

}
#if 0
void drvif_color_smartfit_windows_tvcolorbar( UINT16 *Active_Region, TVColorBar *TVColorBar)
{

    UINT16 Active_HStart;
    UINT16 Active_HWidth;
    UINT16 Active_VStart;
    UINT16 Active_VWidth;
    UINT8 i;

    Active_HStart = Active_Region[0];
    Active_HWidth= Active_Region[1];
    Active_VStart = Active_Region[2];
    Active_VWidth= Active_Region[3];

    for (i=0; i<8; i++)
    {
        TVColorBar[i].h_start = Active_HStart + (i+0.5)*Active_HWidth/8;
        TVColorBar[i].h_end  = TVColorBar[i].h_start  + Active_HWidth/64;
        TVColorBar[i].v_start = Active_VStart + Active_VWidth/20;
        TVColorBar[i].v_end= TVColorBar[i].v_start + Active_VWidth/20;
    }


}
#endif

void drvif_color_save_adc(UINT16 ADC_Data, ADCGainOffset *ptADC_Setting, UINT8 if_offset, UINT8 RGB_ADC)
{

    if (if_offset==1)//if adjust ADC offset
    {
        switch (RGB_ADC)
        {
        case R_ADC_Dragon://R_ADC
            ptADC_Setting->Offset_R = ADC_Data;

#if(auto_clr_test) //for debugging
            RTD_Log(LOGGER_DEBUG,"R_ADC_Offset=%d\n",(UINT32)ADC_Data);
#endif

            //adc_offset0_reg.adc_off_red_9_0 = ADC_Data;
            break;
        case G_ADC_Dragon://G_ADC
            ptADC_Setting->Offset_G = ADC_Data;

#if(auto_clr_test) //for debugging
            RTD_Log(LOGGER_DEBUG,"G_ADC_Offset=%d\n",(UINT32)ADC_Data);
#endif

            //adc_offset0_reg.adc_off_grn_9_0= ADC_Data;
            break;
        case B_ADC_Dragon://B_ADC
            ptADC_Setting->Offset_B = ADC_Data;

#if(auto_clr_test) //for debugging
            RTD_Log(LOGGER_DEBUG,"B_ADC_Offset=%d\n",(UINT32)ADC_Data);
#endif
            //adc_offset1_reg.adc_off_blu_9_0= ADC_Data;
            break;
        }
    }
    else//if adjust ADC gain
    {
        switch (RGB_ADC)
        {
        case R_ADC_Dragon://R_ADC
            ptADC_Setting->Gain_R = ADC_Data;

#if(auto_clr_test) //for debugging
            RTD_Log(LOGGER_DEBUG,"R_ADC_Gain=%d\n",(UINT32)ADC_Data);
#endif

            //adc_gain0_reg.adc_gain_red= ADC_Data;
            break;
        case G_ADC_Dragon://G_ADC
            ptADC_Setting->Gain_G = ADC_Data;

#if(auto_clr_test) //for debugging
            RTD_Log(LOGGER_DEBUG,"G_ADC_Gain=%d\n",(UINT32)ADC_Data);
#endif
            //adc_gain0_reg.adc_gain_grn= ADC_Data;
            break;
        case B_ADC_Dragon://B_ADC
            ptADC_Setting->Gain_B = ADC_Data;

#if(auto_clr_test) //for debugging
            RTD_Log(LOGGER_DEBUG,"B_ADC_Gain=%d\n",(UINT32)ADC_Data);
#endif
            //adc_gain1_reg.adc_gain_blu= ADC_Data;
            break;
        }
    }
}

//[Code Sync][AlanLi][0980604][1]
//reduce time of doing ypbpr auto color
void drvif_color_adjust_loop_offset_off(ADCGainOffset *pt_info,UINT16 OffsetTarget, UINT16 GainTarget, UINT16 *OffsetPos, UINT16 *GainPos, UINT16 GainData, UINT8 RGB_idx, UINT8 Channel)
{
    UINT16 Gain_Check;
    UINT16 Offset_Check;
    UINT8 Check_idx = 0;
    UINT8 diff_offset;
    UINT8 diff_gain;

//	UINT8 gain_fail=0;
//	UINT8 offset_fail=0;

    drvif_color_write_adc_dragon(512, imgain, RGB_idx);
    //zaxdrvif_color_write_adc_dragon(512, imoffset, RGB_idx);

    do
    {
        flow_timer_Reset_WatchDog();

        //Offset adjustment Begin
        //zaxOffsetData = drvif_color_adc_adjust_dragon(OffsetPos[0], OffsetPos[1], OffsetPos[2], OffsetPos[3],
        //zaxOffsetTarget, OffsetData, imoffset, RGB_idx, Channel);


        //20070731 check offset_adjustment failed or not
        //zaxOffset_Check = drvif_color_finddominant_smartfit_dragon(OffsetPos[0], OffsetPos[1], OffsetPos[2], OffsetPos[3],Channel);
        /*
        		if(  abs(Offset_Check-OffsetTarget) > 20 )//12 is just a appraising value
        			offset_fail = 1;
        */
        //Gain adjustment Begin
        GainData  = drvif_color_adc_adjust_dragon(GainPos[0], GainPos[1], GainPos[2], GainPos[3],
                    GainTarget, GainData, imgain, RGB_idx, Channel);

        Offset_Check = drvif_color_finddominant_smartfit_dragon(OffsetPos[0], OffsetPos[1], OffsetPos[2], OffsetPos[3],Channel);
        Gain_Check   = drvif_color_finddominant_smartfit_dragon(GainPos[0], GainPos[1], GainPos[2], GainPos[3],Channel);
        /*
        		//20070731 check offset_adjustment failed or not
        		if(  abs(Gain_Check-GainTarget) > 20 )//12 is just a appraising value
        			gain_fail = 1;

        		if(offset_fail==1 || gain_fail==1)
        			break;
        */

#if 1 //for debugging
        RTD_Log(LOGGER_DEBUG,"drvif_color_adjust_loop Check_idx=%d\n", (UINT32)Check_idx);
        RTD_Log(LOGGER_DEBUG,"GainTarget=%d, OffsetTarget=%d\n", (UINT32)GainTarget, (UINT32)OffsetTarget);
        RTD_Log(LOGGER_DEBUG,"Gain_Check  =%d, Offset_Check  =%d\n", (UINT32)Gain_Check, (UINT32)Offset_Check);
#endif

        Check_idx++;

        if ( (Offset_Check-OffsetTarget)<0 )
            diff_offset = -(Offset_Check-OffsetTarget);
        else
            diff_offset = (Offset_Check-OffsetTarget);

        if ( (Gain_Check-GainTarget)<0 )
            diff_gain = -(Gain_Check-GainTarget);
        else
            diff_gain = (Gain_Check-GainTarget);

        if (exit_autocolor_flag==1)
            break;

    }
    while ( (diff_offset>4|| diff_gain>4) && (Check_idx < 8) );//16 is by the experience, modified to 5 to speed up

    //zaxdrvif_color_save_adc(OffsetData, pt_info, 1,  RGB_idx);
    drvif_color_save_adc(GainData, pt_info, 0,  RGB_idx);

}
//[Code Sync][AlanLi][0980604][1][end]

#if 0
void drvif_color_adjust_loop(ADCGainOffset *pt_info,UINT16 OffsetTarget, UINT16 GainTarget, UINT16 *OffsetPos, UINT16 *GainPos, UINT16 OffsetData, UINT16 GainData, UINT8 RGB_idx, UINT8 Channel)
{

    UINT16 Gain_Check;
    UINT16 Offset_Check;
    UINT8 Check_idx = 0;
    UINT8 diff_offset;
    UINT8 diff_gain;

//	UINT8 gain_fail=0;
//	UINT8 offset_fail=0;
//	RTD_Log(LOGGER_DEBUG,"Adjust_Loop start\n");
    drvif_color_write_adc_dragon(512, imgain, RGB_idx);
    drvif_color_write_adc_dragon(512, imoffset, RGB_idx);

    do
    {
        flow_timer_Reset_WatchDog();


        //Offset adjustment Begin
        OffsetData = drvif_color_adc_adjust_dragon(OffsetPos[0], OffsetPos[1], OffsetPos[2], OffsetPos[3],
                     OffsetTarget, OffsetData, imoffset, RGB_idx, Channel);


        //20070731 check offset_adjustment failed or not
        Offset_Check = drvif_color_finddominant_smartfit_dragon(OffsetPos[0], OffsetPos[1], OffsetPos[2], OffsetPos[3],Channel);
        /*
        		if(  abs(Offset_Check-OffsetTarget) > 20 )//12 is just a appraising value
        			offset_fail = 1;
        */
        //Gain adjustment Begin
        GainData  = drvif_color_adc_adjust_dragon(GainPos[0], GainPos[1], GainPos[2], GainPos[3],
                    GainTarget, GainData, imgain, RGB_idx, Channel);

        Offset_Check = drvif_color_finddominant_smartfit_dragon(OffsetPos[0], OffsetPos[1], OffsetPos[2], OffsetPos[3],Channel);
        Gain_Check   = drvif_color_finddominant_smartfit_dragon(GainPos[0], GainPos[1], GainPos[2], GainPos[3],Channel);
        /*
        		//20070731 check offset_adjustment failed or not
        		if(  abs(Gain_Check-GainTarget) > 20 )//12 is just a appraising value
        			gain_fail = 1;

        		if(offset_fail==1 || gain_fail==1)
        			break;
        */

#if 0 //for debugging
        RTD_Log(LOGGER_DEBUG,"Adjust_Loop Check_idx=%d\n", Check_idx);
        RTD_Log(LOGGER_DEBUG,"GainTarget=%d, OffsetTarget=%d\n", GainTarget, OffsetTarget);
        RTD_Log(LOGGER_DEBUG,"Gain_Check  =%d, Offset_Check  =%d\n", Gain_Check, Offset_Check);
#endif

        Check_idx++;

        if ( (Offset_Check-OffsetTarget)<0 )
            diff_offset = -(Offset_Check-OffsetTarget);
        else
            diff_offset = (Offset_Check-OffsetTarget);

        if ( (Gain_Check-GainTarget)<0 )
            diff_gain = -(Gain_Check-GainTarget);
        else
            diff_gain = (Gain_Check-GainTarget);

        if (exit_autocolor_flag==1)
            break;

    }
    while ( (diff_offset>1|| diff_gain>1) && (Check_idx < 16) );//16 is by the experience, modified to 5 to speed up

    drvif_color_save_adc(OffsetData, pt_info, 1,  RGB_idx);
    drvif_color_save_adc(GainData, pt_info, 0,  RGB_idx);


}
#endif

void drvif_color_adc_swap_check(YUV_RGB_ADC *ptSwap)
{
    UINT8 adc_swap =(UINT8) (rtdf_inl(ADC_CLKGEN_RGB_CAPTURE_0x043c)>>12)&0x07;

    switch (adc_swap)//Check Whether ADC Swap, 0:G_ADCin, 1:B_ADCin, 2:R_ADCin
    {
    case 0://Rin->R, Gin->G, Bin->B, 	Rin->V, Gin->Y, Bin->U
        ptSwap->V_RGB_idx = R_ADC_Dragon;
        ptSwap->Y_RGB_idx = G_ADC_Dragon;
        ptSwap->U_RGB_idx = B_ADC_Dragon;
        break;
    case 1://Gin->R, Rin->G, Bin->B, 	Gin->V, Rin->Y, Bin->U
        ptSwap->V_RGB_idx = G_ADC_Dragon;
        ptSwap->Y_RGB_idx = R_ADC_Dragon;
        ptSwap->U_RGB_idx = B_ADC_Dragon;
        break;
    case 3://Bin->R, Rin->G, Gin->B, 	Bin->V, Rin->Y, Gin->U
        ptSwap->V_RGB_idx = B_ADC_Dragon;
        ptSwap->Y_RGB_idx = R_ADC_Dragon;
        ptSwap->U_RGB_idx = G_ADC_Dragon;
        break;
    case 4://Rin->R, Bin->G, Gin->B, 	Rin->V, Bin->Y, Gin->U
        ptSwap->V_RGB_idx = R_ADC_Dragon;
        ptSwap->Y_RGB_idx = B_ADC_Dragon;
        ptSwap->U_RGB_idx = G_ADC_Dragon;
        break;
    case 5://Gin->R, Bin->G, Rin->B, 	Gin->V, Bin->Y, Rin->U
        ptSwap->V_RGB_idx = G_ADC_Dragon;
        ptSwap->Y_RGB_idx = B_ADC_Dragon;
        ptSwap->U_RGB_idx = R_ADC_Dragon;
        break;

    default://Bin->R, Gin->G, Rin->B,	 Bin->V, Gin->Y, Rin->U
        ptSwap->V_RGB_idx = B_ADC_Dragon;
        ptSwap->Y_RGB_idx = G_ADC_Dragon;
        ptSwap->U_RGB_idx = R_ADC_Dragon;

    }
}
#if 0
UINT16 drvif_color_ypbpr_adjustment_dragon(ADCGainOffset * ptADC_info)
{
    UINT16 Active_Region[4];
    UINT16 Black_Pos[4];
    UINT16 White_Pos[4];
    UINT16 Magenta_Pos[4];
    YUV_RGB_ADC DRAGON_ADC;
    UINT8 flag_colorbar=1;

    UINT16 Y_Target_Offset	= 4;
    UINT16 U_Target_Offset	= 512;
    UINT16 V_Target_Offset 	= 512;
    UINT16 Y_Target_Gain	= 1016;//1008; //1016; modified by JerryWu for reducing more brightness. 20070731
#ifdef  ADC_not_enough
    UINT16 U_Target_Gain 	= 598; //= 748;  //* 0.8 modified by Jerry Wu for ADC range not enough, compensate it on icm sat 20070729
    UINT16 V_Target_Gain 	= 669; //= 836; //* 0.8  modified by Jerry Wu for ADC range not enough, compensate it on icm sat 20070729
#else
    UINT16 U_Target_Gain 	 = 748;  //* 0.8 modified by Jerry Wu for ADC range not enough, compensate it on icm sat 20070729
    UINT16 V_Target_Gain 	 = 836; //* 0.8  modified by Jerry Wu for ADC range not enough, compensate it on icm sat 20070729

#endif

    RTD_Log(LOGGER_DEBUG,"\n\n==================YPbPr_Adjustment start==================\n\n");



    //disable VidVid Color
    //d_vc_global_ctrl_RBUS d_vc_global_ctrl_Reg;
    //d_vc_global_ctrl_Reg.regValue = rtdf_inl(COLOR_D_VC_GLOBAL_CTRL_VADDR);
    //d_vc_global_ctrl_Reg.m_vc_en = 0;
    //rtdf_outl(COLOR_D_VC_GLOBAL_CTRL_VADDR, d_vc_global_ctrl_Reg.regValue);


    //20070717 Dragon ADC setting: SH gain = 1.45 for expand adc range
    rtdf_maskl(ADC_CTL_0x0414,0x0fffffff, 0xa0000000);


// marked for release this code , Jerry Wu 20071116
//#ifdef CONFIG_CUSTOMER_1_sz
//	flag_colorbar = drvif_color_dragon_autocolor_colobar_dectector(display, 16);
//#endif

    if ( flag_colorbar ==1 )
    {
        // Disable ABL
        //Thur modified for compile 20090115
        rtdf_maskl(ABL_CTRL_0x0c00, ~_BIT31, 0);

        //Active region
        drvif_color_activeregion_dragon(&Active_Region[0]);
        RTD_Log(LOGGER_DEBUG,"true Active_Region=%d,%d,%d,%d\n\n",Active_Region[0],Active_Region[0]+Active_Region[1],Active_Region[2],Active_Region[2]+Active_Region[3]);

#if (auto_clr_acitve_Y_pvr) //for debugging
        UINT16 jj;
        UINT16 h_cut[480];
        UINT16 gap=0;
        for (jj=0; jj<=480; jj++)
        {
            h_cut[jj]=drvif_color_finddominant_smartfit_dragon(jj+Active_Region[0], jj+Active_Region[0]+5, Active_Region[2]+20, Active_Region[2]+30,AutoColor_Y);
            RTD_Log(LOGGER_DEBUG,"%d %d\n",jj,h_cut[jj]);
            if (jj>21)
            {
                if ( (h_cut[jj]-h_cut[jj-20])>350 && ((h_cut[jj]-h_cut[jj-1])<3)  && ((h_cut[jj]-h_cut[jj-2])<3) )
                {
                    gap = jj;
                    break;
                }
            }

        }
        Active_Region[0] += gap;
        Active_Region[1] -= gap*2;
        RTD_Log(LOGGER_DEBUG,"jj=%d, gap=%d\n",jj,gap);
#endif


        drvif_color_smartfit_windows( &Active_Region[0], &Black_Pos[0], &White_Pos[0], &Magenta_Pos[0]);

#if (auto_clr_acitve_Y) //for debugging
        UINT16 i;
        for (i=Active_Region[0]; i<=Active_Region[0]+Active_Region[1]-5; i++)
            RTD_Log(LOGGER_DEBUG,"%d %d\n",i,drvif_color_finddominant_smartfit_dragon(i, i+5, White_Pos[2], White_Pos[3],AutoColor_Y));

        RTD_Log(LOGGER_DEBUG,"Active_Region=%d,%d,%d,%d\n\n",Active_Region[0],Active_Region[0]+Active_Region[1],Active_Region[2],Active_Region[2]+Active_Region[3]);
        RTD_Log(LOGGER_DEBUG,"Black_Pos=%d,%d,%d,%d\n",Black_Pos[0],Black_Pos[1],Black_Pos[2],Black_Pos[3]);
        RTD_Log(LOGGER_DEBUG,"White_Pos=%d,%d,%d,%d\n",White_Pos[0],White_Pos[1],White_Pos[2],White_Pos[3]);
        RTD_Log(LOGGER_DEBUG,"Magenta_Pos=%d,%d,%d,%d\n\n",Magenta_Pos[0],Magenta_Pos[1],Magenta_Pos[2],Magenta_Pos[3]);
#endif
        flow_timer_Reset_WatchDog();

        drvif_color_adc_swap_check(&DRAGON_ADC);//Check RGB-YUV whether been swaped

        //20070713 VIP/Jason
        RTD_Log(LOGGER_DEBUG,"<<< Y adjustment: >>>\n");
        drvif_color_adjust_loop(&ptADC_info[0], Y_Target_Offset, Y_Target_Gain, &Black_Pos[0], &White_Pos[0], 512, 512, DRAGON_ADC.Y_RGB_idx, AutoColor_Y);
        flow_timer_Reset_WatchDog();

        RTD_Log(LOGGER_DEBUG,"<<< U adjustment: >>>\n");
        drvif_color_adjust_loop(&ptADC_info[0], U_Target_Offset, U_Target_Gain, &Black_Pos[0], &Magenta_Pos[0], 512, 512, DRAGON_ADC.U_RGB_idx, AutoColor_U);
        flow_timer_Reset_WatchDog();

        RTD_Log(LOGGER_DEBUG,"<<< V adjustment: >>>\n");
        drvif_color_adjust_loop(&ptADC_info[0], V_Target_Offset, V_Target_Gain, &Black_Pos[0], &Magenta_Pos[0], 512, 512, DRAGON_ADC.V_RGB_idx, AutoColor_V);
        flow_timer_Reset_WatchDog();

        RTD_Log(LOGGER_DEBUG,"\n\n<<<FINAL CHECK>>>\n\n");
        RTD_Log(LOGGER_DEBUG,"Y of Black(%d)=%d\n",Y_Target_Offset,drvif_color_finddominant_smartfit_dragon(Black_Pos[0], Black_Pos[1], Black_Pos[2], Black_Pos[3],AutoColor_Y));
        RTD_Log(LOGGER_DEBUG,"U of Black(%d)=%d\n",U_Target_Offset,drvif_color_finddominant_smartfit_dragon(Black_Pos[0], Black_Pos[1], Black_Pos[2], Black_Pos[3],AutoColor_U));
        RTD_Log(LOGGER_DEBUG,"V of Black(%d)=%d\n\n",V_Target_Offset,drvif_color_finddominant_smartfit_dragon(Black_Pos[0], Black_Pos[1], Black_Pos[2], Black_Pos[3],AutoColor_V));

        RTD_Log(LOGGER_DEBUG,"Y of White(%d)=%d\n",Y_Target_Gain,drvif_color_finddominant_smartfit_dragon(White_Pos[0], White_Pos[1], White_Pos[2], White_Pos[3],AutoColor_Y));
        RTD_Log(LOGGER_DEBUG,"U of Magenta(%d)=%d\n",U_Target_Gain,drvif_color_finddominant_smartfit_dragon(Magenta_Pos[0], Magenta_Pos[1], Magenta_Pos[2], Magenta_Pos[3],AutoColor_U));
        RTD_Log(LOGGER_DEBUG,"V of Magenta(%d)=%d\n",V_Target_Gain,drvif_color_finddominant_smartfit_dragon(Magenta_Pos[0], Magenta_Pos[1], Magenta_Pos[2], Magenta_Pos[3],AutoColor_V));


        // Enable ABL
        rtdf_maskl(ABL_CTRL_0x0c00, ~_BIT31, _BIT31);

        //d_vc_global_ctrl_Reg.regValue = rtdf_inl(COLOR_D_VC_GLOBAL_CTRL_VADDR);
        //d_vc_global_ctrl_Reg.m_vc_en = 1;
        //rtdf_outl(COLOR_D_VC_GLOBAL_CTRL_VADDR, d_vc_global_ctrl_Reg.regValue);

    }
    else
    {
        RTD_Log(LOGGER_DEBUG,"Wrong Pattern ==>  Exit AutoColor\n");
    }

    RTD_Log(LOGGER_DEBUG,"\n\n================== YPbPr_Adjustment end==================\n\n");

    return 1;
}
#endif


UINT16 drvif_color_ypbpr_adjustment_dragon_bartype(ADCGainOffset * ptADC_info, UINT8 bartype)
{
//	UINT16 i;
    UINT16 Active_Region[4];
    UINT16 Black_Pos[4];
    UINT16 White_Pos[4];
    UINT16 Magenta_Pos[4];
    YUV_RGB_ADC DRAGON_ADC;
    UINT8 flag_colorbar=1;

    UINT16 Y_Target_Offset;
    UINT16 U_Target_Offset;
    UINT16 V_Target_Offset;
    UINT16 Y_Target_Gain;
    UINT16 U_Target_Gain;
    UINT16 V_Target_Gain;

    RTD_Log(LOGGER_DEBUG,"\n\n==================YPbPr_Adjustment start==================\n\n");
    //disable VidVid Color
    //d_vc_global_ctrl_RBUS d_vc_global_ctrl_Reg;
    //d_vc_global_ctrl_Reg.regValue = rtdf_inl(COLOR_D_VC_GLOBAL_CTRL_VADDR);
    //d_vc_global_ctrl_Reg.m_vc_en = 0;
    //rtdf_outl(COLOR_D_VC_GLOBAL_CTRL_VADDR, d_vc_global_ctrl_Reg.regValue);


    //20070717 Dragon ADC setting: SH gain = 1.45 for expand adc range
    rtdf_maskl(ADC_CTL_0x0414,0x0fffffff, 0xa0000000);

    Y_Target_Offset	= 4;
    Y_Target_Gain	= 1016;
    U_Target_Offset	= 512;
    V_Target_Offset    = 512;

    if (bartype==ClrBR100)
    {
        RTD_Log(LOGGER_DEBUG,"Pattern: ClrBR100\n");
        U_Target_Gain 	 = 856;
        V_Target_Gain 	 = 920;
    }
    else
    {
        if (bartype==ClrBR75)
            RTD_Log(LOGGER_DEBUG,"Pattern: ClrBR75\n");
        else
            RTD_Log(LOGGER_DEBUG,"Pattern: not ClrBR75 or ClrBR100, use ClrBR75\n");
        U_Target_Gain 	 = 748;
        V_Target_Gain 	 = 836;
    }



// marked for release this code , Jerry Wu 20071116
//#ifdef CONFIG_CUSTOMER_1_sz
//	flag_colorbar = drvif_color_dragon_autocolor_colobar_dectector(display, 16);
//#endif

    if ( flag_colorbar ==1 )
    {
//[Code Sync][AlanLi][0980604][1]
//reduce time of doing ypbpr auto color
        // Disable ABL
        //rtdf_maskl(ABL_CTRL_0x0c00, ~_BIT31, 0);
//[Code Sync][AlanLi][0980604][1][end]

        //Active region
        drvif_color_activeregion_dragon(&Active_Region[0]);
        drvif_color_smartfit_windows( &Active_Region[0], &Black_Pos[0], &White_Pos[0], &Magenta_Pos[0]);

#if (auto_clr_acitve_Y) //for debugging
        for (i=Active_Region[0]; i<=Active_Region[0]+Active_Region[1]-5; i++)
            RTD_Log(LOGGER_DEBUG,"%d %d\n",(UINT32)i,(UINT32)drvif_color_finddominant_smartfit_dragon((UINT32)i, (UINT32)i+5, (UINT32)White_Pos[2], (UINT32)White_Pos[3],(UINT32)AutoColor_Y));

        RTD_Log(LOGGER_DEBUG,"Active=%d,%d,%d,%d\n\n",(UINT32)Active_Region[0],(UINT32)Active_Region[0]+(UINT32)Active_Region[1],(UINT32)Active_Region[2],(UINT32)Active_Region[2]+(UINT32)Active_Region[3]);
        RTD_Log(LOGGER_DEBUG,"B_Pos=%d,%d,%d,%d\n",(UINT32)Black_Pos[0],(UINT32)Black_Pos[1],(UINT32)Black_Pos[2],(UINT32)Black_Pos[3]);
        RTD_Log(LOGGER_DEBUG,"W_Pos=%d,%d,%d,%d\n",(UINT32)White_Pos[0],(UINT32)White_Pos[1],(UINT32)White_Pos[2],(UINT32)White_Pos[3]);
        RTD_Log(LOGGER_DEBUG,"M_Pos=%d,%d,%d,%d\n\n",(UINT32)Magenta_Pos[0],(UINT32)Magenta_Pos[1],(UINT32)Magenta_Pos[2],(UINT32)Magenta_Pos[3]);
#endif
        flow_timer_Reset_WatchDog();

        drvif_color_adc_swap_check(&DRAGON_ADC);//Check RGB-YUV whether been swaped

        //20070713 VIP/Jason
        RTD_Log(LOGGER_DEBUG,"< Y : >\n");
//[Code Sync][AlanLi][0980604][1]
//reduce time of doing ypbpr auto color
        //drvif_color_adjust_loop(&ptADC_info[0], Y_Target_Offset, Y_Target_Gain, &Black_Pos[0], &White_Pos[0], 512, 512, DRAGON_ADC.Y_RGB_idx, AutoColor_Y);
        drvif_color_adjust_loop_offset_off(ptADC_info, Y_Target_Offset, Y_Target_Gain, &Black_Pos[0], &White_Pos[0], 512, DRAGON_ADC.Y_RGB_idx, AutoColor_Y);
        flow_timer_Reset_WatchDog();

        RTD_Log(LOGGER_DEBUG,"< U : >\n");
        //drvif_color_adjust_loop(&ptADC_info[0], U_Target_Offset, U_Target_Gain, &Black_Pos[0], &Magenta_Pos[0], 512, 512, DRAGON_ADC.U_RGB_idx, AutoColor_U);
        drvif_color_adjust_loop_offset_off(ptADC_info, U_Target_Offset, U_Target_Gain, &Black_Pos[0], &Magenta_Pos[0], 512, DRAGON_ADC.U_RGB_idx, AutoColor_U);
        flow_timer_Reset_WatchDog();

        RTD_Log(LOGGER_DEBUG,"< V : >\n");
        //drvif_color_adjust_loop(&ptADC_info[0], V_Target_Offset, V_Target_Gain, &Black_Pos[0], &Magenta_Pos[0], 512, 512, DRAGON_ADC.V_RGB_idx, AutoColor_V);
        drvif_color_adjust_loop_offset_off(ptADC_info, V_Target_Offset, V_Target_Gain, &Black_Pos[0], &Magenta_Pos[0], 512, DRAGON_ADC.V_RGB_idx, AutoColor_V);
//[Code Sync][AlanLi][0980604][1][end]
        flow_timer_Reset_WatchDog();



        RTD_Log(LOGGER_DEBUG,"\n\n<<<FINAL CHECK>>>\n\n");
        RTD_Log(LOGGER_DEBUG,"Y: B(%d)=%d\n",(UINT32)Y_Target_Offset,(UINT32)drvif_color_finddominant_smartfit_dragon(Black_Pos[0], Black_Pos[1], Black_Pos[2], Black_Pos[3],AutoColor_Y));
        RTD_Log(LOGGER_DEBUG,"U: B(%d)=%d\n",(UINT32)U_Target_Offset,(UINT32)drvif_color_finddominant_smartfit_dragon(Black_Pos[0], Black_Pos[1], Black_Pos[2], Black_Pos[3],AutoColor_U));
        RTD_Log(LOGGER_DEBUG,"V: B(%d)=%d\n\n",(UINT32)V_Target_Offset,(UINT32)drvif_color_finddominant_smartfit_dragon(Black_Pos[0], Black_Pos[1], Black_Pos[2], Black_Pos[3],AutoColor_V));

        RTD_Log(LOGGER_DEBUG,"Y: W(%d)=%d\n",(UINT32)Y_Target_Gain,(UINT32)drvif_color_finddominant_smartfit_dragon(White_Pos[0], White_Pos[1], White_Pos[2], White_Pos[3],AutoColor_Y));
        RTD_Log(LOGGER_DEBUG,"U: M(%d)=%d\n",(UINT32)U_Target_Gain,(UINT32)drvif_color_finddominant_smartfit_dragon(Magenta_Pos[0], Magenta_Pos[1], Magenta_Pos[2], Magenta_Pos[3],AutoColor_U));
        RTD_Log(LOGGER_DEBUG,"V: M(%d)=%d\n",(UINT32)V_Target_Gain,(UINT32)drvif_color_finddominant_smartfit_dragon(Magenta_Pos[0], Magenta_Pos[1], Magenta_Pos[2], Magenta_Pos[3],AutoColor_V));

//[Code Sync][AlanLi][0980604][1]
//reduce time of doing ypbpr auto color
        // Enable ABL
        //	rtdf_maskl(ABL_CTRL_0x0c00, ~_BIT31, _BIT31);
//[Code Sync][AlanLi][0980604][1][end]

        //d_vc_global_ctrl_Reg.regValue = rtdf_inl(COLOR_D_VC_GLOBAL_CTRL_VADDR);
        //d_vc_global_ctrl_Reg.m_vc_en = 1;
        //rtdf_outl(COLOR_D_VC_GLOBAL_CTRL_VADDR, d_vc_global_ctrl_Reg.regValue);

    }
    else
    {
        RTD_Log(LOGGER_DEBUG,"Wrong Pattern ==>  Exit AutoColor\n");
    }

    RTD_Log(LOGGER_DEBUG,"\n\n================== YPbPr_Adjustment end==================\n\n");


    return 1;
}
#endif

#if defined(CONFIG_SCART_AUTO_SWITCH)

//SCARTrgb begin===========================================================================================
#if 0 // not used
void drvif_color_SCART_adc_analog_gain( UINT8 RGB_idx, UINT16 ADC_Data)
{
    UINT32 RegValue;

    switch (RGB_idx)
    {
    case _SCART_R:
        RegValue = rtdf_inl(ADC_GAIN0_0x0424) & 0xffff0000;
        RegValue |= ADC_Data;
        rtdf_outl(ADC_GAIN0_0x0424, RegValue);
        break;
    case _SCART_G:
        RegValue = rtdf_inl(ADC_GAIN0_0x0424) & 0x0000ffff;
        RegValue |= ((UINT32)ADC_Data << 16);
        rtdf_outl(ADC_GAIN0_0x0424, RegValue);
        break;
    case _SCART_B:
        RegValue = rtdf_inl(ADC_GAIN1_0x0428) & 0xffff0000;
        RegValue |= ADC_Data;
        rtdf_outl(ADC_GAIN1_0x0428, RegValue);
        break;
    }


}

void drv_video_SCART_ADC_adjust_offset(UINT8 RGB_idx, UINT16 Data)
{
    if (RGB_idx == _SCART_R)
        rtdf_outl(VDC_ADC_OFFSET_R_0x56fc, Data);
    else if (RGB_idx == _SCART_G)
        rtdf_outl(VDC_ADC_OFFSET_G_0x5700, Data);
    else		// VDC_B
        rtdf_outl(VDC_ADC_OFFSET_B_0x5704, Data);

}
#endif

void drv_video_SCART_ADC_adjust_gain(UINT8 RGB_idx, UINT16 Data)
{
    UINT8 Gain_H;
    UINT8 Gain_L;

    Gain_H = (UINT16)(Data & 0x0300) >> 8;
    Gain_L = (UINT16)(Data & 0x00FF);

    if (RGB_idx == _SCART_R )
    {
        rtdf_outl(VDC_ADC_GAIN_R1_0x56e4, Gain_H);
        rtdf_outl(VDC_ADC_GAIN_R0_0x56e8, Gain_L);
    }
    else if (RGB_idx == _SCART_G)
    {
        rtdf_outl(VDC_ADC_GAIN_G1_0x56ec, Gain_H);
        rtdf_outl(VDC_ADC_GAIN_G0_0x56f0, Gain_L);
    }
    else if (RGB_idx == _SCART_B)
    {
        rtdf_outl(VDC_ADC_GAIN_B1_0x56f4, Gain_H);
        rtdf_outl(VDC_ADC_GAIN_B0_0x56f8, Gain_L);
    }
}

UINT16 drvif_color_adc_SCARTrgb_adjust_inverse(UINT16 *POS, UINT16 *Target, UINT16 ADC_Data, UINT8 RGB_ADC, UINT8 HistCh)
{
    UINT16 counter = 0;
    UINT16 max = 0;
    UINT16 min = 1023;
    UINT16 PEAK_Level;
    UINT16 diff;
    UINT16 diff_temp = 0;
    UINT16 same_diff=0;

    UINT16 StartH=POS[0];
    UINT16 EndH=POS[1];
    UINT16 StartV=POS[2];
    UINT16 EndV=POS[3];

//	RTD_Log(LOGGER_DEBUG,"POS=(%d, %d, %d, %d), RGB_ADC=%d, HistCh=%d\n", (UINT32)StartH, (UINT32)EndH, (UINT32)StartV, (UINT32)EndV, (UINT32)RGB_ADC, (UINT32)HistCh);
//	RTD_Log(LOGGER_DEBUG,"Target=(%d, %d, %d), Targe[Hist]=%d\n", (UINT32)Target[AutoColor_Y], (UINT32)Target[AutoColor_U], (UINT32)Target[AutoColor_V], (UINT32)Target[HistCh]);

    //drvif_color_SCART_adc_analog_gain(RGB_ADC,ADC_Data);
    drv_video_SCART_ADC_adjust_gain(RGB_ADC,ADC_Data);


    do
    {
//		#ifdef CONFIG_WATCHDOG_ENABLE
//		rtdf_outl(PERI_WR_TR_VADDR, 1);//tick system WD, to avoid system reset
//		#endif

        PEAK_Level = drvif_color_finddominant_smartfit_dragon(StartH, EndH, StartV, EndV,HistCh);
        /*
        		if((Target - PEAK_Level)<0)
        			diff = -(Target - PEAK_Level);
        		else
        			diff = (Target - PEAK_Level);
        */
        diff = ABS(Target[HistCh], PEAK_Level);

        if (same_diff>4)
            break;

        RTD_Log(LOGGER_DEBUG,"[sub loop]:\ncounter=%d\nTarget=%d\nADC_Data=%d\nPEAK_Level=%d\ndiff=%d\n\n", (UINT32)counter,(UINT32)Target[HistCh],(UINT32)ADC_Data,(UINT32)PEAK_Level,(UINT32)diff);

        if (diff == 0)
        {
            break;
        }
        else if (diff != 0 && counter == 10)
        {
            break;
        }
        else if (PEAK_Level >Target[HistCh])
        {
            max = ADC_Data;
            ADC_Data = ( max + min ) / 2;

            //tune digital gain
            drv_video_SCART_ADC_adjust_gain(RGB_ADC,ADC_Data);//drvif_color_write_adc_dragon(ADC_Data, if_offset, RGB_ADC);
            //drvif_color_SCART_adc_analog_gain(RGB_ADC,ADC_Data);
        }
        else if (PEAK_Level < Target[HistCh])
        {
            min = ADC_Data;
            ADC_Data = ( max + min) / 2;

            //tune digital gain
            drv_video_SCART_ADC_adjust_gain(RGB_ADC,ADC_Data);//drvif_color_write_adc_dragon(ADC_Data, if_offset, RGB_ADC);
            //drvif_color_SCART_adc_analog_gain(RGB_ADC,ADC_Data);
        }

        if (diff==diff_temp)
            same_diff++;
        counter++;

        diff_temp=diff;

    }
    while (exit_autocolor_flag==0);

    return( ADC_Data);

}

//UINT16 drvif_color_adc_SCARTrgb_adjust(UINT16 StartH,UINT16 EndH, UINT16 StartV, UINT16 EndV, UINT16 *Target, UINT16 ADC_Data, UINT8 RGB_ADC, UINT8 HistCh)
UINT16 drvif_color_adc_SCARTrgb_adjust(UINT16 *POS, UINT16 *Target, UINT16 ADC_Data, UINT8 RGB_ADC, UINT8 HistCh)
{
    UINT16 counter = 0;
    UINT16 max = 1023;
    UINT16 min = 0;
    UINT16 PEAK_Level;
    UINT16 diff;
    UINT16 diff_temp = 0;
    UINT16 same_diff=0;

    UINT16 StartH=POS[0];
    UINT16 EndH=POS[1];
    UINT16 StartV=POS[2];
    UINT16 EndV=POS[3];

//	RTD_Log(LOGGER_DEBUG,"POS=(%d, %d, %d, %d), RGB_ADC=%d, HistCh=%d\n", (UINT32)StartH, (UINT32)EndH, (UINT32)StartV, (UINT32)EndV, (UINT32)RGB_ADC, (UINT32)HistCh);
//	RTD_Log(LOGGER_DEBUG,"Target=(%d, %d, %d), Targe[Hist]=%d\n", (UINT32)Target[AutoColor_Y], (UINT32)Target[AutoColor_U], (UINT32)Target[AutoColor_V], (UINT32)Target[HistCh]);

    //drvif_color_SCART_adc_analog_gain(RGB_ADC,ADC_Data);
    drv_video_SCART_ADC_adjust_gain(RGB_ADC,ADC_Data);


    do
    {
//		#ifdef CONFIG_WATCHDOG_ENABLE
//		rtdf_outl(PERI_WR_TR_VADDR, 1);//tick system WD, to avoid system reset
//		#endif

        PEAK_Level = drvif_color_finddominant_smartfit_dragon(StartH, EndH, StartV, EndV,HistCh);
        /*
        		if((Target - PEAK_Level)<0)
        			diff = -(Target - PEAK_Level);
        		else
        			diff = (Target - PEAK_Level);
        */
        diff = ABS(Target[HistCh], PEAK_Level);

        if (same_diff>4)
            break;

        RTD_Log(LOGGER_DEBUG,"[sub loop]:\ncounter=%d\nTarget=%d\nADC_Data=%d\nPEAK_Level=%d\ndiff=%d\n\n", (UINT32)counter,(UINT32)Target[HistCh],(UINT32)ADC_Data,(UINT32)PEAK_Level,(UINT32)diff);

        if (diff == 0)
        {
            break;
        }
        else if (diff != 0 && counter == 10)
        {
            break;
        }
        else if (PEAK_Level >Target[HistCh])
        {
            max = ADC_Data;
            ADC_Data = ( max + min ) / 2;

            //tune digital gain
            drv_video_SCART_ADC_adjust_gain(RGB_ADC,ADC_Data);//drvif_color_write_adc_dragon(ADC_Data, if_offset, RGB_ADC);
            //drvif_color_SCART_adc_analog_gain(RGB_ADC,ADC_Data);
        }
        else if (PEAK_Level < Target[HistCh])
        {
            min = ADC_Data;
            ADC_Data = ( max + min) / 2;

            //tune digital gain
            drv_video_SCART_ADC_adjust_gain(RGB_ADC,ADC_Data);//drvif_color_write_adc_dragon(ADC_Data, if_offset, RGB_ADC);
            //drvif_color_SCART_adc_analog_gain(RGB_ADC,ADC_Data);
        }

        if (diff==diff_temp)
            same_diff++;
        counter++;

        diff_temp=diff;

    }
    while (exit_autocolor_flag==0);

    return( ADC_Data);

}

void drvif_color_adjust_loop_SCARTrgb_offset_off(ADCGainOffset *pt_info,UINT16 *OffsetTarget, UINT16 *GainTarget, UINT16 *OffsetPos, UINT16 *GainPos, UINT8 RGB_idx, UINT8 HistCh)
{
    UINT8 Check_idx = 0;
    UINT16 Gain_Check;
    UINT16 Offset_Check;
//	UINT16 diff_offset;
    UINT16 diff_gain;
    UINT16 GainData = 512;

    //drvif_color_write_adc_dragon(512, imgain, RGB_idx);
//	drv_video_SCART_ADC_adjust_gain(RGB_idx, GainData);

    do
    {
#ifdef CONFIG_WATCHDOG_ENABLE
        WatchdogTickSystem();//tick system WD, to avoid system reset
#endif

        //Gain adjustment Begin
//		if(RGB_idx==_SCART_G)
//			GainData  = drvif_color_adc_SCARTrgb_adjust_inverse(GainPos, GainTarget, GainData, RGB_idx, HistCh);
//		else
        //Gain_Check   = drvif_color_finddominant_smartfit_dragon(GainPos[0], GainPos[1], GainPos[2], GainPos[3], HistCh);
        //RTD_Log(LOGGER_DEBUG,"\n>!!!!!!!!!!!!!!!!!!!!!channel= %d>>>>>>>>> default target = %d\n",(UINT32)HistCh,(UINT32)Gain_Check);
        if (RGB_idx==_SCART_R ||RGB_idx==_SCART_B)
            GainData  = drvif_color_adc_SCARTrgb_adjust_inverse(GainPos, GainTarget, GainData, RGB_idx, HistCh);
        else
            GainData  = drvif_color_adc_SCARTrgb_adjust(GainPos, GainTarget, GainData, RGB_idx, HistCh);

        Offset_Check = drvif_color_finddominant_smartfit_dragon(OffsetPos[0], OffsetPos[1], OffsetPos[2], OffsetPos[3], HistCh);
        Gain_Check   = drvif_color_finddominant_smartfit_dragon(GainPos[0], GainPos[1], GainPos[2], GainPos[3], HistCh);



#if 0 //for debugging
        RTD_Log(LOGGER_DEBUG,"[main loop (gain/offset)]:\nCheck_idx=%d\n", (UINT32)Check_idx);
        RTD_Log(LOGGER_DEBUG,"GainTarget=%d, OffsetTarget=%d\n", (UINT32)(GainTarget[HistCh]), (UINT32)(OffsetTarget[HistCh]));
        RTD_Log(LOGGER_DEBUG,"Gain_Check  =%d, Offset_Check  =%d\n\n", (UINT32)Gain_Check, (UINT32)Offset_Check);
#endif

        Check_idx++;


#if 0
        if ( (Offset_Check-OffsetTarget)<0 )
            diff_offset = -(Offset_Check-OffsetTarget);
        else
            diff_offset = (Offset_Check-OffsetTarget);
#endif

#if 0
        if ( (Gain_Check-GainTarget[HistCh])<0 )
            diff_gain = -(Gain_Check-GainTarget[HistCh]);
        else
            diff_gain = (Gain_Check-GainTarget[HistCh]);
#endif
        diff_gain = ABS(Gain_Check, GainTarget[HistCh]);
        //	RTD_Log(LOGGER_DEBUG,"Gain_Check=%d, GainTarget[HistCh]=%d, diff_gain=%d\n", (UINT32)Gain_Check, (UINT32)(GainTarget[HistCh]), (UINT32)diff_gain);

        if (exit_autocolor_flag==1)
            break;

//	}while( (diff_offset>4|| diff_gain>4) && (Check_idx < 8) );//16 is by the experience, modified to 5 to speed up
    }
    while ( diff_gain>3 && (Check_idx < 8) );//16 is by the experience, modified to 5 to speed up


//return ADC value=============================================
    if (RGB_idx==_SCART_R)
    {
        pt_info->Gain_R = GainData;
        //	RTD_Log(LOGGER_DEBUG,"_SCART_R=%x\n", (UINT32)GainData);
    }
    else if (RGB_idx==_SCART_G)
    {
        pt_info->Gain_G = GainData;
        //	RTD_Log(LOGGER_DEBUG,"_SCART_G=%x\n", (UINT32)GainData);
    }
    else
    {
        pt_info->Gain_B = GainData;
        //	RTD_Log(LOGGER_DEBUG,"_SCART_B=%x\n", (UINT32)GainData);
    }
//========================================================

//	drvif_color_save_adc(GainData, pt_info, 0,  RGB_idx);
    *OffsetTarget = *OffsetTarget;
}

void drvif_color_smartfit_SCARTrgb_windows( UINT16 *Active_Region, UINT16 *Offset_Pos, UINT16 *Gain_Pos)
{
//horizontal

    //offsetY(Black)
    Offset_Pos[0]	= Active_Region[0] + Active_Region[1]/64;
    Offset_Pos[1]	= Offset_Pos[0] + Active_Region[1]/64;

    //GainY(White)
    Gain_Pos[0]	= (Active_Region[0]+Active_Region[1])/2 + Active_Region[1]/32;
    Gain_Pos[1]	= Gain_Pos[0] + Active_Region[1]/64;
    /*
    	//offsetY(Black)
    	Offset_Pos[0]	= Active_Region[0] + Active_Region[1]/32;
    	Offset_Pos[1]	= Offset_Pos[0] + Active_Region[1]/64;

    	//GainY(White)
    	Gain_Pos[0]	= (Active_Region[0]+Active_Region[1]) - Active_Region[1]/16;
    	Gain_Pos[1]	= Gain_Pos[0] + Active_Region[1]/64;

    //vertical
    	Offset_Pos[2]	= Active_Region[2] + Active_Region[3]/64;
    	Offset_Pos[3]	= Offset_Pos[2] + Active_Region[3]/64;

    */
//vertical
    Offset_Pos[2]	= (Active_Region[2] + Active_Region[3])/2 + Active_Region[3]/64;
    Offset_Pos[3]	= Offset_Pos[2] + Active_Region[3]/64;

    Gain_Pos[2]  	= Offset_Pos[2];
    Gain_Pos[3]  	= Offset_Pos[3];

}

UINT16 drvif_color_SCARTrgb_adjustment(ADCGainOffset * VD_ADC)
{
    //static UINT16 i =0 ;


    //ADCGainOffset VD_ADC;
    //UINT32 target_value;


    UINT16 Active_Region[4];
    UINT16 Black_Pos[4];
    UINT16 White_Pos[4];
    UINT16 SCART_R_Target_Gain[3];
    UINT16 SCART_G_Target_Gain[3];
    UINT16 SCART_B_Target_Gain[3];
    UINT16 SCART_R_Target_Offset[3];
    UINT16 SCART_G_Target_Offset[3];
    UINT16 SCART_B_Target_Offset[3];

//according VD RGB2YUV matrix======
#if 1
    SCART_R_Target_Gain[AutoColor_Y]	= 288;
    SCART_R_Target_Gain[AutoColor_U] 	= 356;
    SCART_R_Target_Gain[AutoColor_V]	= 960;//up to 960 scaler cannot see

    SCART_G_Target_Gain[AutoColor_Y]	= 584;
    SCART_G_Target_Gain[AutoColor_U] 	= 212;
    SCART_G_Target_Gain[AutoColor_V]	= 92;

    SCART_B_Target_Gain[AutoColor_Y]	= 96;
    SCART_B_Target_Gain[AutoColor_U] 	= 960;// up to 960 scaler cannot see
    SCART_B_Target_Gain[AutoColor_V]	= 428;
    //i++;
#else
    SCART_R_Target_Gain[AutoColor_Y]	= 512;
    SCART_R_Target_Gain[AutoColor_U] 	= 512;
    SCART_R_Target_Gain[AutoColor_V]	= 512;

    SCART_G_Target_Gain[AutoColor_Y]	= 1023;
    SCART_G_Target_Gain[AutoColor_U] 	= 512;
    SCART_G_Target_Gain[AutoColor_V]	= 512;

    SCART_B_Target_Gain[AutoColor_Y]	= 512;
    SCART_B_Target_Gain[AutoColor_U] 	= 512;
    SCART_B_Target_Gain[AutoColor_V]	= 512;
#endif
    SCART_R_Target_Offset[AutoColor_Y]	= 512;
    SCART_R_Target_Offset[AutoColor_U] = 512;
    SCART_R_Target_Offset[AutoColor_V]	= 512;

    SCART_G_Target_Offset[AutoColor_Y]	= 512;
    SCART_G_Target_Offset[AutoColor_U] = 512;
    SCART_G_Target_Offset[AutoColor_V]	= 512;

    SCART_B_Target_Offset[AutoColor_Y]	= 512;
    SCART_B_Target_Offset[AutoColor_U] = 512;
    SCART_B_Target_Offset[AutoColor_V]	= 512;
//==========================


    //drvif_color_SCART_adc_analog_gain(_SCART_R, 0x300);
    //drvif_color_SCART_adc_analog_gain(_SCART_G, 0x300);
    //drvif_color_SCART_adc_analog_gain(_SCART_B, 0x300);


    RTD_Log(LOGGER_DEBUG,"\n>>>>>>>>>> SCARTrgb ADC adj. start \n");

    //Active region
    drvif_color_activeregion_dragon(&Active_Region[0]);
    drvif_color_smartfit_SCARTrgb_windows( &Active_Region[0], &Black_Pos[0], &White_Pos[0]);

#if (auto_clr_acitve_Y) //for debugging
    for (i=Active_Region[0]; i<=Active_Region[0]+Active_Region[1]-5; i++)
        RTD_Log(LOGGER_DEBUG,"%d %d\n",(UINT32)i,(UINT32)drvif_color_finddominant_smartfit_dragon((UINT32)i, (UINT32)i+5, (UINT32)White_Pos[2], (UINT32)White_Pos[3],(UINT32)AutoColor_Y));

    RTD_Log(LOGGER_DEBUG,"Active=%d,%d,%d,%d\n\n",(UINT32)Active_Region[0],(UINT32)Active_Region[0]+(UINT32)Active_Region[1],(UINT32)Active_Region[2],(UINT32)Active_Region[2]+(UINT32)Active_Region[3]);
    RTD_Log(LOGGER_DEBUG,"B_Pos=%d,%d,%d,%d\n",(UINT32)Black_Pos[0],(UINT32)Black_Pos[1],(UINT32)Black_Pos[2],(UINT32)Black_Pos[3]);
    RTD_Log(LOGGER_DEBUG,"W_Pos=%d,%d,%d,%d\n",(UINT32)White_Pos[0],(UINT32)White_Pos[1],(UINT32)White_Pos[2],(UINT32)White_Pos[3]);
#endif
    //20070713 VIP/Jason
    RTD_Log(LOGGER_DEBUG,"Y = %d \n",(UINT32)drvif_color_finddominant_smartfit_dragon(White_Pos[0], White_Pos[1], White_Pos[2], White_Pos[3], AutoColor_Y));
    RTD_Log(LOGGER_DEBUG,"u = %d \n",(UINT32)drvif_color_finddominant_smartfit_dragon(White_Pos[0], White_Pos[1], White_Pos[2], White_Pos[3], AutoColor_U));
    RTD_Log(LOGGER_DEBUG,"v = %d \n",(UINT32)drvif_color_finddominant_smartfit_dragon(White_Pos[0], White_Pos[1], White_Pos[2], White_Pos[3], AutoColor_V));
    flow_timer_Reset_WatchDog();
//		RTD_Log(LOGGER_DEBUG,"< R : >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    drv_video_SCART_ADC_adjust_gain(_SCART_R, 300);
    drv_video_SCART_ADC_adjust_gain(_SCART_G, 0);
    drv_video_SCART_ADC_adjust_gain(_SCART_B,0);
    drvif_color_adjust_loop_SCARTrgb_offset_off( VD_ADC, SCART_R_Target_Offset, SCART_R_Target_Gain, &Black_Pos[0], &White_Pos[0], _SCART_R, AutoColor_U);
    flow_timer_Reset_WatchDog();

    RTD_Log(LOGGER_DEBUG,"RY = %d \n",(UINT32)drvif_color_finddominant_smartfit_dragon(White_Pos[0], White_Pos[1], White_Pos[2], White_Pos[3], AutoColor_Y));
    RTD_Log(LOGGER_DEBUG,"u = %d \n",(UINT32)drvif_color_finddominant_smartfit_dragon(White_Pos[0], White_Pos[1], White_Pos[2], White_Pos[3], AutoColor_U));
    RTD_Log(LOGGER_DEBUG,"v = %d \n",(UINT32)drvif_color_finddominant_smartfit_dragon(White_Pos[0], White_Pos[1], White_Pos[2], White_Pos[3], AutoColor_V));

    //	RTD_Log(LOGGER_DEBUG,"< B : >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    drv_video_SCART_ADC_adjust_gain(_SCART_R, 0);
    drv_video_SCART_ADC_adjust_gain(_SCART_G, 0);
    drv_video_SCART_ADC_adjust_gain(_SCART_B, 300);
    drvif_color_adjust_loop_SCARTrgb_offset_off( VD_ADC, SCART_B_Target_Offset, SCART_B_Target_Gain, &Black_Pos[0], &White_Pos[0], _SCART_B, AutoColor_V);
    flow_timer_Reset_WatchDog();

    RTD_Log(LOGGER_DEBUG,"BY = %d \n",(UINT32)drvif_color_finddominant_smartfit_dragon(White_Pos[0], White_Pos[1], White_Pos[2], White_Pos[3], AutoColor_Y));
    RTD_Log(LOGGER_DEBUG,"u = %d \n",(UINT32)drvif_color_finddominant_smartfit_dragon(White_Pos[0], White_Pos[1], White_Pos[2], White_Pos[3], AutoColor_U));
    RTD_Log(LOGGER_DEBUG,"v = %d \n",(UINT32)drvif_color_finddominant_smartfit_dragon(White_Pos[0], White_Pos[1], White_Pos[2], White_Pos[3], AutoColor_V));

//		RTD_Log(LOGGER_DEBUG,"< G : >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    drv_video_SCART_ADC_adjust_gain(_SCART_R, 0);
    drv_video_SCART_ADC_adjust_gain(_SCART_G, 300);
    drv_video_SCART_ADC_adjust_gain(_SCART_B, 0);
    drvif_color_adjust_loop_SCARTrgb_offset_off( VD_ADC, SCART_G_Target_Offset, SCART_G_Target_Gain, &Black_Pos[0], &White_Pos[0], _SCART_G, AutoColor_Y);
    flow_timer_Reset_WatchDog();

    RTD_Log(LOGGER_DEBUG,"GY = %d \n",(UINT32)drvif_color_finddominant_smartfit_dragon(White_Pos[0], White_Pos[1], White_Pos[2], White_Pos[3], AutoColor_Y));
    RTD_Log(LOGGER_DEBUG,"u = %d \n",(UINT32)drvif_color_finddominant_smartfit_dragon(White_Pos[0], White_Pos[1], White_Pos[2], White_Pos[3], AutoColor_U));
    RTD_Log(LOGGER_DEBUG,"v = %d \n",(UINT32)drvif_color_finddominant_smartfit_dragon(White_Pos[0], White_Pos[1], White_Pos[2], White_Pos[3], AutoColor_V));
//		RTD_Log(LOGGER_DEBUG,"VD_ADC=(%x, %x, %x)\n", (UINT32)VD_ADC.Gain_R, (UINT32)VD_ADC.Gain_G, (UINT32)VD_ADC.Gain_B);



    //apply auto-color result
    drv_video_SCART_ADC_adjust_gain(_SCART_R, VD_ADC->Gain_R);
    drv_video_SCART_ADC_adjust_gain(_SCART_G, VD_ADC->Gain_G);
    drv_video_SCART_ADC_adjust_gain(_SCART_B, VD_ADC->Gain_B);

    //	drvif_color_SCART_adc_analog_gain(_SCART_R, VD_ADC.Gain_R);
    //	drvif_color_SCART_adc_analog_gain(_SCART_G, VD_ADC.Gain_G);
    //	drvif_color_SCART_adc_analog_gain(_SCART_B, VD_ADC.Gain_B);
    RTD_Log(LOGGER_DEBUG,"Y = %d \n",(UINT32)drvif_color_finddominant_smartfit_dragon(White_Pos[0], White_Pos[1], White_Pos[2], White_Pos[3], AutoColor_Y));
    RTD_Log(LOGGER_DEBUG,"u = %d \n",(UINT32)drvif_color_finddominant_smartfit_dragon(White_Pos[0], White_Pos[1], White_Pos[2], White_Pos[3], AutoColor_U));
    RTD_Log(LOGGER_DEBUG,"v = %d \n",(UINT32)drvif_color_finddominant_smartfit_dragon(White_Pos[0], White_Pos[1], White_Pos[2], White_Pos[3], AutoColor_V));
    RTD_Log(LOGGER_DEBUG,"\n\n>>>>>>>>>> SCARTrgb ADC adj. end !\n\n");

    return 1;
}






//SCARTrgb end============================================================================
#endif

