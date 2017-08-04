/*===========================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2005
 * All rights reserved.
 * ========================================================== */

/*==================== File Description ===================== */
/**
 * @file
 * 	This file osd message process is for osd related functions.
 *
 * @author 	$Author: kingee $
 * @date 	$Date: 2006-09-19 19:47:08 +0800 (?üÊ?‰∫? 19 ‰πùÊ? 2006) $
 * @version 	$Revision: 1204 $
 */

/**
 * @addtogroup vbi
 * @{
 */
//#define _OSDMESSAGE_C

/*===================== Module dependency  ================== */
#include "rosprintf.h"
#include "message.h"
#include "timer_event.h"
#include "timer_event_ui_table.h"
#include "flow_struct.h"
#include "osdparameter.h"
#include "osdcore.h"
#include "osdcontrol.h"
#include "reg52.h"
#include "autoconf.h"
#include "osdfont.h"
#include "osdfunction.h"
#include "osdregdef.h"
#include "osdobjectdef.h"
#include "sparrow.h"
#include "power.h"
#include "panel\panelapi.h"
#ifdef CONFIG_VBI_CC_SUPPORT
#include <vbi/slr.h>
#include "osdvbi.h"
#include "closecaption.h"

UINT8  xdata ucOsdVbiControl;
UINT8 xdata ucOsdVbiCoreBuf[8];

/*======================= Private Types ===================== */

/*======================== Definitions ====================== */
#define SET_OSDVBI_SHOWMEMORYBLOCK() 	(ucOsdVbiControl |= _BIT1)
#define CLR_OSDVBI_SHOWMEMORYBLOCK() 	(ucOsdVbiControl &= (~_BIT1))
#define GET_OSDVBI_SHOWMEMORYBLOCK() 	((ucOsdVbiControl & _BIT1) >> 1)

#define SET_OSDVBI_HADINITIALLASTROW() 	(ucOsdVbiControl |= _BIT3)
#define CLR_OSDVBI_HADINITIALLASTROW() 	(ucOsdVbiControl &= (~_BIT3))
#define GET_OSDVBI_HADINITIALLASTROW() 	((ucOsdVbiControl & _BIT3) >> 3)

#define SET_OSDVBI_CANSCROLLUP() 		(ucOsdVbiControl |= _BIT4)
#define CLR_OSDVBI_CANSCROLLUP() 		(ucOsdVbiControl &= (~_BIT4))
#define GET_OSDVBI_CANSCROLLUP() 		(ucOsdVbiControl & _BIT4)

/*========================== Variables ====================== */
// For Row Command
//static idata UINT8 ucOsdVbiShowStartRow = 0;
//static idata UINT8 ucOsdVbiShowNumRow= 16;

// For ColumnSpace
static xdata UINT8 ucOsdVbiColSpaceStartRow = 0x00; //idata
static xdata UINT8 ucOsdVbiRowOffset = 0; //idata
static xdata UINT8 ucOsdVbiColOffset = 0; //idata

// Control Flag
static xdata UINT16 ucOsdVbiShowRowFlag = 0x0000; 		// which row to be showed idata
//Disable by Gilbert	static idata UINT16 ucOsdVbiNonShowRowFlag = 0x0000;	// which row to be showed in future
static xdata UINT8	ucOsdVbiRollupStartRow = 0x00; //idata
static xdata UINT8 ucOsdVbiRollupNumber = 0x00;   //idata
//static idata UINT8 ucTmpTest;
#define _AUTOINC          0
#define _NON_AUTOINC 1
UINT8 xdata Ccsel=0;
UINT8 xdata ucRollupTimerCnt;
#define VBIOSD_512FONT_UNSUPPORT _TRUE

/*=================== Local Functions Phototype ==============*/
static void COsdVbiSendOsdAddress(UINT16 usValue);
static void COsdVbiEstablishEnv(void);
void COsdFxLoadVbiFont(void);


void COsdFxLoadVbiFont(void)
{
    ap_osdfx_load_Vbi_vlcfont(tFONT_GLOBAL_Italic, 0x00, 0x60);
    ap_osdfx_load_Vbi_vlcfont(tFONT_WESTEUR, 0x60, 0x50);
    SET_FIRST_LOADFONT();
}

/*=========================== Functions ===================== */
static void COsdVbiSendOsdAddress(UINT16 usValue)
{
    rtdf_pageSelect(0);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, ((usValue & 0xff00)>> 8));
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, (usValue & 0x00ff));
}

void COsdVbiBlendingOperation(UINT8 on_off)
{
    ucOsdVbiCoreBuf[0] = 0x00;
    ucOsdVbiCoreBuf[1] = 0x03;
    if (on_off)
        ucOsdVbiCoreBuf[2] = 0x70;
    else
        ucOsdVbiCoreBuf[2] = 0x10;

    rtdf_pageSelect(_PAGE0);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, ucOsdVbiCoreBuf[0]);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, ucOsdVbiCoreBuf[1]);
    rtdf_writeScalerPort(UIOSD_OSD_DATA_PORT_0x0092, ucOsdVbiCoreBuf[2]);
}

static void COsdVbiEstablishEnv(void)
{
    // Initialize Colum Space
    COsdVbiClearAllMemory();
    rtdf_pageSelect(_PAGE0);
    // Reload font table
    COsdFxLoadVbiFont();
    ap_osdfx_load_vbipalette();

    // Enable Blinking
    ucOsdVbiCoreBuf[0] = 0x80;
    ucOsdVbiCoreBuf[1] = 0x03;
    ucOsdVbiCoreBuf[2] = 0x02;
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, ucOsdVbiCoreBuf[0]);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, ucOsdVbiCoreBuf[1]);
    rtdf_writeScalerPort(UIOSD_OSD_DATA_PORT_0x0092, ucOsdVbiCoreBuf[2]);
}

void COsdVbiInitialVariable(void)
{
    ucRollupTimerCnt = 0;
    ucOsdVbiRowOffset = 0;
    ucOsdVbiColOffset = 0;
//Disable by Gilbert	ucOsdVbiNonShowRowFlag = 0x0000;
    ucOsdVbiRollupStartRow = 0x00;
    ucOsdVbiRollupNumber = 0x00;
    ucOsdVbiColSpaceStartRow = 0x00;
    COsdVbiResetScrollRow();
}


// Initial function
void COsdVbiInitial(void)
{
    bCCDecodeStalbe = _TRUE;
    //
    SET_OSDVBI_HADSHOWED();
    COsdVbiShowData(0);
    SetOSDDouble(0);
    ap_osdfx_map_setting(_OSD_MAP0);

    COsdFxSetWinRefPos(((Get_DISP_ACT_H_WID() - 14 * 34 * 2) /8 + 144/4),((Get_DISP_ACT_V_LEN() - 24 * 15 * 2) / 2));
    COsdVbiBlendingOperation(1);
    COsdVbiSetShowRow(1, 15);
    COsdFxLoadVbiFont();

    COsdVbiInitialVariable();
    COsdVbiEstablishEnv();

    CLR_OSDVBI_CANSCROLLUP();
    CLR_OSDVBI_SHOWMEMORYBLOCK();
    SET_OSDVBI_INITIAL();
}

WindowStyle code tOsdVbiTextMode_BackgroundWindow = {0,
        {0, 47, 14 * _OSDVBI_ROW_LEN_INCOLUMNSPACE * 2, 24 * 16 * 2},
        {0x00, (_CP_BLACK << 4)| _CP_WHITE,  0x00},
        {0x00, 0x10 | _CP_BLACK, 0x01}
                                                    };

// Enable/Disable vbi show
void COsdVbiShowData(UINT8 on_off)
{
    if (on_off)
    {
#if 0//def CONFIG_VBI_VCHIP
        if (!GET_OSDVBI_HADSHOWED() && !VchipBlockStatus)
        {
#else
        if (!GET_OSDVBI_HADSHOWED())
        {
#endif
            if (bCCDecodeStalbe ==_TRUE)
            {
                SET_OSDVBI_HADSHOWED();
                if ((GET_CLOSEDCAPTION() > 4) && (GET_CLOSEDCAPTION() <= 8))
                {
#if 0
                    COsdFxDrawWindow(&tOsdVbiTextMode_BackgroundWindow);
#else
                    rtdf_pageSelect(0);
                    ucOsdVbiCoreBuf[0] = (tOsdVbiTextMode_BackgroundWindow.ucWindowID) << 2;		// x4
                    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, 0xc1);		// window control regs, all bytes
                    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, ucOsdVbiCoreBuf[0]);

                    ucOsdVbiCoreBuf[0] = tOsdVbiTextMode_BackgroundWindow.ucCtrlRegAddr0[0];
                    ucOsdVbiCoreBuf[1] = tOsdVbiTextMode_BackgroundWindow.ucCtrlRegAddr0[1];
                    ucOsdVbiCoreBuf[2] = tOsdVbiTextMode_BackgroundWindow.ucCtrlRegAddr0[2];
                    RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_DATA_PORT_0x0092, 3, ucOsdVbiCoreBuf, _NON_AUTOINC);

                    // Window Start Position
                    ucOsdVbiCoreBuf[0] = ((tOsdVbiTextMode_BackgroundWindow.stPosition.usHStart&0x0f00) >> 4) | ((tOsdVbiTextMode_BackgroundWindow.stPosition.usVStart&0x0f00) >> 8);
                    ucOsdVbiCoreBuf[1] = (tOsdVbiTextMode_BackgroundWindow.stPosition.usHStart&0xff);
                    ucOsdVbiCoreBuf[2] = (tOsdVbiTextMode_BackgroundWindow.stPosition.usVStart&0xff);
                    RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_DATA_PORT_0x0092, 3, ucOsdVbiCoreBuf, _NON_AUTOINC);

                    // Window End Position
                    ucOsdVbiCoreBuf[0]= ((tOsdVbiTextMode_BackgroundWindow.stPosition.usHEnd&0x0f00) >> 4) | ((tOsdVbiTextMode_BackgroundWindow.stPosition.usVEnd&0x0f00) >> 8);
                    ucOsdVbiCoreBuf[1] = LOBYTE(tOsdVbiTextMode_BackgroundWindow.stPosition.usHEnd);
                    ucOsdVbiCoreBuf[2] = LOBYTE(tOsdVbiTextMode_BackgroundWindow.stPosition.usVEnd);
                    RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_DATA_PORT_0x0092, 3, ucOsdVbiCoreBuf, _NON_AUTOINC);

                    ucOsdVbiCoreBuf[0] = tOsdVbiTextMode_BackgroundWindow.ucCtrlRegAddr2[0];
                    ucOsdVbiCoreBuf[1] = tOsdVbiTextMode_BackgroundWindow.ucCtrlRegAddr2[1];
                    ucOsdVbiCoreBuf[2] = tOsdVbiTextMode_BackgroundWindow.ucCtrlRegAddr2[2];
                    RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_DATA_PORT_0x0092, 3, ucOsdVbiCoreBuf, _NON_AUTOINC);
#endif
                }
                else
                {
                    COsdFxCloseWindow(tOsdVbiTextMode_BackgroundWindow.ucWindowID);

                }
                COsdFxEnableOsd();
            }
            COsdVbiBlendingOperation(1);
        }
#if 0//def CONFIG_VBI_VCHIP
        else
        {
            if (VchipBlockStatus)
            {
                CLR_OSDVBI_HADSHOWED();
                COsdFxDisableOsd();
            }
        }
#endif
    }
    else
    {
        if (GET_OSDVBI_HADSHOWED())
        {
            CLR_OSDVBI_HADSHOWED();
            COsdFxDisableOsd();
        }
    }
}

// Store data into Display/NonDisplay memory
void	COsdVbiSetDisplayMemory(UINT8 ucRow, UINT8 ucCol, UINT8 ucAttr, UINT8 ucColor, UINT8 ucData)
{
    rtdf_pageSelect(_PAGE0);
    if (!ucOsdVbiRollupNumber)
    {
        //Disable by Gilbert ucOsdVbiShowRowFlag |= ((UINT16) 1 << ucRow);
        COsdVbiStartAddress(ucOsdVbiColSpaceStartRow + ucRow - 1, ucCol, _WRITE_BYTEALL);
    }
    else
    {
        //Disable by Gilbert if((ucOsdVbiColSpaceStartRow + ucOsdVbiRollupNumber) > 15)
        //Disable by Gilbert 	ucOsdVbiNonShowRowFlag |= ((UINT16) 1 << ucOsdVbiColSpaceStartRow + ucOsdVbiRollupNumber);
        //Disable by Gilbert else
        //Disable by Gilbert 	ucOsdVbiNonShowRowFlag |= ((UINT16) 1 << ucOsdVbiColSpaceStartRow + ucOsdVbiRollupNumber);
        COsdVbiStartAddress(ucOsdVbiColSpaceStartRow + ucOsdVbiRollupNumber - 1, ucCol, _WRITE_BYTEALL);
    }
    ucData = ucData - 0x20;

//	if(ucColor & 0xf0)
//		ucColor += 0x80;
#if 0//(_VBI_ITALIC_ENABLE == _TRUE)
    if (ucAttr & 0x80)
    {
        if ((ucData >= 0x10) && (ucData <= 0x19)) // For 0~9
            ucData = (ucData - 0x10) + 0xB0;
        else if ((ucData >= 0x21) && (ucData <= 0x3A)) // For A~Z
            ucData = (ucData - 0x21) + 0xBA;
        else if ((ucData >= 0x41) && (ucData <= 0x5A)) // For a~z
            ucData = (ucData - 0x41) + 0xD4;
    }
#endif

#if(VBIOSD_512FONT_UNSUPPORT==_TRUE)
    switch (ucAttr & (_BIT0|_BIT1))
    {
    case 0x03://( underline flag==_ON)&&( FLASHON_FLAG==_ON)
        ucOsdVbiCoreBuf[0] = 0xc6;
        break;
    case 0x01://if( underline flag==_ON)
        ucOsdVbiCoreBuf[0] = 0x82;
        break;
    case 0x02://if( FLASHON_FLAG==_ON)
        ucOsdVbiCoreBuf[0] = 0xc4;
        break;
    default:
        ucOsdVbiCoreBuf[0] = 0x80;
        break;
    }
#else
    switch ((ucAttr & (_BIT0|_BIT1)))
    {
    case 0x03://( underline flag==_ON)&&( FLASHON_FLAG==_ON)
        ucOsdVbiCoreBuf[0] = 0xce;
        break;
    case 0x01://if( underline flag==_ON)
        ucOsdVbiCoreBuf[0] = 0x8a;
        break;
    case 0x02://if( FLASHON_FLAG==_ON)
        ucOsdVbiCoreBuf[0] = 0xcc;
        break;
    default:
        ucOsdVbiCoreBuf[0] = 0x88;
        break;
    }
#endif

    ucOsdVbiCoreBuf[1] = ucData;
    ucOsdVbiCoreBuf[2] = ((ucColor & 0x0f) << 4) | ((ucColor & 0xf0) >> 4);
    RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_DATA_PORT_0x0092,3,ucOsdVbiCoreBuf,_NON_AUTOINC);

    if (ucOsdVbiRollupNumber && (ucOsdVbiColSpaceStartRow > 1))
    {
        COsdVbiStartAddress(ucOsdVbiColSpaceStartRow - 2, ucCol, _WRITE_BYTEALL);
#if(VBIOSD_512FONT_UNSUPPORT==_TRUE)
        switch (ucAttr & (_BIT0|_BIT1))
        {
        case 0x03://( underline flag==_ON)&&( FLASHON_FLAG==_ON)
            ucOsdVbiCoreBuf[0] = 0xc6;
            break;
        case 0x01://if( underline flag==_ON)
            ucOsdVbiCoreBuf[0] = 0x82;
            break;
        case 0x02://if( FLASHON_FLAG==_ON)
            ucOsdVbiCoreBuf[0] = 0xc4;
            break;
        default:
            ucOsdVbiCoreBuf[0] = 0x80;
            break;
        }
#else
        switch ((ucAttr & (_BIT0|_BIT1)))
        {
        case 0x03://( underline flag==_ON)&&( FLASHON_FLAG==_ON)
            ucOsdVbiCoreBuf[0] = 0xce;
            break;
        case 0x01://if( underline flag==_ON)
            ucOsdVbiCoreBuf[0] = 0x8a;
            break;
        case 0x02://if( FLASHON_FLAG==_ON)
            ucOsdVbiCoreBuf[0] = 0xcc;
            break;
        default:
            ucOsdVbiCoreBuf[0] = 0x88;
            break;
        }
#endif
        ucOsdVbiCoreBuf[1] = ucData;
        ucOsdVbiCoreBuf[2] = ((ucColor & 0x0f) << 4) | ((ucColor & 0xf0) >> 4);
        RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_DATA_PORT_0x0092,3,ucOsdVbiCoreBuf,_NON_AUTOINC);
    }
}

void	COsdVbiSetNonDisplayMemory(UINT8 ucRow, UINT8 ucCol, UINT8 ucAttr, UINT8 ucColor, UINT8 ucData)
{
    //Disable by Gilbert ucOsdVbiNonShowRowFlag |= ((UINT16) 1 << ucRow);

    if (!GET_OSDVBI_SHOWMEMORYBLOCK())
        ucRow += _OSDVBI_DOUBLEBUFFER_ROW;

    ucData = ucData - 0x20;
#if 0//(_VBI_ITALIC_ENABLE == _TRUE)
    if (ucAttr & 0x80)
    {
        if ((ucData >= 0x10) && (ucData <= 0x19)) // For 0~9
            ucData = (ucData - 0x10) + 0xB0;
        else if ((ucData >= 0x21) && (ucData <= 0x3A)) // For A~Z
            ucData = (ucData - 0x21) + 0xBA;
        else if ((ucData >= 0x41) && (ucData <= 0x5A)) // For a~z
            ucData = (ucData - 0x41) + 0xD4;
    }
#endif

    rtdf_pageSelect(_PAGE0);
    COsdVbiStartAddress(ucRow - 1, ucCol, _WRITE_BYTEALL);
#if(VBIOSD_512FONT_UNSUPPORT==_TRUE)
    switch (ucAttr & (_BIT0|_BIT1))
    {
    case 0x03://( underline flag==_ON)&&( FLASHON_FLAG==_ON)
        ucOsdVbiCoreBuf[0] = 0xc6;
        break;
    case 0x01://if( underline flag==_ON)
        ucOsdVbiCoreBuf[0] = 0x82;
        break;
    case 0x02://if( FLASHON_FLAG==_ON)
        ucOsdVbiCoreBuf[0] = 0xc4;
        break;
    default:
        ucOsdVbiCoreBuf[0] = 0x80;
        break;
    }
#else
    switch (ucAttr & (_BIT0|_BIT1))
    {
    case 0x03://( underline flag==_ON)&&( FLASHON_FLAG==_ON)
        ucOsdVbiCoreBuf[0] = 0xce;
        break;
    case 0x01://if( underline flag==_ON)
        ucOsdVbiCoreBuf[0] = 0x8a;
        break;
    case 0x02://if( FLASHON_FLAG==_ON)
        ucOsdVbiCoreBuf[0] = 0xcc;
        break;
    default:
        ucOsdVbiCoreBuf[0] = 0x88;
        break;
    }
#endif

    ucOsdVbiCoreBuf[1] = ucData;
    ucOsdVbiCoreBuf[2] = ((ucColor & 0x0f) << 4) | ((ucColor & 0xf0) >> 4);
    RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_DATA_PORT_0x0092,3,ucOsdVbiCoreBuf,_NON_AUTOINC);
}

// Scroll up function
void COsdVbiResetScrollUpEnv(void)
{
    ucOsdVbiRollupNumber = 0;
    ucRollupTimerCnt = 0;
    ucOsdVbiColSpaceStartRow = 0;
}

//extern BYTE CheckCCTTMode(void);
void COsdVbiSetScrollRow(void)
{
    UINT8 cnt;

    if (ucOsdVbiRollupNumber)
    {

        while (ucRollupTimerCnt)
        {
            COsdVbiRullUpProc();
        }

        if (ucOsdVbiColSpaceStartRow > ucOsdVbiRollupNumber)
            COsdVbiResetScrollRow();

        ucOsdVbiColSpaceStartRow++;

        ucOsdVbiShowRowFlag = ucOsdVbiShowRowFlag >> 1;

        COsdVbiClearRow(ucOsdVbiRollupNumber - 1);

        ucRollupTimerCnt = _OSDVBI_ROLLUP_NUM;			//CC Mode

        CLR_OSDVBI_HADINITIALLASTROW();
        SET_OSDVBI_CANSCROLLUP();

        COsdVbiRullUpProc();

        rtdf_pageSelect(_PAGE0);
        if (ucOsdVbiColSpaceStartRow > 1)
        {
            COsdVbiStartAddress(ucOsdVbiColSpaceStartRow - 2, 0, _WRITE_BYTEALL);
#if(VBIOSD_512FONT_UNSUPPORT==_TRUE)
            ucOsdVbiCoreBuf[0] = 0x80;//0x84
#else
            ucOsdVbiCoreBuf[0] = 0x88;//0x8c
#endif
            ucOsdVbiCoreBuf[1] = 0x00;
            ucOsdVbiCoreBuf[2] = 0x10;
            for (cnt = 0; cnt < _OSDVBI_ROW_LEN_INCOLUMNSPACE; cnt++)
                RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_DATA_PORT_0x0092, 3, ucOsdVbiCoreBuf, _NON_AUTOINC);
        }
    }
}

void COsdVbiRullUpProc(void)
{
    if (!GET_OSDVBI_CANSCROLLUP())
        return;

    if (--ucRollupTimerCnt == 0)
    {
        COsdVbiStopRullUpProc();
        return;
    }
#if 0  //tommy delete 20100810
    rtdf_pageSelect(_PAGE0);
    // Decrease first row(row 0) height
    /*	ucOsdVbiCoreBuf[0] = 0x50;
    	ucOsdVbiCoreBuf[1] = 0x00;
    	ucOsdVbiCoreBuf[2] = ((_OSDVBI_ROWHEIGNT - (_OSDVBI_ROLLUP_NUM - ucRollupTimerCnt) * _OSDVBI_ROLLUP_STEP - 1) << 3) | _OSDVBI_CHARACTERSPACE;
    */
    // Add rull up in last row
    if (!GET_OSDVBI_HADINITIALLASTROW())
    {
        SET_OSDVBI_HADINITIALLASTROW();
        ucOsdVbiCoreBuf[3] = 0xD0;
        ucOsdVbiCoreBuf[4] = ucOsdVbiRollupStartRow + 1;

        ucOsdVbiCoreBuf[5] = 0xc0 | (_OSDVBI_CHARACTER_DOUBLE_WIDTH  << 1) | _OSDVBI_CHARACTER_DOUBLE_HEIGHT;
        ucOsdVbiCoreBuf[6] = (((_OSDVBI_ROLLUP_NUM - ucRollupTimerCnt) * _OSDVBI_ROLLUP_STEP - 1) << 3) | _OSDVBI_CHARACTERSPACE;
        ucOsdVbiCoreBuf[7] = _OSDVBI_ROW_LEN_INCOLUMNSPACE;
        //Disable by Gilbert
        //CTimerWaitForEvent(_EVENT_DEN_STOP);
#if 0
        RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, 3, ucOsdVbiCoreBuf, _AUTOINC);//0:address auto inc,1 :no inc
        RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, 2, &ucOsdVbiCoreBuf[3], _AUTOINC);//0:address auto inc,1 :no inc
#else
        /*rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, ucOsdVbiCoreBuf[0]);
        rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, ucOsdVbiCoreBuf[1]);
        rtdf_writeScalerPort(UIOSD_OSD_DATA_PORT_0x0092, ucOsdVbiCoreBuf[2]);*/
        rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, ucOsdVbiCoreBuf[3]);
        rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, ucOsdVbiCoreBuf[4]);
#endif
        RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_DATA_PORT_0x0092, 3, &ucOsdVbiCoreBuf[5], _NON_AUTOINC);//0:address auto inc,1 :no inc
    }
    else
    {
        ucOsdVbiCoreBuf[3] = 0x50;
        ucOsdVbiCoreBuf[4] = ucOsdVbiRollupStartRow + 1;
        ucOsdVbiCoreBuf[5] = (((_OSDVBI_ROLLUP_NUM - ucRollupTimerCnt) * _OSDVBI_ROLLUP_STEP - 1) << 3) | _OSDVBI_CHARACTERSPACE;
        //Disable by Gilbert
        //CTimerWaitForEvent(_EVENT_DEN_STOP);
#if 0
        RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, 3, ucOsdVbiCoreBuf, _AUTOINC);//0:address auto inc,1 :no inc
        RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, 3, &ucOsdVbiCoreBuf[3], _AUTOINC);//0:address auto inc,1 :no inc
#else
        /*rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, ucOsdVbiCoreBuf[0]);
        rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, ucOsdVbiCoreBuf[1]);
        rtdf_writeScalerPort(UIOSD_OSD_DATA_PORT_0x0092, ucOsdVbiCoreBuf[2]);*/
        rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, ucOsdVbiCoreBuf[3]);
        rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, ucOsdVbiCoreBuf[4]);
        rtdf_writeScalerPort(UIOSD_OSD_DATA_PORT_0x0092, ucOsdVbiCoreBuf[5]);
#endif
    }
#endif
}

void COsdVbiStopRullUpProc(void)
{
    UINT16 usStartAddr = _OSDVBI_ROWCOUNT + 1 + ucOsdVbiColSpaceStartRow * _OSDVBI_ROW_LEN_INCOLUMNSPACE;

    // Modify first row to normal value and clear pre row
    ucOsdVbiCoreBuf[0] = 0x50;
    ucOsdVbiCoreBuf[1] = 0x00;
    ucOsdVbiCoreBuf[2] = ((_OSDVBI_ROWHEIGNT - 1) << 3) | _OSDVBI_CHARACTERSPACE;

    ucOsdVbiCoreBuf[3] = 0xc0;
    ucOsdVbiCoreBuf[4] = 0x04;

    ucOsdVbiCoreBuf[5] = (usStartAddr) & 0x00ff;
    ucOsdVbiCoreBuf[6] = (((usStartAddr) & 0x0f00) >> 4)
                         | ((_OSDVBI_FONTTABLE_STARTADDRESS) & 0x000f);
    ucOsdVbiCoreBuf[7] = ((_OSDVBI_FONTTABLE_STARTADDRESS) & 0x0ff0) >> 4;

    rtdf_pageSelect(_PAGE0);

    //Disable by Gilbert
    //CTimerWaitForEvent(_EVENT_DEN_STOP);
#if 0
    RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, 2, &ucOsdVbiCoreBuf[3], _AUTOINC);//0:address auto inc,1 :no inc
#else
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, ucOsdVbiCoreBuf[3]);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, ucOsdVbiCoreBuf[4]);
#endif
    RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_DATA_PORT_0x0092, 3, &ucOsdVbiCoreBuf[5], _NON_AUTOINC);//0:address auto inc,1 :no inc
#if 0
    RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, 3, ucOsdVbiCoreBuf, _AUTOINC);//0:address auto inc,1 :no inc
#else
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, ucOsdVbiCoreBuf[0]);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, ucOsdVbiCoreBuf[1]);
    rtdf_writeScalerPort(UIOSD_OSD_DATA_PORT_0x0092, ucOsdVbiCoreBuf[2]);
#endif

    // Close last
    ucOsdVbiCoreBuf[0] = 0xD0;
    ucOsdVbiCoreBuf[1] = ucOsdVbiRollupStartRow + 1;
#if 0
    RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, 2, ucOsdVbiCoreBuf, _AUTOINC);//0:address auto inc,1 :no inc
#else
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, ucOsdVbiCoreBuf[0]);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, ucOsdVbiCoreBuf[1]);
#endif

    //[0xc0:vbi osd function enable];[0x80:vbi osd function Disable]
    ucOsdVbiCoreBuf[2] = 0xc0 | (_OSDVBI_CHARACTER_DOUBLE_WIDTH  << 1) | _OSDVBI_CHARACTER_DOUBLE_HEIGHT;
    ucOsdVbiCoreBuf[3] = ((_OSDVBI_ROWHEIGNT - 1) << 3) | _OSDVBI_CHARACTERSPACE;
    ucOsdVbiCoreBuf[4] = 0x00;
    RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_DATA_PORT_0x0092, 3, &ucOsdVbiCoreBuf[2], _NON_AUTOINC);//0:address auto inc,1 :no inc

    CLR_OSDVBI_CANSCROLLUP();
}

void COsdVbiResetScrollRow(void)
{
    ucOsdVbiCoreBuf[0] = 0xc0;
    ucOsdVbiCoreBuf[1] = 0x04;

    ucOsdVbiCoreBuf[2] = (_OSDVBI_ROWCOUNT + 1) & 0x00ff;
    ucOsdVbiCoreBuf[3] = (((_OSDVBI_ROWCOUNT + 1) & 0x0f00) >> 4) | ((_OSDVBI_FONTTABLE_STARTADDRESS) & 0x000f);
    ucOsdVbiCoreBuf[4] = ((_OSDVBI_FONTTABLE_STARTADDRESS) & 0x0ff0) >> 4;

    rtdf_pageSelect(_PAGE0);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, ucOsdVbiCoreBuf[0]);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, ucOsdVbiCoreBuf[1]);
    RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_DATA_PORT_0x0092, 3, &ucOsdVbiCoreBuf[2], _NON_AUTOINC);//0:address auto inc,1 :no inc
    CLR_OSDVBI_SHOWMEMORYBLOCK();
    ucOsdVbiColSpaceStartRow = 0;
}
//
void COsdVbiSetRullUpStartRow(UINT8 ucStartRow, UINT8 ucRollNum)
{
//	UINT16 usWindowYStart, usWindowYEnd;

    if ((ucStartRow + 1) <= ucOsdVbiRollupNumber)
        ucStartRow = ucOsdVbiRollupNumber;
    else if ((ucStartRow - ucOsdVbiRollupNumber + 1) > 15)
        ucStartRow = 15;

    if ((ucRollNum == ucOsdVbiRollupNumber) && (ucOsdVbiRollupStartRow == ucStartRow))
        return;

    rtdf_pageSelect(_PAGE0);
    COsdVbiSetShowRow(ucStartRow - ucRollNum + 1, ucRollNum);

    ucOsdVbiRollupNumber = ucRollNum;
    ucOsdVbiRollupStartRow = ucStartRow;

#if 0
    usWindowYStart =	(ucOsdVbiRollupStartRow - ucOsdVbiRollupNumber) * _OSDVBI_ROWHEIGNT * 2 ;
    usWindowYEnd = usWindowYStart + _OSDVBI_ROWHEIGNT * 2;

    ucOsdVbiCoreBuf[0] = 0xC1;
    ucOsdVbiCoreBuf[1] = 0x00;//1D
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, ucOsdVbiCoreBuf[0]);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, ucOsdVbiCoreBuf[1]);

    ucOsdVbiCoreBuf[0] = 0x00;
    ucOsdVbiCoreBuf[1] = 0x01;
    ucOsdVbiCoreBuf[2] = 0x00;
    RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_DATA_PORT_0x0092, 3, ucOsdVbiCoreBuf, _NON_AUTOINC);

    ucOsdVbiCoreBuf[0] = (usWindowYStart & 0x0F00)>>8;
    ucOsdVbiCoreBuf[1] = 0;
    ucOsdVbiCoreBuf[2] = usWindowYStart & 0x00FF;
    RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_DATA_PORT_0x0092, 3, ucOsdVbiCoreBuf, _NON_AUTOINC);

    ucOsdVbiCoreBuf[0] = (((14 * 34 * 2 + 6) & 0x0F00)>>4)|((usWindowYEnd & 0x0F00) >>8);
    ucOsdVbiCoreBuf[1] = (14 * 34 * 2 + 12) & 0x00FF;
    ucOsdVbiCoreBuf[2]=  usWindowYEnd & 0x00FF;
    RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_DATA_PORT_0x0092, 3, ucOsdVbiCoreBuf, _NON_AUTOINC);

    ucOsdVbiCoreBuf[0] = 0;
    ucOsdVbiCoreBuf[1] = 0;
    ucOsdVbiCoreBuf[2] = 1;
    RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_DATA_PORT_0x0092, 3, ucOsdVbiCoreBuf, _NON_AUTOINC);
#endif

}

// Operating in Display/NonDisplay Memory
void	COsdVbiClearDisplayMemory(void)
{
    UINT8 ucRow = 0x00;
    UINT8 cnt, cnt1;

    if (ucOsdVbiRollupNumber == 0 && GET_OSDVBI_SHOWMEMORYBLOCK())
        ucRow = _OSDVBI_DOUBLEBUFFER_ROW;

#if(VBIOSD_512FONT_UNSUPPORT==_TRUE)
    ucOsdVbiCoreBuf[0] = 0x80;//0x84
#else
    ucOsdVbiCoreBuf[0] = 0x88;//0x8c
#endif
    ucOsdVbiCoreBuf[1] = 0x00;
    ucOsdVbiCoreBuf[2] = 0x10;

    rtdf_pageSelect(_PAGE0);

    for (cnt = 1; cnt < _OSDVBI_DOUBLEBUFFER_ROW; cnt++)
    {
        COsdVbiStartAddress(ucRow + cnt -1, 0, _WRITE_BYTEALL);

        for (cnt1 = 0; cnt1 < _OSDVBI_ROW_LEN_INCOLUMNSPACE; cnt1++)
            RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_DATA_PORT_0x0092, 3, ucOsdVbiCoreBuf, _NON_AUTOINC);
    }
}

void	COsdVbiClearNonDisplayMemory(void)
{
    UINT8 ucRow = 0x00;
    UINT8 cnt, cnt1;

    if (!GET_OSDVBI_SHOWMEMORYBLOCK())
        ucRow = _OSDVBI_DOUBLEBUFFER_ROW;

    rtdf_pageSelect(_PAGE0);

#if(VBIOSD_512FONT_UNSUPPORT==_TRUE)
    ucOsdVbiCoreBuf[0] = 0x80;//0x84
#else
    ucOsdVbiCoreBuf[0] = 0x88;//0x8c
#endif
    ucOsdVbiCoreBuf[1] = 0x00;
    ucOsdVbiCoreBuf[2] = 0x10;

    for (cnt = 0; cnt < _OSDVBI_DOUBLEBUFFER_ROW; cnt++)
    {
        COsdVbiStartAddress(ucRow + cnt - 1, 0, _WRITE_BYTEALL);

        for (cnt1 = 0; cnt1 < _OSDVBI_ROW_LEN_INCOLUMNSPACE; cnt1++)
            RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_DATA_PORT_0x0092, 3, ucOsdVbiCoreBuf, _NON_AUTOINC);
    }
}

void COsdVbiSwitchDisplayMemory(void)
{
    UINT16	usTmp;

    if (GET_OSDVBI_SHOWMEMORYBLOCK())
    {
        CLR_OSDVBI_SHOWMEMORYBLOCK();
        usTmp = _OSDVBI_ROWCOUNT + 1;
    }
    else
    {
        SET_OSDVBI_SHOWMEMORYBLOCK();
        usTmp = _OSDVBI_ROWCOUNT + 1 + _OSDVBI_ROW_LEN_INCOLUMNSPACE * _OSDVBI_DOUBLEBUFFER_ROW;
    }
    ucOsdVbiCoreBuf[0] = 0xc0;
    ucOsdVbiCoreBuf[1] = 0x04;

    ucOsdVbiCoreBuf[2] = usTmp & 0x00ff;
    ucOsdVbiCoreBuf[3] = ((usTmp & 0x0f00) >> 4) | ((_OSDVBI_FONTTABLE_STARTADDRESS) & 0x000f);
    ucOsdVbiCoreBuf[4] = ((_OSDVBI_FONTTABLE_STARTADDRESS) & 0x0ff0) >> 4;

    rtdf_pageSelect(_PAGE0);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, ucOsdVbiCoreBuf[0]);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091,  ucOsdVbiCoreBuf[1]);
    RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_DATA_PORT_0x0092, 3, &ucOsdVbiCoreBuf[2], _NON_AUTOINC);

    COsdVbiShowData(1);
}

void	COsdVbiClearAllMemory(void)
{
    UINT16 cnt, StopPoint;

    rtdf_pageSelect(0);
    COsdVbiSendOsdAddress(0xD000 + _OSDVBI_ROWCOUNT + 1);

#define usOsdRamMax (_OSDVBI_ROW_NUMBER_INCOLUMNSPACE * _OSDVBI_ROW_LEN_INCOLUMNSPACE)
///////////////////////////////////
//Limitation is about 700 for One CC data processing
///////////////////////////////////
    StopPoint = usOsdRamMax/2;

#if(VBIOSD_512FONT_UNSUPPORT==_TRUE)
    ucOsdVbiCoreBuf[0] = 0x80;//0x84
#else
    ucOsdVbiCoreBuf[0] = 0x88;//0x8c
#endif
    ucOsdVbiCoreBuf[1] = 0x00;
    ucOsdVbiCoreBuf[2] = 0x10;

    for (cnt = 0; cnt < StopPoint; cnt++)
        RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_DATA_PORT_0x0092, 3, ucOsdVbiCoreBuf, _NON_AUTOINC);

    COsdVbiSendOsdAddress(0xD000 + _OSDVBI_ROWCOUNT + 1+ StopPoint);
    //////////////////////////////////////////////
    for (cnt = 0; cnt < StopPoint; cnt++)
        RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_DATA_PORT_0x0092, 3, ucOsdVbiCoreBuf, _NON_AUTOINC);

#undef usOsdRamMax
}

// Clear all data in a row
void COsdVbiClearRow(UINT8 row)
{
    UINT8 cnt;

    rtdf_pageSelect(0);
    if (ucOsdVbiRollupNumber)
        COsdVbiStartAddress(ucOsdVbiColSpaceStartRow + row, 0, _WRITE_BYTEALL);
    else
        COsdVbiStartAddress(ucOsdVbiColSpaceStartRow + row - 1, 0, _WRITE_BYTEALL);

#if(VBIOSD_512FONT_UNSUPPORT==_TRUE)
    ucOsdVbiCoreBuf[0] = 0x80;//0x84
#else
    ucOsdVbiCoreBuf[0] = 0x88;//0x8c
#endif
    ucOsdVbiCoreBuf[1] = 0x00;
    ucOsdVbiCoreBuf[2] = 0x10;
    for (cnt = 0; cnt < _OSDVBI_ROW_LEN_INCOLUMNSPACE; cnt++)
        RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_DATA_PORT_0x0092, 3, ucOsdVbiCoreBuf, _NON_AUTOINC);

    ucOsdVbiShowRowFlag &= ~(1 << row);
}

// Set which row to be showed
void COsdVbiSetShowRow(UINT8 ucStartRow, UINT8 ucNumber)
{
    UINT8 cnt;

    if ((ucStartRow +  ucNumber - 1) >= _OSDVBI_ROWCOUNT)
        return;

    //[0xc0:vbi osd function enable];[0x80:vbi osd function Disable]
    ucOsdVbiCoreBuf[0] = 0xc0 | (_OSDVBI_CHARACTER_DOUBLE_WIDTH  << 1) | _OSDVBI_CHARACTER_DOUBLE_HEIGHT;
    ucOsdVbiCoreBuf[1] = ((_OSDVBI_ROWHEIGNT - 1) << 3) | _OSDVBI_CHARACTERSPACE;

    // Row command address
    rtdf_pageSelect(0);
    COsdVbiSendOsdAddress(0xD000);
    for (cnt = 0; cnt < _OSDVBI_ROWCOUNT; cnt++)
    {
        if ((cnt >= ucStartRow) && (cnt < (ucStartRow + ucNumber)))
            ucOsdVbiCoreBuf[2] =  _OSDVBI_ROW_LEN_INCOLUMNSPACE;
        else
            ucOsdVbiCoreBuf[2] =  0x00;
        RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_DATA_PORT_0x0092, 3, ucOsdVbiCoreBuf, _NON_AUTOINC);
    }

    ucOsdVbiCoreBuf[0] = 0x00;
    ucOsdVbiCoreBuf[1] = 0x00;
    ucOsdVbiCoreBuf[2] = 0x00;
    RtdCtrl_CScalerWriteScalerPort(UIOSD_OSD_DATA_PORT_0x0092, 3, ucOsdVbiCoreBuf, _NON_AUTOINC);

}

void COsdVbiStartAddress(UINT8 ucRow, UINT8 ucCol, UINT8 ucIndicate)
{
    UINT16 usStartAddress = 0;

    usStartAddress = _OSDVBI_ROWCOUNT + 1;
    usStartAddress += ((ucRow * _OSDVBI_ROW_LEN_INCOLUMNSPACE) + ucCol);
    usStartAddress |= (((ucIndicate << 2) + 1) << 12);
    COsdVbiSendOsdAddress(usStartAddress);

}

void CosdVbiDisableShow(void)
{
    CaptionMode = 0;//jerry0725
    COsdVbiClearAllMemory();
    COsdVbiShowData(0);
}

#endif



