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

#ifndef  _IRRC_PARAMETER_H_
#define  _IRRC_PARAMETER_H_
#include "sparrow.h"
#include "autoconf.h"

#if defined(CONFIG_IRDA_PROTOCOL_NEC)
//  IR_PSR Reg:0x2880
#define DEFAULT_IRRBL	0x4eUL
#define DEFAULT_IRRML	0x13UL
#define DEFAULT_IRRDZL	0x0eUL//0x13UL jokerxie modified
#define DEFAULT_IRRDOL	0x3bUL

// IR_PER Reg:0x2884
#define DEFAULT_IRRDL	0x01UL
#define DEFAULT_IRRRL	0x13UL
#define DEFAULT_IRRSL	0x27UL

// IR_SF Reg:0x2888
#define DEFAULT_IRIT	0xffUL
#define DEFAULT_IROCT	0xffUL
#define IRFD				0x021bUL

// IR_CR  Reg:0x2980
#define DEFAULT_IREDNM	0UL
#define DEFAULT_IREDN	0UL
#define DEFAULT_MLAE	0UL
#define DEFAULT_IRRES	0UL
#define DEFAULT_IRCM	1UL
#define DEFAULT_IRBME	1UL
#define DEFAULT_IRDPM	0UL
#define DEFAULT_IRDN	0x1fUL

// IR_SR	Reg:0x2898
#define DEFAULT_IRRF	0x01UL
#elif defined(CONFIG_IRDA_PROTOCOL_RC5)
//  IR_PSR Reg:0x2880
#define DEFAULT_IRRBL	0x23
#define DEFAULT_IRRML	0x23
#define DEFAULT_IRRDZL	0x23
#define DEFAULT_IRRDOL	0x23

// IR_PER Reg:0x2884
#define DEFAULT_IRRDL	0x07
#define DEFAULT_IRRRL	0x00
#define DEFAULT_IRRSL	0x00

// IR_SF Reg:0x2888
#define DEFAULT_IRIT	0xff
#define DEFAULT_IROCT	0xff
#define IRFD				0x00c7UL

// IR_CR  Reg:0x2980
#define DEFAULT_IREDNM	0
#define DEFAULT_IREDN	0
#define DEFAULT_MLAE	0
#define DEFAULT_IRRES	0
#define DEFAULT_IRCM	1
#define DEFAULT_IRBME	1
#define DEFAULT_IRDPM	0
#define DEFAULT_IRDN	0x31

// IR_SR	Reg:0x2898
#define DEFAULT_IRRF	0x00

#elif defined(CONFIG_IRDA_PROTOCOL_Panasonic)
//  IR_PSR Reg:0x2880
#define DEFAULT_IRRBL	0x20
#define DEFAULT_IRRML	0x11
#define DEFAULT_IRRDZL	0x11
#define DEFAULT_IRRDOL	0x3c

// IR_PER Reg:0x2884
#define DEFAULT_IRRDL	0x07
#define DEFAULT_IRRRL	0x00
#define DEFAULT_IRRSL	0x09

// IR_SF Reg:0x2888
#define DEFAULT_IRIT	0xff
#define DEFAULT_IROCT	0xff
#define IRFD				0x00c7

// IR_CR  Reg:0x2980
#define DEFAULT_IREDNM	0
#define DEFAULT_IREDN	0
#define DEFAULT_MLAE	0
#define DEFAULT_IRRES	0
#define DEFAULT_IRCM	1
#define DEFAULT_IRBME	1
#define DEFAULT_IRDPM	0
#define DEFAULT_IRDN	0x31

// IR_SR	Reg:0x2898
#define DEFAULT_IRRF	0x00

#elif defined(CONFIG_IRDA_PROTOCOL_Sony)
//  IR_PSR Reg:0x2880
#define DEFAULT_IRRBL	0x17
#define DEFAULT_IRRML	0x15
#define DEFAULT_IRRDZL	0x15
#define DEFAULT_IRRDOL	0x2f

// IR_PER Reg:0x2884
#define DEFAULT_IRRDL	0x07
#define DEFAULT_IRRRL	0x00
#define DEFAULT_IRRSL	0x06

// IR_SF Reg:0x2888
#define DEFAULT_IRIT	0xff
#define DEFAULT_IROCT	0xff
#define IRFD				0x00c7

// IR_CR  Reg:0x2980
#define DEFAULT_IREDNM	0
#define DEFAULT_IREDN	0
#define DEFAULT_MLAE	0
#define DEFAULT_IRRES	0
#define DEFAULT_IRCM	1
#define DEFAULT_IRBME	1
#define DEFAULT_IRDPM	0
#define DEFAULT_IRDN	0x31

// IR_SR	Reg:0x2898
#define DEFAULT_IRRF	0x00

#elif defined(CONFIG_IRDA_PROTOCOL_Sharp_48_bit)
//  IR_PSR Reg:0x2880
#define DEFAULT_IRRBL	0x4e
#define DEFAULT_IRRML	0x13
#define DEFAULT_IRRDZL	0x13
#define DEFAULT_IRRDOL	0x3b

// IR_PER Reg:0x2884
#define DEFAULT_IRRDL	0x01
#define DEFAULT_IRRRL	0x13
#define DEFAULT_IRRSL	0x27

// IR_SF Reg:0x2888
#define DEFAULT_IRIT	0xff
#define DEFAULT_IROCT	0xff
#define IRFD				0x00c7

// IR_CR  Reg:0x2980
#define DEFAULT_IREDNM	0
#define DEFAULT_IREDN	0
#define DEFAULT_MLAE	0
#define DEFAULT_IRRES	0
#define DEFAULT_IRCM	1
#define DEFAULT_IRBME	1
#define DEFAULT_IRDPM	0
#define DEFAULT_IRDN	0x31

// IR_SR	Reg:0x2898
#define DEFAULT_IRRF	0x01

#elif defined(CONFIG_IRDA_PROTOCOL_Skyworth)
//  IR_PSR Reg:0x2880
#define DEFAULT_IRRBL	0x30
#define DEFAULT_IRRML	0x1a
#define DEFAULT_IRRDZL	0x16
#define DEFAULT_IRRDOL	0x3f

// IR_PER Reg:0x2884
#define DEFAULT_IRRDL	0x07
#define DEFAULT_IRRRL	0x30
#define DEFAULT_IRRSL	0x30

// IR_SF Reg:0x2888
#define DEFAULT_IRIT	0xff
#define DEFAULT_IROCT	0xff
#define IRFD				0x00c7

// IR_CR  Reg:0x2980
#define DEFAULT_IREDNM	0
#define DEFAULT_IREDN	0
#define DEFAULT_MLAE	0
#define DEFAULT_IRRES	0
#define DEFAULT_IRCM	1
#define DEFAULT_IRBME	1
#define DEFAULT_IRDPM	0
#define DEFAULT_IRDN	0x31

// IR_SR	Reg:0x2898
#define DEFAULT_IRRF	0x01

#elif defined(IRDA_PROTOCOL_Customerize)
//  IR_PSR Reg:0x2880
#define DEFAULT_IRRBL	0x4e
#define DEFAULT_IRRML	0x13
#define DEFAULT_IRRDZL	0x13
#define DEFAULT_IRRDOL	0x3b

// IR_PER Reg:0x2884
#define DEFAULT_IRRDL	0x01
#define DEFAULT_IRRRL	0x13
#define DEFAULT_IRRSL	0x27

// IR_SF Reg:0x2888
#define DEFAULT_IRIT	0xff
#define DEFAULT_IROCT	0xff
#define IRFD				0x00c7

// IR_CR  Reg:0x2980
#define DEFAULT_IREDNM	0
#define DEFAULT_IREDN	0
#define DEFAULT_MLAE	0
#define DEFAULT_IRRES	0
#define DEFAULT_IRCM	1
#define DEFAULT_IRBME	1
#define DEFAULT_IRDPM	0
#define DEFAULT_IRDN	0x31

// IR_SR	Reg:0x2898
#define DEFAULT_IRRF	0x01
#else
//  IR_PSR Reg:0x2880
#define DEFAULT_IRRBL	0x4e
#define DEFAULT_IRRML	0x13
#define DEFAULT_IRRDZL	0x13
#define DEFAULT_IRRDOL	0x3b

// IR_PER Reg:0x2884
#define DEFAULT_IRRDL	0x01
#define DEFAULT_IRRRL	0x13
#define DEFAULT_IRRSL	0x27

// IR_SF Reg:0x2888
#define DEFAULT_IRIT	0xff
#define DEFAULT_IROCT	0xff
#define IRFD				0x00c7

// IR_CR  Reg:0x2980
#define DEFAULT_IREDNM	0
#define DEFAULT_IREDN	0
#define DEFAULT_MLAE	0
#define DEFAULT_IRRES	0
#define DEFAULT_IRCM	1
#define DEFAULT_IRBME	1
#define DEFAULT_IRDPM	0
#define DEFAULT_IRDN	0x31

// IR_SR	Reg:0x2898
#define DEFAULT_IRRF	0x01

#endif
#endif // #ifndef  _IRRC_PARAMETER_H_
