/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2009
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is a boot fuction for sparrow
 *
 * @author 	: Weihao Lo
 * @date		: 06/04/2009
 * @version	: 1.0
 * @ingroup	: System
 */

#include "sparrow.h"
#include "scaler_idomain\measure.h"
#include "sparrow_utility.h"
#include "video\video.h"
#include "reg52.h"
#include "char\uart.h"
#include "flow_conf_struct.h"

#define PLL_CPU_189_FLASH_315	0
#define PLL_CPU_162_FLASH_405	1
#define PLL_CPU_108_FLASH_27	2
#define USE_PLL_CONFIG			PLL_CPU_162_FLASH_405
extern UINT8 fCPUSpeed;

void drv_sparrow_PowerOn(void)
{
    // make CPU PLL to normal operation
    rtdf_clearBitsb(SYS_PLL_CPU_0_0x2c60, _BIT1);

// Enable Clock
    //7: lsadc crystal clock enable
    //6: timer crystal clock enable
    //5: gpio crystal clock enable
    //4: ddc1 crystal clock enable
    //3: ddc0 crystal clock enable
    //2: hdmi crystal clock enable
    //1: video decoder 108MHz clock enable
    //0: vbi slicer clock enable
    rtd_outb(SYS_CLKEN_0_0x2cc0, 0xff);

    //7: i2cm1 clock enable
    //6: i2cm0 clock enable
    //5: spi clock enable
    //4: disp m-domain clock enable
    //3: uart 98MHz clock enable
    //2: if demod 49MHz clock enable
    //1: audio 49MHz clock enable (divide from audio 98MHz)
    //0: audio 98MHz clock enable
    rtd_outb(SYS_CLKEN_1_0x2cc1, 0xff);

    //7: PWM crystal clock enable
    //6: IR crystal clock enable
    //5: UART1 crystal clock enable
    //4: UART0 crystal clock enable
    //3: audio 16MHz clock enable (divide from audio 98MHz)
    //2: audio 24.5MHz clock enable (divide from audio 98MHz)
    //1: dispd domain clock enable
    //0: dispi domain clock enable
    rtd_outb(SYS_CLKEN_2_0x2cc2, 0xff);

    //7: share_mem osd12k clock enable
    //6: share_mem osd8k clock enable
    //5: display crystal clock enable
    //4: MIS system clock enable
    //3: MIS crystal clock enable
    //2: I2C2 crystal clock enable
    //1: I2C1 crystal clock enable
    //0: I2C0 crystal clock enable
    rtd_outb(SYS_CLKEN_3_0x2cc3, 0xff);

    //7: Clock enable for audio Audio_pre512fs
    //6: Clock enable for Audio_daad_256fs
    //5: Clock enable for Audio_daad_128fs
    //4: Clock enable for Audio Decoder, including BBADC/BBDAC
    //2: Clock enable for audio path 2 Audio_512fs
    //1: Clock enable for audio path 2 Audio_daad_256fs
    //0: Clock enable for audio path 2 Audio_daad_128fs
    rtd_outb(SYS_CLKEN_4_0x2cc4, 0xff);

// Release reset for each ip
	//7: HDMI RX software reset, low active
	//6: VBI Slicer software reset, low active
	//5: IF demodulator software reset, low active
	//4: Video Decoder logic software reset, low active
	//3: Video Decoder register software reset, low active
	//2: Flash Controller software reset, low active
	//1: SPI-DMA software reset, low active
	//0: Rbus software reset, low active
	rtd_outb(SYS_SRST_0_0x2ca0, 0xff);

	//5: Disp D-domain software reset, low active
	//4: Disp I-domain software reset, low active
	//3: Audio DAC 2 software reset, low active
	//2: Audio DAC 1 software reset, low active
	//1: Audio ADC software reset, low active
	//0: Audio software reset, low active
	rtd_outb(SYS_SRST_1_0x2ca1, 0xff);

	//7: VBI OSD software reset, low active
	//6: VBI VTE software reset, low active
	//5: VBI PPR demodulator software reset, low active
	//4: VBI RBus/DBus software reset, low active
	//2: Share-Mem UI OSD-12KB function software reset, low active
	//1: Share-Mem UI OSD-8KB function software reset, low active
	//0: Share-Mem TT-13KB function software reset, low active
	rtd_outb(SYS_SRST_2_0x2ca2, 0xff);

	//7: PWMs software reset, low active
	//6: Misc software reset, low active
	//3: Timers software reset, low active
	//2: UART 1 software reset, low active
	//1: UART 0 software reset, low active
	//0: LSADCs software reset, low active
	rtd_outb(SYS_SRST_3_0x2ca3, 0xcf);

	//6: DDC1 software reset, low active
	//5: DDC0 software reset, low active
	//4: GPIOs software reset, low active
	//3: IrDA software reset, low active
	//2: I2C 2 software reset, low active
	//1: I2C 1 software reset, low active
	//0: I2C 0 software reset, low active
	rtd_outb(SYS_SRST_4_0x2ca4, 0xff);
}

void drv_sparrow_PowerOff(void)
{
#if 0
    // disable interrupt
    rtdf_outb(SYS_GIE_0_0x2c34, 0x00);
    rtdf_outb(SYS_GIE_1_0x2c35, 0x00);
    rtdf_outb(SYS_GIE_2_0x2c36, 0x08);
    rtdf_outb(SYS_GIE_3_0x2c37, 0x44);

    //-- Clock Enable
    // lsadc & timer & gpio clock enable
    rtdf_outb(SYS_CLKEN_0_0x2cc0, 0x60);

    // spi clock enable:Enable
    rtdf_outb(SYS_CLKEN_1_0x2cc1, 0x28);

    rtdf_outb(SYS_CLKEN_2_0x2cc2, 0x50);

    // MIS system clock enable: Enable
    // MIS crystal clock enable: Enable
    rtdf_outb(SYS_CLKEN_3_0x2cc3, 0xf8);

    rtdf_outb(SYS_CLKEN_4_0x2cc4, 0x00);
#else
    //-- Clock Enable
    // lsadc & timer & gpio clock enable
    rtdf_maskb(SYS_CLKEN_0_0x2cc0, 0x00, _BIT5 | _BIT6 | _BIT7);

    // spi clock enable:Enable
    rtdf_maskb(SYS_CLKEN_1_0x2cc1, 0x00 , 0);

    rtdf_maskb(SYS_CLKEN_2_0x2cc2, 0x00, _BIT6 | _BIT4);

#ifdef CONFIG_STORAGE_ACCESS_EEPROM
    // MIS system clock enable: Enable
    // MIS crystal clock enable: Enable
    // I2c0 crystal clock enable: Enable
    rtdf_maskb(SYS_CLKEN_3_0x2cc3, 0x00, _BIT4 | _BIT3 | _BIT5 | _BIT0);
#else
    // MIS system clock enable: Enable
    // MIS crystal clock enable: Enable

    rtdf_maskb(SYS_CLKEN_3_0x2cc3, 0x00, _BIT4| _BIT3 /*| _BIT5*/);
#endif

#ifdef CONFIG_STORAGE_ACCESS_EEPROM
//	rtdf_setBits(SYS_SRST_4_0x2ca4, _BIT0);
#endif

    rtdf_maskb(SYS_CLKEN_4_0x2cc4, 0x00, 0x00);

	rtdf_maskb(SYS_SRST_0_0x2ca0, 0x7f, 0x00);
    rtdf_maskb(SYS_PWRSAV_0x2cc8, 0x00, _BIT1);

    rtdf_setBitsb(SYS_PLL_CPU_0_0x2c60, _BIT1);
#endif
}

void drv_sparrow_boot(void)
{
    //-- flash divider not to make flash higher speed
#if 0  // marked by weihao because of framesync error  980817
    rtd_outb(0xffed, 0x84);
//	drv_imem8k_SetAddress(0);
    rtd_outb(0x2c69,0x88);	// Audio pll enable
    rtd_writeScalerPort(0x22, 0x03);	//FiFo frequency reference audio
#else
//	drv_sparrow_PowerOn();
    //Low Voltage Reset set to 0x11
//	rtd_outb(SYS_POR_0x2c78, 0x03);

//       rtd_outb(READ_INSTRUCTION_0xff6c, 0x20);
    rtd_outb(0x2c69,0x88);	// Audio pll enable
    rtd_writeScalerPort(0x22, 0x03);	//FiFo frequency reference audio
//	rtd_outb(0x2d04,0xcc);
//	rtd_outb(0x2d05,0xcc);
    drv_sparrow_CPU_Switch_Clock(_FALSE);
//	rtd_outb(0xffed, 0x9b);		// divider 7
//  	rtd_outb(0xffed, 0x87);		// divider 1
//  	rtd_outb(0xffed, 0x8b);		// divider 2

    //-- PLL setting as followed in order to gump
    // Charge-pump current select: 7.5 uA
    // Loop filter capacitor (CS) select: 11: 28 pF (default)
    // CLK_PLLVCPU output enable control: Disable
    // PLL reset control: Normal
    // PLL power down control: active
    // VCO test clock output enable: disable (default)
    rtd_outb(SYS_PLL_CPU_0_0x2c60, 0xb5);

    //N (reference) divider control: divide by 2 (default)
    // PLL watch dog reset: normal (default)
    // PLL watch dog set: normal (default)
    // Loop filter resistor (RS) select: 26 k£[(default)
    // Loop filter capacitor (CP) select: 0: 1.9 pF (default)
#if (USE_PLL_CONFIG == PLL_CPU_189_FLASH_315)
    rtd_outb(SYS_PLL_CPU_1_0x2c61, 0x2a);		// N = 1;
#elif (USE_PLL_CONFIG == PLL_CPU_162_FLASH_405)
    rtd_outb(SYS_PLL_CPU_1_0x2c61, 0x6a);		// N = 2;
#elif (USE_PLL_CONFIG == PLL_CPU_108_FLASH_27)
    rtd_outb(SYS_PLL_CPU_1_0x2c61, 0x6a);		// N = 2;
#endif

    // M (feedback) divider control: 0x1d (default)
#if (USE_PLL_CONFIG == PLL_CPU_189_FLASH_315)
    rtd_outb(SYS_PLL_CPU_2_0x2c62, 0x0c);	// M = 12
#elif (USE_PLL_CONFIG == PLL_CPU_162_FLASH_405)
    rtd_outb(SYS_PLL_CPU_2_0x2c62, 0x16);	// M = 0x16 // M=0x18
#elif (USE_PLL_CONFIG == PLL_CPU_108_FLASH_27)
    rtd_outb(SYS_PLL_CPU_2_0x2c62, 0x0e);	// M = 0x0e
#endif

#endif
    // Enable Bank switch in romcode
//	rtdf_maskb(MCU_XDATA_SWITCH_CONTROL_0xFFFC, 0xff, _BIT1 | _BIT0);

    // Set dual port to speed up access flash for cpu
    rtd_outb(FAST_READ_OP_CODE_0xff6b, 0x3B);
//->fh	rtd_outb(READ_INSTRUCTION_0xff6c, 0x80);	// //->fh
#if 1
// Release reset for each ip
    //7: HDMI RX software reset, low active
    //6: VBI Slicer software reset, low active
    //5: IF demodulator software reset, low active
    //4: Video Decoder logic software reset, low active
    //3: Video Decoder register software reset, low active
    //2: Flash Controller software reset, low active
    //1: SPI-DMA software reset, low active
    //0: Rbus software reset, low active
    rtd_outb(SYS_SRST_0_0x2ca0, 0xff);

    //5: Disp D-domain software reset, low active
    //4: Disp I-domain software reset, low active
    //3: Audio DAC 2 software reset, low active
    //2: Audio DAC 1 software reset, low active
    //1: Audio ADC software reset, low active
    //0: Audio software reset, low active
    rtd_outb(SYS_SRST_1_0x2ca1, 0xff);

    //7: VBI OSD software reset, low active
    //6: VBI VTE software reset, low active
    //5: VBI PPR demodulator software reset, low active
    //4: VBI RBus/DBus software reset, low active
    //2: Share-Mem UI OSD-12KB function software reset, low active
    //1: Share-Mem UI OSD-8KB function software reset, low active
    //0: Share-Mem TT-13KB function software reset, low active
    rtd_outb(SYS_SRST_2_0x2ca2, 0xff);

    //7: PWMs software reset, low active
    //6: Misc software reset, low active
    //3: Timers software reset, low active
    //2: UART 1 software reset, low active
    //1: UART 0 software reset, low active
    //0: LSADCs software reset, low active
    rtd_outb(SYS_SRST_3_0x2ca3, 0xcf);

    //6: DDC1 software reset, low active
    //5: DDC0 software reset, low active
    //4: GPIOs software reset, low active
    //3: IrDA software reset, low active
    //2: I2C 2 software reset, low active
    //1: I2C 1 software reset, low active
    //0: I2C 0 software reset, low active
    rtd_outb(SYS_SRST_4_0x2ca4, 0xff);

// Enable Clock
    //7: lsadc crystal clock enable
    //6: timer crystal clock enable
    //5: gpio crystal clock enable
    //4: ddc1 crystal clock enable
    //3: ddc0 crystal clock enable
    //2: hdmi crystal clock enable
    //1: video decoder 108MHz clock enable
    //0: vbi slicer clock enable
    rtd_outb(SYS_CLKEN_0_0x2cc0, 0xff);

    //7: i2cm1 clock enable
    //6: i2cm0 clock enable
    //5: spi clock enable
    //4: disp m-domain clock enable
    //3: uart 98MHz clock enable
    //2: if demod 49MHz clock enable
    //1: audio 49MHz clock enable (divide from audio 98MHz)
    //0: audio 98MHz clock enable
    rtd_outb(SYS_CLKEN_1_0x2cc1, 0xff);

    //7: PWM crystal clock enable
    //6: IR crystal clock enable
    //5: UART1 crystal clock enable
    //4: UART0 crystal clock enable
    //3: audio 16MHz clock enable (divide from audio 98MHz)
    //2: audio 24.5MHz clock enable (divide from audio 98MHz)
    //1: dispd domain clock enable
    //0: dispi domain clock enable
    rtd_outb(SYS_CLKEN_2_0x2cc2, 0xff);

    //7: share_mem osd12k clock enable
    //6: share_mem osd8k clock enable
    //5: display crystal clock enable
    //4: MIS system clock enable
    //3: MIS crystal clock enable
    //2: I2C2 crystal clock enable
    //1: I2C1 crystal clock enable
    //0: I2C0 crystal clock enable
    rtd_outb(SYS_CLKEN_3_0x2cc3, 0xff);

    //7: Clock enable for audio Audio_pre512fs
    //6: Clock enable for Audio_daad_256fs
    //5: Clock enable for Audio_daad_128fs
    //4: Clock enable for Audio Decoder, including BBADC/BBDAC
    //2: Clock enable for audio path 2 Audio_512fs
    //1: Clock enable for audio path 2 Audio_daad_256fs
    //0: Clock enable for audio path 2 Audio_daad_128fs
    rtd_outb(SYS_CLKEN_4_0x2cc4, 0xff);
#else
    //-- Release reset
    // Release the reset status for Flash controller, SPI_DMA , and RBus
    rtdf_setBitsb(SYS_SRST_0_0x2ca0, _BIT0);

    rtdf_setBitsb(SYS_SRST_1_0x2ca1, 0x00);

    rtdf_setBitsb(SYS_SRST_2_0x2ca2, 0x00);

    // Release the reset status for misc & timers
    rtdf_setBitsb(SYS_SRST_3_0x2ca3, _BIT6 | _BIT3 | _BIT1);

    rtdf_setBitsb(SYS_SRST_4_0x2ca4, _BIT4 | _BIT3);

    //-- Clock Enable
    // lsadc & timer & gpio clock enable
    rtdf_setBitsb(SYS_CLKEN_0_0x2cc0, _BIT5 | _BIT6);

    // spi clock enable:Enable
    rtdf_setBitsb(SYS_CLKEN_1_0x2cc1, 0);

    rtdf_setBitsb(SYS_CLKEN_2_0x2cc2, _BIT6 | _BIT4);

    // MIS system clock enable: Enable
    // MIS crystal clock enable: Enable
    rtdf_setBitsb(SYS_CLKEN_3_0x2cc3, 0x00);

    rtdf_setBitsb(SYS_CLKEN_4_0x2cc4, 0x00);
#endif
}

//	0: use crystal clock(27MHz)
//	1: use CPU PLL clock
void drv_sparrow_CPU_Switch_Clock(bit type)
{
    // Switching MCU from Crystal clock to PLLCPU clock divied by DIV
    if (type)
    {
//		rtdf_setBitsb(MCU_CONTROL_0xFFED, _BIT1);
#if (USE_PLL_CONFIG == PLL_CPU_189_FLASH_315)
        rtd_outb(0xffed, 0x9b);						// divider 6
#elif (USE_PLL_CONFIG == PLL_CPU_162_FLASH_405)
        rtd_outb(0xffed, 0x93);						// divider 4
#elif (USE_PLL_CONFIG == PLL_CPU_108_FLASH_27)
        rtd_outb(0xffed, 0x93);						// divider 4
#endif
        rtdf_setBitsb(SYS_CLKSEL_0x2ccc, _BIT1);

        if (stflowbData.nFirstReadBy2Bits_Flash)
        {
            rtd_outb(READ_INSTRUCTION_0xff6c, 0xa0);
        }
        else
        {
            rtd_outb(READ_INSTRUCTION_0xff6c, 0x20);
        }
        fCPUSpeed = 1;
    }
    else
    {
//		rtdf_clearBitsb(MCU_CONTROL_0xFFED, _BIT1);
        rtd_outb(READ_INSTRUCTION_0xff6c, 0x20);
        rtdf_clearBitsb(SYS_CLKSEL_0x2ccc, _BIT1);
        rtd_outb(0xffed, 0x84);		// divider 1
        fCPUSpeed = 0;
    }
}

void drv_sparrow_power_off_misc(bit on_off)
{
    if (on_off)
    {
        rtd_outb(SYS_CLKDIV_0x2ccd, 0x00);
        rtd_outb(SYS_CLKDIV_0x2ccd, 0x00);
    }
    else
    {
        //HPWang add start
		rtd_outb(SYS_CLKDIV_0x2ccd, 0x02);//0x03
		rtd_outb(SYS_CLKDIV_0x2ccd, 0x1a); //0x1b
        rtd_outb(0x2d03,0x00);
        rtd_outb(0x2d04,0x00);
        rtd_outb(0x2d05,0x00);
        rtd_outb(0x2d07,0x00);

        rtd_outl(CONTROL_MISC_0x2258,0x00);
//		rtdf_clearBits(SYS_PLL_27X_0_0x2c64, _BIT6);
        rtdf_setBits(PLL_DISP_0_0x2f00, _BIT19); // power down DPLL
        rtdf_setBits(PLL_DISP_1_0x2f04, _BIT1); // power down DPLL
        //HPWang add end
    }
}

#if 0
void drv_sparrow_isr_CPU_Switch_Clock(bit type)
{
    // Switching MCU from Crystal clock to PLLCPU clock divied by DIV
    if (type)
    {
//		rtdf_setBitsb(MCU_CONTROL_0xFFED, _BIT1);
#if (USE_PLL_CONFIG == PLL_CPU_189_FLASH_315)
        rtd_outb(0xffed, 0x9b);						// divider 6
#elif (USE_PLL_CONFIG == PLL_CPU_162_FLASH_405)
        rtd_outb(0xffed, 0x93);						// divider 4
#elif (USE_PLL_CONFIG == PLL_CPU_108_FLASH_27)
        rtd_outb(0xffed, 0x93);						// divider 4
#endif
        rtdf_setBitsb(SYS_CLKSEL_0x2ccc, _BIT1);

        if (stflowbData.nFirstReadBy2Bits_Flash)
        {
            rtd_outb(READ_INSTRUCTION_0xff6c, 0xa0);
        }
        else
        {
            rtd_outb(READ_INSTRUCTION_0xff6c, 0x20);
        }
        fCPUSpeed = 1;
    }
    else
    {
//		rtdf_clearBitsb(MCU_CONTROL_0xFFED, _BIT1);
        rtd_outb(READ_INSTRUCTION_0xff6c, 0x20);
        rtdf_clearBitsb(SYS_CLKSEL_0x2ccc, _BIT1);
        rtd_outb(0xffed, 0x84);		// divider 1
        fCPUSpeed = 0;
    }
}
#endif

