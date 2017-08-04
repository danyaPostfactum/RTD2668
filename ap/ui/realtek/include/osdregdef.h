/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2009
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is for rtdaccess functions.
 *
 * @author	$Author$
 * @date	$Date$
 * @version	$Revision$
 * @ingroup	System
 */

/**
* @addtogroup system
* @{
*/


#ifndef _OSD_DEF_H_
#define _OSD_DEF_H_

#include "rtd_types.h"
#include "reg_def.h"

#if 0
#define rtdf_writeSRAMByte0(offset,val) rtd_outl(offset,(rtd_inl(OSDFONT_OSD_F_BIST_VADDR) & 0xffffff00) | (val & 0x000000ff))
#define rtdf_writeSRAMByte1(offset,val)  rtd_outl(offset, (rtd_inl(OSDFONT_OSD_F_BIST_VADDR) & 0xffff00ff) | ((val & 0x000000ff) << 8))
#define rtdf_writeSRAMByte2(offset,val)  rtd_outl(offset, (rtd_inl(OSDFONT_OSD_F_BIST_VADDR) & 0xff00ffff) | ((val & 0x000000ff) << 16))
#define rtdf_writeSRAMByte3(offset,val)  rtd_outl(offset, (rtd_inl(OSDFONT_OSD_F_BIST_VADDR) & 0x00ffffff) | ((val & 0x000000ff) << 24))
#endif

/*===========================================================================
 *
 *	UI OSD Register Address
 *
 */
//#define UIOSD_OSD_ADDR_MSB			(0x0090)
//#define UIOSD_OSD_ADDR_LSB			(0x0091)
//#define UIOSD_OSD_DATA_PORT		(0x0092)
//#define UIOSD_OSD_SCRAMBLE			(0x0093)
//#define UIOSD_OSD_TEST				(0x0094)


#if 0
#define COLOR_LUT_PORT				(0x0000)
#define OVERLAY_LUT_ADDR			(0x0000)
#define BGND_COLOR_CTRL			(0x0000)
#define OVERLAY_CTRL				(0x0000)
#endif


#endif		//_OSD_DEF_H_
