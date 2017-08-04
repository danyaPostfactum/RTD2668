
/*************************************************************************************
*                                 Realtek's ?? Project
*
*    	  		 			  Module: Printf through IO device
*	 		 				(c) Copyright 2001-2007, Realtek Corp.
*                                      All Rights Reserved
*
*                                            V???
*
* File : ROSPrintf.c
*************************************************************************************
*/
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
GENERAL DESCRIPTION
     User printf code through IO device

     The functions in this file are used when enable the option
	 RTDBG_HAL_DEBUG_GDB_INCLUDE_STUBS. It impliments remote printf
	 to user's monitor (maybe Hyperterminal, RCL tool).


Copyright (c) 2001-2003 by Realtek Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

when           who         what, where, why
--------      -----      ----------------------------------------------------------
04/05/2004    Rinoa       Create this file.

===========================================================================*/


/*===========================================================================

                       INCLUDE FILES FOR MODULE

===========================================================================*/
#include "rosprintf.h"
#include "flow_conf_struct.h"

extern volatile UINT8 fEnableLog;
void	fw_rosprintf_para_Init()
{
    fEnableLog = stflowbData.fEnable_Output_Log;
}
