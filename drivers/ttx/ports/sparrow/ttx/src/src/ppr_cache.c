/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ppr_cahce.c
 * @brief    PPR hardware driver.
 * @details  This header declares the public access application interface for this TtLib library.
 * @author   Janus Cheng
 * @date     Dec. 11, 2007
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/

#include <ppr.h>            /* Provides: functions to be implemented */

#if !PPR_SOFTWARE_EMULATOR

#include "ttx_pkg.h"

#include <stdlib.h>         /* Provides: NULL */
#include <ttx_assert.h>


#include "ppr_hal_test.h"   /* Provides: compile-time tests for CCL PPR */

#include <ttx_source.h>     /* Provides: TTX_SRC_Open() */


TTX_DEFINE_THIS_MODULE( ppr_cache.c )




/* -- Implementations of Public Functions -----------------------------------*/
#if 0

/*---------------------------------------------------------------------------*/
bool_t
PPR_PfeSetDestMag             (TTX_STD_MagazineNum_t mag, const void TTX_MEM* pDest)
/*****************************************************************************\
 *  @brief   Set destination of magazine m.
 *  @details Use this function to de-initialize this library.
 *  @note    Must be called in advance of any other functions.
 *
 *  @return  the initialization status.
 *  @retval  TRUE  success
 *  @retval  FALSE failure
 *
 *  @ingroup ttx
 *  @see     TTX_Init()
\*****************************************************************************/
{
    bool_t bOk;

    const TTX_Address_t dest    = ((TTX_Address_t)pDest) >> 3; // 8-byte basis
    const TTX_Address_t addrReg = PPR_ADDR( MAG1 )+ (mag-1)*4;

//    PPR_REQUIRE( TTX_STD_MagNumIsValid( mag ) );    // ERROR: mag is not within valid range of TTX_STD_MagazineNum_t.
//    PPR_REQUIRE( (((TTX_Address_t)pDest) & 0x7) == 0 ); // ERROR: mis-aligned pDest.


    PPR_RegWrite( addrReg , PPR_MAG1_RMK( dest ) );
    bOk = ( ((TTX_Address_t)8) * PPR_RegRead( addrReg ) == ((TTX_Address_t)pDest) );   // addrReg is 8-byte basis
    PPR_ENSURE( bOk == TRUE );                      // ERROR: addrReg cannot hold entire address of pDest.

    return( bOk );
}


#endif
/*---------------------------------------------------------------------------*/
void PPR_PfeSetMag(TTX_STD_MagazineNum_t mag, uint8_t avtive)
/*****************************************************************************\
 *  @brief   Activate moving mag for prefetch moving engine.
 *  @param   mag    the magazine you wanna enable.
 *
 *  @return  the activation status.
 *  @retval  TRUE  success
 *  @retval  FALSE failure
\*****************************************************************************/
{
    volatile uint32_t orig;
    const    uint32_t val  = (0x00000001UL << (mag));

    orig = rtd_isr_inl(VBIPPR_PFE_ACTIVE_REG_0x40dc);
    if (avtive)
        orig |= val;
    else
        orig &= ~val;
    rtd_isr_outl( VBIPPR_PFE_ACTIVE_REG_0x40dc, orig );
}

#if 0

/*---------------------------------------------------------------------------*/
bool_t
PPR_FieldBuffIsValid            (void)
/*****************************************************************************\
 *  @brief   Returns whether Field Buffer is valid or not.
 *
 *  @return  Validity of field buffer.
 *  @retval  TRUE  Content of field buffer can be used.
 *  @retval  FALSE Content of field buffer can not be used.
\*****************************************************************************/
{
    return ((TTX_Reg32Read(VBIPPR_DATA_ACQU_ST_REG_0x4014) & 0x1) ||
            (TTX_Reg32Read(VBIPPR_INT_ST_REG_0x4008) & _PPR_INT_ST_FB_VALID_MASK));

}

#endif

/*---------------------------------------------------------------------------*/
void
PPR_PfeMove    (PPR_PfeMode_t mode, uint8_t* fetched_line)
/*****************************************************************************\
 *  @brief   Start prefetch moving engine.
 *  @param   bEnableOddParityChk    enable odd parity check for TT packet in
 *                                  X/1~X/25: 0~39 bytes
 *
 *  @return  the initialization status.
 *  @retval  TRUE  success
 *  @retval  FALSE failure
 *
 *  @ingroup ttx
 *  @see     TTX_Deinit()
\*****************************************************************************/
{
    uint16_t addr_before_pfe, addr_after_pfe;
    addr_before_pfe = rtd_isr_inl(VBIPPR_PFE_NPROC_ADDR_REG_0x40e0);
    rtd_isr_outl( VBIPPR_PFE_CTRL_REG_0x40e4, mode | _BIT1);  // 10 page, PPR_PFE_MODE0
    rtd_isr_outl( VBIPPR_PFE_CTRL_REG_0x40e4, mode | _BIT1 | _BIT0);  // dma enable
    while ( (rtd_isr_inl(VBIPPR_PFE_CTRL_REG_0x40e4)&0x00000001) );
    addr_after_pfe = rtd_isr_inl(VBIPPR_PFE_NPROC_ADDR_REG_0x40e0);
    if ( fetched_line != NULL )
    {
        *fetched_line = (addr_after_pfe - addr_before_pfe) / 10;
    }
}

#endif  /* end of #if !PPR_SOFTWARE_EMULATOR */

/*---------------------------------------------------------------------------*\
 * end of ppr_cache.c                                                        *
\*---------------------------------------------------------------------------*/
