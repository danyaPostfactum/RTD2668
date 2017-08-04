/*===========================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2009
 * All rights reserved.
 * ========================================================== */

/*==================== File Description ===================== */
/**
 * @file
 * 	This file is customer struct data for ap.
 *
 * @author 	$Author: cf.tsai $
 * @date 	$Date: 2009-03-16 $
 * @version $Revision: 10 $
 */

//----------------------------------------------------------------------------------------------------
// ID Code      : Struct.h No.0000
// Update Note  :
//
//----------------------------------------------------------------------------------------------------
//#include "Core\Header\Maindef.h"
//--------------------------------------------------
// Struct Type
//--------------------------------------------------
#ifndef _CNI_TABLE_H
#define _CNI_TABLE_H


typedef struct _cni_network_table
{
    UINT16 ats_ranking;		// for ATS
    UINT16 format1_ni;		// 8/30 format 1 NI codes
    UINT16 format2_cni;		// 8/30 format 2 CNI codes
    UINT8 name[6];			// 5­Ó¦r¤¸
} CNI_NETWORK_TABLE;

typedef struct _cni_gloabl_table
{
    CNI_NETWORK_TABLE *pTable;
    UINT8 count;
} CNI_GLOBAL_TABLE;

UINT8 CGetNetworkInfo(UINT8* pChnName);
void drv_CNI_Init(void);

#if !defined(_CNI_TABLE_C)

#endif // !defined(_CNI_TABLE_C)


#endif//_STRUCT_CUSTOMER_H

