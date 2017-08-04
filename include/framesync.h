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
#ifndef _FRAMESYNC_H
#define _FRAMESYNC_H
#include "sparrow.h"
#include "panel\panelapi.h"
//--------------------------------------------------
// Offset_Step is the DCLK frequency step for searching frame-sync.
//--------------------------------------------------
#define _OFFSET_STEP    24
#define _SEARCH_TIME    24

#define _LAST_LINE_FOR_PROGRESS	Get_DISP_HORIZONTAL_TOTAL()/20
#define _LAST_LINE_FOR_INTERLACE	Get_DISP_HORIZONTAL_TOTAL()/12

UINT8 flow_framesync_do(void);
//void flow_framesync_Success_Setting(void);
void flow_framesync_Clear_StatusFlag(void);
UINT8 flow_linebuffer_status(void);
void flow_framesync_Clear_LineBufferFlag(void);
UINT8 flow_framesync_status(void);


#endif // #ifdef _FRAMESYNC_H
