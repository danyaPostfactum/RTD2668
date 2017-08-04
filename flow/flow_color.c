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
#include "flow_color.h"
#include "flow_struct.h"
#include "source.h"
#include "mode_customer.h"
#include "panel\panelapi.h"
#include "video\video.h"
#include "vip\color.h"
#include "vip\video_quality.h"
#include "vip\ultrazoom.h"
#include "vip\digital_filter.h"
#include "ui_struct.h"
#include "pcb_customer.h"
#include "pcb\pcb_utility.h"
#include "rosprintf.h"
/*================================== Variables ==============================*/
//for common use in vip flow
UINT8 xdata vip_flow_8bit_tmp[3];
UINT32 xdata vip_flow_32bit_tmp[2];

/*================================== Function ===============================*/
UINT8 flow_vip_Get_Input_Mode(void)
{
    if (GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
    {
        return _VGA_MODE_ALL;
    }
    else if (GET_INPUTSOURCE_TYPE() == _SOURCE_HDMI)
    {
        if ((GET_INPUT_MODE() == _HDMI_MODE_1080I25) || (GET_INPUT_MODE() == _HDMI_MODE_1080I30))
            return _HDMI_MODE_1080I;
        else if ((GET_INPUT_MODE() == _HDMI_MODE_1080P50) || (GET_INPUT_MODE() == _HDMI_MODE_1080P60))
            return _HDMI_MODE_1080P;
        else if ((GET_INPUT_MODE() == _HDMI_MODE_720P50) || (GET_INPUT_MODE() == _HDMI_MODE_720P60))
            return _HDMI_MODE_720P;
	else
        return _HDMI_MODE_1080P;
		
    }
    else if (GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR)
    {
        if ((GET_INPUT_MODE() == _MODE_1080I25) || (GET_INPUT_MODE() == _MODE_1080I30))
            return _MODE_1080I;
        else if ((GET_INPUT_MODE() == _MODE_1080P50) || (GET_INPUT_MODE() == _MODE_1080P60)
                 ||(GET_INPUT_MODE() == _MODE_1080PM50)||(GET_INPUT_MODE() == _MODE_1080PM60)
                 ||(GET_INPUT_MODE() == _MODE_1080P24)||(GET_INPUT_MODE() == _MODE_1080P25)||(GET_INPUT_MODE() == _MODE_1080P30))
            return _MODE_1080P;
        else if ((GET_INPUT_MODE() == _MODE_720P50) || (GET_INPUT_MODE() == _MODE_720P60))
            return _MODE_720P;
    }
    else if (GET_INPUTSOURCE_TYPE() == _SOURCE_CVBS)
    {
        if (GET_INPUT_MODE() == _MODE_480I)
            return _AV_MODE_480I;
        else if (GET_INPUT_MODE() == _MODE_576I)
            return _AV_MODE_576I;
    }
    else if (GET_INPUTSOURCE_TYPE() == _SOURCE_SV)
    {
        if (GET_INPUT_MODE() == _MODE_480I)
            return _SV_MODE_480I;
        else if (GET_INPUT_MODE() == _MODE_576I)
            return _SV_MODE_576I;
    }
    else if (GET_INPUTSOURCE_TYPE() == _SOURCE_TV)
    {
        if (GET_INPUT_MODE() == _MODE_480I)
            return _TV_MODE_480I;
        else if (GET_INPUT_MODE() == _MODE_576I)
            return _TV_MODE_576I;
    }
    else
        return GET_INPUT_MODE();
}

void flow_color_Transform_Format()
{
    // Enable / Disable 422To444
    drv_color_set422to444(GET_422_FORMAT());

    //color matrix setting
    vip_flow_8bit_tmp[0]=0;//nMode=0

    if (GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR)
    {
        if ((info->ucMode_Curr!= _MODE_480I)
                &&(info->ucMode_Curr!= _MODE_480P)
                &&(info->ucMode_Curr!= _MODE_576I)
                &&(info->ucMode_Curr!= _MODE_576P))
            vip_flow_8bit_tmp[0] = 1;
    }
    else if (GET_INPUTSOURCE_TYPE() == _SOURCE_HDMI)
    {
        if ((info->ucMode_Curr!= _HDMIMODE_480I)
                &&(info->ucMode_Curr!= _HDMIMODE_480P)
                &&(info->ucMode_Curr!= _HDMIMODE_576I)
                &&(info->ucMode_Curr!= _HDMIMODE_576P))
            vip_flow_8bit_tmp[0] = 1;
    }

    if (info->ucMode_Curr !=_HDMIMODE_NEW)
        vip_flow_8bit_tmp[1]=TRUE;

    drv_color_colorspacergb2yuvtransfer( GET_INPUTSOURCE_TYPE(), vip_flow_8bit_tmp[0], vip_flow_8bit_tmp[1]);
    drv_color_colorspaceyuv2rgbtransfer( GET_INPUTSOURCE_TYPE());

}

void flow_color_ultrazoom_SetScalingDown(void)
{
    UZ_Para SD;//weihauo help to set the value of the structure

    // Not use PANORAMA mode
    SD.panorama = IS_RATIO_PANORAMA();

    if (info->IPV_ACT_LEN > info->DispLen)
    {
        SD.InSize.nLength = info->IPV_ACT_LEN;
        SD.OutSize.nLength = info->DispLen;
        SD.VScale_En = 1;
    }
    else
    {
        SD.InSize.nLength = 0;
        SD.OutSize.nLength = 0;
        SD.VScale_En = 0;
    }

    if (info->IPH_ACT_WID > info->DispWid)
    {
        SD.InSize.nWidth = info->IPH_ACT_WID;
        SD.OutSize.nWidth = info->DispWid;
        SD.HScale_En = 1;
    }
    else
    {
        SD.InSize.nWidth = 0;
        SD.OutSize.nWidth = 0;
        SD.HScale_En = 0;
    }

    drvif_color_ultrazoom_config_scaling_down(&SD);
}

void flow_color_ultrazoom_SetScalingUp(void)
{
    UZ_Para SU;//weihauo help to set the value of the structure
    SU.panorama = IS_RATIO_PANORAMA();
    if (info->IPV_ACT_LEN < info->DispLen)
    {
        SU.InSize.nLength = info->IPV_ACT_LEN;
        SU.OutSize.nLength = info->DispLen;
        SU.VScale_En = 1;
    }
    else
    {
        SU.InSize.nLength = info->IPV_ACT_LEN;
        SU.OutSize.nLength = info->DispLen;
        SU.VScale_En = 0;
    }

    if (info->IPH_ACT_WID <  info->DispWid)
    {
        SU.InSize.nWidth = info->IPH_ACT_WID;
        SU.OutSize.nWidth = info->DispWid;
        SU.HScale_En = 1;
    }
    else
    {
        SU.InSize.nWidth = info->IPH_ACT_WID;
        SU.OutSize.nWidth =info->DispWid;
        SU.HScale_En = 0;
    }
    if (GET_INTERLACE_MODE())
        SU.Video_Com_En = 1;
    else
        SU.Video_Com_En = 0;
    drvif_color_ultrazoom_config_scaling_up(&SU);
}

void flow_color_DCR()
{
    if (GET_DCR_STATUS())
    {
        drv_color_DcrMeasureStartHD( info->DispLen, info->DispWid, (UINT16)(255*g_ucUserBacklight)/10);
    }
    else if (!GET_DCR_STATUS())
    {
        SET_PANEL_BACKLIGHT( (UINT16)(254*g_ucUserBacklight)/10 );//drv_color_DcrSlowAdjust( (UINT16)(255*g_ucUserBacklight)/10);
    }

#if 0//we use _DCR_TYPE_1 for the moment
#if(_DCR_MODE == _DCR_TYPE_3)

    if (g_ucDcrTimer > 1)    // 20mSec
    {
        drv_color_DcrDcr3();
        g_ucDcrTimer = 0;
    }

#endif
#endif

}

void flow_color_Handler(void)
{
    g_ucPreBacklightPWM = 255;
    //drv_color_setdithering(Get_DISPLAY_COLOR_BITS());//move to video_quality_handler

    //RTD_Log(LOGGER_INFO, "flow_color_Handler() finished !\n");
    /*
    	if(GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR || GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)	//only vga and ypbpr need to set digital filter(refer to Monitor's firmware)
    		drvif_color_digital_filter_config(_ENABLE);
    	else
    		drvif_color_digital_filter_config(_DISABLE);
    */

    //Load (128, 0, 0) as osd default value(linear curve)
    SET_S_INDEX(128);
    SET_S_LOW(0);
    SET_S_HIGH(0);

}

void flow_color_Video_Quality_Handler(void)
{
    VIP_QUALITY_COEF code *VIP_Quality_Coef;

    RTD_Log(LOGGER_INFO, "\n===================\nVIP_Quality_handler():\n\n");

    //to get data from vip table
    if ( drvif_color_VIP_Quality_Coef_GetValue(flow_vip_Get_Input_Mode(), &VIP_Quality_Coef) ==_FALSE)
    {
        RTD_Log(LOGGER_INFO, "VIP:VQH-idx err!\n");
        return;
    }
    else
    {
        RTD_Log(LOGGER_INFO, "I_DLTi=%d, I_DCTi=%d\nGamma=%d\nDigital_Filter=%d\n", (UINT32)VIP_Quality_Coef->I_DLTi, (UINT32)VIP_Quality_Coef->I_DCTi,(UINT32)VIP_Quality_Coef->Gamma, (UINT32)VIP_Quality_Coef->Digital_Filter);
        RTD_Log(LOGGER_INFO, "SUNR_Y=%d, SUNR_C=%d, SU_MASK=%d\n", (UINT32)VIP_Quality_Coef->SUNR_Y, (UINT32)VIP_Quality_Coef->SUNR_C, (UINT32)VIP_Quality_Coef->SU_MASK);
        RTD_Log(LOGGER_INFO, "DCC=%d\nD_DLTi=%d, D_DCTi=%d\nSU_H=%d, SU_V=%d\nSU_PK=%d\n", (UINT32)VIP_Quality_Coef->DCC, (UINT32)VIP_Quality_Coef->D_DLTi, (UINT32)VIP_Quality_Coef->D_DCTi, (UINT32)VIP_Quality_Coef->SU_H, (UINT32)VIP_Quality_Coef->SU_V, (UINT32)VIP_Quality_Coef->SU_PK);
        RTD_Log(LOGGER_INFO, "VD: HSBC=(%d, %d, %d, %d)\n", (UINT32)VIP_Quality_Coef->VD_Hue, (UINT32)VIP_Quality_Coef->VD_Sat, (UINT32)VIP_Quality_Coef->VD_Y_Brightness, (UINT32)VIP_Quality_Coef->VD_Y_Contrast);
        RTD_Log(LOGGER_INFO, "ICM_Table_Apply=%d\nDiter_Bit=%d\n", (UINT32)VIP_Quality_Coef->ICM_Table_Apply, (UINT32)VIP_Quality_Coef->Diter_Bit);

#if 0
        RTD_Log(LOGGER_INFO, "01. I_DLTi=%d\n", (UINT32)VIP_Quality_Coef->I_DLTi);
        RTD_Log(LOGGER_INFO, "02. I_DCTi=%d\n", (UINT32)VIP_Quality_Coef->I_DCTi);
        RTD_Log(LOGGER_INFO, "03. Gamma=%d\n",(UINT32)VIP_Quality_Coef->Gamma);
        RTD_Log(LOGGER_INFO, "04. Digital_Filter=%d\n", (UINT32)VIP_Quality_Coef->Digital_Filter);
        RTD_Log(LOGGER_INFO, "05. SUNR_Y=%d\n", (UINT32)VIP_Quality_Coef->SUNR_Y);
        RTD_Log(LOGGER_INFO, "06. SUNR_C=%d\n", (UINT32)VIP_Quality_Coef->SUNR_C);
        RTD_Log(LOGGER_INFO, "07. SU_MASK=%d\n", (UINT32)VIP_Quality_Coef->SU_MASK);
        RTD_Log(LOGGER_INFO, "08. DCC=%d\n", (UINT32)VIP_Quality_Coef->DCC);
        RTD_Log(LOGGER_INFO, "09. D_DLTi=%d\n", (UINT32)VIP_Quality_Coef->D_DLTi);
        RTD_Log(LOGGER_INFO, "10. D_DCTi=%d\n", (UINT32)VIP_Quality_Coef->D_DCTi);
        RTD_Log(LOGGER_INFO, "11. SU_H=%d\n", (UINT32)VIP_Quality_Coef->SU_H);
        RTD_Log(LOGGER_INFO, "12. SU_V=%d\n", (UINT32)VIP_Quality_Coef->SU_V);
        RTD_Log(LOGGER_INFO, "13. SU_PK=%d\n", (UINT32)VIP_Quality_Coef->SU_PK);
        RTD_Log(LOGGER_INFO, "14. VD_Hue=%d\n", (UINT32)VIP_Quality_Coef->VD_Hue);
        RTD_Log(LOGGER_INFO, "15. VD_Sat=%d\n", (UINT32)VIP_Quality_Coef->VD_Sat);
        RTD_Log(LOGGER_INFO, "16. VD_Y_Contrast=%d\n", (UINT32)VIP_Quality_Coef->VD_Y_Contrast);
        RTD_Log(LOGGER_INFO, "17. VD_Y_Brightness=%d\n", (UINT32)VIP_Quality_Coef->VD_Y_Brightness);
        RTD_Log(LOGGER_INFO, "18. ICM_Table_Apply=%d\n", (UINT32)VIP_Quality_Coef->ICM_Table_Apply);
        RTD_Log(LOGGER_INFO, "19. Diter_Bit=%d\n", (UINT32)VIP_Quality_Coef->Diter_Bit);
#endif
        // 1. I_DLTi
        SET_I_DLTI( VIP_Quality_Coef->I_DLTi);
        drv_color_setydlti( VIP_I_DOMAIN, VIP_Quality_Coef->I_DLTi);

        // 2. I_DCTi
        SET_I_DCTI( VIP_Quality_Coef->I_DCTi);
        drv_color_setcdcti( VIP_I_DOMAIN, VIP_Quality_Coef->I_DCTi);

        // 3. Gamma
        SET_GAMMA( VIP_Quality_Coef->Gamma);
        //	drv_color_setgamma( VIP_Quality_Coef->Gamma);

        drv_color_setgamma_s( VIP_Quality_Coef->Gamma, 0);//not apply s-curve result

        // 4. Digital_Filter
        SET_DIGI_FILTER( VIP_Quality_Coef->Digital_Filter);
        drvif_color_digital_filter_config( VIP_Quality_Coef->Digital_Filter);

        // 5. SUNR_Y
        SET_SUNR_Y( VIP_Quality_Coef->SUNR_Y);
        drvif_color_sunr_y( VIP_Quality_Coef->SUNR_Y);

        // 6. SUNR_C
        SET_SUNR_C( VIP_Quality_Coef->SUNR_C);
        drvif_color_sunr_c( VIP_Quality_Coef->SUNR_C);

        // 7. SU_MASK
        SET_SUPK_MASK( VIP_Quality_Coef->SU_MASK);
        drvif_color_supk_mask( VIP_Quality_Coef->SU_MASK);

        // 8. DCC
        SET_DCC_MODE( VIP_Quality_Coef->DCC);
//		drv_color_AdjustNormalizeFactor( info->DispWid, info->DispLen);//HighLightWindow func. is ignored in Sparrow.
        if ( VIP_Quality_Coef->DCC )
        {
            drv_color_FillDCCTable( (VIP_Quality_Coef->DCC)-1);
            drv_color_EnableDCC( 1, _ENABLE);
        }
        else
            drv_color_EnableDCC( 1, _DISABLE);

        // 9. D_DLTi
        SET_DLTI( VIP_Quality_Coef->D_DLTi);
        drv_color_setydlti( VIP_D_DOMAIN, VIP_Quality_Coef->D_DLTi);

        // 10. D_DCTi
        SET_DCTI( VIP_Quality_Coef->D_DCTi);
        drv_color_setcdcti( VIP_D_DOMAIN, VIP_Quality_Coef->D_DCTi);

        // 11. SU_H
        SET_SCALEUPH( VIP_Quality_Coef->SU_H);
        drvif_color_scaler_setscaleup_hcoef( VIP_Quality_Coef->SU_H);

        // 12. SU_V
        SET_SCALEUPV( VIP_Quality_Coef->SU_V);
        drvif_color_scaler_setscaleup_vcoef( VIP_Quality_Coef->SU_V);

        // 13. SU_PK
        SET_SCALEPK( VIP_Quality_Coef->SU_PK);
        drvif_color_scaler_setds_pk( VIP_Quality_Coef->SU_PK);


        if (GET_INPUTSOURCE_FROM() ==_SRC_FROM_VDC)
        {
            // 14. VD_Hue
            drv_video_vdc_SetHue( VIP_Quality_Coef->VD_Hue);

            // 15. VD_Sat
            drv_video_vdc_SetSaturation( VIP_Quality_Coef->VD_Sat);

            // 16. VD_Y_Contrast
            drv_video_vdc_SetContrast( VIP_Quality_Coef->VD_Y_Contrast);

            // 17. VD_Y_Brightness
            drv_video_vdc_SetBrightness( VIP_Quality_Coef->VD_Y_Brightness);
        }

        // 18. ICM_Table_Apply
        SET_ICM_TABLE( VIP_Quality_Coef->ICM_Table_Apply);
        drvif_color_icm_table_select( VIP_Quality_Coef->ICM_Table_Apply );


        // 19. Dither_Bit
        SET_DITHER_BIT( VIP_Quality_Coef->Diter_Bit);
        drv_color_setdithering( VIP_Quality_Coef->Diter_Bit );

        //20. input gamma
//		drv_color_set_inputgamma( _DISABLE, _InputGamma_RGB_Sep);//temporary disable

        RTD_Log(LOGGER_INFO, "\n\nVideo_Quality_Handler() finish!\n===================\n\n");
    }

}
#if 0	// not used
extern RGB_ColorTemp xdata _stRGBColorTemp[35];
extern RGB_ColorTemp code stColorTemp[];
void Load_default_color_data(void)
{
    /*
    	UINT8 code *srcAddr1;
    	UINT8 xdata* ptBuf;
    	int count;
    	int i;

    	srcAddr1 = (UINT8 code*)&stColorTemp;
    	ptBuf = (UINT8 xdata*)&_stRGBColorTemp;
    	count = sizeof(RGB_ColorTemp) * 35;
    	for(i = 0; i < count; i++)
    		*(ptBuf + i) = *(srcAddr1 + i);
    */
}
#endif

