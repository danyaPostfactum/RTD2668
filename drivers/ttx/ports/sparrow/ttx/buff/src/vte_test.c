/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     vte.c
 * @brief    Prototypes of public application interface.
 * @details  This header declares the public access application interface for this TtLib library.
 * @author   Janus Cheng
 * @date     Dec. 11, 2007
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/

#include "vte.h"            /* Provides: functions to be implemented */

#include <ttx_assert.h>     /* Provides: TTX_DEFINE_THIS_MODULE() */

#include <ttx_buff_pkg.h>   /* Provides: TTX_DEFAULT_CD_PAGE_LAST_ADDR and TTX_DEFAULT_CD_PAGE_ADDRESS */

#include <stdlib.h>         /* Provides: NULL */
#include <string.h>         /* Provides: memcmp() */

#if !VTE_SOFTWARE_EMULATOR
#include "sys.h"            /* Provides: SYS_SDMA_Copy() */
#include "vte_hal_test.h"   /* Provides: compile-time tests for CCL VTE */
#endif

#define _VTE_TEST_PTN_BANK  (0) /* bank ID of VTE test patterns */


TTX_DEFINE_THIS_MODULE( vte_test.c )


/* -- Prototypes of Private Functions ---------------------------------------*/
#if TTX_VERIFY  /* if enabled verficiation of TTX related IPs */
#if !VTE_SOFTWARE_EMULATOR
static void     VTE_TestRegsReadWrite       (void);
#endif  /* end of #if !VTE_SOFTWARE_EMULATOR */

static bool_t   VTE_TestChanndlDecoder      (VTE_CDMode_t mode, uint8_t bank, const void TTX_ROM* ptn, const void TTX_ROM* answer, uint16_t sizeOfPtn);
static void     VTE_TestModeHamming84M0_0   (void);
static void     VTE_TestModeHamming84M1_0   (void);
static void     VTE_TestModeHamming2418_0   (void);
static void     VTE_TestModeOddParity_0     (void);
static void     VTE_TestModeAit_0           (void);
#endif  /* end of #if TTX_VERIFY */

/* -- Implementations of Public Functions -----------------------------------*/

#if TTX_VERIFY  /* if enabled verficiation of TTX related IPs */

/*---------------------------------------------------------------------------*/
bool_t
VTE_Test (void)
/*****************************************************************************\
 *  @brief   The initialization routine.
 *  @details Use this function to initialize this library.
 *  @note    Must be called in advance of any other functions.
 *
 *  @return  the initialization status.
 *  @retval  TTX_TRUE  success
 *  @retval  TTX_FALSE failure
 *
 *  @ingroup ttx
 *  @see     TTX_Deinit()
\*****************************************************************************/
{
#if !VTE_SOFTWARE_EMULATOR

    // Register read-write tests
    VTE_TestRegsReadWrite();

#endif

    /* Interrupt Test */

    /* Channel Decoder Test */
    {
        VTE_TestModeHamming84M0_0();
        VTE_TestModeHamming84M1_0();
        VTE_TestModeHamming2418_0();
        VTE_TestModeOddParity_0();
        VTE_TestModeAit_0();
    }

    return( TRUE );
}



/* -- Implementations of Private Functions ----------------------------------*/

#if TTX_VERIFY && !VTE_SOFTWARE_EMULATOR
/*---------------------------------------------------------------------------*/
static void
VTE_TestRegsReadWrite   (void)
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
\*****************************************************************************/
{
#if !VTE_SOFTWARE_EMULATOR
    volatile uint32_t origValue;

    VTE_Log( LOGGER_VERBOSE, "[VTE]" "[Trace]" "Entering VTE_TestRegsReadWrite()... ");

#define VTE_TEST_REG_RW(reg,val)        \
    origValue = VTE_RGET(reg);          \
    VTE_RSET( reg, val );               \
    VTE_ENSURE( VTE_RGET(reg) == val ); \
    VTE_RSET( reg, origValue )

    VTE_TEST_REG_RW( CD_SADDR, VTE_CD_SADDR_RMK(0x03FFFFFF) );
    VTE_TEST_REG_RW( CD_SADDR, VTE_CD_SADDR_RMK(0x07654321) );

    VTE_TEST_REG_RW( CD_DADDR, VTE_CD_DADDR_RMK(0x03FFFFFF) );
    VTE_TEST_REG_RW( CD_DADDR, VTE_CD_DADDR_RMK(0x07BCDEF2) );

    VTE_TEST_REG_RW( CD_CTRL , VTE_CD_CTRL_RMKS( 0xA5   ,  /*  DATA_LEN  */  \
                     OK     ,  /*  DATA_ERR  */  \
                     ODDPAR ,  /*  MODE      */  \
                     DISABLE   /*  EN        */  \
                                               ));

    VTE_TEST_REG_RW( CD_ERR_RVALUE, VTE_CD_ERR_RVALUE_RMK( 0xFFFFFF ) );
    VTE_TEST_REG_RW( CD_ERR_RVALUE, VTE_CD_ERR_RVALUE_RMK( 0x5A5AA5 ) );
    VTE_TEST_REG_RW( CD_ERR_RVALUE, VTE_CD_ERR_RVALUE_RMK( 0x123456 ) );



    VTE_Log( LOGGER_VERBOSE, "[done]\n");
#endif  // end of #if !VTE_SOFTWARE_EMULATOR
}
#endif  // end of #if TTX_VERIFY && !VTE_SOFTWARE_EMULATOR

/*---------------------------------------------------------------------------*/
static bool_t
VTE_TestChanndlDecoder      (VTE_CDMode_t mode, uint8_t bank, const void TTX_ROM* ptn, const void TTX_ROM* answer, uint16_t sizeOfPtn)
/*****************************************************************************\
 *  @brief   Helper funtion for testing channel decoder
 *  @param   mode        the channel decoder mode that you want to perform tests.
 *  @param   bank        the flash ROM bank that both ptn[] and answer[] are located.
 *  @param   ptn[]       the data to be decoded.
 *  @param   answer[]    the ideally decoded data.
 *  @param   sizeOfPtn   Number of bytes of ptn[] and answer[].
 *  @pre     No any IP is occupying the SRAM.
 *  @return  Test OK/fail
 *  @retval  TRUE        test OK
 *  @retval  FALSE       test fail
\*****************************************************************************/
{
    const TTX_Address_t _addrOfPtn = TTX_DEFAULT_CD_PAGE_LAST_ADDR;
    uint8_t TTX_MEM* addrOfPtn         = (uint8_t TTX_MEM*)(_addrOfPtn);   // SRAM address of ptn, stored in the 1st page of page queue
    uint8_t TTX_MEM* addrOfDecodedData = (uint8_t TTX_MEM*)TTX_DEFAULT_CD_PAGE_ADDRESS;

    const uint8_t TTX_ROM* pAns = answer;

    bool_t   bOk;
    uint16_t i;

    VTE_REQUIRE( addrOfDecodedData != NULL );

    VTE_REQUIRE( _addrOfPtn %  4 == 0);    // ERROR: This function cannot run VTE_CD_MODE_H84M0 and VTE_CD_MODE_ODDPAR
    VTE_REQUIRE( _addrOfPtn % 40 == 0);    // ERROR: This function cannot run VTE_CD_MODE_H84M1, VTE_CD_MODE_H2418 and VTE_CD_MODE_AIT

    VTE_REQUIRE( TTX_DEFAULT_CD_PAGE_ADDRESS  %  4 == 0);    // ERROR: This function cannot run VTE_CD_MODE_H84M0 and VTE_CD_MODE_ODDPAR
    VTE_REQUIRE( TTX_DEFAULT_CD_PAGE_ADDRESS  % 40 == 0);    // ERROR: This function cannot run VTE_CD_MODE_H84M1, VTE_CD_MODE_H2418 and VTE_CD_MODE_AIT

    VTE_REQUIRE( addrOfPtn         != NULL );
    VTE_REQUIRE( addrOfDecodedData != NULL );
    if ( (addrOfPtn != NULL) && (addrOfDecodedData != NULL) )
    {
        // Prepare source using SPI-DMA
        {
            // Copy Attributes to SRAM@0xC000 through SPI-DMA.
#if VTE_SOFTWARE_EMULATOR
            memcpy( addrOfPtn, ptn, sizeOfPtn );
#else
            SYS_SDMA_Copy( addrOfPtn, bank, ptn, sizeOfPtn );

            // Verify above SYS_SDMA_Copy()
            if ( memcmp( addrOfPtn, ptn, sizeOfPtn ) != 0 )
            {
                VTE_Log( LOGGER_ERROR, "[VTE]""[ERR] SYSHW_SDMA_Copy()\n");
                return( FALSE );
            }
#endif

        }

        bOk = VTE_Decode( mode, addrOfDecodedData, addrOfPtn, sizeOfPtn );
        for ( i=0; i<sizeOfPtn; i++ )
        {
            if ( addrOfDecodedData[i] != pAns[i] )
            {
                VTE_Log( LOGGER_ERROR,  "[VTE]" "[ERR] VTE_Decode() @ 0x%X\n", (uint32_t)(addrOfDecodedData+i) );
                return( FALSE );
            }
        }

        return( bOk );
    }
    return( FALSE );
}

/*---------------------------------------------------------------------------*/
static void
VTE_TestModeHamming84M0_0     (void)
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
 *  @pre     No any IP is occupying the SRAM.
\*****************************************************************************/
{
    extern uint8_t const TTX_ROM VTE_testCdMode0In[];   // data to be decoded
    extern uint8_t const TTX_ROM VTE_testCdMode0Out[];  // decoded golden data

    const uint16_t SIZE_OF_PTN = 23*40; // bytes, size of VTE_testCdMode0In[]
    const uint8_t  BANK_OF_PTN = _VTE_TEST_PTN_BANK;     // VTE_testCdMode0In[] is located in bank _VTE_TEST_PTN_BANK
    bool_t bOk;

    const VTE_CDMode_t MODE = VTE_CD_MODE_H84M0;

    VTE_Log( LOGGER_VERBOSE, "[VTE]" "[Trace]" "Entering VTE_TestModeHamming84M0_0()... ");

    VTE_REQUIRE( VTE_testCdMode0In[0] == 0x73 );    // ERROR: Cannot read pattern to be fed into.
    bOk = VTE_TestChanndlDecoder( MODE, BANK_OF_PTN, VTE_testCdMode0In, VTE_testCdMode0Out, SIZE_OF_PTN );
    VTE_ENSURE( bOk == TRUE );

    VTE_Log( LOGGER_VERBOSE, "[done]\n");
}

/*---------------------------------------------------------------------------*/
static void
VTE_TestModeHamming84M1_0     (void)
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
 *  @pre     No any IP is occupying the SRAM.
\*****************************************************************************/
{
    extern uint8_t const TTX_ROM VTE_testCdMode1In[];   // data to be decoded
    extern uint8_t const TTX_ROM VTE_testCdMode1Out[];  // decoded golden data

    const uint16_t SIZE_OF_PTN = 8*40; // bytes, size of VTE_testCdMode1In[]
    const uint8_t  BANK_OF_PTN = _VTE_TEST_PTN_BANK;     // VTE_testCdMode1In[] is located in bank _VTE_TEST_PTN_BANK
    bool_t bOk;

    const VTE_CDMode_t MODE = VTE_CD_MODE_H84M1;

    VTE_Log( LOGGER_VERBOSE, "[VTE]" "[Trace]" "Entering VTE_TestModeHamming84M1_0()... ");

    VTE_REQUIRE( VTE_testCdMode1In[0] == 0x15 );    // ERROR: Cannot read pattern to be fed into.
    bOk = VTE_TestChanndlDecoder( MODE, BANK_OF_PTN, VTE_testCdMode1In, VTE_testCdMode1Out, SIZE_OF_PTN );
    VTE_ENSURE( bOk == TRUE );

    VTE_Log( LOGGER_VERBOSE, "[done]\n");
}

/*---------------------------------------------------------------------------*/
static void
VTE_TestModeHamming2418_0     (void)
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
 *  @pre     No any IP is occupying the SRAM.
\*****************************************************************************/
{
    extern uint8_t const TTX_ROM VTE_testCdMode2In[];   // data to be decoded
    extern uint8_t const TTX_ROM VTE_testCdMode2Out[];  // decoded golden data

    const uint16_t SIZE_OF_PTN = 41*40; // bytes, size of VTE_testCdMode2In[]
    const uint8_t  BANK_OF_PTN = _VTE_TEST_PTN_BANK;     // VTE_testCdMode2In[] is located in bank _VTE_TEST_PTN_BANK
    bool_t bOk;

    const VTE_CDMode_t MODE = VTE_CD_MODE_H2418;

    VTE_Log( LOGGER_VERBOSE, "[VTE]" "[Trace]" "Entering VTE_TestModeHamming2418_0()... ");

    VTE_REQUIRE( VTE_testCdMode2In[1] == 0x74 );    // ERROR: Cannot read pattern to be fed into.
    VTE_REQUIRE( VTE_testCdMode2Out[3] == 0x7F );    // ERROR: Cannot read pattern to be fed into.
    bOk = VTE_TestChanndlDecoder( MODE, BANK_OF_PTN, VTE_testCdMode2In, VTE_testCdMode2Out, SIZE_OF_PTN );
    VTE_ENSURE( bOk == FALSE ); // VTE_testCdMode2In[] contains double error.

    VTE_Log( LOGGER_VERBOSE, "[done]\n");
}

/*---------------------------------------------------------------------------*/
static void
VTE_TestModeOddParity_0     (void)
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
 *  @pre     No any IP is occupying the SRAM.
\*****************************************************************************/
{
    extern uint8_t const TTX_ROM VTE_testCdMode3In[];   // data to be decoded
    extern uint8_t const TTX_ROM VTE_testCdMode3Out[];  // decoded golden data

    const uint16_t SIZE_OF_PTN = 25*40; // bytes, size of VTE_testCdMode3In[]
    const uint8_t  BANK_OF_PTN = _VTE_TEST_PTN_BANK;     // VTE_testCdMode3In[] is located in bank _VTE_TEST_PTN_BANK
    bool_t bOk;

    const VTE_CDMode_t MODE = VTE_CD_MODE_ODDPAR;

    VTE_Log( LOGGER_VERBOSE, "[VTE]" "[Trace]" "Entering VTE_TestModeOddParity_0()... ");

    VTE_REQUIRE( VTE_testCdMode3In[0] == 0x9D );    // ERROR: Cannot read pattern to be fed into.
    bOk = VTE_TestChanndlDecoder( MODE, BANK_OF_PTN, VTE_testCdMode3In, VTE_testCdMode3Out, SIZE_OF_PTN );
    VTE_ENSURE( bOk == TRUE );

    VTE_Log( LOGGER_VERBOSE, "[done]\n");
}

/*---------------------------------------------------------------------------*/
static void
VTE_TestModeAit_0     (void)
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
 *  @pre     No any IP is occupying the SRAM.
\*****************************************************************************/
{
    extern uint8_t const TTX_ROM VTE_testCdMode4In[];   // data to be decoded
    extern uint8_t const TTX_ROM VTE_testCdMode4Out[];  // decoded golden data

    const uint16_t SIZE_OF_PTN = 22*40; // bytes, size of VTE_testCdMode3In[]
    const uint8_t  BANK_OF_PTN = _VTE_TEST_PTN_BANK;     // VTE_testCdMode3In[] is located in bank _VTE_TEST_PTN_BANK
    bool_t bOk;

    const VTE_CDMode_t MODE = VTE_CD_MODE_AIT;

    VTE_Log( LOGGER_VERBOSE, "[VTE]" "[Trace]" "Entering VTE_TestModeAit_0()... ");

    VTE_REQUIRE( VTE_testCdMode4In[0] == 0x02 );    // ERROR: Cannot read pattern to be fed into.
    VTE_REQUIRE( VTE_testCdMode4Out[8] == 0x53 );    // ERROR: Cannot read pattern to be fed into.
    bOk = VTE_TestChanndlDecoder( MODE, BANK_OF_PTN, VTE_testCdMode4In, VTE_testCdMode4Out, SIZE_OF_PTN );
    VTE_ENSURE( bOk == TRUE );

    VTE_Log( LOGGER_VERBOSE, "[done]\n");
}

#endif  /* end of #if TTX_VERIFY */

/*---------------------------------------------------------------------------*\
 * end of vte_test.c                                                         *
\*---------------------------------------------------------------------------*/
