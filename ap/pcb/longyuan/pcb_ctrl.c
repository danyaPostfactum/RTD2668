/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2006
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is for scaler related functions.
 *
 * @author 	$Author$
 * @date		$Date$
 * @version	$Revision$
 * @ingroup	scaler
 */

/**
 * @addtogroup scaler
 * @{
 */

/*============================ Module dependency  ===========================*/
#include "sparrow.h"
#include "ui_ctrl.h"
#include "power.h"
#include "pcb\pcb_utility.h"
#include "pcb_customer.h"
#include "audio/audio_api.h"
#include "ui_struct.h"
#include "ui_ctrl.h"
#include "ap_rtirkey.h"
//--------------------------------------------------
// Description  : Set panel power off
// Input Value  : None
// Output Value : None
//--------------------------------------------------
void pcb_ctrl_PowerOn(void)
{
//   PANELPOWER_DOWN();
	POWER_ON_CTL(1);

}

void pcb_ctrl_PowerOff(void)
{
//   PANELPOWER_DOWN();
	POWER_ON_CTL(0);

}

UINT32 PCB_GetIrdaCustomeCode()
{
    return CUSTOM_CODE;
}

#if 0
/**
 * appcb_pin_share_init
 * Intial all pin share in Dragon
 *
 * @param <void> { none }
 * @return { none }
 *
 */
void __init appcb_pin_share_init(void)
{
    UINT32 PinSetting = 0;

    PinSetting = ((_DRAGON_PIN_1)|(_DRAGON_PIN_2<<4)|
                  (_DRAGON_PIN_6<<8)|(_DRAGON_PIN_7<<12));

    rtdf_maskl(PAD_PA_CFG0_VADDR, PAD_PA_CFG0_MASK, PinSetting);

    PinSetting = 0;
    PinSetting = ((_DRAGON_PIN_215)|(_DRAGON_PIN_5<<28));

    rtdf_maskl(PAD_PB_CFG0_VADDR, PAD_PB_CFG0_MASK, PinSetting);


    PinSetting = 0;
    PinSetting = ((_DRAGON_PIN_216)|(_DRAGON_PIN_194<<28));

    rtdf_maskl(PAD_PC_CFG0_VADDR, PAD_PC_CFG0_MASK, PinSetting);

    PinSetting = 0;
    PinSetting = ((_DRAGON_PIN_37)|(_DRAGON_PIN_37<<4)|
                  (_DRAGON_PIN_94<<16)|(_DRAGON_PIN_95<<20)|(_DRAGON_PIN_111<<28));

    rtdf_maskl(PAD_PD_CFG0_VADDR, PAD_PD_CFG0_MASK, PinSetting);

    PinSetting = 0;
    PinSetting = ((_DRAGON_PIN_108)|(_DRAGON_PIN_107<<4)|
                  (_DRAGON_PIN_106<<8)|(_DRAGON_PIN_105<<12)|
                  (_DRAGON_PIN_104<<16)|(_DRAGON_PIN_103<<20)|
                  (_DRAGON_PIN_102<<24)|(_DRAGON_PIN_101<<28));

    rtdf_maskl(PAD_PE_CFG0_VADDR, PAD_PE_CFG0_MASK, PinSetting);


    PinSetting = 0;
    PinSetting = ((_DRAGON_PIN_118)|(_DRAGON_PIN_117<<4)|
                  (_DRAGON_PIN_116<<8)|(_DRAGON_PIN_115<<12)|
                  (_DRAGON_PIN_114<<16)|(_DRAGON_PIN_113<<20));

    rtdf_maskl(PAD_PG_CFG0_VADDR, PAD_PG_CFG0_MASK, PinSetting);

    PinSetting = 0;
    PinSetting = ((_DRAGON_PIN_122<<16)|(_DRAGON_PIN_121<<20)|
                  (_DRAGON_PIN_120<<24)|(_DRAGON_PIN_119<<28));

    rtdf_maskl(PAD_PH_CFG0_VADDR, PAD_PH_CFG0_MASK, PinSetting);

    PinSetting = 0;
    PinSetting = ((_DRAGON_PIN_132)|(_DRAGON_PIN_131<<4)|
                  (_DRAGON_PIN_130<<8)|(_DRAGON_PIN_129<<12)|
                  (_DRAGON_PIN_128<<16)|(_DRAGON_PIN_127<<20)|
                  (_DRAGON_PIN_126<<24)|(_DRAGON_PIN_125<<28));

    rtdf_maskl(PAD_PI_CFG0_VADDR, PAD_PI_CFG0_MASK, PinSetting);

    PinSetting = 0;
    PinSetting = ((_DRAGON_PIN_134<<24)|(_DRAGON_PIN_133<<28));

    rtdf_maskl(PAD_PJ_CFG0_VADDR, PAD_PJ_CFG0_MASK, PinSetting);

    PinSetting = 0;
    PinSetting = ((_DRAGON_PIN_99)|(_DRAGON_PIN_98<<4)|
                  (_DRAGON_PIN_97<<8)|(_DRAGON_PIN_96<<12)|(_DRAGON_PIN_91<<28));

    rtdf_maskl(PAD_PK_CFG0_VADDR, PAD_PK_CFG0_MASK, PinSetting);
#ifndef CONFIG_USE_HWICE
    //GPIOB[7] as GPIO output
    rtdf_maskl(PERI_GPIO_BCR_VADDR,~(_BIT31),(_BIT31));
    //GPIOB[7] as PWM func & PWM0
    rtdf_maskl(PERI_PCS_VADDR, 0xFFFFFFFF,0x00000008);

    //GPIOD[7] as GPIO output
    rtdf_maskl(PERI_GPIO_DCR_VADDR,~(_BIT31),(_BIT31));
    //GPIOC[7] as GPIO
    rtdf_maskl(PERI_PCS_VADDR, 0xFFFFFFFF,0x00000008);

    //GPIOD[7, 4:5] as GPIO output
    rtdf_maskl(PERI_GPIO_DCR_VADDR,~(_BIT31|_BIT29|_BIT28),(_BIT31|_BIT29|_BIT28));
    //GPIOD[7] as GPIO, GPIOD[4:5] as PWM func & PWM1&2
    rtdf_maskl(PERI_PDS_VADDR, 0xFFFF00F0,0x00001208);

    //GPIOE[0] as GPIO output pin(Pin direction config)
    rtdf_maskl(PERI_GPIO_ECR_VADDR,~_BIT24,_BIT24);
    //GPIOE[0] as bit mask for being set
    rtdf_maskl(PERI_GPIO_EDATA_VADDR,~_BIT0,_BIT0);
    //GPIOE[0, 2:3] as GPIO
    rtdf_maskl(PERI_PES_VADDR, 0x0F00FFFF,0x8F88FFFF);

    //PWM1&2 output clock divisor
    rtdf_maskl(PERI_PWM_B1_VADDR,0xFF0000FF,0x00101000);
    //PWM1&2 clock source divisor
    rtdf_maskl(PERI_PWM_ER_VADDR,0xFFFFF00F,0x00000550);

#endif


    //GpioExtern_Initial();
}



/**
 * appcb_dev_power_on
 * Power on devices on PCB
 *
 * @param <void> { none }
 * @return { none }
 *
 */
void __init appcb_dev_power_on(void)
{
    //TUNER_POWER_ENABLE(TUNERPOWER_ENABLE);
//-->	Rspi_CodeW(initialtable);
//ROSPrintf("HDMI HP OFF...................\n");
    drvif_Hdmi_HpdOff(_SOURCE_INDEX_HDMI);
    drvif_Hdmi_HpdOff(_SOURCE_INDEX_HDMI1);
}

/**
 * appcb_dev_power_off
 * Power off devices on PCB
 *
 * @param <void> { none }
 * @return { none }
 *
 */
void appcb_dev_power_off(void)
{

    //TUNER_POWER_ENABLE(TUNERPOWER_DISABLE);
}
//#endif

void appcb_set_audio_path(UINT8 sel)
{
    UINT8 soundsystem;
//       RTD_Log(LOGGER_INFO,"\nappcb_set_audio_path=%x", sel);
    switch (sel)
    {
    case _SOURCE_INDEX_VGA://VGA mode
        Audio_AioSetIptSrc_BBADC(AIO_BBADC_AIN1);
        Audio_AioSetPathSrc(&audio_path_cfg_bbadc);
        break;
    case _SOURCE_INDEX_AV:
//		case _SOURCE_INDEX_AV1:
//		case _SOURCE_INDEX_AV2:
//		case _SOURCE_INDEX_AV3:
        Audio_AioSetIptSrc_BBADC(AIO_BBADC_AIN3);
        Audio_AioSetPathSrc(&audio_path_cfg_bbadc);
        break;
    case _SOURCE_INDEX_SV:
//		case _SOURCE_INDEX_SV1:
        Audio_AioSetIptSrc_SPDIF(AIO_BBADC_AIN3);
        Audio_AioSetPathSrc(&audio_path_cfg_bbadc);
        break;
    case _SOURCE_INDEX_TV:
        Audio_AioSetPathSrc(&audio_path_cfg_atv);
//			soundsystem=GET_SOUND_SYSTEM();
        RTD_Log(LOGGER_DEBUG, "Set tv audio stand %x\n",soundsystem);
//			fwif_audio_setsoundmode(soundsystem);
        break;
    case _SOURCE_INDEX_YPbPr:
        Audio_AioSetIptSrc_BBADC(AIO_BBADC_AIN2);
        Audio_AioSetPathSrc(&audio_path_cfg_bbadc);
        break;
//		case _SOURCE_INDEX_HDMI:
    case _SOURCE_INDEX_HDMI1:
        //only set at HDMI driver//drvif_audio_set_source(ADC_SRC_HDMI_I2S);
        Audio_AioSetIptSrc_SPDIF(AIO_SPDIFI_HDMI);
        Audio_AioSetPathSrc(&audio_path_cfg_spdif);
        Audio_AioSetIptSrc_I2SPri(AIO_I2SI_HDMI);
        Audio_AioSetPathSrc(&audio_path_cfg_i2s);
        break;
    default:
        break;
    };
}

//#if 0
void appcb_sev_ddc2uart(void)
{
#ifdef CONFIG_UART2_ENABLE
    Change_PinShare_ToUart1();
#else
    UINT32 PinSetting = 0;

    PinSetting = ((_DRAGON_PIN_1)|(_DRAGON_PIN_2<<4)|
                  (_DRAGON_PIN_6<<8)|(_DRAGON_PIN_7<<12));
    rtdf_maskl(PAD_PA_CFG0_VADDR, PAD_PA_CFG0_MASK, PinSetting);//change UART0 pin share
#endif
}

void appcb_set_input_src_mux(UINT8 sel)
{
    switch (sel)
    {
    case _SOURCE_INDEX_YPbPr:
        break;
#ifdef CONFIG_PVR_ENABLE
    case _SOURCE_INDEX_PVR:
        //AV_PVR_SEL(PVR_SEL);
        break;
#endif // CONFIG_PVR_ENABLE
    case _SOURCE_INDEX_HDMI:
        rtdf_maskl(HDMI_TMDS_ACC2_ADDR, ~(_BIT1|_BIT0), _BIT0);//HDMI channel0 Power ON //0114 kist for malata dvd player
        rtdf_maskl(PAD_PA_CFG0_VADDR, PAD_PA_CFG0_MASK, 0x0);//change DDC pin share
        rtdf_maskl(HDMI_HDCP_PCR_ADDR, ~_BIT1, _BIT1);//HDCP DDC0
        rtdf_maskl(HDMI_TMDS_MRR0_ADDR, ~_BIT1, 0x0);//HDMI channel0
//			drvif_Hdmi_HpdOff(_SOURCE_INDEX_HDMI1);//HDMI HPD1 off
        drvif_Hdmi_HpdOn(_SOURCE_INDEX_HDMI);//HDMI HPD0 on
        break;
    case _SOURCE_INDEX_HDMI1:
        rtdf_maskl(HDMI_TMDS_ACC2_ADDR, ~(_BIT1|_BIT0), _BIT1);//HDMI channel1 Power ON //0114 kist for malata dvd player
        rtdf_maskl(HDMI_HDCP_PCR_ADDR, ~_BIT1, 0);//HDCP DDC1
        rtdf_maskl(HDMI_TMDS_MRR0_ADDR, ~_BIT1, _BIT1);//HDMI channel1
//			drvif_Hdmi_HpdOff(_SOURCE_INDEX_HDMI);//HDMI HPD0 off
        drvif_Hdmi_HpdOn(_SOURCE_INDEX_HDMI1);//HDMI HPD1 on
        break;
    case _SOURCE_INDEX_AV:
        break;
    };

    bShowNoSupport = _FALSE; // Initialize flag
}
// The key value
#endif
// Key Pad
/*
#define _MENU_KEY_THRESHOLD			0xc1
#define _SOURCE_KEY_THRESHOLD		0x43

#define _DOWN_KEY_THRESHOLD			0x83
#define _UP_KEY_THRESHOLD         		0x43
#define _RIGHT_KEY_THRESHOLD         	0x8c
#define _LEFT_KEY_THRESHOLD          		0xc

#define _COMPOSE1_KEY_THRESHOLD		0xe6// Right + Left
#define _COMPOSE2_KEY_THRESHOLD		0xeb// Menu + Down
#define _AUDIO_IN_THRESHOLD			0xe0
#define _AUDIO_HEAD_IN_THRESHOLD		0xFD
#define _AUDIO_HEAD_OUT_THRESHOLD	0x30
#define _KEY_THRESHOLD					0xA
#define _KEYPRESSED_THRESHOLD			0xB
*/
//--------------------------------------------------
// Description  : Set backlight power on
// Input Value  : None
// Output Value : None
//--------------------------------------------------
void pcb_ctrl_LightPowerOn(void)
{
    BACKLIGHT_POWER_ENABLE(0);
    SET_LIGHTPOWERSTATUS();
}


//--------------------------------------------------
// Description  : Set backlight power off
// Input Value  : None
// Output Value : None
//--------------------------------------------------
void pcb_ctrl_LightPowerOff(void)
{
    BACKLIGHT_POWER_ENABLE(1);
    CLR_LIGHTPOWERSTATUS();
}

//--------------------------------------------------
// Description  : Set panel power on
// Input Value  : None
// Output Value : None
//--------------------------------------------------
void pcb_ctrl_PanelPowerOn(void)
{
    PANEL_POWER_ENABLE(_ON);
    SET_PANELPOWERSTATUS();
}

//--------------------------------------------------
// Description  : Set panel power off
// Input Value  : None
// Output Value : None
//--------------------------------------------------
void pcb_ctrl_PanelPowerOff(void)
{
    PANEL_POWER_ENABLE(_OFF);
    CLR_PANELPOWERSTATUS();
}

/*void pcb_ctrl_TunerPoweron(void)
{
    TUNER_POWER_ENABLE(_ON);
}
void pcb_ctrl_TunerPowerOff(void)
{
    TUNER_POWER_ENABLE(_OFF);
}
*/

#if 0
/*============================================================================*/
/**
 * appcb_power_off_detect_key
 * Detect power on keypad status according customer's PCB.
 *
 * @param <void>
 * @return 			{ TRUE:have detect power on key,FALSE:no detect}
 *
 */
UINT8 __sleepcode appcb_power_off_detect_key(void)
{
    //Detect keypad power key
    if (!((rtd_inl(PERI_GPIO_EDATA_VADDR)& (_BIT1 <<24)) >>24))
        return _TRUE;
    else
        return _FALSE;
}


/*============================================================================*/
/**
 * appcb_power_off_detect_ypbpr
 * Detect signal on YPbPr sync type.
 *
 * @param <void>
 * @return 			{ TRUE:have detect YPbPr sync type,FALSE:no YPbPr sync type detected}
 *
 */

UINT8 __sleepcode appcb_power_off_detect_ypbpr(void)
{
    /*
    UINT8 register ucCounter;
    	UINT8 ucResult;
    	UINT8 register YPBPR_SYNC,HSYNC_ONLY;
    	UINT8 register SERRATION_COMPOSITE_SYNC;
    	UINT8 register XOR_OR_COMPOSITE_WITH_EQUALIZER;
    	UINT8 register XOR_OR_COMPOSITE_WITHOUT_EQUALIZER;
    	UINT32 register usTimeout;

    	YPBPR_SYNC = 2;
    	HSYNC_ONLY = 7;



    	// CSW+ 0960713 to solve the error in VGA/PbPr plug in/out
    	// Turn off sync processor auto-measure first
    	rtdf_clearBits(0 ? IDOMAIN_SYNCPROC_REG18051080_VADDR: IDOMAIN_SYNCPROC_REG18051000_VADDR, _BIT10);//turn-off Automatic
    	usTimeout = 3000000;
    	while(usTimeout--);
    	rtd_outl(0 ? IDOMAIN_SYNCPROC_REG18051084_VADDR : IDOMAIN_SYNCPROC_REG18051004_VADDR, 0);//clear all
    	rtdf_setBits(0 ? IDOMAIN_SYNCPROC_REG18051080_VADDR: IDOMAIN_SYNCPROC_REG18051000_VADDR, _BIT10);//set Automatic
    	usTimeout = 28000000;
    	while(usTimeout--);

    //<Stanely K80902> modify for vga sog process
    //#ifdef CONFIG_VGA_SUPPORT_SOG
    //#else
    	// CSW+ 0970327 To resolve VGA timing(640*480 60Hz & 720*400 70 Hz) shaking in the booting up of PC
    //	if(GET_INPUTSOURCE_TYPE() == _SRC_VGA){
    		//frank@0507 openauto run will turn on coast,turn-off coast will let VGA source phase change good
    //		rtdf_clearBits(0 ? IDOMAIN_SYNCPROC_REG18051080_VADDR: IDOMAIN_SYNCPROC_REG18051000_VADDR, _BIT9);
    //	}
    //#endif

    	ucCounter= 25; //500 ms from 100ms read protection,To prevent pattern gen for VGA timing tends from output wrong timing
    	ucResult = 0;

    	 do{
    		ucResult = rtdf_readByte3(0 ? IDOMAIN_SYNCPROC_REG18051084_VADDR : IDOMAIN_SYNCPROC_REG18051004_VADDR)& 0x80; //check whether auto run result is ready
    	 	//ScalerTimer_DelayXms(20);//wait a frame time
    	 	usTimeout = 56000000;
    		while(usTimeout--);
    	}while((ucResult != 0x80) && (--ucCounter));

    	 if(ucCounter != 0){
    		ucResult = (rtdf_readByte0(0 ? IDOMAIN_SYNCPROC_REG18051084_VADDR : IDOMAIN_SYNCPROC_REG18051004_VADDR) & (_BIT4| _BIT5 | _BIT6)) >>4; //get the measure result
    	 }
    	 else //measure timeout! treat as no signal
    	     ucResult = 0;



    	RTD_Log(LOGGER_INFO,"Test sync type = %x \n",ucResult);
    	if(ucCounter != 0){
    		RTD_Log(LOGGER_INFO,"Test sync type = %x !!!!!!!!!!!!!!!!!!!!!!!!!!!\n",ucResult);
    		ucResult = (rtdf_readByte0(0 ? IDOMAIN_SYNCPROC_REG18051084_VADDR : IDOMAIN_SYNCPROC_REG18051004_VADDR) & (_BIT4| _BIT5 | _BIT6)) >>4; //get the measure result
    		if ((ucResult < YPBPR_SYNC) ||(ucResult > HSYNC_ONLY))
    			return  _FALSE;
    	}
    	else //measure timeout! treat as no signal
    		return _FALSE;

    	return _TRUE;
    */
}


/*============================================================================*/
/**
 * appcb_power_off_detect_vga
 * Detect signal on VGA H/V sync.
 *
 * @param <void>
 * @return 			{ TRUE:have detect VGA signal,FALSE:no VGA signal detected}
 *
 */

UINT8 __sleepcode appcb_power_off_detect_vga(void)
{
    UINT32 register VSyncStatus=0,HSyncStatus=0;

    // Used AVsync to detect VGA wake up
    VSyncStatus = (rtd_inl(PERI_GPIO_DCR_VADDR) & _BIT9);
    HSyncStatus = (rtd_inl(PERI_GPIO_DCR_VADDR) & _BIT8);


    if (VSyncStatus)	// only Source is VGA is to check VSync happens wake up
    {
        // Used AVsync to detect VGA wake up
        if (rtd_inl(PERI_GPIO_DCR_VADDR) & _BIT9)
        {
            rtdf_maskl(PERI_GPIO_DCR_VADDR,0xffffffff,0xff00);	// clear the interrupt status
            power_off_backup.ucTemp2++;
        }
    }

    if (HSyncStatus)	// only Source is VGA is to check HSync happens wake up
    {
        if (rtd_inl(PERI_GPIO_DCR_VADDR) & _BIT8)
        {
            rtdf_maskl(PERI_GPIO_DCR_VADDR,0xffffffff,0xff00);	// clear the interrupt status
            power_off_backup.ucTemp3++;
        }
    }

    if ((power_off_backup.ucTemp2 > 30)&&VSyncStatus) // only Source is VGA & VSync happens more than 30times
    {
        VSyncStatus = 1;
    }
    else
        VSyncStatus = 0;

    if ((power_off_backup.ucTemp3 > 60)&&HSyncStatus) // only Source is VGA & HSync happens more than 100times
    {
        HSyncStatus = 1;
    }
    else
        HSyncStatus = 0;



    if (VSyncStatus && HSyncStatus)
    {
        rtd_outl(MDOMAIN_CAP_DDR_IN2LINESTEP_VADDR,0x00);			// clear flag for power saving for VGA
        return _TRUE;
    }
    else
        return _FALSE;


}



/*============================================================================*/
/**
 * appcb_power_off_detect_hdmi
 * Detect signal on hdmi input clock.
 *
 * @param <void>
 * @return 			{ TRUE:have detect hdmi input clock,FALSE:no hdmi input clock}
 *
 */

UINT8 __sleepcode appcb_power_off_detect_hdmi(void)
{
    UINT16 register usTimeout;
    UINT16 register fs;

    rtdf_maskl(HDMI_TMDS_Z0CC2_ADDR ,~_BIT6,_BIT6);

    //_DE_ONLY_OFF
    rtdf_outl(HDMI_TMDS_PWDCTL_ADDR, rtdf_inl(HDMI_TMDS_PWDCTL_ADDR)&(~_BIT7));

    if  (rtdf_readWord1(SYS_SC_VERID_VADDR) == 0x2671)
    {
        //RTD_Log(LOGGER_DEBUG,"_DRAGON_IC_DRAGON_P!!!!!!!!!!\n");
        rtdf_maskl(HDMI_HDMI_AOCR_ADDR,~(_BIT8|_BIT9|_BIT11|_BIT12),0);
        rtdf_maskl(HDMI_TMDS_UDC0_ADDR,~_BIT6, _BIT6);
    }
    else
        //RTD_Log(LOGGER_DEBUG,"_DRAGON_IC_DRAGON!!!!!!!!!!\n");
        ;//<Fred 20080925 dis>

    //for fs detect//
    //ScalerTimer_DelayXms(10);         // 10ms
    usTimeout = 0x02ff;
    while (usTimeout--);

    fs=rtdf_inl(HDMI_TMDS_UDC1_ADDR) & 0xf0;

    if ((fs  == 0x80) || (fs == 0x60)) //50-80,80-112MHz
    {
        power_off_backup.ucTemp++;
    }
    else if ((fs == 0x40) || (fs  == 0x30)) //112-140,140-165MHz
    {
        power_off_backup.ucTemp++;
    }
    else if (fs == 0xe0) //25-50MHz
    {
        power_off_backup.ucTemp++;
    }
    else if (fs == 0x10) //<25M,>165MHz
    {
        if  (rtdf_readWord1(SYS_SC_VERID_VADDR) == 0x2671)
            power_off_backup.ucTemp++;
        else
            return _FALSE;
    }
    else// 0Hz
        return _FALSE;

    //if (power_off_backup.ucTemp > 10)
    return _TRUE;
}



/*============================================================================*/
/**
 * appcb_power_off_detect_source
 * Detect signal to wakeup.
 *
 * @param <void>
 * @return 			{ TRUE:have detect power on key,FALSE:no detect}
 *
 */

UINT8 __sleepcode appcb_power_off_detect_source(void)
{
    UINT32 register status;
    //UINT32 register  ucSource = 0;


    //RTD_Log(LOGGER_INFO,"VGA/YPbPr source detect~~  \n");

    status = rtd_inl(MDOMAIN_CAP_DDR_IN2LINESTEP_VADDR);
    if (status == 0x2580) // VGA power saving mode
    {
        //ucSource = power_off_backup.InputSource; // read current source
        status = appcb_power_off_detect_vga();
    }
    else if (status == 0x2581) // HDMI power saving mode
    {
        status = appcb_power_off_detect_hdmi();
    }
    else
    {
        status = _FALSE;

    }





    return status;
}

/*============================================================================*/

void __sleepcode appcb_ddr_power_ctrl(UINT8 on_off)
{

    UINT32 volatile register status = 28000000;	// 28 Mega
    if (on_off == _OFF)
    {
        // power-off system power & ddr power
        rtdf_maskl(PAD_PE_CFG0_VADDR/* 8007120 */, 0xfffffff8, _BIT0); // set pin share GPIOE[0]
        rtdf_maskl(PERI_PES_VADDR/* 80c5074 */, 0x0fffffff, _BIT31);	// set  GPIOE[0] GPIO
        rtdf_maskl(PERI_GPIO_ECR_VADDR/* 80c5020 */, ~_BIT24, _BIT24);	// set GPIOE[0] output
        rtdf_maskl(PERI_GPIO_EDATA_VADDR/* 80c5024 */,~(_BIT16|_BIT0), (_BIT16|_BIT0));	// set GPIOE[0] = 1 to power down DDR power module

        //<Stanley K81103> Mark for hannstar shut down speed.
        /*
        status = 28000000;
        while(status--);//delay for HW power stable
        */

        //<Stanley K81024>Add for power off led.
        rtdf_maskl(PERI_GPIO_EDATA_VADDR/* 80c5024 */,~(_BIT20), (_BIT20));	// set GPIOE[4] = 1 to power down LED power module
        // Used AV sync to detect VGA wake up
        rtdf_maskl(SYSTEM_PD_CFG0_VADDR, 0xffffff8f, 0x10);	// set Dragon 38 pin share to GPIOD(1)[bit 4~6]
        rtdf_maskl(PERI_PDS_VADDR,0xf0ffffff,_BIT27);	// SET GPIOD(1) Group is GPIO[bit 27]
        rtdf_maskl(PERI_GPIO_DCR_VADDR,0xfdfffffd,0xff00);	// SET GPIOD(1) is input[bit 25],disable GPIOD(1) de-bounce,clear the interrupt status
        rtdf_maskl(PERI_GPIO_DDATA_VADDR,0xfffffffd,0x00);		// disable GPIOD(1) mask bits[bit 1]
        rtdf_maskl(PERI_GPIO_DIMR_VADDR,0xfffffff3,0x0c);		// eable GPIOD(1) interrupt and set falling and rising edge trigger

        // Used AH sync to detect VGA wake up
        rtdf_maskl(SYSTEM_PD_CFG0_VADDR, 0xfffffff8, 0x01);	// set Dragon 37 pin share to GPIOD(0)[bit 0~2]
        rtdf_maskl(PERI_PDS_VADDR,0x0fffffff,_BIT31);	// SET GPIOD(0) Group is GPIO[bit 31]
        rtdf_maskl(PERI_GPIO_DCR_VADDR,0xfefffffe,0xff00);	// SET GPIOD(0) is input[bit 24],disable GPIOD(0) de-bounce,clear the interrupt status
        rtdf_maskl(PERI_GPIO_DDATA_VADDR,0xfffffffe,0x00);		// disable GPIOD(0) mask bits[bit 0]
        rtdf_maskl(PERI_GPIO_DIMR_VADDR,0xfffffffc,0x03);		// eable GPIOD(0) interrupt and set falling and rising edge trigger

        // Set compare level
        /*		if (YPBPR_SOY_PATH_SELECT)
        			rtdf_outl(IDOMAIN_SOYCH1_A0_VADDR , 0x0000001a); // ADC/YPbPr , SOYCH1, compare level
        		else
        			rtdf_outl(IDOMAIN_SOYCH0_A1_VADDR, 0x001a0000); // ADC/YPbPr , SOYCH0, compare level

        		//YPbPr_DCRestore_Ctrl(YPBPR_SOY_PATH_SELECT, _DISABLE, srcinput_pt->InitOptions);
        		rtdf_maskl(IDOMAIN_YPBPRADC_ADC_DCRESTORE_CTRL_VADDR, 0x0000000f, 0x00000004);

        		//YPbPr_Clamp_Ctrl(_MAIN_DISPLAY, YPBPR_SOY_PATH_SELECT, _ENABLE, srcinput_pt->InitOptions);
        		rtdf_maskl(IDOMAIN_YPBPRADC_ADC_CLAMP_CTRL_VADDR, 0xffffff00, 0);
        */


    }
    else
    {

        // set GPIOE[0](b80c5024 bit16 = 0)  to power on DDR & led power module
        rtdf_maskl(PERI_GPIO_EDATA_VADDR,~(_BIT20|_BIT16|_BIT0), (_BIT0));
        //status = 28000000;
        status = 0xa000;//<Stanley K81103>Modified for hannstar boot up speed.
        while (status--);//delay for HW power stable
    }
}

void appcb_set_hdmi0_hot_plug(int x)
{
    if (x==HDMI0_HOT_ENABLE)
        rtdf_outl(HDMI_CEC_CR3_ADDR , 0x1f);//<Stanley K90911> Set HPD to high
    else
        rtdf_outl(HDMI_CEC_CR3_ADDR , 0);//<Stanley K90911> Set HPD to low
}

void appcb_set_hdmi1_hot_plug(int x)
{
    if (x==HDMI1_HOT_ENABLE)
        rtdf_outl(HDMI_CEC_CR3_ADDR , 0x1f);//<Stanley K90911> Set HPD to high
    else
        rtdf_outl(HDMI_CEC_CR3_ADDR , 0);//<Stanley K90911> Set HPD to low
}

void appcb_backlight_en(UINT8 x)
{
    if (x==BACKLIGHT_ENABLE)
        clr_gpio_pin(PERI_GPIO_ECR_VADDR, _BIT2);
    else
        set_gpio_pin(PERI_GPIO_ECR_VADDR,  _BIT2);
}

void appcb_panel_power_onoff(UINT8 on_off)
{
    if (on_off==PANELPOWER_ENABLE)
        set_gpio_pin(PERI_GPIO_CCR_VADDR,  _BIT7);
    else
        clr_gpio_pin(PERI_GPIO_CCR_VADDR, _BIT7);
}

void appcb_ado_amp_mute_onoff(UINT8 x)
{
    if (x==AUDIO_AMP_MUTE)
        set_gpio_pin(PERI_GPIO_DCR_VADDR,  _BIT7);
    else
        clr_gpio_pin(PERI_GPIO_DCR_VADDR, _BIT7);
}

void appcb_ado_amp_stb_onoff(UINT8 x)
{
    if (x==AUDIO_AMP_STB_ENABLE)
        set_gpio_pin(PERI_GPIO_ECR_VADDR,  _BIT3);
    else
        clr_gpio_pin(PERI_GPIO_ECR_VADDR, _BIT3);
}

void appcb_led_b1_ctrl(UINT8 level)
{
    UINT8 pwm=level;

    //PWM1 clock duty
    rtdf_maskl(PERI_PWM_AD1_VADDR,0xFFFF00FF,(pwm<<8));
}

void appcb_led_b2_ctrl(UINT8 level)
{
    UINT8 pwm=level;

    //PWM2 clock duty
    rtdf_maskl(PERI_PWM_AD1_VADDR,0xFF00FFFF,(pwm<<16));
}

void appcb_led_ctrl(UINT8 x)
{
    if (x==LED_ON)
        clr_gpio_pin(PERI_GPIO_ECR_VADDR, _BIT4);
    else
        set_gpio_pin(PERI_GPIO_ECR_VADDR,  _BIT4);
}

#endif
