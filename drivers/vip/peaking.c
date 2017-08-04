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
#include "vip\digital_filter.h"
#include "vip\video_quality.h"
#include "reg_def.h"
#include "mode_customer.h"
#include "rosprintf.h"
#include "Flow_scaler.h"
#include "reg52.h"
/*===================================  Types ================================*/

/*================================== Variables ==============================*/
extern UINT8 xdata vip_i;
extern UINT8 xdata vip_j;
extern UINT8 xdata vip_8bit_tmp[8];
extern UINT32 xdata vip_16bit_tmp[3];
extern UINT32 xdata vip_32bit_tmp[3];
//extern UINT16 xdata *vip_16bit_table_index;//*vip_table_index;
//extern UINT8 xdata *vip_8bit_table_index;
extern INT32 xdata vip_INT32_tmp[2];
extern INT16 xdata vip_INT16_tmp[5];
/*================================ Definitions ==============================*/
#if 0//dragon use
#define    nPxlSel              		(0)//not use in sparrow
#define    nC0                  			(1)
#define    nC1                  			(2)
#define    nC2                  			(3)
#define    nG_Blr               		(4)//not use in sparrow
#define    nG_Pos_Min           		(5)//not use in sparrow
#define    nG_Pos_Max           		(6)//not use in sparrow
#define    nG_Neg_Min           		(7)//not use in sparrow
#define    nG_Neg_Max           		(8)//not use in sparrow
#define    nHV_POS_Min          		(9)
#define    nHV_POS_Max          	(10)
#define    nHV_NEG_Min          		(11)
#define    nHV_NEG_Max          	(12)
#define    nLV_min              		(13)
#define    nLV_max              		(14)
#else
#define    nC0                  			(0)
#define    nC1                  			(1)
#define    nC2                  			(2)
#define    nG_Min           			(3)
#define    nG_Max           			(4)
#define    nHV_POS_Min          		(5)
#define    nHV_POS_Max          		(6)
#define    nHV_NEG_Min          		(7)
#define    nHV_NEG_Max          		(8)
#define    nLV_min              		(9)
#define    nLV_max              		(10)
#endif

/*================================== Function ===============================*/


/*============================================================================*/
/**
 * Peaking
 * This function is used to set Peaking registers
 *
 * @param <info> { display-info struecture }
 * @return { none }
 * @note
 * It can be linear or non-linear either, based on the non-linear flag condition:
 */

bit drvif_color_SHP_Coef_GetValue(UINT8 which_source, SHP_COEF code ** pptr)
{
    SHP_COEF code * ptr;

//	RTD_Log(LOGGER_INFO, "SHP_Coef_GetValue: which_source=%d, stSHP_Coef_Table.num=%d\n", (UINT32)which_source, (UINT32)(stSHP_Coef_Table.num));

    vip_i = 0;
    while (vip_i < stSHP_Coef_Table.num)
    {
        ptr = &stSHP_Coef_Table.ptSHP_COEF[ vip_i ];
        if (ptr->mode == which_source)
        {
            *pptr = ptr;
            break;
        }
        vip_i++;
    }

    if ( vip_i>= stSHP_Coef_Table.num ||( ptr->mode == _VGA_MODE_ALL) )// Check whether not found or VGA input
        return _FALSE;
    else
        return _TRUE;


}

INT16 drvif_color_peaking_MaxCoeff_check(INT16 c0, INT16 c1, INT16 c2)
{
//input c0, c1, c2 must be -128<= c <= 127


    vip_INT16_tmp[0] = (c0>=0)?(127 - c0):(128 + c0);
    vip_INT16_tmp[1] = (c1>=0)?(127 - c1):(128 + c1);
    vip_INT16_tmp[2] = (c2>=0)?(127 - c2):(128 + c2);


//find the mimum
    if ( vip_INT16_tmp[0] < vip_INT16_tmp[1] )
    {
        vip_8bit_tmp[0] = c0;
        vip_INT16_tmp[3] = vip_INT16_tmp[0];
    }
    else
    {
        vip_8bit_tmp[0] = c1;
        vip_INT16_tmp[3] = vip_INT16_tmp[1];
    }


    if ( vip_INT16_tmp[3] < vip_INT16_tmp[2] )
        vip_8bit_tmp[0] = vip_8bit_tmp[0];
    else
        vip_8bit_tmp[0] = c2;

    return(vip_8bit_tmp[0]);

}

void drvif_color_peaking_resolution( UINT8 enable)
{
    rtdf_pageSelect(_PAGE7);//page 7
    if (enable==0)//Y peaking Coefficient Resolution 0: n/32 1: n/64
        rtdf_clearBitsScalerPort(peaking_coring_access_port_control_PAGE7_0x00d6, _BIT5);
    else
        rtdf_setBitsScalerPort(peaking_coring_access_port_control_PAGE7_0x00d6, _BIT5);
}

void drvif_color_peaking_config(UINT8 which_source, UINT8 level)
{
    SHP_COEF code * ptSHP_Coef;

    rtdf_pageSelect(_PAGE7);//page 7

//	RTD_Log(LOGGER_INFO, "********************************\n  VIP: Peaking which_source=%d\n",(UINT32)which_source);

    if ( level == 0 )//Disable sharpness
    {
        rtdf_clearBitsScalerPort(peaking_coring_access_port_control_PAGE7_0x00d6, _BIT6);//d_vc_global_ctrl_reg.m_sharp_en=0;
        return;
    }
    else//enable sharpness
    {
        if ( drvif_color_SHP_Coef_GetValue( which_source, &ptSHP_Coef)==_FALSE )
        {
            RTD_Log(LOGGER_INFO, "VIP: peaking => VGA_in or input src idx error, disable Peaking !\n");
            rtdf_clearBitsScalerPort(peaking_coring_access_port_control_PAGE7_0x00d6, _BIT6);//Peaking/coring Enable
            return;
        }
        else
        {
            /*
            			RTD_Log(LOGGER_INFO, "------------------------------------------\n  VIP: Peaking input src idx=%d\n",(UINT32)which_source);
            			RTD_Log(LOGGER_INFO, "VIP: drvif_color_peaking_config() => level=%d\n", (UINT32)level);
            			RTD_Log(LOGGER_INFO, "VIP: mode=%d, Resolution=%d\n", (UINT32)(ptSHP_Coef->mode), (UINT32)(ptSHP_Coef->Resolution));
            			RTD_Log(LOGGER_INFO, "VIP: C0=%d, C1=%d, C2=%d\n", (UINT32)(ptSHP_Coef->C0), (UINT32)(ptSHP_Coef->C1), (UINT32)(ptSHP_Coef->C2));
            			RTD_Log(LOGGER_INFO, "VIP: C4_Blr=%d\n", (UINT32)(ptSHP_Coef->C4_Blr));
            			RTD_Log(LOGGER_INFO, "VIP: G_Pos_Max=%d, G_Pos_Min=%d\n", (UINT32)(ptSHP_Coef->G_Pos_Max), (UINT32)(ptSHP_Coef->G_Pos_Min));
            			RTD_Log(LOGGER_INFO, "VIP: G_Neg_Max=%d, G_Neg_Min=%d\n",  (UINT32)(ptSHP_Coef->G_Neg_Max), (UINT32)(ptSHP_Coef->G_Neg_Min));
            			RTD_Log(LOGGER_INFO, "VIP: HV_POS_Max=%d, HV_POS_Min=%d\n", (UINT32)(ptSHP_Coef->HV_POS_Max), (UINT32)(ptSHP_Coef->HV_POS_Min));
            			RTD_Log(LOGGER_INFO, "VIP: HV_NEG_Max=%d, HV_NEG_Min=%d\n", (UINT32)(ptSHP_Coef->HV_NEG_Max), (UINT32)(ptSHP_Coef->HV_NEG_Min));
            			RTD_Log(LOGGER_INFO, "VIP: LV_Min=%d, LV_Max=%d\n", (UINT32)(ptSHP_Coef->LV_Min), (UINT32)(ptSHP_Coef->LV_Max));
            */
            //Decide the OSD mapping ratio======================================================================

            //		vip_8bit_tmp[7] = SHP_Coef[which_source][nG_Min]+
            //				(SHP_Coef[which_source][nG_Max]-SHP_Coef[which_source][nG_Min])*level/100;
            //vip_8bit_tmp[7] = ptSHP_Coef->G_Pos_Min + ((ptSHP_Coef->G_Pos_Max-ptSHP_Coef->G_Pos_Min)*level)/100;
            vip_16bit_tmp[0] = ((ptSHP_Coef->G_Pos_Max-ptSHP_Coef->G_Pos_Min)*level);
            vip_16bit_tmp[0] = vip_16bit_tmp[0]/100;
            vip_8bit_tmp[7] = ptSHP_Coef->G_Pos_Min +vip_16bit_tmp[0];

            if (vip_8bit_tmp[7]>255)//gain is 8 bit in dragon
                vip_8bit_tmp[7]=255;

            //		vip_8bit_tmp[7]=vip_8bit_tmp[7]>>4;//Gain

            //Decide the Coefficient ratio======================================================================

            vip_INT16_tmp[4] = drvif_color_peaking_MaxCoeff_check(ptSHP_Coef->C0, ptSHP_Coef->C1, ptSHP_Coef->C2);//find the base

            if (vip_INT16_tmp[4]<0)//decide maximum
                vip_INT16_tmp[5] = 128;
            else
                vip_INT16_tmp[5] = 127;

            //==> Coefficient max ratio = maximum/base

            //Calculate the coefficient====================================================================================
#if 0
            vip_INT16_tmp[0] = (vip_INT16_tmp[5] *  ptSHP_Coef->C0 )/vip_INT16_tmp[4];
            vip_INT16_tmp[0] = ( vip_8bit_tmp[7] * vip_INT16_tmp[0] )/255;
            vip_8bit_tmp[0]= ( ( vip_INT16_tmp[0] < 0 ) ? ( vip_INT16_tmp[0] + 256 ) : vip_INT16_tmp[0] );



            vip_INT16_tmp[1] = (vip_INT16_tmp[5] *  ptSHP_Coef->C1 )/vip_INT16_tmp[4];
            vip_INT16_tmp[1] = ( vip_8bit_tmp[7] * vip_INT16_tmp[1] )/255;
            vip_8bit_tmp[1]= ( ( vip_INT16_tmp[1] < 0 ) ? ( vip_INT16_tmp[1] + 256 ) : vip_INT16_tmp[1] );

            //calculate c2
            vip_8bit_tmp[2] = (-1)*((vip_8bit_tmp[0]+vip_8bit_tmp[1]*2)>>1);
#else
            //c1
            vip_INT16_tmp[1] = (vip_INT16_tmp[5] *  ptSHP_Coef->C1 )/vip_INT16_tmp[4];
            vip_INT16_tmp[1] = ( vip_8bit_tmp[7] * vip_INT16_tmp[1] )/255;
            vip_8bit_tmp[1]= ( ( vip_INT16_tmp[1] < 0 ) ? ( vip_INT16_tmp[1] + 256 ) : vip_INT16_tmp[1] );

            //c2
            vip_INT16_tmp[0] = (vip_INT16_tmp[5] *  ptSHP_Coef->C2 )/vip_INT16_tmp[4];
            vip_INT16_tmp[0] = ( vip_8bit_tmp[7] * vip_INT16_tmp[0] )/255;
            vip_8bit_tmp[2]= ( ( vip_INT16_tmp[0] < 0 ) ? ( vip_INT16_tmp[0] + 256 ) : vip_INT16_tmp[0] );


            //calculate c0
            vip_8bit_tmp[0] = (-2)*(vip_8bit_tmp[1]+vip_8bit_tmp[2]);

#endif

#if 0
            //		vip_INT16_tmp[0]= vip_8bit_tmp[7]*SHP_Coef[which_source][nC0];
            vip_INT16_tmp[0]= vip_8bit_tmp[7] * ptSHP_Coef->C0;
            vip_8bit_tmp[0]= ( ( vip_INT16_tmp[0] < 0 ) ? ( vip_INT16_tmp[0] + 256 ) : vip_INT16_tmp[0] );

            //		vip_INT16_tmp[1] = vip_8bit_tmp[7]*SHP_Coef[which_source][nC1];
            vip_INT16_tmp[1] = vip_8bit_tmp[7] * ptSHP_Coef->C1;
            vip_8bit_tmp[1] = ( ( vip_INT16_tmp[1] < 0 ) ? ( vip_INT16_tmp[1] + 256 ) : vip_INT16_tmp[1] );

            //		vip_INT16_tmp[2] = vip_8bit_tmp[7]*SHP_Coef[which_source][nC2];
            vip_INT16_tmp[2] = vip_8bit_tmp[7] * ptSHP_Coef->C2;
            vip_8bit_tmp[2] = ( ( vip_INT16_tmp[2] < 0 ) ? ( vip_INT16_tmp[2] + 256 ) : vip_INT16_tmp[2] );
#endif



            //		vip_8bit_tmp[3]= SHP_Coef[which_source][nLV_max]-
            //			        (SHP_Coef[which_source][nLV_max]-SHP_Coef[which_source][nLV_min])*level/100;
            //		vip_8bit_tmp[3]= ptSHP_Coef->LV_Max -
            //			        (ptSHP_Coef->LV_Max - ptSHP_Coef->LV_Min) * level / 100;

            if (ptSHP_Coef->LV_Max>ptSHP_Coef->LV_Min)
                RTD_Log(LOGGER_INFO, "Peaking: LV_Max>LV_Min! Error\n");
            else
            {
                vip_16bit_tmp[0] = (ptSHP_Coef->LV_Max - ptSHP_Coef->LV_Min);
                vip_16bit_tmp[0] = vip_16bit_tmp[0] * level/100 ;
                vip_8bit_tmp[3] = ptSHP_Coef->LV_Max - vip_16bit_tmp[0];
            }



            //		vip_8bit_tmp[4]= SHP_Coef[which_source][nHV_POS_Min]+
            //			(SHP_Coef[which_source][nHV_POS_Max]-SHP_Coef[which_source][nHV_POS_Min])*level/100;
            //		vip_8bit_tmp[4]=  ptSHP_Coef->HV_POS_Min +
            //			(ptSHP_Coef->HV_POS_Max - ptSHP_Coef->HV_POS_Min)*level/100;
            vip_16bit_tmp[0] = (ptSHP_Coef->HV_POS_Max - ptSHP_Coef->HV_POS_Min);
            vip_16bit_tmp[0] = vip_16bit_tmp[0] * level/100 ;
            vip_8bit_tmp[4] = ptSHP_Coef->HV_POS_Min + vip_16bit_tmp[0];


            //		vip_8bit_tmp[5]= SHP_Coef[which_source][nHV_NEG_Min]+
            //				(SHP_Coef[which_source][nHV_NEG_Max]-SHP_Coef[which_source][nHV_NEG_Min])*level/100;
            //		vip_8bit_tmp[5]= ptSHP_Coef->HV_NEG_Min +
            //				(ptSHP_Coef->HV_NEG_Max - ptSHP_Coef->HV_NEG_Min) * level / 100;
            vip_16bit_tmp[0] = (ptSHP_Coef->HV_NEG_Max - ptSHP_Coef->HV_NEG_Min);

            vip_16bit_tmp[0] = vip_16bit_tmp[0] * level/100 ;
            vip_8bit_tmp[5]= ptSHP_Coef->HV_NEG_Min + vip_16bit_tmp[0];





            //Sparrow: Coring Maximum Negitive value should be filled by 9bit's nagative value (2's complement), althought the register is 8bit
            //range: [(0xff)~(0x00)] mapping to [(-1)~(-256)]
#if 0
            vip_8bit_tmp[5] = (vip_8bit_tmp[5]==0)å?1å:vip_8bit_tmp[5];//nagative range => -1 to -256, force 0 to be -1
            vip_8bit_tmp[5] = (256 - vip_8bit_tmp[5]);
#else
            if (!vip_8bit_tmp[5])
                vip_8bit_tmp[5] = 1;
            vip_8bit_tmp[5] = (256 - vip_8bit_tmp[5]);
#endif
            WaitFor_DEN_STOP();
            EA = 0;
            rtdf_setBitsScalerPort(peaking_coring_access_port_control_PAGE7_0x00d6, _BIT7);
            for (vip_i=0; vip_i<6; vip_i++)
            {
                //rtdf_writeScalerPort(peaking_coring_access_port_control_PAGE7_0x00d6, (0x80+vip_i));
                rtdf_maskScalerPort(peaking_coring_access_port_control_PAGE7_0x00d6, ~(0x7), (vip_i&0x7));
                rtdf_writeScalerPort(peaking_coring_data_port_PAGE7_0x00d7, vip_8bit_tmp[vip_i]);

                //RTD_Log(LOGGER_INFO, "Peaking: port[%d]=> %d\n", (UINT32)vip_i, (UINT32)vip_8bit_tmp[vip_i]);
            }
            EA = 1;

            for (vip_i=0; vip_i<6; vip_i++)
                RTD_Log(LOGGER_INFO, "Peaking: port[%d]=> %d\n", (UINT32)vip_i, (UINT32)vip_8bit_tmp[vip_i]);

            rtdf_clearBitsScalerPort(peaking_coring_access_port_control_PAGE7_0x00d6, _BIT7);

            if (ptSHP_Coef->Resolution==0)//Y peaking Coefficient Resolution 0: n/32 1: n/64
                rtdf_clearBitsScalerPort(peaking_coring_access_port_control_PAGE7_0x00d6, _BIT5);
            else
                rtdf_setBitsScalerPort(peaking_coring_access_port_control_PAGE7_0x00d6, _BIT5);

            if ( !(rtdf_readScalerPort(peaking_coring_access_port_control_PAGE7_0x00d6) & _BIT6))
            {
                RTD_Log(LOGGER_DEBUG, "\nPeaking Enable !!\n");
                rtdf_setBitsScalerPort(peaking_coring_access_port_control_PAGE7_0x00d6, _BIT6);//Peaking/coring Enable
            }

            for (vip_i=0; vip_i<3; vip_i++)
            {
                if ( (vip_INT16_tmp[vip_i]<(-128))  || (vip_INT16_tmp[vip_i]>127) )
                    RTD_Log(LOGGER_DEBUG, "\nPeaking DC Balance Fail => C[%d] error\n", vip_i);
            }

        }

    }

}



/*======================== End of File =======================================*/
/**
*
* @}
*/


