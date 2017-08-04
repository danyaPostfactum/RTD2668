/*===========================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2009
 * All rights reserved.
 * ========================================================== */

/*==================== File Description ===================== */
/**
 * @file
 * 	This file is osd engine declaration file for osd related functions.
 *
 * @author 	$Author: cf.tsai $
 * @date 	$Date: 2009-03-16 $
 * @version $Revision: 10 $
 */

/**
 * @addtogroup osd
 * @{
 */

#ifndef _OSDCORE_H
#define _OSDCORE_H
/*===================== Module Dependency  ================== */
#include "sparrow.h"
#include "osdobjectdef.h"
#include "panel\panelapi.h"

/*========================== Types ========================== */
typedef struct
{
    MenuItemSet*	pMenuItemSet;
    MenuItem* pMenuItem;
    UINT8	ucMainMenu;
    UINT8	ucSubMenu;
} LayerStack;


/*======================== Definitions ====================== */
#define _LOGOL_ENABLE                            1
#define _SLIDER_FONT_ADDR				112
#define _SLIDER_2BIT_FS					0x04  //(_SLIDER_FONT_ADDR/2)
#define _2BITICON_FONT_ADDR                   0xFA
#define _2BITICON_FS                   0
#define _2BITOFFSET_ADDR                   0x80
#define _MAINICON_FONT_ADDR				(_SLIDER_FONT_ADDR+24)		// 136
#define _MAINICON_FS					(_MAINICON_FONT_ADDR/2 - (_SLIDER_FONT_ADDR/2))
#define _MAINICON_4BIT_ADDR				(_MAINICON_FONT_ADDR+76)	// 212
#define _MAINICON_4BIT_FS				(_MAINICON_4BIT_ADDR/4)
#define _FRAME_2BIT_PATCH_ADDR			(_SLIDER_FONT_ADDR+24)	// 284
#define _FRAME_PATCH_FS					(_MAINICON_FONT_ADDR/2 - (_SLIDER_FONT_ADDR/2))

#define _SIGN_ADDR                   0xD8
#define FntSIGN_NUM                   0x06
#define FntVolume_StartAddr				0x78
#define FntMute_StartAddr				0x78

#define FntLangaugeName_StartAddr		0x60
#define FntLangaugeName_NUM		0x18

#define FntKor_StartAddr				0x80
#define FntKor_NUM		0x57

#define FntRUN_StartAddr				0x80
#define FntRUN_NUM		0x21
#define FntCHI_S_StartAddr		0x80
#define FntCHI_COLORPAGE_NUM		0x20
#define FntCHI_ADJUSTPAGE_NUM		0x1A
#define FntCHI_AUDIOPAGE_NUM		0x0E
#define FntCHI_FUNPAGE_NUM		0x20
#define FntCHI_TVPAGE_NUM		0x24

#define FntCHI_OTHERPAGE_NUM   0x1C
#define OTHER_PG   0x0F

#define FntJAPAN_S_StartAddr		0x80
#define FntJAPAN_COLORPAGE_NUM		0x4A
#define FntJAPAN_FUNPAGE_NUM		0x46
#define FntJAPAN_TVPAGE_NUM		0x3a


#define _SUBMENU_ACTIVE					(_BIT7)
#define MAINMENU_SET_SUB_ACTIVE(o)		(o)|=_SUBMENU_ACTIVE
#define MAINMENU_CLR_SUB_ACTIVE(o)		(o)&=~_SUBMENU_ACTIVE
#define MAINMENU_GET_SUB_ACTIVE(o)		((o)&_SUBMENU_ACTIVE)
#define MAINMENU_GET_ITEM_ID(_idd)		((_idd)&0x7f)

#define _OSD_AUDIO_BALANCE_MAX	20
#define _OSD_AUDIO_BALANCE_MIN	0
#define _OSD_AUDIO_BASS_MAX	10
#define _OSD_AUDIO_BASS_MIN	0
#define _OSD_AUDIO_TREBLE_MAX	10
#define _OSD_AUDIO_TREBLE_MIN	0

//---- language mask---------------------
#define ENGLISH_EN                  1//Tim 0322
#define LNG_CHI_EN                  1
#define LNG_FRA_EN                  1
#define LNG_DEU_EN                  0
#define LNG_SPAIN_EN               	1
#define LNG_ITA_EN                  0
#define LNG_KOREN_EN                1
#define LNG_PORTUGUESE_EN       	0
#define LNG_JAPAN_EN                0
#define LNG_FINISH_EN               0
#define LNG_DANISH_EN               0
#define LNG_SWEDISH_EN             	0
#define LNG_POLISH_EN               0
#define LNG_RUSSIAN_EN              1
//---- language type define--------------
#define ENGLISH                     0
#define LNG_CHI_S                   1
#define LNG_FRA                     2
#define LNG_DEU                     3
#define LNG_SPAIN                    4
#define LNG_ITA                     5
#define LNG_KOREN                    6
#define LNG_PORTUGUESE                     7
#define LNG_JAPAN                     8
#define LNG_FINISH                     9
#define LNG_DANISH                     10
#define LNG_SWEDISH                   11
#define LNG_POLISH                     12
#define LNG_RUSSIAN                      13
/**
 * OSD menu position.
 */

 #define _OSD_X_CENTER	((Get_DISP_DEN_END_HPOS() - Get_DISP_DEN_STA_HPOS())>>1)
#define _OSD_Y_CENTER	(Get_DISP_ACT_V_LEN()>>1)
#define _OSD_X_WIDTH	((Get_DISP_DEN_END_HPOS() - Get_DISP_DEN_STA_HPOS()))
#define _OSD_Y_HEIGHT	(Get_DISP_ACT_V_LEN())

#define _OSD_MAINMENU_H_POS				((((_OSD_X_WIDTH-(39*12+20))/2))>>2)
#define _OSD_MAINMENU_V_POS				((_OSD_Y_HEIGHT-(18*18+10))/2)
#define _OSD_Logo_H_POS				(((((_OSD_X_WIDTH-((16*12+20)*2))/2)>>1))>>2)
#define _OSD_Logo_V_POS				(((_OSD_Y_HEIGHT-((4*18+10)*2))/2)>>1)
#define _OSD_EQ_MENU_H_POS				_OSD_MAINMENU_H_POS
#define _OSD_EQ_MENU_V_POS				_OSD_MAINMENU_V_POS
#define _OSD_CHN_EDIT_MENU_H_POS			_OSD_MAINMENU_H_POS
#define _OSD_CHN_EDIT_MENU_V_POS			_OSD_MAINMENU_V_POS
#define _OSD_POPON_SLIDER_H_POS			(_OSD_MAINMENU_H_POS+10)
#define _OSD_POPON_SLIDER_V_POS			(((_OSD_Y_CENTER - 15)>>2)<<2)
#define _OSD_VOLUME_BAR_H_POS			((((_OSD_X_WIDTH-(7+27*12))/2))>>2)//(_OSD_MAINMENU_H_POS+10)
#define _OSD_VOLUME_BAR_V_POS				(((Get_DISP_ACT_V_LEN()-240)>>2)<<2)
#define _OSD_MUTE_H_POS					((_OSD_X_CENTER -(25*6 ))>>2)//80  //Tim 0325
#define _OSD_MUTE_V_POS					15
#define _OSD_LISTMENU_H_POS				(15)//(60)
#define _OSD_LISTMENU_V_POS				(Get_DISP_DEN_STA_VPOS()+10)
#define _OSD_INFO_BOX_H_POS				(60)
#define _OSD_INFO_BOX_V_POS				(40)
#define _OSD_MODE_BOX_H_POS				((_OSD_X_CENTER - (16*6 + 50))>>2)
#define _OSD_MODE_BOX_V_POS				(((Get_DISP_ACT_V_LEN()-240)>>2)<<2)
    //#define _OSD_MESSAGE_BOX_H_POS            ((_OSD_X_CENTER - (16*6 + 50))>>2)
    //#define _OSD_MESSAGE_BOX_V_POS            (((_OSD_Y_CENTER - 8)>>2)<<2)
#define _OSD_MESSAGE_BOX_H_POS			((((_OSD_X_WIDTH-((27 * 12) + 20))/2))>>2)
#define _OSD_MESSAGE_BOX_V_POS			((_OSD_Y_HEIGHT-(0+(7 * 18 - 4)))/2)

#define _PM_COLORTMP_ROW_START                      9
#define _PM_COLORTMP_ROW_HEIGHT                     5
#define _PM_COLORTMP_COL_START                      3
#define _OSD_TVCHANNEL                             ((_OSD_X_CENTER -(25*6 ))>>2)
/**
 * OSD UI flags.
 */
#define _OSD_FLAG_FACTORY_MODE			(_BIT0)
#define _OSD_FLAG_RTK_FACTORY_MODE		(_BIT1)
#define _OSD_FLAG_SCART_PLUG_IN			(_BIT2)
#define _OSD_FLAG_SCART_MANUAL_MODE		(_BIT3)
#define _OSD_FLAG_TV_TABLE_SCAN			(_BIT4)
#define _OSD_FLAG_TV_MANUAL_SCAN		(_BIT5)
#define _OSD_FLAG_SOURCE_INFO			(_BIT6)
#define _OSD_FLAG_MUTE					(_BIT7)

#define GET_OSD_SOURCE_INFO()		(ucOsdCoreFlag&_OSD_FLAG_SOURCE_INFO)
#define SET_OSD_SOURCE_INFO()		ucOsdCoreFlag |= _OSD_FLAG_SOURCE_INFO
#define CLR_OSD_SOURCE_INFO()		ucOsdCoreFlag &= ~_OSD_FLAG_SOURCE_INFO

/**
 * Factory mode macro.
 */
#define GET_FACTORY_MODE_MENU()		(ucOsdCoreFlag&_OSD_FLAG_FACTORY_MODE)
#define SET_FACTORY_MODE_MENU()		ucOsdCoreFlag|=_OSD_FLAG_FACTORY_MODE
#define CLR_FACTORY_MODE_MENU()		ucOsdCoreFlag&=~_OSD_FLAG_FACTORY_MODE

#define GET_RTK_FACTORY_MODE()		(ucOsdCoreFlag&_OSD_FLAG_RTK_FACTORY_MODE)
#define SET_RTK_FACTORY_MODE()		ucOsdCoreFlag|=_OSD_FLAG_RTK_FACTORY_MODE
#define CLR_RTK_FACTORY_MODE()		ucOsdCoreFlag&=~_OSD_FLAG_RTK_FACTORY_MODE

/**
 * SCART mode macro.
 */
#define GET_SCART_PLUG_IN()				(ucOsdCoreFlag&_OSD_FLAG_SCART_PLUG_IN)
#define SET_SCART_PLUG_IN()				ucOsdCoreFlag |= _OSD_FLAG_SCART_PLUG_IN
#define CLR_SCART_PLUG_IN()				ucOsdCoreFlag &= ~_OSD_FLAG_SCART_PLUG_IN

#define GET_SCART_MANUAL_MODE()			(ucOsdCoreFlag&_OSD_FLAG_SCART_MANUAL_MODE)
#define SET_SCART_MANUAL_MODE()			ucOsdCoreFlag |= _OSD_FLAG_SCART_MANUAL_MODE
#define CLR_SCART_MANUAL_MODE()			ucOsdCoreFlag &= ~_OSD_FLAG_SCART_MANUAL_MODE

/**
 * TV table scan mode
 */
#define GET_TV_TABLE_SCAN()				(ucOsdCoreFlag&_OSD_FLAG_TV_TABLE_SCAN)
#define SET_TV_TABLE_SCAN()				ucOsdCoreFlag |= _OSD_FLAG_TV_TABLE_SCAN
#define CLR_TV_TABLE_SCAN()				ucOsdCoreFlag &= ~_OSD_FLAG_TV_TABLE_SCAN

#define GET_TV_MANUAL_SCAN()			(ucOsdCoreFlag&_OSD_FLAG_TV_MANUAL_SCAN)
#define SET_TV_MANUAL_SCAN()			ucOsdCoreFlag |= _OSD_FLAG_TV_MANUAL_SCAN
#define CLR_TV_MANUAL_SCAN()			ucOsdCoreFlag &= ~_OSD_FLAG_TV_MANUAL_SCAN

/**
 * Audio mute flag
 */
#define GET_AUDIO_MUTE()				(ucOsdCoreFlag&_OSD_FLAG_MUTE)
#define SET_AUDIO_MUTE()				ucOsdCoreFlag |= _OSD_FLAG_MUTE
#define CLR_AUDIO_MUTE()				ucOsdCoreFlag &= ~_OSD_FLAG_MUTE


/* ***** OSD Core flag 2 ***** */
/**
 * Channel manual scan flag
 */
#define _OSD_FLAG2_MANUAL_SCAN_DIRECTION	(_BIT0)
//#define _OSD_TTX_MODE						(_BIT1)
#define _OSD_FLAG2_MAINMENU_ON				(_BIT2)
#define _OSD_FLAG2_SCART2_PLUG_IN			(_BIT3)
#define _OSD_FLAG2_SLIDER_ON					(_BIT4)

#define _MANUAL_SCAN_INC					(0x00)
#define _MANUAL_SCAN_DEC					(0X01)
#define GET_TV_MANUAL_SCAN_DIRECTION()			(ucOsdCoreFlag2&_OSD_FLAG2_MANUAL_SCAN_DIRECTION)
#define SET_TV_MANUAL_SCAN_DIRECTION(_d)		ucOsdCoreFlag2 = (ucOsdCoreFlag2&~_OSD_FLAG2_MANUAL_SCAN_DIRECTION) | ((_d)&_BIT0)


#define GET_MAINMENU_ON()					(ucOsdCoreFlag2&_OSD_FLAG2_MAINMENU_ON)
#define SET_MAINMENU_ON()					ucOsdCoreFlag2 |= _OSD_FLAG2_MAINMENU_ON
#define CLR_MAINMENU_ON()					ucOsdCoreFlag2 &= ~_OSD_FLAG2_MAINMENU_ON

#define GET_SCART2_PLUG_IN()				(ucOsdCoreFlag2&_OSD_FLAG2_SCART2_PLUG_IN)
#define SET_SCART2_PLUG_IN()				ucOsdCoreFlag2 |= _OSD_FLAG2_SCART2_PLUG_IN
#define CLR_SCART2_PLUG_IN()				ucOsdCoreFlag2 &= ~_OSD_FLAG2_SCART2_PLUG_IN

#define GET_SLIDER_ON()					(ucOsdCoreFlag2&_OSD_FLAG2_SLIDER_ON)
#define SET_SLIDER_ON()					ucOsdCoreFlag2 |= _OSD_FLAG2_SLIDER_ON
#define CLR_SLIDER_ON()					ucOsdCoreFlag2 &= ~_OSD_FLAG2_SLIDER_ON

#define ROW(x)                          x

#define COL(x)                          x

#define HEIGHT(x)                       x

#define WIDTH(x)                       x

#define LENGTH(x)                       x
#define SHOWINDEX_TO_LINE(y)				        (4 + y * 2)
#define SHOWINDEX_TO_LINE1(y)				        (1 + y * 2)

/**
 * Audio EQ bar OSD settings.
 */
#define _OSD_EQ_MAX_VALUE			(12)
#define _OSD_EQ_MID_VALUE			(6)
#define _OSD_EQ_MIN_VALUE			(0)
#define _INFOBOX_HINT_BAR		(_BIT0)
#define _MODEBOX_TYPE			(_BIT1)
#define _MESSAGEBOX_TYPE		(_BIT2)


/**
 * OSD state.
 */
typedef enum
{
    _IDLE_STATE = 0,
    _BOOT_STATE,
    _CHN_INPUT_STATE,
    _CHN_EDIT_STATE,
    _CHN_NAME_EDIT_STATE,
    _CHN_KEYPUSH_STATE,
    _VOLUME_STATE,
    _MAINMENU_STATE,
    _SLIDER_STATE,
    _LISTITEM_STATE,
    _AUTO_SCAN_STATE,
    _MANUAL_SCAN_STATE,
    _NOSIGNAL_STATE,
    _LIST_MENU_STATE,

	_HOTKEY_STATE,
	_SHOW_SLEEP_STATE,
    _MODE_BOX_STATE,
    _AUTO_ADJUST_STATE,
    _FACTORY_MENU_STATE,
    _RTK_FACTORY_MODE_STATE,

    _PICTURE_MODE_STATE,
    _SOURCESELECT_STATE,

    _EQ_CONTROL_STATE,

    _AGING_STATE,
    _AUTOWB_STATE,
    _MAGIC_PICTURE_STATE,		// CF 20071106 added for Magic Picture Demo
    _SHOW_INFO_STATE,
    _AUTO_PWROFF_CONFIRM_STATE,
    _SCREENSAVER_STATE,
    _SHOW_NO_SIGNAL_STATE,
    _SHOW_NO_SUPPORT_STATE,
    _SHOW_SLEEP_PWROFF_CONFIRM_STATE,	//leeqeejee. sleep confirm move from State_Handler, prevent OSD display issue.  20080530
    _SLEEP_PWROFF_CONFIRM_STATE,	// Added by Libin. 2007/12/18

    _MTS_STATE,
#ifdef CONFIG_TTX_SUPPORT
    _TELETEXT_STATE,
    _SUBTITLE_STATE,
#endif
    _PWRSAVE_SELECT_STATE,
#ifdef CONFIG_VBI_CC_SUPPORT
    _VBI_SHOW_STATE,
#endif
 #if _LOGOL_ENABLE
     _LOGOL_STATE,
#endif

} OsdSateType;
// Get osd state
#define COsdCoreGetState() ucOsdState

typedef enum
{
    INFORM_NO_SIGNAL = 0,
    INFORM_NO_SUPPORT,
    INFORM_ADJUSTING,
    INFORM_NO_CABLE,
    INFORM_AUTO_COLOR,
    INFORM_NO_TT,
} INFORM_MESSAGE;
#define CLRCOLOR_SUBMENU_SELLINE1(y)		        	OSDLine(y, COL(2), LENGTH(6), 0x00, _WRITE_BYTE1)
#define DRAWCOLOR_SUBMENU_SELLINE1(y)			        OSDLine(y, COL(2), LENGTH(6), 0x0D, _WRITE_BYTE1)

#define SETCOLOR_SUBMENU_SELLINE(y)			        OSDLine(y, COL(7), LENGTH(31), 0x20, _WRITE_BYTE2)
void COsdCoreDispMessageBox(UINT8 ucOpt);


void fw_osdcore_Initialze(void);
void fw_osdcore_Handler(void);

void ap_osdcore_init_data(void);

void COsdCoreDispatchMsg(void);

void COsdCoreInitMainMenu(void);
void CShowMode(UINT8 ucRedraw,UINT8 ucRow,UINT8 ucCol);

void COsdCoreEnableMainMenu(void);
void COsdCoreExitOsd(void);
void CFacInit(void);
void COsdCoreRtkFacDrawItems(UINT8 ucSelectedItem);
void COsdCoreRtkFacUpdateSliderItem(UINT8 ucItemId);
void COsdCoreRtkFacUpdateBracketItem(UINT8 ucItemId);
void COsdCoreRtkFacUpdateListItem(UINT8 ucItemId);

void COsdCoreUpdateChannelScanSlider(UINT8 ucCh);//, UINT16 uwFreq);
//void COsdCoreUpdateTableScanSlider(UINT8 ucCh, UINT8 ucPercent);
void COsdCoreUpdateTableScanSlider(UINT8 ucCh, UINT8 ucProgress,UINT32 uwFreq);//Tim 0325
void COsdCoreDrawInfoBoxFrame(UINT8* pTitleStr, UINT8 ucOpt);
void COsdCoreFacItemEventProc(void);

void COsdCoreInitChannelScanState(void);
void COsdCoreInitTableScanState(void);
void COsdCoreInitChannelEditState(void);
#if DISABLE_OPTTION
void CGetSleepString();
#endif
void CCloseCCStatus();



//Tim 0407
void InitOsdFrame(void);

//void COsdCoreInitAudioEqControlMenu(void);

UINT8 COsdCoreDrawSliderNumber(UINT8 ucRow, UINT8 ucCol, UINT8 ucColor, ItemSlider* pSlider);
void COsdCoreDispLogo(void);
UINT8* COsdCoreGetBracketString(void);
UINT8 COsdCoreListItemValueToIdx(UINT8 ucValue);
UINT8 COsdCoreSliderOperation(ItemSlider *pSlider);
UINT16 COsdCoreBracketOperation(ItemBracket *pBracket);

UINT8 ap_osdcore_irkey_to_num(UINT16 message);
void COsdRecallVGAValue(void);

void COsdCoreAutoAdjustEventProc(void);
void COsdCoreInitAutoAdjustState(UINT8 msg);
void COsdCtrlSetEQ(UINT8 index,UINT8 value);
void InitLanguagefont(void);
void UpdateLanguagePage(void);
void COsdCoreDrawLogo(UINT8 ucDouble);
bit Repeatkey_en(UINT8 key_message);
void COsdCoreShowDisplayRatioType(UINT8 cc);
void COsdCoreShowDisplayDimmerType(UINT8 DisplayType);
extern void COsdCtrlSetYCB(void);


#ifdef SUPPORT_FIVE_KEY
bit COsd_LRkey2UDkey(void);
void COsdCore_Chn_keypush_EventProc(void);
#endif

UINT8 COsdGetNextSource(ucsource);

#ifndef _OSDCORE_C
extern UINT8 xdata ucOsdState;
extern UINT8 xdata ucOsdCoreFlag;
extern UINT8 xdata ucOsdCoreFlag2;
extern UINT8 DisplayRatioFlag;
extern UINT8 xdata ucShowSleepFlag;
extern bit bloadfont;
//extern UINT8 xdata ucMainIcon;
#endif



//=============================================================================================
/**
 *
 * @}
 */

#endif // #ifndef _OSDCORE_HCORE_H
