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
#include "scaler_idomain\measure.h"
#include "scaler_idomain\syncproc.h"
#include "autoconf.h"
#include "source.h"
#include "sparrow.h"
#include "reg_def.h"
#include "mode.h"
#include "timer_event.h"
#include "rosprintf.h"
#include "flow_struct.h"
#include "isr_utility.h"
#include "scaler\scaler.h"
#include "scaler_ddomain\scaler_display.h"
#include "modestate.h"
#include "message.h"
#include "ui_struct.h"
#include "video\video.h"
#include "scaler_idomain\vgip.h"
#include "mode_customer.h"

#if 1
#define MEASURE_PRINTF	RTD_Log
#else
#define MEASURE_PRINTF //
#endif

static UINT16 HDMIDiditalMeasureIVH;
#if 0
void drv_measure_onlinemeasure_Interrupt_Init(void)
{
    drv_isr_Enable(SYS_ISR_ONLINE_MEASURE, _DISABLE);

    // Route select to 0 for online measure
    drv_isr_SelectRoute(ROUTE_SEL_ONLINE_MEASURE, IRQ_ROUTE_IRQ1);
}

bit drv_measure_onlinemeasure_Isr(void)
{
    UINT32 status;

    if (!drv_isr_GetPending(SYS_ISR_ONLINE_MEASURE))
        return RT_ISR_UNHANDLED;

    drv_isr_ClearPending(SYS_ISR_ONLINE_MEASURE);

    // if occurs VS period timeout, VS period overflow, VS period over range,
    // VS polarity change, HS period overflow, HS period over range, HS polarity change
    status = rtd_inl(SP_MS1STUS_0x090c);
    if (!(status & (_BIT7 | _BIT5 | _BIT4 | _BIT3 | _BIT2 | _BIT1 | _BIT0)))
        return RT_ISR_UNHANDLED;//not occurs mode change interrupt


    // Set display mode to free-run
    drv_scaler_set_display_mode(DISPLAY_MODE_FRC);

    drv_scalerdisplay_ForceBackground(_TRUE);

    drv_isr_Enable(SYS_ISR_ONLINE_MEASURE, _DISABLE);

    // send message
    fw_flow_message_push(_FL2FL_MAIN_STATE_PERIODIC_EVENT);

    return RT_ISR_HANDLED;
}
#endif

/**
 * This function is used for the on-line measurement
 *
 * @param measure_index: [input] Measure block 1 or measure block 2
 * @param source: [input] indicate the HDMI or YPbPr or Video
 * @param mode: [input] indicate the analog mode or digital mode
 *
 *
 * @return : None
 *
 * @Note :  On line measure is used to supervise the mode change, this function should be called once after mode setting
 *
 *
 */
UINT8 drv_measure_onlinemeasure(void)
{
    UINT16 IHCount = 0, IVCount = 0;
    UINT8 timeoutcnt = 6;   // Tracking timeout 60ms
    UINT32 ulResult;
    //MEASURE_PRINTF(LOGGER_DEBUG,"\nonline SP1_STBCTRL_0x0710 = %x\n",(UINT32)rtdf_inl(SP1_STBCTRL_0x0710));

    if ((GET_INPUTSOURCE_TYPE()  == _SOURCE_HDMI) ||(GET_INPUTSOURCE_FROM() == _SRC_FROM_ADC))
    {
        if (GET_INPUTSOURCE_TYPE()==_SOURCE_VGA)
            fw_timer_event_DelayXms(15);//delay for ADC source APLL stable (15)

        //pop up result
        timeoutcnt = 6;
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
            MEASURE_PRINTF(LOGGER_DEBUG,"Pop Up Timeout\n");
            return _FALSE;
        }

        ulResult = rtdf_inl(SP_MS1RST0_0x0904);
        IHCount = (UINT32)(ulResult &0x0000fff0)>>4;
        IVCount = (UINT32)(ulResult &0x0fff0000)>>16;

        switch (GET_INPUTSOURCE_TYPE())
        {
        #ifdef CONFIG_ENABLE_HDMI
        case _SOURCE_HDMI:
        #endif
        case _SOURCE_YPBPR:
            if ((ABS(IHCount, info->IHTotal)>10) || (ABS(IVCount, info->IVTotal)>10))
                goto error_result;
            break;

        case _SOURCE_CVBS:
        case _SOURCE_SV:
//			case _SOURCE_SCART_AV:	//jj 960505
//			case _SOURCE_SCART_SV:	//jj 960505
#ifdef CONFIG_SCART_AUTO_SWITCH
        case _SOURCE_FULL_SCART:
        case _SOURCE_HALF_SCART:
#endif
            if ((ABS(IHCount, info->IHTotal)>10))
                goto error_result;
            break;

        case _SOURCE_VGA:
            //MEASURE_PRINTF(LOGGER_DEBUG,"IHCount = %d, info->IHTotal = %d\n",(UINT32)IHCount, (UINT32)info->IHTotal);
            //MEASURE_PRINTF(LOGGER_DEBUG,"IVCount = %d, info->IVTotal = %d\n",(UINT32)IVCount, (UINT32)info->IVTotal);
            if ((ABS(IHCount, info->IHTotal)>30) || (ABS(IVCount, info->IVTotal)>30))
                goto error_result;
            break;

        default:
            break;
        }
        #ifdef CONFIG_ENABLE_HDMI
        if (GET_INPUTSOURCE_TYPE() == _SOURCE_HDMI )
        {
            rtdf_outl(SP_MS1STUS_0x090c,0x3ff);
            fw_timer_event_DelayXms(120);//fw_timer_event_DelayXms(30);
        }
        #endif
        //check status
        ulResult = rtdf_inl(SP_MS1STUS_0x090c);

        //if(((ulResult & 0xA4)!=0) && (GET_INPUTSOURCE_FROM() != _SRC_FROM_ADC)){ //timeout or polarity change treat as no signal
        //	MEASURE_PRINTF(LOGGER_DEBUG,"\nErrFlag = %x\n",(UINT32)(ulResult & 0xA4));
        //	goto error_result;
        //}
        //else if (GET_INPUTSOURCE_FROM() == _SRC_FROM_ADC) 	{
        if ((ulResult & 0xB6)!=0)  // timeout or polarity change treat as no signal
        {
            MEASURE_PRINTF(LOGGER_DEBUG,"\nErrFlag = %x\n",(UINT32)(ulResult & 0xB6));
            goto error_result;
        }
        //}
    }

    return _TRUE;

error_result:
    MEASURE_PRINTF(LOGGER_DEBUG,"IHCount = %d, info->IHTotal = %d\n",(UINT32)IHCount, (UINT32)info->IHTotal);
    MEASURE_PRINTF(LOGGER_DEBUG,"IVCount = %d, info->IVTotal = %d\n",(UINT32)IVCount, (UINT32)info->IVTotal);
    MEASURE_PRINTF(LOGGER_DEBUG,"Online Measure Err\n");

    drv_syncproc_FunctionDisable();
    rtdf_clearBits(SP_MS1CTRL_0x0900, (_BIT16|_BIT9 |_BIT10)); //disable online measure
//	rtdf_writeByte0(Scaler_DispGetInputInfo(SLR_INPUT_CHANNEL) ? SYNCPROCESSOR_SP_MS2STUS_VADDR : SYNCPROCESSOR_SP_MS1STUS_VADDR, 0xFF); //clear status, WD ForceBG will disappear
    return _FALSE;
}
#if 0 // not used
#include "mode_customer.h"
void debug_temp(void)
{
#if 0
    UINT16 usHighPulsePeriod=0;
    UINT8 mode_index,start_index,end_index,search_cnt;
    UINT8 count = 0;

    RTD_Log(LOGGER_DEBUG,"mode_searchmode YPBPR debug_temp\n");
    RTD_Log(LOGGER_DEBUG,"_MODE_480I=%x\n",(UINT32)_MODE_480I);
    start_index =  _MODE_480I;
    end_index =  _MODE_1080P30;

    for (search_cnt = start_index; search_cnt <= end_index; search_cnt++)
    {
        mode_index = mode_SearchModeMap(search_cnt);
        /*do {
        	RTD_Log(LOGGER_DEBUG,"modeno = %d\n",(UINT32)search_cnt);
        	RTD_Log(LOGGER_DEBUG,"tINPUTMODE_PRESET_TABLE[count].ModeCurr = %d\n",(UINT32)tINPUTMODE_PRESET_TABLE[count].ModeCurr);
        	if(GET_MODE_INPUT_MODE(tINPUTMODE_PRESET_TABLE[count].ModeCurr) == search_cnt)
        		mode_index = count;
        	count++;
        } while(count < stModeTableTypeTable.num);*/
        RTD_Log(LOGGER_DEBUG,"\search_cnt = %x\n",(UINT32)search_cnt);
        RTD_Log(LOGGER_DEBUG,"tINPUTMODE_PRESET_TABLE[count].ModeCurr = %x\n",(UINT32)tINPUTMODE_PRESET_TABLE[search_cnt].ModeCurr);
        RTD_Log(LOGGER_DEBUG,"stModeInfo.IHFreq = %d\n",(UINT32)stModeInfo.IHFreq);
        RTD_Log(LOGGER_DEBUG,"stModeInfo.IVFreq = %d\n",(UINT32)stModeInfo.IVFreq);
        RTD_Log(LOGGER_DEBUG,"tINPUTMODE_PRESET_TABLE[mode_index].IHFreq = %d\n",(UINT32)tINPUTMODE_PRESET_TABLE[search_cnt].IHFreq);
        RTD_Log(LOGGER_DEBUG,"tINPUTMODE_PRESET_TABLE[mode_index].IVFreq) = %d\n",(UINT32)tINPUTMODE_PRESET_TABLE[search_cnt].IVFreq);
        RTD_Log(LOGGER_DEBUG,"\nmode_index = %x\n",(UINT32)mode_index);

    }
#else

    if (info->input_src == 6)
    {
        RTD_Log(LOGGER_DEBUG,"\n debug_temp = %x",(UINT32)info->input_src);
    }
    else if (info->input_src == 0)
    {
        RTD_Log(LOGGER_DEBUG,"\n debug_temp");
    }

    RTD_Log(LOGGER_DEBUG,"\nttttttttt");

#endif
}
#endif

/**
 * This function is used to start off-line measure.
 *
 * @param [input] : None
 * @param st263xInfo->Hsync: [output] This para. record the measure result of Hsync period
 * @param st263xInfo->Vsync: [output] This para. record the measure result of Vsync lines
 * @param st263xInfo->YPbPrSyncType [output] This para. indicate that when mode change or nosignal occur, sync type will set to NO_SIGNAL
 *
 *
 * @return : None
 * @Note : This function should be called only after progress the YPbPr_OffLineMeasure and get the return TRUE.
 *
 */
UINT8 drv_measure_offlinemeasure(UINT8 source, UINT8 mode, ModeInformationType* stModeInfo)
{
    UINT8 timeoutcnt = 6;   // Tracking timeout 60ms
    UINT32 ulResult;

    MEASURE_PRINTF(LOGGER_DEBUG,"\nGet source=%x\n", (UINT32)source);
    MEASURE_PRINTF(LOGGER_DEBUG,"mode = %x in off-line measure\n", (UINT32)mode);

    if (mode == ANALOG_MODE_MEASUREMENT)
    {
        stModeInfo->IHCount = 0;
        stModeInfo->IVCount = 0;
    }
    ulResult = (source & 0x0f) ;
    ulResult = ulResult <<16;
    rtdf_maskl(SP_MS3CTRL_0x0800 , 0xfff0ffff,ulResult);  //  set measure source

    rtdf_maskl(SP_MS3CTRL_0x0800 , 0xfff0ffff,(((UINT32)source & 0x0f) << 16));  //  set measure source

    //clear offline measure error status
    rtdf_outl(SP_MS3STUS_0x080c, 0xff);

    //set measure mode & start measure
    rtdf_maskl(SP_MS3CTRL_0x0800, ~(_BIT10|_BIT13), _BIT9 |(mode? _BIT13: 0));

    //MEASURE_PRINTF(LOGGER_DEBUG,"SP_MS3CTRL_0x0800 setting = %x \n", (UINT32)rtdf_inl(SP_MS3CTRL_0x0800));

    do
    {
        fw_timer_event_DelayXms(20);
        ulResult = rtdf_inl(SP_MS3CTRL_0x0800);
        //MEASURE_PRINTF(LOGGER_DEBUG,"SP_MS3CTRL_0x0800_ulResult = %x\n", (UINT32)ulResult);
        if (!(ulResult & _BIT9)) // polling start bit
        {
            if ((rtdf_inl(SP_MS3STUS_0x080c)&_BIT7)>0) //have error status,restart measure
            {
                rtdf_outl(SP_MS3STUS_0x080c, 0xff);
                rtdf_maskl(SP_MS3CTRL_0x0800, ~(_BIT10|_BIT13), _BIT9 |(mode? _BIT13: 0));
                continue;
            }
            else
            {
                break;  // off-line measure is ready
            }
        }
    }
    while (--timeoutcnt);

    if (timeoutcnt == 0x00)
    {
        MEASURE_PRINTF(LOGGER_DEBUG,"drvif_mode_offlinemeasure ERROR1\n");
        return _MODE_NOSIGNAL;
    }
    ulResult = rtdf_inl(SP_MS3STUS_0x080c);
    //MEASURE_PRINTF(LOGGER_DEBUG,"SP_MS3STUS_0x080c_ulResult = %x\n", (UINT32)ulResult);

    //pop up result
    timeoutcnt = 6;

    // pop-up period measure result
    rtdf_setBits(SP_MS3CTRL_0x0800, _BIT12);  // pop-up period measure result

    do
    {
        fw_timer_event_DelayXms(15);
        ulResult = rtdf_inl(SP_MS3CTRL_0x0800);
        if (!(ulResult & _BIT12)) // polling status bit 12
            break;
    }
    while (--timeoutcnt);

    if (timeoutcnt == 0x00)
    {
        MEASURE_PRINTF(LOGGER_DEBUG,"OFFMS POP Err\n");
        goto error_result;
    }

    // get measure result
    if (mode == ANALOG_MODE_MEASUREMENT) 	//ANALOG MODE
    {
        MEASURE_PRINTF(LOGGER_DEBUG,"\nANALOG_MODE_MEASUREMENT\n");
        stModeInfo->IHCount = (rtdf_readWord0(SP_MS3RST0_0x0804)&0xFFF0) >> 4;
        stModeInfo->IVCount = rtdf_readWord1(SP_MS3RST0_0x0804)&0x0FFF;
        stModeInfo->IHSyncPulseCount = rtdf_readWord0(SP_MS3RST1_0x0808)& 0x0FFF;
        stModeInfo->IVTotal = stModeInfo->IVCount;

        if ((stModeInfo->IHCount == 0) || (stModeInfo->IVCount == 0))
        {
            MEASURE_PRINTF(LOGGER_DEBUG,"\nstModeInfo->IVCount == %x) || (stModeInfo->IVCount == %x)\n",(UINT32)stModeInfo->IHCount,(UINT32)stModeInfo->IVCount);
            goto error_result;
        }
        MEASURE_PRINTF(LOGGER_DEBUG,"CONFIG_CPU_XTAL_FREQ= %d\n",(UINT32)CONFIG_CPU_XTAL_FREQ);
        MEASURE_PRINTF(LOGGER_DEBUG,"stModeInfo->IHSyncPulseCount= %d\n",(UINT32)stModeInfo->IHSyncPulseCount);
        stModeInfo->IHFreq = (UINT16) (((UINT32)CONFIG_CPU_XTAL_FREQ / (UINT32)stModeInfo->IHCount) / 100); //calcluate H Frequency in 100Hz unit.
        stModeInfo->IVFreq = (UINT16) ((UINT32)(CONFIG_CPU_XTAL_FREQ * 10) / ((UINT32) stModeInfo->IHCount * stModeInfo->IVCount));//calcluate V Frequency in 1/10Hz unit.

        MEASURE_PRINTF(LOGGER_DEBUG,"OffLineMeasure, IHCount : %d, IVCount : %d\n", (UINT32)stModeInfo->IHCount, (UINT32)stModeInfo->IVCount);
        MEASURE_PRINTF(LOGGER_DEBUG,"OffLineMeasure, IHFreq : %d, IVFreq : %d\n", (UINT32)stModeInfo->IHFreq, (UINT32)stModeInfo->IVFreq);
    }
    else  		//DIGITAL MODE
    {
//		UINT16 ihTotal = (rtdf_readWord0(SP_MS3RST0_0x0804)&0xFFF0) >> 4;
//		UINT16 ivHeight = rtdf_readWord1(SP_MS3RST0_0x0804)&0x0FFF;
//		UINT16 ihWidth = rtdf_readWord0(SP_MS3RST1_0x0808)&0x0FFF;

//		stModeInfo->IHTotal = ihTotal;
//		stModeInfo->IVHeight= ivHeight;
//		stModeInfo->IHWidth = ihWidth;
        stModeInfo->IHTotal = (rtdf_readWord0(SP_MS3RST0_0x0804)&0xFFF0) >> 4;
        stModeInfo->IVHeight = rtdf_readWord1(SP_MS3RST0_0x0804)&0x0FFF;;
        stModeInfo->IHWidth = rtdf_readWord0(SP_MS3RST1_0x0808)&0x0FFF;;

//		if((ihWidth == 0) || (ivHeight == 0)){//1221 kist
        if ((stModeInfo->IHWidth == 0) || (stModeInfo->IVHeight == 0)) //1221 kist
        {
//			MEASURE_PRINTF(LOGGER_DEBUG,"\n(stModeInfo->IHWidth == %x) || (stModeInfo->IVHeight == %x)\n",ihWidth,ivHeight);
            //MEASURE_PRINTF(LOGGER_DEBUG,"\n(stModeInfo->IHWidth == %x) || (stModeInfo->IVHeight == %x)\n",stModeInfo->IHWidth,stModeInfo->IVHeight);
            goto error_result;
        }

//		ihWidth++;//match resolution(cause measure result will lose one line)
        stModeInfo->IHWidth++; //match resolution(cause measure result will lose one line)
//		ivHeight++;//match resolution(cause measure result will lose one line)
        stModeInfo->IVHeight++; //match resolution(cause measure result will lose one line)
//		stModeInfo->IHWidth = ihWidth;
//		stModeInfo->IVHeight= ivHeight;

//		if(ihWidth%2){
        if (stModeInfo->IHWidth % 2)
        {
            MEASURE_PRINTF(LOGGER_DEBUG,"IN_ERROR_IHWidth:%d\n",stModeInfo->IHWidth);
//			ihWidth++;
            stModeInfo->IHWidth++;
//			stModeInfo->IHWidth = ihWidth;
        }

//		HDMIDiditalMeasureIVH = ivHeight;
        HDMIDiditalMeasureIVH = stModeInfo->IVHeight;
        MEASURE_PRINTF(LOGGER_DEBUG,"\nIHTotal:%d\n",stModeInfo->IHTotal );
        MEASURE_PRINTF(LOGGER_DEBUG,"IVHeight:%d\n",stModeInfo->IVHeight);
        MEASURE_PRINTF(LOGGER_DEBUG,"IHWidth:%d\n",stModeInfo->IHWidth);
    }


    if (GET_INPUTSOURCE_FROM() == _SRC_FROM_TMDS)
    {
        if (rtdf_inl(SP_MS3STUS_0x080c) & _BIT7)
        {
            MEASURE_PRINTF(LOGGER_DEBUG,"Error Flag:%x\n", (UINT32)rtdf_inl(SP_MS3STUS_0x080c));
            goto error_result;
        }
    }

    rtdf_maskl(VGIP_CHN1_CTRL_0x0a10, ~(_BIT2|_BIT3), 0);

    fw_timer_event_DelayXms(10);
    ulResult =  rtdf_inl(SP_MS3STUS_0x080c);
    MEASURE_PRINTF(LOGGER_DEBUG,"=== ulResult:%x ====\n",(UINT32)ulResult);

    // Can't use info because of layer authority
    if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)// CSW+ 0961001 Invert VGA H/V Sync polarity to positive
    {
        if ((ulResult & _BIT9) && (ulResult & _BIT8)) //VS positive, HS positive
        {
//			info->Polarity = _SYNC_HP_VP;
            stModeInfo->Polarity = _SYNC_HP_VP;
            MEASURE_PRINTF(LOGGER_DEBUG,"=== HS, VS positive ====VGA\n");
        }
        else if (ulResult & _BIT9) 	//VS positive, HS negative
        {
//			info->Polarity = _SYNC_HN_VP;
            stModeInfo->Polarity = _SYNC_HN_VP;
            //Lewis, in de-composite IP will auto inverse polarity to positive
//			rtdf_maskl(Scaler_DispGetInputInfo(SLR_INPUT_CHANNEL)? VGIP_VGIP_CHN2_CTRL_VADDR: VGIP_VGIP_CHN1_CTRL_VADDR,, ~(_BIT2|_BIT3), _BIT2); // Let HS negtive polarity to positive polarity
            MEASURE_PRINTF(LOGGER_DEBUG,"=== HS negtive ====VGA\n");
        }
        else if (ulResult & _BIT8) 	//VS negative, HS positive
        {
            MEASURE_PRINTF(LOGGER_DEBUG,"=== VS negtive ====VGA\n");
            rtdf_maskl(VGIP_CHN1_CTRL_0x0a10, ~(_BIT2|_BIT3), _BIT3); // Let VS negtive polarity to positive polarity
//			info->Polarity =_SYNC_HP_VN;
            stModeInfo->Polarity =_SYNC_HP_VN;
        }
        else
        {
            MEASURE_PRINTF(LOGGER_DEBUG,"=== HS, VS negative ====VGA\n");
            //rtdf_maskl(Scaler_DispGetInputInfo(SLR_INPUT_CHANNEL)? VGIP_VGIP_CHN2_CTRL_VADDR: VGIP_VGIP_CHN1_CTRL_VADDR, ~(_BIT2|_BIT3), _BIT2|_BIT3); // Let HS/VS negtive polarity to positive polarity
            //Lewis, in de-composite IP will auto inverse polarity to positive
            rtdf_maskl(VGIP_CHN1_CTRL_0x0a10, ~(_BIT2|_BIT3), _BIT3); // Let VS negtive polarity to positive polarity
//			info->Polarity =_SYNC_HN_VN;
            stModeInfo->Polarity =_SYNC_HN_VN;
        }
    }
    else
    {
        if ((ulResult & _BIT9) && (ulResult & _BIT8)) //VS positive, HS positive
        {
//			info->Polarity = _SYNC_HP_VP;
            stModeInfo->Polarity = _SYNC_HP_VP;
            MEASURE_PRINTF(LOGGER_DEBUG,"=== HS, VS positive ====\n");
        }
        else if (ulResult & _BIT9) 	//VS positive, HS negative
        {
//			info->Polarity = _SYNC_HN_VP;
            stModeInfo->Polarity = _SYNC_HN_VP;
            MEASURE_PRINTF(LOGGER_DEBUG,"=== HS negtive ====\n");
        }
        else if (ulResult & _BIT8) 	//VS negative, HS positive
        {
            MEASURE_PRINTF(LOGGER_DEBUG,"=== VS negtive ====\n");
//			info->Polarity = _SYNC_HP_VN;
            stModeInfo->Polarity = _SYNC_HP_VN;
        }
        else
        {
            MEASURE_PRINTF(LOGGER_DEBUG,"=== HS, VS negative ====\n");
//			info->Polarity = _SYNC_HN_VN;
            stModeInfo->Polarity = _SYNC_HN_VN;
        }
    }

    return _MODE_SUCCESS;

error_result:
    return _MODE_NOSIGNAL;
}

void drv_measure_enableonlinemeasure(void)
{
    UINT32 status;
    // start online measure

    if ((GET_INPUTSOURCE_TYPE()  == _SOURCE_HDMI) ||(GET_INPUTSOURCE_FROM() == _SRC_FROM_ADC))
    {
        // set over range threshold (VSYNC_MAX_DELTA = 2-HSync, HSYNC_MAX_DELTA = 6-clock)
        rtdf_maskl(SP_MS1CTRL_0x0900, ~0x3ff,((6<<4)|2));  // set over range threshold (VSYNC_MAX_DELTA = 2-HSync, HSYNC_MAX_DELTA = 6-clock)
        if (GET_INPUTSOURCE_FROM() == _SRC_FROM_ADC)
            fw_timer_event_DelayXms(60);//delay for ADC source APLL stable

        rtdf_setBits(SP_MS1CTRL_0x0900, (_BIT16|_BIT9 |_BIT10)); // enable online measure
//rtdf_clearBits(SP_MS1CTRL_0x0900, _BIT16);
        status = rtdf_inl(SP_MS1STUS_0x090c);
        rtdf_outl(SP_MS1STUS_0x090c, status);
    }
}

UINT16 drv_measure_HighPulsePeriod(void)
{
    UINT16 value = rtdf_readWord0(SP1_STBRST1_0x0718);
    return value;
}

// Disable on-measure
void drv_measure_Disable_OnMeasure(void)
{
    rtdf_clearBits(SP_MS1CTRL_0x0900, (_BIT16|_BIT9 |_BIT10));
}

UINT8 flow_Get_Preset_Table_For_VDC(UINT8 type, UINT8 field, ModeInformationType* stModeInfo)
{
    switch (type)
    {
    case VDC_SOURCE_CVBS:
    case VDC_SOURCE_TV:
//		case VDC_SOURCE_SCART_AV:
#ifdef CONFIG_SCART_AUTO_SWITCH
    case VDC_SOURCE_FULL_SCART:
    case VDC_SOURCE_HALF_SCART:
#endif
        stModeInfo->IHTotal = tAV_PRESET_TABLE[field][0];
        stModeInfo->IHStartPos = tAV_PRESET_TABLE[field][1];
        stModeInfo->IHWidth = tAV_PRESET_TABLE[field][2];
        stModeInfo->IVStartPos = tAV_PRESET_TABLE[field][3];
        stModeInfo->IVHeight = tAV_PRESET_TABLE[field][4];
        break;

    case VDC_SOURCE_SV:
//		case VDC_SOURCE_SCART_SV:
        stModeInfo->IHTotal = tSV_PRESET_TABLE[field][0];
        stModeInfo->IHStartPos = tSV_PRESET_TABLE[field][1];
        stModeInfo->IHWidth = tSV_PRESET_TABLE[field][2];
        stModeInfo->IVStartPos = tSV_PRESET_TABLE[field][3];
        stModeInfo->IVHeight = tSV_PRESET_TABLE[field][4];
        break;

    default:
        return _FALSE;
    }

    return _TRUE;
}


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

#if 0 // not used
UINT16 mode_get_IVTotal(void)
{
//	UINT32 value,DclkOffset,Dclk,temp,temp2;
    UINT32 temp2;
//	UINT16 DVTotal,DHTotal ;
//	UINT8 M_code, N_code,div;

    UINT8 timeoutcnt;
    UINT32 ulResult;

    //fw_scaler_set_vgip(VGIP_SRC_ADC, VGIP_MODE_ANALOG);
    temp2 = rtd_inl(VGIP_CHN1_CTRL_0x0a10);
    // Disable SRC_Clock_Enbale bit
    rtdf_maskl(VGIP_CHN1_CTRL_0x0a10, ~_BIT0,0x0);
    // 2nd determine the input source
    rtdf_maskl(VGIP_CHN1_CTRL_0x0a10, ~(_BIT30|_BIT29|_BIT28|_BIT1), _BIT31|((UINT32)VGIP_SRC_ADC<<28) | ((UINT32)(VGIP_MODE_ANALOG & 0x01)<<1));
    // 3rd enable SRC_Clock_Enbale bit
    rtdf_maskl(VGIP_CHN1_CTRL_0x0a10, ~(_BIT0), _BIT0);

    RTD_Log(LOGGER_DEBUG, "\nVGIP_CHN1_CTRL_0x0a10 = %x\n", (UINT32)rtd_inl(VGIP_CHN1_CTRL_0x0a10));
    RTD_Log(LOGGER_DEBUG, "SP_MS1CTRL_0x0900 = %x\n", (UINT32)rtd_inl(SP_MS1CTRL_0x0900));
    RTD_Log(LOGGER_DEBUG, "SP1_CTRL_0x0700 = %x\n", (UINT32)rtd_inl(SP1_CTRL_0x0700));
    rtdf_clearBits(SP1_CTRL_0x0700, _BIT17);


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
    RTD_Log(LOGGER_DEBUG,"SP_MS1RST0_0x0904 = %x\n",(UINT32)rtdf_inl(SP_MS1RST0_0x0904));

    ulResult = (UINT32)(rtdf_inl(SP_MS1RST0_0x0904) & 0x0fff0000)>>16;
    RTD_Log(LOGGER_DEBUG,"ivcount_ulResult = %d\n",(UINT32)ulResult);
    rtdf_setBits(SP1_CTRL_0x0700, _BIT17);

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

UINT8 mode_judge_confuse_mode(UINT8 mode_index,UINT16 usHighPulsePeriod)
{
    UINT8 mode_table_index = mode_SearchModeMap(mode_index);


    if (VGADetecSyncTypeMode == SEPARATE_MODE)
    {
//RTD_Log(LOGGER_INFO,"stModeInfo.IHSyncPulseCount--------SEPARATE_MODE-----------------%x\n",(UINT32)stModeInfo.IHSyncPulseCount);
//RTD_Log(LOGGER_INFO,"usHighPulsePeriod--------SEPARATE_MODE-----------------%x\n",(UINT32)usHighPulsePeriod);
//RTD_Log(LOGGER_INFO,"mode_table_index--------SEPARATE_MODE-----------------%x\n",(UINT32)mode_table_index);
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
            else if (ABS(stModeInfo.IVCount,tINPUTMODE_PRESET_TABLE[mode_table_index].IVTotal) > 3)
                return _FALSE;
        }
        else if (mode_index == _MODE_640x480_72HZ)
        {
            if (ABS(stModeInfo.IVCount,tINPUTMODE_PRESET_TABLE[mode_table_index].IVTotal) > 3)
                return _FALSE;
        }
        else if (mode_index == _MODE_640x480_75HZ)
        {
            if ((ABS(usHighPulsePeriod ,0x37) > 10) || (info->Polarity == _SYNC_HN_VP))
                return _FALSE;
            else if ((ABS(stModeInfo.IVCount,tINPUTMODE_PRESET_TABLE[mode_table_index].IVTotal) > 2)||(stModeInfo.IVCount>tINPUTMODE_PRESET_TABLE[mode_table_index].IVTotal))
                return _FALSE;
        }
        else if (mode_index == _MODE_800x600_60HZ)
        {
            if (ABS(usHighPulsePeriod ,0x58) > 10)
                return _FALSE;
        }
        else if (mode_index == _MODE_800x600_72HZ)
        {
            if (ABS(stModeInfo.IHSyncPulseCount,64) > 20)
                return _FALSE;
        }
        else if (mode_index == _MODE_800x600_75HZ)
        {
            if (ABS(stModeInfo.IHSyncPulseCount,44) > 20)
                return _FALSE;
        }
	 else if (mode_index == _MODE_960x600_60HZ)
        {
            if (ABS(stModeInfo.IVCount,tINPUTMODE_PRESET_TABLE[mode_table_index].IVTotal) > 3)
                return _FALSE;
        }
        else if (mode_index == _MODE_848x480_60HZ)
        {
            if (ABS(stModeInfo.IVCount,tINPUTMODE_PRESET_TABLE[mode_table_index].IVTotal) > 3)
                return _FALSE;
        }
        else if (mode_index == _MODE_1024x768_60HZ)
        {
            if (ABS(stModeInfo.IVCount,tINPUTMODE_PRESET_TABLE[mode_table_index].IVTotal) > 3)
                return _FALSE;
        }
        else if (mode_index == _MODE_1024x768_70HZ)
        {
            if (ABS(stModeInfo.IVCount,tINPUTMODE_PRESET_TABLE[mode_table_index].IVTotal) > 3)
                return _FALSE;
        }
        //<Stanley 2K100105> marked, because it will be ignore correct timing at Chroma 22291

        else if (mode_index == _MODE_1024x768_75HZ)
        {
            if (ABS(stModeInfo.IHSyncPulseCount,414) <= 10)
                return _FALSE;
        }
        else if ((mode_index == _MODE_1024x800_73HZ)/* && (info->Polarity == _SYNC_HN_VP)*/)
        {
            if (ABS(usHighPulsePeriod ,0x1f) < 3) // CSW+ 0980527 Should be 1280*768 60 not 1024*768 60Hz
                return _FALSE;
        }
        else if (mode_index == _MODE_1152x864_60HZ)
        {
            if (ABS(stModeInfo.IVCount, tINPUTMODE_PRESET_TABLE[mode_table_index].IVTotal) >3)
                return _FALSE;
        }
        else if (mode_index == _MODE_1152x864_75HZ)
        {
            if (ABS(stModeInfo.IVCount,914) < 3)
                return _FALSE;
        }
        /*else if (mode_index == _MODE_1280x720_60HZ){
        	if (ABS(usHighPulsePeriod ,0x31) > 5) // Should be VGA 720p not VGA 1280*720
        		return _FALSE;
        }*/
        else if (mode_index == _MODE_1280x768_60HZ)
        {
            if (ABS(stModeInfo.IVCount, tINPUTMODE_PRESET_TABLE[mode_table_index].IVTotal) >5)
                return _FALSE;
            if (ABS(usHighPulsePeriod ,0x2c) > 3) // CSW+ 0970115 Should be 1366*768 60 not 1280*768 60Hz
                return _FALSE;
        }
        else if (mode_index == _MODE_1280x768_60HZ_RB)
        {
            if (stModeInfo.IVCount>tINPUTMODE_PRESET_TABLE[mode_table_index].IVTotal)
                return _FALSE;
        }
        else if (mode_index == _MODE_1280x768_75HZ)
        {
            if (ABS(stModeInfo.IHSyncPulseCount,411) > 10)
                return _FALSE;

        }
        else if (mode_index == _MODE_1280x800_60HZ)
        {
            if (ABS(stModeInfo.IVCount,tINPUTMODE_PRESET_TABLE[mode_table_index].IVTotal) > 4)
                return _FALSE;
        }
        else if (mode_index == _MODE_1280x1024_60HZ)
        {
            if ((ABS(stModeInfo.IHSyncPulseCount,6) < 10) || (ABS(stModeInfo.IHSyncPulseCount, 386) < 10))
                return _FALSE;
        }

        else if (mode_index == _MODE_1440x900_60HZ)
        {
            if (ABS(stModeInfo.IVCount,tINPUTMODE_PRESET_TABLE[mode_table_index].IVTotal) > 3)
                return _FALSE;
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
            if (ABS(stModeInfo.IVCount, tINPUTMODE_PRESET_TABLE[mode_table_index].IVTotal) >3)
                return _FALSE;
        }
//		else if((mode_index == _MODE_1400x1050_50HZ)||(mode_index == _MODE_1400x1050_60RHZ)||(mode_index == _MODE_1400x1050_60HZ)||(mode_index == _MODE_1400x1050_75HZ)){
//				return _FALSE;//use 1680x1050 display
//		}
        else if (mode_index == _MODE_1680x1050_60HZ)
        {
            if (ABS(stModeInfo.IVCount, tINPUTMODE_PRESET_TABLE[mode_table_index].IVTotal) >3)
                return _FALSE;
        }
//		else if(mode_index == _MODE_1600x1200_60HZ){
//			if(ABS(stModeInfo.IVCount, tINPUTMODE_PRESET_TABLE[mode_table_index].IVTotal) >3)
//				return _FALSE;
//		}
        else if (mode_index == _MODE_1920x1080_60Hz_148)
        {
            if (ABS(stModeInfo.IVCount, tINPUTMODE_PRESET_TABLE[mode_table_index].IVTotal) >3)
                return _FALSE;
        }

    }
    else
    {
//RTD_Log(LOGGER_INFO,"stModeInfo.IHSyncPulseCount--------OTHERMODE----------------%x\n",(UINT32)stModeInfo.IHSyncPulseCount);
//RTD_Log(LOGGER_INFO,"usHighPulsePeriod--------OTHER MODE-----------------%x\n",(UINT32)usHighPulsePeriod);
//RTD_Log(LOGGER_INFO,"mode_table_index----------OTHER MODE------------------%x\n",(UINT32)mode_table_index);
        if (mode_index == _MODE_640x400_70HZ)
        {
            return _FALSE;//display by 720x400@70Hz
        }
        else if ((mode_index == _MODE_640x350_70HZ))
        {
            return _FALSE;//display by 720x400@70Hz
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
            if (stModeInfo.IVCount>tINPUTMODE_PRESET_TABLE[mode_table_index].IVTotal)
                return _FALSE;
            if (ABS(stModeInfo.IHSyncPulseCount, 36)<3)//<Stanley 2K100104>add for chroma 22291, 1280x768@75, H+V sync type.
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
        else if (mode_index == _MODE_1600x1200_60HZ)
        {
            if (ABS(stModeInfo.IVCount, tINPUTMODE_PRESET_TABLE[mode_table_index].IVTotal) >10)//should be 1920x1200
                return _FALSE;
        }
        else if (mode_index == _MODE_1920x1080_60Hz_138)
        {
            if (stModeInfo.IVCount>tINPUTMODE_PRESET_TABLE[mode_table_index].IVTotal)
                return _FALSE;
        }
        else if (mode_index == _MODE_1920x1080_60Hz_148)
        {
            if (ABS(stModeInfo.IVCount, tINPUTMODE_PRESET_TABLE[mode_table_index].IVTotal) >3)
                return _FALSE;
        }

    }
    return _TRUE;
}


UINT8 mode_identifymode(UINT8 modeno)
{
    UINT16 usHighPulsePeriod=0;
    UINT8 mode_index;

    mode_index = mode_SearchModeMap(modeno);
    //RTD_Log(LOGGER_DEBUG,"mode_identifymode mode_no = %x\n",(UINT32)modeno);
    //RTD_Log(LOGGER_DEBUG,"mode_identifymode mode_index = %x\n",(UINT32)mode_index);
    if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
    {
        // CSW+  add usHighPulsePeriod comparison to enhace mode identification
        RTD_Log(LOGGER_LEVEL,"\n stModeInfo.IHFreq = %d,tINPUTMODE_PRESET_TABLE[mode_index].IHFreq=%d \n",(UINT32)stModeInfo.IHFreq,(UINT32)tINPUTMODE_PRESET_TABLE[mode_index].IHFreq);
        if (ABS(stModeInfo.IHFreq , tINPUTMODE_PRESET_TABLE[mode_index].IHFreq) > tINPUTMODE_PRESET_TABLE[mode_index].IHFreqTolerance)
            return FALSE;
        if (VGADetecSyncTypeMode == SEPARATE_MODE)
        {
            if (ABS(stModeInfo.IVFreq , tINPUTMODE_PRESET_TABLE[mode_index].IVFreq) > tINPUTMODE_PRESET_TABLE[mode_index].IVFreqTolerance)
                return FALSE;

            if ((tINPUTMODE_PRESET_TABLE[mode_index].PolarityFlag & stModeInfo.Polarity) == 0 )
            {
                RTD_Log(LOGGER_DEBUG,"mode_index = %d\n\ntINPUTMODE_PRESET_TABLE[mode_index].PolarityFlag = %x\ninfo.Polarity = %x\n"
                        ,(UINT32)mode_index,(UINT32)tINPUTMODE_PRESET_TABLE[mode_index].PolarityFlag,(UINT32)stModeInfo.Polarity);
                return FALSE;
            }
        }
        else
        {
            if (ABS(stModeInfo.IVFreq , tINPUTMODE_PRESET_TABLE[mode_index].IVFreq) > (tINPUTMODE_PRESET_TABLE[mode_index].IVFreqTolerance+5))
                return FALSE;
        }

        usHighPulsePeriod = drv_measure_HighPulsePeriod();
        RTD_Log(LOGGER_DEBUG,"\n####### usHighPulsePeriod = %x\n",(UINT32)usHighPulsePeriod);
        if (mode_judge_confuse_mode(modeno, usHighPulsePeriod)==_FALSE)
        {
            RTD_Log(LOGGER_DEBUG,"\n mode_judge_confuse_mode fail = %d\n",(UINT32)modeno);
            return _FALSE;
        }

    }
    else
    {
        //RTD_Log(LOGGER_DEBUG,"stModeInfo.IHFreq = %d\n",(UINT32)stModeInfo.IHFreq);
        //RTD_Log(LOGGER_DEBUG,"stModeInfo.IVFreq = %d\n",(UINT32)stModeInfo.IVFreq);
        //RTD_Log(LOGGER_DEBUG,"tINPUTMODE_PRESET_TABLE[mode_index].IHFreq = %d\n",(UINT32)tINPUTMODE_PRESET_TABLE[mode_index].IHFreq);
       // RTD_Log(LOGGER_DEBUG,"tINPUTMODE_PRESET_TABLE[mode_index].IVFreq) = %d\n",(UINT32)tINPUTMODE_PRESET_TABLE[mode_index].IVFreq);
		//RTD_Log(LOGGER_DEBUG,"tINPUTMODE_PRESET_TABLE[mode_index].IHStartPos) = %d\n",(UINT32)tINPUTMODE_PRESET_TABLE[mode_index].IHStartPos);
	//	RTD_Log(LOGGER_DEBUG,"tINPUTMODE_PRESET_TABLE[mode_index].IVStartPos) = %d\n",(UINT32)tINPUTMODE_PRESET_TABLE[mode_index].IVStartPos);
        if (ABS(stModeInfo.IHFreq , tINPUTMODE_PRESET_TABLE[mode_index].IHFreq) > tINPUTMODE_PRESET_TABLE[mode_index].IHFreqTolerance)
            return FALSE;
        if (ABS(stModeInfo.IVFreq , tINPUTMODE_PRESET_TABLE[mode_index].IVFreq) > tINPUTMODE_PRESET_TABLE[mode_index].IVFreqTolerance)
            return FALSE;
        if (GET_INPUTSOURCE_TYPE() == _SOURCE_HDMI)
        {
            if (ABS(stModeInfo.IVCount , tINPUTMODE_PRESET_TABLE[mode_index].IVTotal) > 10)
                return FALSE;
            if (ABS(stModeInfo.IHCount , tINPUTMODE_PRESET_TABLE[mode_index].IHTotal) > 10)
                return FALSE;
        }
    }

    return TRUE;
}
#if 0
UINT8 mode_identify_user_mode(UINT8 modeno)
{
    UINT16 usHighPulsePeriod=0;
    UINT8 mode_index;

    mode_index = mode_SearchModeMap(modeno);
    //RTD_Log(LOGGER_DEBUG,"mode_identifymode mode_no = %x\n",(UINT32)modeno);
    RTD_Log(LOGGER_DEBUG,"mode_identify_user_mode mode_index = %d\n",(UINT32)mode_index);
    RTD_Log(LOGGER_DEBUG,"IVFq=%d  tbIVFq=%d\n IVTt=%d  tbIVTt%d\n"
            ,(UINT32)stModeInfo.IVFreq,(UINT32)tINPUTMODE_PRESET_TABLE[mode_index].IVFreq,(UINT32)stModeInfo.IVCount,(UINT32)tINPUTMODE_PRESET_TABLE[mode_index].IVTotal);

    if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
    {
        if (ABS(stModeInfo.IVFreq , tINPUTMODE_PRESET_TABLE[mode_index].IVFreq) > 25)
            return FALSE;
        if (ABS(stModeInfo.IVCount , tINPUTMODE_PRESET_TABLE[mode_index].IVTotal) >20)
            return FALSE;
    }
    return TRUE;
}
#endif
UINT8 mode_searchmode(void)
{
    UINT8 search_cnt=0;
    UINT16 start_index=0, end_index=0;
    RTD_Log(LOGGER_DEBUG,"mode_searchmode start!!\n");
	
    if (stModeInfo.IVFreq >_MAX_VFREQ )
        return FALSE;

    if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
    {
        start_index =  _MODE_640x350_70HZ;
        end_index = _MODE_USER_MODE-1;
    }
    else if (GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR)
    {
        start_index =  _MODE_480I;
        end_index =	_MODE_1080P30;
    }

    for (search_cnt = start_index; search_cnt <= end_index; search_cnt++)
    {
        //RTD_Log(LOGGER_DEBUG,"mode_searchmode search_cnt= %x\n",(UINT32)search_cnt);
        if (mode_identifymode(search_cnt))
        {
            //interlace setting
            if ((search_cnt == _MODE_480I)  || (search_cnt == _MODE_576I) ||
                    (search_cnt == _MODE_1080I25) || (search_cnt == _MODE_1080I30))
            {
                RTD_Log(LOGGER_DEBUG,"SET_INTERLACE_IN_INPUT_MODE\n");
                SET_INTERLACE_IN_INPUT_MODE();
            }
            else
                CLR_INTERLACE_IN_INPUT_MODE();

            RTD_Log(LOGGER_DEBUG,"\n mode_searchmode OK,mode id%d\n",(UINT32)search_cnt);
            RTD_Log(LOGGER_DEBUG,"Interlace mode =%x\n",(UINT32)GET_INTERLACE_IN_INPUT_MODE());
            stModeInfo.ModeCurr = search_cnt;
            return search_cnt;
        }
    }
    RTD_Log(LOGGER_DEBUG,"\nmode_searchmode fail, start search user mode \n");
    if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)//tommy modify20091109 for prevent YPBPR get incorrect parameter
        return  _MODE_NOSUPPORT;
	
    if (GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR)//tommy modify20091109 for prevent YPBPR get incorrect parameter
        return  _MODE_NOSUPPORT;
    else
        return  _MODE_USER_MODE;

   
    //return  _MODE_NOSUPPORT;
}

UINT8 flow_mode_detectmode(UINT8 source)
{
    UINT8 result = _MODE_NOSIGNAL;

    //RTD_Log(LOGGER_DEBUG,"drvif_mode_detectmode\n");

    if (source == _OFFLINE_MS_SRC_HDMI)
    {
        //rtdf_clearBits(HDMI_TMDS_OUTCTL_ADDR, _BIT2);
        //fw_timer_event_DelayXms(100);
        /*if(mode_hdmidetect()!= _MODE_SUCCESS){
        	//rtdf_clearBits(HDMI_TMDS_OUTCTL_ADDR, _BIT2);
        	//fw_timer_event_DelayXms(100);
        	goto error_result;
        }*/
    }
    else
    {
        if (drv_measure_offlinemeasure(source, ANALOG_MODE_MEASUREMENT, &stModeInfo) != _MODE_SUCCESS)
            goto error_result;
    }

    result =mode_searchmode();
    RTD_Log(LOGGER_DEBUG,"\nmode_searchmode result = %d\n", (UINT32)result);
    if ((result != _MODE_NOSUPPORT) && (result != _MODE_NOSIGNAL))
    {
        mode_setinfofromtable(result);
    }
    else
    {
        RTD_Log(LOGGER_DEBUG,"drvif_mode_detectmode error!!!\n");
        goto error_result;
    }

    return _MODE_SUCCESS;

error_result:
    drv_syncproc_FunctionDisable();
    return result;
}
UINT16 mode_get_user_mode_IVLength(UINT16 IVTotal)
{
    if (IVTotal<500)
        return 400;
    else if (IVTotal<600)
        return 480;
    else if (IVTotal<700)
        return 600;
    else if (IVTotal<770)
        return 720;
    else if (IVTotal<850)
        return 768;
    else if (IVTotal<950)
        return 864;
    else if (IVTotal<1020)
        return 960;
    else if (IVTotal<1070)
        return 1024;
    else if (IVTotal<1100)
        return 1050;
    else if (IVTotal<1170)
        return 1080;
    else if (IVTotal<1300)
        return 1200;
}

#ifdef CONFIG_ENABLE_HDMI
void mode_setHDMIinfo(UINT8 modeno)
{
	UINT8	OverScan_HWidth,OverScan_VHeight;
    UINT8 index = mode_SearchModeMap(modeno);
    stModeInfo.IHStartPos = tINPUTMODE_PRESET_TABLE[index].IHStartPos;
    stModeInfo.IHWidth =tINPUTMODE_PRESET_TABLE[index].IHWidth;
    stModeInfo.IVStartPos = tINPUTMODE_PRESET_TABLE[index].IVStartPos;
    stModeInfo.IVHeight = tINPUTMODE_PRESET_TABLE[index].IVHeight;
	
	if(_HDMI_OVERSCAN_PERCENT ==_OVERSCAN_93)

	{
		OverScan_HWidth = (UINT8)(stModeInfo.IHWidth/16);	//Horizontal: 93.75%
		OverScan_VHeight= (UINT8)(stModeInfo.IVHeight/16);	//Vertical: 93.75%   //forster modified 061102 for HDMI 576P @ 1280x1024 panel frame sync problem
	}
	else if(_HDMI_OVERSCAN_PERCENT==_OVERSCAN_95)
	{
		OverScan_HWidth = (UINT8)(stModeInfo.IHWidth/20);		//Horizontal: 95.00%
		OverScan_VHeight= (UINT8)(stModeInfo.IVHeight/20);		//Vertical: 95.00%
	}
	else if(_HDMI_OVERSCAN_PERCENT==_OVERSCAN_97)
	{
		OverScan_HWidth = (UINT8)(stModeInfo.IHWidth/32);		//Horizontal: 95.00%
		OverScan_VHeight= (UINT8)(stModeInfo.IVHeight/32);		//Vertical: 95.00%
	}
	else 
	{
		OverScan_HWidth = 0;															//Horizontal: 100.00%
		OverScan_VHeight= 0;	
	}//Vertical: 100.00%

	stModeInfo.IHWidth = stModeInfo.IHWidth - OverScan_HWidth; 
	if(stModeInfo.IHWidth % 8)
		stModeInfo.IHWidth = ((stModeInfo.IHWidth + 4) >> 3) << 3;
	stModeInfo.IHStartPos = stModeInfo.IHStartPos + OverScan_HWidth/2;
	stModeInfo.IVHeight = stModeInfo.IVHeight - OverScan_VHeight;
	stModeInfo.IVStartPos = stModeInfo.IVStartPos + OverScan_VHeight/2;
}
#endif
void mode_setinfofromtable(UINT8 modeno)
{
    UINT8 index;

    info->IHSyncPulseCount = stModeInfo.IHSyncPulseCount;
    info->Hsync = stModeInfo.IHCount;
    info->Vsync = stModeInfo.IVCount;
    info->Polarity = stModeInfo.Polarity;

    if ((GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)&& modeno==_MODE_USER_MODE)
    {
        RTD_Log(LOGGER_DEBUG,"\nmode_setinfofromtable user mode!!\n");
        info->ucMode_Curr = _MODE_USER_MODE;
        info->IVTotal = stModeInfo.IVCount ;
        info->IHFreq = stModeInfo.IHFreq ;
        info->IVFreq = stModeInfo.IVFreq ;
        info->IPV_ACT_LEN = mode_get_user_mode_IVLength(stModeInfo.IVTotal);
        info->IPH_ACT_WID = ((UINT32)info->IPV_ACT_LEN<<2)/3;
        info->IHTotal = ((UINT32)info->IPH_ACT_WID<<2)/3;
        info->ADC_CLOCK= info->IHTotal;
        info->IPV_ACT_STA = ((UINT32)(info->IVTotal - info->IPV_ACT_LEN)<<2)/5;
        info->IPH_ACT_STA = ((UINT32)(info->IHTotal - info->IPH_ACT_WID)<<2)/5;
    }
    else
    {
        RTD_Log(LOGGER_DEBUG,"\nmode_setinfofromtable from table!!\n");
        index = mode_SearchModeMap(modeno);
        info->ucMode_Curr = GET_MODE_INPUT_MODE(tINPUTMODE_PRESET_TABLE[index].ModeCurr);
        info->ADC_CLOCK= tINPUTMODE_PRESET_TABLE[index].IHTotal;
        info->IHTotal = tINPUTMODE_PRESET_TABLE[index].IHTotal;
        info->IPH_ACT_STA= tINPUTMODE_PRESET_TABLE[index].IHStartPos;
        info->IPH_ACT_WID= tINPUTMODE_PRESET_TABLE[index].IHWidth;
        info->IHFreq = tINPUTMODE_PRESET_TABLE[index].IHFreq;
        info->IVFreq = tINPUTMODE_PRESET_TABLE[index].IVFreq;
        info->IPV_ACT_STA= tINPUTMODE_PRESET_TABLE[index].IVStartPos;
        info->IPV_ACT_LEN= tINPUTMODE_PRESET_TABLE[index].IVHeight;
        info->IVTotal = tINPUTMODE_PRESET_TABLE[index].IVTotal;

    }
/*  if(GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR)
  	{
	if(_YPBPR_OVERSCAN_PERCENT ==_OVERSCAN_93)
	{
		OverScan_HWidth = (UINT8)(info->IPH_ACT_WID/16);	//Horizontal: 93.75%
		OverScan_VHeight= (UINT8)( info->IPV_ACT_LEN/16);	//Vertical: 93.75%   //forster modified 061102 for HDMI 576P @ 1280x1024 panel frame sync problem
	}
	else if(_YPBPR_OVERSCAN_PERCENT==_OVERSCAN_95)
	{
		OverScan_HWidth = (UINT8)(info->IPH_ACT_WID/20);		//Horizontal: 95.00%
		OverScan_VHeight= (UINT8)( info->IPV_ACT_LEN/20);		//Vertical: 95.00%
	}
	else if(_YPBPR_OVERSCAN_PERCENT==_OVERSCAN_97)
	{
		OverScan_HWidth = (UINT8)(info->IPH_ACT_WID/32);		//Horizontal: 95.00%
		OverScan_VHeight= (UINT8)( info->IPV_ACT_LEN/32);		//Vertical: 95.00%
	}
	else 
	{
		OverScan_HWidth = 0;															//Horizontal: 100.00%
		OverScan_VHeight= 0;	
	}//Vertical: 100.00%

	info->IPH_ACT_WID = info->IPH_ACT_WID - OverScan_HWidth; 
	if(info->IPH_ACT_WID % 8)
		info->IPH_ACT_WID = ((info->IPH_ACT_WID + 4) >> 3) << 3;
	info->IPH_ACT_STA = info->IPH_ACT_STA + OverScan_HWidth/2;
	info->IPV_ACT_LEN = info->IPV_ACT_LEN - OverScan_VHeight;
	info->IPV_ACT_STA= info->IPV_ACT_STA + OverScan_VHeight/2;
  	}*/
    info->IPH_ACT_STA_PRE = info->IPH_ACT_STA;
    info->IPH_ACT_WID_PRE = info->IPH_ACT_WID;
    info->IPV_ACT_STA_PRE = info->IPV_ACT_STA;
    info->IPV_ACT_LEN_PRE = info->IPV_ACT_LEN;
    if ((info->ucMode_Curr == _MODE_480I) ||
            (info->ucMode_Curr == _MODE_576I) ||
            (info->ucMode_Curr == _MODE_1080I) ||
            (info->ucMode_Curr == _MODE_1080I25) ||
            (info->ucMode_Curr == _MODE_1080I30) )
        SET_INTERLACE_MODE();
    else
        CLR_INTERLACE_MODE();

    RTD_Log(LOGGER_DEBUG,"**HS=%d,HW=%d,VS=%d,VW=%d\n",(UINT32)info->IPH_ACT_STA,(UINT32)info->IPH_ACT_WID,(UINT32)info->IPV_ACT_STA,(UINT32)info->IPV_ACT_LEN);
    RTD_Log(LOGGER_DEBUG,"**IHFreq=%d,IVFreq=%d,IHTotal=%d,IVTotal=%d,ADC_CLOCK:%d\n",(UINT32)info->IHFreq,(UINT32)info->IVFreq,(UINT32)info->IHTotal,(UINT32)info->IVTotal,(UINT32)info->ADC_CLOCK);

}
#if 0 // not used
UINT16 flow_Get_SrcMode_Parameter(UINT8 idx, UINT16 offset, UINT8 len)
{
    UINT16* ptVal;
    ptVal = (UINT16*)((UINT8*)&tINPUTMODE_PRESET_TABLE[idx] + offset);

    if (len == 1)
    {
        return ((*ptVal & 0xff00) >> 8);
    }
    else if (len == 2)
    {
        return *ptVal;
    }
    else
        return 0;
}
#endif

// Disable on-measure's watch dog
//void drv_measure_Disable_OnMeasure_Watchdog(void)
//{
//}

//clear online ms error flag
//void drv_measure_Clear_OnMeasure_Status(void)
//{
//}
