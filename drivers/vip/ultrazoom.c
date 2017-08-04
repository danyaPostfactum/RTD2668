/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2006
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is for scaler related functions.
 *
 * @author 	$Author$
 * @date		$Date$
 * @version	$Revision$
 * @ingroup	scaler
 */

/**
 * @addtogroup scaler
 * @{
 */

/*============================ Module dependency  ===========================*/
#include "vip\ultrazoom.h"
//#include "VIP_video_performance.h"
#include "vip\video_quality.h"
#include "vip\color.h"
#include "reg_def.h"
#include "rosprintf.h"
#include "vip\vip_parameter_def.h"
/*===================================  Types ================================*/

/*================================== Variables ==============================*/
#if 0 // move to ultrazoom_parameter.c
UINT16 code tSU_COEF_0[] =
{
    4,    3,    2,    0,   -2,   -6,  -10,  -14,
    -20,  -26,  -32,  -39,  -46,  -54,  -61,  -69,
    -76,  -83,  -91,  -98, -104, -109, -113, -116,
    -117, -117, -115, -111, -104,  -97,  -85,  -71,
    -54,  -32,   -9,   19,   49,   84,  120,  161,
    203,  250,  296,  347,  397,  451,  503,  558,
    611,  665,  718,  771,  820,  869,  913,  957,
    994, 1031, 1061, 1088, 1109, 1128, 1138, 1145
};

UINT16 code tSU_COEF_1[] =
{
    -7,   -7,   -8,  -11,  -13,  -16,  -20,  -24,
    -30,  -35,  -41,  -47,  -54,  -60,  -66,  -73,
    -79,  -85,  -91,  -95,  -99, -101, -102, -103,
    -101,  -97,  -92,  -84,  -74,  -62,  -47,  -30,
    -10,   14,   39,   69,   99,  133,  167,  206,
    246,  288,  330,  377,  422,  470,  516,  565,
    611,  659,  705,  751,  793,  836,  873,  911,
    943,  974,  999, 1022, 1040, 1055, 1064, 1071
};

UINT16  code tSU_COEF_2[] =
{
    -19,  -19,  -21,  -23,  -26,  -29,  -33,  -37,
    -41,  -46,  -52,  -56,  -61,  -66,  -71,  -75,
    -79,  -82,  -84,  -85,  -84,  -83,  -81,  -76,
    -70,  -62,  -52,  -41,  -28,  -11,    7,   18,
    37,   66,   92,  125,  155,  189,  222,  259,
    295,  334,  372,  411,  450,  491,  529,  570,
    608,  648,  683,  720,  753,  787,  817,  846,
    872,  897,  916,  936,  950,  964,  970,  988
};

UINT16  code tSU_COEF_3[] =
{
    -32,  -23,  -25,  -25,  -29,  -30,  -34,  -37,
    -41,  -45,  -49,  -52,  -55,  -58,  -60,  -61,
    -62,  -62,  -61,  -58,  -55,  -50,  -45,  -38,
    -29,  -19,   -7,    7,   21,   37,   55,   73,
    93,  116,  141,  170,  198,  229,  260,  292,
    324,  359,  390,  425,  458,  493,  525,  558,
    589,  621,  650,  679,  706,  733,  755,  779,
    798,  817,  832,  848,  858,  871,  876,  890
};

UINT16  code tSU_COEF_4[] =
{
    -42,  -31,  -32,  -31,  -32,  -32,  -35,  -36,
    -37,  -39,  -41,  -41,  -43,  -42,  -43,  -41,
    -40,  -37,  -35,  -30,  -25,  -18,  -11,   -3,
    7,   19,   30,   43,   59,   73,   90,  107,
    127,  148,  171,  197,  222,  249,  276,  306,
    334,  364,  392,  423,  452,  482,  510,  539,
    566,  594,  619,  645,  667,  691,  710,  730,
    747,  764,  777,  791,  799,  812,  817,  832
};

UINT16  code tSU_2tap_Linear[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    16, 48, 80, 112, 144, 176, 208, 240, 272, 304, 336, 368, 400, 432, 464, 496,
    528, 560, 592, 624, 656, 688, 720, 752, 784, 816, 848, 880, 912, 944, 976, 1008
};

UINT16  code tSU_1_64[] =
{
    // 1/64		The blurest
    36, 80, 73, 81, 77, 81, 79, 83, 82, 86, 88, 93, 95, 100, 103, 109,
    112, 117, 121, 127, 129, 134, 137, 141, 143, 146, 149, 150, 154, 155, 161, 167,
    194, 224, 241, 263, 274, 292, 300, 316, 322, 339, 343, 359, 363, 380, 384, 400,
    403, 420, 423, 439, 443, 459, 462, 479, 482, 499, 502, 523, 526, 555, 559, 627
};

UINT16  code tSU_1_60[] =
{
    // 1/60
    1, 47, 42, 53, 50, 55, 54, 58, 58, 62, 63, 69, 71, 76, 80, 85,
    89, 95, 100, 106, 111, 118, 122, 129, 134, 139, 145, 151, 158, 164, 172, 181,
    208, 236, 253, 275, 288, 308, 317, 335, 342, 360, 366, 382, 387, 403, 407, 423,
    427, 442, 445, 460, 462, 477, 480, 495, 497, 514, 516, 535, 538, 565, 569, 634
};

UINT16  code tSU_1_56[] =
{
    // 1/56
    -47, 0, -3, 10, 9, 16, 17, 23, 25, 31, 34, 41, 45, 52, 57, 64,
    71, 78, 85, 93, 100, 109, 117, 125, 134, 143, 152, 160, 170, 179, 190, 201,
    225, 251, 268, 290, 303, 322, 332, 351, 359, 377, 384, 401, 406, 422, 427, 443,
    446, 462, 465, 480, 482, 497, 499, 515, 516, 532, 534, 552, 554, 580, 583, 645
};

UINT16  code tSU_1_48[] =
{
    // 1/48
    -135, -89, -87, -73, -70, -60, -56, -47, -41, -32, -25, -15, -7, 3, 12, 24,
    33, 46, 55, 69, 80, 93, 105, 118, 131, 144, 157, 170, 185, 197, 213, 226,
    248, 269, 284, 305, 318, 338, 350, 370, 380, 400, 409, 428, 436, 455, 462, 480,
    487, 504, 511, 526, 531, 547, 551, 567, 570, 586, 589, 606, 607, 630, 631, 685
};

UINT16  code tSU_1_40[] =
{
    // 1/40
    -122, -90, -87, -76, -75, -69, -66, -60, -58, -52, -49, -43, -39, -33, -28, -21,
    -14, -6, 2, 11, 19, 31, 41, 53, 64, 77, 91, 105, 120, 135, 153, 168,
    186, 207, 224, 247, 266, 291, 310, 336, 355, 382, 402, 429, 448, 475, 494, 521,
    538, 564, 580, 604, 619, 640, 653, 674, 684, 703, 711, 728, 733, 752, 754, 792
};


UINT16  code *tSU_COEF_TABLE[] =
{
    tSU_COEF_0,     tSU_COEF_1,     tSU_COEF_2,     tSU_COEF_3,     tSU_COEF_4,
    tSU_2tap_Linear,    tSU_1_40,	tSU_1_48,	tSU_1_56,	tSU_1_60,	tSU_1_64
};

UINT8  code tsub_su_coef1[] =
{
    0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70,
    0x80, 0x90, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0,
};
UINT8  code tsub_su_coef2[] =
{
    0, 1, 5, 11, 19, 30, 43, 58, 75, 94, 114, 135, 158, 182, 206, 231
};

UINT8  code *tsub_SU_COEF_TABLE[] =
{
    tsub_su_coef1, tsub_su_coef2
};

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

UINT16 code  tScale_Down_Coef_2tap[] =
{
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    31,  95, 159, 223, 287, 351, 415, 479,
    543, 607, 671, 735, 799, 863, 927, 991,
};

UINT16  code tScale_Down_Coef_Blur[] =
{
    2,   3,   5,   7,  10,  15,  20,  28,  38,  49,  64,  81, 101, 124, 150, 178,
    209, 242, 277, 314, 351, 389, 426, 462, 496, 529, 556, 582, 602, 618, 629, 635,
};

UINT16 code  tScale_Down_Coef_Mid[] =
{
    -4,  -2,   0,   2,   5,   9,  15,  22,  32,  45,  60,  77,  98, 122, 149, 179,
    211, 245, 281, 318, 356, 394, 431, 468, 502, 533, 561, 586, 606, 621, 632, 638,
};

UINT16  code tScale_Down_Coef_Sharp[] =
{
    -2,   0,   1,   3,   6,  10,  15,  22,  32,  43,  58,  75,  95, 119, 145, 174,
    206, 240, 276, 314, 353, 391, 430, 467, 503, 536, 565, 590, 612, 628, 639, 646,
};

UINT16  code  *tScaleDown_COEF_TAB[] =
{
    tScale_Down_Coef_Blur,     tScale_Down_Coef_Mid,     tScale_Down_Coef_Sharp,     tScale_Down_Coef_2tap
};
#endif
extern UINT16 code tSU_COEF_0[];
extern UINT16 code tSU_COEF_1[];
extern UINT16  code tSU_COEF_2[];
extern UINT16  code tSU_COEF_3[];
extern UINT16  code tSU_COEF_4[];

extern UINT16  code tSU_2tap_Linear[];
extern UINT16  code tSU_1_64[];
extern UINT16  code tSU_1_60[];
extern UINT16  code tSU_1_56[];
extern UINT16  code tSU_1_48[];
extern UINT16  code tSU_1_40[];
extern UINT16  code tSU_cnst_table2[];
extern UINT16  code *tSU_COEF_TABLE[];
extern UINT8  code tsub_su_coef1[];
extern UINT8  code tsub_su_coef2[];
extern UINT8  code *tsub_SU_COEF_TABLE[];

extern UINT8 xdata vip_i;
extern UINT8 xdata vip_j;
extern UINT8 xdata vip_8bit_tmp[8];
extern UINT32 xdata vip_16bit_tmp[3];
extern UINT32 xdata vip_32bit_tmp[3];
//extern UINT16 xdata *vip_16bit_table_index;//*vip_table_index;
//extern UINT8 xdata *vip_8bit_table_index;
extern INT32 xdata vip_INT32_tmp[2];
extern INT16 xdata vip_INT16_tmp[5];

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

extern UINT16 code  tScale_Down_Coef_2tap[];
extern UINT16  code tScale_Down_Coef_Blur[];
extern UINT16 code  tScale_Down_Coef_Mid[];
extern UINT16  code tScale_Down_Coef_Sharp[];
extern INT16  code  *tScaleDown_COEF_TAB[];

/*================================ Definitions ==============================*/
/*================================== Function ===============================*/

/*============================================================================*/
/**
 * Scaler_SetScaleUp
 * This function is used to set scaling-up registers, including main and sub display.
 * It contains several parts:
 * 1. video compensation (scale control register)
 * 2. horizontal scaling factor
 * 3. vertical scaling factor
 * 4. horizontal scaling factor delta1 & delta2
 * 5. scaling factor segment1, segment2 and segment3
 *
 * @param <info> { display-info struecture }
 * @return { none }
 * @note
 * It can be linear or non-linear either, based on the following condition:
 * Non-linear scaling up, if it is main display and non-linear flag is active,
 * linear scaling up, otherwise
 *
 */
//void UltraZoom_SetScaleUp(UINT8 display)

static void drvif_color_ultrazoom_setscaleup(SIZE* ptInSize, SIZE* ptOutSize, UINT8 panorama)
{
    UINT32 nFactor, D1, D2,Hini,Vini;
    UINT16 S1,S2,S3;
    UINT8  r;

    S1 = 0;
    S2 = 0;
    S3 = ptOutSize->nWidth;
    D1 = 0;
    D2 = 0;

    if (ptOutSize->nWidth > ptInSize->nWidth)     // H scale-up, set h scaling factor
    {
        Hini = 0xc0;//0xff;
        rtdf_maskl(DM_UZU_INITIAL_VALUE_0x1624, ~(0x0000ff00), Hini<<8);//dm_uzu_Initial_Value_REG.hor_ini = Hini;
        if (ptInSize->nWidth > 1024)
            r = 15; // CSW+ 0970104 Restore to original setting
        else
            r = 8; // CSW+ 0961112 enlarge non-linear scaling result

        if (panorama)  	// CSW+ 0960830 Non-linear SU
        {
            if (ptInSize->nWidth > 1024)
                S1 = (ptOutSize->nWidth) / 6; // CSW+ 0970104 Restore to original setting
            else
                S1 = (ptOutSize->nWidth) / 5; // CSW+ 0961112 enlarge non-linear scaling result
            S2 = S1;
            S3 = (ptOutSize->nWidth) - 2*S1 - 2*S2;
            //=vv==old from TONY, if no problem, use this=vv==
            nFactor = (((UINT32)(ptInSize->nWidth)<< 21) - ((UINT32)Hini<<13)) / ((7+4*r)*S1 + (3+r)*S3 - r);
            //==vv==Thur debugged==vv==
            //nFactor = ((((UINT64)(ptInSize->nWidth))*(ptOutSize->nWidth-1))<<21) / ptOutSize->nWidth;
            //nFactor = nFactor / ((7+4*r)*S1 + (3+r)*S3 - r);
            //=====================
            nFactor = nFactor * r;
            D1 = 2*nFactor / S1 / r;
            D2 = nFactor / S1 / r;
//			RTD_Log(LOGGER_INFO,"PANORAMA1 HScale up factor before ROUND = %x, In Wid=%x, Out Wid=%x \n", nFactor, ptInSize->nWidth, ptOutSize->nWidth);
            nFactor = (nFactor + 1)>>1; //rounding
            D1 = (D1+1)>>1; //rounding
            D2 = (D2+1)>>1; //rounding

            if ((nFactor + (3*nFactor/r)) >= 1048576)	 	// Can't do nonlinear SU
            {
//				RTD_Log(LOGGER_INFO,"PANORAMA1 Can't do nonlinear SU \n");
                //			nFactor =(((UINT32)(ptInSize->nWidth)<< 21) - ((UINT32)Hini<<13))  / (ptOutSize->nWidth-1);
                nFactor =(((UINT32)(ptInSize->nWidth)<< 21))  / (ptOutSize->nWidth);//Thur 20071227 modified
//				RTD_Log(LOGGER_INFO,"PANORAMA1 HScale up factor before ROUND = %x, In Wid=%x, Out Wid=%x \n", nFactor, ptInSize->nWidth, ptOutSize->nWidth);
                nFactor = (nFactor + 1)>>1; //rounding
                S1 = 0;
                S2 = 0;
                S3 = ptOutSize->nWidth;
                D1 = 0;
                D2 = 0;
            }
        }
        else
        {
//			nFactor = (((UINT32)(ptInSize->nWidth)<< 21) - ((UINT32)Hini<<13))  / (ptOutSize->nWidth-1);
            nFactor = (((UINT32)(ptInSize->nWidth)<< 21))  / (ptOutSize->nWidth);//Thur 20071228 modified
//			UltraZoom_Printf("HScale up factor before ROUND = %x, In Wid=%x, Out Wid=%x \n", nFactor, ptInSize->nWidth, ptOutSize->nWidth);
            nFactor = (nFactor + 1)>>1; //rounding
        }
//		UltraZoom_Printf("HScale up factor = %x\n", nFactor);
    }
    else
    {
        nFactor = 0xffffff;
    }
    rtdf_maskl(DM_UZU_SCALE_HOR_FACTOR_0x1608, ~(0x000fffff), nFactor);//dm_uzu_Scale_Hor_Factor_REG.regValue = nFactor;
    rtdf_maskl(DM_UZU_HOR_DELTA1_0x1610, ~(0x00001fff), D1);//dm_uzu_Hor_Delta1_REG.hor_delta1 = D1;
    rtdf_maskl(DM_UZU_HOR_DELTA2_0x1614, ~(0x00001fff), D2);//dm_uzu_Hor_Delta2_REG.hor_delta2 = D2;
    rtdf_maskl(DM_UZU_HOR_SEGMENT1_0x1618, ~(0x000007ff), S1);//dm_uzu_Hor_Segment1_REG.hor_segment1 = S1;
    rtdf_maskl(DM_UZU_HOR_SEGMENT2_0x161c,~(0x000007ff), S2);//dm_uzu_Hor_Segment2_REG.hor_segment2 = S2;
    rtdf_maskl(DM_UZU_HOR_SEGMENT3_0x1620,~(0x000007ff), S3);//_uzu_Hor_Segment3_REG.hor_segment3 = S3;

    if (ptOutSize->nLength > ptInSize->nLength)    // V scale-up, set v scaling factor
    {

        // CSW- 0961128 for frame sync V scale up initial position change
        //	Vini = 0x78;
        // CSW+ 0961128 for frame sync V scale up initial position change by tyan
        //Vini = ((ptInSize->nLength*(0xFF - 0x7F))/(ptOutSize->nLength*2))+(1*(0xFF - 0x7F)/2);
        Vini = (0x80 + (0x80*ptInSize->nLength)/ptOutSize->nLength)&0xff;//20080113 Thur modified for VGA border
//		UltraZoom_Printf("====> Vini:%x ptInSize->nLength:%x ptOutSize->nLength:%x\n",Vini, ptInSize->nLength,ptOutSize->nLength);

        //=======
        rtdf_maskl(DM_UZU_INITIAL_VALUE_0x1624, ~(0x000000ff), Vini);//dm_uzu_Initial_Value_REG.ver_ini = Vini;
        //nFactor = ((UINT32)((ptInSize->nLength) << 20) - (Vini<<12)) / ((ptOutSize->nLength)-1);
        nFactor = ptInSize->nLength;
        nFactor = nFactor <<21;
        nFactor = nFactor /ptOutSize->nLength;
        nFactor = (nFactor + 1)>>1;
        //nFactor = ((UINT32)((ptInSize->nLength) << 21)) / ((ptOutSize->nLength));
//		UltraZoom_Printf("VScale up factor before ROUND= %x, in Len=%x, Out Len=%x\n", nFactor, ptInSize->nLength, ptOutSize->nLength);
        //nFactor = (nFactor + 1)>>1; //rounding
//		UltraZoom_Printf("VScale up factor = %x\n", nFactor);
    }
    else
    {
        nFactor = 0xffffff;
    }
    rtdf_maskl(DM_UZU_SCALE_VER_FACTOR_0x160c, ~(0x000fffff), nFactor);//dm_uzu_Scale_Ver_Factor_REG.regValue = nFactor;

//	rtd_outl(SCALEUP_DM_UZU_SCALE_VER_FACTOR_ADDR, dm_uzu_Scale_Ver_Factor_REG.regValue);
//	rtd_outl(SCALEUP_DM_UZU_SCALE_HOR_FACTOR_ADDR, dm_uzu_Scale_Hor_Factor_REG.regValue);
//	rtd_outl(SCALEUP_DM_UZU_HOR_DELTA1_ADDR, dm_uzu_Hor_Delta1_REG.regValue);
//	rtd_outl(SCALEUP_DM_UZU_HOR_DELTA2_ADDR, dm_uzu_Hor_Delta2_REG.regValue);
//	rtd_outl(SCALEUP_DM_UZU_HOR_SEGMENT1_ADDR, dm_uzu_Hor_Segment1_REG.regValue);
//	rtd_outl(SCALEUP_DM_UZU_HOR_SEGMENT2_ADDR, dm_uzu_Hor_Segment2_REG.regValue);
//	rtd_outl(SCALEUP_DM_UZU_HOR_SEGMENT3_ADDR, dm_uzu_Hor_Segment3_REG.regValue);
//	rtd_outl(SCALEUP_DM_UZU_INITIAL_VALUE_ADDR, dm_uzu_Initial_Value_REG.regValue);

//	dm_uzu_Input_Size_RBUS	dm_uzu_Input_Size_REG;
//	dm_uzu_Input_Size_REG.regValue = rtd_inl(SCALEUP_DM_UZU_INPUT_SIZE_ADDR);
    // Horizontal SD enable , FIFO width equal to display width
    if (ptOutSize->nWidth < ptInSize->nWidth)
    {
        nFactor = (ptOutSize->nWidth&0x07ff);
    }
    else //Horizontal SD disable , FIFO width equal to input width
    {
        nFactor = (ptInSize->nWidth&0x07ff);
    }
    if (ptOutSize->nLength < ptInSize->nLength)
    {
        nFactor = nFactor << 16|(ptOutSize->nLength&0x07ff);
    }
    else
    {
        nFactor = nFactor << 16|(ptInSize->nLength&0x07ff);
    }
    rtdf_outl(DM_UZU_INPUT_SIZE_0x1628,nFactor);
    //rtdf_maskl(DM_UZU_INPUT_SIZE_0x1628, ~(0x07ff0000), nFactor);//dm_uzu_Input_Size_REG.hor_input_size = ptInSize->nWidth;
    //rtdf_maskl(DM_UZU_INPUT_SIZE_0x1628, ~(0x000007ff), ptInSize->nLength);//dm_uzu_Input_Size_REG.ver_input_size = ptInSize->nLength;
//	rtd_outl(SCALEUP_DM_UZU_INPUT_SIZE_ADDR, dm_uzu_Input_Size_REG.regValue);

#if 0
    if (display == _MAIN_DISPLAY)
    {
        dm_uzu_Scale_Hor_Factor_RBUS		dm_uzu_Scale_Hor_Factor_REG;
        dm_uzu_Scale_Ver_Factor_RBUS		dm_uzu_Scale_Ver_Factor_REG;
        dm_uzu_Initial_Value_RBUS			dm_uzu_Initial_Value_REG;
        dm_uzu_Hor_Delta1_RBUS			dm_uzu_Hor_Delta1_REG;
        dm_uzu_Hor_Delta2_RBUS			dm_uzu_Hor_Delta2_REG;
        dm_uzu_Hor_Segment1_RBUS		dm_uzu_Hor_Segment1_REG;
        dm_uzu_Hor_Segment2_RBUS		dm_uzu_Hor_Segment2_REG;
        dm_uzu_Hor_Segment3_RBUS		dm_uzu_Hor_Segment3_REG;

        S1 = 0;
        S2 = 0;
        S3 = ptOutSize->nWidth;
        D1 = 0;
        D2 = 0;

        if (ptOutSize->nWidth > ptInSize->nWidth)    // H scale-up, set h scaling factor
        {
            Hini = 0xff;
            dm_uzu_Initial_Value_REG.hor_ini = Hini;
            if (ptInSize->nWidth > 1024)
                r = 15; // CSW+ 0970104 Restore to original setting
            else
                r = 8; // CSW+ 0961112 enlarge non-linear scaling result

            if (panorama)  	// CSW+ 0960830 Non-linear SU
            {
                if (ptInSize->nWidth > 1024)
                    S1 = (ptOutSize->nWidth)  / 6; // CSW+ 0970104 Restore to original setting
                else
                    S1 = (ptOutSize->nWidth)  / 5; // CSW+ 0961112 enlarge non-linear scaling result
                S2 = S1;
                S3 = (ptOutSize->nWidth) - 2*S1 - 2*S2;
                //=vv==old from TONY, if no problem, use this=vv==
                nFactor = (((UINT32)(ptInSize->nWidth)<< 21) - ((UINT32)Hini<<13)) / ((7+4*r)*S1 + (3+r)*S3 - r);
                //==vv==Thur debugged==vv==
                //nFactor = ((((UINT64)(ptInSize->nWidth))*(ptOutSize->nWidth-1))<<21) / ptOutSize->nWidth;
                //nFactor = nFactor / ((7+4*r)*S1 + (3+r)*S3 - r);
                //=====================
                nFactor = nFactor * r;
                D1 = 2*nFactor / S1 / r;
                D2 = nFactor / S1 / r;
                RTD_Log(LOGGER_INFO,"PANORAMA1 HScale up factor before ROUND = %x, In Wid=%x, Out Wid=%x \n", nFactor, ptInSize->nWidth, ptOutSize->nWidth);
                nFactor = SHR((nFactor + 1), 1); //rounding
                D1 = SHR(D1+1,1); //rounding
                D2 = SHR(D2+1,1); //rounding

                if ((nFactor + (3*nFactor/r)) >= 1048576)	 	// Can't do nonlinear SU
                {
                    RTD_Log(LOGGER_INFO,"PANORAMA1 Can't do nonlinear SU \n");
                    //			nFactor =(((UINT32)(ptInSize->nWidth)<< 21) - ((UINT32)Hini<<13))  / (ptOutSize->nWidth-1);
                    nFactor =(((UINT32)(ptInSize->nWidth)<< 21))  / (ptOutSize->nWidth);//Thur 20071227 modified
                    RTD_Log(LOGGER_INFO,"PANORAMA1 HScale up factor before ROUND = %x, In Wid=%x, Out Wid=%x \n", nFactor, ptInSize->nWidth, ptOutSize->nWidth);
                    nFactor = SHR((nFactor + 1), 1); //rounding
                    S1 = 0;
                    S2 = 0;
                    S3 = ptOutSize->nWidth;
                    D1 = 0;
                    D2 = 0;
                }
            }
            else
            {
//				nFactor = (((UINT32)(ptInSize->nWidth)<< 21) - ((UINT32)Hini<<13))  / (ptOutSize->nWidth-1);
                nFactor = (((UINT32)(ptInSize->nWidth)<< 21))  / (ptOutSize->nWidth);//Thur 20071228 modified
                UltraZoom_Printf("HScale up factor before ROUND = %x, In Wid=%x, Out Wid=%x \n", nFactor, ptInSize->nWidth, ptOutSize->nWidth);
                nFactor = SHR((nFactor + 1), 1); //rounding
            }
            UltraZoom_Printf("HScale up factor = %x\n", nFactor);
        }
        else
        {
            nFactor = 0xffffff;
        }
        dm_uzu_Scale_Hor_Factor_REG.regValue = nFactor;
        dm_uzu_Hor_Delta1_REG.hor_delta1 = D1;
        dm_uzu_Hor_Delta2_REG.hor_delta2 = D2;
        dm_uzu_Hor_Segment1_REG.hor_segment1 = S1;
        dm_uzu_Hor_Segment2_REG.hor_segment2 = S2;
        dm_uzu_Hor_Segment3_REG.hor_segment3 = S3;

        if (ptOutSize->nLength > ptInSize->nLength)     // V scale-up, set v scaling factor
        {

            // CSW- 0961128 for frame sync V scale up initial position change
            //	Vini = 0x78;
            // CSW+ 0961128 for frame sync V scale up initial position change by tyan
            //Vini = ((ptInSize->nLength*(0xFF - 0x7F))/(ptOutSize->nLength*2))+(1*(0xFF - 0x7F)/2);
            Vini = (0x80 + (0x80*ptInSize->nLength)/ptOutSize->nLength)&0xff;//20080113 Thur modified for VGA border
            UltraZoom_Printf("====> Vini:%x ptInSize->nLength:%x ptOutSize->nLength:%x\n",Vini, ptInSize->nLength,ptOutSize->nLength);

            //=======
            dm_uzu_Initial_Value_REG.ver_ini = Vini;
            //nFactor = ((UINT32)((ptInSize->nLength) << 20) - (Vini<<12)) / ((ptOutSize->nLength)-1);
            nFactor = ((UINT32)((ptInSize->nLength) << 21)) / ((ptOutSize->nLength));
            UltraZoom_Printf("VScale up factor before ROUND= %x, in Len=%x, Out Len=%x\n", nFactor, ptInSize->nLength, ptOutSize->nLength);
            nFactor = SHR((nFactor + 1), 1); //rounding
            UltraZoom_Printf("VScale up factor = %x\n", nFactor);
        }
        else
        {
            nFactor = 0xffffff;
        }
        dm_uzu_Scale_Ver_Factor_REG.regValue = nFactor;

        rtd_outl(SCALEUP_DM_UZU_SCALE_VER_FACTOR_ADDR, dm_uzu_Scale_Ver_Factor_REG.regValue);
        rtd_outl(SCALEUP_DM_UZU_SCALE_HOR_FACTOR_ADDR, dm_uzu_Scale_Hor_Factor_REG.regValue);
        rtd_outl(SCALEUP_DM_UZU_HOR_DELTA1_ADDR, dm_uzu_Hor_Delta1_REG.regValue);
        rtd_outl(SCALEUP_DM_UZU_HOR_DELTA2_ADDR, dm_uzu_Hor_Delta2_REG.regValue);
        rtd_outl(SCALEUP_DM_UZU_HOR_SEGMENT1_ADDR, dm_uzu_Hor_Segment1_REG.regValue);
        rtd_outl(SCALEUP_DM_UZU_HOR_SEGMENT2_ADDR, dm_uzu_Hor_Segment2_REG.regValue);
        rtd_outl(SCALEUP_DM_UZU_HOR_SEGMENT3_ADDR, dm_uzu_Hor_Segment3_REG.regValue);
        rtd_outl(SCALEUP_DM_UZU_INITIAL_VALUE_ADDR, dm_uzu_Initial_Value_REG.regValue);

        dm_uzu_Input_Size_RBUS	dm_uzu_Input_Size_REG;
        dm_uzu_Input_Size_REG.regValue = rtd_inl(SCALEUP_DM_UZU_INPUT_SIZE_ADDR);
        dm_uzu_Input_Size_REG.hor_input_size = ptInSize->nWidth;
        dm_uzu_Input_Size_REG.ver_input_size = ptInSize->nLength;
        rtd_outl(SCALEUP_DM_UZU_INPUT_SIZE_ADDR, dm_uzu_Input_Size_REG.regValue);
    }
#endif



}

/*============================================================================*/
/**
 * CScalerSetScaleDown
 * This function is used to set scaling-down registers
 *
 * @param <info> { display-info struecture }
 * @return { none }
 * @note
 * It can be linear or non-linear either, based on the non-linear flag condition:
 */
//void Scaler_SetScaleDown(UINT8 display)
void drvif_color_ultrazoom_set_scale_down(SIZE* ptInSize, SIZE* ptOutSize)
{
//	rtdf_outb(Page_Select_0x009f, _PAGE0);
//	rtdf_pageSelect(_PAGE0);

// Set scale-down coefficient for RTD2472D
    if ((ptInSize->nWidth) > (ptOutSize->nWidth))  //if(ucOption & _BIT3)    // H scale-down
    {
        // Data[12~15]
        vip_32bit_tmp[0]=(UINT32)(1 * 1048576 * ptInSize->nWidth);//((DWORD *)pData)[3] = (DWORD)1 * 1048576 * stModeInfo.IHWidth;
        vip_32bit_tmp[0] = ((UINT32)(vip_32bit_tmp[0])%(ptOutSize->nWidth) )?( (vip_32bit_tmp[0]/(ptOutSize->nWidth))+1  ):(vip_32bit_tmp[0]/(ptOutSize->nWidth));//((DWORD *)pData)[3] = (((DWORD *)pData)[3] % stDisplayInfo.DHWidth) ? ((((DWORD *)pData)[3] / stDisplayInfo.DHWidth) + 1) : (((DWORD *)pData)[3] / stDisplayInfo.DHWidth);
        // H scale-down factor
        vip_8bit_tmp[4] = ( (vip_32bit_tmp[0]* 16 / 1048576 - 16) & 0x3f );//pData[4] = ((((DWORD *)pData)[3] * 16 / 1048576 - 16) & 0x3f);
        vip_8bit_tmp[5] = ((vip_32bit_tmp[0]>>16) & 0xff);//pData[5] = ((((DWORD *)pData)[3] >> 16) & 0xff);
        vip_8bit_tmp[6] = ((vip_32bit_tmp[0]>>8) & 0xff);//pData[6] = ((((DWORD *)pData)[3] >> 8) & 0xff);
        vip_8bit_tmp[7] =  (vip_32bit_tmp[0] & 0xff);//pData[7] = ((((DWORD *)pData)[3]) & 0xff);
    }
    else
    {
        vip_8bit_tmp[4] = 0x00;
        vip_8bit_tmp[5] = 0x10;
        vip_8bit_tmp[6] = 0x00;
        vip_8bit_tmp[7] = 0x00;
    }

    if (ptInSize->nLength> ptOutSize->nLength) //if(ucOption & _BIT1)    // V scale-down
    {

        // Data[12~15]
        (UINT32)vip_32bit_tmp[3]= (UINT32)2 * 131072 * ptInSize->nLength / ptOutSize->nLength;//((DWORD *)pData)[3] = (DWORD)2 * 131072 * stModeInfo.IVHeight / stDisplayInfo.DVHeight;
        (UINT32)vip_32bit_tmp[3] =( ((UINT32)vip_32bit_tmp[3]) >> 1) + (((UINT32)vip_32bit_tmp[3]) & 0x01) ;//((DWORD *)pData)[3] = (((DWORD *)pData)[3] >> 1) + (((DWORD *)pData)[3] & 0x01);

        // V scale-down factor
        vip_8bit_tmp[0] = (((UINT32)vip_32bit_tmp[3] * 16 / 131072 - 16) & 0x3f);//pData[0] = ((((DWORD *)pData)[3] * 16 / 131072 - 16) & 0x3f);
        vip_8bit_tmp[1] = (((UINT32)vip_32bit_tmp[3] >> 16) & 0x07);
        vip_8bit_tmp[2] = (((UINT32)vip_32bit_tmp[3] >> 8) & 0xff);
        vip_8bit_tmp[3] = ((UINT32)vip_32bit_tmp[3] & 0xff);

//        CScalerSetBit(_SCALE_DOWN_CTRL_23, ~_BIT2, 0x00);    // Diable V scale-down compensation
    }
    else
    {
        vip_8bit_tmp[0] = 0x00;//pData[0] = 0x00;
        vip_8bit_tmp[1] = 0x02;//pData[1] = 0x02;
        vip_8bit_tmp[2] = 0x00;//pData[2] = 0x00;
        vip_8bit_tmp[3] = 0x00;//pData[3] = 0x00;

//        CScalerSetBit(_SCALE_DOWN_CTRL_23, ~_BIT2, 0x00);    // Diable V scale-down compensation
    }

    rtdf_pageSelect(_PAGE0);

    for (vip_i=0; vip_i<8; vip_i++)  //CScalerWrite(_SD_DATA_PORT_25, 8, pData, _NON_AUTOINC);
    {
        rtdf_writeScalerPort(Scale_Down_Access_Port_Ctrl_PAGE0_0x0024, 0x80+vip_i);//CScalerSetByte(_SD_ACCESS_PORT_24, 0x80);
        rtdf_writeScalerPort(Scale_Down_Data_Port_PAGE0_0x0025, vip_8bit_tmp[vip_i]);
    }
    rtdf_writeScalerPort(Scale_Down_Access_Port_Ctrl_PAGE0_0x0024, 0x00);//CScalerSetByte(_SD_ACCESS_PORT_24, 0x00);

    //lack of non-linear scaling down

#if 0
//	UINT8 FlatSdFac = 72;//This value must large than 64, scale down factor can't exceed 4 times
//	UINT16 w1;
//	UINT16 w2;
    UINT8 SDRatio;
    UINT8 SDFilter=0;
    UINT32 tmp_data;
    UINT16 *coef_pt;

    UINT8 i;

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    ich1_uzd_Ctrl0_RBUS				ich1_uzd_Ctrl0_REG;
    ich1_uzd_Ctrl1_RBUS				ich1_uzd_Ctrl1_REG;
    ich1_uzd_Scale_Hor_Factor_RBUS	ich1_uzd_Scale_Hor_Factor_REG;
    ich1_uzd_Scale_Ver_Factor_RBUS	ich1_uzd_Scale_Ver_Factor_REG;
    ich1_uzd_Hor_Segment_RBUS		ich1_uzd_Hor_Segment_REG;
    ich1_uzd_Hor_Delta1_RBUS			ich1_uzd_Hor_Delta1_REG;
    ich1_uzd_Initial_Value_RBUS			ich1_uzd_Initial_Value_REG;

    ich1_uzd_Ctrl0_REG.regValue		= rtd_inl(SCALEDOWN_ICH1_UZD_CTRL0_ADDR);
    ich1_uzd_Ctrl1_REG.regValue		= rtd_inl(SCALEDOWN_ICH1_UZD_CTRL1_ADDR);
    ich1_uzd_Hor_Segment_REG.regValue = rtd_inl(SCALEDOWN_ICH1_UZD_HOR_SEGMENT_ADDR);
    ich1_uzd_Hor_Delta1_REG.regValue	= rtd_inl(SCALEDOWN_ICH1_UZD_HOR_DELTA1_ADDR);
    //ich1_uzd_Initial_Value_REG.regValue	= rtd_inl(SCALEDOWN_ICH1_UZD_INITIAL_VALUE_ADDR);


    ich2_uzd_Ctrl0_RBUS				ich2_uzd_Ctrl0_REG;
    ich2_uzd_Ctrl1_RBUS				ich2_uzd_Ctrl1_REG;
    ich2_uzd_H_Output_Size_RBUS		ich2_uzd_H_Output_Size_REG;
    ich2_uzd_Scale_Hor_Factor_RBUS	ich2_uzd_Scale_Hor_Factor_REG;
    ich2_uzd_Scale_Ver_Factor_RBUS	ich2_uzd_Scale_Ver_Factor_REG;
    ich2_uzd_Initial_Value_RBUS			ich2_uzd_Initial_Value_REG;

    ich2_uzd_Ctrl0_REG.regValue		= rtd_inl(SCALEDOWN_ICH2_UZD_CTRL0_ADDR);
    ich2_uzd_Ctrl1_REG.regValue		= rtd_inl(SCALEDOWN_ICH2_UZD_CTRL1_ADDR);
    ich2_uzd_H_Output_Size_REG.regValue= rtd_inl(SCALEDOWN_ICH2_UZD_H_OUTPUT_SIZE_ADDR);
    //ich2_uzd_Initial_Value_REG.regValue	= rtd_inl(SCALEDOWN_ICH2_UZD_INITIAL_VALUE_ADDR);

#define TMPMUL	(16)


    if (ptInSize->nWidth > ptOutSize->nWidth)      // H scale-down
    {
        // calculate scaledown ratio to select one of different bandwith filters.
#if 0
        SDRatio = ptOutSize->nWidth * TMPMUL / ptInSize->nWidth;
        if (SDRatio > (TMPMUL*2))	// sharp, wider bw
            SDFilter = 2;
        else if (SDRatio <= (TMPMUL*2) && SDRatio > (TMPMUL*3/2) )	// Mid
            SDFilter = 1;
        else		// blurest, narrow bw
            SDFilter = 0;
#else
        SDRatio = (ptInSize->nWidth*TMPMUL) / ptOutSize->nWidth ;

        //UltraZoom_Printf("CSW SDRatio number=%d\n",SDRatio);

        if (SDRatio <= ((TMPMUL*3)/2))	//<1.5 sharp, wider bw
            SDFilter = 2;
        else if (SDRatio <= (TMPMUL*2) && SDRatio > ((TMPMUL*3)/2) )	// Mid
            SDFilter = 1;
        else		// blurest, narrow bw
            SDFilter = 0;
#endif

        //SDFilter = 2;

        // fill coefficients into access port
        coef_pt = tScaleDown_COEF_TAB[SDFilter];

        if (channel == _CHANNEL1)		// channel 1
        {
            ich1_uzd_Ctrl0_REG.h_y_table_sel = 0;	// TAB1
            ich1_uzd_Ctrl0_REG.h_c_table_sel = 0;	// TAB1

            for (i=0; i<16; i++)
            {
                tmp_data = ((UINT32)(*coef_pt++)<<16);
                tmp_data += (UINT32)(*coef_pt++);
                rtd_outl(SCALEDOWN_ICH1_UZD_FIR_COEF_TAB1_C0_ADDR + i*4, tmp_data);
            }
        }
        else			// channel 2
        {

            ich2_uzd_Ctrl1_REG.h_c_filter = (SDFilter+1)%3;
            ich2_uzd_Ctrl1_REG.h_y_filter = ich2_uzd_Ctrl1_REG.h_c_filter;
            /*
            if (SDFilter == 0)	{		// narrow
            	ich2_uzd_Ctrl1_REG.h_c_filter = 1;
            	ich2_uzd_Ctrl1_REG.h_y_filter = 1;
            }else if(SDFilter == 1)	{	// mid
            	ich2_uzd_Ctrl1_REG.h_c_filter = 2;
            	ich2_uzd_Ctrl1_REG.h_y_filter = 2;
            }else{					// wider
            	ich2_uzd_Ctrl1_REG.h_c_filter = 0;
            	ich2_uzd_Ctrl1_REG.h_y_filter = 0;
            }
            */
        }
    }

    if (ptInSize->nLength > ptOutSize->nLength)      // V scale-down
    {
        // calculate scaledown ratio to select one of different bandwith filters.
#if 0
        SDRatio = ptOutSize->nLength*TMPMUL / ptInSize->nLength;
        if (SDRatio > (TMPMUL*2))	// sharp, wider bw
            SDFilter = 2;
        else if (SDRatio <= (TMPMUL*2) && SDRatio > (TMPMUL*3/2) )	// Mid
            SDFilter = 1;
        else		// blurest, narrow bw
            SDFilter = 0;
#else //jeffrey 961231
        SDRatio = (ptInSize->nLength*TMPMUL) / ptOutSize->nLength ;

        //UltraZoom_Printf("CSW SDRatio number=%d\n",SDRatio);

        if (SDRatio <= ((TMPMUL*3)/2))	//<1.5 sharp, wider bw
            SDFilter = 2;
        else if (SDRatio <= (TMPMUL*2) && SDRatio > ((TMPMUL*3)/2) )	// Mid
            SDFilter = 1;
        else		// blurest, narrow bw
            SDFilter = 0;
#endif

        UltraZoom_Printf("filter number=%d\n",SDFilter);

        if (channel == _CHANNEL1)  		// ch1 horizontal input size > 1440, enable buffer extension, use 2-tap filter
        {
            //if(ptOutSize->nWidth > 1440){
            if (ptInSize->nWidth > 1440) //Thur 20080416
            {
                SDFilter = 3;
                ich1_uzd_Ctrl0_REG.buff_ext = 1;
            }
            else
                ich1_uzd_Ctrl0_REG.buff_ext = 0;
        }
        else		// ch2 horizontal input size > 720, enable buffer extension, use 2-tap filter
        {
            //if(ptOutSize->nWidth  > 720)
            if (ptInSize->nWidth  > 720)
                ich2_uzd_Ctrl0_REG.buff_ext = 1;
            else
                ich2_uzd_Ctrl0_REG.buff_ext = 0;
        }

        // fill coefficients into access port
        coef_pt = tScaleDown_COEF_TAB[SDFilter];

        if (channel == _CHANNEL1)		// channel 1
        {
            ich1_uzd_Ctrl0_REG.v_y_table_sel = 1;	// TAB2
            ich1_uzd_Ctrl0_REG.v_c_table_sel = 1;	// TAB2

            for (i=0; i<16; i++)
            {
                tmp_data = ((UINT32)(*coef_pt++)<<16);
                tmp_data += (UINT32)(*coef_pt++);
                rtd_outl(SCALEDOWN_ICH1_UZD_FIR_COEF_TAB2_C0_ADDR + i*4, tmp_data);
            }
        }
        else			// channel 2
        {
            ich2_uzd_Ctrl1_REG.v_c_filter = (SDFilter+1)%3;
            ich2_uzd_Ctrl1_REG.v_y_filter = ich2_uzd_Ctrl1_REG.v_c_filter;
            /*
            			if (SDFilter == 0)	{		// narrow
            				ich2_uzd_Ctrl1_REG.v_c_filter = 1;
            				ich2_uzd_Ctrl1_REG.v_y_filter = 1;
            			}else if(SDFilter == 1)	{	// mid
            				ich2_uzd_Ctrl1_REG.v_c_filter = 2;
            				ich2_uzd_Ctrl1_REG.v_y_filter = 2;
            			}else{					// wider
            				ich2_uzd_Ctrl1_REG.v_c_filter = 0;
            				ich2_uzd_Ctrl1_REG.v_y_filter = 0;
            			}
            */
        }
    }
    else //no need  V scale-down
    {
        if (channel == _CHANNEL1)
        {
            ich1_uzd_Ctrl0_REG.buff_ext = 0;
        }
        else
        {
            ich2_uzd_Ctrl0_REG.buff_ext = 0;
        }
    }

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    UINT32 nFactor, D;
    UINT8 Hini, Vini, a;
    UINT16 S1,S2;


    // Decide Output data format for scale down
    if (channel == _CHANNEL1)
    {

        if (ptInSize->nWidth > ptOutSize->nWidth)      // H scale-down
        {
            Hini = 0xff;//0x78;
            ich1_uzd_Initial_Value_REG.hor_ini = Hini;
            a = 5;

            if (panorama && (!GET_HSCALE_UP(_MAIN_DISPLAY)))
            {  // CSW+ 0960830 Non-linear SD
                S1 = (ptOutSize->nWidth) >> 2;
                S2 = (ptOutSize->nWidth) - S1*2;
                //=vv==old from TONY, if no problem, use this=vv==
                nFactor = (UINT32)(((ptInSize->nWidth)<<21) - ((UINT32)Hini<<13));
                //=vv==Thur debugged=vv==
                //nFactor = ((((UINT64)(ptInSize->nWidth))*(ptOutSize->nWidth-1))<<21) / ptOutSize->nWidth;
                //===================
                nFactor = nFactor /((2*a+1)*S1 + (a+1)*S2 - a) * a;
                D = nFactor / a / S1;
                nFactor = SHR((nFactor + 1), 1); //rounding
                D = SHR(D + 1, 1); //rounding
                RTD_Log(LOGGER_INFO,"\nPANORAMA2 TEST 111\n");
                RTD_Log(LOGGER_INFO,"nPANORAMA2 Factor = %d\n",nFactor);
                RTD_Log(LOGGER_INFO,"PANORAMA2 S1=%d, S2=%d, D = %d\n",S1, S2, D);

                if (nFactor < 1048576)
                {
                    RTD_Log(LOGGER_INFO,"PANORAMA2 Can't do nonlinear SD \n");
                    //nFactor = (UINT32)(((ptInSize->nWidth)<<21) - ((UINT32)Hini<<13)) / (ptOutSize->nWidth-1);
                    nFactor = (UINT32)(((ptInSize->nWidth)<<21)) / (ptOutSize->nWidth);
                    nFactor = SHR(nFactor + 1, 1); //rounding
                    RTD_Log(LOGGER_INFO,"PANORAMA2 nFactor=%x, input Wid=%d, Out Wid=%d\n",nFactor, ptInSize->nWidth, ptOutSize->nWidth);
                    S1 = 0;
                    S2 = ptOutSize->nWidth;
                    D = 0;
                }
            }
            else
            {
#ifdef CONFIG_CUSTOMER_1
                if (COsdCtrlGetOverSampling() && (info->ucMode_Curr==_MODE_720P60 ||info->ucMode_Curr==_MODE_720P50 ||info->ucMode_Curr==_MODE_480P ||info->ucMode_Curr==_MODE_480I))
                {
                    //nFactor = (UINT32)(((ptInSize->nWidth)<<19) - ((UINT32)Hini<<11)) / (ptOutSize->nWidth-1) *2;
                    nFactor = (UINT32)(((ptInSize->nWidth)<<19)) / (ptOutSize->nWidth) *2;
                }
                else
#endif
                {
                    //nFactor = (UINT32)(((ptInSize->nWidth)<<21) - ((UINT32)Hini<<13)) / (ptOutSize->nWidth-1);
                    nFactor = (UINT32)(((ptInSize->nWidth)<<21)) / (ptOutSize->nWidth);
                    nFactor = SHR(nFactor + 1, 1); //rounding
                }
                UltraZoom_Printf("nFactor=%x, input Wid=%d, Out Wid=%d\n",nFactor, ptInSize->nWidth, ptOutSize->nWidth);
                S1 = 0;
                S2 = ptOutSize->nWidth;
                D = 0;
            }
        }
        else
        {
            nFactor = 0x100000;
            S1 = 0;
            S2 = ptOutSize->nWidth;
            D = 0;
        }
        ich1_uzd_Scale_Hor_Factor_REG.hor_fac = nFactor;
        ich1_uzd_Hor_Segment_REG.nl_seg1 = S1;
        ich1_uzd_Hor_Segment_REG.nl_seg2 = S2;
        ich1_uzd_Hor_Delta1_REG.nl_d1 = D;
//RTD_Log(LOGGER_INFO,"reg_seg1=%x, reg_Seg_all=%x\n",ich1_uzd_Hor_Segment_REG.nl_seg1, ich1_uzd_Hor_Segment_REG.regValue);
        UltraZoom_Printf("nFactor=%x, input_Wid=%d, Output_Wid=%d\n",nFactor, ptInSize->nWidth, ptOutSize->nWidth);

        if (ptInSize->nLength > ptOutSize->nLength)      // V scale-down
        {
//			ich1_uzd_Ctrl0_REG.buffer_mode = 0x02;	// buffer assign to vertical uzd
            Vini = 0xff;//0x78;
            ich1_uzd_Initial_Value_REG.ver_ini = Vini;
            //nFactor = (UINT32)((ptInSize->nLength <<20) - ((UINT32)Vini<<12)) / (ptOutSize->nLength - 1);
            nFactor = (UINT32)((ptInSize->nLength <<21)) / (ptOutSize->nLength);
            UltraZoom_Printf("Ch1 Ver: CAP=%d,Disp=%d, factor=%x\n", ptInSize->nLength,ptOutSize->nLength, nFactor);
            nFactor = SHR(nFactor + 1, 1); //rounding
            ich1_uzd_Scale_Ver_Factor_REG.ver_fac = nFactor;
        }
        else
        {
//			ich1_uzd_Ctrl0_REG.buffer_mode = 0;
            ich1_uzd_Scale_Ver_Factor_REG.ver_fac = 0x100000;
        }

//	ich1_uzd_Ctrl0_REG.   = 1;
//	ich1_uzd_Ctrl0_REG.sort_fmt = 1;
//	CLR_422_Fr_SD();

        rtd_outl(SCALEDOWN_ICH1_UZD_CTRL0_ADDR, ich1_uzd_Ctrl0_REG.regValue);
        rtd_outl(SCALEDOWN_ICH1_UZD_CTRL1_ADDR, ich1_uzd_Ctrl1_REG.regValue);
        rtd_outl(SCALEDOWN_ICH1_UZD_SCALE_HOR_FACTOR_ADDR, ich1_uzd_Scale_Hor_Factor_REG.regValue);
        rtd_outl(SCALEDOWN_ICH1_UZD_SCALE_VER_FACTOR_ADDR, ich1_uzd_Scale_Ver_Factor_REG.regValue);
        rtd_outl(SCALEDOWN_ICH1_UZD_HOR_SEGMENT_ADDR, ich1_uzd_Hor_Segment_REG.regValue);
        rtd_outl(SCALEDOWN_ICH1_UZD_HOR_DELTA1_ADDR, ich1_uzd_Hor_Delta1_REG.regValue);
        rtd_outl(SCALEDOWN_ICH1_UZD_INITIAL_VALUE_ADDR, ich1_uzd_Initial_Value_REG.regValue);
    }

    if (channel == _CHANNEL2)
    {

        if (ptInSize->nWidth > ptOutSize->nWidth)      // H scale-down
        {
            Hini = 0xff;//0x78;
            ich2_uzd_Initial_Value_REG.hor_ini = Hini;
#ifdef CONFIG_CUSTOMER_1
            if (COsdCtrlGetOverSampling() && (info->ucMode_Curr==_MODE_720P60 ||info->ucMode_Curr==_MODE_720P50 ||info->ucMode_Curr==_MODE_480P ||info->ucMode_Curr==_MODE_480I))
                //nFactor = (UINT32)((ptInSize->nWidth<<19) - (Hini<<11)) / (ptOutSize->nWidth-1) *2;
                nFactor = (UINT32)((ptInSize->nWidth<<19)) / (ptOutSize->nWidth) *2;
            else
#endif
                //nFactor = (UINT32)((ptInSize->nWidth<<20) - (Hini<<12)) / (ptOutSize->nWidth-1);
                nFactor = (UINT32)((ptInSize->nWidth<<21)) / (ptOutSize->nWidth);
            nFactor = SHR(nFactor + 1, 1); //rounding
            ich2_uzd_Scale_Hor_Factor_REG.hor_fac = nFactor;
        }
        else
        {
            ich2_uzd_Scale_Hor_Factor_REG.hor_fac = 0x100000;
        }
        ich2_uzd_H_Output_Size_REG.h_output_size = ptOutSize->nWidth;

        if (ptInSize->nLength > ptOutSize->nLength)      // V scale-down
        {
//			ich2_uzd_Ctrl0_REG.buffer_mode = 0x02;
            Vini = 0xff;//0x78;
            ich2_uzd_Initial_Value_REG.ver_ini = Vini;
            //nFactor = (UINT32)((ptInSize->nLength << 20) - (Vini<<12)) / (ptOutSize->nLength - 1);
            nFactor = (UINT32)((ptInSize->nLength << 21)) / (ptOutSize->nLength);
            nFactor = SHR(nFactor+1, 1); //rounding
            ich2_uzd_Scale_Ver_Factor_REG.ver_fac = nFactor;
        }
        else
        {
//			ich2_uzd_Ctrl0_REG.buffer_mode = 0;
            ich2_uzd_Scale_Ver_Factor_REG.ver_fac = 0x100000;
        }

//	ich2_uzd_Ctrl0_REG.output_fmt = 1;
//	CLR_422_Fr_SD();

        rtd_outl(SCALEDOWN_ICH2_UZD_CTRL0_ADDR, ich2_uzd_Ctrl0_REG.regValue);
        rtd_outl(SCALEDOWN_ICH2_UZD_CTRL1_ADDR, ich2_uzd_Ctrl1_REG.regValue);
        rtd_outl(SCALEDOWN_ICH2_UZD_H_OUTPUT_SIZE_ADDR, ich2_uzd_H_Output_Size_REG.regValue);
        rtd_outl(SCALEDOWN_ICH2_UZD_SCALE_HOR_FACTOR_ADDR, ich2_uzd_Scale_Hor_Factor_REG.regValue);
        rtd_outl(SCALEDOWN_ICH2_UZD_SCALE_VER_FACTOR_ADDR, ich2_uzd_Scale_Ver_Factor_REG.regValue);
        rtd_outl(SCALEDOWN_ICH2_UZD_INITIAL_VALUE_ADDR, ich2_uzd_Initial_Value_REG.regValue);
    }
#endif
}



#if 0//20090403 jason9.ccc marked, Sparrow always FrameSync, so there's no need to set 422 for FRC
/*============================================================================*/
/**
 * CScalerSetRGB422ScaleDown
 * This function is used to set scaling-down registers if RGB format to do 444->422, including main and sub display.
 *
 * @param <info> { display-info struecture }
 * @return { none }
 * @note
 * It can be linear or non-linear either, based on the non-linear flag condition:
 */
//void Scaler_SetScaleDown(UINT8 display)
void drvif_color_ultrazoom_scale_down_444_to_422(UINT8 channel, SIZE* ptInSize, SIZE* ptOutSize)
{
    channel=0;
    ptInSize=0;
    ptOutSize=0;


    UINT8 SDFilter=0;
    UINT32 tmp_data;
    UINT16 *coef_pt;

    UINT8 i;

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//ch1
    ich1_uzd_Ctrl0_RBUS				ich1_uzd_Ctrl0_REG;
    ich1_uzd_Initial_Value_RBUS			ich1_uzd_Initial_Value_REG;
    ich1_uzd_Scale_Hor_Factor_RBUS	ich1_uzd_Scale_Hor_Factor_REG;
    ich1_uzd_Hor_Segment_RBUS		ich1_uzd_Hor_Segment_REG;

    ich1_uzd_Ctrl0_REG.regValue		= rtd_inl(SCALEDOWN_ICH1_UZD_CTRL0_ADDR);
    ich1_uzd_Initial_Value_REG.regValue		= rtd_inl(SCALEDOWN_ICH1_UZD_INITIAL_VALUE_ADDR);
    ich1_uzd_Scale_Hor_Factor_REG.regValue =  rtd_inl(SCALEDOWN_ICH1_UZD_SCALE_HOR_FACTOR_ADDR);
    ich1_uzd_Hor_Segment_REG.regValue=  rtd_inl(SCALEDOWN_ICH1_UZD_HOR_SEGMENT_ADDR);
//ch2
    ich2_uzd_Ctrl0_RBUS				ich2_uzd_Ctrl0_REG;
    ich2_uzd_Initial_Value_RBUS			ich2_uzd_Initial_Value_REG;
    ich2_uzd_Scale_Hor_Factor_RBUS	ich2_uzd_Scale_Hor_Factor_REG;

    ich2_uzd_Ctrl0_REG.regValue		= rtd_inl(SCALEDOWN_ICH2_UZD_CTRL0_ADDR);
    ich2_uzd_Initial_Value_REG.regValue		= rtd_inl(SCALEDOWN_ICH2_UZD_INITIAL_VALUE_ADDR);
    ich2_uzd_Scale_Hor_Factor_REG.regValue =  rtd_inl(SCALEDOWN_ICH2_UZD_SCALE_HOR_FACTOR_ADDR);


    // H no scale-down and It's 444->422
    if ((ptInSize->nWidth <= ptOutSize->nWidth)&& GET_422_Fr_SD(info->display) && (!GET_INTERLACE_MODE(info->display))
            &&((GET_INPUTSOURCE_TYPE()== _SRC_VGA)||((GET_INPUTSOURCE_TYPE()==_SRC_HDMI)&&(!drvif_Hdmi_FormatDetect()))))
    {
        if (channel == _CHANNEL1)
        {
            SDFilter = 1;

            // fill coefficients into access port
            coef_pt = tScaleDown_COEF_TAB[SDFilter];
            ich1_uzd_Ctrl0_REG.h_y_table_sel = 0;	// TAB1
            ich1_uzd_Ctrl0_REG.h_c_table_sel = 0;	// TAB1
            ich1_uzd_Ctrl0_REG.h_zoom_en = 1;
            ich1_uzd_Initial_Value_REG.hor_ini = 0xff;
            ich1_uzd_Scale_Hor_Factor_REG.hor_fac = 1<<20;
            ich1_uzd_Hor_Segment_REG.nl_seg1 = 0;
            ich1_uzd_Hor_Segment_REG.nl_seg2 = ptInSize->nWidth;

            for (i=0; i<16; i++)
            {
                tmp_data = ((UINT32)(*coef_pt++)<<16);
                tmp_data += (UINT32)(*coef_pt++);
                rtd_outl(SCALEDOWN_ICH1_UZD_FIR_COEF_TAB1_C0_ADDR + i*4, tmp_data);
            }

            rtd_outl(SCALEDOWN_ICH1_UZD_INITIAL_VALUE_ADDR, ich1_uzd_Initial_Value_REG.regValue);
            rtd_outl(SCALEDOWN_ICH1_UZD_SCALE_HOR_FACTOR_ADDR, ich1_uzd_Scale_Hor_Factor_REG.regValue);
            rtd_outl(SCALEDOWN_ICH1_UZD_HOR_SEGMENT_ADDR, ich1_uzd_Hor_Segment_REG.regValue);
            rtd_outl(SCALEDOWN_ICH1_UZD_CTRL0_ADDR, ich1_uzd_Ctrl0_REG.regValue);
        }

        else if (channel == _CHANNEL2)
        {

            ich2_uzd_Ctrl0_REG.h_zoom_en = 1;
            ich2_uzd_Initial_Value_REG.hor_ini = 0xff;
            ich2_uzd_Scale_Hor_Factor_REG.hor_fac = 1<<20;

            rtd_outl(SCALEDOWN_ICH2_UZD_INITIAL_VALUE_ADDR, ich2_uzd_Initial_Value_REG.regValue);
            rtd_outl(SCALEDOWN_ICH2_UZD_SCALE_HOR_FACTOR_ADDR, ich2_uzd_Scale_Hor_Factor_REG.regValue);
            rtd_outl(SCALEDOWN_ICH2_UZD_CTRL0_ADDR, ich2_uzd_Ctrl0_REG.regValue);
        }
    }

}
#endif

void drvif_color_ultrazoom_config_scaling_down(UZ_Para *UZD)
{
//	SIZE inSize, outSize;
    // For sparrow only ....Enable SD_buf
#if 0
    if ((UZD->VScale_En)||(UZD->HScale_En))
        rtdf_clearBitsb(SYS_CLKSEL_0x2ccc, _BIT2);
    else
        rtdf_setBitsb(SYS_CLKSEL_0x2ccc, _BIT2);
#endif
    //Scaling Down : Page 0
//	rtdf_outb(Page_Select_0x009f, _PAGE0);
    rtdf_pageSelect(_PAGE0);

//Start of SD ========================================================================================
    // Config scaling

    if (UZD->VScale_En)
        rtdf_setBitsScalerPort(SCALE_DOWN_CTRL_PAGE0_0x0023, _BIT0);
    else
        rtdf_clearBitsScalerPort(SCALE_DOWN_CTRL_PAGE0_0x0023, _BIT0);

    if (UZD->HScale_En)
        rtdf_setBitsScalerPort(SCALE_DOWN_CTRL_PAGE0_0x0023, _BIT1);
    else
        rtdf_clearBitsScalerPort(SCALE_DOWN_CTRL_PAGE0_0x0023, _BIT1);

    if (UZD->Video_Com_En)
        rtdf_setBitsScalerPort(SCALE_DOWN_CTRL_PAGE0_0x0023, _BIT2);
    else
        rtdf_clearBitsScalerPort(SCALE_DOWN_CTRL_PAGE0_0x0023, _BIT2);

//	inSize.nLength = UZD->InLen;
//	inSize.nWidth= UZD->InWid;

//	outSize.nLength = UZD->OutLen;
//	outSize.nWidth= UZD->OutWid;

    drvif_color_ultrazoom_set_scale_down(&UZD->InSize, &UZD->OutSize);


//end of SD ========================================================================================
#if 0
    SIZE inSize, outSize;
    // Set scale down control register
    //	ScalerSetScaleDownCtrl(info->display);

    if (info->channel == _CHANNEL1)
    {
        // Scale down setup for Channel1
        ich1_uzd_Ctrl0_RBUS ich1_uzd_Ctrl0_REG;
        ich1_uzd_Ctrl0_REG.regValue = rtd_inl(SCALEDOWN_ICH1_UZD_CTRL0_ADDR);
        ich1_uzd_Ctrl0_REG.v_zoom_en = (GET_VSCALE_DOWN(info->display)!=0);
        ich1_uzd_Ctrl0_REG.buffer_mode = GET_VUZD_BUFFER_MODE(info->display);
        ich1_uzd_Ctrl0_REG.h_zoom_en = (GET_HSCALE_DOWN(info->display)!=0);
        ich1_uzd_Ctrl0_REG.sort_fmt = (info->FrameSyncFlag) || (!GET_422_Fr_SD(info->display));
        ich1_uzd_Ctrl0_REG.out_bit = GET_10BIT_FORMAT(info->display);
        ich1_uzd_Ctrl0_REG.video_comp_en = (GET_VIDEO_COMP_MODE(info->display) ? 1 : 0 );
        rtdf_outl(SCALEDOWN_ICH1_UZD_CTRL0_ADDR, ich1_uzd_Ctrl0_REG.regValue);

    }
    else
    {
        // Scale down Setup for Channel 2
        ich2_uzd_Ctrl0_RBUS ich2_uzd_Ctrl0_REG;
        ich2_uzd_Ctrl0_REG.regValue = rtd_inl(SCALEDOWN_ICH2_UZD_CTRL0_ADDR);
        ich2_uzd_Ctrl0_REG.v_zoom_en = (GET_VSCALE_DOWN(info->display)!=0);
        ich2_uzd_Ctrl0_REG.buffer_mode = GET_VUZD_BUFFER_MODE(info->display);
        ich2_uzd_Ctrl0_REG.h_zoom_en = (GET_HSCALE_DOWN(info->display)!=0);
        ich2_uzd_Ctrl0_REG.output_fmt = (GET_422_Fr_SD(info->display) ? 0 : 1);
        ich2_uzd_Ctrl0_REG.video_comp_en = (GET_VIDEO_COMP_MODE(info->display) ? 1 : 0 );
        rtdf_outl(SCALEDOWN_ICH2_UZD_CTRL0_ADDR, ich2_uzd_Ctrl0_REG.regValue);

    }

    //20080121 jason marked for 1080i display crash after enable DelPixelEn
    if ((GET_INTERLACE_IN_INPUT_MODE(info->display) == _TRUE) && ((GET_INPUTSOURCE_TYPE() == _SRC_VGA) ||
            ((GET_INPUTSOURCE_FROM() == _SRC_FROM_TMDS) && (!drvif_Hdmi_FormatDetect()))) )
        inSize.nLength = GET_INTERLACE_MODE(info->display) ? GET_INPUT_LENGTH(info->display) << 1 : GET_INPUT_LENGTH(info->display);
    else
        inSize.nLength = GET_INTERLACE_MODE(info->display) ? (GET_INPUT_LENGTH(info->display) << 1)-4 : GET_INPUT_LENGTH(info->display);

    inSize.nWidth = GET_INPUT_WIDTH(info->display);
    outSize.nLength = GET_MEMORY_CAP_LENGTH(info->display);
    outSize.nWidth = GET_MEMORY_CAP_WIDTH(info->display);
    drvif_color_ultrazoom_set_scale_down(info->channel, &inSize, &outSize, panorama);

    drvif_color_ultrazoom_scale_down_444_to_422(info->channel, &inSize, &outSize);	//CSW+ 0970324 for RGB to do 444->422 color will have sample error phase

#endif

}

//void drvif_color_ultrazoom_config_scaling_up(UINT8 panorama, UINT8 input422_flag , UINT8 VSCALE_UP_En, UINT8 HSCALE_UP_En, UINT8 VIDEO_COMP_En, UINT16 inLen, UINT16 inWid, UINT16 DispLen, UINT16 DispWid)
void drvif_color_ultrazoom_config_scaling_up(UZ_Para *UZU)
{
//	SIZE inSize, outSize;

//	ScalerSetScaleUpCtrl(info->display);
//	dm_uzu_Ctrl_RBUS dm_uzu_Ctrl_REG;
//	dm_uzu_Ctrl_REG.regValue = rtd_inl(SCALEUP_DM_UZU_CTRL_ADDR);
    UINT32 tmp;
    tmp = UZU->Video_Com_En;
    tmp = tmp <<2 | UZU->Input422_flag;
    tmp = tmp <<8 |	UZU->VScale_En;
    tmp = tmp <<1|UZU->HScale_En;
    rtdf_maskl(DM_UZU_CTRL_0x1604, ~(_BIT0|_BIT1|_BIT9|_BIT11), tmp);//dm_uzu_Ctrl_REG.v_zoom_en = VSCALE_UP_En;
//	rtdf_maskl(DM_UZU_CTRL_0x1604, ~(_BIT0), (UZU->HScale_En));//dm_uzu_Ctrl_REG.h_zoom_en = HSCALE_UP_En;
//	tmp = UZU->Input422_flag;
//	tmp = tmp <<9;
//	rtdf_maskl(DM_UZU_CTRL_0x1604, ~(_BIT9), tmp);//dm_uzu_Ctrl_REG.in_fmt_conv =input422_flag;//dm_uzu_Ctrl_REG.in_fmt_conv = !(info->FrameSyncFlag) && GET_422_Fr_SD(info->display);
//	tmp = UZU->Video_Com_En;
//	tmp = tmp <<11;
//	rtdf_maskl(DM_UZU_CTRL_0x1604, ~(_BIT11), tmp);//dm_uzu_Ctrl_REG.video_comp_en = VIDEO_COMP_En;

//	rtdf_outl(SCALEUP_DM_UZU_CTRL_ADDR, dm_uzu_Ctrl_REG.regValue);
//	UltraZoom_Printf("dm_uzu_Ctrl_REG.regValue=%x\n", dm_uzu_Ctrl_REG.regValue);

//	inSize.nLength = UZU->InLen;//GET_MEMORY_DISP_LENGTH(info->display);
//	inSize.nWidth = UZU->InWid;//GET_MEMORY_DISP_WIDTH(info->display);
//	outSize.nLength = UZU->OutLen;
//	outSize.nWidth = UZU->OutWid;
    drvif_color_ultrazoom_setscaleup(&UZU->InSize, &UZU->OutSize, UZU->panorama);	// Display scaling up control, linear or non-linear either

#if 0

    // Set scale up control register
    dm_uzu_Ctrl_RBUS dm_uzu_Ctrl_REG;
    dm_uzu_Ctrl_REG.regValue = rtd_inl(SCALEUP_DM_UZU_CTRL_ADDR);
    dm_uzu_Ctrl_REG.v_zoom_en = (GET_VSCALE_UP(info->display)!=0);
    dm_uzu_Ctrl_REG.h_zoom_en = (GET_HSCALE_UP(info->display)!=0);
    dm_uzu_Ctrl_REG.in_fmt_conv = !(info->FrameSyncFlag) && GET_422_Fr_SD(info->display);
    dm_uzu_Ctrl_REG.video_comp_en = (GET_VIDEO_COMP_MODE(info->display) ? 1 : 0 );

    rtdf_outl(SCALEUP_DM_UZU_CTRL_ADDR, dm_uzu_Ctrl_REG.regValue);
    UltraZoom_Printf("dm_uzu_Ctrl_REG.regValue=%x\n", dm_uzu_Ctrl_REG.regValue);

    inSize.nLength = GET_MEMORY_DISP_LENGTH(info->display);
    inSize.nWidth = GET_MEMORY_DISP_WIDTH(info->display);
    outSize.nLength = info->DispLen;
    outSize.nWidth = info->DispWid;
    drvif_color_ultrazoom_setscaleup(info->display, &inSize, &outSize, panorama);	// Display scaling up control, linear or non-linear either
#endif

}

void drvif_color_scaler_setscaleup_hcoef(UINT8 factor)
{
    UINT16 *table_index;//UINT16 *coef_pt;
    vip_32bit_tmp[0]=0;//UINT32 temp;

//	RTD_Log(LOGGER_INFO,"\nhcoef==>%d\n",(UINT32)factor);

    if (factor > 10)
        return; // _FALSE;

//	dm_uzu_Ctrl_RBUS dm_uzu_Ctrl_REG;
//	dm_uzu_Ctrl_REG.regValue = rtd_inl(SCALEUP_DM_UZU_CTRL_ADDR);
    rtdf_clearBits( DM_UZU_CTRL_0x1604, _BIT5);// H_TABLE_SEL = 0
    rtdf_clearBits( DM_UZU_CTRL_0x1604, _BIT4);// H_TABLE_SEL = 0

    table_index = tSU_COEF_TABLE[factor];//coef_pt = tSU_COEF_TABLE[factor];

    for (vip_i=0; vip_i<32; vip_i++)
    {
        vip_32bit_tmp[0] = ((*table_index++)&0x0fff);
        vip_32bit_tmp[0] =  (vip_32bit_tmp[0]<<16);	//vip_32bit_tmp[0] =  (((*coef_pt++)&0x0fff)<<16);
        vip_32bit_tmp[0] += ((*table_index++)&0x0fff);	//vip_32bit_tmp[0] += ((*coef_pt++)&0x0fff);
        rtd_outl(DM_UZU_FIR_COEF_TAB1_C0_0x1700+vip_i*4, vip_32bit_tmp[0]);
    }

#if 0 //original dragon
    if (display == _MAIN_DISPLAY)	// 0:main / 1:sub
    {
        dm_uzu_Ctrl_RBUS dm_uzu_Ctrl_REG;

        dm_uzu_Ctrl_REG.regValue = rtd_inl(SCALEUP_DM_UZU_CTRL_ADDR);
        dm_uzu_Ctrl_REG.h_y_table_sel = 0;	// H_TABLE_SEL = 0
        dm_uzu_Ctrl_REG.h_c_table_sel = 0;  // H_TABLE_SEL = 0
        rtdf_outl(SCALEUP_DM_UZU_CTRL_ADDR, dm_uzu_Ctrl_REG.regValue);

        coef_pt = tSU_COEF_TABLE[factor];
        for (i=0; i<32; i++)
        {
            temp =  (((*coef_pt++)&0x0fff)<<16);
            temp += ((*coef_pt++)&0x0fff);
            rtd_outl(SCALEUP_DM_UZU_FIR_COEF_TAB1_C0_ADDR+i*4, temp);
        }
    }

    else
    {
        sub_pt = tsub_SU_COEF_TABLE[0];

        for (i=0; i<4; i++)
        {
            temp = ((*sub_pt++)<<24);
            temp += ((*sub_pt++)<<16);
            temp +=	((*sub_pt++)<<8);
            temp += (*sub_pt++);
            rtd_outl(SCALEUP_DS_UZU_FIR_COEF_HOR_C0_ADDR+i*4, temp);
        }
    }
#endif
}

void drvif_color_scaler_setscaleup_vcoef(UINT8 factor)
{
    UINT16 *table_index;
    vip_32bit_tmp[0]=0;//UINT32 temp;

//	RTD_Log(LOGGER_INFO,"\nvcoef==>%d\n",(UINT32)factor);

    if (factor > 10)
        return; // _FALSE;

//		dm_uzu_Ctrl_RBUS dm_uzu_Ctrl_REG;
//		dm_uzu_Ctrl_REG.regValue = rtd_inl(SCALEUP_DM_UZU_CTRL_ADDR);
    rtdf_setBits( DM_UZU_CTRL_0x1604, _BIT7);		// V_TABLE_SEL = 1
    rtdf_clearBits( DM_UZU_CTRL_0x1604, _BIT6);	 // V_TABLE_SEL = 0
//		rtdf_outl(SCALEUP_DM_UZU_CTRL_ADDR, dm_uzu_Ctrl_REG.regValue);

    table_index = tSU_COEF_TABLE[factor];//coef_pt = tSU_COEF_TABLE[factor];

    for (vip_i=0; vip_i<32; vip_i++)
    {
        vip_32bit_tmp[0] = ((*table_index++)&0x0fff);
        vip_32bit_tmp[0] =  (vip_32bit_tmp[0]<<16);//temp =  (((*coef_pt++)&0x0fff)<<16);
        vip_32bit_tmp[0] += ((*table_index++)&0x0fff);//temp += ((*coef_pt++)&0x0fff);
        rtd_outl(DM_UZU_FIR_COEF_TAB2_C0_0x1780 + vip_i*4, vip_32bit_tmp[0]);
    }

#if 0
    if (display == _MAIN_DISPLAY)	// 0:main / 1:sub
    {
        dm_uzu_Ctrl_RBUS dm_uzu_Ctrl_REG;

        dm_uzu_Ctrl_REG.regValue = rtd_inl(SCALEUP_DM_UZU_CTRL_ADDR);
        dm_uzu_Ctrl_REG.v_y_table_sel = 1;	// V_TABLE_SEL = 1
        dm_uzu_Ctrl_REG.v_c_table_sel = 0;  // V_TABLE_SEL = 0
        rtdf_outl(SCALEUP_DM_UZU_CTRL_ADDR, dm_uzu_Ctrl_REG.regValue);

        coef_pt = tSU_COEF_TABLE[factor];
        for (i=0; i<32; i++)
        {
            temp =  (((*coef_pt++)&0x0fff)<<16);
            temp += ((*coef_pt++)&0x0fff);
            rtd_outl(SCALEUP_DM_UZU_FIR_COEF_TAB2_C0_ADDR + i*4, temp);
        }
    }
    else
    {
        sub_pt = tsub_SU_COEF_TABLE[0];

        for (i=0; i<4; i++)
        {
            temp = ((*sub_pt++)<<24);
            temp += ((*sub_pt++)<<16);
            temp +=	((*sub_pt++)<<8);
            temp += (*sub_pt++);
            rtd_outl(SCALEUP_DS_UZU_FIR_COEF_VER_C0_C3_ADDR + i*4, temp);
        }
    }
#endif
}

void drvif_color_scaler_setds_pk(/*UINT8 display, */UINT8 factor)
{

//	dm_uzu_Peaking_Coring_RBUS	dm_uzu_Peaking_Coring_REG;
//	dm_uzu_Peaking_Gain_RBUS	dm_uzu_Peaking_Gain_REG;

//	dm_uzu_Peaking_Coring_REG.regValue = rtd_inl(SCALEUP_DM_UZU_PEAKING_CORING_ADDR);
//	dm_uzu_Peaking_Gain_REG.regValue = rtd_inl(SCALEUP_DM_UZU_PEAKING_GAIN_ADDR);

    if (factor>10)	factor = 10;

//	INT32 ndG1, ndG2, ndOffset, ndLmtP, ndLmtN;
    vip_8bit_tmp[0]=0;//INT32 Coring,
    vip_8bit_tmp[1]=0;//X1;
    vip_8bit_tmp[2]=0;//ndG1
    vip_8bit_tmp[3]=0;// ndG2
    vip_8bit_tmp[4]=0;//ndOffset
    vip_8bit_tmp[5]=0;//ndLmtP
    vip_8bit_tmp[6]=0;//ndLmtN;
    vip_8bit_tmp[7]=0;//INT32 pk_en;

    if (factor < 6)
    {
        vip_8bit_tmp[0]=4;//Coring = 4;
        vip_8bit_tmp[1]=8;//X1 = 8;
        vip_8bit_tmp[5]=5;//ndLmtP = 5;
        vip_8bit_tmp[6]=5;//ndLmtN = 5;
        vip_8bit_tmp[2]=factor;//ndG1 = factor;
        vip_8bit_tmp[3]=factor;//ndG2 = factor;

    }
    else
    {
        vip_8bit_tmp[0]	=SU_PK_Coeff[factor - 6][0];//Coring = SU_PK_Coeff[factor - 6][0];
        vip_8bit_tmp[1]  	=SU_PK_Coeff[factor - 6][1];//X1 = 	SU_PK_Coeff[factor - 6][1];
        vip_8bit_tmp[5] 	=SU_PK_Coeff[factor - 6][2];//ndLmtP = SU_PK_Coeff[factor - 6][2];
        vip_8bit_tmp[6] 	=SU_PK_Coeff[factor - 6][3];//ndLmtN = SU_PK_Coeff[factor - 6][3];
        vip_8bit_tmp[2]	=SU_PK_Coeff[factor - 6][4];//ndG1 = 	SU_PK_Coeff[factor - 6][4];
        vip_8bit_tmp[3]	=SU_PK_Coeff[factor - 6][5];//ndG2 = 	SU_PK_Coeff[factor - 6][5];
    }


    vip_8bit_tmp[4] = (vip_8bit_tmp[1]-vip_8bit_tmp[0])*vip_8bit_tmp[2]/4;//ndOffset = (X1-Coring)*ndG1/4;

    if (factor)
        vip_8bit_tmp[7]=1;//pk_en = 1;
    else
    {
        vip_8bit_tmp[7]=0;//pk_en = 0;
        //	G1 = 0;
        //	G2 = 0;
        vip_8bit_tmp[2]=0;//ndG1 = 0;
        vip_8bit_tmp[3]=0;//ndG2 = 0;
    }

//	dm_uzu_Peaking_Coring_REG.pk_en		= pk_en;
//	dm_uzu_Peaking_Coring_REG.pk_x1		= X1;
//	dm_uzu_Peaking_Coring_REG.pk_coring	= Coring;
//	dm_uzu_Peaking_Gain_REG.pk_offset		= ndOffset;
//	dm_uzu_Peaking_Gain_REG.pk_g1		= ndG1;
//	dm_uzu_Peaking_Gain_REG.pk_g2		= ndG2;
//	dm_uzu_Peaking_Gain_REG.pk_lmtn		= ndLmtN;
//	dm_uzu_Peaking_Gain_REG.pk_lmtp		= ndLmtP;

//	rtd_outl(SCALEUP_DM_UZU_PEAKING_CORING_ADDR, dm_uzu_Peaking_Coring_REG.regValue);
//	rtd_outl(SCALEUP_DM_UZU_PEAKING_GAIN_ADDR, dm_uzu_Peaking_Gain_REG.regValue);

//	rtdf_maskl(DM_UZU_PEAKING_CORING_0x162c, ~(_BIT16), (vip_8bit_tmp[7]&0x1)<<16);//pk_en;
//	rtdf_maskl( DM_UZU_PEAKING_CORING_0x162c, ~(0x000000ff), (vip_8bit_tmp[1]));//X1;
//	rtdf_maskl( DM_UZU_PEAKING_CORING_0x162c, ~(0x0000ff00), (vip_8bit_tmp[0])<<8);//Coring;
#if 0
    vip_32bit_tmp[0] = ( rtd_inl(DM_UZU_PEAKING_CORING_0x162c) & 0x001E0000);
    vip_32bit_tmp[1] =  vip_8bit_tmp[7];
    vip_32bit_tmp[1] = (vip_32bit_tmp[1]<<8) + vip_8bit_tmp[0];
    vip_32bit_tmp[1] = (vip_32bit_tmp[1]<<8) + vip_8bit_tmp[1];
#else
    vip_32bit_tmp[0] = rtd_inl(DM_UZU_PEAKING_CORING_0x162c);
    *((UINT8*)&vip_32bit_tmp[0]) = 0;
    *((UINT8*)&vip_32bit_tmp[0] + 1) &= 0x1e;
    *((UINT8*)&vip_32bit_tmp[0] + 2) = 0;
    *((UINT8*)&vip_32bit_tmp[0] + 3) = 0;

    *((UINT8*)&vip_32bit_tmp[1]) = 0;
    *((UINT8*)&vip_32bit_tmp[1] + 1) =  vip_8bit_tmp[7];
    *((UINT8*)&vip_32bit_tmp[1] + 2) =  vip_8bit_tmp[0];
    *((UINT8*)&vip_32bit_tmp[1] + 3) =  vip_8bit_tmp[1];
#endif
    rtd_outl(DM_UZU_PEAKING_CORING_0x162c, (vip_32bit_tmp[0] | vip_32bit_tmp[1]) );

//	rtdf_maskl( DM_UZU_PEAKING_GAIN_0x1630, ~(0x00ff0000), (vip_8bit_tmp[4])<<16);//ndOffset;
//	rtdf_maskl( DM_UZU_PEAKING_GAIN_0x1630, ~(0x0000f000), (vip_8bit_tmp[5]&0xf)<<12);//ndLmtP;
//	rtdf_maskl( DM_UZU_PEAKING_GAIN_0x1630, ~(0x00000f00), (vip_8bit_tmp[6]&0xf)<<8);//ndLmtN;
//	rtdf_maskl( DM_UZU_PEAKING_GAIN_0x1630, ~(0x000000f0), (vip_8bit_tmp[2]&0xf)<<4);//ndG1;
//	rtdf_maskl( DM_UZU_PEAKING_GAIN_0x1630, ~(0x0000000f), (vip_8bit_tmp[3]&0xf));//ndG2;
#if 0
    vip_32bit_tmp[0] = vip_8bit_tmp[4];
    vip_32bit_tmp[0] = (vip_32bit_tmp[0]<<4) + vip_8bit_tmp[5];
    vip_32bit_tmp[0] = (vip_32bit_tmp[0]<<4) + vip_8bit_tmp[6];
    vip_32bit_tmp[0] = (vip_32bit_tmp[0]<<4) + vip_8bit_tmp[2];
    vip_32bit_tmp[0] = (vip_32bit_tmp[0]<<4) + vip_8bit_tmp[3];
#else
    *((UINT8*)&vip_32bit_tmp[0]) = 0;
    *((UINT8*)&vip_32bit_tmp[0] + 1) = vip_8bit_tmp[4];
    vip_8bit_tmp[7] = (vip_8bit_tmp[5] & 0x0f) << 4 | vip_8bit_tmp[6];
    *((UINT8*)&vip_32bit_tmp[0] + 2) = vip_8bit_tmp[7];
    vip_8bit_tmp[7] = (vip_8bit_tmp[2] & 0x0f) << 4 | vip_8bit_tmp[3];
    *((UINT8*)&vip_32bit_tmp[0] + 3) = vip_8bit_tmp[7];
#endif
    rtd_outl(DM_UZU_PEAKING_GAIN_0x1630, vip_32bit_tmp[0]);
}

void drvif_color_supk_mask( UINT8 type)
{
    if (type>3)//0: Pulse(original)	 1:vertical, 2:horizontal, 3:diagonal
    {
        RTD_Log(LOGGER_INFO, "\nVIP supk_mask error => return!\n");
        return;
    }
#if 0
    vip_32bit_tmp[0] = type;
    rtd_maskl(DM_UZU_PEAKING_CORING_0x162c, ~(0x00060000), (vip_32bit_tmp[0]<<17));
#else
    *((UINT8*)&vip_32bit_tmp[0]) = 0;
    *((UINT8*)&vip_32bit_tmp[0] + 1) = (type << 1);
    *((UINT8*)&vip_32bit_tmp[0] + 2) = 0;
    *((UINT8*)&vip_32bit_tmp[0] + 3) = 0;
    vip_32bit_tmp[0] = type;
    rtd_maskl(DM_UZU_PEAKING_CORING_0x162c, ~(0x00060000), vip_32bit_tmp[0]);
#endif
}

void drvif_color_sunr_middleware( UINT8 which_source, UINT8 *level_y, UINT8 *level_c)
{
    VIP_QUALITY_COEF code *VIP_Quality_Coef;
//	RTD_Log(LOGGER_INFO, "VIP: sunr_middleware > which_source=%d\n", (UINT32)which_source);

    if ( drvif_color_VIP_Quality_Coef_GetValue( which_source, &VIP_Quality_Coef) ==_FALSE)
    {
        RTD_Log(LOGGER_INFO, "VIP: drvif_color_sunr_middleware=> input src idx error!!\n");
        return;
    }
    else
    {
        /*VIP_Quality_Coef->SUNR_Y=(*level_y);
        VIP_Quality_Coef->SUNR_C=(*level_c);*/

        RTD_Log(LOGGER_INFO, "VIP: sunr_middleware out level_y=%d, level_y=%d\n", (UINT32)(*level_y), (UINT32)(*level_c) );
    }


}

void drvif_color_sunr_y( UINT8 level)
{
//	RTD_Log(LOGGER_INFO,"\ sunr_y level==>%d\n",(UINT32)level);

    rtd_maskl(DM_UZU_CP_CTRL_0x1650, ~(0x00000700), (0x7)<<8);//SResultWeight = 7

    if (level==0)
        rtdf_clearBits(DM_UZU_CP_CTRL_0x1650, _BIT1);	//ich1_CP_Ctrl_reg.cp_spatialenabley = 0x0;
    else
        rtdf_setBits(DM_UZU_CP_CTRL_0x1650, _BIT1);	//ich1_CP_Ctrl_reg.cp_spatialenabley = 0x1;

    //th3>th2>th1
    vip_8bit_tmp[0] = ( (level)>255 )?255:(level);
    vip_8bit_tmp[1] = ( (level+1)>255 )?255:(level+1);
    vip_8bit_tmp[2] = ( (level+2)>255 )?255:(level+2);

    RTD_Log(LOGGER_INFO, "VIP: Y th1=%d, th2=%d, th3=%d\n\n",(UINT32)vip_8bit_tmp[0],(UINT32)vip_8bit_tmp[1],(UINT32)vip_8bit_tmp[2]);
#if 0
    vip_32bit_tmp[0] = vip_8bit_tmp[2];
    vip_32bit_tmp[0] = (vip_32bit_tmp[0] << 8) + vip_8bit_tmp[1];
    vip_32bit_tmp[0] = (vip_32bit_tmp[0] << 8) + vip_8bit_tmp[0];
#else
    *((UINT8*)&vip_32bit_tmp[0]) = 0;
    *((UINT8*)&vip_32bit_tmp[0] + 1) = vip_8bit_tmp[2];
    *((UINT8*)&vip_32bit_tmp[0] + 2) = vip_8bit_tmp[1];
    *((UINT8*)&vip_32bit_tmp[0] + 3) = vip_8bit_tmp[0];
#endif
    rtd_outl(DM_UZU_CP_THL_Y_0x1654, vip_32bit_tmp[0]);

}

void drvif_color_sunr_c( UINT8 level)
{
//	RTD_Log(LOGGER_INFO,"\ sunr_c level==>%d\n",(UINT32)level);

    rtd_maskl(DM_UZU_CP_CTRL_0x1650, ~(0x00000700), (0x7)<<8);//SResultWeight = 7

    if (level==0)
        rtdf_clearBits(DM_UZU_CP_CTRL_0x1650, _BIT0);	//ich1_CP_Ctrl_reg.cp_spatialenabley = 0x0;
    else
        rtdf_setBits(DM_UZU_CP_CTRL_0x1650, _BIT0);	//ich1_CP_Ctrl_reg.cp_spatialenabley = 0x1;

    //th3>th2>th1
    vip_8bit_tmp[0] = 0;//Sparrow HW restriction, SpatialThlC_th1 must be 0.
    vip_8bit_tmp[1] = ( (level+2)>255 )?255:(level+2);
    vip_8bit_tmp[2] = ( (level+4)>255 )?255:(level+4);

    RTD_Log(LOGGER_INFO, "VIP: C th1=%d, th2=%d, th3=%d\n\n",(UINT32)vip_8bit_tmp[0],(UINT32)vip_8bit_tmp[1],(UINT32)vip_8bit_tmp[2]);
#if 0
    vip_32bit_tmp[0] = vip_8bit_tmp[2];
    vip_32bit_tmp[0] = (vip_32bit_tmp[0] << 8) + vip_8bit_tmp[1];
    vip_32bit_tmp[0] = (vip_32bit_tmp[0] << 8) + vip_8bit_tmp[0];
#else
    *((UINT8*)&vip_32bit_tmp[0]) = 0;
    *((UINT8*)&vip_32bit_tmp[0] + 1) = vip_8bit_tmp[2];
    *((UINT8*)&vip_32bit_tmp[0] + 2) = vip_8bit_tmp[1];
    *((UINT8*)&vip_32bit_tmp[0] + 3) = vip_8bit_tmp[0];
#endif
    rtd_outl(DM_UZU_CP_THL_C_0x1658, vip_32bit_tmp[0]);

}
/*======================== End of File =======================================*/
/**
*
* @}
*/


