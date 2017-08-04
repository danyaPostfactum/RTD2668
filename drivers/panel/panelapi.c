/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2009
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is for rtdaccess related functions.
 *
 * @author 	$Author$
 * @date	$Date$
 * @version	$Revision$
 * @ingroup	rtdaccess
 */

/**
 * @addtogroup rtdaccess
 * @{
 */
#include "autoconf.h"
#include "panel\panelapi.h"
#include "panel\panel.h"

UINT16 flow_Get_Panel_Parameter(UINT16 offset, UINT8 len)
{
    UINT16* ptVal;
    ptVal = (UINT16*)((UINT8*)&stPanelData + offset);
    if (len == 1)
    {
        return ((*ptVal & 0xff00) >> 8);
    }
    else if (len == 2)
    {
        return *ptVal;
    }
    else
        return 0;
}
#if 0
UINT32 Get_DISPLAY_PORT()
{
    return CONFIG_DISPLAY_PORT;
}

UINT32 Get_DISPLAY_COLOR_BITS()
{
    return CONFIG_DISPLAY_COLOR_BITS;
}

UINT32 Get_DISPLAY_EVEN_RSV1_BIT()
{
    // 0: Indicate 0, 1: Indicate 1
// 	return 0;
    return CONFIG_DISPLAY_EVEN_RSV1_BIT;
}

UINT32 Get_DISPLAY_ODD_RSV1_BIT()
{
    // return 0;
    return CONFIG_DISPLAY_ODD_RSV1_BIT;
} 	// 0: Indicate 0, 1: Indicate 1

UINT32 Get_DISPLAY_BITMAPPING_TABLE()
{
    return CONFIG_DISPLAY_BITMAPPING_TABLE;
} // 0: Table1, 1:Table2


UINT32 Get_DISPLAY_PORTAB_SWAP()
{
    return CONFIG_DISPLAY_PORTAB_SWAP ; // inverse
}	// 0: No Swap, 1: Swap

UINT32 Get_DISPLAY_RED_BLUE_SWAP()
{
    return CONFIG_DISPLAY_RED_BLUE_SWAP;
}	// 0: No Swap, 1: Swap

UINT32 Get_DISPLAY_MSB_LSB_SWAP()
{
    return CONFIG_DISPLAY_MSB_LSB_SWAP;
}	// 0: No Swap, 1: Swap

UINT32 Get_DISPLAY_SKEW_DATA_OUTPUT()
{
    // return 0 ;
    return CONFIG_DISPLAY_SKEW_DATA_OUTPUT;
} 	// 0: Disable, 1: Skew data output


UINT32 Get_DISPLAY_OUTPUT_INVERSE()
{
    return CONFIG_DISPLAY_OUTPUT_INVERSE;
}	// 0: No Swap, 1: Swap
/*
UINT32 Get_DISPLAY_CLOCK_DELAY()
{
	return CONFIG_DISPLAY_CLOCK_DELAY;
 }	// 0: No Swap, 1: Swap
*/

/*
 * Display Sync Output polarity
 */

UINT32 Get_DISPLAY_VERTICAL_SYNC_NORMAL()
{
    //return 1;
    return CONFIG_DISPLAY_VERTICAL_SYNC_NORMAL;
}

UINT32 Get_DISPLAY_HORIZONTAL_SYNC_NORMAL()
{
    // return 1;
    return CONFIG_DISPLAY_HORIZONTAL_SYNC_NORMAL;
}

UINT32 Get_DISPLAY_RATIO_4X3()
{
    return CONFIG_DISPLAY_RATIO_4X3;
}

/*
UINT32 Get_DISPLAY_CLOCK_MAX()
{
	//return 119;
	return CONFIG_DISPLAY_CLOCK_MAX;
}
*/
UINT32 Get_DISPLAY_REFRESH_RATE()
{
    //return 75;
    return CONFIG_DISPLAY_REFRESH_RATE;
}

/*
 * Display total window setup
 */

UINT32 Get_DISP_HORIZONTAL_TOTAL()
{
    return (CONFIG_DISP_HORIZONTAL_TOTAL&(~_BIT0));
}


UINT32 Get_DISP_VERTICAL_TOTAL()
{
    return CONFIG_DISP_VERTICAL_TOTAL;
}


/*
 * Display Sync Width setup
 */
UINT32 Get_DISP_HSYNC_WIDTH()
{
    return CONFIG_DISP_HSYNC_WIDTH;
}

UINT32 Get_DISP_VSYNC_LENGTH()
{
    return CONFIG_DISP_VSYNC_LENGTH;
}

/*
 * Display Enable window setup
 */
UINT32 Get_DISP_DEN_STA_HPOS()
{
    return CONFIG_DISP_DEN_STA_HPOS;
}

UINT32 Get_DISP_DEN_END_HPOS()
{
    return CONFIG_DISP_DEN_END_HPOS;
}
UINT32 Get_DISP_DEN_STA_VPOS()
{
    return CONFIG_DISP_DEN_STA_VPOS;
}

UINT32 Get_DISP_DEN_END_VPOS()
{
    return CONFIG_DISP_DEN_END_VPOS;
}

/*
 * Display active window setup
 */
UINT32 Get_DISP_ACT_STA_HPOS()
{
    return CONFIG_DISP_ACT_STA_HPOS;
}

UINT32 Get_DISP_ACT_END_HPOS()
{
    return CONFIG_DISP_ACT_END_HPOS;
}

UINT32 Get_DISP_ACT_STA_VPOS()
{
    return CONFIG_DISP_ACT_STA_VPOS;
}
UINT32 Get_DISP_ACT_END_VPOS()
{
    return CONFIG_DISP_ACT_END_VPOS;
}

UINT32 Get_DISP_HSYNC_LASTLINE()
{
    return CONFIG_DISP_HSYNC_LASTLINE;
}

UINT32 Get_DISP_DCLK_DELAY()
{
    // retunr 0;
    return CONFIG_DISP_DCLK_DELAY;
    CONFIG_DISPLAY_CLOCK_DELAY
}


UINT32 Get_DISP_ACT_STA_BIOS()
{
    // 0x00
    return _CONFIG_DISP_ACT_STA_BIOS;
}

UINT32 Get_DEFAULT_DPLL_M_DIVIDER()
{
    //0x68;  //hpwang 20071119 set default DCLK=(27*104/8)/2/2=87.75MHz
    return CONFIG_DEFAULT_DPLL_M_DIVIDER;
}

UINT32 Get_DEFAULT_DPLL_N_DIVIDER()
{
    //0x18;
    return CONFIG_DEFAULT_DPLL_N_DIVIDER;
}

UINT32 Get_PANEL_TO_LVDS_ON_ms()
{
    return PANEL_TO_LVDS_ON_ms;
}

UINT32 Get_LVDS_TO_LIGHT_ON_ms()
{
    return LVDS_TO_LIGHT_ON_ms;
}

UINT32 Get_LIGHT_TO_LDVS_OFF_ms()
{
    return LIGHT_TO_LDVS_OFF_ms;
}

UINT32 Get_LVDS_TO_PANEL_OFF_ms()
{
    return LVDS_TO_PANEL_OFF_ms;
}

UINT32 Get_PANEL_OFF_TO_ON_ms()
{
    return PANEL_OFF_TO_ON_ms;
}

#ifdef CONFIG_DISPLAY_CLOCK_MAX
#ifndef CONFIG_DISP_HORIZONTAL_TOTAL_MIN
#define CONFIG_DISP_HORIZONTAL_TOTAL_MIN	(CONFIG_DISP_HORIZONTAL_TOTAL-10)
#endif
#ifndef CONFIG_DISP_HORIZONTAL_TOTAL_MAX
#define CONFIG_DISP_HORIZONTAL_TOTAL_MAX	(CONFIG_DISP_HORIZONTAL_TOTAL+100)
#endif
#ifndef CONFIG_DISPLAY_CLOCK_MIN
#define CONFIG_DISPLAY_CLOCK_MIN	((CONFIG_DISPLAY_CLOCK_MAX*8)/10)
#endif
#define _PxClockTor 0
#define CLK_MAX	((CONFIG_DISPLAY_CLOCK_MAX + _PxClockTor)*1000000)
#define CLK_MIN	((CONFIG_DISPLAY_CLOCK_MIN - _PxClockTor)*1000000)

UINT32 Get_DISP_HORIZONTAL_TOTAL_MIN(void)
{
    return CONFIG_DISP_HORIZONTAL_TOTAL_MIN;
}

UINT32 Get_DISP_HORIZONTAL_TOTAL_MAX(void)
{
    return CONFIG_DISP_HORIZONTAL_TOTAL_MAX;
}

UINT32 Get_DISPLAY_CLOCK_MIN(void)
{
    return CLK_MIN;
}

UINT32 Get_DISPLAY_CLOCK_MAX(void)
{
    return CLK_MAX;
}

#endif

#endif

