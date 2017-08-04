/*
 * Automatically generated C config: don't edit
 * Linux kernel version: 
 * Sun Sep 19 17:25:07 2010
 */
#define AUTOCONF_INCLUDED
#undef CONFIG_SPARROW_VER_A
#define CONFIG_SPARROW_VER_B 1
#define CONFIG_CPU_XTAL_FREQ 27000000

/*
 * Storage related
 */
#define CONFIG_STORAGE_ACCESS_FLASH 1
#undef CONFIG_STORAGE_ACCESS_EEPROM
#undef CONFIG_FLASH_8M_SIZE //1

/*
 * Debug related
 */
//#define CONFIG_DEFAULT_POWER_ON 1
#undef CONFIG_DEFAULT_POWER_ON
#undef CONFIG_ENABLE_AUTO_TEST
#undef CONFIG_ENABLE_AUTO_SCAN_AFTER_POEWR_ON
#define CONFIG_ENABLE_RTD_LOG	1

/*
 * Driver related
 */

/*
 * panel select
 */
#undef CONFIG_PANEL_AU_M170EG01  
#undef CONFIG_PANEL_AU170_480p
#undef CONFIG_PANEL_AUM22001AGCL   
#undef CONFIG_PANEL_AUM240HW01_V0
#undef CONFIG_PANEL_AUT315XW01
#undef CONFIG_PANEL_AUT32PV320TVM
#undef CONFIG_PANEL_AUT37PV370TVM
#undef CONFIG_PANEL_CMO_V420H1
#undef CONFIG_PANEL_LG_47
#undef CONFIG_PANEL_LG_32 
#undef CONFIG_PANEL_CPT_32
#undef CONFIG_PANEL_LG_37
#undef CONFIG_PANEL_SAMSUNG_32
#undef CONFIG_PANEL_SAMSUNG_46
#undef CONFIG_PANEL_HT_32
#undef CONFIG_PANEL_CMO_V420B1_LH1
#undef CONFIG_PANEL_AU_M220EW01 
#undef CONFIG_PANEL_CMO_V315B3L01
#undef CONFIG_PANEL_CMO_V320B1L02
#undef CONFIG_PANEL_CMO_V320B1L07
#undef CONFIG_PANEL_CMO_V260B1L02
#undef CONFIG_PANEL_CMO_V216
#undef CONFIG_PANEL_LG_V420V02
#undef CONFIG_PANEL_LG_420WUNSAA1
#undef CONFIG_PANEL_CMO_M190A1L02 //1
#undef CONFIG_PANEL_AU_M190PW01 
#undef CONFIG_PANEL_MT_19AW01
#undef CONFIG_PANEL_HANNSTAR_HSD250MUW1
#undef CONFIG_PANEL_HANNSTAR_HSD280MUW1
#undef CONFIG_PANEL_CMO_22M220J1L01
#undef CONFIG_PANEL_CMO_M216H1_L01 //1
#undef CONFIG_PANEL_CMO_N116B6_L02
#undef CONFIG_PANEL_LVDS_1280x1024
#undef CONFIG_PANEL_LVDS_1280x800 
#undef CONFIG_PANEL_SHARP_LQ104W1LH02
#undef CONFIG_PANEL_LVDS_1024x768  
#undef CONFIG_PANEL_LVDS_800x600
#undef CONFIG_PANEL_LVDS_1024x600  
#undef CONFIG_PANEL_TTL_640x480	


#undef CONFIG_PANEL_TTL_1024x600	 
#define CONFIG_PANEL_TTL_800x480	1
/*
 * irda select
 */
#define CONFIG_IRDA_PROTOCOL_NEC 1
#undef CONFIG_IRDA_PROTOCOL_RC5
#undef CONFIG_IRDA_PROTOCOL_Panasonic
#undef CONFIG_IRDA_PROTOCOL_Sony
#undef CONFIG_IRDA_PROTOCOL_Sharp_48_bit
#undef CONFIG_IRDA_PROTOCOL_Skyworth
#undef CONFIG_IRDA_PROTOCOL_Customerize

/*
 * vbi
 */
#undef CONFIG_VBI_SUPPORT	//1
#undef CONFIG_WSS_SUPPORT
#undef CONFIG_VBI_CC_SUPPORT	//1

/*
 * flash_related
 */
#define CONFIG_FLASH_USE_2BITS_READ 1

/*
 * video
 */
#undef CONFIG_ENABLE_FHFVBug
#undef CONFIG_ENABLE_AV_FHFV
#undef CONFIG_SUPPORT_PAL60

/*
 * scaler_idomain
 */
#undef CONFIG_YPBPR_USE_AUTO_SOY

/*
 * hdmi
 */
#define CONFIG_ENABLE_HDMI		1
#define _VIDEO_TV_SUPPORT 		1
#define _VIDEO_VGA_SUPPORT 		0
#define _DISABLE_ITEM_OPTION 	0
#define _MELON_DEBUG			1
#define CONFIG_ENABLE_FM		1
#define DISABLE_OPTTION   		0
#define CONFIG_ENABLE_USB  		0
#define CONFIG_ENABLE_INFO 		1
#define ENABLE_FM_FREQSET   	1
#define DEBUG_OFF	        	1
#define MELON_DEBUG_TEST		0
#define CONFIG_USB_CHECK		0
#define CONFIG_TU300			0
#define ENABLE_AUDIO_SEP		1
#define ENABLE_OPENDISPLAY_CTL	0
/*
 * Tuner
 */
 #define CONFIG_TUNER_SILICON    1
//RT610S 
#define _SILICON_JIE_CODE	1		//11

 //RT610S&&RT810
#undef _SILICON_BRYAN_CODE	
 #undef CONFIG_TUNER_SILICON_RT810 
 #define CONFIG_TUNER_SLICON_RT610  1





#undef CONFIG_TUNER_DEFAULT  //1
#undef CONFIG_TUNER_XUGUANG_TDQ6K_T116CW //1 
#undef CONFIG_TUNER_QINGJIA_AFT7_W000G
#undef CONFIG_TUNER_XUGUANG_T114
#undef CONFIG_TUNER_SAMSUNG_DTVS203FH201B
#undef CONFIG_TUNER_THOMSON_76801
#undef CONFIG_TUNER_SAMSUNG_DTVS205EH201A
#undef CONFIG_TUNER_SAMSUNG_DTVS205FH201A
#undef CONFIG_TUNER_NXP_TD1636ALFMK2
#undef CONFIG_TUNER_CHANGHONG_DTI5_C400F1VH
#undef CONFIG_TUNER_ALPS_TDAG4_D01A
#undef CONFIG_TUNER_LG_TDTC_G29D
#undef CONFIG_TUNER_KEH_FTDC3S20V04
#undef CONFIG_TUNER_XUGUANG_TDQ_6FT_W116H
#undef CONFIG_TUNER_XUGUANG_TDQ_6ID_GW115HR
#undef CONFIG_TUNER_XUGUANG_HFT8B_115CW
#undef CONFIG_TUNER_XUGUANG_DVT_8ADC1_T41F0HS
#undef CONFIG_TUNER_TCL_F41CT_2_E
#undef CONFIG_TUNER_TCL_F13WP_2DP_E
#undef CONFIG_TUNER_CHANGHONG_DTI1_E4I12VH
#undef CONFIG_TUNER_ALPS_TDAU3XD01A
#undef CONFIG_TUNER_CDT_3FC2F4_01_BJT
#undef CONFIG_TUNER_TVISION_TVF85P4_AF_3XG_1
/*
 * IFD
 */
#define CONFIG_IFD_ENABLE 1

/*
 * IFD Driver related
 */
#undef CONFIG_IFD_NONSTD_3P10_DETECT
#undef CONFIG_IFD_OUTPUT_CVBS_AMP_COMPENSATE
#define CONFIG_IFD_CR_VARIANCE_DETECT 1
#undef CONFIG_TUNER_SILICON_DEBUG_MSG

/*
 * Debug messages
 */
#undef CONFIG_IFD_DRVIF_DEBUG_MSG
#undef CONFIG_IFD_TUNER_AUTO

/*
 * TV related
 */
#define CONFIG_SEQUENTIAL_SCAN 1
#undef CONFIG_TABLE_WEIGHT_SCAN
#define CONFIG_ALL_AUTO_SCAN_FLOW 1//Edit SVN53
#define CONFIG_CHINA_AUTO_SCAN_FLOW  1
#undef CONFIG_EUROPE_AUTO_SCAN_FLOW
#undef CONFIG_USA_AUTO_SCAN_FLOW
#undef CONFIG_TV_AUDIO_MONO_ONLY
#undef CONFIG_TV_AUDIO_BTSC_TYPE
#define CONFIG_TV_AUDIO_EURO_TYPE 1
#undef CONFIG_ENABLE_TV_NOISE_BOX_ENALBE

/*
 * audio
 */
#define CONFIG_ENABLE_AUDIO 1
#define CONFIG_AUDIO_DEFAULT_HWP_TABLE 1
#undef CONFIG_AUDIO_DEFAULT_ATV_TABLE 
#define CONFIG_AUDIO_DEFAULT_AIO_TABLE 1
#undef CONFIG_AUDIO_ENABLE_AIO_LOG
#undef CONFIG_AUDIO_ENABLE_ATV_LOG
#undef CONFIG_AUDIO_ENABLE_HWP_LOG

/*
 * ATV setting
 */
#define CONFIG_AUDIO_ATV_SUPPORT_BGDKI 1
#undef CONFIG_AUDIO_ATV_SUPPORT_BGDKI_DEV_MODE_HIGH
#define CONFIG_AUDIO_ATV_SUPPORT_L 1
#undef CONFIG_AUDIO_ATV_SUPPORT_NICAM 
#define CONFIG_AUDIO_ATV_SUPPORT_M 1
#undef CONFIG_AUDIO_ATV_SUPPORT_M_DEV_MODE_HIGH
#undef CONFIG_AUDIO_ATV_SUPPORT_BTSC
#define CONFIG_AUDIO_ATV_SUPPORT_AUTO_SOUND_MODE 1
#define CONFIG_AUDIO_ATV_SUPPORT_FREQ_SHIFT 1
#undef CONFIG_AUDIO_ATV_FREQ_SHIFT_PRESCAN
#define CONFIG_AUDIO_ATV_FREQ_SHIFT_RANGE 300

/*
 * vip
 */

/*
 * Teletext
 */
#undef CONFIG_TTX_SUPPORT

/*
 * Customer related
 */
#define CONFIG_APP_EURO	1//Edit SVN53
#define CONFIG_APP_CHINA 1

/*
 * PCB related
 */
#define CONFIG_RTD2668_PCB1_DEMO_VER1 1
#undef CONFIG_RTD2668_PCB1_DEMO_VER4
#undef CONFIG_RTD2668_PCB1_QC_VER1
#undef CONFIG_RTD2668_PCB_LONGYUAN_VER1

/*
 * UI related
 */
#define CONFIG_CUSTOMER_REALTEK 1

/*
 * RTK OSD options
 */
#define CONFIG_CHANNEL_TABLE_SCAN 1
#define CONFIG_CHANNEL_TABLE_SHIFT_SCAN 1
#undef CONFIG_MAINICON_4BITS_ANIMATION
#define CONFIG_EUROPE_LANGUAGE 1
#undef CONFIG_FAC_DEFAULT_LANG_OPT

/*
 * EUROPE functions
 */
#undef CONFIG_SCART_AUTO_SWITCH
//#undef _ONLY_NTSC_SYSTEM		//1
#undef _TABLE_MODE				//1
//==============================================================================
// Support FIVE_KEY
//==============================================================================
#undef SUPPORT_FIVE_KEY	

#undef _REALTEK_OSD_USE				//1

#define _SOURCE_LISTMENU						0
#define _SOURCE_LISTNOTMENU						1
#define _SOURCE_LIST_TYPE						_SOURCE_LISTMENU