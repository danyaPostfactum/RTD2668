/*=============================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2004
  *
  * All rights reserved.
  *
  *============================================================*/

/*======================= Description ============================
  *
  * file: 		hdmiRxDrv.c
  *
  * author: 	Justin Chung
  * date:
  * version: 	0.0
  *
  *============================================================*/

/*========================Header Files===========================*/
#include "autoconf.h"
#ifdef CONFIG_ENABLE_HDMI
#define HDMI_COMPILE
#include "pcb_customer.h"
#include <hdmi/hdmiFun.h>
#define DEBUG_PRINTK	RTD_Log//
#ifndef IRQ_HANDLED
#define IRQ_HANDLED  1
#endif
#define Enable_BBM	0

/*======================== Definition and Marco =============================*/
typedef struct
{

    UINT16 b_upper;
    UINT16 b_lower;
    UINT8 reg_CDR_01;
    UINT8 reg_CDR_02;
    UINT8 reg_CDR_03;
    UINT8 reg_FLD_09;
    UINT8 reg_RXMISC_01;
    UINT8 reg_RXMISC_02;
    UINT8 reg_RESERVED_00;
//	UINT8 reg_RESERVED_02;
    UINT8 reg_RESERVED_05;
    UINT8 reg_ADAPTIVE_EQUALIZER;
    UINT8 reg_RESERVED_FA;
    char HDMI_CONST* band_name;

} HDMI_PHY_PARAM_T;

volatile UINT8  b_debounce = 0;
volatile UINT8 hdcp_flag = 0;
volatile UINT8 clr_err_flag = 0;
volatile UINT8 toggle_hpg = 0;

/*======================== Global Variable ================================*/
#if 0 //not used
// Keil C default non-reentrance code such that we need to redefine hdmiport_xxx for ISR
/**
 * hdmi_inl
 * Columbus hdmi register read
 *
 * @param <addr>	{ addr of register }
 * @return 		{ read value }
 * @ingroup drv_hdmi
 */
static UINT8 hdmiisrport_in(UINT8 addr)
{
    UINT8 result;
    rtdf_isr_pageSelect(0x02);
    hdmi_isr_out(HDMI_HDMI_AP_VADDR, addr);
    result = hdmi_isr_in(HDMI_HDMI_DP_VADDR);
    return result;
}
#endif

static UINT8 hdmiisrport_isr_in(UINT8 addr)
{
    UINT8 result;
    rtdf_isr_pageSelect(0x02);
    hdmi_isr_out(HDMI_HDMI_AP_VADDR, addr);
    result = hdmi_isr_in(HDMI_HDMI_DP_VADDR);
    return result;
}

#if 0 // not used
/**
 * hdmi_maskl
 * Columbus register read and mask function
 *
 * @param <addr>		{ register addr }
 * @param <andmask> { andmask AND with register value }
 * @param <ormask>	 { ormask OR with register value }
 * @return
 * @ingroup drv_hdmi
 */
static void hdmiisrport_mask(UINT8 addr, UINT8 andmask, UINT8 ormask)
{
    UINT8	result;
    rtdf_isr_pageSelect(0x02);
    hdmi_isr_out(HDMI_HDMI_AP_VADDR, addr);
    result = hdmi_isr_in(HDMI_HDMI_DP_VADDR);
    result &= andmask;
    result |= ormask;
    hdmi_isr_out(HDMI_HDMI_AP_VADDR, addr);
    hdmi_isr_out(HDMI_HDMI_DP_VADDR, result);
}
#endif

static void hdmiisrport_isr_mask(UINT8 addr, UINT8 andmask, UINT8 ormask)
{
    UINT8	result;
    rtdf_isr_pageSelect(0x02);
    hdmi_isr_out(HDMI_HDMI_AP_VADDR, addr);
    result = hdmi_isr_in(HDMI_HDMI_DP_VADDR);
    result &= andmask;
    result |= ormask;
    hdmi_isr_out(HDMI_HDMI_AP_VADDR, addr);
    hdmi_isr_out(HDMI_HDMI_DP_VADDR, result);
}

UINT8 inline Hdmi_HdcpisrPortRead(UINT8 addr)
{
    rtdf_isr_pageSelect(0x02);
    hdmi_isr_out(HDMI_HDCP_AP_VADDR, addr);
    return hdmi_isr_in(HDMI_HDCP_DP_VADDR);
}

HDMIRX_IOCTL_STRUCT_T hdmi_ioctl_struct;
#if 0
HDMI_CONST HDMI_PHY_PARAM_T hdmi_phy_param[] =
{
    { 1106, 2100,  0xC2 , 0xAD , 0xD5 , 0x0F, 0x09, 0xC7,  0x82, 0xE4, 0x56, 0xC0, "<250M"},  // 250 ~ 500 M
    { 553, 1106,  0x82 , 0xAD , 0xD5 , 0x0F, 0x09, 0xC7,  0x82, 0xE4, 0x56, 0xC0,"250 ~ 500 M"},  // 250 ~ 500 M
    { 442,  553,  0x42 , 0xAD , 0xD5 , 0x0F, 0x09, 0xC7,  0x82, 0xE4, 0x56,  0xC0,"500 ~ 625 M"},   // 500 ~ 625 M
    { 276,   442, 0xb2 , 0x11 , 0xD4 , 0x0F, 0x09, 0xC7,  0x82, 0xE4, 0x56,  0x00, "625 ~ 1G"   },   // 625 ~ 1G
//		{ 276,   442, 0x42 ,// 0xBD , 0xD5 , 0x0F, 0x09, 0xC7,  0x82, 0xE4, 0x56,  0xC0, "625 ~ 1G"   },   // 625 ~ 1G
    { 184,   276, 0x72 , 0x11 , 0xD4 , 0x0F, 0x09, 0xC7,  0x82, 0xE4, 0x56,  0x00,"1G ~ 1.5G"    },   // 1G ~ 1.5G
    { 138,   184, 0x72 , 0x11 , 0xD4 , 0x0F, 0x09, 0xC7,  0x82, 0xE4, 0x56, 0x00,"1.5G ~ 2G"    },   // 1.5G ~ 2G
    {  81,   138, 0x32 , 0x11 , 0xD4 , 0x0F, 0x09, 0xC7,  0x82, 0xE4, 0x56, 0x00,"2G ~ 3.4G"    }   // 2G ~ 3.4G

};
#else
HDMI_CONST HDMI_PHY_PARAM_T hdmi_phy_param[] =
{
    { 1106, 2100,  0xC2 , 0xAD , 0xCC , 0x0F, 0x09, 0xC7,  0x88, 0xE4, 0x76, 0xC0, "<250M"},  // 250 ~ 500 M
    { 553, 1106,  0x82 , 0xAD , 0xCC , 0x0F, 0x09, 0xC7,  0x88, 0xD4, 0x76, 0xC0,"250 ~ 500 M"},  // 250 ~ 500 M
    { 442,  553,  0x42 , 0xAD , 0xCC , 0x0F, 0x09, 0xC7,  0x88, 0xD4, 0x76,  0xC0,"500 ~ 625 M"},   // 500 ~ 625 M
    { 276,   442, 0xb2 , 0x11 , 0xCC , 0x0F, 0x09, 0xC7,  0x88, 0xD4, 0x76,  0x00, "625 ~ 1G"   },   // 625 ~ 1G
//		{ 276,   442, 0x42 ,// 0xBD , 0xD5 , 0x0F, 0x09, 0xC7,  0x82, 0xE4, 0x56,  0xC0, "625 ~ 1G"   },   // 625 ~ 1G
    { 184,   276, 0x72 , 0x11 , 0xCC , 0x0F, 0x09, 0xC7,  0x88, 0xD4, 0x76,  0x00,"1G ~ 1.5G"    },   // 1G ~ 1.5G
    { 138,   184, 0x72 , 0x11 , 0xCC , 0x0F, 0x09, 0xC7,  0x88, 0xD4, 0x76, 0x00,"1.5G ~ 2G"    },   // 1.5G ~ 2G
    {  81,   138, 0x32 , 0x11 , 0xCC , 0x0F, 0x09, 0xC7,  0x88, 0xD4, 0x76, 0x00,"2G ~ 3.4G"    }   // 2G ~ 3.4G

};
#endif
#if HDMI_DEBUG_AUDIO_PLL_RING

#define AUDIO_PLL_RING_SIZE		300

INT16 audio_pll_ring_buffer[AUDIO_PLL_RING_SIZE];

typedef struct
{
    INT16 *buffer;
    int 	 ring_start;
    int	 ring_end;
    UINT8 enable;
} HDMI_AUDIO_PLL_BUFFER_T;

HDMI_AUDIO_PLL_BUFFER_T audio_pll_buffer;

#endif

HDMI_BOOL rtd_hdmiRx_cmd(UINT8 cmd,  void * arg)
{
    // Switch to Page 2 referenced from spec
    rtdf_pageSelect(2);

    switch ((HDMIRX_IOCTL_T) cmd)
    {

    case IOCTRL_HDMI_PHY_START:
    {
//->w				DEBUG_PRINTK(LOGGER_INFO, "hdmiRxDrv.o: 	IOCTRL_HDMI_PHY_START\n");
        hdmi_ioctl_struct.b = 0;
        hdmi_ioctl_struct.b_change = 0;
        hdmi_ioctl_struct.b_pre = 0;
        hdmi_ioctl_struct.measure_source = MEASURE_TMDS;
        hdmi_ioctl_struct.measure_count = 0;
        rtdf_pageSelect(2);
        hdmi_mask(HDMI_TMDS_CPS_VADDR, ~0x60, 0x70);  // select TMDS clock as measure source
        hdmiport_mask(HDMI_HDMI_NTX1024TR0_VADDR,0xff,_BIT3);			//Restart measure b
        HDMI_ENABLE_TIMER_ISR();

    }
    break;
    case IOCTRL_HDMI_PHY_STOP:
//->w				DEBUG_PRINTK(LOGGER_INFO, "hdmiRxDrv.o: 	IOCTRL_HDMI_PHY_STOP\n");
        hdmi_ioctl_struct.b = 0;
        hdmi_ioctl_struct.b_change = 0;
        hdmi_ioctl_struct.b_pre = 0;
        HDMI_DISABLE_TIMER_ISR();
        break;
    case IOCTRL_HDMI_GET_STRUCT:
    {
//->w				DEBUG_PRINTK(LOGGER_INFO, "hdmiRxDrv.o: 	IOCTRL_GET_HDMI_STRUCT %d\n", (UINT32)hdmi_ioctl_struct.b);
        HDMI_DISABLE_TIMER_ISR();
        HDMI_COPY(arg, (UINT8 xdata*)&hdmi_ioctl_struct, sizeof(hdmi_ioctl_struct));
        hdmi_ioctl_struct.b_change = 0;
        HDMI_ENABLE_TIMER_ISR();
        return 0;
    }
    break;
#if HDMI_DEBUG_AUDIO_PLL_RING
    case IOCTRL_HDMI_AUDIO_PLL_SAMPLE_START:
    {
//->w					DEBUG_PRINTK(LOGGER_INFO, "hdmiRxDrv.o: 	IOCTRL_HDMI_AUDIO_PLL_SAMPLE_START\n");
        HDMI_DISABLE_TIMER_ISR();
        audio_pll_buffer.buffer = audio_pll_ring_buffer;
        audio_pll_buffer.ring_end = 0;
        audio_pll_buffer.ring_start = 0;
        audio_pll_buffer.enable = 1;
        HDMI_ENABLE_TIMER_ISR();
        return 0;
    }
    break;

    case IOCTRL_HDMI_AUDIO_PLL_SAMPLE_STOP:
    {
//->w					DEBUG_PRINTK(LOGGER_INFO, "hdmiRxDrv.o: 	IOCTRL_HDMI_AUDIO_PLL_SAMPLE_STOP\n");
        HDMI_DISABLE_TIMER_ISR();
        audio_pll_buffer.ring_end = 0;
        audio_pll_buffer.ring_start = 0;
        audio_pll_buffer.enable = 0;
        HDMI_ENABLE_TIMER_ISR();
        return 0;
    }
    break;

    case IOCTRL_HDMI_AUDIO_PLL_SAMPLE_DUMP:
    {
//->w					DEBUG_PRINTK(LOGGER_INFO, "hdmiRxDrv.o: 	IOCTRL_HDMI_AUDIO_PLL_SAMPLE_DUMP\n");
        HDMI_DISABLE_TIMER_ISR();
        if (audio_pll_buffer.enable)
        {
            while (audio_pll_buffer.ring_start != audio_pll_buffer.ring_end)
            {
                RTD_Log(LOGGER_INFO, "%d\n", audio_pll_buffer.buffer[audio_pll_buffer.ring_start]);
                audio_pll_buffer.ring_start ++;
                if (audio_pll_buffer.ring_start == AUDIO_PLL_RING_SIZE)
                    audio_pll_buffer.ring_start = 0;
            }
        }

        HDMI_ENABLE_TIMER_ISR();
        return 0;
    }
    break;
#endif
    default:
        break;
    }

    return 0;
}


void SetupTMDSPhy(UINT16 b, HDMI_BOOL force)
{

    int i;

    if ((HDMI_ABS(hdmi_ioctl_struct.b, b) < 4) && (force == 0)) return;
    if (force)
    {
        b = hdmi_ioctl_struct.b;
    }
//->w	HDMI_PRINTF(LOGGER_INFO, "****************** setup phy %d************************\n", (UINT32)b);
    hdmi_ioctl_struct.b = b;
//	hdmi_ioctl_struct.b_change = 1;
    for (i=0; i<7; i++)
    {
        if (hdmi_phy_param[i].b_upper < b && hdmi_phy_param[i].b_lower >= b)
        {
            hdmi_ioctl_struct.b_change = 1;
            //HDMI_PRINTF(LOGGER_INFO, "Band : %s\n", hdmi_phy_param[i].band_name);
//->w						HDMI_PRINTF(LOGGER_INFO, "hdmi_phy_param[%d]\n", (UINT32)i);
            rtdf_isr_pageSelect(0x0b);
            hdmi_isr_out(HDMI_RXMISC_01_VADDR, hdmi_phy_param[i].reg_RXMISC_01);
            hdmi_isr_out(HDMI_ADAPTIVE_EQUALIZER_VADDR, hdmi_phy_param[i].reg_ADAPTIVE_EQUALIZER);
            hdmi_isr_out(HDMI_RESERVED_05_VADDR, hdmi_phy_param[i].reg_RESERVED_05);
            hdmi_isr_mask(HDMI_ADAPTIVE_EQUALIZER_VADDR,~(_BIT6|_BIT5), (_BIT6|_BIT5));
            hdmi_isr_mask(HDMI_ADAPTIVE_EQUALIZER_VADDR,~(_BIT6), 0);
            hdmi_isr_mask(HDMI_ADAPTIVE_EQUALIZER_VADDR,~(_BIT6), _BIT6);
//						hdmi_isr_mask(HDMI_POWER_CONTROL_VADDR, ~0xFF, 0);
            hdmi_isr_mask(HDMI_SWITCH_2D_VADDR,~_BIT0, 0);						//enable bit 7 before setting VCO parameters
            hdmi_isr_out(HDMI_CDR_01_VADDR, hdmi_phy_param[i].reg_CDR_01);
            hdmi_isr_out(HDMI_CDR_02_VADDR, hdmi_phy_param[i].reg_CDR_02);
            hdmi_isr_out(HDMI_CDR_03_VADDR, hdmi_phy_param[i].reg_CDR_03);
            hdmi_isr_out(HDMI_FLD_09_VADDR, hdmi_phy_param[i].reg_FLD_09);
//						hdmi_isr_out(HDMI_RXMISC_01_VADDR, hdmi_phy_param[i].reg_RXMISC_01);
            hdmi_isr_out(HDMI_RXMISC_02_VADDR, hdmi_phy_param[i].reg_RXMISC_02);
            hdmi_isr_out(HDMI_RESERVED_00_VADDR, hdmi_phy_param[i].reg_RESERVED_00);
//						hdmi_isr_out(HDMI_RESERVED_05_VADDR, hdmi_phy_param[i].reg_RESERVED_05);
            hdmi_isr_out(HDMI_RESERVED_FA_VADDR, hdmi_phy_param[i].reg_RESERVED_FA);
            if (i < 2)
            {
                hdmi_isr_mask(HDMI_FLD_00_VADDR, ~_BIT0, _BIT0);
#if Enable_BBM
                rtdf_isr_pageSelect(0x02);
                hdmi_isr_mask(HDMI_PHY_CONTROL2_VADDR,~_BIT0,_BIT0);
                hdmi_isr_mask(HDMI_DIG_BBANG00_VADDR,~_BIT7,_BIT7);
                rtdf_isr_pageSelect(0x0b);
#endif
            }
            else
            {
                hdmi_isr_mask(HDMI_FLD_00_VADDR, ~_BIT0, 0);
#if Enable_BBM
                rtdf_isr_pageSelect(0x02);
                hdmi_isr_mask(HDMI_PHY_CONTROL2_VADDR,~_BIT0,0);
                hdmi_isr_mask(HDMI_DIG_BBANG00_VADDR,~_BIT7,0);
                rtdf_isr_pageSelect(0x0b);
#endif
            }
//						hdmi_isr_mask(HDMI_POWER_CONTROL_VADDR, ~0xFF, 0xFF);
#if 1//HDMI_SPARROW_STTING
#if 0
            hdmi_out(HDMI_POWERCONTROL_VADDR, 0x00);
            hdmi_out(HDMI_POWERCONTROL_VADDR, 0xFF);
            hdmi_mask(HDMI_ADAPTIVE_EQUALIZER_VADDR, ~_BIT6, 0);
            hdmi_out(HDMI_ADAPTIVE_EQUALIZER_VADDR, 0x56);
            hdmi_out(HDMI_Z0_CALIBRATION_VADDR, 0x08);
            hdmi_out(HDMI_Z0_CALIBRATION_VADDR, 0xF8);
            hdmi_mask(HDMI_SWITCH_2D_VADDR,~_BIT1, 0);						//enable bit 7 before setting VCO parameters

            dmi_mask(HDMI_SWITCH_2D_VADDR,~_BIT1, _BIT1);						//enable bit 7 before setting VCO parameters
#endif
            hdmi_isr_mask(HDMI_FLD_05_VADDR, ~_BIT5, _BIT5);
            hdmi_isr_mask(HDMI_FLD_05_VADDR, ~_BIT6, _BIT6);
            hdmi_isr_mask(HDMI_FLD_05_VADDR, ~_BIT6, 0);
            hdmi_isr_mask(HDMI_SWITCH_2D_VADDR,~_BIT0, 0);						//enable bit 7 before setting VCO parameters
//						HDMI_DELAYMS(1);
            hdmi_isr_mask(HDMI_SWITCH_2D_VADDR,~_BIT0, _BIT0);						//clean bit 7 to reset PLL after setting VCO parameters

#else
            hdmi_out(HDMI_RESERVED_00_VADDR, hdmi_phy_param[i].reg_RESERVED_00);
            hdmi_out(HDMI_RESERVED_05_VADDR, hdmi_phy_param[i].reg_RESERVED_05);
            hdmi_out(HDMI_ADAPTIVE_EQUALIZER_VADDR, hdmi_phy_param[i].reg_ADAPTIVE_EQUALIZER);
            hdmi_mask(HDMI_FLD_08_VADDR,~_BIT3, _BIT3);
            hdmi_mask(HDMI_FLD_08_VADDR,~_BIT3, 0);
            hdmi_mask(HDMI_OOBS_RXAMP_VADDR,~_BIT7, 0);						//clean bit 7 to reset PLL after setting VCO parameters
            hdmi_mask(HDMI_OOBS_RXAMP_VADDR,~_BIT7, _BIT7);						//enable bit 7 before setting VCO parameters
            hdmi_mask(HDMI_OOBS_RXAMP_VADDR,~_BIT7, 0);						//clean bit 7 to reset PLL after setting VCO parameters
#endif
            return;
        }

    }
//->w	HDMI_PRINTF(LOGGER_INFO, "Band : NONE\n");

}

int HdmiMeasureVedioClock(void)
{

    UINT16 b;
#if HDMI_DEBUG_AUDIO_PLL_RING
    UINT16 value;
#endif
//->w	DEBUG_PRINTK(LOGGER_INFO, "HDMI_HDMI_NTX1024TR0_VADDR = %x\n", (UINT32)hdmiport_in(HDMI_HDMI_NTX1024TR0_VADDR));

#if HDMI_DEBUG_AUDIO_PLL_RING
    if (audio_pll_buffer.enable)
    {
        value = hdmiisrport_isr_in(HDMI_HDMI_DPCR4_VADDR);
        value <<= 8;
        value |= hdmiisrport_isr_in(HDMI_HDMI_DPCR5_VADDR);
        if (value & 0x8000)
        {
            value = ~value;
            audio_pll_buffer.buffer[audio_pll_buffer.ring_end] = value + 1;
            audio_pll_buffer.buffer[audio_pll_buffer.ring_end] = -audio_pll_buffer.buffer[audio_pll_buffer.ring_end];
        }
        else
        {
            audio_pll_buffer.buffer[audio_pll_buffer.ring_end] = value;
        }
        audio_pll_buffer.ring_end ++;
        if (audio_pll_buffer.ring_end == AUDIO_PLL_RING_SIZE)
            audio_pll_buffer.ring_end = 0;
        if (audio_pll_buffer.ring_start == audio_pll_buffer.ring_end)
        {
            audio_pll_buffer.ring_start ++;
            if (audio_pll_buffer.ring_start == AUDIO_PLL_RING_SIZE)
                audio_pll_buffer.ring_start = 0;
        }
    }
#endif
    if ((hdmiisrport_isr_in(HDMI_HDMI_NTX1024TR0_VADDR) & _BIT3) == 0)
    {
        b =  (((UINT16)hdmiisrport_isr_in(HDMI_HDMI_NTX1024TR0_VADDR) & 0x7) << 8) | (UINT16)hdmiisrport_isr_in(HDMI_HDMI_NTX1024TR1_VADDR) ;

        if ((hdmi_isr_in(HDMI_TMDS_ACC2_VADDR)&(0x80)) && b<50)
            b = b + 2048;
//->W		DEBUG_PRINTK(LOGGER_INFO, "\nb = %d\n", (UINT32)b);
        b_debounce = 0;
        if (HDMI_ABS(b, hdmi_ioctl_struct.b_pre) < 5)
        {
            hdmi_isr_mask(HDMI_TMDS_CPS_VADDR, ~0x60, 0x60);  // select TMDS clock as measure source
            if (Force_HDMIPHY_RST)
            {
                SetupTMDSPhy(b, 1);
                Force_HDMIPHY_RST = 0;
            }
            else
            {
                SetupTMDSPhy(b, 0);
            }
        }
        hdmi_ioctl_struct.b_pre = b;
        hdmiisrport_isr_mask(HDMI_HDMI_NTX1024TR0_VADDR,0xff,_BIT3);			//Restart measure b
    }
    else
    {
//->w		DEBUG_PRINTK(LOGGER_INFO, "measure vedio time out\n");
        //SetupTMDSPhy(hdmi_ioctl_struct.b, 1);
        hdmiisrport_isr_mask(HDMI_HDMI_NTX1024TR0_VADDR,0xff,_BIT3);			//Restart measure b
        //hdmiisrport_isr_mask(HDMI_HDMI_NTX1024TR0_VADDR,0xff,0);
        if (GET_INPUTSOURCE_INDEX() == _SOURCE_INDEX_HDMI0)
        {
            //hdmi_isr_mask(HDMI_TMDS_CPS_VADDR, ~0x60, 0x60);
            hdmi_isr_mask(HDMI_TMDS_CPS_VADDR, ~0x60, 0x40);  // select green clock  as measure source
            hdmi_ioctl_struct.b  = 0;
            hdmi_ioctl_struct.b_pre = 0;
        }
        else
        {
            hdmi_isr_mask(HDMI_TMDS_CPS_VADDR, ~0x60, 0x60);  // select TMDS clock as measure source

        }
#if 0
        if ((hdmi_isr_in(HDMI_TMDS_CPS_VADDR)&(0x60)) == 0x60)
            hdmi_isr_mask(HDMI_TMDS_CPS_VADDR, ~0x60, 0x40);  // select green clock  as measure source
        else if ((hdmi_isr_in(HDMI_TMDS_CPS_VADDR)&(0x60)) == 0x40)
            hdmi_isr_mask(HDMI_TMDS_CPS_VADDR, ~0x60, 0x00);  // select red clock  as measure source
        else
            hdmi_isr_mask(HDMI_TMDS_CPS_VADDR, ~0x60, 0x60);  // select TMDS clock as measure source
#endif
        b_debounce++;
        if (b_debounce < 5)
        {
            return 0;
        }
        else
        {
//			if ((hdmi_isr_in(HDMI_TMDS_CPS_VADDR)&(0x60)) == 0x60)
//				hdmi_isr_mask(HDMI_TMDS_CPS_VADDR, ~0x60, 0x40);  // select green clock  as measure source
//			else if ((hdmi_isr_in(HDMI_TMDS_CPS_VADDR)&(0x60)) == 0x40)
//				hdmi_isr_mask(HDMI_TMDS_CPS_VADDR, ~0x60, 0x00);  // select red clock  as measure source
//			else
            hdmi_isr_mask(HDMI_TMDS_CPS_VADDR, ~0x60, 0x60);  // select TMDS clock as measure source

            b_debounce = 0;
            hdmi_ioctl_struct.b  = 0;
            hdmi_ioctl_struct.b_pre = 0;
            return 0;
        }
    }

    return -1;
}
void HdmiMeasureDataClock(void)
{

    if ((hdmiisrport_isr_in(HDMI_HDMI_NTX1024TR0_VADDR) & _BIT3) == 0)
    {
        hdmi_ioctl_struct.data_b =  (((UINT16)hdmiisrport_isr_in(HDMI_HDMI_NTX1024TR0_VADDR) & 0x7) << 8) | (UINT16)hdmiisrport_isr_in(HDMI_HDMI_NTX1024TR1_VADDR) ;
        if ((hdmi_isr_in(HDMI_TMDS_ACC2_VADDR)&(0x80)) && hdmi_ioctl_struct.data_b<50)
            hdmi_ioctl_struct.data_b = hdmi_ioctl_struct.data_b + 2048;
    }
    else
    {
        hdmi_ioctl_struct.data_b = 0;
    }
}

void ResetDataCDR(UINT8 channel)
{
    UINT8 j=0;
    rtdf_isr_pageSelect(0xb);
    if (channel == MEASURE_RED)
    {
        hdmi_isr_mask(HDMI_POWER_CONTROL_VADDR, ~(_BIT0) , 0x0);
        for (j= 0; j<100; j++) j=j; // delay
        //RTD_Log(LOGGER_INFO,"Reset R channel CDR\n");
        hdmi_isr_mask(HDMI_POWER_CONTROL_VADDR, ~(_BIT0) , _BIT0);
    }
    else if (channel == MEASURE_GREEN)
    {
        hdmi_isr_mask(HDMI_POWER_CONTROL_VADDR, ~(_BIT1) , 0x0);
        for (j= 0; j<100; j++) j=j; // delay
        //RTD_Log(LOGGER_INFO,"Reset G channel CDR\n");
        hdmi_isr_mask(HDMI_POWER_CONTROL_VADDR, ~(_BIT1) , _BIT1);
    }
    else if (channel == MEASURE_BLUE)
    {
        hdmi_isr_mask(HDMI_POWER_CONTROL_VADDR, ~(_BIT2) , 0x0);
        for (j= 0; j<100; j++) j=j; // delay
        //RTD_Log(LOGGER_INFO,"Reset B channel CDR\n");
        hdmi_isr_mask(HDMI_POWER_CONTROL_VADDR, ~(_BIT2) , _BIT2);
    }
    rtdf_isr_pageSelect(2);

}

void hdmi_isr_set_gpio_pin(UINT8 which_bit, bit val)
{
    if (which_bit < GPIO32)
    {
        if (val)
            rtd_isr_maskl(GPDATO_0x2584, ~(1L << which_bit),(1L << which_bit));
        else
            rtd_isr_maskl(GPDATO_0x2584, ~(1L << which_bit),0);
    }
}

void HdmiLGPatchFSM(void)
{

//	static UINT8 hdcp_flag = 0;
//	static UINT8 clr_err_flag = 0;
//	static UINT8 toggle_hpg = 0;
    static UINT32 i=0;
    UINT8 result = Hdmi_HdcpisrPortRead(0xC1) & ( _BIT5|_BIT6) ? 1:0;

    if (toggle_hpg)
    {
        hdmi_isr_set_gpio_pin(hdmi.channel[0]->HotPlugPin, hdmi.channel[0]->HotPlugOn == 0);	//hpg low
        hdcp_flag = 0;
        rtdf_isr_pageSelect(0xb);
        hdmi_isr_out(HDMI_POWER_CONTROL_VADDR,0x0);
        if (hdmi.timer == 0)
        {
            rtdf_isr_pageSelect(0xb);
            hdmi_isr_out(HDMI_POWER_CONTROL_VADDR,0xFF);
            hdmi_isr_set_gpio_pin(hdmi.channel[0]->HotPlugPin, hdmi.channel[0]->HotPlugOn);	//hpg high
            AVMute_Timer =0;
            toggle_hpg = 0;
            if (clr_err_flag)
            {
                rtdf_isr_pageSelect(0x2);
                hdmiisrport_isr_mask(HDMI_HDMI_BCSR_VADDR,~_BIT0,_BIT0);
            }
            //RTD_Log(LOGGER_INFO,"\nhpg high\n");
        }
//				RTD_Log(LOGGER_INFO,"\n/hdcpflg:%x => chge\n",(UINT32)Hdmi_HdcpisrPortRead(0xC1));
        return;
    }

    if ((result ^ hdcp_flag)&&(result == 1))
    {
        i=0;
        rtdf_isr_pageSelect(2);
//		RTD_Log(LOGGER_INFO,"\nhdcpflg:%d => chge\n",(UINT32)hdmi_ioctl_struct.b_pre);
//		RTD_Log(LOGGER_INFO,"\n/hdcpflg:%d  %d=> chge\n",(UINT32)Hdmi_HdcpisrPortRead(0xC0),(UINT32)hdcp_flag);
        hdmi.timer = 450;
        /*if ((hdmi_ioctl_struct.b_pre >350) && (hdmi_ioctl_struct.b_pre <400))//for 720p,1080i
        	Hdmi_SetTimer10ms(150);
        else if ((hdmi_ioctl_struct.b_pre >1000) && (hdmi_ioctl_struct.b_pre <1100))//for 480i,480p
        	Hdmi_SetTimer10ms(300);*/
        AVMute_Timer=1;
    }
    else if ((result ^ hdcp_flag)&&(result == 0))
    {
        AVMute_Timer = 0;
    }
    if ((Hdmi_HdcpisrPortRead(0xC0) == 0) && (result==1))
    {
    }
    else
    {
        hdcp_flag= result;
    }

    switch (AVMute_Timer)
    {
    case 0:
        rtdf_isr_pageSelect(2);
        hdmi_isr_mask(HDMI_TMDS_RPS_VADDR, ~_BIT7, 0x0);
        break;
    case 1:
        i++;
        //if (hdmi_in(HDMI_HDMI_SR_VADDR) & _BIT6)
        //RTD_Log(LOGGER_INFO,"\n i= %d ,AVmute =  %x\n",i,(hdmi_in(HDMI_HDMI_SR_VADDR) & _BIT6));

        if (hdmi.timer == 0)
        {
//				RTD_Log(LOGGER_INFO,"\n i= %d ,AVMute Clear Start .......\n",i);
            rtdf_isr_pageSelect(2);
            hdmi_isr_mask(HDMI_TMDS_RPS_VADDR, ~_BIT7, _BIT7);
            AVMute_Timer = 2;
            hdmi.timer = 200;
        }
        break;
    case 2:
        //RTD_Log(LOGGER_INFO,"\nAVMute Clear Middle = %x \n",Hdmi_HdcpPortRead(0xC1));
        if (hdmi.timer == 0)
        {
            rtdf_isr_pageSelect(2);
            hdmi_isr_mask(HDMI_TMDS_RPS_VADDR, ~_BIT7, 0x0);
            if (clr_err_flag)
                hdmiisrport_isr_mask(HDMI_HDMI_BCSR_VADDR,~_BIT0,_BIT0);
            AVMute_Timer = 0;
//				RTD_Log(LOGGER_INFO,"\nAVMute Clear End !!!!!\n");
        }
        break;
    default:
//			RTD_Log(LOGGER_INFO,"\n i= %d ,AVMute Clear ERROR.......\n",i);
        rtdf_isr_pageSelect(2);
        hdmi_isr_mask(HDMI_TMDS_RPS_VADDR, ~_BIT7, 0x0);
        hdmi_isr_mask(HDMI_HDMI_AVMCR_VADDR, ~_BIT7, 0x0);
        break;
    }
}

HDMI_BOOL rtd_hdmiPhy_ISR(void)
{


    UINT8 hdmi_ap_addr;
    // Switch to Page 2 referenced from spec
//->w		rtdf_pageSelect(2);
    if (hdmi.timer != 0) hdmi.timer--;
    rtdf_isr_pageSelect(2);

//->w		hdmi_ap_addr = hdmi_in(HDMI_HDMI_AP_VADDR);
    hdmi_ap_addr = hdmi_isr_in(HDMI_HDMI_AP_VADDR);
//->w		DEBUG_PRINTK(LOGGER_INFO, "rtd_hdmiPhy_ISR\n");
    switch (hdmi_ioctl_struct.measure_source)
    {
    case MEASURE_TMDS:
        HdmiMeasureVedioClock();
#if 1
        rtdf_isr_pageSelect(2);//rtdf_pageSelect(2);
        if (hdmi_ioctl_struct.measure_count % 50 == 1)  		//adams modify to 50 for flicker red screen
        {
            hdmi_ioctl_struct.measure_source = MEASURE_GREEN;
            // switch to green
            rtdf_isr_pageSelect(2);//rtdf_pageSelect(2);
            hdmi_isr_mask(HDMI_TMDS_CPS_VADDR, ~0x60, 0x50);  // select GREEN clock as measure source
            hdmiisrport_isr_mask(HDMI_HDMI_NTX1024TR0_VADDR,0xff,_BIT3);//Restart measure b
        }

        if (hdmi_ioctl_struct.measure_count)
        {
            hdmi_ioctl_struct.measure_count--;
        }
#endif
        break;
    case MEASURE_GREEN:
        HdmiMeasureDataClock();
        //RTD_Log(LOGGER_INFO, "check data clock data_g = %d b =%d\n",(UINT32) hdmi_ioctl_struct.data_b,(UINT32) hdmi_ioctl_struct.b );
        if ((ABS(hdmi_ioctl_struct.data_b, hdmi_ioctl_struct.b) > 5)&& (hdmi_ioctl_struct.b >80)&&(hdmi_ioctl_struct.b<2100))
        {
            //RTD_Log(LOGGER_INFO, "data_g = %d b =%d\n",(UINT32) hdmi_ioctl_struct.data_b,(UINT32) hdmi_ioctl_struct.b );
            //SetupTMDSPhy(0, 1);
            ResetDataCDR(MEASURE_GREEN);
            hdmi_ioctl_struct.measure_source = MEASURE_TMDS;
            rtdf_isr_pageSelect(2);//rtdf_pageSelect(2);
            hdmi_isr_mask(HDMI_TMDS_CPS_VADDR, ~0x60, 0x70);  // select TMDS clock as measure source
            hdmiisrport_isr_mask(HDMI_HDMI_NTX1024TR0_VADDR,0xff,_BIT3);//Restart measure b
        }
        else
        {
            hdmi_ioctl_struct.measure_source = MEASURE_RED;
            rtdf_isr_pageSelect(2);//rtdf_pageSelect(2);
            hdmi_isr_mask(HDMI_TMDS_CPS_VADDR, ~0x60, 0x10);  // select Red channel as measure source
            hdmiisrport_isr_mask(HDMI_HDMI_NTX1024TR0_VADDR,0xff,_BIT3);//Restart measure b
        }
        break;
    case MEASURE_RED:
        HdmiMeasureDataClock();
        //RTD_Log(LOGGER_INFO, "red check data clock data_r = %d b =%d\n",(UINT32) hdmi_ioctl_struct.data_b,(UINT32) hdmi_ioctl_struct.b );
        if ((ABS(hdmi_ioctl_struct.data_b, hdmi_ioctl_struct.b) > 5)&& (hdmi_ioctl_struct.b >80)&&(hdmi_ioctl_struct.b<2100))
        {
            //RTD_Log(LOGGER_INFO, "data_r = %d b =%d\n",(UINT32) hdmi_ioctl_struct.data_b,(UINT32) hdmi_ioctl_struct.b );
            //SetupTMDSPhy(0, 1);
            ResetDataCDR(MEASURE_RED);
            hdmi_ioctl_struct.measure_source = MEASURE_TMDS;
            rtdf_isr_pageSelect(2);//rtdf_pageSelect(2);
            hdmi_isr_mask(HDMI_TMDS_CPS_VADDR, ~0x60, 0x70);  // select TMDS clock as measure source
            hdmiisrport_isr_mask(HDMI_HDMI_NTX1024TR0_VADDR,0xff,_BIT3);//Restart measure b
        }
        else
        {
            hdmi_ioctl_struct.measure_source = MEASURE_BLUE;
            rtdf_isr_pageSelect(2);//rtdf_pageSelect(2);
            hdmi_isr_mask(HDMI_TMDS_CPS_VADDR, ~0x60, 0x30);  // select Blue channel as measure source
            hdmiisrport_isr_mask(HDMI_HDMI_NTX1024TR0_VADDR,0xff,_BIT3);//Restart measure b
        }
        break;
    case MEASURE_BLUE:
        HdmiMeasureDataClock();
        //RTD_Log(LOGGER_INFO, "check data clock data_b = %d b =%d\n",(UINT32) hdmi_ioctl_struct.data_b,(UINT32) hdmi_ioctl_struct.b );
        if ((ABS(hdmi_ioctl_struct.data_b, hdmi_ioctl_struct.b) > 5)&& (hdmi_ioctl_struct.b >80)&&(hdmi_ioctl_struct.b<2100))
        {
            //RTD_Log(LOGGER_INFO, "data_b = %d b =%d\n",(UINT32) hdmi_ioctl_struct.data_b,(UINT32) hdmi_ioctl_struct.b );
            //SetupTMDSPhy(0, 1);
            ResetDataCDR(MEASURE_BLUE);
        }
        hdmi_ioctl_struct.measure_source = MEASURE_TMDS;
        rtdf_isr_pageSelect(2);//rtdf_pageSelect(2);
        hdmi_isr_mask(HDMI_TMDS_CPS_VADDR, ~0x60, 0x70);  // select TMDS clock as measure source
        hdmiisrport_isr_mask(HDMI_HDMI_NTX1024TR0_VADDR,0xff,_BIT3);//Restart measure b
        break;
    default:
        hdmi_ioctl_struct.measure_source = MEASURE_TMDS;
        rtdf_isr_pageSelect(2);//rtdf_pageSelect(2);
        hdmi_isr_mask(HDMI_TMDS_CPS_VADDR, ~0x60, 0x70);  // select TMDS clock as measure source
        hdmiisrport_isr_mask(HDMI_HDMI_NTX1024TR0_VADDR,0xff,_BIT3);//Restart measure b
        break;
    }
#if LG_DF9921N
    rtdf_isr_pageSelect(2);
    HdmiLGPatchFSM();
#endif
//->w		rtdf_pageSelect(2);
    rtdf_isr_pageSelect(2);
//->w		hdmi_out(HDMI_HDMI_AP_VADDR, hdmi_ap_addr);
    hdmi_isr_out(HDMI_HDMI_AP_VADDR, hdmi_ap_addr);

    return IRQ_HANDLED;
}
#if 0
//debug function to check phy CRC
void rtd_hdmiPhy_Check_CRC(void)
{
    UINT32 CRC1,CRC2,CRC3;
    UINT8 temp=20;

    temp=20;
    HDMI_LOG(LOGGER_INFO,"Signal check:%x\n",(UINT32)hdmi_in(HDMI_TMDS_CTRL_VADDR));
    hdmi_mask(HDMI_TMDS_CTRL_VADDR, 0xff, 0xF9);
    while (temp)
    {
        HDMI_DELAYMS(100);
        if ((hdmi_in(HDMI_TMDS_CTRL_VADDR) & _BIT0) == 0)
            break;
        temp--;
    }

    if (temp != 0)
    {
        CRC1 = (UINT32)hdmi_in(HDMI_TMDS_CRCOB2_VADDR);
        CRC2 = (UINT32)hdmi_in(HDMI_TMDS_CRCOB2_VADDR);
        CRC3 = (UINT32)hdmi_in(HDMI_TMDS_CRCOB2_VADDR);
        HDMI_LOG(LOGGER_INFO,"CRC:%x %x %x\n",CRC1,CRC2,CRC3);
        CRC1 = (UINT32)hdmi_in(HDMI_TMDS_CRCOB2_VADDR);
        CRC2 = (UINT32)hdmi_in(HDMI_TMDS_CRCOB2_VADDR);
        CRC3 = (UINT32)hdmi_in(HDMI_TMDS_CRCOB2_VADDR);
        HDMI_LOG(LOGGER_INFO,"CRC:%x %x %x\n",CRC1,CRC2,CRC3);
        CRC1 = (UINT32)hdmi_in(HDMI_TMDS_CRCOB2_VADDR);
        CRC2 = (UINT32)hdmi_in(HDMI_TMDS_CRCOB2_VADDR);
        CRC3 = (UINT32)hdmi_in(HDMI_TMDS_CRCOB2_VADDR);
        HDMI_LOG(LOGGER_INFO,"CRC:%x %x %x\n",CRC1,CRC2,CRC3);
        CRC1 = (UINT32)hdmi_in(HDMI_TMDS_CRCOB2_VADDR);
        CRC2 = (UINT32)hdmi_in(HDMI_TMDS_CRCOB2_VADDR);
        CRC3 = (UINT32)hdmi_in(HDMI_TMDS_CRCOB2_VADDR);
        HDMI_LOG(LOGGER_INFO,"CRC:%x %x %x\n",CRC1,CRC2,CRC3);
    }
    else
    {
        hdmiisrport_mask(HDMI_HDMI_NTX1024TR0_VADDR,0xff,_BIT3);			//Restart measure b
        HDMI_DELAYMS(20);
        if ((hdmiisrport_in(HDMI_HDMI_NTX1024TR0_VADDR) & _BIT3) == 0)
        {
            UINT16 b =  (((UINT16)hdmiisrport_in(HDMI_HDMI_NTX1024TR0_VADDR) & 0x7) << 8) | (UINT16)hdmiisrport_in(HDMI_HDMI_NTX1024TR1_VADDR) ;
            HDMI_LOG(LOGGER_INFO,"[b]:%x\n",(UINT32)b);
        }
        else
        {
            HDMI_LOG(LOGGER_INFO,"no b\n");
        }
        HDMI_LOG(LOGGER_INFO,"CRC timeout\n");
    }
}

#endif

#endif
