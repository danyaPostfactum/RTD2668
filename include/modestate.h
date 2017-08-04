/*=============================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2005
  * All rights reserved.
  * ============================================ */

/*================= File Description ================= */
/**
 * @file
 * The file is a declaration and definition header file
 *
 * @author 	$Author:  $
 * @date 	$Date:  $
 * @version 	$Revision:  $
 * @ingroup
 */

/**
* @addtogroup
* @{
*/
#ifndef _MODESTATE_H
#define _MODESTATE_H

/*============================ Module dependency  ===========================*/
#include "sparrow.h"

enum _MODE_STATE
{
    _MODE_STATE_NONE = 0,						// none state
    _MODE_STATE_SEARCH,						// detect mode
    _MODE_STATE_ACTIVE,						// ative mode to show
    _MODE_STATE_VERIFY_ACTIVE,				// ative mode to show for verify
    _MODE_STATE_FREEZE,						// freeze
    _MODE_STATE_TV_SCAN,						// tv scan channel
    _MODE_STATE_SHOWSNOW,					// Show snow in TV
    _MODE_STATE_BLOCKING,					// BLOCK show
};

enum _MODE_MESSAGE
{
    _MODE_MSG_NONE = 0,
    _UI2FL_MAIN_INIT_SRC_EVENT = 1,	// 0x01:for source change state
//	_UI2FL_MAIN_SEARCH_MODE_EVENT,	// 0x02:for source change state
    // don't change _FL2FL_MAIN_STATE_PERIODIC_EVENT positon, it must be  _UI2FL_MAIN_INIT_SRC_EVENT + 1
    // because we use  _UI2FL_MAIN_INIT_SRC_EVENT + 1 in fw_modestate_process()
    _FL2FL_MAIN_STATE_PERIODIC_EVENT,							//  for search state
    _MODE_MSG_MAIN_SET_VGIP_AFTER_FOUND_MODE_SIGNAL,
    _FL2FL_MAIN_SET_FRAMESYNC_SIGNAL,
    _FL2FL_MAIN_SET_DISPLAY_ON,
//	_FL2FL_MAIN_ACTIVE_STATE_PERIODIC_EVENT,
    _UI2FL_FLOW_GO,
    _UI2FL_MAIN_DISPLAYRATIO_CHANGE,					// 0x1d change pip type
    _UI2FL_TV_AUTOSCAN_START,
    _UI2FL_TV_AUTOSCAN_CANCEL,
    _UI2FL_TV_MENUALSCAN_START,
    _UI2FL_TV_MENUALSCAN_CANCEL,
    _UI2FL_ADC_AUTOADJUST_START,
    _UI2FL_ADC_AUTOCOLOR_START,
    _MODE_MSG_ENABLE_BLOCKING,								// 0x21
    _MODE_MSG_DISABLE_BLOCKING,
    _FL2FL_POWER_KEY_MESSAGE,
    _FL2FL_POWER_SAVING_ON_MESSAGE,
    _FL2FL_POWER_SAVING_OFF_MESSAGE,
    _UI2FL_MAIN_VERIFY_SRC_EVENT,
    _FL2FL_ADC_AUTOADJUST_START,
    _MODE_MSG_MAIN_UNKNOWN
};

extern UINT8 ucUIScalerSyncInfo;


//use this parameter to sync UI & Scaler task status when change ui setting rapidly
#define GET_COLOR_SYSTEM_CHANGE() (ucUIScalerSyncInfo & _BIT0)
#define SET_COLOR_SYSTEM_CHANGE() (ucUIScalerSyncInfo = ucUIScalerSyncInfo |_BIT0)
#define CLR_COLOR_SYSTEM_CHANGE() (ucUIScalerSyncInfo = (ucUIScalerSyncInfo & ~_BIT0))



/**
 * This API is used to init some variables  input signal
 *
 * @param [input] :
 * @param [output] :
 * @return _MODE_RESULT_NOSIGNAL,_MODE_RESULT_NOSUPPORT,_MODE_RESULT_NOCABLE...
 *
 */
void fw_modestate_Init(void);

/**
 * This API is used to process mode state message
 *
 * @param [input] :  <msg>		{ Accept message from message queues }
 * @param [output] :
 * @return None
 *
 */
void fw_modestate_Process(void);

/**
 * This API will check is the display is on the avtive state.
 *
 * @param [input] :  <display>		{ The display you want to query }
 * @param [output] :
 * @return false: if display on search state or none state
 *             true: if display not on search state or none state
 *
 */
UINT8 fw_modestate_check_display_status(UINT8 display);

// void fw_modestate_CheckTVFHFVEvent(void);

void fw_dvdstatus_Process(void);

#endif /* _MODESTATE_H */

/**
 *
 * @
 */
