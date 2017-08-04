//
//
//

#include "flow_struct.h"
#include "video\video.h"
#include "tv.h"
#include "eepromdefaultcustomer.h"

SYS_STRUCT xdata sysdata;
StructDisplayInfo xdata disp_info;
StructDisplayInfo* xdata info;
StructScreenModeInfo xdata stVGAModeCurrInfo;
StructScreenModeInfo xdata stScreenModeCurrInfo =
{
    0,
    0x80,
    0x80,
    0x80,
    0,
    0,
    0,
    0
};
DisplayTableType    xdata 	stDisplayInfo;

//StructScreenModeInfo stScreenModeCurrInfo[_SOURCE_TOTAL_NUM+1];
