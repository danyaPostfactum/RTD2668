/*===========================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2009
 * All rights reserved.
 * ========================================================== */

/*==================== File Description ===================== */
/**
 * @file
 * 	This file is string definition for OSD related functions.
 *
 * @author 	$Author: cf.tsai $
 * @date 	$Date: 2009-03-16 $
 * @version $Revision: 10 $
 */

/**
 * @addtogroup osd
 * @{
 */

#define _OSDSTRING_C

/*===================== Module dependency  ================== */
#include "autoconf.h"
#include "rtd_types.h"
#include "osdstring.h"
#include "osdfont.h"
#include "osdflow.h"
#include "osdcore.h"
#include "osdParameter.h"


/*======================= Private Types ===================== */


/*======================== Definitions ====================== */


/*========================== Variables ====================== */
#define GOLBAL
#define CONFIG_OSD_LANGUAGE_GERMAN 1
#define CONFIG_OSD_LANGUAGE_CHINESE 2


//============================================================================



#define _PICTURE_SETTING_STR_
/* ===========================================================================
 * Picture setting items
 */
UINT8 code tOsdContrastStr[] =
{
    0x13,0x49,0x48,0x4E,0x4C,0x3A,0x4D,0x4E,_END_,
    0x8C,0x8D,0x86,0x87,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x13,0x49,0x48,0x4E,0x4C,0x3A,0x4D,0x4E,_END_,
    0x1C,0x49,0x48,0x4E,0x4C,0x3A,0x4D,0x4E,_END_,
    0x13,0x49,0x48,0x4E,0x4C,0x4D,0x4E,0x3E,_END_,
    _THE_SAME_,
    /*²üÄõ*/0x84,0x85,0x86,_END_,
    /*¸²¤ú*/0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,_END_,
    /*¸²¤ú*/0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,_END_,
    /*ªâÄõ*/_K_,_o_,_n_,_t_,_r_,_a_,_s_,_t_,_i_,_END_,
    /*¤¦³Á*/_K_,_o_,_n_,_t_,_r_,_a_,_s_,_t_,_END_,
    /*·ç¨å*/_K_,_o_,_n_,_t_,_r_,_a_,_s_,_t_,_END_,
    /*ªiÄõ*/_K_,_o_,_n_,_t_,_r_,_a_,_s_,_t_,_END_,
    0x9B,0x85,0x83,0x8A,0x87,0x90,0x89,0x8A,0x83,0x85,_END_,
#endif
};

UINT8 code tOsdBrightnessStr[] =
{
    0x12,0x4C,0x42,0x40,0x41,0x4E,0x48,0x3E,0x4D,0x4D,_END_,
    0x88,0x89,0x82,0x83,0x86,0x87,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x1D,0x4F,0x46,0x47,0x42,0x48,0x49,0x4D,0x42,0x4E,_END_,
    0x19,0x3E,0x45,0x45,0x42,0x40,0x44,0x3E,0x42,0x4E,_END_,
    0x12,0x4C,0x42,0x45,0x45,0x49,_END_,
    0x1D,0x4F,0x46,0x47,0x42,0x48,0x49,0x4D,0x42,0x4E,0x5C,_END_,
    /*²üÄõ*/0x82,0x83,_END_,
    /*¸²¤ú*/0x80,0x81,0x82,0x83,0x84,_END_,
    /*¸²¤ú*/0x80,0x81,0x82,0x83,0x84,_END_,
    /*ªâÄõ*/_K_,_i_,_r_,_k_,_k_,_a_,_u_,_s_,_END_,
    /*¤¦³Á*/_L_,_y_,_s_,_s_,_t_,_y_,_r_,_k_,_e_,_END_,
    /*·ç¨å*/_L_,_j_,_u_,_s_,_s_,_t_,_y_,_r_,_k_,_a_,_END_,
    /*ªiÄõ*/_J_,_a_,_s_,_n_,_o_,0xb8,0xb9,_END_,
    0x9C,0x87,0x9B,0x85,0x89,0x8A,0x8C,_END_,
#endif
};


UINT8 code tOsdHueStr[]=
{
    0x19,0x4F,0x3E,_END_,
    0x8E,0x8F,0x84,0x85,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x28,0x3E,0x42,0x48,0x4E,0x3E,_END_,
    0x16,0x3A,0x4C,0x3B,0x4E,0x49,0x48,_END_,
    0x1E,0x1F,0x3A,0x4E,0x42,0x55,_END_,
    0x28,0x49,0x48,0x3A,0x45,0x42,0x4E,0x3A,0x45,_END_,
    /*²üÄõ*/0xC1,0xC2,_END_,
    /*¸²¤ú*/0x8D,0x8E,0x8F,0x90,_END_,
    /*¸²¤ú*/0x8D,0x8E,0x8F,0x90,_END_,
    /*ªâÄõ*/_S_,0x91,_v_,_y_,_END_,
    /*¤¦³Á*/_N_,_u_,_a_,_n_,_c_,_e_,_END_,
    /*·ç¨å*/_N_,_y_,_a_,_n_,_s_,_END_,
    /*ªiÄõ*/_B_,_a_,_r_,_w_,_a_,_END_,
     0x85,0x8a,0x8a,0x9e,0x83,0x85,0x9b,_END_,
#endif
};

UINT8 code tOsdSaturationStr[]=
{
    0x27,0x3A,0x4E,0x4F,0x4C,0x3A,0x4E,0x42,0x49,0x48,_END_,
    0x80,0x81,0x8A,0x8B,0x86,0x87,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x27,0x3A,0x4E,0x4F,0x4C,0x3A,0x4E,0x42,0x49,0x48,_END_,
    0x27,0x59,0x13,0x4E,0x4E,0x42,0x40,0x4F,0x48,0x40,_END_,
    0x27,0x3A,0x4E,0x4F,0x4C,0x3A,0x3C,0x42,0x5C,0x16,0x48,_END_,
    0x27,0x3A,0x4E,0x4F,0x4C,0x3A,0x55,0x42,0x49,0x48,0x3E,_END_,
    /*²üÄõ*/0xb1,0xC5,_END_,
    /*¸²¤ú*/0x91,0x92,0x93,0x94,_END_,
    /*¸²¤ú*/0x91,0x92,0x93,0x94,0x95,0x96,_END_,
    /*ªâÄõ*/_K_,_y_,_l_,_l_,0x91,_i_,_s_,_y_,_y_,_s_,_END_,
    /*¤¦³Á*/_M_,0xb6,_t_,_h_,_e_,_d_,_END_,
    /*·ç¨å*/_M_,0x91,_t_,_t_,_n_,_a_,_d_,_END_,
    /*ªiÄõ*/_N_,_a_,_s_,_y_,_c_,_e_,_n_,_l_,_e_,_END_,
    0x83,0x90,0x89,0x8D,0x95,0x9E,0x83,0x83,0x85,_END_,
#endif
};


UINT8 code tOsdSharpnessStr[] =
{
    0x27,0x41,0x3A,0x4C,0x4A,0x48,0x3E,0x4D,0x4D,_END_,
    0x92,0x93,0x94,0x95,0x86,0x87,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x20,0x3E,0x4E,0x4E,0x3E,0x4E,_END_,
    0x12,0x42,0x45,0x3D,0x4D,0x3C,0x41,0x59,0x4C,0x3F,0x3E,_END_,
    0x20,0x42,0x4E,0x42,0x3D,0x3E,0x55,_END_,
    0x20,0x42,0x4E,0x42,0x3D,0x3E,0x55,0x55,0x3A,_END_,
    /*²üÄõ*/0xcf,0xbe,0x89,_END_,
    /*¸²¤ú*/0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,_END_,
    /*¸²¤ú*/0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,_END_,
    /*ªâÄõ*/_T_,_e_,_r_,0x91,_v_,_y_,_y_,_s_,_END_,
    /*¤¦³Á*/_S_,_k_,_a_,_r_,_p_,_h_,_e_,_d_,_END_,
    /*·ç¨å*/_S_,_k_,0x91,_r_,_p_,_a_,_END_,
    /*ªiÄõ*/_O_,_s_,_t_,_r_,_o_,0xb8,0xb9,_END_,
    0x87,0x98,0x8E,0x9B,0x85,0x89,0x8A,0x8C,_END_,
#endif
};


UINT8 code tOsdBacklightStr[] =
{
    _B_,_a_,_c_,_k_,_l_,_i_,_g_,_h_,_t_,_END_,
    _B_,_a_,_c_,_k_,_l_,_i_,_g_,_h_,_t_,_END_,

#if defined(CONFIG_EUROPE_LANGUAGE)
    _C_,_o_,_n_,_t_,_r_,_e_,_MINUS_,_j_,_o_,_u_,_r_,_END_,
    _H_,_i_,_n_,_t_,_e_,_r_,_g_,_r_,_u_,_n_,_d_,_b_,_e_,_l_,_e_,_u_,_c_,_h_,_t_,_u_,_n_,_g_,_END_,
    _C_,_o_,_n_,_t_,_r_,_a_,_l_,_u_,_z_,_END_,
    _L_,_a_,_m_,_p_,_a_,_d_,_i_,_n_,_a_,_END_,
    /*²üÄõ*/_A_,_c_,_h_,_t_,_e_,_r_,_g_,_r_,_o_,_n_,_d_,_v_,_e_,_r_,_l_,_i_,_c_,_h_,_t_,_i_,_n_,_END_,
    /*¸²¤ú*/_L_,_u_,_z_,___,_d_,_e_,___,_f_,_u_,_n_,_d_,_o_,_END_,
    /*®¿«Â*/_B_,_a_,_k_,_l_,_y_,_s_,_END_,
    /*ªâÄõ*/_T_,_a_,_u_,_s_,_t_,_a_,_v_,_a_,_l_,_o_,_END_,
    /*¤¦³Á*/_B_,_a_,_g_,_g_,_r_,_u_,_n_,_d_,_s_,_l_,_y_,_s_,_END_,
    /*·ç¨å*/_B_,_a_,_c_,_k_,_l_,_i_,_g_,_h_,_t_,_END_,
    /*ªiÄõ*/_P_,_o_,_d_,0xb8,_w_,_i_,_e_,_t_,_l_,_e_,_n_,_i_,_e_,_END_,
#endif
};

UINT8 code tOsdIcmStr[] =
{
    _I_,_C_,_M_,_END_,
};

UINT8 code tOsdDCRStr[] =
{
    0x14,0x13,0x26,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,_END_,
    0x14,0x13,0x26,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};

UINT8 code tOsdDNRStr[] =
{
    0x14,0x20,0x26,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xbf,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};

UINT8 code tOsdDNRAutoStr[]=
{
    0x10,0x11,0x4F,0x4E,0x49,_END_,
};
UINT8 code tOsdDNRLowStr[]=
{
    0x1D,0x49,0x51,0x52,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
   0xA4,0xA5,0xA6,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};
UINT8 code tOsdDNRMidStr[]=
{
    0x1E,0x1F,0x42,0x3D,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    0xB3,0xB4,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};
UINT8 code tOsdDNRHighStr[]=
{
    0x19,0x42,0x40,0x41,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    0xAa,0xAb,0xAc,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};
UINT8 code tOsdDNROffStr[]=
{
    0x49,0x3F,0x3F,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    /*®¿«Â*/0xb0,0xb1,0xb2,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};


#define _PICTURE_MODE_SETTING_STR_
UINT8 code tStr_PictureMode_Vivid[]=
{
    _B_,_r_,_i_,_g_,_h_,_t_,_END_,
};
UINT8 code tStr_PictureMode_Standard[]=
{
    _S_,_t_,_a_,_n_,_d_,_a_,_r_,_d_,_END_,
};
UINT8 code tStr_PictureMode_Gentle[]=
{
    _S_,_o_,_f_,_t_,_END_,
};
UINT8 code tStr_PictureMode_User[]=
{
    _U_,_s_,_e_,_r_,_END_,
};

UINT8 code tStr_PictureMode[]=
{
    _P_,_i_,_c_,_t_,_u_,_r_,_e_,0x01,_M_,_o_,_d_,_e_,_END_,
};

UINT8* code tPictureMode_StrSet[] =
{
    tStr_PictureMode_Vivid,
    tStr_PictureMode_Standard,
    tStr_PictureMode_Gentle,
    tStr_PictureMode_User
};


#define _EQ_MODE_SETTING_STR_
UINT8 code tStr_EQ_Disable[]=
{
    _S_,_t_,_a_,_n_,_d_,_a_,_r_,_d_,_END_,
};
UINT8 code tStr_EQ_Club[]=
{
    //_C_,_l_,_u_,_b_,_END_,
    _C_,_l_,_a_,_s_,_s_,_i_,_c_,_a_,_l_,_END_,
};
UINT8 code tStr_EQ_Dance[]=
{
    _D_,_a_,_n_,_c_,_e_,_END_,
};
UINT8 code tStr_EQ_Live[]=
{
    //_L_,_i_,_v_,_e_,_END_,
    _C_,_o_,_u_,_n_,_t_,_r_,_y_,_END_,
};
UINT8 code tStr_EQ_Pop[]=
{
    _P_,_o_,_p_,_END_,
};
UINT8 code tStr_EQ_Rock[]=
{
    _R_,_o_,_c_,_k_,_END_,
};
UINT8 code tStr_EQ_User[]=
{
    _U_,_s_,_e_,_r_,_END_,
};

UINT8 code tStr_MusicMode[]=
{
    _M_,_u_,_s_,_i_,_c_,0x10,_M_,_o_,_d_,_e_,_END_
};

UINT8* code tMusicMode_StrSet[] =
{
    tStr_EQ_Disable,
    tStr_EQ_Club,
    tStr_EQ_Dance,
    tStr_EQ_Live,
    tStr_EQ_Pop,
    tStr_EQ_Rock,
    tStr_EQ_User
};

UINT8 code tStr_MTS[]=
{
    _M_,_T_,_S_,0x00,_M_,_o_,_d_,_e_,_END_
};

UINT8 code tStr_Mono[]=
{
    _M_,_o_,_n_,_o_,_END_,
};

UINT8 code tStr_Stereo[]=
{
    _S_,_t_,_e_,_r_,_e_,_o_,_END_,
};

UINT8 code tStr_Dual[]=
{
    _D_,_u_,_a_,_l_,_END_,
};

UINT8* code tMTS_StrSet[] =
{
    tStr_Mono,
    tStr_Stereo,
    tStr_Dual
};


#define _VGA_SETTING_STR_
/* ===========================================================================
 * VGA menu items
 */
UINT8 code tOsdAutoAdjustStr[] =
{
    0x10,0x11,0x4F,0x4E,0x49,0x01,0x13,0x49,0x48,0x3F,0x42,0x40,_END_,
    0x8C,0x8D,0x82,0x83,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x13,0x49,0x48,0x3F,0x42,0x40,0x5F,0x10,0x11,0x4F,0x4E,0x49,_END_,
    0x10,0x11,0x4F,0x4E,0x49,0x5F,0x44,0x49,0x48,0x3F,0x42,0x40,0x4F,0x4C,0x42,0x3E,0x4C,0x3E,0x48,_END_,
    0x10,0x11,0x4F,0x4E,0x49,0x5F,0x27,0x3E,0x45,0x3E,0x3C,0x3C,0x42,0x5C,0x16,0x48,_END_,
    0x13,0x49,0x48,0x3F,0x42,0x40,0x5F,0x01,0x10,0x11,0x4F,0x4E,0x49,0x46,0x47,0x5F,_END_,
    /*²üÄõ*/0x8C,0x8D,0x8E,0x8F,_END_,
    /*¸²¤ú*/0x10,0x11,0x4F,0x4E,0x49,_END_,
    /*®¿«Â*/0x8B,0x8C,0x8D,0x8E,_END_,
    /*ªâÄõ*/_A_,_u_,_t_,_o_,_END_,
    /*¤¦³Á*/_A_,_u_,_t_,_o_,_m_,0x0e,_END_,
    /*·ç¨å*/_A_,_u_,_t_,_o_,_END_,
    /*ªiÄõ*/_A_,_u_,_t_,_o_,_m_,_a_,_t_,_y_,_c_,_z_,_n_,_e_,_END_,
          0x90,0x84,0x8A,0x85,_END_,
#endif
};


UINT8 code tOsdClockStr[] =
{
    0x13,0x45,0x49,0x3C,0x44,_END_,
    0x84,0x85,0x8A,0x8B,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x19,0x49,0x4C,0x45,0x49,0x40,0x3E,_END_,
    0x28,0x3A,0x44,0x4E,_END_,
    0x26,0x3E,0x45,0x49,0x43,_END_,
    0x13,0x45,0x49,0x3C,0x44,_END_,
    /*²üÄõ*/0x96,0x97,_END_,
    /*¸²¤ú*/0x26,0x3E,0x45,0x40,0x41,0x49,_END_,
    /*®¿«Â*/0x92,0x93,0x94,0x95,0x96,_END_,
    /*ªâÄõ*/_K_,_e_,_l_,_l_,_o_,_END_,
    /*¤¦³Á*/_K_,_l_,_o_,_k_,_k_,_e_,_END_,
    /*·ç¨å*/_K_,_l_,_o_,_c_,_k_,_a_,_END_,
    /*ªiÄõ*/_Z_,_e_,_g_,_a_,_r_,_END_,
    0x97,0x90,0x89,0x8D,_END_,
#endif
};


UINT8 code tOsdPhaseStr[] =
{
    0x23,0x41,0x3A,0x4D,0x3E,_END_,
    0x86,0x87,0x8E,0x8F,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x23,0x41,0x3A,0x4D,0x3E,_END_,
    _THE_SAME_,
    _THE_SAME_,
    0x16,0x3A,0x4D,0x3E,_END_,
    /*²üÄõ*/0x94,0x95,_END_,
    /*¸²¤ú*/ 0x16,0x3A,0x4D,0x3E,_END_,
    /*®¿«Â*/0x97,0x98,0x99,_END_,
    /*ªâÄõ*/_V_,_a_,_i_,_h_,_e_,_END_,
    /*¤¦³Á*/_F_,_a_,_s_,_e_,_END_,
    /*·ç¨å*/_F_,_a_,_s_,_END_,
    /*ªiÄõ*/_F_,_a_,_z_,_a_,_END_,
    0x9D,0x90,0x8E,0x90,_END_,
#endif
};


UINT8 code tOsdHPositionStr[] =
{
    0x19,0x01,0x23,0x49,0x4D,0x42,0x4E,0x42,0x49,0x48,_END_,
    0x90,0x91,0x92,0x93,0x8E,0x8F,0x98,0x99,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x23,0x49,0x4D,0x42,0x4E,0x42,0x49,0x48,0x01,0x19,0x5F,_END_,
    0x19,0x5F,0x23,0x49,0x4D,0x42,0x4E,0x42,0x49,0x48,_END_,
    0x23,0x49,0x4D,0x42,0x3C,0x42,0x5C,0x16,0x48,0x01,0x19,_END_,
    0x23,0x49,0x4D,0x42,0x55,0x42,0x49,0x48,0x3E,0x01,0x21,0x22,0x4C,0x42,0x5F,_END_,
    /*²üÄõ*/0x90,0x91,0x92,0x8D,_END_,
    0x19,0x01,0x23,0x49,0x4D,0x42,0x4E,0x42,0x49,0x48,_END_,
    /*®¿«Â*/0xc3,0xc4,0xc5,0xbd,0xBE,0xBF,0xC0,0xC1,0xC2,_END_,
    /*ªâÄõ*/_V_,_a_,_a_,_k_,_a_,_s_,_i_,_j_,_a_,_i_,_n_,_t_,_i_,_END_,
    /*¤¦³Á*/_H_,_MINUS_,_p_,_o_,_s_,_i_,_t_,_i_,_o_,_n_,_END_,
    /*·ç¨å*/_H_,_DOT_,_P_,_o_,_s_,_i_,_t_,_i_,_o_,_n_,_END_,
    /*ªiÄõ*/_P_,_o_,_z_,_y_,_c_,_j_,_a_,___,_p_,_o_,_z_,_i_,_o_,_m_,_a_,_END_,
    0x91,0x85,0x87,0x01,0x86,0x85,0x80,0x85,0x88,0x9E,_END_,
#endif
};


UINT8 code tOsdVPositionStr[] =
{
    0x2A,0x01,0x23,0x49,0x4D,0x42,0x4E,0x42,0x49,0x48,_END_,
    0x94,0x95,0x96,0x97,0x8E,0x8F,0x98,0x99,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x23,0x49,0x4D,0x42,0x4E,0x42,0x49,0x48,0x01,0x2A,0x5F,_END_,
    0x2A,0x5F,0x23,0x49,0x4D,0x42,0x4E,0x42,0x49,0x48,_END_,
    0x23,0x49,0x4D,0x42,0x3C,0x42,0x5C,0x16,0x48,0x01,0x2A,_END_,
    0x23,0x49,0x4D,0x42,0x55,0x42,0x49,0x48,0x3E,0x01,0x2A,0x3E,0x4C,0x5F,_END_,
    /*²üÄõ*/0x90,0x93,0x92,0x8D,_END_,
    0x2A,0x01,0x23,0x49,0x4D,0x42,0x4E,0x42,0x49,0x48,_END_,
    /*®¿«Â*/0x8F,0x90,0x91,0xBD,0xBE,0xBF,0xC0,0xC1,0xC2,_END_,
    /*ªâÄõ*/_P_,_y_,_s_,_t_,_y_,_s_,_i_,_j_,_a_,_i_,_n_,_t_,_i_,_END_,
    /*¤¦³Á*/_V_,_MINUS_,_p_,_o_,_s_,_i_,_t_,_i_,_o_,_n_,_END_,
    /*·ç¨å*/_V_,_DOT_,_P_,_o_,_s_,_i_,_t_,_i_,_o_,_n_,_END_,
    /*ªiÄõ*/_P_,_o_,_z_,_y_,_c_,_j_,_a_,___,_p_,_i_,_o_,_n_,_o_,_w_,_a_,_END_,
0x84,0x9E,0x87,0x01,0x86,0x85,0x80,0x85,0x88,0x9E,_END_
#endif
};

UINT8 code tOsdHSizeStr[] =
{
    _H_,_DOT_,_S_,_i_,_z_,_e_,_END_,
    _H_,_DOT_,_S_,_i_,_z_,_e_,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};

UINT8 code tOsdVSizeStr[] =
{
    _V_,_DOT_,_S_,_i_,_z_,_e_,_END_,
    _V_,_DOT_,_S_,_i_,_z_,_e_,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};

UINT8 code tOsdColorTempStr[] =
{
    0x13,0x49,0x45,0x49,0x4C,0x01,0x28,0x3E,0x46,0x47,0x4A,_END_,
    0x8E,0x8F,0x90,0x91,_END_,

#if defined(CONFIG_EUROPE_LANGUAGE)
    0x13,0x49,0x45,0x49,0x4C,0x01,0x28,0x3E,0x46,0x47,0x4A,_END_,
    0x16,0x3A,0x4C,0x3B,0x4E,0x3E,0x46,0x47,0x4A,0x5F,_END_,
    0x13,0x49,0x45,0x49,0x4C,0x01,0x28,0x3E,0x46,0x47,0x4A,_END_,
    0x13,0x49,0x45,0x49,0x4C,0x01,0x28,0x3E,0x46,0x47,0x4A,_END_,
    0x87,0x88,0x89,_END_,
    0x13,0x49,0x45,0x49,0x4C,0x01,0x28,0x3E,0x46,0x47,0x4A,_END_,
    0x9F,0xA0,0xA1,0xA2,0xA3,_END_,
    0x13,0x49,0x45,0x49,0x4C,0x01,0x28,0x3E,0x46,0x47,0x4A,_END_,
    0x13,0x49,0x45,0x49,0x4C,0x01,0x28,0x3E,0x46,0x47,0x4A,_END_,
    0x13,0x49,0x45,0x49,0x4C,0x01,0x28,0x3E,0x46,0x47,0x4A,_END_,
    0x13,0x49,0x45,0x49,0x4C,0x01,0x28,0x3E,0x46,0x47,0x4A,_END_,
    0x8A,0x9E,0x92,0x86,0x9E,0x87,0x00,0x99,0x84,0x9E,0x8A,0x90,_END_,
  #endif
};


UINT8 code tOsdAutoColorStr[] =
{
    0x10,0x11,0x4F,0x4E,0x49,0x01,0x13,0x49,0x45,0x49,0x4C,_END_,
    _THE_SAME_,

#if defined(CONFIG_EUROPE_LANGUAGE)
    _C_,_o_,_u_,_l_,_e_,_u_,_r_,___,_a_,_u_,_t_,_o_,_END_,
    _A_,_u_,_t_,_o_,_MINUS_,_f_,_a_,_r_,_b_,_e_,_END_,
    _C_,_o_,_l_,_o_,_r_,___,_a_,_u_,_t_,_o_,_m_,_DOT_,_END_,
    _C_,_o_,_l_,_o_,_r_,_e_,___,_a_,_u_,_t_,_o_,_m_,_DOT_,_END_,
    /*²üÄõ*/_A_,_u_,_t_,_o_,___,_k_,_l_,_e_,_u_,_r_,_END_,
    /*¸²¤ú*/_C_,_o_,_r_,___,_a_,_u_,_t_,_o_,_m_,0x0a,_t_,_i_,_c_,_a_,_END_,
    /*®¿«Â*/_A_,_u_,_t_,_o_,_f_,_a_,_r_,_g_,_e_,_END_,
    /*ªâÄõ*/_A_,_u_,_t_,_o_,_m_,0x0e,_v_,0x91,_r_,_i_,_END_,
    /*¤¦³Á*/_A_,_u_,_t_,_o_,_m_,0x0e,_f_,_a_,_r_,_v_,_e_,_END_,
    /*·ç¨å*/_A_,_u_,_t_,_o_,_f_,0x91,_r_,_g_,_END_,
    /*ªiÄõ*/_A_,_u_,_t_,_o_,_m_,_a_,_t_,_y_,_c_,_z_,_n_,_y_,___,_k_,_o_,_l_,_o_,_r_,_END_,
#endif
};


#define _AUDIO_SETTING_STR_
/*============================================================================
 * Audio related OSD strings
 */
UINT8 code tOsdAudioBassStr[]=
{
    0x12,0x3A,0x4D,0x4D,_END_,
    0x86,0x87,0x84,0x85,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x27,0x49,0x48,0x01,0x40,0x4C,0x3A,0x50,0x3E,_END_,
    0x28,0x42,0x3E,0x3F,0x3E,0x48,_END_,
    0x12,0x3A,0x4D,0x4D,_END_,
    0x12,0x3A,0x4D,0x4D,0x42,_END_,
    /*²üÄõ*/0xD0,0xB4,_END_,
    /*¸²¤ú*/0x17,0x18,0x4C,0x3A,0x50,0x3E,0x4D,_END_,
    /*®¿«Â*/0x85,0x86,0x87,_END_,
    /*ªâÄõ*/_B_,_a_,_s_,_s_,_o_,_END_,
    /*¤¦³Á*/_B_,_a_,_s_,_END_,
    /*·ç¨å*/_B_,_a_,_s_,_END_,
    /*ªiÄõ*/_B_,_a_,_s_,_y_,_END_,
    0x83,0x97,_END_,
#endif
};


UINT8 code tOsdAudioTrebleStr[]=
{
    0x28,0x4C,0x3E,0x3B,0x45,0x3E,_END_,
    0x88,0x89,0x84,0x85,_END_,

#if defined(CONFIG_EUROPE_LANGUAGE)
    0x10,0x11,0x42,0x40,0x4F,_END_,
    0x19,0x49,0x3C,0x41,0x4E,0x49,0x48,_END_,
    0x10,0x40,0x4F,0x3D,0x49,0x4D,_END_,
    0x10,0x11,0x45,0x4E,0x42,_END_,
    /*²üÄõ*/0xD1,0xB4,_END_,
    /*¸²¤ú*/0x10,0x11,0x40,0x4F,0x3D,0x49,0x4D,_END_,
    /*®¿«Â*/0x88,0x89,0x8A,_END_,
    /*ªâÄõ*/_D_,_i_,_s_,_k_,_a_,_n_,_t_,_t_,_i_,_END_,
    /*¤¦³Á*/_D_,_i_,_s_,_k_,_a_,_n_,_t_,_END_,
    /*·ç¨å*/_D_,_i_,_s_,_k_,_a_,_n_,_t_,_END_,
    /*ªiÄõ*/_T_,_o_,_n_,_y_,___,_w_,_y_,_s_,_o_,_k_,_i_,_e_,_END_,
    0x96,0x90,0x80,0x90,0x83,0x89,_END_,
#endif
};


UINT8 code tOsdAudioBalanceStr[] =
{
    0x12,0x3A,0x45,0x3A,0x48,0x3C,0x3E,_END_,
    0x82,0x83,0x80,0x81,_END_,

#if defined(CONFIG_EUROPE_LANGUAGE)
    0x12,0x3A,0x45,0x3A,0x48,0x3C,0x3E,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    /*²üÄõ*/0xA2,0xCD,_END_,
    /*¸²¤ú*/0x12,0x3A,0x45,0x3A,0x48,0x5F,0x49,0x5F,_END_,
    /*®¿«Â*/0x80,0x81,0x82,0x83,0x84,_END_,
    /*ªâÄõ*/_T_,_a_,_s_,_a_,_p_,_a_,_i_,_n_,_o_,_END_,
    /*¤¦³Á*/_B_,_a_,_l_,_a_,_n_,_c_,_e_,_END_,
    /*·ç¨å*/_B_,_a_,_l_,_a_,_n_,_s_,_END_,
    /*ªiÄõ*/_B_,_a_,_l_,_a_,_n_,_s_,_END_,
    0x96,0x90,0x80,0x90,0x83,0x89,_END_,
#endif
};


UINT8 code tOsdAudioMuteStr[]=
{
    _M_,_u_,_t_,_e_,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _S_,_i_,_l_,_e_,_n_,_c_,_e_,_END_,
    _S_,_t_,_u_,_m_,_m_,_END_,
    _S_,_i_,_l_,_e_,_n_,_c_,_i_,_o_,_END_,
    _M_,_u_,_t_,_e_,_END_,
    /*²üÄõ*/_D_,_e_,_m_,_p_,_e_,_n_,_END_,
    /*¸²¤ú*/_C_,_o_,_r_,_t_,_a_,_r_,___,_o_,___,_s_,_o_,_m_,_END_,
    /*®¿«Â*/_D_,_e_,_m_,_p_,_END_,
    /*ªâÄõ*/_M_,_y_,_k_,_i_,_s_,_t_,_y_,_s_,_END_,
    /*¤¦³Á*/_T_,_a_,_v_,_s_,_END_,
    /*·ç¨å*/_M_,_u_,_t_,_e_,_END_,
    /*ªiÄõ*/_W_,_y_,_c_,_i_,_s_,_z_,_e_,_n_,_i_,_e_,_END_,
#endif
};


UINT8 code tOsdAudioSpatialStr[]=
{
    _S_,_u_,_r_,_r_,_o_,_u_,_n_,_d_,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};

UINT8 code tStr_Audio_Equalizer[] =
{
    0x15,0x4B,0x4F,0x3A,0x45,0x42,0x55,0x3E,0x4C,_END_,
    0x8A,0x8B,0x80,0x81,0x8C,0x8D,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x15,0x4B,0x4F,0x3A,0x45,0x42,0x55,0x3E,0x4C,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    0xA1,0xA2,0xA3,0xA4,_END_,
    0x15,0x4B,0x4F,0x3A,0x45,0x42,0x55,0x3E,0x4C,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif

};



#define _CHANNEL_SETTING_STR_
/*============================================================================
 * Channel setting related OSD strings
 */
UINT8 code tOsdAutoScanStr[]=
{
    0x10,0x11,0x4F,0x4E,0x49,0x01,0x27,0x3E,0x3A,0x4C,0x3C,0x41,_END_,
    0x94,0x95,0x82,0x83,0x8A,0x8B,0x8C,0x8D,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x26,0x3E,0x3C,0x41,0x3E,0x4C,0x3C,0x41,0x3E,0x01,0x10,0x11,0x4F,0x4E,0x49,_END_,
    0x10,0x11,0x4F,0x4E,0x49,0x5F,0x27,0x4F,0x3C,0x41,0x3E,_END_,
    0x10,0x11,0x4F,0x4E,0x49,0x5F,0x27,0x3E,0x45,0x3E,0x3C,0x3C,0x42,0x5C,0x16,0x48,_END_,
    0x13,0x3E,0x4C,0x3C,0x3A,0x01,0x10,0x11,0x4F,0x4E,0x3A,_END_,
    /*²üÄõ*/0x8C,0x8D,0xB5,0xB6,_END_,
    0x13,0x3E,0x4C,0x3C,0x3A,0x01,0x10,0x11,0x4F,0x4E,0x3A,_END_,
    /*®¿«Â*/0x9D,0x9E,0x9F,0xA0,_END_,
    /*ªâÄõ*/_A_,_u_,_t_,_o_,_m_,0x0e,_s_,_k_,_a_,_n_,_n_,_a_,_u_,_s_,_END_,
    /*¤¦³Á*/_A_,_u_,_t_,_o_,_m_,0x0e,_s_,_k_,_a_,_n_,_n_,_i_,_n_,_g_,_END_,
    /*·ç¨å*/_A_,_u_,_t_,_o_,_s_,0x93,_k_,_END_,
    /*ªiÄõ*/_A_,_u_,_t_,_o_,_m_,0x0e,_s_,_k_,_a_,_n_,_o_,_w_,_a_,_n_,_i_,_e_,_END_,
0x90,0x84,0x8A,0x85,0x01,0x86,0x85,0x8F,0x89,0x9B,0x8F,_END_,
#endif
};


UINT8 code tOsdTableScanStr[] =
{
#if 1
    0x28,0x3A,0x3B,0x45,0x3E,0x00,0x27,0x3C,0x3A,0x48,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    /*®¿«Â*/0x9D,0x9E,0x9F,0xA0,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
#else//Tim 0325
    0x10,0x11,0x4F,0x4E,0x49,0x01,0x27,0x3E,0x3A,0x4C,0x3C,0x41,_END_,
    0x94,0x95,0x82,0x83,0x8A,0x8B,0x8C,0x8D,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x26,0x3E,0x3C,0x41,0x3E,0x4C,0x3C,0x41,0x3E,0x01,0x10,0x11,0x4F,0x4E,0x49,_END_,
    0x10,0x11,0x4F,0x4E,0x49,0x5F,0x27,0x4F,0x3C,0x41,0x3E,_END_,
    0x10,0x11,0x4F,0x4E,0x49,0x5F,0x27,0x3E,0x45,0x3E,0x3C,0x3C,0x42,0x5C,0x16,0x48,_END_,
    0x13,0x3E,0x4C,0x3C,0x3A,0x01,0x10,0x11,0x4F,0x4E,0x3A,_END_,
    /*²üÄõ*/0x8C,0x8D,0xB5,0xB6,_END_,
    0x13,0x3E,0x4C,0x3C,0x3A,0x01,0x10,0x11,0x4F,0x4E,0x3A,_END_,
    /*®¿«Â*/_A_,_u_,_t_,_o_,_s_,0x9b,_k_,_END_,
    /*ªâÄõ*/_A_,_u_,_t_,_o_,_m_,0x0e,_s_,_k_,_a_,_n_,_n_,_a_,_u_,_s_,_END_,
    /*¤¦³Á*/_A_,_u_,_t_,_o_,_m_,0x0e,_s_,_k_,_a_,_n_,_n_,_i_,_n_,_g_,_END_,
    /*·ç¨å*/_A_,_u_,_t_,_o_,_s_,0x93,_k_,_END_,
    /*ªiÄõ*/_A_,_u_,_t_,_o_,_m_,0x0e,_s_,_k_,_a_,_n_,_o_,_w_,_a_,_n_,_i_,_e_,_END_,
#endif

#endif
};

UINT8 code tStr_ManualScan[] =
{
    0x1E,0x1F,0x3A,0x48,0x4F,0x3A,0x45,0x01,0x27,0x3E,0x3A,0x4C,0x3C,0x41,_END_,
    0x88,0x89,0x82,0x83,0x8A,0x8B,0x8C,0x8D,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x26,0x3E,0x3C,0x41,0x3E,0x4C,0x1E,0x1F,0x3A,0x48,0x4F,0x3E,0x45,_END_,
    0x1E,0x1F,0x3A,0x48,0x4F,0x3E,0x45,0x45,0x3E,0x01,0x27,0x4F,0x3C,0x41,0x3E,_END_,
    0x27,0x3E,0x45,0x3E,0x3C,0x5F,0x1E,0x1F,0x3A,0x48,0x4F,0x3A,0x45,_END_,
    0x13,0x3E,0x4C,0x3C,0x3A,0x01,0x1E,0x1F,0x3A,0x48,0x4F,0x3A,0x45,0x3E,_END_,
    0x9D,0xA3,0xA4,0x8B,_END_,
    0x1E,0x1F,0x3A,0x48,0x4F,0x3A,0x45,0x01,0x27,0x3E,0x3A,0x4C,0x3C,0x41,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};

UINT8 code tOSdCanalPlusStr[] =
{
    _C_,_a_,_n_,_a_,_l_,_PLUS_,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};


UINT8 code tStr_Channel_Edit[] =
{
#if DISABLE_OPTTION
   0x13,0x41,0x3A,0x48,0x48,0x3E,0x45,_END_,
   #endif
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _M_,_o_,_d_,_i_,_f_,_i_,_e_,_r_,___,_c_,_h_,_a_,_n_,_n_,_e_,_END_,
    _K_,_a_,_n_,_a_,_l_,___,_b_,_e_,_a_,_r_,_b_,_e_,_DOT_,_END_,
    _E_,_d_,_i_,_t_,_a_,_r_,___,_c_,_a_,_n_,_a_,_l_,_END_,
    _M_,_o_,_d_,_i_,_f_,_i_,_c_,_a_,___,_c_,_a_,_n_,_a_,_l_,_e_,_END_,
    /*²üÄõ*/_K_,_a_,_n_,_a_,_a_,_l_,___,_b_,_e_,_w_,_e_,_r_,_k_,_e_,_n_,_END_,
    /*¸²¤ú*/_E_,_d_,_i_,_t_,_a_,_r_,___,_c_,_a_,_n_,_a_,_i_,_s_,_END_,
    /*®¿«Â*/_K_,_a_,_n_,_a_,_l_,_e_,_n_,_d_,_r_,_i_,_n_,_g_,_END_,
    /*ªâÄõ*/_K_,_a_,_n_,_a_,_v_,_a_,_n_,___,_m_,_u_,_o_,_k_,_k_,_a_,_u_,_s_,_END_,
    /*¤¦³Á*/_K_,_a_,_n_,_a_,_l_,_r_,_e_,_d_,_i_,_g_,_e_,_r_,_i_,_n_,_g_,_END_,
    /*·ç¨å*/_K_,_a_,_n_,_a_,_l_,_r_,_e_,_d_,_i_,_g_,_e_,_r_,_i_,_n_,_g_,_END_,
    /*ªiÄõ*/_E_,_d_,_y_,_c_,_j_,_a_,___,_k_,_a_,_n_,_a_,_l_,0x3f,_w_,_END_,
#endif

};




UINT8 code tOsdBlueScreenStr[] =
{
    _B_,_l_,_u_,_e_,___,_S_,_c_,_r_,_e_,_e_,_n_,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};

UINT8 code tStr_CH[] =
{
    _C_,_H_,_END_,
};

UINT8 code tStr_Name[] =
{
    _N_,_a_,_m_,_e_,_END_,
};

UINT8 code tStr_Skip[] =
{
    _S_,_k_,_i_,_p_,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    0xA5,0xA6,0xA7,0xA8,0xA9,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
0x86,0x87,0x8D,0x88,0x85,0x9B,_END_,
#endif
};
UINT8 code tStr_AFC[] =
{
    _A_,_F_,_C_,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};

UINT8 code tStr_MHz[] =
{
    0x1E,0x1F,0x41,0x55,_END_,
};

UINT8 code tOSD_Dual1[]=
{
    _D_,_u_,_a_,_l_,___,_1_,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};


UINT8 code tOSD_Dual2[]=
{
    _D_,_u_,_a_,_l_,___,_2_,_END_,
    _THE_SAME_,

#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};



/*============================================================================
 * Country Name for 23 Countries
UINT8 code sAustria[] = { // "Austria"                     00
	0x10,0x11,0x4F,0x4D,0x4E,0x4C,0x42,0x3A,_END_,                     // English
};
UINT8 code sBelgium[] = { // "Belgium"                     01
	0x12,0x3E,0x45,0x40,0x42,0x4F,0x46,0x47,_END_,                     // English
};
UINT8 code sCroatia[] = { // "Croatia"                     02
	0x13,0x4C,0x49,0x3A,0x4E,0x42,0x3A,_END_,                     // English
};
UINT8 code sCzech[] = { // "Czech"                         03
	0x13,0x55,0x3E,0x3C,0x41,_END_,                               // English
};
UINT8 code sDenmark[] = { // "Denmark"                     04
	0x14,0x3E,0x48,0x46,0x47,0x3A,0x4C,0x44,_END_,                    // English
};
UINT8 code sFinland[] = { // "Finland"                     05
	0x16,0x42,0x48,0x45,0x3A,0x48,0x3D,_END_,                     // English
};
UINT8 code sFrance[] = { // "France"                       06
	0x16,0x4C,0x3A,0x48,0x3C,0x3E,_END_,                          // English
};
UINT8 code sGermany[] = { // "Germany"                     07
	0x17,0x18,0x3E,0x4C,0x46,0x47,0x3A,0x48,_END_,                     // English
};
UINT8 code sGreece[] = { // "Greece"                       08
	0x17,0x4C,0x3E,0x3E,0x3C,0x3E,_END_,                          // English
};
UINT8 code sHungary[] = { // "Hungary"                     09
	0x19,0x4F,0x48,0x40,0x3A,0x4C,0x54,_END_,                     // English
};
UINT8 code sIceland[] = { // "Iceland"                     10
	0x1A,0x3C,0x3E,0x45,0x3A,0x48,0x3D,_END_,                     // English
};
UINT8 code sIreland[] = { // "Ireland"                     11
	0x1A,0x4C,0x3E,0x45,0x3A,0x48,0x3D,_END_,                     // English
};
UINT8 code sItaly[] = { // "Italy"                         12
	0x1A,0x4E,0x3A,0x45,0x54,_END_,                               // English
};
UINT8 code sNetherlands[] = { // "Netherlands"             13
	0x20,0x3E,0x4E,0x41,0x3E,0x4C,0x45,0x3A,0x48,0x3D,_END_, // English
};
UINT8 code sNorway[] = { // "Norway"                       14
	0x20,0x49,0x4C,0x51,0x52,0x3A,0x54,_END_,                          // English
};
UINT8 code sPoland[] = { // "Poland"                       15
	0x23,0x49,0x45,0x3A,0x48,0x3D,_END_,                          // English
};
UINT8 code sPortugal[] = { // "Portugal"                   16
	0x23,0x49,0x4C,0x4E,0x4F,0x40,0x3A,0x45,_END_,                // English
};
UINT8 code sSanMarino[] = { // "SanMarino"                 17
	0x27,0x3A,0x48,0x1E,0x1F,0x3A,0x4C,0x42,0x48,0x49,_END_,           // English
};
UINT8 code sSpain[] = { // "Spain"                         18
	0x27,0x4A,0x3A,0x42,0x48,_END_,                               // English
};
UINT8 code sSweden[] = { // "Sweden"                       19
	0x27,0x4A,0x3A,0x42,0x48,_END_,                          // English
};
UINT8 code sSwitzerland[] = { // "Switzerland"             20
	0x27,0x51,0x52,0x42,0x4E,0x55,0x3E,0x4C,0x45,0x3A,0x48,0x3D,_END_, // English
};
UINT8 code sTurkey[] = { // "Turkey"                       21
	0x28,0x4F,0x4C,0x44,0x3E,0x54,_END_,                          // English
};
UINT8 code sUK[] = { // "UnitedKingdom"                    22
	0x29,0x1C,_END_,                                              // English
};
UINT8 code sOther[] = { // "Other country"                  23
	0x21,0x22,0x4E,0x41,0x3E,0x4C,0x5F,0x5F,0x5F,_END_,         // English
};

UINT8* code tOsdCountryStrSet[_MAX_COUNTRY_NUMBER] =
{
	sAustria,
	sBelgium,
	sCroatia,
	sCzech,
	sDenmark,
	sFinland,
	sFrance,
	sGermany,
	sGreece,
	sHungary,
	sIceland,
	sIreland,
	sItaly,
	sNetherlands,
	sNorway,
	sPoland,
	sPortugal,
	sSanMarino,
	sSpain,
	sSweden,
	sSwitzerland,
	sTurkey,
	sUK,
	sOther
};
*/

/* ===========================================================================
 * Setting menu items
 */

UINT8 code tOsdOsdBlendStr[] =
{
    0x28,0x4C,0x3A,0x48,0x4D,0x4A,0x3A,0x4C,0x3E,0x48,0x3C,0x54,_END_,
    0x84,0x85,0x80,0x81,_END_,

#if defined(CONFIG_EUROPE_LANGUAGE)
    0x28,0x4C,0x3A,0x48,0x4D,0x4A,0x3A,0x4C,0x3E,0x48,0x3C,0x54,_END_,
    0x28,0x4C,0x3A,0x48,0x4D,0x4A,0x3A,0x4C,0x3E,0x48,0x4E,_END_,
    0x28,0x4C,0x3A,0x4D,0x45,0x5B,0x3C,0x42,0x3D,0x49,_END_,
    0x28,0x4C,0x3A,0x4D,0x45,0x4F,0x3C,0x42,0x3D,0x49,_END_,
    /*²üÄõ*/0x9D,0x84,0x89,_END_,
    /*¸²¤ú*/0x28,0x4C,0x3A,0x48,0x4D,0x4A,0x3A,0x4C,0x48,0x3C,0x42,0x3A,_END_,
    /*®¿«Â*/0x21,0x22,0x27,0x14,0xab,0xAC,0xAD,0xAE,0xAF,_END_,
    /*ªâÄõ*/_L_,0x91,_p_,_i_,_n_,0x91,_k_,_y_,_v_,_y_,_y_,_s_,_END_,
    /*¤¦³Á*/_T_,_r_,_a_,_n_,_s_,_p_,_a_,_r_,_e_,_n_,_s_,_END_,
    /*·ç¨å*/_T_,_r_,_a_,_n_,_s_,_p_,_a_,_r_,_e_,_n_,_s_,_END_,
    /*ªiÄõ*/_P_,_r_,_z_,_e_,_z_,_r_,_o_,_c_,_z_,_y_,_s_,_t_,_o_,0xb8,0xb9,_END_,
    0x8A,0x87,0x90,0x83,0x89,0x86,0x90,0x87,0x9E,_END_,
#endif
};

#if defined(CONFIG_EUROPE_LANGUAGE)
UINT8 code tOsdLanguageStr[] =
{
    0x1D,0x3A,0x48,0x40,0x4F,0x3A,0x40,0x3E,_END_,
    0x8A,0x8B,0x88,0x89,_END_,

    0x1D,0x3A,0x48,0x40,0x4F,0x3E,_END_,
    0x27,0x4A,0x4C,0x3A,0x3C,0x41,0x3E,_END_,
    0x1D,0x3E,0x48,0x40,0x4F,0x3A,0x43,0x3E,_END_,
    0x1A,0x4E,0x3A,0x45,0x42,0x3A,0x48,0x49,_END_,
    0x98,0x99,_END_,
    /*¸²¤ú*/0x1D,0x3D,0x3D,0x42,0x49,0x46,0x47,_END_,
    /*®¿«Â*/0x9A,0x9B,0x9C,_END_,
    /*ªâÄõ*/0x9A,0x9B,0x9C,_END_,
    /*¤¦³Á*/_O_,_S_,_D_,___,_s_,_p_,_r_,_o_,_g_,_END_,
    /*·ç¨å*/_O_,_S_,_D_,___,_s_,_p_,_r_,0x99,_k_,_END_,
    /*ªiÄõ*/_J_,0xc2,_z_,_y_,_k_,___,_O_,_S_,_D_,_END_,
    0x9C,0x8E,0x8D,0x9B,_END_,
};
#endif


UINT8 code tOsdResetStr[] =
{
    0x26,0x3E,0x4D,0x3E,0x4E,_END_,
    0x8E,0x8F,0x86,0x87,_END_,

#if defined(CONFIG_EUROPE_LANGUAGE)
    0x26,0x58,0x3D,0x42,0x48,0x42,0x4E,0x42,0x3A,0x45,0x42,0x4D,0x3A,0x4E,0x42,0x49,0x48,_END_,
    0x2F,0x4F,0x4C,0x5B,0x3C,0x44,0x5F,0x4D,0x3E,0x4E,0x55,0x3E,0x48,_END_,
    0x26,0x3E,0x4D,0x4E,0x3A,0x4F,0x4C,0x3A,0x4C,_END_,
    0x26,0x42,0x4A,0x4C,0x42,0x4D,0x4E,0x42,0x48,0x49,_END_,
    0xd4,0x83,0xA9,_END_,
    0x23,0x4C,0x3E,0x3D,0x3E,0x3F,0x42,0x48,0x42,_END_,
    0xB8,0xB9,0xba,0xbb,0xbc,_END_,
    _T_,_e_,_h_,_d_,_a_,_s_,_a_,_s_,_e_,_t_,_u_,_s_,_t_,_e_,_n_,___,_p_,_a_,_l_,_a_,_u_,_t_,_u_,_s_,_END_,
    _T_,_i_,_l_,_b_,0x0e,_t_,_i_,_l_,_l_,___,_f_,_a_,_b_,_r_,_i_,_k_,_s_,_i_,_n_,_d_,_s_,_t_,_i_,_l_,_l_,_END_,
    _F_,_a_,_b_,_r_,_i_,_k_,_s_,0x99,_t_,_e_,_r_,_s_,_t_,0x91,_l_,_l_,_n_,_i_,_n_,_g_,_END_,
    _R_,_e_,_s_,_e_,_t_,_o_,_w_,_a_,_n_,_i_,_e_,/*___,_d_,_o_,___,_u_,_s_,_t_,_a_,_w_,_i_,_e_,___,_f_,_a_,_b_,_r_,_y_,_c_,_z_,_n_,_y_,_c_,_h_,*/_END_,
    0x84,0x85,0x89,0x89,0x8A,0x90,0x83,0x85,0x84,_END_,
#endif
};


UINT8 code tOsdOsdTimeStr[] =
{
    0x21,0x22,0x27,0x14,0x01,0x28,0x42,0x46,0x47,0x3E,0x49,0x4F,0x4E,_END_,
    0x82,0x83,0x8C,0x8D,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x14,0x4F,0x4C,0x58,0x3E,0x00,0x21,0x22,0x27,0x14,_END_,
    0x21,0x22,0x27,0x14,0x5F,0x10,0x11,0x4F,0x4D,0x55,0x3E,0x42,0x4E,_END_,
    0x28,0x42,0x3E,0x46,0x47,0x4A,0x5F,0x3E,0x4D,0x4A,0x5F,0x21,0x22,0x27,0x14,_END_,
    0x28,0x42,0x46,0x47,0x3E,0x49,0x4F,0x4E,0x01,0x21,0x22,0x27,0x14,_END_,
    /*²üÄõ*/0x21,0x22,0x27,0x14,0x01,0x96,0x97,_END_,
    /*¸²¤ú*/_L_,_i_,_m_,_i_,_t_,_e_,___,_d_,_e_,___,_t_,_e_,_m_,_p_,_o_,_END_,
    /*®¿«Â*/0x21,0x22,0x27,0x14,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,_END_,
    /*ªâÄõ*/_O_,_S_,_D_,_MINUS_,_a_,_i_,_k_,_a_,_k_,_a_,_t_,_k_,_a_,_i_,_s_,_u_,_END_,
    /*¤¦³Á*/_O_,_S_,_D_,___,_t_,_i_,_m_,_e_,_o_,_u_,_t_,_END_,
    /*·ç¨å*/_O_,_S_,_D_,_MINUS_,_t_,_i_,_m_,_e_,_o_,_u_,_t_,_END_,
    /*ªiÄõ*/_C_,_z_,_a_,_s_,___,_z_,_a_,_k_,_o_,___,_c_,_z_,_e_,_n_,_i_,_a_,___,_O_,_S_,_D_,_END_,
0x8A,0x90,0x9A,0x8F,0x90,0x8B,0x8A,_END_,
#endif
};

UINT8 code tStr_Osd_Setting[] =
{
    0x21,0x22,0x27,0x14,0x00,0x4D,0x3E,0x4E,0x4E,0x42,0x48,0x40,_END_,
    0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x00,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x21,0x22,0x27,0x14,0x00,0x4D,0x3E,0x4E,0x4E,0x42,0x48,0x40,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    0x21,0x22,0x27,0x14,0x01,0xa0,0xa1,_END_,
    0x21,0x22,0x27,0x14,0x00,0x4D,0x3E,0x4E,0x4E,0x42,0x48,0x40,_END_,
    0x21,0x22,0x27,0x14,0xA1,0xA2,0xA3,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    0x21,0x22,0x27,0x14,0x00,0x8b,0x87,0x8a,0x90,0x83,0x85,0x84,0x9b,0x90,_END_,
#endif
};

UINT8 code tOsdOnStr[] =
{
    0x21,0x22,0x48,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x10,0x11,0x45,0x45,0x4F,0x46,0x47,0x3E,_END_,
    0x10,0x11,0x4F,0x4D,_END_,
    0x15,0x48,0x3C,0x3E,_END_,
    0x10,0x11,0x3C,0x3C,0x3E,0x4D,0x49,_END_,
    /*²üÄõ*/0xC6,0x83,_END_,
    /*¸²¤ú*/_A_,_c_,_t_,_i_,_v_,_a_,_r_,_END_,
    /*®¿«Â*/0xAd,0xAe,0xAF,_END_,
    /*ªâÄõ*/_P_,0x91,0x91,_l_,_l_,0x91,_END_,
    /*¤¦³Á*/_T_,0xb6,_n_,_d_,_t_,_END_,
    /*·ç¨å*/_P_,0x99,_END_,
    /*ªiÄõ*/_W_,_l_,_END_,
0x84,_END_,
#endif
};


UINT8 code tOsdOffStr[] =
{
    0x21,0x22,0x3F,0x3F,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x15,0x4E,0x3E,0x42,0x48,0x4E,_END_,
    0x15,0x42,0x48,_END_,
    0x10,0x11,0x3A,0x40,_END_,
    0x21,0x22,0x3F,0x3F,_END_,
    /*²üÄõ*/0xC7,0x83,_END_,
    /*¸²¤ú*/_D_,_e_,_s_,_A_,_c_,_t_,_i_,_v_,_a_,_r_,_END_,
    /*®¿«Â*/0xb0,0xb1,0xb2,_END_,
    /*ªâÄõ*/_P_,_o_,_i_,_s_,___,_p_,0x91,0x91,_l_,_t_,0x91,_END_,
    /*¤¦³Á*/_S_,_l_,_u_,_k_,_k_,_e_,_t_,_END_,
    /*·ç¨å*/_A_,_v_,_END_,
    /*ªiÄõ*/_W_,_y_,_l_,_END_,
0x83,0x9E,0x93,0x85,0x87,0x85,_END_,
#endif
};

UINT8* code tOsdOnOffStrSet[] =
{
    tOsdOffStr, tOsdOnStr
};



#if defined(CONFIG_EUROPE_LANGUAGE)
// Language
UINT8 code tOsdEnglishStr[]=
{
    0x15,0x48,0x40,0x45,0x42,0x4D,0x41,_END_,
};
UINT8 code tOsdCHIStr[]=
{
    0x60,0x61,0x62,0x63,0x66,0x67,0x68,0x69,_END_,

};
UINT8 code tOsdFranceStr[]=
{
    0x16,0x4C,0x3A,0x48,0x3C,0x3A,0x42,0x4D,_END_,
};

UINT8 code tOsdGermanStr[]=
{
    0x14,0x3E,0x4F,0x4E,0x4D,0x3C,0x41,_END_,
};

UINT8 code tOsdSpainStr[]=
{
    //_E_,_s_,_p_,_a_,_n_,_o_,_l_,_END_,
    0x15,0x4D,0x4A,0x3A,0x0F,0x49,0x45,_END_,
};

UINT8 code tOsdItalianoStr[] =
{
    0x1A,0x4E,0x3A,0x45,0x42,0x3A,0x48,0x49,_END_,
};

UINT8 code tOsdkorenStr[] =  /*²üÄõ*/
{
    0x75,0x76,0x77,_END_,
};

UINT8 code tOsdPortugueseStr[] =  /*¸²¤ú*/
{
    0x23,0x49,0x4C,0x4E,0x4F,0x40,0x4F,0x3E,0x4D,0x3E,_END_,
};

UINT8 code tOsdJAPANStr[] =  /*®¿«Â*/
{
    0x9D,0x9E,0x9F,0xA0,_END_,
};

UINT8 code tOsdFinnishStr[] =  /*ªâÄõ*/
{
    _F_,_i_,_n_,_n_,_i_,_s_,_h_,_END_,
};

UINT8 code tOsdDanishStr[] =  /*¤¦³Á*/
{
    0x14,0x3A,0x48,0x42,0x4D,0x41,_END_,
};

UINT8 code tOsdSwedishStr[] =  /*·ç¨å*/
{
    0x27,0x51,0x52,0x3E,0x3D,0x42,0x4D,0x41,_END_,
};

UINT8 code tOsdPolishStr[] =  /*ªiÄõ*/
{
    0x23,0x49,0x45,0x42,0x4D,0x41,_END_,
};
UINT8 code tOsdRussianStr[] =  /*ªiÄõ*/
{
    0x72,0x73,0x74,0x74,_END_,
};

UINT8 code tOsdCzechStr[] =  /*±¶§J*/
{
    0x13,0x55,0x3E,0x3C,0x41,_END_,
};


UINT8* code tOsdLanguageStrSet[] =
{
    tOsdEnglishStr,
    tOsdCHIStr,
    tOsdFranceStr,
    tOsdGermanStr,
    tOsdSpainStr,
    tOsdItalianoStr,
    tOsdkorenStr,
    tOsdPortugueseStr,
    tOsdJAPANStr,
    tOsdFinnishStr,
    tOsdDanishStr,
    tOsdSwedishStr,
    tOsdPolishStr,
    tOsdRussianStr,
};
#endif

/* ===========================================================================
 * Sleep Timer item
 */
UINT8 code tOsdSleepStr[] =
{
    _S_,_l_,_e_,_e_,_p_,_END_,
};

UINT8 code tOsdSleepTimerStr[]=
{
    _S_,_l_,_e_,_e_,_p_,___,_T_,_i_,_m_,_e_,_r_,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _M_,_i_,_n_,_u_,_t_,_DOT_,_v_,_e_,_i_,_l_,_l_,_e_,_END_,
    _S_,_c_,_h_,_l_,_a_,_f_,_MINUS_,_t_,_i_,_m_,_e_,_r_,_END_,
    _A_,_p_,_a_,_g_,___,_a_,_u_,_t_,_o_,_m_,_END_,
    _T_,_i_,_m_,_e_,_r_,___,_d_,_i_,_s_,_a_,_t_,_t_,_DOT_,_END_,
    /*²üÄõ*/_S_,_l_,_a_,_a_,_p_,_t_,_i_,_m_,_e_,_r_,_END_,
    /*¸²¤ú*/_T_,_e_,_m_,_p_,_o_,_r_,_i_,_z_,_a_,_d_,_o_,_r_,___,_p_,_a_,_r_,_a_,___,_d_,_e_,_s_,_l_,_i_,_g_,_a_,_r_,_END_,
    /*®¿«Â*/_S_,_o_,_v_,_t_,_i_,_d_,_t_,_a_,_k_,_e_,_r_,_END_,
    /*ªâÄõ*/_U_,_n_,_i_,_a_,_j_,_a_,_s_,_t_,_i_,_n_,_END_,
    /*¤¦³Á*/_P_,_a_,_u_,_s_,_e_,_t_,_i_,_m_,_e_,_r_,_END_,
    /*·ç¨å*/_I_,_n_,_s_,_o_,_m_,_n_,_i_,_n_,_g_,_s_,_t_,_i_,_m_,_e_,_r_,_END_,
    /*ªiÄõ*/_T_,_i_,_m_,_e_,_r_,___,_u_,0xb8,_p_,_i_,_e_,_n_,_i_,_a_,_END_,
#endif
};


/* ===========================================================================
 * Volume Bar
 */
UINT8 code tOsdVolumnStr[]=
{
    0x2A,0x49,0x45,0x4F,0x46,0x47,0x3E,_END_,// 0:English
    0x90,0x91,0x8A,0x8B,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x2A,0x49,0x45,0x4F,0x46,0x47,0x3E,_END_,// 0:English
    0x1D,0x3A,0x4F,0x4E,0x4D,0x4E,0x59,0x13,0x4C,0x44,0x3E,_END_,// 9:German
    0x2A,0x49,0x45,0x4F,0x46,0x47,0x3E,0x48,_END_,// 4:Spanish
    0x2A,0x49,0x45,0x4F,0x46,0x47,0x3E,_END_,// 2:Italian
    /*²üÄõ*/0xAF,0xB0,_END_,
    /*¸²¤ú*/_THE_SAME_,
    /*®¿«Â*/0x87,0x88,0x89,_END_,
    /*ªâÄõ*/0x90,0x91,_n_,_e_,_n_,_v_,_o_,_i_,_m_,_a_,_k_,_k_,_u_,_u_,_s_,_END_,
    /*¤¦³Á*/_L_,_y_,_d_,_s_,_t_,_y_,_r_,_k_,_e_,_END_,
    /*·ç¨å*/_V_,_o_,_l_,_y_,_m_,_END_,
    /*ªiÄõ*/_G_,_l_,_o_,0xb8,_n_,_o_,0xb8,0xb9,_END_,
#endif
};


#define _OSD_MESSAGE_STR_
/* ===========================================================================
 * Inform message
 */
UINT8 code tOsdNoCableStr[] =
{
    0x20,0x49,0x01,0x3C,0x3A,0x3B,0x45,0x3E,0x00,_END_,// 0:English
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _A_,_u_,_c_,_u_,_n_,___,_c_,_a_,_b_,_l_,_e_,_END_,// 1:French
    _K_,_e_,_i_,_n_,___,_K_,_a_,_b_,_l_,_e_,_END_,// 9:German
    _S_,_i_,_n_,___,_c_,_a_,_b_,_l_,_e_,_END_,// 4:Spanish
    _N_,_e_,_s_,_s_,_u_,_n_,___,_c_,_a_,_v_,_o_,_END_,// 2:Italian
    _N_,_o_,___,_c_,_a_,_b_,_l_,_e_,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};

UINT8 code tOsdNoTTStr[] =
{
    0x20,0x49,0x01,0x28,0x2D,0x28,0x01,0x4D,0x42,0x40,0x48,0x3A,0x45,_END_,// 0:English
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};

UINT8 code tOsdNoSignalStr[] =
{
    0x20,0x49,0x01,0x4D,0x42,0x40,0x48,0x3A,0x45,_END_,// 0:English
    0x20,0x49,0x01,0x4D,0x42,0x40,0x48,0x3A,0x45,_END_,//0x8C,0x8D,0x8E,0x8F,0x86,0x87,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    0xA5,0xA6,0xA7,0xA8,0xA9,0xaa,_END_,
    0x20,0x49,0x01,0x4D,0x42,0x40,0x48,0x3A,0x45,_END_,// 0:English
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
0x96,0x9E,0x8E,0x89,0x8F,0x91,0x83,0x90,0x80,0x90,_END_,
#endif
};

UINT8 code tOsdNoSupportStr[] =
{
    0x21,0x22,0x4F,0x4E,0x01,0x49,0x3F,0x01,0x4C,0x3A,0x48,0x40,0x3E,_END_,// 0:English
    0x80,0x81,0x92,0x93,0x82,0x83,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x19,0x49,0x4C,0x4D,0x01,0x1D,0x42,0x46,0x47,0x42,0x4E,0x3E,_END_,// 1:French
    0x29,0x48,0x3E,0x4C,0x4C,0x3E,0x42,0x3C,0x41,0x3B,0x3A,0x4C,_END_,// 9:German
    0x16,0x4F,0x3E,0x4C,0x3A,0x01,0x14,0x3D,0x3A,0x45,0x3C,0x3A,0x48,0x3C,0x3E,_END_,// 4:Spanish
    0x16,0x4F,0x49,0x4C,0x42,0x01,0x26,0x3A,0x48,0x40,0x49,_END_,// 2:Italian
    /*²üÄõ*/0xD2,0xD3,0xD4,0xD5,_END_,
    0x21,0x22,0x4F,0x4E,0x01,0x49,0x3F,0x01,0x4C,0x3A,0x48,0x40,0x3E,_END_,// 0:English
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
0x89,0x84,0x9E,0x87,0x9F,0x86,0x87,0x9E,0x93,0x9E,0x80,0x90,_END_,
#endif
};


#define _SOURCE_MENU_STR_
/* ===========================================================================
 * Source menu items
 */
UINT8 code tStr_source[] =
{
    0x27,0x49,0x4F,0x4C,0x3C,0x3E,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _S_,_o_,_u_,_r_,_c_,_e_,_END_,
    _Q_,_u_,_e_,_l_,_l_,_e_,_END_,
    _F_,_u_,_e_,_n_,_t_,_e_,_END_,
    _O_,_r_,_i_,_g_,_i_,_n_,_e_,_END_,
    /*²üÄõ*/_B_,_r_,_o_,_n_,_END_,
    /*¸²¤ú*/_O_,_r_,_i_,_g_,_e_,_m_,_END_,
    /*®¿«Â*/_K_,_i_,_l_,_d_,_e_,_END_,
    /*ªâÄõ*/_L_,0x91,_h_,_d_,_e_,_END_,
    /*¤¦³Á*/_K_,_i_,_l_,_d_,_e_,_END_,
    /*·ç¨å*/_K_,0x91,_l_,_l_,_a_,_END_,
    /*ªiÄõ*/0xbd,_r_,0x3f,_d_,_l_,_o_,_END_,
    _S_,_o_,_u_,_r_,_c_,_e_,_END_,
#endif
};


UINT8 code tStrSignal_VGA[] =
{
    //_D_,0x0d,_S_,_u_,_b_,_END_,
    0x2A,0x17,0x10,0x11,_END_
};

UINT8 code tStrSignal_DVI[] =
{
    0x14,0x2A,0x1A,_END_
};

UINT8 code tStrSignal_HDMI_0[] =
{
    //0x19,0x14,0x1E,0x1F,0x1A,_END_,
    _H_,_D_,_M_,_I_,_END_,
};



UINT8 code tStrSignal_CVBS_0[] =
{
    //_C_,_V_,_B_,_S_,_END_,
    0x10,0x11,0x2A,/*0x31,*/_END_,
};
UINT8 code tStrSignal_CVBS_1[] =
{
    _D_,_T_,_V_,_END_,
};
UINT8 code tStrSignal_CVBS_2[] =
{
    0x10,0x11,0x2A,0x31,_END_,
};
UINT8 code tStrSignal_CVBS_3[] =
{
    0x10,0x11,0x2A,0x32,_END_,
};
UINT8 code tStrSignal_SV_0[] =
{
    0x27,0x2A,_END_,
};
UINT8 code tStrSignal_SV_1[] =
{
    0x27,0x2A,0x32,_END_,
};
UINT8 code tStrSignal_TV[] =
{
    0x28,0x2A,_END_,
};

UINT8 code tStrSignal_YPbPr_0[] =
{
    //0x2E,0x23,0x3B,0x23,0x4C,_END_,
   _D_,_V_,_D_,_END_,
};
#if CONFIG_ENABLE_USB
UINT8 code tStrSignal_Usb_0[] =
{
   _U_,_S_,_B_,_END_,
};
#endif

UINT8 code tStrSignal_YPbPr_1[] =
{
    _Y_,_P_,_b_,_P_,_r_,___,_2_,_END_,
};

UINT8 code tStrSignal_AvSv[] =
{
    _A_,_v_,_SLASH_,_S_,_v_,_END_
};

UINT8 code tStrSignal_Scart_0[] =
{
    _S_,_C_,_A_,_R_,_T_,___,_1_,_END_,
};

UINT8 code tStrSignal_Scart_1[] =
{
    _S_,_C_,_A_,_R_,_T_,___,_2_,_END_,
};

UINT8 code tStrNoPort[] =
{
    _N_,_o_,___,_P_,_o_,_r_,_t_,_END_
};

UINT8* code tOsdSourceStrSet[] =
{
#if 0 // for Sparrow Demo PCB
    tOsdSignal_VGA_Str,
    tOsdSignal_HDMI_0_Str,
    tOsdSignal_HDMI_1_Str,
    tOsdSignal_TV_Str,
    tOsdSignal_CVBS_Str,
    tOsdSignal_SV_Str,
    tOsdSignal_YPbPr_0_Str,

    tOsdSignal_YPbPr_1_Str,
    tOsdSignal_Scart_0_Str,
    tOsdSignal_Scart_1_Str,
    tOsdSignal_AvSv_Str,
    tOsdSignal_DVI_Str,
#else
    tStrSignal_VGA,
    tStrSignal_HDMI_0,
    tStrSignal_CVBS_0,
    tStrSignal_CVBS_1,
    tStrSignal_SV_0,
    tStrSignal_YPbPr_0,
    tStrSignal_YPbPr_1,
    tStrSignal_TV,
#endif
};




// Color Temperature

UINT8 code tOsdColorTempWarmerStr[] =
{
    0x2B,0x2C,0x3A,0x4C,0x46,0x47,0x3E,0x4C,_END_,
    0x9E,0x9F,0x9C,0x9D,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x2B,0x2C,0x3A,0x4C,0x46,0x47,0x3E,0x4C,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    0x80,_END_,
    0x2B,0x2C,0x3A,0x4C,0x46,0x47,0x3E,0x4C,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};
UINT8 code tOsdColorTempWarmStr[] =
{
    0x2B,0x2C,0x3A,0x4C,0x46,0x47,_END_,
    0x9C,0x9D,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x2B,0x2C,0x3A,0x4C,0x46,0x47,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    0xd5,0xd6,0xc9,0x9f,_END_,
    0x2B,0x2C,0x3A,0x4C,0x46,0x47,_END_,
    0xA4,0xA5,0xA6,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    0x8a,0x9e,0x86,0x80,0x8d,0x9a,_END_,
#endif
};
UINT8 code tOsdColorTempNormalStr[] =
{
    0x20,0x49,0x4C,0x46,0x47,0x3A,0x45, _END_,
    0x96,0x97,0x98,0x99,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x20,0x49,0x4C,0x46,0x47,0x3A,0x45, _END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    0xc3,0xb9,_END_,
    0x20,0x49,0x4C,0x46,0x47,0x3A,0x45, _END_,
    0xA7,0xA8,0xA9,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    0x83,0x85,0x87,0x92,0x90,0x80,0x8c,0x83,0x8d,0x9a, _END_,
#endif
};
UINT8 code tOsdColorTempCoolStr[] =
{
    0x13,0x49,0x49,0x45, _END_,
    0x9A,0x9B, _END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x13,0x49,0x49,0x45, _END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    0x8c,0xac,0x9f,_END_,
    0x13,0x49,0x49,0x45, _END_,
    0xAa,0xAb,0xAC,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    0x89,0x84,0x9e,0x88,0x85,_END_,
#endif
};
UINT8 code tOsdColorTempCoolerStr[] =
{
    0x13,0x49,0x49,0x45,0x3E,0x4C,_END_,
    0x9E,0x9F,0x9A,0x9B,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x13,0x49,0x49,0x45,0x3E,0x4C,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    0x84,_END_,
    0x13,0x49,0x49,0x45,0x3E,0x4C,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};

UINT8* code tColorTempStrSet[] =
{
    tOsdColorTempWarmerStr,
    tOsdColorTempWarmStr,
    tOsdColorTempNormalStr,
    tOsdColorTempCoolStr,
    tOsdColorTempCoolerStr
};


// Number Set
UINT8 code tOsdNumber0Str[] =
{
    _0_, _END_,
};

UINT8 code tOsdNumber1Str[] =
{
    _1_, _END_,
};

UINT8 code tOsdNumber2Str[] =
{
    _2_, _END_,
};

UINT8 code tOsdNumber3Str[] =
{
    _3_, _END_,
};

UINT8 code tOsdNumber4Str[] =
{
    _4_, _END_,
};

UINT8 code tOsdNumber5Str[] =
{
    _5_, _END_,
};

UINT8 code tOsdNumber6Str[] =
{
    _6_, _END_,
};

UINT8 code tOsdNumber7Str[] =
{
    _7_, _END_,
};

UINT8 code tOsdNumber8Str[] =
{
    _8_, _END_,
};

UINT8 code tOsdNumber9Str[] =
{
    _9_, _END_,
};

UINT8* code tOsdNumberStrSet[] =
{
    tOsdNumber0Str, tOsdNumber1Str, tOsdNumber2Str, tOsdNumber3Str,
    tOsdNumber4Str, tOsdNumber5Str, tOsdNumber6Str, tOsdNumber7Str,
    tOsdNumber8Str, tOsdNumber9Str,
};


/* ===========================================================================
 * Audio MTS
 */
UINT8 code tOSD_MTS_Mono[]=
{
    _M_,_o_,_n_,_o_,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    /*²üÄõ*/_THE_SAME_,
    /*¸²¤ú*/_THE_SAME_,
    /*®¿«Â*/_THE_SAME_,
    /*ªâÄõ*/_THE_SAME_,
    /*¤¦³Á*/_THE_SAME_,
    /*·ç¨å*/_THE_SAME_,
    /*ªiÄõ*/_THE_SAME_,
#endif
};


UINT8 code tOSD_MTS_Stereo[]=
{
    _S_,_t_,_e_,_r_,_e_,_o_,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _S_,_t_,0x5f,_r_,0x5f,_o_,_END_,
    _S_,_t_,_e_,_r_,_e_,_o_,_END_,
    _E_,_s_,_t_,_r_,_r_,_e_,_o_,_END_,
    _S_,_t_,_e_,_r_,_e_,_o_,_END_,
    /*²üÄõ*/_S_,_t_,_e_,_r_,_e_,_o_,_END_,
    /*¸²¤ú*/_E_,_s_,_t_,0x5f,_r_,_e_,_o_,_END_,
    /*®¿«Â*/_S_,_t_,_e_,_r_,_e_,_o_,_END_,
    /*ªâÄõ*/_S_,_t_,_e_,_r_,_e_,_o_,_END_,
    /*¤¦³Á*/_S_,_t_,_e_,_r_,_e_,_o_,_END_,
    /*·ç¨å*/_S_,_t_,_e_,_r_,_e_,_o_,_END_,
    /*ªiÄõ*/_S_,_t_,_e_,_r_,_e_,_o_,_END_,
#endif
};


UINT8 code tOSD_MTS_Sap[]=
{
    _S_,_A_,_P_,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    /*²üÄõ*/_THE_SAME_,
    /*¸²¤ú*/_THE_SAME_,
    /*®¿«Â*/_THE_SAME_,
    /*ªâÄõ*/_THE_SAME_,
    /*¤¦³Á*/_THE_SAME_,
    /*·ç¨å*/_THE_SAME_,
    /*ªiÄõ*/_THE_SAME_,
#endif
};


UINT8 code tOSD_MTS_Dual[]=
{
    _D_,_u_,_a_,_l_,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};


UINT8 code tOSD_MTS_Auto[]=
{
    _A_,_u_,_t_,_o_,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};


UINT8 code tOsdMTSDigitalStr[] =
{
    _D_,_i_,_g_,_i_,_t_,_a_,_l_,_END_,
};

UINT8 code tOsdMTSAnalogStr[] =
{
    _A_,_n_,_a_,_l_,_o_,_g_,_END_,
};

UINT8 code tOsdNicamStr[] =
{
    _N_,_I_,_C_,_A_,_M_,_END_,
};


UINT8 code tStr_MTS_Dual_1[] =
{
    _D_,_u_,_a_,_l_,___,_1_,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};


UINT8 code tStr_MTS_Dual_2[] =
{
    _D_,_u_,_a_,_l_,___,_2_,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};


UINT8* code tOsdMTSStrSet[] =
{
#if 1
    tOSD_MTS_Mono,
    tOSD_MTS_Stereo,
    tOSD_MTS_Dual,
    tStr_MTS_Dual_1,
    tStr_MTS_Dual_2,
    tOSD_MTS_Sap,
    tOSD_MTS_Auto
#else
    tOSD_MTS_Main,
    tOSD_MTS_Sub,
#endif
};


UINT8 code tOsdAdjustingStr[] =
{
    0x10,0x11,0x3D,0x43,0x4F,0x4D,0x4E,0x42,0x48,0x40,_END_,
    0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x10,0x11,0x3D,0x43,0x4F,0x4D,0x4E,0x42,0x48,0x40,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    0xb6,0xb7,0xb8,0xb9,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};

#if DISABLE_OPTTION

UINT8 code tOsdSleepNoneStr[]=
{
#if 0
    _N_,_o_,_n_,_e_,_END_,
#else
    _O_,_f_,_f_,_END_,
#endif
};

UINT8 code tOsdSleep30Str[]=
{
    0x00,_3_,_0_,_END_,
};

UINT8 code tOsdSleep60Str[]=
{
    0x00,_6_,_0_,_END_,
};

UINT8 code tOsdSleep90Str[]=
{
    0x00,_9_,_0_,_END_,
};

UINT8 code tOsdSleep120Str[]=
{
    _1_,_2_,_0_,_END_,
};

UINT8 code tOsdSleep150Str[]=
{
    _1_,_5_,_0_,_END_,
};

UINT8* code tOsdSleepOptionStrSet[6] =
{
    tOsdSleepNoneStr,
    tOsdSleep30Str,
    tOsdSleep60Str,
    tOsdSleep90Str,
    tOsdSleep120Str,
    tOsdSleep150Str
};

#endif
UINT8 code tStr_AUTO[] =
{
    0x10,0x11,0x29,0x28,0x21,0x22,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    0x9D,0x9E,0x9F,0xA0,_END_,
    0x10,0x11,0x29,0x28,0x21,0x22,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};

UINT8 code tStr_NTSC443[] =
{
    0x20,0x28,0x27,0x13,0x0D,0x34,0x34,0x33,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};

UINT8 code tStr_NTSC[] =
{
    0x20,0x28,0x27,0x13,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};

UINT8 code tStr_PAL[] =
{
    0x23,0x10,0x11,0x1D,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};

UINT8 code tStr_SECAM[] =
{
    0x27,0x15,0x13,0x10,0x11,0x1E,0x1F,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};

UINT8 code tStr_ColorSystem[] =
{
    0x13,0x49,0x45,0x49,0x4C,0x00,0x27,0x54,0x4D,0x4E,0x3E,0x46,0x47,_END_,
    0x9C,0x9D,0x9E,0x9F,0x92,0x93,0x86,0x87,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x13,0x49,0x45,0x49,0x4C,0x00,0x27,0x54,0x4D,0x4E,0x3E,0x46,0x47,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    0x89,0x90,_END_,
    0x13,0x49,0x45,0x49,0x4C,0x00,0x27,0x54,0x4D,0x4E,0x3E,0x46,0x47,_END_,
    0x84,0x85,0x86,0x80,0x81,0x82,0x83,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};



UINT8 code tStr_BG[] =
{
    0x12,0x17,0x18,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};

UINT8 code tStr_DK[] =
{
    0x14,0x1C,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};

UINT8 code tStr_I[] =
{
    0x1A,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};

UINT8 code tStr_L[] =
{
    0x1D, _END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};

UINT8 code tStr_LL[] =
{
    0x1D,  _COMMENT_, _END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};

UINT8 code tStr_M[] =
{
    0x1e,0x1f,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};

UINT8 code tStr_N[] =
{
    0x20,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};

//Tim 0408
//==============================================================================
//=============================
UINT8 code tStr_SoundSystem[] =
{
    0x27,0x49,0x4F,0x48,0x3D,0x00,0x27,0x54,0x4D,0x4E,0x3E,0x46,0x47,_END_,
    0x98,0x99,0x9A,0x9B,0x92,0x93,0x86,0x87,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x27,0x49,0x4F,0x48,0x3D,0x00,0x27,0x54,0x4D,0x4E,0x3E,0x46,0x47,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    0xa1,0xA2,_END_,
    0x27,0x49,0x4F,0x48,0x3D,0x00,0x27,0x54,0x4D,0x4E,0x3E,0x46,0x47,_END_,
    0x87,0x88,0x89,0x80,0x81,0x82,0x83,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};

UINT8 code tStr_FineTune[]=
{
    0x16,0x42,0x48,0x3E,0x00,0x28,0x4F,0x48,0x3E,_END_,
    0x90,0x91,0x80,0x81,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x26,0x58,0x3D,0x40,0x45,0x3A,0x40,0x3E,_END_,
    0x16,0x42,0x48,0x3E,0x00,0x28,0x4F,0x48,0x3E,_END_,
    0x16,0x42,0x48,0x3E,0x00,0x28,0x4F,0x48,0x3E,_END_,
    0x17,0x18,0x42,0x4C,0x3A,0x48,0x3D,0x49,_END_,
    0xB7,0xB8,0xB9,0xBA,_END_,
    0x16,0x42,0x48,0x3E,0x00,0x28,0x4F,0x48,0x3E,_END_,
    0x8A,0x8B,0x8C,0x8D,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    0x86,0x85,0x93,0x89,0x8A,0x87,0x85,0x9A,0x9B,0x90,_END_,
#endif
};





UINT8* code tOsdShowStatusStrSet[] =
{
    tOsdNoSignalStr,
    tOsdNoSupportStr,
    tOsdAdjustingStr,
    tOsdNoCableStr,
    tOsdAutoColorStr,
    tOsdNoTTStr,
};

#if 0
UINT8 code tOsdTtNoPageStr[] =
{
    _N_, _o_, ___, _p_, _a_, _g_, _e_, ___, _a_, _v_, _a_, _i_, _l_, _a_, _b_, _l_, _e_, _END_,
};

UINT8 code tOsdTtNoDataStr[] =
{
    _N_, _o_, ___, _T_, _e_, _l_, _e_, _t_, _e_, _x_, _t_, ___, _d_, _a_, _t_, _a_, ___, _a_, _v_, _a_, _i_, _l_, _a_, _b_, _l_, _e_, _END_,
};

UINT8* code tOsdTtEventStrSet[] =
{
    tOsdTtNoPageStr,
    tOsdTtNoDataStr,
};
#endif






/* ===========================================================================
 * for Factory mode menu
 */


#if 0
UINT8 code tOsdInitialFlashStr[] =
{
    _F_,_a_,_c_,_t_,_o_,_r_,_y_,___,_R_,_e_,_s_,_e_,_t_,_END_,
    _R_,_e_,_i_,_n_,_i_,_t_,_DOT_,_u_,_s_,_i_,_n_,_e_,_END_,
    _S_,_t_,_a_,_n_,_d_,_a_,_r_,_d_,_w_,_e_,_r_,_t_,_e_,_END_,
    _R_,_e_,_s_,_t_,_DOT_,_f_,_au_,_b_,_r_,_i_,_c_,_a_,_END_,
    _R_,_i_,_p_,_r_,_i_,_s_,_t_,_DOT_,_p_,_r_,_e_,_d_,_DOT_,_END_,
    /*²üÄõ*/_S_,_t_,_a_,_n_,_d_,_a_,_a_,_r_,_d_,_w_,_a_,_a_,_r_,_d_,_e_,_n_,_END_,
    /*¸²¤ú*/_P_,_r_,_e_,_d_,_e_,_f_,_i_,_n_,_i_,0x5b,0x88,_e_,_s_,___,_d_,_e_,___,_f_,0x0a,_b_,_r_,_i_,_c_,_a_,_END_,
    /*®¿«Â*/_F_,_a_,_b_,_r_,_i_,_k_,_k_,_t_,_i_,_l_,_b_,_a_,_k_,_e_,_s_,_t_,_i_,_l_,_l_,_i_,_n_,_g_,_END_,
    /*ªâÄõ*/_T_,_e_,_h_,_d_,_a_,_s_,_a_,_s_,_e_,_t_,_u_,_s_,_t_,_e_,_n_,___,_p_,_a_,_l_,_a_,_u_,_t_,_u_,_s_,_END_,
    /*¤¦³Á*/_T_,_i_,_l_,_b_,0x0e,_t_,_i_,_l_,_l_,___,_f_,_a_,_b_,_r_,_i_,_k_,_s_,_i_,_n_,_d_,_s_,_t_,_i_,_l_,_l_,_END_,
    /*·ç¨å*/_F_,_a_,_b_,_r_,_i_,_k_,_s_,0x99,_t_,_e_,_r_,_s_,_t_,0x91,_l_,_l_,_n_,_i_,_n_,_g_,_END_,
    /*ªiÄõ*/_R_,_e_,_s_,_e_,_t_,_o_,_w_,_a_,_n_,_i_,_e_,/*___,_d_,_o_,___,_u_,_s_,_t_,_a_,_w_,_i_,_e_,___,_f_,_a_,_b_,_r_,_y_,_c_,_z_,_n_,_y_,_c_,_h_,*/_END_,
    /*±¶§J*/_V_,0xba,_r_,_o_,_b_,_n_,0x3e,___,_n_,_a_,_s_,_t_,_a_,_v_,_e_,_n_,0x3e,_END_,
};
#endif

UINT8 code tStr_svn[] =
{
    _SLASH_,___,_s_,_v_,_n_,___,_END_,
};

UINT8 code tOsdFactoryModeStr[] =
{
    _F_,_a_,_c_,_t_,_o_,_r_,_y_,___,_M_,_o_,_d_,_e_,___,_END_,
};


UINT8 code tOsdAVStr[] =
{
    _A_,_V_,_END_
};

UINT8 code tOsdSVStr[] =
{
    _S_,_V_,_END_
};

UINT8 code tOsdRGBStr[] =
{
    _R_,_G_,_B_,_END_
};


/* ===========================================================================
 * Menu Title items
 */
#define _MENU_TITLE_STRINGS_
UINT8 code tStr_picture_title[] =
{
    _P_,_i_,_c_,_t_,_u_,_r_,_e_,_END_,
};
UINT8 code tStr_sound_title[] =
{
    _S_,_o_,_u_,_n_,_d_,_END_,
};
UINT8 code tStr_screen_title[] =
{
    _S_,_c_,_r_,_e_,_e_,_n_,_END_,
};
UINT8 code tStr_setup_title[] =
{
    _S_,_e_,_t_,_u_,_p_,_END_,
};
UINT8 code tStr_channel_title[] =
{
    _C_,_h_,_a_,_n_,_n_,_e_,_l_,_END_,
};

#if 0
UINT8 code tOsd_COLOR_Str[] =
{
    _C_,_O_,_L_,_O_,_R_,_END_,
    _C_,_O_,_U_,_L_,_E_,_U_,_R_,_S_,_END_,
    _F_,_A_,_R_,_B_,_E_,_END_,
    _C_,_O_,_L_,_O_,_R_,_END_,
    _C_,_O_,_L_,_O_,_R_,_E_,_END_,
    /*²üÄõ*/_K_,_L_,_E_,_U_,_R_,_END_,
    /*¸²¤ú*/_C_,_O_,_R_,_END_,
    /*®¿«Â*/_F_,_A_,_R_,_G_,_E_,_END_,
    /*ªâÄõ*/_V_,0x90,_R_,_I_,_END_,
    /*¤¦³Á*/_F_,_A_,_R_,_V_,_E_,_END_,
    /*·ç¨å*/_F_,0x91,_R_,_G_,_END_,
    /*ªiÄõ*/_K_,_O_,_L_,_O_,_R_,_END_,
    /*±¶§J*/_B_,_A_,_R_,_V_,_A_,_END_,
};
#endif

#if 0
UINT8 code tOsd_PC_Str[] =
{
    _P_,_C_,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    /*²üÄõ*/_THE_SAME_,
    /*¸²¤ú*/_THE_SAME_,
    /*®¿«Â*/_THE_SAME_,
    /*ªâÄõ*/_THE_SAME_,
    /*¤¦³Á*/_THE_SAME_,
    /*·ç¨å*/_THE_SAME_,
    /*ªiÄõ*/_K_,_o_,_m_,_p_,_u_,_t_,_e_,_r_,_END_,
    /*±¶§J*/_P_,_O_,0xb7,0x82,_T_,_A_,0xb7,_END_,
};
#endif

#if 0
UINT8 code tOsd_AUDIO_Str[] =
{
    _A_,_U_,_D_,_I_,_O_,_END_,
    _A_,_U_,_D_,_I_,_O_,_END_,
    _A_,_U_,_D_,_I_,_O_,_END_,
    _S_,_O_,_N_,_I_,_D_,_O_,_END_,
    _S_,_O_,_N_,_I_,_D_,_O_,_END_,
    /*²üÄõ*/_A_,_U_,_D_,_I_,_O_,_END_,
    /*¸²¤ú*/0x60,_U_,_D_,_I_,_O_,_END_,
    /*®¿«Â*/_L_,_Y_,_D_,_END_,
    /*ªâÄõ*/0x90,0x90,_N_,_I_,_END_,
    /*¤¦³Á*/_L_,_Y_,_D_,_END_,
    /*·ç¨å*/_L_,_J_,_U_,_D_,_END_,
    /*ªiÄõ*/_A_,_U_,_D_,_I_,_O_,_END_,
    /*±¶§J*/_Z_,_V_,_U_,_K_,_END_,
};
#endif

#if 0
UINT8 code tOsd_TV_Str[] =
{
    _T_,_V_,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _T_,_E_,_L_,_E_,_V_,_I_,_S_,_I_,_Ou_,_N_,_END_,
    _T_,_V_,_END_,
    /*²üÄõ*/_T_,_V_,_END_,
    /*¸²¤ú*/_T_,_V_,_END_,
    /*®¿«Â*/_T_,_V_,_END_,
    /*ªâÄõ*/_T_,_V_,_END_,
    /*¤¦³Á*/_T_,_V_,_END_,
    /*·ç¨å*/_T_,_V_,_END_,
    /*ªiÄõ*/_T_,_V_,_END_,
    /*±¶§J*/_T_,_V_,_END_,
};
#endif

#if 0
UINT8 code tOsd_OSD_Str[] =
{
    _O_,_S_,_D_,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    /*²üÄõ*/_THE_SAME_,
    /*¸²¤ú*/_THE_SAME_,
    /*®¿«Â*/_THE_SAME_,
    /*ªâÄõ*/_THE_SAME_,
    /*¤¦³Á*/_THE_SAME_,
    /*·ç¨å*/_THE_SAME_,
    /*ªiÄõ*/_THE_SAME_,
    /*±¶§J*/_THE_SAME_,
};
#endif

#if 0
UINT8* code tOsdMainMenuHintStrSet[] =
{
    tOsd_COLOR_Str,
    tOsd_PC_Str,
    tOsd_AUDIO_Str,
    tOsd_TV_Str,
    tOsd_OSD_Str
};
#endif

/* ===========================================================================
 * Display Ratio item
 */
UINT8 code tStr_display_ratio_auto[] =
{
    _A_,_u_,_t_,_o_,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    /*²üÄõ*/_THE_SAME_,
    /*¸²¤ú*/_THE_SAME_,
    /*®¿«Â*/_THE_SAME_,
    /*ªâÄõ*/0x8B,0x8C,0x8D,0x8E,_END_,
    /*¤¦³Á*/_THE_SAME_,
    /*·ç¨å*/_THE_SAME_,
    /*ªiÄõ*/_THE_SAME_,
    /*±¶§J*/_THE_SAME_,
    _THE_SAME_,
};
UINT8 code tStr_display_ratio_4x3[] =
{
    _4_,_COLON_,_3_,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    /*²üÄõ*/_THE_SAME_,
    /*¸²¤ú*/_THE_SAME_,
    /*®¿«Â*/_THE_SAME_,
    /*ªâÄõ*/_THE_SAME_,
    /*¤¦³Á*/_THE_SAME_,
    /*·ç¨å*/_THE_SAME_,
    /*ªiÄõ*/_THE_SAME_,
    /*±¶§J*/_THE_SAME_,
    _THE_SAME_,
};
UINT8 code tStr_display_ratio_16x9[] =
{
    _1_,_6_,_COLON_,_9_,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    /*²üÄõ*/_THE_SAME_,
    /*¸²¤ú*/_THE_SAME_,
    /*®¿«Â*/_THE_SAME_,
    /*®¿«Â*/_THE_SAME_,
    /*ªâÄõ*/_THE_SAME_,
    /*¤¦³Á*/_THE_SAME_,
    /*·ç¨å*/_THE_SAME_,
    /*ªiÄõ*/_THE_SAME_,
    /*±¶§J*/_THE_SAME_,
};
UINT8 code tStr_display_ratio_14x9[] =
{
    _1_,_4_,_COLON_,_9_,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    /*²üÄõ*/_THE_SAME_,
    /*¸²¤ú*/_THE_SAME_,
    /*®¿«Â*/_THE_SAME_,
    /*ªâÄõ*/_THE_SAME_,
    /*¤¦³Á*/_THE_SAME_,
    /*·ç¨å*/_THE_SAME_,
    /*ªiÄõ*/_THE_SAME_,
    /*±¶§J*/_THE_SAME_,
};

UINT8 code tStr_display_Dimmer_on[] =
{
    _D_,_I_,_M_,_M_,_E_,_R_,0x00,_O_,_N_,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    /*²üÄõ*/_THE_SAME_,
    /*¸²¤ú*/_THE_SAME_,
    /*®¿«Â*/_THE_SAME_,
    /*ªâÄõ*/_THE_SAME_,
    /*¤¦³Á*/_THE_SAME_,
    /*·ç¨å*/_THE_SAME_,
    /*ªiÄõ*/_THE_SAME_,
    /*±¶§J*/_THE_SAME_,
    _THE_SAME_,
};
UINT8 code tStr_display_Dimmer_off[] =
{
    _D_,_I_,_M_,_M_,_E_,_R_,0x00,_O_,_F_,_F_,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    /*²üÄõ*/_THE_SAME_,
    /*¸²¤ú*/_THE_SAME_,
    /*®¿«Â*/_THE_SAME_,
    /*®¿«Â*/_THE_SAME_,
    /*ªâÄõ*/_THE_SAME_,
    /*¤¦³Á*/_THE_SAME_,
    /*·ç¨å*/_THE_SAME_,
    /*ªiÄõ*/_THE_SAME_,
    /*±¶§J*/_THE_SAME_,
};

UINT8 code tStr_display_ratio_letterbox[] =
{
    _L_,_e_,_t_,_t_,_e_,_r_,___,_B_,_o_,_x_,_END_
};
UINT8 code tStr_display_ratio_panorama[] =
{
    _P_,_a_,_n_,_o_,_r_,_a_,_m_,_a_,_END_,
};

UINT8 code tOsdDisplayRatioStr[] =
{
    _D_,_i_,_s_,_p_,_l_,_a_,_y_,0x00,_R_,_a_,_t_,_i_,_o_,_END_,
    0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _D_,_i_,_s_,_p_,_l_,_a_,_y_,0x00,_R_,_a_,_t_,_i_,_o_,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    0xA9,0xAA,0xC0,0xA5,0x00,_END_,
    _D_,_i_,_s_,_p_,_l_,_a_,_y_,0x00,_R_,_a_,_t_,_i_,_o_,_END_,
    0xBD,0xBE,0xBF,0xB0,0xB1,0xB2,0xB3,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    0x86,0x85,0x9B,0x89,0x85,0x85,0x8A,0x83,_END_,
#endif
};

UINT8 code tStr_display_IR_OFF[] =
{
    _O_,_F_,_F_,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    /*²üÄõ*/_THE_SAME_,
    /*¸²¤ú*/_THE_SAME_,
    /*®¿«Â*/_THE_SAME_,
    /*ªâÄõ*/_THE_SAME_,//0x8B,0x8C,0x8D,0x8E,_END_,
    /*¤¦³Á*/_THE_SAME_,
    /*·ç¨å*/_THE_SAME_,
    /*ªiÄõ*/_THE_SAME_,
    /*±¶§J*/_THE_SAME_,
    _THE_SAME_,
};
UINT8 code tStr_display_IR_A[] =
{
    _A_,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    /*²üÄõ*/_THE_SAME_,
    /*¸²¤ú*/_THE_SAME_,
    /*®¿«Â*/_THE_SAME_,
    /*ªâÄõ*/_THE_SAME_,
    /*¤¦³Á*/_THE_SAME_,
    /*·ç¨å*/_THE_SAME_,
    /*ªiÄõ*/_THE_SAME_,
    /*±¶§J*/_THE_SAME_,
     _THE_SAME_,
};
UINT8 code tStr_display_IR_B[] =
{
    _B_,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    /*²üÄõ*/_THE_SAME_,
    /*¸²¤ú*/_THE_SAME_,
    /*®¿«Â*/_THE_SAME_,
    /*®¿«Â*/_THE_SAME_,
    /*ªâÄõ*/_THE_SAME_,
    /*¤¦³Á*/_THE_SAME_,
    /*·ç¨å*/_THE_SAME_,
    /*ªiÄõ*/_THE_SAME_,
    /*±¶§J*/_THE_SAME_,
};

UINT8 code tOsdDisplayIrStr[] =
{
    _I_,_R_,_END_,
	_I_,_R_,_END_,
    //0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _I_,_R_,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,// 0xA9,0xAA,0xC0,0xA5,0x00,_END_,
    _THE_SAME_,// _I_,_R_,_END_,
     _THE_SAME_,//0xBD,0xBE,0xBF,0xB0,0xB1,0xB2,0xB3,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,// 0x86,0x85,0x9B,0x89,0x85,0x85,0x8A,0x83,_END_,
#endif
};


#if CONFIG_ENABLE_FM
#if (!ENABLE_FM_FREQSET)	
	UINT8 code tStr_display_OFF[] =
	{
		_O_,_F_,_F_,_END_,
		_THE_SAME_,
		_THE_SAME_,
		_THE_SAME_,
		_THE_SAME_,
		/*²üÄõ*/_THE_SAME_,
		/*¸²¤ú*/_THE_SAME_,
		/*®¿«Â*/_THE_SAME_,
		/*ªâÄõ*/_THE_SAME_,
		/*¤¦³Á*/_THE_SAME_,
		/*·ç¨å*/_THE_SAME_,
		/*ªiÄõ*/_THE_SAME_,
		/*±¶§J*/_THE_SAME_,
		_THE_SAME_,
	};

	UINT8 code tStr_display_877[] =
	{
		_8_,_7_,_DOD_,_7_,_END_,
		_THE_SAME_,
		_THE_SAME_,
		_THE_SAME_,
		_THE_SAME_,
		/*²üÄõ*/_THE_SAME_,
		/*¸²¤ú*/_THE_SAME_,
		/*®¿«Â*/_THE_SAME_,
		/*ªâÄõ*/_THE_SAME_,
		/*¤¦³Á*/_THE_SAME_,
		/*·ç¨å*/_THE_SAME_,
		/*ªiÄõ*/_THE_SAME_,
		/*±¶§J*/_THE_SAME_,
		_THE_SAME_,
		
	};
	UINT8 code tStr_display_879[] =
	{
		_8_,_7_,_DOD_,_9_,_END_,
		_THE_SAME_,
		_THE_SAME_,
		_THE_SAME_,
		_THE_SAME_,
		/*²üÄõ*/_THE_SAME_,
		/*¸²¤ú*/_THE_SAME_,
		/*®¿«Â*/_THE_SAME_,
		/*ªâÄõ*/_THE_SAME_,
		/*¤¦³Á*/_THE_SAME_,
		/*·ç¨å*/_THE_SAME_,
		/*ªiÄõ*/_THE_SAME_,
		/*±¶§J*/_THE_SAME_,
		_THE_SAME_,
	};
	UINT8 code tStr_display_881[] =
	{
		_8_,_8_,_DOD_,_1_,_END_,
		_THE_SAME_,
		_THE_SAME_,
		_THE_SAME_,
		_THE_SAME_,
		/*²üÄõ*/_THE_SAME_,
		/*¸²¤ú*/_THE_SAME_,
		/*®¿«Â*/_THE_SAME_,
		/*®¿«Â*/_THE_SAME_,
		/*ªâÄõ*/_THE_SAME_,
		/*¤¦³Á*/_THE_SAME_,
		/*·ç¨å*/_THE_SAME_,
		/*ªiÄõ*/_THE_SAME_,
		/*±¶§J*/_THE_SAME_,
	};

	UINT8 code tStr_display_883[] =
	{
		_8_,_8_,_DOD_,_3_,_END_,
		_THE_SAME_,
		_THE_SAME_,
		_THE_SAME_,
		_THE_SAME_,
		/*²üÄõ*/_THE_SAME_,
		/*¸²¤ú*/_THE_SAME_,
		/*®¿«Â*/_THE_SAME_,
		/*ªâÄõ*/_THE_SAME_,
		/*¤¦³Á*/_THE_SAME_,
		/*·ç¨å*/_THE_SAME_,
		/*ªiÄõ*/_THE_SAME_,
		/*±¶§J*/_THE_SAME_,
		_THE_SAME_,
	};
	UINT8 code tStr_display_885[] =
	{
		_8_,_8_,_DOD_,_5_,_END_,
		_THE_SAME_,
		_THE_SAME_,
		_THE_SAME_,
		_THE_SAME_,
		/*²üÄõ*/_THE_SAME_,
		/*¸²¤ú*/_THE_SAME_,
		/*®¿«Â*/_THE_SAME_,
		/*®¿«Â*/_THE_SAME_,
		/*ªâÄõ*/_THE_SAME_,
		/*¤¦³Á*/_THE_SAME_,
		/*·ç¨å*/_THE_SAME_,
		/*ªiÄõ*/_THE_SAME_,
		/*±¶§J*/_THE_SAME_,
	};
	UINT8 code tStr_display_887[] =
	{
		_8_,_8_,_DOD_,_7_,_END_,
		_THE_SAME_,
		_THE_SAME_,
		_THE_SAME_,
		_THE_SAME_,
		/*²üÄõ*/_THE_SAME_,
		/*¸²¤ú*/_THE_SAME_,
		/*®¿«Â*/_THE_SAME_,
		/*ªâÄõ*/_THE_SAME_,
		/*¤¦³Á*/_THE_SAME_,
		/*·ç¨å*/_THE_SAME_,
		/*ªiÄõ*/_THE_SAME_,
		/*±¶§J*/_THE_SAME_,
		_THE_SAME_,
	};
	UINT8 code tStr_display_889[] =
	{
		_8_,_8_,_DOD_,_9_,_END_,
		_THE_SAME_,
		_THE_SAME_,
		_THE_SAME_,
		_THE_SAME_,
		/*²üÄõ*/_THE_SAME_,
		/*¸²¤ú*/_THE_SAME_,
		/*®¿«Â*/_THE_SAME_,
		/*ªâÄõ*/_THE_SAME_,
		/*¤¦³Á*/_THE_SAME_,
		/*·ç¨å*/_THE_SAME_,
		/*ªiÄõ*/_THE_SAME_,
		/*±¶§J*/_THE_SAME_,
		_THE_SAME_,
	};
	
	UINT8 code tStr_display_891[] =
	{
		_8_,_9_,_DOD_,_1_,_END_,
		_THE_SAME_,
		_THE_SAME_,
		_THE_SAME_,
		_THE_SAME_,
		/*²üÄõ*/_THE_SAME_,
		/*¸²¤ú*/_THE_SAME_,
		/*®¿«Â*/_THE_SAME_,
		/*®¿«Â*/_THE_SAME_,
		/*ªâÄõ*/_THE_SAME_,
		/*¤¦³Á*/_THE_SAME_,
		/*·ç¨å*/_THE_SAME_,
		/*ªiÄõ*/_THE_SAME_,
		/*±¶§J*/_THE_SAME_,
	};
#endif

	UINT8 code tOsdDisplayRadioStr[] =
	{
		_F_,_M_,0x01,_T_,_X_,_END_,
		_F_,_M_,0x01,_T_,_X_,_END_,
		//0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,_END_,
#if defined(CONFIG_EUROPE_LANGUAGE)
		_F_,_M_,0x01,_T_,_X_,_END_,
		_THE_SAME_,
		_THE_SAME_,
		_THE_SAME_,
		_THE_SAME_,// 0xA9,0xAA,0xC0,0xA5,0x00,_END_,
		_THE_SAME_,// _I_,_R_,_END_,
		 _THE_SAME_,//0xBD,0xBE,0xBF,0xB0,0xB1,0xB2,0xB3,_END_,
		_THE_SAME_,
		_THE_SAME_,
		_THE_SAME_,
		_THE_SAME_,
		_THE_SAME_,// 0x86,0x85,0x9B,0x89,0x85,0x85,0x8A,0x83,_END_,
#endif
	};
#endif

#if 0
/* ===========================================================================
 * Picture Mode menu items
 */
UINT8 code tOsdNormalStr[] =
{
    _N_,_o_,_r_,_m_,_a_,_l_,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _N_,_o_,_r_,_m_,_a_,_l_,_e_,_END_,
    /*²üÄõ*/_N_,_o_,_r_,_m_,_a_,_a_,_l_,_END_,
    /*¸²¤ú*/_N_,_o_,_r_,_m_,_a_,_l_,_END_,
    /*®¿«Â*/_N_,_o_,_r_,_m_,_a_,_l_,_END_,
    /*ªâÄõ*/_N_,_o_,_r_,_m_,_a_,_a_,_l_,_i_,_END_,
    /*¤¦³Á*/_N_,_o_,_r_,_m_,_a_,_l_,_END_,
    /*·ç¨å*/_N_,_o_,_r_,_m_,_a_,_l_,_END_,
    /*ªiÄõ*/_N_,_o_,_r_,_m_,_a_,_l_,_n_,_y_,_END_,
    /*±¶§J*/_N_,_o_,_r_,_m_,0x0a,_l_,_n_,0x3e,_END_,
};

UINT8 code tOsdClearStr[] =
{
    _C_,_l_,_e_,_a_,_r_,_END_,
    _E_,_s_,_p_,_a_,_c_,_e_,___,_l_,_i_,_b_,_r_,_e_,_END_,
    _F_,_r_,_e_,_i_,_e_,_r_,___,_R_,_a_,_u_,_m_,_END_,
    _C_,_l_,_a_,_r_,_o_,_END_,
    _R_,_a_,_d_,_u_,_r_,_a_,_END_,
    /*²üÄõ*/_H_,_e_,_l_,_d_,_e_,_r_,_END_,
    /*¸²¤ú*/_L_,_i_,_m_,_p_,_a_,_r_,_END_,
    /*®¿«Â*/_S_,_l_,_e_,_t_,_t_,_END_,
    /*ªâÄõ*/_K_,_i_,_r_,_k_,_a_,_s_,_END_,
    /*¤¦³Á*/_K_,_l_,_a_,_r_,_END_,
    /*·ç¨å*/_R_,_e_,_n_,_s_,_a_,_END_,
    /*ªiÄõ*/_U_,_s_,_u_,_END_,
    /*±¶§J*/_J_,_a_,_s_,_n_,0x5f,_END_,
};

UINT8 code tOsdDarkStr[] =
{
    _D_,_a_,_r_,_k_,_END_,
    _F_,_o_,_n_,_c_,0x5f,_END_,
    _D_,_u_,_n_,_k_,_e_,_l_,_END_,
    _O_,_s_,_c_,_u_,_r_,_o_,_END_,
    _S_,_c_,_u_,_r_,_o_,_END_,
    /*²üÄõ*/_D_,_o_,_n_,_k_,_e_,_r_,_END_,
    /*¸²¤ú*/_E_,_s_,_c_,_u_,_r_,_o_,_END_,
    /*®¿«Â*/_M_,0x9b,_r_,_k_,_END_,
    /*ªâÄõ*/_T_,_u_,_m_,_m_,_a_,_END_,
    /*¤¦³Á*/_M_,0x9b,_r_,_k_,_END_,
    /*·ç¨å*/_M_,0x93,_r_,_k_,_END_,
    /*ªiÄõ*/_C_,_i_,_e_,_m_,_n_,_y_,_END_,
    /*±¶§J*/_T_,_m_,_a_,_v_,0x5f,_END_,
};

UINT8 code tOsdTheaterStr[] =
{
    _T_,_h_,_e_,_a_,_t_,_e_,_r_,_END_,
    _T_,_h_,0x5f,0xab,_t_,_r_,_e_,_END_,
    _T_,_h_,_e_,_a_,_t_,_e_,_r_,_END_,
    _T_,_e_,_a_,_t_,_r_,_o_,_END_,
    _T_,_e_,_a_,_t_,_r_,_o_,_END_,
    /*²üÄõ*/_T_,_h_,_e_,_a_,_t_,_e_,_r_,_END_,
    /*¸²¤ú*/_C_,_i_,_n_,_e_,_m_,_a_,_END_,
    /*®¿«Â*/_T_,_h_,_e_,_a_,_t_,_e_,_r_,_END_,
    /*ªâÄõ*/_T_,_h_,_e_,_a_,_t_,_t_,_e_,_r_,_i_,_END_,
    /*¤¦³Á*/_T_,_h_,_e_,_a_,_t_,_e_,_r_,_END_,
    /*·ç¨å*/_T_,_e_,_a_,_t_,_e_,_r_,_END_,
    /*ªiÄõ*/_T_,_e_,_a_,_t_,_r_,_END_,
    /*±¶§J*/_D_,_i_,_v_,_a_,_d_,_l_,_o_,_END_,
};
#endif

#if 0
UINT8* code tOsdPictureStrSet[] =
{
    tOsdNormalStr,
    tOsdClearStr,
    tOsdDarkStr,
    tOsdTheaterStr
};
#endif

#if 0
UINT8 code tOsdNonPCMStr[] =
{
    _N_,_o_,_n_,___,_P_,_C_,_M_,_END_,
};
#endif

#if 0
UINT8 code tOsdMinStr[] =
{
    _M_,_i_,_n_,_END_,
};
#endif


/** ==========================================================================
 * Resolution strings
 */
UINT8 code tOsdResolution480iStr[] =
{
    _4_,_8_,_0_,_i_,_END_,
};
UINT8 code tOsdResolution576iStr[] =
{
    _5_,_7_,_6_,_i_,_END_,
};
UINT8 code tOsdResolution480pStr[] =
{
    _4_,_8_,_0_,_p_,_END_,
};
UINT8 code tOsdResolution576pStr[] =
{
    _5_,_7_,_6_,_p_,_END_,
};
UINT8 code tOsdResolution720p50Str[] =
{
    _7_,_2_,_0_,_p_,___,_5_,_0_,_H_,_z_,_END_,
};
UINT8 code tOsdResolution720p60Str[] =
{
    _7_,_2_,_0_,_p_,___,_6_,_0_,_H_,_z_,_END_,
};
UINT8 code tOsdResolution1080i50Str[] =
{
    _1_,_0_,_8_,_0_,_i_,___,_5_,_0_,_H_,_z_,_END_,
};
UINT8 code tOsdResolution1080i60Str[] =
{
    _1_,_0_,_8_,_0_,_i_,___,_6_,_0_,_H_,_z_,_END_,
};
UINT8 code tOsdResolution1080p50Str[] =
{
    _1_,_0_,_8_,_0_,_p_,___,_5_,_0_,_H_,_z_,_END_,
};
UINT8 code tOsdResolution1080p60Str[] =
{
    _1_,_0_,_8_,_0_,_p_,___,_6_,_0_,_H_,_z_,_END_,
};
UINT8 code tOsdResolution1080p24Str[] =
{
    _1_,_0_,_8_,_0_,_p_,___,_2_,_4_,_H_,_z_,_END_,
};
UINT8 code tOsdResolution1080p25Str[] =
{
    _1_,_0_,_8_,_0_,_p_,___,_2_,_5_,_H_,_z_,_END_,
};
UINT8 code tOsdResolution1080p30Str[] =
{
    _1_,_0_,_8_,_0_,_p_,___,_3_,_0_,_H_,_z_,_END_,
};

UINT8 code tOsdResolution240pStr[]=
{
    _2_,_4_,_0_,_p_,_END_
};
UINT8 code tOsdResolution288pStr[]=
{
    _2_,_8_,_8_,_p_,_END_
};
UINT8 code tOsdResolution720pStr[]=
{
    _7_,_2_,_0_,_p_,0x0,_END_
};
UINT8 code tOsdResolution1080iStr[]=
{
    _1_,_0_,_8_,_0_,_i_,_END_
};
UINT8 code tOsdResolution1080pStr[]=
{
    _1_,_0_,_8_,_0_,_p_,_END_
};

UINT8* code tOsdResolutionStrSet[] =
{
    tOsdResolution480iStr,
    tOsdResolution576iStr,
    tOsdResolution480pStr,
    tOsdResolution576pStr,
    tOsdResolution480pStr,		//1440x 480px50HZ (VG859)
    tOsdResolution576pStr,		//1440x 576px50HZ (VG859)
    tOsdResolution720p50Str,
    tOsdResolution720p60Str,
    tOsdResolution1080i50Str,
    tOsdResolution1080i60Str,
    tOsdResolution1080p50Str,
    tOsdResolution1080p60Str,
    tOsdResolution1080p50Str,	//1080pm50(960x1080)
    tOsdResolution1080p60Str,	//1080pm60(960x1080)
    tOsdResolution1080p24Str,
    tOsdResolution1080p25Str,
    tOsdResolution1080p30Str,
};
#if CONFIG_ENABLE_INFO
UINT8 code tOsdResolutionStr[]=
{
    _E_,_N_,_T_,_E_,_R_,0x5e,_E_,_n_,_t_,_e_,_r_,0x01,0x01,0x56,0x57,0x5e,0x10,_d_,_j_,_u_,_s_,_t_,0x01,0x01,0x5d,0x5e,_S_,_e_,_l_,_e_,_c_,_t_,0x01,0x01,_E_,_X_,_I_,_T_,0x5e,_E_,_x_,_i_,_t_,_END_
};

#endif
#if _VIDEO_VGA_SUPPORT
UINT8* code tOsdVGAResolutionStrSet[] =
{
    tOsdResolution480iStr,
    tOsdResolution576iStr,
    tOsdResolution480pStr,
    tOsdResolution576pStr,
    tOsdResolution720pStr,		//1440x 480px50HZ (VG859)
    tOsdResolution720p50Str,
    tOsdResolution720p60Str,
    tOsdResolution1080iStr,
    tOsdResolution1080iStr,
    tOsdResolution1080iStr,
    tOsdResolution1080pStr,
    tOsdResolution1080p50Str,
    tOsdResolution1080p60Str,	//1080pm50(960x1080)
    tOsdResolution1080p50Str,	//1080pm60(960x1080)
    tOsdResolution1080p60Str,
    tOsdResolution1080p24Str,
    tOsdResolution1080p30Str,
};
#endif

UINT8* code tOsdHdmiResolutionStrSet[] =
{
    tOsdResolution480iStr,
    tOsdResolution576iStr,
    tOsdResolution480pStr,
    tOsdResolution576pStr,
    tOsdResolution720pStr,		//1440x 480px50HZ (VG859)
    tOsdResolution720p50Str,
    tOsdResolution720p60Str,
    tOsdResolution1080iStr,
    tOsdResolution1080i50Str,
    tOsdResolution1080i60Str,
    tOsdResolution1080pStr,
    tOsdResolution1080p50Str,
    tOsdResolution1080p60Str,	//1080pm50(960x1080)
    tOsdResolution1080p50Str,	//1080pm60(960x1080)
    tOsdResolution1080p60Str,
    tOsdResolution1080p24Str,
    tOsdResolution1080p30Str,
};

UINT8 code tOsdFreq24Str[]=
{
    _2_,_4_,_H_,_z_,_END_
};
UINT8 code tOsdFreq25Str[]=
{
    _2_,_5_,_H_,_z_,_END_
};
UINT8 code tOsdFreq30Str[]=
{
    _3_,_0_,_H_,_z_,_END_
};

UINT8 code tOsdFreq50Str[]=
{
    _5_,_0_,_H_,_z_,_END_
};
UINT8 code tOsdFreq60Str[]=
{
    _6_,_0_,_H_,_z_,_END_
};
UINT8 code tOsdFreq100Str[]=
{
    _1_,_0_,_0_,_H_,_z_,_END_
};
UINT8 code tOsdFreq120Str[]=
{
    _1_,_2_,_0_,_H_,_z_,_END_
};




/**
 * UART strings
 */
UINT8 code tOsdUart9600Str[] =
{
    _9_, _6_,_0_,_0_,_END_,
};

UINT8 code tOsdUart115200Str[] =
{
    _1_, _1_,_5_,_2_,_0_,_0_,_END_,
};

UINT8* code tOsdUartStrSet[2] =
{
    tOsdOffStr,
    tStrSignal_VGA,
};


/**
 * PQ setting strings
 */
UINT8 code tOsdD_DLtiStr[] =
{
    _D_,_MINUS_,_D_,_L_,_t_,_i_,_END_,
};

UINT8 code tOsdD_DCtiStr[] =
{
    _D_,_MINUS_,_D_,_C_,_t_,_i_,_END_,
};

UINT8 code tOsdI_DLtiStr[] =
{
    _I_,_MINUS_,_D_,_L_,_t_,_i_,_END_,
};

UINT8 code tOsdI_DCtiStr[] =
{
    _I_,_MINUS_,_D_,_C_,_t_,_i_,_END_,
};

UINT8 code tOsdSUHCoefStr[] =
{
    _S_,_c_,_a_,_l_,_e_,_U_,_p_,0x00,_H_,_END_,
};

UINT8 code tOsdSUVCoefStr[] =
{
    _S_,_c_,_a_,_l_,_e_,_U_,_p_,0x00,_V_,_END_,
};

UINT8 code tOsdSUPKStr[] =
{
    _S_,_c_,_a_,_l_,_e_,0x00,_P_,_K_,_END_,
};

UINT8 code tOsdSUPK_MaskStr[] =
{
    _S_,_U_,_P_,_K_,0x00,_M_,_a_,_s_,_k_,_END_,
};

UINT8 code tOsdPeakingStr[] =
{
    _P_,_e_,_a_,_k_,_i_,_n_,_g_,_END_,
};

UINT8 code tOsdSUNR_YStr[] =
{
    _S_,_U_,_N_,_R_,___,_Y_,_END_,
};

UINT8 code tOsdSUNR_CStr[] =
{
    _S_,_U_,_N_,_R_,___,_C_,_END_,
};

UINT8 code tOsd_DigitalFilter_Str[] =
{
    _D_,_i_,_g_,_i_,_t_,_a_,_l_,0x00,_F_,_i_,_l_,_t_,_e_,_r_,_END_,
};

UINT8 code tOsd_Dither_Bit_Str[] =
{
    _D_,_i_,_t_,_h_,_e_,_r_,0x00,_B_,_i_,_t_,_END_,
};

UINT8 code tOsdSharpStr[] =
{
    _S_,_h_,_a_,_r_,_p_,_n_,_e_,_s_,_s_,_END_,
};

UINT8 code tOsdUartStr[] =
{
    _U_, _A_, _R_, _T_, 0x00, _P_, _a_, _t_, _h_, _END_,
};

UINT8 code tOsdYPbPrPhaseStr[] =
{
    _Y_,_P_,_b_,_P_,_r_,___,_P_,_h_,_a_,_s_,_e_,_END_,
};

UINT8 code tOsdSpreadSpectrumStr[] =
{
    _S_,_p_,_r_,_e_,_a_,_d_,0x00,_S_,_p_,_e_,_c_,_t_,_r_,_u_,_m_,_END_,
};

UINT8 code tOsdFacResetStr[] =
{
    _F_,_a_,_c_,_t_,_o_,_r_,_y_,0x00,_R_,_e_,_s_,_e_,_t_,_END_,
};

UINT8 code tOsdGammaStr[] =
{
    _G_,_a_,_m_,_m_,_a_,_END_,
};

UINT8 code tOsdSIndexStr[] =
{
    _S_,_I_,_n_,_d_,_e_,_x_,_END_,
};

UINT8 code tOsdSLowStr[] =
{
    _S_,_L_,_o_,_w_,_END_,
};

UINT8 code tOsdSHighStr[] =
{
    _S_,_H_,_i_,_g_,_h_,_END_,
};

UINT8 code tOsdDccStr[] =
{
    _D_, _C_, _C_, _END_,
};

UINT8 code tStr_XNR[] =
{
    _X_,_N_,_R_,_END_,
};

UINT8 code tStr_Demo[] =
{
    _D_,_e_,_m_,_o_,_END_,
};

/** ==========================================================================
 * Menu Hint strings
 */
UINT8 code tMenuHint_MainIcon[] =
{
    0x02, 0x01, _SPC_, 8, _S_,_e_,_l_,_e_,_c_,_t_, _SPC_, 24,
    0x03, 0x04, 0x05, _SPC_, 8,	_E_,_n_,_t_,_e_,_r_, _SPC_, 24,
    0x07, _SPC_, 8, _E_,_x_,_i_,_t_,_END_,
};
UINT8 code tSourceMenuHint[] =
{
    0x02, 0x01, _SPC_, 8, _S_,_e_,_l_,_e_,_c_,_t_, _SPC_, 24,
    0x03, 0x04, 0x05, _SPC_, 8, _E_,_n_,_t_,_e_,_r_, _END_,
};
UINT8 code tOsdUpDownButtonStr[] =
{
    0x02, 0x01, _END_,
};
UINT8 code tOsdLeftRightButtonStr[] =
{
    0x06, 0x05, _END_,
};
UINT8 code tOsdOkButtonStr[] =
{
    0x03, 0x04, _END_,
};
UINT8 code tOsdOkRightButtonStr[] =
{
    0x03, 0x04, 0x05, _END_,
};
UINT8 code tOsdMenuButtonStr[] =
{
    0x07, _END_,
};

UINT8 code tOsdSelectStr[] =
{
    _S_,_e_,_l_,_e_,_c_,_t_,_END_,
};

UINT8 code tOsdEnterStr[] =
{
    _E_,_n_,_t_,_e_,_r_,_END_,
};

UINT8 code tOsdAdjustStr[] =
{
    _A_,_d_,_j_,_u_,_s_,_t_,_END_,
};

UINT8 code tOsdConfirmStr[] =
{
    _C_,_o_,_n_,_f_,_i_,_r_,_m_,_END_,
};

UINT8 code tOsdReturnStr[] =
{
    _R_,_e_,_t_,_u_,_r_,_n_,_END_,
};

UINT8 code tOsdExitStr[] =
{
    _E_,_x_,_i_,_t_,_END_,
};




/**
 * Factory mode OSD strings.
 */
UINT8 code tStr_WB_R_Gain[] =
{
    _R_,0x00,_G_,_a_,_i_,_n_,_END_
};
UINT8 code tStr_WB_G_Gain[] =
{
    _G_,0x00,_G_,_a_,_i_,_n_,_END_
};
UINT8 code tStr_WB_B_Gain[] =
{
    _B_,0x00,_G_,_a_,_i_,_n_,_END_
};
UINT8 code tStr_WB_R_Offset[] =
{
    _R_,0x00,_O_,_f_,_f_,_s_,_e_,_t_,_END_
};
UINT8 code tStr_WB_G_Offset[] =
{
    _G_,0x00,_O_,_f_,_f_,_s_,_e_,_t_,_END_
};
UINT8 code tStr_WB_B_Offset[] =
{
    _B_,0x00,_O_,_f_,_f_,_s_,_e_,_t_,_END_
};
UINT8 code tStr_WBH_Bri[] =
{
    _W_,_B_,_H_,0x00,_B_,_r_,_i_,_END_
};
UINT8 code tStr_WBH_Cont[] =
{
    _W_,_B_,_H_,0x00,_C_,_o_,_n_,_t_,_END_
};
UINT8 code tStr_WBL_Bri[] =
{
    _W_,_B_,_L_,0x00,_B_,_r_,_i_,_END_
};
UINT8 code tStr_WBL_Cont[] =
{
    _W_,_B_,_L_,0x00,_C_,_o_,_n_,_t_,_END_
};
UINT8 code tStrWhiteBalance[] =
{
    _W_,_h_,_i_,_t_,_e_,0x00,_B_,_a_,_l_,_a_,_n_,_c_,_e_,_END_
};

UINT8 code tStr_ColorTemp[] =
{
    _C_,_o_,_l_,_o_,_r_,0x00,_T_,_e_,_m_,_p_,_END_
};
UINT8 code tStr_ColorTemp_Red[] =
{
    _R_,_e_,_d_,_END_
};
UINT8 code tStr_ColorTemp_Green[] =
{
    _G_,_r_,_e_,_e_,_n_,_END_
};
UINT8 code tStr_ColorTemp_Blue[] =
{
    _B_,_l_,_u_,_e_,_END_
};

UINT8 code tStr_ADC_Calib_HD720P[] =
{
    _H_,_D_,0x00,_7_,_2_,_0_,_p_,0x00,_6_,_0_,_H_,_z_,_END_
};
UINT8 code tStr_ADC_Calib_VGA[] =
{
    _V_,_G_,_A_,0x00,_1_,_0_,_2_,_4_,_CROSS_,_7_,_6_,_8_,_END_
};
UINT8 code tStr_ADC_Calib_1BY1[] =
{
    _C_,_o_,_m_,_p_,_DOT_,0x00,_1_,_b_,_y_,_1_,_END_
};
UINT8 code tStr_ADC_Calibration[] =
{
    _A_,_D_,_C_,0x00,_C_,_a_,_l_,_i_,_b_,_r_,_a_,_t_,_i_,_o_,_n_,_END_
};


UINT8 code tLogo_Disable[] =
{
    _D_,_i_,_s_,_a_,_b_,_l_,_e_,_END_
};
UINT8 code tLogo_RTK2668[] =
{
    _R_,_T_,_K_,0x00,_2_,_6_,_6_,_8_,_END_
};
UINT8 code tLogo_Customer[] =
{
    _C_,_u_,_s_,_t_,_o_,_m_,_e_,_r_,_END_
};
UINT8* code tDefaultLogo_StrSet[] =
{
    tLogo_Disable,
    tLogo_RTK2668,
    tLogo_Customer,
};

UINT8 code tStr_Default_Logo[] =
{
    _L_,_o_,_g_,_o_,_END_,
};
#if defined(CONFIG_FAC_DEFAULT_LANG_OPT)
UINT8 code tStr_Default_Language[] =
{
    _L_,_a_,_n_,_g_,_u_,_a_,_g_,_e_,_END_
};
#endif
UINT8 code tStr_Option[] =
{
    _O_,_p_,_t_,_i_,_o_,_n_,_END_
};

UINT8 code tStr_Clear[] =
{
    _C_,_l_,_e_,_a_,_r_,_END_
};
UINT8 code tStr_ClearAll[] =
{
    _C_,_l_,_e_,_a_,_r_,0x00,_A_,_l_,_l_,_END_
};
UINT8 code tStr_Factory_Reset[] =
{
    _F_,_a_,_c_,_t_,_o_,_r_,_y_,0x00,_R_,_e_,_s_,_e_,_t_,_END_
};

UINT8 code tStr_Version[] =
{
    _V_,_e_,_r_,_s_,_i_,_o_,_n_,_END_
};
UINT8 code tStr_FwVersion[] =
{
    _S_,_V_,_N_,_END_
};
UINT8 code tStr_Empty[] =
{
    0x00,_END_
};
UINT8 code tStr_Panel[] =
{
    _P_,_a_,_n_,_e_,_l_,_END_
};
UINT8 code tStr_PanelType[] =
{
    _A_,_U_,_1_,_7_,_END_
};
UINT8 code tStr_Falsh[] =
{
    _F_,_l_,_a_,_s_,_h_,_END_
};
UINT8 code tStr_FalshType[] =
{
    _W_,_2_,_5_,_X_,_8_,_0_,_END_
};

UINT8 code tStr_Brightness_0[] =
{
    _B_,_r_,_i_,_g_,_h_,_t_,_n_,_e_,_s_,_s_,_UNDERLINE_,_0_,_END_
};
UINT8 code tStr_Brightness_50[] =
{
    _B_,_r_,_i_,_g_,_h_,_t_,_n_,_e_,_s_,_s_,_UNDERLINE_,_5_,_0_,_END_
};
UINT8 code tStr_Brightness_100[] =
{
    _B_,_r_,_i_,_g_,_h_,_t_,_n_,_e_,_s_,_s_,_UNDERLINE_,_1_,_0_,_0_,_END_
};
UINT8 code tStr_Contrast_0[] =
{
    _C_,_o_,_n_,_t_,_r_,_a_,_s_,_t_,_UNDERLINE_,_0_,_END_
};
UINT8 code tStr_Contrast_50[] =
{
    _C_,_o_,_n_,_t_,_r_,_a_,_s_,_t_,_UNDERLINE_,_5_,_0_,_END_
};
UINT8 code tStr_Contrast_100[] =
{
    _C_,_o_,_n_,_t_,_r_,_a_,_s_,_t_,_UNDERLINE_,_1_,_0_,_0_,_END_
};
UINT8 code tStr_Saturation_0[] =
{
    _S_,_a_,_t_,_u_,_r_,_a_,_t_,_i_,_o_,_n_,_UNDERLINE_,_0_,_END_
};
UINT8 code tStr_Saturation_50[] =
{
    _S_,_a_,_t_,_u_,_r_,_a_,_t_,_i_,_o_,_n_,_UNDERLINE_,_5_,_0_,_END_
};
UINT8 code tStr_Saturation_100[] =
{
    _S_,_a_,_t_,_u_,_r_,_a_,_t_,_i_,_o_,_n_,_UNDERLINE_,_1_,_0_,_0_,_END_
};
UINT8 code tStr_PictureSetting[] =
{
    _P_,_i_,_c_,_t_,_u_,_r_,_e_,0x00,_S_,_e_,_t_,_t_,_i_,_n_,_g_,_END_
};

UINT8 code tStr_200Hz[] =
{
    _2_,_0_,_0_,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};
UINT8 code tStr_300Hz[] =
{
    _3_,_0_,_0_,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};
UINT8 code tStr_1KHz[] =
{
    _1_,_K_,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};
UINT8 code tStr_3KHz[] =
{
    _3_,0x1c,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};
UINT8 code tStr_8KHz[] =
{
    _8_,0x1c,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};
UINT8 code tStr_15KHz[] =
{
    _1_,_5_,0x1c,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};
UINT8 code tStr_Hz[] =
{
    0x19,0x55,_END_,
};
UINT8 code tStr_Volume_1[] =
{
    _V_,_o_,_l_,_u_,_m_,_e_,_UNDERLINE_,_1_,_END_,
};
UINT8 code tStr_Volume_25[] =
{
    _V_,_o_,_l_,_u_,_m_,_e_,_UNDERLINE_,_2_,_5_,_END_,
};
UINT8 code tStr_Volume_50[] =
{
    _V_,_o_,_l_,_u_,_m_,_e_,_UNDERLINE_,_5_,_0_,_END_,
};
UINT8 code tStr_Volume_75[] =
{
    _V_,_o_,_l_,_u_,_m_,_e_,_UNDERLINE_,_7_,_5_,_END_,
};
UINT8 code tStr_Volume_100[] =
{
    _V_,_o_,_l_,_u_,_m_,_e_,_UNDERLINE_,_1_,_0_,_0_,_END_,
};
UINT8 code tStr_TV_Volume_1[] =
{
    _T_,_V_,___,_V_,_o_,_l_,_u_,_m_,_e_,_UNDERLINE_,_1_,_END_,
};
UINT8 code tStr_TV_Volume_25[] =
{
    _T_,_V_,___,_V_,_o_,_l_,_u_,_m_,_e_,_UNDERLINE_,_2_,_5_,_END_,
};
UINT8 code tStr_TV_Volume_50[] =
{
    _T_,_V_,___,_V_,_o_,_l_,_u_,_m_,_e_,_UNDERLINE_,_5_,_0_,_END_,
};
UINT8 code tStr_TV_Volume_75[] =
{
    _T_,_V_,___,_V_,_o_,_l_,_u_,_m_,_e_,_UNDERLINE_,_7_,_5_,_END_
};
UINT8 code tStr_TV_Volume_100[] =
{
    _T_,_V_,___,_V_,_o_,_l_,_u_,_m_,_e_,_UNDERLINE_,_1_,_0_,_0_,_END_
};
UINT8 code tStr_VolumeSetting[] =
{
    _V_,_o_,_l_,_u_,_m_,_e_,___,_S_,_e_,_t_,_t_,_i_,_n_,_g_,_END_
};

UINT8 code tStr_Dclk_RANGE[] =
{
    _D_,_c_,_l_,_k_,___,_R_,_A_,_N_,_G_,_E_,_END_
};
UINT8 code tStr_Dclk_FMDIV[] =
{
    _D_,_c_,_l_,_k_,___,_F_,_M_,_D_,_I_,_V_,_END_
};
UINT8 code tStr_EMI[] =
{
    _E_,_M_,_I_,_END_
};

UINT8 code tStr_RF_StartDb[] =
{
    _R_,_F_,___,_A_,_G_,_C_,_UNDERLINE_,_G_,_a_,_i_,_n_,_END_
};

UINT8 code tStr_Setting[] =
{
    _S_,_E_,_T_,_T_,_I_,_N_,_G_,_END_
};
UINT8 code tStr_Design[] =
{
    _D_,_E_,_S_,_I_,_G_,_N_,_END_
};


UINT8 code tStr_Set_Skip[] =
{
    _S_,_e_,_t_,0x00,_S_,_k_,_i_,_p_,_END_,
};

UINT8 code tStr_Remove[] =
{
    _R_,_e_,_m_,_o_,_v_,_e_,_END_,
};

UINT8 code tStr_Copy[] =
{
    _C_,_o_,_p_,_y_,_END_,
};

UINT8 code tStr_Insert[] =
{
    _I_,_n_,_s_,_e_,_r_,_t_,_END_,
};

UINT8 code Icons_normale_av[3][3] =
{
    {0xA8   , _2BITICON_FS+ 0, 0x0D },
#if _DISABLE_ITEM_OPTION
    {0xA8   , _2BITICON_FS+ 30, 0x0D },
#endif
    {0xA8   , _2BITICON_FS+ 24, 0x0D },
};
UINT8 code Icons_Select_av[3][3] =
{
    {0xA8   , _2BITICON_FS+ 0, 0x89 },
	#if _DISABLE_ITEM_OPTION
    {0xA8   , _2BITICON_FS+ 30, 0x89 },
    #endif
    {0xA8   , _2BITICON_FS+ 24, 0x89 },
};

UINT8 code Icons_normale_vga[4][3] =
{
    {0xA8   , _2BITICON_FS+ 0, 0x0D },
    {0xA8   , _2BITICON_FS+ 30, 0x0D },
    {0xA8   , _2BITICON_FS+ 6, 0x0D },
    {0xA8   , _2BITICON_FS+ 24, 0x0D },
};
UINT8 code Icons_Select_vga[4][3] =
{
    {0xA8   , _2BITICON_FS+ 0, 0x89 },
    {0xA8   , _2BITICON_FS+ 30, 0x89 },
    {0xA8   , _2BITICON_FS+ 6, 0x89 },
    {0xA8   , _2BITICON_FS+ 24, 0x89 },
};

UINT8 code Icons_normale_hdmi[3][3] =
{
    {0xA8   , _2BITICON_FS+ 0, 0x0D },
	#if _DISABLE_ITEM_OPTION
    {0xA8   , _2BITICON_FS+ 30, 0x0D },
    #endif
    {0xA8   , _2BITICON_FS+ 24, 0x0D },
};
UINT8 code Icons_Select_hdmi[3][3] =
{
    {0xA8   , _2BITICON_FS+ 0, 0x89 },
	#if _DISABLE_ITEM_OPTION
    {0xA8   , _2BITICON_FS+ 30, 0x89 },
    #endif
    {0xA8   , _2BITICON_FS+ 24, 0x89 },
};


UINT8 code Icons_disable[5][3] =
{
    {0xAE   , _2BITICON_FS+ 0, 0x35 },
    {0xAE	, _2BITICON_FS+ 30, 0x35 },
    {0xAE   , _2BITICON_FS+ 6, 0x35 },
    {0xAE   , _2BITICON_FS+ 24, 0x35 },
    {0xAE   , _2BITICON_FS+ 12, 0x35 },
};
#ifdef CONFIG_VBI_CC_SUPPORT
UINT8 code tStr_Cctype[] =
{
    0x13,0x13,_END_,
    0x13,0x13,_END_,

#if defined(CONFIG_EUROPE_LANGUAGE)
    0x13,0x13,_END_,
    0x13,0x13,_END_,
    0x13,0x13,_END_,
    0x13,0x13,_END_,
    0x13,0x13,_END_,
    0x13,0x13,_END_,
    0x13,0x13,_END_,
    0x13,0x13,_END_,
    0x13,0x13,_END_,
    0x13,0x13,_END_,
    0x13,0x13,_END_,
#endif
};
#endif


UINT8 code tOsdCC1Str[] =
{
    0x13,0x13,_1_,_END_,
};

UINT8 code tOsdCC2Str[] =
{
    0x13,0x13,_2_,_END_,
};

UINT8 code tOsdCC3Str[] =
{
    0x13,0x13,_3_,_END_,
};

UINT8 code tOsdCC4Str[] =
{
    0x13,0x13,_4_,_END_,
};


UINT8 code tOsdTT1Str[] =
{
    0x28,0x28,_1_,_END_,
};

UINT8 code tOsdTT2Str[] =
{
    0x28,0x28,_2_,_END_,
};

UINT8 code tOsdTT3Str[] =
{
    0x28,0x28,_3_,_END_,
};

UINT8 code tOsdTT4Str[] =
{
    0x28,0x28,_4_,_END_,
};

UINT8* code tOsdCCSelStrSet[] =
{
    tOsdOffStr,
    tOsdCC1Str,
    tOsdCC2Str,
    tOsdCC3Str,
    tOsdCC4Str,
    tOsdTT1Str,
    tOsdTT2Str,
    tOsdTT3Str,
    tOsdTT4Str,
};
//Tim 0323
//==============================================================================
UINT8 code tStr_Ntsc_Type[] =
{
    0x28,0x2A,0x01,0x27,0x49,0x4F,0x4C,0x3C,0x3E,_END_,//0x20,0x28,0x27,0x13,0x00,0x28,0x54,0x4A,0x3E,_END_,
    0x28,0x2A,0x01,0x27,0x49,0x4F,0x4C,0x3C,0x3E,_END_,//
#if defined(CONFIG_EUROPE_LANGUAGE)
    0x28,0x2A,0x01,0x27,0x49,0x4F,0x4C,0x3C,0x3E,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    0x28,0x2A,0x01,0x27,0x49,0x4F,0x4C,0x3C,0x3E,_END_,
    0x28,0x2A,0x01,0x27,0x49,0x4F,0x4C,0x3C,0x3E,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};
UINT8 code tStr_Cable[] =
{
    0x13,0x3A,0x3B,0x45,0x3E,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};

UINT8 code tStr_AIR[] =
{
    0x10,0x11,0x42,0x4C,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};
UINT8 code tStr_NtscFree[] =
{
    _M_,_o_,_d_,_e_,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    0x8E,0x8F,0x90,0x91,_END_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif
};
UINT8 code tOsdOffTableStr[] =
{
    _F_,_r_,_e_,_e_,_END_,
    _THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
    _THE_SAME_,
#endif

};
UINT8 code tOsdOnTableStr[] =
{    
	_T_,_a_,_b_,_l_,_e_,_END_,
	_THE_SAME_,
#if defined(CONFIG_EUROPE_LANGUAGE)
	_THE_SAME_,
	_THE_SAME_,
	_THE_SAME_,
	_THE_SAME_,
	_THE_SAME_,
	_THE_SAME_,
	_THE_SAME_,
	_THE_SAME_,
	_THE_SAME_,
	_THE_SAME_,
	_THE_SAME_,
#endif
};

//==============================================================================


/*=================== Local Functions Phototype ==============*/

/*=========================== Functions ===================== */



/**
 *
 * @}
 */

