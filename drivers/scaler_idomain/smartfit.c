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
#include "scaler_idomain\smartfit.h"
#include "scaler_idomain\vgip.h"
#include "rosprintf.h"
#include "timer_event.h"
#include "mode.h"
#include "sparrow.h"
#include "reg_def.h"
#include "flow_struct.h"
#if 0 // not used
UINT16 drv_smartfit_ablgetsmartfitihStart(void)
{
    UINT16 ulResult, ulIHStart=0;
    UINT8 timeoutcnt = 6;   // Tracking timeout 60ms
    UINT8 display = 0;

    fw_timer_event_DelayXms(10);

    //enable smart fit
    rtdf_maskl(AUTO_FIELD_0x0600, ~_BIT5 , 0);


    RTD_Log(LOGGER_DEBUG," Scaler_DispGetInputInfoByDisp(display,SLR_INPUT_H_LEN) = %d\n", info->IHTotal);
    RTD_Log(LOGGER_DEBUG," Scaler_DispGetInputInfoByDisp(display,SLR_INPUT_V_LEN) = %d\n", info->IVTotal);

    rtdf_maskl(AUTO_H_BOUNDARY_0x0604, 0x00000000,info->IHTotal);
    rtdf_maskl(AUTO_V_BOUNDARY_0x0608, 0x00000000,info->IVTotal);

    //pop up result
    timeoutcnt = 20;
    rtdf_setBits(AUTO_ADJ_0x0610,_BIT7|_BIT0);

    do
    {
        fw_timer_event_DelayXms(10);
        ulResult = rtdf_readByte0(AUTO_ADJ_0x0610) & _BIT0;

        if (!ulResult) // polling status bit
            break;
    }
    while (--timeoutcnt);

    if (timeoutcnt == 0x00)
    {
        RTD_Log(LOGGER_DEBUG,"\n Smart Fit error \n");
        return  0;
    }

    //ulIHStart = AUTO_RESULT_HSTA_END_get_hx_sta(rtdf_inl(AUTO_RESULT_HSTA_END_reg));
    ulIHStart =  rtdf_inl(AUTO_RESULT_HSTA_END_0x0618) >> 16;// Jen 0961113 adjust smart fit H shift error, for display ratio

    return ulIHStart;


}
#endif

/**
 * This function is used to get smartfit h,v data enable start value
 *
 * @param mode_index: [input] the index of identified mode
 * @return UINT8
 * @retval TRUE :  smartfit success
 * @retval FALSE: smartfit fail
 */
/*
UINT8 drv_smartfit_getsmartfithvstart(void)
{
   UINT16 ulResult;
   UINT8 timeoutcnt = 6;   // Tracking timeout 60ms
   UINT32 value;

   drv_vgip_set_vgip(VGIP_SRC_TMDS, VGIP_MODE_DIGITAL);
   drv_vgip_set_sample(_ENABLE);//for DI bug when do HSD

   //mark by frank@0512 RtSystem_Sleep(10);//20080219 kist

   rtdf_setBits(VGIP_CHN1_CTRL_0x0a10, _BIT1); // Input graphic/video modeaChannel 1 => From digital input

   value = rtdf_inl(VGIP_CHN1_ACT_HSTA_WIDTH_0x0a18);
   value &= 0xF0000000;
   value |= stModeInfo.IHWidth;

   rtdf_outl(VGIP_CHN1_ACT_HSTA_WIDTH_0x0a18, value);


   value = rtdf_inl(VGIP_CHN1_ACT_VSTA_LENGTH_0x0a1c);
   value &= 0xF0000000;
   value |= stModeInfo.IVHeight;
   rtdf_outl(VGIP_CHN1_ACT_VSTA_LENGTH_0x0a1c, value);

   //enable smart fit
   rtdf_maskl(AUTO_FIELD_0x0600, ~_BIT5 , 0);

   rtdf_maskl(AUTO_H_BOUNDARY_0x0604, 0x00000000,info->IHTotal);
   rtdf_maskl(AUTO_V_BOUNDARY_0x0608, 0x00000000,info->IVTotal);

   //pop up result
   timeoutcnt = 20;
   rtdf_setBits(AUTO_ADJ_0x0610,_BIT7|_BIT0);
   do {
   	fw_timer_event_DelayXms(10);
   	ulResult = rtdf_readByte0(AUTO_ADJ_0x0610);

   	if(!(ulResult & _BIT0)) // polling status bit
   		break;
   } while (--timeoutcnt);

   if (timeoutcnt == 0x00){
   	RTD_Log(LOGGER_DEBUG,"\n Smart Fit error \n");
   	return  FALSE;
   }

   value = rtdf_inl(AUTO_RESULT_VSTA_END_0x0614);
   stModeInfo.IVStartPos = value>>16;
   info->IPV_SMARTFIT_START = value>>16;


   value = rtdf_inl(AUTO_RESULT_HSTA_END_0x0618);
   stModeInfo.IHStartPos = (value>>16-2);// Jen 0961113 adjust smart fit H shift error
   info->IPH_SMARTFIT_START = (value>>16-2);// Jen 0961113 adjust smart fit H shift error, for display ratio


   RTD_Log(LOGGER_DEBUG,"smart fit IHStartPos:%x\n",stModeInfo.IHStartPos);
   RTD_Log(LOGGER_DEBUG,"smart fit IVStartPos:%x\n",stModeInfo.IVStartPos);

   drv_vgip_set_sample(_DISABLE);//smartfit after VGIP so want to enable VGIP

   drv_vgip_set_vgip(VGIP_SRC_TMDS, VGIP_MODE_ANALOG);
   return TRUE;
}
*/


