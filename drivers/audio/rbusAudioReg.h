/**
* @file rbusAudioReg.h
* RBus systemc program.
*
* @author Yu-Cheng Huang
* @email timhuang@realtek.com.tw
* @date 2009/6/15
* @version 1.1
* @ingroup model_rbus
*
*/

#ifndef _RBUS_AUDIO_REG_H_
#define _RBUS_AUDIO_REG_H_

#define AUDIO_REG_FOR_C51

#ifndef UINT8
#define UINT8	unsigned char
#endif
#ifndef UINT16
#define UINT16	unsigned int
#endif
#ifndef UINT32
#define UINT32	unsigned long
#endif



#ifdef AUDIO_REG_FOR_C51

/*
 * Note :
 * 1. C51 pack struct in big-endian but order from LSB to MSB in each BYTE/HALF-WORD
 * 2. some RTD2668-Audio Registers can't not seperate into BBBB,BBH,BHB or HBB group naturally,
 *    we need refine it manually(Ex. seperate the reserved field).
 */
typedef struct
{
    UINT8	bit24_31:8;							// BIT[24:31]
    UINT8	bit16_23:8;							// BIT[16:23]
    UINT8	bit08_15:8;							// BIT[08:15]
    UINT8	bit00_07:8;							// BIT[00:07]
} WORD_BBBB;

typedef struct
{
    UINT8	bit24_31:8;							// BIT[24:31]
    UINT8	bit16_23:8;							// BIT[16:23]
    UINT16	bit00_15:16;						// BIT[00:15]
} WORD_BBH;

typedef struct
{
    UINT8	bit24_31:8;							// BIT[24:31]
    UINT16	bit08_23:16;						// BIT[08:23]
    UINT8	bit00_07:8;							// BIT[00:07]
} WORD_BHB;

typedef struct
{
    UINT16	bit16_31:16;						// BIT[16:31]
    UINT8	bit08_15:8;							// BIT[08:15]
    UINT8	bit00_07:8;							// BIT[00:07]
} WORD_HBB;

typedef struct
{
    UINT16	bit16_31:16;						// BIT[16:31]
    UINT16	bit00_15:16;						// BIT[00:15]
} WORD_HH;

//======AUDIO register structure define==========
// --------------------------------
typedef struct
{
    UINT16  proj:16;							// BIT[16:31]
    UINT16  version:16;							// BIT[00:15]
} _proj_version_RBUS;

typedef union
{
    UINT32				regValue;
    _proj_version_RBUS	bitField;
} proj_version_RBUS;
// --------------------------------
typedef struct
{
    UINT8	res1:3;								// BIT[24:26]
    UINT8	audio_rom_on:1;						// BIT[27]
    UINT8	a2_eiaj_sram_on:1;					// BIT[28]
    UINT8	am_sram_on:1;						// BIT[29]
    UINT8	dac_sram_on:1;						// BIT[30]
    UINT8	adc_sram_on:1;						// BIT[31]

    UINT8	spdif_nicam:1;						// BIT[16]
    UINT8	sel_i2s_hdmi:2;						// BIT[17:18]
    UINT8	sel_i2s_spdif:1;					// BIT[19]
    UINT8	sif_adc_clk_en:1;					// BIT[20]
    UINT8	sif_adc_clk_sel:2;					// BIT[21:22]
    UINT8	hdv_mode_sel:1;						// BIT[23]

    UINT8	daad2_down_sel:2;					// BIT[08:09]
    UINT8	res2:2;								// BIT[10:11]
    UINT8	sel_dac_play_ck_source:2;			// BIT[12:13]
    UINT8	adc_512fs_sel_2:2;					// BIT[14:15]

    UINT8	sel_down_conv:2;					// BIT[00:01]
    UINT8	back_end_play_sample_rate:2;		// BIT[02:03]
    UINT8	sel_play_ck_source:2;				// BIT[04:05]
    UINT8	adc_512fs_sel:2;					// BIT[06:07]
} _global_ctrl_RBUS;

typedef union
{
    UINT32				regValue;
    _global_ctrl_RBUS	bitField;
} global_ctrl_RBUS;
// --------------------------------
typedef struct
{
    UINT8	reg_para_e:3;						// BIT[24:26]
    UINT8	res2:1;								// BIT[27]
    UINT8	div_2fs:2;							// BIT[28:29]
    UINT8	res1:1;								// BIT[30]
    UINT8	contword_inv:1;						// BIT[31]

    UINT8	ps_contword:5;						// BIT[16:20]
    UINT8	ps_testmode:1;						// BIT[21]
    UINT8	res3:2;								// BIT[22:23]

    UINT8	reg_para_d:4;						// BIT[08:11]
    UINT8	mod_acc_rst:1;						// BIT[12]
    UINT8	pll_spdif_auto_mode:1;				// BIT[13]
    UINT8	reg_smooth_en:1;					// BIT[14]
    UINT8	pll_spdif_dtrack_en:1;				// BIT[15]

    UINT8	reg_para_a:6;						// BIT[00:05]
    UINT8	res4:2;								// BIT[06:07]
} _pllcg_spdif_gain_cr0_RBUS;

typedef union
{
    UINT32						regValue;
    _pllcg_spdif_gain_cr0_RBUS	bitField;
} pllcg_spdif_gain_cr0_RBUS;
// --------------------------------
typedef struct
{
    UINT16	p_gain_set:10;						// BIT[16:25]
    UINT16	res1:6;								// BIT[26:31]

    UINT16	p_gain_set2:10;						// BIT[00:09]
    UINT16	res2:6;								// BIT[10:15]
} _pllcg_spdif_gain_cr1_RBUS;

typedef union
{
    UINT32						regValue;
    _pllcg_spdif_gain_cr1_RBUS	bitField;
} pllcg_spdif_gain_cr1_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1_1:16;							// BIT[16:31]

    UINT16	res1_2:6;							// BIT[10:15]
    UINT16	i_gain_set:10;						// BIT[00:09]
} _pllcg_spdif_gain_cr2_RBUS;

typedef union
{
    UINT32						regValue;
    _pllcg_spdif_gain_cr2_RBUS	bitField;
} pllcg_spdif_gain_cr2_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1_1:16;								// BIT[16:31]

    UINT8	res1_2:8;								// BIT[08:15]

    UINT8	max_correct_range:4;					// BIT[00:03]
    UINT8	reach_time_num:3;						// BIT[04:06]
    UINT8	res1_3:1;								// BIT[07]
} _pllcg_spdif_number_RBUS;

typedef union
{
    UINT32						regValue;
    _pllcg_spdif_number_RBUS	bitField;
} pllcg_spdif_number_RBUS;
// --------------------------------
typedef struct
{
    UINT8	high_bound1:8;							// BIT[24:31]
    UINT8	low_bound1:8;							// BIT[16:23]
    UINT8	high_bound2:8;							// BIT[08:15]
    UINT8	low_bound2:8;							// BIT[00:07]
} _pllcg_spdif_bound_RBUS;

typedef union
{
    UINT32						regValue;
    _pllcg_spdif_bound_RBUS		bitField;
} pllcg_spdif_bound_RBUS;
// --------------------------------
typedef union
{
    UINT32		regValue;
    WORD_BBBB	bitField;
} global_dummy_RBUS;
// --------------------------------
typedef struct
{
    UINT8	int_btsc_mode_chg_en:1;				// BIT[24]
    UINT8	int_rbus_timeout_en:1;				// BIT[25]
    UINT8	int_msd_fw_scan_done_en:1;			// BIT[26]
    UINT8	int_msd_decision_done_en:1;			// BIT[27]
    UINT8	res1:4;								// BIT[28:31]

    UINT8	int_fifo0_empty_en:1;				// BIT[16]
    UINT8	int_fifo1_empty_en:1;				// BIT[17]
    UINT8	int_fifo2_empty_en:1;				// BIT[18]
    UINT8	int_fifo3_empty_en:1;				// BIT[19]
    UINT8	int_fifo0_full_en:1;				// BIT[20]
    UINT8	int_fifo1_full_en:1;				// BIT[21]
    UINT8	int_fifo2_full_en:1;				// BIT[22]
    UINT8	int_fifo3_full_en:1;				// BIT[23]

    UINT8	int_i2s_sec_sample_rate_chg_en:1;	// BIT[08]
    UINT8	int_i2s_pri_sample_rate_chg_en:1;	// BIT[09]
    UINT8	int_nicam_hierr_change_en:1;		// BIT[10]
    UINT8	int_spi_in_lock_change_en:1;		// BIT[11]
    UINT8	int_spi_in_datatype_change_en:1;	// BIT[12]
    UINT8	int_aout_timer_en:1;				// BIT[13]
    UINT8	int_ain_timer_en:1;					// BIT[14]
    UINT8	int_pcm_fs_chg_en:1;				// BIT[15]

    UINT8	int_sif_ovf_en:1;					// BIT[00]
    UINT8	int_mode_det_status_change_en:1;	// BIT[01]
    UINT8	int_msd_timeout_en:1;				// BIT[02]
    UINT8	int_msd_change_en:1;				// BIT[03]
    UINT8	int_sif_nopt_en:1;					// BIT[04]
    UINT8	int_i2s_sec_word_length_change_en:1;// BIT[05]
    UINT8	int_i2s_pri_word_length_change_en:1;// BIT[06]
    UINT8	int_spi_sample_rate_chg_en:1;		// BIT[07]
} _interrupt_en_RBUS;

typedef union
{
    UINT32						regValue;
    _interrupt_en_RBUS			bitField;
} interrupt_en_RBUS;
// --------------------------------
typedef struct
{
    UINT8	int_btsc_mode_chg:1;				// BIT[24]
    UINT8	int_rbus_timeout:1;					// BIT[25]
    UINT8	int_msd_fw_scan_done:1;				// BIT[26]
    UINT8	int_msd_decision_done:1;			// BIT[27]
    UINT8	res1:4;								// BIT[28:31]

    UINT8	int_fifo0_empty:1;					// BIT[16]
    UINT8	int_fifo1_empty:1;					// BIT[17]
    UINT8	int_fifo2_empty:1;					// BIT[18]
    UINT8	int_fifo3_empty:1;					// BIT[19]
    UINT8	int_fifo0_full:1;					// BIT[20]
    UINT8	int_fifo1_full:1;					// BIT[21]
    UINT8	int_fifo2_full:1;					// BIT[22]
    UINT8	int_fifo3_full:1;					// BIT[23]

    UINT8	int_i2s_sec_sample_rate_chg:1;		// BIT[08]
    UINT8	int_i2s_pri_sample_rate_chg:1;		// BIT[09]
    UINT8	int_nicam_hierr_change:1;			// BIT[10]
    UINT8	int_spi_in_lock_change:1;			// BIT[11]
    UINT8	int_spi_in_datatype_change:1;		// BIT[12]
    UINT8	int_aout_timer:1;					// BIT[13]
    UINT8	int_ain_timer:1;					// BIT[14]
    UINT8	int_pcm_fs_chg:1;					// BIT[15]

    UINT8	int_sif_ovf:1;						// BIT[00]
    UINT8	int_mode_det_status_change:1;		// BIT[01]
    UINT8	int_msd_timeout:1;					// BIT[02]
    UINT8	int_msd_change:1;					// BIT[03]
    UINT8	int_sif_nopt:1;						// BIT[04]
    UINT8	int_i2s_sec_word_length_change:1;	// BIT[05]
    UINT8	int_i2s_pri_word_length_change:1;	// BIT[06]
    UINT8	int_spi_sample_rate_chg:1;			// BIT[07]
} _interrupt_status_RBUS;


typedef union
{
    UINT32						regValue;
    _interrupt_status_RBUS		bitField;
} interrupt_status_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1_1:16;							// BIT[16:31]

    UINT8	res1_2:8;							// BIT[08:15]

    UINT8	aout_timer_en:1;					// BIT[00]
    UINT8	ain_timer_en:1;						// BIT[01]
    UINT8	res1_3:6;							// BIT[02:07]
} _aio_timer_en_RBUS;

typedef union
{
    UINT32						regValue;
    _aio_timer_en_RBUS			bitField;
} aio_timer_en_RBUS;
// --------------------------------
typedef union
{
    UINT32						regValue;
    WORD_BBBB					bitField;
} ain_counter_RBUS;
// --------------------------------
typedef union
{
    UINT32						regValue;
    WORD_BBBB					bitField;
} ain_compare_RBUS;
// --------------------------------
typedef union
{
    UINT32						regValue;
    WORD_BBBB					bitField;
} aout_counter_RBUS;
// --------------------------------
typedef union
{
    UINT32						regValue;
    WORD_BBBB					bitField;
} aout_compare_RBUS;
// --------------------------------
typedef struct
{
    UINT8	res1:2;								// BIT[24:25]
    UINT8	debounce_en:1;						// BIT[26]
    UINT8	mode_sel:1;							// BIT[27]
    UINT8	left_right:2;						// BIT[28:29]
    UINT8	source_sel:2;						// BIT[30:31]

    UINT8	reserved_dummy:8;					// BIT[16:23]

    UINT8	sampling_rate:4;					// BIT[08:11]
    UINT8	res2:4;								// BIT[12:15]

    UINT8	pcm_width:7;						// BIT[00:06]
    UINT8	res3:1;								// BIT[07]
} _i2s_in_primary_RBUS;

typedef union
{
    UINT32						regValue;
    _i2s_in_primary_RBUS		bitField;
} i2s_in_primary_RBUS;
// --------------------------------
typedef struct
{
    UINT8	res1_1:2;							// BIT[24:25]
    UINT8	debounce_en:1;						// BIT[26]
    UINT8	mode_sel:1;							// BIT[27]
    UINT8	left_right:2;						// BIT[28:29]
    UINT8	source_sel:2;						// BIT[30:31]

    UINT8	res1_2:8;							// BIT[16:23]

    UINT8	sampling_rate:4;					// BIT[08:11]
    UINT8	res1_3:4;							// BIT[12:15]

    UINT8	pcm_width:7;						// BIT[00:06]
    UINT8	res2:1;								// BIT[07]
} _i2s_in_secondary_RBUS;


typedef union
{
    UINT32						regValue;
    _i2s_in_secondary_RBUS		bitField;
} i2s_in_secondary_RBUS;
// --------------------------------
typedef struct
{
    UINT8	res1_1:4;							// BIT[24:27]
    UINT8	debounce_en:1;						// BIT[28]
    UINT8	req_after_lock:1;					// BIT[29]
    UINT8	source_sel:2;						// BIT[30:31]

    UINT8	res1_2:8;							// BIT[16:23]

    UINT8	reserved_dummy_1:8;					// BIT[08:15]

    UINT8	sampling_rate:4;					// BIT[00:03]
    UINT8	res2:1;								// BIT[04]
    UINT8	reserved_dummy_2:3;					// BIT[05:07]
} _spdif_in_RBUS;

typedef union
{
    UINT32						regValue;
    _spdif_in_RBUS				bitField;
} spdif_in_RBUS;
// --------------------------------
typedef struct
{
    UINT8	sampling_freq:4;					// BIT[24:27]
    UINT8	clock_accuracy:2;					// BIT[28:29]
    UINT8	invalid_bit:1;						// BIT[30]
    UINT8	hw_locked:1;						// BIT[31]

    UINT8	source_number:4;					// BIT[16:19]
    UINT8	channel_number:4;					// BIT[20:23]

    UINT8	category_code:8;					// BIT[08:15]

    UINT8	professional:1;						// BIT[00]
    UINT8	data_type:1;						// BIT[01]
    UINT8	copyright:1;						// BIT[02]
    UINT8	pre_emphasis:3;						// BIT[03:05]
    UINT8	mode:2;								// BIT[06:07]
} _spdif_in_cs_RBUS;

typedef union
{
    UINT32						regValue;
    _spdif_in_cs_RBUS			bitField;
} spdif_in_cs_RBUS;
// --------------------------------
typedef struct
{
    UINT8	mclk_sel:2;							// BIT[24:25]
    UINT8	res2:2;								// BIT[26:27]
    UINT8	output_sel:3;						// BIT[28:30]
    UINT8	res1:1;								// BIT[31]

    UINT8	res3:8;								// BIT[16:23]

    UINT16	reserved_dummy:16;					// BIT[00:15]
} _i2s_out_RBUS;

typedef union
{
    UINT32						regValue;
    _i2s_out_RBUS				bitField;
} i2s_out_RBUS;
// --------------------------------
typedef  struct
{
    UINT8	res2_1:4;							// BIT[24:27]
    UINT8	validity_ctrl:1;					// BIT[28]
    UINT8	res1:1;								// BIT[29]
    UINT8	output_sel:2;						// BIT[30:31]

    UINT8	res2_2:8;							// BIT[16:23]

    UINT16	reserved_dummy:16;					// BIT[00:15]
} _spdif_out_RBUS;

typedef union
{
    UINT32						regValue;
    _spdif_out_RBUS				bitField;
} spdif_out_RBUS;
// --------------------------------
typedef struct
{
    UINT8	sampling_freq:4;					// BIT[24:27]
    UINT8	clock_accuracy:2;					// BIT[28:29]
    UINT8	res1:2;								// BIT[30:31]

    UINT8	source_number:4;					// BIT[16:19]
    UINT8	channel_number:4;					// BIT[20:23]

    UINT8	category_code:8;					// BIT[08:15]

    UINT8	professional:1;						// BIT[00]
    UINT8	data_type:1;						// BIT[01]
    UINT8	copyright:1;						// BIT[02]
    UINT8	pre_emphasis:3;						// BIT[03:05]
    UINT8	mode:2;								// BIT[06:07]
} _spdif_out_cs_RBUS;

typedef union
{
    UINT32						regValue;
    _spdif_out_cs_RBUS			bitField;
} spdif_out_cs_RBUS;
// --------------------------------
typedef  struct
{
    UINT8	res1_1:8;							// BIT[24:31]

    UINT8	hw_agc_en:1;						// BIT[16]
    UINT8	dummy_19_17:3;						// BIT[17:19]
    UINT8	res1_2:4;							// BIT[20:23]

    UINT16	ob_time:16;							// BIT[00:15]
} _sif_agc_eval_cnt_RBUS;

typedef union
{
    UINT32						regValue;
    _sif_agc_eval_cnt_RBUS		bitField;
} sif_agc_eval_cnt_RBUS;
// --------------------------------
typedef  struct
{
    UINT8	res1_2:5;							// BIT[19:23]
    UINT8	res1_1:8;							// BIT[24:31]

    UINT8	non_opt_cnt_th:3;					// BIT[16:18]

    UINT8	non_opt_cnt_under:8;				// BIT[08:15]

    UINT8	non_opt_cnt_ovf:8;					// BIT[00:07]
} _sif_agc_optimal_RBUS;

typedef union
{
    UINT32						regValue;
    _sif_agc_optimal_RBUS		bitField;
} sif_agc_optimal_RBUS;
// --------------------------------
typedef  struct
{
    UINT16	ovf_th:10;							// BIT[16:25]
    UINT16	res1:6;								// BIT[26:31]

    UINT16	opt_th:10;							// BIT[00:09]
    UINT16	res2:6;								// BIT[10:15]
} _sif_agc_threshold_RBUS;

typedef union
{
    UINT32						regValue;
    _sif_agc_threshold_RBUS		bitField;
} sif_agc_threshold_RBUS;
// --------------------------------
typedef  struct
{
    UINT16	res1_1:16;							// BIT[16:31]

    UINT16	abs_peak:10;						// BIT[00:09]
    UINT16	res2:2;								// BIT[10:11]
    UINT16	opt:1;								// BIT[12]
    UINT16	ovf:1;								// BIT[13]
    UINT16	res1_2:2;							// BIT[14:15]
} _sif_agc_status_RBUS;

typedef union
{
    UINT32						regValue;
    _sif_agc_status_RBUS		bitField;
} sif_agc_status_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1_1:16;							// BIT[16:31]

    UINT8	res1_2:8;							// BIT[08:15]

    UINT8	pdetect_enable:1;					// BIT[00]
    UINT8	detect_source_sel:3;				// BIT[01:03]
    UINT8	res1_3:4;							// BIT[04:07]
} _peak_contorl_RBUS;

typedef union
{
    UINT32						regValue;
    _peak_contorl_RBUS			bitField;
} peak_contorl_RBUS;
// --------------------------------
typedef struct
{
    UINT16	opt_th_l:16;						// BIT[16:31]
    UINT16	ovf_th_l:16;						// BIT[00:15]
} _peak_contorl_l_0_RBUS;

typedef union
{
    UINT32						regValue;
    _peak_contorl_l_0_RBUS		bitField;
} peak_contorl_l_0_RBUS;
// --------------------------------
typedef struct
{
    UINT16	nonopt_cnt_tr_l:3;					// BIT[16:18]
    UINT16	dummy_61ac:13;						// BIT[19:31]

    UINT16	vpp_eval_cnt_l:14;					// BIT[00:13]
    UINT16	res1:2;								// BIT[14:15]
} _peak_control_l_1_RBUS;

typedef union
{
    UINT32						regValue;
    _peak_control_l_1_RBUS		bitField;
} peak_control_l_1_RBUS;
// --------------------------------
typedef struct
{
    UINT8	res1:8;								// BIT[24:31]

    UINT8	pk_adj_l:6;							// BIT[16:21]
    UINT8	overflow_l:1;						// BIT[22]
    UINT8	optimize_l:1;						// BIT[23]

    UINT16	magnitude_l:16;						// BIT[00:15]
} _peak_status_l_RBUS;

typedef union
{
    UINT32						regValue;
    _peak_status_l_RBUS			bitField;
} peak_status_l_RBUS;
// --------------------------------
typedef struct
{
    UINT16	opt_th_r:16;						// BIT[16:31]
    UINT16	ovf_th_r:16;						// BIT[00:15]
} _peak_contorl_r_0_RBUS;

typedef union
{
    UINT32						regValue;
    _peak_contorl_r_0_RBUS		bitField;
} peak_contorl_r_0_RBUS;
// --------------------------------
typedef struct
{
    UINT16	nonopt_cnt_tr_r:3;					// BIT[16:18]
    UINT16	dummy_61b8:13;						// BIT[19:31]

    UINT16	vpp_eval_cnt_r:14;					// BIT[00:13]
    UINT16	res1:2;								// BIT[14:15]
} _peak_control_r_1_RBUS;

typedef union
{
    UINT32						regValue;
    _peak_control_r_1_RBUS		bitField;
} peak_contorl_r_1_RBUS;
// --------------------------------
typedef struct
{
    UINT8	res1:8;								// BIT[24:31]

    UINT8	pk_adj_r:6;							// BIT[16:21]
    UINT8	overflow_r:1;						// BIT[22]
    UINT8	optimize_r:1;						// BIT[23]

    UINT16	magnitude_r:16;						// BIT[00:15]
} _peak_status_r_RBUS;

typedef union
{
    UINT32						regValue;
    _peak_status_r_RBUS			bitField;
} peak_status_r_RBUS;
// --------------------------------
typedef  struct
{
    UINT8	carrier_phase_delta:3;				// BIT[24:26]
    UINT8	carrier_shift_scan_en:1;			// BIT[27]
    UINT8	update_en:1;						// BIT[28]
    UINT8	msd_en:1;							// BIT[29]
    UINT8	fw_scan_en:1;						// BIT[30]
    UINT8	res1:1;								// BIT[31]

    UINT16	dkl_priority:1;						// BIT[08]
    UINT16	mn_priority:2;						// BIT[09:10]
    UINT16	res3:1;								// BIT[11]
    UINT16	reg_scan_cnt:8;						// BIT[12:19]
    UINT16	msd_debounce_cnt:3;					// BIT[20:22]
    UINT16	res2:1;								// BIT[23]

    UINT8	play_sel_b:2;						// BIT[00:01]
    UINT8	play_sel_a:2;						// BIT[02:03]
    UINT8	sound_select:2;						// BIT[04:05]
    UINT8	res4:2;								// BIT[06:07]
} _msd_ctrl_word_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_ctrl_word_RBUS			bitField;
} msd_ctrl_word_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1_1:16;							// BIT[16:31]

    UINT8	hw_detected_std:5;					// BIT[08:12]
    UINT8	res1_2:3;							// BIT[13:15]

    UINT8	reg_std:5;							// BIT[00:04]
    UINT8	res2:3;								// BIT[05:07]
} _msd_result_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_result_RBUS			bitField;
} msd_result_RBUS;
// --------------------------------
typedef struct
{
    UINT16	thresh_4_5:16;						// BIT[16:31]
    UINT16	thresh_4_5_l:16;					// BIT[00:15]
} _msd_thresh_4_5_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_thresh_4_5_RBUS		bitField;
} msd_thresh_4_5_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1:16;							// BIT[16:31]
    UINT16	reg_mag_4_5:16;						// BIT[00:15]
} _msd_mag_4_5_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_mag_4_5_RBUS			bitField;
} msd_mag_4_5_RBUS;
// --------------------------------
typedef struct
{
    UINT16	thresh_4_72:16;						// BIT[16:31]
    UINT16	reg_mag_4_72:16;					// BIT[00:15]
} _msd_carrier_4_72_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_carrier_4_72_RBUS		bitField;
} msd_carrier_4_72_RBUS;
// --------------------------------
typedef struct
{
    UINT16	thresh_5_5:16;						// BIT[16:31]
    UINT16	thresh_5_5_l:16;					// BIT[00:15]
} _msd_thresh_5_5_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_thresh_5_5_RBUS		bitField;
} msd_thresh_5_5_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1:16;							// BIT[16:31]
    UINT16	reg_mag_5_5:16;						// BIT[00:15]
} _msd_mag_5_5_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_mag_5_5_RBUS			bitField;
} msd_mag_5_5_RBUS;
// --------------------------------
typedef struct
{
    UINT16	thresh_5_74:16;						// BIT[16:31]
    UINT16	reg_mag_5_74:16;					// BIT[00:15]
} _msd_carrier_5_74_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_carrier_5_74_RBUS		bitField;
} msd_carrier_5_74_RBUS;
// --------------------------------
typedef struct
{
    UINT16	thresh_5_85:16;						// BIT[16:31]
    UINT16	reg_mag_5_85:16;					// BIT[00:15]
} _msd_carrier_5_85_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_carrier_5_85_RBUS		bitField;
} msd_carrier_5_85_RBUS;
// --------------------------------
typedef struct
{
    UINT16	thresh_6:16;						// BIT[16:31]
    UINT16	thresh_6_l:16;						// BIT[00:15]
} _msd_thresh_6_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_thresh_6_RBUS			bitField;
} msd_thresh_6_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1:16;							// BIT[16:31]
    UINT16	reg_mag_6:16;						// BIT[00:15]
} _msd_mag_6_0_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_mag_6_0_RBUS			bitField;
} msd_mag_6_0_RBUS;//btsc_mag_6_RBUS;
// --------------------------------
typedef struct
{
    UINT16	thresh_6_25:16;						// BIT[16:31]
    UINT16	reg_mag_6_25:16;					// BIT[00:15]
} _msd_carrier_6_25_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_carrier_6_25_RBUS		bitField;
} msd_carrier_6_25_RBUS;//msd_carrier_625_RBUS;
// --------------------------------
typedef struct
{
    UINT16	thresh_6_35:16;						// BIT[16:31]
    UINT16	reg_mag_6_35:16;					// BIT[00:15]
} _msd_carrier_6_35_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_carrier_6_35_RBUS		bitField;
} msd_carrier_6_35_RBUS;
// --------------------------------
typedef struct
{
    UINT16	thresh_6_45:16;						// BIT[16:31]
    UINT16	reg_mag_6_45:16;					// BIT[00:15]
} _msd_carrier_6_45_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_carrier_6_45_RBUS		bitField;
} msd_carrier_6_45_RBUS;
// --------------------------------
typedef struct
{
    UINT16	thresh_6_5:16;						// BIT[16:31]
    UINT16	thresh_6_5_l:16;					// BIT[00:15]
} _msd_thresh_6_5_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_thresh_6_5_RBUS		bitField;
} msd_thresh_6_5_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1:16;							// BIT[16:31]
    UINT16	reg_mag_6_5:16;						// BIT[00:15]
} _msd_mag_6_5_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_mag_6_5_RBUS			bitField;
} msd_mag_6_5_RBUS;
// --------------------------------
typedef struct
{
    UINT16	thresh_6_552:16;					// BIT[16:31]
    UINT16	reg_mag_6_552:16;					// BIT[00:15]
} _msd_carrier_6_552_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_carrier_6_552_RBUS		bitField;
} msd_carrier_6_552_RBUS;//btsc_carrier_6_52_RBUS;
// --------------------------------
typedef struct
{
    UINT16	thresh_6_62:16;						// BIT[16:31]
    UINT16	reg_mag_6_62:16;					// BIT[00:15]
} _msd_carrier_6_62_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_carrier_6_62_RBUS		bitField;
} msd_carrier_6_62_RBUS;//btsc_mag_6_62_RBUS;
// --------------------------------
typedef struct
{
    UINT16	thresh_6_74:16;						// BIT[16:31]
    UINT16	reg_mag_6_74:16;					// BIT[00:15]
} _msd_carrier_6_74_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_carrier_6_74_RBUS		bitField;
} msd_carrier_6_74_RBUS;//btsc_carrier_6_74_RBUS;
// --------------------------------
typedef struct
{
    UINT16	thresh_6_8:16;						// BIT[16:31]
    UINT16	reg_mag_6_8:16;						// BIT[00:15]
} _msd_carrier_6_8_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_carrier_6_8_RBUS		bitField;
} msd_carrier_6_8_RBUS;//btsc_carrier_6_8_RBUS;
// --------------------------------
typedef struct
{
    UINT16	thresh_fw_l:16;						// BIT[16:31]

    UINT16	fw_phase_delta:14;					// BIT[00:13]
    UINT16	res1:2;								// BIT[14:15]
} _msd_prog_freq_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_prog_freq_RBUS	bitField;
} msd_prog_freq_RBUS;//msd_programmable_freq_RBUS;
// --------------------------------
typedef struct
{
    UINT16	thresh_fw:16;						// BIT[16:31]
    UINT16	reg_mag_fw:16;						// BIT[00:15]
} _msd_prog_freq_threshold_RBUS;

typedef union
{
    UINT32							regValue;
    _msd_prog_freq_threshold_RBUS	bitField;
} msd_prog_freq_threshold_RBUS;
// --------------------------------
typedef  struct
{
    UINT16	thresh_574_585:16;					// BIT[16:31]
    UINT16	diff_574_585:16;					// BIT[00:15]
} _msd_574_585_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_574_585_RBUS			bitField;
} msd_574_585_RBUS;
// --------------------------------
typedef struct
{
    UINT16	thresh_585_55:16;					// BIT[16:31]
    UINT16	diff_585_55:16;						// BIT[00:15]
} _msd_585_55_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_585_55_RBUS			bitField;
} msd_585_55_RBUS;
// --------------------------------
typedef struct
{
    UINT16	thresh_585_635:16;					// BIT[16:31]
    UINT16	diff_585_635:16;					// BIT[00:15]
} _msd_585_635_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_585_635_RBUS			bitField;
} msd_585_635_RBUS;
// --------------------------------
typedef struct
{
    UINT16	thresh_625_55:16;					// BIT[16:31]
    UINT16	diff_625_55:16;						// BIT[00:15]
} _msd_625_55_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_625_55_RBUS			bitField;
} msd_625_55_RBUS;
// --------------------------------
typedef struct
{
    UINT16	thresh_6552_68:16;					// BIT[16:31]
    UINT16	diff_6552_68:16;					// BIT[00:15]
} _msd_6552_68_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_6552_68_RBUS			bitField;
} msd_6552_68_RBUS;
// --------------------------------
typedef struct
{
    UINT8	res1_1:8;							// BIT[24:31]

    UINT8	flag_fw_freq_0:1;					// BIT[16]
    UINT8	flag_fw_freq_1:1;					// BIT[17]
    UINT8	flag_fw_freq_2:1;					// BIT[18]
    UINT8	flag_fw_freq_3:1;					// BIT[19]
    UINT8	res1_2:4;							// BIT[20:23]

    UINT8	flag_6_25:1;						// BIT[08]
    UINT8	flag_6:1;							// BIT[09]
    UINT8	flag_5_85:1;						// BIT[10]
    UINT8	flag_5_74:1;						// BIT[11]
    UINT8	flag_5_5:1;							// BIT[12]
    UINT8	flag_4_72:1;						// BIT[13]
    UINT8	flag_4_5:1;							// BIT[14]
    UINT8	res2:1;								// BIT[15]

    UINT8	flag_fw:1;							// BIT[00]
    UINT8	flag_6_8:1;							// BIT[01]
    UINT8	flag_6_74:1;						// BIT[02]
    UINT8	flag_6_62:1;						// BIT[03]
    UINT8	flag_6_552:1;						// BIT[04]
    UINT8	flag_6_5:1;							// BIT[05]
    UINT8	flag_6_45:1;						// BIT[06]
    UINT8	flag_6_35:1;						// BIT[07]
} _msd_flag_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_flag_RBUS				bitField;
} msd_flag_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1_1:16;							// BIT[16:31]

    UINT8	large_574_585:1;					// BIT[08]
    UINT8	only_6_5:1;							// BIT[09]
    UINT8	only_6:1;							// BIT[10]
    UINT8	only_5_5:1;							// BIT[11]
    UINT8	only_4_5:1;							// BIT[12]
    UINT8	res1_2:3;							// BIT[13:15]

    UINT8	large_6552_thresh:1;				// BIT[00]
    UINT8	large_625_thresh:1;					// BIT[01]
    UINT8	large_585_635_thresh:1;				// BIT[02]
    UINT8	large_585_thresh:1;					// BIT[03]
    UINT8	large_diff_thresh:1;				// BIT[04]
    UINT8	large_625_55:1;						// BIT[05]
    UINT8	large_585_635:1;					// BIT[06]
    UINT8	large_585_55:1;						// BIT[07]
} _msd_large_status_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_large_status_RBUS		bitField;
} msd_large_status_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1:16;							// BIT[16:31]
    UINT16	std_list:16;						// BIT[00:15]
} _msd_std_list_RBUS;


typedef union
{
    UINT32						regValue;
    _msd_std_list_RBUS			bitField;
} msd_std_list_RBUS;
// --------------------------------
typedef struct
{
    UINT16	thresh_4_5_second:16;				// BIT[16:31]
    UINT16	thresh_5_5_second:16;				// BIT[00:15]
} _msd_phase2_4_5_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_phase2_4_5_RBUS		bitField;
} msd_phase2_4_5_RBUS;
// --------------------------------
typedef struct
{
    UINT16	thresh_6_second:16;					// BIT[16:31]
    UINT16	thresh_6_5_second:16;				// BIT[00:15]
} _msd_phase2_5_5_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_phase2_5_5_RBUS		bitField;
} msd_phase2_5_5_RBUS;
// --------------------------------
typedef  struct
{
    UINT16	res1_1:16;							// BIT[16:31]

    UINT8	mono_std_sel:1;						// BIT[08]
    UINT8	res1_2:7;							// BIT[09:15]

    UINT8	second_reg_std:5;					// BIT[00:04]
    UINT8	res2:3;								// BIT[05:07]
} _msd_second_reg_std_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_second_reg_std_RBUS	bitField;
} msd_second_reg_std_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1_1:16;							// BIT[16:31]

    UINT8	res1_2:8;							// BIT[08:15]

    UINT8	bist_fail_msd:1;					// BIT[00]
    UINT8	bist_done_msd:1;					// BIT[01]
    UINT8	bist_mode_msd:1;					// BIT[02]
    UINT8	bist_rstn_msd:1;					// BIT[03]
    UINT8	res1_3:4;							// BIT[04:07]
} _msd_bist_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_bist_RBUS				bitField;
} msd_bist_RBUS;
// --------------------------------
typedef struct
{
    UINT16	fw_freq_0_set:14;					// BIT[16:29]
    UINT16	res1:2;								// BIT[30:31]

    UINT16	mag_fw_freq_0:16;					// BIT[00:15]
} _msd_fw_freq_0_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_fw_freq_0_RBUS			bitField;
} msd_fw_freq_0_RBUS;
// --------------------------------
typedef  struct
{
    UINT16	fw_freq_0_high_thre:16;				// BIT[16:31]
    UINT16	fw_freq_0_low_thre:16;				// BIT[00:15]
} _msd_fw_req_0_thre_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_fw_req_0_thre_RBUS		bitField;
} msd_fw_req_0_thre_RBUS;
// --------------------------------
typedef struct
{
    UINT16	fw_freq_1_set:14;					// BIT[16:29]
    UINT16	res1:2;								// BIT[30:31]

    UINT16	mag_fw_freq_1:16;					// BIT[00:15]
} _msd_fw_freq_1_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_fw_freq_1_RBUS			bitField;
} msd_fw_freq_1_RBUS;
// --------------------------------
typedef struct
{
    UINT16	fw_freq_1_high_thre:16;				// BIT[16:31]
    UINT16	fw_freq_1_low_thre:16;				// BIT[00:15]
} _msd_fw_req_1_thre_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_fw_req_1_thre_RBUS		bitField;
} msd_fw_req_1_thre_RBUS;
// --------------------------------
typedef struct
{
    UINT16	fw_freq_2_set:14;					// BIT[16:29]
    UINT16	res1:2;								// BIT[30:31]

    UINT16	mag_fw_freq_2:16;					// BIT[00:15]
} _msd_fw_freq_2_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_fw_freq_2_RBUS			bitField;
} msd_fw_freq_2_RBUS;
// --------------------------------
typedef struct
{
    UINT16	fw_freq_2_high_thre:16;				// BIT[16:31]
    UINT16	fw_freq_2_low_thre:16;				// BIT[00:15]
} _msd_fw_req_2_thre_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_fw_req_2_thre_RBUS		bitField;
} msd_fw_req_2_thre_RBUS;
// --------------------------------
typedef struct
{
    UINT16	fw_freq_3_set:14;					// BIT[16:29]
    UINT16	res1:2;								// BIT[30:31]

    UINT16	mag_fw_freq_3:16;					// BIT[00:15]
} _msd_fw_freq_3_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_fw_freq_3_RBUS			bitField;
} msd_fw_freq_3_RBUS;
// --------------------------------
typedef struct
{
    UINT16	fw_freq_3_high_thre:16;				// BIT[16:31]
    UINT16	fw_freq_3_low_thre:16;				// BIT[00:15]
} _msd_fw_req_3_thre_RBUS;

typedef union
{
    UINT32						regValue;
    _msd_fw_req_3_thre_RBUS		bitField;
} msd_fw_req_3_thre_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1_1:16;							// BIT[16:31]

    UINT8	res1_2:8;							// BIT[08:15]

    UINT8	mode_result:2;						// BIT[00:01]
    UINT8	hw_mode_en:1;						// BIT[02]
    UINT8	res1_3:5;							// BIT[03:07]
} _mode_detect_RBUS;

typedef union
{
    UINT32						regValue;
    _mode_detect_RBUS			bitField;
} mode_detect_RBUS;
// --------------------------------
typedef struct
{
    UINT8	low_threshold:2;						// BIT[24:25]
    UINT8	high_threshold:2;					// BIT[26:27]
    UINT8	accu_num:2;							// BIT[28:29]
    UINT8	res1:1;								// BIT[30]
    UINT8	enable:1;							// BIT[31]

    UINT8	res3:4;								// BIT[16:19]
    UINT8	debounce_cnt:3;						// BIT[20:22]
    UINT8	res2:1;								// BIT[23]


    UINT16	max_threshold:16;					// BIT[00:15]
} _md_ctrl_RBUS;

typedef union
{
    UINT32						regValue;
    _md_ctrl_RBUS				bitField;
} md_ctrl_RBUS;
// --------------------------------
typedef struct
{
    UINT16	max_other:16;						// BIT[16:31]

    UINT8	reserved_dummy:8;					// BIT[08:15]

    UINT8	mode_result:2;						// BIT[00:01]
    UINT8	res1:2;								// BIT[02:03]
    UINT8	gain_result:4;						// BIT[04:07]
} _md_status_1_RBUS;

typedef union
{
    UINT32						regValue;
    _md_status_1_RBUS			bitField;
} md_status_1_RBUS;
// --------------------------------
typedef struct
{
    UINT16	max_stereo:16;						// BIT[16:31]
    UINT16	max_dual:16;						// BIT[00:15]
} _md_status_2_RBUS;

typedef union
{
    UINT32						regValue;
    _md_status_2_RBUS			bitField;
} md_status_2_RBUS;
// --------------------------------
typedef struct
{
    UINT8	res1_1:5;							// BIT[24:28]
    UINT8	done:1;								// BIT[29]
    UINT8	enable:1;							// BIT[30]
    UINT8	rst_n:1;							// BIT[31]

    UINT16	res1_2:16;							// BIT[08:23]

    UINT8	fail_51x16:1;						// BIT[00]
    UINT8	fail_52x16:1;						// BIT[01]
    UINT8	res1_3:6;							// BIT[02:07]
} _mbist_md_RBUS;

typedef union
{
    UINT32						regValue;
    _mbist_md_RBUS				bitField;
} mbist_md_RBUS;
// --------------------------------
typedef struct
{
    UINT8	res1_1:7;							// BIT[24:30]
    UINT8	sif_data_source:1;					// BIT[31]

    UINT16	res1_2:16;							// BIT[08:23]

    UINT8	carrier_freq_correct_en:1;			// BIT[00]
    UINT8	res1_3:7;							// BIT[01:07]
} _carrier_freq_deviatio_RBUS;

typedef union
{
    UINT32						regValue;
    _carrier_freq_deviatio_RBUS	bitField;
} carrier_freq_deviatio_RBUS;
// --------------------------------
typedef struct
{
    UINT16	main_phase_delta:14;				// BIT[16:29]
    UINT16	res1:2;								// BIT[30:31]

    UINT16	sub_phase_delta:14;					// BIT[00:13]
    UINT16	res2:2;								// BIT[14:15]
} _carrier_phase_delta_RBUS;

typedef union
{
    UINT32						regValue;
    _carrier_phase_delta_RBUS	bitField;
} carrier_phase_delta_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1_1:16;							// BIT[16:31]

    UINT8	bist_fail_am:1;						// BIT[08]
    UINT8	bist_done_am:1;						// BIT[09]
    UINT8	bist_mode_am:1;						// BIT[10]
    UINT8	bist_rstn_am:1;						// BIT[11]
    UINT8	res1_2:4;							// BIT[12:15]

    UINT8	hw_am_prescale:2;					// BIT[00:01]
    UINT8	am_path_sel:1;						// BIT[02]
    UINT8	res2:1;								// BIT[03]
    UINT8	dummy_6358:4;						// BIT[04:07]
} _am_RBUS;

typedef union
{
    UINT32						regValue;
    _am_RBUS					bitField;
} am_RBUS;
// --------------------------------
typedef struct
{
    UINT16	pilot_hithresh:10;					// BIT[16:25]
    UINT16	res1:5;								// BIT[26:30]
    UINT16	lock_reference:1;					// BIT[31]

    UINT16	pilot_lothresh:10;					// BIT[00:09]
    UINT16	res2:6;								// BIT[10:15]
} _btsc_pilot_thres_RBUS;

typedef union
{
    UINT32						regValue;
    _btsc_pilot_thres_RBUS		bitField;
} btsc_pilot_thres_RBUS;
// --------------------------------
typedef struct
{
    UINT16	w1pilotpresent:1;					// BIT[16]
    UINT16	res1:15;							// BIT[17:31]

    UINT16	w10pilotmagnitude:10;				// BIT[00:09]
    UINT16	res2:2;								// BIT[10:11]
    UINT16	pilot_lo_refuse:1;					// BIT[12]
    UINT16	pilot_hi_pass:1;					// BIT[13]
    UINT16	pilot_level_pass:1;					// BIT[14]
    UINT16	w1pilotlocked:1;					// BIT[15]
} _btsc_pilot_status_RBUS;

typedef union
{
    UINT32						regValue;
    _btsc_pilot_status_RBUS		bitField;
} btsc_pilot_status_RBUS;
// --------------------------------
typedef struct
{
    UINT16	sap_hithresh:10;					// BIT[16:25]
    UINT16	res1:6;								// BIT[26:31]

    UINT16	sap_lothresh:10;					// BIT[00:09]
    UINT16	res2:6;								// BIT[10:15]
} _btsc_sap_thres_RBUS;

typedef union
{
    UINT32						regValue;
    _btsc_sap_thres_RBUS		bitField;
} btsc_sap_thres_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1_1:16;							// BIT[16:31]

    UINT16	w10sapnoise:10;						// BIT[00:09]
    UINT16	res1_2:6;							// BIT[10:15]
} _btsc_sap_noise_RBUS;

typedef union
{
    UINT32						regValue;
    _btsc_sap_noise_RBUS		bitField;
} btsc_sap_noise_RBUS;
// --------------------------------
typedef struct
{
    UINT16	sap_noise_hi_thresh:10;				// BIT[16:25]
    UINT16	res1:6;								// BIT[26:31]

    UINT16	sap_noise_lo_thresh:10;				// BIT[00:09]
    UINT16	res2:6;								// BIT[10:15]
} _btsc_sap_noise_th_RBUS;

typedef union
{
    UINT32						regValue;
    _btsc_sap_noise_th_RBUS		bitField;
} btsc_sap_noise_th_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1_1:16;							// BIT[16:31]

    UINT8	res1_2:8;							// BIT[08:15]

    UINT8	sap_debounce:4;						// BIT[00:03]
    UINT8	sap_debounce_en:1;					// BIT[04]
    UINT8	res1_3:3;							// BIT[05:07]
} _btsc_sap_debounce_RBUS;

typedef union
{
    UINT32						regValue;
    _btsc_sap_debounce_RBUS		bitField;
} btsc_sap_debounce_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1_1:16;							// BIT[16:31]

    UINT16	w10sapmagnitude:10;					// BIT[00:09]
    UINT16	res2:1;								// BIT[10]
    UINT16	sap_noise_flag:1;					// BIT[11]
    UINT16	sap_lo_refuse:1;					// BIT[12]
    UINT16	sap_hi_pass:1;						// BIT[13]
    UINT16	w1sapqualitygood:1;					// BIT[14]
    UINT16	res1_2:1;							// BIT[15]
} _btsc_sap_status_RBUS;

typedef union
{
    UINT32						regValue;
    _btsc_sap_status_RBUS		bitField;
} btsc_sap_status_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1_1:16;							// BIT[16:31]

    UINT16	w10fmmagnitude:10;					// BIT[00:09]
    UINT16	res2:2;								// BIT[10:11]
    UINT16	fm_mag_optimize:1;					// BIT[12]
    UINT16	fm_mag_overflow:1;					// BIT[13]
    UINT16	res1_2:2;							// BIT[14:15]
} _btsc_sif_fm_magnitude_RBUS;

typedef union
{
    UINT32						regValue;
    _btsc_sif_fm_magnitude_RBUS	bitField;
} btsc_sif_fm_magnitude_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1_1:16;							// BIT[16:31]

    UINT16	testoutputselect:4;					// BIT[00:03]
    UINT16	res1_2:12;							// BIT[04:15]
} _btsc_test_mux_sel_RBUS;

typedef union
{
    UINT32						regValue;
    _btsc_test_mux_sel_RBUS		bitField;
} btsc_test_mux_sel_RBUS;
// --------------------------------
typedef union
{
    UINT32						regValue;
    WORD_HH						bitField;
} btsc_dummy_RBUS;
// --------------------------------
typedef struct
{
    UINT8	res1_1:2;							// BIT[24:25]
    UINT8	ultra_hdv_mode:1;					// BIT[26]
    UINT8	iir_lpf_en:1;						// BIT[27]
    UINT8	de_emphasis_time:2;					// BIT[28:29]
    UINT8	a2_bw_sel:2;						// BIT[30:31]

    UINT8	res2_1:4;							// BIT[16:19]
    UINT8	ultra_hdv_gain:3;					// BIT[20:22]
    UINT8	res1_2:1;							// BIT[23]

    UINT8	fm1_deviation_gain:3;				// BIT[08:10]
    UINT8	res2_2:5;							// BIT[11:15]

    UINT8	eiaj_sub_deviation:1;				// BIT[00]
    UINT8	res4:3;								// BIT[01:03]
    UINT8	fm2_deviation_gain:3;				// BIT[04:06]
    UINT8	res3:1;								// BIT[07]
} _a2_eiaj_demod_RBUS;

typedef union
{
    UINT32						regValue;
    _a2_eiaj_demod_RBUS			bitField;
} a2_eiaj_demod_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1_1:13;							// BIT[16:28]
    UINT16	done:1;								// BIT[29]
    UINT16	enable:1;							// BIT[30]
    UINT16	rst_n:1;							// BIT[31]

    UINT16	fail_24x13_0:1;						// BIT[00]
    UINT16	fail_24x13_1:1;						// BIT[01]
    UINT16	fail_24x14_0:1;						// BIT[02]
    UINT16	fail_24x14_1:1;						// BIT[03]
    UINT16	fail_24x66_0:1;						// BIT[04]
    UINT16	fail_24x66_1:1;						// BIT[05]
    UINT16	fail_24x74_0:1;						// BIT[06]
    UINT16	fail_24x98_0:1;						// BIT[07]
    UINT16	fail_24x98_1:1;						// BIT[08]
    UINT16	res1_2:7;							// BIT[09:15]
} _mbist_a2_eiaj_demod_RBUS;

typedef union
{
    UINT32						regValue;
    _mbist_a2_eiaj_demod_RBUS	bitField;
} mbist_a2_eiaj_demod_RBUS;
// --------------------------------
typedef struct
{
    UINT16	magnitude:16;							// BIT[16:31]
    UINT16	reserved_dummy:16;						// BIT[00:15]
} _a2_eiaj_dc_filter_RBUS;

typedef union
{
    UINT32						regValue;
    _a2_eiaj_dc_filter_RBUS		bitField;
} a2_eiaj_dc_filter_RBUS;
// --------------------------------
typedef union
{
    UINT32						regValue;
    WORD_BBBB					bitField;
    // this bit field is to large to fit in 16bit-width(max. bit-field for C51)
    /*
    struct{
        RBus_UInt32  tr_timing_step:20;						// BIT[00:19]
        RBus_UInt32  cr_lpf_ki:2;							// BIT[20:21]
        RBus_UInt32  res2:2;								// BIT[22:23]
        RBus_UInt32  cr_lpf_kp:3;							// BIT[24:26]
        RBus_UInt32  res1:1;								// BIT[27]
        RBus_UInt32  tr_lpf_kp:3;							// BIT[28:30]
        RBus_UInt32  psf_sel:1;								// BIT[31]
    };
    */
} nicam_demod_RBUS;
// --------------------------------
typedef struct
{
    UINT16	window_size:10;						// BIT[16:25]
    UINT16	res1:2;								// BIT[26:27]
    UINT16	debounce_cnt:3;						// BIT[28:30]
    UINT16	enable:1;							// BIT[31]

    UINT16	gain_adjust:6;						// BIT[00:05]
    UINT16	res2:10;							// BIT[06:15]
} _nicam_dagc_ctrl_RBUS;

typedef union
{
    UINT32						regValue;
    _nicam_dagc_ctrl_RBUS		bitField;
} nicam_dagc_ctrl_RBUS;
// --------------------------------
typedef struct
{
    UINT16	high:16;							// BIT[16:31]
    UINT16	low:16;								// BIT[00:15]
} _nicam_dagc_threshold_RBUS;

typedef union
{
    UINT32						regValue;
    _nicam_dagc_threshold_RBUS	bitField;
} nicam_dagc_threshold_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1:16;							// BIT[16:31]
    UINT16	magnitude:16;						// BIT[00:15]
} _nicam_dagc_mag_RBUS;

typedef union
{
    UINT32						regValue;
    _nicam_dagc_mag_RBUS		bitField;
} nicam_dagc_mag_RBUS;
// --------------------------------
typedef struct
{
    UINT8	res1:4;								// BIT[24:27]
    UINT8	pcm_r_out_sel:1;					// BIT[28]
    UINT8	pcm_l_out_sel:1;					// BIT[29]
    UINT8	data_channel_sel:1;					// BIT[30]
    UINT8	resampler_ctrl:1;					// BIT[31]

    UINT8	res2:4;								// BIT[16:19]
    UINT8	status_debounce_cnt:4;				// BIT[20:23]

    UINT16	reserved_dummy:16;					// BIT[00:15]
} _nicam_decode_ctrl_RBUS;

typedef union
{
    UINT32						regValue;
    _nicam_decode_ctrl_RBUS		bitField;
} nicam_decode_ctrl_RBUS;
// --------------------------------
typedef struct
{
    UINT16	additional_data:11;					// BIT[16:26]
    UINT16	c4:1;								// BIT[27]
    UINT16	c1c2c3:3;							// BIT[28:30]
    UINT16	c0:1;								// BIT[31]

    UINT16	bit_error_rate:12;					// BIT[00:11]
    UINT16	true_detect:1;						// BIT[12]
    UINT16	res1:3;								// BIT[13:15]
} _nicam_decode_status_RBUS;

typedef union
{
    UINT32						regValue;
    _nicam_decode_status_RBUS	bitField;
} nicam_decode_status_RBUS;
// --------------------------------
typedef union
{
    UINT32						regValue;
    WORD_BBBB					bitField;
    // this bit field is to large to fit in 16bit-width(max. bit-field for C51)
    /*
    struct{
        RBus_UInt32  low_threshold:12;						// BIT[00:11]
        RBus_UInt32  high_threshold:12;						// BIT[12:23]
        RBus_UInt32  hi_ber_debounce:3;						// BIT[24:26]
        RBus_UInt32  analog_flag:1;							// BIT[27]
        RBus_UInt32  c4_hi_ber:2;							// BIT[28:29]
        RBus_UInt32  res1:1;								// BIT[30]
        RBus_UInt32  enable:1;								// BIT[31]
    };*/
} nicam_auto_switch_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1_1:16;								// BIT[16:31]

    UINT8	res1_2:8;								// BIT[08:15]

    UINT8	down_rate:1;							// BIT[00]
    UINT8	downsample_en:1;						// BIT[01]
    UINT8	backend_play_en:1;						// BIT[02]
    UINT8	res1_3:5;								// BIT[03:07]
} _down_sample_control_for_playback_RBUS;

typedef union
{
    UINT32									regValue;
    _down_sample_control_for_playback_RBUS	bitField;
} down_sample_control_for_playback_RBUS;
// --------------------------------
typedef struct
{
    UINT8	res1_1:8;								// BIT[24:31]

    UINT8	fifo_ch12_scale:2;						// BIT[16:17]
    UINT8	fifo_ch2_source:1;						// BIT[18]
    UINT8	fifo_ch1_source:1;						// BIT[19]
    UINT8	res1_2:4;								// BIT[20:23]

    UINT8	fifo_ch56_sel:4;						// BIT[08:11]
    UINT8	fifo_ch78_sel:4;						// BIT[12:15]

    UINT8	fifo_ch12_sel:4;						// BIT[00:03]
    UINT8	fifo_ch34_sel:4;						// BIT[04:07]
} _down_sample_fifo_source_sel_RBUS;

typedef union
{
    UINT32								regValue;
    _down_sample_fifo_source_sel_RBUS	bitField;
} down_sample_fifo_source_sel_RBUS;
// --------------------------------
typedef struct
{
    UINT16	dummy_6558_1:16;						// BIT[16:31]

    UINT16	playback_bist_fail:1;					// BIT[00]
    UINT16	playback_bist_done:1;					// BIT[01]
    UINT16	playback_bist_mode:1;					// BIT[02]
    UINT16	playback_bist_rstn:1;					// BIT[03]
    UINT16	dummy_6558_2:12;						// BIT[04:15]
} _down_sample_mbist_recoding_RBUS;

typedef union
{
    UINT32								regValue;
    _down_sample_mbist_recoding_RBUS	bitField;
} down_sample_mbist_recoding_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1:15;							// BIT[16:30]
    UINT16	hw_8ch_bypass_en:1;					// BIT[31]

    UINT16	dvcm:2;								// BIT[00:01]
    UINT16	res3:2;								// BIT[02:03]
    UINT16	zcth:5;								// BIT[04:08]
    UINT16	res2:1;								// BIT[09]
    UINT16	auto_volenable:1;					// BIT[10]
    UINT16	spatial_ctrlenable:1;				// BIT[11]
    UINT16	deq_enable:1;						// BIT[12]
    UINT16	loud_enable:1;						// BIT[13]
    UINT16	dvol_enable:1;						// BIT[14]
    UINT16	basic_premium_sel:1;				// BIT[15]
} _hw_process_control_RBUS;

typedef union
{
    UINT32						regValue;
    _hw_process_control_RBUS	bitField;
} hw_process_control_RBUS;
// --------------------------------
typedef struct
{
    UINT16	delay_ctrl:10;						// BIT[16:25]
    UINT16	ramp_step:2;						// BIT[26:27]
    UINT16	zcto:3;								// BIT[28:30]
    UINT16	dvol_apply:1;						// BIT[31]

    UINT16	delay_ctrl_mute:10;					// BIT[00:09]
    UINT16	ramp_step_mute:2;					// BIT[10:11]
    UINT16	zcto_mute:3;						// BIT[12:14]
    UINT16	mute_apply:1;						// BIT[15]
} _digital_volume_control_0_RBUS;

typedef union
{
    UINT32							regValue;
    _digital_volume_control_0_RBUS	bitField;
} digital_volume_control_0_RBUS;
// --------------------------------
typedef struct
{
    UINT16	vol_l_b:6;							// BIT[16:21]
    UINT16	vol_l_a:5;							// BIT[22:26]
    UINT16	vol_l_ready:1;						// BIT[27]
    UINT16	hpf_l_enable:1;						// BIT[28]
    UINT16	res1:1;								// BIT[29]
    UINT16	rep_l_status:1;						// BIT[30]
    UINT16	mute_l:1;							// BIT[31]

    UINT16	vol_r_b:6;							// BIT[00:05]
    UINT16	vol_r_a:5;							// BIT[06:10]
    UINT16	vol_r_ready:1;						// BIT[11]
    UINT16	hpf_r_enable:1;						// BIT[12]
    UINT16	res2:1;								// BIT[13]
    UINT16	rep_r_status:1;						// BIT[14]
    UINT16	mute_r:1;							// BIT[15]
} _lr_digital_volume_control_RBUS;

typedef union
{
    UINT32							regValue;
    _lr_digital_volume_control_RBUS	bitField;
} lr_digital_volume_control_RBUS;
// --------------------------------
typedef struct
{
    UINT16	vol_ls_b:6;							// BIT[16:21]
    UINT16	vol_ls_a:5;							// BIT[22:26]
    UINT16	vol_ls_ready:1;						// BIT[27]
    UINT16	hpf_ls_enable:1;					// BIT[28]
    UINT16	res1:1;								// BIT[29]
    UINT16	rep_ls_status:1;					// BIT[30]
    UINT16	mute_ls:1;							// BIT[31]

    UINT16	vol_rs_b:6;							// BIT[00:05]
    UINT16	vol_rs_a:5;							// BIT[06:10]
    UINT16	vol_rs_ready:1;						// BIT[11]
    UINT16	hpf_rs_enable:1;					// BIT[12]
    UINT16	res2:1;								// BIT[13]
    UINT16	rep_rs_status:1;					// BIT[14]
    UINT16	mute_rs:1;							// BIT[15]
} _ls_digital_volume_control_RBUS;

typedef union
{
    UINT32							regValue;
    _ls_digital_volume_control_RBUS	bitField;
} ls_digital_volume_control_RBUS;
// --------------------------------
typedef struct
{
    UINT16	vol_c_b:6;							// BIT[16:21]
    UINT16	vol_c_a:5;							// BIT[22:26]
    UINT16	vol_c_ready:1;						// BIT[27]
    UINT16	hpf_c_enable:1;						// BIT[28]
    UINT16	res1:1;								// BIT[29]
    UINT16	rep_c_status:1;						// BIT[30]
    UINT16	mute_c:1;							// BIT[31]

    UINT16	vol_sw_b:6;							// BIT[00:05]
    UINT16	vol_sw_a:5;							// BIT[06:10]
    UINT16	vol_sw_ready:1;						// BIT[11]
    UINT16	hpf_sw_enable:1;					// BIT[12]
    UINT16	sw_source:1;						// BIT[13]
    UINT16	rep_sw_status:1;					// BIT[14]
    UINT16	mute_sw:1;							// BIT[15]
} _c_sw_digital_volume_control_RBUS;

typedef union
{
    UINT32								regValue;
    _c_sw_digital_volume_control_RBUS	bitField;
} c_sw_digital_volume_control_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1_1:16;							// BIT[16:31]

    UINT8	res1_2:8;							// BIT[08:15]

    UINT8	double_buffer_ack:1;				// BIT[00]
    UINT8	zcto_status:1;						// BIT[01]
    UINT8	res1_3:6;							// BIT[02:07]
} _double_buffer_ack_RBUS;

typedef union
{
    UINT32						regValue;
    _double_buffer_ack_RBUS		bitField;
} double_buffer_ack_RBUS;
// --------------------------------
typedef union
{
    UINT32						regValue;
    WORD_BBBB					bitField;
    /*
    struct{
        RBus_UInt32  dummy_6718:32;							// BIT[00:31]
    };*/
} dummy_6718_RBUS;
// --------------------------------
typedef union
{
    UINT32						regValue;
    WORD_BBBB					bitField;
    /*
    struct{
        RBus_UInt32  dummy_671c:32;							// BIT[00:31]
    };*/
} dummy_671c_RBUS;
// --------------------------------
typedef union
{
    UINT32						regValue;
    WORD_BBBB					bitField;
    /*
    struct{
        RBus_UInt32  dummy_6720:32;							// BIT[00:31]
    };*/
} dummy_6720_RBUS;
// --------------------------------
typedef union
{
    UINT32						regValue;
    WORD_BBBB					bitField;
    /*
    struct{
        RBus_UInt32  dummy_6724:32;							// BIT[00:31]
    };*/
} dummy_6724_RBUS;
// --------------------------------
typedef union
{
    UINT32						regValue;
    WORD_BBBB					bitField;
    /*
    struct{
        RBus_UInt32  res1:32;								// BIT[00:31]
    };*/
} dummy_6728_RBUS;
// --------------------------------
typedef struct
{
    UINT8	freq_beeper:8;						// BIT[24:31]

    UINT8	beeper_volume:4;					// BIT[16:19]
    UINT8	res1:4;								// BIT[20:23]

    UINT8	res2:6;								// BIT[08:13]
    UINT8	mode_beeper:1;						// BIT[14]
    UINT8	mute_beeper:1;						// BIT[15]

    UINT8	duration_beeper:8;					// BIT[00:07]
} _programmable_beeper_control_RBUS;

typedef union
{
    UINT32								regValue;
    _programmable_beeper_control_RBUS	bitField;
} programmable_beeper_control_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1_1:16;							// BIT[16:31]

    UINT16	f_coef:9;							// BIT[00:08]
    UINT16	res2:1;								// BIT[09]
    UINT16	spatial_in_r_sel:1;					// BIT[10]
    UINT16	spatial_in_l_sel:1;					// BIT[11]
    UINT16	seb_en:1;							// BIT[12]
    UINT16	psb_en:1;							// BIT[13]
    UINT16	apf_en:1;							// BIT[14]
    UINT16	res1_2:1;							// BIT[15]
} _spatial_effect_control_0_RBUS;

typedef union
{
    UINT32							regValue;
    _spatial_effect_control_0_RBUS	bitField;
} spatial_effect_control_0_RBUS;
// --------------------------------
typedef struct
{
    UINT16	depth1:9;							// BIT[16:24]
    UINT16	res1:7;								// BIT[25:31]

    UINT16	depth2:9;							// BIT[00:08]
    UINT16	res2:7;								// BIT[09:15]
} _spatial_effect_control_1_RBUS;

typedef union
{
    UINT32							regValue;
    _spatial_effect_control_1_RBUS	bitField;
} spatial_effect_control_1_RBUS;
// --------------------------------
typedef union
{
    UINT32						regValue;
    WORD_BBBB					bitField;
    // this bit field is to large to fit in 16bit-width(max. bit-field for C51)
    /*
    struct{
        RBus_UInt32  avc_up_db:2;							// BIT[00:01]
        RBus_UInt32  avc_down_db:2;							// BIT[02:03]
        RBus_UInt32  avc_level_min:8;						// BIT[04:11]
        RBus_UInt32  avc_level_max:8;						// BIT[12:19]
        RBus_UInt32  avc_noise_thre:8;						// BIT[20:27]
        RBus_UInt32  avc_noise_gain:2;						// BIT[28:29]
        RBus_UInt32  res1:2;								// BIT[30:31]
    };*/
} auto_volume_control_0_RBUS;
// --------------------------------
typedef union
{
    UINT32						regValue;
    WORD_BBBB					bitField;
    // this bit field is to large to fit in 16bit-width(max. bit-field for C51)
    /*
    struct{
        RBus_UInt32  avc_mon_win:12;						// BIT[00:11]
        RBus_UInt32  avc_zc_win:12;							// BIT[12:23]
        RBus_UInt32  avc_cnt_thre:8;						// BIT[24:31]
    };*/
} avc_volume_control_1_RBUS;
// --------------------------------
typedef struct
{
    UINT16	eq_lp_a1:16;							// BIT[16:31]
    UINT16	eq_bp1_a1:16;							// BIT[00:15]
} _eq_a1_0_RBUS;

typedef union
{
    UINT32						regValue;
    _eq_a1_0_RBUS				bitField;
} eq_a1_0_RBUS;
// --------------------------------
typedef struct
{
    UINT16	eq_bp2_a1:16;							// BIT[16:31]
    UINT16	eq_bp3_a1:16;							// BIT[00:15]
} _eq_a1_1_RBUS;

typedef union
{
    UINT32						regValue;
    _eq_a1_1_RBUS				bitField;
} eq_a1_1_RBUS;
// --------------------------------
typedef struct
{
    UINT16	eq_bp4_a1:16;							// BIT[16:31]
    UINT16	eq_bp5_a1:16;							// BIT[00:15]
} _eq_a1_2_RBUS;

typedef union
{
    UINT32						regValue;
    _eq_a1_2_RBUS				bitField;
} eq_a1_2_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1:16;								// BIT[16:31]
    UINT16	eq_hp_a1:16;							// BIT[00:15]
} _eq_a1_3_RBUS;

typedef union
{
    UINT32						regValue;
    _eq_a1_3_RBUS				bitField;
} eq_a1_3_RBUS;
// --------------------------------
typedef struct
{
    UINT16	eq_bp1_a2:16;							// BIT[16:31]
    UINT16	eq_bp2_a2:16;							// BIT[00:15]
} _eq_a2_1_RBUS;

typedef union
{
    UINT32						regValue;
    _eq_a2_1_RBUS				bitField;
} eq_a2_1_RBUS;
// --------------------------------
typedef struct
{
    UINT16	eq_bp3_a2:16;							// BIT[16:31]
    UINT16	eq_bp4_a2:16;							// BIT[00:15]
} _eq_a2_2_RBUS;

typedef union
{
    UINT32						regValue;
    _eq_a2_2_RBUS				bitField;
} eq_a2_2_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1:16;								// BIT[16:31]
    UINT16	eq_bp5_a2:16;							// BIT[00:15]
} _eq_a2_3_RBUS;

typedef union
{
    UINT32						regValue;
    _eq_a2_3_RBUS				bitField;
} eq_a2_3_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1_1:16;								// BIT[16:31]

    UINT8	res1_2:8;								// BIT[08:15]

    UINT8	eq_hp_slope:1;							// BIT[00]
    UINT8	res1_3:7;								// BIT[01:07]
} _eq_h0_0_RBUS;

typedef union
{
    UINT32						regValue;
    _eq_h0_0_RBUS				bitField;
} eq_h0_0_RBUS;
// --------------------------------
typedef struct
{
    UINT16	eq_lp_h0:16;							// BIT[16:31]
    UINT16	eq_bp1_h0:16;							// BIT[00:15]
} _eq_h0_1_RBUS;

typedef union
{
    UINT32						regValue;
    _eq_h0_1_RBUS				bitField;
} eq_h0_1_RBUS;
// --------------------------------
typedef struct
{
    UINT16	eq_bp2_h0:16;							// BIT[16:31]
    UINT16	eq_bp3_h0:16;							// BIT[00:15]
} _eq_h0_2_RBUS;

typedef union
{
    UINT32						regValue;
    _eq_h0_2_RBUS				bitField;
} eq_h0_2_RBUS;
// --------------------------------
typedef struct
{
    UINT16	eq_bp4_h0:16;							// BIT[16:31]
    UINT16	eq_bp5_h0:16;							// BIT[00:15]
} _eq_h0_3_RBUS;

typedef union
{
    UINT32						regValue;
    _eq_h0_3_RBUS				bitField;
} eq_h0_3_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1:16;							// BIT[16:31]

    UINT8	eq_hp_enable:1;						// BIT[08]
    UINT8	eq_lp_enable:1;						// BIT[09]
    UINT8	eq_band1_enable:1;					// BIT[10]
    UINT8	eq_band2_enable:1;					// BIT[11]
    UINT8	eq_band3_enable:1;					// BIT[12]
    UINT8	eq_band4_enable:1;					// BIT[13]
    UINT8	eq_band5_enable:1;					// BIT[14]
    UINT8	res2:1;								// BIT[15]
    UINT8	eq_hp_of:1;							// BIT[00]
    UINT8	eq_lp_of:1;							// BIT[01]
    UINT8	eq_band1_of:1;						// BIT[02]
    UINT8	eq_band2_of:1;						// BIT[03]
    UINT8	eq_band3_of:1;						// BIT[04]
    UINT8	eq_band4_of:1;						// BIT[05]
    UINT8	eq_band5_of:1;						// BIT[06]
    UINT8	res3:1;								// BIT[07]
} _eq_control_status_register_RBUS;

typedef union
{
    UINT32								regValue;
    _eq_control_status_register_RBUS	bitField;
} eq_control_status_register_RBUS;
// --------------------------------
typedef struct
{
    UINT16	lr_loud_lp0_a1:16;						// BIT[16:31]
    UINT16	lr_loud_bp1_a1:16;						// BIT[00:15]
} _lr_loud_a1_0_RBUS;

typedef union
{
    UINT32						regValue;
    _lr_loud_a1_0_RBUS			bitField;
} lr_loud_a1_0_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1:16;								// BIT[16:31]
    UINT16	lr_loud_bp2_a1:16;						// BIT[00:15]
} _lr_loud_a1_1_RBUS;

typedef union
{
    UINT32						regValue;
    _lr_loud_a1_1_RBUS			bitField;
} lr_loud_a1_1_RBUS;
// --------------------------------
typedef struct
{
    UINT16	lr_loud_bp1_a2:16;						// BIT[16:31]
    UINT16	lr_loud_bp2_a2:16;						// BIT[00:15]
} _lr_loud_a2_RBUS;

typedef union
{
    UINT32						regValue;
    _lr_loud_a2_RBUS			bitField;
} lr_loud_a2_RBUS;
// --------------------------------
typedef struct
{
    UINT16	lr_loud_lp0_h0:16;						// BIT[16:31]
    UINT16	lr_loud_bp1_h0:16;						// BIT[00:15]
} _lr_loud_h0_0_RBUS;

typedef union
{
    UINT32						regValue;
    _lr_loud_h0_0_RBUS			bitField;
} lr_loud_h0_0_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1:16;								// BIT[16:31]
    UINT16	lr_loud_bp2_h0:16;						// BIT[00:15]
} _lr_loud_h0_1_RBUS;

typedef union
{
    UINT32						regValue;
    _lr_loud_h0_1_RBUS			bitField;
} lr_loud_h0_1_RBUS;
// --------------------------------
typedef struct
{
    UINT16	lsrs_loud_lp0_a1:16;					// BIT[16:31]
    UINT16	lsrs_loud_bp1_a1:16;					// BIT[00:15]
} _lsrs_loud_a1_0_RBUS;

typedef union
{
    UINT32						regValue;
    _lsrs_loud_a1_0_RBUS		bitField;
} lsrs_loud_a1_0_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1:16;								// BIT[16:31]
    UINT16	lsrs_loud_bp2_a1:16;					// BIT[00:15]
} _lsrs_loud_a1_1_RBUS;

typedef union
{
    UINT32						regValue;
    _lsrs_loud_a1_1_RBUS		bitField;
} lsrs_loud_a1_1_RBUS;
// --------------------------------
typedef struct
{
    UINT16	lsrs_loud_bp1_a2:16;					// BIT[16:31]
    UINT16	lsrs_loud_bp2_a2:16;					// BIT[00:15]
} _lsrs_loud_a2_RBUS;

typedef union
{
    UINT32						regValue;
    _lsrs_loud_a2_RBUS			bitField;
} lsrs_loud_a2_RBUS;
// --------------------------------
typedef struct
{
    UINT16	lsrs_loud_lp0_h0:16;					// BIT[16:31]
    UINT16	lsrs_loud_bp1_h0:16;					// BIT[00:15]
} _lsrs_loud_h0_0_RBUS;

typedef union
{
    UINT32						regValue;
    _lsrs_loud_h0_0_RBUS		bitField;
} lsrs_loud_h0_0_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1:16;								// BIT[16:31]
    UINT16	lsrs_loud_bp2_h0:16;					// BIT[00:15]
} _lsrs_loud_h0_1_RBUS;

typedef union
{
    UINT32						regValue;
    _lsrs_loud_h0_1_RBUS		bitField;
} lsrs_loud_h0_1_RBUS;
// --------------------------------
typedef struct
{
    UINT16	csw_loud_lp0_a1:16;					// BIT[16:31]
    UINT16	csw_loud_bp1_a1:16;					// BIT[00:15]
} _csw_loud_a1_0_RBUS;

typedef union
{
    UINT32						regValue;
    _csw_loud_a1_0_RBUS			bitField;
} csw_loud_a1_0_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1:16;							// BIT[16:31]
    UINT16	csw_loud_bp2_a1:16;					// BIT[00:15]
} _csw_loud_a1_1_RBUS;

typedef union
{
    UINT32						regValue;
    _csw_loud_a1_1_RBUS			bitField;
} csw_loud_a1_1_RBUS;
// --------------------------------
typedef struct
{
    UINT16	csw_loud_bp1_a2:16;					// BIT[16:31]
    UINT16	csw_loud_bp2_a2:16;					// BIT[00:15]
} _csw_loud_a2_RBUS;

typedef union
{
    UINT32						regValue;
    _csw_loud_a2_RBUS			bitField;
} csw_loud_a2_RBUS;
// --------------------------------
typedef struct
{
    UINT16	csw_loud_lp0_h0:16;					// BIT[16:31]
    UINT16	csw_loud_bp1_h0:16;					// BIT[00:15]
} _csw_loud_h0_0_RBUS;

typedef union
{
    UINT32						regValue;
    _csw_loud_h0_0_RBUS			bitField;
} csw_loud_h0_0_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1:16;							// BIT[16:31]
    UINT16	csw_loud_bp2_h0:16;					// BIT[00:15]
} _csw_loud_h0_1_RBUS;

typedef union
{
    UINT32						regValue;
    _csw_loud_h0_1_RBUS			bitField;
} csw_loud_h0_1_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1_1:16;								// BIT[16:31]

    UINT8	ld_bpf1_en_lr:1;						// BIT[08]
    UINT8	res1_2:7;								// BIT[09:15]

    UINT8	ld_bpf2_en_csw:1;						// BIT[00]
    UINT8	ld_lpf_en_csw:1;						// BIT[01]
    UINT8	ld_bpf1_en_csw:1;						// BIT[02]
    UINT8	ld_bpf2_en_lsrs:1;						// BIT[03]
    UINT8	ld_lpf_en_lsrs:1;						// BIT[04]
    UINT8	ld_bpf1_en_lsrs:1;						// BIT[05]
    UINT8	ld_bpf2_en_lr:1;						// BIT[06]
    UINT8	ld_lpf_en_lr:1;							// BIT[07]
} _loudness_control_register_RBUS;

typedef union
{
    UINT32							regValue;
    _loudness_control_register_RBUS	bitField;
} loudness_control_register_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1_1:16;								// BIT[16:31]

    UINT8	ld_lpf_of_lr:1;							// BIT[08]
    UINT8	res1_2:7;								// BIT[09:15]

    UINT8	ld_bpf2_of_csw:1;						// BIT[00]
    UINT8	ld_bpf1_of_csw:1;						// BIT[01]
    UINT8	ld_lpf_of_csw:1;						// BIT[02]
    UINT8	ld_bpf2_of_lsrs:1;						// BIT[03]
    UINT8	ld_bpf1_of_lsrs:1;						// BIT[04]
    UINT8	ld_lpf_of_lsrs:1;						// BIT[05]
    UINT8	ld_bpf2_of_lr:1;						// BIT[06]
    UINT8	ld_bpf1_of_lr:1;						// BIT[07]
} _loudness_status_register_RBUS;

typedef union
{
    UINT32							regValue;
    _loudness_status_register_RBUS	bitField;
} loudness_status_register_RBUS;
// --------------------------------
typedef struct
{
    UINT8	res1_1:8;							// BIT[24:31]

    UINT8	loudness_ovf_en:1;					// BIT[16]
    UINT8	eq_ovf_en:1;						// BIT[17]
    UINT8	res1_2:6;							// BIT[18:23]

    UINT8	res2_1:7;							// BIT[08:14]
    UINT8	dvol_int_en:1;						// BIT[15]

    UINT8	loudness_ovf:1;						// BIT[00]
    UINT8	eq_ovf:1;							// BIT[01]
    UINT8	dvol_done:1;						// BIT[02]
    UINT8	res2_2:5;							// BIT[03:07]
} _system_int_en_RBUS;

typedef union
{
    UINT32						regValue;
    _system_int_en_RBUS			bitField;
} system_int_en_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1_1:16;							// BIT[16:31]

    UINT8	res1_2:8;							// BIT[08:15]

    UINT8	hw_processing_source_sel:1;			// BIT[00]
    UINT8	res1_3:7;							// BIT[01:07]
} _test_data_source_RBUS;

typedef union
{
    UINT32						regValue;
    _test_data_source_RBUS		bitField;
} test_data_source_RBUS;
// --------------------------------
typedef struct
{
    UINT8	hw_i2s_out_1_sel:2;					// BIT[24:25]
    UINT8	hw_i2s_out_0_sel:2;					// BIT[26:27]
    UINT8	res1:4;								// BIT[28:31]

    UINT8	hw_dac_out_sel:2;					// BIT[16:17]
    UINT8	hw_spdif_out_sel:2;					// BIT[18:19]
    UINT8	hw_i2s_out_3_sel:2;					// BIT[20:21]
    UINT8	hw_i2s_out_2_sel:2;					// BIT[22:23]

    UINT8	res2_1:6;							// BIT[08:13]
    UINT8	hw_pcm_out_sel:2;					// BIT[14:15]

    UINT8	w_ch_mix_factor:2;					// BIT[00:01]
    UINT8	w_ch_12_factor:2;					// BIT[02:03]
    UINT8	hw_mixing_sel:2;					// BIT[04:05]
    UINT8	res2_2:2;							// BIT[06:07]
} _hw_backplay_transfer_RBUS;

typedef union
{
    UINT32						regValue;
    _hw_backplay_transfer_RBUS	bitField;
} hw_backplay_transfer_RBUS;
// --------------------------------
typedef struct
{
    UINT16	lrrr_delay_ctrl:10;					// BIT[16:25]
    UINT16	lrrr_ramp_step:2;					// BIT[26:27]
    UINT16	lrrr_zcto:3;						// BIT[28:30]
    UINT16	lrrr_dvol_apply:1;					// BIT[31]

    UINT16	lrrr_delay_ctrl_mute:10;			// BIT[00:09]
    UINT16	lrrr_ramp_step_mute:2;				// BIT[10:11]
    UINT16	lrrr_zcto_mute:3;					// BIT[12:14]
    UINT16	lrrr_mute_apply:1;					// BIT[15]
} _lrrr_digital_volume_control_0_RBUS;

typedef union
{
    UINT32								regValue;
    _lrrr_digital_volume_control_0_RBUS	bitField;
} lrrr_digital_volume_control_0_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1_1:16;							// BIT[16:31]

    UINT16	lrrr_dvcm:2;						// BIT[00:01]
    UINT16	res2:2;								// BIT[02:03]
    UINT16	lrrr_zcth:5;						// BIT[04:08]
    UINT16	lrrr_zcto_status:1;					// BIT[09]
    UINT16	lrrr_dvol_enable:1;					// BIT[10]
    UINT16	res1_2:5;							// BIT[11:15]
} _lrrr_digital_volume_control_1_RBUS;

typedef union
{
    UINT32								regValue;
    _lrrr_digital_volume_control_1_RBUS	bitField;
} lrrr_digital_volume_control_1_RBUS;
// --------------------------------
typedef struct
{
    UINT16	vol_lr_b:6;							// BIT[16:21]
    UINT16	vol_lr_a:5;							// BIT[22:26]
    UINT16	vol_lr_ready:1;						// BIT[27]
    UINT16	hpf_lr_enable:1;					// BIT[28]
    UINT16	res1:2;								// BIT[29:30]
    UINT16	mute_lr:1;							// BIT[31]

    UINT16	vol_rr_b:6;							// BIT[00:05]
    UINT16	vol_rr_a:5;							// BIT[06:10]
    UINT16	vol_rr_ready:1;						// BIT[11]
    UINT16	hpf_rr_enable:1;					// BIT[12]
    UINT16	res2:2;								// BIT[13:14]
    UINT16	mute_rr:1;							// BIT[15]
} _lrrr_digital_volume_control_2_RBUS;

typedef union
{
    UINT32								regValue;
    _lrrr_digital_volume_control_2_RBUS	bitField;
} lrrr_digital_volume_control_2_RBUS;
// --------------------------------
typedef union
{
    UINT32						regValue;
    WORD_BBBB					bitField;
    /*
    struct{
        RBus_UInt32  dummy_6810:32;							// BIT[00:31]
    };*/
} dummy_6810_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1_1:16;							// BIT[16:31]

    UINT8	res1_2:8;							// BIT[08:15]

    UINT8	vol_source_sel:3;					// BIT[00:02]
    UINT8	res2:1;								// BIT[03]
    UINT8	ch2_source_sel:1;					// BIT[04]
    UINT8	ch1_source_sel:1;					// BIT[05]
    UINT8	res1_3:2;							// BIT[06:07]
} _dvol_control_RBUS;

typedef union
{
    UINT32						regValue;
    _dvol_control_RBUS			bitField;
} dvol_control_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1_1:16;							// BIT[16:31]

    UINT8	int_pattern_gen_en:1;				// BIT[08]
    UINT8	test_fifo_read_enable:1;			// BIT[09]
    UINT8	res2:2;								// BIT[10:11]
    UINT8	sif_adc_in_test_sel:1;				// BIT[12]
    UINT8	sif_adc_in_test_en:1;				// BIT[13]
    UINT8	res1_2:2;							// BIT[14:15]

    UINT8	test_fifo_out_sel:3;				// BIT[00:02]
    UINT8	res3:1;								// BIT[03]
    UINT8	sin_amplitude:2;					// BIT[04:05]
    UINT8	int_pattern_sel:2;					// BIT[06:07]
} _test_pattern_gen_RBUS;

typedef union
{
    UINT32						regValue;
    _test_pattern_gen_RBUS		bitField;
} test_pattern_gen_RBUS;
// --------------------------------
typedef union
{
    UINT32						regValue;
    WORD_BBBB					bitField;
    // this bit field is to large to fit in 16bit-width(max. bit-field for C51)
    /*
    struct{
        RBus_UInt32  audio_crc_header:24;		// BIT[00:23]
        RBus_UInt32  res1:8;					// BIT[24:31]
    };*/
} crc_header_RBUS;
// --------------------------------
typedef struct
{
    UINT16	audio_crc_ch_sel:3;					// BIT[16:18]
    UINT16	audio_crc_en:1;						// BIT[19]
    UINT16	res1:12;							// BIT[20:31]

    UINT16	audio_crc_frame_length:16;			// BIT[00:15]
} _crc_contorl_RBUS;

typedef union
{
    UINT32						regValue;
    _crc_contorl_RBUS			bitField;
} crc_contorl_RBUS;
// --------------------------------
typedef union
{
    UINT32						regValue;
    // this bit field is to large to fit in 16bit-width(max. bit-field for C51)
    /*
    struct{
        RBus_UInt32  audio_crc:24;							// BIT[00:23]
        RBus_UInt32  audio_crc_done:1;						// BIT[24]
        RBus_UInt32  res1:7;								// BIT[25:31]
    };*/
} crc_status_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1:16;								// BIT[16:31]

    UINT8	audio_digital_output_ch_sel:3;			// BIT[08:10]
    UINT8	adc_test_ch_sel:1;						// BIT[11]
    UINT8	bist_fail_rom:1;						// BIT[12]
    UINT8	bist_done_rom:1;						// BIT[13]
    UINT8	bist_mode_rom:1;						// BIT[14]
    UINT8	bist_rstn_rom:1;						// BIT[15]

    UINT8	debug_mode_sel:8;						// BIT[00:07]
} _test_debug_mode_RBUS;

typedef union
{
    UINT32						regValue;
    _test_debug_mode_RBUS		bitField;
} test_debug_mode_RBUS;
// --------------------------------
typedef union
{
    UINT32						regValue;
    WORD_BBBB					bitField;
    /*
    struct{
        RBus_UInt32  dummy_6864:32;							// BIT[00:31]
    };*/
} pattern_gen_dummy_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1_1:16;							// BIT[16:31]

    UINT8	res1_2:8;							// BIT[08:15]

    UINT8	to_ack_en:1;						// BIT[00]
    UINT8	dummy_6890:3;						// BIT[01:03]
    UINT8	res1_3:4;							// BIT[04:07]
} _rbus_ack_ctrl_RBUS;

typedef union
{
    UINT32						regValue;
    _rbus_ack_ctrl_RBUS			bitField;
} rbus_ack_ctrl_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1_1:16;								// BIT[16:31]

    UINT16	ack_fail_addr:12;						// BIT[00:11]
    UINT16	ack_fail_rw:1;							// BIT[12]
    UINT16	res1_2:13;								// BIT[13:15]
} _ack_fail_info_RBUS;

typedef union
{
    UINT32						regValue;
    _ack_fail_info_RBUS			bitField;
} ack_fail_info_RBUS;
// --------------------------------
typedef struct
{
    UINT8	dac2_afifo_empty:1;					// BIT[24]
    UINT8	dac_afifo_empty:1;					// BIT[25]
    UINT8	dac2_afifo_full:1;					// BIT[26]
    UINT8	dac_afifo_full:1;					// BIT[27]
    UINT8	res1:2;								// BIT[28:29]
    UINT8	dac2_afifo_enable:1;				// BIT[30]
    UINT8	dac_afifo_enable:1;					// BIT[31]

    UINT8	res2:4;								// BIT[16:19]
    UINT8	dac2_afifo_in_drop_sel:2;			// BIT[20:21]
    UINT8	dac_afifo_in_drop_sel:2;			// BIT[22:23]

    UINT8	bist_fail_dac1:1;					// BIT[08]
    UINT8	bist_done_dac1:1;					// BIT[09]
    UINT8	bist_rstn_dac1:1;					// BIT[10]
    UINT8	bist_mode_dac1:1;					// BIT[11]
    UINT8	bist_fail_adc:1;					// BIT[12]
    UINT8	bist_done_adc:1;					// BIT[13]
    UINT8	bist_rstn_adc:1;					// BIT[14]
    UINT8	bist_mode_adc:1;					// BIT[15]

    UINT8	sel_daad_lp:1;						// BIT[00]
    UINT8	sel_1ch_2ch:1;						// BIT[01]
    UINT8	sel_adc_2ch_lp_source:1;			// BIT[02]
    UINT8	sel_adda_lp:1;						// BIT[03]
    UINT8	bist_fail_dac0:1;					// BIT[04]
    UINT8	bist_done_dac0:1;					// BIT[05]
    UINT8	bist_rstn_dac0:1;					// BIT[06]
    UINT8	bist_mode_dac0:1;					// BIT[07]
} _adda_loopback_RBUS;

typedef union
{
    UINT32						regValue;
    _adda_loopback_RBUS			bitField;
} adda_loopback_RBUS;
// --------------------------------
typedef struct
{
    UINT8	res1:8;								// BIT[24:31]

    UINT8	reg_bb_mono_io:1;					// BIT[16]
    UINT8	reg_bb_aio2_io:1;					// BIT[17]
    UINT8	reg_bb_aio1_io:1;					// BIT[18]
    UINT8	reg_bb_ain_df2se:1;					// BIT[19]
    UINT8	reg_bb_mono_stereo:1;				// BIT[20]
    UINT8	reg_bb_mono_in_sel:1;				// BIT[21]
    UINT8	reg_bb_sw_in:1;						// BIT[22]
    UINT8	reg_bb_depop:1;						// BIT[23]

    UINT8	reg_bb_aio2_mux:3;					// BIT[08:10]
    UINT8	res3:1;								// BIT[11]
    UINT8	reg_bb_aio1_mux:3;					// BIT[12:14]
    UINT8	res2:1;								// BIT[15]

    UINT8	aio1out_source:2;					// BIT[00:01]
    UINT8	aio2out_source:2;					// BIT[02:03]
    UINT8	hpout_source:2;						// BIT[04:05]
    UINT8	lineout_source:2;					// BIT[06:07]
} _analog_in_out_sel_RBUS;

typedef union
{
    UINT32						regValue;
    _analog_in_out_sel_RBUS		bitField;
} analog_in_out_sel_RBUS;
// --------------------------------
typedef struct
{
    UINT8	res1_1:8;								// BIT[24:31]

    UINT8	mute_avol1_r:1;							// BIT[16]
    UINT8	mute_avol1_l:1;							// BIT[17]
    UINT8	res1_2:6;								// BIT[18:23]

    UINT8	mute_r_ain5_adc:1;						// BIT[08]
    UINT8	mute_l_ain5_adc:1;						// BIT[09]
    UINT8	mute_r_aio1_adc:1;						// BIT[10]
    UINT8	mute_l_aio1_adc:1;						// BIT[11]
    UINT8	mute_r_aio2_adc:1;						// BIT[12]
    UINT8	mute_l_aio2_adc:1;						// BIT[13]
    UINT8	mute_monoin_r_adc:1;					// BIT[14]
    UINT8	mute_monoin_l_adc:1;					// BIT[15]

    UINT8	mute_r_ain1_adc:1;						// BIT[00]
    UINT8	mute_l_ain1_adc:1;						// BIT[01]
    UINT8	mute_r_ain2_adc:1;						// BIT[02]
    UINT8	mute_l_ain2_adc:1;						// BIT[03]
    UINT8	mute_r_ain3_adc:1;						// BIT[04]
    UINT8	mute_l_ain3_adc:1;						// BIT[05]
    UINT8	mute_r_ain4_adc:1;						// BIT[06]
    UINT8	mute_l_ain4_adc:1;						// BIT[07]
} _analog_frontend_mute_RBUS;

typedef union
{
    UINT32						regValue;
    _analog_frontend_mute_RBUS	bitField;
} analog_frontend_mute_RBUS;
// --------------------------------
typedef  struct
{
    UINT16	res1:16;								// BIT[16:31]

    UINT8	mute_r_monoin_wireio1:1;				// BIT[08]
    UINT8	mute_l_monoin_wireio1:1;				// BIT[09]
    UINT8	mute_r_ain1_wireio1:1;					// BIT[10]
    UINT8	mute_l_ain1_wireio1:1;					// BIT[11]
    UINT8	mute_r_ain2_wireio1:1;					// BIT[12]
    UINT8	mute_l_ain2_wireio1:1;					// BIT[13]
    UINT8	mute_r_adc_in_wireio1:1;				// BIT[14]
    UINT8	mute_l_adc_in_wireio1:1;				// BIT[15]

    UINT8	mute_r_monoin_wireio2:1;				// BIT[00]
    UINT8	mute_l_monoin_wireio2:1;				// BIT[01]
    UINT8	mute_r_ain1_wireio2:1;					// BIT[02]
    UINT8	mute_l_ain1_wireio2:1;					// BIT[03]
    UINT8	mute_r_ain2_wireio2:1;					// BIT[04]
    UINT8	mute_l_ain2_wireio2:1;					// BIT[05]
    UINT8	mute_r_adc_in_wireio2:1;				// BIT[06]
    UINT8	mute_l_adc_in_wireio2:1;				// BIT[07]
} _analog_mixer_mute_RBUS;

typedef union
{
    UINT32						regValue;
    _analog_mixer_mute_RBUS		bitField;
} analog_mixer_mute_RBUS;
// --------------------------------
typedef struct
{
    UINT8	res1_1:8;							// BIT[24:31]

    UINT8	reg_bb_m_aio2_r:1;					// BIT[16]
    UINT8	reg_bb_m_aio2_l:1;					// BIT[17]
    UINT8	reg_bb_m_aio1_r:1;					// BIT[18]
    UINT8	reg_bb_m_aio1_l:1;					// BIT[19]
    UINT8	res1_2:4;							// BIT[20:23]

    UINT8	mute_left_wire2_out:1;				// BIT[08]
    UINT8	mute_right_dac1_out:1;				// BIT[09]
    UINT8	mute_left_dac1_out:1;				// BIT[10]
    UINT8	mute_right_amp:1;					// BIT[11]
    UINT8	mute_left_amp:1;					// BIT[12]
    UINT8	mute_right_lineout:1;				// BIT[13]
    UINT8	mute_left_lineout:1;				// BIT[14]
    UINT8	reg_bb_en_amp:1;					// BIT[15]

    UINT8	mute_right_dac0_out:1;				// BIT[00]
    UINT8	mute_left_dac0_out:1;				// BIT[01]
    UINT8	mute_right_in_amix1:1;				// BIT[02]
    UINT8	mute_left_in_amix1:1;				// BIT[03]
    UINT8	mute_in_amix2:1;					// BIT[04]
    UINT8	mute_right_wire1_out:1;				// BIT[05]
    UINT8	mute_left_wire1_out:1;				// BIT[06]
    UINT8	mute_right_wire2_out:1;				// BIT[07]
} _analog_aio_master_hpout_lineout_RBUS;

typedef union
{
    UINT32									regValue;
    _analog_aio_master_hpout_lineout_RBUS	bitField;
} analog_aio_master_hpout_lineout_RBUS;
// --------------------------------
typedef struct
{
    UINT8	avol_1_in_left_gain:6;				// BIT[24:29]
    UINT8	res1:1;								// BIT[30]
    UINT8	smute_left_avol_1_en:1;				// BIT[31]

    UINT8	avol_1_in_right_gain:6;				// BIT[16:21]
    UINT8	res2:1;								// BIT[22]
    UINT8	smute_right_avol_1_en:1;			// BIT[23]

    UINT8	mono_in_boost_sel:2;				// BIT[08:09]
    UINT8	soft_volume_ctrl_avol:4;			// BIT[10:13]
    UINT8	res3:2;								// BIT[14:15]

    UINT8	avol_2_in_gain:6;					// BIT[00:05]
    UINT8	res4:1;								// BIT[06]
    UINT8	smute_avol_2_en:1;					// BIT[07]
} _adc_volume_gain_control_RBUS;

typedef union
{
    UINT32							regValue;
    _adc_volume_gain_control_RBUS	bitField;
} adc_volume_gain_control_RBUS;
// --------------------------------
typedef struct
{
    UINT8	res1_1:8;							// BIT[24:31]

    UINT8	pow_bb_dac1:1;						// BIT[16]
    UINT8	pow_bb_dac0:1;						// BIT[17]
    UINT8	pow_bb_adc:1;						// BIT[18]
    UINT8	pow_master:1;						// BIT[19]
    UINT8	pow_mixer:1;						// BIT[20]
    UINT8	pow_afe:1;							// BIT[21]
    UINT8	dacvref_mode:1;						// BIT[22]
    UINT8	res1_2:1;							// BIT[23]

    UINT8	pow_dachp:1;						// BIT[08]
    UINT8	pow_aout:1;							// BIT[09]
    UINT8	pow_aio2:1;							// BIT[10]
    UINT8	pow_aio1:1;							// BIT[11]
    UINT8	pow_mono_dac:1;						// BIT[12]
    UINT8	pow_dacvref:1;						// BIT[13]
    UINT8	pow_vref:1;							// BIT[14]
    UINT8	pow_mbias:1;						// BIT[15]

    UINT8	pow_mono_in:1;						// BIT[00]
    UINT8	pow_ain5:1;							// BIT[01]
    UINT8	pow_ain4:1;							// BIT[02]
    UINT8	pow_ain3:1;							// BIT[03]
    UINT8	pow_ain2:1;							// BIT[04]
    UINT8	pow_ain1:1;							// BIT[05]
    UINT8	pow_aio_mono:1;						// BIT[06]
    UINT8	pow_mono_out:1;						// BIT[07]
} _analog_power_RBUS;

typedef union
{
    UINT32						regValue;
    _analog_power_RBUS			bitField;
} analog_power_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1_1:16;							// BIT[16:31]

    UINT8	adc_pgabias:4;						// BIT[08:11]
    UINT8	if_adc_pgapow:1;					// BIT[12]
    UINT8	res1_2:3;							// BIT[13:15]

    UINT8	afe_pga_vol:6;						// BIT[00:05]
    UINT8	res2:2;								// BIT[06:07]
} _analog_pga_RBUS;

typedef union
{
    UINT32						regValue;
    _analog_pga_RBUS			bitField;
} analog_pga_RBUS;
// --------------------------------
typedef struct
{
    UINT8	res1_1:8;							// BIT[24:31]

    UINT8	vcm:2;								// BIT[16:17]
    UINT8	gain_adc_anti_aliasing_filter:1;	// BIT[18]
    UINT8	adc_chopper_clock_en:1;				// BIT[19]
    UINT8	res1_2:4;							// BIT[20:23]

    UINT8	ib_adc_ref_gen_vcm_buf:2;			// BIT[08:09]
    UINT8	ib_adc_anti_alias_filter:2;			// BIT[10:11]
    UINT8	ib_adc_diff_op:2;					// BIT[12:13]
    UINT8	res2:2;								// BIT[14:15]

    UINT8	ib_mixer_sum_op:2;					// BIT[00:01]
    UINT8	ib_dac_df2se:2;						// BIT[02:03]
    UINT8	ib_dac_vrb:2;						// BIT[04:05]
    UINT8	ib_dac_dct_op:2;					// BIT[06:07]
} _analog_ci01_RBUS;

typedef union
{
    UINT32						regValue;
    _analog_ci01_RBUS			bitField;
} analog_ci01_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1_1:16;							// BIT[16:31]

    UINT8	ib_vref_buffer:2;					// BIT[08:09]
    UINT8	ib_dacvref_buffer:2;				// BIT[10:11]
    UINT8	ib_adfe_op:2;						// BIT[12:13]
    UINT8	res1_2:2;							// BIT[14:15]

    UINT8	ib_in_3_stage_op:2;					// BIT[00:01]
    UINT8	ib_io_3_stage_op:2;					// BIT[02:03]
    UINT8	ib_cascade_io_op:2;					// BIT[04:05]
    UINT8	ib_hp_amp:2;						// BIT[06:07]
} _analog_ci02_RBUS;

typedef union
{
    UINT32						regValue;
    _analog_ci02_RBUS			bitField;
} analog_ci02_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1:16;							// BIT[16:31]

    UINT8	ib_mono_in_op1:2;					// BIT[08:09]
    UINT8	ib_mono_out_op2:2;					// BIT[10:11]
    UINT8	ib_mono_out_op1:2;					// BIT[12:13]
    UINT8	ib_master_sum_op:2;					// BIT[14:15]

    UINT8	ib_mono_io_op:2;					// BIT[00:01]
    UINT8	ib_adc_ref_buffer_op_vrn:2;			// BIT[02:03]
    UINT8	ib_adc_ref_buffer_op_vrp:2;			// BIT[04:05]
    UINT8	ib_mono_in_op2:2;					// BIT[06:07]
} _analog_ci03_RBUS;

typedef union
{
    UINT32						regValue;
    _analog_ci03_RBUS			bitField;
} analog_ci03_RBUS;
// --------------------------------
typedef struct
{
    UINT8	res2_1:4;							// BIT[24:27]
    UINT8	time_out_sel:2;						// BIT[28:29]
    UINT8	res1:1;								// BIT[30]
    UINT8	hard_mute_sel:1;					// BIT[31]

    UINT16	reg_bb_zcd_curr:2;					// BIT[08:09]
    UINT16	res3:2;								// BIT[10:11]
    UINT16	reg_bb_pow_zcdcomp:8;				// BIT[12:19]
    UINT16	reg_bb_pow_zcd:1;					// BIT[20]
    UINT16	res2_2:3;							// BIT[21:23]

    UINT8	zc_mute_right_amp:1;				// BIT[00]
    UINT8	zc_mute_left_amp:1;					// BIT[01]
    UINT8	zc_mute_right_lineout:1;			// BIT[02]
    UINT8	zc_mute_left_lineout:1;				// BIT[03]
    UINT8	zc_reg_bb_m_aio2_r:1;				// BIT[04]
    UINT8	zc_reg_bb_m_aio2_l:1;				// BIT[05]
    UINT8	zc_reg_bb_m_aio1_r:1;				// BIT[06]
    UINT8	zc_reg_bb_m_aio1_l:1;				// BIT[07]
} _analog_hard_mute_RBUS;

typedef union
{
    UINT32						regValue;
    _analog_hard_mute_RBUS		bitField;
} analog_hard_mute_RBUS;
// --------------------------------
typedef struct
{
    UINT16	reserved_dummy:16;					// BIT[16:31]

    UINT8	aio2_zc_sel:2;						// BIT[08:09]
    UINT8	aio1_zc_sel:2;						// BIT[10:11]
    UINT8	lineout_zc_sel:2;					// BIT[12:13]
    UINT8	hp_zc_sel:2;						// BIT[14:15]

    UINT8	dac_water_nomask:1;					// BIT[00]
    UINT8	res1:7;								// BIT[01:07]
} _analog_dummy_RBUS;

typedef union
{
    UINT32						regValue;
    _analog_dummy_RBUS			bitField;
} analog_dummy_RBUS;
// --------------------------------
typedef struct
{
    UINT16	res1_1:16;							// BIT[16:31]

    UINT8	res1_2:8;							// BIT[08:15]

    UINT8	sel_bb_ck_depop:3;					// BIT[00:02]
    UINT8	bb_ck_depop_en:1;					// BIT[03]
    UINT8	res1_3:4;							// BIT[04:07]
} _analog_depop1_RBUS;

typedef union
{
    UINT32						regValue;
    _analog_depop1_RBUS			bitField;
} analog_depop1_RBUS;
// --------------------------------
typedef struct
{
    UINT16	reg_bb_depop_cur_sel:3;				// BIT[16:18]
    UINT16	res2:1;								// BIT[19]
    UINT16	reg_bb_cio4:6;						// BIT[20:25]
    UINT16	res1:3;								// BIT[26:28]
    UINT16	reg_bb_pow_depop_op:1;				// BIT[29]
    UINT16	reg_bb_pow_depop_core:1;			// BIT[30]
    UINT16	reg_bb_pow_depop:1;					// BIT[31]

    UINT8	reg_bb_depop_sc_sel:1;				// BIT[08]
    UINT8	reg_bb_aio_norm:1;					// BIT[09]
    UINT8	reg_bb_aout_norm:1;					// BIT[10]
    UINT8	res4:1;								// BIT[11]
    UINT8	reg_bb_depop_cap_sel:2;				// BIT[12:13]
    UINT8	reg_bb_en_io_out:1;					// BIT[14]
    UINT8	res3:1;								// BIT[15]

    UINT8	reg_bb_ib_vcm_det_comp:2;			// BIT[00:01]
    UINT8	reg_bb_hpout_norm:1;				// BIT[02]
    UINT8	reg_bb_vcm_ready_soft:1;			// BIT[03]
    UINT8	reg_bb_en_depop_soft_ctrl:1;		// BIT[04]
    UINT8	reg_bb_depop_sel:2;					// BIT[05:06]
    UINT8	reg_bb_en_depop_ckgen:1;			// BIT[07]
} _analog_depop2_RBUS;

typedef union
{
    UINT32						regValue;
    _analog_depop2_RBUS			bitField;
} analog_depop2_RBUS;
// --------------------------------
typedef union
{
    UINT32						regValue;
    WORD_BBBB					bitField;
    /*
    struct{
        RBus_UInt32  reg_bb_reserved:32;					// BIT[00:31]
    };*/
} analog_depop_dummy_RBUS;
// --------------------------------

#else //apply LITTLE_ENDIAN


//======AUDIO register structure define==========
typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  version:16;							// BIT[00:15]
        RBus_UInt32  proj:16;								// BIT[16:31]
    };
} proj_version_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  sel_down_conv:2;						// BIT[00:01]
        RBus_UInt32  back_end_play_sample_rate:2;			// BIT[02:03]
        RBus_UInt32  sel_play_ck_source:2;					// BIT[04:05]
        RBus_UInt32  adc_512fs_sel:2;						// BIT[06:07]
        RBus_UInt32  daad2_down_sel:2;						// BIT[08:09]
        RBus_UInt32  res2:2;								// BIT[10:11]
        RBus_UInt32  sel_dac_play_ck_source:2;				// BIT[12:13]
        RBus_UInt32  adc_512fs_sel_2:2;						// BIT[14:15]
        RBus_UInt32  spdif_nicam:1;							// BIT[16]
        RBus_UInt32  sel_i2s_hdmi:2;						// BIT[17:18]
        RBus_UInt32  sel_i2s_spdif:1;						// BIT[19]
        RBus_UInt32  sif_adc_clk_en:1;						// BIT[20]
        RBus_UInt32  sif_adc_clk_sel:2;						// BIT[21:22]
        RBus_UInt32  hdv_mode_sel:1;						// BIT[23]
        RBus_UInt32  res1:3;								// BIT[24:26]
        RBus_UInt32  audio_rom_on:1;						// BIT[27]
        RBus_UInt32  a2_eiaj_sram_on:1;						// BIT[28]
        RBus_UInt32  am_sram_on:1;							// BIT[29]
        RBus_UInt32  dac_sram_on:1;							// BIT[30]
        RBus_UInt32  adc_sram_on:1;							// BIT[31]
    };
} global_ctrl_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  reg_para_a:6;							// BIT[00:05]
        RBus_UInt32  res4:2;								// BIT[06:07]
        RBus_UInt32  reg_para_d:4;							// BIT[08:11]
        RBus_UInt32  mod_acc_rst:1;							// BIT[12]
        RBus_UInt32  pll_spdif_auto_mode:1;					// BIT[13]
        RBus_UInt32  reg_smooth_en:1;						// BIT[14]
        RBus_UInt32  pll_spdif_dtrack_en:1;					// BIT[15]
        RBus_UInt32  ps_contword:5;							// BIT[16:20]
        RBus_UInt32  ps_testmode:1;							// BIT[21]
        RBus_UInt32  res3:2;								// BIT[22:23]
        RBus_UInt32  reg_para_e:3;							// BIT[24:26]
        RBus_UInt32  res2:1;								// BIT[27]
        RBus_UInt32  div_2fs:2;								// BIT[28:29]
        RBus_UInt32  res1:1;								// BIT[30]
        RBus_UInt32  contword_inv:1;						// BIT[31]
    };
} pllcg_spdif_gain_cr0_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  p_gain_set2:10;						// BIT[00:09]
        RBus_UInt32  res2:6;								// BIT[10:15]
        RBus_UInt32  p_gain_set:10;							// BIT[16:25]
        RBus_UInt32  res1:6;								// BIT[26:31]
    };
} pllcg_spdif_gain_cr1_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  i_gain_set:10;							// BIT[00:09]
        RBus_UInt32  res1:22;								// BIT[10:31]
    };
} pllcg_spdif_gain_cr2_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  max_correct_range:4;					// BIT[00:03]
        RBus_UInt32  reach_time_num:3;						// BIT[04:06]
        RBus_UInt32  res1:25;								// BIT[07:31]
    };
} pllcg_spdif_number_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  low_bound2:8;							// BIT[00:07]
        RBus_UInt32  high_bound2:8;							// BIT[08:15]
        RBus_UInt32  low_bound1:8;							// BIT[16:23]
        RBus_UInt32  high_bound1:8;							// BIT[24:31]
    };
} pllcg_spdif_bound_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  dummy_601c:32;							// BIT[00:31]
    };
} global_dummy_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  int_sif_ovf_en:1;						// BIT[00]
        RBus_UInt32  int_mode_det_status_change_en:1;		// BIT[01]
        RBus_UInt32  int_msd_timeout_en:1;					// BIT[02]
        RBus_UInt32  int_msd_change_en:1;					// BIT[03]
        RBus_UInt32  int_sif_nopt_en:1;						// BIT[04]
        RBus_UInt32  int_i2s_sec_word_length_change_en:1;	// BIT[05]
        RBus_UInt32  int_i2s_pri_word_length_change_en:1;	// BIT[06]
        RBus_UInt32  int_spi_sample_rate_chg_en:1;			// BIT[07]
        RBus_UInt32  int_i2s_sec_sample_rate_chg_en:1;		// BIT[08]
        RBus_UInt32  int_i2s_pri_sample_rate_chg_en:1;		// BIT[09]
        RBus_UInt32  int_nicam_hierr_change_en:1;			// BIT[10]
        RBus_UInt32  int_spi_in_lock_change_en:1;			// BIT[11]
        RBus_UInt32  int_spi_in_datatype_change_en:1;		// BIT[12]
        RBus_UInt32  int_aout_timer_en:1;					// BIT[13]
        RBus_UInt32  int_ain_timer_en:1;					// BIT[14]
        RBus_UInt32  int_pcm_fs_chg_en:1;					// BIT[15]
        RBus_UInt32  int_fifo0_empty_en:1;					// BIT[16]
        RBus_UInt32  int_fifo1_empty_en:1;					// BIT[17]
        RBus_UInt32  int_fifo2_empty_en:1;					// BIT[18]
        RBus_UInt32  int_fifo3_empty_en:1;					// BIT[19]
        RBus_UInt32  int_fifo0_full_en:1;					// BIT[20]
        RBus_UInt32  int_fifo1_full_en:1;					// BIT[21]
        RBus_UInt32  int_fifo2_full_en:1;					// BIT[22]
        RBus_UInt32  int_fifo3_full_en:1;					// BIT[23]
        RBus_UInt32  int_btsc_mode_chg_en:1;				// BIT[24]
        RBus_UInt32  int_rbus_timeout_en:1;					// BIT[25]
        RBus_UInt32  int_msd_fw_scan_done_en:1;				// BIT[26]
        RBus_UInt32  int_msd_decision_done_en:1;			// BIT[27]
        RBus_UInt32  res1:4;								// BIT[28:31]
    };
} interrupt_en_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  int_sif_ovf:1;							// BIT[00]
        RBus_UInt32  int_mode_det_status_change:1;			// BIT[01]
        RBus_UInt32  int_msd_timeout:1;						// BIT[02]
        RBus_UInt32  int_msd_change:1;						// BIT[03]
        RBus_UInt32  int_sif_nopt:1;						// BIT[04]
        RBus_UInt32  int_i2s_sec_word_length_change:1;		// BIT[05]
        RBus_UInt32  int_i2s_pri_word_length_change:1;		// BIT[06]
        RBus_UInt32  int_spi_sample_rate_chg:1;				// BIT[07]
        RBus_UInt32  int_i2s_sec_sample_rate_chg:1;			// BIT[08]
        RBus_UInt32  int_i2s_pri_sample_rate_chg:1;			// BIT[09]
        RBus_UInt32  int_nicam_hierr_change:1;				// BIT[10]
        RBus_UInt32  int_spi_in_lock_change:1;				// BIT[11]
        RBus_UInt32  int_spi_in_datatype_change:1;			// BIT[12]
        RBus_UInt32  int_aout_timer:1;						// BIT[13]
        RBus_UInt32  int_ain_timer:1;						// BIT[14]
        RBus_UInt32  int_pcm_fs_chg:1;						// BIT[15]
        RBus_UInt32  int_fifo0_empty:1;						// BIT[16]
        RBus_UInt32  int_fifo1_empty:1;						// BIT[17]
        RBus_UInt32  int_fifo2_empty:1;						// BIT[18]
        RBus_UInt32  int_fifo3_empty:1;						// BIT[19]
        RBus_UInt32  int_fifo0_full:1;						// BIT[20]
        RBus_UInt32  int_fifo1_full:1;						// BIT[21]
        RBus_UInt32  int_fifo2_full:1;						// BIT[22]
        RBus_UInt32  int_fifo3_full:1;						// BIT[23]
        RBus_UInt32  int_btsc_mode_chg:1;					// BIT[24]
        RBus_UInt32  int_rbus_timeout:1;					// BIT[25]
        RBus_UInt32  int_msd_fw_scan_done:1;				// BIT[26]
        RBus_UInt32  int_msd_decision_done:1;				// BIT[27]
        RBus_UInt32  res1:4;								// BIT[28:31]
    };
} interrupt_status_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  aout_timer_en:1;						// BIT[00]
        RBus_UInt32  ain_timer_en:1;						// BIT[01]
        RBus_UInt32  res1:30;								// BIT[02:31]
    };
} aio_timer_en_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  ain_timer:32;							// BIT[00:31]
    };
} ain_counter_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  ain_compare:32;						// BIT[00:31]
    };
} ain_compare_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  aout_timer:32;							// BIT[00:31]
    };
} aout_counter_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  aout_compare:32;						// BIT[00:31]
    };
} aout_compare_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  pcm_width:7;							// BIT[00:06]
        RBus_UInt32  res3:1;								// BIT[07]
        RBus_UInt32  sampling_rate:4;						// BIT[08:11]
        RBus_UInt32  res2:4;								// BIT[12:15]
        RBus_UInt32  reserved_dummy:8;						// BIT[16:23]
        RBus_UInt32  res1:2;								// BIT[24:25]
        RBus_UInt32  debounce_en:1;							// BIT[26]
        RBus_UInt32  mode_sel:1;							// BIT[27]
        RBus_UInt32  left_right:2;							// BIT[28:29]
        RBus_UInt32  source_sel:2;							// BIT[30:31]
    };
} i2s_in_primary_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  pcm_width:7;							// BIT[00:06]
        RBus_UInt32  res2:1;								// BIT[07]
        RBus_UInt32  sampling_rate:4;						// BIT[08:11]
        RBus_UInt32  res1:14;								// BIT[12:25]
        RBus_UInt32  debounce_en:1;							// BIT[26]
        RBus_UInt32  mode_sel:1;							// BIT[27]
        RBus_UInt32  left_right:2;							// BIT[28:29]
        RBus_UInt32  source_sel:2;							// BIT[30:31]
    };
} i2s_in_secondary_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  sampling_rate:4;						// BIT[00:03]
        RBus_UInt32  res2:1;								// BIT[04]
        RBus_UInt32  reserved_dummy:11;						// BIT[05:15]
        RBus_UInt32  res1:12;								// BIT[16:27]
        RBus_UInt32  debounce_en:1;							// BIT[28]
        RBus_UInt32  req_after_lock:1;						// BIT[29]
        RBus_UInt32  source_sel:2;							// BIT[30:31]
    };
} spdif_in_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  professional:1;						// BIT[00]
        RBus_UInt32  data_type:1;							// BIT[01]
        RBus_UInt32  copyright:1;							// BIT[02]
        RBus_UInt32  pre_emphasis:3;						// BIT[03:05]
        RBus_UInt32  mode:2;								// BIT[06:07]
        RBus_UInt32  category_code:8;						// BIT[08:15]
        RBus_UInt32  source_number:4;						// BIT[16:19]
        RBus_UInt32  channel_number:4;						// BIT[20:23]
        RBus_UInt32  sampling_freq:4;						// BIT[24:27]
        RBus_UInt32  clock_accuracy:2;						// BIT[28:29]
        RBus_UInt32  invalid_bit:1;							// BIT[30]
        RBus_UInt32  hw_locked:1;							// BIT[31]
    };
} spdif_in_cs_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  reserved_dummy:16;						// BIT[00:15]
        RBus_UInt32  res3:8;								// BIT[16:23]
        RBus_UInt32  mclk_sel:2;							// BIT[24:25]
        RBus_UInt32  res2:2;								// BIT[26:27]
        RBus_UInt32  output_sel:3;							// BIT[28:30]
        RBus_UInt32  res1:1;								// BIT[31]
    };
} i2s_out_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  reserved_dummy:16;						// BIT[00:15]
        RBus_UInt32  res2:12;								// BIT[16:27]
        RBus_UInt32  validity_ctrl:1;						// BIT[28]
        RBus_UInt32  res1:1;								// BIT[29]
        RBus_UInt32  output_sel:2;							// BIT[30:31]
    };
} spdif_out_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  professional:1;						// BIT[00]
        RBus_UInt32  data_type:1;							// BIT[01]
        RBus_UInt32  copyright:1;							// BIT[02]
        RBus_UInt32  pre_emphasis:3;						// BIT[03:05]
        RBus_UInt32  mode:2;								// BIT[06:07]
        RBus_UInt32  category_code:8;						// BIT[08:15]
        RBus_UInt32  source_number:4;						// BIT[16:19]
        RBus_UInt32  channel_number:4;						// BIT[20:23]
        RBus_UInt32  sampling_freq:4;						// BIT[24:27]
        RBus_UInt32  clock_accuracy:2;						// BIT[28:29]
        RBus_UInt32  res1:2;								// BIT[30:31]
    };
} spdif_out_cs_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  ob_time:16;							// BIT[00:15]
        RBus_UInt32  hw_agc_en:1;							// BIT[16]
        RBus_UInt32  dummy_19_17:3;							// BIT[17:19]
        RBus_UInt32  res1:12;								// BIT[20:31]
    };
} sif_agc_eval_cnt_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  non_opt_cnt_ovf:8;						// BIT[00:07]
        RBus_UInt32  non_opt_cnt_under:8;					// BIT[08:15]
        RBus_UInt32  non_opt_cnt_th:3;						// BIT[16:18]
        RBus_UInt32  res1:13;								// BIT[19:31]
    };
} sif_agc_optimal_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  opt_th:10;								// BIT[00:09]
        RBus_UInt32  res2:6;								// BIT[10:15]
        RBus_UInt32  ovf_th:10;								// BIT[16:25]
        RBus_UInt32  res1:6;								// BIT[26:31]
    };
} sif_agc_threshold_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  abs_peak:10;							// BIT[00:09]
        RBus_UInt32  res2:2;								// BIT[10:11]
        RBus_UInt32  opt:1;									// BIT[12]
        RBus_UInt32  ovf:1;									// BIT[13]
        RBus_UInt32  res1:18;								// BIT[14:31]
    };
} sif_agc_status_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  pdetect_enable:1;						// BIT[00]
        RBus_UInt32  detect_source_sel:3;					// BIT[01:03]
        RBus_UInt32  res1:28;								// BIT[04:31]
    };
} peak_contorl_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  ovf_th_l:16;							// BIT[00:15]
        RBus_UInt32  opt_th_l:16;							// BIT[16:31]
    };
} peak_contorl_l_0_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  vpp_eval_cnt_l:14;						// BIT[00:13]
        RBus_UInt32  res1:2;								// BIT[14:15]
        RBus_UInt32  nonopt_cnt_tr_l:3;						// BIT[16:18]
        RBus_UInt32  dummy_61ac:13;							// BIT[19:31]
    };
} peak_control_l_1_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  magnitude_l:16;						// BIT[00:15]
        RBus_UInt32  pk_adj_l:6;							// BIT[16:21]
        RBus_UInt32  overflow_l:1;							// BIT[22]
        RBus_UInt32  optimize_l:1;							// BIT[23]
        RBus_UInt32  res1:8;								// BIT[24:31]
    };
} peak_status_l_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  ovf_th_r:16;							// BIT[00:15]
        RBus_UInt32  opt_th_r:16;							// BIT[16:31]
    };
} peak_contorl_r_0_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  vpp_eval_cnt_r:14;						// BIT[00:13]
        RBus_UInt32  res1:2;								// BIT[14:15]
        RBus_UInt32  nonopt_cnt_tr_r:3;						// BIT[16:18]
        RBus_UInt32  dummy_61b8:13;							// BIT[19:31]
    };
} peak_contorl_r_1_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  magnitude_r:16;						// BIT[00:15]
        RBus_UInt32  pk_adj_r:6;							// BIT[16:21]
        RBus_UInt32  overflow_r:1;							// BIT[22]
        RBus_UInt32  optimize_r:1;							// BIT[23]
        RBus_UInt32  res1:8;								// BIT[24:31]
    };
} peak_status_r_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  play_sel_b:2;							// BIT[00:01]
        RBus_UInt32  play_sel_a:2;							// BIT[02:03]
        RBus_UInt32  sound_select:2;						// BIT[04:05]
        RBus_UInt32  res4:2;								// BIT[06:07]
        RBus_UInt32  dkl_priority:1;						// BIT[08]
        RBus_UInt32  mn_priority:2;							// BIT[09:10]
        RBus_UInt32  res3:1;								// BIT[11]
        RBus_UInt32  reg_scan_cnt:8;						// BIT[12:19]
        RBus_UInt32  msd_debounce_cnt:3;					// BIT[20:22]
        RBus_UInt32  res2:1;								// BIT[23]
        RBus_UInt32  carrier_phase_delta:3;					// BIT[24:26]
        RBus_UInt32  carrier_shift_scan_en:1;				// BIT[27]
        RBus_UInt32  update_en:1;							// BIT[28]
        RBus_UInt32  msd_en:1;								// BIT[29]
        RBus_UInt32  fw_scan_en:1;							// BIT[30]
        RBus_UInt32  res1:1;								// BIT[31]
    };
} msd_ctrl_word_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  reg_std:5;								// BIT[00:04]
        RBus_UInt32  res2:3;								// BIT[05:07]
        RBus_UInt32  hw_detected_std:5;						// BIT[08:12]
        RBus_UInt32  res1:19;								// BIT[13:31]
    };
} msd_result_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  thresh_4_5_l:16;						// BIT[00:15]
        RBus_UInt32  thresh_4_5:16;							// BIT[16:31]
    };
} msd_thresh_4_5_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  reg_mag_4_5:16;						// BIT[00:15]
        RBus_UInt32  res1:16;								// BIT[16:31]
    };
} msd_mag_4_5_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  reg_mag_4_72:16;						// BIT[00:15]
        RBus_UInt32  thresh_4_72:16;						// BIT[16:31]
    };
} msd_carrier_4_72_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  thresh_5_5_l:16;						// BIT[00:15]
        RBus_UInt32  thresh_5_5:16;							// BIT[16:31]
    };
} msd_thresh_5_5_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  reg_mag_5_5:16;						// BIT[00:15]
        RBus_UInt32  res1:16;								// BIT[16:31]
    };
} msd_mag_5_5_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  reg_mag_5_74:16;						// BIT[00:15]
        RBus_UInt32  thresh_5_74:16;						// BIT[16:31]
    };
} msd_carrier_5_74_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  reg_mag_5_85:16;						// BIT[00:15]
        RBus_UInt32  thresh_5_85:16;						// BIT[16:31]
    };
} msd_carrier_5_85_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  thresh_6_l:16;							// BIT[00:15]
        RBus_UInt32  thresh_6:16;							// BIT[16:31]
    };
} msd_thresh_6_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  reg_mag_6:16;							// BIT[00:15]
        RBus_UInt32  res1:16;								// BIT[16:31]
    };
} btsc_mag_6_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  reg_mag_6_25:16;						// BIT[00:15]
        RBus_UInt32  thresh_6_25:16;						// BIT[16:31]
    };
} msd_carrier_625_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  reg_mag_6_35:16;						// BIT[00:15]
        RBus_UInt32  thresh_6_35:16;						// BIT[16:31]
    };
} msd_carrier_6_35_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  reg_mag_6_45:16;						// BIT[00:15]
        RBus_UInt32  thresh_6_45:16;						// BIT[16:31]
    };
} msd_carrier_6_45_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  thresh_6_5_l:16;						// BIT[00:15]
        RBus_UInt32  thresh_6_5:16;							// BIT[16:31]
    };
} msd_thresh_6_5_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  reg_mag_6_5:16;						// BIT[00:15]
        RBus_UInt32  res1:16;								// BIT[16:31]
    };
} msd_mag_6_5_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  reg_mag_6_552:16;						// BIT[00:15]
        RBus_UInt32  thresh_6_552:16;						// BIT[16:31]
    };
} btsc_carrier_6_52_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  reg_mag_6_62:16;						// BIT[00:15]
        RBus_UInt32  thresh_6_62:16;						// BIT[16:31]
    };
} btsc_mag_6_62_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  reg_mag_6_74:16;						// BIT[00:15]
        RBus_UInt32  thresh_6_74:16;						// BIT[16:31]
    };
} btsc_carrier_6_74_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  reg_mag_6_8:16;						// BIT[00:15]
        RBus_UInt32  thresh_6_8:16;							// BIT[16:31]
    };
} btsc_carrier_6_8_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  fw_phase_delta:14;						// BIT[00:13]
        RBus_UInt32  res1:2;								// BIT[14:15]
        RBus_UInt32  thresh_fw_l:16;						// BIT[16:31]
    };
} msd_programmable_freq_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  reg_mag_fw:16;							// BIT[00:15]
        RBus_UInt32  thresh_fw:16;							// BIT[16:31]
    };
} msd_programmable_freq_threshold_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  diff_574_585:16;						// BIT[00:15]
        RBus_UInt32  thresh_574_585:16;						// BIT[16:31]
    };
} msd_574_585_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  diff_585_55:16;						// BIT[00:15]
        RBus_UInt32  thresh_585_55:16;						// BIT[16:31]
    };
} msd_585_55_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  diff_585_635:16;						// BIT[00:15]
        RBus_UInt32  thresh_585_635:16;						// BIT[16:31]
    };
} msd_585_635_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  diff_625_55:16;						// BIT[00:15]
        RBus_UInt32  thresh_625_55:16;						// BIT[16:31]
    };
} msd_625_55_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  diff_6552_68:16;						// BIT[00:15]
        RBus_UInt32  thresh_6552_68:16;						// BIT[16:31]
    };
} msd_6552_68_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  flag_fw:1;								// BIT[00]
        RBus_UInt32  flag_6_8:1;							// BIT[01]
        RBus_UInt32  flag_6_74:1;							// BIT[02]
        RBus_UInt32  flag_6_62:1;							// BIT[03]
        RBus_UInt32  flag_6_552:1;							// BIT[04]
        RBus_UInt32  flag_6_5:1;							// BIT[05]
        RBus_UInt32  flag_6_45:1;							// BIT[06]
        RBus_UInt32  flag_6_35:1;							// BIT[07]
        RBus_UInt32  flag_6_25:1;							// BIT[08]
        RBus_UInt32  flag_6:1;								// BIT[09]
        RBus_UInt32  flag_5_85:1;							// BIT[10]
        RBus_UInt32  flag_5_74:1;							// BIT[11]
        RBus_UInt32  flag_5_5:1;							// BIT[12]
        RBus_UInt32  flag_4_72:1;							// BIT[13]
        RBus_UInt32  flag_4_5:1;							// BIT[14]
        RBus_UInt32  res2:1;								// BIT[15]
        RBus_UInt32  flag_fw_freq_0:1;						// BIT[16]
        RBus_UInt32  flag_fw_freq_1:1;						// BIT[17]
        RBus_UInt32  flag_fw_freq_2:1;						// BIT[18]
        RBus_UInt32  flag_fw_freq_3:1;						// BIT[19]
        RBus_UInt32  res1:12;								// BIT[20:31]
    };
} msd_flag_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  large_6552_thresh:1;					// BIT[00]
        RBus_UInt32  large_625_thresh:1;					// BIT[01]
        RBus_UInt32  large_585_635_thresh:1;				// BIT[02]
        RBus_UInt32  large_585_thresh:1;					// BIT[03]
        RBus_UInt32  large_diff_thresh:1;					// BIT[04]
        RBus_UInt32  large_625_55:1;						// BIT[05]
        RBus_UInt32  large_585_635:1;						// BIT[06]
        RBus_UInt32  large_585_55:1;						// BIT[07]
        RBus_UInt32  large_574_585:1;						// BIT[08]
        RBus_UInt32  only_6_5:1;							// BIT[09]
        RBus_UInt32  only_6:1;								// BIT[10]
        RBus_UInt32  only_5_5:1;							// BIT[11]
        RBus_UInt32  only_4_5:1;							// BIT[12]
        RBus_UInt32  res1:19;								// BIT[13:31]
    };
} msd_large_status_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  std_list:16;							// BIT[00:15]
        RBus_UInt32  res1:16;								// BIT[16:31]
    };
} msd_std_list_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  thresh_5_5_second:16;					// BIT[00:15]
        RBus_UInt32  thresh_4_5_second:16;					// BIT[16:31]
    };
} msd_phase2_4_5_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  thresh_6_5_second:16;					// BIT[00:15]
        RBus_UInt32  thresh_6_second:16;					// BIT[16:31]
    };
} msd_phase2_5_5_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  second_reg_std:5;						// BIT[00:04]
        RBus_UInt32  res2:3;								// BIT[05:07]
        RBus_UInt32  mono_std_sel:1;						// BIT[08]
        RBus_UInt32  res1:23;								// BIT[09:31]
    };
} msd_second_reg_std_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  bist_fail_msd:1;						// BIT[00]
        RBus_UInt32  bist_done_msd:1;						// BIT[01]
        RBus_UInt32  bist_mode_msd:1;						// BIT[02]
        RBus_UInt32  bist_rstn_msd:1;						// BIT[03]
        RBus_UInt32  res1:28;								// BIT[04:31]
    };
} msd_bist_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  mag_fw_freq_0:16;						// BIT[00:15]
        RBus_UInt32  fw_freq_0_set:14;						// BIT[16:29]
        RBus_UInt32  res1:2;								// BIT[30:31]
    };
} msd_fw_freq_0_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  fw_freq_0_low_thre:16;					// BIT[00:15]
        RBus_UInt32  fw_freq_0_high_thre:16;				// BIT[16:31]
    };
} msd_fw_req_0_thre_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  mag_fw_freq_1:16;						// BIT[00:15]
        RBus_UInt32  fw_freq_1_set:14;						// BIT[16:29]
        RBus_UInt32  res1:2;								// BIT[30:31]
    };
} msd_fw_freq_1_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  fw_freq_1_low_thre:16;					// BIT[00:15]
        RBus_UInt32  fw_freq_1_high_thre:16;				// BIT[16:31]
    };
} msd_fw_req_1_thre_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  mag_fw_freq_2:16;						// BIT[00:15]
        RBus_UInt32  fw_freq_2_set:14;						// BIT[16:29]
        RBus_UInt32  res1:2;								// BIT[30:31]
    };
} msd_fw_freq_2_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  fw_freq_2_low_thre:16;					// BIT[00:15]
        RBus_UInt32  fw_freq_2_high_thre:16;				// BIT[16:31]
    };
} msd_fw_req_2_thre_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  mag_fw_freq_3:16;						// BIT[00:15]
        RBus_UInt32  fw_freq_3_set:14;						// BIT[16:29]
        RBus_UInt32  res1:2;								// BIT[30:31]
    };
} msd_fw_freq_3_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  fw_freq_3_low_thre:16;					// BIT[00:15]
        RBus_UInt32  fw_freq_3_high_thre:16;				// BIT[16:31]
    };
} msd_fw_req_3_thre_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  mode_result:2;							// BIT[00:01]
        RBus_UInt32  hw_mode_en:1;							// BIT[02]
        RBus_UInt32  res1:29;								// BIT[03:31]
    };
} mode_detect_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  max_threshold:16;						// BIT[00:15]
        RBus_UInt32  res3:4;								// BIT[16:19]
        RBus_UInt32  debounce_cnt:3;						// BIT[20:22]
        RBus_UInt32  res2:1;								// BIT[23]
        RBus_UInt32  low_threshold:2;						// BIT[24:25]
        RBus_UInt32  high_threshold:2;						// BIT[26:27]
        RBus_UInt32  accu_num:2;							// BIT[28:29]
        RBus_UInt32  res1:1;								// BIT[30]
        RBus_UInt32  enable:1;								// BIT[31]
    };
} md_ctrl_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  mode_result:2;							// BIT[00:01]
        RBus_UInt32  res1:2;								// BIT[02:03]
        RBus_UInt32  gain_result:4;							// BIT[04:07]
        RBus_UInt32  reserved_dummy:8;						// BIT[08:15]
        RBus_UInt32  max_other:16;							// BIT[16:31]
    };
} md_status_1_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  max_dual:16;							// BIT[00:15]
        RBus_UInt32  max_stereo:16;							// BIT[16:31]
    };
} md_status_2_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  fail_51x16:1;							// BIT[00]
        RBus_UInt32  fail_52x16:1;							// BIT[01]
        RBus_UInt32  res1:27;								// BIT[02:28]
        RBus_UInt32  done:1;								// BIT[29]
        RBus_UInt32  enable:1;								// BIT[30]
        RBus_UInt32  rst_n:1;								// BIT[31]
    };
} mbist_md_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  carrier_freq_correct_en:1;				// BIT[00]
        RBus_UInt32  res1:30;								// BIT[01]
        RBus_UInt32  sif_data_source:1;						// BIT[31]
    };
} carrier_freq_deviatio_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  sub_phase_delta:14;					// BIT[00:13]
        RBus_UInt32  res2:2;								// BIT[14:15]
        RBus_UInt32  main_phase_delta:14;					// BIT[16:29]
        RBus_UInt32  res1:2;								// BIT[30:31]
    };
} carrier_phase_delta_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  hw_am_prescale:2;						// BIT[00:01]
        RBus_UInt32  am_path_sel:1;							// BIT[02]
        RBus_UInt32  res2:1;								// BIT[03]
        RBus_UInt32  dummy_6358:4;							// BIT[04:07]
        RBus_UInt32  bist_fail_am:1;						// BIT[08]
        RBus_UInt32  bist_done_am:1;						// BIT[09]
        RBus_UInt32  bist_mode_am:1;						// BIT[10]
        RBus_UInt32  bist_rstn_am:1;						// BIT[11]
        RBus_UInt32  res1:20;								// BIT[12:31]
    };
} am_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  pilot_lothresh:10;						// BIT[00:09]
        RBus_UInt32  res2:6;								// BIT[10:15]
        RBus_UInt32  pilot_hithresh:10;						// BIT[16:25]
        RBus_UInt32  res1:5;								// BIT[26:30]
        RBus_UInt32  lock_reference:1;						// BIT[31]
    };
} btsc_pilot_thres_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  w10pilotmagnitude:10;					// BIT[00:09]
        RBus_UInt32  res2:2;								// BIT[10:11]
        RBus_UInt32  pilot_lo_refuse:1;						// BIT[12]
        RBus_UInt32  pilot_hi_pass:1;						// BIT[13]
        RBus_UInt32  pilot_level_pass:1;					// BIT[14]
        RBus_UInt32  w1pilotlocked:1;						// BIT[15]
        RBus_UInt32  w1pilotpresent:1;						// BIT[16]
        RBus_UInt32  res1:15;								// BIT[17:31]
    };
} btsc_pilot_status_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  sap_lothresh:10;						// BIT[00:09]
        RBus_UInt32  res2:6;								// BIT[10:15]
        RBus_UInt32  sap_hithresh:10;						// BIT[16:25]
        RBus_UInt32  res1:6;								// BIT[26:31]
    };
} btsc_sap_thres_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  w10sapnoise:10;						// BIT[00:09]
        RBus_UInt32  res1:22;								// BIT[10:31]
    };
} btsc_sap_noise_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  sap_noise_lo_thresh:10;				// BIT[00:09]
        RBus_UInt32  res2:6;								// BIT[10:15]
        RBus_UInt32  sap_noise_hi_thresh:10;				// BIT[16:25]
        RBus_UInt32  res1:6;								// BIT[26:31]
    };
} btsc_sap_noise_th_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  sap_debounce:4;						// BIT[00:03]
        RBus_UInt32  sap_debounce_en:1;						// BIT[04]
        RBus_UInt32  res1:27;								// BIT[05:31]
    };
} btsc_sap_debounce_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  w10sapmagnitude:10;					// BIT[00:09]
        RBus_UInt32  res2:1;								// BIT[10]
        RBus_UInt32  sap_noise_flag:1;						// BIT[11]
        RBus_UInt32  sap_lo_refuse:1;						// BIT[12]
        RBus_UInt32  sap_hi_pass:1;							// BIT[13]
        RBus_UInt32  w1sapqualitygood:1;					// BIT[14]
        RBus_UInt32  res1:17;								// BIT[15:31]
    };
} btsc_sap_status_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  w10fmmagnitude:10;						// BIT[00:09]
        RBus_UInt32  res2:2;								// BIT[10:11]
        RBus_UInt32  fm_mag_optimize:1;						// BIT[12]
        RBus_UInt32  fm_mag_overflow:1;						// BIT[13]
        RBus_UInt32  res1:18;								// BIT[14:31]
    };
} btsc_sif_fm_magnitude_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  testoutputselect:4;					// BIT[00:03]
        RBus_UInt32  res1:28;								// BIT[04:31]
    };
} btsc_test_mux_sel_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  dummy_6424:16;							// BIT[00:15]
        RBus_UInt32  res1:16;								// BIT[16:31]
    };
} btsc_dummy_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  eiaj_sub_deviation:1;					// BIT[00]
        RBus_UInt32  res4:3;								// BIT[01:03]
        RBus_UInt32  fm2_deviation_gain:3;					// BIT[04:06]
        RBus_UInt32  res3:1;								// BIT[07]
        RBus_UInt32  fm1_deviation_gain:3;					// BIT[08:10]
        RBus_UInt32  res2:9;								// BIT[11:19]
        RBus_UInt32  ultra_hdv_gain:3;						// BIT[20:22]
        RBus_UInt32  res1:3;								// BIT[23:25]
        RBus_UInt32  ultra_hdv_mode:1;						// BIT[26]
        RBus_UInt32  iir_lpf_en:1;							// BIT[27]
        RBus_UInt32  de_emphasis_time:2;					// BIT[28:29]
        RBus_UInt32  a2_bw_sel:2;							// BIT[30:31]
    };
} a2_eiaj_demod_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  fail_24x13_0:1;						// BIT[00]
        RBus_UInt32  fail_24x13_1:1;						// BIT[01]
        RBus_UInt32  fail_24x14_0:1;						// BIT[02]
        RBus_UInt32  fail_24x14_1:1;						// BIT[03]
        RBus_UInt32  fail_24x66_0:1;						// BIT[04]
        RBus_UInt32  fail_24x66_1:1;						// BIT[05]
        RBus_UInt32  fail_24x74_0:1;						// BIT[06]
        RBus_UInt32  fail_24x98_0:1;						// BIT[07]
        RBus_UInt32  fail_24x98_1:1;						// BIT[08]
        RBus_UInt32  res1:20;								// BIT[09:28]
        RBus_UInt32  done:1;								// BIT[29]
        RBus_UInt32  enable:1;								// BIT[30]
        RBus_UInt32  rst_n:1;								// BIT[31]
    };
} mbist_a2_eiaj_demod_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  reserved_dummy:16;						// BIT[00:15]
        RBus_UInt32  magnitude:16;							// BIT[16:31]
    };
} a2_eiaj_dc_filter_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  tr_timing_step:20;						// BIT[00:19]
        RBus_UInt32  cr_lpf_ki:2;							// BIT[20:21]
        RBus_UInt32  res2:2;								// BIT[22:23]
        RBus_UInt32  cr_lpf_kp:3;							// BIT[24:26]
        RBus_UInt32  res1:1;								// BIT[27]
        RBus_UInt32  tr_lpf_kp:3;							// BIT[28:30]
        RBus_UInt32  psf_sel:1;								// BIT[31]
    };
} nicam_demod_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  gain_adjust:6;							// BIT[00:05]
        RBus_UInt32  res2:10;								// BIT[06:15]
        RBus_UInt32  window_size:10;						// BIT[16:25]
        RBus_UInt32  res1:2;								// BIT[26:27]
        RBus_UInt32  debounce_cnt:3;						// BIT[28:30]
        RBus_UInt32  enable:1;								// BIT[31]
    };
} nicam_dagc_ctrl_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  low:16;								// BIT[00:15]
        RBus_UInt32  high:16;								// BIT[16:31]
    };
} nicam_dagc_threshold_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  magnitude:16;							// BIT[00:15]
        RBus_UInt32  res1:16;								// BIT[16:31]
    };
} nicam_dagc_mag_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  reserved_dummy:16;						// BIT[00:15]
        RBus_UInt32  res2:4;								// BIT[16:19]
        RBus_UInt32  status_debounce_cnt:4;					// BIT[20:23]
        RBus_UInt32  res1:4;								// BIT[24:27]
        RBus_UInt32  pcm_r_out_sel:1;						// BIT[28]
        RBus_UInt32  pcm_l_out_sel:1;						// BIT[29]
        RBus_UInt32  data_channel_sel:1;					// BIT[30]
        RBus_UInt32  resampler_ctrl:1;						// BIT[31]
    };
} nicam_decode_ctrl_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  bit_error_rate:12;						// BIT[00:11]
        RBus_UInt32  true_detect:1;							// BIT[12]
        RBus_UInt32  res1:3;								// BIT[13:15]
        RBus_UInt32  additional_data:11;					// BIT[16:26]
        RBus_UInt32  c4:1;									// BIT[27]
        RBus_UInt32  c1c2c3:3;								// BIT[28:30]
        RBus_UInt32  c0:1;									// BIT[31]
    };
} nicam_decode_status_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  low_threshold:12;						// BIT[00:11]
        RBus_UInt32  high_threshold:12;						// BIT[12:23]
        RBus_UInt32  hi_ber_debounce:3;						// BIT[24:26]
        RBus_UInt32  analog_flag:1;							// BIT[27]
        RBus_UInt32  c4_hi_ber:2;							// BIT[28:29]
        RBus_UInt32  res1:1;								// BIT[30]
        RBus_UInt32  enable:1;								// BIT[31]
    };
} nicam_auto_switch_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  down_rate:1;							// BIT[00]
        RBus_UInt32  downsample_en:1;						// BIT[01]
        RBus_UInt32  backend_play_en:1;						// BIT[02]
        RBus_UInt32  res1:29;								// BIT[03:31]
    };
} down_sample_control_for_playback_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  fifo_ch12_sel:4;						// BIT[00:03]
        RBus_UInt32  fifo_ch34_sel:4;						// BIT[04:07]
        RBus_UInt32  fifo_ch56_sel:4;						// BIT[08:11]
        RBus_UInt32  fifo_ch78_sel:4;						// BIT[12:15]
        RBus_UInt32  fifo_ch12_scale:2;						// BIT[16:17]
        RBus_UInt32  fifo_ch2_source:1;						// BIT[18]
        RBus_UInt32  fifo_ch1_source:1;						// BIT[19]
        RBus_UInt32  res1:12;								// BIT[20:31]
    };
} down_sample_fifo_source_sel_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  playback_bist_fail:1;					// BIT[00]
        RBus_UInt32  playback_bist_done:1;					// BIT[01]
        RBus_UInt32  playback_bist_mode:1;					// BIT[02]
        RBus_UInt32  playback_bist_rstn:1;					// BIT[03]
        RBus_UInt32  dummy_6558:28;							// BIT[04:31]
    };
} down_sample_mbist_recoding_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  dvcm:2;								// BIT[00:01]
        RBus_UInt32  res3:2;								// BIT[02:03]
        RBus_UInt32  zcth:5;								// BIT[04:08]
        RBus_UInt32  res2:1;								// BIT[09]
        RBus_UInt32  auto_volenable:1;						// BIT[10]
        RBus_UInt32  spatial_ctrlenable:1;					// BIT[11]
        RBus_UInt32  deq_enable:1;							// BIT[12]
        RBus_UInt32  loud_enable:1;							// BIT[13]
        RBus_UInt32  dvol_enable:1;							// BIT[14]
        RBus_UInt32  basic_premium_sel:1;					// BIT[15]
        RBus_UInt32  res1:15;								// BIT[16:30]
        RBus_UInt32  hw_8ch_bypass_en:1;					// BIT[31]
    };
} hw_process_control_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  delay_ctrl_mute:10;					// BIT[00:09]
        RBus_UInt32  ramp_step_mute:2;						// BIT[10:11]
        RBus_UInt32  zcto_mute:3;							// BIT[12:14]
        RBus_UInt32  mute_apply:1;							// BIT[15]
        RBus_UInt32  delay_ctrl:10;							// BIT[16:25]
        RBus_UInt32  ramp_step:2;							// BIT[26:27]
        RBus_UInt32  zcto:3;								// BIT[28:30]
        RBus_UInt32  dvol_apply:1;							// BIT[31]
    };
} digital_volume_control_0_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  vol_r_b:6;								// BIT[00:05]
        RBus_UInt32  vol_r_a:5;								// BIT[06:10]
        RBus_UInt32  vol_r_ready:1;							// BIT[11]
        RBus_UInt32  hpf_r_enable:1;						// BIT[12]
        RBus_UInt32  res2:1;								// BIT[13]
        RBus_UInt32  rep_r_status:1;						// BIT[14]
        RBus_UInt32  mute_r:1;								// BIT[15]
        RBus_UInt32  vol_l_b:6;								// BIT[16:21]
        RBus_UInt32  vol_l_a:5;								// BIT[22:26]
        RBus_UInt32  vol_l_ready:1;							// BIT[27]
        RBus_UInt32  hpf_l_enable:1;						// BIT[28]
        RBus_UInt32  res1:1;								// BIT[29]
        RBus_UInt32  rep_l_status:1;						// BIT[30]
        RBus_UInt32  mute_l:1;								// BIT[31]
    };
} lr_digital_volume_control_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  vol_rs_b:6;							// BIT[00:05]
        RBus_UInt32  vol_rs_a:5;							// BIT[06:10]
        RBus_UInt32  vol_rs_ready:1;						// BIT[11]
        RBus_UInt32  hpf_rs_enable:1;						// BIT[12]
        RBus_UInt32  res2:1;								// BIT[13]
        RBus_UInt32  rep_rs_status:1;						// BIT[14]
        RBus_UInt32  mute_rs:1;								// BIT[15]
        RBus_UInt32  vol_ls_b:6;							// BIT[16:21]
        RBus_UInt32  vol_ls_a:5;							// BIT[22:26]
        RBus_UInt32  vol_ls_ready:1;						// BIT[27]
        RBus_UInt32  hpf_ls_enable:1;						// BIT[28]
        RBus_UInt32  res1:1;								// BIT[29]
        RBus_UInt32  rep_ls_status:1;						// BIT[30]
        RBus_UInt32  mute_ls:1;								// BIT[31]
    };
} ls_digital_volume_control_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  vol_sw_b:6;							// BIT[00:05]
        RBus_UInt32  vol_sw_a:5;							// BIT[06:10]
        RBus_UInt32  vol_sw_ready:1;						// BIT[11]
        RBus_UInt32  hpf_sw_enable:1;						// BIT[12]
        RBus_UInt32  sw_source:1;							// BIT[13]
        RBus_UInt32  rep_sw_status:1;						// BIT[14]
        RBus_UInt32  mute_sw:1;								// BIT[15]
        RBus_UInt32  vol_c_b:6;								// BIT[16:21]
        RBus_UInt32  vol_c_a:5;								// BIT[22:26]
        RBus_UInt32  vol_c_ready:1;							// BIT[27]
        RBus_UInt32  hpf_c_enable:1;						// BIT[28]
        RBus_UInt32  res1:1;								// BIT[29]
        RBus_UInt32  rep_c_status:1;						// BIT[30]
        RBus_UInt32  mute_c:1;								// BIT[31]
    };
} c_sw_digital_volume_control_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  double_buffer_ack:1;					// BIT[00]
        RBus_UInt32  zcto_status:1;							// BIT[01]
        RBus_UInt32  res1:30;								// BIT[02:31]
    };
} double_buffer_ack_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  dummy_6718:32;							// BIT[00:31]
    };
} dummy_6718_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  dummy_671c:32;							// BIT[00:31]
    };
} dummy_671c_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  dummy_6720:32;							// BIT[00:31]
    };
} dummy_6720_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  dummy_6724:32;							// BIT[00:31]
    };
} dummy_6724_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  res1:32;								// BIT[00:31]
    };
} dummy_6728_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  duration_beeper:8;						// BIT[00:07]
        RBus_UInt32  res2:6;								// BIT[08:13]
        RBus_UInt32  mode_beeper:1;							// BIT[14]
        RBus_UInt32  mute_beeper:1;							// BIT[15]
        RBus_UInt32  beeper_volume:4;						// BIT[16:19]
        RBus_UInt32  res1:4;								// BIT[20:23]
        RBus_UInt32  freq_beeper:8;							// BIT[24:31]
    };
} programmable_beeper_control_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  f_coef:9;								// BIT[00:08]
        RBus_UInt32  res2:1;								// BIT[09]
        RBus_UInt32  spatial_in_r_sel:1;					// BIT[10]
        RBus_UInt32  spatial_in_l_sel:1;					// BIT[11]
        RBus_UInt32  seb_en:1;								// BIT[12]
        RBus_UInt32  psb_en:1;								// BIT[13]
        RBus_UInt32  apf_en:1;								// BIT[14]
        RBus_UInt32  res1:17;								// BIT[15:31]
    };
} spatial_effect_control_0_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  depth2:9;								// BIT[00:08]
        RBus_UInt32  res2:7;								// BIT[09:15]
        RBus_UInt32  depth1:9;								// BIT[16:24]
        RBus_UInt32  res1:7;								// BIT[25:31]
    };
} spatial_effect_control_1_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  avc_up_db:2;							// BIT[00:01]
        RBus_UInt32  avc_down_db:2;							// BIT[02:03]
        RBus_UInt32  avc_level_min:8;						// BIT[04:11]
        RBus_UInt32  avc_level_max:8;						// BIT[12:19]
        RBus_UInt32  avc_noise_thre:8;						// BIT[20:27]
        RBus_UInt32  avc_noise_gain:2;						// BIT[28:29]
        RBus_UInt32  res1:2;								// BIT[30:31]
    };
} auto_volume_control_0_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  avc_mon_win:12;						// BIT[00:11]
        RBus_UInt32  avc_zc_win:12;							// BIT[12:23]
        RBus_UInt32  avc_cnt_thre:8;						// BIT[24:31]
    };
} avc_volume_control_1_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  eq_bp1_a1:16;							// BIT[00:15]
        RBus_UInt32  eq_lp_a1:16;							// BIT[16:31]
    };
} eq_a1_0_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  eq_bp3_a1:16;							// BIT[00:15]
        RBus_UInt32  eq_bp2_a1:16;							// BIT[16:31]
    };
} eq_a1_1_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  eq_bp5_a1:16;							// BIT[00:15]
        RBus_UInt32  eq_bp4_a1:16;							// BIT[16:31]
    };
} eq_a1_2_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  eq_hp_a1:16;							// BIT[00:15]
        RBus_UInt32  res1:16;								// BIT[16:31]
    };
} eq_a1_3_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  eq_bp2_a2:16;							// BIT[00:15]
        RBus_UInt32  eq_bp1_a2:16;							// BIT[16:31]
    };
} eq_a2_1_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  eq_bp4_a2:16;							// BIT[00:15]
        RBus_UInt32  eq_bp3_a2:16;							// BIT[16:31]
    };
} eq_a2_2_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  eq_bp5_a2:16;							// BIT[00:15]
        RBus_UInt32  res1:16;								// BIT[16:31]
    };
} eq_a2_3_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  eq_hp_slope:1;							// BIT[00]
        RBus_UInt32  res1:31;								// BIT[01:31]
    };
} eq_h0_0_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  eq_bp1_h0:16;							// BIT[00:15]
        RBus_UInt32  eq_lp_h0:16;							// BIT[16:31]
    };
} eq_h0_1_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  eq_bp3_h0:16;							// BIT[00:15]
        RBus_UInt32  eq_bp2_h0:16;							// BIT[16:31]
    };
} eq_h0_2_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  eq_bp5_h0:16;							// BIT[00:15]
        RBus_UInt32  eq_bp4_h0:16;							// BIT[16:31]
    };
} eq_h0_3_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  eq_hp_of:1;							// BIT[00]
        RBus_UInt32  eq_lp_of:1;							// BIT[01]
        RBus_UInt32  eq_band1_of:1;							// BIT[02]
        RBus_UInt32  eq_band2_of:1;							// BIT[03]
        RBus_UInt32  eq_band3_of:1;							// BIT[04]
        RBus_UInt32  eq_band4_of:1;							// BIT[05]
        RBus_UInt32  eq_band5_of:1;							// BIT[06]
        RBus_UInt32  res3:1;								// BIT[07]
        RBus_UInt32  eq_hp_enable:1;						// BIT[08]
        RBus_UInt32  eq_lp_enable:1;						// BIT[09]
        RBus_UInt32  eq_band1_enable:1;						// BIT[10]
        RBus_UInt32  eq_band2_enable:1;						// BIT[11]
        RBus_UInt32  eq_band3_enable:1;						// BIT[12]
        RBus_UInt32  eq_band4_enable:1;						// BIT[13]
        RBus_UInt32  eq_band5_enable:1;						// BIT[14]
        RBus_UInt32  res2:1;								// BIT[15]
        RBus_UInt32  res1:16;								// BIT[16:31]
    };
} eq_control_status_register_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  lr_loud_bp1_a1:16;						// BIT[00:15]
        RBus_UInt32  lr_loud_lp0_a1:16;						// BIT[16:31]
    };
} lr_loud_a1_0_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  lr_loud_bp2_a1:16;						// BIT[00:15]
        RBus_UInt32  res1:16;								// BIT[16:31]
    };
} lr_loud_a1_1_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  lr_loud_bp2_a2:16;						// BIT[00:15]
        RBus_UInt32  lr_loud_bp1_a2:16;						// BIT[16:31]
    };
} lr_loud_a2_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  lr_loud_bp1_h0:16;						// BIT[00:15]
        RBus_UInt32  lr_loud_lp0_h0:16;						// BIT[16:31]
    };
} lr_loud_h0_0_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  lr_loud_bp2_h0:16;						// BIT[00:15]
        RBus_UInt32  res1:16;								// BIT[16:31]
    };
} lr_loud_h0_1_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  lsrs_loud_bp1_a1:16;					// BIT[00:15]
        RBus_UInt32  lsrs_loud_lp0_a1:16;					// BIT[16:31]
    };
} lsrs_loud_a1_0_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  lsrs_loud_bp2_a1:16;					// BIT[00:15]
        RBus_UInt32  res1:16;								// BIT[16:31]
    };
} lsrs_loud_a1_1_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  lsrs_loud_bp2_a2:16;					// BIT[00:15]
        RBus_UInt32  lsrs_loud_bp1_a2:16;					// BIT[16:31]
    };
} lsrs_loud_a2_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  lsrs_loud_bp1_h0:16;					// BIT[00:15]
        RBus_UInt32  lsrs_loud_lp0_h0:16;					// BIT[16:31]
    };
} lsrs_loud_h0_0_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  lsrs_loud_bp2_h0:16;					// BIT[00:15]
        RBus_UInt32  res1:16;								// BIT[16:31]
    };
} lsrs_loud_h0_1_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  csw_loud_bp1_a1:16;					// BIT[00:15]
        RBus_UInt32  csw_loud_lp0_a1:16;					// BIT[16:31]
    };
} csw_loud_a1_0_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  csw_loud_bp2_a1:16;					// BIT[00:15]
        RBus_UInt32  res1:16;								// BIT[16:31]
    };
} csw_loud_a1_1_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  csw_loud_bp2_a2:16;					// BIT[00:15]
        RBus_UInt32  csw_loud_bp1_a2:16;					// BIT[16:31]
    };
} csw_loud_a2_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  csw_loud_bp1_h0:16;					// BIT[00:15]
        RBus_UInt32  csw_loud_lp0_h0:16;					// BIT[16:31]
    };
} csw_loud_h0_0_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  csw_loud_bp2_h0:16;					// BIT[00:15]
        RBus_UInt32  res1:16;								// BIT[16:31]
    };
} csw_loud_h0_1_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  ld_bpf2_en_csw:1;						// BIT[00]
        RBus_UInt32  ld_lpf_en_csw:1;						// BIT[01]
        RBus_UInt32  ld_bpf1_en_csw:1;						// BIT[02]
        RBus_UInt32  ld_bpf2_en_lsrs:1;						// BIT[03]
        RBus_UInt32  ld_lpf_en_lsrs:1;						// BIT[04]
        RBus_UInt32  ld_bpf1_en_lsrs:1;						// BIT[05]
        RBus_UInt32  ld_bpf2_en_lr:1;						// BIT[06]
        RBus_UInt32  ld_lpf_en_lr:1;						// BIT[07]
        RBus_UInt32  ld_bpf1_en_lr:1;						// BIT[08]
        RBus_UInt32  res1:23;								// BIT[09:31]
    };
} loudness_control_register_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  ld_bpf2_of_csw:1;						// BIT[00]
        RBus_UInt32  ld_bpf1_of_csw:1;						// BIT[01]
        RBus_UInt32  ld_lpf_of_csw:1;						// BIT[02]
        RBus_UInt32  ld_bpf2_of_lsrs:1;						// BIT[03]
        RBus_UInt32  ld_bpf1_of_lsrs:1;						// BIT[04]
        RBus_UInt32  ld_lpf_of_lsrs:1;						// BIT[05]
        RBus_UInt32  ld_bpf2_of_lr:1;						// BIT[06]
        RBus_UInt32  ld_bpf1_of_lr:1;						// BIT[07]
        RBus_UInt32  ld_lpf_of_lr:1;						// BIT[08]
        RBus_UInt32  res1:23;								// BIT[09:31]
    };
} loudness_status_register_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  loudness_ovf:1;						// BIT[00]
        RBus_UInt32  eq_ovf:1;								// BIT[01]
        RBus_UInt32  dvol_done:1;							// BIT[02]
        RBus_UInt32  res2:12;								// BIT[03:14]
        RBus_UInt32  dvol_int_en:1;							// BIT[15]
        RBus_UInt32  loudness_ovf_en:1;						// BIT[16]
        RBus_UInt32  eq_ovf_en:1;							// BIT[17]
        RBus_UInt32  res1:14;								// BIT[18:31]
    };
} system_int_en_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  hw_processing_source_sel:1;			// BIT[00]
        RBus_UInt32  res1:31;								// BIT[01:31]
    };
} test_data_source_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  w_ch_mix_factor:2;						// BIT[00:01]
        RBus_UInt32  w_ch_12_factor:2;						// BIT[02:03]
        RBus_UInt32  hw_mixing_sel:2;						// BIT[04:05]
        RBus_UInt32  res2:8;								// BIT[06:13]
        RBus_UInt32  hw_pcm_out_sel:2;						// BIT[14:15]
        RBus_UInt32  hw_dac_out_sel:2;						// BIT[16:17]
        RBus_UInt32  hw_spdif_out_sel:2;					// BIT[18:19]
        RBus_UInt32  hw_i2s_out_3_sel:2;					// BIT[20:21]
        RBus_UInt32  hw_i2s_out_2_sel:2;					// BIT[22:23]
        RBus_UInt32  hw_i2s_out_1_sel:2;					// BIT[24:25]
        RBus_UInt32  hw_i2s_out_0_sel:2;					// BIT[26:27]
        RBus_UInt32  res1:4;								// BIT[28:31]
    };
} hw_backplay_transfer_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  lrrr_delay_ctrl_mute:10;				// BIT[00:09]
        RBus_UInt32  lrrr_ramp_step_mute:2;					// BIT[10:11]
        RBus_UInt32  lrrr_zcto_mute:3;						// BIT[12:14]
        RBus_UInt32  lrrr_mute_apply:1;						// BIT[15]
        RBus_UInt32  lrrr_delay_ctrl:10;					// BIT[16:25]
        RBus_UInt32  lrrr_ramp_step:2;						// BIT[26:27]
        RBus_UInt32  lrrr_zcto:3;							// BIT[28:30]
        RBus_UInt32  lrrr_dvol_apply:1;						// BIT[31]
    };
} lrrr_digital_volume_control_0_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  lrrr_dvcm:2;							// BIT[00:01]
        RBus_UInt32  res2:2;								// BIT[02:03]
        RBus_UInt32  lrrr_zcth:5;							// BIT[04:08]
        RBus_UInt32  lrrr_zcto_status:1;					// BIT[09]
        RBus_UInt32  lrrr_dvol_enable:1;					// BIT[10]
        RBus_UInt32  res1:21;								// BIT[11:31]
    };
} lrrr_digital_volume_control_1_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  vol_rr_b:6;							// BIT[00:05]
        RBus_UInt32  vol_rr_a:5;							// BIT[06:10]
        RBus_UInt32  vol_rr_ready:1;						// BIT[11]
        RBus_UInt32  hpf_rr_enable:1;						// BIT[12]
        RBus_UInt32  res2:2;								// BIT[13:14]
        RBus_UInt32  mute_rr:1;								// BIT[15]
        RBus_UInt32  vol_lr_b:6;							// BIT[16:21]
        RBus_UInt32  vol_lr_a:5;							// BIT[22:26]
        RBus_UInt32  vol_lr_ready:1;						// BIT[27]
        RBus_UInt32  hpf_lr_enable:1;						// BIT[28]
        RBus_UInt32  res1:2;								// BIT[29:30]
        RBus_UInt32  mute_lr:1;								// BIT[31]
    };
} lrrr_digital_volume_control_2_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  dummy_6810:32;							// BIT[00:31]
    };
} dummy_6810_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  vol_source_sel:3;						// BIT[00:02]
        RBus_UInt32  res2:1;								// BIT[03]
        RBus_UInt32  ch2_source_sel:1;						// BIT[04]
        RBus_UInt32  ch1_source_sel:1;						// BIT[05]
        RBus_UInt32  res1:26;								// BIT[06:31]
    };
} dvol_control_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  test_fifo_out_sel:3;					// BIT[00:02]
        RBus_UInt32  res3:1;								// BIT[03]
        RBus_UInt32  sin_amplitude:2;						// BIT[04:05]
        RBus_UInt32  int_pattern_sel:2;						// BIT[06:07]
        RBus_UInt32  int_pattern_gen_en:1;					// BIT[08]
        RBus_UInt32  test_fifo_read_enable:1;				// BIT[09]
        RBus_UInt32  res2:2;								// BIT[10:11]
        RBus_UInt32  sif_adc_in_test_sel:1;					// BIT[12]
        RBus_UInt32  sif_adc_in_test_en:1;					// BIT[13]
        RBus_UInt32  res1:18;								// BIT[14:31]
    };
} test_pattern_gen_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  audio_crc_header:24;					// BIT[00:23]
        RBus_UInt32  res1:8;								// BIT[24:31]
    };
} crc_header_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  audio_crc_frame_length:16;				// BIT[00:15]
        RBus_UInt32  audio_crc_ch_sel:3;					// BIT[16:18]
        RBus_UInt32  audio_crc_en:1;						// BIT[19]
        RBus_UInt32  res1:12;								// BIT[20:31]
    };
} crc_contorl_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  audio_crc:24;							// BIT[00:23]
        RBus_UInt32  audio_crc_done:1;						// BIT[24]
        RBus_UInt32  res1:7;								// BIT[25:31]
    };
} crc_status_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  debug_mode_sel:8;						// BIT[00:07]
        RBus_UInt32  audio_digital_output_ch_sel:3;			// BIT[08:10]
        RBus_UInt32  adc_test_ch_sel:1;						// BIT[11]
        RBus_UInt32  bist_fail_rom:1;						// BIT[12]
        RBus_UInt32  bist_done_rom:1;						// BIT[13]
        RBus_UInt32  bist_mode_rom:1;						// BIT[14]
        RBus_UInt32  bist_rstn_rom:1;						// BIT[15]
        RBus_UInt32  res1:16;								// BIT[16:31]
    };
} test_debug_mode_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  dummy_6864:32;							// BIT[00:31]
    };
} pattern_gen_dummy_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  to_ack_en:1;							// BIT[00]
        RBus_UInt32  dummy_6890:3;							// BIT[01:03]
        RBus_UInt32  res1:28;								// BIT[04:31]
    };
} rbus_ack_ctrl_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  ack_fail_addr:12;						// BIT[00:11]
        RBus_UInt32  ack_fail_rw:1;							// BIT[12]
        RBus_UInt32  res1:19;								// BIT[13:31]
    };
} ack_fail_info_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  sel_daad_lp:1;							// BIT[00]
        RBus_UInt32  sel_1ch_2ch:1;							// BIT[01]
        RBus_UInt32  sel_adc_2ch_lp_source:1;				// BIT[02]
        RBus_UInt32  sel_adda_lp:1;							// BIT[03]
        RBus_UInt32  bist_fail_dac0:1;						// BIT[04]
        RBus_UInt32  bist_done_dac0:1;						// BIT[05]
        RBus_UInt32  bist_rstn_dac0:1;						// BIT[06]
        RBus_UInt32  bist_mode_dac0:1;						// BIT[07]
        RBus_UInt32  bist_fail_dac1:1;						// BIT[08]
        RBus_UInt32  bist_done_dac1:1;						// BIT[09]
        RBus_UInt32  bist_rstn_dac1:1;						// BIT[10]
        RBus_UInt32  bist_mode_dac1:1;						// BIT[11]
        RBus_UInt32  bist_fail_adc:1;						// BIT[12]
        RBus_UInt32  bist_done_adc:1;						// BIT[13]
        RBus_UInt32  bist_rstn_adc:1;						// BIT[14]
        RBus_UInt32  bist_mode_adc:1;						// BIT[15]
        RBus_UInt32  res2:4;								// BIT[16:19]
        RBus_UInt32  dac2_afifo_in_drop_sel:2;				// BIT[20:21]
        RBus_UInt32  dac_afifo_in_drop_sel:2;				// BIT[22:23]
        RBus_UInt32  dac2_afifo_empty:1;					// BIT[24]
        RBus_UInt32  dac_afifo_empty:1;						// BIT[25]
        RBus_UInt32  dac2_afifo_full:1;						// BIT[26]
        RBus_UInt32  dac_afifo_full:1;						// BIT[27]
        RBus_UInt32  res1:2;								// BIT[28:29]
        RBus_UInt32  dac2_afifo_enable:1;					// BIT[30]
        RBus_UInt32  dac_afifo_enable:1;					// BIT[31]
    };
} adda_loopback_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  aio1out_source:2;						// BIT[00:01]
        RBus_UInt32  aio2out_source:2;						// BIT[02:03]
        RBus_UInt32  hpout_source:2;						// BIT[04:05]
        RBus_UInt32  lineout_source:2;						// BIT[06:07]
        RBus_UInt32  reg_bb_aio2_mux:3;						// BIT[08:10]
        RBus_UInt32  res3:1;								// BIT[11]
        RBus_UInt32  reg_bb_aio1_mux:3;						// BIT[12:14]
        RBus_UInt32  res2:1;								// BIT[15]
        RBus_UInt32  reg_bb_mono_io:1;						// BIT[16]
        RBus_UInt32  reg_bb_aio2_io:1;						// BIT[17]
        RBus_UInt32  reg_bb_aio1_io:1;						// BIT[18]
        RBus_UInt32  reg_bb_ain_df2se:1;					// BIT[19]
        RBus_UInt32  reg_bb_mono_stereo:1;					// BIT[20]
        RBus_UInt32  reg_bb_mono_in_sel:1;					// BIT[21]
        RBus_UInt32  reg_bb_sw_in:1;						// BIT[22]
        RBus_UInt32  reg_bb_depop:1;						// BIT[23]
        RBus_UInt32  res1:8;								// BIT[24:31]
    };
} analog_in_out_sel_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  mute_r_ain1_adc:1;						// BIT[00]
        RBus_UInt32  mute_l_ain1_adc:1;						// BIT[01]
        RBus_UInt32  mute_r_ain2_adc:1;						// BIT[02]
        RBus_UInt32  mute_l_ain2_adc:1;						// BIT[03]
        RBus_UInt32  mute_r_ain3_adc:1;						// BIT[04]
        RBus_UInt32  mute_l_ain3_adc:1;						// BIT[05]
        RBus_UInt32  mute_r_ain4_adc:1;						// BIT[06]
        RBus_UInt32  mute_l_ain4_adc:1;						// BIT[07]
        RBus_UInt32  mute_r_ain5_adc:1;						// BIT[08]
        RBus_UInt32  mute_l_ain5_adc:1;						// BIT[09]
        RBus_UInt32  mute_r_aio1_adc:1;						// BIT[10]
        RBus_UInt32  mute_l_aio1_adc:1;						// BIT[11]
        RBus_UInt32  mute_r_aio2_adc:1;						// BIT[12]
        RBus_UInt32  mute_l_aio2_adc:1;						// BIT[13]
        RBus_UInt32  mute_monoin_r_adc:1;					// BIT[14]
        RBus_UInt32  mute_monoin_l_adc:1;					// BIT[15]
        RBus_UInt32  mute_avol1_r:1;						// BIT[16]
        RBus_UInt32  mute_avol1_l:1;						// BIT[17]
        RBus_UInt32  res1:14;								// BIT[18:31]
    };
} analog_frontend_mute_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  mute_r_monoin_wireio2:1;				// BIT[00]
        RBus_UInt32  mute_l_monoin_wireio2:1;				// BIT[01]
        RBus_UInt32  mute_r_ain1_wireio2:1;					// BIT[02]
        RBus_UInt32  mute_l_ain1_wireio2:1;					// BIT[03]
        RBus_UInt32  mute_r_ain2_wireio2:1;					// BIT[04]
        RBus_UInt32  mute_l_ain2_wireio2:1;					// BIT[05]
        RBus_UInt32  mute_r_adc_in_wireio2:1;				// BIT[06]
        RBus_UInt32  mute_l_adc_in_wireio2:1;				// BIT[07]
        RBus_UInt32  mute_r_monoin_wireio1:1;				// BIT[08]
        RBus_UInt32  mute_l_monoin_wireio1:1;				// BIT[09]
        RBus_UInt32  mute_r_ain1_wireio1:1;					// BIT[10]
        RBus_UInt32  mute_l_ain1_wireio1:1;					// BIT[11]
        RBus_UInt32  mute_r_ain2_wireio1:1;					// BIT[12]
        RBus_UInt32  mute_l_ain2_wireio1:1;					// BIT[13]
        RBus_UInt32  mute_r_adc_in_wireio1:1;				// BIT[14]
        RBus_UInt32  mute_l_adc_in_wireio1:1;				// BIT[15]
        RBus_UInt32  res1:16;								// BIT[16:31]
    };
} analog_mixer_mute_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  mute_right_dac0_out:1;					// BIT[00]
        RBus_UInt32  mute_left_dac0_out:1;					// BIT[01]
        RBus_UInt32  mute_right_in_amix1:1;					// BIT[02]
        RBus_UInt32  mute_left_in_amix1:1;					// BIT[03]
        RBus_UInt32  mute_in_amix2:1;						// BIT[04]
        RBus_UInt32  mute_right_wire1_out:1;				// BIT[05]
        RBus_UInt32  mute_left_wire1_out:1;					// BIT[06]
        RBus_UInt32  mute_right_wire2_out:1;				// BIT[07]
        RBus_UInt32  mute_left_wire2_out:1;					// BIT[08]
        RBus_UInt32  mute_right_dac1_out:1;					// BIT[09]
        RBus_UInt32  mute_left_dac1_out:1;					// BIT[10]
        RBus_UInt32  mute_right_amp:1;						// BIT[11]
        RBus_UInt32  mute_left_amp:1;						// BIT[12]
        RBus_UInt32  mute_right_lineout:1;					// BIT[13]
        RBus_UInt32  mute_left_lineout:1;					// BIT[14]
        RBus_UInt32  reg_bb_en_amp:1;						// BIT[15]
        RBus_UInt32  reg_bb_m_aio2_r:1;						// BIT[16]
        RBus_UInt32  reg_bb_m_aio2_l:1;						// BIT[17]
        RBus_UInt32  reg_bb_m_aio1_r:1;						// BIT[18]
        RBus_UInt32  reg_bb_m_aio1_l:1;						// BIT[19]
        RBus_UInt32  res1:12;								// BIT[20:31]
    };
} analog_aio_master_hpout_lineout_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  avol_2_in_gain:6;						// BIT[00:05]
        RBus_UInt32  res4:1;								// BIT[06]
        RBus_UInt32  smute_avol_2_en:1;						// BIT[07]
        RBus_UInt32  mono_in_boost_sel:2;					// BIT[08:09]
        RBus_UInt32  soft_volume_ctrl_avol:4;				// BIT[10:13]
        RBus_UInt32  res3:2;								// BIT[14:15]
        RBus_UInt32  avol_1_in_right_gain:6;				// BIT[16:21]
        RBus_UInt32  res2:1;								// BIT[22]
        RBus_UInt32  smute_right_avol_1_en:1;				// BIT[23]
        RBus_UInt32  avol_1_in_left_gain:6;					// BIT[24:29]
        RBus_UInt32  res1:1;								// BIT[30]
        RBus_UInt32  smute_left_avol_1_en:1;				// BIT[31]
    };
} adc_volume_gain_control_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  pow_mono_in:1;							// BIT[00]
        RBus_UInt32  pow_ain5:1;							// BIT[01]
        RBus_UInt32  pow_ain4:1;							// BIT[02]
        RBus_UInt32  pow_ain3:1;							// BIT[03]
        RBus_UInt32  pow_ain2:1;							// BIT[04]
        RBus_UInt32  pow_ain1:1;							// BIT[05]
        RBus_UInt32  pow_aio_mono:1;						// BIT[06]
        RBus_UInt32  pow_mono_out:1;						// BIT[07]
        RBus_UInt32  pow_dachp:1;							// BIT[08]
        RBus_UInt32  pow_aout:1;							// BIT[09]
        RBus_UInt32  pow_aio2:1;							// BIT[10]
        RBus_UInt32  pow_aio1:1;							// BIT[11]
        RBus_UInt32  pow_mono_dac:1;						// BIT[12]
        RBus_UInt32  pow_dacvref:1;							// BIT[13]
        RBus_UInt32  pow_vref:1;							// BIT[14]
        RBus_UInt32  pow_mbias:1;							// BIT[15]
        RBus_UInt32  pow_bb_dac1:1;							// BIT[16]
        RBus_UInt32  pow_bb_dac0:1;							// BIT[17]
        RBus_UInt32  pow_bb_adc:1;							// BIT[18]
        RBus_UInt32  pow_master:1;							// BIT[19]
        RBus_UInt32  pow_mixer:1;							// BIT[20]
        RBus_UInt32  pow_afe:1;								// BIT[21]
        RBus_UInt32  dacvref_mode:1;						// BIT[22]
        RBus_UInt32  res1:9;								// BIT[23:31]
    };
} analog_power_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  afe_pga_vol:6;							// BIT[00:05]
        RBus_UInt32  res2:2;								// BIT[06:07]
        RBus_UInt32  adc_pgabias:4;							// BIT[08:11]
        RBus_UInt32  if_adc_pgapow:1;						// BIT[12]
        RBus_UInt32  res1:19;								// BIT[13:31]
    };
} analog_pga_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  ib_mixer_sum_op:2;						// BIT[00:01]
        RBus_UInt32  ib_dac_df2se:2;						// BIT[02:03]
        RBus_UInt32  ib_dac_vrb:2;							// BIT[04:05]
        RBus_UInt32  ib_dac_dct_op:2;						// BIT[06:07]
        RBus_UInt32  ib_adc_ref_gen_vcm_buf:2;				// BIT[08:09]
        RBus_UInt32  ib_adc_anti_alias_filter:2;			// BIT[10:11]
        RBus_UInt32  ib_adc_diff_op:2;						// BIT[12:13]
        RBus_UInt32  res2:2;								// BIT[14:15]
        RBus_UInt32  vcm:2;									// BIT[16:17]
        RBus_UInt32  gain_adc_anti_aliasing_filter:1;		// BIT[18]
        RBus_UInt32  adc_chopper_clock_en:1;				// BIT[19]
        RBus_UInt32  res1:12;								// BIT[20:31]
    };
} analog_ci01_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  ib_in_3_stage_op:2;					// BIT[00:01]
        RBus_UInt32  ib_io_3_stage_op:2;					// BIT[02:03]
        RBus_UInt32  ib_cascade_io_op:2;					// BIT[04:05]
        RBus_UInt32  ib_hp_amp:2;							// BIT[06:07]
        RBus_UInt32  ib_vref_buffer:2;						// BIT[08:09]
        RBus_UInt32  ib_dacvref_buffer:2;					// BIT[10:11]
        RBus_UInt32  ib_adfe_op:2;							// BIT[12:13]
        RBus_UInt32  res1:18;								// BIT[14:31]
    };
} analog_ci02_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  ib_mono_io_op:2;						// BIT[00:01]
        RBus_UInt32  ib_adc_ref_buffer_op_vrn:2;			// BIT[02:03]
        RBus_UInt32  ib_adc_ref_buffer_op_vrp:2;			// BIT[04:05]
        RBus_UInt32  ib_mono_in_op2:2;						// BIT[06:07]
        RBus_UInt32  ib_mono_in_op1:2;						// BIT[08:09]
        RBus_UInt32  ib_mono_out_op2:2;						// BIT[10:11]
        RBus_UInt32  ib_mono_out_op1:2;						// BIT[12:13]
        RBus_UInt32  ib_master_sum_op:2;					// BIT[14:15]
        RBus_UInt32  res1:16;								// BIT[16:31]
    };
} analog_ci03_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  zc_mute_right_amp:1;					// BIT[00]
        RBus_UInt32  zc_mute_left_amp:1;					// BIT[01]
        RBus_UInt32  zc_mute_right_lineout:1;				// BIT[02]
        RBus_UInt32  zc_mute_left_lineout:1;				// BIT[03]
        RBus_UInt32  zc_reg_bb_m_aio2_r:1;					// BIT[04]
        RBus_UInt32  zc_reg_bb_m_aio2_l:1;					// BIT[05]
        RBus_UInt32  zc_reg_bb_m_aio1_r:1;					// BIT[06]
        RBus_UInt32  zc_reg_bb_m_aio1_l:1;					// BIT[07]
        RBus_UInt32  reg_bb_zcd_curr:2;						// BIT[08:09]
        RBus_UInt32  res3:2;								// BIT[10:11]
        RBus_UInt32  reg_bb_pow_zcdcomp:8;					// BIT[12:19]
        RBus_UInt32  reg_bb_pow_zcd:1;						// BIT[20]
        RBus_UInt32  res2:7;								// BIT[21:27]
        RBus_UInt32  time_out_sel:2;						// BIT[28:29]
        RBus_UInt32  res1:1;								// BIT[30]
        RBus_UInt32  hard_mute_sel:1;						// BIT[31]
    };
} analog_hard_mute_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  dac_water_nomask:1;					// BIT[00]
        RBus_UInt32  res1:7;								// BIT[01:07]
        RBus_UInt32  aio2_zc_sel:2;							// BIT[08:09]
        RBus_UInt32  aio1_zc_sel:2;							// BIT[10:11]
        RBus_UInt32  lineout_zc_sel:2;						// BIT[12:13]
        RBus_UInt32  hp_zc_sel:2;							// BIT[14:15]
        RBus_UInt32  reserved_dummy:16;						// BIT[16:31]
    };
} analog_dummy_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  sel_bb_ck_depop:3;						// BIT[00:02]
        RBus_UInt32  bb_ck_depop_en:1;						// BIT[03]
        RBus_UInt32  res1:28;								// BIT[04:31]
    };
} analog_depop1_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  reg_bb_ib_vcm_det_comp:2;				// BIT[00:01]
        RBus_UInt32  reg_bb_hpout_norm:1;					// BIT[02]
        RBus_UInt32  reg_bb_vcm_ready_soft:1;				// BIT[03]
        RBus_UInt32  reg_bb_en_depop_soft_ctrl:1;			// BIT[04]
        RBus_UInt32  reg_bb_depop_sel:2;					// BIT[05:06]
        RBus_UInt32  reg_bb_en_depop_ckgen:1;				// BIT[07]
        RBus_UInt32  reg_bb_depop_sc_sel:1;					// BIT[08]
        RBus_UInt32  reg_bb_aio_norm:1;						// BIT[09]
        RBus_UInt32  reg_bb_aout_norm:1;					// BIT[10]
        RBus_UInt32  res4:1;								// BIT[11]
        RBus_UInt32  reg_bb_depop_cap_sel:2;				// BIT[12:13]
        RBus_UInt32  reg_bb_en_io_out:1;					// BIT[14]
        RBus_UInt32  res3:1;								// BIT[15]
        RBus_UInt32  reg_bb_depop_cur_sel:3;				// BIT[16:18]
        RBus_UInt32  res2:1;								// BIT[19]
        RBus_UInt32  reg_bb_cio4:6;							// BIT[20:25]
        RBus_UInt32  res1:3;								// BIT[26:28]
        RBus_UInt32  reg_bb_pow_depop_op:1;					// BIT[29]
        RBus_UInt32  reg_bb_pow_depop_core:1;				// BIT[30]
        RBus_UInt32  reg_bb_pow_depop:1;					// BIT[31]
    };
} analog_depop2_RBUS;

typedef union
{
    RBus_UInt32 regValue;
    struct
    {
        RBus_UInt32  reg_bb_reserved:32;					// BIT[00:31]
    };
} analog_depop_dummy_RBUS;


#endif







#endif //_RBUS_AUDIO_REG_H_

