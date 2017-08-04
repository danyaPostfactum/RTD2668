/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2006
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is process key and ir input related functions.
 *
 * @author 	$Author$
 * @date		$Date$
 * @version	$Revision$
 * @ingroup	input
 */

/**
 * @addtogroup input
 * @{
 */

/*============================ Module dependency  ===========================*/
#include "signal\sig_ypbpr.h"
#include "scaler_idomain\adc.h"
#include "scaler_idomain\syncproc.h"
#include "scaler_idomain\vgip.h"
#include "sparrow.h"
#include "reg_def.h"
#include "mode.h"
#include "timer_event.h"
#include "source.h"
#include "rosprintf.h"
#include "flow_struct.h"
#include "pcb_customer.h"
#include "rtd_types.h"
#include "pcb\pcb_utility.h"
#include "scaler_idomain\abl.h"
#include "audio/audio_api.h"
#include "framesync.h"
#include "timer_event_sys_table.h"


//#include "scaler_idomain\dcrestore.h"

#ifdef CONFIG_VBI_CC_SUPPORT
#include "vbi/slr.h"
#endif


/*===================================  Types ================================*/
/*================================== Variables ==============================*/
/*================================ Definitions ==============================*/
/*================================== Function ===============================*/
void drv_sig_ypbpr_Init(void)
{
    //RTD_Log(LOGGER_INFO, "drv_sig_ypbpr_Init\n");
	//rtd_maskl(DOWN_SAMPLE_FIFO_SOURCE_SEL_0x3554,~(_BIT18|_BIT19),(_BIT18|_BIT19));
	
	fw_timer_event_ReactiveTimerEvent(SEC(1), timerevent_table_showloading);
	#if CONFIG_ENABLE_USB
	STB_DVDPWR_CTRL(1);
	#endif
    YPBPR_SOURCE_SELECT(ADC0_MUX_YPBPR);
    drv_adc_SOY0_input_select(GET_INPUTSOURCE_INDEX());
    drv_syncproc_InitSrc(YPBPR_INIT);
    drv_adc_ADCSelectInput(GET_INPUTSOURCE_INDEX());
#ifdef CONFIG_YPBPR_USE_AUTO_SOY
    drv_auto_soy_AutoSOY();
#endif
    drv_syncproc_PLL27X_Enable(_ENABLE);
}

UINT8 drv_sig_ypbpr_DetectMode(StructSourceInput *srcinput_pt)
{
#ifdef CONFIG_VBI_CC_SUPPORT
    UINT8 result;
#endif

    ADC_INITIAL_OPTIONS  *ptOptions;
    ptOptions =  srcinput_pt->InitOptions;

#ifdef CONFIG_VBI_CC_SUPPORT
    result = drv_syncproc_DetectMode(srcinput_pt);
    if ( result == _MODE_SUCCESS )
        drvif_vbi_cc_verify_play_ypbpr();
    return result;
#else
    return drv_syncproc_DetectMode(srcinput_pt);
#endif
}

bit drv_sig_ypbpr_CheckMode(void)
{
    static UINT8 usClampCheckCount = 0;
    Audio_HwpInputNoiseReduction();
    if (flow_framesync_status())
        return _FALSE;
    if (flow_linebuffer_status())
        return _FALSE;
    if (drv_syncproc_CheckMode())
    {	//input Timing is stable
        if (usClampCheckCount > CLAMP_POSITION_CHECK_CNT)
        {
            usClampCheckCount = 0;
            //drv_syncproc_ClampPositionAdjust();
        }
        else
            usClampCheckCount++;

        return TRUE;
    }
    else
    {	//input Timing is un-stable
        usClampCheckCount = 0;
        return FALSE;
    }

}

void drv_sig_ypbpr_SetupMode(void)
{
    RTD_Log(LOGGER_INFO, "drv_sig_ypbpr_SetupMode\n");

    drv_vgip_set_vgip(VGIP_SRC_ADC, VGIP_MODE_ANALOG);
    drv_syncproc_inprocypbpr_setup();
    rtdf_outl(VGIP_CHN1_MISC_0x0a24, 0x80<<8); //Video_Processor_H_Backporch:0x80
    rtdf_clearBits(VGIP_CHN1_CTRL_0x0a10, _BIT3);  // CSW+  0960824 for IVS polarity unchange
    drv_vgip_set_sample(_ENABLE);//for DI bug when do HSD
    //ypbpr_decide_main_timing_mode();
    drvif_abl_setting_on();
}

void drv_sig_ypbpr_Release(void)
{
    RTD_Log(LOGGER_INFO, "\n drv_sig_ypbpr_Release\n");

    drv_syncproc_FunctionPowerDown();
    drvif_abl_disable();
    drv_syncproc_PLL27X_Enable(_DISABLE);
}



/*============================================================================*/
