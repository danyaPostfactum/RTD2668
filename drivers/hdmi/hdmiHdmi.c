/*=============================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2005
  * All rights reserved.
  * ============================================ */

/*================= File Description ================= */
/**
 * @file hdmi_hdmi.c
 * To supply Columbus HDMI RX driver APIs.
 * @author justin
 * @date 2008/08/01
 * @version 0.1
 * @ingroup drv_hdmi
 */

/**
* @addtogroup
* @{
*/
#include "autoconf.h"
#ifdef CONFIG_ENABLE_HDMI

#define HDMI_COMPILE
#define HDMI_PRINT_CRC 0
#define HDMI_Patch_For_Splitter 0
#define N_CTS_FIFO_TRACKING 0
#include <hdmi\hdmiFun.h>
#include "hdmi\hdmisparrow.h"
#include "sparrow_utility.h"
#include "flash\spi_flash.h"
#include "flow_conf_struct.h"
/*===================== Global Variable =======================*/

UINT8 AVMute_Timer = 0;
UINT8 HDMI_MES_FAIL_COUNT=0;
UINT8 Force_HDMIPHY_RST=0;
HDMI_INFO_T hdmi;


HDMI_CONST HDMI_AUDIO_PLL_COEFF_T audio_pll_coeff[] =
{
    { 32000, _AUDIO_256_TIMES, 0},
    { 44100, _AUDIO_256_TIMES, 0},
    { 48000, _AUDIO_256_TIMES, 0},
    { 88200, _AUDIO_256_TIMES, 1},
    { 96000, _AUDIO_256_TIMES, 1},
    { 176400, _AUDIO_128_TIMES, 2},
    { 192000, _AUDIO_128_TIMES, 3}
};



HDMI_CONST HDMI_AUDIO_PLL_PARAM_T hdmi_audiopll_param[] =
{

    { 0,   20,  2,  8,  1,  0x1d, 0xdc,  "32K"     },
//		{ 0,   20,  2,  8,  1,  0x1E, 0xB0,  "32K"     },
    { 0,   20,	2,   6,  1,  0xFe, 0x6b,  "44.1K"  },
//		{ 0,   20,  2,  6,  1,  0xFC, 0x68,  "44.1K"  },	//adams modify for TSB project,20110615
    { 0,   22,  2,  6,  1,  0x07, 0x40,  "48K"     },
//		{ 0,   20,  1,  6,  1,  0xFC, 0x68,  "88.2K"  },
    { 0,   20,  2,  6,  0,  0xFC, 0x68,  "88.2K"  },
//		{ 0,   22,  1,  6,  1,  0x07, 0x40,  "96K"     },
    { 0,   22,  2,  6,  0,  0x07, 0x40,  "96K"     },
//		{ 0,   20,  1,  6,  0,  0xFC, 0x68,  "176.4K" },
    { 0,   28,  2,  4,  0,  0x27, 0xA0,  "176.4K" },
//		{ 0,   22,  1,  6,  0,  0x07, 0x40,  "192K"    }
    { 0,   28,  2,  4,  0,  0xD8, 0x5F,  "192K"    }
};

HDMI_CONST HDMI_VEDIO_PLL_PARAM_T hdmi_videopll_param[] =
{

//                b       b      O  S
    { 1106, 2100,  2 , 2,  "<25M"},  // < 250 M
    { 553, 1106,   0 , 4, "25 ~ 50 M"},  // 250 ~ 500 M
    { 442,  553,    0 , 2, "50 ~ 62.5 M"},   // 500 ~ 625 M
    { 276,   442,   0 , 2,  "62.5 ~ 100M"   },   // 625 ~ 1G
    { 184,   276,   0 , 1, "100M ~ 150M"    }   // 1G ~ 1.5G
//		{ 138,   184, 0x72 , 0x11 , 0xD4 , 0x0F, "150M ~ 200M"    },   // 1.5G ~ 2G
//		{  81,   138, 0x32 , 0x11 , 0xD4 , 0x0F, "200M ~ 340M"    }   // 2G ~ 3.4G
};

HDMI_CONST HDMI_COLOR_SPACE_T ColorMap[] =
{
    COLOR_RGB, 		// 0
    COLOR_YUV422,   // 01
    COLOR_YUV444,   // 10
    COLOR_YUV444,   // 11
};

HDMI_CONST UINT16 AUDIO_INFO_FREQ_TABLE[] =
{
    0,  	// refer to stream headers
    320, 	// 44.1k
    441, 	 // Unkwon
    480, // 48k
    882,
    960,
    1764,
    1920
};

HDMI_CONST UINT32 AUDIO_CHANNEL_STATUS[] =
{
    44100,
    000,		// 000 indicate standard no support
    48000,
    32000,
    22000,
    24000,
    88200,
    96000,
    17600,
    19200,
    0,
    0,
    0,
    0,
    0,
    0,
};

#if 0
extern UINT32 xdata profile_time;
void fw_osdcore_InitProfile(void)
{
    profile_time = 0;
}

void fw_osdcore_PrintProfile(void)
{
    RTD_Log(LOGGER_INFO, "\nProfile time = %d ms \n", profile_time*10);
}
#endif


/*===================== Local Routines ========================*/

#if 0 // move to hdmiphy.c
/**
 * hdmi_inl
 * Columbus hdmi register read
 *
 * @param <addr>	{ addr of register }
 * @return 		{ read value }
 * @ingroup drv_hdmi
 */

UINT32 hdmi_inl(UINT32 addr)
{
    UINT32 result;
    rtd_outl(HDMI_HDMI_AP_ADDR, addr);
    result = rtd_inl(HDMI_HDMI_DP_ADDR);
    return result;
}

/**
 * hdmi_outl
 * Columbus hdmi register write
 *
 * @param <addr>	{ addr of register }
 * @param <value>	{ write value }
 * @return
 * @ingroup drv_hdmi
 */

void hdmi_outl(UINT32 addr, UINT32 value)
{

    rtd_outl(HDMI_HDMI_AP_ADDR, addr);
    rtd_outl(HDMI_HDMI_DP_ADDR, value);
}

/**
 * hdmi_maskl
 * Columbus register read and mask function
 *
 * @param <addr>	{ register addr }
 * @param <andmask> { andmask AND with register value }
 * @param <ormask>  { ormask OR with register value }
 * @return
 * @ingroup drv_hdmi
 */
void hdmi_maskl(UINT32 addr, UINT32 andmask, UINT32 ormask)
{
    UINT32 result;
    rtd_outl(HDMI_HDMI_AP_ADDR, addr);
    result = rtd_inl(HDMI_HDMI_DP_ADDR);
    result &= andmask;
    result |= ormask;
    rtd_outl(HDMI_HDMI_AP_ADDR, addr);
    rtd_outl(HDMI_HDMI_DP_ADDR, result);

}
#endif

/**
 * dp_inl
 * Columbus dp register read
 *
 * @param <addr>	{ addr of register }
 * @return 				{ register value }
 * @ingroup drv_hdmi
 */
UINT8 dp_inl(UINT8 addr)
{
    UINT8 result;

    // Switch to Page 2 referenced from spec
    rtdf_pageSelect(0x0c);

    hdmi_out(HDMI_DP_ADR_VADDR, addr);
    result = (UINT8) (hdmi_in(HDMI_DP_DAT_VADDR) & 0xFF);
    return result;
}

/**
 * dp_outl
 * Columbus dp register write
 *
 * @param <addr>		{ addr of register }
 * @param <value>	{ write value }
 * @return
 * @ingroup drv_hdmi
 */
void dp_outl(UINT8 addr, UINT8 value)
{
    rtdf_pageSelect(0x0c);

    hdmi_out(HDMI_DP_ADR_VADDR, addr);
    hdmi_out(HDMI_DP_DAT_VADDR, value);
}

/**
 * Hdmi_HdcpPortCWrite
 * HDCP DDC port setting download function with auto-address increase
 * where HDCP port ddc address for HOST side is 0x74/0x75
 *
 * @param <addr>			{ hdcp register address }
 * @param <data>			{ data pointer }
 * @param <num>			{ length data }
 * @return 				{ void }
 * @ingroup drv_hdmi
 */

void inline Hdmi_HdcpPortCWrite(UINT8 addr ,UINT8 HDMI_CONST* value ,UINT8 num )
{
    hdmi_mask(HDMI_HDCP_PCR_VADDR, ~_BIT0, 0);
    hdmi_out(HDMI_HDCP_AP_VADDR, addr);

    while (num--)
        hdmi_out(HDMI_HDCP_DP_VADDR, *(value++));

    hdmi_mask(HDMI_HDCP_PCR_VADDR, ~_BIT0, _BIT0);
}


/**
 * Hdmi_HdcpPortWrite
 * HDCP DDC port setting download function
 * where HDCP port ddc address for HOST side is 0x74/0x75
 *
 * @param <addr>			{ hdcp register address }
 * @param <data>			{ data }
 * @return 				{ void }
 * @ingroup drv_hdmi
 */


void inline Hdmi_HdcpPortWrite(UINT8 addr ,UINT8 value)
{
    hdmi_out(HDMI_HDCP_AP_VADDR, addr);
    hdmi_out(HDMI_HDCP_DP_VADDR, value);
}
#if 0 // not used
/**
 * Hdmi_HdcpPortRead
 * HDCP DDC port setting read function
 * where HDCP port ddc address for HOST side is 0x74/0x75
 *
 * @param <addr>			{ hdcp register address }
 * @return 				{ read data }
 * @ingroup drv_hdmi
 */
UINT8 inline Hdmi_HdcpPortRead(UINT8 addr)
{
    hdmi_out(HDMI_HDCP_AP_VADDR, addr);
    return hdmi_in(HDMI_HDCP_DP_VADDR);
}
#endif

void drv_hdmi_LoadHDCPKeyCode(void)
{
    UINT8 getVal[6];
    UINT8 nBank;
    UINT16 nFlashAddr;
    UINT8 existed;
    int num;

    // Check
    GET_FLOW_PARAMETER_DATA(KEYCODE_BANK, nBank);
    GET_FLOW_PARAMETER_DATA(KEYCODE_HDCP_ADDR, nFlashAddr);
    // Check "HDCP" whether existed or not
    existed = 0;
    drv_spi_flash_Read((UINT16)&getVal, nBank, nFlashAddr, 4);
    if (getVal[0] == 'H')
        if (getVal[1] == 'D')
            if (getVal[2] == 'C')
                if (getVal[3] == 'P')
                {
                    nFlashAddr += 4;
                    drv_spi_flash_Read((UINT16)&getVal, nBank, nFlashAddr, 3);
                    if ((getVal[0] != 0xff)  || (getVal[1] != 0xff) || (getVal[2] == 0xff))
                    {
                        existed = 1;
                    }
                }

    if (!existed)
    {
        Hdmi_HdcpPortCWrite(0x00 , hdmi.hdcpkey->BKsv, 5 );	//set KSV 40 bits
        nBank = _BANK5;
        nFlashAddr = (UINT16)hdmi.hdcpkey->Key;
    }
    else
    {
        // Write BKsv
        drv_spi_flash_Read((UINT16)&getVal, nBank, nFlashAddr, 5);
        hdmi_mask(HDMI_HDCP_PCR_VADDR, ~_BIT0, 0);
        hdmi_out(HDMI_HDCP_AP_VADDR, 0x00);
        num = 0;
        while (num < 5)
            hdmi_out(HDMI_HDCP_DP_VADDR, getVal[num++]);

        hdmi_mask(HDMI_HDCP_PCR_VADDR, ~_BIT0, _BIT0);

        nFlashAddr += 5;
    }
    Hdmi_HdcpPortWrite(0x40,HDCP_11_HDMI);				//set OESS for DVI

    drv_sparrow_CPU_Switch_Clock(_FALSE);
#if 1
    drv_spi_dma_6093cpy((HDMI_HDCP_DKAP_VADDR & 0xff), nBank , nFlashAddr, 320, 1);
#else
    for (i=0; i<320; i++)
    {
//			if (i% 8 == 0) HDMI_PRINTF(LOGGER_INFO,"\n");
//			HDMI_PRINTF(LOGGER_INFO,"%x ", hdmi.hdcpkey->Key[i]);
        hdmi_out(HDMI_HDCP_DKAP_VADDR,hdmi.hdcpkey->Key[i]);
    }
#endif
    drv_sparrow_CPU_Switch_Clock(_TRUE);
}

void drv_hdmi_LoadEDIDKeyCode(UINT8 channel)
{
    UINT8 getVal[16];
    UINT8 nBank;
    UINT16 nFlashAddr;
    int i, j;
    bit fDefault;

    // Check
    GET_FLOW_PARAMETER_DATA(KEYCODE_BANK, nBank);
    GET_FLOW_PARAMETER_DATA(KEYCODE_EDID0_ADDR, nFlashAddr);
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

    if (fDefault)  	// use default
    {
        HDMI_PRINTF(LOGGER_INFO, "use default hdmi edid\n");
        for (i=0; i<256; i++)
            rtd_outl(GET_DDC_SAP_VADDR(hdmi.channel[channel]->ddc_index), hdmi.channel[channel]->EDID[i]);
    }
    else
    {
        HDMI_PRINTF(LOGGER_INFO, "use hdmi edid : keycode bank = %x, flash_addr = %x\n", (UINT32)nBank, (UINT32)nFlashAddr);
        i = 256;
        while (i)
        {
            if (i > 16)
            {
                drv_spi_flash_Read((UINT16)getVal, nBank, nFlashAddr, 16);
                for (j = 0; j < 16; j++)
                    rtd_outl(GET_DDC_SAP_VADDR(hdmi.channel[channel]->ddc_index), getVal[j]);

                nFlashAddr += 16;
                i -= 16;
            }
            else
            {
                drv_spi_flash_Read((UINT16)getVal, nBank, nFlashAddr, i);
                for (j = 0; j < i; j++)
                    rtd_outl(GET_DDC_SAP_VADDR(hdmi.channel[channel]->ddc_index), getVal[j]);

                nFlashAddr += 16;
                i = 0;
            }
        }
    }
}

/**
 * Hdmi_PacketSRAMRead
 * Read content from HDMI packet SRAM
 *
 * @param <addr>		{ SRAM address }
 * @return 			{ read data }
 * @ingroup drv_hdmi
 */
UINT8 inline Hdmi_PacketSRAMRead(UINT8 addr)
{
    hdmi_out(HDMI_HDMI_PSAP_VADDR, addr);
    return hdmi_in(HDMI_HDMI_PSDP_VADDR) ;
}



/**
 * Hdmi_DumpState
 * Dump all HDMI driver context
 *
 * @param
 * @return
 * @ingroup drv_hdmi
 */

void Hdmi_DumpState(void)
{

    HDMI_PRINTF(LOGGER_INFO, "bHDMIColorSpace = %d\n", (UINT32)GET_HDMI_COLOR_SPACE());
    HDMI_PRINTF(LOGGER_INFO, "IsInterlaced = %d\n", (UINT32)GET_HDMI_ISINTERLACE());
    HDMI_PRINTF(LOGGER_INFO, "bIsHDMIDVI = %d\n", (UINT32)GET_ISHDMI());
    HDMI_PRINTF(LOGGER_INFO, "VedioFSMState = %d\n", (UINT32)GET_HDMI_VIDEO_FSM());
    HDMI_PRINTF(LOGGER_INFO, "AudioFSMState = %d\n", (UINT32)GET_HDMI_AUDIO_FSM());
    HDMI_PRINTF(LOGGER_INFO, "Is422 = %d\n", (UINT32)GET_SCALER_IS422());
}
#if HDMI_PRINT_CRC	// not used
void HDMI_Check_CRC(void)
{
    UINT32 CRC1,CRC2,CRC3;
    UINT8 temp=20;

    temp=20;
    HDMI_LOG(LOGGER_INFO,"Signal check:%x\n",(UINT32)hdmi_in(HDMI_TMDS_CTRL_VADDR));
    hdmi_mask(HDMI_TMDS_CTRL_VADDR, 0xff, 0x01);
    while (temp)
    {
        HDMI_DELAYMS(20);
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
    }
    else
    {
        HDMI_LOG(LOGGER_INFO,"CRC timeout\n");
    }
}
#endif

#if 0
void Hdmi_CheckTransition(void)
{
    // Switch to Page 2 referenced from spec
    rtdf_pageSelect(2);

    hdmiport_out(HDMI_HDMI_BCSR_VADDR, hdmiport_in(HDMI_HDMI_BCSR_VADDR));
    while (1)
    {
        HDMI_PRINTF(LOGGER_INFO,"Hdmi_HdcpPortRead(0xC1) = %x\n", Hdmi_HdcpPortRead(0xC1));
#if 1
        //clear rgb and hv detect flag
        hdmi_out(HDMI_TMDS_CTRL_VADDR, 0xf8);
        // Delay 25ms
        HDMI_DELAYMS(50);

        if ((hdmi_in(HDMI_TMDS_CTRL_VADDR) & 0xf8) != 0xf8)   // TMDS loss sync
        {
            HDMI_PRINTF(LOGGER_INFO,">>>>>>>>>>>>>>>>>>> TMDS Loss Sync\n");
        }
#endif
        if (hdmi_in(HDMI_HDMI_SR_VADDR) & _BIT6)
        {
            HDMI_PRINTF(LOGGER_INFO,"AVMute ON\n");
        }

        if (hdmiport_in(HDMI_HDMI_BCSR_VADDR) & (_BIT1 | _BIT0))
        {
            HDMI_PRINTF(LOGGER_INFO,"Packet Error %x\n", hdmiport_in(HDMI_HDMI_BCSR_VADDR));
            hdmiport_out(HDMI_HDMI_BCSR_VADDR, hdmiport_in(HDMI_HDMI_BCSR_VADDR));
        }

    }
}

/**
 * Hdmi_VideoOutputEnable
 * Enable HDMI Video Output
 *
 * @param
 * @return
 * @ingroup drv_hdmi
 */
void Hdmi_WriteHDCPKey(void)
{
//	UINT32 i;
//	UINT8 getVal, getVal1;
//	UINT16 src_addr = 0xf123;

    rtdf_pageSelect(2);
    hdmi_mask(HDMI_HDCP_PCR_VADDR, ~(_BIT1 | _BIT2),(((HDMI_CHANNEL_T *)hdmi.channel[0])->ddc_index & 0x03)<< 1);
    hdmi_out(HDMI_HDCP_CR_VADDR,0x06);
#if 1
    drv_spi_dma_6093cpy((HDMI_HDCP_DKAP_VADDR & 0xff), _BANK4 , (UINT16)hdmi.hdcpkey->Key, 320, 1);

//		rtdf_setBitsb(0xfff3, _BIT5);
//		i = (UINT16)hdmi.hdcpkey->Key;
//		*((UINT8*)&i + 1) = _BANK4;
//		drv_spi_dma_memcpy((HDMI_HDCP_DKAP_VADDR & 0xff), i, 320, SPI_DMA_TRANSFER_MODE_6093);
#else
    for (i=0; i<320; i++)
    {
//-->w		if (i% 8 == 0) HDMI_PRINTF(LOGGER_INFO,"\n");
//-->w		HDMI_PRINTF(LOGGER_INFO,"%x ", (UINT32)hdmi.hdcpkey->Key[i]);

        hdmi_out(HDMI_HDCP_DKAP_VADDR,hdmi.hdcpkey->Key[i]);
    }
#endif
    hdmi_out(HDMI_HDCP_CR_VADDR,0x00);
    HDMI_PRINTF(LOGGER_INFO,"\n=============================================\n");
    hdmi_out(HDMI_HDCP_CR_VADDR,0x06);
//-->w	for(i=0;i<320;i++) {
//-->w		if (i% 8 == 0) HDMI_PRINTF(LOGGER_INFO,"\n");
//-->w		HDMI_PRINTF(LOGGER_INFO,"%x ", (UINT32)hdmi_in(HDMI_HDCP_DKAP_VADDR));
//-->w	}
    hdmi_out(HDMI_HDCP_CR_VADDR,0x00);
}
#endif
void inline Hdmi_VideoOutputEnable(void)
{
    // Switch to Page 2 referenced from spec
    rtdf_pageSelect(2);

    hdmiport_mask(HDMI_HDMI_AVMCR_VADDR,~_BIT3,_BIT3);
}

/**
 * Hdmi_VideoOutputDisable
 * Disable HDMI Video Output
 *
 * @param
 * @return
 * @ingroup drv_hdmi
 */
void inline Hdmi_VideoOutputDisable(void)
{
    // Switch to Page 2 referenced from spec
    rtdf_pageSelect(2);

    hdmiport_mask(HDMI_HDMI_AVMCR_VADDR,~_BIT3,0x00);
}


/**
 * Hdmi_AudioOutputDisable
 * Disable HDMI Audio Ouput
 *
 * @param
 * @return
 * @ingroup drv_hdmi
 */
void Hdmi_AudioOutputDisable(void)
{
    // Switch to Page 2 referenced from spec
    rtdf_pageSelect(2);
#if 0	//adams mask 20110616, solve audio start mute immediate
//	hdmiport_mask(HDMI_HDMI_AOCR_VADDR,~(0x0ff),0x00);//Disable SPDIF/I2S Output
//	for (cnt = 0; cnt < 5000;cnt++);
    //hdmiport_out(HDMI_HDMI_MAGCR1_VADDR, 0x30);
    //hdmiport_mask(HDMI_HDMI_AVMCR_VADDR,~(_BIT5),0);
    //hdmiport_out(HDMI_HDMI_AFCR_VADDR, 0x06);
    //for (cnt = 0; cnt < 3000;cnt++);
//	hdmiport_out(HDMI_HDMI_AFCR_VADDR, 0x0E);
//	hdmiport_mask(HDMI_HDMI_AOCR_VADDR,(UINT8)(~(0x0ff)),0x00);//Enable SPDIF/I2S Output
    hdmiport_mask(HDMI_HDMI_AVMCR_VADDR,~(_BIT6 | _BIT5), _BIT6);
//	hdmiport_mask(HDMI_HDMI_AVMCR_VADDR,~(_BIT6 | _BIT5), 0);

    //hdmiport_out(HDMI_HDMI_MAGCR1_VADDR, 0x24);
    //HDMI_PRINTF(LOGGER_INFO,"Hdmi_AudioOutputDisable\n");
    hdmiport_out(HDMI_HDMI_AAPNR_VADDR, 0x08);	// Disable SDM
    /*Disable N/CTS tracking*/
    hdmiport_out(HDMI_HDMI_PSCR_VADDR,0x02);
    hdmiport_out(HDMI_HDMI_ICPSNCR0_VADDR,0x0);    //Set I code
    hdmiport_out(HDMI_HDMI_ICPSNCR1_VADDR,0x0);    //Set I code
    hdmiport_out(HDMI_HDMI_PCPSNCR0_VADDR,0x00);	//Set P code
    hdmiport_out(HDMI_HDMI_PCPSNCR1_VADDR,0x0);	//Set P code
    /*Update Double Buffer*/
    hdmiport_out(HDMI_HDMI_CMCR_VADDR,0x50);   //K code =2
//	hdmiport_out(HDMI_HDMI_AFCR_VADDR, 0x50);
    //HDMI_DELAYMS(1000);
    //HdmiAudioPLLSampleDump();
#else
    hdmiport_mask(HDMI_HDMI_AOCR_VADDR,(UINT8)(~0x0ff),(UINT8)0x00);	//Disable SPDIF/I2S Output
    hdmiport_mask(HDMI_HDMI_AVMCR_VADDR,(UINT8)~(_BIT6 | _BIT5), (UINT8)_BIT6);//audio output = mute
    hdmiport_out(HDMI_HDMI_CMCR_VADDR,0x50);   //K code =2

#endif

}

UINT8 Hdmi_WaitAudioSample(void)
{


    char   timeout_cnt = 50;
    /*
    	looking for ACR info using RSV1
    */
    rtdf_pageSelect(0x02);
    hdmiport_out(HDMI_HDMI_PTRSV2_VADDR, 0x02);		// Wait for ACR : Packet Type = 0x01
    hdmi_out(HDMI_HDMI_GPVS_VADDR, _BIT6);  // Clear RSV1 indicator

    for (; timeout_cnt>0; timeout_cnt--)  // Wait 30ms max. to wait ACR
    {
        Ds_DelayXms(5);//HDMI_DELAYMS(5); //jokerxie
        if (hdmi_in(HDMI_HDMI_GPVS_VADDR) & _BIT6)
        {
            HDMI_PRINTF(LOGGER_INFO,"Audio Sample timeout_cnt : %x\n",(UINT32)timeout_cnt);
            return TRUE;
        }
    }
   // HDMI_PRINTF(LOGGER_INFO,"Audio Sample miss\n");
    return FALSE;
}

/**
 * Hdmi_AudioOutputEnable
 * Enable HDMI Audio Ouput
 *
 * @param
 * @return
 * @ingroup drv_hdmi
 */
/*	// not used Willy
void Hdmi_AudioOutputEnable(void)
{
   // Switch to Page 2 referenced from spec
   rtdf_pageSelect(2);

   hdmiport_mask(HDMI_HDMI_AVMCR_VADDR,~(_BIT6|_BIT5),(_BIT6|_BIT5));
}
*/

void Hdmi_Power(char enable)
{

    // PHY power control¡G(0x1800_FAD0[0~7] = ¡¥0x00¡¦)
    // PHY band-gap¡G(0x1800_FAEC[7] = ¡¥0¡¦)
    // HDMI MAC output enable¡G(0x1800_F298[0~7] = ¡¥0x00¡¦)
    // DP MAC control¡G(0x1800_FE84[6] = ¡¥1¡¦)

    if (enable)
    {
        rtdf_pageSelect(0x0b);
        hdmi_mask(HDMI_POWER_CONTROL_VADDR, ~0xFF, 0xFF);
        hdmi_mask(HDMI_RESERVED_01_VADDR, ~(UINT8) _BIT7, _BIT7);
        rtdf_pageSelect(2);
        hdmi_mask(HDMI_TMDS_OUTCTL_VADDR, ~0xFF, 0x78);
//		hdmi_mask(HDMI_DP_CTRL_VADDR, ~_BIT6, 0);
    }
    else
    {
        rtdf_pageSelect(0x0b);
        hdmi_mask(HDMI_POWER_CONTROL_VADDR, ~0xFF, 0);
        hdmi_mask(HDMI_RESERVED_01_VADDR, ~(UINT8) _BIT7, 0);
        rtdf_pageSelect(2);
        hdmi_mask(HDMI_TMDS_OUTCTL_VADDR, ~0xFF, 0);
//		hdmi_mask(HDMI_DP_CTRL_VADDR, ~_BIT6, _BIT6);
    }
}

/**
 * Hdmi_WaitACR
 * Wait ACR packet : ACR packet is used for transmit Audio N/CTS value
 *
 * @param 				{  }
 * @return 				{ TRUE if ACR is recieved or return FALSE }
 * @ingroup drv_hdmi
 */

UINT8 Hdmi_WaitACR(void)
{

    char   timeout_cnt = 6;

    /*
    	looking for ACR info using RSV1
    */
    // Switch to Page 2 referenced from spec
    rtdf_pageSelect(2);

    hdmiport_out(HDMI_HDMI_PTRSV1_VADDR, 0x01);		// Wait for ACR : Packet Type = 0x01
    hdmi_mask(HDMI_HDMI_GPVS_VADDR, ~(_BIT5), _BIT5);  // Clear RSV1 indicator

    for (; timeout_cnt>0; timeout_cnt--)  // Wait 30ms max. to wait ACR
    {
        if (hdmi_in(HDMI_HDMI_GPVS_VADDR) & _BIT5)
            if (Hdmi_PacketSRAMRead(78) == 0)  return TRUE; // if ACR in BCH correct
        HDMI_DELAYMS(5);
    }
    // timeout
    return FALSE;
}


/**
 * Hdmi_GetAudioFreq
 * Get Audio Sampling Freqency from N/CTS structure , SPDIF Channel Status Info or Audio info Frame
 *
 * @param <freq>			{ freq structure }
 * @return <void>			{  }
 * @ingroup drv_hdmi_audio
 */

void Hdmi_GetAudioFreq(HDMI_AUDIO_FREQ_T *freq, HDMI_AUDIO_TRACK_MODE *track_mode)
{

    unsigned long cts, n , b;
    UINT32 timeoutcnt=10;
    UINT32 Reg_val;

    HDMI_LOG(LOGGER_INFO,"Hdmi_GetAudioFreq\n");

    freq->ACR_freq = 0;
    freq->AudioInfo_freq = 0;
    freq->SPDIF_freq = 0;

    /*
    	Set trigger to get CTS&N and LPCM Channel Status Info
    */
    // Switch to Page 2 referenced from spec
    rtdf_pageSelect(2);

    //Start Pop up N_CTS value
    hdmiport_mask(HDMI_HDMI_ACRCR_VADDR, ~(_BIT1|_BIT0),_BIT1|_BIT0);

    // Restart measure b
    hdmiport_mask(HDMI_HDMI_NTX1024TR0_VADDR,~ _BIT3,_BIT3);


    // Clear Info Frame update indicator
    hdmiport_out(HDMI_HDMI_ASR0_VADDR, 0x07);


    if (Hdmi_WaitACR() == FALSE)
    {
        HDMI_LOG(LOGGER_INFO, "No CTS & N Packet\n");
    };

    //HDMI_DELAYMS(25 * 6);	//adams mask for descrease time,20110415
    //for (count = 0; count < 1000; count++) {
    /*	while(1) {

    		HDMI_LOG(LOGGER_INFO,"HDMI_HDMI_ACRCR_VADDR=%x\n",hdmi_in(HDMI_HDMI_ACRCR_VADDR));

    		HDMI_LOG(LOGGER_INFO,"HDMI_HDMI_NTX1024TR0_VADDR=%x\n",hdmi_in(HDMI_HDMI_NTX1024TR0_VADDR));

    		if (((hdmi_in(HDMI_HDMI_ACRCR_VADDR) & (_BIT1|_BIT0)) != (_BIT1 | _BIT0)) && ((hdmi_in(HDMI_HDMI_ASR0_VADDR) & 0x03) != 0x03)) break;
    	}*/

    // Delay 25ms
    //HDMI_DELAYMS(25);

    //adams add for wait N&CTS popup,20110415
    do
    {
        HDMI_DELAYMS(5);
        if ((hdmiport_in(HDMI_HDMI_ACRCR_VADDR)&0x3)==0x0)
        {
            break;
        }
    }
    while (--timeoutcnt);
    //HDMI_PRINTF(LOGGER_INFO,"HDMI time counter=%d\n",timeoutcnt);
    if (timeoutcnt==0)
    {
        HDMI_LOG(LOGGER_INFO, "CTS & N Packet popup timeout!\n");
    }

    Reg_val=hdmiport_in(HDMI_HDMI_ACRCR_VADDR);
    HDMI_PRINTF(LOGGER_INFO,"HDMI_HDMI_ACRCR_VADDR=%x\n",Reg_val);

    //if (((hdmi_in(HDMI_HDMI_ACRCR_VADDR) & (_BIT1|_BIT0)) != (_BIT1 | _BIT0)) && ((hdmi_in(HDMI_HDMI_ASR0_VADDR) & 0x03) != 0x03)) return FALSE;

    // goto METHOD_AUDIO_INFO;
    //if ((hdmi_in(HDMI_HDMI_ACRCR_VADDR) & (_BIT1|_BIT0)) != (_BIT1 | _BIT0)) goto METHOD_AUDIO_INFO;

    /*
    		Get Audio Frequency from CTS&N
    */
    cts= (((unsigned long)hdmiport_in(HDMI_HDMI_ACRSR0_VADDR)) << 12) | (((unsigned long)hdmiport_in(HDMI_HDMI_ACRSR1_VADDR))<<4)   \
         | ((unsigned long)hdmiport_in(HDMI_HDMI_ACRSR2_VADDR) & 0xf0) >> 4 ;
    n =  (((unsigned long)hdmiport_in(HDMI_HDMI_ACRSR2_VADDR) & 0xf) << 16) |  (((unsigned long)hdmiport_in(HDMI_HDMI_ACRSR3_VADDR)) << 8)  \
         |  (unsigned long)hdmiport_in(HDMI_HDMI_ACRSR4_VADDR) ;
//	b =  (((unsigned int)hdmiport_in(HDMI_HDMI_NTX1024TR0_VADDR) & 0x7) << 8) | (unsigned int)hdmiport_in(HDMI_HDMI_NTX1024TR1_VADDR) ;

    b = hdmi.b;

    HDMI_PRINTF(LOGGER_INFO,"cts=%d n=%d b=%d\n",(UINT32)cts, (UINT32)n, (UINT32)b);

    if (cts==0 || n==0 || b == 0)  goto METHOD_AUDIO_INFO;

    // 128fs = 1024/b * fx * N / CTS  =>  fs = (1024 * fx *N)/(128 * b * CTS) = (8 * fx *N)/(b*CTS)
    // calculate freq in 0.1kHz unit
    // freq = (unsigned long)8 * 2 * 10000 * HDMI_RTD_XTAL/ cts * n / ((unsigned long)b * 1000);

    freq->ACR_freq = (8 * 2 * 27000UL *10/b)*n/cts;
    freq->ACR_freq  = (freq->ACR_freq >> 1) + (freq->ACR_freq & 0x01); 	//¥|±Ë¤­¤J//
    freq->ACR_freq *= 100;
    HDMI_PRINTF(LOGGER_INFO,"\n *************** N/CTS_freq=%d\n",(unsigned long)freq->ACR_freq);
    freq->ACR_freq = Hdmi_AudioFreqCorrect(freq->ACR_freq, b, track_mode);
    HDMI_PRINTF(LOGGER_INFO,"\n *************** After Correct : N/CTS_freq=%d\n",(unsigned long)freq->ACR_freq);

    /*
    	Get Audio Frequency from Audio Info Frame
    */
METHOD_AUDIO_INFO:

    if ((hdmiport_in(HDMI_HDMI_ASR0_VADDR) & 0x03) == 0x03)
    {

        freq->SPDIF_freq = AUDIO_CHANNEL_STATUS[hdmiport_in(HDMI_HDMI_ASR1_VADDR)&0xf];

    }
    HDMI_PRINTF(LOGGER_INFO,"\n *************** SPDIF freq=%d\n",(unsigned long)freq->SPDIF_freq);


}
/**
 * drv_hdmi_AFD
 * Get Active Format Aspect Ratio from AVI infoFrame Data byte 2
 *
 * @param <void>			{ }
 * @return <Aspect Ratio>			{ 0: <no data>, 1:< 4:3>, 2:<16:9> , 3:<Future>}
 * @ingroup drv_hdmi_audio
 */

UINT8 drv_hdmi_AFD(void)
{
    UINT8 ucResult,ucAVI;
    // Switch to Page 2 referenced from spec
    rtdf_pageSelect(2);
    // Get AVI infoFrame Data byte 2
    ucAVI = Hdmi_PacketSRAMRead(0x05);
    ucResult = (ucAVI>>4)&0x03;	// Picture aspect ratio
    // Active Format Aspect Ratio analyst
    switch (ucAVI&0x0f)
    {
    case 0x09:	// 4:3(Center)
        return 1;
    case 0x0a:	// 16:9(Center)
    case 0x0b:	// 14:9(Center)
        return 2;
    case 0x08:	// Same as picture aspect ratio
        return ucResult;
    default:
        return 0;
    }
    return 0;
}

/**
 * Hdmi_GetColorSpace
 * get HDMI ColorSpace ---> RGB/YUV444/YUV422
 *
 * @param 				{  }
 * @return 				{ return HDMI COLOR SPACE }
 * @ingroup drv_hdmi
 */

HDMI_COLOR_SPACE_T Hdmi_GetColorSpace(void)
{
    return ColorMap[(hdmiport_in(HDMI_HDMI_ACRCR_RO_VADDR) >> 4) & 0x03];
}

/*	// not used Willy
HDMI_COLOR_SPACE_T drvif_Hdmi_GetColorSpace(void)
{
	return ColorMap[GET_HDMI_COLOR_SPACE()];
}
*/

/**
 * Hdmi_AudioFreqCorrect
 * Because sometimes calculated audio freqency is not accuracy , so need to correct it
 *
 * @param <freq>			{ freq }
 * @return 				{ return corrected freq result }
 * @ingroup drv_hdmi_audio
 */

unsigned long Hdmi_AudioFreqCorrect(unsigned long freq, unsigned long b, HDMI_AUDIO_TRACK_MODE *track_mode)
{

    /*
    	TO-DO : use ABS function
    */
#if 1
    UINT32 b_ratio=1000;
    UINT32 b_array[7]={2048, 1097, 1024, 512, 372, 186, 93};
    UINT8 i;

    for (i=0; i<7; i++)
    {
        if ((b > b_array[i]*966/1000) && (b <= b_array[i]*1035/1000))
        {
            b_ratio = 1000*(2*b_array[i]-b)/b_array[i];
            if ((b >= (b_array[i]-10)) && (b <= (b_array[i]+10)))
                *track_mode = HDMI_AUDIO_N_CTS_TREND_BOUND;
            else
                *track_mode = HDMI_AUDIO_TREND_BOUND;
            break;
        }
    }

    freq *= 10;
    if ((freq >= 31700*b_ratio/100) && (freq <= 32300*b_ratio/100))
        freq = 32000;
    else if ((freq >= 43500*b_ratio/100) && (freq <= 44600*b_ratio/100))
        freq = 44100;
    else if ((freq >= 47500*b_ratio/100) && (freq <= 48500*b_ratio/100))
        freq = 48000;
    else if ((freq >= 87700*b_ratio/100) && (freq <= 88700*b_ratio/100))
        freq = 88200;
    else if ((freq >= 95500*b_ratio/100) && (freq <= 96500*b_ratio/100))
        freq = 96000;
    else if ((freq >= 175400*b_ratio/100) && (freq <= 177400*b_ratio/100))
        freq = 176400;
    else if ((freq >= 191000*b_ratio/100) && (freq <= 193000*b_ratio/100))
        freq = 192000;
    else
        freq = 0;
#else
    if ((freq >= 31700) && (freq <= 32300))
        freq = 32000;
    else if ((freq >= 43500) && (freq <= 44600))
        freq = 44100;
    else if ((freq >= 47500) && (freq <= 48500))
        freq = 48000;
    else if ((freq >= 87700) && (freq <= 88700))
        freq = 88200;
    else if ((freq >= 95500) && (freq <= 96500))
        freq = 96000;
    else if ((freq >= 175400) && (freq <= 177400))
        freq = 176400;
    else if ((freq >= 191000) && (freq <= 193000))
        freq = 192000;
    else
        freq = 0;
#endif
    return freq;
}


#if HDMI_RUNTIME_AUDIO_PLL_CAL
/**
 * Hdmi_AudioPLLSetting
 * setting Audio PLL and enable Audio output
 *
 * @param <freq>			{ Audio sampling frequency }
 * @return 				{ buffer length,"FLASE" means freqency is not standard freqency }
 * @ingroup drv_hdmi
 */

HDMI_BOOL Hdmi_AudioPLLSetting(unsigned long freq, int use_trend_tracking)
{

    UINT8 coeff = 0, rate = 0, lvr=0;
    INT8 s = 0, o = 2;
    unsigned int   m=0 ,a=0;
    unsigned long  fvco=0;//1226 kist
    UINT32 tmp1, tmp2;
    UINT8 i;
    int timeout = 10;
    HDMI_LOG(LOGGER_INFO, "HDMI_HDMI_AVMCR_VADDR = %x\n", (UINT32)hdmiport_in(HDMI_HDMI_AVMCR_VADDR));

    // patch for Audio FIFO unstable at begining of audio auto flow
    hdmiport_mask(HDMI_HDMI_WDCR0_VADDR ,(UINT8)~(_BIT1 | _BIT2),(UINT8)0x00);//Disable Set_AVMute Watch Dog
    hdmiport_mask(HDMI_HDMI_DPCR0_VADDR,  (UINT8)~( _BIT7 | _BIT6), (UINT8)(_BIT7 | _BIT6)); // Disable PLL
    HDMI_DELAYMS(1);
    for (i=0; i < 7; i++)
    {
        if (audio_pll_coeff[i].freq == freq)
        {
            coeff = audio_pll_coeff[i].coeff;
            rate = audio_pll_coeff[i].rate;
            goto PLL_SETTING;
        }
    }
    HDMI_PRINTF(LOGGER_INFO, "Unsupport audio freq = %d\n", freq);
    return FALSE;

PLL_SETTING:

    do
    {
        s = s + 4;
        fvco = (UINT32) 128 * freq * coeff * s;
    }
    while (fvco < 180000000);


    // Fa(Audio Freq) = 128 * Fs(Sampling Freq)
    // Fa(Audio Freq) =  Xtal * (M/K/2^O/(2S))
    // Getting :   M  = 128 * Fs * 2^O * 2S * K / Xtal;
    // K=1 , O=1;
    m = (fvco << o ) / ((UINT32)RTD_XTAL * 1000); // cyyeh 20080415
    //m = (DWORD)2 * 128 * freq * coeff * s / _RTD_XTAL / 1000;
    //m = (m >> 1) + (m & 0x01);
    m = m + 1;
    s =  s / (o * 2);



    // Calculate D code
    tmp2 = (UINT32)1000 * RTD_XTAL * m / 2;  // PLL freq
    if (fvco > tmp2)
    {
        a = (fvco - tmp2) * 128 / (tmp2 / 2048);

        a = 0xffff - a;
    }
    else
    {
        a = (tmp2- fvco) * 128 / (tmp2 / 2048);

        //      a += 100; // MUST for compatibility
    }

    o = o + 1;



    hdmiport_out(HDMI_HDMI_AAPNR_VADDR, 0x08);	// Disable SDM
    hdmiport_out(HDMI_HDMI_MCAPR_VADDR, (m - 2));
    hdmiport_out(HDMI_HDMI_SCAPR_VADDR,  (coeff == _AUDIO_256_TIMES) ? ((s / 2) | 0x80) : (s / 2));
    hdmiport_out(HDMI_HDMI_DCAPR0_VADDR, a >> 8);
    hdmiport_out(HDMI_HDMI_DCAPR1_VADDR, a & 0xff);
    hdmiport_mask(HDMI_HDMI_DPCR0_VADDR, ~(_BIT4 | _BIT5), (o << 4));

    /*Calculate Ich for audio PLL*/
    //            pData[0] = (m < 1067 / 100) ? 0 : ((m * 100/ 1067) - 1); // cyyeh 20080415

    tmp1 = ((UINT32) m * 4 * 100 / 1067);    // 2bit fractional part
    lvr = 0x00;

    if (tmp1 >=10)
    {
        tmp1 -= 10;
    }

    if (tmp1>= 40)       // 2bit fractional part
    {
        tmp1 -= 40;
        lvr |= 0x04;
    }

    if (tmp1 >= 20)       // 2bit fractional part
    {
        tmp1 -= 20;
        lvr |= 0x02;
    }

    if (tmp1 >= 10)       // 2bit fractional part
    {
        tmp1-= 10;
        lvr |= 0x01;
    }
    hdmiport_out(HDMI_HDMI_DPCR1_VADDR, lvr | 0x70);

    hdmiport_out(HDMI_HDMI_CMCR_VADDR,0x50);		//Enable Double Buffer for K/M/S/D/O
    HDMI_DELAYMS(1);
    //hdmi_mask(HDMI_HDMI_DPCR0_VADDR, ~(_BIT6 | (UINT8) _BIT7),0x00); // Enable PLL --> move down,after  "Enable Set_AVMute Watch Dog" for o code often not wirte in bug!
    //hdmi_out(HDMI_HDMI_AFCR_VADDR, 0x00);
    //hdmi_mask(HDMI_HDMI_AOCR_VADDR,~(0x0ff),0x00);//Disable SPDIF/I2S Output
//	hdmiport_mask(HDMI_HDMI_WDCR0_VADDR ,~AV_WD_AVMute,0x00);//Disable Set_AVMute Watch Dog
    hdmiport_mask(HDMI_HDMI_AVMCR_VADDR , ~(_BIT5|_BIT6),_BIT6); //Enable Audio Output
//	hdmi_mask(HDMI_HDMI_AVMCR_VADDR , ~(_BIT5|_BIT6),0); //Enable Audio Output
//	hdmiport_mask(HDMI_HDMI_WDCR0_VADDR , ~AV_WD_AVMute,AV_WD_AVMute);//Enable Set_AVMute Watch Dog
    hdmiport_mask(HDMI_HDMI_DPCR0_VADDR, (UINT8)(~(_BIT6 | _BIT7)),0x00); // Enable PLL


    HDMI_PRINTF(LOGGER_INFO," m = %x\n o = %x\n s = %x\n a=%x\n", m, o, s, a);
    HDMI_PRINTF(LOGGER_INFO," fvco=%lx\n",fvco);

    hdmi_out(HDMI_HDMI_SR_VADDR, hdmi_in(HDMI_HDMI_SR_VADDR));
    hdmi_out(HDMI_HDMI_SR_VADDR, _BIT3);  // clear PLL lock indicator
    if (use_trend_tracking) /*Trend& Boundary Tracking*/
    {
        HDMI_LOG(LOGGER_INFO,"\n *****Trend& Boundary Tracking*****\n");
        //hdmi_out(HDMI_HDMI_PSCR_VADDR,0xEC);		//Enable boundary tracking  0x04
        hdmiport_out(HDMI_HDMI_PSCR_VADDR,0x6C);		//Enable boundary tracking  0x04
        hdmiport_out(HDMI_HDMI_CMCR_VADDR,0x50);		//update double buffer
        hdmiport_out(HDMI_HDMI_ICBPSR1_VADDR,0x01);      	//Set I code   //0x01
        hdmiport_out(HDMI_HDMI_PCBPSR1_VADDR,0x00);		//Set P code         //0x01
        hdmiport_out(HDMI_HDMI_STBPR_VADDR,0x01);//0x80		//Set Boundary Tracking Update Response Time
        hdmiport_out(HDMI_HDMI_FBR_VADDR,0x72);
        hdmiport_out(HDMI_HDMI_APDMCR_VADDR,0xC0);       	//power on swallow maybe   //kist
        hdmiport_out(HDMI_HDMI_AAPNR_VADDR,0xC2);         	//Enable SDM
        hdmiport_mask(HDMI_HDMI_WDCR0_VADDR,~_BIT5,_BIT5);
        hdmiport_out(HDMI_HDMI_ICTPSR1_VADDR,0x10);      	//Fine tune    0x07
    }
    else //  H/W N/CTS Tracking
    {
        HDMI_PRINTF(LOGGER_INFO,"\n ***** N/CTS Tracking*****\n");
        hdmiport_out(HDMI_HDMI_APDMCR_VADDR,0x00);       //disable trend & boundary tracking
        hdmiport_out(HDMI_HDMI_PSCR_VADDR,0xF2);		//Enable N/CTS tracking & FIFO depth
        hdmiport_out(HDMI_HDMI_ICPSNCR1_VADDR,0x30);    //Set I code
        hdmiport_out(HDMI_HDMI_PCPSNCR1_VADDR,0xFF);	//Set P code
        hdmiport_out(HDMI_HDMI_AAPNR_VADDR,0x02);		//Enable SDM
        hdmiport_out(HDMI_HDMI_CMCR_VADDR,0x50);           //updatedouble buffer
    }
    // new PLL setting for LV

    hdmiport_mask(HDMI_HDMI_DPCR2_VADDR,  ~(_BIT4|_BIT3), _BIT4); // Set VCO default
    HDMI_DELAYMS(10);
    hdmiport_mask(HDMI_HDMI_DPCR0_VADDR, ~_BIT0, _BIT0);         // Reg DPLL_CMPEN
    HDMI_DELAYMS(10);
    hdmiport_mask(HDMI_HDMI_DPCR0_VADDR, ~_BIT1, _BIT1);         // Reg DPLL_CALLCH
    HDMI_DELAYMS(10);
    hdmiport_mask(HDMI_HDMI_DPCR0_VADDR, ~_BIT2, _BIT2);         // Reg DPLL_CALSW

    HDMI_DELAYMS(70);
    // jj hdmi_mask(HDMI_HDMI_SR_VADDR,~(_BIT1 | _BIT2),_BIT1 | _BIT2);	//Write 1 clear

    //hdmi_out(HDMI_HDMI_SR_VADDR, hdmi_in(HDMI_HDMI_SR_VADDR));
    for (timeout = 0; timeout < 5; timeout++)
    {

        if ((hdmi_in(HDMI_HDMI_SR_VADDR) & (_BIT3)) == 0) break;
        HDMI_DELAYMS(10);
    }

    if (timeout == 10)
    {
        HDMI_LOG(LOGGER_INFO, "PLL not lock %d %x\n", timeout, (UINT32)hdmiport_in(HDMI_HDMI_AVMCR_VADDR));
        HDMI_LOG(LOGGER_INFO, "HDMI_HDMI_SR_VADDR = %x\n", (UINT32)hdmi_in(HDMI_HDMI_SR_VADDR));
        HDMI_LOG(LOGGER_INFO, "HDMI_HDMI_AFCR_RO_VADDR = %x\n", (UINT32)hdmiport_in(HDMI_HDMI_AFCR_VADDR));
        HDMI_LOG(LOGGER_INFO, "HDMI_HDMI_AFCR_VADDR = %x\n", (UINT32)hdmiport_in(HDMI_HDMI_AFCR_VADDR));
    }
    else
    {
        HDMI_LOG(LOGGER_INFO, "PLL lock  %d %x\n", timeout, (UINT32)hdmiport_in(HDMI_HDMI_AVMCR_VADDR));
        HDMI_LOG(LOGGER_INFO, "HDMI_HDMI_SR_VADDR = %x\n", (UINT32)hdmi_in(HDMI_HDMI_SR_VADDR));
        HDMI_LOG(LOGGER_INFO, "HDMI_HDMI_AFCR_VADDR = %x\n", (UINT32)hdmiport_in(HDMI_HDMI_AFCR_VADDR));
        HDMI_LOG(LOGGER_INFO, "HDMI_HDMI_AFCR_VADDR = %x\n", (UINT32)hdmiport_in(HDMI_HDMI_AFCR_VADDR));
    }
    //while(1);
    //hdmi_mask(HDMI_HDMI_AVMCR_VADDR , ~(_BIT5|_BIT6),_BIT5|_BIT6); //Enable Audio Output
    HDMI_PRINTF(LOGGER_INFO, "HDMI_HDMI_SR_VADDR = %x\n",(UINT32)hdmi_in(HDMI_HDMI_SR_VADDR));
    HDMI_PRINTF(LOGGER_INFO, "HDMI_HDMI_AVMCR_VADDR = %x\n", (UINT32)hdmiport_in(HDMI_HDMI_AVMCR_VADDR));
    hdmi_out(HDMI_HDMI_SR_VADDR, hdmi_in(HDMI_HDMI_SR_VADDR));
    //hdmiport_out(HDMI_HDMI_MAGCR1_VADDR, 0x0F);
    hdmiport_out(HDMI_HDMI_AFCR_VADDR, 0x40);
    return TRUE;
}

#elif 0
/**
 * Hdmi_AudioPLLSetting
 * setting Audio PLL and enable Audio output
 *
 * @param <freq>			{ Audio sampling frequency }
 * @return 				{ buffer length,"FLASE" means freqency is not standard freqency }
 * @ingroup drv_hdmi
 */

HDMI_BOOL Hdmi_AudioPLLSetting(unsigned long freq, HDMI_AUDIO_TRACK_MODE track_mode)
{

    UINT8 coeff = 0, rate = 0, lvr=0;
    UINT8 i;
    int timeout = 10;
    UINT32 tmp1;
    HDMI_LOG(LOGGER_INFO, "HDMI_HDMI_AVMCR_VADDR = %x\n", (UINT32)hdmiport_in(HDMI_HDMI_AVMCR_VADDR));


    for (i=0; i < 7; i++)
    {
        if (audio_pll_coeff[i].freq == freq)
        {
            coeff = audio_pll_coeff[i].coeff;
            rate = audio_pll_coeff[i].rate;
            goto PLL_SETTING;
        }
    }
    HDMI_PRINTF(LOGGER_INFO, "Unsupport audio freq = %d\n", freq);
    return FALSE;

PLL_SETTING:

    HDMI_DELAYMS(1);
    /*Disable N/CTS tracking*/
    hdmiport_out(HDMI_HDMI_PSCR_VADDR,0x02);

    // set D code 0
    hdmiport_out(HDMI_HDMI_DCAPR0_VADDR, 0);	//D=0
    hdmiport_out(HDMI_HDMI_DCAPR1_VADDR, 0);    //D=0

    /*Update Double Buffer*/
    hdmiport_out(HDMI_HDMI_CMCR_VADDR,0x50);   //K code =2
//	hdmiport_out(HDMI_HDMI_AFCR_VADDR, 0x40);
    hdmiport_out(HDMI_HDMI_AFCR_VADDR, 0x02); //20101020

    hdmiport_mask(HDMI_HDMI_WDCR0_VADDR ,(UINT8)~(_BIT1 | _BIT2),(UINT8)0x00);//Disable Set_AVMute Watch Dog
    hdmiport_mask(HDMI_HDMI_DPCR0_VADDR,  (UINT8)~( _BIT7 | _BIT6), (UINT8)(_BIT7 | _BIT6)); // Disable PLL
    hdmi_out(HDMI_HDMI_SR_VADDR, hdmi_in(HDMI_HDMI_SR_VADDR));
    HDMI_DELAYMS(1);

    hdmiport_out(HDMI_HDMI_AAPNR_VADDR, 0x08);	// Disable SDM
    hdmiport_out(HDMI_HDMI_MCAPR_VADDR, (hdmi_audiopll_param[i].M- 2));
    hdmiport_out(HDMI_HDMI_SCAPR_VADDR,  (hdmi_audiopll_param[i].S1) ? ((hdmi_audiopll_param[i].S / 2) | 0x80) : (hdmi_audiopll_param[i].S / 2));
    hdmiport_out(HDMI_HDMI_DCAPR0_VADDR, hdmi_audiopll_param[i].D_HighByte);
    hdmiport_out(HDMI_HDMI_DCAPR1_VADDR, hdmi_audiopll_param[i].D_LowByte);
    hdmiport_mask(HDMI_HDMI_DPCR0_VADDR, ~(_BIT4 | _BIT5), (hdmi_audiopll_param[i].O << 4));

    /*Calculate Ich for audio PLL*/
    //            pData[0] = (m < 1067 / 100) ? 0 : ((m * 100/ 1067) - 1); // cyyeh 20080415

    tmp1 = ((UINT32) hdmi_audiopll_param[i].M * 4 * 100 / 1067);    // 2bit fractional part
    lvr = 0x00;

    if (tmp1 >=10)
    {
        tmp1 -= 10;
    }

    if (tmp1>= 40)       // 2bit fractional part
    {
        tmp1 -= 40;
        lvr |= 0x04;
    }

    if (tmp1 >= 20)       // 2bit fractional part
    {
        tmp1 -= 20;
        lvr |= 0x02;
    }

    if (tmp1 >= 10)       // 2bit fractional part
    {
        tmp1-= 10;
        lvr |= 0x01;
    }
    hdmiport_out(HDMI_HDMI_DPCR1_VADDR, lvr | 0x70);


    hdmiport_out(HDMI_HDMI_CMCR_VADDR,0x50);		//Enable Double Buffer for K/M/S/D/O
    HDMI_DELAYMS(1);
    //hdmi_mask(HDMI_HDMI_DPCR0_VADDR, ~(_BIT6 | (UINT8) _BIT7),0x00); // Enable PLL --> move down,after  "Enable Set_AVMute Watch Dog" for o code often not wirte in bug!
    //hdmi_out(HDMI_HDMI_AFCR_VADDR, 0x00);
    //hdmi_mask(HDMI_HDMI_AOCR_VADDR,~(0x0ff),0x00);//Disable SPDIF/I2S Output
//	hdmiport_mask(HDMI_HDMI_WDCR0_VADDR ,~AV_WD_AVMute,0x00);//Disable Set_AVMute Watch Dog
//	hdmiport_mask(HDMI_HDMI_AVMCR_VADDR , ~(_BIT5|_BIT6),_BIT6); //Enable Audio Output
//	hdmi_mask(HDMI_HDMI_AVMCR_VADDR , ~(_BIT5|_BIT6),0); //Enable Audio Output
//	hdmiport_mask(HDMI_HDMI_WDCR0_VADDR , ~AV_WD_AVMute,AV_WD_AVMute);//Enable Set_AVMute Watch Dog
    hdmiport_mask(HDMI_HDMI_DPCR0_VADDR, (UINT8)(~(_BIT6 | _BIT7)),0x00); // Enable PLL

    HDMI_PRINTF(LOGGER_INFO," m = %x\n o = %x\n s = %x\n ", (UINT32)hdmi_audiopll_param[i].M, (UINT32)hdmi_audiopll_param[i].O, (UINT32)hdmi_audiopll_param[i].S);


    hdmi_out(HDMI_HDMI_SR_VADDR, hdmi_in(HDMI_HDMI_SR_VADDR));
    hdmi_out(HDMI_HDMI_SR_VADDR, _BIT3);  // clear PLL lock indicator
//#if N_CTS_FIFO_TRACKING
    if (track_mode == HDMI_AUDIO_N_CTS_TREND_BOUND)
    {
        HDMI_PRINTF(LOGGER_INFO,"\n *****N/CTS Trend& Boundary Tracking*****\n");
#if 0  //old setting
        hdmiport_out(HDMI_HDMI_ICBPSR1_VADDR,0x01);      	//Set I code   //0x01
        hdmiport_out(HDMI_HDMI_PCBPSR1_VADDR,0x00);		//Set P code         //0x01
        hdmiport_out(HDMI_HDMI_STBPR_VADDR,0x01);//0x80		//Set Boundary Tracking Update Response Time
        hdmiport_out(HDMI_HDMI_FBR_VADDR,0x72);
        hdmiport_out(HDMI_HDMI_ICTPSR1_VADDR,0x10);      	//Fine tune    0x07

        hdmiport_out(HDMI_HDMI_ICPSNCR0_VADDR,0x0);    //Set I code
        hdmiport_out(HDMI_HDMI_ICPSNCR1_VADDR,0x03);    //Set I code
        hdmiport_out(HDMI_HDMI_PCPSNCR0_VADDR,0x02);	//Set P code
        hdmiport_out(HDMI_HDMI_PCPSNCR1_VADDR,0xFF);	//Set P code

        hdmiport_out(HDMI_HDMI_APDMCR_VADDR,0xC0);       	//power on swallow maybe   //kist
        hdmiport_out(HDMI_HDMI_AAPNR_VADDR,0xC2);         	//Enable SDM
        hdmiport_out(HDMI_HDMI_PSCR_VADDR,0xFE);		//Enable boundary tracking  0x04
        hdmiport_mask(HDMI_HDMI_WDCR0_VADDR,~_BIT5,_BIT5);

        hdmiport_out(HDMI_HDMI_CMCR_VADDR,0x50);           //updatedouble buffer
#else

		hdmiport_out(HDMI_HDMI_PSCR_VADDR,0x9E);		//Enable boundary tracking  0x04
		hdmiport_out(HDMI_HDMI_ICPSNCR0_VADDR,0x0);    //Set I code of Ncts[15:8]
		hdmiport_out(HDMI_HDMI_ICPSNCR1_VADDR,0x02);    //Set I code of Ncts [7:0]
		hdmiport_out(HDMI_HDMI_PCPSNCR0_VADDR,0x02);	//Set P code of Ncts [15:8]
		hdmiport_out(HDMI_HDMI_PCPSNCR1_VADDR,0xFF);	//Set P code of Ncts [7:0]

		hdmiport_out(HDMI_HDMI_ICTPSR0_VADDR,0x00);      	//Set I code  of trend [15:8]
		hdmiport_out(HDMI_HDMI_ICTPSR1_VADDR,0x05);      	//Set I code  of trend [7:0]
		hdmiport_out(HDMI_HDMI_PCTPSR0_VADDR,0x02);	//Set P code of trend [15:8]        //0x01
		hdmiport_out(HDMI_HDMI_PCTPSR1_VADDR,0xFF);	//Set P code of trend [7:0]        //0x01
		hdmiport_out(HDMI_HDMI_ICBPSR0_VADDR,0x00);      //Set I code of bnd [15:8]  //0x01
		hdmiport_out(HDMI_HDMI_ICBPSR1_VADDR,0x05);      //Set I code of bnd [7:0]  //0x01
		hdmiport_out(HDMI_HDMI_PCBPSR0_VADDR,0x02);	//Set P code of bnd [15:8]        //0x01
		hdmiport_out(HDMI_HDMI_PCBPSR1_VADDR,0xFF);	//Set P code of bnd [7:0]        //0x01
		hdmiport_out(HDMI_HDMI_CMCR_VADDR,0x50);		//update double buffer
		hdmiport_out(HDMI_HDMI_STBPR_VADDR,0x01);//0x80		//Set Boundary Tracking Update Response Time
		hdmiport_out(HDMI_HDMI_FBR_VADDR,0x74);
		hdmiport_out(HDMI_HDMI_APDMCR_VADDR,0xC0);       	//power on swallow maybe   //kist
		hdmiport_out(HDMI_HDMI_AAPNR_VADDR,0xC2);         	//Enable SDM
		hdmiport_mask(HDMI_HDMI_WDCR0_VADDR,~_BIT5,_BIT5);
		hdmiport_out(HDMI_HDMI_CMCR_VADDR,0x50);		//update double buffer
#endif

    }
    else if (track_mode == HDMI_AUDIO_TREND_BOUND)
    {
//#else
//      if(use_trend_tracking){ /*Trend& Boundary Tracking*/
        HDMI_LOG(LOGGER_INFO,"\n *****Trend& Boundary Tracking*****\n");
        //hdmi_out(HDMI_HDMI_PSCR_VADDR,0xEC);		//Enable boundary tracking  0x04
        hdmiport_out(HDMI_HDMI_PSCR_VADDR,0x6C);		//Enable boundary tracking  0x04
        hdmiport_out(HDMI_HDMI_CMCR_VADDR,0x50);		//update double buffer
//		hdmiport_out(HDMI_HDMI_ICBPSR1_VADDR,0x01);      	//Set I code   //0x01
//		hdmiport_out(HDMI_HDMI_PCBPSR1_VADDR,0x00);		//Set P code         //0x01

        hdmiport_out(HDMI_HDMI_ICTPSR0_VADDR,0x00);      	//Set I code  of trend [15:8]
        hdmiport_out(HDMI_HDMI_ICTPSR1_VADDR,0x05);      	//Set I code  of trend [7:0]
        hdmiport_out(HDMI_HDMI_PCTPSR0_VADDR,0x02);	//Set P code of trend [15:8]        //0x01
        hdmiport_out(HDMI_HDMI_PCTPSR1_VADDR,0xFF);	//Set P code of trend [7:0]        //0x01
        hdmiport_out(HDMI_HDMI_ICBPSR0_VADDR,0x00);      //Set I code of bnd [15:8]  //0x01
        hdmiport_out(HDMI_HDMI_ICBPSR1_VADDR,0x05);      //Set I code of bnd [7:0]  //0x01
        hdmiport_out(HDMI_HDMI_PCBPSR0_VADDR,0x02);	//Set P code of bnd [15:8]        //0x01
        hdmiport_out(HDMI_HDMI_PCBPSR1_VADDR,0xFF);	//Set P code of bnd [7:0]        //0x01
        hdmiport_out(HDMI_HDMI_CMCR_VADDR,0x50);		//update double buffer


        hdmiport_out(HDMI_HDMI_STBPR_VADDR,0x01);//0x80		//Set Boundary Tracking Update Response Time
        hdmiport_out(HDMI_HDMI_FBR_VADDR,0x72);
        hdmiport_out(HDMI_HDMI_APDMCR_VADDR,0xC0);       	//power on swallow maybe   //kist
        hdmiport_out(HDMI_HDMI_AAPNR_VADDR,0xC2);         	//Enable SDM
        hdmiport_mask(HDMI_HDMI_WDCR0_VADDR,~_BIT5,_BIT5);
        //hdmiport_out(HDMI_HDMI_ICTPSR1_VADDR,0x10);      	//Fine tune    0x07 //For SVN1911
    }
    else
    {//  H/W N/CTS Tracking
        HDMI_PRINTF(LOGGER_INFO,"\n ***** N/CTS Tracking*****\n");
        hdmiport_out(HDMI_HDMI_APDMCR_VADDR,0x00);       //disable trend & boundary tracking
        hdmiport_out(HDMI_HDMI_PSCR_VADDR,0xF2);		//Enable N/CTS tracking & FIFO depth
        hdmiport_out(HDMI_HDMI_ICPSNCR0_VADDR,0x0);    //Set I code
        hdmiport_out(HDMI_HDMI_ICPSNCR1_VADDR,0x03);    //Set I code
        hdmiport_out(HDMI_HDMI_PCPSNCR0_VADDR,0x02);	//Set P code
        hdmiport_out(HDMI_HDMI_PCPSNCR1_VADDR,0xFF);	//Set P code
        hdmiport_out(HDMI_HDMI_AAPNR_VADDR,0x02);		//Enable SDM
        hdmiport_out(HDMI_HDMI_CMCR_VADDR,0x50);           //updatedouble buffer
    }
//#endif
    // new PLL setting for LV

    hdmiport_mask(HDMI_HDMI_DPCR2_VADDR,  ~(_BIT4|_BIT3), _BIT4); // Set VCO default
    HDMI_DELAYMS(10);
    hdmiport_mask(HDMI_HDMI_DPCR0_VADDR, ~_BIT0, _BIT0);         // Reg DPLL_CMPEN
    HDMI_DELAYMS(10);
    hdmiport_mask(HDMI_HDMI_DPCR0_VADDR, ~_BIT1, _BIT1);         // Reg DPLL_CALLCH
    HDMI_DELAYMS(10);
    hdmiport_mask(HDMI_HDMI_DPCR0_VADDR, ~_BIT2, _BIT2);         // Reg DPLL_CALSW

    HDMI_DELAYMS(70);
    // jj hdmi_mask(HDMI_HDMI_SR_VADDR,~(_BIT1 | _BIT2),_BIT1 | _BIT2);	//Write 1 clear

    //hdmi_out(HDMI_HDMI_SR_VADDR, hdmi_in(HDMI_HDMI_SR_VADDR));
    for (timeout = 0; timeout < 5; timeout++)
    {

        if ((hdmi_in(HDMI_HDMI_SR_VADDR) & (_BIT3)) == 0) break;
        HDMI_DELAYMS(10);
    }
    Hdmi_VideoOutputEnable();
    if (timeout == 10)
    {
        HDMI_LOG(LOGGER_INFO, "PLL not lock %d %x\n", (UINT32)timeout, (UINT32)hdmiport_in(HDMI_HDMI_AVMCR_VADDR));
        HDMI_LOG(LOGGER_INFO, "HDMI_HDMI_SR_VADDR = %x\n", (UINT32)hdmi_in(HDMI_HDMI_SR_VADDR));
        HDMI_LOG(LOGGER_INFO, "HDMI_HDMI_AFCR_RO_VADDR = %x\n", (UINT32)hdmiport_in(HDMI_HDMI_AFCR_VADDR));
        HDMI_LOG(LOGGER_INFO, "HDMI_HDMI_AFCR_VADDR = %x\n", (UINT32)hdmiport_in(HDMI_HDMI_AFCR_VADDR));
    }
    else
    {
        HDMI_LOG(LOGGER_INFO, "PLL lock  %d %x\n", (UINT32)timeout, (UINT32)hdmiport_in(HDMI_HDMI_AVMCR_VADDR));
        HDMI_LOG(LOGGER_INFO, "HDMI_HDMI_SR_VADDR = %x\n", (UINT32)hdmi_in(HDMI_HDMI_SR_VADDR));
        HDMI_LOG(LOGGER_INFO, "HDMI_HDMI_AFCR_VADDR = %x\n", (UINT32)hdmiport_in(HDMI_HDMI_AFCR_VADDR));
    }
    //while(1);
    //hdmi_mask(HDMI_HDMI_AVMCR_VADDR , ~(_BIT5|_BIT6),_BIT5|_BIT6); //Enable Audio Output
    HDMI_PRINTF(LOGGER_INFO, "HDMI_HDMI_SR_VADDR = %x\n", (UINT32)hdmi_in(HDMI_HDMI_SR_VADDR));
    HDMI_PRINTF(LOGGER_INFO, "HDMI_HDMI_AVMCR_VADDR = %x\n", (UINT32)hdmiport_in(HDMI_HDMI_AVMCR_VADDR));
    hdmi_out(HDMI_HDMI_SR_VADDR, hdmi_in(HDMI_HDMI_SR_VADDR));
    //hdmiport_out(HDMI_HDMI_MAGCR1_VADDR, 0x0F);
    hdmiport_out(HDMI_HDMI_AFCR_VADDR, 0x40);
    return TRUE;
}
#else //adams add new flow,20110504

/**
 * Hdmi_AudioPLLSetting
 * setting Audio PLL and enable Audio output
 *
 * @param <freq>			{ Audio sampling frequency }
 * @return				{ buffer length,"FLASE" means freqency is not standard freqency }
 * @ingroup drv_hdmi
 */

HDMI_BOOL Hdmi_AudioPLLSetting(unsigned long freq, HDMI_AUDIO_TRACK_MODE track_mode)
{

    UINT8 coeff = 0, rate = 0, lvr=0;
    UINT8 i;
    int timeout = 10;
    UINT32 tmp1;
//    UINT32 N,CTS;
//    UINT32 cts,n,b;
//    HDMIRX_IOCTL_STRUCT_T isr_info;
    HDMI_LOG(LOGGER_INFO, "HDMI audio PLL setting\n");


    for (i=0; i < 7; i++)
    {
        if (audio_pll_coeff[i].freq == freq)
        {
            coeff = audio_pll_coeff[i].coeff;
            rate = audio_pll_coeff[i].rate;
            goto PLL_SETTING;
        }
    }
    HDMI_PRINTF(LOGGER_INFO, "Unsupport audio freq = %d\n", freq);
    return FALSE;

PLL_SETTING:

    hdmiport_mask(HDMI_HDMI_WDCR1_VADDR ,(UINT8)~( _BIT7),(UINT8) 0x00);	//disable  tmds clock audio watch dog
    hdmiport_mask(HDMI_HDMI_WDCR0_VADDR , (UINT8)~(_BIT1 |_BIT2|_BIT3 | _BIT4 | _BIT7 ),(UINT8)0x00);	//Disable Set_AVMute Watch Dog

    hdmiport_out(HDMI_HDMI_AFCR_VADDR, 0x40);	//audio auto output enable
    hdmiport_mask(HDMI_HDMI_WDCR0_VADDR ,(UINT8)~(_BIT5),(UINT8)0x00);//Disable FIFO depth and trend tracking into SDM
    hdmiport_mask(HDMI_HDMI_PSCR_VADDR,(UINT8)~(_BIT3|_BIT2),(UINT8)0x0);		//disable FIFO depth & trend tracking
    hdmiport_mask(HDMI_HDMI_CMCR_VADDR,  (UINT8)~( _BIT4), (UINT8)( _BIT4));	//Enable Double Buffer
    hdmiport_mask(HDMI_HDMI_AVMCR_VADDR,(UINT8)~(_BIT5),(UINT8)0x0);		//disable AOMC, let FSM into Pre-mode
    hdmiport_mask(HDMI_HDMI_PSCR_VADDR,(UINT8)~(_BIT4),(UINT8)0x0);		//disable N&CTS tracking
    hdmiport_mask(HDMI_HDMI_CMCR_VADDR,  (UINT8)~( _BIT4), (UINT8)( _BIT4));	//Enable Double Buffer
    hdmiport_mask(HDMI_HDMI_AAPNR_VADDR,(UINT8)~(_BIT1),(UINT8)0x0);			//disable SDM
    hdmiport_mask(HDMI_HDMI_DPCR0_VADDR,  (UINT8)~( _BIT7 | _BIT6|_BIT2|_BIT1|_BIT0), (UINT8)(_BIT7 | _BIT6)); // Disable PLL and cmp&latch&cali. disable

//adams add to avoid dead lock,20110516
    hdmiport_mask(HDMI_HDMI_CMCR_VADDR,  (UINT8)~( _BIT6|_BIT5), (UINT8)0x0); // PLL output clk sel from crystal
    hdmiport_out(HDMI_HDMI_SCAPR_VADDR,0x00);	//S1 & S2 code clear to 0 , to avoid dead lock
    hdmiport_out(HDMI_HDMI_CMCR_VADDR, 0x10);	//Enable Double Buffer
    HDMI_DELAYMS(1);
    hdmiport_mask(HDMI_HDMI_CMCR_VADDR,  (UINT8)~( _BIT6|_BIT5), (UINT8)(_BIT6)); // PLL output clk sel from VCO
    hdmiport_out(HDMI_HDMI_CMCR_VADDR,  0x50);	//Enable Double Buffer


    // set D & M & N code
    hdmiport_out(HDMI_HDMI_DCAPR0_VADDR, hdmi_audiopll_param[i].D_HighByte);
    hdmiport_out(HDMI_HDMI_DCAPR1_VADDR, hdmi_audiopll_param[i].D_LowByte);
    hdmiport_out(HDMI_HDMI_MCAPR_VADDR, (hdmi_audiopll_param[i].M- 2));
    hdmiport_out(HDMI_HDMI_SCAPR_VADDR,  (hdmi_audiopll_param[i].S1) ? ((hdmi_audiopll_param[i].S / 2) | 0x80) : (hdmi_audiopll_param[i].S / 2));
    hdmiport_mask(HDMI_HDMI_DPCR0_VADDR, ~(_BIT4 | _BIT5), (hdmi_audiopll_param[i].O << 4));
    hdmiport_mask(HDMI_HDMI_CMCR_VADDR,  (UINT8)~( _BIT4), (UINT8)( _BIT4));   //K code =2 and enable double buffer


    /*Calculate Ich for audio PLL*/
    //			 pData[0] = (m < 1067 / 100) ? 0 : ((m * 100/ 1067) - 1); // cyyeh 20080415

    tmp1 = ((UINT32) hdmi_audiopll_param[i].M * 4 * 100 / 1067);    // 2bit fractional part
    lvr = 0x00;

    if (tmp1 >=10)
    {
        tmp1 -= 10;
    }

    if (tmp1>= 40)		// 2bit fractional part
    {
        tmp1 -= 40;
        lvr |= 0x04;
    }

    if (tmp1 >= 20)		 // 2bit fractional part
    {
        tmp1 -= 20;
        lvr |= 0x02;
    }

    if (tmp1 >= 10)		 // 2bit fractional part
    {
        tmp1-= 10;
        lvr |= 0x01;
    }
    hdmiport_out(HDMI_HDMI_DPCR1_VADDR, lvr | 0x70);

    hdmiport_mask(HDMI_HDMI_CMCR_VADDR,  (UINT8)~( _BIT4), (UINT8)( _BIT4));		//Enable Double Buffer for K/M/S/D/O
    HDMI_DELAYMS(1);

    hdmiport_mask(HDMI_HDMI_DPCR2_VADDR,  (UINT8)~(_BIT4|_BIT3),(UINT8)( _BIT4)); // Set VCO default
    hdmiport_mask(HDMI_HDMI_DPCR0_VADDR, (UINT8)~(_BIT7),(UINT8)0x00); // Enable PLL
    HDMI_DELAYMS(1);
    hdmiport_mask(HDMI_HDMI_DPCR0_VADDR, (UINT8)~_BIT0,(UINT8) (_BIT0)); 		// Reg DPLL_CMPEN
    HDMI_DELAYMS(1);
    hdmiport_mask(HDMI_HDMI_DPCR0_VADDR, (UINT8)~_BIT1, (UINT8)(_BIT1)); 		// Reg DPLL_CALLCH
    HDMI_DELAYMS(1);
    hdmiport_mask(HDMI_HDMI_DPCR0_VADDR, (UINT8)~_BIT2,(UINT8) (_BIT2)); 		// Reg DPLL_CALSW
    HDMI_DELAYMS(1);
    hdmiport_mask(HDMI_HDMI_DPCR0_VADDR, (UINT8)~(_BIT6),(UINT8)0x00); // PLL frezee

    HDMI_PRINTF(LOGGER_INFO," m = %x\n o = %x\n s = %x\n ", (UINT32)hdmi_audiopll_param[i].M, (UINT32)hdmi_audiopll_param[i].O, (UINT32)hdmi_audiopll_param[i].S);
    hdmi_out(HDMI_HDMI_SR_VADDR, hdmi_in(HDMI_HDMI_SR_VADDR));	//clr error flag

    HDMI_DELAYMS(1);
    hdmiport_mask(HDMI_HDMI_AAPNR_VADDR,(UINT8)~(_BIT1),(UINT8)_BIT1);			//enable SDM
    HDMI_DELAYMS(1);
#if 0	//adams add,for audio test
    HDMI_LOG(LOGGER_INFO, "before tracking SUMC1 = %x\n", (UINT32)hdmiport_in(HDMI_HDMI_DPCR4_VADDR));
    HDMI_LOG(LOGGER_INFO, "before tracking SUMC2 = %x\n", (UINT32)hdmiport_in(HDMI_HDMI_DPCR5_VADDR));
    hdmiport_out(HDMI_HDMI_PTRSV2_VADDR, 0x01);		// Wait for ACR : Packet Type = 0x01
    HDMI_DELAYMS(20);
    for (i=0; i<50; i++)
    {
        cts = Hdmi_PacketSRAMRead(113)<<16;
        cts = cts|Hdmi_PacketSRAMRead(114)<<8;
        cts = cts|Hdmi_PacketSRAMRead(115);
        n = Hdmi_PacketSRAMRead(116)<<16;
        n = n|Hdmi_PacketSRAMRead(117)<<8;
        n = n|Hdmi_PacketSRAMRead(118);
        HDMI_PRINTF( LOGGER_INFO,"Before N&CTS tracking enable CTS = %d  N = %d  \n",(UINT32)cts,(UINT32)n);
        HDMI_DELAYMS(20);
    }
#endif

    if (track_mode == HDMI_AUDIO_N_CTS_TREND_BOUND)
    {
//N&CTS tracking
        HDMI_PRINTF(LOGGER_INFO,"*****N/CTS Trend& Boundary Tracking*****\n");
        hdmiport_out(HDMI_HDMI_ICPSNCR0_VADDR,0x0);    //Set I code of Ncts[15:8]
        hdmiport_out(HDMI_HDMI_ICPSNCR1_VADDR,0x02);	//Set I code of Ncts [7:0]
        hdmiport_out(HDMI_HDMI_PCPSNCR0_VADDR,0x05);	//Set P code of Ncts [15:8] 	//0x02, adams modify for TSB sony player
        hdmiport_out(HDMI_HDMI_PCPSNCR1_VADDR,0xFF);	//Set P code of Ncts [7:0]
        hdmiport_out(HDMI_HDMI_PSCR_VADDR,0x72);		//Enable N&CTS tracking
        hdmiport_mask(HDMI_HDMI_CMCR_VADDR,  (UINT8)~( _BIT4), (UINT8)( _BIT4));	//Enable Double Buffer
#if 0	// audio N&CTS tracking lock time measure
        //fw_osdcore_InitProfile();		//profile time clear to 0
        //fw_osdcore_PrintProfile();	//profile...

        rtdf_pageSelect(2);
        hdmiport_out(HDMI_HDMI_NCPER_VADDR ,0x1f);	//clear phase error
        hdmiport_out(HDMI_HDMI_PETR_VADDR ,0x10);	//set phase error threshold
        HDMI_PRINTF(LOGGER_INFO,"-----Audio PLL lock wait time-----\n");
        for (i=0; i<200; i++)
        {
            hdmi_mask(HDMI_HDMI_SR_VADDR, ~_BIT3, _BIT3);	//clear unlock flag
            HDMI_DELAYMS(30);
            HDMI_LOG(LOGGER_INFO, "SUMC1 = %x\n", (UINT32)hdmiport_in(HDMI_HDMI_DPCR4_VADDR));
            HDMI_LOG(LOGGER_INFO, "SUMC2 = %x\n", (UINT32)hdmiport_in(HDMI_HDMI_DPCR5_VADDR));
            /*
            			hdmiport_mask(HDMI_HDMI_ACRCR_VADDR, (UINT8)~_BIT1,(UINT8) (_BIT1)); 		// popup N&CTS value
            			HDMI_DELAYMS(10);
            			N=(hdmiport_in(HDMI_HDMI_ACRSR2_VADDR)&0xf)<<16;
            			N=N|hdmiport_in(HDMI_HDMI_ACRSR3_VADDR)<<8;
            			N=N|hdmiport_in(HDMI_HDMI_ACRSR4_VADDR);
            			CTS=hdmiport_in(HDMI_HDMI_ACRSR0_VADDR)<<12;
            			CTS=CTS|hdmiport_in(HDMI_HDMI_ACRSR1_VADDR)<<4;
            			CTS=CTS|(hdmiport_in(HDMI_HDMI_ACRSR2_VADDR)&0xf0)>>4;
            			HDMI_PRINTF(LOGGER_INFO,"N&CTS value =%d & %d-----\n",(UINT32)N,(UINT32)CTS);
            */
            hdmiport_out(HDMI_HDMI_PTRSV2_VADDR, 0x01);		// Wait for ACR : Packet Type = 0x01
            HdmiGetStruct(&isr_info);
            b = isr_info.b;
            HDMI_DELAYMS(20);


            cts = Hdmi_PacketSRAMRead(113)<<16;
            cts = cts|Hdmi_PacketSRAMRead(114)<<8;
            cts = cts|Hdmi_PacketSRAMRead(115);
            n = Hdmi_PacketSRAMRead(116)<<16;
            n = n|Hdmi_PacketSRAMRead(117)<<8;
            n = n|Hdmi_PacketSRAMRead(118);
            HDMI_PRINTF( LOGGER_INFO,"CTS = %d  N = %d  \n",(UINT32)cts,(UINT32)n);
            HDMI_PRINTF(LOGGER_INFO, "b=%d Phase err = %x\n",(UINT32)b,(UINT32)hdmiport_in(HDMI_HDMI_NCPER_VADDR));
            HDMI_PRINTF(LOGGER_INFO,"FIFO Check  = %x\n", (UINT32)hdmi_in(HDMI_HDMI_SR_VADDR));

            if (!(hdmi_in(HDMI_HDMI_SR_VADDR) & _BIT3))
            {
                HDMI_DELAYMS(30);
                break;
            }
        }
        HDMI_PRINTF(LOGGER_INFO,"-----Audio PLL lock wait time i =%d-----\n",(UINT32)i);
        HDMI_LOG(LOGGER_INFO, "Phase error = %x\n", (UINT32)hdmiport_in(HDMI_HDMI_NCPER_VADDR));
        //fw_osdcore_PrintProfile();
        HDMI_LOG(LOGGER_INFO, "SUMC1 = %x\n", (UINT32)hdmiport_in(HDMI_HDMI_DPCR4_VADDR));
        HDMI_LOG(LOGGER_INFO, "SUMC2 = %x\n", (UINT32)hdmiport_in(HDMI_HDMI_DPCR5_VADDR));
#endif

        hdmiport_out(HDMI_HDMI_PETR_VADDR,0x1e);		//phase error threshold is 0x1e
        //hdmiport_out(HDMI_HDMI_NCPER_VADDR,0xff);		//clr phase error register

        for (i=0 ; i<30 ; i++)		//wait PLL and CTS tracking stable
        {
            hdmiport_out(HDMI_HDMI_NCPER_VADDR,0xff);		//clr phase error register
            hdmi_out(HDMI_HDMI_SR_VADDR, hdmi_in(HDMI_HDMI_SR_VADDR));	//clr error flag
            HDMI_DELAYMS(25);
            if (!(hdmi_in(HDMI_HDMI_SR_VADDR)&_BIT3))
            {
                HDMI_LOG(LOGGER_INFO, "Audio PLL is lock \n");
                HDMI_LOG(LOGGER_INFO, "Audio PLL lock count = %d \n",(UINT32)i);
                break;
            }
        }
        if (i==50)
            HDMI_LOG(LOGGER_INFO, "Audio PLL is not lock \n");

        hdmiport_out(HDMI_HDMI_FBR_VADDR,0x74);	//FIFO target depth
        hdmiport_out(HDMI_HDMI_FTR_VADDR,0x03);//target times for summation of one trend to decide the trend
        hdmiport_mask(HDMI_HDMI_AVMCR_VADDR,(UINT8)~(_BIT6),(UINT8)(_BIT6));	//enable AOC, let FSM from idle into Pre-mode
        hdmiport_mask(HDMI_HDMI_AVMCR_VADDR,(UINT8)~(_BIT5),(UINT8)(_BIT5));	//enable AOMC, let FSM from Pre-mode into next mode
        HDMI_DELAYMS(1);

//FIFO boundary and trend tracking
        hdmiport_out(HDMI_HDMI_ICTPSR0_VADDR,0x00); 		//Set I code  of trend [15:8]
        hdmiport_out(HDMI_HDMI_ICTPSR1_VADDR,0x05); 		//Set I code  of trend [7:0]
        hdmiport_out(HDMI_HDMI_PCTPSR0_VADDR,0x02); //Set P code of trend [15:8]		//0x01
        hdmiport_out(HDMI_HDMI_PCTPSR1_VADDR,0xFF); //Set P code of trend [7:0] 	   //0x01
        hdmiport_out(HDMI_HDMI_ICBPSR0_VADDR,0x00); 	 //Set I code of bnd [15:8]  //0x01
        hdmiport_out(HDMI_HDMI_ICBPSR1_VADDR,0x01); 	 //Set I code of bnd [7:0]	//0x01
        hdmiport_out(HDMI_HDMI_PCBPSR0_VADDR,0x02); //Set P code of bnd [15:8]		  //0x01
        hdmiport_out(HDMI_HDMI_PCBPSR1_VADDR,0xFF); //Set P code of bnd [7:0]		 //0x01
        hdmiport_out(HDMI_HDMI_STBPR_VADDR,0x01);//0x80 	//Set Boundary Tracking Update Response Time
        hdmiport_out(HDMI_HDMI_PSCR_VADDR,0x7e);		//Enable boundary and trend tracking
        hdmiport_mask(HDMI_HDMI_CMCR_VADDR,  (UINT8)~( _BIT4), (UINT8)( _BIT4));	//Enable Double Buffer
        //hdmiport_out(HDMI_HDMI_APDMCR_VADDR,0xC0);			//power on swallow maybe   //kist
        HDMI_DELAYMS(20);		//wait tracking stable
        hdmiport_mask(HDMI_HDMI_WDCR0_VADDR,~_BIT5,_BIT5);	//Enable trend and boundary tracking into SDM
    }

    else if (track_mode == HDMI_AUDIO_TREND_BOUND)
    {
        HDMI_PRINTF(LOGGER_INFO,"\n ***** TREND_BOUND Tracking*****\n");
        hdmiport_out(HDMI_HDMI_FBR_VADDR,0x74);	//FIFO target depth
        hdmiport_out(HDMI_HDMI_FTR_VADDR,0x03);//target times for summation of one trend to decide the trend
        hdmiport_mask(HDMI_HDMI_AVMCR_VADDR,(UINT8)~(_BIT6),(UINT8)(_BIT6));	//enable AOC, let FSM from idle into Pre-mode
        hdmiport_mask(HDMI_HDMI_AVMCR_VADDR,(UINT8)~(_BIT5),(UINT8)(_BIT5));	//enable AOMC, let FSM from Pre-mode into next mode
        HDMI_DELAYMS(1);

//FIFO boundary and trend tracking
        hdmiport_out(HDMI_HDMI_ICTPSR0_VADDR,0x00); 		//Set I code  of trend [15:8]
        hdmiport_out(HDMI_HDMI_ICTPSR1_VADDR,0x05); 		//Set I code  of trend [7:0]
        hdmiport_out(HDMI_HDMI_PCTPSR0_VADDR,0x02); //Set P code of trend [15:8]		//0x01
        hdmiport_out(HDMI_HDMI_PCTPSR1_VADDR,0xFF); //Set P code of trend [7:0] 	   //0x01
        hdmiport_out(HDMI_HDMI_ICBPSR0_VADDR,0x00); 	 //Set I code of bnd [15:8]  //0x01
        hdmiport_out(HDMI_HDMI_ICBPSR1_VADDR,0x01); 	 //Set I code of bnd [7:0]	//0x01
        hdmiport_out(HDMI_HDMI_PCBPSR0_VADDR,0x02); //Set P code of bnd [15:8]		  //0x01
        hdmiport_out(HDMI_HDMI_PCBPSR1_VADDR,0xFF); //Set P code of bnd [7:0]		 //0x01
        hdmiport_out(HDMI_HDMI_STBPR_VADDR,0x01);//0x80 	//Set Boundary Tracking Update Response Time
        hdmiport_out(HDMI_HDMI_PSCR_VADDR,0x7e);		//Enable boundary and trend tracking
        hdmiport_mask(HDMI_HDMI_CMCR_VADDR,  (UINT8)~( _BIT4), (UINT8)( _BIT4));	//Enable Double Buffer
        //hdmiport_out(HDMI_HDMI_APDMCR_VADDR,0xC0);			//power on swallow maybe   //kist
        HDMI_DELAYMS(20);		//wait tracking stable
        hdmiport_mask(HDMI_HDMI_WDCR0_VADDR,~_BIT5,_BIT5);	//Enable trend and boundary tracking into SDM
    }

    else
    {
        HDMI_PRINTF(LOGGER_INFO,"\n ***** N/CTS Tracking*****\n");
        hdmiport_out(HDMI_HDMI_ICPSNCR0_VADDR,0x0);    //Set I code of Ncts[15:8]
        hdmiport_out(HDMI_HDMI_ICPSNCR1_VADDR,0x02);	//Set I code of Ncts [7:0]
        hdmiport_out(HDMI_HDMI_PCPSNCR0_VADDR,0x02);	//Set P code of Ncts [15:8]
        hdmiport_out(HDMI_HDMI_PCPSNCR1_VADDR,0xFF);	//Set P code of Ncts [7:0]
        hdmiport_out(HDMI_HDMI_PSCR_VADDR,0x72);		//Enable N&CTS tracking
        hdmiport_mask(HDMI_HDMI_CMCR_VADDR,  (UINT8)~( _BIT4), (UINT8)( _BIT4));	//Enable Double Buffer

        hdmiport_out(HDMI_HDMI_PETR_VADDR,0x1e);		//phase error threshold is 0x1e
        hdmiport_out(HDMI_HDMI_NCPER_VADDR,0xff);		//clr phase error register

        for (i=0 ; i<30 ; i++)		//wait PLL and CTS tracking stable
        {
            hdmiport_out(HDMI_HDMI_NCPER_VADDR,0xff);		//clr phase error register
            hdmi_out(HDMI_HDMI_SR_VADDR, hdmi_in(HDMI_HDMI_SR_VADDR));	//clr error flag
            HDMI_DELAYMS(25);
            if (!(hdmi_in(HDMI_HDMI_SR_VADDR)&_BIT3))
            {
                HDMI_LOG(LOGGER_INFO, "Audio PLL is lock \n");
                HDMI_LOG(LOGGER_INFO, "Audio PLL lock count = %d \n",(UINT32)i);
                break;
            }
        }
        if (i==50)
            HDMI_LOG(LOGGER_INFO, "Audio PLL is not lock \n");

        hdmiport_out(HDMI_HDMI_FBR_VADDR,0x74);	//FIFO target depth
        hdmiport_out(HDMI_HDMI_FTR_VADDR,0x03);//target times for summation of one trend to decide the trend
        hdmiport_mask(HDMI_HDMI_AVMCR_VADDR,(UINT8)~(_BIT6),(UINT8)(_BIT6));	//enable AOC, let FSM from idle into Pre-mode
        hdmiport_mask(HDMI_HDMI_AVMCR_VADDR,(UINT8)~(_BIT5),(UINT8)(_BIT5));	//enable AOMC, let FSM from Pre-mode into next mode
        HDMI_DELAYMS(1);
    }

    //Hdmi_VideoOutputEnable();	//why is in here?

    HDMI_LOG(LOGGER_INFO, "HDMI_HDMI_SR_VADDR = %x\n", (UINT32)hdmi_in(HDMI_HDMI_SR_VADDR));
    //HDMI_LOG(LOGGER_INFO, "HDMI_HDMI_AFCR_VADDR = %x\n", (UINT32)hdmiport_in(HDMI_HDMI_AFCR_VADDR));
    //HDMI_PRINTF(LOGGER_INFO, "HDMI_HDMI_AVMCR_VADDR = %x\n", (UINT32)hdmiport_in(HDMI_HDMI_AVMCR_VADDR));

    return TRUE;
}
#endif

typedef struct
{
    UINT8 SM;
    UINT8 SN;
    UINT8 RatioM;
    UINT8 RatioN;
} VIDEO_DPLL_RATIO_T;

HDMI_CONST VIDEO_DPLL_RATIO_T dpll_ratio[] =
{

    { 	15, 15, 	2, 2	},	// 24 bits
    { 	12, 15, 	4, 5	},	// 30 bits
    { 	10, 15, 	2, 3	},	// 36 bits
    { 	15, 30, 	1, 2	},	// 48 bits
};

/**
 * Hdmi_VedioPLLSetting
 * setting Vedio PLL for interlace video or deep color vedio
 *
 * @param <Enable2X>			{ 1 for interlaced video ;0 for progressive }
 * @return 					{ }
 * @ingroup drv_hdmi
 */


BOOL Hdmi_VedioPLLSetting(int Enable2X)
{

//  Fin =   Fxtal * 1024 / b
//  Target vco = ( Fin * m / n )     , TagretVco_HB = 400 ,  TagretVco_LB=200
//  Fin * M / N / 2^o / 2 * s = Fout = Fin * [24/30, 24/36, 24/48] ,  [10bits, 12bits,16bits]
//  200 <  ( Fin * m / n )  < 400  -->  200 <   Fin * 2^o * s * [ 8/5 , 4/3, 1 ]   < 400
//  Scode_LB =  200 * 15 * b  / ( Fxtal *1024 * [24,20,15] )
//  Scode_HB =  400 * 15 * b  / ( Fxtal *1024 * [24,20,15] )
//  Smean = (Scode_LB +Scode_HB ) /2
//  M/N = [8/5 , 4/3 , 1 ] * S

//	UINT32 large_ratio,cd, Scode_LB, Scode_HB, Smean, m, n, o, fraction1, fraction2,pixel_clock;
    UINT32 large_ratio,cd, Smean, m, n, o, fraction1, fraction2,pixel_clock;
    UINT8 i;
    HDMIRX_IOCTL_STRUCT_T isr_info;

    if (Enable2X) large_ratio = 2;
    else large_ratio = 1;

    // Switch to Page 2 referenced from spec
    rtdf_pageSelect(2);

    cd = (hdmi_in(HDMI_TMDS_DPC0_VADDR) & (0x0f));
    SET_HDMI_CD(cd);

    HdmiGetStruct(&isr_info);

    hdmi.b = isr_info.b;


    HDMI_PRINTF(LOGGER_INFO,"b = %x\n", (UINT32)hdmi.b);
    HDMI_PRINTF(LOGGER_INFO,"cd = %x\n", (UINT32)cd);
    HDMI_PRINTF(LOGGER_INFO,"Enable2X = %x\n", (UINT32)Enable2X);
    if (hdmi.b  < 80 || hdmi.b > 2100)
        return FALSE;

    //if (cd < 4) return;
    //cd = cd - 4;


    if  ( cd >= 0x04 && cd < 0x08)
    {
        cd = cd - 0x04;
        if (cd != 0)
        {
            hdmi_out(HDMI_TMDS_UDC0_VADDR, 0x80);   // deep color mode
        }
        else
        {
            hdmi_out(HDMI_TMDS_UDC0_VADDR, 0x00);	// non-deep color mode
        }



    }
    else
    {
        cd  = 0;
        hdmi_out(HDMI_TMDS_UDC0_VADDR, 0x00);

    }

#if 1
    pixel_clock = (UINT32)hdmi.b*(UINT32) dpll_ratio[cd].RatioN/(UINT32) dpll_ratio[cd].RatioM;
    for (i=0; i<5; i++)
    {
        if (hdmi_videopll_param[i].b_upper < pixel_clock && hdmi_videopll_param[i].b_lower >= pixel_clock)
        {
            o = hdmi_videopll_param[i].O;
            Smean = hdmi_videopll_param[i].S;
            n = dpll_ratio[cd].RatioN;
            m = (dpll_ratio[cd].RatioM*Smean*2)<<o;
        }

    }
    HDMI_PRINTF(LOGGER_INFO,"pixel_clk = %x\n", (UINT32)pixel_clock);
#else
    Scode_LB = 200 * 128 *((UINT32) dpll_ratio[cd].SN) * hdmi.b / (27) /1024 / ((UINT32) dpll_ratio[cd].SM)/large_ratio/2 ;
//	Scode_HB = 400 * 128 *((UINT32) dpll_ratio[cd].SN) * hdmi.b / (27) /1024 / ((UINT32) dpll_ratio[cd].SM)/large_ratio/2;
    Scode_HB = 2*Scode_LB;
    Smean = ((Scode_LB + Scode_HB) + 64) / 2 / 128;

    if (Smean == 0)
    {
        m = dpll_ratio[cd].SM*large_ratio;
        n = dpll_ratio[cd].SN;
        o = 0;
    }
    else
    {
        n = 0;
        o = 0;
        do
        {
            n += dpll_ratio[cd].RatioN;
            if (n > 15)
            {
                o=o+1 ;
                n=0;
            }
            m =(( dpll_ratio[cd].RatioM * Smean * n * large_ratio * 2)<<o) / dpll_ratio[cd].RatioN ;

        }
        while ((m<40) ||( n < 2));
    }



    if ((m==48) && (o==1))
    {
        m = 24;
        o = 0;
    }
#endif
#if 0
    fraction1 = ((unsigned long)m *4 * 100 / 1067) ;    // 2bit fractional part
    fraction2 = 0x00;

    if (fraction1 >=10)
        fraction1 -= 10;

    if (fraction1 >= 40)       // 2bit fractional part
    {
        fraction1 -= 40;
        fraction2 |= 0x04;
    }

    if (fraction1 >= 20)       // 2bit fractional part
    {
        fraction1 -= 20;
        fraction2 |= 0x02;
    }

    if (fraction1 >= 10)       // 2bit fractional part
    {
        fraction1 -= 10;
        fraction2 |= 0x01;
    }
    fraction2|= 0x70;
#else
    fraction1 = ((unsigned long)m *4 * 100 / 511) ;    // 2bit fractional part
    fraction2 = 0x00;

    if (((fraction1&0x01) == 0x00)||(fraction1 >= 140))
        fraction2 |= 0x20;
    else
        fraction1 = fraction1 << 1;


    if (fraction1 >=10)
        fraction1 -= 10;

    if (fraction1 >= 120)      // 2bit fractional part
    {
        fraction1 -= 120;
        fraction2 |= 0x10;
    }

    if (fraction1 >= 80)       // 2bit fractional part
    {
        fraction1 -= 80;
        fraction2 |= 0x08;
    }

    if (fraction1 >= 40)       // 2bit fractional part
    {
        fraction1 -= 40;
        fraction2 |= 0x04;
    }

    if (fraction1 >= 20)       // 2bit fractional part
    {
        fraction1 -= 20;
        fraction2 |= 0x02;
    }

    if (fraction1 >= 10)       // 2bit fractional part
    {
        fraction1 -= 10;
        fraction2 |= 0x01;
    }
    fraction2 |= 0x80;
#endif
    //HDMI_PRINTF(LOGGER_INFO,"***************Supper=%d\n", Scode_HB);
    //HDMI_PRINTF(LOGGER_INFO,"***************Slower=%d\n",Scode_LB);
    //HDMI_PRINTF(LOGGER_INFO,"***************m=%d\n",m);
    //HDMI_PRINTF(LOGGER_INFO,"***************n=%d\n",n);
    //HDMI_PRINTF(LOGGER_INFO,"***************o=%d\n",o);
    //HDMI_PRINTF(LOGGER_INFO,"***************s=%d\n",Smean);
    //HDMI_PRINTF(LOGGER_INFO,"***************fraction1=%d\n",fraction1);
    //HDMI_PRINTF(LOGGER_INFO,"***************fraction2=%d\n",fraction2);

    dp_outl(0x55, 0);
    dp_outl(0x54, 0);
    dp_outl(0x53, 0x00);
    dp_outl(0x53, 0x80);
    dp_outl(0x53, 0x00);
    dp_outl(0x67, 0x10);
    dp_outl(0x51,(m-2));
    dp_outl(0x52,Smean);
    dp_outl(0x70,o);
    dp_outl(0x71,fraction2); // Set LFR , Ich
    dp_outl(0x72, ((dp_inl(0x72))&((~_BIT0 | _BIT1))));  // Set LPF mode0
    dp_outl(0x50, (UINT8) _BIT7 | _BIT4|(n-2));
    // Power On --> Off PLL
    dp_outl(0x70,dp_inl(0x70) | 0x80);
    HDMI_DELAYMS(1);
    dp_outl(0x70,dp_inl(0x70) & (~0x80));
    HDMI_DELAYMS(5);
    return TRUE;
}


void HDMITimingCompensation(void)
{

//	UINT8  i = 0;

    if (drvif_Hdmi_GetInterlace())
    {
        if (GET_HDMI_DEONLY_MODE())
        {
            if ((GET_MODE_IVTOTAL()& 0x01) == 1)  	//  ODD field
            {
                SET_MODE_IVSTA(GET_MODE_IVSTA() - 1);
                SET_MODE_IVTOTAL(GET_MODE_IVTOTAL() - 1);
                SET_MODE_IVCOUNT(GET_MODE_IVCOUNT() - 1);
            }
        }
        //For Sparrow only, enable video compensate will case the last line disappear
        SET_MODE_IVHEIGHT(GET_MODE_IVHEIGHT()+1);
    }

    // Horizontal active width must be even
    if (GET_MODE_IHWIDTH() & 0x01) SET_MODE_IHWIDTH(GET_MODE_IHWIDTH() - 1);
#if 0
    while (hdmi_timing_compensate[i].IVTotal != 0)
    {

        if ((ABS(hdmi_timing_compensate[i].IVTotal, GET_MODE_IVTOTAL()) < 5) &&
                (ABS(hdmi_timing_compensate[i].IHTotal, GET_MODE_IHTOTAL()) < 5) &&
                (ABS(hdmi_timing_compensate[i].IVFreq, GET_MODE_IVFREQ()) < 5))
        {

            SET_MODE_IVHEIGHT(GET_MODE_IVHEIGHT() + hdmi_timing_compensate[i].IVHEIGHT_ADJ);
            SET_MODE_IVTOTAL(GET_MODE_IVTOTAL() + hdmi_timing_compensate[i].IVHEIGHT_ADJ);
            SET_MODE_IHWIDTH(GET_MODE_IHWIDTH() + hdmi_timing_compensate[i].IHWIDTH_ADJ);
            SET_MODE_IHTOTAL(GET_MODE_IHTOTAL() + hdmi_timing_compensate[i].IHWIDTH_ADJ);
            SET_MODE_IVSTA(GET_MODE_IVSTA() + hdmi_timing_compensate[i].IVSTA_ADJ);
            SET_MODE_IHSTA(GET_MODE_IHSTA() + hdmi_timing_compensate[i].IHSTA_ADJ);

            break;
        }

    }
#endif
    HDMI_PRINTF(LOGGER_INFO,"After Timing Compensation \n");
    HDMI_PRINTF(LOGGER_INFO,"IVTotal: %d\n", (UINT32)GET_MODE_IVTOTAL());
    HDMI_PRINTF(LOGGER_INFO,"IHTotal: %d\n", (UINT32)GET_MODE_IHTOTAL());
    HDMI_PRINTF(LOGGER_INFO,"IHWidth:%d\n", (UINT32)GET_MODE_IHWIDTH());
    HDMI_PRINTF(LOGGER_INFO,"IVHeight:%d\n", (UINT32)GET_MODE_IVHEIGHT());
    HDMI_PRINTF(LOGGER_INFO,"IHCount:%d\n", (UINT32)GET_MODE_IHTOTAL());
    HDMI_PRINTF(LOGGER_INFO,"IVCount:%d\n", (UINT32)GET_MODE_IVTOTAL());
    HDMI_PRINTF(LOGGER_INFO,"IHStart:%d\n", (UINT32)GET_MODE_IHSTA());
    HDMI_PRINTF(LOGGER_INFO,"IVStart:%d\n", (UINT32)GET_MODE_IVSTA());
    HDMI_PRINTF(LOGGER_INFO,"IHFreq:%d\n", (UINT32)GET_MODE_IHFREQ());
    HDMI_PRINTF(LOGGER_INFO,"IVFreq:%d\n", (UINT32)GET_MODE_IVFREQ());
    HDMI_PRINTF(LOGGER_INFO,"Mode ID: %d\n", (UINT32)GET_MODE_CURR());

}

HDMI_BOOL Hdmi_FixedPolarity(void)
{

    char timeoutcnt;

    //clear measure error status
    rtd_outl(GET_SP_MS3STUS_VADDR(), 0xff);

    //  set offline measure source
    rtd_maskl(GET_SP_MS3CTRL_VADDR() , ~(SP_MS3CTRL_off_sycms_mode_mask | SP_MS3CTRL_off_meas_source_mask |SP_MS3CTRL_off_start_ms_mask)
              ,SP_MS3CTRL_off_meas_source(_OFFLINE_MS_SRC_HDMI));


    //set measure mode & start measure
    rtd_maskl(GET_SP_MS3CTRL_VADDR(), ~(SP_MS3CTRL_off_sycms_mode_mask | SP_MS3CTRL_off_online_en_mask | SP_MS3CTRL_off_start_ms_mask)
              , SP_MS3CTRL_off_sycms_mode(ANALOG_MODE_MEASUREMENT) | SP_MS3CTRL_off_start_ms(1) | SP_MS3CTRL_off_online_en(0));

    /*

    	Measure Wait Trigger Done

    */
    timeoutcnt = 10;

    do
    {
        HDMI_DELAYMS(10);

        if (rtd_inl(GET_SP_MS3CTRL_VADDR()) & SP_MS3CTRL_off_start_ms_mask) continue;
        break;
    }
    while (--timeoutcnt);


    if (timeoutcnt == 0) return FALSE;

    HDMI_PRINTF(LOGGER_INFO,"=== ulResult:%x ====\n", rtd_inl(GET_SP_MS3STUS_VADDR()));

    switch ((rtd_inl(GET_SP_MS3STUS_VADDR()) >> 8)& 0x03)
    {

    case 0x3:
        SET_MODE_POLARITY(_SYNC_HP_VP);
        rtd_maskl(GET_VGIP_CHNx_CTRL_VADDR(), ~(VGIP_CHN1_CTRL_ch1_vs_inv_mask | VGIP_CHN1_CTRL_ch1_hs_inv_mask ), 0); // Let HS negtive polarity to positive polarity
        HDMI_PRINTF(LOGGER_INFO,"=== HS, VS positive ====\n");
        break;
    case 0x1:
        SET_MODE_POLARITY(_SYNC_HN_VP);
        rtd_maskl(GET_VGIP_CHNx_CTRL_VADDR(), ~(VGIP_CHN1_CTRL_ch1_hs_inv_mask | VGIP_CHN1_CTRL_ch1_hs_inv_mask), (VGIP_CHN1_CTRL_ch1_hs_inv_mask)); // Let HS negtive polarity to positive polarity
        HDMI_PRINTF(LOGGER_INFO,"=== HS negtive ====\n");
        break;
    case 0x2:

        SET_MODE_POLARITY(_SYNC_HP_VN);
        rtd_maskl(GET_VGIP_CHNx_CTRL_VADDR(), ~(VGIP_CHN1_CTRL_ch1_vs_inv_mask | VGIP_CHN1_CTRL_ch1_hs_inv_mask), (VGIP_CHN1_CTRL_ch1_vs_inv_mask)); // Let HS negtive polarity to positive polarity
        HDMI_PRINTF(LOGGER_INFO,"=== VS negtive ====\n");
        break;
    case 0x0:
        SET_MODE_POLARITY(_SYNC_HN_VN);
        rtd_maskl(GET_VGIP_CHNx_CTRL_VADDR(), ~(VGIP_CHN1_CTRL_ch1_vs_inv_mask | VGIP_CHN1_CTRL_ch1_hs_inv_mask ), (VGIP_CHN1_CTRL_ch1_vs_inv_mask | VGIP_CHN1_CTRL_ch1_hs_inv_mask )); // Let HS negtive polarity to positive polarity
        HDMI_PRINTF(LOGGER_INFO,"=== HS, VS negative ====\n");
        break;

    }

    return TRUE;
}
HDMI_BOOL Hdmi_MeasureTiming(void)
{


    char timeoutcnt;
//	char polarity_fixed = 0;
    UINT8 cd;
    UINT32 Q, D;


    if (hdmi.b == 0) return FALSE;
    /*

    	Measure Setup

    */
//adams move to hdmi_measure() ,20110419
    /*
    	if(GET_HDMI_DEONLY_MODE() == 0)
    		Hdmi_FixedPolarity();
    	else
    	// Set VGIP Sync Polarity to disable
    		rtd_maskl(GET_VGIP_CHNx_CTRL_VADDR(), ~(VGIP_CHN1_CTRL_ch1_vs_inv_mask|VGIP_CHN1_CTRL_ch1_hs_inv_mask), 0);
    */
    HDMI_PRINTF(LOGGER_INFO,"GET_VGIP_CHNx_CTRL_VADDR = %x\n", rtd_inl(GET_VGIP_CHNx_CTRL_VADDR()));
    // Disable SRC_Clock_Enbale bit
    rtd_maskl(GET_VGIP_CHNx_CTRL_VADDR(), ~_BIT0,0x0);

    // 2nd determine the input source TMDS(0), DIGITAL mode(1)
    rtd_maskl(GET_VGIP_CHNx_CTRL_VADDR(), 0x0fffffff, (0 <<28) | (_BIT1));

    // 3rd enable SRC_Clock_Enbale bit and sample enable
    rtd_maskl(GET_VGIP_CHNx_CTRL_VADDR(), ~(_BIT31|_BIT16 | _BIT18 | _BIT22 | _BIT21 | _BIT0), (_BIT31|_BIT0));

    // Set IVS sync with IHS control befor smart-Fit measuring
    rtd_maskl(GET_VGIP_CHNx_CTRL_VADDR(), ~(VGIP_CHN1_CTRL_ch1_vs_syncedge_mask | VGIP_CHN1_CTRL_ch1_vs_by_hs_en_n_mask),	VGIP_CHN1_CTRL_ch1_vs_syncedge(1));
    // setup VGIP capture for smartfit measure
    rtd_outl(GET_VGIP_CHNx_V_CAP_VADDR(),  (0 << 16) | (120));
    rtd_outl(GET_VGIP_CHNx_H_CAP_VADDR(),  (0 << 16) | (240));

    //  set offline measure source
    rtd_maskl(GET_SP_MS3CTRL_VADDR() , ~(SP_MS3CTRL_off_sycms_mode_mask | SP_MS3CTRL_off_meas_source_mask |SP_MS3CTRL_off_start_ms_mask)
              ,SP_MS3CTRL_off_meas_source(_OFFLINE_MS_SRC_HDMI));

    // set over range threshold (VSYNC_MAX_DELTA = 2-HSync, HSYNC_MAX_DELTA = 4-clock)
    rtd_maskl(GET_SP_MSxCTRL_VADDR(), ~(SP_MS1CTRL_on1_start_ms_mask|SP_MS1CTRL_on1_hs_max_delta_mask|SP_MS1CTRL_on1_vs_max_delta_mask),
              ((SP_MS1CTRL_on1_hs_max_delta(6))|SP_MS1CTRL_on1_vs_max_delta(2)));


    // disable smartfit auto field selection
    rtd_maskl(GET_SFT_AUTO_FIELD_VADDR(), ~(_BIT7 | _BIT6) , 0);
    rtd_maskl(GET_SFT_AUTO_FIELD_VADDR(), ~_BIT5 , GET_SCALER_CHANNEL()  ? _BIT5:0);

    // set Smartfit larger active region
    rtd_outl(GET_SFT_AUTO_H_BOUNDARY_VADDR(), (0 << 16) | 3000);
    rtd_outl(GET_SFT_AUTO_V_BOUNDARY_VADDR(), (0 << 16) | 1080);

    //clear measure error status
    rtd_outl(GET_SP_MS3STUS_VADDR(), 0xff);
    rtd_outl(GET_SP_MSxSTUS_VADDR(), 0xff);

    /*

    	Measure Trigger

    */

    rtd_maskl(GET_SP_MSxCTRL_VADDR(),~(SP_MS1CTRL_on1_sycms_clk_mask | SP_MS1CTRL_on1_online_en_mask| SP_MS1CTRL_on1_start_ms_mask | SP_MS1CTRL_on1_sycms_mode_mask),
              SP_MS1CTRL_on1_sycms_clk(1) | SP_MS1CTRL_on1_sycms_mode(ANALOG_MODE_MEASUREMENT) | SP_MS1CTRL_on1_online_en(0)  | SP_MS1CTRL_on1_start_ms(1));

    //enable smart fit
    rtd_outl(GET_SFT_AUTO_ADJ_VADDR() , _BIT7 | _BIT0);

    //set measure mode & start measure
    rtd_maskl(GET_SP_MS3CTRL_VADDR(), ~(SP_MS3CTRL_off_sycms_mode_mask | SP_MS3CTRL_off_online_en_mask | SP_MS3CTRL_off_start_ms_mask)
              , SP_MS3CTRL_off_sycms_mode(DIGITAL_MODE_MEASUREMENT) | SP_MS3CTRL_off_start_ms(1) | SP_MS3CTRL_off_online_en(0));



    /*

    	Measure Wait Trigger Done

    */
    timeoutcnt = 10;

    do
    {
        HDMI_DELAYMS(10);

        if (rtd_inl(GET_SP_MSxCTRL_VADDR()) & SP_MS1CTRL_on1_start_ms_mask) continue;
        if (rtd_inl(GET_SP_MS3CTRL_VADDR()) & SP_MS3CTRL_off_start_ms_mask) continue;
        if (rtd_inl(GET_SFT_AUTO_ADJ_VADDR()) & _BIT0) continue;
        break;
    }
    while (--timeoutcnt);

    if (timeoutcnt == 0x00)
    {
        HDMI_PRINTF(LOGGER_INFO,"Measure Timeout\n");
        return FALSE;
    }

    if (rtd_inl(GET_SP_MS3STUS_VADDR())
            & ~(SP_MS3STUS_off_vs_pol_chg_mask | SP_MS3STUS_off_hs_pol_chg_mask | SP_MS3STUS_off_hs_per_of_mask | SP_MS3STUS_off_vs_high_to_mask) & 0xFF)
    {
        HDMI_PRINTF(LOGGER_INFO,"Error MS3STUS: %x\n", rtd_inl(GET_SP_MS3STUS_VADDR()));
        return FALSE;
    }
    if (rtd_inl(GET_SP_MSxSTUS_VADDR())
            & ~(SP_MS2STUS_on2_vs_pol_chg_mask | SP_MS2STUS_on2_hs_pol_chg_mask |  SP_MS2STUS_on2_hs_per_of_mask | SP_MS2STUS_on2_vs_high_to_mask) & 0xFF)
    {
        HDMI_PRINTF(LOGGER_INFO,"Error MSxSTUS: %x\n", rtd_inl(GET_SP_MSxSTUS_VADDR()));
        return FALSE;
    }



    /*

    	Measure Wait Result Popup

    */

    timeoutcnt = 10;

    // pop-up period measure result
    rtd_maskl(GET_SP_MSxCTRL_VADDR(), ~SP_MS2CTRL_on2_popup_meas_mask, SP_MS2CTRL_on2_popup_meas(1));
    rtd_maskl(GET_SP_MS3CTRL_VADDR(), ~SP_MS3CTRL_off_popup_meas_mask, SP_MS3CTRL_off_popup_meas(1));

    do
    {
        HDMI_DELAYMS(10);

        if (rtd_inl(GET_SP_MSxCTRL_VADDR())& SP_MS2CTRL_on2_popup_meas_mask) continue;
        if (rtd_inl(GET_SP_MS3CTRL_VADDR()) & SP_MS3CTRL_off_popup_meas_mask) continue;
        break;

    }
    while (--timeoutcnt);

    if (timeoutcnt == 0x00)
    {
        HDMI_PRINTF(LOGGER_INFO,"Popup Timeout\n");
        return FALSE;
    }

    /*

    	Measure Result  Read

    */

    SET_MODE_IVSTA((rtd_inl(GET_SFT_AUTO_RESULT_VSTA_END_VADDR()) >> 16) & 0xFFF);
    SET_MODE_IVSTA(GET_MODE_IVSTA());
    SET_MODE_IHSTA((rtd_inl(GET_SFT_AUTO_RESULT_HSTA_END_VADDR()) >> 16) & 0xFFF);
    SET_MODE_IHSTA(GET_MODE_IHSTA() - 2);
    SET_MODE_IVHEIGHT(SP_MS3RST0_get_off_vs_period_out(rtd_inl(GET_SP_MS3RST0_VADDR()))+1);      // Get Input VSYNC Period Measurement Result ;match resolution(cause measure result will lose one line)
    SET_MODE_IHWIDTH(SP_MS3RST1_get_off_hs_high_out(rtd_inl(GET_SP_MS3RST1_VADDR()))+1);  // Input HSYNC High Period Measurement Result; match resolution(cause measure result will lose one line)
    SET_MODE_IVTOTAL(SP_MS1RST0_get_on1_vs_period_out(rtd_inl(GET_SP_MSxRST0_VADDR()))+ 1);
    SET_MODE_IHTOTAL(SP_MS1RST0_get_on1_hs_period_out_h12b(rtd_inl(GET_SP_MSxRST0_VADDR()))+1);
    SET_MODE_IHCOUNT(GET_MODE_IHTOTAL());
    SET_MODE_IVCOUNT(GET_MODE_IVTOTAL());


    if ((GET_MODE_IHWIDTH() == 0) || (GET_MODE_IVHEIGHT() == 0) || (GET_MODE_IVTOTAL() == 0) || (GET_MODE_IHTOTAL() == 0))//1221 kist
    {
        HDMI_PRINTF(LOGGER_INFO,"\n(stModeInfo.IHWidth == %x) || (stModeInfo.IVHeight == %x)\n",GET_MODE_IHWIDTH(),GET_MODE_IVHEIGHT());
        HDMI_PRINTF(LOGGER_INFO,"\n(stModeInfo.IHTotal == %x) || (stModeInfo.IVTotal== %x)\n", GET_MODE_IHTOTAL(), GET_MODE_IVTOTAL());
        return FALSE;
    }


    /*

    	To calculate IVFreq

    */
    cd = GET_HDMI_CD();

    if  ( cd >= 0x04 && cd < 0x08)
    {
        cd = cd - 0x04;
    }
    else
    {
        cd  = 0;
    }
    // implement Q = (27000000 * RatioM) / RatioN / hdmi.b
    Q = ((27000000UL * dpll_ratio[cd].RatioM) + (dpll_ratio[cd].RatioN >> 1)) / dpll_ratio[cd].RatioN;
    Q = (Q * 8 + (hdmi.b>>1)) / hdmi.b;
    D = ((hdmiport_in(HDMI_HDMI_VCR_VADDR) & 0x0f) + 1) * GET_MODE_IHTOTAL();
    Q =   ((Q + (D >> 1)) / D) * (1024/8); // here Q = video clock / ((hdmiport_in(HDMI_HDMI_VCR_VADDR) & 0x0f) + 1) * pModeInfo->IHTotal

    SET_MODE_IHFREQ((Q + 50) / 100);
    SET_MODE_IVFREQ((Q * 10 + (GET_MODE_IVTOTAL()>> 1)) / GET_MODE_IVTOTAL());




    HDMI_PRINTF(LOGGER_INFO,"IVTotal: %d\n", (UINT32)GET_MODE_IVTOTAL());
    HDMI_PRINTF(LOGGER_INFO,"IHTotal: %d\n", (UINT32)GET_MODE_IHTOTAL());
    HDMI_PRINTF(LOGGER_INFO,"IHWidth:%d\n", (UINT32)GET_MODE_IHWIDTH());
    HDMI_PRINTF(LOGGER_INFO,"IVHeight:%d\n", (UINT32)GET_MODE_IVHEIGHT());
    HDMI_PRINTF(LOGGER_INFO,"IHCount:%d\n", (UINT32)GET_MODE_IHTOTAL());
    HDMI_PRINTF(LOGGER_INFO,"IVCount:%d\n", (UINT32)GET_MODE_IVTOTAL());
    HDMI_PRINTF(LOGGER_INFO,"IVStart:%d\n", (UINT32)GET_MODE_IVSTA());
    HDMI_PRINTF(LOGGER_INFO,"IHStart:%d\n", (UINT32)GET_MODE_IHSTA());
    HDMI_PRINTF(LOGGER_INFO,"IHFreq:%d\n", (UINT32)GET_MODE_IHFREQ());
    HDMI_PRINTF(LOGGER_INFO,"IVFreq:%d\n", (UINT32)GET_MODE_IVFREQ());



    // switch online measure to analog mode for next step
//	hdmi_mask(GET_SP_MSxCTRL_VADDR(),~(SP_MS1CTRL_on1_sycms_clk_mask | SP_MS1CTRL_on1_online_en_mask	| SP_MS1CTRL_on1_start_ms_mask | SP_MS1CTRL_on1_sycms_mode_mask),
//			SP_MS1CTRL_on1_sycms_clk(1) | SP_MS1CTRL_on1_sycms_mode(0) | SP_MS1CTRL_on1_online_en(0)  | SP_MS1CTRL_on1_start_ms(0));
    return TRUE;

}

void HDMISetScalerInfo(void)
{

    SET_SCALER_POLARITY(GET_MODE_POLARITY());
    SET_SCALER_IVTOTAL(GET_MODE_IVTOTAL());
    SET_SCALER_IHTOTAL(GET_MODE_IHTOTAL());
    SET_SCALER_IVHEIGHT(GET_MODE_IVHEIGHT());
    SET_SCALER_IVHEIGHT_PRE(GET_MODE_IVHEIGHT());
    SET_SCALER_IHWIDTH(GET_MODE_IHWIDTH());
    SET_SCALER_IHWIDTH_PRE(GET_MODE_IHWIDTH());
    SET_SCALER_IVSTA(GET_MODE_IVSTA());
    SET_SCALER_IVSTA_PRE(GET_MODE_IVSTA());
    SET_SCALER_IHSTA(GET_MODE_IHSTA());
    SET_SCALER_IHSTA_PRE(GET_MODE_IHSTA());
    SET_SCALER_IVFREQ(GET_MODE_IVFREQ())	;
    SET_SCALER_IHFREQ(GET_MODE_IHFREQ());
    SET_SCALER_IVCOUNT(GET_MODE_IVCOUNT());
    SET_SCALER_IHCOUNT(GET_MODE_IHCOUNT());
    SET_SCALER_ISINTERLACE(GET_HDMI_ISINTERLACE())
    SET_SCALER_IS422(GET_HDMI_COLOR_SPACE() == COLOR_YUV422);

}

#if 0
HDMI_BOOL Hdmi_OnlineMeasure2(void)
{


    UINT32 ulResult1, timeoutcnt, IVTotal, IHTotal,vsynctimeoutcnt=0;

    // set over range threshold (VSYNC_MAX_DELTA = 2-HSync, HSYNC_MAX_DELTA = 4-clock)
HDMIOnlineMeasureStart:
    rtd_maskl(GET_SP_MSxCTRL_VADDR(),
              ~(SP_MS1CTRL_on1_start_ms_mask|SP_MS1CTRL_on1_hs_max_delta_mask|SP_MS1CTRL_on1_vs_max_delta_mask),
              ((SP_MS1CTRL_on1_hs_max_delta(6))|SP_MS1CTRL_on1_vs_max_delta(2)));

    rtd_outl(GET_SP_MSxSTUS_VADDR(), 0xff);

    rtd_maskl(GET_SP_MSxCTRL_VADDR(),
              ~(SP_MS1CTRL_on1_sycms_clk_mask | SP_MS1CTRL_on1_online_en_mask	| SP_MS1CTRL_on1_start_ms_mask | SP_MS1CTRL_on1_sycms_mode_mask),
              SP_MS1CTRL_on1_sycms_clk(1) | SP_MS1CTRL_on1_sycms_mode(ANALOG_MODE_MEASUREMENT) | SP_MS1CTRL_on1_online_en(0)  | SP_MS1CTRL_on1_start_ms(1)); // enable online measure

    // Measure Start
    timeoutcnt = 10;

    do
    {
        HDMI_DELAYMS(10);

        ulResult1 = rtd_inl(GET_SP_MSxCTRL_VADDR());

        if (((ulResult1 &  SP_MS1CTRL_on1_start_ms_mask) == 0)) // polling status(Popup Measure Result) bit
            break;
    }
    while (--timeoutcnt);
    if (timeoutcnt == 0x00)
    {
        HDMI_PRINTF(LOGGER_INFO,"OnlineMeasure2:Measure Timeout\n");
        return FALSE;
    }

    if (rtd_inl(GET_SP_MSxSTUS_VADDR()) & ~(SP_MS2STUS_on2_vs_pol_chg_mask | SP_MS2STUS_on2_hs_pol_chg_mask | 0x44) & 0xFF)
    {
        HDMI_PRINTF(LOGGER_INFO,"Measure Error : %x \n",rtd_inl(GET_SP_MSxSTUS_VADDR()));
        if (rtd_inl(GET_SP_MSxSTUS_VADDR()) & (SP_MS2STUS_on2_vs_per_to_mask) )
        {
            vsynctimeoutcnt++;
            if (vsynctimeoutcnt<2)	goto HDMIOnlineMeasureStart;
        }
        return FALSE;
    }
    //pop up result
    timeoutcnt = 10;

    rtd_maskl(GET_SP_MSxCTRL_VADDR(), ~SP_MS1CTRL_on1_popup_meas_mask, SP_MS1CTRL_on1_popup_meas(1));

    if (GET_MODE_IHWIDTH() < 120) return _MODE_NOSUPPORT;

    do
    {
        ulResult1 = rtd_inl(GET_SP_MSxCTRL_VADDR());


        if (((ulResult1 &  SP_MS2CTRL_on2_popup_meas_mask) == 0)) // polling status(Popup Measure Result) bit
            break;


        HDMI_DELAYMS(10);

    }
    while (--timeoutcnt);

    if (timeoutcnt == 0x00)
    {
        HDMI_PRINTF(LOGGER_INFO,"Popup Timeout\n");
        return FALSE;
    }

    IHTotal = SP_MS1RST0_get_on1_hs_period_out_h12b(rtd_inl(GET_SP_MSxRST0_VADDR()));
    IVTotal = SP_MS1RST0_get_on1_vs_period_out(rtd_inl(GET_SP_MSxRST0_VADDR()));

    if ((ABS(IHTotal,  GET_SCALER_IHTOTAL())>10) || (ABS(IVTotal, GET_SCALER_IVTOTAL())>10))
    {
        HDMI_PRINTF(LOGGER_INFO,"Timing change\n");
        return FALSE;
    }
    return TRUE;

}
#endif
UINT8 Hdmi_Measure(void)
{


    UINT16 IHTotal_index, IVTotal_index;
    UINT32 tmds_clk;



    if (GET_HDMI_DEONLY_MODE() == 0)
    {
        if ( !Hdmi_FixedPolarity())//get HS&VS polarity
            return	_MODE_NOSUPPORT;
    }
    else
        // Set VGIP Sync Polarity to disable
        rtd_maskl(GET_VGIP_CHNx_CTRL_VADDR(), ~(VGIP_CHN1_CTRL_ch1_vs_inv_mask|VGIP_CHN1_CTRL_ch1_hs_inv_mask), 0);


    if (Hdmi_MeasureTiming() == FALSE)
    {
        HDMI_PRINTF(LOGGER_INFO,"Hdmi_MeasureTiming Error\n");
        return	_MODE_NOSUPPORT;
    }
    /*	//adams mask for decrease time,20110415
    	if (Hdmi_MeasureTiming() == FALSE) {
    				HDMI_PRINTF(LOGGER_INFO,"Hdmi_MeasureTiming Error\n");
    		return	_MODE_NOSUPPORT;
    	}
    */

    // check timing
    if (GET_MODE_IVSTA() < 1) return _MODE_NOSUPPORT;

    if (GET_MODE_IHSTA() < 1) return _MODE_NOSUPPORT;

    if (GET_MODE_IVTOTAL() < (GET_MODE_IVHEIGHT() + GET_MODE_IVSTA()))
        return	_MODE_NOSUPPORT;

    if (GET_MODE_IHTOTAL() < (GET_MODE_IHWIDTH() + GET_MODE_IHSTA()))
        return	_MODE_NOSUPPORT;

    if (GET_MODE_IHWIDTH() < 120) return _MODE_NOSUPPORT;

    if (GET_MODE_IVHEIGHT() < 240) return _MODE_NOSUPPORT;

    IHTotal_index=(GET_MODE_IHTOTAL()  + 1)/10;

    IVTotal_index=(GET_MODE_IVTOTAL()  + 1)/10;

    if (drvif_Hdmi_GetInterlace())
        IVTotal_index*=2;

    tmds_clk=IHTotal_index*IVTotal_index*(GET_MODE_IVFREQ()/10);
    tmds_clk=tmds_clk/10;

    if (tmds_clk > 165000)   //kHz
    {
        HDMI_PRINTF(LOGGER_INFO, "TMDS CLOCK is over 165MHz! \n");
        return  _MODE_NOSUPPORT;
    }


    Hdmi_Search_Mode();			// search mode for UI

    HDMITimingCompensation();		// do timing compensation


    return _MODE_SUCCESS;

}

/*======================= API Function ========================*/
/**
 * IsHDMI
 * check is HDMI/DVI
 *
 * @param 				{  }
 * @return 				{ return MODE of detection, MODE_HDMI = 1, MODE_DVI = 0 }
 * @ingroup drv_hdmi
 */
HDMI_DVI_MODE_T IsHDMI(void)
{
    // Switch to Page 2 referenced from spec
    rtdf_pageSelect(2);

    if (hdmi_in(HDMI_HDMI_SR_VADDR) & _BIT0)
    {
        //HDMI_PRINTF(LOGGER_INFO,"<<<<<<<<<< HDMI mode \n");
        return MODE_HDMI; // HDMI mode
    }
    else
    {
        //HDMI_PRINTF(LOGGER_INFO,"<<<<<<<<<< DVI mode \n");
        return MODE_DVI; // DVI mode
    }
}

/**
 * drvif_IsHDMI
 * To tell  scaler is HDMI/DVI
 *
 * @param 				{  }
 * @return 				{ return MODE of detection, MODE_HDMI = 1, MODE_DVI = 0 }
 * @ingroup drv_hdmi
 */
#if 0
HDMI_DVI_MODE_T drvif_IsHDMI(void)
{

    return (HDMI_DVI_MODE_T)GET_ISHDMI();
}
#endif
/**
 * Hdmi_GetInterlace
 * check if input signal is interlace or not , for
 *
 * @param 				{  }
 * @return 				{ TRUE means interlaced, FALSE means progressive }
 * @ingroup drv_hdmi
 */

HDMI_BOOL Hdmi_GetInterlace(void)
{
    // Switch to Page 2 referenced from spec
    rtdf_pageSelect(2);

    hdmiport_out(HDMI_HDMI_VCR_VADDR, hdmiport_in(HDMI_HDMI_VCR_VADDR) | _BIT6); //clear status
    HDMI_DELAYMS(60);
    if (hdmiport_in(HDMI_HDMI_VCR_VADDR) & _BIT6) // interlace mode
    {
//		HDMI_PRINTF(LOGGER_INFO,">>>> HDMI interlaced >>>>\n");
        return TRUE;
    }
    else
    {
//		HDMI_PRINTF(LOGGER_INFO,">>>> HDMI Progressive >>>>\n");
        return FALSE;
    }

}
/*	not used Willy
HDMI_BOOL drvif_IsAVMute(void) {
	return ((hdmi_in(HDMI_HDMI_SR_VADDR) & _BIT6)	!= 0);
}
*/

/**
 * drvif_Hdmi_GetInterlace
 * To tell scaler hdmi signal is interlace or not
 *
 * @param 				{  }
 * @return 				{ TRUE means interlaced, FALSE means progressive }
 * @ingroup drv_hdmi
 */
HDMI_BOOL drvif_Hdmi_GetInterlace(void)
{
    return GET_HDMI_ISINTERLACE();
}

/**
 * drvif_Hdmi_TmdsInit
 * Columbus HDMI TMDS initial function
 *
 * @param 				{  }
 * @return 				{ no meaning , reserved for future }
 * @ingroup drv_hdmi
 */
HDMI_BOOL  drvif_Hdmi_TmdsInit(void)
{
    // Switch to Page 2 referenced from spec
    rtdf_pageSelect(2);


    hdmi_mask(HDMI_TMDS_Z0CC2_VADDR, ~_BIT6, _BIT6);  // enable TMDS clock and pull up resistor
#if   HDMI_AUTO_DEONLY
    SET_HDMI_DEONLY_MODE(0);
    //hdmi_mask(HDMI_TMDS_PWDCTL_VADDR,~0xbf, 0x1f);  // Enable Auto Channel Input Control and Generate DE VS/HS from DE
    hdmi_mask(HDMI_TMDS_PWDCTL_VADDR,~0xbf, 0x0f);  // adams modify 20110124,disable Auto Channel Input Control and Generate DE VS/HS from DE
#else

#if   HDMI_DE_ONLY
    SET_HDMI_DEONLY_MODE(1);
    //hdmi_mask(HDMI_TMDS_PWDCTL_VADDR,~0xbf, 0x9f);  // Enable Auto Channel Input Control and Generate DE VS/HS from DE
    hdmi_mask(HDMI_TMDS_PWDCTL_VADDR,~0xbf, 0x8f);  // adams modify 20110124,disable Auto Channel Input Control and Generate DE VS/HS from DE
#else
    SET_HDMI_DEONLY_MODE(0);
    //hdmi_mask(HDMI_TMDS_PWDCTL_VADDR,~0xbf, 0x1f);  // Enable Auto Channel Input Control and Generate DE VS/HS from DE
    hdmi_mask(HDMI_TMDS_PWDCTL_VADDR,~0xbf, 0x0f);  // adams modify 20110124,disable Auto Channel Input Control and Generate DE VS/HS from DE
#endif
#endif
    hdmi_out(HDMI_TMDS_OUTCTL_VADDR, 0x78);//enable output & cbcr correct enable
    //??hdmiport_mask(HDMI_TMDS_DPC_SET0_VADDR,0xff,(UINT8) _BIT7);
    //hdmi_out(HDMI_TMDS_OUTCTL_VADDR, 0x58);//enable output & cbcr correct enable

    rtdf_pageSelect(0x0b);
    /*TMDS inital setting*/
#if 1//HDMI setting for SPARROW
    hdmi_out(HDMI_Z0_CALIBRATION_VADDR,0xf8);					//
    hdmi_out(HDMI_POWER_CONTROL_VADDR,0xFF);					//power off
//	hdmi_out(HDMI_POWER_CONTROL_VADDR,0xEE);					//power off
    hdmi_out(HDMI_RXMISC_02_VADDR,0x87);
    //===vco divider mod decision code===
    hdmi_out(HDMI_FLD_05_VADDR,0x2f);	// sparrow setting
    hdmi_out(HDMI_CDR_01_VADDR,0x82);
    hdmi_out(HDMI_CDR_02_VADDR,0xad);
    hdmi_out(HDMI_FLD_09_VADDR,0x0F);
    hdmi_out(HDMI_CDR_03_VADDR,0xd5);
    hdmi_out(HDMI_RXMISC_01_VADDR,0x09);
    hdmi_out(HDMI_RXMISC_02_VADDR,0xc7);
    hdmi_out(HDMI_RESERVED_00_VADDR, 0x88);	//adams add 20110330 for clk b value measure fail

    hdmi_mask(HDMI_ADAPTIVE_EQUALIZER_VADDR,~(_BIT6|_BIT5), (_BIT6|_BIT5));
    hdmi_mask(HDMI_ADAPTIVE_EQUALIZER_VADDR,~(_BIT6), 0);	//adams add 20110714 for clk b value measure fail
    hdmi_mask(HDMI_ADAPTIVE_EQUALIZER_VADDR,~(_BIT6), _BIT6);	//Koffset calibration enable

    hdmi_mask(HDMI_FLD_05_VADDR, ~_BIT5, _BIT5);
    hdmi_mask(HDMI_FLD_05_VADDR, ~_BIT6, _BIT6);
    hdmi_mask(HDMI_FLD_05_VADDR, ~_BIT6, 0);
    hdmi_mask(HDMI_SWITCH_2D_VADDR,~_BIT0, 0);						//enable bit 7 before setting VCO parameters
    //HDMI_DELAYMS(1);
    hdmi_mask(HDMI_SWITCH_2D_VADDR,~_BIT0, _BIT0);						//clean bit 7 to reset PLL after setting VCO parameters
#else//viking
    hdmi_mask(HDMI_RESERVED_01_VADDR, ~(UINT8) _BIT7, (UINT8) _BIT7);						//power on PHY
    hdmi_mask(HDMI_POWER_CONTROL_VADDR,~0xff, 0xff);					//power on channel and CMU
    hdmi_out(HDMI_AUX_00_VADDR,0x92);								//
    hdmi_out(HDMI_RXMISC_01_VADDR,0x01);
    hdmi_out(HDMI_RXMISC_02_VADDR,0x87);

//	hdmi_out(HDMI_DP_CTRL_VADDR,0x20);									//power on and select HDMI in MAC layer
//	hdmi_out(0xb800fb80,0xD0);									//
    hdmi_out(HDMI_Z0_CALIBRATION_VADDR,0xf8);					//
    hdmi_out(HDMI_POWER_CONTROL_VADDR,0xFF);					//power off
    hdmi_out(HDMI_ADAPTIVE_EQUALIZER_VADDR,0x16);				//
    hdmi_out(HDMI_RXMISC_02_VADDR,0x87);
    //===vco divider mod decision code===
    hdmi_out(HDMI_FLD_05_VADDR,0x3b);
    hdmi_out(HDMI_FLD_06_VADDR,0x76);
    hdmi_out(HDMI_FLD_07_VADDR,0xed);
    hdmi_out(HDMI_CDR_01_VADDR,0x82);
    hdmi_out(HDMI_CDR_02_VADDR,0x39);
    hdmi_out(HDMI_FLD_09_VADDR,0x2F);
    hdmi_out(HDMI_CDR_03_VADDR,0x94);
    hdmi_out(HDMI_RESERVED_05_VADDR,0xf1);
    hdmi_out(HDMI_RESERVED_00_VADDR,0xb0);
    hdmi_out(HDMI_RXMISC_01_VADDR,0x01);
    hdmi_out(HDMI_RXMISC_02_VADDR,0xc7);
    hdmi_out(HDMI_ADAPTIVE_EQUALIZER_VADDR,0x76);
    hdmi_mask(HDMI_OOBS_RXAMP_VADDR,~(UINT8) _BIT7, 0);						//clean bit 7 to reset PLL after setting VCO parameters
    hdmi_mask(HDMI_OOBS_RXAMP_VADDR,~(UINT8) _BIT7, (UINT8) _BIT7);						//enable bit 7 before setting VCO parameters
    hdmi_mask(HDMI_OOBS_RXAMP_VADDR,~(UINT8) _BIT7, 0);						//clean bit 7 to reset PLL after setting VCO parameters
#endif
    rtdf_pageSelect(2);
    hdmi_mask(HDMI_TMDS_CPS_VADDR, ~0x60, 0x60);  // select TMDS clock as measure source


    return TRUE;

}



/**
 * drvif_Hdmi_Init
 * initial function for HDMI
 *
 * @param 				{  }
 * @return 				{  }
 * @ingroup drv_hdmi
 */
void __init drvif_Hdmi_Init(void)
{
    int i, j;
    HDMI_LOG(LOGGER_INFO,"drvif_Hdmi_Init\n");

    Force_HDMIPHY_RST=0;
#if HDMI_BULD_IN_TABLE
    for (i=0; i< HDMI_MAX_CHANNEL; i++)
    {
        hdmi.channel[i] = &default_channel[i];
        //HDMI_PRINTF(LOGGER_INFO,"init %d %d\n", i, (hdmi.channel[i])->Enable);
    }
    hdmi.hdcpkey  = &default_hdcpkey;
#endif


    // fill out EDID table
    for (j=0; j<HDMI_MAX_CHANNEL; j++)
    {
        if (hdmi.channel[j] == 0) continue;
        if (hdmi.channel[j]->Enable == 0) continue;
        if (hdmi.channel[j]->DDCEnable)
        {
            rtd_outl(GET_DDC_SIR_VADDR(hdmi.channel[j]->ddc_index),0);	//SRAM Index = 0
#if 1
            drv_hdmi_LoadEDIDKeyCode(j);
#else
            for (i=0; i<256; i++)	//Write EDID to SRAM
                rtd_outl(GET_DDC_SAP_VADDR(hdmi.channel[j]->ddc_index), hdmi.channel[j]->EDID[i]);
#endif
            rtd_outl(GET_DDC_SIR_VADDR(hdmi.channel[j]->ddc_index), 0);	//SRAM Index = 0
            rtd_outl(GET_EDID_CR_VADDR(hdmi.channel[j]->ddc_index), 0x1);	// External Access Enable
        }
    }

    // all hot plug high
    for (j=0; j<HDMI_MAX_CHANNEL; j++)
    {
        if (hdmi.channel[j] == 0) continue;
        if (hdmi.channel[j]->Enable == 0) continue;
        if (hdmi.channel[j]->HotPlugPin == 0) continue;
        HDMI_HPD_LEVEL((hdmi.channel[j])->HotPlugPin, (hdmi.channel[j])->HotPlugOn);
    }


    // setup/hook 10ms timer ISR for PHY adjustment
    // and Timer ISR initital state is suspend mode
    rtdf_pageSelect(0x0b);
    hdmi_mask(HDMI_RESERVED_01_VADDR, ~(UINT8) _BIT7, (UINT8) _BIT7);
    HdmiISRInit();

}


/**
 * drvif_Hdmi_InitSrc
 * Initial Call back function for Upper mode state machine
 *
 * @param 				{  channel: HDMI chnanel }
 * @return 				{  }
 * @ingroup drv_hdmi
 */
extern void SetupTMDSPhy(UINT16 b, HDMI_BOOL force);

void drvif_Hdmi_InitSrc(UINT8 channel)
{
//	int i;
//	UINT32 addr;
//	UINT32 value;
    HDMI_PRINTF(LOGGER_INFO,"drvif_Hdmi_InitSrc\n");
//	rtd_maskl(0xb800183c, 0x0f0f0f0f, 0x90909090);//DDC
    // reset FSM
    SET_HDMI_VIDEO_FSM(MAIN_FSM_HDMI_SETUP_VEDIO_PLL);
    SET_HDMI_AUDIO_FSM(AUDIO_FSM_AUDIO_START);

    if (channel >= HDMI_MAX_CHANNEL)
    {
        HDMI_PRINTF(LOGGER_INFO,"Wrong Channel Number: %d\n", channel);
        return;
    }
    if (hdmi.channel[channel] == 0) return;
    if (hdmi.channel[channel]->Enable == 0)
    {
        HDMI_PRINTF(LOGGER_INFO,"This channel %d is Disable\n", channel);
        return;
    }
    Hdmi_Power(1);	    // HDMI power on , exit power saving mode
    // reset hdmi struct
    SET_HDMI_COLOR_SPACE(COLOR_RGB);
    SET_HDMI_ISINTERLACE(0);
    SET_ISHDMI(MODE_DVI);
    SET_HDMI_CD(0);
    hdmi.b = 0;

    drvif_Hdmi_TmdsInit();

    // Switch to Page 2 referenced from spec
    rtdf_pageSelect(2);

    hdmiport_out(HDMI_HDMI_SCR_VADDR , 0x02);//DVI/HDMI condition(A,B) select //02
    hdmiport_out(HDMI_HDMI_AFCR_VADDR , 0x06);//Enable Audio FIFO
    hdmiport_out(HDMI_HDMI_AVMCR_VADDR , 0x48);//enable video & audio output
    hdmiport_out(HDMI_HDMI_WDCR0_VADDR , 0x00);  //disable watch dog
    hdmiport_out(HDMI_HDMI_WDCR1_VADDR , 0x00);  //disable watch dog
    hdmiport_out(HDMI_HDMI_WDCR2_VADDR , 0x00);  //disable watch dog
    hdmiport_out(HDMI_HDMI_BCHCR_VADDR , 0x19);	// Enable BCH Function
    hdmiport_out(HDMI_HDMI_PVGCR0_VADDR , 0x09);	//For HDMI Packet
    hdmiport_out(HDMI_HDMI_ACRCR_VADDR , 0x44);//pixel repetetion auto mode,YUV444/422/RGB auto detect mode
//	hdmiport_mask(HDMI_HDMI_AOCR_VADDR,(UINT8)~(0xff),0x00);//Disable SPDIF/I2S Output
//	hdmiport_mask(HDMI_HDMI_AOCR_VADDR,~_BIT10,_BIT10);//Hold avmute value outside v-sync region
    hdmiport_out(HDMI_HDMI_MAGCR1_VADDR, 0x14);

    // clear HDMI interrupt control register
    hdmiport_out(HDMI_HDMI_INTCR_VADDR, 0);
    hdmiport_mask(HDMI_HDMI_VCR_VADDR,(UINT8) ~(_BIT7) , 0);  // not inverse EVEN/ODD

    hdmiport_mask(HDMI_HDMI_WDCR1_VADDR ,(((UINT8) ~_BIT7)), 0x00);//disable  tmds clock audio watch dog
    hdmiport_mask(HDMI_HDMI_PAMICR_VADDR ,~_BIT0, 0x00);//Disable packet variation Watch Dog
    hdmiport_mask(HDMI_HDMI_AOCR_VADDR,0x00, 0x00);//Disable SPDIF/I2S Output
    hdmi_mask(HDMI_TMDS_PWDCTL_VADDR ,~( _BIT3 | _BIT2 | _BIT1 | _BIT0), (_BIT3 | _BIT2 | _BIT1 | _BIT0));
    hdmi_mask(HDMI_TMDS_OUTCTL_VADDR ,(UINT8)~(_BIT7 | _BIT6 | _BIT5 | _BIT4| _BIT3), (_BIT6 | _BIT5 | _BIT4 | _BIT3));
    hdmi_mask(HDMI_TMDS_Z0CC2_VADDR ,~_BIT6,_BIT6);
    hdmiport_mask(HDMI_HDMI_AOCR_VADDR,(UINT8)(~0x0ff),0xFF);	//Enable SPDIF/I2S Output
    hdmi_mask(HDMI_TMDS_DPC0_VADDR, (~_BIT7),  (_BIT7));
    rtdf_pageSelect(0x0b);
    hdmi_mask(HDMI_DIG_00_VADDR, (~_BIT4),  (_BIT4));

    //20100409 YH Modify DDC slew rate setting for long cable + CASA player
    rtd_outb(0x2D07, 0xC0);

#if 0
    hdmi_out(HDMI_DIG_00_VADDR, 0xd0);
    rtd_outl(0xb800fae8, 0x000000e0);
#endif

    rtdf_pageSelect(2);
    //ECN_Method 0->1(ECN_en=1,Encryption,ECN_en=0,not Encryption)
    Hdmi_HdcpPortWrite(0xc4,0x00);
    hdmiport_out(HDMI_HDMI_PTRSV1_VADDR, 0x82);		// Wait for ACR : Packet Type = 0x01
    // setup Audio PLL
//	Hdmi_AudioPLLSetting(48000, 0);
    Hdmi_AudioOutputDisable();

    if (channel < HDMI_MAX_CHANNEL)
    {

        // set DDC channel for HDCPs
        hdmi_mask(HDMI_HDCP_PCR_VADDR, ~(_BIT1 | _BIT2),(((HDMI_CHANNEL_T *)hdmi.channel[channel])->ddc_index & 0x03)<< 1);
        // Disable HDCP and clear HDCP address
        hdmi_out(HDMI_HDCP_CR_VADDR,0x06);

        // write BKsv into DDC channel
        /*HDMI_PRINTF(LOGGER_INFO,"\n");
        for(i=0;i<5;i++) {
        	HDMI_PRINTF(LOGGER_INFO,"%x ", hdmi.hdcpkey->BKsv[i]);
        }
        HDMI_PRINTF(LOGGER_INFO,"\n");*/
#if 1
        drv_hdmi_LoadHDCPKeyCode();
//		hdmi_out(HDMI_HDCP_CR_VADDR,0x00);
//		HDMI_PRINTF(LOGGER_INFO,"\n=============================================\n");
//		hdmi_out(HDMI_HDCP_CR_VADDR,0x06);
//		for(i=0;i<320;i++) {
//			if (i% 8 == 0) HDMI_PRINTF(LOGGER_INFO,"\n");
//			HDMI_PRINTF(LOGGER_INFO,"%x, \n", (UINT32)hdmi_in(HDMI_HDCP_DKAP_VADDR));
//		}
#else
        {
//			rtdf_clearBitsb(SYS_CLKSEL_0x2ccc, _BIT1);
//			rtd_outb(0xffed, 0x84);		// divider 7
//			HDMI_DELAYMS(1);
            drv_sparrow_CPU_Switch_Clock(_FALSE);
        }
        /*Write device private key*/
#if 1
        drv_spi_dma_6093cpy((HDMI_HDCP_DKAP_VADDR & 0xff), _BANK4 , (UINT16)hdmi.hdcpkey->Key, 320, 1);

//		rtdf_setBitsb(0xfff3, _BIT5);
//		addr = (UINT16)hdmi.hdcpkey->Key;
//		*((UINT8*)&addr + 1) = _BANK4;
//		drv_spi_dma_memcpy((HDMI_HDCP_DKAP_VADDR & 0xff), addr, 320, SPI_DMA_TRANSFER_MODE_6093);
#else
        for (i=0; i<320; i++)
        {
//			if (i% 8 == 0) HDMI_PRINTF(LOGGER_INFO,"\n");
//			HDMI_PRINTF(LOGGER_INFO,"%x ", hdmi.hdcpkey->Key[i]);

            hdmi_out(HDMI_HDCP_DKAP_VADDR,hdmi.hdcpkey->Key[i]);
        }
#endif
//		hdmi_out(HDMI_HDCP_CR_VADDR,0x00);
//		HDMI_PRINTF(LOGGER_INFO,"\n=============================================\n");
//		hdmi_out(HDMI_HDCP_CR_VADDR,0x06);
//		for(i=0;i<320;i++) {
//			if (i% 8 == 0) HDMI_PRINTF(LOGGER_INFO,"\n");
//			HDMI_PRINTF(LOGGER_INFO,"%x ", (UINT32)hdmi_in(HDMI_HDCP_DKAP_VADDR));
//		}
        {
//			rtd_outb(0xffed, 0x9b);		// divider 7
//			rtdf_setBitsb(SYS_CLKSEL_0x2ccc, _BIT1);
//			HDMI_DELAYMS(1);
            drv_sparrow_CPU_Switch_Clock(_TRUE);

        }
#endif
        hdmi_out(HDMI_HDCP_CR_VADDR,0x00);

        // enable HDCP function for all
        hdmi_mask(HDMI_HDCP_CR_VADDR,~_BIT0,_BIT0);

        // Lane switch
        rtdf_pageSelect(0x0b);
        hdmi_out(DP_PHY_2D_switch_VADDR, hdmi.channel[channel]->DP_PHY_2D_switch_value);
        hdmi_out(DP_PHY_RESERVED_07, hdmi.channel[channel]->DP_PHY_RESERVED_07_value);
        // HPG High
        if (hdmi.channel[channel]->HotPlugPin != 0)
        {
            HDMI_HPD_LEVEL(hdmi.channel[channel]->HotPlugPin, hdmi.channel[channel]->HotPlugOn == 0);
            HDMI_DELAYMS(500);
            HDMI_HPD_LEVEL(hdmi.channel[channel]->HotPlugPin, hdmi.channel[channel]->HotPlugOn);
        }
        SET_HDMI_CHANNEL(channel);
    }
    else
    {
        HDMI_PRINTF(LOGGER_INFO,"HDMI assigned channel excess maximun Channel\n");
    }
#if HDMI_DEBUG_AUDIO_PLL_RING
    HdmiAudioPLLSampleStart();
#endif
    hdmi.timer = 0;
    HdmiISREnable(1);  // enable 10ms timer ISR for PHY adjustment
    AVMute_Timer = 0;
#if 0//for temp test
//	rtdf_pageSelect(0x0b);
    //hdmi_out(HDMI_DIG_01_VADDR, 0xFf);
    HDMI_DELAYMS(1000);
    rtd_hdmiPhy_ISR();
    HDMI_DELAYMS(1000);
    rtd_hdmiPhy_ISR();
    HDMI_DELAYMS(1000);
    rtd_hdmiPhy_ISR();
    HDMI_DELAYMS(1000);
    rtd_hdmiPhy_ISR();
    HDMI_DELAYMS(1000);
//	rtdf_pageSelect(0x0b);
#if 0
    for (addr = 0xf81b00a0; addr < 0xf81b0100; addr++)
    {
        value = hdmi_in(addr);
        HDMI_PRINTF(LOGGER_INFO, "%x, ", value);
        if ((addr & 0xF) == 0x0F) HDMI_PRINTF(LOGGER_INFO, "\n");
    }

    for (addr = 0; addr < 16; addr++)
    {
        rtdf_pageSelect(0x02);
        HDMI_PRINTF(LOGGER_INFO, "\nselect = %d\n", (addr & 0x3) );
        hdmi_mask(HDMI_TMDS_CPS_VADDR, ~0x60, (addr & 0x3) << 5);  // select TMDS clock as measure source
        hdmiport_mask(HDMI_HDMI_NTX1024TR0_VADDR,0xff,_BIT3);			//Restart measure b

        HDMI_DELAYMS(100);

        if ((hdmiport_in(HDMI_HDMI_NTX1024TR0_VADDR) & _BIT3) == 0)
        {
            value =  (((UINT16)hdmiport_in(HDMI_HDMI_NTX1024TR0_VADDR) & 0x7) << 8) | (UINT16)hdmiport_in(HDMI_HDMI_NTX1024TR1_VADDR) ;
            HDMI_PRINTF(LOGGER_INFO, "\nb = %d\n", (UINT32)value);
        }

        //	if (addr == 7) 	SetupTMDSPhy(0, 1);
    }
#endif
#endif
}

/**
 * drvif_Hdmi_Release
 * Release HDMI
 *
 * @param 				{  channel: HDMI chnanel }
 * @return 				{  }
 * @ingroup drv_hdmi
 */
void drvif_Hdmi_Release(void)
{

    HDMI_LOG(LOGGER_INFO,"drvif_Hdmi_Release\n");
#if HDMI_DEBUG_AUDIO_PLL_RING
    HdmiAudioPLLSampleStop();
#endif
    Hdmi_AudioOutputDisable();
    Hdmi_VideoOutputDisable();
    HdmiISREnable(0);
    Hdmi_Power(0);
    if (hdmi.channel[GET_HDMI_CHANNEL()]->Enable == 0)
    {
        HDMI_LOG(LOGGER_INFO,"This channel %d is Disable\n", GET_HDMI_CHANNEL());
        return;
    }

    //reset VGIP even/odd inversion.comment by justin.
    //rtd_maskl(GET_VGIP_CHNx_CTRL_VADDR(), ~(_BIT31| _BIT21 | _BIT16 | _BIT18 |_BIT0), 0x0);
    HDMI_DELAYMS(300); // for PIP case this delay is neccasary
}


/**
 * InProcHDMI_Setup
 * setup function after detect mode, return FALSE will result in re-detect mode
 *
 * @param 				{  }
 * @return 				{ FALSE  }
 * @ingroup drv_adc
 */

HDMI_BOOL InProcHDMI_Setup(void)
{
    HDMI_LOG(LOGGER_INFO,"InProcHDMI_Setup\n");

    // Switch to Page 2 referenced from spec
    rtdf_pageSelect(2);

    if (hdmi_in(HDMI_HDMI_SR_VADDR) & _BIT6)   // AVMute Set
    {
        return FALSE;
    }
    //frank@0507 move from mode.c
    //interlace setting
    if (drvif_Hdmi_GetInterlace()) // interlace mode
    {
        HDMI_LOG(LOGGER_INFO,"Set Interlace mode\n");
        //Scaler_DispSetStatus(Scaler_DispGetInputInfo(SLR_INPUT_DISPLAY), SLR_DISP_INTERLACE, TRUE);


    }
    else
    {
        HDMI_LOG(LOGGER_INFO,"Clear Interlace mode\n");
        //Scaler_DispSetStatus(Scaler_DispGetInputInfo(SLR_INPUT_DISPLAY), SLR_DISP_INTERLACE, FALSE);
    }
    drvif_mode_enableonlinemeasure();

    return TRUE;
}

/**
 * InProcHDMI_Init
 * Initial Function after InitSrc
 *
 * @param 				{  }
 * @return 				{  }
 * @ingroup drv_hdmi
 */

void InProcHDMI_Init(void)
{
    HDMI_LOG(LOGGER_INFO,"InProcHDMI_Init\n");
}


/**
 * drvif_Hdmi_CheckMode
 * Check Function for upper level mode state process;  return FALSE will result in re-detect mode
 *
 * @param <sizeInByte>	{ length of circular buffer }
 * @return 				{ "FLASE" means something wrong }
 * @ingroup drv_hdmi
 */
HDMI_BOOL drvif_Hdmi_CheckMode(void)
{

    HDMIRX_IOCTL_STRUCT_T isr_info;
//->w	HDMI_LOG(LOGGER_INFO,"drvif_Hdmi_CheckMode\n");

    /*
    	Enable Vedio WDG as Check Mode
    */
//->w	HDMI_LOG(LOGGER_INFO, "PLL lock  %x\n", (UINT32)hdmiport_in(HDMI_HDMI_AVMCR_VADDR));
//->w	HDMI_LOG(LOGGER_INFO, "HDMI_HDMI_SR_VADDR = %x\n", hdmi_in(HDMI_HDMI_SR_VADDR));
//->w	HDMI_LOG(LOGGER_INFO, "HDMI_HDMI_AFCR_VADDR = %x\n", hdmiport_in(HDMI_HDMI_AFCR_VADDR));
//	HDMI_LOG(LOGGER_INFO, "HDMI_HDCP_CR_VADDR = %x\n", (UINT32)hdmi_in(HDMI_HDCP_CR_VADDR));
#if HDMI_PRINT_CRC	//print CRC
    HDMI_Check_CRC();
    rtdf_pageSelect(0x0b);
//->w		HDMI_LOG(LOGGER_INFO, "HDMI_CDR_01_VADDR = %x\n", (UINT32)hdmi_in(HDMI_CDR_01_VADDR));
#endif
//	Hdmi_DumpState();

#if 0
    RTD_Log(LOGGER_INFO, "0x3050:%x\n",(UINT32)rtd_inl(0x3050));
    RTD_Log(LOGGER_INFO, "0x3054:%x\n",(UINT32)rtd_inl(0x3054));
    RTD_Log(LOGGER_INFO, "0x3100:%x\n",(UINT32)rtd_inl(0x3100));
    RTD_Log(LOGGER_INFO, "0x3130:%x\n",(UINT32)rtd_inl(0x3130));
    RTD_Log(LOGGER_INFO, "0x3134:%x\n",(UINT32)rtd_inl(0x3134));
#endif
//	rtd_hdmiPhy_ISR();
//	HDMI_DELAYMS(500);
//	rtd_hdmiPhy_ISR();
//	HDMI_DELAYMS(500);
    // Switch to Page 2 referenced from spec
    rtdf_pageSelect(2);

    if (GET_ISHDMI() == MODE_HDMI)     		 //HDMI mode
    {
        //Hdmi_DumpState();
//		hdmiport_mask(HDMI_HDMI_WDCR0_VADDR, (UINT8)~(_BIT7 | _BIT6), (UINT8) (_BIT7 | _BIT6));		// Enable AVMute  	WDG

        // Check Vedio Format change
        if (Hdmi_GetColorSpace() != GET_HDMI_COLOR_SPACE())
        {
            HDMI_PRINTF(LOGGER_INFO,"Color Space Change\n");
            SET_HDMI_COLOR_SPACE(Hdmi_GetColorSpace());
//->w			HDMI_LOG(LOGGER_INFO, " Format = %d\n", GET_HDMI_COLOR_SPACE());
            HDMISetScalerInfo();
            HdmiSetScalerColor();
        }

        drvif_Hdmi_AudioModeDetect();
        if ((hdmi_in(HDMI_HDMI_SR_VADDR) & _BIT6)&&  (!(hdmiport_in(HDMI_HDMI_BCSR_VADDR) & _BIT0)))
        {
            Hdmi_VideoOutputDisable();
            Hdmi_AudioOutputDisable();
            hdmi_mask(HDMI_HDMI_SR_VADDR, ~(_BIT6), (_BIT6));
            HDMI_PRINTF(LOGGER_INFO,"AVMute ON----> Force re-detect signal\n");
            return FALSE;
        }
    }
    if (GET_HDMI_ISINTERLACE() != Hdmi_GetInterlace())     // if interlace mode change
    {
        Hdmi_VideoOutputDisable();
        Hdmi_AudioOutputDisable();
        HDMI_PRINTF(LOGGER_INFO,"IsInterlaced change\n");
        return FALSE;
    }

    if (GET_ISHDMI()!= IsHDMI())
    {
        HDMI_DELAYMS(10);
        if (GET_ISHDMI()!= IsHDMI())   //for debounce
        {
            Hdmi_VideoOutputDisable();
            Hdmi_AudioOutputDisable();
            HDMI_PRINTF(LOGGER_INFO,"bIsHDMIDVI change\n");
            return FALSE;
        }
    }

    if ((UINT32)GET_HDMI_CD()!= (hdmi_in(HDMI_TMDS_DPC0_VADDR) & 0xf))
    {
        Hdmi_VideoOutputDisable();
        Hdmi_AudioOutputDisable();
        HDMI_PRINTF(LOGGER_INFO,"IsDCmode change\n");
        return  FALSE;
    }

    HdmiGetStruct(&isr_info);
    if (ABS(hdmi.b, isr_info.b) > 4)
    {
        Hdmi_VideoOutputDisable();
        Hdmi_AudioOutputDisable();
        HDMI_PRINTF(LOGGER_INFO,"isr_info.b change\n");
        return FALSE;
    }
#if 0 //tyh
    if (isr_info.b_change)
    {
        Hdmi_VideoOutputDisable();
        Hdmi_AudioOutputDisable();
        HDMI_PRINTF(LOGGER_INFO,"b change\n");
        return FALSE;

    }
#endif
//#if (HDMI_DE_ONLY)
#if 1
    if (!drvif_mode_onlinemeasure(ANALOG_MODE_MEASUREMENT))
    {
        HDMI_PRINTF(LOGGER_INFO,"mode change\n");
        Hdmi_VideoOutputDisable();
        Hdmi_AudioOutputDisable();
        //hdmi.force_reseek = 1;
        return  FALSE;
    }
#else
    if (Hdmi_OnlineMeasure2() == FALSE)
    {
        Hdmi_VideoOutputDisable();
        Hdmi_AudioOutputDisable();
        return FALSE;
    }

#endif
    return TRUE;
}

/**
 * drvif_Hdmi_DetectMode
 * Detect Function for upper level mode state process;  return MODE_SUCCESS will transition from
 *	detect state into check mode
 *
 * @param 				{ }
 * @return 				{ }
 * @ingroup drv_hdmi
 */

extern HDMIRX_IOCTL_STRUCT_T hdmi_ioctl_struct;
UINT8 drvif_Hdmi_DetectMode(void)
{
    UINT8 result = _MODE_NOSIGNAL;
    HDMIRX_IOCTL_STRUCT_T isr_info;
#if HDMI_Patch_For_Splitter
    UINT32 detect_timeout;
#endif
//	rtd_hdmiPhy_ISR();
//	HDMI_DELAYMS(500);
//	rtd_hdmiPhy_ISR();
//	HDMI_DELAYMS(500);

    // Switch to Page 2 referenced from spec
    rtdf_pageSelect(2);
#if 0//for debug
    {
        UINT8 temp;
        for (temp=0; temp<10; temp++)
            rtd_hdmiPhy_Check_CRC();
    }
#endif
//	return _MODE_NOSIGNAL;
    switch (GET_HDMI_VIDEO_FSM())
    {

    case MAIN_FSM_HDMI_SETUP_VEDIO_PLL:
        HDMI_PRINTF(LOGGER_INFO,"MAIN_FSM_HDMI_SETUP_VEDIO_PLL\n");
        hdmi_ioctl_struct.measure_count = 1000;
//					HDMI_DISABLE_TIMER_ISR();
        hdmi_mask(HDMI_TMDS_CPS_VADDR, (~(UINT8) _BIT7), 0);
//					HDMI_ENABLE_TIMER_ISR();
        hdmi_out(HDMI_TMDS_UDC0_VADDR, 0x00);			// Disable deep color mode
        hdmiport_out(HDMI_HDMI_WDCR0_VADDR, 0x00);  	// Disable Audio Watch Dog //??
        hdmiport_mask(HDMI_HDMI_WDCR1_VADDR, ~0xF, 0x00); // Clear Audio Watch Dog and Set X: 15
        hdmiport_mask(HDMI_HDMI_WDCR2_VADDR, ~0xF, 0x00); // Clear Vedio Watch Dog and Set Y: 15
        hdmiport_mask(HDMI_HDMI_VCR_VADDR, (UINT8)(~_BIT7) , 0);  // not inverse EVEN/ODD
        hdmiport_out(HDMI_HDMI_ACRCR_VADDR ,(_BIT6|_BIT2)); // auto color space and pixel repeat determination

        SET_HDMI_ISINTERLACE(Hdmi_GetInterlace());

        if (GET_HDMI_ISINTERLACE())
        {
#if HDMI_AUTO_DEONLY
            SET_HDMI_DEONLY_MODE(0);
            //hdmi_mask(HDMI_TMDS_PWDCTL_VADDR,~0xbf, 0x1f);  // Enable Auto Channel Input Control and Generate DE VS/HS from DE
            hdmi_mask(HDMI_TMDS_PWDCTL_VADDR,~0xbf, 0x0f);  // adams modify 20110124,disable Auto Channel Input Control and Generate DE VS/HS from DE
#endif
            if (!Hdmi_VedioPLLSetting(0))
                return _MODE_NOSIGNAL;
            HDMI_DISABLE_TIMER_ISR();
//							hdmi_mask(HDMI_TMDS_CPS_VADDR, (~(UINT8) _BIT7), (UINT8) _BIT7);
            HDMI_ENABLE_TIMER_ISR();
//							hdmi_out(HDMI_TMDS_UDC0_VADDR, 0x80);			// Enable deep color mode
        }
        else
        {
#if HDMI_AUTO_DEONLY
            SET_HDMI_DEONLY_MODE(0);//SET_HDMI_DEONLY_MODE(1);
            //hdmi_mask(HDMI_TMDS_PWDCTL_VADDR,~0xbf, 0x9f);  // Enable Auto Channel Input Control and Generate DE VS/HS from DE
            //hdmi_mask(HDMI_TMDS_PWDCTL_VADDR,~0xbf, 0x8f);  // adams modify 20110124,disable Auto Channel Input Control and Generate DE VS/HS from DE
            hdmi_mask(HDMI_TMDS_PWDCTL_VADDR,~0xbf, 0x0f);  // Kist modify 20110810,disable Generate DE VS/HS from DE
#endif

            if (!Hdmi_VedioPLLSetting(0))
                return _MODE_NOSIGNAL;
//							Hdmi_VedioPLLSetting(0);
        }
        HDMI_DELAYMS(30);

        SET_ISHDMI(IsHDMI());
        if (GET_ISHDMI() == MODE_DVI)
        {

            SET_HDMI_COLOR_SPACE(COLOR_RGB);
            //Disable Auto color space detect,Auto pixel reapeat down sample
            hdmiport_mask(HDMI_HDMI_ACRCR_VADDR ,~(_BIT6|_BIT2 | _BIT5 | _BIT4), 0);
            // Set down sampling = 1
            hdmiport_mask(HDMI_HDMI_VCR_VADDR,~(_BIT3|_BIT2|_BIT1|_BIT0), 0);

        }
        SET_HDMI_VIDEO_FSM(MAIN_FSM_HDMI_MEASURE);

        break;
    case MAIN_FSM_HDMI_MEASURE:
        HDMI_PRINTF(LOGGER_INFO,"MAIN_FSM_HDMI_MEASURE\n");
        Hdmi_VideoOutputEnable();
        if (GET_HDMI_ISINTERLACE()!= Hdmi_GetInterlace())     // if interlace mode change
        {
            HDMI_PRINTF(LOGGER_INFO,"interlace change\n");
            SET_HDMI_VIDEO_FSM(MAIN_FSM_HDMI_SETUP_VEDIO_PLL);
            break;
        }

        if (GET_ISHDMI() != IsHDMI())
        {
            HDMI_PRINTF(LOGGER_INFO,"HDMI/DVI change\n");
            SET_HDMI_VIDEO_FSM(MAIN_FSM_HDMI_SETUP_VEDIO_PLL);
            break;
        }

        if ((UINT32)GET_HDMI_CD() != (hdmi_in(HDMI_TMDS_DPC0_VADDR) & 0xf))
        {
            HDMI_PRINTF(LOGGER_INFO,"color depth change\n");
            SET_HDMI_VIDEO_FSM(MAIN_FSM_HDMI_SETUP_VEDIO_PLL);
            break;
        }

        HdmiGetStruct(&isr_info);
        HDMI_PRINTF(LOGGER_INFO,"isr_info.b = %x\n", (UINT32)isr_info.b);

        if (HDMI_ABS(hdmi.b, isr_info.b) > 4)
        {
            HDMI_PRINTF(LOGGER_INFO,"b change\n");
            SET_HDMI_VIDEO_FSM(MAIN_FSM_HDMI_SETUP_VEDIO_PLL);
            break;

        };
        if (isr_info.b_change)
        {
            HDMI_PRINTF(LOGGER_INFO,"b change\n");
            SET_HDMI_VIDEO_FSM(MAIN_FSM_HDMI_SETUP_VEDIO_PLL);
            break;

        }

        rtdf_pageSelect(2);
        if ( (hdmi_in(HDMI_HDMI_SR_VADDR) & _BIT6)&& (!(hdmiport_in(HDMI_HDMI_BCSR_VADDR) & _BIT0)) )
        {
            //if ((hdmi_in(HDMI_HDMI_SR_VADDR) & _BIT6)) { // AVMute not Set
            HDMI_PRINTF(LOGGER_INFO,"HDMI_TMDS_RPS_VADDR = %x\n",(UINT32)hdmi_in(HDMI_TMDS_RPS_VADDR));
            HDMI_PRINTF(LOGGER_INFO,"AV Mute\n");
        }
        else
        {
            if (Hdmi_Measure() == _MODE_SUCCESS)
            {
                // Determine DVI/HDMI mode
                if (GET_ISHDMI() == MODE_HDMI)
                {
                    SET_HDMI_AUDIO_FSM(AUDIO_FSM_AUDIO_START);	//tyh
                    drvif_Hdmi_AudioModeDetect();
                    // Determine Color Space
                    SET_HDMI_COLOR_SPACE(Hdmi_GetColorSpace());
                }
                else
                {
                    // Determine Color Space
                    SET_HDMI_COLOR_SPACE(COLOR_RGB);
                }

                HDMISetScalerInfo();

                HdmiSetScalerColor();

                // Dump All Measure Result
                Hdmi_DumpState();

                rtdf_pageSelect(2);
                if (hdmi_in(HDMI_HDMI_SR_VADDR) & _BIT6)   // if AVMute Set --> wait	AVMute Off
                {
                    HDMI_PRINTF(LOGGER_INFO,"#################### AV mute ####################\n");
                    break;
                }
                hdmi_out(HDMI_HDMI_SR_VADDR, hdmi_in(HDMI_HDMI_SR_VADDR));
                result = _MODE_SUCCESS;
#if HDMI_FIX_GREEN_LINE
                hdmi_mask(HDMI_TMDS_OUT_CTRL_VADDR, ~(_BIT7) , 0);
                hdmi_out(HDMI_TMDS_ROUT_HIGH_BYTE_VADDR, 0);
                hdmi_out(HDMI_TMDS_ROUT_LOW_BYTE_VADDR, 0);
                hdmi_out(HDMI_TMDS_GOUT_HIGH_BYTE_VADDR, 0);
                hdmi_out(HDMI_TMDS_GOUT_LOW_BYTE_VADDR, 0);
                hdmi_out(HDMI_TMDS_BOUT_HIGH_BYTE_VADDR, 0);
                hdmi_out(HDMI_TMDS_BOUT_LOW_BYTE_VADDR, 0);

                if (GET_HDMI_COLOR_SPACE() != COLOR_RGB)
                {
                    hdmi_out(HDMI_TMDS_ROUT_HIGH_BYTE_VADDR, 0x90);
                }
#endif

                /*
                	Set VGIP into Analog mode

                */
                // Disable SRC_Clock_Enbale bit
                rtd_maskl(GET_VGIP_CHNx_CTRL_VADDR(), ~_BIT0,0x0);
                // 2nd determine the input source TMDS(0), ANALOG mode(0)
                rtd_maskl(GET_VGIP_CHNx_CTRL_VADDR(), 0x0ffffffd, (0 <<28) | ((0 & 0x01)<<1));
                // 3rd enable SRC_Clock_Enbale bit and sample enable
#if HDMI_DE_ONLY
                rtd_maskl(GET_VGIP_CHNx_CTRL_VADDR(), ~(_BIT31| _BIT21 | _BIT16 | _BIT18 |_BIT0), (_BIT31| _BIT16 |_BIT0));
                if (drvif_Hdmi_GetInterlace())
                    rtd_maskl(GET_VGIP_CHNx_CTRL_VADDR(), ~(_BIT21 | _BIT22), _BIT21);
#else
                rtd_maskl(GET_VGIP_CHNx_CTRL_VADDR(), ~(_BIT31| _BIT21 | _BIT16 | _BIT18 |_BIT0), (_BIT31 |_BIT0));
#endif
                rtd_outl(GET_VGIP_CHNx_DELAY_VADDR(), 0);
//						SET_HDMI_AUDIO_FSM(AUDIO_FSM_AUDIO_START);	//tyh
                SET_HDMI_VIDEO_FSM(MAIN_FSM_HDMI_SETUP_VEDIO_PLL);  // reset Check mode state to initial
                HDMI_MES_FAIL_COUNT = 0;
            }
            else
            {
                HDMI_MES_FAIL_COUNT++;
                if (HDMI_MES_FAIL_COUNT >3 )
                {
                    Force_HDMIPHY_RST = 1;
                    HDMI_MES_FAIL_COUNT = 0;
                    HDMI_PRINTF(LOGGER_INFO,"Force reset phy\n");
                }
                SET_HDMI_VIDEO_FSM(MAIN_FSM_HDMI_SETUP_VEDIO_PLL);  // reset Check mode state to initial

            }

        }

//			20100429 TYH modify for some splitters compliance issue (to check clock again)
#if HDMI_Patch_For_Splitter
        detect_timeout = 0;
        while (!IsHDMI() && (detect_timeout<200))
        {
            HDMI_DELAYMS(5);
            detect_timeout++;

        }
        HDMI_PRINTF(LOGGER_INFO,"========||||||||=============||||||======hdmidetecttime=%d\n",detect_timeout);
#else
        HDMI_DELAYMS(150);
#endif

        HdmiGetStruct(&isr_info);
        HDMI_PRINTF(LOGGER_INFO,"isr_info.b_1 = %x\n", (UINT32)isr_info.b);
        if (HDMI_ABS(hdmi.b, isr_info.b) > 4)
        {
            HDMI_PRINTF(LOGGER_INFO,"b change_1\n");
            SET_HDMI_VIDEO_FSM(MAIN_FSM_HDMI_SETUP_VEDIO_PLL);
            result = _MODE_NOSIGNAL;
            break;

        }
        if (isr_info.b_change)
        {
            HDMI_PRINTF(LOGGER_INFO,"b change_1\n");
            SET_HDMI_VIDEO_FSM(MAIN_FSM_HDMI_SETUP_VEDIO_PLL);
            result = _MODE_NOSIGNAL;
            break;

        }
        if (GET_HDMI_ISINTERLACE()!= Hdmi_GetInterlace())     // if interlace mode change
        {
            HDMI_PRINTF(LOGGER_INFO,"interlace change_1\n");
            SET_HDMI_VIDEO_FSM(MAIN_FSM_HDMI_SETUP_VEDIO_PLL);
            result = _MODE_NOSIGNAL;
            break;
        }
        if (GET_ISHDMI() != IsHDMI())
        {
            HDMI_PRINTF(LOGGER_INFO,"HDMI/DVI change_1\n");
            SET_HDMI_VIDEO_FSM(MAIN_FSM_HDMI_SETUP_VEDIO_PLL);
            result = _MODE_NOSIGNAL;
            break;
        }
//			20100429 TYH modify end

        break;
    default:
        SET_HDMI_VIDEO_FSM(MAIN_FSM_HDMI_SETUP_VEDIO_PLL);
        break;
    }
//	}
    return result;
}




/**
 * drvif_Hdmi_AudioModeDetect
 * Audio main FSM
 *
 * @param 				{ }
 * @return 				{ reserve for future use }
 * @ingroup drv_adc
 */
HDMI_BOOL drvif_Hdmi_AudioModeDetect(void)
{

    UINT8 result = FALSE;
    UINT8 value1,value2;
    int i;
    HDMI_AUDIO_FREQ_T t;
    HDMI_AUDIO_TRACK_MODE track_mode;
    //UINT32 cts,n,b;
    //HDMIRX_IOCTL_STRUCT_T isr_info;

    rtdf_pageSelect(2);
    if ( (hdmi_in(HDMI_HDMI_SR_VADDR) & _BIT6)&& (!(hdmiport_in(HDMI_HDMI_BCSR_VADDR) & _BIT0)) )
    {
        HDMI_PRINTF(LOGGER_INFO,"Audio Detect AV Mute\n");
        Hdmi_AudioOutputDisable();
        SET_HDMI_AUDIO_FSM(AUDIO_FSM_AUDIO_START);
        return FALSE;
    }

    // Switch to Page 2 referenced from spec
    rtdf_pageSelect(2);

//	HDMI_PRINTF(LOGGER_INFO,"======> Dcode0/1=%x, %x\n", (UINT32)hdmiport_in(HDMI_HDMI_DPCR4_VADDR), (UINT32)hdmiport_in(HDMI_HDMI_DPCR5_VADDR));
//	HDMI_PRINTF(LOGGER_INFO,"======> FIFO at rising/falling=%d, %d\n", (UINT32)hdmiport_in(HDMI_HDMI_FDDR_VADDR), (UINT32)hdmiport_in(HDMI_HDMI_FDDF_VADDR));

    switch (GET_HDMI_AUDIO_FSM())
    {
    case AUDIO_FSM_AUDIO_START:
    {
        HDMI_PRINTF(LOGGER_INFO,"AUDIO_FSM_AUDIO_START\n");//ZZD
        //clear Overflow,Underflow,phase_non_lock, auto load double buffter
        hdmiport_out(HDMI_HDMI_DBCR_VADDR , 0x00);
        /*Disable audio watch dog*/
        hdmiport_mask(HDMI_HDMI_WDCR1_VADDR ,((UINT8)~(UINT8) _BIT7), 0x00);	//disable  tmds clock audio watch dog
        hdmiport_mask(HDMI_HDMI_WDCR0_VADDR , ~(_BIT1 |_BIT2|_BIT3 | _BIT4 | _BIT7 ),0x00);
        hdmi_out(HDMI_HDMI_SR_VADDR, hdmi_in(HDMI_HDMI_SR_VADDR));			//clear status

        /*Disable FIFO trend tracking*/
        hdmiport_out(HDMI_HDMI_PSCR_VADDR,0xE2);
        /*Update Double Buffer*/
        hdmiport_out(HDMI_HDMI_CMCR_VADDR,0x50);   //K code =2
        if (Hdmi_WaitAudioSample() == FALSE)
            break;
        SET_HDMI_AUDIO_FSM(AUDIO_FSM_FREQ_DETECT);
    }
//		break;	//tyh
    case AUDIO_FSM_FREQ_DETECT:
    {
        HDMI_PRINTF(LOGGER_INFO,"AUDIO_FSM_FREQ_DETECT\n");
        if (HDMI_AUDIO_IS_LPCM())
        {
            Hdmi_GetAudioFreq(&t, &track_mode);
            if (t.ACR_freq != 0)
            {
                if (Hdmi_AudioPLLSetting(t.ACR_freq, track_mode) == TRUE)
                {
                    SET_HDMI_AUDIO_FSM(AUDIO_FSM_AUDIO_START_OUT);
                    HDMI_DELAYMS(100);
                }
            }
            else
            {
//adams sync pacific,20110513
                if (t.ACR_freq == 0)
                {//cts = 0,use t.SPDIF_freq and force to trend_boundary tracking
                    if (Hdmi_AudioPLLSetting(t.SPDIF_freq, HDMI_AUDIO_TREND_BOUND) == TRUE)
                    {
                        SET_HDMI_AUDIO_FSM(AUDIO_FSM_AUDIO_START_OUT);
                        HDMI_DELAYMS(100);
                    }
                    //Hdmi_AudioPLLSetting(48000, 0);
//						hdmiport_out(HDMI_HDMI_AAPNR_VADDR, 0x08);	// Disable SDM
                    /*Disable N/CTS tracking*/
//						hdmiport_out(HDMI_HDMI_PSCR_VADDR,0x02);
                    /*Update Double Buffer*/
//						hdmiport_out(HDMI_HDMI_CMCR_VADDR,0x50);   //K code =2
//						break;	//tyh
                }
            }
        }
        else
        {				// Non-LPCM don't support
            break;
        }
    }
    // Play audio here
    case AUDIO_FSM_AUDIO_START_OUT:
    {
        HDMI_PRINTF(LOGGER_INFO,"AUDIO_FSM_AUDIO_START_OUT\n");
        if (HDMI_AUDIO_IS_LPCM() )
        {
        }
        else   // Dragon don't support NON-LPCM
        {
            SET_HDMI_AUDIO_FSM(AUDIO_FSM_AUDIO_START);
        }
        hdmiport_mask(HDMI_HDMI_AVMCR_VADDR ,~ (_BIT4 | _BIT5|_BIT6),_BIT5|_BIT6);//Enable Audio Output

//adams mask for descrease time,20110415
#if 0
        for (i=0; i<50; i++)
        {

            if (hdmi_in(HDMI_HDMI_SR_VADDR) & (_BIT1|_BIT2))
            {
                hdmi_mask(HDMI_HDMI_SR_VADDR, ~(_BIT1|_BIT2), (_BIT1|_BIT2));
//					HDMI_PRINTF(LOGGER_INFO,"Audio PLL not ready %d\n",(UINT32)i);
            }
            else
            {
            }
            HDMI_DELAYMS(10);
        }
#endif
//adams add for wait PLL tracking time,20110415
#if 1
		//jokerxie
        Ds_DelayXms(100);//HDMI_DELAYMS(100);		//PLL FIFO tacking time
        hdmi_mask(HDMI_HDMI_SR_VADDR, ~(_BIT1|_BIT2), (_BIT1|_BIT2));	//clear FIFO over&underflow status
        Ds_DelayXms(20);//HDMI_DELAYMS(20);
#endif
        if (hdmi_in(HDMI_HDMI_SR_VADDR) & (_BIT1|_BIT2))  	//check PLL FIFO status
        {
            HDMI_PRINTF(LOGGER_INFO,"Audio PLL not ready\n");
            HDMI_LOG(LOGGER_INFO, "HDMI_HDMI_SR_VADDR = %x\n", (UINT32)hdmi_in(HDMI_HDMI_SR_VADDR));
//				SET_HDMI_AUDIO_FSM(AUDIO_FSM_FREQ_DETECT);
            SET_HDMI_AUDIO_FSM(AUDIO_FSM_AUDIO_START);
#if 0	//adams add for audio test,20110610
            for (i=0 ; i<20 ; i++)
            {
                hdmiport_out(HDMI_HDMI_PTRSV2_VADDR, 0x01);		// Wait for ACR : Packet Type = 0x01
                HdmiGetStruct(&isr_info);
                b = isr_info.b;
                HDMI_DELAYMS(20);
                cts = Hdmi_PacketSRAMRead(113)<<16;
                cts = cts|Hdmi_PacketSRAMRead(114)<<8;
                cts = cts|Hdmi_PacketSRAMRead(115);
                n = Hdmi_PacketSRAMRead(116)<<16;
                n = n|Hdmi_PacketSRAMRead(117)<<8;
                n = n|Hdmi_PacketSRAMRead(118);
                HDMI_PRINTF( LOGGER_INFO,"CTS = %d  N = %d  \n",(UINT32)cts,(UINT32)n);
                HDMI_PRINTF(LOGGER_INFO,"FIFO Check  = %x\n", (UINT32)hdmi_in(HDMI_HDMI_SR_VADDR));
                HDMI_PRINTF(LOGGER_INFO, "b=%d \n",(UINT32)b);
            }
#endif
            return FALSE;
        }
#if HDMI_DEBUG_AUDIO_PLL_RING
        HdmiAudioPLLSampleDump();
#endif

        hdmiport_mask(HDMI_HDMI_DBCR_VADDR , 0xF0, 0x0F);

        value1 = hdmiport_in(HDMI_HDMI_DPCR4_VADDR);
        value2 = hdmiport_in(HDMI_HDMI_DPCR5_VADDR);

        for (i=0; i<5; i++)
        {

            if (value1 == hdmiport_in(HDMI_HDMI_DPCR4_VADDR)) break;
            value1 = hdmiport_in(HDMI_HDMI_DPCR4_VADDR);
            value2 = hdmiport_in(HDMI_HDMI_DPCR5_VADDR);
        }

        hdmiport_out(HDMI_HDMI_DCAPR0_VADDR, value1);
        hdmiport_out(HDMI_HDMI_DCAPR1_VADDR, value2);
        //hdmiport_out(HDMI_HDMI_PSCR_VADDR,0xE2);		//Disable N/CTS tracking & FIFO depth
        hdmiport_mask(HDMI_HDMI_PSCR_VADDR,~(_BIT4|_BIT3|_BIT2),0x0);//Set "Disable N/CTS tracking & FIFO depth" in first buffer

        //Enable audio Overflow & Underflow watch dog but not Audio type wdg
        hdmiport_mask(HDMI_HDMI_WDCR0_VADDR ,(UINT8)(~(_BIT1|_BIT2|_BIT3 | _BIT4|_BIT7)), (UINT8)(_BIT1|_BIT2|_BIT3 | _BIT4|_BIT7));
//			hdmiport_mask(HDMI_HDMI_WDCR0_VADDR ,(UINT8)(~(_BIT1|_BIT2|_BIT3 | _BIT4)), (UINT8)_BIT3 | _BIT4);
        hdmiport_mask(HDMI_HDMI_WDCR1_VADDR , (UINT8)(~_BIT7),(UINT8) _BIT7);//Enable audio tmds clock  watch dog
        hdmiport_mask(HDMI_HDMI_AOCR_VADDR,(UINT8)(~(0x0ff)),0xff);//Enable SPDIF/I2S Output
        SET_HDMI_AUDIO_FSM(AUDIO_FSM_AUDIO_CHECK);
#if 0
        HDMI_PRINTF(LOGGER_INFO,"=======================================\n");
        HDMI_LOG(LOGGER_INFO, "HDMI_HDMI_AVMCR_VADDR = %x\n",  (UINT32)hdmiport_in(HDMI_HDMI_AVMCR_VADDR));
        HDMI_LOG(LOGGER_INFO, "HDMI_HDMI_SR_VADDR = %x\n", (UINT32)hdmi_in(HDMI_HDMI_SR_VADDR));
        HDMI_LOG(LOGGER_INFO, "HDMI_HDMI_AFCR_VADDR = %x\n", (UINT32)hdmiport_in(HDMI_HDMI_AFCR_VADDR));
        HDMI_PRINTF(LOGGER_INFO,"=======================================\n");
#endif
        result = TRUE;
    }
//		break;

    case AUDIO_FSM_AUDIO_CHECK:
    {
        HDMI_LOG(LOGGER_INFO,"AUDIO_FSM_AUDIO_CHECK\n");
        rtdf_pageSelect(2);
        // if FIFO overflow then restart Audio process
#if 0	//adams mask 20110616, solve audio start mute immediate
        if (hdmi_in(HDMI_HDMI_SR_VADDR) & (_BIT1|_BIT2))
        {
            i = 0;
            while (hdmi_in(HDMI_HDMI_SR_VADDR) & (_BIT1|_BIT2))
            {
                hdmi_mask(HDMI_HDMI_SR_VADDR, ~(_BIT1|_BIT2), (_BIT1|_BIT2));
                i++;
                if (i>10)
                {
                    HDMI_PRINTF(LOGGER_INFO, "Audio clear FLAG %x\n",(UINT32)hdmi_in(HDMI_HDMI_SR_VADDR));
                    break;
                }
                HDMI_DELAYMS(10);
            }
            if (i<10)
                break;
#else
        if (hdmi_in(HDMI_HDMI_SR_VADDR) & (_BIT1|_BIT2))
        {
#endif
            Hdmi_AudioOutputDisable();
            HDMI_PRINTF(LOGGER_INFO, "Audio Output Disable cause by overflow %x\n",(UINT32)hdmi_in(HDMI_HDMI_SR_VADDR));
            SET_HDMI_AUDIO_FSM(AUDIO_FSM_AUDIO_START);
            //break;
        }
        if (HDMI_AUDIO_IS_LPCM() == 0)  		// if TX change audio mode to non-LPCM
        {
            Hdmi_AudioOutputDisable();
            HDMI_PRINTF(LOGGER_INFO, "Audio Output Disable cause non-Linear PCM\n");
            SET_HDMI_AUDIO_FSM(AUDIO_FSM_AUDIO_START);
            break;
        }
        if ((hdmiport_in(HDMI_HDMI_AFCR_VADDR) & 0x7)!=0x06)
        {
            Hdmi_AudioOutputDisable();
            HDMI_PRINTF(LOGGER_INFO,"Audio FIFO sfail %x\n", (UINT32)hdmiport_in(HDMI_HDMI_AFCR_VADDR));
            HDMI_LOG(LOGGER_INFO, "HDMI_HDMI_SR_VADDR = %x\n", (UINT32)hdmi_in(HDMI_HDMI_SR_VADDR));
            SET_HDMI_AUDIO_FSM(AUDIO_FSM_AUDIO_START);
            break;
        }


    }
    break;
    case AUDIO_FSM_AUDIO_WAIT_PLL_READY:
    {
        if (hdmi_in(HDMI_HDMI_SR_VADDR) & (_BIT1|_BIT2))
            hdmi_mask(HDMI_HDMI_SR_VADDR, ~(_BIT1|_BIT2), (_BIT1|_BIT2));

    }
    break;
    default:
    {
        SET_HDMI_AUDIO_FSM(AUDIO_FSM_AUDIO_START);
    }
    break;

    }

    return result;
}




/*

	function for EDID/Hotplug/HDCP Key Registration

*/
/*	not used Willy
HDMI_BOOL drvif_Hdmi_regTable(HDMI_TABLE_TYPE index, void *ptr)
{

	if ( !ptr )
		return FALSE;

	HDMI_PRINTF(LOGGER_INFO,"drvif_Hdmi_regTable %d %x\n", index, (UINT32) ptr);
	switch ( index )
	{
		case HDMI_HDCP_KEY_TABLE:
			hdmi.hdcpkey = (HDCP_KEY_T *) ptr;
			break;
		case HDMI_CHANNEL0:
			hdmi.channel[0] = (HDMI_CHANNEL_T *) ptr;
			break;
		case HDMI_CHANNEL1:
			hdmi.channel[1] = (HDMI_CHANNEL_T *) ptr;
			break;
		case HDMI_CHANNEL2:
			hdmi.channel[2] = (HDMI_CHANNEL_T *) ptr;
			break;

		default:

			return FALSE;
	}
	return TRUE;
}
*/




#endif
