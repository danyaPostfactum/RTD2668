#include "autoconf.h"

#ifdef CONFIG_ENABLE_HDMI

#ifndef SPARROW_HDMI_PLATFORM_H
#include <rtd_types.h>
#include <stdlib.h>
#include <sparrow.h>
//#include <hdmi\rbusHDMIReg.h>
#include <hdmi\Vgip_fw.h>
#include <hdmi\SyncProcessor_fw.h>
#include <scaler_idomain\syncproc.h>
#include <mode.h>
#include <reg_def.h>
#include <scaler_idomain\vgip.h>
#include <scaler_idomain\smartfit.h>
#include <vip/color.h>
#include <mode.h>
#include <mode_customer.h>
#include "rosprintf.h"
#include "scaler_idomain\measure.h"
#include "scaler_idomain\smartfit.h"
#include "pcb_customer.h"
#include "pcb\pcb_utility.h"
#include "hdmi\rbushdmiReg.h"
#include "system.h"
#include "timer_event.h"
/********************************************************************************************


						HDMI Driver Mode Config : important !!!!!!


********************************************************************************************/
#define HDMI_FIX_GREEN_LINE 		1  	   	// mask green line into black when measure result is error

#define HDMI_AUTO_DEONLY			1		// auto DE_ONLY mode decision flow
#define HDMI_DE_ONLY  			0		// manual control of DE_ONLY mode , avaliable when HDMI_AUTO_DEONLY = 0

#define HDMI_RUNTIME_AUDIO_PLL_CAL 0 		// Enable Runtime Audio PLL parameter caculation
#define HDMI_BULD_IN_TABLE 		   1		// 1: channel table define by code 0: load by drvif_Hdmi_regTable
#define HDMI_SUPPORT_NONPCM		   0		// 1: SUPPROT_NONPCM
#define HDMI_MAX_CHANNEL			   1
#define HDMI_DEBUG_AUDIO_PLL_RING   0		// enable Audio pll monitor ring
#define HDMI_RTD_XTAL  (27000UL)

#define LG_DF9921N					    1
extern UINT8 Force_HDMIPHY_RST;
//#define HDMI_PRINTF(string, args , x) 	RTD_Log(LOGGER_INFO, string , ##args)
//#define HDMI_PRINTF(...)
//#define HDMI_PRINTF				//
#define HDMI_PRINTF				RTD_Log
#if 0
#define HDMI_DELAYMS(x) 			fw_timer_event_DelayXms(x)
#else

void Audio_TimeDelay_ms(UINT16 us);
#define HDMI_DELAYMS(x) 		fw_timer_event_DelayXms(x)	// Audio_TimeDelay_ms(x)
#endif
#define HDMI_RTD_XTAL  (27000UL)

#define inline
#define __initdata
#define __init


extern ModeInformationType xdata stModeInfo;
extern UINT8 mode_identifymode(UINT8 mode_index);
// for portable code
#define GET_MODE_CURR()			(stModeInfo.ModeCurr)
#define GET_MODE_POLARITY()		(stModeInfo.Polarity)
#define GET_MODE_IVTOTAL()			(stModeInfo.IVTotal)
#define GET_MODE_IHTOTAL()			(stModeInfo.IHTotal)
#define GET_MODE_IVHEIGHT()		(stModeInfo.IVHeight)
#define GET_MODE_IHWIDTH()			(stModeInfo.IHWidth)
#define GET_MODE_IVSTA()			(stModeInfo.IVStartPos)
#define GET_MODE_IHSTA()			(stModeInfo.IHStartPos)
#define GET_MODE_IVFREQ()			(stModeInfo.IVFreq)
#define GET_MODE_IHFREQ()			(stModeInfo.IHFreq)
#define GET_MODE_IVCOUNT()			(stModeInfo.IVCount)
#define GET_MODE_IHCOUNT()			(stModeInfo.IHCount)

#define SET_MODE_CURR(x)			(stModeInfo.ModeCurr = (x))
#define SET_MODE_POLARITY(x)		(stModeInfo.Polarity = (x))
#define SET_MODE_IVTOTAL(x)		(stModeInfo.IVTotal = (x))
#define SET_MODE_IHTOTAL(x)		(stModeInfo.IHTotal = (x))
#define SET_MODE_IVHEIGHT(x)		(stModeInfo.IVHeight = (x))
#define SET_MODE_IHWIDTH(x)		(stModeInfo.IHWidth = (x))
#define SET_MODE_IVSTA(x)			(stModeInfo.IVStartPos = (x))
#define SET_MODE_IHSTA(x)			(stModeInfo.IHStartPos = (x))
#define SET_MODE_IVFREQ(x)			(stModeInfo.IVFreq = (x))
#define SET_MODE_IHFREQ(x)			(stModeInfo.IHFreq = (x))
#define SET_MODE_IVCOUNT(x)		(stModeInfo.IVCount = (x))
#define SET_MODE_IHCOUNT(x)		(stModeInfo.IHCount = (x))

#define SET_SCALER_POLARITY(x)			(info->Polarity = (x))
#define SET_SCALER_IVTOTAL(x)			(info->IVTotal = (x))
#define SET_SCALER_IHTOTAL(x)			(info->IHTotal = (x))
#define SET_SCALER_IVHEIGHT(x)			(info->IPV_ACT_LEN = (x))
#define SET_SCALER_IVHEIGHT_PRE(x)		(info->IPV_ACT_LEN_PRE = (x))
#define SET_SCALER_IHWIDTH(x)			(info->IPH_ACT_WID= (x))
#define SET_SCALER_IHWIDTH_PRE(x)		(info->IPH_ACT_WID_PRE = (x))
#define SET_SCALER_IVSTA(x)				(info->IPV_ACT_STA = (x))
#define SET_SCALER_IVSTA_PRE(x)		(info->IPV_ACT_STA_PRE = (x))
#define SET_SCALER_IHSTA(x)				(info->IPH_ACT_STA = (x))
#define SET_SCALER_IHSTA_PRE(x)		(info->IPH_ACT_STA_PRE = (x))
#define SET_SCALER_IVFREQ(x)			(info->IVFreq = (x))
#define SET_SCALER_IHFREQ(x)			(info->IHFreq = (x))
#define SET_SCALER_IVCOUNT(x)			(info->IVCount_PRE= (x))
#define SET_SCALER_IHCOUNT(x)			(info->IHCount_PRE = (x))
#define SET_SCALER_MODE_CURR(x)		(info->ucMode_Curr = (x))
#define SET_SCALER_ISINTERLACE(x)		if (x) {			\
											SET_INTERLACE_IN_INPUT_MODE();	\
										} else {	\
											CLR_INTERLACE_IN_INPUT_MODE();	\
										}

#define SET_SCALER_IS422(x)				if (x) {			\
											SET_422_FORMAT();	\
										} else {	\
											CLR_422_FORMAT();	\
										}

#define GET_SCALER_POLARITY()			(info->Polarity)
#define GET_SCALER_IVTOTAL()			(info->IVTotal)
#define GET_SCALER_IHTOTAL()			(info->IHTotal)
#define GET_SCALER_IVHEIGHT()			(info->IPV_ACT_LEN)
#define GET_SCALER_IVHEIGHT_PRE()		(info->IPV_ACT_LEN_PRE)
#define GET_SCALER_IHWIDTH()			(info->IPH_ACT_WID)
#define GET_SCALER_IHWIDTH_PRE()		(info->IPH_ACT_WID_PRE)
#define GET_SCALER_IVSTA()				(info->IPV_ACT_STA)
#define GET_SCALER_IVSTA_PRE()			(info->IPV_ACT_STA_PRE)
#define GET_SCALER_IHSTA()				(info->IPH_ACT_STA)
#define GET_SCALER_IHSTA_PRE()			(info->IPH_ACT_STA_PRE)
#define GET_SCALER_IVFREQ()				(info->IVFreq)
#define GET_SCALER_IHFREQ()				(info->IHFreq)
#define GET_SCALER_IVCOUNT()			(info->IVCount_PRE)
#define GET_SCALER_IHCOUNT()			(info->IHCount_PRE)
#define GET_SCALER_CHANNEL()			(0)
#define GET_SCALER_IS422()				(GET_422_FORMAT())

#define GET_SP_MS3CTRL_VADDR()				(SP_MS3CTRL_0x0800)
#define GET_SP_MS3RST0_VADDR()			(SP_MS3RST0_0x0804)
#define GET_SP_MS3RST1_VADDR()			(SP_MS3RST1_0x0808)
#define GET_SP_MS3STUS_VADDR()			(SP_MS3STUS_0x080c)

#define GET_SP_MSxCTRL_VADDR()				(SP_MS1CTRL_0x0900)
#define GET_SP_MSxSTUS_VADDR()			(SP_MS1STUS_0x090c)
#define GET_SP_MSxRST0_VADDR()			(SP_MS1RST0_0x0904)

#define GET_VGIP_CHNx_CTRL_VADDR()				(VGIP_CHN1_CTRL_0x0a10)
#define GET_VGIP_CHNx_V_CAP_VADDR()  				(VGIP_CHN1_ACT_VSTA_LENGTH_0x0a1c)
#define GET_VGIP_CHNx_H_CAP_VADDR()  				(VGIP_CHN1_ACT_HSTA_WIDTH_0x0a18)
#define GET_VGIP_CHNx_DELAY_VADDR()  				(VGIP_CHN1_DELAY_0x0a20)

#define GET_SFT_AUTO_FIELD_VADDR()				(AUTO_FIELD_0x0600)
#define GET_SFT_AUTO_H_BOUNDARY_VADDR()			(AUTO_H_BOUNDARY_0x0604)
#define GET_SFT_AUTO_V_BOUNDARY_VADDR()			(AUTO_V_BOUNDARY_0x0608)
#define GET_SFT_AUTO_ADJ_VADDR()					(AUTO_ADJ_0x0610)
#define GET_SFT_AUTO_RESULT_VSTA_END_VADDR() 	(AUTO_RESULT_VSTA_END_0x0614)
#define GET_SFT_AUTO_RESULT_HSTA_END_VADDR() 	(AUTO_RESULT_HSTA_END_0x0618)

#define GET_DDC_SIR_VADDR(x)				(DDC_SIR_0x2720+ (0x80 * x))
#define GET_DDC_SAP_VADDR(x)				(DDC_SAP_0x2724 + (0x80 * x))
#define GET_EDID_CR_VADDR(x)				(EDID_CR_0x270c + (0x80 * x))


#define fw_scaler_set_vgip 					drv_vgip_set_vgip
#define drvif_mode_enableonlinemeasure()  		drv_measure_enableonlinemeasure()
#define drvif_mode_onlinemeasure(x)		   	drv_measure_onlinemeasure()
#define HDMI_COPY 							flow_storage_memcpy

#define HDMI_DISABLE_TIMER_ISR()
#define HDMI_ENABLE_TIMER_ISR()

extern UINT8 hdmiport_in(UINT8 addr);
extern void hdmiport_out(UINT8 addr, UINT8 value);
extern void hdmiport_mask(UINT8 addr, UINT8 andmask, UINT8 ormask);

extern void fw_timer_event_DelayXms(UINT16 usNum);
extern UINT8 drv_measure_onlinemeasure(void);

#define HDMI_ABS(x, y)	 			((x > y) ? (x-y) : (y-x))

#define HDMI_CONST					code
#define HDMI_BOOL					bit


#define hdmi_in(addr)								rtdf_readScalerPort(((UINT16) (addr &0xFFFF)))
#define hdmi_out(addr, value)						rtdf_writeScalerPort(((UINT16) (addr &0xFFFF)), value)
#define hdmi_mask(addr, andmask, ormask) 			rtdf_maskScalerPort(((UINT16) (addr &0xFFFF)), andmask, ormask)

#define hdmi_isr_in(addr)								rtdf_isr_readScalerPort(((UINT16) (addr &0xFFFF)))
#define hdmi_isr_out(addr, value)						rtdf_isr_writeScalerPort(((UINT16) (addr &0xFFFF)), value)
#define hdmi_isr_mask(addr, andmask, ormask) 			rtdf_isr_maskScalerPort(((UINT16) (addr &0xFFFF)), andmask, ormask)

#endif
#endif
