/*===========================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2005
 * All rights reserved.
 * ========================================================== */

/*==================== File Description ===================== */
/**
 * @file
 * 	This file is osd flow declaration for osd related functions.
 *
 * @author 	$Author: forster $
 * @date 	$Date: 2006-02-06 14:03:22 +0800 (?Ÿæ?ä¸€, 06 äºŒæ? 2006) $
 * @version	$Revision: 2 $
 */

/**
 * @addtogroup osd
 * @{
 */

#ifndef _OSDFLOW_H
#define _OSDFLOW_H
/*===================== Module Dependency  ================== */
#include "osdobjectdef.h"

/*======================== PRIVATE CONSTANTS =================================*/

/*======================== PRIVATE MACROS ====================================*/

/*======================== PRIVATE TYPES =====================================*/

/*======================== PRIVATE VARIABLES =================================*/

/*========================== Variables =======================================*/
#if !defined(_OSDFLOW_C)
// Mainmenu
extern ItemSlider code tVolumeSlider;
extern ItemObject code tVolumeItem;
extern ItemObject code tChannelScanItem;
extern ItemObject code tTableScanItem;
extern ItemObject code tManualScanItem;
extern ItemList code tDisplayRatioList;
extern ItemList code tDisplayIRList;


extern ItemSlider code tAudioTrebleItem;
extern ItemSlider code tAudioBassItem;

extern ItemList code tSourceSelectList;
extern ItemList code tSleepModeList;
extern ItemList code tPictureModeList;
extern ItemList code tEQModeList;
extern ItemList code tMTSModeList;

extern MenuItemSet code stAVMainMenuItemSet;
extern MenuItemSet code stVGAMainMenuItemSet;
extern MenuItemSet code stHDMIMainMenuItemSet;

extern ItemObject code tMenuDSubItemSet[];
extern ItemObject code tMenuToolItemSet[];
extern ItemObject code tRtkFactoryItemSet[];

extern MenuItem code tMenuFactoryMode;

extern MenuItemSet code tFac_Design_MenuItemSet;
extern MenuItemSet code tFac_Setting_MenuItemSet;


#endif // !defined(_OSDFLOW_C)



enum _menuID
{
    MAINMENU_COLOR = 0,
    MAINMENU_AUDIO,
    MAINMENU_VGA,
    MAINMENU_TOOL,
    MAINMENU_CHANNEL,
    SUBMENU_OSD_SETTING,
    SUBMENU_MUSIC_MODE,
    FAC_SUBMENU_WB,
    FAC_SUBMENU_COLORTEMP,
    FAC_SUBMENU_ADC_CALIBRATION,
    FAC_SUBMENU_SETTING_OPTION,
    FAC_SUBMENU_RESET,
    FAC_SUBMENU_VERSION,
    FAC_PIC_MODE_MENU,
    FAC_MUSIC_MODE_MENU,
    FAC_VOLUME_SETTING_MENU,
    FAC_EMI_MENU,
    FAC_DESIGN_MENU,
    RTK_FAC_MENU,
};


/*======================== FUNCTIONS =========================================*/


#endif // #ifndef _OSDFLOW_H
/**
 *
 * @}
 */

