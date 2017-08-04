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
* @{
*/

/*==================== Definitions ================= */

#include "source.h"
#include "flow_struct.h"
#include "signal\sig_ypbpr.h"
#include "signal\sig_vga.h"
#include "signal\sig_avsv.h"
#include "signal\sig_scart.h"
#include "signal\sig_tv.h"
#include "signal\sig_hdmi.h"
#include "scaler_idomain\vgip.h"
#include "flow_audio.h"
#include <string.h>	/* Provides: memcpy() */
#include <rosprintf.h>
#include "mode.h"
#include "video\video.h"
#include "pcb_customer.h"
#include "pcb\pcb_utility.h"
#include "autoconf.h"
#if 1//CONFIG_ENABLE_USB
#include "ui_ctrl.h"
#include "reg52.h"
#include "i2c\i2c.h"
#endif


#ifdef CONFIG_VBI_CC_SUPPORT
extern BOOL Get_CC_Status();
extern void Set_CC_Status(BOOL on);
extern void Disable_CC_Input();
#endif
/*==================== Definitions ================ */
StructSourceInput *ptSrcInput;
#if CONFIG_ENABLE_USB
void Send_IR(UINT16 usCode)
{
	UINT8 idata i;
	UINT16 idata usCC = 0xff00;//客户码
	#if CONFIG_ENABLE_USB
	static UINT32 temp;
	#endif
	//UINT16 idata usTemp = (usCC & 0xFF) << 8;

	//usCC = (usCC >>8) | usTemp;

	temp=rtd_inl(GPDATO_0x2584);
	//rtd_outl(GPDATO_0x2584, temp|0x0020);
	//Delay5ms(200);
	//rtd_outl(GPDATO_0x2584, temp&(~0x0020));

	EA = 0;
	// IR Start
	rtd_outl(GPDATO_0x2584, temp&(~0x0020));
	DelayStartLow();
	rtd_outl(GPDATO_0x2584, temp|0x0020);
	DelayStartHight();

	// Send Custom Code
	for( i = 0; i< 16; i++)
	{
		rtd_outl(GPDATO_0x2584, temp&(~0x0020));
		DelayBit0();
		rtd_outl(GPDATO_0x2584, temp|0x0020);

		if(usCC & 0x0001)    
			DelayBit1();
		else                 
			DelayBit0();

		usCC = usCC >> 1;
	}
	
	// Send Key Code
	for(i=0;i<16;i++)
	{
		rtd_outl(GPDATO_0x2584, temp&(~0x0020));
		DelayBit0();

		rtd_outl(GPDATO_0x2584, temp|0x0020);
		if(usCode & 0x0001)
			DelayBit1();
		else
			DelayBit0();

		usCode = usCode >> 1;
	}

	rtd_outl(GPDATO_0x2584, temp&(~0x0020));
	DelayBit0();

	rtd_outl(GPDATO_0x2584, temp|0x0020);

	EA = 1;
}
#endif
#if  CONFIG_ENABLE_USB
UINT8 g_EnterUsb=0;
UINT16 g_cnt_2=0;
UINT16 g_cnt_1=0;

UINT8 g_EnterDvd=0;


#endif
#if CONFIG_TU300
void Send_IR_DTV(UINT16 usCode)
{
	UINT8 idata i;
	UINT16 idata usCC = 0xff00;//客户码
	static UINT32 temp;
	//UINT16 idata usTemp = (usCC & 0xFF) << 8;

	//usCC = (usCC >>8) | usTemp;

	temp=rtd_inl(GPDATO_0x2584);
	//rtd_outl(GPDATO_0x2584, temp|0x0020);
	//Delay5ms(200);
	//rtd_outl(GPDATO_0x2584, temp&(~0x0020));

	EA = 0;
	// IR Start
	rtd_outl(GPDATO_0x2584, temp&(~0x0010));
	DelayStartLow();
	rtd_outl(GPDATO_0x2584, temp|0x0010);
	DelayStartHight();

	// Send Custom Code
	for( i = 0; i< 16; i++)
	{
		rtd_outl(GPDATO_0x2584, temp&(~0x0010));
		DelayBit0();
		rtd_outl(GPDATO_0x2584, temp|0x0010);

		if(usCC & 0x0001)	 
			DelayBit1();
		else				 
			DelayBit0();

		usCC = usCC >> 1;
	}
	
	// Send Key Code
	for(i=0;i<16;i++)
	{
		rtd_outl(GPDATO_0x2584, temp&(~0x0010));
		DelayBit0();

		rtd_outl(GPDATO_0x2584, temp|0x0010);
		if(usCode & 0x0001)
			DelayBit1();
		else
			DelayBit0();

		usCode = usCode >> 1;
	}

	rtd_outl(GPDATO_0x2584, temp&(~0x0010));
	DelayBit0();

	rtd_outl(GPDATO_0x2584, temp|0x0010);

	EA = 1;
}
#if 0
{
	UINT8 idata i;
	UINT16 idata usCC = 0xef10;//0xff00;//客户码
	#if CONFIG_ENABLE_USB
	static UINT32 temp;
	#endif
	//UINT16 idata usTemp = (usCC & 0xFF) << 8;

	//usCC = (usCC >>8) | usTemp;
	
	temp=rtd_inl(GPDATO_0x2584);
	//rtd_outl(GPDATO_0x2584, temp|0x0020);
	//Delay5ms(200);
	//rtd_outl(GPDATO_0x2584, temp&(~0x0020));

	EA = 0;
	// IR Start
	rtd_outl(GPDATO_0x2584, temp&(~0x0010));
	DelayDtvStartLow();
	rtd_outl(GPDATO_0x2584, temp|0x0010);
	DelayDtvStartHight();

	// Send Custom Code
	for( i = 0; i< 16; i++)
	{
		rtd_outl(GPDATO_0x2584, temp&(~0x0010));
		DelayDtvBit0();
		rtd_outl(GPDATO_0x2584, temp|0x0010);

		if(usCC & 0x0001)    
			DelayDtvBit1();
		else                 
			DelayDtvBit0();

		usCC = usCC >> 1;
	}
	
	// Send Key Code
	for(i=0;i<16;i++)
	{
		rtd_outl(GPDATO_0x2584, temp&(~0x0010));
		DelayDtvBit0();

		rtd_outl(GPDATO_0x2584, temp|0x0010);
		if(usCode & 0x0001)
			DelayDtvBit1();
		else
			DelayDtvBit0();

		usCode = usCode >> 1;
	}

	rtd_outl(GPDATO_0x2584, temp&(~0x0010));
	DelayDtvBit0();

	rtd_outl(GPDATO_0x2584, temp|0x0010);

	EA = 1;
}
#endif
#endif
extern UINT8 g_power_flag;
extern UINT8 g_power_dtvflag;

void flow_source_Init(void)
{
    // <Janus>2009/08/06: Because src_info[] can only be accessed by codes in bank#0,
    //      to ensure codes in other bank can access src_info[] as well, we place a working
    //      copy of src_info[] in xdata: tmpSrc_info[].
    UINT16 srcType;
    UINT8 dataBuf[8];
    StructSourceInput tmpSrc_info;
    srcType = GET_SRC_TYPE(info->input_src);

    memcpy( &tmpSrc_info, &(src_info[info->input_src]), sizeof(StructSourceInput) );
    ptSrcInput = &(tmpSrc_info);
    flow_Get_SrcInput_InitOptions(info->input_src, dataBuf, 8);

    flow_audio_selectSource(ptSrcInput);
//	flow_audio_selectSource(GET_INPUTSOURCE_TYPE());
    RTD_Log(LOGGER_INFO, "[SOURCE] info->input_src == %x\n", (UINT32)info->input_src);
			
#if  CONFIG_ENABLE_USB	
	STB_LOADINGPIN(1);
#endif
#if CONFIG_TU300
		  // if(GET_INPUTSOURCE_INDEX()== _SOURCE_INDEX_AV_1)
		   if(g_power_dtvflag)
		   	{
			Send_IR_DTV((((~TV_POWER_CODE)<<8)|TV_POWER_CODE));
			g_power_dtvflag=0;
		   	}
#endif

    switch (GET_INPUTSOURCE_TYPE())
    {
    case _SOURCE_YPBPR:
        RTD_Log(LOGGER_INFO, "[SOURCE] YPBPR\n");
        drv_sig_ypbpr_Init();
		
#if  CONFIG_ENABLE_USB	
		//RTD_Log(LOGGER_INFO, "[MELON] g_EnterUsb == %x\n", (UINT32)g_EnterUsb);
		//RTD_Log(LOGGER_INFO, "[MELON] g_EnterDvd == %x\n", (UINT32)g_EnterDvd);
		if(GET_INPUTSOURCE_INDEX()== _SOURCE_INDEX_USB)
		{
		 		
			STB_LOADINGPIN(0);
			//if(g_LastStatusNoDVD==1)
			//g_cnt_2=400;
			//else
			//RTD_Log(LOGGER_INFO, "[MELON--------SOURCE] USB\n");
			//if(!g_EnterUsb)
			#if ENABLE_OPENDISPLAY_CTL
			
		    //if(g_EnterDvd==1)
			Send_IR((((~IR_89JDVD_OPEN_CMD)<<8)|IR_89JDVD_OPEN_CMD));
			//RTD_Log(LOGGER_INFO, "[MELON--------SOURCE] USB\n");
			#endif
			{
				g_cnt_2=2000;//g_cnt_2=0X5FFF;//Send_IR((((~IR_89JDVD_USB_SD)<<8)|IR_89JDVD_USB_SD));
						
			}
			if((g_EnterDvd==0)&&(g_EnterUsb==0))
			  g_cnt_2=5000;//g_cnt_2=1000;
			 g_EnterUsb=1;
			 g_EnterDvd=0;
   		 	
		}
		else	
		{
		   g_cnt_2=0;
		   //RTD_Log(LOGGER_INFO, "[MELON--------SOURCE] DVD\n");
		#if 1
		 if((g_EnterUsb))
		 	{
		 	//RTD_Log(LOGGER_INFO, "[MELON--------SOURCE] DVD\n");	
		 	Send_IR((((~IR_89JDVD_USB_SD)<<8)|IR_89JDVD_USB_SD));
			//Send_IR((((~IR_89JDVD_ENTER)<<8)|IR_89JDVD_ENTER));
			 
		 	}
		 #endif
		 
		    g_EnterDvd=1;
		   g_EnterUsb=0;
	        	
			
			
		}
		
#endif
#if  0//CONFIG_ENABLE_USB
	  	if(GET_INPUTSOURCE_INDEX()== _SOURCE_INDEX_USB)
  		{
  			g_cnt=400;
  		}
#endif

        break;
	

    case _SOURCE_VGA:
        RTD_Log(LOGGER_INFO, "[SOURCE] VGA\n");
        drv_sig_vga_Init();
        break;
        
#ifdef _VIDEO_TV_SUPPORT
    case _SOURCE_TV:
        RTD_Log(LOGGER_INFO, "[SOURCE] TV\n");
        drv_sig_tv_Init(dataBuf);
        break;
#endif

    case _SOURCE_CVBS:
    case _SOURCE_SV:
//		case _SOURCE_SCART_AV:	//jj 960505
//		case _SOURCE_SCART_SV:	//jj 960505
#ifdef CONFIG_SCART_AUTO_SWITCH
    case _SOURCE_FULL_SCART:
    case _SOURCE_HALF_SCART:
#endif
//			drv_sig_avsv_Init(ptSrcInput);
		//g_EnterUsb=0;
		//if(g_EnterDvd)
		#if  CONFIG_ENABLE_USB	
		g_EnterDvd=0;
		g_EnterUsb=0;
		#endif
		
 #if 0//CONFIG_TU300
		if(GET_INPUTSOURCE_INDEX()== _SOURCE_INDEX_AV_1)
		 Send_IR_DTV((((~TV_POWER_CODE)<<8)|TV_POWER_CODE));
 #endif
 #if CONFIG_ENABLE_USB
		STB_DVDPWR_CTRL(0);
 #endif
        RTD_Log(LOGGER_INFO, "[SOURCE] AV/SV/SCART\n");
        drv_sig_avsv_Init(GET_INPUTSOURCE_TYPE(), dataBuf);
		//g_cnt_1=5000;
        break;

#ifdef CONFIG_ENABLE_HDMI
    case _SOURCE_HDMI:
		//g_EnterUsb=0;
		//if(g_EnterDvd)
		#if  CONFIG_ENABLE_USB	
		g_EnterDvd=0;
		g_EnterUsb=0;
		
		STB_DVDPWR_CTRL(0);
		#endif
        RTD_Log(LOGGER_INFO, "[SOURCE] HDMI\n");
//			drv_sig_hdmi_Init(ptSrcInput);
        drv_sig_hdmi_Init();
		//g_cnt_1=5000;	

        break;
#endif

    default:
        break;
    };
}

UINT8 flow_source_DetectMode(void)
{
    ptSrcInput = &(src_info[info->input_src]);
    switch (GET_INPUTSOURCE_TYPE())
    {
    case _SOURCE_YPBPR:
        return drv_sig_ypbpr_DetectMode(ptSrcInput);

    case _SOURCE_VGA:
        return drv_sig_vga_DetectMode(ptSrcInput);

#ifdef _VIDEO_TV_SUPPORT
    case _SOURCE_TV:
        return drv_sig_tv_DetectMode();
#endif

    case _SOURCE_CVBS:
    case _SOURCE_SV:
        return drv_sig_avsv_DetectMode();

//		case _SOURCE_SCART_AV:	//jj 960505
//		case _SOURCE_SCART_SV:	//jj 960505
#ifdef CONFIG_SCART_AUTO_SWITCH
    case _SOURCE_FULL_SCART:
    case _SOURCE_HALF_SCART:
        return drv_sig_scart_DetectMode();
#endif

#ifdef CONFIG_ENABLE_HDMI
    case _SOURCE_HDMI:
        return drv_sig_hdmi_DetectMode();
#endif
    default:
        break;
    };

    return 0;
}

bit flow_source_CheckMode(void)
{
    switch (GET_INPUTSOURCE_TYPE())
    {
    case _SOURCE_YPBPR:
        return drv_sig_ypbpr_CheckMode();

    case _SOURCE_VGA:
        return drv_sig_vga_CheckMode();

#ifdef _VIDEO_TV_SUPPORT
    case _SOURCE_TV:
        return drv_sig_tv_CheckMode();
#endif

    case _SOURCE_CVBS:
    case _SOURCE_SV:
        return drv_sig_avsv_CheckMode();

//		case _SOURCE_SCART_AV:	//jj 960505
//		case _SOURCE_SCART_SV:	//jj 960505
#ifdef CONFIG_SCART_AUTO_SWITCH
    case _SOURCE_FULL_SCART:
    case _SOURCE_HALF_SCART:
        return drv_sig_scart_CheckMode();
#endif

#ifdef CONFIG_ENABLE_HDMI
    case _SOURCE_HDMI:
        return drv_sig_hdmi_CheckMode();
#endif
    };
    return 0;
}

void flow_source_Inproc_Setup(void)
{
    switch (GET_INPUTSOURCE_TYPE())
    {
    case _SOURCE_YPBPR:
        drv_sig_ypbpr_SetupMode();
        break;

    case _SOURCE_VGA:
        drv_sig_vga_SetupMode();
        break;

#ifdef _VIDEO_TV_SUPPORT
    case _SOURCE_TV:
        drv_sig_tv_SetupMode();
        break;
#endif

    case _SOURCE_CVBS:
    case _SOURCE_SV:
//		case _SOURCE_SCART_AV:	//jj 960505
//		case _SOURCE_SCART_SV:	//jj 960505
#ifdef CONFIG_SCART_AUTO_SWITCH
    case _SOURCE_FULL_SCART:
    case _SOURCE_HALF_SCART:
#endif
        drv_sig_avsv_SetupMode();
        break;

#ifdef CONFIG_ENABLE_HDMI
    case _SOURCE_HDMI:
        drv_sig_hdmi_SetupMode();
        break;
#endif
    };

    // Store ivsdelay to IPV_DLY_PRE move to modestate
//	info->IPV_DLY_PRE = drv_vgip_GetVSDelay();
}

void flow_source_Release(void)
{
#ifdef CONFIG_VBI_CC_SUPPORT
    if (Get_CC_Status() == TRUE)
    {
        Disable_CC_Input();
        Set_CC_Status(FALSE);
    }
#endif
//	ptSrcInput = &(src_info[info->input_src]);
    switch (GET_INPUTSOURCE_TYPE())
    {
    
    case _SOURCE_YPBPR:
        drv_sig_ypbpr_Release();
        break;

    case _SOURCE_VGA:
        drv_sig_vga_Release();
        break;

#ifdef _VIDEO_TV_SUPPORT
    case _SOURCE_TV:
        drv_sig_tv_Release();
        break;
#endif

    case _SOURCE_CVBS:
    case _SOURCE_SV:
//		case _SOURCE_SCART_AV:	//jj 960505
//		case _SOURCE_SCART_SV:	//jj 960505
#ifdef CONFIG_SCART_AUTO_SWITCH
    case _SOURCE_FULL_SCART:
    case _SOURCE_HALF_SCART:
#endif
        drv_sig_avsv_Release();
        break;

#ifdef CONFIG_ENABLE_HDMI
    case _SOURCE_HDMI:
        drv_sig_hdmi_Release();
        break;
#endif
    };
}

UINT8 flow_UsePowerSaving(void)
{
    switch (GET_INPUTSOURCE_TYPE())
    {
    case _SOURCE_VGA:
    case _SOURCE_YPBPR:
    case _SOURCE_CVBS:
    #ifdef CONFIG_ENABLE_HDMI
    case _SOURCE_HDMI:
    #endif
    case _SOURCE_SV:
        //case _SOURCE_TV:
#ifdef CONFIG_SCART_AUTO_SWITCH
    case _SOURCE_HALF_SCART:
#endif
        return _TRUE;
    };
    return _FALSE;
}


UINT16 flow_Get_SrcInput_Parameter(UINT8 idx, UINT16 offset, UINT8 len)
{
    UINT16* ptVal;
    ptVal = (UINT16*)((UINT8*)&src_info[idx] + offset);
    if (len == 1)
    {
        return ((*ptVal & 0xff00) >> 8);
    }
    else if (len == 2)
    {
        return *ptVal;
    }
    else
        return 0;
}

bit flow_Get_SrcInput_InitOptions(UINT8 idx, UINT8* ptData, UINT8 len)
{
    UINT8 getVal;
    UINT8 strcut_len;
    UINT8 code *ptSrcInfo;
    if (ptData != NULL)
    {
        getVal = ((src_info[idx].src_type & 0xff00) >> 8);

        switch (getVal)
        {
        case _SOURCE_VGA:
        case _SOURCE_YPBPR:
            strcut_len = 1;
            break;

#ifdef CONFIG_ENABLE_HDMI
        case _SOURCE_HDMI:
            strcut_len = 0;
            break;
#endif
        case _SOURCE_TV:
        case _SOURCE_CVBS:
            strcut_len = 3;
            break;

        case _SOURCE_SV:
#ifdef CONFIG_SCART_AUTO_SWITCH
        case _SOURCE_FULL_SCART:
        case _SOURCE_HALF_SCART:
#endif
            strcut_len = 5;
            break;
        };

        if (len < strcut_len)
            return _FALSE;

        ptSrcInfo = src_info[idx].InitOptions;
        for (getVal = 0; getVal < strcut_len; getVal++)
        {
            *(ptData + getVal) = *(ptSrcInfo + getVal);
        }

        return _TRUE;
    }

    return _FALSE;
}


/**
 *
 * @}
 */
