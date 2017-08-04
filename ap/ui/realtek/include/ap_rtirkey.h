/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2009
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is for ir and message related functions.
 *
 * @author	$Author: cf.tsai $
 * @date	$Date: 2009/03/17 $
 * @version	$Revision: 10 $
 * @ingroup	ap
 */

/**
 * @addtogroup ap
 * @{
 */

#ifndef _AP_IRKEY_H
#define _AP_IRKEY_H

//#define CUSTOM_CODE				(0xfb04)

#define _IRDA_POWER_KEY		(0xa05ffb04)
#define _IRDA_UP_KEY		(0x00000000)  //NEC IR no detect
#define _IRDA_DOWN_KEY		(0x00000000)  //NEC IR no detect

//--------------------------------------------------
// Remote Control
//--------------------------------------------------
#define _IR_NONE               0
#define _IR_REALTEK            1
#define _IR_DEMO               2
#define _IR_DINGKE             3
#define _IR_DEMO2              4
#define _IR_HTR				   5
#define _IR_HM				   6
#define  _IR_DEMO3                     7
#define _IR_PENGJIA			   8
#define _IR_PJ_JAP				9
#define _IR_PJ_JAP_2			10
#define _IR_PJ_YL7288			11
#define _IR_SAVV			    12

#define _REMOTE_CONTROLLER     _IR_SAVV
#if(_REMOTE_CONTROLLER==_IR_PJ_YL7288) 
#define KEY_BOARD_IR
#endif



#define DVD_CUSTOM_CODE 0xFF00
#define IC_CUSTOM_CODE 0xEF10


/*======================== Definitions ====================== */
//--------------------------------------------------
// Definitions of IR Key
//--------------------------------------------------

#if(_REMOTE_CONTROLLER == _IR_REALTEK)
#define CUSTOM_CODE     	0xfb04
#elif(_REMOTE_CONTROLLER == _IR_DEMO)
#define CUSTOM_CODE     	0x7F80
#elif(_REMOTE_CONTROLLER == _IR_DINGKE)
#define CUSTOM_CODE     	0x7F00
#elif(_REMOTE_CONTROLLER == _IR_DEMO2)
#define CUSTOM_CODE     	0xfe01
#elif(_REMOTE_CONTROLLER == _IR_HTR)
#define CUSTOM_CODE     	0xfb04
#elif(_REMOTE_CONTROLLER == _IR_HM)
#define CUSTOM_CODE     	0xDF20
#elif(_REMOTE_CONTROLLER == _IR_DEMO3)
#define CUSTOM_CODE     	0x807F
#elif(_REMOTE_CONTROLLER == _IR_PENGJIA)
#define CUSTOM_CODE     	0xff00
#elif(_REMOTE_CONTROLLER == _IR_PJ_JAP)
#define CUSTOM_CODE     	0xff00
#elif(_REMOTE_CONTROLLER == _IR_PJ_JAP_2)
#define CUSTOM_CODE     	0xFF00
#elif(_REMOTE_CONTROLLER == _IR_PJ_YL7288)
#define CUSTOM_CODE     	0x7f00
#elif(_REMOTE_CONTROLLER == _IR_SAVV)
#define CUSTOM_CODE     	0xFF00

#endif

#ifdef _IRKEY_
UINT8 code IRKeyMsg[][2] =
{

    /******************************************************************************

                    (_REMOTE_CONTROLLER == _IR_REALTEK)

    *******************************************************************************/

#if(_REMOTE_CONTROLLER == _IR_REALTEK)
    {
        0x5F, _POWER_KEY_MESSAGE },
    {0x57, _SOURCE_IR_MESSAGE		},
    {0x40, _CHANNEL_INC_MESSAGE		},
    {0x5D, _CHANNEL_DEC_MESSAGE		},
    {0x03, _VOLUME_INC_MESSAGE		},
    {0x1F, _VOLUME_DEC_MESSAGE		},
    {0x09, _MENU_KEY_MESSAGE		},
    {0x46, _PIPMP_KEY_MESSAGE		},
    {0x55, _OK_KEY_MESSAGE			},
    {0x06, _ENTER_KEY_MESSAGE		},
    {0x14, _RIGHT_IR_MESSAGE		},
    {0x56, _LEFT_IR_MESSAGE		},
    {0x59, _UP_KEY_MESSAGE			},
    {0x51, _DOWN_KEY_MESSAGE		},
    {0x10, _EXIT_KEY_MESSAGE		},
    {0x42, _CHGUIDE_KEY_MESSAGE		},
    {0x41, _MULTICH_KEY_MESSAGE		},
    {0x1B, _DISPLAY_KEY_MESSAGE		},
    {0x5A, _3D_RESET_MESSAGE		},
    {0x18, _RESET_MESSAGE			},
    {0x5B, _MUTE_KEY_MESSAGE		},
    {0x58, _SLEEP_KEY_MESSAGE		},
    {0x1A, _WIDE_KEY_MESSAGE		},
    {0x52, _JUMP_KEY_MESSAGE		},
    {0x17, _VIDEO_KEY_MESSAGE		},
    {0x16, _AUDIO_KEY_MESSAGE		},
    {0x04, _FREEZE_KEY_MESSAGE		},
    {0x1C, _AUDIO_DEMO_KEY_MESSAGE	},
    {0x02, _VOLUMNBAR_KEY_MESSAGE	},
    {0x47, _DASH_KEY_MESSAGE		},
    {0x54, _CAPTION_KEY_MESSAGE		},
    {0x43, _SIZE_KEY_MESSAGE		},
    {0x5E, _MIXER_KEY_MESSAGE		},

    {0x09, _TTX_INDEXL_KEY_MESSAGE	},
    {0x4E, _REDBUTTON_MESSAGE		},
    {0x0D, _BLUEBUTTON_MESSAGE		},
    {0x4D, _GREENBUTTON_MESSAGE		},
    {0x0C, _YELLOWBUTTON_MESSAGE	},

    {0x44, IR_0_KEY_MESSAGE			},
    {0x53, IR_1_KEY_MESSAGE			},
    {0x50, IR_2_KEY_MESSAGE			},
    {0x12, IR_3_KEY_MESSAGE			},
    {0x4F, IR_4_KEY_MESSAGE			},
    {0x4C, IR_5_KEY_MESSAGE			},
    {0x0E, IR_6_KEY_MESSAGE			},
    {0x4B, IR_7_KEY_MESSAGE			},
    {0x48, IR_8_KEY_MESSAGE			},
    {0x0A, IR_9_KEY_MESSAGE			},

    //????
    {0XAA, _HOTKEY_TIMING_SWAP_MESSAGE	},
    {0xBB, _HOTKEY_RECALL_MESSAGE		},
    {0xCC, _HOTKEY_FACTORY_MODE_MESSAGE	}

    /******************************************************************************

    			_REMOTE_CONTROLLER == _IR_DEMO

    *******************************************************************************/
#elif(_REMOTE_CONTROLLER == _IR_DEMO)
    {
        0x0A, _POWER_KEY_MESSAGE
    },
    {0x17, _SOURCE_IR_MESSAGE      },
    {0x14, _MENU_KEY_MESSAGE        },
    {0x12, _RIGHT_IR_MESSAGE       },
    {0x13, _LEFT_IR_MESSAGE        },
    {0x10, _UP_KEY_MESSAGE          },
    {0x11, _DOWN_KEY_MESSAGE        },
    {0x15, _EXIT_KEY_MESSAGE        },
    {0x0E,     _OK_IR_MESSAGE},
    {0x0B, _MUTE_KEY_MESSAGE        },

    {0x00, IR_0_KEY_MESSAGE         },
    {0x01, IR_1_KEY_MESSAGE         },
    {0x02, IR_2_KEY_MESSAGE         },
    {0x03, IR_3_KEY_MESSAGE         },
    {0x04, IR_4_KEY_MESSAGE         },
    {0x05, IR_5_KEY_MESSAGE         },
    {0x06, IR_6_KEY_MESSAGE         },
    {0x07, IR_7_KEY_MESSAGE         },
    {0x08, IR_8_KEY_MESSAGE         },
    {0x09, IR_9_KEY_MESSAGE         },
#elif(_REMOTE_CONTROLLER == _IR_HM)
    {
        0x09, _POWER_KEY_MESSAGE
    },
    {0x17, _SOURCE_IR_MESSAGE      },
    {0x11, _MENU_KEY_MESSAGE        },
    {0x15, _RIGHT_IR_MESSAGE       },
    {0x1B, _LEFT_IR_MESSAGE        },
    {0x1F, _UP_KEY_MESSAGE          },
    {0x16, _DOWN_KEY_MESSAGE        },
    //{0x15, _EXIT_KEY_MESSAGE        },
    {0x0E,     _OK_IR_MESSAGE},
    {0x19, _MUTE_KEY_MESSAGE        },

    {0x14, IR_0_KEY_MESSAGE         },
    {0x40, IR_1_KEY_MESSAGE         },
    {0x4C, IR_2_KEY_MESSAGE         },
    {0x04, IR_3_KEY_MESSAGE         },
    {0x00, IR_4_KEY_MESSAGE         },
    {0x0A, IR_5_KEY_MESSAGE         },
    {0x1E, IR_6_KEY_MESSAGE         },
    {0x0E, IR_7_KEY_MESSAGE         },
    {0x1A, IR_8_KEY_MESSAGE         },
    {0x1C, IR_9_KEY_MESSAGE         },

#elif(_REMOTE_CONTROLLER == _IR_HTR)
	{
	    0x40, _POWER_KEY_MESSAGE
	},
	{0x13, _SOURCE_IR_MESSAGE      },
	{0x4c, _MENU_KEY_MESSAGE        },
	{0x4a, _RIGHT_IR_MESSAGE       },
	{0x48, _LEFT_IR_MESSAGE        },
	{0x47, _UP_KEY_MESSAGE          },
	{0x4b, _DOWN_KEY_MESSAGE        },
	{0x4d, _EXIT_KEY_MESSAGE        },
	{0x49, _OK_KEY_MESSAGE			},
	
	{0x12, _MUTE_KEY_MESSAGE        },

    {0x0d, _CC_KEY_MESSAGE          },
    {0x11, _DISPLAY_KEY_MESSAGE     },
    {0x1C, _DISPLAY_RATIO_KEY_MESSAGE},
    {0x42, _SLEEP_KEY_MESSAGE		},

	{0x16, _RETURN_KEY_MESSAGE		},
	
	{0x00, IR_0_KEY_MESSAGE         },
	{0x01, IR_1_KEY_MESSAGE         },
	{0x02, IR_2_KEY_MESSAGE         },
	{0x03, IR_3_KEY_MESSAGE         },
	{0x04, IR_4_KEY_MESSAGE         },
	{0x05, IR_5_KEY_MESSAGE         },
	{0x06, IR_6_KEY_MESSAGE         },
	{0x07, IR_7_KEY_MESSAGE         },
	{0x08, IR_8_KEY_MESSAGE         },
	{0x09, IR_9_KEY_MESSAGE         },

#elif(_REMOTE_CONTROLLER == _IR_DINGKE)
    {
        0x12, _POWER_KEY_MESSAGE
    },
    {0x55, _SOURCE_IR_MESSAGE      },
    {0x17, _MENU_KEY_MESSAGE        },
    {0x1A, _RIGHT_IR_MESSAGE       },
    {0x1E, _LEFT_IR_MESSAGE        },
    {0x1B, _UP_KEY_MESSAGE          },
    {0x1F, _DOWN_KEY_MESSAGE        },
    {0x18, _EXIT_KEY_MESSAGE        },
    {0x0E,     _OK_IR_MESSAGE},
    {0x10, _MUTE_KEY_MESSAGE        },

    {0x00, IR_0_KEY_MESSAGE         },
    {0x01, IR_1_KEY_MESSAGE         },
    {0x02, IR_2_KEY_MESSAGE         },
    {0x03, IR_3_KEY_MESSAGE         },
    {0x04, IR_4_KEY_MESSAGE         },
    {0x05, IR_5_KEY_MESSAGE         },
    {0x06, IR_6_KEY_MESSAGE         },
    {0x07, IR_7_KEY_MESSAGE         },
    {0x08, IR_8_KEY_MESSAGE         },
    {0x09, IR_9_KEY_MESSAGE         }
#elif(_REMOTE_CONTROLLER == _IR_DEMO2)
    {
        0x1F, _POWER_KEY_MESSAGE
    },
    {0x0a, _SOURCE_IR_MESSAGE      },
    {0x00, _MENU_KEY_MESSAGE        },
    {0x07, _RIGHT_IR_MESSAGE       },
    {0x04, _LEFT_IR_MESSAGE        },
    {0x02, _UP_KEY_MESSAGE          },
    {0x06, _DOWN_KEY_MESSAGE        },
    {0x80, _EXIT_KEY_MESSAGE        },
    {0x0E,     _OK_IR_MESSAGE},
    {0x1C, _MUTE_KEY_MESSAGE        },

    {0x0C, IR_0_KEY_MESSAGE         },
    {0x18, IR_1_KEY_MESSAGE         },
    {0x1A, IR_2_KEY_MESSAGE         },
    {0x1B, IR_3_KEY_MESSAGE         },
    {0x14, IR_4_KEY_MESSAGE         },
    {0x16, IR_5_KEY_MESSAGE         },
    {0x17, IR_6_KEY_MESSAGE         },
    {0x10, IR_7_KEY_MESSAGE         },
    {0x12, IR_8_KEY_MESSAGE         },
    {0x13, IR_9_KEY_MESSAGE         },
#elif(_REMOTE_CONTROLLER == _IR_DEMO3)
    {0x0C, _POWER_KEY_MESSAGE    },
    {0x46, _SOURCE_IR_MESSAGE      },
    {0x14, _MENU_KEY_MESSAGE        },
    {0x0E,     _OK_IR_MESSAGE},
    {0x4D, _RIGHT_IR_MESSAGE       },
    {0x4C, _LEFT_IR_MESSAGE        },
    {0x0D, _UP_KEY_MESSAGE          },
    {0x0f, _DOWN_KEY_MESSAGE        },
    {0x15, _EXIT_KEY_MESSAGE        },
    {0x44, _CHANNEL_INC_MESSAGE		},
    {0x45, _CHANNEL_DEC_MESSAGE		},
    {0x12, _REDBUTTON_MESSAGE		},
    {0x13, _BLUEBUTTON_MESSAGE		},
    {0x50, _GREENBUTTON_MESSAGE		},
    {0x51, _YELLOWBUTTON_MESSAGE	},

    {0x42, _MUTE_KEY_MESSAGE        },

    {0x00, IR_0_KEY_MESSAGE         },
    {0x01, IR_1_KEY_MESSAGE         },
    {0x02, IR_2_KEY_MESSAGE         },
    {0x03, IR_3_KEY_MESSAGE         },
    {0x04, IR_4_KEY_MESSAGE         },
    {0x05, IR_5_KEY_MESSAGE         },
    {0x06, IR_6_KEY_MESSAGE         },
    {0x07, IR_7_KEY_MESSAGE         },
    {0x08, IR_8_KEY_MESSAGE         },
    {0x09, IR_9_KEY_MESSAGE         },
#elif(_REMOTE_CONTROLLER == _IR_PENGJIA)
    {0x10, _POWER_KEY_MESSAGE    },
    {0x4c, _SOURCE_IR_MESSAGE      },
    {0x1e, _MENU_KEY_MESSAGE        },
    {0x54, _OK_IR_MESSAGE},
    {0x4d, _RIGHT_IR_MESSAGE       },
    {0x05, _LEFT_IR_MESSAGE        },
    {0x04, _UP_KEY_MESSAGE          },
    {0x0e, _DOWN_KEY_MESSAGE        },
    {0x1a, _EXIT_KEY_MESSAGE        },

    {0x06, _MUTE_KEY_MESSAGE        },

    {0x12, IR_0_KEY_MESSAGE         },
    {0x09, IR_1_KEY_MESSAGE         },
    {0x1d, IR_2_KEY_MESSAGE         },
    {0x1f, IR_3_KEY_MESSAGE         },
    {0x0d, IR_4_KEY_MESSAGE         },
    {0x19, IR_5_KEY_MESSAGE         },
    {0x1b, IR_6_KEY_MESSAGE         },
    {0x11, IR_7_KEY_MESSAGE         },
    {0x15, IR_8_KEY_MESSAGE         },
    {0x17, IR_9_KEY_MESSAGE         },
#elif(_REMOTE_CONTROLLER == _IR_PJ_JAP)
    {0x57, _POWER_KEY_MESSAGE    },
    {0x14, _SOURCE_IR_MESSAGE      },
    //{0x05, _MENU_KEY_MESSAGE        },
    {0x41, _MENU_KEY_MESSAGE        },
    {0x07, _OK_IR_MESSAGE},
    {0x40, _RIGHT_IR_MESSAGE       },
    {0x47, _LEFT_IR_MESSAGE        },
    {0x06, _UP_KEY_MESSAGE          },
    {0x44, _DOWN_KEY_MESSAGE        },
    {0x1e, _EXIT_KEY_MESSAGE        },
    {0x52, _MUTE_KEY_MESSAGE        },
    //{0x41, _ISDB_RECALL_MESSAGE},
    {0x05, _ISDB_MENU_MESSAGE},
    {0x5e, _ISDB_SUB_MESSAGE},
	{0x08, _ISDB_LIST_MESSAGE},
	{0x01,_ISDB_SEARCH_MESSAGE},
	{0x45,_ISDB_AUDIO_MESSAGE},
	{0x0a,_ISDB_EPG_MESSAGE},
	{0x0f,_ISDB_KEY_100_MESSAGE},
	
    {0x1a, _CHANNEL_INC_MESSAGE		},
    {0x53, _CHANNEL_DEC_MESSAGE		},
    {0x03, _VOLUME_INC_MESSAGE		},
    {0x02, _VOLUME_DEC_MESSAGE		},

    {0x50, IR_0_KEY_MESSAGE         },
    {0x09, IR_1_KEY_MESSAGE         },
    {0x1d, IR_2_KEY_MESSAGE         },
    {0x1f, IR_3_KEY_MESSAGE         },
    {0x19, IR_4_KEY_MESSAGE         },
    {0x1b, IR_5_KEY_MESSAGE         },
    {0x11, IR_6_KEY_MESSAGE         },
    {0x17, IR_7_KEY_MESSAGE         },
    {0x12, IR_8_KEY_MESSAGE         },
    {0x59, IR_9_KEY_MESSAGE         },
    {0x0e, _ISDB_KEY_11_MESSAGE},
    {0x46, _ISDB_KEY_12_MESSAGE}
#elif(_REMOTE_CONTROLLER == _IR_PJ_JAP_2)
    {0x5D, _POWER_KEY_MESSAGE    },
    {0x5F, _SOURCE_IR_MESSAGE      },
    //{0x05, _MENU_KEY_MESSAGE        },
    {0x09, _MENU_KEY_MESSAGE        },
    {0x48, _OK_IR_MESSAGE},
    {0x4b, _RIGHT_KEY_MESSAGE       },
    {0x49, _LEFT_KEY_MESSAGE        },
    {0x4c, _UP_KEY_MESSAGE          },
    {0x4a, _DOWN_KEY_MESSAGE        },
    {0x5e, _EXIT_KEY_MESSAGE        },
    {0x15, _MUTE_KEY_MESSAGE        },
    //{0x41, _ISDB_RECALL_MESSAGE},
    {0x08, _ISDB_MENU_MESSAGE},
    {0x14, _ISDB_SUB_MESSAGE},
	{0x4D, _ISDB_LIST_MESSAGE},
	{0x1f,_ISDB_SEARCH_MESSAGE},
	//{0x45,_ISDB_AUDIO_MESSAGE},
	{0x49,_ISDB_EPG_MESSAGE},
	//{0x0f,_ISDB_KEY_100_MESSAGE},
	
    //{0x1a, _CHANNEL_INC_MESSAGE		},
    //{0x53, _CHANNEL_DEC_MESSAGE		},
    //{0x03, _VOLUME_INC_MESSAGE		},
    //{0x02, _VOLUME_DEC_MESSAGE		},

    {0x4b, IR_0_KEY_MESSAGE         },
    {0x18, IR_1_KEY_MESSAGE         },
    {0x19, IR_2_KEY_MESSAGE         },
    {0x1a, IR_3_KEY_MESSAGE         },
    {0x1c, IR_4_KEY_MESSAGE         },
    {0x1d, IR_5_KEY_MESSAGE         },
    {0x1e, IR_6_KEY_MESSAGE         },
    {0x46, IR_7_KEY_MESSAGE         },
    {0x47, IR_8_KEY_MESSAGE         },
    {0x48, IR_9_KEY_MESSAGE         },
    //{0x0e, _ISDB_KEY_11_MESSAGE},
    //{0x46, _ISDB_KEY_12_MESSAGE}    
#elif(_REMOTE_CONTROLLER == _IR_SAVV)

    {0x00, _POWER_KEY_MESSAGE   },
    {0x19, _SOURCE_KEY_MESSAGE   },	//46
    {0x1D, _DIMMER_KEY_MESSAGE 	},
  //  {0x19, _MENU_KEY_MESSAGE	}, 
    {0x1B, _DISPLAY_RATIO_IR_MESSAGE },
    {0x11, _MUTE_KEY_MESSAGE     },
    {0x0A, _DVD_DISPLAY_MESSAGE},
    {0x0B, _DVD_REPEAT_MESSAGE},
    {0x03, _VOLUME_DEC_MESSAGE	}, 
    {0x08, _VOLUME_INC_MESSAGE	},
    {0x0F, _DVD_FREW_MESSAGE},
	{0x51, _DVD_FFWD_MESSAGE},
	{0x18, _DVD_PREV_MESSAGE},
	{0x44, _DVD_NEXT_MESSAGE},
	{0x47,_DVD_PLAY_PAUSE_MESSAGE},
    {0x12, _UP_KEY_MESSAGE       },
    {0x07,_DVD_STOP_MESSAGE},
    {0x50, _LEFT_IR_MESSAGE        },
    {0x55, _OK_IR_MESSAGE			},
    {0x48, _RIGHT_IR_MESSAGE       },
    {0x1F, _EXIT_KEY_MESSAGE		},
    {0x06, _DOWN_KEY_MESSAGE        }, 
	{0x5e,_DVD_GOTO_MESSAGE},
	{0x40,_DVD_SETUP_MESSAGE},
	{0x42,_DVD_ZOOM_MESSAGE},
	#if CONFIG_TU300
	{0x15,_ISDB_LIST_MESSAGE},
	{0x4A,_ISDB_EPG_MESSAGE},
	#endif
    {0x01, IR_1_KEY_MESSAGE      },
    {0x02, IR_2_KEY_MESSAGE      },
    {0x41, IR_3_KEY_MESSAGE      },
    {0x5c,_DVD_SET_A_MESSAGE},
    {0x05, IR_4_KEY_MESSAGE      },
    {0x45, IR_5_KEY_MESSAGE      },
    {0x57, IR_6_KEY_MESSAGE      },
    {0x59,_DVD_ANGLE_MESSAGE},
    {0x09, IR_7_KEY_MESSAGE      },
    {0x52, IR_8_KEY_MESSAGE      },
    {0x53, IR_9_KEY_MESSAGE      },
    {0x1e,_DVD_ROOT_MENU_MESSAGE},
    {0x17, IR_0_KEY_MESSAGE      },
    {0x0e,_DVD_SUBTITLE_MESSAGE},
    {0x1a,_DVD_AUDIO_MESSAGE},
    {0x10,_DVD_TITLE_MESSAGE},
    
	//{0x5d,_DVD_SUBTITLE_MESSAGE},//pbc
  	//{0x33, _CHANNEL_INC_MESSAGE},
   // {0x34, _CHANNEL_DEC_MESSAGE},	
	//{0x04,_DVD_SLOW_MESSAGE},	
	//{0x0e,_DVD_ROOT_MENU_MESSAGE},
	{0x4C,_DVD_EJECT_MESSAGE},
	{0x4D,_DVD_USB_SD_MESSAGE},
	{0xAA,_MENU_KEY_MESSAGE},//NO USE
	{0xBB,_MENU_KEY_MESSAGE},//NO USE


#endif
};
#endif //#ifdef _IRKEY_



#endif // _AP_IRKEY_H


/*======================== End of File =======================================*/
/**
*
* @}
*/

