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
#include "signal\sig_vga.h"
#include "scaler_idomain\adc.h"
#include "scaler_idomain\syncproc.h"
#include "scaler_idomain\vgip.h"
#include "mode_customer.h"
#include "sparrow.h"
#include "reg_def.h"
#include "mode.h"
#include "timer_event.h"
#include "source.h"
#include "rosprintf.h"
#include "flow_struct.h"
#include "scaler_idomain\abl.h"
#include "rtd_types.h"
#include "rt_storage.h"
#include "storage_system.h"
#include "ui_struct.h"
#include "osdcore.h"
#include "audio/audio_api.h"
#include "framesync.h"
#include "flow_conf_struct.h"
#include "sparrow_utility.h"
#include "flash\spi_flash.h"

#define _VGAEDID_
#include "panel\panel.h"
/*===================================  Types ================================*/
/*================================== Variables ==============================*/
/*================================ Definitions ==============================*/
/*================================== Function ===============================*/
void drv_sig_vga_edid_init(void)
{
#if 1
    UINT8 getVal[16];
    UINT8 nBank;
    UINT16 nFlashAddr;
    int i, j;
    bit fDefault;

    // Check
    GET_FLOW_PARAMETER_DATA(KEYCODE_BANK, nBank);
    GET_FLOW_PARAMETER_DATA(KEYCODE_EDID1_ADDR, nFlashAddr);
    drv_spi_flash_Read((UINT16)&getVal, nBank, nFlashAddr, 16);
    fDefault = 1;
    if (getVal[0] == 0x00)
    {
        i = 1;
        while (i < 7)
        {
            if (getVal[i] != 0xff)
            {
                break;
            }
            i++;
        }

        if (i >= 7)
        {
            while (i < 16)
            {
                if (getVal[i] == 0xff)
                {
                    break;
                }
                i++;
            }
        }

        if (i >= 16)
            fDefault = 0;
    }

    rtd_outl(DDC_SIR_0x2720,0);	//SRAM Index = 0
    if (fDefault)  	// use default
    {
        RTD_Log(LOGGER_INFO, "use default vga edid\n");
        for (i=0; i<128; i++)
            rtd_outl(DDC_SAP_0x2724, EDID_KEY_TABLE[i]);
    }
    else
    {
        RTD_Log(LOGGER_INFO, "use vga edid : keycode bank = %x, flash_addr = %x\n", (UINT32)nBank, (UINT32)nFlashAddr);
        i = 128;
        while (i)
        {
            if (i > 16)
            {
                drv_spi_flash_Read((UINT16)getVal, nBank, nFlashAddr, 16);
                for (j = 0; j < 16; j++)
                    rtd_outl(DDC_SAP_0x2724, getVal[j]);

                nFlashAddr += 16;
                i -= 16;
            }
            else
            {
                drv_spi_flash_Read((UINT16)getVal, nBank, nFlashAddr, i);
                for (j = 0; j < i; j++)
                    rtd_outl(DDC_SAP_0x2724, getVal[j]);

                nFlashAddr += 16;
                i = 0;
            }
        }
    }
    rtd_outl(DDC_SIR_0x2720,0);	//SRAM Index = 0
    rtd_outl(EDID_CR_0x270c, 0x3);	// External Access Enable(BIT0) and I2C Master SRAM Write Enable(BIT1)
    rtdf_setBits(I2C_CR_0x2700,_BIT4|_BIT5);//add for ddc0 I2C CLOCK _BIT4|_BIT5 clocks debounce
#else
    UINT8 i;

    rtd_outl(DDC_SIR_0x2720,0);	//SRAM Index = 0
    for (i=0; i<128; i++)
    {
        rtd_outl(DDC_SAP_0x2724, EDID_KEY_TABLE[i]);
    }
    rtd_outl(DDC_SIR_0x2720,0);	//SRAM Index = 0
    rtd_outl(EDID_CR_0x270c, 0x3);	// External Access Enable(BIT0) and I2C Master SRAM Write Enable(BIT1)
    rtdf_setBits(I2C_CR_0x2700,_BIT4|_BIT5);//add for ddc0 I2C CLOCK _BIT4|_BIT5 clocks debounce
#endif
}

void drv_sig_vga_Init(void)
{
    //[Code Sync][AlanLi][0980403][1]
    //VGA support composite sync
    VGADetecSyncTypeMode = SEPARATE_MODE;
    //[Code Sync][AlanLi][0980403][1][end]
    //rtd_maskl(DOWN_SAMPLE_FIFO_SOURCE_SEL_0x3554,~(_BIT18|_BIT19),0x00);

    drv_sig_vga_edid_init();
    drv_syncproc_InitSrc(VGA_Init);

    drv_adc_SOY0_input_select(GET_INPUTSOURCE_INDEX());

    drv_adc_ADCSelectInput(GET_INPUTSOURCE_INDEX());

    //{tommy restore SVN1083,SOY need toggle this bit after power on
    //tommy disable PLL27X for remove vga noise
    drv_syncproc_PLL27X_Enable(_ENABLE);
    fw_timer_event_DelayXms(10);
    drv_syncproc_PLL27X_Enable(_DISABLE);
    //tommy 091216 restoreSVN1083 }

}

UINT8 drv_sig_vga_DetectMode(StructSourceInput *srcinput_pt)
{
    ADC_INITIAL_OPTIONS  *ptOptions;
    //RTD_Log(LOGGER_INFO, "drv_sig_vga_DetectMode\n");

    ptOptions =  srcinput_pt->InitOptions;

    drv_syncproc_VGA_sync_input();


    return drv_syncproc_DetectMode(srcinput_pt);
}

bit drv_sig_vga_CheckMode(void)
{
    Audio_HwpInputNoiseReduction();
    if (flow_framesync_status())
        return _FALSE;
    if (flow_linebuffer_status())
        return _FALSE;
    return flow_mode_CheckSignalStable();
}

void drv_sig_vga_SetupMode(void)
{
    RTD_Log(LOGGER_INFO, "drv_sig_vga_SetupMode\n");

    drv_syncproc_inprocvga_setup();
    drv_vgip_set_vgip(VGIP_SRC_ADC, VGIP_MODE_ANALOG);
    rtdf_outl(VGIP_CHN1_MISC_0x0a24, (UINT32)0x80<<8); //Video_Processor_H_Backporch:0x80A
    drv_vgip_set_sample(_ENABLE);//for DI bug when do HSD
    //vga_decide_main_timing_mode();
    //vga_decide_data_format();
    // change clamp position for reduce blanking timing
    if ((info->ucMode_Curr == _MODE_1920x1080_60Hz_138) || (info->ucMode_Curr == _MODE_1920x1080_60Hz_148)
            || (info->ucMode_Curr == _MODE_1680x1050_60HZ_RB) || (info->ucMode_Curr == _MODE_1440x900_60RHZ)
            || (info->ucMode_Curr == _MODE_1920x1200_60Hz))
    {
        drv_syncproc_SetClampPulsePos(0x02, 0x5, CLAMP_Y);
        drv_syncproc_SetClampPulsePos(0x02, 0x5, CLAMP_NY);
    }
    drv_adc_setadcgainoffset();
    drvif_abl_setting_on();
}

void drv_sig_vga_Release(void)
{
    RTD_Log(LOGGER_INFO, "\n drv_sig_vga_Release\n");
    drv_syncproc_FunctionPowerDown();
    drvif_abl_disable();
    drv_syncproc_PLL27X_Enable(_DISABLE);
}

/*============================================================================*/
