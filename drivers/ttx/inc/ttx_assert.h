/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_assert.h
 * @brief    Internal (package scope) Ttx/C interface.
 * @details  This header declares the platform-dependent interface for this ttx library.
 * @author   Janus Cheng
 * @date     Jul. 22, 2009
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/

#ifndef __ENHANCED_TELETEXT_DECODER_LIBRARY_DEBUG_ASSERTION_HEADER_INCLUDED__
#define __ENHANCED_TELETEXT_DECODER_LIBRARY_DEBUG_ASSERTION_HEADER_INCLUDED__   (1)

#define TTX_NASSERT
//#define TTX_NASSERT (1) /* #define this macro to disable error checking */

/**
* \file
* \ingroup TTX
* \brief Customizable TTX assertions.
*
* Defines customizable and memory-efficient assertions applicable to
* embedded systems. This header file can be used in C, C++, and mixed C/C++
* programs.
*
* \note The preprocessor switch TTX_NASSERT disables checking assertions.
* In particular macros \ref TTX_ASSERT, \ref TTX_REQUIRE, \ref TTX_ENSURE,
* \ref TTX_INVARIANT, and \ref TTX_ERROR do NOT evaluate the test condition
* passed as the argument to these macros. One notable exception is the
* macro \ref TTX_ALLEGE, that still evaluates the test condition, but does
* not report assertion failures when the switch TTX_NASSERT is defined.
*/
#ifdef TTX_NASSERT          /* TTX_NASSERT defined--assertion checking disabled */
#define TTX_DEFINE_THIS_FILE
#define TTX_DEFINE_THIS_MODULE(name_)

#ifdef __KEIL__ /* Hacking "comment" of KeilC 8051 compiler to eliminate compiler warnings */
#define TTX_ASSERT(test_)    //
#define TTX_ALLEGE(test_)    //
#define TTX_ERROR()          //
#else
#define TTX_ASSERT(test_)    ((void)0)
#define TTX_ALLEGE(test_)    ((void)(test_))
#define TTX_ERROR()          ((void)0)
#endif  /* end of #ifdef __KEIL__ */

#else                  /* TTX_NASSERT not defined--assertion checking enabled */

#ifdef __cplusplus
extern "C"
{
#endif

    /** callback invoked in case the condition passed to \ref TTX_ASSERT,
    * \ref TTX_REQUIRE, \ref TTX_ENSURE, \ref TTX_ERROR, or \ref TTX_ALLEGE
    * evaluates to FALSE.
    *
    * \param file file name where the assertion failed
    * \param line line number at which the assertion failed
    */
    /*lint -sem(TTX_onAssert, r_no)                TTX_onAssert() never returns */
    void TTX_onAssert(char const TTX_ROM * const TTX_ROM_VAR file, int line);

#ifdef __cplusplus
}
#endif

/** Place this macro at the top of each C/C++ module to define the file
* name string using __FILE__ (NOTE: __FILE__ might contain lengthy path
* name). This file name will be used in reporting assertions in this file.
*/
#define TTX_DEFINE_THIS_FILE \
        static char const TTX_ROM TTX_ROM_VAR l_this_file[] = __FILE__;

/** Place this macro at the top of each C/C++ module to define the module
* name as the argument \a name_. This file name will be used in reporting
* assertions in this file.
*/
#define TTX_DEFINE_THIS_MODULE(name_) \
        static char const TTX_ROM TTX_ROM_VAR l_this_file[] = #name_;

/** General purpose assertion that makes sure the \a test_ argument is
* TRUE. Calls the TTX_onAssert() callback if the \a test_ evaluates
* to FALSE.
* \note the \a test_ is NOT evaluated if assertions are
* disabled with the TTX_NASSERT switch.
*/
#define TTX_ASSERT(test_) \
        if (test_) { \
        } \
        else (TTX_onAssert(l_this_file, __LINE__))

/** General purpose assertion that ALWAYS evaluates the \a test_
* argument and calls the TTX_onAssert() callback if the \a test_
* evaluates to FALSE.
* \note the \a test_ argument IS always evaluated even when assertions are
* disabled with the TTX_NASSERT macro. When the TTX_NASSERT macro is
* defined, the TTX_onAssert() callback is NOT called, even if the
* \a test_ evaluates to FALSE.
*/
#define TTX_ALLEGE(test_)    TTX_ASSERT(test_)

/** Assertion that always calls the TTX_onAssert() callback if
* ever executed.
* \note can be disabled with the TTX_NASSERT switch.
*/
#define TTX_ERROR() \
        (TTX_onAssert(l_this_file, __LINE__))

#endif                                                         /* TTX_NASSERT */

/** Assertion that checks for a precondition. This macro is equivalent to
* \ref TTX_ASSERT, except the name provides a better documentation of the
* intention of this assertion.
*/
#define TTX_REQUIRE(test_)   TTX_ASSERT(test_)

/** Assertion that checks for a postcondition. This macro is equivalent to
* \ref TTX_ASSERT, except the name provides a better documentation of the
* intention of this assertion.
*/
#define TTX_ENSURE(test_)    TTX_ASSERT(test_)

/** Assertion that checks for an invariant. This macro is equivalent to
* \ref TTX_ASSERT, except the name provides a better documentation of the
* intention of this assertion.
*/
#define TTX_INVARIANT(test_) TTX_ASSERT(test_)

/** Compile-time assertion exploits the fact that in C/C++ a dimension of
 * an array must be non-zero. The following declaration causes a compilation
 * error if the compile-time expression (\a test_) is not TRUE. The assertion
 * has no runtime side effects.
 */
#define TTX_ASSERT_COMPILE(test_) \
    extern char TTX_assert_compile[(test_) ? 1 : -1]

#endif  /* !__ENHANCED_TELETEXT_DECODER_LIBRARY_DEBUG_ASSERTION_HEADER_INCLUDED__ */

/*---------------------------------------------------------------------------*\
 * end of ttx_assert.h                                                       *
\*---------------------------------------------------------------------------*/

