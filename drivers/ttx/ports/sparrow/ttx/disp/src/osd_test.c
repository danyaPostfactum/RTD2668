/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     osd_test.c
 * @brief    Prototypes of public application interface.
 * @details  This header declares the public access application interface for this TtLib library.
 * @author   Janus Cheng
 * @date     Dec. 11, 2007
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/

#include "osd.h"            /* Provides: functions to be implemented */
#include <stdlib.h>         /* Provides: NULL */
#include <string.h>         /* Provides: memcpy(), memcmp() and strlen() */
#include "vd.h"             /* Provides: VD_Reset() */

#include <ttx.h>            /* Provides: TTX_DumpMem() */
#include <ttx_disp.h>       /* Provides: TTX_DISP_Attr_t */
#include "sys.h"            /* Provides: SYS_SDMA_Copy() */
#include <ttx_source.h>     /* Provides: TTX_SRC_Open() */


#if !OSD_SOFTWARE_EMULATOR
#include <vd_hal_test.h>    /* Provides: Test for <vd_hal.h> */
#include <osd_hal_test.h>   /* Provides: Test for <osd_hal.h> */
#endif  // end of #if !OSD_SOFTWARE_EMULATOR

#define TO_FLASH_ADDR(bank,addr)    (((uint32_t)bank)<<16|addr|0x10000000u) // "OR 0x10000000" means Flash

TTX_DEFINE_THIS_MODULE( osd_test.c )

/* -- Prototypes of Private Functions ---------------------------------------*/
#if OSD_VERIFY  /* if enabled verficiation of TTX related IPs */
static void     OSD_TestRegsReadWrite   (void);
static void     OSD_TestSoftware        (void);
static void     OSD_TestDisplaySimple0  (void);
static void     OSD_TestDisplaySimple1  (void);
static void     OSD_TestDisplaySimple2  (void);
static void     OSD_TestDisplaySimple3  (void);
static void     OSD_TestLatinNatlOpt0   (void);
static bool_t   TtAttrEqual             (const TTX_DISP_Attr_t* pAttr1, const TTX_DISP_Attr_t* pAttr2);
static bool_t   TtAttrCopy              (      TTX_DISP_Attr_t* pTo   , const TTX_DISP_Attr_t* pFrom );
#endif  /* end of #if OSD_VERIFY */

extern uint8_t const TTX_ROM OSD_ttFont[][96][16];

#if OSD_VERIFY  /* if enabled verficiation of TTX related IPs */

static void delay1ms(int count)
{
    volatile int i,j;

    for (i=0; i<count; i++)
    {
        for (j=0; j<4; j++)
        {
        }
    }
}

/* -- Implementations of Public Functions -----------------------------------*/

/*---------------------------------------------------------------------------*/
bool_t
OSD_Test (void)
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
    // Test programming errors
    OSD_TestSoftware();

    // Register read-write tests
    OSD_TestRegsReadWrite();

    {
        _OSD_REG_WRITE( 0x4734, 0x000000F1 );   // Enable mixer
        _SYS_REG_WRITE( 0x2DA1, 0x00 );         // set IMEM to TT only
    }



    // Display color bar using OSD-internal pattern gen
//    OSD_TestDisplaySimple0();

    // Display golden pattern #1
//    OSD_TestDisplaySimple1();


    // Display golden pattern #2: font in SRAM
    OSD_TestDisplaySimple2();
    delay1ms( 5000 );

    /*
        // Display golden pattern #3: font in Flash ROM
        OSD_TestDisplaySimple3();
        delay1ms( 3000 );
    */
    /*
        // Test Latin National Option Subsets
        OSD_TestLatinNatlOpt0();
        delay1ms( 3000 );
    */

    return( TRUE );
}

#endif  /* end of #if OSD_VERIFY */

/*---------------------------------------------------------------------------*/
void
OSD_TestPwrOnDefaults   (void)
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
\*****************************************************************************/
{
#if !OSD_SOFTWARE_EMULATOR

    OSD_Log( LOGGER_VERBOSE, "[OSD]" "[Trace]" "Entering OSD_TestPwrOnDefaults()... ");

#define     OSD_DEFAULT_TEST(reg)  OSD_ENSURE( OSD_RGET( reg ) == OSD_##reg##_DEFAULT )

    OSD_DEFAULT_TEST( CTRL1 );
    OSD_DEFAULT_TEST( CTRL2 );
    OSD_DEFAULT_TEST( CTRL3 );
    OSD_DEFAULT_TEST( CTRL4 );
    OSD_DEFAULT_TEST( CTRL5 );
    OSD_DEFAULT_TEST( CSBMPST );
    OSD_DEFAULT_TEST( ATTST1 );
    OSD_DEFAULT_TEST( UPDATE );
    OSD_DEFAULT_TEST( P1CTRL );
    OSD_DEFAULT_TEST( P1PNUM_CTRL );
    OSD_DEFAULT_TEST( P1PNUM_CINDEX1 );
    OSD_DEFAULT_TEST( P1PNUM_CINDEX2 );
    OSD_DEFAULT_TEST( P1MSG_CTRL1 );
    OSD_DEFAULT_TEST( P1MSG_CTRL2 );
    OSD_DEFAULT_TEST( P1MSG_CINDEX1 );
    OSD_DEFAULT_TEST( P1MSG_CINDEX2 );
    OSD_DEFAULT_TEST( P1MSG_CINDEX3 );
    OSD_DEFAULT_TEST( ITG1 );
    OSD_DEFAULT_TEST( P1TC_INIT );
    OSD_DEFAULT_TEST( CLUT0_ENTRY0 );   // 0x4900
    OSD_DEFAULT_TEST( CLUT0_ENTRY1 );
    OSD_DEFAULT_TEST( CLUT0_ENTRY2 );
    OSD_DEFAULT_TEST( CLUT0_ENTRY3 );
    OSD_DEFAULT_TEST( CLUT0_ENTRY4 );
    OSD_DEFAULT_TEST( CLUT0_ENTRY5 );
    OSD_DEFAULT_TEST( CLUT0_ENTRY6 );
    OSD_DEFAULT_TEST( CLUT0_ENTRY7 );
    OSD_DEFAULT_TEST( DM_MSB00 );
    OSD_DEFAULT_TEST( DM_MSB01 );
    OSD_DEFAULT_TEST( DM_MSB02 );
    OSD_DEFAULT_TEST( DM_MSB03 );
    OSD_DEFAULT_TEST( DM_MSB04 );
    OSD_DEFAULT_TEST( DM_MSB05 );
    OSD_DEFAULT_TEST( DM_MSB06 );
    OSD_DEFAULT_TEST( DM_MSB07 );
    OSD_DEFAULT_TEST( DM_MSB08 );
    OSD_DEFAULT_TEST( DM_MSB09 );
    OSD_DEFAULT_TEST( DM_MSB10 );
    OSD_DEFAULT_TEST( DM_MSB11 );
    OSD_DEFAULT_TEST( DM_MSB12 );
    OSD_DEFAULT_TEST( DM_MSB13 );
    OSD_DEFAULT_TEST( DM_MSB14 );
    OSD_DEFAULT_TEST( DM_MSB15 );
    OSD_DEFAULT_TEST( DM_LSB0 );
    OSD_DEFAULT_TEST( DM_LSB1 );
    OSD_DEFAULT_TEST( FMSYNC_CTRL1 );

    OSD_Log( LOGGER_VERBOSE, "[done]\n");

#endif  // end of #if !OSD_SOFTWARE_EMULATOR

}



/* -- Implementations of Private Functions ----------------------------------*/

#if 0  /* if enabled verficiation of TTX related IPs */

/*---------------------------------------------------------------------------*/
static void
OSD_TestRegsReadWrite   (void)
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
\*****************************************************************************/
{
    OSD_Log( LOGGER_VERBOSE, "[OSD]" "[Trace]" "Entering OSD_TestRegsReadWrite()... ");

    {
        OSD_Address_t orig;

        orig = OSD_GetTtCharsetBitmapAddress(); // store original value

        OSD_SetTtCharsetBitmapAddress( (OSD_Address_t)OSD_ttFont );
        OSD_ENSURE( OSD_GetTtCharsetBitmapAddress() == (OSD_Address_t)OSD_ttFont );

        OSD_SetTtCharsetBitmapAddress( orig );  // restore original value
    }

    /* This is a latch-double-flip-flop register, cannot be verify above writes immediately here.
    {
        OSD_SetTtAttr1Address( 0x5A5A );
        OSD_ENSURE( OSD_GetTtAttr1Address() == 0x5A5A );
    }
    */

    OSD_Log( LOGGER_VERBOSE, "[done]\n");
}

/*---------------------------------------------------------------------------*/
static void
OSD_TestSoftware        (void)
/*****************************************************************************\
 *  @brief   Test programming errors.
\*****************************************************************************/
{
    OSD_Log( LOGGER_VERBOSE, "[OSD]" "[Trace]" "Entering OSD_TestSoftware()... ");

    // Test TO_FLASH_ADDR()
    {
        TTX_REQUIRE( TO_FLASH_ADDR(0x1F,0x1234) == 0x101F1234 );
    }

    // Test TtAttrCopy() and TtAttrEqual()
    {
        bool_t          bOk;
        TTX_DISP_Attr_t attr1, attr2;

        attr1.dmIdx = TTX_DIAC_MARK_2;
        attr2.dmIdx = TTX_DIAC_MARK_1;
        OSD_ENSURE( !TtAttrEqual(&attr1, &attr2) );

        attr1.conceal    = TRUE;
        attr1.box        = FALSE;
        attr1.graphic    = FALSE;
        attr1.size       = TTX_STD_SIZE_CFG_DS_R_UPPER;
        attr1.bUnderline = FALSE;
        attr1.mosaics    = TTX_MOSAICS_CON;
        attr1.dmIdx      = TTX_DIAC_MARK_14;
        attr1.flashMode  = TTX_FLASH_STEADY;
        attr1.fgColorIdx = 0x04;
        attr1.bgColorIdx = 0x05;
        attr1.charset    = TTX_CHARSET_G0_LATIN_ESTONIAN;
        attr1.charIdx    = 0x6A;

        bOk = TtAttrCopy( &attr2, &attr1 ); // copy attr1 to attr2.

        OSD_ENSURE( bOk == TRUE );

        OSD_ENSURE( attr1.conceal    == attr2.conceal    );
        OSD_ENSURE( attr1.box        == attr2.box        );
        OSD_ENSURE( attr1.graphic    == attr2.graphic    );
        OSD_ENSURE( attr1.size       == attr2.size       );
        OSD_ENSURE( attr1.bUnderline == attr2.bUnderline );
        OSD_ENSURE( attr1.mosaics    == attr2.mosaics    );
        OSD_ENSURE( attr1.dmIdx      == attr2.dmIdx      );
        OSD_ENSURE( attr1.flashMode  == attr2.flashMode  );
        OSD_ENSURE( attr1.fgColorIdx == attr2.fgColorIdx );
        OSD_ENSURE( attr1.bgColorIdx == attr2.bgColorIdx );
        OSD_ENSURE( attr1.charset    == attr2.charset    );
        OSD_ENSURE( attr1.charIdx    == attr2.charIdx    );

        OSD_ENSURE( TtAttrEqual(&attr1, &attr2) );

        attr1.flashMode = !attr1.flashMode; // Force attr1 != attr2.
        OSD_ENSURE( !TtAttrEqual(&attr1, &attr2) );
    }

    // Test TTX_DISP_TtAttrCselToFontOffset()
    {
        TTX_STD_CharSet_t   inCharSet;
        TTX_STD_CharIndex_t inCharIndex;
        uint8_t outCharSet;
        TTX_STD_CharIndex_t outCharIndex;

        // Case-1
        {
            inCharSet   = TTX_CHARSET_G0_LATIN_ESTONIAN;
            inCharIndex = 0x40;

            TTX_DISP_TtAttrCselToFontOffset( inCharSet, inCharIndex, &outCharSet, &outCharIndex );

            OSD_ENSURE( outCharSet   == 4 );
            OSD_ENSURE( outCharIndex == 0x42 );
        }

        // Case-2
        {
            inCharSet   = TTX_CHARSET_G0_LATIN_TURKISH;
            inCharIndex = 0x7E;

            TTX_DISP_TtAttrCselToFontOffset( inCharSet, inCharIndex, &outCharSet, &outCharIndex );

            OSD_ENSURE( outCharSet   == 6 );
            OSD_ENSURE( outCharIndex == 0x2C );
        }

        // Case-3
        {
            inCharSet   = TTX_CHARSET_G0_LATIN_TURKISH;
            inCharIndex = 0x7F;

            TTX_DISP_TtAttrCselToFontOffset( inCharSet, inCharIndex, &outCharSet, &outCharIndex );

            OSD_ENSURE( outCharSet   == 0 );
            OSD_ENSURE( outCharIndex == 0x7F );
        }

        // Case-4
        {
            inCharSet   = TTX_CHARSET_G0_LATIN_PORTUGUESE_SPANISH;
            inCharIndex = 0x5C;

            TTX_DISP_TtAttrCselToFontOffset( inCharSet, inCharIndex, &outCharSet, &outCharIndex );

            OSD_ENSURE( outCharSet   == 5 );
            OSD_ENSURE( outCharIndex == 0x44 );
        }

        // Case-5
        {
            inCharSet   = TTX_CHARSET_G2_LATIN;
            inCharIndex = 0x5D;

            TTX_DISP_TtAttrCselToFontOffset( inCharSet, inCharIndex, &outCharSet, &outCharIndex );

            OSD_ENSURE( outCharSet   == 1 );
            OSD_ENSURE( outCharIndex == 0x5D );
        }
    }

    // Test TTX_DISP_FontOffsetToTtAttrCsel()
    {
        TTX_STD_CharSet_t   outCharSet;
        TTX_STD_CharIndex_t outCharIndex;
        uint8_t             inCharSet;
        uint8_t             inCharIndex;

        // Case-1
        {
            inCharSet   = 5;
            inCharIndex = 0x4B;
            TTX_DISP_FontOffsetToTtAttrCsel( inCharSet, inCharIndex, &outCharSet, &outCharIndex );

            OSD_ENSURE( outCharSet   == TTX_CHARSET_G0_LATIN_PORTUGUESE_SPANISH );
            OSD_ENSURE( outCharIndex == 0x7D );
        }

        // Case-2
        {
            inCharSet   = 6;
            inCharIndex = 0x22;
            TTX_DISP_FontOffsetToTtAttrCsel( inCharSet, inCharIndex, &outCharSet, &outCharIndex );

            OSD_ENSURE( outCharSet   == TTX_CHARSET_G0_LATIN_TURKISH );
            OSD_ENSURE( outCharIndex == 0x40 );
        }

        // Case-3
        {
            inCharSet   = 4;
            inCharIndex = 0x20;
            TTX_DISP_FontOffsetToTtAttrCsel( inCharSet, inCharIndex, &outCharSet, &outCharIndex );

            OSD_ENSURE( outCharSet   == TTX_CHARSET_G0_LATIN_CZECH_SLOVAK );
            OSD_ENSURE( outCharIndex == 0x23 );
        }
    }

    // Test OSD_TtAttrToUint32() and OSD_Uint32ToTtAttr()
    {
        bool_t             bOk;
        TTX_DISP_Attr_t    attr1, attr2;
        OSD_TtAttr_t raw;

        // Case-1
        {
            attr1.conceal    = TRUE;
            attr1.box        = TRUE;
            attr1.graphic    = TRUE;
            attr1.size       = TTX_STD_SIZE_CFG_DS_R_LOWER;
            attr1.bUnderline = TRUE;
            attr1.mosaics    = TRUE;
            attr1.dmIdx      = TTX_DIAC_MARK_15;
            attr1.flashMode  = TRUE;
            attr1.fgColorIdx = 0x07;
            attr1.bgColorIdx = 0x07;
            attr1.charset    = TTX_CHARSET_G0_LATIN_LETTISH_LITHUANIAN;
            attr1.charIdx    = 0x7F;

            bOk = TTX_DISP_AttrToOsdAttr(&attr1, &raw);
            OSD_ENSURE( bOk == TRUE );
            OSD_ENSURE( raw == 0xEF7FFE7F );

            bOk = TTX_DISP_OsdAttrToAttr(&raw, &attr2);
            OSD_ENSURE( bOk == TRUE );

            OSD_ENSURE( ! TtAttrEqual(&attr1, &attr2) );  // attr1 != attr2

            attr2.charset = attr1.charset;

            OSD_ENSURE( TtAttrEqual(&attr1, &attr2) );  // attr1 == attr2
        }

        // Case-2
        {
            attr1.conceal    = TRUE;
            attr1.box        = FALSE;
            attr1.graphic    = FALSE;
            attr1.size       = TTX_STD_SIZE_CFG_DS_R_UPPER;
            attr1.bUnderline = FALSE;
            attr1.mosaics    = FALSE;
            attr1.dmIdx      = 0x1E;
            attr1.flashMode  = FALSE;
            attr1.fgColorIdx = 0x04;
            attr1.bgColorIdx = 0x05;
            attr1.charset    = TTX_CHARSET_G0_LATIN_ESTONIAN;
            attr1.charIdx    = 0x6A;

            bOk = TTX_DISP_AttrToOsdAttr(&attr1, &raw);
            OSD_ENSURE( bOk == TRUE );
            OSD_ENSURE( raw == 0x8D1E4A6A );

            bOk = TTX_DISP_OsdAttrToAttr(&raw, &attr2);
            OSD_ENSURE( bOk == TRUE );

            OSD_ENSURE( ! TtAttrEqual(&attr1, &attr2) );  // attr1 != attr2

            attr2.charset = attr1.charset;

            OSD_ENSURE( TtAttrEqual(&attr1, &attr2) );  // attr1 == attr2
        }

        // Case-3
        {
            attr1.conceal    = FALSE;
            attr1.box        = TRUE;
            attr1.graphic    = FALSE;
            attr1.size       = 0x0A;
            attr1.bUnderline = FALSE;
            attr1.mosaics    = TRUE;
            attr1.dmIdx      = TTX_DIAC_MARK_13;
            attr1.flashMode  = TTX_FLASH_NORMAL;
            attr1.fgColorIdx = 0x07;
            attr1.bgColorIdx = 0x06;
            attr1.charset    = TTX_CHARSET_G0_LATIN_ITALIAN;
            attr1.charIdx    = 0x4B;

            bOk = TTX_DISP_AttrToOsdAttr(&attr1, &raw);
            OSD_ENSURE( bOk == TRUE );
            OSD_ENSURE( raw == 0x4A3DFC4B );

            bOk = TTX_DISP_OsdAttrToAttr(&raw, &attr2);
            OSD_ENSURE( bOk == TRUE );

            OSD_ENSURE( ! TtAttrEqual(&attr1, &attr2) );  // attr1 != attr2

            attr2.charset = attr1.charset;

            OSD_ENSURE( TtAttrEqual(&attr1, &attr2) );  // attr1 == attr2
        }
    }


    OSD_Log( LOGGER_VERBOSE, "[done]\n");
}

/*---------------------------------------------------------------------------*/
static void
OSD_TestDisplaySimple0  (void)
/*****************************************************************************\
 *  @brief   Display color bar by using OSD internal pattern generator on VD.
\*****************************************************************************/
{
#if !OSD_SOFTWARE_EMULATOR

    /*
        // Display an attribute on screen
        OSD_TtAttr_t attr;

        OSD_Open( OSD_OPEN_RESET );

        attr.conceal  = TRUE;
        attr.box      = TRUE;
        attr.graphic  = TRUE;
        attr.size     = 0x0F;
        attr.ol       = TRUE;
        attr.ul       = TRUE;
        attr.separate = TRUE;
        attr.dm       = 0x1F;
        attr.flash    = TRUE;
        attr.fgc      = 0x07;
        attr.bgc      = 0x07;
        attr.csel     = 0x03;
        attr.cindex   = 'A';

        OSD_Close();
    */

    size_t i;
    volatile _OSD_ITG3_TYPE  lncnts[2];
    const volatile _OSD_CTRL1_TYPE ctrl1 = OSD_RGET( CTRL1 );
    const volatile _OSD_ITG1_TYPE  itg1  = OSD_RGET( ITG1 );;

    OSD_Log( LOGGER_VERBOSE, "[OSD]" "[Trace]" "Entering OSD_TestDisplaySimple0()... ");
    VD_Reset();

    OSD_FSETS( CTRL1, VTYPE, INTL );    // Set interlace mode
    OSD_FSETS( CTRL1, PATEN, ENABLE );  // Enable pattern generator
    OSD_ENSURE( OSD_FGET( CTRL1, VTYPE ) == OSD_CTRL1_VTYPE_INTL   );
    OSD_ENSURE( OSD_FGET( CTRL1, PATEN ) == OSD_CTRL1_PATEN_ENABLE );

    // Set FPGABP=1 and HVSMODE = 0
    OSD_FSETS( ITG1, FPGABP, FPGA );
    OSD_FSETS( ITG1, HVSMODE, 0 );  // VSync is generated by TG or from outside
    OSD_ENSURE( OSD_FGET( ITG1, FPGABP  ) == OSD_ITG1_FPGABP_FPGA );
    OSD_ENSURE( OSD_FGET( ITG1, HVSMODE ) == OSD_ITG1_HVSMODE_0   );

    // Show pattern on screen to allow people to see
    delay1ms( 10000 );

    // LNCNT: will increase by 1 whenever HSync and been reset whenever VSync.
    for ( i=0; i<10; i++ )
    {
        lncnts[0] = OSD_FGET( ITG3, LNCNT );
        delay1ms( 1 );
        lncnts[1] = OSD_FGET( ITG3, LNCNT );
        if ( lncnts[0] == lncnts[1] )
        {
            OSD_Log( LOGGER_ERROR, "[OSD][ERROR] line cntr doesn't change: 0x%X\n", (uint32_t)lncnts[0]);
        }
    }

    // restore original contents
    OSD_RSET( CTRL1, ctrl1 );
    OSD_RSET( ITG1 , itg1  );

    OSD_Log( LOGGER_VERBOSE, "[done]\n");
#endif  // end of #if !OSD_SOFTWARE_EMULATOR

}

/*---------------------------------------------------------------------------*/
static void
OSD_TestDisplaySimple1  (void)
/*****************************************************************************\
 *  @brief   Display color bar by using OSD internal pattern generator on VD.
\*****************************************************************************/
{
#if !OSD_SOFTWARE_EMULATOR

    extern uint8_t const TTX_ROM OSD_testSimple1Attr[];

    const uint16_t  addrOfAtt = 0xC000;
    const uint16_t  sizeOfAtt = 4*26*40*sizeof(uint8_t);

    /* Copy using SPI-DMA
        {
            uint16_t i;

            // Copy Attributes to SRAM@0xC000 through SPI-DMA.
            OSD_REQUIRE( OSD_testSimple1Attr[sizeOfAtt-2] == 0x70 );
            SYS_SDMA_Copy( (void TTX_MEM*)(addrOfAtt), TTX_FONT_BANK, OSD_testSimple1Attr, sizeOfAtt );
            OSD_ENSURE( *(volatile uint8_t TTX_MEM*)(addrOfAtt+sizeOfAtt-1) == OSD_testSimple1Attr[sizeOfAtt-1] ); // ERROR: SYSHW_SDMA_Copy() failed!
            for( i=0; i<sizeOfAtt; i++ )
            {
                if( *(volatile uint8_t TTX_MEM*)(addrOfAtt+i) != OSD_testSimple1Attr[i] )
                {
                    OSD_Log( LOGGER_ERROR, "[OSD]""[ERR] SYSHW_SDMA_Copy() @ 0x%X\n", (uint32_t)(addrOfAtt+i) );
                    break;
                }
            }
        }
    */
    {
        uint16_t i;

        OSD_REQUIRE( OSD_testSimple1Attr[sizeOfAtt-2] == 0x70 );
        // Warning! To let folliowing code work, Osd_testSimple1Attr[] must be in the
        //   same code bank with the following code.
        memcpy( (void TTX_MEM*)(addrOfAtt), OSD_testSimple1Attr, sizeOfAtt );
        OSD_ENSURE( *(volatile uint8_t TTX_MEM*)(addrOfAtt+sizeOfAtt-1) == OSD_testSimple1Attr[sizeOfAtt-1] ); // ERROR: SYSHW_SDMA_Copy() failed!

        for ( i=0; i<sizeOfAtt; i++ )
        {
            if ( *(volatile uint8_t TTX_MEM*)(addrOfAtt+i) != OSD_testSimple1Attr[i] )
            {
                OSD_Log( LOGGER_ERROR, "[OSD]""[ERR] SYSHW_SDMA_Copy() @ 0x%X\n", (uint32_t)(addrOfAtt+i) );
                break;
            }
        }
    }

    /*  // Output font to hex format for designer
    {
        FILE* fp = fopen("font.txt", "wt");
        for( i=0; i<26*96*16; i+=16, j++ )
        {
            fprintf(fp, "@%4X      %2X %2X %2X %2X %2X %2X %2X %2X\n", 0x1000+i, OSD_ttFont[i+0], OSD_ttFont[i+1], OSD_ttFont[i+2], OSD_ttFont[i+3], OSD_ttFont[i+4], OSD_ttFont[i+5], OSD_ttFont[i+6], OSD_ttFont[i+7]);
        }
        fclose( fp );
    }*/

    /*// Output font to my format
    {
        uint8_t m,n;
        FILE* fp = fopen("font.txt", "wt");
        for( m=0; m<26; m++ )
        {
            fprintf(fp, "    {\n");
            for( n=0; n<96; n++ )
            {
                fprintf(fp, "        {\n");
                fprintf(fp, "            0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X,\n", OSD_ttFont[m*96*16+n*16+0], OSD_ttFont[m*96*16+n*16+1], OSD_ttFont[m*96*16+n*16+2], OSD_ttFont[m*96*16+n*16+3], OSD_ttFont[m*96*16+n*16+4], OSD_ttFont[m*96*16+n*16+5], OSD_ttFont[m*96*16+n*16+6], OSD_ttFont[m*96*16+n*16+7]);
                fprintf(fp, "            0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X\n" , OSD_ttFont[m*96*16+n*16+8], OSD_ttFont[m*96*16+n*16+9], OSD_ttFont[m*96*16+n*16+10], OSD_ttFont[m*96*16+n*16+11], OSD_ttFont[m*96*16+n*16+12], OSD_ttFont[m*96*16+n*16+13], OSD_ttFont[m*96*16+n*16+14], OSD_ttFont[m*96*16+n*16+15]);
                if( n == 95 )
                {
                    fprintf(fp, "        }\n");
                }
                else
                {
                    fprintf(fp, "        },\n");
                }
            }

            if( m==25)
            {
                fprintf(fp, "    }\n");
            }
            else
            {
                fprintf(fp, "    },\n");
            }
        }
        fclose( fp );
    }*/

    //----------------------------
    // Color Table Setting
    //----------------------------
    // CLUT 0
    _OSD_REG_WRITE( 0x4900, 0x00000000 );
    _OSD_REG_WRITE( 0x4904, 0x00F00000 );
    _OSD_REG_WRITE( 0x4908, 0x0000F000 );
    _OSD_REG_WRITE( 0x490C, 0x00F0F000 );
    _OSD_REG_WRITE( 0x4910, 0x000000F0 );
    _OSD_REG_WRITE( 0x4914, 0x00F000F0 );
    _OSD_REG_WRITE( 0x4918, 0x0000F0F0 );
    _OSD_REG_WRITE( 0x491C, 0x00F0F0F0 );

    // CLUT 1
    _OSD_REG_WRITE( 0x4920, 0x00000000 );
    _OSD_REG_WRITE( 0x4924, 0x00700000 );
    _OSD_REG_WRITE( 0x4928, 0x00007000 );
    _OSD_REG_WRITE( 0x492C, 0x00707000 );
    _OSD_REG_WRITE( 0x4930, 0x00000070 );
    _OSD_REG_WRITE( 0x4934, 0x00700070 );
    _OSD_REG_WRITE( 0x4938, 0x00007070 );
    _OSD_REG_WRITE( 0x493C, 0x00707070 );

    //----------------------------
    // Scaling Coefficient
    //----------------------------
    _OSD_REG_WRITE( 0x49C0, 0x07D307C3 );
    _OSD_REG_WRITE( 0x49C4, 0x07A6078F );
    _OSD_REG_WRITE( 0x49C8, 0x078A07A4 );
    _OSD_REG_WRITE( 0x49CC, 0x07E8005B );
    _OSD_REG_WRITE( 0x49D0, 0x00F701B1 );
    _OSD_REG_WRITE( 0x49D4, 0x0271031F );
    _OSD_REG_WRITE( 0x49D8, 0x03A203EA );

    //----------------------------
    // Diacritical Marks SettiNG
    //----------------------------
    _OSD_REG_WRITE( 0x49E0, 0x00000000 );
    _OSD_REG_WRITE( 0x49E4, 0x06003000 );
    _OSD_REG_WRITE( 0x49E8, 0x03006000 );
    _OSD_REG_WRITE( 0x49EC, 0x0700D800 );
    _OSD_REG_WRITE( 0x49F0, 0x1C423800 );
    _OSD_REG_WRITE( 0x49F4, 0x3FC00000 );
    _OSD_REG_WRITE( 0x49F8, 0x19806000 );
    _OSD_REG_WRITE( 0x49FC, 0x06000000 );
    _OSD_REG_WRITE( 0x4A00, 0x19800000 );
    _OSD_REG_WRITE( 0x4A04, 0x00000006 );
    _OSD_REG_WRITE( 0x4A08, 0x0F01980F );
    _OSD_REG_WRITE( 0x4A0C, 0x00002006 );
    _OSD_REG_WRITE( 0x4A10, 0x0000007F );
    _OSD_REG_WRITE( 0x4A14, 0x0CC19800 );
    _OSD_REG_WRITE( 0x4A18, 0x00004006 );
    _OSD_REG_WRITE( 0x4A1C, 0x0D807000 );
    _OSD_REG_WRITE( 0x4A20, 0x00000000 );
    _OSD_REG_WRITE( 0x4A24, 0x0000E000 );

    //----------------------------
    // Page 1 Setting
    //----------------------------
    _OSD_REG_WRITE( 0x4830, 0x00000003 );
    _OSD_REG_WRITE( 0x4834, 0x00000000 );
    _OSD_REG_WRITE( 0x4838, 0x00000000 );
    _OSD_REG_WRITE( 0x483C, 0x00000000 );
    _OSD_REG_WRITE( 0x4840, 0x00000000 );
    _OSD_REG_WRITE( 0x4844, 0x00000000 );
    _OSD_REG_WRITE( 0x4848, 0x00000000 );
    _OSD_REG_WRITE( 0x484C, 0x00000000 );
    _OSD_REG_WRITE( 0x4850, 0x07000001 );
    _OSD_REG_WRITE( 0x4854, 0x20205031 );
    _OSD_REG_WRITE( 0x4858, 0x30302020 );

    //----------------------------
    // Font & Attribute Address
    //----------------------------
//    OSD_SetTtCharsetBitmapAddress( (TTX_Address_t)OSD_ttFont );//_OSD_REG_WRITE( 0x4814, (uint32_t)OSD_ttFont );
    OSD_SetTtCharsetBitmapAddress( (TTX_Address_t)0xC54E );//_OSD_REG_WRITE( 0x4814, (uint32_t)OSD_ttFont );
    TTX_Log(LOGGER_VERBOSE, "[OSD] font addr Reg(0x4814): 0x%X\n", _OSD_REG_READ(0x4814) );

    _OSD_REG_WRITE( 0x4818, (uint32_t)addrOfAtt );
    _OSD_REG_WRITE( 0x481C, 0x00000000          );

    //----------------------------
    // Global Setting
    //----------------------------
    _OSD_REG_WRITE( 0x4800, 0x40000249 );	// normal// _OSD_REG_WRITE( 0x4800, 0x00000049 );	// normal
    _OSD_REG_WRITE( 0x4808, 0x00003BA3 );	// normal
    _OSD_REG_WRITE( 0x480C, 0x01B001F5 );	// normal
    _OSD_REG_WRITE( 0x4820, 0x80000000 );	// normal

#endif  // end of #if !OSD_SOFTWARE_EMULATOR

}

/*---------------------------------------------------------------------------*/
static void
OSD_TestDisplaySimple2  (void)
/*****************************************************************************\
 *  @brief   Display patterns through VBIOSD. Fonts are place in SRAM.
\*****************************************************************************/
{
#if !OSD_SOFTWARE_EMULATOR

    extern uint8_t const TTX_ROM OSD_testSimple1Attr[];

    bool_t bAttrSdmaOk, bFontSdmaOk;

    const uint16_t  addrOfAtt = 0xC000;
    const uint16_t  sizeOfAtt = 4*26*40*sizeof(uint8_t);

//    const uint16_t  addrOfFont = 0xC000 + sizeOfAtt;
    const uint16_t  addrOfFont = 0xd100;
    const uint16_t  sizeOfFont = 1*96*16;   // (bytes), only one set

    OSD_Log( LOGGER_VERBOSE, "[OSD]" "[Trace]" "Entering OSD_TestDisplaySimple2()... ");


    OSD_REQUIRE( addrOfAtt  % 4 == 0 ); // attr address must be four-byte-algigned.
    OSD_REQUIRE( addrOfFont % 4 == 0 ); // font address must be four-byte-algigned.

    // Prepare attributes to SRAM
    {
        bAttrSdmaOk = TRUE;

        // Copy using SPI-DMA
        {
            // Copy Attributes to SRAM@0xC000 through SPI-DMA.
            OSD_REQUIRE( OSD_testSimple1Attr[sizeOfAtt-2] == 0x70 );

            {
                SYS_SDMA_Copy( (void TTX_MEM*)(addrOfAtt), TTX_FONT_BANK, OSD_testSimple1Attr, sizeOfAtt );
            }

            // Verify above SYS_SDMA_Copy()
            if ( memcmp((void TTX_MEM*)(addrOfAtt), OSD_testSimple1Attr, sizeOfAtt) != 0 )
            {
                bAttrSdmaOk = FALSE;
                OSD_Log( LOGGER_ERROR, "[OSD]""[ERR] SYSHW_SDMA_Copy() @ %x\n", (uint32_t)(addrOfAtt) );
            }
        }

#ifndef WIN32
        // Copy attributes to addrOfAtt from OSD_testSimple1Attr[]
        if ( bAttrSdmaOk != TRUE )
        {
            OSD_Log( LOGGER_WARN, "[OSD] Copied attr by SDMA fail. Using memcpy() instead...\n");

            OSD_REQUIRE( OSD_testSimple1Attr[sizeOfAtt-2] == 0x70 );

            // Warning! To let folliowing code work, Osd_testSimple1Attr[] must be in the
            //   same code bank with the following code.
            memcpy( (void TTX_MEM*)(addrOfAtt), OSD_testSimple1Attr, sizeOfAtt );
            OSD_ENSURE( *(volatile uint8_t TTX_MEM*)(addrOfAtt+sizeOfAtt-1) == OSD_testSimple1Attr[sizeOfAtt-1] ); // ERROR: SYSHW_SDMA_Copy() failed!

            // Verify above SYS_SDMA_Copy()
            if ( memcmp((void TTX_MEM*)(addrOfAtt), OSD_testSimple1Attr, sizeOfAtt) != 0 )
            {
                OSD_Log( LOGGER_ERROR, "[OSD]""[ERR] memcpy() @ %x\n", (uint32_t)(addrOfAtt) );
            }
        }
#endif
    }   // end of prapare attributes

    // Prepare fonts to SRAM
    {
        const uint8_t TTX_ROM* pTtFont = &(OSD_ttFont[0][0][0]);

        bFontSdmaOk = TRUE;

#ifndef WIN32
        // Copy using SPI-DMA
        {
            // Copy Fonts to SRAM@0xE000 through SPI-DMA.
            OSD_REQUIRE( OSD_ttFont[0][95][3] == 0x77 );

            SYS_SDMA_Copy( (void TTX_MEM*)(addrOfFont), TTX_FONT_BANK, OSD_ttFont, sizeOfFont );

            // Verify above SYS_SDMA_Copy()
            OSD_ENSURE( *(volatile uint8_t TTX_MEM*)(addrOfFont+sizeOfFont-1) == pTtFont[sizeOfFont-1] ); // ERROR: SYSHW_SDMA_Copy() failed!
            if ( memcmp((void TTX_MEM*)(addrOfFont), pTtFont, sizeOfFont) != 0 )
            {
                bFontSdmaOk = FALSE;
                OSD_Log( LOGGER_ERROR, "[OSD]""[ERR] SYSHW_SDMA_Copy() @ %x\n", (uint32_t)(addrOfFont) );
            }
        }

        // Copy fonts to addrOfFont from OSD_ttFont[]
        if ( bFontSdmaOk != TRUE )
        {
            // Warning! To let folliowing code work, OSD_ttFont[] must be in the
            //   same code bank with the following code.
            memcpy( (void TTX_MEM*)(addrOfFont), OSD_ttFont, sizeOfFont );

            OSD_ENSURE( *(volatile uint8_t TTX_MEM*)(addrOfFont+sizeOfFont-1) == pTtFont[sizeOfFont-1] ); // ERROR: SYSHW_SDMA_Copy() failed!

            if ( memcmp((void TTX_MEM*)(addrOfFont), pTtFont, sizeOfFont) != 0 )
            {
                OSD_Log( LOGGER_ERROR, "[OSD]""[ERR] memcpy() @ %x\n", (uint32_t)(addrOfFont) );
            }
        }
#endif
    }

    /* Output to designer
    {
        uint16_t i;
        // Output font to hex format
        {
            FILE* fp = fopen("font.txt", "wt");
            for( i=0; i<sizeOfFont; i+=8)
            {
                fprintf(fp, "@3%04X  %02X %02X %02X %02X %02X %02X %02X %02X\n", 0x1200+i, OSD_ttFont[i+0], OSD_ttFont[i+1], OSD_ttFont[i+2], OSD_ttFont[i+3], OSD_ttFont[i+4], OSD_ttFont[i+5], OSD_ttFont[i+6], OSD_ttFont[i+7]);
            }
            fclose( fp );
        }

        // Output attr to hex format
        {
            FILE* fp = fopen("attr.txt", "wt");
            for( i=0; i<sizeOfAtt; i+=8)
            {
                fprintf(fp, "@3%04X  %02X %02X %02X %02X %02X %02X %02X %02X\n", 0x3C00+i, OSD_testSimple1Attr[i+0], OSD_testSimple1Attr[i+1], OSD_testSimple1Attr[i+2], OSD_testSimple1Attr[i+3], OSD_testSimple1Attr[i+4], OSD_testSimple1Attr[i+5], OSD_testSimple1Attr[i+6], OSD_testSimple1Attr[i+7]);
            }
            fclose( fp );
        }
    }*/



    /* Output font to my format (un-compressed(full 26 char sets))
    {
        uint8_t m,n;
        FILE* fp = fopen("font.txt", "wt");
        char comment [26][500] =
        {
            "Latin G0 Primary Set (TTX_CHARSET_G0_LATIN)",
            "Latin National Option Subset: Czech/Slovak (TTX_CHARSET_G0_LATIN_CZECH_SLOVAK)",
            "Latin National Option Subset: English (TTX_CHARSET_G0_LATIN_ENGLISH)",
            "Latin National Option Subset: Estonian (TTX_CHARSET_G0_LATIN_ESTONIAN)",
            "Latin National Option Subset: French (TTX_CHARSET_G0_LATIN_FRENCH)",
            "Latin National Option Subset: German (TTX_CHARSET_G0_LATIN_GERMAN)",
            "Latin National Option Subset: Italian (TTX_CHARSET_G0_LATIN_ITALIAN)",
            "Latin National Option Subset: Lettish/Lithuanian (TTX_CHARSET_G0_LATIN_LETTISH_LITHUANIAN)",
            "Latin National Option Subset: Polish (TTX_CHARSET_G0_LATIN_POLISH)",
            "Latin National Option Subset: Portuguese/Spanish (TTX_CHARSET_G0_LATIN_PORTUGUESE_SPANISH)",
            "Latin National Option Subset: Rumanian (TTX_CHARSET_G0_LATIN_RUMANIAN)",
            "Latin National Option Subset: Serbian/Croatian/Slovenian (TTX_CHARSET_G0_LATIN_SERBIAN_CROATIAN_SLOVENIAN)",
            "Latin National Option Subset: Swedish/Finnish (TTX_CHARSET_G0_LATIN_SWEDISH_FINNISH)",
            "Latin National Option Subset: Turkish (TTX_CHARSET_G0_LATIN_TURKISH)",
            "Latin G2 Set (TTX_CHARSET_G2_LATIN)",
            "Cyrillic G0 Set - Option 1 -Serbian/Croatian (TTX_CHARSET_G0_CYRILLIC_OP1)",
            "Cyrillic G0 Set - Option 2 -Russian/Bulgarian (TTX_CHARSET_G0_CYRILLIC_OP2)",
            "Cyrillic G0 Set - Option 3 - Ukrainian (TTX_CHARSET_G0_CYRILLIC_OP3)",
            "Cyrillic G2 Set (TTX_CHARSET_G2_CYRILLIC)",
            "Greek G0 Set (TTX_CHARSET_G0_GREEK)",
            "Greek G2 Set (TTX_CHARSET_G2_GREEK)",
            "Arabic G0 Set (TTX_CHARSET_G0_ARABIC)",
            "Arabic G2 Set (TTX_CHARSET_G2_ARABIC)",
            "Hebrew G0 Set (TTX_CHARSET_G0_HEBREW)",
            "G1 Block Mosaics Set (TTX_CHARSET_G1_MOSAICS)",
            "G3 Smooth Mosaics and Line Drawing Set (TTX_CHARSET_G3_MOSAICS)"
        };
        for( m=0; m<26; m++ )
        {
            fprintf(fp, "    // %s\n", comment[m]);
            fprintf(fp, "    {\n");
            for( n=0; n<96; n++ )
            {
                fprintf(fp, "        // %X/%X\n", (n+0x20)>>4, (n+0x20)&0xF );
                fprintf(fp, "        {\n");
                fprintf(fp, "            0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X,\n", OSD_ttFont[m*96*16+n*16+0], OSD_ttFont[m*96*16+n*16+1], OSD_ttFont[m*96*16+n*16+2], OSD_ttFont[m*96*16+n*16+3], OSD_ttFont[m*96*16+n*16+4], OSD_ttFont[m*96*16+n*16+5], OSD_ttFont[m*96*16+n*16+6], OSD_ttFont[m*96*16+n*16+7]);
                fprintf(fp, "            0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X\n" , OSD_ttFont[m*96*16+n*16+8], OSD_ttFont[m*96*16+n*16+9], OSD_ttFont[m*96*16+n*16+10], OSD_ttFont[m*96*16+n*16+11], OSD_ttFont[m*96*16+n*16+12], OSD_ttFont[m*96*16+n*16+13], OSD_ttFont[m*96*16+n*16+14], OSD_ttFont[m*96*16+n*16+15]);
                if( n == 95 )
                {
                    fprintf(fp, "        }\n");
                }
                else
                {
                    fprintf(fp, "        },\n");
                }
            }

            if( m==25)
            {
                fprintf(fp, "    }  // end of %s\n", comment[m]);
            }
            else
            {
                fprintf(fp, "    }, // end of %s\n", comment[m]);
            }
        }
        fclose( fp );
    }*/



    // Set FPGABP=0 and HVSMODE = 0
    _OSD_REG_WRITE(0x48E0, 0x00000000);  // VSync is generated by TG or from outside    // OSD_FSETS( ITG1, FPGABP, FPGA );


    //----------------------------
    // Color Table Setting
    //----------------------------
    // CLUT 0
    _OSD_REG_WRITE( 0x4900, 0x00108080 );   // _OSD_REG_WRITE( 0x4900, 0x00000000 );
    _OSD_REG_WRITE( 0x4904, 0x00808080 );
    _OSD_REG_WRITE( 0x4908, 0x0000F000 );
    _OSD_REG_WRITE( 0x490C, 0x00F0F000 );
    _OSD_REG_WRITE( 0x4910, 0x000000F0 );
    _OSD_REG_WRITE( 0x4914, 0x00F000F0 );
    _OSD_REG_WRITE( 0x4918, 0x0000F0F0 );
    _OSD_REG_WRITE( 0x491C, 0x00E08080 );   // _OSD_REG_WRITE( 0x491C, 0x00F0F0F0 );

    // CLUT 1
    _OSD_REG_WRITE( 0x4920, 0x00000000 );
    _OSD_REG_WRITE( 0x4924, 0x00700000 );
    _OSD_REG_WRITE( 0x4928, 0x00007000 );
    _OSD_REG_WRITE( 0x492C, 0x00707000 );
    _OSD_REG_WRITE( 0x4930, 0x00000070 );
    _OSD_REG_WRITE( 0x4934, 0x00700070 );
    _OSD_REG_WRITE( 0x4938, 0x00007070 );
    _OSD_REG_WRITE( 0x493C, 0x00707070 );

    //----------------------------
    // Scaling Coefficient
    //----------------------------
    _OSD_REG_WRITE( 0x49C0, 0x07D307C3 );
    _OSD_REG_WRITE( 0x49C4, 0x07A6078F );
    _OSD_REG_WRITE( 0x49C8, 0x078A07A4 );
    _OSD_REG_WRITE( 0x49CC, 0x07E8005B );
    _OSD_REG_WRITE( 0x49D0, 0x00F701B1 );
    _OSD_REG_WRITE( 0x49D4, 0x0271031F );
    _OSD_REG_WRITE( 0x49D8, 0x03A203EA );

    //----------------------------
    // Diacritical Marks SettiNG
    //----------------------------
    _OSD_REG_WRITE( 0x49E0, 0x00000000 );
    _OSD_REG_WRITE( 0x49E4, 0x06003000 );
    _OSD_REG_WRITE( 0x49E8, 0x03006000 );
    _OSD_REG_WRITE( 0x49EC, 0x0700D800 );
    _OSD_REG_WRITE( 0x49F0, 0x1C423800 );
    _OSD_REG_WRITE( 0x49F4, 0x3FC00000 );
    _OSD_REG_WRITE( 0x49F8, 0x19806000 );
    _OSD_REG_WRITE( 0x49FC, 0x06000000 );
    _OSD_REG_WRITE( 0x4A00, 0x19800000 );
    _OSD_REG_WRITE( 0x4A04, 0x00000006 );
    _OSD_REG_WRITE( 0x4A08, 0x0F01980F );
    _OSD_REG_WRITE( 0x4A0C, 0x00002006 );
    _OSD_REG_WRITE( 0x4A10, 0x0000007F );
    _OSD_REG_WRITE( 0x4A14, 0x0CC19800 );
    _OSD_REG_WRITE( 0x4A18, 0x00004006 );
    _OSD_REG_WRITE( 0x4A1C, 0x0D807000 );
    _OSD_REG_WRITE( 0x4A20, 0x00000000 );
    _OSD_REG_WRITE( 0x4A24, 0x0000E000 );

    //----------------------------
    // Page 1 Setting
    //----------------------------
    _OSD_REG_WRITE( 0x4830, 0x00000703 );
    _OSD_REG_WRITE( 0x4834, 0x00000000 );
    _OSD_REG_WRITE( 0x4838, 0x00000000 );
    _OSD_REG_WRITE( 0x483C, 0x00000000 );
    _OSD_REG_WRITE( 0x4840, 0x00000000 );
    _OSD_REG_WRITE( 0x4844, 0x00000000 );
    _OSD_REG_WRITE( 0x4848, 0x00000000 );
    _OSD_REG_WRITE( 0x484C, 0x00000000 );
    _OSD_REG_WRITE( 0x4850, 0x07010001 );
    _OSD_REG_WRITE( 0x4854, 0x20205031 );
    _OSD_REG_WRITE( 0x4858, 0x30302020 );

    //----------------------------
    // Font & Attribute Address
    //----------------------------
//    OSD_SetTtCharsetBitmapAddress( (TTX_Address_t)OSD_ttFont );//_OSD_REG_WRITE( 0x4814, (uint32_t)OSD_ttFont );
//OSD_SetTtCharsetBitmapAddress( (TTX_Address_t)0xC54E );//_OSD_REG_WRITE( 0x4814, (uint32_t)OSD_ttFont );

    _OSD_REG_WRITE( 0x4814, (uint32_t)addrOfFont ); // unit: byte (4-byte alignment)
    _OSD_REG_WRITE( 0x4818, (uint32_t)addrOfAtt  ); // unit: byte (4-byte alignment)
    _OSD_REG_WRITE( 0x481C, 0x00000000          );

    //----------------------------
    // Global Setting
    //----------------------------
    _OSD_REG_WRITE( 0x4808, 0x000013F3 );	// normal// _OSD_REG_WRITE( 0x4808, 0x00003BA3 );
    _OSD_REG_WRITE( 0x480C, 0x01B001F5 );	// normal
    _OSD_REG_WRITE( 0x4820, 0x80000000 );	// normal
    _OSD_REG_WRITE( 0x4800, 0x00000248 );	// normal// _OSD_REG_WRITE( 0x4800, 0x00000049 );	// normal
    _OSD_REG_WRITE( 0x4800, _OSD_REG_READ(0x4800) | 0x00000001 );

    /*    // Clean up
        {
            delay1ms( 5000 );
            _OSD_REG_WRITE( 0x4800, 0x00000000 );   // turn OFF VBIOSD
        }
    */

    OSD_Log( LOGGER_VERBOSE, "[done]\n");
//TTX_DumpMem( OSD_ttFont, 96*16 );

#endif  // end of #if !OSD_SOFTWARE_EMULATOR
}


/*---------------------------------------------------------------------------*/
static void
OSD_TestDisplaySimple3  (void)
/*****************************************************************************\
 *  @brief   Display patterns through VBIOSD. Fonts are place in Flash ROM.
\*****************************************************************************/
{
#if !OSD_SOFTWARE_EMULATOR

    extern uint8_t const TTX_ROM OSD_testSimple1Attr[];

    const uint16_t  addrOfAtt = 0xC000;
    const uint16_t  sizeOfAtt = 4*26*40*sizeof(uint8_t);

    const uint16_t  sizeOfFont = 1*96*16;   // (bytes), only one set

    OSD_Log( LOGGER_VERBOSE, "[OSD]" "[Trace]" "Entering OSD_TestDisplaySimple3()... ");

    OSD_REQUIRE( addrOfAtt  % 4 == 0 ); // attr address must be four-byte-algigned.
    OSD_REQUIRE( (uint32_t)OSD_ttFont % 4 == 0 ); // font address must be four-byte-algigned.


#ifndef WIN32
    // Copy attributes to addrOfAtt from OSD_testSimple1Attr[]
    {
        uint16_t i;

        OSD_REQUIRE( OSD_testSimple1Attr[sizeOfAtt-2] == 0x70 );

        // Warning! To let folliowing code work, Osd_testSimple1Attr[] must be in the
        //   same code bank with the following code.
        memcpy( (void TTX_MEM*)(addrOfAtt), OSD_testSimple1Attr, sizeOfAtt );
        OSD_ENSURE( *(volatile uint8_t TTX_MEM*)(addrOfAtt+sizeOfAtt-1) == OSD_testSimple1Attr[sizeOfAtt-1] ); // ERROR: SYSHW_SDMA_Copy() failed!

        for ( i=0; i<sizeOfAtt; i++ )
        {
            if ( *(volatile uint8_t TTX_MEM*)(addrOfAtt+i) != OSD_testSimple1Attr[i] )
            {
                OSD_Log( LOGGER_ERROR, "[OSD]""[ERR] memcpy() @ 0x%X\n", (uint32_t)(addrOfAtt+i) );
                break;
            }
        }
    }

#endif
    /*
        // Set interlace mode
        // Enable pattern generator
        _OSD_REG_WRITE(0x4800, 0x80000200);
    */
    // Set FPGABP=1 and HVSMODE = 0
//    _OSD_REG_WRITE(0x48E0, 0x00000002);  // VSync is generated by TG or from outside


    //----------------------------
    // Color Table Setting
    //----------------------------
    // CLUT 0
    _OSD_REG_WRITE( 0x4900, 0x00108080 );   // _OSD_REG_WRITE( 0x4900, 0x00000000 );
    _OSD_REG_WRITE( 0x4904, 0x00808080 );
    _OSD_REG_WRITE( 0x4908, 0x0000F000 );
    _OSD_REG_WRITE( 0x490C, 0x00F0F000 );
    _OSD_REG_WRITE( 0x4910, 0x000000F0 );
    _OSD_REG_WRITE( 0x4914, 0x00F000F0 );
    _OSD_REG_WRITE( 0x4918, 0x0000F0F0 );
    _OSD_REG_WRITE( 0x491C, 0x00E08080 );   // _OSD_REG_WRITE( 0x491C, 0x00F0F0F0 );

    // CLUT 1
    _OSD_REG_WRITE( 0x4920, 0x00000000 );
    _OSD_REG_WRITE( 0x4924, 0x00700000 );
    _OSD_REG_WRITE( 0x4928, 0x00007000 );
    _OSD_REG_WRITE( 0x492C, 0x00707000 );
    _OSD_REG_WRITE( 0x4930, 0x00000070 );
    _OSD_REG_WRITE( 0x4934, 0x00700070 );
    _OSD_REG_WRITE( 0x4938, 0x00007070 );
    _OSD_REG_WRITE( 0x493C, 0x00707070 );

    //----------------------------
    // Scaling Coefficient
    //----------------------------
    _OSD_REG_WRITE( 0x49C0, 0x07D307C3 );
    _OSD_REG_WRITE( 0x49C4, 0x07A6078F );
    _OSD_REG_WRITE( 0x49C8, 0x078A07A4 );
    _OSD_REG_WRITE( 0x49CC, 0x07E8005B );
    _OSD_REG_WRITE( 0x49D0, 0x00F701B1 );
    _OSD_REG_WRITE( 0x49D4, 0x0271031F );
    _OSD_REG_WRITE( 0x49D8, 0x03A203EA );

    //----------------------------
    // Diacritical Marks SettiNG
    //----------------------------
    _OSD_REG_WRITE( 0x49E0, 0x00000000 );
    _OSD_REG_WRITE( 0x49E4, 0x06003000 );
    _OSD_REG_WRITE( 0x49E8, 0x03006000 );
    _OSD_REG_WRITE( 0x49EC, 0x0700D800 );
    _OSD_REG_WRITE( 0x49F0, 0x1C423800 );
    _OSD_REG_WRITE( 0x49F4, 0x3FC00000 );
    _OSD_REG_WRITE( 0x49F8, 0x19806000 );
    _OSD_REG_WRITE( 0x49FC, 0x06000000 );
    _OSD_REG_WRITE( 0x4A00, 0x19800000 );
    _OSD_REG_WRITE( 0x4A04, 0x00000006 );
    _OSD_REG_WRITE( 0x4A08, 0x0F01980F );
    _OSD_REG_WRITE( 0x4A0C, 0x00002006 );
    _OSD_REG_WRITE( 0x4A10, 0x0000007F );
    _OSD_REG_WRITE( 0x4A14, 0x0CC19800 );
    _OSD_REG_WRITE( 0x4A18, 0x00004006 );
    _OSD_REG_WRITE( 0x4A1C, 0x0D807000 );
    _OSD_REG_WRITE( 0x4A20, 0x00000000 );
    _OSD_REG_WRITE( 0x4A24, 0x0000E000 );

    //----------------------------
    // Page 1 Setting
    //----------------------------
    _OSD_REG_WRITE( 0x4830, 0x00000703 );
    _OSD_REG_WRITE( 0x4834, 0x00000000 );
    _OSD_REG_WRITE( 0x4838, 0x00000000 );
    _OSD_REG_WRITE( 0x483C, 0x00000000 );
    _OSD_REG_WRITE( 0x4840, 0x00000000 );
    _OSD_REG_WRITE( 0x4844, 0x00000000 );
    _OSD_REG_WRITE( 0x4848, 0x00000000 );
    _OSD_REG_WRITE( 0x484C, 0x00000000 );
    _OSD_REG_WRITE( 0x4850, 0x07010001 );
    _OSD_REG_WRITE( 0x4854, 0x20205031 );
    _OSD_REG_WRITE( 0x4858, 0x30302020 );

    //----------------------------
    // Font & Attribute Address
    //----------------------------
    OSD_SetTtCharsetBitmapAddress( (TTX_Address_t)OSD_ttFont ); // unit: byte (4-byte alignment)
    _OSD_REG_WRITE( 0x4818, (uint32_t)addrOfAtt  ); // unit: byte (4-byte alignment)
    _OSD_REG_WRITE( 0x481C, 0x00000000          );

    //----------------------------
    // Global Setting
    //----------------------------
    _OSD_REG_WRITE( 0x4808, 0x000013F3 );	// normal// _OSD_REG_WRITE( 0x4808, 0x00003BA3 );
    _OSD_REG_WRITE( 0x480C, 0x01B001F5 );	// normal
    _OSD_REG_WRITE( 0x4820, 0x80000000 );	// normal
    _OSD_REG_WRITE( 0x4800, 0x00000248 );	// normal// _OSD_REG_WRITE( 0x4800, 0x00000049 );	// normal
    _OSD_REG_WRITE( 0x4800, _OSD_REG_READ(0x4800) | 0x00000001 );

    /*
        // Clean up
        {
            delay1ms( 5000 );
            _OSD_REG_WRITE( 0x4800, 0x00000000 );   // turn OFF VBIOSD
        }
    */

//TTX_DumpMem( OSD_ttFont, 96*16 );

    OSD_Log( LOGGER_VERBOSE, "[done]\n");

#endif  // end of #if !OSD_SOFTWARE_EMULATOR
}

/*---------------------------------------------------------------------------*/
static void
OSD_TestLatinNatlOpt0   (void)
/*****************************************************************************\
 *  @brief   Display Latin National Option Subsets
 *  @post    attribute memory 1 will be modified.
\*****************************************************************************/
{
#define CHAR_IDX_NUM (13)


    TTX_STD_CharSet_t charset;
    uint8_t           i;

    void TTX_MEM* pAttrMem;

    // Caluse 15.6.2 (p.107)
    const uint8_t charIdx[CHAR_IDX_NUM] = { 0x23, 0x24, 0x40, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x7B, 0x7C, 0x7D, 0x7E };

    TTX_DISP_Attr_t attr;

    OSD_Log( LOGGER_VERBOSE, "[OSD]" "[Trace]" "Entering OSD_TestLatinNatlOpt0()... ");


    TTX_DISP_Open( OSD_OPEN_RESET );

    // Note: Calling OSD_GetTtAttr1Address() must be after OSD_Open().
    //         Because attributes address is updated in OSD_Open().
    pAttrMem = (void TTX_MEM*)OSD_GetTtAttr1Address();

    OSD_REQUIRE( pAttrMem != NULL );

    // Reset background attributes in share mem
    TTX_DISP_ZeroAttrMem();


    attr.fgColorIdx = TTX_COLOR_YELLOW;
    attr.bgColorIdx = TTX_COLOR_BLUE;
    attr.box        = FALSE;
    attr.graphic    = FALSE;
    attr.conceal    = FALSE;
    attr.flashMode  = FALSE;
    attr.size       = TTX_STD_SIZE_CFG_NORMAL;
    attr.bUnderline = FALSE;
    attr.dmIdx      = 0;
    attr.mosaics    = FALSE;

    // Iterate through Lation National Option Sub-Sets
    for ( charset = TTX_CHARSET_G0_LATIN_CZECH_SLOVAK; charset <= TTX_CHARSET_G0_LATIN_TURKISH ; charset++ )
    {
        attr.charset = charset;

        for ( i = 0; i < CHAR_IDX_NUM ; i++ )
        {
            attr.charIdx  = charIdx[i];
            TTX_DISP_SetAttr( ( 3 + charset - TTX_CHARSET_G0_LATIN_CZECH_SLOVAK ), ( 2 + i ), & attr );
        }
    }

    // enable output
    OSD_SetTtDisplayEnable( TRUE );
    OSD_SetVbiEnable( TRUE );

    // Refresh screen
    TTX_DISP_DoRefresh();


    TTX_DISP_Close();

    OSD_Log( LOGGER_VERBOSE, "[done]\n");
}

/*---------------------------------------------------------------------------*/
static bool_t
TtAttrEqual     (const TTX_DISP_Attr_t* pAttr1, const TTX_DISP_Attr_t* pAttr2)
/*****************************************************************************\
 *  @brief   Returns TRUE if *pAttr1 == *pAttr2.
 *  @note    Must be called in advance of any other functions.
 *
 *  @return  Equality of *pAttr1 and *pAttr2.
 *  @retval  TRUE  *pAttr1 == *pAttr2.
 *  @retval  FALSE *pAttr1 != *pAttr2.
 *
 *  @ingroup ttx
 *  @see     TTX_Init()
\*****************************************************************************/
{
    if ( pAttr1 == pAttr2 )
        return( TRUE );

    /* Because OSD_TtAttr_t contains bit-fields, we cannot use the following:
     *   return( memcmp( pAttr1, pAttr2, sizeof( OSD_TtAttr_t ) ) == 0 );
     */
    return( ( pAttr1->conceal    == pAttr2->conceal    )   \
            &&( pAttr1->box        == pAttr2->box        )   \
            &&( pAttr1->graphic    == pAttr2->graphic    )   \
            &&( pAttr1->size       == pAttr2->size       )   \
            &&( pAttr1->bUnderline == pAttr2->bUnderline )   \
            &&( pAttr1->mosaics    == pAttr2->mosaics    )   \
            &&( pAttr1->dmIdx      == pAttr2->dmIdx      )   \
            &&( pAttr1->flashMode  == pAttr2->flashMode  )   \
            &&( pAttr1->fgColorIdx == pAttr2->fgColorIdx )   \
            &&( pAttr1->bgColorIdx == pAttr2->bgColorIdx )   \
            &&( pAttr1->charset    == pAttr2->charset    )   \
            &&( pAttr1->charIdx    == pAttr2->charIdx    )   );

}

/*---------------------------------------------------------------------------*/
static bool_t
TtAttrCopy      (TTX_DISP_Attr_t* pTo, const TTX_DISP_Attr_t* pFrom)
/*****************************************************************************\
 *  @brief   Copy contents of *pFrom to *pTo.
 *  @note    Must be called in advance of any other functions.
 *
 *  @return  Equality of *pAttr1 and *pAttr2.
 *  @retval  TRUE  *pAttr1 == *pAttr2.
 *  @retval  FALSE *pAttr1 != *pAttr2.
 *
 *  @ingroup ttx
 *  @see     TTX_Init()
\*****************************************************************************/
{
    if ( pTo != pFrom )
    {
        if ( (pTo != NULL) && (pFrom != NULL) )
        {
            memcpy( (void*)pTo, (void*)pFrom, sizeof(TTX_DISP_Attr_t) );
        }

        return( TRUE );
    }

    return( FALSE );
}

#endif  /* end of #if OSD_VERIFY */

/*---------------------------------------------------------------------------*\
 * end of osd_test.c                                                         *
\*---------------------------------------------------------------------------*/

