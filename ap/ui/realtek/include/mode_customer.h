/*=============================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2005
  * All rights reserved.
  * ============================================ */

/*================= File Description ================= */
/**
 * @file
 * 	This file is for global structure's declaration.
 *
 * @author 	$Author$
 * @date 	$Date$
 * @version 	$Revision$
 * @ingroup 	mode_customer
 */

/**
* @addtogroup mode_customer
* @{
*/
#ifndef _MODE_CUSTOMER_H
#define _MODE_CUSTOMER_H
#include "sparrow.h"
#include "source.h"
#include "mode.h"

//--------------------------------------------------
// Preset Mode
//--------------------------------------------------
enum PresetModeDef
{
    _VGA_MODE_ALL 	= 0,		  	// Mode 00:	// All Mode
    _MODE_640x350_70HZ,        	  	// Mode 01:
    _MODE_640x400_56HZ,             // Mode 02:
    _MODE_640x400_70HZ,             // Mode 03:
    _MODE_720x400_70HZ,             // Mode 04:
    _MODE_700x570_50HZ,             // Mode 05:
    _MODE_640x480_60HZ,             // Mode 06:
    _MODE_640x480_66HZ,             // Mode 07:
    _MODE_640x480_70HZ,             // Mode 08:
    _MODE_640x480_72HZ,             // Mode 09:
    _MODE_640x480_75HZ,             // Mode 10:
    _MODE_640x480_85HZ,                                         // +1

    _MODE_800x600_56HZ,             // Mode 11:
    _MODE_800x600_60HZ,             // Mode 12:
    _MODE_800x600_70HZ,             // Mode 13:
    _MODE_800x600_72HZ,             // Mode 14:
    _MODE_800x600_75HZ,             // Mode 15:
    _MODE_832x624_75HZ,             // Mode 16:
    _MODE_960x600_60HZ,             // Mode 17:
    _MODE_960x600_72HZ,             // Mode 18:
    _MODE_960x600_75HZ,            // Mode 19:
    _MODE_848x480_60HZ,             // Mode 20:

    _MODE_848x480_75HZ,            	 // Mode 21:
    _MODE_1024x768_60HZ,         	 // Mode 22:
    _MODE_1024x768_66HZ,           	// Mode 23:
    _MODE_1024x768_70HZ,           	// Mode 24:
    _MODE_1024x768_72HZ,		  	// Mode 25:
    _MODE_1024x768_75HZ,           	// Mode 26:
    _MODE_1024x768_85HZ,                                    //+1
    _MODE_1024x800_73HZ,           	// Mode 27:
    _MODE_1152x864_60HZ,           	// Mode 28:
    _MODE_1152x864_70HZ,           	// Mode 29:
    _MODE_1152x864_72HZ,           	// Mode 30:

    _MODE_1152x864_75HZ,           	// Mode 31:
    _MODE_1152x870_75HZ,           	// Mode 32:
    _MODE_1152x900_66HZ,           	// Mode 33:
    _MODE_1152x900_76HZ,           	// Mode 34:
    _MODE_1280x720_60HZ,           	// Mode 35:
    _MODE_1280x720_70HZ,           	// Mode 36:
    _MODE_1280x720_72HZ,           	// Mode 37:
    _MODE_1280x720_75HZ,           	// Mode 38:
    _MODE_1280x768_60HZ,           	// Mode 39:
    _MODE_1280x768_60HZ_RB,     	// Mode 40:

    _MODE_1280x768_70HZ,           	// Mode 41:
    _MODE_1280x768_72HZ,           	// Mode 42:
    _MODE_1280x768_75HZ,           	// Mode 43:
    _MODE_1280x800_60HZ,           	// Mode 44:
    _MODE_1280x800_60HZ_RB,     	// Mode 45:
    _MODE_1280x800_70HZ,              // Mode 46:
    _MODE_1280x800_70HZ_RB,      	// Mode 47:
    _MODE_1280x800_72HZ,      	    // Mode 48:
    _MODE_1280x800_75HZ,            // Mode 49:
    _MODE_1280x960_60HZ,            // Mode 50:

    _MODE_1280x960_70HZ,               // Mode 51:
    _MODE_1280x960_72HZ,               // Mode 52:
    _MODE_1280x960_75HZ,               // Mode 53:
    _MODE_1280x1024_60HZ,          	// Mode 54:
    _MODE_1280x1024_70HZ,          	// Mode 55:
    _MODE_1280x1024_72HZ,          	// Mode 56:
    _MODE_1280x1024_75HZ,          	// Mode 57:

    _MODE_1360x768_60HZ,               // Mode 58:
    _MODE_1440x900_60HZ,               // Mode 59:
    _MODE_1440x900_60RHZ,      	   	// Mode 60:

    _MODE_1440x900_70HZ,               // Mode 61:
    _MODE_1440x900_72HZ,               // Mode 62:
    _MODE_1440x900_75HZ,               // Mode 63:
    _MODE_1600x1200_50HZ,	    	// Mode 64:
    _MODE_1600x1200_60HZ,	        // Mode 65:
    _MODE_1600x1200_65HZ,	    	// Mode 66:
    _MODE_1600x1200_70HZ,	    	// Mode 67:
    _MODE_1600x1200_75HZ,	    	// Mode 68:
    _MODE_1680x1050_60HZ,          	// Mode 69:
    _MODE_1680x1050_60HZ_RB,    	// Mode 70:

    _MODE_1680x1050_75HZ,          	// Mode 71:
    _MODE_1920x1080_60Hz_138,   	// Mode 72
    _MODE_1920x1080_60Hz_148,   	// Mode 73
    _MODE_1920x1080_60Hz_173,   	// Mode 74
    _MODE_1920x1080_60Hz_220,   	// Mode 74-1
    _MODE_1920x1200_60Hz,	    	// Mode 75
    //[Code Sync][AlanLi][0980527][1]
    //add MAC timing
    _MODE_1024x576_60HZ,				// Mode 76:for MAC
    _MODE_1920x1080_60Hz_173MAC, 	// Mode 77:for MAC
    //[Code Sync][AlanLi][0980527][1][end]

    //for YPbPr
    _YPBPR_MODE_ALL,         			// Mode 78
    _MODE_480I,         				// Mode 79
    _MODE_576I,        				// Mode 80:
    _MODE_480P,					// Mode 81: YPbPr  720x 480px60HZ (802R)
    _MODE_576P,  					// Mode 82: YPbPr  720x 576px50HZ (802R)
    _MODE_480P_1440,				// Mode 83: YPbPr  1440x 480px60HZ (VG859)
    _MODE_576P_1440,  				// Mode 84: YPbPr  1440x 576px50HZ (VG859)
    _MODE_720P50,  	   			// Mode 85: YPbPr 1280x 720px50HZ        //forster modified 050823
    _MODE_720P60,  	  			// Mode 86: YPbPr 1280x 720px60HZ (802R) //forster modified 050823
    _MODE_1080I25,   				// Mode 87: YPbPr 1920x1080ix50HZ (802R) //forster modified 050823
    _MODE_1080I30,	 				// Mode 88: YPbPr 1920x1080ix60HZ (802R) //forster modified 050823
    _MODE_1080P50,   				// Mode 89: YPbPr 1920x1080px50HZ (802R) //forster modified 050823
    _MODE_1080P60,	 			// Mode 90:  YPbPr 1920x1080px60HZ (802R) //forster modified 050823
    _MODE_1080PM50,				// Mode 91 : 1080pm50(960x1080)
    _MODE_1080PM60,				// Mode 92 : 1080pm60(960x1080)
    _MODE_1080P24,	 			// Mode 93: YPbPr 1920x1080px24HZ (802R)
    _MODE_1080P25,   				// Mode 94: YPbPr 1920x1080px25HZ (802R)
    _MODE_1080P30,   				// Mode 95: YPbPr 1920x1080px30HZ (802R)
    _MODE_USER_MODE,				// Mode 96: user mode
//for HDMI
    _HDMI_MODE_ALL,
    _HDMI_MODE_480I,         		// Mode 97:
    _HDMI_MODE_576I,        		// Mode 98:
    _HDMI_MODE_480P,			// Mode 99: YPbPr  720x 480px50HZ (802R)
    _HDMI_MODE_576P,  			// Mode 100: YPbPr  720x 576px50HZ (802R)
    _HDMI_MODE_720P,  	   		// Mode 101: YPbPr 1280x 720px50HZ        //forster modified 050823
    _HDMI_MODE_720P50,  	   	// Mode 102: YPbPr 1280x 720px50HZ        //forster modified 050823
    _HDMI_MODE_720P60,  	  	// Mode 103: YPbPr 1280x 720px60HZ (802R) //forster modified 050823
    _HDMI_MODE_1080I,   		// Mode 104: YPbPr 1920x1080ix50HZ (802R) //forster modified 050823
    _HDMI_MODE_1080I25,   		// Mode 105: YPbPr 1920x1080ix50HZ (802R) //forster modified 050823
    _HDMI_MODE_1080I30,	 	// Mode 106: YPbPr 1920x1080ix60HZ (802R) //forster modified 050823
    _HDMI_MODE_1080P,   		// Mode 107: YPbPr 1920x1080px50HZ (802R) //forster modified 050823
    _HDMI_MODE_1080P50,   		// Mode 108: YPbPr 1920x1080px50HZ (802R) //forster modified 050823
    _HDMI_MODE_1080P60,	 	// Mode 109:  YPbPr 1920x1080px60HZ (802R) //forster modified 050823
    _HDMI_MODE_1080PM50,		// Mode 110:  HDMI 960x1080px50HZ (802R) //forster modified 050823
    _HDMI_MODE_1080PM60,		// Mode 111:  HDMI 960x1080px60HZ (802R) //forster modified 050823
    // Double DVS to solve 1080p24 & 1080p30 video flicker
    // Add  HDMI 1080P24 &  HDMI 1080P30
    _HDMI_MODE_1080P24,		// Mode 112:  HDMI 1080P24 (802R)
    _HDMI_MODE_1080P30,		// Mode 113:  HDMI 1080P30 (802R)

//for AV
    _AV_MODE_ALL,         		// Mode 114:
    _AV_MODE_480I,         		// Mode 115:
    _AV_MODE_576I,		        	// Mode 116:

//for SV
    _SV_MODE_ALL,		         	// Mode 117:
    _SV_MODE_480I,		         	// Mode 118:
    _SV_MODE_576I,       		 	// Mode 119:

//for TV
    _TV_MODE_ALL,		         	// Mode 120:
    _TV_MODE_480I,       		  	// Mode 121:
    _TV_MODE_576I,		        	// Mode 122:

//for Ypbpr vip
    _MODE_720P,  	   		     // Mode 123:
    _MODE_1080I,   			     // Mode 124:
    _MODE_1080P,			    // Mode 125:


    _MODE_NEW,				// Mode 126:
    _MODE_NEW1,				// Mode 127:
    _MODE_NEW2,				// Mode 128:
    _MODE_NEW3,				// Mode 129:
    _MODE_NEW4,				// Mode 130:
    _MODE_NEW5,				// Mode 131:
    _MODE_NEW6,				// Mode 132:
    _MODE_NEW7,				// Mode 133:
    _MAX_PRESET_MODE			// Mode 134: the last mode
};

enum HDMIModeDef
{
//for HDMI
    _HDMIMODE_480I=_HDMI_MODE_480I,//_MODE_480I,         			// Mode 00:
    _HDMIMODE_576I,        				// Mode 01:
    _HDMIMODE_480P,					// Mode 02: YPbPr  720x 480px50HZ (802R)
    _HDMIMODE_576P,  					// Mode 03: YPbPr  720x 576px50HZ (802R)
    _HDMIMODE_720P50,  	   			// Mode 04: YPbPr 1280x 720px50HZ        //forster modified 050823
    _HDMIMODE_720P60,  	  			// Mode 05: YPbPr 1280x 720px60HZ (802R) //forster modified 050823
    _HDMIMODE_1080I25,   				// Mode 06: YPbPr 1920x1080ix50HZ (802R) //forster modified 050823
    _HDMIMODE_1080I30,	 			// Mode 07: YPbPr 1920x1080ix60HZ (802R) //forster modified 050823
    _HDMIMODE_1080P50,   				// Mode 08: YPbPr 1920x1080px50HZ (802R) //forster modified 050823
    _HDMIMODE_1080P60,	 			// Mode 09:  YPbPr 1920x1080px60HZ (802R) //forster modified 050823
    _HDMIMODE_1080PM50,				// Mode 10:  HDMI 960x1080px50HZ (802R) //forster modified 050823
    _HDMIMODE_1080PM60,				// Mode 11:  HDMI 960x1080px60HZ (802R) //forster modified 050823
    // Double DVS to solve 1080p24 & 1080p30 video flicker
    // Add  HDMI 1080P24 &  HDMI 1080P30
    _HDMIMODE_1080P24,			// Mode 12:  HDMI 1080P24 (802R)
    _HDMIMODE_1080P30,			// Mode 13:  HDMI 1080P30 (802R)
    _HDMIMODE_NEW					// Mode 14:
};

/**
 *
 * @}
 */
#endif
