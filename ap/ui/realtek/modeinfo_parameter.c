//
//
//

#include "flow_struct.h"
#include "source.h"

#if 0	// temporarily not used
StructScreenModeInfo code stVGAModeInfo[]=
{
    {0x00, 0x80, 0x80, 0x80, 0x00, 100, 100},
};

StructScreenModeInfo code stScreenModeInfo[]=
{
    {_SOURCE_VGA, 0x80, 0x80, 0x80, 0x00,100,100},	//VGA
    {_SOURCE_HDMI, 0x80, 0x80, 0x80, 0x00,97,97},		//HDMI

    {_SOURCE_YPBPR, 0x80, 0x80, 0x80, 0x00,97,97},		//YPbPr
    {_SOURCE_CVBS, 0x80, 0x80, 0x80, 0x00,95,95},		//AV
    {_SOURCE_SV, 0x80, 0x80, 0x80, 0x00,95,95},		//SV
#ifdef CONFIG_PVR_ENABLE
    {0x80, 0x80, 0x80, 0x00,97,97,_DisplayRange_FULL,0x00},	//YPbPr (PVR)
#endif
};
#endif

#if 0  // Temporily not use
StructScreenModeInfoSet code stVGAModeCurrInfoSet =
{
    sizeof(stVGAModeCurrInfo) / sizeof(StructScreenModeInfo),
    stVGAModeInfo
};

StructScreenModeInfoSet code stScreenModeCurrInfoSet =
{
    sizeof(stScreenModeCurrInfo) / sizeof(StructScreenModeInfo),
    stScreenModeInfo
};
#endif

