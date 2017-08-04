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
* @
*/
#ifndef _SCALER_H
#define _SCALER_H
#include "sparrow.h"

// the paramter for increment / non-increment address
//#define _AUTO_INC	0
//#define _NON_INC	(1 << 31)

//--------------------------------------------------
// Definations of Scaler Write/Read Type
//--------------------------------------------------
#define _NON_INC                    1
#define _AUTO_INC                        0
#define _BURST                          2
#define _END                            0

typedef enum
{
    _EVENT_IVS	= 0,
    _EVENT_IVS_END,
    _EVENT_IENSTART1,
    _EVENT_IENSTOP1,
    _EVENT_DENSTOP,
    _EVENT_DENMAIN_START,
    _EVENT_DENMAIN_STOP
};

enum
{
    DISPLAY_MODE_FRC = 0,
    DISPLAY_MODE_FRAME_SYNC
};


//------------------------------------------
// Definitions of Table Flag for Scaler Access.
//------------------------------------------
#define _TBL_ADDR_NO_INC 	(_BIT7)
#define _TBL_ZERO 			(_BIT6)
#define _TBL_END			(_ZERO)

extern UINT8 ucIPH_Max_Margin;
extern UINT8 ucIPH_Min_Margin;
extern UINT8 ucIPV_Max_Margin;
extern UINT8 ucIPV_Min_Margin;

extern UINT8 ucIVS2DVSDelay;
extern UINT8 IVS2DVSDelay_VGA;
extern UINT8 ucIHSDelay;


// Disable Watch enable in D-domain
//#define drv_scaler_enable_ddomain_wdg(bit enable) 	rtdf_maskb(MAIN_DISPLAY_CONTROL_RSV_0x2100, ~_BIT15, enable ? _BIT15 : 0)

void drv_scaler_WriteTable(UINT32* array);
bit drv_scaler_WaitFor_Event(UINT8 event);
void drv_scaler_enable_fs_wdg(bit enable);
void drv_scaler_enable_measure_wdg(bit enable);
void drv_scaler_enable_ddomain_wdg(bit enable);
void drv_scaler_set_display_mode(bit mode);
void drv_scaler_update_mode_para(void);
void drv_scaler_poweron_Init(void);
void drv_scaler_idomain_reset(void);

#endif // #ifdef _SCALER_H
