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
#include "scaler_ddomain\scaler_display.h"
#include "panel\panelapi.h"
#include "panel\panel_library.h"
#include "panel\panel.h"
#include "rosprintf.h"
#include "panel\panel_struct.h"
#include "scaler\scaler_pll.h"
#include "scaler\scaler.h"
#include "reg_def.h"
#include "flow_struct.h"
#include "flow_scaler.h"
#include "vip\color.h"
#include "pcb\pcb_utility.h"
#include "pcb_customer.h"
#include "ui_struct.h"

/*================================== Function ===============================*/
/*============================================================================*/
/*============================================================================*/
void drv_scalerdisplay_set_dbuffer_en(BOOL En)
{
    WaitFor_DEN_STOP();
    if (En)
        rtdf_setBits(DOUBLE_BUFFER_CTRL_0x2028, _BIT2 | _BIT0);
    else
        rtdf_setBits(DOUBLE_BUFFER_CTRL_0x2028, _BIT0);
}

/*============================================================================*/
/**
 * scalerdisplay_set_main_act_window
 * This function is used to set the position of Main active window,
 * including the start/end position in both horizontal and vertical direction
 *
 * @param <DispHSta> {start position in horizontal direction}
 * @param <DispHEnd> {end position in horizontal direction}
 * @param <DispVSta> {start position in vertical direction}
 * @param <DispVEnd> {end position in vertical direction}
 * @return { none }
 *
 */
static void scalerdisplay_set_main_act_window(UINT16 DispHSta, UINT16 DispHEnd, UINT16 DispVSta,UINT16 DispVEnd)
{
    UINT32 tmp;
    DispHSta += Get_DISP_ACT_STA_BIOS();
    DispHEnd += Get_DISP_ACT_STA_BIOS();
    // Main Active H pos
    tmp = DispHSta & 0x07ff;
    tmp = tmp <<16 |(DispHEnd & 0x07ff);
    rtdf_outl(MAIN_ACTIVE_H_START_END_0x2120, tmp);

    // Main Active V pos
    tmp = DispVSta & 0x07ff;
    tmp = tmp <<16 |(DispVEnd & 0x07ff);
    rtdf_outl(MAIN_ACTIVE_V_START_END_0x2124, tmp);

    drv_scalerdisplay_set_dbuffer_en(TRUE);
}

/**
 * scalerdisplay_get_main_pos
 * Retrieve main window position.Mostly, it will be at the top-left corner,
 * except 1C12B sub diaply
 *
 * @return { none }
 *
 */
void CreateDisplayTimingTable(void)
{
//	usDisplaytiming[0] = 1;
//	usDisplaytiming[1] = PANEL_INTERFACE_RSDS_CTRL1_VADDR;
//	usDisplaytiming[2] = 0;
    UINT32 tmp;
    tmp = Get_DISPLAY_COLOR_BITS();
    tmp = tmp <<8|Get_DISPLAY_VERTICAL_SYNC_NORMAL();
    tmp = tmp<< 7|Get_DISPLAY_RED_BLUE_SWAP();
    tmp = tmp <<1|Get_DISPLAY_MSB_LSB_SWAP() ;
    tmp = tmp <<2|Get_DISPLAY_OUTPUT_INVERSE();
	
    tmp = tmp <<4| _BIT25 | _BIT24 | _BIT16 | _BIT10 | (Get_DISPLAY_CLOCK_DELAY()  & 0x07);
    RTD_Log(LOGGER_INFO,"panel parameter --------------%x\n",(UINT32)tmp);

    rtdf_outl(DISPLAY_TIMING_CTRL1_0x2000,tmp);

    //rtdf_outl(DISPLAY_TIMING_CTRL1_0x2000,_BIT25 | _BIT24 | _BIT16 | _BIT10 | (Get_DISPLAY_COLOR_BITS() << 22)| (Get_DISPLAY_RED_BLUE_SWAP() << 7) |
    //(Get_DISPLAY_MSB_LSB_SWAP() << 6) | (Get_DISPLAY_OUTPUT_INVERSE() << 4) | (Get_DISPLAY_CLOCK_DELAY()  & 0x07));
    tmp = Get_DISPLAY_PORT();
    tmp = tmp <<2 | Get_DISPLAY_PORT();
    tmp = tmp <<3 | (Get_DISPLAY_PORTAB_SWAP()?0:1);
    tmp = tmp <<3 | (Get_DISPLAY_PORTAB_SWAP()?1:0);
    tmp = tmp <<6;
    rtdf_outl(DHS_FINE_TUNIG_REGISTER_0x2004,tmp);

    //rtdf_outl(DHS_FINE_TUNIG_REGISTER_0x2004,(Get_DISPLAY_PORT() << 14) | (Get_DISPLAY_PORT() << 12) |((Get_DISPLAY_PORTAB_SWAP()?0:1)<<9)|((Get_DISPLAY_PORTAB_SWAP()?1:0)<<6));
    tmp = Get_DISP_HSYNC_WIDTH();
    rtdf_outl(DH_WIDTH_0x2008,tmp);

    tmp = Get_DISP_HORIZONTAL_TOTAL()-1;
    tmp = tmp << 16 | (Get_DISP_HSYNC_LASTLINE()-1);
    rtdf_outl(DH_TOTAL_LAST_LINE_LENGTH_0x200c,tmp);

    //rtdf_outl(DH_TOTAL_LAST_LINE_LENGTH_0x200c,((Get_DISP_HORIZONTAL_TOTAL()) << 16) | (Get_DISP_HSYNC_LASTLINE()));
    tmp = Get_DISP_VSYNC_LENGTH();
    rtdf_outl(DV_LENGTH_0x2010,tmp);
    tmp = Get_DISP_VERTICAL_TOTAL()-1;
    rtdf_outl(DV_TOTAL_0x2014,tmp);

    tmp = Get_DISP_DEN_STA_HPOS();
    tmp = tmp <<16 | Get_DISP_DEN_END_HPOS();
    rtdf_outl(DH_DEN_START_END_0x2018,tmp);
    tmp = Get_DISP_DEN_STA_VPOS();
    tmp = tmp <<16 | Get_DISP_DEN_END_VPOS();
    rtdf_outl(DV_DEN_START_END_0x201c,tmp);
    rtdf_outl(DOUBLE_BUFFER_CTRL_0x2028,5);
}

void CreateDisplayPlayMainTable( )
{
    rtdf_outl(DOUBLE_BUFFER_CTRL_0x2028,5);

    rtdf_outl(MAIN_DISPLAY_CONTROL_RSV_0x2100,1);

    rtdf_outl(MAIN_BACKGROUND_COLOR_0x210c,0);

    scalerdisplay_set_main_act_window(Get_DISP_ACT_STA_HPOS(), Get_DISP_ACT_END_HPOS(), Get_DISP_ACT_STA_VPOS(), Get_DISP_ACT_END_VPOS());
}

/*============================================================================*/
/**
 * drvif_scalerdisplay_set_bg_color
 * This function is used to set background and border color of both main and sub.
 *
 * @param <display> {main or sub display}
 * @param <type> {offset value of background or external or highlight address}
 * @param <color_red> {red value}
 * @param <color_grn> {green value}
 * @param <color_blu> {blue value}
 * @return { none }
 *
 */
void drv_scalerdisplay_set_bg_color(UINT8 ColorR, UINT8 ColorG, UINT8 ColorB)
{
    UINT32 tmp;
    tmp = ColorR&0x0f;
    tmp = tmp <<4 | (ColorG&0x0f);
    tmp = tmp <<4 | (ColorB&0x0f);

    rtdf_maskl(MAIN_BACKGROUND_COLOR_0x210c, 0xffff000, tmp);

    drv_scalerdisplay_set_dbuffer_en(_TRUE);
    rtdf_pageSelect(_PAGE0);
    tmp = rtdf_readScalerPort(0x6c)|_BIT5;
    rtd_writeScalerPort(0x6c,(UINT8)tmp ); // Background color
    rtd_writeScalerPort(0x6D,ColorR); //R 8-bit
    rtd_writeScalerPort(0x6D,ColorG ); //G 8-bit
    rtd_writeScalerPort(0x6D,ColorB ); //B 8-bit
    tmp = rtdf_readScalerPort(0x6c)&(~_BIT5);
    rtd_writeScalerPort(0x6c,(UINT8)tmp );
}

void drv_scalerdisplay_ForceBackground(BOOL Enable)
{
    if (Enable)  	// enable force-to-background
    {
        //rtdf_setBits(MAIN_DISPLAY_CONTROL_RSV_0x2100, _BIT1);
        rtdf_setBits(DISPLAY_TIMING_CTRL1_0x2000, _BIT18);
    }
    else  	// disable force-to-background
    {
        //rtdf_clearBits(MAIN_DISPLAY_CONTROL_RSV_0x2100, _BIT1);
        rtdf_maskl(DISPLAY_TIMING_CTRL1_0x2000, ~_BIT18, 0);
    }
    drv_scalerdisplay_set_dbuffer_en(_TRUE);
}

/*
void drv_scalerdisplay_handler(void)
{
	RTD_Log(LOGGER_DEBUG,"fw_scalerdisplay_handler!!!!! \n");
	////////////////////////////////////////////////
	// Display main window control
	////////////////////////////////////////////////
	drv_scalerdisplay_set_main_ctrl();
	////////////////////////////////////////////////
	// Display timing control
	////////////////////////////////////////////////
	//scalerdisplay_set_timing();


}


void drv_scalerdisplay_enable_display(BOOL bEnable)
{
	if(bEnable)
	{
		rtdf_setBits(MAIN_DISPLAY_CONTROL_RSV_0x2100, _BIT0);
		SET_SHOW_STATUS();
	}
	else
	{
		rtdf_clearBits(MAIN_DISPLAY_CONTROL_RSV_0x2100, _BIT0);
		CLR_SHOW_STATUS();
	}
	drv_scalerdisplay_set_dbuffer_en(_TRUE);
}
*/

/*============================================================================*/
/**
 * drvif_scalerdisplay_init
 * Init display control module. Collect the register, which should be insert only once.
 *
 * @param  { none }
 * @return { none }
 *
 */
void drv_scalerdisplay_Init(void)
{
    UINT32 temp = Get_DISPLAY_CLOCK_TYPICAL();
//	UINT16 getVal;
    temp = temp *1000000;
    drv_pll_Set_DClk(0,0, temp);


//	getVal = flow_Get_Panel_Parameter(OFFSET_STRUCT(STRUCT_PANEL_PARAMETER, fOptions), SIZEOF_STRUCT(STRUCT_PANEL_PARAMETER, fOptions));
//	RTD_Log(LOGGER_DEBUG, "Get paramter fOptions = %x\n", (UINT32)getVal);

//	getVal = Get_DISPLAY_PORT();
//	RTD_Log(LOGGER_DEBUG, "Get paramter data port = %x\n", (UINT32)getVal);

//	getVal = Get_DISPLAY_COLOR_BITS();
//	RTD_Log(LOGGER_DEBUG, "Get paramter color bit = %x\n", (UINT32)getVal);

    // Turn off power down operation for ALL PLL, because of affecting access regsiter in D-Domain
//	rtdf_maskl(PLL_DISP_1_0x2f04, ~(_BIT0 | _BIT1 | _BIT2 | _BIT3), 0x00);

    drv_panel_InitDisplayPort();
    // enable panel interface
#if (CONFIG_DISPLAY_PANEL_STYLE == _PANEL_INTERFACE_RSDS)
    rtdf_maskl(CONTROL_MISC_0x2258,~(_BIT6|_BIT7|_BIT8|_BIT9), _BIT8);
#elif (CONFIG_DISPLAY_PANEL_STYLE == _PANEL_INTERFACE_TTL)
    rtdf_maskl(CONTROL_MISC_0x2258,~(_BIT6|_BIT7|_BIT8|_BIT9), _BIT9);
#else
    rtdf_maskl(CONTROL_MISC_0x2258,~(_BIT11|_BIT6|_BIT7|_BIT8|_BIT9), _BIT6|_BIT7|(Get_DISPLAY_PORT()<<5));
#endif
//	rtdf_outl(SYSTEM_LVDS_RSDS_PAD_CTRL1_VADDR, 0x00000001);
    // Set LVDS Ctrl's Bit-mapping Table Select
//	rtdf_maskl(DDOMAIN_LVDS_CTRL_VADDR, ~_BIT0, (Get_DISPLAY_BITMAPPING_TABLE()  & _BIT0));

    CreateDisplayTimingTable();
    //RtdCtrl_WriteTable(tDisplayTiming);

    CreateDisplayPlayMainTable();
    //RtdCtrl_WriteTable(tDisplayMain);

    SET_PANEL_BACKLIGHT( (UINT16)(254*GET_COLOR_BACKLIGHT())/10);

    drv_scalerdisplay_ForceBackground(_ENABLE);

    //Init Display setting for no signal when boot
//	scalerdisplay_set_main_display_window(0, _DISP_WID, 0, _DISP_LEN, 0);

#ifdef FIX_DATA_SHIFT_2BITS //for RSDS Color error to reserve color MSB 2bit HPWang
    drvif_color_setgamma(1);
#endif
    drv_color_set_inputgamma( _DISABLE, _InputGamma_RGB_Sep);//temporary disable

    RTD_Log(LOGGER_DEBUG, "Initial Display is completed. \n");
}

/*============================================================================*/
/**
 * scalerdisplay_set_timing
 * This function is used to set display timing
 *
 * @param <info> { display-info struecture }
 * @return { none }
 *
 */
void drv_scalerdisplay_set_timing(void)
{
    UINT32 tmp;
    //   Last line will be used only in FrameSync mode
    //	if we don't marked these lines, sometimes it will get the
    //   wrong line number & the display of YPbPr in Sub will
    //	become ugly.

    // Set Display Horizontal Sync Width
    tmp = Get_DISP_HSYNC_WIDTH() & 0x000000ff;
    rtdf_maskl(DH_WIDTH_0x2008, 0xffffff00, tmp);

    // Set display horizantal total
    tmp = (Get_DISP_HORIZONTAL_TOTAL()-1) & 0x0fff;
    tmp = tmp << 16 | ((Get_DISP_HSYNC_LASTLINE()-1) & 0x0fff);
    rtdf_outl(DH_TOTAL_LAST_LINE_LENGTH_0x200c, tmp);

    // Set display V-sync length
    tmp = Get_DISP_VSYNC_LENGTH() & 0x000000ff;
    rtdf_maskl(DV_LENGTH_0x2010, 0xffffff00, tmp);

    tmp = (Get_DISP_VERTICAL_TOTAL()-1) & 0x00000fff;
    rtdf_maskl(DV_TOTAL_0x2014, 0xfffff000, tmp);
}

void drv_scalerdisplay_DVST_reset(void)
{
    rtdf_writeWord1(DV_DEN_START_END_0x201c,Get_DISP_DEN_STA_VPOS());
}

/*============================================================================*/
/**
 * scalerdisplay_get_main_display_ori_size
 * Retrieve main window size, includes width and length.
 * Mostly, it will be the same as panel size, except 1C12B or 1L7B in sub display.
 *
 * @param <*DispWid> {pointer to DispWid}
 * @param <*DispLen> {pointer to DispLen}
 * @return { none }
 *
 */
void scalerdisplay_get_main_display_ori_size()
{
    disp_info.DispWid = _DISP_WID;
    disp_info.DispLen = _DISP_LEN;
}

//******************************************************************************
//
// FUNCTION       :  void fw_scalerdisplay_get_display_size(UINT8 display)
//
// USAGE          	:  	Set display window size, it must considers aspect ratio.
//
// INPUT         	 	:  	{ none }
//
// OUTPUT         	:	{ none }
//
// GLOBALS        	:
//
// USED_REGS      :
//
//******************************************************************************
void drv_scalerdisplay_get_display_size(void)
{
    //UINT32  ucModeRatio;//For Keil Building
    scalerdisplay_get_main_display_ori_size();
    //RTD_Log(LOGGER_DEBUG,"DV start = %x\n",rtdf_inl(DV_DEN_START_END_0x201c));
    //rtdf_writeWord1(DV_DEN_START_END_0x201c,Get_DISP_DEN_STA_VPOS());
#if 1
    if (Get_DISPLAY_RATIO_4X3())
        //=============================//
    {
        //=============================//
        /////////////////////////////////////
        // 4:3 Panel
        /////////////////////////////////////
        //
        //if (IS_RATIO_16_9()) //4	//4:3 panel but require 16:9 diplay only in FRC
        //////////////////////////////////////////
        //Calculate 4:3 Display
        //////////////////////////////////////////
        //	disp_info.DispLen = SHR(disp_info.DispLen * 3, 2);
    }
    else	//CONFIG_DISPLAY_RATIO_4X3
    {
        //=============================//
        /////////////////////////////////////
        //16:9 Panel
        /////////////////////////////////////
        if (IS_RATIO_4_3()) 												//16 :9 panel but require 4:3 display
            disp_info.DispWid = (SHL(info->DispLen, 2) / 3);

    }
#else
    // if run to here Panel Ratio is 16 : 9 or 16 : 10

    RTD_Log(LOGGER_DEBUG,"GET_DISPLAY_RATIO()-------------------------------------- %d\n",(UINT32)GET_DISPLAY_RATIO());

    if (IS_RATIO_4_3() )
    {
        disp_info.DispWid = Get_DISP_ACT_V_LEN()*4/3;
    }
    //    else if(IS_RATIO_16_9())
    //     {
    //			disp_info.DispWid = Get_DISP_ACT_V_LEN()*16/9;
    //     }
    else	if (IS_RATIO_AUTO())	// auto
    {
        RTD_Log(LOGGER_DEBUG,"disp_info.DispWid=---------IS_RATIO_AUTO----------------------------- %d\n");
        ucModeRatio = (UINT32) Get_DISP_ACT_V_LEN()* 100 / Get_DISP_ACT_H_WID();

        if (ucModeRatio >= 75)
            disp_info.DispWid = Get_DISP_ACT_V_LEN()*4/3;
        // else
        //	disp_info.DispWid  = Get_DISP_ACT_V_LEN()*16/9;
    }
#endif
    //disp_info.DispWid_PRE = disp_info.DispWid;
    //disp_info.DispLen_PRE = disp_info.DispLen;
    //ROSPrintf("display=%x, DispWid=%x, DispLen=%x in SetWindowSize\n",info->display, info->DispWid, info->DispLen);
    //ROSPrintf("display=%x, DispWid_PRE=%x, DispLen_PRE =%x in SetWindowSize\n",info->display, info->DispWid_PRE, info->DispLen_PRE);
}

/*============================================================================*/
//******************************************************************************
//
// FUNCTION       :  void scalerdisplay_set_main_ctrl(void)
//
// USAGE          	:  	 This function is used to control main display
//
// INPUT         	 	:  	{ none }
//
// OUTPUT         	:	{ none }
//
// GLOBALS        	:
//
// USED_REGS      :
//
//******************************************************************************
void drv_scalerdisplay_set_main_ctrl(void)
{
    UINT16  VRatioOffset = 0,HRatioOffset = 0;
    UINT16  HStartOffset = 0;
    UINT16  VStartOffset = 0;

    //============================//
    if (Get_DISPLAY_RATIO_4X3())
        //============================//
    {

        VRatioOffset = 0; // offset at vertical direction, zero at this case
        scalerdisplay_set_main_act_window(	(UINT16)HStartOffset,
                                           (UINT16)info->DispWid + HStartOffset,
                                           (UINT16)VRatioOffset + VStartOffset,
                                           (UINT16)info->DispLen + VRatioOffset + VStartOffset);

    }
    //============================//
    else  // 16:9 panel	CONFIG_DISPLAY_RATIO_4X3
        //============================//
    {
        //////////////////////////////////////////
        //16:9 Panel
        ///////////////////////////////////////////

        if (IS_RATIO_4_3()) //4 : 3 ratio
        {
            //////////////////////////////////////////
            //4:3 Display
            ///////////////////////////////////////////
            //VRatioOffset = (UINT16)SHR(_DISP_WID , 3); // offset at horizontal direction
            ////////////////////////////////////////////
            //correct calculate H start <Gabriel 2008/01/16>
            ////////////////////////////////////////////
            VRatioOffset = (_DISP_WID - info->DispWid) >> 1;
            scalerdisplay_set_main_act_window(	(UINT16)Get_DISP_ACT_STA_HPOS()  + VRatioOffset + HStartOffset,
                                               (UINT16)Get_DISP_ACT_STA_HPOS()  + info->DispWid + VRatioOffset + HStartOffset,
                                               (UINT16)Get_DISP_ACT_STA_VPOS()  + VStartOffset,
                                               (UINT16)Get_DISP_ACT_STA_VPOS()  + info->DispLen + VStartOffset);
        }
        else //16:9 ratio
        {
            VRatioOffset = 0; // offset at horizontal direction, zero at this case
            scalerdisplay_set_main_act_window(	(UINT16)Get_DISP_ACT_STA_HPOS()  + VRatioOffset + HStartOffset,
                                               (UINT16)Get_DISP_ACT_STA_HPOS()  + info->DispWid + VRatioOffset + HStartOffset,
                                               (UINT16)Get_DISP_ACT_STA_VPOS()  + VStartOffset,
                                               (UINT16)Get_DISP_ACT_STA_VPOS()  + info->DispLen + VStartOffset);
        }
    }

}

