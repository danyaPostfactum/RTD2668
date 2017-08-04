


#include "ifd\ifd.h"
#include "ifd\ifd_factorymode.h"
#include "ifd\ifd_sparrow.h"

#if 0

/*

*/
//=================================================================
//IFD Factory Mode
//=================================================================
//////////////////////////////////////////////////////////////////////////////////////
//Page 1
/////////////////////////////////////////////////////////////////////////////////////
//=================================================================
/**
 * drvif_ifd_factorymode_set_tuner_freq
 *	Set tuner freuency.
 *     <Uint> 10kHz
 * @param <freq>
 * @return { void }
 */
void drvif_ifd_factorymode_set_tuner_freq(UINT32 freq)
{
    drv_tuner_SetFreq(freq);
}
//=================================================================
/**
 * drvif_ifd_factorymode_ifdagc_setting
 *	Set IFD AGC setting
 *
 * @param {enable}<1:IFAGC/0:GPIO>
 * @return  {void}
 */
void drvif_ifd_factorymode_ifdagc_setting(bool enable)
{
    if (enable)
    {
        rtd_maskb(PA_PS0_0x2d30, ~((UINT32)0xf0), ((UINT32)6)<<4);
    }
    else
    {
        rtd_maskb(GPDIR_0x2580, ~_BIT7, 0); // set to GPI
        rtd_maskb(PA_PS0_0x2d30, ~((UINT32)0xf0), ((UINT32)1)<<4); // pin share to GPIO
    }
}
//=================================================================
/**
 * drvif_ifd_factorymode_ifdagc_setting_get_info
 *	Get IFD AGC setting
 *
 * @param   {void}
 * @return   <_TURE:IFAGC/_FLASE:GPIO>
 */
UINT8 drvif_ifd_factorymode_ifdagc_setting_get_info(void)
{
    if ((rtd_inb(PA_PS0_0x2d30)&((UINT32)0xf0))==0x00600000) // pin share to IF_AGC
        return _TRUE;
    else
        return _FALSE;
}
//=================================================================
/**
 * drvif_ifd_factorymode_rfagc_setting
 *	Set RF AGC setting
 *
 * @param {enable}<1:IFAGC/0:GPIO>
 * @return  {void}
 */
void drvif_ifd_factorymode_rfagc_setting(bool enable)
{
    if (enable)
    {
        rtd_maskb(PC_PS2_0x2d3a,~((UINT32)0xf0), ((UINT32)0)<<4); // pin share to RF_AGC
    }
    else
    {
        rtd_maskb(GPDIR_0x2580, ~_BIT19, 0x00000000); // set to GPI
        rtd_maskb(PC_PS2_0x2d3a,~((UINT32)0xf0), ((UINT32)1)<<4); // pin share to GPIO
    }
}
//=================================================================
/**
 * drvif_ifd_factorymode_rfagc_setting_get_info
 *	Get RF AGC setting
 *
 * @param   {void}
 * @return   <_TURE:IFAGC/_FLASE:GPIO>
 */
UINT8 drvif_ifd_factorymode_rfagc_setting_get_info(void)
{
    if ((rtd_inl(PC_PS2_0x2d3a)&((UINT32)0xf0))==0x00000000) // pin share to RF_AGC
        return _TRUE;
    else
        return _FALSE;
}
//=================================================================
/**
 * drvif_ifd_factorymode_pll_lvds7x_turnoff
 *	Turn off LVDS 7x PLL
 *
 * @param  {void}
 * @return  {void}
 */
void drvif_ifd_factorymode_pll_lvds7x_turnoff(void)
{
    //not found. //rtd_outl(0xb800091c, 0xC2684423);
}
//=================================================================
/**
 * drvif_ifd_factorymode_pll_bus_turnoff
 *	Turn off BUS PLL
 *
 * @param  {void}
 * @return  {void}
 */
void drvif_ifd_factorymode_pll_bus_turnoff(void)
{
    //rtd_outl(0xb8001040, 0x00000001); //Switch to Crystal Clock
    //ScalerTimer_DelayXms(200);
    //rtd_outl(0xb8000624, 0x00000003); //Power Down DDR PLL
}
//=================================================================
/**
 * drvif_ifd_factorymode_pll_dpll_turnoff
 *	Turn off DPLL
 *
 * @param  {void}
 * @return  {void}
 */
void drvif_ifd_factorymode_pll_dpll_turnoff(void)
{
    rtd_maskl(PLL_DISP_0_0x2f00, ~_BIT19, 0);//rtd_outl(0xb8000640, 0x0be80daa); //Power Down DISP PLL
    rtd_maskl(PLL_DISP_1_0x2f04, ~_BIT1, 0);//rtd_outl(0xb8000644, 0x00009603); //Power Down ALL DISP PLL
}
//=================================================================
/**
 * drvif_ifd_factorymode_pll_audiopll_turnoff
 *	Turn off Audio PLL
 *
 * @param  {void}
 * @return  {void}
 */
void drvif_ifd_factorymode_pll_audiopll_turnoff(void)
{
    rtd_maskb(SYS_PLL_AUDIO_1_0x2c69, ~_BIT1, 0);
    rtd_maskb(SYS_PLL_512FS_0_0x2c6c, ~_BIT1, 0);
    //rtd_outl(0xb8000660, 0x05288240); //Power Down Audio PLL
    //rtd_outl(0xb8000664, 0x001ec232); //Power Down 512FS PLL
}
#endif





