/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     sys.c
 * @brief    Prototypes of public application interface.
 * @details  This header declares the public access application interface for this TtLib library.
 * @author   Janus Cheng
 * @date     Dec. 11, 2007
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/

#include <sys.h>            /* Provides: functions to be implemented */
#include <stdlib.h>         /* Provides: NULL */
#include <ttx_assert.h>     /* Provides: TTX_ASSERT_COMPILE() */

#include <sys_hal.h>
#include <sys_hal_test.h>   /* Provides: compile-time tests for CCL SYS */
#include <sram_hal.h>       /* Provides: SRAM_Read() and SRAM_Write() */

#include <string.h>         /* Provides: memset() and memcpy() */

#if TTX_VERIFY  /* if enabled verficiation of TTX related IPs */
TTX_DEFINE_THIS_MODULE( sys.c )
#endif  /* #if TTX_VERIFY */

/* -- Prototypes of Private Functions ---------------------------------------*/
static bool_t   SYS_TestSRAM    (TTX_Address_t startAddr, size_t numOfBytes);





/* -- Implementations of Public Functions -----------------------------------*/
/*---------------------------------------------------------------------------*/
bool_t
SYS_Open (const SYS_OpenFlag_t flags)
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
    /* Thourgh tests
        {
            SYS_TestSRAM( 0x8000, (0xB400 - 0x8000) );
            SYS_TestSRAM( 0xC000, (0xE200 - 0xC000) );
        }
    */
    /*
        // Fast tests
        {
            SYS_TestSRAM( 0x8000, 0x0100 ); // Test 0x8000~0x8100 (head)
            SYS_TestSRAM( 0xB300, 0x0100 ); // Test 0xB300~0xB400 (tail)

            SYS_TestSRAM( 0xC000, 0x0100 ); // Test 0xC000~0xC100 (head)
            SYS_TestSRAM( 0xD000, 0x0100 ); // Test 0xD000~0xD100 (body)
            SYS_TestSRAM( 0xE100, 0x0100 ); // Test 0xE100~0xE200 (tail)
        }
    */
    if ( flags & SYS_OPEN_RESET )
    {
//        SYS_Reset();
    }


    return( TRUE );
}


#if TTX_VERIFY  /* if enabled verficiation of TTX related IPs */

/*---------------------------------------------------------------------------*/
bool_t
SYS_Test      (void)
/*****************************************************************************\
 *  @brief   Copy data from SPI flash to DMEM or registers.
 *  @note    This is a blocking call which will be blocked until copy is done.
 *  @param   mode    : 0     write 8bit (6093) registers (address won't increase)
 *                   : 1     write dmem (address increases)
 *                   : 2     write 32 bit register / system register (address won't increase)
 *  @param   bankFrom: 0~31  the bank of source flash address.
 *  @param   addrFrom: 0x0000~0xFFFF the address of source flash.
 *  @param   addrTo  : 0x0000~0xFFFF the destination address.
 *  @param   length  : for mode 0~1, unit is 8-bit
 *                     for mode  2 , unit is 32-bit
\*****************************************************************************/
{
//        SYS_TestSRAM( 0x8000, (0xB400 - 0x8000) );
//        SYS_TestSRAM( 0xC000, (0xE200 - 0xC000) );
    return( TRUE );
}

/*---------------------------------------------------------------------------*/
void
SYS_SDMA_Copy                      (void TTX_MEM*         addrTo  , \
                                    uint8_t               bankFrom, \
                                    const void TTX_ROM*   addrFrom, \
                                    uint16_t              numOfBytes  )
/*****************************************************************************\
 *  @brief   Copy data from SPI flash to DMEM or registers.
 *  @note    This is a blocking call which will be blocked until copy is done.
 *  @param   mode    : 0     write 8bit (6093) registers (address won't increase)
 *                   : 1     write dmem (address increases)
 *                   : 2     write 32 bit register / system register (address won't increase)
 *  @param   bankFrom: 0~31  the bank of source flash address.
 *  @param   addrTo  : 0x0000~0xFFFF the destination address.
 *  @param   addrFrom: 0x0000~0xFFFF the address of source flash.
 *  @param   numOfBytes : for mode 0~1, unit is 8-bit
 *                        for mode  2 , unit is 32-bit
\*****************************************************************************/
{
#if TTX_USE_SDMA
    const uint8_t   mode = SYS_SDMA_CTRL_MODE_1;

#if TTX_USE_SDMA_NONSTOP
    const volatile _SYS_NSDMA_TYPE nsdma = SYS_RGET( NSDMA );  /* store original value of NSDMA */
#endif

    /* Stop SPI-DMA */
    SYS_RSET( SDMA_CTRL, SYS_SDMA_CTRL_RMK(  mode, SYS_SDMA_CTRL_GO_STOP ) );

#if TTX_USE_SDMA_NONSTOP
    /* Disable Non-Stop DMA */
    SYS_FSETS( NSDMA, ENABLE, DISABLE );
#endif

    /* Prepare source */
    SYS_RSET( SDMA_FLASHLSB, SYS_SDMA_FLASHLSB_RMK(  (uint16_t)addrFrom       & 0xFF ) );
    SYS_RSET( SDMA_FLASHMSB, SYS_SDMA_FLASHMSB_RMK( ((uint16_t)addrFrom >> 8) & 0xFF ) );
    SYS_RSET( SDMA_BANK    , SYS_SDMA_BANK_RMK    (  bankFrom              ) );
    /* Commtented to save code-size of common bank
        SYS_ALLEGE( SYS_RGET( SDMA_FLASHLSB ) == SYS_SDMA_FLASHLSB_RMK(  (uint16_t)addrFrom       & 0xFF ) );
        SYS_ALLEGE( SYS_RGET( SDMA_FLASHMSB ) == SYS_SDMA_FLASHMSB_RMK( ((uint16_t)addrFrom >> 8) & 0xFF ) );
        SYS_ALLEGE( SYS_RGET( SDMA_BANK     ) == SYS_SDMA_BANK_RMK    (  bankFrom              ) );
    */
    /* Prepare destination */
    SYS_RSET( SDMA_DESTLSB , SYS_SDMA_DESTLSB_RMK (  (uint16_t)addrTo         & 0xFF ) );
    SYS_RSET( SDMA_DESTMSB , SYS_SDMA_DESTMSB_RMK ( ((uint16_t)addrTo   >> 8) & 0xFF ) );
    SYS_ALLEGE( SYS_RGET( SDMA_DESTLSB ) == SYS_SDMA_DESTLSB_RMK (  (uint16_t)addrTo         & 0xFF ) );
    SYS_ALLEGE( SYS_RGET( SDMA_DESTMSB ) == SYS_SDMA_DESTMSB_RMK ( ((uint16_t)addrTo   >> 8) & 0xFF ) );

    /* Prepare the length to copy */
    SYS_RSET( SDMA_LENLSB  , SYS_SDMA_LENLSB_RMK  (  numOfBytes         & 0xFF ) );
    SYS_RSET( SDMA_LENMSB  , SYS_SDMA_LENLSB_RMK  ( (numOfBytes   >> 8) & 0xFF ) );
    SYS_ALLEGE( SYS_RGET( SDMA_LENLSB ) == SYS_SDMA_LENLSB_RMK  (  numOfBytes         & 0xFF ) );
    SYS_ALLEGE( SYS_RGET( SDMA_LENMSB ) == SYS_SDMA_LENMSB_RMK  ( (numOfBytes   >> 8) & 0xFF ) );

    /* Start Control */
    SYS_RSET( SDMA_CTRL    , SYS_SDMA_CTRL_RMK    (  mode , SYS_SDMA_CTRL_GO_START ) );
    SYS_RSET( SDMA_CTRL    , SYS_SDMA_CTRL_RMK    (  mode , SYS_SDMA_CTRL_GO_STOP ) );

#if TTX_USE_SDMA_NONSTOP
    SYS_RSET( NSDMA, nsdma ); /* Restore Non-Stop DMA */
#endif

#else
    bankFrom = 0;   /* Eliminate compiler warning */
    memcpy( addrTo, addrFrom, numOfBytes );
#endif
}
#endif  /* end of #if TTX_VERIFY */



/* -- Implementations of Private Functions ----------------------------------*/

#if 0   // Unused function
/*---------------------------------------------------------------------------*/
static bool_t
SYS_TestSRAM    (TTX_Address_t startAddr, size_t numOfBytes)
/*****************************************************************************\
 *  @brief   The de-initialization routine.
 *  @details Use this function to de-initialize this library.
 *  @note    Must be called in advance of any other functions.
 *
 *  @return  the initialization status.
 *  @retval  TTX_TRUE  success
 *  @retval  TTX_FALSE failure
 *
 *  @ingroup ttx
 *  @see     TTX_Init()
\*****************************************************************************/
{
    const uint8_t   ptns[] = {0x00, 0xFF, 0x5A, 0xA5, 0xF0, 0x0F};
    const size_t    NUM_OF_PATTERNS = sizeof( ptns )/sizeof( ptns[0] );

    size_t i, j;

    SYS_REQUIRE( NUM_OF_PATTERNS == 6 );

    SYS_Log( LOGGER_VERBOSE, "[SYS] Testing SRAM (0x%X - 0x%X)... ", (uint32_t)startAddr, (uint32_t)(startAddr + numOfBytes -1) );

    for ( j=0; j< NUM_OF_PATTERNS; j++ )
    {
        for ( i=0; i<numOfBytes; i++ )
        {
            SRAM_Write( startAddr+i, ptns[(i+j)%NUM_OF_PATTERNS] );
        }

        for ( i=0; i<numOfBytes; i++ )
        {
            if ( SRAM_Read( startAddr+i ) != ptns[(i+j)%NUM_OF_PATTERNS] )
            {
                SYS_Log( LOGGER_ERROR, "[FAIL@0x%X]\n", (uint32_t)(startAddr+i));
                return( FALSE );
            }
        }
    }


    // Reset memory to zero
    memset( (void TTX_MEM*)startAddr, 0, numOfBytes );

    SYS_Log( LOGGER_VERBOSE, "[OK]\n");
    return( TRUE );
}
#endif

/*---------------------------------------------------------------------------*\
 * end of sys.c                                                              *
\*---------------------------------------------------------------------------*/
