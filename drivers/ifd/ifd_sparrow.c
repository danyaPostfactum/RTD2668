#include "ifd\ifd.h"
#include "ifd\ifd_factorymode.h"
#include "ifd\ifd_sparrow.h"
//=================================================================
//IFD driver -Keep original function name for Middle Layer or UI
//=================================================================


#ifdef CONFIG_TUNER_DEFAULT

UINT8 drv_ifd_Freq_Offset_Small(void)
{
    INT32 ifd_offset;
    INT32 thd = 3; //threshold

    ifd_offset = drvif_tuner_get_afcoffset(0);
    thd = thd*drv_tuner_GetRatioInStepSize();

    if ((ifd_offset > -thd)&&(ifd_offset < thd))
    {
        if (drvif_ifd_get_cr_lock()==_TRUE)
        {
            return 1;
        }
    }
    else
    {
        return 0;
    }
}
#endif

#ifdef _VIDEO_TV_SUPPORT

//=================================================================
//IFD driver -system setting for sparrow
//=================================================================
//=================================================================
/**
 * drv_ifd_analog_preset
 *
 *
 * @param <void>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_analog_preset(void)
{
    rtd_outl(SYSTEM_PLL27X_VADDR, 0xFA03F040);
    rtd_outl(IFD_AIN_SEL_VADDR, 0x00000001);
    rtd_outl(IFD_ADC_SHGAIN_VADDR, 0x00000000);
    rtd_outl(IFD_ADC_CMSEL_VADDR, 0x00000001);
    rtd_outl(IFD_ADC_IFBW_VADDR, 0x00000002);
    rtd_outl(IFD_ADC_CK_VADDR, 0x000000d3);
    rtd_outl(IFD_ADC_CKOUTDELAY_VADDR, 0x00000000);
    rtd_outl(IFD_ADC_SVRT_VADDR, 0x00000004);
    rtd_outl(IFD_ADC_SVRB_VADDR, 0x00000004);
    rtd_outl(IFD_ADC_TOS_VADDR, 0x00000000);
    if (IFD_CHIP_VER_A()==_TRUE)
        rtd_outl(IFD_ADC_SIBMBIAS_VADDR, 0x00000088);
    else
        rtd_outl(IFD_ADC_SIBMBIAS_VADDR, 0x00000088);

    rtd_outl(IFD_ADC_SIBIFPGA_VADDR, 0x0000000A);
    rtd_outl(IFD_ADC_MBIASPOW_VADDR, 0x00000001);
    rtd_outl(IFD_ADC_IFPGAPOW_VADDR, 0x00000001);
    if (IFD_CHIP_VER_A()==_TRUE)
        rtd_outl(IFD_ADC_ANALOG_RESERVE_VADDR, 0x04001750); //0x04001550);
    else
        rtd_outl(IFD_ADC_ANALOG_RESERVE_VADDR, 0x44001750); //0x44001550);

    drv_ifd_adc_clk_sel(IFD_CLK_RATE_108M);
}
#endif
#endif
//=================================================================
/**
 * drv_ifd_power_on
 *
 *
 * @param <enable>
 * @return {void}
 */
void drv_ifd_power_on(bool enable)
{
    if (enable)
    {
        rtdf_maskl(ADC_IFPGAPOW_0x5ff0, ~_BIT0, _BIT0);
        rtdf_maskl(ADC_MBIASPOW_0x5fec, ~_BIT0, _BIT0);
        rtdf_maskl(VDC_VD_ADC2X_POW_0x57a8, ~(_BIT1|_BIT0), (_BIT1|_BIT0));
        rtdf_setBits(ADC_ANALOG_RESERVE_0x5ff4,_BIT12);	//HPWang add
    }
    else
    {
        rtdf_maskl(ADC_IFPGAPOW_0x5ff0, ~_BIT0, 0);
        rtdf_maskl(ADC_MBIASPOW_0x5fec, ~_BIT0, 0);
        rtdf_maskl(VDC_VD_ADC2X_POW_0x57a8,  ~(_BIT1|_BIT0), 0);
        rtdf_clearBits(ADC_ANALOG_RESERVE_0x5ff4,_BIT12);	 //HPWang add
    }
}
//=================================================================
//IFD driver - local function for sparrow
//=================================================================
#ifdef _VIDEO_TV_SUPPORT
//=================================================================
/**
 * drv_ifd_user_data_init
 *
 *
 * @param {void}
 * @return {void}
 */
void drv_ifd_user_data_init(void)
{
    ;
}
//=================================================================
/**
 * ifd_active_tuner_assign
 *
 *
 * @param {tunerID}
 * @return {void}
 */
void ifd_active_tuner_assign(UINT8 tunerID)
{
    tunerID = tunerID;//For Keil Building
}
//=================================================================
/**
 * drv_ifd_set_cr_bpf_coef
 *
 *
 * @param <cr_bpf_en><table>
 * @return { void }
 */
void drv_ifd_set_cr_bpf_coef(bool cr_bpf_en, UINT16 *table)
{
    UINT32 cr_bpf_sel_rbus_data;
    UINT32 i;

    ////IFD_DRV_DEBUG_MSG("%s(%x)\n", __FUNCTION__,cr_bpf_en);

    for (i=0; i<CR_BPF_TABLE_SIZE; i++)
    {
        cr_bpf_sel_rbus_data = 0;
        rtd_outl(IFD_CR_BPF_SEL_VADDR, cr_bpf_sel_rbus_data);
        cr_bpf_sel_rbus_data = ((UINT32)1<<24) | (((UINT32)i&0x3F)<<16) | ((UINT32)table[i]&0x7FFF);
        rtd_outl(IFD_CR_BPF_SEL_VADDR, cr_bpf_sel_rbus_data);
    }
    cr_bpf_sel_rbus_data = 0;
    rtd_outl(IFD_CR_BPF_SEL_VADDR, cr_bpf_sel_rbus_data);
    drv_ifd_cr_bpf_en(cr_bpf_en);
    drv_ifd_cr_inner_lpf_en(cr_bpf_en);
}
/*=============================================================*/
/**
 * drv_ifd_cr_bpf_en
 *
 *
 * @param <cr_bpf_en>
 * @return { void }
 */
void drv_ifd_cr_bpf_en(bool cr_bpf_en)
{
    ////IFD_DRV_DEBUG_MSG("%s(%x)\n", __FUNCTION__,cr_bpf_en);

    if (cr_bpf_en)
    {
        rtd_maskl(IFD_CR_BPF_SEL_VADDR, ~(_BIT31|_BIT30|_BIT29|_BIT28|_BIT25), (((UINT32)6)<<28)|(((UINT32)1)<<25));
    }
    else
    {
        rtd_maskl(IFD_CR_BPF_SEL_VADDR, ~(_BIT31|_BIT30|_BIT29|_BIT28|_BIT25), (((UINT32)6)<<28));
    }
}
#if 0
/*=============================================================*/
/**
 * drv_ifd_cr_bpf_en_get_info
 *
 *
 * @param <void>
 * @return { cr_bpf_en }
 */
UINT8 drv_ifd_cr_bpf_en_get_info(void)
{
    UINT32 cr_bpf_sel_rbus_data;
    UINT8 cr_bpf_en;

    cr_bpf_sel_rbus_data = rtd_inl(IFD_CR_BPF_SEL_VADDR);
    cr_bpf_en = (cr_bpf_sel_rbus_data>>25) & 0x1;
    return cr_bpf_en;
}
#endif
/*=============================================================*/
/**
 * drv_ifd_cr_inner_lpf_en
 *
 *
 * @param <cr_inner_lpf_en>
 * @return { void }
 */
void drv_ifd_cr_inner_lpf_en(bool cr_inner_lpf_en)
{
    ////IFD_DRV_DEBUG_MSG("%s(%x)\n", __FUNCTION__,cr_inner_lpf_en);

    if (cr_inner_lpf_en)
        rtd_maskl(IFD_CR_BPF_SEL_VADDR, ~(_BIT26), (((UINT32)1)<<26));
    else
        rtd_maskl(IFD_CR_BPF_SEL_VADDR, ~(_BIT26), 0);
}
#if 0
/*=============================================================*/
/**
 * drv_ifd_cr_inner_lpf_en_get_info
 *
 *
 * @param <void>
 * @return { cr_inner_lpf_en }
 */
UINT8 drv_ifd_cr_inner_lpf_en_get_info(void)
{
    UINT32 cr_bpf_sel_rbus_data;
    UINT8 cr_inner_lpf_en;

    cr_bpf_sel_rbus_data = rtd_inl(IFD_CR_BPF_SEL_VADDR);
    cr_inner_lpf_en = (cr_bpf_sel_rbus_data>>26)&0x1;
    return cr_inner_lpf_en;
}
#endif
/*=============================================================*/
/**
 * drv_ifd_cr_loop_filter_kp
 *
 *
 * @param <cr_pgain>
 * @return { void }
 */
void drv_ifd_cr_loop_filter_kp( UINT8 cr_pgain)
{
    ////IFD_DRV_DEBUG_MSG("%s(%x)\n", __FUNCTION__,cr_pgain);

    rtd_maskl(IFD_CR_PGAIN_VADDR, ~(_BIT7|_BIT6|_BIT5|_BIT4), ((UINT32)cr_pgain)<<4);
}
#if 0
/*=============================================================*/
/**
 * drv_ifd_cr_kp_get_info
 *
 *
 * @param <void>
 * @return { cr_pgain }
 */
UINT8 drv_ifd_cr_kp_get_info(void)
{
    UINT32 cr_pgain_rbus_data;
    UINT8 cr_pgain;

    cr_pgain_rbus_data = rtd_inl(IFD_CR_PGAIN_VADDR);
    cr_pgain = (cr_pgain_rbus_data>>4)&0xf;
    return cr_pgain;
}
#endif
/*=============================================================*/
/**
 * drv_ifd_cr_loop_filter_ki
 *
 *
 * @param <cr_igain>
 * @return { void }
 */
void drv_ifd_cr_loop_filter_ki(UINT8 cr_igain)
{
    ////IFD_DRV_DEBUG_MSG("%s(%x)\n", __FUNCTION__,cr_igain);

    rtd_maskl(IFD_CR_PGAIN_VADDR, ~(_BIT3|_BIT2|_BIT1|_BIT0), (UINT32)cr_igain);
}
#if 0
/*=============================================================*/
/**
 * drv_ifd_cr_ki_get_info
 *
 *
 * @param <void>
 * @return { cr_igain }
 */
UINT8 drv_ifd_cr_ki_get_info(void)
{
    UINT32 cr_pgain_rbus_data;
    UINT8 cr_igain;

    cr_pgain_rbus_data = rtd_inl(IFD_CR_PGAIN_VADDR);
    cr_igain = cr_pgain_rbus_data&0xf;
    return cr_igain;
}
#endif
/*=============================================================*/
/**
 * drv_ifd_get_cr_lock_status
 *
 *
 * @param <void>
 * @return <lock_status>
 */
bool drv_ifd_get_cr_lock_status()
{
    UINT32 cr_lock_rbus_data;

    cr_lock_rbus_data = rtd_inl(IFD_CR_LOCK_VADDR);

    return cr_lock_rbus_data&0x1;
}

/*=============================================================*/
/**
 * drv_ifd_set_cr_nco
 *
 *
 * @param <para>
 * @return { void }
 */
void drv_ifd_set_cr_nco (bool enable, UINT32 para)
{
    if (enable)
        rtd_outl(IFD_CR_NCO_FIX_DATA_VADDR, (((UINT32)1)<<20)|(para&(UINT32)0xfffff));
    else
        rtd_outl(IFD_CR_NCO_FIX_DATA_VADDR, (para&(UINT32)0xfffff));
}
/*=============================================================*/
/**
 * drv_ifd_get_cr_freq_offset
 *
 *
 * @param <void>
 * @return <cr_freq_offset>
 */
UINT32 drv_ifd_get_cr_freq_offset(void)
{
    UINT32 value;

    value = rtd_inl(IFD_CR_FREQ_STATUS_VADDR)&0xfffff;

    return value;
}
/*=============================================================*/
/**
 * drv_ifd_set_cr_weighting_en
 *
 *
 * @param <cr_weight_en>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_set_cr_weighting_en (bool cr_weight_en)
{
    if (cr_weight_en)
        rtd_maskl(IFD_CR_WEIGHT_EN_VADDR, ~_BIT8, _BIT8);
    else
        rtd_maskl(IFD_CR_WEIGHT_EN_VADDR, ~_BIT8, 0);
}
#endif
#if 0
/*=============================================================*/
/**
 * drv_ifd_cr_weighting_en_get_info
 *
 *
 * @param <void>
 * @return { cr_weight_en }
 */
UINT8 drv_ifd_cr_weighting_en_get_info (void)
{
    UINT32 cr_weight_en_rbus_data;

    cr_weight_en_rbus_data = rtd_inl(IFD_CR_WEIGHT_EN_VADDR);

    return (cr_weight_en_rbus_data>>8)&0x1;
}
#endif
/*=============================================================*/
/**
 * drv_ifd_set_cr_weighting_settings
 *
 *
 * @param <cr_weighting_cr_lock_dis><cr_weight_max_decrease><cr_weight_shift>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_set_cr_weighting_settings (IFD_CR_WEIGHT_CR_LOCK cr_weighting_cr_lock_dis, IFD_CR_WEIGHT_MAX_DECREASE cr_weight_max_decrease, IFD_CR_WEIGHT_SHIFT cr_weight_shift)
{
    rtd_maskl(IFD_CR_WEIGHT_EN_VADDR, ~((UINT32)(_BIT9|0xFF)), ((UINT32)cr_weighting_cr_lock_dis<<9)|((UINT32)cr_weight_max_decrease<<4)|(UINT32)cr_weight_shift);
}
#endif
#if 0
/*=============================================================*/
/**
 * drv_ifd_set_cr_weighting_speed
 *
 *
 * @param <cr_weight_max_decrease>
 * @return { void }
 */
void drv_ifd_set_cr_weighting_speed (IFD_CR_WEIGHT_MAX_DECREASE cr_weight_max_decrease)
{
    rtd_maskl(IFD_CR_WEIGHT_EN_VADDR, ~((UINT32)0xF0), (cr_weight_max_decrease<<4));
}
/*=============================================================*/
/**
 * drv_ifd_set_cr_weighting_speed_get_info
 *
 *
 * @param <void>
 * @return { cr_weight_max_decrease }
 */
IFD_CR_WEIGHT_MAX_DECREASE drv_ifd_set_cr_weighting_speed_get_info (void)
{
    UINT32 cr_weight_en_rbus_data;

    cr_weight_en_rbus_data = rtd_inl(IFD_CR_WEIGHT_EN_VADDR);

    return (cr_weight_en_rbus_data>>4)&0xf;
}
#endif
/*=============================================================*/
/**
 * drv_ifd_set_cr_phase_bad_en
 *
 *
 * @param <cr_phase_bad_check_en>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void  drv_ifd_set_cr_phase_bad_en(bool cr_phase_bad_check_en)
{
    if (cr_phase_bad_check_en)
        rtd_maskl(IFD_CR_PHASE_BAD_VADDR, ~(_BIT7), _BIT7);
    else
        rtd_maskl(IFD_CR_PHASE_BAD_VADDR, ~(_BIT7), 0);
}
#endif
#if 0
/*=============================================================*/
/**
 * drv_ifd_cr_phase_bad_en_get_info
 *
 *
 * @param <void>
 * @return { cr_phase_bad_check_en }
 */
UINT8 drv_ifd_cr_phase_bad_en_get_info(void)
{
    UINT32 cr_phase_bad_rbus_data;

    cr_phase_bad_rbus_data = rtd_inl(IFD_CR_PHASE_BAD_VADDR);
    return (cr_phase_bad_rbus_data>>7)&0x1;
}
#endif
/*=============================================================*/
/**
 * drv_ifd_set_slow_charge_en_for_max
 *
 *
 * @param <enable>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void  drv_ifd_set_slow_charge_en_for_max(bool enable)
{
    if (IFD_CHIP_VER_A()==_TRUE)
    {
        if (enable)
            rtd_maskl(IFD_DUMMY1_VADDR, ~_BIT0, _BIT0);
        else
            rtd_maskl(IFD_DUMMY1_VADDR, ~_BIT0, 0);
    }
    else
    {
        if (enable)
            rtd_maskl(IFD_DUMMY1_VADDR, ~_BIT0, _BIT0);
        else
            rtd_maskl(IFD_DUMMY1_VADDR, ~_BIT0, 0);
    }
}
#endif
#if 0
/*=============================================================*/
/**
 * drv_ifd_set_slow_charge_en_for_max_get_info
 *
 *
 * @param <void>
 * @return { enable }
 */
UINT8 drv_ifd_set_slow_charge_en_for_max_get_info(void)
{
    UINT32 dummy1_rbus_data;

    dummy1_rbus_data = rtd_inl(IFD_DUMMY1_VADDR);
    if (IFD_CHIP_VER_A()==_TRUE)
        return (dummy1_rbus_data&0x1);
    else
        return (dummy1_rbus_data&0x1);

}
#endif
/*=============================================================*/
/**
 * drv_ifd_set_slow_charge_en_for_peak
 *
 *
 * @param <enable>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void  drv_ifd_set_slow_charge_en_for_peak(bool enable)
{
    if (IFD_CHIP_VER_A()==_TRUE)
    {
        if (enable)
            rtd_maskl(IFD_DUMMY1_VADDR, ~_BIT0, _BIT0);
        else
            rtd_maskl(IFD_DUMMY1_VADDR, ~_BIT0, 0);
    }
    else
    {
        if (enable)
            rtd_maskl(IFD_DUMMY1_VADDR, ~_BIT4, _BIT4);
        else
            rtd_maskl(IFD_DUMMY1_VADDR, ~_BIT4, 0);
    }
}
#endif
#if 0
/*=============================================================*/
/**
 * drv_ifd_set_slow_charge_en_for_peak_get_info
 *
 *
 * @param <void>
 * @return { enable }
 */
UINT8 drv_ifd_set_slow_charge_en_for_peak_get_info(void)
{
    UINT32 dummy1_rbus_data;

    dummy1_rbus_data = rtd_inl(IFD_DUMMY1_VADDR);
    if (IFD_CHIP_VER_A()==_TRUE)
        return (dummy1_rbus_data&0x1);
    else
        return ((dummy1_rbus_data>>4)&0x1);

}
#endif
#endif
/*=============================================================*/
/**
 * drv_ifd_set_videopath_output_gain
 *
 *
 * @param <data_gain>
 * @return { void }
 */
void drv_ifd_set_videopath_output_gain(IFD_VIDEOPATH_OUTPUT_GAIN data_gain)
{
    rtd_maskl(IFD_SHAPE_FILTER_SELECTION_VADDR, ~((UINT32)0x7<<8), ((UINT32)(data_gain)<<8));
}
#ifdef _VIDEO_TV_SUPPORT
/*=============================================================*/
/**
 * drv_ifd_set_shaping_filter
 *
 *
 * @param <shaping_bw_sel>
 * @return { void }
 */
void drv_ifd_set_shaping_filter(IFD_SHAPING_BW_SEL shaping_bw_sel)
{
    rtd_maskl(IFD_SHAPE_FILTER_SELECTION_VADDR, ~((UINT32)(0xF0)), ((UINT32)shaping_bw_sel<<4));
}
#if 0
/*=============================================================*/
/**
 * drv_ifd_set_shaping_filter_img_get_info
 *
 *
 * @param <void>
 * @return <shaping_image_sel>
 */
bool drv_ifd_set_shaping_filter_img_get_info()
{
    UINT32 shape_filter_selection_rbus_data;

    shape_filter_selection_rbus_data = rtd_inl(IFD_SHAPE_FILTER_SELECTION_VADDR);

    return shape_filter_selection_rbus_data&0x1;
}
/*=============================================================*/
/**
 * drv_ifd_cr_phase_error_variance_get_info
 *
 *
 * @param <void>
 * @return { cr_phase_error_variance }
 */
UINT16 drv_ifd_cr_phase_error_variance_get_info(void)
{
    UINT32 cr_phase_err_cal_rbus_data;

    cr_phase_err_cal_rbus_data = rtd_inl(IFD_CR_PHASE_ERR_CAL_VADDR);
    return (UINT16)((cr_phase_err_cal_rbus_data>>16)&0xffff);
}
#endif
/**
 * drv_ifd_cr_phase_error_variance_get_info
 *
 *
 * @param <void>
 * @return { cr_phase_error_variance }
 */
UINT16 drv_ifd_cr_phase_error_variance_get_info(void)
{
    UINT32 cr_phase_err_cal_rbus_data;

    cr_phase_err_cal_rbus_data = rtd_inl(IFD_CR_PHASE_ERR_CAL_VADDR);
    return (UINT16)((cr_phase_err_cal_rbus_data>>16)&0xffff);
}
/*=============================================================*/
/**
 * drv_ifd_set_shaping_img
 *
 *
 * @param <shaping_image_sel>
 * @return { void }
 */
void drv_ifd_set_shaping_img( IFD_SHAPING_IMG_SEL shaping_image_sel)
{
    rtd_maskl(IFD_SHAPE_FILTER_SELECTION_VADDR, ~_BIT0, ((UINT32)shaping_image_sel&0x1));
}
/*=============================================================*/
/**
 * drv_ifd_soft_reset
 *
 *
 * @param <reset>
 * @return { void }
 */
void drv_ifd_soft_reset(bool reset)
{
    if (reset)
        rtd_maskl(IFD_SOFT_RESET_27_CLK_PHASE_VADDR, ~(_BIT0), _BIT0);
    else

        rtd_maskl(IFD_SOFT_RESET_27_CLK_PHASE_VADDR, ~(_BIT0), 0);
}
/*=============================================================*/
/**
 * drv_ifd_cr_error_thd
 *
 *
 * @param <err_thd>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_cr_error_thd(UINT8 err_thd)
{
    rtd_maskl(IFD_CR_ERR_THD_VADDR, ~((UINT32)(0xFF0000)), ((UINT32)err_thd)<<16);
}
#endif
#if 0
/*=============================================================*/
/**
 * drv_ifd_cr_error_thd_get_info
 *
 *
 * @param <void>
 * @return { err_thd }
 */
#ifndef CONFIG_TUNER_SILICON
UINT8 drv_ifd_cr_error_thd_get_info(void)
{
    UINT32 cr_err_thd_rbus_data;

    cr_err_thd_rbus_data = rtd_inl(IFD_CR_ERR_THD_VADDR);

    return (cr_err_thd_rbus_data>>16)&0xFF;
}
#endif
#endif
/*=============================================================*/
/**
 * drv_ifd_cr_auto_reset_en
 *
 *
 * @param <enable>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_cr_auto_reset_en (bool enable)
{
    if (enable)
        rtd_maskl(IFD_CR_AUTO_RESET_EN_VADDR, ~(_BIT0), _BIT0);
    else
        rtd_maskl(IFD_CR_AUTO_RESET_EN_VADDR, ~(_BIT0), 0);
}
#endif
#if 0
/*=============================================================*/
/**
 * drv_ifd_cr_auto_reset_en_get_info
 *
 *
 * @param <void>
 * @return { enable }
 */
bool drv_ifd_cr_auto_reset_en_get_info(void)
{
    UINT32 cr_auto_reset_en_rbus_data;

    cr_auto_reset_en_rbus_data = rtd_inl(IFD_CR_AUTO_RESET_EN_VADDR);

    return (cr_auto_reset_en_rbus_data&0x1);
}
#endif
#endif
#ifdef _VIDEO_TV_SUPPORT
/*=============================================================*/
/**
 * drv_ifd_cr_phase_inverse_en
 *
 *
 * @param <enable>
 * @return { void }
 */
void drv_ifd_cr_phase_inverse_en(bool enable)
{
    if (enable)
        rtd_maskl(IFD_CR_PHASE_INVERSE_VADDR, ~(_BIT0), _BIT0);
    else
        rtd_maskl(IFD_CR_PHASE_INVERSE_VADDR, ~(_BIT0), 0);
}
#if 0
/*=============================================================*/
/**
 * drv_ifd_cr_phase_inverse_en_get_info
 *
 *
 * @param <void>
 * @return { enable }
 */
bool drv_ifd_cr_phase_inverse_en_get_info(void)
{
    UINT32 cr_phase_inverse_rbus_data;

    cr_phase_inverse_rbus_data = rtd_inl(IFD_CR_PHASE_INVERSE_VADDR);

    return (cr_phase_inverse_rbus_data&0x1);

}
#endif
//#if !(IFD_CHIP_VER_A)
#if 0
/*=============================================================*/
/**
 * drv_ifd_cr_phase_inverse_en_depend_crlock_diable
 *
 *
 * @param <enable>
 * @return { void }
 */
void drv_ifd_cr_phase_inverse_en_depend_crlock_diable(bool enable)
{
    if (enable)
        rtd_maskl(IFD_DUMMY1_VADDR, ~(_BIT8), _BIT8);
    else
        rtd_maskl(IFD_DUMMY1_VADDR, ~(_BIT8), 0);
}
/*=============================================================*/
/**
 * drv_ifd_cr_phase_inverse_en_depend_crlock_disable_get_info
 *
 *
 * @param <void>
 * @return { enable }
 */
bool drv_ifd_cr_phase_inverse_en_depend_crlock_disable_get_info(void)
{
    UINT32 dummy1_rbus_data;

    dummy1_rbus_data = rtd_inl(IFD_DUMMY1_VADDR);

    return (dummy1_rbus_data>>8)&0x1;
}

#endif
/*=============================================================*/
/**
 * drv_ifd_eq_en
 *
 *
 * @param <enable>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_eq_en(bool enable)
{
    if (enable)
        rtd_maskl(IFD_EQ_FILTER_SEL_VADDR, ~_BIT17, _BIT17);
    else
        rtd_maskl(IFD_EQ_FILTER_SEL_VADDR, ~_BIT17, 0);
}
#endif
/*=============================================================*/
/**
 * drv_ifd_eq_en_get_info
 *
 *
 * @param <void>
 * @return { enable }
 */
#ifndef CONFIG_TUNER_SILICON
bool drv_ifd_eq_en_get_info(void)
{
    UINT32 eq_filter_sel_rbus_data;

    eq_filter_sel_rbus_data = rtd_inl(IFD_EQ_FILTER_SEL_VADDR);

    return (eq_filter_sel_rbus_data>>17)&0x1;
}
#endif
#endif
#ifdef _VIDEO_TV_SUPPORT
/*=============================================================*/
/**
 * drv_ifd_set_eq_coef
 *
 *
 * @param <table>
 * @return { void }
 */
//#ifndef CONFIG_TUNER_SILICON
void drv_ifd_set_eq_coef(UINT16 *table)
{
    UINT32 eq_filter_sel_rbus_data;
    UINT32 i;

    for (i=0; i<EQ_TABLE_SIZE; i++)
    {
        eq_filter_sel_rbus_data = 0;
        rtd_outl(IFD_EQ_FILTER_SEL_VADDR, eq_filter_sel_rbus_data);
        eq_filter_sel_rbus_data = (UINT32)(0x10000);
        eq_filter_sel_rbus_data |= ((UINT32)((i&0xf)<<12)|(table[i]&0x3ff));
        rtd_outl(IFD_EQ_FILTER_SEL_VADDR, eq_filter_sel_rbus_data);
    }

    eq_filter_sel_rbus_data = 0;
    rtd_outl(IFD_EQ_FILTER_SEL_VADDR, eq_filter_sel_rbus_data);

}
//#endif
/*=============================================================*/
/**
 * drv_ifd_set_54to27_decimation_enable
 *
 *
 * @param <enable>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_set_54to27_decimation_enable(bool enable)
{
    //RTD_Log(LOGGER_INFO, "drv_ifd_set_54to27_decimation_enable(%x)\n", enable);
    if (enable)
    {
        //RTD_Log(LOGGER_INFO, "drv_ifd_set_54to27_decimation_enable(1)\n");
        rtd_maskl(IFD_VIDEO_BW_SEL_VADDR, ~_BIT20, _BIT20);
    }
    else
    {
        //RTD_Log(LOGGER_INFO, "drv_ifd_set_54to27_decimation_enable(0)\n");
        rtd_maskl(IFD_VIDEO_BW_SEL_VADDR, ~_BIT20, 0);
    }
}
#endif
#endif
#ifdef _VIDEO_TV_SUPPORT
/*=============================================================*/
/**
 * drv_ifd_set_pos_neg_mod
 *
 *
 * @param <mod_sel>
 * @return { void }
 */
void drv_ifd_set_pos_neg_mod(IFD_MODULATION_SEL mod_sel)
{
    if (mod_sel==1)
    {
        rtd_maskl(IFD_VIDEO_BW_SEL_VADDR, ~_BIT16, (UINT32)_BIT16);
    }
    else
    {
        rtd_maskl(IFD_VIDEO_BW_SEL_VADDR, ~_BIT16, 0);
    }
}
/*=============================================================*/
/**
 * drv_ifd_set_video_audio_nf_bw
 *
 *
 * @param <video_bw_sel><sound_notch_sel2><sound_notch_sel><video_upsample_lp_sel>
 * @return { void }
 */
void drv_ifd_set_video_audio_nf_bw(IFD_VIDEO_BW_SEL video_bw_sel, IFD_SOUND_NOTCH_SEL2 sound_notch_sel2, IFD_SOUND_NOTCH_SEL sound_notch_sel, IFD_UPSAMPLE_LP_SEL video_upsample_lp_sel)
{
    rtd_maskl(IFD_VIDEO_BW_SEL_VADDR, ~((UINT32)0xFFFF), (((UINT32)video_bw_sel<<12)|((UINT32)sound_notch_sel2<<8)|((UINT32)sound_notch_sel<<4)|(UINT32)video_upsample_lp_sel));
}
#if 0
/*=============================================================*/
/**
 * drv_ifd_set_video_bw
 *
 *
 * @param <video_bw_sel>
 * @return { void }
 */
void drv_ifd_set_video_bw(IFD_VIDEO_BW_SEL video_bw_sel)
{
    rtd_maskl(IFD_VIDEO_BW_SEL_VADDR, ~((UINT32)(0xF<<12)), ((UINT32)video_bw_sel<<12));
}
/*=============================================================*/
/**
 * drv_ifd_get_video_bw_info
 *
 *
 * @param <void>
 * @return { video_bw_sel }
 */
IFD_VIDEO_BW_SEL drv_ifd_get_video_bw_info(void)
{
    UINT32 video_bw_sel_rbus_data;

    video_bw_sel_rbus_data = rtd_inl(IFD_VIDEO_BW_SEL_VADDR);

    return (video_bw_sel_rbus_data>>12)&0xF;
}
#endif
/*=============================================================*/
/**
 * drv_ifd_grp_delay_en
 *
 *
 * @param <enable><table>
 * @return { void }
 */
void drv_ifd_grp_delay_en (bool enable,  UINT16 *table)
{
    UINT32 grp_delay_en_rbus_data;
    UINT32 i, tap_num;

    ////IFD_DRV_DEBUG_MSG("%s(%x)\n", __FUNCTION__,enable);

    tap_num = (UINT32)table[0];
    for (i=0; i<tap_num; i++)
    {
        grp_delay_en_rbus_data = 0;
        rtd_outl(IFD_GRP_DELAY_EN_VADDR, grp_delay_en_rbus_data);
        grp_delay_en_rbus_data = (_BIT20)|((UINT32)(i&0xFF)<<12)|((UINT32)table[i+1]);
        rtd_outl(IFD_GRP_DELAY_EN_VADDR, grp_delay_en_rbus_data);
    }
    grp_delay_en_rbus_data = 0;
    rtd_outl(IFD_GRP_DELAY_EN_VADDR, grp_delay_en_rbus_data);

    if (enable)
        grp_delay_en_rbus_data = _BIT24;
    rtd_outl(IFD_GRP_DELAY_EN_VADDR, grp_delay_en_rbus_data);

}
#if 0
/*=============================================================*/
/**
 * drv_ifd_clamp_offset_hold
 *
 *
 * @param <clamp_offset_hold>
 * @return { void }
 */
void drv_ifd_clamp_offset_hold (IFD_CLAMP_OFFSET clamp_offset_hold)
{
    if (clamp_offset_hold==1)
        rtd_maskl(IFD_CLAMPING_SCALE_VADDR, ~_BIT8, _BIT8);
    else
        rtd_maskl(IFD_CLAMPING_SCALE_VADDR, ~_BIT8, 0);
}
#endif
/*=============================================================*/
/**
 * drv_ifd_set_clamping_scale
 *
 *
 * @param <clamping_scale>
 * @return { void }
 */
void drv_ifd_set_clamping_scale (IFD_CLAMP_SCALE clamping_scale)
{
    rtd_maskl(IFD_CLAMPING_SCALE_VADDR, ~((UINT32)0xF), ((UINT32)clamping_scale)&0xF);
}
#if 0
/*=============================================================*/
/**
 * drv_ifd_set_clamping_scale_get_info
 *
 *
 * @param <void>
 * @return { clamping_scale }
 */
IFD_CLAMP_SCALE drv_ifd_set_clamping_scale_get_info (void)
{
    UINT32 clamping_scale_rbus_data;

    clamping_scale_rbus_data = rtd_inl(IFD_CLAMPING_SCALE_VADDR);

    return (clamping_scale_rbus_data&0xF);
}
#endif
/*=============================================================*/
/**
 * drv_ifd_set_clamping_thr_max
 *
 *
 * @param <clamping_thr_max>
 * @return <void>
 */
void drv_ifd_set_clamping_thr_max (UINT16 clamping_thr_max)
{
    rtd_maskl(IFD_CLAMPING_THR_MAX_VADDR, ~((UINT32)0x3FFF), ((UINT32)clamping_thr_max)&0x3FFF);
}
#if 0
/*=============================================================*/
/**
 * drv_ifd_set_clamping_thr_max_get_info
 *
 *
 * @param <void>
 * @return <clamping_thr_max>
 */
UINT16 drv_ifd_set_clamping_thr_max_get_info (void)
{
    UINT32 clamping_thr_max_rbus_data;

    clamping_thr_max_rbus_data = rtd_inl(IFD_CLAMPING_THR_MAX_VADDR);

    return (clamping_thr_max_rbus_data&0x1FFF);
}
#endif
/*=============================================================*/
/**
 * drv_ifd_set_clamping_thr_min
 *
 *
 * @param <clamping_thr_min>
 * @return <void>
 */
void drv_ifd_set_clamping_thr_min (UINT16 clamping_thr_min)
{
    rtd_maskl(IFD_CLAMPING_THR_MIN_VADDR, ~((UINT32)0x3FFF), ((UINT32)clamping_thr_min)&0x3FFF);
}
#if 0
/*=============================================================*/
/**
 * drv_ifd_set_clamping_thr_min_get_info
 *
 *
 * @param <void>
 * @return <clamping_thr_min>
 */
UINT16 drv_ifd_set_clamping_thr_min_get_info (void)
{
    UINT32 clamping_thr_min_rbus_data;

    clamping_thr_min_rbus_data = rtd_inl(IFD_CLAMPING_THR_MAX_VADDR);

    return (clamping_thr_min_rbus_data&0x1FFF);
}
/*=============================================================*/
/**
 * drv_ifd_get_clamping_offset_status
 *
 *
 * @param <void>
 * @return <clamping_offset_status>
 */
UINT16 drv_ifd_get_clamping_offset_status (void)
{
    UINT32 offset_rbus_data;

    offset_rbus_data= rtd_inl(IFD_OFFSET_VADDR);

    return offset_rbus_data&0x1FFF;
}
#endif
/*=============================================================*/
/**
 * drv_ifd_set_audio_bp_up_lp
 *
 *
 * @param <audio_bp_sel><audio_upsample_lp_sel>
 * @return { void }
 */
void drv_ifd_set_audio_bp_up_lp (IFD_AUDIO_BP_SEL audio_bp_sel, IFD_AUDIO_UPSAMPLE_LP_SEL audio_upsample_lp_sel)
{
    rtd_maskl(IFD_AUDIO_BP_SEL_VADDR, ~((UINT32)(0xFF3)), (((UINT32)audio_bp_sel&0xFF)<<4)|((UINT32)audio_upsample_lp_sel&0x3));
}
#endif
#if 0
/*=============================================================*/
/**
 * drv_ifd_set_audio_bp
 *
 *
 * @param <audio_bp_sel>
 * @return { void }
 */
void drv_ifd_set_audio_bp (IFD_AUDIO_BP_SEL audio_bp_sel)
{
    rtd_maskl(IFD_AUDIO_BP_SEL_VADDR, ~(0xFF0), ((audio_bp_sel&0xFF)<<4));

}
/*=============================================================*/
/**
 * drv_ifd_get_audio_bp_info
 *
 *
 * @param <void>
 * @return { audio_bp_sel }
 */
IFD_AUDIO_BP_SEL drv_ifd_get_audio_bp_info (void)
{
    UINT32 audio_bp_sel_rbus_data;

    audio_bp_sel_rbus_data = rtd_inl(IFD_AUDIO_BP_SEL_VADDR);

    return (audio_bp_sel_rbus_data>>4)&0xFF;
}
/*=============================================================*/
/**
 * drv_ifd_audio_gain_en
 *
 *
 * @param <enable>
 * @return { void }
 */
void drv_ifd_audio_gain_en(bool enable)
{
    if (enable)
        rtd_maskl(IFD_AUDIO_GAIN_MANU_VADDR, ~_BIT8, _BIT8);
    else
        rtd_maskl(IFD_AUDIO_GAIN_MANU_VADDR, ~_BIT8, 0);
}
/*=============================================================*/
/**
 * drv_ifd_audio_gain_en_get_info
 *
 *
 * @param <void>
 * @return { enable }
 */
bool drv_ifd_audio_gain_en_get_info(void)
{
    UINT32 audio_gain_manu_rbus_data;

    audio_gain_manu_rbus_data = rtd_inl(IFD_AUDIO_GAIN_MANU_VADDR);

    return (audio_gain_manu_rbus_data>>8)&0x1;
}
/*=============================================================*/
/**
 * drv_ifd_agc_dgain_en
 *
 *
 * @param <enable>
 * @return { void }
 */
void drv_ifd_agc_dgain_en(bool enable)
{
    if (enable)
        rtd_maskl(IFD_AGC_ENABLE_SEL_VADDR, ~_BIT6, _BIT6);
    else
        rtd_maskl(IFD_AGC_ENABLE_SEL_VADDR, ~_BIT6, 0);
}
/*=============================================================*/
/**
 * drv_ifd_agc_dgain_en
 *
 *
 * @param <enable>
 * @return { void }
 */
void drv_ifd_agc_agc_rf_en(bool enable)
{
    if (enable)
        rtd_maskl(IFD_AGC_ENABLE_SEL_VADDR, ~_BIT1, _BIT1);
    else
        rtd_maskl(IFD_AGC_ENABLE_SEL_VADDR, ~_BIT1, 0);
}
#endif
#ifdef _VIDEO_TV_SUPPORT
/*=============================================================*/
/**
 * drv_ifd_agc_loop1_fine_tune_en
 *
 *
 * @param <enable>
 * @return { void }
 */
void drv_ifd_agc_loop1_fine_tune_en(bool enable)
{
    if (enable)
        rtd_maskl(IFD_AGC_ENABLE_SEL_VADDR, ~_BIT0, _BIT0);
    else
        rtd_maskl(IFD_AGC_ENABLE_SEL_VADDR, ~_BIT0, 0);
}
/*=============================================================*/
/**
 * drv_ifd_agc_loop1_fine_tune_en_get_info
 *
 *
 * @param <void>
 * @return { enable }
 */
bool drv_ifd_agc_loop1_fine_tune_en_get_info()
{
    UINT32 agc_enable_sel_rbus_data;

    agc_enable_sel_rbus_data = rtd_inl(IFD_AGC_ENABLE_SEL_VADDR);

    return agc_enable_sel_rbus_data&0x1;
}
#if 0
/*=============================================================*/
/**
 * drv_ifd_agc_bpf_en
 *
 *
 * @param <enable>
 * @return { void }
 */
void drv_ifd_agc_bpf_en(bool enable)
{
    if (enable)
        rtd_maskl(IFD_AGC_ENABLE_SEL_VADDR, ~_BIT5, _BIT5);
    else
        rtd_maskl(IFD_AGC_ENABLE_SEL_VADDR, ~_BIT5, 0);
}
/*=============================================================*/
/**
 * drv_ifd_agc_bpf_en_get_info
 *
 *
 * @param <void>
 * @return { enable }
 */
bool drv_ifd_agc_bpf_en_get_info(void)
{
    UINT32 agc_enable_sel_rbus_data;

    agc_enable_sel_rbus_data = rtd_inl(IFD_AGC_ENABLE_SEL_VADDR);

    return (agc_enable_sel_rbus_data>>5)&0x1;
}
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_diff_targ
 *
 *
 * @param <agc_diff_targ>
 * @return { void }
 */
void drv_ifd_agc_diff_targ(UINT16 agc_diff_targ)
{
    rtd_maskl(IFD_AGC_AVG_TARGET_VADDR, ~((UINT32)(0x7FF)), (UINT32)agc_diff_targ&0x7FF);
}
#if 0
/*=============================================================*/
/**
 * drv_ifd_agc_diff_target_get_info
 *
 *
 * @param <void>
 * @return { agc_diff_targ }
 */
UINT16  drv_ifd_agc_diff_target_get_info(void)
{
    UINT32 agc_avg_target_rbus_data;

    agc_avg_target_rbus_data= rtd_inl(IFD_AGC_AVG_TARGET_VADDR);

    return (agc_avg_target_rbus_data)&0x7FFF;
}
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_loop1_pos_en
 *
 *
 * @param <enable>
 * @return { void }
 */
void drv_ifd_agc_loop1_pos_en (bool enable)
{
    if (enable)
        rtd_maskl(IFD_AGC_MAX_COUNT_FOR_COARSE_MODE_AGC_LOCK2_VADDR, ~_BIT0, _BIT0);
    else
        rtd_maskl(IFD_AGC_MAX_COUNT_FOR_COARSE_MODE_AGC_LOCK2_VADDR, ~_BIT0, 0);
}
#if 0
/*=============================================================*/
/**
 * drv_ifd_agc_pos_en_get_info
 *
 *
 * @param <void>
 * @return { enable }
 */
bool drv_ifd_agc_pos_en_get_info (void)
{
    UINT32 agc_max_count_for_coarse_mode_agc_lock2_rbus_data;

    agc_max_count_for_coarse_mode_agc_lock2_rbus_data = rtd_inl(IFD_AGC_MAX_COUNT_FOR_COARSE_MODE_AGC_LOCK2_VADDR);

    return agc_max_count_for_coarse_mode_agc_lock2_rbus_data&0x1;
}
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_targ_val
 *
 *
 * @param <agc_targ_val>
 * @return { void }
 */
void drv_ifd_agc_targ_val(UINT16 agc_targ_val)
{
    rtd_maskl(IFD_AGC_TARG_VAL_VADDR, ~((UINT32)(0x7FF)), (UINT32)agc_targ_val&0x7FF);
}
#if 0
/*=============================================================*/
/**
 * drv_ifd_agc_targ_val_get_info
 *
 *
 * @param <void>
 * @return { agc_targ_val }
 */
UINT16 drv_ifd_agc_targ_val_get_info(void)
{
    UINT32 agc_targ_val_rbus_data;

    agc_targ_val_rbus_data = rtd_inl(IFD_AGC_TARG_VAL_VADDR);
    return (agc_targ_val_rbus_data&0x7FFF);
}
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_out_inv
 *
 *
 * @param <agc_out_inv_rf><agc_out_inv_if>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_out_inv(bool agc_out_inv_rf, bool agc_out_inv_if)
{
    rtd_maskl(IFD_AGC_CNTL_SEL_VADDR, ~(_BIT9|_BIT5), ((UINT32)agc_out_inv_rf<<9)|((UINT32)agc_out_inv_if<<5));
}
#endif
#if 0
/*=============================================================*/
/**
 * drv_ifd_agc_if_manual_en
 *
 *
 * @param <agc_if_manu_en>
 * @return { void }
 */
void drv_ifd_agc_if_manual_en(bool agc_if_manu_en)
{
    rtd_maskl(IFD_AGC_CNTL_SEL_VADDR, ~(_BIT4), ((UINT32)agc_if_manu_en<<4));
}
/*=============================================================*/
/**
 * drv_ifd_agc_if_manual_en_get_info
 *
 *
 * @param <void>
 * @return { agc_if_manu_en }
 */
UINT16 drv_ifd_agc_if_manual_en_get_info(void)
{
    UINT32 agc_cntl_sel_rbus_data;

    agc_cntl_sel_rbus_data = rtd_inl(IFD_AGC_CNTL_SEL_VADDR);
    return (agc_cntl_sel_rbus_data>>4)&0x1;
}
/*=============================================================*/
/**
 * drv_ifd_agc_if_manual_gain
 *
 *
 * @param <agc_if_manu_value>
 * @return { void }
 */
void drv_ifd_agc_if_manual_gain(UINT16 agc_if_manu_value)
{
    rtd_maskl(IFD_AGC_IF_MANU_VALUE_VADDR, ~((UINT32)0x1FFF), (UINT32)agc_if_manu_value&0x1FFF);
}
/*=============================================================*/
/**
 * drv_ifd_agc_if_manual_gain_get_info
 *
 *
 * @param <void>
 * @return { agc_if_manu_value }
 */
UINT16 drv_ifd_agc_if_manual_gain_get_info(void)
{
    UINT32 agc_if_manu_value_rbus_data;

    agc_if_manu_value_rbus_data = rtd_inl(IFD_AGC_IF_MANU_VALUE_VADDR);

    return (agc_if_manu_value_rbus_data&0x1FFF);
}

/*=============================================================*/
/**
 * drv_ifd_agc_rf_manual_en
 *
 *
 * @param <agc_rf_manu_en>
 * @return { void }
 */
void drv_ifd_agc_rf_manual_en(bool agc_rf_manu_en)
{
    rtd_maskl(IFD_AGC_CNTL_SEL_VADDR, ~(_BIT3), ((UINT32)agc_rf_manu_en<<3));
}
/*=============================================================*/
/**
 * drv_ifd_agc_rf_manual_en_get_info
 *
 *
 * @param <void>
 * @return { agc_rf_manu_en }
 */
bool drv_ifd_agc_rf_manual_en_get_info(void)
{
    UINT32 agc_cntl_sel_rbus_data;

    agc_cntl_sel_rbus_data = rtd_inl(IFD_AGC_CNTL_SEL_VADDR);
    return (agc_cntl_sel_rbus_data>>3)&0x1;
}
/*=============================================================*/
/**
 * drv_ifd_agc_manual_rf_gain
 *
 *
 * @param <agc_rf_manu_value>
 * @return { void }
 */
void drv_ifd_agc_manual_rf_gain(UINT16 agc_rf_manu_value)
{
    rtd_maskl(IFD_AGC_RF_MANU_VALUE_VADDR, ~((UINT32)0x1FFF), (UINT32)agc_rf_manu_value&0x1FFF);
}
/*=============================================================*/
/**
 * drv_ifd_agc_manual_rf_gain
 *
 *
 * @param <void>
 * @return { agc_rf_manu_value }
 */
UINT16 drv_ifd_agc_manual_rf_gain_get_info(void)
{
    UINT32 agc_rf_manu_value_rbus_data;

    agc_rf_manu_value_rbus_data = rtd_inl(IFD_AGC_RF_MANU_VALUE_VADDR);

    return (agc_rf_manu_value_rbus_data&0x1FFF);
}
/*=============================================================*/
/**
 * drv_ifd_agc_manual_en
 *
 *
 * @param <agc_manual_en><agc_manual_value>
 * @return { void }
 */
void drv_ifd_agc_manual_en(bool agc_manual_en, UINT16 agc_manual_value)
{
    rtd_maskl(IFD_AGC_CNTL_SEL_VADDR, ~(_BIT2), (agc_manual_en<<2));
    rtd_maskl(IFD_AGC_MANU_VALUE_VADDR, ~((UINT32)0x1FFF), (UINT32)agc_manual_value&0x1FFF);
}
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_loop1_kp_settings
 *
 *
 * @param <agc_kp>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_loop1_kp_settings(UINT8 agc_kp)
{
    rtd_maskl(IFD_KI_VAL_VADDR, ~((UINT32)0xF0), ((UINT32)agc_kp&0x1F)<<4);
}
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_loop1_ki_settings
 *
 *
 * @param <agc_ki>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_loop1_ki_settings(UINT8 agc_ki)
{
    rtd_maskl(IFD_KI_VAL_VADDR, ~((UINT32)0xF), ((UINT32)agc_ki&0xF));
}
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_loop1_pishift_settings
 *
 *
 * @param <agc_kpd_add_method_shift>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_loop1_pishift_settings(UINT8 agc_kpd_add_method_shift)
{

//**************irene reminder**************
// agc_kpd_add_method_shift is signed value need modify
    rtd_maskl(IFD_KI_VAL_VADDR, ~((UINT32)(0x1F<<24)), ((UINT32)agc_kpd_add_method_shift&0x1F)<<24);
}
#endif
#endif
#if 0
/*=============================================================*/
/**
 * drv_ifd_get_agc_loop1_kp_settings
 *
 *
 * @param <void>
 * @return <agc_kp>
 */
UINT8 drv_ifd_get_agc_loop1_kp_settings()
{
    UINT32 ki_val_rbus_data;

    ki_val_rbus_data = rtd_inl(IFD_KI_VAL_VADDR);

    return (ki_val_rbus_data>>4)&0xF;
}
/*=============================================================*/
/**
 * drv_ifd_get_agc_loop1_ki_settings
 *
 *
 * @param <void>
 * @return <agc_ki >
 */
UINT8 drv_ifd_get_agc_loop1_ki_settings()
{
    UINT32 ki_val_rbus_data;

    ki_val_rbus_data = rtd_inl(IFD_KI_VAL_VADDR);

    return ki_val_rbus_data&0xF;
}
#endif
#if 0
/*=============================================================*/
/**
 * drv_ifd_get_agc_loop1_pishift_settings
 *
 *
 * @param <void>
 * @return <agc_kpd_add_method_shift >
 */
UINT8 drv_ifd_get_agc_loop1_pishift_settings()
{
    UINT32 ki_val_rbus_data;

    ki_val_rbus_data = rtd_inl(IFD_KI_VAL_VADDR);

    return (ki_val_rbus_data>>24)&0x1F;
}
/*=============================================================*/
/**
 * drv_ifd_agc_rf_max
 *
 *
 * @param <rf_max>
 * @return <void >
 */
void drv_ifd_agc_rf_max(UINT16 rf_max)
{
    rtd_maskl(IFD_RF_AGC_LIMIT_VALUE_VADDR, ~((UINT32)0x1FFF), (UINT32)rf_max&0x1FFF);

    drv_ifd_soft_reset(IFD_ENABLE);
    ScalerTimer_DelayXms(1);
    drv_ifd_soft_reset(IFD_DISABLE);
}
/*=============================================================*/
/**
 * drv_ifd_agc_rf_max_get_info
 *
 *
 * @param <void>
 * @return <rf_max >
 */
UINT16 drv_ifd_agc_rf_max_get_info()
{
    UINT32 rf_agc_limit_value_rbus_data;

    rf_agc_limit_value_rbus_data = rtd_inl(IFD_RF_AGC_LIMIT_VALUE_VADDR);

    return rf_agc_limit_value_rbus_data&0x1FFF;

}
/*=============================================================*/
/**
 * drv_ifd_agc_get_loop1_lock
 *
 *
 * @param <void>
 * @return <lock_status>
 */
bool drv_ifd_agc_get_loop1_lock()
{
    UINT32 agc_lock_status_rbus_data;

    agc_lock_status_rbus_data = rtd_inl(IFD_AGC_LOCK_STATUS_VADDR);

    return agc_lock_status_rbus_data&0x1;
}
/*=============================================================*/
/**
 * drv_ifd_agc_get_loop1_fine_lock
 *
 *
 * @param <void>
 * @return <fine_lock_status>
 */
bool drv_ifd_agc_get_loop1_fine_lock()
{
    UINT32 agc_lock_status_rbus_data;

    agc_lock_status_rbus_data = rtd_inl(IFD_AGC_LOCK_STATUS_VADDR);

    return (agc_lock_status_rbus_data>>4)&0x1;
}
/*=============================================================*/
/**
 * drv_ifd_agc_get_loop1_peak_status
 *
 *
 * @param <void>
 * @return <peak_status>
 */
bool drv_ifd_agc_get_loop1_peak_status()
{
    UINT32 agc_peak_detect_rbus_data;

    agc_peak_detect_rbus_data = rtd_inl(IFD_AGC_PEAK_DETECT_VADDR);

    return (agc_peak_detect_rbus_data>>16)&0x1;
}
#endif
#ifdef _VIDEO_TV_SUPPORT
/*=============================================================*/
/**
 * drv_ifd_agc_loop1_peak_en
 *
 *
 * @param <agc_peak_en>
 * @return { void }
 */
void drv_ifd_agc_loop1_peak_en(bool agc_peak_en)
{
    rtd_maskl(IFD_AGC_PEAK_DETECT_VADDR, ~_BIT12, (UINT32)agc_peak_en<<12);
}
/*=============================================================*/
/**
 * drv_ifd_get_agc_loop1_peak_en
 *
 *
 * @param <void>
 * @return <agc_peak_en>
 */
bool drv_ifd_get_agc_loop1_peak_en()
{
    UINT32 agc_peak_detect_rbus_data;

    agc_peak_detect_rbus_data = rtd_inl(IFD_AGC_PEAK_DETECT_VADDR);

    return (agc_peak_detect_rbus_data>>12)&0x1;
}
/*=============================================================*/
/**
 * drv_ifd_agc_peak_target
 *
 *
 * @param <agc_peak_value>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_peak_target(UINT16 agc_peak_value)
{
    rtd_maskl(IFD_AGC_PEAK_VALUE_VADDR, ~((UINT32)0x7FF), (UINT32)agc_peak_value&0x7FF);
}
#endif
#if 0
/*=============================================================*/
/**
 * drv_ifd_agc_peak_target_get_info
 *
 *
 * @param <void>
 * @return { agc_peak_value }
 */
UINT16  drv_ifd_agc_peak_target_get_info(void)
{
    UINT32 agc_peak_value_rbus_data;

    agc_peak_value_rbus_data = rtd_inl(IFD_AGC_PEAK_VALUE_VADDR);

    return (agc_peak_value_rbus_data&0x7FF);
}

/*=============================================================*/
/**
 * drv_ifd_agc_get_loop1_err0
 *
 *
 * @param <void>
 * @return <agc_err>
 */
UINT16 drv_ifd_agc_get_loop1_err0()
{
    UINT32 agc_err0_status_rbus_data;

    agc_err0_status_rbus_data = rtd_inl(IFD_AGC_ERR0_STATUS_VADDR);

    return agc_err0_status_rbus_data&0xFFF;
}
/*=============================================================*/
/**
 * drv_ifd_agc_get_if_output
 *
 *
 * @param <void>
 * @return <agc_if_mode>
 */
UINT16 drv_ifd_agc_get_if_output()
{
    UINT32 agc_if_status_rbus_data;

    agc_if_status_rbus_data = rtd_inl(IFD_AGC_IF_STATUS_VADDR);

    return agc_if_status_rbus_data&0x1FFF;
}
/*=============================================================*/
/**
 * drv_ifd_agc_get_rf_output
 *
 *
 * @param <void>
 * @return <agc_rf_mode>
 */
UINT16 drv_ifd_agc_get_rf_output()
{
    UINT32 agc_rf_status_rbus_data;

    agc_rf_status_rbus_data = rtd_inl(IFD_AGC_RF_STATUS_VADDR);

    return agc_rf_status_rbus_data&0x1FFF;
}
/*=============================================================*/
/**
 * drv_ifd_agc_pga_manual_en
 *
 *
 * @param <agc_pga_fix_en><agc_pga_fix>
 * @return { void }
 */
void drv_ifd_agc_pga_manual_en(bool agc_pga_fix_en, UINT16 agc_pga_fix)
{
    rtd_maskl(IFD_AGC_PGA_1_VADDR, ~(_BIT16|0x1FFF), ((UINT32)agc_pga_fix_en<<16)|((UINT32)agc_pga_fix&0x1FFF));
}
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_vtop
 *
 *
 * @param <agc_vtop>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_vtop(UINT32 agc_vtop)
{
    rtd_maskl(IFD_VTOP_VALUE_VADDR, ~((UINT32)0x1FFF), (UINT32)agc_vtop);
}
#endif
#if 0
/*=============================================================*/
/**
 * drv_ifd_get_agc_vtop
 *
 *
 * @param <void>
 * @return <agc_vtop>
 */
UINT32 drv_ifd_get_agc_vtop()
{
    UINT32 vtop_value_rbus_data;

    vtop_value_rbus_data = rtd_inl(IFD_VTOP_VALUE_VADDR);

    return vtop_value_rbus_data&0x1FFF;
}
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_krf
 *
 *
 * @param <agc_krf>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_krf(UINT32 agc_krf)
{
    rtd_maskl(IFD_KRF_VALUE_VADDR, ~((UINT32)0x3FFF), (UINT32)agc_krf);
}
#endif
#if 0
/*=============================================================*/
/**
 * drv_ifd_get_agc_krf
 *
 *
 * @param <void>
 * @return <agc_krf>
 */
UINT32 drv_ifd_get_agc_krf()
{
    UINT32 krf_value_rbus_data;

    krf_value_rbus_data = rtd_inl(IFD_KRF_VALUE_VADDR);

    return krf_value_rbus_data&0x3FFF;
}
/*=============================================================*/
/**
 * drv_ifd_agc_get_pga_code
 *
 *
 * @param <void>
 * @return <pga_set_read>
 */
UINT16 drv_ifd_agc_get_pga_code()
{
    UINT32 read_pga_rbus_data;

    read_pga_rbus_data = rtd_inl(IFD_READ_PGA_VADDR);

    return read_pga_rbus_data&0x1FFF;
}
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_original_mode
 *
 *
 * @param <agc_original>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_original_mode(bool agc_original)
{
    rtd_maskl(IFD_AGC_ORIGINAL_SEL_VADDR, ~_BIT0, (UINT32)agc_original&0x1);
}
#endif
#if 0
/*=============================================================*/
/**
 * drv_ifd_get_agc_original_mode
 *
 *
 * @param <void>
 * @return <agc_original>
 */
bool drv_ifd_get_agc_original_mode()
{
    UINT32 vtop_value_rbus_data;

    vtop_value_rbus_data = rtd_inl(IFD_AGC_ORIGINAL_SEL_VADDR);

    return vtop_value_rbus_data&0x1;

}
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_vsync_mode_en
 *
 *
 * @param <agc_vsync_mode_en>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_vsync_mode_en(bool agc_vsync_mode_en)
{
    rtd_maskl(IFD_AGC_PGA_CNTL_VADDR, ~_BIT8, (UINT32)agc_vsync_mode_en<<8);
}
#endif
#if 0
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_hold
 *
 *
 * @param <agc_loop2_hold>
 * @return { void }
 */
void drv_ifd_agc_loop2_hold(bool agc_loop2_hold)
{
    rtd_maskl(IFD_AGC_LINEBASE_CTL_VADDR, ~_BIT20, (UINT32)agc_loop2_hold<<20);
}
#endif
#if 0
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_hold_get_info
 *
 *
 * @param <void>
 * @return { agc_loop2_hold }
 */
bool drv_ifd_agc_loop2_hold_get_info()
{
    UINT32 agc_linebase_ctl_rbus_data;

    agc_linebase_ctl_rbus_data= rtd_inl(IFD_AGC_LINEBASE_CTL_VADDR);
    return (agc_linebase_ctl_rbus_data>>20)&0x1;
}
#endif
/*=============================================================*/
/**
 * drv_ifd_loop2_agcin_kpki_settings
 *
 *
 * @param <agc_in_kp><agc_in_ki><agc_in_kp_shift><agc_in_ki_shift>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_loop2_agcin_kpki_settings(UINT8 agc_in_kp, UINT8 agc_in_ki, UINT8 agc_in_kp_shift, UINT8 agc_in_ki_shift)
{
    rtd_maskl(IFD_AGC_IN_KP_KI_VADDR, ~((UINT32)((0xF<<20)|(0xF<<16)|((0x1F)<<8)|(0x1F))), ((UINT32)agc_in_kp_shift<<20)|((UINT32)agc_in_ki_shift<<16)|((UINT32)agc_in_kp<<8)|((UINT32)agc_in_ki));
}
#endif
#endif
#if 0
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_kp_settings
 *
 *
 * @param <agc_in_kp>
 * @return { void }
 */
void drv_ifd_agc_loop2_kp_settings(UINT8 agc_in_kp)
{
    rtd_maskl(IFD_AGC_IN_KP_KI_VADDR, ~((UINT32)(0x1F)<<8), ((UINT32)agc_in_kp<<8));
}
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_kp_settings_get_info
 *
 *
 * @param <void>
 * @return { agc_in_kp }
 */
UINT8 drv_ifd_agc_loop2_kp_settings_get_info(void)
{
    UINT32 agc_in_kp_ki_rbus_data;

    agc_in_kp_ki_rbus_data = rtd_inl(IFD_AGC_IN_KP_KI_VADDR);

    return (agc_in_kp_ki_rbus_data>>8)&0x1F;
}
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_ki_settings
 *
 *
 * @param <agc_in_ki>
 * @return { void }
 */
void drv_ifd_agc_loop2_ki_settings(UINT8 agc_in_ki)
{
    rtd_maskl(IFD_AGC_IN_KP_KI_VADDR, ~((UINT32)(0x1F)), ((UINT32)agc_in_ki));
}
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_ki_settings_get_info
 *
 *
 * @param <void>
 * @return { agc_in_ki }
 */
UINT8 drv_ifd_agc_loop2_ki_settings_get_info(void)
{
    UINT32 agc_in_kp_ki_rbus_data;

    agc_in_kp_ki_rbus_data = rtd_inl(IFD_AGC_IN_KP_KI_VADDR);

    return (agc_in_kp_ki_rbus_data)&0x1F;
}

/*=============================================================*/
/**
 * drv_ifd_agc_loop2_kishift_settings
 *
 *
 * @param <agc_in_ki_shift>
 * @return { void }
 */
void drv_ifd_agc_loop2_kishift_settings(UINT8 agc_in_ki_shift)
{
    rtd_maskl(IFD_AGC_IN_KP_KI_VADDR, ~((UINT32)(0xF<<16)), ((UINT32)agc_in_ki_shift<<16));
}
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_kishift_settings_get_info
 *
 *
 * @param <void>
 * @return { agc_in_ki_shift }
 */
UINT8 drv_ifd_agc_loop2_kishift_settings_get_info(void)
{
    UINT32 agc_in_kp_ki_rbus_data;

    agc_in_kp_ki_rbus_data = rtd_inl(IFD_AGC_IN_KP_KI_VADDR);

    return (agc_in_kp_ki_rbus_data>>16)&0xF;
}
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_kpshift_settings
 *
 *
 * @param <agc_in_kp_shift>
 * @return { void }
 */
void drv_ifd_agc_loop2_kpshift_settings(UINT8 agc_in_kp_shift)
{
    rtd_maskl(IFD_AGC_IN_KP_KI_VADDR, ~((UINT32)(0xF<<20)), ((UINT32)agc_in_kp_shift<<20));
}
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_kpshift_settings_get_info
 *
 *
 * @param <void>
 * @return { agc_in_kp_shift }
 */
UINT8 drv_ifd_agc_loop2_kpshift_settings_get_info(void)
{
    UINT32 agc_in_kp_ki_rbus_data;

    agc_in_kp_ki_rbus_data = rtd_inl(IFD_AGC_IN_KP_KI_VADDR);

    return (agc_in_kp_ki_rbus_data>>20)&0xF;
}
/*=============================================================*/
/**
 * drv_ifd_agc_dgain_boundary
 *
 *
 * @param <agc_in_max><agc_in_min>
 * @return { void }
 */
void drv_ifd_agc_dgain_boundary(UINT16 agc_in_max, UINT16 agc_in_min)
{
    rtd_outl(IFD_AGC_IN_MAX_MIN_VADDR, ((UINT32)agc_in_max<<16)|(UINT32)agc_in_min);
}
#endif
#ifdef _VIDEO_TV_SUPPORT
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_target
 *
 *
 * @param <agc_targ_val_loop2>
 * @return { void }
 */
void drv_ifd_agc_loop2_target(UINT16 agc_targ_val_loop2)
{
    rtd_maskl(IFD_AGC_TARG_VAL_LOOP2_VADDR, ~((UINT32)0x7FF), (UINT32)agc_targ_val_loop2&0x7FF);
}
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_target_get_info
 *
 *
 * @param <void>
 * @return { target }
 */
UINT16 drv_ifd_agc_loop2_target_get_info()
{
    UINT32 agc_targ_val_loop2_rbus_data;

    agc_targ_val_loop2_rbus_data = rtd_inl(IFD_AGC_TARG_VAL_LOOP2_VADDR);

    return agc_targ_val_loop2_rbus_data&0x7FF;
}
#if 0
/*=============================================================*/
/**
 * drv_ifd_agc_get_loop2_err
 *
 *
 * @param {void}
 * @return  <loop2_err>
 */
UINT16 drv_ifd_agc_get_loop2_err()
{
    UINT32 agc_looop2_err_rbus_data;

    agc_looop2_err_rbus_data= rtd_inl(IFD_AGC_LOOOP2_ERR_VADDR);

    return agc_looop2_err_rbus_data&0xFFFF;
}

/*=============================================================*/
/**
 * drv_ifd_agc_get_loop1_dgain_output
 *
 *
 * @param {void}
 * @return  <agc_inner1>
 */
UINT16 drv_ifd_agc_get_loop1_dgain_output()
{
    UINT32 agc_inner_status_rbus_data;

    agc_inner_status_rbus_data= rtd_inl(IFD_AGC_INNER_STATUS_VADDR);

    return (agc_inner_status_rbus_data>>16)&0xFFFF;
}
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_get_loop2_dgain_output
 *
 *
 * @param {void}
 * @return  <agc_inner2>
 */
UINT16 drv_ifd_agc_get_loop2_dgain_output()
{
    UINT32 agc_inner_status_rbus_data;

    agc_inner_status_rbus_data= rtd_inl(IFD_AGC_INNER_STATUS_VADDR);

    return agc_inner_status_rbus_data&0xFFFF;
}
#if 0
/*=============================================================*/
/**
 * drv_ifd_agc_adaptive_peak2_en
 *
 *
 * @param <enable>
 * @return { void }
 */
void drv_ifd_agc_adaptive_peak2_en(bool enable)
{
    rtd_maskl(IFD_AGC_PEAK2_DETECT_VADDR, ~_BIT28, (UINT32)enable<<28);
}
/*=============================================================*/
/**
 * drv_ifd_agc_get_loop2_peak_status
 *
 *
 * @param {void}
 * @return  <agc_peak2_status>
 */
bool drv_ifd_agc_get_loop2_peak_status()
{
    UINT32 agc_peak2_detect_rbus_data;

    agc_peak2_detect_rbus_data = rtd_inl(IFD_AGC_PEAK2_DETECT_VADDR);

    return (agc_peak2_detect_rbus_data>>16)&0x1;
}
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_peak_en
 *
 *
 * @param <enable>
 * @return { void }
 */
void drv_ifd_agc_loop2_peak_en(bool enable)
{
    rtd_maskl(IFD_AGC_PEAK2_DETECT_VADDR, ~_BIT12, (UINT32)enable<<12);
}
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_peak_cnt
 *
 *
 * @param <agc_peak2_cnt>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_loop2_peak_cnt(UINT16 agc_peak2_cnt)
{
    rtd_maskl(IFD_AGC_PEAK2_DETECT_VADDR, ~((UINT32)0xFFF), (UINT32)agc_peak2_cnt&0xFFF);
}
#endif
/*=============================================================*/
/**
 * drv_ifd_get_agc_loop2_peak_en
 *
 *
 * @param <void>
 * @return <agc_peak2_en>
 */
bool drv_ifd_get_agc_loop2_peak_en()
{
    UINT32 agc_peak2_detect_rbus_data;

    agc_peak2_detect_rbus_data = rtd_inl(IFD_AGC_PEAK2_DETECT_VADDR);

    return (agc_peak2_detect_rbus_data>>12)&0x1;
}
/*=============================================================*/
/**
 * drv_ifd_agc_peak2_target
 *
 *
 * @param <agc_peak2_value>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_peak2_target(UINT16 agc_peak2_value)
{
    rtd_maskl(IFD_AGC_PEAK2_VALUE_VADDR, ~((UINT32)0xFFF), (UINT32)agc_peak2_value&0xFFF);
}
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_get_loop2_peak_error
 *
 *
 * @param {void}
 * @return  <error>
 */
UINT16 drv_ifd_agc_get_loop2_peak_error()
{
    UINT32 agc_peak2_status_rbus_data;

    agc_peak2_status_rbus_data = rtd_inl(IFD_AGC_PEAK2_STATUS_VADDR);

    return agc_peak2_status_rbus_data&0xFFF;
}
/*=============================================================*/
/**
 * drv_ifd_agc_get_loop2_peak_max
 *
 *
 * @param {void}
 * @return  <max>
 */
UINT16 drv_ifd_agc_get_loop2_peak_max()
{
    UINT32 agc_loop2_peak_max_rbus_data;

    agc_loop2_peak_max_rbus_data = rtd_inl(IFD_AGC_PEAK2_STATUS_VADDR);

    return (UINT16)((agc_loop2_peak_max_rbus_data >> 16)&0xFFF);
}
/*=============================================================*/
/**
 * drv_ifd_agc_get_loop2_lock
 *
 *
 * @param {void}
 * @return  <agc_lock2>
 */
bool drv_ifd_agc_get_loop2_lock()
{
    UINT32 agc_lock2_th_rbus_data;

    agc_lock2_th_rbus_data = rtd_inl(IFD_AGC_LOCK2_TH_VADDR);

    return (agc_lock2_th_rbus_data>>28)&0x1;
}
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_fine_tune_en
 *
 *
 * @param <enable>
 * @return { void }
 */
void drv_ifd_agc_loop2_fine_tune_en(bool enable)
{
    rtd_maskl(IFD_AGC_2STEP_EN_VADDR, ~_BIT28, ((UINT32)enable)<<28);
}
/*=============================================================*/
/**
 * drv_ifd_agc_2step_en
 *
 *
 * @param <enable>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_2step_en(bool enable)
{
    rtd_maskl(IFD_AGC_2STEP_EN_VADDR, ~_BIT24, ((UINT32)enable)<<24);
}
#endif
#if 0
/*=============================================================*/
/**
 * drv_ifd_get_agc_2step_en
 *
 *
 * @param <void>
 * @return <enable>
 */
bool drv_ifd_get_agc_2step_en()
{
    UINT32 agc_2step_en_rbus_data;

    agc_2step_en_rbus_data = rtd_inl(IFD_AGC_2STEP_EN_VADDR);

    return (agc_2step_en_rbus_data>>24)&0x1;
}
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_pos_en
 *
 *
 * @param <enable>
 * @return { void }
 */
void drv_ifd_agc_loop2_pos_en(bool enable)
{
    rtd_maskl(IFD_AGC_2STEP_EN_VADDR, ~_BIT12, (UINT32)enable<<12);
}
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_diff_target
 *
 *
 * @param <agc_diff_targ2>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_loop2_diff_target(UINT16 agc_diff_targ2)
{
    rtd_maskl(IFD_AGC_2STEP_EN_VADDR, ~((UINT32)0x7FF), (UINT32)agc_diff_targ2&0x7FF);
}
#endif
#if 0
/*=============================================================*/
/**
 * drv_ifd_agc_dgain_up_dn_step
 *
 *
 * @param <agc_dgain_up_step><agc_dgain_down_step>
 * @return { void }
 */
void drv_ifd_agc_dgain_up_dn_step(UINT16 agc_dgain_up_step, UINT16 agc_dgain_down_step)
{
    rtd_outl(IFD_AGC_DGAIN_STEP_VADDR, ((UINT32)agc_dgain_up_step<<16)|(UINT32)agc_dgain_down_step);
}
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_dgain_up_dn_boundary
 *
 *
 * @param <agc_dgain_low_thr><agc_dgain_up_thr>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_dgain_up_dn_boundary(UINT16 agc_dgain_low_thr, UINT16 agc_dgain_up_thr)
{
    rtd_outl(IFD_AGC_DGAIN_THR_VADDR, ((UINT32)agc_dgain_low_thr<<16)|(UINT32)agc_dgain_up_thr);
}
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_2step_pga_step
 *
 *
 * @param <agc_step_num_pga>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_2step_pga_step(UINT8 agc_step_num_pga)
{
    rtd_maskl(IFD_AGC_DGAIN_TO_PGA_VADDR, ~(((UINT32)0xF)<<20), ((UINT32)agc_step_num_pga&0xF)<<20);
}
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_2step_pga_minimum
 *
 *
 * @param <agc_dgain_min_pga>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_2step_pga_minimum(UINT32 agc_dgain_min_pga)
{
    rtd_maskl(IFD_AGC_DGAIN_TO_PGA_VADDR, ~(((UINT32)0xFF)<<24), ((UINT32)agc_dgain_min_pga&0xFF)<<24);
}
#endif
#if 0
/*=============================================================*/
/**
 * drv_ifd_agc_2step_pga_minimum_get_info
 *
 *
 * @param <void>
 * @return < agc_dgain_min_pga >
 */
UINT32 drv_ifd_agc_2step_pga_minimum_get_info()
{
    UINT32 agc_dgain_to_pga_rbus_data;

    agc_dgain_to_pga_rbus_data = rtd_inl(IFD_AGC_DGAIN_TO_PGA_VADDR);

    return (agc_dgain_to_pga_rbus_data>>24)&0xFF;
}
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_fine_lock
 *
 *
 * @param {void}
 * @return  <fine_lock2>
 */
bool drv_ifd_agc_loop2_fine_lock()
{
    UINT32 agc_2step_status_rbus_data;

    agc_2step_status_rbus_data = rtd_inl(IFD_2STEP_AGC_STATUS_VADDR);

    return (agc_2step_status_rbus_data>>24)&0x1;

}
/*=============================================================*/
/**
 * drv_ifd_bypass_demod
 *
 *
 * @param <bypass>
 * @return { void }
 */
void drv_ifd_bypass_demod(bool bypass)
{
    rtd_maskl(IFD_AD_BYPASS_ENABLE_VADDR, ~_BIT0, (UINT32)bypass);
}
#endif
/*=============================================================*/
/**
 * drv_ifd_adc_input_sel
 *
 *
 * @param <input_source>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_adc_input_sel(IFD_ADC_INPUT input_source)
{
    rtd_maskl(IFD_AIN_SEL_VADDR, ~(_BIT1|_BIT0), (UINT32)input_source&0x3);
    if (input_source ==IFD_ADC_INPUT_IFPGA)
        rtd_maskl(IFD_ADC_IFPGAPOW_VADDR, ~_BIT0, _BIT0);
    else
        rtd_maskl(IFD_ADC_IFPGAPOW_VADDR, ~_BIT0, 0);
}
#endif

/*=============================================================*/
/**
 * drv_ifd_adc_clk_sel
 *
 *
 * @param <clk_rate>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_adc_clk_sel(IFD_CLK_RATE clk_rate)
{
    switch (clk_rate)
    {
    case IFD_CLK_RATE_108M:
        rtd_maskl(IFD_INPUT_BYPASS_ENABLE_VADDR, ~_BIT0, 0);
        rtd_maskl(IFD_ADC_CK_VADDR, ~_BIT7, _BIT7);
        rtd_maskl(IFD_ADC_ANALOG_RESERVE_VADDR, ~_BIT26, _BIT26);
        break;
    case IFD_CLK_RATE_54M:
        rtd_maskl(IFD_INPUT_BYPASS_ENABLE_VADDR, ~_BIT0, _BIT0);
        rtd_maskl(IFD_ADC_CK_VADDR, ~_BIT7, _BIT7);
        rtd_maskl(IFD_ADC_ANALOG_RESERVE_VADDR, ~_BIT26, 0);
        break;
    case IFD_CLK_RATE_24P576M:
        rtd_maskl(IFD_INPUT_BYPASS_ENABLE_VADDR, ~_BIT0, _BIT0);
        rtd_maskl(IFD_ADC_CK_VADDR, ~_BIT7, 0);
        rtd_maskl(IFD_ADC_ANALOG_RESERVE_VADDR, ~_BIT26, 0);
        break;
    default:
        break;
    }
}
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_mode_sel
 *
 *
 * @param <agc_mode>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_mode_sel(IFD_AGC_MODE agc_mode)
{
    //IFD_DRV_DEBUG_MSG(IFD_LOG_LEVEL, "drv_ifd_agc_mode_sel(%x)\n", (UINT32)agc_mode);
    switch (agc_mode)
    {
    case IFD_AGC_MODE_ORIGINAL:
        drv_ifd_agc_original_mode(IFD_ENABLE);
        drv_ifd_agc_2step_en(IFD_DISABLE);
        break;
    case IFD_AGC_MODE_2LOOP:
        drv_ifd_agc_original_mode(IFD_DISABLE);
        drv_ifd_agc_2step_en(IFD_DISABLE);
        break;
    case IFD_AGC_MODE_2STEP:
        drv_ifd_agc_original_mode(IFD_DISABLE);
        drv_ifd_agc_2step_en(IFD_ENABLE);
        break;
    default:
        break;
    }

}
#endif
#if 0
/*=============================================================*/
/**
 * drv_ifd_get_agc_mode_sel
 *
 *
 * @param <void>
 * @return <agc_mode>
 */
IFD_AGC_MODE drv_ifd_get_agc_mode_sel()
{
    if (drv_ifd_get_agc_original_mode())
        return IFD_AGC_MODE_ORIGINAL;
    else if (drv_ifd_get_agc_2step_en())
        return IFD_AGC_MODE_2STEP;
    else
        return IFD_AGC_MODE_2LOOP;

}
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_vsync_start1
 *
 *
 * @param <start><end>
 * @return { void }
 */
void drv_ifd_agc_vsync_start1(UINT16 start, UINT16 end)
{
    rtd_outl(IFD_AGC_VSYNC_START1_VADDR, ((UINT32)start<<16)|((UINT32)end));
}

/*=============================================================*/
/**
 * drv_ifd_agc_hsync_start1
 *
 *
 * @param <start><end>
 * @return { void }
 */
void drv_ifd_agc_hsync_start1(UINT16 start, UINT16 end)
{
    rtd_outl(IFD_AGC_HSYNC_START_VADDR, ((UINT32)start<<8)|((UINT32)end));
}
/*=============================================================*/
/**
 * drv_ifd_agc_vsync_start2
 *
 *
 * @param <start><end>
 * @return { void }
 */
void drv_ifd_agc_vsync_start2(UINT16 start, UINT16 end)
{
    rtd_outl(IFD_AGC_VSYNC_START2_VADDR, ((UINT32)start<<16)|((UINT32)end));
}
/*=============================================================*/
/**
 * drv_ifd_agc_hsync_start2
 *
 *
 * @param <start><end>
 * @return { void }
 */
void drv_ifd_agc_hsync_start2(UINT16 start, UINT16 end)
{
    rtd_outl(IFD_AGC_HSYNC_START2_VADDR, ((UINT32)start<<8)|((UINT32)end));
}
/*=============================================================*/
/**
 * drv_ifd_agc_loop1_finetune_dn_sample
 *
 *
 * @param <dn_sample>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_loop1_finetune_dn_sample(UINT16 dn_sample)
{
    if (dn_sample == 0)
    {
        rtd_maskl(IFD_AGC_FINE_TUNE_DOWN_SAMPLE_VADDR, ~((UINT32)0xFFF), 0);
    }
    else
    {
        rtd_maskl(IFD_AGC_FINE_TUNE_DOWN_SAMPLE_VADDR, ~((UINT32)0xFFF), _BIT12|((UINT32)dn_sample&0xFFF));
    }
}
#endif
#if 0
/*=============================================================*/
/**
 * drv_ifd_agc_loop1_finetune_dn_sample_get_info
 *
 *
 * @param <void>
 * @return { dn_sample }
 */
UINT16 drv_ifd_agc_loop1_finetune_dn_sample_get_info(void)
{
    UINT32 agc_fine_tune_down_sample_rbus_data;

    agc_fine_tune_down_sample_rbus_data = rtd_inl(IFD_AGC_FINE_TUNE_DOWN_SAMPLE_VADDR);

    return (agc_fine_tune_down_sample_rbus_data&0xFFF);
}
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_finetune_dn_sample
 *
 *
 * @param <dn_sample>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_loop2_finetune_dn_sample(UINT16 dn_sample)
{
    rtd_maskl(IFD_AGC_FINE_TUNE_DOWN_SAMPLE_VADDR, ~(UINT32)(0xFFF<<16), _BIT28|(((UINT32)dn_sample&0xFFF)<<16));
}
#endif
#if 0
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_finetune_dn_sample_get_info
 *
 *
 * @param <void>
 * @return { dn_sample }
 */
UINT16 drv_ifd_agc_loop2_finetune_dn_sample_get_info(void)
{
    UINT32 agc_fine_tune_down_sample_rbus_data;

    agc_fine_tune_down_sample_rbus_data = rtd_inl(IFD_AGC_FINE_TUNE_DOWN_SAMPLE_VADDR);

    return (((agc_fine_tune_down_sample_rbus_data&0xFFF)>>16)>>4);
}
/*=============================================================*/
/**
 * drv_ifd_agc_loop1_dn_sample
 *
 *
 * @param <dnsample>
 * @return { void }
 */
void drv_ifd_agc_loop1_dn_sample (UINT16 dnsample)
{
    rtd_maskl(IFD_AGC_MAX_COUNT_FOR_COARSE_MODE_AGC_LOCK2_VADDR, ~((UINT32)0xFFF0), ((UINT32)(dnsample&0xFFF))<<4);
}
/*=============================================================*/
/**
 * drv_ifd_agc_loop1_dn_sample_get_info
 *
 *
 * @param <void>
 * @return { dnsample }
 */
UINT16 drv_ifd_agc_loop1_dn_sample_get_info(void)
{
    UINT32 agc_max_count_for_coarse_mode_agc_lock2_rbus_data;

    agc_max_count_for_coarse_mode_agc_lock2_rbus_data = rtd_inl(IFD_AGC_MAX_COUNT_FOR_COARSE_MODE_AGC_LOCK2_VADDR);

    return (agc_max_count_for_coarse_mode_agc_lock2_rbus_data>>4)&0xFFF;
}
#endif
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_dn_sample
 *
 *
 * @param <dnsample>
 * @return { void }
 */
void drv_ifd_agc_loop2_dn_sample(UINT16 dnsample)
{
    rtd_maskl(IFD_AGC_LOOP2_CNTL_VADDR, ~((UINT32)0xFFF), (UINT32)(dnsample&0xFFF));
}
#if 0
/*=============================================================*/
/**
 * drv_ifd_agc_loop2_dn_sample_get_info
 *
 *
 * @param <void>
 * @return { dnsample }
 */
UINT16 drv_ifd_agc_loop2_dn_sample_get_info(void)
{
    UINT32 agc_loop2_cntl_rbus_data;

    agc_loop2_cntl_rbus_data = rtd_inl(IFD_AGC_LOOP2_CNTL_VADDR);

    return (agc_loop2_cntl_rbus_data&0xFFF);
}
#endif
/*=============================================================*/
/**
 * drv_ifd_set_vd_path
 *
 *
 * @param <void>
 * @return {void}
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_set_vd_path(void)
{
    rtd_maskl(VDC_INOUT_CTRL_VADDR,(UINT32)0xfffffff0,(UINT32)0x0000000b);	// vd clamp enable
    rtd_maskl(VDC_AGC_GATE_TH_SWAP_VADDR,(UINT32)0xffffff7f,(UINT32)0x00000080);  // dc clamp up dn swap
}
#endif
/*=============================================================*/
/**
 * drv_ifd_set_cr_error_var_sel
 *
 *
 * @param <enable>
 * @return { void }
 */
void  drv_ifd_set_cr_error_var_sel(bool sel)
{

    if (IFD_CHIP_VER() >= 2)
    {
        if (sel)
            rtd_maskl(IFD_DUMMY1_VADDR, (UINT32)0xFFFEFFFF, (UINT32)0x10000);
        else
            rtd_maskl(IFD_DUMMY1_VADDR, (UINT32)0xFFFEFFFF, (UINT32)0);
    }

}
/*=============================================================*/
/**
 * drv_ifd_set_agc_cr_var_sel
 *
 *
 * @param <enable>
 * @return { void }
 */
void  drv_ifd_set_agc_cr_var_sel(bool sel)
{
    if (IFD_CHIP_VER() >= 2)
    {
        if (sel)
            rtd_maskl(IFD_DUMMY1_VADDR, (UINT32)0xFFFFEFFF, (UINT32)0x1000);
        else
            rtd_maskl(IFD_DUMMY1_VADDR, (UINT32)0xFFFFEFFF, (UINT32)0);
    }
}

/*=============================================================*/
/**
 * drv_ifd_cr_wblf_reset
 *
 *
 * @param <enable>
 * @return { void }
 */
void drv_ifd_cr_wblf_reset(bool enable)
{
    if (IFD_CHIP_VER() >= 2)
    {
        if (enable)
            rtd_maskl(IFD_CR_WBLF_CNTL_VADDR, (UINT32)0xFFFFFEFF, (UINT32)0x100);
        else
            rtd_maskl(IFD_CR_WBLF_CNTL_VADDR, (UINT32)0xFFFFFEFF, (UINT32)0);
    }
}
/*=============================================================*/
/**
 * drv_ifd_agc_get_loop2_max_out
 *
 *
 * @param <void>
 * @return { UINT16 }
 */
UINT16 drv_ifd_agc_get_loop2_max_out()
{
    UINT32 agc_max2_in_out_rbus_data;

    agc_max2_in_out_rbus_data = rtd_inl(IFD_AGC_MAX2_IN_OUT_VADDR);

    return (UINT16)(agc_max2_in_out_rbus_data&0x7FF);
}
#endif
