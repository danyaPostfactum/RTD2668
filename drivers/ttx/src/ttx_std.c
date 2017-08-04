/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_std.c
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
#include <ttx_log.h>            /* Provides: TTX_Log(LOGGER_VERBOSE, ) */

TTX_DEFINE_THIS_MODULE( ttx_std.c )
#if 1   //Toshiba Setting
static const uint8_t TTX_ROM DefaultCharSet[17] =
{
    TTX_CHARSET_G0_LATIN_ENGLISH, // Default G0 of National Option 000
    TTX_CHARSET_G0_LATIN_GERMAN, // Default G0 of National Option 001
    TTX_CHARSET_G0_LATIN_ESTONIAN, // Default G0 of National Option 010
    TTX_CHARSET_G0_LATIN_LETTISH_LITHUANIAN, // Default G0 of National Option 011
    TTX_CHARSET_G0_CYRILLIC_OP2, // Default G0 of National Option 100
    TTX_CHARSET_G0_CYRILLIC_OP3, // Default G0 of National Option 101
    TTX_CHARSET_G0_LATIN_CZECH_SLOVAK, // Default G0 of National Option 110
    TTX_CHARSET_G0_CYRILLIC_OP3, // Default G0 of National Option 111
    TTX_CHARSET_G2_LATIN, // Default G2 of National Option 000
    TTX_CHARSET_G2_LATIN, // Default G2 of National Option 001
    TTX_CHARSET_G2_LATIN, // Default G2 of National Option 010
    TTX_CHARSET_G2_LATIN, // Default G2 of National Option 011
    TTX_CHARSET_G2_CYRILLIC, // Default G2 of National Option 100
    TTX_CHARSET_G2_CYRILLIC, // Default G2 of National Option 101
    TTX_CHARSET_G2_LATIN, // Default G2 of National Option 110
    TTX_CHARSET_G2_CYRILLIC, // Default G2 of National Option 111
    TTX_CHARSET_G0_LATIN_ENGLISH,
};  // Default Second G0
#else //Philips Setting

static const uint8_t TTX_ROM DefaultCharSet[17] =
{
    TTX_CHARSET_G0_LATIN_ENGLISH, // Default G0 of National Option 000
    TTX_CHARSET_G0_LATIN_GERMAN, // Default G0 of National Option 001
    TTX_CHARSET_G0_LATIN_SWEDISH_FINNISH, // Default G0 of National Option 010
    TTX_CHARSET_G0_LATIN_ITALIAN, // Default G0 of National Option 011
    TTX_CHARSET_G0_LATIN_FRENCH, // Default G0 of National Option 100
    TTX_CHARSET_G0_LATIN_PORTUGUESE_SPANISH, // Default G0 of National Option 101
    TTX_CHARSET_G0_LATIN_TURKISH, // Default G0 of National Option 110
    TTX_CHARSET_G0_CYRILLIC_OP3, // Default G0 of National Option 111
    TTX_CHARSET_G2_LATIN, // Default G2 of National Option 000
    TTX_CHARSET_G2_LATIN, // Default G2 of National Option 001
    TTX_CHARSET_G2_LATIN, // Default G2 of National Option 010
    TTX_CHARSET_G2_LATIN, // Default G2 of National Option 011
    TTX_CHARSET_G2_CYRILLIC, // Default G2 of National Option 100
    TTX_CHARSET_G2_CYRILLIC, // Default G2 of National Option 101
    TTX_CHARSET_G2_LATIN, // Default G2 of National Option 110
    TTX_CHARSET_G2_CYRILLIC, // Default G2 of National Option 111
    TTX_CHARSET_G0_LATIN_ENGLISH,
};  // Default Second G0

#endif

static const TTX_STD_CharSet_t TTX_ROM G0_CHAR_SET_TABLE[11][8] =
{
    {TTX_CHARSET_G0_LATIN_ENGLISH, TTX_CHARSET_G0_LATIN_GERMAN, TTX_CHARSET_G0_LATIN_SWEDISH_FINNISH, TTX_CHARSET_G0_LATIN_ITALIAN, TTX_CHARSET_G0_LATIN_FRENCH, TTX_CHARSET_G0_LATIN_PORTUGUESE_SPANISH, TTX_CHARSET_G0_LATIN_CZECH_SLOVAK, TTX_CHARSET_G0_LATIN},
    {TTX_CHARSET_G0_LATIN_POLISH, TTX_CHARSET_G0_LATIN_GERMAN, TTX_CHARSET_G0_LATIN_SWEDISH_FINNISH, TTX_CHARSET_G0_LATIN_ITALIAN, TTX_CHARSET_G0_LATIN_FRENCH, TTX_CHARSET_G0_LATIN, TTX_CHARSET_G0_LATIN_CZECH_SLOVAK, TTX_CHARSET_G0_LATIN},
    {TTX_CHARSET_G0_LATIN_ENGLISH, TTX_CHARSET_G0_LATIN_GERMAN, TTX_CHARSET_G0_LATIN_SWEDISH_FINNISH, TTX_CHARSET_G0_LATIN_ITALIAN, TTX_CHARSET_G0_LATIN_FRENCH, TTX_CHARSET_G0_LATIN_PORTUGUESE_SPANISH, TTX_CHARSET_G0_LATIN_TURKISH, TTX_CHARSET_G0_LATIN},
    {TTX_CHARSET_G0_LATIN, TTX_CHARSET_G0_LATIN, TTX_CHARSET_G0_LATIN, TTX_CHARSET_G0_LATIN, TTX_CHARSET_G0_LATIN, TTX_CHARSET_G0_LATIN_SERBIAN_CROATIAN_SLOVENIAN, TTX_CHARSET_G0_LATIN, TTX_CHARSET_G0_LATIN_RUMANIAN},
    {TTX_CHARSET_G0_CYRILLIC_OP1, TTX_CHARSET_G0_LATIN_GERMAN, TTX_CHARSET_G0_LATIN_ESTONIAN, TTX_CHARSET_G0_LATIN_LETTISH_LITHUANIAN, TTX_CHARSET_G0_CYRILLIC_OP2, TTX_CHARSET_G0_CYRILLIC_OP3, TTX_CHARSET_G0_LATIN_CZECH_SLOVAK, TTX_CHARSET_RESERVED},
    {TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED},
    {TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_G0_LATIN_TURKISH, TTX_CHARSET_G0_GREEK},
    {TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED},
    {TTX_CHARSET_G0_LATIN_ENGLISH, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_G0_LATIN_FRENCH, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_G0_ARABIC},
    {TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED},
    {TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_G0_HEBREW, TTX_CHARSET_RESERVED, TTX_CHARSET_G0_ARABIC},
};


static const TTX_STD_CharSet_t TTX_ROM G2_CHAR_SET_TABLE[11][8] =
{
    {TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_LATIN},
    {TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_LATIN},
    {TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_LATIN},
    {TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_LATIN},
    {TTX_CHARSET_G2_CYRILLIC, TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_CYRILLIC, TTX_CHARSET_G2_CYRILLIC, TTX_CHARSET_G2_LATIN, TTX_CHARSET_RESERVED},
    {TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED},
    {TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_G2_LATIN, TTX_CHARSET_G2_GREEK},
    {TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED},
    {TTX_CHARSET_G2_ARABIC, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_G2_ARABIC, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_G2_ARABIC},
    {TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED},
    {TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_G2_ARABIC, TTX_CHARSET_RESERVED, TTX_CHARSET_G2_ARABIC},
};


/* -- Prototypes of Private Functions ---------------------------------------*/
//static bool_t           TTX_STD_TestSoftware                    (void);


/* -- Implementations of Public Functions -----------------------------------*/

/*---------------------------------------------------------------------------*/
#if 0
bool_t
TTX_STD_GetCharSetFromDefaultG0G2       (const uint8_t triplet1, const uint8_t c12c13c14, TTX_STD_CharSet_t* pCharSet)
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
 *  @param   triplet1    bit14~bit8 of triplet1
 *  @param   c12c13c14   national option selection bits
 *  @note    Section 15.2 (P.101)
\*****************************************************************************/
{

    TTX_STD_CharSet_t G0_CHAR_SET_TABLE[11][8] =
    {

        {TTX_CHARSET_G0_LATIN_ENGLISH, TTX_CHARSET_G0_LATIN_GERMAN, TTX_CHARSET_G0_LATIN_SWEDISH_FINNISH, TTX_CHARSET_G0_LATIN_ITALIAN, TTX_CHARSET_G0_LATIN_FRENCH, TTX_CHARSET_G0_LATIN_PORTUGUESE_SPANISH, TTX_CHARSET_G0_LATIN_CZECH_SLOVAK, TTX_CHARSET_G0_LATIN},
        {TTX_CHARSET_G0_LATIN_POLISH, TTX_CHARSET_G0_LATIN_GERMAN, TTX_CHARSET_G0_LATIN_SWEDISH_FINNISH, TTX_CHARSET_G0_LATIN_ITALIAN, TTX_CHARSET_G0_LATIN_FRENCH, TTX_CHARSET_G0_LATIN, TTX_CHARSET_G0_LATIN_CZECH_SLOVAK, TTX_CHARSET_G0_LATIN},
        {TTX_CHARSET_G0_LATIN_ENGLISH, TTX_CHARSET_G0_LATIN_GERMAN, TTX_CHARSET_G0_LATIN_SWEDISH_FINNISH, TTX_CHARSET_G0_LATIN_ITALIAN, TTX_CHARSET_G0_LATIN_FRENCH, TTX_CHARSET_G0_LATIN_PORTUGUESE_SPANISH, TTX_CHARSET_G0_LATIN_TURKISH, TTX_CHARSET_G0_LATIN},
        {TTX_CHARSET_G0_LATIN, TTX_CHARSET_G0_LATIN, TTX_CHARSET_G0_LATIN, TTX_CHARSET_G0_LATIN, TTX_CHARSET_G0_LATIN, TTX_CHARSET_G0_LATIN_SERBIAN_CROATIAN_SLOVENIAN, TTX_CHARSET_G0_LATIN, TTX_CHARSET_G0_LATIN_RUMANIAN},
        {TTX_CHARSET_G0_CYRILLIC_OP1, TTX_CHARSET_G0_LATIN_GERMAN, TTX_CHARSET_G0_LATIN_ESTONIAN, TTX_CHARSET_G0_LATIN_LETTISH_LITHUANIAN, TTX_CHARSET_G0_CYRILLIC_OP2, TTX_CHARSET_G0_CYRILLIC_OP3, TTX_CHARSET_G0_LATIN_CZECH_SLOVAK, TTX_CHARSET_RESERVED},
        {TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED},
        {TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_G0_LATIN_TURKISH, TTX_CHARSET_G0_GREEK},
        {TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED},
        {TTX_CHARSET_G0_LATIN_ENGLISH, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_G0_LATIN_FRENCH, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_G0_ARABIC},
        {TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED},
        {TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_RESERVED, TTX_CHARSET_G0_HEBREW, TTX_CHARSET_RESERVED, TTX_CHARSET_G0_ARABIC},
    };

    if ( ((triplet1>>3)&0x0F) < 11 )
    {
        if ( c12c13c14 < 8 )
        {
            const TTX_STD_CharSet_t result = G0_CHAR_SET_TABLE[(triplet1>>3)&0x0F][c12c13c14];
            if ( pCharSet != NULL )
            {
                *pCharSet = result;
                return( TRUE );
            }
        }
    }

    return( FALSE );
}
#endif
bool_t TTX_STD_GetDefaultG0G2FromNatlOpt (const uint8_t c12c13c14, TTX_STD_CharSet_t* pG0CharSet, TTX_STD_CharSet_t* pG2CharSet)
/*****************************************************************************\
 *  @brief	get default G0 character set from national option
 *  @param	<c12c13c14>	national option selection bits
\*****************************************************************************/
{
    if ( c12c13c14 < 8 )
    {
        if ( pG0CharSet != NULL )
        {
            *pG0CharSet = DefaultCharSet[c12c13c14];
        }
        if ( pG2CharSet != NULL )
        {
            *pG2CharSet = DefaultCharSet[8+c12c13c14];
        }
        return( TRUE );
    }
    return( FALSE );
}

/*---------------------------------------------------------------------------*/
bool_t TTX_STD_GetSecondG0 (TTX_STD_CharSet_t* pCharSet)
/*****************************************************************************\
 *  @brief	get default G0 character set from national option
 *  @param	<c12c13c14>	national option selection bits
\*****************************************************************************/
{
    if ( pCharSet != NULL )
    {
        *pCharSet = DefaultCharSet[16];
        return( TRUE );
    }
    return( FALSE );
}

uint16_t TTX_STD_PageAddrHexToInt(uint16_t pageAddr)
{
    uint8_t mag,  tens,  units;

    mag = pageAddr>>8;
    tens     = ((pageAddr&0xF0)>>4);
    units    = ( pageAddr&0x0F );

    return (mag-1)*100 + tens*10 + units;
}

uint16_t TTX_STD_PageAddrIntToHex(uint16_t pageAddr)
{
    uint16_t ret;
    ret = ((pageAddr / 100)<<8) |
          (((pageAddr / 10)%10) <<4) |
          ((pageAddr)%10);

    return ret;
}

uint8_t TTX_STD_SubCodeAddrHexToInt(uint16_t SubCode)
{
    uint8_t tens,  units;

    tens     = ((SubCode&0xF0)>>4);
    units    = ( SubCode&0x0F );

    return tens*10 + units;
}

uint8_t TTX_STD_SubCodeAddrIntToHex(uint8_t SubCode)
{
    uint8_t ret;
    ret = (((SubCode / 10)%10) <<4) |
          ((SubCode)%10);

    return ret;
}


TTX_STD_Page_Addr_SubCode_t TTX_STD_SubCodeAToSubcode(TTX_STD_Page_Addr_SubCode_Hex_t subcodeA)
{
    TTX_STD_Page_Addr_SubCode_t pageSubcode;

    pageSubcode.s1 = subcodeA&0xF;
    pageSubcode.s2 = (subcodeA&0xF0)>>4;
    pageSubcode.s3 = (subcodeA&0xF00)>>8;
    pageSubcode.s4 = (subcodeA&0xF000)>>12;

    return pageSubcode;
}

TTX_STD_Page_Addr_SubCode_Hex_t TTX_STD_SubCodeToSubcodeA(TTX_STD_Page_Addr_SubCode_t subcode)
{
    TTX_STD_Page_Addr_SubCode_Hex_t subcodeA;

    subcodeA = ((subcode.s4 & 0xF) <<12)|((subcode.s3 & 0xF) <<8)|((subcode.s2 & 0xF) <<4)|(subcode.s1 & 0xF);


    return subcodeA;

}

#if 0
/*---------------------------------------------------------------------------*/
bool_t
TTX_STD_HexToPageAddrNum        (const TTX_STD_Page_Addr_Num_Hex_t pageAddrNum    , TTX_STD_Page_Addr_Number_t*  pPageAddressNumber)
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
 *  @param   triplet1    bit14~bit8 of triplet1
 *  @param   c12c13c14   national option selection bits
 *  @note    Section 15.2 (P.101)
\*****************************************************************************/
{
    if ( pPageAddressNumber != NULL )
    {
        pPageAddressNumber->tens  = (pageAddrNum >> 4);
        pPageAddressNumber->units = (pageAddrNum & 0x0F );

        TTX_ENSURE( pPageAddressNumber->tens <= 0x0F );

        return( TRUE );
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_STD_HexToPageAddrSubCode      (const TTX_STD_Page_Addr_SubCode_Hex_t pageAddrSubCode, TTX_STD_Page_Addr_SubCode_t* pPageAddressSubCode)
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
 *  @param   triplet1    bit14~bit8 of triplet1
 *  @param   c12c13c14   national option selection bits
 *  @note    Section 15.2 (P.101)
\*****************************************************************************/
{
    const uint8_t s1 =  pageAddrSubCode       & 0x0F;
    const uint8_t s2 = (pageAddrSubCode >> 4) & 0x0F;
    const uint8_t s3 = (pageAddrSubCode >> 8) & 0x0F;
    const uint8_t s4 = (pageAddrSubCode >>12) & 0x0F;

    TTX_REQUIRE( s1 <= 0x0F );
    TTX_REQUIRE( s2 <= 0x07 );
    TTX_REQUIRE( s3 <= 0x0F );
    TTX_REQUIRE( s4 <= 0x03 );

    if ( pPageAddressSubCode != NULL )
    {
        pPageAddressSubCode->s1 = s1;
        pPageAddressSubCode->s2 = s2;
        pPageAddressSubCode->s3 = s3;
        pPageAddressSubCode->s4 = s4;

        return( TRUE );
    }

    return( FALSE );
}


/*---------------------------------------------------------------------------*/
bool_t
TTX_STD_HexToPageAddrWithSubCode     (const TTX_STD_Page_AddrWithSubCode_Hex_t pageAddr, TTX_STD_Page_AddrWithSubCode_t* pPageAddress)
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
 *  @param   triplet1    bit14~bit8 of triplet1
 *  @param   c12c13c14   national option selection bits
 *  @note    Section 15.2 (P.101)
\*****************************************************************************/
{
    const TTX_STD_MagazineNum_t              magNum  = (pageAddr >> 24) & 0x0F;
    const TTX_STD_Page_Addr_Num_Hex_t  pageNum = (pageAddr >> 16) & 0xFF;
    const TTX_STD_Page_Addr_SubCode_Hex_t subCode =  pageAddr        & 0xFFFF;

    TTX_REQUIRE( ( 1 <= magNum ) && ( magNum <= 8 ) );

    if ( pPageAddress != NULL )
    {
        bool_t bPageNumOk, bSubCodeOk;

        pPageAddress->magNum = magNum;
        bPageNumOk = TTX_STD_HexToPageAddrNum ( pageNum, &(pPageAddress->pageNum) );
        bSubCodeOk = TTX_STD_HexToPageAddrSubCode( subCode, &(pPageAddress->subCode) );

        return( bPageNumOk && bSubCodeOk );
    }

    return( FALSE );
}
#endif
/*---------------------------------------------------------------------------*/
TTX_STD_Page_Addr_Num_Dec_t
TTX_STD_PageAddrNumToDec  (const TTX_STD_Page_Addr_Number_t* pPageAddressNumber)
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
 *  @param   triplet1    bit14~bit8 of triplet1
 *  @param   c12c13c14   national option selection bits
 *  @note    Section 15.2 (P.101)
\*****************************************************************************/
{
    TTX_STD_Page_Addr_Num_Dec_t ret = 0;
    if ( pPageAddressNumber != NULL )
    {
        TTX_REQUIRE( pPageAddressNumber->tens  <= 0x09 );
        TTX_REQUIRE( pPageAddressNumber->units <= 0x09 );

        ret = (((TTX_STD_Page_Addr_Num_Dec_t)pPageAddressNumber->tens) * 10 + pPageAddressNumber->units);
    }

    return( ret );
}

/*---------------------------------------------------------------------------*/
TTX_STD_Page_Address_Dec_t
TTX_STD_PageAddrToDec        (const TTX_STD_Page_Addr_t* pPageAddress)
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
 *  @param   triplet1    bit14~bit8 of triplet1
 *  @param   c12c13c14   national option selection bits
 *  @note    Section 15.2 (P.101)
\*****************************************************************************/
{
    TTX_STD_Page_Address_Dec_t ret = 0;

    TTX_STD_MagazineNum_t              magNum;
    TTX_STD_Page_Addr_Num_Dec_t  pageNum;

    if ( pPageAddress != NULL )
    {
//        TTX_REQUIRE( TTX_STD_MagNumIsValid( pPageAddress->magNum ) );
        magNum = pPageAddress->magNum;

//        TTX_REQUIRE( ( pPageAddress->pageNum.tens <= 0x9 ) && ( pPageAddress->pageNum.units <= 0x9 ) );
        pageNum = TTX_STD_PageAddrNumToDec  ( &(pPageAddress->pageNum) );

        ret     = ((TTX_STD_Page_Address_Dec_t)magNum)*100 + pageNum;
    }

    return( ret );
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_STD_DecToPageAddrNum  (const TTX_STD_Page_Addr_Num_Dec_t pageAddrNum, TTX_STD_Page_Addr_Number_t* pPageAddressNumber)
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
 *  @param   triplet1    bit14~bit8 of triplet1
 *  @param   c12c13c14   national option selection bits
 *  @note    Section 15.2 (P.101)
\*****************************************************************************/
{
    if ( pPageAddressNumber != NULL )
    {
        pPageAddressNumber->tens  = (pageAddrNum / 10);
        pPageAddressNumber->units = (pageAddrNum % 10);

        TTX_ENSURE( pPageAddressNumber->tens  <= 0x09 );
        TTX_ENSURE( pPageAddressNumber->units <= 0x09 );

        return( TRUE );
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_STD_DecToPageAddr  (const TTX_STD_Page_Address_Dec_t pageAddr, TTX_STD_Page_Addr_t* pPageAddress)
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
 *  @param   triplet1    bit14~bit8 of triplet1
 *  @param   c12c13c14   national option selection bits
 *  @note    Section 15.2 (P.101)
\*****************************************************************************/
{
    TTX_STD_MagazineNum_t        magNum  = (pageAddr/100)%8;
    const TTX_STD_Page_Addr_Num_Dec_t  pageNum =  pageAddr%100;

    if ( magNum == 0 )
        magNum = 8;

    TTX_REQUIRE( ( 1 <= magNum ) && ( magNum <= 8 ) );

    if ( pPageAddress != NULL )
    {
        const bool_t bPageNumOk = TTX_STD_DecToPageAddrNum( pageNum, &(pPageAddress->pageNum) );
        TTX_ENSURE( bPageNumOk );

        pPageAddress->magNum = magNum;

        return( bPageNumOk );
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_STD_RemoveSubCode (const TTX_STD_Page_AddrWithSubCode_t* pFrom, TTX_STD_Page_Addr_t* pTo)
/*****************************************************************************\
 *  @brief   Transforms from TTX_STD_Page_AddrWithSubCode_t to TTX_STD_Page_Addr_t
 *  @param   triplet1    bit14~bit8 of triplet1
 *  @param   c12c13c14   national option selection bits
 *  @note    Section 15.2 (P.101)
\*****************************************************************************/
{
    if ( ( pFrom != NULL ) && ( pTo != NULL ) )
    {
        pTo->magNum  = pFrom->pageA >>8;
        pTo->pageNum.tens = (pFrom->pageA >>4)&0xf;
        pTo->pageNum.units = pFrom->pageA & 0xf;

        return( TRUE );
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
bool_t TTX_STD_ParseCtrlBits (const uint16_t input, TTX_STD_Page_CtrlBits_t *output)
/*****************************************************************************\
 *  @brief   Parse control bits
\*****************************************************************************/
{
    if ( output == NULL )
        return FALSE;

    output->bErasePage = ((input>>14)&0x1); // C4
    output->bNewsFlash = ((input>>13)&0x1); // C5
    output->bSubtitle = ((input>>12)&0x1); // C6
    output->bSuppressHeader = ((input>>11)&0x1); // C7
    output->bUpdate = ((input>>10)&0x1); // C8
    output->bInterruptedSequence = ((input>>9)&0x1); // C9
    output->bInhibitDisplay = ((input>>8)&0x1); // C10
    output->bSerialMode = ((input>>3)&0x1); // C11
    output->natlOpt = (input&0x7); // C12, C13, C14

    return TRUE;
}

/*---------------------------------------------------------------------------*/
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

#if 0
/*---------------------------------------------------------------------------*/
bool_t
TTX_STD_isPageTerminator                (const TTX_STD_Page_AddrWithSubCode_t*  pPageAddress)
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
 *  @param   triplet1    bit14~bit8 of triplet1
 *  @param   c12c13c14   national option selection bits
 *  @note    Section 15.2 (P.101)
\*****************************************************************************/
{
    TTX_REQUIRE( pPageAddress != NULL );

    if ( pPageAddress != NULL )
    {
        if ( ( pPageAddress->pageNum.tens == 0xF ) && (pPageAddress->pageNum.units == 0xF ) )
        {
            return( TRUE );
        }
    }

    return( FALSE );
}


/*---------------------------------------------------------------------------*/
bool_t
TTX_STD_canHaveSubPage (const TTX_STD_Page_AddrWithSubCode_t*  pPageAddress)
/*****************************************************************************\
 *  @brief   Indicate whether *pPageAddress has sub-page(s) associated.
 *  @details Implements Clause A.1 (p.120).
 *  @param   *pPageAddress is the page address of the page you want to query.
 *  @retval  TRUE if *pPageAddress has one or more subpages associated.
\*****************************************************************************/
{
    return( TTX_STD_PageAddrSubCodeToHex( &(pPageAddress->subCode) ) != 0 );
}
#endif


/*---------------------------------------------------------------------------*/
bool_t
TTX_STD_Page_AddrIsValid             (const TTX_STD_Page_AddrWithSubCode_t*  pPageAddress)
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
 *  @param   triplet1    bit14~bit8 of triplet1
 *  @param   c12c13c14   national option selection bits
 *  @note    Section 15.2 (P.101)
\*****************************************************************************/
{
    bool_t bMagOk, bPageNumOk, bSubCodeOk;
    bMagOk     =  pPageAddress->pageA >> 8 ;
    //bPageNumOk = TTX_STD_PageAddrNumberIsValid ( (pPageAddress->pageA >> 4) );
    //bSubCodeOk = TTX_STD_PageAddrSubCodeIsValid( pPageAddress->subCode );
    bPageNumOk = 1;
    bSubCodeOk = 1;
    return( bMagOk && bPageNumOk && bSubCodeOk );

}


bool_t TTX_STD_PageAddrSubCodeIsValid (uint16_t subcode)
{
    if ((subcode>0) &&
            ((subcode&0xF000) <0x3000) &&
            ((subcode&0x0F00) <0xF00) &&
            ((subcode&0x00F0) <0x70) &&
            ((subcode&0x000F) <0xF))
        return TRUE;
    return FALSE;
}

#if 0  /* if enabled verficiation of TTX related IPs */

/*---------------------------------------------------------------------------*/
bool_t
TTX_STD_OddParityCheck                  (const uint8_t val)
/*****************************************************************************\
 *  @brief   perform odd-parily check for the 8-bit data.
 *  @details Use this function to perform odd-parily check for the 8-bit data.
 *  @note    <ol>
 *              <li>To decoder, whether the odd-parity bit is located in
 *                    MSB or LSB doesn't matter.</li>
 *              <li>This function is the same as Keith's TTUtil_OddParityCheck().</li>
 *           </ol>
 *  @param   data the odd-parity encoded data with odd-parity bit located at MSB.
 *
 *  @return  decoding status
 *  @retval  true if success
 *  @retval  false if failure
 *
 *  @pre     data: the lower 7-bit are the actual data, the MSB is the
 *                   odd-parity bit.
 *  @post    no external data will be modified.
 *
 *  @ingroup Tim
 *  @see     TTApi_DeInit()
\*****************************************************************************/
{
    uint8_t i, odd_parity = 0;

    for ( i=0 ; i<8 ; i++ )
        odd_parity ^= (val>>i) & 1;

    return( (odd_parity == 0)? FALSE : TRUE );
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_STD_Hamming84Decode                 (uint8_t* des, const uint8_t  src)
/*****************************************************************************\
 *  @brief   Hamming 8/4 decoder routine.
 *  @details Use this function to decode Hamming 8/4 encoded data.
 *  @param   des  points to the destination to store the decoded *src
 *  @param   src  points to the source Hamming 8/4 encoded data
 *
 *  @return  the decode status
 *  @retval  TRUE  success
 *  @retval  FALSE failure
 *
 *  @ingroup Tim
 *  @see     TTApi_DeInit()
 *  @todo
 *           nothing to do.
\*****************************************************************************/
{
    uint8_t A, B, C, D;
    uint8_t D1, D2, D3, D4;
    uint8_t P1, P2, P3, P4;
    uint8_t hamming_value = 0;
    bool_t success = TRUE;

    //BIT  0  1   2   3   4   5   6   7
    //    P1  D1  P2  D2  P3  D3  P4  D4
    P1 = src&0x01;
    D1 = (src>>1)&0x01;
    P2 = (src>>2)&0x01;
    D2 = (src>>3)&0x01;
    P3 = (src>>4)&0x01;
    D3 = (src>>5)&0x01;
    P4 = (src>>6)&0x01;
    D4 = (src>>7)&0x01;

    A = P1 ^ D1 ^ D3 ^ D4;
    B = D1 ^ P2 ^ D2 ^ D4;
    C = D1 ^ D2 ^ P3 ^ D3;
    D = P1 ^ D1 ^ P2 ^ D2 ^ P3 ^ D3 ^ P4 ^ D4;

    hamming_value = (A<<3)+(B<<2)+(C<<1)+D;

    switch (hamming_value)
    {
    case 0: //D1
        D1 = ~D1&0x01;
        break;
    case 2: //D4
        D4 = ~D4&0x01;
        break;
    case 4: //D3
        D3 = ~D3&0x01;
        break;
    case 6: //P1
        P1 = ~P1&0x01;
        break;
    case 8: //D2
        D2 = ~D2&0x01;
        break;
    case 10: //P2
        P2 = ~P2&0x01;
        break;
    case 12: //P3
        P3 = ~P3&0x01;
        break;
    case 14: //P4
        P4 = ~P4&0x01;
        break;
    case 15://no error
        break;
    default: //double error
        success = FALSE;
        break;
    }

    if ( success == TRUE )
        *des = D1 + (D2<<1) + (D3<<2) + (D4<<3);

    return( success );

}

/*---------------------------------------------------------------------------*/
bool_t
TTX_STD_Hamming2418Decode               (uint8_t* des, const uint8_t* src)
/*****************************************************************************\
 *  @brief   Hamming 24/18 decoder routine.
 *  @details Use this function to decode Hamming 24/18 encoded data.
 *  @param   des  points to the destination to store the decoded *src
 *  @param   src  points to the source Hamming 24/18 encoded data
 *
 *  @return  the decode status
 *  @retval  true  success
 *  @retval  false failure
 *
 *  @ingroup Tim
 *  @see     TTApi_DeInit()
 *  @todo
 *           nothing to do.
\*****************************************************************************/
{
    uint8_t A,B,C,D,E,F;
    uint8_t P1,P2,P3,P4,P5,P6;
    uint8_t D1,D2,D3,D4,D5,D6;
    uint8_t D7,D8,D9,D10,D11,D12;
    uint8_t D13,D14,D15,D16,D17,D18;
    uint8_t hamming_value=0;
    bool_t  success = TRUE;

    P1 = src[0]&0x01;
    P2 = (src[0]>>1)&0x01;
    D1 = (src[0]>>2)&0x01;
    P3 = (src[0]>>3)&0x01;
    D2 = (src[0]>>4)&0x01;
    D3 = (src[0]>>5)&0x01;
    D4 = (src[0]>>6)&0x01;
    P4 = (src[0]>>7)&0x01;

    D5 = src[1]&0x01;
    D6 = (src[1]>>1)&0x01;
    D7 = (src[1]>>2)&0x01;
    D8 = (src[1]>>3)&0x01;
    D9 = (src[1]>>4)&0x01;
    D10 = (src[1]>>5)&0x01;
    D11 = (src[1]>>6)&0x01;
    P5 = (src[1]>>7)&0x01;

    D12 = src[2]&0x01;
    D13 = (src[2]>>1)&0x01;
    D14 = (src[2]>>2)&0x01;
    D15 = (src[2]>>3)&0x01;
    D16 = (src[2]>>4)&0x01;
    D17 = (src[2]>>5)&0x01;
    D18 = (src[2]>>6)&0x01;
    P6 = (src[2]>>7)&0x01;

    A = P1 ^ D1 ^ D2 ^ D4 ^ D5 ^ D7 ^ D9 ^ D11 ^ D12 ^ D14 ^ D16 ^ D18;
    B = P2 ^ D1 ^ D3 ^ D4 ^ D6 ^ D7 ^ D10 ^ D11 ^ D13 ^ D14 ^ D17 ^ D18;
    C = P3 ^ D2 ^ D3 ^ D4 ^ D8 ^ D9 ^ D10 ^ D11 ^ D15 ^ D16 ^ D17 ^ D18;
    D = P4 ^ D5 ^ D6 ^ D7 ^ D8 ^ D9 ^ D10 ^ D11;
    E = P5 ^ D12 ^ D13 ^ D14 ^ D15 ^ D16 ^ D17 ^ D18;
    F = P1 ^ P2 ^ D1 ^ P3 ^ D2 ^ D3 ^ D4 ^ P4 ^ D5 ^ D6 ^ D7 ^ D8 ^ D9 ^ D10 ^ D11 ^ P5 ^ D12 ^ D13 ^ D14 ^ D15 ^ D16 ^ D17 ^ D18 ^ P6;

    hamming_value = A + (B<<1) + (C<<2) + (D<<3) + (E<<4) + (F<<5);

    switch (hamming_value)
    {
    case 0x3f:
        break; // no error
    case 0x1f:
        P6 = ~P6&0x01;
        break; // P6
    default:

        return( FALSE );// Added by Janus: To emulate the error handling behavior of RTD2638.
        /*
        // Commented by Janus: Suppose we shall make the following error correction. However,
        //                       to emulate the error handling behavior of RTD2638, we cannot
        //                       but abort it.
        {
            uint_fast8_t err_pos;
            if (F) {
                success = FALSE;
                return( success );
            } else {
                err_pos = 0;

                if (!A)
                    err_pos|= 0x1 << 0;
                if (!B)
                    err_pos|= 0x1 << 1;
                if (!C)
                    err_pos|= 0x1 << 2;
                if (!D)
                    err_pos|= 0x1 << 3;
                if (!E)
                    err_pos|= 0x1 << 4;

                switch (err_pos)
                {
                case 1: P1 = ~P1&0x01; break;  // P1
                case 2: P2 = ~P2&0x01; break;  // P2
                case 3: D1 = ~D1&0x01; break;  // D1
                case 4: P3 = ~P3&0x01; break;  // P3
                case 5: D2 = ~D2&0x01; break;  // D2
                case 6: D3 = ~D3&0x01; break;  // D3
                case 7: D4 = ~D4&0x01; break;  // D4
                case 8: P4 = ~P4&0x01; break;  // P4
                case 9: D5 = ~D5&0x01; break;  // D5
                case 10: D6 = ~D6&0x01; break;  // D6
                case 11: D7 = ~D7&0x01; break;  // D7
                case 12: D8 = ~D8&0x01; break;  // D8
                case 13: D9 = ~D9&0x01; break;  // D9
                case 14: D10 = ~D10&0x01; break;  // D10
                case 15: D11 = ~D11&0x01; break;  // D11
                case 16: P5 = ~P5&0x01; break;  // P5
                case 17: D12 = ~D12&0x01; break;  // D12
                case 18: D13 = ~D13&0x01; break;  // D13
                case 19: D14 = ~D14&0x01; break;  // D14
                case 20: D15 = ~D15&0x01; break;  // D15
                case 21: D16 = ~D16&0x01; break;  // D16
                case 22: D17 = ~D17&0x01; break;  // D17
                case 23: D18 = ~D18&0x01; break;  // D18
                }
            }
        }*/
        break;
    }

    des[0] = des[1] = des[2] = 0;

    des[0] = D1 + (D2<<1) + (D3<<2) + (D4<<3) + (D5<<4) + (D6<<5);
    des[1] = D7 + (D8<<1) + (D9<<2) + (D10<<3) + (D11<<4);
    des[2] = D12 + (D13<<1) + (D14<<2) + (D15<<3) + (D16<<4) + (D17<<5) + (D18<<6);

    return( success );
}

#endif  /* end of #if TTX_VERIFY */

/* -- Implementations of Private Functions ----------------------------------*/



/*---------------------------------------------------------------------------*\
 * end of ttx_std.c                                                          *
\*---------------------------------------------------------------------------*/
#endif
