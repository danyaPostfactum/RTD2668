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
#include "scaler_idomain\auto_soy.h"
#include "sparrow.h"
#include "Reg_def.h"

void drv_auto_soy_SetSOYCompareLevel(UINT8 soy_path, UINT8 value)
{
    switch (soy_path)
    {
    case 0:
        rtdf_maskl(SOYCH0_A3_0x0b0c, ~(0x003f0000),(0x003F0000&((UINT32)(value)<<16)));
        break;

    case 1:
        rtdf_outl(SOYCH1_A0_0x0b5c, (UINT32)value);
        break;
    }
}

UINT8 drv_auto_soy_GetSOYCompareLevel(UINT8 soy_path)
{
    UINT32 value;

    switch (soy_path)
    {
    case 0:
        value = rtdf_inl(SOYCH0_A3_0x0b0c);
        value = (value & 0x003f0000)>>16;
        break;

    case 1:
        value = rtdf_inl(SOYCH1_A0_0x0b5c);
        value = value & 0x0000003f;
        break;
    }
    return value;
}

/**
 * This API is used to do auto SOY of input source YPbPr
 *
 * @param [input] :
 * @param [output] :
 * @return None
 *
 */
#ifdef CONFIG_YPBPR_USE_AUTO_SOY
void drv_auto_soy_AutoSOY(void) // CSW+ 0961031 Auto SOY
{
    // calibration do not enable
    /*IoReg_Write32(AUTOSOY_SOYCH0_B0_VADDR, SOYCH0_B0_reg_ch0_calibrate_cnt_base(0x00)
    	| SOYCH0_B0_reg_ch0_calibrate_bypass(0) | SOYCH0_B0_reg_ch0_calibrate_val_rst(0)
    	| SOYCH0_B0_reg_ch0_auto_calibrate_en(0));*/
    rtdf_maskl(SOYCH0_B0_0x0b14, ~0x000001fd, 0);

    // set the boundary of max and min
    /*IoReg_Write32(AUTOSOY_SOYCH0_A1_VADDR, SOYCH0_A1_reg_max_hb(0x3F) | SOYCH0_A1_reg_max_lb(0x00)
    	| SOYCH0_A1_reg_min_hb(0x3F) | SOYCH0_A1_reg_min_lb(0x00));*/
    rtdf_maskl(SOYCH0_A1_0x0b04, 0x00000000, 0x3f003f00);

    // seek max and seek min window
    /*IoReg_Write32(AUTOSOY_SOYCH0_A2_VADDR, SOYCH0_A2_reg_max_window(0x1F)
    	|SOYCH0_A2_reg_min_window(0x1F) | SOYCH0_A2_reg_min_diff(0x04));*/
    rtdf_maskl(SOYCH0_A2_0x0b08, 0x00000000, 0x001f1f04);

    // DC level & offset
    /*IoReg_Write32(AUTOSOY_SOYCH0_A3_VADDR, SOYCH0_A3_reg_dc_out_offset(0x00)
    	| SOYCH0_A3_reg_manu_comp(SOY_CH0_COMPARE_LEVEL) | SOYCH0_A3_reg_dc_out_mode(0x00));*/
    rtdf_maskl(SOYCH0_A3_0x0b0c, ~0x3f3f0003,((UINT32)_SOY_MANUAL_COMPARE_LEVEL <<16));

    // Status read register
    /*IoReg_Write32(AUTOSOY_SOYCH0_A4_VADDR, SOYCH0_A4_cmp_level_r(0) | SOYCH0_A4_dc_level_r(0)
    	| SOYCH0_A4_max_hb_flag_r(0) | SOYCH0_A4_max_lb_flag_r(0)
    	| SOYCH0_A4_max_r(0) | SOYCH0_A4_min_hb_flag_r(0)
    	| SOYCH0_A4_min_lb_flag_r(0) | SOYCH0_A4_min_r(0));*/
    rtdf_outl(SOYCH0_A4_0x0b10, 0x00000000);

    // Enable Auto SOY
    //When the first DC level is not triggered, no Sync can be extracted.
    //if this happens that we need to change the First trigger DC level, let it can be trigger
    /*IoReg_Write32(AUTOSOY_SOYCH0_A0_VADDR, SOYCH0_A0_reg_auto_ini_dc(SOY_CH0_COMPARE_LEVEL)
    	| SOYCH0_A0_reg_deb_valid(0x0F) | SOYCH0_A0_dummy18020300_15_14(0)
    	| SOYCH0_A0_reg_deb_update(0x0F) | SOYCH0_A0_dummy18020300_7_6(0)
    	| SOYCH0_A0_reg_update_mode(0) | SOYCH0_A0_reg_ma_mode(1)
    	| SOYCH0_A0_reg_auto_mode(1));*/
    rtdf_maskl(SOYCH0_A3_0x0b0c, ~0x3f3fffff,(((UINT32)_SOY_MANUAL_COMPARE_LEVEL <<24)|(0x000f0f03)));




}
#endif




