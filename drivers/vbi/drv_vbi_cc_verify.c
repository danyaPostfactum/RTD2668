/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     vbi_cc_verify.c
 * @brief    Vertical Blanking Interval (VBI) slicer verification for
 *             Closed Caption (CC).
 * @author   Janus Cheng
 * @date     Jul. 24, 2009
 * @version  0.1
 * @ingroup  vbi
\*****************************************************************************/

#include <autoconf.h> // Provides: CONFIG_VBI_CC_SUPPORT
#include <rosprintf.h> // Provides: RTD_Log()

#ifdef CONFIG_VBI_CC_SUPPORT

#include <rtd_types.h> // Provides: UINT8 and UINT32
#include "char\uart.h"
#include "pcb_customer.h"
#include <stdarg.h>
#include <reg_def.h>
#include <sparrow.h>
#include <isr_utility.h> // Provides: RT_ISR_HANDLED, RT_ISR_UNHANDLED
#include "closecaption.h"
UINT8 xdata SpaceComEnable =0;//TOMMY ADD FOR cc space command process


/* -- Prototypes of Private Functions ---------------------------------------*/
//static void CC_Log(char code *format, ...);

/* -- Implementations of Public Functions -----------------------------------*/

/*---------------------------------------------------------------------------*/
void drvif_vbi_cc_verify_init (void)
/*****************************************************************************\
 *  @brief   VBI slicer ISR for VBI CC verification.
\*****************************************************************************/
{
    RTD_Log(LOGGER_INFO, "initial CC \n");

    rtdf_setBits(SYS_CLKEN_0_0x2cc0, _BIT0 ); // enable vbi clk

    // vbi_slicer_1_vd_1_CC_normal_mode.tbl
    {
        rtd_outl(0x5800, 0x01600022);
        rtd_outl(0x5a00, 0x005a2717);  //  auto adjust slicer level
        rtd_outl(0x5a60, 0x2050a023);  // bit 29  set that data from vd put through 16 tape low pass filter
        rtd_outl(0x5a74, 0x0f141110);  //  resample data don't do "rounding"

        rtd_outl(0x5000, 0x00000003);
        rtd_outl(0x5304, 0x00000001);
        rtd_outl(0x5308, 0x00ffff23);
        rtd_outl(0x530c, 0x2a021515);
        rtd_outl(0x5310, 0x15151515);

        rtd_outl(0x5310, 0x20202043);
        rtd_outl(0x5314, 0xaec4aed5);
        rtd_outl(0x5318, 0xae865445);
        rtd_outl(0x531c, 0xd35420d0);
        rtd_outl(0x5320, 0xc1c74520);
        rtd_outl(0x5324, 0x31202002);
        rtd_outl(0x5328, 0x2032b0b0);
        rtd_outl(0x532c, 0x20202020);
        rtd_outl(0x5220, 0x00170005);
        rtd_outl(0x5224, 0x0150013e);

        rtd_outl(0x5800, 0x90000020);

        // added by hsliao 20091021, VBI should follow VDC's "inverse" setting
        if ( rtd_inl(VDC_VSYNC_TIME_CONST_0x515c) & _BIT7 )
            rtdf_setBits(VBISL00X1803_0800_0x5800, _BIT5);
        else
            rtdf_clearBits(VBISL00X1803_0800_0x5800, _BIT5);

        rtd_outl(0x5a00, 0x006c2713);       // fix slicer level =6C
        rtd_outl(0x5a08, 0x00000000);  //line 9,10,11,12
        rtd_outl(0x5a0c, 0x00000000);
        rtd_outl(0x5a10, 0x00000000);  //line 17,18,19,20
        rtd_outl(0x5a14, 0x00001100);  //line 21,22,23,24
        rtd_outl(0x5a20, 0x00001ad0);
        rtd_outl(0x5a68, 0x005a0100);
        rtd_outl(0x5a6c, 0x00000000);
        rtd_outl(0x5a74, 0x0f043110);
        rtd_outl(0x5a78, 0x0f001000);
        rtd_outl(0x5bc0, 0x80000009);  // vbi_slicer_1	 debug mode
    }

    RTD_Log(LOGGER_INFO, "VBI Slicer Setting Finish \n");

    rtdf_setBits( 0x5A30, _BIT29 ); // enable CC interrupt
}

/*---------------------------------------------------------------------------*/
void drvif_vbi_cc_verify_play (void)
{
    // We have to set clk before vbi, so please do not change following setting order.

    rtdf_maskl(SYS_CLKSEL_0x2ccc, ~_BIT0, 0); // VBI clk from Video Decoder
    rtd_outl(VBISL00X1803_0800_0x5800, 0x90000000); // ADC source from Video Decode

    // added by hsliao 20091020, VBI should follow VDC's "inverse" setting
    if ( rtd_inl(VDC_VSYNC_TIME_CONST_0x515c) & _BIT7 )
        rtdf_setBits(VBISL00X1803_0800_0x5800, _BIT5);
    else
        rtdf_clearBits(VBISL00X1803_0800_0x5800, _BIT5);

    //rtdf_maskl(SYS_CLKSEL_0x2ccc, ~_BIT0, 0); // VBI clk from Video Decoder
    rtdf_maskl(ADC_CLKGEN_RGB_CAPTURE_0x043c, ~_BIT2, 0); // reset vbi clk to default
    rtd_outl(TT_VBI_DATA_TYPE_CONFIG_REG_FOR_LINE_21_240X1803_0A14_0x5a14, 0x00001100); // line 22
    rtd_outl(TT_VBI_CC_CTRL00X1803_0804_0x5804, 0xC7041000); // reset  data sift and swing
    //rtd_outl(TT_VBI_DTO_REGISTER10X1803_0A20_0x5a20, 0x0f911b00); // reset DTO rate
    rtd_outl(TT_VBI_DTO_REGISTER10X1803_0A20_0x5a20, 0x0F911AD0); // reset DTO rate

//#ifdef CONFIG_VBI_CC_SUPPORT
    CCcEnable();
//#endif
}

/*---------------------------------------------------------------------------*/
void drvif_vbi_cc_verify_play_ypbpr (void)
{
    // We have to set clk before vbi, so please do not change following setting order.

    rtd_outl(VBISL00X1803_0800_0x5800, 0x90000000); // vd source, then we can change following registers

    // added by hsliao 20091020, VBI should follow VDC's "inverse" setting
    if ( rtd_inl(VDC_VSYNC_TIME_CONST_0x515c) & _BIT7 )
        rtdf_setBits(VBISL00X1803_0800_0x5800, _BIT5);
    else
        rtdf_clearBits(VBISL00X1803_0800_0x5800, _BIT5);

    rtd_outl(TT_VBI_DATA_TYPE_CONFIG_REG_FOR_LINE_21_240X1803_0A14_0x5a14, 0x00000011); //line 21
    rtd_outl(TT_VBI_CC_CTRL00X1803_0804_0x5804, 0xC7020200); // set data sift and swing
    rtd_outl(TT_VBI_DTO_REGISTER10X1803_0A20_0x5a20, 0x000035a0); // change DTO rate
    rtd_outl(VBISL00X1803_0800_0x5800, 0x900000b0); // Ypp source , we should inverse field bit4 bit5
    rtdf_maskl(SYS_CLKSEL_0x2ccc, ~_BIT0, _BIT0); // VBI clk from YPP
    rtdf_maskl(ADC_CLKGEN_RGB_CAPTURE_0x043c, ~_BIT2, _BIT2); // vbi clk use adc clk x2

//#ifdef CONFIG_VBI_CC_SUPPORT
    CCcEnable();
//#endif
}

/*---------------------------------------------------------------------------*/
bit drvif_vbi_cc_verify_isr (void)
/*****************************************************************************\
 *  @brief   VBI slicer ISR for VBI CC verification.
\*****************************************************************************/
{
    UINT32 regCCData;
    UINT8 field_type;
    //static UINT16 count=0;

    if (CVbiIsrDisable()==_TRUE)
        return RT_ISR_UNHANDLED;

    if ( rtd_isr_inb( SYS_GIP_1_0x2c39 ) & _BIT5 ) // check if is vbi pending (VBI Slicer Interrupt Pending Status)
    {
        if ( rtd_isr_inl(0x5A30) & _BIT17 ) // check if is vbi cc ready (int)
        {
            regCCData = rtd_isr_inl(0x5a2c) & 0x0000FFFF; //read CC data from VBISL17 register
            //RTD_Log( LOGGER_FATAL, "%x\n", (UINT32)regCCData ); // Dump CC data
            regCCData=regCCData&0x7F7F;
            //CC_Log( "%x\n", (UINT32)regCCData ); // Dump CC data
            field_type = (UINT8)(( rtd_isr_inl(0x5A30) & _BIT31 )>>24);
            //CC_Log( "%x\n", (UINT32)field_type);
            rtd_isr_outl(0x5A30, (rtd_isr_inl(0x5A30) | _BIT17)); // clear vbi cc ready (int)

            if (regCCData!=0)
            {
                //////////////////////////////////
                //Extract Field Type
                //////////////////////////////////
                //field_type = (pData[0] &0x04) << 5;
                /////////////////////////////////
                /////////////////////////////////
                //@ Recieved, put into buffer.
                /////////////////////////////////
                CCBuffer[ucCCWriteCnt][0] = ((regCCData&0x7F00)>>8)+ field_type;
                CCBuffer[ucCCWriteCnt][1] = (regCCData&0x007F);
                if (CCBuffer[ucCCWriteCnt][1]==0x21)
                {
                    if (((CCBuffer[ucCCWriteCnt][0]&0x7F)==0x14)||((CCBuffer[ucCCWriteCnt][0]&0x7F)==0x15)
                            ||((CCBuffer[ucCCWriteCnt][0]&0x7F)==0x1c)||((CCBuffer[ucCCWriteCnt][0]&0x7F)==0x1D))
                    {
                        SpaceComEnable =1;
                        //CC_Log( "\ndetect_back\n", 0);
                    }
                }
                else
                {
                    if ((field_type !=0)&&(SpaceComEnable==1))
                        SpaceComEnable =0;
                }

                /////////////////////////////////
                //@ Move buffer pointer
                /////////////////////////////////
                ucCCWriteCnt = (ucCCWriteCnt +1) % CCBuffer_Size;
                /////////////////////////////////
                //@ Inform CC is received
                /////////////////////////////////
                bCCReceiveOK = _TRUE;
            }
            else
            {
                if (SpaceComEnable==1)
                {
                    if (field_type!=0)
                    {
                        //CC_Log( "\nInsert_space\n", 0);
                        CCBuffer[ucCCWriteCnt][0] = 0x00+field_type;
                        CCBuffer[ucCCWriteCnt][1] = 0x00;
                        ucCCWriteCnt = (ucCCWriteCnt +1) % CCBuffer_Size;
                        SpaceComEnable=0;
                    }

                }
            }
            //count = 0;

            return RT_ISR_HANDLED;
        }
    }
    //count ++;

    /*if(count>100)
    {
    	CC_Log( "No CC data \n" ); // Dump CC data Fail
    	Disable_CC_Input();
    	Set_CC_Status(FALSE);
    	count = 0;
    }*/

    return RT_ISR_UNHANDLED;
}

/* -- Implementations of Private Functions ----------------------------------*/
#if 0
static void CC_Log(char code *format, ...)
{
    int xdata i;
    UINT8 xdata getVal;
    bit fFirstNotZero;
    long xdata argint = 0;
    long xdata base;
    va_list xdata arglist;

    va_start(arglist,format);

    while (*format)
    {
        if (*format == '%')
        {
            switch  (*(format+1))
            {
            case 'd':
                argint = va_arg(arglist,long);
                if (argint == 0)
                {
                    drv_isr_uart_Write(UART_MONITOR_SOURCE, '0');
                }
                else
                {
                    if (argint < 0)
                    {
                        drv_isr_uart_Write(UART_MONITOR_SOURCE, '-');
                        argint = -argint;
                    }
                    base = 1;
                    i = 0;
                    while (argint / base)
                    {
                        base *= 10;
                        i++;
                    }
                    base /= 10;
                    while (i > 0)
                    {
                        getVal = argint / base;
                        argint = argint - (base * getVal);
                        getVal += '0';
                        base /= 10;
                        drv_isr_uart_Write(UART_MONITOR_SOURCE, getVal);
                        i--;
                    }
                }
                break;

            case 'X':	// <Janus>2009/08/18: Add support to "%X"
            case 'x':
                argint = va_arg(arglist, long);
                drv_isr_uart_Write(UART_MONITOR_SOURCE, '0');
                drv_isr_uart_Write(UART_MONITOR_SOURCE, 'x');
                if (argint == 0)
                {
                    drv_isr_uart_Write(UART_MONITOR_SOURCE, '0');
                }
                else
                {
                    i = 8;
                    fFirstNotZero = 0;
                    while (i > 0)
                    {
                        if (!fFirstNotZero)
                        {
                            getVal = ((argint & ((UINT32)0x0f) << ((i - 1) << 2)) >> ((i - 1) << 2)) & 0x0f;
                            if (getVal != 0)
                                fFirstNotZero = 1;
                        }

                        if (fFirstNotZero)
                        {
                            getVal = ((argint & ((UINT32)0x0f) << ((i - 1) << 2)) >> ((i - 1) << 2)) & 0x0f;
                            if (getVal > 9)
                                getVal = (getVal - 0x0a) + 'a';
                            else
                                getVal += '0';

                            drv_isr_uart_Write(UART_MONITOR_SOURCE, getVal);
                        }
                        i--;

                    }
                }
                break;
            case 's':	// <Janus>2009/08/06: Add support to "%s"
            {
                char* szStr = va_arg( arglist, char* );
                while ( *szStr != '\0' )
                {
                    drv_isr_uart_Write(UART_MONITOR_SOURCE, *szStr);
                    szStr++;
                }
                break;
            }
            }
            format+=2;
        }
        if (*format == 0x0a)
        {
            drv_isr_uart_Write(UART_MONITOR_SOURCE, 0x0a);
            drv_isr_uart_Write(UART_MONITOR_SOURCE, 0x0d);
            format++;
        }
        else
        {
            if (*format != 0)
                drv_isr_uart_Write(UART_MONITOR_SOURCE, *format);
            format++;
        }
    }
}
#endif
#else
void CTemp(void)
{
	unsigned char i;
	i = 0;
	RTD_Log(LOGGER_INFO, "initial CC \n");
}
#endif

/*---------------------------------------------------------------------------*\
 * end of vbi_cc_verify.c                                                    *
\*---------------------------------------------------------------------------*/

