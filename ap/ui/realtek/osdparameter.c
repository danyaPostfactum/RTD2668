/*===========================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2009
 * All rights reserved.
 * ========================================================== */

/*==================== File Description ===================== */
/**
 * @file
 * 	This file is gobal parameter definition for OSD related functions.
 *
 * @author 	$Author: cf.tsai $
 * @date 	$Date: 2009-03-16 $
 * @version $Revision: 10 $
 */

/**
 * @addtogroup osd
 * @{
 */

#define _OSDPARAMETER_C

/*===================== Module dependency  ================== */
#include "rtd_types.h"
//#include "sparrow.h"
//#include "osdregdef.h"
#include "osdobjectdef.h"
#include "osdfont.h"
#include "osdparameter.h"
#include "osdfunction.h"



/*======================= Private Types ===================== */


/*======================== Definitions ====================== */
#define _H_DELTA		(7)
#define _V_DELTA		(0)


/*========================== Variables ====================== */



#define _MAIN_MENU_WINDOW_STYLE_
WindowStyle code tPopupMenuWindowStyle =
{
    4,
    {21*12, 4*18, (21+18)*12, (4+8)*18},
    // ctrl byte 0: [5:3]border width [2:0]border height
    //      byte 1: [7:4]left/top color [3:0]right/bottom color
    //      byte 2:
    {(2 << 3) | 2, (1 << 4) | 3, _G_GRADIENT_INC|_R_GRADIENT_ENABLE|_B_GRADIENT_ENABLE},
    // ctrl byte 0:
    //      byte 1: [3:0]window color
    //      byte 2: [4]border/shadow/3d enable [3:1]window type [0]window enable
    {0x00, WINDOW_COLOR(2), _BLENDING_ENABLE | BORDER_ENABLE(1) | _3DBUTTON_TYPE1 | WINDOW_ENABLE(1)}
};

WindowStyle code tMainWindowStyle =
{
    0,
    {0, 0, (39*12+20), 18*18+10},
    // ctrl byte 0: [5:3]border width [2:0]border height
    //      byte 1: [7:4]left/top color [3:0]right/bottom color
    //      byte 2:
    {(1 << 3) | 1, (1 << 4) | 1, _GRADIENT_LEVEL(3) |_R_GRADIENT_ENABLE|_G_GRADIENT_ENABLE|_B_GRADIENT_ENABLE},
    // ctrl byte 0:
    //      byte 1: [3:0]window color
    //      byte 2: [4]border/shadow/3d enable [3:1]window type [0]window enable
    {0x00, _LEVEL_PER_GRADIENT(0)|WINDOW_COLOR(2), _BLENDING_ENABLE|_GRADIENT_ENABLE|_GRADIENT_V | BORDER_ENABLE(1) | _3DBUTTON_TYPE1 | WINDOW_ENABLE(1)}
};
WindowStyle code tModeWindowStyle =
{
    0,
    {0, 0, 11 * 12 + 4, 5 * 18},
    // ctrl byte 0: [5:3]border width [2:0]border height
    //      byte 1: [7:4]left/top color [3:0]right/bottom color
    //      byte 2:
    {(1 << 3) | 1, (1 << 4) | 1, _GRADIENT_LEVEL(3) |_R_GRADIENT_ENABLE|_G_GRADIENT_ENABLE|_B_GRADIENT_ENABLE},
    // ctrl byte 0:
    //      byte 1: [3:0]window color
    //      byte 2: [4]border/shadow/3d enable [3:1]window type [0]window enable
    {0x00, _LEVEL_PER_GRADIENT(0)|WINDOW_COLOR(2),_BLENDING_ENABLE|_GRADIENT_ENABLE|_GRADIENT_V | BORDER_ENABLE(1) | _3DBUTTON_TYPE1 | WINDOW_ENABLE(1)}
};
WindowStyle code tModeKeyWindowStyle =
{
    0,
    {0, 0, 15 * 12 + 4, 6 * 18},
    // ctrl byte 0: [5:3]border width [2:0]border height
    //      byte 1: [7:4]left/top color [3:0]right/bottom color
    //      byte 2:
    {(1 << 3) | 1, (1 << 4) | 1, _GRADIENT_LEVEL(3) |_R_GRADIENT_ENABLE|_G_GRADIENT_ENABLE|_B_GRADIENT_ENABLE},
    // ctrl byte 0:
    //      byte 1: [3:0]window color
    //      byte 2: [4]border/shadow/3d enable [3:1]window type [0]window enable
    {0x00, _LEVEL_PER_GRADIENT(0)|WINDOW_COLOR(2),_BLENDING_ENABLE|_GRADIENT_ENABLE|_GRADIENT_V | BORDER_ENABLE(1) | _3DBUTTON_TYPE1 | WINDOW_ENABLE(1)}
};

WindowStyle code tMuteBgWindowStyle =
{
    0,
    {0, 16, 54, 52},
    // ctrl byte 0: [5:3]border width [2:0]border height
    //      byte 1: [7:4]left/top color [3:0]right/bottom color
    //      byte 2:
    {(1 << 3) | 1, (1 << 4) | 1, _GRADIENT_LEVEL(3) |_R_GRADIENT_ENABLE|_G_GRADIENT_ENABLE|_B_GRADIENT_ENABLE},
    // ctrl byte 0:
    //      byte 1: [3:0]window color
    //      byte 2: [4]border/shadow/3d enable [3:1]window type [0]window enable
    {0x00, _LEVEL_PER_GRADIENT(0)|WINDOW_COLOR(2),_BLENDING_ENABLE|_GRADIENT_ENABLE|_GRADIENT_V | BORDER_ENABLE(1) | _3DBUTTON_TYPE1 | WINDOW_ENABLE(1)}
};
WindowStyle code tSourceWindowStyle =
{
    5,
    {0, 0, (16 * 12 - 4), 9*2*18+4},
    // ctrl byte 0: [5:3]border width [2:0]border height
    //      byte 1: [7:4]left/top color [3:0]right/bottom color
    //      byte 2:
   {(1 << 3) | 1, (1 << 4) | 1, _GRADIENT_LEVEL(3) |_R_GRADIENT_ENABLE|_G_GRADIENT_ENABLE|_B_GRADIENT_ENABLE},
    // ctrl byte 0:
    //      byte 1: [3:0]window color
    //      byte 2: [4]border/shadow/3d enable [3:1]window type [0]window enable
    {0x00, _LEVEL_PER_GRADIENT(0)|WINDOW_COLOR(2), _BLENDING_ENABLE|_GRADIENT_ENABLE|_GRADIENT_V | BORDER_ENABLE(1) | _3DBUTTON_TYPE1 | WINDOW_ENABLE(1)}
};
ItemPosition code tSourceWindowPos =
{
    0, 0, (12*12-4), _V_DELTA+34+16+24+4
};

WindowStyle code tHighLightWindowStyle =
{
    5,
    {_H_DELTA+70, _V_DELTA+62, _H_DELTA+68+33*12, _V_DELTA+62+30},
    // ctrl byte 0: [5:3]border width [2:0]border height
    //      byte 1: [7:4]left/top color [3:0]right/bottom color
    //      byte 2:
    {(0 << 3) | 0, (_CP_WHITE << 4) | _CP_DARK_YELLOW_ORANGE, _GRADIENT_LEVEL(1) |_R_GRADIENT_ENABLE|_G_GRADIENT_ENABLE|_B_GRADIENT_ENABLE},
    // ctrl byte 0:
    //      byte 1: [3:0]window color
    //      byte 2: [4]border/shadow/3d enable [3:1]window type [0]window enable
    {0x00, _LEVEL_PER_GRADIENT(0)|WINDOW_COLOR(6), _BLENDING_ENABLE|_GRADIENT_H|BORDER_ENABLE(0)  | WINDOW_ENABLE(1)}
};

WindowStyle code tTitleBarWindowStyle =
{
    2,
    {_H_DELTA+10, _V_DELTA+10, _H_DELTA+60+(12*37), _V_DELTA+43},
    // ctrl byte 0: [5:3]border width [2:0]border height,
    //      byte 1: [7:4]left/top color [3:0]right/bottom color,
    //      byte 2:
    {(0 << 3) | 0, (_CP_WHITE << 4) | _CP_DARKER_CYAN_BLUE, 0x00},
    // ctrl byte 0:
    //      byte 1: [3:0]window color
    //      byte 2: [4]border/shadow/3d enable [3:1]window type [0]window enable
    {0x00, WINDOW_COLOR(_CP_PASTEL_CYAN_BLUE), _BLENDING_ENABLE | BORDER_ENABLE(1) | _3DBUTTON_TYPE1 | WINDOW_ENABLE(1)}
};

WindowStyle code tItemAreaWindowStyle =
{
    0,
    {_H_DELTA+63, _V_DELTA+34+3, _H_DELTA+60+(12*37), _V_DELTA+74+258},
    // ctrl byte 0: [7:6]Multiple Factor of Gradient Step [5:3]border width [2:0]border height,
    //      byte 1: [7:4]left/top color [3:0]right/bottom color,
    //      byte 2: Gradient Ctrl [7]R Polarity [6]G polarity [5]B polarity [4:3]step pixels [2]enable Red [1]enable Green [0]enable Blue
    { (0x03 << 6) | (0 << 3) | 0, (_CP_WHITE << 4) | _CP_DARKER_CYAN_BLUE, (0x03 << 3) | 0x01},
    // ctrl byte 0:
    //      byte 1: [7]gradient type [6:4]levle size [3:0]window color
    //      byte 2: [6]gradient enable [5]gradient H/V [4]border/shadow/3d enable [3:1]window type [0]window enable
    {0x00, (0x01 << 4) | _CP_DARK_CYAN_BLUE, _BLENDING_ENABLE | (0x00 << 6) | (0x01 << 5) | BORDER_ENABLE(1) | _BORDER | WINDOW_ENABLE(1)}
};

WindowStyle code tHintBarWindowStyle =
{
    3,
    {_H_DELTA+10, _V_DELTA+74+258, _H_DELTA+60+(12*37), _V_DELTA+368},
    // ctrl byte 0: [5:3]border width [2:0]border height,
    //      byte 1: [7:4]left/top color [3:0]right/bottom color,
    //      byte 2:
    { (0 << 3) | 0, (_CP_WHITE << 4) | _CP_DARKER_CYAN_BLUE, 0x00},
    // ctrl byte 0:
    //      byte 1: [3:0]window color
    //      byte 2: [4]border/shadow/3d enable [3:1]window type [0]window enable
    {0x00, WINDOW_COLOR(_CP_PASTEL_CYAN_BLUE), _BLENDING_ENABLE | BORDER_ENABLE(1) | _3DBUTTON_TYPE1 | WINDOW_ENABLE(1)}
};

WindowStyle code tMainIconBgWindowStyle =
{
    4,
    {_H_DELTA+10, _V_DELTA+49, _H_DELTA+62, _V_DELTA+74+250},
    // ctrl byte 0: [5:3]border width [2:0]border height,
    //      byte 1: [7:4]left/top color [3:0]right/bottom color,
    //      byte 2:
    { (0 << 3) | 0, (_CP_WHITE << 4) | _CP_DARKER_CYAN_BLUE, 0x00},
    // ctrl byte 0:
    //      byte 1: [3:0]window color
    //      byte 2: [4]border/shadow/3d enable [3:1]window type [0]window enable
    {0x00, WINDOW_COLOR(_CP_PASTEL_CYAN_BLUE), _BLENDING_ENABLE | BORDER_ENABLE(1) | _3DBUTTON_TYPE1 | WINDOW_ENABLE(1)}
};

WindowStyle code tMainIconHighlightWindowStyle =
{
    6,
    {_H_DELTA+6, _V_DELTA+53, _H_DELTA+61, _V_DELTA+53+35},
    // ctrl byte 0: [7:6]Multiple Factor of Gradient Step [5:3]border width [2:0]border height,
    //      byte 1: [7:4]left/top color [3:0]right/bottom color,
    //      byte 2: Gradient Ctrl [7]R Polarity [6]G polarity [5]B polarity [4:3]step pixels [2]enable Red [1]enable Green [0]enable Blue
    { (0x00 << 3) | 0x00, (_CP_WHITE << 4) | _CP_DARKER_CYAN_BLUE,  _BIT7 | _BIT6 | _BIT5 | (0x01 << 3)| _BIT0},
    // ctrl byte 0:
    //      byte 1: [7]gradient type [6:4]levle size [3:0]window color
    //      byte 2: [6]gradient enable [5]gradient H/V [4]border/shadow/3d enable [3:1]window type [0]window enable
    {0x00, WINDOW_COLOR(_CP_PURE_CYAN_BLUE), _BLENDING_ENABLE | GRADIENT_ENABLE(0) | _BIT5 | BORDER_ENABLE(1) | _3DBUTTON_TYPE1 | WINDOW_ENABLE(1)}
};

WindowStyle code tMainIconHighlightWindowStyle2 =
{
    7,
    {_H_DELTA+7, _V_DELTA+53+17, _H_DELTA+60, _V_DELTA+53+34},
    // ctrl byte 0: [7:6]Multiple Factor of Gradient Step [5:3]border width [2:0]border height,
    //      byte 1: [7:4]left/top color [3:0]right/bottom color,
    //      byte 2: Gradient Ctrl [7]R Polarity [6]G polarity [5]B polarity [4:3]step pixels [2]enable Red [1]enable Green [0]enable Blue
    { (0x00 << 3) | 0x00, (_CP_WHITE<< 4) | _CP_DARKER_CYAN_BLUE, _BIT7 | _BIT6 | _BIT5 | (0x00 << 3) | _BIT2 | _BIT1 | _BIT0},
    // ctrl byte 0:
    //      byte 1: [7]gradient type [6:4]levle size [3:0]window color
    //      byte 2: [6]gradient enable [5]gradient H/V [4]border/shadow/3d enable [3:1]window type [0]window enable
    {0x00, WINDOW_COLOR(_CP_DARK_CYAN_BLUE), _BLENDING_ENABLE | GRADIENT_ENABLE(1) | _BIT5 | BORDER_ENABLE(0) | _BORDER | WINDOW_ENABLE(1)}
};

#define _POP_ON_SLIDER_WINDOW_STYLE_
WindowStyle code tPopon_Slider_Title_WindowStyle =
{
    2,
    {_H_DELTA+10, _V_DELTA, _H_DELTA+27*12, _V_DELTA+6*18},
    // ctrl byte 0: [5:3]border width [2:0]border height,
    //      byte 1: [7:4]left/top color [3:0]right/bottom color,
    //      byte 2:
    {(1 << 3) | 1, (1 << 4) | 1, _G_GRADIENT_INC|_R_GRADIENT_ENABLE|_B_GRADIENT_ENABLE},
    // ctrl byte 0:
    //      byte 1: [3:0]window color
    //      byte 2: [4]border/shadow/3d enable [3:1]window type [0]window enable
    {0x00, WINDOW_COLOR(5), _BLENDING_ENABLE | BORDER_ENABLE(1) | _3DBUTTON_TYPE1 | WINDOW_ENABLE(1)}
};

WindowStyle code tPopon_Slider_WindowStyle =
{
    0,
    {_H_DELTA+10, _V_DELTA, _H_DELTA+27*12, _V_DELTA+6*18},
    // ctrl byte 0: [7:6]Multiple Factor of Gradient Step [5:3]border width [2:0]border height,
    //      byte 1: [7:4]left/top color [3:0]right/bottom color,
    //      byte 2: Gradient Ctrl [7]R Polarity [6]G polarity [5]B polarity [4:3]step pixels [2]enable Red [1]enable Green [0]enable Blue
   {(1 << 3) | 1, (1 << 4) | 1, _GRADIENT_LEVEL(3) |_R_GRADIENT_ENABLE|_G_GRADIENT_ENABLE|_B_GRADIENT_ENABLE},
    // ctrl byte 0:
    //      byte 1: [3:0]window color
    //      byte 2: [4]border/shadow/3d enable [3:1]window type [0]window enable
    {0x00, _LEVEL_PER_GRADIENT(0)|WINDOW_COLOR(2),_BLENDING_ENABLE|_GRADIENT_ENABLE|_GRADIENT_V | BORDER_ENABLE(1) | _3DBUTTON_TYPE1 | WINDOW_ENABLE(1)}
};

WindowStyle code tPopon_Slider_Hint_WindowStyle =
{
    4,
    {_H_DELTA+10, _V_DELTA, _H_DELTA+27*12, _V_DELTA+6*18},
    // ctrl byte 0: [5:3]border width [2:0]border height,
    //      byte 1: [7:4]left/top color [3:0]right/bottom color,
    //      byte 2:
    {(1 << 3) | 1, (1 << 4) | 1, _G_GRADIENT_INC|_R_GRADIENT_ENABLE|_B_GRADIENT_ENABLE},
    // ctrl byte 0:
    //      byte 1: [3:0]window color
    //      byte 2: [4]border/shadow/3d enable [3:1]window type [0]window enable
    {0x00, WINDOW_COLOR(5), _BLENDING_ENABLE | BORDER_ENABLE(1) | _3DBUTTON_TYPE1 | WINDOW_ENABLE(1)}
};


#define _SOURCE_SELECT_MENU_WINDOW_STYLE_
WindowStyle code tSource_Select_Title_WindowStyle =
{
    2,
    {_H_DELTA+10, _V_DELTA+8, _H_DELTA+60+(12*39), _V_DELTA+39},
    // ctrl byte 0: [5:3]border width [2:0]border height,
    //      byte 1: [7:4]left/top color [3:0]right/bottom color,
    //      byte 2:
    {(0 << 3) | 0, (_CP_DARKER_CYAN_BLUE << 4) | _CP_DARKER_CYAN_BLUE, 0x00},
    // ctrl byte 0:
    //      byte 1: [3:0]window color
    //      byte 2: [4]border/shadow/3d enable [3:1]window type [0]window enable
    {0x00, WINDOW_COLOR(_CP_PASTEL_CYAN_BLUE), _BLENDING_ENABLE | BORDER_ENABLE(1) | _3DBUTTON_TYPE1 | WINDOW_ENABLE(1)}
};

WindowStyle code tSource_Select_Window_Style =
{
    0,
    {_H_DELTA+10, _V_DELTA+34, _H_DELTA+60+(12*39), _V_DELTA+34+(24*13)+18},
    // ctrl byte 0: [7:6]Multiple Factor of Gradient Step [5:3]border width [2:0]border height,
    //      byte 1: [7:4]left/top color [3:0]right/bottom color,
    //      byte 2: Gradient Ctrl [7]R Polarity [6]G polarity [5]B polarity [4:3]step pixels [2]enable Red [1]enable Green [0]enable Blue
    { (0x03 << 6) | (0 << 3) | 0, (_CP_WHITE << 4) | _CP_DARKER_CYAN_BLUE, (0x03 << 3) | 0x01},
    // ctrl byte 0:
    //      byte 1: [7]gradient type [6:4]levle size [3:0]window color
    //      byte 2: [6]gradient enable [5]gradient H/V [4]border/shadow/3d enable [3:1]window type [0]window enable
    {0x00, (0x01 << 4) | _CP_DARK_CYAN_BLUE, _BLENDING_ENABLE | (0x00 << 6) | (0x01 << 5) | BORDER_ENABLE(1) | _BORDER | WINDOW_ENABLE(1)}
};

WindowStyle code tSource_Select_Hint_WindowStyle =
{
    4,
    {_H_DELTA+10, _V_DELTA+34+(24*13)+14, _H_DELTA+60+(12*39), _V_DELTA+34+(24*13)+50},
    // ctrl byte 0: [5:3]border width [2:0]border height,
    //      byte 1: [7:4]left/top color [3:0]right/bottom color,
    //      byte 2:
    { (0 << 3) | 0, (_CP_DARKER_CYAN_BLUE << 4) | _CP_DARKER_CYAN_BLUE, 0x00},
    // ctrl byte 0:
    //      byte 1: [3:0]window color
    //      byte 2: [4]border/shadow/3d enable [3:1]window type [0]window enable
    {0x00, WINDOW_COLOR(_CP_PASTEL_CYAN_BLUE), _BLENDING_ENABLE | BORDER_ENABLE(1) | _3DBUTTON_TYPE1 | WINDOW_ENABLE(1)}
};

WindowStyle code tSource_Select_Highlight_WindowStyle =
{
    3,
    {_H_DELTA+10+4, _V_DELTA+34+16, _H_DELTA+60+(12*30)-4, _V_DELTA+34+16+24},
    // ctrl byte 0: [5:3]border width [2:0]border height
    //      byte 1: [7:4]left/top color [3:0]right/bottom color
    //      byte 2:
    {(0 << 3) | 0, (_CP_WHITE << 4) | _CP_WHITE, 0x00},
    // ctrl byte 0:
    //      byte 1: [3:0]window color
    //      byte 2: [4]border/shadow/3d enable [3:1]window type [0]window enable
    {0x00, WINDOW_COLOR(8), _BLENDING_ENABLE | BORDER_ENABLE(1) | _3DBUTTON_TYPE1 | WINDOW_ENABLE(1)}
};

#define _EQ_CONTROL_MENU_WINDOW_POS_
ItemPosition code tEqControl_title_window_pos =
{
    _H_DELTA+10, _V_DELTA+8, _H_DELTA+60+(12*28), _V_DELTA+39
};

ItemPosition code tEqControl_menu_window_pos =
{
    _H_DELTA+10, _V_DELTA+34, _H_DELTA+60+(12*28), _V_DELTA+34+(24*10)+8
};

ItemPosition code tEqControl_hint_window_pos =
{
    _H_DELTA+10, _V_DELTA+34+(24*10)+8, _H_DELTA+60+(12*28), _V_DELTA+34+(24*11)+10
};
ItemPosition code tEqControl_highlight_window_pos =
{
    _H_DELTA+20, _V_DELTA+34+12, _H_DELTA+20+36, _V_DELTA+34+(24*8)+14
};

#define _SOURCE_INFO_WINDOW_STYLE_
WindowStyle code tSource_Info_Title_WindowStyle =
{
    2,
    {_H_DELTA+10, _V_DELTA+8, _H_DELTA+60+(12*16), _V_DELTA+39},
    // ctrl byte 0: [5:3]border width [2:0]border height,
    //      byte 1: [7:4]left/top color [3:0]right/bottom color,
    //      byte 2:
    {(0 << 3) | 0, (_CP_DARKER_CYAN_BLUE << 4) | _CP_DARKER_CYAN_BLUE, 0x00},
    // ctrl byte 0:
    //      byte 1: [3:0]window color
    //      byte 2: [4]border/shadow/3d enable [3:1]window type [0]window enable
    {0x00, WINDOW_COLOR(_CP_PASTEL_CYAN_BLUE), _BLENDING_ENABLE | BORDER_ENABLE(1) | _3DBUTTON_TYPE1 | WINDOW_ENABLE(1)}
};

WindowStyle code tSource_Info_WindowStyle =
{
    0,
    {0, _V_DELTA+0, (27 * 12) + 20, _V_DELTA+(7 * 18 - 4)},
    // ctrl byte 0: [7:6]Multiple Factor of Gradient Step [5:3]border width [2:0]border height,
    //      byte 1: [7:4]left/top color [3:0]right/bottom color,
    //      byte 2: Gradient Ctrl [7]R Polarity [6]G polarity [5]B polarity [4:3]step pixels [2]enable Red [1]enable Green [0]enable Blue
    {(1 << 3) | 1, (1 << 4) | 1, _GRADIENT_LEVEL(3) |_R_GRADIENT_ENABLE|_G_GRADIENT_ENABLE|_B_GRADIENT_ENABLE},
    // ctrl byte 0:
    //      byte 1: [3:0]window color
    //      byte 2: [4]border/shadow/3d enable [3:1]window type [0]window enable
    {0x00, _LEVEL_PER_GRADIENT(0)|WINDOW_COLOR(2),_BLENDING_ENABLE|_GRADIENT_ENABLE|_GRADIENT_V | BORDER_ENABLE(1) | _3DBUTTON_TYPE1 | WINDOW_ENABLE(1)}
};

ItemPosition code tModeInfoHintWindowPos =
{
    _H_DELTA+10, _V_DELTA+34+(24*2)+2, _H_DELTA+60+(12*16), _V_DELTA+34+(24*2)+32
};


#define _CHANNEL_EDIT_WINDOW_POS_

ItemPosition code tChannel_Edit_TitleWindowPos =
{
    _H_DELTA+10, _V_DELTA+11, _H_DELTA+60+(12*20), _V_DELTA+39
};

ItemPosition code tChannel_Edit_WindowPos =
{
    _H_DELTA+10, _V_DELTA+34, _H_DELTA+60+(12*20), _V_DELTA+34+(18*13)+16
};

ItemPosition code tChannel_Edit_HintWindowPos =
{
    _H_DELTA+10, _V_DELTA+34+(18*13)+14, _H_DELTA+60+(12*20), _V_DELTA+34+(18*16)+10
};

ItemPosition code tChannel_Edit_Highlight_WindowPos =
{
    _H_DELTA+10+4, _V_DELTA+34+45, _H_DELTA+60+(12*20)-4, _V_DELTA+34+45+24
};

ItemPosition code tChannel_Edit_Name_Highlight_WindowPos =
{
    _H_DELTA+10+2+12*7, _V_DELTA+34+46, _H_DELTA+10+2+12*8+4, _V_DELTA+34+46+22
};

#define _RTK_FACTORY_MODE_WINDOW_POS_

ItemPosition code tFactory_mode_Title_WindowPos =
{
    _H_DELTA+10, _V_DELTA+11, _H_DELTA+60+(12*30), _V_DELTA+34
};

ItemPosition code tFactory_mode_WindowPos =
{
    _H_DELTA+10, _V_DELTA+34, _H_DELTA+60+(12*30), _V_DELTA+34+(24*14)+10
};

ItemPosition code tFactory_mode_Hint_WindowPos =
{
    _H_DELTA+10, _V_DELTA+34+(24*14)+8, _H_DELTA+60+(12*30), _V_DELTA+34+(24*15)+12
};

ItemPosition code tFactory_mode_Highlight_WindowPos =
{
    _H_DELTA+10+4, _V_DELTA+34+16, _H_DELTA+60+(12*30)-4, _V_DELTA+34+16+24
};

#define _FACTORY_MODE_WINDOW_STYLE_

WindowStyle code tFac_Title0_WindowStyle =
{
    2,
    {_H_DELTA+12, _V_DELTA+10, _H_DELTA+(12*39)-4, _V_DELTA+40},
    // ctrl byte 0: [5:3]border width [2:0]border height,
    //      byte 1: [7:4]left/top color [3:0]right/bottom color,
    //      byte 2:
    {(1 << 3) | 0, (_CP_PURE_CYAN_BLUE << 4) | _CP_WHITE, 0x00},
    // ctrl byte 0:
    //      byte 1: [3:0]window color
    //      byte 2: [4]border/shadow/3d enable [3:1]window type [0]window enable
    {0x00, WINDOW_COLOR(_CP_DARKER_CYAN_BLUE), BORDER_ENABLE(1) | _BORDER | WINDOW_ENABLE(1)}
};

WindowStyle code tFac_Title1_Window_Style =
{
    0,
    {_H_DELTA+(12*19)-4, _V_DELTA+10, _H_DELTA+(12*39), _V_DELTA+40},
    // ctrl byte 0: [7:6]Multiple Factor of Gradient Step [5:3]border width [2:0]border height,
    //      byte 1: [7:4]left/top color [3:0]right/bottom color,
    //      byte 2: Gradient Ctrl [7]R Polarity [6]G polarity [5]B polarity [4:3]step pixels [2]enable Red [1]enable Green [0]enable Blue
    { (0x03 << 6) | (1 << 3) | 0, (_CP_PURE_CYAN_BLUE << 4) | _CP_WHITE, (0x03 << 3) | 0x01},
    // ctrl byte 0:
    //      byte 1: [7]gradient type [6:4]levle size [3:0]window color
    //      byte 2: [6]gradient enable [5]gradient H/V [4]border/shadow/3d enable [3:1]window type [0]window enable
    {0x00, (0x01 << 4) | _CP_DARKER_CYAN_BLUE, (0x00 << 6) | (0x01 << 5) | BORDER_ENABLE(1) | _BORDER | WINDOW_ENABLE(1)}
};

WindowStyle code tFac_Mainmenu_WindowStyle =
{
    3,
    {_H_DELTA+12, _V_DELTA+40, _H_DELTA+(12*39)-4, _V_DELTA+40+(18*12)},
    // ctrl byte 0: [5:3]border width [2:0]border height
    //      byte 1: [7:4]left/top color [3:0]right/bottom color
    //      byte 2:
    {(1 << 3) | 0, (_CP_PURE_CYAN_BLUE<< 4) | _CP_WHITE, 0x00},
    // ctrl byte 0:
    //      byte 1: [3:0]window color
    //      byte 2: [4]border/shadow/3d enable [3:1]window type [0]window enable
    {0x00, WINDOW_COLOR(_CP_DARKER_CYAN_BLUE), BORDER_ENABLE(1) | _BORDER | WINDOW_ENABLE(1)}
};

WindowStyle code tFac_Submenu_WindowStyle =
{
    4,
    {_H_DELTA+(12*19)-2, _V_DELTA+44, _H_DELTA+(12*47)+4, _V_DELTA+40+(24*12)},
    // ctrl byte 0: [5:3]border width [2:0]border height,
    //      byte 1: [7:4]left/top color [3:0]right/bottom color,
    //      byte 2:
    { (1 << 3) | 0, (_CP_PURE_CYAN_BLUE << 4) | _CP_WHITE, 0x00},
    // ctrl byte 0:
    //      byte 1: [3:0]window color
    //      byte 2: [4]border/shadow/3d enable [3:1]window type [0]window enable
    {0x00, WINDOW_COLOR(_CP_DARKER_CYAN_BLUE), BORDER_ENABLE(1) | _BORDER | WINDOW_ENABLE(1)}
};

#define _MUTE_ICON_BACKGROUND_WINDOW
ItemPosition code tMuteIcon_BG_WindowPos =
{
    _H_DELTA+10, _V_DELTA+10, _H_DELTA+10+48, _V_DELTA+10+48
};



//======================================================
// ** Row map 0, for Main Menu
UINT8 code tRowLength_LUT0[] =
{
    18,         // Row Count
    1,    // Row 0 char count
    30,   // Row 1 char count
    30,   // Row 2 char count
    39,   // Row 3 char count
    46,   // Row 4 char count
    39,   // Row 5 char count
    46,   // Row 6 char count
    39,   // Row 7 char count
    46,   // Row 8 char count
    39,   // Row 9
    46,   // Row 10
    39,   // Row 11
    46,   // Row 12
    39,   // 13
    46,   // 14
    39,   // 15
    39,   // 16
    50//39
};

UINT8 code tOsdRowCommand0[] =
{
    FONTBYTE0(0x000),
    0x80, _REPEAT_, 18,
    _NEXT_,

    FONTBYTE1(0x000),
    ROW_HEIGHT(18), _REPEAT_, 18,
    _NEXT_,

    FONTBYTE2(0x000),
    ROW_LENGTH(1),
    ROW_LENGTH(30),_REPEAT_, 2,
    ROW_LENGTH(39),
    ROW_LENGTH(46),
    ROW_LENGTH(39),
    ROW_LENGTH(46),
    ROW_LENGTH(39),
    ROW_LENGTH(46),
    ROW_LENGTH(39),
    ROW_LENGTH(46),
    ROW_LENGTH(39),
    ROW_LENGTH(46),
    ROW_LENGTH(39),
    ROW_LENGTH(46),
    ROW_LENGTH(39), _REPEAT_, 2,// ROW_LENGTH(39), _REPEAT_, 3,
    ROW_LENGTH(50), 
    _NEXT_,

    FONTBYTE0(18),	// 21 rows
    0x00,
    _NEXT_,
    _END_
};

//======================================================
// ** Row map 1, for Source Select
UINT8 code tRowLength_LUT1[] =
{
    3, // total row
    16,	// title bar
    16,
    16,
    0	// end symbol
};

UINT8 code tOsdRowCommand1[] =
{
    FONTBYTE0(0x000),
    0x80, _REPEAT_, 3,
    _NEXT_,

    FONTBYTE1(0x000),
    ROW_HEIGHT(18), _REPEAT_, 3,
    _NEXT_,

    FONTBYTE2(0x000),
    ROW_LENGTH(16), _REPEAT_, 3,
    _NEXT_,

    FONTBYTE0(3),	// 21 rows
    0x00,
    _NEXT_,
    _END_
};



UINT8* code tRowLength_LUT_Set[] =
{
    tRowLength_LUT0,
    tRowLength_LUT1,
};

UINT8* code tOsdRowCmdSet[] =
{
    tOsdRowCommand0,
    tOsdRowCommand1,
};


UINT8 code tOsd_Palette[] =
{
    _COLOR_0,
    _COLOR_1,
    _COLOR_2,
    _COLOR_3,
    _COLOR_4,
    _COLOR_5,
    _COLOR_6,
    _COLOR_7,
    _COLOR_8,
    _COLOR_9,
    _COLOR_10,
    _COLOR_11,
    _COLOR_12,
    _COLOR_13,
    _COLOR_14,
    _COLOR_15,
    // ** Palette LUT 0
    _RGB_BLACK,
    _RGB_WHITE,
    _RGB_PASTEL_CYAN_BLUE, // for Main menu
    _RGB_DARK_CYAN_BLUE,  // for Main menu
    _RGB_PURE_YELLOW_ORANGE, // for Main menu
    _RGB_DARKER_CYAN_BLUE, // for Main menu
    _RGB_GRAY, //_RGB_LIGHT_BLUE,
    _RGB_SILVER,

    _RGB_LIGHT_MAGENTA,	//_RGB_GRAY,
    _RGB_GREEN,
    _RGB_BLUE,
    _RGB_YELLOW,
    _RGB_MAGENTA,
    _RGB_PURE_RED,
    _RGB_DARK_YELLOW_ORANGE, // for Main menu
    _RGB_PURE_CYAN_BLUE,

    // ** Palette LUT 1
    _RGB_BLACK,		//--0
    _RGB_WHITE,		//--1
    _RGB_PASTEL_CYAN_BLUE, 		//--2
    _RGB_DARK_CYAN_BLUE, 		//--3
    _RGB_PURE_YELLOW_ORANGE,	//--4
    _RGB_DARKER_CYAN_BLUE,	 	//--5
    _RGB_LIGHT_BLUE,			//--6
    _RGB_SILVER,				//--7

    _RGB_LIGHT_MAGENTA,		//--8
    _RGB_GREEN,				//--9
    _RGB_BLUE,			//--a
    _RGB_YELLOW_04,			//-- 11
    _RGB_YELLOW_03,			//-- 12
    _RGB_YELLOW_02,			//-- 13
    _RGB_YELLOW_01,			//-- 14
    _RGB_PURE_CYAN_BLUE, 	//-- 15

    // ** Palette LUT 2
};


//#ifdef CONFIG_VBI_CC_SUPPORT
#define _VBIRGB_BLACK                  0x00,0x00,0x00
#define _VBIRGB_GRAY_	  		0x13,0x13,0x13
#define _VBIRGB_WHITE_	  		0xFF,0xFF,0xFF
#define _VBIRGB_GREEN_	  		0x00,0xFF,0x00
#define _VBIRGB_BLUE_	  		0x00,0x00,0xFF
#define _VBIRGB_PURE_CYAN_	  	0x00,0xAE,0xEF//00,174,239
#define _VBIRGB_RED_			0xFF,0x00,0x00
#define _VBIRGB_YELLOW_		0xFF,0xFF,0x00
#define _VBIRGB_MAGENTA_		0xFF,0x00,0xFF
UINT8 code Palette_VBI[] =
{
    _VBIRGB_BLACK/*_VBIRGB_GRAY_*/,   	//-0 Background Color
    _VBIRGB_WHITE_,	    		//-1
    _VBIRGB_GREEN_,    		//-2
    _VBIRGB_BLUE_,	   		//-3
    _VBIRGB_PURE_CYAN_, 	//-4
    _VBIRGB_RED_,	        	//-5
    _VBIRGB_YELLOW_,       	//-6
    _VBIRGB_MAGENTA_,	 	//-7
    _VBIRGB_GRAY_,      		//-8 Foreground Color
    _VBIRGB_WHITE_,			//-9
    _VBIRGB_GREEN_,			//-10
    _VBIRGB_BLUE_,			//-11
    _VBIRGB_PURE_CYAN_,	//-12
    _VBIRGB_RED_,			//-13
    _VBIRGB_YELLOW_,		//-14
    _VBIRGB_MAGENTA_,		//-15
};
//#endif



/*=================== Local Functions Phototype ==============*/

/*=========================== Functions ===================== */


/**
 *
 * @}
 */

