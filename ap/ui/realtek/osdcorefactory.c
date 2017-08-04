/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2009
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is for osdcore factory mode related functions.
 *
 * @author 	$Author: cf.tsai $
 * @date	$Date: 2009/03/17 $
 * @version	$Revision: 10 $
 * @ingroup	osdcorefactory
 */

/**
 * @addtogroup osdcorefactory
 * @{
 */

/*============================ Module dependency  ===========================*/
#include "autoconf.h"
#include "sparrow.h"
#include "rosprintf.h"
#include "sys_info.h"
#include "osd_sys_msg.h"
#include "ui_struct.h"
#include "timer_event.h"
#include "timer_event_ui_table.h"
#include "flow_scaler.h"
#include "ui_ctrl.h"
#include "ap_rtirkey.h"

#include "message.h"
#include "osdobjectdef.h"
#include "osdfunction.h"
#include "osdparameter.h"
#include "osdfont.h"
#include "osdstring.h"
#include "osdcore.h"
#include "osdcorefactory.h"
#include "osdflow.h"
#include "osdcontrol.h"
#include "timer_event_ui_table.h"


#if 0
#define IS_MENU_ON(_x)				((_x)&_BIT7)
#define SET_MENU_ON(_x)				(_x) |= _BIT7
#define GET_MENUITEM_ID(_x)			((_x)&0x0f)
#endif

/*=========================  Types ================================*/



/*========================= Variables ==============================*/
extern volatile UINT16 xdata fUI_Msg;
UINT8 code stFactoryModePwd[] = {1, 2, 3, 4};
UINT8 code stRtkFactoryModePwd[] = {2, 6, 6, 8};
UINT8 xdata ucFactoryModePwdState = 0;
UINT8  code ucTimTest = 23;
extern volatile UINT8 xdata ucLayerIndex;
extern volatile LayerStack xdata stLayerStack[];
extern UINT8 xdata ucPage;
extern ItemPosition xdata stItemPos;
extern ItemObject* xdata pItemObj;
extern ItemList* xdata pListItem;
extern UINT8 xdata ucOsdCoreBuf[8];




/*========================= Local Functions Phototype ==============*/



void COsdCoreRtkFacDrawItems(UINT8 ucSelectedItem);
//void COsdCoreRtkFacUpdateSliderItem(UINT8 ucItemId);
//void COsdCoreRtkFacUpdateBracketItem(UINT8 ucItemId);
//void COsdCoreRtkFacUpdateListItem(UINT8 ucItemId);
//void COsdCoreRtkFacDispHint(UINT8 ucItemType);
//void COsdCoreRtkFacMoveHighlight(UINT8 ucSelectedItem);


/*UINT8 Ctest(void)
{
	RTD_Log(LOGGER_LEVEL,"\n  Tim test \n ");
	return 1;
}*/
/*========================= Functions ===================== */

/**
 *
 * @return none.
 */
void ap_factory_mode_check_pwd(void)
{
#if 1
    UINT8 ucVal;
	//RTD_Log(LOGGER_LEVEL,"\n Tim pwd start \n");
    if ( GET_FACTORY_MODE_MENU() == 0 && GET_RTK_FACTORY_MODE() == 0)
    {
        ucVal = ap_osdcore_irkey_to_num(fUI_Msg);
        
        //RTD_Log(LOGGER_LEVEL,"\n Tim pwd start \n");
        if (ucVal == stFactoryModePwd[ucFactoryModePwdState&0x0f])
            ucFactoryModePwdState++;
        else if (ucVal == stRtkFactoryModePwd[(ucFactoryModePwdState>>4)])
            ucFactoryModePwdState = ucFactoryModePwdState + 0x10;
        else
            ucFactoryModePwdState = 0;

        //RTD_Log(LOGGER_LEVEL,"\n Tim facstate=%d,ucval=%d \n",(UINT32)ucFactoryModePwdState,(UINT32)ucVal);
        
        if ( (ucFactoryModePwdState&0x0f) == 4)
        {
            ucFactoryModePwdState = 0;
            COsdCoreFacInitMenu();
        }
        else if ( (ucFactoryModePwdState >> 4) == 4)
        {
            ucFactoryModePwdState = 0;
            COsdCoreRtkFacInitMenu();
        }
    }
    //return _FALSE;
#else
CFacInit();
#endif

}

#if 1
/**
 * To initial factory mode OSD menu.
 * @return none.
 */
void COsdCoreFacInitMenu(void)
{
    //UINT8 ucCnt;

    /*fw_timer_event_CancelTimerEvent(timerevent_ui_table_OSD_timeout_event);

    SET_MAINMENU_ON();
    SET_FACTORY_MODE_MENU();
    ucOsdState = _FACTORY_MENU_STATE;*/

	CFacInit();
	#if 0
    ucLayerIndex = 0;
    stLayerStack[0].ucMainMenu = 0;
    stLayerStack[0].ucSubMenu = 0;
    stLayerStack[0].pMenuItemSet = &tFac_Setting_MenuItemSet;
    stLayerStack[0].pMenuItem = tFac_Setting_MenuItemSet.ppMenuItems[0];
    stLayerStack[1].ucMainMenu = 0;
    stLayerStack[1].ucSubMenu = 0;
    stLayerStack[1].pMenuItemSet = &tFac_Design_MenuItemSet;
    stLayerStack[1].pMenuItem = tFac_Design_MenuItemSet.ppMenuItems[1];


    COsdFxBlending(0);

    ap_osdfx_map_setting(_OSD_MAP0);
    for (ucCnt=0; ucCnt<12; ucCnt++)
        COsdFxSetRowCtrl1(2+ucCnt, ROW_HEIGHT(18));
    COsdFxSetWinRefPos(40, 80);

    //ap_osdfx_load_vlc_font(tFont_SourcePatch, _FRAME_2BIT_PATCH_ADDR, 12);

    COsdFxDrawWindow(&tFac_Title0_WindowStyle);
    COsdFxDrawWindow(&tFac_Title1_Window_Style);
    COsdFxDrawWindow(&tFac_Mainmenu_WindowStyle);

    // draw title
    COsdFxLoadBlankCmd(0, 0, 30, _CP_BLACK);
    ucCnt = 1;
    ucCnt += COsdFxLoadText(0, 1, 0, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tFac_Setting_MenuItemSet.pString);
    ucCnt += COsdFxLoadBlankCmd(0, ucCnt, 132, _CP_BLACK);
    ucCnt += COsdFxLoadText(0, ucCnt, 0, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tFac_Design_MenuItemSet.pString);
    if (ucCnt < _OSD_TITLE_ROW_LENGTH)
        COsdFxClearText(0, ucCnt, 1, _OSD_TITLE_ROW_LENGTH-ucCnt);
    COsdCoreFacSetMenuTitleHighlight(ucLayerIndex, _CP_PURE_YELLOW_ORANGE<<4 | _CP_BLACK);

    // display main menu items
    COsdCoreFacDispMainMenuItems();

    // clear sub menu item area
    COsdCoreFacDispSubmenuItems();

    COsdFxEnableOsd();
    #endif
}

#endif
/**
 * To set highlight of factory mode menu title.
 * @return none.
 */
void COsdCoreFacSetMenuTitleHighlight(UINT8 ucID, UINT8 ucColor)
{
    UINT8 ucCnt;

    if (ucID == 0)
        COsdFxHLine(0, 1, COsdFxCalTextCount(0, stLayerStack[0].pMenuItemSet->pString), ucColor, _WRITE_BYTE2);
    else
    {
        ucCnt = 2 + COsdFxCalTextCount(0, stLayerStack[0].pMenuItemSet->pString);
        COsdFxHLine(0, ucCnt, COsdFxCalTextCount(0, stLayerStack[1].pMenuItemSet->pString), ucColor, _WRITE_BYTE2);
    }
}

/**
 * To display factory mode main menu items.
 * @return none.
 */
void COsdCoreFacDispMainMenuItems(void)
{
    UINT8 j;
    UINT8 ucCnt;
    UINT8 ucWidth;
    UINT8* pStr;

    WaitFor_DEN_STOP();
    /* length is 280 pixels */
    for (j=0; j<12; j++)
    {
        if (j < stLayerStack[ucLayerIndex].pMenuItemSet->ucItemCount)
        {
            pStr = stLayerStack[ucLayerIndex].pMenuItemSet->ppMenuItems[j]->pTitle;
            ucCnt = COsdFxLoadText(2+j, 0, 30, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, pStr);
            ucWidth = ucCnt*12 - COsdFxCalTextWidth(0, pStr);
            if ( ucWidth < 5)
            {
                ucCnt += COsdFxLoadBlankCmd(2+j, ucCnt, 8, _CP_BLACK);
                ucCnt += COsdFxLoadBlankCmd(2+j, ucCnt, ucWidth+4, _CP_BLACK);
            }
            else
                ucCnt += COsdFxLoadBlankCmd(2+j, ucCnt, ucWidth, _CP_BLACK);
            if (ucCnt < _FAC_MAINITEM_COL)
                COsdFxClearText(2+j, ucCnt, 1, _FAC_MAINITEM_COL-ucCnt);

        }
        else
        {
            COsdFxLoadBlankCmd(2+j, 0, 30, _CP_BLACK);
            COsdFxClearText(2+j, 1, 1, _FAC_MAINITEM_COL-1);
        }
    }
}

/**
 * To set highlight item of main menu.
 * @return none.
 */
void COsdCoreFacSetMainmenuHighlight(UINT8 ucID, UINT8 ucColor)
{
    UINT8 ucCnt;

    ucCnt = COsdFxCalTextCount(0, stLayerStack[ucLayerIndex].pMenuItemSet->ppMenuItems[ucID]->pTitle);
    COsdFxHLine(2+ucID, 1, ucCnt, ucColor, _WRITE_BYTE2);
}

/**
 * To display sub menu items.
 * @return none.
 */
void COsdCoreFacDispSubmenuItems(void)
{
    UINT8 j;
    UINT8 ucCnt;
    UINT8 ucWidth;
    UINT8 ucColor;
    ItemDescription* pDescription;

    WaitFor_DEN_STOP();
    /* sub menu item start from col 20 */
    for (j=0; j<12; j++)
    {
        if (IS_MENU_ON(stLayerStack[ucLayerIndex].ucMainMenu) && j < stLayerStack[ucLayerIndex].pMenuItem->ucItemCount)
        {
            pItemObj = &(stLayerStack[ucLayerIndex].pMenuItem->pItemList[j]);
            if ( pItemObj->ucType == _DESCRIPTION_TYPE)
            {
                pDescription = (ItemDescription*)(pItemObj->pStruct);
                if (pDescription->ucControl & _DESCRIPTION_DYNAMIC_STR)
                {
                    ucWidth = (_FAC_SUBITEM_COL)*12 - COsdFxCalTextWidth(0, COsdCtrlMenuItemGetString(pDescription->ucMenuItemID));
                    ucWidth = ucWidth / 2;
                    ucCnt = _FAC_MAINITEM_COL + COsdFxLoadText(2+j, _FAC_MAINITEM_COL, ucWidth, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, COsdCtrlMenuItemGetString(pDescription->ucMenuItemID));
                    if ( ucCnt < _OSD_ITEM_ROW_LENGTH)
                        COsdFxClearText(2+j, ucCnt, 1, _OSD_ITEM_ROW_LENGTH-ucCnt);
                }
                else/* if (pDescription->ucControl & _DESCRIPTION_COLOR_CTRL)*/
                {
                    ucWidth = (_FAC_SUBITEM_COL) * 12 - COsdFxCalTextWidth(0, pItemObj->pString);
                    ucWidth = ucWidth / 2;
                    if (pDescription->ucControl & _DESCRIPTION_COLOR_CTRL)
                        ucColor = pDescription->ucMenuItemID;
                    else
                        ucColor = _CP_WHITE;
                    ucCnt = _FAC_MAINITEM_COL + COsdFxLoadText(2+j, _FAC_MAINITEM_COL, ucWidth, FONT_COLOR(ucColor, _CP_BLACK), 0, pItemObj->pString);
                    if ( ucCnt < _OSD_ITEM_ROW_LENGTH)
                        COsdFxClearText(2+j, ucCnt, 1, _OSD_ITEM_ROW_LENGTH-ucCnt);
                }
                continue;
            }
            ucCnt = _FAC_MAINITEM_COL + COsdFxLoadText(2+j, _FAC_MAINITEM_COL, 0, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, pItemObj->pString);
            ucWidth = (ucCnt-_FAC_MAINITEM_COL)*12 - COsdFxCalTextWidth(0, pItemObj->pString);
            if (ucWidth < 5)
            {
                ucCnt += COsdFxLoadBlankCmd(2+j, ucCnt, 8, _CP_BLACK);
                ucCnt += COsdFxLoadBlankCmd(2+j, ucCnt, ucWidth+4, _CP_BLACK);
            }
            else
                ucCnt += COsdFxLoadBlankCmd(2+j, ucCnt, ucWidth, _CP_BLACK);
            if (ucCnt < _FAC_MAINITEM_COL+_FAC_SUBITEM_COL)
                COsdFxClearText(2+j, ucCnt, 1, _FAC_MAINITEM_COL+_FAC_SUBITEM_COL-ucCnt);
            if ( pItemObj->ucType == _SLIDER_TYPE )
                COsdCoreFacUpdateSliderItem(j);
            else if ( pItemObj->ucType == _BRACKET_TYPE	|| pItemObj->ucType == _BRACKET1_TYPE)
                COsdCoreFacUpdateBracketItem(j);
            else if ( pItemObj->ucType == _LIST_TYPE)
                COsdCoreFacUpdateListItem(j);
            else if ( pItemObj->ucType == _IMMEDIATE_TYPE)
            {
                COsdFxClearText(2+j, _FAC_MAINITEM_COL+_FAC_SUBITEM_COL, 1, _FAC_BRACKET_COL-1);
                COsdFxWrite1BitFont(2+j, _FAC_MAINITEM_COL+_FAC_SUBITEM_COL+_FAC_BRACKET_COL-1, _R_ARROW_, _CP_WHITE<<4|_CP_BLACK);
            }
            else
            {
                COsdFxClearText(2+j, _FAC_MAINITEM_COL+_FAC_SUBITEM_COL, 1, _FAC_BRACKET_COL);
            }
        }
        else
        {
            COsdFxClearText(2+j, _FAC_SUBITEM_COL, 1, _OSD_ITEM_ROW_LENGTH-_FAC_SUBITEM_COL);
        }
    }
}

/**
 * To update slider value of factory mode menu.
 * @return none.
 */
void COsdCoreFacUpdateSliderItem(UINT8 ucID)
{
    UINT8 ucColor;

    if (IS_MENU_ON(stLayerStack[ucLayerIndex].ucSubMenu) && ucID == GET_MENUITEM_ID(stLayerStack[ucLayerIndex].ucSubMenu))
        ucColor = _CP_PURE_YELLOW_ORANGE << 4 | _CP_BLACK;
    else
        ucColor = _CP_WHITE << 4 | _CP_BLACK;
    COsdFxClearText(2+ucID, _FAC_MAINITEM_COL+_FAC_SUBITEM_COL, 1, (_FAC_BRACKET_COL-4));
    COsdCoreDrawSliderNumber(2+ucID, _FAC_MAINITEM_COL+_FAC_SUBITEM_COL+(_FAC_BRACKET_COL-4), ucColor, (ItemSlider *)(stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucID].pStruct));
    COsdFxClearText(2+ucID, _FAC_MAINITEM_COL+_FAC_SUBITEM_COL+_FAC_BRACKET_COL, 1, _OSD_ITEM_ROW_LENGTH-(_FAC_MAINITEM_COL+_FAC_SUBITEM_COL+_FAC_BRACKET_COL));
}

/**
 * To update bracket item of factory mode menu.
 * @return none.
 */
void COsdCoreFacUpdateBracketItem(UINT8 ucID)
{
    UINT8 ucWidth;
    UINT8 ucCnt;
    UINT8 ucColor;
    UINT8* pStr;

    pItemObj = &(stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucID]);
    pStr = COsdCoreGetBracketString();
    if (IS_MENU_ON(stLayerStack[ucLayerIndex].ucSubMenu) && ucID == GET_MENUITEM_ID(stLayerStack[ucLayerIndex].ucSubMenu))
        ucColor = _CP_PURE_YELLOW_ORANGE << 4 | _CP_BLACK;
    else
        ucColor = _CP_WHITE << 4 | _CP_BLACK;
    ucWidth = _FAC_BRACKET_COL*12 - COsdFxCalTextWidth(0, pStr);
    ucCnt = _FAC_MAINITEM_COL+_FAC_SUBITEM_COL;
    ucCnt += COsdFxLoadText(2+ucID, ucCnt, ucWidth, ucColor, 0, pStr);
    if (ucCnt < _OSD_ITEM_ROW_LENGTH)
        COsdFxClearText(2+ucID, ucCnt, 1, _OSD_ITEM_ROW_LENGTH-ucCnt);
}

/**
 * To update list item of factory mode menu.
 * @return none.
 */
void COsdCoreFacUpdateListItem(UINT8 ucID)
{
    UINT8 ucCnt;
    UINT8 ucWidth;
    UINT8 ucColor;
    UINT8* pStr;

    pItemObj = &(stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucID]);
    pListItem = (ItemList *)(pItemObj->pStruct);
    ucCnt = COsdCoreListItemValueToIdx(COsdCtrlMenuItemGetValue(pListItem->ucMenuItemID));
    pStr = pListItem->pItemList[ucCnt].pString;
    if (IS_MENU_ON(stLayerStack[ucLayerIndex].ucSubMenu) && ucID == GET_MENUITEM_ID(stLayerStack[ucLayerIndex].ucSubMenu))
        ucColor = _CP_PURE_YELLOW_ORANGE << 4 | _CP_BLACK;
    else
        ucColor = _CP_WHITE << 4 | _CP_BLACK;
    ucWidth = _FAC_BRACKET_COL*12 - COsdFxCalTextWidth(0, pStr);
    ucCnt = _FAC_MAINITEM_COL+_FAC_SUBITEM_COL;
    ucCnt += COsdFxLoadText(2+ucID, ucCnt, ucWidth, ucColor, 0, pStr);
    if (ucCnt < _OSD_ITEM_ROW_LENGTH)
        COsdFxClearText(2+ucID, ucCnt, 1, _OSD_ITEM_ROW_LENGTH-ucCnt);
}

/**
 * To set highlight item of sub menu.
 * @return none.
 */
void COsdCoreFacSetSubmenuHighlight(UINT8 ucID, UINT8 ucColor)
{
    COsdFxHLine(2+ucID, _FAC_MAINITEM_COL, _OSD_ITEM_ROW_LENGTH-_FAC_MAINITEM_COL, ucColor, _WRITE_BYTE2);
}

/**
 * Customer factory mode message event handler.
 * @return none.
 */
void COsdCoreFacMenuEventProc(void)
{
    UINT8 ucTmp;
#if 0
    switch (fUI_Msg)
    {
    case _LEFT_KEY_MESSAGE:
    case _RIGHT_KEY_MESSAGE:
    case _LEFT_IR_MESSAGE:
    case _RIGHT_IR_MESSAGE:
        if ( IS_MENU_ON(stLayerStack[ucLayerIndex].ucSubMenu))
        {
            COsdCoreFacItemEventProc();
        }
        else if ( !IS_MENU_ON(stLayerStack[ucLayerIndex].ucMainMenu) )
        {
            COsdCoreFacSetMenuTitleHighlight(ucLayerIndex, (_CP_WHITE << 4 | _CP_BLACK));
            ucLayerIndex = (ucLayerIndex+1) % 2;
            COsdCoreFacSetMenuTitleHighlight(ucLayerIndex, (_CP_PURE_YELLOW_ORANGE << 4 | _CP_BLACK));
            COsdCoreFacDispMainMenuItems();
        }
        else if (( fUI_Msg == _RIGHT_KEY_MESSAGE||fUI_Msg == _RIGHT_IR_MESSAGE) && !IS_MENU_ON(stLayerStack[ucLayerIndex].ucSubMenu) )
        {
            for (ucTmp=0; ucTmp<stLayerStack[ucLayerIndex].pMenuItem->ucItemCount; ucTmp++)
            {
                if ( stLayerStack[ucLayerIndex].pMenuItem->pItemList[ucTmp].ucType != _DESCRIPTION_TYPE)
                    break;
            }
            if ( ucTmp < stLayerStack[ucLayerIndex].pMenuItem->ucItemCount)
            {
                SET_MENU_ON(stLayerStack[ucLayerIndex].ucSubMenu);
                COsdCoreFacSetSubmenuHighlight(0, _CP_PURE_YELLOW_ORANGE<<4 | _CP_BLACK);
            }
        }

        break;

    case _UP_KEY_MESSAGE:
        if ( IS_MENU_ON(stLayerStack[ucLayerIndex].ucSubMenu) )
        {
            ucTmp = GET_MENUITEM_ID(stLayerStack[ucLayerIndex].ucSubMenu);
            COsdCoreFacSetSubmenuHighlight(ucTmp, 5<<4 | _CP_BLACK);
            if (ucTmp == 0)
                ucTmp = stLayerStack[ucLayerIndex].pMenuItem->ucItemCount - 1;
            else
                ucTmp--;
            COsdCoreFacSetSubmenuHighlight(ucTmp, 8<<4 | _CP_BLACK);
            stLayerStack[ucLayerIndex].ucSubMenu = _BIT7 | ucTmp;
        }
        else if ( !IS_MENU_ON(stLayerStack[ucLayerIndex].ucMainMenu) )
        {
            ucTmp = stLayerStack[ucLayerIndex].pMenuItemSet->ucItemCount - 1;
            COsdCoreFacSetMainmenuHighlight(ucTmp, (_CP_PURE_YELLOW_ORANGE<<4) | _CP_BLACK);
            stLayerStack[ucLayerIndex].ucMainMenu = _BIT7 | ucTmp;
            stLayerStack[ucLayerIndex].pMenuItem = stLayerStack[ucLayerIndex].pMenuItemSet->ppMenuItems[ucTmp];
            COsdCoreFacDispSubmenuItems();
            COsdFxDrawWindow(&tFac_Submenu_WindowStyle);
        }
        else
        {
            ucTmp = GET_MENUITEM_ID(stLayerStack[ucLayerIndex].ucMainMenu);
            COsdCoreFacSetMainmenuHighlight(ucTmp, (5<<4) | _CP_BLACK);
            if (ucTmp == 0)
                ucTmp = stLayerStack[ucLayerIndex].pMenuItemSet->ucItemCount - 1;
            else
                ucTmp--;
            COsdCoreFacSetMainmenuHighlight(ucTmp, (8<<4) | _CP_BLACK);
            stLayerStack[ucLayerIndex].pMenuItem = stLayerStack[ucLayerIndex].pMenuItemSet->ppMenuItems[ucTmp];
            stLayerStack[ucLayerIndex].ucMainMenu = _BIT7 | ucTmp;
            COsdCoreFacDispSubmenuItems();
        }
        break;

    case _DOWN_KEY_MESSAGE:
        if ( IS_MENU_ON(stLayerStack[ucLayerIndex].ucSubMenu) )
        {
            ucTmp = GET_MENUITEM_ID(stLayerStack[ucLayerIndex].ucSubMenu);
            COsdCoreFacSetSubmenuHighlight(ucTmp, 5<<4 | _CP_BLACK);
            ucTmp++;
            if (ucTmp > stLayerStack[ucLayerIndex].pMenuItem->ucItemCount - 1)
                ucTmp = 0;
            COsdCoreFacSetSubmenuHighlight(ucTmp, 8<<4 | _CP_BLACK);
            stLayerStack[ucLayerIndex].ucSubMenu = _BIT7 | ucTmp;
            RTD_Log(LOGGER_INFO,"is menu on -----\n");
        }
        else if ( !IS_MENU_ON(stLayerStack[ucLayerIndex].ucMainMenu) )
        {
            ucTmp = 0;
            COsdCoreFacSetMainmenuHighlight(ucTmp, (_CP_PURE_YELLOW_ORANGE<<4)|_CP_BLACK);
            stLayerStack[ucLayerIndex].ucMainMenu = _BIT7 | ucTmp;
            stLayerStack[ucLayerIndex].pMenuItem = stLayerStack[ucLayerIndex].pMenuItemSet->ppMenuItems[ucTmp];
            COsdCoreFacDispSubmenuItems();
            COsdFxDrawWindow(&tFac_Submenu_WindowStyle);
            RTD_Log(LOGGER_INFO,"!IS_MENU_ONon -----\n");
        }
        else
        {
            ucTmp = GET_MENUITEM_ID(stLayerStack[ucLayerIndex].ucMainMenu);
            COsdCoreFacSetMainmenuHighlight(ucTmp, (5<<4) | _CP_BLACK);
            ucTmp++;
            if ( ucTmp > stLayerStack[ucLayerIndex].pMenuItemSet->ucItemCount - 1)
                ucTmp = 0;
            COsdCoreFacSetMainmenuHighlight(ucTmp, (8<<4) | _CP_BLACK);
            stLayerStack[ucLayerIndex].pMenuItem = stLayerStack[ucLayerIndex].pMenuItemSet->ppMenuItems[ucTmp];
            stLayerStack[ucLayerIndex].ucMainMenu = _BIT7 | ucTmp;
            COsdCoreFacDispSubmenuItems();
            RTD_Log(LOGGER_INFO,"!else on -----\n");
        }
        break;

    case _MENU_KEY_MESSAGE:
        if ( IS_MENU_ON(stLayerStack[ucLayerIndex].ucSubMenu) )
        {
            ucTmp = GET_MENUITEM_ID(stLayerStack[ucLayerIndex].ucSubMenu);
            COsdCoreFacSetSubmenuHighlight(ucTmp, _CP_WHITE<<4 | _CP_BLACK);
            stLayerStack[ucLayerIndex].ucSubMenu = 0;
        }
        else if ( IS_MENU_ON(stLayerStack[ucLayerIndex].ucMainMenu) )
        {
            ucTmp = GET_MENUITEM_ID(stLayerStack[ucLayerIndex].ucMainMenu);
            COsdCoreFacSetMainmenuHighlight(ucTmp, (_CP_WHITE<<4) | _CP_BLACK);
            stLayerStack[ucLayerIndex].ucMainMenu = 0;
            COsdCoreFacDispSubmenuItems();
            COsdFxCloseWindow(tFac_Submenu_WindowStyle.ucWindowID);
        }
        break;

    case _EXIT_KEY_MESSAGE:
        CLR_FACTORY_MODE_MENU();
        
        //RTD_Log(LOGGER_LEVEL,"\n _SHOW_INFO_STATE bCheckCCStatus=%d \n",(UINT32)bCheckCCStatus);
        COsdCoreExitOsd();
        break;

    default:
        break;
    }
#endif
}
#if 0
/**
 * The message event handler of factory mode menu item.
 * @return none.
 */
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
#endif

/* =========================================================================== */
/**
 * RTK factory mode.
 */

/**
 *
 * @return none.
 */
void COsdCoreRtkFacInitMenu(void)
{
#if 0
#if 0
    UINT8 j;
    UINT8 ucCnt;
    UINT8 code tStr_SvnCode[] = SYS_COMPILER_VERSION;
    UINT8 code tSvnCodeDigit = sizeof(tStr_SvnCode) / sizeof(UINT8);

    fw_timer_event_CancelTimerEvent(timerevent_ui_table_OSD_timeout_event);

    SET_MAINMENU_ON();
    SET_RTK_FACTORY_MODE();
    ucOsdState = _RTK_FACTORY_MODE_STATE;

    ucLayerIndex = 0;
    stLayerStack[0].pMenuItem = &tMenuFactoryMode;
    stLayerStack[0].ucSubMenu = 0;

    COsdFxBlending(0);	// disable OSD blending

    ap_osdfx_map_setting(_OSD_MAP0);
    for (j=0; j<13; j++)
        COsdFxSetRowCtrl1(3+j, ROW_HEIGHT(24));
    COsdFxSetWinRefPos(40, 80);

    //ap_osdfx_load_vlc_font(tFont_SourcePatch, _FRAME_2BIT_PATCH_ADDR, 12);

    COsdFxDrawWindow(&tSource_Select_Title_WindowStyle);
    COsdFxMoveWindow(tSource_Select_Title_WindowStyle.ucWindowID, &tFactory_mode_Title_WindowPos);
    COsdFxDrawWindow(&tSource_Select_Window_Style);
    COsdFxMoveWindow(tSource_Select_Window_Style.ucWindowID, &tFactory_mode_WindowPos);
    COsdFxDrawWindow(&tSource_Select_Hint_WindowStyle);
    COsdFxMoveWindow(tSource_Select_Hint_WindowStyle.ucWindowID, &tFactory_mode_Hint_WindowPos);
    COsdFxDrawWindow(&tSource_Select_Highlight_WindowStyle);
    COsdFxMoveWindow(tSource_Select_Highlight_WindowStyle.ucWindowID, &tFactory_mode_Highlight_WindowPos);

    COsdFxSet2BitColor(0, _CP_BLACK);
    COsdFxSet2BitColor(1, _CP_WHITE);
    COsdFxSet2BitColor(2, _CP_PASTEL_CYAN_BLUE);
    COsdFxSet2BitColor(3, _CP_DARK_CYAN_BLUE);

    // patch the title bar
    //COsdFxWrite2BitFont(1, 0, _FRAME_PATCH_FS+3);
    //for(j=1;j<16;j++)
    //	COsdFxWrite2BitFont(1, j, _FRAME_PATCH_FS+4);
    //COsdFxWrite2BitFont(1, j, _FRAME_PATCH_FS+5);
    //j++;
    //if ( j < 40)
    //	COsdFxClearText(1, j, 1, 40-j);
    OSDLine(1, 0, 30, 0xFF,_WRITE_BYTE2);

    // draw title string
    ucCnt = COsdFxLoadText(0, 1, 18, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tOsdFactoryModeStr);
    // display svn number
    ucCnt += COsdFxLoadText(0, ucCnt, 0, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tStr_svn);
    for (j=0; j<tSvnCodeDigit; j++)
    {
        ucOsdCoreBuf[j] = tStr_SvnCode[j] - '0' + _0_;
    }
    ucOsdCoreBuf[j-1] = _END_;
    ucCnt += COsdFxLoadText(0, ucCnt, 0, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, ucOsdCoreBuf);
    if (ucCnt < _OSD_TITLE_ROW_LENGTH)
        COsdFxClearText(0, ucCnt, 1, _OSD_TITLE_ROW_LENGTH-ucCnt);

    // patch the hint bar
    //COsdFxLoadBlankCmd(15, 0, 244, _CP_BLACK);
    //COsdFxWrite2BitFont(15, 1, _FRAME_PATCH_FS);
    //for(j=0;j<9;j++)
    //	COsdFxWrite2BitFont(15, 2+j, _FRAME_PATCH_FS+1);
    //COsdFxWrite2BitFont(15, 2+j, _FRAME_PATCH_FS+2);
    j = j + 3;
    if (j < 60)
        COsdFxClearText(15, j, 1, 60-j);

    // display hint string
    COsdFxLoadBlankCmd(16, 0, 30, _CP_BLACK);
    //COsdCoreRtkFacDispHint(stLayerStack[ucLayerIndex].pMenuItem->pItemList[0].ucType);

    COsdCoreRtkFacDrawItems(0);

    COsdFxEnableOsd();
#else
	UINT8 j;
	UINT8 ucCnt;
	//UINT8 temp;
	UINT8 code tStr_SvnCode[] = SYS_COMPILER_VERSION;
	UINT8 code tSvnCodeDigit = sizeof(tStr_SvnCode) / sizeof(UINT8);

	fw_timer_event_CancelTimerEvent(timerevent_ui_table_OSD_timeout_event);

	SET_MAINMENU_ON();
	SET_RTK_FACTORY_MODE();
	ucOsdState = _RTK_FACTORY_MODE_STATE;
    //temp = ucTimTest;// = 33;
    ucLayerIndex = 0;
    stLayerStack[0].pMenuItem = &tMenuFactoryMode;
    stLayerStack[0].ucSubMenu = 0;

    ap_osdfx_map_setting(_OSD_MAP0);
    for (j=0; j<13; j++)
        COsdFxSetRowCtrl1(3+j, ROW_HEIGHT(24));
    COsdFxSetWinRefPos(40, 80);

    CFacInit();
        
    COsdFxSet2BitColor(0, _CP_BLACK);
    COsdFxSet2BitColor(1, _CP_WHITE);
    COsdFxSet2BitColor(2, _CP_PASTEL_CYAN_BLUE);
    COsdFxSet2BitColor(3, _CP_DARK_CYAN_BLUE);
    
    OSDLine(1, 0, 30, 0xFF,_WRITE_BYTE2);
    
    // draw title string
    ucCnt = COsdFxLoadText(0, 1, 18, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tOsdFactoryModeStr);
    // display svn number
    ucCnt += COsdFxLoadText(0, ucCnt, 0, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tStr_svn);
    for (j=0; j<tSvnCodeDigit; j++)
    {
        ucOsdCoreBuf[j] = tStr_SvnCode[j] - '0' + _0_;
    }
    ucOsdCoreBuf[j-1] = _END_;
    ucCnt += COsdFxLoadText(0, ucCnt, 0, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, ucOsdCoreBuf);
    if (ucCnt < _OSD_TITLE_ROW_LENGTH)
        COsdFxClearText(0, ucCnt, 1, _OSD_TITLE_ROW_LENGTH-ucCnt);

	j = j + 3;
	if (j < 60)
	    COsdFxClearText(15, j, 1, 60-j);

	// display hint string
	COsdFxLoadBlankCmd(16, 0, 30, _CP_BLACK);
    COsdCoreRtkFacDrawItems(0);

	COsdFxEnableOsd();
	
#endif
#endif
}
#if 0//1
/**
 * To redraw factory mode menu items.
 *
 */
void COsdCoreRtkFacDrawItems(UINT8 ucSelectedItem)
{

    UINT8 j;
    UINT8 ucCnt;
    UINT8 ucWidth;
    UINT8 ucItemID;
    //stLayerStack[0].pMenuItem->pItemList[1].pString = &tMenuFactoryMode.pItemList[1].pString;
    pItemObj = &tRtkFactoryItemSet;
  
    RTD_Log(LOGGER_LEVEL,"\nfac111 = %d\n",(UINT32)stLayerStack[0].pMenuItem);

    RTD_Log(LOGGER_LEVEL,"\nfac222 = %d\n",(UINT32)&tMenuFactoryMode);

	RTD_Log(LOGGER_LEVEL,"\nfac333 = %d\n",(UINT32)&pItemObj);
	RTD_Log(LOGGER_LEVEL,"\nfac3330 = %d\n",(UINT32)pItemObj);
    
	RTD_Log(LOGGER_LEVEL,"\nfac3331 = %d\n",(UINT32)pItemObj[0].pString);
        RTD_Log(LOGGER_LEVEL,"\nfac = %d\n",(UINT32)&tOsdBacklightStr);
        RTD_Log(LOGGER_LEVEL,"\nfac1 = %d\n",(UINT32)tOsdBacklightStr);

	RTD_Log(LOGGER_LEVEL,"\nfac444 = %d\n",(UINT32)&tRtkFactoryItemSet);
	RTD_Log(LOGGER_LEVEL,"\nfac4 = %d\n",(UINT32)&tRtkFactoryItemSet[0].pString);

   /* RTD_Log(LOGGER_LEVEL,"\nfac 333 = %d\n",(UINT32)stLayerStack[0].pMenuItem->pItemList[1].pString);
    
    RTD_Log(LOGGER_LEVEL,"\nfac 444 = %d\n",(UINT32)tMenuFactoryMode.pItemList[1].pString);
    RTD_Log(LOGGER_LEVEL,"\nfac 555 = %d\n",(UINT32)&tMenuFactoryMode.pItemList[1].pString);
*/
    WaitFor_DEN_STOP();
    for (j=0; j<13; j++)
    {
        // ­×¹¢«eºÝ°Ï°ì
        COsdFxLoadBlankCmd(3+j, 0, 20, _CP_BLACK);
        ucCnt = 1;
        ucPage = ucSelectedItem / 13;
        

        // Draw Items
        if ( ucPage*13+j <29)// stLayerStack[ucLayerIndex].pMenuItem->ucItemCount)
        {
            ucItemID = ucPage*13+j;
            // Draw item string
            //RTD_Log(LOGGER_LEVEL,"\n string = %d \n",(UINT32)stLayerStack[0].pMenuItem->pItemList[1].pString);
            
            //RTD_Log(LOGGER_LEVEL,"\n string111 = %d \n",(UINT32)&tOsdD_DLtiStr);
            //stLayerStack[0].pMenuItem->pItemList[1].pString= tOsdD_DLtiStr;
            ucCnt += COsdFxLoadText(3+j, ucCnt, 0, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, pItemObj[0].pString);
            COsdFxClearText(3+j, ucCnt, 1, 60-ucCnt);
/*
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
            }*/
        }
        else
        {
            COsdFxClearText(3+j, 1, 1, 60-1);
        }
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
    case _LEFT_KEY_MESSAGE:
    case _RIGHT_KEY_MESSAGE:
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

#if 0
/**
 * Update factory mode menu slider item.
 *
 */
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

/**
 * Update factory mode menu bracket item.
 *
 */
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


/**
 * Update factory mode list item.
 *
 */
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
#endif

/**
 * To display hint depends on item type.
 *
 */
#if 0
void COsdCoreRtkFacDispHint(UINT8 ucItemType)
{
    UINT8 ucCnt;

    ucCnt = 1;
    if (ucItemType == _IMMEDIATE_TYPE || ucItemType == _SUBPAGE_TYPE)
    {
        ucCnt += COsdFxLoadText(16, ucCnt, 0, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tOsdUpDownButtonStr);
        ucCnt += COsdFxLoadText(16, ucCnt, 8, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tOsdSelectStr);
        ucCnt += COsdFxLoadText(16, ucCnt, 24, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tOsdOkRightButtonStr);
        ucCnt += COsdFxLoadText(16, ucCnt, 8, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tOsdEnterStr);
    }
    else //if (ucItemType == _SLIDER_TYPE || ucItemType == _POPON_SLIDER_TYPE || ucItemType == _BRACKET_TYPE || ucItemType == _BRACKET1_TYPE)
    {
        ucCnt += COsdFxLoadText(16, ucCnt, 0, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tOsdUpDownButtonStr);
        ucCnt += COsdFxLoadText(16, ucCnt, 8, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tOsdSelectStr);
        ucCnt += COsdFxLoadText(16, ucCnt, 24, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tOsdLeftRightButtonStr);
        ucCnt += COsdFxLoadText(16, ucCnt, 8, FONT_COLOR(_CP_WHITE, _CP_BLACK), 0, tOsdAdjustStr);
    }

    if (ucCnt < 60)
        COsdFxClearText(16, ucCnt, 1, 60-ucCnt);
}
#endif

#if 0
/**
 * To move menu high-light bar.
 *
 */
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
#endif



/**
 *
 * @}
 */

