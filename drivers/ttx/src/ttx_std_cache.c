/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_std_cache.c
 * @brief    Platform-independent standard Teletext interface.
 * @author   Janus Cheng
 * @date     Nov. 11, 2008
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/
#ifdef CONFIG_TTX_SUPPORT

#include "ttx_std.h"            /* Provides: functions to be implemented */

#include <stdlib.h>             /* Provides: NULL */
#include <ttx_assert.h>         /* Provides: TTX_DEFINE_THIS_MODULE() */
#include <ttx_source.h>     /* Provides: TTX_SRC_Open() */
#include "reg52.h"

TTX_DEFINE_THIS_MODULE( ttx_std_cache.c )




/* -- Prototypes of Private Functions ---------------------------------------*/


/* -- Implementations of Public Functions -----------------------------------*/
#if 0

/*---------------------------------------------------------------------------*/
TTX_STD_Page_Addr_Num_Hex_t
TTX_STD_PageAddrNumToHex  (const TTX_STD_Page_Addr_Number_t*  pPageAddressNumber) TTX_REENTRANT
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
 *  @param   triplet1    bit14~bit8 of triplet1
 *  @param   c12c13c14   national option selection bits
 *  @note    Section 15.2 (P.101)
\*****************************************************************************/
{
    TTX_STD_Page_Addr_Num_Hex_t ret = 0;
    if ( pPageAddressNumber != NULL )
    {
        TTX_REQUIRE( pPageAddressNumber->tens  <= 0x0F );
        TTX_REQUIRE( pPageAddressNumber->units <= 0x0F );

        ret = ((((TTX_STD_Page_Addr_Num_Hex_t)pPageAddressNumber->tens) << 4) | pPageAddressNumber->units);
    }

    return( ret );
}


/*---------------------------------------------------------------------------*/
TTX_STD_Page_Addr_SubCode_Hex_t
TTX_STD_PageAddrSubCodeToHex (const TTX_STD_Page_Addr_SubCode_t* pPageAddressSubCode) TTX_REENTRANT
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
 *  @param   triplet1    bit14~bit8 of triplet1
 *  @param   c12c13c14   national option selection bits
 *  @note    Section 15.2 (P.101)
\*****************************************************************************/
{
    TTX_STD_Page_Addr_SubCode_Hex_t ret = 0;

    if ( pPageAddressSubCode != NULL )
    {
        TTX_REQUIRE( pPageAddressSubCode->s1 <= 0x0F );
        TTX_REQUIRE( pPageAddressSubCode->s2 <= 0x07 );
        TTX_REQUIRE( pPageAddressSubCode->s3 <= 0x0F );
        TTX_REQUIRE( pPageAddressSubCode->s4 <= 0x03 );

        ret  =  (TTX_STD_Page_Addr_SubCode_Hex_t)pPageAddressSubCode->s1;
        ret |= ((TTX_STD_Page_Addr_SubCode_Hex_t)pPageAddressSubCode->s2) << 4;
        ret |= ((TTX_STD_Page_Addr_SubCode_Hex_t)pPageAddressSubCode->s3) << 8;
        ret |= ((TTX_STD_Page_Addr_SubCode_Hex_t)pPageAddressSubCode->s4) <<12;
    }

    return( ret );
}

/*---------------------------------------------------------------------------*/
TTX_STD_Page_AddrWithSubCode_Hex_t
TTX_STD_PageAddrWithSubCodeToHex  (const TTX_STD_Page_AddrWithSubCode_t*  pPageAddress) TTX_REENTRANT
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
 *  @param   triplet1    bit14~bit8 of triplet1
 *  @param   c12c13c14   national option selection bits
 *  @note    Section 15.2 (P.101)
\*****************************************************************************/
{
    TTX_STD_Page_AddrWithSubCode_Hex_t ret = 0;

    TTX_STD_MagazineNum_t              magNum;
    TTX_STD_Page_Addr_Num_Hex_t  pageNum;
    TTX_STD_Page_Addr_SubCode_Hex_t subCode;

    if ( pPageAddress != NULL )
    {
//        TTX_REQUIRE( TTX_STD_MagNumIsValid( pPageAddress->magNum ) );
        magNum = pPageAddress->magNum;
        ret     = ((TTX_STD_Page_AddrWithSubCode_Hex_t)magNum)<<24;

        TTX_REQUIRE( TTX_STD_PageAddrNumberIsValid( pPageAddress->pageNum ) );
        pageNum = TTX_STD_PageAddrNumToHex  ( &(pPageAddress->pageNum) );

        TTX_REQUIRE( TTX_STD_PageAddrSubCodeIsValid( pPageAddress->subCode ) );
        subCode = TTX_STD_PageAddrSubCodeToHex( &(pPageAddress->subCode) );

        ret    |= ( ((TTX_STD_Page_AddrWithSubCode_Hex_t)pageNum)<<16 );
        ret    |= (  (TTX_STD_Page_AddrWithSubCode_Hex_t)subCode      );
    }

    return( ret );
}

#endif


#if 1
/*---------------------------------------------------------------------------*/
uint8_t
TTX_STD_PageAddrWithSubCodeAreEqual (const TTX_STD_Page_AddrWithSubCode_t* pAddr1, const TTX_STD_Page_AddrWithSubCode_t* pAddr2, uint8_t bCompareSubPage ) TTX_REENTRANT
/*****************************************************************************\
 *  @brief   Compare the page-addresses *pAddr1 and *pAddr2 are equal or not
 *             based on the comparison option: compSubpage.
 *  @param   *pAddr1          The page-address to be compared.
 *  @param   *pAddr2          The page-address to be compared.
 *  @param   bCompareSubPage  Compare the subcode of *pAddr1 and *pAddr2 or not.
 *
 *  @return  *pAddr1 and *pAddr2 are equal or not.
 *  @retval  TRUE         *pAddr1 and *pAddr2 are identical.
 *  @retval  FALSE        *pAddr1 and *pAddr2 are different.
 *
 *  @pre     pAddr1 != NULL
 *  @pre     pAddr2 != NULL
 *  @pre     bCompareSubPage == TRUE or FALSE
\*****************************************************************************/
{
    bool_t result = TRUE;
    EA=0;
    if (pAddr1->pageA != pAddr2->pageA)
        result = FALSE;

    if ( bCompareSubPage == FALSE )
    {
        EA =1;
        return TRUE;
    }
    if (pAddr1->subcodeA!= pAddr2->subcodeA)
        result = FALSE;
    EA=1;
    return( result );

}

uint8_t
TTX_ISR_PageAddrWithSubCodeAreEqual (const TTX_STD_Page_AddrWithSubCode_t* pAddr1, const TTX_STD_Page_AddrWithSubCode_t* pAddr2, uint8_t bCompareSubPage ) TTX_REENTRANT
/*****************************************************************************\
 *  @brief   Compare the page-addresses *pAddr1 and *pAddr2 are equal or not
 *             based on the comparison option: compSubpage.
 *  @param   *pAddr1          The page-address to be compared.
 *  @param   *pAddr2          The page-address to be compared.
 *  @param   bCompareSubPage  Compare the subcode of *pAddr1 and *pAddr2 or not.
 *
 *  @return  *pAddr1 and *pAddr2 are equal or not.
 *  @retval  TRUE         *pAddr1 and *pAddr2 are identical.
 *  @retval  FALSE        *pAddr1 and *pAddr2 are different.
 *
 *  @pre     pAddr1 != NULL
 *  @pre     pAddr2 != NULL
 *  @pre     bCompareSubPage == TRUE or FALSE
\*****************************************************************************/
{
    if (pAddr1->pageA != pAddr2->pageA)
        return( FALSE );

    if ( bCompareSubPage == FALSE )
        return( TRUE );

    if (pAddr1->subcodeA!= pAddr2->subcodeA)
        return( FALSE );

    return( TRUE );

}
#endif

#if 0

/*---------------------------------------------------------------------------*/
uint8_t
TTX_STD_PageAddrNumberIsValid        (const TTX_STD_Page_Addr_Number_t pageNumber) TTX_REENTRANT
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
 *  @param   triplet1    bit14~bit8 of triplet1
 *  @param   c12c13c14   national option selection bits
 *  @note    Section 15.2 (P.101)
\*****************************************************************************/
{
    return( ( pageNumber.tens <= 0xF ) && ( pageNumber.units <= 0xF ) );
}

/*---------------------------------------------------------------------------*/
uint8_t
TTX_STD_PageAddrSubCodeIsValid       (const TTX_STD_Page_Addr_SubCode_t subcode) TTX_REENTRANT
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
 *  @param   triplet1    bit14~bit8 of triplet1
 *  @param   c12c13c14   national option selection bits
 *  @note    Section 15.2 (P.101)
\*****************************************************************************/
{
    return(   ( subcode.s4 <= 0x03 )
    &&( subcode.s3 <= 0x0F )
    &&( subcode.s2 <= 0x07 )
    &&( subcode.s1 <= 0x0F ) );
}

/*---------------------------------------------------------------------------*/
#if 0
bool_t
TTX_STD_MagNumIsValid   (const TTX_STD_MagazineNum_t magNum)
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
 *  @param   triplet1    bit14~bit8 of triplet1
 *  @param   c12c13c14   national option selection bits
 *  @note    Section 15.2 (P.101)
\*****************************************************************************/
{
    return( (1<=magNum)&&(magNum<=8) );
}
#endif

/* -- Implementations of Private Functions ----------------------------------*/


#endif


/*---------------------------------------------------------------------------*\
 * end of ttx_std_cache.c                                                    *
\*---------------------------------------------------------------------------*/
#endif
