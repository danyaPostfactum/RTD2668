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

#include <ttx_assert.h>
#include <ttx_source.h>     /* Provides: TTX_SRC_Open() */

#if VTE_SOFTWARE_EMULATOR
#include <ttx_cd.h>     /* Provides: TTX_CD_ChannelDecode() */
#endif

#include <stdlib.h>         /* Provides: NULL */
#include <string.h>         /* Provides: memcpy() */

TTX_DEFINE_THIS_MODULE( vte.c )


/* -- Prototypes of Private Functions ---------------------------------------*/
static void         VTE_TestPwrOnDefaults   (void);


/* -- Implementations of Public Functions -----------------------------------*/

/*---------------------------------------------------------------------------*/
bool_t
VTE_Open (const VTE_OpenFlag_t flags)
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
    static bool_t bFirst = TRUE;

    if ( bFirst )
    {
        // Power-On Tests
        VTE_TestPwrOnDefaults();
        bFirst = FALSE;
    }

    if ( flags & VTE_OPEN_RESET )
    {
        //VTE_Reset();
    }

    return( TRUE );

}

/*---------------------------------------------------------------------------*/
bool_t
VTE_Close (void)
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

    return( TRUE );
}

/*---------------------------------------------------------------------------*/
void
VTE_Reset (void)
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
#if !VTE_SOFTWARE_EMULATOR
    VTE_FSETS( INT_ST, CD_ST, RESET );   /* Reset the INT_ST register's CD_ST field */
    VTE_ENSURE( VTE_FGET( INT_ST, CD_ST) == VTE_INT_ST_CD_ST_CLEARED );  /* Verify the above operation */
#endif
}

/*---------------------------------------------------------------------------*/
bool_t
VTE_Decode  (VTE_CDMode_t mode, uint8_t TTX_MEM* dest, const uint8_t TTX_MEM* src,
             uint16_t len)
/*****************************************************************************\
 *  @brief   VTE channel decoder.
 *  @param   mode : VTE_CD_MODE_H84M0  (Hamming 8/4 Mode 0)
 *                  VTE_CD_MODE_H84M1  (Hamming 8/4 Mode 1)
 *                  VTE_CD_MODE_H2418  (Hamming 24/18)
 *                  VTE_CD_MODE_ODDPAR (Odd Parity)
 *                  VTE_CD_MODE_AIT    (Additional Information Table)
 *  @param   dest : destination address
 *  @param   src  : source address
 *  @param   len  : unit is "number of bytes" for VTE_CD_MODE_H84M0 and VTE_CD_MODE_ODDPAR;
 *                  unit is "number of packets" for VTE_CD_MODE_H84M1 and VTE_CD_MODE_H2418 and VTE_CD_MODE_AIT.
 *  @ret     TRUE if no error occurs duing decoding.
\*****************************************************************************/
{
#if VTE_SOFTWARE_EMULATOR
    VTE_REQUIRE( VTE_CD_MODE_H84M0  == TTX_CD_HAMMING_8_4_MODE_0 );
    VTE_REQUIRE( VTE_CD_MODE_H84M1  == TTX_CD_HAMMING_8_4_MODE_1 );
    VTE_REQUIRE( VTE_CD_MODE_H2418  == TTX_CD_HAMMING_24_18      );
    VTE_REQUIRE( VTE_CD_MODE_ODDPAR == TTX_CD_ODD_PARITY         );
    VTE_REQUIRE( VTE_CD_MODE_AIT    == TTX_CD_AIT                );


    return( TTX_CD_ChannelDecode( src, dest, len, (TTX_CD_Mode_t) mode) );
#else
    VTE_ASSERT_COMPILE( VTE_CD_MODE_H84M0  == 0 );
    VTE_ASSERT_COMPILE( VTE_CD_MODE_H84M1  == 1 );
    VTE_ASSERT_COMPILE( VTE_CD_MODE_H2418  == 2 );
    VTE_ASSERT_COMPILE( VTE_CD_MODE_ODDPAR == 3 );
    VTE_ASSERT_COMPILE( VTE_CD_MODE_AIT    == 4 );

    const _VTE_CD_ERR_RVALUE_TYPE ERR_RVAL_LOOKUP[] =
    {
        0xFFFFFF,   /* VTE_CD_MODE_H84M0  */ \
        0xFFFFFF,   /* VTE_CD_MODE_H84M1  */ \
        0x200000,   /* VTE_CD_MODE_H2418  */ \
        0x000020,   /* VTE_CD_MODE_ODDPAR */ \
        0xFFFFFF    /* VTE_CD_MODE_AIT    */ \
    };

    const _VTE_CD_ERR_RVALUE_TYPE err_rval = ERR_RVAL_LOOKUP[mode];

    const TTX_Address_t UNIT_DIV[] =
    {
        4,   /* VTE_CD_MODE_H84M0  */ \
        40,   /* VTE_CD_MODE_H84M1  */ \
        40,   /* VTE_CD_MODE_H2418  */ \
        4,   /* VTE_CD_MODE_ODDPAR */ \
        40    /* VTE_CD_MODE_AIT    */ \
    };

    const TTX_Address_t cd_src_addr  = (TTX_Address_t)src /UNIT_DIV[mode];
    const TTX_Address_t cd_dest_addr = (TTX_Address_t)dest/UNIT_DIV[mode];
    const uint16_t      cd_len       =                len /UNIT_DIV[mode];

    VTE_REQUIRE( src  != NULL );
    VTE_REQUIRE( dest != NULL );
    VTE_REQUIRE( len  != 0    );

    VTE_REQUIRE( ((TTX_Address_t)src  % UNIT_DIV[mode]) == 0 );  // ERROR: mis-aligned src address
    VTE_REQUIRE( ((TTX_Address_t)dest % UNIT_DIV[mode]) == 0 );  // ERROR: mis-aligned dest address
    VTE_REQUIRE( (               len  % UNIT_DIV[mode]) == 0 );  // ERROR: mis-aligned len

    VTE_FSETS( CD_CTRL, EN, DISABLE );  // disable channel decoder
    VTE_ENSURE( VTE_FGET( CD_CTRL, EN ) == VTE_CD_CTRL_EN_DISABLE );

    VTE_RSET( INT_ST, VTE_INT_ST_RMKS( RESET ) ); // reset running status
    VTE_ENSURE( VTE_FGET(INT_ST, CD_ST) == VTE_INT_ST_CD_ST_CLEARED );

    VTE_RSET( CD_SADDR, VTE_CD_SADDR_RMK(cd_src_addr) );    // set source address
    VTE_RSET( CD_DADDR, VTE_CD_DADDR_RMK(cd_dest_addr) );   // set destination address
    VTE_RSET( CD_ERR_RVALUE, VTE_CD_ERR_RVALUE_RMK(err_rval) ); // set error-restore value

    // Verify above register writes.
    VTE_ENSURE( VTE_RGET( CD_SADDR ) == VTE_CD_SADDR_RMK(cd_src_addr) );
    VTE_ENSURE( VTE_RGET( CD_DADDR ) == VTE_CD_DADDR_RMK(cd_dest_addr) );
    VTE_ENSURE( VTE_RGET( CD_ERR_RVALUE ) == VTE_CD_ERR_RVALUE_RMK( err_rval ) );

    // Start decoding
    VTE_RSET( CD_CTRL, VTE_CD_CTRL_RMK(         cd_len            ,  /*  DATA_LEN  */  \
              VTE_CD_CTRL_DATA_ERR_CLEAR,  /*  DATA_ERR  */  \
              mode             ,  /*    MODE    */  \
              VTE_CD_CTRL_EN_ENABLE        /*     EN     */  \
                                      ) );

    // Wait for channel decoding done
    while ( VTE_FGET(INT_ST, CD_ST) != VTE_INT_ST_CD_ST_DONE );
    VTE_ALLEGE( VTE_FGET(INT_ST, CD_ST) == VTE_INT_ST_CD_ST_DONE );

    VTE_RSET( INT_ST, VTE_INT_ST_RMKS( RESET ) ); // reset running status
    VTE_ENSURE( VTE_FGET(INT_ST, CD_ST) == VTE_INT_ST_CD_ST_CLEARED );

    return( VTE_FGET( CD_CTRL, DATA_ERR ) == VTE_CD_CTRL_DATA_ERR_OK );

#endif  /* end of #if VTE_SOFTWARE_EMULATOR */
}



/* -- Implementations of Private Functions ----------------------------------*/

/*---------------------------------------------------------------------------*/
static void
VTE_TestPwrOnDefaults   (void)
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
\*****************************************************************************/
{
    VTE_Log( LOGGER_VERBOSE, "[VTE]" "[Trace]" "Entering VTE_TestPwrOnDefaults()... ");

#if !VTE_SOFTWARE_EMULATOR


#define     VTE_DEFAULT_TEST(reg)  VTE_ENSURE( VTE_RGET( reg ) == VTE_##reg##_DEFAULT )

    VTE_DEFAULT_TEST( INT_ST );
    VTE_DEFAULT_TEST( INT_EN );
    VTE_DEFAULT_TEST( INT_DIS );
    VTE_DEFAULT_TEST( CD_SADDR );
    VTE_DEFAULT_TEST( CD_DADDR );
    VTE_DEFAULT_TEST( CD_CTRL );
    VTE_DEFAULT_TEST( CD_ERR_RVALUE );
#endif

    VTE_Log( LOGGER_VERBOSE, "[done]\n");
}



/*---------------------------------------------------------------------------*\
 * end of vte.c                                                              *
\*---------------------------------------------------------------------------*/
