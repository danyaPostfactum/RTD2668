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
#include "scaler_idomain\syncproc.h"
#include "scaler_idomain\auto_soy.h"
#include "scaler_idomain\vgip.h"
#include "scaler_idomain\measure.h"
#include "scaler_idomain\adc.h"
#include "scaler_idomain\abl.h"
#include "scaler_idomain\smartfit.h"
#include "scaler\scaler_pll.h"
#include "scaler\scaler.h"
#include "signal\sig_ypbpr.h"
#include "source.h"
#include "flow_struct.h"
#include "flow_scaler.h"
#include "reg_def.h"
#include "mode.h"
#include "mode_customer.h"
#include "rosprintf.h"
#include "autoconf.h"
#include "timer_event.h"
#include "sparrow.h"
#include "ui_struct.h"
#include "osdcore.h"
#include "framesync.h"
#include "modestate.h"
#include "pcb_customer.h"
#include "pcb\pcb_utility.h"

// Definitions of ADC Fine Tune Delay (Value == 0 ~ 7)
//--------------------------------------------------
#define _ADC_FINE_TUNE_DELAY_RED        (0) //_C_ADC_FINE_TUNE_DELAY_RED//0
#define _ADC_FINE_TUNE_DELAY_GREEN    (0)// _C_ADC_FINE_TUNE_DELAY_GREEN//0
#define _ADC_FINE_TUNE_DELAY_BLUE       (0)//_C_ADC_FINE_TUNE_DELAY_BLUE//0

#define _ADAPTIVE_STEP		2
#define _ADAPTIVE_TIMES		6
#define	_SOY_MANUAL_COMPARE_LEVEL_MAX _SOY_MANUAL_COMPARE_LEVEL+(_ADAPTIVE_STEP*(_ADAPTIVE_TIMES>>1))


UINT8 ucIPH_Max_Margin=0;
UINT8 ucIPH_Min_Margin=0;
UINT8 ucIPV_Max_Margin=0;
UINT8 ucIPV_Min_Margin=0;


UINT8 ucIVS2DVSDelay=0;
UINT8 IVS2DVSDelay_VGA = 0;
UINT8 ucIHSDelay=0;

//[Code Sync][AlanLi][0980403][1]
//VGA support composite sync
UINT8 VGADetecSyncTypeMode = 0;
typedef struct
{
    UINT8 FirstAuto;                 // First Auto
    UINT16 HPosition;                 // Horizontal Position
    UINT16 VPosition;                 // Vertical Position
    UINT16 Clock;                     // Clock Adjust
    UINT8 Phase;                     // Phase Adjust

} StructModeUserDataType;

static StructModeUserDataType          	   stModeUserData;

UINT32 code VGA_Init[] =
{
    1, ADC_POWER_0x0400, 0x000000ff,// enable ADC  power
    1,SP_ADC_OUT_SEL_0x076c, 0x00000000, // Source select of SyncProc output => ADC Mode
    1,SP1_CLMPMSK_0x0740, 0x00000000, // CSW+ 0961204 for clamp mask disable
    // ADC
    //1,ADC_VBIAS_0x040c, 0x00000842, //SH boot enable, 0.8 SH boot adjust, RGB0, 1.5 regulator adjust, Bandgap Voltage 1.25
    1,PLL_SET_0x0500, 0x03030115,    //forster modified 071116
    1, ABL_CTRL_0x0c00, 0x00000000,//Disable ABL first
    1,ABL_WINDOW_0x0c04, 0x00000000,//Disable ABL first
    1,PLL_CTRL_0x0504, 0x00000208,
    1,PLLDIV_0x0508, 0x00000697,
    1,PLLPHASE_CTRL_0x050c, 0x00400000,
    1,DDS_MIX_W1_0x051c, 0x00ffff00, //P_code_max[16:0]
    1,DDS_MIX_W2_0x0520,0x000002A0,
//	1,SP1_CTRL_0x0700, 0x00020402,  //FBHS - Feedback HS,  pad_hs1, pad_vs1
    1,SP1_CTRL_0x0700, 0x00000402,  //AHS prevent adjust clock image flick,  pad_hs1, pad_vs1
    1,SP1_STBCTRL_0x0710, 0x00003200, // Set Capture window tolerance +-6
    1,SP1_DCNT_0x0728, 0x00000980, // Set VSync counter level
    1,SP1_CLMPCTRL_0x0738, 0x00000012, // 1 :_BIT5 => ADC_Clk,  ~_BIT5 => XTal_Clk, 2: Clamp source from CS_Raw, Trailing edge of input HS
    1,SP1_CLMP_0x073c, 0x04100410, // Y/nY Clamp start/end
    1,SP1_CLMPCTRL_0x0738, 0x00000092, // 0x00,	      SP1_AutoRst
    1, DUMMY_0x0438, 0x00000000,// ADC clamp type
    1, ADC_CLOCK_0x0410, 0x00000001,// LPF select = 10MHz (MSB)
    _TBL_END

};

UINT32 code YPBPR_INIT[] =
{
    1, ADC_POWER_0x0400, 0x000000ff,// enable ADC  power
    1, ADC_CLOCK_0x0410, 0x00000001,// LPF select = 10MHz (MSB)

    1, SP1_CTRL_0x0700, 0x00020000,  //FBHS - Feedback HS
    1, SP1_DCNT_0x0728, 0x216d	, //chroma canaling debounce
//	1, ADC_CTL_0x0414, 0x84c8c840,   // LPF select = 10MHz (LSB)
//	1, ADC_VBIAS_0x040c, 0x00000941,  // ADC Input source select = RGB 1	//<--need modify

//	1, ADC_DCRESTORE_CTRL_0x0418, 0x00000038,	// Input RGB1 DC Restore enable
//	1, ADC_CLAMP_CTRL0_0x041c, 0xabab0049,  // SOY0 source select = B1, DC Restore = 1M ohms //<--need modify
//	1, ADC_CLAMP_CTRL1_0x0420, 0x00000000,  // Clamp disable

//	1, SP_ADCCLAMPSEL0_0x0764, 0x00004100,  // Clamp source select
    1, SP_ADC_OUT_SEL_0x076c, 0x00000000,  // SP output = ADC data , HS/VS/Field source = ADC
    1, SP1_CLMPMSK_0x0740, 0x00008202, // Clamp mask enable

//	1, ABL_CTRL_0x0c00, 0x00000000,//Disable ABL first

//	1, PLL_SET_0x0500, 0x03030103, // DDS_PLL_PLL_SET_REGISTER_VADDR // M=6,N=3,K=0
    1, PLL_CTRL_0x0504, 0x00000000, // DDS_PLL_PLL_CTRLPLL_REGISTER_VADDR // enable APLL & dual clk
//	1, PLLDIV_0x0508, 0x00000359, // Divider=858
    1, PLLPHASE_CTRL_0x050c, 0x00530000,// ADC clk=(1/4)*VCO clk
    1, DDS_MIX_W1_0x051c, 0x3009a733,// P code max=316321 and new mode disable
    1, DDS_MIX_W2_0x0520, 0x000002a0,// Delay chain select (Default value)
    1, ABL_CTRL_0x0c00, 0x00000000,//Disable ABL first
    1, DUMMY_0x0438, 0x00000000,// ADC clamp type

    _TBL_END
};

#ifdef CLAMP_NEW_CLAMP_POSITION
//480i, 576i, 480p, 576p, 720px50, 720px60, 1080ix25, 1080ix30, 1080px50, 1080px60,  1080px23 ,  1080px24,  1080px25,  1080px29,  1080px30
UINT8 CLAMP_START_TABLE[] = {0x29, 0x2B, 0x29, 0x2B, 0x4D,    0x4D,    0x45,     0x45,     0x45,     0x45,      0x45,       0x45,      0x45,      0x45,      0x45};
UINT8 CLAMP_END_TABLE[]   = {0x33, 0x36, 0x33, 0x36, 0x72,    0x72,    0x5D,     0x5D,     0x5D,     0x5D,      0x5D,       0x5D,      0x5D,      0x5D,      0x5D};
#endif //#ifdef CLAMP_NEW_CLAMP_POSITION
#if 0 // not used
UINT8 drv_syncproc_Get_Sync_Type(void)
{
    return ((rtdf_readByte0(SP1_AUTORST_0x0704) & (_BIT4| _BIT5 | _BIT6)) >>4);
}
#endif

UINT8 drv_syncproc_Get_SOY_Path(UINT8 input_index)
{
    if (input_index == _SOURCE_VGA)
        return VGA_SOG_PATH_SELECT;
    else if (input_index == _SOURCE_YPBPR)
        return YPBPR_SOY_PATH_SELECT;
    else
        return VGA_SOG_PATH_SELECT;
}

UINT8 drv_syncproc_Capture_Window_Tolerance ()
{
    if (GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR)
    {
        switch (info->ucMode_Curr)
        {
        case _MODE_480I:
        case _MODE_576I:
        case _MODE_480P:
        case _MODE_576P:
            rtdf_maskl(SP1_STBCTRL_0x0710, ~0xF800, (UINT32)0x05<<11);		// capture win 20 for SD
            break;
        case _MODE_720P50:
            rtdf_maskl(SP1_STBCTRL_0x0710, ~0xF800, (UINT32)0x08<<11);
            break;
        case _MODE_1080I25:
        case _MODE_1080I30:
        case _MODE_1080P50:
        case _MODE_1080P60:
        default:
            rtdf_maskl(SP1_STBCTRL_0x0710, ~0xF800, (UINT32)0x06<<11);		// capture win 24 for HD
            break;
        }

        /*if(info->ucMode_Curr== _MODE_480P)
        	rtdf_maskl(SP1_STBCTRL_0x0710, ~0xF800, (UINT32)0x04<<11); // CSW+ 0980216 To set capture window tolerance to +- 19 clock
        else if(info->ucMode_Curr == _MODE_576P)
        	rtdf_maskl(SP1_STBCTRL_0x0710, ~0xF800, (UINT32)0x04<<11); // CSW+ 0980216 To set capture window tolerance to +- 19 clock
        else if(info->ucMode_Curr == _MODE_1080I25)
        	rtdf_maskl(SP1_STBCTRL_0x0710, ~0xF800, (UINT32)0x04<<11); // CSW+ 0980216 To set capture window tolerance to +- 19 clock
        else if(info->ucMode_Curr == _MODE_1080I30)
        	rtdf_maskl(SP1_STBCTRL_0x0710, ~0xF800, (UINT32)0x04<<11); // CSW+ 0980216 To set capture window tolerance to +- 19 clock
        else if(info->ucMode_Curr == _MODE_1080P50)
        	rtdf_maskl(SP1_STBCTRL_0x0710, ~0xF800, (UINT32)0x04<<11); // CSW+ 0980216 To set capture window tolerance to +- 19 clock
        else if(info->ucMode_Curr == _MODE_1080P60)
        	rtdf_maskl(SP1_STBCTRL_0x0710, ~0xF800, (UINT32)0x05<<11); // CSW+ 0980216 To set capture window tolerance to +- 19 clock
        else
        	rtdf_maskl(SP1_STBCTRL_0x0710, ~0xF800, (UINT32)0x0A<<11); // CSW+ 0980216 To set capture window tolerance to +- 19 clock
        */
    }
    return 1;
}


/**
 * This function is called to get the stable period.(Continue 64 times to get the same H period)
 *
 * @param indicator: [input] sync processor 1 or sync processor 2
 * @param st263xInfo->YPbPrSyncType: [output] When overflow or stable period change or stable polarity change, SyncType will set to SYNC_CHANGE
 *
 *
 * @return INT16
 * @retval result : the stable period result
 *
 * @Note : This function should be called after calling the YPbPr_SyncDetector
 *
 */
static UINT16 SyncProc_GetStablePeriod(void)
{
    UINT16 result = 0;
    UINT8 value = 0;
    UINT32 log;

    rtdf_clearBits(SP1_STBCTRL_0x0710, _BIT0|_BIT7);//clear start measure & popup
    rtdf_setBits(SP1_STBCTRL_0x0710, _BIT0);//stable measure start

    fw_timer_event_DelayXms(30);//wait a little time

    value = rtdf_readByte3(SP1_STBRST0_0x0714);

    if (value & (_BIT7|_BIT6|_BIT5)) //check HS Overflow, Stable Period Change, Stable Polarity Change
    {
        RTD_Log(LOGGER_DEBUG,"\nHS Overflow, Stable Period Change, Stable Polarity Change\n");
        goto error_result;
    }

    rtdf_setBits(SP1_STBCTRL_0x0710, _BIT7);//pop up result
    value = rtdf_readByte2(SP1_STBRST0_0x0714);

    if ((value & _BIT7) == 0) //check stable flag
    {
        RTD_Log(LOGGER_DEBUG,"\nHS stable flag error\n");
        goto error_result;
    }

    // CSW* 0960613 value should be UINT16
    result = rtdf_readWord1(SP1_STBRST1_0x0718);
    result = result & 0x3ff;//stable period
    log = result;
    RTD_Log(LOGGER_DEBUG,"\nStable Period = %x",log);
    log = rtdf_readWord0(SP1_STBRST1_0x0718);
    RTD_Log(LOGGER_DEBUG,"\nStable Hight Pulse Period = %x\n",log);
    return result;

error_result:
    return 0;
}

/**
 * This function is called to set the VSync counter level
 *
 * @param indicator: [input] sync processor1 or sync processor2
 *
 * @return None
 *
 */
static UINT8 SyncProc_SetVsyncCounterLevel(void)
{
    UINT16 result = 0;
    result = SyncProc_GetStablePeriod();
    if (result == 0)
        return _MODE_NOSUPPORT;
    //frank@0529 the below formula is the tuning result of the vsync count level for any timing
    result = (result >> 3) + 150;

//	RTD_Log(LOGGER_DEBUG,"Vsyn Level = 0x%x",result);

    rtdf_maskl(SP1_DCNT_0x0728, ~0x7ff, (result & 0x7ff));
    return _MODE_SUCCESS;
}

/**
* This function is used for setting the ADC phase
*
* @param phase: [input] Set the selected phase
*
* @return None
*
*/
// Forster+ 0961204 Solve jump phase error

void drv_syncproc_YPbPr_SetPhase(UINT8 phase, UINT8 uc64phase_limited)
{

    UINT8 ucDanger_distance,ucVCO_Divider4=0, ucDanger_tolerance= 20,ctrl,ucDanger_position;
//	UINT16 usSelect, temp = 0;
    UINT16 temp = 0;
    UINT8 PhaseComp = 0;
    UINT8 PhaseMax = 0;
    UINT32 pixelclock;
    UINT32 PhaseDelayICvG = 100;// Dragon IC version G phase delay = 10.0ns

    //RTD_Log(LOGGER_DEBUG,"\ninfo->ADC_CLOCK = %d\n",(UINT32)info->ADC_CLOCK);
    //RTD_Log(LOGGER_DEBUG,"\nstModeInfo.IHCount = %d\n",(UINT32)stModeInfo.IHCount);

    rtdf_maskl(ADC_CLOCK_0x0410, ~_BIT13, _BIT13);

    if (stModeInfo.IHCount)
        pixelclock = (UINT32) CONFIG_CPU_XTAL_FREQ/1000 * info->ADC_CLOCK /stModeInfo.IHCount;  // Standard pixel clock in kHz
    else
        return;

    RTD_Log(LOGGER_DEBUG,"\n YPbPr_SetPhase after progressing2...!");
    // Code below is to select stable HSYNC latch edge.
    // There is about 2.8 ns delay between input clock into ADC and output from ADC.
    // Calculating the corresponding phase delay for 2.8 ns
    // Original Formula : Calculate the corresponding phase
    // ucSelect = 64 * 2.8 / (1000000 / ulRate); //for ver C

    // Calculate pixel clock rate (round to MHz)
    pixelclock  = (((UINT32)info->IHFreq* (UINT32)info->IHTotal) * 2 / (1000 * 10));
    pixelclock  = (pixelclock >> 1) + (pixelclock & 0x01);

    //[Code Sync][AlanLi][0980617][1]
    ucVCO_Divider4 = select_adc_vco_div==4? 1 :  0;
    //[Code Sync][AlanLi][0980617][1][end]

#if 0
    PhaseComp = 64 * PhaseDelayICvG * pixelclock / (100*1000);
#else
    if (pixelclock>100)
        ucDanger_tolerance=10;
    else if (pixelclock>52)
        ucDanger_tolerance=8;
    else
        ucDanger_tolerance=3;

    if (pixelclock<100)
        PhaseComp = 64-((pixelclock*32+3100)/100);
    else
        PhaseComp = 64-((pixelclock-100)*29/100);
#endif

    RTD_Log(LOGGER_DEBUG,"\n $pixelclock=%d \n  $PhaseComp = %d\n",(UINT32)pixelclock,(UINT32)PhaseComp);

    if (uc64phase_limited)
    {
        if (phase > 63)
            phase = 126 - phase;
    }

    //ucDanger_distance = IoReg_ReadByte1(IDOMAIN_PLLDIV_VADDR);


    if (ucVCO_Divider4 == 1)//vco divider = 4;
    {
        if (uc64phase_limited)
        {
            phase = (UINT32)phase << 1;
            PhaseComp = (UINT32)PhaseComp << 1;
            ucDanger_tolerance = (UINT32)ucDanger_tolerance << 1;
        }
        PhaseMax = 128;
    }
    else
        PhaseMax = 64;

    //usSelect = ucVCO_Divider4 ?   (66 - PhaseComp)  :  (33 -PhaseComp) ;

    //Calculate the absolute value from the selected phase to transition
    //ucDanger_distance = (phase >= usSelect) ? phase - usSelect : usSelect - phase;
    ucDanger_position = PhaseMax-PhaseComp;

    if (ucDanger_position >= phase)
        ucDanger_distance = ((ucDanger_position - phase)<(PhaseMax+phase-ucDanger_position))?(ucDanger_position - phase):(PhaseMax+phase-ucDanger_position);
    else
        ucDanger_distance = ((phase - ucDanger_position)<(PhaseMax+ucDanger_position-phase))?(phase - ucDanger_position):(PhaseMax+ucDanger_position-phase);

    ctrl = (ucDanger_distance >= ucDanger_tolerance) ? 0x00 : 0x01;
// CF 20090901, use source type instead with source index ->	//if(info->input_src == _SOURCE_INDEX_VGA)
    if ( GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
        temp = _H_POSITION_MOVE_TOLERANCE + stScreenModeCurrInfo.H_Position - 0x80;
    else
        temp = 0;

    RTD_Log(LOGGER_DEBUG,"\n$phase=%d \n$ucDanger_distance=%d \n$ucDanger_position=%d \n$ctrl=%d\n",(UINT32)phase,(UINT32)ucDanger_distance,(UINT32)ucDanger_position,(UINT32)ctrl);
    //RTD_Log(LOGGER_DEBUG,"\n temp Pre = %x",(UINT32)temp);

    if (((ctrl==0x00)&&(phase > ucDanger_position))||((ctrl==0x01)&&(phase - ucDanger_position > ucDanger_tolerance)&&(phase>ucDanger_position)))
    {
        temp = temp + 1;
        RTD_Log(LOGGER_DEBUG,"temp+1\n");
    }

    if ((UINT32)(rtd_inl(VGIP_CHN1_DELAY_0x0a20) >> 16) != temp)
    {
        //RTD_Log(LOGGER_DEBUG,"\n phase WaitFor_IVS1");
        //WaitFor_IVS1();
    }
    rtdf_maskl(ADC_CLOCK_0x0410, ~(_BIT6 ), ctrl   ?_BIT6 : 0x00);
    drv_vgip_adjust_ihs_delay(temp);
    rtdf_setBits(PLLPHASE_CTRL_0x050c, _BIT22);//set phase through Look-Up-Table
    rtdf_maskl(PLLPHASE_CTRL_0x050c, ~0x0000007f,(phase & 0x0000007f));//set the phase

}



UINT8 drv_syncproc_YPbPr_GetPhase(UINT8 Mode_Curr)
{
    UINT8 ucTempPhase = 0;
    switch (Mode_Curr)
    {
    case _MODE_480P:

        //Rspi_WriteByte(_P1_7C_PLLPHASE_CTRL1, 0x1E);
        ucTempPhase = 0x1e;
        break;

    case _MODE_480I:

        //Rspi_WriteByte(_P1_7C_PLLPHASE_CTRL1, 0x05);
        ucTempPhase = 0x00;
        break;
    case _MODE_576P:

        //Rspi_WriteByte(_P1_7C_PLLPHASE_CTRL1, 0x1E);
        ucTempPhase = 0x00;
        break;

    case _MODE_720P50:

        //Rspi_WriteByte(_P1_7C_PLLPHASE_CTRL1, 0x05);
        ucTempPhase = 0x00;
        break;
    case _MODE_720P60:

        //Rspi_WriteByte(_P1_7C_PLLPHASE_CTRL1, 0x1E);
        ucTempPhase = 0x00;
        break;

    case _MODE_1080I25:

        //Rspi_WriteByte(_P1_7C_PLLPHASE_CTRL1, 0x05);
        ucTempPhase = 0x00;
        break;
    case _MODE_1080I30:

        //Rspi_WriteByte(_P1_7C_PLLPHASE_CTRL1, 0x1E);
        ucTempPhase = 0x00;
        break;

    case _MODE_1080P50:

        //Rspi_WriteByte(_P1_7C_PLLPHASE_CTRL1, 0x05);
        ucTempPhase = 0x00;
        break;
    case _MODE_1080P60:

        //Rspi_WriteByte(_P1_7C_PLLPHASE_CTRL1, 0x05);
        ucTempPhase = 0x00;
        break;

    }

    return ucTempPhase;
}


/**
 * This API is used for ADC setting which include the clockphasegain & offset
 *
 * @param : [input] None
 * @param : [output] None
 *
 * @return : None
 *
 */
void drv_syncproc_YPbPr_ADCSetting(void)
{
    UINT16 pixelclock;

    // Calculate pixel clock rate (round to MHz)
    pixelclock  = (((UINT32)info->IHFreq* (UINT32)info->IHTotal) * 2 / (1000 * 10));
    pixelclock  = (pixelclock >> 1) + (pixelclock & 0x01);

//	if (GET_DRAGON_IC_TYPE() == _DRAGON_IC_DRAGON_P)
//		rtdf_clearBits(ADC_CLOCK_0x0410, _BIT12);

    // ADC differential mode and Set ADC bandwidth to reduce high frequency noise
    rtdf_setBits(ADC_CTL_0x0414,_BIT27); //Sparrow move this setting to ADC_CTL Bit27

    if (pixelclock < 38)
    {
        // 75 MHz  100
        rtdf_maskl(ADC_CTL_0x0414,  ~(_BIT26|_BIT25|_BIT24), (_BIT26));
    }
    else if (pixelclock < 68)
    {
        // 150 MHz  101
        rtdf_maskl(ADC_CTL_0x0414,  ~(_BIT26|_BIT25|_BIT24), (_BIT26|_BIT24));
    }
    else if (pixelclock < 160)
    {
        // 300 MHz  110
        rtdf_maskl(ADC_CTL_0x0414,  ~(_BIT26|_BIT25|_BIT24), (_BIT26|_BIT25));
    }
    else   // 500MHz  111
    {
        rtdf_maskl(ADC_CTL_0x0414,  ~(_BIT26|_BIT25|_BIT24), (_BIT26|_BIT25|_BIT24));
    }


    //YPbPr_SetADCClock(info->INPUT_ADC_CLOCK);
    RTD_Log(LOGGER_DEBUG,"\n ADC setting in YPbPr");
    //YPbPr_ADCPowerControl(_ON);
//[Code Sync][AlanLi][0980617][1]
    drv_pll_select_VCO_div();
//[Code Sync][AlanLi][0980617][1][end]

    RTD_Log(LOGGER_DEBUG,"\ninfo->INPUT_ADC_CLOCK = %d\n",(UINT32)info->ADC_CLOCK);
    drv_pll_Set_AClk(info->ADC_CLOCK, FALSE);
    RTD_Log(LOGGER_DEBUG,"\n*************** ADC Reset = %x\n", (UINT32)rtdf_inl(PLLPHASE_CTRL_0x050c));

    //stModeUserData.Phase = drv_syncproc_YPbPr_GetPhase(info->ucMode_Curr);
    //RTD_Log(LOGGER_DEBUG,"\nstModeUserData.Phase = %x\n",(UINT32)stModeUserData.Phase);
    drv_syncproc_YPbPr_SetPhase(drv_syncproc_YPbPr_GetPhase(info->ucMode_Curr), _TRUE);

    //YPbPr_SetADCGainOffset();
    drv_adc_setadcgainoffset();

}

/**
 * This API is used to adjust adc clock
 *
 * @param [display] : main or sub channel
 * @param [fast_lock_enable] : fask clock enable or not
 *
 * @return : none
 */
void drv_syncproc_Adjust_ADC_Clock(UINT8 fast_lock_enable)
{
    UINT16 usTempClock;

    usTempClock = info->ADC_CLOCK + GET_CLOCK() - 0x80;     //updated the ADC IH_Total or the on-line measure will error
    info->IHTotal = usTempClock;
    RTD_Log(LOGGER_DEBUG,"usTempClock=%x\n", (UINT32)usTempClock);
    drv_pll_Set_AClk( usTempClock, fast_lock_enable);
    //mark by frank@0509  RtSystem_Sleep(20);
    drv_syncproc_ScalerSetHPosition();
}

void drv_syncproc_SOYSourceSelect(UINT8 soy_source_index)
{
    rtdf_maskl(SP1_CTRL_0x0700, ~0x0407,soy_source_index ? 0x0405 : 0x0404);

}

UINT8 drv_syncproc_SetClampPulsePos(UINT8 clamp_start, UINT8 clamp_end, UINT8 select) //select for Y or nY
{
    UINT32 getvalue;

    if (select == CLAMP_Y)
    {
        getvalue = rtdf_inl(SP1_CLMP_0x073c)& 0x0000ffff;
        getvalue = getvalue | (((UINT32)clamp_start << 24) | ((UINT32)clamp_end << 16));
    }
    else
    {
        getvalue = rtdf_inl(SP1_CLMP_0x073c)& 0xffff0000;
        getvalue = getvalue | (((UINT32)clamp_start << 8) | (clamp_end));
    }
    rtdf_outl(SP1_CLMP_0x073c,getvalue);

    //rtdf_outl(IDOMAIN_SYNCPROC_REG1805101C_VADDR, 0x0000009c);//enable double buffer load in
    rtdf_outl(SP1_CLMPCTRL_0x0738 , 0x0000001c);  //enable CLAMP signal output and select leading edge of HS for YPbPr(GBR), XTal_clk for SD source
    rtdf_outl(SP1_CLMPCTRL_0x0738 , 0x0000009c);  //enable load double buffer

    return TRUE;
}


/**
 * This API is used to set the YPbPr mode related setting, like clockphaseclamp pulseoutput clamp signalenable auto SOY level compare
 * enable ABLenable ADC
 * @param [input] : None
 * @param [output] : None
 *
 *
 * @return None
 *
 */
void drv_syncproc_YPbPr_ModeSetting(StructSourceInput *srcinput_pt)
{
    ADC_INITIAL_OPTIONS  *ptOptions;

    ptOptions =  srcinput_pt->InitOptions;

    drv_syncproc_YPbPr_ADCSetting(); //ADC clock has locked

    if (GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR)
    {
#ifdef CLAMP_NEW_CLAMP_POSITION
        rtdf_maskl(ADC_VBIAS_0x040c, ~(0x00007000),0);// Set Clamp ctrl voltage level 100 => 400 mV

        if (info->ucMode_Curr <= _MODE_1080P60)
        {
            rtdf_outl(SP1_CLMP_0x073c, (0xFF000000 & ((UINT32)CLAMP_START_TABLE[info->ucMode_Curr - _MODE_480I]<<24))
                      | (0x00FF0000 & ((UINT32)CLAMP_END_TABLE[info->ucMode_Curr - _MODE_480I]  <<16))
                      | (0x0000FF00 & ((UINT32)CLAMP_START_TABLE[info->ucMode_Curr - _MODE_480I]<<8))
                      | (0x000000FF & (UINT32)CLAMP_END_TABLE[info->ucMode_Curr - _MODE_480I]));
        }
        else
        {
            rtdf_outl(SP1_CLMP_0x073c, (0xFF000000 & ((UINT32)CLAMP_START_TABLE[_MODE_1080P60 - _MODE_480I]<<24))
                      | (0x00FF0000 & ((UINT32)CLAMP_END_TABLE[_MODE_1080P60 - _MODE_480I]  <<16))
                      | (0x0000FF00 & ((UINT32)CLAMP_START_TABLE[_MODE_1080P60 - _MODE_480I]<<8))
                      | (0x000000FF & (UINT32)CLAMP_END_TABLE[_MODE_1080P60 - _MODE_480I]));
        }
        //enable CLAMP signal output and select leading edge of HS for YPbPr(GBR), XTal_clk for SD source
        rtdf_maskl(SP1_CLMPCTRL_0x0738 , ~0x0000007f,((0x00000040&((UINT32)(CLAMP_CNT_UNIT)<<6))
                   | (0x00000020&((UINT32)(CLAMP_CLKSRC)<<5))
                   | (0x00000010&((UINT32)(_ENABLE)<<4))
                   | (0x00000008&((UINT32)(CLAMP_Y_REF_EDGE)<<3))
                   | (0x00000004&((UINT32)(CLAMP_NY_REF_EDGE)<<2))));

        //enable load double buffer
        rtdf_maskl(SP1_CLMPCTRL_0x0738 , ~_BIT7,(_BIT7));
        RTD_Log(LOGGER_DEBUG,"CLAMP_NEW_CLAMP_POSITION\n");

#else //#ifdef CLAMP_NEW_CLAMP_POSITION
        // CSW+ for setting YPbPr SD & HD source Clamp ctrl voltage level
        if (info->ucMode_Curr >= _MODE_720P50) // YPbPr HD source
        {
            rtdf_maskl(ADC_VBIAS_0x040c, ~0x00007000,0);// Set Clamp ctrl voltage level 100 => 400 mV

            if ((info->ucMode_Curr == _MODE_1080P50) ||
                    (info->ucMode_Curr == _MODE_1080P60))
                /*rtdf_outl(SP1_CLMP_0x073c, SP1_CLMP_s1_yclamp_sta(DISP_1080P_CLAMP_Y_START)
                	| SP1_CLMP_s1_yclamp_end(DISP_1080P_CLAMP_Y_END)
                	| SP1_CLMP_s1_nyclamp_sta(DISP_1080P_CLAMP_NY_START)
                	| SP1_CLMP_s1_nyclamp_end(DISP_1080P_CLAMP_NY_END));*/
                rtdf_outl(SP1_CLMP_0x073c, (0xFF000000&(DISP_1080P_CLAMP_Y_START<<24))
                          | (0x00FF0000 & (DISP_1080P_CLAMP_Y_END << 16))
                          | (0x0000FF00 & (DISP_1080P_CLAMP_NY_START << 8))
                          | (0x000000FF & DISP_1080P_CLAMP_NY_END));
            else
                /*rtdf_outl(SP1_CLMP_0x073c, SP1_CLMP_s1_yclamp_sta(DISP_HD_CLAMP_Y_START)
                	| SP1_CLMP_s1_yclamp_end(DISP_HD_CLAMP_Y_END)
                	| SP1_CLMP_s1_nyclamp_sta(DISP_HD_CLAMP_NY_START)
                	| SP1_CLMP_s1_nyclamp_end(DISP_HD_CLAMP_NY_END));*/
                rtdf_outl(SP1_CLMP_0x073c, (0xFF000000&(DISP_HD_CLAMP_Y_START<<24))
                          | (0x00FF0000 & (DISP_HD_CLAMP_Y_END << 16))
                          | (0x0000FF00 & (DISP_HD_CLAMP_NY_START << 8))
                          | (0x000000FF & DISP_HD_CLAMP_NY_END));

            rtdf_outl(SP1_CLMPCTRL_0x0738 , 0x0000001c);  //enable CLAMP signal output and select leading edge of HS for YPbPr(GBR), XTal_clk for SD source
            rtdf_outl(SP1_CLMPCTRL_0x0738 , 0x0000009c);  //enable load double buffer
        }
        else  // YPbPr SD source
        {
            rtdf_maskl(ADC_VBIAS_0x040c, ~0x00007000,0);// Set Clamp ctrl voltage level 100 => 400 mV

            /*rtdf_outl(SP1_CLMP_0x073c, SP1_CLMP_s1_yclamp_sta(DISP_SD_CLAMP_Y_START)
            	| SP1_CLMP_s1_yclamp_end(DISP_SD_CLAMP_Y_END)
            	| SP1_CLMP_s1_nyclamp_sta(DISP_SD_CLAMP_NY_START)
            	| SP1_CLMP_s1_nyclamp_end(DISP_SD_CLAMP_NY_END));*/
            rtdf_outl(SP1_CLMP_0x073c, (0xFF000000&(DISP_SD_CLAMP_Y_START<<24))
                      | (0x00FF0000 & (DISP_SD_CLAMP_Y_END << 16))
                      | (0x0000FF00 & (DISP_SD_CLAMP_NY_START << 8))
                      | (0x000000FF & DISP_SD_CLAMP_NY_END));

            rtdf_outl(SP1_CLMPCTRL_0x0738 , 0x0000003c);  //enable CLAMP signal output and select leading edge of HS for YPbPr(GBR) and select adc_clk for SD source
            rtdf_outl(SP1_CLMPCTRL_0x0738 , 0x000000bc);  //enable load double buffer
        }
        RTD_Log(LOGGER_DEBUG,"not CLAMP_NEW_CLAMP_POSITION\n");
#endif//#ifdef CLAMP_NEW_CLAMP_POSITION


    }

    // CSW+[Code Sync][CSW][0980512][1]

    drv_adc_Clamp_Ctrl(_ENABLE, GET_INPUTSOURCE_INDEX());

    drv_adc_DCRestore_Ctrl(_DISABLE, GET_INPUTSOURCE_INDEX());

#ifndef CONFIG_YPBPR_USE_AUTO_SOY
    // CSW+ 0960731 for setting YPbPr Clamp 2nd compare level

    RTD_Log(LOGGER_DEBUG,"DC restore compare level=%x\n",(UINT32)drv_auto_soy_GetSOYCompareLevel(drv_syncproc_Get_SOY_Path(GET_INPUTSOURCE_TYPE())));
    drv_auto_soy_SetSOYCompareLevel(drv_syncproc_Get_SOY_Path(GET_INPUTSOURCE_TYPE()), _SOY_MANUAL_COMPARE_LEVEL/*drv_auto_soy_GetSOYCompareLevel(drv_syncproc_Get_SOY_Path(GET_INPUTSOURCE_TYPE()))+2*/);
    //rtdf_outl(SOYCH0_A3_0x0b0c, (0x003F0000&((UINT32)(SOY_CH0_COMPARE_LEVEL)<<16)));// ADC/YPbPr , SOYCH0, compare level
#endif
    // CSW+[Code Sync][CSW][0980512][1] end

    // CSW+ 0961224 Set ABL after initial gain/offset
    //drv_abl_YPbPr_Setting_On();

}


/**
 * This function is used to progress the sync detection.
 *
 * @param sync_indicator: [input] sync processor 1 or sync processor2
 *
 * @return UINT8
 * @retval 0 : NO_SIGNAL
 * @retval 1 : NOT_SUPPORT
 * @retval 2 : YPBPR_SYNC
 * @retval 3 : SERRATION_COMPOSITE_SYNC
 * @retval 4 : XOR_OR_COMPOSITE_WITH_EQUALIZER
 * @retval 5 : XOR_OR_COMPOSITE_WITHOUT_EQUALIZER
 * @retval 6 : SEPARATE_SYNC
 * @retval 7 : HSYNC_ONLY
 *
 */
static UINT8 SyncProc_Detector(void)
{
    UINT8 ucCounter,ucResult;

    // CSW+ 0960713 to solve the error in VGA/PbPr plug in/out
    // Turn off sync processor auto-measure first
    rtdf_clearBits(SP1_CTRL_0x0700, _BIT10);//turn-off Automatic
    fw_timer_event_DelayXms(20);
    //in columbus, have no write clear
    //rtdf_outl(sync_indicator ? SYNCPROCESSOR_SP2_AUTORST_VADDR : SYNCPROCESSOR_SP1_AUTORST_VADDR, 0);//clear all
    rtdf_setBits(SP1_CTRL_0x0700, _BIT10);//set Automatic
    //fw_timer_event_DelayXms(30);//IC need 80ms to detect sync type

    //[Code Sync][AlanLi][0980403][1]
    //VGA support composite sync
    /*if(GET_INPUTSOURCE_TYPE() == _SOURCE_VGA){
    	if(VGADetecSyncTypeMode == SEPARATE_MODE)
    		//frank@0507 openauto run will turn on coast,turn-off coast will let VGA source phase change good
    		rtdf_clearBits(SP1_CTRL_0x0700, _BIT9);
    }*/
    //[Code Sync][AlanLi][0980403][1][end]


    ucCounter= 30; //500 ms from 100ms read protection,To prevent pattern gen for VGA timing tends from output wrong timing
    ucResult = 0;

    do
    {
        //if(GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
        //	fw_timer_event_DelayXms(100);	// To prevent pattern gen for VGA timing tends from output wrong timing
        //else
        fw_timer_event_DelayXms(10);

        ucResult = rtdf_readByte3(SP1_AUTORST_0x0704)& 0x80; //check whether auto run result is ready
    }
    while ((ucResult != 0x80) && (--ucCounter));

    if (ucCounter != 0)
    {
        ucResult = (rtdf_readByte0(SP1_AUTORST_0x0704) & (_BIT4| _BIT5 | _BIT6)) >>4; //get the measure result
    }
    else //measure timeout! treat as no signal
        ucResult = NO_SIGNAL;

    return ucResult;
}
#if 0 // not used
/**
 * This API is used to Adaptive compare level backward/forward search for Sync type
 *
 * @param [SyncProcessor] : which syncprocessor
 * @param [usCompareLevel] : compare level
 * @param [usAdaptiveStep] : the step of active
 * @param [usAdaptiveCnt] : the count of step
 * @param [result] : the pre result
 * @param [output] :
 * @return UINT8 :the sync type
 *
 */
static UINT8 Adaptive_SyncProc_Detector(UINT8 usCompareLevel, UINT8 usAdaptiveStep, UINT8 usAdaptiveCnt, UINT8 result)
{
    UINT8 usAdaptiveCompareLevel=usCompareLevel; // compare level
    UINT8 result_ret=result;

    if ((result > HSYNC_ONLY)||(result < YPBPR_SYNC))  // No Sync type found
    {
        // Backward compare level
        usAdaptiveCompareLevel = usCompareLevel - usAdaptiveStep*usAdaptiveCnt;
        //<Stanley 080312> Do not write to register directly, need to write to register by driver.
        if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
            drv_auto_soy_SetSOYCompareLevel(VGA_SOG_PATH_SELECT, usAdaptiveCompareLevel);
        else if (GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR)
            drv_auto_soy_SetSOYCompareLevel(YPBPR_SOY_PATH_SELECT, usAdaptiveCompareLevel);

        // Step 1. Set compare level first
        if (GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR) //<Stanley 080625> Only be execute by YPbPr
        {
            drv_auto_soy_SetSOYCompareLevel(YPBPR_SOY_PATH_SELECT, usAdaptiveCompareLevel);
            //<Stanley 080312>Modify by stanley, do not write to register directly, need to write to register by driver.
            /*
            if (YPBPR_SOY_PATH_SELECT)
            	IoReg_WriteByte0(IDOMAIN_SOYCH1_A0_VADDR , usAdaptiveCompareLevel); // ADC/YPbPr , SOYCH1, compare level
            else
            	rtdf_writeByte2(IDOMAIN_SOYCH0_A1_VADDR, usAdaptiveCompareLevel); // ADC/YPbPr , SOYCH0, compare level
            */
        }

        fw_timer_event_DelayXms(5);

        // Step 2. Run Sync processor to detect sync type
        result_ret = SyncProc_Detector();

        if ((result_ret > HSYNC_ONLY)||(result_ret < YPBPR_SYNC))  // No Sync type found
        {
            // Forward compare level
            usAdaptiveCompareLevel = usCompareLevel + usAdaptiveStep*usAdaptiveCnt;
            // Step 1. Set compare level first
            //<Stanley 080312>Modify by stanley, do not write to register directly, need to write to register by driver.
            if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
                drv_auto_soy_SetSOYCompareLevel(VGA_SOG_PATH_SELECT, usAdaptiveCompareLevel);
            else if (GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR)
                drv_auto_soy_SetSOYCompareLevel(YPBPR_SOY_PATH_SELECT, usAdaptiveCompareLevel);
            /*
            if (YPBPR_SOY_PATH_SELECT)
            	IoReg_WriteByte0(IDOMAIN_SOYCH1_A0_VADDR , usAdaptiveCompareLevel); // ADC/YPbPr , SOYCH1, compare level
            else
            	rtdf_writeByte2(IDOMAIN_SOYCH0_A1_VADDR, usAdaptiveCompareLevel); // ADC/YPbPr , SOYCH0, compare level
            */

            fw_timer_event_DelayXms(5);

            // Step 2. Run Sync processor to detect sync type
            result_ret = SyncProc_Detector();

            if ((result_ret <= HSYNC_ONLY)&&(result_ret >= YPBPR_SYNC))
                RTD_Log(LOGGER_DEBUG,"\n 3. Find Sync type in the compare level %x !!",(UINT32)usAdaptiveCompareLevel);
        }
        else
            RTD_Log(LOGGER_DEBUG,"\n 2. Find Sync type in the compare level %x !!",(UINT32)usAdaptiveCompareLevel);
    }
    else
        RTD_Log(LOGGER_DEBUG,"\n 1. Find Sync type in the compare level %x !!",(UINT32)usAdaptiveCompareLevel);
    return result_ret;
}
#endif

/**
 * This API is used to init setting of input source YPbPr
 *
 * @param [input] :
 * @param [output] :
 * @return None
 *
 */
void drv_syncproc_YPbPr_Init(void)
{
    drv_syncproc_SOYSourceSelect(YPBPR_SOY_PATH_SELECT); //select the soy source SOG1
    RTD_Log(LOGGER_DEBUG,"\nSP1_CTRL_0x0700= %x\n",(UINT32)rtdf_inl(SP1_CTRL_0x0700));
    RTD_Log(LOGGER_DEBUG,"\nYPbPr Initial...\n");
}

/**
 * This API is used to Adaptive compare level for Sync type search
 *
 * @param [ucTempSync] : sync type
 * @param [output] :
 * @return UINT8 :the compare sync type
 *
 */
#ifndef CONFIG_YPBPR_USE_AUTO_SOY
UINT8 SyncProc_Adaptive_SyncProc_Detector()
{
    UINT8 usCompareLevel_min=_SOY_MANUAL_COMPARE_LEVEL-(_ADAPTIVE_STEP*(_ADAPTIVE_TIMES>>1));//0x16; // 1st compare level = 16
    UINT8 usCompareLevel=0;
    static UINT8 usAdaptiveCount = 0; // loop counter
    UINT8 result_ret=0;

    if (usAdaptiveCount > _ADAPTIVE_TIMES)
        usAdaptiveCount = 0;

    usCompareLevel = usCompareLevel_min + usAdaptiveCount * _ADAPTIVE_STEP;

    RTD_Log(LOGGER_DEBUG,"\nusAdaptive usCompareLevel= %x\n",(UINT32)usCompareLevel);

    drv_auto_soy_SetSOYCompareLevel(drv_syncproc_Get_SOY_Path(GET_INPUTSOURCE_TYPE()), usCompareLevel);

    result_ret = SyncProc_Detector();

    usAdaptiveCount++;

    if ((result_ret <= HSYNC_ONLY)&&(result_ret >= YPBPR_SYNC))
    {
        drv_auto_soy_SetSOYCompareLevel(drv_syncproc_Get_SOY_Path(GET_INPUTSOURCE_TYPE()), usCompareLevel+_ADAPTIVE_STEP);
        result_ret = SyncProc_Detector();
        if ((result_ret <= HSYNC_ONLY)&&(result_ret >= YPBPR_SYNC))
        {
            //usAdaptiveCount = 0;
            RTD_Log(LOGGER_DEBUG,"\nfinal adaptiveCompareLevel= %x\n",(UINT32)rtdf_inl(SOYCH0_A3_0x0b0c));
            return result_ret;
        }
        else
        {
            drv_auto_soy_SetSOYCompareLevel(drv_syncproc_Get_SOY_Path(GET_INPUTSOURCE_TYPE()), usCompareLevel);
            RTD_Log(LOGGER_DEBUG,"\nfinal adaptiveCompareLevel_2= %x\n",(UINT32)rtdf_inl(SOYCH0_A3_0x0b0c));
            result_ret = SyncProc_Detector();
        }
    }
    //else
    //usAdaptiveCount++;

    return result_ret;
}
#endif
#if 0 // not used
static UINT8 SyncProc_Adaptive_Compare_Level(UINT8 ucTempSync)
{
    UINT8 usCompareLevel=0x16; // 1st compare level = 16
    UINT8 usAdaptiveStep=2; // Adaptive step
    UINT8 i=0;
    UINT8 usAdaptiveCount = 0; // loop counter
    UINT8 result_ret=ucTempSync;

    for (i=0; i<usAdaptiveCount; i++)
    {
        result_ret = Adaptive_SyncProc_Detector(usCompareLevel, usAdaptiveStep, i+1, result_ret);
        if ((result_ret < HSYNC_ONLY)&&(result_ret >= YPBPR_SYNC)) // Sync type found
            break;
    }

    return result_ret;
}

SyncProc_Set_Manual_Compare_Level(UINT8 value)
{
    rtdf_outl(SOYCH1_A0_0x0b5c, value);
}
#endif

/**
 * This API is used to know whether current sync status is a valid sync
 * Here we support the YPBPR_SYNC, SERRATION_COMPOSITE_SYNC and XOR_OR_COMPOSITE_WITHOUT_EQUALIZER
 *
 * @param [input] : None
 *
 * @return UINT8
 * @retval TRUE : sync is valid
 * @retval FALSE : sync is non valid
 */
static UINT8 SyncProc_SyncHandler(void)
{
    UINT8 ucTempSync;

#ifdef CONFIG_YPBPR_USE_AUTO_SOY
    ucTempSync = SyncProc_Detector();
#else
    ucTempSync = SyncProc_Adaptive_SyncProc_Detector();
#endif

    RTD_Log(LOGGER_DEBUG,"\n Detected signal sync type => %x\n",(UINT32)ucTempSync);

    //[Code Sync][AlanLi][0980403][1]
    //VGA support composite sync
    if (GET_INPUTSOURCE_TYPE()== _SOURCE_VGA)
    {
        if ((VGADetecSyncTypeMode == SEPARATE_MODE) && (ucTempSync != SEPARATE_SYNC))
        {
            RTD_Log(LOGGER_DEBUG,"\n BBBBBBB\n");
            return _MODE_NOSIGNAL;
        }
        else if ((VGADetecSyncTypeMode != SEPARATE_MODE) && (ucTempSync == SEPARATE_SYNC))
        {
            RTD_Log(LOGGER_DEBUG,"\n AAAAAAA\n");
           // return _MODE_NOSIGNAL;
            return _MODE_NOSUPPORT;//juney  test
        }
    }
    //[Code Sync][AlanLi][0980403][1][end]


    if (ucTempSync == YPBPR_SYNC ||
            ucTempSync == SERRATION_COMPOSITE_SYNC ||
            ucTempSync == XOR_OR_COMPOSITE_WITHOUT_EQUALIZER ||
            ucTempSync == XOR_OR_COMPOSITE_WITH_EQUALIZER ||
            ucTempSync == SEPARATE_SYNC) //||
        //ucTempSync == HSYNC_ONLY)
    {
        return SyncProc_SetVsyncCounterLevel(); //set the Vsync counter level to separate the Vsync
    }
    else  /* detect fail*/
    {
        if (ucTempSync == NO_SIGNAL)
            return _MODE_NOSIGNAL;
        else if (ucTempSync == NOT_SUPPORT)
            return _MODE_NOSUPPORT; //sync is detected, but not the YPbPr valid sync, treat as no support
        else
            return _MODE_NOCABLE;
    }

    /* detect success */
    return _MODE_SUCCESS;

}

void drv_syncproc_ResetICodeSetting(UINT8 icode)
{
    if (GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR)
        rtdf_outl(I_CODE_0x0528, 0x000000ff & icode);
}
#if 0 // not used
void drv_syncproc_ClampPositionAdjust(void)
{
    UINT32 usADCClock=0;
    UINT32 usHighPulsePeriod=0, usStablePeriod = 0;
    UINT16 usSmartFitIHStart=0;
    UINT8 usClampStart = 0, usClampEnd = 0, usClampWidth = 0, usChangeFlag = 0;


    // Get H sync high pulse period
    usStablePeriod = (rtdf_inl(SP1_STBRST1_0x0718)&0x07ff0000)>>16;
    usHighPulsePeriod = rtdf_inl(SP1_STBRST1_0x0718)&0x7ff;

    if (usStablePeriod < usHighPulsePeriod)
        usHighPulsePeriod = usStablePeriod;


    //RTD_Log(LOGGER_DEBUG,"usHighPulsePeriod Pre= %x in clamp \n",usHighPulsePeriod);

    // Calculate ADC clock
    //usADCClock =(UINT32) ((UINT64)CONFIG_CPU_XTAL_FREQ * disp_info[display].IHTotal/(UINT64)disp_info[display].IHCount_PRE/1000);
    usADCClock = CONFIG_CPU_XTAL_FREQ/1000;
    usADCClock =(UINT32) (usADCClock* disp_info.IHTotal/disp_info.IHCount_PRE);
    usADCClock = (usADCClock * 1000); // Hz

    //RTD_Log(LOGGER_DEBUG,"ADC Clock = %x in clamp \n",usADCClock);


    // Calculate usHighPulsePeriod with ADC clock sampling rate
    usHighPulsePeriod = usHighPulsePeriod * usADCClock / CONFIG_CPU_XTAL_FREQ; // Hz

    // YPbPr HD source with tri-state sync
    if (disp_info.ucMode_Curr >= _MODE_720P50)
        usHighPulsePeriod = usHighPulsePeriod * 2;


    //RTD_Log(LOGGER_DEBUG,"usHighPulsePeriod Aft= %x in clamp \n",usHighPulsePeriod);


    // Get YPbPr smart-fit IH start
    usSmartFitIHStart = drv_smartfit_ablgetsmartfitihStart();


    //RTD_Log(LOGGER_DEBUG,"usSmartFitIHStart = %x in clamp \n",usSmartFitIHStart);


    if (((usSmartFitIHStart>200) || (usSmartFitIHStart<usHighPulsePeriod)) && (disp_info.ucMode_Curr < _MODE_720P50)) //<Stanley K90904> Prevent smartfit value got wrong value at SD timing.
    {

        RTD_Log(LOGGER_DEBUG,"usSmartFitIHStart measure error!!!");

        return;
    }

#if (CLAMP_CNT_UNIT	== CLAMP_CNT_UNIT_2CLK)
    usHighPulsePeriod = usHighPulsePeriod>>1;
    usSmartFitIHStart = usSmartFitIHStart>>1;

    //RTD_Log(LOGGER_DEBUG,"In 2xClock, usHighPulsePeriod : %x\n", usHighPulsePeriod);
    //RTD_Log(LOGGER_DEBUG,"In 2xClock, usSmartFitIHStart : %x\n", usSmartFitIHStart);

#endif //#if (CLAMP_CNT_UNIT	== CLAMP_CNT_UNIT_2CLK)

    usClampStart = (rtdf_inl(SP1_CLMP_0x073c) & 0xff000000) >> 24;
    usClampEnd =   (rtdf_inl(SP1_CLMP_0x073c) & 0x00ff0000) >> 16;
    usClampWidth = usClampEnd - usClampStart;

    if (usClampEnd > (usSmartFitIHStart - CLAMP_POSITION_TOLERANCE))
    {
        //RTD_Log(LOGGER_DEBUG,"Condition 1, Before Adj, usClampStart : %x\n", usClampStart);
        //RTD_Log(LOGGER_DEBUG,"Condition 1, Before Adj, usClampEnd : %x\n", usClampEnd);
        //RTD_Log(LOGGER_DEBUG,"Condition 1, Before Adj, usClampWidth : %x\n", usClampWidth);

        usClampEnd = usSmartFitIHStart - ((usSmartFitIHStart - usHighPulsePeriod)>>2);
        usClampStart = usClampEnd - usClampWidth;

        //RTD_Log(LOGGER_DEBUG,"Condition 1, After Adj, usClampStart : %x\n", usClampStart);
        //RTD_Log(LOGGER_DEBUG,"Condition 1, After Adj, usClampEnd : %x\n", usClampEnd);
        //RTD_Log(LOGGER_DEBUG,"Condition 1, After Adj, usClampWidth : %x\n", usClampWidth);

        usChangeFlag = TRUE;
    }

    if (usClampStart < (usHighPulsePeriod + CLAMP_POSITION_TOLERANCE))
    {

        //RTD_Log(LOGGER_DEBUG,"Condition 2, Before Adj, usClampStart : %x\n", usClampStart);
        //RTD_Log(LOGGER_DEBUG,"Condition 2, Before Adj, usClampEnd : %x\n", usClampEnd);
        //RTD_Log(LOGGER_DEBUG,"Condition 2, Before Adj, usClampWidth : %x\n", usClampWidth);

        usClampStart = usHighPulsePeriod + CLAMP_POSITION_TOLERANCE;

        if ((usClampStart + usClampWidth) >= 0xff)
            usClampEnd = 0xff;
        else
            usClampEnd = usClampStart + usClampWidth;

        //RTD_Log(LOGGER_DEBUG,"Condition 2, After Adj, usClampStart : %x\n", usClampStart);
        //RTD_Log(LOGGER_DEBUG,"Condition 2, After Adj, usClampEnd : %x\n", usClampEnd);
        //RTD_Log(LOGGER_DEBUG,"Condition 2, After Adj, usClampWidth : %x\n", usClampWidth);

        usChangeFlag = TRUE;
    }

    if (usClampEnd > (usSmartFitIHStart - CLAMP_POSITION_TOLERANCE))
    {

        //RTD_Log(LOGGER_DEBUG,"Condition 3, Before Adj, usClampStart : %x\n", usClampStart);
        //RTD_Log(LOGGER_DEBUG,"Condition 3, Before Adj, usClampEnd : %x\n", usClampEnd);
        //RTD_Log(LOGGER_DEBUG,"Condition 3, Before Adj, usClampWidth : %x\n", usClampWidth);

        usClampEnd = usSmartFitIHStart - CLAMP_POSITION_TOLERANCE;

        //RTD_Log(LOGGER_DEBUG,"Condition 3, After Adj, usClampStart : %x\n", usClampStart);
        //RTD_Log(LOGGER_DEBUG,"Condition 3, After Adj, usClampEnd : %x\n", usClampEnd);
        //RTD_Log(LOGGER_DEBUG,"Condition 3, After Adj, usClampWidth : %x\n", usClampWidth);

        usChangeFlag = TRUE;
    }

    if (TRUE == usChangeFlag)
    {
        /*rtdf_outl(SP1_CLMP_reg, SP1_CLMP_s1_yclamp_sta(usClampStart)
        	| SP1_CLMP_s1_yclamp_end(usClampEnd)
        	| SP1_CLMP_s1_nyclamp_sta(usClampStart)
        	| SP1_CLMP_s1_nyclamp_end(usClampEnd));*/
        rtdf_maskl(SP1_CLMP_0x073c, ~(0xffffffff),((usClampStart << 24)|(usClampEnd << 16)|(usClampStart << 8)|(usClampEnd)));
    }

}
#endif

/**
 * This API is used to get the VGIP active window information and handle the mode setting
 *
 * @param [input] : None
 * @param [output] : None
 *
 *
 * @return : result
 * @retval  TRUE : detect a valid mode and progress the mode setting
 * @retval FALSE : no valid mode is detected
 */
UINT8 drv_syncproc_ModeHandler(StructSourceInput *srcinput_pt)
{
    UINT8 result = 0;
    UINT8 compare_value;
    ADC_INITIAL_OPTIONS  *ptOptions;

    ptOptions =  srcinput_pt->InitOptions;

    compare_value = 0;
#if 1
    switch (GET_INPUTSOURCE_TYPE())
    {
    case _SOURCE_YPBPR:
        result = flow_mode_detectmode(_OFFLINE_MS_SRC_DEHV1);
        //RTD_Log(LOGGER_DEBUG,"\n flow_mode_detectmode ypbpr result = %x",(UINT32)result);
        break;

    case _SOURCE_VGA:
        if (VGADetecSyncTypeMode == SEPARATE_MODE)
        {
            //RTD_Log(LOGGER_DEBUG,"\n sync type is separate");
            rtdf_outl(SP_ADC_OUT_SEL_0x076c, 0x00000000);
            result = flow_mode_detectmode(_OFFLINE_MS_SRC_RAWHV1);
        }
        else if (VGADetecSyncTypeMode==CS_ON_H_MODE)
        {
            //RTD_Log(LOGGER_DEBUG,"\n sync type is CS on H");
            rtdf_outl(SP_ADC_OUT_SEL_0x076c, 0x00000000);
            result = flow_mode_detectmode(_OFFLINE_MS_SRC_DEHV1);
        }
        else if (VGADetecSyncTypeMode==SOG_MODE)
        {
            //RTD_Log(LOGGER_DEBUG,"\n sync type is SOG");
            rtdf_outl(SP_ADC_OUT_SEL_0x076c, 0x00000000);
            result = flow_mode_detectmode(_OFFLINE_MS_SRC_DEHV1);
        }
        else
            return _MODE_NOSIGNAL;

        break;

    default:
        return _MDOE_UNDKOWN;
        break;
    }

#else
    if (info->input_src == _SOURCE_INDEX_VGA)
    {
#if (CONFIG_VGA_SUPPORT_SOG == 1)
        if (VGADetecSyncTypeMode == SEPARATE_MODE)
        {
            RTD_Log(LOGGER_DEBUG,"\n sync type is separate SUPPORT_SOG");
            rtdf_outl(SP_ADC_OUT_SEL_0x076c, 0x00000000);
            result = flow_mode_detectmode(_OFFLINE_MS_SRC_RAWHV1);
        }
        else if (VGADetecSyncTypeMode==CS_ON_H_MODE)
        {
            RTD_Log(LOGGER_DEBUG,"\n sync type is CS on H");
            rtdf_outl(SP_ADC_OUT_SEL_0x076c, 0x00000000);
            result = flow_mode_detectmode(_OFFLINE_MS_SRC_DEHV1);
        }
        else if (VGADetecSyncTypeMode==SOG_MODE)
        {
            RTD_Log(LOGGER_DEBUG,"\n sync type is SOG");
            rtdf_outl(SP_ADC_OUT_SEL_0x076c, 0x00000000);
            result = flow_mode_detectmode(_OFFLINE_MS_SRC_DEHV1);
        }
        else
            return _MODE_NOSIGNAL;
#else //#ifdef VGA_SUPPORT_SOG
    if (VGADetecSyncTypeMode == SEPARATE_MODE)
    {
        RTD_Log(LOGGER_DEBUG,"\n sync type is separate VGA");
        rtdf_outl(SP_ADC_OUT_SEL_0x076c, 0x00000000);
        result = flow_mode_detectmode(_OFFLINE_MS_SRC_RAWHV1);
    }
    else if (VGADetecSyncTypeMode==CS_ON_H_MODE)
    {
        RTD_Log(LOGGER_DEBUG,"\n sync type is CS on H");
        rtdf_outl(SP_ADC_OUT_SEL_0x076c, 0x00000000);
        result = flow_mode_detectmode(_OFFLINE_MS_SRC_DEHV1);
    }
    else
        return _MODE_NOSIGNAL;
#endif //#ifdef VGA_SUPPORT_SOG

    }
    else if (info->input_src == _SOURCE_INDEX_YPBPR)
    {

        result = flow_mode_detectmode(_OFFLINE_MS_SRC_DEHV1);
        RTD_Log(LOGGER_DEBUG,"\n flow_mode_detectmode ypbpr result = %x",(UINT32)result);

    }
    else
        return _MDOE_UNDKOWN;
#endif
    if ((result != _MODE_NOSUPPORT) && (result != _MODE_NOSIGNAL))
    {
        //rtdf_maskl(VGIP_CHN1_ACT_HSTA_WIDTH_0x0a18, 0xfffff000, info->IPH_ACT_WID);//H active width
        //rtdf_maskl(VGIP_CHN1_ACT_VSTA_LENGTH_0x0a1c, 0xfffff000, info->IPV_ACT_LEN);//V active len

        drv_pll_pfd_calibration();  // For the calculattion of PI code, (PFD used to calculate the delayed timming between Sync measured H-Sync and PLL H-Sync)

//		RTD_Log(LOGGER_DEBUG,"ihtotal=%d, ivtotal=%d, ivfreq=%d\n", info->IHTotal, info->IVTotal, info->IVFreq / 10);

        if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA) 	// weihao 960308
        {
            ui_get_VGA_info(GET_INPUT_MODE());
            COsdRecallVGAValue();
            rtdf_maskl(SP_ADCCLAMPSEL0_0x0764, 0x40000000,0x00015015);
            if (VGADetecSyncTypeMode==SOG_MODE)
            {
                rtdf_outl(SP1_CLMPMSK_0x0740, 0x00008202);
                drv_syncproc_SetClampPulsePos(0x10, 0x15, CLAMP_Y);
                drv_syncproc_SetClampPulsePos(0x10, 0x15, CLAMP_NY);
            }
            else
            {
                rtdf_outl(SP1_CLMPMSK_0x0740, 0x00000000);
                drv_syncproc_SetClampPulsePos(0x04, 0x10, CLAMP_Y);
                drv_syncproc_SetClampPulsePos(0x04, 0x10, CLAMP_NY);
            }
            drv_adc_Clamp_Ctrl(_ENABLE, GET_INPUTSOURCE_INDEX());
            drv_adc_DCRestore_Ctrl(_DISABLE, GET_INPUTSOURCE_INDEX());
            rtdf_writeByte1(ADC_CTL_0x0414, 0x50);//@ Pull up clamp voltage.

            drv_auto_soy_SetSOYCompareLevel(VGA_SOG_PATH_SELECT, _SOY_MANUAL_COMPARE_LEVEL);

            CLR_INTERLACE_IN_INPUT_MODE(); // Clear interlace flag in input mode
            drv_scaler_update_mode_para();
            //drv_adc_setadcgainoffset();

            // VGA ABL setting
            //drv_abl_YPbPr_Setting_On();

        }
        else if (GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR)
        {	// weihao 960308
            rtdf_maskl(SP_ADCCLAMPSEL0_0x0764, 0x40000000,0x00015015);
            drv_syncproc_YPbPr_ModeSetting(srcinput_pt);
#ifdef CONFIG_YPBPR_OPEN_H_V_POSITION
            drv_scaler_update_mode_para();
#endif
        }

        return _MODE_SUCCESS;
    }
    return result;
}

/**
 * This API is used to init sync processor
 *
 * @param [input] :
 * @param [output] :
 * @return None
 *
 */

void drv_syncproc_VGA_sync_input(void)
{
    UINT8 ucS1SrcSel;

    //[Code Sync][AlanLi][0980403][1]
    //VGA support composite sync
#ifdef CONFIG_YPBPR_USE_AUTO_SOY
    VGADetecSyncTypeMode = ((VGADetecSyncTypeMode+1)%(VGA_SYNC_DETECT_MODE));
#else
    if (drv_auto_soy_GetSOYCompareLevel(drv_syncproc_Get_SOY_Path(GET_INPUTSOURCE_TYPE())) == _SOY_MANUAL_COMPARE_LEVEL_MAX)
        VGADetecSyncTypeMode = ((VGADetecSyncTypeMode+1)%(VGA_SYNC_DETECT_MODE));
#endif
    //[Code Sync][AlanLi][0980403][1][end]

    //[Code Sync][AlanLi][0980403][1]
    //VGA support composite sync
    RTD_Log(LOGGER_DEBUG,"VGADetecSyncTypeMode = %d\n", (UINT32)VGADetecSyncTypeMode);
    switch (VGADetecSyncTypeMode)
    {
    case CS_ON_H_MODE:// CS on H type
        rtdf_maskl(SP1_STBCTRL_0x0710, ~(_BIT11|_BIT12|_BIT13|_BIT14|_BIT15), (_BIT14|_BIT15));  // +/- 24 crystal clks for capture window
        ucS1SrcSel = 2;// Force to CS on H type

        break;
    case SEPARATE_MODE:// Seperate type
    default:
        rtdf_maskl(SP1_STBCTRL_0x0710, ~(_BIT11|_BIT12|_BIT13|_BIT14|_BIT15), (_BIT12|_BIT13));  // +/- 6 crystal clks for capture window
        ucS1SrcSel = 2;// Force to seperate type

        break;

    case SOG_MODE:// SOG type
        ucS1SrcSel = 4;// Force to SOG type
        rtdf_maskl(SP1_STBCTRL_0x0710, ~(_BIT11|_BIT12|_BIT13|_BIT14|_BIT15), (_BIT14|_BIT15));  // +/- 24 crystal clks for capture window
        break;
    }
    //[Code Sync][AlanLi][0980403][1][end]

    rtdf_writeByte0(SP1_CTRL_0x0700, ucS1SrcSel);
}


void	drv_syncproc_InitSrc(UINT32 *init_table)
{
    if (init_table != NULL)
        drv_scaler_WriteTable(init_table);

    if (GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR)
    { 	// weihao 960308
        drv_syncproc_YPbPr_Init();  // change to sync processor setting
        RTD_Log(LOGGER_DEBUG,"YPbPr in SyncProc_InitSrc\n");
    }
    else if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
    { 	// weihao 960308
        rtdf_writeByte3(SP1_CTRL_0x0700, 0x80);//De-Composite HS out in CS mode
        rtdf_setBits(SP1_STBCTRL_0x0710, _BIT8/*(0x1<<8)*/);//Stable Period Tolerance +/-4
        RTD_Log(LOGGER_DEBUG,"VGA in SyncProc_InitSrc\n");
    }
}

/**
 * inprocvga_adcsetup
 * Startup settings for VGA
 * calculate pixelclock and set the ADC port
 * @param <none>
 * @return {none}
*/
static void inprocvga_adcsetup(void)
{
    UINT16 pixelclock;

    // Calculate pixel clock rate (round to MHz)
    pixelclock  = (((UINT32)info->IHFreq * (UINT32)info->IHTotal) * 2 / (1000 * 10));
    pixelclock  = (pixelclock >> 1) + (pixelclock & 0x01);

    // ADC differential mode and Set ADC bandwidth to reduce high frequency noise
    if (pixelclock < 38)
    {
        // 75 MHz  100
        rtdf_maskl(ADC_CTL_0x0414,  ~(_BIT26|_BIT25|_BIT24), (_BIT26));
    }
    else if (pixelclock < 68)
    {
        // 150 MHz  101
        rtdf_maskl(ADC_CTL_0x0414,  ~(_BIT26|_BIT25|_BIT24), (_BIT26|_BIT24));
    }
    else if (pixelclock < 160)
    {
        // 300 MHz  110
        rtdf_maskl(ADC_CTL_0x0414,  ~(_BIT26|_BIT25|_BIT24), (_BIT26|_BIT25));
    }
    else   // 500MHz  111
    {
        rtdf_maskl(ADC_CTL_0x0414,  ~(_BIT26|_BIT25|_BIT24), (_BIT26|_BIT25|_BIT24));
    }
    rtdf_writeByte2(ADC_CTL_0x0414, ((rtdf_readByte2(ADC_CTL_0x0414)& 0xf8) | (_ADC_FINE_TUNE_DELAY_RED & 0x07)));
    rtdf_writeByte1(ADC_CTL_0x0414, ((rtdf_readByte1(ADC_CTL_0x0414)& 0xf8) | (_ADC_FINE_TUNE_DELAY_GREEN & 0x07)));
    rtdf_writeByte0(ADC_CTL_0x0414, ((rtdf_readByte0(ADC_CTL_0x0414)& 0xf8) | (_ADC_FINE_TUNE_DELAY_BLUE & 0x07)));

    // HSYNC positive/negtive tracking
    // CSW+
    rtdf_clearBits(PLL_DIV_CTRL_0x0524, _BIT6);
}


UINT8 drv_syncproc_inprocypbpr_setup(void)
{
    drv_syncproc_Capture_Window_Tolerance();
    drv_measure_enableonlinemeasure();
    //rtdf_writeByte1(ADC_CTL_0x0414, 0x40);  // Green
    CLR_422_FORMAT();
    return SUCCESS;
}
#if 0
void drv_syncproc_inprocvga_init(void)
{
    rtdf_outl(ABL_CTRL_0x0c00, 0x00000000);
    rtdf_outl(ABL_WINDOW_0x0c04, 0x00000000);
}
#endif
UINT8 drv_syncproc_inprocvga_setup(void)
{
    drv_measure_enableonlinemeasure();
    inprocvga_adcsetup();
    CLR_422_FORMAT();
    return SUCCESS;

}

/*================= Sync Proc Callback Function ================= */
/**
 * This API is used to init sync processor
 *
 * @param [input] :
 * @param [output] :
 * @return None
 *
 */
#if 0 //def CONFIG_VGA_SUPPORT_SOG
void drv_syncproc_VGA_SOG_Init(void)
{
    //<Stanley 080312> For SOG signal type
    UINT8 ucS1SrcSel;
    rtdf_outl(ABL_CTRL_0x0c00, 0x00); //disable ABL

    ucS1SrcSel = (0x00000007&(rtdf_inl(SP1_CTRL_0x0700)));

    switch (ucS1SrcSel)
    {
    case SP_VHS_SOURCE_PAD_HS1VS1:// Seperate type
        ucS1SrcSel = SP_VHS_SOURCE_SOY_0;// Force to SOG type
        rtdf_outl(SP1_CLMP_0x073c, 0x10151015);
        rtdf_outl(SP1_CLMPMSK_0x0740, 0x00008202);

        rtdf_maskl(SP1_STBCTRL_0x0710, ~(0x0000F800), (0x0000F800&((0x06)<<11)));  // +/- 27 crystal clks for capture window

        rtdf_maskl(SP1_CTRL_0x0700, ~_BIT9, _BIT9);

        RTD_Log(LOGGER_DEBUG,"In VGA_SOG_SyncProc_Init, switch to SOG type!\n");
        break;

    case SP_VHS_SOURCE_SOY_0:// SOG type
    default:
        ucS1SrcSel = SP_VHS_SOURCE_PAD_HS1VS1;// Force to seperate type
        rtdf_outl(SP1_CLMP_0x073c, 0x04100410);

        //Lewis, 20081021, it have double buffer
        rtdf_outl(SP1_CLMPCTRL_0x0738, 0x00000010);  //enable CLAMP signal output and select leading edge of HS for VGA(SOG), XTal_clk
        rtdf_outl(SP1_CLMPCTRL_0x0738, 0x00000092);  //enable load double buffer

        rtdf_outl(SP1_CLMPMSK_0x0740, 0x00);
        rtdf_maskl(SP1_STBCTRL_0x0710, ~(0x0000F800), (0x0000F800&((0x01)<<11)));  // +/- 1 crystal clks for capture window

        //De-Composite HS out in CS mode, disable coast
        rtdf_maskl(SP1_CTRL_0x0700, ~(_BIT31 | _BIT9), _BIT31);

        RTD_Log(LOGGER_DEBUG,"In VGA_SOG_SyncProc_Init, switch to Separate Sync type!\n");
        break;
    }

    rtdf_maskl(SP1_CTRL_0x0700, ~(0x00000007), (0x00000007&(ucS1SrcSel)));
}
#endif// end of #ifdef CONFIG_VGA_SUPPORT_SOG


UINT8 drv_syncproc_CheckMode(void)
{
    if (!drv_measure_onlinemeasure())
    {
        return  FALSE;
    }
    return TRUE;
}

UINT8 drv_syncproc_DetectMode(StructSourceInput *srcinput_pt)
{
    UINT8 result=0, SignalMode;

    result = SyncProc_SyncHandler();

    //RTD_Log(LOGGER_DEBUG,"\nSyncHandler Detected = %x\n",(UINT32)result);
    if ( result == _MODE_SUCCESS)
    {

        //CSW+ 0980217 For reset main display window size
        //drv_scalerdisplay_set_main_display_window(0, _DISP_WID, 0, _DISP_LEN, 0);

        SignalMode = drv_syncproc_ModeHandler(srcinput_pt);
        RTD_Log(LOGGER_DEBUG,"\nSignalMode = %x\n", (UINT32)SignalMode);
        if (SignalMode == _MODE_SUCCESS)
        {
            return _MODE_SUCCESS;
        }
        else
        {
            if (SignalMode == _MODE_NOSUPPORT)
            {
                // CSW+ 0961002 add "No Support signal state" to show "No Support" OSD in VGA mode
                if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
                    result = _MODE_NOSUPPORT;
                else
                    result = _MODE_NOSIGNAL;
            }
            else
            {
                result = _MODE_NOSIGNAL;
            }
            drv_syncproc_FunctionDisable();
        }
    }

    return result;
}


/**
 * This API is used to adjust adc signal Hposition
 *
 * @param [display] : main or sub channel
 *
 * @return : none
 */
void drv_syncproc_ScalerSetHPosition(void)
{
    //forster modified 071002
    drv_syncproc_YPbPr_SetPhase(GET_PHASE(), TRUE); //set phase will call the drv_scaler_adjust_ihs_delay function //forster modified 071211
}

/**
 * This API is used to adjust adc signal Vposition
 *
 * @param [display] : main or sub channel
 *
 * @return : none
 */
void drv_syncproc_ScalerSetVPosition(void)
{
    UINT8 usIVSDelay=0;

    if (!GET_AUTO_CONFIG_DOING())
        drv_scaler_enable_fs_wdg(_FALSE);

    RTD_Log(LOGGER_DEBUG,"\n ucIVS2DVSDelay in ScalerSetVPosition= %x\n ucIPV_Max_Margin in ScalerSetVPosition = %x\n ucIPV_Min_Margin in ScalerSetVPosition = %x\n info->IPV_ACT_STA=%d\n GET_V_POSITION()=%x\n "
            ,(UINT32)ucIVS2DVSDelay,(UINT32)ucIPV_Max_Margin,(UINT32)ucIPV_Min_Margin,(UINT32)info->IPV_ACT_STA,(UINT32)GET_V_POSITION());


    //RTD_Log(LOGGER_DEBUG,"\n ucIPV_Max_Margin in ScalerSetVPosition = %x",(UINT32)ucIPV_Max_Margin);
    //RTD_Log(LOGGER_DEBUG,"\n ucIPV_Min_Margin in ScalerSetVPosition = %x",(UINT32)ucIPV_Min_Margin);
    //RTD_Log(LOGGER_DEBUG,"\n info->IPV_ACT_STA=%d\n", (UINT32)info->IPV_ACT_STA);
    if (ucIPV_Min_Margin != 0 && ucIPV_Max_Margin != 0)
    {
        if (GET_V_POSITION() < ucIPV_Min_Margin)
            SET_V_POSITION(ucIPV_Min_Margin);

        if (GET_V_POSITION()> ucIPV_Max_Margin)
            SET_V_POSITION(ucIPV_Max_Margin);
    }

    if (GET_INTERLACE_IN_INPUT_MODE())
    {
        if ((ucIVS2DVSDelay >> 1) >= ABS(0x80, GET_V_POSITION()))
            usIVSDelay = (ucIVS2DVSDelay >> 1) - (0x80 - GET_V_POSITION());
        else
            usIVSDelay = 0;

        if ((info->IPV_ACT_STA - (ucIVS2DVSDelay)) <4)
            drv_vgip_adjust_ivs_delay(usIVSDelay + (info->IPV_ACT_STA - (ucIVS2DVSDelay)) -4);
        else
            drv_vgip_adjust_ivs_delay(usIVSDelay);
    }
    else
    {
        if ( ucIVS2DVSDelay >= ABS(0x80, GET_V_POSITION()))
            usIVSDelay = ucIVS2DVSDelay - (0x80 - GET_V_POSITION());
        else
            usIVSDelay = 0;

        //ROSRTD_Log(LOGGER_DEBUG," actually ucIVS2DVSDelay=%d\n",ucIVS2DVSDelay);
        //ROSPrintf(" actually GET_V_POSITION()=%d\n",GET_V_POSITION(disp_info[display].input_src));
        if ((info->IPV_ACT_STA - (ucIVS2DVSDelay)) <4)
            drv_vgip_adjust_ivs_delay(usIVSDelay + (info->IPV_ACT_STA - (ucIVS2DVSDelay)) - 4);
        else
            drv_vgip_adjust_ivs_delay(usIVSDelay);


    }
    flow_framesync_Clear_StatusFlag();
    if ((!GET_AUTO_CONFIG_DOING())&&(info->state == _MODE_STATE_ACTIVE))
        drv_scaler_enable_fs_wdg( _TRUE);
}


void drv_syncproc_FunctionDisable(void)
{
    switch (GET_INPUTSOURCE_TYPE())
    {
    case _SOURCE_YPBPR:
        rtdf_clearBits(SP1_CTRL_0x0700, _BIT10);
#ifdef CONFIG_YPBPR_USE_AUTO_SOY
        //lewis 20080728, disable auto-soy, to prevent detect fail
        rtdf_clearBits(SOYCH0_A0_0x0b00,  _BIT0);
#endif //#ifdef CONFIG_YPBPR_USE_AUTO_SOY
        rtdf_outl(SP1_CLMPCTRL_0x0738, 0x00); //disable clamp signal output
        rtdf_clearBits(ADC_CLAMP_CTRL1_0x0420, 0x1FF); //disable clamp
        //drv_pll_power_AdcApllEnable(_OFF);
        break;

    case _SOURCE_VGA:
        rtdf_clearBits(SP1_CTRL_0x0700, _BIT10);
        rtdf_outl(SP1_CLMPCTRL_0x0738, 0x00); //disable clamp signal output
        rtdf_clearBits(ADC_CLAMP_CTRL1_0x0420, 0x1FF); //disable clamp
        //drv_pll_power_AdcApllEnable(_OFF);
        break;


    default:
        break;
    }

}

void drv_syncproc_FunctionPowerDown(void)
{
    //rtd_outl(0x0700, 0x80a30005);	// ADC HS source = HSFB , CS source = SOY0
    //rtd_outl(0x0710, 0x00011400);

    //ABL disable
    //rtdf_clearBits(ABL_CTRL_0x0c00, _BIT31);
    //AGC disable => LV remove AGC
    //rtdf_maskl(ABL_AGC_ABL_CTRL_VADDR, ~AGC_CTRL_agc_en_mask, AGC_CTRL_agc_en(0));
    //Auto SOY disable

    rtdf_clearBits(SOYCH0_A0_0x0b00, _BIT0);
    //Sync Processor disable
    rtdf_clearBits(SP1_CTRL_0x0700, _BIT10);
    //APLL power down
    rtdf_maskl(PLL_CTRL_0x0504, ~_BIT0, _BIT0);
    //ADC RGB channel power down 0x18020000[2:0]
    //SOY clock disable and power down 0x18020000[7:4]

    //rtdf_outl(ADC_POWER_0x0400, 0);

    //DC restore disable
    rtdf_outl(ADC_DCRESTORE_CTRL_0x0418, 0x00);

    //Clamp disable
    rtdf_outl(ADC_CLAMP_CTRL1_0x0420, 0x00);
}

void drv_syncproc_PLL27X_Enable(UINT8 enable)
{
    //SOY need enable this bit for auto soy block
    if (enable)
        rtdf_setBits(SYS_PLL_27X_0_0x2c64, _BIT6);
    else
        rtdf_clearBits(SYS_PLL_27X_0_0x2c64, _BIT6);
}

//--------------------------------------------------------------------------
//函数名称: bit drv_syncdetect_powersaving(void)
//功能描述: 当检测到高低电频都超过4次，则判断是hsync。
//输　  入: 无
//输　  出: 是否为hsync信号
//--------------------------------------------------------------------------

BOOL drv_hsyncdetect_powersaving(void)
{
	static UINT8 hsynchigh = 0;
	static UINT8 hsynclow = 0;
    if (GET_INPUTSOURCE_TYPE() != _SOURCE_VGA)
	return _FALSE;

	if(drv_pcb_get_gpio_pin(GPIO_C5))
		hsynchigh++;
	else
		hsynclow++;

	if(hsynchigh>50 || hsynclow>50)
	{
		hsynchigh = 0;
		hsynclow = 0;
	}

	if(hsynclow>4 && hsynchigh>4)
	{
		hsynchigh = 0;
		hsynclow = 0;
		return _TRUE;
	}

	return _FALSE;
}

