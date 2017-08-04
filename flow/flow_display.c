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
#include "flow_display.h"
#include "flow_struct.h"
#include "panel\panel_struct.h"
#include "rosprintf.h"

//_STRUCT_BACKGROUND_COLOR xdata stBackgroundColor;

void flow_display_Handler(void)
{
    RTD_Log(LOGGER_DEBUG,"fw_scalerdisplay_handler!!!!! \n");
    ////////////////////////////////////////////////
    // Display main window control
    ////////////////////////////////////////////////
    drv_scalerdisplay_set_main_ctrl();
    ////////////////////////////////////////////////
    // Display timing control
    ////////////////////////////////////////////////
    drv_scalerdisplay_set_timing();


}

#if 0  // Temporily not use
/*============================================================================*/
/**
 * fw_scalerdisplay_get_display_size
 * Set display window size, it must considers aspect ratio and pip.
 * This function will decide the info->DispLen and info->DisdWid
 *
 * @param { none }
 * @return none
 *
 */
void flow_display_Get_DisplaySize(void)
{
    // get main window size
    info->DispWid = stPanelData.nActiveWidth;
    info->DispLen = stPanelData.nActiveVLength;

#if 0
#if CONFIG_DISPLAY_RATIO_4X3	//17" panel
    if ((IS_RATIO_16_9()) && (disp_info[display].FrameSyncFlag == _FALSE)) //4//4:3 panel but require 16:9 diplay only in FRC
        disp_info[display].DispLen = SHR(disp_info[display].DispLen * 3, 2);// 4:3 -> 16:9 length become 3/4
    //else if (IS_RATIO_14_9()) //14 :9 panel but require letter box display
    //		disp_info[display].DispWid = SHR(disp_info[display].DispWid * 14, 4);
#else
    if (IS_RATIO_4_3()) //16 :9 panel but require 4:3 display
        disp_info[display].DispWid = SHR(disp_info[display].DispWid * 3, 2);
#ifdef CONFIG_CUSTOMER_HANNSTAR
    else if (IS_RATIO_FIT())
    {

        UINT16 v_ratio;

        //temp=disp_info[display].DispLen;
        if (GET_INTERLACE_MODE(info->display))
            v_ratio=(disp_info[display].DispLen*1000)/(info->IPV_ACT_LEN<<1);
        else
            v_ratio=(disp_info[display].DispLen*1000)/(info->IPV_ACT_LEN);

        if ((info->IPH_ACT_WID*v_ratio/1000)<=_DISP_WID)
        {
            disp_info[display].DispWid=(info->IPH_ACT_WID*v_ratio/1000);
            RTD_Log(LOGGER_INFO,"Fit mode, align to V: disp_info[display].DispWid=%d ****\n",disp_info[display].DispWid);
        }



    }
#endif
    //else if (IS_RATIO_14_9()) //14 :9 panel but require letter box display
    //		disp_info[display].DispWid = SHR(disp_info[display].DispWid * 14, 4) & ~_BIT0; //CSW+ 0961120 Add ~_BIT0 to set width to be even in 422 mode
#endif
#endif
    RTD_Log(LOGGER_INFO,"DispWid=%x, DispLen=%x in SetWindowSize\n",info->DispWid, info->DispLen);
}
#endif

