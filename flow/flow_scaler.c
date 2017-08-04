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
#include "flow_scaler.h"
#include "flow_struct.h"
#include "rosprintf.h"


//UINT8 ucIVS2DVSDelay=0;
//UINT8 ucIHSDelay=0;

//If source is FR, handle display ratio at movie or caption
//void flow_scaler_cal_vgip_capture(void)
//{
//}

/* setup capture window */
//void flow_scaler_set_vgip_capture(void)
//{
//}


// Calculate the output size of scale down
void flow_scaler_Decide_ScaleSize(void)
{
    RTD_Log(LOGGER_DEBUG,"\naa info->IPH_ACT_WID= %d, info->DispWid = %d\n",(UINT32)(info->IPH_ACT_WID),(UINT32)info->DispWid);
    RTD_Log(LOGGER_DEBUG,"\naa info->IPV_ACT_LEN= %d, info->DispLen = %d\n",(UINT32)(info->IPV_ACT_LEN),(UINT32)info->DispLen);

    if (info->IPH_ACT_WID > info->DispWid)
    {
        SET_HSCALE_DOWN();
        CLR_HSCALE_UP();
    }
    else if (info->IPH_ACT_WID < info->DispWid)
    {
        CLR_HSCALE_DOWN();
        SET_HSCALE_UP();
    }
    else
    {
        CLR_HSCALE_DOWN();
        CLR_HSCALE_UP();
    }

    if (info->IPV_ACT_LEN > info->DispLen)
    {
        SET_VSCALE_DOWN();
        CLR_VSCALE_UP();
    }
    else if (info->IPV_ACT_LEN < info->DispLen)
    {
        CLR_VSCALE_DOWN();
        SET_VSCALE_UP();
    }
    else
    {
        CLR_VSCALE_DOWN();
        CLR_VSCALE_UP();
    }

    //RTD_Log(LOGGER_INFO,"IPV_ACT_LEN = %x, IPH_ACT_WID = %x\n", info->IPV_ACT_LEN, info->IPH_ACT_WID);
    //RTD_Log(LOGGER_INFO,"DispLen = %x, DispWid = %x\n", info->DispLen, info->DispWid);
    RTD_Log(LOGGER_INFO,"IPV_ACT_LEN = %x\n", (UINT32)(info->IPV_ACT_LEN));
    RTD_Log(LOGGER_INFO,"IPH_ACT_WID = %x\n", (UINT32)(info->IPH_ACT_WID));
    RTD_Log(LOGGER_INFO,"DispLen = %x\n", (UINT32)(info->DispLen));
    RTD_Log(LOGGER_INFO,"DispWid = %x\n", (UINT32)(info->DispWid));

}

// Enable OnLine Measure Watchdog
//void flow_scaler_Enable_Measure_Watchdog(void)
//{

//}

// Disable OnLine Measure Watchdog
//void flow_scaler_Disable_Measure_Watchdog(void)
//{

//}

// Enable FrameSync Measure Watchdog
//void flow_scaler_Enable_Framesync_WatchDog(void)
//{
//}

// Disable FrameSync Measure Watchdog
//void flow_scaler_Disable_Framesync_WatchDog(void)
//{
//}

