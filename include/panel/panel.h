//
//
#ifndef _PANEL_H
#define _PANEL_H

//#define CONFIG_PANEL_AU_M170EG01

//--------------------------------------------------
// Definitions for Panel Settings
//--------------------------------------------------
#define _PANEL_INTERFACE_TTL                              0
#define _PANEL_INTERFACE_LVDS                             1
#define _PANEL_INTERFACE_RSDS                             2

//--------------------------------------------------
// Set Last Line Finetune Method
//--------------------------------------------------
#define _LAST_LINE_METHOD_NONE                  0
#define _LAST_LINE_METHOD_0                     1                   // Formula: Tne DEN End to DEN Start Length must be (1280*N)
#define _LAST_LINE_METHOD_1                     2                   // Formula: Tne DEN End to DEN Start Length must be (4096*N + 128)
#define _LAST_LINE_METHOD_2                     3                   // Formula: Set to target last line.


#ifdef CONFIG_PANEL_AU_M170EG01
#include "au_m170eg01.h"
#endif

#ifdef CONFIG_PANEL_AU170_480p
#include "au_m170eg01_480p.h"
#endif

#ifdef CONFIG_PANEL_AUM22001AGCL
#include	"au_m22001AGCL.h"
#endif

#ifdef CONFIG_PANEL_AUM240HW01_V0
#include "au_m240HW01_V0.h"
#endif

#ifdef CONFIG_PANEL_AUT315XW01
#include "au_t315xw01.h"
#endif

#ifdef CONFIG_PANEL_AUT32PV320TVM  //For Proview AU32" panel//2007-12-28 12:40
#include "au_pv320tvm.h"
#endif

#ifdef CONFIG_PANEL_AUT37PV370TVM  //For Proview AU37" panel//2008-1-24
#include "au_pv370tvm.h"
#endif

#ifdef CONFIG_PANEL_CMO_V420H1
#include "cmo_v420h1.h"
#endif

#ifdef CONFIG_PANEL_LG_47
#include "lg_47.h"
#endif

#ifdef CONFIG_PANEL_LG_32
#include "lg_32.h"
#endif

#ifdef CONFIG_PANEL_LG_37
#include "lg_37.h"
#endif


#ifdef CONFIG_PANEL_SAMSUNG_32
#include "samsung_32.h"
#endif

#ifdef CONFIG_PANEL_SAMSUNG_46
#include "samsung_46.h"
#endif

#ifdef CONFIG_PANEL_HT_32
#include "ht_32.h"
#endif

#ifdef CONFIG_PANEL_CMO_V420B1_LH1
#include "cmo_v420b1_lh1.h"
#endif

#ifdef CONFIG_PANEL_AU_M220EW01//y
#include "au_m220ew01.h"
#endif

#ifdef CONFIG_PANEL_CMO_V320B1L02
#include "cmo_v320b1l01.h"
#endif

#ifdef CONFIG_PANEL_CMO_V260B1L02
#include "cmo_v260b1l01.h"
#endif

#ifdef	CONFIG_PANEL_CMO_V216
#include "cmo_v216.h"
#endif

#ifdef CONFIG_PANEL_LG_V420V02
#include "lg_lc420v02.h"
#endif

#ifdef CONFIG_PANEL_LG_420WUNSAA1
#include	"lg_42_420WUN_SAA1.h"
#endif

#ifdef CONFIG_PANEL_CMO_V315B3L01
#include "cmo_v315b3l01.h"
#endif

#ifdef CONFIG_PANEL_CMO_V320B1L07
#include "cmo_v320b1l07.h"
#endif

#ifdef CONFIG_PANEL_CMO_M190A1L02
#include "cmo_m190a1_l02.h"
#endif

#ifdef CONFIG_PANEL_AU_M190PW01
#include "au_m190pw01.h"
#endif

#ifdef CONFIG_PANEL_CPT_32
#include "cpt_32.h"
#endif

#ifdef CONFIG_PANEL_MT_19AW01
#include	"mt19aw01.h"
#endif

#ifdef CONFIG_PANEL_HANNSTAR_HSD250MUW1
#include	"hannstar_hsd250muw1.h"
#endif

#ifdef CONFIG_PANEL_HANNSTAR_HSD280MUW1
#include	"hannstar_hsd280muw1.h"
#endif

#ifdef CONFIG_PANEL_CMO_22M220J1L01
#include	"cmo_22_m220j1_l01.h"
#endif

#ifdef CONFIG_PANEL_CMO_M216H1_L01
#include	"cmo_m216h1_l01.h"
#endif

#ifdef CONFIG_PANEL_CMO_N116B6_L02
#include	"cmo_n116b6_l02.h"
#endif


#ifdef CONFIG_PANEL_LVDS_1280x1024
#include	"PNL_LVDS_1280x1024.h"
#endif

#ifdef CONFIG_PANEL_LVDS_1280x800
#include	"PNL_LVDS_1280x800.h"
#endif

#ifdef CONFIG_PANEL_LVDS_1280x800
#include	"PNL_LVDS_1280x800.h"
#endif 

#ifdef CONFIG_PANEL_SHARP_LQ104W1LH02
#include	"SHARP_LQ104W1LH02.h"
#endif 

#ifdef CONFIG_PANEL_LVDS_1024x768
#include	"PNL_LVDS_1024x768.h"
#endif 
#ifdef CONFIG_PANEL_LVDS_1024x600
#include	"PNL_LVDS_1024x600.h"
#endif 

#ifdef CONFIG_PANEL_LVDS_800x600
#include	"PNL_LVDS_800x600.h"
#endif

#ifdef CONFIG_PANEL_TTL_640x480
#include	"PNL_TTL_640x480.h"
#endif
#ifdef CONFIG_PANEL_TTL_1024x600
#include	"PNL_TTL_1024x600.h"
#endif
#ifdef CONFIG_PANEL_TTL_800x480
#include	"PNL_TTL_800x480.h"
#endif

#endif // _PANEL_H


