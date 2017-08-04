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

#include "panel\panel_library.h"
#include "panel\panel_struct.h"
#include "panel\panel.h"
#include "panel\panelapi.h"
#include "scaler\scaler.h"
#include "power.h"
#include "sparrow\rtdctrl.h"
#include "timer_event.h"
#include "reg_def.h"
#include "pcb_sys.h"

//----------------------------------------------------------------------------------------------------
// Display Interface Initial
//----------------------------------------------------------------------------------------------------
#if (CONFIG_DISPLAY_PANEL_STYLE == _PANEL_INTERFACE_TTL)
UINT8 code tTTL_INITIAL[] =
{
    5,  _AUTO_INC,       _P0_TCON_ADDR_PORT_8B,             _TCON_CTRL0_00,0x00,

    4,  _NON_INC,   DISPLAY_TIMING_CTRL1_0x2000,       0x00,

    _TBL_END
};

#endif // End of #if(_PANEL_STYLE == _PANEL_TTL)


#if (CONFIG_DISPLAY_PANEL_STYLE == _PANEL_INTERFACE_LVDS)

UINT8 code tLVDS_INITIAL[] =
{
    5,  _AUTO_INC,       _P0_TCON_ADDR_PORT_8B,             _TCON_CTRL0_00,0x01,
    4,  _NON_INC,   _P0_TCON_ADDR_PORT_8B,             _TCON_LVDS_CTRL0_A0,

//cyyeh 20080331
#if(CONFIG_DISPLAY_BITMAPPING_TABLE == 0)//cyyeh 20080225
//    9,  _NON_INC,   _TCON_DATA_PORT_8C,             0x40,0x13,0x83,0x1c,0x80,0x80,
    10,  _NON_INC,   _P0_TCON_DATA_PORT_8C,             0x40,0x16,0x42,0x9c,0x80,0x80,01,
#elif(CONFIG_DISPLAY_BITMAPPING_TABLE == 1)
//    9,  _NON_INC,   _TCON_DATA_PORT_8C,             0x40,0x13,0x83,0x1d,0x80,0x80,
    10,  _NON_INC,   _P0_TCON_DATA_PORT_8C,             0x40,0x16,0x42,0x9d,0x80,0x80,01,
#endif

    _TBL_END
};

#endif // End of #if(_PANEL_STYLE == _PANEL_INTERFACE_LVDS)


#if(CONFIG_DISPLAY_PANEL_STYLE == _PANEL_INTERFACE_RSDS)

UINT8 code tRSDS_INITIAL[] =
{

    // AUO RSDS Panel  Model :M170E805 V.1   50 Pin , 30 Pin
    4,  _NON_INC,   _P0_TCON_ADDR_PORT_8B,             _TCON_LVDS_CTRL1_A1,
    5,  _NON_INC,   _P0_TCON_DATA_PORT_8C,             0x12,0x43,


    5,  _AUTO_INC,       _P0_TCON_ADDR_PORT_8B,             _TCON_CTRL0_00,0x82, // Switch to RSDS
    5,  _AUTO_INC,       _P0_TCON_ADDR_PORT_8B,             _TCON_CTRL1_01,0x00,

    5,  _AUTO_INC,       _P0_TCON_ADDR_PORT_8B,             _TCON_RSDS_INTERLEAVING_H_C1,0x06,
    5,  _AUTO_INC,       _P0_TCON_ADDR_PORT_8B,             _TCON_RSDS_INTERLEAVING_L_C2,0x70,

    //TCON1  FXDIO
    4,  _NON_INC,   _P0_TCON_ADDR_PORT_8B,             _TCON_TCON1_V_STA_LSB_10,
    10, _NON_INC,   _P0_TCON_DATA_PORT_8C,             0x0c,0x40,0x0c,0x90,0x22,0x95,0x80,

    //TCON6    XSTB
    4,  _NON_INC,   _P0_TCON_ADDR_PORT_8B,             _TCON_TCON6_V_STA_LSB_38,
    10, _NON_INC,   _P0_TCON_DATA_PORT_8C,             0x0c,0x40,0x0c,0x35,0x22,0xac,0x80,

    //TCON7    POL
    4,  _NON_INC,   _P0_TCON_ADDR_PORT_8B,             _TCON_TCON7_V_STA_LSB_40,
    10, _NON_INC,   _P0_TCON_DATA_PORT_8C,             0x0c,0x40,0x0d,0x00,0x00,0x02,0x88,

    //TCON4  BXDIO
    4,  _NON_INC,   _P0_TCON_ADDR_PORT_8B,             _TCON_TCON4_V_STA_LSB_28,
    10, _NON_INC,   _P0_TCON_DATA_PORT_8C,             0x0c,0x40,0x0c,0xb4,0x22,0xb9,0x80,

    //TCON5 YCLK
    4,  _NON_INC,   _P0_TCON_ADDR_PORT_8B,             _TCON_TCON5_V_STA_LSB_30,
    10, _NON_INC,   _P0_TCON_DATA_PORT_8C,             0x0c,0x40,0x0c,0xf4,0x21,0x94,0x80,

    //TCON11 YDIO
    4,  _NON_INC,   _P0_TCON_ADDR_PORT_8B,             _TCON_TCON11_V_STA_LSB_60,
    10, _NON_INC,   _P0_TCON_DATA_PORT_8C,             0x0c,0x00,0x0e,0xd0,0x33,0xd7,0x88,

    //TCON13 YOE
    4,  _NON_INC,   _P0_TCON_ADDR_PORT_8B,             _TCON_TCON13_V_STA_LSB_70,
    10, _NON_INC,   _P0_TCON_DATA_PORT_8C,             0x0c,0x40,0x0c,0x94,0x52,0x14,0x80,

    _TBL_END
};

#endif // End of #if(_PANEL_STYLE == _PANEL_RSDS)

/*
void drv_panel_Init(void)
{
	// set Display_Timing_CTRL1 register
	rtd_outl(DISPLAY_TIMING_CTRL1_0x2000, _BIT25 |_BIT24 | _BIT16 | _BIT10 |
		(Get_DISPLAY_COLOR_BITS() << 22) |
		(Get_DISPLAY_PORT() << 17) 	|
		(Get_DISPLAY_PORTAB_SWAP() << 8) |
		(Get_DISPLAY_RED_BLUE_SWAP() << 7) |
		(Get_DISPLAY_MSB_LSB_SWAP() << 6) |
		(Get_DISPLAY_OUTPUT_INVERSE() << 4) |
		(stPanelData.nDClkDelay & 0x07));

	// set DHS_Fine_Tunig_Register register to default value
	rtd_outl(DHS_FINE_TUNIG_REGISTER_0x2004, 0);

	// set DH_Width register to default value
	rtd_outl(DH_WIDTH_0x2008, Get_DISP_HSYNC_WIDTH());

	// set DH_Total_Last_Line_Length register to default value
	rtd_outl(DH_TOTAL_LAST_LINE_LENGTH_0x200c, ((Get_DISP_HORIZONTAL_TOTAL() - 1) << 16) | (Get_DISP_HSYNC_LASTLINE() - 1));

	// set DV_Length register to default value
	rtd_outl(DV_LENGTH_0x2010, Get_DISP_VSYNC_LENGTH() - 1);

	// set  DV_total register to default value
	rtd_outl(DV_TOTAL_0x2014, Get_DISP_VERTICAL_TOTAL() - 1);

	// set DH_DEN_Start_End register to default value
	rtd_outl(DH_DEN_START_END_0x2018, (Get_DISP_DEN_STA_HPOS() << 16) | Get_DISP_DEN_END_HPOS());

	// set DV_DEN_Start_End register to default value
	rtd_outl(DV_DEN_START_END_0x201c, (Get_DISP_DEN_STA_VPOS() << 16) | Get_DISP_DEN_END_VPOS());

	// set FS_IV_DV_Fine_Tuning   register to default value
	rtd_outl(FS_IV_DV_FINE_TUNING_0x2020, 0);
}
*/

//--------------------------------------------------
// Description  : Initial display interface for TTL and LVDS
// Input Value  : None
// Output Value : None
//--------------------------------------------------
void drv_panel_InitDisplayPort(void)
{
    rtdf_pageSelect(_PAGE0);

#if(CONFIG_DISPLAY_PANEL_STYLE == _PANEL_INTERFACE_TTL)

    //rtdf_writeCodeWScalerPort(tTTL_INITIAL);
    RtdCtrl_WriteScalerDataPort(_P0_TCON_ADDR_PORT_8B, _TCON_CTRL0_00, 0);

    // Dclk output enable
    rtdf_setBits(DISPLAY_TIMING_CTRL1_0x2000, _BIT10);

#endif // End of #if(_PANEL_STYLE == _PANEL_TTL)

#if(CONFIG_DISPLAY_PANEL_STYLE == _PANEL_INTERFACE_LVDS)

    rtdf_writeCodeWScalerPort(tLVDS_INITIAL);

#endif // End of #if(_PANEL_STYLE == _PANEL_INTERFACE_LVDS)


#if(CONFIG_DISPLAY_PANEL_STYLE == _PANEL_INTERFACE_RSDS)

    rtdf_writeCodeWScalerPort(tLVDS_INITIAL);
    rtdf_writeCodeWScalerPort(tRSDS_INITIAL);
    RtdCtrl_WriteScalerDataPort(_P0_TCON_ADDR_PORT_8B, _TCON_RSDS_OUTPUT_CTRL0_C0, 0x92);
    // Dclk polarity inverted
    rtdf_setBits(DISPLAY_TIMING_CTRL1_0x2000, _BIT11);


#endif // End of #if(_PANEL_STYLE == _PANEL_RSDS)

}


//--------------------------------------------------
// Description  : Power on display port
// Input Value  : None
// Output Value : None
//--------------------------------------------------
void drv_panel_PowerDisplayPortOn(void)
{

#if((CONFIG_DISPLAY_PANEL_STYLE == _PANEL_INTERFACE_TTL) || (CONFIG_DISPLAY_PANEL_STYLE == _PANEL_INTERFACE_LVDS))
    drv_panel_PowerLVDSOn();

#endif

#if(CONFIG_DISPLAY_PANEL_STYLE == _PANEL_INTERFACE_RSDS)
    drv_panel_PowerRSDSOn();

#endif

}

//--------------------------------------------------
// Description  : Power off display port
// Input Value  : None
// Output Value : None
//--------------------------------------------------
void drv_panel_PowerDisplayPortOff(void)
{
#if((CONFIG_DISPLAY_PANEL_STYLE == _PANEL_INTERFACE_TTL) || (CONFIG_DISPLAY_PANEL_STYLE == _PANEL_INTERFACE_LVDS))

    drv_panel_PowerLVDSOff();

#endif // End of #if(CONFIG_DISPLAYCONFIG_DISPLAY_PANEL_STYLE == _PANEL_INTERFACE_TTL)


#if(CONFIG_DISPLAY_PANEL_STYLE == _PANEL_INTERFACE_RSDS)
    drv_panel_PowerRSDSOff();

#endif // End of #if(CONFIG_DISPLAYCONFIG_DISPLAY_PANEL_STYLE == _PANEL_INTERFACE_RSDS)

}




//--------------------------------------------------
// Description  : Power on LVDS
// Input Value  : None
// Output Value : None
//--------------------------------------------------
void drv_panel_PowerLVDSOn(void)
{
    rtdf_pageSelect(_PAGE0);
    if (Get_DISPLAY_PORT())// double port
    {
        RtdCtrl_WriteScalerDataPort(_P0_TCON_ADDR_PORT_8B, _TCON_LVDS_CTRL0_A0, 0xb0);
    }
    else
    {
        //LVDS from even port, if from odd port, the below value should be 0x20
        RtdCtrl_WriteScalerDataPort(_P0_TCON_ADDR_PORT_8B, _TCON_LVDS_CTRL0_A0, 0xb0);
    }

}

//--------------------------------------------------
// Description  : Power off LVDS
// Input Value  : None
// Output Value : None
//--------------------------------------------------
void drv_panel_PowerLVDSOff(void)
{
//cyyeh 20080115
    rtdf_pageSelect(_PAGE0);
    RtdCtrl_WriteScalerDataPort(_P0_TCON_ADDR_PORT_8B, _TCON_LVDS_CTRL0_A0, 0x40);
    RtdCtrl_WriteScalerDataPort(_P0_TCON_ADDR_PORT_8B, 0xa6, 0x00);
}

#if(CONFIG_DISPLAY_PANEL_STYLE == _PANEL_INTERFACE_RSDS)
//--------------------------------------------------
// Description  : Power on RSDS
// Input Value  : None
// Output Value : None
//--------------------------------------------------
void drv_panel_PowerRSDSOn(void)
{
    drv_panel_PowerLVDSOn();
    RtdCtrl_WriteScalerDataPort(_P0_TCON_ADDR_PORT_8B, _TCON_CTRL0_00, 0xc6);//for 8-bit RSDS panel
    fw_timer_event_DelayXms(40);
    RtdCtrl_WriteScalerDataPort(_P0_TCON_ADDR_PORT_8B, _TCON_CTRL0_00, 0x86);
    RtdCtrl_WriteScalerDataPort(_P0_TCON_ADDR_PORT_8B, _TCON_RSDS_PWR_CTL_C3, 0x03);
    //2007_09_05_Edward
    RtdCtrl_WriteScalerDataPort(_P0_TCON_ADDR_PORT_8B, _TCON_LVDS_CTRL0_A0, 0x70);
}

//--------------------------------------------------
// Description  : Power off RSDS
// Input Value  : None
// Output Value : None
//--------------------------------------------------
void drv_panel_PowerRSDSOff(void)
{
    //Issac 2007-01-05
    drv_panel_PowerLVDSOff();
//    CScalerSetBit(_VDISP_CTRL_28, ~_BIT1, _BIT1); // Force HS/VS/DEN/Data to 0
    RtdCtrl_WriteScalerDataPort(_P0_TCON_ADDR_PORT_8B, _TCON_RSDS_PWR_CTL_C3, 0x00); //Even,Odd Port Power Off
    fw_timer_event_DelayXms(40);
    RtdCtrl_WriteScalerDataPort(_P0_TCON_ADDR_PORT_8B, _TCON_CTRL0_00, 0x48); // TTL output 0
    fw_timer_event_DelayXms(40);
    RtdCtrl_WriteScalerDataPort(_P0_TCON_ADDR_PORT_8B, _TCON_CTRL0_00, 0x42); // Back to HZ

//    CScalerSetBit(_VDISP_CTRL_28, ~_BIT1, 0); // Release force
    //2007_09_05_Edward
    RtdCtrl_WriteScalerDataPort(_P0_TCON_ADDR_PORT_8B, _TCON_LVDS_CTRL0_A0, 0x00);
}

#endif // End of #if(CONFIG_DISPLAYCONFIG_DISPLAY_PANEL_STYLE == _PANEL_INTERFACE_RSDS)
//--------------------------------------------------
// Description  : Set panel back light process
// Input Value  : None
// Output Value : None
//--------------------------------------------------
/*void drv_panel_Backlighton(void)
{

    if (GET_LIGHTPOWERSTATUS()==0)
        pcb_ctrl_LightPowerOn();
}*/
//--------------------------------------------------
// Description  : Set panel on process
// Input Value  : None
// Output Value : None
//--------------------------------------------------
void drv_panel_PowerPanelOn(void)
{
    if (GET_PANELPOWERSTATUS() == _OFF)
    {
        // Turn on Panel power
        pcb_ctrl_PanelPowerOn();

        // Delay
        fw_timer_event_DelayXms(Get_PANEL_TO_LVDS_ON_ms());

        // Turn on LVDS/RSDS/TTL...
        drv_panel_PowerDisplayPortOn();

        // CScalerEnableDisplayOutput();
        fw_timer_event_DelayXms(Get_LVDS_TO_LIGHT_ON_ms());
    }
    //RTD_Log(LOGGER_INFO,"power panel on-------------------------------------------");

	fw_timer_event_DelayXms(800);
    // Turn on BackLight
    pcb_ctrl_LightPowerOn();
//    CMiscClearStatusRegister();
}
//--------------------------------------------------
// Description  : Set panel off process
// Input Value  : None
// Output Value : None
//--------------------------------------------------
void drv_panel_PowerPanelOff(void)
{
    pcb_ctrl_LightPowerOff();

#if((CONFIG_DISPLAY_PANEL_STYLE == _PANEL_INTERFACE_TTL) || (CONFIG_DISPLAY_PANEL_STYLE == _PANEL_INTERFACE_LVDS))
    //fw_timer_event_DelayXms(Get_LIGHT_TO_LDVS_OFF_ms());
    //     CScalerDisableDisplayOutput();
    pcb_ctrl_PanelPowerOff();
    //fw_timer_event_DelayXms(Get_LVDS_TO_PANEL_OFF_ms());
    drv_panel_PowerDisplayPortOff();
#endif // End of #if((_PANEL_STYLE == _PANEL_TTL) || (_PANEL_STYLE == _PANEL_INTERFACE_LVDS))

#if(CONFIG_DISPLAY_PANEL_STYLE == _PANEL_INTERFACE_RSDS)
    //--- BACKLIGHT OFF TO DATA OFF TIME ---------------
//    CScalerSetBit(_VDISP_CTRL_28, ~_BIT5, _BIT5);

    fw_timer_event_DelayXms(200);
//    CScalerSetBit(_VDISP_CTRL_28, 0xDF, 0x20);//MUTE ON
    // CScalerDisableDisplayOutput();
    pcb_ctrl_PanelPowerOff();
    drv_panel_PowerDisplayPortOff();
    //--- DATA OFF TO PANEL OFF TIME ---------------
//    CScalerSetBit(_VDISP_CTRL_28, 0xDF, 0x00);//MUTE OFF
#endif
    rtdf_setBits(PLL_DISP_0_0x2f00, _BIT19);

    // The delay below is to prevent from short period between panel OFF and next ON
    //fw_timer_event_DelayXms(Get_PANEL_OFF_TO_ON_ms());
}







