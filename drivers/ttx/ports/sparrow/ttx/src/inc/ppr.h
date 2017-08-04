/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ppr.h
 * @brief    Internal (package scope) Ttx/C interface.
 * @details  This header declares the platform-dependent interface for this ttx library.
 * @author   Janus Cheng
 * @date     Dec. 21, 2007
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/

/**
 * @addtogroup ttx
 * @{
 */
#ifndef __REALTEK_SPARROW_VERTICAL_BLANKING_INTERVAL_PRE_PROCESSOR_DRIVER_HEADER_INCLUDED__
#define __REALTEK_SPARROW_VERTICAL_BLANKING_INTERVAL_PRE_PROCESSOR_DRIVER_HEADER_INCLUDED__ (1)

#include <stdlib.h>     /* Provides: size_t */

/* -- CCL headers -----------------------------------------------------------*/
#include <ppr_hal.h>
#include <ttx_pkg.h>
#include <ttx_log.h>    /* Provides: TTX_Warning() */
#include <ttx_assert.h> /* Provides: TTX_REQUIRE() and TTX_ENSURE() */
#include <ttx_std.h>    /* Provides: TTX_STD_MagazineNum_t */

#ifdef __cplusplus
extern "C"
{
#endif

    /* -- Global Macro Definitions ----------------------------------------------*/
#ifdef __KEIL__  /* KeilC 8051 compiler */
#define PPR_SOFTWARE_EMULATOR           (0)     // Define (1) to use software PPR; (0) to use hardware PPR.
#else
#define PPR_SOFTWARE_EMULATOR           (1)     // Define (1) to use software PPR; (0) to use hardware PPR.
#endif

#if !PPR_SOFTWARE_EMULATOR
    /* Same register read/write functions as CCL */
#define PPR_RegRead(addr)               _PPR_REG_READ(addr)
#define PPR_RegWrite(addr,val)          _PPR_REG_WRITE(addr,val)
#endif  /* end of #if !PPR_SOFTWARE_EMULATOR */

    /* PPR Log functions */
#define PPR_Log                         TTX_Log

    /* PPR Assertion functions */

#define PPR_REQUIRE(_test)              TTX_REQUIRE(_test)
#define PPR_ENSURE(_test)               TTX_ENSURE(_test)
#define PPR_ALLEGE(_test)               TTX_ALLEGE(_test)
#define PPR_ASSERT_COMPILE(_test)       TTX_ASSERT_COMPILE(_test)
#define PPR_ERROR()                     TTX_ERROR()
    /*
    #define PPR_REQUIRE(_test)              //
    #define PPR_ENSURE(_test)               //
    #define PPR_ALLEGE(_test)               //
    #define PPR_ASSERT_COMPILE(_test)       //
    #define PPR_ERROR()                     //
    */

    /* PPR Constants */
#define PPR_FIELD_BUFF_PKT_NUM          (18)    // Total number of packets in field buffer
#define PPR_FIELD_BUFF_PKT_SIZE         (40)    // number of bytes of each packets in field buffer
#define PPR_PAGE_DATA_CORRECT_COUNT	    (1)     // clone of VBIPPR_PAGE_DATA_CORRECT_COUNT

    /* PPR_open() flags */
    typedef enum
    {
        PPR_OPEN_NOP   = 0x00000000u,
        PPR_OPEN_RESET = 0x00000001u
    } PPR_OpenFlag_t;

    typedef enum
    {
        PPR_PFE_MODE0 = _BIT2,
        PPR_PFE_MODE1 = _BIT4,
        PPR_PFE_MODE2 = _BIT6,
        PPR_PFE_MODE3 = _BIT8
    } PPR_PfeMode_t;


    /* -- Global typedef Declarations -------------------------------------------*/
    typedef uint32_t           PPR_Data_t;     /* width of data bus of OSD */

#define PPR_PTNGEN_PKT_LEN_BYTES   (47)    // Pattern Generator data length in bytes (including preamble bytes)


    /* -- Function Prototypes ---------------------------------------------------*/

    bool_t                  PPR_Open                        (const PPR_OpenFlag_t flags);
    bool_t                  PPR_Close                       (void);
    void                    PPR_Reset                       (void);
    void                    PPR_TestPwrOnDefaults           (void);

    bool_t                  PPR_Test                        (void);

    void                  PPR_PfeMove                     (PPR_PfeMode_t mode, uint8_t* fetched_line);

    bool_t                  PPR_PfeSetDestMag               (TTX_STD_MagazineNum_t mag, const void TTX_MEM* pDest);
    bool_t                  PPR_PfeGetDestMag               (TTX_STD_MagazineNum_t mag, void TTX_MEM** pDest);
    void                  PPR_PfeSetMag(TTX_STD_MagazineNum_t mag, uint8_t avtive);

    bool_t                  PPR_PfeSetDestM29               (const void TTX_MEM* pDest);
    bool_t                  PPR_PfeActivateM29              (void);
    bool_t                  PPR_PfeDeactivateM29            (void);

    void                    PPR_PgEnable                    (void);
    void                    PPR_PgDisable                   (void);
    size_t                  PPR_PgGenerateOneField          (const uint8_t pkts[][PPR_PTNGEN_PKT_LEN_BYTES], size_t numOfPkts);

    bool_t                  PPR_FieldBuffIsValid            (void);

    /* Debug routines */
    void                    PPR_DumpFieldBuff               (void);

#ifdef __cplusplus
}   /* extern "C" */
#endif



#endif /* !__REALTEK_SPARROW_VERTICAL_BLANKING_INTERVAL_PRE_PROCESSOR_DRIVER_HEADER_INCLUDED__ */

/**
 * @}
 */

/*---------------------------------------------------------------------------*\
 * end of ppr.h                                                              *
\*---------------------------------------------------------------------------*/
