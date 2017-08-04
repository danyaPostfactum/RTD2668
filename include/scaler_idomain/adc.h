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
#include "sparrow.h"
#include "source.h"
#include "flow_struct.h"
/**
* @addtogroup
* @
*/
#ifndef _ADC_H
#define _ADC_H

//------ERROR message : SUCCESS(MSB=0)
#define ERROR_SUCCEED   	(0x00)
#define ERROR_SUCCESS_1 	(0x01)
#define ERROR_SUCCESS_2 	(0x02)
#define ERROR_SUCCESS_4 	(0x04)
#define ERROR_SUCCESS_8 	(0x08)
#define ERROR_SUCCESS_16	(0x10)
#define ERROR_SUCCESS_32	(0x20)
#define ERROR_SUCCESS_64	(0x40)
//------ERROR message : ERROR(MSB=1)
#define ERROR_FINISH             (0xf7)
#define ERROR_NOTACTIVE 	(0xf8)
#define ERROR_NOISE_TOO_BIG (0xf9)
#define ERROR_TOO_SMALL 	(0xfa)
#define ERROR_TOO_BIG   	(0xfb)
#define ERROR_INPUT 		(0xfc)
#define ERROR_TIMEOUT   	(0xfd)
#define ERROR_ABORT 		(0xff)

#define PIXEL_1 			(0x00)
#define PIXEL_2 			(0x01)
#define PIXEL_4 			(0x02)
#define PIXEL_8 			(0x03)

#define AUTO_CLOCK_PRECISION		(1)// 2
#define _COLORS_RED  		(0x00)//0x02
#define _COLORS_GREEN		(0x02)//0x01
#define _COLORS_BLUE 		(0x01)//0x00
#define _COLORS_ALL               (0x03)
#define _COLOR_MAX   		(0x20)
#define _COLOR_MIN   		(0x00)
//--------------------------------------------------
// Hardware Auto Phase Step
//--------------------------------------------------
#define HWAUTO_STEP_1              (0x00)
#define HWAUTO_STEP_2              (0x01)
#define HWAUTO_STEP_4              (0x02)
#define HWAUTO_STEP_8              (0x03)
#define HWAUTO_STEP_16             (0x04)

#define HWAUTOSTEPNUM(x)		(x)
#define HWAUTOSTEPSTART(x) 	(x)

#define _ADC_SWAP_PATH0   	(0x00)  //Rin -> Rout ,  Gin -> Gout, Bin -> Bout
#define _ADC_SWAP_PATH1   (0x01)  //Gin -> Rout ,  Rin -> Gout, Bin -> Bout
#define _ADC_SWAP_PATH2   (0x02)  //Bin -> Rout ,  Gin -> Gout, Rin -> Bout
#define _ADC_SWAP_PATH3   (0x03)  //Bin -> Rout ,  Rin -> Gout, Gin -> Bout
#define _ADC_SWAP_PATH4   (0x04)  //Rin -> Rout ,  Bin -> Gout, Gin -> Bout
#define _ADC_SWAP_PATH5   (0x05)  //Gin -> Rout ,  Bin -> Gout, Rin -> Bout
#define _ADC_SWAP_PATH6   (0x06)  //Bin -> Rout ,  Rin -> Gout, Gin -> Bout
#define _ADC_SWAP_PATH7   (0x07)  //Bin -> Rout ,  Gin -> Gout, Rin -> Bout

//#if (_AUTO_COLOR)
#define VERTICAL_MARGIN 		(0x30)
#define HORIZONTAL_MARGIN   	(0x30)
#define SELECT_RED  				(0x01)
#define SELECT_GREEN			(0x02)
#define SELECT_BLUE 			(0x04)

#define SELECT_PR                       (0x01)//0          // Ken
#define SELECT_Y                         (0x02)// 1
#define SELECT_PB                       (0x04)// 2

#define SELECT_RGB  		(SELECT_RED | SELECT_GREEN | SELECT_BLUE)
//#endif
extern ADCGainOffset stYPbPrGainOffsetData;
extern ADCGainOffset2 stScartGainOffsetData;
typedef struct _AUTO_PARAMETER
{
    UINT8 ucFieldControl;
    UINT16 usAutoHStart;
    UINT16 usAutoHEnd;
    UINT16 usAutoVStart;
    UINT16 usAutoVEnd;
    UINT8 ucNMR;
    UINT8 ucNMG;
    UINT8 ucNMB;
    UINT8 ucColor;
    UINT8 ucFieldEnable;
    UINT8 ucVBoundSearch;
} AUTO_PARAMETER, * LP_AUTO_PARAETER;
extern UINT8  ucAutoControl;

#define GET_AUTOREADY()		(ucAutoControl & _BIT1)
#define SET_AUTOREADY()  	(ucAutoControl |= _BIT1)
#define CLR_AUTOREADY()  	(ucAutoControl &= ~_BIT1)
//[Code Sync][AlanLi][0980525][1]
//sync auto adjust flow
#define GET_AUTO_CONFIG_DOING()		(ucAutoControl & _BIT7)
#define SET_AUTO_CONFIG_DOING()  	(ucAutoControl |= _BIT7)
#define CLR_AUTO_CONFIG_DOING()  	(ucAutoControl &= ~_BIT7)
//[Code Sync][AlanLi][0980525][1][end]

// [Code Sync][CSW][0980806][1] CSW+ 0980806
UINT8 drv_adc_YPbPr_Auto_Phase(void);
// [Code Sync][CSW][0980806][1] end

void drv_adc_ADCPowerControl(UINT8 on_off);
void drv_adc_ADC_BangGapPowerControl(UINT8 on_off);
void drv_adc_SOY0_input_select(UINT8 input_index);
void drv_adc_SOY1_input_select(UINT8 input_index);

void drv_adc_DCRestore_Ctrl(UINT8 enable, ADC_DATA_PATH adc_path);
void drv_adc_Clamp_Ctrl(UINT8 output_enable, UINT8 input_index);
void drv_adc_ADCSelectInput(UINT8 input_index);
void drv_adc_setadcgainoffset(void);
void drv_adc_setcolorgainoffset(UINT8 addr, UINT16 parameter, UINT8 bios);
void drv_adc_gettargetvalue(UINT16* max_value, UINT16* min_value, UINT8 color);

UINT8 drv_adc_measurepositionv(UINT8 NMR_V, UINT8 NMG_V, UINT8 NMB_V, UINT8 color);
UINT8 drv_adc_measurepositionh(UINT8 NM_H, UINT8 color);
UINT8 drv_adc_auto_clockdo(UINT8 color);
UINT8 drv_adc_auto_phasedo(UINT8 color);
UINT8 drv_adc_auto_positiondo(UINT8 color);
UINT8 drv_adc_auto_config(void);
UINT8 drv_adc_measurecolor(UINT8 color, UINT8 margin, UINT16* rev);
UINT8 drv_adc_tunedevicegain(UINT8 ucColor, UINT16 *pMargin, UINT16 ucMax_Target);
UINT8 drv_adc_tunedeviceoffset(UINT8 ucColor, UINT16 *pMargin, UINT16 ucMin_Target);
UINT8 drv_adc_autobalance(void);
UINT8 drv_auto_pre_check(UINT8 *select_result);
void drv_auto_clock_fail_handler(UINT8 Curr_PosH, UINT8 Curr_Clock);
void drv_auto_phase_fail_handler(UINT8 Curr_Phase);
void drv_auto_position_fail_handler(UINT8 Curr_PosH, UINT8 Curr_PosV, UINT8 Curr_Phase, UINT8 Curr_Clock);


#endif // #ifndef _ADC_H
