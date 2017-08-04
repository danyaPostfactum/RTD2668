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
#ifndef _FLOW_SCALER_H
#define _FLOW_SCALER_H
#include "sparrow.h"
#include "scaler\scaler.h"

#define _H_POSITION_MOVE_TOLERANCE      0x32 // +- 50 pixels
#define _V_POSITION_MOVE_TOLERANCE      0x32 // 50 lines

#define WaitFor_IVS1()			drv_scaler_WaitFor_Event(_EVENT_IVS)
#define WaitFor_IVS_END()			drv_scaler_WaitFor_Event(_EVENT_IVS_END)
#define WaitFor_IEN_START1()		drv_scaler_WaitFor_Event(_EVENT_IENSTART1)
#define WaitFor_IEN_STOP1()		drv_scaler_WaitFor_Event(_EVENT_IENSTOP1)
#define WaitFor_DEN_STOP()		drv_scaler_WaitFor_Event(_EVENT_DENSTOP)
#define WaitFor_EN_START_MAIN() drv_scaler_WaitFor_Event(_EVENT_DENMAIN_START)
#define WaitFor_EN_STOP_MAIN()	drv_scaler_WaitFor_Event(_EVENT_DENMAIN_STOP)

#define Scaler_ClearLineBufferStatus() rtdf_clearBits(SCALEDOWN_ICH1_IBUFF_STATUS_ADDR, _BIT0 | _BIT1 | _BIT2)
//If source is FR, handle display ratio at movie or caption
//void flow_scaler_cal_vgip_capture(void);

/* setup capture window */
//void flow_scaler_set_vgip_capture(void);

// Calculate the output size of scale down
void flow_scaler_Decide_ScaleSize(void);

// Enable OnLine Measure Watchdog
//void flow_scaler_Enable_Measure_Watchdog(void);

// Disable OnLine Measure Watchdog
//void flow_scaler_Disable_Measure_Watchdog(void);

// Enable FrameSync Measure Watchdog
//void flow_scaler_Enable_Framesync_WatchDog(void);

// Disable FrameSync Measure Watchdog
//void flow_scaler_Disable_Framesync_WatchDog(void);

#endif
