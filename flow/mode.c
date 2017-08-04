/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2009
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is for rtdaccess related functions.
 *
 * @author 	$Author$
 * @date	$Date$
 * @version	$Revision$
 * @ingroup	rtdaccess
 */

/**
 * @addtogroup rtdaccess
 * @{
 */

/*============================ Module dependency  ===========================*/
#include "mode.h"
#include "mode_customer.h"
#include "flow_struct.h"
#include "rtd_types.h"
#include "scaler_idomain\measure.h"
#include "scaler_idomain\syncproc.h"
#include "scaler_idomain\abl.h"
#include "scaler\scaler.h"
#include "scaler_idomain\vgip.h"
#include "rosprintf.h"
#include "flow_display.h"
#include "flow_scaler.h"
#include "flow_color.h"
#include "timer_event.h"
#include "hdmi\hdmiFun.h"
#include "scaler_ddomain\scaler_display.h"
#include "panel\panelapi.h"
#include "video\video.h"

ModeInformationType xdata stModeInfo;

//--------------------------------------------------
// Support Timing
//--------------------------------------------------
#define _H_FREQ_MAX                     (1100)            // Unit in 0.1 kHZ
#define _H_FREQ_MIN                     (100)             // Unit in 0.1 kHZ
#define _V_FREQ_MAX                     (880)             // Unit in 0.1 HZ
#define _V_FREQ_MIN                     (490)             // Unit in 0.1 HZ

extern void drv_sig_vga_Init(void);
extern void drv_sig_ypbpr_Init(void);
#if 0
void mode_setinfofromtable(UINT8 modeno)
{
    UINT8 index;

    RTD_Log(LOGGER_DEBUG,"\nmode_setinfofromtable \n");
#if 1
    index = mode_SearchModeMap(modeno);
#else
    index = modeno-2;
#endif
    //without overscan
    info->ucMode_Curr = GET_MODE_INPUT_MODE(tINPUTMODE_PRESET_TABLE[index].ModeCurr);

    info->ADC_CLOCK= tINPUTMODE_PRESET_TABLE[index].IHTotal;

    // replace by linmc, temporately, 20081122
    info->IPH_ACT_STA= tINPUTMODE_PRESET_TABLE[index].IHStartPos;
    //info->IPH_ACT_STA= tINPUTMODE_PRESET_TABLE[info->ucMode_Curr].IHStartPos - IHSDelay_Shift[info->ucMode_Curr];
    info->IPH_ACT_WID= tINPUTMODE_PRESET_TABLE[index].IHWidth;
    info->IHFreq = tINPUTMODE_PRESET_TABLE[index].IHFreq;
    info->IHTotal = tINPUTMODE_PRESET_TABLE[index].IHTotal;
    info->IPV_ACT_STA= tINPUTMODE_PRESET_TABLE[index].IVStartPos; // CSWCSW
    info->IPV_ACT_LEN= tINPUTMODE_PRESET_TABLE[index].IVHeight;
    info->IVFreq = tINPUTMODE_PRESET_TABLE[index].IVFreq;
    info->IVTotal = tINPUTMODE_PRESET_TABLE[index].IVTotal;

    info->IPH_ACT_STA_PRE = info->IPH_ACT_STA;
    info->IPH_ACT_WID_PRE = info->IPH_ACT_WID;
    info->IPV_ACT_STA_PRE = info->IPV_ACT_STA;
    info->IPV_ACT_LEN_PRE = info->IPV_ACT_LEN;


    RTD_Log(LOGGER_DEBUG," stModeInfo.IHStartPos = %x\n",stModeInfo.IHStartPos);
    RTD_Log(LOGGER_DEBUG," stModeInfo.IVStartPos = %x\n",stModeInfo.IVStartPos);

    RTD_Log(LOGGER_DEBUG,"**HS=%x,HW=%x,VS=%x,VW=%x\n",info->IPH_ACT_STA,info->IPH_ACT_WID,info->IPV_ACT_STA,info->IPV_ACT_LEN);
    RTD_Log(LOGGER_DEBUG,"**IHFreq=%x,IVFreq=%x,IHTotal=%x,IVTotal=%x,ADC_CLOCK:%x\n",info->IHFreq,info->IVFreq,info->IHTotal,info->IVTotal,info->ADC_CLOCK);

}
#endif
#if 0 // not used
//--------------------------------------------------
/** Description  : Mode detect process for HDMI
* Input Value  : None
* Output Value : Return _TRUE if we get a stable mode
*/
static UINT8 mode_hdmidetect(void)//V402 modify
{
    UINT8 modetemp = _MODE_NOSIGNAL;
    UINT16 hdmi_IHTotal=0,hdmi_IVHeight=0,hdmi_IHWidth=0;
    UINT16 IHTotal_index=0,IVTotal_index=0;
    UINT32 tmds_clk=0;

    // Get measure results and decide " modetemp = _MODE_NOSIGNAL/_MODE_NOSUPPORT/_MODE_EXIST "
    if (drv_measure_offlinemeasure(_OFFLINE_MS_SRC_HDMI, ANALOG_MODE_MEASUREMENT, &stModeInfo)==_MODE_SUCCESS)
    {
        //let Vsyn,Hsyn's polarity is positive
        //20080525 kist mark, done in drvif_mode_offlinemeasure
        //mode_setsignalpolatity(ANALOG_MODE_MEASUREMENT);

        if ((stModeInfo.IHFreq < _H_FREQ_MIN) || (stModeInfo.IHFreq > _H_FREQ_MAX) || (stModeInfo.IVFreq < _V_FREQ_MIN) || (stModeInfo.IVFreq > _V_FREQ_MAX))
        {
            RTD_Log(LOGGER_DEBUG,"HFreq or VFreq not support!\n");
            return _MODE_NOSUPPORT;
        }

        if (drv_measure_offlinemeasure(_OFFLINE_MS_SRC_HDMI, DIGITAL_MODE_MEASUREMENT, &stModeInfo)==_MODE_SUCCESS)
        {
            hdmi_IHTotal=stModeInfo.IHTotal;
            hdmi_IVHeight=stModeInfo.IVHeight;
            hdmi_IHWidth=stModeInfo.IHWidth;
        }
        else
            return _MODE_NOSIGNAL;

        if (drv_measure_offlinemeasure(_OFFLINE_MS_SRC_HDMI, DIGITAL_MODE_MEASUREMENT, &stModeInfo)!=_MODE_SUCCESS)
            return _MODE_NOSIGNAL;

        if (ABS(stModeInfo.IHTotal, hdmi_IHTotal) <= 1)
            stModeInfo.IHCount = hdmi_IHTotal;

        if (ABS(stModeInfo.IVHeight ,hdmi_IVHeight) < 1)
            stModeInfo.IVHeight = hdmi_IVHeight;

        if (ABS(stModeInfo.IHWidth ,hdmi_IHWidth) < 1)
            stModeInfo.IHWidth = hdmi_IHWidth;

        if ((stModeInfo.IHTotal != hdmi_IHTotal) || (stModeInfo.IVHeight != hdmi_IVHeight) ||(stModeInfo.IHWidth != hdmi_IHWidth)) //|| (stModeInfo.Polarity != polaritytemp))
        {
            RTD_Log(LOGGER_DEBUG,"IHTotal,IVHeight,or IHWidth do not equal first measure!\n");
            modetemp = _MODE_NOSIGNAL;
        }
        else
        {
            IHTotal_index=(stModeInfo.IHTotal+1)/10;
            IVTotal_index=(stModeInfo.IVTotal+1)/10;
//			if(drvif_Hdmi_GetInterlace())
//				IVTotal_index*=2;

            tmds_clk=IHTotal_index*IVTotal_index*(stModeInfo.IVFreq/10);
            tmds_clk=tmds_clk/10;

            if (tmds_clk > 165000)   //kHz
            {
                RTD_Log(LOGGER_DEBUG, "TMDS CLOCK is over 165MHz! \n");
                return  _MODE_NOSUPPORT;
            }
            modetemp = _MODE_SUCCESS;
        }
    }
    else
    {
        RTD_Log(LOGGER_DEBUG,"drvif_drvif_mode_offlinemeasure(ANALOG) Fail !!\n");
        modetemp =  _MODE_NOSIGNAL;
    }
    return modetemp;


}
#endif

#if 0
UINT16 mode_get_IVTotal(void)
{
    UINT32 value,DclkOffset,Dclk,temp,temp2;
    UINT16 DVTotal,DHTotal ;
    UINT8 M_code, N_code,div;

    UINT8 timeoutcnt;
    UINT32 ulResult;

    //fw_scaler_set_vgip(VGIP_SRC_ADC, VGIP_MODE_ANALOG);
    temp2 = rtd_inl(VGIP_CHN1_CTRL_0x0a10);
    // Disable SRC_Clock_Enbale bit
    rtdf_maskl(VGIP_CHN1_CTRL_0x0a10, ~_BIT0,0x0);
    // 2nd determine the input source
    rtdf_maskl(VGIP_CHN1_CTRL_0x0a10, ~(_BIT30|_BIT29|_BIT28|_BIT1), ((UINT32)VGIP_SRC_ADC<<28) | ((UINT32)(VGIP_MODE_ANALOG & 0x01)<<1));
    // 3rd enable SRC_Clock_Enbale bit
    rtdf_maskl(VGIP_CHN1_CTRL_0x0a10, ~(_BIT0), _BIT0);

    RTD_Log(LOGGER_DEBUG, "\nIDOMAIN_VGIP_VGIP_CHN1_CTRL_VADDR = %x\n", (UINT32)rtd_inl(VGIP_CHN1_CTRL_0x0a10));


    //start measure
    timeoutcnt = 10;
    rtdf_setBits(SP_MS1CTRL_0x0900, _BIT9);
    do
    {
        fw_timer_event_DelayXms(30);//fw_timer_event_DelayXms(30);

        ulResult = rtdf_inl(SP_MS1CTRL_0x0900);
        //RTD_Log(LOGGER_DEBUG,"onlinemeasure ulResult=%x\n",ulResult);
        if (!(ulResult & _BIT9)) // polling status(Popup Measure Result) bit
            break;
    }
    while (--timeoutcnt);

    if (timeoutcnt == 0x00)
    {
        RTD_Log(LOGGER_DEBUG,"Pop Up Timeout_1\n");
        return _FALSE;
    }


    //pop up result
    timeoutcnt = 10;
    rtdf_setBits(SP_MS1CTRL_0x0900, _BIT12);
    do
    {
        fw_timer_event_DelayXms(30);//fw_timer_event_DelayXms(30);

        ulResult = rtdf_inl(SP_MS1CTRL_0x0900);
        //RTD_Log(LOGGER_DEBUG,"onlinemeasure ulResult=%x\n",ulResult);
        if (!(ulResult & _BIT12)) // polling status(Popup Measure Result) bit
            break;
    }
    while (--timeoutcnt);

    if (timeoutcnt == 0x00)
    {
        RTD_Log(LOGGER_DEBUG,"Pop Up Timeout_2\n");
        return _FALSE;
    }

    ulResult = (UINT32)(rtdf_inl(SP_MS1RST0_0x0904) & 0x0fff0000)>>16;
    RTD_Log(LOGGER_DEBUG,"ivcount_ulResult = %d\n",(UINT32)ulResult);

    return ulResult;


#if 0
    temp = rtd_inl(DISPLAY_TIMING_CTRL1_0x2000);
    rtd_outl(DISPLAY_TIMING_CTRL1_0x2000,0x07430400);
    rtd_outl(PLL_DISP_1_0x2f04, 0x00000001);

    rtdf_outl(MEASURE_DVTOTAL_LAST_LINE_PIXELS_RESULT1_0x203c,0xe0000000);
    WaitFor_IVS1();
    WaitFor_IVS1();
    WaitFor_IVS1();
    WaitFor_IVS1();
    value = rtd_inl(MEASURE_DVTOTAL_LAST_LINE_PIXELS_RESULT1_0x203c)&0x0fff0fff;
    DVTotal = value>>16;
    DHTotal = value & 0x00000fff;
    RTD_Log(LOGGER_DEBUG, "\nmode_get_IVTotal!!!!!!\n");
    RTD_Log(LOGGER_DEBUG, "DVTotal=%d\n",DVTotal);
    RTD_Log(LOGGER_DEBUG, "DHTotal=%d\n",DHTotal);

    DclkOffset = (rtd_inl(DCLKSS_0x2f08)&0x0fff0000)>>16;
    RTD_Log(LOGGER_DEBUG, "DclkOffset=%x\n",DclkOffset);
    value = rtd_inl(PLL_DISP_0_0x2f00);
    RTD_Log(LOGGER_DEBUG, "data=%x\n",value);
    M_code = ((value & 0x3ff00000)>>20)+2;
    N_code = ((value & 0x00000f00)>>8)+2;
    div = 1<<((value & 0x00030000)>>16);
    RTD_Log(LOGGER_DEBUG, "M_code=%d\n",M_code);
    RTD_Log(LOGGER_DEBUG, "N_code=%d\n",N_code);
    RTD_Log(LOGGER_DEBUG, "div=%d\n",div);
    Dclk = (CONFIG_CPU_XTAL_FREQ/N_code/div/2*M_code)*10;
    Dclk = Dclk -(Dclk/32768*DclkOffset);
    RTD_Log(LOGGER_DEBUG, "Dclk=%d\n",Dclk);

    Dclk = Dclk/(DVTotal*Get_DISP_HORIZONTAL_TOTAL()+DHTotal);
    RTD_Log(LOGGER_DEBUG, "IVFreq=%d\n",Dclk);
    RTD_Log(LOGGER_DEBUG, "stModeInfo.IHFreq=%d\n",stModeInfo.IHFreq);
    Dclk = stModeInfo.IHFreq*1000/Dclk;
    RTD_Log(LOGGER_DEBUG, "IVTotal=%d\n",Dclk);

    rtd_outl(DISPLAY_TIMING_CTRL1_0x2000,temp);
    rtd_outl(VGIP_CHN1_CTRL_0x0a10,temp2);
    if (Dclk<=600)
        return Dclk;
    else
        return Dclk - 2;
#endif
}
#endif
#if 0
UINT8 mode_judge_confuse_mode(UINT8 mode_index,UINT16 usHighPulsePeriod)
{
    if (VGADetecSyncTypeMode == SEPARATE_MODE)
    {
        RTD_Log(LOGGER_DEBUG, "DIFF=%d\n",(UINT32)ABS(stModeInfo.IVCount,tINPUTMODE_PRESET_TABLE[mode_index].IVTotal));
        if (mode_index == _MODE_640x400_70HZ)
        {
            return _FALSE;//display by 720x400@70Hz
        }
        else if (mode_index == _MODE_640x400_70HZ)
        {
            if (ABS(stModeInfo.IHSyncPulseCount,754) < 3)
                return _FALSE;
        }
        else if ((mode_index == _MODE_640x350_70HZ) && (ABS(stModeInfo.IHSyncPulseCount,103) > 10))
        {
            return _FALSE;

        }
        else if ((mode_index == _MODE_480P) && (info->Polarity == _SYNC_HP_VP))
        {
            if (ABS(usHighPulsePeriod ,0x5a) < 2) // CSW+ 0970923 848*480 60 not support
                return _FALSE;
        }
        else if (mode_index == _MODE_640x480_60HZ)
        {
            if ((ABS(usHighPulsePeriod ,0x67) > 8) || (info->Polarity==_SYNC_HN_VP)) // Should be VGA 480p not VGA 640*480
                return _FALSE;
            else if (ABS(stModeInfo.IVCount,tINPUTMODE_PRESET_TABLE[mode_index].IVTotal) > 3)
                return _FALSE;
        }
        else if (mode_index == _MODE_640x480_72HZ)
        {
            if (ABS(stModeInfo.IVCount,tINPUTMODE_PRESET_TABLE[mode_index].IVTotal) > 3)
                return _FALSE;
        }
        else if (mode_index == _MODE_640x480_75HZ)
        {
            if ((ABS(usHighPulsePeriod ,0x37) > 10) || (info->Polarity == _SYNC_HN_VP))
                return _FALSE;
        }
//		else if (mode_index == _MODE_720x400_50HZ){
//			if (ABS(usHighPulsePeriod ,67) > 10)
//				return _FALSE;
//		}
//		else if (mode_index == _MODE_720x480_60HZ){
//			if (ABS(usHighPulsePeriod ,0x3e) > 10) // Should be VGA 640*480
//				return _FALSE;
//		}
        else if (mode_index == _MODE_800x600_72HZ)
        {
            if (ABS(stModeInfo.IHSyncPulseCount,64) > 20)
                return _FALSE;
        }
        // charlie, 20090203, for Video timing display in VGA source
        else if (mode_index == _MODE_848x480_60HZ)
        {
            if (ABS(stModeInfo.IVCount,tINPUTMODE_PRESET_TABLE[mode_index].IVTotal) > 3)
                return _FALSE;;
        }
        else if (mode_index == _MODE_1024x768_60HZ)
        {
            RTD_Log(LOGGER_DEBUG, "DIFF=%d\n",(UINT32)ABS(stModeInfo.IVCount,tINPUTMODE_PRESET_TABLE[mode_index].IVTotal));

            if (ABS(stModeInfo.IVCount,tINPUTMODE_PRESET_TABLE[mode_index].IVTotal) > 3)
                return _FALSE;;
        }
        else if (mode_index == _MODE_1024x768_70HZ)
        {
            if (ABS(stModeInfo.IVCount,tINPUTMODE_PRESET_TABLE[mode_index].IVTotal) > 3)
                return _FALSE;;
        }
        else if (mode_index == _MODE_1024x768_75HZ)
        {
            if (ABS(stModeInfo.IHSyncPulseCount,33) > 10)
                return _FALSE;
        }
        else if ((mode_index == _MODE_1024x800_73HZ)/* && (info->Polarity == _SYNC_HN_VP)*/)
        {
            if (ABS(usHighPulsePeriod ,0x1f) < 3) // CSW+ 0980527 Should be 1280*768 60 not 1024*768 60Hz
                return _FALSE;
        }
        else if (mode_index == _MODE_1152x864_60HZ)
        {
            if (ABS(stModeInfo.IVCount, tINPUTMODE_PRESET_TABLE[mode_index].IVTotal) >3)
                return _FALSE;
        }
        else if (mode_index == _MODE_1152x864_75HZ)
        {
            if (ABS(stModeInfo.IVCount,914) < 3)
                return _FALSE;
        }
        else if (mode_index == _MODE_1280x720_60HZ)
        {
            if (ABS(usHighPulsePeriod ,0x31) > 5) // Should be VGA 720p not VGA 1280*720
                return _FALSE;
        }
//		else if(mode_index == _MODE_1280x768_60RHZ){
//			if(ABS(stModeInfo.IVCount,tINPUTMODE_PRESET_TABLE[mode_index].IVTotal) > 3)
//				return _FALSE;;
//		}
        else if (mode_index == _MODE_1280x768_60HZ)
        {
            if (ABS(stModeInfo.IVCount, tINPUTMODE_PRESET_TABLE[mode_index].IVTotal) >2)
                return _FALSE;
        }
        else if (mode_index == _MODE_1280x800_60HZ)
        {
            if (ABS(stModeInfo.IVCount,tINPUTMODE_PRESET_TABLE[mode_index].IVCount) > 3)
                return _FALSE;;
        }
        else if (mode_index == _MODE_1280x1024_60HZ)
        {
            if ((ABS(stModeInfo.IHSyncPulseCount,6) < 10) || (ABS(stModeInfo.IHSyncPulseCount, 386) < 10))
                return _FALSE;
        }

        else if (mode_index == _MODE_1440x900_60HZ)
        {
            if (ABS(stModeInfo.IVCount,tINPUTMODE_PRESET_TABLE[mode_index].IVCount) > 3)
                return _FALSE;;
        }
        //<Stanley K90304> confuse timing between 720x4000@70Hz & 640x400@70, please check another way to solve this confuse.
        //=======================================================================================//
        /*
        else if(mode_index == _MODE_1024x768_72HZ){
        	if(ABS(stModeInfo.IHSyncPulseCount,35) > 10)
        		return _FALSE;
        }
        */
        //=======================================================================================//

        else if (mode_index == _MODE_1440x900_60HZ)
        {
            if (ABS(stModeInfo.IVCount, tINPUTMODE_PRESET_TABLE[mode_index].IVTotal) >3)
                return _FALSE;
        }
//		else if((mode_index == _MODE_1400x1050_50HZ)||(mode_index == _MODE_1400x1050_60RHZ)||(mode_index == _MODE_1400x1050_60HZ)||(mode_index == _MODE_1400x1050_75HZ)){
//				return _FALSE;//use 1680x1050 display
//		}
        else if (mode_index == _MODE_1680x1050_60HZ)
        {
            if (ABS(stModeInfo.IVCount, tINPUTMODE_PRESET_TABLE[mode_index].IVTotal) >3)
                return _FALSE;
        }
        else if (mode_index == _MODE_1600x1200_60HZ)
        {
            if (ABS(stModeInfo.IVCount, tINPUTMODE_PRESET_TABLE[mode_index].IVTotal) >3)
                return _FALSE;
        }

    }
    else
    {
        if (mode_index == _MODE_640x400_70HZ)
        {
            return _FALSE;//display by 720x400@70Hz
        }
        else if ((mode_index == _MODE_640x350_70HZ) && (ABS(stModeInfo.IHSyncPulseCount,103) > 10))
        {
            return _FALSE;
        }
        else if (mode_index == _MODE_640x480_60HZ)
        {
            if (ABS(usHighPulsePeriod ,0x67) > 8) // Should be VGA 480p not VGA 640*480
                return _FALSE;
        }
        else if (mode_index == _MODE_640x480_75HZ)
        {
            if (ABS(usHighPulsePeriod ,0x38) > 1)
                return _FALSE;
        }
        else if (mode_index == _MODE_1024x768_60HZ)
        {
            if (ABS(usHighPulsePeriod ,0x39) > 8)
                return _FALSE;;
        }
        else if (mode_index == _MODE_1024x768_75HZ)
        {
            if (ABS(mode_get_IVTotal(),tINPUTMODE_PRESET_TABLE[mode_index].IVTotal) > 3)
                return _FALSE;
        }
        else if (mode_index == _MODE_1024x800_73HZ)
        {
            if (ABS(usHighPulsePeriod ,0x21) < 2)
                return _FALSE;
        }
        else if (mode_index == _MODE_1440x900_60HZ)
        {
            if (ABS(stModeInfo.IHSyncPulseCount,0x27) > 10)
                return _FALSE;
        }
        else if (mode_index == _MODE_1280x768_60HZ)
        {
            if (ABS(usHighPulsePeriod ,0x2c) > 3) // CSW+ 0970115 Should be 1366*768 60 not 1280*768 60Hz
                return _FALSE;
        }
        else if (mode_index == _MODE_1280x1024_60HZ)
        {
            if (ABS(usHighPulsePeriod ,0x1d) > 3)
                return _FALSE;
        }
        else if (mode_index == _MODE_1680x1050_60HZ)
        {
            if (ABS(usHighPulsePeriod ,0x21) > 3)
                return _FALSE;
        }
        else if (mode_index == _MODE_1280x768_60HZ_RB)
        {
            if (ABS(usHighPulsePeriod ,0x0e) > 3) // CSW+ 0970115 Should be 1366*768 60 not 1280*768 60Hz
                return _FALSE;
        }
        else if (mode_index == _MODE_1280x800_60HZ)
        {
            if (ABS(usHighPulsePeriod ,0x2a) > 3)
                return _FALSE;
        }

    }
    return _TRUE;
}


/**
 * This function is used for identify the YPbPr mode.
 *
 * @param mode_index: [input] the index of identified mode
 *
 * @return BIT
 * @retval TRUE :  The input mode is identified.
 * @retval FALSE: The identified mode is not the right one.
 *
 *
 */

UINT8 mode_identifymode(UINT8 modeno)
{
    UINT16 usHighPulsePeriod=0;
    UINT8 mode_index;
#if 0
    mode_index = mode_SearchModeMap(modeno);
#else
    mode_index = modeno-2;
#endif
    RTD_Log(LOGGER_DEBUG,"mode_index = %x\n",(UINT32)mode_index);
    if (info->input_src==0)//if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
    {
        // CSW+  add usHighPulsePeriod comparison to enhace mode identification
        if (ABS(stModeInfo.IHFreq , tINPUTMODE_PRESET_TABLE[mode_index].IHFreq) > tINPUTMODE_PRESET_TABLE[mode_index].IHFreqTolerance)
            return FALSE;
        if (ABS(stModeInfo.IVFreq , tINPUTMODE_PRESET_TABLE[mode_index].IVFreq) > tINPUTMODE_PRESET_TABLE[mode_index].IVFreqTolerance)
            return FALSE;

        //tINPUTMODE_PRESET_TABLE[mode_index].PolarityFlag = tINPUTMODE_PRESET_TABLE[mode_index].PolarityFlag & Scaler_DispGetInputInfo(SLR_INPUT_POLARITY);

#ifdef CONFIG_VGA_SUPPORT_SOG
        if ((VGADetecSyncTypeMode != SOG_MODE)&&(VGADetecSyncTypeMode != CS_ON_H_MODE))//SOG and CS on H always _SYNC_HP_VP,don't need check polarity
            if ((tINPUTMODE_PRESET_TABLE[mode_index].PolarityFlag & info->Polarity) == 0 )
            {
                RTD_Log(LOGGER_DEBUG,"mode_index = %d\n",mode_index);
                RTD_Log(LOGGER_DEBUG,"\ntINPUTMODE_PRESET_TABLE[mode_index].PolarityFlag = %x\n",tINPUTMODE_PRESET_TABLE[mode_index].PolarityFlag);
                RTD_Log(LOGGER_DEBUG,"info.Polarity = %x\n",info->Polarity);
                return FALSE;
            }
#else
        if (VGADetecSyncTypeMode != CS_ON_H_MODE)//SOG and CS on H always _SYNC_HP_VP,don't need check polarity
            if ((tINPUTMODE_PRESET_TABLE[mode_index].PolarityFlag & info->Polarity) == 0 )
            {
                RTD_Log(LOGGER_DEBUG,"mode_index = %d\n",mode_index);
                RTD_Log(LOGGER_DEBUG,"\ntINPUTMODE_PRESET_TABLE[mode_index].PolarityFlag = %x\n",tINPUTMODE_PRESET_TABLE[mode_index].PolarityFlag);
                RTD_Log(LOGGER_DEBUG,"info.Polarity = %x\n",info->Polarity);
                return FALSE;
            }
#endif

        usHighPulsePeriod = drv_measure_HighPulsePeriod();
        RTD_Log(LOGGER_DEBUG,"####### usHighPulsePeriod = %x\n",usHighPulsePeriod);
        if (mode_judge_confuse_mode(mode_index, usHighPulsePeriod)==_FALSE)
        {
            RTD_Log(LOGGER_DEBUG,"\n usHighPulsePeriod fail, usHighPulsePeriod=%x", usHighPulsePeriod);
            return _FALSE;
        }

    }
    else
    {
        RTD_Log(LOGGER_DEBUG,"stModeInfo.IHFreq = %d\n",(UINT32)stModeInfo.IHFreq);
        RTD_Log(LOGGER_DEBUG,"stModeInfo.IVFreq = %d\n",(UINT32)stModeInfo.IVFreq);
        RTD_Log(LOGGER_DEBUG,"tINPUTMODE_PRESET_TABLE[mode_index].IHFreq = %d\n",(UINT32)tINPUTMODE_PRESET_TABLE[mode_index].IHFreq);
        RTD_Log(LOGGER_DEBUG,"tINPUTMODE_PRESET_TABLE[mode_index].IVFreq) = %d\n",(UINT32)tINPUTMODE_PRESET_TABLE[mode_index].IVFreq);
        if (ABS(stModeInfo.IHFreq , tINPUTMODE_PRESET_TABLE[mode_index].IHFreq) > tINPUTMODE_PRESET_TABLE[mode_index].IHFreqTolerance)
            return FALSE;
        if (ABS(stModeInfo.IVFreq , tINPUTMODE_PRESET_TABLE[mode_index].IVFreq) > tINPUTMODE_PRESET_TABLE[mode_index].IVFreqTolerance)
            return FALSE;
        /* if(GET_INPUTSOURCE_TYPE() == _SOURCE_HDMI) {
        	if(ABS(stModeInfo.IVCount , tINPUTMODE_PRESET_TABLE[mode_index].IVTotal) > 10)
        		return FALSE;
        	if(ABS(stModeInfo.IHCount , tINPUTMODE_PRESET_TABLE[mode_index].IHTotal) > 10)
        		return FALSE;
         }*/
    }

    return TRUE;
}
#endif

#if 0

/**
 * This function is used for search index in mode table
 *
 * @param : [input] modeno
 * @return : index in mode table
 *
 */
UINT8 mode_SearchModeMap(UINT8 modeno)
{
    UINT8 count = 0;
    do
    {
        if (GET_MODE_INPUT_MODE(tINPUTMODE_PRESET_TABLE[count].ModeCurr) == modeno)
            return count;
        count++;
    }
    while (count < stModeTableTypeTable.num);

    return 0xff;
}


/**
 * This function is used for mode searching
 *
 * @param : [input] None
 * @param st263xInfo->Mode_Curr: [output] Indicate the current mode.
 *
 */
static UINT8 mode_searchmode(void)
{
    UINT8 search_cnt=0;
    UINT16 start_index=0, end_index=0;
    RTD_Log(LOGGER_DEBUG,"mode_searchmode start!!\n");

    if (info->input_src == 0)//if(GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
    {
        RTD_Log(LOGGER_DEBUG,"mode_searchmode VGA\n");
        start_index =  _MODE_640x350_70HZ;
        end_index = _MODE_1080P30;
    }
    else if (info->input_src == 6)//else if (GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR)
    {
        RTD_Log(LOGGER_DEBUG,"mode_searchmode YPBPR\n");
        start_index =  _MODE_480I;
        end_index =  _MODE_1080P30;
    }

    for (search_cnt = start_index; search_cnt <= end_index; search_cnt++)
    {
        RTD_Log(LOGGER_DEBUG,"mode_searchmode search_cnt= %x\n",(UINT32)search_cnt);
        if (mode_identifymode(search_cnt))
        {
            //interlace setting
            search_cnt = search_cnt;
            if ((search_cnt == _MODE_480I)  || (search_cnt == _MODE_576I) ||
                    (search_cnt == _MODE_1080I25) || (search_cnt == _MODE_1080I30))
            {
                RTD_Log(LOGGER_DEBUG,"SET_INTERLACE_IN_INPUT_MODE\n");
                SET_INTERLACE_IN_INPUT_MODE();
            }
            else
                CLR_INTERLACE_IN_INPUT_MODE();

            RTD_Log(LOGGER_DEBUG,"\n mode_searchmode OK,mode id%d\n",search_cnt);
            RTD_Log(LOGGER_DEBUG,"Interlace mode =%x\n",GET_INTERLACE_IN_INPUT_MODE());
            stModeInfo.ModeCurr = search_cnt;

            return search_cnt;
        }
    }

    return  _MODE_NOSUPPORT;
}
#endif
/*============================================================================*/
/**
 * flow_mode_DisableChannelProc
 * Force to background color
 *
 * @param <void>	{ void }
 * @return 			{ void }
 *
 */
void flow_mode_PoweroffProc(void)
{
    // Force to Background
    rtdf_setBits(MAIN_DISPLAY_CONTROL_RSV_0x2100, _BIT1);
    rtdf_setBits(DOUBLE_BUFFER_CTRL_0x2028, _BIT2 | _BIT0);

    // Disable Watchdog in D-domain
    flow_display_Disable_Watchdog();

    // Disable Watchdog in on-line measure
    flow_mode_Disable_OnMeasure_Watchdog();

    ///////////////////////////////////////////////////////
    rtdf_maskl(DISPLAY_TIMING_CTRL1_0x2000, ~(_BIT26 | _BIT29), 0);
}

void flow_mode_DisableChannelProc(void)
{

    // Disable Watchdog in D-domain
    flow_display_Disable_Watchdog();

    // Disable Watchdog in on-line measure
    flow_mode_Disable_OnMeasure_Watchdog();

    //set Free Run mode
    flow_display_FreeRun_Mode();

    // Disable Sample Clock in VGIP
    //drv_vgip_set_sample(_FALSE);
}

/*============================================================================*/
/**
 * flow_mode_DisableChannelProc
 * Force to background color
 *
 * @param <void>	{ void }
 * @return 			{ void }
 *
 */
void flow_mode_ResetMode(void)
{
    // Set free run avoid not to catch vs
    drv_scaler_set_display_mode(DISPLAY_MODE_FRC); //set FRC mode

    // Disable Measure Watchdog
    drv_scaler_enable_measure_wdg(_DISABLE);

    // Disable Sample Clock in VGIP
    drv_vgip_set_sample(_DISABLE);

    // Set default value to VGIP
    drv_vgip_Set_Default();

    //set polarity to default: H-positive, V-positive
    info->Polarity = _SYNC_HP_VP;

    // Set IVS Delay in Vgip to 0
    info->IPV_DLY_PRE = 0;

    // Disable on-line measure
    drv_measure_Disable_OnMeasure();

    // Clear interlace flag for signal
    CLR_INTERLACE_IN_INPUT_MODE();

    //measure IVS2DVS delay may change DVST,so need reset default
    drv_scalerdisplay_DVST_reset();

}

/*============================================================================*/
/**
 * flow_mode_DisableChannelProc
 * Force to background color
 *
 * @param <void>	{ void }
 * @return 			{ void }
 *
 */
void flow_mode_SetVGIP(void)
{
    // restore original signal's info
    drv_vgip_SetVSDelay(info->IPV_DLY_PRE);

#ifdef CONFIG_ENABLE_3D_SETTING
//-->w	fw_mode_decide3D_for_vdc();	--> not used in sparrow
#endif
    //If source is FR, handle display ratio at movie or caption
    drv_vgip_cal_vgip_capture();

    /* setup capture window */
    drv_vgip_set_vgip_capture();


    // CSW+ 0970812 Use smart-fit to double check ABL window setting
    //drv_abl_YPbPr_Setting_Finetune();

}

/*============================================================================*/
/**
 * flow_mode_SetIDomain
 * Force to background color
 *
 * @param <void>	{ void }
 * @return 			{ void }
 *
 */
void flow_mode_SetIDomain(void)
{
    //  decide which channel to use DI  --> not used in sparrow
    //	modestate_decide_DI();

    //	It use framesync uniformly in sparrow
    //if((info->display == _MAIN_DISPLAY)&&(info->FrameSyncFlag != _TRUE))
    //	drvif_scaler_set_display_mode(DISPLAY_MODE_FRC);

    // Calculate main or sub display width and display height and set it to info
    // remember main's display width/height may change later if display ratio is
    // pointer to pointer
    drv_scalerdisplay_get_display_size();

    // Calculate the output size of scale
    flow_scaler_Decide_ScaleSize();

    /* Decide RTNR flag */ //	-->  not used in sparrow
    //	modestate_decide_rtnr();

    // Set in the scale down
    flow_color_ultrazoom_SetScalingDown();

    //DI control setting	--> not used in sparrow
    //	fw_scalerip_set_di();

    //  should move to souce.c	--> seem for sub display specially
    //CSW+ 0970710 Sub Channel(480i or 576i or 1080i50 or 1080i60) interlace mode by video compensation
    // modestate_setup_vgip_even_odd_toggle();

    // Weihao 20081031 added	--> not used in sparrow
    // modestate_check_if_tvbus_reset();
}

bit flow_mode_CheckSignalStable(void)
{
    if (drv_measure_onlinemeasure() != _TRUE)
    {
        RTD_Log(LOGGER_DEBUG,"Mode on measure error\n");
        return _FALSE;
    }

    return _TRUE;
}

void flow_mode_Timing_Change_Init(void)
{
    if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
        drv_sig_vga_Init();
    else if (GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR)
        drv_sig_ypbpr_Init();
}

#if 0
/**
 * This API is used to progress the YPbPr mode detection.
 * after getting the measure result, if no overflow or timeout happened,
 * YPbPrSyncType will not set to NO_SIGNAL
 * and YPbPr_ModeSearch will be called to fine a valid YPbPr mode
 *
 * @param [input] : None
 * @param ppNextCmd: [output] The pointer to next command.
 *
 *
 * @return : result
 * @retval : TRUE : Detect a valid mode
 * @retval : FALSE:
 */
//suppose sync detector has decoded the valid sync
UINT8 flow_mode_detectmode(UINT8 source)
{
    UINT8 result = _MODE_NOSIGNAL;
    UINT32 log;
    RTD_Log(LOGGER_DEBUG,"drvif_mode_detectmode\n");

    if (source == _OFFLINE_MS_SRC_HDMI)
    {
        //rtdf_clearBits(HDMI_TMDS_OUTCTL_ADDR, _BIT2);
        //fw_timer_event_DelayXms(100);
        if (mode_hdmidetect()!= _MODE_SUCCESS)
        {
            //rtdf_clearBits(HDMI_TMDS_OUTCTL_ADDR, _BIT2);
            //fw_timer_event_DelayXms(100);
            goto error_result;
        }
    }
    else
    {
        if (drv_measure_offlinemeasure(source, ANALOG_MODE_MEASUREMENT, &stModeInfo) != _MODE_SUCCESS)
            goto error_result;
    }

    //debug_temp();

    result =mode_searchmode();

    if ((result != _MODE_NOSUPPORT) && (result != _MODE_NOSIGNAL))
    {
        RTD_Log(LOGGER_DEBUG,"\ndrvif_mode_detectmode result = %d\n", (UINT32)result);
        info->ucMode_Curr = result;
        info->IHSyncPulseCount = stModeInfo.IHSyncPulseCount;

        info->Hsync = stModeInfo.IHCount;
        info->Vsync = stModeInfo.IVCount;
        mode_setinfofromtable(result);
    }
    else
    {
        RTD_Log(LOGGER_DEBUG,"drvif_mode_detectmode error!!!\n");
        goto error_result;
    }

    //remove SYNCPROCESSOR_SP1_STBRST0_VADDR status check, no help

    return _MODE_SUCCESS;

error_result:
    drv_syncproc_FunctionDisable();
    return result;
}
#endif
#if 0  // Temporily not use
void flow_mode_GetVGAModeCurrInfo(UINT8 modeIndex, StructScreenModeInfo* ptModeInfo)
{
    UINT8 xdata count;
    if (ptModeInfo != NULL)
    {
        count = 0;
        while (count < stVGAModeCurrInfoSet.nModeinfoNum)
        {
            if (stVGAModeCurrInfoSet.ptModeInfo[count].unIndex == modeIndex)
                break;
            count++;
        }
    }

    if (count >= stVGAModeCurrInfoSet.nModeinfoNum)
        count = 0;

    // Add copy function
}

void flow_mode_GetScreenModeCurrInfo(UINT8 srcIndex, StructScreenModeInfo* ptModeInfo)
{
    UINT8 xdata count;
    if (ptModeInfo != NULL)
    {
        count = 0;
        while (count < stScreenModeCurrInfoSet.nModeinfoNum)
        {
            if (stScreenModeCurrInfoSet.ptModeInfo[count].unIndex == srcIndex)
                break;
            count++;
        }
    }

    if (count >= stVGAModeCurrInfoSet.nModeinfoNum)
        count = 0;

    // Add copy function
}
#endif

void flow_mode_ClearOnMeasureflag(void)
{
    //rtdf_writeByte0(SP_MS1STUS_0x090c, (_BIT4|_BIT3|_BIT1|_BIT0)); //clear over range, polarity change status
    rtdf_setBits(SP_MS1STUS_0x090c, (_BIT7|_BIT6|_BIT5|_BIT4|_BIT3|_BIT1|_BIT0));
}

