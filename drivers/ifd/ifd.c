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


#ifdef CONFIG_PLATFORM_PACIFIC
#include "if_demod/ifd.h"
#include "if_demod/ifd_factorymode.h"
#include "if_demod/ifd_pacific.h"
#include "rtd_system.h"
#elif defined(CONFIG_ATLANTIC)||defined(CONFIG_LAS_VEGAS)
#include "libs/frontend/ifd.h"
#include "libs/frontend/ifd_factorymode.h"
#include "libs/frontend/ifd_atlantic.h"
#else
#include "ifd\ifd.h"
#include "ifd\ifd_factorymode.h"
#include "ifd\ifd_sparrow.h"
#include "tv.h"
#endif


//=================================================================
//Global Variable
//=================================================================
UINT8 IFD_XDATA g_phase_inverse_enable_counter = 0;
UINT8 IFD_XDATA g_curIFMapIdx = 4;


UINT32 IFD_XDATA g_agc_sum;
//UINT8 IFD_XDATA g_in_peak_status_sum;
UINT32 IFD_XDATA g_agc_avg_pre;
UINT32 IFD_XDATA g_agc_var_diff_abs_sum;
UINT8 IFD_XDATA g_agc_var_index;
UINT8 IFD_XDATA g_state_now;

UINT8 IFD_XDATA g_agc_cal_count;
bool IFD_XDATA g_next_time_dont_check_variance;
INT32 IFD_XDATA g_agc_peak_err_sum;
UINT32 IFD_XDATA g_agc_peak_err_var_diff_abs_sum;
INT32 IFD_XDATA g_agc_peak_err_avg_pre;
UINT32 IFD_XDATA g_agc_peak_err_low_bound;

#define AGC_PEAK_ERR_LOW_BOUND_DEFAULT 0x100



#if 1//#ifdef CONFIG_IFD_AGC_VARIANCE_NEW_ALGO
UINT32 IFD_XDATA g_agc_max_taget_max;
UINT32 IFD_XDATA g_agc_peak_cnt;
#define MAX_TARGET_MIN 0x100
#define MAX_TARGET_RATIO 672
#endif
#ifdef CONFIG_IFD_OUTPUT_CVBS_AMP_COMPENSATE
UINT32 IFD_XDATA g_cvbs_gain_flag= 0;
#endif

UINT8 IFD_XDATA g_afc_stage_count;
UINT32 IFD_XDATA g_afc_curr_freq;
UINT32 IFD_XDATA g_afc_center_freq;
UINT8 IFD_XDATA g_afc_direction;
UINT8 IFD_XDATA g_curr_std=0xff;
UINT8 IFD_XDATA g_afc_hvunlock_debounce;


UINT8 IFD_XDATA g_pre_clamp_rst_flag = 2;
bool IFD_XDATA non_std_cr_fast_flag = 0;


//==============================
#define _IFD_NONSTD_STABLE_CNTNUM 10
#define _IFD_NONSTD_DGAIN_200 0x36b
#define _IFD_NONSTD_DGAIN_50 0x4cf
#define _IFD_NONSTD_DGAIN_THR 0x10
//==============================

//=================================================================
//IFD Table
//=================================================================
#ifdef CONFIG_TUNER_SILICON
static UINT32 IFD_CODE tunerIFMap[] = {58200, 78200,78200};
//static UINT32 IFD_CODE ncoIFMap[] = {0x11b975,0x124bda,0x125129};
static UINT32 IFD_CODE ncoIFMap[] = {0x11b975,0x125129,0x124bda};
#else
static UINT32 IFD_CODE tunerIFMap[] = {339000, 368800, 380000, 389000, 457500};
static UINT32 IFD_CODE ncoIFMap[] = {0x5F49F, 0x51295, 0x4BDA1, 0x4795d, 0x271C7}; //{0x15F49F,0x151295,0x14BDA1,0x14795C,0x1271C7};
#endif
static UINT32 IFD_CODE ncoIF_SECAMLA_Map[] = {0x7e1a8, 0x6FF9E, 0x6AAAA, 0x66666, 0x45ED0};//{0x1E57, 0x6FF9E, 0x6AAAA, 0x99999, 0x45ED0};

UINT16 IFD_CODE group_delay_table1[] =
{
    33,
    0x026,0x7F4,0x7E7,0x7DE,
    0x146,0x207,0x1BF,0x711,
    0x7B5,0x086,0x052,0x69A,
    0x185,0x71E,0x018,0x065,
    0x77B,0x072,0x7B4,0x027,
    0x7F8,0x7F5,0x012,0x7EF,
    0x00D,0x7F6,0x007,0x7FC,
    0x001,0x002,0x7FD,0x004,
    0x7FD,
};

UINT16 IFD_CODE group_delay_table2[] =
{
    33,
    0x01f,0x7E4,0x7CF,0x7EA,
    0x167,0x21F,0x179,0x6FE,
    0x7F0,0x0B0,0x7F8,0x6E8,
    0x18C,0x6CD,0x07E,0x015,
    0x7A4,0x069,0x7A6,0x040,
    0x00D,0x002,0x7F8,0x00A,
    0x7F6,0x00A,0x007,0x7F7,
    0x007,0x7FB,0x002,0x000,
    0x7FE,
};

UINT16 IFD_CODE group_delay_table3[] =
{
    33,
    0x004,0x7F9,0x018,0x7BF,
    0x085,0x721,0x137,0x6B5,
    0x0CC,0x037,0x703,0x0B6,
    0x065,0x6F7,0x031,0x218,
    0x1E5,0x08D,0x7D6,0x7E2,
    0x00C,0x00C,0x7FC,0x7FA,
    0x001,0x004,0x000,0x7FE,
    0x000,0x001,0x000,0x7FE,
    0x7FF,
};

UINT16 IFD_CODE group_delay_table4[] =
{
    30,
    0x00B,0x7D5,0x06E,0x738,
    0x12E,0x6A1,0x0FA,0x005,
    0x717,0x0CD,0x044,0x6F5,
    0x05A,0x229,0x1CF,0x078,
    0x7D3,0x7E6,0x00D,0x00B,
    0x7FA,0x7FA,0x003,0x004,
    0x7FF,0x7FD,0x7FF,0x000,
    0x000,0x7FF,
};

UINT16 IFD_CODE group_delay_table5[] =
{
    33,
    0x023,    0x7A2,    0x09C,    0x729,    0x116,
    0x6E0,    0x08A,    0x09F,    0x1C8,    0x26D,
    0x74D,    0x739,    0x0F7,    0x7D6,    0x780,
    0x06E,    0x002,    0x7BF,    0x025,    0x003,
    0x7E9,    0x011,    0x7FE,    0x7F7,    0x009,
    0x7FD,    0x7FC,    0x005,    0x7FE,    0x7FE,
    0x003,    0x7FF,    0x7FF,

};

static UINT16 IFD_CODE crBPFMap[CR_BPF_TABLE_NUM][CR_BPF_TABLE_SIZE] =
{
    // [0]   //20.1M==>not coorect copy form 16M
    {
        0x0000,    0x0000,    0x0022,    0x001E,
        0x7FB9,    0x001B,    0x0026,    0x7FE1,
        0x7FFF,    0x0031,    0x006D,    0x0018,
        0x7EDD,    0x011D,    0x00BD,    0x7D8F,
        0x015C,    0x01D5,    0x7CC6,    0x00CD,
        0x0271,    0x7D88,    0x7FF8,    0x010C,
        0x0011,    0x0071,    0x7C96,    0x0324,
        0x0406,    0x759A,    0x0463,    0x0B50,
        0x6E8A,    0x01BB,    0x14B1,    0x6AAD,
        0x7B07,    0x1CB6,    0x6C2F,    0x729C,
        0x1FDF,
    },

    // [1]  //17.12M==>not coorect copy form 16M
    {
        0x0000,    0x0000,    0x0022,    0x001E,
        0x7FB9,    0x001B,    0x0026,    0x7FE1,
        0x7FFF,    0x0031,    0x006D,    0x0018,
        0x7EDD,    0x011D,    0x00BD,    0x7D8F,
        0x015C,    0x01D5,    0x7CC6,    0x00CD,
        0x0271,    0x7D88,    0x7FF8,    0x010C,
        0x0011,    0x0071,    0x7C96,    0x0324,
        0x0406,    0x759A,    0x0463,    0x0B50,
        0x6E8A,    0x01BB,    0x14B1,    0x6AAD,
        0x7B07,    0x1CB6,    0x6C2F,    0x729C,
        0x1FDF,
    },

    // [2]  //16M
    {
        0xFFEF, 0xFFE2, 0x0015, 0x002D,
        0xFFC9, 0xFFD2, 0x0073, 0x0006,
        0xFF4E, 0x0062, 0x00C5, 0xFEFB,
        0xFF91, 0x01A9, 0xFF8A, 0xFE1C,
        0x01C9, 0x014B, 0xFCF2, 0x0050,
        0x038C, 0xFD68, 0xFD60, 0x04B3,
        0x0027, 0xFA78, 0x033E, 0x0447,
        0xF9BC, 0xFF15, 0x0776, 0xFC77,
        0xFA13, 0x075E, 0x01DC, 0xF721,
        0x035D, 0x072E, 0xF84B, 0xFD40,
        0x0964,
    },

#ifndef CONFIG_TUNER_SILICON
    // [3]  //15.1M
    {
        0x000D, 0x000A, 0xFFE0, 0xFFEC,
        0x003F, 0x0011, 0xFF91, 0x000C,
        0x00A8, 0xFFAD, 0xFF2C, 0x00CC,
        0x00D6, 0xFE91, 0xFF76, 0x021E,
        0xFFD4, 0xFD5D, 0x014E, 0x02BE,
        0xFD43, 0xFDCE, 0x0431, 0x00E0,
        0xFABB, 0x0127, 0x0593, 0xFC69,
        0xFB34, 0x05F3, 0x02E0, 0xF857,
        0xFFFB, 0x0839, 0xFCB9, 0xF8A6,
        0x065A, 0x0517, 0xF778, 0xFE2E,
        0x0951,
    },

    // [4] //8.25M
    {
        0x0003, 0x0001, 0xFFF2, 0xFFD8,
        0xFFC8, 0xFFE4, 0x0032, 0x007F,
        0x0074, 0xFFEB, 0xFF32, 0xFEF9,
        0xFFB3, 0x0104, 0x01CA, 0x010D,
        0xFF0A, 0xFD62, 0xFDD3, 0x007B,
        0x034F, 0x0399, 0x0086, 0xFC5E,
        0xFAE4, 0xFDF4, 0x0363, 0x0670,
        0x03F2, 0xFD8B, 0xF8B5, 0xFA0B,
        0x00E2, 0x076F, 0x07BE, 0x0128,
        0xF93E, 0xF707, 0xFCA8, 0x0552,
        0x096A,
    },
#else
    // [3] //7.82M
    {

        /*0xFFDF,     0x0016,     0x0066,     0x0061,
        0xFFB3,     0xFEF0,     0xFF3E,     0x00D3,
        0x0219,     0x0126,     0xFE5B,     0xFCBA,
        0xFEA8,    0x0295,      0x0422,     0x0131,
        0xFCD2,     0xFBE9,     0xFF4C,     0x02C9,
        0x029F,     0x0025,     0xFF47,     0x0072,
        0xFFFE,     0xFC9C,     0xFB39,     0x00D9,
        0x0962,     0x0989,     0xFCFF,     0xEF98,
        0xF265,     0x0661,     0x1717,     0x0FF5,
        0xF5A6,     0xE418,     0xEFF9,     0x0DEC,
        0x1DAA,*/
        0x7FE6,0x0016,0x0055,0x0059,
        0x7FFB,0x7F74,0x7F44,0x7FB6,
        0x0082,0x00FB,0x00AC,0x7FD9,
        0x7F41,0x7F5C,0x7FD8,0x0005,
        0x7FB7,0x7FA5,0x0093,0x0213,
        0x0265,0x0020,0x7C39,0x7A1B,
        0x7CE1,0x03CA,0x0999,0x0869,
        0x7F51,0x749B,0x7186,0x7A51,
        0x0965,0x12BE,0x0DAB,0x7CD5,
        0x6D29,0x6BA7,0x7A59,0x0DFF,
        0x16F3,

    },
    // [4] //5.82M
    {

        /*0x0026,0xFFF2,0x0005,0x0045,
        0x0064,0x001E,0xFF6A,0xFE9C,
        0xFE41,0xFECC,0x0046,0x0224,
        0x0373,0x0350,0x016A,0xFE57,
        0xFB6D,0xFA36,0xFBA1,0xFF6D,
        0x041A,0x077B,0x07BE,0x045A,
        0xFE85,0xF8C3,0xF5D4,0xF75E,
        0xFD09,0x047E,0x0A63,0x0BE2,
        0x080A,0x005B,0xF832,0xF347,
        0xF3F1,0xFA09,0x02E7,0x0A93,
        0x0D96,*/
        0x0018,0x7FF9,0x7FC1,0x7F90,
        0x7F90,0x7FD8,0x0054,0x00C6,
        0x00E8,0x0099,0x7FFA,0x7F63,
        0x7F2A,0x7F64,0x7FD1,0x0005,
        0x7FC0,0x7F34,0x7EFF,0x7FC3,
        0x0196,0x03AD,0x049C,0x031C,
        0x7F06,0x79D8,0x7641,0x76D3,
        0x7C6A,0x053B,0x0D4A,0x103A,
        0x0BA4,0x00BC,0x7423,0x6BE1,
        0x6C63,0x760E,0x04C9,0x11CD,
        0x16F6,

    },
#endif
};

static UINT16 IFD_CODE crBPF_SECAMLA_Map[CR_BPF_TABLE_NUM][CR_BPF_TABLE_SIZE] =
{
    // [0]
    {
        0x0000,	0x0000,	0x0000,	0x0000,
        0x0000,	0x0000,	0x0000,	0x0000,
        0x0000,	0x0000,	0x0000,	0x0000,
        0x0000,	0x0000,	0x0000,	0x0000,
        0x0000,	0x0000,	0x0000,	0x0000,
        0x0000,	0x0000,	0x0000,	0x0000,
        0x0000,	0x0000,	0x0000,	0x0000,
        0x0000,	0x0000,	0x0000,	0x0000,
        0x0000,	0x0000,	0x0000,	0x0000,
        0x0000,	0x0000,	0x0000,	0x0000,
        0x3FFF,
    },

    // [1]
    {
        0x0000,	0x0000,	0x0000,	0x0000,
        0x0000,	0x0000,	0x0000,	0x0000,
        0x0000,	0x0000,	0x0000,	0x0000,
        0x0000,	0x0000,	0x0000,	0x0000,
        0x0000,	0x0000,	0x0000,	0x0000,
        0x0000,	0x0000,	0x0000,	0x0000,
        0x0000,	0x0000,	0x0000,	0x0000,
        0x0000,	0x0000,	0x0000,	0x0000,
        0x0000,	0x0000,	0x0000,	0x0000,
        0x0000,	0x0000,	0x0000,	0x0000,
        0x3FFF,
    },

    // [2]  //21.8M
    {
        0xFFFC, 0x000F, 0xFFE0, 0x0033,
        0xFFC3, 0x0032, 0xFFF9, 0xFFBE,
        0x0095, 0xFF31, 0x00C6, 0xFF9E,
        0xFFAA, 0x012D, 0xFE33, 0x01DC,
        0xFEDA, 0xFFC1, 0x01E8, 0xFCD0,
        0x0374, 0xFDA2, 0x001B, 0x029F,
        0xFB3C, 0x055C, 0xFC16, 0x00BC,
        0x0322, 0xF9C9, 0x0735, 0xFA7E,
        0x018D, 0x0349, 0xF8D4, 0x088E,
        0xF93B, 0x0262, 0x0303, 0xF89F,
        0x090D,
    },

    // [3]	//20.9M
    {
        0x0001, 0xFFF8, 0x0017, 0xFFD4,
        0x0040, 0xFFC2, 0x0019, 0x0033,
        0xFF75, 0x00C2, 0xFF5F, 0x0016,
        0x00BC, 0xFE8E, 0x0198, 0xFF1B,
        0xFF7B, 0x020D, 0xFD22, 0x0251,
        0xFFA0, 0xFDCE, 0x041A, 0xFBD4,
        0x0208, 0x0189, 0xFB2C, 0x0609,
        0xFBC2, 0x0009, 0x04A1, 0xF8A8,
        0x0684, 0xFDB4, 0xFCA4, 0x079F,
        0xF7C4, 0x04BB, 0x013B, 0xF951,
        0x08DF,
    },

    // [4]
    {
        0x0000,	0x0000,	0x0000,	0x0000,
        0x0000,	0x0000,	0x0000,	0x0000,
        0x0000,	0x0000,	0x0000,	0x0000,
        0x0000,	0x0000,	0x0000,	0x0000,
        0x0000,	0x0000,	0x0000,	0x0000,
        0x0000,	0x0000,	0x0000,	0x0000,
        0x0000,	0x0000,	0x0000,	0x0000,
        0x0000,	0x0000,	0x0000,	0x0000,
        0x0000,	0x0000,	0x0000,	0x0000,
        0x0000,	0x0000,	0x0000,	0x0000,
        0x3FFF,
    }
};
#if defined(CONFIG_PLATFORM_PACIFIC)||defined(CONFIG_ATLANTIC)||defined(CONFIG_LAS_VEGAS)
static UINT16 IFD_CODE nyquist_detect_BPF_Map[2][CR_BPF_TABLE_NUM][CR_BPF_TABLE_SIZE] =
{
    {	// [0][0]
        {
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x3FFF,
        },

        // [0][1]
        {
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x3FFF,
        },

        // [0][2]  //15.5M
        {
            0x7EC3, 0x0071, 0x011D, 0x7F01,
            0x7F4B, 0x0164, 0x0016, 0x7E7E,
            0x009F, 0x014A, 0x7EBD, 0x7F3F,
            0x01AC, 0x0000, 0x7E42, 0x00D2,
            0x016C, 0x7E7C, 0x7F3D, 0x01EC,
            0x7FE3, 0x7E14, 0x0104, 0x0180,
            0x7E42, 0x7F48, 0x021D, 0x7FC0,
            0x7DF6, 0x0133, 0x0183, 0x7E15,
            0x7F5C, 0x023D, 0x7F9C, 0x7DED,
            0x015B, 0x0176, 0x7DF7, 0x7F79,
            0x0247,
        },

        // [0][3]
        {
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x3FFF,
        },

        // [0][4]
        {
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x3FFF,
        }
    },
    {
        // [1][0]
        {
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x3FFF,
        },

        // [1][1]
        {
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x3FFF,
        },

        // [1][2]  //16.5M
        {
            0x0036,	0x0117,	0x7F02,	0x7F8B,
            0x0160,	0x7F84,	0x7EE3,	0x014B,
            0x0044,	0x7E75,	0x00CD,	0x010E,
            0x7E6D,	0x0000,	0x01A4,	0x7EDC,
            0x7F18,	0x01D0,	0x7FAD,	0x7E5B,
            0x017A,	0x00AB,	0x7E05,	0x00B0,
            0x018E,	0x7E39,	0x7FA4,	0x0210,
            0x7EF2,	0x7EA2,	0x0204,	0x0000,
            0x7DF6,	0x0167,	0x0119,	0x7DD5,
            0x0062,	0x01EB,	0x7E4D,	0x7F3E,
            0x0238,
        },

        // [1][3]
        {
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x3FFF,
        },

        // [1][4]
        {
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x0000,	0x0000,	0x0000,	0x0000,
            0x3FFF,
        }
    }
};
#endif

#ifdef CONFIG_TUNER_SILICON
#if defined(CONFIG_ATLANTIC)||defined(CONFIG_LAS_VEGAS)
#else
static UINT16 IFD_CODE eq_coef_non_secamla[CR_BPF_TABLE_NUM][EQ_TABLE_SIZE] =
{
    //  non secam L'
    // [0]   //5.82*2
    {
        0x077, 0x77, 0x18B, 0x18B, 0x02F,  0x020, 0x020, 0x02F, //[119   395    47   395   119]
    },

    // [1]  //7.82*2
    {
        0x3E8, 0x3E8, 0x1CF, 0x1CF, 0x0BA,  0x020, 0x020, 0x0BA,//[-24   463   186   463   -24]
    },

    // [2]  //16M
    {
        0x000, 0x000, 0x000, 0x000,0x1FF,  0x020, 0x020, 0x1FF,
    },

    // [3]  //15.1M
    {
        0x000, 0x000, 0x000, 0x000,0x1FF,  0x020, 0x020, 0x1FF,
    },

    // [4] //8.25M
    {
        0x000, 0x000, 0x000, 0x000,0x1FF,  0x020, 0x020, 0x1FF,
    },
};
static UINT16 IFD_CODE eq_coef_secamla[CR_BPF_TABLE_NUM][EQ_TABLE_SIZE] =
{
    //  secam L'
    // [0]   //20.1M ==>not coorect; bypass
    {
        0x000, 0x000, 0x000, 0x000,0x1FF,  0x020, 0x020, 0x1FF,
    },

    // [1]  //17.12M ==>not coorect; bypass
    {
        0x000, 0x000, 0x000, 0x000,0x1FF,  0x020, 0x020, 0x1FF,
    },

    // [2]  // zero at 9M
    {	// secam L'
        0x093, 0x093, 0x047, 0x047,0x04C, 0x020, 0x020, 0x04C,
    },

    // [3]  // zero at 10.8M
    {
        0x069, 0x069, 0x062, 0x062,0x06D,  0x020, 0x020, 0x06D,
    },

    // [4] //8.25M ==>not coorect; bypass
    {
        0x000, 0x000, 0x000, 0x000,0x1FF,  0x020, 0x020, 0x1FF,
    },
};
#endif

#else //#ifdef CONFIG_TUNER_SILICON

#if defined(CONFIG_ATLANTIC)||defined(CONFIG_LAS_VEGAS)
#else
static UINT16 IFD_CODE eq_coef_non_secamla[CR_BPF_TABLE_NUM][EQ_TABLE_SIZE] =
{
    //  non secam L'
    // [0]   //20.1M
    {
        0x000, 0x000, 0x000, 0x000,0x1FF,  0x020, 0x020, 0x1FF,
    },

    // [1]  //17.12M
    {
        0x000, 0x000, 0x000, 0x000,0x1FF,  0x020, 0x020, 0x1FF,
    },

    // [2]  //16M
    {
        0x000, 0x000, 0x000, 0x000,0x1FF,  0x020, 0x020, 0x1FF,
    },

    // [3]  //15.1M
    {
        0x000, 0x000, 0x000, 0x000,0x1FF,  0x020, 0x020, 0x1FF,
    },

    // [4] //8.25M
    {
        0x000, 0x000, 0x000, 0x000,0x1FF,  0x020, 0x020, 0x1FF,
    },
};
static UINT16 IFD_CODE eq_coef_secamla[CR_BPF_TABLE_NUM][EQ_TABLE_SIZE] =
{
    //  secam L'
    // [0]   //20.1M ==>not coorect; bypass
    {
        0x000, 0x000, 0x000, 0x000,0x1FF,  0x020, 0x020, 0x1FF,
    },

    // [1]  //17.12M ==>not coorect; bypass
    {
        0x000, 0x000, 0x000, 0x000,0x1FF,  0x020, 0x020, 0x1FF,
    },

    // [2]  // zero at 9M
    {	// secam L'
        0x093, 0x093, 0x047, 0x047,0x04C, 0x020, 0x020, 0x04C,
    },

    // [3]  // zero at 10.8M
    {
        0x069, 0x069, 0x062, 0x062,0x06D,  0x020, 0x020, 0x06D,
    },

    // [4] //8.25M ==>not coorect; bypass
    {
        0x000, 0x000, 0x000, 0x000,0x1FF,  0x020, 0x020, 0x1FF,
    },
};
#endif
#endif //#ifdef CONFIG_TUNER_SILICON

#if defined(CONFIG_PLATFORM_PACIFIC)||defined(CONFIG_ATLANTIC)||defined(CONFIG_LAS_VEGAS)
#else
static UINT8 IFD_CODE gTunerActiveID = 0;
static StructIFDDataType IFD_CODE stIFDUserData=
{
#ifdef CONFIG_TUNER_XUGUANG_TDQ6K_T116CW
    IFD_FLASH_MAGIC_CODE,
    IFD_AGC_2STEP_NOIFAMP_PGA,
    {0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,},
    {0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,},
    3,
    15,
    0,
    0,
    16,
    0x5,
    0x9,
    0x10,//modified by wjchen, 20100226, speedup channel scan
    0,
    IFD_DISABLE,
    5,
    0,
    0,
    0,
    IFD_VIDEO_BW_SEL_4P14M,
    IFD_VIDEO_BW_SEL_5P1M,
    IFD_VIDEO_BW_SEL_5P1M,
    IFD_VIDEO_BW_SEL_5P1M,
    SOUND_NOTCH_SEL_4P5M200k,
    SOUND_NOTCH_SEL2_4P72M135K,
    SOUND_NOTCH_SEL_BYPASS,
    SOUND_NOTCH_SEL2_BYPASS,
    SOUND_NOTCH_SEL_BYPASS,
    SOUND_NOTCH_SEL2_BYPASS,
    IFD_AUDIO_BP_SEL_4P000M_5P000M,
    IFD_AUDIO_BP_SEL_5P182M_7P020M,
    IFD_AUDIO_BP_SEL_BYPASS,
    IFD_UPSAMPLE_LP_SEL_NTSC,
    IFD_UPSAMPLE_LP_SEL_PAL,
    IFD_UPSAMPLE_LP_SEL_PAL,
    0x1fff,
    IFD_ENABLE,
    IFD_ENABLE,
    0x300,
    0x400,
    0x400,
    0x110,
    0x7b0,
    0x18,
    0x0,
    0x06,
    0x0F,
    0x000,
    0x300,
    0x400,
    0x400,
    0x110,
    0x7b0,
    0x1,
    0x0,
    0x3,
    0x7,
    0x0F,
    0x0FFF,

#elif defined(CONFIG_TUNER_TCL_F41CT_2_E)
IFD_FLASH_MAGIC_CODE,
IFD_AGC_2STEP_NOIFAMP_PGA,
{0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,},
{37,26,41,0x22,0x22,0x22,0x22,0x22,0x22,0x22,},
3,
15,
0,
0,
16,
0x5,
0x9,
0x10,//modified by wjchen, 20100226, speedup channel scan
0,
IFD_DISABLE,
5,
0,
0,
0,
IFD_VIDEO_BW_SEL_4P14M,
IFD_VIDEO_BW_SEL_5P1M,
IFD_VIDEO_BW_SEL_5P1M,
IFD_VIDEO_BW_SEL_5P1M,
SOUND_NOTCH_SEL_4P5M200k,
SOUND_NOTCH_SEL2_4P72M135K,
SOUND_NOTCH_SEL_BYPASS,
SOUND_NOTCH_SEL2_BYPASS,
SOUND_NOTCH_SEL_BYPASS,
SOUND_NOTCH_SEL2_BYPASS,
IFD_AUDIO_BP_SEL_4P000M_5P000M,
IFD_AUDIO_BP_SEL_5P182M_7P020M,
IFD_AUDIO_BP_SEL_BYPASS,
IFD_UPSAMPLE_LP_SEL_NTSC,
IFD_UPSAMPLE_LP_SEL_PAL,
IFD_UPSAMPLE_LP_SEL_PAL,
0x1fff,
IFD_ENABLE,
IFD_ENABLE,
0x300,
0x300,
0x300,
0x110,
0x7b0,
0x18,
0x0,
0x06,
0x0F,
0x000,
0x300,
0x400,
0x400,
0x110,
0x7b0,
0x1,
0x0,
0x3,
0x7,
0x0F,
0x0FFF,
#elif defined(CONFIG_TUNER_KEH_FTDC3S20V04)
IFD_FLASH_MAGIC_CODE,
IFD_AGC_2STEP_NOIFAMP_PGA,
{0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,},
{30,38,50,0x22,0x22,0x22,0x22,0x22,0x22,0x22,},
3,
15,
0,
0,
16,
0x5,
0x9,
0x10,//modified by wjchen, 20100226, speedup channel scan
0,
IFD_DISABLE,
5,
0,
0,
0,
IFD_VIDEO_BW_SEL_4P14M,
IFD_VIDEO_BW_SEL_5P1M,
IFD_VIDEO_BW_SEL_5P1M,
IFD_VIDEO_BW_SEL_5P1M,
SOUND_NOTCH_SEL_4P5M200k,
SOUND_NOTCH_SEL2_4P72M135K,
SOUND_NOTCH_SEL_BYPASS,
SOUND_NOTCH_SEL2_BYPASS,
SOUND_NOTCH_SEL_BYPASS,
SOUND_NOTCH_SEL2_BYPASS,
IFD_AUDIO_BP_SEL_4P000M_5P000M,
IFD_AUDIO_BP_SEL_5P182M_7P020M,
IFD_AUDIO_BP_SEL_BYPASS,
IFD_UPSAMPLE_LP_SEL_NTSC,
IFD_UPSAMPLE_LP_SEL_PAL,
IFD_UPSAMPLE_LP_SEL_PAL,
0x1fff,
IFD_ENABLE,
IFD_ENABLE,
0x300,
0x300,
0x300,
0x110,
0x7b0,
0x18,
0x0,
0x06,
0x0F,
0x000,
0x300,
0x400,
0x400,
0x110,
0x7b0,
0x1,
0x0,
0x3,
0x7,
0x0F,
0x0FFF,

#elif defined(CONFIG_TUNER_XUGUANG_TDQ_6ID_GW115HR)
IFD_FLASH_MAGIC_CODE,
IFD_AGC_2STEP_NOIFAMP_PGA,
{0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,},
{32,39,43,0x22,0x22,0x22,0x22,0x22,0x22,0x22,},
3,
15,
0,
0,
16,
0x5,
0x9,
0x10,//modified by wjchen, 20100226, speedup channel scan
0,
IFD_DISABLE,
5,
0,
0,
0,
IFD_VIDEO_BW_SEL_4P14M,
IFD_VIDEO_BW_SEL_5P1M,
IFD_VIDEO_BW_SEL_5P1M,
IFD_VIDEO_BW_SEL_5P1M,
SOUND_NOTCH_SEL_4P5M200k,
SOUND_NOTCH_SEL2_4P72M135K,
SOUND_NOTCH_SEL_BYPASS,
SOUND_NOTCH_SEL2_BYPASS,
SOUND_NOTCH_SEL_BYPASS,
SOUND_NOTCH_SEL2_BYPASS,
IFD_AUDIO_BP_SEL_4P000M_5P000M,
IFD_AUDIO_BP_SEL_5P182M_7P020M,
IFD_AUDIO_BP_SEL_BYPASS,
IFD_UPSAMPLE_LP_SEL_NTSC,
IFD_UPSAMPLE_LP_SEL_PAL,
IFD_UPSAMPLE_LP_SEL_PAL,
0x1fff,
IFD_ENABLE,
IFD_ENABLE,
0x300,
0x300,
0x300,
0x110,
0x7b0,
0x18,
0x0,
0x06,
0x0F,
0x000,
0x300,
0x400,
0x400,
0x110,
0x7b0,
0x1,
0x0,
0x3,
0x7,
0x0F,
0x0FFF,

#elif defined(CONFIG_TUNER_XUGUANG_TDQ_6FT_W116H)
IFD_FLASH_MAGIC_CODE,
IFD_AGC_2STEP_NOIFAMP_PGA,
{0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,},
{42,49,56,0x22,0x22,0x22,0x22,0x22,0x22,0x22,},
3,
15,
0,
0,
16,
0x5,
0x9,
0x10,//modified by wjchen, 20100226, speedup channel scan
0,
IFD_DISABLE,
5,
0,
0,
0,
IFD_VIDEO_BW_SEL_4P14M,
IFD_VIDEO_BW_SEL_5P1M,
IFD_VIDEO_BW_SEL_5P1M,
IFD_VIDEO_BW_SEL_5P1M,
SOUND_NOTCH_SEL_4P5M200k,
SOUND_NOTCH_SEL2_4P72M135K,
SOUND_NOTCH_SEL_BYPASS,
SOUND_NOTCH_SEL2_BYPASS,
SOUND_NOTCH_SEL_BYPASS,
SOUND_NOTCH_SEL2_BYPASS,
IFD_AUDIO_BP_SEL_4P000M_5P000M,
IFD_AUDIO_BP_SEL_5P182M_7P020M,
IFD_AUDIO_BP_SEL_BYPASS,
IFD_UPSAMPLE_LP_SEL_NTSC,
IFD_UPSAMPLE_LP_SEL_PAL,
IFD_UPSAMPLE_LP_SEL_PAL,
0x1fff,
IFD_ENABLE,
IFD_ENABLE,
0x300,
0x300,
0x300,
0x110,
0x7b0,
0x18,
0x0,
0x06,
0x0F,
0x000,
0x300,
0x400,
0x400,
0x110,
0x7b0,
0x1,
0x0,
0x3,
0x7,
0x0F,
0x0FFF,

#elif defined(CONFIG_TCL_F13WP_2DP_E)
IFD_FLASH_MAGIC_CODE,
IFD_AGC_2STEP_NOIFAMP_PGA,
{0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,},
{30,34,39,0x22,0x22,0x22,0x22,0x22,0x22,0x22,},
3,
15,
0,
0,
16,
0x5,
0x9,
0x10,//modified by wjchen, 20100226, speedup channel scan
0,
IFD_DISABLE,
5,
0,
0,
0,
IFD_VIDEO_BW_SEL_4P14M,
IFD_VIDEO_BW_SEL_5P1M,
IFD_VIDEO_BW_SEL_5P1M,
IFD_VIDEO_BW_SEL_5P1M,
SOUND_NOTCH_SEL_4P5M200k,
SOUND_NOTCH_SEL2_4P72M135K,
SOUND_NOTCH_SEL_BYPASS,
SOUND_NOTCH_SEL2_BYPASS,
SOUND_NOTCH_SEL_BYPASS,
SOUND_NOTCH_SEL2_BYPASS,
IFD_AUDIO_BP_SEL_4P000M_5P000M,
IFD_AUDIO_BP_SEL_5P182M_7P020M,
IFD_AUDIO_BP_SEL_BYPASS,
IFD_UPSAMPLE_LP_SEL_NTSC,
IFD_UPSAMPLE_LP_SEL_PAL,
IFD_UPSAMPLE_LP_SEL_PAL,
0x1fff,
IFD_ENABLE,
IFD_ENABLE,
0x300,
0x400,
0x400,
0x110,
0x7b0,
0x18,
0x0,
0x06,
0x0F,
0x000,
0x300,
0x400,
0x400,
0x110,
0x7b0,
0x1,
0x0,
0x3,
0x7,
0x0F,
0x0FFF,

#elif defined(CONFIG_TUNER_LG_TDTC_G29D)
IFD_FLASH_MAGIC_CODE,
IFD_AGC_2STEP_NOIFAMP_PGA,
{0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,},
{20,31,31,0x22,0x22,0x22,0x22,0x22,0x22,0x22,},
3,
15,
0,
0,
16,
0x5,
0x9,
0x10,//modified by wjchen, 20100226, speedup channel scan
0,
IFD_DISABLE,
5,
0,
0,
0,
IFD_VIDEO_BW_SEL_4P14M,
IFD_VIDEO_BW_SEL_5P1M,
IFD_VIDEO_BW_SEL_5P1M,
IFD_VIDEO_BW_SEL_5P1M,
SOUND_NOTCH_SEL_4P5M200k,
SOUND_NOTCH_SEL2_4P72M135K,
SOUND_NOTCH_SEL_BYPASS,
SOUND_NOTCH_SEL2_BYPASS,
SOUND_NOTCH_SEL_BYPASS,
SOUND_NOTCH_SEL2_BYPASS,
IFD_AUDIO_BP_SEL_4P000M_5P000M,
IFD_AUDIO_BP_SEL_5P182M_7P020M,
IFD_AUDIO_BP_SEL_BYPASS,
IFD_UPSAMPLE_LP_SEL_NTSC,
IFD_UPSAMPLE_LP_SEL_PAL,
IFD_UPSAMPLE_LP_SEL_PAL,
0x1fff,
IFD_ENABLE,
IFD_ENABLE,
0x300,
0x300,
0x280,
0x110,
0x7b0,
0x18,
0x0,
0x06,
0x0F,
0x000,
0x300,
0x300,
0x280,
0x110,
0x7b0,
0x1,
0x0,
0x3,
0x7,
0x0F,
0x0FFF,

#elif defined(CONFIG_TUNER_QINGJIA_AFT7_W000G)
IFD_FLASH_MAGIC_CODE,
IFD_AGC_2STEP_NOIFAMP_PGA,
{0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,},
{0x20,0x24,0x24,0x22,0x22,0x22,0x22,0x22,0x22,0x22,},
3,
15,
0,
0,
16,
0x5,
0x9,
0x10,//modified by wjchen, 20100226, speedup channel scan
0,
IFD_DISABLE,
5,
0,
0,
0,
IFD_VIDEO_BW_SEL_4P14M,
IFD_VIDEO_BW_SEL_5P1M,
IFD_VIDEO_BW_SEL_5P1M,
IFD_VIDEO_BW_SEL_5P1M,
SOUND_NOTCH_SEL_4P5M200k,
SOUND_NOTCH_SEL2_4P72M135K,
SOUND_NOTCH_SEL_BYPASS,
SOUND_NOTCH_SEL2_BYPASS,
SOUND_NOTCH_SEL_BYPASS,
SOUND_NOTCH_SEL2_BYPASS,
IFD_AUDIO_BP_SEL_4P000M_5P000M,
IFD_AUDIO_BP_SEL_5P182M_7P020M,
IFD_AUDIO_BP_SEL_BYPASS,
IFD_UPSAMPLE_LP_SEL_NTSC,
IFD_UPSAMPLE_LP_SEL_PAL,
IFD_UPSAMPLE_LP_SEL_PAL,
0x1fff,
IFD_ENABLE,
IFD_ENABLE,
0x300,
0x400,
0x400,
0x110,
0x7b0,
0x18,
0x0,
0x06,
0x0F,
0x000,
0x300,
0x400,
0x400,
0x110,
0x7b0,
0x1,
0x0,
0x3,
0x7,
0x0F,
0x0FFF,
#else
IFD_FLASH_MAGIC_CODE,
IFD_AGC_2STEP_NOIFAMP_PGA,
{0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,0xa00,},
{0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,},
3,
15,
0,
0,
16,
0x5,
0x9,
0x10,//modified by wjchen, 20100226, speedup channel scan
0,
IFD_DISABLE,
5,
0,
0,
0,
IFD_VIDEO_BW_SEL_4P14M,
IFD_VIDEO_BW_SEL_5P1M,
IFD_VIDEO_BW_SEL_5P1M,
IFD_VIDEO_BW_SEL_5P1M,
SOUND_NOTCH_SEL_4P5M200k,
SOUND_NOTCH_SEL2_4P72M135K,
SOUND_NOTCH_SEL_BYPASS,
SOUND_NOTCH_SEL2_BYPASS,
SOUND_NOTCH_SEL_BYPASS,
SOUND_NOTCH_SEL2_BYPASS,
IFD_AUDIO_BP_SEL_4P000M_5P000M,
IFD_AUDIO_BP_SEL_5P182M_7P020M,
IFD_AUDIO_BP_SEL_BYPASS,
IFD_UPSAMPLE_LP_SEL_NTSC,
IFD_UPSAMPLE_LP_SEL_PAL,
IFD_UPSAMPLE_LP_SEL_PAL,
0x1fff,
IFD_ENABLE,
IFD_ENABLE,
0x300,
0x400,
0x400,
0x110,
0x7b0,
0x18,
0x0,
0x06,
0x0F,
0x000,
0x300,
0x400,
0x400,
0x110,
0x7b0,
0x1,
0x0,
0x3,
0x7,
0x0F,
0x0FFF,
#endif
};
#endif

//=================================================================
//IFD driver
//=================================================================

#ifdef _VIDEO_TV_SUPPORT
#ifdef CONFIG_TUNER_SILICON

//=================================================================
/**
 * drvif_tuner_is_locked
 *
 *
 * @param <afc>
 * @return <locked>
 */
UINT8 drvif_tuner_is_locked(UINT8 *afc)
{
    INT32 ifd_offset = 0;
    INT32 thd = 3; //threshold
    UINT8 AFC = 0;

    ifd_offset=drvif_tuner_get_afcoffset(AFC);
    *afc = AFC;
#ifdef CONFIG_TUNER_SILICON
    thd = 3*drv_tuner_GetRatioInStepSize();//999stIFDUserData.AFClockthd * CTunerGetRatioInStepSize();
#else
    thd = GET_IFD_AFCLOCK_THD()*drv_tuner_GetRatioInStepSize();//999stIFDUserData.AFClockthd * CTunerGetRatioInStepSize();
#endif
    if ((ifd_offset> -thd)&&(ifd_offset< thd))
    {
        return _TRUE;
    }
    else
    {
        return _FALSE;
    }
}
#endif
//=================================================================
/**
 * drvif_tuner_init
 *
 *
 * @param <mode>
 * @return { void }
 */
void drvif_tuner_init(UINT8 mode)
{

    IFD_DEBUG_MSG(IFD_LOG_LEVEL, "***drvif_tuner_init()\n");

#if defined(CONFIG_ATLANTIC)||defined(CONFIG_LAS_VEGAS)
    switch (mode)
    {
    case ZNTSC:
        drv_ifd_set_video_ntsc();
        break;
    case ZNTSC_443:
        drv_ifd_set_video_pal();
        break;
    case ZPAL_N:
        drv_ifd_set_video_ntsc();
        break;
    case ZPAL_M:
        drv_ifd_set_video_ntsc();
        break;
    case ZPAL_I:
        drv_ifd_set_video_pal();
        break;
    case ZSECAM:
        drv_ifd_set_video_pal();
        break;
    case ZSECAML:
        drv_ifd_set_video_secaml();
        break;
    case ZSECAMLA:
        drv_ifd_set_video_secamla();
        break;
#ifdef  CONFIG_SUPPORT_PAL60
    case ZPAL_60:
        drv_ifd_set_video_pal();
        break;
#endif
    default:
        break;
    }
#else
    switch (mode)
    {
    case _SIG_MODE_NTSC:
        drv_ifd_set_video_ntsc();
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "TUNER SETTTTT NTSC\n");
        break;
    case _SIG_MODE_PAL:
    case _SIG_MODE_PAL_DK:
    case _SIG_MODE_PAL_I:
    case _SIG_MODE_PAL_BG:
    case _SIG_MODE_NTSC_443:
        drv_ifd_set_video_pal();
        break;
    case _SIG_MODE_SECAM_L:
        drv_ifd_set_video_secaml();
        break;
    case _SIG_MODE_SECAM_LA:
        drv_ifd_set_video_secamla();
        break;
    default:
        break;
    }
#endif
#ifdef CONFIG_TUNER_SILICON
    drv_tuner_initRT810(mode);//wjchen test silicon
#endif
}
//=================================================================
/**
 * drvif_tuner_get_afcoffset
 *
 *
 * @param <void>
 * @return <offset>
 */
INT32 drvif_tuner_get_afcoffset(UINT8 afc)   //drvif
{
	UINT32 ifd_offset = 0;
	INT32 shift_freq;
	INT32 divider_offset;
	UINT8 in_afc;

	in_afc = afc;
	ifd_offset = drv_ifd_get_cr_freq_offset()>>8;
	//RTD_Log(LOGGER_LEVEL,"\n Tim ifd_offset =%d \n",(UINT32)ifd_offset);
#ifdef CONFIG_TUNER_SILICON
	if(ifd_offset > 2048) {
		shift_freq =  -(((4096-ifd_offset)*54000)>>16);  //kHz
		shift_freq = - shift_freq;
		divider_offset = (shift_freq*10+((UINT16)drv_tuner_GetRatioInStepSize()/2)) /(UINT16)drv_tuner_GetRatioInStepSize();
	} else {
		shift_freq = (ifd_offset*54000)>>16;  //kHz
		shift_freq = - shift_freq;
		divider_offset = (shift_freq*10-((UINT16)drv_tuner_GetRatioInStepSize()/2)) /(UINT16)drv_tuner_GetRatioInStepSize();
	}
	divider_offset = divider_offset*drv_tuner_GetRatioInStepSize();
#else
	if(ifd_offset > 2048) {
		shift_freq =  -(((4096-ifd_offset)*54000)>>16);  //kHz
		divider_offset = (shift_freq*10-((UINT16)drv_tuner_GetRatioInStepSize()/2)) /(UINT16)drv_tuner_GetRatioInStepSize();
	} else {
		shift_freq = (ifd_offset*54000)>>16;  //kHz
		divider_offset = (shift_freq*10+((UINT16)drv_tuner_GetRatioInStepSize()/2)) /(UINT16)drv_tuner_GetRatioInStepSize();
	}
	divider_offset = divider_offset*drv_tuner_GetRatioInStepSize();
#endif
	//RTD_Log(LOGGER_LEVEL,"\n Tim divider_offset=%d \n",(UINT32)divider_offset);
	return divider_offset;
	
}
#endif
//=================================================================
/**
 * drvif_ifd_system_init
 *
 *
 * @param <void>
 * @return { void }
 */
void drvif_ifd_system_init(void)
{
    RTD_Log(IFD_LOG_LEVEL, "***drvif_ifd_globalvar_init()\n");
    g_phase_inverse_enable_counter = 0;
#ifdef CONFIG_TUNER_SILICON
    g_curIFMapIdx = 1;	//default value
#else
    g_curIFMapIdx = 4;	//default value
#endif
    g_curr_std = 0xff;
    g_agc_sum = 0;
    //g_in_peak_status_sum = 0;
    g_agc_avg_pre = 0;
    g_agc_var_diff_abs_sum = 0;
    g_agc_var_index = 0;
    g_next_time_dont_check_variance = _FALSE;
    g_state_now = 0;

    g_agc_cal_count = 0;
    g_agc_peak_err_sum = 0;
    g_agc_peak_err_var_diff_abs_sum= 0;
    g_agc_peak_err_avg_pre= 0;
    g_agc_peak_err_low_bound= 0;


    g_afc_stage_count=0;
    g_afc_curr_freq=0;
    g_afc_center_freq=0;
    g_afc_direction=1;
    g_afc_hvunlock_debounce=AFC_HV_UNLOCK_DEBOUNCE_COUNT;

//#ifdef _VIDEO_TV_SUPPORT
//    CTunerGlobalVarInit();
//#endif
}
//=================================================================
#ifdef _VIDEO_TV_SUPPORT
/**
 * drvif_ifd_init
 *
 *
 * @param <void>
 * @return { void }
 */
void drvif_ifd_init(void)
{
    //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "***drvif_ifd_init()\n");
#ifdef CONFIG_PLATFORM_PACIFIC
    drvif_ifd_system_init();
#endif
#ifdef CONFIG_TUNER_SILICON
//    drv_tuner_initRT810(_SIG_MODE_PAL);//wjchen test silicon
#else
    tuner_data_init();
#endif
    drv_ifd_user_data_init();
    ifd_active_tuner_assign(gTunerActiveID);
    drv_ifd_set_adc_init();
    drvif_ifd_softreset();

    //drvif_ifd_set_video_standard(ZPAL_I, stIFDUserData.tunerPara.tuner_MaxFreq);
}
#endif
//=================================================================
/**
 * drvif_ifd_power_on
 *
 *
 * @param <enable>
 * @return { void }
 */
void drvif_ifd_power_on(bool enable)
{
    ////IFD_DEBUG_MSG(IFD_LOG_LEVEL, "***drvif_ifd_power_on()\n");
    drv_ifd_power_on(enable);
}
/*
static UINT8 getifdfmtfromvdcmode2(UINT8 video_mode)
{
	UINT8 ifd_mode = _SIG_MODE_PAL;
	switch(video_mode)
	{
		case ZNTSC :
		case ZNTSC_50:
		case ZPAL_M:
		case ZPAL_N:
			ifd_mode = _SIG_MODE_NTSC;
			break;
		case ZSECAML:
			ifd_mode = _SIG_MODE_SECAM_L;
			break;
		case ZSECAMLA:
			ifd_mode = _SIG_MODE_SECAM_LA;
			break;
#ifdef  CONFIG_SUPPORT_PAL60
		case ZPAL_60:
			ifd_mode = _SIG_MODE_PAL_I;
			break;
#endif
		case ZSECAM:
		case ZNTSC_443:
		case ZPAL_I:
			ifd_mode = _SIG_MODE_PAL;
			break;
		default:
			break;
	}
	return ifd_mode;
}
*/
#ifdef _VIDEO_TV_SUPPORT
//=================================================================
/**
 * drvif_ifd_set_iftuner_setting
 *
 *
 * @param <mode><freq><usnum>
 * @return {void}
 */
void drvif_ifd_set_iftuner_setting(UINT8 mode, UINT32 freq, UINT16 usNum)
{
    UINT32 old_freq = 0;
    UINT8 clamp_rst_flag = 0;
    UINT8 newmode = 0;
    usNum = usNum;

    //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "***drvif_ifd_set_iftuner_setting(%x, %d, %d)\n", (UINT32)mode,(UINT32)freq,(UINT32)usNum);

//	UINT8 cur_ifd_mode = getifdfmtfromvdcmode2(mode);
    old_freq = CTunerGetFreq();
    if ((old_freq == freq) && (g_curr_std==mode) )
        return;

    if (g_curr_std != mode)
    {
        newmode = 1;
    }

    if (newmode || !(tvAutoScanFlag || tvManualScanFlag))
    {
        g_curr_std = mode;
#ifdef  CONFIG_TUNER_SILICON
		drv_tuner_initRT810(_SIG_MODE_PAL_I);
        ScalerTimer_DelayXms(20);   //suggest >15,200
#endif
        switch (mode)
        {
        case ZNTSC:
            drv_ifd_set_video_ntsc();
            break;
        case ZNTSC_443:
            drv_ifd_set_video_pal();
            break;
        case ZPAL_N:
            drv_ifd_set_video_ntsc();
            break;
        case ZPAL_M:
            drv_ifd_set_video_ntsc();
            break;
        case ZPAL_I:
            drv_ifd_set_video_pal();
            break;
        case ZSECAM:
            drv_ifd_set_video_pal();
            break;
        case ZSECAML:
            drv_ifd_set_video_secaml();
            clamp_rst_flag = 1;
            break;
        case ZSECAMLA:
            drv_ifd_set_video_secamla();
            clamp_rst_flag = 1;
            break;
#ifdef  CONFIG_SUPPORT_PAL60
        case ZPAL_60:
            drv_ifd_set_video_pal();
            break;
#endif
        default:
            break;
        }
    }

    drvif_ifd_cr_freq_status_check(IFD_CR_STATE_SET_TUNER); //irene@100210
    if (mode ==  ZSECAMLA)
        CTunerSetFreqSearch(freq -SECAMLA_FREQ_SHIFT);
    else
        CTunerSetFreqSearch(freq);
    if (cvideo_tv_channel_scan_is_running) //nessessary
    {
        drv_ifd_cr_loop_settings(IFD_CR_LOOP_SLOW);
    }

#ifdef CONFIG_PLATFORM_PACIFIC
#ifdef CONFIG_IFD_REG_LOG_MSG
    //drv_ifd_log_reg();
#endif
#endif
    if (newmode || !(tvAutoScanFlag || tvManualScanFlag))
    {
        ScalerTimer_DelayXms(20);   //suggest >15,
        if (g_pre_clamp_rst_flag != clamp_rst_flag)
        {
            drvif_ifd_clamp_reset(IFD_MODE_CH_SCAN);
        }
#ifndef CONFIG_TUNER_SILICON//tommy delete 20100805,for speed up autoscan
        ScalerTimer_DelayXms(2);
        ScalerTimer_DelayXms(usNum);
        ScalerTimer_DelayXms(GET_IFD_SCAN_DELAY());
#endif
        g_pre_clamp_rst_flag = clamp_rst_flag;
    }
    else
        ScalerTimer_DelayXms(10);   //tommy add,prevent IFD not lock

}
#endif

#ifdef _VIDEO_TV_SUPPORT
#if 1//ndef CONFIG_TUNER_SILICON
//=================================================================
/**
 * drvif_ifd_set_standard
 *
 *
 * @param <video_mode>
 * @return { void }
 */
void drvif_ifd_set_video_standard(UINT8 video_mode, UINT32 freq)
{
    UINT32 tuner_freq;

    //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "***drvif_ifd_set_video_standard(%x, %d)\n", (UINT32)video_mode,(UINT32)freq);

    if (video_mode ==  ZSECAMLA)
    {
        tuner_freq= freq -SECAMLA_FREQ_SHIFT;
        g_afc_curr_freq = freq-SECAMLA_FREQ_SHIFT;
        g_afc_stage_count =0; //reset afc
    }
    else
    {
        tuner_freq = freq ;
        g_afc_curr_freq = freq;
        g_afc_stage_count = 0; //reset afc
    }
    if (CTunerGetFreq()!=tuner_freq)
    {
        drvif_ifd_cr_freq_status_check(IFD_CR_STATE_SET_TUNER); //irene@100210
#ifndef CONFIG_TUNER_SILICON
        drv_tuner_SetFreq(tuner_freq);
#endif
    }
    if (g_curr_std!=video_mode)
    {

#ifdef CONFIG_TUNER_SILICON
        if (video_mode ==  ZSECAMLA)
        {
            drv_tuner_initRT810(_SIG_MODE_PAL);
            drv_tuner_SetFreq(tuner_freq -SECAMLA_FREQ_SHIFT);
        }
        else
        {

            if ((video_mode ==  ZNTSC)||(video_mode ==  ZPAL_M)||(video_mode ==  ZPAL_N))
                drv_tuner_initRT810(_SIG_MODE_NTSC);
            else
                drv_tuner_initRT810(_SIG_MODE_PAL);

            drv_tuner_SetFreq(tuner_freq);
        }
#endif

        g_curr_std = video_mode;

        switch (video_mode)
        {
        case ZNTSC:
            drv_ifd_set_video_ntsc();
            break;
        case ZNTSC_443:
            drv_ifd_set_video_pal();
            break;
        case ZPAL_N:
            drv_ifd_set_video_ntsc();
            break;
        case ZPAL_M:
            drv_ifd_set_video_ntsc();
            break;
        case ZPAL_I:
            drv_ifd_set_video_pal();
            break;
        case ZSECAM:
            drv_ifd_set_video_pal();
            break;
        case ZSECAML:
            drv_ifd_set_video_secaml();
            break;
        case ZSECAMLA:
            drv_ifd_set_video_secamla();
            break;
#ifdef  CONFIG_SUPPORT_PAL60
        case ZPAL_60:
            drv_ifd_set_video_pal();
            break;
#endif
        default:
            break;
        }
//add irene@20091228 for clamp error while neg -> pos
        drvif_ifd_clamp_reset(IFD_MODE_CH_SET);
        g_pre_clamp_rst_flag = 2; // clear this flag for channel scan
    }

    ScalerTimer_DelayXms(GET_IFD_SETCHANNEL_DELAY());

#ifdef CONFIG_PLATFORM_PACIFIC
#ifdef CONFIG_IFD_REG_LOG_MSG
    ScalerTimer_ReactiveTimerEvent(IFD_REG_LOG_TIMER, drv_ifd_periodic_log_reg);
#endif
#endif
}

#endif
#if 0
//=================================================================
/**
 * drvif_ifd_set_standard
 *
 *
 * @param <video_mode>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drvif_iftuner_finetune_setting(UINT32 freq)
{
    drvif_ifd_cr_freq_status_check(IFD_CR_STATE_SET_TUNER);
    drv_tuner_SetFreq(freq);
}
#endif
//=================================================================

/**
 * drvif_ifd_scan_start_setting
 *
 *
 * @param <video_mode>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drvif_ifd_scan_start_setting(void)
{
    drvif_ifd_set_agc_variance_off(0xf);
}
#endif
//=================================================================
#endif
/**
 * drvif_ifd_get_agc_lock
 *
 *
 * @param {void}
 * @return <lock>
 */
bool drvif_ifd_get_agc_lock()
{
    return drv_ifd_agc_get_loop2_lock();
}
//=================================================================
/**
 * drvif_ifd_get_cr_lock
 *
 *
 * @param {void}
 * @return <lock>
 */
bool drvif_ifd_get_cr_lock()
{
    bool status;

    status = drv_ifd_get_cr_lock_status();
    //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "***drvif_ifd_get_cr_lock()-->%x\n", (UINT32)status);
	//RTD_Log(IFD_LOG_LEVEL, "***drvif_ifd_get_cr_lock()-->%x\n", (UINT32)status);
    return status;
}
//=================================================================
/**
 * drvif_ifd_set_audio_bp
 *
 *
 * @param <mode>
 * @return <newset>
 */
UINT32 drvif_ifd_set_audio_bp(UINT8 mode)
{
    UINT32 ori, newSet, addr;

    //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "***drvif_ifd_set_audio_bp(%x)\n", (UINT32)mode);

#if defined(CONFIG_ATLANTIC)||defined(CONFIG_LAS_VEGAS)
    addr = IFD_49M_AUDIO_BP_SEL_VADDR;
#else
    addr = IFD_AUDIO_BP_SEL_VADDR;
    if (mode==_SIG_MODE_NTSC) mode = ZNTSC;
    else if (mode==_SIG_MODE_AUTO) mode = ZAUTO;
    else mode = ZPAL_I;
#endif
    ori = rtd_inl(addr);

    if (mode == ZNTSC)
        drv_ifd_set_audio_bp_up_lp(GET_IFD_AUDIOBPF_NTSC()/*stIFDUserData.AudioBPF_NTSC*/,IFD_AUDIO_UPSAMPLE_LP_SEL_PAL);
    else if (mode == ZAUTO)
        drv_ifd_set_audio_bp_up_lp(GET_IFD_AUDIOBPF_AUTO()/*stIFDUserData.AudioBPF_AUTO*/,IFD_AUDIO_UPSAMPLE_LP_SEL_PAL);
    else
        drv_ifd_set_audio_bp_up_lp(GET_IFD_AUDIOBPF_PAL()/*stIFDUserData.AudioBPF_PAL*/,IFD_AUDIO_UPSAMPLE_LP_SEL_PAL);

    newSet = rtd_inl(addr);

    return  (ori == newSet); // no change BP
}
//=================================================================
/**
 * drvif_ifd_clamp_reset
 *
 *
 * @param {mode}
 * @return {void}
 */
void drvif_ifd_clamp_reset(IFD_MODE_OPERATE mode)
{
    //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "***drv_ifd_clamp_reset()\n");

    if (mode == IFD_MODE_CH_SCAN)
        drv_ifd_set_clamping_scale(IFD_CLAMP_SCALE_1);
    else if (mode == IFD_MODE_CH_SET)
        drv_ifd_set_clamping_scale(IFD_CLAMP_SCALE_1D4);

    if ((g_curr_std == ZSECAML)||((g_curr_std == ZSECAMLA)))
        drv_ifd_clamp_reset(IFD_CLAMP_RESET_POS);
    else
        drv_ifd_clamp_reset(IFD_CLAMP_RESET_NEG);
}
//=================================================================
/**
 * drvif_ifd_cr_freq_status_check
 *
 *
 * @param <status>
 * @return {void}
 */
void drvif_ifd_cr_freq_status_check(IFD_CR_FLOW_STATE status)
{
    bool control_by_factorymode = 0;
    if (control_by_factorymode == IFD_ENABLE)
    {
        if (status == IFD_CR_FLOW_STATE_ENABLE_BY_FACTORYMODE)
        {
            control_by_factorymode = IFD_DISABLE;
        }
    }
    else
    {
        UINT8 thd;
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "***drvif_ifd_cr_freq_status_check(%x)\n", (UINT32)status);
        thd = GET_IFD_CRINVERSE_DEBOUNCE();

        //IFD_DEBUG_MSG(LOGGER_DEBUG,"AGC LOCK  FLAG %x!! \n",(UINT32)drvif_ifd_get_agc_lock());
        if (status == IFD_CR_STATE_CHECK)    // cr_phase inverse
        {
            if (IFD_CHIP_VER_A()==_TRUE)
            {
                if (drv_ifd_get_cr_lock_status())
                {
                    if (g_phase_inverse_enable_counter>thd)
                    {
                        drv_ifd_cr_phase_inverse_en(IFD_ENABLE);
                    }
                    else
                    {
                        g_phase_inverse_enable_counter++;
                        drv_ifd_cr_phase_inverse_en(IFD_DISABLE);
                    }
                }
                else
                {
                    g_phase_inverse_enable_counter = 0;
                    drv_ifd_cr_phase_inverse_en(IFD_DISABLE);
                }
            }
            else //  for after pacific ver. C / for after sparrow ver. B
            {
                drv_ifd_cr_phase_inverse_en(IFD_ENABLE);
            }
        }
        else if (status == IFD_CR_STATE_SET_TUNER)
        {
            g_phase_inverse_enable_counter = 0;
            drv_ifd_cr_phase_inverse_en(IFD_DISABLE);
        }
        else if (status == IFD_CR_FLOW_STATE_DISABLE_BY_FACTORYMODE)
        {
            g_phase_inverse_enable_counter = 0;
            drv_ifd_cr_phase_inverse_en(IFD_DISABLE);
            control_by_factorymode = IFD_ENABLE;
            //IFD_DEBUG_MSG(LOGGER_DEBUG,"factory mode stop phase inverse\n");
        }
        return;
    }
}
//=================================================================
/**
 * drvif_ifd_softreset
 *
 *
 * @param {void}
 * @return {void}
 */
void drvif_ifd_softreset(void)
{
    //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "***drvif_ifd_softreset()\n");
    drv_ifd_soft_reset(IFD_ENABLE);
    ScalerTimer_DelayXms(1);
    drv_ifd_soft_reset(IFD_DISABLE);
}
//=================================================================
/**
 * drvif_ifd_set_agc_variance_off
 *
 *
 * @param <agc_speed>
 * @return {void}
 */
void drvif_ifd_set_agc_variance_off(UINT8 agc_speed)
{
    UINT16 target = GET_IFD_AGCLOOP1_MAXTARGET_NTSC();

    //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "***drvif_ifd_set_agc_variance_off(%d)\n", (UINT32)agc_speed);

    g_agc_var_index = 0;
    g_agc_sum = 0;
    g_agc_var_diff_abs_sum = 0;
    // g_in_peak_status_sum = 0;
    g_agc_avg_pre = 0;
    g_state_now = 4;
    g_agc_cal_count = 2;

    g_agc_peak_err_sum = 0;
    g_agc_peak_err_avg_pre = 0;
    g_agc_peak_err_var_diff_abs_sum = 0;
    g_agc_peak_err_low_bound = AGC_PEAK_ERR_LOW_BOUND_DEFAULT;

    drv_ifd_agc_loop2_dn_sample((UINT16)agc_speed);

#ifndef CONFIG_TUNER_SILICON
    switch (g_curr_std)
    {
    case ZNTSC:
        target=GET_IFD_AGCLOOP1_MAXTARGET_NTSC();
        break;
    case ZNTSC_443:
        target=GET_IFD_AGCLOOP1_MAXTARGET_PAL();
        break;
    case ZPAL_N:
        target=GET_IFD_AGCLOOP1_MAXTARGET_NTSC();
        break;
    case ZPAL_M:
        target=GET_IFD_AGCLOOP1_MAXTARGET_NTSC();
        break;
    default:
    case ZPAL_I:
        target=GET_IFD_AGCLOOP1_MAXTARGET_PAL();
        break;
    case ZSECAM:
        target=GET_IFD_AGCLOOP1_MAXTARGET_PAL();
        break;
    case ZSECAML:
        target=GET_IFD_AGCLOOP1_MAXTARGET_SECAM();
        break;
    case ZSECAMLA:
        target=GET_IFD_AGCLOOP1_MAXTARGET_SECAM();
        break;
#ifdef  CONFIG_SUPPORT_PAL60
    case ZPAL_60:
        target=GET_IFD_AGCLOOP1_MAXTARGET_PAL();
        break;
#endif
#ifdef  CONFIG_SUPPORT_PAL_L
    case ZPAL_L:
        target=GET_IFD_AGCLOOP1_MAXTARGET_SECAM();
        break;
#endif
    }

#else //CONFIG_TUNER_SILICON
    if ((g_curr_std == ZNTSC) || (g_curr_std == ZPAL_N) || (g_curr_std == ZPAL_M))
        target = 0x300;
    else
        target = 0x400;
#endif //CONFIG_TUNER_SILICON

#if 1 //#ifdef CONFIG_IFD_AGC_VARIANCE_NEW_ALGO
    g_agc_max_taget_max=target;
    g_agc_peak_cnt = 1;
#endif

#ifdef CONFIG_IFD_OUTPUT_CVBS_AMP_COMPENSATE
    drv_ifd_set_videopath_output_gain(IFD_VIDEOPATH_OUTPUT_GAIN_1);
    g_cvbs_gain_flag=0;
#endif

    drv_ifd_agc_targ_val(target);
    drv_ifd_agc_loop2_target(target);

    drv_ifd_agc_loop1_peak_en(IFD_ENABLE);
    drv_ifd_agc_loop2_peak_en(IFD_ENABLE);

#ifdef CONFIG_IFD_NONSTD_3P10_DETECT
    rtdf_maskl(VDC_AVOUT_FIFO_STATUS_VADDR, 0x1f, 0x40); //Switch AVout Mux to  normal path
#endif

    IFD_ScalerTimer_CancelTimerEvent(ifd_set_agc_variance_on_cancel);

#if 0
    UINT16 target = GET_IFD_AGCLOOP1_MAXTARGET_NTSC();

    //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "***drvif_ifd_set_agc_variance_off(%d)\n", (UINT32)agc_speed);

    g_agc_var_index = 0;
    g_agc_sum = 0;
    g_agc_var_diff_abs_sum = 0;
    // g_in_peak_status_sum = 0;
    g_agc_avg_pre = 0;
    g_state_now = 4;


    drv_ifd_agc_loop2_dn_sample((UINT16)agc_speed);

    switch (g_curr_std)
    {
    case ZNTSC:
        target=GET_IFD_AGCLOOP1_MAXTARGET_NTSC();
        break;
    case ZNTSC_443:
        target=GET_IFD_AGCLOOP1_MAXTARGET_PAL();
        break;
    case ZPAL_N:
        target=GET_IFD_AGCLOOP1_MAXTARGET_NTSC();
        break;
    case ZPAL_M:
        target=GET_IFD_AGCLOOP1_MAXTARGET_NTSC();
        break;
    case ZPAL_I:
        target=GET_IFD_AGCLOOP1_MAXTARGET_PAL();
        break;
    case ZSECAM:
        target=GET_IFD_AGCLOOP1_MAXTARGET_PAL();
        break;
    case ZSECAML:
        target=GET_IFD_AGCLOOP1_MAXTARGET_SECAM();
        break;
    case ZSECAMLA:
        target=GET_IFD_AGCLOOP1_MAXTARGET_SECAM();
        break;
#ifdef  CONFIG_SUPPORT_PAL60
    case ZPAL_60:
        target=GET_IFD_AGCLOOP1_MAXTARGET_PAL();
        break;
#endif
    default:
        break;
    }

#if 1//#ifdef CONFIG_IFD_AGC_VARIANCE_NEW_ALGO
    g_agc_max_taget_max=target;
#endif
#ifdef CONFIG_IFD_OUTPUT_CVBS_AMP_COMPENSATE
    drv_ifd_set_videopath_output_gain(IFD_VIDEOPATH_OUTPUT_GAIN_1);
    g_cvbs_gain_flag=0;
#endif

    drv_ifd_agc_targ_val(target);
    drv_ifd_agc_loop2_target(target);

    drv_ifd_agc_loop1_peak_en(IFD_ENABLE);
    drv_ifd_agc_loop2_peak_en(IFD_ENABLE);

#ifdef CONFIG_IFD_NONSTD_3P10_DETECT
    rtdf_maskl(VDC_AVOUT_FIFO_STATUS_VADDR, 0x1f, 0x40); //Switch AVout Mux to  normal path
#endif

    IFD_ScalerTimer_CancelTimerEvent(ifd_set_agc_variance_on_cancel);
#endif
}
//=================================================================
/**
 * drvif_ifd_set_agc_variance_on
 *
 *
 * @param {void}
 * @return {void}
 */

void drvif_ifd_set_agc_variance_on(void)
{

    UINT16 agc_tmp = 0;
    INT32 variance_diff = 0;
    UINT32 agc_var_diff_abs_avg = 0;
    UINT16 max_targ2 = 0;
    UINT16 peak_err2 = 0;
    UINT16 peak_max2 = 0;
    UINT16 default_target;
    bool fine_mode_en;

    INT32 peak_err2_tmp;

#ifndef CONFIG_TUNER_SILICON
    switch (g_curr_std)
    {
    case ZNTSC:
        default_target=GET_IFD_AGCLOOP1_MAXTARGET_NTSC();
        break;
    case ZNTSC_443:
        default_target=GET_IFD_AGCLOOP1_MAXTARGET_PAL();
        break;
    case ZPAL_N:
        default_target=GET_IFD_AGCLOOP1_MAXTARGET_NTSC();
        break;
    case ZPAL_M:
        default_target=GET_IFD_AGCLOOP1_MAXTARGET_NTSC();
        break;
    default:
    case ZPAL_I:
        default_target=GET_IFD_AGCLOOP1_MAXTARGET_PAL();
        break;
    case ZSECAM:
        default_target=GET_IFD_AGCLOOP1_MAXTARGET_PAL();
        break;
    case ZSECAML:
        default_target=GET_IFD_AGCLOOP1_MAXTARGET_SECAM();
        break;
    case ZSECAMLA:
        default_target=GET_IFD_AGCLOOP1_MAXTARGET_SECAM();
        break;
#ifdef  CONFIG_SUPPORT_PAL60
    case ZPAL_60:
        default_target=GET_IFD_AGCLOOP1_MAXTARGET_PAL();
        break;
#endif
#ifdef  CONFIG_SUPPORT_PAL_L
    case ZPAL_L:
        default_target=GET_IFD_AGCLOOP1_MAXTARGET_SECAM();
        break;
#endif
    }
#else //CONFIG_TUNER_SILICON
    if ((g_curr_std == ZNTSC) || (g_curr_std == ZPAL_N) || (g_curr_std == ZPAL_M))
        default_target = 0x300;
    else
        default_target = 0x400;
#endif //CONFIG_TUNER_SILICON

    fine_mode_en = drv_ifd_agc_loop1_fine_tune_en_get_info();

    if (fine_mode_en == _TRUE)
    {
        drv_ifd_agc_loop1_peak_en(IFD_ENABLE);
        drv_ifd_agc_loop2_peak_en(IFD_ENABLE);
        drv_ifd_agc_loop2_dn_sample(0x0000000f);
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "[AGC][pos mod mode]\n");
        return;
    }
    else if ((drv_ifd_get_agc_loop1_peak_en()||drv_ifd_get_agc_loop2_peak_en()) && (g_next_time_dont_check_variance != _TRUE))
    {
        drv_ifd_agc_loop1_peak_en(IFD_DISABLE);
        drv_ifd_agc_loop2_peak_en(IFD_DISABLE);
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "[AGC] Disable peak mode\n");
    }

    agc_tmp = drv_ifd_agc_get_loop2_dgain_output();
    max_targ2 = drv_ifd_agc_loop2_target_get_info();
    peak_err2 = drv_ifd_agc_get_loop2_peak_error();
    peak_max2 = drv_ifd_agc_get_loop2_peak_max();
//	RTD_Log(IFD_LOG_LEVEL, "[AGClp1] %x, %x, %x\n", (UINT32)rtd_inl(0xb8032118), (UINT32)rtd_inl(0xb803215c), (UINT32)rtd_inl(0xb8032160));
//	RTD_Log(IFD_LOG_LEVEL, "[AGClp2] %x, %x, %x, %x\n", (UINT32)rtd_inl(0xb80321a8), (UINT32)rtd_inl(0xb80321b0), (UINT32)rtd_inl(0xb80321bc), (UINT32)rtd_inl(0xb80321c8));

    g_agc_sum = g_agc_sum +(UINT32)agc_tmp;
    //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "[AGC]agc_tmp= %d, max_targ2=%x, peak_err2=%x, g_agc_sum= %d, peak_max2 = %x\n", (UINT32)agc_tmp, (UINT32)max_targ2, (UINT32)peak_err2, (UINT32)g_agc_sum, (UINT32)peak_max2);
    //g_in_peak_status_sum = g_in_peak_status_sum + ((peak_err2 >= 0x800)? 1:0);
    ////IFD_DEBUG_MSG(IFD_LOG_LEVEL, "g_in_peak_status_sum= %d\n", (UINT32)g_in_peak_status_sum);
    g_agc_var_index = g_agc_var_index + 1;
    variance_diff = (INT32)agc_tmp - (INT32)g_agc_avg_pre;
    //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "[AGC]g_agc_avg_pre= %d\n", (UINT32)g_agc_avg_pre);
    if (variance_diff < 0)
        variance_diff = -variance_diff;
    g_agc_var_diff_abs_sum = g_agc_var_diff_abs_sum + (UINT32)variance_diff;
    //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "[AGC]variance_diff= %d\n", (UINT32)variance_diff);
    //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "[AGC]g_agc_var_diff_abs_sum= %d\n", (UINT32)g_agc_var_diff_abs_sum);

    if (peak_err2>=0x800)
        peak_err2_tmp = (INT32)(peak_err2-4096);
    else
        peak_err2_tmp = (INT32)peak_err2;

    g_agc_peak_err_sum += peak_err2_tmp;
    g_agc_peak_err_var_diff_abs_sum += (UINT32)(abs1(peak_err2_tmp-g_agc_peak_err_avg_pre));

    if (g_agc_var_index == (1<< g_agc_cal_count))
    {
        g_agc_peak_err_avg_pre = (g_agc_peak_err_sum >> g_agc_cal_count);
    }

    if ((g_state_now != 4) && (g_state_now != 0))
    {

#if 1 //#ifdef CONFIG_IFD_AGC_VARIANCE_NEW_ALGO
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "[AGC]g_state_now=%d, g_agc_var_index=%d, g_agc_peak_cnt=%d, max_targ2=%x, peak_err2=%x, g_agc_max_taget_max=%x\n",(UINT32)g_state_now, (UINT32)g_agc_var_index, (UINT32)g_agc_peak_cnt, (UINT32)max_targ2, (UINT32)peak_err2, (UINT32)g_agc_max_taget_max);

        if (peak_err2>=0x800)
        {
            if (peak_max2 < 0xfff)
            {
                if (peak_err2>=0xfc0)
                {
                    peak_err2 = 4095 -peak_err2 + 1;
                    if (g_agc_peak_err_low_bound == AGC_PEAK_ERR_LOW_BOUND_DEFAULT)
                        max_targ2 = g_agc_max_taget_max - ((UINT32)peak_err2*MAX_TARGET_RATIO)/1000;
                    else
                        max_targ2 = g_agc_max_taget_max - ((UINT32)peak_err2)/10;
                    if (max_targ2<MAX_TARGET_MIN) max_targ2 = MAX_TARGET_MIN;
                }
                else
                {
                    max_targ2 = ( max_targ2 - MAX_TARGET_MIN)*2/3 +  MAX_TARGET_MIN;
                }
            }
            else
            {
                if (drv_ifd_agc_get_loop2_max_out() < (max_targ2>>2))
                {
                    drv_ifd_agc_loop2_dn_sample(0x00000001);
                    drv_ifd_agc_loop1_peak_en(IFD_ENABLE);
                    g_next_time_dont_check_variance = _TRUE;
                    drv_ifd_agc_loop2_peak_en(IFD_ENABLE);
//					max_targ2 = default_target;
                    //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "[AGC] Enable peak mode\n");
                }
            }

            g_agc_max_taget_max = max_targ2;
            if (g_agc_var_index!=(1<< g_agc_cal_count)) g_agc_peak_cnt++;
        }
        else if ((peak_err2<0x800)&&(peak_err2<g_agc_peak_err_low_bound))
        {
            if (g_agc_var_index!=(1<< g_agc_cal_count)) g_agc_peak_cnt++;
        }
        else //if((peak_err2<0x800)&&(peak_err2>g_agc_peak_err_low_bound)&&(g_agc_var_index==(1<< g_agc_cal_count)))
        {
            if ((g_agc_var_index==(1<< g_agc_cal_count))&&(g_agc_peak_cnt==0)&&(g_agc_peak_err_avg_pre>g_agc_peak_err_low_bound))
            {
                max_targ2 = ((UINT32)(g_agc_peak_err_avg_pre-g_agc_peak_err_low_bound)*MAX_TARGET_RATIO)/1000+ g_agc_max_taget_max;
                if (max_targ2>default_target) max_targ2 = default_target;
                g_agc_max_taget_max = max_targ2;
            }
        }
#else
        if (peak_err2 >= 0x800)
        {
            if (max_targ2 > 0x60)
                max_targ2 = max_targ2 - 0x10;
            else if (max_targ2 > 0x40)
                max_targ2 = max_targ2 - 0x1;
        }
        else if (peak_err2 >= 0x100 && peak_err2 < 0x800 && max_targ2 <= 0x3f0)
        {
            if (max_targ2 >= 0x60)
                max_targ2 = max_targ2 + 0x10;
            else
                max_targ2 = max_targ2 + 0x1;
        }
#endif
        drv_ifd_agc_targ_val(max_targ2);
        drv_ifd_agc_loop2_target(max_targ2);
    }

#ifdef CONFIG_IFD_OUTPUT_CVBS_AMP_COMPENSATE
    if ((g_cvbs_gain_flag==0) &&(max_targ2 < 0x1b0))   //vd dgain full in x1 stage
    {
        drv_ifd_set_videopath_output_gain(IFD_VIDEOPATH_OUTPUT_GAIN_2);
        g_cvbs_gain_flag=1;
    }
    else if ((g_cvbs_gain_flag==1) &&(max_targ2 > 0x1c0))
    {
        drv_ifd_set_videopath_output_gain(IFD_VIDEOPATH_OUTPUT_GAIN_1);
        g_cvbs_gain_flag=0;
    }
    else if ((g_cvbs_gain_flag==1) &&(max_targ2 < 0xc0))   //vd dgain full in x2 stage
    {
        drv_ifd_set_videopath_output_gain(IFD_VIDEOPATH_OUTPUT_GAIN_1);
        g_cvbs_gain_flag=2;
    }
    else if ((g_cvbs_gain_flag==2) &&(max_targ2 > 0xc8))
    {
        drv_ifd_set_videopath_output_gain(IFD_VIDEOPATH_OUTPUT_GAIN_1);
        g_cvbs_gain_flag=1;
    }
#endif

    if (g_agc_var_index == (1<< g_agc_cal_count))
    {
        g_agc_avg_pre = (g_agc_sum >> g_agc_cal_count);
        agc_var_diff_abs_avg = (g_agc_var_diff_abs_sum >> g_agc_cal_count);
        g_state_now = 1;

        if (g_agc_peak_cnt==0)
            g_agc_peak_err_low_bound = ((g_agc_peak_err_low_bound*9 + (UINT32)(g_agc_peak_err_var_diff_abs_sum>>(g_agc_cal_count-2)))/10);
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "[AGC][peak_err]g_agc_peak_err_avg_pre = %d, g_agc_peak_err_var_diff_abs_sum/(2^g_agc_cal_count) = %d, %x\n", (INT32)g_agc_peak_err_avg_pre, (UINT32)(g_agc_peak_err_var_diff_abs_sum>>g_agc_cal_count), (UINT32)g_agc_peak_err_low_bound);
        if (g_agc_peak_err_low_bound > AGC_PEAK_ERR_LOW_BOUND_DEFAULT)
            g_agc_peak_err_low_bound = AGC_PEAK_ERR_LOW_BOUND_DEFAULT;

        g_agc_cal_count++;
        if (g_agc_cal_count > 6)
            g_agc_cal_count = 6;

        if (g_next_time_dont_check_variance == _TRUE)
        {
            if (peak_max2 < 0xfff) g_next_time_dont_check_variance = _FALSE;
            g_state_now = 5;
            //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "[AGC][DONT CHECK AGC]\n");
        }
        else
        {
            if (agc_var_diff_abs_avg >= 0x900)
            {
                drv_ifd_agc_loop2_dn_sample(0x00000001);
                g_state_now = 2;
            }
            else	if (agc_var_diff_abs_avg <= 0x500)
            {
                drv_ifd_agc_loop2_dn_sample(0x0000000f);
                g_state_now = 3;
            }
        }

        //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "[AGC][state = %d]  agc_sum = %x, agc_avg_pre = %x\n", (UINT32)g_state_now, (UINT32)g_agc_sum, (UINT32)g_agc_avg_pre);
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "[AGC][state = %d]  agc_var_diff_abs_sum = %x, agc_var_diff_abs_avg = %x, max_targ = %x\n", (UINT32)g_state_now, (UINT32)g_agc_var_diff_abs_sum, (UINT32)agc_var_diff_abs_avg, (UINT32)max_targ2);
        g_agc_var_index = 0;
        g_agc_sum = 0;
        g_agc_var_diff_abs_sum = 0;
        //g_in_peak_status_sum = 0;

        g_agc_peak_err_sum = 0;
        g_agc_peak_err_var_diff_abs_sum = 0;


#if 1 //#ifdef CONFIG_IFD_AGC_VARIANCE_NEW_ALGO
        g_agc_peak_cnt = 0;
#endif
    }
    //drvif_ifd_agc_tuning_for_strong_signal();
#ifdef CONFIG_IFD_NONSTD_3P10_DETECT
    drvif_ifd_non_std_3p10_detection();
#endif
    IFD_ReactiveTimerEvent(SLRTIMER_IFD_AGC_VAR, AGC_VAR_TIMER, ifd_set_agc_variance_on_active);

#if 0
    UINT16 agc_tmp = 0;
    INT32 variance_diff = 0;
    UINT32 agc_var_diff_abs_avg = 0;
    UINT16 max_targ2 = 0;
    UINT16 peak_err2 = 0;
    bool fine_mode_en = _FALSE;


    fine_mode_en = drv_ifd_agc_loop1_fine_tune_en_get_info();
    if (fine_mode_en == _TRUE)
    {
        drv_ifd_agc_loop1_peak_en(IFD_ENABLE);
        drv_ifd_agc_loop2_peak_en(IFD_ENABLE);
        drv_ifd_agc_loop2_dn_sample(0x0000000f);
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "[SECAMMMMMMMMMMMMMMMMMMMM]\n");
        return;
    }
    else if (drv_ifd_get_agc_loop1_peak_en()||drv_ifd_get_agc_loop2_peak_en())
    {
        drv_ifd_agc_loop1_peak_en(IFD_DISABLE);
        drv_ifd_agc_loop2_peak_en(IFD_DISABLE);

    }

    agc_tmp = drv_ifd_agc_get_loop2_dgain_output();
    max_targ2 = drv_ifd_agc_loop2_target_get_info();
    peak_err2 = drv_ifd_agc_get_loop2_peak_error();

    g_agc_sum = g_agc_sum +(UINT32)agc_tmp;
    //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "agc_tmp= %d, max_targ2=%x, peak_err2=%x, g_agc_sum= %d\n", (UINT32)agc_tmp, (UINT32)max_targ2, (UINT32)peak_err2, (UINT32)g_agc_sum);
    //g_in_peak_status_sum = g_in_peak_status_sum + ((peak_err2 >= 0x800)? 1:0);
    ////IFD_DEBUG_MSG(IFD_LOG_LEVEL, "g_in_peak_status_sum= %d\n", (UINT32)g_in_peak_status_sum);
    g_agc_var_index = g_agc_var_index + 1;
    variance_diff = (INT32)agc_tmp - (INT32)g_agc_avg_pre;
    //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "g_agc_avg_pre= %d\n", (UINT32)g_agc_avg_pre);
    if (variance_diff < 0)
        variance_diff = -variance_diff;
    g_agc_var_diff_abs_sum = g_agc_var_diff_abs_sum + (UINT32)variance_diff;
    //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "variance_diff= %d\n", (UINT32)variance_diff);
    //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "g_agc_var_diff_abs_sum= %d\n", (UINT32)g_agc_var_diff_abs_sum);

#if 1//#ifdef CONFIG_IFD_AGC_VARIANCE_NEW_ALGO
    //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "g_agc_var_index=%d, g_agc_peak_cnt=%d, max_targ2=%x, peak_err2=%x, g_agc_max_taget_max=%x\n", (UINT32)g_agc_var_index, (UINT32)g_agc_peak_cnt, (UINT32)max_targ2, peak_err2, (UINT32)g_agc_max_taget_max);

    if (peak_err2>=0x800)
    {
        if (peak_err2>=0xfc0)
        {
            g_agc_max_taget_max = max_targ2;
            peak_err2 = 4095 -peak_err2 + 1;
            max_targ2 = g_agc_max_taget_max - ((UINT32)peak_err2*MAX_TARGET_RATIO)/1000;
            if (max_targ2<MAX_TARGET_MIN) max_targ2 = MAX_TARGET_MIN;
        }
        else
        {
            g_agc_max_taget_max = max_targ2;
            max_targ2 = ( max_targ2 -MAX_TARGET_MIN)*2/3 +  MAX_TARGET_MIN;
        }
        if (g_agc_var_index!=64) g_agc_peak_cnt++;

    }
    else if ((peak_err2<0x800)&&(peak_err2>0x100)&&(g_agc_var_index==64))
    {
        if (g_agc_peak_cnt==0)
        {
            max_targ2 = ((UINT32)(peak_err2-0x100)*MAX_TARGET_RATIO)/1000+ g_agc_max_taget_max;
            g_agc_max_taget_max = max_targ2;
        }
        else
        {
            g_agc_peak_cnt = 0;
        }
    }
    else if ((peak_err2<0x800)&&(peak_err2<0x100))
    {
        if (g_agc_var_index!=64) g_agc_peak_cnt++;
    }
#else
    if (peak_err2 >= 0x800)
    {
        if (max_targ2 > 0x60)
            max_targ2 = max_targ2 - 0x10;
        else if (max_targ2 > 0x40)
            max_targ2 = max_targ2 - 0x1;

    }
    else if (peak_err2 >= 0x100 && peak_err2 < 0x800 && max_targ2 <= 0x3f0)
    {
        if (max_targ2 >= 0x60)
            max_targ2 = max_targ2 + 0x10;
        else
            max_targ2 = max_targ2 + 0x1;
    }
#endif
    drv_ifd_agc_targ_val(max_targ2);
    drv_ifd_agc_loop2_target(max_targ2);


#ifdef CONFIG_IFD_OUTPUT_CVBS_AMP_COMPENSATE
    if ((g_cvbs_gain_flag==0) &&(max_targ2 < 0x1b0))   //vd dgain full in x1 stage
    {
        drv_ifd_set_videopath_output_gain(IFD_VIDEOPATH_OUTPUT_GAIN_2);
        g_cvbs_gain_flag=1;
    }
    else if ((g_cvbs_gain_flag==1) &&(max_targ2 > 0x1c0))
    {
        drv_ifd_set_videopath_output_gain(IFD_VIDEOPATH_OUTPUT_GAIN_1);
        g_cvbs_gain_flag=0;
    }
    else if ((g_cvbs_gain_flag==1) &&(max_targ2 < 0xc0))   //vd dgain full in x2 stage
    {
        drv_ifd_set_videopath_output_gain(IFD_VIDEOPATH_OUTPUT_GAIN_1);
        g_cvbs_gain_flag=2;
    }
    else if ((g_cvbs_gain_flag==2) &&(max_targ2 > 0xc8))
    {
        drv_ifd_set_videopath_output_gain(IFD_VIDEOPATH_OUTPUT_GAIN_1);
        g_cvbs_gain_flag=1;
    }
#endif

    if (g_agc_var_index == 64)
    {
        g_agc_avg_pre = (g_agc_sum >> 6);
        agc_var_diff_abs_avg = (g_agc_var_diff_abs_sum >> 6);

        if (g_next_time_dont_check_variance == _TRUE)
        {
            g_next_time_dont_check_variance = _FALSE;
            g_state_now = 5;
            //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "[DONTDONTDONT CHECK AGCAGC]\n");
        }
        else
        {
            if (agc_var_diff_abs_avg >= 0x900)
            {
                drv_ifd_agc_loop2_dn_sample(0x00000001);
                g_state_now = 2;
            }
            else	if (agc_var_diff_abs_avg <= 0x500)
            {
                drv_ifd_agc_loop2_dn_sample(0x0000000f);
                g_state_now = 3;
            }
        }


        //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "[AGC state = %d]  agc_sum = %x, agc_avg_pre = %x\n", (UINT32)g_state_now, (UINT32)g_agc_sum, (UINT32)g_agc_avg_pre);
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "[AGC state = %d]  agc_var_diff_abs_sum = %x, agc_var_diff_abs_avg = %x, max_targ = %x\n", (UINT32)g_state_now, (UINT32)g_agc_var_diff_abs_sum, (UINT32)agc_var_diff_abs_avg, (UINT32)max_targ2);
        g_agc_var_index = 0;
        g_agc_sum = 0;
        g_agc_var_diff_abs_sum = 0;
        //g_in_peak_status_sum = 0;
        g_agc_peak_cnt = 0;
    }

#ifdef CONFIG_IFD_NONSTD_3P10_DETECT
    drvif_ifd_non_std_3p10_detection();
#endif
    IFD_ReactiveTimerEvent(SLRTIMER_IFD_AGC_VAR, AGC_VAR_TIMER, ifd_set_agc_variance_on_active);
#endif
}
//=================================================================
/**
 * drvif_ifd_agc_peak_en
 *
 *
 * @param <enable>
 * @return {void}
 */
void drvif_ifd_agc_peak_en(bool enable)
{
    //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "***drvif_ifd_agc_peak_en(%d)\n", enable);
    drv_ifd_agc_loop1_peak_en(enable);
    drv_ifd_agc_loop2_peak_en(enable);
}
#endif
//=================================================================
/**
 * drvif_ifd_vtop_change_with_tunerband
 *
 *
 * @param {void}
 * @return {void}
 */
#ifdef _VIDEO_TV_SUPPORT
#ifndef CONFIG_TUNER_SILICON
void drvif_ifd_vtop_change_with_tunerband()
{
    //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "***drvif_ifd_vtop_change_with_tunerband()\n");

    drv_ifd_agc_vtop(GET_IFD_AGC_VTOP());
    drv_ifd_agc_2step_pga_minimum(GET_IFD_AGC_PGA_MINIMUM());
}
#endif


//=================================================================
/**
 * drvif_ifd_cr_freq_offset_in_100hz_get_info
 *
 *
 * @param {void}
 * @return {shift_freq}
 */
INT32 drvif_ifd_cr_freq_offset_in_100hz_get_info()
{
    UINT32 ifd_offset = 0;
    INT32 shift_freq;

    ifd_offset = drv_ifd_get_cr_freq_offset()>>6;
    //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "ifd_offset = %x\n", (UINT32)ifd_offset);
    if (ifd_offset > 8192)
    {
        shift_freq =  -(((16384-ifd_offset)*540000)>>18);  // Hz
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "shift_freq = %d\n", (UINT32)shift_freq);
    }
    else
    {
        shift_freq = (ifd_offset*540000)>>18;  // Hz
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "shift_freq = %d\n", (UINT32)shift_freq);
    }
    return shift_freq;  // Hz
}
#endif

#if defined(CONFIG_PLATFORM_PACIFIC)||defined(CONFIG_ATLANTIC)||defined(CONFIG_LAS_VEGAS)
//=================================================================
/**
 * drvif_ifd_get_signal_strength
 *
 *
 * @param {void}
 * @return {signal_strength}
 */
UINT32 drvif_ifd_get_signal_strength()
{
    UINT32 rslt=0;

    switch (GET_IFD_AGCPGA_MODE())
    {
    case IFD_AGC_SINGLE_NOIFAMP_PGA :
    case IFD_AGC_DOUBLE_NOIFAMP_PGA :
    case IFD_AGC_2STEP_NOIFAMP_PGA :
    default:
        if (drv_ifd_agc_get_rf_output() < 0x1fff) // ie. 60dB
            rslt = 0;//IFD_SIG_STRONG
        else
        {
            if (drv_ifd_agc_pga_index_get_info() < 70) // ie. 30dB
                rslt = 1;//IFD_SIG_MID;
            else
                rslt = 2;//IFD_SIG_LOW;
        }
        break;
    case IFD_AGC_SINGLE_IFAMP_NOPGA :
    case IFD_AGC_DOUBLE_IFAMP_NOPGA :
    case IFD_AGC_2STEP_IFAMP_NOPGA :
        rslt = 0;//IFD_SIG_STRONG
        break;
    }

    return rslt;
}
#endif
#if defined(CONFIG_PLATFORM_PACIFIC)||defined(CONFIG_ATLANTIC)||defined(CONFIG_LAS_VEGAS)
//=================================================================
/**
 * drvif_ifd_pre_distortioned_signal_detection
 *
 *
 * @param {void}
 * @return {void}
 */
void drvif_ifd_pre_distortioned_signal_detection()
{
    //remember different AGC mode need to observe different gain!!

    UINT16 target =0x35;
    UINT8 shift = 2;
    UINT8 delay = 100;
    UINT16 old_target;
    UINT8 old_shift;
    INT32 cr_offset;
    INT32 rf1,rf2,pga1,pga2;
    INT32 dgain1,dgain2;
    cr_offset = drvif_ifd_cr_freq_offset_in_100hz_get_info();

    if ((cr_offset<-400)||(cr_offset>400)) // if  cr offset is not within 50K don't detect
    {
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL,"[shape] freq shift don't check predistortioned signal\n");
        return;
    }
    old_target = drv_ifd_agc_targ_val_get_info();
    old_shift = drv_ifd_get_agc_loop1_pishift_settings();
    //target = old_target /10;
    if (GET_IFD_AGCPGA_MODE() == IFD_AGC_2STEP_NOIFAMP_PGA)
    {
        INT32 rf_thd = 400;  //  816
        INT32 pga_thd = 120;
        INT32 dgain_thd = 5000;
        drv_ifd_agc_loop1_pishift_settings(shift);

        drv_ifd_set_cr_bpf_coef(IFD_ENABLE, nyquist_detect_BPF_Map[0][g_curIFMapIdx]);
        drv_ifd_agc_targ_val(target);
        drv_ifd_agc_loop2_target(target);
        ScalerTimer_DelayXms(delay);
        rf1 = drv_ifd_agc_get_rf_output();
        pga1 = drv_ifd_agc_get_pga_code();
        dgain1 = drv_ifd_agc_get_loop2_dgain_output();
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL,"[shape][0]agc lock = %d, rf = %x, pga =%x,dgain1 =%x\n",drvif_ifd_get_agc_lock(),rf1,pga1,dgain1);
        drv_ifd_set_cr_bpf_coef(IFD_ENABLE, nyquist_detect_BPF_Map[1][g_curIFMapIdx]);
        drv_ifd_agc_targ_val(target);
        drv_ifd_agc_loop2_target(target);
        ScalerTimer_DelayXms(delay);
        rf2 = drv_ifd_agc_get_rf_output();
        pga2 = drv_ifd_agc_get_pga_code();
        dgain2 = drv_ifd_agc_get_loop2_dgain_output();
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL,"[shape][1]agc lock = %d, rf = %x, pga =%x, dgain2 =%x, div_rf = %d,div_pga = %d,div_dgain =%d\n",drvif_ifd_get_agc_lock(),rf2,pga2,dgain2 ,rf1-rf2,pga1-pga2,dgain1-dgain2);

        if (drv_ifd_rf_full_get_info())
        {
            if (pga1-pga2 > pga_thd) // for weak signal observe pga
                drv_ifd_set_shaping_filter(IFD_SHAPING_BW_SEL_BYPASS);
            else if (dgain1-dgain2 > dgain_thd)
                drv_ifd_set_shaping_filter(IFD_SHAPING_BW_SEL_BYPASS);
            else
                drv_ifd_set_shaping_filter(IFD_SHAPING_BW_SEL_0P86M);
        }
        else
        {
            if (rf1-rf2 > rf_thd) // for strong signal observe rf
                drv_ifd_set_shaping_filter(IFD_SHAPING_BW_SEL_BYPASS);
            else
                drv_ifd_set_shaping_filter(IFD_SHAPING_BW_SEL_0P86M);
#if 0
#ifdef CONFIG_S16_NonStandard_Function
//			if(rf1-rf2 > 1200)
            if (non_std_cr_fast_flag==IFD_ENABLE) //_osTvnostand_xinxiang
            {
                drv_ifd_cr_loop_settings(IFD_CR_LOOP_FAST);
                //IFD_DEBUG_MSG(IFD_LOG_LEVEL,"[shape] shangdong!!!!\n");
            }
#endif
#endif
        }
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL,"[shape] result with shaping filter = %x\n",rtd_inl(IFD_SHAPE_FILTER_SELECTION_VADDR));

        drv_ifd_set_cr_bpf_coef(IFD_ENABLE, crBPFMap[g_curIFMapIdx]);
        drv_ifd_agc_targ_val(old_target);
        drv_ifd_agc_loop2_target(old_target);
        drv_ifd_agc_loop1_pishift_settings(old_shift);

        ScalerTimer_DelayXms(delay);
        ScalerTimer_DelayXms(delay);
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL,"[shape][ori]agc lock = %d, rf = %x, pga =%x,target= %x, shift = %d\n",drvif_ifd_get_agc_lock(),drv_ifd_agc_get_rf_output(),drv_ifd_agc_get_pga_code(),target,old_shift);

    }

    /*
    	else if(GET_IFD_AGCPGA_MODE() == IFD_AGC_DOUBLE_IFAMP_NOPGA)
    	{

    		drv_ifd_agc_mode_sel(IFD_AGC_MODE_2LOOP);


    		INT32 acc3_thd ;  //  816
    		INT32 acc31,acc32;

    		drv_ifd_set_cr_bpf_coef(IFD_ENABLE, nyquist_detect_BPF_Map[0][g_curIFMapIdx]);
    		drv_ifd_agc_targ_val(target);
    		ScalerTimer_DelayXms(delay);
    		acc31 = drv_ifd_agc_get_rf_output();
    		//IFD_DEBUG_MSG(IFD_LOG_LEVEL,"[shape][0]agc lock = %d, acc31 = %x\n",drvif_ifd_get_agc_lock(),acc31);
    		drv_ifd_set_cr_bpf_coef(IFD_ENABLE, nyquist_detect_BPF_Map[1][g_curIFMapIdx]);
    		drv_ifd_agc_targ_val(target);
    		ScalerTimer_DelayXms(delay);
    		acc32 = drv_ifd_agc_get_rf_output();
    		//IFD_DEBUG_MSG(IFD_LOG_LEVEL,"[shape][1]agc lock = %d, acc32 =%x, div_acc3 = %d\n",drvif_ifd_get_agc_lock(),acc32, acc31-acc32);

    		if(acc31-acc32 > acc3_thd)  // for weak signal observe pga
    			drv_ifd_set_shaping_filter(IFD_SHAPING_BW_SEL_BYPASS);
    		else if(dgain1-dgain2 > dgain_thd)
    			drv_ifd_set_shaping_filter(IFD_SHAPING_BW_SEL_BYPASS);
    		else
    			drv_ifd_set_shaping_filter(IFD_SHAPING_BW_SEL_0P86M);
    		//IFD_DEBUG_MSG(IFD_LOG_LEVEL,"[shape] result with shaping filter = %x\n",rtd_inl(IFD_SHAPE_FILTER_SELECTION_VADDR));


    		drv_ifd_set_cr_bpf_coef(IFD_ENABLE, crBPFMap[g_curIFMapIdx]);
    		drv_ifd_agc_targ_val(old_target);
    		drv_ifd_agc_loop2_target(old_target);
    		drv_ifd_agc_loop1_pishift_settings(old_shift);

    		ScalerTimer_DelayXms(delay);
    		ScalerTimer_DelayXms(delay);
    		//IFD_DEBUG_MSG(IFD_LOG_LEVEL,"[shape][ori]agc lock = %d, rf = %x, pga =%x,target= %x, shift = %d\n",drvif_ifd_get_agc_lock(),drv_ifd_agc_get_rf_output(),drv_ifd_agc_get_pga_code(),target,old_shift);


    	}

    	*/

}
#ifdef CONFIG_S16_NonStandard_Function
//=================================================================
/**
 * drvif_ifd_non_std_cr_fast
 *
 *
 * @param {on_off}
 * @return {void}
 */
void drvif_ifd_non_std_cr_fast(bool on_off)
{
    if (on_off)
        non_std_cr_fast_flag = IFD_ENABLE;
    else
        non_std_cr_fast_flag = IFD_DISABLE;
}
#endif
#endif


#ifdef _VIDEO_TV_SUPPORT
/**
 * drvif_ifd_afc_auto_tracking
 *
 *
 * @param {curr_freq/afc_finetune_en}
 * @return {status}
 */
#ifdef CONFIG_TUNER_SILICON
bool drvif_ifd_afc_auto_tracking(UINT32* curr_freq)
{

// 	return _TRUE;

    bool status=_FALSE;
    UINT8 afc=0;
    INT32 iAfcOffset=0;
    UINT8 video_standard;
    UINT32 freq;
    UINT32 original_freq;
    bool cr_lock_flag, agc_lock_flag;

    video_standard=g_curr_std;
    original_freq= * curr_freq;

    //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "***[afc]*drvif_ifd_afc_auto_tracking(%d),(tuner freq=%d)\n", (UINT32)*curr_freq, (UINT32)g_afc_curr_freq);
    //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "***[afc]*video_standard = %x, g_afc_stage_count=%d\n",(UINT32)video_standard, (UINT32) g_afc_stage_count);

#ifdef CONFIG_TUNER_SILICON
    if (g_afc_stage_count<3)
#else
    if (g_afc_stage_count<GET_IFD_AFC_STAGE_MAX_COUNT())
#endif
    {
        g_afc_stage_count ++;
        return status;
    }
    else
    {
        g_afc_stage_count = 1;
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "***[afc]*g_afc_stage_count change from 15 to %d\n",(UINT32) g_afc_stage_count);
    }

    freq = g_afc_center_freq;

#if defined(CONFIG_PLATFORM_PACIFIC)||defined(CONFIG_ATLANTIC)||defined(CONFIG_LAS_VEGAS)
    if ((drvif_module_vdc_CheckHVisLocked()==_SUCCESS)&&(drv_ifd_get_cr_lock_status()==_TRUE))
#else
    if ((drv_video_vdc_CheckHVisLocked()==_VDC_SUCCESS)&&(drv_ifd_get_cr_lock_status()==_TRUE))
#endif
    {
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL,"***[afc]*HV lock & CR lock\n");
        if (drvif_tuner_is_locked(&afc) != _TRUE)
        {		// RF Tuner mislock, AFC tracking action...
            //IFD_DEBUG_MSG(LOGGER_DEBUG,"***[afc]*HV lock but Tuner mislock...\n");
            iAfcOffset = drvif_tuner_get_afcoffset(afc);
            //IFD_DEBUG_MSG(LOGGER_DEBUG,"***[afc]*AFC offset = %d\n",iAfcOffset);
            g_afc_curr_freq += iAfcOffset;
            if (g_afc_curr_freq>= g_afc_center_freq+AFC_FREQ_STEP*2)
                g_afc_curr_freq = g_afc_center_freq+AFC_FREQ_STEP*2;
            if (g_afc_curr_freq<= g_afc_center_freq-AFC_FREQ_STEP*2)
                g_afc_curr_freq = g_afc_center_freq-AFC_FREQ_STEP*2;
            drvif_ifd_cr_freq_status_check(IFD_CR_STATE_SET_TUNER);
            drv_tuner_SetFreq(g_afc_curr_freq);
            ScalerTimer_DelayXms(15);
//				g_afc_stage_count = 0;
        }
        status = _TRUE;
//		g_afc_direction = 0;
        g_afc_hvunlock_debounce = 0;
    }
#ifndef CONFIG_TUNER_SILICON
    else if ((drv_ifd_get_cr_lock_status()==_TRUE)&&(g_afc_hvunlock_debounce<AFC_HV_UNLOCK_DEBOUNCE_COUNT))
    {
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL,"***[afc]*HV unlock & CR lock (%d)\n", (UINT32)g_afc_hvunlock_debounce);

        status = _FALSE;
#ifdef CONFIG_TUNER_SILICON
        g_afc_stage_count = 3;
#else
        g_afc_stage_count =GET_IFD_AFC_STAGE_MAX_COUNT();
#endif
        g_afc_hvunlock_debounce++;
    }
#endif
    else
    {
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL,"***[afc]*HV unlock & CR unlock\n");

        status = _FALSE;
#ifdef CONFIG_TUNER_SILICON
        g_afc_stage_count = 3;
#else
        g_afc_stage_count =GET_IFD_AFC_STAGE_MAX_COUNT();
#endif
        g_afc_hvunlock_debounce = AFC_HV_UNLOCK_DEBOUNCE_COUNT;

        switch (g_afc_direction)
        {
        case 0:
            freq += 0;
            break;
        case 1:
            freq += (AFC_FREQ_STEP<<1);
            break;
        case 2:
            freq += 0;
            break;
        case 3:
            freq -= (AFC_FREQ_STEP<<1);
            break;
        default:
            freq += 0;
            break;
        }
        g_afc_direction++;
        g_afc_direction %= 4;
        g_afc_curr_freq = freq;
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL,"***[afc]*g_afc_curr_freq=%d, g_afc_stage_count=%d, g_afc_direction=%d\n", (UINT32)g_afc_curr_freq, (UINT32)g_afc_stage_count, (UINT32)g_afc_direction);
        drvif_ifd_cr_freq_status_check(IFD_CR_STATE_SET_TUNER);
        drv_tuner_SetFreq(g_afc_curr_freq);
        ScalerTimer_DelayXms(50);

        cr_lock_flag = drvif_ifd_get_cr_lock();
        agc_lock_flag = drvif_ifd_get_agc_lock();
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL,"***[afc]*cr lock =%d, agc lock =%d\n", (UINT32)cr_lock_flag,(UINT32)agc_lock_flag);
        if ((cr_lock_flag == _TRUE) && (agc_lock_flag == _TRUE))
        {
            iAfcOffset = drvif_tuner_get_afcoffset(afc);
            g_afc_curr_freq += iAfcOffset;
            if (g_afc_curr_freq>= g_afc_center_freq+AFC_FREQ_STEP*2)
                g_afc_curr_freq = g_afc_center_freq+AFC_FREQ_STEP*2;
            if (g_afc_curr_freq<= g_afc_center_freq-AFC_FREQ_STEP*2)
                g_afc_curr_freq = g_afc_center_freq-AFC_FREQ_STEP*2;
            //IFD_DEBUG_MSG(LOGGER_DEBUG,"***[afc]* AFC  find signal! AFC offset = %d, g_afc_curr_freq=%d\n", (UINT32)iAfcOffset, (UINT32)g_afc_curr_freq);
            drvif_ifd_cr_freq_status_check(IFD_CR_STATE_SET_TUNER);
            drv_tuner_SetFreq(g_afc_curr_freq);
            ScalerTimer_DelayXms(20);
            drvif_ifd_set_cr_state(drv_video_vdc_CheckHVisLocked());
            //irene mark @100305  g_afc_direction= 0;
            g_afc_stage_count = 0;
            g_afc_hvunlock_debounce = 0;
        }

    }

    if ((video_standard == g_curr_std)&&(g_afc_curr_freq<= g_afc_center_freq+AFC_FREQ_STEP*2)&&(g_afc_curr_freq >= g_afc_center_freq-AFC_FREQ_STEP*2)) //avoid interrupt bug
    {
        //transfer tuner freq to channel freq
        if (g_curr_std ==  ZSECAMLA)
            * curr_freq = g_afc_curr_freq + SECAMLA_FREQ_SHIFT;
        else
            * curr_freq = g_afc_curr_freq;

        return status;
    }
    else
    {
        //IFD_DEBUG_MSG(LOGGER_DEBUG,"***[afc]*inturrupt may occur. don't update status!\n");
        return _FALSE;
    }

}
#endif
#if 0
//=================================================================
/**
 * drvif_ifd_afc_auto_tracking
 *
 *
 * @param {curr_freq/afc_finetune_en}
 * @return {status}
 */
#ifndef CONFIG_TUNER_SILICON
bool drvif_ifd_afc_auto_tracking(UINT32* curr_freq)
{
    bool status=_FALSE;
    UINT8 afc=0;
    INT32 iAfcOffset=0;
    UINT8 video_standard;
    UINT32 freq;
    UINT32 original_freq;
    bool cr_lock_flag, agc_lock_flag;

    video_standard=g_curr_std;
    original_freq= * curr_freq;

    //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "drvif_ifd_afc_auto_tracking(%d)\n", (UINT32)*curr_freq);
    //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "****video_standard = %x\n",(UINT32)video_standard);

    if (g_afc_stage_count<GET_IFD_AFC_STAGE_MAX_COUNT())
    {
        g_afc_stage_count ++;
        return status;
    }
    else
    {
        g_afc_stage_count = 0;
    }
    //transfer channel freq to tuner freq
    if (g_curr_std ==  ZSECAMLA)
        freq = * curr_freq - SECAMLA_FREQ_SHIFT;
    else
        freq = * curr_freq;

#if defined(CONFIG_PLATFORM_PACIFIC)||defined(CONFIG_ATLANTIC)||defined(CONFIG_LAS_VEGAS)
    if ((drvif_module_vdc_CheckHVisLocked()==_SUCCESS)&&(drv_ifd_get_cr_lock_status()==_TRUE))
#else
    if ((drv_video_vdc_CheckHVisLocked()==_VDC_SUCCESS)&&(drv_ifd_get_cr_lock_status()==_TRUE))
#endif
    {
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL,"****HV lock & CR lock\n");
        if (drvif_tuner_is_locked(&afc) != _TRUE)
        {		// RF Tuner mislock, AFC tracking action...
            //IFD_DEBUG_MSG(LOGGER_DEBUG,"****HV lock but Tuner mislock...\n");
            iAfcOffset = drvif_tuner_get_afcoffset(afc);
            //IFD_DEBUG_MSG(LOGGER_DEBUG,"****AFC offset = %d\n",iAfcOffset);
            g_afc_curr_freq += iAfcOffset;
            drvif_ifd_cr_freq_status_check(IFD_CR_STATE_SET_TUNER);
            drv_tuner_SetFreq(g_afc_curr_freq);
            ScalerTimer_DelayXms(15);
            g_afc_stage_count = 0;
        }
        status = _TRUE;
        g_afc_direction = 0;

    }
    else
    {
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL,"****HV or CR not lock\n");

        status = _FALSE;

        g_afc_stage_count =GET_IFD_AFC_STAGE_MAX_COUNT();

        switch (g_afc_direction)
        {
        case 0:
            freq -= AFC_FREQ_STEP;
            break;
        case 1:
            freq += AFC_FREQ_STEP;
            break;
        case 2:
            freq -= AFC_FREQ_STEP*2;
            break;
        case 3:
            freq += AFC_FREQ_STEP*2;
            break;
        default:
            freq += 0;
            break;
        }
        g_afc_direction++;
        if ( g_afc_direction> 4)
        {
            g_afc_direction= 0;
        }
        g_afc_curr_freq = freq;
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL,"****g_afc_curr_freq=%d, g_afc_stage_count=%d, g_afc_direction=%d\n", g_afc_curr_freq, g_afc_stage_count, g_afc_direction);
        drvif_ifd_cr_freq_status_check(IFD_CR_STATE_SET_TUNER);
        drv_tuner_SetFreq(g_afc_curr_freq);
        ScalerTimer_DelayXms(30);


        cr_lock_flag = drvif_ifd_get_cr_lock();
        agc_lock_flag = drvif_ifd_get_agc_lock();
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL,"****cr lock =%d, agc lock =%d\n", cr_lock_flag,agc_lock_flag);
        if ((cr_lock_flag == _TRUE) && (agc_lock_flag == _TRUE))
        {
            iAfcOffset = drvif_tuner_get_afcoffset(afc);
            g_afc_curr_freq += iAfcOffset;
            //IFD_DEBUG_MSG(LOGGER_DEBUG,"**** AFC  find signal! AFC offset = %d, g_afc_curr_freq=%d\n", iAfcOffset, g_afc_curr_freq);
            drvif_ifd_cr_freq_status_check(IFD_CR_STATE_SET_TUNER);
            drv_tuner_SetFreq(g_afc_curr_freq);
            ScalerTimer_DelayXms(20);
            drvif_ifd_set_cr_state(2);
            //g_afc_direction= 0;
            g_afc_stage_count = 0;
        }

    }

    if ((video_standard == g_curr_std)&&(g_afc_curr_freq<= original_freq+AFC_FREQ_STEP*2)&&(g_afc_curr_freq >= original_freq-AFC_FREQ_STEP*2)) //avoid interrupt bug
    {
        //transfer tuner freq to channel freq
        if (g_curr_std ==  ZSECAMLA)
            * curr_freq = g_afc_curr_freq + SECAMLA_FREQ_SHIFT;
        else
            * curr_freq = g_afc_curr_freq;

        return status;
    }
    else
    {
        //IFD_DEBUG_MSG(LOGGER_DEBUG,"****[AFC]inturrupt may occur. don't update status!\n");
        return _FALSE;
    }
}
#endif
#endif

//=================================================================
/**
 * drvif_ifd_set_cr_state
 *
 *
 * @param {swap}
 * @return {void}
 */

void drvif_ifd_set_cr_state(UINT8 mode)
{

    static UINT8 mode_pre=0;
#ifdef CONFIG_IFD_CR_VARIANCE_DETECT
    static IFD_CR_LOOP_STATE cur_state;
#endif
    UINT8 cr_offset_counter  = 4;  // 3-> 8 point mean; 4-> 16 point mean
    UINT16 cr_variance_thd = 700;
    UINT8 length = 0;
    INT32 offset_max = 0;
    INT32 offset_min = 0;
    UINT8 offset_max_index = 0;
    UINT8 offset_min_index = 0;
    UINT8 offset_index = 0;
    INT32 sum = 0;
    INT32 avg = 0;
    INT32 offset[20]; // linmc@20100110, prevent sp jump
    INT32 variance = 0;
    UINT32 pre_freq;
    UINT8 video_standard;
    pre_freq = CTunerGetFreq(); // irene add avoid interrupt
    video_standard = g_curr_std;

    drv_ifd_set_cr_error_var_sel(1);
    drv_ifd_set_agc_cr_var_sel(0);
    drv_ifd_cr_wblf_reset(1);
    drv_ifd_cr_wblf_reset(0);

    //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "***drvif_ifd_set_cr_state(%x, pre=%x)\n", (UINT32)mode, (UINT32)mode_pre);
// mode = 0 : h/v unlock, do nothing
// mode = 1 : h/v lock, do "cr bw check" as from unlock to lock
// mode = 2 : force to do "cr bw check"

    if ((mode != 2))
    {
        if ((mode==1) && (mode_pre==0))  	//no signal to signal continue this process
        {
            mode_pre = mode;
            // do detection
        }
        else
        {
            mode_pre = mode;
            return;
        }
    }

    drv_ifd_cr_loop_settings(IFD_CR_LOOP_SLOW);
    mode_pre = mode;
    drvif_ifd_set_agc_variance_off(1);
    drvif_ifd_set_agc_variance_on();

    //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "***drvif_ifd_set_cr_state(%x, pre=%x) non-return\n", (UINT32)mode, (UINT32)mode_pre);
    ScalerTimer_DelayXms(50);

    if (IFD_CHIP_VER() < 2)
    {
        length = (1 << cr_offset_counter) + 2;
        cr_variance_thd = cr_variance_thd>>(4-cr_offset_counter);
        offset[offset_index] = drvif_ifd_cr_freq_offset_in_100hz_get_info();
        offset_max = offset[offset_index] ;
        offset_min = offset[offset_index] ;
        sum = offset[offset_index];
        offset_index=offset_index+1;
        do
        {
            offset[offset_index] = drvif_ifd_cr_freq_offset_in_100hz_get_info();
            if (offset[offset_index]  > offset_max)
            {
                offset_max = offset[offset_index] ;
                offset_max_index = offset_index;
            }
            else if (offset[offset_index]  < offset_min)
            {
                offset_min = offset[offset_index] ;
                offset_min_index = offset_index;
            }
            sum = sum + offset[offset_index];
            //IFD_DEBUG_MSG(IFD_LOG_LEVEL,"[CR state0] offset_max = %d, offset_min = %d, offset= %d, cr lock = %d\n", offset_max, offset_min, offset[offset_index],drvif_ifd_get_cr_lock());
            ScalerTimer_DelayXms(1);
            offset_index = offset_index+1;
        }
        while (offset_index < length);

        sum = sum - offset_min - offset_max; //discard min & max
        avg = sum>>	cr_offset_counter;
        for (offset_index = 0; offset_index< length; offset_index++)
        {
            if ((offset_index!= offset_min_index)&&(offset_index!= offset_max_index))
            {
                if (offset[offset_index] >= avg)
                    variance = offset[offset_index] - avg + variance;
                else
                    variance = avg - offset[offset_index] + variance;
            }
        }
    }
    else
    {
        cr_variance_thd = 0x88;
        variance = drv_ifd_cr_phase_error_variance_get_info();
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "$$$$$$IFD:cr_variance_thd=%x, variance=%x\n", cr_variance_thd, variance);
    }

    //IFD_DEBUG_MSG(IFD_LOG_LEVEL,"[CR state1] sum = %d, avg = %d, var = %d\n", sum, avg, variance);

    if ((CTunerGetFreq() != pre_freq)||(video_standard != g_curr_std)) // irene add avoid interrupt
    {
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL,"[CR state1] interrupt occur discard result!\n");
        mode_pre = 0;
        return;
    }
#ifdef CONFIG_IFD_CR_VARIANCE_DETECT
    if (variance >= cr_variance_thd)
    {
        drv_ifd_cr_loop_settings(IFD_CR_LOOP_FAST);
        cur_state = IFD_CR_LOOP_FAST;
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL,"[CR state2] variance is too big!! cur_freq = %d, cur_std = %d, pi = %x\n",CTunerGetFreq() ,g_curr_std,rtd_inl(IFD_CR_PGAIN_VADDR));
    }
    else
    {
        drv_ifd_cr_loop_settings(IFD_CR_LOOP_SLOW);
        cur_state = IFD_CR_LOOP_SLOW;
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL,"[CR state2]  pass!! cur_freq = %d, cur_std = %d, pi = %x \n",CTunerGetFreq() ,g_curr_std,rtd_inl(IFD_CR_PGAIN_VADDR));
    }
#else
    if (non_std_cr_fast_flag==IFD_ENABLE) //_osTvnostand_xinxiang
    {
        drv_ifd_cr_loop_settings(IFD_CR_LOOP_FAST);
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL,"[shape] shangdong!!!!\n");
    }
    else
    {
        drv_ifd_cr_loop_settings(IFD_CR_LOOP_SLOW);
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL,"[CR state2] no detection fix at slow mode. \n");
    }
#endif
}
#endif

#ifdef CONFIG_IFD_NONSTD_3P10_DETECT
//=================================================================
/**
 * drvif_ifd_non_std_3p10_detection
 *
 *
 * @param {void}
 * @return {void}
 */
void drvif_ifd_non_std_3p10_detection(void)
{
    UINT16 dgain[_IFD_NONSTD_STABLE_CNTNUM], dgain_max, dgain_min;
    UINT32 dgain_avg;
    UINT32 dgain_tmp1, dgain_tmp2;
    UINT8 i;

    //read dgain/cgain
    dgain_max=0;
    dgain_min=0xffff;

    for (i=0; i<_IFD_NONSTD_STABLE_CNTNUM; i++)
    {
        dgain[i]=((rtd_inl(VDC_AGC_DGAIN_STATUS2_VADDR)&0xf)<<8) | rtd_inl(VDC_AGC_DGAIN_STATUS1_VADDR);
        if (dgain[i]>dgain_max) dgain_max = dgain[i];
        if (dgain[i]<dgain_min) dgain_min = dgain[i];
    }
    //RTD_Log(IFD_LOG_LEVEL,"dgain=%x, %x, %x, %x, %x, %x, %x, %x, %x, %x\n", dgain[0], dgain[1], dgain[2], dgain[3], dgain[4], dgain[5], dgain[6], dgain[7], dgain[8], dgain[9]);
    //If dgain and cgain are stable, to caculate the avg of dgain and cgain
    if ((dgain_max-dgain_min)<10)
    {
        dgain_avg = dgain[0];
        for (i=1; i<_IFD_NONSTD_STABLE_CNTNUM; i++)
        {
            dgain_avg = dgain_avg + dgain[i];
        }
        dgain_avg = dgain_avg/_IFD_NONSTD_STABLE_CNTNUM;
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL,"dgain_avg=%x\n", dgain_avg);

        //compare
        dgain_tmp1 = ABS(dgain_avg,_IFD_NONSTD_DGAIN_200);
        dgain_tmp2 = ABS(dgain_avg,_IFD_NONSTD_DGAIN_50);
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL,"dgain_tmp1=%x;dgain_tmp2=%x\n", dgain_tmp1, dgain_tmp2);

        //IFD_DEBUG_MSG(IFD_LOG_LEVEL,"flag_chessboard=%x\n", drvif_color_get_flag_chessboard());
        if (((dgain_tmp1<_IFD_NONSTD_DGAIN_THR)||(dgain_tmp2<_IFD_NONSTD_DGAIN_THR))&&(drvif_color_get_flag_chessboard()==_TRUE))
        {
            rtdf_maskl(VDC_AVOUT_FIFO_STATUS_VADDR, 0x1f, 0xa0);//Switch AVout Mux to  IFD out
            //IFD_DEBUG_MSG(IFD_LOG_LEVEL,"Enter Non STD\n");
        }
        else
        {
            rtdf_maskl(VDC_AVOUT_FIFO_STATUS_VADDR, 0x1f, 0x40);//Switch AVout Mux to  normal path
            //IFD_DEBUG_MSG(IFD_LOG_LEVEL,"Enter Normal STD\n");
        }
    }
}
#endif
/*=============================================================*/
/**
 * drvif_ifd_set_videopath_output_gain
 *
 *
 * @param <data_gain>
 * @return { void }
 */
void drvif_ifd_set_videopath_output_gain(IFD_VIDEOPATH_OUTPUT_GAIN data_gain)
{
    drv_ifd_set_videopath_output_gain(data_gain);
}
#ifdef _VIDEO_TV_SUPPORT
//=================================================================
//IFD local driver
//=================================================================
//=================================================================
/**
 * drv_ifd_clamp_reset
 *
 *
 * @param {void}
 * @return {void}
 */
void drv_ifd_clamp_reset(IFD_CLAMP_RESET_POLARITY polarity)
{
    IFD_DRV_DEBUG_MSG(IFD_LOG_LEVEL, "*****drv_ifd_clamp_reset() polarity = %d\n",polarity);

    if (polarity == IFD_CLAMP_RESET_POS)
    {
        drv_ifd_set_clamping_thr_max(IFD_CLAMP_MID_THD_POS);
        drv_ifd_set_clamping_thr_min(IFD_CLAMP_MID_THD_POS);
        ScalerTimer_DelayXms(1);
        drv_ifd_set_clamping_thr_max(IFD_CLAMP_MAX_THD);
        drv_ifd_set_clamping_thr_min(IFD_CLAMP_MIN_THD);
    }
    else
    {
        drv_ifd_set_clamping_thr_max(IFD_CLAMP_MID_THD_NEG);
        drv_ifd_set_clamping_thr_min(IFD_CLAMP_MID_THD_NEG);
        ScalerTimer_DelayXms(1);
        drv_ifd_set_clamping_thr_max(IFD_CLAMP_MAX_THD);
        drv_ifd_set_clamping_thr_min(IFD_CLAMP_MIN_THD);
    }
}
//=================================================================
/**
 * drv_ifd_set_video_ntsc
 *
 *
 * @param {void}
 * @return {void}
 */
void drv_ifd_set_video_ntsc(void)
{
    IFD_DRV_DEBUG_MSG(IFD_LOG_LEVEL, "*****drv_ifd_set_video_ntsc()\n");
#ifdef CONFIG_TUNER_SILICON

if(!(tvAutoScanFlag || tvManualScanFlag))
    {
        drv_ifd_set_cr_bpf_coef(IFD_ENABLE, crBPFMap[4]);
#ifdef CONFIG_TUNER_SILICON
        drv_ifd_set_cr_nco(IFD_ENABLE, ncoIFMap[0]);
#else
        drv_ifd_set_cr_nco(IFD_ENABLE, ncoIFMap[g_curIFMapIdx]);
#endif
        //IFD_DEBUG_MSG(LOGGER_LEVEL, "crcr NTSC\n");
    }
    else
    {
        drv_ifd_set_cr_bpf_coef(IFD_ENABLE, crBPFMap[3]);
        drv_ifd_set_cr_nco(IFD_ENABLE, ncoIFMap[1]);
        //IFD_DEBUG_MSG(LOGGER_LEVEL, "crcr PALLLLL\n");
    }
#else
    drv_ifd_set_cr_bpf_coef(IFD_ENABLE, crBPFMap[g_curIFMapIdx]);
    drv_ifd_set_cr_nco(IFD_ENABLE, ncoIFMap[g_curIFMapIdx]);
#endif //#ifdef CONFIG_TUNER_SILICON

#ifdef CONFIG_TUNER_SILICON
#if defined(CONFIG_ATLANTIC)||defined(CONFIG_LAS_VEGAS)
#else
    drv_ifd_set_eq_coef(eq_coef_non_secamla[0]);
#endif
#else //#ifdef CONFIG_TUNER_SILICON
#if defined(CONFIG_ATLANTIC)||defined(CONFIG_LAS_VEGAS)
#else
    drv_ifd_set_eq_coef(eq_coef_non_secamla[g_curIFMapIdx]);
#endif
#endif //#ifdef CONFIG_TUNER_SILICON

    drv_ifd_set_pos_neg_mod(IFD_MODULATION_SEL_NEG);
    drv_ifd_set_video_audio_nf_bw(GET_IFD_VIDEOBPF_NTSC(), GET_IFD_AUDIOBPF_NF2NTSC(), GET_IFD_AUDIOBPF_NF1NTSC(), GET_IFD_VIDEOUPSAMPLE_LPF_NTSC());

    drv_ifd_set_shaping_filter(IFD_SHAPING_BW_SEL_0P86M);
#ifdef CONFIG_TUNER_SILICON
    drv_ifd_set_shaping_img(IFD_SHAPING_IMG_POS);
#else
    drv_ifd_set_shaping_img(IFD_SHAPING_IMG_NEG);
#endif
    drv_ifd_agc_loop1_pos_en(IFD_DISABLE);
    drv_ifd_agc_loop2_pos_en(IFD_DISABLE); // irene's code without this, irene has to check
    drv_ifd_agc_vsync_start2(IFD_VSmode2_Vstart_NTSC, IFD_VSmode2_Vend_NTSC);
    drv_ifd_agc_targ_val(GET_IFD_AGCLOOP1_MAXTARGET_NTSC());
    drv_ifd_agc_loop2_target(GET_IFD_AGCLOOP2_MAXTARGET_NTSC());
    drv_ifd_agc_hsync_start1(IFD_VSmode1_Hstart_NTSC, IFD_VSmode1_Hend_NTSC);
    drv_ifd_agc_vsync_start1(IFD_VSmode1_Vstart_NTSC, IFD_VSmode1_Vend_NTSC); //irene ok// irene has to check why w/o vsync_start2/hsync_start2
    drv_ifd_agc_hsync_start2(IFD_VSmode2_Hstart_NTSC, IFD_VSmode2_Hend_NTSC);
    drv_ifd_agc_vsync_start2(IFD_VSmode2_Vstart_NTSC, IFD_VSmode2_Vend_NTSC);
    rtd_outl(IFD_AGC_ENABLE_SEL_VADDR, 0x000000FE);  //agc enable
    drv_ifd_agc_loop1_fine_tune_en(IFD_DISABLE);
    drv_ifd_agc_loop2_fine_tune_en(IFD_DISABLE);
    drv_ifd_group_delay_coe_table(GET_IFD_GRPDEALY_NTSC());
}
//=================================================================
/**
 * drv_ifd_set_video_pal
 *
 *
 * @param <mode>
 * @return {void}
 */
void drv_ifd_set_video_pal(void/*UINT8 mode*/)
{
    IFD_DRV_DEBUG_MSG(IFD_LOG_LEVEL, "*****drv_ifd_set_video_pal()\n");
#ifdef CONFIG_TUNER_SILICON
    drv_ifd_set_cr_bpf_coef(IFD_ENABLE, crBPFMap[3]);
    drv_ifd_set_cr_nco(IFD_ENABLE, ncoIFMap[1]);
#else
    drv_ifd_set_cr_bpf_coef(IFD_ENABLE, crBPFMap[g_curIFMapIdx]);
    drv_ifd_set_cr_nco(IFD_ENABLE, ncoIFMap[g_curIFMapIdx]);
#endif  //#ifdef CONFIG_TUNER_SILICON

#ifdef CONFIG_TUNER_SILICON
#if defined(CONFIG_ATLANTIC)||defined(CONFIG_LAS_VEGAS)
#else
    drv_ifd_set_eq_coef(eq_coef_non_secamla[1]);
#endif
#else //#ifdef CONFIG_TUNER_SILICON
#if defined(CONFIG_ATLANTIC)||defined(CONFIG_LAS_VEGAS)
#else
    drv_ifd_set_eq_coef(eq_coef_non_secamla[g_curIFMapIdx]);
#endif
#endif //#ifdef CONFIG_TUNER_SILICON

    drv_ifd_set_shaping_filter(IFD_SHAPING_BW_SEL_0P86M);
#ifdef CONFIG_TUNER_SILICON
    drv_ifd_set_shaping_img(IFD_SHAPING_IMG_POS);
#else
    drv_ifd_set_shaping_img(IFD_SHAPING_IMG_NEG);
#endif
    drv_ifd_set_pos_neg_mod(IFD_MODULATION_SEL_NEG);
    //if ((mode == _SIG_MODE_PAL_BG)||(mode == _SIG_MODE_PAL)) {
    //	drv_ifd_set_video_audio_nf_bw(GET_IFD_VIDEOBPF_PALBG(), GET_IFD_AUDIOBPF_NF2PAL(), GET_IFD_AUDIOBPF_NF1PAL(), GET_IFD_VIDEOUPSAMPLE_LPF_PAL());
    //} else {
    drv_ifd_set_video_audio_nf_bw(GET_IFD_VIDEOBPF_PALDKI(), GET_IFD_AUDIOBPF_NF2PAL(), GET_IFD_AUDIOBPF_NF1PAL(), GET_IFD_VIDEOUPSAMPLE_LPF_PAL());
    //}
    drv_ifd_agc_loop1_pos_en(IFD_DISABLE);
    drv_ifd_agc_loop2_pos_en(IFD_DISABLE); // irene have to check why loss this
    drv_ifd_agc_hsync_start1(IFD_VSmode1_Hstart_PAL, IFD_VSmode1_Hend_PAL);
    drv_ifd_agc_vsync_start1(IFD_VSmode1_Vstart_PAL, IFD_VSmode1_Vend_PAL);
    drv_ifd_agc_hsync_start2(IFD_VSmode2_Hstart_PAL, IFD_VSmode2_Hend_PAL);
    drv_ifd_agc_vsync_start2(IFD_VSmode2_Vstart_PAL, IFD_VSmode2_Vend_PAL);
    drv_ifd_agc_targ_val(GET_IFD_AGCLOOP1_MAXTARGET_PAL());
    drv_ifd_agc_loop2_target(GET_IFD_AGCLOOP2_MAXTARGET_PAL());
    rtd_outl(IFD_AGC_ENABLE_SEL_VADDR, 0x000000FE);
    drv_ifd_agc_loop1_fine_tune_en(IFD_DISABLE);
    drv_ifd_agc_loop2_fine_tune_en(IFD_DISABLE);
    drv_ifd_group_delay_coe_table(GET_IFD_GRPDEALY_PAL());
}
//=================================================================
/**
 * drv_ifd_set_video_secaml
 *
 *
 * @param {void}
 * @return {void}
 */
void drv_ifd_set_video_secaml(void)
{
    IFD_DRV_DEBUG_MSG(IFD_LOG_LEVEL, "*****drv_ifd_set_video_secaml()\n");
#ifdef CONFIG_TUNER_SILICON
    drv_ifd_set_cr_bpf_coef(IFD_ENABLE, crBPFMap[3]);
    drv_ifd_set_cr_nco(IFD_ENABLE, ncoIFMap[1]);
#else
    drv_ifd_set_cr_bpf_coef(IFD_ENABLE, crBPFMap[g_curIFMapIdx]);
    drv_ifd_set_cr_nco(IFD_ENABLE, ncoIFMap[g_curIFMapIdx]);
#endif //#ifdef CONFIG_TUNER_SILICON	

#ifdef CONFIG_TUNER_SILICON
#if defined(CONFIG_ATLANTIC)||defined(CONFIG_LAS_VEGAS)
#else
    drv_ifd_set_eq_coef(eq_coef_non_secamla[1]);
#endif
#else //#ifdef CONFIG_TUNER_SILICON
#if defined(CONFIG_ATLANTIC)||defined(CONFIG_LAS_VEGAS)
#else
    drv_ifd_set_eq_coef(eq_coef_non_secamla[g_curIFMapIdx]);
#endif
#endif //#ifdef CONFIG_TUNER_SILICON

    drv_ifd_set_shaping_filter(IFD_SHAPING_BW_SEL_0P86M);
#ifdef CONFIG_TUNER_SILICON
    drv_ifd_set_shaping_img(IFD_SHAPING_IMG_POS);
#else
    drv_ifd_set_shaping_img(IFD_SHAPING_IMG_NEG);
#endif
    drv_ifd_set_pos_neg_mod(IFD_MODULATION_SEL_POS);
    drv_ifd_set_video_audio_nf_bw(GET_IFD_VIDEOBPF_SECAM(), GET_IFD_AUDIOBPF_NF2SECAM(), GET_IFD_AUDIOBPF_NF1SECAM(), GET_IFD_VIDEOUPSAMPLE_LPF_SECAM());
    drv_ifd_agc_loop1_pos_en(IFD_ENABLE);
    drv_ifd_agc_loop2_pos_en(IFD_ENABLE); // irene have to check
    drv_ifd_agc_hsync_start1(IFD_VSmode1_Hstart_SECAM, IFD_VSmode1_Hend_SECAM);
    drv_ifd_agc_vsync_start1(IFD_VSmode1_Vstart_SECAM, IFD_VSmode1_Vend_SECAM);
    drv_ifd_agc_targ_val(GET_IFD_AGCLOOP1_MAXTARGET_SECAM());
    drv_ifd_agc_loop2_target(GET_IFD_AGCLOOP2_MAXTARGET_SECAM());
    drv_ifd_agc_hsync_start2(IFD_VSmode2_Hstart_SECAM, IFD_VSmode2_Hend_SECAM);
    drv_ifd_agc_vsync_start2(IFD_VSmode2_Vstart_SECAM, IFD_VSmode2_Vend_SECAM);
    drv_ifd_agc_loop1_fine_tune_en(IFD_ENABLE);
    drv_ifd_agc_loop2_fine_tune_en(IFD_ENABLE);

    drv_ifd_group_delay_coe_table(GET_IFD_GRPDEALY_SECAML());

}
//=================================================================
/**
 * drv_ifd_set_video_secamla
 *
 *
 * @param {void}
 * @return {void}
 */
void drv_ifd_set_video_secamla()
{
    IFD_DRV_DEBUG_MSG(IFD_LOG_LEVEL, "*****drv_ifd_set_video_secamla()\n");
#ifdef CONFIG_TUNER_SILICON
    drv_ifd_set_cr_bpf_coef(IFD_ENABLE, crBPFMap[3]);
    drv_ifd_set_cr_nco(IFD_ENABLE, ncoIFMap[1]);
#else
    drv_ifd_set_cr_nco(IFD_ENABLE, ncoIF_SECAMLA_Map[g_curIFMapIdx]);
    drv_ifd_set_cr_bpf_coef(IFD_ENABLE,crBPF_SECAMLA_Map[g_curIFMapIdx]);
#endif  //#ifdef CONFIG_TUNER_SILICON

#ifdef CONFIG_TUNER_SILICON
#if defined(CONFIG_ATLANTIC)||defined(CONFIG_LAS_VEGAS)
#else
    drv_ifd_set_eq_coef(eq_coef_secamla[g_curIFMapIdx]);
#endif
#else //#ifdef CONFIG_TUNER_SILICON
#if defined(CONFIG_ATLANTIC)||defined(CONFIG_LAS_VEGAS)
#else
    drv_ifd_set_eq_coef(eq_coef_secamla[g_curIFMapIdx]);
#endif
#endif //#ifdef CONFIG_TUNER_SILICON

    drv_ifd_set_shaping_filter(IFD_SHAPING_BW_SEL_0P86M);
#ifdef CONFIG_TUNER_SILICON
    drv_ifd_set_shaping_img(IFD_SHAPING_IMG_NEG);
#else
    drv_ifd_set_shaping_img(IFD_SHAPING_IMG_POS);
#endif
    drv_ifd_set_pos_neg_mod(IFD_MODULATION_SEL_POS);
    drv_ifd_set_video_audio_nf_bw(IFD_VIDEO_BW_SEL_5P32M, SOUND_NOTCH_SEL2_BYPASS, SOUND_NOTCH_SEL_BYPASS, IFD_UPSAMPLE_LP_SEL_PAL);
    drv_ifd_agc_diff_targ(GET_IFD_AGCLOOP1_DIFFTARGET());
    drv_ifd_agc_loop1_pos_en(IFD_ENABLE);
    drv_ifd_agc_loop2_pos_en(IFD_ENABLE);
    drv_ifd_agc_hsync_start1(IFD_VSmode1_Hstart_SECAM, IFD_VSmode1_Hend_SECAM);
    drv_ifd_agc_vsync_start1(IFD_VSmode1_Vstart_SECAM, IFD_VSmode1_Vend_SECAM);
    drv_ifd_agc_targ_val(GET_IFD_AGCLOOP1_MAXTARGET_SECAM());
    drv_ifd_agc_loop2_target(GET_IFD_AGCLOOP2_MAXTARGET_SECAM());
    drv_ifd_agc_hsync_start2(IFD_VSmode2_Hstart_SECAM, IFD_VSmode2_Hend_SECAM);
    drv_ifd_agc_vsync_start2(IFD_VSmode2_Vstart_SECAM, IFD_VSmode2_Vend_SECAM);
    drv_ifd_agc_loop1_fine_tune_en(IFD_ENABLE);
    drv_ifd_agc_loop2_fine_tune_en(IFD_ENABLE);

    drv_ifd_group_delay_coe_table(GET_IFD_GRPDEALY_SECAMLA());


}
//=================================================================
/**
 * drv_ifd_group_delay_coe_table
 *
 *
 * @param <table>
 * @return {void}
 */
void drv_ifd_group_delay_coe_table(UINT8 table)
{
    IFD_DRV_DEBUG_MSG(IFD_LOG_LEVEL, "*****drv_ifd_group_delay_coe_table(%d)\n", (UINT32)table);

    switch (table)
    {
    case 0:
        drv_ifd_grp_delay_en (IFD_DISABLE,  group_delay_table1);//disable grp filter
        break;
    case 1:
        drv_ifd_grp_delay_en (IFD_ENABLE,  group_delay_table1);
        break;
    case 2:
        drv_ifd_grp_delay_en (IFD_ENABLE,  group_delay_table2);
        break;
    case 3:
        drv_ifd_grp_delay_en (IFD_ENABLE,  group_delay_table3);
        break;
    case 4:
        drv_ifd_grp_delay_en (IFD_ENABLE,  group_delay_table4);
        break;
    case 5:
        drv_ifd_grp_delay_en (IFD_ENABLE,  group_delay_table5);
        break;
    default:
        drv_ifd_grp_delay_en (IFD_DISABLE,  group_delay_table1);//disable grp filter
        break;
    }
}

//=================================================================
/**
 * drv_ifd_set_adc_init
 *
 *
 * @param {void}
 * @return {void}
 */
void drv_ifd_set_adc_init(void)
{

#ifdef CONFIG_TUNER_SILICON

    rtd_outl(ADC_IFBW_0x5fcc,0x00000002);//ADC2x LPF 50M, //cyhsiao suggests this setting, for better SNR, 20091117
    rtd_outl(ADC_IFPGAPOW_0x5ff0,0x00000001);	//IFPGA power on
    rtdf_setBits(ADC_MBIASPOW_0x5fec, _BIT0);
    rtd_outl(ADC_ANALOG_RESERVE_0x5ff4,0x04001aa0);	//change default


    //drv_ifd_initdata_by_tuner_if();
    g_curIFMapIdx = 1;
    rtd_outl(INPUT_BYPASS_ENABLE_0x5f68, 0x00000000);	// 108 clock
    rtd_outl(AGC_VSYNC_START2_0x5f74, 0x02000400);  // vsync mode sync gate
//		IFD_Cr_BPF_Coe_Table(CR_BPF_SEL_0x5c00,crBPFMap[g_curIFMapIdx]);
    rtd_outl(CR_NCO_FIX_DATA_0x5c10, ncoIFMap[g_curIFMapIdx]);
    rtd_outl(CR_FREQ_SAT_0x5c14, 0x0000A000);	// cr_offset_sat
    rtdf_outl(CR_WEIGHT_EN_0x5c1c,0x00000181);	//cr weighting enable
    rtdf_outl(CR_PHASE_BAD_0x5c20,0x00000000);	//cr weighting enable
    rtd_outl(DUMMY1_0x5c2c, 0x00000001);	// agc_slow_charge_en
    drv_ifd_set_shaping_filter(IFD_SHAPING_BW_SEL_0P86M);
    drv_ifd_set_shaping_img(IFD_SHAPING_IMG_NEG);
    rtd_outl(CR_ERR_THD_0x5c50, 0x00081010);  // cr_lock thd
    rtd_outl(CR_AUTO_RESET_EN_0x5c54, 0x00000001);  // cr_auto_reset_en
    rtd_outl(CR_WBLF_CNTL_0x5c60, 0x00000050);	// wblf disable
//        rtd_outl(CR_WBLF_CNTL_0x5c60, 0x00000051);	// wblf_en
    rtd_outl(CR_WBLF_TH_0x5c64, 0x0d000d00);	// wblf_th
    rtd_outl(CR_VARIANCE_DEBOUNCE_0x5c6c, 0x0000ffff);	// wblf_check_interval
    rtd_outl(CLAMPING_SCALE_0x5c8c, 0x00000004);
    rtd_outl(CLAMPING_THR_MAX_0x5c90, 0x00001fff);
    rtd_outl(CLAMPING_THR_MIN_0x5c94, 0x00003500);


    rtd_outl(AGC_ENABLE_SEL_0x5d00, 0x0000009e);  // filter not yet ready disable first
    rtd_outl(AGC_MAX_COUNT_FOR_COARSE_MODE_AGC_CNTL1_0x5d08, 0x00000020);
    rtd_outl(AGC_MAX_COUNT_FOR_FINELOCK_MODE_AGC_CNTL2_0x5d0c, 0x00000020);
    rtd_outl(AGC_AVG_TARGET_0x5d10, 0x00000110); //agc diff target
    rtd_outl(AGC_MAX_COUNT_FOR_COARSE_MODE_AGC_LOCK2_0x5d14, 0x000000f0);  //loop1 dn sample 16
    rtd_outl(AGC_TARG_VAL_0x5d18, 0x00000400);
    rtd_outl(AGC_CNTL_SEL_0x5d1c  , 0x00000000);  // if inverse
        rtd_outl(KI_VAL_0x5d2c, 0x0B000045);
    rtd_outl(RF_AGC_LIMIT_VALUE_0x5d34, 0x00001FFF); //rf max
    rtd_outl(AGC_LOCK_TH_0x5d3c, 0x00001810);
    rtd_outl(AGC_HSYNC_START_0x5d44, 0x0000b5c5); //agc h gate
    rtd_outl(AGC_VSYNC_START1_0x5d48, 0x80800990); //agc V gate
    rtd_outl(AGC_PEAK_DETECT_0x5d50, 0x00000db0); //peak en
    rtd_outl(AGC_PEAK_VALUE_0x5d54, 0x000007b0); //peak target
    rtd_outl(AGC_PEAK2_VALUE_0x5dc4, 0x000007b0); //peak target
    rtd_outl(VTOP_VALUE_0x5d80, 0x00001800); //vtop
    rtd_outl(KRF_VALUE_0x5d84, 0x00000400); //krf
    rtd_outl(AGC_PGA_CNTL_0x5d94, 0x00000000); // vsync mode disable
    rtd_outl(AGC_LINEBASE_CTL_0x5d98, 0x00000070); //linebase off
    rtd_outl(AGC_MAX2_CNT_1_0x5d9c, 0x00010010);  //max_cntl
    rtd_outl(AGC_IN_KP_KI_0x5da0, 0x00370100);  //ki shift
    rtd_outl(AGC_TARG_VAL_LOOP2_0x5da8, 0x00000400);    // loop2 gain target
    rtd_outl(AGC_MAX2_LIMIT_CNT_1_0x5dac, 0x0000FFF0);
    rtd_outl(AGC_PEAK2_DETECT_0x5dc0, 0x00000db0);
    rtd_outl(AGC_LOCK2_TH_0x5dcc, 0x00001810);  //loop2 lock
    rtd_outl(AGC_LOOP2_CNTL_0x5dd0, 0x00000001); // dn16 irene@090327
    rtd_outl(AGC_DGAIN_THR_0x5de4, 0x7f00fff0);
    rtd_outl(AGC_DGAIN_TO_PGA_0x5de8, 0x20401388);
    rtd_outl(IF_AGC_MIN_0x5d30, 0x17000800);

    rtdf_setBits(CR_BPF_SEL_0x5c00, _BIT26);
    rtdf_setBits(AIN_SEL_0x5fc0, _BIT1);
    rtdf_clearBits(AIN_SEL_0x5fc0, _BIT0);
    rtd_outl(INPUT_BYPASS_ENABLE_0x5f68,0x00000001);
    rtdf_clearBits(ADC_ANALOG_RESERVE_0x5ff4,_BIT26);
#else
    IFD_DRV_DEBUG_MSG(IFD_LOG_LEVEL, "*****drv_ifd_set_adc_init()\n");

    drv_ifd_power_on(IFD_ENABLE);
    drv_ifd_analog_preset();
    drv_ifd_set_vd_path();
    drv_ifd_initdata_by_tuner_if();

    drv_ifd_agc_mode_set(GET_IFD_AGCPGA_MODE());

    drv_ifd_set_cr_bpf_coef(IFD_ENABLE, crBPFMap[g_curIFMapIdx]);
    //irenefan@1222 change below code
//	drv_ifd_cr_loop_filter_kp(GET_IFD_CR_PGAIN());
//	drv_ifd_cr_loop_filter_ki(GET_IFD_CR_IGAIN());
    drv_ifd_cr_loop_settings(IFD_CR_LOOP_SLOW);
    drv_ifd_set_cr_nco(IFD_ENABLE, ncoIFMap[g_curIFMapIdx]);
    rtd_outl(IFD_CR_FREQ_SAT_VADDR, IFD_CR_OFFSET_SAT);	// cr_offset_sat
    drv_ifd_set_cr_weighting_settings(IFD_CR_WEIGHT_CR_LOCK_ENABLE, IFD_CR_WEIGHT_MAX_DECREASE_512CYCLE, IFD_CR_WEIGHT_MAX_SHIFT_RIGHT_1BIT);
    drv_ifd_set_cr_weighting_en(IFD_ENABLE);
    rtd_outl(IFD_CR_PHASE_BAD_VADDR,(IFD_CR_PHASE_BAD_THD&0x07F)); //cr weighting disable
    drv_ifd_set_cr_phase_bad_en(GET_IFD_CR_PHASEBAD_EN());
    drv_ifd_set_slow_charge_en_for_max(GET_IFD_AGCMAX_SLOWCHARGE_EN());  //irene ok // irene has to check
    drv_ifd_set_slow_charge_en_for_peak(GET_IFD_AGCPEAK_SLOWCHARGE_EN());  //irene ok // irene has to check
    rtd_outl(IFD_SHAPE_FILTER_SELECTION_VADDR, 0x010); //data_gain => 0
    drv_ifd_set_shaping_filter(IFD_SHAPING_BW_SEL_0P86M);
    drv_ifd_set_shaping_img(IFD_SHAPING_IMG_NEG);
    drv_ifd_cr_error_thd(GET_IFD_CR_LOCK_ERR_THD());//rtd_outl(IFD_CR_ERR_THD_VADDR, ((UINT32)GET_IFD_CR_LOCK_ERR_THD()<<16) |0x1010);  // cr_lock thd
    drv_ifd_cr_auto_reset_en(IFD_ENABLE);
    drvif_ifd_cr_freq_status_check(IFD_CR_STATE_SET_TUNER);
    drv_ifd_set_54to27_decimation_enable(IFD_ENABLE);
    rtd_outl(IFD_CR_WBLF_CNTL_VADDR, 0x00000050); // wblf disable
    rtd_outl(IFD_CR_WBLF_TH_VADDR, 0x0d000d00); // wblf_th
    rtd_outl(IFD_CR_VARIANCE_DEBOUNCE_VADDR, 0x0000ffff); // wblf_check_interval
    drv_ifd_set_clamping_scale(IFD_CLAMP_SCALE_1D4);
    drv_ifd_set_clamping_thr_max(IFD_CLAMP_MAX_THD);//rtd_outl(IFD_CLAMPING_THR_MAX_VADDR, IFD_CLAMP_MAX_THD); // irene has to check with factory mode?
    drv_ifd_set_clamping_thr_min(IFD_CLAMP_MIN_THD);//rtd_outl(IFD_CLAMPING_THR_MIN_VADDR, IFD_CLAMP_MIN_THD); // irene has to check with factory mode?

    rtd_outl(IFD_AGC_ENABLE_SEL_VADDR, 0x0000009E);  // filter not yet ready disable first
    rtd_outl(IFD_AGC_MAX_COUNT_FOR_COARSE_MODE_AGC_CNTL1_VADDR, IFD_UNLOCK_MAX_DEC_CNT);
    rtd_outl(IFD_AGC_MAX_COUNT_FOR_FINELOCK_MODE_AGC_CNTL2_VADDR, IFD_UNLOCK_MAX_DEC_CNT);
    drv_ifd_agc_diff_targ(GET_IFD_AGCLOOP1_DIFFTARGET());
    rtd_outl(IFD_AGC_MAX_COUNT_FOR_COARSE_MODE_AGC_LOCK2_VADDR, GET_IFD_AGCLOOP1_DNSAMPLE()<<4);
    drv_ifd_agc_targ_val(GET_IFD_AGCLOOP1_MAXTARGET_PAL());
    drv_ifd_agc_loop2_target(GET_IFD_AGCLOOP2_MAXTARGET_PAL());

    rtd_outl(IFD_AGC_CNTL_SEL_VADDR, 0x0);
    drv_ifd_agc_out_inv(IFD_RF_AGC_PORT_INV, IFD_IF_AGC_PORT_INV);
    rtd_outl(IFD_KI_VAL_VADDR, 0x0);
    drv_ifd_agc_loop1_kp_settings(GET_IFD_AGCLOOP1_KPGAIN());
    drv_ifd_agc_loop1_ki_settings(GET_IFD_AGCLOOP1_KIGAIN());
    drv_ifd_agc_loop1_pishift_settings(GET_IFD_AGCLOOP1_KPKISHIFT());
    rtd_outl(IFD_RF_AGC_LIMIT_VALUE_VADDR, ((IFD_RF_AGC_MIN<<16) |GET_IFD_AGC_RF_MAX()));
    rtd_outl(IFD_IF_AGC_MIN_VADDR, ((IFD_IF_AGC_MAX<<16) |IFD_IF_AGC_MIN));
    rtd_outl(IFD_AGC_LOCK_TH_VADDR, ((IFD_LOOP1_AGC_LOCK_OUT_THD<<8) | IFD_LOOP1_AGC_LOCK_IN_THD));
    drv_ifd_agc_hsync_start1(IFD_VSmode1_Hstart_PAL, IFD_VSmode1_Hend_PAL);
    drv_ifd_agc_vsync_start1(IFD_VSmode1_Vstart_PAL, IFD_VSmode1_Vend_PAL);
    drv_ifd_agc_hsync_start2(IFD_VSmode2_Hstart_PAL, IFD_VSmode2_Hend_PAL);
    drv_ifd_agc_vsync_start2(IFD_VSmode2_Vstart_PAL, IFD_VSmode2_Vend_PAL);

    rtd_outl(IFD_AGC_PEAK_DETECT_VADDR, (IFD_PEAK_DEC_CNT&0xfff));
    drv_ifd_agc_loop2_peak_cnt(IFD_PEAK2_DEC_CNT);
    drv_ifd_agc_loop1_peak_en(IFD_ENABLE);
    drv_ifd_agc_loop2_peak_en(IFD_ENABLE);
    drv_ifd_agc_peak_target(GET_IFD_AGCLOOP1_PEAKTARGET());
    drv_ifd_agc_krf(IFD_AGC_KRF);
    rtd_outl(IFD_AGC_PGA_CNTL_VADDR, 0x0);
    drv_ifd_agc_vsync_mode_en(IFD_VSMODE_EN);
    rtd_outl(IFD_AGC_LINEBASE_CTL_VADDR, 0x00000070);
    rtd_outl(IFD_AGC_MAX2_CNT_1_VADDR, ((IFD_UNLOCK_MAX2_DEC_CNT<<12)|IFD_LOCK_MAX2_DEC_CNT));
    drv_ifd_loop2_agcin_kpki_settings(GET_IFD_AGCLOOP2_KPGAIN(), GET_IFD_AGCLOOP2_KIGAIN(), GET_IFD_AGCLOOP2_KPSHIFT(), GET_IFD_AGCLOOP2_KISHIFT());
    rtd_outl(IFD_AGC_MAX2_LIMIT_CNT_1_VADDR, 0x0000FFF0);


    drv_ifd_agc_peak2_target(GET_IFD_AGCLOOP2_PEAKTARGET());
    rtd_outl(IFD_AGC_LOCK2_TH_VADDR, ((IFD_LOOP2_AGC_LOCK_OUT_THD<<8) | IFD_LOOP2_AGC_LOCK_IN_THD));
    drv_ifd_agc_loop2_dn_sample(GET_IFD_AGCLOOP2_DNSAMPLE());
    drv_ifd_agc_dgain_up_dn_boundary(IFD_AGC_2STEP_DGAIN_LOW_THD, IFD_AGC_2STEP_DGAIN_HIGH_THD);
    rtd_outl(IFD_AGC_DGAIN_TO_PGA_VADDR, 0x20401388);
    drv_ifd_agc_2step_pga_step(IFD_AGC_2STEP_PGA_STEP);
    drv_ifd_agc_vtop(GET_IFD_AGC_VTOP());
    drv_ifd_agc_2step_pga_minimum(GET_IFD_AGC_PGA_MINIMUM());
    drv_ifd_agc_loop1_finetune_dn_sample(GET_IFD_AGCLOOP1_FINEDNSAMPLE());
    drv_ifd_agc_loop2_finetune_dn_sample(GET_IFD_AGCLOOP2_FINEDNSAMPLE());
    drv_ifd_agc_loop2_diff_target(GET_IFD_AGCLOOP2_DIFFTARGET());
    rtd_outl(IFD_AGC_ENABLE_SEL_VADDR, 0x000000FE);
#endif
}
//=================================================================
/**
 * drv_ifd_agc_mode_set
 *
 *
 * @param <ifdAGCmode>
 * @return { void }
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_agc_mode_set(IFD_MODE_AGC_PGA_CONFIG ifd_agc_mode)
{
    IFD_DRV_DEBUG_MSG(IFD_LOG_LEVEL, "*****drv_ifd_agc_mode_set(%x)\n", (UINT32)ifd_agc_mode);

    switch (ifd_agc_mode)
    {
    case IFD_AGC_SINGLE_NOIFAMP_PGA :
    case IFD_AGC_DOUBLE_NOIFAMP_PGA :
    case IFD_AGC_2STEP_NOIFAMP_PGA :
        drv_ifd_adc_input_sel(IFD_ADC_INPUT_IFPGA);
        break;
    case IFD_AGC_SINGLE_IFAMP_NOPGA :
    case IFD_AGC_DOUBLE_IFAMP_NOPGA :
    case IFD_AGC_2STEP_IFAMP_NOPGA :
        drv_ifd_adc_input_sel(IFD_ADC_INPUT_BYPASS_PGA);
        break;
    default:
        drv_ifd_adc_input_sel(IFD_ADC_INPUT_IFPGA);
        break;
    }
    switch (ifd_agc_mode)
    {
    case IFD_AGC_SINGLE_NOIFAMP_PGA:
    case IFD_AGC_SINGLE_IFAMP_NOPGA:
        drv_ifd_agc_mode_sel(IFD_AGC_MODE_ORIGINAL);
        break;
    case IFD_AGC_DOUBLE_NOIFAMP_PGA:
    case IFD_AGC_DOUBLE_IFAMP_NOPGA:
        drv_ifd_agc_mode_sel(IFD_AGC_MODE_2LOOP);
        break;
    case IFD_AGC_2STEP_IFAMP_NOPGA:
    case IFD_AGC_2STEP_NOIFAMP_PGA:
        drv_ifd_agc_mode_sel(IFD_AGC_MODE_2STEP);
        break;
    default:
        drv_ifd_agc_mode_sel(IFD_AGC_MODE_2STEP);
        break;
    }

    return;
}
#endif
//=================================================================
/**
 * drv_ifd_initdata_by_tuner_if
 *
 *
 * @param {void}
 * @return {void}
 */
#ifndef CONFIG_TUNER_SILICON
void drv_ifd_initdata_by_tuner_if(void)
{
    UINT32	i;
    UINT32 curTunerIFVal = drv_tuner_GetPictureInterFreq();//999CTunerGetPictureInterFreq();

    IFD_DRV_DEBUG_MSG(IFD_LOG_LEVEL, "*****drv_ifd_initdata_by_tuner_if()\n");

    for (i=0; i<sizeof(tunerIFMap)/sizeof(UINT32); i++)
    {
        if (curTunerIFVal == tunerIFMap[i])
        {
            g_curIFMapIdx = i;
            break;
        }
    }
}
#endif
//=================================================================
/**
 * drv_ifd_cr_loop_settings
 *
 *
 * @param {state}
 * @return {void}
 */
void drv_ifd_cr_loop_settings(IFD_CR_LOOP_STATE state)
{
    switch (state)
    {
    case IFD_CR_LOOP_FAST:
        drv_ifd_cr_loop_filter_kp(1);
        drv_ifd_cr_loop_filter_ki(6);
#ifndef CONFIG_TUNER_SILICON
        drv_ifd_set_shaping_filter(IFD_SHAPING_BW_SEL_BYPASS);
#endif
        break;
    case IFD_CR_LOOP_SLOW:
    default:
        drv_ifd_cr_loop_filter_kp(2);
        drv_ifd_cr_loop_filter_ki(7);
#ifndef CONFIG_TUNER_SILICON
        drv_ifd_set_shaping_filter(IFD_SHAPING_BW_SEL_0P86M);
#endif
        break;
    }
}
#endif
#if defined(CONFIG_PLATFORM_PACIFIC)||defined(CONFIG_ATLANTIC)||defined(CONFIG_LAS_VEGAS)
//=================================================================
/**
 * drv_ifd_log_reg
 *
 *
 * @param { void }
 * @return { void }
 */
void drv_ifd_log_reg(void)
{
    UINT32 i;
    UINT32 length1 = 0x01fc;
    UINT32 length2 = 0x00fc;

    IFD_DRV_DEBUG_MSG(IFD_LOG_LEVEL, "*****drv_ifd_log_reg()\n");

    IFD_DRV_DEBUG_MSG(IFD_LOG_LEVEL, "======Start to log register======\n");
    for (i=IFD_CR_BPF_SEL_VADDR; i<(IFD_CR_BPF_SEL_VADDR+length1); i+=0x10)
    {
        IFD_DRV_DEBUG_MSG(IFD_LOG_LEVEL, "Address %x = %x,   %x,   %x,   %x\n", (UINT32)i, (UINT32)rtd_inl(i), (UINT32)rtd_inl(i+4), (UINT32)rtd_inl(i+8), (UINT32)rtd_inl(i+12));
    }
    for (i=IFD_DEBUG_MODE_VADDR; i<(IFD_DEBUG_MODE_VADDR+length2); i+=0x10)
    {
        IFD_DRV_DEBUG_MSG(IFD_LOG_LEVEL,"Address %x = %x,   %x,   %x,   %x\n", (UINT32)i, (UINT32)rtd_inl(i), (UINT32)rtd_inl(i+4), (UINT32)rtd_inl(i+8), (UINT32)rtd_inl(i+12));
    }
    IFD_DRV_DEBUG_MSG(IFD_LOG_LEVEL, "======End of register logging======\n");
}
//=================================================================
/**
 * drv_ifd_periodic_log_reg
 *
 *
 * @param { void }
 * @return { void }
 */
void drv_ifd_periodic_log_reg(void)
{
    UINT32 i;
    UINT32 length1 = 0x01fc;
    UINT32 length2 = 0x00fc;

    IFD_DRV_DEBUG_MSG(IFD_LOG_LEVEL, "*****drv_ifd_periodic_log_reg()\n");

    IFD_DRV_DEBUG_MSG(IFD_LOG_LEVEL, "======Start to log register======\n");
    for (i=IFD_CR_BPF_SEL_VADDR; i<(IFD_CR_BPF_SEL_VADDR+length1); i+=0x10)
    {
        IFD_DRV_DEBUG_MSG(IFD_LOG_LEVEL, "Address %x = %x,   %x,   %x,   %x\n", (UINT32)i, (UINT32)rtd_inl(i), (UINT32)rtd_inl(i+4), (UINT32)rtd_inl(i+8), (UINT32)rtd_inl(i+12));
    }
    for (i=IFD_DEBUG_MODE_VADDR; i<(IFD_DEBUG_MODE_VADDR+length2); i+=0x10)
    {
        IFD_DRV_DEBUG_MSG(IFD_LOG_LEVEL,"Address %x = %x,   %x,   %x,   %x\n", (UINT32)i, (UINT32)rtd_inl(i), (UINT32)rtd_inl(i+4), (UINT32)rtd_inl(i+8), (UINT32)rtd_inl(i+12));
    }
    IFD_DRV_DEBUG_MSG(IFD_LOG_LEVEL, "======End of register logging======\n");
    ScalerTimer_ReactiveTimerEvent(IFD_REG_LOG_TIMER, drv_ifd_periodic_log_reg);
}

#endif
#ifdef CONFIG_SPARROW
#ifdef CONFIG_IFD_TUNER_AUTO
//=================================================================
/**
 * drvif_ifd_isr_set_standard
 *
 *
 * @param <video_mode><freq>
 * @return { void }
 */
void drvif_ifd_isr_set_standard(UINT8 video_mode, UINT32 freq)
{
    UINT32 tuner_freq;
    UINT32 cr_bpf_sel_rbus_data;
    UINT32 i;
    UINT32 grp_delay_en_rbus_data;
    UINT32 tap_num;

    if (video_mode ==  ZSECAMLA)
    {
        tuner_freq= freq -SECAMLA_FREQ_SHIFT;
    }
    else
    {
        tuner_freq = freq ;
    }

    drv_tuner_isr_SetFreq(tuner_freq);

    if (video_mode==0)
    {
        for (i=0; i<CR_BPF_TABLE_SIZE; i++)
        {
            cr_bpf_sel_rbus_data = 0;
            rtd_isr_outl(IFD_CR_BPF_SEL_VADDR, cr_bpf_sel_rbus_data);
            cr_bpf_sel_rbus_data = ((UINT32)1<<24) | (((UINT32)i&0x3F)<<16) | ((UINT32)crBPFMap[g_curIFMapIdx][i]&0x7FFF);
            rtd_isr_outl(IFD_CR_BPF_SEL_VADDR, cr_bpf_sel_rbus_data);
        }
        cr_bpf_sel_rbus_data = 0;
        rtd_isr_outl(IFD_CR_BPF_SEL_VADDR, cr_bpf_sel_rbus_data);
        rtd_isr_maskl(IFD_CR_BPF_SEL_VADDR, ~(_BIT31|_BIT30|_BIT29|_BIT28|_BIT25), (((UINT32)6)<<28)|(((UINT32)1)<<25));
        rtd_isr_maskl(IFD_CR_BPF_SEL_VADDR, ~(_BIT26), (((UINT32)1)<<26));

        rtd_isr_outl(IFD_CR_NCO_FIX_DATA_VADDR, (((UINT32)1)<<20)|(ncoIFMap[g_curIFMapIdx]&(UINT32)0xfffff));
        rtd_isr_maskl(IFD_VIDEO_BW_SEL_VADDR, ~_BIT16, (UINT32)_BIT16);
        rtd_isr_maskl(IFD_VIDEO_BW_SEL_VADDR, ~((UINT32)0xFFFF), (((UINT32)GET_IFD_VIDEOBPF_NTSC()<<12)|((UINT32)GET_IFD_AUDIOBPF_NF2NTSC()<<8)|((UINT32)GET_IFD_AUDIOBPF_NF1NTSC()<<4)|(UINT32)GET_IFD_VIDEOUPSAMPLE_LPF_NTSC()));

        rtd_isr_maskl(IFD_SHAPE_FILTER_SELECTION_VADDR, ~((UINT32)(0xF0)), ((UINT32)IFD_SHAPING_BW_SEL_0P86M<<4));
        rtd_isr_maskl(IFD_SHAPE_FILTER_SELECTION_VADDR, ~_BIT0, ((UINT32)IFD_SHAPING_IMG_NEG&0x1));
        rtd_isr_maskl(IFD_AGC_MAX_COUNT_FOR_COARSE_MODE_AGC_LOCK2_VADDR, ~_BIT0, 0);
        rtd_isr_maskl(IFD_AGC_2STEP_EN_VADDR, ~_BIT12, (UINT32)0<<12);
        rtd_isr_outl(IFD_AGC_VSYNC_START2_VADDR, ((UINT32)IFD_VSmode2_Vstart_NTSC<<16)|((UINT32)IFD_VSmode2_Vend_NTSC));

        rtd_isr_maskl(IFD_AGC_TARG_VAL_VADDR, ~((UINT32)(0x7FF)), (UINT32)GET_IFD_AGCLOOP1_MAXTARGET_NTSC()&0x7FF);
        rtd_isr_maskl(IFD_AGC_TARG_VAL_LOOP2_VADDR, ~((UINT32)0x7FF), (UINT32)GET_IFD_AGCLOOP2_MAXTARGET_NTSC()&0x7FF);

        rtd_isr_outl(IFD_AGC_HSYNC_START_VADDR, ((UINT32)IFD_VSmode1_Hstart_NTSC<<8)|((UINT32)IFD_VSmode1_Hend_NTSC));
        rtd_isr_outl(IFD_AGC_VSYNC_START1_VADDR, ((UINT32)IFD_VSmode1_Vstart_NTSC<<16)|((UINT32)IFD_VSmode1_Vend_NTSC));
        rtd_isr_outl(IFD_AGC_HSYNC_START2_VADDR, ((UINT32)IFD_VSmode2_Hstart_NTSC<<8)|((UINT32)IFD_VSmode2_Hend_NTSC));
        rtd_isr_outl(IFD_AGC_VSYNC_START2_VADDR, ((UINT32)IFD_VSmode2_Vstart_NTSC<<16)|((UINT32)IFD_VSmode2_Vend_NTSC));
        rtd_isr_outl(IFD_AGC_ENABLE_SEL_VADDR, 0x000000FE);  //agc enable
        rtd_isr_maskl(IFD_AGC_ENABLE_SEL_VADDR, ~_BIT0, 0);
        rtd_isr_maskl(IFD_AGC_2STEP_EN_VADDR, ~_BIT28, ((UINT32)0)<<28);


        tap_num = (UINT32)group_delay_table5[0];
        for (i=0; i<tap_num; i++)
        {
            grp_delay_en_rbus_data = 0;
            rtd_isr_outl(IFD_GRP_DELAY_EN_VADDR, grp_delay_en_rbus_data);
            grp_delay_en_rbus_data = (_BIT20)|((UINT32)(i&0xFF)<<12)|((UINT32)group_delay_table5[i+1]);
            rtd_isr_outl(IFD_GRP_DELAY_EN_VADDR, grp_delay_en_rbus_data);
        }
        grp_delay_en_rbus_data = 0;
        rtd_isr_outl(IFD_GRP_DELAY_EN_VADDR, grp_delay_en_rbus_data);
        grp_delay_en_rbus_data = _BIT24;
        rtd_isr_outl(IFD_GRP_DELAY_EN_VADDR, grp_delay_en_rbus_data);
    }
    else
    {
        for (i=0; i<CR_BPF_TABLE_SIZE; i++)
        {
            cr_bpf_sel_rbus_data = 0;
            rtd_isr_outl(IFD_CR_BPF_SEL_VADDR, cr_bpf_sel_rbus_data);
            cr_bpf_sel_rbus_data = ((UINT32)1<<24) | (((UINT32)i&0x3F)<<16) | ((UINT32)crBPFMap[g_curIFMapIdx][i]&0x7FFF);
            rtd_isr_outl(IFD_CR_BPF_SEL_VADDR, cr_bpf_sel_rbus_data);
        }
        cr_bpf_sel_rbus_data = 0;
        rtd_isr_outl(IFD_CR_BPF_SEL_VADDR, cr_bpf_sel_rbus_data);
        rtd_isr_maskl(IFD_CR_BPF_SEL_VADDR, ~(_BIT31|_BIT30|_BIT29|_BIT28|_BIT25), (((UINT32)6)<<28)|(((UINT32)1)<<25));
        rtd_isr_maskl(IFD_CR_BPF_SEL_VADDR, ~(_BIT26), (((UINT32)1)<<26));

        rtd_isr_outl(IFD_CR_NCO_FIX_DATA_VADDR, (((UINT32)1)<<20)|(ncoIFMap[g_curIFMapIdx]&(UINT32)0xfffff));

        rtd_isr_maskl(IFD_SHAPE_FILTER_SELECTION_VADDR, ~((UINT32)(0xF0)), ((UINT32)IFD_SHAPING_BW_SEL_0P86M<<4));
        rtd_isr_maskl(IFD_SHAPE_FILTER_SELECTION_VADDR, ~_BIT0, ((UINT32)IFD_SHAPING_IMG_NEG&0x1));
        rtd_isr_maskl(IFD_VIDEO_BW_SEL_VADDR, ~_BIT16, (UINT32)_BIT16);
        rtd_isr_maskl(IFD_VIDEO_BW_SEL_VADDR, ~((UINT32)0xFFFF), (((UINT32)GET_IFD_VIDEOBPF_PALDKI()<<12)|((UINT32)GET_IFD_AUDIOBPF_NF2PAL()<<8)|((UINT32)GET_IFD_AUDIOBPF_NF1PAL()<<4)|(UINT32)GET_IFD_VIDEOUPSAMPLE_LPF_PAL()));
        rtd_isr_maskl(IFD_AGC_MAX_COUNT_FOR_COARSE_MODE_AGC_LOCK2_VADDR, ~_BIT0, 0);
        rtd_isr_maskl(IFD_AGC_2STEP_EN_VADDR, ~_BIT12, (UINT32)0<<12);
        rtd_isr_outl(IFD_AGC_HSYNC_START_VADDR, ((UINT32)IFD_VSmode1_Hstart_NTSC<<8)|((UINT32)IFD_VSmode1_Hend_NTSC));
        rtd_isr_outl(IFD_AGC_VSYNC_START1_VADDR, ((UINT32)IFD_VSmode1_Vstart_NTSC<<16)|((UINT32)IFD_VSmode1_Vend_NTSC));
        rtd_isr_outl(IFD_AGC_HSYNC_START2_VADDR, ((UINT32)IFD_VSmode2_Hstart_NTSC<<8)|((UINT32)IFD_VSmode2_Hend_NTSC));
        rtd_isr_outl(IFD_AGC_VSYNC_START2_VADDR, ((UINT32)IFD_VSmode2_Vstart_NTSC<<16)|((UINT32)IFD_VSmode2_Vend_NTSC));
        rtd_isr_maskl(IFD_AGC_TARG_VAL_VADDR, ~((UINT32)(0x7FF)), (UINT32)GET_IFD_AGCLOOP1_MAXTARGET_PAL()&0x7FF);
        rtd_isr_maskl(IFD_AGC_TARG_VAL_LOOP2_VADDR, ~((UINT32)0x7FF), (UINT32)GET_IFD_AGCLOOP2_MAXTARGET_PAL()&0x7FF);
        rtd_isr_outl(IFD_AGC_ENABLE_SEL_VADDR, 0x000000FE);
        rtd_isr_maskl(IFD_AGC_ENABLE_SEL_VADDR, ~_BIT0, 0);
        rtd_isr_maskl(IFD_AGC_2STEP_EN_VADDR, ~_BIT28, ((UINT32)0)<<28);


        tap_num = (UINT32)group_delay_table1[0];
        for (i=0; i<tap_num; i++)
        {
            grp_delay_en_rbus_data = 0;
            rtd_isr_outl(IFD_GRP_DELAY_EN_VADDR, grp_delay_en_rbus_data);
            grp_delay_en_rbus_data = (_BIT20)|((UINT32)(i&0xFF)<<12)|((UINT32)group_delay_table1[i+1]);
            rtd_isr_outl(IFD_GRP_DELAY_EN_VADDR, grp_delay_en_rbus_data);
        }
        grp_delay_en_rbus_data = 0;
        rtd_isr_outl(IFD_GRP_DELAY_EN_VADDR, grp_delay_en_rbus_data);
    }
}

#endif
#endif


