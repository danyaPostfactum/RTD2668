/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_pkg.h
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
#ifndef __ENHANCED_TELETEXT_DECODER_LIBRARY_PLATFORM_PORT_HEADER_INCLUDED__
#define __ENHANCED_TELETEXT_DECODER_LIBRARY_PLATFORM_PORT_HEADER_INCLUDED__ (1)

#include "ttx_config.h"                         /* Provides: configurations of this library. */
#include <stdint.h>                             /* Provides: int8_t, int16_t, int32_t... */
#include "sparrow.h"                            /* Provides: rtd_inl(), rtd_outl(), rtd_inb() and rtd_outb() */
#include <rosprintf.h>                          /* Provides: RTD_Log() */

#ifdef __cplusplus
extern "C"
{
#endif

    /* Platform-dependent configurations */
#define TTX_CPU_CLOCK           (UINT32_C(27000000))    /* Unit: Hz */
#define TTX_FONT_BANK           (5)                     /* which bank is TTX font placed. (0~3) */
#define TTX_USE_PERI_IO_UART    (1)                     /* Set (0) to use 8051 on-chip UART (57600 bps); (1) to use Peri-IO uart (115200 bps) */
#define TTX_USE_SDMA            (1)                     /* Set (0) to use memcpy(); Set (1) to use SPI DMA */
#define TTX_USE_MALLOC          (0)                     /* Set (0) to use static memory management; Set (1) to use dynamic memory manaagement such as malloc() and free() */
#define TTX_REENTRANT

#if TTX_RELEASE
#define TTX_VERIFY              (0)                     /* Set (1) to enable verification of TTX related IPs */
#else
#define TTX_VERIFY              (1)                     /* Set (1) to enable verification of TTX related IPs */
#endif  /* end of #if TTX_RELEASE */


#if TTX_USE_SDMA
#define TTX_USE_SDMA_NONSTOP    (0)                 /* Set (1) to enable NON-StOP feature of SPI-DMA, i.e., when MCU in IMEM, using SPI-DMA will not halt MCU */
#endif

#define TTX_SUPPORT_DVB         (0)                     /* Set (1) to support reading TTX data from Digital Video Broadcasting */


#ifdef __KEIL__ /* KeilC 8051 compiler */
#define TTX_SUPPORT_FILE    (0)                     /* Set (1) if this platform supports file operations, such as: fopen(), fread(), fwrite()... etc. */
#define TTX_SUPPORT_MT      (0)                     /* Set (1) if this platform supports multi-threads and synchronizations */
#else
#define TTX_SUPPORT_FILE    (1)                     /* Set (1) if this platform supports file operations, such as: fopen(), fread(), fwrite()... etc. */
#define TTX_SUPPORT_MT      (1)                     /* Set (1) if this platform supports multi-threads and synchronizations */
#endif  /* end of #ifdef __KEIL__ */

    /** the Boolean type for ttx library */
    typedef bit            bool_t;

#ifndef TRUE
#define                TRUE         (1)
#endif

#ifndef FALSE
#define                FALSE        (0)
#endif

    /** Improve KeilC's <stdlib.h> */
#define                EXIT_SUCCESS (0)
#define                EXIT_FAILURE (1)

    /** address bus width of platform */
    typedef uint16_t       TTX_Address_t;

    /** data width of registers */
    typedef uint32_t       TTX_RegData_t;


    /**
     *  @brief    Choose either TTX_BIG_ENDIAN or TTX_LITTLE_ENDIAN that matches
     *              your platform architecture.
     */
#define TTX_BIG_ENDIAN  /* or TTX_BIG_ENDIAN */

    /**
     *  @details  The TTX_ROM macro used in the ttx_assert.h header file allows enforcing
     *              placing the constant strings in ROM. On CPUs with the Harvard
     *              architecture (such as 8051 or the Atmel AVR), the code and data spaces
     *              are separate and are accessed through different CPU instructions.
     *              The compilers often provide specific extended keywords to designate
     *              code or data space, such as the “__code” extended keyword in the
     *              IAR 8051 compiler. To use qassert.h with a different MCU/Compiler,
     *              you need to check how to allocate constants to ROM.
     */
#define TTX_ROM         code

    /**
     *  @brief    access const objects in ROM through far pointers.
     *  @details  The macro TTX_ROM_VAR specifies the kind of the pointer to be
     *              used to access the ROM objects. An example of valid TTX_ROM_VAR
     *              macro definition is: __far (Freescale HC(S)08 compiler). Many
     *              compilers for 8-bit MCUs provide different size pointers for
     *              accessing objects in various memo-ries. Constant objects allocated
     *              in ROM often mandate the use of specific-size pointers (e.g., far
     *              pointers) to get access to ROM objects.
     */
#define TTX_ROM_VAR


    /**
     *  @brief    specify how registers are accessed.
     */
#define TTX_REG         xdata

    /**
     *  @brief    specify how memory (such as SRAM) is accessed.
     */
#define TTX_MEM         xdata


    /* @brief Platform dependent initialization */
    void            TTX_PlatInit            (void);

    /* @brief Platform dependent codes for enabling interrupts for PPR */
    bool_t          TTX_onPprEnablingInterrupt  (void);

    /* @brief Platform dependent codes for disabling interrupts for PPR */
    bool_t          TTX_onPprDisablingInterrupt  (void);

    /* @brief Platform dependent codes for eabling interrupts for VBI OSD */
    bool_t          TTX_onOsdEnablingInterrupt  (void);

    /* @brief Platform dependent codes for disabling interrupts for VBI OSD */
    bool_t          TTX_onOsdDisablingInterrupt  (void);

    /*****************************************************************************/
    /** @brief Write val to addr
    *
    * @param addr   The address (TTX_Address_t type) of the 32-bit register to be written.
    * @param val    32-bit value
    */
//void            TTX_Reg32Write          (TTX_Address_t addr, TTX_RegData_t val);
#define         TTX_Reg32Write(addr,val)   TTX_SRC_Parse_Write(addr,val)


    /* @brief Read value of addr */
//TTX_RegData_t   TTX_Reg32Read           (TTX_Address_t addr);
#define         TTX_Reg32Read(addr)        TTX_SRC_Parse_Read(addr)

    /*****************************************************************************/
    /** @brief Write val to addr
    *
    * @param addr   The address (TTX_Address_t type) of the 8-bit register to be written.
    * @param val    8-bit value
    */
//#define         TTX_Reg8Write(addr,val) (*((volatile uint8_t TTX_REG*) addr) = (val&0xFF))
#define         TTX_Reg8Write(addr,val)     rtd_outb(addr,val)

    /* @brief Read value of addr */
//#define         TTX_Reg8Read(addr)      (*((volatile uint8_t TTX_REG*) addr))
#define         TTX_Reg8Read(addr)          rtd_inb(addr)


//void            TTX_Log                 (char const* format, ...);
#define         TTX_Log                      RTD_Log


#ifdef __cplusplus
}   /* extern "C" */
#endif


//#include <ttx.h>               /* ttx platform-independent public interface */


#endif /* !__ENHANCED_TELETEXT_DECODER_LIBRARY_PLATFORM_PORT_HEADER_INCLUDED__ */

/**
 * @}
 */

/*---------------------------------------------------------------------------*\
 * end of ttx_port.h                                                         *
\*---------------------------------------------------------------------------*/
