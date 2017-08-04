/*=============================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2005
  * All rights reserved.
  * ============================================ */

/*================= File Description ================= */
/**
 * @file
 * 	This file is for customize configurations.
 *
 * @author 	$Author: kingee $
 * @date 	$Date: 2006-09-11 19:13:22 +0800 (?üÊ?‰∏Ä, 11 ‰πùÊ? 2006) $
 * @version 	$Revision: 1191 $
 * @ingroup 	vbi
 */

/**
* @addtogroup vbi
* @{
*/
#define _CLOSECAPTION_C
/*======================== MODULE DEPENDENCY =================================*/
#include "sparrow.h"
#include "rosprintf.h"
#include "message.h"
#include "timer_event_ui_table.h"
#include "osdregdef.h"
#include "osdobjectdef.h"
#include "osdfunction.h"
#include "osdparameter.h"
#include "osdcontrol.h"
#include "reg52.h"
#include "timer_event_sys_table.h"
#include "autoconf.h"
#include "source.h"
#include "osdvbi.h"
#include "closecaption.h"
#include "flow_struct.h"
#include "osdcore.h"
#include "timer_event.h"
#include "modestate.h"

#ifdef CONFIG_VBI_CC_SUPPORT
#include <vbi/slr.h>

//leo 20060430
#ifdef CC_AUTO_TEST
#define CC1_table_max 	90
static UINT8 code CC1_table[CC1_table_max][2] =
{
    {0x00,   0x00},
    {0x14,   0x2c},    //EDM => Channel 1, Field 1: Erase Display Memory
    {0x14,   0x2d},    //CR => Channel 1, Field 1: Carriage
    {0x14,   0x2e},    //ENM => Channel 1, Field 1: Erase Non-Display Memory
    {0x14,   0x2f},    //EOC => Channel 1, Field 1: End of Caption
    {0x14,   0x27},    //RU4 => Channel 1, Field 1: Roll-Up Captions-4 Rows
    {0x13,   0x46},    //PAC => Channel 1, Underline 0, Row Pos 12: Cyan
    {0x10,   0x24},    //Optional Attribute => Channel 1, Transparent 0 : Background-Blue
    {0x20,   0x41},    //    Character:   A
    {0x42,   0x43},    //    Character: B C
    {0x44,   0x45},    //    Character: D E
    {0x46,   0x47},    //    Character: F G
    {0x48,   0x49},    //    Character: H I
    {0x4a,   0x4b},    //    Character: J K
    {0x4c,   0x4d},    //    Character: L M
    {0x4e,   0x4f},    //    Character: N O
    {0x50,   0x51},    //    Character: P Q
    {0x52,   0x53},    //    Character: R S
    {0x54,   0x55},    //    Character: T U
    {0x56,   0x57},    //    Character: V W
    {0x58,   0x59},    //    Character: X Y
    {0x5a,   0x20},    //    Character: Z
    {0x13,   0x4e},    //PAC => Channel 1, Underline 0, Row Pos 12: Italics
    {0x10,   0x25},    //Optional Attribute => Channel 1, Transparent 1 : Background-Blue
    {0x20,   0x61},    //    Character:   a
    {0x62,   0x63},   //    Character: b c
    {0x64,   0x65},   //    Character: d e
    {0x66,   0x67},    //    Character: f g
    {0x68,   0x69},    //    Character: h i
    {0x6a,   0x6b},    //    Character: j k
    {0x6c,   0x6d},   //    Character: l m
    {0x6e,   0x6f },   //    Character: n o
    {0x70,   0x71},   //    Character: p q
    {0x72,   0x73},   //    Character: r s
    {0x74,   0x75},   //    Character: t u
    {0x76,   0x77},    //    Character: v w
    {0x78,   0x79},    //    Character: x y
    {0x7a,   0x20},    //    Character: z
    {0x13,   0x45},   //PAC => Channel 1, Underline 1, Row Pos 12: Blue
    {0x10,   0x20},    //Optional Attribute => Channel 1, Transparent 0 : Background-White
    {0x20,   0x21},    //    Character:   !
    {0x2c,   0x2e},    //    Character: , .
    {0x3b,   0x3a},    //    Character: ; :
    {0x27,   0x22},    //    Character: ' "
    {0x23,   0x25},    //    Character: # %
    {0x26,   0x40},    //    Character: & @
    {0x2f,    0x28},    //    Character: / (
    {0x29,   0x5b},    //    Character: ) [
    {0x5d,   0x2b},    //    Character: ] +
    {0x2d,   0x7c},    //    Character: - ?
    {0x3c,   0x3d},    //    Character: < =
    {0x3e,   0x3f },   //    Character: > ?
    {0x11,   0x35},    //Special Character => Channel 1, S_character 5
    {0x24,   0x00},    //    Character: $
    {0x11,   0x36},    //Special Character => Channel 1, S_character 6
    {0x20,   0x00},   //    Character:
    {0x13,   0x48},   //PAC => Channel 1, Underline 0, Row Pos 12: Red
    {0x10,   0x21},   //Optional Attribute => Channel 1, Transparent 1 : Background-White
    {0x20,   0x2a},    //    Character:   ?
    {0x11,   0x38},   //Special Character => Channel 1, S_character 8
    {0x11,   0x3b},   //Special Character => Channel 1, S_character 11
    {0x7b,   0x5c},    //    Character: ??
    {0x11,   0x3a},   //Special Character => Channel 1, S_character 10
    {0x11,   0x3c},   //Special Character => Channel 1, S_character 12
    {0x5e,   0x00},   //    Character: ?
    {0x11,   0x3d},    //Special Character => Channel 1, S_character 13
    {0x7d,   0x7e},   //    Character: ??
    {0x11,   0x3e},   //Special Character => Channel 1, S_character 14
    {0x5f,    0x00},   //    Character: ?
    {0x11,   0x3f },    //Special Character => Channel 1, S_character 15
    {0x60,   0x20},   //    Character: ?
    {0x11,   0x30},   //Special Character => Channel 1, S_character 0
    {0x11,   0x31},   //Special Character => Channel 1, S_character 1
    {0x11,   0x34},   //Special Character => Channel 1, S_character 4
    {0x11,   0x32},   //Special Character => Channel 1, S_character 2
    {0x11,   0x33},   //Special Character => Channel 1, S_character 3
    {0x11,   0x37},   //Special Character => Channel 1, S_character 7
    {0x11,   0x39},   //Special Character => Channel 1, S_character 9
    {0x20,   0x7f },    //    Character:   ?
    {0x13,   0x4c},    //PAC => Channel 1, Underline 0, Row Pos 12: Magenta
    {0x10,   0x28},   //Optional Attribute => Channel 1, Transparent 0 : Background-Red
    {0x17,   0x2e},    //Optional Attribute => Channel 1, Foreground-Black
    {0x17,   0x25},   //Closed Group Extensions => Channel 1, double size
    {0x20,   0x20},   //    Character:
    {0x20,   0x30},   //    Character:   0
    {0x31,   0x32},    //    Character: 1 2
    {0x33,   0x34},   //    Character: 3 4
    {0x35,   0x36},    //    Character: 5 6
    {0x37,   0x38},   //    Character: 7 8
    {0x39,   0x20}	  //    Character: 9
};

#define CC2_table_max 	118
static UINT8 code CC2_table[CC2_table_max][2] =
{
    {0x00,   0x00},
    {0x44,   0x61},    //    Character: D a
    {0x6e,   0x69},    //    Character: n i
    {0x73,   0x68},    //    Character: s h
    {0x20,   0x20},    //    Character:
    {0x3a,   0x20},    //    Character: :
    {0x41,   0x00},    //    Character: A
    {0x61,   0x00},    //    Character: a
    {0x4f,   0x00},    //    Character: O
    {0x6f,   0x00},    //    Character: o
    {0x2b,   0x00},    //    Character: +
    {0x53,   0x70},    //    Character: S p
    {0x61,   0x6e},    //    Character: a n
    {0x69,   0x73},    //    Character: i s
    {0x68,   0x20},    //    Character: h
    {0x45,   0x00},    //    Character: E
    {0x55,   0x00},    //    Character: U
    {0x75,   0x00},    //    Character: u
    {0x27,   0x00},    //    Character: '
    {0x69,   0x00},    //    Character: i
    {0x27,   0x00},    //    Character: '
    {0x2d,   0x00},    //    Character: -
    {0x63,   0x00},    //    Character: c
    {0x53,   0x00},    //    Character: S
    {0x22,   0x00},    //    Character: "
    {0x46,   0x72},     //    Character: F r
    {0x65,   0x6e},     //    Character: e n
    {0x63,   0x68},     //    Character: c h
    {0x43,   0x00},    //    Character: C
    {0x65,   0x00},    //    Character: e
    {0x49,   0x00},    //    Character: I
    {0x50,   0x6f},     //    Character: P o
    {0x72,   0x74},     //    Character: r t
    {0x75,   0x67},     //    Character: u g
    {0x75,   0x65},     //    Character: u e
    {0x73,   0x65},     //    Character: s e
    {0x28,   0x00},    //    Character: (
    {0x29,   0x00},    //    Character: )
    {0x2d,   0x00},    //    Character: -
    {0x2f,   0x00},    //    Character: /
    {0x47,   0x65},     //    Character: G e
    {0x72,   0x6d},     //    Character: r m
    {0x73,   0x00},    //    Character: s
    {0x59,   0x00},    //    Character: Y
    {0x2f,   0x00},    //    Character: /
    {0x1c,   0x2c},     //EDM => Channel 2, Field 1: Erase Display Memory
    {0x1c,   0x2e},     //ENM => Channel 2, Field 1: Erase Non-Display Memory
    {0x1c,   0x2f},     //EOC => Channel 2, Field 1: End of Caption
    {0x1c,   0x29},     //RDC => Channel 2, Field 1: Resume Direct Captioning
    {0x1d,   0x6a},     //PAC => Channel 2, Underline 0, Row Pos 6: Yellow
    {0x1e,   0x42},     //PAC => Channel 2, Underline 0, Row Pos 7: Green
    {0x1e,   0x64},     //PAC => Channel 2, Underline 0, Row Pos 8: Blue
    {0x1f,   0x48},     //PAC => Channel 2, Underline 0, Row Pos 9: Red
    {0x1f,   0x60},     //PAC => Channel 2, Underline 0, Row Pos 10: White

    {0x1a,   0x20},     //Extended Character Set=> Channel 2, E_character 32
    {0x1a,   0x21},     //Extended Character Set=> Channel 2, E_character 33
    {0x1a,   0x22},     //Extended Character Set=> Channel 2, E_character 34
    {0x1a,   0x23},     //Extended Character Set=> Channel 2, E_character 35
    {0x1a,   0x24},     //Extended Character Set=> Channel 2, E_character 36
    {0x1a,   0x25},     //Extended Character Set=> Channel 2, E_character 37
    {0x1a,   0x26},     //Extended Character Set=> Channel 2, E_character 38
    {0x1a,   0x27},     //Extended Character Set=> Channel 2, E_character 39
    {0x1a,   0x28},     //Extended Character Set=> Channel 2, E_character 40
    {0x1a,   0x29},     //Extended Character Set=> Channel 2, E_character 41
    {0x1a,   0x2a},     //Extended Character Set=> Channel 2, E_character 42
    {0x1a,   0x2b},     //Extended Character Set=> Channel 2, E_character 43
    {0x1a,   0x2c},     //Extended Character Set=> Channel 2, E_character 44
    {0x1a,   0x2d},     //Extended Character Set=> Channel 2, E_character 45
    {0x1a,   0x2e},     //Extended Character Set=> Channel 2, E_character 46
    {0x1a,   0x2f},     //Extended Character Set=> Channel 2, E_character 47
    {0x1a,   0x30},     //Extended Character Set=> Channel 2, E_character 48
    {0x1a,   0x31},     //Extended Character Set=> Channel 2, E_character 49
    {0x1a,   0x32},     //Extended Character Set=> Channel 2, E_character 50
    {0x1a,   0x33},     //Extended Character Set=> Channel 2, E_character 51
    {0x1a,   0x34},     //Extended Character Set=> Channel 2, E_character 52
    {0x1a,   0x35},     //Extended Character Set=> Channel 2, E_character 53
    {0x1a,   0x36},     //Extended Character Set=> Channel 2, E_character 54
    {0x1a,   0x37},     //Extended Character Set=> Channel 2, E_character 55
    {0x1a,   0x38},     //Extended Character Set=> Channel 2, E_character 56
    {0x1a,   0x39},     //Extended Character Set=> Channel 2, E_character 57
    {0x1a,   0x3a},     //Extended Character Set=> Channel 2, E_character 58
    {0x1a,   0x3b},     //Extended Character Set=> Channel 2, E_character 59
    {0x1a,   0x3c},     //Extended Character Set=> Channel 2, E_character 60
    {0x1a,   0x3d},     //Extended Character Set=> Channel 2, E_character 61
    {0x1a,   0x3e},     //Extended Character Set=> Channel 2, E_character 62
    {0x1a,   0x3f },    //Extended Character Set=> Channel 2, E_character 63

    {0x1b,   0x20},     //Extended Character Set=> Channel 2, E_character 32
    {0x1b,   0x21},     //Extended Character Set=> Channel 2, E_character 33
    {0x1b,   0x22},     //Extended Character Set=> Channel 2, E_character 34
    {0x1b,   0x23},     //Extended Character Set=> Channel 2, E_character 35
    {0x1b,   0x24},     //Extended Character Set=> Channel 2, E_character 36
    {0x1b,   0x25},     //Extended Character Set=> Channel 2, E_character 37
    {0x1b,   0x26},     //Extended Character Set=> Channel 2, E_character 38
    {0x1b,   0x27},     //Extended Character Set=> Channel 2, E_character 39
    {0x1b,   0x28},     //Extended Character Set=> Channel 2, E_character 40
    {0x1b,   0x29},     //Extended Character Set=> Channel 2, E_character 41
    {0x1b,   0x2a},     //Extended Character Set=> Channel 2, E_character 42
    {0x1b,   0x2b},     //Extended Character Set=> Channel 2, E_character 43
    {0x1b,   0x2c},     //Extended Character Set=> Channel 2, E_character 44
    {0x1b,   0x2d},     //Extended Character Set=> Channel 2, E_character 45
    {0x1b,   0x2e},     //Extended Character Set=> Channel 2, E_character 46
    {0x1b,   0x2f },    //Extended Character Set=> Channel 2, E_character 47
    {0x1b,   0x30},     //Extended Character Set=> Channel 2, E_character 48
    {0x1b,   0x31},     //Extended Character Set=> Channel 2, E_character 49
    {0x1b,   0x32},     //Extended Character Set=> Channel 2, E_character 50
    {0x1b,   0x33 },    //Extended Character Set=> Channel 2, E_character 51
    {0x1b,   0x34},     //Extended Character Set=> Channel 2, E_character 52
    {0x1b,   0x35 },    //Extended Character Set=> Channel 2, E_character 53
    {0x1b,   0x36},     //Extended Character Set=> Channel 2, E_character 54
    {0x1b,   0x37},     //Extended Character Set=> Channel 2, E_character 55
    {0x1b,   0x38},     //Extended Character Set=> Channel 2, E_character 56
    {0x1b,   0x39 },    //Extended Character Set=> Channel 2, E_character 57
    {0x1b,   0x3a },    //Extended Character Set=> Channel 2, E_character 58
    {0x1b,   0x3b },    //Extended Character Set=> Channel 2, E_character 59
    {0x1b,   0x3c },    //Extended Character Set=> Channel 2, E_character 60
    {0x1b,   0x3d },    //Extended Character Set=> Channel 2, E_character 61
    {0x1b,   0x3e },    //Extended Character Set=> Channel 2, E_character 62
    {0x1b,   0x3f  }     //Extended Character Set=> Channel 2, E_character 63
};

//leo 20060509
#define CC3_table_max 	23
static UINT8 code CC3_table[CC3_table_max][2] =
{
    {0x01,   0x05},     //XDS => Control: Current-Start, Type: Program Rating
    {0x41,   0x40},     //    Character: A @
//{0x0f,   0x6a},
    {0x00,   0x00},     //    Character:
    {0x15,   0x2c},     //EDM => Channel 1, Field 2: Erase Display Memory
    {0x15,   0x2e},     //ENM => Channel 1, Field 2: Erase Non-Display Memory
    {0x15,   0x20},     //RCL => Channel 1, Field 2: Resume Caption Loading
    {0x16,   0x46},     //PAC => Channel 1, Underline 0, Row Pos 7: Cyan
    {0x20,   0x20},     //    Character:
    {0x10,   0x22},     //Optional Attribute => Channel 1, Transparent 0 : Background-Green
    {0x43,   0x6c},     //    Character: C l
    {0x6f,   0x73},     //    Character: o s
    {0x65,   0x64},     //    Character: e d
    {0x20,   0x43},     //    Character:   C
    {0x61,   0x70},     //    Character: a p
    {0x74,   0x69},     //    Character: t i
    {0x6f,   0x6e},     //    Character: o n
    {0x20,   0x43},     //    Character:   C
    {0x43,   0x33},     //    Character: C 3
    {0x20,   0x6d},     //    Character:   m
    {0x6f,   0x64},     //    Character: o d
    {0x65,   0x2e},     //    Character: e .
    {0x20,   0x00},     //    Character:
    {0x15,   0x2f}    //EOC => Channel 1, Field 2: End of Caption
};

#define CC4_table_max 	23
static UINT8 code CC4_table[CC4_table_max][2] =
{
    {0x01,   0x5},     //XDS => Control: Current-Start, Type: Program Rating
    {0x46,   0x40},    //    Character: F @
//{0x0f,   0x65},
    {0x00,   0x00},    //    Character:
    {0x1d,   0x2c},    //EDM => Channel 2, Field 2: Erase Display Memory
    {0x1d,   0x2e},    //ENM => Channel 2, Field 2: Erase Non-Display Memory
    {0x1d,   0x29},    //RDC => Channel 2, Field 2: Resume Direct Captioning
    {0x1e,   0x48},    //PAC => Channel 2, Underline 0, Row Pos 7: Red
    {0x20,   0x20},    //    Character:
    {0x18,   0x23},    //Optional Attribute => Channel 2, Transparent 1 : Background-Green
    {0x1f,   0x2e},    //Optional Attribute => Channel 2, Foreground-Black
    {0x43,   0x6c},    //    Character: C l
    {0x6f,   0x73},    //    Character: o s
    {0x65,   0x64},    //    Character: e d
    {0x20,   0x43},    //    Character:   C
    {0x61,   0x70},    //    Character: a p
    {0x74,   0x69},    //    Character: t i
    {0x6f,   0x6e},    //    Character: o n
    {0x20,   0x43},    //    Character:   C
    {0x43,   0x34},    //    Character: C 4
    {0x20,   0x6d},    //    Character:   m
    {0x6f,   0x64},    //    Character: o d
    {0x65,   0x2e},    //    Character: e .
    {0x20,   0x00}     //    Character:
};

#define T1_table_max	166
static UINT8 code T1_table[T1_table_max][2] =
{
    {0x14,   0x2b},    //RTD => Channel 1, Field 1: Resume Text Display
    {0x11,   0x2a},    //Midrow Code => Channel 1, Underline 0: Yellow
    {0x14,   0x2e},    //ENM => Channel 1, Field 1: Erase Non-Display Memory
    {0x14,   0x2f},    //EOC => Channel 1, Field 1: End of Caption
    {0x14,   0x2c},    //EDM => Channel 1, Field 1: Erase Display Memory
    {0x1c,   0x2c},    //EDM => Channel 2, Field 1: Erase Display Memory
    {0x1c,   0x2e},    //ENM => Channel 2, Field 1: Erase Non-Display Memory
    {0x1c,   0x2f},    //EOC => Channel 2, Field 1: End of Caption
    {0x3a,   0x00},    //    Character: :
    {0x00,   0x00},    //    Character:
    {0x50,   0x72},    //    Character: P r
    {0x69,   0x6d},    //    Character: i m
    {0x61,   0x72},    //    Character: a r
    {0x79,   0x20},    //    Character: y
    {0x54,   0x65},    //    Character: T e
    {0x78,   0x74},    //    Character: x t
    {0x20,   0x4d},    //    Character:   M
    {0x6f,   0x64},    //    Character: o d
    {0x65,   0x20},    //    Character: e
    {0x53,   0x65},    //    Character: S e
    {0x72,   0x76},    //    Character: r v
    {0x69,   0x63},    //    Character: i c
    {0x65,   0x00},    //    Character: e
    {0x41,   0x20},    //    Character: A
    {0x6d,   0x61},    //    Character: m a
    {0x62,   0x65},    //    Character: b e
    {0x20,   0x75},    //    Character:   u
    {0x73,   0x65},    //    Character: s e
    {0x64,   0x20},    //    Character: d
    {0x74,   0x68},    //    Character: t h
    {0x61,   0x74},    //    Character: a t
    {0x20,   0x0},    //    Character:
    {0x63,   0x6f},    //    Character: c o
    {0x6e,   0x73},    //    Character: n s
    {0x69,   0x73},    //    Character: i s
    {0x74,   0x73},    //    Character: t s
    {0x20,   0x6f},    //    Character:   o
    {0x66,   0x20},    //    Character: f
    {0x64,   0x61},    //    Character: d a
    {0x74,   0x61},    //    Character: t a
    {0x20,   0x66},    //    Character:   f
    {0x6f,   0x72},    //    Character: o r
    {0x6d,   0x61},    //    Character: m a
    {0x74,   0x74},    //    Character: t t
    {0x65,   0x64},    //    Character: e d
    {0x20,   0x74},    //    Character:   t
    {0x6f,   0x00},    //    Character: o
    {0x66,   0x69},    //    Character: f i
    {0x6c,   0x6c},    //    Character: l l
    {0x20,   0x62},    //    Character:   b
    {0x6f,   0x78},    //    Character: o x
    {0x20,   0x77},    //    Character:   w
    {0x68,   0x69},    //    Character: h i
    {0x63,   0x68},    //    Character: c h
    {0x20,   0x69},    //    Character:   i
    {0x6e,   0x20},    //    Character: n
    {0x68,   0x65},    //    Character: h e
    {0x69,   0x67},    //    Character: i g
    {0x68,   0x74},    //    Character: h t
    {0x73,   0x00},    //    Character: s
    {0x6e,   0x6f},    //    Character: n o
    {0x74,   0x20},    //    Character: t
    {0x6c,   0x65},    //    Character: l e
    {0x73,   0x73},    //    Character: s s
    {0x20,   0x74},    //    Character:   t
    {0x68,   0x61},    //    Character: h a
    {0x6e,   0x20},    //    Character: n
    {0x37,   0x20},    //    Character: 7
    {0x72,   0x6f},    //    Character: r o
    {0x77,   0x73},    //    Character: w s
    {0x6e,   0x64},    //    Character: n d
    {0x20,   0x6e},    //    Character:   n
    {0x6f,   0x74},    //    Character: o t
    {0x6d,   0x6f},    //    Character: m o
    {0x72,   0x65},    //    Character: r e
    {0x31,   0x35},    //    Character: 1 5
    {0x20,   0x72},    //    Character:   r
    {0x6f,   0x77},    //    Character: o w
    {0x73,   0x20},    //    Character: s
    {0x28,   0x61},    //    Character: ( a
    {0x6c,   0x6c},    //    Character: l l
    {0x20,   0x6f},    //    Character:   o
    {0x66,   0x20},    //    Character: f
    {0x77,   0x68},    //    Character: w h
    {0x69,   0x63},    //    Character: i c
    {0x68,   0x00},    //    Character: h
    {0x73,   0x68},    //    Character: s h
    {0x6f,   0x75},    //    Character: o u
    {0x6c,   0x64},    //    Character: l d
    {0x65,   0x20},    //    Character: e
    {0x63,   0x6f},    //    Character: c o
    {0x6e,   0x74},    //    Character: n t
    {0x69,   0x67},    //    Character: i g
    {0x75,   0x6f},    //    Character: u o
    {0x75,   0x73},    //    Character: u s
    {0x29,   0x2c},    //    Character: ) ,
    {0x20,   0x61},    //    Character:   a
    {0x6e,   0x64},    //    Character: n d
    {0x20,   0x69},    //    Character:   i
    {0x6e,   0x00},    //    Character: n
    {0x77,   0x69},    //    Character: w i
    {0x64,   0x74},    //    Character: d t
    {0x68,   0x20},    //    Character: h
    {0x69,   0x73},    //    Character: i s
    {0x20,   0x6e},    //    Character:   n
    {0x6f,   0x74},    //    Character: o t
    {0x20,   0x6c},    //    Character:   l
    {0x65,   0x73},    //    Character: e s
    {0x73,   0x20},    //    Character: s
    {0x74,   0x68},    //    Character: t h
    {0x65,   0x6e},    //    Character: e n
    {0x20,   0x33},    //    Character:   3
    {0x32,   0x20},    //    Character: 2
    {0x63,   0x6f},    //    Character: c o
    {0x6c,   0x2d},    //    Character: l -
    {0x14,   0x2d},   //CR => Channel 1, Field 1: Carriage
    {0x75,   0x6d},    //    Character: u m
    {0x6e,   0x73},    //    Character: n s
    {0x2e,   0x20},    //    Character: .
    {0x54,   0x65},    //    Character: T e
    {0x78,   0x74},    //    Character: x t
    {0x20,   0x73},    //    Character:   s
    {0x68,   0x6f},    //    Character: h o
    {0x75,   0x6c},    //    Character: u l
    {0x64,  0x20},    //    Character: d
    {0x62,   0x65},    //    Character: b e
    {0x7c,   0x7c},    //    Character: ??
    {0x20,   0x64},    //    Character:   d
    {0x69,   0x73},    //    Character: i s
    {0x70,   0x6c},    //    Character: p l
    {0x61,   0x79},    //    Character: a y
    {0x65,   0x64},    //    Character: e d
    {0x6f,   0x76},    //    Character: o v
    {0x65,   0x72},    //    Character: e r
    {0x6f,   0x6c},    //    Character: o l
    {0x69,   0x64},    //    Character: i d
    {0x20,   0x62},    //    Character:   b
    {0x61,   0x63},    //    Character: a c
    {0x6b,   0x67},    //    Character: k g
    {0x72,   0x6f},    //    Character: r o
    {0x75,   0x6e},    //    Character: u n
    {0x64,   0x20},    //    Character: d
    {0x74,   0x6f},    //    Character: t o
    {0x20,   0x69},    //    Character:   i
    {0x73,   0x6f},    //    Character: s o
    {0x2d,   0x00},    //    Character: -
    {0x6c,   0x61},    //    Character: l a
    {0x74,   0x65},    //    Character: t e
    {0x20,   0x69},    //    Character:   i
    {0x74,   0x20},    //    Character: t
    {0x66,   0x72},    //    Character: f r
    {0x6f,   0x6d},    //    Character: o m
    {0x20,   0x74},    //    Character:   t
    {0x68,   0x65},    //    Character: h e
    {0x20,   0x75},    //    Character:   u
    {0x6e,   0x72},    //    Character: n r
    {0x65,   0x6c},   //    Character: e l
    {0x61,   0x74},    //    Character: a t
    {0x65,   0x64},    //    Character: e d
    {0x20,   0x70},    //    Character:   p
    {0x72,   0x6f},    //    Character: r o
    {0x2d,   0x00},    //    Character: -
    {0x67,   0x72},    //    Character: g r
    {0x61,   0x6d},    //    Character: a m
    {0x20,   0x76},    //    Character:   v
    {0x65,   0x6f}   //    Character: e o
};

//leo 20060510
#define T2_table_max	138
static UINT8 code T2_table[T2_table_max][2] =
{
    {0x1c,   0x2d},    //CR => Channel 2, Field 1: Carriage
    {0x00,   0x00},     //    Character:
    {0x1c,   0x2b},     //RTD => Channel 2, Field 1: Resume Text Display
    {0x19,   0x2a},     //Midrow Code => Channel 2, Underline 0: Yellow
    {0x54,   0x65},     //    Character: T e
    {0x78,   0x74},     //    Character: x t
    {0x20,   0x4d},     //    Character:   M
    {0x6f,   0x64},     //    Character: o d
    {0x65,   0x2c},     //    Character: e ,
    {0x20,   0x73},     //    Character:   s
    {0x65,   0x72},     //    Character: e r
    {0x76,   0x69},     //    Character: v i
    {0x63,   0x65},     //    Character: c e
    {0x20,   0x23},     //    Character:   #
    {0x32,   0x00},     //    Character: 2
    {0x45,   0x61},     //    Character: E a
    {0x63,   0x68},     //    Character: c h
    {0x20,   0x72},     //    Character:   r
    {0x6f,   0x77},     //    Character: o w
    {0x20,   0x6f},     //    Character:   o
    {0x66,   0x20},     //    Character: f
    {0x74,   0x65},     //    Character: t e
    {0x78,   0x74},     //    Character: x t
    {0x7f,   0x7f},     //    Character: ??
    {0x20,   0x63},     //    Character:   c
    {0x6f,   0x6e},     //    Character: o n
    {0x74,   0x61},     //    Character: t a
    {0x69,   0x6e},     //    Character: i n
    {0x73,   0x20},     //    Character: s
    {0x61,   0x00},     //    Character: a
    {0x6d,   0x61},     //    Character: m a
    {0x78,   0x69},     //    Character: x i
    {0x6d,   0x75},     //    Character: m u
    {0x6d,   0x20},     //    Character: m
    {0x6f,   0x66},     //    Character: o f
    {0x20,   0x33},     //    Character:   3
    {0x32,   0x20},     //    Character: 2
    {0x63,   0x68},     //    Character: c h
    {0x61,   0x72},     //    Character: a r
    {0x61,   0x63},     //    Character: a c
    {0x74,   0x65},     //    Character: t e
    {0x72,   0x73},     //    Character: r s
    {0x2e,   0x20},     //    Character: .
    {0x49,   0x66},     //    Character: I f
    {0x20,   0x74},     //    Character:   t
    {0x68,   0x65},     //    Character: h e
    {0x73,   0x6f},     //    Character: s o
    {0x6c,   0x69},     //    Character: l i
    {0x64,   0x20},     //    Character: d
    {0x62,   0x61},     //    Character: b a
    {0x63,   0x6b},     //    Character: c k
    {0x67,   0x72},     //    Character: g r
    {0x6f,   0x75},     //    Character: o u
    {0x6e,   0x64},     //    Character: n d
    {0x20,   0x64},     //    Character:   d
    {0x6f,   0x65},     //    Character: o e
    {0x6e,   0x6f},     //    Character: n o
    {0x74,   0x20},     //    Character: t
    {0x65,   0x78},     //    Character: e x
    {0x74,   0x68},     //    Character: t h
    {0x65,   0x20},     //    Character: e
    {0x66,   0x75},     //    Character: f u
    {0x6c,   0x6c},     //    Character: l l
    {0x20,   0x77},     //    Character:   w
    {0x69,   0x64},     //    Character: i d
    {0x66,   0x20},     //    Character: f
    {0x73,   0x63},     //    Character: s c
    {0x72,   0x65},     //    Character: r e
    {0x65,   0x6e},     //    Character: e n
    {0x2c,   0x20},     //    Character: ,
    {0x61,   0x74},     //    Character: a t
    {0x6c,   0x65},     //    Character: l e
    {0x61,   0x73},     //    Character: a s
    {0x74,   0x20},     //    Character: t
    {0x6f,   0x6e},     //    Character: o n
    {0x73,   0x6f},     //    Character: s o
    {0x6c,   0x69},     //    Character: l i
    {0x64,   0x20},     //    Character: d
    {0x73,   0x70},     //    Character: s p
    {0x61,   0x63},     //    Character: a c
    {0x65,   0x71},     //    Character: e q
    {0x75,   0x61},     //    Character: u a
    {0x6c,   0x20},     //    Character: l
    {0x74,   0x6f},     //    Character: t o
    {0x20,   0x61},     //    Character:   a
    {0x73,   0x69},     //    Character: s i
    {0x6e,   0x67},     //    Character: n g
    {0x6c,   0x65},     //    Character: l e
    {0x20,   0x63},     //    Character:   c
    {0x6f,   0x6c},     //    Character: o l
    {0x75,   0x6d},     //    Character: u m
    {0x6e,   0x20},     //    Character: n
    {0x77,   0x69},     //    Character: w i
    {0x64,   0x74},     //    Character: d t
    {0x68,   0x20},     //    Character: h
    {0x6d,   0x61},     //    Character: m a
    {0x79,   0x20},     //    Character: y
    {0x7c,   0x7c},     //    Character: ??
    {0x62,   0x65},     //    Character: b e
    {0x70,   0x6c},     //    Character: p l
    {0x61,   0x63},     //    Character: a c
    {0x65,   0x64},     //    Character: e d
    {0x20,   0x62},     //    Character:   b
    {0x65,   0x66},     //    Character: e f
    {0x6f,   0x72},     //    Character: o r
    {0x66,   0x69},     //    Character: f i
    {0x72,   0x73},     //    Character: r s
    {0x63,   0x68},     //    Character: c h
    {0x61,   0x72},     //    Character: a r
    {0x61,   0x63},     //    Character: a c
    {0x2d,   0x00},     //    Character: -
    {0x74,   0x65},     //    Character: t e
    {0x72,   0x20},     //    Character: r
    {0x61,   0x6e},     //    Character: a n
    {0x61,   0x66},     //    Character: a f
    {0x72,   0x20},     //    Character: r
    {0x6c,   0x61},     //    Character: l a
    {0x73,   0x74},     //    Character: s t
    {0x20,   0x63},     //    Character:   c
    {0x68,   0x61},     //    Character: h a
    {0x72,   0x61},     //    Character: r a
    {0x63,   0x74},     //    Character: c t
    {0x65,   0x72},     //    Character: e r
    {0x6f,   0x66},     //    Character: o f
    {0x20,   0x65},     //    Character:   e
    {0x68,   0x20},     //    Character: h
    {0x72,   0x6f},     //    Character: r o
    {0x77,   0x20},     //    Character: w
    {0x74,   0x6f},     //    Character: t o
    {0x6e,   0x68},     //    Character: n h
    {0x61,   0x6e},     //    Character: a n
    {0x63,   0x65},     //    Character: c e
    {0x20,   0x6c},     //    Character:   l
    {0x65,   0x67},     //    Character: e g
    {0x69,   0x62},     //    Character: i b
    {0x69,   0x6c},     //    Character: i l
    {0x69,   0x74},     //    Character: i t
    {0x79,   0x2e}     //    Character: y .
};

//leo 20060510
#define T3_table_max	27
static UINT8 code T3_table[T3_table_max][2] =
{
    {0x20,   0x20},     //    Character:
    {0x00,   0x00},     //    Character:
    {0x15,   0x2d},     //CR => Channel 1, Field 2: Carriage
    {0x15,   0x29},     //RDC => Channel 1, Field 2: Resume Direct Captioning
    {0x7c,   0x7c},     //    Character: ??
    {0x01,   0x05},     //XDS => Control: Current-Start, Type: Program Rating
    {0x48,   0x41},     //    Character: H A
//{0x0f,   0x62},
    {0x15,   0x2b},     //RTD => Channel 1, Field 2: Resume Text Display
    {0x11,   0x2a},     //Midrow Code => Channel 1, Underline 0: Yellow
    {0x54,   0x65},     //    Character: T e
    {0x78,   0x74},     //    Character: x t
    {0x20,   0x4d},     //    Character:   M
    {0x6f,   0x64},     //    Character: o d
    {0x65,   0x2c},     //    Character: e ,
    {0x20,   0x73},     //    Character:   s
    {0x65,   0x72},     //    Character: e r
    {0x76,   0x69},     //    Character: v i
    {0x63,   0x65},     //    Character: c e
    {0x20,   0x23},     //    Character:   #
    {0x33,   0x20},     //    Character: 3
    {0x4c,   0x69},     //    Character: L i
    {0x6e,   0x65},     //    Character: n e
    {0x20,   0x33},     //    Character:   3
    {0x20,   0x34},     //    Character:   4
    {0x20,   0x35},     //    Character:   5
    {0x20,   0x36},     //    Character:   6
    {0x20,   0x37}      //    Character:   7
};

#define T4_table_max	27
static UINT8 code T4_table[T4_table_max][2] =
{
    {0x1d,   0x2d},    //CR => Channel 2, Field 2: Carriage
    {0x00,   0x00},    //    Character:
    {0x1d,   0x29},    //RDC => Channel 2, Field 2: Resume Direct Captioning
    {0x01,   0x05},    //XDS => Control: Current-Start, Type: Program Rating
    {0x48,   0x66},    //    Character: H f
//{0x0f,   0x3d},
    {0x1d,   0x2b},    //RTD => Channel 2, Field 2: Resume Text Display
    {0x19,   0x2a},    //Midrow Code => Channel 2, Underline 0: Yellow
    {0x54,   0x65},    //    Character: T e
    {0x78,   0x74},    //    Character: x t
    {0x20,   0x4d},    //    Character:   M
    {0x6f,   0x64},    //    Character: o d
    {0x65,   0x2c},    //    Character: e ,
    {0x20,   0x73},    //    Character:   s
    {0x65,   0x72},    //    Character: e r
    {0x76,   0x69},    //    Character: v i
    {0x63,   0x65},    //    Character: c e
    {0x20,   0x23},    //    Character:   #
    {0x34,   0x20},    //    Character: 4
    {0x20,   0x20},    //    Character:
    {0x4c,   0x69},    //    Character: L i
    {0x6e,   0x65},    //    Character: n e
    {0x20,   0x33},    //    Character:   3
    {0x20,   0x34},    //    Character:   4
    {0x20,   0x35},    //    Character:   5
    {0x20,   0x36},    //    Character:   6
    {0x20,   0x37},    //    Character:   7
    {0x7f,   0x7f}    //    Character: ?
};
#endif
/*======================== PRIVATE CONSTANTS =================================*/
//#define RU_TYPE			2
#define RU_TYPE			2//-1
#define _COLOR_BACKGROUND_CONSTATNT _FALSE

#define column_length			32
#define buffer_length			34
#define default_color			0x10//foreground color = white,background color = black
#define default_attribute		0x00
#define default_font			0x20
#define JUMP_TYPE		0
#define SMOOTH_TYPE		1
#define SCROLL_TYPE		SMOOTH_TYPE	//jerry0413B


/*======================== PRIVATE MACROS ====================================*/

/*======================== PRIVATE TYPES =====================================*/
typedef enum FIELDSTATE
{
    CC1_State,
    CC2_State,
    T1_State,
    T2_State,
    XDS_State
} eFieldState;

enum	CodeFormat
{
    unknown_code,
    pac_code,
    miscellaneous_code,
    midrow_code,
    xds_code,
    optionalattr_code,
    specialchar_code,
    extrenedchar_Code,
    closegroup_code,
    character_code,
    bgattr_code,
    fgattr_code,
    afterunknown_code
};

enum	colorattr
{
    black,
    white,
    green,
    blue,
    cyan,
    red,
    yellow,
    magenta
};

enum CcCommand
{
    nocommand,
    rcl,		//01: resume caption loading
    backspace,	//02: backspace
    reserved1,	//03: reserved
    reserved2,	//04: reserved
    der,		//05: delete to end of row
    ru2,		//06: roll-up captions,2rows
    ru3,		//07: roll-up captions,3rows
    ru4,		//08: roll-up captions,4rows
    flash,		//09: flash on
    rdc,		//10: resume direct captioning
    tr,			//11: text restart
    rtd,		//12: resume text display
    edm,		//13: erase displayed memory
    cr,			//14: carriage return
    enm,		//15: erase nondisplayed memory
    eoc,		//16: end of caption(flipmemories)
    to1,		//17: tab offset(1column)
    to2,		//18: tab offset(2columns)
    to3			//19: tab offset(3columns)
};

enum	rowposition
{
    row0,
    row1,
    row2,
    row3,
    row4,
    row5,
    row6,
    row7,
    row8,
    row9,
    row10,
    row11,
    row12,
    row13,
    row14,
    row15
};

enum	identattr
{
    indent0_white,
    indent4_white,
    indent8_white,
    indent12_white,
    indent16_white,
    indent20_white,
    indent24_white,
    indent28_white
};

enum	SpecialAssignments
{
    normalsize,
    doublesize,
    firstprivate,
    secondprivate,
    china,
    korean,
    firstregistered
};

enum CC_ATTRIBUTE_FLAG
{
    UNDERLINE_FLAG,		//0: underline off	//1: underline on
    ITALICS_FLAG,		//0: italics off	//1: italics on
    FLASHON_FLAG,		//0: flash off		//1: flash on
    DEFAULT_FLAG,		//0: default off	//1:default on
    TRANSPARENT_FLAG	//0: transparent off//1: transparent on
};

enum CC_SYSTEM_FLAG
{
    DISPLAYEMEMSELECT_FLAG,	//0: select nondisplay memory 1		//1: select nondisplay memory 2
    FIELD1CONTROLCODE_FLAG,	//0: Send first command				//1: Send twice command
    INSERTCHARACTER_FLAG,	//0: insert first character			//1: insert continuous character
    DATACHANNEL_FLAG,		//0: data channel 1					//1: data channel 2
    FIELD_FLAG,				//0: field 1						//1: field 2
    DATAFILTER_FLAG,			//0: disable data filter			//1: enable data filter
    SCROLLUP_FLAG				//0: Disable scroll up			//1: Enable scroll up
};

enum CaptionMode
{
    unknow,
    roll_up2,
    roll_up3,
    roll_up4,
    paint_on,
    pop_on,
    text_restart,
    resume_text_display
};

enum	CCSystemState
{
    idle_state,
    cc1_state,
    cc2_state,
    cc3_state,
    cc4_state,
    t1_state,
    t2_state,
    t3_state,
    t4_state,
    xds_state
};

/*======================== PRIVATE VARIABLES =================================*/
static UINT8 xdata CCData1,CCData2;
static UINT8 xdata CCData1_old,CCData2_old;
static UINT8 xdata CCField;           	//Bill 2005-Feb-21 start
//static UINT8 F1ID;
static eFieldState xdata F2State;	//Bill 2005-Feb-21 end
static UINT8 xdata CodeFormat;		//jerry0401(reduce xdata)
static UINT8 xdata CcCommand;			//jerry0401(reduce xdata)
static UINT8 xdata CC_ATTRIBUTE_FLAG;
static UINT8 xdata CC_SYSTEM_FLAG;
static UINT8 xdata Color_F,Color_B;
static UINT8 xdata RowPosition;
static UINT8 xdata Ident;
static UINT8 xdata ColumnIndex, RowIndex;
static UINT8 xdata Caption_RollUp;
static UINT8 xdata RollUp_Mode;
static UINT16 xdata usDispRow;
static UINT8 xdata ucShowCntForPaintOn;
UINT8 xdata bCcStartFlag = 0;
UINT8 xdata bCheckCCStatus = 0;//Tim 0328

UINT8 xdata bVBI_INT_Enable=0;
UINT8 xdata entered = 0;
static UINT8 xdata PreAttribute;			//jerry0519
static UINT8 xdata Color_FS,Color_BS;	//jerry0519
//static UINT8 ucRollupRow;
//static UINT8 CaptionMemIndex1[4];
//static UINT8 CaptionMemIndex2[4];
static UINT8  bCaptionText_Mode;	//jerry0530
//atic BIT  TabOffset_Flag;
static UINT8 TabOffset;
UINT8 xdata  CCBuffer[75][2];//CCBuffer_Size
UINT8 xdata CC_System_Mode;
UINT8 xdata bCCDecodeStalbe=_TRUE;
UINT8 xdata bCCReceiveOK;
bool VbiCc_bEOCnoRCLorPAC = FALSE;//tommy add20100716
UINT8 FirstDataByteFlag =0;

//static UINT8 xdata DisplayBuffer[2][146]; // weihao1012(for 2553V)

/*======================== PRIVATE FUNCTIONS =================================*/
static void CCcInitVariables(void);

//static void CVbiCcDecoder(bit fieldtype);
static void CCcParserPattern(UINT8 fieldtype);
static void CCcParserCommand(void);
static void CCcCaptionModeFunction(void);
static void CCcTextModeFunction(void);
static void CCcSetCaptionModeState(void);
static void CCcSetTextModeState(void);
static void CCcScrollRowOnTimer(void);
static void CCcSetDefaultCharacterAttr(void);
static void CCcInsertCharacterFunction(void);

// Initial
static void CCcInitialPaintOnCommand(void);
static void CCcInitialPopOnCommand(void);
static void CCcInitialRullUpCommand(void);
static void CCcInitialTextModeCommand(void);

static void CCcPopOnInsertChar(UINT8 data1, UINT8 color, UINT8 attr);
static void CCcPopOnInsertCharacterFunction(void);

static void CCcPaintOnInsertChar(UINT8 data1, UINT8 color, UINT8 attr);
static void CCcPaintOnInsertCharacterFunction(void);

static void CCcInsertCharacter2NonDisplayMem(UINT8 row,UINT8 column,UINT8 data1,UINT8 color,UINT8 attribute);
static void CCcInsertCharacter2DisplayMem(UINT8 row,UINT8 column,UINT8 data1,UINT8 color,UINT8 attribute);

static void CCcSpecialCharacterCodes(void);
static void CCcSpaceCharacterToDisplayMem(void);

static void CCcRullUpInsertCharacterFunction(void);
static void CCcRullUpInsertChar2Mem(UINT8 data1, UINT8 color, UINT8 attr);

static void CCcTextModeInsertCharacterFunction(void);

static void CCcSpecialCharacterCodes(void);
static void CCcExternedCharacterCodes(void);
static void CCcOptionalAttributeCodes(UINT8 codeFormat);
static void CCcBackspaceCommand(void);
static void CCcCarriageReturnCommand(void);
static void CCcEndOfCaptionCommand(void);
void CVbiInitial(void);
static void CCcBackspace(void);
void VbiCcOsd_InsertReferNowPreSpace( UINT8 row,UINT8 column,UINT8 color,UINT8 attr);
//leo 20060430
#ifdef CC_AUTO_TEST
static UINT8 CCcAutoTestFunction(void);
static UINT8 CTtAutoTestFunction(void);
#endif

//-----------------------------------------------------------------------------------------------//
//------------------------------------- Variables ------------------------------------------------//
//-----------------------------------------------------------------------------------------------//

//UINT8 VBI_DDR_NO,VBI_DDR_LEN,VBI_DDR_REM;	//jerry0401(reduce xdata)
UINT8 CVbiIsrDisable(void)
{
    if ((GET_CLOSEDCAPTION()==0)||

            ((GET_INPUTSOURCE_TYPE()==_SOURCE_VGA)	||(GET_INPUTSOURCE_TYPE()==_SOURCE_HDMI)))
        return _TRUE;
    else
        return _FALSE;
}

void CVbiInitial(void)
{
    ucCCWriteCnt = 0;
    ucCCReadCnt = 0;
    bCCReceiveOK = 0;
}

//-----------------------------------------------------------------------------------------------//
//------------------------------------- program  ------------------------------------------------//
//-----------------------------------------------------------------------------------------------//
void CCcSetSystemMode(void)
{
    UINT8 code OSDMenu_CCMap[] = {0, _Cc1Mode,_Cc2Mode,_Cc3Mode,_Cc4Mode,
                                  _T1Mode ,_T2Mode ,_T3Mode ,_T4Mode
                                 };

    UINT8 	Mode  = GET_CLOSEDCAPTION();
    CC_System_Mode = OSDMenu_CCMap[Mode];
}

static void CCcInitVariables(void)
{
//	CC_System_Mode = 0x00;			//VBI_ENABLE(osd)
//	ScrollRowIndex_Save	= 0;
//	Caption_RollUp = 0;
    //bCCInterrupt = 0;
    ucRollupTimerCnt = 0;
    RollUp_Mode = 0;
    CCData1 = 0;
    CCData2 = 0;
    CCData1_old = 0;
    CCData2_old = 0;
    CCSystemState = 0;
    CodeFormat =0;
    CaptionMode = 0;
    CcCommand = 0;
    CC_SYSTEM_FLAG = 0;
    CC_ATTRIBUTE_FLAG = 0;
    RowPosition = 0;
    Ident = 0;					//weihao 1120
    TabOffset = 0;				// weihao 1122
    usDispRow = 0x0000;
    ucShowCntForPaintOn = 0;
//	Ident_Flag = 0;					//jerry0329 , canceled by weihao
//	Insert_DefaultCharacter = 0;	//jerry0329
    Variable_BitClear(CC_ATTRIBUTE_FLAG,DEFAULT_FLAG);

//	F1ID = 0;               			//default setting, Bill 2005-Feb-21 start
    F2State = XDS_State;				//default setting, Bill 2005-Feb-21 end
#ifdef _VCHIP
    Vchip_Init();
#endif
    RowIndex = ColumnIndex = 0;
//	ucRollupRow = 1;
//	ScreenDispRowIndex = 0;
    //CVbiScrollRowIndexTable(0);
//	V_START_POS_Temp = _VBI_V_START_POS;
//	ScrollRow_Flag = 0;
//	EraseRow_Flag = 0;
//	ScrollRow_Cnt = 0;
//	CaptionDispRowIndex1 = 0;
//	CaptionDispRowIndex2 = 0;
}

void CCcInitial(void)
{
    ucOsdVbiControl = 0x00;
    CVbiInitial();
    CCcInitVariables();
    CCcSetSystemMode();

    //--->w	CVbiClearScreenMemory();
    //--->w	CVbiClearDisplayMemory();
    //--->w	CVbiFillCharacter2DisplayBuffer(default_font,_DISPBUFMEM1);
    //--->w	CVbiFillCharacter2DisplayBuffer(default_font,_DISPBUFMEM2);
    CLR_OSDVBI_INITIAL();
}
/*
BYTE CheckCCTTMode(void)
{
	if(bCaptionText_Mode == 0)	return 0;		//CC
	else										return 1;		//TT

}
*/
void CCcDecoder(void)
{
    UINT8 bFieldFlag;

    if ((GET_INPUTSOURCE_TYPE()!=_SOURCE_CVBS)&&(GET_INPUTSOURCE_TYPE()!=_SOURCE_SV)
            &&(GET_INPUTSOURCE_TYPE()!=_SOURCE_TV)&&(GET_INPUTSOURCE_TYPE()!=_SOURCE_YPBPR))
        return;

    if (info->state != _MODE_STATE_ACTIVE)
        return;

    if (ucOsdState != _IDLE_STATE)
    {
        if (ucOsdState != _VBI_SHOW_STATE)
            return;
    }

    //////////////////////////////////////////////////////////
    //Check if CC is diable  and VBI is disable
    ///////////////////////////////////////////////////////////
    //if((!bCcStartFlag)&&(!bVBI_INT_Enable))
    if ((!bCcStartFlag)&&(!bVBI_INT_Enable))
        return;
    //RTD_Log(LOGGER_INFO, "\n++bCcStartFlag,bVBI_INT_Enable002 = %x,%x", (UINT32)bCcStartFlag,(UINT32)bVBI_INT_Enable);
    //////////////////////////////////////////////////////////
    //Check if CC is On
    // ==> Check if Roll Up mode
    // ==> Roll Up processing
    ///////////////////////////////////////////////////////////
    if (bCcStartFlag)
        CCcScrollRowOnTimer();
//==========================//
#if 0//def _VCHIP
#ifdef _VBI_ENABLE
    /////////////////////////////////////////////////////////
    // if it doesn't received vchip data for a period of time, cancel the blanking action
    /////////////////////////////////////////////////////////
    if (bVchip_CanInitial)
    {
        Vchip_Init();
        ////////////////////////////////////////////////////
        // disable force to background
        ////////////////////////////////////////////////////
        if ((ucSyncProcessState == _ACTIVE_STATE) || (ucVideoProcessState == _ACTIVE_STATE))
            CScalerSetBit(_VDISP_CTRL_28, 0xdf, 0x00);
    }
#endif
#endif
//==========================//
    if (ucCCReadCnt!=ucCCWriteCnt /*|| bCCReceiveOK==1*/)
    {
        ////////////////////////////////
        //Get CC Data
        ////////////////////////////////
        CCData1 = CCBuffer[ucCCReadCnt][0]&0x7F;
        CCData2 = CCBuffer[ucCCReadCnt][1];
        ////////////////////////////////
        //Get Field Data
        ////////////////////////////////
        bFieldFlag = CCBuffer[ucCCReadCnt][0] & 0x80;
        ////////////////////////////////
        //Change read Buffer pointer
        ////////////////////////////////
        ucCCReadCnt = (ucCCReadCnt + 1) % CCBuffer_Size;
        ////////////////////////////////
        //Reset receive status
        ////////////////////////////////
        bCCReceiveOK = 0;
//==========================//
#ifdef	CC_AUTO_TEST
        /*
        		if (!CCcAutoTestFunction())
        			{
        			COsdMessagePush(_SHOW_CC_ERROR_MESSAGE);
        			return;
        			}
        */
#endif
//==========================//
        ///////////////////////////////////////
        //Original: 0: Top, 1: Bottom
        //==> Transfer to be 1: Top, 0: Bottom
        ///////////////////////////////////////
#if 0             //cc test 20100315
        bFieldFlag = !bFieldFlag;
#else
        if (bFieldFlag != 0)
            bFieldFlag =1;
        else
            bFieldFlag=0;
#endif

        ///////////////////////////////////////
        if (bFieldFlag != 0)
        {
            /////////////////
            // 1:field 1(Top)
            //FIELD_FLAG: Bottom field
            /////////////////
            Variable_BitClear(CC_SYSTEM_FLAG,FIELD_FLAG);
        }
        else
        {
            ///////////////////
            //0:field 2(Bottom)
            //FIELD_FLAG: Bottom field
            ///////////////////
            Variable_BitSet(CC_SYSTEM_FLAG,FIELD_FLAG);
        }

        ////////////////////////////////////////////
        //Parser CC Data for EIA-608 spec.
        ////////////////////////////////////////////
        CCcParserPattern(bFieldFlag);

//===============//
#ifdef _VCHIP
//===============//
        //if(bCcStartFlag /*&& !VchipBlockStatus*/ && COSDGetCurrentCCAnalogType())
        if (bCcStartFlag)
        {
//===============//
#else	//CONFIG_VBI_VCHIP
//===============//
        ////////////////////////////////////////////////////////////////
        //Check if CC is On and Type(CC_AnalogCCType) is not off
        ////////////////////////////////////////////////////////////////
        //if(bCcStartFlag && COSDGetCurrentCCAnalogType())
        if (bCcStartFlag)
        {
//===============//
#endif	//CONFIG_VBI_VCHIP
//===============//
            //////////////////////////////
            //When CCcEnable() is called,
            //entered will be reset to be 0
            //////////////////////////////
            if (entered == 0)
            {
                entered = 1;
                /////////////////////////////////////////////
                //Reset OSD related variable for OSD display:
                //	ucRollupTimerCnt /	ucOsdVbiRowOffset	/	ucOsdVbiColOffset	 /	ucOsdVbiNonShowRowFlag......
                //////////////////////////////////////////////
                //COsdVbiInitial();

                if ( GET_CLOSEDCAPTION()&&(ucOsdState!=_VBI_SHOW_STATE) )
                {
                    CLR_OSDVBI_INITIAL();
                    CCcInitial();
                    COsdVbiInitial();
                    ucOsdState = _VBI_SHOW_STATE;
                }
            }
            ///////////////////////////////////
            //Parser CC Command for OSD
            ///////////////////////////////////
            CCcParserCommand();
        }	//if(bCcStartFlag && COSDGetCurrentCCAnalogType())
    }
}

void CCcEnable(void)
{
#if 0 //temp disable
    ET1 = 0; // disable timer1
#endif
//	RTD_ShowScreenData(0);
//	RTD_CtrlScreen_StartRow(1);
//	RTD_CtrlScreen_TotalRow(15);
//	CCcInitVariables ();
#if 0//temp disable20100311 for test
    CCcInitial();
#endif

#if 0//temp disable
    TL1 = _TIMER1_COUNT_LBYTE;
    TH1 = _TIMER1_COUNT_HBYTE;
    ET1=1; // enable timer1
    TR1 = 1;          // Start Timer1
#endif
    bVBI_INT_Enable	 = 1;
    bCcStartFlag = 1;
    //CScalerSetDataPortByte(_VBI_ACCESS_PORT_9A, _VBISL0_0, 0x03);//@Set VBI function enable
    //////////////////////////////////////
    //Reset CC enterance state
    //////////////////////////////////////
    entered = 0;

}
#if 1
void CCcDisable(void)
{
//	RTD_ShowScreenData(0);
#if 0 ////temp disable
    TR1 = 0;          // Stop Timer1
    ET1=0;  // disable timer1
#endif
    //bCCInterrupt=0;
    bVBI_INT_Enable	 = 0;
    bCCReceiveOK = 0;
    //CScalerSetDataPortByte(_VBI_ACCESS_PORT_9A, _VBISL0_0, 0x02);//@Set VBI function disable
    ///////////////////////////////////////
    //Check if CC is on
    //////////////////////////////////////
    if (bCcStartFlag == 1)
    {
        ///////////////////////////////////////
        //Yes ==> Turn off CC and Set for normal OSD
        //////////////////////////////////////
        COsdVbiShowData(0);
        COsdVbiBlendingOperation(0);
        //COsdReloadFont();
    }
    bCcStartFlag = 0;
}
#endif

static void CCcParserPattern(UINT8 fieldtype)
{
    UINT8 ControlByte;
    static UINT16 XDS_CheckSum;
    static UINT8  XDS_Data[2];
    static BOOL Program_Rating;

    UINT8 pos[] = { 	0xff, 0xff, row11, 0xff, 0xff, 0xff, 				//5
                     row1, row2, 0xff, 0xff, row3, row4, 0xff, 0xff,  	//13
                     row12, row13, 0xff, 0xff, row14, row15, 0xff, 		//20
                     0xff, row5, row6, 0xff, 0xff,
                     row7, row8, 0xff, 0xff, row9, row10
                  };

    if ((CCData1 >= 0x01) && (CCData1 <= 0x0F) && fieldtype==0)
    {
        //RTD_Log(LOGGER_INFO, "\n+++CCcParserPattern001", 0);
        //////////////////////////////////////////////////////
        //Extend Data Services
        //////////////////////////////////////////////////////
        //
        ////////////////////////////////////////////
        //From code, bCCDecodeStalbe is always _TRUE)
        ////////////////////////////////////////////
        if (bCCDecodeStalbe == _FALSE)		//jerry0725
            return;
        //RTD_Log(LOGGER_INFO, "\n+++CCcParserPattern", 0);
        if (CCData1==0x01 || CCData1==0x02 || CCData1==0x03 || CCData1==0x05 || CCData1==0x07)
        {
            ///////////////////////////////////////////////////////////////////////////////////////////////
            //Current-Start / Future-Start / Channel-Start / Miscellaneous-Start
            ///////////////////////////////////////////////////////////////////////////////////////////////
            F2State = XDS_State;
            Program_Rating = _FALSE;
            XDS_CheckSum = 0;
            XDS_CheckSum += CCData1 + CCData2;
            if ((CCData1==0x01&&CCData2==0x05)||(CCData1==0x02&&CCData2==0x05)) //0x01:Current-Start ; 0x05:Program Rating
            {
                XDS_Data[0] = 0x00;
                XDS_Data[1] = 0x00;
                Program_Rating = _TRUE;
            }
        }
        /*
        		if(CCData1==0x0F){	//All-end & Checksum
        			XDS_CheckSum += CCData1 + CCData2;
        			if((UINT8)XDS_CheckSum==0x00 && Program_Rating==_TRUE)
        				Vchip_InsertCharacterFunction(XDS_Data[0],XDS_Data[1]);
        			XDS_CheckSum = 0;
        			F2State = CC1_State;
        		}
        */
        ////////////////////////////////////////
        ///All-end & Checksum
        // Reset to be CC state
        ////////////////////////////////////////
        if (CCData1==0x0F)	//All-end & Checksum
            F2State = CC1_State;

        CodeFormat = xds_code;	//jerry1111
        /*tdl
        	  Extended data services informations are contained
        	  in Line 284(even field, fieldtype=0); CCData1 contains
        	  the control characters which has value of 0x01 ~ 0x0F
        	  and used as a mode switch to enable the extended data
        	  mode.
         */
        //F2State = XDS_State; 	//Bill, 2005-Feb-21, it begins xds data from this command code start
//==========================//
#ifdef _VCHIP

//		F1ID = !CCField;
        Vchip_ParseXDSCmd(CCData1, CCData2);	//Bill, 2005-Feb-21, it begins xds data from this command code end
#endif
//==========================//
    }
    else if ((CCData1 & 0x10) && (CCData1 <= 0x1F))
    {
        //RTD_Log(LOGGER_INFO, "\n+++CCcParserPattern003", 0);
        ////////////////////////////////
        // Control code
        ////////////////////////////////
        if (fieldtype == 0)
        {
            ///////////////////////////////////
            //Bottom Field
            ///////////////////////////////////
            F2State = CC1_State;
        }

        //////////////////////////////////////////////////////////////////////////////
        //CC1/CC2/T1/T2(Field1) and Current is Bottom Field ==> Ignore
        //////////////////////////////////////////////////////////////////////////////
        if ((CC_System_Mode&0x0F)!=0 && fieldtype==0)		//OSD:CC1/CC2/T1/T2(Field1);Data:(Field2)//jerry1103
            return;
        //////////////////////////////////////////////////////////////////////////////
        //CC1/CC2/T1/T2(Field1) and Current is Bottom Field ==> Ignore
        //////////////////////////////////////////////////////////////////////////////
        if ((CC_System_Mode&0xF0)!=0 && fieldtype==1)		//OSD:CC1/CC2/T1/T2(Field1);Data:(Field2)//jerry1103
            return;
        //RTD_Log(LOGGER_INFO, "\n+++CCcParserPattern005", 0);
        /*tdl
        	  Control code could be transmitted on line 21 or line 284.
        	  On line 21 they are normally transmitted twice but only once
        	  line 284. CCData1 is a nondisplay control byte with a range of
        	  0x10 ~ 0x1F.
         */
        //if (fieldtype == 0)
        //	F2State = CC1_State;

        //------------------------------------ PAC -----------------------------------------//
        ///////////////////////////////////////////////////////
        // Display Control Data
        ///////////////////////////////////////////////////////
        if (CCData2 & 0x40)
        {
            //RTD_Log(LOGGER_INFO, "\n+++CCcParserPattern006", 0);
            ///////////////////////////////////////////////////////
            // PAC //tdl Preamble Address Code
            //////////////////////////////////////////////////////
            //
            /////////////////////////////////////////////////////
            //Check if CC mode(roll_up2 /	roll_up3 / 	roll_up4 /	paint_on /	pop_on)
            //////////////////////////////////////////////////////
            if (CaptionMode >= roll_up2 && CaptionMode <= pop_on)
            {
                ////////////////////////////////////////////////
                //Set color attribute to be default value: Color_F / Color_B
                // CC_ATTRIBUTE_FLAG, Ident , Color_FS,  Color_BS......
                ////////////////////////////////////////////////
                CCcSetDefaultCharacterAttr();
            }
            /////////////////////////////////////////////////////
            //Set Preamble Address codes
            //////////////////////////////////////////////////////
            CodeFormat = pac_code;
            ////////////////////////////////
            //Set Underline
            //D0: U: °ß0°® = no underline, °ß1°® = underline.
            ////////////////////////////////
            if (CCData2&0x01)
                Variable_BitSet(CC_ATTRIBUTE_FLAG,UNDERLINE_FLAG);
            else
                Variable_BitClear(CC_ATTRIBUTE_FLAG,UNDERLINE_FLAG);

            ////////////////////////////////
            //Set Data Channel
            //D3: CH: °ß0°® = data channel 1, °ß1°® = data channel 2.
            ////////////////////////////////
            if (CCData1&0x08)
                Variable_BitSet(CC_SYSTEM_FLAG,DATACHANNEL_FLAG);
            else
                Variable_BitClear(CC_SYSTEM_FLAG,DATACHANNEL_FLAG);

            ////////////////////////////////
            //Set Color & Ident attribute
            /*
            D4 D3 D2 	D1  			Attribute
            0 0 0 0 						white
            0 0 0 1 						green
            0 0 1 0 						blue
            0 0 1 1 						cyan
            0 1 0 0 						red
            0 1 0 1 						yellow
            0 1 1 0 						magenta
            0 1 1 1 						italics
            1 0 0 0 						indent 0, white
            1 0 0 1						indent 4, white
            1 0 1 0 						indent 8, white
            1 0 1 1 						indent 12, white
            1 1 0 0 						indent 16, white
            1 1 0 1 						indent 20, white
            1 1 1 0 						indent 24, white
            1 1 1 1 						indent 28, white
            */
            ////////////////////////////////
            ControlByte = (CCData2>>1)&0x0f;	//jerry0324 start
            if (ControlByte<=6)
            {
                ///////////////////////////////////
                //Color Attribute
                ///////////////////////////////////
                Color_F	= ControlByte + 0x01;
                Color_FS = (Color_FS<<4 | Color_F);
                PreAttribute |= 0x80;
            }
            else if (ControlByte==7)
            {
                ///////////////////////////////////////
                //Italics font
                //////////////////////////////////////
                Variable_BitSet(CC_ATTRIBUTE_FLAG,ITALICS_FLAG);
            }
            else if (ControlByte>=8)
            {
                ///////////////////////////////////////
                //Indent, white
                //////////////////////////////////////
                Color_F = white;
                Color_FS = (Color_FS<<4 | Color_F);
                PreAttribute |= 0x80;
                Ident = (ControlByte - 8);
            }
            //////////////////////////////////////
            //Set Row position
            /*
            00110	Row 1
            00111	Row 2
            01010	Row 3
            01011	Row 4
            10110	Row 5
            10111	Row 6
            11010	Row 7
            11011	Row 8
            11110	Row 9
            11111	Row 10
            00010	Row 11
            01110	Row 12
            01111	Row 13
            10010	Row 14
            10011	Row 15
            */
            //////////////////////////////////////
            ControlByte = (CCData1&0x07)<<2;
            ControlByte += (CCData2&0x60)>>5;
            //////////////////////////////////////////
            //Check if Row position exist?
            /*
            UINT8 pos[] =
            {
            	0xff, 0xff, row11, 0xff, 0xff, 0xff, 						//5
            	row1, row2, 0xff, 0xff, row3, row4, 0xff, 0xff,  	//13
            	row12, row13, 0xff, 0xff, row14, row15, 0xff, 	//20
            	0xff, row5, row6, 0xff, 0xff,
            	row7, row8, 0xff, 0xff, row9, row10
            };
            */
            if (pos[ControlByte] != 0xff)
            {
                ////////////////////////////////
                //Transfer to Row Position
                ///////////////////////////////
                RowPosition =  pos[ControlByte];
            }
        }
        else
        {
            /////////////////////////////////////////////////////////////////////////////
            //CC3/CC4/T3/T4(Field2) and Current is Bottom Field or
            //CC1/CC2/T1/T2(Field1) and Current is Top Field
            //==> Data is correct
            //////////////////////////////////////////////////////////////////////////////
            if ((fieldtype==0 && CC_System_Mode&0xF0)!=0||(fieldtype==1 && CC_System_Mode&0x0F)!=0) //jerry0530
            {
                /*tdl
                	  I think (CC_System_Mode&0x0F) !=0 means CC1/CC2/T1/T2(Field1) mode.
                 */
                /////////////////////////////////
                //check which channel
                // 0: channe1, 1: channel2
                //	  Test the CH bit in CCData1: CH=0 means channel 1; CH=1 means channel2.
                //////////////////////////////////
                RTD_Log(LOGGER_INFO, "\n+++CCcParserPattern007", 0);
                if (CCData1&0x08)
                    /*tdl
                    	  Test the CH bit in CCData1: CH=0 means channel 1; CH=1 means channel2.
                     */
                    Variable_BitSet(CC_SYSTEM_FLAG,DATACHANNEL_FLAG);	//tdl channel 2
                else
                    Variable_BitClear(CC_SYSTEM_FLAG,DATACHANNEL_FLAG);	//tdl channel 1
            }
            /*tdl
            Table for switch case:
            							CCData1	  CCData2
            miscellaneous:		001x  10x	  010  	xxxx -> 22/2A
            midrow:				001x  001	  010  	xxxx -> 0A
            special char:			001x  001    011  	xxxx -> 0B
            option:					001x  000	  010  	xxxx -> 02
            extension:				001x  111	  010  	xxxx -> 3A
            extension char:		001x  01x	  01x  	xxxx -> 12/13/1A/1B
               */
            //////////////////////////////////////
            //Combine CCData1 and CCData2
            //////////////////////////////////////
            ControlByte = ((CCData1<<3) + (CCData2>>4)) & 0x3F;
//			if(CCData1 == 0x10 && CCData2 == 0x24)
            //------------------------------------ Miscellaneous -----------------------------------------//
            //ROSPrintf("ControlByte = %X when enter not pac control code.....\n", ControlByte);
            /////////////////////////////////////////////////
            //Check which mode
            /////////////////////////////////////////////////
            RTD_Log(LOGGER_INFO, "\n+++CCcParserPattern008", 0);
            switch (ControlByte)
            {

                //============//
                //miscellaneous
                //============//
            case 0x22:
            case 0x2A:
                //////////////////////////////////////////////
                //Check if the same field
                //Data1: D0: 0(line21), 1(line284)
                ///////////////////////////////////////////////
                if ((CCData1&0x01) ^ Variable_BitValue(CC_SYSTEM_FLAG,FIELD_FLAG))
                {
                    CodeFormat = unknown_code;
                    return;
                }
                //////////////////////////////////////////////
                //Set format to be miscellaneous mode.
                ///////////////////////////////////////////////
                CodeFormat = miscellaneous_code;
                //////////////////////////////////////////////
                //Extract data from data2
                ///////////////////////////////////////////////
                ControlByte = CCData2&0x0f;
                //////////////////////////////////
                //8: Flash On
                //////////////////////////////////
                if (ControlByte==8)
                {
                    CcCommand = flash;			//Flash On
                    Variable_BitSet(CC_ATTRIBUTE_FLAG,FLASHON_FLAG);
                    PreAttribute = flash | 0x40;
                }
                else
                    ////////////////////////////////////
                    //Save other command
                    ///////////////////////////////////
                    CcCommand = (ControlByte+0x01);
                break;
                //------------------------------------ Midrow code -----------------------------------------//
                //============//
                //midrow
                //============//
            case 0x0A:
                //////////////////////////////////////////////
                //Set format to be midrow mode.
                ///////////////////////////////////////////////
                CodeFormat = midrow_code;
                //////////////////////////////////////////////
                //Data2: D0: Underline
                ///////////////////////////////////////////////
                if (CCData2&0x01)
                    Variable_BitSet(CC_ATTRIBUTE_FLAG,UNDERLINE_FLAG);
                else
                    Variable_BitClear(CC_ATTRIBUTE_FLAG,UNDERLINE_FLAG);
                //////////////////////////////////////////////
                //Data1: D3: Data Channel
                ///////////////////////////////////////////////
                if (CCData1&0x08)
                    Variable_BitSet(CC_SYSTEM_FLAG,DATACHANNEL_FLAG);
                else
                    Variable_BitClear(CC_SYSTEM_FLAG,DATACHANNEL_FLAG);
                ///////////////////////////////////////////////
                //A color or italics midrow control code turns off the flash.
                //Set Color & Ident attribute
                //However, a color attribute turns off the italics attribute.
                ///////////////////////////////////////////////
                Variable_BitClear(CC_ATTRIBUTE_FLAG,FLASHON_FLAG);
                Variable_BitClear(CC_ATTRIBUTE_FLAG,ITALICS_FLAG);
                //////////////////////////////////////////////
                //Extract Color Data
                ///////////////////////////////////////////////
                ControlByte = (CCData2>>1)&0x07;
                //////////////////////////////////////////////
                // Check if italics(7)
                ///////////////////////////////////////////////
                if (ControlByte!=7)
                {
                    //////////////////////////////////////////////
                    // Save color data
                    ///////////////////////////////////////////////
                    Color_F = ControlByte + 0x01;
                    Color_FS = (Color_FS<<4 | Color_F);
                    PreAttribute |= 0xC0;
                }
                else
                {
                    //////////////////////////////////////////////
                    // Italics
                    ///////////////////////////////////////////////
                    Variable_BitSet(CC_ATTRIBUTE_FLAG,ITALICS_FLAG);
                    //////////////////////////////
                    //rdc doesn't mean italics, it is used for recognizing in CCcExternedCharacterCodes(),.....
                    ///////////////////////////////
                    PreAttribute = rdc | 0x40;
                }
                break;
                //------------------------------------ Special Characters -----------------------------------------//
                //============//
                //Special char(Not support)
                //============//
            case 0x0B:
                //////////////////////////////////////////////
                //Set format to be special char
                ///////////////////////////////////////////////
                CodeFormat = specialchar_code;
                break;
                //------------------------------------  optional attribute -----------------------------------------//
                //============//
                //Optional Attribute
                //============//
            case 0x02:
                //////////////////////////////////////////////
                //Set format to be Optional Attribute
                ///////////////////////////////////////////////
                //CodeFormat = optionalattr_code;
                CodeFormat = bgattr_code;
                ////////////////////////////////////////
                //Extract Color
                ///////////////////////////////////////
                ControlByte = (CCData2>>1)&0x0f;
//===========================================//
#if (_COLOR_BACKGROUND_CONSTATNT == _FALSE)
//===========================================//
                ////////////////////////////////////////
                //Background Attribute
                ////////////////////////////////////////
                Color_B = (ControlByte+0x01)&0x07;
//===========================================//
#else	//(_COLOR_BACKGROUND_CONSTATNT == _FALSE)
//===========================================//
                Color_B = black;
//===========================================//
#endif	//(_COLOR_BACKGROUND_CONSTATNT == _FALSE)
//===========================================//
//===========================================//
#if 0	//(_TRANSPARENT_ENABLE == _TRUE)
//===========================================//
                if ((CCData1==0x10 || CCData1==0x18) && (CCData2&0x21)==0x21)
                {
                    Variable_BitSet(CC_ATTRIBUTE_FLAG,TRANSPARENT_FLAG);
                }
                else
                    Variable_BitClear(CC_ATTRIBUTE_FLAG,TRANSPARENT_FLAG);
//===========================================//
#endif	//(_TRANSPARENT_ENABLE == _TRUE)
//===========================================//
                break;
                //------------------------------------  Closed Group Extensions + Miscellaneous + optional attribute -----------------------------------------//
                //============//
                //Optional Attribute
                //============//
            case 0x3A:
                //////////////////////////////////
                //Check which mode
                ///////////////////////////////////
                switch (CCData2 & 0x0f)
                {
                    ////////////////////////////////
                    // Miscellaneous
                    ////////////////////////////////
                    //============//
                    //tab offset1
                    //============//
                case 0x01:
                    CcCommand = to1;
                    CodeFormat = miscellaneous_code;
                    break;
                    //============//
                    //tab offset2
                    //============//
                case 0x02:
                    CcCommand = to2;
                    CodeFormat = miscellaneous_code;
                    break;
                    //============//
                    //tab offset3
                    //============//
                case 0x03:
                    CcCommand = to3;
                    CodeFormat = miscellaneous_code;
                    break;
                    ////////////////////////////////
                    //Closed Group Extensions
                    ////////////////////////////////
                case 0x04:
                case 0x05:
                case 0x06:
                case 0x07:
                case 0x08:
                case 0x09:
                case 0x0a:
                    CodeFormat = closegroup_code;
                    break;
                    ////////////////////////////////
                    //optional attribute
                    ////////////////////////////////
                    //============//
                    // background transparent
                    //============//
                case 0x0d:
                    CodeFormat = optionalattr_code;
                    break;
                    //============//
                    //black
                    //============//
                case 0x0e:
                    //As with midrow codes, a foreground attribute code turns off
                    //italics and blinking, and the least significant bit controls
                    //underlining.
                    Variable_BitClear(CC_ATTRIBUTE_FLAG,FLASHON_FLAG);
                    Variable_BitClear(CC_ATTRIBUTE_FLAG,ITALICS_FLAG);
                    Variable_BitClear(CC_ATTRIBUTE_FLAG,UNDERLINE_FLAG);
//============================================//
#if (_COLOR_BACKGROUND_CONSTATNT == _FALSE)
//============================================//
                    Color_F = black;
//============================================//
#else
//============================================//
                    Color_F = black;
//============================================//
#endif
//============================================//
                    //CodeFormat = optionalattr_code;
                    CodeFormat = fgattr_code;
                    break;
                    //============//
                    //black underline
                    //============//
                case 0x0f:
                    //As with midrow codes, a foreground attribute code turns off
                    //italics and blinking, and the least significant bit controls
                    //underlining.
                    Variable_BitClear(CC_ATTRIBUTE_FLAG,FLASHON_FLAG);
                    Variable_BitClear(CC_ATTRIBUTE_FLAG,ITALICS_FLAG);
                    Variable_BitClear(CC_ATTRIBUTE_FLAG,UNDERLINE_FLAG);
//============================================//
#if (_COLOR_BACKGROUND_CONSTATNT == _FALSE)
//============================================//
                    Color_F = black;
//============================================//
#else
//============================================//
                    Color_F = black;
//============================================//
#endif
//============================================//
                    Variable_BitSet(CC_ATTRIBUTE_FLAG,UNDERLINE_FLAG);
                    //CodeFormat = optionalattr_code;
                    CodeFormat = fgattr_code;
                    break;
                default:
                    CodeFormat = unknown_code;
                    break;
                }
                break;
                //------------------------------------  Extended Character Set -----------------------------------------//
                //============//
                //Extended Character Set
                //============//
            case 0x12:
            case 0x13:
            case 0x1A:
            case 0x1B:
                CodeFormat = extrenedchar_Code;
                break;
                //------------------------------------  Unknown Control Code -----------------------------------------//
                //============//
                //Unknown code
                //============//
            default:
                CodeFormat = unknown_code;
                break;
            }	//switch(ControlByte)
        }	//		//if(CCData2 & 0x40)
    }
    else		////if((CCData1 >= 0x01) && (CCData1 <= 0x0F) && fieldtype==0)
    {
        //------------------------------------  Basic Character Set -----------------------------------------//
        //////////////////////////////////////
        // Basic Character Set
        //////////////////////////////////////
//=======================//
#ifdef _VCHIP
        if ((F2State == XDS_State) && fieldtype==0)
        {
            if (CCData1!=0x00 && CCData2!=0x00)
            {
                XDS_CheckSum += CCData1 + CCData2;
                XDS_Data[0] = CCData1;
                XDS_Data[1] = CCData2;
            }
            // hill open,to avoid Fluke non-standard vchip rating not work
            Vchip_InsertCharacterFunction(CCData1, CCData2);
            //F2State = CC1_State;
        }
        else //if((F2State == XDS_State) && fieldtype==0)
        {
            ///////////////////////////////////////
            //CC1,2,3,4/ T1,2,3,4
            ///////////////////////////////////////
            if ((CC_System_Mode&0x0F)!=0 && fieldtype==0)		//OSD:CC1/CC2/T1/T2(Field1);Data:(Field2)//jerry1103
                return;
            if ((CC_System_Mode&0xF0)!=0 && fieldtype==1)		//OSD:CC3/CC4/T3/T4(Field2);Data:(Field2)
                return;

//			if(CodeFormat != afterunknown_code) {
//				if((CCData1 ==0x00) && (CCData2 ==0x00))
//					CodeFormat = afterunknown_code;
//				else
            ///////////////////////////////////////////
            //Set Character format status
            ///////////////////////////////////////////
            CodeFormat = character_code;
//			}
        }
//=======================//
#else
        //RTD_Log(LOGGER_INFO, "\n+++CCcParserPattern004", 0);
//=======================//
        if (!((F2State == XDS_State) && fieldtype==0))
            CodeFormat = character_code;
//=======================//
#endif
//=======================//
    }
}

static void CCcParserCommand(void)
{

    if ((((CC_System_Mode&0x0F)!=0) ^ Variable_BitValue(CC_SYSTEM_FLAG,FIELD_FLAG)) == 0x00)
        return;
    if ((((CC_System_Mode&0x55)!=0) ^ Variable_BitValue(CC_SYSTEM_FLAG,DATACHANNEL_FLAG)) == 0x00)
        return;

    RTD_Log(LOGGER_INFO, "\n+++CCcParserCommand001", 0);
    ////////////////////////////////////////////////////////////
    //Control codes transmit twice in field 1.
    ////////////////////////////////////////////////////////////
    if (CodeFormat != character_code && CodeFormat != unknown_code /*&& !Variable_BitValue(CC_SYSTEM_FLAG,FIELD_FLAG)*/)
    {
        //jerry0415B
        if (!Variable_BitValue(CC_SYSTEM_FLAG,FIELD1CONTROLCODE_FLAG))
        {
            Variable_BitSet(CC_SYSTEM_FLAG,FIELD1CONTROLCODE_FLAG);
            CCData1_old = CCData1;
            CCData2_old = CCData2;
        }
        else
        {
            if (CCData1 == CCData1_old && CCData2 == CCData2_old)
            {
                Variable_BitClear(CC_SYSTEM_FLAG,FIELD1CONTROLCODE_FLAG);
                return;
            }
            else
            {
                //Variable_BitSet(CC_SYSTEM_FLAG,FIELD1CONTROLCODE_FLAG);
                CCData1_old = CCData1;
                CCData2_old = CCData2;
            }
        }
    }
    else 									//jerry0419B start
    {
        CCData1_old = CCData1;
        CCData2_old = CCData2;
        Variable_BitClear(CC_SYSTEM_FLAG,FIELD1CONTROLCODE_FLAG);
    }										//jerry0419B end

    if (((CCData1&0x7F)==0)&&((CCData2&0x7F)==0))
        return;

    if ((CC_System_Mode&0x33)!=0)
    {
        /////////////////////////////////
        //CC1/CC2/CC3/CC4
        /////////////////////////////////
        //RTD_Log(LOGGER_INFO, "\n+++CCcParserCommand002", 0);
        CCcCaptionModeFunction();
    }
    else								//T1/T2/T3/T4
    {
#ifdef	CC_AUTO_TEST
        if (!CTtAutoTestFunction())
        {
            COsdMessagePush(_SHOW_CC_ERROR_MESSAGE);
            return;
        }
        else

#endif
            CCcTextModeFunction();
    }
}


static void CCcCaptionModeFunction(void)
{
    switch (CodeFormat)
    {
        //---------------------------------- unknown_code---------------------------------//
    case unknown_code:
    case afterunknown_code:
        return;
        //---------------------------------- pac_code---------------------------------//
    case pac_code:
        if (bCaptionText_Mode == 0)
        {
            VbiCc_bEOCnoRCLorPAC = FALSE;
            if ((CaptionMode == roll_up2) || (CaptionMode == roll_up3) || (CaptionMode == roll_up4))
            {
                RowIndex = RowPosition - 1;
                COsdVbiSetRullUpStartRow(RowIndex, RollUp_Mode);
            }
            else if (CaptionMode == paint_on)
            {
                usDispRow |= (UINT16)(1 << RowPosition);
                ucShowCntForPaintOn++;
                if (RowPosition > 8)
                {
                    COsdVbiClearRow(RowPosition - 8);
                    ucShowCntForPaintOn = 8;
                }
            }
            else if (CaptionMode == pop_on)
            {
            }
        }
        break;
        //---------------------------------- miscellaneous_code---------------------------------//
    case miscellaneous_code:
        switch (CcCommand)
        {
            //------------------Caption Mode-----------------//
        case rdc://resume direct captioning(paint-on)
            CCcInitialPaintOnCommand();
            break;

        case rcl://resume caption loading(pop-on)
            VbiCc_bEOCnoRCLorPAC = FALSE;
            CCcInitialPopOnCommand();
            break;

        case ru2://roll-up captions,2rows
        case ru3://roll-up captions,3rows
        case ru4://roll-up captions,4rows
            RowPosition = 0;
//			COsdVbiInitialVariable();
            CCcInitialRullUpCommand();
            break;
            //------------------Text Mode-----------------//
        case tr://text restart
        case rtd://resume text display
            bCaptionText_Mode = 1;
            break;
            //------------------Other command-----------------//
        case backspace://backspace
            CCcBackspaceCommand();
            break;
        case eoc://end of caption(flipmemories)
            if (CaptionMode ==pop_on)
                CCcEndOfCaptionCommand();
            else
            {
                COsdVbiClearAllMemory();
                VbiCc_bEOCnoRCLorPAC = TRUE;

                if (CaptionMode==0)
                    CaptionMode = pop_on;
                else
                    CaptionMode = 0;
            }

            //CaptionDispRowIndex2 = 0;	//jerry1109A
//			COsdVbiSwitchDisplayMemory();
//			COsdVbiClearNonDisplayMemory();
//			COsdVbiResetNonShowRow();
            break;
        case der://delete to end of row
#if _DER_COMMAND
            //--->w		 	CVbiDelete2EndOfRowCommand();
#endif
            break;
        case flash://flash on
            CCcSpaceCharacterToDisplayMem();
            break;
        case edm://erase displayed memory
            COsdVbiShowData(0);
            COsdVbiClearDisplayMemory();
            bCaptionText_Mode = 0;
            ColumnIndex = 0;
            usDispRow = 0x0000;
            ucShowCntForPaintOn = 0;
            break;
        case enm://erase nondisplayed memory
            COsdVbiClearNonDisplayMemory();
            bCaptionText_Mode = 0;
            break;
        case cr://carriage return
            if (bCaptionText_Mode==0)
            {
                CCcCarriageReturnCommand();
//				ColumnIndex = 0;
            }
            break;
        case to1://tab offset(1column)
// 	ColumnIndex+=1;
            TabOffset = 1;
            break;
        case to2://tab offset(2columns)
// 	ColumnIndex+=2;
            TabOffset = 2;
            break;
        case to3://tab offset(3columns)
// 	ColumnIndex+=3;
            TabOffset = 3;
            break;
        case reserved1:
        case reserved2:
        default:
            break;
        }
        break;
        //---------------------------------- midrow_code---------------------------------//
    case midrow_code:
        CCcSpaceCharacterToDisplayMem();
        break;
        //---------------------------------- xds_code---------------------------------//
    case xds_code:
        return;
//	 	break;
        //---------------------------------- optionalattr_code---------------------------------//
    case optionalattr_code:
        //CCcOptionalAttributeCodes();
        break;
    case fgattr_code:
        CCcOptionalAttributeCodes(fgattr_code);
        break;
    case bgattr_code:
        CCcOptionalAttributeCodes(bgattr_code);
        break;
        //---------------------------------- specialchar_code---------------------------------//
    case specialchar_code:
        if ((CaptionMode!=pop_on)||
                ((CaptionMode==pop_on)&&(VbiCc_bEOCnoRCLorPAC==FALSE)))
            CCcSpecialCharacterCodes();
        break;
        //---------------------------------- extrenedchar_Code---------------------------------//
    case extrenedchar_Code:
        if ((CaptionMode!=pop_on)||
                ((CaptionMode==pop_on)&&(VbiCc_bEOCnoRCLorPAC==FALSE)))
        {
            //CCcBackspaceCommand();
            CCcBackspace();
            CCcExternedCharacterCodes();
        }
        break;
        //---------------------------------- closegroup_code---------------------------------//
    case closegroup_code:
        break;
        //---------------------------------- character_code---------------------------------//
    case character_code:
        if ((CaptionMode!=pop_on)||
                ((CaptionMode==pop_on)&&(VbiCc_bEOCnoRCLorPAC==FALSE)))
            CCcInsertCharacterFunction();
        break;
    default:
        return;
    }

    fw_timer_event_ReactiveTimerEvent(SEC(_OSDVBI_CLOSESHOW_TIME),COsdEventCloseVbi);

    if (bCCDecodeStalbe==_FALSE)
        COsdEventCloseVbi();
}

static void CCcTextModeFunction(void)
{
    switch (CodeFormat)
    {

        //---------------------------------- miscellaneous_code---------------------------------//
    case miscellaneous_code:
        switch (CcCommand)
        {
            //------------------Caption Mode-----------------//
        case rdc://resume direct captioning(paint-on)
        case rcl://resume caption loading(pop-on)
        case ru2://roll-up captions,2rows
        case ru3://roll-up captions,3rows
        case ru4://roll-up captions,4rows
            bCaptionText_Mode = 0;
            break;
            //------------------Text Mode-----------------//
#if(_TEXT_RESTART == _TRUE)
        case tr://text restart
//			if(CaptionMode != text_restart) {
//				CTimerCancelTimerEvent(COsdEventCloseVbi);
//				COsdVbiClearDisplayMemory();
//				COsdVbiResetScrollUpEnv();
//				COsdVbiResetScrollRow();
//				COsdVbiSetShowRow(1,15);
//	 			CCcSetTextModeState();
//			}
//			bCaptionText_Mode = 1;
            CCcInitialTextModeCommand();
            //CVbiClearVariablesAndFlags();
            //--->w 			CVbiClearScreenMemory();	//jerry1109 start
//			COsdVbiInitialRow();
//			COsdVbiResetScrollRow();
//			COsdVbiSetNumberofRow(16);
//			COsdVbiClearAllMemory();
//			COsdVbiSetShowRow(0, 16);
//			CCcSetDefaultCharacterAttr();
//		 	CCcSetTextModeState();
            // RowIndex = 0;
            ColumnIndex = 0;			//jerry1109 end
            RowIndex = 1;
            CaptionMode = text_restart;
            break;
        case rtd://resume text display
//			if((CaptionMode != resume_text_display) || (CaptionMode != text_restart)) {
//				CTimerCancelTimerEvent(COsdEventCloseVbi);
//				COsdVbiClearDisplayMemory();
//				COsdVbiResetScrollUpEnv();
//				COsdVbiResetScrollRow();
//				COsdVbiSetShowRow(1,15);
//		 		CCcSetTextModeState();
//			}
            bCaptionText_Mode = 1;
            CCcSetDefaultCharacterAttr();
//			 CCcInitialTextModeCommand();
            //CVbiClearVariablesAndFlags();
            CaptionMode = resume_text_display;
            break;
#endif
            //------------------Other command-----------------//
        case backspace://backspace
            CCcBackspaceCommand();
//			CCcInsertCharacterFunction();
            break;
        case eoc://end of caption(flipmemories)
            break;
        case der://delete to end of row
#if _DER_COMMAND
            //--->w		 	CVbiDelete2EndOfRowCommand();
#endif
            break;
        case flash://flash on
            CCcSpaceCharacterToDisplayMem();
            break;
        case edm://erase displayed memory
        case enm://erase nondisplayed memory
            bCaptionText_Mode = 0;
            break;
        case cr://carriage return
            if (bCaptionText_Mode==1)
            {
                CCcCarriageReturnCommand();
                ColumnIndex = 0;
            }
            break;
        case to1://tab offset(1column)
            ColumnIndex+=1;
            break;
        case to2://tab offset(2columns)
            ColumnIndex+=2;
            break;
        case to3://tab offset(3columns)
            ColumnIndex+=3;
            break;
            //tdl case reserved1:
            //tdl case reserved2:
            //---------------------------------- unknown_code---------------------------------//
            //tdl case unknown_code:
            //tdl break;
            //---------------------------------- pac_code---------------------------------//
            //tdl case pac_code:
            //tdl break;
        default:
            break;
        }
        break;
        //---------------------------------- midrow_code---------------------------------//
    case midrow_code:
        CCcSpaceCharacterToDisplayMem();
        break;
        //---------------------------------- xds_code---------------------------------//
        //tdl case xds_code:
        //tdl	break;
        //---------------------------------- optionalattr_code---------------------------------//
    case optionalattr_code:
        //CCcOptionalAttributeCodes();
        break;
    case fgattr_code:
        CCcOptionalAttributeCodes(fgattr_code);
        break;
    case bgattr_code:
        CCcOptionalAttributeCodes(bgattr_code);
        break;
        //---------------------------------- specialchar_code---------------------------------//
    case specialchar_code:
        CCcSpecialCharacterCodes();
        break;
        //---------------------------------- extrenedchar_Code---------------------------------//
    case extrenedchar_Code:
        //CCcBackspaceCommand();
        CCcBackspace();
        CCcExternedCharacterCodes();
        break;
        //---------------------------------- closegroup_code---------------------------------//
    case closegroup_code:
        break;
        //---------------------------------- character_code---------------------------------//
    case character_code:
        CCcInsertCharacterFunction();
        break;
    default:
        break;
    }

#if 1//def _TEXTMODE_TIMEOUTE_ENABLE // when no signal or no CC data,T1~T4 disable OSD,hill added
    fw_timer_event_ReactiveTimerEvent(SEC(_OSDVBI_CLOSESHOW_TIME),COsdEventCloseVbi);
#endif

}

static void CCcSetCaptionModeState(void)
{
    UINT8 tmp;

    tmp = (CC_SYSTEM_FLAG & 0x18) >> 3;

    switch (tmp)
    {
    case 0x00:
        CCSystemState = cc1_state;
        break;
    case 0x01:
        CCSystemState = cc2_state;
        break;
    case 0x02:
        CCSystemState = cc3_state;
        break;
    case 0x03:
        CCSystemState = cc4_state;
        break;
    default:
        break;
    }

}

static void CCcSetTextModeState(void)
{
    UINT8 tmp;

    tmp = (CC_SYSTEM_FLAG & 0x18) >> 3;

    switch (tmp)
    {
    case 0x00:
        CCSystemState = t1_state;
        break;
    case 0x01:
        CCSystemState = t2_state;
        break;
    case 0x02:
        CCSystemState = t3_state;
        break;
    case 0x03:
        CCSystemState = t4_state;
        break;
    default:
        break;
    }

}

static void CCcSetDefaultCharacterAttr(void)
{
    Color_F = white;
    Color_B = black;
    CC_ATTRIBUTE_FLAG = 0;
    Ident = 0;
    Color_FS = (Color_FS<<4 | Color_F);	//jerry0519
    Color_BS = (Color_BS<<4 | Color_B);	//jerry0519
    PreAttribute |= 0x80;				//jerry0519
    //RowPosition = 0;	//jerry0330
    Variable_BitClear(CC_SYSTEM_FLAG,INSERTCHARACTER_FLAG);
//	Variable_BitClear(CC_ATTRIBUTE_FLAG,UNDERLINE_FLAG);
//	Variable_BitClear(CC_ATTRIBUTE_FLAG,FLASHON_FLAG);
//	Variable_BitClear(CC_ATTRIBUTE_FLAG,ITALICS_FLAG);
}

static void CCcInitialPaintOnCommand(void)
{
    if (CaptionMode != paint_on)
    {
        COsdVbiInitialVariable();
        COsdVbiResetScrollRow();
        COsdVbiSetShowRow(1,15);
        CCcSetCaptionModeState();
        COsdFxCloseWindow(7);
        CaptionMode = paint_on;
    }
    usDispRow = 0x0000;
    ucShowCntForPaintOn = 0;
    COsdFxCloseWindow(7);
    CCcSetDefaultCharacterAttr();
    bCaptionText_Mode = 0;
}

static void CCcInitialPopOnCommand(void)
{
    if (CaptionMode != pop_on)
    {
        //COsdVbiShowData(0); //tommy delete20100716
        COsdVbiInitialVariable();
        COsdVbiResetScrollRow();
        COsdVbiSetShowRow(1,15);
        CCcSetCaptionModeState();
//		COsdFxCloseWindow(7);
        CaptionMode = pop_on;
    }
    bCaptionText_Mode = 0;
    COsdFxCloseWindow(7);
    CCcSetDefaultCharacterAttr();
}

static void CCcInitialRullUpCommand(void)
{
    switch (CcCommand)
    {
    case ru2://roll-up captions,2rows
        if (CaptionMode != roll_up2)
        {
            COsdVbiResetScrollUpEnv();
            COsdVbiResetScrollRow();
            CCcSetCaptionModeState();
            Caption_RollUp = 1;
            RollUp_Mode = 2;
            CaptionMode = roll_up2;
        }
        bCaptionText_Mode = 0;
        CCcSetDefaultCharacterAttr();
        break;
    case ru3://roll-up captions,3rows
        if (CaptionMode != roll_up3)
        {
            COsdVbiResetScrollUpEnv();
            COsdVbiResetScrollRow();
            CCcSetCaptionModeState();
            Caption_RollUp = 1;
            RollUp_Mode = 3;
            CaptionMode = roll_up3;
        }
        bCaptionText_Mode = 0;
        CCcSetDefaultCharacterAttr();
        break;
    case ru4://roll-up captions,4rows
        if (CaptionMode != roll_up4)
        {
            COsdVbiResetScrollUpEnv();
            COsdVbiResetScrollRow();
            CCcSetCaptionModeState();
            Caption_RollUp = 1;
            RollUp_Mode = 4;
            CaptionMode = roll_up4;
        }
        bCaptionText_Mode = 0;
        CCcSetDefaultCharacterAttr();
        break;
    };
}

static void CCcInitialTextModeCommand(void)
{
//	if(CaptionMode != resume_text_display) {
//		CTimerCancelTimerEvent(COsdEventCloseVbi);
    COsdVbiClearDisplayMemory();
    COsdVbiResetScrollUpEnv();
    COsdVbiResetScrollRow();
    COsdVbiSetShowRow(1,15);
    CCcSetTextModeState();
//	}
    bCaptionText_Mode = 1;
    CCcSetDefaultCharacterAttr();
}

static void CCcInsertCharacterFunction(void)
{
    if (CCData1 == 0 && CCData2 == 0)
        return;
    switch (CaptionMode)
    {
    case roll_up2:
    case roll_up3:
    case roll_up4:
        if (bCaptionText_Mode == 0  && (CC_System_Mode&0x33)!=0)
        {
            if (Caption_RollUp)
            {
                if (RowPosition == 0)
                {
                    RowPosition = 15;
                    RowIndex = RowPosition - 1;
                    COsdVbiSetRullUpStartRow(RowIndex, RollUp_Mode);
                }
                Caption_RollUp = 0;
            }
            CCcRullUpInsertCharacterFunction();
        }
        break;

    case paint_on:
        if (bCaptionText_Mode==0 && (CC_System_Mode&0x33)!=0)
            CCcPaintOnInsertCharacterFunction();
        break;

    case pop_on:
        if (bCaptionText_Mode==0 && (CC_System_Mode&0x33)!=0)
            CCcPopOnInsertCharacterFunction();
        break;

    case text_restart:
    case resume_text_display:
        if (bCaptionText_Mode==1  && (CC_System_Mode&0xCC)!=0)
            CCcTextModeInsertCharacterFunction();
        break;
    default:
        break;
    }
}

static void CCcPopOnInsertChar(UINT8 data1, UINT8 color, UINT8 attr)//, BIT index)
{
    if (ColumnIndex < (column_length + 1))
    {
        CCcInsertCharacter2NonDisplayMem(RowPosition,ColumnIndex,data1,color, attr);
//		Variable_BitSet(CC_ATTRIBUTE_FLAG,DEFAULT_FLAG);
        if (!Variable_BitValue(CC_SYSTEM_FLAG,INSERTCHARACTER_FLAG))
        {
            CCcInsertCharacter2NonDisplayMem(RowPosition,ColumnIndex - 1, 0x20,color, attr);
            Variable_BitSet(CC_SYSTEM_FLAG,INSERTCHARACTER_FLAG);
        }
        CCcInsertCharacter2NonDisplayMem(RowPosition,ColumnIndex + 1, 0x20,color, attr);
//		Variable_BitClear(CC_ATTRIBUTE_FLAG,DEFAULT_FLAG);
        ColumnIndex++;
    }
    else
    {
        CCcInsertCharacter2NonDisplayMem(RowPosition,ColumnIndex + 1, 0x20,color, attr);
    }
}

static void CCcPopOnInsertCharacterFunction(void)
{
    UINT8 color1,attribute1;
    if (!Variable_BitValue(CC_SYSTEM_FLAG,INSERTCHARACTER_FLAG))
    {
        ColumnIndex =  1 + Ident*4 + TabOffset;
        Ident = 0;
        TabOffset = 0;

        if (RowPosition == row0)
            RowPosition = row15;
    }

    color1 = (Color_F&0x0F)+((Color_B<<4)&0x70);
    attribute1 = CC_ATTRIBUTE_FLAG;

    if (ColumnIndex == 0)
        ColumnIndex = 1;

    if (CCData1 != 0)
    {
        CCcPopOnInsertChar(CCData1, color1, attribute1);
        VbiCcOsd_InsertReferNowPreSpace(RowPosition,ColumnIndex-2, color1, attribute1);
    }

    if (CCData2 != 0)
    {
        CCcPopOnInsertChar(CCData2, color1, attribute1);
        VbiCcOsd_InsertReferNowPreSpace(RowPosition,ColumnIndex-2, color1, attribute1);

    }
}

static void CCcInsertCharacter2NonDisplayMem(UINT8 row,UINT8 column,UINT8 data1,UINT8 color,UINT8 attribute)
{
    UINT8	Attribute1;

    if (data1 == 0)
        return;

    if (data1<0x20 || data1>0xCF)
        data1 = 0x20;

    if (column > 0x21)	//jerry1104
        column = 0x21;

    // Added by weihao because of avoid show !
#if 0//tommy delete20100705 ,avoid show !
    if (data1==0x21) //Switch Ascii font index(0x01)/(0xA9)
        data1 = 0xC9;

    else if (data1==0xC9)
        data1 = 0x21;
#endif

#if 1//(_VBI_ITALIC_ENABLE == _TRUE)
    if (Variable_BitValue(CC_ATTRIBUTE_FLAG,FLASHON_FLAG))
        Attribute1 = 0x00;
    else
        Attribute1 = attribute;
    if (Variable_BitValue(CC_ATTRIBUTE_FLAG,ITALICS_FLAG))
        Attribute1 = 0x80;
    else
        Attribute1 = 0x7F;
#else
    if (Variable_BitValue(CC_ATTRIBUTE_FLAG,FLASHON_FLAG))
        Attribute1 = 0x00;
    else
        Attribute1 = attribute;
#endif

#if _LR_COL_MORE_PIXEL
    Attribute1 |= 0x10;
#endif

    if (Variable_BitValue(CC_ATTRIBUTE_FLAG,DEFAULT_FLAG))
        Attribute1 &= 0xFC;
    else
    {
        if (Variable_BitValue(CC_ATTRIBUTE_FLAG,UNDERLINE_FLAG))
            Attribute1 |= 0x01;
        else
            Attribute1 &= 0xFE;
        if (Variable_BitValue(CC_ATTRIBUTE_FLAG,FLASHON_FLAG))
            Attribute1 |= 0x02;
        else
            Attribute1 &= 0xFD;
    }

    if ((CC_System_Mode&0x33)!=0)
    {
        if ((color & 0xf0) == 0)
            color = color | 0x80;
    }

#ifdef _TRANSPARENT_ENABLE
    if (Variable_BitValue(CC_ATTRIBUTE_FLAG,TRANSPARENT_FLAG))
        color &= 0x0F;
#endif

    if (column==33)		//jerry1110C
        Attribute1 = 0;

    COsdVbiSetNonDisplayMemory(row, column, Attribute1, color, data1);
}

static void CCcSpaceCharacterToDisplayMem(void)
{
    CCData1 = 0x20;
    CCData2 = 0;
    CCcInsertCharacterFunction();
}

static void CCcPaintOnInsertChar(UINT8 data1, UINT8 color, UINT8 attr)
{
    if (ColumnIndex < column_length)
    {
        CCcInsertCharacter2DisplayMem(RowIndex,ColumnIndex,data1,color,attr);
//		Variable_BitSet(CC_ATTRIBUTE_FLAG,DEFAULT_FLAG);
        if (!Variable_BitValue(CC_SYSTEM_FLAG,INSERTCHARACTER_FLAG))
        {
            Variable_BitSet(CC_SYSTEM_FLAG,INSERTCHARACTER_FLAG);
            CCcInsertCharacter2DisplayMem(RowIndex,ColumnIndex-1,0x20,color,attr);
        }								//right-side
        CCcInsertCharacter2DisplayMem(RowIndex,ColumnIndex+1,0x20,color,attr);
//		Variable_BitClear(CC_ATTRIBUTE_FLAG,DEFAULT_FLAG);
        ColumnIndex++;
    }
    else
    {
        CCcInsertCharacter2DisplayMem(RowIndex,column_length,data1,color,attr);
        CCcInsertCharacter2DisplayMem(RowIndex,column_length+1,0x20,color,attr);//jerry1104
    }
}

static void CCcPaintOnInsertCharacterFunction(void)
{
    UINT8 color1,attribute1;

    if (!Variable_BitValue(CC_SYSTEM_FLAG,INSERTCHARACTER_FLAG))
    {
        // RowIndex = 0;
        ColumnIndex = 1 + Ident*4 + TabOffset;
        Ident = 0;
        TabOffset = 0;

        if (RowPosition == row0)
            RowPosition = row1;

        RowIndex = RowPosition; // - 1;
    }
    color1 = (Color_F&0x0F)+((Color_B<<4)&0x70);
    attribute1 = CC_ATTRIBUTE_FLAG;
    if (ColumnIndex == 0)
        ColumnIndex = 1;

    if (CCData1 != 0)
    {
        CCcPaintOnInsertChar(CCData1, color1, attribute1);
        VbiCcOsd_InsertReferNowPreSpace(RowIndex,ColumnIndex-2, color1, attribute1);
    }

    if (CCData2 != 0)
    {
        CCcPaintOnInsertChar(CCData2, color1, attribute1);
        VbiCcOsd_InsertReferNowPreSpace(RowIndex,ColumnIndex-2, color1, attribute1);
    }
}

static void CCcInsertCharacter2DisplayMem(UINT8 row,UINT8 column,UINT8 data1,UINT8 color,UINT8 attribute)
{
    UINT8	Attribute1;

    if (data1==0)
        return;

    if (data1<0x20 || data1>0xCF)
        data1 = 0x20;

    if (column >= 0x21)//jerry1104
        column = 0x21;

    // Added by weihao because of avoid show !
#if 0//tommy delete20100705 ,avoid show !
    if (data1==0x21) //Switch Ascii font index(0x01)/(0xA9)
        data1 = 0xC9;
    else if (data1==0xC9)
        data1 = 0x21;
#endif

#if 1//(_VBI_ITALIC_ENABLE == _TRUE)
    if (Variable_BitValue(CC_ATTRIBUTE_FLAG,FLASHON_FLAG))
        Attribute1 = 0x00;
    else
        Attribute1 = attribute;
    if (Variable_BitValue(CC_ATTRIBUTE_FLAG,ITALICS_FLAG))
        Attribute1 = 0x80;
    else
        Attribute1 = 0x7F;
#else
    if (Variable_BitValue(CC_ATTRIBUTE_FLAG,FLASHON_FLAG))
        Attribute1 = 0x00;
    else
        Attribute1 = attribute;

#endif

#if _LR_COL_MORE_PIXEL
    Attribute1 |= 0x10;
#endif
    if (Variable_BitValue(CC_ATTRIBUTE_FLAG,DEFAULT_FLAG))		//jerry0329
        Attribute1 &= 0xFC;
    else
    {
        if (Variable_BitValue(CC_ATTRIBUTE_FLAG,UNDERLINE_FLAG))
            Attribute1 |= 0x01;
        else
            Attribute1 &= 0xFE;
        if (Variable_BitValue(CC_ATTRIBUTE_FLAG,FLASHON_FLAG))
            Attribute1 |= 0x02;
        else
            Attribute1 &= 0xFD;
    }

    if ((CC_System_Mode&0x33)!=0)
    {
        if ((color & 0xf0) == 0)
            color = color | 0x80;
    }

#ifdef _TRANSPARENT_ENABLE
    if (Variable_BitValue(CC_ATTRIBUTE_FLAG,TRANSPARENT_FLAG))
        color &= 0x0F;
#endif

    RTD_Log(LOGGER_INFO, "\n+++CCcInsertCharacter2DisplayMemDa = %x ", (UINT32)data1);

    COsdVbiSetDisplayMemory(row, column, Attribute1, color, data1);

    COsdVbiShowData(1);
}


static void CCcRullUpInsertChar2Mem(UINT8 data1, UINT8 color, UINT8 attr)
{
    if (ColumnIndex < column_length)
    {
        CCcInsertCharacter2DisplayMem(RowPosition,ColumnIndex,data1,color,attr);
        Variable_BitSet(CC_ATTRIBUTE_FLAG,DEFAULT_FLAG);
        if (!Variable_BitValue(CC_SYSTEM_FLAG,INSERTCHARACTER_FLAG) || ColumnIndex == 1)
        {
            ///////////////////////////////////
            //Row Start Condition
            ///////////////////////////////////
            //
            Variable_BitSet(CC_SYSTEM_FLAG,INSERTCHARACTER_FLAG);

            CCcInsertCharacter2DisplayMem(RowPosition,ColumnIndex-1,0x20,color,attr);
        }					//right-side
        CCcInsertCharacter2DisplayMem(RowPosition,ColumnIndex+1,0x20,color,attr);//jerry0329
        Variable_BitClear(CC_ATTRIBUTE_FLAG,DEFAULT_FLAG);
        ColumnIndex++;
    }
    else
    {
        ///////////////////////////////////
        //Insert Character
        ///////////////////////////////////
        CCcInsertCharacter2DisplayMem(RowPosition,column_length,data1,color,attr);
        ///////////////////////////////////
        //Insert Space for next position
        ///////////////////////////////////
        CCcInsertCharacter2DisplayMem(RowPosition,column_length+1,0x20,color,attr);//jerry1104
    }
}


static void CCcRullUpInsertCharacterFunction(void)
{

    UINT8 color1,attribute1;

    if (!Variable_BitValue(CC_SYSTEM_FLAG,INSERTCHARACTER_FLAG))
    {
//		Variable_BitSet(CC_SYSTEM_FLAG,INSERTCHARACTER_FLAG);
        // RowIndex = 0;
//if(Ident_Flag){				//jerry0329 start
        ColumnIndex = 1 + Ident*4 + TabOffset;
        Ident = 0;
        TabOffset = 0;
//	Ident_Flag = 1;
//}							//jerry0329 end
    }

    color1 = (Color_F&0x0F)+((Color_B<<4)&0x70);
    attribute1 = CC_ATTRIBUTE_FLAG;
    if (ColumnIndex == 0)
        ColumnIndex = 1;


    if (CCData1 != 0)
    {
        CCcRullUpInsertChar2Mem(CCData1, color1, attribute1);
        VbiCcOsd_InsertReferNowPreSpace(RowPosition,ColumnIndex-2, color1, attribute1);
    }

    if (CCData2 != 0)
    {
        CCcRullUpInsertChar2Mem(CCData2, color1, attribute1);
        VbiCcOsd_InsertReferNowPreSpace(RowPosition,ColumnIndex-2, color1, attribute1);
    }

    //insert data to the same index
}

static void CCcSpecialCharacterCodes(void)
{
    //Special characters 	// 10h ~ 1Fh
    CCData1 = 0xc0 + (CCData2&0x0f);
    CCData2 = 0;
    CCcInsertCharacterFunction();
}

static void CCcExternedCharacterCodes(void)
{
    //Extend Characters 	//80h ~ BFh
    if (CCData1&0x01)
        CCData1 = 0xa0 + (CCData2-0x20);
    else
        CCData1 = 0x80 + (CCData2-0x20);
    CCData2 = 0;
    if (PreAttribute&0x40) 			//jerry0522
    {
        if (PreAttribute&0x80) 		//jerry0519
        {
            Color_F = Color_FS>>4;
            Color_FS = Color_F<<4;
        }
        else
        {
            if ((PreAttribute&0x3F)==flash)
                Variable_BitClear(CC_ATTRIBUTE_FLAG,FLASHON_FLAG);
            else if ((PreAttribute&0x3F)==rdc)//jerry0523
                Variable_BitClear(CC_ATTRIBUTE_FLAG,ITALICS_FLAG);
        }
        PreAttribute = 0x00;
    }
    CCcInsertCharacterFunction();
}

static void CCcOptionalAttributeCodes(UINT8 codeFormat)
{
#if 1
    //	UINT8 buf_length;
    //UINT8 color1;

    CCcBackspaceCommand();

//	buf_length = buffer_length - ColumnIndex;
    switch (codeFormat)
    {
    case bgattr_code:
        FirstDataByteFlag=1;
        break;
    case fgattr_code:
        FirstDataByteFlag=1;
        break;
    default:
        FirstDataByteFlag=0;
        break;
    }

    CCcSpaceCharacterToDisplayMem();
#else
//	UINT8 buf_length;
    UINT8 color1;

    if ((CC_System_Mode&0x01)==0x01)//tommy modify 20100713,only for cc1
        CCcBackspaceCommand();
//	buf_length = buffer_length - ColumnIndex;
    switch (CaptionMode)
    {
    case text_restart:
    case resume_text_display:
        color1 = (Color_F&0x0F);
//		CCcInsertCharacter2DisplayMem(RowIndex,ColumnIndex,0x20/*data1*/,color1,0x00/*attribute1*/);	//insert color-attr to end of row(jerry).
        break;
    case roll_up2:
    case roll_up3:
    case roll_up4:
    case paint_on:
    case pop_on:
    default:
        color1 = (Color_F&0x0F)+((Color_B<<4)&0x70);
        break;
    }
    if ((CC_System_Mode&0x01)==0x01) //tommy modify 20100713,only for cc1
        CCcSpaceCharacterToDisplayMem();
#endif
}

static void CCcBackspaceCommand(void)
{
    UINT8 color1,attribute1,data1;

    data1 = ' ';
    color1 = (Color_F&0x0F)+((Color_B<<4)&0x70);
    attribute1 = CC_ATTRIBUTE_FLAG | 0x04;

    if (ColumnIndex == 0)
        return;

    switch (CaptionMode)
    {
    case roll_up2:
    case roll_up3:
    case roll_up4:
        color1 = (Color_F&0x0F);
        ColumnIndex --;
        COsdVbiSetDisplayMemory(RowPosition,ColumnIndex, attribute1, color1, data1);
        //column 32
        color1 = (Color_F&0x0F)+((Color_B<<4)&0x70);
        if (ColumnIndex == column_length + 1)
        {
            ColumnIndex --;
            COsdVbiSetDisplayMemory(RowPosition,ColumnIndex, attribute1, color1, data1);
        }
        break;
    case paint_on:
        color1 = (Color_F&0x0F);
        ColumnIndex --;
        COsdVbiSetDisplayMemory(RowIndex,ColumnIndex+1, attribute1, color1, data1);
        COsdVbiSetDisplayMemory(RowIndex,ColumnIndex, attribute1, color1, data1);
        color1 = (Color_F&0x0F)+((Color_B<<4)&0x70);
        //column 32
        if (ColumnIndex == column_length + 1)
        {
            ColumnIndex --;
            COsdVbiSetDisplayMemory(RowIndex,ColumnIndex, attribute1, color1, data1);
        }
        break;
    case pop_on:
        ColumnIndex --;
        COsdVbiSetNonDisplayMemory(RowPosition,ColumnIndex+1, attribute1, color1, data1);
        COsdVbiSetNonDisplayMemory(RowPosition,ColumnIndex, attribute1, color1, data1);
        //ColumnIndex --;
        if (ColumnIndex == column_length + 1)
        {
            ColumnIndex --;
            COsdVbiSetNonDisplayMemory(RowPosition,ColumnIndex, attribute1, color1, data1);
        }
        break;
    case text_restart:
    case resume_text_display:
        ColumnIndex --;
        COsdVbiSetDisplayMemory(RowIndex, ColumnIndex, attribute1, color1, data1);
        //ColumnIndex --;
        //column 32
        if (ColumnIndex == column_length + 1)
        {
            ColumnIndex --;
            COsdVbiSetDisplayMemory(RowIndex, ColumnIndex, attribute1, color1, data1);
        }
        break;
    default:
        break;
    }
    Variable_BitSet(CC_SYSTEM_FLAG,INSERTCHARACTER_FLAG);

}

void CCcScrollRowOnTimer(void)
{
    if ((CaptionMode == paint_on) || (CaptionMode == pop_on))
        return;

    COsdVbiRullUpProc();

    //bCCInterrupt = 0;

}

static void CCcTextModeInsertChar(UINT8 data1, UINT8 color, UINT8 attr)
{
    if (ColumnIndex < column_length + 1)
        CCcInsertCharacter2DisplayMem(RowIndex, ColumnIndex++,data1,color,attr);
    else
        CCcInsertCharacter2DisplayMem(RowIndex, column_length,data1,color,attr);
}

static void CCcTextModeInsertCharacterFunction(void)
{
    UINT8 color1,attribute1;

    color1 = (Color_F&0x0F)+((Color_B<<4)&0x70);
    attribute1 = CC_ATTRIBUTE_FLAG;

    if (ColumnIndex == 0)
        ColumnIndex = 1;

    if (Ident) 				//jerry0329 start
    {
        ColumnIndex = 1 + Ident*4;
        Ident = 0;
    }

    if (TabOffset)
    {
        ColumnIndex += TabOffset;
        TabOffset = 0;
    }

    if (RowIndex == 0)
        RowIndex = 1;

    if (CCData1 != 0 && CCData1>=0x20)	//jerry0530
        CCcTextModeInsertChar(CCData1, color1, attribute1);

    if (CCData2 != 0 && CCData2>=0x20)	//jerry0530
        CCcTextModeInsertChar(CCData2, color1, attribute1);
}

static void CCcCarriageReturnCommand(void)
{
//	UINT8	ScrollRowIndex_Temp;//jerry0428

    Variable_BitSet(CC_SYSTEM_FLAG,INSERTCHARACTER_FLAG);
    switch (CaptionMode)
    {
    case roll_up2:
        //COsdVbiSetScrollRow();
        //ColumnIndex = 0;

//		COsdVbiClearRow(RowPosition - 3);

//---->w	 	CVbiCarriageReturnCmd(1);
//---->w	 	 		ScrollRowIndex_Temp = (ScrollRowIndex+15)&0x1F;//jerry1104
        //RTD_EraseOneRowOfDDRMem(0,3,8,_VBI_DDR_START_ADDR + ScrollRowIndex_Temp*(0x20));	//jerry1104
        //break;
    case roll_up3:
        //COsdVbiSetScrollRow();
        //ColumnIndex = 0;

//		COsdVbiClearRow(RowPosition - 4);

//---->w	 		 	CVbiCarriageReturnCmd(2);
//---->w	 	 		ScrollRowIndex_Temp = (ScrollRowIndex+15)&0x1F;//jerry1104
        //RTD_EraseOneRowOfDDRMem(0,3,8,_VBI_DDR_START_ADDR + ScrollRowIndex_Temp*(0x20));	//jerry1104
        //break;
    case roll_up4:
        COsdVbiSetScrollRow();
        ColumnIndex = 0;
//		COsdVbiClearRow(RowPosition - 5);

//---->w	 		 	CVbiCarriageReturnCmd(3);
//---->w	 	 		ScrollRowIndex_Temp = (ScrollRowIndex+15)&0x1F;//jerry1104
        //RTD_EraseOneRowOfDDRMem(0,3,8,_VBI_DDR_START_ADDR + ScrollRowIndex_Temp*(0x20));	//jerry1104
        CCcSetDefaultCharacterAttr();
        break;
    case text_restart:
    case resume_text_display:

        CCcSetDefaultCharacterAttr();

        if (RowIndex < 15)
        {
            if ((RowIndex + 1) == 15)
                COsdVbiSetRullUpStartRow(15, 15);

            RowIndex++;
        }
        else
            COsdVbiSetScrollRow();
        ColumnIndex = 0;
//---->w	 		 	CVbiSetDefaultCharacterAttr();
//		if
//---->w	 		 	if(RowIndex == row15 - 1){
//---->w	 		 		CVbiTextModeScrolling();

//---->w	 				ScrollRowIndex_Temp = (ScrollRowIndex+15)&0x1F;	//jerry0428
//---->w	 				COsdVbiClearRow(15);
//			COsdVbiClearRow(RowIndex);
//			COsdFxCalcStartAddress(0, 0, _WRITE_BYTE1);
//			CScalerWriteAmount(_OSD_DATA_PORT_92, 34 * 3, 0x00, _NON_AUTOINC);
//			RTD_EraseOneRowOfDDRMem(0,3,8,_VBI_DDR_START_ADDR + ScrollRowIndex_Temp*(0x20));	//jerry0503A
//---->w	 			}
//---->w	 		 	else{
//---->w	 		 		RowIndex++;
//---->w	 				ColumnIndex = 0;
//---->w	 		 	}
        break;
    case paint_on:
    case pop_on:
    default:
        break;
    }
}

static void CCcEndOfCaptionCommand(void)
{
    switch (CaptionMode)
    {
    case roll_up2:
    case roll_up3:
    case roll_up4:
    case paint_on:
        break;
    case pop_on:
        COsdVbiSwitchDisplayMemory();
        COsdVbiClearNonDisplayMemory();
        break;
    case text_restart:
    case resume_text_display:
        RowIndex = 0;
        ColumnIndex = 0;
        COsdVbiResetScrollRow();
    default:
        break;
    }
//	CCcInitialPopOnCommand();
    COsdVbiSetShowRow(1,15);
    CaptionMode = pop_on;
    bCaptionText_Mode = 0;
}

static void CCcBackspace(void)
{
    UINT8 color1,attribute1,data1;

    data1 = ' ';
    color1 = (Color_F&0x0F)+((Color_B<<4)&0x70);
    attribute1 = CC_ATTRIBUTE_FLAG | 0x04;

    if (ColumnIndex == 0)
        return;

    switch (CaptionMode)
    {
    case roll_up2:
    case roll_up3:
    case roll_up4:
        color1 = (Color_F&0x0F);
        COsdVbiSetDisplayMemory(RowPosition,ColumnIndex, attribute1, color1, data1);
        ColumnIndex --;
        //column 32
        color1 = (Color_F&0x0F)+((Color_B<<4)&0x70);
        if (ColumnIndex == column_length + 1)
        {
            COsdVbiSetDisplayMemory(RowPosition,ColumnIndex, attribute1, color1, data1);
            ColumnIndex --;
        }
        break;
    case paint_on:
        color1 = (Color_F&0x0F);
        COsdVbiSetDisplayMemory(RowIndex,ColumnIndex, attribute1, color1, data1);
        ColumnIndex --;
        color1 = (Color_F&0x0F)+((Color_B<<4)&0x70);
        //column 32
        if (ColumnIndex == column_length + 1)
        {
            COsdVbiSetDisplayMemory(RowIndex,ColumnIndex, attribute1, color1, data1);
            ColumnIndex --;
        }
        break;
    case pop_on:
        COsdVbiSetNonDisplayMemory(RowPosition,ColumnIndex, attribute1, color1, data1);
        ColumnIndex --;
        if (ColumnIndex == column_length + 1)
        {
            COsdVbiSetNonDisplayMemory(RowPosition,ColumnIndex, attribute1, color1, data1);
            ColumnIndex --;
        }
        break;
    case text_restart:
    case resume_text_display:
        COsdVbiSetDisplayMemory(RowIndex, ColumnIndex, attribute1, color1, data1);
        ColumnIndex --;
        //column 32
        if (ColumnIndex == column_length + 1)
        {
            COsdVbiSetDisplayMemory(RowIndex, ColumnIndex, attribute1, color1, data1);
            ColumnIndex --;
        }
        break;
    default:
        break;
    }
    Variable_BitSet(CC_SYSTEM_FLAG,INSERTCHARACTER_FLAG);

}

void VbiCcOsd_InsertReferNowPreSpace( UINT8 row,UINT8 column,UINT8 color,UINT8 attr)
{
    UINT8 tempcol = 0;

    if (FirstDataByteFlag != 1)
        return;

    if ((CC_System_Mode&0x33)==0) //onlye cc1/cc2/cc3/cc4 funciton the below
        return;

    if (column>column_length)
        return;

    //if((CodeFormat==fgattr_code)||(CodeFormat==bgattr_code)||(CodeFormat==optionalattr_code))
    //return;
    if ((CodeFormat!=character_code)&&(CodeFormat!=extrenedchar_Code)&&(CodeFormat!=specialchar_code))
        return;

    FirstDataByteFlag=0;

    switch (CaptionMode)
    {
    case roll_up2:
    case roll_up3:
    case roll_up4:
    case paint_on:
        if (column == 0)
            CCcInsertCharacter2DisplayMem(row,column,0x20,color,attr);
        else if (column == 1)
        {
            CCcInsertCharacter2DisplayMem(row,tempcol,0x20,color,attr);
            tempcol = 1;
            CCcInsertCharacter2DisplayMem(row,tempcol,0x20,color,attr);
        }
        else if (column == 2)
        {
            CCcInsertCharacter2DisplayMem(row,tempcol,0x20,color,attr);
            tempcol = 1;
            CCcInsertCharacter2DisplayMem(row,tempcol,0x20,color,attr);
            tempcol = 2;
            CCcInsertCharacter2DisplayMem(row,tempcol,0x20,color,attr);
        }
        else
            CCcInsertCharacter2DisplayMem(row,column,0x20,color,attr);
        break;

    case pop_on:
        if (column == 0)
            CCcInsertCharacter2NonDisplayMem(row,column,0x20,color,attr);
        else if (column == 1)
        {
            CCcInsertCharacter2NonDisplayMem(row,tempcol,0x20,color,attr);
            tempcol = 1;
            CCcInsertCharacter2NonDisplayMem(row,tempcol,0x20,color,attr);
        }
        else if (column == 2)
        {
            CCcInsertCharacter2NonDisplayMem(row,tempcol,0x20,color,attr);
            tempcol = 1;
            CCcInsertCharacter2NonDisplayMem(row,tempcol,0x20,color,attr);
            tempcol = 2;
            CCcInsertCharacter2NonDisplayMem(row,tempcol,0x20,color,attr);
        }
        else
            CCcInsertCharacter2NonDisplayMem(row,column,0x20,color,attr);
        break;

    default:
        break;
    }

}

//leo 20060430
#ifdef CC_AUTO_TEST
static UINT8 CCcAutoTestFunction(void)
{
    UINT8 result,i;

    if ((CC_System_Mode&0x01)!=0)			//cc1 type
    {
        for (i=0; i<CC1_table_max; i++)
        {
            if ((CCData1==CC1_table[i][0])&&(CCData2==CC1_table[i][1]))
            {
                result = 1;
                break;
            }
        }
        if (i >= CC1_table_max) result = 0;
    }

    else if ((CC_System_Mode&0x02)!=0)	//cc2 type
    {
        for (i=0; i<CC2_table_max; i++)
        {
            if ((CCData1==CC2_table[i][0])&&(CCData2==CC2_table[i][1]))
            {
                result = 1;
                break;
            }
        }
        if (i >= CC2_table_max) result = 0;
    }
    //leo 20060509
    else if ((CC_System_Mode&0x10)!=0)	//cc3 type
    {
        for (i=0; i<CC3_table_max; i++)
        {
            if ((CCData1==CC3_table[i][0])&&(CCData2==CC3_table[i][1]))
            {
                result = 1;
                break;
            }
        }
        if (i >= CC3_table_max) result = 0;
    }
    else if ((CC_System_Mode&0x20)!=0)	//cc4 type
    {
        for (i=0; i<CC4_table_max; i++)
        {
            if ((CCData1==CC4_table[i][0])&&(CCData2==CC4_table[i][1]))
            {
                result = 1;
                break;
            }
        }
        if (i >= CC4_table_max) result = 0;
    }
    return result;
}

//leo 20060509
static UINT8 CTtAutoTestFunction(void)
{

    UINT8 result,i;

    if ((CC_System_Mode&0x04)!=0)			//cc1 type
    {
        for (i=0; i<T1_table_max; i++)
        {
            if ((CCData1==T1_table[i][0])&&(CCData2==T1_table[i][1]))
            {
                result = 1;
                break;
            }
        }
        if (i >= T1_table_max) result = 0;
    }
    //leo 20060510
    else if ((CC_System_Mode&0x08)!=0)	//cc2 type
    {
        for (i=0; i<T2_table_max; i++)
        {
            if ((CCData1==T2_table[i][0])&&(CCData2==T2_table[i][1]))
            {
                result = 1;
                break;
            }
        }
        if (i >= T2_table_max) result = 0;
    }

    else if ((CC_System_Mode&0x40)!=0)	//cc3 type
    {
        for (i=0; i<T3_table_max; i++)
        {
            if ((CCData1==T3_table[i][0])&&(CCData2==T3_table[i][1]))
            {
                result = 1;
                break;
            }
        }
        if (i >=T3_table_max) result = 0;
    }
    else if ((CC_System_Mode&0x80)!=0)	//cc4 type
    {
        for (i=0; i<T4_table_max; i++)
        {
            if ((CCData1==T4_table[i][0])&&(CCData2==T4_table[i][1]))
            {
                result = 1;
                break;
            }
        }
        if (i >= T4_table_max) result = 0;
    }
    return result;

}
#endif

#endif
/**
*
* @}
*/









