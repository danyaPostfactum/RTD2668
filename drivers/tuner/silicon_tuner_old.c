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


#include "pcb_parameter_def.h"
#include "i2c\i2c.h"
#include "ifd\ifd.h"
#include "tuner\tuner.h"
#include "video\video.h"
#include "flow_struct.h"
#include "scaler_idomain\syncproc.h"
#include "scaler_idomain\measure.h"
#include "reg_def.h"
#include "flow_scaler.h"
#include "rosprintf.h"
#include "timer_event.h"
#include "mode.h"
#include "mode_customer.h"
#include "pcb_customer.h"
#include "sparrow\spi_dma.h"
#ifdef _VIDEO_TV_SUPPORT
#ifdef CONFIG_TUNER_SILICON
#ifdef _SILICON_BRYAN_CODE

#define RT810_P2VERSION 1
//#define RT810_T4VERSION 1
#define RT810_LOWCOST    1
#if  defined(CONFIG_TUNER_SILICON_RT810)
      #define _TUNER_WRITE_ADDRESS		0x1A
#elif defined(CONFIG_TUNER_SLICON_RT610)
      #define _TUNER_WRITE_ADDRESS		0x3A
#endif


#define	_TUNER_MIN_FREQ								(450000)
#define	_TUNER_MAX_FREQ								(9000000)
#define	_TUNER_VHF_LOW_FREQ							(1442500)
#define 	_TUNER_VHF_HIGH_FREQ   						(4322500)

#define			RINGPLL0		64000
#define			RINGPLL1		96000
#define			RINGPLL2		128000
#define			RINGPLL3		160000
#define			RINGPLL4		192000
#define			RINGPLL5		224000
#define			RINGPLL6		256000
#define			RINGPLL7		288000
#define			RINGPLL8		320000
#define			RINGPLL9		352000
#define			RINGPLL10		384000
#define			RINGPLL11		416000
#define			RINGPLL12		448000
#define			RINGPLL13		480000
#define			RINGPLL14		512000
#define			RINGPLL15		544000
#define			RINGPLL16		576000
#define			RINGPLL17		608000
#define			RINGPLL18		640000
#define			RINGPLL19		672000
#define			RINGPLL20		704000
#define			RINGPLL21		736000
#define			RINGPLL22		768000
#define			RINGPLL23		800000
#define			RINGPLL24		832000
#define			RINGPLL25		864000
#define			RINGPLL26		896000
#define			RINGPLL27		928000
#define			RINGPLL28		960000
#define			RINGPLL29		992000
#define			RINGPLL30		1024000
#define			RINGPLL31		1056000

#define			RINGLO			5800
#define			RINGLO6M		6000
#define	_TUNER_PICTURE_INTERMEDIATE_FREQ   			(78200)//(77500)
static UINT8 code RT810BW8MCalib0[2]  =    {0,140};
static UINT8 code RT810BW8MCalib1[2]  =    {4,140};

static UINT8 code RT810BW8MCalib2[2]  =    {0,136};
static UINT8 code RT810BW7MCalib2[2]  =    {16,136};
static UINT8 code RT810BW6MCalib2[2]  =    {24,136};

UINT8 xdata g_curr_tunerband;
UINT32 xdata g_curr_freq;

//static UINT8 xdata RT810RegsMap[23];
#ifdef RT810_P2VERSION
static UINT8 xdata RT810RegsMap[25] =
{
    /*0x00,0x00,0x00,*/0xd6,0x93,0xa3,0x1c,0x50,
    0x20,0x20,0x6f,0x02,0x80,0x62,0x53,0x98,
    0x60,0x50,0x70,0x00,0xd2,0xf5,0x08,0x83,0x01,0x00,0x00,0x00

};

//for BW calibation
static UINT8 code RT810BW8MCalib[25]  =    {214,147,191,28,112,32,96,96,0,56,98,83,136,128,80,112,0,78,0,0,131,1,0,0,0};
static UINT8 code RT810BW6MCalib[25]  =    {214,147,191,28,112,32,96,96,0,56,98,83,136,128,80,112,0,77,0,0,131,1,0,0,0};
#endif

#ifdef RT810_T4VERSION
static UINT8 xdata RT810RegsMap[24] =
{
    /*0x00,0x00,0x00,*/0xd6,0x93,0xa3,0x1c,0x50,
    0x20,0x20,0x6f,0x02,0x80,0x62,0x53,0x98,
    0x60,0x50,0x70,0x00,0xd2,0xf5,0x08,0x83,0x01,0x00,0x00

};

//for BW calibation
static UINT8 code RT810BW8MCalib[24]  =    {214,147,191,28,112,32,96,96,0,56,98,83,136,128,80,112,0,78,0,0,131,1,0,0};
static UINT8 code RT810BW6MCalib[24]  =    {214,147,191,28,112,32,96,96,0,56,98,83,136,128,80,112,0,77,0,0,131,1,0,0};

#endif


#define VCOFREQ_MIN 1416000000
#define VCOFREQ_MAX 2832000000
#define  VcoFmin 1416000  //KHz
#define  VcoFmax 2832000  //KHz
enum
{
    CHANNELBW_8M=0,
    CHANNELBW_7M=0x2,
    CHANNELBW_6M=0x3,
};
static UINT8 xdata gans;
static UINT8 xdata gans4;
static UINT8 xdata phs;
static UINT8 xdata phs4;
static UINT8 xdata R1Dat,x,y,pkmark;
static UINT8 xdata rf_in= 0;

UINT8 xdata pk[7][2];
void drv_tuner_write_rt810(UINT8* pRegsMap);
void drv_tuner_write_rt810_vcoonly(UINT8* pRegsMap);
void drv_tuner_write_rt810_cali(UINT8* pRegsMap);

#define PAL 0
#define NTSC 1
static UINT8 Standard;
static UINT32 ChannelIF;
static UINT8 ChannelBW;
// Unit: 10Hz
#define	_TUNER_RATIO_IN_STEP_SIZE					(625)//625)	//(625)




/*=============================================================*/
/**
	This routine will set init.
	@param
	@return
*/

void CTunerGlobalVarInit(void)
{
    UINT8 i;
    g_curr_freq=0;

//	drv_spi_dma_memcpy((UINT16)RT810RegsMap, _BANK7, (UINT16)RT810RegsCodeMap, sizeof(RT810RegsCodeMap), SPI_DMA_TRANSFER_MODE_DMEM);

    gans = 0;
    gans4 = 0;
    phs = 0;
    phs4 = 0;
    Standard=0xff;
    ChannelIF=_TUNER_PICTURE_INTERMEDIATE_FREQ;
    ChannelBW=CHANNELBW_8M;
    for (i=0; i<7; i++)
        pk[i][0]=0x99;
    pkmark = 0;
}


void updatereg(UINT8 regno, UINT8 mask, UINT8 regdata)
{
    RT810RegsMap[regno-3]=(RT810RegsMap[regno-3] & mask) | regdata;
}

#ifdef RT810_P2VERSION

void initialreg(UINT32 freq)
{
    UINT8 rfmux=0,tf,itf;

    // *****Reg 3
// pw_lt = 3;
// rfin_sw = 0;
// lna_gain_mode = 1;
// lna_gain = 9;

    updatereg(3,0x3f,0xc0);
#if (_TUNER_WRITE_ADDRESS ==0x1A)
	if(rf_in ==0)
#elif(_TUNER_WRITE_ADDRESS ==0x3A)
	if (rf_in==1)
#endif
        updatereg(3,0xdf,0x0);
    else
        updatereg(3,0xdf,0x1 << 5);

    updatereg(3,0xef,0x10);
    updatereg(3,0xf0,0x09);

//***** Reg 4
// ring_pwd = 1;
// pwd_lna = 0;
// pw1_0_lna = 1;
// pwd_pdetec1 = 0;
// pdetec1 = 2;	// +6dB

    updatereg(4,0x7f,0x80);
    updatereg(4,0xbf,0x0);
    updatereg(4,0xcf,0x10);
    updatereg(4,0xf7,0x0);
    updatereg(4,0xf8,0x02);

//***** Reg 5
// hfgain_up = 0;
// rfmux = 0;
// tf_dvbt = 1;
// tf = 0;

    updatereg(5,0x7f,0x0);
    updatereg(5,0xbf,0x0);
    updatereg(5,0xdf,0x20);
    updatereg(5,0xe0,0x0);

// ***** Reg 6
// pw_ring = 0;
// pa74_enb = 0;
// mix_gain_mode = 1;
// mix_gain = 10;

    updatereg(6,0x3f,0x0);
    updatereg(6,0xdf,0x0);
    updatereg(6,0xef,0x10);
    updatereg(6,0xf0,0x0a);

//*****  Reg 7
// pwd_mix = 0;
// lna_top_sw = 0;
// pw0_mix = 1;
// pw0_amp = 1;
// pdetec2 = 4;		// 11

    updatereg(7,0x7f,0x0);
    updatereg(7,0xbf,0x0);
    updatereg(7,0xdf,0x1 << 5);			// Mixer current low, 3 is lowest
    updatereg(7,0xef,0x1 << 4);
    updatereg(7,0xf0,0x04);

//*****  Reg 8
// pwd_IQ = 0;
// pw0_IQ = 1;
//gans = 0;
//gans4 = 0;
    updatereg(8,0x7f,0x0);
    updatereg(8,0xbf,0x1 << 6);

//*****  Reg 9
// pwd_poly = 0;
// pw0_poly = 1;
//phs = 0;
//phs4 = 0;
    updatereg(9,0x7f,0x0);
    updatereg(9,0xbf,0x1 << 6);

//*****  Reg 10
// pwd_filt = 0;
// pw1_0_filt = 3;
// enb_tune = 0;
// cor = 12;

    updatereg(10,0x7f,0x0);
    updatereg(10,0x9f,0x3 << 5);
    updatereg(10,0xef,0x0);
    updatereg(10,0xf0,0x0c);
//*****  Reg 11
// edge_delay2 = 0;
// edge_delay = 0;
// v7M = 0;
// v6M = 0;
// vstart = 0;
// pwd_adc = 1;
// pwd_vga = 0;

    updatereg(11,0x1f,0x0);
    updatereg(11,0xfb,0x0);
    updatereg(11,0xfd,0x1 << 1);
    updatereg(11,0xfe,0x0);

//*****  Reg 12
// vga_mode = Tvga_mode;
// dmode = 0;
// vga_code = Tvga_code;

    updatereg(12,0x7f,0x1 <<7);				// VGA Pin mode
    updatereg(12,0x80,20);

// lnavthh = 8;
// lnavthl = 3;
    updatereg(13,0x0f,0x8 << 4);
    updatereg(13,0xf0,0x3);

// mix_vthh = 5;
// mix_vthl = 3;
    updatereg(14,0x0f,0x6 << 4);
    updatereg(14,0xf0,0x3);

//*****  Reg 15
// agc_clks = 1;
// pllauto_clks = 0;
// clk_ring_enb = 1;
// clk_file_enb = 0;
// clk_agc_enb = 0;
// gpio_in = 0;
    updatereg(15,0x3f,0x2 << 6);
    updatereg(15,0xcf,0);
    updatereg(15,0xf7,0x1 << 3);
    updatereg(15,0xfb,0x0);
    updatereg(15,0xfd,0x0);
    updatereg(15,0xfe,0x0);
//*****  Reg 16
// sel_div = 2;
// ring = 12;
    updatereg(16,0x1f,0x2 << 5);
    updatereg(16,0xe0,0x0);

//*****  Reg 17
// pwd_vco_out = 0;
// cp_0406 = 0;
// pw_cp = 2;
// pwd_bias = 0;
// pw_hfd = 3;
    updatereg(17,0x7f,0x0);
    updatereg(17,0xbf,0x0);
    updatereg(17,0xc7,0x2 << 3);
    updatereg(17,0xfb,0x0);
    updatereg(17,0xfc,0x3);
//*****  Reg 18
// pw_vco = 3;
// pw_dither = 1;
// pw_sdm = 0;
// pw_sdmout = 0;
// pw_ldo_a = 0;
// mixhg_enb = 0;
    updatereg(18,0x1f,0x2 << 5);
    updatereg(18,0xef,0x1 << 4);
    updatereg(18,0xf7,0x0);
    updatereg(18,0xfb,0x0);
    updatereg(18,0xfc,0x0);
//*****  Reg 19
// pa74par = 0;
// pa74_lp = 0;
// pa74hp = 0;
    updatereg(19,0x00,0x0);
//*****  Reg 23
// pw1_vga = 1;
// clk_out_enb = 0;
// pw45 = 0;
// itf = 15;
    updatereg(23,0x7f,0);				// VGA power high.
    updatereg(23,0xbf,0x0);
    updatereg(23,0xcf,0x0);
    updatereg(23,0xf0,0xf);

//*****  Reg 24
//ring_lf = 0;
//refdiv2 = 0;
//ringldo = 0;
//polyrfmux = 1;
// sw_res = 1;
//offset = 1;
    updatereg(24,0x3f,0x0);
    updatereg(24,0xdf,0x0);
    updatereg(24,0xef,0x0);
    updatereg(24,0xf3,0x1 << 2);
    updatereg(24,0xfc,0x3);

//*****  Reg 25
// share_ring_cap = 0;
//filt_gainmode  = 1;
//hppwd = 0;
//shunt_ldo = 0;
//ringdiv2 = 0;
// pw1_IQ = 0;
// pw_ldo_d = 2;
    updatereg(25,0x7f,0x0);				// 6dB off
    updatereg(25,0xbf,0x1 << 6);
if(rf_in==1)
	updatereg(25,0xdf,0x1 << 5);			// HPF on
else
	updatereg(25,0xdf,0x0 << 5);			// HPF on
	updatereg(25,0xef,0x0);
    updatereg(25,0xf7,0x0);
    updatereg(25,0xfb,0x0);
    updatereg(25,0xfc,0x2);
//*****  Reg 26
// filtgain = 2;
// hprsw = 2;
// pwd_porf = 0;
// pw_porf = 2;

    updatereg(26,0x3f,0x1 << 6);			// // +3dB
 	updatereg(26,0xc7,0x2 << 3);						
    updatereg(26,0xf8,0x2);

//*****  Reg 27
//    vth = 0;
//    vtl = 0;
//    filt_dect = 0;
    updatereg(27,0x00,0x0);

    if (freq<= 140000)
        updatereg(24,0xf3,0x3 << 2);
    else if (freq >  140000 && freq <= 300000)
        updatereg(24,0xf3,0x2 << 2);
    else if (freq >  300000 && freq <= 588000)
        updatereg(24,0xf3,0x1 << 2);
    else if (freq >  588000 )
        updatereg(24,0xf3,0x0 << 2);

//*******************************************************

    if (freq <= 70000)
    {
        tf =31;
        itf = 3 ;
    }
    if (freq >  70000 && freq <= 80000)
    {
        tf =28;
        itf = 3 ;
    }
    if (freq >  80000 && freq <= 90000)
    {
        tf = 19;
        itf = 3 ;
    }
    if (freq >  90000 && freq <= 100000)
    {
        tf =16;
        itf = 3 ;
    }
    if (freq >  100000 && freq <= 120000)
    {
        tf =10;
        itf = 3 ;
    }
    if (freq >  120000 && freq <= 140000)
    {
        tf =7;
        itf = 3 ;
    }
    if (freq >  140000 && freq <= 170000)
    {
        tf =3;
        itf = 3 ;
    }
    if (freq >  170000 && freq <= 200000)
    {
        tf =2;
        itf = 3 ;
    }
    if (freq >  200000 && freq <= 270000)
    {
        tf =0;
        itf = 3 ;
    }

    if (freq >  270000 && freq <= 340000  )
    {
        tf =0;
        rfmux = 1;
        itf = 3 ;
    }
    if (freq >  340000 && freq <= 440000  )
    {
        tf =0;
        rfmux = 1;
        itf = 7 ;
    }
    if (freq >  440000 && freq <= 520000  )
    {
        tf =0;
        rfmux = 1;
        itf = 12 ;
    }
    if (freq >  520000 && freq <= 700000  )
    {
        tf =0;
        rfmux = 1;
        itf = 13 ;
    }
    if (freq >  700000 && freq <= 900000  )
    {
        tf =0;
        rfmux = 1;
        itf = 14 ;
    }
    if (freq >  900000  )
    {
        tf =0;
        rfmux = 1;
        itf = 15 ;
    }


// Tracking filter selection
    /*
    	if (freq < 95000)
    	{
    		tf =31;
           }
    	else if (freq >=  95000 && freq < 140000)
           {
                tf =12;

            }
        	else if (freq >= 140000 && freq <= 190000)
           {
                	tf = 5;
           }
        	else if (freq >=  190000  &&  freq < 280000)
           {
                	tf =0;
            }
    	else
    		rfmux=1;

    // RF LPF/HPF filter
    	if (freq <= 340000)
    	{
    		itf =3;
           }
    	else if (freq >  340000 && freq <= 440000)
           {
                itf =7;

            }
        	else if (freq > 440000 && freq <= 520000)
           {
                	itf = 12;
           }
        	else if (freq >  520000  &&  freq <= 700000)
           {
                	itf =13;
            }
        	else if (freq >  700000  &&  freq <= 900000)
           {
                	itf =14;
            }
    	else
    		itf=15;
    */
    updatereg(5,0xbf,rfmux << 6);			// rf_mux
    updatereg(5,0xe0,tf );
	if (rf_in ==0)
    	updatereg(23,0xf0,itf);
	else
	updatereg(23,0xf0,0x0f);
// 	drv_tuner_write_rt810(RT810RegsMap);

}


UINT32 cali_initialreg(UINT32 LOFrequency)
{
    UINT8 lna_gain_mode,lna_gain,mix_gain_mode,mix_gain ,rfmux=0 ,polyrfmux;
    UINT8 ring , ring_lf ,ringdiv2,tf ,pw_ring;
    UINT32 PLLLOFrequency;
// *****Reg 3
// UINT8 pw_lt = 3;
// UINT8 rfin_sw = 0;
// UINT8 lna_gain_mode = 0;
// UINT8 lna_gain = 3;

    updatereg(3,0x3f,0xc0);
// 	updatereg(3,0xdf,0x0);
// 	updatereg(3,0xef,0x0);
//	updatereg(3,0xf0,0x03);

//***** Reg 4, lna gain pdetec1
// UINT8 ring_pwd = 1;
// UINT8 pwd_lna = 0;
// UINT8 pw1_0_lna = 1;
// UINT8 pwd_pdetec1 = 0;
// UINT8 pdetec1 = 1;

    updatereg(4,0x7f,0x1<< 7);
    updatereg(4,0xbf,0x1 <<6);
    updatereg(4,0xcf,0x10);
    updatereg(4,0xf7,0x0);
    updatereg(4,0xf8,0x02);

//***** Reg 5
// UINT8 hfgain_up = 0;
// UINT8 rfmux = 0;
// UINT8 tf_dvbt = 0;
// UINT8 tf = 0;

    updatereg(5,0x7f,0x0);
//	updatereg(5,0xbf,0x0);
    updatereg(5,0xdf,0x0);
// 	updatereg(5,0xe0,0x0);

// ***** Reg 6
// UINT8 pw_ring = 0;
// UINT8 pa74_enb = 0;
// UINT8 mix_gain_mode = 1;
// UINT8 mix_gain = 10;
    updatereg(6,0x3f,0x0);
    updatereg(6,0xdf,0x0);
    updatereg(6,0xef,0x10);
    updatereg(6,0xf0,0x0a);

//*****  Reg 7, mixer TOP pdetec2
// UINT8 pwd_mix = 0;
// UINT8 lna_top_sw = 0;
// UINT8 pw0_mix = 1;
// UINT8 pw0_amp = 1;
// UINT8 pdetec2 = 2;
    updatereg(7,0x7f,0x0);
    updatereg(7,0xbf,0x0);
    updatereg(7,0xdf,0x1 << 5);
    updatereg(7,0xef,0x1 << 4);
    updatereg(7,0xf0,0x02);

//*****  Reg 8
// UINT8 pwd_IQ = 0;
// UINT8 pw0_IQ = 1;
    updatereg(8,0x7f,0x0);
    updatereg(8,0xbf,0x1 << 6);
    updatereg(8,0xc0,0);

//*****  Reg 9
// UINT8 pwd_poly = 0;
// UINT8 pw0_poly = 1;
    updatereg(9,0x7f,0x0);
    updatereg(9,0xbf,0x1 << 6);
    updatereg(9,0xc0,0);

//*****  Reg 10
// UINT8 pwd_filt = 0;
// UINT8 pw1_0_filt = 3;
// UINT8 enb_tune = 1;
// UINT8 cor = 12;
    updatereg(10,0x7f,0x0);
    updatereg(10,0x9f,0x3 << 5);
    updatereg(10,0xef,0x1 << 4);
    updatereg(10,0xf0,0x0c);

//*****  Reg 11
// UINT8 edge_delay2 = 0;
// UINT8 edge_delay = 0;
// UINT8 v7M = 1;
// UINT8 v6M = 1;
// UINT8 vstart = 0;
// UINT8 pwd_adc = 0;
// UINT8 pwd_vga = 0;
    updatereg(11,0x1f,0x0);
    updatereg(11,0xef,0x1 << 4);
    updatereg(11,0xf7,0x1 << 3);

    updatereg(11,0xfb,0x0);
    updatereg(11,0xfd,0x0);
    updatereg(11,0xfe,0x0);

//*****  Reg 12
// UINT8 vga_mode = 0;
// UINT8 dmode = 0;
// UINT8 vga_code = 60;
    updatereg(12,0x7f,0x0);				// VGA code mode
    updatereg(12,0x80,60);

//*****  Reg 13
// UINT8 lnavthh = 8;
// UINT8 lnavthl = 3;
    updatereg(13,0x0f,0x8 << 4);
    updatereg(13,0xf0,0x3);

//*****  Reg 14
// UINT8 mix_vthh = 5;
// UINT8 mix_vthl = 3;
    updatereg(14,0x0f,0x5 << 4);
    updatereg(14,0xf0,0x3);


//*****  Reg 15
// UINT8 agc_clks = 2;
// UINT8 pllauto_clks = 0;
// UINT8 clk_ring_enb = 0;
// UINT8 clk_file_enb = 0;
// UINT8 clk_agc_enb = 0;
// UINT8 gpio_in = 0;
    updatereg(15,0x3f,0x2 << 6);
    updatereg(15,0xcf,0x0);
    updatereg(15,0xf7,0x0);
    updatereg(15,0xfb,0x0);
    updatereg(15,0xfd,0x0);
    updatereg(15,0xfe,0x0);

//*****  Reg 16
// UINT8 sel_div = 1;
// UINT8 ring = 4;
    updatereg(16,0x1f,0x1 << 5);
// 	updatereg(16,0xe0,0x4);

//*****  Reg 17
// UINT8 pwd_vco_out = 0;
// UINT8 cp_0406 = 0;
// UINT8 pw_cp = 2;
// UINT8 pwd_bias = 0;
// UINT8 pw_hfd = 3;
    updatereg(17,0x7f,0x0);
    updatereg(17,0xbf,0x0);
    updatereg(17,0xc7,0x2 << 3);
    updatereg(17,0xfb,0x0);
    updatereg(17,0xfc,0x3);

//*****  Reg 18
// UINT8 pw_vco = 3;
// UINT8 pw_dither = 0;
// UINT8 pw_sdm = 0;
// UINT8 pw_sdmout = 0;
// UINT8 pw_ldo_a = 0;
// UINT8 mixhg_enb = 0;
    updatereg(18,0x1f,0x3 << 5);
    updatereg(18,0xef,0x0);
    updatereg(18,0xf7,0x0);
    updatereg(18,0xfb,0x0);
    updatereg(18,0xfc,0x0);

//*****  Reg 19
// UINT8  pa74par = 0;
// UINT8  pa74_lp = 0;
// UINT8  pa74hp = 0;
    updatereg(19,0x00,0x0);

//*****  Reg 23
// UINT8 pw1_vga = 1;
// UINT8 clk_out_enb = 1;
// UINT8 pw45 = 0;
// UINT8 itf = 15;
    updatereg(23,0x7f,0x1 << 7);
    updatereg(23,0xbf,0x1 << 6);
    updatereg(23,0xcf,0x0);
    updatereg(23,0xf0,0xf);


//*****  Reg 24
// UINT8 ring_lf = 1;
// UINT8 refdiv2 = 0;
// UINT8 ringldo = 0;
// UINT8 polyrfmux = 2;
// UINT sw_res = 0;
// UINT8 offset = 1;

// 	updatereg(24,0x3f,0x1 << 6);
// 	updatereg(24,0xdf,0x0);
    updatereg(24,0xef,0x0);
// 	updatereg(24,0xf3,0x2 << 2);
    updatereg(24,0xfc,0x1);

//*****  Reg 25
// UINT8 share_ring_cap  = 0;
// UINT8 filt_gainmode  = 1;
// UINT8 hppwd = 0;
// UINT8 shunt_ldo = 0;
// UINT8 ringdiv2 = 0;
// UINT8 pw1_IQ = 0;
// UINT8 pw_ldo_d = 2;
    updatereg(25,0x7f,0);
    updatereg(25,0xbf,0x1 << 6);
    updatereg(25,0xdf,0x0 << 5);		// HPF on
    updatereg(25,0xef,0x0);
    updatereg(25,0xf7,0x0);
    updatereg(25,0xfb,0x0);
    updatereg(25,0xfc,0x2);

//*****  Reg 26
// UINT8 filtgain = 3;
// UINT8 hprsw = 0;
// UINT8 pwd_porf = 0;
// UINT8 pw_porf = 0;
    updatereg(26,0x3f,0x3 << 6);
    updatereg(26,0xc7,0x0);
    updatereg(26,0xf8,0x0);

//*****  Reg 27
//    vth = 0;
//    vtl = 0;
//    filt_dect = 0;
    updatereg(27,0x00,0x0);

    if (LOFrequency > 48000 && LOFrequency <= 70000)
        tf=31;
    else if (LOFrequency > 70000 && LOFrequency <= 80000)
        tf=28;
    else if (LOFrequency > 80000 && LOFrequency <= 90000)
        tf=19;
    else if (LOFrequency > 90000 && LOFrequency <= 100000)
        tf=16;
    else if (LOFrequency > 100000 && LOFrequency <= 120000)
        tf=10;
    else if (LOFrequency > 120000 && LOFrequency <= 140000)
        tf=7;
    else if (LOFrequency > 140000 && LOFrequency <= 170000)
        tf=3;
    else if (LOFrequency > 170000 && LOFrequency <= 200000)
        tf=2;
    else if (LOFrequency > 200000 && LOFrequency <= 270000)
        tf=0;
    else
    {
        tf=0;
        rfmux=1;
    }

    ringdiv2 = 0;
    pw_ring = 0;
    polyrfmux = 0;
    if (LOFrequency <= 90625)
    {
        lna_gain_mode=0;
        lna_gain = 3;
        mix_gain_mode=0;
        mix_gain = 12;

        rfmux = 0;		// tracking filter


        PLLLOFrequency = RINGPLL0 - RINGLO;  //122000 ;			// RingPLL - LO = 6MHz
        ring =2 ;						// RingPLL 128MHz
        ring_lf = 0;
        ringdiv2 = 1;
        polyrfmux = 3;

    }
    else if (LOFrequency > 90625 && LOFrequency <= 268000)
    {
        lna_gain_mode=0;
        lna_gain = 3;
        mix_gain_mode=0;
        mix_gain = 12;

    }
    else if (LOFrequency > 268000 && LOFrequency <= 725000)
    {
        lna_gain_mode=0;
        lna_gain = 3;
        mix_gain_mode=0;
        mix_gain = 12;
    }
    else
    {
        pw_ring = 1;
        lna_gain_mode=0;
        lna_gain = 3;
        mix_gain_mode=0;
        mix_gain = 12;

    }


    if (LOFrequency >  90625 && LOFrequency <= 108000)
    {
        PLLLOFrequency = RINGPLL1 - RINGLO ;
        ring =1 ;
        ring_lf = 1;
        polyrfmux = 3;
    }
    else if (LOFrequency >  108000 && LOFrequency <= 140000)
    {
        PLLLOFrequency = RINGPLL2 - RINGLO ;
        ring =2 ;
        ring_lf = 1;
        polyrfmux = 3;
    }
    else if (LOFrequency >  140000 && LOFrequency <= 181250)
    {
        PLLLOFrequency = RINGPLL3 - RINGLO ;
        ring =3 ;
        ring_lf = 1;
        polyrfmux = 2;
    }
    else if (LOFrequency >  181250 && LOFrequency <= 204000)
    {
        PLLLOFrequency = RINGPLL4 - RINGLO ;
        ring =4 ;
        ring_lf = 1;
        polyrfmux = 2;
    }
    else if (LOFrequency >  204000 && LOFrequency <= 236000)
    {
        PLLLOFrequency = RINGPLL5 - RINGLO ;
        ring =5 ;
        ring_lf = 2;
        polyrfmux = 2;
    }
    else if (LOFrequency >  236000 && LOFrequency <= 268000)
    {
        PLLLOFrequency = RINGPLL6 - RINGLO ;
        ring =6 ;
        ring_lf = 2;
        polyrfmux = 2;
    }
    else if (LOFrequency >  268000 && LOFrequency <= 300000)
    {
        PLLLOFrequency = RINGPLL7 - RINGLO ;
        ring =7 ;
        ring_lf = 2;
        polyrfmux = 2;
    }
    else if (LOFrequency >  300000 && LOFrequency <= 332000)
    {
        PLLLOFrequency = RINGPLL8 - RINGLO ;
        ring =8 ;
        ring_lf = 2;
        polyrfmux = 1;
    }
    else if (LOFrequency >  332000 && LOFrequency <= 362500)
    {
        PLLLOFrequency = RINGPLL9 - RINGLO ;
        ring =9 ;
        ring_lf = 2;
        polyrfmux = 1;
    }
    else if (LOFrequency >  362500 && LOFrequency <= 396000)
    {
        PLLLOFrequency = RINGPLL10 - RINGLO ;
        ring =10 ;
        ring_lf = 2;
        polyrfmux = 1;
    }
    else if (LOFrequency >  396000 && LOFrequency <= 428000)
    {
        PLLLOFrequency = RINGPLL11 - RINGLO ;
        ring =11 ;
        ring_lf = 2;
        polyrfmux = 1;
    }
    else if (LOFrequency >  428000 && LOFrequency <= 460000)
    {
        PLLLOFrequency = RINGPLL12 - RINGLO ;
        ring =12 ;
        ring_lf = 2;
        polyrfmux = 1;
    }
    else if (LOFrequency >  460000 && LOFrequency <= 492000)
    {
        PLLLOFrequency = RINGPLL13 - RINGLO ;
        ring =13 ;
        ring_lf = 3;
        polyrfmux = 1;
    }
    else if (LOFrequency >  492000 && LOFrequency <= 524000)
    {
        PLLLOFrequency = RINGPLL14 - RINGLO ;
        ring =14 ;
        ring_lf = 3;
        polyrfmux = 1;
    }
    else if (LOFrequency >  524000 && LOFrequency <= 556000)
    {
        PLLLOFrequency = RINGPLL15 - RINGLO ;
        ring =15 ;
        ring_lf = 3;
        polyrfmux = 1;
    }
    else if (LOFrequency >  556000 && LOFrequency <= 588000)
    {
        PLLLOFrequency = RINGPLL16 - RINGLO ;
        ring =16 ;
        ring_lf = 3;
        polyrfmux = 1;
    }
    else if (LOFrequency >  588000 && LOFrequency <= 620000)
    {
        PLLLOFrequency = RINGPLL17 - RINGLO ;
        ring =17 ;
        ring_lf = 3;
    }
    else if (LOFrequency >  620000 && LOFrequency <= 652000)
    {
        PLLLOFrequency = RINGPLL18 - RINGLO ;
        ring =18 ;
        ring_lf = 3;
    }
    else if (LOFrequency >  652000 && LOFrequency <= 684000)
    {
        PLLLOFrequency = RINGPLL19 - RINGLO ;
        ring =19 ;
        ring_lf = 3;
    }
    else if (LOFrequency >  684000 && LOFrequency <= 725000)
    {
        PLLLOFrequency = RINGPLL20 - RINGLO6M ;
        ring =20 ;
        ring_lf = 3;
    }
    else if (LOFrequency >  725000 && LOFrequency <= 748000)
    {
        PLLLOFrequency = RINGPLL21 - RINGLO6M ;
        ring =21 ;
        ring_lf = 3;
    }
    else if (LOFrequency >  748000 && LOFrequency <= 780000)
    {
        PLLLOFrequency = RINGPLL22 - RINGLO6M ;
        ring =22 ;
        ring_lf = 3;
    }
    else if (LOFrequency >  780000 && LOFrequency <= 812000)
    {
        PLLLOFrequency = RINGPLL23 - RINGLO6M ;
        ring =23 ;
        ring_lf = 3;
    }
    else if (LOFrequency >  812000 && LOFrequency <= 844000)
    {
        PLLLOFrequency = RINGPLL24 - RINGLO6M ;
        ring =24 ;
        ring_lf = 3;
    }
    else if (LOFrequency >  844000 && LOFrequency <= 876000)
    {
        PLLLOFrequency = RINGPLL25 - RINGLO6M ;
        ring =25 ;
        ring_lf = 3;
    }
    else if (LOFrequency >  876000 && LOFrequency <= 908000)
    {
        PLLLOFrequency = RINGPLL26 - RINGLO6M ;
        ring =26 ;
        ring_lf = 3;
    }
    else if (LOFrequency >  908000 && LOFrequency <= 940000)
    {
        PLLLOFrequency = RINGPLL27 - RINGLO6M ;
        ring =27 ;
        ring_lf = 3;
    }
    else if (LOFrequency >  940000 && LOFrequency <= 972000)
    {
        PLLLOFrequency = RINGPLL28 - RINGLO6M ;
        ring =28 ;
        ring_lf = 3;
    }
    else if (LOFrequency >  972000 && LOFrequency <= 1004000)
    {
        PLLLOFrequency = RINGPLL29 - RINGLO6M ;
        ring =29 ;
        ring_lf = 3;
    }
    else if (LOFrequency >  1004000 && LOFrequency <= 1036000)
    {
        PLLLOFrequency = RINGPLL30 - RINGLO6M ;
        ring =30 ;
        ring_lf = 3;
    }
    else if (LOFrequency >  1036000)
    {
        PLLLOFrequency = RINGPLL31 - RINGLO6M ;
        ring =31 ;
        ring_lf = 3;
    }

    updatereg(5,0xbf,rfmux << 6);
    updatereg(5,0xe0,tf);
    updatereg(16,0xe0,ring);
    updatereg(24,0x3f,ring_lf << 6);
    updatereg(24,0xf3,polyrfmux<< 2);
    updatereg(6,0xef,mix_gain_mode << 4);
    updatereg(6,0xf0,mix_gain);
    updatereg(3,0xef,lna_gain_mode << 4);
    updatereg(3,0xf0,lna_gain+3);
    updatereg(25,0xf7,ringdiv2 << 3);
    updatereg(6,0x3f,pw_ring << 6);


    drv_tuner_write_rt810(RT810RegsMap);
    return PLLLOFrequency;

}
#else
void initialreg(UINT32 freq)
{
    UINT8 rfmux=0,tf,itf;

    // *****Reg 3
// pw_lt = 3;
// rfin_sw = 0;
// lna_gain_mode = 1;
// lna_gain = 9;

    updatereg(3,0x3f,0xc0);

    if (rf_in==0)
        updatereg(3,0xdf,0x0);
    else
        updatereg(3,0xdf,0x1 << 5);

    updatereg(3,0xef,0x10);
    updatereg(3,0xf0,0x09);

//***** Reg 4
// ring_pwd = 1;
// pwd_lna = 0;
// pw1_0_lna = 1;
// pwd_pdetec1 = 0;
// pdetec1 = 2;	// +6dB

    updatereg(4,0x7f,0x80);
    updatereg(4,0xbf,0x0);
    updatereg(4,0xcf,0x10);
    updatereg(4,0xf7,0x0);
    updatereg(4,0xf8,0x02);

//***** Reg 5
// ringsw = 1;
// rfmux = 0;
// tf_dvbt = 1;
// tf = 0;

    updatereg(5,0x7f,0x80);
    updatereg(5,0xbf,0x0);
    updatereg(5,0xdf,0x20);
    updatereg(5,0xe0,0x0);

// ***** Reg 6
// pw_ring = 0;
// Si2c_MSB = 0;
// mix_gain_mode = 1;
// mix_gain = 10;

    updatereg(6,0x3f,0x0);
    updatereg(6,0xdf,0x0);
    updatereg(6,0xef,0x10);
    updatereg(6,0xf0,0x0a);

//*****  Reg 7
// pwd_mix = 0;
// pw1_0_mix = 2;
// pw0_amp = 1;
// pdetec2 = 2;		// 13

    updatereg(7,0x7f,0x0);
    updatereg(7,0x9f,0x2 << 5);			// Mixer current low, 3 is lowest
    updatereg(7,0xef,0x1 << 4);
    updatereg(7,0xf0,0x02);

//*****  Reg 8
// pwd_IQ = 0;
// pw0_IQ = 1;
//gans = 0;
//gans4 = 0;
    updatereg(8,0x7f,0x0);
    updatereg(8,0xbf,0x1 << 6);

//*****  Reg 9
// pwd_poly = 0;
// pw0_poly = 1;
//phs = 0;
//phs4 = 0;
    updatereg(9,0x7f,0x0);
// 	updatereg(9,0xbf,0x1 << 6);
    updatereg(9,0xbf,0x0);

//*****  Reg 10
// pwd_filt = 0;
// pw1_0_filt = 3;
// enb_tune = 0;
// cor = 12;

    updatereg(10,0x7f,0x0);
    updatereg(10,0x9f,0x3 << 5);
    updatereg(10,0xef,0x0);
    updatereg(10,0xf0,0x0c);
//*****  Reg 11
// edge_delay2 = 0;
// edge_delay = 0;
// v7M = 0;
// v6M = 0;
// vstart = 0;
// pwd_adc = 1;
// pwd_vga = 0;

    updatereg(11,0x1f,0x0);
    updatereg(11,0xfb,0x0);
    updatereg(11,0xfd,0x1 << 1);
    updatereg(11,0xfe,0x0);

//*****  Reg 12
// vga_mode = Tvga_mode;
// offset0 = 0;
// vga_code = Tvga_code;

    updatereg(12,0x7f,0x1 <<7);				// VGA Pin mode
    updatereg(12,0x80,20);

// lnavthh = 8;
// lnavthl = 3;
    updatereg(13,0x0f,0x8 << 4);
    updatereg(13,0xf0,0x3);

// mix_vthh = 5;
// mix_vthl = 3;
    updatereg(14,0x0f,0x6 << 4);
    updatereg(14,0xf0,0x3);

//*****  Reg 15
// agc_clks = 1;
// pllauto_clks = 0;
// clk_ring_enb = 1;
// clk_file_enb = 0;
// clk_agc_enb = 0;
// gpio_in = 0;
    updatereg(15,0x3f,0x2 << 6);
    updatereg(15,0xcf,0);
    updatereg(15,0xf7,0x1 << 3);
    updatereg(15,0xfb,0x0);
    updatereg(15,0xfd,0x0);
    updatereg(15,0xfe,0x0);
//*****  Reg 16
// sel_div = 2;
// ring = 12;
    updatereg(16,0x1f,0x2 << 5);
    updatereg(16,0xe0,0x0);

//*****  Reg 17
// pwd_vco_out = 0;
// cp_0406 = 0;
// pw_cp = 2;
// pwd_bias = 0;
// pw_hfd = 3;
    updatereg(17,0x7f,0x0);
    updatereg(17,0xbf,0x0);
    updatereg(17,0xc7,0x2 << 3);
    updatereg(17,0xfb,0x0);
    updatereg(17,0xfc,0x3);
//*****  Reg 18
// pw_vco = 3;
// pw_dither = 1;
// pw_sdm = 0;
// pw_sdmout = 0;
// pw_ldo_a = 0;
// pw_ldo_b = 0;
    updatereg(18,0x1f,0x2 << 5);
    updatereg(18,0xef,0x1 << 4);
    updatereg(18,0xf7,0x0);
    updatereg(18,0xfb,0x0);
    updatereg(18,0xfd,0x0);
//*****  Reg 19
//pw_auto = 0;
//sel_vco = 0;

    updatereg(19,0x7f,0x0);
    updatereg(19,0x80,0x0);
//*****  Reg 23
// pw1_vga = 1;
// clk_out_enb = 0;
// pw45 = 0;
// itf = 15;
    updatereg(23,0x7f,0);				// VGA power high.
    updatereg(23,0xbf,0x0);
    updatereg(23,0xcf,0x0);
    updatereg(23,0xf0,0xf);

//*****  Reg 24
//ring_lf = 0;
//refdiv2 = 0;
//ringldo = 0;
//polyrfmux = 1;
//offset = 1;
    updatereg(24,0x3f,0x0);
    updatereg(24,0xdf,0x0);
    updatereg(24,0xef,0x0);
    updatereg(24,0xf3,0x1 << 2);
    updatereg(24,0xfc,0x1);

//*****  Reg 25
// filt6db = 1;
//sh_pdect = 0;
//hpbypass = 0;
//shunt_ldo = 0;
//ringdiv2 = 0;
// pw1_IQ = 0;
// pw_ldo_d = 2;
    updatereg(25,0x7f,0x0);				// 6dB off
    updatereg(25,0xbf,0x0);
    updatereg(25,0xdf,0x0);
    updatereg(25,0xef,0x0);
    updatereg(25,0xf7,0x0);
    updatereg(25,0xfb,0x0);
    updatereg(25,0xfc,0x2);
//*****  Reg 26
//v5m_hpf = 1;
// pwd_porf = 0;
// pw_porf = 0;

    updatereg(26,0xf7,0x1 << 3);
    updatereg(26,0xfb,0x0);
    updatereg(26,0xfc,0x0);


    if (freq<= 140000)
        updatereg(24,0xf3,0x3 << 2);
    else if (freq >  140000 && freq <= 300000)
        updatereg(24,0xf3,0x2 << 2);
    else if (freq >  300000 && freq <= 588000)
        updatereg(24,0xf3,0x1 << 2);
    else if (freq >  588000 )
        updatereg(24,0xf3,0x0 << 2);

//*******************************************************
    if (freq <= 70000)
    {
        tf =31;
        itf = 3 ;
    }
    if (freq >  70000 && freq <= 80000)
    {
        tf =28;
        itf = 3 ;
    }
    if (freq >  80000 && freq <= 90000)
    {
        tf = 19;
        itf = 3 ;
    }
    if (freq >  90000 && freq <= 100000)
    {
        tf =16;
        itf = 3 ;
    }
    if (freq >  100000 && freq <= 120000)
    {
        tf =10;
        itf = 3 ;
    }
    if (freq >  120000 && freq <= 140000)
    {
        tf =7;
        itf = 3 ;
    }
    if (freq >  140000 && freq <= 170000)
    {
        tf =3;
        itf = 3 ;
    }
    if (freq >  170000 && freq <= 200000)
    {
        tf =2;
        itf = 3 ;
    }
    if (freq >  200000 && freq <= 270000)
    {
        tf =0;
        itf = 3 ;
    }

    if (freq >  270000 && freq <= 340000  )
    {
        tf =0;
        rfmux = 1;
        itf = 3 ;
    }
    if (freq >  340000 && freq <= 440000  )
    {
        tf =0;
        rfmux = 1;
        itf = 7 ;
    }
    if (freq >  440000 && freq <= 520000  )
    {
        tf =0;
        rfmux = 1;
        itf = 12 ;
    }
    if (freq >  520000 && freq <= 700000  )
    {
        tf =0;
        rfmux = 1;
        itf = 13 ;
    }
    if (freq >  700000 && freq <= 900000  )
    {
        tf =0;
        rfmux = 1;
        itf = 14 ;
    }
    if (freq >  900000  )
    {
        tf =0;
        rfmux = 1;
        itf = 15 ;
    }

    updatereg(5,0xbf,rfmux << 6);			// rf_mux
    updatereg(5,0xe0,tf );
    updatereg(23,0xf0,itf);
    drv_tuner_write_rt810(RT810RegsMap);

}


UINT32 cali_initialreg(UINT32 LOFrequency)
{
    UINT8 lna_gain_mode,lna_gain,mix_gain_mode,mix_gain ,rfmux ,polyrfmux;
    UINT8 ring , ring_lf ,ringdiv2,tf ,pw_ring;
    UINT32 PLLLOFrequency;
// *****Reg 3
// UINT8 pw_lt = 3;
// UINT8 rfin_sw = 0;
// UINT8 lna_gain_mode = 0;
// UINT8 lna_gain = 3;

    updatereg(3,0x3f,0xc0);
    updatereg(3,0xdf,0x0);
// 	updatereg(3,0xef,0x0);
//	updatereg(3,0xf0,0x03);

//***** Reg 4, lna gain pdetec1
// UINT8 ring_pwd = 1;
// UINT8 pwd_lna = 0;
// UINT8 pw1_0_lna = 1;
// UINT8 pwd_pdetec1 = 0;
// UINT8 pdetec1 = 1;

    updatereg(4,0x7f,0x1<< 7);
    updatereg(4,0xbf,0x0);
    updatereg(4,0xcf,0x10);
    updatereg(4,0xf7,0x0);
    updatereg(4,0xf8,0x02);

//***** Reg 5
// UINT8 ringsw = 0;
// UINT8 rfmux = 0;
// UINT8 tf_dvbt = 0;
// UINT8 tf = 0;

    updatereg(5,0x7f,0x0);
//	updatereg(5,0xbf,0x0);
    updatereg(5,0xdf,0x0);
// 	updatereg(5,0xe0,0x0);

// ***** Reg 6
// UINT8 pw_ring = 0;
// UINT8 Si2c_MSB = 0;
// UINT8 mix_gain_mode = 1;
// UINT8 mix_gain = 10;
// 	updatereg(6,0x3f,0x0);
    updatereg(6,0xdf,0x0);
// 	updatereg(6,0xef,0x10);
//	updatereg(6,0xf0,0x0a);

//*****  Reg 7, mixer TOP pdetec2
// UINT8 pwd_mix = 0;
// UINT8 pw1_0_mix = 2;
// UINT8 pw0_amp = 1;
// UINT8 pdetec2 = 2;
    updatereg(7,0x7f,0x0);
    updatereg(7,0x9f,0x2 << 5);
    updatereg(7,0xef,0x1 << 4);
    updatereg(7,0xf0,0x02);

//*****  Reg 8
// UINT8 pwd_IQ = 0;
// UINT8 pw0_IQ = 1;
    updatereg(8,0x7f,0x0);
    updatereg(8,0xbf,0x1 << 6);
    updatereg(8,0xc0,0);

//*****  Reg 9
// UINT8 pwd_poly = 0;
// UINT8 pw0_poly = 1;
    updatereg(9,0x7f,0x0);
    updatereg(9,0xbf,0x1 << 6);
    updatereg(9,0xc0,0);

//*****  Reg 10
// UINT8 pwd_filt = 0;
// UINT8 pw1_0_filt = 3;
// UINT8 enb_tune = 1;
// UINT8 cor = 12;
    updatereg(10,0x7f,0x0);
    updatereg(10,0x9f,0x3 << 5);
    updatereg(10,0xef,0x1 << 4);
    updatereg(10,0xf0,0x0c);

//*****  Reg 11
// UINT8 edge_delay2 = 0;
// UINT8 edge_delay = 0;
// UINT8 v7M = 1;
// UINT8 v6M = 1;
// UINT8 vstart = 0;
// UINT8 pwd_adc = 0;
// UINT8 pwd_vga = 0;
    updatereg(11,0x1f,0x0);
    updatereg(11,0xef,0x1 << 4);
    updatereg(11,0xf7,0x1 << 3);

    updatereg(11,0xfb,0x0);
    updatereg(11,0xfd,0x0);
    updatereg(11,0xfe,0x0);

//*****  Reg 12
// UINT8 vga_mode = 0;
// UINT8 offset0 = 0;
// UINT8 vga_code = 60;
    updatereg(12,0x7f,0x0);				// VGA code mode
    updatereg(12,0x80,60);

//*****  Reg 13
// UINT8 lnavthh = 8;
// UINT8 lnavthl = 3;
    updatereg(13,0x0f,0x8 << 4);
    updatereg(13,0xf0,0x3);

//*****  Reg 14
// UINT8 mix_vthh = 5;
// UINT8 mix_vthl = 3;
    updatereg(14,0x0f,0x5 << 4);
    updatereg(14,0xf0,0x3);


//*****  Reg 15
// UINT8 agc_clks = 2;
// UINT8 pllauto_clks = 0;
// UINT8 clk_ring_enb = 0;
// UINT8 clk_file_enb = 0;
// UINT8 clk_agc_enb = 0;
// UINT8 gpio_in = 0;
    updatereg(15,0x3f,0x2 << 6);
    updatereg(15,0xcf,0x0);
    updatereg(15,0xf7,0x0);
    updatereg(15,0xfb,0x0);
    updatereg(15,0xfd,0x0);
    updatereg(15,0xfe,0x0);

//*****  Reg 16
// UINT8 sel_div = 1;
// UINT8 ring = 4;
    updatereg(16,0x1f,0x1 << 5);
// 	updatereg(16,0xe0,0x4);

//*****  Reg 17
// UINT8 pwd_vco_out = 0;
// UINT8 cp_0406 = 0;
// UINT8 pw_cp = 2;
// UINT8 pwd_bias = 0;
// UINT8 pw_hfd = 3;
    updatereg(17,0x7f,0x0);
    updatereg(17,0xbf,0x0);
    updatereg(17,0xc7,0x2 << 3);
    updatereg(17,0xfb,0x0);
    updatereg(17,0xfc,0x3);

//*****  Reg 18
// UINT8 pw_vco = 3;
// UINT8 pw_dither = 0;
// UINT8 pw_sdm = 0;
// UINT8 pw_sdmout = 0;
// UINT8 pw_ldo_a = 0;

    updatereg(18,0x1f,0x3 << 5);
    updatereg(18,0xef,0x0);
    updatereg(18,0xf7,0x0);
    updatereg(18,0xfb,0x0);
    updatereg(18,0xfd,0x0);

//*****  Reg 19
// UINT8 pw_auto = 0;
// UINT8 sel_vco = 11;
    updatereg(19,0x7f,0x0);
    updatereg(19,0x80,0x0b);

//*****  Reg 23
// UINT8 pw1_vga = 1;
// UINT8 clk_out_enb = 1;
// UINT8 pw45 = 0;
// UINT8 itf = 15;
    updatereg(23,0x7f,0x1 << 7);
    updatereg(23,0xbf,0x1 << 6);
    updatereg(23,0xcf,0x0);
    updatereg(23,0xf0,0xf);


//*****  Reg 24
// UINT8 ring_lf = 1;
// UINT8 refdiv2 = 0;
// UINT8 ringldo = 0;
// UINT8 polyrfmux = 2;
// UINT8 offset = 1;
// 	updatereg(24,0x3f,0x1 << 6);
// 	updatereg(24,0xdf,0x0);
    updatereg(24,0xef,0x0);
// 	updatereg(24,0xf3,0x2 << 2);
    updatereg(24,0xfc,0x1);

//*****  Reg 25
// UINT8 filt6db = 1;
// UINT8 sh_pdect = 0;
// UINT8 hpbypass = 0;
// UINT8 shunt_ldo = 0;
// UINT8 ringdiv2 = 0;
// UINT8 pw1_IQ = 0;
// UINT8 pw_ldo_d = 2;
    updatereg(25,0x7f,0x1 << 7);
    updatereg(25,0xbf,0x0);
    updatereg(25,0xdf,0x0);
    updatereg(25,0xef,0x0);
    updatereg(25,0xf7,0x0);
    updatereg(25,0xfb,0x0);
    updatereg(25,0xfc,0x2);

//*****  Reg 26
// UINT8 v5m_hpf = 1;
// UINT8 pwd_porf = 0;
// UINT8 pw_porf = 0;
    updatereg(26,0xf7,0x1 << 3);
    updatereg(26,0xfb,0x0);
    updatereg(26,0xfc,0x0);

    if (LOFrequency > 48000 && LOFrequency <= 70000)
        tf=31;
    else if (LOFrequency > 70000 && LOFrequency <= 80000)
        tf=28;
    else if (LOFrequency > 80000 && LOFrequency <= 90000)
        tf=19;
    else if (LOFrequency > 90000 && LOFrequency <= 100000)
        tf=16;
    else if (LOFrequency > 100000 && LOFrequency <= 120000)
        tf=10;
    else if (LOFrequency > 120000 && LOFrequency <= 140000)
        tf=7;
    else if (LOFrequency > 140000 && LOFrequency <= 170000)
        tf=3;
    else if (LOFrequency > 170000 && LOFrequency <= 200000)
        tf=2;
    else if (LOFrequency > 200000 && LOFrequency <= 270000)
        tf=0;
    else
    {
        tf=0;
        rfmux=1;
    }

    ringdiv2 = 0;
    pw_ring = 0;
    polyrfmux = 0;
    if (LOFrequency <= 90625)
    {
        lna_gain_mode=0;
        lna_gain = 3;
        mix_gain_mode=0;
        mix_gain = 12;

        rfmux = 0;		// tracking filter


        PLLLOFrequency = RINGPLL0 - RINGLO;  //122000 ;			// RingPLL - LO = 6MHz
        ring =2 ;						// RingPLL 128MHz
        ring_lf = 0;
        ringdiv2 = 1;
        polyrfmux = 3;

    }
    else if (LOFrequency > 90625 && LOFrequency <= 268000)
    {
        lna_gain_mode=0;
        lna_gain = 3;
        mix_gain_mode=0;
        mix_gain = 12;

    }
    else if (LOFrequency > 268000 && LOFrequency <= 725000)
    {
        lna_gain_mode=0;
        lna_gain = 3;
        mix_gain_mode=0;
        mix_gain = 12;
    }
    else
    {
        pw_ring = 1;
        lna_gain_mode=0;
        lna_gain = 3;
        mix_gain_mode=0;
        mix_gain = 12;

    }


    if (LOFrequency >  90625 && LOFrequency <= 108000)
    {
        PLLLOFrequency = RINGPLL1 - RINGLO ;
        ring =1 ;
        ring_lf = 1;
        polyrfmux = 3;
    }
    else if (LOFrequency >  108000 && LOFrequency <= 140000)
    {
        PLLLOFrequency = RINGPLL2 - RINGLO ;
        ring =2 ;
        ring_lf = 1;
        polyrfmux = 3;
    }
    else if (LOFrequency >  140000 && LOFrequency <= 181250)
    {
        PLLLOFrequency = RINGPLL3 - RINGLO ;
        ring =3 ;
        ring_lf = 1;
        polyrfmux = 2;
    }
    else if (LOFrequency >  181250 && LOFrequency <= 204000)
    {
        PLLLOFrequency = RINGPLL4 - RINGLO ;
        ring =4 ;
        ring_lf = 1;
        polyrfmux = 2;
    }
    else if (LOFrequency >  204000 && LOFrequency <= 236000)
    {
        PLLLOFrequency = RINGPLL5 - RINGLO ;
        ring =5 ;
        ring_lf = 2;
        polyrfmux = 2;
    }
    else if (LOFrequency >  236000 && LOFrequency <= 268000)
    {
        PLLLOFrequency = RINGPLL6 - RINGLO ;
        ring =6 ;
        ring_lf = 2;
        polyrfmux = 2;
    }
    else if (LOFrequency >  268000 && LOFrequency <= 300000)
    {
        PLLLOFrequency = RINGPLL7 - RINGLO ;
        ring =7 ;
        ring_lf = 2;
        polyrfmux = 2;
    }
    else if (LOFrequency >  300000 && LOFrequency <= 332000)
    {
        PLLLOFrequency = RINGPLL8 - RINGLO ;
        ring =8 ;
        ring_lf = 2;
        polyrfmux = 1;
    }
    else if (LOFrequency >  332000 && LOFrequency <= 362500)
    {
        PLLLOFrequency = RINGPLL9 - RINGLO ;
        ring =9 ;
        ring_lf = 2;
        polyrfmux = 1;
    }
    else if (LOFrequency >  362500 && LOFrequency <= 396000)
    {
        PLLLOFrequency = RINGPLL10 - RINGLO ;
        ring =10 ;
        ring_lf = 2;
        polyrfmux = 1;
    }
    else if (LOFrequency >  396000 && LOFrequency <= 428000)
    {
        PLLLOFrequency = RINGPLL11 - RINGLO ;
        ring =11 ;
        ring_lf = 2;
        polyrfmux = 1;
    }
    else if (LOFrequency >  428000 && LOFrequency <= 460000)
    {
        PLLLOFrequency = RINGPLL12 - RINGLO ;
        ring =12 ;
        ring_lf = 2;
        polyrfmux = 1;
    }
    else if (LOFrequency >  460000 && LOFrequency <= 492000)
    {
        PLLLOFrequency = RINGPLL13 - RINGLO ;
        ring =13 ;
        ring_lf = 3;
        polyrfmux = 1;
    }
    else if (LOFrequency >  492000 && LOFrequency <= 524000)
    {
        PLLLOFrequency = RINGPLL14 - RINGLO ;
        ring =14 ;
        ring_lf = 3;
        polyrfmux = 1;
    }
    else if (LOFrequency >  524000 && LOFrequency <= 556000)
    {
        PLLLOFrequency = RINGPLL15 - RINGLO ;
        ring =15 ;
        ring_lf = 3;
        polyrfmux = 1;
    }
    else if (LOFrequency >  556000 && LOFrequency <= 588000)
    {
        PLLLOFrequency = RINGPLL16 - RINGLO ;
        ring =16 ;
        ring_lf = 3;
        polyrfmux = 1;
    }
    else if (LOFrequency >  588000 && LOFrequency <= 620000)
    {
        PLLLOFrequency = RINGPLL17 - RINGLO ;
        ring =17 ;
        ring_lf = 3;
    }
    else if (LOFrequency >  620000 && LOFrequency <= 652000)
    {
        PLLLOFrequency = RINGPLL18 - RINGLO ;
        ring =18 ;
        ring_lf = 3;
    }
    else if (LOFrequency >  652000 && LOFrequency <= 684000)
    {
        PLLLOFrequency = RINGPLL19 - RINGLO ;
        ring =19 ;
        ring_lf = 3;
    }
    else if (LOFrequency >  684000 && LOFrequency <= 725000)
    {
        PLLLOFrequency = RINGPLL20 - RINGLO6M ;
        ring =20 ;
        ring_lf = 3;
    }
    else if (LOFrequency >  725000 && LOFrequency <= 748000)
    {
        PLLLOFrequency = RINGPLL21 - RINGLO6M ;
        ring =21 ;
        ring_lf = 3;
    }
    else if (LOFrequency >  748000 && LOFrequency <= 780000)
    {
        PLLLOFrequency = RINGPLL22 - RINGLO6M ;
        ring =22 ;
        ring_lf = 3;
    }
    else if (LOFrequency >  780000 && LOFrequency <= 812000)
    {
        PLLLOFrequency = RINGPLL23 - RINGLO6M ;
        ring =23 ;
        ring_lf = 3;
    }
    else if (LOFrequency >  812000 && LOFrequency <= 844000)
    {
        PLLLOFrequency = RINGPLL24 - RINGLO6M ;
        ring =24 ;
        ring_lf = 3;
    }
    else if (LOFrequency >  844000 && LOFrequency <= 876000)
    {
        PLLLOFrequency = RINGPLL25 - RINGLO6M ;
        ring =25 ;
        ring_lf = 3;
    }
    else if (LOFrequency >  876000 && LOFrequency <= 908000)
    {
        PLLLOFrequency = RINGPLL26 - RINGLO6M ;
        ring =26 ;
        ring_lf = 3;
    }
    else if (LOFrequency >  908000 && LOFrequency <= 940000)
    {
        PLLLOFrequency = RINGPLL27 - RINGLO6M ;
        ring =27 ;
        ring_lf = 3;
    }
    else if (LOFrequency >  940000 && LOFrequency <= 972000)
    {
        PLLLOFrequency = RINGPLL28 - RINGLO6M ;
        ring =28 ;
        ring_lf = 3;
    }
    else if (LOFrequency >  972000 && LOFrequency <= 1004000)
    {
        PLLLOFrequency = RINGPLL29 - RINGLO6M ;
        ring =29 ;
        ring_lf = 3;
    }
    else if (LOFrequency >  1004000 && LOFrequency <= 1036000)
    {
        PLLLOFrequency = RINGPLL30 - RINGLO6M ;
        ring =30 ;
        ring_lf = 3;
    }
    else if (LOFrequency >  1036000)
    {
        PLLLOFrequency = RINGPLL31 - RINGLO6M ;
        ring =31 ;
        ring_lf = 3;
    }

    updatereg(5,0xbf,rfmux << 6);
    updatereg(5,0xe0,tf);
    updatereg(16,0xe0,ring);
    updatereg(24,0x3f,ring_lf << 6);
    updatereg(24,0xf3,polyrfmux<< 2);
    updatereg(6,0xef,mix_gain_mode << 4);
    updatereg(6,0xf0,mix_gain);
    updatereg(3,0xef,lna_gain_mode << 4);
    updatereg(3,0xf0,lna_gain+3);
    updatereg(25,0xf7,ringdiv2 << 3);
    updatereg(6,0x3f,pw_ring << 6);


    drv_tuner_write_rt810(RT810RegsMap);
    return PLLLOFrequency;

}
#endif

UINT8  VCOpll (UINT32 freq,UINT8 total)
{
    float LOFrequency;//=(freq + ChannelIF)*100;
    float VcoFrequency;
//	UINT8 b0 =0,b1=0,b2=0,b3=0,b4=0,b5=0,b6=0,b7=0 ;
    float LOFreq2;// = (float)LOFrequency * 2;
    float LOFreq4;// = (float)LOFrequency * 4;
    float LOFreq8;// = (float)LOFrequency * 8;
    float LOFreq16;// = (float)LOFrequency * 16;
    float LOFreq32;// = (float)LOFrequency * 32;
    float VcoNfra=0 ;
    float VcoDivNum =0;
    UINT8 sel_div;
    UINT8 pw_sdm=0;
    UINT8 SDM_IN_H;
    UINT8 SDM_IN_L;
    UINT8 Si2c ;
    UINT8 Ni2c;
	UINT8 ms,i=10;

    UINT32 Mut2Xtal = 16000000 * 2; //Xtal=16MHz
    float VcoDivintNum;
    int Nint;
    UINT8 ReadBuf[3];
    //freq=1682500;

    g_curr_tunerband = drv_tuner_CheckBand(freq);
    g_curr_freq= freq;

    LOFrequency=(float)freq *100;
    LOFreq2 = (float)LOFrequency * 2;
    LOFreq4 = (float)LOFrequency * 4;
    LOFreq8 = (float)LOFrequency * 8;
    LOFreq16 = (float)LOFrequency * 16;
    LOFreq32 = (float)LOFrequency * 32;

    //************Setting sel_div**********************
    if (freq >_TUNER_MAX_FREQ && freq < _TUNER_MIN_FREQ)
    {
        freq = _TUNER_MIN_FREQ;
    }

    if ((LOFreq2 >= VCOFREQ_MIN) && ( LOFreq2 <= VCOFREQ_MAX ))
    {
        sel_div = 0;
        VcoFrequency = LOFreq2 ;
    }

    if ((LOFreq4 >= VCOFREQ_MIN) && (LOFreq4 <= VCOFREQ_MAX))
    {
        sel_div = 1;
        VcoFrequency = LOFreq4 ;
    }

    if ((LOFreq8 >= VCOFREQ_MIN) && (LOFreq8 <= VCOFREQ_MAX))
    {
        sel_div = 2;
        VcoFrequency = LOFreq8 ;
    }

    if ((LOFreq16 >= VCOFREQ_MIN) && (LOFreq16 <= VCOFREQ_MAX))
    {
        sel_div = 3;
        VcoFrequency = LOFreq16 ;
    }

    if ((LOFreq32 >= VCOFREQ_MIN) && (LOFreq32 <= VCOFREQ_MAX))
    {
        sel_div = 4;
        VcoFrequency = LOFreq32 ;
    }
    //*************************************************
    VcoDivintNum = (VcoFrequency / Mut2Xtal);
    Nint = (int)VcoDivintNum;
    VcoNfra = (VcoFrequency / Mut2Xtal)- Nint ;

    Ni2c = (Nint-13) / 4 ;
    Si2c = (Nint-13) % 4 ;
    if (VcoNfra == 0)
    {
        pw_sdm = 1 ; // Integer PLL
        SDM_IN_H = 0;
        SDM_IN_L = 0;
    }
    else
    {

        VcoNfra *= 65536;
        SDM_IN_H = VcoNfra /256;
        SDM_IN_L = (UINT8) VcoNfra ;

    }

    updatereg(16,0x1f,sel_div<< 5);
    updatereg(18,0xf7,pw_sdm << 3);
    updatereg(20,0x3f,Si2c << 6);
    updatereg(20,0xc0,Ni2c );
    updatereg(21,0x00,SDM_IN_L);
    updatereg(22,0x00,SDM_IN_H);
    if (!total)
        drv_tuner_write_rt810(RT810RegsMap);
    else
        drv_tuner_write_rt810_vcoonly(RT810RegsMap);
    fw_timer_event_DelayXms(7);
//	return 0;
    drv_i2c_ContRead(I2C_MASTER_TUNER,3,ReadBuf);
    ms=ReadBuf[2];
    ReadBuf[2]=(UINT8)(((ms & 0x80) >> 7 ) + ((ms & 0x40) >> 5) + ((ms & 0x20) >> 3 ) + ((ms & 0x10) >> 1 ) +
                       ((ms & 0x08) << 1 ) + ((ms & 0x04) << 3 ) + ((ms & 0x02) << 5 ) + ((ms & 0x01) << 7 ));

    while ((ReadBuf[2]==0x2f) || (ReadBuf[2]==0x00))
    {
        updatereg(18,0xfd,1<<1);
        drv_tuner_write_rt810_vcoonly(RT810RegsMap);
        fw_timer_event_DelayXms(10);
        updatereg(18,0xfd,0);
        drv_tuner_write_rt810_vcoonly(RT810RegsMap);
        drv_i2c_ContRead(I2C_MASTER_TUNER,3,ReadBuf);
        ms=ReadBuf[2];
        ReadBuf[2]=(UINT8)(((ms & 0x80) >> 7 ) + ((ms & 0x40) >> 5) + ((ms & 0x20) >> 3 ) + ((ms & 0x10) >> 1 ) +
                           ((ms & 0x08) << 1 ) + ((ms & 0x04) << 3 ) + ((ms & 0x02) << 5 ) + ((ms & 0x01) << 7 ));
		if (--i==0)
			return 0;
    }
    return 0;
}

/*=============================================================*/
/**
	This routine will set TV to specific frequency  in Search mode
	@param			freq					frequency
	@return 			Return the status code	_TURE,_FALSE
*/
UINT8 drv_tuner_SetFreq(UINT32 freq)
{
    int min,i,j,k=0;
    UINT8 rt810_read[3],ms,x1,y1;

    UINT32 LOFrequency;
    UINT32 PLLLOFrequency;
    LOFrequency=(freq + ChannelIF)/10;

    if  (LOFrequency <= 55250)
    {
        k=0;
    }
    else if  ( (LOFrequency >55250) && (LOFrequency < 90600))
    {
        k=1;
    }
    else if  ( (LOFrequency >= 90600) && (LOFrequency < 181250))
    {
        k=2;
    }
    else if  ( (LOFrequency >= 181250) && (LOFrequency < 362500))
    {
        k=3;
    }
    else if  ( (LOFrequency >= 362500) && (LOFrequency < 576000))
    {
        k=4;
    }
    else if  ( (LOFrequency >= 576000) && (LOFrequency < 680000))
    {
        k=5;
    }
    else if  (LOFrequency >= 680000)
    {
        k=6;
    }



    /*	if ( (LOFrequency >= 700000) && (LOFrequency < 830000))
    	{
    		LOFrequency = 700000;
    	}
    	if ( (LOFrequency >= 830000) && (LOFrequency < 960000))
    	{
    		LOFrequency = 960000;
    	}
    */

    if (pk[4][0]==0x99)
    {
        PLLLOFrequency = cali_initialreg(LOFrequency);
        VCOpll(PLLLOFrequency*10,0);
        fw_timer_event_DelayXms(10);

// Turn on Ring
        updatereg(15,0xf7,0);
        updatereg(5,0x7f,0x0);
        drv_tuner_write_rt810(RT810RegsMap);
        fw_timer_event_DelayXms(1);
        updatereg(4,0x7f,0x0);
        drv_tuner_write_rt810(RT810RegsMap);
        fw_timer_event_DelayXms(1);

        for (i=0; i<3; i++)
        {
            switch (i)
            {
            case 0:
                min=52;
                break;
            case 1:
                min=56;
                break;
            case 2:
                min=60;
                break;

            }
            updatereg(12,0x80,min);
            drv_tuner_write_rt810(RT810RegsMap);
            fw_timer_event_DelayXms(5);

            drv_i2c_ContRead(I2C_MASTER_TUNER,3,rt810_read);
            for (j=0; j<3; j++)
            {
                ms=rt810_read[j];
                rt810_read[j]=(UINT8)(((ms & 0x80) >> 7 ) + ((ms & 0x40) >> 5) + ((ms & 0x20) >> 3 ) + ((ms & 0x10) >> 1 ) +
                                      ((ms & 0x08) << 1 ) + ((ms & 0x04) << 3 ) + ((ms & 0x02) << 5 ) + ((ms & 0x01) << 7 ));
            }

            if (rt810_read[1]>40)
                break;

        }

// Set to 6MHz channel filter
// Set VGA code mode and code = 60
        IMRAutoCal(0);
        /*		do {
        			x1=x;
        			y1=y;
        			IMRAutoCal();
        		} while ( (x!=x1) || (y!=y1));

        */
        ADCRead(gans,gans4,phs,phs4);
        RTD_Log(0, "K, Gans:%d\nGans4:%d\nPhs:%d\nPhs4:%d\n",(UINT32)gans,(UINT32)gans4,(UINT32)phs,(UINT32)phs4);
        if ((gans4 > 4) || (phs4 > 4))
        {
            gans4=0;
            phs4=0;
            RTD_Log(0, "Gans:%d\nGans4:%d\nPhs:%d\nPhs4:%d\n",(UINT32)gans,(UINT32)gans4,(UINT32)phs,(UINT32)phs4);
            ADCRead(gans,gans4,phs,phs4);
            x=0;
            y=0;
        }

        pk[4][0]=x;
        pk[4][1]=y;
    }

    if (pk[k][0]==0x99)
    {
        PLLLOFrequency = cali_initialreg(LOFrequency);
        VCOpll(PLLLOFrequency*10,0);
        fw_timer_event_DelayXms(10);

// 		Turn on Ring
        updatereg(15,0xf7,0);
        updatereg(5,0x7f,0x0);
        drv_tuner_write_rt810(RT810RegsMap);
        fw_timer_event_DelayXms(1);
        updatereg(4,0x7f,0x0);
        drv_tuner_write_rt810(RT810RegsMap);
        fw_timer_event_DelayXms(1);

        for (i=0; i<3; i++)
        {
            switch (i)
            {
            case 0:
                min=52;
                break;
            case 1:
                min=56;
                break;
            case 2:
                min=60;
                break;

            }
            updatereg(12,0x80,min);
            drv_tuner_write_rt810(RT810RegsMap);
            fw_timer_event_DelayXms(5);

            drv_i2c_ContRead(I2C_MASTER_TUNER,3,rt810_read);
            for (j=0; j<3; j++)
            {
                ms=rt810_read[j];
                rt810_read[j]=(UINT8)(((ms & 0x80) >> 7 ) + ((ms & 0x40) >> 5) + ((ms & 0x20) >> 3 ) + ((ms & 0x10) >> 1 ) +
                                      ((ms & 0x08) << 1 ) + ((ms & 0x04) << 3 ) + ((ms & 0x02) << 5 ) + ((ms & 0x01) << 7 ));
            }

            if (rt810_read[1]>40)
                break;

        }
        x1=x=pk[4][0];
        y1=y=pk[4][1];

        IMRAutoCal(1);
        if ( (x!=x1) || (y!=y1))
        {
            IMRAutoCal(1);
        }
        pk[k][0]=x;
        pk[k][1]=y;

    }

    else
    {
        x=pk[k][0];
        y=pk[k][1];

//     	       IMRAutoCal(1);
        writeIQ(x,y);
        RTD_Log(0, "No K, Gans:%d\nGans4:%d\nPhs:%d\nPhs4:%d\n",(UINT32)gans,(UINT32)gans4,(UINT32)phs,(UINT32)phs4);
    }
//	CTunerSetFreqSearch(freq);
    LOFrequency=(freq + ChannelIF)/10;
    initialreg(LOFrequency);
// 	updatereg(12,0x7f,0x00);				// VGA code mode
// 	updatereg(12,0x80,4);
    updatereg(11,0xe7,ChannelBW << 3);
    VCOpll(freq+ChannelIF,0);

    RTD_Log(LOGGER_INFO, "Jump channel freq:%d, ChannelIF : %d, LO:%d\n",freq,ChannelIF,LOFrequency);
    RTD_Log(LOGGER_INFO, "R11 %x,R15 %x\n",(UINT32)RT810RegsMap[11-3],(UINT32)RT810RegsMap[15-3]);
//	drv_tuner_write_rt810(RT810RegsMap);

    return 0;
}
/*
UINT8  ADCRead(UINT8 a, UINT8 b, UINT8 c, UINT8 d)
{
		UINT8 RT810RegAccessBuf[4];
		UINT8 rt810_read[3],ms;

              R1Dat = 0 ;
              RT810RegAccessBuf[0]=8;
	       RT810RegAccessBuf[1]=(b & 0x1F)|((a & 0x01) << 5)|((1 & 0x01 ) << 6)| ((0 & 0x01) << 7) ;
              RT810RegAccessBuf[2]=9;
		RT810RegAccessBuf[3]=(d & 0x1F)|((c & 0x01) << 5)|((1 & 0x01 ) << 6)| ((0 & 0x01) << 7) ;
		updatereg(8,0xdf,a<<5);
		updatereg(8,0xe0,b);
		updatereg(9,0xdf,c<<5);
		updatereg(9,0xe0,d);

		drv_i2c_Set_TargetAddr(I2C_MASTER_TUNER, I2C_MASTER_TUNER_ADDR);
		if(!drv_i2c_Write(I2C_MASTER_TUNER, 4, RT810RegAccessBuf))
			RTD_Log(LOGGER_INFO, "######Silicon Access Error!!!!!!\n");
		fw_timer_event_DelayXms(6);
		drv_i2c_ContRead(I2C_MASTER_TUNER,3,rt810_read);

		do {
			R1Dat=rt810_read[1];
		   	fw_timer_event_DelayXms(1);
			drv_i2c_ContRead(I2C_MASTER_TUNER,3,rt810_read);

			while  (rt810_read[1]==0) {
				RTD_Log(LOGGER_INFO, "ADC = 0, again...\n");
				if (!drv_i2c_ContRead(I2C_MASTER_TUNER,3,rt810_read))
					RTD_Log(LOGGER_INFO, "######Silicon RAEDinf ADC  Error!!!!!!\n");;
				}
		} while (R1Dat!=rt810_read[1]);

		ms=rt810_read[1];
		rt810_read[1]=(UINT8)(((ms & 0x80) >> 7 ) + ((ms & 0x40) >> 5) + ((ms & 0x20) >> 3 ) + ((ms & 0x10) >> 1 ) +
									 ((ms & 0x08) << 1 ) + ((ms & 0x04) << 3 ) + ((ms & 0x02) << 5 ) + ((ms & 0x01) << 7 ));

		R1Dat = rt810_read[1];
		RTD_Log(LOGGER_INFO, "R0: %x, R1Dat: %d, R2: %d\n",(UINT32) rt810_read[0],(UINT32)R1Dat,(UINT32) rt810_read[2]);
		return  R1Dat;
}
*/
UINT8  ADCRead(UINT8 a, UINT8 b, UINT8 c, UINT8 d)
{
    UINT8 RT810RegAccessBuf[4];
		UINT8 rt810_read[3],ms,c1,c2,i=30,j=5;

    R1Dat = 0 ;
    RT810RegAccessBuf[0]=8;
    RT810RegAccessBuf[1]=(b & 0x1F)|((a & 0x01) << 5)|((1 & 0x01 ) << 6)| ((0 & 0x01) << 7) ;
    RT810RegAccessBuf[2]=9;
    RT810RegAccessBuf[3]=(d & 0x1F)|((c & 0x01) << 5)|((1 & 0x01 ) << 6)| ((0 & 0x01) << 7) ;
    updatereg(8,0xdf,a<<5);
    updatereg(8,0xe0,b);
    updatereg(9,0xdf,c<<5);
    updatereg(9,0xe0,d);

    drv_i2c_Set_TargetAddr(I2C_MASTER_TUNER, _TUNER_WRITE_ADDRESS);
    if (!drv_i2c_Write(I2C_MASTER_TUNER, 4, RT810RegAccessBuf))
        RTD_Log(LOGGER_INFO, "######Silicon Access Error!!!!!!\n");
    fw_timer_event_DelayXms(5);
    drv_i2c_ContRead(I2C_MASTER_TUNER,3,rt810_read);
    c1=rt810_read[1];
    fw_timer_event_DelayXms(1);
    drv_i2c_ContRead(I2C_MASTER_TUNER,3,rt810_read);
    c2=rt810_read[1];
    fw_timer_event_DelayXms(1);
    drv_i2c_ContRead(I2C_MASTER_TUNER,3,rt810_read);


    do
    {
        R1Dat=c1;
        c1=c2;
        c2=rt810_read[1];
        fw_timer_event_DelayXms(1);
        drv_i2c_ContRead(I2C_MASTER_TUNER,3,rt810_read);

			while  (rt810_read[1]==0) {
    		drv_i2c_ContRead(I2C_MASTER_TUNER,3,rt810_read);
				if (--j==0)
					break;
			}		
			j=5;
			if ( R1Dat==rt810_read[1] && R1Dat==c1 && R1Dat==c2)  
					break;
			if (--i==0)
				break;
		 } while (1) ;

    ms=rt810_read[1];
    rt810_read[1]=(UINT8)(((ms & 0x80) >> 7 ) + ((ms & 0x40) >> 5) + ((ms & 0x20) >> 3 ) + ((ms & 0x10) >> 1 ) +
                          ((ms & 0x08) << 1 ) + ((ms & 0x04) << 3 ) + ((ms & 0x02) << 5 ) + ((ms & 0x01) << 7 ));

    R1Dat = rt810_read[1];
    RTD_Log(LOGGER_INFO, "R0: %x, R1Dat: %d, R2: %d\n",(UINT32) rt810_read[0],(UINT32)R1Dat,(UINT32) rt810_read[2]);
    return  R1Dat;
}
UINT8  NADCRead(UINT8 a, UINT8 b)
{
    UINT8 g,g4,p,p4;
    g=a>>5;
    g4=(g) ? (a & 0x1f) : (31 - (a & 0x1f)) ;
    p=(b >> 5) & 0x01;
    p4=(p) ? (b & 0x1f) : (31 - (b & 0x1f)) ;

    return ADCRead(g,g4,p,p4);

}

void  writeIQ(UINT8 a, UINT8 b)
{

    gans=a>>5;
    gans4=(gans) ? (a & 0x1f) : (31 - (a & 0x1f)) ;
    phs=(b >> 5) & 0x01;
    phs4=(phs) ? (b & 0x1f) : (31 - (b & 0x1f)) ;

    updatereg(8,0xdf,gans<<5);
    updatereg(8,0xe0,gans4);
    updatereg(9,0xdf,phs<<5);
    updatereg(9,0xe0,phs4);

}


void course(UINT8 g, UINT8 p)
{
    UINT8 x1,y1,d,mem1,mem2;
    x=g;
    y=p;
    d=0;
    do
    {
        d++;
        mem1=NADCRead(x+d,y);
        mem2=NADCRead(x,y);
    }
    while ( (mem1==mem2) && d<=5);

    d=1;
    if (mem1<mem2)
    {
        do
        {
            d++;
            mem2=NADCRead(x+d,y);
            if (mem2<=mem1)
            {
                mem1=mem2;
                continue;
            }
            else
                break;
        }
        while (1);
        x1=x+d-1;
    }
    else
    {
        mem1=mem2;
        do
        {
            d++;
            mem2=NADCRead(x-d,y);
            if (mem2<=mem1)
            {
                mem1=mem2;
                continue;
            }
            else
                break;
        }
        while (1);
        x1=(x-d+1);
    }

    x=x1;
    d=0;
    do
    {
        d++;
        mem1=NADCRead(x,y+d);
        mem2=NADCRead(x,y);
    }
    while ( (mem1==mem2) && d<=5);
    d=1;
    if (mem1<mem2)
    {
        do
        {
            d++;
            mem2=NADCRead(x,y+d);
            if (mem2<=mem1)
            {
                mem1=mem2;
                continue;
            }
            else
                break;
        }
        while (1);
        y1=y+d-1;
    }
    else
    {
        mem1=mem2;
        do
        {
            d++;
            mem2=NADCRead(x,y-d);
            if (mem2<=mem1)
            {
                mem1=mem2;
                continue;
            }
            else
                break;
        }
        while (1);
        y1=y-d+1;
    }
    y=y1;

}

void IMRAutoCal(UINT8 skip)
{
    UINT8 BB,l1,x1,y1;
    if (skip==0)
    {
        gans=1;
        phs=1;
        gans4 = 0;
        phs4=0;

        x= ((gans & 0x01)<<5) | (gans & 0x01) ? ( (gans4 & 0x1f)+32) : (31-gans4);
        y= ((phs & 0x01) <<5) | (phs & 0x01) ? ( (phs4 & 0x1f)+32) : (31-phs4);

        course(x,y);
    }
//	course(x,y);
    BB=NADCRead(x,y);
    x1=x;
    y1=y;


    if ((l1=NADCRead( ((x-1)==31) ? 30 : (x-1),  ((y-1)==31) ? 30 : (y-1)  )) <BB)
    {
        x1=((x-1)==31) ? 30 : (x-1);
        y1=((y-1)==31) ? 30 : (y-1);
        BB = l1;
    }
    if ( (l1=NADCRead(((x-1)==31) ? 30 : (x-1),y)) <BB)
    {
        x1=((x-1)==31) ? 30 : (x-1);
        y1=y;
        BB = l1;
    }
    if ((l1=NADCRead(((x-1)==31) ? 30 : (x-1),((y+1)==32) ? 33 : (y+1) ))<BB)
    {
        x1=((x-1)==31) ? 30 : (x-1);
        y1=((y+1)==32) ? 33 : (y+1);
        BB=l1;
    }
    if ((l1=NADCRead(((x+1)==32) ? 33 : (x+1),((y-1)==31) ? 30 : (y-1) ))<BB)
    {
        x1=((x+1)==32) ? 33 : (x+1);
        y1=((y-1)==31) ? 30 : (y-1);
        BB=l1;
    }
    if ((l1=NADCRead(((x+1)==32) ? 33 : (x+1),y))<BB)
    {
        x1=((x+1)==32) ? 33 : (x+1);
        y1=y;
        BB=l1;
    }
    if ((l1=NADCRead(((x+1)==32) ? 33 : (x+1),((y+1)==32) ? 33 : (y+1)))<BB)
    {
        x1=((x+1)==32) ? 33 : (x+1);
        y1=((y+1)==32) ? 33 : (y+1);
        BB=l1;
    }
    if ((l1=NADCRead(x,((y+1)==32) ? 33 : (y+1)))<BB)
    {
        x1=x;
        y1=((y+1)==32) ? 33 : (y+1);
        BB=l1;
    }
    if ((l1=NADCRead(x,((y-1)==31) ? 30 : (y-1) ))<BB)
    {
        x1=x;
        y1=((y-1)==31) ? 30 : (y-1);
        BB=l1;
    }


    if (x1==31)
        x1=32;
    if (y1==31)
        y1=32;

    gans=x1>>5;
    gans4=(gans) ? (x1 & 0x1f) : (31 - (x1 & 0x1f)) ;
    phs=y1 >> 5;
    phs4=(phs) ? (y1 & 0x1f) : (31 - (y1 & 0x1f)) ;

    x=x1;
    y=y1;
}
/*=============================================================*/
/**
	This routine will get TV to current frequency.
	@param			void
	@return 			Return the freq	frequency
*/

UINT32 CTunerGetFreq(void)
{
    return g_curr_freq;
}

/**
	This routine will get divider ratio for specific frequency.
	@param freq	frequency
	@return 			Return the status code  _TURE,_FALSE
*/
UINT16 drv_tuner_GeDiverRatio(UINT32 freq)
{
    return (freq + _TUNER_PICTURE_INTERMEDIATE_FREQ) / _TUNER_RATIO_IN_STEP_SIZE;
}

UINT32 drv_tuner_GetRatioInStepSize(void)
{
    return _TUNER_RATIO_IN_STEP_SIZE;
}



void drv_tuner_TunerAudioStdInit(UINT8 mode)
{
    if (mode==_SIG_MODE_AUDIO_MN)
        drvif_ifd_set_audio_bp(_SIG_MODE_NTSC);
    else
        drvif_ifd_set_audio_bp(_SIG_MODE_PAL);
}
#if 0
UINT8  drv_tuner_SetFreqafc (UINT32 freq)
{
    UINT32 LOFrequency;
    LOFrequency=(freq + ChannelIF)/10;
    initialreg(LOFrequency);
    updatereg(12,0x7f,0x80);				// VGA code mode
    updatereg(12,0x80,40);
    /* 	updatereg(3,0xef,0x00);
     	updatereg(3,0xf0,0x08);
     	updatereg(6,0xef,0x1<<4);
     	updatereg(6,0xf0,0x01);

     	updatereg(4,0xf8,0x00);
     	updatereg(7,0xf0,0x08);
     	updatereg(26,0x3f,0x0);			// // +3dB
     	updatereg(23,0xf0,0);
    */
    VCOpll(freq+ChannelIF,1);


    //RTD_Log(LOGGER_INFO, "Searching AFC channel freq:%d, ChannelIF : %d, LO:%d\n",freq,ChannelIF,LOFrequency);
    return _TRUE;
}

#endif

UINT8  CTunerSetFreqSearch (UINT32 freq)
{
    UINT32 LOFrequency;

    LOFrequency=(freq + ChannelIF)/10;
//	LOFrequency=(freq + ChannelIF+1000000)/10;
//	LOFrequency=(freq )/10;
    initialreg(LOFrequency);
    updatereg(8,0xc0,0x0);
    updatereg(9,0xc0,0x0);
    updatereg(12,0x7f,0x80);				// VGA code mode
    updatereg(12,0x80,60);
    VCOpll(freq+ChannelIF,0);

    //RTD_Log(LOGGER_INFO, "Searching channel freq:%d, ChannelIF : %d, LO:%d\n",freq,ChannelIF,LOFrequency);
    return _TRUE;
}


UINT8 drv_tuner_CheckBand(UINT32 freq)
{
    UINT8 TunerBand;
    TunerBand=(_TUNER_VHF_LOW_FREQ > freq) ?0x01 :(_TUNER_VHF_HIGH_FREQ > freq) ?0x02 :	0x08;

    return TunerBand;
}

UINT32 drv_tuner_GetChanelMaxFreq(void)
{
    return _TUNER_MAX_FREQ;
}

UINT32 drv_tuner_GetChanelMinFreq(void)
{
    return _TUNER_MIN_FREQ;
}

#ifndef CONFIG_TUNER_SILICON

UINT32 drv_tuner_GetPictureInterFreq(void)
{
    //return ChannelIF;//_TUNER_PICTURE_INTERMEDIATE_FREQ;
    return _TUNER_PICTURE_INTERMEDIATE_FREQ;
}
#endif
#if 0
void drv_tuner_isr_TunerAudioStdInit(UINT8 mode)
{
    drv_ifd_isr_Set_Audio_BP(mode);
}
#endif
#if 0
void drv_tuner_RT810_poweroff(void)
{
    updatereg(4,0xff,0xff);
    updatereg(7,0x7f,0x80);
    updatereg(10,0x7f,0x80);
    updatereg(11,0xfd,0x02);
    updatereg(17,0xfb,0x04);
    updatereg(18,0x1d,0xe2);
    updatereg(26,0xff,0x04);
    drv_tuner_write_rt810(RT810RegsMap);

}
#endif
/*
	rf = 0 ,  cable in
	rf = 1, Air in
*/
/*void drv_tuner_RT810_input_selection(UINT8 rf)
{

    rf_in = rf;
}*/

void drv_tuner_initRT810(UINT8 mode)
{
    UINT8 PalNtsc=(mode == _SIG_MODE_NTSC || mode == _SIG_MODE_NTSC_443) ? NTSC : PAL;
    if (pkmark==0)
    {
        ChannelIF=58200;
        drv_tuner_SetFreq(3910000);
        pkmark=1;
    }
    if (Standard!=PalNtsc)
    {
        Standard=PalNtsc;

        if ( mode == _SIG_MODE_NTSC)
        {
            ChannelIF=58200;
            ChannelBW=CHANNELBW_6M;
            drv_tuner_write_rt810(RT810BW6MCalib);
            fw_timer_event_DelayXms(10);
            drv_tuner_write_rt810_cali(RT810BW8MCalib0);
            fw_timer_event_DelayXms(10);

            drv_tuner_write_rt810_cali(RT810BW8MCalib1);
            fw_timer_event_DelayXms(10);

            drv_tuner_write_rt810_cali(RT810BW6MCalib2);
            RTD_Log(LOGGER_INFO, "drv_tuner_initRT810(),ChannelBW=%x\n",(UINT32)ChannelBW);
        }
        else
        {
            ChannelIF=_TUNER_PICTURE_INTERMEDIATE_FREQ;//77500;
            ChannelBW=CHANNELBW_8M;
            drv_tuner_write_rt810(RT810BW8MCalib);
            fw_timer_event_DelayXms(10);
            drv_tuner_write_rt810_cali(RT810BW8MCalib0);
            fw_timer_event_DelayXms(10);
            drv_tuner_write_rt810_cali(RT810BW8MCalib1);
            fw_timer_event_DelayXms(10);

            drv_tuner_write_rt810_cali(RT810BW8MCalib2);
            RTD_Log(LOGGER_INFO, "drv_tuner_initRT810(),ChannelBW=%x\n",(UINT32)ChannelBW);
        }
        updatereg(11,0xe7,ChannelBW << 3);
        RTD_Log(LOGGER_INFO, "R11 %x,R15 %x\n",(UINT32)RT810RegsMap[11-3],(UINT32)RT810RegsMap[15-3]);

    }
}
#if 0
void drv_tuner_initRT810_FM(void)
{
    ChannelIF=58200;
    ChannelBW=CHANNELBW_6M;
    drv_tuner_write_rt810(RT810BW6MCalib);
    fw_timer_event_DelayXms(10);
    drv_tuner_write_rt810_cali(RT810BW8MCalib0);
    fw_timer_event_DelayXms(10);

    drv_tuner_write_rt810_cali(RT810BW8MCalib1);
    fw_timer_event_DelayXms(10);

    drv_tuner_write_rt810_cali(RT810BW6MCalib2);
    RTD_Log(LOGGER_INFO, "drv_tuner_initRT810(),ChannelBW=%x\n",(UINT32)ChannelBW);

}
#endif
void drv_tuner_write_rt810(UINT8* pRegsMap)
{
    UINT8 i;
    UINT8 RT810RegAccessBuf[2];
    drv_i2c_Set_TargetAddr(I2C_MASTER_TUNER, _TUNER_WRITE_ADDRESS);
#ifdef RT810_P2VERSION
    for (i=0; i<25; i++)
#endif

#ifdef RT810_T4VERSION
        for (i=0; i<24; i++)
#endif
        {
            RT810RegAccessBuf[0]=i+3;
            RT810RegAccessBuf[1]=pRegsMap[i];

            if (!drv_i2c_Write(I2C_MASTER_TUNER, 2, RT810RegAccessBuf))
                RTD_Log(LOGGER_INFO, "######Silicon Access Error!!!!!!\n");
        }
//	fw_timer_event_DelayXms(30);

}



void drv_tuner_write_rt810_cali(UINT8* pRegsMap)
{

    UINT8 RT810RegAccessBuf[2];
    drv_i2c_Set_TargetAddr(I2C_MASTER_TUNER, _TUNER_WRITE_ADDRESS);


    RT810RegAccessBuf[0]=11;
    RT810RegAccessBuf[1]=pRegsMap[0];
    if (!drv_i2c_Write(I2C_MASTER_TUNER, 2, RT810RegAccessBuf))
        RTD_Log(LOGGER_INFO, "######Silicon Access Error!!!!!!\n");

    RT810RegAccessBuf[0]=15;
    RT810RegAccessBuf[1]=pRegsMap[1];
    if (!drv_i2c_Write(I2C_MASTER_TUNER, 2, RT810RegAccessBuf))
        RTD_Log(LOGGER_INFO, "######Silicon Access Error!!!!!!\n");

//	fw_timer_event_DelayXms(5);

}

void drv_tuner_write_rt810_vcoonly(UINT8* pRegsMap)
{

    UINT8 RT810RegAccessBuf[2];
    drv_i2c_Set_TargetAddr(I2C_MASTER_TUNER, _TUNER_WRITE_ADDRESS);


    RT810RegAccessBuf[0]=22;
    RT810RegAccessBuf[1]=pRegsMap[22-3];
    if (!drv_i2c_Write(I2C_MASTER_TUNER, 2, RT810RegAccessBuf))
        RTD_Log(LOGGER_INFO, "######Silicon Access Error!!!!!!\n");

    RT810RegAccessBuf[0]=21;
    RT810RegAccessBuf[1]=pRegsMap[21-3];
    if (!drv_i2c_Write(I2C_MASTER_TUNER, 2, RT810RegAccessBuf))
        RTD_Log(LOGGER_INFO, "######Silicon Access Error!!!!!!\n");

    RT810RegAccessBuf[0]=20;
    RT810RegAccessBuf[1]=pRegsMap[20-3];
    if (!drv_i2c_Write(I2C_MASTER_TUNER, 2, RT810RegAccessBuf))
        RTD_Log(LOGGER_INFO, "######Silicon Access Error!!!!!!\n");

    RT810RegAccessBuf[0]=18;
    RT810RegAccessBuf[1]=pRegsMap[18-3];
    if (!drv_i2c_Write(I2C_MASTER_TUNER, 2, RT810RegAccessBuf))
        RTD_Log(LOGGER_INFO, "######Silicon Access Error!!!!!!\n");

}
#endif
#endif
#endif
