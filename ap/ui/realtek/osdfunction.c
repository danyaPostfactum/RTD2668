/*===========================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2009
 * All rights reserved.
 * ========================================================== */

/*==================== File Description ===================== */
/**
 * @file
 * 	This file is osd implement for osd related functions.
 *
 * @author 	$Author: cf.tsai $
 * @date 	$Date: 2009-03-09 $
 * @version $Revision: 10 $
 */

/**
 * @addtogroup osd
 * @{
 */

#define _OSDFUNCTION_C
/*==================== Module dependency  ======================== */
#include "autoconf.h"
#include "sparrow.h"
#include "rosprintf.h"
#include "reg52.h"
#include "rtd_types.h"
#include "flow_scaler.h"
#include "osdregdef.h"
#include "osdobjectdef.h"
#include "osdfont.h"
#include "osdparameter.h"
#include "osdfunction.h"
#include "sparrow\imem.h"
#include "spi_dma.h"
#ifdef CONFIG_VBI_CC_SUPPORT
#include "osdvbi.h"
#endif

/*==================== Private Types ============================= */


/*==================== Definitions =============================== */
#define  _OSD_VERTICAL_DELAY		(0x04)
#define  _OSD_HORIZONTAL_DELAY		(0x22)



/*==================== Variables ================================= */
UINT8 xdata uwWindowCtrlByte2[10];
UINT8 xdata uwWindowCtrlByte1[10];
UINT8 xdata ucOsdRowMap = 0x00;
UINT8 xdata ucOsdData[3];

UINT8 xdata vlcData[16];
UINT8 xdata ucVLDCnt, ucVLDTemp;
UINT8 code *pvldarray;

UINT8 xdata uc2BitColor_Palette = 0;
UINT8 xdata uc2BitColor[4];
UINT16 xdata usFontSelectAddr = 0;

/*==================== Macros ==================================== */
#if 0
#define WRITE_TO_OSDDATAPORT()		{\
			rtdf_writeScalerPort(UIOSD_OSD_DATA_PORT_0x0092, ucOsdData[0]);\
			ap_osdfx_delayXms(10);\
			rtdf_writeScalerPort(UIOSD_OSD_DATA_PORT_0x0092, ucOsdData[1]);\
			ap_osdfx_delayXms(10);\
			rtdf_writeScalerPort(UIOSD_OSD_DATA_PORT_0x0092, ucOsdData[2]);\
			ap_osdfx_delayXms(10);\
			}
#else
#if 1
#define WRITE_TO_OSDDATAPORT()	RtdCtrl_CScalerWriteScalerPort(0x0092, 3, ucOsdData, 1);
#else
#define WRITE_TO_OSDDATAPORT()		{\
			rtdf_writeScalerPort(UIOSD_OSD_DATA_PORT_0x0092, ucOsdData[0]);\
			rtdf_writeScalerPort(UIOSD_OSD_DATA_PORT_0x0092, ucOsdData[1]);\
			rtdf_writeScalerPort(UIOSD_OSD_DATA_PORT_0x0092, ucOsdData[2]);\
			}
#endif
#endif
/*==================== Local Functions Phototype ==================*/



/*==================== Functions ================================= */

/*============================================================================*/

/**
 * To write same value into OSD data port.
 * @return none.
 */
void ap_osdfx_write_amount(UINT8 value, UINT8 len)
{
    //rtdf_pageSelect(0);
    while (len--)
        rtdf_writeScalerPort(UIOSD_OSD_DATA_PORT_0x0092, value);
}

#if 0	// no used
/**
 * OSD delay function.
 *
 */
void ap_osdfx_delayXms(UINT16 usMs)
{
    if ( usMs == 0)
        return;
    while (usMs--);
}

/**
 * To write array data into OSD data port.
 * @return none.
 */
void ap_osdfx_write_array(UINT8 *pArray, UINT8 len)
{
    while (len--)
    {
        rtdf_outb(UIOSD_OSD_DATA_PORT_0x0092, *pArray);
        pArray = pArray + 1;
    }
}
#endif

/**
 * To write a formatted data into OSD regs.
 * @return none.
 */
void ap_osdfx_code_write(UINT8 code *pArray)
{
    rtdf_pageSelect(0);
    while (_TRUE)
    {
        if ( *pArray == _END_)
            return;

        rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, *pArray);
        rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, *(pArray+1));
        pArray = pArray + 2;

        do
        {
            if (*(pArray+1) == _REPEAT_)
            {
                ap_osdfx_write_amount(*pArray, *(pArray+2));
                pArray = pArray + 3;
            }
            else
            {
                rtdf_writeScalerPort(UIOSD_OSD_DATA_PORT_0x0092, *pArray);
                pArray = pArray + 1;
            }
        }
        while (*pArray != _NEXT_);
        pArray = pArray + 1;
    }
}


#if 0
/**
 * To write data into OSD register.
 *
 */
void ap_osdfx_frame_write(UINT16 usAddr, UINT8 ucData, UINT8 ucIndicate)
{
    UINT8 addr0;
    UINT8 addr1;

    if (ucIndicate == _WRITE_BYTE0)
        addr0 = 0x00;
    else if ( ucIndicate == _WRITE_BYTE1)
        addr0 = 0x40;
    else if ( ucIndicate == _WRITE_BYTE2)
        addr0 = 0x80;
    else
        addr0 = 0xc0;

    addr0 = addr0 | ((usAddr&0x0f00) >> 8);
    addr1 = (usAddr&0xff);
    rtdf_pageSelect(0);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, addr0);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, addr1);
    rtdf_writeScalerPort(UIOSD_OSD_DATA_PORT_0x0092, ucData);
}


/**
 * To read data from OSD register.
 *
 */
UINT8 ap_osdfx_frame_read(UINT16 usAddr, UINT8 ucIndicate)
{
    UINT8 addr0;
    UINT8 addr1;
    UINT8 ucData;

    if (ucIndicate == _WRITE_BYTE0)
        addr0 = 0x00;
    else if ( ucIndicate == _WRITE_BYTE1)
        addr0 = 0x40;
    else if ( ucIndicate == _WRITE_BYTE2)
        addr0 = 0x80;
    else
        addr0 = 0xc0;

    addr0 = addr0 | ((usAddr&0x0f00) >> 8);
    addr1 = (usAddr&0xff);
    rtdf_pageSelect(0);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, addr0);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, addr1);
    ucData = rtdf_readScalerPort(UIOSD_OSD_DATA_PORT_0x0092);
    return ucData;
}
#endif

/**
 * To fill same value into specified OSD regs.
 * @param ucIndicate:	_WRITE_BYTE0
 *					_WRITE_BYTE1
 *					_WRITE_BYTE2
 *					_WRITE_BYTEALL
 * @return none.
 */
void COsdFxHLine(UINT8 ucRow, UINT8 ucCol, UINT8 ucLength, UINT8 ucValue, UINT8 ucIndicate)
{
    ap_osdfx_calc_start_addr(ucRow, ucCol, ucIndicate);

    if (ucIndicate == _WRITE_BYTEALL)
        ap_osdfx_write_amount(ucValue, ucLength*3);
    else
        ap_osdfx_write_amount(ucValue, ucLength);
}


/**
 * To load OSD palette LUT.
 * @return none.
 */
void ap_osdfx_load_palette(void)
{
    // OVERLAY_LUT_ADDR : 0xb810006E
    // COLOR_LUT_PORT   : 0xb810006F
    //	UINT8 j, k;

#if 1
    //	Modified by weihao : 990326
    //	Because of writting error palette color,
    //	we must operation in sequence of explaination in spec as followed:
    //
    //	Step:
    //	1. Disable Interrupt [EA = 0] -> Protect the flow from interrupt
    //   2. Enable Overlay Color Plate Access
    //   3. Use SPI DAM in order to copt color palette
    //   4. Disable Overlay Color Plate Access
    //	5. Enable Interrupt [EA = 0]
    EA = 0;
    rtdf_pageSelect(0);
    rtdf_writeScalerPort(0x6e, 0x80);
    drv_spi_dma_6093cpy	(0x6f, _BANK3, (UINT16)tOsd_Palette,  192, 1);
    rtdf_writeScalerPort(0x6e, 0x00);
    EA = 1;
#else
    rtdf_pageSelect(0);
    //	Modified by weihao : 990326
    //	Because of writting error palette color,
    //	we must operation in sequence of explaination in spec as followed:
    //
    //	While OSDON, enable double buffer write is necessary for modifying color palette LUT.
    //   Write [6] to be 1 and set LUT write address first
    //   Write 3 data R[7:0], G[7:0], B[7:0] to 6F
    //   Wait for [6] auto clear to be 0, then repeat the flow until the procedure is done
    // To do:
    // 1. Search the optimized flow to load color palette
#if 1
    // 64 color x RGB 3 bytes
    for (j=0; j<64; j++)
    {
        rtdf_writeScalerPort(0x6e, 0xc0 | j);
        for (k=0; k<3; k++)
            rtdf_writeScalerPort(0x6f, tOsd_Palette[j*3+k]);

        while (rtdf_readScalerPort(0x6e) & 0x40);
#if 0
        do
        {
            regVal = rtdf_readScalerPort(0x6e);
            //RTD_Log(LOGGER_INFO, "Wait DBuf...\n");
        }
        while ( (regVal&_BIT6) != 0);
        regVal |= _BIT6;
        tmp32 = regVal;
        RTD_Log(LOGGER_INFO, "regVal = %x\n", regVal);
        rtdf_writeScalerPort(0x6e, regVal);
#else
//		ap_osdfx_delayXms(10);
#endif
    }
#else
    drv_spi_dma_6093cpy	(0x6f, _BANK3, (UINT16)tOsd_Palette,  192, 1);
#endif
    rtdf_writeScalerPort(0x6e, 0x00);
#endif
}

#if 0 // Temporily not use
/**
 * To load a 16-color palette to specified LUT id.
 * @param ucPalette16Table: 16-color palette id
 * @return none.
 */
void ap_osdfx_load_palette16(UINT8 ucPalette16Table, UINT8* pColorPaletteArray)
{
    UINT8 j;
    UINT8 len;

    len = 48;	// 16 color x RGB 3 bytes
    ucPalette16Table = ucPalette16Table & 0x03;
    rtdf_pageSelect(0);
    rtdf_writeScalerPort(0x6e, (_BIT7 | (ucPalette16Table*16)));

    j = 0;
    while (len--)
    {
        rtdf_writeScalerPort(0x6f, *pColorPaletteArray);
        pColorPaletteArray = pColorPaletteArray + 1;
        j++;
        if ( j>= 3)
        {
            j = 0;
            ap_osdfx_delayXms(10);
        }
    }

    rtdf_writeScalerPort(0x6e, 0x00);
}
#endif

/**
 * To set OSD SRAM address.
 * @param	Font_Base: font base address
 *			Font_Select_Base: font select base address
 * @return none.
 */
void ap_osdfx_base_addr_set(UINT16 Font_Base, UINT16 Font_Select_Base)
{
    rtdf_pageSelect(0);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, 0xc0);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, 0x04);	// 0x004 : Base Address Offset

    ucOsdData[0] = (UINT8)Font_Select_Base;
    ucOsdData[1] = ((Font_Select_Base&0x0f00) >> 4) | (Font_Base&0x000f);
    ucOsdData[2] = (UINT8)(Font_Base >> 4);
    WRITE_TO_OSDDATAPORT();
}


/**
 * To clear OSD SRAM by filling with space character.
 * @return none.
 */
void ap_osdfx_setmap(void)
{
    UINT8 j;
    UINT16 usTotal;
    UINT8 code *pLut;

    // Base Address Offset Setting
    pLut = tRowLength_LUT_Set[ucOsdRowMap];
    j = 1;
    usTotal = 0;
    while ( pLut[j] != 0)
    {
        usTotal += pLut[j];
        j++;
    }

    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, (HIBYTE(usFontSelectAddr)&0x000f) | 0xd0);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, LOBYTE(usFontSelectAddr));

    ucOsdData[0] = 0x8c;
#if 0
    ucOsdData[1] = _x_;
    ucOsdData[2] = 0x10;
#else
    ucOsdData[1] = 0x00;
    ucOsdData[2] = 0x00;
#endif
    while (usTotal)
    {
        WRITE_TO_OSDDATAPORT();
        usTotal--;
    }
}


/**
 * To set row height(Ctrl byte 1).
 * @return none.
 */
void COsdFxSetRowCtrl1(UINT8 ucRow, UINT8 ucByte1)
{
    // Write start address to Addressing Port
    rtdf_pageSelect(0);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, 0x50);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, ucRow);

    rtdf_writeScalerPort(UIOSD_OSD_DATA_PORT_0x0092, ucByte1);
}

/**
 * To select OSD row command map.
 * @param	ucOsdmaptype: osd row command map id
 * @return none.
 */
void ap_osdfx_map_setting(UINT8 ucOsdmaptype)
{
    COsdFxDisableOsd();
    COsdFxCloseAllWindow();			// Close all window

    if ( ucOsdmaptype < _OSD_MAP_MAX)
    {
        ucOsdRowMap = ucOsdmaptype;

        //pLut = tRowLength_LUT_Set[ucOsdRowMap];
        usFontSelectAddr = tRowLength_LUT_Set[ucOsdRowMap][0] + 1;
        ap_osdfx_base_addr_set(_OSD_SRAM_FONT_DATA_START_ADDR, usFontSelectAddr);

        ap_osdfx_code_write(tOsdRowCmdSet[ucOsdmaptype]);
    }
    //else
    //	return;

    ap_osdfx_setmap();		// Clear all
}


/**
 * To set color of font shadow.
 *
 */
void ap_osdfx_font_shadow_color_set(UINT8 color)
{
    rtdf_pageSelect(0);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, 0x40);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, 0x03);
    ucOsdData[1] = rtdf_readScalerPort(UIOSD_OSD_DATA_PORT_0x0092) & 0x0f;
    ucOsdData[1] = ucOsdData[1] | ((color&0x0f)<<4);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, 0x40);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, 0x03);
    rtdf_writeScalerPort(UIOSD_OSD_DATA_PORT_0x0092, ucOsdData[1]);

    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, 0x80);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, 0x02);
    ucOsdData[2] = rtdf_readScalerPort(UIOSD_OSD_DATA_PORT_0x0092) & ~0x03;
    ucOsdData[2] = ucOsdData[2] | ((color&0x30)>>4);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, 0x80);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, 0x02);
    rtdf_writeScalerPort(UIOSD_OSD_DATA_PORT_0x0092, ucOsdData[2]);
}


/**
 * VLC Font decode API.
 *
 */
bit GetBit(void)
{
    ucVLDTemp = ((ucVLDCnt & 0x07) == 0) ? *(pvldarray++) : (ucVLDTemp << 1);

    ucVLDCnt += 1;

    return (bit)(ucVLDTemp & 0x80);
}

UINT8 Get_VLD(void)
{
    UINT8 zerocnt = 0;

    while (!GetBit())
        zerocnt  += 1;

    if (zerocnt == 0)		// Return First value in Vlc table if zero
        return *(vlcData);

    switch (zerocnt)
    {
    case 1:
        return (GetBit() ? *(vlcData + 1) : *(vlcData + 2));

    case 2:
        return (GetBit() ? *(vlcData + 3) : *(vlcData + 4));

    case 3:
        return (GetBit() ? *(vlcData + 5) : *(vlcData + 6));

    case 4:
        if (GetBit())
        {
            return (GetBit() ? *(vlcData + 7) : *(vlcData + 8));
        }
        else
        {
            if (GetBit())
            {
                return (GetBit() ? *(vlcData + 9) : *(vlcData + 10));
            }
            else
            {
                return (GetBit() ? *(vlcData + 11) : *(vlcData + 12));
            }
        }

    default:
        if (GetBit())
        {
            return (GetBit() ? *(vlcData + 13) : *(vlcData + 14));
        }
        else
        {
            GetBit();
            return *(vlcData + 15);
        }
    }
}


/**
 * To load VLC font to specified SRAM address.
 * @param	pArray:		VLC font data
 *			usStart:		SRAM address
 *			usLength:	length of font data
 * @return none.
 */
void ap_osdfx_load_abs_addr_vlc_font(UINT8 code *pArray, UINT16 usStart, UINT16 usLength)
{
    UINT8 temp0, temp1, temp2;
    rtdf_pageSelect(0);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, (HIBYTE(usStart)&0x000f) | 0xd0 );
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, LOBYTE(usStart));

    for (temp0=0; temp0<16; temp0++)
        vlcData[temp0] = *(pArray + temp0);

    ucVLDCnt = 0;
    ucVLDTemp = 0;
    pvldarray  = pArray + 16;
    do
    {
        for (usStart = 0; usStart < 9; usStart++)
        {
            temp0 = Get_VLD() << 4;
            temp0 |= Get_VLD();
            temp1 = Get_VLD()  << 4;
            temp1 |= Get_VLD();
            temp2 = Get_VLD()  << 4;
            temp2 |= Get_VLD();

            // Rearrange the byte order
            ucOsdData[0] = ((temp1&0x0f) << 4) | (temp2 & 0x0f);
            ucOsdData[1] = (temp2 & 0xf0) | (temp0 & 0x0f);
            ucOsdData[2] = (temp0 & 0xf0) | ((temp1&0xf0) >> 4);
            WRITE_TO_OSDDATAPORT();
        }
    }
    while (--usLength);

}


/**
 * To load VLC Font.
 * @return none.
 */
void ap_osdfx_load_vlc_font(UINT8 code *pArray, UINT16 usStart, UINT16 usLength)
{
    if (usLength == 0)
        return;

    usStart = usStart * 9;
    usStart += _OSD_SRAM_FONT_DATA_START_ADDR;
    ap_osdfx_load_abs_addr_vlc_font(pArray, usStart, usLength);
}


/**
 * To set SRAM share memory mode.
 * @return none.
 */
void ap_osdfx_set_share_memory(void)
{
    UINT8 getVal;

    getVal = rtdf_inb(0x2cc3);
    getVal |= _BIT7;
    rtdf_outb(0x2cc3, getVal);	// share_mem osd12k clock enable

//	rtdf_outb(0x2da1, 0x05);	// set share memory mode, mode 1:  TT, Display UI OSD;   TT Page 13KB, UI OSD 20KB
    // Set sys share memory to mode - 0
    drv_mem_SetShareMode(MEM_SHARE_MODE_1);
}

/**
 * To turn on the power of OSD circuit.
 * @return none.
 */
void ap_osdfx_power_on(UINT8 on)
{
    rtdf_pageSelect(0);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, 0x80);		// frame control regs, byte 2
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, 0x00);

    ucOsdData[0] = rtdf_readScalerPort(UIOSD_OSD_DATA_PORT_0x0092);
    if ( on == _ON)
        ucOsdData[0] |= _BIT0;
    else
        ucOsdData[0] &= ~_BIT0;

    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, 0x80);		// frame control regs, byte 2
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, 0x00);
    rtdf_writeScalerPort(UIOSD_OSD_DATA_PORT_0x0092, ucOsdData[0]);
}

/**
 * To display the OSD layer.
 * @return none.
 */
void COsdFxEnableOsd(void)
{
    // OVERLAY_CTRL : 0xb810006C
    //RTD_Log(LOGGER_LEVEL,"\n Tim ensable \n");
    rtdf_pageSelect(0);
    ucOsdData[0] = rtdf_readScalerPort(0x6c);
    ucOsdData[0] |= _BIT0; // Enable Overlay Port

    WaitFor_DEN_STOP();

    rtdf_writeScalerPort(0x6c, ucOsdData[0]);
}


/**
 * To hide the OSD layer.
 * @return none.
 */
void COsdFxDisableOsd(void)
{
	//RTD_Log(LOGGER_LEVEL,"\n Tim DIsable \n");
    rtdf_pageSelect(0);
    ucOsdData[0] = rtdf_readScalerPort(0x6c);
    ucOsdData[0] &= ~_BIT0;

    //WaitFor_DEN_STOP();

    rtdf_writeScalerPort(0x6c, ucOsdData[0]);
}


void ap_osdfx_set_blending_type(UINT8 type)
{
    type = type&0x03;

    rtdf_pageSelect(0);
    // frame control 3 byte 1
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, 0x40);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, 0x03);
    ucOsdData[1] = rtdf_readScalerPort(UIOSD_OSD_DATA_PORT_0x0092) & 0xf3;
    ucOsdData[1] = ucOsdData[1] | (type<<2);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, 0x40);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, 0x03);
    rtdf_writeScalerPort(UIOSD_OSD_DATA_PORT_0x0092, ucOsdData[1]);
}

/**
 * To set OSD blending level.
 * @return none.
 */
void COsdFxBlending(UINT8 ucLevel)
{
    ucLevel = (ucLevel & 0x07) << 2;	// [4:2]

    rtdf_pageSelect(0);
    ucOsdData[0] = rtdf_readScalerPort(0x6c) & ~(_BIT4 | _BIT3 | _BIT2);
    ucOsdData[0] |= ucLevel;

    WaitFor_DEN_STOP();

    rtdf_writeScalerPort(0x6c, ucOsdData[0]);

    ap_osdfx_set_blending_type(_OSD_BLENDING_ALL);
}


/**
 * To display OSD window by specified window style.
 * @param	pStyle:	window style struct.
 * @return none.
 */
void COsdFxDrawWindow(WindowStyle* pStyle)
{
    UINT8 j;

    rtdf_pageSelect(0);
    ucOsdData[0] = pStyle->ucWindowID << 2;		// x4
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, 0xc1);		// window control regs, all bytes
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, ucOsdData[0]);

    for (j=0; j<3; j++)
        rtdf_writeScalerPort(UIOSD_OSD_DATA_PORT_0x0092, pStyle->ucCtrlRegAddr0[j]);

    // Window Start Position
    ucOsdData[0] = ((pStyle->stPosition.usHStart&0x0f00) >> 4) | ((pStyle->stPosition.usVStart&0x0f00) >> 8);
    ucOsdData[1] = (pStyle->stPosition.usHStart&0xff);
    ucOsdData[2] = (pStyle->stPosition.usVStart&0xff);
    WRITE_TO_OSDDATAPORT();

    // Window End Position
    ucOsdData[0]= ((pStyle->stPosition.usHEnd&0x0f00) >> 4) | ((pStyle->stPosition.usVEnd&0x0f00) >> 8);
    ucOsdData[1] = LOBYTE(pStyle->stPosition.usHEnd);
    ucOsdData[2] = LOBYTE(pStyle->stPosition.usVEnd);
    WRITE_TO_OSDDATAPORT();

    for (j=0; j<3; j++)
        rtdf_writeScalerPort(UIOSD_OSD_DATA_PORT_0x0092, pStyle->ucCtrlRegAddr2[j]);

    uwWindowCtrlByte1[pStyle->ucWindowID] = pStyle->ucCtrlRegAddr2[1];
    uwWindowCtrlByte2[pStyle->ucWindowID] = pStyle->ucCtrlRegAddr2[2];
}


/**
 * To close window by specified window id.
 * @param	ucWinNumber:	window id
 * @return none.
 */
void COsdFxCloseWindow(UINT8 ucWinNumber)
{
    rtdf_pageSelect(0);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, 0x81);	// window control regs, byte 2
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, (ucWinNumber << 2) + 3);

    //WaitFor_DEN_STOP();
    rtdf_writeScalerPort(UIOSD_OSD_DATA_PORT_0x0092, 0x00);

    uwWindowCtrlByte2[ucWinNumber] = uwWindowCtrlByte2[ucWinNumber] & ~_BIT0;
}


/**
 * To close all OSD window.
 * @return none.
 */
void COsdFxCloseAllWindow(void)
{
    UINT8 j;
    for (j=0; j<10; j++)
        COsdFxCloseWindow(j);
}


/**
 * To move specified window to a new position.
 * @param	ucWinNumber:	window id
 *			stPos: new position
 * @return none.
 */
void COsdFxMoveWindow(UINT8 ucWinNumber, ItemPosition* pstPos)
{
    rtdf_pageSelect(0);
    ucOsdData[0] = (ucWinNumber << 2) + 1;
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, (0xc1 | _BIT5));	// _BIT5 : enable double buffer
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, ucOsdData[0]);

    WaitFor_DEN_STOP();

    // Window Start Position
    ucOsdData[0] = ((pstPos->usHStart&0x0f00) >> 4) | ((pstPos->usVStart&0x0f00) >> 8);
    ucOsdData[1]= LOBYTE(pstPos->usHStart);
    ucOsdData[2]= LOBYTE(pstPos->usVStart);
    WRITE_TO_OSDDATAPORT();

    // Window End Position
    ucOsdData[0] = ((pstPos->usHEnd&0x0f00) >> 4) | ((pstPos->usVEnd&0x0f00) >> 8);
    ucOsdData[1] = LOBYTE(pstPos->usHEnd);
    ucOsdData[2] = LOBYTE(pstPos->usVEnd);
    WRITE_TO_OSDDATAPORT();

    //WaitFor_DEN_STOP();
    //WaitFor_DEN_STOP();
}

#if 0
/**
 * To get the enable status of specified window.
 * ucWindowID : ID of window.
 * @return statue of window.
 */
UINT8 COsdFxGetWindowEnable(UINT8 ucWindowID)
{
    return (uwWindowCtrlByte2[ucWindowID]&_BIT0);
}
#endif

/**
 * To show the specified window.
 * @return none.
 */
void COsdFxShowWindow(UINT8 ucWindowID, UINT8 ucShow)
{
    if (ucShow)
    {
        if (ucShow & _BIT1)
            uwWindowCtrlByte2[ucWindowID] = uwWindowCtrlByte2[ucWindowID] | (_BIT4 | _BIT0);
        else
            uwWindowCtrlByte2[ucWindowID] = uwWindowCtrlByte2[ucWindowID] | (_BIT4 | _BIT0);
    }
    else
        uwWindowCtrlByte2[ucWindowID] = uwWindowCtrlByte2[ucWindowID] & ~(_BIT4 | _BIT0);

    rtdf_pageSelect(0);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, 0x81);	// window control regs, byte 2
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, (ucWindowID << 2) + 3);
    WaitFor_DEN_STOP();
    rtdf_writeScalerPort(UIOSD_OSD_DATA_PORT_0x0092, uwWindowCtrlByte2[ucWindowID]);
}

#ifdef _VIDEO_TV_SUPPORT
#if defined(_REALTEK_OSD_USE)

/**
 * To enable/disable window border effect.
 * @return none.
 */
void COsdFxOpenWindowEffect(UINT8 ucWindowID, UINT8 ucEnable)
{
    if (ucEnable)
        uwWindowCtrlByte2[ucWindowID] = uwWindowCtrlByte2[ucWindowID] | _BIT4;
    else
        uwWindowCtrlByte2[ucWindowID] = uwWindowCtrlByte2[ucWindowID] & ~_BIT4;

    rtdf_pageSelect(0);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, 0x81);	// window control regs, byte 2
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, (ucWindowID << 2) + 3);
    rtdf_writeScalerPort(UIOSD_OSD_DATA_PORT_0x0092, uwWindowCtrlByte2[ucWindowID]);
}
#endif
#endif
/**
 * To change window color of specified window ID
 * @return none.
void COsdFxChangeWindowColor(UINT8 ucWindowID, UINT8 ucColor)
{
	uwWindowCtrlByte1[ucWindowID] = (uwWindowCtrlByte1[ucWindowID]&0xf0) | (ucColor&0x0f);

	rtdf_pageSelect(0);
	rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, 0x41);	// window control regs, byte 1
	rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, (ucWindowID << 2) + 3);
	rtdf_writeScalerPort(UIOSD_OSD_DATA_PORT_0x0092, uwWindowCtrlByte1[ucWindowID]);
}
 */
/**
 * To set OSD HV delay.
 * @param	uiXPos: H delay
 			uiYPos: V delay
 * @return none.
 */
void COsdFxSetWinRefPos(UINT16 uiXPos, UINT16 uiYPos)
{
//	UINT8 tmp0;

    uiXPos = uiXPos & 0x3ff;	// 10 bits H delay
    if ( uiXPos < 2)
        uiXPos = 2;
    uiYPos = uiYPos & 0x7ff;	// 11 bits V delay
    if ( uiYPos < 1)
        uiYPos = 1;

    rtdf_pageSelect(0);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, (0xc0/* | _BIT5*/));	// _bit5 : enable double buffer
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, 0x00);

    ucOsdData[0] = (uiYPos >> 3)&0xff;
    ucOsdData[1] = (uiXPos >> 2)&0xff;
    ucOsdData[2] = ((uiXPos&0x0003) << 6) | ((uiYPos&0x0007) << 3) | 3;
    WRITE_TO_OSDDATAPORT();

    // wait double buffer write out
//	do{
//		tmp0 = rtdf_readScalerPort(UIOSD_OSD_SCRAMBLE_0x0093);
//	}while( (tmp0&_BIT4) != 0);
    //WaitFor_DEN_STOP();
}


/**
 * To display osd string.
 * @return none.
 */
UINT8 COsdFxLoadText(UINT8 ucRow, UINT8 ucCol, UINT16 uwBKLength, UINT8 ucColor, UINT8 ucSelect, UINT8 *pArray)
{
    UINT8 cnt, temp;
    UINT8 ucCount;
    UINT8* pPreArray;

    ucCount = 0;
    if (uwBKLength)
    {
        ucCount = COsdFxLoadBlankCmd(ucRow, ucCol, uwBKLength, _CP_BLACK);
        ucCol = ucCol + ucCount;
    }

    temp = 0;
    pPreArray = pArray;
    while (_TRUE)
    {
        if (*pArray == _END_)
        {
            if (temp == ucSelect)
            {
                pArray = pPreArray;
                break;
            }
            else
            {
                temp++;
                if (*(pArray + 1)  != _THE_SAME_)
                    pPreArray = pArray + 1;
            }
        }
        else if (*pArray ==_THE_SAME_)
        {
            if (temp == ucSelect)
            {
                pArray = pPreArray;
                break;
            }
            else
            {
                temp++;
                if (*(pArray + 1)  != _THE_SAME_)
                    pPreArray = pArray + 1;
            }
        }
        pArray++;
    }

    ap_osdfx_calc_start_addr(ucRow, ucCol, _WRITE_BYTEALL);

    while (*pArray != _END_)
    {
        temp = *pArray;
        pArray++;

        if (temp == _SPC_) 	// write blank command
        {
            ucOsdData[0] = _BIT4;
            ucOsdData[2] = 0x00;
            ucOsdData[1] = *pArray;
            WRITE_TO_OSDDATAPORT();
            ucCount++;
            pArray++;
            continue;
        }

        cnt = 0;
        cnt = ap_osdfx_font_width(temp);
        ucOsdData[0] = 0x80 | (cnt&0x0f);
        ucOsdData[1] = temp;
        //ucOsdData[2] = (ucFgColor&0x0f) << 4 | (ucBgColor&0x0f);
        ucOsdData[2] = ucColor;
        WRITE_TO_OSDDATAPORT();

        ucCount++;
    }

    return ucCount;
}


#if 0
/**
 * To calculate blank command count by pixel length.
 * @return the count of blank command.
 */
UINT8 COsdFxLoadFixedCountBlankCmd(UINT8 ucRow, UINT8 ucCol, UINT8 ucCount, UINT16 usLen, UINT8 ucBgColor)
{
    UINT8 quotient;
    UINT16 remainder;
    UINT8 ucData[3];
    UINT8 cnt;

    if (usLen <  4)
        usLen = 4;

    remainder = usLen;
    quotient = 0;
    while (remainder >= ucCount)
    {
        remainder -= ucCount;
        quotient++;
    }
    ucData[0] = 0;
    ucData[2] = (ucBgColor&0x0f);
    ap_osdfx_calc_start_addr(ucRow, ucCol, 0x03);
    for (cnt = 0; cnt < ucCount; cnt++)
    {
        if (quotient + remainder > 255)
        {
            ucData[1] = 255;
            remainder = quotient + remainder - 255;
        }
        else
        {
            ucData[1] = quotient + remainder;
            remainder = 0;
        }
        RtdCtrl_WriteByteSerial(OSDFONT_OSD_F_DATA_VADDR,_WRITEBYTE_OPT(3,_NON_INC,_BYTE_N3), ucData);

    }

    return ucCount;
}
#endif

/**
 * To calculate blank command count by pixel length.
 * @return the count of blank command.
 */
UINT8 COsdFxCalBlankCmdCount(UINT16 uwBKLength)
{
    UINT8 ucBKCount = 0;

    if ( uwBKLength < 4)
    {
        if (uwBKLength)
            uwBKLength = 4;
        else
            return 0;
    }

    ucBKCount = 1;
    while (uwBKLength > 255)
    {
        uwBKLength -= 255;
        ucBKCount += 1;
    }

    return ucBKCount;
}


/**
 * To load blank command.
 * @return the count of blank command.
 */
UINT8 COsdFxLoadBlankCmd(UINT8 ucRow, UINT8 ucCol, UINT16 uwLength, UINT8 ucBgColor)
{
#define remainder uwLength
    //UINT16 remainder;
    UINT8 cnt, quotient;
    UINT8 ucCount;

    if ( uwLength < 4)
    {
        if (uwLength)
            uwLength = 4;
        else
            return 0;
    }
    ucCount = COsdFxCalBlankCmdCount(uwLength);

    //remainder = uwLength;
    // +++
    // equal to 'uwLength divided by ucCount' and we will get quotient and remainder
    quotient = 0;
    while ( remainder >= ucCount)
    {
        remainder -= ucCount;
        quotient++;
    }
    // +++

    ap_osdfx_calc_start_addr(ucRow, ucCol, _WRITE_BYTEALL);

    ucOsdData[0] = _BIT4;
    ucOsdData[2] = ucBgColor&0x0f;
    for (cnt = 0; cnt < ucCount; cnt++)
    {
        if (quotient + remainder > 255)
        {	// can't exceed 255 pixels
            ucOsdData[1] = 255;
            remainder = (UINT16)quotient + remainder - 255;
        }
        else
        {
            ucOsdData[1] = quotient + remainder;
            remainder = 0;
        }
        WRITE_TO_OSDDATAPORT();
    }

    return ucCount;

#undef remainder
}


/**
 * To clear osd string.
 * @return none.
 */
void COsdFxClearText(UINT8 ucRow, UINT8 ucCol, UINT8 ucHeight, UINT8 ucWidth)
{
    UINT8 temp;
    UINT8 j;

    ucOsdData[0] = 0x8c;
    ucOsdData[1] = 0x00;
    ucOsdData[2] = 0x40;
    for (temp = 0 ; temp < ucHeight ; temp++)
    {
        ap_osdfx_calc_start_addr(ucRow+temp, ucCol, _WRITE_BYTEALL);
        for (j=0; j<ucWidth; j++)
        {
            WRITE_TO_OSDDATAPORT();
        }
    }
}

#if 0
/**
 * To change color of 1bit font by specified row and col.
 * @return none.
 */
void ap_osdfx_change_1bit_color(UINT8 ucRow, UINT8 ucCol, UINT8 ucSizeH, UINT8 ucSizeW, UINT8 ucColor)
{
#if 0
    UINT8 ucCnt;

    for (ucCnt = 0; ucCnt < ucSizeH; ucCnt++)
    {
        ap_osdfx_calc_start_addr(ucRow + ucCnt, ucCol, _WRITE_BYTE2);
        ap_osdfx_write_amount(ucColor, ucSizeW);
    }
#else
    while (ucSizeH)
    {
        ucSizeH--;
        ap_osdfx_calc_start_addr(ucRow + ucSizeH, ucCol, _WRITE_BYTE2);
        ap_osdfx_write_amount(ucColor, ucSizeW);
    }
#endif
}
#endif

#if 0 // Temporily not use
/**
 * To change color of 2bits font.
 * @return none.
 */
void COsdFxChange2bitColor(UINT8 ucRow, UINT8 ucCol, UINT8 ucSizeH, UINT8 ucSizeW)
{
#if 1
    UINT8 ucCnt;

    ucOsdData[0]= MAKE_2BITS_BYTE0(0);
    for (ucCnt=0; ucCnt < ucSizeH; ucCnt++)
    {
        ap_osdfx_calc_start_addr(ucRow, ucCol, _WRITE_BYTE0);
        ap_osdfx_write_amount(ucOsdData[0], ucSizeW);
    }
    ucOsdData[2] = MAKE_2BITS_BYTE2();
    for (ucCnt=0; ucCnt < ucSizeH; ucCnt++)
    {
        ap_osdfx_calc_start_addr(ucRow, ucCol, _WRITE_BYTE2);
        ap_osdfx_write_amount(ucOsdData[2], ucSizeW);
    }
#else
    ap_osdfx_calc_start_addr(ucRow, ucCol, _WRITE_BYTE0);
    //ucOsdData[0] = (uc2BitColor0&_BIT2) >> 2 | (uc2BitColor3&0x07) << 1 | (uc2BitColor0&0x08) << 3 | _BIT7 | _BIT5;
    osdData[0] = MAKE_2BITS_BYTE0(0);
    rtdf_outb(UIOSD_OSD_DATA_PORT_0x0092, osdData[0]);

    ap_osdfx_calc_start_addr(ucRow, ucCol, _WRITE_BYTE2);
    //ucOsdData[2] = (uc2BitColor0&0x03) << 6 | (uc2BitColor2&0x07) << 3 | (uc2BitColor1&0x07);
    osdData[2] = MAKE_2BITS_BYTE2();
    rtdf_outb(UIOSD_OSD_DATA_PORT_0x0092, osdData[2]);
#endif
}
#endif

/**
 * To display 1bit font.
 * @return none.
 */
void COsdFxWrite1BitFont_value(UINT8 ucFontSelect,UINT8 ucColor)
{
    ucOsdData[0] = 0x80 | 12; // width is 12
    ucOsdData[1] = ucFontSelect;
    ucOsdData[2] = ucColor;
    WRITE_TO_OSDDATAPORT();
}
void COsdFxWrite1BitFont(UINT8 ucRow, UINT8 ucCol, UINT8 ucFontSelect, UINT8 ucColor)
{
    ap_osdfx_calc_start_addr(ucRow, ucCol, _WRITE_BYTEALL);
    ucOsdData[0] = 0x80 | 12; // width is 12
    ucOsdData[1] = ucFontSelect;
    ucOsdData[2] = ucColor;
    WRITE_TO_OSDDATAPORT();
}

/**
 * To set the offset of 2bit font.
 * @return none.
 */
void ap_osdfx_set_2bit_font_offset(UINT8 ucOffset)
{
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, 0x40);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, 0x0b);	// 0x00b byte1 : 2bit font Base Address Offset
    rtdf_writeScalerPort(UIOSD_OSD_DATA_PORT_0x0092, (ucOffset&0x7f));
}

/**
 * To write 2bit font into OSD data port.
 * @return none.
void ap_osdfx_write_2bit_font_seq(UINT8 ucChar)
{
	ucOsdData[0] = MAKE_2BITS_BYTE0(0);
	ucOsdData[1] = ucChar&0x7f;
	ucOsdData[2] = MAKE_2BITS_BYTE2();
	WRITE_TO_OSDDATAPORT();
}
 */
/**
 * To display 2bits font.
 * @return none.
 */
void COsdFxWrite2BitFont(UINT8 ucRow, UINT8 ucCol, UINT8 ucChar)
{
    ap_osdfx_calc_start_addr(ucRow, ucCol, _WRITE_BYTEALL);
    ucOsdData[0] = MAKE_2BITS_BYTE0();//0xA6
    ucOsdData[1] = ucChar&0x7F;
    ucOsdData[2] = MAKE_2BITS_BYTE2();//0x11
    WRITE_TO_OSDDATAPORT();
}

/**
 *
 *
void COsdFxLoad2BitFontArray(UINT8 ucRow, UINT8 ucCol, UINT8 code *pArray)
{
	UINT8 temp, ucCnt;

	EA = 0;

	ap_osdfx_calc_start_addr(ucRow, ucCol, _WRITE_BYTEALL);

	while(*pArray != _END_){

		if ( *pArray == _SPC_) {// write blank command
			ucOsdData[0] = _BIT4;
			ucOsdData[2] = 0x00;
			ucOsdData[1] = *pArray;
		}
		else if ( *pArray == _REPEAT_) {
			temp = *(pArray-1);
			pArray++;
			ucCnt = *pArray - 1;
			ucOsdData[0] = MAKE_2BITS_BYTE0(0);
			ucOsdData[1] = temp&0x7f;
			ucOsdData[2] = MAKE_2BITS_BYTE2();
		}
		else {
			ucCnt = 1;
			temp = *pArray;
			ucOsdData[0] = MAKE_2BITS_BYTE0(0);
			ucOsdData[1] = temp&0x7f;
			ucOsdData[2] = MAKE_2BITS_BYTE2();
		}
		while(ucCnt--)
			WRITE_TO_OSDDATAPORT();
		pArray++;
	}

	EA = 1;
}
 */
#if 0
void COsdFxChange4bitColor(UINT8 ucRow, UINT8 ucCol, UINT8 ucSizeH, UINT8 ucSizeW)
{
    UINT8 cntY, cntx;
    UINT8 Data[3];

    Data[0] = 0x90;
    Data[1] = 0x00;
    Data[2] = 0x07;	//Use Color LUT index 7
    for (cntY = 0; cntY < ucSizeH; cntY++)
    {
        ap_osdfx_calc_start_addr(ucRow + cntY, ucCol, _WRITE_BYTEALL);
        for (cntx = 0; cntx < ucSizeW; cntx++)
        {
            RtdCtrl_WriteByteSerial(OSDFONT_OSD_F_DATA_VADDR,_WRITEBYTE_OPT(3,_NON_INC,_BYTE_N3), Data);
        }
    }
}
#endif

#if 1 //defined(CONFIG_MAINICON_4BITS_ANIMATION)
/**
 * To display 4bits font.
 * @return none.
 */
void COsdFxWrite4BitChar(UINT8 ucRow, UINT8 ucCol, UINT8 ucCharSelect, UINT8 ucBgColor, UINT8 ucLUT)
{
    ap_osdfx_calc_start_addr(ucRow, ucCol, _WRITE_BYTEALL);

    ucOsdData[0] = 0x90 | (ucBgColor&0x0f);
    ucOsdData[1] = ucCharSelect;
    // we force to use same palette LUT here...
    ucLUT = ucLUT&0x03;
    ucOsdData[2] = (ucLUT << 6) | (ucLUT << 4) | (ucLUT << 2) | ucLUT;

    WRITE_TO_OSDDATAPORT();
}
#endif

/**
 * To calculate SRAM font select start address by row and col.
 * @return none.
 */
void ap_osdfx_calc_start_addr(UINT8 ucRow, UINT8 ucCol, UINT8 ucIndicate)
{
    UINT16 startAddr;
    UINT8 j;
    UINT8 code *pLut;

    pLut = tRowLength_LUT_Set[ucOsdRowMap];
    startAddr = usFontSelectAddr;
    for (j=0; j<ucRow; j++)
        startAddr += pLut[j+1];
    startAddr += ucCol;

    j = 0x10 | ucIndicate << 6 | HIBYTE(startAddr);
    rtdf_pageSelect(0);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, j);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, LOBYTE(startAddr));
}


/**
 * To calculate char count of osd string.
 * @return char count of the string.
 */
UINT8 COsdFxCalTextCount(UINT8 ucSelect, UINT8 *pArray)
{
    UINT8 cnt, temp;
    UINT8 prevcnt = 0;

    cnt = 0;
    temp = 0;
    while (_TRUE)
    {
        if (*pArray == _END_)
        {
            if (temp == ucSelect)
                return cnt;
            else
            {
                prevcnt = cnt;
                cnt = 0;
                temp++;
            }
        }
        else if (*pArray ==_THE_SAME_)
        {
            if (temp == ucSelect)
                return prevcnt;
            else
            {
                cnt = 0;
                temp++;
            }
        }
        else
            cnt++;
        pArray++;
    }
}


/**
 * To calculate pixel width of osd string.
 * @return width of the string.
 */
UINT16 COsdFxCalTextWidth(UINT8 ucSelect, UINT8 *pArray)
{
    UINT8 temp = 0;
    UINT16 length = 0;
    UINT16 prevlength = 0;

    while (_TRUE)
    {
        if (*pArray == _END_)
        {
            if (temp == ucSelect)
                return length;
            else
            {
                prevlength = length;
                length = 0;
                temp++;
            }
        }
        else if (*pArray ==_THE_SAME_)
        {
            if (temp == ucSelect)
                return prevlength;
            else
            {
                length = 0;
                temp++;
            }
        }
        else
            length += ap_osdfx_font_width(*pArray);
        pArray++;
    }
}


/**
 * To get the width of specified font.
 * @return width of the font.
 */
UINT8 ap_osdfx_font_width(UINT8 ucFont)
{
    UINT8 ucwidth = 12;
    /*UINT8 count;*///For Keil Building
    if (ucFont>0x5F)
        return 12;
    ucwidth = tOSD_CHARWIDTH_TABLE[ucFont-0x00];
    return ucwidth;
}
#ifdef _VIDEO_TV_SUPPORT
/**
 * To get number width.
 * @return width.
 */
UINT8 COsdFxGetNumberwidth(UINT32 ucValue, UINT8 ucPar)
{
    // ucPar
    // BIT 0 (Align left or right)	[ 0: Align left		1:	Align right			]
    // BIT 1 (Assign show number)	[ 0: Disable 		1:	Enable (強 制 顯 示) ]
    // BIT 2 (Assign show %)		[ 0: Disable 		1:	Enable 顯示小數2位	]
    // BIT 4 ~ 6 (顯示位數)		[ 000: 顯示 1 位，以此類推					]
    UINT8 index;
    UINT8 j;
    UINT8 width;
    UINT8 totalwidth;
    UINT8 digiNum;
    //RTD_Log(LOGGER_DEBUG,"\nCOsdFxShowNumber = %d",(UINT32)ucValue);

    digiNum = ((ucPar>>4)&0x07) + 1;
    totalwidth = 0;
    if ((ucPar&_BIT2) != 0)
    {	
        if ( digiNum < 4)
            digiNum = 4;
    }
    for (j=0; j<digiNum; j++)
    {
        if ((ucPar&_BIT2) && j == 2)
            vlcData[j] = 0x5F;	// sign dot
        else
        {
            vlcData[j] = _0_ + (ucValue%10);
            ucValue /= 10;
        }
        if (ucValue == 0)
        {
            j++;
            break;
        }
    }
    index = j;
    for (; j<digiNum; j++)
        vlcData[j] = 0x00;

    if ( (ucPar&_BIT0) != 0)
    { // align right
        for (j=0; j<digiNum; j++)
        {
            ucOsdData[1] = vlcData[digiNum-1-j];
            width = ap_osdfx_font_width(ucOsdData[1]);
            totalwidth+=width;
        }
    }
    else
    { // align left
        for (j=0; j<digiNum; j++)
        {
            if ( j<=index)
                ucOsdData[1] = vlcData[index-j];
            else
                ucOsdData[1] = 0x00;
            width = ap_osdfx_font_width(ucOsdData[1]);
            totalwidth+=width;
        }
    }

    return totalwidth;
}
#endif
/**
 * To display number.
 * @return number counter.
 */
UINT8 COsdFxShowNumber(UINT8 ucRow, UINT8 ucCol, UINT32 ucValue, UINT8 ucPar, UINT8 ucColor)
{
    // ucPar
    // BIT 0 (Align left or right)	[ 0: Align left		1:	Align right			]
    // BIT 1 (Assign show number)	[ 0: Disable 		1:	Enable (強 制 顯 示) ]
    // BIT 2 (Assign show %)		[ 0: Disable 		1:	Enable 顯示小數2位	]
    // BIT 4 ~ 6 (顯示位數)		[ 000: 顯示 1 位，以此類推					]
    UINT8 index;
    UINT8 j;
    UINT8 digiNum;
    //RTD_Log(LOGGER_DEBUG,"\nCOsdFxShowNumber = %d",(UINT32)ucValue);

    digiNum = ((ucPar>>4)&0x07) + 1;
    if ((ucPar&_BIT2) != 0)
    {	// 指定小數表示時，包含點號最少 4 個位數
        if ( digiNum < 4)
            digiNum = 4;
    }
    for (j=0; j<digiNum; j++)
    {
        if ((ucPar&_BIT2) && j == 2)
            vlcData[j] = 0x5F;	// sign dot
        else
        {
            vlcData[j] = _0_ + (ucValue%10);
            ucValue /= 10;
        }
        if (ucValue == 0)
        {
            j++;
            vlcData[j] = 0x00;
            break;
        }
    }
    index = j;
    for (; j<digiNum; j++)
        vlcData[j] = 0x00;
	OSDLine(ucRow, ucCol, digiNum,0x00,_WRITE_BYTE1);
    ap_osdfx_calc_start_addr(ucRow, ucCol, _WRITE_BYTEALL);

    ucOsdData[0] = 0x80 | 12;
    ucOsdData[2] = (ucColor << 4) | _CP_BLACK;
    if ( (ucPar&_BIT0) != 0)
    { // align right
        for (j=0; j<digiNum; j++)
        {
            ucOsdData[1] = vlcData[digiNum-1-j];
            if(ucOsdData[1]!=0)
            	WRITE_TO_OSDDATAPORT();
        }
    }
    else
    { // align left
        for (j=0; j<digiNum; j++)
        {
            if ( j<=index)
                ucOsdData[1] = vlcData[index-j];
            else
                ucOsdData[1] = 0x00;
            if(ucOsdData[1]!=0)    
            	WRITE_TO_OSDDATAPORT();
        }
    }

    return index;
}

#ifdef CONFIG_VBI_CC_SUPPORT
void ap_osdfx_load_vbipalette(void)
{
    // OVERLAY_LUT_ADDR : 0xb810006E
    // COLOR_LUT_PORT   : 0xb810006F
    UINT8 j, k;

    rtdf_pageSelect(0);

    //	Modified by weihao : 990326
    //	Because of writting error palette color,
    //	we must operation in sequence of explaination in spec as followed:
    //
    //	While OSDON, enable double buffer write is necessary for modifying color palette LUT.
    //   Write [6] to be 1 and set LUT write address first
    //   Write 3 data R[7:0], G[7:0], B[7:0] to 6F
    //   Wait for [6] auto clear to be 0, then repeat the flow until the procedure is done
    // To do:
    // 1. Search the optimized flow to load color palette
    // 64 color x RGB 3 bytes
    for (j=0; j<16; j++)
    {
        rtdf_writeScalerPort(0x6e, 0xc0 | j);
        for (k=0; k<3; k++)
            rtdf_writeScalerPort(0x6f, Palette_VBI[j*3+k]);

        while (rtdf_readScalerPort(0x6e) & 0x40);
    }

    rtdf_writeScalerPort(0x6e, 0x00);
}

void ap_osdfx_load_Vbi_vlcfont(UINT8 code *pArray, UINT16 usStart, UINT16 usLength)
{
    if (usLength == 0)
        return;

    usStart = usStart * 9;
    usStart += _OSDVBI_FONTTABLE_STARTADDRESS;//_OSD_SRAM_FONT_DATA_START_ADDR;
    ap_osdfx_load_abs_addr_vlc_font(pArray, usStart, usLength);
}

void ap_osdfx_reload_osdpalette(void)
{
    UINT8 j, k;

    rtdf_pageSelect(0);

    //	Modified by weihao : 990326
    //	Because of writting error palette color,
    //	we must operation in sequence of explaination in spec as followed:
    //
    //	While OSDON, enable double buffer write is necessary for modifying color palette LUT.
    //   Write [6] to be 1 and set LUT write address first
    //   Write 3 data R[7:0], G[7:0], B[7:0] to 6F
    //   Wait for [6] auto clear to be 0, then repeat the flow until the procedure is done
    // To do:
    // 1. Search the optimized flow to load color palette
    // 64 color x RGB 3 bytes
    for (j=0; j<16; j++)
    {
        rtdf_writeScalerPort(0x6e, 0xc0 | j);
        for (k=0; k<3; k++)
            rtdf_writeScalerPort(0x6f, tOsd_Palette[j*3+k]);

        while (rtdf_readScalerPort(0x6e) & 0x40);
    }

    rtdf_writeScalerPort(0x6e, 0x00);
}

#endif



#if 0
/**
 * To copy osd string.
 * @return none.
 */
void ap_osdfx_str_cpy(UINT8 *destStr, UINT8 *srcStr, UINT8 length)
{
    while (length)
    {
        length--;
        destStr[length] = srcStr[length];
    }
}
#endif

UINT8 CGetOsdRowLength(UINT8 Row)
{
    return  tRowLength_LUT_Set[ucOsdRowMap][Row + 1];
}
void SetOSDDouble(UINT8 ucAttr)
{
    ucAttr = ucAttr&0x03;

    rtdf_pageSelect(0);
    // frame control 3 byte 1
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, 0x40);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, 0x03);
    ucOsdData[1] = rtdf_readScalerPort(UIOSD_OSD_DATA_PORT_0x0092) & 0xfC;
    ucOsdData[1] = ucOsdData[1] | ucAttr;
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_MSB_0x0090, 0x40);
    rtdf_writeScalerPort(UIOSD_OSD_ADDR_LSB_0x0091, 0x03);
    rtdf_writeScalerPort(UIOSD_OSD_DATA_PORT_0x0092, ucOsdData[1]);
}
//---------------------------------------------------------------------------
void OSDClear(UINT8 row_start, UINT8 height,
              UINT8 col_start, UINT8 width,
              UINT8 Value, UINT8 indicate)
{          
    if (height)
    {
        do
        {
            OSDLine(row_start, col_start, width, Value, indicate);
            row_start++;
        }
        while (--height);
    }
}

void OSDLine(UINT8 row, UINT8 col, UINT8 length, UINT8 value, UINT8 TheByte)
{

    UINT8 ucRowCharLength,j;
    ucRowCharLength	= CGetOsdRowLength(row);

    if (ucRowCharLength < (col + length))
    {
        length = ucRowCharLength - col;
    }

    ap_osdfx_calc_start_addr(row, col, TheByte);

    if (TheByte == _WRITE_BYTEALL)
        length *= 3;

    for (j=0; j<length; j++)
    {
        RtdCtrl_CScalerSendAddr(0x0092,  value,  1);
    }

}
void DrawLeftLine(void)
{
    UINT8 i;
    ucOsdData[0]    = 0xA6;
    ucOsdData[1]    = 0xA5;
    ucOsdData[2]    = 0x11;

    for (i=0; i<12; i++)
    {
        ap_osdfx_calc_start_addr(4+i,  5, _WRITE_BYTEALL);
        WRITE_TO_OSDDATAPORT();
    }
}

void Draw2bit3x2Icon(UINT8*tIcon,UINT8 row,UINT8 col)
{
    UINT8 i;

    ucOsdData[0] = tIcon[0];
    ucOsdData[1] = tIcon[1];
    ucOsdData[2] = tIcon[2];

    for (i=0; i<3; i++)
    {
        ucOsdData[1] = tIcon[1]+i;
        ap_osdfx_calc_start_addr(row,  col+i, _WRITE_BYTEALL);
        WRITE_TO_OSDDATAPORT();
    }
    for (i=0; i<3; i++)
    {
        ucOsdData[1] = tIcon[1]+i+3;
        ap_osdfx_calc_start_addr(row+1,  col+i, _WRITE_BYTEALL);
        WRITE_TO_OSDDATAPORT();
    }

}

