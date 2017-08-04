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


/*============================ Module dependency  ===========================*/
#include "scaler\scaler_pll.h"
#include "scaler\scaler.h"
#include "scaler\adjust.h"
#include "panel\panelapi.h"
#include "panel\panel_struct.h"
// #include "scaler_ddomain\scaler_display.h"
#include "flow_scaler.h"
#include "autoconf.h"
#include "sparrow.h"
#include "reg_def.h"
#include "mode_customer.h"
#include "timer_event.h"
#include "source.h"
#include "rosprintf.h"
#include "flow_struct.h"
#include "reg52.h"
#include "ui_ctrl.h"
/*===================================  Types ================================*/
/*================================== Variables ==============================*/
/*================================ Definitions ==============================*/

/*================================== Function ===============================*/
/*============================================================================*/

int error_flag = 0;
/**
* drvif_framesync_offlinemeasureendevent
*  This function will Measure start/end polling event
* @param <None>
*@return <result>  	 {Return _TRUE if measure finished}
*/
UINT8 drv_adjust_offlinemeasureendevent(void)
{
    //tick system WD, to avoid system reset
    flow_timer_Reset_WatchDog();//tommy add 091210,add for prevent  VGA Restart when signal have->no

    if (!(rtdf_readByte2(SP_MS3CTRL_0x0800) & _BIT5))
    {
        if ((rtdf_inl(SP_MS3STUS_0x080c)&_BIT7)>0) //have error status,restart measure
        {
            error_flag++;
            RTD_Log(LOGGER_DEBUG,"ERROR measure times %d\n",error_flag);
            drv_adjust_syncprocessormeasurestart();
            return FALSE;
        }
        else
        {
            return TRUE;  // off-line measure is ready
        }
    }
    else
        return FALSE;
}


/**
 * CTimerPollingEventProc
 * Polling Event Process
 * @param <ucTimeout --> Timeout number (Max: 255 ms)>
 * @param <Event     --> Polling event. This event has to return _TRUE or _FALSE>
 * @return {_TRUE while polling success, _FALSE for timeout}
 *
*/
UINT8 ScalerTimer_PollingEventProc(UINT8 ucTimeout, UINT8 (*Event)(void))
{
    /////////////////////////////////////
    //Loop is timer(Timer is ucTimeout * 20ms)
    ////////////////////////////////////
    do
    {
        fw_timer_event_DelayXms(20);
        //////////////////////////////////
        //Check if ready
        /////////////////////////////////
        if (Event())
            return _TRUE;
    }
    while (--ucTimeout);
    /////////////////////////////////////
    //Time out occur
    ////////////////////////////////////
    return _FALSE;
}


/**
* Description  : get all delay of each ip(scale up, di. nr)  in the video path
* Input Value  : display
* Output Value : the display all ip delay
*/
static UINT32  adjust_getdelaytarget(void)
{
    UINT8 ucDelay=0;
    UINT32  ucTotal_Delay=0;
//----------------------Get_ScaleUp_Delay-----------------------------//
    if (GET_HSCALE_UP() ||GET_VSCALE_UP())
        ucDelay = 4;
    else // No scale-up
        ucDelay =1;

    ucTotal_Delay+=ucDelay*(disp_info.IPV_ACT_LEN/ _DISP_LEN)* disp_info.IHTotal;
    RTD_Log(LOGGER_DEBUG,"\n ScaleUp Delay = %x", ucDelay);

    return (ucTotal_Delay);
}



/**
* Description  : Calculate the suitable IVS to DVS delay
* Input Value  : Scaling setting
*/

UINT8 drv_adjust_ivs2dvsdelay(void)
{
    UINT8 pData[18];
    UINT32 pData0_32, pData1_32,SYSTEM_PDELAY=3;
    UINT8 DI_Factor,SYSTEM_VDELAY=0;

    if (info->IPH_ACT_WID == 0)
        return 0;

    DI_Factor = GET_INTERLACE_MODE() ? 2 : 1;
    //DI_Mode = (IoReg_ReadByte2(DI_REG18061100_ADDR) & 0x08) >> 3; // CSW+ 0960920 To get DI mode => DI_Mode=1(2D), DI_Mode=0(3D)

    // SYSTEM_VDELAY : Fine tune IV2DV delay lines; Increase SYSTEM_VDELAY will decrease delay target lines
    // SYSTEM_PDELAY : Fine tune IV2DV delay lines; Increase SYSTEM_PDELAY will decrease delay target pixels
    if ((GET_INPUTSOURCE_TYPE() == _SOURCE_VGA) ||
            ((GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR) && (info->ucMode_Curr<= _MODE_720P60)) ||
            ((GET_INPUTSOURCE_TYPE() == _SOURCE_HDMI) && (info->ucMode_Curr <= _HDMIMODE_720P60)) )
    {
        if (GET_INTERLACE_MODE())
        {
            SYSTEM_VDELAY=3;
        }
        else
        {
            SYSTEM_VDELAY=0;
        }
    }
    else
    {
        if (Get_DISPLAY_RATIO_4X3())  	// 4:3 panel
        {
            if ((GET_INPUTSOURCE_TYPE() == _SOURCE_HDMI) && (info->ucMode_Curr == _HDMIMODE_1080P60))
                SYSTEM_VDELAY=1;
            else
                SYSTEM_VDELAY=2;
        }
        else  // 16:9 panel
        {
            if (GET_INTERLACE_MODE()) // YPbPr/HDMI 1080i
            {
                if ((GET_INPUTSOURCE_TYPE() == _SOURCE_HDMI) && ((info->ucMode_Curr == _HDMIMODE_1080I25) || (info->ucMode_Curr == _HDMIMODE_1080I30)))
                {
                    SYSTEM_VDELAY=1;
                    SYSTEM_PDELAY=info->IHTotal*2;
                }
                else
                {
                    if (IS_RATIO_4_3())
                    {
                        SYSTEM_VDELAY=1;
                        SYSTEM_PDELAY=0;//info->IHTotal;
                    }
                    else
                    {
                        SYSTEM_VDELAY=1;
                        SYSTEM_PDELAY=info->IHTotal*2;
                    }
                }
            }
            else  // YPbPr/HDMI 1080p
            {
                SYSTEM_VDELAY=0;
            }
        }
    }

    // Get DVSt
    pData0_32 = ((UINT32)Get_DISP_HORIZONTAL_TOTAL() * Get_DISP_DEN_STA_VPOS()) + Get_DISP_DEN_STA_HPOS() ;
    pData0_32 = pData0_32 * info->IPV_ACT_LEN*DI_Factor/ _DISP_LEN * info->IHTotal / Get_DISP_HORIZONTAL_TOTAL();


    //  channel 1 VGIP
    //	(Channel 1) Input graphic/video mode selection
    //	0: From analog input,ie, follow Hsync/Vsync
    //	1: From digital input,ie, follow DE(data enable)
    if ((rtdf_readByte0(VGIP_CHN1_CTRL_0x0a10)&0x02))// Get IVSt
    {
        if (Get_DISPLAY_RATIO_4X3()) // 4:3 panel
        {
            pData1_32 = ((UINT32)info->IHTotal * ((info->IPV_ACT_STA - (ucIVS2DVSDelay + SYSTEM_VDELAY))*DI_Factor)) + info->IPH_ACT_STA - (ucIHSDelay+ SYSTEM_PDELAY);
        }
        else
        {
            //digital input
            if ((GET_INPUTSOURCE_TYPE() == _SOURCE_HDMI) && (info->ucMode_Curr == _HDMIMODE_1080P50))
            {
                pData1_32 = ((UINT32)info->IHTotal * (info->IPV_ACT_STA - (ucIVS2DVSDelay + SYSTEM_VDELAY-3))*DI_Factor) + info->IPH_ACT_STA - (ucIHSDelay+ SYSTEM_PDELAY);
            }
            else if ((GET_INPUTSOURCE_TYPE() == _SOURCE_HDMI) && (info->ucMode_Curr == _HDMIMODE_1080P60))
                pData1_32 = ((UINT32)info->IHTotal * (info->IPV_ACT_STA - (ucIVS2DVSDelay + SYSTEM_VDELAY))*DI_Factor) + info->IPH_ACT_STA - (ucIHSDelay+ SYSTEM_PDELAY-info->IHTotal*3);
            else if ((GET_INPUTSOURCE_TYPE() == _SOURCE_HDMI) && (info->ucMode_Curr == _HDMIMODE_1080I30))
            {
                pData1_32 = ((UINT32)info->IHTotal * ((info->IPV_ACT_STA - (ucIVS2DVSDelay + SYSTEM_VDELAY+1))*DI_Factor)) + info->IPH_ACT_STA - (ucIHSDelay+ SYSTEM_PDELAY);
            }
            else if ((GET_INPUTSOURCE_TYPE() == _SOURCE_HDMI) && (info->ucMode_Curr == _HDMIMODE_1080I25))
            {
                pData1_32 = ((UINT32)info->IHTotal * (info->IPV_ACT_STA - (ucIVS2DVSDelay + SYSTEM_VDELAY+1))*DI_Factor) + info->IPH_ACT_STA - (ucIHSDelay+ SYSTEM_PDELAY);
            }
            else
            {
                pData1_32 = ((UINT32)info->IHTotal * (info->IPV_ACT_STA - (ucIVS2DVSDelay + SYSTEM_VDELAY))*DI_Factor) + info->IPH_ACT_STA - (ucIHSDelay+ SYSTEM_PDELAY);
            }
        }
    }
    else
    {
        if (Get_DISPLAY_RATIO_4X3()) // 4 : 3 panel
        {

#ifdef CONFIG_ENABLE_FVBug
            if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
                pData1_32 = ((UINT32)info->IHTotal * ((info->IPV_ACT_STA-1) - (ucIVS2DVSDelay + SYSTEM_VDELAY-4))*DI_Factor) + info->IPH_ACT_STA - (ucIHSDelay+ SYSTEM_PDELAY);
            else
                pData1_32 = ((UINT32)info->IHTotal * ((info->IPV_ACT_STA-1) - (ucIVS2DVSDelay + SYSTEM_VDELAY))*DI_Factor) + info->IPH_ACT_STA - (ucIHSDelay+ SYSTEM_PDELAY);
#else
            if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
                pData1_32 = ((UINT32)info->IHTotal * ((info->IPV_ACT_STA-1) - (ucIVS2DVSDelay + SYSTEM_VDELAY-3))*DI_Factor) + info->IPH_ACT_STA - (ucIHSDelay+ SYSTEM_PDELAY);
            else
                pData1_32 = ((UINT32)info->IHTotal * ((info->IPV_ACT_STA-1) - (ucIVS2DVSDelay + SYSTEM_VDELAY+1))*DI_Factor) + info->IPH_ACT_STA - (ucIHSDelay+ SYSTEM_PDELAY);
#endif

        }
        else
        {
            //analog input
            if ((GET_INPUTSOURCE_TYPE() == _SOURCE_HDMI) && (info->ucMode_Curr == _HDMIMODE_1080P50))
            {
                pData1_32 = ((UINT32)info->IHTotal * ((info->IPV_ACT_STA-1) - (ucIVS2DVSDelay + SYSTEM_VDELAY-4))*DI_Factor) + info->IPH_ACT_STA - (ucIHSDelay+ SYSTEM_PDELAY);
            }
            else if ((GET_INPUTSOURCE_TYPE() == _SOURCE_HDMI) && (info->ucMode_Curr == _HDMIMODE_1080P60))
            {
                pData1_32 = ((UINT32)info->IHTotal * ((info->IPV_ACT_STA-1) - (ucIVS2DVSDelay + SYSTEM_VDELAY - 4))*DI_Factor) + info->IPH_ACT_STA - (ucIHSDelay+ SYSTEM_PDELAY);
            }
            else if (((GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR) && ((info->ucMode_Curr == _MODE_1080P50) || (info->ucMode_Curr == _MODE_1080P60))) )// &&
            {
                if (info->ucMode_Curr == _MODE_1080P60)
                {
                    pData1_32 = ((UINT32)info->IHTotal * ((info->IPV_ACT_STA-1) - (ucIVS2DVSDelay + SYSTEM_VDELAY-4))*DI_Factor) + info->IPH_ACT_STA - (ucIHSDelay+ SYSTEM_PDELAY+info->IHTotal);
                }
                else // 1080p50
                {
                    pData1_32 = ((UINT32)info->IHTotal * ((info->IPV_ACT_STA-1) - (ucIVS2DVSDelay + SYSTEM_VDELAY-3))*DI_Factor) + info->IPH_ACT_STA - (ucIHSDelay+ SYSTEM_PDELAY);
                }
            }
            else if ((GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR) && (info->ucMode_Curr <= _MODE_720P60))// && (!Scaler_DispGetStatus(Scaler_DispGetInputInfo(SLR_INPUT_DISPLAY),SLR_DISP_THRIP)))
            {
                if (!GET_INTERLACE_MODE())
                {
                    pData1_32 = ((UINT32)info->IHTotal * ((info->IPV_ACT_STA-1) - (ucIVS2DVSDelay + SYSTEM_VDELAY-1))*DI_Factor) + info->IPH_ACT_STA - (ucIHSDelay+ SYSTEM_PDELAY);
                }
                else
                {
                    pData1_32 = ((UINT32)info->IHTotal * ((info->IPV_ACT_STA-1) - (ucIVS2DVSDelay + SYSTEM_VDELAY-1))*DI_Factor) + info->IPH_ACT_STA - (ucIHSDelay+ SYSTEM_PDELAY+info->IHTotal);
                }

            }
            else if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
            {
                pData1_32 = ((UINT32)info->IHTotal * ((info->IPV_ACT_STA-1) - (ucIVS2DVSDelay + SYSTEM_VDELAY-3))*DI_Factor) + info->IPH_ACT_STA - (ucIHSDelay+ SYSTEM_PDELAY-info->IHTotal);
            }
            else
                pData1_32 = ((UINT32)info->IHTotal * ((info->IPV_ACT_STA-1) - (ucIVS2DVSDelay + SYSTEM_VDELAY))*DI_Factor) + info->IPH_ACT_STA - (ucIHSDelay+ SYSTEM_PDELAY);
        }
    }

    //if (GET_INTERLACE_MODE(display))
    //	if (DI_Mode == 1) // 2D DI
    //		pData1_32 -= (UINT32)info->IHTotal;

    RTD_Log(LOGGER_DEBUG,"\ninfo->IHTotal=%x, info->IPV_ACT_STA=%x , info->IPH_ACT_STA=%x\n", info->IHTotal , info->IPV_ACT_STA,info->IPH_ACT_STA);
    RTD_Log(LOGGER_DEBUG,"\nucIVS2DVSDelay =%x, SYSTEM_VDELAY=%x \n", ucIVS2DVSDelay  , SYSTEM_VDELAY);
    RTD_Log(LOGGER_DEBUG,"\nDI_Factor =%x, SYSTEM_PDELAY=%x \n", DI_Factor  , SYSTEM_PDELAY);

    // Get IVSt + Delay_Target
    pData1_32 += adjust_getdelaytarget();


    RTD_Log(LOGGER_DEBUG, "\nAfter pData0_32=%x, pData1_32=%x \n", pData0_32, pData1_32);
    RTD_Log(LOGGER_DEBUG,"\nAfter pDiff = %x \n", pData1_32 - pData0_32);
    RTD_Log(LOGGER_DEBUG,"\nAfter info->IPV_ACT_LEN=%x, info->IHTotal=%x \n", info->IPV_ACT_LEN, info->IHTotal);
    RTD_Log(LOGGER_DEBUG,"\n##################  After DI_Factor=%x, info->IPH_ACT_STA=%x,  info->IPV_ACT_STA=%x \n", DI_Factor,info->IPH_ACT_STA,info->IPV_ACT_STA);

    // Get IV2DV delay in pixel
    pData0_32  = pData1_32 - pData0_32;   //  Data [0] = IV2DV delay in pixel

    // Get IV2DV delay (unit : line)
    pData[14] = (pData0_32) / info->IHTotal;


    // Get the remainder of IV2DV delay (unit : pixels)
    pData1_32 = pData0_32 - ((UINT32)info->IHTotal * pData[14]);

    //pData[15] = (pData0_32 - ((UINT32)IHTotal * pData[14]) - 16) / 16;
    if (pData1_32 > 16)
        pData[15] = (pData1_32 - 16) >> 4;
    else
        pData[15] = 0;


    RTD_Log(LOGGER_DEBUG,"\n IV2DV delay line & pixel  =====>  pData[14]=%x, pData[15]=%x \n",pData[14], pData[15]);


    // forster modified 0960703
    //if(Scaler_DispGetStatus(Scaler_DispGetInputInfo(SLR_INPUT_DISPLAY),SLR_DISP_THRIP) || Scaler_DispGetStatus(Scaler_DispGetInputInfo(SLR_INPUT_DISPLAY),SLR_DISP_INTERLACE))
    if (GET_INTERLACE_IN_INPUT_MODE())
    {
        RTD_Log(LOGGER_DEBUG,"\n******* Toggle different IV2DV pixel delay with different mode (ODD/Even signal) ********\n");
        pData[17] = ((info->IHTotal >> 1) - 16) >> 4;
        pData[16] = pData[15];
        pData[15] += pData[17];
        // Toggle different IV2DV pixel delay with different mode (ODD/Even signal)
        rtdf_maskl(FS_IV_DV_FINE_TUNING_0x2020, 0x0000007f, (pData[14] << 24) | (pData[15] << 16) |
                   (pData[16] << 8) | _BIT7);
    }
    else
    {
        rtdf_maskl(FS_IV_DV_FINE_TUNING_0x2020, 0x0000ff7f, (pData[14] << 24) | (pData[15] << 16));
    }


    return pData[14];

}

//--------------------------------------------------
// Description  : Adjust spread spectrum range
// Input Value  : Spread spectrum range
// Output Value : None
//--------------------------------------------------
void drv_adjust_spreadspectrumrange(UINT8 ucSpreadRange)
{
    rtdf_maskl(DCLKSS_0x2f08, ~(_BIT4 |_BIT5|_BIT6|_BIT7),  (ucSpreadRange & 0x0f)<<4);
    rtdf_setBits(DCLKSS_0x2f08 , _BIT2);
}

//--------------------------------------------------
// Description  : Sync processor measure start
// Input Value  : None
// Output Value : None
//--------------------------------------------------
void drv_adjust_syncprocessormeasurestart(void)
{
    rtdf_outl(SP_MS3STUS_0x080c, 0xff);
    // CSW 0960702 Start off-line measure
    rtdf_maskl(SP_MS3CTRL_0x0800, ~_BIT10, _BIT9);
}

/**
* framesync_fast_get_sync_point
*  This function will Calculate the suitable framesync point
* @param <None>
*@return <result>  	 {Return Dclk offset}
*/
UINT16 drv_adjust_fast_get_sync_point(void)
{
    //UINT64   	offset=0,aclkcnt_mean=0;
    UINT32 		offset= 0,aclkcnt_mean= 0;
    UINT32 		aclkcnt=0,ulFreq;
    UINT16 		nMCode,nNCode;
    UINT8 		div=1,cpc,cnt=0,index=0;
    //UINT8 		nTryError;

//	sc_plldisp0_RBUS	dpll0_reg;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Measure input active region by xtal clk & Enable active region measure
    //#define IDOMAIN_OFFMS_SP_MS3ENATVCTRL_VADDR        (0xb8051118)
    /*
    Off_ena_source	7	R/W	0
    	Select Source to Measure Active Region (Measured by Crystal clock)
    	0: Measure Input Active Region (Vertical IDEN start to IDEN stop) (measure at IDEN STOP) (Default)
    	1: Measure Display Active Region(Vertical DEN start to DEN stop) (measure at DEN STOP)
    	The function should work correctly when IVS or DVS occurs and enable.
    Off_ms_act_en	6	R/W	0
    	Active Region Measure Enable
    	0: Disable (Default) 1: Enable
    */
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    rtdf_maskl(SP_MS3ENATVCTRL_0x0818, ~(_BIT6 | _BIT7), _BIT6);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //#define IDOMAIN_OFFMS_REG18051100_VADDR            (0xb8051100)
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //rtdf_writeByte1(SP1_CTRL_0x0700, 1); //De-composite circuit enable
    ////////////////////////////////////////////////////////
    // CSW+ enable DClk fine tune
    //#define PA_PS0_0x2d30                   (0xb8004070)
    /*
    DPLLFUPDN 2 R/W 1 DPLL Freqency Tuning
    	1:FreqDn
    	0:FreqUp
    DPLLSTOP 1 R/W 1 DPLL Frequency Tuning
    	1:Enable
    	0:Disable
    */
    ////////////////////////////////////////////////////////
    rtdf_setBits(PLL_DISP_1_0x2f04, _BIT9 | _BIT10);
    ///////////////////////////////////////////////////////////
    //#define SYSTEM_SC_DCLKSS_VADDR                     (0xb8004078)
    /*
    DCLK_ss_en 0 R/W 0
    	Enable DDS Spread Spectrum Output Function
    		0: Disable
    		1: Enable
    */
    ///////////////////////////////////////////////////////////
    rtdf_setBits(DCLKSS_0x2f08, _BIT0);

    drv_adjust_syncprocessormeasurestart();

Measure_Aclkcnt:
    aclkcnt_mean=0;
    aclkcnt=0;
    for (cnt=0; cnt<=4; cnt++)
    {
        if (ScalerTimer_PollingEventProc(50, drv_adjust_offlinemeasureendevent))
        {
            rtdf_setBits(SP_MS3CTRL_0x0800, _BIT12);  // off-line measure pop-up
            //fw_timer_event_DelayXms(10);
            aclkcnt = rtd_inl(SP_MS3ENATV_0x081c) & 0x00ffffff;
            RTD_Log(LOGGER_DEBUG,"Read aclkcnt=%x in FastDo\n", aclkcnt);

            if ((aclkcnt == 0)||(_DISP_LEN ==0))
            {
                offset = 0xfff;
                //DEBUG_PRINT("\n aclkcnt =%x  _DISP_LEN = %x !!\n", aclkcnt,_DISP_LEN);
                //DEBUG_PRINT("Fast frame sync OffLineMeasure ERROR : off-line measure status polling time-out!!\n");
                return offset;
            }
        }
        if (cnt<4)
            aclkcnt_mean=aclkcnt_mean+aclkcnt;
    }
    aclkcnt_mean=aclkcnt_mean>>2;
    if ((aclkcnt_mean>(aclkcnt+11))||aclkcnt_mean<(aclkcnt-11))
    {
        RTD_Log(LOGGER_DEBUG,"\n off-line measure fail !!!!\n");
        RTD_Log(LOGGER_DEBUG,"\n ^^^^^^aclkcnt_mean =%x !!\n", (UINT32)aclkcnt_mean);
        RTD_Log(LOGGER_DEBUG,"\n ^^^^^^aclkcnt =%x !!\n", aclkcnt);
        if (index<=0)
        {
            index++;
            goto Measure_Aclkcnt;
        }
        else
        {
            offset = 0xfff;
            //DEBUG_PRINT("Fast frame sync OffLineMeasure ERROR : off-line measure status polling time-out!!\n");
            return offset;
        }
    }

    RTD_Log(LOGGER_DEBUG,"\n ^^^^^^aclkcnt_mean =%x !!\n", (UINT32)aclkcnt_mean);
    RTD_Log(LOGGER_DEBUG,"\n ^^^^^^aclkcnt =%x !!\n", aclkcnt);

    rtdf_clearBits(SP_MS3CTRL_0x0800, _BIT9|_BIT10);  // clear off-line measureing flag
    ulFreq = (CONFIG_CPU_XTAL_FREQ*128/aclkcnt);
    aclkcnt_mean = Get_DISP_HORIZONTAL_TOTAL();
    aclkcnt_mean = aclkcnt_mean*(Get_DISP_ACT_V_LEN()-1)+Get_DISP_ACT_H_WID();
    //RTD_Log(LOGGER_DEBUG,"aclk_mean = %x\n ", aclkcnt_mean);
    ulFreq = ulFreq * (aclkcnt_mean>>7);
    //ulFreq = ((Get_DISP_HORIZONTAL_TOTAL() +1)*(Get_DISP_ACT_V_LEN()-1)+Get_DISP_ACT_H_WID())*ulFreq;
    RTD_Log(LOGGER_DEBUG,"ulFreq = %x\n ", ulFreq);

    // change range from 46.8Mhz-234Mhz  to  60Mhz-234Mhz,
    // because Dpll will be failed near 50 Mhz,
    // modify by linmc for test, 20080925
    if ((ulFreq<=234000000) && (ulFreq>=60000000))
        div = 1;
    else if ((ulFreq<=117000000) && (ulFreq>=30000000))
        div=2;
    else if ((ulFreq<=58000000) && (ulFreq>=15000000))
        div=4;
    else if ((ulFreq<=29200000) && (ulFreq>=7500000))
        div=8;


    //DEBUG_PRINT("\n DIV = %x",div);

    //Ncode/Mcode=2*16*Div*(DHtotal*DVactive-DH_porch)/(15*aclkcnt)
    //////////////////////////////////////////////////////////////////////
    //Calculate M/N
    // ==> Xtal * M/N * (1/div) * (1/2) * (15/16) = (Xtal/Aclkcnt) * DHTotal * DVHeight
    //Dclk_setting = (Fcrystal/Aclkcnt) * DHTotal * DVHeight
    ///////////////////////////////////////////////////////////////////////
    //offset=2*16*div*1000*((Get_DISP_HORIZONTAL_TOTAL() +1)*(_DISP_LEN-1)+_DISP_WID)/(15*aclkcnt);
    offset = 32*div;
    offset = offset *aclkcnt_mean/15;
    offset = offset*256/aclkcnt;
    //offset=2*16*div*((Get_DISP_HORIZONTAL_TOTAL() +1)*(Get_DISP_ACT_V_LEN()-1)+Get_DISP_ACT_H_WID())/(15*aclkcnt);

    RTD_Log(LOGGER_DEBUG,"nMCode /NCode= %x\n",offset);
    //DEBUG_PRINT("\n offset = %x",offset);
    ////////////////////////////////////////////////
    //Calculate M, N Code
    // N : 4 Bits(3~15)
    // M: 8 Bits( 30 ~255)
    ////////////////////////////////////////////////
    for (cnt=3; cnt<=15; cnt++)
    {
        nNCode=cnt;
        nMCode=nNCode*2*offset/256;
        //nMCode=nNCode*2*offset;
        ////////////////////////////////////
        //四捨五入
        /////////////////////////////////////
        nMCode = ((nMCode >> 1) + (nMCode & 0x01));
        if ((nMCode<=255)&&(nMCode>=3))
            break;
    }
    cpc = drv_pll_cal_cpc(nMCode,nNCode);
    RTD_Log(LOGGER_DEBUG,"M= %x\n ", (UINT32)nMCode);
    RTD_Log(LOGGER_DEBUG,"N= %x\n ", (UINT32)nNCode);

//	offset =32768-32768*2*div*nNCode*((Get_DISP_HORIZONTAL_TOTAL() +1)*(_DISP_LEN-1)+_DISP_WID)/(nMCode*aclkcnt);
    offset = 2*div*nNCode*aclkcnt_mean;
    if (div == 1)
    {
        offset = (offset *256)/aclkcnt;
        offset = offset*32768;
        offset = (offset/nMCode)>>8;
    }
    else
    {
        offset = (offset *128)/aclkcnt;
        offset = offset*32768;
        offset = (offset/nMCode)>>7;
    }
    RTD_Log(LOGGER_DEBUG,"\n @@DClock offset0 = %x\n",(UINT32) offset);
    offset = 32768 - offset;
    RTD_Log(LOGGER_DEBUG,"\n @@DClock offset1 = %x, div = %x, cpc = %x\n",(UINT32) offset, (UINT32)div, (UINT32)cpc);
    drv_pll_SetPLL(nMCode, nNCode, div, cpc);
    drv_pll_Set_DClk_offset((UINT16) offset);

    rtdf_clearBits(SP_MS3ENATVCTRL_0x0818, _BIT6);
    return (UINT16)offset;
}

//******************************************************************************
//
// FUNCTION       :  UINT8  drv_adjust_measureivs2dvsdelay(void)
//
// USAGE          	:  	Calculate the suitable IVS to DVS delay
//								CR[38] : IVS to DVS delay in IHS lines
//								CR[1E] : IVS to DVS delay in ICLK * 16
//
// INPUT         	 	:  	{ none }
//
// OUTPUT         	:	IV to DV delay lines
//
// GLOBALS        	:
//
// USED_REGS      :
//
//******************************************************************************
//forster modified 071002
//void  drv_adjust_measureivs2dvsdelay(void)
UINT8  drv_adjust_measureivs2dvsdelay(void)
{
    UINT32 ivs2dvsdelay_cnt=0,ivs2dvsdelay_cnt1=0,ucTimeout=80,SPixels=0,SLines=0;
    UINT16  MLines=0,MPixels=0,MLines1=0,MPixels1=0,Dvst=0,MainStart,MainEnd;//,Dvst_Factor=0x80;
    UINT8  DI_Factor=0,NR=0,goto_cnt = 0;
    UINT16 ivst=0,iv_len=0,adjust_delay= 0,Min_Front_Porch=0,iv2dv_line=0,iv2dv_line1=0,iv2dv_16p1=0,iv2dv_16p2=0;
    UINT32 timeoutcnt = 0x1ffff;
    UINT32 value,i;
    RTD_Log(LOGGER_DEBUG,"\n Modify IVS2DVSdelay Start  !!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	//drv_scaler_set_display_mode(DISPLAY_MODE_FRAME_SYNC);
    //rtdf_writeByte3(DISPLAY_TIMING_CTRL1_0x2000,(rtdf_readByte3(DISPLAY_TIMING_CTRL1_0x2000)|0x04));
    stDisplayInfo.DHTotal = Get_DISP_HORIZONTAL_TOTAL();
    DI_Factor = /*GET_INTERLACE_MODE() ? 2 :*/ 1;
    /*-------------------------------------------------------------------------------*/
    //modify de-interlace setting 0x1806_104c(DragonLite spec)for odd field delay even field 1 line
    //rtdf_writeByte0(DI_REG1806104C_ADDR,(rtdf_readByte0(DI_REG1806104C_ADDR)|0x01));

    /*-------------------------------------------------------------------------------*/


    MainStart = rtdf_readWord1(MAIN_ACTIVE_V_START_END_0x2124)&0x07ff;
    MainEnd = rtdf_readWord0(MAIN_ACTIVE_V_START_END_0x2124)&0x07ff;
    /*-------------------------------------------------------------------------------*/
    RTD_Log(LOGGER_DEBUG,"\GET_INTERLACE_MODE() = %x\n",(UINT32)GET_INTERLACE_MODE());
    RTD_Log(LOGGER_DEBUG,"\MainStart= %x\n",(UINT32)MainStart);
    RTD_Log(LOGGER_DEBUG,"\MainEnd= %x\n",(UINT32)MainEnd);
    Dvst= rtdf_readWord1(DV_DEN_START_END_0x201c)&0x0fff;

    //RTD_Log(LOGGER_DEBUG,"\n DDOMAIN_DOUBLE_BUFFER_CTRL_VADDR=%x", rtdf_readByte0(DDOMAIN_DOUBLE_BUFFER_CTRL_VADDR));
    //rtdf_outl(DDOMAIN_DOUBLE_BUFFER_CTRL_VADDR,0x00000000);

    rtdf_maskl(FS_IV_DV_FINE_TUNING_0x2020, 0x00000000, (UINT32)_IVS2DVS_MEASURE_DELAY<<22);
    //rtdf_outl(DDOMAIN_DOUBLE_BUFFER_CTRL_VADDR,0x00000001);
    
    fw_timer_event_DelayXms(10);//Tim 0407
    drv_scaler_set_display_mode(DISPLAY_MODE_FRAME_SYNC);    
    flow_timer_Reset_WatchDog();//Tim 0407
    rtdf_writeWord1(MAIN_ACTIVE_V_START_END_0x2124,MainEnd);

    RTD_Log(LOGGER_DEBUG,"\nIHTotal= %x, info->IPH_ACT_WID = %d\n",(UINT32)(info->IHTotal),(UINT32)info->IPH_ACT_WID);

    // FIFO 有 488x48x2 bits，1952 pixels
    // SU 有 960x24x8 bits，7680 pixels，SU or NR on 的時候只有 1920
    NR = rtdf_inl(DM_UZU_CP_CTRL_0x1650)&(_BIT0|_BIT1);
    if (GET_HSCALE_UP() ||GET_VSCALE_UP() || NR)
    {
        RTD_Log(LOGGER_DEBUG,"\n GET_HSCALE_UP/GET_VSCALE_UP **********************");
        //SPixels=((512*64*3/Factor)+/*info->CapWid*/info->IHTotal)/2;//frame sync buffer size
        SPixels = (1952+info->IHTotal)/2; //frame sync buffer size
    }
    else
    {
        RTD_Log(LOGGER_DEBUG,"\n  Not GET_HSCALE_UP/GET_VSCALE_UP   *****************",'*');
        //SPixels=((512*64*3+960*27*8)/Factor)/2;//frame sync buffer size,960*27*8==>scaler up buffer size
        SPixels = (1952 + 7680)/2;
        //SPixels += 2000; // add margin delay for not scaler up case
    }
    // SD 有 960x24x2 bits，1920 pixels，只有 SD off 時可以給 frame sync 用
    if (GET_HSCALE_DOWN() ||GET_VSCALE_DOWN())
    {
        RTD_Log(LOGGER_DEBUG,"\n GET_HSCALE_DOWN/GET_VSCALE_DOWN **********************");
        SLines=SPixels/info->IPH_ACT_WID;
        SPixels=(SPixels-SLines*info->IPH_ACT_WID)>>4;
    }
    else
    {
        RTD_Log(LOGGER_DEBUG,"\n Not GET_HSCALE_DOWN/GET_VSCALE_DOWN **********************");
        RTD_Log(LOGGER_DEBUG,"\n Input VFreq = %d\n",(UINT32)info->IVFreq);
        //SPixels += 1920/2;
        if (Get_DISP_ACT_H_WID() == 1920)
        {
            if (GET_INTERLACE_MODE())
                SPixels += 1220/2;
            else
            {
                if (info->IPH_ACT_WID > 1300) // 1080p
                {
                    if (info->IVFreq > 270)
                        SPixels += 520/2;
                    else if (info->IVFreq > 240) // 25hz
                        SPixels += 420/2;
                    else
                        SPixels += 1120/2;
                }
                else
                    SPixels += 820/2;
            }
        }
        else
            SPixels += 1220/2;
        SLines=SPixels/info->IHTotal;

        SPixels=(SPixels-SLines*info->IHTotal)>>4;
    }


    RTD_Log(LOGGER_DEBUG,"\n SLines=%x ",SLines);
    RTD_Log(LOGGER_DEBUG,"\n SPixels=%x ",SPixels);

    //>>>>>>>>>>>
    //
    // IVS front porch check
    //
    ivst= rtdf_readWord1(VGIP_CHN1_ACT_VSTA_LENGTH_0x0a1c)&0x0fff;
    iv_len = rtdf_readWord0(VGIP_CHN1_ACT_VSTA_LENGTH_0x0a1c)&0x07ff;

    if (rtdf_readByte2(DISPLAY_TIMING_CTRL1_0x2000)&0x20)
        Min_Front_Porch=300*info->IVTotal/(Get_DISP_VERTICAL_TOTAL()*2);
    else
        Min_Front_Porch=300*info->IVTotal/Get_DISP_VERTICAL_TOTAL();

    if (Min_Front_Porch==0)
        Min_Front_Porch=0;
    else
        Min_Front_Porch=Min_Front_Porch/100 +1;

    RTD_Log(LOGGER_DEBUG,"\n Min_Front_Porch=%x ",(UINT32)Min_Front_Porch);
    RTD_Log(LOGGER_DEBUG,"\n IVTotal=%x ",(UINT32)info->IVTotal);

    if ((GET_HSCALE_UP() ||GET_VSCALE_UP() || NR) && ((ivst+iv_len+Min_Front_Porch)>info->IVTotal)&&(info->IVTotal != 0)) // only check front porch when Dclk faster than Iclk
    {
        adjust_delay = ivst+iv_len+Min_Front_Porch-info->IVTotal;
        info->IPV_ACT_STA_PRE=info->IPV_ACT_STA = ivst-adjust_delay;
        rtdf_writeWord1(VGIP_CHN1_ACT_VSTA_LENGTH_0x0a1c,(ivst-adjust_delay));
        rtdf_writeWord0(VGIP_CHN1_DELAY_0x0a20,((rtdf_readWord0(VGIP_CHN1_DELAY_0x0a20)&0x07ff)+adjust_delay));

        RTD_Log(LOGGER_DEBUG,"\n Modify IVST=%x ",info->IPV_ACT_STA);
    }
    //>>>>>>>>>>>


Measure_Start:
    /*-------------------------------------------------------------------------------*/
    //while((rtdf_readByte0(VGIP_CHN1_STATUS_0x0a14)&0x80)==0x80); //odd field
    //ucTimeout = 200;	//ucTimeout=80000;
    //do {
    //	fw_timer_event_DelayXms(1);
    // 	if((rtdf_readByte0(VGIP_CHN1_STATUS_0x0a14)&0x80)==0x80)
    //      		break;
    //}while(--ucTimeout);
    //RTD_Log(LOGGER_DEBUG,"\n VGIP_CHN1_STATUS_0x0a14=%x ", rtdf_readByte0(VGIP_CHN1_STATUS_0x0a14));


    if (GET_HSCALE_UP() ||GET_VSCALE_UP() || NR)
    {
        rtd_outl(SRAM_OK_MIAN_SET_0x222c,0x00011001);			//0x000011001 // 2
    }
    else //---------------------------------------------------------------------------SU not work
    {
        rtd_outl(SRAM_OK_MIAN_SET_0x222c,0x00010001);				//0x000010001
    }
    RTD_Log(LOGGER_DEBUG,"\n SRAM_OK_MIAN_SET_0x222c=%x \n", rtdf_inl(SRAM_OK_MIAN_SET_0x222c));

    ucTimeout = 100;	//ucTimeout=80000; 0.5 sec(100)
    do
    {
        fw_timer_event_DelayXms(1);
        if (rtdf_readByte2(SRAM_OK_MIAN_SET_0x222c)==0x00)
            break;
    }
    while (--ucTimeout);

    if (ucTimeout == 0)
    {
   	rtdf_writeWord1(MAIN_ACTIVE_V_START_END_0x2124,MainStart);//avoid vga reset
        RTD_Log(LOGGER_DEBUG,"\n SRAM measure timeout!!%x \n");
        return 0;
    }

    //read even/odd result depend on VGIP field signal
    if (rtdf_inl(VGIP_CHN1_STATUS_0x0a14) & _BIT7)
    {//odd
        MPixels=rtdf_readWord0(SRAM_OK_MAIN_RESULT_ODD_0x2230)&0x0fff;
        MLines=rtdf_readWord1(SRAM_OK_MAIN_RESULT_ODD_0x2230)&0x0fff;
        MPixels1=rtdf_readWord0(SRAM_OK_MAIN_RESULT_EVEN_0x2234)&0x0fff;
        MLines1=rtdf_readWord1(SRAM_OK_MAIN_RESULT_EVEN_0x2234)&0x0fff;
    }
    else
    {//even
        MPixels=rtdf_readWord0(SRAM_OK_MAIN_RESULT_EVEN_0x2234)&0x0fff;
        MLines=rtdf_readWord1(SRAM_OK_MAIN_RESULT_EVEN_0x2234)&0x0fff;
        MPixels1=rtdf_readWord0(SRAM_OK_MAIN_RESULT_ODD_0x2230)&0x0fff;
        MLines1=rtdf_readWord1(SRAM_OK_MAIN_RESULT_ODD_0x2230)&0x0fff;
    }


    RTD_Log(LOGGER_DEBUG,"MLines1=%x\n",(UINT32)MLines1);
    RTD_Log(LOGGER_DEBUG,"MPixels1=%x\n", (UINT32)MPixels1);

    RTD_Log(LOGGER_DEBUG,"MLines=%x\n",(UINT32)MLines);
    RTD_Log(LOGGER_DEBUG,"MPixels=%x\n", (UINT32)MPixels);

    RTD_Log(LOGGER_DEBUG,"Dvst=%x \n", (UINT32)Dvst);
    RTD_Log(LOGGER_DEBUG,"Dhtotal=%x\n", (UINT32)Get_DISP_HORIZONTAL_TOTAL());
    RTD_Log(LOGGER_DEBUG,"DI_Factor=%x \n", (UINT32)DI_Factor);
    RTD_Log(LOGGER_DEBUG,"_DISP_LEN=%x\n", (UINT32)Get_DISP_ACT_V_LEN());
    RTD_Log(LOGGER_DEBUG,"info->IPV_ACT_LEN=%x\n", (UINT32)info->IPV_ACT_LEN);
    if ((MLines <= 0x30)&&(GET_INPUTSOURCE_TYPE() == _SOURCE_TV))	// unresonable value should be something wrong
    {
        MLines = 0x6d;	// for NTSC
    }
    /*----------------------------------------------------------------------------------------------------------*/
    if (MLines <= Dvst)
    {
        RTD_Log(LOGGER_DEBUG,"\n (MLines <= Dvst)");
#if 1
        if (((MLines)>4) && (goto_cnt<1))
        {
            Dvst=MLines-4;
            rtdf_writeWord1(DV_DEN_START_END_0x201c,Dvst);
            RTD_Log(LOGGER_DEBUG,"\n Modify Dvst= %x\n ", rtdf_readWord1(DV_DEN_START_END_0x201c)&0x0fff);
            goto_cnt++;
            goto Measure_Start;
        }
        else
        {
            RTD_Log(LOGGER_DEBUG,"\n Measure CNT-Dvst =Ivs2DvsDelay<0");
            RTD_Log(LOGGER_DEBUG,"\n Phase increase the Value of IVST or Review the value of Dvst");
        }
#endif
        //return 0;
    }
    //ivs2dvsdelay_cnt=((MLines1-Dvst)*stDisplayInfo.DHTotal+MPixels)*(info->IPV_ACT_LEN* info->IHTotal *DI_Factor/ (disp_info.DispLen)*1000/ stDisplayInfo.DHTotal);//<Fred 20090206 (disp_info[_MAIN_DISPLAY].DispLen)>
    //iv2dv_line=ivs2dvsdelay_cnt/1000/info->IHTotal;
    //iv2dv_16p1=((ivs2dvsdelay_cnt/1000)-iv2dv_line*info->IHTotal)>>4;


    ivs2dvsdelay_cnt=(MLines-Dvst);
    ivs2dvsdelay_cnt= ivs2dvsdelay_cnt*Get_DISP_HORIZONTAL_TOTAL()+MPixels;
    //RTD_Log(LOGGER_DEBUG,"\n ivs2dvsdelay_cnt0=%x\n", ivs2dvsdelay_cnt);
    ucTimeout = (info->IPV_ACT_LEN);
    ucTimeout = ucTimeout * (info->IHTotal);
    //RTD_Log(LOGGER_DEBUG,"\n ucTimeout1=%x\n", ucTimeout);
    ucTimeout = ucTimeout/ Get_DISP_ACT_V_LEN();
    ucTimeout = ucTimeout *1000/ Get_DISP_HORIZONTAL_TOTAL();//<Fred 20090206 (disp_info[_MAIN_DISPLAY].DispLen)>
    //RTD_Log(LOGGER_DEBUG,"\n ucTimeout=%x\n", ucTimeout);
    ivs2dvsdelay_cnt = ivs2dvsdelay_cnt * ucTimeout;
    ivs2dvsdelay_cnt = ivs2dvsdelay_cnt/1000;
    iv2dv_line = ivs2dvsdelay_cnt /info->IHTotal;
    //RTD_Log(LOGGER_DEBUG,"\n iv2dv_line=%x\n", (UINT32)iv2dv_line);
    ucTimeout = (UINT32)iv2dv_line*info->IHTotal;
    ucTimeout = ivs2dvsdelay_cnt - ucTimeout;
    //RTD_Log(LOGGER_DEBUG,"\n iv2dv_16p1=%x\n", (UINT32)ucTimeout);
    iv2dv_16p1=ucTimeout>>4;

    RTD_Log(LOGGER_DEBUG,"\n ivs2dvsdelay_cnt=%x\n", ivs2dvsdelay_cnt);
    RTD_Log(LOGGER_DEBUG,"\n iv2dv_line=%x\n", (UINT32)iv2dv_line);
    RTD_Log(LOGGER_DEBUG,"\n iv2dv_16p1=%x\n", (UINT32)iv2dv_16p1);


    iv2dv_line=iv2dv_line+SLines;
    iv2dv_16p1=iv2dv_16p1+SPixels;
    if (iv2dv_16p1 >= (info->IHTotal>>4))
    {
        iv2dv_line ++;
        iv2dv_16p1 -= (info->IHTotal>>4);
    }
    RTD_Log(LOGGER_DEBUG,"\n Set ivs2dvsdelay_line=%x\n", (UINT32)iv2dv_line);
    RTD_Log(LOGGER_DEBUG,"\n Set iv2dv_16p1=%x\n", (UINT32)iv2dv_16p1);

    if (GET_INTERLACE_MODE())
    {
        if (iv2dv_16p1<(info->IHTotal >>5))
        {
            iv2dv_line -= 1;
            iv2dv_16p1 += (info->IHTotal >>4);
        }
        RTD_Log(LOGGER_DEBUG,"\n Set ivs2dvsdelay_line=%x\n", (UINT32)iv2dv_line);
        RTD_Log(LOGGER_DEBUG,"\n Set iv2dv_16p1=%x\n", (UINT32)iv2dv_16p1);
    }
    rtdf_writeWord1(MAIN_ACTIVE_V_START_END_0x2124,MainStart);
    ////////////////////////////////////
    //Restore Dvst data
    ////////////////////////////////////
    //rtdf_writeWord1(DV_DEN_START_END_0x201c,Dvst);//overflow
    ////////////////////////////////////
    /*----------------------------------------------------------------------------------------------------------*/
    //rtdf_outl(DDOMAIN_DOUBLE_BUFFER_CTRL_VADDR,0x00000000);
    if (GET_INTERLACE_MODE())
    {
        ucTimeout = (iv2dv_line+_IVS2DVS_MEASURE_DELAY);
        ucTimeout = ucTimeout <<8;
        ucTimeout = ucTimeout |iv2dv_16p1;
        ucTimeout = ucTimeout <<8;
        ucTimeout = ucTimeout |(iv2dv_16p1-(info->IHTotal >>5));
        ucTimeout = ucTimeout <<6;
        ucTimeout = ucTimeout | _BIT5;
        EA = 0;
        timeoutcnt = 0x1ffff;
        rtdf_setBits(MAIN_DISPLAY_CONTROL_RSV_0x2100,_BIT11|_BIT10);
        for (i =0 ; i<0x1ffff; i ++)
        {
            value = rtdf_inl(MAIN_DISPLAY_CONTROL_RSV_0x2100)&_BIT10;
            if (value == 0)
                break;
        }

        do
        {
            value = (UINT32)rtdf_inl(MAIN_DISPLAY_CONTROL_RSV_0x2100)&_BIT10;
            if (value)
                break;
        }
        while (timeoutcnt-- != 0);
        rtdf_outl(FS_IV_DV_FINE_TUNING_0x2020, ucTimeout);
        EA = 1;
        RTD_Log(LOGGER_DEBUG,"\nSet IVS2DVSDELAY i = %x,timeout=%x\n", i,timeoutcnt);
        //rtdf_maskl(FS_IV_DV_FINE_TUNING_0x2020, 0x0000007f, ((iv2dv_line+1)<< 22) | ((iv2dv_16p1+ (info->IHTotal>>4))<< 14) |(iv2dv_16p1<< 6) | _BIT5);
    }
    else
    {
        ucTimeout =(iv2dv_line+_IVS2DVS_MEASURE_DELAY);
        ucTimeout = ucTimeout <<8;
        ucTimeout = ucTimeout |iv2dv_16p1;
        ucTimeout = ucTimeout <<14;
        EA = 0;
        timeoutcnt = 0x1ffff;
        rtdf_setBits(MAIN_DISPLAY_CONTROL_RSV_0x2100,_BIT11|_BIT10);
        for (i =0 ; i<0x1ffff; i ++)
        {
            value = rtdf_inl(MAIN_DISPLAY_CONTROL_RSV_0x2100)&_BIT10;
            if (value == 0)
                break;
        }

        do
        {
            value = (UINT32)rtdf_inl(MAIN_DISPLAY_CONTROL_RSV_0x2100)&_BIT10;
            if (value)
                break;
        }
        while (timeoutcnt-- != 0);
        rtdf_maskl(FS_IV_DV_FINE_TUNING_0x2020, 0x0003ffff, ucTimeout);
        EA = 1;
        RTD_Log(LOGGER_DEBUG,"\nSet IVS2DVSDELAY i = %x,timeout=%x\n", i,timeoutcnt);
        //rtdf_maskl(FS_IV_DV_FINE_TUNING_0x2020, 0x0003ffff, ((UINT32)(iv2dv_line+1)<< 22) | ((UINT32)iv2dv_16p1<< 14));
    }

    //rtdf_outl(DDOMAIN_DOUBLE_BUFFER_CTRL_VADDR,0x00000001);

    fw_timer_event_DelayXms(8);

    //RTD_Log(LOGGER_DEBUG,"\n MEASURE:iv2dv_line(%d),iv2dv_16p1(%d)",iv2dv_line,iv2dv_16p1);


    RTD_Log(LOGGER_DEBUG,"\n IVS2DVSDELAY setting value=%x\n", (UINT32)rtd_inl(FS_IV_DV_FINE_TUNING_0x2020));
    RTD_Log(LOGGER_DEBUG,"\n Modify IVS2DVSdelay End  !!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    return iv2dv_line;
}

void drv_adjust_setivs2dvslines(UINT8 line)
{
    UINT32 value;
    value = line + _IVS2DVS_MEASURE_DELAY;
    value = value << 22;
    rtdf_maskl(FS_IV_DV_FINE_TUNING_0x2020,0x003fffff, value);
}

/*======================== End of File =======================================*/
/**
*
* @}
*/

