/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_source_test.c
 * @brief    Teletext input data processor.
 * @author   Janus Cheng
 * @date     Nov. 11, 2008
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/
#ifdef CONFIG_TTX_SUPPORT

#include <ttx_source.h> /* Provides: functions to be implemented */

#include <ttx_obj.h>    /* Provides: TTX_Handle */
#include <ttx_assert.h> /* Provides: TTX_REQUIRE() and TTX_ENSURE() */

#include <ppr.h>        /* Provides: PPR module */

#include <string.h>     /* Provides: memcpy() */
#include "slr.h"            /* Provides: functions to be implemented */


TTX_DEFINE_THIS_MODULE( ttx_source_test.c )


/* -- Prototypes of Private Functions ---------------------------------------*/


//static bool_t   TTX_SRC_Fetch_FromPg0                   (TTX_Handle hTtx);
#if 0  /* Un-used functions */
static bool_t   TTX_SRC_Fetch_FromPg1                   (TTX_Handle hTtx);
#endif  /* end of #if 0 */


/* -- Implementations of Public Functions -----------------------------------*/

#if TTX_VERIFY  /* if enabled verficiation of TTX related IPs */

/*---------------------------------------------------------------------------*/
bool_t
TTX_SRC_Test    (void)
/*****************************************************************************\
 *  @brief   Fetch data from data sources.
\*****************************************************************************/
{
    return( PPR_Test() );
}

#endif  /* end of #if TTX_VERIFY */



/* -- Implementations of Private Functions ----------------------------------*/



#if 0   // Un-used
/*---------------------------------------------------------------------------*/
static bool_t
TTX_SRC_Fetch_FromPg0    (TTX_Handle hTtx)
/*****************************************************************************\
 *  @brief   Simulates slicer by PPR pattern generator to fetch PPR_FIELD_BUFF_PKT_NUM
 *             packets from prepared patterns stored in flash ROM.
 *  @pre     TTX_SRC_PrepareToAcceptNextField() has already been run.
\*****************************************************************************/
{
    extern uint8_t const TTX_ROM TTX_test_DEN_Sonderbourg_474000_ZDFmobil_1128_233_pid0x237_P100_0[130][47];
    const size_t numOfPkts = 130;

    /*
        extern uint8_t const TTX_ROM TTX_test_7_Melb_151106_P100_0[24][47];
        const size_t numOfPkts = 24;
    */
    /*
        extern uint8_t const TTX_ROM TTX_test_GER_Augsburg_586000_ZDFmobil_1101_0432_P798_1[23][47];
        const size_t numOfPkts = 23;
    */
    static size_t remainNumOfPkts = 0;

    const uint8_t (*ptr)[][PPR_PTNGEN_PKT_LEN_BYTES];

    static size_t i=0;

    // Enable pattern gen
    PPR_PgEnable();

    // Simulates arrival of v-sync
    TTX_SRC_onVSync();

    // Fill field buffer
    {
        // Generates TTX_testDEN_Sonderbourg_474000_ZDFmobil_1128_233_pid0x237_P100_0[] to field buffer
        TTX_REQUIRE( TTX_test_DEN_Sonderbourg_474000_ZDFmobil_1128_233_pid0x237_P100_0[2][9] == 0x73 ); // ERROR: Cannot read TTX_test_DEN_Sonderbourg_474000_ZDFmobil_1128_233_pid0x237_P100_0[]. This code should be placed with the same bank as TTX_test_DEN_Sonderbourg_474000_ZDFmobil_1128_233_pid0x237_P100_0[][].
        ptr = (const uint8_t (*)[][PPR_PTNGEN_PKT_LEN_BYTES])(TTX_test_DEN_Sonderbourg_474000_ZDFmobil_1128_233_pid0x237_P100_0[i]);

//        ptr = (const uint8_t (*)[][PPR_PTNGEN_PKT_LEN_BYTES])(TTX_test_7_Melb_151106_P100_0[i]);

//        ptr = (const uint8_t (*)[][PPR_PTNGEN_PKT_LEN_BYTES])(TTX_test_GER_Augsburg_586000_ZDFmobil_1101_0432_P798_1[i]);


        if ( remainNumOfPkts > 0 )  // this is the last iteration
        {
            // Generates (*ptr)[0][] to (*ptr)[remainNumOfPkts-1][] to field buffer
            PPR_PgGenerateOneField( *ptr, remainNumOfPkts );

            // Next iteration will be the first one
            i = 0;
            remainNumOfPkts = 0;
        }
        else
        {
            // Generates (*ptr)[0][] to (*ptr)[PPR_FIELD_BUFF_PKT_NUM-1][] to field buffer
            PPR_PgGenerateOneField( *ptr, PPR_FIELD_BUFF_PKT_NUM );

            i += PPR_FIELD_BUFF_PKT_NUM;
        }


        // Next iteration is the last one
        if ( ( i + PPR_FIELD_BUFF_PKT_NUM ) >= numOfPkts )
        {
            remainNumOfPkts = numOfPkts - i;
        }

    }

    // Wait for field buffer valid
    while ( ! PPR_FieldBuffIsValid() );

    // Simulates arrival of field buffer valid
    TTX_SRC_onFieldBuffValid( hTtx );

    return( TRUE );
}
#endif


/*---------------------------------------------------------------------------*/
void
TTX_SRC_PrepareToAcceptNextField  (void)
/*****************************************************************************\
 *  @brief   Initialize PPR to accept next coming of V-Sync.
 *  @post    TTX_DEFAULT_CD_PAGE_ADDRESS will be the parsed data.
 *  @post    Parsed data size is TTX_DEFAULT_CD_PAGE_TOTAL_SIZE.
\*****************************************************************************/
{

    // X/0 hamming error filter
//        PPR_FSETS( BFILTER_CTRL, MAGPKTDC, DISCARD );   // discard magazine packet when hamming error

    // ERROR: Our software cannot process field buffer before this coming of field buffer.
    //        In other words, our software didn't meet real-time requirements.
    //        This will cause frame loss.
    TTX_Log( LOGGER_VERBOSE, "TTX_SRC_PrepareToAcceptNextField\n");

    TTX_ENSURE( PPR_FGET( DATA_ACQU_ST, FB_FULL ) != PPR_DATA_ACQU_ST_FB_FULL_TRUE );

    // Clear Data Valid
    TTX_Reg32Write( VBIPPR_DATA_ACQU_ST_REG_0x4014, 0x3 );

    // Clear PPR INT status
    TTX_Reg32Write( VBIPPR_INT_ST_REG_0x4008, 0xffffffff );

    // Reset prefetch start address of field buffer
    PPR_FSETS( PFE_NPROC_ADDR, ADDR, PKT(0) );

    // Reset field-buffer indicators
    TTX_Reg32Write( VBIPPR_FB_IND_REG_0x40b0, 0);

    // Accept data from Slicer
    PPR_FSETS( CTRL, DATA_ACQU, ENABLE );
    TTX_ENSURE( PPR_FGET( CTRL, DATA_ACQU ) == PPR_CTRL_DATA_ACQU_ENABLE );


}

/*---------------------------------------------------------------------------*/
bool_t
TTX_SRC_Open    (const TTX_SRC_OpenFlag_t flags)
/*****************************************************************************\
 *  @brief   Initialize Teletext data source.
\*****************************************************************************/
{
    bool_t bPprOk, bSlicerOk;

    if ( flags & TTX_SRC_OPEN_RESET )
    {
        bSlicerOk = SLR_Open( SLR_OPEN_RESET );
        bPprOk    = PPR_Open( PPR_OPEN_RESET );
    }
    else
    {
        bSlicerOk = SLR_Open( SLR_OPEN_NOP );
        bPprOk    = PPR_Open( PPR_OPEN_NOP );
    }

    TTX_ENSURE( bPprOk    == TRUE );
    TTX_ENSURE( bSlicerOk == TRUE );

    /* Initialize PPR here may cause PPR tests failure.
        // Initialize PPR to accept data
        TTX_SRC_PrepareToAcceptNextField();
    */

    return( bPprOk );
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_SRC_Close    (void)
/*****************************************************************************\
 *  @brief   Close Teletext data source.
\*****************************************************************************/
{

    bool_t bPprOk, bSlrOk;

    bPprOk = PPR_Close();
    TTX_ENSURE( bPprOk == TRUE );

    bSlrOk = SLR_Close();
    TTX_ENSURE( bSlrOk == TRUE );


    return( bPprOk );
}

/*---------------------------------------------------------------------------*/
void
TTX_SRC_Reset    (void)
/*****************************************************************************\
 *  @brief   Close Teletext data source.
\*****************************************************************************/
{
    TTX_SRC_Close();
    TTX_SRC_Open( TTX_SRC_OPEN_RESET );
}


/*
void get_time() {
	rtd_isr_outl(0x2860, 0);

	rtd_isr_outl(0x2860, _BIT31);

	myTimer1 = rtd_isr_inl( 0x2854 );
}
*/

UINT32 TTX_SRC_Parse_Read(UINT16 addr)
{
    UINT8 getVal[4];
    getVal[3] = *((UINT8 xdata*) addr);
    addr++;
    getVal[2] = *((UINT8 xdata*) addr);
    addr++;
    getVal[1] = *((UINT8 xdata*) addr);
    addr++;
    getVal[0] = *((UINT8 xdata*) addr);

    return *((UINT32*)getVal);
}


void TTX_SRC_Parse_Write(UINT16 addr, UINT32 val)
{
    *((UINT8 xdata*) addr) = *((UINT8 xdata*)&val + 3);
    *((UINT8 xdata*) addr + 1) = *((UINT8 xdata*)&val + 2);
    addr +=2;
    *((UINT8 xdata*) addr) = *((UINT8 xdata*)&val + 1);
    *((UINT8 xdata*) addr + 1) = *((UINT8 xdata*)&val);
}


#if 0  /* Un-used functions */
/*---------------------------------------------------------------------------*/
static bool_t
TTX_SRC_Fetch_FromPg1    (TTX_Handle hTtx)
/*****************************************************************************\
 *  @brief   Simulates slicer by PPR pattern generator to fetch PPR_FIELD_BUFF_PKT_NUM
 *             packets from prepared two test patterns stored in flash ROM.
 *  @pre     TTX_SRC_PrepareToAcceptNextField() has already been run.
 \*****************************************************************************/
{
    static uint8_t ptn = 1;

    extern uint8_t const TTX_ROM TTX_test_7_Melb_151106_P100_0[24][47];
    const size_t numOfPkts1 = 24;

    extern uint8_t const TTX_ROM TTX_test_GER_Augsburg_586000_ZDFmobil_1101_0432_P798_1[23][47];
    const size_t numOfPkts2 = 23;

    static size_t remainNumOfPkts = 0;
    size_t numOfPkts;

    const uint8_t (*ptr)[][PPR_PTNGEN_PKT_LEN_BYTES];

    static size_t i=0;

    // Enable pattern gen
    PPR_PgEnable();

    // Simulates arrival of v-sync
    TTX_SRC_onVSync();

    // Fill field buffer
    {
        // Generates TTX_testDEN_Sonderbourg_474000_ZDFmobil_1128_233_pid0x237_P100_0[] to field buffer
//        TTX_REQUIRE( TTX_test_DEN_Sonderbourg_474000_ZDFmobil_1128_233_pid0x237_P100_0[2][9] == 0x73 ); // ERROR: Cannot read TTX_test_DEN_Sonderbourg_474000_ZDFmobil_1128_233_pid0x237_P100_0[]. This code should be placed with the same bank as TTX_test_DEN_Sonderbourg_474000_ZDFmobil_1128_233_pid0x237_P100_0[][].
//        ptr = (const uint8_t (*)[][PPR_PTNGEN_PKT_LEN_BYTES])(TTX_test_DEN_Sonderbourg_474000_ZDFmobil_1128_233_pid0x237_P100_0[i]);

        if ( ptn == 1 )
        {
            ptr = (const uint8_t (*)[][PPR_PTNGEN_PKT_LEN_BYTES])(TTX_test_7_Melb_151106_P100_0[i]);
        }
        else
        {
            ptr = (const uint8_t (*)[][PPR_PTNGEN_PKT_LEN_BYTES])(TTX_test_GER_Augsburg_586000_ZDFmobil_1101_0432_P798_1[i]);
        }

        if ( remainNumOfPkts > 0 )  // this is the last iteration
        {
            // Generates (*ptr)[0][] to (*ptr)[remainNumOfPkts-1][] to field buffer
            PPR_PgGenerateOneField( *ptr, remainNumOfPkts );

            // Next iteration will be the first one
            i = 0;
            remainNumOfPkts = 0;

            // Toggle ptn to switch to next pattern in the next iteration
            if ( ptn == 1 )
            {
                ptn = 2;
            }
            else
            {
                ptn = 1;
            }
        }
        else
        {
            // Generates (*ptr)[0][] to (*ptr)[PPR_FIELD_BUFF_PKT_NUM-1][] to field buffer
            PPR_PgGenerateOneField( *ptr, PPR_FIELD_BUFF_PKT_NUM );

            i += PPR_FIELD_BUFF_PKT_NUM;
        }


        // Next iteration is the last one
        if ( ptn == 1 )
        {
            numOfPkts = numOfPkts1;
        }
        else
        {
            numOfPkts = numOfPkts2;
        }

        if ( ( i + PPR_FIELD_BUFF_PKT_NUM ) >= numOfPkts )
        {
            remainNumOfPkts = numOfPkts - i;
        }

    }

    // Wait for field buffer valid
    while ( ! PPR_FieldBuffIsValid() ); // Wait by polling
    /* Wait by interrupt
    {
        while( ! TTX_EVT_isFired( hTtx, PPR, FB_VALID ) );  // wait for FB_VALID interrupt occurs.
        TTX_EVT_Clear( hTtx, PPR, FB_VALID );   // clear flag
    }*/


    // Simulates arrival of field buffer valid
    TTX_SRC_onFieldBuffValid( hTtx );

    return( TRUE );
}

#endif  /* end of #if 0 */

#if 0  /* Un-used functions */

/*---------------------------------------------------------------------------*/
static bool_t
TTX_SRC_Fetch_FromPg1ToLocalVariable    (TTX_Handle hTtx)
/*****************************************************************************\
 *  @brief   Load PG1 pattern to local variable and fetch it to TTX_SRC. This
 *             avoid re-read PG1 from FlashROM again and again.
 \*****************************************************************************/
{
#ifndef WIN32   /* Visual C++ doesn't support #warning */
#warning "TTX_SRC_Fetch_FromPg1ToLocalVariable() is being compiled. It drains much spaces!"
#endif  /* end of #ifndef WIN32 */

    static uint8_t ptn = 1;

    extern uint8_t const TTX_ROM TTX_test_7_Melb_151106_P100_0[24][47];
    const size_t numOfPkts1 = 24;

    extern uint8_t const TTX_ROM TTX_test_GER_Augsburg_586000_ZDFmobil_1101_0432_P798_1[23][47];
    const size_t numOfPkts2 = 23;

    static size_t remainNumOfPkts = 0;
    size_t numOfPkts;

    const uint8_t (*ptr)[][PPR_PTNGEN_PKT_LEN_BYTES];

    static size_t i=0;


    static bool_t bFirst = TRUE;
    uint8_t localVar1[24][47];
    uint8_t localVar2[23][47];

    if ( bFirst )
    {
        memcpy( localVar1, TTX_test_7_Melb_151106_P100_0                         , numOfPkts1*47 );
        memcpy( localVar2, TTX_test_GER_Augsburg_586000_ZDFmobil_1101_0432_P798_1, numOfPkts2*47 );

        bFirst = FALSE;
    }

    // Enable pattern gen
    PPR_PgEnable();

    // Simulates arrival of v-sync
    TTX_SRC_onVSync();

    // Fill field buffer
    {
        // Generates TTX_testDEN_Sonderbourg_474000_ZDFmobil_1128_233_pid0x237_P100_0[] to field buffer
//        TTX_REQUIRE( TTX_test_DEN_Sonderbourg_474000_ZDFmobil_1128_233_pid0x237_P100_0[2][9] == 0x73 ); // ERROR: Cannot read TTX_test_DEN_Sonderbourg_474000_ZDFmobil_1128_233_pid0x237_P100_0[]. This code should be placed with the same bank as TTX_test_DEN_Sonderbourg_474000_ZDFmobil_1128_233_pid0x237_P100_0[][].
//        ptr = (const uint8_t (*)[][PPR_PTNGEN_PKT_LEN_BYTES])(TTX_test_DEN_Sonderbourg_474000_ZDFmobil_1128_233_pid0x237_P100_0[i]);

        if ( ptn == 1 )
        {
            ptr = (const uint8_t (*)[][PPR_PTNGEN_PKT_LEN_BYTES])(localVar1[i]);
        }
        else
        {
            ptr = (const uint8_t (*)[][PPR_PTNGEN_PKT_LEN_BYTES])(localVar2[i]);
        }

        if ( remainNumOfPkts > 0 )  // this is the last iteration
        {
            // Generates (*ptr)[0][] to (*ptr)[remainNumOfPkts-1][] to field buffer
            PPR_PgGenerateOneField( *ptr, remainNumOfPkts );

            // Next iteration will be the first one
            i = 0;
            remainNumOfPkts = 0;

            // Toggle ptn to switch to next pattern in the next iteration
            if ( ptn == 1 )
            {
                ptn = 2;
            }
            else
            {
                ptn = 1;
            }
        }
        else
        {
            // Generates (*ptr)[0][] to (*ptr)[PPR_FIELD_BUFF_PKT_NUM-1][] to field buffer
            PPR_PgGenerateOneField( *ptr, PPR_FIELD_BUFF_PKT_NUM );

            i += PPR_FIELD_BUFF_PKT_NUM;
        }


        // Next iteration is the last one
        if ( ptn == 1 )
        {
            numOfPkts = numOfPkts1;
        }
        else
        {
            numOfPkts = numOfPkts2;
        }

        if ( ( i + PPR_FIELD_BUFF_PKT_NUM ) >= numOfPkts )
        {
            remainNumOfPkts = numOfPkts - i;
        }

    }

    // Wait for field buffer valid
    while ( ! PPR_FieldBuffIsValid() ); // Wait by polling
    /* Wait by interrupt
    {
        while( ! TTX_EVT_isFired( hTtx, PPR, FB_VALID ) );  // wait for FB_VALID interrupt occurs.
        TTX_EVT_Clear( hTtx, PPR, FB_VALID );   // clear flag
    }*/


    // Simulates arrival of field buffer valid
    TTX_SRC_onFieldBuffValid( hTtx );

    return( TRUE );
}
#endif  /* end of #if 0 */



/*---------------------------------------------------------------------------*\
 * end of ttx_source_test.c                                                  *
\*---------------------------------------------------------------------------*/
#endif
