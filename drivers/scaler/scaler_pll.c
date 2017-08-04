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
#include "panel\panelapi.h"
#include "panel\panel_struct.h"
// #include "scaler_ddomain\scaler_display.h"
#include "autoconf.h"
#include "sparrow.h"
#include "reg_def.h"
#include "mode_customer.h"
#include "timer_event.h"
#include "source.h"
#include "rosprintf.h"
#include "flow_struct.h"
#include "scaler_idomain\adc.h"
#include "modestate.h"

//[Code Sync][AlanLi][0980617][1]
UINT8 select_adc_vco_div = 0;
//[Code Sync][AlanLi][0980617][1][end]

static UINT16 usPE_Value_rtd2622 = 120;
/*===================================  Types ================================*/
/*================================== Variables ==============================*/
/*================================ Definitions ==============================*/

/*================================== Function ===============================*/
/*============================================================================*/
/**
 * drvif_clk_power_AdcApllEnable
 * Power on/off ADC and APLL.
 *
 * @param <on_off>	{ power up or down for system }
 * @return 			{ void }
 *

void drv_pll_power_AdcApllEnable(UINT8 on_off)
{
	if(on_off) {
		rtdf_outl(ADC_POWER_0x0400, 0x1ff);// DCRestore Power On
	}else{
		rtdf_outl(ADC_POWER_0x0400, _BIT3);// DCRestore Power On
//		ROSPrintf("Entry Power ADC PLL off process\n");
	}
}
*/
static UINT8 get_apll_setting(UINT8 num, UINT16 usClock)
{
    UINT16 ucTemp;
    usClock -= 1;
    ///////////////////////////////////////

    ucTemp = rtdf_readWord0(PLLDIV_0x0508) & 0x0fff;

    RTD_Log(LOGGER_DEBUG,"\nTemp AClock = %x\n", num);

    if (ucTemp  != usClock)
    {
        RTD_Log(LOGGER_DEBUG,"\n Clock no match !n");
        return TRUE;
    }
    else
    {
        RTD_Log(LOGGER_DEBUG,"\n Clock match !\n");
        return FALSE;
    }
    ////////////////////////////////////////////////

}

//[Code Sync][AlanLi][0980617][1]
void drv_pll_select_VCO_div(void)
{
    UINT32 clock;

#if 0 //def CONFIG_VGA_JUMP_PHASE_SOLVED
    clock =(UINT32) ((UINT64)CONFIG_CPU_XTAL_FREQ * disp_info.IHTotal /(UINT64)stModeInfo.IHCount/1000);
#else
    clock = (UINT32)disp_info.IHFreq * disp_info.IHTotal / 10;
#endif
    select_adc_vco_div = clock < 100000 ? 4 : 2;
    RTD_Log(LOGGER_DEBUG,"\nselect_adc_vco_div = %d\n",(UINT32)select_adc_vco_div);

    //RTD_Log(LOGGER_DEBUG,"@@disp_info[display].IHTotal =%d\n", disp_info[display].IHTotal );
    //RTD_Log(LOGGER_DEBUG,"@@select_adc_clk_div=%d\n", select_adc_vco_div);
}
//[Code Sync][AlanLi][0980617][1][end]

/*============================================================================*/
/**
 * drvif_clock_set_dclk
 * Set display clock setup
 *
 * @param <void>		{ void }
 * @return 			{ void }
 *
 */
bit drv_pll_Set_DClk(UINT16 niHFreq, UINT16 niVActLen, UINT32 default_freq)
{
    UINT8  cpc,cnt=0,div=1, ret=0;
    UINT16 nMCode,nNCode;
    UINT32 nDPLL,temp=0;
    UINT32 nStep;
    UINT32 ulFreq;

    if (default_freq!=0)
    {
        ulFreq = default_freq;
    }
    else
    {

        ulFreq = ((Get_DISP_HORIZONTAL_TOTAL() * niHFreq*100 * 2) / niVActLen) * _DISP_LEN;
        ulFreq = ((ulFreq >> 1) + (ulFreq & 0x01));
    }

    // to avoid DISPLAY CLOCK over panel limit
    //printf("\n DClk = %d Hz  &  Max DClk = %d Hz\n",ulFreq,Get_DISPLAY_CLOCK_MAX() *1000000);

    //if(ulFreq>Get_DISPLAY_CLOCK_MAX() *1000000 )
    //{
    //	ulFreq=Get_DISPLAY_CLOCK_MAX() *1000000;
    //	ret = 1;
    //}

    //ulFreq>200MHz===>ulFreq * 16  too big(>32bit)
    //24.567MHz*(nMCode/nNCode)<=500 MHz
    //24.567MHz*(nMCode/nNCode)>=100 MHz
    //nMCode/nNCode<=20.3
    //nMCode/nNCode>=4
    //div>=25MHz*15/(ulFreq*8)
    //div<=125MHz*15/(ulFreq*8)
    //225MHz>=ulFreq>=28MHz
    //250MHz===>   1.04>=div>=0.208
    //28MHz===>   8.37>=div>=1.67

    if ((ulFreq<=234000000) && (ulFreq>=46800000))
        div=1;
    else if ((ulFreq<=117000000) && (ulFreq>=23400000))
        div=2;
    else if ((ulFreq<=58000000) && (ulFreq>=11700000))
        div=4;
    else if ((ulFreq<=29200000) && (ulFreq>=5850000))
        div=8;

    //nMCode/nNCode<=20;setting  nMCode=temp*nNCode
    //nMCode/nNCode>=4;setting  nMCode=temp*nNCode
    //temp=ulFreq/24.567MHz*div*2*16/15 <=20
    //temp=ulFreq/24.567MHz*div*2*16/15 >=4
    temp=(ulFreq)*div*2*4/15/(CONFIG_CPU_XTAL_FREQ/256);
    temp=temp*4;

    if (temp>=(20*256))
        temp=20*256;
    if (temp<=(4*256))
        temp=4*256;

    for (cnt=3; cnt<=15; cnt++)
    {
        nNCode=cnt;
        nMCode=nNCode*2*temp/256;
        nMCode = ((nMCode >> 1) + (nMCode & 0x01));
        if ((nMCode<=255)&&(nMCode>=3))
            break;
    }
    //WaitFor_DEN_STOP();
    cpc = drv_pll_cal_cpc(nMCode,nNCode);
    drv_pll_SetPLL(nMCode, nNCode, div, cpc);

    if (rtdf_readByte1(PLL_DISP_1_0x2f04)  & _BIT7)
    {
        rtdf_setBits(PLL_DISP_1_0x2f04, _BIT14);
        rtdf_clearBits(PLL_DISP_1_0x2f04, _BIT8);
    }

    // Dclk frequency in Hz
    //CONFIG_SCALER_XTAL_FREQ * nMCode too big(>32bit)
    nDPLL = ((CONFIG_CPU_XTAL_FREQ/128 * nMCode) / (div * nNCode*2))*128;

    // Offset resolution (DPLL / 2^15) in Hz
    nStep = nDPLL >> 15;

    // Target frequency of Dclk in Hz
    if (ulFreq >= (nDPLL*15/16))
    {
        temp = (ulFreq-(nDPLL*15/16) ) / nStep;
        temp = 0x800-temp;
    }
    else
    {
        temp = ((nDPLL*15/16)-ulFreq ) / nStep;
        temp = 0x800+temp;
    }

    //Enable DDS spread spectrum output function
    drv_pll_Set_DClk_offset(temp);

    return ret;
}


//--------------------------------------------------
// Description  : Adjust Dclk offset
// Input Value  : Dclk offset
// Output Value : None
//--------------------------------------------------
void drv_pll_Set_DClk_offset(UINT16 usDclkOffset)
{
//	UINT32 value,tmp;
    UINT32 tmp;
    tmp = usDclkOffset&0x0fff;
    //tmp = tmp <<16;
    //value = rtdf_inl(DCLKSS_0x2f08);
    //value &= 0xf000ffff;
    //value |= tmp;
    //value |= 0x05;
    rtdf_maskl(DCLKSS_0x2f08, 0xf000ffff, tmp <<16);
    //rtdf_outl(DCLKSS_0x2f08, value);
    rtdf_setBits(DCLKSS_0x2f08, _BIT0);
    rtdf_setBits(DCLKSS_0x2f08, _BIT2);
    RTD_Log(LOGGER_DEBUG,"\nDCLKSS_0x2f08 = %x\n",(UINT32)rtdf_inl(DCLKSS_0x2f08));

}

#if 0 //def CONFIG_VGA_JUMP_PHASE_SOLVED
#define _FRACTION_DIGIT           (16) //this value should not be larger than 7, or some timing will result in overflow in the calculation process
#else
#define _FRACTION_DIGIT           (7) //this value should not be larger than 7, or some timing will result in overflow in the calculation process
#endif


/*============================================================================*/
/**
 * drvif_clock_set_aclk
 * Set ADC clock setup
 *
 * @param <UINT16>	{ freq in KHz}
 * @return 			{ void }
 *
 */
bit drv_pll_Set_AClk(UINT32 usClock, UINT8 fast_lock_enable)
{
#if 0 //def CONFIG_VGA_JUMP_PHASE_SOLVED
    UINT64 pllclock, icode,SUM_I;
#else
    UINT32 pllclock, icode, SUM_I;
#endif
#ifdef ADJUST_APLL_NEW_METHOD
#if 0 //def CONFIG_VGA_JUMP_PHASE_SOLVED
    uint64_t pllclock1;
#else
    UINT32 pllclock1;
#endif
#endif //#ifdef ADJUST_APLL_NEW_METHO
    UINT16 mcode = 0, ncode = 0;
    UINT8 Data[4];
    INT8 kcode, g_value = 1;
    UINT16 pcode;
#ifdef APLL_NEW_MODE
    UINT16 pcode_max;
#endif //#ifdef APLL_NEW_MODE
    static UINT8 vco_divider = 2;
    UINT16 g_value_divider[6] = {4, 16, 64, 128, 256, 512};
    UINT32 	tmpclock;


    //if(!CAdjustCheckAdcClockRange(usClock, &delta))
    //    usClock = stModeInfo.IHTotal;
    //pllclock = (DWORD)stModeInfo.IHFreq * usClock * 2 * 100 / _RTD_XTAL;
    /**
    Fvco : Frequency of APLL
    Fxtal : Frequency of Crystal
    IHF : Input Horizontal Frequency
    usClock : Divider number of input clock
    stModeInfo.IHFreq = 10 * IHF(in KHz)
    _RTD_XTAl : Defined crystal clock unit in KHz

    Fvco = Fxtal*(M1+K/16)/N1 = IHF * usClock * vco_divider
    Assum N1 = 2
    (M1 + K/16) = IHF * usClock * N1 * vco_divider / Fxtal
    16*M1 + K = IHF * usClock * N1 * vco_divider * 16 / Fxtal = pllclock / 2^_FRACTION_DIGIT
    stModeInfo.IHFreq UINT in 100Hz

    */
    if (info->IHFreq==0)
        return FALSE;//Hardy20071018
    ncode = 3;

    //if(Scaler_DispGetInputInfoByDisp(display,SLR_INPUT_FRAMESYNC))
//[Code Sync][AlanLi][0980525][1]
//sync auto adjust flow
    if (!GET_AUTO_CONFIG_DOING())
        drv_scaler_enable_fs_wdg(_DISABLE);
//[Code Sync][AlanLi][0980525][1][end]

    //printf("Scaler_DispGetInputInfoByDisp(display,SLR_INPUT_H_COUNT)=%d\n",Scaler_DispGetInputInfoByDisp(display,SLR_INPUT_H_COUNT));
    //printf("usClock=%d\n",usClock);

    //ADC sampling clock, UNIT in KHz
#if 0 //def CONFIG_VGA_JUMP_PHASE_SOLVED
    tmpclock =(UINT32) ((UINT64)CONFIG_CPU_XTAL_FREQ * usClock/(UINT64)/*stModeInfo.IHCount*/info->IHCount_PRE/1000);
#else
    tmpclock = (UINT32)info->IHFreq* usClock / 10;
#endif

//[Code Sync][AlanLi][0980617][1]
    //vco_divider = tmp2636 < 100000 ? 4 : 2;
    vco_divider =select_adc_vco_div;
//[Code Sync][AlanLi][0980617][1][end]

    // CSW+ 0970131 To solve low temperature system hang
    if (tmpclock < 51000)
        rtdf_maskl(ADC_IBIAS0123_0x0404, ~(0xff000000), (0x7f000000));
    else
        rtdf_maskl(ADC_IBIAS0123_0x0404, ~(0xff000000), (0x5e000000));

    // Phase interpolation control load modified.	Marvin 0812
    if (tmpclock < 51000)
    {
        rtdf_maskl(PLL_PHASE_INTERPOLATION_0x0510, ~(_BIT9 | _BIT8), 0x00);
    }
    else
    {

        rtdf_maskl(PLL_PHASE_INTERPOLATION_0x0510, ~(_BIT9 | _BIT8), _BIT8);
    }

#if 0 //def CONFIG_VGA_JUMP_PHASE_SOLVED
    pllclock = (((UINT64)tmpclock * ncode * vco_divider) << (4 + _FRACTION_DIGIT)) / (UINT64)(CONFIG_CPU_XTAL_FREQ/1000); //calculate the more precise one
    SUM_I = ((((UINT64)tmpclock * ncode * vco_divider) << 4) / (UINT64)(CONFIG_CPU_XTAL_FREQ/1000)) << _FRACTION_DIGIT; //calculate the truncated one
#else
    pllclock = (((UINT32)tmpclock * ncode * vco_divider) << (4 + _FRACTION_DIGIT)) / (UINT32)(CONFIG_CPU_XTAL_FREQ/1000); //calculate the more precise one
    SUM_I = ((((UINT32)tmpclock * ncode * vco_divider) << 4) / (UINT32)(CONFIG_CPU_XTAL_FREQ/1000)) << _FRACTION_DIGIT; //calculate the truncated one
#endif

    RTD_Log(LOGGER_DEBUG,"\n tmpclock=%d\nAPLLclock 1 = %x\n",(UINT32)tmpclock, (UINT32)pllclock);//test by tyan
    //-	printf("\n SUM_I0 = %x", SUM_I);

    rtdf_maskl(PLL_DIV_CTRL_0x0524,~(0xffffffff),0x08);

    if (fast_lock_enable)
        rtdf_maskl(FAST_PLL_CTRL_0x053c,  ~(0x0000007F),0x20);// enable fast pll setting and
    //rtdf_maskl(PLL_DIV_CTRL_0x0524,~(0xffffffff),_BIT5); //enable fast lock


    rtdf_outl(PE_TRACKING_METHOD_0x0518, 0x00000002);
#if 0 //def APLL_NEW_MODE
    IoReg_Mask32(DDS_PLL_DDS_MIX_W1_VADDR, ~(0x7E000078), 0x0c000040);
#else //#ifdef APLL_NEW_MODE
    //rtdf_maskl(DDS_MIX_W1_0x051c, ~(0x7E000000), 0x0c000000);

    //set the P_code_max
    //rtdf_maskl(DDS_MIX_W1_0x051c, ~0x01FFFF00, 0x01FE0000);

    //need discuss, old mode do not need P code max
#endif //#ifdef APLL_NEW_MODE

    //因目前APLL 調整Icp的MSB 2個bit沒有做,所以無法達到Icp / M code = constant(0.5).目前designer建議monitor的因應之道是無論M,N code設多少,Icp均設為15uA.designer認為在
    //現在無法固定constant的情況之下設為此值的tracking jitter會是較小的.而目前TV這邊的Icp都是固定設成default的10uA , 所以可能要改為建議的15uA.
    //在Dragon APLL Icp的設定 : 0x1805_0100[20:16]改設定成"5".
    rtdf_maskl(PLL_SET_0x0500, ~0x0F1F0000, 0x03030000);//rtdf_maskl(PLL_SET_0x0500, ~0x001F0000, 0x00050000);

#if 0 //def CONFIG_VGA_JUMP_PHASE_SOLVED
    if (2 == vco_divider) //ADC CLK=1/2 VCO CLK
        rtdf_maskl(PLLPHASE_CTRL_0x050c, ~(0x00700000), 0x00400000);
    else //ADC CLK=1/4 VCO CLK
        rtdf_maskl(PLLPHASE_CTRL_0x050c, ~(0x00700000), 0x00500000);
#else	//#ifdef CONFIG_VGA_JUMP_PHASE_SOLVED
    //long path
    if (2 == vco_divider) //ADC CLK=1/2 VCO CLK
        rtdf_maskl(PLLPHASE_CTRL_0x050c, ~(0X700000), 0X600000);
    else //ADC CLK=1/4 VCO CLK
        rtdf_maskl(PLLPHASE_CTRL_0x050c, ~(0X700000), 0X700000);
#endif

    //RTD_Log(LOGGER_DEBUG,"IDOMAIN_PLLDIV_VADDR=%x\n",(UINT32)rtdf_inl(PLLDIV_0x0508));
#ifdef CONFIG_WATCHDOG_ENABLE
//	IoReg_Write32(PERI_WR_TR_VADDR, 1);//tick system WD, to avoid system reset
#endif

    //rtdf_maskl(PLLPHASE_CTRL_0x050c, ~(0x000F1F7F),0X0);

    rtdf_maskl(PLLDIV_0x0508, ~0x00000FFF, ((usClock - 1)&0x0FFF));

    // PLL N1
////////////////////////////////////////
//      get_apll_setting(0, usClock);
////////////////////////////////////////

    if (fast_lock_enable)  	//set the FAST PLL CONTROL
        rtdf_maskl(FAST_PLL_CTRL_0x053c,  ~(0x0000007F),0x22);// enable fast pll setting and
    //set ncode
    rtdf_maskl(PLL_SET_0x0500, ~0x00000700, ((ncode - 2) & 0x07)<<8);

#if 0 //def ADJUST_APLL_NEW_METHOD
//==========================================================
    printf("\n uOldMKRecord = %x",uOldMKRecord);
    printf("\n uOldPllClock = %x",uOldPllClock);
    printf("\n info->state = %x",info->state);

    //if(uOldMKRecord != 0)
    if ((uOldMKRecord != 0) && (info->state == _MODE_STATE_ACTIVE))
    {
        //         16 x (M+K/16)             Iclk x DIV x N/Xclk
        //   -----------------  =  -------------------------
        //         16 x (M'+K'/16)            Ilk' x DVI x N/Xclk
        //
        //         16M + K        Iclk
        //      --------- =  -----
        //         16M'+K'        Iclk'
        pllclock1 = (UINT64)uOldMKRecord *tmpclock / (UINT64)uOldPllClock;
        mcode = pllclock1 >> _FRACTION_DIGIT >> 4; //M1 is the integer part
        kcode = (pllclock1 >> _FRACTION_DIGIT) -(mcode << 4);

        SUM_I = pllclock1 -(((mcode<<4) + kcode) << _FRACTION_DIGIT);

        printf("\n mcode0 = %x", mcode);
        printf("\n kcode0 = %x", kcode);
    }
    else
    {
        // PLL M1, K1
        // 16*M1 + K1 = pllclock/1024
        /**
        mcode = pllclock / 1024 / 16; //M1 is the integer part
        kcode = pllclock/1024 - 16*M1 //K1 is the fraction part quantized by 16
        */
        mcode = pllclock >> _FRACTION_DIGIT >> 4; //M1 is the integer part
        kcode = (pllclock >> _FRACTION_DIGIT) -(mcode << 4);
        SUM_I = pllclock - SUM_I;  //SUM_I is the truncated part by calculation quantized by 1024

        printf("\n mcode = %x", mcode);
        printf("\n kcode = %x", kcode);

        //k is range from -8 ~ 7
        if (kcode>7)
        {
            mcode +=1;
            kcode -= 16;
        }
        else if (kcode<(-8))
        {
            mcode -=1;
            kcode += 16;
        }

        printf("\n mcode after = %x", mcode - 3);
        printf("\n kcode after = %x", kcode);
    }
#else //ADJUST_APLL_NEW_METHOD
    // PLL M1, K1
    // 16*M1 + K1 = pllclock/1024
    /**
    mcode = pllclock / 1024 / 16; //M1 is the integer part
    kcode = pllclock/1024 - 16*M1 //K1 is the fraction part quantized by 16
    */
    mcode = pllclock >> _FRACTION_DIGIT >> 4; //M1 is the integer part
    kcode = (pllclock >> _FRACTION_DIGIT) -((UINT32)mcode << 4);
    SUM_I = pllclock - SUM_I;  //SUM_I is the truncated part by calculation quantized by 1024


#endif
    //RTD_Log(LOGGER_DEBUG,"\n mcode = %x\n kcode = %x\n SUM_I : %x\n", (UINT32)mcode, (UINT32)kcode,(UINT32)SUM_I);

    //k is range from -8 ~ 7
    if (kcode>7)
    {
        mcode +=1;
        kcode -= 16;
    }
    else if (kcode<(-8))
    {
        mcode -=1;
        kcode += 16;
    }

    RTD_Log(LOGGER_DEBUG,"\n mcode after = %x \n kcode after = %x\n", (UINT32)mcode - 3, (UINT32)kcode);

    //set mcode
    //IoReg_WriteByte0(IDOMAIN_PLL_SET_VADDR, (mcode-3));

    //set kcode
    //forster modified 071030
    //rtdf_maskl(PLL_SET_0x0500, ~0x0000f0ff,((UINT32)(kcode & 0x0f)<<12) | ((mcode - 3) & 0xff));
    rtd_outl(PLL_SET_0x0500,(rtdf_inl(PLL_SET_0x0500)&0xffff0f00)|((UINT32)(kcode & 0x0f)<<12) | ((mcode - 3) & 0xff));

    if (fast_lock_enable)
    {
        //Because SUM_I has 25 bits
        //SUM_I = ((DWORD)2^22*(DWORD)SUM_I)/2^_FRACTION_DIGIT; // over flow need to modify
        //SUM_I = SUM_I << (22 - _FRACTION_DIGIT); // over flow need to modify
        SUM_I = (UINT32)SUM_I << (22 - _FRACTION_DIGIT); // over flow need to modify
        RTD_Log(LOGGER_DEBUG,"\n SUM_I1 = %x\n",(UINT32)SUM_I);
        rtdf_outl(FAST_PLL_SUM_I_0x0540, SUM_I);
    }

    //pllclock = pllclock >> 4;

    /**
    Formula :
          I_gain          Ths                              PE(UNIT)                                      1
    --------- x  ------- = ------------------------------------ x  -----
       2^22           Tbck               Txclk x 16N1/(16M1 +- K) x 1/16                8

       I_gain             Ths                                 PE(UNIT)                                      1
    --------- x  ----------- = ------------------------------------ x  -----
       2^22           Txclk x N1               Txclk x 16N1/(16M1 +- K) x 1/16                8

                      2^22 x PE_U x (16M1 +- K)             1
    I_gain = ----------------------------- x -----
                                       Ths                               8

           2^19 x PE_U x (16M1 +- K)
    = -----------------------------
                          Ths

    = IHF x 2^19 x PE_U x (16M1 +- K)

    */

    //16M1 +- K = pllclock/2^_FRACTION_DIGIT
    // _PE_VALUE UNIT is ps, so result has to multiply 10^(-12)
    // stModeInfo.IHFreq/10 UNIT is KHz, so result has to multiply 10^2
    //10^(-12)*10^2 = 10^(-10)
    //icode = ((DWORD)(tYPbPrMODE_TABLE[st263xInfo.Mode_Curr].IHFreq* usRTD2622PE_Value * pllclock) << (19 - _FRACTION_DIGIT)) /10^10;
    //         =((DWORD)(tYPbPrMODE_TABLE[st263xInfo.Mode_Curr].IHFreq* usRTD2622PE_Value * pllclock) ) /(10^10 >> 2^(19 - _FRACTION_DIGIT));
#if 0 //def CONFIG_VGA_JUMP_PHASE_SOLVED
    //icode = (UINT64)(Scaler_DispGetInputInfoByDisp(display,SLR_INPUT_H_FREQ)) * usPE_Value_rtd2622 * (pllclock >> 4)/(10000000000 >> (19 + 4 -_FRACTION_DIGIT));
    icode = (UINT64)(Scaler_DispGetInputInfoByDisp(display,SLR_INPUT_H_FREQ)) * usPE_Value_rtd2622 * (pllclock >> 4)/(10000000000 >> ((22 - _APLL_N_VALUE_FOR_I_CODE) + 4 -_FRACTION_DIGIT));
#else
    icode = (UINT32)(info->IHFreq) * usPE_Value_rtd2622 * (pllclock >> 4)/((1000000000 >> (19 + 4 -_FRACTION_DIGIT))*10);	//(10000000000 >> (19 + 4 -_FRACTION_DIGIT)) -->10000000000 >32bit
#endif
    //RTD_Log(LOGGER_DEBUG,"\n(info->IHFreq) = %d\nusPE_Value_rtd2622= %d\npllclock = %d\nicode=%d", (UINT32)info->IHFreq, (UINT32)usPE_Value_rtd2622, (UINT32)pllclock,(UINT32)icode);

    rtdf_outl(PE_TRACKING_METHOD_0x0518, 0x02);
    //Set the I_Code
    //icode &= 0x00007fff;

    rtdf_outl(I_CODE_0x0528, icode & 0x00007fff);

    // Set the P code
    //pcode = ((_APLL_N_VALUE_FOR_P_CODE -1) * ((((UINT32)icode)<<_APLL_N_VALUE_FOR_I_CODE)/_APLL_N_VALUE_FOR_P_CODE) * (CONFIG_CPU_XTAL_FREQ/1000) /info->IHFreq/ncode) >> 7;
    pcode = (31 * icode * (CONFIG_CPU_XTAL_FREQ/1000/info->IHFreq)/ncode) >> 7;		// p code (n=32)
    RTD_Log(LOGGER_DEBUG,"\nScaler_DispGetInputInfoByDisp(display,SLR_INPUT_H_FREQ) = %d\npllclock = %d\nicode1 = %d\npcode1 = %d\nncode1 = %d\n", (UINT32)info->IHFreq, (UINT32)pllclock, (UINT32)icode, (UINT32)pcode, (UINT32)ncode);

    if (pcode > 255)
    {
        for (Data[0] = 9; Data[0] < 14; Data[0]++)
        {
            if ((pcode >> Data[0]) == 0)
                break;
        }
//		ROSPrintf("g_value_divider=%x\n", g_value_divider[(Data[0] - 9)]);
        pcode = pcode/g_value_divider[(Data[0] - 9)];		// notice: damage
        g_value = Data[0] -7;
#if 1 //def _PRINT_APLL_RESULT
        RTD_Log(LOGGER_DEBUG,"\n g_value_divider[(Data[0] - 9)] : %x\n Data[0] : %x\n pcode : %x\n g_value : %x\n",
                (UINT32)g_value_divider[(Data[0] - 9)],(UINT32)Data[0],(UINT32)pcode,(UINT32)g_value);

#endif //#ifdef _PRINT_APLL_RESULT

    }

    rtdf_outl(P_CODE_MAPPING_METHODS_0x0514, 0xf0&((g_value)<<4));
    //set the P_code_max
    //rtdf_maskl(DDS_MIX_W1_0x051c, ~0x1ffff00, (UINT32)0x9a7<<8);

    rtdf_outl(P_CODE_0x052c, pcode);


    if (fast_lock_enable)      //Enable Double buffer write in PLL M/N K、PLLDIV、DDS SUM_I
        rtdf_outl(FAST_PLL_CTRL_0x053c, 0x60);
    else       //Enable Double buffer write in PLL M/N K、PLLDIV、DDS SUM_I
        rtdf_setBits(FAST_PLL_CTRL_0x053c, _BIT6);

    fw_timer_event_DelayXms(10);

    Data[0] = 16;
    do
    {
        rtdf_maskl(PLLDIV_0x0508, ~0xfff,((usClock - 1)&0x0fff));
    }
    while (get_apll_setting(1,usClock) && --Data[0]);
//	drv_pll_power_AdcApllEnable(_ON);

    rtdf_setBits(VGIP_CHN1_STATUS_0x0a14, _BIT12| _BIT13 | _BIT14);//clear status
//	IoReg_SetBits(SCALEDOWN_ICH1_IBUFF_STATUS_VADDR,  _BIT0|_BIT1|_BIT2);//clear status

    rtdf_setBits(MAIN_DISPLAY_CONTROL_RSV_0x2100,  _BIT18|_BIT19);//clear status

    rtdf_setBits(PLL_CTRL_0x0504, _BIT10| _BIT9 | _BIT3);	 // Enable APLL & dual clk

//[Code Sync][AlanLi][0980525][1]
//sync auto adjust flow
    if ((!GET_AUTO_CONFIG_DOING())&&(info->state == _MODE_STATE_ACTIVE))
        drv_scaler_enable_fs_wdg(_ENABLE);
//[Code Sync][AlanLi][0980525][1][end]
#if 0
    rtd_outl(0x0500, 0x03030103);	// M=6,N=3,K=0
    rtd_outl(0x0508, 0x00000359);	// Divider=858
    rtd_outl(0x050c, 0x00530000);	// ADC clk=(1/4)*VCO clk
    rtd_outl(0x0510, 0x00000020);	// PLL phase interpolation control load
    rtd_outl(0x0514, 0x00000010);	// G value=1, P code no over flow
    rtd_outl(0x0518, 0x00000002);	// Adaptive Tracking (Default value)
    rtd_outl(0x051c, 0x3009a733);	// P code max=316321 and new mode disable
    rtd_outl(0x0520, 0x000002a0);	// Delay chain select (Default value)
    rtd_outl(0x0524, 0x00000008);	// DDS tracking HS positive edge (Default value)
    rtd_outl(0x0528, 0x0000004f);	// I code=79
    rtd_outl(0x052c, 0x000000f7);	// P code=31632, no over flow
    rtd_outl(0x0530, 0x00000000);	// Calibration disable (Default value)
    rtd_outl(0x0534, 0x00000000);	// PE measure disable (Default value)
    rtd_outl(0x0538, 0x00000000);	// PE Max measure disable (Default value)

    rtd_outl(0x053c, 0x00000040);	// enable double buffer load
    rtd_outl(0x0504, 0x00000608);	// enable APLL & dual clk
    RTD_Log(LOGGER_DEBUG,"\nxxxxxxxxxxxxxx\n");
#endif
    return ((Data[0] == 0) ? _TRUE : _FALSE);
}

void drv_pll_pfd_calibration(void)
{
    UINT8 result=255;
    UINT16 usTemp;

    rtdf_outl(PFD_CALIBRATED_RESULTS_0x0530, 0); //clear
    rtdf_setBits(PFD_CALIBRATED_RESULTS_0x0530, _BIT15); //enable
    do
    {
        result--;
        if (((rtdf_readByte1(PFD_CALIBRATED_RESULTS_0x0530) & 0x80) == 0) || (result == 0))
            break;

        fw_timer_event_DelayXms(4);
    }
    while (result);

    if (result == 0)
    {
        usPE_Value_rtd2622 = 70;//timeout, set a default value,uint : ps
    }
    else
    {
        usTemp = rtdf_readWord0(PFD_CALIBRATED_RESULTS_0x0530) & 0x0fff;
        //usPE_Value_rtd2622 = 1000000000/(CONFIG_CPU_XTAL_FREQ/1000) / ((UINT16*)ucTemp)[0]; //uint :ps
        usPE_Value_rtd2622 = 1000000000/(CONFIG_CPU_XTAL_FREQ/1000) / usTemp; //uint :ps
    }
}

UINT8 drv_pll_cal_cpc(UINT8 M_Code,UINT8 N_Code) // CPC : charge pump current
{
    UINT32 ucTemp1;
    UINT32  ucTemp2 = 0;
    //Modify by tyan start 20070701  but not test yet
    if ( N_Code <= 5)
    {
        //LPF_Mode = 0, suppose  iM / Ich = 1.673
        ucTemp1 = ((UINT32) M_Code  *10000/ 1673);
        rtdf_clearBits(PLL_DISP_1_0x2f04, _BIT8);
    }
    else
    {
        //16>=N_Code>5
        //LPF_Mode = 1, suppose iM / Ich =17.6
        ucTemp1 = ( (UINT32)M_Code *10000 / 17600);
        rtdf_setBits(PLL_DISP_1_0x2f04, _BIT8);
    }

    //printf("\n Ich *10= %x",ucTemp1);

//DPLL_CRNT[4:0] = DPLLSI[4:0]
//I=(2.5uA+2.5uA*bit[0]+5uA*bit[1]+10uA*bit[2]+20uA*bit[3]+30uA*bit[4]) ,
//Ip=I*[(bit[5]+1) / 2]Keep Icp/DPM constant


    ucTemp1=ucTemp1-25;

    if (ucTemp1 > 300)
    {
        ucTemp1 -= 300;
        ucTemp2 += 16;
    }

    if (ucTemp1 > 200)
    {
        ucTemp1 -= 200;
        ucTemp2 += 8;
    }

    if (ucTemp1 > 100)
    {
        ucTemp1 -= 100;
        ucTemp2 |= 4;
    }

    if (ucTemp1 > 50)
    {
        ucTemp1 -= 50;
        ucTemp2 += 2;
    }

    if (ucTemp1 > 25)
    {
        ucTemp1 -= 25;
        ucTemp2 += 1;
    }

    ucTemp2 += 32;

    return ucTemp2;
}

void drv_pll_SetPLL(UINT8 M_Code, UINT8 N_Code, UINT8 div, UINT8 CPC)
{
    UINT32 tmp;
    //Modify by tyan
    if (div == 1)
        div = 0;
    else	if (div == 2)
        div = 1;
    else if (div == 4)
        div = 2;
    else if (div == 8)
        div = 3;


#ifdef CONFIG_DPLL_RESET_NEW
    rtdf_outl(PLL_DISP_0_0x2f00, ((M_Code - 2) << 20) | (div << 16) | ((N_Code - 2) << 8) |CPC);
    rtdf_setBits(PLL_DISP_1_0x2f04, _BIT15|_BIT10|_BIT9);
    if (N_Code>5)
        rtdf_setBits(PLL_DISP_1_0x2f04, _BIT8);
    else
        rtdf_clearBits(PLL_DISP_1_0x2f04, _BIT8);
    if (!(rtdf_inl(PLL_DISP_1_0x2f04)&_BIT15))
    {
        //Lewis, to prevent DPLL in reset mode
        rtdf_maskl(PLL_DISP_1_0x2f04, ~_BIT15, 0x0);
        fw_timer_event_DelayXms(1);
        rtdf_maskl(PLL_DISP_1_0x2f04, ~_BIT15, _BIT15);
    }
#else //#ifdef CONFIG_DPLL_RESET_NEW
    //frank mark ,tyan add but not test yetPLL_SYSTEM_PLL_DISP0_VADDR
    tmp = M_Code - 2;
    tmp = (tmp <<4) |div;
    tmp = (tmp << 8) | (N_Code - 2);
    tmp = (tmp << 8) | CPC;
    rtdf_outl(PLL_DISP_0_0x2f00, tmp);
    rtdf_setBits(PLL_DISP_1_0x2f04, (_BIT15 | _BIT10 | _BIT9));
    if (N_Code>5)
        rtdf_setBits(PLL_DISP_1_0x2f04, _BIT8);
    else
        rtdf_clearBits(PLL_DISP_1_0x2f04, _BIT8);
#endif //#ifdef CONFIG_DPLL_RESET_NEW
//CSW+ 0961214 new code for frame sync by tyan/cal. dclk for path1
    //if (Scaler_DispGetInputInfo(SLR_INPUT_FRAMESYNC))
    rtdf_maskl(PLL_DISP_1_0x2f04, ~(_BIT0 | _BIT1 | _BIT2 | _BIT3), _BIT0);
    //else
    //	rtdf_maskl(PLL_DISP_1_0x2f04, ~(_BIT0 | _BIT1 | _BIT2 | _BIT3), 0);

#ifdef CONFIG_SPREAD_SPECTRUM_ENABLE
    //if (Scaler_DispGetInputInfo(SLR_INPUT_FRAMESYNC))
    rtdf_maskl(DCLKSS_0x2f08, 0xff, _BIT2|_BIT0);
    //else
    //	rtdf_maskl(DCLKSS_0x2f08, 0xff, _BIT31 | _BIT2|_BIT0);
#else
    //if (Scaler_DispGetInputInfo(SLR_INPUT_FRAMESYNC))
    rtdf_maskl(DCLKSS_0x2f08, 0x00,  _BIT2);
    //else
    //	rtdf_maskl(DCLKSS_0x2f08, 0x00, _BIT31 | _BIT2);
#endif

}

#ifdef APLL_NEW_MODE
void drv_pll_set_aclk_newmode(UINT8 NewModeEnable)
{
    //in new mode, need enable I code to update sum I
    rtdf_maskl(DDS_MIX_W2_0x0520, ~_BIT11, 0x800&((NewModeEnable)<11));

    rtdf_maskl(DDS_MIX_W1_0x051c, ~_BIT3, 0x8&((NewModeEnable)<<3));
}
#endif //#ifdef APLL_NEW_MODE

/*======================== End of File =======================================*/
/**
*
* @}
*/

