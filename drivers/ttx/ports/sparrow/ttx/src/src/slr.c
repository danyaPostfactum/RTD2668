/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     slr.c
 * @brief    Prototypes of public application interface.
 * @details  This header declares the public access application interface for this TtLib library.
 * @author   Janus Cheng
 * @date     Dec. 11, 2007
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/

#include "slr.h"            /* Provides: functions to be implemented */

#include <slr_hal_test.h>   /* Provides: Test for <slr_hal.h> */
#include <ttx_source.h> /* Provides: functions to be implemented */



/* Because no one uses TTX_ASSERT functions, to eliminate compiler's warning that
     ?CO?SLR not used, we comment it here.
TTX_DEFINE_THIS_MODULE( slr.c )
*/

/* -- Prototypes of Private Functions ---------------------------------------*/

/* -- Implementations of Public Functions -----------------------------------*/

/*---------------------------------------------------------------------------*/
bool_t
SLR_Open (const SLR_OpenFlag_t flags)
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
    if ( flags & SLR_OPEN_RESET )
    {
        SLR_Reset();
    }

#if 1
    // Ported from Chin-How's mice commands: startice.cmd
    {
        /*
                // TT-1033 line setup start
                {
                    SLR_RegWrite( 0x5A08, 0x66660000 ); // ew bf00aa08 = 66660000   //line 11,12
                    SLR_RegWrite( 0x5A0C, 0x00006666 ); // ew bf00aa0c = 00006666   //line 13,14
                    SLR_RegWrite( 0x5A10, 0x00000000 ); // ew bf00aa10 = 0
                    SLR_RegWrite( 0x5A14, 0x00000000 ); // ew bf00aa14 = 0
                    SLR_RegWrite( 0x5A18, 0x00000000 ); // ew bf00aa18 = 0
                }
                // TT-1033 line setup end
        */
        /*
                // fluke line setup start
                {
                    SLR_RegWrite( 0x5A0C, 0x00006666 ); // ew bf00aa0c = 00006666 // line 13,14
                    SLR_RegWrite( 0x5A10, 0x66000000 ); // ew bf00aa10 = 66000000 // line 20
                    SLR_RegWrite( 0x5A14, 0x00000066 ); // ew bf00aa14 = 00000066 // line 21
                    SLR_RegWrite( 0x5A18, 0x00000000 ); // ew bf00aa18 = 0
                }
                // fluke line setup end
        */

        // field test all line setup start
        {
            SLR_RegWrite( 0x5A04, 0x66660000 ); // ew bf00aa04 = 66660000 // line 7,8
            SLR_RegWrite( 0x5A08, 0x66666666 ); // ew bf00aa08 = 66666666 // line 9,10,11,12
            SLR_RegWrite( 0x5A0C, 0x66666666 ); // ew bf00aa0c = 66666666 // line 13,14,15,16
            SLR_RegWrite( 0x5A10, 0x66666666 ); // ew bf00aa10 = 66666666 // line 17,18,19,20
            SLR_RegWrite( 0x5A14, 0xcc666666 ); // ew bf00aa14 = 66666666 // line 21,22,24      , line 23 for WSS
            SLR_RegWrite( 0x5A18, 0x0000cc00 ); // ew bf00aa18 = 0
        }
        // field test all line setup end

        if ( rtd_inl( 0x515C ) & 0x00000080 )
        {
            SLR_RegWrite( 0x5800, 0x00000060 ); // ew bf00a800 = 01600022   // inverse or not
        }
        else
        {
            SLR_RegWrite( 0x5800, 0x00000040 ); // ew bf00a800 = 01600022   // inverse or not
        }

        SLR_RegWrite( 0x5A00, 0x4b4b2713 ); // ew bf00aa00 = 004b2713  //   slicer level determined by
        SLR_RegWrite( 0x5A30, 0x40004600 ); // ew bf00aa30 = 00004600  // tt_vbi_noise_threshold : the lowest of adaptive slicer level when no tt signal
        //  46 is  for  noise/eye pattern
        SLR_RegWrite( 0x5A60, 0x2150a023 ); // ew bf00aa60 = 2150a023  // bit 29  set that data from vd put through 16 tape low pass filter
        // start code correction is 80(50h)

        SLR_RegWrite( 0x5A6C, 0x00000000 ); // ew bf00aa6c = 00000000
        SLR_RegWrite( 0x5A74, 0x0f143110 ); // ew bf00aa74 = 0f143110  //  bit 20 = 1 : enable fix slicer level

        SLR_RegWrite( 0x5BC4, 0x00000001 ); // slicer data output rising edge triggered (1: rising (correct); 0: falling (failed))


        /*
                // Enable this setting when running Co-Channel tests:
                {
                    VD_RegWrite( 0x53C4, 0x00000080 );  // Enable PLL of VD.
                    SLR_RegWrite( 0x5A74, 0x0f043110 ); // ew bf00aa74 = 0f081110  //  setting when ch-channel series
                    SLR_RegWrite( 0x5A30, 0x00003C00 );
                    SLR_RegWrite( 0x5A6C, 0x40000000 );
                }
        */
        SLR_RegWrite( 0x5A94, 0x800e01d7 ); // ew bf00aa94  = 800e01d7 //  setting  check tt-line

    }
#else

    /*
        // For Slicer performance tests (CGMS)
        {
            SLR_RegWrite( 0x5800, 0x90000020 );  // rtd_outl(0xb8030800, 0x90000020);
            SLR_RegWrite( 0x5A00, 0x006c2713 );  // rtd_outl(0xb8030a00, 0x006c2713);      // fix slicer level =6C
            SLR_RegWrite( 0x5A08, 0x00000000 );  // rtd_outl(0xb8030a08, 0x00000000);  //line 9,10,11,12
            SLR_RegWrite( 0x5A0C, 0x00000000 );  // rtd_outl(0xb8030a0c, 0x00000000);
            SLR_RegWrite( 0x5A10, 0x00000000 );  // rtd_outl(0xb8030a10, 0x00000000);  //line 17,18,19,20
            SLR_RegWrite( 0x5A14, 0x00001100 );  // rtd_outl(0xb8030a14, 0x00001100);  //line 21,22,23,24
            SLR_RegWrite( 0x5A20, 0x00001ad0 );  // rtd_outl(0xb8030a20, 0x00001ad0);
            SLR_RegWrite( 0x5A68, 0x005a0100 );  // rtd_outl(0xb8030a68, 0x005a0100);
            SLR_RegWrite( 0x5A6C, 0x00000000 );  // rtd_outl(0xb8030a6c, 0x00000000);
            SLR_RegWrite( 0x5A74, 0x0f043110 );  // rtd_outl(0xb8030a74, 0x0f043110);
            SLR_RegWrite( 0x5A78, 0x0f001000 );  // rtd_outl(0xb8030a78, 0x0f001000);
            SLR_RegWrite( 0x5BC0, 0x80000009 );  // rtd_outl(0xb8030bc0, 0x80000009);  // vbi_slicer_1   debug mode

            SLR_RegWrite( 0x5800, 0x90000020 );  // rtd_outl(0xB8030800, 0x90000020)
            SLR_RegWrite( 0x5820, 0x00000000 );  // rtd_outl(0xB8030820, 0x00000000)

            SLR_RegWrite( 0x5900, 0x07006413 );  // rtd_outl(0xB8030900,0x0700641F)
            SLR_RegWrite( 0x5904, 0x000F000F );  // rtd_outl(0xB8030904,0x000c0114) // active line  17/281 , =>setting 12/276 due to shift 5 line
            SLR_RegWrite( 0x5908, 0x00300130 );  // rtd_outl(0xB8030908,0x005500CC)
            SLR_RegWrite( 0x590C, 0x0100001E );
            SLR_RegWrite( 0x5914, 0x00102419 );  // rtd_outl(0xB8030914,0x0032463C)

            // To check the result: When bit-24 of 0x5910 is one, if bit-19~0 is 0x896AA, then it's correct.

        }
    */
#endif  // end of #if

    return( TRUE );
}

/*---------------------------------------------------------------------------*/
bool_t
SLR_Close (void)
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
SLR_Reset (void)
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

}



/* -- Implementations of Private Functions ----------------------------------*/

/*---------------------------------------------------------------------------*\
 * end of slr.c                                                              *
\*---------------------------------------------------------------------------*/

