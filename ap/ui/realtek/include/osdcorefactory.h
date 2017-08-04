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
 * @date 	$Date: 2009/03/17 $
 * @version 	$Revision: 10 $
 */

/**
 * @addtogroup osdcorefactory
 * @{
 */

/*===================== Module Dependency  ================== */


/*========================== Types ========================== */


/*======================== Definitions ====================== */


/*=====================  Extern Variables =================== */

/*========================= Definitions ====================== */
#define _PAGE_ITEMS_				(13)
#define _FAC_ITEM_LEN				(224)
#define _FAC_MAINITEM_COL			(19)
#define _FAC_SUBITEM_COL			(18)
#define _FAC_BRACKET_COL			(9)

#define IS_MENU_ON(_x)				((_x)&_BIT7)
#define SET_MENU_ON(_x)				(_x) |= _BIT7
#define GET_MENUITEM_ID(_x)			((_x)&0x0f)

/*======================= Extern Functions ================== */
void ap_factory_mode_check_pwd(void);
//UINT8 Ctest(void);

void COsdCoreFacInitMenu(void);
void COsdCoreFacMenuEventProc(void);
//void COsdCoreRtkFacDrawItems(UINT8 ucSelectedItem);

void COsdCoreRtkFacInitMenu(void);
//void COsdCoreRtkFacMenuEventProc(void);
void COsdCoreFacSetMenuTitleHighlight(UINT8 ucID, UINT8 ucColor);
void COsdCoreFacDispMainMenuItems(void);
void COsdCoreFacSetMainmenuHighlight(UINT8 ucID, UINT8 ucColor);
void COsdCoreFacDispSubmenuItems(void);
void COsdCoreFacSetSubmenuHighlight(UINT8 ucID, UINT8 ucColor);
void COsdCoreFacUpdateSliderItem(UINT8 ucID);
void COsdCoreFacUpdateBracketItem(UINT8 ucID);
void COsdCoreFacUpdateListItem(UINT8 ucID);
//void COsdCoreFacItemEventProc(void);
extern UINT8  code ucTimTest;



/**
 *
 * @}
 */

