/*===========================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2009
 * All rights reserved.
 * ========================================================== */

/*==================== File Description ===================== */
/**
 * @file
 * 	This file is osd engine implement file for osd related functions.
 *
 * @author 	$Author: cf.tsai $
 * @date 	$Date: 2009/05/08  $
 * @version 	$Revision: 100 $
 */

/**
 * @addtogroup osd
 * @{
 */

#define _OSDCORE_C
/*===================== Module Dependency  ================== */
#include "autoconf.h"
#include "sparrow.h"
#include "rosprintf.h"
#include "message.h"
#include "osd_sys_msg.h"
#include "mode_customer.h"
#include "eepromdefaultcustomer.h"
#include "ui_struct.h"
#include "timer_event.h"
#include "timer_event_ui_table.h"
#include "flow_scaler.h"
#include "flow_struct.h"
#include "flow_audio.h"
#include "modestate.h"
#include "video/video.h"
#include "source.h"
#include "ui_ctrl.h"
#include "osdregdef.h"
#include "osdobjectdef.h"
#include "osdfunction.h"
#include "osdparameter.h"
#include "osdfont.h"
#include "osdstring.h"
#include "osdcore.h"
#include "osdflow.h"
#include "osdcontrol.h"
#include "osdcorefactory.h"
#include "rt_storage.h"
#include "storage_system.h"
#include "tuner\tuner.h"
#include "ui_ctrl.h"
#include "ap_rtirkey.h"
#include "tv.h"
#include <sparrow/imem.h>	// Provides: drv_imem4k_Start()
#include "reg52.h"
#include <hdmi\hdmiFun.h>
#include "power.h"
#include "spi_dma.h"
#include "pcb_customer.h"
#include "pcb\pcb_utility.h"
#ifdef CONFIG_TTX_SUPPORT
#include "ttx/ttx.h"
#include "ttx_disp.h"
#endif
#ifdef CONFIG_VBI_CC_SUPPORT
#include <isr_utility.h>    /* Provides: SYS_ISR_VBIPPR */
#include <vbi/slr.h>
BOOL xdata enable_cc_verify = FALSE;
#include "osdvbi.h"
#include "closecaption.h"
#endif
#include "timer_event_sys_table.h"
#include "flow_conf_struct.h"
#if 1 //black_backcolor
#include "flow_display.h"
#endif
#include "ifd\ifd_sparrow.h"
#include "irrc\irrc.h"
#include "sys_info.h"

/*========================== Types ========================== */


/*======================== Definitions ====================== */
#define _MAXLAYER_NUM					5
#define _ITEM_AREA_LEN					180
#define _OSD_DISPLAY_TIMEOUT			SEC(3) //ms //Hardy20091218

/*======================= Private Types ===================== */
//typedef struct
//{
//	MenuItemSet*	pMenuItemSet;
//	MenuItem* pMenuItem;
//	UINT8	ucMainMenu;
//	UINT8	ucSubMenu;
//} LayerStack;

/*========================== Variables ====================== */
UINT8 xdata ucOsdCoreFlag;
UINT8 xdata ucOsdCoreFlag2;
UINT16 xdata uwOsdCoreModeKey;
#ifdef SUPPORT_FIVE_KEY
UINT8 xdata ucOsditem_choose; 
#endif
bit bloadfont;

ItemPosition xdata stItemPos;
ItemObject* xdata pItemObj;
ItemList* xdata pListItem;
UINT8 xdata ucListSelectedItem;
UINT8 xdata ucPage;
UINT8 xdata ucNoshowItem = 0;
UINT8 xdata ucFirstInList = 0;
UINT8 xdata ucOsdCoreBuf[8];
UINT8 xdata scartOrgSrcIdx;
UINT8 xdata ucChnNum;
UINT8 xdata ucCopyChn;
UINT8 xdata ucShowSleepFlag = 0;
extern	UINT8 xdata ucSleepType;
extern UINT8 xdata sleep_10ms;
extern UINT8 xdata sleep_sec;
extern UINT8 xdata sleep_min;
UINT8 g_dimmer=0;
UINT8 BriConIdx_temp;
UINT8 g_power_flag=0;
UINT8 g_power_dtvflag=0;



//UINT8 xdata ucMainIcon;
StructUserDataChannelSettingsType xdata stCopyChannelData;

UINT8 xdata EQBar_Value[7];

UINT8 xdata menu_item_col[8];

UINT8 xdata ucOsdState;
volatile UINT8 xdata ucLayerIndex;
volatile LayerStack xdata stLayerStack[_MAXLAYER_NUM];
volatile UINT16 xdata fUI_Msg;
volatile UINT16 xdata uwUI_PreMsg;
//volatile bit flow_task_ready = 0;
//volatile bit flow_go_ready = 0;
UINT8  code ucTeTest = 23;
UINT8 DisplayRatioFlag = 0;

#ifdef CONFIG_TTX_SUPPORT
TTX_Handle xdata m_hTtx;
static UINT8 xdata ttx_noTT = 1; // 0: TT OK, 1: initial TT OK, 2: TT NG, 3: initial TT NG
TTX_STD_Page_AddrWithInputSubCode_t ttx_select_subpage;
#endif
extern UINT8 xdata IsActivePowerSavingEvent;
extern UINT32 xdata profile_time;
UINT16 xdata usLANGUAGE_MASK;

/*======================== function ====================== */
void COsdCoreRtkFacMenuEventProc(void);

void COsdChnInputTimeoutEvent(void);

void COsdCoreEnterIdeProc(void);

static void ap_osdcore_boot_state_proc(void);

// ** OSD information display API
static void ap_osdcore_display_state_init(void);

// ** TV source UI related API
static void  ap_osdcore_jump_prev_chn(void);

// ** Display Ratio related API
static void ap_osdcore_set_display_ratio(void);


static UINT8 ap_osdcore_hotkey_handler(void);
static void ap_osdcore_poweroff_proc(void);

void COsdCoreIdleEventProc(void);
void COsdCoreVolumeEventProc(void);
void COsdCoreMainMenuEventProc(void);
void COsdCoreMenuDirectionKeyProc(void);

void COsdCoreDrawSlider(UINT8 ucRow, UINT8 ucCol, UINT8 ucLen, ItemSlider* pSlider,UINT8 color);

void COsdCoreDispMenuTitle(MenuItem *pMenuItem);
void LoadCHI_S_Font(UINT8 ucPage);
void LoadJAPAN_Font(UINT8 ucPage);
void LoadLanguage_FONT(UINT8 ucPage);

void COsdCoreDrawMainMenuFrame(UINT8 ucMainID);
void COsdCoreDrawMenuItems(MenuItem* pMenuItem, UINT8 ucItemID, BOOL bRefresh);
void COsdCoreFallBackMenu(void);
UINT8 GetShowIndex(UINT8 ucSubItem);
void COsdCoreSetMainActive(UINT8 id, UINT8 active);
void COsdCoreMainIconAnime(UINT8 id);
//void COsdCoreSetMainHighlight(UINT8 ucActive);


#define _REFRESH_ALL		(0x03)
#define _REFRESH_MAIN		(0x02)
#define _REFRESH_SUB		(0x01)
#define _REFRESH_NONE		(0x00)
#define _NORMAL_COLOR          (0x40)
#define _SELECT_COLOR          (0x80)

void COsdCoreMoveHighlight(UINT8 ucMain, UINT8 ucSub, UINT8 bRefresh);

#define _MENU_HINT_MAINICON			(0)
#define _MENU_HINT_SLIDER			(1)
#define _MENU_HINT_LIST				(2)
#define _MENU_HINT_LIST_SELECT		(3)
#define _MENU_HINT_SUBPAGE			(4)
#define _MENU_HINT_BRACKET			(5)
void COsdCoreDispMenuHint(UINT8 opt);

void COsdCoreUpdateSliderItem(UINT8 ucItemId,UINT8 color);

void COsdCoreUpdateBracketItem(UINT8 ucItemId,UINT8 ucFontColor);
void COsdCoreUpdateListItem(UINT8 ucItemId,UINT8 ucFontColor);
void COsdCoreInitListPage(void);
//void COsdCoreSetListHighlight(void);
void COsdCoreUpdateListPage(void);
void COsdCoreListItemEventProc(void);

#ifdef _VIDEO_TV_SUPPORT
void COsdCoreChannelScanEventProc(void);
#endif
void COsdCoreManualScanEventProc(void);


#define _OPT_CHANNEL_SCAN_MODE		(_BIT1)
#define _OPT_VOLUME_ADJ_MODE		(_BIT0)
#define IS_VOLUME_MODE(_m)			((_m)&_BIT0)
#define IS_CHANNEL_SCAN_MODE(_m)	((_m)&_BIT1)
void COsdCoreInitPoponSlider(UINT8 opt);
void COsdCoreUpdatePoponSlider(void);
void COsdCorePoponSliderEventProc(void);
void COsdCorePoponSliderNextItem(UINT8 ucOpt);

//void COsdCoreUpdateChannelScanSlider(UINT8 ucCh, UINT16 uwFreq);
//void COsdCoreUpdateTableScanSlider(UINT8 ucCh, UINT8 ucProgress);

void COsdCoreInitListMenu(ItemList *pList);
void COsdCoreListMenuEventProc(void);
//void COsdCoreSetListMenuHighlight(UINT8 idx);

void CosdCoreDrawModeFrame(UINT8* pTitleStr, UINT8 ucOpt);
void COsdCoreDrawInfoBoxFrame(UINT8* pTitleStr, UINT8 ucOpt);
void CosdCoreDrawModeFrame_key(UINT8* pTitleStr, UINT8 ucOpt);

void COSdCoreDispResolution(UINT8 ucRow,UINT8 ucCol_Pos);
void COSdCoreScartDispResolution(void);
void COSdCoreVGADispResolution(UINT8 ucRow,UINT8 ucCol_Pos);
#ifdef CONFIG_ENABLE_HDMI
void COsdCoreDispHDMIMode(UINT8 ucRow,UINT8 ucCol_Pos);
#endif

#define _OPT_REDRAW				(_BIT0)
void COsdCoreDispModeBox(UINT8 ucOpt);
void COsdCoreModeBoxEventProc(void);


void COsdCoreUpdateChannelEditOsd(UINT8 ucItemID);
void COsdCoreSetChannelEditItemHighlight(UINT8 ucItemID);
void COsdCoreChnEditUpdateChnName(UINT8 ucID);
void COsdCoreLoadChannelName(UINT8 ucChn);
void COsdCoreChnEditEventProc(void);

#ifdef _VIDEO_TV_SUPPORT
void COsdCoreChnInputBox(UINT8 ucChn, UINT8 ucRedraw);
void COsdCoreChnInputEventProc(void);
#endif
void COsdCoreSourceInfo_key(UINT8 ucRedraw);
void COsdCoreSourceInfo(UINT8 ucRedraw);
#ifdef _VIDEO_TV_SUPPORT
void COsdCoreShowTVChannel(UINT8 ch);
#endif
void CshowTVMode(UINT8 ch,UINT8 row,UINT8 col,UINT8 ucColor);
void COsdCoreDrawMuteIcon(bit bMute);

void COsdCoreEqControlEventProc(void);
//void DrawSelectLine(UINT8 id);
void ClearSelectLine(void);

bit xIsChnCharValid(UINT8 ucCh);

#ifdef CONFIG_TTX_SUPPORT
void COsdCoreTeleTextEventProc(void);
void COsdCoreSubTitleEventProc(void);
static void xResetPageAddr(void);
#endif // end of CONFIG_TTX_SUPPORT

#ifdef CONFIG_VBI_CC_SUPPORT

BOOL Get_CC_Status();
void Set_CC_Status(BOOL on);
void Enable_CC_Input();
void Disable_CC_Input();

#endif
extern void * memset(void * s, int c, size_t count);

#ifdef CONFIG_VBI_CC_SUPPORT
static void COsdCoreVbiProc(void);
#endif
/*=========================== Functions ===================== */

#if 0
char * strcat (char *r, const char *s)
{
    char *t;
    const char *cs;

    t = r;
    while (*t++) {}

    t--;
    cs = s;

    while ((*t++ = *cs++)) {}
    return r;
}
#endif
extern UINT8 xdata fAutoscan1;
#if 0
void fw_osdcore_InitProfile(void)
{
    profile_time = 0;
}

void fw_osdcore_PrintProfile(void)
{
    RTD_Log(LOGGER_INFO, "\nProfile time = %d ms \n", profile_time*10);
}
#endif
void DrawAdjustMenuItemText(MenuItem* pMenuItem, UINT8 ucItemID,UINT8 ucFontColor)
{
    UINT8 j;
    UINT8 ucCnt,ucIndex;
    UINT8 ucWidth;

    ucPage = ucItemID / 6;
    ucCnt = 7;
    ucIndex = GetShowIndex(ucItemID);
    j = ucIndex%6;
    // Draw item string
    ucCnt += COsdFxLoadText(4+j*2, ucCnt, 0, ucFontColor, COsdCtrlGetLanguage(), pMenuItem->pItemList[ucItemID].pString);
    ucWidth = COsdFxCalTextWidth(COsdCtrlGetLanguage(),pMenuItem->pItemList[ucItemID].pString);
    if ( ucWidth < 168)
        ucCnt += COsdFxLoadBlankCmd(4+j*2, ucCnt, 168-ucWidth, _CP_BLACK);
    menu_item_col[j] = ucCnt;

    if (pMenuItem->pItemList[ucItemID].ucType == _SLIDER_TYPE)
    {
        COsdCoreUpdateSliderItem(ucItemID,_NORMAL_COLOR);
    }
    else if (pMenuItem->pItemList[ucItemID].ucType == _BRACKET_TYPE)
    {
        COsdCoreUpdateBracketItem(ucItemID,_NORMAL_COLOR);
    }
    else if (pMenuItem->pItemList[ucItemID].ucType == _BRACKET1_TYPE)
    {
        COsdCoreUpdateBracketItem(ucItemID,_NORMAL_COLOR);
    }
    else if (pMenuItem->pItemList[ucItemID].ucType == _LIST_TYPE)
    {
        COsdCoreUpdateListItem(ucItemID,_NORMAL_COLOR);
    }
    else if (pMenuItem->pItemList[ucItemID].ucType == _IMMEDIATE_TYPE || pMenuItem->pItemList[ucItemID].ucType == _SUBPAGE_TYPE)
    {
        ucCnt += 1; // 蛤 slider bar 癸霍
        ucCnt += COsdFxLoadBlankCmd(4+j*2, ucCnt, _ITEM_AREA_LEN, _CP_BLACK);
        COsdFxWrite1BitFont(4+j*2, ucCnt, _R_ARROW_,_NORMAL_COLOR);
    }
    else
    {
        // no support item?
    }
}
/**
 * OsdCore Initialize function.
 * @param None.
 * @return None.
 *
 */
void DrawSelectMenuItem(MenuItem* pMenuItem, UINT8 ucItemID,UINT8 ucFontColor)
{
    UINT8 j;
    UINT8 ucCnt,ucIndex;
    UINT8 ucWidth;

    ucPage = ucItemID / 6;
    ucCnt = 7;
    ucIndex = GetShowIndex(ucItemID);
    j = ucIndex%6;
    // Draw item string
    ucCnt += COsdFxLoadText(4+j*2, ucCnt, 0, ucFontColor, COsdCtrlGetLanguage(), pMenuItem->pItemList[ucItemID].pString);
    ucWidth = COsdFxCalTextWidth(COsdCtrlGetLanguage(),pMenuItem->pItemList[ucItemID].pString);
    if ( ucWidth < 168)
        ucCnt += COsdFxLoadBlankCmd(4+j*2, ucCnt, 168-ucWidth, _CP_BLACK);
    menu_item_col[j] = ucCnt;

    if (pMenuItem->pItemList[ucItemID].ucType == _SLIDER_TYPE)
    {
        COsdCoreUpdateSliderItem(ucItemID,ucFontColor);
    }
    else if (pMenuItem->pItemList[ucItemID].ucType == _BRACKET_TYPE)
    {
        COsdCoreUpdateBracketItem(ucItemID,ucFontColor);
    }
    else if (pMenuItem->pItemList[ucItemID].ucType == _BRACKET1_TYPE)
    {
        COsdCoreUpdateBracketItem(ucItemID,ucFontColor);
    }
    else if (pMenuItem->pItemList[ucItemID].ucType == _LIST_TYPE)
    {
        COsdCoreUpdateListItem(ucItemID,ucFontColor);
    }
    else if (pMenuItem->pItemList[ucItemID].ucType == _IMMEDIATE_TYPE || pMenuItem->pItemList[ucItemID].ucType == _SUBPAGE_TYPE)
    {
        ucCnt += 1; // 蛤 slider bar 癸霍
        ucCnt += COsdFxLoadBlankCmd(4+j*2, ucCnt, _ITEM_AREA_LEN, _CP_BLACK);
        COsdFxWrite1BitFont(4+j*2, ucCnt, _R_ARROW_,ucFontColor);
    }
    else
    {
        // no support item?
    }
}
/*void DrawSelectLine(UINT8 id)
{
    UINT8 row;
    id = MAINMENU_GET_ITEM_ID(id);
    id = GetShowIndex(id);
    row =(id%6)*2+5;
    OSDLine(row, 7, 31, 0x0D ,_WRITE_BYTE1);
}*/
void ClearSelectLine(void)
{
    UINT8 row,ucsub;
    ucsub = MAINMENU_GET_ITEM_ID(stLayerStack[ucLayerIndex].ucSubMenu);
    ucsub = GetShowIndex(ucsub);
    row =(ucsub%6)*2+5;
    OSDLine(row, 7, 31, 0x00, _WRITE_BYTE1);
}
void fw_osdcore_Initialze(void)
{
    //UINT8 getVal;
    //UINT32 getVal32bits = 0;

    uwOsdCoreModeKey = 0;
    fUI_Msg = 0;
    uwUI_PreMsg = 0;

#ifdef CONFIG_TTX_SUPPORT
    m_hTtx = NULL;
#endif
#ifdef SUPPORT_FIVE_KEY
    ucOsditem_choose = _FALSE;
#endif
    flow_osdctrl_system_Init();

    ucLayerIndex = 0;
    stLayerStack[ucLayerIndex].ucMainMenu = 0;
    stLayerStack[ucLayerIndex].ucSubMenu = 0;

    LOAD_STORAGE_DATA(StructSystemDataType, InputSource, stUISignalSource);
	#if CONFIG_ENABLE_USB
	if(stUISignalSource==_SOURCE_INDEX_USB)
	{
		stUISignalSource=_SOURCE_INDEX_YPBPR;
	}
	#endif
    if (stUISignalSource  > _MAX_SOURCE_INDEX)
    {
        SYNC_STORAGE_DEFAULT_DATA(StructSystemDataType);
        stUISignalSource = stSystemDataDefault.InputSource;
    }

    LOAD_STORAGE_DATA(StructSystemDataType, UartPath, ucUartPath);
    //RTD_Log(LOGGER_INFO, "Get Input source %x from storage\n", (UINT32)stUISignalSource);
#ifdef CONFIG_VBI_CC_SUPPORT
    LOAD_STORAGE_DATA(StructSystemDataType, CcSystem, Ccsel);
#endif
    //RTD_Log(LOGGER_INFO, "CONFIG_VBI_CC_SUPPORT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    scartOrgSrcIdx = stUISignalSource;

//->w	stLayerStack[ucLayerIndex].pMenuItemSet = tMainMenuSet[ucMenuSelect];
    ucOsdState = _IDLE_STATE;
    ucPage = 0;
    ucChnNum = 0;
    ucOsdCoreFlag = 0;
    ucOsdCoreFlag2 = 0;
    ucCopyChn = 1;
    #ifdef _VIDEO_TV_SUPPORT
    ui_get_chn_data(ui_get_channel_idx(1), &stCopyChannelData);
    #endif
    usLANGUAGE_MASK  =  ( ENGLISH_EN   << ENGLISH ) |
                        ( LNG_CHI_EN   << LNG_CHI_S ) |
                        ( LNG_FRA_EN   << LNG_FRA ) |
                        ( LNG_DEU_EN   << LNG_DEU ) |
                        ( LNG_SPAIN_EN   << LNG_SPAIN ) |
                        ( LNG_ITA_EN << LNG_ITA ) |
                        ( LNG_KOREN_EN   << LNG_KOREN) |
                        ( LNG_PORTUGUESE_EN   << LNG_PORTUGUESE ) |
                        ( LNG_JAPAN_EN   << LNG_JAPAN)|
                        ( LNG_FINISH_EN << LNG_FINISH ) |
                        ( LNG_DANISH_EN   << LNG_DANISH ) |
                        ( LNG_SWEDISH_EN   << LNG_SWEDISH ) |
                        ( LNG_POLISH_EN   << LNG_POLISH )|
                         ( LNG_RUSSIAN_EN<< LNG_RUSSIAN);
;
    
    ap_osdcore_init_data();
    //COsdCoreInitMainMenu();
    //RTD_Log(LOGGER_INFO, "UI osd initial complete ...\n");
}


/**
 * To initial OSD row map and load font data.
 * @return none.
 */
void ap_osdcore_init_data(void)
{
    // set share memory mode
    ap_osdfx_set_share_memory();

    // turn on OSD circuit and set share memory
    ap_osdfx_power_on(_ON);

    // Load OSD palette
    ap_osdfx_load_palette();

    // init SRAM
    ap_osdfx_map_setting(_OSD_MAP0);
    ap_osdfx_font_shadow_color_set(0);
    ap_osdfx_set_2bit_font_offset(_2BITICON_FONT_ADDR/2);
	
#if 1//(!_LOGOL_ENABLE)
    {
        ap_osdfx_load_vlc_font(tFONT_GLOBAL, 0x00, 0x60);
        ap_osdfx_load_vlc_font(tfont_2biticon, _2BITICON_FONT_ADDR, 76);
        // Load Font
    }
#endif
    ap_osdfx_load_vlc_font(tFont_OTHERSIGN, _SIGN_ADDR, FntSIGN_NUM);
    // Load 2bits Main Icon Font

    //ucMainIcon = 0;
    ap_osdfx_load_vlc_font(FntLangaugeName, FntLangaugeName_StartAddr, FntLangaugeName_NUM);
    InitLanguagefont();
    COsdFxClearText(0, 0,18, 46);

    COsdFxBlending(GET_OSD_BLENDING());

    bloadfont = 0;
}

void ap_osdcore_num_irkey_converter(void)
{
    UINT16 ret;

    switch (fUI_Msg)
    {
    case IR_0_KEY_MESSAGE:
        ret = _0_KEY_MESSAGE;
        break;
    case IR_1_KEY_MESSAGE:
        ret = _1_KEY_MESSAGE;
        break;
    case IR_2_KEY_MESSAGE:
        ret = _2_KEY_MESSAGE;
        break;
    case IR_3_KEY_MESSAGE:
        ret = _3_KEY_MESSAGE;
        break;
    case IR_4_KEY_MESSAGE:
        ret = _4_KEY_MESSAGE;
        break;
    case IR_5_KEY_MESSAGE:
        ret = _5_KEY_MESSAGE;
        break;
    case IR_6_KEY_MESSAGE:
        ret = _6_KEY_MESSAGE;
        break;
    case IR_7_KEY_MESSAGE:
        ret = _7_KEY_MESSAGE;
        break;
    case IR_8_KEY_MESSAGE:
        ret = _8_KEY_MESSAGE;
        break;
    case IR_9_KEY_MESSAGE:
        ret = _9_KEY_MESSAGE;
        break;
//	case _DASH_KEY_MESSAGE:
//		ret = _100_KEY_MESSAGE;
//		break;

    default:
        ret = 0;
        break;
    }

    if ( ret != 0)
        fUI_Msg = ret;
}
extern void CTemp(void);

void CNoUse(void)
{
	CTemp();
    fw_IR_message_push(0);
    #ifndef CONFIG_TUNER_SILICON
    drv_ifd_eq_en(0);
    drv_ifd_eq_en_get_info();
    #endif
    IRRC_Command_Mapping(0,0);
    fw_GPIO_inputerrupt_Init();
    fw_GPIO_Interrupt_Proc();
    RtdCtrl_ReadDS(0);
    RtdCtrl_MaskByteDS(0,0,0);
    RtdCtrl_isr_ReadDS(0);
    RtdCtrl_isr_MaskByteDS(0,0,0);
    RtdCtrl_isr_MaskDS(0);
    bReadID();
    drv_pcb_set_gpio_pin_isr(0,0);

    CShowTimer();
    drv_tuner_GeDiverRatio(0);
    drv_tuner_CheckBand(0);
    drv_tuner_GetPictureInterFreq();
    fwif_tv_auto_SECAML_funtion(0);//Edit SVN53
    fwif_tv_auto_SECAMLA_funtion(0);//Edit SVN53
    RT610_Standby(0);//Edit SVN53

}

UINT8 xdata ttse = 0;

/**
 * Osd handler.
 */
// void fw_ir_message_clear(void);
void fw_osdcore_Handler(void)
{
    static UINT32 xdata tmp32;
    static UINT8 xdata FlagPowesaving=0;
	UINT8  nouse = 0;
	UINT8 cur_source;
	//CTemp();
	cur_source=cur_source;
	if(nouse)
	{
		CNoUse();
	}
	
    fUI_Msg =  fw_ui_message_pop();

    if (fUI_Msg == _POWER_KEY_MESSAGE)
    {
        if (!GET_FIRST_POWER_SWITCH())
        {
            SET_POWERSWITCH();
//			fw_ir_message_clear();
        }
        RTD_Log(LOGGER_INFO, "Accept power key msg in UI\n");
        return;
    }

    if (fUI_Msg == _SLEEP_TIMEOUT_MESSAGE)
    {
        SET_POWERSWITCH();
        RTD_Log(LOGGER_INFO, "Sleep time out\n");
        return;
    }

#ifdef CONFIG_SCART_AUTO_SWITCH
    if ( !tvAutoScanFlag)  // SCART FS detection
    {
        UINT8 scart1_FS, scart2_FS;

        scart1_FS = ui_SCART_GetVal(GPIO_SCART1_FUNCTION_SELECT);
        scart2_FS = ui_SCART_GetVal(GPIO_SCART2_FUNCTION_SELECT);
        //RTD_Log(LOGGER_INFO, "SCART1 FS = %x  SCART2 FS = %x\n", (UINT32)scart1_FS, (UINT32)scart2_FS);

//      SCART1 FS high -> low,  SCART2 FS high -> low
//      SCART1 FS low,          SCART2 FS low
//      SCART1 FS high -> low,  SCART2 FS low
//      SCART1 FS low,          SCART2 FS high -> low

//      SCART1 FS high,         SCART2 FS high
//      SCART1 FS low -> high,  SCART2 FS low -> high
//      SCART1 FS low -> high,  SCART2 FS high
//      SCART1 FS high,         SCART2 FS low-> high

//      SCART1 FS low -> high,  SCART2 FS low
//      SCART1 FS high,         SCART2 FS low
//      SCART1 FS low,          SCART2 FS low -> high
//      SCART1 FS low,          SCART2 FS high

//      SCART1 FS high -> low,  SCART2 FS low -> high
//      SCART1 FS high -> low,  SCART2 FS high
//      SCART1 FS low -> high,  SCART2 FS high -> low
//      SCART1 FS high,         SCART2 FS high -> low

        if ( !GET_SCART_MANUAL_MODE() )
        {
            if ( scart1_FS == _SCART_FS_LOW && scart2_FS == _SCART_FS_LOW)
            {
                bit bSrcSwitchBack = FALSE;
                if ( GET_SCART_PLUG_IN() )
                {
                    if ((info->state == _MODE_STATE_SEARCH)&& (info->FailureCount < 2))
                    {
                        bSrcSwitchBack = FALSE;
                    }
                    else
                    {
                        if (drv_video_vdc_CheckHVisLocked())
                        {
                            bSrcSwitchBack = FALSE;
                        }
                        else
                        {
                            CLR_SCART_PLUG_IN();
                            bSrcSwitchBack = TRUE;
                        }
                    }
                }
                if ( GET_SCART2_PLUG_IN() )
                {
                    CLR_SCART2_PLUG_IN();
                    bSrcSwitchBack = TRUE;
                }

                if ( bSrcSwitchBack)
                {
                    ap_osdctrl_scart_change_source(scartOrgSrcIdx);
                }
            }
            else
            {
                if ( scart1_FS != _SCART_FS_LOW && scart2_FS != _SCART_FS_LOW)
                {
                    if ( GET_SCART_PLUG_IN() == 0 && GET_SCART2_PLUG_IN() == 0)
                    {
                        SET_SCART_PLUG_IN();
                        SET_SCART2_PLUG_IN();
                        scartOrgSrcIdx = stUISignalSource;
                        ap_osdctrl_scart_change_source(_SOURCE_INDEX_SCART_0);
                    }
                    else if ( GET_SCART_PLUG_IN() == 0)   // SCART2 FS already high
                    {
                        SET_SCART_PLUG_IN();
                        ap_osdctrl_scart_change_source(_SOURCE_INDEX_SCART_0);
                    }
                    else if ( GET_SCART2_PLUG_IN() == 0)   // SCART1 FS already high
                    {
                        SET_SCART2_PLUG_IN();
                        ap_osdctrl_scart_change_source(_SOURCE_INDEX_SCART_1);
                    }
                }
                else if ( scart1_FS != _SCART_FS_LOW && GET_SCART_PLUG_IN() == 0)  	// SCART1 FS high
                {
                    SET_SCART_PLUG_IN();
                    if ( GET_SCART2_PLUG_IN() == 0)
                        scartOrgSrcIdx = stUISignalSource;
                    ap_osdctrl_scart_change_source(_SOURCE_INDEX_SCART_0);
                }
                else if ( scart2_FS != _SCART_FS_LOW && GET_SCART2_PLUG_IN() == 0)   // SCART2 FS high
                {
                    SET_SCART2_PLUG_IN();
                    if ( GET_SCART_PLUG_IN() == 0)
                        scartOrgSrcIdx = stUISignalSource;
                    ap_osdctrl_scart_change_source(_SOURCE_INDEX_SCART_1);
                }
                else if ( scart1_FS == _SCART_FS_LOW && GET_SCART_PLUG_IN() != 0)   // SCART1 FS low
                {
                    CLR_SCART_PLUG_IN();
                    ap_osdctrl_scart_change_source(_SOURCE_INDEX_SCART_1);	// change to SCART2
                }
                else if ( scart2_FS == _SCART_FS_LOW && GET_SCART2_PLUG_IN() != 0)   // SCART2 FS low
                {
                    CLR_SCART2_PLUG_IN();
                    ap_osdctrl_scart_change_source(_SOURCE_INDEX_SCART_0);	// change to SCART1
                }
            }
        }
        else  	// user change source
        {
            if ( scart1_FS == _SCART_FS_LOW)
            {
                if ( GET_SCART_PLUG_IN() )
                {
                    CLR_SCART_PLUG_IN();
                    CLR_SCART_MANUAL_MODE();
                }
            }
            if ( scart2_FS == _SCART_FS_LOW)
            {
                if ( GET_SCART2_PLUG_IN() )
                {
                    CLR_SCART2_PLUG_IN();
                    CLR_SCART_MANUAL_MODE();
                }
            }
        }
    }
#endif
	//RTD_Log(LOGGER_LEVEL,"\n Tim ucSleepType=%d,ucShowSleepFlag = %d \n",(UINT32)ucSleepType,(UINT32)ucShowSleepFlag);

    // No do if no message
    if (fUI_Msg == _FL2UI_NONE_MESSAGE)
    {
        if (flow_UsePowerSaving())
        {
            //(IsActivePowerSavingEvent==0x01) (info->state != _MODE_STATE_ACTIVE)
            if (((info->state != _MODE_STATE_ACTIVE)||(GET_INPUTSOURCE_TYPE()==_SOURCE_TV))&&(ucOsdState == _IDLE_STATE))
            {
                if (FlagPowesaving==0x01)
                {
                    FlagPowesaving=0;

                    if (GET_INPUTSOURCE_TYPE()==_SOURCE_TV)
                    {
                        if (!drv_video_vdc_CheckHVisLocked())
                            IsActivePowerSavingEvent =0;
                    }
                    else
                    {
                        //RTD_Log(LOGGER_INFO, "\n++++++++Enablepower_saving", 0);
                        fw_timer_event_ReactiveTimerEvent(SEC(stflowbData.nPowerSaving_Parameter),timerevent_table_power_saving_event);
                    }
                }
            }

            if ((GET_INPUTSOURCE_TYPE()==_SOURCE_TV)&&(ucOsdState != _IDLE_STATE)||((ucOsdState == _SHOW_NO_SUPPORT_STATE)))
            {
                if ((FlagPowesaving==0)&&(IsActivePowerSavingEvent==1))
                {
                   // RTD_Log(LOGGER_INFO, "\n++++++++Cancelpower_saving_tv001", 0);
                    FlagPowesaving =1;
                    fw_timer_event_CancelTimerEvent(timerevent_table_power_saving_event);
                }
            }
        }
        //RTD_Log(LOGGER_LEVEL,"\n bCheckCCStatus = %d,bCcStartFlag = %d\n",(UINT32)bCheckCCStatus,(UINT32)bCcStartFlag);

#ifdef CONFIG_VBI_CC_SUPPORT
        if ((GET_CLOSEDCAPTION())&&(ucOsdState == _IDLE_STATE)
                &&((GET_INPUTSOURCE_TYPE()==_SOURCE_CVBS)
                   ||(GET_INPUTSOURCE_TYPE()==_SOURCE_SV)
                   ||(GET_INPUTSOURCE_TYPE()==_SOURCE_TV)
                   ||(GET_INPUTSOURCE_TYPE()==_SOURCE_YPBPR)))
        {
            //Enable CC only when  in active state and NTSC signal
            if (info->state != _MODE_STATE_ACTIVE)//first in active_state
            {
                if (Get_CC_Status() == TRUE)
                {
                    Disable_CC_Input();
                    Set_CC_Status(FALSE);
                }
                return ;
            }

            if (GET_CURRENT_MODE() != _MODE_480I) // NTSC
            {
                if (Get_CC_Status() == TRUE)
                {
                    Disable_CC_Input();
                    Set_CC_Status(FALSE);
                }
                return;
            }

            if (GET_INPUTSOURCE_TYPE()==_SOURCE_TV)
            {
                if (!drv_video_vdc_CheckHVisLocked())//tv and hv unlock
                {
                    if (Get_CC_Status() == TRUE)
                    {
                        Disable_CC_Input();
                        Set_CC_Status(FALSE);
                    }
                    return;
                }
                else
                {
                    if(bCheckCCStatus)
                    {
                    bCheckCCStatus = 0;
                    RTD_Log(LOGGER_INFO, "\n+++Enable_CC_Input", 0);
                    CLR_OSDVBI_INITIAL();
                    
                    CCcInitial();
                    COsdVbiInitial();
                    ucOsdState =_VBI_SHOW_STATE;
                    bCcStartFlag = _TRUE;
                    Set_CC_Status(TRUE);
                    Enable_CC_Input();
                    }

                }
            }



        }
#endif
		
		#ifdef CONFIG_VBI_CC_SUPPORT
        if((ttse != sleep_sec)&&(0x00 != ucSleepType)  && (ucOsdState == _IDLE_STATE) && !GET_CLOSEDCAPTION())
         {  CShowTimer();
			//ttse = 1;
		}
		#endif
		
        return;
    }
    //RTD_Log(LOGGER_LEVEL,"\n bCcStartFlag111 = %d\n",(UINT32)bCcStartFlag);

#ifdef CONFIG_VBI_CC_SUPPORT
    if ((GET_INPUTSOURCE_TYPE()==_SOURCE_VGA)	||(GET_INPUTSOURCE_TYPE()==_SOURCE_HDMI))
    {
        if (Get_CC_Status())
        {

            Disable_CC_Input();
            Set_CC_Status(FALSE);
        }
    }
#endif

    fw_ui_message_clearallType(fUI_Msg);
#ifdef CONFIG_TTX_SUPPORT
    if (ucOsdState != _TELETEXT_STATE)
#endif
#ifdef _VIDEO_TV_SUPPORT
        if (GET_INPUTSOURCE_TYPE() == _SOURCE_TV)
        ap_osdcore_num_irkey_converter();
#endif
    if ( fUI_Msg != _REPEAT_KEY_MESSAGE)
        uwUI_PreMsg = fUI_Msg;

    if (tmp32 != fUI_Msg)
    {
        tmp32 = fUI_Msg;
        RTD_Log(LOGGER_INFO, "UI msg = %x\n", (UINT32)tmp32);
    }
//->w	RTD_Log(LOGGER_INFO, "UI msg = %x\n", (UINT32)fUI_Msg);
    if (fUI_Msg == 0x18)
    {
        if (fAutoscan1)
            fAutoscan1 = 0;
        else
            fAutoscan1 = 1;
        tmp32 = fAutoscan1;
        RTD_Log(LOGGER_INFO, "fAutoscan1 = %x\n", tmp32);
    }
#if 0
    LOAD_STORAGE_DATA(StructOsdUserDataType, AgingMode, getVal);
    if ((fUI_Msg == _POWER_KEY_MESSAGE) && (getVal  != _AGING_MODE_ON))
    {
        ap_osdcore_poweroff_proc();
        fw_flow_message_push(_FL2FL_POWER_KEY_MESSAGE);
    }
#endif

#ifdef CONFIG_VBI_CC_SUPPORT
    bCcStartFlag = 0;
#endif

    if (flow_UsePowerSaving())//(GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
    {
    RTD_Log(LOGGER_INFO,"flow_usepowersaving---------------%d\n",(UINT32)fUI_Msg);
        //(info->state != _MODE_STATE_ACTIVE) (IsActivePowerSavingEvent==0x01)
        if (((info->state != _MODE_STATE_ACTIVE)||(GET_INPUTSOURCE_TYPE()==_SOURCE_TV))
                &&(fUI_Msg!=_FL2UI_MAIN_NO_SIGNAL)&&(fUI_Msg!=_FL2UI_MAIN_NO_CABLE)
                &&(fUI_Msg!=_FL2UI_MAIN_NO_SUPPORT)&&(fUI_Msg!=_TTX_EXIT_MESSAGE)&&(fUI_Msg!=_TTX_SIGNAL_OK_MESSAGE) )
        {
            if (1)//(FlagPowesaving==0x00)
            {
                if ((GET_INPUTSOURCE_TYPE()!=_SOURCE_TV)||((GET_INPUTSOURCE_TYPE()==_SOURCE_TV)&&(IsActivePowerSavingEvent==1)))
                {
                    RTD_Log(LOGGER_INFO, "\n++++++++Cancelpower_saving", 0);
                    FlagPowesaving=0x01;
                    fw_timer_event_CancelTimerEvent(timerevent_table_power_saving_event);
                }
            }
        }
    }
    else
        FlagPowesaving=0;

    if ( ap_osdcore_hotkey_handler() )
        return;

	

    switch (ucOsdState)
    {
    case _BOOT_STATE:
        ap_osdcore_boot_state_proc();
        break;

    case _SHOW_INFO_STATE:
        if (fUI_Msg == _DISPLAY_KEY_MESSAGE
                //|| fUI_Msg == _MENU_KEY_MESSAGE
                || fUI_Msg == _EXIT_KEY_MESSAGE
                || fUI_Msg == _OSD_TIMEOUT_MESSAGE)
                {
                
                //RTD_Log(LOGGER_LEVEL,"\n _SHOW_INFO_STATE bCheckCCStatus=%d \n",(UINT32)bCheckCCStatus);
            COsdCoreExitOsd();}
        else if (fUI_Msg == _DISPLAY_MESSAGE)
        {
            //COsdCoreSourceInfo(0);
            fw_timer_event_ReactiveTimerEvent(SEC(2), timerevent_ui_table_OSD_timeout_event);
        }
        else if(fUI_Msg == _FL2UI_SHOW_LOGO) 
        {
        	COsdCoreDrawLogo(3);
			ucPowersavingdisablelogo = 0;
        }
        else if (!GET_FACTORY_MODE_MENU())
            COsdCoreDispatchMsg();

        break;

    case _SHOW_NO_SIGNAL_STATE:
        COsdCoreNoSignalEvent();
        break;
    case _SHOW_NO_SUPPORT_STATE:
       COsdCoreNoSupportlEvent();
        break;
        
#if _LOGOL_ENABLE//Tim 0322
     case _LOGOL_STATE:
		if (fUI_Msg == _OSD_TIMEOUT_MESSAGE)
			COsdCoreExitOsd();
		else if( fUI_Msg == _FL2UI_LOAD_MAINFONT)
		{
			COsdCoreExitOsd();
			ap_osdfx_load_vlc_font(tFONT_GLOBAL, 0x00, 0x60);
			ap_osdfx_set_2bit_font_offset(_2BITICON_FONT_ADDR/2);
			ap_osdfx_load_vlc_font(tfont_2biticon, _2BITICON_FONT_ADDR, 76);
			COsdFxClearText(0, 0,18, 46);
			bloadfont = 0;
		}
		else if(fUI_Msg == _SOURCE_KEY_MESSAGE || fUI_Msg == _SOURCE_IR_MESSAGE)
		{
#if(_SOURCE_LIST_TYPE == _SOURCE_LISTNOTMENU) 		
            cur_source = COsdCtrlGetSignalSource();
            COsdCtrlSetSignalSource(COsdGetNextSource(cur_source));
            //fw_ui_message_push(_DISPLAY_MESSAGE);
            #if _MELON_DEBUG
			//RTD_Log(LOGGER_INFO, "\n melon_zhang 2222222222222222222----------------------------------------------------------\n");
			#endif
            COsdCoreSourceInfo(_TRUE);
            fw_timer_event_CancelTimerEvent(timerevent_table_no_signal_event);
#else
            //if(COsdCtrlGetSignalSource() != _SOURCE_INDEX_HDMI0)//joker xie 判断HDMI
            
			ucOsdState = _LIST_MENU_STATE;
            COsdCoreInitListMenu(&tSourceSelectList);
			fw_timer_event_ReactiveTimerEvent(_OSD_DISPLAY_TIMEOUT/*ap_osdctrl_get_osd_timeout_ms()*/, timerevent_ui_table_OK_event);
#endif	    
		}		
		else if( fUI_Msg == _MENU_KEY_MESSAGE)
		{	
		    COsdCoreInitMainMenu();
		    COsdCoreEnableMainMenu();
		    fw_timer_event_CancelTimerEvent(timerevent_ui_table_OSD_timeout_event);
		    fw_timer_event_ReactiveTimerEvent(ap_osdctrl_get_osd_timeout_ms(), timerevent_ui_table_OSD_timeout_event);
		    //RTD_Log(LOGGER_INFO, "Timeout0 value0=%x, rel0=%x\n",(UINT32)ap_osdctrl_get_osd_timeout_ms(), (UINT32)rel);
		};
		
	break;
	#endif
	case _HOTKEY_STATE:
    case _IDLE_STATE:
    case _SHOW_SLEEP_STATE:
        COsdCoreIdleEventProc();
        break;

    case _MAINMENU_STATE:
        COsdCoreMainMenuEventProc();
        break;

    case _SLIDER_STATE:
        COsdCorePoponSliderEventProc();
        break;

    case _LISTITEM_STATE:
        COsdCoreListItemEventProc();
        break;

    case _VOLUME_STATE:
        COsdCoreVolumeEventProc();
        break;

#ifdef _VIDEO_TV_SUPPORT
    case _AUTO_SCAN_STATE:
        COsdCoreChannelScanEventProc();
        break;

//	case _WAITMANUALSCANEXIT_STATE:
//		COsdCoreWaitManualScanExitProc();
//		break;

    case _MANUAL_SCAN_STATE:
        COsdCoreManualScanEventProc();
        break;
#endif

    case _LIST_MENU_STATE:
        COsdCoreListMenuEventProc();
        break;

    case _MODE_BOX_STATE:
        COsdCoreModeBoxEventProc();
        break;
        
#ifdef _VIDEO_TV_SUPPORT
    case _CHN_INPUT_STATE:
        COsdCoreChnInputEventProc();
        break;
    case _CHN_KEYPUSH_STATE:
#ifdef SUPPORT_FIVE_KEY    
        COsdCore_Chn_keypush_EventProc();
#endif        
        break;
    #if defined(_REALTEK_OSD_USE)//Realtek OSD
    case _CHN_EDIT_STATE:
    case _CHN_NAME_EDIT_STATE:
        COsdCoreChnEditEventProc();
        break;
    #endif
#endif

    case _AUTO_ADJUST_STATE:
        COsdCoreAutoAdjustEventProc();
        break;

    case _FACTORY_MENU_STATE:
        COsdCoreFacMenuEventProc();
        break;

    case _RTK_FACTORY_MODE_STATE:
        COsdCoreRtkFacMenuEventProc();
        break;

    case _EQ_CONTROL_STATE:
        COsdCoreEqControlEventProc();
        break;
        /*
        	case _AUTO_PWROFF_CONFIRM_STATE:
        		COsdCoreAutoPwrOffEventProc();
        		break;

        	case _SHOW_SLEEP_PWROFF_CONFIRM_STATE:
        		COsdCoreExitOsd();
        		COsdFxShowPwrOffNote(1);
        		COsdCoreSetState(_SLEEP_PWROFF_CONFIRM_STATE);
        		fw_ui_message_push(_SLEEP_PWROFF_NOTIFY);
        		break;
        */
    case _SLEEP_PWROFF_CONFIRM_STATE:
        //COsdCoreSleepPwrOffEventProc();
        break;

    case _AGING_STATE:
        //CosdCoreAgingEventProc();//lyc add 2007-9-7 at this mode ,anything cmd can't been accept including power,except AGING CMD
        break;

    case _AUTOWB_STATE:
        //CosdCoreAutoWBEventProc();
        break;

    case _SCREENSAVER_STATE:
        //COsdFxSetFlashScreenEventProc();
        break;

#ifdef CONFIG_TTX_SUPPORT
    case _TELETEXT_STATE:
        COsdCoreTeleTextEventProc();
        break;

    case _SUBTITLE_STATE:
        COsdCoreSubTitleEventProc();
        break;
#endif // end of CONFIG_TTX_SUPPORT

#ifdef CONFIG_VBI_CC_SUPPORT
    case _VBI_SHOW_STATE:
        COsdCoreVbiProc();
        break;
#endif

    default:
        RTD_Log(LOGGER_DEBUG, "[OSD] Invalid osd state\n");
        break;
    }
}

UINT8 ap_osdcore_irkey_to_num(UINT16 message)
{
#if 0
    switch (message)
    {
    case IR_0_KEY_MESSAGE:
        return 0;
    case IR_1_KEY_MESSAGE:
        return 1;
    case IR_2_KEY_MESSAGE:
        return 2;
    case IR_3_KEY_MESSAGE:
        return 3;
    case IR_4_KEY_MESSAGE:
        return 4;
    case IR_5_KEY_MESSAGE:
        return 5;
    case IR_6_KEY_MESSAGE:
        return 6;
    case IR_7_KEY_MESSAGE:
        return 7;
    case IR_8_KEY_MESSAGE:
        return 8;
    case IR_9_KEY_MESSAGE:
        return 9;
    default:
        return 0;
    }
#else
    if ( message >= _0_KEY_MESSAGE && message <= _9_KEY_MESSAGE)
    {
        return (UINT8)(message - _0_KEY_MESSAGE);
    }

    return 0;
#endif
}

/**
 * Exit OSD.
 *
 */
void COsdCoreExitOsd(void)
{
	#ifdef CONFIG_VBI_CC_SUPPORT
	bCheckCCStatus = 2;//Tim 0328
	#endif
    CLR_SLIDER_ON();
    COsdFxDisableOsd();
    fw_timer_event_CancelTimerEvent(timerevent_ui_table_OSD_timeout_event);
    fw_timer_event_CancelTimerEvent(COsdCoreIconAnimationTimerEvent);

    flow_storage_WriteBackup2Flash();

    ucLayerIndex = 0;
    //stLayerStack[ucLayerIndex].ucMainMenu = 0;
    stLayerStack[ucLayerIndex].ucSubMenu = 0;
    ucOsdState = _IDLE_STATE;
    //COsdCoreInitMainMenu();
    CLR_MAINMENU_ON();
    ap_osdfx_map_setting(_OSD_MAP0);
    
	if(GET_AUDIO_MUTE()&&!ucShowSleepFlag)
	{
		#ifdef CONFIG_VBI_CC_SUPPORT
		if(!GET_CLOSEDCAPTION())
		#endif
		{
		COsdCoreDrawMuteIcon(TRUE);
		fw_timer_event_CancelTimerEvent(timerevent_ui_table_OSD_timeout_event);
		}
	}
	ucShowSleepFlag = 0;
	//if(ucSleepType)
		//ucShowSleepFlag = 1;
RTD_Log(LOGGER_INFO,"COSDCOREEXITOSD\N");
}


/**
 * To set main icon Active.
 * @return none.
 */
#if 0// defined(CONFIG_MAINICON_4BITS_ANIMATION)
UINT8* code tMainIcon4BitPt[] =
{
    tFont_MainIcon_01_4bits,
    tFont_MainIcon_02_4bits,
    tFont_MainIcon_03_4bits,
    tFont_MainIcon_04_4bits,
    tFont_MainIcon_05_4bits,
};
#endif
void COsdCoreSetMainActive(UINT8 id, UINT8 active)
{
    UINT8 row;
    UINT8 *tIcon;
    id = id % 6;
    if((GET_INPUTSOURCE_INDEX() == _SOURCE_INDEX_AV)||(GET_INPUTSOURCE_INDEX() == _SOURCE_INDEX_AV_1)||(GET_INPUTSOURCE_INDEX() == _SOURCE_INDEX_AV_2)||(GET_INPUTSOURCE_INDEX() == _SOURCE_INDEX_AV_3))
    {
	    if (active)
	        tIcon = Icons_Select_av[id];
	    else
	        tIcon = Icons_normale_av[id];
	}
	else if(GET_INPUTSOURCE_INDEX() == _SOURCE_INDEX_VGA)
	{
		if (active)
	        tIcon = Icons_Select_vga[id];
	    else
	        tIcon = Icons_normale_vga[id];
	}
	else
	{
		if (active)
	        tIcon = Icons_Select_hdmi[id];
	    else
	        tIcon = Icons_normale_hdmi[id];
	}
    row = 4+id*2;
    Draw2bit3x2Icon(tIcon,  row,  1);

}
/**
 *
 * @return none.
 */
void COsdCoreMainIconAnime(UINT8 id)
{
#if 1//defined(CONFIG_MAINICON_4BITS_ANIMATION)
    UINT8 j;
    UINT8 ucLut = 0;

    ucLut = (id == 3)? 1 : 0;
    WaitFor_DEN_STOP();
    for (j=0; j<3; j++)
        COsdFxWrite4BitChar(2+id*2, 1+j, _MAINICON_4BIT_FS+6+j, _CP_BLACK, ucLut);
    for (j=0; j<3; j++)
        COsdFxWrite4BitChar(3+id*2, 1+j, _MAINICON_4BIT_FS+6+j+3, _CP_BLACK, ucLut);

    //fw_timer_event_DelayXms(200);
    flow_timer_Reset_WatchDog();

    WaitFor_DEN_STOP();
    for (j=0; j<3; j++)
        COsdFxWrite4BitChar(2+id*2, 1+j, _MAINICON_4BIT_FS+12+j, _CP_BLACK, ucLut);
    for (j=0; j<3; j++)
        COsdFxWrite4BitChar(3+id*2, 1+j, _MAINICON_4BIT_FS+12+j+3, _CP_BLACK, ucLut);

    //fw_timer_event_DelayXms(200);
    flow_timer_Reset_WatchDog();

    WaitFor_DEN_STOP();
    for (j=0; j<3; j++)
        COsdFxWrite4BitChar(2+id*2, 1+j, _MAINICON_4BIT_FS+j, _CP_BLACK, ucLut);
    for (j=0; j<3; j++)
        COsdFxWrite4BitChar(3+id*2, 1+j, _MAINICON_4BIT_FS+j+3, _CP_BLACK, ucLut);
#else
    id = 0;
#endif
}

/**
 * To set highlight status of Main Icon.
 * @return none.
 */
/*
void COsdCoreSetMainHighlight(UINT8 ucActive)
{
   UINT8 ucRow;

   ucRow = 2+stLayerStack[ucLayerIndex].ucMainMenu*2;

   WaitFor_DEN_STOP();
   if ( ucActive)
   {
   	COsdFxChangeWindowColor(tMainIconHighlightWindowStyle.ucWindowID, _CP_PURE_CYAN_BLUE);
   	COsdFxChangeWindowColor(tMainIconHighlightWindowStyle2.ucWindowID, _CP_DARK_CYAN_BLUE);
   }
   else
   {
   	COsdFxChangeWindowColor(tMainIconHighlightWindowStyle.ucWindowID, _CP_SILVER);
   	COsdFxChangeWindowColor(tMainIconHighlightWindowStyle2.ucWindowID, _CP_GRAY);
   }
}
*/
/**
 * To display slider number.
 * @return character count of fonts.
 */
UINT8 COsdCoreDrawSliderNumber(UINT8 ucRow, UINT8 ucCol, UINT8 ucColor, ItemSlider* pSlider)
{
    UINT8 ucVal ;
    UINT8 ucMax, ucMin;

    ucVal = COsdCtrlMenuItemGetValue(pSlider->ucMenuItemID);

    if ( (pSlider->ucCtrl&_SLIDER_VBR_RANGE) != 0)
    {
        ucMax = COsdCtrlMenuItemGetMax(pSlider->ucMenuItemID);
        ucMin = COsdCtrlMenuItemGetMin(pSlider->ucMenuItemID);
    }
    else
    {
        ucMax = pSlider->ucMax;
        ucMin = pSlider->ucMin;
    }

    //090706 Willy added, if the menu item value is unreasonable, load to default
    if (ucVal > ucMax || ucVal < ucMin)
    {
        ucVal = COsdCtrlMenuItemGetDefault(pSlider->ucMenuItemID);
    }

//	if ( ucVal > ucMax) { // default value is invalid ??
//		ucVal = ucMax;
//	}
//	if ( ucVal < ucMin)
//		ucVal = ucMin;

    COsdFxShowNumber(ucRow, ucCol, ucVal, 0x23, (ucColor >> 4));
    return 4;
}

/**
 * To draw slider item on menu OSD.
 * @param
 * @return none.
 */
void COsdCoreDrawSlider(UINT8 ucRow, UINT8 ucCol, UINT8 ucLen, ItemSlider* pSlider,UINT8 color)
{
    UINT8 j;
    UINT8 thumbPos, thumbFinePos;
    UINT8 ucVal;
    UINT16 uwTmp;
    UINT8  sliderlength;
    sliderlength = ucLen;
    ucVal = COsdCtrlMenuItemGetValue(pSlider->ucMenuItemID);
    //090706 Willy added, if the menu item value is unreasonable, load to default
    if (ucVal > pSlider->ucMax)
    {
        ucVal = COsdCtrlMenuItemGetDefault(pSlider->ucMenuItemID);
    }
    uwTmp = sliderlength * 6 - 1; // max step
    if ( (pSlider->ucCtrl & _SLIDER_VBR_RANGE) != 0)
    {
        thumbPos = ucVal - COsdCtrlMenuItemGetMin(pSlider->ucMenuItemID);
        uwTmp = uwTmp * thumbPos;
        thumbPos = COsdCtrlMenuItemGetMax(pSlider->ucMenuItemID) - COsdCtrlMenuItemGetMin(pSlider->ucMenuItemID);
    }
    else
    {
        thumbPos = ucVal - pSlider->ucMin;
        uwTmp = uwTmp * thumbPos;
        thumbPos = pSlider->ucMax - pSlider->ucMin;
    }
    if ( thumbPos > 0)
        uwTmp = uwTmp / thumbPos;
    thumbPos = (UINT8)uwTmp;

    thumbFinePos = thumbPos % 6; // mode 6
    thumbPos = thumbPos / 6;
    ap_osdfx_calc_start_addr(ucRow, ucCol, _WRITE_BYTEALL);
    COsdFxWrite1BitFont_value(_SLIDER_2BIT_FS,color);
    for (j=0; j<sliderlength; j++)
    {
        if ( j < thumbPos)
        {
            COsdFxWrite1BitFont_value(_SLIDER_2BIT_FS+0x07,color);
        }
        else if ( j > thumbPos)
        {
            COsdFxWrite1BitFont_value(_SLIDER_2BIT_FS+0x01,color);
        }
        else
        {
            if ( thumbFinePos <6)
            {
                COsdFxWrite1BitFont_value(_SLIDER_2BIT_FS+0x02+thumbFinePos,color);
            }

        }
    }
    COsdFxWrite1BitFont_value(_SLIDER_2BIT_FS+0x08,color);
}


/**
 * To control the slider value increase or decrease.
 * @return slider value.
 */
UINT8 COsdCoreSliderOperation(ItemSlider *pSlider)
{
    UINT8 ucMaxVal;
    UINT8 ucMinVal;
    UINT8 ucVal;

    //RTD_Log(LOGGER_INFO, "ItemID = %d\n", (UINT32)(pSlider->ucMenuItemID));
    ucVal = COsdCtrlMenuItemGetValue(pSlider->ucMenuItemID);
    //RTD_Log(LOGGER_INFO, "GetValue = %d\n", (UINT32)ucVal);
    if ( (pSlider->ucCtrl & _SLIDER_VBR_RANGE) != 0)
    {
        ucMaxVal = COsdCtrlMenuItemGetMax(pSlider->ucMenuItemID);
        ucMinVal = COsdCtrlMenuItemGetMin(pSlider->ucMenuItemID);
    }
    else
    {
        ucMaxVal = pSlider->ucMax;
        ucMinVal = pSlider->ucMin;
    }
    //RTD_Log(LOGGER_INFO, "Max = %d  Min = %d\n", (UINT32)ucMaxVal, (UINT32)ucMinVal);
    if (ucVal > ucMaxVal)
        ucVal = ucMaxVal;
    else if (ucVal < ucMinVal)
        ucVal = ucMinVal;

    if (fUI_Msg == _RIGHT_KEY_MESSAGE||fUI_Msg == _RIGHT_IR_MESSAGE)
    {
        if (ucVal < ucMaxVal)
            ucVal += 1;
    }
    else if (fUI_Msg == _LEFT_KEY_MESSAGE||fUI_Msg == _LEFT_IR_MESSAGE)
    {
        if (ucVal > ucMinVal)
            ucVal -= 1;
    }

    //RTD_Log(LOGGER_INFO, "new val = %d\n", (UINT32)ucVal);
    return ucVal;
}

/**
 * To control the bracket value increase or decrease.
 * @return bracket value.
 */
UINT16 COsdCoreBracketOperation(ItemBracket *pBracket)
{
    UINT16 ucVal,i;
    UINT16 ucMax;
    UINT16 iMask ;
    i=0;
    ucVal = COsdCtrlMenuItemGetValue(pBracket->ucMenuItemID);
    ucMax = pBracket->ucCount;
    if ( !(ucVal < ucMax) )
        ucVal = COsdCtrlMenuItemGetDefault(pBracket->ucMenuItemID);
    if ( !(ucVal < ucMax) )
        ucVal = 0;

//->w	RTD_Log(LOGGER_INFO, "ucVal = %d   ucMax = %d\n", (UINT32)ucVal, (UINT32)ucMax);
    if (fUI_Msg == _RIGHT_KEY_MESSAGE||fUI_Msg == _RIGHT_IR_MESSAGE)
    {
        ucVal = (ucVal + 1) % ucMax;
    }
    else if (fUI_Msg == _LEFT_KEY_MESSAGE||fUI_Msg == _LEFT_IR_MESSAGE)
    {
        ucVal = (ucVal + ucMax - 1) % ucMax;
    }


    if (pBracket->ucMenuItemID == MENUITEM_OSD_LANGUAGE)
    {
        while (i<pBracket->ucCount)
        {
            iMask = usLANGUAGE_MASK>>ucVal;
            if (iMask&0x01)
            {
                break;
            }
            else
            {
    if (fUI_Msg == _RIGHT_KEY_MESSAGE||fUI_Msg == _RIGHT_IR_MESSAGE)
                {
                    ucVal = (ucVal + 1) % ucMax;
                }
    else if (fUI_Msg == _LEFT_KEY_MESSAGE||fUI_Msg == _LEFT_IR_MESSAGE)
                {
                    ucVal = (ucVal + ucMax - 1) % ucMax;
                }
            }
            i++;
        }
    }
//RTD_Log(LOGGER_INFO, "new ucVal = %d\n", (UINT32)ucVal);
    return ucVal;

}


/**
 * To get ID of menu item.
 * @return menu item id.
 */
UINT8 ap_osdcore_get_item_control_id(ItemObject *pObj)
{
    if ( pObj->ucType == _SLIDER_TYPE)
    {
        return ((ItemSlider *)(pObj->pStruct))->ucMenuItemID;
    }
    else if ( pObj->ucType == _BRACKET_TYPE || pObj->ucType == _BRACKET1_TYPE)
    {
        return ((ItemBracket *)(pObj->pStruct))->ucMenuItemID;
    }
    else if ( pObj->ucType == _LIST_TYPE)
    {
        return ((ItemList *)(pObj->pStruct))->ucMenuItemID;
    }
    else if ( pObj->ucType == _IMMEDIATE_TYPE)
    {
        return ((ItemImmediate *)(pObj->pStruct))->ucMenuItemID;
    }
    else
        return 0xff;
}


/**
 * Show submenu osd in a main menu
 * @param pass a pointer of data structure SubMenuInf to specify all submenu in a main menu
 * @return None.
 */
UINT8 GetShowIndex(UINT8 ucSubItem)
{
    UINT8 ucCount;
    UINT8  ucBeginItem;
    bit fEnable;
    MenuItem* pMenuItem;
    pMenuItem = stLayerStack[ucLayerIndex].pMenuItem;
    ucBeginItem = 0;

    ucCount = 0;

    while (ucBeginItem <= ucSubItem)
    {

        fEnable= COsdCtrlMenuItemGetEnable(ap_osdcore_get_item_control_id(&(pMenuItem->pItemList[ucBeginItem])));

        if (fEnable)
        {
            ucCount++;
        }
        ucBeginItem++;
    }
    if (ucCount>0)
        ucCount= ucCount -1;
    return ucCount;
}

void COsdCoreDrawMenuItems(MenuItem* pMenuItem, UINT8 ucItemID, BOOL bRefresh)
{
    UINT8 j,flag,totalitem;
    UINT8 ucCnt;
    UINT8 ucWidth;
    UINT8 id;
    UINT8 ucFontColor;
    UINT8 menuItemEnable[8];
    UINT8 row;
    row = 0;
    flag = 0;
    totalitem = GetShowIndex(ucItemID);
	//RTD_Log(LOGGER_INFO,"melon__totalitem---------------%d\n",(UINT32)totalitem);
    ucPage = totalitem / 6;
    if (ucPage==0)
        ucNoshowItem = 0;
    if (bRefresh)
    {
        for (j=0; j<6; j++)
        {
            WaitFor_DEN_STOP();
            // 耿玡狠跋办
            COsdFxLoadBlankCmd(4+j*2, 7, 10, _CP_BLACK);
            //ucCnt = 6;
            //clear sub page
            OSDLine(4+j*2, 7, _OSD_ITEM_ROW_LENGTH-7,0x8c,_WRITE_BYTE0);
            OSDLine(4+j*2, 7, _OSD_ITEM_ROW_LENGTH-7,0x00,_WRITE_BYTE1);
        }
    }
    LoadLanguage_FONT(pMenuItem->ucMenuID);
    WaitFor_DEN_STOP();
    for (j=0; j<6; j++)
    {
        ucCnt = 7;
        flag ++;
        if (ucPage>0)
            id =( ucItemID/6)*6+flag-1+ucNoshowItem;
        else
            id =( totalitem/6)*6+flag-1;
        menuItemEnable[j] = COsdCtrlMenuItemGetEnable(ap_osdcore_get_item_control_id(&(pMenuItem->pItemList[id])));
        // Draw Items
        if (id<pMenuItem->ucItemCount)
        {
            // Draw item string
            if ( menuItemEnable[j] )
            {
                ucFontColor = FONT_COLOR(4, _CP_BLACK);
                if (j>0)
                    row ++;
                if (row>5)
                {
                    break;
                }
            }
            else
            {
                j--;
                if (ucPage > 0)//Tim 0323
                ;
                else
                ucNoshowItem++;
                continue;
                //ucFontColor = FONT_COLOR(7, _CP_BLACK);
            }
            ucCnt += COsdFxLoadText(4+row*2, ucCnt, 0, ucFontColor, COsdCtrlGetLanguage(), pMenuItem->pItemList[id].pString);
            if ( menuItemEnable[j] )
            {
                ucWidth = COsdFxCalTextWidth(COsdCtrlGetLanguage(), pMenuItem->pItemList[id].pString);
                if ( ucWidth < 168)
                    ucCnt += COsdFxLoadBlankCmd(4+row*2, ucCnt, 168-ucWidth, _CP_BLACK);
                menu_item_col[row] = ucCnt;

                if (pMenuItem->pItemList[id].ucType == _SLIDER_TYPE)
                {
                    COsdCoreUpdateSliderItem(id,ucFontColor);
                }
                else if (pMenuItem->pItemList[id].ucType == _BRACKET_TYPE)
                {
                    COsdCoreUpdateBracketItem(id,ucFontColor);
                }
                else if (pMenuItem->pItemList[id].ucType == _BRACKET1_TYPE)
                {
                    COsdCoreUpdateBracketItem(id,ucFontColor);
                }
                else if (pMenuItem->pItemList[id].ucType == _LIST_TYPE)
                {
                    COsdCoreUpdateListItem(id,ucFontColor);
                }
                else if (pMenuItem->pItemList[id].ucType == _IMMEDIATE_TYPE || pMenuItem->pItemList[id].ucType == _SUBPAGE_TYPE)
                {
                    ucCnt += 1; //蛤 slider bar 癸霍
                    ucCnt += COsdFxLoadBlankCmd(4+row*2, ucCnt, _ITEM_AREA_LEN, _CP_BLACK);
                    COsdFxWrite1BitFont(4+row*2, ucCnt, _R_ARROW_, ucFontColor);
                }
                else
                {
                    // no support item?
                }
            }
        }
        else
        {
            break;
        }
    }
}

/**
 * Update menu slider item.
 *
 */
void COsdCoreUpdateSliderItem(UINT8 ucItemId,UINT8 color)
{
    UINT8 ucCnt,ucIndex;

    if (ucItemId >= stLayerStack[ucLayerIndex].pMenuItem->ucItemCount)
        return;
    ucIndex = GetShowIndex(ucItemId);
    ucCnt = menu_item_col[ucIndex%6];
    COsdCoreDrawSlider(4+(ucIndex%6)*2, ucCnt, 12, (ItemSlider *)(stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucItemId].pStruct),color);
    ucCnt += 14;	// slider cost 14 fonts
    COsdCoreDrawSliderNumber(4+(ucIndex%6)*2, ucCnt, color, (ItemSlider *)(stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucItemId].pStruct));
}


/**
 * To get string pointer of bracket item.
 * @return bracket item string.
 */
UINT8* COsdCoreGetBracketString(void)
{
    UINT8 ucVal;
    ItemBracket* pBracket;

    pBracket = (ItemBracket *)(pItemObj->pStruct);
    ucVal = COsdCtrlMenuItemGetValue(pBracket->ucMenuItemID);
    if ( !(ucVal < pBracket->ucCount))
        ucVal = COsdCtrlMenuItemGetDefault(pBracket->ucMenuItemID);
    if ( !(ucVal < pBracket->ucCount) )
        ucVal = 0;

    if ( pItemObj->ucType == _BRACKET_TYPE)
        return pBracket->pLabelString[ucVal];
    else if ( pItemObj->ucType == _BRACKET1_TYPE)
        return COsdCtrlMenuItemGetString(pBracket->ucMenuItemID);
    else
        return tStr_Empty;
}
#if ENABLE_FM_FREQSET
UINT8 COsdCoreFMTuningitem(UINT8 ucRow,UINT8 ucCnt,UINT8 ucColor,UINT8 *out_width)

{
    //UINT8 ch,offset;
    UINT32 freq;
    UINT8 uccount;
    UINT32 Mhz;
	UINT16 offset1;
	//UINT32 temp1,temp2,temp3,temp4,temp5,temp6;

   // ch = GET_TV_CHN_DATA(_CUR_CHN_TV);

	
	//RTD_Log(LOGGER_LEVEL,"\n Tim ch = %d \n",(UINT32)ch);
    offset1 = GET_OSD_DISPLAY_Radio();
    freq = 760000 + (UINT32)(offset1-1)*1000; 
	//temp4 = freq;
	#if MELON_DEBUG_TEST
	RTD_Log(LOGGER_LEVEL,"\n melon_freq = %d \n",(UINT32)freq);
	#endif
    //freq = drv_tuner_GeDiverRatio(freq);
	#if 0//1
    temp1 = GET_CHANNEL_FREQ(ch);
	temp2 = drv_tuner_GetRatioInStepSize();
	temp3 = COsdCtrlGetTVFineTune();
	temp5 = freq;
	temp6 = offset;
	//RTD_Log(LOGGER_LEVEL,"\n temp1 = %d,temp2 = %d,temp3 = %d,temp4 = %d,temp5 = %d,temp6 = %d \n",temp1,temp2,temp3,temp4,temp5,temp6);
	#endif
	
	//RTD_Log(LOGGER_LEVEL,"\n temp1 = %d,temp2 = %d,temp3 = %d,temp4 = %d,temp5 = %d,temp6 = %d \n",(UINT32)temp1,(UINT32)temp2,(UINT32)temp3,(UINT32)temp4,(UINT32)temp5,(UINT32)temp6);
    Mhz = freq / 100;
    
#ifdef CONFIG_TUNER_SILICON
    //uccount = COsdFxShowNumber(ucRow, ucCnt, (UINT32)(freq-0x7d)*25/4, 0x57, ucColor);
    //*out_width = COsdFxGetNumberwidth((UINT32)(freq-0x7d)*25/4, 0x57);
	uccount = COsdFxShowNumber(ucRow,ucCnt, (UINT32)Mhz, 0x57, ucColor);//Tim 0323
	*out_width = COsdFxGetNumberwidth( (UINT32)Mhz, 0x57);
#else
    uccount = COsdFxShowNumber(ucRow,ucCnt, (UINT32)Mhz, 0x57, ucColor);//Tim 0323
    *out_width = COsdFxGetNumberwidth( (UINT32)Mhz, 0x57);
#endif
    RTD_Log(LOGGER_INFO, "--------------freq======%x\n",(UINT32)freq);
    return uccount;
}
#endif
#ifdef _VIDEO_TV_SUPPORT
UINT8 COsdCoreTuningitem(UINT8 ucRow,UINT8 ucCnt,UINT8 ucColor,UINT8 *out_width)
{
    UINT8 ch,offset;
    UINT32 freq;
    UINT8 uccount;
    UINT32 Mhz;
	//UINT32 temp1,temp2,temp3,temp4,temp5,temp6;

    ch = GET_TV_CHN_DATA(_CUR_CHN_TV);

	
	//RTD_Log(LOGGER_LEVEL,"\n Tim ch = %d \n",(UINT32)ch);
    offset = COsdCtrlGetTVFineTune();
    freq = GET_CHANNEL_FREQ(ch) + (UINT32)offset * drv_tuner_GetRatioInStepSize() - 24UL * drv_tuner_GetRatioInStepSize();
	//temp4 = freq;

    //freq = drv_tuner_GeDiverRatio(freq);
	#if 0//1
    temp1 = GET_CHANNEL_FREQ(ch);
	temp2 = drv_tuner_GetRatioInStepSize();
	temp3 = COsdCtrlGetTVFineTune();
	temp5 = freq;
	temp6 = offset;
	//RTD_Log(LOGGER_LEVEL,"\n temp1 = %d,temp2 = %d,temp3 = %d,temp4 = %d,temp5 = %d,temp6 = %d \n",temp1,temp2,temp3,temp4,temp5,temp6);
	#endif
	
	//RTD_Log(LOGGER_LEVEL,"\n temp1 = %d,temp2 = %d,temp3 = %d,temp4 = %d,temp5 = %d,temp6 = %d \n",(UINT32)temp1,(UINT32)temp2,(UINT32)temp3,(UINT32)temp4,(UINT32)temp5,(UINT32)temp6);
    Mhz = freq / 100;
    
#ifdef CONFIG_TUNER_SILICON
    //uccount = COsdFxShowNumber(ucRow, ucCnt, (UINT32)(freq-0x7d)*25/4, 0x57, ucColor);
    //*out_width = COsdFxGetNumberwidth((UINT32)(freq-0x7d)*25/4, 0x57);
	uccount = COsdFxShowNumber(ucRow,ucCnt, (UINT32)Mhz, 0x57, ucColor);//Tim 0323
	*out_width = COsdFxGetNumberwidth( (UINT32)Mhz, 0x57);
#else
    uccount = COsdFxShowNumber(ucRow,ucCnt, (UINT32)Mhz, 0x57, ucColor);//Tim 0323
    *out_width = COsdFxGetNumberwidth( (UINT32)Mhz, 0x57);
#endif
    RTD_Log(LOGGER_INFO, "--------------freq======%x\n",(UINT32)freq);
    return uccount;
}
#endif
/**
 * Update menu bracket item.
 *
 */
void COsdCoreUpdateBracketItem(UINT8 ucItemId,UINT8 ucFontColor)
{
    UINT8 ucCnt;
    UINT8 ucRow,unIndex;
    UINT8 ucWidth;

    ItemBracket* pBracket;
    UINT8 code arrow[] =
    {
        _LR_ARROW_,_R_ARROW_,_END_,
    };
	#if ENABLE_FM_FREQSET
    UINT8 code Str_off[] =
    {
        _O_,_f_,_f_,0x01,0x01,_END_,
    };
	#endif
    UINT8* pStr;

    if (ucItemId >= stLayerStack[ucLayerIndex].pMenuItem->ucItemCount)
        return;
    unIndex = GetShowIndex(ucItemId);
    ucCnt = menu_item_col[unIndex%6];
    ucCnt += 1;
    pItemObj = &(stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucItemId]);
    pBracket = pItemObj->pStruct;
    ucRow = 4+(unIndex%6)*2;

#ifdef _VIDEO_TV_SUPPORT
    if (pBracket->ucMenuItemID==MENUITEM_TV_FREQ_FINE_TUNE
		#if ENABLE_FM_FREQSET
		||pBracket->ucMenuItemID==MENUITEM_DISPLAY_RADIO
		#endif
		)
    {
       #if ENABLE_FM_FREQSET
        if(pBracket->ucMenuItemID==MENUITEM_DISPLAY_RADIO)
    	{

			#if MELON_DEBUG_TEST
			RTD_Log(LOGGER_LEVEL,"\n melon_ucWidth = %d ,ucCnt=%d \n",(UINT32)ucWidth,(UINT32)ucCnt);
			#endif
			if(GET_OSD_DISPLAY_Radio())
				{
					ucCnt += COsdCoreFMTuningitem(ucRow,ucCnt,ucFontColor>>4,&ucWidth);	
					ucCnt += COsdFxLoadBlankCmd(ucRow, ucCnt, 168-12-ucWidth, _CP_BLACK);
				}
            else
            	{
            		COsdFxLoadText(ucRow, ucCnt, 0, ucFontColor, 0, Str_off);
            	}
			//COsdFxLoadText(ucRow, ucCnt+10, 0, ucFontColor, 0, Str_Mhz);
			#if MELON_DEBUG_TEST
			//RTD_Log(LOGGER_LEVEL,"\n melon_ucRow111 = %d ,ucCnt111=%d \n",(UINT32)ucRow,(UINT32)ucCnt);
			#endif
			// ucCnt=21;
        	 //ucCnt += COsdFxLoadText(ucRow, ucCnt, 0, ucFontColor, 0, arrow);
    	}
		else
		#endif
		{
        ucCnt += COsdCoreTuningitem(ucRow,ucCnt,ucFontColor>>4,&ucWidth);
        ucCnt += COsdFxLoadBlankCmd(ucRow, ucCnt, 168-12-ucWidth, _CP_BLACK);
        ucCnt += COsdFxLoadText(ucRow, ucCnt, 0, ucFontColor, 0, arrow);
		}
    }
    else
#endif
    {
        pStr = COsdCoreGetBracketString();
        WaitFor_DEN_STOP();//avoid OSD garbage
        WaitFor_DEN_STOP();//avoid OSD garbage
    if (pBracket->ucMenuItemID==MENUITEM_OSD_LANGUAGE)
    	{
        ucWidth = COsdFxCalTextWidth(0, pStr);
        ucCnt += COsdFxLoadText(ucRow, ucCnt, 0, ucFontColor, 0, pStr);
    	}
	else
	{
	  ucWidth = COsdFxCalTextWidth(COsdCtrlGetLanguage(), pStr);
        ucCnt += COsdFxLoadText(ucRow, ucCnt, 0, ucFontColor, COsdCtrlGetLanguage(), pStr);
	}
        if (ucWidth < 168)
            ucCnt += COsdFxLoadBlankCmd(ucRow, ucCnt, 168-ucWidth, _CP_BLACK);
        ucCnt += COsdFxLoadText(ucRow, ucCnt, 0, ucFontColor, 0, arrow);
        if (ucCnt < _OSD_ITEM_ROW_LENGTH)
            COsdFxClearText(ucRow, ucCnt, 1, _OSD_ITEM_ROW_LENGTH-ucCnt);
    }
#if 0
#undef ucWidth
#undef ucRow
#undef ucCnt
#endif
}

/**
 * Update list item.
 *
 */
void COsdCoreUpdateListItem(UINT8 ucItemId,UINT8 ucFontColor)
{
    UINT8 ucCnt;
    UINT8 ucWidth,unIndex;
    ItemList *pList;
    UINT8 ucVal;
    UINT8 row;

    ucCnt = 6;

    if (ucItemId >= stLayerStack[ucLayerIndex].pMenuItem->ucItemCount)
        return;
    unIndex = GetShowIndex(ucItemId);
    ucCnt = menu_item_col[unIndex%6];
    ucCnt += 1;
    pList = (ItemList *)(stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucItemId].pStruct);

    ucVal = COsdCtrlMenuItemGetValue(pList->ucMenuItemID);
    //090706 Willy added, if the menu item value is unreasonable, load to default
    if (ucVal >= pList->ucItemCount)
    {
        ucVal = COsdCtrlMenuItemGetDefault(pList->ucMenuItemID);
    }

    // boundary check
    if ( ucVal > pList->ucItemCount)
        ucVal = 0;
    row = 4+(unIndex%6)*2;
    ucCnt += COsdFxLoadText(row, ucCnt, 0, ucFontColor, COsdCtrlGetLanguage(), pList->pItemList[ucVal].pString);
    ucWidth = COsdFxCalTextWidth(COsdCtrlGetLanguage(), pList->pItemList[ucVal].pString);
    if (ucWidth < _ITEM_AREA_LEN)
        ucCnt += COsdFxLoadBlankCmd(row, ucCnt, _ITEM_AREA_LEN-ucWidth, _CP_BLACK);
    COsdFxWrite1BitFont(row, ucCnt, _R_ARROW_, ucFontColor); // right-arrow sign
    ucCnt += 1;

    if (ucCnt < _OSD_ITEM_ROW_LENGTH)
        COsdFxClearText(row, ucCnt, 1, _OSD_ITEM_ROW_LENGTH-ucCnt);
}

/**
 * To convert list item value to idx.
 *
 */
UINT8 COsdCoreListItemValueToIdx(UINT8 ucValue)
{
    UINT8 idx;

    for (idx=0; idx<pListItem->ucItemCount; idx++)
    {
        if ( pListItem->pItemList[idx].ucValue == ucValue)
        {
            return idx;
        }
    }

    return 0; // default idx
}

/**
 * To initial list page.
 *
 */
void COsdCoreInitListPage(void)
{
    UINT8 ucCnt;
    if ((pListItem->ucMenuItemID==MENUITEM_TV_VIDEO_STD) || (pListItem->ucMenuItemID==MENUITEM_TV_AUDIO_STD))
	    ucListSelectedItem = COsdCtrlMenuItemGetValue(pListItem->ucMenuItemID);
    else 
	    ucListSelectedItem = COsdCoreListItemValueToIdx(COsdCtrlMenuItemGetValue(pListItem->ucMenuItemID));
    //COsdFxShowWindow(tHighLightWindowStyle.ucWindowID, _FALSE);

    // name of list item
    //ucCnt = 5;
    //ucCnt += COsdFxLoadText(4, ucCnt, 10, FONT_COLOR(4, 0), 0, pListItem->pString);
    //COsdFxClearText(4, ucCnt, 1, _OSD_ITEM_ROW_LENGTH-ucCnt);

    ucPage = ucListSelectedItem/6;

    WaitFor_DEN_STOP();
    for (ucCnt=0; ucCnt<6; ucCnt++)
    {
    	WaitFor_DEN_STOP();
    	OSDLine(4+ucCnt*2, 7, _OSD_ITEM_ROW_LENGTH-7,0x8c,_WRITE_BYTE0);
    	OSDLine(4+ucCnt*2, 7, _OSD_ITEM_ROW_LENGTH-7,0x00,_WRITE_BYTE1);
    }
    COsdCoreUpdateListPage();

//	COsdCoreSetListHighlight();
//	COsdFxShowWindow(tHighLightWindowStyle.ucWindowID, _TRUE);
}

/**
 * To set highlist position of list item.
 *
void COsdCoreSetListHighlight(void)
{
	stItemPos = tHighLightWindowStyle.stPosition;
	stItemPos.usVStart = tHighLightWindowStyle.stPosition.usVStart + (ucListSelectedItem%7+1)*36;
	stItemPos.usVEnd = tHighLightWindowStyle.stPosition.usVEnd + (ucListSelectedItem%7+1)*36;
	COsdFxMoveWindow(tHighLightWindowStyle.ucWindowID, &stItemPos);
}
 */
/**
 * To update content of list page.
 *
 */
/*void drawSelctListItem(UINT8 ucFontColor)
{
    COsdFxLoadText(4+ucListSelectedItem*2, 7, 0, ucFontColor, COsdCtrlGetLanguage(), pListItem->pItemList[ucListSelectedItem].pString);

}*/
void COsdCoreUpdateListPage(void)
{
    UINT8 j;
    UINT8 ucCnt;
    UINT8 ucFontColor;
    UINT8 ucIndex;
	UINT8 idx;
    ucFontColor = 0x40;
    if (pListItem == NULL)
    { // clear item area
        WaitFor_DEN_STOP();
        ucCnt = 6;
        for (j=0; j<6; j++)
        {
        	OSDLine(4+j*2, ucCnt, _OSD_ITEM_ROW_LENGTH-ucCnt,0x8c,_WRITE_BYTE0);
    		OSDLine(4+j*2, ucCnt, _OSD_ITEM_ROW_LENGTH-ucCnt,0x00,_WRITE_BYTE1);
        }
        return;
    }

    for (j=0; j<6; j++)
    {
        // 耿玡狠跋办
        COsdFxLoadBlankCmd(4+j*2, 7, 10+12*2, _CP_BLACK);
        ucCnt = 7;
        if ((pListItem->ucMenuItemID==MENUITEM_TV_VIDEO_STD) || (pListItem->ucMenuItemID==MENUITEM_TV_AUDIO_STD))//Edit SVN53
			idx = COsdCtrlMenuItemGetValue(pListItem->ucMenuItemID);
        else
	        idx = COsdCoreListItemValueToIdx(COsdCtrlMenuItemGetValue(pListItem->ucMenuItemID));
	        
        if ((ucPage*6+j == idx) && (ucFirstInList == 1))//COsdCoreListItemValueToIdx(COsdCtrlMenuItemGetValue(pListItem->ucMenuItemID)))
        {
            ucFontColor = _SELECT_COLOR;
            //OSDLine((ucListSelectedItem%6)*2+5, 7, 31, 0x0D ,_WRITE_BYTE1);
            //DrawSelectLine(ucListSelectedItem);
        }
        if (ucPage*6+j < pListItem->ucItemCount)
        {
            ucCnt += COsdFxLoadText(4+j*2, ucCnt, 0,ucFontColor, COsdCtrlGetLanguage(), pListItem->pItemList[ucPage*6+j].pString);
            //RTD_Log(LOGGER_LEVEL,"\n Tim uccnt = %d,ucPage*6+j =%d \n",(UINT32)ucCnt,(UINT32)ucPage*6+j);
        }
        ucFontColor = 0x40;

        if (ucCnt < _OSD_ITEM_ROW_LENGTH)
            COsdFxClearText(4+j*2, ucCnt, 1, _OSD_ITEM_ROW_LENGTH-ucCnt);
    }
    ucIndex =GetShowIndex(MAINMENU_GET_ITEM_ID(stLayerStack[ucLayerIndex].ucSubMenu));
    OSDLine((ucIndex%6)*2+5, 7, 31, 0x00, _WRITE_BYTE1);
    //ClearSelectLine();

}

/**
 * To handle message event in list item state.
 *
 */
void COsdCoreListItemEventProc(void)
{
#ifdef SUPPORT_FIVE_KEY
    if(COsd_LRkey2UDkey() == _TRUE)
    {
        if(fUI_Msg == _LEFT_KEY_MESSAGE)
            fUI_Msg = _UP_KEY_MESSAGE;
        else if(fUI_Msg == _RIGHT_KEY_MESSAGE)
            fUI_Msg = _DOWN_KEY_MESSAGE;
    }
#endif
    if (( fUI_Msg < _TTX_EXIT_MESSAGE)&&(fUI_Msg!=_FL2UI_MAIN_NO_SIGNAL))//Hardy20091209
    	{
        	fw_timer_event_ReactiveTimerEvent(ap_osdctrl_get_osd_timeout_ms(), timerevent_ui_table_OSD_timeout_event);
    	}

    switch (fUI_Msg)
    {
    case _UP_KEY_MESSAGE:
    case _DOWN_KEY_MESSAGE:
	case _MENU_KEY_MESSAGE:
        //drawSelctListItem(_NORMAL_COLOR);
        
        OSDLine(4+(ucListSelectedItem%6)*2, 7, 34, _NORMAL_COLOR, _WRITE_BYTE2);
#ifdef SUPPORT_FIVE_KEY
#else
        OSDLine(5+(ucListSelectedItem%6)*2, 7, 31, 0x00, _WRITE_BYTE1);
#endif
        if ( fUI_Msg == _UP_KEY_MESSAGE)
        {
            if ( ucListSelectedItem == 0)
                ucListSelectedItem = pListItem->ucItemCount-1;
            else
                ucListSelectedItem--;
        }
        else
        {
            ucListSelectedItem++;
            if ( ucListSelectedItem >= pListItem->ucItemCount)
                ucListSelectedItem = 0;
        }
        if (ucPage != ucListSelectedItem/6)
        {
            ucPage = ucListSelectedItem/6;
            ucFirstInList = 2;
            COsdCoreUpdateListPage();
        }
        //OSDLine((ucListSelectedItem%6)*2+5, 7, 31, 0x0D ,_WRITE_BYTE1);
        //DrawSelectLine(ucListSelectedItem);
        //drawSelctListItem(_SELECT_COLOR);
        OSDLine(4+(ucListSelectedItem%6)*2, 7, 34, _SELECT_COLOR, _WRITE_BYTE2);
        break;

#ifdef SUPPORT_FIVE_KEY
    case _OK_KEY_MESSAGE:
   case _OK_IR_MESSAGE:
   case _RIGHT_IR_MESSAGE:
#else
   case _OK_KEY_MESSAGE:
   case _OK_IR_MESSAGE:
   	
   case _RIGHT_KEY_MESSAGE:
   case _LEFT_KEY_MESSAGE:
 #endif
        //COsdCoreUpdateListPage(); // to clear list page
        //COsdFxShowWindow(tHighLightWindowStyle.ucWindowID, _FALSE);
        //COsdCoreFallBackMenu();
        //if ( COsdCoreListItemValueToIdx(pListItem->pGetFunction()) != ucListSelectedItem)
		//RTD_Log(LOGGER_LEVEL,"\n Tim ucListSelectedItem=%d \n",(UINT32)ucListSelectedItem);
        COsdCtrlMenuItemHandler(pListItem->ucMenuItemID, pListItem->pItemList[ucListSelectedItem].ucValue);
        COsdCoreFallBackMenu();			//pListItem = NULL;
        break;

    //case _LEFT_KEY_MESSAGE:
	case _LEFT_IR_MESSAGE:
        break;

    case _EXIT_KEY_MESSAGE:
	case _DISPLAY_RATIO_KEY_MESSAGE:
    //case _MENU_IR_MESSAGE:
  //case _MENU_KEY_MESSAGE:
        COsdCoreFallBackMenu();
        break;
    case _SHOW_OSD_MESSAGE:
        COsdFxEnableOsd();
        break;
    case _OSD_TIMEOUT_MESSAGE:
    	//RTD_Log(LOGGER_LEVEL,"\n _OSD_TIMEOUT_MESSAGE Tim bCheckCCStatus =%d \n",(UINT32)bCheckCCStatus);
        COsdCoreExitOsd();
        break;

    default:
        break;

    }

}

extern UINT32 code tNTSC_CATV_FREQ[];
extern UINT32 code tNTSC_AIR_FREQ[];
/**
 * To initial pop-on slider OSD.
 *
 */
void COsdCoreInitPoponSlider(UINT8 opt)
{
    UINT8 ucWidth;
    UINT8 ucCnt;
    #ifdef _VIDEO_TV_SUPPORT
    UINT8  NtscType;
    UINT32 freq,tableFreq;
	#endif

	InitOsdFrame();
    ap_osdfx_map_setting(_OSD_MAP0);
    SetOSDDouble(_OFF);

    if ( IS_VOLUME_MODE(opt))
        COsdFxSetWinRefPos(_OSD_VOLUME_BAR_H_POS+7, _OSD_VOLUME_BAR_V_POS+120);
    else
        COsdFxSetWinRefPos(_OSD_POPON_SLIDER_H_POS, _OSD_POPON_SLIDER_V_POS);
    // Load pop-on slider fonts
    //ap_osdfx_load_vlc_font(tFont_SourcePatch, _FRAME_2BIT_PATCH_ADDR, 12);

    // draw title bar
    ucWidth = COsdFxCalTextWidth(GET_OSD_LANGUAGE(),pItemObj->pString);	// display message
    ucCnt = (27*12-ucWidth)/2;
    ucCnt = COsdFxLoadText(1, 0, ucCnt, FONT_COLOR(1, _CP_BLACK), GET_OSD_LANGUAGE(), pItemObj->pString);
    ucCnt++;
    COsdFxClearText(0, ucCnt, 1, _OSD_TITLE_ROW_LENGTH-ucCnt);


    OSDLine(ROW(2), COL(2), LENGTH(23), 0xA6, _WRITE_BYTE0);
    OSDLine(ROW(2), COL(2), LENGTH(23), 0xA4, _WRITE_BYTE1);
    OSDLine(ROW(2), COL(2), LENGTH(23), 0x11, _WRITE_BYTE2);

    // Draw Bottom Line
    OSDLine(ROW(4), COL(2), LENGTH(23), 0xA6, _WRITE_BYTE0);
    OSDLine(ROW(4), COL(2), LENGTH(23), 0xA4, _WRITE_BYTE1);
    OSDLine(ROW(4), COL(2), LENGTH(23), 0x11, _WRITE_BYTE2);
    #ifdef _VIDEO_TV_SUPPORT
    if (IS_CHANNEL_SCAN_MODE(opt))
    {
        COsdFxClearText(4, 0, 1, _OSD_TITLE_ROW_LENGTH);
        // Get current channel
        ucCnt = GET_TV_CUR_CHN();

        if (GET_TV_MANUAL_SCAN())
        {
            freq = GET_CHANNEL_FREQ(ucCnt);
            tvCoverCurFreq = freq/100;
            COsdCoreUpdateChannelScanSlider(ucCnt);//, tvCoverCurFreq);//drv_tuner_GeDiverRatio(freq));
        }
        else if (GET_TV_TABLE_SCAN())
        {
            NtscType = GET_TV_CUR_NtscType();
            
            //tvScanCurFreq = tNTSC_CATV_FREQ[ucTableProgress] ;
            if(NtscType == 0)//Cable
                tableFreq = 732500;//tNTSC_CATV_FREQ[1];//Tim 0328
			else
                tableFreq = 552500;//tNTSC_AIR_FREQ[2];//Tim 0328
            COsdFxLoadText(4, 21, 10, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tStr_MHz);
			RTD_Log(LOGGER_LEVEL,"\n tableFreq=%d \n",(UINT32)tableFreq);

			RTD_Log(LOGGER_LEVEL,"\n ucCnt=%d \n",(UINT32)ucCnt);
            COsdCoreUpdateTableScanSlider(0, 0,tableFreq);//Tim 0325  //Tim 0328
        }
        else
        {
            COsdFxLoadText(4, 21, 10, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tStr_MHz);
            tvCoverCurFreq = drv_tuner_GetChanelMinFreq()/100;
            COsdCoreUpdateChannelScanSlider(ucCnt);//, tvCoverCurFreq);//drv_tuner_GeDiverRatio(drv_tuner_GetChanelMinFreq()));
        }
    }
    else
    #endif
        COsdCoreUpdatePoponSlider();
    COsdFxDrawWindow(&tPopon_Slider_WindowStyle);

    COsdFxEnableOsd();
}

/**
 * To redraw popon slider OSD.
 *
 */
void COsdCoreUpdatePoponSlider(void)
{
    UINT8 ucCnt;

    ucCnt = 0;
    ucCnt = COsdFxLoadBlankCmd(3, ucCnt, 36, _CP_BLACK);
    COsdCoreDrawSlider(3, ucCnt, 17, (ItemSlider *)(pItemObj->pStruct), _SELECT_COLOR);
    ucCnt += 19;
    COsdCoreDrawSliderNumber(3, ucCnt, _SELECT_COLOR, (ItemSlider *)(pItemObj->pStruct));
}
#ifdef _VIDEO_TV_SUPPORT
/**
 * To redraw channel scan slider OSD.
 * @return none.
 */
void COsdCoreUpdateChannelScanSlider(UINT8 ucCh)//, UINT16 uwFreq)
{
    if (ucOsdState == _MAINMENU_STATE)
        return;
    COsdFxShowNumber(4, 1/*ucCnt*/, ucCh, 0x23, _CP_WHITE);
    COsdCoreDrawSlider(3, 1/*ucCnt*/, 24, (ItemSlider *)(pItemObj->pStruct), _CP_WHITE<<4 | _CP_BLACK);
    //RTD_Log(LOGGER_INFO, "COsdCoreUpdateChannelScanSlider uwFreq= %d\n", (UINT32)uwFreq);
    //COsdFxShowNumber(4, 15/*ucCnt*/, (UINT32)(uwFreq-0x7d)*25/4, 0x57, _CP_WHITE);
    COsdFxShowNumber(4, 15/*ucCnt*/, (UINT32)(tvCoverCurFreq),0x57, _CP_WHITE);

}


/**
 * To redraw channel scan slider OSD.
 * @return none.
 */
void COsdCoreUpdateTableScanSlider(UINT8 ucCh, UINT8 ucProgress,UINT32 uwFreq)//Tim 0325
{
	UINT32 Mhz;//,Khz;

    ucOsdCoreBuf[7] = ucProgress;

    COsdFxShowNumber(4, 1/*ucCnt*/, ucCh, 0x23, _CP_WHITE);
    COsdCoreDrawSlider(3, 1/*ucCnt*/, 24, (ItemSlider *)(pItemObj->pStruct), _CP_WHITE<<4 | _CP_BLACK);
    COsdFxShowNumber(4, 18, ucProgress, 0x23, _CP_WHITE);
    
    Mhz = uwFreq / 100;
    COsdFxShowNumber(4, 15/*ucCnt*/, (UINT32)Mhz, 0x57, _CP_WHITE);

    //COsdFxWrite1BitFont(4, 22, _PERCENT_, _CP_WHITE << 4 | _CP_BLACK);

}
#endif
/**
 * To switch to next pop-on slider.
 *
 */
void COsdCorePoponSliderNextItem(UINT8 ucOpt)
{
    UINT8 ucSaveCnt;
    UINT8 ucSubTmp;
    UINT8 ucCnt;
    ItemSlider *pSlider;

    ucSaveCnt = 0;
    pSlider = (ItemSlider *)(stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucSubTmp].pStruct);
    ucSubTmp = MAINMENU_GET_ITEM_ID(stLayerStack[ucLayerIndex].ucSubMenu);
    RTD_Log(LOGGER_INFO, "ucSubTmp = %x\n", (UINT32)ucSubTmp);
    ClearSelectLine();
    do
    {
        ucCnt = stLayerStack[ucLayerIndex].pMenuItem->ucItemCount;
        if ( (ucOpt&_BIT0) != 0)  // Next item
        {
            ucSubTmp = (ucSubTmp + 1) % ucCnt;
        }
        else
        {
            ucSubTmp = (ucSubTmp + ucCnt - 1) % ucCnt;
        }
        if ( _SLIDER_TYPE == stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucSubTmp].ucType)
        {
            pSlider = (ItemSlider *)(stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucSubTmp].pStruct);
            if ( (pSlider->ucCtrl & _SLIDER_POPON) != 0)
                break;
        }
        ucSaveCnt++;
        if ( ucSaveCnt > stLayerStack[ucLayerIndex].pMenuItem->ucItemCount)
        {
            ucSubTmp = MAINMENU_GET_ITEM_ID(stLayerStack[ucLayerIndex].ucSubMenu);
            break;
        }
    }
    while (1);

    pItemObj = &(stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucSubTmp]);
    MAINMENU_SET_SUB_ACTIVE(ucSubTmp);
    stLayerStack[ucLayerIndex].ucSubMenu = ucSubTmp;
   // DrawSelectLine(ucSubTmp);
}
/**
 * The message handler of pop-on slider state.
 *
 */
void COsdCorePoponSliderEventProc(void)
{
		UINT8 ucValue;
		ItemSlider *pSlider;
    if (( fUI_Msg < _TTX_EXIT_MESSAGE)&&(fUI_Msg!=_FL2UI_MAIN_NO_SIGNAL))//Hardy20091209
        fw_timer_event_ReactiveTimerEvent(ap_osdctrl_get_osd_timeout_ms(), timerevent_ui_table_OSD_timeout_event);

    switch (fUI_Msg)
    {
#ifdef SUPPORT_FIVE_KEY
#else
    case _LEFT_KEY_MESSAGE:

        pSlider = (ItemSlider*)(pItemObj->pStruct);
        ucValue = COsdCoreSliderOperation(pSlider);
        if (ucValue != COsdCtrlMenuItemGetValue(pSlider->ucMenuItemID) )
        {
            COsdCtrlMenuItemHandler(pSlider->ucMenuItemID, ucValue);
            COsdCoreUpdatePoponSlider();
        }
        break;

    case _RIGHT_KEY_MESSAGE:
            pSlider = (ItemSlider*)(pItemObj->pStruct);
            ucValue = COsdCoreSliderOperation(pSlider);
            if (ucValue != COsdCtrlMenuItemGetValue(pSlider->ucMenuItemID))
            {
                COsdCtrlMenuItemHandler(pSlider->ucMenuItemID, ucValue);
                COsdCoreUpdatePoponSlider();
            }
		break;
#endif

    case _UP_KEY_MESSAGE:
        COsdCorePoponSliderNextItem(0);
        break;

    case _DOWN_KEY_MESSAGE:
        COsdCorePoponSliderNextItem(_BIT0);
        break;

    case _EXIT_KEY_MESSAGE:
//		COsdCoreExitOsd();
//		break;
    //case _MENU_KEY_MESSAGE:
    //case _MENU_IR_MESSAGE:
	case _DISPLAY_RATIO_KEY_MESSAGE:
        COsdFxDisableOsd();
        //ap_osdfx_load_vlc_font(tFont_slider_2bit, _SLIDER_FONT_ADDR, 24);	// re-load slider font
        COsdCoreFallBackMenu();
        break;

    case _OSD_TIMEOUT_MESSAGE:
    
    //RTD_Log(LOGGER_LEVEL,"\n _OSD_TIMEOUT_MESSAGE11 Tim bCheckCCStatus =%d \n",(UINT32)bCheckCCStatus);
        COsdCoreExitOsd();
        //	ap_osdfx_load_vlc_font(tFont_slider_2bit, _SLIDER_FONT_ADDR, 24);	// re-load slider font
        break;

    default:
        break;
    }
}
/**
 * To handle message on Volume state.
 *
 */
void COsdCoreVolumeEventProc(void)
{
    UINT8 ucValue;

    if (( fUI_Msg < _TTX_EXIT_MESSAGE)&&(fUI_Msg!=_FL2UI_MAIN_NO_SIGNAL))//Hardy20091209
        fw_timer_event_ReactiveTimerEvent(ap_osdctrl_get_osd_timeout_ms(), timerevent_ui_table_OSD_timeout_event);
    SET_SLIDER_ON();
    switch (fUI_Msg)
    {
    case _LEFT_KEY_MESSAGE:
    case _VOLUME_DEC_MESSAGE:
   // case _LEFT_IR_MESSAGE:
        ucValue = COsdCtrlMenuItemGetValue(tVolumeSlider.ucMenuItemID);
        if ( ucValue > tVolumeSlider.ucMax)
            ucValue = tVolumeSlider.ucMax;
        if ( ucValue < tVolumeSlider.ucMin)
            ucValue = tVolumeSlider.ucMin;
        if (ucValue > tVolumeSlider.ucMin)
        {
            ucValue--;
            COsdCtrlMenuItemHandler(tVolumeSlider.ucMenuItemID, ucValue);
            COsdCoreUpdatePoponSlider();
        }
        break;

    case _RIGHT_KEY_MESSAGE:
    case _VOLUME_INC_MESSAGE:
   // case  _RIGHT_IR_MESSAGE:
        ucValue = COsdCtrlMenuItemGetValue(tVolumeSlider.ucMenuItemID);
        if ( ucValue > tVolumeSlider.ucMax)
            ucValue = tVolumeSlider.ucMax;
        if ( ucValue < tVolumeSlider.ucMin)
            ucValue = tVolumeSlider.ucMin;
        if (ucValue < tVolumeSlider.ucMax)
        {
            ucValue++;
            COsdCtrlMenuItemHandler(tVolumeSlider.ucMenuItemID, ucValue);
            COsdCoreUpdatePoponSlider();
        }
        break;

    case _OSD_TIMEOUT_MESSAGE:
    case _MENU_KEY_MESSAGE:
    case _MENU_IR_MESSAGE:
    case _EXIT_KEY_MESSAGE:
    
    //RTD_Log(LOGGER_LEVEL,"\n _OSD_TIMEOUT_MESSAGE22 Tim bCheckCCStatus =%d \n",(UINT32)bCheckCCStatus);
        COsdCoreExitOsd();
        break;

    default:
        COsdCoreDispatchMsg();
        break;
    }

}
#ifdef _VIDEO_TV_SUPPORT

/**
 * To initial channel scan state.
 *
 */
void COsdCoreInitChannelScanState(void)
{
    if ( GET_TV_MANUAL_SCAN() )
    {
        pItemObj = &tManualScanItem;
    }
#if defined(CONFIG_CHANNEL_TABLE_SCAN)
    else if ( GET_TV_TABLE_SCAN() )
    {
        ucOsdCoreBuf[7] = 0;
        pItemObj = &tTableScanItem;
    }
#endif
    else
    {
        pItemObj = &tChannelScanItem;
    }

    COsdCoreInitPoponSlider(_OPT_CHANNEL_SCAN_MODE);
    if ( GET_TV_MANUAL_SCAN() )
        ucOsdState = _MANUAL_SCAN_STATE;
    else
        ucOsdState = _AUTO_SCAN_STATE;
}
#endif

/**
 *
 *
 */
void COsdCoreInitAutoAdjustState(UINT8 msg)
{
    COsdFxDisableOsd();
    COsdCoreDispMessageBox(msg);
    if (GET_RTK_FACTORY_MODE()==0)
        ucOsdState = _AUTO_ADJUST_STATE;
}


/**
 * Auto adjust message event handler
 * @Param none
 * @return none
 */
void COsdCoreAutoAdjustEventProc(void)
{
    switch (fUI_Msg)
    {
    case _EXIT_KEY_MESSAGE:
        RTD_Log(LOGGER_INFO, "osdcore exit------------------------------------------------");
        if (GET_MAINMENU_ON())
            COsdCoreFallBackMenu();
        else
            COsdCoreExitOsd();
        break;

    case _OSD_TIMEOUT_MESSAGE:
        COsdCoreExitOsd();
        break;

    default:
        break;
    }
}


/**
 * Back to upper layer
 * @param None
 * @return None.
 */
void COsdCoreFallBackMenu(void)
{
    //UINT8  ucindex;//For Keil Building
    if ( ucOsdState != _MAINMENU_STATE)
    {
        if ( ucOsdState == _LISTITEM_STATE)
        { // listitem state doesn't need to redraw main menu frame
            //COsdFxShowWindow(tHighLightWindowStyle.ucWindowID, _FALSE);
            //COsdCoreMoveHighlight(stLayerStack[ucLayerIndex].ucMainMenu, stLayerStack[ucLayerIndex].ucSubMenu, _REFRESH_SUB);
            COsdCoreDrawMenuItems(stLayerStack[ucLayerIndex].pMenuItem, MAINMENU_GET_ITEM_ID(stLayerStack[ucLayerIndex].ucSubMenu),_TRUE);
           // OSDLine(5+ucListSelectedItem*2, 7, 31, 0x00, _WRITE_BYTE1);
            //DrawSelectLine(stLayerStack[ucLayerIndex].ucSubMenu);
            DrawSelectMenuItem(stLayerStack[ucLayerIndex].pMenuItem, MAINMENU_GET_ITEM_ID(stLayerStack[ucLayerIndex].ucSubMenu), _SELECT_COLOR);
        }
        else
        {
            ap_osdfx_map_setting(_OSD_MAP0);
            COsdCoreDrawMainMenuFrame(stLayerStack[ucLayerIndex].ucMainMenu);
            COsdCoreDispMenuTitle(stLayerStack[ucLayerIndex].pMenuItem);
            COsdCoreMoveHighlight(stLayerStack[ucLayerIndex].ucMainMenu, stLayerStack[ucLayerIndex].ucSubMenu, _REFRESH_SUB);
            ClearSelectLine();
            COsdFxEnableOsd();

        }
        ucOsdState = _MAINMENU_STATE;
        return;
    }
#ifdef SUPPORT_FIVE_KEY
    if(ucOsditem_choose == _TRUE)
    {
        ucOsditem_choose = _FALSE;
	if(fUI_Msg==_MENU_KEY_MESSAGE)
      DrawAdjustMenuItemText(stLayerStack[ucLayerIndex].pMenuItem,MAINMENU_GET_ITEM_ID(stLayerStack[ucLayerIndex].ucSubMenu),_SELECT_COLOR);
   }
    else
    {
        if ( ucLayerIndex != 0)
        { // at subpage
            ClearSelectLine();
            ucLayerIndex--;
            //COsdCoreDispMenuTitle(stLayerStack[ucLayerIndex].pMenuItem);
            COsdCoreDrawMenuItems(stLayerStack[ucLayerIndex].pMenuItem, MAINMENU_GET_ITEM_ID(stLayerStack[ucLayerIndex].ucSubMenu),_TRUE);
            DrawSelectMenuItem(stLayerStack[ucLayerIndex].pMenuItem, MAINMENU_GET_ITEM_ID(stLayerStack[ucLayerIndex].ucSubMenu), _SELECT_COLOR);
        }
        else
        {
            if ( MAINMENU_GET_SUB_ACTIVE(stLayerStack[ucLayerIndex].ucSubMenu) != _FALSE)
            {
                COsdCoreMoveHighlight(stLayerStack[ucLayerIndex].ucMainMenu, (stLayerStack[ucLayerIndex].ucSubMenu & ~_SUBMENU_ACTIVE), _REFRESH_NONE);
                ClearSelectLine();
                DrawSelectMenuItem(stLayerStack[ucLayerIndex].pMenuItem, stLayerStack[ucLayerIndex].ucSubMenu, _NORMAL_COLOR);
            }
            else
                COsdCoreExitOsd();
        }
    }
#else
    if ( ucLayerIndex != 0)
    { // at subpage
       // ClearSelectLine();
        ucLayerIndex--;
        //COsdCoreDispMenuTitle(stLayerStack[ucLayerIndex].pMenuItem);
        COsdCoreDrawMenuItems(stLayerStack[ucLayerIndex].pMenuItem, MAINMENU_GET_ITEM_ID(stLayerStack[ucLayerIndex].ucSubMenu),_TRUE);
    //    DrawSelectLine(stLayerStack[ucLayerIndex].ucSubMenu);
        DrawSelectMenuItem(stLayerStack[ucLayerIndex].pMenuItem, MAINMENU_GET_ITEM_ID(stLayerStack[ucLayerIndex].ucSubMenu), _SELECT_COLOR);
    }
    else
    {
        if ( MAINMENU_GET_SUB_ACTIVE(stLayerStack[ucLayerIndex].ucSubMenu) != _FALSE)
        {
            COsdCoreMoveHighlight(stLayerStack[ucLayerIndex].ucMainMenu, (stLayerStack[ucLayerIndex].ucSubMenu & ~_SUBMENU_ACTIVE), _REFRESH_NONE);
            ClearSelectLine();
            DrawSelectMenuItem(stLayerStack[ucLayerIndex].pMenuItem, stLayerStack[ucLayerIndex].ucSubMenu, _NORMAL_COLOR);
        }
        else
            COsdCoreExitOsd();
    }
#endif
}
#if 0//1 Tim 0406
/**
 * To draw main menu frame.
 *
 */
void COsdCoreDrawMainMenuFrame(UINT8 ucMainID)
{
    UINT8 i,row;
    UINT8 *tIcon;
    OSDLine(ROW(3), COL(1), LENGTH(37), 0xA6, _WRITE_BYTE0);
    OSDLine(ROW(3), COL(1), LENGTH(37), 0xA4, _WRITE_BYTE1);
    OSDLine(ROW(3), COL(1), LENGTH(37), 0x11, _WRITE_BYTE2);

    OSDLine(ROW(16), COL(1), LENGTH(37), 0xA6, _WRITE_BYTE0);
    OSDLine(ROW(16), COL(1), LENGTH(37), 0xA4, _WRITE_BYTE1);
    OSDLine(ROW(16), COL(1), LENGTH(37), 0x11, _WRITE_BYTE2);

    DrawLeftLine();

    for (i=0; i<6; i++)
    {
        SETCOLOR_SUBMENU_SELLINE(SHOWINDEX_TO_LINE(i) + 1);
    }

    for (i=0; i<5; i++)
    {

        if (ucMainID==i)
            tIcon = Icons_Select[i];
        else
            tIcon = Icons_normale[i];
        if ( ( i == MAINMENU_VGA && GET_INPUTSOURCE_TYPE() != _SOURCE_VGA )||\
                ( i == MAINMENU_CHANNEL && GET_INPUTSOURCE_TYPE() != _SOURCE_TV))
            tIcon = Icons_disable[i];
        row = 4+i*2;
        Draw2bit3x2Icon(tIcon,  row,  1);
    }

    COsdFxDrawWindow(&tMainWindowStyle);

    // set osd position
    COsdFxSetWinRefPos(_OSD_MAINMENU_H_POS, _OSD_MAINMENU_V_POS);
}
#endif
void InitOsdFrame(void)
{
    CCloseCCStatus();
    COsdFxDisableOsd();
    //ucShowSleepFlag = 0;

    //ap_osdfx_map_setting(_OSD_MAP0);//Edit SVN53
    // Load OSD palette
    #ifdef CONFIG_VBI_CC_SUPPORT
    ap_osdfx_reload_osdpalette();
    #endif
    // Load Font
    if(bloadfont)
    {
    	ap_osdfx_load_vlc_font(tFONT_GLOBAL, 0x00, 0x68);//Edit SVN53
    	ap_osdfx_load_vlc_font(tfont_2biticon, _2BITICON_FONT_ADDR, 76);
    	ap_osdfx_load_vlc_font(tFont_OTHERSIGN, _SIGN_ADDR, FntSIGN_NUM);
	
    	InitLanguagefont();
    	bloadfont = 0;
    }

 	ap_osdfx_load_vlc_font(FntLangaugeName, FntLangaugeName_StartAddr, FntLangaugeName_NUM);   
    // Init osd
    // insert code to here
    OSDClear(ROW(0), HEIGHT(18), COL(0), WIDTH(46), 0x8C, _WRITE_BYTE0);
    OSDClear(ROW(0), HEIGHT(18), COL(0), WIDTH(46), 0x00, _WRITE_BYTE1);
	OSDClear(ROW(0), HEIGHT(18), COL(0), WIDTH(46), 0x40, _WRITE_BYTE2);
}

/**
 * To Initialze top main menu environment to show
 * @param None
 * @return None.
 */
void COsdCoreInitMainMenu(void)
{
    ucLayerIndex = 0;
    stLayerStack[ucLayerIndex].ucMainMenu = 0;
    stLayerStack[ucLayerIndex].ucSubMenu = 0;
    if((GET_INPUTSOURCE_INDEX() == _SOURCE_INDEX_AV)||(GET_INPUTSOURCE_INDEX() == _SOURCE_INDEX_AV_1)||(GET_INPUTSOURCE_INDEX() == _SOURCE_INDEX_AV_2)||(GET_INPUTSOURCE_INDEX() == _SOURCE_INDEX_AV_3))
    {
    	stLayerStack[ucLayerIndex].pMenuItemSet = &stAVMainMenuItemSet;
    	stLayerStack[ucLayerIndex].pMenuItem = stAVMainMenuItemSet.ppMenuItems[stLayerStack[ucLayerIndex].ucSubMenu];
    }
    else if(GET_INPUTSOURCE_INDEX() == _SOURCE_INDEX_VGA)
    {
        stLayerStack[ucLayerIndex].pMenuItemSet = &stVGAMainMenuItemSet;
    	stLayerStack[ucLayerIndex].pMenuItem = stVGAMainMenuItemSet.ppMenuItems[stLayerStack[ucLayerIndex].ucSubMenu];	
    }
    else
    {
    	stLayerStack[ucLayerIndex].pMenuItemSet = &stHDMIMainMenuItemSet;
    	stLayerStack[ucLayerIndex].pMenuItem = stHDMIMainMenuItemSet.ppMenuItems[stLayerStack[ucLayerIndex].ucSubMenu];    	
    }
	InitOsdFrame();
	//fw_osdcore_InitProfile();
    //COsdFxDisableOsd();
    SetOSDDouble(0);
    
    COsdCoreDrawMainMenuFrame(stLayerStack[ucLayerIndex].ucMainMenu);
    // To display the title string
    COsdCoreDispMenuTitle(stLayerStack[ucLayerIndex].pMenuItemSet->ppMenuItems[stLayerStack[ucLayerIndex].ucMainMenu]);
    // To draw menu items
    COsdCoreDrawMenuItems(stLayerStack[ucLayerIndex].pMenuItemSet->ppMenuItems[stLayerStack[ucLayerIndex].ucMainMenu], MAINMENU_GET_ITEM_ID(stLayerStack[ucLayerIndex].ucSubMenu),_TRUE);
    
	#if CONFIG_ENABLE_INFO
 	COsdFxLoadText(17, 1, 50, FONT_COLOR(1, _CP_BLACK), 0, tOsdResolutionStr);
	#else
	CShowMode(0,17,12);
	#endif
    RTD_Log(LOGGER_INFO, "\nDraw menu item");

//fw_osdcore_PrintProfile();
}

void COsdCoreEnableMainMenu(void)
{
    COsdFxEnableOsd();

    SET_MAINMENU_ON();
    ucOsdState = _MAINMENU_STATE;
#if defined(CONFIG_MAINICON_4BITS_ANIMATION)
    fw_timer_event_ReactiveTimerEvent(SEC(2), COsdCoreIconAnimationTimerEvent);
#endif
}

/**
* To display menu hint string.
* @param
* @return none.
void COsdCoreDispMenuHint(UINT8 opt)
{
	UINT8 ucCnt;

	// Blank command used 1
	COsdFxLoadBlankCmd(18, 0, 82, _CP_BLACK);
	ucCnt = 1;
	if ( opt == _MENU_HINT_MAINICON) {
		ucCnt += COsdFxLoadText(18, ucCnt, 0, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tMenuHint_MainIcon);
	}
	else if ( opt == _MENU_HINT_SLIDER || opt == _MENU_HINT_BRACKET)
	{
		ucCnt += COsdFxLoadText(18, ucCnt, 0, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tOsdUpDownButtonStr);
		ucCnt += COsdFxLoadText(18, ucCnt, 8, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tOsdSelectStr);
		ucCnt += COsdFxLoadText(18, ucCnt, 24, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tOsdLeftRightButtonStr);
		ucCnt += COsdFxLoadText(18, ucCnt, 8, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tOsdAdjustStr);
		ucCnt += COsdFxLoadText(18, ucCnt, 24, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tOsdMenuButtonStr);
		ucCnt += COsdFxLoadText(18, ucCnt, 8, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tOsdReturnStr);
	}
	else if (opt == _MENU_HINT_LIST || opt == _MENU_HINT_SUBPAGE)
	{
		ucCnt += COsdFxLoadText(18, ucCnt, 0, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tOsdUpDownButtonStr);
		ucCnt += COsdFxLoadText(18, ucCnt, 8, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tOsdSelectStr);
		ucCnt += COsdFxLoadText(18, ucCnt, 24, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tOsdOkRightButtonStr);
		ucCnt += COsdFxLoadText(18, ucCnt, 8, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tOsdEnterStr);
		ucCnt += COsdFxLoadText(18, ucCnt, 24, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tOsdMenuButtonStr);
		ucCnt += COsdFxLoadText(18, ucCnt, 8, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tOsdReturnStr);
	}
	else if (opt == _MENU_HINT_LIST_SELECT)
	{
		ucCnt += COsdFxLoadText(18, ucCnt, 0, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tOsdUpDownButtonStr);
		ucCnt += COsdFxLoadText(18, ucCnt, 8, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tOsdSelectStr);
		ucCnt += COsdFxLoadText(18, ucCnt, 24, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tOsdOkRightButtonStr);
		ucCnt += COsdFxLoadText(18, ucCnt, 8, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tOsdConfirmStr);
		ucCnt += COsdFxLoadText(18, ucCnt, 24, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tOsdMenuButtonStr);
		ucCnt += COsdFxLoadText(18, ucCnt, 8, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tOsdReturnStr);
	}

	if ( ucCnt < _OSD_ITEM_ROW_LENGTH)
		COsdFxClearText(18, ucCnt, 1, _OSD_ITEM_ROW_LENGTH-ucCnt);
}
*/
#if 1
/**
 * To display menu title string.
 * @param pointer of MenuItem
 * @return none.
 */
void COsdCoreDispMenuTitle(MenuItem* pMenuItem)
{
    UINT8 Width,x,n,i,id;
    UINT8 code *tFont;
    id =pMenuItem->ucMenuID;
    tFont = tMainTitle[id][COsdCtrlGetLanguage()].Font;
    Width = tMainTitle[id][COsdCtrlGetLanguage()].Width;
    WaitFor_DEN_STOP();
    COsdFxClearText(1, 0, 1, _OSD_TITLE_ROW_LENGTH-10);
    WaitFor_DEN_STOP();
    COsdFxClearText(2, 0, 1, _OSD_TITLE_ROW_LENGTH-10);

    ap_osdfx_load_vlc_font(tFont, 0xDC/*0xE8*/,Width*2);

    x = (39 - Width)/2;
    n = 0xDC;//0xE8;
    ap_osdfx_calc_start_addr(1,  x, _WRITE_BYTE1);
    for (i=0; i<Width; i++)
    {
        RtdCtrl_CScalerSendAddr(0x0092,  n++, 1);
    }
    ap_osdfx_calc_start_addr(2,  x, _WRITE_BYTE1);
    for (i=0; i<Width; i++)
    {
        RtdCtrl_CScalerSendAddr(0x0092,  n++, 1);
    }
}

#endif
void LoadLanguage_FONT(UINT8 ucPage)
{
  switch(COsdCtrlGetLanguage())
  	{
  case LNG_JAPAN: 
	LoadJAPAN_Font(ucPage);
	break;
  case LNG_CHI_S:
 	LoadCHI_S_Font(ucPage);
  	break;
default:
	break;
  }
}
void LoadJAPAN_Font(UINT8 ucPage)
{
    UINT8 code *tFont;
    UINT8 num;
    if (COsdCtrlGetLanguage()!= LNG_JAPAN)
        return;
    switch (ucPage)
    {
    case MAINMENU_COLOR:
        tFont = FntJapan_colorPage_S;
        num = FntJAPAN_COLORPAGE_NUM;
       break;
    case MAINMENU_AUDIO:
    case MAINMENU_VGA:
    case SUBMENU_OSD_SETTING:
    case MAINMENU_TOOL:
        tFont = FntJAPAN_FunctionPage_S;
        num = FntJAPAN_FUNPAGE_NUM;
      break;
    case MAINMENU_CHANNEL:
    case OTHER_PG:
        tFont = FntJAPANTVPage_S;
        num = FntJAPAN_TVPAGE_NUM;
        break;
    }

    ap_osdfx_load_vlc_font(tFont, FntCHI_S_StartAddr,num);
}
void LoadCHI_S_Font(UINT8 ucPage)
{
    UINT8 code *tFont;
    UINT8 num;
    if (COsdCtrlGetLanguage()!= LNG_CHI_S)
        return;
    switch (ucPage)
    {
    case MAINMENU_COLOR:
        tFont = FntColorPage_S;
        num = FntCHI_COLORPAGE_NUM;
        break;
    case SUBMENU_MUSIC_MODE:
    case MAINMENU_AUDIO:
        tFont = FntAudioPage_S;
        num = FntCHI_AUDIOPAGE_NUM;
        break;
    case MAINMENU_VGA:
        tFont = FntAdjustPage_S;
        num = FntCHI_ADJUSTPAGE_NUM;
        break;
    case SUBMENU_OSD_SETTING:
    case MAINMENU_TOOL:
        tFont = FntFuncPage_S;
        num = FntCHI_FUNPAGE_NUM;
        break;
    case MAINMENU_CHANNEL:
        tFont = FntTVPage_S;
        num = FntCHI_TVPAGE_NUM;
        break;
    case OTHER_PG:
        tFont = FntOtherPage_S;
        num = FntCHI_OTHERPAGE_NUM;
        break;
    }

    ap_osdfx_load_vlc_font(tFont, FntCHI_S_StartAddr,num);
}
void InitLanguagefont(void)
{
    UINT8 value;
    //RTD_Log(LOGGER_INFO, "COsdCtrlGetLanguage()=%x\n",(UINT32)COsdCtrlGetLanguage());
    value =COsdCtrlGetLanguage();
    switch (value)
    {
    case LNG_CHI_S:
        LoadLanguage_FONT(0);
        break;
    case LNG_KOREN:
#if(LNG_KOREN_EN)
        ap_osdfx_load_vlc_font(FntKor, FntKor_StartAddr,FntKor_NUM);
#endif
        break;

case LNG_RUSSIAN:
#if(LNG_RUSSIAN_EN)
        ap_osdfx_load_vlc_font(FntRus, FntRUN_StartAddr,FntRUN_NUM);
#endif
        break;

    }
}
void UpdateLanguagePage(void)
{
    UINT8 value;

    RTD_Log(LOGGER_INFO, "COsdCtrlGetLanguage()=%x\n",(UINT32)COsdCtrlGetLanguage());
    value =COsdCtrlGetLanguage();

    switch (value)
    {
    case LNG_CHI_S:
        LoadLanguage_FONT(MAINMENU_TOOL);
        break;
    case LNG_KOREN:
#if(LNG_KOREN_EN)
        ap_osdfx_load_vlc_font(FntKor, FntKor_StartAddr,FntKor_NUM);
#endif
        break;
case LNG_RUSSIAN:
#if(LNG_RUSSIAN_EN)
        ap_osdfx_load_vlc_font(FntRus, FntRUN_StartAddr,FntRUN_NUM);
#endif
        break;

    }
    COsdCoreDispMenuTitle(stLayerStack[ucLayerIndex].pMenuItemSet->ppMenuItems[stLayerStack[ucLayerIndex].ucMainMenu]);
    COsdCoreDrawMenuItems(stLayerStack[ucLayerIndex].pMenuItem, MAINMENU_GET_ITEM_ID(stLayerStack[ucLayerIndex].ucSubMenu),_TRUE);
   // DrawSelectLine(MAINMENU_GET_ITEM_ID(stLayerStack[ucLayerIndex].ucSubMenu));
}
void COsdCoreRtkFacUpdateSliderItem(UINT8 ucItemId)
{
    UINT8 ucRow;
    UINT8 ucCnt;
    UINT8 ucWidth;

    if (ucItemId >= stLayerStack[ucLayerIndex].pMenuItem->ucItemCount)
        return;

    ucCnt = 1;

    // to calculate font count of item string
    ucCnt += COsdFxCalTextCount(0, stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucItemId].pString);
    ucWidth = COsdFxCalTextWidth(0, stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucItemId].pString);
    if ( ucWidth < _FAC_ITEM_LEN)
        ucCnt += COsdFxCalBlankCmdCount(_FAC_ITEM_LEN-ucWidth);

    ucRow = 3+(ucItemId%_PAGE_ITEMS_);

    COsdFxLoadBlankCmd(ucRow, ucCnt, 12*(3+2), _CP_BLACK);
    ucCnt += 1;
    ucCnt += COsdCoreDrawSliderNumber(ucRow, ucCnt, _CP_WHITE<<4 | _CP_BLACK, (ItemSlider *)(stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucItemId].pStruct));
}
void COsdCoreRtkFacUpdateBracketItem(UINT8 ucItemId)
{
    UINT8 ucRow;
    UINT8 ucCnt;
    UINT8 ucWidth;
    UINT8* pStr;

    ucCnt = 1;

    if (ucItemId >= stLayerStack[ucLayerIndex].pMenuItem->ucItemCount)
        return;

    // to calculate font count of item string
    ucCnt += COsdFxCalTextCount(0, stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucItemId].pString);
    ucWidth = COsdFxCalTextWidth(0, stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucItemId].pString);
    if ( ucWidth < _FAC_ITEM_LEN)
        ucCnt += COsdFxCalBlankCmdCount(_FAC_ITEM_LEN-ucWidth);

    ucRow = 3+(ucItemId%_PAGE_ITEMS_);
    pItemObj = &(stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucItemId]);
    pStr = COsdCoreGetBracketString();
    ucWidth = COsdFxCalTextCount(0, pStr);
    ucWidth = (7) - ucWidth;
    ucWidth = ucWidth*12;
    COsdFxWrite1BitFont(ucRow, ucCnt, _FAC_L_ARROW_, _CP_WHITE<<4 | _CP_BLACK);
    ucCnt += 1;
    ucCnt += COsdFxLoadBlankCmd(ucRow, ucCnt, ucWidth, _CP_BLACK);
    ucCnt += COsdFxLoadText(ucRow, ucCnt, 0, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, pStr);
    COsdFxWrite1BitFont(ucRow, ucCnt, _FAC_R_ARROW_, _CP_WHITE<<4 | _CP_BLACK);
    ucCnt += 1;
    if (ucCnt < _OSD_ITEM_ROW_LENGTH)
        COsdFxClearText(ucRow, ucCnt, 1, _OSD_ITEM_ROW_LENGTH-ucCnt);
}
void COsdCoreRtkFacUpdateListItem(UINT8 ucItemId)
{
    UINT8 ucIdx;
    UINT8 ucCnt;
    UINT8 ucRow;
    UINT8 ucWidth;
    UINT8* pStr;

    ucCnt = 1;

    if (ucItemId >= stLayerStack[ucLayerIndex].pMenuItem->ucItemCount)
        return;

    // to calculate font count of item string
    ucCnt += COsdFxCalTextCount(0, stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucItemId].pString);
    ucWidth = COsdFxCalTextWidth(0, stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucItemId].pString);
    if ( ucWidth < _FAC_ITEM_LEN)
        ucCnt += COsdFxCalBlankCmdCount(_FAC_ITEM_LEN-ucWidth);

    ucRow = 3+(ucItemId%_PAGE_ITEMS_);
    pListItem = (ItemList *)(stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucItemId].pStruct);
    ucIdx = COsdCoreListItemValueToIdx(COsdCtrlMenuItemGetValue(pListItem->ucMenuItemID));
    pStr = pListItem->pItemList[ucIdx].pString;
    ucWidth = COsdFxCalTextCount(0, pStr);
    ucWidth = (7+2) - ucWidth;
    ucWidth = ucWidth * 12;
    COsdFxLoadBlankCmd(ucRow, ucCnt, ucWidth, _CP_BLACK);
    ucCnt += 1;
    ucCnt += COsdFxLoadText(ucRow, ucCnt, 0, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, pStr);
}
#if 1
void COsdCoreRtkFacDrawItems(UINT8 ucSelectedItem)
{
    UINT8 j;
    UINT8 ucCnt;
    UINT8 ucWidth;
    UINT8 ucItemID;
    //RTD_Log(LOGGER_LEVEL,"\nfac111 = %d\n",(UINT32)stLayerStack[0].pMenuItem);

    //RTD_Log(LOGGER_LEVEL,"\nfac222 = %d\n",(UINT32)&tMenuFactoryMode);

    //RTD_Log(LOGGER_LEVEL,"\nfac 333 = %d\n",(UINT32)stLayerStack[0].pMenuItem->pItemList[1].pString);
    
    //RTD_Log(LOGGER_LEVEL,"\nfac 444 = %d\n",(UINT32)tMenuFactoryMode.pItemList[1].pString);
    //RTD_Log(LOGGER_LEVEL,"\nfac 555 = %d\n",(UINT32)&tMenuFactoryMode.pItemList[1].pString);

    WaitFor_DEN_STOP();
    for (j=0; j<_PAGE_ITEMS_; j++)
    {
        // 耿玡狠跋办
        COsdFxLoadBlankCmd(3+j, 0, 20, _CP_BLACK);
        ucCnt = 1;
        ucPage = ucSelectedItem / _PAGE_ITEMS_;

        // Draw Items
        if ( ucPage*13+j < stLayerStack[ucLayerIndex].pMenuItem->ucItemCount)
        {
            ucItemID = ucPage*_PAGE_ITEMS_+j;
            // Draw item string
            ucCnt += COsdFxLoadText(3+j, ucCnt, 0, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucItemID].pString);
            COsdFxClearText(3+j, ucCnt, 1, 60-ucCnt);

            ucWidth = COsdFxCalTextWidth(0, stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucItemID].pString);
            if ( ucWidth < _FAC_ITEM_LEN)
                ucCnt += COsdFxLoadBlankCmd(3+j, ucCnt, _FAC_ITEM_LEN-ucWidth, _CP_BLACK);

            if (stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucItemID].ucType == _SLIDER_TYPE)
            {
                COsdCoreRtkFacUpdateSliderItem(ucItemID);
            }
            else if (stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucItemID].ucType == _BRACKET_TYPE
                     || stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucItemID].ucType == _BRACKET1_TYPE
                    )
            {
                COsdCoreRtkFacUpdateBracketItem(ucItemID);
            }
            else if (stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucItemID].ucType == _LIST_TYPE)
            {
                COsdCoreRtkFacUpdateListItem(ucItemID);
            }
            else if (stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucItemID].ucType == _IMMEDIATE_TYPE || stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucItemID].ucType == _SUBPAGE_TYPE)
            {
                ucCnt += COsdFxLoadBlankCmd(3+j, ucCnt, 192-8*12, _CP_BLACK);
                COsdFxWrite1BitFont(3+j, ucCnt, _R_BRACKET_, (_CP_WHITE<<4) | _CP_BLACK);
            }
            else
            {
                // no support item?
            }
        }
        else
        {
            COsdFxClearText(3+j, 1, 1, 60-1);
        }
    }
}
void COsdCoreRtkFacMoveHighlight(UINT8 ucSelectedItem)
{
    if (ucSelectedItem != stLayerStack[ucLayerIndex].ucSubMenu)
    {
        if (ucSelectedItem/_PAGE_ITEMS_ != stLayerStack[ucLayerIndex].ucSubMenu / _PAGE_ITEMS_)
        {
            COsdCoreRtkFacDrawItems(ucSelectedItem);
        }

        stItemPos = tFactory_mode_Highlight_WindowPos;
        stItemPos.usVStart += (ucSelectedItem%_PAGE_ITEMS_)*24;
        stItemPos.usVEnd += (ucSelectedItem%_PAGE_ITEMS_)*24;
        COsdFxMoveWindow(tSource_Select_Highlight_WindowStyle.ucWindowID, &stItemPos);
        if ( !COsdFxGetWindowEnable(tSource_Select_Highlight_WindowStyle.ucWindowID) )
            COsdFxShowWindow(tSource_Select_Highlight_WindowStyle.ucWindowID, _TRUE);

        stLayerStack[ucLayerIndex].ucSubMenu = ucSelectedItem;
        //COsdCoreRtkFacDispHint(stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucSelectedItem].ucType);
    }
}
void COsdCoreFacItemEventProc(void)
{
    UINT8 ucTmp;
    UINT8 ucVal;
    union
    {
        ItemSlider*		pSlider;
        ItemBracket*	pBracket;
//		ItemBracket1*	pBracket1;
    } ItemPt;

    if ( GET_FACTORY_MODE_MENU() )
        ucTmp = GET_MENUITEM_ID(stLayerStack[ucLayerIndex].ucSubMenu);
    else
        ucTmp = stLayerStack[ucLayerIndex].ucSubMenu;
    pItemObj = &(stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucTmp]);
    switch (fUI_Msg)
    {
    case _LEFT_KEY_MESSAGE:
    case _RIGHT_KEY_MESSAGE:
    case _LEFT_IR_MESSAGE:
    case _RIGHT_IR_MESSAGE:
        if (pItemObj->ucType == _SLIDER_TYPE)
        {
            ItemPt.pSlider = (ItemSlider *)(pItemObj->pStruct);
            if ( (ItemPt.pSlider->ucCtrl & _SLIDER_NONLINEAR) != 0)
            {
                COsdCtrlMenuItemHandler(ItemPt.pSlider->ucMenuItemID, fUI_Msg);
                if ( GET_FACTORY_MODE_MENU() )
                    COsdCoreFacUpdateSliderItem(ucTmp);
                else
                    COsdCoreRtkFacUpdateSliderItem(ucTmp);
            }
            else
            {

                ucVal = COsdCoreSliderOperation(ItemPt.pSlider);
                if (ucVal != COsdCtrlMenuItemGetValue(ItemPt.pSlider->ucMenuItemID))
                {
                    COsdCtrlMenuItemHandler(ItemPt.pSlider->ucMenuItemID, ucVal);
                    if ( GET_FACTORY_MODE_MENU() )
                        COsdCoreFacUpdateSliderItem(ucTmp);
                    else
                        COsdCoreRtkFacUpdateSliderItem(ucTmp);
                }
            }

        }
        else if ( pItemObj->ucType == _BRACKET_TYPE || pItemObj->ucType == _BRACKET1_TYPE)
        {
            ItemPt.pBracket = (ItemBracket *)(pItemObj->pStruct);
            ucVal = COsdCoreBracketOperation(ItemPt.pBracket);
            if ( ucVal != COsdCtrlMenuItemGetValue(ItemPt.pBracket->ucMenuItemID))
            {
                COsdCtrlMenuItemHandler( ItemPt.pBracket->ucMenuItemID, ucVal);
                if ( GET_FACTORY_MODE_MENU() )
                    COsdCoreFacUpdateBracketItem(ucTmp);
                else
                    COsdCoreRtkFacUpdateBracketItem(ucTmp);
            }
        }
        else if ( pItemObj->ucType == _LIST_TYPE)
        {
            pListItem = (ItemList *)(pItemObj->pStruct);
            if ((pListItem->ucMenuItemID==MENUITEM_TV_VIDEO_STD) || (pListItem->ucMenuItemID==MENUITEM_TV_AUDIO_STD))//Edit SVN53
				ucVal = COsdCtrlMenuItemGetValue(pListItem->ucMenuItemID);
            else
            	ucVal = COsdCoreListItemValueToIdx(COsdCtrlMenuItemGetValue(pListItem->ucMenuItemID));
            if ( fUI_Msg == _LEFT_KEY_MESSAGE)
            {
                if (ucVal == 0)
                    ucVal = pListItem->ucItemCount - 1;
                else
                    ucVal--;
            }
            else
            {
                ucVal++;
                if (ucVal > pListItem->ucItemCount - 1)
                    ucVal = 0;
            }
            COsdCtrlMenuItemHandler( pListItem->ucMenuItemID, pListItem->pItemList[ucVal].ucValue);
            if ( GET_FACTORY_MODE_MENU() )
                COsdCoreFacUpdateListItem(ucTmp);
            else
                COsdCoreRtkFacUpdateListItem(ucTmp);
        }
        else if ( pItemObj->ucType == _IMMEDIATE_TYPE)
        {
            ItemImmediate* pImmediate;
            if (fUI_Msg ==_LEFT_KEY_MESSAGE)
                return;
            pImmediate = (ItemImmediate *)(pItemObj->pStruct);
            COsdCtrlMenuItemHandler( pImmediate->ucMenuItemID, fUI_Msg);
        }
        else
        {
            RTD_Log(LOGGER_DEBUG, "Unsupport OSD item ???\n");
        }
        break;

    case _OK_KEY_MESSAGE:
   case _OK_IR_MESSAGE:
        if ( pItemObj->ucType == _IMMEDIATE_TYPE)
        {
            ItemImmediate* pImmediate;
            pImmediate = (ItemImmediate *)(pItemObj->pStruct);
            COsdCtrlMenuItemHandler( pImmediate->ucMenuItemID, fUI_Msg);
        }
        break;

    default:
        break;
    }
}

/**
 * RTK factory mode event handler.
 *
 */
void COsdCoreRtkFacMenuEventProc(void)
{
    UINT8 ucSelectedItem;

    ucSelectedItem = stLayerStack[ucLayerIndex].ucSubMenu;
    pItemObj = &(stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucSelectedItem]);
    switch (fUI_Msg)
    {
    case _OK_KEY_MESSAGE:
   case _OK_IR_MESSAGE:
    case _LEFT_KEY_MESSAGE:
    case _RIGHT_KEY_MESSAGE:
    case _LEFT_IR_MESSAGE:
    case _RIGHT_IR_MESSAGE:
        COsdCoreFacItemEventProc();
        break;

    case _UP_KEY_MESSAGE:
        if (ucSelectedItem == 0)
            ucSelectedItem = stLayerStack[ucLayerIndex].pMenuItem->ucItemCount - 1;
        else
            ucSelectedItem--;
        COsdCoreRtkFacMoveHighlight(ucSelectedItem);
        break;

    case _DOWN_KEY_MESSAGE:
        ucSelectedItem++;
        if (ucSelectedItem > stLayerStack[ucLayerIndex].pMenuItem->ucItemCount - 1)
            ucSelectedItem = 0;
        COsdCoreRtkFacMoveHighlight(ucSelectedItem);
        break;

    case _EXIT_KEY_MESSAGE:
    case _MENU_KEY_MESSAGE:
    case _MENU_IR_MESSAGE:
        CLR_RTK_FACTORY_MODE();
        
       // RTD_Log(LOGGER_LEVEL,"\n _SHOW_INFO_STATE bCheckCCStatus=%d \n",(UINT32)bCheckCCStatus);
        COsdCoreExitOsd();
        ap_osdfx_load_vlc_font(tfont_2biticon, _2BITICON_FONT_ADDR, 76);
        break;

    default:
        break;
    }
}

#endif
void CFacInit(void)
{    
    COsdFxDrawWindow(&tSource_Select_Title_WindowStyle);
    COsdFxMoveWindow(tSource_Select_Title_WindowStyle.ucWindowID, &tFactory_mode_Title_WindowPos);
    COsdFxDrawWindow(&tSource_Select_Window_Style);
    COsdFxMoveWindow(tSource_Select_Window_Style.ucWindowID, &tFactory_mode_WindowPos);
    COsdFxDrawWindow(&tSource_Select_Hint_WindowStyle);
    COsdFxMoveWindow(tSource_Select_Hint_WindowStyle.ucWindowID, &tFactory_mode_Hint_WindowPos);
    COsdFxDrawWindow(&tSource_Select_Highlight_WindowStyle);
    COsdFxMoveWindow(tSource_Select_Highlight_WindowStyle.ucWindowID, &tFactory_mode_Highlight_WindowPos);

}
/**
 * OSD mainmenu state handler
 * @return none.
 */
void COsdCoreMainMenuEventProc(void)
{
    UINT8 rel;
    //if ( fUI_Msg != _MAINICON_ANIME_STEP1)
    if (( fUI_Msg < _TTX_EXIT_MESSAGE)&&(fUI_Msg!=_FL2UI_MAIN_NO_SIGNAL))//Hardy20091209
    {
        fw_timer_event_CancelTimerEvent(timerevent_ui_table_OSD_timeout_event);
        rel = fw_timer_event_ReactiveTimerEvent(ap_osdctrl_get_osd_timeout_ms(), timerevent_ui_table_OSD_timeout_event);
   //     RTD_Log(LOGGER_INFO, "Timeout1 value1=%d, rel1=%d,fui_msg=%d\n",(UINT32)ap_osdctrl_get_osd_timeout_ms(), (UINT32)rel,(UINT32)fUI_Msg);
#if defined(CONFIG_MAINICON_4BITS_ANIMATION)
        fw_timer_event_ReactiveTimerEvent(SEC(4), COsdCoreIconAnimationTimerEvent);
#endif
    }

    switch (fUI_Msg)
    {
    case _MAINICON_ANIME_STEP1:
        COsdCoreMainIconAnime(stLayerStack[ucLayerIndex].ucMainMenu);
        break;


    case _UP_KEY_MESSAGE:
    case _DOWN_KEY_MESSAGE:
    case _LEFT_KEY_MESSAGE:
    case _LEFT_IR_MESSAGE:
    case _RIGHT_KEY_MESSAGE:
   case _RIGHT_IR_MESSAGE:
    case _REPEAT_KEY_MESSAGE:
    case _OK_KEY_MESSAGE:
   case _OK_IR_MESSAGE:
    case _MENU_KEY_MESSAGE:
        COsdCoreMenuDirectionKeyProc();
        break;

    case _0_KEY_MESSAGE:
    case _1_KEY_MESSAGE:
    case _2_KEY_MESSAGE:
    case _3_KEY_MESSAGE:
    case _4_KEY_MESSAGE:
    case _5_KEY_MESSAGE:
    case _6_KEY_MESSAGE:
    case _7_KEY_MESSAGE:
    case _8_KEY_MESSAGE:
    case _9_KEY_MESSAGE:
    //RTD_Log(LOGGER_LEVEL,"\n Tim pwd start000 \n");
    	//if(Ctest())	
        //RTD_Log(LOGGER_LEVEL,"\n Tim pwd start111 \n");
        ap_factory_mode_check_pwd();
        //CFacInit();
        break;

   // case _MENU_IR_MESSAGE:
//    case _MENU_KEY_MESSAGE:
    case _EXIT_KEY_MESSAGE:
	case _DISPLAY_RATIO_KEY_MESSAGE:
        CLR_SLIDER_ON();
        COsdCoreFallBackMenu();
        break;

    case _OSD_TIMEOUT_MESSAGE:
        COsdCoreExitOsd();
        break;

    case _VOLUME_DEC_MESSAGE:
    case _VOLUME_INC_MESSAGE:
    case _CHANNEL_DEC_MESSAGE:
    case _CHANNEL_INC_MESSAGE:
   case _SOURCE_IR_MESSAGE:
	case _SOURCE_KEY_MESSAGE:
	case _MUTE_KEY_MESSAGE:
    case _JUMP_KEY_MESSAGE:
    case _SLEEP_KEY_MESSAGE:
    case _DISPLAY_KEY_MESSAGE:
    case _WIDE_KEY_MESSAGE:
    case _VIDEO_KEY_MESSAGE:
    case _AUDIO_KEY_MESSAGE:
    case _CAPTION_KEY_MESSAGE:
    case _TELETEXT_KEY_MESSAGE:
    case _CC_KEY_MESSAGE:
    //case _DISPLAY_RATIO_KEY_MESSAGE:
    case _RETURN_KEY_MESSAGE:
	//case _DISPLAY_RATIO_IR_MESSAGE:	
        COsdCoreDispatchMsg();
        break;
    case _SHOW_OSD_MESSAGE:
        COsdFxEnableOsd();
        break;

    default:
        break;
    }
}

/**
 * To move focus on main icon.
 * @return The focus id of next available icon.
 */
UINT8 ap_osdcore_move_main_icon_focus(bit ucScrollUp)
{
    UINT8 ucTmpMain;
    UINT8 ucItemCount;
    UINT8 ucMenuID;

    ucTmpMain = stLayerStack[ucLayerIndex].ucMainMenu;
    ucItemCount = stLayerStack[ucLayerIndex].pMenuItemSet->ucItemCount;

ReScroll:
    if ( ucScrollUp)
    {
        ucTmpMain = (ucTmpMain + ucItemCount - 1) % ucItemCount;
    }
    else
    {
        ucTmpMain = (ucTmpMain + 1) % ucItemCount;
    }

    ucMenuID = stLayerStack[ucLayerIndex].pMenuItemSet->ppMenuItems[ucTmpMain]->ucMenuID;
    if ( ucMenuID == MAINMENU_VGA && GET_INPUTSOURCE_TYPE() != _SOURCE_VGA )
        goto ReScroll;
    if ( ucMenuID == MAINMENU_CHANNEL && GET_INPUTSOURCE_TYPE() != _SOURCE_TV)
        goto ReScroll;

    return ucTmpMain;
}


/**
 * To get next available ID.
 * @return menu item id
 */
UINT8 ap_osdcore_get_next_available_item(MenuItem *pMenuItem, UINT8 ucItemId, bit bNext)
{
    UINT8 ucItemCnt;
    UINT8 ucCtrlId;

    ucItemCnt = pMenuItem->ucItemCount;
    do
    {
        if ( bNext)
        {
            ucItemId = (ucItemId + 1) % (pMenuItem->ucItemCount);
        }
        else
        {
            ucItemId = (ucItemId + pMenuItem->ucItemCount - 1) % (pMenuItem->ucItemCount);
        }
        ucCtrlId = ap_osdcore_get_item_control_id(&(pMenuItem->pItemList[ucItemId]));
        if ( COsdCtrlMenuItemGetEnable(ucCtrlId) )
            break;
        ucItemCnt--;
    }
    while (ucItemCnt != 0);

    if ( ucItemCnt == 0)   // there is no available item ?!
    {
        return 0xff;
    }

    return ucItemId;
}

/**
 * Menu direction key handler.
 *
 */
void COsdCoreMenuDirectionKeyProc(void)
{
    MenuItem* pMenuItem;
    UINT8 ucTmpMain;
    UINT8 ucTmpSub;
    UINT8 ucType;
    UINT16 ucVal;
    UINT8 ucFontColor;
    ucTmpMain = stLayerStack[ucLayerIndex].ucMainMenu;
    ucTmpSub = stLayerStack[ucLayerIndex].ucSubMenu;
    pMenuItem = stLayerStack[ucLayerIndex].pMenuItem;
/*
if(GET_INPUTSOURCE_INDEX() == _SOURCE_INDEX_AV_1)
   {
	   stLayerStack[ucLayerIndex].pMenuItemSet = &stAVMainMenuItemSet;
	   stLayerStack[ucLayerIndex].pMenuItem = stAVMainMenuItemSet.ppMenuItems[stLayerStack[ucLayerIndex].ucSubMenu];
   }
*/
	

#ifdef SUPPORT_FIVE_KEY
    if(COsd_LRkey2UDkey() == _TRUE)
    {
        if(fUI_Msg == _LEFT_KEY_MESSAGE)
            fUI_Msg = _UP_KEY_MESSAGE;
        else if(fUI_Msg == _RIGHT_KEY_MESSAGE)
            fUI_Msg = _DOWN_KEY_MESSAGE;
    }
#endif
    switch (fUI_Msg)
    {
    case _UP_KEY_MESSAGE:
        if ( MAINMENU_GET_SUB_ACTIVE(ucTmpSub) == _FALSE)
        { // highlight at main icon
            if ( MAINMENU_GET_ITEM_ID(ucTmpSub) != 0)
                ucTmpSub = 0;
            ucTmpMain = ap_osdcore_move_main_icon_focus(TRUE);
        }
        else
        { // highlight at item area
            ucTmpSub = MAINMENU_GET_ITEM_ID(ucTmpSub); // get item id
            ucTmpSub = ap_osdcore_get_next_available_item(pMenuItem, ucTmpSub, FALSE);
            if ( ucTmpSub >= pMenuItem->ucItemCount)
            {
                ucTmpSub = 0;
                MAINMENU_CLR_SUB_ACTIVE(ucTmpSub);
            }
            else
            {
                MAINMENU_SET_SUB_ACTIVE(ucTmpSub);
            }
        }
        break;

    case _DOWN_KEY_MESSAGE:
	case _MENU_KEY_MESSAGE:
        if ( MAINMENU_GET_SUB_ACTIVE(ucTmpSub) == _FALSE)
        {
            if ( MAINMENU_GET_ITEM_ID(ucTmpSub) != 0)
                ucTmpSub = 0;
            ucTmpMain = ap_osdcore_move_main_icon_focus(FALSE);
        }
        else
        {
            ucTmpSub = MAINMENU_GET_ITEM_ID(ucTmpSub); // get item id
            ucTmpSub = ap_osdcore_get_next_available_item(pMenuItem, ucTmpSub, TRUE);
            if ( ucTmpSub >= pMenuItem->ucItemCount)
            {
                ucTmpSub = 0;
                MAINMENU_CLR_SUB_ACTIVE(ucTmpSub);
            }
            else
            {
                MAINMENU_SET_SUB_ACTIVE(ucTmpSub);
            }
        }
        break;

    case _LEFT_KEY_MESSAGE:
    case _LEFT_IR_MESSAGE:
    case _RIGHT_KEY_MESSAGE:
    case _RIGHT_IR_MESSAGE:
    case _OK_KEY_MESSAGE:
   case _OK_IR_MESSAGE:
        if ( MAINMENU_GET_SUB_ACTIVE(ucTmpSub) == _FALSE)
        {	// highlight at main icon
#ifdef SUPPORT_FIVE_KEY
            if (fUI_Msg == _OK_KEY_MESSAGE||fUI_Msg == _OK_IR_MESSAGE||fUI_Msg == _RIGHT_IR_MESSAGE)
#else
            if (fUI_Msg == _OK_KEY_MESSAGE||fUI_Msg == _OK_IR_MESSAGE||fUI_Msg == _RIGHT_KEY_MESSAGE ||fUI_Msg == _LEFT_KEY_MESSAGE)
#endif
            {
            RTD_Log(LOGGER_INFO,"enter the sub menu ---------------------------------------------\n");
                // to get available item
                ucTmpSub = MAINMENU_GET_ITEM_ID(ucTmpSub);
                ucType = ap_osdcore_get_item_control_id(&(pMenuItem->pItemList[ucTmpSub]));
				//RTD_Log(LOGGER_INFO,"melon7777==ucType--------------%d,ucTmpSub=%d\n",(UINT32)ucType,(UINT32)ucTmpSub);
                if ( !COsdCtrlMenuItemGetEnable(ucType) )
                	{
                	
                    ucTmpSub = ap_osdcore_get_next_available_item(pMenuItem, ucTmpSub, TRUE);
                	}
                if ( ucTmpSub >= pMenuItem->ucItemCount)
                {
                	
                    ucTmpSub = 0;
                    MAINMENU_CLR_SUB_ACTIVE(ucTmpSub);
                }
                else
                {
                    MAINMENU_SET_SUB_ACTIVE(ucTmpSub);
                }
            }
            else
                return;
        }
        else
        { // highlight at item area
        	//RTD_Log(LOGGER_INFO,"melon333333333-----------------------------------\n");
            ucTmpSub = MAINMENU_GET_ITEM_ID(ucTmpSub);
            ucType = pMenuItem->pItemList[ucTmpSub].ucType;
            ucFontColor =_SELECT_COLOR;
#ifdef SUPPORT_FIVE_KEY
        	if((ucOsditem_choose == _FALSE) && (ucType == _SLIDER_TYPE || ucType == _BRACKET_TYPE)&&(fUI_Msg == _OK_KEY_MESSAGE))
        	{

        		ucOsditem_choose = _TRUE;
		  if(COsd_LRkey2UDkey() == _TRUE)
		    {
		        if(fUI_Msg == _LEFT_KEY_MESSAGE)
		            fUI_Msg = _UP_KEY_MESSAGE;
		        else if(fUI_Msg == _RIGHT_KEY_MESSAGE)
		            fUI_Msg = _DOWN_KEY_MESSAGE;
		    }
        	}
#endif
	{
            if (ucType == _SLIDER_TYPE)
            {
                ItemSlider *pSlider;
                if (fUI_Msg != _OK_KEY_MESSAGE)
                {
                    SET_SLIDER_ON();
                    pSlider = (ItemSlider *)(pMenuItem->pItemList[ucTmpSub].pStruct);
					//RTD_Log(LOGGER_INFO,"melon4444pSlider--------------%d\n",(UINT32)pSlider->ucCtrl);
                    if ( (pSlider->ucCtrl & _SLIDER_POPON) != 0)
                    {
                        ucOsdState = _SLIDER_STATE;
                        pItemObj = &(pMenuItem->pItemList[ucTmpSub]);
                        COsdCoreInitPoponSlider(0);
                        return;
                    }
                    else if ( (pSlider->ucCtrl & _SLIDER_NONLINEAR) != 0)
                    {
                        COsdCtrlMenuItemHandler(pSlider->ucMenuItemID, fUI_Msg);
                        COsdCoreUpdateSliderItem(ucTmpSub,ucFontColor);
                    }
                    else
                    {
                        ucVal = COsdCoreSliderOperation(pSlider);
                        if (ucVal != COsdCtrlMenuItemGetValue(pSlider->ucMenuItemID))
                        {
                            COsdCtrlMenuItemHandler(pSlider->ucMenuItemID, ucVal);
                            COsdCoreUpdateSliderItem(ucTmpSub,ucFontColor);
                        }
                    }
                }
		  else
                            COsdCoreUpdateSliderItem(ucTmpSub,ucFontColor);
             }
            else if ( ucType == _BRACKET_TYPE || ucType == _BRACKET1_TYPE)
            {
              if (fUI_Msg != _OK_KEY_MESSAGE)  //DIABLE BY JUNEY 0809  FOR 5KEY
              {
                ItemBracket *pBracket;
                pItemObj = &(pMenuItem->pItemList[ucTmpSub]);
                pBracket = (ItemBracket *)(pItemObj->pStruct);
                ucVal = COsdCoreBracketOperation(pBracket);
                if ( ucVal != COsdCtrlMenuItemGetValue(pBracket->ucMenuItemID) )
                {
                    COsdCtrlMenuItemHandler( pBracket->ucMenuItemID, ucVal);
                    COsdCoreUpdateBracketItem(ucTmpSub,ucFontColor);
                }
              }
		  else
                    COsdCoreUpdateBracketItem(ucTmpSub,ucFontColor);
            }
            else if ( ucType == _LIST_TYPE)
            {
                if (fUI_Msg ==_LEFT_KEY_MESSAGE/* ||
                	fUI_Msg ==_RIGHT_KEY_MESSAGE ||*/

                	/*fUI_Msg ==_LEFT_IR_MESSAGE || 
                	fUI_Msg ==_RIGHT_IR_MESSAGE*/)
                    return;
                ucFirstInList = 1;
                pListItem = (ItemList *)(pMenuItem->pItemList[ucTmpSub].pStruct);
                COsdCoreInitListPage();
                //COsdCoreDispMenuHint(_MENU_HINT_LIST_SELECT);
                ucOsdState = _LISTITEM_STATE;
                return;
            }
            else if ( ucType == _IMMEDIATE_TYPE)
            {
                if (
					fUI_Msg ==_LEFT_KEY_MESSAGE || 
                	//fUI_Msg ==_RIGHT_KEY_MESSAGE ||
                	fUI_Msg ==_LEFT_IR_MESSAGE //|| 
                	//fUI_Msg ==_RIGHT_IR_MESSAGE)
                	)
                    return;
                COsdCtrlMenuItemHandler( ((ItemImmediate *)(pMenuItem->pItemList[ucTmpSub].pStruct))->ucMenuItemID, fUI_Msg);
                return;
            }
            else if ( ucType == _SUBPAGE_TYPE)
            {
                if (fUI_Msg ==_LEFT_KEY_MESSAGE || 
                	//fUI_Msg ==_RIGHT_KEY_MESSAGE ||
                	fUI_Msg ==_LEFT_IR_MESSAGE// || 
                	//fUI_Msg ==_RIGHT_IR_MESSAGE
                	)
                    return;
                ucLayerIndex++;
                stLayerStack[ucLayerIndex].pMenuItemSet = stLayerStack[ucLayerIndex-1].pMenuItemSet;
                stLayerStack[ucLayerIndex].ucMainMenu = stLayerStack[ucLayerIndex-1].ucMainMenu;
                stLayerStack[ucLayerIndex].ucSubMenu = 0;
                stLayerStack[ucLayerIndex].pMenuItem = (MenuItem *)(pMenuItem->pItemList[ucTmpSub].pStruct);
                // To display the title string
                //COsdCoreDispMenuTitle(stLayerStack[ucLayerIndex].pMenuItem);
                ucTmpSub = 0;	// set first item actived
                MAINMENU_SET_SUB_ACTIVE(ucTmpSub);
                COsdCoreMoveHighlight(ucTmpMain, ucTmpSub, _REFRESH_SUB);
                return;
            }

            MAINMENU_SET_SUB_ACTIVE(ucTmpSub);
           }
        }
        break;
    default:
        break;
    }
    COsdCoreMoveHighlight(ucTmpMain, ucTmpSub, _REFRESH_NONE);
}

/**
 * To move high light window.
 *
 */
void COsdCoreMoveHighlight(UINT8 ucMain, UINT8 ucSub, UINT8 bRefresh)
{
    UINT8 id,ucount;

    if ( stLayerStack[ucLayerIndex].ucMainMenu == ucMain && stLayerStack[ucLayerIndex].ucSubMenu == ucSub && !bRefresh)
        return;

    if (stLayerStack[ucLayerIndex].ucMainMenu != ucMain || (bRefresh&_REFRESH_MAIN))
    {
        COsdCoreSetMainActive(stLayerStack[ucLayerIndex].ucMainMenu, _FALSE);
        stLayerStack[ucLayerIndex].ucMainMenu = ucMain;
        stLayerStack[ucLayerIndex].pMenuItem = stLayerStack[ucLayerIndex].pMenuItemSet->ppMenuItems[ucMain];
        if (ucMain ==MAINMENU_TOOL)
            ap_osdfx_load_vlc_font(FntLangaugeName, FntLangaugeName_StartAddr, FntLangaugeName_NUM);

        COsdCoreDispMenuTitle(stLayerStack[ucLayerIndex].pMenuItem);
        COsdCoreDrawMenuItems(stLayerStack[ucLayerIndex].pMenuItem, MAINMENU_GET_ITEM_ID(ucSub), _TRUE);
        WaitFor_DEN_STOP();
        COsdCoreSetMainActive(stLayerStack[ucLayerIndex].ucMainMenu, _TRUE);
    }
    if (stLayerStack[ucLayerIndex].ucSubMenu != ucSub || (bRefresh&_REFRESH_SUB))
    {
        if ( MAINMENU_GET_SUB_ACTIVE(ucSub) == _FALSE)
        {
            //WaitFor_DEN_STOP();
            //COsdFxCloseWindow(tHighLightWindowStyle.ucWindowID);
            //WaitFor_DEN_STOP();
            //COsdCoreSetMainHighlight(_TRUE);
            //COsdCoreDispMenuHint(_MENU_HINT_MAINICON);
            if ( (bRefresh&_REFRESH_SUB) != 0)
        	{
            	COsdCoreDrawMenuItems(stLayerStack[ucLayerIndex].pMenuItem, MAINMENU_GET_ITEM_ID(ucSub),_TRUE);
		#ifdef SUPPORT_FIVE_KEY 
		if(fUI_Msg<_MENU_IR_MESSAGE)
             DrawAdjustMenuItemText(stLayerStack[ucLayerIndex].pMenuItem,MAINMENU_GET_ITEM_ID(ucSub),_SELECT_COLOR);
		else
             DrawSelectMenuItem(stLayerStack[ucLayerIndex].pMenuItem,MAINMENU_GET_ITEM_ID(ucSub),_SELECT_COLOR);
	     #else
             DrawSelectMenuItem(stLayerStack[ucLayerIndex].pMenuItem,MAINMENU_GET_ITEM_ID(ucSub),_SELECT_COLOR);
		 #endif
        	}
        }
        else
        {
            // if need to update page content
            id = GetShowIndex(MAINMENU_GET_ITEM_ID(ucSub));
            id =id/6;
            ucount = GetShowIndex(MAINMENU_GET_ITEM_ID(stLayerStack[ucLayerIndex].ucSubMenu));
            if (id!=ucount/6)
            {
                bRefresh = bRefresh | _REFRESH_SUB;
            }

            id =MAINMENU_GET_ITEM_ID(stLayerStack[ucLayerIndex].ucSubMenu);

            if ( MAINMENU_GET_ITEM_ID(ucSub) == 0) // first item
            {
                //sub page
                if (ucLayerIndex>0)
                    id  = MAINMENU_GET_ITEM_ID(stLayerStack[ucLayerIndex-1].ucSubMenu);
                OSDLine(5+(id%6*2), 7, 31, 0x00, _WRITE_BYTE1);
            }
            if ( (bRefresh&_REFRESH_SUB) != 0)
        	{
            	COsdCoreDrawMenuItems(stLayerStack[ucLayerIndex].pMenuItem, MAINMENU_GET_ITEM_ID(ucSub),_TRUE);
        	}
        	if((bRefresh&_REFRESH_SUB) == 0)
        	{
       		RTD_Log(LOGGER_INFO,"draw the     uc  Sub  menu\n");
           		DrawAdjustMenuItemText(stLayerStack[ucLayerIndex].pMenuItem,MAINMENU_GET_ITEM_ID(stLayerStack[ucLayerIndex].ucSubMenu),_NORMAL_COLOR);
            }
          //  ClearSelectLine();
          //  DrawSelectLine(ucSub);
	    #ifdef SUPPORT_FIVE_KEY 
		if(fUI_Msg<_MENU_IR_MESSAGE)
             DrawAdjustMenuItemText(stLayerStack[ucLayerIndex].pMenuItem,MAINMENU_GET_ITEM_ID(ucSub),_SELECT_COLOR);
		else
             DrawSelectMenuItem(stLayerStack[ucLayerIndex].pMenuItem,MAINMENU_GET_ITEM_ID(ucSub),_SELECT_COLOR);
	    #else
             DrawSelectMenuItem(stLayerStack[ucLayerIndex].pMenuItem,MAINMENU_GET_ITEM_ID(ucSub),_SELECT_COLOR);
	    #endif
        }
        stLayerStack[ucLayerIndex].ucSubMenu = ucSub;
    }

}


/**
 * To initial list menu OSD.
 *
 */
void COsdCoreInitListMenu(ItemList *pList)
{
    UINT8 j,count;
    UINT8 ucCnt;
    /*UINT8 row;*///For Keil Building
    //if (( fUI_Msg < _TTX_EXIT_MESSAGE)&&(fUI_Msg!=_FL2UI_MAIN_NO_SIGNAL))//Hardy20091209
    //    fw_timer_event_ReactiveTimerEvent(_OSD_DISPLAY_TIMEOUT/*ap_osdctrl_get_osd_timeout_ms()*/, timerevent_ui_table_OSD_timeout_event);
    ap_osdfx_map_setting(_OSD_MAP0);
    COsdFxSetWinRefPos(_OSD_LISTMENU_H_POS, _OSD_LISTMENU_V_POS);
    SetOSDDouble(_OFF);
    count = pListItem->ucItemCount;
    WaitFor_DEN_STOP();
    // use for loop ,get OSD garbage.
    //for(j=0;j<count;j++)
    //{
    //row = 3+2*j;
    //COsdFxLoadBlankCmd(row, 0, 2, _CP_BLACK);
    InitOsdFrame();
    COsdFxHLine(3, COL(2), LENGTH(10), 0x20, _WRITE_BYTE2);
    COsdFxHLine(5, COL(2), LENGTH(10), 0x20, _WRITE_BYTE2);
    COsdFxHLine(7, COL(2), LENGTH(10), 0x20, _WRITE_BYTE2);
    COsdFxHLine(9, COL(2), LENGTH(10), 0x20, _WRITE_BYTE2);
    COsdFxHLine(11, COL(2), LENGTH(10), 0x20, _WRITE_BYTE2);
    COsdFxHLine(13, COL(2), LENGTH(10), 0x20, _WRITE_BYTE2);
    COsdFxHLine(15, COL(2), LENGTH(10), 0x20, _WRITE_BYTE2);
    //}

    WaitFor_DEN_STOP();
    //COsdFxClearText(UINT8 ucRow, UINT8 ucCol, UINT8 ucHeight, UINT8 ucWidth)
#if 0//CONFIG_USB_CHECK
	//RTD_Log(LOGGER_INFO, "melon22222=%x\n",(UINT32)stUISignalSource);  
	if((stUISignalSource== _SOURCE_INDEX_USB||stUISignalSource== 6))
	{
		 //RTD_Log(LOGGER_INFO, "[SOURCE] melon111111111111111111\n");	
		 Send_IR((((~IR_89JDVD_ASK_USB)<<8)|IR_89JDVD_ASK_USB));
	}
#endif

    pListItem = pList;
//	ucCnt = COsdFxLoadText(0, 0, 30, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, pListItem->pString);
    if ((pListItem->ucMenuItemID==MENUITEM_TV_VIDEO_STD) || (pListItem->ucMenuItemID==MENUITEM_TV_AUDIO_STD))//Edit SVN53
        ucOsdCoreBuf[0] = COsdCtrlMenuItemGetValue(pListItem->ucMenuItemID);
	else
    	ucOsdCoreBuf[0] = COsdCoreListItemValueToIdx(COsdCtrlMenuItemGetValue(pListItem->ucMenuItemID));
    // draw list items
    for (j=0; j<pListItem->ucItemCount; j++)
    {
        COsdFxLoadBlankCmd(2+j*2, 0, 14, _CP_BLACK);
        ucCnt = 2;
        if ( j == ucOsdCoreBuf[0] )
        {
           // OSDLine(3+j*2, 2, 6, 0x0D ,_WRITE_BYTE1);
            ucCnt += COsdFxLoadText(2+j*2, ucCnt, 6, _SELECT_COLOR, 0, pListItem->pItemList[j].pString);
        }
        else
            ucCnt += COsdFxLoadText(2+j*2, ucCnt, 6, FONT_COLOR(4, _CP_BLACK), 0, pListItem->pItemList[j].pString);
    }
    stItemPos = tSourceWindowPos;
    stItemPos.usVEnd = (2+(pListItem->ucItemCount * 2)) * 18 + 4;
    COsdFxDrawWindow(&tSourceWindowStyle);
    COsdFxMoveWindow(tSourceWindowStyle.ucWindowID, &stItemPos);
    //COsdFxShowWindow(tSource_Select_Highlight_WindowStyle.ucWindowID, _TRUE);
    COsdFxEnableOsd();
}

/**
 * To set highlist position in list menu OSD.
 * @return none.
void COsdCoreSetListMenuHighlight(UINT8 idx)
{
	stItemPos = tSource_Select_Highlight_WindowStyle.stPosition;
	stItemPos.usVStart += idx * 24;
	stItemPos.usVEnd += idx * 24;
	COsdFxMoveWindow(tSource_Select_Highlight_WindowStyle.ucWindowID, &stItemPos);
}
 */


void UpdateListMenuColor(UINT8 ucFontColor)
{
    COsdFxLoadText(2+ucOsdCoreBuf[0]*2, 2, 6, ucFontColor, 0, pListItem->pItemList[ucOsdCoreBuf[0]].pString);
}
/**
 * To handle message at source select state.
 *
 */
void COsdCoreListMenuEventProc(void)
{
#ifdef SUPPORT_FIVE_KEY
    if(COsd_LRkey2UDkey() == _TRUE)
    {
        if(fUI_Msg == _LEFT_KEY_MESSAGE)
            fUI_Msg = _UP_KEY_MESSAGE;
        else if(fUI_Msg == _RIGHT_KEY_MESSAGE)
            fUI_Msg = _DOWN_KEY_MESSAGE;
    }
#endif
    if (( fUI_Msg < _TTX_EXIT_MESSAGE)&&(fUI_Msg!=_FL2UI_MAIN_NO_SIGNAL))//Hardy20091209
    	{
    		 // RTD_Log(LOGGER_LEVEL,"\n melon 1111111111111111\n");
       		 //fw_timer_event_ReactiveTimerEvent(_OSD_DISPLAY_TIMEOUT/*ap_osdctrl_get_osd_timeout_ms()*/, timerevent_ui_table_OSD_timeout_event);
			 fw_timer_event_CancelTimerEvent(timerevent_table_no_signal_event);
    	}

    switch (fUI_Msg)
    {
    case _UP_KEY_MESSAGE:
	case _DVD_PREV_MESSAGE:
		fw_timer_event_ReactiveTimerEvent(_OSD_DISPLAY_TIMEOUT/*ap_osdctrl_get_osd_timeout_ms()*/, timerevent_ui_table_OK_event);
        //CLRCOLOR_SUBMENU_SELLINE1(3+2*ucOsdCoreBuf[0]);
        UpdateListMenuColor(_NORMAL_COLOR);
        if (ucOsdCoreBuf[0] == 0)
            ucOsdCoreBuf[0] = pListItem->ucItemCount - 1;
        else
            ucOsdCoreBuf[0] -= 1;
       // DRAWCOLOR_SUBMENU_SELLINE1(3+2*ucOsdCoreBuf[0]);
        UpdateListMenuColor(_SELECT_COLOR);
        break;

    case _DOWN_KEY_MESSAGE:
	case _SOURCE_KEY_MESSAGE:
	case _DVD_NEXT_MESSAGE:
		fw_timer_event_ReactiveTimerEvent(_OSD_DISPLAY_TIMEOUT/*ap_osdctrl_get_osd_timeout_ms()*/, timerevent_ui_table_OK_event);
        //CLRCOLOR_SUBMENU_SELLINE1(3+2*ucOsdCoreBuf[0]);
        UpdateListMenuColor(_NORMAL_COLOR);
        ucOsdCoreBuf[0] += 1;
        if (ucOsdCoreBuf[0] > pListItem->ucItemCount - 1)
            ucOsdCoreBuf[0] = 0;
        //DRAWCOLOR_SUBMENU_SELLINE1(3+2*ucOsdCoreBuf[0]);
        UpdateListMenuColor(_SELECT_COLOR);
        break;

#ifdef SUPPORT_FIVE_KEY
	case _OK_KEY_MESSAGE:
   case _OK_IR_MESSAGE:
	case _RIGHT_IR_MESSAGE:
#else
	case _OK_KEY_MESSAGE:
   case _OK_IR_MESSAGE:
	case _RIGHT_KEY_MESSAGE:
	case _RIGHT_IR_MESSAGE:
#endif
        COsdCoreExitOsd();
        COsdCtrlMenuItemHandler(pListItem->ucMenuItemID, pListItem->pItemList[ucOsdCoreBuf[0]].ucValue);
        break;

    case _OSD_TIMEOUT_MESSAGE:
   /// case _SOURCE_KEY_MESSAGE:
    case _SOURCE_IR_MESSAGE:
	//case _MENU_IR_MESSAGE:
    //case _MENU_KEY_MESSAGE:
    case _EXIT_KEY_MESSAGE:
    case _WIDE_KEY_MESSAGE://Hardy20091218
    case _DISPLAY_RATIO_KEY_MESSAGE:
        COsdCoreExitOsd();
		#if 0
		if((GET_INPUTSOURCE_INDEX() == _SOURCE_INDEX_HDMI0)&&(info->state != _MODE_STATE_ACTIVE))//joker xie 判断HDMI	
		{
		  fw_timer_event_ActiveTimerEvent(SEC(0.1), timerevent_table_showloading);
		}
		#endif
        break;

    case _VOLUME_DEC_MESSAGE:
    case _VOLUME_INC_MESSAGE:
    case _CHANNEL_DEC_MESSAGE:
    case _CHANNEL_INC_MESSAGE:
    case _MUTE_KEY_MESSAGE:
    case _JUMP_KEY_MESSAGE:
    case _SLEEP_KEY_MESSAGE:
    case _DISPLAY_KEY_MESSAGE:
        //case _WIDE_KEY_MESSAGE:
    case _VIDEO_KEY_MESSAGE:
    case _AUDIO_KEY_MESSAGE:
    case _CAPTION_KEY_MESSAGE:
    case _TELETEXT_KEY_MESSAGE:
        COsdCoreDispatchMsg();
        break;
	//case _FL2UI_MAIN_NO_SIGNAL:
	//	 fw_timer_event_ReactiveTimerEvent(_OSD_DISPLAY_TIMEOUT/*ap_osdctrl_get_osd_timeout_ms()*/, timerevent_ui_table_OK_event);
    default:
        break;
    }
}

extern volatile UINT32 myTimer2;
extern volatile UINT32 myTimer3;
#ifdef CONFIG_VBI_CC_SUPPORT
BOOL Get_CC_Status()
{
    return enable_cc_verify;
}

void Set_CC_Status(BOOL on)
{
    enable_cc_verify = on;
}

void Enable_CC_Input()
{
    drv_isr_SelectRoute( ROUTE_SEL_VBI_SLICER, IRQ_ROUTE_IRQ1 );
    //drv_isr_ClearPending( SYS_ISR_VBI_SLICER );
    drv_isr_Enable( SYS_ISR_VBI_SLICER, _TRUE );

}

void Disable_CC_Input()
{
    drv_isr_Enable( SYS_ISR_VBI_SLICER, _FALSE );
    //drv_isr_ClearPending( SYS_ISR_VBI_SLICER );
}

#endif
extern UINT32 TTX_SRC_Parse_Read(UINT16 addr);
/**
 * OSD idle state handler
 * @return none.
 */
 

#ifdef CONFIG_VBI_CC_SUPPORT

extern void drvif_vbi_cc_verify_play_ypbpr (void);
extern void drvif_vbi_cc_verify_play(void);

#endif
#if DISABLE_OPTTION

void CGetSleepString()
{
    UINT8* pStr;

	switch(ucSleepType)
	{
		case 1:			pStr = tOsdSleep30Str;	break;
		case 2:			pStr = tOsdSleep60Str;	break;
		case 3:			pStr = tOsdSleep90Str;  break;
		case 4:			pStr = tOsdSleep120Str;	break;
		case 5:			pStr = tOsdSleep150Str; break;
		default:
		case 0:			pStr = tOsdOffStr;		break;
		
	}
    COsdFxLoadText(2, 2, 0, FONT_COLOR(15, _CP_BLACK), 0, pStr);
    COsdFxSetWinRefPos(_OSD_TVCHANNEL, 10);

}
#endif
#define _CC_OFF		0
#define _CC_1		1
#define _CC_2		2
#ifdef CONFIG_VBI_CC_SUPPORT

void COsdCoreShowCCType(UINT8 cc)
{
    UINT8* pStr;

#define _INFO_CC_ROW		(1)
    //UINT32 freq;
    //freq = GET_CHANNEL_FREQ(ch);
    //RTD_Log(LOGGER_INFO, "freq-------------------------------%x",(UINT32)freq);
	OSDLine(ROW(3), COL(1), LENGTH(37), 0xA6, _WRITE_BYTE0);
	OSDLine(ROW(3), COL(1), LENGTH(37), 0xA4, _WRITE_BYTE1);
	OSDLine(ROW(3), COL(1), LENGTH(37), 0x11, _WRITE_BYTE2);

	OSDLine(ROW(16), COL(1), LENGTH(37), 0xA6, _WRITE_BYTE0);
	OSDLine(ROW(16), COL(1), LENGTH(37), 0xA4, _WRITE_BYTE1);
	OSDLine(ROW(16), COL(1), LENGTH(37), 0x11, _WRITE_BYTE2);
    
    ap_osdfx_map_setting(_OSD_MAP0);
    SetOSDDouble(3);
    // display channel number
    OSDLine(ROW(1), COL(1), LENGTH(8), 0xF0, _WRITE_BYTE2);
    OSDLine(ROW(2), COL(1), LENGTH(8), 0xF0, _WRITE_BYTE2);

	switch(cc)
	{
		case _CC_1:		pStr = tOsdCC1Str;	break;
		case _CC_2:		pStr = tOsdCC2Str;	break;
		default:
		case _CC_OFF:	pStr = tOsdOffStr;	break;
		
	}
    COsdFxLoadText(2, 2, 0, FONT_COLOR(15, _CP_BLACK), 0, pStr);
    COsdFxSetWinRefPos(_OSD_TVCHANNEL, 10);

    COsdFxEnableOsd();
    fw_timer_event_ReactiveTimerEvent(SEC(2), timerevent_ui_table_OSD_timeout_event);
#undef _INFO_CC_ROW
}
#endif
//Tim 0404
//==============================================================================

void CCloseCCStatus()
{
#ifdef CONFIG_VBI_CC_SUPPORT

    bCcStartFlag = _FALSE;
    bCheckCCStatus = 0;//Tim 0328
    Disable_CC_Input();//Tim 0328
    Set_CC_Status(FALSE);
    ap_osdfx_reload_osdpalette();

#endif
}
//==============================================================================

//Tim 0418
//==============================================================================
#if 1
void COsdCoreShowDisplayRatioType(UINT8 DisplayType)
{
    UINT8* pStr;

	#define _INFO_DISPLAY_RATIO_ROW		(1)
	#ifdef CONFIG_VBI_CC_SUPPORT
    COsdVbiShowData(0);
    COsdVbiBlendingOperation(0);
    CLR_OSDVBI_INITIAL();
	#endif
    OSDLine(ROW(3), COL(1), LENGTH(37), 0xA6, _WRITE_BYTE0);
    OSDLine(ROW(3), COL(1), LENGTH(37), 0xA4, _WRITE_BYTE1);
    OSDLine(ROW(3), COL(1), LENGTH(37), 0x11, _WRITE_BYTE2);

    OSDLine(ROW(16), COL(1), LENGTH(37), 0xA6, _WRITE_BYTE0);
    OSDLine(ROW(16), COL(1), LENGTH(37), 0xA4, _WRITE_BYTE1);
    OSDLine(ROW(16), COL(1), LENGTH(37), 0x11, _WRITE_BYTE2);
    
    ap_osdfx_map_setting(_OSD_MAP0);
    SetOSDDouble(3);
    // display channel number
    OSDLine(ROW(1), COL(1), LENGTH(8), 0xF0, _WRITE_BYTE2);
    OSDLine(ROW(2), COL(1), LENGTH(8), 0xF0, _WRITE_BYTE2);

    switch(DisplayType)
    {
    	case RATIO_AUTO:     pStr = tStr_display_ratio_auto;  break;
        case RATIO_16_9:     pStr = tStr_display_ratio_16x9;  break;
        default:
        case RATIO_4_3:     pStr = tStr_display_ratio_4x3;  break;
        
    }
    COsdFxLoadText(2, 2, 0, FONT_COLOR(15, _CP_BLACK), 0, pStr);
    COsdFxSetWinRefPos(_OSD_TVCHANNEL, 10);

    COsdFxEnableOsd();
    fw_timer_event_ReactiveTimerEvent(SEC(2), timerevent_ui_table_OSD_timeout_event);
	#undef _INFO_DISPLAY_RATIO_ROW

}
#endif
//==============================================================================
#if 1

void COsdCoreShowDisplayDimmerType(UINT8 DisplayType)
{
    UINT8* pStr;
	COsdFxDisableOsd();
	#define _INFO_DISPLAY_dimmer_ROW		(1)
	#ifdef CONFIG_VBI_CC_SUPPORT
    COsdVbiShowData(0);
    COsdVbiBlendingOperation(0);
    CLR_OSDVBI_INITIAL();
	#endif
	//InitOsdFrame();
	ap_osdfx_load_vlc_font(tFONT_GLOBAL, 0x00, 0x68);//Edit SVN53
    OSDLine(ROW(3), COL(1), LENGTH(37), 0xA6, _WRITE_BYTE0);
    OSDLine(ROW(3), COL(1), LENGTH(37), 0xA4, _WRITE_BYTE1);
    OSDLine(ROW(3), COL(1), LENGTH(37), 0x11, _WRITE_BYTE2);

    OSDLine(ROW(16), COL(1), LENGTH(37), 0xA6, _WRITE_BYTE0);
    OSDLine(ROW(16), COL(1), LENGTH(37), 0xA4, _WRITE_BYTE1);
    OSDLine(ROW(16), COL(1), LENGTH(37), 0x11, _WRITE_BYTE2);
    
    ap_osdfx_map_setting(_OSD_MAP0);
    SetOSDDouble(3);
    // display channel number
    OSDLine(ROW(1), COL(1), LENGTH(8), 0xF0, _WRITE_BYTE2);
    OSDLine(ROW(2), COL(1), LENGTH(8), 0xF0, _WRITE_BYTE2);

    switch(DisplayType)
    {
    	case _ON:     pStr = tStr_display_Dimmer_on;  break;
        case _OFF:     pStr = tStr_display_Dimmer_off;  break;
        
    }
    COsdFxLoadText(2, 2, 0, FONT_COLOR(15, _CP_BLACK), 0, pStr);
    COsdFxSetWinRefPos(5, 150);

    COsdFxEnableOsd();
    fw_timer_event_ReactiveTimerEvent(SEC(2), timerevent_ui_table_OSD_timeout_event);
	#undef _INFO_DISPLAY_dimmer_ROW

}
#endif

void COsdCoreIdleEventProc(void)
{
    extern void TTX_SRC_PrepareToAcceptNextField();
//	UINT8 getVal;
    UINT8 rel;
#if(_SOURCE_LIST_TYPE == _SOURCE_LISTNOTMENU)    
    UINT8 cur_source;
#endif    
	#ifdef CONFIG_VBI_CC_SUPPORT
    const IRQ_ENABLE_TYPE ccIrqType = SYS_ISR_VBI_SLICER;
    const IRQ_ROUTE_TYPE  ccIrqRoute = ROUTE_SEL_VBI_SLICER;
    const IRQ_ROUTE_PATH  ccIrqRoutePath = IRQ_ROUTE_IRQ1;
	#endif
	//RTD_Log(LOGGER_INFO,"melon_fUI_Msg---------------%d\n",(UINT32)fUI_Msg);
    switch (fUI_Msg)
    {
	#ifdef CONFIG_VBI_CC_SUPPORT
	    case _CAPTION_KEY_MESSAGE:
	        //enable_cc_verify = !enable_cc_verify;
	        if (Get_CC_Status())
	            Set_CC_Status(FALSE);
	        else
	            Set_CC_Status(TRUE);

	        //if ( enable_cc_verify ) {
	        if ( Get_CC_Status() )
	        {
	            RTD_Log(LOGGER_INFO, "enable_cc_verify is TRUE \n");
	            /*
	            			drv_isr_SelectRoute( ccIrqRoute, ccIrqRoutePath );
	            			drv_isr_ClearPending( ccIrqType );
	            			drv_isr_Enable( ccIrqType, _TRUE );
	            */
	            Enable_CC_Input();
	        }
	        else
	        {
	            RTD_Log(LOGGER_INFO, "enable_cc_verify is FALSE \n");
	            /*
	            		drv_isr_Enable( ccIrqType, _FALSE );
	            		drv_isr_ClearPending( ccIrqType );;
	           		 */
	            Disable_CC_Input();
	        }
	        break;
		#endif // end of CONFIG_VBI_CC_SUPPORT

	#ifdef CONFIG_TTX_SUPPORT
	#ifndef CONFIG_VBI_CC_SUPPORT
	    case _CAPTION_KEY_MESSAGE:
	#endif
	    case _TELETEXT_KEY_MESSAGE:
        if (	!( (GET_INPUTSOURCE_TYPE()==_SOURCE_TV) || (GET_INPUTSOURCE_TYPE()==_SOURCE_CVBS) || (GET_INPUTSOURCE_TYPE()==_SOURCE_SV)
	#ifdef CONFIG_SCART_AUTO_SWITCH
            || (GET_INPUTSOURCE_TYPE()==_SOURCE_FULL_SCART) || (GET_INPUTSOURCE_TYPE()==_SOURCE_HALF_SCART)
	#endif
          ) || (info->ucMode_Curr != _MODE_576I) )
        {
            // display NO TT message
            fw_ui_message_push(_NO_TT_SIGNAL);
            break;
        }

	        // close osd
	        COsdCoreExitOsd();

	        // prepare NoTT timer event
	        ttx_noTT = 1; // initial TT OK
	        fw_timer_event_ReactiveTimerEvent(SEC(5), timerevent_ui_table_OSD_NoTT_event);

	        if (GET_INTERLACE_MODE())	// Disable Video compensate
	            rtdf_clearBits(DM_UZU_CTRL_0x1604, _BIT11);
	        // TTX use share memory
	        rtd_outb( SYS_SHARE_MEM_0x2da1, 0x00 );

	        // Set IMEM 8K to bank 4 starting from 0x2000~0x3FFF
	        drv_imem8k_Start( 4, 0x2000);
	        // Set IMEM 4K to common area from 0x1000 ~ 0x1FFF
	        drv_imem4k_Start( 4, 0x1000); // rtd_inl, rtd_outl, rtd_isr_inl, rtd_ise_outl, rosprintf
	        //rtdf_setBits(0x2860, _BIT31);  // enable timer2

	        // clear previous pending bit
	        EA=0;
	        rtdf_setBits( VBIPPR_INT_ST_REG_0x4008, _BIT6 );
	        EA=1;

	        TTX_Open();
	        if ( m_hTtx != NULL )
	        { // Open TTX success
	            TTX_Control(TTX_CMD_SHOW);
	            if ( _TELETEXT_KEY_MESSAGE == fUI_Msg )
	            {
	                TTX_Control(TTX_CMD_NORMAL);
	                ucOsdState = _TELETEXT_STATE;
	//->w				SET_ENABLE_TTX_10ms();
	            }
	            else if ( _CAPTION_KEY_MESSAGE == fUI_Msg )
	            {
	                TTX_Control(TTX_CMD_SUBTITLE);	/* turn on subtitle mode */
	                ucOsdState = _SUBTITLE_STATE;
	//->w				SET_ENABLE_TTX_10ms();
	            }

	            TTX_SRC_PrepareToAcceptNextField();	// Trigger the next PPR ISR
	        }
	        break;
	#endif // end of CONFIG_TTX_SUPPORT

	#if 1
	    case _SHOW_BOOT_MESSAGE:
        //COsdFxBlending(GET_OSD_BLENDING());
        //CTVRegAfcTrackingCallback(CTvAfcAutoTracking);
	        break;
	#if (_LOGOL_ENABLE)
	    case _FL2UI_SHOW_LOGO:
			COsdCoreDrawLogo(3);
			ucPowersavingdisablelogo = 0;
	        break;
	#endif

		case _FL2UI_LOAD_MAINFONT:
			COsdFxDisableOsd();
			ap_osdfx_load_vlc_font(tFONT_GLOBAL, 0x00, 0x60);
	        ap_osdfx_set_2bit_font_offset(_2BITICON_FONT_ADDR/2);
	        ap_osdfx_load_vlc_font(tfont_2biticon, _2BITICON_FONT_ADDR, 76);
	        COsdFxClearText(0, 0,18, 46);
			break;
		
	    case _OSD_TIMEOUT_MESSAGE:
	        COsdCoreExitOsd();
	        return;	// avoid to active OSD timeout event

	    case _DISPLAY_MESSAGE:
	        // flow inform ui to display source info OSD
	        //RTD_Log(LOGGER_LEVEL,"\n displayTim bccStartFlag=%d \n",(UINT32)bCcStartFlag);
			#ifdef CONFIG_VBI_CC_SUPPORT

	        bCcStartFlag = _FALSE;
	        bCheckCCStatus = 0;//Tim 0328
	        Disable_CC_Input();//Tim 0328
	        Set_CC_Status(FALSE);
	        #endif
	        InitOsdFrame();
			#if _MELON_DEBUG
			RTD_Log(LOGGER_INFO, "\n melon_zhang 3333333------------------------------------------------...\n");
			#endif
	       // ap_osdfx_reload_osdpalette();
	        COsdCoreSourceInfo(_TRUE);
	        fw_timer_event_ReactiveTimerEvent(SEC(5), timerevent_ui_table_OSD_timeout_event);
            //fw_timer_event_CancelTimerEvent(timerevent_ui_table_OSD_timeout_event);

	        ucOsdState = _SHOW_INFO_STATE;
	        break;

	    case _DISPLAY_KEY_MESSAGE:
	        //COsdCoreSourceInfo(_TRUE);
	        COsdCoreSourceInfo_key(1);
	        ucOsdState = _SHOW_INFO_STATE;
	        break;

	#ifdef _VIDEO_TV_SUPPORT
	    case _JUMP_KEY_MESSAGE:
	        if ( COsdCtrlGetSignalSource() != _SOURCE_INDEX_TV)
	        {
	            COsdCtrlSetSignalSource(_SOURCE_INDEX_TV);
	        }
	        else
	        {
	            UINT8 newChn;
	            newChn = CTvSelectChannel(_TV_LAST_CHANNEL);
	            CTvSetChannel(newChn);
	            COsdCoreShowTVChannel(newChn);
	            ucOsdState = _SHOW_INFO_STATE;
	        }
	        break;
	#endif

	    case _FL2UI_MAIN_NO_SIGNAL:
	        //	if((disp_info[_MAIN_DISPLAY].state != _MODE_STATE_ACTIVE)&&(disp_info[_SUB_DISPLAY].state != _MODE_STATE_ACTIVE))
	        {
	        	RTD_Log(LOGGER_INFO,"enter no singanl--------------------------\n");
	            COsdCoreDispMessageBox(INFORM_NO_SIGNAL);
	            ucOsdState = _SHOW_NO_SIGNAL_STATE;
	            fw_timer_event_CancelTimerEvent(timerevent_ui_table_OSD_timeout_event);
	        }
	    	break;
    	
	    case _FL2UI_MAIN_NO_SUPPORT:
			COsdCoreDispMessageBox(INFORM_NO_SUPPORT);
			ucOsdState = _SHOW_NO_SUPPORT_STATE;
			fw_timer_event_CancelTimerEvent(timerevent_ui_table_OSD_timeout_event);
	        break;

	    case _NO_TT_SIGNAL:
	        COsdCoreDispMessageBox(INFORM_NO_TT);
	        break;
	#if 0//1
		case _CC_KEY_MESSAGE:
	    if ((GET_INPUTSOURCE_TYPE()==_SOURCE_CVBS) ||
	    	(GET_INPUTSOURCE_TYPE()==_SOURCE_SV)   ||
	    	(GET_INPUTSOURCE_TYPE()==_SOURCE_TV)	)
	    {
			//RTD_Log(LOGGER_LEVEL,"\n CC Hot Key\n");
			#if 1
	    	CcType = GET_CLOSEDCAPTION();
	    	//RTD_Log(LOGGER_LEVEL,"\n CC get value = %d \n",(UINT32)GET_CLOSEDCAPTION());
	    	CcType ++;
	 		if(CcType > 2)
	 			CcType = 0;
	 		
	        COsdFxDisableOsd();
	        bCcStartFlag = _FALSE;
	        bCheckCCStatus = 0;//Tim 0328
	        Disable_CC_Input();//Tim 0328
	        ucOsdState = _MAINMENU_STATE;
	        COsdCoreShowCCType(CcType);
	        SET_CLOSEDCAPTION(CcType);
	        SYNC_STORAGE_DATA(StructSystemDataType,  CcSystem, CcType);
	        #else
	        bCcStartFlag = _FALSE;
	        bCheckCCStatus = 0;//Tim 0328
	        Disable_CC_Input();//Tim 0328
	        //RTD_Log(LOGGER_DEBUG,"Tim Cur Chn %d\n", (UINT32)GET_TV_CUR_CHN());
	        COsdCoreInitMainMenu();
	        COsdCoreEnableMainMenu();
	        //fw_timer_event_CancelTimerEvent(timerevent_ui_table_OSD_timeout_event);
	        fw_timer_event_ReactiveTimerEvent(ap_osdctrl_get_osd_timeout_ms(), timerevent_ui_table_OSD_timeout_event);
	        //RTD_Log(LOGGER_INFO, "Timeout0 value0=%x, rel0=%x\n",(UINT32)ap_osdctrl_get_osd_timeout_ms(), (UINT32)rel);
	        RTD_Log(LOGGER_INFO, "\n Tim cc key\n");


	        #endif
	    }
	    	break;
	#endif

		case _DISPLAY_RATIO_MESSAGE:
			{
					CCloseCCStatus();
		        	COsdCoreShowDisplayRatioType(GET_OSD_DISPLAY_RATIO());
			}
		break;
		case _DIMMER_KEY_MESSAGE:
			CCloseCCStatus();	
			COsdCoreShowDisplayDimmerType(g_dimmer);
			break;
	    case _MENU_KEY_MESSAGE:
	    case _MENU_IR_MESSAGE:
	        if (GET_FACTORY_MODE_MENU())
	        {
	            COsdCoreFacInitMenu();
	        }
	        else if (GET_RTK_FACTORY_MODE())
	        {
	            COsdCoreRtkFacInitMenu();
	        }
	        else
	        {
	            COsdCoreInitMainMenu();
	            COsdCoreEnableMainMenu();
	            fw_timer_event_CancelTimerEvent(timerevent_ui_table_OSD_timeout_event);
	            rel = fw_timer_event_ReactiveTimerEvent(ap_osdctrl_get_osd_timeout_ms(), timerevent_ui_table_OSD_timeout_event);
	            RTD_Log(LOGGER_INFO, "Timeout0 value0=%x, rel0=%x\n",(UINT32)ap_osdctrl_get_osd_timeout_ms(), (UINT32)rel);
	        }
	        break;

	    case _VOLUME_DEC_MESSAGE:
	    case _VOLUME_INC_MESSAGE:
#ifdef SUPPORT_FIVE_KEY
#else
    case _LEFT_KEY_MESSAGE:
    case _RIGHT_KEY_MESSAGE:
#endif
    //case _LEFT_IR_MESSAGE:
    //case _RIGHT_IR_MESSAGE:
	        pItemObj = &tVolumeItem;
	        ucOsdState = _VOLUME_STATE;
	        LoadLanguage_FONT(OTHER_PG);
	        if(!bearphone)
	        	ap_osdctrl_audio_mute(AUDIO_UNMUTE);	// user adjust the audio volume, let's unmute it
	        COsdCoreInitPoponSlider(_OPT_VOLUME_ADJ_MODE); // to indicate volume bar mode
			fw_timer_event_ReactiveTimerEvent(ap_osdctrl_get_osd_timeout_ms(), timerevent_ui_table_OSD_timeout_event);
	        break;

	    case _MUTE_KEY_MESSAGE:
	    	CCloseCCStatus();//Tim 0404
	        COsdCtrlSetAudioMute();
	        if ( GET_AUDIO_MUTE())
	            COsdCoreDrawMuteIcon(TRUE);
	        else
	            COsdCoreDrawMuteIcon(FALSE);
	        break;

	    case _SOURCE_KEY_MESSAGE:
	    case _SOURCE_IR_MESSAGE:
#if(_SOURCE_LIST_TYPE == _SOURCE_LISTNOTMENU) 
			
            cur_source = COsdCtrlGetSignalSource();
            COsdCtrlSetSignalSource(COsdGetNextSource(cur_source));
            //fw_ui_message_push(_DISPLAY_MESSAGE);
            #if _MELON_DEBUG
			RTD_Log(LOGGER_INFO, "\n melon_zhang 11111111------------------------------------------------...\n");
			#endif
            COsdCoreSourceInfo(_TRUE);
            fw_timer_event_CancelTimerEvent(timerevent_table_no_signal_event);
            if(1)//(COsdCtrlGetSignalSource() != _SOURCE_INDEX_HDMI0)//joker xie 判断HDMI
            {
            	fw_ui_message_clearallType(_FL2UI_SHOW_LOGO);
            	fw_timer_event_CancelTimerEvent(timerevent_table_showloading);
            }
#else
			
            //fw_timer_event_CancelTimerEvent(timerevent_table_showloading);
			ucOsdState = _LIST_MENU_STATE;
			//fw_timer_event_DelayXms(1000);

            COsdCoreInitListMenu(&tSourceSelectList);
			fw_timer_event_ReactiveTimerEvent(_OSD_DISPLAY_TIMEOUT/*ap_osdctrl_get_osd_timeout_ms()*/, timerevent_ui_table_OK_event);
#endif	    
			
			

	        break;

#if 0
	    case _VIDEO_KEY_MESSAGE:
	        uwOsdCoreModeKey = _VIDEO_KEY_MESSAGE;
	        ucOsdState = _MODE_BOX_STATE;
	        pListItem = &tPictureModeList;
	        COsdCoreDispModeBox(_OPT_REDRAW);
	        break;

	    case _AUDIO_KEY_MESSAGE:
	        uwOsdCoreModeKey = _AUDIO_KEY_MESSAGE;
	        ucOsdState = _MODE_BOX_STATE;
	        #ifdef _VIDEO_TV_SUPPORT
	        if ( COsdCtrlGetSignalSource() == _SOURCE_INDEX_TV)
	        {
	            pListItem = &tMTSModeList;
	        }
	        else
	        #endif
	        {
	            pListItem = &tEQModeList;
	        }
	        COsdCoreDispModeBox(_OPT_REDRAW);
	        break;
#endif
	#ifdef _VIDEO_TV_SUPPORT
		case _RETURN_KEY_MESSAGE:
			if ( GET_INPUTSOURCE_TYPE() == _SOURCE_TV) {
			UINT8 newChn;
			newChn = CTvSelectChannel(_TV_RETURN_CHANNEL);
			CTvSetChannel(newChn);
			COsdCoreShowTVChannel(newChn);
			//COsdCoreSourceInfo(_TRUE);
			ucOsdState = _SHOW_INFO_STATE;
			}
	    break;
#ifdef SUPPORT_FIVE_KEY	
    case _LEFT_KEY_MESSAGE:
        if(COsdCtrlGetSignalSource() != _SOURCE_INDEX_TV)
        {
            pItemObj = &tVolumeItem;
            ucOsdState = _VOLUME_STATE;
            ap_osdctrl_audio_mute(AUDIO_UNMUTE);    // user adjust the audio volume, let's unmute it
            COsdCoreInitPoponSlider(_OPT_VOLUME_ADJ_MODE); // to indicate volume bar mode
        }
        else
        {
            if(ucOsdState != _SHOW_INFO_STATE)
            {
                pItemObj = &tVolumeItem;
                ucOsdState = _VOLUME_STATE;
                ap_osdctrl_audio_mute(AUDIO_UNMUTE);    // user adjust the audio volume, let's unmute it
                COsdCoreInitPoponSlider(_OPT_VOLUME_ADJ_MODE); // to indicate volume bar mode
            }
            else
            {
                UINT8 newChn;
                newChn = CTvSelectChannel(_TV_NEXT_CHANNEL);
                if ( COsdCtrlGetSignalSource() == _SOURCE_INDEX_TV)
                {
                    CTvSetChannel(newChn);
                    //COsdCoreSourceInfo(_TRUE);
                    COsdCoreShowTVChannel(newChn);
                    ucOsdState = _SHOW_INFO_STATE;
                }
            }
        }
        break;
#endif
	    case _UP_KEY_MESSAGE:
	    case _CHANNEL_INC_MESSAGE:
		    {
		        UINT8 newChn;

		        newChn = CTvSelectChannel(_TV_NEXT_CHANNEL);
	            if ( COsdCtrlGetSignalSource() == _SOURCE_INDEX_TV)
	            {
	                CTvSetChannel(newChn);
	                //COsdCoreSourceInfo(_TRUE);
	                COsdCoreShowTVChannel(newChn);
	                ucOsdState = _SHOW_INFO_STATE;
	            }
		    }
		    break;

#ifdef SUPPORT_FIVE_KEY
    case _RIGHT_KEY_MESSAGE:
        if(COsdCtrlGetSignalSource() != _SOURCE_INDEX_TV)
        {
            pItemObj = &tVolumeItem;
            ucOsdState = _VOLUME_STATE;
            ap_osdctrl_audio_mute(AUDIO_UNMUTE);    // user adjust the audio volume, let's unmute it
            COsdCoreInitPoponSlider(_OPT_VOLUME_ADJ_MODE); // to indicate volume bar mode
        }
        else
        {       
            {
                UINT8 newChn;
                
                newChn = CTvSelectChannel(_TV_PREV_CHANNEL);
                if ( COsdCtrlGetSignalSource() == _SOURCE_INDEX_TV)
                {
                    CTvSetChannel(newChn);
                    //COsdCoreSourceInfo(_TRUE);
                    COsdCoreShowTVChannel(newChn);
                    ucOsdState = _CHN_KEYPUSH_STATE;
                }
            }

        }
        break;
#endif
	    case _DOWN_KEY_MESSAGE:
	    case _CHANNEL_DEC_MESSAGE:
		    {
		        UINT8 newChn;

		        newChn = CTvSelectChannel(_TV_PREV_CHANNEL);
	            if ( COsdCtrlGetSignalSource() == _SOURCE_INDEX_TV)
	            {
	                CTvSetChannel(newChn);
	                //COsdCoreSourceInfo(_TRUE);
	                COsdCoreShowTVChannel(newChn);
	                ucOsdState = _SHOW_INFO_STATE;
	            }
		    }
		    break;

	    case _0_KEY_MESSAGE:
	    case _1_KEY_MESSAGE:
	    case _2_KEY_MESSAGE:
	    case _3_KEY_MESSAGE:
	    case _4_KEY_MESSAGE:
	    case _5_KEY_MESSAGE:
	    case _6_KEY_MESSAGE:
	    case _7_KEY_MESSAGE:
	    case _8_KEY_MESSAGE:
	    case _9_KEY_MESSAGE:
	//	case _100_KEY_MESSAGE:
	        if (GET_INPUTSOURCE_TYPE() == _SOURCE_TV)
			{
	        ucChnNum = ap_osdcore_irkey_to_num(fUI_Msg);
	        // COsdCoreShowTVChannel(ucChnNum);
	        COsdCoreChnInputBox(ucChnNum, 1);

	        //COsdCoreChnInputBox(ucChnNum, _BIT0);
	        fw_timer_event_CancelTimerEvent(timerevent_ui_table_OSD_timeout_event);
	        fw_timer_event_ReactiveTimerEvent(2500, COsdChnInputTimeoutEvent);
	        ucOsdState = _CHN_INPUT_STATE;
			}
	        return;
	#endif
	#if 0
	    case _WIDE_KEY_MESSAGE:
	        ucOsdState = _LIST_MENU_STATE;
	        COsdCoreInitListMenu(&tDisplayRatioList);
	        break;
	#endif

	    case _EXIT_KEY_MESSAGE:
	    
	    //RTD_Log(LOGGER_LEVEL,"\n _OSD_TIMEOUT_MESSAGE44 Tim bCheckCCStatus =%d \n",(UINT32)bCheckCCStatus);
	        COsdCoreExitOsd();
	        break;
	#endif
	    default:
	        break;
    }
/*
	#ifndef CONFIG_TTX_SUPPORT
	    if (ucOsdState != _SHOW_NO_SIGNAL_STATE && GET_INPUTSOURCE_TYPE() != _SOURCE_TV)
	#else
	    if ((ucOsdState != _SHOW_NO_SIGNAL_STATE)&&(ucOsdState != _SHOW_NO_SUPPORT_STATE)&&(fUI_Msg!=_TTX_EXIT_MESSAGE)&&(fUI_Msg!=_TTX_SIGNAL_OK_MESSAGE))
	#endif
        fw_timer_event_ReactiveTimerEvent(_OSD_DISPLAY_TIMEOUT, timerevent_ui_table_OSD_timeout_event);
*/

}

/**
 * To draw info box frame OSD.
 * @return none.
 */
void CosdCoreDrawModeFrame_key(UINT8* pTitleStr, UINT8 ucOpt)
{

    UINT8 ucCol;
    ucOpt = ucOpt;
    COsdFxDisableOsd();
    ap_osdfx_map_setting(_OSD_MAP0);

    SetOSDDouble(0);
    COsdFxSetWinRefPos(_OSD_LISTMENU_H_POS, _OSD_LISTMENU_V_POS);
    // display channel number
    OSDLine(ROW(1), COL(1), LENGTH(12), 0xA6, _WRITE_BYTE0);
    OSDLine(ROW(1), COL(1), LENGTH(12), 0xA4, _WRITE_BYTE1);
    OSDLine(ROW(1), COL(1), LENGTH(12), 0x11, _WRITE_BYTE2);

    OSDLine(ROW(4), COL(1), LENGTH(12), 0xA6, _WRITE_BYTE0);
    OSDLine(ROW(4), COL(1), LENGTH(12), 0xA4, _WRITE_BYTE1);
    OSDLine(ROW(4), COL(1), LENGTH(12), 0x11, _WRITE_BYTE2);
    COsdFxDrawWindow(&tModeKeyWindowStyle);

    ucCol = COsdFxLoadText(2, 3, 24, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, pTitleStr);
    COsdFxEnableOsd();
}

/**
 * To draw info box frame OSD.
 * @return none.
 */
void CosdCoreDrawModeFrame(UINT8* pTitleStr, UINT8 ucOpt)
{

    UINT8 ucCol;
    ucOpt = ucOpt;
    COsdFxDisableOsd();
    ap_osdfx_map_setting(_OSD_MAP0);

    SetOSDDouble(0);
    COsdFxSetWinRefPos(_OSD_LISTMENU_H_POS, _OSD_LISTMENU_V_POS);
    // display channel number
    OSDLine(ROW(1), COL(1), LENGTH(8), 0xA6, _WRITE_BYTE0);
    OSDLine(ROW(1), COL(1), LENGTH(8), 0xA4, _WRITE_BYTE1);
    OSDLine(ROW(1), COL(1), LENGTH(8), 0x11, _WRITE_BYTE2);

    OSDLine(ROW(3), COL(1), LENGTH(8), 0xA6, _WRITE_BYTE0);
    OSDLine(ROW(3), COL(1), LENGTH(8), 0xA4, _WRITE_BYTE1);
    OSDLine(ROW(3), COL(1), LENGTH(8), 0x11, _WRITE_BYTE2);
    COsdFxDrawWindow(&tModeWindowStyle);

	if(/*pTitleStr == tStrSignal_VGA||*/pTitleStr == tStrSignal_CVBS_1||pTitleStr == tStrSignal_CVBS_2||pTitleStr == tStrSignal_HDMI_0)
		ucCol = COsdFxLoadText(2, 1, 24, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, pTitleStr);
	else
    	ucCol = COsdFxLoadText(2, 2, 24, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, pTitleStr);
    COsdFxEnableOsd();
}
void COsdCoreDrawInfoBoxFrame(UINT8* pTitleStr, UINT8 ucOpt)
{
    UINT8 j;
    COsdFxDisableOsd();
    ap_osdfx_map_setting(_OSD_MAP0);

    SetOSDDouble(0);
    if ( (ucOpt&_MODEBOX_TYPE) != 0)
        COsdFxSetWinRefPos(_OSD_MODE_BOX_H_POS, _OSD_MODE_BOX_V_POS);
    else if ( (ucOpt&_MESSAGEBOX_TYPE) != 0)
        COsdFxSetWinRefPos(_OSD_MESSAGE_BOX_H_POS, _OSD_MESSAGE_BOX_V_POS);
    else
        COsdFxSetWinRefPos(_OSD_INFO_BOX_H_POS, _OSD_INFO_BOX_V_POS);

    OSDLine(ROW(1), COL(1), LENGTH(25), 0xA6, _WRITE_BYTE0);
    OSDLine(ROW(1), COL(1), LENGTH(25), 0xA4, _WRITE_BYTE1);
    OSDLine(ROW(1), COL(1), LENGTH(25), 0x11, _WRITE_BYTE2);

    OSDLine(ROW(5), COL(1), LENGTH(25), 0xA6, _WRITE_BYTE0);
    OSDLine(ROW(5), COL(1), LENGTH(25), 0xA4, _WRITE_BYTE1);
    OSDLine(ROW(5), COL(1), LENGTH(25), 0x11, _WRITE_BYTE2);

    OSDLine(ROW(3), COL(1), LENGTH(45), 0x10, _WRITE_BYTE2);

    COsdFxDrawWindow(&tSource_Info_WindowStyle);

    if ( pTitleStr != NULL)
    {
        j = COsdFxLoadText(0, 0, 20, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, pTitleStr);
        if (j < _OSD_TITLE_ROW_LENGTH)
            COsdFxClearText(0, j, 1, _OSD_TITLE_ROW_LENGTH-j);
    }
    COsdFxEnableOsd();
}

/**
 * To display source information.
 *
 */
void CShowMode(UINT8 ucRedraw,UINT8 ucRow,UINT8 ucCol)
{
#ifdef _VIDEO_TV_SUPPORT
    UINT8 idx;
#endif
    ucRedraw = ucRedraw;
    ucRow=ucRow;
    ucCol=ucCol;
    switch (GET_INPUTSOURCE_TYPE())
    {

    case _SOURCE_VGA:
        COSdCoreVGADispResolution(ucRow,ucCol);
        break;
    #ifdef _VIDEO_TV_SUPPORT
    case _SOURCE_TV:
		idx = GET_TV_CHN_DATA(_CUR_CHN_TV);
        //idx = GET_TV_CUR_CHN();
        CshowTVMode( idx, ucRow, ucCol,FONT_COLOR(1, _CP_BLACK));
        break;
    #endif
#ifdef CONFIG_SCART_AUTO_SWITCH
    case _SOURCE_FULL_SCART:
    case _SOURCE_HALF_SCART:
        COSdCoreScartDispResolution();
        break;
#endif

#ifdef CONFIG_ENABLE_HDMI
    case _SOURCE_HDMI:
        COsdCoreDispHDMIMode(ucRow,ucCol);
        break;
#endif
    default :
        COSdCoreDispResolution(ucRow,ucCol);
        break;

    }
}


void COsdCoreSourceInfo_key(UINT8 ucRedraw)
{
    UINT8 idx;
    UINT8 ucCol;

    ucRedraw = ucRedraw;
    /*
    if(GET_INPUTSOURCE_TYPE() == _SOURCE_TV) {
    	idx = GET_TV_CUR_CHN();
    	COsdCoreShowTVChannel(idx);
    	fw_timer_event_ReactiveTimerEvent(_OSD_DISPLAY_TIMEOUT, timerevent_ui_table_OSD_timeout_event);
    	return;
    }
    */
    pListItem = &tSourceSelectList;
    ucCol = COsdCtrlGetSignalSource();
    idx = COsdCoreListItemValueToIdx(ucCol);
    CosdCoreDrawModeFrame_key(pListItem->pItemList[idx].pString, 0);
    if(GET_INPUTSOURCE_TYPE() == _SOURCE_VGA)
    CShowMode(0,3,0);
	else
	CShowMode(0,3,3);
	
    COsdFxEnableOsd();
}


void COsdCoreSourceInfo(UINT8 ucRedraw)
{
    UINT8 idx;
    UINT8 ucCol;

    ucRedraw = ucRedraw;
    /*
    if(GET_INPUTSOURCE_TYPE() == _SOURCE_TV) {
    	idx = GET_TV_CUR_CHN();
    	COsdCoreShowTVChannel(idx);
    	fw_timer_event_ReactiveTimerEvent(_OSD_DISPLAY_TIMEOUT, timerevent_ui_table_OSD_timeout_event);
    	return;
    }
    */
    pListItem = &tSourceSelectList;
    ucCol = COsdCtrlGetSignalSource();
    idx = COsdCoreListItemValueToIdx(ucCol);
    InitOsdFrame();
   	CosdCoreDrawModeFrame(pListItem->pItemList[idx].pString, 0);
    //CShowMode(0,2,3);
    
    COsdFxEnableOsd();
}


/**
 * To display resolution information form ADC source.
 *
 */
void COSdCoreDispResolution(UINT8 ucRow,UINT8 ucCol_Pos)
{
    UINT8 ucCol;
    UINT8 index;

    if (info->state != _MODE_STATE_ACTIVE)
    { // no signal !
        COsdFxClearText(ucRow, 0, 1, _OSD_ITEM_ROW_LENGTH);
        return;
    }
#if 0
    // it means no-cable or no-signal
    if (GET_ACT_WID() == 0)
        return;
#endif
    ucCol = 0;
    if (GET_CURRENT_MODE() >= _MODE_480I)
    {
        index = GET_CURRENT_MODE() - _MODE_480I;
        if (index <= (_MODE_1080P30-_MODE_480I))
        {
            ucCol = COsdFxLoadText(ucRow, ucCol_Pos, 20, FONT_COLOR(1, _CP_BLACK), 0, tOsdResolutionStrSet[index]);
        }
    }
	#if 0
    else
    {
        COsdFxLoadBlankCmd(_INFO_ROW, 0, 20, _CP_BLACK);
        ucCol = 1;
        ucCol += COsdFxShowNumber(_INFO_ROW, 1, GET_ACT_PRE_WID(), 0x33, _CP_WHITE);
        ucCol++;
        COsdFxWrite1BitFont(_INFO_ROW, ucCol, _CROSS_, (_CP_WHITE<<4) | _CP_BLACK);
        ucCol += 2;
        ucCol += COsdFxShowNumber(_INFO_ROW, ucCol, GET_ACT_PRE_LEN(), 0x33, _CP_WHITE);
        ucCol++;
        ucCol += COsdFxShowNumber(_INFO_ROW, ucCol, GET_IV_FREQ()/10, 0x23, _CP_WHITE);
        ucCol++;
        ucCol += COsdFxLoadText(_INFO_ROW, ucCol, 20, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tStr_Hz);
    }
	#endif
}
#if defined(CONFIG_SCART_AUTO_SWITCH)
/**
 * To display resolution information form ADC source.
 *
 */
void COSdCoreScartDispResolution(void)
{
#define _INFO_ROW			(3)

    if (info->state != _MODE_STATE_ACTIVE)
    { // no signal !
        COsdFxClearText(_INFO_ROW, 0, 1, _OSD_ITEM_ROW_LENGTH);
        return;
    }
    if (GET_CURRENT_MODE() == _MODE_480I)
    {
        COsdFxLoadText(_INFO_ROW, 0, 20, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tStr_NTSC);
    }
    else
    {
        COsdFxLoadText(_INFO_ROW, 0, 20, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tStr_PAL);
    }

#undef _INFO_ROW
}
#endif
/**
 * To display resolution information form VGA source.
 *
 */
void COSdCoreVGADispResolution(UINT8 ucRow,UINT8 ucCol_Pos)
{
    UINT8 ucCol;
    //UINT8 index;
    //UINT32 usFreq = (GET_IV_FREQ()+5)/10;
    UINT32 usFreq = GET_IV_FREQ()/10;
    if (info->state != _MODE_STATE_ACTIVE)
    { // no signal !
        COsdFxClearText(ucRow, 0, 1, _OSD_ITEM_ROW_LENGTH);
        return;
    }
	#if 0
    // it means no-cable or no-signal
    if (GET_ACT_WID() == 0)
        return;
	#endif

	#if 1
    ucCol = 0;
    COsdFxLoadBlankCmd(ucRow, 0, 26, _CP_BLACK);
    ucCol = 12;
    if (GET_ACT_PRE_WID()<1000)
        ucCol_Pos += COsdFxShowNumber(ucRow, ucCol_Pos, GET_ACT_WID(), 0x23, 1);
    else
        ucCol_Pos += COsdFxShowNumber(ucRow, ucCol_Pos, GET_ACT_WID(), 0x33, 1);
    COsdFxWrite1BitFont(ucRow, ucCol_Pos, _CROSS_, 0x10);
    ucCol_Pos ++;
    //if (GET_INPUT_MODE() == _MODE_640x350_70HZ)
      //  ucCol += COsdFxShowNumber(_INFO_ROW, ucCol, 350, 0x33, 1);
    //else
        ucCol_Pos += COsdFxShowNumber(ucRow, ucCol_Pos, info->IPV_ACT_LEN, 0x33, 1);
    ucCol_Pos++;
    if (usFreq >=74 && usFreq <= 76) 	// for 75Hz display
        usFreq = 75;
    ucCol_Pos += COsdFxShowNumber(ucRow, ucCol_Pos, usFreq, 0x23, 1);
    //ucCol_Pos++;
    ucCol_Pos += COsdFxLoadText(ucRow, ucCol_Pos, 20,0x10, 0, tStr_Hz);
	#else

	if (GET_CURRENT_MODE() >= _MODE_640x350_70HZ)
	{
	    index = GET_CURRENT_MODE() - _MODE_640x350_70HZ;
	    RTD_Log(LOGGER_LEVEL,"\n GET_CURRENT_MODE() = %d,=%d \n",(UINT32)info->IPH_ACT_WID,(UINT32)info->DispWid);
	    RTD_Log(LOGGER_LEVEL,"\n index = %d \n",(UINT32)index);
	    if (index <= (_MODE_1920x1080_60Hz_173MAC-_MODE_640x350_70HZ))
	    {
	        ucCol = 12;
	        //RTD_Log(LOGGER_LEVEL,"\n Tim curr mode1 = %d \n",(UINT32)info->ucMode_Curr);
	        ucCol += COsdFxLoadText(_INFO_ROW, 12, 20, FONT_COLOR(4, _CP_BLACK), 0, tOsdVGAResolutionStrSet[index]);
	    }
	}

	#endif
}

#ifdef CONFIG_ENABLE_HDMI

/**
 * To display HDMI Resolution.
 *
 */
void COsdCoreDispHDMIMode(UINT8 ucRow,UINT8 ucCol_Pos)
{
    UINT8 ucCol;
    UINT8 index;
    
    if (info->state != _MODE_STATE_ACTIVE)
    { // no signal !
        COsdFxClearText(ucRow, 0, 1, _OSD_ITEM_ROW_LENGTH);
        return;
    }
    ucCol = 0;
	#if 0
	    if (ABS(GET_HDMI_MEASURE_IVH(),240) < 10) // 480i,240p
	    {
	        if (GET_INTERLACE_MODE())
	            pMode = tOsdResolution480iStr;
	        else
	            pMode = tOsdResolution240pStr;
	    }
	    else if (ABS(GET_HDMI_MEASURE_IVH(),288) <10) //576i,288p
	    {
	        if (GET_INTERLACE_MODE())
	            pMode =  tOsdResolution576iStr;
	        else
	            pMode =  tOsdResolution288pStr;
	    }
	    else if ((ABS(GET_HDMI_MEASURE_IVH(),480) <10)&&(ABS(info->IPH_ACT_WID,720) <10))//480p
	        pMode = tOsdResolution480pStr;
	    else if ((ABS(GET_HDMI_MEASURE_IVH(),576) <10)&&(ABS(info->IPH_ACT_WID,720) <10))//576p
	        pMode = tOsdResolution576pStr;
	    else if ((ABS(GET_HDMI_MEASURE_IVH(),720) <10)&&(ABS(info->IPH_ACT_WID,1280) <10))//720P
	        pMode = tOsdResolution720pStr;
	    else if ((ABS(GET_HDMI_MEASURE_IVH(),540) <10)&&(ABS(info->IPH_ACT_WID,1920) <10))//1080i
	        pMode =  tOsdResolution1080iStr;
	    else if ((ABS(GET_HDMI_MEASURE_IVH(),1080) <10)&&(ABS(info->IPH_ACT_WID,1920) <10))//1080p
	        pMode =  tOsdResolution1080pStr;
	    else
	        pMode = NULL;
	    // get Frequence
	    if (ABS(GET_IV_FREQ()/10,50)< 5)
	        pFreq = tOsdFreq50Str;
	    else if (((GET_IV_FREQ()+5)/10)==25)
	        pFreq = tOsdFreq25Str;
	    else if (ABS(GET_IV_FREQ()/10,24)< 3)
	        pFreq = tOsdFreq24Str;
	    else if (ABS(GET_IV_FREQ()/10,30)< 5)
	        pFreq = tOsdFreq30Str;
	    else if (ABS(GET_IV_FREQ()/10,60)< 5)
	        pFreq = tOsdFreq60Str;
	    else if (ABS(GET_IV_FREQ()/10,100)< 10)
	        pFreq = tOsdFreq100Str;
	    else if (ABS(GET_IV_FREQ()/10,120)< 10)
	        pFreq = tOsdFreq120Str;
	    else
	        pFreq = NULL;

	    if (pMode != NULL)
	    {
	    
	    RTD_Log(LOGGER_LEVEL,"\n Tim hdmi show debug2 \n");
	        ucCol += COsdFxLoadText(_INFO_ROW, 12, 20, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, pMode);
	        if (pFreq != NULL)
	            ucCol += COsdFxLoadText(_INFO_ROW, ucCol, 10, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, pFreq);
	    }
	    else
	    {
	    
	    RTD_Log(LOGGER_LEVEL,"\n Tim hdmi show debug3 \n");
	        // it means no-cable or no-signal
	        if (GET_ACT_WID() == 0)
	        {
	            /* nothing to do. */
	        }
	        else
	        {
	        	RTD_Log(LOGGER_LEVEL,"\n Tim hdmi show debug \n");
	            COsdFxLoadBlankCmd(_INFO_ROW, 0, 20, _CP_BLACK);
	            ucCol = 12;
	            ucCol += COsdFxShowNumber(_INFO_ROW, 12, GET_ACT_PRE_WID(), 0x33, _CP_WHITE);
	            ucCol++;
	            COsdFxWrite1BitFont(_INFO_ROW, ucCol, _CROSS_, (_CP_WHITE<<4) | _CP_BLACK);
	            ucCol += 2;
	            ucCol += COsdFxShowNumber(_INFO_ROW, ucCol, GET_ACT_PRE_LEN(), 0x33, _CP_WHITE);
	            ucCol++;
	            ucCol += COsdFxShowNumber(_INFO_ROW, ucCol, (GET_IV_FREQ()+5)/10, 0x23, _CP_WHITE);
	            ucCol++;
	            ucCol += COsdFxLoadText(_INFO_ROW, ucCol, 20, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tStr_Hz);
	        }
	    }
	#else//Tim 0328
		if (GET_CURRENT_MODE() >= _HDMI_MODE_480I)
		{
		    index = GET_CURRENT_MODE() - _HDMI_MODE_480I;
		    //RTD_Log(LOGGER_LEVEL,"\n index = %d \n",(UINT32)index);
		    //RTD_Log(LOGGER_LEVEL,"\n index111 = %d \n",(UINT32)(_HDMI_MODE_1080P30-_HDMI_MODE_480I));
		    if (index <= (_HDMI_MODE_1080P30-_HDMI_MODE_480I))
		    {
		        ucCol = 12;
		        //RTD_Log(LOGGER_LEVEL,"\n Tim curr mode1 = %d \n",(UINT32)info->ucMode_Curr);
		        //ucCol += COsdFxLoadText(ucRow, ucCol_Pos, 20, FONT_COLOR(4, _CP_BLACK), 0, tOsdHdmiResolutionStrSet[index]);
				ucCol += COsdFxLoadText(ucRow, ucCol_Pos, 21, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0,  tOsdHdmiResolutionStrSet[index]);
		    }
		}

	#endif
    //if (ucCol < _OSD_ITEM_ROW_LENGTH)
        //COsdFxClearText(ucRow, ucCol, 1, _OSD_ITEM_ROW_LENGTH-ucCol);

}
#endif

#ifdef _VIDEO_TV_SUPPORT
/**
 * To display TV channel information.
 *
 */
void COsdCoreShowTVChannel(UINT8 ch)
{
#define _INFO_ROW		(1)
    //UINT32 freq;
    //freq = GET_CHANNEL_FREQ(ch);
    //RTD_Log(LOGGER_INFO, "freq-------------------------------%x",(UINT32)freq);
    ap_osdfx_map_setting(_OSD_MAP0);
    COsdFxSetWinRefPos(_OSD_TVCHANNEL, 10);
    SetOSDDouble(3);
    // display channel number
    OSDLine(ROW(1), COL(1), LENGTH(8), 0xF0, _WRITE_BYTE2);
    OSDLine(ROW(2), COL(1), LENGTH(8), 0xF0, _WRITE_BYTE2);
    COsdFxShowNumber(_INFO_ROW, 3, ch, 0x23, 15);

    CshowTVMode(ch,2,2,FONT_COLOR(15, _CP_BLACK));
    COsdFxEnableOsd();
    fw_timer_event_ReactiveTimerEvent(SEC(2), timerevent_ui_table_OSD_timeout_event);
#undef _INFO_ROW
}

void CshowTVMode(UINT8 ch,UINT8 row,UINT8 col,UINT8 ucColor)
{

    UINT8 j;
    // CF 2007 04 18 added, for Debug, display Video & Audio standard
    switch (GET_CHANNEL_COLORSYSTEM(ch))
    {
    case ZPAL_I:
    case ZPAL_M:
    case ZPAL_N:
        j = COsdFxLoadText(row, col, 0, ucColor, 0, tStr_PAL);
        break;
    case ZNTSC:
    case ZNTSC_50:
    case ZNTSC_443:
        j = COsdFxLoadText(row, col, 0, ucColor, 0, tStr_NTSC);
        break;

    case ZSECAM:
    case ZSECAML:
    case ZSECAMLA:
        j = COsdFxLoadText(row, col, 0, ucColor, 0, tStr_SECAM);
        break;

    case ZAUTO:
        j = COsdFxLoadText(row, col, 0, ucColor, 0, tStr_AUTO);
        break;
    default:
        j = COsdFxLoadBlankCmd(row, 0, 0, _CP_BLACK);
        break;
    }
    col ++;

    switch (COsdCtrlGetSoundSystem())
    {
    case _SOUND_AUTO:
        COsdFxLoadText(row, col+5, 0, ucColor, 0, tStr_AUTO);
        break;
    case _SOUND_MN:
        COsdFxLoadText(row, col+5, 0, ucColor, 0, tStr_M);
        break;

	//Tim 0413
    case _SOUND_N:
        COsdFxLoadText(row, col+5, 0, ucColor, 0, tStr_N);
        break;

    case _SOUND_DK:
        COsdFxLoadText(row, col+5, 0, ucColor, 0, tStr_DK);
        break;

    case _SOUND_BG:
        COsdFxLoadText(row, col+5, 0, ucColor, 0, tStr_BG);
        break;

    case _SOUND_I:
        COsdFxLoadText(row, col+5, 0, ucColor, 0, tStr_I);
        break;

    case _SOUND_L:
        COsdFxLoadText(row, col+5, 0, ucColor, 0, tStr_L);
        break;

    default:
        break;
    }


}
#endif
/**
 * To display system message on OSD.
 *
 */
void COsdCoreDispMessageBox(UINT8 ucOpt)
{
    UINT8 j,ucWidth,ucCnt;
    LoadLanguage_FONT(OTHER_PG);
    COsdCoreDrawInfoBoxFrame(NULL, _INFOBOX_HINT_BAR | _MESSAGEBOX_TYPE);
    ucWidth = COsdFxCalTextWidth(0, tOsdShowStatusStrSet[(ucOpt&0x0f)]);	// display message
    ucCnt = (29*12-ucWidth)/2;
    j = COsdFxLoadText(3, 0, ucCnt, FONT_COLOR(_CP_WHITE, _CP_BLACK), COsdCtrlGetLanguage(), tOsdShowStatusStrSet[(ucOpt&0x0f)]);
    if (j < _OSD_ITEM_ROW_LENGTH)
        COsdFxClearText(3, j, 1, _OSD_ITEM_ROW_LENGTH-j);

    COsdFxEnableOsd();
}
#if 0//Tim 0406
/**
 * To display video, audio, sleep mode statue.
 *
 */
void COsdCoreDispModeBox(UINT8 ucOpt)
{
    UINT8 ucCol;
    UINT8 idx;

    if ( (ucOpt&_BIT0) != 0)
    {
        COsdCoreDrawInfoBoxFrame(pListItem->pString, _INFOBOX_HINT_BAR | _MODEBOX_TYPE);
    }

    // display mode status
    idx = COsdCoreListItemValueToIdx(COsdCtrlMenuItemGetValue(pListItem->ucMenuItemID));
    ucCol = COsdFxLoadText(2, 0, 34, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, pListItem->pItemList[idx].pString);
    if ( ucCol < _OSD_ITEM_ROW_LENGTH)
        COsdFxClearText(2, ucCol, 1, _OSD_ITEM_ROW_LENGTH-ucCol);

    if ( (ucOpt&_BIT0) != 0)
        COsdFxEnableOsd();

}
#endif
/**
 * ModeBox message event handler.
 * @return none.
 */
void COsdCoreModeBoxEventProc(void)
{
    if (( fUI_Msg < _TTX_EXIT_MESSAGE)&&(fUI_Msg!=_FL2UI_MAIN_NO_SIGNAL))//Hardy20091209
        fw_timer_event_ReactiveTimerEvent(_OSD_DISPLAY_TIMEOUT/*SEC(5)*/, timerevent_ui_table_OSD_timeout_event);


    if ( _REPEAT_KEY_MESSAGE == fUI_Msg)
    {
        if ( uwOsdCoreModeKey == uwUI_PreMsg)
            fUI_Msg = uwOsdCoreModeKey;
    }

    switch (fUI_Msg)
    {
    case _RIGHT_KEY_MESSAGE:
    case _LEFT_KEY_MESSAGE:
    case _RIGHT_IR_MESSAGE:
    case _LEFT_IR_MESSAGE:
        COsdCtrlMenuItemHandler(pListItem->ucMenuItemID, fUI_Msg);
        COsdCoreDispModeBox(0);
        break;

    case _OSD_TIMEOUT_MESSAGE:
    case _EXIT_KEY_MESSAGE:
    case _MENU_KEY_MESSAGE:
    
    //RTD_Log(LOGGER_LEVEL,"\n _OSD_TIMEOUT_MESSAGE55 Tim bCheckCCStatus =%d \n",(UINT32)bCheckCCStatus);
        COsdCoreExitOsd();
        break;

    default:
        if ( fUI_Msg == uwOsdCoreModeKey)
        {
            COsdCtrlMenuItemHandler(pListItem->ucMenuItemID, _RIGHT_KEY_MESSAGE);
            COsdCoreDispModeBox(0);
            break;
        }

        COsdCoreDispatchMsg();
        break;
    }

}

#ifdef _VIDEO_TV_SUPPORT
#if defined(_REALTEK_OSD_USE)

/**
 * To initial Channel Editor OSD
 *
 */
void COsdCoreInitChannelEditState(void)
{
    UINT8 ucCnt;
    UINT8 j;
    UINT8 CurChn;

    ap_osdfx_map_setting(_OSD_MAP0);
    COsdFxSetRowCtrl1(3, ROW_HEIGHT(22));
    COsdFxSetRowCtrl1(4, ROW_HEIGHT(8));
    for (j=0; j<7; j++)
        COsdFxSetRowCtrl1(5+j, ROW_HEIGHT(24));
    COsdFxSetRowCtrl1(14, ROW_HEIGHT(20));
    COsdFxSetRowCtrl1(15, ROW_HEIGHT(20));

    COsdFxSetWinRefPos(_OSD_CHN_EDIT_MENU_H_POS, _OSD_CHN_EDIT_MENU_V_POS);

//	ap_osdfx_load_vlc_font(tFont_SourcePatch, _FRAME_2BIT_PATCH_ADDR, 16);

    // display title string
    ucCnt = COsdFxLoadText(0, 0, 24, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tStr_Channel_Edit);
    if (ucCnt < _OSD_TITLE_ROW_LENGTH)
        COsdFxClearText(0, ucCnt, 1, _OSD_TITLE_ROW_LENGTH-ucCnt);

    COsdFxDrawWindow(&tSource_Select_Title_WindowStyle);
    COsdFxMoveWindow(tSource_Select_Title_WindowStyle.ucWindowID, &tChannel_Edit_TitleWindowPos);
    COsdFxDrawWindow(&tSource_Select_Window_Style);
    COsdFxMoveWindow(tSource_Select_Window_Style.ucWindowID, &tChannel_Edit_WindowPos);
    COsdFxDrawWindow(&tSource_Select_Hint_WindowStyle);
    COsdFxMoveWindow(tSource_Select_Hint_WindowStyle.ucWindowID, &tChannel_Edit_HintWindowPos);

    COsdFxSet2BitColor(0, _CP_BLACK);
    COsdFxSet2BitColor(1, 5);
    COsdFxSet2BitColor(2, 1);
    COsdFxSet2BitColor(3, 4);

    // patch the title bar
    COsdFxWrite2BitFont(1, 0, _FRAME_PATCH_FS+3);
    for (ucCnt=1; ucCnt<15; ucCnt++)
        COsdFxWrite2BitFont(1, ucCnt, _FRAME_PATCH_FS+4);
    COsdFxWrite2BitFont(1, ucCnt, _FRAME_PATCH_FS+5);
    COsdFxClearText(1, ucCnt+1, 1, _OSD_TITLE_ROW_LENGTH-ucCnt-1);

    // display item label
    ucCnt = COsdFxLoadText(2, 0, 36, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tStr_CH);
    ucCnt += COsdFxLoadText(2, ucCnt, 36, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tStr_Name);
    ucCnt += COsdFxLoadText(2, ucCnt, 48, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tStr_Skip);
    if (ucCnt < _OSD_ITEM_ROW_LENGTH)
        COsdFxClearText(2, ucCnt, 1, _OSD_ITEM_ROW_LENGTH-ucCnt);

    // patch the hint bar
    ucCnt = COsdFxLoadBlankCmd(12, 0, 12*19+4, _CP_BLACK);
    COsdFxWrite2BitFont(12, ucCnt, _FRAME_PATCH_FS);
    ucCnt++;
    for (j=0; j<4; j++)
        COsdFxWrite2BitFont(12, ucCnt+j, _FRAME_PATCH_FS+1);
    ucCnt += 4;
    COsdFxWrite2BitFont(12, ucCnt, _FRAME_PATCH_FS+2);
    ucCnt = ucCnt + 1;
    if ( ucCnt < _OSD_ITEM_ROW_LENGTH)
        COsdFxClearText(12, ucCnt, 1, _OSD_ITEM_ROW_LENGTH-ucCnt);

    COsdFxSet2BitColor(3, _CP_BLACK);

    // seprate line
    COsdFxLoadBlankCmd(3, 0, 20, _CP_BLACK);
    ucCnt = 1;
    for (j=0; j<18; j++)
        COsdFxWrite2BitFont(3, ucCnt+j, _FRAME_PATCH_FS+6);
    COsdFxWrite2BitFont(3, ucCnt+j, _FRAME_PATCH_FS+7);

    // draw hint text
#define _HINT_TEXT_ROW	(13)
    COsdFxLoadBlankCmd(_HINT_TEXT_ROW, 0, 20, _CP_BLACK);
    //COsdFxWrite2BitFont(_HINT_TEXT_ROW, 1, _COLORKEY_FS);
    COsdFxWrite1BitFont(_HINT_TEXT_ROW, 1, 0x67, FONT_COLOR(_CP_YELLOW, _CP_BLACK));
    ucCnt = 2;
    ucCnt += COsdFxLoadText(_HINT_TEXT_ROW, 2, 8, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tStr_Set_Skip);

    COsdFxLoadBlankCmd(_HINT_TEXT_ROW, ucCnt, 20, _CP_BLACK);
    ucCnt++;
    COsdFxWrite1BitFont(_HINT_TEXT_ROW, ucCnt, 0x67, FONT_COLOR(_CP_PURE_RED, _CP_BLACK));
    ucCnt++;
    ucCnt += COsdFxLoadText(_HINT_TEXT_ROW, ucCnt, 8, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tStr_Remove);

    COsdFxLoadBlankCmd(_HINT_TEXT_ROW+1, 0, 20, _CP_BLACK);
    COsdFxWrite1BitFont(_HINT_TEXT_ROW+1, 1, 0x67, FONT_COLOR(_CP_GREEN, _CP_BLACK));
    ucCnt = 2;
    ucCnt += COsdFxLoadText(_HINT_TEXT_ROW+1, 2, 8, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tStr_Copy);

    COsdFxLoadBlankCmd(_HINT_TEXT_ROW+1, ucCnt, 20, _CP_BLACK);
    ucCnt++;
    COsdFxWrite1BitFont(_HINT_TEXT_ROW+1, ucCnt, 0x67, FONT_COLOR(_CP_BLUE, _CP_BLACK));
    ucCnt++;
    COsdFxLoadText(_HINT_TEXT_ROW+1, ucCnt, 8, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tStr_Insert);
#undef _HINT_TEXT_ROW

    ucOsdState = _CHN_EDIT_STATE;
	CurChn = GET_TV_CHN_DATA(_CUR_CHN_TV);
	
    ucCopyChn = CurChn;
#if 0
    ucListSelectedItem = 0;
#else
    ucListSelectedItem = CurChn - 1;
    if ( ucListSelectedItem > _MAX_PAL_CATV_NUM - 1)
        ucListSelectedItem = 0;
#endif
    COsdCoreUpdateChannelEditOsd(ucListSelectedItem);
    COsdFxDrawWindow(&tSource_Select_Highlight_WindowStyle);
    COsdCoreSetChannelEditItemHighlight(ucListSelectedItem);

    COsdFxEnableOsd();
}

/**
 * To redraw channel edit content
 *
 */
void COsdCoreUpdateChannelEditOsd(UINT8 ucItemID)
{
#define _START_ROW_			(4)
    UINT8 j;
    UINT8 ucCnt;
    UINT8 k;
    UINT8 getVal;

    ucPage = ucItemID / 8;

    for (j=0; j<8; j++)
    {
        COsdFxLoadBlankCmd(_START_ROW_+j, 0, 24, _CP_BLACK);
        COsdFxShowNumber(_START_ROW_+j, 1, ucPage*8+j+1, 0x23, _CP_WHITE);
        ucCnt = 4; // <-- start pos of channel name
        COsdCoreLoadChannelName(ucPage*8+j+1);	// chn = id + 1, chn start form 1
        COsdFxLoadBlankCmd(_START_ROW_+j, ucCnt, 36, _CP_BLACK);
        for (k=0; k<5; k++)
        {
            ucOsdCoreBuf[6] = k;	// column position
            COsdCoreChnEditUpdateChnName(ucPage*8+j);
        }
        ucCnt += 6;
        COsdFxLoadBlankCmd(_START_ROW_+j, ucCnt, 48, _CP_BLACK);
        ucCnt++;

        k = ui_get_channel_idx(ucPage*8+j+1);
        LOAD_STORAGE_ARRAY_DATA(StructUserDataChannelSettingsType, k, status, getVal);
        if (getVal & _CHN_STS_SKIP)  // skip status
        {
            COsdFxWrite1BitFont(_START_ROW_+j, ucCnt, _X_, (_CP_WHITE<<4) | _CP_BLACK);
        }
        else
            COsdFxLoadBlankCmd(_START_ROW_+j, ucCnt, 12, _CP_BLACK);
        ucCnt++;
        if (ucCnt < _OSD_ITEM_ROW_LENGTH)
            COsdFxClearText(_START_ROW_+j, ucCnt, 1, _OSD_ITEM_ROW_LENGTH-ucCnt);

    }

#undef _START_ROW_
}

/**
 * To redraw channel name string.
 *
 */
void COsdCoreChnEditUpdateChnName(UINT8 ucID)
{
#define _START_ROW_			(4)

    COsdFxWrite1BitFont(_START_ROW_+(ucID%8), 5+ucOsdCoreBuf[6], ucOsdCoreBuf[ucOsdCoreBuf[6]], _CP_WHITE<<4 | _CP_BLACK);

#undef _START_ROW_
}

/**
 * To move high-light window.
 *
 */
void COsdCoreSetChannelEditItemHighlight(UINT8 ucItemID)
{
    if (ucOsdState == _CHN_NAME_EDIT_STATE)
    {
        stItemPos = tChannel_Edit_Name_Highlight_WindowPos;
        stItemPos.usVStart = stItemPos.usVStart + (ucItemID%8) * 24;
        stItemPos.usVEnd = stItemPos.usVEnd + (ucItemID%8) * 24;
        stItemPos.usHStart = stItemPos.usHStart + ucOsdCoreBuf[6] * 12;
        stItemPos.usHEnd = stItemPos.usHEnd + ucOsdCoreBuf[6] * 12;
        COsdFxMoveWindow(tSource_Select_Highlight_WindowStyle.ucWindowID, &stItemPos);
    }
    else
    {
        stItemPos = tChannel_Edit_Highlight_WindowPos;
        stItemPos.usVStart = stItemPos.usVStart + (ucItemID%8) * 24;
        stItemPos.usVEnd = stItemPos.usVEnd + (ucItemID%8) * 24;
        COsdFxMoveWindow(tSource_Select_Highlight_WindowStyle.ucWindowID, &stItemPos);
    }
}


/**
 * To load channel name from flash.
 *
 */
void COsdCoreLoadChannelName(UINT8 ucChn)
{
    UINT8 j;

    GET_CHANNEL_NAME(ucChn, ucOsdCoreBuf);

    for (j=0; j<5; j++)
    {
        if ( !xIsChnCharValid(ucOsdCoreBuf[j]))
            ucOsdCoreBuf[j] = _MINUS_;
    }
    ucOsdCoreBuf[5] = _END_;
}

/**
 *
 *
 */
UINT8 code validChar[] =
{
    ___, _MINUS_, _PLUS_, _DOT_, _SLASH_, _COLON_,
    _A_, _B_, _C_, _D_, _E_, _F_, _G_, _H_, _I_, _J_, _K_, _L_, _M_, _N_, _O_, _P_, _Q_, _R_, _S_, _T_, _U_, _V_, _W_, _X_, _Y_, _Z_,
    _0_, _1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_
};
UINT8 code validCharCount = sizeof(validChar) / sizeof(UINT8);
void COsdCoreChnEditChangeNameChar(bit ucForward)
{
    UINT8 ucID;

    for (ucID=0; ucID<validCharCount; ucID++)
    {
        if (validChar[ucID] == ucOsdCoreBuf[ucOsdCoreBuf[6]])
            break;
    }
    if (ucID == validCharCount) // invalid character ?
        ucID = 0;

    if (ucForward == 0)
    { // up
        ucID = (ucID + validCharCount - 1) % validCharCount;
    }
    else
    {
        ucID = (ucID + 1) % validCharCount;
    }

    ucOsdCoreBuf[ucOsdCoreBuf[6]] = validChar[ucID];
}

bit xIsChnCharValid(UINT8 ucChar)
{
    UINT8 j;

    for (j=0; j< validCharCount; j++)
    {
        if ( validChar[j] == ucChar)
            return 1;
    }

    return 0;
}

/**
 * To set channel edit menu to channel name edit mode.
 * @return none.
 */
static void COsdCoreChnEditSwitchToNameEditMode(void)
{
    ucOsdState = _CHN_NAME_EDIT_STATE;
    COsdCoreLoadChannelName(ucListSelectedItem+1);
    ucOsdCoreBuf[6] = 0; // character pos
    COsdFxOpenWindowEffect(tSource_Select_Highlight_WindowStyle.ucWindowID, _OFF);
    COsdCoreSetChannelEditItemHighlight(ucListSelectedItem);
}


/**
 * Channel Edit message event handler.
 * @return none.
 */
void COsdCoreChnEditEventProc(void)
{
    UINT8 tmpTVMapIndex[_MAX_CATV_NUM];
    StructUserDataChannelSettingsType xdata tmpChannelData;
//	UINT32 srcPointer;
    if (( fUI_Msg < _TTX_EXIT_MESSAGE)&&(fUI_Msg!=_FL2UI_MAIN_NO_SIGNAL))//Hardy20091209
        fw_timer_event_ReactiveTimerEvent(ap_osdctrl_get_osd_timeout_ms(), timerevent_ui_table_OSD_timeout_event);

    switch (fUI_Msg)
    {
    case _UP_KEY_MESSAGE:
        if (ucOsdState == _CHN_EDIT_STATE)
        {
            ucListSelectedItem= CTvSelectChannel(_TV_PREV_CHANNEL)-1;
            //if (ucListSelectedItem == 0)
            //	ucListSelectedItem = _MAX_PAL_CATV_NUM-1;
            //else
            //	ucListSelectedItem--;

            if (ucPage != ucListSelectedItem/8)
                COsdCoreUpdateChannelEditOsd(ucListSelectedItem);

            COsdCoreSetChannelEditItemHighlight(ucListSelectedItem);
            CTvSetChannel(ucListSelectedItem+1);
        }
        else if (ucOsdState == _CHN_NAME_EDIT_STATE)
        {
            COsdCoreChnEditChangeNameChar(0);
            COsdCoreChnEditUpdateChnName(ucListSelectedItem);
        }
        fw_ui_message_clearallType(fUI_Msg);
        break;

    case _DOWN_KEY_MESSAGE:
        if (ucOsdState == _CHN_EDIT_STATE)
        {
            ucListSelectedItem= CTvSelectChannel(_TV_NEXT_CHANNEL)-1;
            //ucListSelectedItem++;
            //if (ucListSelectedItem >= _MAX_PAL_CATV_NUM)
            //	ucListSelectedItem = 0;

            if (ucPage != ucListSelectedItem/8)
                COsdCoreUpdateChannelEditOsd(ucListSelectedItem);

            COsdCoreSetChannelEditItemHighlight(ucListSelectedItem);
            CTvSetChannel(ucListSelectedItem+1);
        }
        else if (ucOsdState == _CHN_NAME_EDIT_STATE)
        {
            COsdCoreChnEditChangeNameChar(1);
            COsdCoreChnEditUpdateChnName(ucListSelectedItem);
        }
        fw_ui_message_clearallType(fUI_Msg);
        break;

    case _LEFT_KEY_MESSAGE:
    case _LEFT_IR_MESSAGE:
        if (ucOsdState == _CHN_NAME_EDIT_STATE)
        {
            if (ucOsdCoreBuf[6] == 0)
                ucOsdCoreBuf[6] = 4;
            else
                ucOsdCoreBuf[6] -= 1;

            COsdCoreSetChannelEditItemHighlight(ucListSelectedItem);
        }
        break;

    case _RIGHT_KEY_MESSAGE:
    case _RIGHT_IR_MESSAGE:
        if (ucOsdState == _CHN_EDIT_STATE)
        {
            COsdCoreChnEditSwitchToNameEditMode();
        }
        else if (ucOsdState == _CHN_NAME_EDIT_STATE)
        {
            ucOsdCoreBuf[6] += 1;
            if (ucOsdCoreBuf[6] > 4)
                ucOsdCoreBuf[6] = 0;
            COsdCoreSetChannelEditItemHighlight(ucListSelectedItem);
        }
        break;

    case _OK_KEY_MESSAGE:
        if (ucOsdState == _CHN_EDIT_STATE)
        {
            COsdCoreChnEditSwitchToNameEditMode();
        }
        else if (ucOsdState == _CHN_NAME_EDIT_STATE)
        {
            ucOsdState = _CHN_EDIT_STATE;
            SET_CHANNEL_NAME(ucListSelectedItem+1, ucOsdCoreBuf);	// chn start from 1 and ucListSelectedItem start from 0
            COsdFxOpenWindowEffect(tSource_Select_Highlight_WindowStyle.ucWindowID, _ON);
            COsdCoreSetChannelEditItemHighlight(ucListSelectedItem);
        }
        break;

    case _REDBUTTON_MESSAGE:
        // Delete channel Item
        if (ucOsdState == _CHN_EDIT_STATE)
        {
            UINT8 ucChn;
            UINT8 ucRemovedIdx;

            ucChn = ucListSelectedItem;
            LOAD_STORAGE_STRUCT_DATA(StructTVChannelMapTable, tmpTVMapIndex);
            ucRemovedIdx = tmpTVMapIndex[ucChn];
#if 1
//				srcPointer = (UINT16)&tDEFAULT_OSDUSER_CHANNELSETTING;
//				*((UINT8*)&srcPointer + 1) = 0x07;
            //drv_spi_dma_memcpy((UINT16)&tmpChannelData, _BANK7, (UINT16)&tDEFAULT_OSDUSER_CHANNELSETTING, sizeof(StructUserDataChannelSettingsType), SPI_DMA_TRANSFER_MODE_DMEM);
#else
            srcPointer = (UINT16)&tDEFAULT_OSDUSER_CHANNELSETTING;
            *((UINT8*)&srcPointer + 1) = 0x07;
            drv_spi_dma_memcpy((UINT16)&tmpChannelData, srcPointer, sizeof(StructUserDataChannelSettingsType), SPI_DMA_TRANSFER_MODE_DMEM);
#endif
            //{ default data  from tDEFAULT_OSDUSER_CHANNELSETTING
            tmpChannelData.freq = 452500;
            tmpChannelData.chnNo = 1;
            tmpChannelData.colorSystem  = 0x02;
            tmpChannelData.soundSystem = 0x02;
            tmpChannelData.status = 2;
            tmpChannelData.offset =  24;
#ifdef CONFIG_STORAGE_ACCESS_FLASH	// marked by weihao because of china
            tmpChannelData.name[0] = 0x0D;
            tmpChannelData.name[1] = 0x0D;
            tmpChannelData.name[2] = 0x0D;
            tmpChannelData.name[3] = 0x0D;
            tmpChannelData.name[4] = 0x0D;
#endif
            //}default data  from tDEFAULT_OSDUSER_CHANNELSETTING

            ui_set_chn_data(ucRemovedIdx, &tmpChannelData);

            for (; ucChn < (_MAX_PAL_CATV_NUM - 1); ucChn++)
            {
                tmpTVMapIndex[ucChn] = tmpTVMapIndex[ucChn + 1];
            }
            tmpTVMapIndex[_MAX_PAL_CATV_NUM-1] = ucRemovedIdx;//Hardy20091230 for chnanel delete error
            SYNC_STORAGE_STRUCT_DATA(StructTVChannelMapTable, tmpTVMapIndex);
//->w				RTD_Log(LOGGER_INFO, "Save to  to map table\n");

            COsdCoreUpdateChannelEditOsd(ucListSelectedItem);
            CTvSetChannel(ucListSelectedItem);

        }
        break;

    case _GREENBUTTON_MESSAGE:
        // Copy channel item
        if (ucOsdState == _CHN_EDIT_STATE)
        {
            ucCopyChn = ucListSelectedItem+1;
            ui_get_chn_data(ui_get_channel_idx(ucCopyChn), &stCopyChannelData);
        }
        break;

    case _BLUEBUTTON_MESSAGE:
        // Insert channel item
        if (ucOsdState == _CHN_EDIT_STATE)
        {
            UINT8 j;
            UINT8 ucCopyIdx;
//				UINT8 ucIdx;

            ucCopyIdx = ui_get_channel_idx(_MAX_PAL_CATV_NUM);
            ui_set_chn_data(ucCopyIdx, &stCopyChannelData);
            LOAD_STORAGE_STRUCT_DATA(StructTVChannelMapTable, tmpTVMapIndex);
            for (j=_MAX_PAL_CATV_NUM; j> ucListSelectedItem+1; j--)
            {
//					ucIdx = ui_get_channel_idx(j-1);
                tmpTVMapIndex[j - 1] = tmpTVMapIndex[j - 2];
//					ui_set_channel_idx(j, ucIdx);
            }
//				ui_set_channel_idx(ucListSelectedItem+1, ucCopyIdx);
            tmpTVMapIndex[ucListSelectedItem] = ucCopyIdx;
            SYNC_STORAGE_STRUCT_DATA(StructTVChannelMapTable, tmpTVMapIndex);

            COsdCoreUpdateChannelEditOsd(ucListSelectedItem);
            CTvSetChannel(ucListSelectedItem+1);//Hardy20091231 for channel insert error
        }
        break;

    case _YELLOWBUTTON_MESSAGE:
        // skip status
        if (ucOsdState == _CHN_EDIT_STATE)
        {
            if ( GET_CHANNEL_SKIP(ucListSelectedItem+1) )
                CLR_CHANNEL_SKIP(ucListSelectedItem+1);
            else
                SET_CHANNEL_SKIP(ucListSelectedItem+1);

            COsdCoreUpdateChannelEditOsd(ucListSelectedItem);
        }
        break;

    case _EXIT_KEY_MESSAGE:
//		COsdCoreExitOsd();
//		break;
    case _MENU_KEY_MESSAGE:
        if (ucOsdState == _CHN_NAME_EDIT_STATE)
        {
            ucOsdState = _CHN_EDIT_STATE;
            COsdFxOpenWindowEffect(tSource_Select_Highlight_WindowStyle.ucWindowID, _ON);
            COsdCoreSetChannelEditItemHighlight(ucListSelectedItem);
        }
        else
        {
            COsdCoreFallBackMenu();
        }
        break;

    case _OSD_TIMEOUT_MESSAGE:
    
    //RTD_Log(LOGGER_LEVEL,"\n _SHOW_INFO_STATE bCheckCCStatus=%d \n",(UINT32)bCheckCCStatus);
        COsdCoreExitOsd();
        break;

    default:
        break;
    }
}
#endif
/**
 * To display channel input box.
 * @return none.
 */
void COsdCoreChnInputBox(UINT8 ucChn, UINT8 ucRedraw)
{
#define _INFO_ROW		(2)
    /*
    	if ( (ucRedraw&_BIT0) != 0)
    	{
    		pListItem = &tSourceSelectList;
    		idx = COsdCoreListItemValueToIdx(_SOURCE_INDEX_TV);
    		COsdCoreDrawInfoBoxFrame(pListItem->pItemList[idx].pString, 0);
    	}
        */
    //RTD_Log(LOGGER_INFO, "freq-------------------------------%x",(UINT32)freq);
    ap_osdfx_map_setting(_OSD_MAP0);
    COsdFxSetWinRefPos(_OSD_TVCHANNEL, 10);
    SetOSDDouble(3);
    // display channel number
    //OSDLine(ROW(1), COL(1), LENGTH(8), 0xF0, _WRITE_BYTE2);
    //OSDLine(ROW(2), COL(1), LENGTH(8), 0xF0, _WRITE_BYTE2);
    //COsdFxShowNumber(_INFO_ROW, 3, ch, 0x23, 15);
    // display channel number
    COsdFxLoadBlankCmd(_INFO_ROW, 0, 60, _CP_BLACK);
    COsdFxShowNumber(_INFO_ROW, 1, ucChn, 0x23, 15);
    COsdFxClearText(_INFO_ROW, 4, 1, _OSD_ITEM_ROW_LENGTH-4);

    if ( (ucRedraw&_BIT0) != 0)
    {
        COsdFxEnableOsd();
    }
}

/**
 * To handle channel input event.
 * @return none.
 */
void COsdCoreChnInputEventProc(void)
{
    UINT8 CurChn;
	UINT8 MaxChn;
//	fw_timer_event_ReactiveTimerEvent(ap_osdctrl_get_osd_timeout_ms(), timerevent_ui_table_OSD_timeout_event);
    RTD_Log(LOGGER_INFO, "fui_msg--------------------------------%x\n",(UINT32)fUI_Msg);
    switch (fUI_Msg)
    {
    case _0_KEY_MESSAGE:
    case _1_KEY_MESSAGE:
    case _2_KEY_MESSAGE:
    case _3_KEY_MESSAGE:
    case _4_KEY_MESSAGE:
    case _5_KEY_MESSAGE:
    case _6_KEY_MESSAGE:
    case _7_KEY_MESSAGE:
    case _8_KEY_MESSAGE:
    case _9_KEY_MESSAGE:
        //case _100_KEY_MESSAGE:
        
        fw_timer_event_CancelTimerEvent(COsdChnInputTimeoutEvent);
        ucChnNum = ucChnNum * 10 + ap_osdcore_irkey_to_num(fUI_Msg);
        fw_timer_event_ReactiveTimerEvent(2500, COsdChnInputTimeoutEvent);//Tim 0325
        break;

    case _CHN_INPUT_TIMEOUT:
    case _OK_KEY_MESSAGE:
   case _OK_IR_MESSAGE:
        //RTD_Log(LOGGER_INFO, "Chn input timeout\n");
        goto ChnInputFinish;

    case _EXIT_KEY_MESSAGE:
    
    //RTD_Log(LOGGER_LEVEL,"\n _SHOW_INFO_STATE bCheckCCStatus=%d \n",(UINT32)bCheckCCStatus);
        COsdCoreExitOsd();
        break;

    default:
        COsdCoreDispatchMsg();
        break;
    }

    COsdCoreChnInputBox(ucChnNum, 1);

    //if (ucChnNum <= 20)//Tim 0325
        return;


ChnInputFinish:
    //COsdCoreDebugDelay(1);
    if(COsdCtrlGetNtscFree())//Table
    {
        if(GET_TV_CUR_NtscType() == 0)//Cable
            MaxChn = 125;
        else
            MaxChn = 69;
    }
    else
		MaxChn = _MAX_CATV_NUM;

    if ((ucChnNum<1)||(ucChnNum>MaxChn)|| (GET_INPUTSOURCE_TYPE() != _SOURCE_TV))
    {
        COsdCoreExitOsd();
        return;
    }
	if(GET_CHANNEL_SKIP(ucChnNum)==_CHN_STS_SKIP)
	{
        COsdCoreExitOsd();
        return;
    }
    fw_timer_event_CancelTimerEvent(COsdChnInputTimeoutEvent);
//	fw_timer_event_DelayXms(250);
//	COsdCoreExitOsd();

	CurChn = GET_TV_CHN_DATA(_CUR_CHN_TV);

    if ( COsdCtrlGetSignalSource() != _SOURCE_INDEX_TV)
    {
        SET_TV_PRE_CHN(CurChn);
        SET_TV_CUR_CHN(ucChnNum);
        COsdCtrlSetSignalSource(_SOURCE_INDEX_TV);
    }
    else
    {
        if (!CTvSetChannel(ucChnNum))
        {
        
        //RTD_Log(LOGGER_LEVEL,"\n _SHOW_INFO_STATE bCheckCCStatus=%d \n",(UINT32)bCheckCCStatus);
            COsdCoreExitOsd();
		}
        //COsdCoreSourceInfo(_TRUE);
        COsdCoreShowTVChannel(ucChnNum);
        ucOsdState = _SHOW_INFO_STATE;
    }

    fw_ui_message_clearallType(fUI_Msg);

#if 0
    if ( ucOsdCoreBuf[0] >= 1 && ucOsdCoreBuf[0] <= _MAX_PAL_CATV_NUM)
    {
        COsdCoreShowTVChannel(ucOsdCoreBuf[0], _BIT0);
    }
    ScalerTimer_ActiveTimerEvent(ap_osdctrl_get_osd_timeout_ms(), timerevent_ui_table_OSD_timeout_event);
#endif
}
#endif

/**
 * To dispatch osd message to upper state.
 * @return none.
 */
void COsdCoreDispatchMsg(void)
{
    //RTD_Log(LOGGER_LEVEL,"\n Tim herer111\n");

//	UINT8 idx;
    switch (fUI_Msg)
    {
    case _OSD_TIMEOUT_MESSAGE:
    
    //RTD_Log(LOGGER_LEVEL,"\n _OSD_TIMEOUT_MESSAGE66 Tim bCheckCCStatus =%d \n",(UINT32)bCheckCCStatus);
        COsdCoreExitOsd();
        break;

    case _SLEEP_KEY_MESSAGE:
    case _MUTE_KEY_MESSAGE:
    case _SOURCE_KEY_MESSAGE:
    case _SOURCE_IR_MESSAGE:
    case _DISPLAY_KEY_MESSAGE:
    case _DISPLAY_MESSAGE:
    case _WIDE_KEY_MESSAGE:
    case _VIDEO_KEY_MESSAGE:
    case _AUDIO_KEY_MESSAGE:
    case _MENU_KEY_MESSAGE:
    case _VOLUMNBAR_KEY_MESSAGE:
    case _LEFT_KEY_MESSAGE:
    case _RIGHT_KEY_MESSAGE:
    //case _LEFT_IR_MESSAGE:
    //case _RIGHT_IR_MESSAGE:
    case _VOLUME_DEC_MESSAGE:
    case _VOLUME_INC_MESSAGE:
    case _FREEZE_KEY_MESSAGE:
    case _CAPTION_KEY_MESSAGE:
    case _TELETEXT_KEY_MESSAGE:
    case _EXIT_KEY_MESSAGE:	
    case _FL2UI_MAIN_NO_SUPPORT:	
    case _CC_KEY_MESSAGE:
    case _DISPLAY_RATIO_KEY_MESSAGE:
	//case _DISPLAY_RATIO_IR_MESSAGE:	
    case _RETURN_KEY_MESSAGE:
    //RTD_Log(LOGGER_LEVEL,"\n _OSD_TIMEOUT_MESSAGE77 Tim bCheckCCStatus =%d \n",(UINT32)bCheckCCStatus);
        COsdCoreExitOsd();
        fw_ui_message_push(fUI_Msg);
        //RTD_Log(LOGGER_LEVEL,"\n Tim herer\n");
        //ucOsdState = _IDLE_STATE;
        break;

    case _JUMP_KEY_MESSAGE:
    case _UP_KEY_MESSAGE:
    case _DOWN_KEY_MESSAGE:

    case _CHANNEL_DEC_MESSAGE:
    case _CHANNEL_INC_MESSAGE:
    case _0_KEY_MESSAGE:
    case _1_KEY_MESSAGE:
    case _2_KEY_MESSAGE:
    case _3_KEY_MESSAGE:
    case _4_KEY_MESSAGE:
    case _5_KEY_MESSAGE:
    case _6_KEY_MESSAGE:
    case _7_KEY_MESSAGE:
    case _8_KEY_MESSAGE:
    case _9_KEY_MESSAGE:
    case _100_KEY_MESSAGE:
#if 0
        LOAD_STORAGE_DATA(StructSystemDataType, InputSource, idx);
        if (idx ==_SOURCE_INDEX_TV)
        {
            if (ucOsdState == _SHOW_NO_SIGNAL_STATE)
            {
                //fw_timer_event_CancelTimerEvent(COsdFxShowFlashScreenStatus);
            }
            fw_ui_message_push(fUI_Msg);
            ucOsdState = _IDLE_STATE;
            break;
        }
        else
#endif
            //RTD_Log(LOGGER_LEVEL,"\n _SHOW_INFO_STATE bCheckCCStatus=%d \n",(UINT32)bCheckCCStatus);

         //   COsdCoreExitOsd();
        //fw_ui_message_push(fUI_Msg);
        //COsdCoreInitMainMenu();
        return;

    default:
        return;
    };
    //CLR_DONT_ENABLE_OSD_WHEN_CHANGE_SRC();

}


//Tim 0418
//==============================================================================
void CCCTohotKeyStatus(void)
{
	#ifdef CONFIG_VBI_CC_SUPPORT
	if(GET_CLOSEDCAPTION() != 0)
	{
	    fw_timer_event_CancelTimerEvent(COsdEventCloseVbi);
	    COsdVbiShowData(0);
	    COsdVbiBlendingOperation(0);
	    CLR_OSDVBI_INITIAL();
	    
	    bCcStartFlag = _FALSE;
	    ap_osdfx_map_setting(_OSD_MAP0);
	    // Load OSD palette
	    ap_osdfx_reload_osdpalette();
	    // Load Font
	    ap_osdfx_load_vlc_font(tFONT_GLOBAL, 0x00, 0x60);
	}
	#endif
}

//Tim 0331
//==============================================================================


//==============================================================================
/**
 * Hotkey handler
 * @return hotkey handle status. _TRUE : hotkey was be processed.
 */



static UINT8 ap_osdcore_hotkey_handler(void)
{
	#ifdef CONFIG_VBI_CC_SUPPORT
	UINT8 CcType;
	#endif
	#if 0
	UINT8 ucSleepMode;
	#endif
	UINT8 ucDisplayRatio;
    if (ucOsdState == _AUTO_SCAN_STATE || ucOsdState == _MANUAL_SCAN_STATE)
        return _FALSE;

    if (ucOsdState == _AUTOWB_STATE)
        return _FALSE;


    switch (fUI_Msg)
    {
    #ifdef CONFIG_VBI_CC_SUPPORT
        case _CC_KEY_MESSAGE:
        if ((GET_INPUTSOURCE_TYPE()==_SOURCE_CVBS) ||
            (GET_INPUTSOURCE_TYPE()==_SOURCE_SV)   ||
            (GET_INPUTSOURCE_TYPE()==_SOURCE_TV)    )
        {
            CCCTohotKeyStatus();
            COsdCoreExitOsd();
            CcType = GET_CLOSEDCAPTION();
            CcType ++;
            if(CcType > 2)
                CcType = 0;
            
            COsdFxDisableOsd();
            CCloseCCStatus();

            
            COsdCoreShowCCType(CcType);
            SET_CLOSEDCAPTION(CcType);
            SYNC_STORAGE_DATA(StructSystemDataType,  CcSystem, CcType);

        }
        break;
	#endif
	#if 0
       case _SLEEP_KEY_MESSAGE:
           /*uwOsdCoreModeKey = _SLEEP_KEY_MESSAGE;
           ucOsdState = _MODE_BOX_STATE;
           pListItem = &tSleepModeList;
           COsdCoreDispModeBox(_OPT_REDRAW);*/
           ucShowSleepFlag = 0;
           ucSleepMode = COsdCtrlGetSleepMode();
           COsdSetSleepMode();
           
           COsdFxDisableOsd();
           CCloseCCStatus();

           ucShowSleepFlag = 1;
           ucOsdState = _HOTKEY_STATE;
           
           InitOsdFrame();
           COsdCoreShowSleepType();
           if(ucSleepType == 0)
           	ucShowSleepFlag = 0;
           break;
	#endif
        case   _DIMMER_KEY_MESSAGE:
			{
			
				#if 0
				SET_BRIGHTNESS(message);
				if ( COsdCoreGetState() == _SLIDER_STATE && GET_PICTURE_MODE() != _PIC_MODE_USER) // Bug PR00038: cf.tsai resolved 071003
				{
					SET_PICTURE_MODE(_PIC_MODE_USER);
				}
				
				if (GET_PICTURE_MODE() == _PIC_MODE_USER)
				#endif
				if(info->state == _MODE_STATE_ACTIVE)
				{
					if(g_dimmer==0)
					{
						
						BriConIdx_temp=GET_BRIGHTNESS();
						SET_BRIGHTNESS(30);	
						g_dimmer=1;
					}
					else
					{
						
						SET_BRIGHTNESS(BriConIdx_temp);
						g_dimmer=0;
					}
					SYNC_STORAGE_DATA(StructSystemDataType,  DimmerVaule, BriConIdx_temp);
					COsdCtrlSetYCB();
				}
				break;
			}
		   
	    case _DISPLAY_RATIO_KEY_MESSAGE:
		case _DISPLAY_RATIO_IR_MESSAGE:
			 if(ucOsdState<_MAINMENU_STATE||ucOsdState>_LIST_MENU_STATE)
			{
		        CCCTohotKeyStatus();
		        if(info->state == _MODE_STATE_ACTIVE)
		        {
		            COsdCoreExitOsd();
		            switch(GET_OSD_DISPLAY_RATIO())
		            {
		                case RATIO_AUTO:
		                    SET_OSD_DISPLAY_RATIO(RATIO_4_3);
		                    ucDisplayRatio = RATIO_4_3;
		                    break;
		                    
		                case RATIO_16_9:
		                    SET_OSD_DISPLAY_RATIO(RATIO_4_3);
		                    ucDisplayRatio = RATIO_4_3;
		                    break;

		                case RATIO_4_3:
		                default:
		                    SET_OSD_DISPLAY_RATIO(RATIO_16_9);
		                    ucDisplayRatio = RATIO_16_9;
		                    break;
		            }

		            SYNC_STORAGE_DATA(StructSystemDataType,  ZoomMode, ucDisplayRatio);
		            if (ucDisplayRatio == 2) // auto default set to 4:3
		            {
		                SET_DISPLAY_RATIO(RATIO_16_9);
		            }
		            else
		                SET_DISPLAY_RATIO(ucDisplayRatio);

		            DisplayRatioFlag = _ON;
		            

		            fw_flow_message_push(_UI2FL_MAIN_DISPLAYRATIO_CHANGE);  

		        }
			}
	    break;

    default:
        break;
    }


    return _FALSE;
}
#if 0
/**
 * OSD power off event handler
 * @return none.
 */
static void ap_osdcore_poweroff_proc(void)
{
    RTD_Log(LOGGER_INFO, "[OSD] Power off\n");

    while (1);
}
#endif

#if 0 // Temporily not use
/**
 * To display Logo.
 * @return none.
 */
void COsdCoreDispLogo(void)
{
    UINT8 j, k;

    ap_osdfx_map_setting(_OSD_MAP0);

    ap_osdfx_load_vlc_font(tFont_Logo, 0x00, 0x60);

    for (k=0; k<5; k++)
        COsdFxHLine(2+k, 0, _OSD_ITEM_ROW_LENGTH, 0x0f, _WRITE_BYTE1);
    for (k=0; k<3; k++)
    {
        for (j=0; j<16; j++)
            COsdFxWrite1BitFont(2+k, j, k*16+j, _CP_WHITE<<4 | _CP_BLACK);
        for (j=0; j<16; j++)
            COsdFxWrite1BitFont(2+k, 16+j, (k+3)*16+j, _CP_WHITE<<4 | _CP_BLACK);
    }

    COsdFxEnableOsd();
}
#endif

#ifdef _VIDEO_TV_SUPPORT
/**
 * TV channel scan message event handler.
 * @return none.
 */
void COsdCoreChannelScanEventProc(void)
{
    switch (fUI_Msg)
    {
    case _MENU_KEY_MESSAGE:
    case _MENU_IR_MESSAGE:
    case _EXIT_KEY_MESSAGE:
        //CTvAutoScanStop();
        fw_flow_message_push(_UI2FL_TV_AUTOSCAN_CANCEL);	// inform flow to cancel channel scan
        //RTD_Log(LOGGER_LEVEL,"\n _SHOW_INFO_STATE bCheckCCStatus=%d \n",(UINT32)bCheckCCStatus);

        COsdCoreExitOsd();
        break;

    default:
        break;
    }
}
#endif

/**
 * To display mute icon.
 * @return none.
 */
void COsdCoreDrawMuteIcon(bit bMute)
{
    UINT8 code tMuteIcon[][4] =
    {
        {FntVolume_StartAddr, FntVolume_StartAddr+1, FntVolume_StartAddr+2, _END_},
        {FntVolume_StartAddr+3, FntVolume_StartAddr+4, FntVolume_StartAddr+5, _END_}
    };
    //COsdFxDisableOsd();
    InitOsdFrame();
    SetOSDDouble(0x03);
    ap_osdfx_map_setting(_OSD_MAP0);
    ucOsdState = _HOTKEY_STATE;

    COsdFxSetWinRefPos(_OSD_MUTE_H_POS+20, _OSD_MUTE_V_POS);

    if ( bMute)
        ap_osdfx_load_vlc_font(tFont_MuteIcon, FntVolume_StartAddr, 6);
    else
        ap_osdfx_load_vlc_font(tFont_UnmuteIcon, FntVolume_StartAddr, 6);

    COsdFxDrawWindow(&tMuteBgWindowStyle);
    COsdFxLoadText(1, 0, 0, 0xF0, 0,tMuteIcon[0]);
    COsdFxLoadText(2, 0, 0, 0xF0, 0,tMuteIcon[1]);
    
    COsdFxEnableOsd();
	//RTD_Log(LOGGER_LEVEL,"\n Tim ucSleepType = %d \n",(UINT32)ucSleepType);
    if((!bMute)  || ucShowSleepFlag)
    
	fw_timer_event_ReactiveTimerEvent(SEC(3), timerevent_ui_table_OSD_timeout_event);//Tim 0404

}
#if (_LOGOL_ENABLE)
void COsdCoreDrawLogo(UINT8 ucDouble)
{
    UINT8 x;
    UINT8 y;
    UINT8 ucFont = 0x00;

    COsdFxDisableOsd();
    ap_osdfx_map_setting(_OSD_MAP0);
    ap_osdfx_load_vlc_font(tFont_LOGO, 0x00, 48);
    RTD_Log(LOGGER_INFO,"draw logo--\n");
    ucOsdState = _LOGOL_STATE;
    // Init osd
    // insert code to here
    for (x = 0 ; x < 18 ; x++)
    {
        OSDLine(x, 0, 46, 0x8C, _WRITE_BYTE0);
        OSDLine(x, 0, 46, 0x0F, _WRITE_BYTE1);
        OSDLine(x, 0, 46, 0xE0, _WRITE_BYTE2);
    }
    SetOSDDouble(ucDouble);
    flow_display_BackgroundColor(0, 0, 0xff);
    COsdFxSetWinRefPos(_OSD_Logo_H_POS+3, _OSD_Logo_V_POS);

    // 1BIT font
    for (y = 1; y < 4; y++)
    {
        for (x = 0; x < 16; x++)
        {
            COsdFxWrite1BitFont(y, x, ucFont, 	0x40); // right-arrow sign
            ucFont++;
        }
    }

    COsdFxEnableOsd();
    fw_timer_event_CancelTimerEvent(timerevent_ui_table_OSD_timeout_event);
    bloadfont = 1;
    //fw_ui_message_push(_FL2UI_LOAD_MAINFONT);
}
#endif

#ifdef _VIDEO_TV_SUPPORT

/**
 * TV channel manual scan message event handler.
 * @return none.
 */
void COsdCoreManualScanEventProc(void)
{
	#ifdef CONFIG_TUNER_SILICON 
    UINT8 CurChn;
    #endif
    if (fUI_Msg == _REPEAT_KEY_MESSAGE)
        return;

    switch (fUI_Msg)
    {
//	case _MANUAL_SCAN_UPDATE_OSD:
//		fw_timer_event_CancelTimerEvent(timerevent_ui_table_OSD_timeout_event);
//		ShowManualAutoCan_Value(CTvGetCurScanChn(), CTvGetManualScanFreq());
//		break;

//	case _MANUAL_SCAN_STORE_CHANNEL:
//		CTvManualScanStoreChannel();
//
//		if ( (GET_CHANNEL_COLORSYSTEM(GET_TV_CUR_CHN())&0x0f) == 0x01 )
//		{	// Standard is 60Hz
//			if ( disp_info[_MAIN_DISPLAY].ucMode_Curr != _MODE_480I)	// mismatch! Reset state
//				State_MessagePush(_MODE_MSG_MAIN_DETECT_SIGNAL);
//		}
//		else
//		{	// Standard is 50 Hz
//			if ( disp_info[_MAIN_DISPLAY].ucMode_Curr != _MODE_576I)	// mismatch! Reset state
//				State_MessagePush(_MODE_MSG_MAIN_DETECT_SIGNAL);
//		}
//		//sms 2007-12-07
//		ShowManualScanNote();
//		COsdCoreSetManualScanMute(ADC_UNMUTE);
//		fw_timer_event_ReactiveTimerEvent(ap_osdctrl_get_osd_timeout_ms(), timerevent_ui_table_OSD_timeout_event);
//		break;

    case _EXIT_KEY_MESSAGE:
    case _MENU_KEY_MESSAGE:
    case _MENU_IR_MESSAGE:
        fw_flow_message_push(_UI2FL_TV_MENUALSCAN_CANCEL);	// inform flow to cancel channel scan
        COsdFxDisableOsd();
        ap_osdfx_map_setting(_OSD_MAP0);
        COsdCoreDrawMainMenuFrame(stLayerStack[ucLayerIndex].ucMainMenu);
        COsdCoreDispMenuTitle(stLayerStack[ucLayerIndex].pMenuItem);
        COsdCoreMoveHighlight(stLayerStack[ucLayerIndex].ucMainMenu, stLayerStack[ucLayerIndex].ucSubMenu, _REFRESH_SUB);
        COsdFxEnableOsd();
        ucOsdState = _MAINMENU_STATE;
#ifdef CONFIG_TUNER_SILICON //tommy,prevent tv no sound
		CurChn = GET_TV_CHN_DATA(_CUR_CHN_TV);

		if(tvManualScanFlag==1)
			TvSetChannel(CurChn);	
#endif
        break;

    case _UP_KEY_MESSAGE:
        break;

    case _DOWN_KEY_MESSAGE:
        break;

    case _LEFT_KEY_MESSAGE:
    case _LEFT_IR_MESSAGE:
        if (tvManualScanFlag)
            flow_audio_Setmute(_ENABLE, SCALER, (ADC_CH_IDX)(ADC_CH_L|ADC_CH_R|ADC_CH_C|ADC_CH_SW));

        SET_TV_MANUAL_SCAN_DIRECTION(_MANUAL_SCAN_DEC);
        tvScanCurFreq -= _TV_TUNER_SUB_STEP*4;
        tvManualScanFlag = 1;
#if 0
        tvScanCurFreq -= (ucTunerSubStep*4);
        if (tvScanCurFreq < usTunerMinFreq)
            tvScanCurFreq = usTunerMaxFreq - ucTunerSubStep;
        //       ClearManualScanNote();
#endif
        fw_timer_event_CancelTimerEvent(timerevent_ui_table_OSD_timeout_event);
        break;

    case _RIGHT_KEY_MESSAGE:
    case _RIGHT_IR_MESSAGE:
        if (tvManualScanFlag)
            flow_audio_Setmute(_ENABLE, SCALER, (ADC_CH_IDX)(ADC_CH_L|ADC_CH_R|ADC_CH_C|ADC_CH_SW));

        SET_TV_MANUAL_SCAN_DIRECTION(_MANUAL_SCAN_INC);
        tvScanCurFreq += _TV_TUNER_SUB_STEP*4;
        tvManualScanFlag = 1;
#if 0
        tvScanCurFreq += (ucTunerSubStep*4);
        //tvScanCurFreq += 0x20;
        if ( tvScanCurFreq > usTunerMaxFreq)
            tvScanCurFreq = usTunerMinFreq + ucTunerSubStep;
        //	ClearManualScanNote();
#endif
        fw_timer_event_CancelTimerEvent(timerevent_ui_table_OSD_timeout_event);
        break;
    case _SHOW_OSD_MESSAGE:
        COsdFxEnableOsd();
        break;

    default:
        break;
    }
}
#endif
/**
 *
 * @return none.
 */
void COsdCoreDrawEqSlider(UINT8 ucRow, UINT8 ucCol, UINT8 ucValue)
{
#define _SLIDER_LEN		(8)

    UINT8 j;
    UINT8 thumbPos, thumbFinePos;
    UINT16 uwTmp;

    uwTmp = _SLIDER_LEN * 9 - 1; // max step
    thumbPos = ucValue - _OSD_EQ_MIN_VALUE;
    uwTmp = uwTmp * thumbPos;
    thumbPos = _OSD_EQ_MAX_VALUE - _OSD_EQ_MIN_VALUE;
    uwTmp = uwTmp / thumbPos;
    thumbPos = (UINT8)uwTmp;

    thumbFinePos = thumbPos % 9; // mode 9
    thumbPos = thumbPos / 9;

    COsdFxSet2BitColor(0, 0);
    COsdFxSet2BitColor(1, 5);
    COsdFxSet2BitColor(2, 1);
    COsdFxSet2BitColor(3, 4);

    COsdFxWrite2BitFont(ucRow, ucCol, _SLIDER_2BIT_FS+0x16);
    COsdFxWrite2BitFont(ucRow, ucCol+1, _SLIDER_2BIT_FS+0x17);

    for (j=0; j<_SLIDER_LEN; j++)
    {
        if ( j < thumbPos)
        {
            COsdFxWrite2BitFont(ucRow-1-j, ucCol, _SLIDER_2BIT_FS+0x14);
            COsdFxWrite2BitFont(ucRow-1-j, ucCol+1, _SLIDER_2BIT_FS+0x15);
        }
        else if ( j > thumbPos)
        {
            COsdFxWrite2BitFont(ucRow-1-j, ucCol, _SLIDER_2BIT_FS+0x14);
            COsdFxWrite2BitFont(ucRow-1-j, ucCol+1, _SLIDER_2BIT_FS+0x15);
        }
        else
        {
            COsdFxWrite2BitFont(ucRow-1-j, ucCol, _SLIDER_2BIT_FS+thumbFinePos*2);
            COsdFxWrite2BitFont(ucRow-1-j, ucCol+1, _SLIDER_2BIT_FS+thumbFinePos*2+1);
            if ( thumbFinePos == 8 && j < _SLIDER_LEN-1)
            {
                j++;
                COsdFxWrite2BitFont(ucRow-1-j, ucCol, _SLIDER_2BIT_FS+0x12);
                COsdFxWrite2BitFont(ucRow-1-j, ucCol+1, _SLIDER_2BIT_FS+0x13);
            }
        }
    }
    if (thumbPos == _SLIDER_LEN-1 && thumbFinePos == 8)
    {
        COsdFxWrite2BitFont(ucRow-1-j, ucCol, _SLIDER_2BIT_FS+0x1a);
        COsdFxWrite2BitFont(ucRow-1-j, ucCol+1, _SLIDER_2BIT_FS+0X1b);
    }
    else
    {
        COsdFxWrite2BitFont(ucRow-1-j, ucCol, _SLIDER_2BIT_FS+0x18);
        COsdFxWrite2BitFont(ucRow-1-j, ucCol+1, _SLIDER_2BIT_FS+0X19);
    }

#undef _SLIDER_LEN
}

/**
 * To update audio Eq slider OSD.
 * @return none.
 */
void COsdCoreUpdateAudioEqSlider(UINT8 ucItem)
{
    UINT8 ucValue;

    ucValue = EQBar_Value[ucItem];
    if ( ucValue > _OSD_EQ_MAX_VALUE)
    {
        EQBar_Value[ucItem] = ucValue = _OSD_EQ_MAX_VALUE;
    }
    if ( ucValue < _OSD_EQ_MIN_VALUE)
    {
        EQBar_Value[ucItem] = ucValue = _OSD_EQ_MIN_VALUE;
    }

    COsdCoreDrawEqSlider(12, 1+ucItem*5, ucValue);

    if ( ucValue > _OSD_EQ_MID_VALUE)
    {
        COsdFxWrite1BitFont(2, 2+ucItem*5, _PLUS_, FONT_COLOR(_CP_WHITE, _CP_BLACK));
        ucValue = ucValue - _OSD_EQ_MID_VALUE;
    }
    else if ( ucValue < _OSD_EQ_MID_VALUE)
    {
        COsdFxWrite1BitFont(2, 2+ucItem*5, _MINUS_, FONT_COLOR(_CP_WHITE, _CP_BLACK));
        ucValue = _OSD_EQ_MID_VALUE - ucValue;
    }
    else
    {
        COsdFxWrite1BitFont(2, 2+ucItem*5, 0x00, FONT_COLOR(_CP_WHITE, _CP_BLACK));
        ucValue = ucValue - _OSD_EQ_MID_VALUE;
    }
    COsdFxShowNumber(2, 3+ucItem*5, ucValue, 0x03, _CP_WHITE);

}

/**
 * To set highlight window position of audio Eq control menu.
 * @return none.
 */
void COsdCoreSetAudioEqHighlightPos(UINT8 ucItem)
{
    stItemPos = tEqControl_highlight_window_pos;
    stItemPos.usHStart = tEqControl_highlight_window_pos.usHStart + ucItem*(12*5);
    stItemPos.usHEnd = tEqControl_highlight_window_pos.usHEnd + ucItem*(12*5);
    COsdFxMoveWindow(tSource_Select_Highlight_WindowStyle.ucWindowID, &stItemPos);
}

/**
 * To initial Eq adjustment menu.
 * @return none.
void COsdCoreInitAudioEqControlMenu(void)
{
	UINT8 j;
	UINT8 ucCol;

	ap_osdfx_map_setting(_OSD_MAP0);
	COsdFxSetWinRefPos(_OSD_EQ_MENU_H_POS, _OSD_EQ_MENU_V_POS);

	//ap_osdfx_load_vlc_font(tFont_SourcePatch, _FRAME_2BIT_PATCH_ADDR, 12);

	COsdFxSet2BitColor(0, _CP_BLACK);
	COsdFxSet2BitColor(1, _CP_WHITE);
	COsdFxSet2BitColor(2, _CP_PASTEL_CYAN_BLUE);
	COsdFxSet2BitColor(3, _CP_DARKER_CYAN_BLUE);

	// patch the title bar
	COsdFxWrite2BitFont(1, 0, _FRAME_PATCH_FS+3);
	for(j=1;j<10;j++)
		COsdFxWrite2BitFont(1, j, _FRAME_PATCH_FS+4);
	COsdFxWrite2BitFont(1, j, _FRAME_PATCH_FS+5);

	// patch the hint bar
	COsdFxLoadBlankCmd(14, 0, 244, _CP_BLACK);
	COsdFxWrite2BitFont(14, 1, _FRAME_PATCH_FS);
	for(j=0;j<11;j++)
		COsdFxWrite2BitFont(14, 2+j, _FRAME_PATCH_FS+1);
	COsdFxWrite2BitFont(14, 2+j, _FRAME_PATCH_FS+2);

	// load EQ value
//	for (j=0; j<6; j++)
//		EQBar_Value[j] = stDataFacModeType.Sound_Value[COsdCtrlGetAudioEQ()][j];
		EQBar_Value[0] = ui_get_eq_300(GET_EQ_MODE());
		EQBar_Value[1] = ui_get_eq_1k(GET_EQ_MODE());
		EQBar_Value[2] = ui_get_eq_2k(GET_EQ_MODE());
		EQBar_Value[3] = ui_get_eq_3k(GET_EQ_MODE());
		EQBar_Value[4] = ui_get_eq_8k(GET_EQ_MODE());
		EQBar_Value[5] = ui_get_eq_15k(GET_EQ_MODE());

	// DISPLAY TITLE
	COsdFxLoadText(0, 0, 20, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tStr_Audio_Equalizer);

	for(j=3; j<13; j++)
		COsdFxLoadBlankCmd(j, 0, 24, _CP_BLACK);

	for(j=0; j<6; j++)
		COsdCoreUpdateAudioEqSlider(j);

	// Display label
	ucCol = 0;
	ucCol += COsdFxLoadText(13, ucCol, 14, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tStr_200Hz);
	ucCol += COsdFxLoadText(13, ucCol, 22, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tStr_300Hz);
	ucCol += COsdFxLoadText(13, ucCol, 26, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tStr_1KHz);
	ucCol += COsdFxLoadText(13, ucCol, 26, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tStr_3KHz);
	ucCol += COsdFxLoadText(13, ucCol, 26, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tStr_8KHz);
	ucCol += COsdFxLoadText(13, ucCol, 26, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tStr_15KHz);
	ucCol += COsdFxLoadText(13, ucCol, 12, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tStr_Hz);

	COsdFxDrawWindow(&tSource_Select_Title_WindowStyle);
	COsdFxMoveWindow(tSource_Select_Title_WindowStyle.ucWindowID, &tEqControl_title_window_pos);
	COsdFxDrawWindow(&tSource_Select_Window_Style);
	COsdFxMoveWindow(tSource_Select_Window_Style.ucWindowID, &tEqControl_menu_window_pos);
	COsdFxDrawWindow(&tSource_Select_Hint_WindowStyle);
	COsdFxMoveWindow(tSource_Select_Hint_WindowStyle.ucWindowID, &tEqControl_hint_window_pos);
	COsdFxDrawWindow(&tSource_Select_Highlight_WindowStyle);
	//COsdFxMoveWindow(tSource_Select_Highlight_WindowStyle.ucWindowID, &tEqControl_highlight_window_pos);

	ucOsdState = _EQ_CONTROL_STATE;

	ucListSelectedItem = 0;
	COsdCoreSetAudioEqHighlightPos(ucListSelectedItem);

	COsdFxEnableOsd();
}
 */
/**
 * Audio Equalilzer menu message event handler.
 * @return none.
 */
void COsdCoreEqControlEventProc(void)
{
    APP_EQ_VAL	t_val_eq;
    INT16		val_temp;
    //UINT8		getVal;
    UINT8		i;

    if (_OSD_EQ_MID_VALUE)
    {
        t_val_eq.max_val = _OSD_EQ_MAX_VALUE - _OSD_EQ_MID_VALUE;
        t_val_eq.min_val = _OSD_EQ_MIN_VALUE - _OSD_EQ_MID_VALUE;
    }


    if (fUI_Msg == _REPEAT_KEY_MESSAGE)
    {
        if ((uwUI_PreMsg == _LEFT_KEY_MESSAGE) || (uwUI_PreMsg == _RIGHT_KEY_MESSAGE) || (uwUI_PreMsg == _UP_KEY_MESSAGE) || (uwUI_PreMsg == _DOWN_KEY_MESSAGE))
            fUI_Msg = uwUI_PreMsg;
    }

    switch (fUI_Msg)
    {
    case _LEFT_KEY_MESSAGE:
        ucListSelectedItem = (ucListSelectedItem + 5) % 6;
        COsdCoreSetAudioEqHighlightPos(ucListSelectedItem);
        //RTD_Log(LOGGER_INFO,"[EQ][PrevBand]\n");
        break;

    case _RIGHT_KEY_MESSAGE:
        ucListSelectedItem = (ucListSelectedItem + 1) % 6;
        COsdCoreSetAudioEqHighlightPos(ucListSelectedItem);
        //RTD_Log(LOGGER_INFO,"[EQ][NextBand]\n");
        break;

    case _UP_KEY_MESSAGE:
        if ( EQBar_Value[ucListSelectedItem] < _OSD_EQ_MAX_VALUE)
        {
            EQBar_Value[ucListSelectedItem]++;
            COsdCoreUpdateAudioEqSlider(ucListSelectedItem);

            //RTD_Log(LOGGER_INFO,"[EQ][Band%d][Up] ==> %d\n",(INT32)ucListSelectedItem,(INT32)EQBar_Value[ucListSelectedItem]);
            for (i=0; i<APP_EQ_BAND_NUM; i++)
            {
                val_temp = (INT16)EQBar_Value[i] - _OSD_EQ_MID_VALUE;
                t_val_eq.val[i] = val_temp;
                //RTD_Log(LOGGER_INFO,"[EQ][Band%d] = %d\n",(INT32)i,(INT32)val_temp);
            }
            Audio_HwpSetEQValue(_ENABLE, &t_val_eq);
        }
        break;

    case _DOWN_KEY_MESSAGE:
        if ( EQBar_Value[ucListSelectedItem] > _OSD_EQ_MIN_VALUE)
        {
            EQBar_Value[ucListSelectedItem]--;
            COsdCoreUpdateAudioEqSlider(ucListSelectedItem);

            //RTD_Log(LOGGER_INFO,"[EQ][Band%d][Down] ==> %d\n",(INT32)ucListSelectedItem,(INT32)EQBar_Value[ucListSelectedItem]);
            for (i=0; i<APP_EQ_BAND_NUM; i++)
            {
                val_temp = (INT16)EQBar_Value[i] - _OSD_EQ_MID_VALUE;
                t_val_eq.val[i] = val_temp;
                //RTD_Log(LOGGER_INFO,"[EQ][Band%d] = %d\n",(INT32)i,(INT32)val_temp);
            }
            Audio_HwpSetEQValue(_ENABLE, &t_val_eq);
        }
        break;

    case _EXIT_KEY_MESSAGE:
//		COsdCoreExitOsd();
//		break;
    case _MENU_KEY_MESSAGE:
        COsdFxDisableOsd();
        ui_set_eq_300(GET_EQ_MODE(), EQBar_Value[0]);
        ui_set_eq_1k(GET_EQ_MODE(), EQBar_Value[1]);
        ui_set_eq_2k(GET_EQ_MODE(), EQBar_Value[2]);
        ui_set_eq_3k(GET_EQ_MODE(), EQBar_Value[3]);
        ui_set_eq_8k(GET_EQ_MODE(), EQBar_Value[4]);
        ui_set_eq_15k(GET_EQ_MODE(), EQBar_Value[5]);
        COsdCoreFallBackMenu();
        break;

    case _OSD_TIMEOUT_MESSAGE:
    
    //RTD_Log(LOGGER_LEVEL,"\n _SHOW_INFO_STATE bCheckCCStatus=%d \n",(UINT32)bCheckCCStatus);
        COsdCoreExitOsd();
        ui_set_eq_300(GET_EQ_MODE(), EQBar_Value[0]);
        ui_set_eq_1k(GET_EQ_MODE(), EQBar_Value[1]);
        ui_set_eq_2k(GET_EQ_MODE(), EQBar_Value[2]);
        ui_set_eq_3k(GET_EQ_MODE(), EQBar_Value[3]);
        ui_set_eq_8k(GET_EQ_MODE(), EQBar_Value[4]);
        ui_set_eq_15k(GET_EQ_MODE(), EQBar_Value[5]);
        break;

    default :
        break;
    }
    //getVal = GET_OSD_TIMEOUT();
    if (( fUI_Msg < _TTX_EXIT_MESSAGE)&&(fUI_Msg!=_FL2UI_MAIN_NO_SIGNAL))//Hardy20091209
        fw_timer_event_ReactiveTimerEvent(ap_osdctrl_get_osd_timeout_ms(), timerevent_ui_table_OSD_timeout_event);
}
void COsdCtrlSetEQ(UINT8 index,UINT8 value)
{
    APP_EQ_VAL	t_val_eq;
    INT16		val_temp;
    UINT8		i;
    RTD_Log(LOGGER_INFO,"[EQ][index-------=%x][Up] -------vlaue==> %x\n",(INT32)index,(INT32)value);
    EQBar_Value[index] = value;
    for (i=0; i<APP_EQ_BAND_NUM; i++)
    {
        val_temp = (INT16)EQBar_Value[i] - _OSD_EQ_MID_VALUE;
        t_val_eq.val[i] = val_temp;
        //RTD_Log(LOGGER_INFO,"[EQ][Band%d] = %d\n",(INT32)i,(INT32)val_temp);
    }
    RTD_Log(LOGGER_INFO,"[EQ]-----vlaue==> %x\n",(INT32) EQBar_Value[index]);

    Audio_HwpSetEQValue(_ENABLE, &t_val_eq);

}

#ifdef CONFIG_TTX_SUPPORT

UINT8 xdata magNum = 1;
UINT8 xdata pageTens = 0;
UINT8 xdata pageUnits = 0;
UINT8 pageNumIdx = 0;

static void xExitSubpageHold(void)
{
    ttx_subpage_auto_rotation.hold = FALSE; // no hold
    TTX_Control(TTX_CMD_ROTATE);
}

static void xRestoreShowingPageAddr(void)
{
    TTX_CmdArg_t arg;

    magNum = m_hTtx->pageShowing.addr.pageA>>8;
    pageTens = (m_hTtx->pageShowing.addr.pageA&0xF0)>>4;
    pageUnits = (m_hTtx->pageShowing.addr.pageA&0xF);

    arg.pageNum.magNum = magNum;
    arg.pageNum.pageNumTens = pageTens;
    arg.pageNum.pageNumUnits = pageUnits;
    TTX_Control1(TTX_CMD_SET_PAGE_NUM_INDICATOR, &arg);
}
static void xResetPageAddr(void)
{
    TTX_STD_Page_AddrWithSubCode_t initPage;

    TTX_GetInitPage(&initPage);

    magNum = initPage.pageA>>8;
    pageTens = (initPage.pageA&0xF0)>>4;
    pageUnits = (initPage.pageA&0xF);
    pageNumIdx = 0;
}


static void xCalPageAddr(const UINT8 num)
{
    TTX_CmdArg_t arg;

    if ( pageNumIdx == 0 )
    {
        magNum = num;
        pageTens = 0xA;
        pageUnits = 0xA;
    }
    else if ( pageNumIdx == 1 )
    {
        pageTens = num;
    }
    else
    {
        pageUnits = num;
    }

    arg.pageNum.magNum = magNum;
    arg.pageNum.pageNumTens = pageTens;
    arg.pageNum.pageNumUnits = pageUnits;
    TTX_Control1(TTX_CMD_SET_PAGE_NUM_INDICATOR, &arg);

    if ( (pageNumIdx == 0) && ((num <= 0) || (num>=9)) )
        pageNumIdx = 0;
    else
        pageNumIdx++;
    if ( pageNumIdx >= 3 )
    {
        pageNumIdx = 0;
        fw_ui_message_push(_OK_KEY_MESSAGE);
    }
}
static void xCalSubpageAddr(const UINT8 num)
{
    TTX_STD_Page_Addr_SubCode_t subCode;
    TTX_CmdArg_t arg;

    subCode = TTX_STD_SubCodeAToSubcode(ttx_select_subpage.subcodeA);

    ttx_select_subpage.user_input_status = TRUE;

    if (subCode.s4 > 0xA )
    {
        subCode.s4 = num;
    }
    else if ( subCode.s3 > 0xA )
    {
        subCode.s3 = num;
    }
    else if ( subCode.s2 > 0xA )
    {
        subCode.s2 = num;
    }
    else if ( subCode.s1 > 0xA )
    {
        subCode.s1 = num;

        ttx_select_subpage.subcodeA = TTX_STD_SubCodeToSubcodeA(subCode);
        ttx_select_subpage.user_input_status = FALSE;

        arg.pageAddr.pageA = ttx_select_subpage.pageA;
        arg.pageAddr.subcodeA= ttx_select_subpage.subcodeA;
        arg.pageAddr.pageA&= ~(_BIT15);
        if (!TTX_Control1(TTX_CMD_GO_TO_SUBPAGE, &arg))
        {
            ttx_select_subpage.subcodeA = 0xFFFF;
            return;
        }
    }
    else
    {
        subCode.s4 = num;
        subCode.s3 = subCode.s2 = subCode.s1 = 0xF;
    }
    ttx_select_subpage.subcodeA = TTX_STD_SubCodeToSubcodeA(subCode);

    RTD_Log(LOGGER_INFO, "ttx_select_subpage.subcodeA : %x \n", (UINT32) ttx_select_subpage.subcodeA );
}

void COsdCoreTtxPageNum(UINT8 num)
{
    TTX_CmdArg_t arg;

    magNum = pageTens;
    pageTens = pageUnits;
    pageUnits = num;

    if ( m_hTtx != NULL)
    {
        arg.pageNum.magNum = magNum;
        arg.pageNum.pageNumTens = pageTens;
        arg.pageNum.pageNumUnits = pageUnits;
        TTX_Control1( TTX_CMD_SET_PAGE_NUM_INDICATOR, &arg);	/* display user's key strike */
    }
}

UINT8 xdata tt_size_mode = 0; // 0: normal, 1: scale upper helf, 2: scaler lower helf

/**
 * Teletext exit procedure
 */
static void COsdCoreTeleTextExitProc(void)
{
    if ( m_hTtx != NULL)
    {
        TTX_Control( TTX_CMD_HIDE);	/* hide teletext */
        TTX_Control( TTX_CMD_STOP);
        TTX_WaitForCompletion();
        TTX_Close();
    }

    ucOsdState = _IDLE_STATE; // stop TTX_Tick()

    EA=0;
    drv_imem4k_Stop();
    drv_imem8k_Stop();
    rtd_outb( SYS_SHARE_MEM_0x2da1, 0x05 );	// restore share-mem to UIOSD
    EA=1;

    tt_size_mode = 0;
    m_hTtx = NULL;
    if (GET_INTERLACE_MODE())	// Enable Video compensate
        rtdf_setBits(DM_UZU_CTRL_0x1604, _BIT11);

    fw_ui_message_clearallType(_TTX_SIGNAL_OK_MESSAGE); // prevent too many OK messages
    fw_ui_message_clearallType(_TTX_SIGNAL_NG_MESSAGE); // prevent that NG message has already been pushed
    fw_timer_event_CancelTimerEvent(timerevent_ui_table_OSD_NoTT_event); // don't need to detect NoTT status

    ap_osdcore_init_data();	// restore SRAM data of UIOSD
    //COsdCoreInitMainMenu();
}

/**
 * TeleText Message Evnet Handler
 * @return none.
 */
void COsdCoreTeleTextEventProc(void)
{
    ap_osdcore_num_irkey_converter();
    switch (fUI_Msg)
    {
    case _TTX_EXIT_MESSAGE:
        if ( ttx_noTT == 3 )
        {
            fw_ui_message_push(_NO_TT_SIGNAL);
        }
    case _TELETEXT_KEY_MESSAGE:
        xExitSubpageHold();
        memset(&ttx_select_subpage, 0, sizeof(ttx_select_subpage)); // exit selecting subpage mode
        COsdCoreTeleTextExitProc();
        break;

    case _CAPTION_KEY_MESSAGE:
        if ( m_hTtx != NULL)
        {
            TTX_Control( TTX_CMD_SUBTITLE);	/* turn on subtitle mode */
            xExitSubpageHold();
            memset(&ttx_select_subpage, 0, sizeof(ttx_select_subpage)); // exit selecting subpage mode
            ucOsdState = _SUBTITLE_STATE;
//->w			SET_ENABLE_TTX_10ms();
        }
        break;

    case _0_KEY_MESSAGE:
    case _1_KEY_MESSAGE:
    case _2_KEY_MESSAGE:
    case _3_KEY_MESSAGE:
    case _4_KEY_MESSAGE:
    case _5_KEY_MESSAGE:
    case _6_KEY_MESSAGE:
    case _7_KEY_MESSAGE:
    case _8_KEY_MESSAGE:
    case _9_KEY_MESSAGE:
        if ( ttx_select_subpage.pageA & _BIT15)
        {
            xCalSubpageAddr(fUI_Msg-_0_KEY_MESSAGE);
        }
        else
        {
            xExitSubpageHold();
            xCalPageAddr(fUI_Msg-_0_KEY_MESSAGE);
        }
        break;

    case _REDBUTTON_MESSAGE:
        if ( ttx_select_subpage.pageA & _BIT15)
        {
            TTX_Control(TTX_CMD_PREV_SUBPAGE);
        }
        else
        {
            xExitSubpageHold();
            xResetPageAddr();
            TTX_Control( TTX_CMD_BTN_RED);
        }
        break;

    case _GREENBUTTON_MESSAGE:
        if ( ttx_select_subpage.pageA & _BIT15)
        {
            TTX_Control(TTX_CMD_NEXT_SUBPAGE);
        }
        else
        {
            xExitSubpageHold();
            xResetPageAddr();
            TTX_Control( TTX_CMD_BTN_GREEN);
        }
        break;

    case _BLUEBUTTON_MESSAGE:
        if ( !(ttx_select_subpage.pageA & _BIT15))
        {
            xExitSubpageHold();
            xResetPageAddr();
            TTX_Control( TTX_CMD_BTN_CYAN);
        }
        break;

    case _YELLOWBUTTON_MESSAGE:
        if ( !(ttx_select_subpage.pageA & _BIT15))
        {
            xExitSubpageHold();
            xResetPageAddr();
            TTX_Control( TTX_CMD_BTN_YELLOW);
        }
        break;

    case _UP_KEY_MESSAGE:
        xExitSubpageHold();
        memset(&ttx_select_subpage, 0, sizeof(ttx_select_subpage)); // exit selecting subpage mode
        xResetPageAddr();
        TTX_Control( TTX_CMD_NEXT_PAGE);
        break;

    case _DOWN_KEY_MESSAGE:
        xExitSubpageHold();
        memset(&ttx_select_subpage, 0, sizeof(ttx_select_subpage)); // exit selecting subpage mode
        xResetPageAddr();
        TTX_Control( TTX_CMD_PREV_PAGE);
        break;

    case _LEFT_KEY_MESSAGE:
        if (TTX_Control( TTX_CMD_PREV_SUBPAGE))
        {
            xResetPageAddr();
        }
        break;

    case _RIGHT_KEY_MESSAGE:
        if (TTX_Control( TTX_CMD_NEXT_SUBPAGE))
        {
            xResetPageAddr();
        }
        break;
    case _TTX_NEXT_SUBPAGE: // for auto-rotating subpage
        if ( ! ttx_subpage_auto_rotation.hold )   // no hold
        {
            xResetPageAddr();
            TTX_Control(TTX_CMD_ROTATE_SUBPAGE);
        }
        break;
    case _JUMP_KEY_MESSAGE:
        TTX_Control( TTX_CMD_TEST);
        break;

    case _OK_KEY_MESSAGE:
    {
        TTX_STD_Page_AddrWithSubCode_t initPage; // initial page
        TTX_CmdArg_t arg;
        uint16_t curPage;

        xExitSubpageHold();
        memset(&ttx_select_subpage, 0, sizeof(ttx_select_subpage)); // exit selecting subpage mode
        TTX_GetInitPage(&initPage);

        // no 3 digits than press OK
        if ( (pageTens == 0xA) || (pageUnits == 0xA) )
        {
            magNum = initPage.pageA>>8;
            pageTens = (initPage.pageA&0xF0)>>4;
            pageUnits = (initPage.pageA&0xF);
            pageNumIdx = 0;
        }

        arg.pageNum.magNum = magNum;
        arg.pageNum.pageNumTens = pageTens;
        arg.pageNum.pageNumUnits = pageUnits;

        TTX_Control1( TTX_CMD_GO_TO_PAGE, &arg);

        curPage = (magNum<<8|pageTens<<4|pageUnits);

        if (curPage == initPage.pageA)   // press OK again
        {
            TTX_Control1(TTX_CMD_SET_PAGE_NUM_INDICATOR, &arg); // reset page number indicator
        }
        else
        {
            // reset to initial page
            magNum = initPage.pageA>>8;
            pageTens = (initPage.pageA&0xF0)>>4;
            pageUnits = (initPage.pageA&0xF);
        }
    }
    break;

    case _SIZE_KEY_MESSAGE:
        tt_size_mode++;
        tt_size_mode %= 3;
        rtdf_maskl(VBIOSD_CTRL3_REG_0x4808, ~(_BIT25|_BIT24), (UINT32)tt_size_mode<<24);
        break;

    case _MIXER_KEY_MESSAGE:
        if ( rtd_inl(VBIOSD_CTRL1_REG_0x4800) & _BIT1 )
            rtdf_maskl(VBIOSD_CTRL1_REG_0x4800, ~_BIT1, 0);
        else
            rtdf_maskl(VBIOSD_CTRL1_REG_0x4800, ~_BIT1, _BIT1);
        break;

    case _TTX_REVEAL_KEY_MESSAGE:
        RTD_Log(LOGGER_INFO, "Accept key : _TTX_REVEAL_KEY_MESSAGE \n");
        TTX_Control(TTX_CMD_REVEAL);
        break;

    case _TTX_SUBPAGE_KEY_MESSAGE:
        if ( ttx_select_subpage.pageA& _BIT15 )
        {
            ttx_select_subpage.pageA &= (~_BIT15);
        }
        else
        {
            ttx_select_subpage.pageA= (m_hTtx->pageShowing.addr.pageA);
            ttx_select_subpage.pageA |= _BIT15;
            ttx_select_subpage.subcodeA = 0xFFFF;
        }
        break;
    case _TTX_HOLD_KEY_MESSAGE:
        memset(&ttx_select_subpage, 0, sizeof(ttx_select_subpage)); // exit selecting subpage mode
        ttx_subpage_auto_rotation.hold = !ttx_subpage_auto_rotation.hold; // switch hold/rotate
        xResetPageAddr();
        if ( ttx_subpage_auto_rotation.hold )
            TTX_Control(TTX_CMD_HOLD); // display hold icon
        else
        {
            xExitSubpageHold();
            xRestoreShowingPageAddr(); // display page number
        }
        break;
    case _TTX_INDEXL_KEY_MESSAGE:
        memset(&ttx_select_subpage, 0, sizeof(ttx_select_subpage)); // exit selecting subpage mode
        xExitSubpageHold();
        xResetPageAddr();
        TTX_Control( TTX_CMD_INDEX_PAGE);
        RTD_Log(LOGGER_INFO, "Accept key : _TTX_INDEXL_KEY_MESSAGE \n");

        break;
    case _TTX_SIGNAL_OK_MESSAGE:
        fw_ui_message_clearallType(_TTX_SIGNAL_OK_MESSAGE); // prevent too many messages
        fw_ui_message_clearallType(_TTX_SIGNAL_NG_MESSAGE); // prevent NG message has already been pushed
        if ( ttx_noTT == 2 )
        {
            TTX_DISP_DrawNoTTMsg(FALSE);
        }
        ttx_noTT = 0; // TT OK
        fw_timer_event_ReactiveTimerEvent(SEC(5), timerevent_ui_table_OSD_NoTT_event);
        break;

    case _TTX_SIGNAL_NG_MESSAGE:
        if ( (ttx_noTT==1) || (ttx_noTT==3) )
        {
            ttx_noTT = 3;
            fw_ui_message_push(_TTX_EXIT_MESSAGE);
        }
        else
        {
            ttx_noTT = 2;
            TTX_DISP_DrawNoTTMsg(TRUE);
        }
        break;

    default:
        break;
    }
}

/**
 * Subtitle message Event Handler
 * @return none.
 */
void COsdCoreSubTitleEventProc(void)
{
    switch (fUI_Msg)
    {
    case _TTX_EXIT_MESSAGE:
        if ( ttx_noTT == 3 )
        {
            fw_ui_message_push(_NO_TT_SIGNAL);
        }
    case _CAPTION_KEY_MESSAGE:
        COsdCoreTeleTextExitProc();
        break;

    case _TELETEXT_KEY_MESSAGE:
        TTX_Control( TTX_CMD_NORMAL);
        ucOsdState = _TELETEXT_STATE;
//->w		SET_ENABLE_TTX_10ms();
        break;

    case _SIZE_KEY_MESSAGE:
        tt_size_mode++;
        tt_size_mode %= 3;
        rtdf_maskl(VBIOSD_CTRL3_REG_0x4808, ~(_BIT25|_BIT24), (UINT32)tt_size_mode<<24);
        break;

    case _MIXER_KEY_MESSAGE:
        if ( rtd_inl(VBIOSD_CTRL1_REG_0x4800) & _BIT1 )
            rtdf_maskl(VBIOSD_CTRL1_REG_0x4800, ~_BIT1, 0);
        else
            rtdf_maskl(VBIOSD_CTRL1_REG_0x4800, ~_BIT1, _BIT1);
        break;

    case _TTX_SIGNAL_OK_MESSAGE:
        fw_ui_message_clearallType(_TTX_SIGNAL_OK_MESSAGE); // prevent too many messages
        fw_ui_message_clearallType(_TTX_SIGNAL_NG_MESSAGE); // prevent NG message has already been pushed
        if ( ttx_noTT == 2 )
        {
            TTX_DISP_DrawNoTTMsg(FALSE);
        }
        ttx_noTT = 0; // TT OK
        fw_timer_event_ReactiveTimerEvent(SEC(5), timerevent_ui_table_OSD_NoTT_event);
        break;

    case _TTX_SIGNAL_NG_MESSAGE:
        if ( (ttx_noTT==1) || (ttx_noTT==3) )
        {
            ttx_noTT = 3;
            fw_ui_message_push(_TTX_EXIT_MESSAGE);
        }
        else
        {
            ttx_noTT = 2;
            TTX_DISP_DrawNoTTMsg(TRUE);
        }
        break;

    default:
        break;
    }
}
#endif // CONFIG_TTX_SUPPORT




#define _NEW_CODE_END_
















/**
 * OSD boot state handler
 * @return none.
 */
static void ap_osdcore_boot_state_proc(void)
{
}




/* =========================================================================== */
/**
 * OSD event.
 *
 */

void COsdRecallVGAValue(void)
{
//->w	RTD_Log(LOGGER_DEBUG,"\nrecall.Clock= %x\n",(UINT32)stVGAModeCurrInfo.Clock);
//->w	RTD_Log(LOGGER_DEBUG,"\nrecall.Phase= %x\n",(UINT32)stVGAModeCurrInfo.Phase);
//->w	RTD_Log(LOGGER_DEBUG,"\nrecall.H_Position= %x\n",(UINT32)stVGAModeCurrInfo.H_Position);
//->w	RTD_Log(LOGGER_DEBUG,"\nrecall.V_Position= %x\n",(UINT32)stVGAModeCurrInfo.V_Position);


    SET_CLOCK(stVGAModeCurrInfo.Clock);
    SET_PHASE(stVGAModeCurrInfo.Phase);
    SET_H_POSITION(stVGAModeCurrInfo.H_Position);
    SET_V_POSITION(stVGAModeCurrInfo.V_Position);
}

#ifdef CONFIG_VBI_CC_SUPPORT
static void COsdCoreVbiProc(void)
{
    if ((fUI_Msg != _FL2UI_NONE_MESSAGE))
    {
        if ((GET_INPUTSOURCE_TYPE() == _SOURCE_CVBS)||(GET_INPUTSOURCE_TYPE() == _SOURCE_SV)
                ||(GET_INPUTSOURCE_TYPE() == _SOURCE_YPBPR))
        {
            switch (fUI_Msg)
            {

            case 	_0_KEY_MESSAGE:
            case 	_1_KEY_MESSAGE:
            case 	_2_KEY_MESSAGE:
            case 	_3_KEY_MESSAGE:
            case 	_4_KEY_MESSAGE:
            case 	_5_KEY_MESSAGE:
            case 	_6_KEY_MESSAGE:
            case 	_7_KEY_MESSAGE:
            case 	_8_KEY_MESSAGE:
            case 	_9_KEY_MESSAGE:
            case 	_100_KEY_MESSAGE:
            case 	_JUMP_KEY_MESSAGE:
            case	_REPEAT_KEY_MESSAGE:
            case       _UP_KEY_MESSAGE:
            case 	_DOWN_KEY_MESSAGE:
                bCcStartFlag = _TRUE;
                return;
            default:
                break;

            }

        }
        else if (GET_INPUTSOURCE_TYPE() == _SOURCE_TV)
        {
            if (fUI_Msg ==_REPEAT_KEY_MESSAGE)
            {
                bCcStartFlag = _TRUE;
                return;
            }
        }

        fw_timer_event_CancelTimerEvent(COsdEventCloseVbi);
        COsdVbiShowData(0);
        COsdVbiBlendingOperation(0);
        CLR_OSDVBI_INITIAL();
        
        RTD_Log(LOGGER_LEVEL,"\n vbiTim bccStartFlag=%d \n",(UINT32)bCcStartFlag);
        bCcStartFlag = _FALSE;
        ap_osdfx_map_setting(_OSD_MAP0);
        // Load OSD palette
        ap_osdfx_reload_osdpalette();
        // Load Font
        ap_osdfx_load_vlc_font(tFONT_GLOBAL, 0x00, 0x68);
        // Load 2bits Main Icon Font
        COsdCoreDispatchMsg();

    }
}
#endif

#if(IR_DETECTMODE == _IR_DS_SW)

/*********************************************************************
** 函数名称: bit Repeatkey_en(UINT8 key_message)
** 功能描述: 状态是否是允许连续按键
** 输　  入: UINT8 key_message
** 输　  出: 是否 
**********************************************************************/
#ifdef SUPPORT_FIVE_KEY

bit Repeatkey_en(UINT8 key_message)
{
	if(ucOsdState == _MAINMENU_STATE)
	{
		if(ucOsditem_choose == _TRUE)
		{
			if(key_message == _LEFT_KEY_MESSAGE || key_message == _RIGHT_KEY_MESSAGE)
			return _TRUE;
		}
		return _FALSE;	
	}
	else if(ucOsdState == _VOLUME_STATE)
	{
		if(key_message == _LEFT_KEY_MESSAGE || key_message == _RIGHT_KEY_MESSAGE)
			return _TRUE;
		return _FALSE;
	}

	if(_VOLUME_INC_MESSAGE == key_message || _VOLUME_DEC_MESSAGE == key_message)
	{
		return _TRUE;
	}

	return _FALSE;
}

#else
/*********************************************************************
** 函数名称: bit Repeatkey_en(UINT8 key_message)
** 功能描述: 状态是否是允许连续按键
** 输　  入: UINT8 key_message
** 输　  出: 是否 
**********************************************************************/

bit Repeatkey_en(UINT8 key_message)
{
	if(ucOsdState == _MAINMENU_STATE)
	{
		if(key_message == _LEFT_KEY_MESSAGE || key_message == _RIGHT_KEY_MESSAGE)
			return _TRUE;
		return _FALSE;	
	}
	else if(ucOsdState == _VOLUME_STATE)
	{
		if(key_message == _LEFT_KEY_MESSAGE || key_message == _RIGHT_KEY_MESSAGE)
			return _TRUE;
		return _FALSE;
	}

	if(_VOLUME_INC_MESSAGE == key_message || _VOLUME_DEC_MESSAGE == key_message)
	{
		return _TRUE;
	}

	return _FALSE;
}
#endif//#ifdef SUPPORT_FIVE_KEY

/*********************************************************************
** 函数名称: bit Repeatkey_en(UINT8 key_message)
** 功能描述: 状态是否是允许连续按键
** 输　  入: UINT8 key_message
** 输　  出: 是否 
**********************************************************************/
#else
bit Repeatkey_en(UINT8 key_message)
{
	MenuItem* pMenuItem;
    UINT8 ucTmpMain;
    UINT8 ucTmpSub;
    UINT8 ucType;

    ucTmpMain = stLayerStack[ucLayerIndex].ucMainMenu;
    ucTmpSub = stLayerStack[ucLayerIndex].ucSubMenu;
    pMenuItem = stLayerStack[ucLayerIndex].pMenuItem;
    
	if(ucOsdState == _MAINMENU_STATE)
	{
		if(key_message == _LEFT_KEY_MESSAGE || 
			key_message == _RIGHT_KEY_MESSAGE || 
			key_message == _LEFT_IR_MESSAGE || 
			key_message == _RIGHT_IR_MESSAGE)
		{
			ucTmpSub = MAINMENU_GET_ITEM_ID(ucTmpSub);
			//RTD_Log(LOGGER_INFO, "melon=%x\n",(UINT32)ucTmpSub);  
			//RTD_Log(LOGGER_INFO, "melon123=%x\n",(UINT32)ucTmpMain);  
            ucType = pMenuItem->pItemList[ucTmpSub].ucType;
            if(ucType == _SLIDER_TYPE||((ucTmpSub==4)&&(ucTmpMain==1)&&(ucType == _BRACKET_TYPE)))
				return _TRUE;
		}
		return _FALSE;	
	}
	else if(ucOsdState == _VOLUME_STATE)
	{
		if(key_message == _LEFT_KEY_MESSAGE || 
			key_message == _RIGHT_KEY_MESSAGE || 
			key_message == _LEFT_IR_MESSAGE || 
			key_message == _RIGHT_IR_MESSAGE ||
			_VOLUME_INC_MESSAGE == key_message || 
			_VOLUME_DEC_MESSAGE == key_message
			)
			return _TRUE;
		return _FALSE;
	}
	/*if(_VOLUME_INC_MESSAGE == key_message || _VOLUME_DEC_MESSAGE == key_message)
	{
		return _TRUE;
	}*/

	return _FALSE;
}

#endif//#if(IR_DETECTMODE == _IR_DS_SW)


#ifdef SUPPORT_FIVE_KEY
/*********************************************************************
** 函数名称: bit COsd_LRkey2UDkey(void)
** 功能描述: 判断是否在5key按键下面把左右键转换成上下键
** 输　  入: 无
** 输　  出: 是否 
**********************************************************************/

bit COsd_LRkey2UDkey(void)
{
	MenuItem* pMenuItem;
    UINT8 ucTmpMain;
    UINT8 ucTmpSub;
    
    ucTmpMain = stLayerStack[ucLayerIndex].ucMainMenu;
    ucTmpSub = stLayerStack[ucLayerIndex].ucSubMenu;
    pMenuItem = stLayerStack[ucLayerIndex].pMenuItem;

	ucTmpSub = MAINMENU_GET_ITEM_ID(ucTmpSub);
    if((pMenuItem->pItemList[ucTmpSub].ucType == _SLIDER_TYPE
    	||pMenuItem->pItemList[ucTmpSub].ucType == _BRACKET_TYPE) && (ucOsditem_choose == _TRUE))
    {
    	return _FALSE;
    }
    return _TRUE;
}

/*********************************************************************
** 函数名称: void COsdCore_Chn_keypush_EventProc(void)
** 功能描述: 在5key情况下面客户在电视频道在_IDLE_STATE的时候按左键进入音量调节	
**			 按右键进入频道切换
** 输　  入: 无
** 输　  出: 是否 
**********************************************************************/

void COsdCore_Chn_keypush_EventProc(void)
{
	UINT8 newChn;
	
    switch (fUI_Msg)
    {

#ifdef _VIDEO_TV_SUPPORT
    case _LEFT_KEY_MESSAGE:         
        newChn = CTvSelectChannel(_TV_PREV_CHANNEL);
        if ( COsdCtrlGetSignalSource() == _SOURCE_INDEX_TV)
        {
            CTvSetChannel(newChn);
            //COsdCoreSourceInfo(_TRUE);
            COsdCoreShowTVChannel(newChn);
            ucOsdState = _CHN_KEYPUSH_STATE;
        }
    break;

    case _CHANNEL_INC_MESSAGE:
        newChn = CTvSelectChannel(_TV_NEXT_CHANNEL);
        if ( COsdCtrlGetSignalSource() == _SOURCE_INDEX_TV)
        {
            CTvSetChannel(newChn);
            //COsdCoreSourceInfo(_TRUE);
            COsdCoreShowTVChannel(newChn);
            ucOsdState = _CHN_KEYPUSH_STATE;
        }
        break;
        
    case _RIGHT_KEY_MESSAGE:       
        newChn = CTvSelectChannel(_TV_NEXT_CHANNEL);
        if ( COsdCtrlGetSignalSource() == _SOURCE_INDEX_TV)
        {
            CTvSetChannel(newChn);
            //COsdCoreSourceInfo(_TRUE);
            COsdCoreShowTVChannel(newChn);
            ucOsdState = _CHN_KEYPUSH_STATE;
        }
        break;

    case _CHANNEL_DEC_MESSAGE:
        newChn = CTvSelectChannel(_TV_PREV_CHANNEL);
        if ( COsdCtrlGetSignalSource() == _SOURCE_INDEX_TV)
        {
            CTvSetChannel(newChn);
            //COsdCoreSourceInfo(_TRUE);
            COsdCoreShowTVChannel(newChn);
            ucOsdState = _CHN_KEYPUSH_STATE;
        }
        break;

#endif
    default:
    	if (!GET_FACTORY_MODE_MENU())
            COsdCoreDispatchMsg();
        break;
    }
    fw_timer_event_ReactiveTimerEvent(SEC(5), timerevent_ui_table_OSD_timeout_event);
}



#endif//#ifdef SUPPORT_FIVE_KEY

#if(_SOURCE_LIST_TYPE == _SOURCE_LISTNOTMENU) 
UINT8 COsdGetNextSource(ucsource)
{
	UINT8 nextsource;
	ItemList* pListItem_temp;
	UINT8 SourceTotal = 0;
	UINT8 cnt;
	
	pListItem_temp = &tSourceSelectList;
	SourceTotal = pListItem_temp->ucItemCount;
	for(cnt=0;cnt<SourceTotal;cnt++)
	{
		if(pListItem_temp->pItemList[cnt].ucValue== ucsource)
			break;
	}

	if(cnt==SourceTotal)
		nextsource = pListItem_temp->pItemList[0].ucValue;
	else if(cnt== SourceTotal-1)
		nextsource = pListItem_temp->pItemList[0].ucValue;
	else
		nextsource = pListItem_temp->pItemList[cnt+1].ucValue;

	return 	nextsource;
}
#endif


