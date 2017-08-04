// This file is generated using the spec version 1.39, firmware template version 1.39and SRIF Parser                                                                                source code SVN rev:743                    Version flow no.:1.0.197
#ifndef _SYNCPROCESSOR_REG_H_INCLUDED_
#define _SYNCPROCESSOR_REG_H_INCLUDED_

//#define  _SYNCPROCESSOR_USE_STRUCT
#ifdef _SYNCPROCESSOR_USE_STRUCT

typedef struct
{
    unsigned int    s1_dehs_out_en:1;
    unsigned int    dummy18021000_30:1;
    unsigned int    dummy18021000_29:1;
    unsigned int    s1_hsraw_inv:1;
    unsigned int    dummy18021000_27:1;
    unsigned int    dummy18021000_26_24:3;
    unsigned int    hrecovery_en:1;
    unsigned int    s1_adc_vsout_testmd:1;
    unsigned int    s1_devs_out_en:1;
    unsigned int    s1_sel_clpmsk_as_devs:1;
    unsigned int    s1_hsout_inv:1;
    unsigned int    dummy18021000_18:1;
    unsigned int    s1_hs_sync_source:1;
    unsigned int    s1_hsout_en:1;
    unsigned int    dummy18021000_15_13:3;
    unsigned int    s1_seperate_testmode:1;
    unsigned int    s1_auto_run_done_ie:1;
    unsigned int    s1_auto_run:1;
    unsigned int    s1_coast_en:1;
    unsigned int    s1_decmp_en:1;
    unsigned int    dummy18021000_7:1;
    unsigned int    s1_hs_fb_plswid_md:2;
    unsigned int    s1_coast_inv:1;
    unsigned int    dummy18021000_3:1;
    unsigned int    s1_vhs_source_sel:3;
} REG18021000;

typedef struct
{
    unsigned int    s1_auto_result_rdy:1;
    unsigned int    reserved_0:24;
    unsigned int    s1_auto_result:3;
    unsigned int    reserved_1:3;
    unsigned int    s1_input_signal_be_inverted:1;
} REG18021004;

typedef struct
{
    unsigned int    s2_dehs_out_en:1;
    unsigned int    dummy18021008_30_29:2;
    unsigned int    s2_hsraw_inv:1;
    unsigned int    dummy18021008_27_23:5;
    unsigned int    s2_adc_vsout_testmd:1;
    unsigned int    s2_devs_out_en:1;
    unsigned int    s2_sel_clpmsk_as_devs:1;
    unsigned int    dummy18021008_19_13:7;
    unsigned int    s2_seperate_testmode:1;
    unsigned int    s2_auto_run_done_ie:1;
    unsigned int    s2_auto_run:1;
    unsigned int    dummy18021008_9:1;
    unsigned int    s2_decmp_en:1;
    unsigned int    dummy18021008_7_3:5;
    unsigned int    s2_vhs_source_sel:3;
} REG18021008;

typedef struct
{
    unsigned int    s2_auto_result_rdy:1;
    unsigned int    reserved_0:24;
    unsigned int    s2_auto_result:3;
    unsigned int    reserved_1:3;
    unsigned int    s2_input_signal_be_inverted:1;
} REG1802100C;

typedef struct
{
    unsigned int    reserved_0:15;
    unsigned int    s1_cap_win_clo:1;
    unsigned int    s1_cap_win_range:5;
    unsigned int    s1_per_range:2;
    unsigned int    dummy18021010_8:1;
    unsigned int    s1_popup_stable_per:1;
    unsigned int    dummy18021010_6_4:3;
    unsigned int    s1_cap_win_step:1;
    unsigned int    s1_miss_lock_limit:1;
    unsigned int    dummy18021010_1:1;
    unsigned int    s1_stable_en:1;
} REG18021010;

typedef struct
{
    unsigned int    s1_mode_det_of:1;
    unsigned int    s1_stb_per_chg:1;
    unsigned int    s1_stb_pol_chg:1;
    unsigned int    s1_vs_raw_vld:1;
    unsigned int    s1_cap_32unlock:1;
    unsigned int    s1_eq_occur:1;
    unsigned int    s1_hs_pol_chg:1;
    unsigned int    s1_cap_unlock:1;
    unsigned int    s1_mode_det_stable:1;
    unsigned int    s1_stable_pol_out:1;
    unsigned int    reserved_0:22;
} REG18021014;

typedef struct
{
    unsigned int    reserved_0:5;
    unsigned int    s1_stable_period_out:11;
    unsigned int    reserved_1:5;
    unsigned int    s1_stable_plswid_out:11;
} REG18021018;

typedef struct
{
    unsigned int    reserved_0:15;
    unsigned int    s2_cap_win_clo:1;
    unsigned int    s2_cap_win_range:5;
    unsigned int    s2_per_range:2;
    unsigned int    dummy1802101c_8:1;
    unsigned int    s2_popup_stable_per:1;
    unsigned int    dummy1802101c_6_4:3;
    unsigned int    s2_cap_win_step:1;
    unsigned int    s2_miss_lock_limit:1;
    unsigned int    dummy1802101c_1:1;
    unsigned int    s2_stable_en:1;
} REG1802101C;

typedef struct
{
    unsigned int    s2_mode_det_of:1;
    unsigned int    s2_stb_per_chg:1;
    unsigned int    s2_stb_pol_chg:1;
    unsigned int    s2_vs_raw_vld:1;
    unsigned int    s2_cap_32unlock:1;
    unsigned int    s2_eq_occur:1;
    unsigned int    s2_hs_pol_chg:1;
    unsigned int    s2_cap_unlock:1;
    unsigned int    s2_mode_det_stable:1;
    unsigned int    s2_stable_pol_out:1;
    unsigned int    reserved_0:22;
} REG18021020;

typedef struct
{
    unsigned int    reserved_0:5;
    unsigned int    s2_stable_period_out:11;
    unsigned int    reserved_1:5;
    unsigned int    s2_stable_plswid_out:11;
} REG18021024;

typedef struct
{
    unsigned int    reserved_0:16;
    unsigned int    s1_deb_num:5;
    unsigned int    s1_dcnt_level:11;
} REG18021028;

typedef struct
{
    unsigned int    reserved_0:16;
    unsigned int    s2_deb_num:5;
    unsigned int    s2_dcnt_level:11;
} REG1802102C;

typedef struct
{
    unsigned int    reserved_0:24;
    unsigned int    s1_field_det_mode:1;
    unsigned int    s1_f_pos_field:1;
    unsigned int    s1_f_pos_field_tgle:1;
    unsigned int    s1_f_count_field:1;
    unsigned int    s1_f_count_field_tgle:1;
    unsigned int    s1_l_cnt_lsb1:3;
} REG18021030;

typedef struct
{
    unsigned int    dummy18021034_31_8:24;
    unsigned int    s2_field_det_mode:1;
    unsigned int    s2_f_pos_field:1;
    unsigned int    s2_f_pos_field_tgle:1;
    unsigned int    s2_f_count_field:1;
    unsigned int    s2_f_count_field_tgle:1;
    unsigned int    s2_l_cnt_lsb1:3;
} REG18021034;

typedef struct
{
    unsigned int    reserved_0:18;
    unsigned int    dummy18021038_13_8:6;
    unsigned int    s1_clamp_dben:1;
    unsigned int    s1_clamp_cnt_unit:1;
    unsigned int    s1_clamp_clksrc:1;
    unsigned int    s1_clamp_en:1;
    unsigned int    s1_yclamp_hs_inv:1;
    unsigned int    s1_nyclamp_hs_inv:1;
    unsigned int    s1_clamp_test_en:1;
    unsigned int    s1_adc_hs_syncedge:1;
} REG18021038;

typedef struct
{
    unsigned int    s1_yclamp_sta:8;
    unsigned int    s1_yclamp_end:8;
    unsigned int    s1_nyclamp_sta:8;
    unsigned int    s1_nyclamp_end:8;
} REG1802103C;

typedef struct
{
    unsigned int    reserved_0:16;
    unsigned int    s1_mask_clamp_en:1;
    unsigned int    dummy18021040_14:1;
    unsigned int    s1_mask_front:6;
    unsigned int    dummy18021040_7_6:2;
    unsigned int    s1_mask_back:6;
} REG18021040;

typedef struct
{
    unsigned int    reserved_0:22;
    unsigned int    dummy18021044_9_8:2;
    unsigned int    s1_coast_front:4;
    unsigned int    s1_coast_back:4;
} REG18021044;

typedef struct
{
    unsigned int    reserved_0:18;
    unsigned int    dummy18021048_13_8:6;
    unsigned int    s2_clamp_dben:1;
    unsigned int    s2_clamp_cnt_unit:1;
    unsigned int    s2_clamp_clksrc:1;
    unsigned int    s2_clamp_en:1;
    unsigned int    s2_yclamp_hs_inv:1;
    unsigned int    dummy18021048_2:1;
    unsigned int    s2_clamp_test_en:1;
    unsigned int    s2_adc_hs_syncedge:1;
} REG18021048;

typedef struct
{
    unsigned int    s2_yclamp_sta:8;
    unsigned int    s2_yclamp_end:8;
    unsigned int    reserved_0:16;
} REG1802104C;

typedef struct
{
    unsigned int    reserved_0:16;
    unsigned int    s2_mask_clamp_en:1;
    unsigned int    dummy18021050_14:1;
    unsigned int    s2_mask_front:6;
    unsigned int    dummy18021050_7_6:2;
    unsigned int    s2_mask_back:6;
} REG18021050;

typedef struct
{
    unsigned int    reserved_0:32;
} REG18021054;

typedef struct
{
    unsigned int    reserved_0:16;
    unsigned int    dummy18021058_15_13:3;
    unsigned int    s1_deb_num_mvhs:5;
    unsigned int    s1_skip_line_len:4;
    unsigned int    dummy18021058_3_2:2;
    unsigned int    s1_mv_occur:1;
    unsigned int    s1_mv_enable:1;
} REG18021058;

typedef struct
{
    unsigned int    dummy1802105c_31:1;
    unsigned int    s1_o_mv_str:7;
    unsigned int    s1_o_mv_rdy:1;
    unsigned int    s1_o_mv_end:7;
    unsigned int    dummy1802105c_15:1;
    unsigned int    s1_e_mv_str:7;
    unsigned int    s1_e_mv_rdy:1;
    unsigned int    s1_e_mv_end:7;
} REG1802105C;

typedef struct
{
    unsigned int    reserved_0:16;
    unsigned int    dummy18021060_15_13:3;
    unsigned int    s2_deb_num_mvhs:5;
    unsigned int    s2_skip_line_len:4;
    unsigned int    dummy18021060_3_1:3;
    unsigned int    s2_mv_enable:1;
} REG18021060;

typedef struct
{
    unsigned int    reserved_0:2;
    unsigned int    vd_adc_clamp11_sel:1;
    unsigned int    vd_adc_clamp10_sel:1;
    unsigned int    vd_adc_clamp9_sel:1;
    unsigned int    vd_adc_clamp8_sel:1;
    unsigned int    vd_adc_clamp7_sel:1;
    unsigned int    vd_adc_clamp6_sel:1;
    unsigned int    vd_adc_clamp5_sel:1;
    unsigned int    vd_adc_clamp4_sel:1;
    unsigned int    vd_adc_clamp3_sel:1;
    unsigned int    vd_adc_clamp2_sel:1;
    unsigned int    vd_adc_clamp1_sel:1;
    unsigned int    vd_adc_clamp0_sel:1;
    unsigned int    adc_r0_clamp_sel:2;
    unsigned int    adc_r1_clamp_sel:2;
    unsigned int    adc_r2_clamp_sel:2;
    unsigned int    adc_g0_clamp_sel:2;
    unsigned int    adc_g1_clamp_sel:2;
    unsigned int    adc_g2_clamp_sel:2;
    unsigned int    adc_b0_clamp_sel:2;
    unsigned int    adc_b1_clamp_sel:2;
    unsigned int    adc_b2_clamp_sel:2;
} SP_adcclampsel0;

typedef struct
{
    unsigned int    reserved_0:27;
    unsigned int    dummy18021068_4:1;
    unsigned int    sync_test_sel:4;
} SP_adcclampsel1;

typedef struct
{
    unsigned int    reserved_0:23;
    unsigned int    syncedge_rgb:1;
    unsigned int    syncedge_hs:1;
    unsigned int    syncedge_vs:1;
    unsigned int    test_sel_rgb:2;
    unsigned int    test_sel_hs:1;
    unsigned int    test_sel_vs:1;
    unsigned int    sync_out_source:2;
} SP_adc_out_sel;

typedef struct
{
    unsigned int    reserved_0:24;
    unsigned int    dummy18021070_7_6:2;
    unsigned int    tog_adc_sog0:1;
    unsigned int    tog_adc_sog1:1;
    unsigned int    tog_soy_vd:1;
    unsigned int    reserved_1:1;
    unsigned int    tog_pad_avs1:1;
    unsigned int    tog_pad_ahs1:1;
} SP_pad_status;

typedef struct
{
    unsigned int    dummy18021100_31:1;
    unsigned int    reserved_0:9;
    unsigned int    off_ms_now:1;
    unsigned int    off_timeclk_test_mode:1;
    unsigned int    off_meas_source:4;
    unsigned int    off_vs_meas_inv:1;
    unsigned int    off_vsms_inv:1;
    unsigned int    off_sycms_mode:1;
    unsigned int    off_popup_meas:1;
    unsigned int    off_hsms_inv:1;
    unsigned int    off_online_en:1;
    unsigned int    off_start_ms:1;
    unsigned int    off_hs_max_delta:5;
    unsigned int    off_vs_max_delta:4;
} REG18021100;

typedef struct
{
    unsigned int    reserved_0:4;
    unsigned int    off_vs_period_out:12;
    unsigned int    off_hs_period_out_h12b:12;
    unsigned int    off_hs_period_out_f4b:4;
} REG18021104;

typedef struct
{
    unsigned int    dummy18021108_31_28:4;
    unsigned int    off_vs_high_out:12;
    unsigned int    dummy18021108_15_12:4;
    unsigned int    off_hs_high_out:12;
} REG18021108;

typedef struct
{
    unsigned int    reserved_0:20;
    unsigned int    dummy1802110c_11_10:2;
    unsigned int    off_vs_pol_out:1;
    unsigned int    off_hs_pol_out:1;
    unsigned int    off_vs_per_to:1;
    unsigned int    off_vs_high_to:1;
    unsigned int    off_vs_per_of:1;
    unsigned int    off_vs_over_range:1;
    unsigned int    off_vs_pol_chg:1;
    unsigned int    off_hs_per_of:1;
    unsigned int    off_hs_over_range:1;
    unsigned int    off_hs_pol_chg:1;
} SP_ms3stus;

typedef struct
{
    unsigned int    ie_off_msdone:1;
    unsigned int    reserved_0:19;
    unsigned int    dummy18021110_11_8:4;
    unsigned int    ie_off_vs_per_to:1;
    unsigned int    ie_off_vs_high_to:1;
    unsigned int    ie_off_vs_per_of:1;
    unsigned int    ie_off_vs_over_range:1;
    unsigned int    ie_off_vs_pol_chg:1;
    unsigned int    ie_off_hs_per_of:1;
    unsigned int    ie_off_hs_over_range:1;
    unsigned int    ie_off_hs_pol_chg:1;
} SP_ms3ie;

typedef struct
{
    unsigned int    reserved_0:32;
} REG18021114;

typedef struct
{
    unsigned int    reserved_0:24;
    unsigned int    off_ena_source:1;
    unsigned int    off_ms_act_en:1;
    unsigned int    dummy18021118_5_0:6;
} SP_ms3enatvctrl;

typedef struct
{
    unsigned int    reserved_0:8;
    unsigned int    off_ena_region_out:24;
} REG1802111C;

typedef struct
{
    unsigned int    reserved_0:12;
    unsigned int    dummy18021200_19_17:3;
    unsigned int    on1_sycms_clk:1;
    unsigned int    on1_vs_meas_inv:1;
    unsigned int    on1_vsms_inv:1;
    unsigned int    on1_sycms_mode:1;
    unsigned int    on1_popup_meas:1;
    unsigned int    dummy18021200_11:1;
    unsigned int    on1_online_en:1;
    unsigned int    on1_start_ms:1;
    unsigned int    on1_hs_max_delta:5;
    unsigned int    on1_vs_max_delta:4;
} REG18021200;

typedef struct
{
    unsigned int    dummy18021204_31_28:4;
    unsigned int    on1_vs_period_out:12;
    unsigned int    on1_hs_period_out_h12b:12;
    unsigned int    on1_hs_period_out_f4b:4;
} REG18021204;

typedef struct
{
    unsigned int    dummy18021208_31_28:4;
    unsigned int    on1_vs_high_out:12;
    unsigned int    dummy18021208_15_12:4;
    unsigned int    on1_hs_high_out:12;
} REG18021208;

typedef struct
{
    unsigned int    reserved_0:20;
    unsigned int    dummy1802120c_11_10:2;
    unsigned int    on1_vs_pol_out:1;
    unsigned int    on1_hs_pol_out:1;
    unsigned int    on1_vs_per_to:1;
    unsigned int    on1_vs_high_to:1;
    unsigned int    on1_vs_per_of:1;
    unsigned int    on1_vs_over_range:1;
    unsigned int    on1_vs_pol_chg:1;
    unsigned int    on1_hs_per_of:1;
    unsigned int    on1_hs_over_range:1;
    unsigned int    on1_hs_pol_chg:1;
} REG1802120C;

typedef struct
{
    unsigned int    wd_on1_to_main:1;
    unsigned int    wd_on1_to_sub:1;
    unsigned int    reserved_0:6;
    unsigned int    wde_on1_vs_per_to:1;
    unsigned int    wde_on1_vs_high_to:1;
    unsigned int    wde_on1_vs_per_of:1;
    unsigned int    wde_on1_vs_over_range:1;
    unsigned int    wde_on1_vs_pol_chg:1;
    unsigned int    wde_on1_hs_per_of:1;
    unsigned int    wde_on1_hs_over_range:1;
    unsigned int    wde_on1_hs_pol_chg:1;
    unsigned int    dummy18021210_15_8:8;
    unsigned int    on1_ie_vs_per_to:1;
    unsigned int    on1_ie_vs_high_to:1;
    unsigned int    on1_ie_vs_per_of:1;
    unsigned int    on1_ie_vs_over_range:1;
    unsigned int    on1_ie_vs_pol_chg:1;
    unsigned int    on1_ie_hs_per_of:1;
    unsigned int    on1_ie_hs_over_range:1;
    unsigned int    on1_ie_hs_pol_chg:1;
} REG18021210;

typedef struct
{
    unsigned int    reserved_0:12;
    unsigned int    dummy18021214_19_17:3;
    unsigned int    on2_sycms_clk:1;
    unsigned int    on2_vs_meas_inv:1;
    unsigned int    on2_vsms_inv:1;
    unsigned int    on2_sycms_mode:1;
    unsigned int    on2_popup_meas:1;
    unsigned int    dummy18021214_11:1;
    unsigned int    on2_online_en:1;
    unsigned int    on2_start_ms:1;
    unsigned int    on2_hs_max_delta:5;
    unsigned int    on2_vs_max_delta:4;
} REG18021214;

typedef struct
{
    unsigned int    dummy18021218_31_28:4;
    unsigned int    on2_vs_period_out:12;
    unsigned int    on2_hs_period_out_h12b:12;
    unsigned int    on2_hs_period_out_f4b:4;
} REG18021218;

typedef struct
{
    unsigned int    dummy1802121c_31_28:4;
    unsigned int    on2_vs_high_out:12;
    unsigned int    dummy1802121c_15_12:4;
    unsigned int    on2_hs_high_out:12;
} REG1802121C;

typedef struct
{
    unsigned int    reserved_0:22;
    unsigned int    on2_vs_pol_out:1;
    unsigned int    on2_hs_pol_out:1;
    unsigned int    on2_vs_per_to:1;
    unsigned int    on2_vs_high_to:1;
    unsigned int    on2_vs_per_of:1;
    unsigned int    on2_vs_over_range:1;
    unsigned int    on2_vs_pol_chg:1;
    unsigned int    on2_hs_per_of:1;
    unsigned int    on2_hs_over_range:1;
    unsigned int    on2_hs_pol_chg:1;
} REG18021220;

typedef struct
{
    unsigned int    wd_on2_to_main:1;
    unsigned int    wd_on2_to_sub:1;
    unsigned int    dummy18021224_29_24:6;
    unsigned int    wde_on2_vs_per_to:1;
    unsigned int    wde_on2_vs_high_to:1;
    unsigned int    wde_on2_vs_per_of:1;
    unsigned int    wde_on2_vs_over_range:1;
    unsigned int    wde_on2_vs_pol_chg:1;
    unsigned int    wde_on2_hs_per_of:1;
    unsigned int    wde_on2_hs_over_range:1;
    unsigned int    wde_on2_hs_pol_chg:1;
    unsigned int    dummy18021224_15_8:8;
    unsigned int    reserved_0:1;
    unsigned int    on2_ie_vs_high_to:1;
    unsigned int    on2_ie_vs_per_of:1;
    unsigned int    on2_ie_vs_over_range:1;
    unsigned int    on2_ie_vs_pol_chg:1;
    unsigned int    on2_ie_hs_per_of:1;
    unsigned int    on2_ie_hs_over_range:1;
    unsigned int    on2_ie_hs_pol_chg:1;
} REG18021224;
#endif

#define SP1_CTRL                                                      0x18021000
#define SP1_CTRL_reg_addr                                             "0xB8021000"
#define SP1_CTRL_reg                                                  0xB8021000
#define SP1_CTRL_inst_addr                                            "0x0000"
#define SP1_CTRL_inst                                                 0x0000
#define SP1_CTRL_s1_dehs_out_en_shift                                 (31)
#define SP1_CTRL_dummy18021000_30_shift                               (30)
#define SP1_CTRL_dummy18021000_29_shift                               (29)
#define SP1_CTRL_s1_hsraw_inv_shift                                   (28)
#define SP1_CTRL_dummy18021000_27_shift                               (27)
#define SP1_CTRL_dummy18021000_26_24_shift                            (24)
#define SP1_CTRL_hrecovery_en_shift                                   (23)
#define SP1_CTRL_s1_adc_vsout_testmd_shift                            (22)
#define SP1_CTRL_s1_devs_out_en_shift                                 (21)
#define SP1_CTRL_s1_sel_clpmsk_as_devs_shift                          (20)
#define SP1_CTRL_s1_hsout_inv_shift                                   (19)
#define SP1_CTRL_dummy18021000_18_shift                               (18)
#define SP1_CTRL_s1_hs_sync_source_shift                              (17)
#define SP1_CTRL_s1_hsout_en_shift                                    (16)
#define SP1_CTRL_dummy18021000_15_13_shift                            (13)
#define SP1_CTRL_s1_seperate_testmode_shift                           (12)
#define SP1_CTRL_s1_auto_run_done_ie_shift                            (11)
#define SP1_CTRL_s1_auto_run_shift                                    (10)
#define SP1_CTRL_s1_coast_en_shift                                    (9)
#define SP1_CTRL_s1_decmp_en_shift                                    (8)
#define SP1_CTRL_dummy18021000_7_shift                                (7)
#define SP1_CTRL_s1_hs_fb_plswid_md_shift                             (5)
#define SP1_CTRL_s1_coast_inv_shift                                   (4)
#define SP1_CTRL_dummy18021000_3_shift                                (3)
#define SP1_CTRL_s1_vhs_source_sel_shift                              (0)
#define SP1_CTRL_s1_dehs_out_en_mask                                  (0x80000000)
#define SP1_CTRL_dummy18021000_30_mask                                (0x40000000)
#define SP1_CTRL_dummy18021000_29_mask                                (0x20000000)
#define SP1_CTRL_s1_hsraw_inv_mask                                    (0x10000000)
#define SP1_CTRL_dummy18021000_27_mask                                (0x08000000)
#define SP1_CTRL_dummy18021000_26_24_mask                             (0x07000000)
#define SP1_CTRL_hrecovery_en_mask                                    (0x00800000)
#define SP1_CTRL_s1_adc_vsout_testmd_mask                             (0x00400000)
#define SP1_CTRL_s1_devs_out_en_mask                                  (0x00200000)
#define SP1_CTRL_s1_sel_clpmsk_as_devs_mask                           (0x00100000)
#define SP1_CTRL_s1_hsout_inv_mask                                    (0x00080000)
#define SP1_CTRL_dummy18021000_18_mask                                (0x00040000)
#define SP1_CTRL_s1_hs_sync_source_mask                               (0x00020000)
#define SP1_CTRL_s1_hsout_en_mask                                     (0x00010000)
#define SP1_CTRL_dummy18021000_15_13_mask                             (0x0000E000)
#define SP1_CTRL_s1_seperate_testmode_mask                            (0x00001000)
#define SP1_CTRL_s1_auto_run_done_ie_mask                             (0x00000800)
#define SP1_CTRL_s1_auto_run_mask                                     (0x00000400)
#define SP1_CTRL_s1_coast_en_mask                                     (0x00000200)
#define SP1_CTRL_s1_decmp_en_mask                                     (0x00000100)
#define SP1_CTRL_dummy18021000_7_mask                                 (0x00000080)
#define SP1_CTRL_s1_hs_fb_plswid_md_mask                              (0x00000060)
#define SP1_CTRL_s1_coast_inv_mask                                    (0x00000010)
#define SP1_CTRL_dummy18021000_3_mask                                 (0x00000008)
#define SP1_CTRL_s1_vhs_source_sel_mask                               (0x00000007)
#define SP1_CTRL_s1_dehs_out_en(data)                                 (0x80000000&(((UINT32)data)<<31))
#define SP1_CTRL_dummy18021000_30(data)                               (0x40000000&(((UINT32)data)<<30))
#define SP1_CTRL_dummy18021000_29(data)                               (0x20000000&(((UINT32)data)<<29))
#define SP1_CTRL_s1_hsraw_inv(data)                                   (0x10000000&(((UINT32)data)<<28))
#define SP1_CTRL_dummy18021000_27(data)                               (0x08000000&(((UINT32)data)<<27))
#define SP1_CTRL_dummy18021000_26_24(data)                            (0x07000000&(((UINT32)data)<<24))
#define SP1_CTRL_hrecovery_en(data)                                   (0x00800000&(((UINT32)data)<<23))
#define SP1_CTRL_s1_adc_vsout_testmd(data)                            (0x00400000&(((UINT32)data)<<22))
#define SP1_CTRL_s1_devs_out_en(data)                                 (0x00200000&(((UINT32)data)<<21))
#define SP1_CTRL_s1_sel_clpmsk_as_devs(data)                          (0x00100000&(((UINT32)data)<<20))
#define SP1_CTRL_s1_hsout_inv(data)                                   (0x00080000&(((UINT32)data)<<19))
#define SP1_CTRL_dummy18021000_18(data)                               (0x00040000&(((UINT32)data)<<18))
#define SP1_CTRL_s1_hs_sync_source(data)                              (0x00020000&(((UINT32)data)<<17))
#define SP1_CTRL_s1_hsout_en(data)                                    (0x00010000&(((UINT32)data)<<16))
#define SP1_CTRL_dummy18021000_15_13(data)                            (0x0000E000&(((UINT32)data)<<13))
#define SP1_CTRL_s1_seperate_testmode(data)                           (0x00001000&(((UINT32)data)<<12))
#define SP1_CTRL_s1_auto_run_done_ie(data)                            (0x00000800&(((UINT32)data)<<11))
#define SP1_CTRL_s1_auto_run(data)                                    (0x00000400&(((UINT32)data)<<10))
#define SP1_CTRL_s1_coast_en(data)                                    (0x00000200&(((UINT32)data)<<9))
#define SP1_CTRL_s1_decmp_en(data)                                    (0x00000100&(((UINT32)data)<<8))
#define SP1_CTRL_dummy18021000_7(data)                                (0x00000080&(((UINT32)data)<<7))
#define SP1_CTRL_s1_hs_fb_plswid_md(data)                             (0x00000060&(((UINT32)data)<<5))
#define SP1_CTRL_s1_coast_inv(data)                                   (0x00000010&(((UINT32)data)<<4))
#define SP1_CTRL_dummy18021000_3(data)                                (0x00000008&(((UINT32)data)<<3))
#define SP1_CTRL_s1_vhs_source_sel(data)                              (0x00000007&(data))
#define SP1_CTRL_get_s1_dehs_out_en(data)                             ((0x80000000&(data))>>31)
#define SP1_CTRL_get_dummy18021000_30(data)                           ((0x40000000&(data))>>30)
#define SP1_CTRL_get_dummy18021000_29(data)                           ((0x20000000&(data))>>29)
#define SP1_CTRL_get_s1_hsraw_inv(data)                               ((0x10000000&(data))>>28)
#define SP1_CTRL_get_dummy18021000_27(data)                           ((0x08000000&(data))>>27)
#define SP1_CTRL_get_dummy18021000_26_24(data)                        ((0x07000000&(data))>>24)
#define SP1_CTRL_get_hrecovery_en(data)                               ((0x00800000&(data))>>23)
#define SP1_CTRL_get_s1_adc_vsout_testmd(data)                        ((0x00400000&(data))>>22)
#define SP1_CTRL_get_s1_devs_out_en(data)                             ((0x00200000&(data))>>21)
#define SP1_CTRL_get_s1_sel_clpmsk_as_devs(data)                      ((0x00100000&(data))>>20)
#define SP1_CTRL_get_s1_hsout_inv(data)                               ((0x00080000&(data))>>19)
#define SP1_CTRL_get_dummy18021000_18(data)                           ((0x00040000&(data))>>18)
#define SP1_CTRL_get_s1_hs_sync_source(data)                          ((0x00020000&(data))>>17)
#define SP1_CTRL_get_s1_hsout_en(data)                                ((0x00010000&(data))>>16)
#define SP1_CTRL_get_dummy18021000_15_13(data)                        ((0x0000E000&(data))>>13)
#define SP1_CTRL_get_s1_seperate_testmode(data)                       ((0x00001000&(data))>>12)
#define SP1_CTRL_get_s1_auto_run_done_ie(data)                        ((0x00000800&(data))>>11)
#define SP1_CTRL_get_s1_auto_run(data)                                ((0x00000400&(data))>>10)
#define SP1_CTRL_get_s1_coast_en(data)                                ((0x00000200&(data))>>9)
#define SP1_CTRL_get_s1_decmp_en(data)                                ((0x00000100&(data))>>8)
#define SP1_CTRL_get_dummy18021000_7(data)                            ((0x00000080&(data))>>7)
#define SP1_CTRL_get_s1_hs_fb_plswid_md(data)                         ((0x00000060&(data))>>5)
#define SP1_CTRL_get_s1_coast_inv(data)                               ((0x00000010&(data))>>4)
#define SP1_CTRL_get_dummy18021000_3(data)                            ((0x00000008&(data))>>3)
#define SP1_CTRL_get_s1_vhs_source_sel(data)                          (0x00000007&(data))

#define SP1_AUTORST                                                   0x18021004
#define SP1_AUTORST_reg_addr                                          "0xB8021004"
#define SP1_AUTORST_reg                                               0xB8021004
#define SP1_AUTORST_inst_addr                                         "0x0001"
#define SP1_AUTORST_inst                                              0x0001
#define SP1_AUTORST_s1_auto_result_rdy_shift                          (31)
#define SP1_AUTORST_s1_auto_result_shift                              (4)
#define SP1_AUTORST_s1_input_signal_be_inverted_shift                 (0)
#define SP1_AUTORST_s1_auto_result_rdy_mask                           (0x80000000)
#define SP1_AUTORST_s1_auto_result_mask                               (0x00000070)
#define SP1_AUTORST_s1_input_signal_be_inverted_mask                  (0x00000001)
#define SP1_AUTORST_s1_auto_result_rdy(data)                          (0x80000000&(((UINT32)data)<<31))
#define SP1_AUTORST_s1_auto_result(data)                              (0x00000070&(((UINT32)data)<<4))
#define SP1_AUTORST_s1_input_signal_be_inverted(data)                 (0x00000001&(data))
#define SP1_AUTORST_get_s1_auto_result_rdy(data)                      ((0x80000000&(data))>>31)
#define SP1_AUTORST_get_s1_auto_result(data)                          ((0x00000070&(data))>>4)
#define SP1_AUTORST_get_s1_input_signal_be_inverted(data)             (0x00000001&(data))

#define SP2_CTRL                                                      0x18021008
#define SP2_CTRL_reg_addr                                             "0xB8021008"
#define SP2_CTRL_reg                                                  0xB8021008
#define SP2_CTRL_inst_addr                                            "0x0002"
#define SP2_CTRL_inst                                                 0x0002
#define SP2_CTRL_s2_dehs_out_en_shift                                 (31)
#define SP2_CTRL_dummy18021008_30_29_shift                            (29)
#define SP2_CTRL_s2_hsraw_inv_shift                                   (28)
#define SP2_CTRL_dummy18021008_27_23_shift                            (23)
#define SP2_CTRL_s2_adc_vsout_testmd_shift                            (22)
#define SP2_CTRL_s2_devs_out_en_shift                                 (21)
#define SP2_CTRL_s2_sel_clpmsk_as_devs_shift                          (20)
#define SP2_CTRL_dummy18021008_19_13_shift                            (13)
#define SP2_CTRL_s2_seperate_testmode_shift                           (12)
#define SP2_CTRL_s2_auto_run_done_ie_shift                            (11)
#define SP2_CTRL_s2_auto_run_shift                                    (10)
#define SP2_CTRL_dummy18021008_9_shift                                (9)
#define SP2_CTRL_s2_decmp_en_shift                                    (8)
#define SP2_CTRL_dummy18021008_7_3_shift                              (3)
#define SP2_CTRL_s2_vhs_source_sel_shift                              (0)
#define SP2_CTRL_s2_dehs_out_en_mask                                  (0x80000000)
#define SP2_CTRL_dummy18021008_30_29_mask                             (0x60000000)
#define SP2_CTRL_s2_hsraw_inv_mask                                    (0x10000000)
#define SP2_CTRL_dummy18021008_27_23_mask                             (0x0F800000)
#define SP2_CTRL_s2_adc_vsout_testmd_mask                             (0x00400000)
#define SP2_CTRL_s2_devs_out_en_mask                                  (0x00200000)
#define SP2_CTRL_s2_sel_clpmsk_as_devs_mask                           (0x00100000)
#define SP2_CTRL_dummy18021008_19_13_mask                             (0x000FE000)
#define SP2_CTRL_s2_seperate_testmode_mask                            (0x00001000)
#define SP2_CTRL_s2_auto_run_done_ie_mask                             (0x00000800)
#define SP2_CTRL_s2_auto_run_mask                                     (0x00000400)
#define SP2_CTRL_dummy18021008_9_mask                                 (0x00000200)
#define SP2_CTRL_s2_decmp_en_mask                                     (0x00000100)
#define SP2_CTRL_dummy18021008_7_3_mask                               (0x000000F8)
#define SP2_CTRL_s2_vhs_source_sel_mask                               (0x00000007)
#define SP2_CTRL_s2_dehs_out_en(data)                                 (0x80000000&(((UINT32)data)<<31))
#define SP2_CTRL_dummy18021008_30_29(data)                            (0x60000000&(((UINT32)data)<<29))
#define SP2_CTRL_s2_hsraw_inv(data)                                   (0x10000000&(((UINT32)data)<<28))
#define SP2_CTRL_dummy18021008_27_23(data)                            (0x0F800000&(((UINT32)data)<<23))
#define SP2_CTRL_s2_adc_vsout_testmd(data)                            (0x00400000&(((UINT32)data)<<22))
#define SP2_CTRL_s2_devs_out_en(data)                                 (0x00200000&(((UINT32)data)<<21))
#define SP2_CTRL_s2_sel_clpmsk_as_devs(data)                          (0x00100000&(((UINT32)data)<<20))
#define SP2_CTRL_dummy18021008_19_13(data)                            (0x000FE000&(((UINT32)data)<<13))
#define SP2_CTRL_s2_seperate_testmode(data)                           (0x00001000&(((UINT32)data)<<12))
#define SP2_CTRL_s2_auto_run_done_ie(data)                            (0x00000800&(((UINT32)data)<<11))
#define SP2_CTRL_s2_auto_run(data)                                    (0x00000400&(((UINT32)data)<<10))
#define SP2_CTRL_dummy18021008_9(data)                                (0x00000200&(((UINT32)data)<<9))
#define SP2_CTRL_s2_decmp_en(data)                                    (0x00000100&(((UINT32)data)<<8))
#define SP2_CTRL_dummy18021008_7_3(data)                              (0x000000F8&(((UINT32)data)<<3))
#define SP2_CTRL_s2_vhs_source_sel(data)                              (0x00000007&(data))
#define SP2_CTRL_get_s2_dehs_out_en(data)                             ((0x80000000&(data))>>31)
#define SP2_CTRL_get_dummy18021008_30_29(data)                        ((0x60000000&(data))>>29)
#define SP2_CTRL_get_s2_hsraw_inv(data)                               ((0x10000000&(data))>>28)
#define SP2_CTRL_get_dummy18021008_27_23(data)                        ((0x0F800000&(data))>>23)
#define SP2_CTRL_get_s2_adc_vsout_testmd(data)                        ((0x00400000&(data))>>22)
#define SP2_CTRL_get_s2_devs_out_en(data)                             ((0x00200000&(data))>>21)
#define SP2_CTRL_get_s2_sel_clpmsk_as_devs(data)                      ((0x00100000&(data))>>20)
#define SP2_CTRL_get_dummy18021008_19_13(data)                        ((0x000FE000&(data))>>13)
#define SP2_CTRL_get_s2_seperate_testmode(data)                       ((0x00001000&(data))>>12)
#define SP2_CTRL_get_s2_auto_run_done_ie(data)                        ((0x00000800&(data))>>11)
#define SP2_CTRL_get_s2_auto_run(data)                                ((0x00000400&(data))>>10)
#define SP2_CTRL_get_dummy18021008_9(data)                            ((0x00000200&(data))>>9)
#define SP2_CTRL_get_s2_decmp_en(data)                                ((0x00000100&(data))>>8)
#define SP2_CTRL_get_dummy18021008_7_3(data)                          ((0x000000F8&(data))>>3)
#define SP2_CTRL_get_s2_vhs_source_sel(data)                          (0x00000007&(data))

#define SP2_AUTORST                                                   0x1802100c
#define SP2_AUTORST_reg_addr                                          "0xB802100C"
#define SP2_AUTORST_reg                                               0xB802100C
#define SP2_AUTORST_inst_addr                                         "0x0003"
#define SP2_AUTORST_inst                                              0x0003
#define SP2_AUTORST_s2_auto_result_rdy_shift                          (31)
#define SP2_AUTORST_s2_auto_result_shift                              (4)
#define SP2_AUTORST_s2_input_signal_be_inverted_shift                 (0)
#define SP2_AUTORST_s2_auto_result_rdy_mask                           (0x80000000)
#define SP2_AUTORST_s2_auto_result_mask                               (0x00000070)
#define SP2_AUTORST_s2_input_signal_be_inverted_mask                  (0x00000001)
#define SP2_AUTORST_s2_auto_result_rdy(data)                          (0x80000000&(((UINT32)data)<<31))
#define SP2_AUTORST_s2_auto_result(data)                              (0x00000070&(((UINT32)data)<<4))
#define SP2_AUTORST_s2_input_signal_be_inverted(data)                 (0x00000001&(data))
#define SP2_AUTORST_get_s2_auto_result_rdy(data)                      ((0x80000000&(data))>>31)
#define SP2_AUTORST_get_s2_auto_result(data)                          ((0x00000070&(data))>>4)
#define SP2_AUTORST_get_s2_input_signal_be_inverted(data)             (0x00000001&(data))

#define SP1_STBCTRL                                                   0x18021010
#define SP1_STBCTRL_reg_addr                                          "0xB8021010"
#define SP1_STBCTRL_reg                                               0xB8021010
#define SP1_STBCTRL_inst_addr                                         "0x0004"
#define SP1_STBCTRL_inst                                              0x0004
#define SP1_STBCTRL_s1_cap_win_clo_shift                              (16)
#define SP1_STBCTRL_s1_cap_win_range_shift                            (11)
#define SP1_STBCTRL_s1_per_range_shift                                (9)
#define SP1_STBCTRL_dummy18021010_8_shift                             (8)
#define SP1_STBCTRL_s1_popup_stable_per_shift                         (7)
#define SP1_STBCTRL_dummy18021010_6_4_shift                           (4)
#define SP1_STBCTRL_s1_cap_win_step_shift                             (3)
#define SP1_STBCTRL_s1_miss_lock_limit_shift                          (2)
#define SP1_STBCTRL_dummy18021010_1_shift                             (1)
#define SP1_STBCTRL_s1_stable_en_shift                                (0)
#define SP1_STBCTRL_s1_cap_win_clo_mask                               (0x00010000)
#define SP1_STBCTRL_s1_cap_win_range_mask                             (0x0000F800)
#define SP1_STBCTRL_s1_per_range_mask                                 (0x00000600)
#define SP1_STBCTRL_dummy18021010_8_mask                              (0x00000100)
#define SP1_STBCTRL_s1_popup_stable_per_mask                          (0x00000080)
#define SP1_STBCTRL_dummy18021010_6_4_mask                            (0x00000070)
#define SP1_STBCTRL_s1_cap_win_step_mask                              (0x00000008)
#define SP1_STBCTRL_s1_miss_lock_limit_mask                           (0x00000004)
#define SP1_STBCTRL_dummy18021010_1_mask                              (0x00000002)
#define SP1_STBCTRL_s1_stable_en_mask                                 (0x00000001)
#define SP1_STBCTRL_s1_cap_win_clo(data)                              (0x00010000&(((UINT32)data)<<16))
#define SP1_STBCTRL_s1_cap_win_range(data)                            (0x0000F800&(((UINT32)data)<<11))
#define SP1_STBCTRL_s1_per_range(data)                                (0x00000600&(((UINT32)data)<<9))
#define SP1_STBCTRL_dummy18021010_8(data)                             (0x00000100&(((UINT32)data)<<8))
#define SP1_STBCTRL_s1_popup_stable_per(data)                         (0x00000080&(((UINT32)data)<<7))
#define SP1_STBCTRL_dummy18021010_6_4(data)                           (0x00000070&(((UINT32)data)<<4))
#define SP1_STBCTRL_s1_cap_win_step(data)                             (0x00000008&(((UINT32)data)<<3))
#define SP1_STBCTRL_s1_miss_lock_limit(data)                          (0x00000004&(((UINT32)data)<<2))
#define SP1_STBCTRL_dummy18021010_1(data)                             (0x00000002&(((UINT32)data)<<1))
#define SP1_STBCTRL_s1_stable_en(data)                                (0x00000001&(data))
#define SP1_STBCTRL_get_s1_cap_win_clo(data)                          ((0x00010000&(data))>>16)
#define SP1_STBCTRL_get_s1_cap_win_range(data)                        ((0x0000F800&(data))>>11)
#define SP1_STBCTRL_get_s1_per_range(data)                            ((0x00000600&(data))>>9)
#define SP1_STBCTRL_get_dummy18021010_8(data)                         ((0x00000100&(data))>>8)
#define SP1_STBCTRL_get_s1_popup_stable_per(data)                     ((0x00000080&(data))>>7)
#define SP1_STBCTRL_get_dummy18021010_6_4(data)                       ((0x00000070&(data))>>4)
#define SP1_STBCTRL_get_s1_cap_win_step(data)                         ((0x00000008&(data))>>3)
#define SP1_STBCTRL_get_s1_miss_lock_limit(data)                      ((0x00000004&(data))>>2)
#define SP1_STBCTRL_get_dummy18021010_1(data)                         ((0x00000002&(data))>>1)
#define SP1_STBCTRL_get_s1_stable_en(data)                            (0x00000001&(data))

#define SP1_STBRST0                                                   0x18021014
#define SP1_STBRST0_reg_addr                                          "0xB8021014"
#define SP1_STBRST0_reg                                               0xB8021014
#define SP1_STBRST0_inst_addr                                         "0x0005"
#define SP1_STBRST0_inst                                              0x0005
#define SP1_STBRST0_s1_mode_det_of_shift                              (31)
#define SP1_STBRST0_s1_stb_per_chg_shift                              (30)
#define SP1_STBRST0_s1_stb_pol_chg_shift                              (29)
#define SP1_STBRST0_s1_vs_raw_vld_shift                               (28)
#define SP1_STBRST0_s1_cap_32unlock_shift                             (27)
#define SP1_STBRST0_s1_eq_occur_shift                                 (26)
#define SP1_STBRST0_s1_hs_pol_chg_shift                               (25)
#define SP1_STBRST0_s1_cap_unlock_shift                               (24)
#define SP1_STBRST0_s1_mode_det_stable_shift                          (23)
#define SP1_STBRST0_s1_stable_pol_out_shift                           (22)
#define SP1_STBRST0_s1_mode_det_of_mask                               (0x80000000)
#define SP1_STBRST0_s1_stb_per_chg_mask                               (0x40000000)
#define SP1_STBRST0_s1_stb_pol_chg_mask                               (0x20000000)
#define SP1_STBRST0_s1_vs_raw_vld_mask                                (0x10000000)
#define SP1_STBRST0_s1_cap_32unlock_mask                              (0x08000000)
#define SP1_STBRST0_s1_eq_occur_mask                                  (0x04000000)
#define SP1_STBRST0_s1_hs_pol_chg_mask                                (0x02000000)
#define SP1_STBRST0_s1_cap_unlock_mask                                (0x01000000)
#define SP1_STBRST0_s1_mode_det_stable_mask                           (0x00800000)
#define SP1_STBRST0_s1_stable_pol_out_mask                            (0x00400000)
#define SP1_STBRST0_s1_mode_det_of(data)                              (0x80000000&(((UINT32)data)<<31))
#define SP1_STBRST0_s1_stb_per_chg(data)                              (0x40000000&(((UINT32)data)<<30))
#define SP1_STBRST0_s1_stb_pol_chg(data)                              (0x20000000&(((UINT32)data)<<29))
#define SP1_STBRST0_s1_vs_raw_vld(data)                               (0x10000000&(((UINT32)data)<<28))
#define SP1_STBRST0_s1_cap_32unlock(data)                             (0x08000000&(((UINT32)data)<<27))
#define SP1_STBRST0_s1_eq_occur(data)                                 (0x04000000&(((UINT32)data)<<26))
#define SP1_STBRST0_s1_hs_pol_chg(data)                               (0x02000000&(((UINT32)data)<<25))
#define SP1_STBRST0_s1_cap_unlock(data)                               (0x01000000&(((UINT32)data)<<24))
#define SP1_STBRST0_s1_mode_det_stable(data)                          (0x00800000&(((UINT32)data)<<23))
#define SP1_STBRST0_s1_stable_pol_out(data)                           (0x00400000&(((UINT32)data)<<22))
#define SP1_STBRST0_get_s1_mode_det_of(data)                          ((0x80000000&(data))>>31)
#define SP1_STBRST0_get_s1_stb_per_chg(data)                          ((0x40000000&(data))>>30)
#define SP1_STBRST0_get_s1_stb_pol_chg(data)                          ((0x20000000&(data))>>29)
#define SP1_STBRST0_get_s1_vs_raw_vld(data)                           ((0x10000000&(data))>>28)
#define SP1_STBRST0_get_s1_cap_32unlock(data)                         ((0x08000000&(data))>>27)
#define SP1_STBRST0_get_s1_eq_occur(data)                             ((0x04000000&(data))>>26)
#define SP1_STBRST0_get_s1_hs_pol_chg(data)                           ((0x02000000&(data))>>25)
#define SP1_STBRST0_get_s1_cap_unlock(data)                           ((0x01000000&(data))>>24)
#define SP1_STBRST0_get_s1_mode_det_stable(data)                      ((0x00800000&(data))>>23)
#define SP1_STBRST0_get_s1_stable_pol_out(data)                       ((0x00400000&(data))>>22)

#define SP1_STBRST1                                                   0x18021018
#define SP1_STBRST1_reg_addr                                          "0xB8021018"
#define SP1_STBRST1_reg                                               0xB8021018
#define SP1_STBRST1_inst_addr                                         "0x0006"
#define SP1_STBRST1_inst                                              0x0006
#define SP1_STBRST1_s1_stable_period_out_shift                        (16)
#define SP1_STBRST1_s1_stable_plswid_out_shift                        (0)
#define SP1_STBRST1_s1_stable_period_out_mask                         (0x07FF0000)
#define SP1_STBRST1_s1_stable_plswid_out_mask                         (0x000007FF)
#define SP1_STBRST1_s1_stable_period_out(data)                        (0x07FF0000&(((UINT32)data)<<16))
#define SP1_STBRST1_s1_stable_plswid_out(data)                        (0x000007FF&(data))
#define SP1_STBRST1_get_s1_stable_period_out(data)                    ((0x07FF0000&(data))>>16)
#define SP1_STBRST1_get_s1_stable_plswid_out(data)                    (0x000007FF&(data))

#define SP2_STBCTRL                                                   0x1802101c
#define SP2_STBCTRL_reg_addr                                          "0xB802101C"
#define SP2_STBCTRL_reg                                               0xB802101C
#define SP2_STBCTRL_inst_addr                                         "0x0007"
#define SP2_STBCTRL_inst                                              0x0007
#define SP2_STBCTRL_s2_cap_win_clo_shift                              (16)
#define SP2_STBCTRL_s2_cap_win_range_shift                            (11)
#define SP2_STBCTRL_s2_per_range_shift                                (9)
#define SP2_STBCTRL_dummy1802101c_8_shift                             (8)
#define SP2_STBCTRL_s2_popup_stable_per_shift                         (7)
#define SP2_STBCTRL_dummy1802101c_6_4_shift                           (4)
#define SP2_STBCTRL_s2_cap_win_step_shift                             (3)
#define SP2_STBCTRL_s2_miss_lock_limit_shift                          (2)
#define SP2_STBCTRL_dummy1802101c_1_shift                             (1)
#define SP2_STBCTRL_s2_stable_en_shift                                (0)
#define SP2_STBCTRL_s2_cap_win_clo_mask                               (0x00010000)
#define SP2_STBCTRL_s2_cap_win_range_mask                             (0x0000F800)
#define SP2_STBCTRL_s2_per_range_mask                                 (0x00000600)
#define SP2_STBCTRL_dummy1802101c_8_mask                              (0x00000100)
#define SP2_STBCTRL_s2_popup_stable_per_mask                          (0x00000080)
#define SP2_STBCTRL_dummy1802101c_6_4_mask                            (0x00000070)
#define SP2_STBCTRL_s2_cap_win_step_mask                              (0x00000008)
#define SP2_STBCTRL_s2_miss_lock_limit_mask                           (0x00000004)
#define SP2_STBCTRL_dummy1802101c_1_mask                              (0x00000002)
#define SP2_STBCTRL_s2_stable_en_mask                                 (0x00000001)
#define SP2_STBCTRL_s2_cap_win_clo(data)                              (0x00010000&(((UINT32)data)<<16))
#define SP2_STBCTRL_s2_cap_win_range(data)                            (0x0000F800&(((UINT32)data)<<11))
#define SP2_STBCTRL_s2_per_range(data)                                (0x00000600&(((UINT32)data)<<9))
#define SP2_STBCTRL_dummy1802101c_8(data)                             (0x00000100&(((UINT32)data)<<8))
#define SP2_STBCTRL_s2_popup_stable_per(data)                         (0x00000080&(((UINT32)data)<<7))
#define SP2_STBCTRL_dummy1802101c_6_4(data)                           (0x00000070&(((UINT32)data)<<4))
#define SP2_STBCTRL_s2_cap_win_step(data)                             (0x00000008&(((UINT32)data)<<3))
#define SP2_STBCTRL_s2_miss_lock_limit(data)                          (0x00000004&(((UINT32)data)<<2))
#define SP2_STBCTRL_dummy1802101c_1(data)                             (0x00000002&(((UINT32)data)<<1))
#define SP2_STBCTRL_s2_stable_en(data)                                (0x00000001&(data))
#define SP2_STBCTRL_get_s2_cap_win_clo(data)                          ((0x00010000&(data))>>16)
#define SP2_STBCTRL_get_s2_cap_win_range(data)                        ((0x0000F800&(data))>>11)
#define SP2_STBCTRL_get_s2_per_range(data)                            ((0x00000600&(data))>>9)
#define SP2_STBCTRL_get_dummy1802101c_8(data)                         ((0x00000100&(data))>>8)
#define SP2_STBCTRL_get_s2_popup_stable_per(data)                     ((0x00000080&(data))>>7)
#define SP2_STBCTRL_get_dummy1802101c_6_4(data)                       ((0x00000070&(data))>>4)
#define SP2_STBCTRL_get_s2_cap_win_step(data)                         ((0x00000008&(data))>>3)
#define SP2_STBCTRL_get_s2_miss_lock_limit(data)                      ((0x00000004&(data))>>2)
#define SP2_STBCTRL_get_dummy1802101c_1(data)                         ((0x00000002&(data))>>1)
#define SP2_STBCTRL_get_s2_stable_en(data)                            (0x00000001&(data))

#define SP2_STBRST0                                                   0x18021020
#define SP2_STBRST0_reg_addr                                          "0xB8021020"
#define SP2_STBRST0_reg                                               0xB8021020
#define SP2_STBRST0_inst_addr                                         "0x0008"
#define SP2_STBRST0_inst                                              0x0008
#define SP2_STBRST0_s2_mode_det_of_shift                              (31)
#define SP2_STBRST0_s2_stb_per_chg_shift                              (30)
#define SP2_STBRST0_s2_stb_pol_chg_shift                              (29)
#define SP2_STBRST0_s2_vs_raw_vld_shift                               (28)
#define SP2_STBRST0_s2_cap_32unlock_shift                             (27)
#define SP2_STBRST0_s2_eq_occur_shift                                 (26)
#define SP2_STBRST0_s2_hs_pol_chg_shift                               (25)
#define SP2_STBRST0_s2_cap_unlock_shift                               (24)
#define SP2_STBRST0_s2_mode_det_stable_shift                          (23)
#define SP2_STBRST0_s2_stable_pol_out_shift                           (22)
#define SP2_STBRST0_s2_mode_det_of_mask                               (0x80000000)
#define SP2_STBRST0_s2_stb_per_chg_mask                               (0x40000000)
#define SP2_STBRST0_s2_stb_pol_chg_mask                               (0x20000000)
#define SP2_STBRST0_s2_vs_raw_vld_mask                                (0x10000000)
#define SP2_STBRST0_s2_cap_32unlock_mask                              (0x08000000)
#define SP2_STBRST0_s2_eq_occur_mask                                  (0x04000000)
#define SP2_STBRST0_s2_hs_pol_chg_mask                                (0x02000000)
#define SP2_STBRST0_s2_cap_unlock_mask                                (0x01000000)
#define SP2_STBRST0_s2_mode_det_stable_mask                           (0x00800000)
#define SP2_STBRST0_s2_stable_pol_out_mask                            (0x00400000)
#define SP2_STBRST0_s2_mode_det_of(data)                              (0x80000000&(((UINT32)data)<<31))
#define SP2_STBRST0_s2_stb_per_chg(data)                              (0x40000000&(((UINT32)data)<<30))
#define SP2_STBRST0_s2_stb_pol_chg(data)                              (0x20000000&(((UINT32)data)<<29))
#define SP2_STBRST0_s2_vs_raw_vld(data)                               (0x10000000&(((UINT32)data)<<28))
#define SP2_STBRST0_s2_cap_32unlock(data)                             (0x08000000&(((UINT32)data)<<27))
#define SP2_STBRST0_s2_eq_occur(data)                                 (0x04000000&(((UINT32)data)<<26))
#define SP2_STBRST0_s2_hs_pol_chg(data)                               (0x02000000&(((UINT32)data)<<25))
#define SP2_STBRST0_s2_cap_unlock(data)                               (0x01000000&(((UINT32)data)<<24))
#define SP2_STBRST0_s2_mode_det_stable(data)                          (0x00800000&(((UINT32)data)<<23))
#define SP2_STBRST0_s2_stable_pol_out(data)                           (0x00400000&(((UINT32)data)<<22))
#define SP2_STBRST0_get_s2_mode_det_of(data)                          ((0x80000000&(data))>>31)
#define SP2_STBRST0_get_s2_stb_per_chg(data)                          ((0x40000000&(data))>>30)
#define SP2_STBRST0_get_s2_stb_pol_chg(data)                          ((0x20000000&(data))>>29)
#define SP2_STBRST0_get_s2_vs_raw_vld(data)                           ((0x10000000&(data))>>28)
#define SP2_STBRST0_get_s2_cap_32unlock(data)                         ((0x08000000&(data))>>27)
#define SP2_STBRST0_get_s2_eq_occur(data)                             ((0x04000000&(data))>>26)
#define SP2_STBRST0_get_s2_hs_pol_chg(data)                           ((0x02000000&(data))>>25)
#define SP2_STBRST0_get_s2_cap_unlock(data)                           ((0x01000000&(data))>>24)
#define SP2_STBRST0_get_s2_mode_det_stable(data)                      ((0x00800000&(data))>>23)
#define SP2_STBRST0_get_s2_stable_pol_out(data)                       ((0x00400000&(data))>>22)

#define SP2_STBRST1                                                   0x18021024
#define SP2_STBRST1_reg_addr                                          "0xB8021024"
#define SP2_STBRST1_reg                                               0xB8021024
#define SP2_STBRST1_inst_addr                                         "0x0009"
#define SP2_STBRST1_inst                                              0x0009
#define SP2_STBRST1_s2_stable_period_out_shift                        (16)
#define SP2_STBRST1_s2_stable_plswid_out_shift                        (0)
#define SP2_STBRST1_s2_stable_period_out_mask                         (0x07FF0000)
#define SP2_STBRST1_s2_stable_plswid_out_mask                         (0x000007FF)
#define SP2_STBRST1_s2_stable_period_out(data)                        (0x07FF0000&(((UINT32)data)<<16))
#define SP2_STBRST1_s2_stable_plswid_out(data)                        (0x000007FF&(data))
#define SP2_STBRST1_get_s2_stable_period_out(data)                    ((0x07FF0000&(data))>>16)
#define SP2_STBRST1_get_s2_stable_plswid_out(data)                    (0x000007FF&(data))

#define SP1_DCNT                                                      0x18021028
#define SP1_DCNT_reg_addr                                             "0xB8021028"
#define SP1_DCNT_reg                                                  0xB8021028
#define SP1_DCNT_inst_addr                                            "0x000A"
#define SP1_DCNT_inst                                                 0x000A
#define SP1_DCNT_s1_deb_num_shift                                     (11)
#define SP1_DCNT_s1_dcnt_level_shift                                  (0)
#define SP1_DCNT_s1_deb_num_mask                                      (0x0000F800)
#define SP1_DCNT_s1_dcnt_level_mask                                   (0x000007FF)
#define SP1_DCNT_s1_deb_num(data)                                     (0x0000F800&(((UINT32)data)<<11))
#define SP1_DCNT_s1_dcnt_level(data)                                  (0x000007FF&(data))
#define SP1_DCNT_get_s1_deb_num(data)                                 ((0x0000F800&(data))>>11)
#define SP1_DCNT_get_s1_dcnt_level(data)                              (0x000007FF&(data))

#define SP2_DCNT                                                      0x1802102c
#define SP2_DCNT_reg_addr                                             "0xB802102C"
#define SP2_DCNT_reg                                                  0xB802102C
#define SP2_DCNT_inst_addr                                            "0x000B"
#define SP2_DCNT_inst                                                 0x000B
#define SP2_DCNT_s2_deb_num_shift                                     (11)
#define SP2_DCNT_s2_dcnt_level_shift                                  (0)
#define SP2_DCNT_s2_deb_num_mask                                      (0x0000F800)
#define SP2_DCNT_s2_dcnt_level_mask                                   (0x000007FF)
#define SP2_DCNT_s2_deb_num(data)                                     (0x0000F800&(((UINT32)data)<<11))
#define SP2_DCNT_s2_dcnt_level(data)                                  (0x000007FF&(data))
#define SP2_DCNT_get_s2_deb_num(data)                                 ((0x0000F800&(data))>>11)
#define SP2_DCNT_get_s2_dcnt_level(data)                              (0x000007FF&(data))

#define SP1_FIELD                                                     0x18021030
#define SP1_FIELD_reg_addr                                            "0xB8021030"
#define SP1_FIELD_reg                                                 0xB8021030
#define SP1_FIELD_inst_addr                                           "0x000C"
#define SP1_FIELD_inst                                                0x000C
#define SP1_FIELD_s1_field_det_mode_shift                             (7)
#define SP1_FIELD_s1_f_pos_field_shift                                (6)
#define SP1_FIELD_s1_f_pos_field_tgle_shift                           (5)
#define SP1_FIELD_s1_f_count_field_shift                              (4)
#define SP1_FIELD_s1_f_count_field_tgle_shift                         (3)
#define SP1_FIELD_s1_l_cnt_lsb1_shift                                 (0)
#define SP1_FIELD_s1_field_det_mode_mask                              (0x00000080)
#define SP1_FIELD_s1_f_pos_field_mask                                 (0x00000040)
#define SP1_FIELD_s1_f_pos_field_tgle_mask                            (0x00000020)
#define SP1_FIELD_s1_f_count_field_mask                               (0x00000010)
#define SP1_FIELD_s1_f_count_field_tgle_mask                          (0x00000008)
#define SP1_FIELD_s1_l_cnt_lsb1_mask                                  (0x00000007)
#define SP1_FIELD_s1_field_det_mode(data)                             (0x00000080&(((UINT32)data)<<7))
#define SP1_FIELD_s1_f_pos_field(data)                                (0x00000040&(((UINT32)data)<<6))
#define SP1_FIELD_s1_f_pos_field_tgle(data)                           (0x00000020&(((UINT32)data)<<5))
#define SP1_FIELD_s1_f_count_field(data)                              (0x00000010&(((UINT32)data)<<4))
#define SP1_FIELD_s1_f_count_field_tgle(data)                         (0x00000008&(((UINT32)data)<<3))
#define SP1_FIELD_s1_l_cnt_lsb1(data)                                 (0x00000007&(data))
#define SP1_FIELD_get_s1_field_det_mode(data)                         ((0x00000080&(data))>>7)
#define SP1_FIELD_get_s1_f_pos_field(data)                            ((0x00000040&(data))>>6)
#define SP1_FIELD_get_s1_f_pos_field_tgle(data)                       ((0x00000020&(data))>>5)
#define SP1_FIELD_get_s1_f_count_field(data)                          ((0x00000010&(data))>>4)
#define SP1_FIELD_get_s1_f_count_field_tgle(data)                     ((0x00000008&(data))>>3)
#define SP1_FIELD_get_s1_l_cnt_lsb1(data)                             (0x00000007&(data))

#define SP2_FIELD                                                     0x18021034
#define SP2_FIELD_reg_addr                                            "0xB8021034"
#define SP2_FIELD_reg                                                 0xB8021034
#define SP2_FIELD_inst_addr                                           "0x000D"
#define SP2_FIELD_inst                                                0x000D
#define SP2_FIELD_dummy18021034_31_8_shift                            (8)
#define SP2_FIELD_s2_field_det_mode_shift                             (7)
#define SP2_FIELD_s2_f_pos_field_shift                                (6)
#define SP2_FIELD_s2_f_pos_field_tgle_shift                           (5)
#define SP2_FIELD_s2_f_count_field_shift                              (4)
#define SP2_FIELD_s2_f_count_field_tgle_shift                         (3)
#define SP2_FIELD_s2_l_cnt_lsb1_shift                                 (0)
#define SP2_FIELD_dummy18021034_31_8_mask                             (0xFFFFFF00)
#define SP2_FIELD_s2_field_det_mode_mask                              (0x00000080)
#define SP2_FIELD_s2_f_pos_field_mask                                 (0x00000040)
#define SP2_FIELD_s2_f_pos_field_tgle_mask                            (0x00000020)
#define SP2_FIELD_s2_f_count_field_mask                               (0x00000010)
#define SP2_FIELD_s2_f_count_field_tgle_mask                          (0x00000008)
#define SP2_FIELD_s2_l_cnt_lsb1_mask                                  (0x00000007)
#define SP2_FIELD_dummy18021034_31_8(data)                            (0xFFFFFF00&(((UINT32)data)<<8))
#define SP2_FIELD_s2_field_det_mode(data)                             (0x00000080&(((UINT32)data)<<7))
#define SP2_FIELD_s2_f_pos_field(data)                                (0x00000040&(((UINT32)data)<<6))
#define SP2_FIELD_s2_f_pos_field_tgle(data)                           (0x00000020&(((UINT32)data)<<5))
#define SP2_FIELD_s2_f_count_field(data)                              (0x00000010&(((UINT32)data)<<4))
#define SP2_FIELD_s2_f_count_field_tgle(data)                         (0x00000008&(((UINT32)data)<<3))
#define SP2_FIELD_s2_l_cnt_lsb1(data)                                 (0x00000007&(data))
#define SP2_FIELD_get_dummy18021034_31_8(data)                        ((0xFFFFFF00&(data))>>8)
#define SP2_FIELD_get_s2_field_det_mode(data)                         ((0x00000080&(data))>>7)
#define SP2_FIELD_get_s2_f_pos_field(data)                            ((0x00000040&(data))>>6)
#define SP2_FIELD_get_s2_f_pos_field_tgle(data)                       ((0x00000020&(data))>>5)
#define SP2_FIELD_get_s2_f_count_field(data)                          ((0x00000010&(data))>>4)
#define SP2_FIELD_get_s2_f_count_field_tgle(data)                     ((0x00000008&(data))>>3)
#define SP2_FIELD_get_s2_l_cnt_lsb1(data)                             (0x00000007&(data))

#define SP1_CLMPCTRL                                                  0x18021038
#define SP1_CLMPCTRL_reg_addr                                         "0xB8021038"
#define SP1_CLMPCTRL_reg                                              0xB8021038
#define SP1_CLMPCTRL_inst_addr                                        "0x000E"
#define SP1_CLMPCTRL_inst                                             0x000E
#define SP1_CLMPCTRL_dummy18021038_13_8_shift                         (8)
#define SP1_CLMPCTRL_s1_clamp_dben_shift                              (7)
#define SP1_CLMPCTRL_s1_clamp_cnt_unit_shift                          (6)
#define SP1_CLMPCTRL_s1_clamp_clksrc_shift                            (5)
#define SP1_CLMPCTRL_s1_clamp_en_shift                                (4)
#define SP1_CLMPCTRL_s1_yclamp_hs_inv_shift                           (3)
#define SP1_CLMPCTRL_s1_nyclamp_hs_inv_shift                          (2)
#define SP1_CLMPCTRL_s1_clamp_test_en_shift                           (1)
#define SP1_CLMPCTRL_s1_adc_hs_syncedge_shift                         (0)
#define SP1_CLMPCTRL_dummy18021038_13_8_mask                          (0x00003F00)
#define SP1_CLMPCTRL_s1_clamp_dben_mask                               (0x00000080)
#define SP1_CLMPCTRL_s1_clamp_cnt_unit_mask                           (0x00000040)
#define SP1_CLMPCTRL_s1_clamp_clksrc_mask                             (0x00000020)
#define SP1_CLMPCTRL_s1_clamp_en_mask                                 (0x00000010)
#define SP1_CLMPCTRL_s1_yclamp_hs_inv_mask                            (0x00000008)
#define SP1_CLMPCTRL_s1_nyclamp_hs_inv_mask                           (0x00000004)
#define SP1_CLMPCTRL_s1_clamp_test_en_mask                            (0x00000002)
#define SP1_CLMPCTRL_s1_adc_hs_syncedge_mask                          (0x00000001)
#define SP1_CLMPCTRL_dummy18021038_13_8(data)                         (0x00003F00&(((UINT32)data)<<8))
#define SP1_CLMPCTRL_s1_clamp_dben(data)                              (0x00000080&(((UINT32)data)<<7))
#define SP1_CLMPCTRL_s1_clamp_cnt_unit(data)                          (0x00000040&(((UINT32)data)<<6))
#define SP1_CLMPCTRL_s1_clamp_clksrc(data)                            (0x00000020&(((UINT32)data)<<5))
#define SP1_CLMPCTRL_s1_clamp_en(data)                                (0x00000010&(((UINT32)data)<<4))
#define SP1_CLMPCTRL_s1_yclamp_hs_inv(data)                           (0x00000008&(((UINT32)data)<<3))
#define SP1_CLMPCTRL_s1_nyclamp_hs_inv(data)                          (0x00000004&(((UINT32)data)<<2))
#define SP1_CLMPCTRL_s1_clamp_test_en(data)                           (0x00000002&(((UINT32)data)<<1))
#define SP1_CLMPCTRL_s1_adc_hs_syncedge(data)                         (0x00000001&(data))
#define SP1_CLMPCTRL_get_dummy18021038_13_8(data)                     ((0x00003F00&(data))>>8)
#define SP1_CLMPCTRL_get_s1_clamp_dben(data)                          ((0x00000080&(data))>>7)
#define SP1_CLMPCTRL_get_s1_clamp_cnt_unit(data)                      ((0x00000040&(data))>>6)
#define SP1_CLMPCTRL_get_s1_clamp_clksrc(data)                        ((0x00000020&(data))>>5)
#define SP1_CLMPCTRL_get_s1_clamp_en(data)                            ((0x00000010&(data))>>4)
#define SP1_CLMPCTRL_get_s1_yclamp_hs_inv(data)                       ((0x00000008&(data))>>3)
#define SP1_CLMPCTRL_get_s1_nyclamp_hs_inv(data)                      ((0x00000004&(data))>>2)
#define SP1_CLMPCTRL_get_s1_clamp_test_en(data)                       ((0x00000002&(data))>>1)
#define SP1_CLMPCTRL_get_s1_adc_hs_syncedge(data)                     (0x00000001&(data))

#define SP1_CLMP                                                      0x1802103c
#define SP1_CLMP_reg_addr                                             "0xB802103C"
#define SP1_CLMP_reg                                                  0xB802103C
#define SP1_CLMP_inst_addr                                            "0x000F"
#define SP1_CLMP_inst                                                 0x000F
#define SP1_CLMP_s1_yclamp_sta_shift                                  (24)
#define SP1_CLMP_s1_yclamp_end_shift                                  (16)
#define SP1_CLMP_s1_nyclamp_sta_shift                                 (8)
#define SP1_CLMP_s1_nyclamp_end_shift                                 (0)
#define SP1_CLMP_s1_yclamp_sta_mask                                   (0xFF000000)
#define SP1_CLMP_s1_yclamp_end_mask                                   (0x00FF0000)
#define SP1_CLMP_s1_nyclamp_sta_mask                                  (0x0000FF00)
#define SP1_CLMP_s1_nyclamp_end_mask                                  (0x000000FF)
#define SP1_CLMP_s1_yclamp_sta(data)                                  (0xFF000000&(((UINT32)data)<<24))
#define SP1_CLMP_s1_yclamp_end(data)                                  (0x00FF0000&(((UINT32)data)<<16))
#define SP1_CLMP_s1_nyclamp_sta(data)                                 (0x0000FF00&(((UINT32)data)<<8))
#define SP1_CLMP_s1_nyclamp_end(data)                                 (0x000000FF&(data))
#define SP1_CLMP_get_s1_yclamp_sta(data)                              ((0xFF000000&(data))>>24)
#define SP1_CLMP_get_s1_yclamp_end(data)                              ((0x00FF0000&(data))>>16)
#define SP1_CLMP_get_s1_nyclamp_sta(data)                             ((0x0000FF00&(data))>>8)
#define SP1_CLMP_get_s1_nyclamp_end(data)                             (0x000000FF&(data))

#define SP1_CLMPMSK                                                   0x18021040
#define SP1_CLMPMSK_reg_addr                                          "0xB8021040"
#define SP1_CLMPMSK_reg                                               0xB8021040
#define SP1_CLMPMSK_inst_addr                                         "0x0010"
#define SP1_CLMPMSK_inst                                              0x0010
#define SP1_CLMPMSK_s1_mask_clamp_en_shift                            (15)
#define SP1_CLMPMSK_dummy18021040_14_shift                            (14)
#define SP1_CLMPMSK_s1_mask_front_shift                               (8)
#define SP1_CLMPMSK_dummy18021040_7_6_shift                           (6)
#define SP1_CLMPMSK_s1_mask_back_shift                                (0)
#define SP1_CLMPMSK_s1_mask_clamp_en_mask                             (0x00008000)
#define SP1_CLMPMSK_dummy18021040_14_mask                             (0x00004000)
#define SP1_CLMPMSK_s1_mask_front_mask                                (0x00003F00)
#define SP1_CLMPMSK_dummy18021040_7_6_mask                            (0x000000C0)
#define SP1_CLMPMSK_s1_mask_back_mask                                 (0x0000003F)
#define SP1_CLMPMSK_s1_mask_clamp_en(data)                            (0x00008000&(((UINT32)data)<<15))
#define SP1_CLMPMSK_dummy18021040_14(data)                            (0x00004000&(((UINT32)data)<<14))
#define SP1_CLMPMSK_s1_mask_front(data)                               (0x00003F00&(((UINT32)data)<<8))
#define SP1_CLMPMSK_dummy18021040_7_6(data)                           (0x000000C0&(((UINT32)data)<<6))
#define SP1_CLMPMSK_s1_mask_back(data)                                (0x0000003F&(data))
#define SP1_CLMPMSK_get_s1_mask_clamp_en(data)                        ((0x00008000&(data))>>15)
#define SP1_CLMPMSK_get_dummy18021040_14(data)                        ((0x00004000&(data))>>14)
#define SP1_CLMPMSK_get_s1_mask_front(data)                           ((0x00003F00&(data))>>8)
#define SP1_CLMPMSK_get_dummy18021040_7_6(data)                       ((0x000000C0&(data))>>6)
#define SP1_CLMPMSK_get_s1_mask_back(data)                            (0x0000003F&(data))

#define SP1_COASTFTBK                                                 0x18021044
#define SP1_COASTFTBK_reg_addr                                        "0xB8021044"
#define SP1_COASTFTBK_reg                                             0xB8021044
#define SP1_COASTFTBK_inst_addr                                       "0x0011"
#define SP1_COASTFTBK_inst                                            0x0011
#define SP1_COASTFTBK_dummy18021044_9_8_shift                         (8)
#define SP1_COASTFTBK_s1_coast_front_shift                            (4)
#define SP1_COASTFTBK_s1_coast_back_shift                             (0)
#define SP1_COASTFTBK_dummy18021044_9_8_mask                          (0x00000300)
#define SP1_COASTFTBK_s1_coast_front_mask                             (0x000000F0)
#define SP1_COASTFTBK_s1_coast_back_mask                              (0x0000000F)
#define SP1_COASTFTBK_dummy18021044_9_8(data)                         (0x00000300&(((UINT32)data)<<8))
#define SP1_COASTFTBK_s1_coast_front(data)                            (0x000000F0&(((UINT32)data)<<4))
#define SP1_COASTFTBK_s1_coast_back(data)                             (0x0000000F&(data))
#define SP1_COASTFTBK_get_dummy18021044_9_8(data)                     ((0x00000300&(data))>>8)
#define SP1_COASTFTBK_get_s1_coast_front(data)                        ((0x000000F0&(data))>>4)
#define SP1_COASTFTBK_get_s1_coast_back(data)                         (0x0000000F&(data))

#define SP2_CLMPCTRL                                                  0x18021048
#define SP2_CLMPCTRL_reg_addr                                         "0xB8021048"
#define SP2_CLMPCTRL_reg                                              0xB8021048
#define SP2_CLMPCTRL_inst_addr                                        "0x0012"
#define SP2_CLMPCTRL_inst                                             0x0012
#define SP2_CLMPCTRL_dummy18021048_13_8_shift                         (8)
#define SP2_CLMPCTRL_s2_clamp_dben_shift                              (7)
#define SP2_CLMPCTRL_s2_clamp_cnt_unit_shift                          (6)
#define SP2_CLMPCTRL_s2_clamp_clksrc_shift                            (5)
#define SP2_CLMPCTRL_s2_clamp_en_shift                                (4)
#define SP2_CLMPCTRL_s2_yclamp_hs_inv_shift                           (3)
#define SP2_CLMPCTRL_dummy18021048_2_shift                            (2)
#define SP2_CLMPCTRL_s2_clamp_test_en_shift                           (1)
#define SP2_CLMPCTRL_s2_adc_hs_syncedge_shift                         (0)
#define SP2_CLMPCTRL_dummy18021048_13_8_mask                          (0x00003F00)
#define SP2_CLMPCTRL_s2_clamp_dben_mask                               (0x00000080)
#define SP2_CLMPCTRL_s2_clamp_cnt_unit_mask                           (0x00000040)
#define SP2_CLMPCTRL_s2_clamp_clksrc_mask                             (0x00000020)
#define SP2_CLMPCTRL_s2_clamp_en_mask                                 (0x00000010)
#define SP2_CLMPCTRL_s2_yclamp_hs_inv_mask                            (0x00000008)
#define SP2_CLMPCTRL_dummy18021048_2_mask                             (0x00000004)
#define SP2_CLMPCTRL_s2_clamp_test_en_mask                            (0x00000002)
#define SP2_CLMPCTRL_s2_adc_hs_syncedge_mask                          (0x00000001)
#define SP2_CLMPCTRL_dummy18021048_13_8(data)                         (0x00003F00&(((UINT32)data)<<8))
#define SP2_CLMPCTRL_s2_clamp_dben(data)                              (0x00000080&(((UINT32)data)<<7))
#define SP2_CLMPCTRL_s2_clamp_cnt_unit(data)                          (0x00000040&(((UINT32)data)<<6))
#define SP2_CLMPCTRL_s2_clamp_clksrc(data)                            (0x00000020&(((UINT32)data)<<5))
#define SP2_CLMPCTRL_s2_clamp_en(data)                                (0x00000010&(((UINT32)data)<<4))
#define SP2_CLMPCTRL_s2_yclamp_hs_inv(data)                           (0x00000008&(((UINT32)data)<<3))
#define SP2_CLMPCTRL_dummy18021048_2(data)                            (0x00000004&(((UINT32)data)<<2))
#define SP2_CLMPCTRL_s2_clamp_test_en(data)                           (0x00000002&(((UINT32)data)<<1))
#define SP2_CLMPCTRL_s2_adc_hs_syncedge(data)                         (0x00000001&(data))
#define SP2_CLMPCTRL_get_dummy18021048_13_8(data)                     ((0x00003F00&(data))>>8)
#define SP2_CLMPCTRL_get_s2_clamp_dben(data)                          ((0x00000080&(data))>>7)
#define SP2_CLMPCTRL_get_s2_clamp_cnt_unit(data)                      ((0x00000040&(data))>>6)
#define SP2_CLMPCTRL_get_s2_clamp_clksrc(data)                        ((0x00000020&(data))>>5)
#define SP2_CLMPCTRL_get_s2_clamp_en(data)                            ((0x00000010&(data))>>4)
#define SP2_CLMPCTRL_get_s2_yclamp_hs_inv(data)                       ((0x00000008&(data))>>3)
#define SP2_CLMPCTRL_get_dummy18021048_2(data)                        ((0x00000004&(data))>>2)
#define SP2_CLMPCTRL_get_s2_clamp_test_en(data)                       ((0x00000002&(data))>>1)
#define SP2_CLMPCTRL_get_s2_adc_hs_syncedge(data)                     (0x00000001&(data))

#define SP2_CLMP                                                      0x1802104c
#define SP2_CLMP_reg_addr                                             "0xB802104C"
#define SP2_CLMP_reg                                                  0xB802104C
#define SP2_CLMP_inst_addr                                            "0x0013"
#define SP2_CLMP_inst                                                 0x0013
#define SP2_CLMP_s2_yclamp_sta_shift                                  (24)
#define SP2_CLMP_s2_yclamp_end_shift                                  (16)
#define SP2_CLMP_s2_yclamp_sta_mask                                   (0xFF000000)
#define SP2_CLMP_s2_yclamp_end_mask                                   (0x00FF0000)
#define SP2_CLMP_s2_yclamp_sta(data)                                  (0xFF000000&(((UINT32)data)<<24))
#define SP2_CLMP_s2_yclamp_end(data)                                  (0x00FF0000&(((UINT32)data)<<16))
#define SP2_CLMP_get_s2_yclamp_sta(data)                              ((0xFF000000&(data))>>24)
#define SP2_CLMP_get_s2_yclamp_end(data)                              ((0x00FF0000&(data))>>16)

#define SP2_CLMPMSK                                                   0x18021050
#define SP2_CLMPMSK_reg_addr                                          "0xB8021050"
#define SP2_CLMPMSK_reg                                               0xB8021050
#define SP2_CLMPMSK_inst_addr                                         "0x0014"
#define SP2_CLMPMSK_inst                                              0x0014
#define SP2_CLMPMSK_s2_mask_clamp_en_shift                            (15)
#define SP2_CLMPMSK_dummy18021050_14_shift                            (14)
#define SP2_CLMPMSK_s2_mask_front_shift                               (8)
#define SP2_CLMPMSK_dummy18021050_7_6_shift                           (6)
#define SP2_CLMPMSK_s2_mask_back_shift                                (0)
#define SP2_CLMPMSK_s2_mask_clamp_en_mask                             (0x00008000)
#define SP2_CLMPMSK_dummy18021050_14_mask                             (0x00004000)
#define SP2_CLMPMSK_s2_mask_front_mask                                (0x00003F00)
#define SP2_CLMPMSK_dummy18021050_7_6_mask                            (0x000000C0)
#define SP2_CLMPMSK_s2_mask_back_mask                                 (0x0000003F)
#define SP2_CLMPMSK_s2_mask_clamp_en(data)                            (0x00008000&(((UINT32)data)<<15))
#define SP2_CLMPMSK_dummy18021050_14(data)                            (0x00004000&(((UINT32)data)<<14))
#define SP2_CLMPMSK_s2_mask_front(data)                               (0x00003F00&(((UINT32)data)<<8))
#define SP2_CLMPMSK_dummy18021050_7_6(data)                           (0x000000C0&(((UINT32)data)<<6))
#define SP2_CLMPMSK_s2_mask_back(data)                                (0x0000003F&(data))
#define SP2_CLMPMSK_get_s2_mask_clamp_en(data)                        ((0x00008000&(data))>>15)
#define SP2_CLMPMSK_get_dummy18021050_14(data)                        ((0x00004000&(data))>>14)
#define SP2_CLMPMSK_get_s2_mask_front(data)                           ((0x00003F00&(data))>>8)
#define SP2_CLMPMSK_get_dummy18021050_7_6(data)                       ((0x000000C0&(data))>>6)
#define SP2_CLMPMSK_get_s2_mask_back(data)                            (0x0000003F&(data))

#define SP2_COASTFTBK                                                 0x18021054
#define SP2_COASTFTBK_reg_addr                                        "0xB8021054"
#define SP2_COASTFTBK_reg                                             0xB8021054
#define SP2_COASTFTBK_inst_addr                                       "0x0015"
#define SP2_COASTFTBK_inst                                            0x0015

#define MV_CTRL                                                       0x18021058
#define MV_CTRL_reg_addr                                              "0xB8021058"
#define MV_CTRL_reg                                                   0xB8021058
#define MV_CTRL_inst_addr                                             "0x0016"
#define MV_CTRL_inst                                                  0x0016
#define MV_CTRL_dummy18021058_15_13_shift                             (13)
#define MV_CTRL_s1_deb_num_mvhs_shift                                 (8)
#define MV_CTRL_s1_skip_line_len_shift                                (4)
#define MV_CTRL_dummy18021058_3_2_shift                               (2)
#define MV_CTRL_s1_mv_occur_shift                                     (1)
#define MV_CTRL_s1_mv_enable_shift                                    (0)
#define MV_CTRL_dummy18021058_15_13_mask                              (0x0000E000)
#define MV_CTRL_s1_deb_num_mvhs_mask                                  (0x00001F00)
#define MV_CTRL_s1_skip_line_len_mask                                 (0x000000F0)
#define MV_CTRL_dummy18021058_3_2_mask                                (0x0000000C)
#define MV_CTRL_s1_mv_occur_mask                                      (0x00000002)
#define MV_CTRL_s1_mv_enable_mask                                     (0x00000001)
#define MV_CTRL_dummy18021058_15_13(data)                             (0x0000E000&(((UINT32)data)<<13))
#define MV_CTRL_s1_deb_num_mvhs(data)                                 (0x00001F00&(((UINT32)data)<<8))
#define MV_CTRL_s1_skip_line_len(data)                                (0x000000F0&(((UINT32)data)<<4))
#define MV_CTRL_dummy18021058_3_2(data)                               (0x0000000C&(((UINT32)data)<<2))
#define MV_CTRL_s1_mv_occur(data)                                     (0x00000002&(((UINT32)data)<<1))
#define MV_CTRL_s1_mv_enable(data)                                    (0x00000001&(data))
#define MV_CTRL_get_dummy18021058_15_13(data)                         ((0x0000E000&(data))>>13)
#define MV_CTRL_get_s1_deb_num_mvhs(data)                             ((0x00001F00&(data))>>8)
#define MV_CTRL_get_s1_skip_line_len(data)                            ((0x000000F0&(data))>>4)
#define MV_CTRL_get_dummy18021058_3_2(data)                           ((0x0000000C&(data))>>2)
#define MV_CTRL_get_s1_mv_occur(data)                                 ((0x00000002&(data))>>1)
#define MV_CTRL_get_s1_mv_enable(data)                                (0x00000001&(data))

#define MV_FLD_POS                                                    0x1802105c
#define MV_FLD_POS_reg_addr                                           "0xB802105C"
#define MV_FLD_POS_reg                                                0xB802105C
#define MV_FLD_POS_inst_addr                                          "0x0017"
#define MV_FLD_POS_inst                                               0x0017
#define MV_FLD_POS_dummy1802105c_31_shift                             (31)
#define MV_FLD_POS_s1_o_mv_str_shift                                  (24)
#define MV_FLD_POS_s1_o_mv_rdy_shift                                  (23)
#define MV_FLD_POS_s1_o_mv_end_shift                                  (16)
#define MV_FLD_POS_dummy1802105c_15_shift                             (15)
#define MV_FLD_POS_s1_e_mv_str_shift                                  (8)
#define MV_FLD_POS_s1_e_mv_rdy_shift                                  (7)
#define MV_FLD_POS_s1_e_mv_end_shift                                  (0)
#define MV_FLD_POS_dummy1802105c_31_mask                              (0x80000000)
#define MV_FLD_POS_s1_o_mv_str_mask                                   (0x7F000000)
#define MV_FLD_POS_s1_o_mv_rdy_mask                                   (0x00800000)
#define MV_FLD_POS_s1_o_mv_end_mask                                   (0x007F0000)
#define MV_FLD_POS_dummy1802105c_15_mask                              (0x00008000)
#define MV_FLD_POS_s1_e_mv_str_mask                                   (0x00007F00)
#define MV_FLD_POS_s1_e_mv_rdy_mask                                   (0x00000080)
#define MV_FLD_POS_s1_e_mv_end_mask                                   (0x0000007F)
#define MV_FLD_POS_dummy1802105c_31(data)                             (0x80000000&(((UINT32)data)<<31))
#define MV_FLD_POS_s1_o_mv_str(data)                                  (0x7F000000&(((UINT32)data)<<24))
#define MV_FLD_POS_s1_o_mv_rdy(data)                                  (0x00800000&(((UINT32)data)<<23))
#define MV_FLD_POS_s1_o_mv_end(data)                                  (0x007F0000&(((UINT32)data)<<16))
#define MV_FLD_POS_dummy1802105c_15(data)                             (0x00008000&(((UINT32)data)<<15))
#define MV_FLD_POS_s1_e_mv_str(data)                                  (0x00007F00&(((UINT32)data)<<8))
#define MV_FLD_POS_s1_e_mv_rdy(data)                                  (0x00000080&(((UINT32)data)<<7))
#define MV_FLD_POS_s1_e_mv_end(data)                                  (0x0000007F&(data))
#define MV_FLD_POS_get_dummy1802105c_31(data)                         ((0x80000000&(data))>>31)
#define MV_FLD_POS_get_s1_o_mv_str(data)                              ((0x7F000000&(data))>>24)
#define MV_FLD_POS_get_s1_o_mv_rdy(data)                              ((0x00800000&(data))>>23)
#define MV_FLD_POS_get_s1_o_mv_end(data)                              ((0x007F0000&(data))>>16)
#define MV_FLD_POS_get_dummy1802105c_15(data)                         ((0x00008000&(data))>>15)
#define MV_FLD_POS_get_s1_e_mv_str(data)                              ((0x00007F00&(data))>>8)
#define MV_FLD_POS_get_s1_e_mv_rdy(data)                              ((0x00000080&(data))>>7)
#define MV_FLD_POS_get_s1_e_mv_end(data)                              (0x0000007F&(data))

#define MV2_CTRL                                                      0x18021060
#define MV2_CTRL_reg_addr                                             "0xB8021060"
#define MV2_CTRL_reg                                                  0xB8021060
#define MV2_CTRL_inst_addr                                            "0x0018"
#define MV2_CTRL_inst                                                 0x0018
#define MV2_CTRL_dummy18021060_15_13_shift                            (13)
#define MV2_CTRL_s2_deb_num_mvhs_shift                                (8)
#define MV2_CTRL_s2_skip_line_len_shift                               (4)
#define MV2_CTRL_dummy18021060_3_1_shift                              (1)
#define MV2_CTRL_s2_mv_enable_shift                                   (0)
#define MV2_CTRL_dummy18021060_15_13_mask                             (0x0000E000)
#define MV2_CTRL_s2_deb_num_mvhs_mask                                 (0x00001F00)
#define MV2_CTRL_s2_skip_line_len_mask                                (0x000000F0)
#define MV2_CTRL_dummy18021060_3_1_mask                               (0x0000000E)
#define MV2_CTRL_s2_mv_enable_mask                                    (0x00000001)
#define MV2_CTRL_dummy18021060_15_13(data)                            (0x0000E000&(((UINT32)data)<<13))
#define MV2_CTRL_s2_deb_num_mvhs(data)                                (0x00001F00&(((UINT32)data)<<8))
#define MV2_CTRL_s2_skip_line_len(data)                               (0x000000F0&(((UINT32)data)<<4))
#define MV2_CTRL_dummy18021060_3_1(data)                              (0x0000000E&(((UINT32)data)<<1))
#define MV2_CTRL_s2_mv_enable(data)                                   (0x00000001&(data))
#define MV2_CTRL_get_dummy18021060_15_13(data)                        ((0x0000E000&(data))>>13)
#define MV2_CTRL_get_s2_deb_num_mvhs(data)                            ((0x00001F00&(data))>>8)
#define MV2_CTRL_get_s2_skip_line_len(data)                           ((0x000000F0&(data))>>4)
#define MV2_CTRL_get_dummy18021060_3_1(data)                          ((0x0000000E&(data))>>1)
#define MV2_CTRL_get_s2_mv_enable(data)                               (0x00000001&(data))

#define SP_ADCCLAMPSEL0                                               0x18021064
#define SP_ADCCLAMPSEL0_reg_addr                                      "0xB8021064"
#define SP_ADCCLAMPSEL0_reg                                           0xB8021064
#define SP_ADCCLAMPSEL0_inst_addr                                     "0x0019"
#define SP_ADCCLAMPSEL0_inst                                          0x0019
#define SP_ADCCLAMPSEL0_vd_adc_clamp11_sel_shift                      (29)
#define SP_ADCCLAMPSEL0_vd_adc_clamp10_sel_shift                      (28)
#define SP_ADCCLAMPSEL0_vd_adc_clamp9_sel_shift                       (27)
#define SP_ADCCLAMPSEL0_vd_adc_clamp8_sel_shift                       (26)
#define SP_ADCCLAMPSEL0_vd_adc_clamp7_sel_shift                       (25)
#define SP_ADCCLAMPSEL0_vd_adc_clamp6_sel_shift                       (24)
#define SP_ADCCLAMPSEL0_vd_adc_clamp5_sel_shift                       (23)
#define SP_ADCCLAMPSEL0_vd_adc_clamp4_sel_shift                       (22)
#define SP_ADCCLAMPSEL0_vd_adc_clamp3_sel_shift                       (21)
#define SP_ADCCLAMPSEL0_vd_adc_clamp2_sel_shift                       (20)
#define SP_ADCCLAMPSEL0_vd_adc_clamp1_sel_shift                       (19)
#define SP_ADCCLAMPSEL0_vd_adc_clamp0_sel_shift                       (18)
#define SP_ADCCLAMPSEL0_adc_r0_clamp_sel_shift                        (16)
#define SP_ADCCLAMPSEL0_adc_r1_clamp_sel_shift                        (14)
#define SP_ADCCLAMPSEL0_adc_r2_clamp_sel_shift                        (12)
#define SP_ADCCLAMPSEL0_adc_g0_clamp_sel_shift                        (10)
#define SP_ADCCLAMPSEL0_adc_g1_clamp_sel_shift                        (8)
#define SP_ADCCLAMPSEL0_adc_g2_clamp_sel_shift                        (6)
#define SP_ADCCLAMPSEL0_adc_b0_clamp_sel_shift                        (4)
#define SP_ADCCLAMPSEL0_adc_b1_clamp_sel_shift                        (2)
#define SP_ADCCLAMPSEL0_adc_b2_clamp_sel_shift                        (0)
#define SP_ADCCLAMPSEL0_vd_adc_clamp11_sel_mask                       (0x20000000)
#define SP_ADCCLAMPSEL0_vd_adc_clamp10_sel_mask                       (0x10000000)
#define SP_ADCCLAMPSEL0_vd_adc_clamp9_sel_mask                        (0x08000000)
#define SP_ADCCLAMPSEL0_vd_adc_clamp8_sel_mask                        (0x04000000)
#define SP_ADCCLAMPSEL0_vd_adc_clamp7_sel_mask                        (0x02000000)
#define SP_ADCCLAMPSEL0_vd_adc_clamp6_sel_mask                        (0x01000000)
#define SP_ADCCLAMPSEL0_vd_adc_clamp5_sel_mask                        (0x00800000)
#define SP_ADCCLAMPSEL0_vd_adc_clamp4_sel_mask                        (0x00400000)
#define SP_ADCCLAMPSEL0_vd_adc_clamp3_sel_mask                        (0x00200000)
#define SP_ADCCLAMPSEL0_vd_adc_clamp2_sel_mask                        (0x00100000)
#define SP_ADCCLAMPSEL0_vd_adc_clamp1_sel_mask                        (0x00080000)
#define SP_ADCCLAMPSEL0_vd_adc_clamp0_sel_mask                        (0x00040000)
#define SP_ADCCLAMPSEL0_adc_r0_clamp_sel_mask                         (0x00030000)
#define SP_ADCCLAMPSEL0_adc_r1_clamp_sel_mask                         (0x0000C000)
#define SP_ADCCLAMPSEL0_adc_r2_clamp_sel_mask                         (0x00003000)
#define SP_ADCCLAMPSEL0_adc_g0_clamp_sel_mask                         (0x00000C00)
#define SP_ADCCLAMPSEL0_adc_g1_clamp_sel_mask                         (0x00000300)
#define SP_ADCCLAMPSEL0_adc_g2_clamp_sel_mask                         (0x000000C0)
#define SP_ADCCLAMPSEL0_adc_b0_clamp_sel_mask                         (0x00000030)
#define SP_ADCCLAMPSEL0_adc_b1_clamp_sel_mask                         (0x0000000C)
#define SP_ADCCLAMPSEL0_adc_b2_clamp_sel_mask                         (0x00000003)
#define SP_ADCCLAMPSEL0_vd_adc_clamp11_sel(data)                      (0x20000000&(((UINT32)data)<<29))
#define SP_ADCCLAMPSEL0_vd_adc_clamp10_sel(data)                      (0x10000000&(((UINT32)data)<<28))
#define SP_ADCCLAMPSEL0_vd_adc_clamp9_sel(data)                       (0x08000000&(((UINT32)data)<<27))
#define SP_ADCCLAMPSEL0_vd_adc_clamp8_sel(data)                       (0x04000000&(((UINT32)data)<<26))
#define SP_ADCCLAMPSEL0_vd_adc_clamp7_sel(data)                       (0x02000000&(((UINT32)data)<<25))
#define SP_ADCCLAMPSEL0_vd_adc_clamp6_sel(data)                       (0x01000000&(((UINT32)data)<<24))
#define SP_ADCCLAMPSEL0_vd_adc_clamp5_sel(data)                       (0x00800000&(((UINT32)data)<<23))
#define SP_ADCCLAMPSEL0_vd_adc_clamp4_sel(data)                       (0x00400000&(((UINT32)data)<<22))
#define SP_ADCCLAMPSEL0_vd_adc_clamp3_sel(data)                       (0x00200000&(((UINT32)data)<<21))
#define SP_ADCCLAMPSEL0_vd_adc_clamp2_sel(data)                       (0x00100000&(((UINT32)data)<<20))
#define SP_ADCCLAMPSEL0_vd_adc_clamp1_sel(data)                       (0x00080000&(((UINT32)data)<<19))
#define SP_ADCCLAMPSEL0_vd_adc_clamp0_sel(data)                       (0x00040000&(((UINT32)data)<<18))
#define SP_ADCCLAMPSEL0_adc_r0_clamp_sel(data)                        (0x00030000&(((UINT32)data)<<16))
#define SP_ADCCLAMPSEL0_adc_r1_clamp_sel(data)                        (0x0000C000&(((UINT32)data)<<14))
#define SP_ADCCLAMPSEL0_adc_r2_clamp_sel(data)                        (0x00003000&(((UINT32)data)<<12))
#define SP_ADCCLAMPSEL0_adc_g0_clamp_sel(data)                        (0x00000C00&(((UINT32)data)<<10))
#define SP_ADCCLAMPSEL0_adc_g1_clamp_sel(data)                        (0x00000300&(((UINT32)data)<<8))
#define SP_ADCCLAMPSEL0_adc_g2_clamp_sel(data)                        (0x000000C0&(((UINT32)data)<<6))
#define SP_ADCCLAMPSEL0_adc_b0_clamp_sel(data)                        (0x00000030&(((UINT32)data)<<4))
#define SP_ADCCLAMPSEL0_adc_b1_clamp_sel(data)                        (0x0000000C&(((UINT32)data)<<2))
#define SP_ADCCLAMPSEL0_adc_b2_clamp_sel(data)                        (0x00000003&(data))
#define SP_ADCCLAMPSEL0_get_vd_adc_clamp11_sel(data)                  ((0x20000000&(data))>>29)
#define SP_ADCCLAMPSEL0_get_vd_adc_clamp10_sel(data)                  ((0x10000000&(data))>>28)
#define SP_ADCCLAMPSEL0_get_vd_adc_clamp9_sel(data)                   ((0x08000000&(data))>>27)
#define SP_ADCCLAMPSEL0_get_vd_adc_clamp8_sel(data)                   ((0x04000000&(data))>>26)
#define SP_ADCCLAMPSEL0_get_vd_adc_clamp7_sel(data)                   ((0x02000000&(data))>>25)
#define SP_ADCCLAMPSEL0_get_vd_adc_clamp6_sel(data)                   ((0x01000000&(data))>>24)
#define SP_ADCCLAMPSEL0_get_vd_adc_clamp5_sel(data)                   ((0x00800000&(data))>>23)
#define SP_ADCCLAMPSEL0_get_vd_adc_clamp4_sel(data)                   ((0x00400000&(data))>>22)
#define SP_ADCCLAMPSEL0_get_vd_adc_clamp3_sel(data)                   ((0x00200000&(data))>>21)
#define SP_ADCCLAMPSEL0_get_vd_adc_clamp2_sel(data)                   ((0x00100000&(data))>>20)
#define SP_ADCCLAMPSEL0_get_vd_adc_clamp1_sel(data)                   ((0x00080000&(data))>>19)
#define SP_ADCCLAMPSEL0_get_vd_adc_clamp0_sel(data)                   ((0x00040000&(data))>>18)
#define SP_ADCCLAMPSEL0_get_adc_r0_clamp_sel(data)                    ((0x00030000&(data))>>16)
#define SP_ADCCLAMPSEL0_get_adc_r1_clamp_sel(data)                    ((0x0000C000&(data))>>14)
#define SP_ADCCLAMPSEL0_get_adc_r2_clamp_sel(data)                    ((0x00003000&(data))>>12)
#define SP_ADCCLAMPSEL0_get_adc_g0_clamp_sel(data)                    ((0x00000C00&(data))>>10)
#define SP_ADCCLAMPSEL0_get_adc_g1_clamp_sel(data)                    ((0x00000300&(data))>>8)
#define SP_ADCCLAMPSEL0_get_adc_g2_clamp_sel(data)                    ((0x000000C0&(data))>>6)
#define SP_ADCCLAMPSEL0_get_adc_b0_clamp_sel(data)                    ((0x00000030&(data))>>4)
#define SP_ADCCLAMPSEL0_get_adc_b1_clamp_sel(data)                    ((0x0000000C&(data))>>2)
#define SP_ADCCLAMPSEL0_get_adc_b2_clamp_sel(data)                    (0x00000003&(data))

#define SP_ADCCLAMPSEL1                                               0x18021068
#define SP_ADCCLAMPSEL1_reg_addr                                      "0xB8021068"
#define SP_ADCCLAMPSEL1_reg                                           0xB8021068
#define SP_ADCCLAMPSEL1_inst_addr                                     "0x001A"
#define SP_ADCCLAMPSEL1_inst                                          0x001A
#define SP_ADCCLAMPSEL1_dummy18021068_4_shift                         (4)
#define SP_ADCCLAMPSEL1_sync_test_sel_shift                           (0)
#define SP_ADCCLAMPSEL1_dummy18021068_4_mask                          (0x00000010)
#define SP_ADCCLAMPSEL1_sync_test_sel_mask                            (0x0000000F)
#define SP_ADCCLAMPSEL1_dummy18021068_4(data)                         (0x00000010&(((UINT32)data)<<4))
#define SP_ADCCLAMPSEL1_sync_test_sel(data)                           (0x0000000F&(data))
#define SP_ADCCLAMPSEL1_get_dummy18021068_4(data)                     ((0x00000010&(data))>>4)
#define SP_ADCCLAMPSEL1_get_sync_test_sel(data)                       (0x0000000F&(data))

#define SP_ADC_OUT_SEL                                                0x1802106c
#define SP_ADC_OUT_SEL_reg_addr                                       "0xB802106C"
#define SP_ADC_OUT_SEL_reg                                            0xB802106C
#define SP_ADC_OUT_SEL_inst_addr                                      "0x001B"
#define SP_ADC_OUT_SEL_inst                                           0x001B
#define SP_ADC_OUT_SEL_syncedge_rgb_shift                             (8)
#define SP_ADC_OUT_SEL_syncedge_hs_shift                              (7)
#define SP_ADC_OUT_SEL_syncedge_vs_shift                              (6)
#define SP_ADC_OUT_SEL_test_sel_rgb_shift                             (4)
#define SP_ADC_OUT_SEL_test_sel_hs_shift                              (3)
#define SP_ADC_OUT_SEL_test_sel_vs_shift                              (2)
#define SP_ADC_OUT_SEL_sync_out_source_shift                          (0)
#define SP_ADC_OUT_SEL_syncedge_rgb_mask                              (0x00000100)
#define SP_ADC_OUT_SEL_syncedge_hs_mask                               (0x00000080)
#define SP_ADC_OUT_SEL_syncedge_vs_mask                               (0x00000040)
#define SP_ADC_OUT_SEL_test_sel_rgb_mask                              (0x00000030)
#define SP_ADC_OUT_SEL_test_sel_hs_mask                               (0x00000008)
#define SP_ADC_OUT_SEL_test_sel_vs_mask                               (0x00000004)
#define SP_ADC_OUT_SEL_sync_out_source_mask                           (0x00000003)
#define SP_ADC_OUT_SEL_syncedge_rgb(data)                             (0x00000100&(((UINT32)data)<<8))
#define SP_ADC_OUT_SEL_syncedge_hs(data)                              (0x00000080&(((UINT32)data)<<7))
#define SP_ADC_OUT_SEL_syncedge_vs(data)                              (0x00000040&(((UINT32)data)<<6))
#define SP_ADC_OUT_SEL_test_sel_rgb(data)                             (0x00000030&(((UINT32)data)<<4))
#define SP_ADC_OUT_SEL_test_sel_hs(data)                              (0x00000008&(((UINT32)data)<<3))
#define SP_ADC_OUT_SEL_test_sel_vs(data)                              (0x00000004&(((UINT32)data)<<2))
#define SP_ADC_OUT_SEL_sync_out_source(data)                          (0x00000003&(data))
#define SP_ADC_OUT_SEL_get_syncedge_rgb(data)                         ((0x00000100&(data))>>8)
#define SP_ADC_OUT_SEL_get_syncedge_hs(data)                          ((0x00000080&(data))>>7)
#define SP_ADC_OUT_SEL_get_syncedge_vs(data)                          ((0x00000040&(data))>>6)
#define SP_ADC_OUT_SEL_get_test_sel_rgb(data)                         ((0x00000030&(data))>>4)
#define SP_ADC_OUT_SEL_get_test_sel_hs(data)                          ((0x00000008&(data))>>3)
#define SP_ADC_OUT_SEL_get_test_sel_vs(data)                          ((0x00000004&(data))>>2)
#define SP_ADC_OUT_SEL_get_sync_out_source(data)                      (0x00000003&(data))

#define SP_PAD_STATUS                                                 0x18021070
#define SP_PAD_STATUS_reg_addr                                        "0xB8021070"
#define SP_PAD_STATUS_reg                                             0xB8021070
#define SP_PAD_STATUS_inst_addr                                       "0x001C"
#define SP_PAD_STATUS_inst                                            0x001C
#define SP_PAD_STATUS_dummy18021070_7_6_shift                         (6)
#define SP_PAD_STATUS_tog_adc_sog0_shift                              (5)
#define SP_PAD_STATUS_tog_adc_sog1_shift                              (4)
#define SP_PAD_STATUS_tog_soy_vd_shift                                (3)
#define SP_PAD_STATUS_tog_pad_avs1_shift                              (1)
#define SP_PAD_STATUS_tog_pad_ahs1_shift                              (0)
#define SP_PAD_STATUS_dummy18021070_7_6_mask                          (0x000000C0)
#define SP_PAD_STATUS_tog_adc_sog0_mask                               (0x00000020)
#define SP_PAD_STATUS_tog_adc_sog1_mask                               (0x00000010)
#define SP_PAD_STATUS_tog_soy_vd_mask                                 (0x00000008)
#define SP_PAD_STATUS_tog_pad_avs1_mask                               (0x00000002)
#define SP_PAD_STATUS_tog_pad_ahs1_mask                               (0x00000001)
#define SP_PAD_STATUS_dummy18021070_7_6(data)                         (0x000000C0&(((UINT32)data)<<6))
#define SP_PAD_STATUS_tog_adc_sog0(data)                              (0x00000020&(((UINT32)data)<<5))
#define SP_PAD_STATUS_tog_adc_sog1(data)                              (0x00000010&(((UINT32)data)<<4))
#define SP_PAD_STATUS_tog_soy_vd(data)                                (0x00000008&(((UINT32)data)<<3))
#define SP_PAD_STATUS_tog_pad_avs1(data)                              (0x00000002&(((UINT32)data)<<1))
#define SP_PAD_STATUS_tog_pad_ahs1(data)                              (0x00000001&(data))
#define SP_PAD_STATUS_get_dummy18021070_7_6(data)                     ((0x000000C0&(data))>>6)
#define SP_PAD_STATUS_get_tog_adc_sog0(data)                          ((0x00000020&(data))>>5)
#define SP_PAD_STATUS_get_tog_adc_sog1(data)                          ((0x00000010&(data))>>4)
#define SP_PAD_STATUS_get_tog_soy_vd(data)                            ((0x00000008&(data))>>3)
#define SP_PAD_STATUS_get_tog_pad_avs1(data)                          ((0x00000002&(data))>>1)
#define SP_PAD_STATUS_get_tog_pad_ahs1(data)                          (0x00000001&(data))

#define SP_MS3CTRL                                                    0x18021100
#define SP_MS3CTRL_reg_addr                                           "0xB8021100"
#define SP_MS3CTRL_reg                                                0xB8021100
#define SP_MS3CTRL_inst_addr                                          "0x0040"
#define SP_MS3CTRL_inst                                               0x0040
#define SP_MS3CTRL_dummy18021100_31_shift                             (31)
#define SP_MS3CTRL_off_ms_now_shift                                   (21)
#define SP_MS3CTRL_off_timeclk_test_mode_shift                        (20)
#define SP_MS3CTRL_off_meas_source_shift                              (16)
#define SP_MS3CTRL_off_vs_meas_inv_shift                              (15)
#define SP_MS3CTRL_off_vsms_inv_shift                                 (14)
#define SP_MS3CTRL_off_sycms_mode_shift                               (13)
#define SP_MS3CTRL_off_popup_meas_shift                               (12)
#define SP_MS3CTRL_off_hsms_inv_shift                                 (11)
#define SP_MS3CTRL_off_online_en_shift                                (10)
#define SP_MS3CTRL_off_start_ms_shift                                 (9)
#define SP_MS3CTRL_off_hs_max_delta_shift                             (4)
#define SP_MS3CTRL_off_vs_max_delta_shift                             (0)
#define SP_MS3CTRL_dummy18021100_31_mask                              (0x80000000)
#define SP_MS3CTRL_off_ms_now_mask                                    (0x00200000)
#define SP_MS3CTRL_off_timeclk_test_mode_mask                         (0x00100000)
#define SP_MS3CTRL_off_meas_source_mask                               (0x000F0000)
#define SP_MS3CTRL_off_vs_meas_inv_mask                               (0x00008000)
#define SP_MS3CTRL_off_vsms_inv_mask                                  (0x00004000)
#define SP_MS3CTRL_off_sycms_mode_mask                                (0x00002000)
#define SP_MS3CTRL_off_popup_meas_mask                                (0x00001000)
#define SP_MS3CTRL_off_hsms_inv_mask                                  (0x00000800)
#define SP_MS3CTRL_off_online_en_mask                                 (0x00000400)
#define SP_MS3CTRL_off_start_ms_mask                                  (0x00000200)
#define SP_MS3CTRL_off_hs_max_delta_mask                              (0x000001F0)
#define SP_MS3CTRL_off_vs_max_delta_mask                              (0x0000000F)
#define SP_MS3CTRL_dummy18021100_31(data)                             (0x80000000&(((UINT32)data)<<31))
#define SP_MS3CTRL_off_ms_now(data)                                   (0x00200000&(((UINT32)data)<<21))
#define SP_MS3CTRL_off_timeclk_test_mode(data)                        (0x00100000&(((UINT32)data)<<20))
#define SP_MS3CTRL_off_meas_source(data)                              (0x000F0000&(((UINT32)data)<<16))
#define SP_MS3CTRL_off_vs_meas_inv(data)                              (0x00008000&(((UINT32)data)<<15))
#define SP_MS3CTRL_off_vsms_inv(data)                                 (0x00004000&(((UINT32)data)<<14))
#define SP_MS3CTRL_off_sycms_mode(data)                               (0x00002000&(((UINT32)data)<<13))
#define SP_MS3CTRL_off_popup_meas(data)                               (0x00001000&(((UINT32)data)<<12))
#define SP_MS3CTRL_off_hsms_inv(data)                                 (0x00000800&(((UINT32)data)<<11))
#define SP_MS3CTRL_off_online_en(data)                                (0x00000400&(((UINT32)data)<<10))
#define SP_MS3CTRL_off_start_ms(data)                                 (0x00000200&(((UINT32)data)<<9))
#define SP_MS3CTRL_off_hs_max_delta(data)                             (0x000001F0&(((UINT32)data)<<4))
#define SP_MS3CTRL_off_vs_max_delta(data)                             (0x0000000F&(data))
#define SP_MS3CTRL_get_dummy18021100_31(data)                         ((0x80000000&(data))>>31)
#define SP_MS3CTRL_get_off_ms_now(data)                               ((0x00200000&(data))>>21)
#define SP_MS3CTRL_get_off_timeclk_test_mode(data)                    ((0x00100000&(data))>>20)
#define SP_MS3CTRL_get_off_meas_source(data)                          ((0x000F0000&(data))>>16)
#define SP_MS3CTRL_get_off_vs_meas_inv(data)                          ((0x00008000&(data))>>15)
#define SP_MS3CTRL_get_off_vsms_inv(data)                             ((0x00004000&(data))>>14)
#define SP_MS3CTRL_get_off_sycms_mode(data)                           ((0x00002000&(data))>>13)
#define SP_MS3CTRL_get_off_popup_meas(data)                           ((0x00001000&(data))>>12)
#define SP_MS3CTRL_get_off_hsms_inv(data)                             ((0x00000800&(data))>>11)
#define SP_MS3CTRL_get_off_online_en(data)                            ((0x00000400&(data))>>10)
#define SP_MS3CTRL_get_off_start_ms(data)                             ((0x00000200&(data))>>9)
#define SP_MS3CTRL_get_off_hs_max_delta(data)                         ((0x000001F0&(data))>>4)
#define SP_MS3CTRL_get_off_vs_max_delta(data)                         (0x0000000F&(data))

#define SP_MS3RST0                                                    0x18021104
#define SP_MS3RST0_reg_addr                                           "0xB8021104"
#define SP_MS3RST0_reg                                                0xB8021104
#define SP_MS3RST0_inst_addr                                          "0x0041"
#define SP_MS3RST0_inst                                               0x0041
#define SP_MS3RST0_off_vs_period_out_shift                            (16)
#define SP_MS3RST0_off_hs_period_out_h12b_shift                       (4)
#define SP_MS3RST0_off_hs_period_out_f4b_shift                        (0)
#define SP_MS3RST0_off_vs_period_out_mask                             (0x0FFF0000)
#define SP_MS3RST0_off_hs_period_out_h12b_mask                        (0x0000FFF0)
#define SP_MS3RST0_off_hs_period_out_f4b_mask                         (0x0000000F)
#define SP_MS3RST0_off_vs_period_out(data)                            (0x0FFF0000&(((UINT32)data)<<16))
#define SP_MS3RST0_off_hs_period_out_h12b(data)                       (0x0000FFF0&(((UINT32)data)<<4))
#define SP_MS3RST0_off_hs_period_out_f4b(data)                        (0x0000000F&(data))
#define SP_MS3RST0_get_off_vs_period_out(data)                        ((0x0FFF0000&(data))>>16)
#define SP_MS3RST0_get_off_hs_period_out_h12b(data)                   ((0x0000FFF0&(data))>>4)
#define SP_MS3RST0_get_off_hs_period_out_f4b(data)                    (0x0000000F&(data))

#define SP_MS3RST1                                                    0x18021108
#define SP_MS3RST1_reg_addr                                           "0xB8021108"
#define SP_MS3RST1_reg                                                0xB8021108
#define SP_MS3RST1_inst_addr                                          "0x0042"
#define SP_MS3RST1_inst                                               0x0042
#define SP_MS3RST1_dummy18021108_31_28_shift                          (28)
#define SP_MS3RST1_off_vs_high_out_shift                              (16)
#define SP_MS3RST1_dummy18021108_15_12_shift                          (12)
#define SP_MS3RST1_off_hs_high_out_shift                              (0)
#define SP_MS3RST1_dummy18021108_31_28_mask                           (0xF0000000)
#define SP_MS3RST1_off_vs_high_out_mask                               (0x0FFF0000)
#define SP_MS3RST1_dummy18021108_15_12_mask                           (0x0000F000)
#define SP_MS3RST1_off_hs_high_out_mask                               (0x00000FFF)
#define SP_MS3RST1_dummy18021108_31_28(data)                          (0xF0000000&(((UINT32)data)<<28))
#define SP_MS3RST1_off_vs_high_out(data)                              (0x0FFF0000&(((UINT32)data)<<16))
#define SP_MS3RST1_dummy18021108_15_12(data)                          (0x0000F000&(((UINT32)data)<<12))
#define SP_MS3RST1_off_hs_high_out(data)                              (0x00000FFF&(data))
#define SP_MS3RST1_get_dummy18021108_31_28(data)                      ((0xF0000000&(data))>>28)
#define SP_MS3RST1_get_off_vs_high_out(data)                          ((0x0FFF0000&(data))>>16)
#define SP_MS3RST1_get_dummy18021108_15_12(data)                      ((0x0000F000&(data))>>12)
#define SP_MS3RST1_get_off_hs_high_out(data)                          (0x00000FFF&(data))

#define SP_MS3STUS                                                    0x1802110c
#define SP_MS3STUS_reg_addr                                           "0xB802110C"
#define SP_MS3STUS_reg                                                0xB802110C
#define SP_MS3STUS_inst_addr                                          "0x0043"
#define SP_MS3STUS_inst                                               0x0043
#define SP_MS3STUS_dummy1802110c_11_10_shift                          (10)
#define SP_MS3STUS_off_vs_pol_out_shift                               (9)
#define SP_MS3STUS_off_hs_pol_out_shift                               (8)
#define SP_MS3STUS_off_vs_per_to_shift                                (7)
#define SP_MS3STUS_off_vs_high_to_shift                               (6)
#define SP_MS3STUS_off_vs_per_of_shift                                (5)
#define SP_MS3STUS_off_vs_over_range_shift                            (4)
#define SP_MS3STUS_off_vs_pol_chg_shift                               (3)
#define SP_MS3STUS_off_hs_per_of_shift                                (2)
#define SP_MS3STUS_off_hs_over_range_shift                            (1)
#define SP_MS3STUS_off_hs_pol_chg_shift                               (0)
#define SP_MS3STUS_dummy1802110c_11_10_mask                           (0x00000C00)
#define SP_MS3STUS_off_vs_pol_out_mask                                (0x00000200)
#define SP_MS3STUS_off_hs_pol_out_mask                                (0x00000100)
#define SP_MS3STUS_off_vs_per_to_mask                                 (0x00000080)
#define SP_MS3STUS_off_vs_high_to_mask                                (0x00000040)
#define SP_MS3STUS_off_vs_per_of_mask                                 (0x00000020)
#define SP_MS3STUS_off_vs_over_range_mask                             (0x00000010)
#define SP_MS3STUS_off_vs_pol_chg_mask                                (0x00000008)
#define SP_MS3STUS_off_hs_per_of_mask                                 (0x00000004)
#define SP_MS3STUS_off_hs_over_range_mask                             (0x00000002)
#define SP_MS3STUS_off_hs_pol_chg_mask                                (0x00000001)
#define SP_MS3STUS_dummy1802110c_11_10(data)                          (0x00000C00&(((UINT32)data)<<10))
#define SP_MS3STUS_off_vs_pol_out(data)                               (0x00000200&(((UINT32)data)<<9))
#define SP_MS3STUS_off_hs_pol_out(data)                               (0x00000100&(((UINT32)data)<<8))
#define SP_MS3STUS_off_vs_per_to(data)                                (0x00000080&(((UINT32)data)<<7))
#define SP_MS3STUS_off_vs_high_to(data)                               (0x00000040&(((UINT32)data)<<6))
#define SP_MS3STUS_off_vs_per_of(data)                                (0x00000020&(((UINT32)data)<<5))
#define SP_MS3STUS_off_vs_over_range(data)                            (0x00000010&(((UINT32)data)<<4))
#define SP_MS3STUS_off_vs_pol_chg(data)                               (0x00000008&(((UINT32)data)<<3))
#define SP_MS3STUS_off_hs_per_of(data)                                (0x00000004&(((UINT32)data)<<2))
#define SP_MS3STUS_off_hs_over_range(data)                            (0x00000002&(((UINT32)data)<<1))
#define SP_MS3STUS_off_hs_pol_chg(data)                               (0x00000001&(data))
#define SP_MS3STUS_get_dummy1802110c_11_10(data)                      ((0x00000C00&(data))>>10)
#define SP_MS3STUS_get_off_vs_pol_out(data)                           ((0x00000200&(data))>>9)
#define SP_MS3STUS_get_off_hs_pol_out(data)                           ((0x00000100&(data))>>8)
#define SP_MS3STUS_get_off_vs_per_to(data)                            ((0x00000080&(data))>>7)
#define SP_MS3STUS_get_off_vs_high_to(data)                           ((0x00000040&(data))>>6)
#define SP_MS3STUS_get_off_vs_per_of(data)                            ((0x00000020&(data))>>5)
#define SP_MS3STUS_get_off_vs_over_range(data)                        ((0x00000010&(data))>>4)
#define SP_MS3STUS_get_off_vs_pol_chg(data)                           ((0x00000008&(data))>>3)
#define SP_MS3STUS_get_off_hs_per_of(data)                            ((0x00000004&(data))>>2)
#define SP_MS3STUS_get_off_hs_over_range(data)                        ((0x00000002&(data))>>1)
#define SP_MS3STUS_get_off_hs_pol_chg(data)                           (0x00000001&(data))

#define SP_MS3IE                                                      0x18021110
#define SP_MS3IE_reg_addr                                             "0xB8021110"
#define SP_MS3IE_reg                                                  0xB8021110
#define SP_MS3IE_inst_addr                                            "0x0044"
#define SP_MS3IE_inst                                                 0x0044
#define SP_MS3IE_ie_off_msdone_shift                                  (31)
#define SP_MS3IE_dummy18021110_11_8_shift                             (8)
#define SP_MS3IE_ie_off_vs_per_to_shift                               (7)
#define SP_MS3IE_ie_off_vs_high_to_shift                              (6)
#define SP_MS3IE_ie_off_vs_per_of_shift                               (5)
#define SP_MS3IE_ie_off_vs_over_range_shift                           (4)
#define SP_MS3IE_ie_off_vs_pol_chg_shift                              (3)
#define SP_MS3IE_ie_off_hs_per_of_shift                               (2)
#define SP_MS3IE_ie_off_hs_over_range_shift                           (1)
#define SP_MS3IE_ie_off_hs_pol_chg_shift                              (0)
#define SP_MS3IE_ie_off_msdone_mask                                   (0x80000000)
#define SP_MS3IE_dummy18021110_11_8_mask                              (0x00000F00)
#define SP_MS3IE_ie_off_vs_per_to_mask                                (0x00000080)
#define SP_MS3IE_ie_off_vs_high_to_mask                               (0x00000040)
#define SP_MS3IE_ie_off_vs_per_of_mask                                (0x00000020)
#define SP_MS3IE_ie_off_vs_over_range_mask                            (0x00000010)
#define SP_MS3IE_ie_off_vs_pol_chg_mask                               (0x00000008)
#define SP_MS3IE_ie_off_hs_per_of_mask                                (0x00000004)
#define SP_MS3IE_ie_off_hs_over_range_mask                            (0x00000002)
#define SP_MS3IE_ie_off_hs_pol_chg_mask                               (0x00000001)
#define SP_MS3IE_ie_off_msdone(data)                                  (0x80000000&(((UINT32)data)<<31))
#define SP_MS3IE_dummy18021110_11_8(data)                             (0x00000F00&(((UINT32)data)<<8))
#define SP_MS3IE_ie_off_vs_per_to(data)                               (0x00000080&(((UINT32)data)<<7))
#define SP_MS3IE_ie_off_vs_high_to(data)                              (0x00000040&(((UINT32)data)<<6))
#define SP_MS3IE_ie_off_vs_per_of(data)                               (0x00000020&(((UINT32)data)<<5))
#define SP_MS3IE_ie_off_vs_over_range(data)                           (0x00000010&(((UINT32)data)<<4))
#define SP_MS3IE_ie_off_vs_pol_chg(data)                              (0x00000008&(((UINT32)data)<<3))
#define SP_MS3IE_ie_off_hs_per_of(data)                               (0x00000004&(((UINT32)data)<<2))
#define SP_MS3IE_ie_off_hs_over_range(data)                           (0x00000002&(((UINT32)data)<<1))
#define SP_MS3IE_ie_off_hs_pol_chg(data)                              (0x00000001&(data))
#define SP_MS3IE_get_ie_off_msdone(data)                              ((0x80000000&(data))>>31)
#define SP_MS3IE_get_dummy18021110_11_8(data)                         ((0x00000F00&(data))>>8)
#define SP_MS3IE_get_ie_off_vs_per_to(data)                           ((0x00000080&(data))>>7)
#define SP_MS3IE_get_ie_off_vs_high_to(data)                          ((0x00000040&(data))>>6)
#define SP_MS3IE_get_ie_off_vs_per_of(data)                           ((0x00000020&(data))>>5)
#define SP_MS3IE_get_ie_off_vs_over_range(data)                       ((0x00000010&(data))>>4)
#define SP_MS3IE_get_ie_off_vs_pol_chg(data)                          ((0x00000008&(data))>>3)
#define SP_MS3IE_get_ie_off_hs_per_of(data)                           ((0x00000004&(data))>>2)
#define SP_MS3IE_get_ie_off_hs_over_range(data)                       ((0x00000002&(data))>>1)
#define SP_MS3IE_get_ie_off_hs_pol_chg(data)                          (0x00000001&(data))
#if 0
//Lewis mask, for complier
#define RESERVED                                                      0x18021114
#define RESERVED_reg_addr                                             "0xB8021114"
#define RESERVED_reg                                                  0xB8021114
#define RESERVED_inst_addr                                            "0x0045"
#define RESERVED_inst                                                 0x0045
#endif

#define SP_MS3ENATVCTRL                                               0x18021118
#define SP_MS3ENATVCTRL_reg_addr                                      "0xB8021118"
#define SP_MS3ENATVCTRL_reg                                           0xB8021118
#define SP_MS3ENATVCTRL_inst_addr                                     "0x0046"
#define SP_MS3ENATVCTRL_inst                                          0x0046
#define SP_MS3ENATVCTRL_off_ena_source_shift                          (7)
#define SP_MS3ENATVCTRL_off_ms_act_en_shift                           (6)
#define SP_MS3ENATVCTRL_dummy18021118_5_0_shift                       (0)
#define SP_MS3ENATVCTRL_off_ena_source_mask                           (0x00000080)
#define SP_MS3ENATVCTRL_off_ms_act_en_mask                            (0x00000040)
#define SP_MS3ENATVCTRL_dummy18021118_5_0_mask                        (0x0000003F)
#define SP_MS3ENATVCTRL_off_ena_source(data)                          (0x00000080&(((UINT32)data)<<7))
#define SP_MS3ENATVCTRL_off_ms_act_en(data)                           (0x00000040&(((UINT32)data)<<6))
#define SP_MS3ENATVCTRL_dummy18021118_5_0(data)                       (0x0000003F&(data))
#define SP_MS3ENATVCTRL_get_off_ena_source(data)                      ((0x00000080&(data))>>7)
#define SP_MS3ENATVCTRL_get_off_ms_act_en(data)                       ((0x00000040&(data))>>6)
#define SP_MS3ENATVCTRL_get_dummy18021118_5_0(data)                   (0x0000003F&(data))

#define SP_MS3ENATV                                                   0x1802111c
#define SP_MS3ENATV_reg_addr                                          "0xB802111C"
#define SP_MS3ENATV_reg                                               0xB802111C
#define SP_MS3ENATV_inst_addr                                         "0x0047"
#define SP_MS3ENATV_inst                                              0x0047
#define SP_MS3ENATV_off_ena_region_out_shift                          (0)
#define SP_MS3ENATV_off_ena_region_out_mask                           (0x00FFFFFF)
#define SP_MS3ENATV_off_ena_region_out(data)                          (0x00FFFFFF&(data))
#define SP_MS3ENATV_get_off_ena_region_out(data)                      (0x00FFFFFF&(data))

#define SP_MS1CTRL                                                    0x18021200
#define SP_MS1CTRL_reg_addr                                           "0xB8021200"
#define SP_MS1CTRL_reg                                                0xB8021200
#define SP_MS1CTRL_inst_addr                                          "0x0080"
#define SP_MS1CTRL_inst                                               0x0080
#define SP_MS1CTRL_dummy18021200_19_17_shift                          (17)
#define SP_MS1CTRL_on1_sycms_clk_shift                                (16)
#define SP_MS1CTRL_on1_vs_meas_inv_shift                              (15)
#define SP_MS1CTRL_on1_vsms_inv_shift                                 (14)
#define SP_MS1CTRL_on1_sycms_mode_shift                               (13)
#define SP_MS1CTRL_on1_popup_meas_shift                               (12)
#define SP_MS1CTRL_dummy18021200_11_shift                             (11)
#define SP_MS1CTRL_on1_online_en_shift                                (10)
#define SP_MS1CTRL_on1_start_ms_shift                                 (9)
#define SP_MS1CTRL_on1_hs_max_delta_shift                             (4)
#define SP_MS1CTRL_on1_vs_max_delta_shift                             (0)
#define SP_MS1CTRL_dummy18021200_19_17_mask                           (0x000E0000)
#define SP_MS1CTRL_on1_sycms_clk_mask                                 (0x00010000)
#define SP_MS1CTRL_on1_vs_meas_inv_mask                               (0x00008000)
#define SP_MS1CTRL_on1_vsms_inv_mask                                  (0x00004000)
#define SP_MS1CTRL_on1_sycms_mode_mask                                (0x00002000)
#define SP_MS1CTRL_on1_popup_meas_mask                                (0x00001000)
#define SP_MS1CTRL_dummy18021200_11_mask                              (0x00000800)
#define SP_MS1CTRL_on1_online_en_mask                                 (0x00000400)
#define SP_MS1CTRL_on1_start_ms_mask                                  (0x00000200)
#define SP_MS1CTRL_on1_hs_max_delta_mask                              (0x000001F0)
#define SP_MS1CTRL_on1_vs_max_delta_mask                              (0x0000000F)
#define SP_MS1CTRL_dummy18021200_19_17(data)                          (0x000E0000&(((UINT32)data)<<17))
#define SP_MS1CTRL_on1_sycms_clk(data)                                (0x00010000&(((UINT32)data)<<16))
#define SP_MS1CTRL_on1_vs_meas_inv(data)                              (0x00008000&(((UINT32)data)<<15))
#define SP_MS1CTRL_on1_vsms_inv(data)                                 (0x00004000&(((UINT32)data)<<14))
#define SP_MS1CTRL_on1_sycms_mode(data)                               (0x00002000&(((UINT32)data)<<13))
#define SP_MS1CTRL_on1_popup_meas(data)                               (0x00001000&(((UINT32)data)<<12))
#define SP_MS1CTRL_dummy18021200_11(data)                             (0x00000800&(((UINT32)data)<<11))
#define SP_MS1CTRL_on1_online_en(data)                                (0x00000400&(((UINT32)data)<<10))
#define SP_MS1CTRL_on1_start_ms(data)                                 (0x00000200&(((UINT32)data)<<9))
#define SP_MS1CTRL_on1_hs_max_delta(data)                             (0x000001F0&(((UINT32)data)<<4))
#define SP_MS1CTRL_on1_vs_max_delta(data)                             (0x0000000F&(data))
#define SP_MS1CTRL_get_dummy18021200_19_17(data)                      ((0x000E0000&(data))>>17)
#define SP_MS1CTRL_get_on1_sycms_clk(data)                            ((0x00010000&(data))>>16)
#define SP_MS1CTRL_get_on1_vs_meas_inv(data)                          ((0x00008000&(data))>>15)
#define SP_MS1CTRL_get_on1_vsms_inv(data)                             ((0x00004000&(data))>>14)
#define SP_MS1CTRL_get_on1_sycms_mode(data)                           ((0x00002000&(data))>>13)
#define SP_MS1CTRL_get_on1_popup_meas(data)                           ((0x00001000&(data))>>12)
#define SP_MS1CTRL_get_dummy18021200_11(data)                         ((0x00000800&(data))>>11)
#define SP_MS1CTRL_get_on1_online_en(data)                            ((0x00000400&(data))>>10)
#define SP_MS1CTRL_get_on1_start_ms(data)                             ((0x00000200&(data))>>9)
#define SP_MS1CTRL_get_on1_hs_max_delta(data)                         ((0x000001F0&(data))>>4)
#define SP_MS1CTRL_get_on1_vs_max_delta(data)                         (0x0000000F&(data))

#define SP_MS1RST0                                                    0x18021204
#define SP_MS1RST0_reg_addr                                           "0xB8021204"
#define SP_MS1RST0_reg                                                0xB8021204
#define SP_MS1RST0_inst_addr                                          "0x0081"
#define SP_MS1RST0_inst                                               0x0081
#define SP_MS1RST0_dummy18021204_31_28_shift                          (28)
#define SP_MS1RST0_on1_vs_period_out_shift                            (16)
#define SP_MS1RST0_on1_hs_period_out_h12b_shift                       (4)
#define SP_MS1RST0_on1_hs_period_out_f4b_shift                        (0)
#define SP_MS1RST0_dummy18021204_31_28_mask                           (0xF0000000)
#define SP_MS1RST0_on1_vs_period_out_mask                             (0x0FFF0000)
#define SP_MS1RST0_on1_hs_period_out_h12b_mask                        (0x0000FFF0)
#define SP_MS1RST0_on1_hs_period_out_f4b_mask                         (0x0000000F)
#define SP_MS1RST0_dummy18021204_31_28(data)                          (0xF0000000&(((UINT32)data)<<28))
#define SP_MS1RST0_on1_vs_period_out(data)                            (0x0FFF0000&(((UINT32)data)<<16))
#define SP_MS1RST0_on1_hs_period_out_h12b(data)                       (0x0000FFF0&(((UINT32)data)<<4))
#define SP_MS1RST0_on1_hs_period_out_f4b(data)                        (0x0000000F&(data))
#define SP_MS1RST0_get_dummy18021204_31_28(data)                      ((0xF0000000&(data))>>28)
#define SP_MS1RST0_get_on1_vs_period_out(data)                        ((0x0FFF0000&(data))>>16)
#define SP_MS1RST0_get_on1_hs_period_out_h12b(data)                   ((0x0000FFF0&(data))>>4)
#define SP_MS1RST0_get_on1_hs_period_out_f4b(data)                    (0x0000000F&(data))

#define SP_MS1RST1                                                    0x18021208
#define SP_MS1RST1_reg_addr                                           "0xB8021208"
#define SP_MS1RST1_reg                                                0xB8021208
#define SP_MS1RST1_inst_addr                                          "0x0082"
#define SP_MS1RST1_inst                                               0x0082
#define SP_MS1RST1_dummy18021208_31_28_shift                          (28)
#define SP_MS1RST1_on1_vs_high_out_shift                              (16)
#define SP_MS1RST1_dummy18021208_15_12_shift                          (12)
#define SP_MS1RST1_on1_hs_high_out_shift                              (0)
#define SP_MS1RST1_dummy18021208_31_28_mask                           (0xF0000000)
#define SP_MS1RST1_on1_vs_high_out_mask                               (0x0FFF0000)
#define SP_MS1RST1_dummy18021208_15_12_mask                           (0x0000F000)
#define SP_MS1RST1_on1_hs_high_out_mask                               (0x00000FFF)
#define SP_MS1RST1_dummy18021208_31_28(data)                          (0xF0000000&(((UINT32)data)<<28))
#define SP_MS1RST1_on1_vs_high_out(data)                              (0x0FFF0000&(((UINT32)data)<<16))
#define SP_MS1RST1_dummy18021208_15_12(data)                          (0x0000F000&(((UINT32)data)<<12))
#define SP_MS1RST1_on1_hs_high_out(data)                              (0x00000FFF&(data))
#define SP_MS1RST1_get_dummy18021208_31_28(data)                      ((0xF0000000&(data))>>28)
#define SP_MS1RST1_get_on1_vs_high_out(data)                          ((0x0FFF0000&(data))>>16)
#define SP_MS1RST1_get_dummy18021208_15_12(data)                      ((0x0000F000&(data))>>12)
#define SP_MS1RST1_get_on1_hs_high_out(data)                          (0x00000FFF&(data))

#define SP_MS1STUS                                                    0x1802120c
#define SP_MS1STUS_reg_addr                                           "0xB802120C"
#define SP_MS1STUS_reg                                                0xB802120C
#define SP_MS1STUS_inst_addr                                          "0x0083"
#define SP_MS1STUS_inst                                               0x0083
#define SP_MS1STUS_dummy1802120c_11_10_shift                          (10)
#define SP_MS1STUS_on1_vs_pol_out_shift                               (9)
#define SP_MS1STUS_on1_hs_pol_out_shift                               (8)
#define SP_MS1STUS_on1_vs_per_to_shift                                (7)
#define SP_MS1STUS_on1_vs_high_to_shift                               (6)
#define SP_MS1STUS_on1_vs_per_of_shift                                (5)
#define SP_MS1STUS_on1_vs_over_range_shift                            (4)
#define SP_MS1STUS_on1_vs_pol_chg_shift                               (3)
#define SP_MS1STUS_on1_hs_per_of_shift                                (2)
#define SP_MS1STUS_on1_hs_over_range_shift                            (1)
#define SP_MS1STUS_on1_hs_pol_chg_shift                               (0)
#define SP_MS1STUS_dummy1802120c_11_10_mask                           (0x00000C00)
#define SP_MS1STUS_on1_vs_pol_out_mask                                (0x00000200)
#define SP_MS1STUS_on1_hs_pol_out_mask                                (0x00000100)
#define SP_MS1STUS_on1_vs_per_to_mask                                 (0x00000080)
#define SP_MS1STUS_on1_vs_high_to_mask                                (0x00000040)
#define SP_MS1STUS_on1_vs_per_of_mask                                 (0x00000020)
#define SP_MS1STUS_on1_vs_over_range_mask                             (0x00000010)
#define SP_MS1STUS_on1_vs_pol_chg_mask                                (0x00000008)
#define SP_MS1STUS_on1_hs_per_of_mask                                 (0x00000004)
#define SP_MS1STUS_on1_hs_over_range_mask                             (0x00000002)
#define SP_MS1STUS_on1_hs_pol_chg_mask                                (0x00000001)
#define SP_MS1STUS_dummy1802120c_11_10(data)                          (0x00000C00&(((UINT32)data)<<10))
#define SP_MS1STUS_on1_vs_pol_out(data)                               (0x00000200&(((UINT32)data)<<9))
#define SP_MS1STUS_on1_hs_pol_out(data)                               (0x00000100&(((UINT32)data)<<8))
#define SP_MS1STUS_on1_vs_per_to(data)                                (0x00000080&(((UINT32)data)<<7))
#define SP_MS1STUS_on1_vs_high_to(data)                               (0x00000040&(((UINT32)data)<<6))
#define SP_MS1STUS_on1_vs_per_of(data)                                (0x00000020&(((UINT32)data)<<5))
#define SP_MS1STUS_on1_vs_over_range(data)                            (0x00000010&(((UINT32)data)<<4))
#define SP_MS1STUS_on1_vs_pol_chg(data)                               (0x00000008&(((UINT32)data)<<3))
#define SP_MS1STUS_on1_hs_per_of(data)                                (0x00000004&(((UINT32)data)<<2))
#define SP_MS1STUS_on1_hs_over_range(data)                            (0x00000002&(((UINT32)data)<<1))
#define SP_MS1STUS_on1_hs_pol_chg(data)                               (0x00000001&(data))
#define SP_MS1STUS_get_dummy1802120c_11_10(data)                      ((0x00000C00&(data))>>10)
#define SP_MS1STUS_get_on1_vs_pol_out(data)                           ((0x00000200&(data))>>9)
#define SP_MS1STUS_get_on1_hs_pol_out(data)                           ((0x00000100&(data))>>8)
#define SP_MS1STUS_get_on1_vs_per_to(data)                            ((0x00000080&(data))>>7)
#define SP_MS1STUS_get_on1_vs_high_to(data)                           ((0x00000040&(data))>>6)
#define SP_MS1STUS_get_on1_vs_per_of(data)                            ((0x00000020&(data))>>5)
#define SP_MS1STUS_get_on1_vs_over_range(data)                        ((0x00000010&(data))>>4)
#define SP_MS1STUS_get_on1_vs_pol_chg(data)                           ((0x00000008&(data))>>3)
#define SP_MS1STUS_get_on1_hs_per_of(data)                            ((0x00000004&(data))>>2)
#define SP_MS1STUS_get_on1_hs_over_range(data)                        ((0x00000002&(data))>>1)
#define SP_MS1STUS_get_on1_hs_pol_chg(data)                           (0x00000001&(data))

#define SP_MS1IE                                                      0x18021210
#define SP_MS1IE_reg_addr                                             "0xB8021210"
#define SP_MS1IE_reg                                                  0xB8021210
#define SP_MS1IE_inst_addr                                            "0x0084"
#define SP_MS1IE_inst                                                 0x0084
#define SP_MS1IE_wd_on1_to_main_shift                                 (31)
#define SP_MS1IE_wd_on1_to_sub_shift                                  (30)
#define SP_MS1IE_wde_on1_vs_per_to_shift                              (23)
#define SP_MS1IE_wde_on1_vs_high_to_shift                             (22)
#define SP_MS1IE_wde_on1_vs_per_of_shift                              (21)
#define SP_MS1IE_wde_on1_vs_over_range_shift                          (20)
#define SP_MS1IE_wde_on1_vs_pol_chg_shift                             (19)
#define SP_MS1IE_wde_on1_hs_per_of_shift                              (18)
#define SP_MS1IE_wde_on1_hs_over_range_shift                          (17)
#define SP_MS1IE_wde_on1_hs_pol_chg_shift                             (16)
#define SP_MS1IE_dummy18021210_15_8_shift                             (8)
#define SP_MS1IE_on1_ie_vs_per_to_shift                               (7)
#define SP_MS1IE_on1_ie_vs_high_to_shift                              (6)
#define SP_MS1IE_on1_ie_vs_per_of_shift                               (5)
#define SP_MS1IE_on1_ie_vs_over_range_shift                           (4)
#define SP_MS1IE_on1_ie_vs_pol_chg_shift                              (3)
#define SP_MS1IE_on1_ie_hs_per_of_shift                               (2)
#define SP_MS1IE_on1_ie_hs_over_range_shift                           (1)
#define SP_MS1IE_on1_ie_hs_pol_chg_shift                              (0)
#define SP_MS1IE_wd_on1_to_main_mask                                  (0x80000000)
#define SP_MS1IE_wd_on1_to_sub_mask                                   (0x40000000)
#define SP_MS1IE_wde_on1_vs_per_to_mask                               (0x00800000)
#define SP_MS1IE_wde_on1_vs_high_to_mask                              (0x00400000)
#define SP_MS1IE_wde_on1_vs_per_of_mask                               (0x00200000)
#define SP_MS1IE_wde_on1_vs_over_range_mask                           (0x00100000)
#define SP_MS1IE_wde_on1_vs_pol_chg_mask                              (0x00080000)
#define SP_MS1IE_wde_on1_hs_per_of_mask                               (0x00040000)
#define SP_MS1IE_wde_on1_hs_over_range_mask                           (0x00020000)
#define SP_MS1IE_wde_on1_hs_pol_chg_mask                              (0x00010000)
#define SP_MS1IE_dummy18021210_15_8_mask                              (0x0000FF00)
#define SP_MS1IE_on1_ie_vs_per_to_mask                                (0x00000080)
#define SP_MS1IE_on1_ie_vs_high_to_mask                               (0x00000040)
#define SP_MS1IE_on1_ie_vs_per_of_mask                                (0x00000020)
#define SP_MS1IE_on1_ie_vs_over_range_mask                            (0x00000010)
#define SP_MS1IE_on1_ie_vs_pol_chg_mask                               (0x00000008)
#define SP_MS1IE_on1_ie_hs_per_of_mask                                (0x00000004)
#define SP_MS1IE_on1_ie_hs_over_range_mask                            (0x00000002)
#define SP_MS1IE_on1_ie_hs_pol_chg_mask                               (0x00000001)
#define SP_MS1IE_wd_on1_to_main(data)                                 (0x80000000&(((UINT32)data)<<31))
#define SP_MS1IE_wd_on1_to_sub(data)                                  (0x40000000&(((UINT32)data)<<30))
#define SP_MS1IE_wde_on1_vs_per_to(data)                              (0x00800000&(((UINT32)data)<<23))
#define SP_MS1IE_wde_on1_vs_high_to(data)                             (0x00400000&(((UINT32)data)<<22))
#define SP_MS1IE_wde_on1_vs_per_of(data)                              (0x00200000&(((UINT32)data)<<21))
#define SP_MS1IE_wde_on1_vs_over_range(data)                          (0x00100000&(((UINT32)data)<<20))
#define SP_MS1IE_wde_on1_vs_pol_chg(data)                             (0x00080000&(((UINT32)data)<<19))
#define SP_MS1IE_wde_on1_hs_per_of(data)                              (0x00040000&(((UINT32)data)<<18))
#define SP_MS1IE_wde_on1_hs_over_range(data)                          (0x00020000&(((UINT32)data)<<17))
#define SP_MS1IE_wde_on1_hs_pol_chg(data)                             (0x00010000&(((UINT32)data)<<16))
#define SP_MS1IE_dummy18021210_15_8(data)                             (0x0000FF00&(((UINT32)data)<<8))
#define SP_MS1IE_on1_ie_vs_per_to(data)                               (0x00000080&(((UINT32)data)<<7))
#define SP_MS1IE_on1_ie_vs_high_to(data)                              (0x00000040&(((UINT32)data)<<6))
#define SP_MS1IE_on1_ie_vs_per_of(data)                               (0x00000020&(((UINT32)data)<<5))
#define SP_MS1IE_on1_ie_vs_over_range(data)                           (0x00000010&(((UINT32)data)<<4))
#define SP_MS1IE_on1_ie_vs_pol_chg(data)                              (0x00000008&(((UINT32)data)<<3))
#define SP_MS1IE_on1_ie_hs_per_of(data)                               (0x00000004&(((UINT32)data)<<2))
#define SP_MS1IE_on1_ie_hs_over_range(data)                           (0x00000002&(((UINT32)data)<<1))
#define SP_MS1IE_on1_ie_hs_pol_chg(data)                              (0x00000001&(data))
#define SP_MS1IE_get_wd_on1_to_main(data)                             ((0x80000000&(data))>>31)
#define SP_MS1IE_get_wd_on1_to_sub(data)                              ((0x40000000&(data))>>30)
#define SP_MS1IE_get_wde_on1_vs_per_to(data)                          ((0x00800000&(data))>>23)
#define SP_MS1IE_get_wde_on1_vs_high_to(data)                         ((0x00400000&(data))>>22)
#define SP_MS1IE_get_wde_on1_vs_per_of(data)                          ((0x00200000&(data))>>21)
#define SP_MS1IE_get_wde_on1_vs_over_range(data)                      ((0x00100000&(data))>>20)
#define SP_MS1IE_get_wde_on1_vs_pol_chg(data)                         ((0x00080000&(data))>>19)
#define SP_MS1IE_get_wde_on1_hs_per_of(data)                          ((0x00040000&(data))>>18)
#define SP_MS1IE_get_wde_on1_hs_over_range(data)                      ((0x00020000&(data))>>17)
#define SP_MS1IE_get_wde_on1_hs_pol_chg(data)                         ((0x00010000&(data))>>16)
#define SP_MS1IE_get_dummy18021210_15_8(data)                         ((0x0000FF00&(data))>>8)
#define SP_MS1IE_get_on1_ie_vs_per_to(data)                           ((0x00000080&(data))>>7)
#define SP_MS1IE_get_on1_ie_vs_high_to(data)                          ((0x00000040&(data))>>6)
#define SP_MS1IE_get_on1_ie_vs_per_of(data)                           ((0x00000020&(data))>>5)
#define SP_MS1IE_get_on1_ie_vs_over_range(data)                       ((0x00000010&(data))>>4)
#define SP_MS1IE_get_on1_ie_vs_pol_chg(data)                          ((0x00000008&(data))>>3)
#define SP_MS1IE_get_on1_ie_hs_per_of(data)                           ((0x00000004&(data))>>2)
#define SP_MS1IE_get_on1_ie_hs_over_range(data)                       ((0x00000002&(data))>>1)
#define SP_MS1IE_get_on1_ie_hs_pol_chg(data)                          (0x00000001&(data))

#define SP_MS2CTRL                                                    0x18021214
#define SP_MS2CTRL_reg_addr                                           "0xB8021214"
#define SP_MS2CTRL_reg                                                0xB8021214
#define SP_MS2CTRL_inst_addr                                          "0x0085"
#define SP_MS2CTRL_inst                                               0x0085
#define SP_MS2CTRL_dummy18021214_19_17_shift                          (17)
#define SP_MS2CTRL_on2_sycms_clk_shift                                (16)
#define SP_MS2CTRL_on2_vs_meas_inv_shift                              (15)
#define SP_MS2CTRL_on2_vsms_inv_shift                                 (14)
#define SP_MS2CTRL_on2_sycms_mode_shift                               (13)
#define SP_MS2CTRL_on2_popup_meas_shift                               (12)
#define SP_MS2CTRL_dummy18021214_11_shift                             (11)
#define SP_MS2CTRL_on2_online_en_shift                                (10)
#define SP_MS2CTRL_on2_start_ms_shift                                 (9)
#define SP_MS2CTRL_on2_hs_max_delta_shift                             (4)
#define SP_MS2CTRL_on2_vs_max_delta_shift                             (0)
#define SP_MS2CTRL_dummy18021214_19_17_mask                           (0x000E0000)
#define SP_MS2CTRL_on2_sycms_clk_mask                                 (0x00010000)
#define SP_MS2CTRL_on2_vs_meas_inv_mask                               (0x00008000)
#define SP_MS2CTRL_on2_vsms_inv_mask                                  (0x00004000)
#define SP_MS2CTRL_on2_sycms_mode_mask                                (0x00002000)
#define SP_MS2CTRL_on2_popup_meas_mask                                (0x00001000)
#define SP_MS2CTRL_dummy18021214_11_mask                              (0x00000800)
#define SP_MS2CTRL_on2_online_en_mask                                 (0x00000400)
#define SP_MS2CTRL_on2_start_ms_mask                                  (0x00000200)
#define SP_MS2CTRL_on2_hs_max_delta_mask                              (0x000001F0)
#define SP_MS2CTRL_on2_vs_max_delta_mask                              (0x0000000F)
#define SP_MS2CTRL_dummy18021214_19_17(data)                          (0x000E0000&(((UINT32)data)<<17))
#define SP_MS2CTRL_on2_sycms_clk(data)                                (0x00010000&(((UINT32)data)<<16))
#define SP_MS2CTRL_on2_vs_meas_inv(data)                              (0x00008000&(((UINT32)data)<<15))
#define SP_MS2CTRL_on2_vsms_inv(data)                                 (0x00004000&(((UINT32)data)<<14))
#define SP_MS2CTRL_on2_sycms_mode(data)                               (0x00002000&(((UINT32)data)<<13))
#define SP_MS2CTRL_on2_popup_meas(data)                               (0x00001000&(((UINT32)data)<<12))
#define SP_MS2CTRL_dummy18021214_11(data)                             (0x00000800&(((UINT32)data)<<11))
#define SP_MS2CTRL_on2_online_en(data)                                (0x00000400&(((UINT32)data)<<10))
#define SP_MS2CTRL_on2_start_ms(data)                                 (0x00000200&(((UINT32)data)<<9))
#define SP_MS2CTRL_on2_hs_max_delta(data)                             (0x000001F0&(((UINT32)data)<<4))
#define SP_MS2CTRL_on2_vs_max_delta(data)                             (0x0000000F&(data))
#define SP_MS2CTRL_get_dummy18021214_19_17(data)                      ((0x000E0000&(data))>>17)
#define SP_MS2CTRL_get_on2_sycms_clk(data)                            ((0x00010000&(data))>>16)
#define SP_MS2CTRL_get_on2_vs_meas_inv(data)                          ((0x00008000&(data))>>15)
#define SP_MS2CTRL_get_on2_vsms_inv(data)                             ((0x00004000&(data))>>14)
#define SP_MS2CTRL_get_on2_sycms_mode(data)                           ((0x00002000&(data))>>13)
#define SP_MS2CTRL_get_on2_popup_meas(data)                           ((0x00001000&(data))>>12)
#define SP_MS2CTRL_get_dummy18021214_11(data)                         ((0x00000800&(data))>>11)
#define SP_MS2CTRL_get_on2_online_en(data)                            ((0x00000400&(data))>>10)
#define SP_MS2CTRL_get_on2_start_ms(data)                             ((0x00000200&(data))>>9)
#define SP_MS2CTRL_get_on2_hs_max_delta(data)                         ((0x000001F0&(data))>>4)
#define SP_MS2CTRL_get_on2_vs_max_delta(data)                         (0x0000000F&(data))

#define SP_MS2RST0                                                    0x18021218
#define SP_MS2RST0_reg_addr                                           "0xB8021218"
#define SP_MS2RST0_reg                                                0xB8021218
#define SP_MS2RST0_inst_addr                                          "0x0086"
#define SP_MS2RST0_inst                                               0x0086
#define SP_MS2RST0_dummy18021218_31_28_shift                          (28)
#define SP_MS2RST0_on2_vs_period_out_shift                            (16)
#define SP_MS2RST0_on2_hs_period_out_h12b_shift                       (4)
#define SP_MS2RST0_on2_hs_period_out_f4b_shift                        (0)
#define SP_MS2RST0_dummy18021218_31_28_mask                           (0xF0000000)
#define SP_MS2RST0_on2_vs_period_out_mask                             (0x0FFF0000)
#define SP_MS2RST0_on2_hs_period_out_h12b_mask                        (0x0000FFF0)
#define SP_MS2RST0_on2_hs_period_out_f4b_mask                         (0x0000000F)
#define SP_MS2RST0_dummy18021218_31_28(data)                          (0xF0000000&(((UINT32)data)<<28))
#define SP_MS2RST0_on2_vs_period_out(data)                            (0x0FFF0000&(((UINT32)data)<<16))
#define SP_MS2RST0_on2_hs_period_out_h12b(data)                       (0x0000FFF0&(((UINT32)data)<<4))
#define SP_MS2RST0_on2_hs_period_out_f4b(data)                        (0x0000000F&(data))
#define SP_MS2RST0_get_dummy18021218_31_28(data)                      ((0xF0000000&(data))>>28)
#define SP_MS2RST0_get_on2_vs_period_out(data)                        ((0x0FFF0000&(data))>>16)
#define SP_MS2RST0_get_on2_hs_period_out_h12b(data)                   ((0x0000FFF0&(data))>>4)
#define SP_MS2RST0_get_on2_hs_period_out_f4b(data)                    (0x0000000F&(data))

#define SP_MS2RST1                                                    0x1802121c
#define SP_MS2RST1_reg_addr                                           "0xB802121C"
#define SP_MS2RST1_reg                                                0xB802121C
#define SP_MS2RST1_inst_addr                                          "0x0087"
#define SP_MS2RST1_inst                                               0x0087
#define SP_MS2RST1_dummy1802121c_31_28_shift                          (28)
#define SP_MS2RST1_on2_vs_high_out_shift                              (16)
#define SP_MS2RST1_dummy1802121c_15_12_shift                          (12)
#define SP_MS2RST1_on2_hs_high_out_shift                              (0)
#define SP_MS2RST1_dummy1802121c_31_28_mask                           (0xF0000000)
#define SP_MS2RST1_on2_vs_high_out_mask                               (0x0FFF0000)
#define SP_MS2RST1_dummy1802121c_15_12_mask                           (0x0000F000)
#define SP_MS2RST1_on2_hs_high_out_mask                               (0x00000FFF)
#define SP_MS2RST1_dummy1802121c_31_28(data)                          (0xF0000000&(((UINT32)data)<<28))
#define SP_MS2RST1_on2_vs_high_out(data)                              (0x0FFF0000&(((UINT32)data)<<16))
#define SP_MS2RST1_dummy1802121c_15_12(data)                          (0x0000F000&(((UINT32)data)<<12))
#define SP_MS2RST1_on2_hs_high_out(data)                              (0x00000FFF&(data))
#define SP_MS2RST1_get_dummy1802121c_31_28(data)                      ((0xF0000000&(data))>>28)
#define SP_MS2RST1_get_on2_vs_high_out(data)                          ((0x0FFF0000&(data))>>16)
#define SP_MS2RST1_get_dummy1802121c_15_12(data)                      ((0x0000F000&(data))>>12)
#define SP_MS2RST1_get_on2_hs_high_out(data)                          (0x00000FFF&(data))

#define SP_MS2STUS                                                    0x18021220
#define SP_MS2STUS_reg_addr                                           "0xB8021220"
#define SP_MS2STUS_reg                                                0xB8021220
#define SP_MS2STUS_inst_addr                                          "0x0088"
#define SP_MS2STUS_inst                                               0x0088
#define SP_MS2STUS_on2_vs_pol_out_shift                               (9)
#define SP_MS2STUS_on2_hs_pol_out_shift                               (8)
#define SP_MS2STUS_on2_vs_per_to_shift                                (7)
#define SP_MS2STUS_on2_vs_high_to_shift                               (6)
#define SP_MS2STUS_on2_vs_per_of_shift                                (5)
#define SP_MS2STUS_on2_vs_over_range_shift                            (4)
#define SP_MS2STUS_on2_vs_pol_chg_shift                               (3)
#define SP_MS2STUS_on2_hs_per_of_shift                                (2)
#define SP_MS2STUS_on2_hs_over_range_shift                            (1)
#define SP_MS2STUS_on2_hs_pol_chg_shift                               (0)
#define SP_MS2STUS_on2_vs_pol_out_mask                                (0x00000200)
#define SP_MS2STUS_on2_hs_pol_out_mask                                (0x00000100)
#define SP_MS2STUS_on2_vs_per_to_mask                                 (0x00000080)
#define SP_MS2STUS_on2_vs_high_to_mask                                (0x00000040)
#define SP_MS2STUS_on2_vs_per_of_mask                                 (0x00000020)
#define SP_MS2STUS_on2_vs_over_range_mask                             (0x00000010)
#define SP_MS2STUS_on2_vs_pol_chg_mask                                (0x00000008)
#define SP_MS2STUS_on2_hs_per_of_mask                                 (0x00000004)
#define SP_MS2STUS_on2_hs_over_range_mask                             (0x00000002)
#define SP_MS2STUS_on2_hs_pol_chg_mask                                (0x00000001)
#define SP_MS2STUS_on2_vs_pol_out(data)                               (0x00000200&(((UINT32)data)<<9))
#define SP_MS2STUS_on2_hs_pol_out(data)                               (0x00000100&(((UINT32)data)<<8))
#define SP_MS2STUS_on2_vs_per_to(data)                                (0x00000080&(((UINT32)data)<<7))
#define SP_MS2STUS_on2_vs_high_to(data)                               (0x00000040&(((UINT32)data)<<6))
#define SP_MS2STUS_on2_vs_per_of(data)                                (0x00000020&(((UINT32)data)<<5))
#define SP_MS2STUS_on2_vs_over_range(data)                            (0x00000010&(((UINT32)data)<<4))
#define SP_MS2STUS_on2_vs_pol_chg(data)                               (0x00000008&(((UINT32)data)<<3))
#define SP_MS2STUS_on2_hs_per_of(data)                                (0x00000004&(((UINT32)data)<<2))
#define SP_MS2STUS_on2_hs_over_range(data)                            (0x00000002&(((UINT32)data)<<1))
#define SP_MS2STUS_on2_hs_pol_chg(data)                               (0x00000001&(data))
#define SP_MS2STUS_get_on2_vs_pol_out(data)                           ((0x00000200&(data))>>9)
#define SP_MS2STUS_get_on2_hs_pol_out(data)                           ((0x00000100&(data))>>8)
#define SP_MS2STUS_get_on2_vs_per_to(data)                            ((0x00000080&(data))>>7)
#define SP_MS2STUS_get_on2_vs_high_to(data)                           ((0x00000040&(data))>>6)
#define SP_MS2STUS_get_on2_vs_per_of(data)                            ((0x00000020&(data))>>5)
#define SP_MS2STUS_get_on2_vs_over_range(data)                        ((0x00000010&(data))>>4)
#define SP_MS2STUS_get_on2_vs_pol_chg(data)                           ((0x00000008&(data))>>3)
#define SP_MS2STUS_get_on2_hs_per_of(data)                            ((0x00000004&(data))>>2)
#define SP_MS2STUS_get_on2_hs_over_range(data)                        ((0x00000002&(data))>>1)
#define SP_MS2STUS_get_on2_hs_pol_chg(data)                           (0x00000001&(data))

#define SP_MS2IE                                                      0x18021224
#define SP_MS2IE_reg_addr                                             "0xB8021224"
#define SP_MS2IE_reg                                                  0xB8021224
#define SP_MS2IE_inst_addr                                            "0x0089"
#define SP_MS2IE_inst                                                 0x0089
#define SP_MS2IE_wd_on2_to_main_shift                                 (31)
#define SP_MS2IE_wd_on2_to_sub_shift                                  (30)
#define SP_MS2IE_dummy18021224_29_24_shift                            (24)
#define SP_MS2IE_wde_on2_vs_per_to_shift                              (23)
#define SP_MS2IE_wde_on2_vs_high_to_shift                             (22)
#define SP_MS2IE_wde_on2_vs_per_of_shift                              (21)
#define SP_MS2IE_wde_on2_vs_over_range_shift                          (20)
#define SP_MS2IE_wde_on2_vs_pol_chg_shift                             (19)
#define SP_MS2IE_wde_on2_hs_per_of_shift                              (18)
#define SP_MS2IE_wde_on2_hs_over_range_shift                          (17)
#define SP_MS2IE_wde_on2_hs_pol_chg_shift                             (16)
#define SP_MS2IE_dummy18021224_15_8_shift                             (8)
#define SP_MS2IE_on2_ie_vs_high_to_shift                              (6)
#define SP_MS2IE_on2_ie_vs_per_of_shift                               (5)
#define SP_MS2IE_on2_ie_vs_over_range_shift                           (4)
#define SP_MS2IE_on2_ie_vs_pol_chg_shift                              (3)
#define SP_MS2IE_on2_ie_hs_per_of_shift                               (2)
#define SP_MS2IE_on2_ie_hs_over_range_shift                           (1)
#define SP_MS2IE_on2_ie_hs_pol_chg_shift                              (0)
#define SP_MS2IE_wd_on2_to_main_mask                                  (0x80000000)
#define SP_MS2IE_wd_on2_to_sub_mask                                   (0x40000000)
#define SP_MS2IE_dummy18021224_29_24_mask                             (0x3F000000)
#define SP_MS2IE_wde_on2_vs_per_to_mask                               (0x00800000)
#define SP_MS2IE_wde_on2_vs_high_to_mask                              (0x00400000)
#define SP_MS2IE_wde_on2_vs_per_of_mask                               (0x00200000)
#define SP_MS2IE_wde_on2_vs_over_range_mask                           (0x00100000)
#define SP_MS2IE_wde_on2_vs_pol_chg_mask                              (0x00080000)
#define SP_MS2IE_wde_on2_hs_per_of_mask                               (0x00040000)
#define SP_MS2IE_wde_on2_hs_over_range_mask                           (0x00020000)
#define SP_MS2IE_wde_on2_hs_pol_chg_mask                              (0x00010000)
#define SP_MS2IE_dummy18021224_15_8_mask                              (0x0000FF00)
#define SP_MS2IE_on2_ie_vs_high_to_mask                               (0x00000040)
#define SP_MS2IE_on2_ie_vs_per_of_mask                                (0x00000020)
#define SP_MS2IE_on2_ie_vs_over_range_mask                            (0x00000010)
#define SP_MS2IE_on2_ie_vs_pol_chg_mask                               (0x00000008)
#define SP_MS2IE_on2_ie_hs_per_of_mask                                (0x00000004)
#define SP_MS2IE_on2_ie_hs_over_range_mask                            (0x00000002)
#define SP_MS2IE_on2_ie_hs_pol_chg_mask                               (0x00000001)
#define SP_MS2IE_wd_on2_to_main(data)                                 (0x80000000&(((UINT32)data)<<31))
#define SP_MS2IE_wd_on2_to_sub(data)                                  (0x40000000&(((UINT32)data)<<30))
#define SP_MS2IE_dummy18021224_29_24(data)                            (0x3F000000&(((UINT32)data)<<24))
#define SP_MS2IE_wde_on2_vs_per_to(data)                              (0x00800000&(((UINT32)data)<<23))
#define SP_MS2IE_wde_on2_vs_high_to(data)                             (0x00400000&(((UINT32)data)<<22))
#define SP_MS2IE_wde_on2_vs_per_of(data)                              (0x00200000&(((UINT32)data)<<21))
#define SP_MS2IE_wde_on2_vs_over_range(data)                          (0x00100000&(((UINT32)data)<<20))
#define SP_MS2IE_wde_on2_vs_pol_chg(data)                             (0x00080000&(((UINT32)data)<<19))
#define SP_MS2IE_wde_on2_hs_per_of(data)                              (0x00040000&(((UINT32)data)<<18))
#define SP_MS2IE_wde_on2_hs_over_range(data)                          (0x00020000&(((UINT32)data)<<17))
#define SP_MS2IE_wde_on2_hs_pol_chg(data)                             (0x00010000&(((UINT32)data)<<16))
#define SP_MS2IE_dummy18021224_15_8(data)                             (0x0000FF00&(((UINT32)data)<<8))
#define SP_MS2IE_on2_ie_vs_high_to(data)                              (0x00000040&(((UINT32)data)<<6))
#define SP_MS2IE_on2_ie_vs_per_of(data)                               (0x00000020&(((UINT32)data)<<5))
#define SP_MS2IE_on2_ie_vs_over_range(data)                           (0x00000010&(((UINT32)data)<<4))
#define SP_MS2IE_on2_ie_vs_pol_chg(data)                              (0x00000008&(((UINT32)data)<<3))
#define SP_MS2IE_on2_ie_hs_per_of(data)                               (0x00000004&(((UINT32)data)<<2))
#define SP_MS2IE_on2_ie_hs_over_range(data)                           (0x00000002&(((UINT32)data)<<1))
#define SP_MS2IE_on2_ie_hs_pol_chg(data)                              (0x00000001&(data))
#define SP_MS2IE_get_wd_on2_to_main(data)                             ((0x80000000&(data))>>31)
#define SP_MS2IE_get_wd_on2_to_sub(data)                              ((0x40000000&(data))>>30)
#define SP_MS2IE_get_dummy18021224_29_24(data)                        ((0x3F000000&(data))>>24)
#define SP_MS2IE_get_wde_on2_vs_per_to(data)                          ((0x00800000&(data))>>23)
#define SP_MS2IE_get_wde_on2_vs_high_to(data)                         ((0x00400000&(data))>>22)
#define SP_MS2IE_get_wde_on2_vs_per_of(data)                          ((0x00200000&(data))>>21)
#define SP_MS2IE_get_wde_on2_vs_over_range(data)                      ((0x00100000&(data))>>20)
#define SP_MS2IE_get_wde_on2_vs_pol_chg(data)                         ((0x00080000&(data))>>19)
#define SP_MS2IE_get_wde_on2_hs_per_of(data)                          ((0x00040000&(data))>>18)
#define SP_MS2IE_get_wde_on2_hs_over_range(data)                      ((0x00020000&(data))>>17)
#define SP_MS2IE_get_wde_on2_hs_pol_chg(data)                         ((0x00010000&(data))>>16)
#define SP_MS2IE_get_dummy18021224_15_8(data)                         ((0x0000FF00&(data))>>8)
#define SP_MS2IE_get_on2_ie_vs_high_to(data)                          ((0x00000040&(data))>>6)
#define SP_MS2IE_get_on2_ie_vs_per_of(data)                           ((0x00000020&(data))>>5)
#define SP_MS2IE_get_on2_ie_vs_over_range(data)                       ((0x00000010&(data))>>4)
#define SP_MS2IE_get_on2_ie_vs_pol_chg(data)                          ((0x00000008&(data))>>3)
#define SP_MS2IE_get_on2_ie_hs_per_of(data)                           ((0x00000004&(data))>>2)
#define SP_MS2IE_get_on2_ie_hs_over_range(data)                       ((0x00000002&(data))>>1)
#define SP_MS2IE_get_on2_ie_hs_pol_chg(data)                          (0x00000001&(data))
#endif
