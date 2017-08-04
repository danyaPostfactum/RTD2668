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
#include "scaler_idomain\abl.h"
#include "scaler_idomain\smartfit.h"
#include "sparrow.h"
#include "reg_def.h"
#include "mode_customer.h"
#include "timer_event.h"
#include "source.h"
#include "rosprintf.h"
#include "flow_struct.h"

#define _16_PIXEL	0x00
#define _32_PIXEL	0x01
#define _64_PIXEL	0x02
#define _04_PIXEL	0x03

#define _AVG_08_LINE		0x00
#define _AVG_16_LINE		0x01
#define _AVG_32_LINE		0x02
#define _AVG_64_LINE		0x03

#define _DELAY_016_LINE		0x00
#define _DELAY_032_LINE		0x01
#define _DELAY_064_LINE		0x02
#define _DELAY_128_LINE		0x03
#define _DELAY_192_LINE		0x04
#define _DELAY_256_LINE		0x05
#define _DELAY_384_LINE		0x06
#define _DELAY_640_LINE		0x07

#define VGA_SOG_ABL_V_START	0x00000008
#define VGA_SOG_ABL_V_END		0x000000e0
#define VGA_SOG_ABL_WAIT_LINE	_DELAY_016_LINE
#define VGA_SOG_ABL_H_WIDTH	_16_PIXEL
#define VGA_SOG_ABL_LINE_AVG	_AVG_08_LINE

/*
void drv_abl_VGA_Setting_On(UINT16 Hstart)
{
	UINT32 win=0;
	rtdf_outl(ADC_OFFSET0_0x042c, 0x02000200);
	rtdf_outl(ADC_OFFSET1_0x0430, 0x00000200);
	fw_timer_event_DelayXms(100);

	// CSW+ 0961224 Disable ABL first
	rtdf_outl(ABL_CTRL_0x0c00, 0x01800110);

	fw_timer_event_DelayXms(100);

	// Avoid to see moving process
	//display ? WaitFor_IVS2() : WaitFor_IVS1();
	//display ? WaitFor_IVS2() : WaitFor_IVS1();
	//display ? WaitFor_IVS2() : WaitFor_IVS1();
	win |= (VGA_SOG_ABL_V_END<<20);
	win |= ((VGA_SOG_ABL_LINE_AVG&0x0000000f) <<10);
	win |= (VGA_SOG_ABL_V_START<<12);
	win |= ((VGA_SOG_ABL_WAIT_LINE&0x0000000f)<<17);
	win |= ((VGA_SOG_ABL_H_WIDTH&0x0000000f)<<8);
	win |= Hstart	;

	rtdf_outl(ABL_WINDOW_0x0c04, win);

	fw_timer_event_DelayXms(100);

	rtdf_outl(ABL_CTRL_0x0c00, 0xf08000110);
}
*/

#if 0 // not used
// ABL window reference HS failing edge
void drv_abl_YPbPr_Setting_On(void)
{
    if (GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR)
    {
        // CSW+ 0970108 Reset offset to 0x200
        //IoReg_Write32(ADC_ADC_OFFSET0_VADDR, 0x02000200);
        //IoReg_Write32(ADC_ADC_OFFSET1_VADDR, 0x00000200);
        fw_timer_event_DelayXms(100);

        // CSW+ 0961224 Disable ABL first
        rtdf_outl(ABL_CTRL_0x0c00, 0x01800110);

        fw_timer_event_DelayXms(100);

        // Avoid to see moving process
        //display ? WaitFor_IVS2() : WaitFor_IVS1();
        //display ? WaitFor_IVS2() : WaitFor_IVS1();
        //display ? WaitFor_IVS2() : WaitFor_IVS1();

        if (info->ucMode_Curr >= _MODE_720P50) // YPbPr HD source
        {
            // CSW+ 0961224 Set ABL window
            if ((info->ucMode_Curr == _MODE_720P50) ||
                    (info->ucMode_Curr == _MODE_720P60))
                rtdf_outl(ABL_WINDOW_0x0c04, 0x0e008096);
            else
                rtdf_outl(ABL_WINDOW_0x0c04, 0x0e008088);
        }
        else  // YPbPr SD source
        {
            if (GET_INTERLACE_IN_INPUT_MODE())
            {
                if (info->ucMode_Curr == _MODE_480I) // 480i
                    rtdf_outl(ABL_WINDOW_0x0c04, 0x0e008052);
                else  // 576i
                    rtdf_outl(ABL_WINDOW_0x0c04, 0x0e008056);
            }
            else // 480p & 576p
            {
                if (info->ucMode_Curr == _MODE_480P) // 480p
                    rtdf_outl(ABL_WINDOW_0x0c04, 0x0e008052);
                else // 576p
                    rtdf_outl(ABL_WINDOW_0x0c04, 0x0e008056);
            }
        }

        fw_timer_event_DelayXms(100);

        //rtdf_outl(IDOMAIN_ABL_CTRL_VADDR, 0x81800110);
        rtdf_outl(ABL_CTRL_0x0c00, 0xf1800100);//eq margin = 0
        fw_timer_event_DelayXms(100);
        rtdf_outl(ABL_CTRL_0x0c00, 0xf1800110);//eq margin = 4
    }
    else if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
    {
        //rtdf_outl(IDOMAIN_ABL_CTRL_VADDR, 0x00810110);
        rtdf_outl(ABL_CTRL_0x0c00, 0x00800110);
        fw_timer_event_DelayXms(100);

        //rtdf_outl(IDOMAIN_ABL_WINDOW_VADDR, 0x0e008010);
        rtdf_outl(ABL_WINDOW_0x0c04, 0x0e008010);
        fw_timer_event_DelayXms(100);

        // CSW+ 0961106 Add APLL setting for YPbPr auto color
        //rtdf_outl(IDOMAIN_ABL_CTRL_VADDR, 0x80810110);
        rtdf_outl(ABL_CTRL_0x0c00, 0xf0800100);
        fw_timer_event_DelayXms(100);
        rtdf_outl(ABL_CTRL_0x0c00, 0xf0800110);
    }

}
#endif

#if 0 // not used
/*
void drv_abl_YPbPr_Setting_Off(void)
{
	// Disable ABL
	rtdf_clearBits(ABL_CTRL_0x0c00, _BIT28|_BIT29|_BIT30|_BIT31);

}
*/
void drv_abl_YPbPr_Setting_Finetune(void)
{
    UINT8  ABLWindowSafe, TestCnt=0;
    UINT32 usADCClock=0;
    UINT32 usHighPulsePeriod=0;
    UINT16 usSmartFitIHStart=0;
    UINT8 usABLWindowStart=0;
    UINT8 usABLWindowDefaultStart=0;
    UINT16 usABLWindowWidth, usABLWindowTolerance=0;
    UINT16 IHTotal, IHCount_Pre;
    if (GET_INPUTSOURCE_TYPE() != _SOURCE_YPBPR)
        return;
    RTD_Log(LOGGER_DEBUG, "drv_abl_YPbPr_Setting_Finetune \n");

    return;


    //display = Scaler_DispGetInputInfo(SLR_INPUT_DISPLAY);
    IHTotal = info->IHTotal;//Scaler_ModeGetModeInfo(SLR_MODE_IHLEN);
    IHCount_Pre = info->IHCount_PRE;

    // Get H sync high pulse period
    //if(SLR_MAIN_DISPLAY == display)
    usHighPulsePeriod = (0x000007FF&(rtdf_inl(SP1_STBRST1_0x0718)));
    //else //if(_SUB_DISPLAY == display)
    //	usHighPulsePeriod = (0x000007FF&(IoReg_Read32(SYNCPROCESSOR_SP2_STBRST1_VADDR)));

    //printf( "usHighPulsePeriod Pre= %x in ABL \n",usHighPulsePeriod);

    // Calculate ADC clock
    usADCClock = CRYSTAL_CLOCK/1000;
    usADCClock =(UINT32) (usADCClock * IHTotal/IHCount_Pre);
    usADCClock = (usADCClock * 1000); // Hz
    RTD_Log(LOGGER_DEBUG,"ADC Clock = %x in ABL \n",usADCClock);


    // Calculate usHighPulsePeriod with ADC clock sampling rate
    usHighPulsePeriod = usHighPulsePeriod * usADCClock / CRYSTAL_CLOCK; // Hz

    // YPbPr HD source with tri-state sync
    if (info->ucMode_Curr >= _MODE_720P50)
        usHighPulsePeriod = usHighPulsePeriod * 2;

    RTD_Log(LOGGER_DEBUG, "usHighPulsePeriod Aft= %x in ABL \n",usHighPulsePeriod);

    // Get YPbPr smart-fit IH start
    usSmartFitIHStart = drv_smartfit_ablgetsmartfitihStart();

    RTD_Log(LOGGER_DEBUG, "usSmartFitIHStart = %x in ABL \n",usSmartFitIHStart);

    if (((usSmartFitIHStart>200) || (usSmartFitIHStart<usHighPulsePeriod)) &&
            (info->ucMode_Curr < _MODE_720P50)) //<Stanley K90904> Prevent smartfit value got wrong value at SD timing.
    {
        RTD_Log(LOGGER_DEBUG, "usSmartFitIHStart measure error!!!");
        return;
    }

    // Get the theoretic ABL window start from original setting
    usABLWindowDefaultStart = (0x000000FF&(rtdf_inl(ABL_WINDOW_0x0c04)));

    // Get the theoretic ABL window start by smart-fit
    usABLWindowStart = usHighPulsePeriod + (usSmartFitIHStart - usHighPulsePeriod)/4;

    // Add protection for ABL window start
    if (usABLWindowDefaultStart > usABLWindowStart)
        rtdf_maskl(ABL_WINDOW_0x0c04, ~0x000000FF, (0x000000FF&(usABLWindowStart)));

    RTD_Log( LOGGER_DEBUG,"usABLWindow default start = %x in ABL \n",usABLWindowDefaultStart);
    RTD_Log( LOGGER_DEBUG,"usABLWindow start by smart-fit = %x in ABL \n",usABLWindowStart);

    usABLWindowTolerance = usSmartFitIHStart - usABLWindowStart;

    RTD_Log(LOGGER_DEBUG, "usABLWindowTolerance(usSmartFitIHStart - usABLWindowStart) = %x in ABL \n",usABLWindowTolerance);

    usABLWindowWidth = 16;

    if (((usSmartFitIHStart - usHighPulsePeriod) < 2) || // Back portch too small
            (usSmartFitIHStart > 0x0200)) // IH start too large
        return;

    // Shift ABL window left to expand the size of (usABLWindowTolerance - usABLWindowWidth)
    ABLWindowSafe = 0;
    TestCnt = 0;
    while ((!ABLWindowSafe) && (TestCnt < 5))
    {
        if ((usABLWindowTolerance - usABLWindowWidth) < 2)
        {
            usABLWindowStart = usHighPulsePeriod + (usSmartFitIHStart - usHighPulsePeriod)/(5+TestCnt);
            usABLWindowTolerance = usSmartFitIHStart - usABLWindowStart;

            TestCnt++;
        }
        else
            ABLWindowSafe = 1;
    }

    // If (usABLWindowTolerance - usABLWindowWidth) still < 2 after shifting ABL window then shrink usABLWindowWidth
    if ((TestCnt == 5) && (!ABLWindowSafe))
    {
        usABLWindowWidth = 4;
        if ((usABLWindowTolerance - usABLWindowWidth) < 2)
            return; // Backporch too small, ABL window can be finetuned
    }

    RTD_Log(LOGGER_DEBUG, "TestCnt = %x in ABL \n",TestCnt);
    RTD_Log(LOGGER_DEBUG, "usABLWindowWidth = %x in ABL \n",usABLWindowWidth);
    RTD_Log( LOGGER_DEBUG,"usABLWindowStart = %x in ABL \n",usABLWindowStart);


    if (usABLWindowWidth == 4)
        rtdf_maskl(ABL_WINDOW_0x0c04, ~0x00000300, (0x00000300&((_AVG_08_LINE)<<8)));
    else // usABLWindowWidth = 16
        rtdf_maskl(ABL_WINDOW_0x0c04, ~0x00000300, (0x00000300&((_AVG_16_LINE)<<8)));

    if (TestCnt > 0)
        rtdf_maskl(ABL_WINDOW_0x0c04, ~0x000000FF, (0x000000FF&(usABLWindowStart)));

}
#endif

void drvif_abl_setting_on(void)
{
    if ((GET_INPUTSOURCE_TYPE() != _SOURCE_YPBPR) && (GET_INPUTSOURCE_TYPE() != _SOURCE_VGA))
        return;

    rtdf_outl(ADC_OFFSET0_0x042c, 0x02000200);
    rtdf_outl(ADC_OFFSET1_0x0430, 0x00000200);
    fw_timer_event_DelayXms(100);
    rtdf_outl(ABL_CTRL_0x0c00, 0x01800110);
    fw_timer_event_DelayXms(100);

    if (GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR)
    {
        if (info->ucMode_Curr >= _MODE_720P50) // YPbPr HD source
        {
            // CSW+ 0961224 Set ABL window
            if ((info->ucMode_Curr == _MODE_720P50) ||
                    (info->ucMode_Curr == _MODE_720P60))
                rtdf_outl(ABL_WINDOW_0x0c04, 0x0e008096);
            else
                rtdf_outl(ABL_WINDOW_0x0c04, 0x0e008088);
        }
        else  // YPbPr SD source
        {
            if (GET_INTERLACE_IN_INPUT_MODE())
            {
                if (info->ucMode_Curr == _MODE_480I) // 480i
                    rtdf_outl(ABL_WINDOW_0x0c04, 0x0e008052);
                else  // 576i
                    rtdf_outl(ABL_WINDOW_0x0c04, 0x0e008056);
            }
            else // 480p & 576p
            {
                if (info->ucMode_Curr == _MODE_480P) // 480p
                    rtdf_outl(ABL_WINDOW_0x0c04, 0x0e008052);
                else // 576p
                    rtdf_outl(ABL_WINDOW_0x0c04, 0x0e008056);
            }
        }

        fw_timer_event_DelayXms(100);

        //rtdf_outl(IDOMAIN_ABL_CTRL_VADDR, 0x81800110);
        rtdf_outl(ABL_CTRL_0x0c00, 0xf1800100);//eq margin = 0
        fw_timer_event_DelayXms(100);
        rtdf_outl(ABL_CTRL_0x0c00, 0xf1800110);//eq margin = 4
    }
    else if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
    {
        UINT32 usADCClock=0;
        UINT32 usHighPulsePeriod=0;

        // Get H sync high pulse period
        usHighPulsePeriod = rtdf_readWord0(SP1_STBRST1_0x0718);

        RTD_Log(LOGGER_DEBUG,"usHighPulsePeriod Pre= %x in ABL \n",(UINT32)usHighPulsePeriod);

        // Calculate ADC clock
        usADCClock =(UINT32)((UINT32)CONFIG_CPU_XTAL_FREQ /stModeInfo.IHCount/1000* info->IHTotal);
        usADCClock = (usADCClock * 1000); // Hz
        RTD_Log(LOGGER_DEBUG,"usADCClock= %x in ABL \n",(UINT32)usADCClock);
        usADCClock = usADCClock <<4;
        // Calculate usHighPulsePeriod with ADC clock sampling rate

        usHighPulsePeriod = ((usADCClock / CONFIG_CPU_XTAL_FREQ))* usHighPulsePeriod ; // Hz

        usHighPulsePeriod = usHighPulsePeriod >>4;
        // YPbPr HD source with tri-state sync
        if (GET_INPUT_MODE() >= _MODE_720P50)
            usHighPulsePeriod = usHighPulsePeriod * 2;

        RTD_Log(LOGGER_DEBUG,"usHighPulsePeriod Aft= %x in ABL \n",(UINT32)usHighPulsePeriod);

        rtdf_outl(ABL_WINDOW_0x0c04, (0x0e008000|(usHighPulsePeriod+16)));

        rtdf_outl(ABL_CTRL_0x0c00, 0xf0800110);
    }
}

void drvif_abl_disable(void)
{
    rtdf_maskl(ABL_CTRL_0x0c00, 0x0fffffff, 0);
}

void drvif_abl_enable(void)
{
    rtdf_setBits(ABL_CTRL_0x0c00, _BIT31|_BIT30|_BIT29|_BIT28);
}




