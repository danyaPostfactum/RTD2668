/*===========================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2009
 * All rights reserved.
 * ========================================================== */

/*==================== File Description ===================== */
/**
 * @file
 * 	This file is a osd object declaration file.
 *
 * @author 	$Author: cf.tsai $
 * @date 	$Date: 2009-03-16 $
 * @version	$Revision: 10 $
 */

/**
 * @addtogroup osd
 * @{
 */

#ifndef _OSDOBJECTDEF_H
#define _OSDOBJECTDEF_H
/*===================== Module Dependency  ================== */


/*========================== Types ========================== */
typedef enum
{
    _NONE_TYPE,
    _BRACKET_TYPE,
    _BRACKET1_TYPE,
    _LIST_TYPE,
    _SLIDER_TYPE, // menu system handle slider value
    _IMMEDIATE_TYPE,
    _SUBPAGE_TYPE,
    _DESCRIPTION_TYPE,
}  ItemType;

typedef struct
{
    UINT16	usHStart;
    UINT16	usVStart;
    UINT16	usHEnd;
    UINT16	usVEnd;
} ItemPosition;

typedef struct
{
    UINT16	usWidth;
    UINT16	usHeight;
} ItemSize;

typedef struct
{
    UINT8	ucWindowID;
    ItemPosition	stPosition;
    UINT8	ucCtrlRegAddr0[3];
    UINT8	ucCtrlRegAddr2[3];
} WindowStyle;

typedef struct
{
    UINT8*	pString;
    UINT8	ucType;
    void*	pStruct;
} ItemObject;

typedef struct
{
    UINT8	ucControl;
    UINT8	ucMenuItemID;
} ItemImmediate;

#define _DESCRIPTION_DYNAMIC_STR		(_BIT1)
#define _DESCRIPTION_COLOR_CTRL			(_BIT2)
typedef struct
{
    UINT8	ucControl;
    UINT8	ucMenuItemID;
} ItemDescription;


typedef struct
{
    UINT8*	pString;
    UINT8	ucValue;
} ItemSelection;

typedef struct
{
    UINT8*	pString;
    UINT8	ucItemCount;
    UINT8	ucControl;				// bit0: specify multi-language
    ItemSelection*	pItemList;
    UINT8	ucMenuItemID;
} ItemList;


typedef struct
{
    UINT8*	pTitle;
    UINT8	ucItemCount;
    UINT8	ucMenuID;
    ItemObject*	pItemList;
} MenuItem;


typedef struct
{
    UINT8*	pString;
    UINT8	ucItemCount;
    MenuItem** ppMenuItems;
} MenuItemSet;


typedef struct
{
    UINT8**	pLabelString;
    UINT8	ucControl;				// bit0: specify multi-language
    UINT16	ucCount;				// value cannot exceed this value otherwise reset to default
    UINT8	ucMenuItemID;
} ItemBracket;


#define _SLIDER_VBR_RANGE		_BIT0
#define _SLIDER_MID_RANGE		_BIT1
#define _SLIDER_POPON			_BIT2
#define _SLIDER_NONLINEAR		_BIT3
typedef struct
{
    UINT8	ucCtrl;
    UINT8	ucMax;
    UINT8	ucMin;
    UINT8	ucMenuItemID;
} ItemSlider;


typedef struct
{
    UINT8*	pTitleString;
    MenuItemSet* pMenuItemSet;
    WindowStyle*	pBackGroundWindow;
    WindowStyle*	pForegroundWindow;
    WindowStyle* pMoveWindow;
    void		(*pHandlerFunction)(UINT8);
    UINT8	(*pGetFunction)();
    void  	(*pInitializeFunction)();
} VChipDialog;

typedef struct
{
    UINT8 code *Font;		//Font
    UINT8 Width;				//Width
} STitle;

/*======================== Definitions ====================== */

#endif // #ifndef _OSDOBJECTDEF_H

/**
 *
 * @}
 */

