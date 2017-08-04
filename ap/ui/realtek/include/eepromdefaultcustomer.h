//----------------------------------------------------------------------------------------------------
// ID Code      : EepromDefault.h No.0001
// Update Note  :
//
//----------------------------------------------------------------------------------------------------

//#ifdef __EEPROM__
//#include "global.h"

//---------------------------------------------------------------------------------
// System Data Default Value
// They are used by tEEPROM_SYSTEM_DATA & tEEPROM_ICM_COLOR in the EepromDefault.h
//---------------------------------------------------------------------------------
#include "flow_struct.h"

#define _VERSION_CODE               0x00000044//+ _CUSTOMER_TYPE + _C_PCB_TYPE + _C_PANEL_TYPE

#define _PATH_VGA					1
// Added by weihao
#define _MONITORFLAG            		0x40
#define _USERFIFOMODE           		0x0f
#define _DISPLAYMODE				RATIO_16_9//RATIO_AUTO//0
#define _IRMODE						IR_A
#define _DIMMER						50

#if CONFIG_ENABLE_FM
#define _FMMODE						RADIO_OFF//RADIO_877  
#endif


#define _VIVID						0
#define _CC_SYS  					_OFF
#define _VCHIP_TYPE					_OFF
#define _ZoomMode					RATIO_16_9//RATIO_AUTO//RATIO_4_3//	 4x3
#if DEBUG_OFF
#define _UART_PATH					0
#else
#define _UART_PATH					_PATH_VGA
#endif



#define	_MAX_NTSC_CATV_NUM 		125
#define	_MAX_PAL_CATV_NUM 	200//Ding modify 08-01-31 //255
#define	_MAX_PALDK_CATV_NUM 	95		//todo:need to modify
#define	_MAX_SECAM_CATV_NUM 	125		//todo:need to modify
#ifndef SOUNDMODE_NUM
#define SOUNDMODE_NUM  7
#endif
#define PALETTEMODE_NUM  7

#define _INPUTSOURCE            	0x01
#define _SUBINPUTSOURCE				0x00
#define _SUBINPUTSOURCE            	0x00
#define _BOOTSOURCE					12 //_PCB_SOURCE_TOTAL_NUM	// auto
#define _COLORSYSTEM					0
#define _PICTUREMODE					1	// standard
#define _SCREEN_SAVER					3	//blue screen
#define _AUDIO_AUTODEMO				0	//disable
#define _AUDIO_AUTODEMO_TYPE			0	//Volumn Style
#ifdef CONFIG_PVR_ENABLE
#define _PVR_QUALITY                         	1
#define _PVR_FLAG						0
#endif
#define _PWR_SAVE_MODE				0	// Daytime


#define _BACKLIGHT              				10


#define _TVBACKLIGHT              			0//allen modify ==>16
#define _COLORTEMP              				1
#define _SHARPNESS              				1
#define _SHARPNESS_HD              			70

#define _PEAKING						0 //for 2622
#define _HUE								50 //for 2622
#define _SATURATION					70 //for 2622
#define _INTENSITY						50 //for 2622
#define _YPbPrHue               				50
#define _YPbPrSaturation        				60
#define _HDMIHue               				50

#define _HDMISaturation        				75


#define _CONTRAST						50 //for 2622
#define _BRIGHTNESS						50 //for 2622
#define _CONTRAST_AV					51//55 //for 2622//reynold set default to 51 for dark field
#define _BRIGHTNESS_AV					52//47 //for 2622//reynold set default to 52 for dark field

#define _GAMMA							1
#define _COLORSETTING					2 //for 2622    // Bit3 ~ Bit 1:DCC, Bit0:ICM onoff
#define _CCSYSTEM						0x00
#define _DLTI							5 //for 2622
#define _DCTI							5 //for 2622
#define _FILM							1 //for 2622
#define _NRSPATIAL						0
#define _NRTEMPORAL					3
#define _CHROMAERRREDUCTION			4
#define _NOISE_ESTIMATION				0
#define _MOTIONADAPTIVE				2
#define _CROSSCOLOR					2 //for 2622
#define _INTRA							11 //for 2622
#define _SCALEUPH						4 //for 2622
#define _SCALEUPV						2 //for 2622
#define _SCALEPK						1 //for 2622
#define _DCCLEVEL				       	0  // jacklong 08/13  	3
#define LOWANGLEEN          				1
//allen add for Display Range 20071001
#define _DisplayRange_FULL				0
#define _DisplayRange_ZOOM				1

// CF 2007 0831
#define _BRIGHTNESS_0					0
#define _BRIGHTNESS_50					127
#define _BRIGHTNESS_100					255
#define _CONTRAST_0						0
#define _CONTRAST_50					136
#define _CONTRAST_100					255
#define _SATURATION_0					0
#define _SATURATION_50					127
#define _SATURATION_100				255

#define _BRIGHTNESS_0_VGA				0
#define _BRIGHTNESS_50_VGA				127
#define _BRIGHTNESS_100_VGA			255
#define _CONTRAST_0_VGA				0
#define _CONTRAST_50_VGA				127
#define _CONTRAST_100_VGA				255
#define _SATURATION_0_VGA				0
#define _SATURATION_50_VGA			127
#define _SATURATION_100_VGA			255

#define _BRIGHTNESS_0_AV				0
#define _BRIGHTNESS_50_AV				127
#define _BRIGHTNESS_100_AV				255
#define _CONTRAST_0_AV					0
#define _CONTRAST_50_AV				127
#define _CONTRAST_100_AV				255
#define _SATURATION_0_AV				0
#define _SATURATION_50_AV				127
#define _SATURATION_100_AV			255

#define _BRIGHTNESS_0_HDMI			0
#define _BRIGHTNESS_50_HDMI			127
#define _BRIGHTNESS_100_HDMI			255
#define _CONTRAST_0_HDMI				0
#define _CONTRAST_50_HDMI				127
#define _CONTRAST_100_HDMI			255
#define _SATURATION_0_HDMI			0
#define _SATURATION_50_HDMI			127
#define _SATURATION_100_HDMI			255

#define _BRIGHTNESS_0_YPBPR			0
#define _BRIGHTNESS_50_YPBPR			127
#define _BRIGHTNESS_100_YPBPR			255
#define _CONTRAST_0_YPBPR				0
#define _CONTRAST_50_YPBPR			127
#define _CONTRAST_100_YPBPR			255
#define _SATURATION_0_YPBPR			0
#define _SATURATION_50_YPBPR			127
#define _SATURATION_100_YPBPR			255




//***********yyadd *** _color_temp_menu//
/* Asen 20070917 modify for user color temperature work normally begin*/
/*
#define _COLOR_TEMP_USER_R	128
#define _COLOR_TEMP_USER_G	128
#define _COLOR_TEMP_USER_B	128 // original: 144

*/
#define _COLOR_TEMP_USER_R			100
#define _COLOR_TEMP_USER_G			100
#define _COLOR_TEMP_USER_B			100
/* Asen 20070917 modify for user color temperature work normally end*/
//***********yy end******


//-------- ICM --------
#define _SAT_SC_ENABLE					0
#define _SAT_SC_SLOP					8
#define _INT_SC_ENABLE					0
#define _INT_SC_SLOP					8
#define _ICM_ADJ_SELECT					0
#define _ICM_ADJ_ENABLE				0
#define _ICM_ADJ_ENLIST					0x55

#define _SYSTEM_DATA_RESERVED0  		0x00
#define _ICM_R_HUE						0x08
#define _ICM_R_SAT						0x08
#define _ICM_G_HUE						0x08
#define _ICM_G_SAT						0x08
#define _ICM_B_HUE						0x08
#define _ICM_B_SAT						0x08
#define _ICM_Y_HUE						0x08
#define _ICM_Y_SAT						0x08
#define _ICM_C_HUE						0x08
#define _ICM_C_SAT						0x08
#define _ICM_COLOR_RESERVED0    		0x00
#define _ICM_COLOR_RESERVED1    		0x00
#define _ICM_COLOR_RESERVED2    		0x00
#define _ICM_COLOR_RESERVED3    		0x00
#define _ICM_COLOR_RESERVED4    		0x00
#define _ICM_COLOR_RESERVED5    		0x00

//--------------------------------------------------
// OSD User Data Default Value
// They are used by tEEPROM_OSDUSER_DATA
//--------------------------------------------------
#define _OSD_H_POSITION             		0x64
#define _OSD_V_POSITION             		0x64

#define _LANGUAGE                   			0x00
#define _OSD_TIMEOUT                			30
#define _VOLUME                     			0x20
#define _FILTER                     				0x01
#define _SCARTDETECT                     		0x00
#define _OSD_BLEND						0x02

#define _OSD_PASSWORD0				0x00
#define _OSD_PASSWORD1				0x00
#define _OSD_PASSWORD2				0x00
#define _OSD_PASSWORD3				0x00
//#define _OSDUser_RESERVED0                      0x00
#define	_OSDUSER_DATA_RESERVED0     	0x00
#define _OSDUSER_DATA_RESERVED1     	0x00
#define _OSDUSER_DATA_RESERVED2     	0x00
#define _OSDUSER_DATA_RESERVED3     	0x00
#define _OSDUSER_DATA_RESERVED4     	0x00


//--------------------------------------------------
// VGA ADC Gain/Offset Default Value
// They are used by tEEPROM_ADC_DATA
//--------------------------------------------------
#define _ADC_GAIN_RED               			0x83//0x80
#define _ADC_GAIN_GREEN             		0x8c//0x80
#define _ADC_GAIN_BLUE              			0x8b//0x80
#define _ADC_OFFSET_RED             		0x82//0x80
#define _ADC_OFFSET_GREEN           		0x89//0x80
#define _ADC_OFFSET_BLUE           		0x80
#define _ADC_DATA_RESERVED0         		0x00
#define _ADC_DATA_RESERVED1         		0x00
#define _ADC_DATA_RESERVED2         		0x00
#define _ADC_DATA_RESERVED3         		0x00
#define _ADC_DATA_RESERVED4         		0x00
#define _ADC_DATA_RESERVED5         		0x00
#define _ADC_DATA_RESERVED6         		0x00
#define _ADC_DATA_RESERVED7         		0x00
#define _ADC_DATA_RESERVED8         		0x00
#define _ADC_DATA_RESERVED9         		0x00

//--------------------------------------------------
// YPbPr ADC Gain/Offset Default Value
// They are used by tEEPROM_YPbPr_DATA
//--------------------------------------------------
#define _YPbPr_GAIN_RED               		0x48//0x80
#define _YPbPr_GAIN_GREEN             		0x8d//0x80
#define _YPbPr_GAIN_BLUE              		0x4c//0x80
#define _YPbPr_OFFSET_RED             		0x76//0x80
#define _YPbPr_OFFSET_GREEN           		0x86//0x80
#define _YPbPr_OFFSET_BLUE            		0x72//0x80
#if 0
#if(_VIDEO_AUTO_WHITE_BLANCE == _ENABLE)
#define _VDC_GAIN						0x80// VDC Contrast
#define _VDC_OFFSET						0x80// VDC Brightness
#define _VDC_CHOMA						0xA0// VDC Saturation
#define _YPbPr_DATA_RESERVED0        	0x00
#define _YPbPr_DATA_RESERVED1      	0x00
#define _YPbPr_DATA_RESERVED2       	0x00
#define _YPbPr_DATA_RESERVED3       	0x00
#define _YPbPr_DATA_RESERVED4     	0x00
#define _YPbPr_DATA_RESERVED5     	0x00
#define _YPbPr_DATA_RESERVED6      	0x00
#else //(_VIDEO_AUTO_WHITE_BLANCE == _ENABLE)
#define _YPbPr_DATA_RESERVED0           0x00
#define _YPbPr_DATA_RESERVED1           0x00
#define _YPbPr_DATA_RESERVED2           0x00
#define _YPbPr_DATA_RESERVED3           0x00
#define _YPbPr_DATA_RESERVED4           0x00
#define _YPbPr_DATA_RESERVED5           0x00
#define _YPbPr_DATA_RESERVED6           0x00
#define _YPbPr_DATA_RESERVED7           0x00
#define _YPbPr_DATA_RESERVED8           0x00
#define _YPbPr_DATA_RESERVED9           0x00
#endif //(_VIDEO_AUTO_WHITE_BLANCE == _ENABLE)
#endif

//--------------------------------------------------
// Color Temperature Default Value
// They are used by tEEPROM_COLORTEMP_DATA
//--------------------------------------------------
#define _CT9300_RED_VGA             108//0x67
#define _CT9300_GREEN_VGA           113//0x61
#define _CT9300_BLUE_VGA            121//0x56
#define _CT6500_RED_VGA             120//0x61
#define _CT6500_GREEN_VGA           128//0x60
#define _CT6500_BLUE_VGA            115//0x63
#define _CT5800_RED_VGA             121//0x69
#define _CT5800_GREEN_VGA           128//0x61
#define _CT5800_BLUE_VGA            105//0x4e
#define _CTSRGB_RED_VGA             0x75
#define _CTSRGB_GREEN_VGA           0x80
#define _CTSRGB_BLUE_VGA            0x73
#define _CTUSER_RED_VGA             0x80
#define _CTUSER_GREEN_VGA           0x80
#define _CTUSER_BLUE_VGA            0x80

#define _CT9300_RED_DVI             108//0x67
#define _CT9300_GREEN_DVI           113//0x61
#define _CT9300_BLUE_DVI            121//0x56
#define _CT6500_RED_DVI             120//0x61
#define _CT6500_GREEN_DVI           128//0x60
#define _CT6500_BLUE_DVI            115//0x63
#define _CT5800_RED_DVI             121//0x69
#define _CT5800_GREEN_DVI           128//0x61
#define _CT5800_BLUE_DVI            105//0x4e
#define _CTSRGB_RED_DVI             0x75
#define _CTSRGB_GREEN_DVI           0x80
#define _CTSRGB_BLUE_DVI            0x73
#define _CTUSER_RED_DVI             0x80
#define _CTUSER_GREEN_DVI           0x80
#define _CTUSER_BLUE_DVI            0x80

#define _CT9300_RED_VIDEO           0x80
#define _CT9300_GREEN_VIDEO         0x80
#define _CT9300_BLUE_VIDEO          0x80
#define _CT6500_RED_VIDEO           0x80
#define _CT6500_GREEN_VIDEO         0x80
#define _CT6500_BLUE_VIDEO          0x80
#define _CT5800_RED_VIDEO           0x80
#define _CT5800_GREEN_VIDEO         0x80
#define _CT5800_BLUE_VIDEO          0x80
#define _CTSRGB_RED_VIDEO           0x80
#define _CTSRGB_GREEN_VIDEO         0x80
#define _CTSRGB_BLUE_VIDEO          0x80
#define _CTUSER_RED_VIDEO           0x80
#define _CTUSER_GREEN_VIDEO         0x80
#define _CTUSER_BLUE_VIDEO          0x80
#define _CT9300_PALETTE_VIDEO_BAR1           (50)
#define _CT9300_PALETTE_VIDEO_BAR2        (50)
#define _CT9300_PALETTE_VIDEO_BAR3         (50)
#define _CT9300_PALETTE_VIDEO_BAR4        50
#define _CT9300_PALETTE_VIDEO_BAR5       60
#define _CT9300_PALETTE_VIDEO_BAR6         50
#define _CT9300_PALETTE_VIDEO_BAR7         60
#define _CT9300_PALETTE_VIDEO_BAR8       60
#define _CT9300_PALETTE_VIDEO_BAR9        80

//yymodify 1.8
#define _CT8200_PALETTE_VIDEO_BAR1           (40)//yy1.7
#define _CT8200_PALETTE_VIDEO_BAR2        (40)
#define _CT8200_PALETTE_VIDEO_BAR3         (30)
#define _CT8200_PALETTE_VIDEO_BAR4           (50)
#define _CT8200_PALETTE_VIDEO_BAR5     (45)
#define _CT8200_PALETTE_VIDEO_BAR6        (50)
#define _CT8200_PALETTE_VIDEO_BAR7           (60)
#define _CT8200_PALETTE_VIDEO_BAR8        (50)
#define _CT8200_PALETTE_VIDEO_BAR9        (60)



#define _CT7300_PALETTE_VIDEO_BAR1        60//   (40)
#define _CT7300_PALETTE_VIDEO_BAR2       54// (40)
#define _CT7300_PALETTE_VIDEO_BAR3        55// (40)
#define _CT7300_PALETTE_VIDEO_BAR4       60//    (50)
#define _CT7300_PALETTE_VIDEO_BAR5       60// (45)
#define _CT7300_PALETTE_VIDEO_BAR6       60// (50)
#define _CT7300_PALETTE_VIDEO_BAR7        62//   (60)
#define _CT7300_PALETTE_VIDEO_BAR8      70//   (50)
#define _CT7300_PALETTE_VIDEO_BAR9       70//  (65)


#define _CT6500_PALETTE_VIDEO_BAR1           (60)
#define _CT6500_PALETTE_VIDEO_BAR2        (70)
#define _CT6500_PALETTE_VIDEO_BAR3         (70)
#define _CT6500_PALETTE_VIDEO_BAR4           (80)
#define _CT6500_PALETTE_VIDEO_BAR5       (75)
#define _CT6500_PALETTE_VIDEO_BAR6         (60)
#define _CT6500_PALETTE_VIDEO_BAR7           (65)
#define _CT6500_PALETTE_VIDEO_BAR8        (65)
#define _CT6500_PALETTE_VIDEO_BAR9       (65)
//yy1.8end


#define _CTSTD_PALETTE_VIDEO_BAR1           (50)//Ding modify 08-1-21(60)
#define _CTSTD_PALETTE_VIDEO_BAR2        (50)
#define _CTSTD_PALETTE_VIDEO_BAR3         (50)
#define _CTSTD_PALETTE_VIDEO_BAR4           (50)
#define _CTSTD_PALETTE_VIDEO_BAR5       (50)
#define _CTSTD_PALETTE_VIDEO_BAR6        (50)
#define _CTSTD_PALETTE_VIDEO_BAR7         (50)
#define _CTSTD_PALETTE_VIDEO_BAR8       (50)
#define _CTSTD_PALETTE_VIDEO_BAR9         (50)

#define _CTUSER_PALETTE_VIDEO_BAR1           (50)
#define _CTUSER_PALETTE_VIDEO_BAR2        (50)
#define _CTUSER_PALETTE_VIDEO_BAR3         (50)
#define _CTUSER_PALETTE_VIDEO_BAR4           (50)
#define _CTUSER_PALETTE_VIDEO_BAR5       (50)
#define _CTUSER_PALETTE_VIDEO_BAR6         (50)
#define _CTUSER_PALETTE_VIDEO_BAR7           (50)
#define _CTUSER_PALETTE_VIDEO_BAR8        (50)
#define _CTUSER_PALETTE_VIDEO_BAR9         (50)

//--------------------------------------------------
// TV Default Value
// They are used by tEEPROM_TV_DATA
//--------------------------------------------------
#define _TV_DEFAULT_SIG_MODE 	_SIG_MODE_NTSC//_SIG_MODE_TYPE
#define _TV_DEFAULT_CH_NUM     	2
#define _TV_DEFAULT_PREV_CH_NUM _TV_DEFAULT_CH_NUM
#define _TV_DEFAULT_SET_FLAG   	0x02			//CATV
#if 0//(CONFIG_NR_ENBALE)
#define _TV_DEFAULT_NRLEVEL     0x05
#endif
#define _TV_DATA_RESERVED0      0x00
#define _TV_DATA_RESERVED1      0x00
#define _TV_DATA_RESERVED2      0x00
#define _TV_DATA_RESERVED3      0x00
#define _TV_DATA_RESERVED4      0x00
#define _TV_DATA_RESERVED5      0x00
#define _TV_DATA_RESERVED6      0x00

//---------------------------------------------------
// Audio defualt value
// They are used by tEEPROM_AUDIO_DATA
//---------------------------------------------------
#define _AUDIO_DATA_RESERVED0    0x00
#define _AUDIO_DATA_RESERVED1    0x00
#define _AUDIO_DATA_RESERVED2    0x00
#define _AUDIO_DATA_RESERVED3    0x00
#define _AUDIO_DATA_RESERVED4    0x00
#define _AUDIO_DATA_RESERVED5    0x00
#define _AUDIO_DATA_RESERVED6    0x00
#define _AUDIO_DATA_RESERVED7    0x00
#define _AUDIO_DATA_RESERVED8    0x00
#define _AUDIO_DATA_RESERVED9    0x00

//#else
//#endif

#ifdef __EEPROM_CUSTOMER__
///////////Below are customer extra definitaion///////////////////////////
#endif

