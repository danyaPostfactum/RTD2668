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

#include "sys_info.h"
#include "flow_conf_struct.h"

char code compiler_version_info[] = SYS_COMPILER_VERSION;
char code compiler_date_info[] = SYS_COMPILER_DATE;
char code compiler_clock_info[] = SYS_COMPILER_CLOCK;
char code compiler_username_info[] = SYS_COMPILER_USERNAME;

STRUCT_INFORMATION_ELEMENT code stCompilerVersion =
{
    sizeof(compiler_version_info), compiler_version_info
};

STRUCT_INFORMATION_ELEMENT code stCompilerDateInfo =
{
    sizeof(compiler_date_info), compiler_date_info
};

STRUCT_INFORMATION_ELEMENT code stCompilerClock =
{
    sizeof(compiler_clock_info), compiler_clock_info
};

STRUCT_INFORMATION_ELEMENT code stCompilerUsername =
{
    sizeof(compiler_username_info), compiler_username_info
};


