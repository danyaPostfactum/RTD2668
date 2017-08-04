// This file is generated using the spec version 1.39, firmware template version 1.39and SRIF Parser                                                                                source code SVN rev:743                    Version flow no.:1.0.197
#ifndef _VGIP_REG_H_INCLUDED_
#define _VGIP_REG_H_INCLUDED_

//#define  _VGIP_USE_STRUCT
#ifdef _VGIP_USE_STRUCT

typedef struct
{
    unsigned int    reserved_0:19;
    unsigned int    hdmi_ph_in:6;
    unsigned int    vclk_dly:6;
    unsigned int    vclk_inv:1;
} REG18022200;

typedef struct
{
    unsigned int    v10_1biterr:1;
    unsigned int    v10_2biterr:1;
    unsigned int    v10_1biterr_clr:1;
    unsigned int    v10_2biterr_clr:1;
    unsigned int    v10_1biterr_ie:1;
    unsigned int    v10_2biterr_ie:1;
    unsigned int    reserved_0:17;
    unsigned int    video_sel:1;
    unsigned int    dummy18022204_7_5:3;
    unsigned int    v10_bit_swap:1;
    unsigned int    dummy18022204_3_2:2;
    unsigned int    v10_en_cor:1;
    unsigned int    v10_en_dec:1;
} REG18022204;

typedef struct
{
    unsigned int    dummy18022208_31_0:32;
} REG18022208;

typedef struct
{
    unsigned int    dummy1802220c_31_0:32;
} REG1802220C;

typedef struct
{
    unsigned int    ch1_ivrun:1;
    unsigned int    ch1_in_sel:3;
    unsigned int    ch1_random_en:1;
    unsigned int    ch1_fdrop_en:1;
    unsigned int    ch1_vact_end_ie:1;
    unsigned int    ch1_vact_start_ie:1;
    unsigned int    ch1_field_hs_inv:1;
    unsigned int    ch1_odd_vd_inv:1;
    unsigned int    ch1_odd_vd_en:1;
    unsigned int    reserved_0:1;
    unsigned int    reserved_1:1;
    unsigned int    ch1_odd_inv:1;
    unsigned int    ch1_odd_sync_edge:1;
    unsigned int    ch1_odd_det_en:1;
    unsigned int    ch1_hpact_is_hact:1;
    unsigned int    ch1_fcap_str_odd:1;
    unsigned int    ch1_mask_den_inv:1;
    unsigned int    ch1_mask_den_1l:1;
    unsigned int    dummy18022210_11:1;
    unsigned int    ch1_safe_odd_inv:1;
    unsigned int    ch1_force_tog:1;
    unsigned int    ch1_safe_mode:1;
    unsigned int    dummy18022210_7:1;
    unsigned int    ch1_hs_syncedge:1;
    unsigned int    ch1_vs_syncedge:1;
    unsigned int    ch1_vs_by_hs_en_n:1;
    unsigned int    ch1_vs_inv:1;
    unsigned int    ch1_hs_inv:1;
    unsigned int    ch1_digital_mode:1;
    unsigned int    ch1_in_clk_en:1;
} VGIP_chn1_ctrl;

typedef struct
{
    unsigned int    reserved_0:4;
    unsigned int    ch1_vs_end:1;
    unsigned int    ch1_vs_start:1;
    unsigned int    ch1_vact_end:1;
    unsigned int    ch1_vact_start:1;
    unsigned int    dummy18022214_23_15:9;
    unsigned int    ch1_vs_err:1;
    unsigned int    ch1_hs_err:1;
    unsigned int    ch1_field_err:1;
    unsigned int    dummy18022214_11_10:2;
    unsigned int    ch1_vs_tog:1;
    unsigned int    ch1_hs_tog:1;
    unsigned int    ch1_field_pol:1;
    unsigned int    ch1_field_tog:1;
    unsigned int    ch1_field_vs_lsb:2;
    unsigned int    ch1_more_1line_field:1;
    unsigned int    ch1_bounce_status_2:1;
    unsigned int    ch1_bounce_status_1:1;
    unsigned int    ch1_bounce_status_0:1;
} VGIP_chn1_status;

typedef struct
{
    unsigned int    ch1_db_rdy:1;
    unsigned int    ch1_db_en:1;
    unsigned int    reserved_0:2;
    unsigned int    ch1_ih_act_sta:12;
    unsigned int    reserved_1:4;
    unsigned int    ch1_ih_act_wid:12;
} VGIP_chn1_act_hsta_width;

typedef struct
{
    unsigned int    reserved_0:4;
    unsigned int    ch1_iv_act_sta:12;
    unsigned int    reserved_1:4;
    unsigned int    ch1_iv_act_len:12;
} VGIP_chn1_act_vsta_length;

typedef struct
{
    unsigned int    reserved_0:4;
    unsigned int    ch1_ihs_dly:12;
    unsigned int    reserved_1:4;
    unsigned int    ch1_ivs_dly:12;
} VGIP_chn1_delay;

typedef struct
{
    unsigned int    ch1_de_mask_en:1;
    unsigned int    reserved_0:12;
    unsigned int    ch1_hporch_num:11;
    unsigned int    ch1_auto_ihs_dly:2;
    unsigned int    ch1_auto_ivs_dly:2;
    unsigned int    ch1_hdly_one:1;
    unsigned int    reserved_1:3;
} VGIP_chn1_misc;

typedef struct
{
    unsigned int    ch1_captst_en:1;
    unsigned int    ch1_captst_mode:1;
    unsigned int    reserved_0:2;
    unsigned int    ch1_vcaptst_num:12;
    unsigned int    reserved_1:4;
    unsigned int    ch1_hcaptst_num:12;
} VGIP_chn1_ptn_h_vi;

typedef struct
{
    unsigned int    reserved_0:7;
    unsigned int    ch1_captst_r_data:9;
    unsigned int    reserved_1:7;
    unsigned int    ch1_captst_g_data:9;
} VGIP_chn1_ptn0;

typedef struct
{
    unsigned int    reserved_0:23;
    unsigned int    ch1_captst_b_data:9;
} VGIP_chn1_ptn1;

typedef struct
{
    unsigned int    ch1_vlcen:1;
    unsigned int    ch1_vlc_mode:1;
    unsigned int    ch1_vlc_ie:1;
    unsigned int    dummy18022234_28:1;
    unsigned int    ch1_vlc_status:1;
    unsigned int    reserved_0:11;
    unsigned int    dummy18022234_15_11:5;
    unsigned int    ch1_vln:11;
} VGIP_chn1_lc;

typedef struct
{
    unsigned int    reserved_0:29;
    unsigned int    reserved_1:1;
    unsigned int    ch1_crc_conti:1;
    unsigned int    ch1_crc_start:1;
} ICH1_vgip_crc_ctrl;

typedef struct
{
    unsigned int    ch1_crc_result:32;
} ICH1_vgip_crc_result;

typedef struct
{
    unsigned int    ch1_wd_to_main:1;
    unsigned int    ch1_wd_to_sub:1;
    unsigned int    dummy18022240_29_19:11;
    unsigned int    ch1_vs_err_wde:1;
    unsigned int    ch1_hs_err_wde:1;
    unsigned int    ch1_field_err_wde:1;
    unsigned int    dummy18022240_15_3:13;
    unsigned int    ch1_vs_err_ie:1;
    unsigned int    ch1_hs_err_ie:1;
    unsigned int    ch1_field_err_ie:1;
} VGIP_chn1_wde;

typedef struct
{
    unsigned int    ch2_ivrun:1;
    unsigned int    ch2_in_sel:3;
    unsigned int    ch2_random_en:1;
    unsigned int    ch2_fdrop_en:1;
    unsigned int    ch2_vact_end_ie:1;
    unsigned int    ch2_vact_start_ie:1;
    unsigned int    ch2_field_hs_inv:1;
    unsigned int    ch2_odd_vd_inv:1;
    unsigned int    ch2_odd_vd_en:1;
    unsigned int    reserved_0:1;
    unsigned int    reserved_1:1;
    unsigned int    ch2_odd_inv:1;
    unsigned int    ch2_odd_sync_edge:1;
    unsigned int    ch2_odd_det_en:1;
    unsigned int    ch2_hpact_is_hact:1;
    unsigned int    ch2_fcap_str_odd:1;
    unsigned int    ch2_mask_den_inv:1;
    unsigned int    ch2_mask_den_1l:1;
    unsigned int    dummy18022244_11:1;
    unsigned int    ch2_safe_odd_inv:1;
    unsigned int    ch2_force_tog:1;
    unsigned int    ch2_safe_mode:1;
    unsigned int    dummy18022244_7:1;
    unsigned int    ch2_hs_syncedge:1;
    unsigned int    ch2_vs_syncedge:1;
    unsigned int    ch2_vs_by_hs_en_n:1;
    unsigned int    ch2_vs_inv:1;
    unsigned int    ch2_hs_inv:1;
    unsigned int    ch2_digital_mode:1;
    unsigned int    ch2_in_clk_en:1;
} VGIP_chn2_ctrl;

typedef struct
{
    unsigned int    reserved_0:4;
    unsigned int    ch2_vs_end:1;
    unsigned int    ch2_vs_start:1;
    unsigned int    ch2_vact_end:1;
    unsigned int    ch2_vact_start:1;
    unsigned int    dummy18022248_23_15:9;
    unsigned int    ch2_vs_err:1;
    unsigned int    ch2_hs_err:1;
    unsigned int    ch2_field_err:1;
    unsigned int    dummy18022248_11_10:2;
    unsigned int    ch2_vs_tog:1;
    unsigned int    ch2_hs_tog:1;
    unsigned int    ch2_field_pol:1;
    unsigned int    ch2_field_tog:1;
    unsigned int    ch2_field_vs_lsb:2;
    unsigned int    ch2_more_1line_field:1;
    unsigned int    ch2_bounce_status_2:1;
    unsigned int    ch2_bounce_status_1:1;
    unsigned int    ch2_bounce_status_0:1;
} VGIP_chn2_status;

typedef struct
{
    unsigned int    ch2_db_rdy:1;
    unsigned int    ch2_db_en:1;
    unsigned int    reserved_0:2;
    unsigned int    ch2_ih_act_sta:12;
    unsigned int    reserved_1:4;
    unsigned int    ch2_ih_act_wid:12;
} VGIP_chn2_act_hsta_width;

typedef struct
{
    unsigned int    reserved_0:4;
    unsigned int    ch2_iv_act_sta:12;
    unsigned int    reserved_1:4;
    unsigned int    ch2_iv_act_len:12;
} VGIP_chn2_act_vsta_length;

typedef struct
{
    unsigned int    reserved_0:4;
    unsigned int    ch2_ihs_dly:12;
    unsigned int    reserved_1:4;
    unsigned int    ch2_ivs_dly:12;
} VGIP_chn2_delay;

typedef struct
{
    unsigned int    ch2_de_mask_en:1;
    unsigned int    reserved_0:12;
    unsigned int    ch2_hporch_num:11;
    unsigned int    ch2_auto_ihs_dly:2;
    unsigned int    ch2_auto_ivs_dly:2;
    unsigned int    ch2_hdly_one:1;
    unsigned int    reserved_1:3;
} VGIP_chn2_misc;

typedef struct
{
    unsigned int    ch2_captst_en:1;
    unsigned int    ch2_captst_mode:1;
    unsigned int    reserved_0:2;
    unsigned int    ch2_vcaptst_num:12;
    unsigned int    reserved_1:4;
    unsigned int    ch2_hcaptst_num:12;
} VGIP_chn2_ptn_h_vi;

typedef struct
{
    unsigned int    reserved_0:8;
    unsigned int    ch2_captst_r_data:8;
    unsigned int    reserved_1:8;
    unsigned int    ch2_captst_g_data:8;
} VGIP_chn2_ptn0;

typedef struct
{
    unsigned int    reserved_0:24;
    unsigned int    ch2_captst_b_data:8;
} VGIP_chn2_ptn1;

typedef struct
{
    unsigned int    ch2_vlcen:1;
    unsigned int    ch2_vlc_mode:1;
    unsigned int    ch2_vlc_ie:1;
    unsigned int    dummy18022268_28:1;
    unsigned int    ch2_vlc_status:1;
    unsigned int    reserved_0:11;
    unsigned int    dummy18022268_15_11:5;
    unsigned int    ch2_vln:11;
} VGIP_chn2_lc;

typedef struct
{
    unsigned int    reserved_0:29;
    unsigned int    reserved_1:1;
    unsigned int    ch2_crc_conti:1;
    unsigned int    ch2_crc_start:1;
} ICH2_vgip_crc_ctrl;

typedef struct
{
    unsigned int    ch2_crc_result:32;
} ICH2_vgip_crc_result;

typedef struct
{
    unsigned int    ch2_wd_to_main:1;
    unsigned int    ch2_wd_to_sub:1;
    unsigned int    dummy18022274_29_19:11;
    unsigned int    ch2_vs_err_wde:1;
    unsigned int    ch2_hs_err_wde:1;
    unsigned int    ch2_field_err_wde:1;
    unsigned int    dummy18022274_15_3:13;
    unsigned int    ch2_vs_err_ie:1;
    unsigned int    ch2_hs_err_ie:1;
    unsigned int    ch2_field_err_ie:1;
} VGIP_chn2_wde;

typedef struct
{
    unsigned int    reserved_0:32;
} RESERVED0;

typedef struct
{
    unsigned int    reserved_0:32;
} RESERVED1;

typedef struct
{
    unsigned int    reserved_0:32;
} RESERVED2;

typedef struct
{
    unsigned int    reserved_0:24;
    unsigned int    vo_1_insel:1;
    unsigned int    vo_2_insel:1;
    unsigned int    fsync_ch_sel:1;
    unsigned int    adc_ch_sel:1;
    unsigned int    xcdi_clk_en:1;
    unsigned int    uzd1_in_sel:1;
    unsigned int    uzd2_in_sel:1;
    unsigned int    xcdi_in_sel:1;
} DATA_path_select;

typedef struct
{
    unsigned int    reserved_0:3;
    unsigned int    vbisli_in_sel:1;
    unsigned int    vbisli_ivs_dly:12;
    unsigned int    reserved_1:4;
    unsigned int    vbisli_ihs_dly:12;
} VGIP_vbisli;

typedef struct
{
    unsigned int    reserved_0:23;
    unsigned int    offms_int_ie:1;
    unsigned int    onms2_int_ie:1;
    unsigned int    onms1_int_ie:1;
    unsigned int    hist_ch1_int_ie:1;
    unsigned int    hist_ch2_int_ie:1;
    unsigned int    vgip_int_ie:1;
    unsigned int    vi_int_ie:1;
    unsigned int    vodma_int_ie:1;
    unsigned int    dispi_int_ie:1;
} INT_ctl;
#endif

#define VCLK_CTRL                                                     0x18022200
#define VCLK_CTRL_reg_addr                                            "0xB8022200"
#define VCLK_CTRL_reg                                                 0xB8022200
#define VCLK_CTRL_inst_addr                                           "0x0080"
#define VCLK_CTRL_inst                                                0x0080
#define VCLK_CTRL_hdmi_ph_in_shift                                    (7)
#define VCLK_CTRL_vclk_dly_shift                                      (1)
#define VCLK_CTRL_vclk_inv_shift                                      (0)
#define VCLK_CTRL_hdmi_ph_in_mask                                     (0x00001F80)
#define VCLK_CTRL_vclk_dly_mask                                       (0x0000007E)
#define VCLK_CTRL_vclk_inv_mask                                       (0x00000001)
#define VCLK_CTRL_hdmi_ph_in(data)                                    (0x00001F80&(((UINT32)data)<<7))
#define VCLK_CTRL_vclk_dly(data)                                      (0x0000007E&(((UINT32)data)<<1))
#define VCLK_CTRL_vclk_inv(data)                                      (0x00000001&(data))
#define VCLK_CTRL_get_hdmi_ph_in(data)                                ((0x00001F80&(data))>>7)
#define VCLK_CTRL_get_vclk_dly(data)                                  ((0x0000007E&(data))>>1)
#define VCLK_CTRL_get_vclk_inv(data)                                  (0x00000001&(data))

#define V10_CTRL                                                      0x18022204
#define V10_CTRL_reg_addr                                             "0xB8022204"
#define V10_CTRL_reg                                                  0xB8022204
#define V10_CTRL_inst_addr                                            "0x0081"
#define V10_CTRL_inst                                                 0x0081
#define V10_CTRL_v10_1biterr_shift                                    (31)
#define V10_CTRL_v10_2biterr_shift                                    (30)
#define V10_CTRL_v10_1biterr_clr_shift                                (29)
#define V10_CTRL_v10_2biterr_clr_shift                                (28)
#define V10_CTRL_v10_1biterr_ie_shift                                 (27)
#define V10_CTRL_v10_2biterr_ie_shift                                 (26)
#define V10_CTRL_video_sel_shift                                      (8)
#define V10_CTRL_dummy18022204_7_5_shift                              (5)
#define V10_CTRL_v10_bit_swap_shift                                   (4)
#define V10_CTRL_dummy18022204_3_2_shift                              (2)
#define V10_CTRL_v10_en_cor_shift                                     (1)
#define V10_CTRL_v10_en_dec_shift                                     (0)
#define V10_CTRL_v10_1biterr_mask                                     (0x80000000)
#define V10_CTRL_v10_2biterr_mask                                     (0x40000000)
#define V10_CTRL_v10_1biterr_clr_mask                                 (0x20000000)
#define V10_CTRL_v10_2biterr_clr_mask                                 (0x10000000)
#define V10_CTRL_v10_1biterr_ie_mask                                  (0x08000000)
#define V10_CTRL_v10_2biterr_ie_mask                                  (0x04000000)
#define V10_CTRL_video_sel_mask                                       (0x00000100)
#define V10_CTRL_dummy18022204_7_5_mask                               (0x000000E0)
#define V10_CTRL_v10_bit_swap_mask                                    (0x00000010)
#define V10_CTRL_dummy18022204_3_2_mask                               (0x0000000C)
#define V10_CTRL_v10_en_cor_mask                                      (0x00000002)
#define V10_CTRL_v10_en_dec_mask                                      (0x00000001)
#define V10_CTRL_v10_1biterr(data)                                    (0x80000000&(((UINT32)data)<<31))
#define V10_CTRL_v10_2biterr(data)                                    (0x40000000&(((UINT32)data)<<30))
#define V10_CTRL_v10_1biterr_clr(data)                                (0x20000000&(((UINT32)data)<<29))
#define V10_CTRL_v10_2biterr_clr(data)                                (0x10000000&(((UINT32)data)<<28))
#define V10_CTRL_v10_1biterr_ie(data)                                 (0x08000000&(((UINT32)data)<<27))
#define V10_CTRL_v10_2biterr_ie(data)                                 (0x04000000&(((UINT32)data)<<26))
#define V10_CTRL_video_sel(data)                                      (0x00000100&(((UINT32)data)<<8))
#define V10_CTRL_dummy18022204_7_5(data)                              (0x000000E0&(((UINT32)data)<<5))
#define V10_CTRL_v10_bit_swap(data)                                   (0x00000010&(((UINT32)data)<<4))
#define V10_CTRL_dummy18022204_3_2(data)                              (0x0000000C&(((UINT32)data)<<2))
#define V10_CTRL_v10_en_cor(data)                                     (0x00000002&(((UINT32)data)<<1))
#define V10_CTRL_v10_en_dec(data)                                     (0x00000001&(data))
#define V10_CTRL_get_v10_1biterr(data)                                ((0x80000000&(data))>>31)
#define V10_CTRL_get_v10_2biterr(data)                                ((0x40000000&(data))>>30)
#define V10_CTRL_get_v10_1biterr_clr(data)                            ((0x20000000&(data))>>29)
#define V10_CTRL_get_v10_2biterr_clr(data)                            ((0x10000000&(data))>>28)
#define V10_CTRL_get_v10_1biterr_ie(data)                             ((0x08000000&(data))>>27)
#define V10_CTRL_get_v10_2biterr_ie(data)                             ((0x04000000&(data))>>26)
#define V10_CTRL_get_video_sel(data)                                  ((0x00000100&(data))>>8)
#define V10_CTRL_get_dummy18022204_7_5(data)                          ((0x000000E0&(data))>>5)
#define V10_CTRL_get_v10_bit_swap(data)                               ((0x00000010&(data))>>4)
#define V10_CTRL_get_dummy18022204_3_2(data)                          ((0x0000000C&(data))>>2)
#define V10_CTRL_get_v10_en_cor(data)                                 ((0x00000002&(data))>>1)
#define V10_CTRL_get_v10_en_dec(data)                                 (0x00000001&(data))

#define V8_2_CTRL                                                     0x18022208
#define V8_2_CTRL_reg_addr                                            "0xB8022208"
#define V8_2_CTRL_reg                                                 0xB8022208
#define V8_2_CTRL_inst_addr                                           "0x0082"
#define V8_2_CTRL_inst                                                0x0082
#define V8_2_CTRL_dummy18022208_31_0_shift                            (0)
#define V8_2_CTRL_dummy18022208_31_0_mask                             (0xFFFFFFFF)
#define V8_2_CTRL_dummy18022208_31_0(data)                            (0xFFFFFFFF&(data))
#define V8_2_CTRL_get_dummy18022208_31_0(data)                        (0xFFFFFFFF&(data))

#define V16_CTRL                                                      0x1802220c
#define V16_CTRL_reg_addr                                             "0xB802220C"
#define V16_CTRL_reg                                                  0xB802220C
#define V16_CTRL_inst_addr                                            "0x0083"
#define V16_CTRL_inst                                                 0x0083
#define V16_CTRL_dummy1802220c_31_0_shift                             (0)
#define V16_CTRL_dummy1802220c_31_0_mask                              (0xFFFFFFFF)
#define V16_CTRL_dummy1802220c_31_0(data)                             (0xFFFFFFFF&(data))
#define V16_CTRL_get_dummy1802220c_31_0(data)                         (0xFFFFFFFF&(data))

#define VGIP_CHN1_CTRL                                                0x18022210
#define VGIP_CHN1_CTRL_reg_addr                                       "0xB8022210"
#define VGIP_CHN1_CTRL_reg                                            0xB8022210
#define VGIP_CHN1_CTRL_inst_addr                                      "0x0084"
#define VGIP_CHN1_CTRL_inst                                           0x0084
#define VGIP_CHN1_CTRL_ch1_ivrun_shift                                (31)
#define VGIP_CHN1_CTRL_ch1_in_sel_shift                               (28)
#define VGIP_CHN1_CTRL_ch1_random_en_shift                            (27)
#define VGIP_CHN1_CTRL_ch1_fdrop_en_shift                             (26)
#define VGIP_CHN1_CTRL_ch1_vact_end_ie_shift                          (25)
#define VGIP_CHN1_CTRL_ch1_vact_start_ie_shift                        (24)
#define VGIP_CHN1_CTRL_ch1_field_hs_inv_shift                         (23)
#define VGIP_CHN1_CTRL_ch1_odd_vd_inv_shift                           (22)
#define VGIP_CHN1_CTRL_ch1_odd_vd_en_shift                            (21)
#define VGIP_CHN1_CTRL_ch1_odd_inv_shift                              (18)
#define VGIP_CHN1_CTRL_ch1_odd_sync_edge_shift                        (17)
#define VGIP_CHN1_CTRL_ch1_odd_det_en_shift                           (16)
#define VGIP_CHN1_CTRL_ch1_hpact_is_hact_shift                        (15)
#define VGIP_CHN1_CTRL_ch1_fcap_str_odd_shift                         (14)
#define VGIP_CHN1_CTRL_ch1_mask_den_inv_shift                         (13)
#define VGIP_CHN1_CTRL_ch1_mask_den_1l_shift                          (12)
#define VGIP_CHN1_CTRL_dummy18022210_11_shift                         (11)
#define VGIP_CHN1_CTRL_ch1_safe_odd_inv_shift                         (10)
#define VGIP_CHN1_CTRL_ch1_force_tog_shift                            (9)
#define VGIP_CHN1_CTRL_ch1_safe_mode_shift                            (8)
#define VGIP_CHN1_CTRL_dummy18022210_7_shift                          (7)
#define VGIP_CHN1_CTRL_ch1_hs_syncedge_shift                          (6)
#define VGIP_CHN1_CTRL_ch1_vs_syncedge_shift                          (5)
#define VGIP_CHN1_CTRL_ch1_vs_by_hs_en_n_shift                        (4)
#define VGIP_CHN1_CTRL_ch1_vs_inv_shift                               (3)
#define VGIP_CHN1_CTRL_ch1_hs_inv_shift                               (2)
#define VGIP_CHN1_CTRL_ch1_digital_mode_shift                         (1)
#define VGIP_CHN1_CTRL_ch1_in_clk_en_shift                            (0)
#define VGIP_CHN1_CTRL_ch1_ivrun_mask                                 (0x80000000)
#define VGIP_CHN1_CTRL_ch1_in_sel_mask                                (0x70000000)
#define VGIP_CHN1_CTRL_ch1_random_en_mask                             (0x08000000)
#define VGIP_CHN1_CTRL_ch1_fdrop_en_mask                              (0x04000000)
#define VGIP_CHN1_CTRL_ch1_vact_end_ie_mask                           (0x02000000)
#define VGIP_CHN1_CTRL_ch1_vact_start_ie_mask                         (0x01000000)
#define VGIP_CHN1_CTRL_ch1_field_hs_inv_mask                          (0x00800000)
#define VGIP_CHN1_CTRL_ch1_odd_vd_inv_mask                            (0x00400000)
#define VGIP_CHN1_CTRL_ch1_odd_vd_en_mask                             (0x00200000)
#define VGIP_CHN1_CTRL_ch1_odd_inv_mask                               (0x00040000)
#define VGIP_CHN1_CTRL_ch1_odd_sync_edge_mask                         (0x00020000)
#define VGIP_CHN1_CTRL_ch1_odd_det_en_mask                            (0x00010000)
#define VGIP_CHN1_CTRL_ch1_hpact_is_hact_mask                         (0x00008000)
#define VGIP_CHN1_CTRL_ch1_fcap_str_odd_mask                          (0x00004000)
#define VGIP_CHN1_CTRL_ch1_mask_den_inv_mask                          (0x00002000)
#define VGIP_CHN1_CTRL_ch1_mask_den_1l_mask                           (0x00001000)
#define VGIP_CHN1_CTRL_dummy18022210_11_mask                          (0x00000800)
#define VGIP_CHN1_CTRL_ch1_safe_odd_inv_mask                          (0x00000400)
#define VGIP_CHN1_CTRL_ch1_force_tog_mask                             (0x00000200)
#define VGIP_CHN1_CTRL_ch1_safe_mode_mask                             (0x00000100)
#define VGIP_CHN1_CTRL_dummy18022210_7_mask                           (0x00000080)
#define VGIP_CHN1_CTRL_ch1_hs_syncedge_mask                           (0x00000040)
#define VGIP_CHN1_CTRL_ch1_vs_syncedge_mask                           (0x00000020)
#define VGIP_CHN1_CTRL_ch1_vs_by_hs_en_n_mask                         (0x00000010)
#define VGIP_CHN1_CTRL_ch1_vs_inv_mask                                (0x00000008)
#define VGIP_CHN1_CTRL_ch1_hs_inv_mask                                (0x00000004)
#define VGIP_CHN1_CTRL_ch1_digital_mode_mask                          (0x00000002)
#define VGIP_CHN1_CTRL_ch1_in_clk_en_mask                             (0x00000001)
#define VGIP_CHN1_CTRL_ch1_ivrun(data)                                (0x80000000&(((UINT32)data)<<31))
#define VGIP_CHN1_CTRL_ch1_in_sel(data)                               (0x70000000&(((UINT32)data)<<28))
#define VGIP_CHN1_CTRL_ch1_random_en(data)                            (0x08000000&(((UINT32)data)<<27))
#define VGIP_CHN1_CTRL_ch1_fdrop_en(data)                             (0x04000000&(((UINT32)data)<<26))
#define VGIP_CHN1_CTRL_ch1_vact_end_ie(data)                          (0x02000000&(((UINT32)data)<<25))
#define VGIP_CHN1_CTRL_ch1_vact_start_ie(data)                        (0x01000000&(((UINT32)data)<<24))
#define VGIP_CHN1_CTRL_ch1_field_hs_inv(data)                         (0x00800000&(((UINT32)data)<<23))
#define VGIP_CHN1_CTRL_ch1_odd_vd_inv(data)                           (0x00400000&(((UINT32)data)<<22))
#define VGIP_CHN1_CTRL_ch1_odd_vd_en(data)                            (0x00200000&(((UINT32)data)<<21))
#define VGIP_CHN1_CTRL_ch1_odd_inv(data)                              (0x00040000&(((UINT32)data)<<18))
#define VGIP_CHN1_CTRL_ch1_odd_sync_edge(data)                        (0x00020000&(((UINT32)data)<<17))
#define VGIP_CHN1_CTRL_ch1_odd_det_en(data)                           (0x00010000&(((UINT32)data)<<16))
#define VGIP_CHN1_CTRL_ch1_hpact_is_hact(data)                        (0x00008000&(((UINT32)data)<<15))
#define VGIP_CHN1_CTRL_ch1_fcap_str_odd(data)                         (0x00004000&(((UINT32)data)<<14))
#define VGIP_CHN1_CTRL_ch1_mask_den_inv(data)                         (0x00002000&(((UINT32)data)<<13))
#define VGIP_CHN1_CTRL_ch1_mask_den_1l(data)                          (0x00001000&(((UINT32)data)<<12))
#define VGIP_CHN1_CTRL_dummy18022210_11(data)                         (0x00000800&(((UINT32)data)<<11))
#define VGIP_CHN1_CTRL_ch1_safe_odd_inv(data)                         (0x00000400&(((UINT32)data)<<10))
#define VGIP_CHN1_CTRL_ch1_force_tog(data)                            (0x00000200&(((UINT32)data)<<9))
#define VGIP_CHN1_CTRL_ch1_safe_mode(data)                            (0x00000100&(((UINT32)data)<<8))
#define VGIP_CHN1_CTRL_dummy18022210_7(data)                          (0x00000080&(((UINT32)data)<<7))
#define VGIP_CHN1_CTRL_ch1_hs_syncedge(data)                          (0x00000040&(((UINT32)data)<<6))
#define VGIP_CHN1_CTRL_ch1_vs_syncedge(data)                          (0x00000020&(((UINT32)data)<<5))
#define VGIP_CHN1_CTRL_ch1_vs_by_hs_en_n(data)                        (0x00000010&(((UINT32)data)<<4))
#define VGIP_CHN1_CTRL_ch1_vs_inv(data)                               (0x00000008&(((UINT32)data)<<3))
#define VGIP_CHN1_CTRL_ch1_hs_inv(data)                               (0x00000004&(((UINT32)data)<<2))
#define VGIP_CHN1_CTRL_ch1_digital_mode(data)                         (0x00000002&(((UINT32)data)<<1))
#define VGIP_CHN1_CTRL_ch1_in_clk_en(data)                            (0x00000001&(data))
#define VGIP_CHN1_CTRL_get_ch1_ivrun(data)                            ((0x80000000&(data))>>31)
#define VGIP_CHN1_CTRL_get_ch1_in_sel(data)                           ((0x70000000&(data))>>28)
#define VGIP_CHN1_CTRL_get_ch1_random_en(data)                        ((0x08000000&(data))>>27)
#define VGIP_CHN1_CTRL_get_ch1_fdrop_en(data)                         ((0x04000000&(data))>>26)
#define VGIP_CHN1_CTRL_get_ch1_vact_end_ie(data)                      ((0x02000000&(data))>>25)
#define VGIP_CHN1_CTRL_get_ch1_vact_start_ie(data)                    ((0x01000000&(data))>>24)
#define VGIP_CHN1_CTRL_get_ch1_field_hs_inv(data)                     ((0x00800000&(data))>>23)
#define VGIP_CHN1_CTRL_get_ch1_odd_vd_inv(data)                       ((0x00400000&(data))>>22)
#define VGIP_CHN1_CTRL_get_ch1_odd_vd_en(data)                        ((0x00200000&(data))>>21)
#define VGIP_CHN1_CTRL_get_ch1_odd_inv(data)                          ((0x00040000&(data))>>18)
#define VGIP_CHN1_CTRL_get_ch1_odd_sync_edge(data)                    ((0x00020000&(data))>>17)
#define VGIP_CHN1_CTRL_get_ch1_odd_det_en(data)                       ((0x00010000&(data))>>16)
#define VGIP_CHN1_CTRL_get_ch1_hpact_is_hact(data)                    ((0x00008000&(data))>>15)
#define VGIP_CHN1_CTRL_get_ch1_fcap_str_odd(data)                     ((0x00004000&(data))>>14)
#define VGIP_CHN1_CTRL_get_ch1_mask_den_inv(data)                     ((0x00002000&(data))>>13)
#define VGIP_CHN1_CTRL_get_ch1_mask_den_1l(data)                      ((0x00001000&(data))>>12)
#define VGIP_CHN1_CTRL_get_dummy18022210_11(data)                     ((0x00000800&(data))>>11)
#define VGIP_CHN1_CTRL_get_ch1_safe_odd_inv(data)                     ((0x00000400&(data))>>10)
#define VGIP_CHN1_CTRL_get_ch1_force_tog(data)                        ((0x00000200&(data))>>9)
#define VGIP_CHN1_CTRL_get_ch1_safe_mode(data)                        ((0x00000100&(data))>>8)
#define VGIP_CHN1_CTRL_get_dummy18022210_7(data)                      ((0x00000080&(data))>>7)
#define VGIP_CHN1_CTRL_get_ch1_hs_syncedge(data)                      ((0x00000040&(data))>>6)
#define VGIP_CHN1_CTRL_get_ch1_vs_syncedge(data)                      ((0x00000020&(data))>>5)
#define VGIP_CHN1_CTRL_get_ch1_vs_by_hs_en_n(data)                    ((0x00000010&(data))>>4)
#define VGIP_CHN1_CTRL_get_ch1_vs_inv(data)                           ((0x00000008&(data))>>3)
#define VGIP_CHN1_CTRL_get_ch1_hs_inv(data)                           ((0x00000004&(data))>>2)
#define VGIP_CHN1_CTRL_get_ch1_digital_mode(data)                     ((0x00000002&(data))>>1)
#define VGIP_CHN1_CTRL_get_ch1_in_clk_en(data)                        (0x00000001&(data))

#define VGIP_CHN1_STATUS                                              0x18022214
#define VGIP_CHN1_STATUS_reg_addr                                     "0xB8022214"
#define VGIP_CHN1_STATUS_reg                                          0xB8022214
#define VGIP_CHN1_STATUS_inst_addr                                    "0x0085"
#define VGIP_CHN1_STATUS_inst                                         0x0085
#define VGIP_CHN1_STATUS_ch1_vs_end_shift                             (27)
#define VGIP_CHN1_STATUS_ch1_vs_start_shift                           (26)
#define VGIP_CHN1_STATUS_ch1_vact_end_shift                           (25)
#define VGIP_CHN1_STATUS_ch1_vact_start_shift                         (24)
#define VGIP_CHN1_STATUS_dummy18022214_23_15_shift                    (15)
#define VGIP_CHN1_STATUS_ch1_vs_err_shift                             (14)
#define VGIP_CHN1_STATUS_ch1_hs_err_shift                             (13)
#define VGIP_CHN1_STATUS_ch1_field_err_shift                          (12)
#define VGIP_CHN1_STATUS_dummy18022214_11_10_shift                    (10)
#define VGIP_CHN1_STATUS_ch1_vs_tog_shift                             (9)
#define VGIP_CHN1_STATUS_ch1_hs_tog_shift                             (8)
#define VGIP_CHN1_STATUS_ch1_field_pol_shift                          (7)
#define VGIP_CHN1_STATUS_ch1_field_tog_shift                          (6)
#define VGIP_CHN1_STATUS_ch1_field_vs_lsb_shift                       (4)
#define VGIP_CHN1_STATUS_ch1_more_1line_field_shift                   (3)
#define VGIP_CHN1_STATUS_ch1_bounce_status_2_shift                    (2)
#define VGIP_CHN1_STATUS_ch1_bounce_status_1_shift                    (1)
#define VGIP_CHN1_STATUS_ch1_bounce_status_0_shift                    (0)
#define VGIP_CHN1_STATUS_ch1_vs_end_mask                              (0x08000000)
#define VGIP_CHN1_STATUS_ch1_vs_start_mask                            (0x04000000)
#define VGIP_CHN1_STATUS_ch1_vact_end_mask                            (0x02000000)
#define VGIP_CHN1_STATUS_ch1_vact_start_mask                          (0x01000000)
#define VGIP_CHN1_STATUS_dummy18022214_23_15_mask                     (0x00FF8000)
#define VGIP_CHN1_STATUS_ch1_vs_err_mask                              (0x00004000)
#define VGIP_CHN1_STATUS_ch1_hs_err_mask                              (0x00002000)
#define VGIP_CHN1_STATUS_ch1_field_err_mask                           (0x00001000)
#define VGIP_CHN1_STATUS_dummy18022214_11_10_mask                     (0x00000C00)
#define VGIP_CHN1_STATUS_ch1_vs_tog_mask                              (0x00000200)
#define VGIP_CHN1_STATUS_ch1_hs_tog_mask                              (0x00000100)
#define VGIP_CHN1_STATUS_ch1_field_pol_mask                           (0x00000080)
#define VGIP_CHN1_STATUS_ch1_field_tog_mask                           (0x00000040)
#define VGIP_CHN1_STATUS_ch1_field_vs_lsb_mask                        (0x00000030)
#define VGIP_CHN1_STATUS_ch1_more_1line_field_mask                    (0x00000008)
#define VGIP_CHN1_STATUS_ch1_bounce_status_2_mask                     (0x00000004)
#define VGIP_CHN1_STATUS_ch1_bounce_status_1_mask                     (0x00000002)
#define VGIP_CHN1_STATUS_ch1_bounce_status_0_mask                     (0x00000001)
#define VGIP_CHN1_STATUS_ch1_vs_end(data)                             (0x08000000&(((UINT32)data)<<27))
#define VGIP_CHN1_STATUS_ch1_vs_start(data)                           (0x04000000&(((UINT32)data)<<26))
#define VGIP_CHN1_STATUS_ch1_vact_end(data)                           (0x02000000&(((UINT32)data)<<25))
#define VGIP_CHN1_STATUS_ch1_vact_start(data)                         (0x01000000&(((UINT32)data)<<24))
#define VGIP_CHN1_STATUS_dummy18022214_23_15(data)                    (0x00FF8000&(((UINT32)data)<<15))
#define VGIP_CHN1_STATUS_ch1_vs_err(data)                             (0x00004000&(((UINT32)data)<<14))
#define VGIP_CHN1_STATUS_ch1_hs_err(data)                             (0x00002000&(((UINT32)data)<<13))
#define VGIP_CHN1_STATUS_ch1_field_err(data)                          (0x00001000&(((UINT32)data)<<12))
#define VGIP_CHN1_STATUS_dummy18022214_11_10(data)                    (0x00000C00&(((UINT32)data)<<10))
#define VGIP_CHN1_STATUS_ch1_vs_tog(data)                             (0x00000200&(((UINT32)data)<<9))
#define VGIP_CHN1_STATUS_ch1_hs_tog(data)                             (0x00000100&(((UINT32)data)<<8))
#define VGIP_CHN1_STATUS_ch1_field_pol(data)                          (0x00000080&(((UINT32)data)<<7))
#define VGIP_CHN1_STATUS_ch1_field_tog(data)                          (0x00000040&(((UINT32)data)<<6))
#define VGIP_CHN1_STATUS_ch1_field_vs_lsb(data)                       (0x00000030&(((UINT32)data)<<4))
#define VGIP_CHN1_STATUS_ch1_more_1line_field(data)                   (0x00000008&(((UINT32)data)<<3))
#define VGIP_CHN1_STATUS_ch1_bounce_status_2(data)                    (0x00000004&(((UINT32)data)<<2))
#define VGIP_CHN1_STATUS_ch1_bounce_status_1(data)                    (0x00000002&(((UINT32)data)<<1))
#define VGIP_CHN1_STATUS_ch1_bounce_status_0(data)                    (0x00000001&(data))
#define VGIP_CHN1_STATUS_get_ch1_vs_end(data)                         ((0x08000000&(data))>>27)
#define VGIP_CHN1_STATUS_get_ch1_vs_start(data)                       ((0x04000000&(data))>>26)
#define VGIP_CHN1_STATUS_get_ch1_vact_end(data)                       ((0x02000000&(data))>>25)
#define VGIP_CHN1_STATUS_get_ch1_vact_start(data)                     ((0x01000000&(data))>>24)
#define VGIP_CHN1_STATUS_get_dummy18022214_23_15(data)                ((0x00FF8000&(data))>>15)
#define VGIP_CHN1_STATUS_get_ch1_vs_err(data)                         ((0x00004000&(data))>>14)
#define VGIP_CHN1_STATUS_get_ch1_hs_err(data)                         ((0x00002000&(data))>>13)
#define VGIP_CHN1_STATUS_get_ch1_field_err(data)                      ((0x00001000&(data))>>12)
#define VGIP_CHN1_STATUS_get_dummy18022214_11_10(data)                ((0x00000C00&(data))>>10)
#define VGIP_CHN1_STATUS_get_ch1_vs_tog(data)                         ((0x00000200&(data))>>9)
#define VGIP_CHN1_STATUS_get_ch1_hs_tog(data)                         ((0x00000100&(data))>>8)
#define VGIP_CHN1_STATUS_get_ch1_field_pol(data)                      ((0x00000080&(data))>>7)
#define VGIP_CHN1_STATUS_get_ch1_field_tog(data)                      ((0x00000040&(data))>>6)
#define VGIP_CHN1_STATUS_get_ch1_field_vs_lsb(data)                   ((0x00000030&(data))>>4)
#define VGIP_CHN1_STATUS_get_ch1_more_1line_field(data)               ((0x00000008&(data))>>3)
#define VGIP_CHN1_STATUS_get_ch1_bounce_status_2(data)                ((0x00000004&(data))>>2)
#define VGIP_CHN1_STATUS_get_ch1_bounce_status_1(data)                ((0x00000002&(data))>>1)
#define VGIP_CHN1_STATUS_get_ch1_bounce_status_0(data)                (0x00000001&(data))

#define VGIP_CHN1_ACT_HSTA_WIDTH                                      0x18022218
#define VGIP_CHN1_ACT_HSTA_WIDTH_reg_addr                             "0xB8022218"
#define VGIP_CHN1_ACT_HSTA_WIDTH_reg                                  0xB8022218
#define VGIP_CHN1_ACT_HSTA_WIDTH_inst_addr                            "0x0086"
#define VGIP_CHN1_ACT_HSTA_WIDTH_inst                                 0x0086
#define VGIP_CHN1_ACT_HSTA_WIDTH_ch1_db_rdy_shift                     (31)
#define VGIP_CHN1_ACT_HSTA_WIDTH_ch1_db_en_shift                      (30)
#define VGIP_CHN1_ACT_HSTA_WIDTH_ch1_ih_act_sta_shift                 (16)
#define VGIP_CHN1_ACT_HSTA_WIDTH_ch1_ih_act_wid_shift                 (0)
#define VGIP_CHN1_ACT_HSTA_WIDTH_ch1_db_rdy_mask                      (0x80000000)
#define VGIP_CHN1_ACT_HSTA_WIDTH_ch1_db_en_mask                       (0x40000000)
#define VGIP_CHN1_ACT_HSTA_WIDTH_ch1_ih_act_sta_mask                  (0x0FFF0000)
#define VGIP_CHN1_ACT_HSTA_WIDTH_ch1_ih_act_wid_mask                  (0x00000FFF)
#define VGIP_CHN1_ACT_HSTA_WIDTH_ch1_db_rdy(data)                     (0x80000000&(((UINT32)data)<<31))
#define VGIP_CHN1_ACT_HSTA_WIDTH_ch1_db_en(data)                      (0x40000000&(((UINT32)data)<<30))
#define VGIP_CHN1_ACT_HSTA_WIDTH_ch1_ih_act_sta(data)                 (0x0FFF0000&(((UINT32)data)<<16))
#define VGIP_CHN1_ACT_HSTA_WIDTH_ch1_ih_act_wid(data)                 (0x00000FFF&(data))
#define VGIP_CHN1_ACT_HSTA_WIDTH_get_ch1_db_rdy(data)                 ((0x80000000&(data))>>31)
#define VGIP_CHN1_ACT_HSTA_WIDTH_get_ch1_db_en(data)                  ((0x40000000&(data))>>30)
#define VGIP_CHN1_ACT_HSTA_WIDTH_get_ch1_ih_act_sta(data)             ((0x0FFF0000&(data))>>16)
#define VGIP_CHN1_ACT_HSTA_WIDTH_get_ch1_ih_act_wid(data)             (0x00000FFF&(data))

#define VGIP_CHN1_ACT_VSTA_LENGTH                                     0x1802221c
#define VGIP_CHN1_ACT_VSTA_LENGTH_reg_addr                            "0xB802221C"
#define VGIP_CHN1_ACT_VSTA_LENGTH_reg                                 0xB802221C
#define VGIP_CHN1_ACT_VSTA_LENGTH_inst_addr                           "0x0087"
#define VGIP_CHN1_ACT_VSTA_LENGTH_inst                                0x0087
#define VGIP_CHN1_ACT_VSTA_LENGTH_ch1_iv_act_sta_shift                (16)
#define VGIP_CHN1_ACT_VSTA_LENGTH_ch1_iv_act_len_shift                (0)
#define VGIP_CHN1_ACT_VSTA_LENGTH_ch1_iv_act_sta_mask                 (0x0FFF0000)
#define VGIP_CHN1_ACT_VSTA_LENGTH_ch1_iv_act_len_mask                 (0x00000FFF)
#define VGIP_CHN1_ACT_VSTA_LENGTH_ch1_iv_act_sta(data)                (0x0FFF0000&(((UINT32)data)<<16))
#define VGIP_CHN1_ACT_VSTA_LENGTH_ch1_iv_act_len(data)                (0x00000FFF&(data))
#define VGIP_CHN1_ACT_VSTA_LENGTH_get_ch1_iv_act_sta(data)            ((0x0FFF0000&(data))>>16)
#define VGIP_CHN1_ACT_VSTA_LENGTH_get_ch1_iv_act_len(data)            (0x00000FFF&(data))

#define VGIP_CHN1_DELAY                                               0x18022220
#define VGIP_CHN1_DELAY_reg_addr                                      "0xB8022220"
#define VGIP_CHN1_DELAY_reg                                           0xB8022220
#define VGIP_CHN1_DELAY_inst_addr                                     "0x0088"
#define VGIP_CHN1_DELAY_inst                                          0x0088
#define VGIP_CHN1_DELAY_ch1_ihs_dly_shift                             (16)
#define VGIP_CHN1_DELAY_ch1_ivs_dly_shift                             (0)
#define VGIP_CHN1_DELAY_ch1_ihs_dly_mask                              (0x0FFF0000)
#define VGIP_CHN1_DELAY_ch1_ivs_dly_mask                              (0x00000FFF)
#define VGIP_CHN1_DELAY_ch1_ihs_dly(data)                             (0x0FFF0000&(((UINT32)data)<<16))
#define VGIP_CHN1_DELAY_ch1_ivs_dly(data)                             (0x00000FFF&(data))
#define VGIP_CHN1_DELAY_get_ch1_ihs_dly(data)                         ((0x0FFF0000&(data))>>16)
#define VGIP_CHN1_DELAY_get_ch1_ivs_dly(data)                         (0x00000FFF&(data))

#define VGIP_CHN1_MISC                                                0x18022224
#define VGIP_CHN1_MISC_reg_addr                                       "0xB8022224"
#define VGIP_CHN1_MISC_reg                                            0xB8022224
#define VGIP_CHN1_MISC_inst_addr                                      "0x0089"
#define VGIP_CHN1_MISC_inst                                           0x0089
#define VGIP_CHN1_MISC_ch1_de_mask_en_shift                           (31)
#define VGIP_CHN1_MISC_ch1_hporch_num_shift                           (8)
#define VGIP_CHN1_MISC_ch1_auto_ihs_dly_shift                         (6)
#define VGIP_CHN1_MISC_ch1_auto_ivs_dly_shift                         (4)
#define VGIP_CHN1_MISC_ch1_hdly_one_shift                             (3)
#define VGIP_CHN1_MISC_ch1_de_mask_en_mask                            (0x80000000)
#define VGIP_CHN1_MISC_ch1_hporch_num_mask                            (0x0007FF00)
#define VGIP_CHN1_MISC_ch1_auto_ihs_dly_mask                          (0x000000C0)
#define VGIP_CHN1_MISC_ch1_auto_ivs_dly_mask                          (0x00000030)
#define VGIP_CHN1_MISC_ch1_hdly_one_mask                              (0x00000008)
#define VGIP_CHN1_MISC_ch1_de_mask_en(data)                           (0x80000000&(((UINT32)data)<<31))
#define VGIP_CHN1_MISC_ch1_hporch_num(data)                           (0x0007FF00&(((UINT32)data)<<8))
#define VGIP_CHN1_MISC_ch1_auto_ihs_dly(data)                         (0x000000C0&(((UINT32)data)<<6))
#define VGIP_CHN1_MISC_ch1_auto_ivs_dly(data)                         (0x00000030&(((UINT32)data)<<4))
#define VGIP_CHN1_MISC_ch1_hdly_one(data)                             (0x00000008&(((UINT32)data)<<3))
#define VGIP_CHN1_MISC_get_ch1_de_mask_en(data)                       ((0x80000000&(data))>>31)
#define VGIP_CHN1_MISC_get_ch1_hporch_num(data)                       ((0x0007FF00&(data))>>8)
#define VGIP_CHN1_MISC_get_ch1_auto_ihs_dly(data)                     ((0x000000C0&(data))>>6)
#define VGIP_CHN1_MISC_get_ch1_auto_ivs_dly(data)                     ((0x00000030&(data))>>4)
#define VGIP_CHN1_MISC_get_ch1_hdly_one(data)                         ((0x00000008&(data))>>3)

#define VGIP_CHN1_PTN_H_VI                                            0x18022228
#define VGIP_CHN1_PTN_H_VI_reg_addr                                   "0xB8022228"
#define VGIP_CHN1_PTN_H_VI_reg                                        0xB8022228
#define VGIP_CHN1_PTN_H_VI_inst_addr                                  "0x008A"
#define VGIP_CHN1_PTN_H_VI_inst                                       0x008A
#define VGIP_CHN1_PTN_H_VI_ch1_captst_en_shift                        (31)
#define VGIP_CHN1_PTN_H_VI_ch1_captst_mode_shift                      (30)
#define VGIP_CHN1_PTN_H_VI_ch1_vcaptst_num_shift                      (16)
#define VGIP_CHN1_PTN_H_VI_ch1_hcaptst_num_shift                      (0)
#define VGIP_CHN1_PTN_H_VI_ch1_captst_en_mask                         (0x80000000)
#define VGIP_CHN1_PTN_H_VI_ch1_captst_mode_mask                       (0x40000000)
#define VGIP_CHN1_PTN_H_VI_ch1_vcaptst_num_mask                       (0x0FFF0000)
#define VGIP_CHN1_PTN_H_VI_ch1_hcaptst_num_mask                       (0x00000FFF)
#define VGIP_CHN1_PTN_H_VI_ch1_captst_en(data)                        (0x80000000&(((UINT32)data)<<31))
#define VGIP_CHN1_PTN_H_VI_ch1_captst_mode(data)                      (0x40000000&(((UINT32)data)<<30))
#define VGIP_CHN1_PTN_H_VI_ch1_vcaptst_num(data)                      (0x0FFF0000&(((UINT32)data)<<16))
#define VGIP_CHN1_PTN_H_VI_ch1_hcaptst_num(data)                      (0x00000FFF&(data))
#define VGIP_CHN1_PTN_H_VI_get_ch1_captst_en(data)                    ((0x80000000&(data))>>31)
#define VGIP_CHN1_PTN_H_VI_get_ch1_captst_mode(data)                  ((0x40000000&(data))>>30)
#define VGIP_CHN1_PTN_H_VI_get_ch1_vcaptst_num(data)                  ((0x0FFF0000&(data))>>16)
#define VGIP_CHN1_PTN_H_VI_get_ch1_hcaptst_num(data)                  (0x00000FFF&(data))

#define VGIP_CHN1_PTN0                                                0x1802222c
#define VGIP_CHN1_PTN0_reg_addr                                       "0xB802222C"
#define VGIP_CHN1_PTN0_reg                                            0xB802222C
#define VGIP_CHN1_PTN0_inst_addr                                      "0x008B"
#define VGIP_CHN1_PTN0_inst                                           0x008B
#define VGIP_CHN1_PTN0_ch1_captst_r_data_shift                        (16)
#define VGIP_CHN1_PTN0_ch1_captst_g_data_shift                        (0)
#define VGIP_CHN1_PTN0_ch1_captst_r_data_mask                         (0x01FF0000)
#define VGIP_CHN1_PTN0_ch1_captst_g_data_mask                         (0x000001FF)
#define VGIP_CHN1_PTN0_ch1_captst_r_data(data)                        (0x01FF0000&(((UINT32)data)<<16))
#define VGIP_CHN1_PTN0_ch1_captst_g_data(data)                        (0x000001FF&(data))
#define VGIP_CHN1_PTN0_get_ch1_captst_r_data(data)                    ((0x01FF0000&(data))>>16)
#define VGIP_CHN1_PTN0_get_ch1_captst_g_data(data)                    (0x000001FF&(data))

#define VGIP_CHN1_PTN1                                                0x18022230
#define VGIP_CHN1_PTN1_reg_addr                                       "0xB8022230"
#define VGIP_CHN1_PTN1_reg                                            0xB8022230
#define VGIP_CHN1_PTN1_inst_addr                                      "0x008C"
#define VGIP_CHN1_PTN1_inst                                           0x008C
#define VGIP_CHN1_PTN1_ch1_captst_b_data_shift                        (0)
#define VGIP_CHN1_PTN1_ch1_captst_b_data_mask                         (0x000001FF)
#define VGIP_CHN1_PTN1_ch1_captst_b_data(data)                        (0x000001FF&(data))
#define VGIP_CHN1_PTN1_get_ch1_captst_b_data(data)                    (0x000001FF&(data))

#define VGIP_CHN1_LC                                                  0x18022234
#define VGIP_CHN1_LC_reg_addr                                         "0xB8022234"
#define VGIP_CHN1_LC_reg                                              0xB8022234
#define VGIP_CHN1_LC_inst_addr                                        "0x008D"
#define VGIP_CHN1_LC_inst                                             0x008D
#define VGIP_CHN1_LC_ch1_vlcen_shift                                  (31)
#define VGIP_CHN1_LC_ch1_vlc_mode_shift                               (30)
#define VGIP_CHN1_LC_ch1_vlc_ie_shift                                 (29)
#define VGIP_CHN1_LC_dummy18022234_28_shift                           (28)
#define VGIP_CHN1_LC_ch1_vlc_status_shift                             (27)
#define VGIP_CHN1_LC_dummy18022234_15_11_shift                        (11)
#define VGIP_CHN1_LC_ch1_vln_shift                                    (0)
#define VGIP_CHN1_LC_ch1_vlcen_mask                                   (0x80000000)
#define VGIP_CHN1_LC_ch1_vlc_mode_mask                                (0x40000000)
#define VGIP_CHN1_LC_ch1_vlc_ie_mask                                  (0x20000000)
#define VGIP_CHN1_LC_dummy18022234_28_mask                            (0x10000000)
#define VGIP_CHN1_LC_ch1_vlc_status_mask                              (0x08000000)
#define VGIP_CHN1_LC_dummy18022234_15_11_mask                         (0x0000F800)
#define VGIP_CHN1_LC_ch1_vln_mask                                     (0x000007FF)
#define VGIP_CHN1_LC_ch1_vlcen(data)                                  (0x80000000&(((UINT32)data)<<31))
#define VGIP_CHN1_LC_ch1_vlc_mode(data)                               (0x40000000&(((UINT32)data)<<30))
#define VGIP_CHN1_LC_ch1_vlc_ie(data)                                 (0x20000000&(((UINT32)data)<<29))
#define VGIP_CHN1_LC_dummy18022234_28(data)                           (0x10000000&(((UINT32)data)<<28))
#define VGIP_CHN1_LC_ch1_vlc_status(data)                             (0x08000000&(((UINT32)data)<<27))
#define VGIP_CHN1_LC_dummy18022234_15_11(data)                        (0x0000F800&(((UINT32)data)<<11))
#define VGIP_CHN1_LC_ch1_vln(data)                                    (0x000007FF&(data))
#define VGIP_CHN1_LC_get_ch1_vlcen(data)                              ((0x80000000&(data))>>31)
#define VGIP_CHN1_LC_get_ch1_vlc_mode(data)                           ((0x40000000&(data))>>30)
#define VGIP_CHN1_LC_get_ch1_vlc_ie(data)                             ((0x20000000&(data))>>29)
#define VGIP_CHN1_LC_get_dummy18022234_28(data)                       ((0x10000000&(data))>>28)
#define VGIP_CHN1_LC_get_ch1_vlc_status(data)                         ((0x08000000&(data))>>27)
#define VGIP_CHN1_LC_get_dummy18022234_15_11(data)                    ((0x0000F800&(data))>>11)
#define VGIP_CHN1_LC_get_ch1_vln(data)                                (0x000007FF&(data))

#define ICH1_VGIP_CRC_CTRL                                            0x18022238
#define ICH1_VGIP_CRC_CTRL_reg_addr                                   "0xB8022238"
#define ICH1_VGIP_CRC_CTRL_reg                                        0xB8022238
#define ICH1_VGIP_CRC_CTRL_inst_addr                                  "0x008E"
#define ICH1_VGIP_CRC_CTRL_inst                                       0x008E
#define ICH1_VGIP_CRC_CTRL_ch1_crc_conti_shift                        (1)
#define ICH1_VGIP_CRC_CTRL_ch1_crc_start_shift                        (0)
#define ICH1_VGIP_CRC_CTRL_ch1_crc_conti_mask                         (0x00000002)
#define ICH1_VGIP_CRC_CTRL_ch1_crc_start_mask                         (0x00000001)
#define ICH1_VGIP_CRC_CTRL_ch1_crc_conti(data)                        (0x00000002&(((UINT32)data)<<1))
#define ICH1_VGIP_CRC_CTRL_ch1_crc_start(data)                        (0x00000001&(data))
#define ICH1_VGIP_CRC_CTRL_get_ch1_crc_conti(data)                    ((0x00000002&(data))>>1)
#define ICH1_VGIP_CRC_CTRL_get_ch1_crc_start(data)                    (0x00000001&(data))

#define ICH1_VGIP_CRC_RESULT                                          0x1802223c
#define ICH1_VGIP_CRC_RESULT_reg_addr                                 "0xB802223C"
#define ICH1_VGIP_CRC_RESULT_reg                                      0xB802223C
#define ICH1_VGIP_CRC_RESULT_inst_addr                                "0x008F"
#define ICH1_VGIP_CRC_RESULT_inst                                     0x008F
#define ICH1_VGIP_CRC_RESULT_ch1_crc_result_shift                     (0)
#define ICH1_VGIP_CRC_RESULT_ch1_crc_result_mask                      (0xFFFFFFFF)
#define ICH1_VGIP_CRC_RESULT_ch1_crc_result(data)                     (0xFFFFFFFF&(data))
#define ICH1_VGIP_CRC_RESULT_get_ch1_crc_result(data)                 (0xFFFFFFFF&(data))

#define VGIP_CHN1_WDE                                                 0x18022240
#define VGIP_CHN1_WDE_reg_addr                                        "0xB8022240"
#define VGIP_CHN1_WDE_reg                                             0xB8022240
#define VGIP_CHN1_WDE_inst_addr                                       "0x0090"
#define VGIP_CHN1_WDE_inst                                            0x0090
#define VGIP_CHN1_WDE_ch1_wd_to_main_shift                            (31)
#define VGIP_CHN1_WDE_ch1_wd_to_sub_shift                             (30)
#define VGIP_CHN1_WDE_dummy18022240_29_19_shift                       (19)
#define VGIP_CHN1_WDE_ch1_vs_err_wde_shift                            (18)
#define VGIP_CHN1_WDE_ch1_hs_err_wde_shift                            (17)
#define VGIP_CHN1_WDE_ch1_field_err_wde_shift                         (16)
#define VGIP_CHN1_WDE_dummy18022240_15_3_shift                        (3)
#define VGIP_CHN1_WDE_ch1_vs_err_ie_shift                             (2)
#define VGIP_CHN1_WDE_ch1_hs_err_ie_shift                             (1)
#define VGIP_CHN1_WDE_ch1_field_err_ie_shift                          (0)
#define VGIP_CHN1_WDE_ch1_wd_to_main_mask                             (0x80000000)
#define VGIP_CHN1_WDE_ch1_wd_to_sub_mask                              (0x40000000)
#define VGIP_CHN1_WDE_dummy18022240_29_19_mask                        (0x3FF80000)
#define VGIP_CHN1_WDE_ch1_vs_err_wde_mask                             (0x00040000)
#define VGIP_CHN1_WDE_ch1_hs_err_wde_mask                             (0x00020000)
#define VGIP_CHN1_WDE_ch1_field_err_wde_mask                          (0x00010000)
#define VGIP_CHN1_WDE_dummy18022240_15_3_mask                         (0x0000FFF8)
#define VGIP_CHN1_WDE_ch1_vs_err_ie_mask                              (0x00000004)
#define VGIP_CHN1_WDE_ch1_hs_err_ie_mask                              (0x00000002)
#define VGIP_CHN1_WDE_ch1_field_err_ie_mask                           (0x00000001)
#define VGIP_CHN1_WDE_ch1_wd_to_main(data)                            (0x80000000&(((UINT32)data)<<31))
#define VGIP_CHN1_WDE_ch1_wd_to_sub(data)                             (0x40000000&(((UINT32)data)<<30))
#define VGIP_CHN1_WDE_dummy18022240_29_19(data)                       (0x3FF80000&(((UINT32)data)<<19))
#define VGIP_CHN1_WDE_ch1_vs_err_wde(data)                            (0x00040000&(((UINT32)data)<<18))
#define VGIP_CHN1_WDE_ch1_hs_err_wde(data)                            (0x00020000&(((UINT32)data)<<17))
#define VGIP_CHN1_WDE_ch1_field_err_wde(data)                         (0x00010000&(((UINT32)data)<<16))
#define VGIP_CHN1_WDE_dummy18022240_15_3(data)                        (0x0000FFF8&(((UINT32)data)<<3))
#define VGIP_CHN1_WDE_ch1_vs_err_ie(data)                             (0x00000004&(((UINT32)data)<<2))
#define VGIP_CHN1_WDE_ch1_hs_err_ie(data)                             (0x00000002&(((UINT32)data)<<1))
#define VGIP_CHN1_WDE_ch1_field_err_ie(data)                          (0x00000001&(data))
#define VGIP_CHN1_WDE_get_ch1_wd_to_main(data)                        ((0x80000000&(data))>>31)
#define VGIP_CHN1_WDE_get_ch1_wd_to_sub(data)                         ((0x40000000&(data))>>30)
#define VGIP_CHN1_WDE_get_dummy18022240_29_19(data)                   ((0x3FF80000&(data))>>19)
#define VGIP_CHN1_WDE_get_ch1_vs_err_wde(data)                        ((0x00040000&(data))>>18)
#define VGIP_CHN1_WDE_get_ch1_hs_err_wde(data)                        ((0x00020000&(data))>>17)
#define VGIP_CHN1_WDE_get_ch1_field_err_wde(data)                     ((0x00010000&(data))>>16)
#define VGIP_CHN1_WDE_get_dummy18022240_15_3(data)                    ((0x0000FFF8&(data))>>3)
#define VGIP_CHN1_WDE_get_ch1_vs_err_ie(data)                         ((0x00000004&(data))>>2)
#define VGIP_CHN1_WDE_get_ch1_hs_err_ie(data)                         ((0x00000002&(data))>>1)
#define VGIP_CHN1_WDE_get_ch1_field_err_ie(data)                      (0x00000001&(data))

#define VGIP_CHN2_CTRL                                                0x18022244
#define VGIP_CHN2_CTRL_reg_addr                                       "0xB8022244"
#define VGIP_CHN2_CTRL_reg                                            0xB8022244
#define VGIP_CHN2_CTRL_inst_addr                                      "0x0091"
#define VGIP_CHN2_CTRL_inst                                           0x0091
#define VGIP_CHN2_CTRL_ch2_ivrun_shift                                (31)
#define VGIP_CHN2_CTRL_ch2_in_sel_shift                               (28)
#define VGIP_CHN2_CTRL_ch2_random_en_shift                            (27)
#define VGIP_CHN2_CTRL_ch2_fdrop_en_shift                             (26)
#define VGIP_CHN2_CTRL_ch2_vact_end_ie_shift                          (25)
#define VGIP_CHN2_CTRL_ch2_vact_start_ie_shift                        (24)
#define VGIP_CHN2_CTRL_ch2_field_hs_inv_shift                         (23)
#define VGIP_CHN2_CTRL_ch2_odd_vd_inv_shift                           (22)
#define VGIP_CHN2_CTRL_ch2_odd_vd_en_shift                            (21)
#define VGIP_CHN2_CTRL_ch2_odd_inv_shift                              (18)
#define VGIP_CHN2_CTRL_ch2_odd_sync_edge_shift                        (17)
#define VGIP_CHN2_CTRL_ch2_odd_det_en_shift                           (16)
#define VGIP_CHN2_CTRL_ch2_hpact_is_hact_shift                        (15)
#define VGIP_CHN2_CTRL_ch2_fcap_str_odd_shift                         (14)
#define VGIP_CHN2_CTRL_ch2_mask_den_inv_shift                         (13)
#define VGIP_CHN2_CTRL_ch2_mask_den_1l_shift                          (12)
#define VGIP_CHN2_CTRL_dummy18022244_11_shift                         (11)
#define VGIP_CHN2_CTRL_ch2_safe_odd_inv_shift                         (10)
#define VGIP_CHN2_CTRL_ch2_force_tog_shift                            (9)
#define VGIP_CHN2_CTRL_ch2_safe_mode_shift                            (8)
#define VGIP_CHN2_CTRL_dummy18022244_7_shift                          (7)
#define VGIP_CHN2_CTRL_ch2_hs_syncedge_shift                          (6)
#define VGIP_CHN2_CTRL_ch2_vs_syncedge_shift                          (5)
#define VGIP_CHN2_CTRL_ch2_vs_by_hs_en_n_shift                        (4)
#define VGIP_CHN2_CTRL_ch2_vs_inv_shift                               (3)
#define VGIP_CHN2_CTRL_ch2_hs_inv_shift                               (2)
#define VGIP_CHN2_CTRL_ch2_digital_mode_shift                         (1)
#define VGIP_CHN2_CTRL_ch2_in_clk_en_shift                            (0)
#define VGIP_CHN2_CTRL_ch2_ivrun_mask                                 (0x80000000)
#define VGIP_CHN2_CTRL_ch2_in_sel_mask                                (0x70000000)
#define VGIP_CHN2_CTRL_ch2_random_en_mask                             (0x08000000)
#define VGIP_CHN2_CTRL_ch2_fdrop_en_mask                              (0x04000000)
#define VGIP_CHN2_CTRL_ch2_vact_end_ie_mask                           (0x02000000)
#define VGIP_CHN2_CTRL_ch2_vact_start_ie_mask                         (0x01000000)
#define VGIP_CHN2_CTRL_ch2_field_hs_inv_mask                          (0x00800000)
#define VGIP_CHN2_CTRL_ch2_odd_vd_inv_mask                            (0x00400000)
#define VGIP_CHN2_CTRL_ch2_odd_vd_en_mask                             (0x00200000)
#define VGIP_CHN2_CTRL_ch2_odd_inv_mask                               (0x00040000)
#define VGIP_CHN2_CTRL_ch2_odd_sync_edge_mask                         (0x00020000)
#define VGIP_CHN2_CTRL_ch2_odd_det_en_mask                            (0x00010000)
#define VGIP_CHN2_CTRL_ch2_hpact_is_hact_mask                         (0x00008000)
#define VGIP_CHN2_CTRL_ch2_fcap_str_odd_mask                          (0x00004000)
#define VGIP_CHN2_CTRL_ch2_mask_den_inv_mask                          (0x00002000)
#define VGIP_CHN2_CTRL_ch2_mask_den_1l_mask                           (0x00001000)
#define VGIP_CHN2_CTRL_dummy18022244_11_mask                          (0x00000800)
#define VGIP_CHN2_CTRL_ch2_safe_odd_inv_mask                          (0x00000400)
#define VGIP_CHN2_CTRL_ch2_force_tog_mask                             (0x00000200)
#define VGIP_CHN2_CTRL_ch2_safe_mode_mask                             (0x00000100)
#define VGIP_CHN2_CTRL_dummy18022244_7_mask                           (0x00000080)
#define VGIP_CHN2_CTRL_ch2_hs_syncedge_mask                           (0x00000040)
#define VGIP_CHN2_CTRL_ch2_vs_syncedge_mask                           (0x00000020)
#define VGIP_CHN2_CTRL_ch2_vs_by_hs_en_n_mask                         (0x00000010)
#define VGIP_CHN2_CTRL_ch2_vs_inv_mask                                (0x00000008)
#define VGIP_CHN2_CTRL_ch2_hs_inv_mask                                (0x00000004)
#define VGIP_CHN2_CTRL_ch2_digital_mode_mask                          (0x00000002)
#define VGIP_CHN2_CTRL_ch2_in_clk_en_mask                             (0x00000001)
#define VGIP_CHN2_CTRL_ch2_ivrun(data)                                (0x80000000&(((UINT32)data)<<31))
#define VGIP_CHN2_CTRL_ch2_in_sel(data)                               (0x70000000&(((UINT32)data)<<28))
#define VGIP_CHN2_CTRL_ch2_random_en(data)                            (0x08000000&(((UINT32)data)<<27))
#define VGIP_CHN2_CTRL_ch2_fdrop_en(data)                             (0x04000000&(((UINT32)data)<<26))
#define VGIP_CHN2_CTRL_ch2_vact_end_ie(data)                          (0x02000000&(((UINT32)data)<<25))
#define VGIP_CHN2_CTRL_ch2_vact_start_ie(data)                        (0x01000000&(((UINT32)data)<<24))
#define VGIP_CHN2_CTRL_ch2_field_hs_inv(data)                         (0x00800000&(((UINT32)data)<<23))
#define VGIP_CHN2_CTRL_ch2_odd_vd_inv(data)                           (0x00400000&(((UINT32)data)<<22))
#define VGIP_CHN2_CTRL_ch2_odd_vd_en(data)                            (0x00200000&(((UINT32)data)<<21))
#define VGIP_CHN2_CTRL_ch2_odd_inv(data)                              (0x00040000&(((UINT32)data)<<18))
#define VGIP_CHN2_CTRL_ch2_odd_sync_edge(data)                        (0x00020000&(((UINT32)data)<<17))
#define VGIP_CHN2_CTRL_ch2_odd_det_en(data)                           (0x00010000&(((UINT32)data)<<16))
#define VGIP_CHN2_CTRL_ch2_hpact_is_hact(data)                        (0x00008000&(((UINT32)data)<<15))
#define VGIP_CHN2_CTRL_ch2_fcap_str_odd(data)                         (0x00004000&(((UINT32)data)<<14))
#define VGIP_CHN2_CTRL_ch2_mask_den_inv(data)                         (0x00002000&(((UINT32)data)<<13))
#define VGIP_CHN2_CTRL_ch2_mask_den_1l(data)                          (0x00001000&(((UINT32)data)<<12))
#define VGIP_CHN2_CTRL_dummy18022244_11(data)                         (0x00000800&(((UINT32)data)<<11))
#define VGIP_CHN2_CTRL_ch2_safe_odd_inv(data)                         (0x00000400&(((UINT32)data)<<10))
#define VGIP_CHN2_CTRL_ch2_force_tog(data)                            (0x00000200&(((UINT32)data)<<9))
#define VGIP_CHN2_CTRL_ch2_safe_mode(data)                            (0x00000100&(((UINT32)data)<<8))
#define VGIP_CHN2_CTRL_dummy18022244_7(data)                          (0x00000080&(((UINT32)data)<<7))
#define VGIP_CHN2_CTRL_ch2_hs_syncedge(data)                          (0x00000040&(((UINT32)data)<<6))
#define VGIP_CHN2_CTRL_ch2_vs_syncedge(data)                          (0x00000020&(((UINT32)data)<<5))
#define VGIP_CHN2_CTRL_ch2_vs_by_hs_en_n(data)                        (0x00000010&(((UINT32)data)<<4))
#define VGIP_CHN2_CTRL_ch2_vs_inv(data)                               (0x00000008&(((UINT32)data)<<3))
#define VGIP_CHN2_CTRL_ch2_hs_inv(data)                               (0x00000004&(((UINT32)data)<<2))
#define VGIP_CHN2_CTRL_ch2_digital_mode(data)                         (0x00000002&(((UINT32)data)<<1))
#define VGIP_CHN2_CTRL_ch2_in_clk_en(data)                            (0x00000001&(data))
#define VGIP_CHN2_CTRL_get_ch2_ivrun(data)                            ((0x80000000&(data))>>31)
#define VGIP_CHN2_CTRL_get_ch2_in_sel(data)                           ((0x70000000&(data))>>28)
#define VGIP_CHN2_CTRL_get_ch2_random_en(data)                        ((0x08000000&(data))>>27)
#define VGIP_CHN2_CTRL_get_ch2_fdrop_en(data)                         ((0x04000000&(data))>>26)
#define VGIP_CHN2_CTRL_get_ch2_vact_end_ie(data)                      ((0x02000000&(data))>>25)
#define VGIP_CHN2_CTRL_get_ch2_vact_start_ie(data)                    ((0x01000000&(data))>>24)
#define VGIP_CHN2_CTRL_get_ch2_field_hs_inv(data)                     ((0x00800000&(data))>>23)
#define VGIP_CHN2_CTRL_get_ch2_odd_vd_inv(data)                       ((0x00400000&(data))>>22)
#define VGIP_CHN2_CTRL_get_ch2_odd_vd_en(data)                        ((0x00200000&(data))>>21)
#define VGIP_CHN2_CTRL_get_ch2_odd_inv(data)                          ((0x00040000&(data))>>18)
#define VGIP_CHN2_CTRL_get_ch2_odd_sync_edge(data)                    ((0x00020000&(data))>>17)
#define VGIP_CHN2_CTRL_get_ch2_odd_det_en(data)                       ((0x00010000&(data))>>16)
#define VGIP_CHN2_CTRL_get_ch2_hpact_is_hact(data)                    ((0x00008000&(data))>>15)
#define VGIP_CHN2_CTRL_get_ch2_fcap_str_odd(data)                     ((0x00004000&(data))>>14)
#define VGIP_CHN2_CTRL_get_ch2_mask_den_inv(data)                     ((0x00002000&(data))>>13)
#define VGIP_CHN2_CTRL_get_ch2_mask_den_1l(data)                      ((0x00001000&(data))>>12)
#define VGIP_CHN2_CTRL_get_dummy18022244_11(data)                     ((0x00000800&(data))>>11)
#define VGIP_CHN2_CTRL_get_ch2_safe_odd_inv(data)                     ((0x00000400&(data))>>10)
#define VGIP_CHN2_CTRL_get_ch2_force_tog(data)                        ((0x00000200&(data))>>9)
#define VGIP_CHN2_CTRL_get_ch2_safe_mode(data)                        ((0x00000100&(data))>>8)
#define VGIP_CHN2_CTRL_get_dummy18022244_7(data)                      ((0x00000080&(data))>>7)
#define VGIP_CHN2_CTRL_get_ch2_hs_syncedge(data)                      ((0x00000040&(data))>>6)
#define VGIP_CHN2_CTRL_get_ch2_vs_syncedge(data)                      ((0x00000020&(data))>>5)
#define VGIP_CHN2_CTRL_get_ch2_vs_by_hs_en_n(data)                    ((0x00000010&(data))>>4)
#define VGIP_CHN2_CTRL_get_ch2_vs_inv(data)                           ((0x00000008&(data))>>3)
#define VGIP_CHN2_CTRL_get_ch2_hs_inv(data)                           ((0x00000004&(data))>>2)
#define VGIP_CHN2_CTRL_get_ch2_digital_mode(data)                     ((0x00000002&(data))>>1)
#define VGIP_CHN2_CTRL_get_ch2_in_clk_en(data)                        (0x00000001&(data))

#define VGIP_CHN2_STATUS                                              0x18022248
#define VGIP_CHN2_STATUS_reg_addr                                     "0xB8022248"
#define VGIP_CHN2_STATUS_reg                                          0xB8022248
#define VGIP_CHN2_STATUS_inst_addr                                    "0x0092"
#define VGIP_CHN2_STATUS_inst                                         0x0092
#define VGIP_CHN2_STATUS_ch2_vs_end_shift                             (27)
#define VGIP_CHN2_STATUS_ch2_vs_start_shift                           (26)
#define VGIP_CHN2_STATUS_ch2_vact_end_shift                           (25)
#define VGIP_CHN2_STATUS_ch2_vact_start_shift                         (24)
#define VGIP_CHN2_STATUS_dummy18022248_23_15_shift                    (15)
#define VGIP_CHN2_STATUS_ch2_vs_err_shift                             (14)
#define VGIP_CHN2_STATUS_ch2_hs_err_shift                             (13)
#define VGIP_CHN2_STATUS_ch2_field_err_shift                          (12)
#define VGIP_CHN2_STATUS_dummy18022248_11_10_shift                    (10)
#define VGIP_CHN2_STATUS_ch2_vs_tog_shift                             (9)
#define VGIP_CHN2_STATUS_ch2_hs_tog_shift                             (8)
#define VGIP_CHN2_STATUS_ch2_field_pol_shift                          (7)
#define VGIP_CHN2_STATUS_ch2_field_tog_shift                          (6)
#define VGIP_CHN2_STATUS_ch2_field_vs_lsb_shift                       (4)
#define VGIP_CHN2_STATUS_ch2_more_1line_field_shift                   (3)
#define VGIP_CHN2_STATUS_ch2_bounce_status_2_shift                    (2)
#define VGIP_CHN2_STATUS_ch2_bounce_status_1_shift                    (1)
#define VGIP_CHN2_STATUS_ch2_bounce_status_0_shift                    (0)
#define VGIP_CHN2_STATUS_ch2_vs_end_mask                              (0x08000000)
#define VGIP_CHN2_STATUS_ch2_vs_start_mask                            (0x04000000)
#define VGIP_CHN2_STATUS_ch2_vact_end_mask                            (0x02000000)
#define VGIP_CHN2_STATUS_ch2_vact_start_mask                          (0x01000000)
#define VGIP_CHN2_STATUS_dummy18022248_23_15_mask                     (0x00FF8000)
#define VGIP_CHN2_STATUS_ch2_vs_err_mask                              (0x00004000)
#define VGIP_CHN2_STATUS_ch2_hs_err_mask                              (0x00002000)
#define VGIP_CHN2_STATUS_ch2_field_err_mask                           (0x00001000)
#define VGIP_CHN2_STATUS_dummy18022248_11_10_mask                     (0x00000C00)
#define VGIP_CHN2_STATUS_ch2_vs_tog_mask                              (0x00000200)
#define VGIP_CHN2_STATUS_ch2_hs_tog_mask                              (0x00000100)
#define VGIP_CHN2_STATUS_ch2_field_pol_mask                           (0x00000080)
#define VGIP_CHN2_STATUS_ch2_field_tog_mask                           (0x00000040)
#define VGIP_CHN2_STATUS_ch2_field_vs_lsb_mask                        (0x00000030)
#define VGIP_CHN2_STATUS_ch2_more_1line_field_mask                    (0x00000008)
#define VGIP_CHN2_STATUS_ch2_bounce_status_2_mask                     (0x00000004)
#define VGIP_CHN2_STATUS_ch2_bounce_status_1_mask                     (0x00000002)
#define VGIP_CHN2_STATUS_ch2_bounce_status_0_mask                     (0x00000001)
#define VGIP_CHN2_STATUS_ch2_vs_end(data)                             (0x08000000&(((UINT32)data)<<27))
#define VGIP_CHN2_STATUS_ch2_vs_start(data)                           (0x04000000&(((UINT32)data)<<26))
#define VGIP_CHN2_STATUS_ch2_vact_end(data)                           (0x02000000&(((UINT32)data)<<25))
#define VGIP_CHN2_STATUS_ch2_vact_start(data)                         (0x01000000&(((UINT32)data)<<24))
#define VGIP_CHN2_STATUS_dummy18022248_23_15(data)                    (0x00FF8000&(((UINT32)data)<<15))
#define VGIP_CHN2_STATUS_ch2_vs_err(data)                             (0x00004000&(((UINT32)data)<<14))
#define VGIP_CHN2_STATUS_ch2_hs_err(data)                             (0x00002000&(((UINT32)data)<<13))
#define VGIP_CHN2_STATUS_ch2_field_err(data)                          (0x00001000&(((UINT32)data)<<12))
#define VGIP_CHN2_STATUS_dummy18022248_11_10(data)                    (0x00000C00&(((UINT32)data)<<10))
#define VGIP_CHN2_STATUS_ch2_vs_tog(data)                             (0x00000200&(((UINT32)data)<<9))
#define VGIP_CHN2_STATUS_ch2_hs_tog(data)                             (0x00000100&(((UINT32)data)<<8))
#define VGIP_CHN2_STATUS_ch2_field_pol(data)                          (0x00000080&(((UINT32)data)<<7))
#define VGIP_CHN2_STATUS_ch2_field_tog(data)                          (0x00000040&(((UINT32)data)<<6))
#define VGIP_CHN2_STATUS_ch2_field_vs_lsb(data)                       (0x00000030&(((UINT32)data)<<4))
#define VGIP_CHN2_STATUS_ch2_more_1line_field(data)                   (0x00000008&(((UINT32)data)<<3))
#define VGIP_CHN2_STATUS_ch2_bounce_status_2(data)                    (0x00000004&(((UINT32)data)<<2))
#define VGIP_CHN2_STATUS_ch2_bounce_status_1(data)                    (0x00000002&(((UINT32)data)<<1))
#define VGIP_CHN2_STATUS_ch2_bounce_status_0(data)                    (0x00000001&(data))
#define VGIP_CHN2_STATUS_get_ch2_vs_end(data)                         ((0x08000000&(data))>>27)
#define VGIP_CHN2_STATUS_get_ch2_vs_start(data)                       ((0x04000000&(data))>>26)
#define VGIP_CHN2_STATUS_get_ch2_vact_end(data)                       ((0x02000000&(data))>>25)
#define VGIP_CHN2_STATUS_get_ch2_vact_start(data)                     ((0x01000000&(data))>>24)
#define VGIP_CHN2_STATUS_get_dummy18022248_23_15(data)                ((0x00FF8000&(data))>>15)
#define VGIP_CHN2_STATUS_get_ch2_vs_err(data)                         ((0x00004000&(data))>>14)
#define VGIP_CHN2_STATUS_get_ch2_hs_err(data)                         ((0x00002000&(data))>>13)
#define VGIP_CHN2_STATUS_get_ch2_field_err(data)                      ((0x00001000&(data))>>12)
#define VGIP_CHN2_STATUS_get_dummy18022248_11_10(data)                ((0x00000C00&(data))>>10)
#define VGIP_CHN2_STATUS_get_ch2_vs_tog(data)                         ((0x00000200&(data))>>9)
#define VGIP_CHN2_STATUS_get_ch2_hs_tog(data)                         ((0x00000100&(data))>>8)
#define VGIP_CHN2_STATUS_get_ch2_field_pol(data)                      ((0x00000080&(data))>>7)
#define VGIP_CHN2_STATUS_get_ch2_field_tog(data)                      ((0x00000040&(data))>>6)
#define VGIP_CHN2_STATUS_get_ch2_field_vs_lsb(data)                   ((0x00000030&(data))>>4)
#define VGIP_CHN2_STATUS_get_ch2_more_1line_field(data)               ((0x00000008&(data))>>3)
#define VGIP_CHN2_STATUS_get_ch2_bounce_status_2(data)                ((0x00000004&(data))>>2)
#define VGIP_CHN2_STATUS_get_ch2_bounce_status_1(data)                ((0x00000002&(data))>>1)
#define VGIP_CHN2_STATUS_get_ch2_bounce_status_0(data)                (0x00000001&(data))

#define VGIP_CHN2_ACT_HSTA_WIDTH                                      0x1802224c
#define VGIP_CHN2_ACT_HSTA_WIDTH_reg_addr                             "0xB802224C"
#define VGIP_CHN2_ACT_HSTA_WIDTH_reg                                  0xB802224C
#define VGIP_CHN2_ACT_HSTA_WIDTH_inst_addr                            "0x0093"
#define VGIP_CHN2_ACT_HSTA_WIDTH_inst                                 0x0093
#define VGIP_CHN2_ACT_HSTA_WIDTH_ch2_db_rdy_shift                     (31)
#define VGIP_CHN2_ACT_HSTA_WIDTH_ch2_db_en_shift                      (30)
#define VGIP_CHN2_ACT_HSTA_WIDTH_ch2_ih_act_sta_shift                 (16)
#define VGIP_CHN2_ACT_HSTA_WIDTH_ch2_ih_act_wid_shift                 (0)
#define VGIP_CHN2_ACT_HSTA_WIDTH_ch2_db_rdy_mask                      (0x80000000)
#define VGIP_CHN2_ACT_HSTA_WIDTH_ch2_db_en_mask                       (0x40000000)
#define VGIP_CHN2_ACT_HSTA_WIDTH_ch2_ih_act_sta_mask                  (0x0FFF0000)
#define VGIP_CHN2_ACT_HSTA_WIDTH_ch2_ih_act_wid_mask                  (0x00000FFF)
#define VGIP_CHN2_ACT_HSTA_WIDTH_ch2_db_rdy(data)                     (0x80000000&(((UINT32)data)<<31))
#define VGIP_CHN2_ACT_HSTA_WIDTH_ch2_db_en(data)                      (0x40000000&(((UINT32)data)<<30))
#define VGIP_CHN2_ACT_HSTA_WIDTH_ch2_ih_act_sta(data)                 (0x0FFF0000&(((UINT32)data)<<16))
#define VGIP_CHN2_ACT_HSTA_WIDTH_ch2_ih_act_wid(data)                 (0x00000FFF&(data))
#define VGIP_CHN2_ACT_HSTA_WIDTH_get_ch2_db_rdy(data)                 ((0x80000000&(data))>>31)
#define VGIP_CHN2_ACT_HSTA_WIDTH_get_ch2_db_en(data)                  ((0x40000000&(data))>>30)
#define VGIP_CHN2_ACT_HSTA_WIDTH_get_ch2_ih_act_sta(data)             ((0x0FFF0000&(data))>>16)
#define VGIP_CHN2_ACT_HSTA_WIDTH_get_ch2_ih_act_wid(data)             (0x00000FFF&(data))

#define VGIP_CHN2_ACT_VSTA_LENGTH                                     0x18022250
#define VGIP_CHN2_ACT_VSTA_LENGTH_reg_addr                            "0xB8022250"
#define VGIP_CHN2_ACT_VSTA_LENGTH_reg                                 0xB8022250
#define VGIP_CHN2_ACT_VSTA_LENGTH_inst_addr                           "0x0094"
#define VGIP_CHN2_ACT_VSTA_LENGTH_inst                                0x0094
#define VGIP_CHN2_ACT_VSTA_LENGTH_ch2_iv_act_sta_shift                (16)
#define VGIP_CHN2_ACT_VSTA_LENGTH_ch2_iv_act_len_shift                (0)
#define VGIP_CHN2_ACT_VSTA_LENGTH_ch2_iv_act_sta_mask                 (0x0FFF0000)
#define VGIP_CHN2_ACT_VSTA_LENGTH_ch2_iv_act_len_mask                 (0x00000FFF)
#define VGIP_CHN2_ACT_VSTA_LENGTH_ch2_iv_act_sta(data)                (0x0FFF0000&(((UINT32)data)<<16))
#define VGIP_CHN2_ACT_VSTA_LENGTH_ch2_iv_act_len(data)                (0x00000FFF&(data))
#define VGIP_CHN2_ACT_VSTA_LENGTH_get_ch2_iv_act_sta(data)            ((0x0FFF0000&(data))>>16)
#define VGIP_CHN2_ACT_VSTA_LENGTH_get_ch2_iv_act_len(data)            (0x00000FFF&(data))

#define VGIP_CHN2_DELAY                                               0x18022254
#define VGIP_CHN2_DELAY_reg_addr                                      "0xB8022254"
#define VGIP_CHN2_DELAY_reg                                           0xB8022254
#define VGIP_CHN2_DELAY_inst_addr                                     "0x0095"
#define VGIP_CHN2_DELAY_inst                                          0x0095
#define VGIP_CHN2_DELAY_ch2_ihs_dly_shift                             (16)
#define VGIP_CHN2_DELAY_ch2_ivs_dly_shift                             (0)
#define VGIP_CHN2_DELAY_ch2_ihs_dly_mask                              (0x0FFF0000)
#define VGIP_CHN2_DELAY_ch2_ivs_dly_mask                              (0x00000FFF)
#define VGIP_CHN2_DELAY_ch2_ihs_dly(data)                             (0x0FFF0000&(((UINT32)data)<<16))
#define VGIP_CHN2_DELAY_ch2_ivs_dly(data)                             (0x00000FFF&(data))
#define VGIP_CHN2_DELAY_get_ch2_ihs_dly(data)                         ((0x0FFF0000&(data))>>16)
#define VGIP_CHN2_DELAY_get_ch2_ivs_dly(data)                         (0x00000FFF&(data))

#define VGIP_CHN2_MISC                                                0x18022258
#define VGIP_CHN2_MISC_reg_addr                                       "0xB8022258"
#define VGIP_CHN2_MISC_reg                                            0xB8022258
#define VGIP_CHN2_MISC_inst_addr                                      "0x0096"
#define VGIP_CHN2_MISC_inst                                           0x0096
#define VGIP_CHN2_MISC_ch2_de_mask_en_shift                           (31)
#define VGIP_CHN2_MISC_ch2_hporch_num_shift                           (8)
#define VGIP_CHN2_MISC_ch2_auto_ihs_dly_shift                         (6)
#define VGIP_CHN2_MISC_ch2_auto_ivs_dly_shift                         (4)
#define VGIP_CHN2_MISC_ch2_hdly_one_shift                             (3)
#define VGIP_CHN2_MISC_ch2_de_mask_en_mask                            (0x80000000)
#define VGIP_CHN2_MISC_ch2_hporch_num_mask                            (0x0007FF00)
#define VGIP_CHN2_MISC_ch2_auto_ihs_dly_mask                          (0x000000C0)
#define VGIP_CHN2_MISC_ch2_auto_ivs_dly_mask                          (0x00000030)
#define VGIP_CHN2_MISC_ch2_hdly_one_mask                              (0x00000008)
#define VGIP_CHN2_MISC_ch2_de_mask_en(data)                           (0x80000000&(((UINT32)data)<<31))
#define VGIP_CHN2_MISC_ch2_hporch_num(data)                           (0x0007FF00&(((UINT32)data)<<8))
#define VGIP_CHN2_MISC_ch2_auto_ihs_dly(data)                         (0x000000C0&(((UINT32)data)<<6))
#define VGIP_CHN2_MISC_ch2_auto_ivs_dly(data)                         (0x00000030&(((UINT32)data)<<4))
#define VGIP_CHN2_MISC_ch2_hdly_one(data)                             (0x00000008&(((UINT32)data)<<3))
#define VGIP_CHN2_MISC_get_ch2_de_mask_en(data)                       ((0x80000000&(data))>>31)
#define VGIP_CHN2_MISC_get_ch2_hporch_num(data)                       ((0x0007FF00&(data))>>8)
#define VGIP_CHN2_MISC_get_ch2_auto_ihs_dly(data)                     ((0x000000C0&(data))>>6)
#define VGIP_CHN2_MISC_get_ch2_auto_ivs_dly(data)                     ((0x00000030&(data))>>4)
#define VGIP_CHN2_MISC_get_ch2_hdly_one(data)                         ((0x00000008&(data))>>3)

#define VGIP_CHN2_PTN_H_VI                                            0x1802225c
#define VGIP_CHN2_PTN_H_VI_reg_addr                                   "0xB802225C"
#define VGIP_CHN2_PTN_H_VI_reg                                        0xB802225C
#define VGIP_CHN2_PTN_H_VI_inst_addr                                  "0x0097"
#define VGIP_CHN2_PTN_H_VI_inst                                       0x0097
#define VGIP_CHN2_PTN_H_VI_ch2_captst_en_shift                        (31)
#define VGIP_CHN2_PTN_H_VI_ch2_captst_mode_shift                      (30)
#define VGIP_CHN2_PTN_H_VI_ch2_vcaptst_num_shift                      (16)
#define VGIP_CHN2_PTN_H_VI_ch2_hcaptst_num_shift                      (0)
#define VGIP_CHN2_PTN_H_VI_ch2_captst_en_mask                         (0x80000000)
#define VGIP_CHN2_PTN_H_VI_ch2_captst_mode_mask                       (0x40000000)
#define VGIP_CHN2_PTN_H_VI_ch2_vcaptst_num_mask                       (0x0FFF0000)
#define VGIP_CHN2_PTN_H_VI_ch2_hcaptst_num_mask                       (0x00000FFF)
#define VGIP_CHN2_PTN_H_VI_ch2_captst_en(data)                        (0x80000000&(((UINT32)data)<<31))
#define VGIP_CHN2_PTN_H_VI_ch2_captst_mode(data)                      (0x40000000&(((UINT32)data)<<30))
#define VGIP_CHN2_PTN_H_VI_ch2_vcaptst_num(data)                      (0x0FFF0000&(((UINT32)data)<<16))
#define VGIP_CHN2_PTN_H_VI_ch2_hcaptst_num(data)                      (0x00000FFF&(data))
#define VGIP_CHN2_PTN_H_VI_get_ch2_captst_en(data)                    ((0x80000000&(data))>>31)
#define VGIP_CHN2_PTN_H_VI_get_ch2_captst_mode(data)                  ((0x40000000&(data))>>30)
#define VGIP_CHN2_PTN_H_VI_get_ch2_vcaptst_num(data)                  ((0x0FFF0000&(data))>>16)
#define VGIP_CHN2_PTN_H_VI_get_ch2_hcaptst_num(data)                  (0x00000FFF&(data))

#define VGIP_CHN2_PTN0                                                0x18022260
#define VGIP_CHN2_PTN0_reg_addr                                       "0xB8022260"
#define VGIP_CHN2_PTN0_reg                                            0xB8022260
#define VGIP_CHN2_PTN0_inst_addr                                      "0x0098"
#define VGIP_CHN2_PTN0_inst                                           0x0098
#define VGIP_CHN2_PTN0_ch2_captst_r_data_shift                        (16)
#define VGIP_CHN2_PTN0_ch2_captst_g_data_shift                        (0)
#define VGIP_CHN2_PTN0_ch2_captst_r_data_mask                         (0x00FF0000)
#define VGIP_CHN2_PTN0_ch2_captst_g_data_mask                         (0x000000FF)
#define VGIP_CHN2_PTN0_ch2_captst_r_data(data)                        (0x00FF0000&(((UINT32)data)<<16))
#define VGIP_CHN2_PTN0_ch2_captst_g_data(data)                        (0x000000FF&(data))
#define VGIP_CHN2_PTN0_get_ch2_captst_r_data(data)                    ((0x00FF0000&(data))>>16)
#define VGIP_CHN2_PTN0_get_ch2_captst_g_data(data)                    (0x000000FF&(data))

#define VGIP_CHN2_PTN1                                                0x18022264
#define VGIP_CHN2_PTN1_reg_addr                                       "0xB8022264"
#define VGIP_CHN2_PTN1_reg                                            0xB8022264
#define VGIP_CHN2_PTN1_inst_addr                                      "0x0099"
#define VGIP_CHN2_PTN1_inst                                           0x0099
#define VGIP_CHN2_PTN1_ch2_captst_b_data_shift                        (0)
#define VGIP_CHN2_PTN1_ch2_captst_b_data_mask                         (0x000000FF)
#define VGIP_CHN2_PTN1_ch2_captst_b_data(data)                        (0x000000FF&(data))
#define VGIP_CHN2_PTN1_get_ch2_captst_b_data(data)                    (0x000000FF&(data))

#define VGIP_CHN2_LC                                                  0x18022268
#define VGIP_CHN2_LC_reg_addr                                         "0xB8022268"
#define VGIP_CHN2_LC_reg                                              0xB8022268
#define VGIP_CHN2_LC_inst_addr                                        "0x009A"
#define VGIP_CHN2_LC_inst                                             0x009A
#define VGIP_CHN2_LC_ch2_vlcen_shift                                  (31)
#define VGIP_CHN2_LC_ch2_vlc_mode_shift                               (30)
#define VGIP_CHN2_LC_ch2_vlc_ie_shift                                 (29)
#define VGIP_CHN2_LC_dummy18022268_28_shift                           (28)
#define VGIP_CHN2_LC_ch2_vlc_status_shift                             (27)
#define VGIP_CHN2_LC_dummy18022268_15_11_shift                        (11)
#define VGIP_CHN2_LC_ch2_vln_shift                                    (0)
#define VGIP_CHN2_LC_ch2_vlcen_mask                                   (0x80000000)
#define VGIP_CHN2_LC_ch2_vlc_mode_mask                                (0x40000000)
#define VGIP_CHN2_LC_ch2_vlc_ie_mask                                  (0x20000000)
#define VGIP_CHN2_LC_dummy18022268_28_mask                            (0x10000000)
#define VGIP_CHN2_LC_ch2_vlc_status_mask                              (0x08000000)
#define VGIP_CHN2_LC_dummy18022268_15_11_mask                         (0x0000F800)
#define VGIP_CHN2_LC_ch2_vln_mask                                     (0x000007FF)
#define VGIP_CHN2_LC_ch2_vlcen(data)                                  (0x80000000&(((UINT32)data)<<31))
#define VGIP_CHN2_LC_ch2_vlc_mode(data)                               (0x40000000&(((UINT32)data)<<30))
#define VGIP_CHN2_LC_ch2_vlc_ie(data)                                 (0x20000000&(((UINT32)data)<<29))
#define VGIP_CHN2_LC_dummy18022268_28(data)                           (0x10000000&(((UINT32)data)<<28))
#define VGIP_CHN2_LC_ch2_vlc_status(data)                             (0x08000000&(((UINT32)data)<<27))
#define VGIP_CHN2_LC_dummy18022268_15_11(data)                        (0x0000F800&(((UINT32)data)<<11))
#define VGIP_CHN2_LC_ch2_vln(data)                                    (0x000007FF&(data))
#define VGIP_CHN2_LC_get_ch2_vlcen(data)                              ((0x80000000&(data))>>31)
#define VGIP_CHN2_LC_get_ch2_vlc_mode(data)                           ((0x40000000&(data))>>30)
#define VGIP_CHN2_LC_get_ch2_vlc_ie(data)                             ((0x20000000&(data))>>29)
#define VGIP_CHN2_LC_get_dummy18022268_28(data)                       ((0x10000000&(data))>>28)
#define VGIP_CHN2_LC_get_ch2_vlc_status(data)                         ((0x08000000&(data))>>27)
#define VGIP_CHN2_LC_get_dummy18022268_15_11(data)                    ((0x0000F800&(data))>>11)
#define VGIP_CHN2_LC_get_ch2_vln(data)                                (0x000007FF&(data))

#define ICH2_VGIP_CRC_CTRL                                            0x1802226c
#define ICH2_VGIP_CRC_CTRL_reg_addr                                   "0xB802226C"
#define ICH2_VGIP_CRC_CTRL_reg                                        0xB802226C
#define ICH2_VGIP_CRC_CTRL_inst_addr                                  "0x009B"
#define ICH2_VGIP_CRC_CTRL_inst                                       0x009B
#define ICH2_VGIP_CRC_CTRL_ch2_crc_conti_shift                        (1)
#define ICH2_VGIP_CRC_CTRL_ch2_crc_start_shift                        (0)
#define ICH2_VGIP_CRC_CTRL_ch2_crc_conti_mask                         (0x00000002)
#define ICH2_VGIP_CRC_CTRL_ch2_crc_start_mask                         (0x00000001)
#define ICH2_VGIP_CRC_CTRL_ch2_crc_conti(data)                        (0x00000002&(((UINT32)data)<<1))
#define ICH2_VGIP_CRC_CTRL_ch2_crc_start(data)                        (0x00000001&(data))
#define ICH2_VGIP_CRC_CTRL_get_ch2_crc_conti(data)                    ((0x00000002&(data))>>1)
#define ICH2_VGIP_CRC_CTRL_get_ch2_crc_start(data)                    (0x00000001&(data))

#define ICH2_VGIP_CRC_RESULT                                          0x18022270
#define ICH2_VGIP_CRC_RESULT_reg_addr                                 "0xB8022270"
#define ICH2_VGIP_CRC_RESULT_reg                                      0xB8022270
#define ICH2_VGIP_CRC_RESULT_inst_addr                                "0x009C"
#define ICH2_VGIP_CRC_RESULT_inst                                     0x009C
#define ICH2_VGIP_CRC_RESULT_ch2_crc_result_shift                     (0)
#define ICH2_VGIP_CRC_RESULT_ch2_crc_result_mask                      (0xFFFFFFFF)
#define ICH2_VGIP_CRC_RESULT_ch2_crc_result(data)                     (0xFFFFFFFF&(data))
#define ICH2_VGIP_CRC_RESULT_get_ch2_crc_result(data)                 (0xFFFFFFFF&(data))

#define VGIP_CHN2_WDE                                                 0x18022274
#define VGIP_CHN2_WDE_reg_addr                                        "0xB8022274"
#define VGIP_CHN2_WDE_reg                                             0xB8022274
#define VGIP_CHN2_WDE_inst_addr                                       "0x009D"
#define VGIP_CHN2_WDE_inst                                            0x009D
#define VGIP_CHN2_WDE_ch2_wd_to_main_shift                            (31)
#define VGIP_CHN2_WDE_ch2_wd_to_sub_shift                             (30)
#define VGIP_CHN2_WDE_dummy18022274_29_19_shift                       (19)
#define VGIP_CHN2_WDE_ch2_vs_err_wde_shift                            (18)
#define VGIP_CHN2_WDE_ch2_hs_err_wde_shift                            (17)
#define VGIP_CHN2_WDE_ch2_field_err_wde_shift                         (16)
#define VGIP_CHN2_WDE_dummy18022274_15_3_shift                        (3)
#define VGIP_CHN2_WDE_ch2_vs_err_ie_shift                             (2)
#define VGIP_CHN2_WDE_ch2_hs_err_ie_shift                             (1)
#define VGIP_CHN2_WDE_ch2_field_err_ie_shift                          (0)
#define VGIP_CHN2_WDE_ch2_wd_to_main_mask                             (0x80000000)
#define VGIP_CHN2_WDE_ch2_wd_to_sub_mask                              (0x40000000)
#define VGIP_CHN2_WDE_dummy18022274_29_19_mask                        (0x3FF80000)
#define VGIP_CHN2_WDE_ch2_vs_err_wde_mask                             (0x00040000)
#define VGIP_CHN2_WDE_ch2_hs_err_wde_mask                             (0x00020000)
#define VGIP_CHN2_WDE_ch2_field_err_wde_mask                          (0x00010000)
#define VGIP_CHN2_WDE_dummy18022274_15_3_mask                         (0x0000FFF8)
#define VGIP_CHN2_WDE_ch2_vs_err_ie_mask                              (0x00000004)
#define VGIP_CHN2_WDE_ch2_hs_err_ie_mask                              (0x00000002)
#define VGIP_CHN2_WDE_ch2_field_err_ie_mask                           (0x00000001)
#define VGIP_CHN2_WDE_ch2_wd_to_main(data)                            (0x80000000&(((UINT32)data)<<31))
#define VGIP_CHN2_WDE_ch2_wd_to_sub(data)                             (0x40000000&(((UINT32)data)<<30))
#define VGIP_CHN2_WDE_dummy18022274_29_19(data)                       (0x3FF80000&(((UINT32)data)<<19))
#define VGIP_CHN2_WDE_ch2_vs_err_wde(data)                            (0x00040000&(((UINT32)data)<<18))
#define VGIP_CHN2_WDE_ch2_hs_err_wde(data)                            (0x00020000&(((UINT32)data)<<17))
#define VGIP_CHN2_WDE_ch2_field_err_wde(data)                         (0x00010000&(((UINT32)data)<<16))
#define VGIP_CHN2_WDE_dummy18022274_15_3(data)                        (0x0000FFF8&(((UINT32)data)<<3))
#define VGIP_CHN2_WDE_ch2_vs_err_ie(data)                             (0x00000004&(((UINT32)data)<<2))
#define VGIP_CHN2_WDE_ch2_hs_err_ie(data)                             (0x00000002&(((UINT32)data)<<1))
#define VGIP_CHN2_WDE_ch2_field_err_ie(data)                          (0x00000001&(data))
#define VGIP_CHN2_WDE_get_ch2_wd_to_main(data)                        ((0x80000000&(data))>>31)
#define VGIP_CHN2_WDE_get_ch2_wd_to_sub(data)                         ((0x40000000&(data))>>30)
#define VGIP_CHN2_WDE_get_dummy18022274_29_19(data)                   ((0x3FF80000&(data))>>19)
#define VGIP_CHN2_WDE_get_ch2_vs_err_wde(data)                        ((0x00040000&(data))>>18)
#define VGIP_CHN2_WDE_get_ch2_hs_err_wde(data)                        ((0x00020000&(data))>>17)
#define VGIP_CHN2_WDE_get_ch2_field_err_wde(data)                     ((0x00010000&(data))>>16)
#define VGIP_CHN2_WDE_get_dummy18022274_15_3(data)                    ((0x0000FFF8&(data))>>3)
#define VGIP_CHN2_WDE_get_ch2_vs_err_ie(data)                         ((0x00000004&(data))>>2)
#define VGIP_CHN2_WDE_get_ch2_hs_err_ie(data)                         ((0x00000002&(data))>>1)
#define VGIP_CHN2_WDE_get_ch2_field_err_ie(data)                      (0x00000001&(data))

#define RESERVED0                                                     0x18022278
#define RESERVED0_reg_addr                                            "0xB8022278"
#define RESERVED0_reg                                                 0xB8022278
#define RESERVED0_inst_addr                                           "0x009E"
#define RESERVED0_inst                                                0x009E

#define RESERVED1                                                     0x1802227c
#define RESERVED1_reg_addr                                            "0xB802227C"
#define RESERVED1_reg                                                 0xB802227C
#define RESERVED1_inst_addr                                           "0x009F"
#define RESERVED1_inst                                                0x009F

#define RESERVED2                                                     0x18022280
#define RESERVED2_reg_addr                                            "0xB8022280"
#define RESERVED2_reg                                                 0xB8022280
#define RESERVED2_inst_addr                                           "0x00A0"
#define RESERVED2_inst                                                0x00A0

#define DATA_PATH_SELECT                                              0x18022284
#define DATA_PATH_SELECT_reg_addr                                     "0xB8022284"
#define DATA_PATH_SELECT_reg                                          0xB8022284
#define DATA_PATH_SELECT_inst_addr                                    "0x00A1"
#define DATA_PATH_SELECT_inst                                         0x00A1
#define DATA_PATH_SELECT_vo_1_insel_shift                             (7)
#define DATA_PATH_SELECT_vo_2_insel_shift                             (6)
#define DATA_PATH_SELECT_fsync_ch_sel_shift                           (5)
#define DATA_PATH_SELECT_adc_ch_sel_shift                             (4)
#define DATA_PATH_SELECT_xcdi_clk_en_shift                            (3)
#define DATA_PATH_SELECT_uzd1_in_sel_shift                            (2)
#define DATA_PATH_SELECT_uzd2_in_sel_shift                            (1)
#define DATA_PATH_SELECT_xcdi_in_sel_shift                            (0)
#define DATA_PATH_SELECT_vo_1_insel_mask                              (0x00000080)
#define DATA_PATH_SELECT_vo_2_insel_mask                              (0x00000040)
#define DATA_PATH_SELECT_fsync_ch_sel_mask                            (0x00000020)
#define DATA_PATH_SELECT_adc_ch_sel_mask                              (0x00000010)
#define DATA_PATH_SELECT_xcdi_clk_en_mask                             (0x00000008)
#define DATA_PATH_SELECT_uzd1_in_sel_mask                             (0x00000004)
#define DATA_PATH_SELECT_uzd2_in_sel_mask                             (0x00000002)
#define DATA_PATH_SELECT_xcdi_in_sel_mask                             (0x00000001)
#define DATA_PATH_SELECT_vo_1_insel(data)                             (0x00000080&(((UINT32)data)<<7))
#define DATA_PATH_SELECT_vo_2_insel(data)                             (0x00000040&(((UINT32)data)<<6))
#define DATA_PATH_SELECT_fsync_ch_sel(data)                           (0x00000020&(((UINT32)data)<<5))
#define DATA_PATH_SELECT_adc_ch_sel(data)                             (0x00000010&(((UINT32)data)<<4))
#define DATA_PATH_SELECT_xcdi_clk_en(data)                            (0x00000008&(((UINT32)data)<<3))
#define DATA_PATH_SELECT_uzd1_in_sel(data)                            (0x00000004&(((UINT32)data)<<2))
#define DATA_PATH_SELECT_uzd2_in_sel(data)                            (0x00000002&(((UINT32)data)<<1))
#define DATA_PATH_SELECT_xcdi_in_sel(data)                            (0x00000001&(data))
#define DATA_PATH_SELECT_get_vo_1_insel(data)                         ((0x00000080&(data))>>7)
#define DATA_PATH_SELECT_get_vo_2_insel(data)                         ((0x00000040&(data))>>6)
#define DATA_PATH_SELECT_get_fsync_ch_sel(data)                       ((0x00000020&(data))>>5)
#define DATA_PATH_SELECT_get_adc_ch_sel(data)                         ((0x00000010&(data))>>4)
#define DATA_PATH_SELECT_get_xcdi_clk_en(data)                        ((0x00000008&(data))>>3)
#define DATA_PATH_SELECT_get_uzd1_in_sel(data)                        ((0x00000004&(data))>>2)
#define DATA_PATH_SELECT_get_uzd2_in_sel(data)                        ((0x00000002&(data))>>1)
#define DATA_PATH_SELECT_get_xcdi_in_sel(data)                        (0x00000001&(data))

#define VGIP_VBISLI                                                   0x18022288
#define VGIP_VBISLI_reg_addr                                          "0xB8022288"
#define VGIP_VBISLI_reg                                               0xB8022288
#define VGIP_VBISLI_inst_addr                                         "0x00A2"
#define VGIP_VBISLI_inst                                              0x00A2
#define VGIP_VBISLI_vbisli_in_sel_shift                               (28)
#define VGIP_VBISLI_vbisli_ivs_dly_shift                              (16)
#define VGIP_VBISLI_vbisli_ihs_dly_shift                              (0)
#define VGIP_VBISLI_vbisli_in_sel_mask                                (0x10000000)
#define VGIP_VBISLI_vbisli_ivs_dly_mask                               (0x0FFF0000)
#define VGIP_VBISLI_vbisli_ihs_dly_mask                               (0x00000FFF)
#define VGIP_VBISLI_vbisli_in_sel(data)                               (0x10000000&(((UINT32)data)<<28))
#define VGIP_VBISLI_vbisli_ivs_dly(data)                              (0x0FFF0000&(((UINT32)data)<<16))
#define VGIP_VBISLI_vbisli_ihs_dly(data)                              (0x00000FFF&(data))
#define VGIP_VBISLI_get_vbisli_in_sel(data)                           ((0x10000000&(data))>>28)
#define VGIP_VBISLI_get_vbisli_ivs_dly(data)                          ((0x0FFF0000&(data))>>16)
#define VGIP_VBISLI_get_vbisli_ihs_dly(data)                          (0x00000FFF&(data))

#define INT_CTL                                                       0x1802228c
#define INT_CTL_reg_addr                                              "0xB802228C"
#define INT_CTL_reg                                                   0xB802228C
#define INT_CTL_inst_addr                                             "0x00A3"
#define INT_CTL_inst                                                  0x00A3
#define INT_CTL_offms_int_ie_shift                                    (8)
#define INT_CTL_onms2_int_ie_shift                                    (7)
#define INT_CTL_onms1_int_ie_shift                                    (6)
#define INT_CTL_hist_ch1_int_ie_shift                                 (5)
#define INT_CTL_hist_ch2_int_ie_shift                                 (4)
#define INT_CTL_vgip_int_ie_shift                                     (3)
#define INT_CTL_vi_int_ie_shift                                       (2)
#define INT_CTL_vodma_int_ie_shift                                    (1)
#define INT_CTL_dispi_int_ie_shift                                    (0)
#define INT_CTL_offms_int_ie_mask                                     (0x00000100)
#define INT_CTL_onms2_int_ie_mask                                     (0x00000080)
#define INT_CTL_onms1_int_ie_mask                                     (0x00000040)
#define INT_CTL_hist_ch1_int_ie_mask                                  (0x00000020)
#define INT_CTL_hist_ch2_int_ie_mask                                  (0x00000010)
#define INT_CTL_vgip_int_ie_mask                                      (0x00000008)
#define INT_CTL_vi_int_ie_mask                                        (0x00000004)
#define INT_CTL_vodma_int_ie_mask                                     (0x00000002)
#define INT_CTL_dispi_int_ie_mask                                     (0x00000001)
#define INT_CTL_offms_int_ie(data)                                    (0x00000100&(((UINT32)data)<<8))
#define INT_CTL_onms2_int_ie(data)                                    (0x00000080&(((UINT32)data)<<7))
#define INT_CTL_onms1_int_ie(data)                                    (0x00000040&(((UINT32)data)<<6))
#define INT_CTL_hist_ch1_int_ie(data)                                 (0x00000020&(((UINT32)data)<<5))
#define INT_CTL_hist_ch2_int_ie(data)                                 (0x00000010&(((UINT32)data)<<4))
#define INT_CTL_vgip_int_ie(data)                                     (0x00000008&(((UINT32)data)<<3))
#define INT_CTL_vi_int_ie(data)                                       (0x00000004&(((UINT32)data)<<2))
#define INT_CTL_vodma_int_ie(data)                                    (0x00000002&(((UINT32)data)<<1))
#define INT_CTL_dispi_int_ie(data)                                    (0x00000001&(data))
#define INT_CTL_get_offms_int_ie(data)                                ((0x00000100&(data))>>8)
#define INT_CTL_get_onms2_int_ie(data)                                ((0x00000080&(data))>>7)
#define INT_CTL_get_onms1_int_ie(data)                                ((0x00000040&(data))>>6)
#define INT_CTL_get_hist_ch1_int_ie(data)                             ((0x00000020&(data))>>5)
#define INT_CTL_get_hist_ch2_int_ie(data)                             ((0x00000010&(data))>>4)
#define INT_CTL_get_vgip_int_ie(data)                                 ((0x00000008&(data))>>3)
#define INT_CTL_get_vi_int_ie(data)                                   ((0x00000004&(data))>>2)
#define INT_CTL_get_vodma_int_ie(data)                                ((0x00000002&(data))>>1)
#define INT_CTL_get_dispi_int_ie(data)                                (0x00000001&(data))
#endif
