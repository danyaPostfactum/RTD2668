/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/
/*****************************************************************************\
 * @file     stdint.h
 * @brief    Implements ISO C99: 7.18 integer types to extend the KeilC C51 compiler.
 * @details  This header shall declare sets of integer types having specified
 *              widths, and shall  define  corresponding  sets of macros. It
 *              shall also define macros that specify limits of integer types
 *              corresponding to types defined in other standard headers.
 * @author   Janus Cheng
 * @date     Jan. 9, 2009
\*****************************************************************************/


#ifndef _KEILC_C51_STDINT_H
#define _KEILC_C51_STDINT_H    (1)


/* Exact integral types.  */

/* Signed.  */

/* There is some amount of overlap with <sys/types.h> as known by inet code */
#ifndef __int8_t_defined
#define __int8_t_defined
typedef signed char         int8_t;
typedef signed int          int16_t;
typedef signed long         int32_t;
#endif

/* Unsigned.  */
typedef unsigned char       uint8_t;
typedef unsigned int        uint16_t;
#ifndef __uint32_t_defined
typedef unsigned long       uint32_t;
#define __uint32_t_defined
#endif


/* Small types.  */

/* Signed.  */
typedef signed char         int_least8_t;
typedef signed int          int_least16_t;
typedef signed long         int_least32_t;

/* Unsigned.  */
typedef unsigned char       uint_least8_t;
typedef unsigned int        uint_least16_t;
typedef unsigned long       uint_least32_t;


/* Fast types.  */

/* Signed.  */
typedef signed char         int_fast8_t;
typedef signed int          int_fast16_t;
typedef signed long         int_fast32_t;

/* Unsigned.  */
typedef unsigned char       uint_fast8_t;
typedef unsigned int        uint_fast16_t;
typedef unsigned long       uint_fast32_t;


/* Types for `void *' pointers.  */
#ifndef __intptr_t_defined
typedef int                 intptr_t;
#define __intptr_t_defined
#endif
typedef unsigned int        uintptr_t;


/* Largest integral types.  */
typedef signed   long int   intmax_t;
typedef unsigned long int   uintmax_t;


/* The ISO C99 standard specifies that in C++ implementations these
   macros should only be defined if explicitly requested.  */
#if !defined __cplusplus || defined __STDC_LIMIT_MACROS


/* Limits of integral types.  */

/* Minimum of signed integral types.  */
#define INT8_MIN          (-128)
#define INT16_MIN         (-32767-1)
#define INT32_MIN         (-2147483647L-1)
/* Maximum of signed integral types.  */
#define INT8_MAX          (127)
#define INT16_MAX         (32767)
#define INT32_MAX         (2147483647L)

/* Maximum of unsigned integral types.  */
#define UINT8_MAX         (255)
#define UINT16_MAX        (65535)
#define UINT32_MAX        (4294967295UL)


/* Minimum of signed integral types having a minimum size.  */
#define INT_LEAST8_MIN    (-128)
#define INT_LEAST16_MIN   (-32767-1)
#define INT_LEAST32_MIN   (-2147483647L-1)
/* Maximum of signed integral types having a minimum size.  */
#define INT_LEAST8_MAX    (127)
#define INT_LEAST16_MAX   (32767)
#define INT_LEAST32_MAX   (2147483647L)

/* Maximum of unsigned integral types having a minimum size.  */
#define UINT_LEAST8_MAX   (255)
#define UINT_LEAST16_MAX  (65535)
#define UINT_LEAST32_MAX  (4294967295UL)


/* Minimum of fast signed integral types having a minimum size.  */
#define INT_FAST8_MIN     (-128)
#define INT_FAST16_MIN    (-32767-1)
#define INT_FAST32_MIN    (-2147483647L-1)
/* Maximum of fast signed integral types having a minimum size.  */
#define INT_FAST8_MAX     (127)
#define INT_FAST16_MAX    (32767)
#define INT_FAST32_MAX    (2147483647L)

/* Maximum of fast unsigned integral types having a minimum size.  */
#define UINT_FAST8_MAX    (255)
#define UINT_FAST16_MAX   (65535)
#define UINT_FAST32_MAX   (4294967295UL)

/* Values to test for integral types holding `void *' pointer.  */
#define INTPTR_MIN        (-32767-1)
#define INTPTR_MAX        (32767)
#define UINTPTR_MAX       (65535)


/* Minimum for largest signed integral type.  */
#define INTMAX_MIN        (-2147483647L-1)
/* Maximum for largest signed integral type.  */
#define INTMAX_MAX        (2147483647L)

/* Maximum for largest unsigned integral type.  */
#define UINTMAX_MAX       (4294967295UL)


/* Limits of other integer types.  */


/* Limit of `size_t' type.  */
#define SIZE_MAX          (65535)


#endif    /* C++ && limit macros */


/* The ISO C99 standard specifies that in C++ implementations these
   should only be defined if explicitly requested.  */
#if !defined __cplusplus || defined __STDC_CONSTANT_MACROS

/* Signed.  */
#define INT8_C(c)       c
#define INT16_C(c)      c
#define INT32_C(c)      c ## L

/* Unsigned.  */
#define UINT8_C(c)      c
#define UINT16_C(c)     c ## U
#define UINT32_C(c)     c ## UL

/* Maximal type.  */
#define INTMAX_C(c)     c ## L
#define UINTMAX_C(c)    c ## UL

#endif    /* C++ && constant macros */

#endif /* !_KEILC_C51_STDINT_H */

/*---------------------------------------------------------------------------*\
 * end of stdint.h                                                           *
\*---------------------------------------------------------------------------*/
