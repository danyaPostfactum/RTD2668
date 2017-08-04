/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     vd.c
 * @brief    Prototypes of public application interface.
 * @details  This header declares the public access application interface for this TtLib library.
 * @author   Janus Cheng
 * @date     Dec. 11, 2007
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/

#include "vd.h"            /* Provides: functions to be implemented */

#include <vd_hal.h>         /* Provides: CCL for VD */
#include <ttx_source.h>     /* Provides: TTX_SRC_Open() */


TTX_DEFINE_THIS_MODULE( vd.c )

#if 0 // not used
/* -- Prototypes of Private Functions ---------------------------------------*/
static void delay1ms(int count)
{
    int i,j;

    for (i=0; i<count; i++)
        for (j=0; j<4; j++);
}
#endif

static void     VD_TestRegsReadWrite   (void);

#define VIDEO_CONTROL1		                0x5004  // 0x01

//#define DFILTER_SEL		                    0x06
#define OUTPUT_CONTROL      	            0x503C  // 0x0F
#define FPGA_test_reg1			        	0x5430  // 0x10C
#define FPGA_test_reg2			        	0x5434  // 0x10D


/* -- Implementations of Public Functions -----------------------------------*/
#if 0	// not used
/*---------------------------------------------------------------------------*/
bool_t
VD_Open (const VD_OpenFlag_t flags)
/*****************************************************************************\
 *  @brief   The initialization routine for demo board.
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
        // Power-On default Tests
        VD_TestPwrOnDefaults();
        bFirst = FALSE;
    }

    if ( flags & VD_OPEN_RESET )
    {
        VD_Reset();
    }

    // vd_demoboard_AV.tbl
    {
        VD_RegWrite( 0x5780, 0x3f );		// global power on
        VD_RegWrite( 0x5784, 0x7f );		// vdc clamp power on
        VD_RegWrite( 0x5004, 0x01 );		// Set to AV
        VD_RegWrite( 0x5418, 0x00 );		// disable vd pattern generator

        VD_RegWrite( 0x5300, 0x10 );		// set Y form AD2
        VD_RegWrite( 0x5308, 0x0f );		// clamp enable, AD2 is Y
        VD_RegWrite( 0x5744, 0x42 );		// AV:VIN11P/VIN1N
    }


    return( TRUE );
}
#endif

#if 0
/*---------------------------------------------------------------------------*/
bool_t
VD_Open (const VD_OpenFlag_t flags)
/*****************************************************************************\
 *  @brief   The initialization routine for FPGA.
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
        // Power-On default Tests
        VD_TestPwrOnDefaults();
        bFirst = FALSE;
    }

    if ( flags & VD_OPEN_RESET )
    {
        VD_Reset();
    }


    // Important: Set to PAL mode. Becasue:
    //   1. VBIOSD requires VD in PAL mode.
    //   2. VD power-on default is in NTSC mode.
    {
        VD_RegWrite( 0x5000, 0x32 );
        VD_RegWrite( 0x5004, 0x00 );
        VD_RegWrite( 0x5090, 0x67 );
        VD_RegWrite( 0x5170, 0x2A );
        VD_RegWrite( 0x5174, 0x09 );
        VD_RegWrite( 0x5178, 0x8A );
        VD_RegWrite( 0x517C, 0xCB );
        VD_RegWrite( 0x5138, 0x84 );
        VD_RegWrite( 0x5140, 0x2A );
        VD_RegWrite( 0x5144, 0xC1 );
        VD_RegWrite( 0x51C0, 0x00 );
        VD_RegWrite( 0x51D4, 0xFF );
        VD_RegWrite( 0x5188, 0xFF );
    }

    /*
        // Important: Set to PAL mode. Becasue:
        //   1. VBIOSD requires VD in PAL mode.
        //   2. VD power-on default is in NTSC mode.
        {
            VD_RegWrite( 0x5000, 0x32 );
            VD_RegWrite( 0x5004, 0x00 );
            VD_RegWrite( 0x51C0, 0x06 );
            VD_RegWrite( 0x51D4, 0x08 );
            VD_RegWrite( 0x503c, 0x10 );
            VD_RegWrite( 0x5188, 0xFF );
        }
    */
    /*
        // Old: This is ported from Chin-How's King-Mice firmware.
        {
            //
            //VD_RegWrite(DFILTER_SEL,0x08);    // Set ADC run @ 27MHz
            VD_RegWrite(BPF_BW_SEL_Y,0x44); // Ychroma BPF BW
            VD_RegWrite(BPF_BW_SEL_C,0x77); // Cchroma BPF BW
            VD_RegWrite(ADAP_BPF_C_TH1,0x0c);
            VD_RegWrite(ADAP_BPF_C_TH2,0x14);
            VD_RegWrite(ADAP_BPF_C_TH3,0x04);
            VD_RegWrite(ADAP_BPF_C_TH4,0x4c);
            VD_RegWrite(ADAP_BPF_Y_TH1,0x08);
            VD_RegWrite(ADAP_BPF_Y_TH2,0x03);
            VD_RegWrite(ADAP_BPF_Y_TH3,0x01); //duallowpass BW
            VD_RegWrite(BW_detection,0x14);
            VD_RegWrite(palm_flag_compensation1,0x27);


            //wendy modified the values for the ntsc/pal
            VD_RegWrite(BPF_BW_SEL_Y,0x54);
            VD_RegWrite(BPF_BW_SEL_C,0x70);
            VD_RegWrite(ADAP_BPF_C_TH1,0x0c);
            VD_RegWrite(ADAP_BPF_C_TH2,0x14);
            VD_RegWrite(ADAP_BPF_C_TH3,0x04);
            VD_RegWrite(ADAP_BPF_C_TH4,0x4c);
            VD_RegWrite(ADAP_BPF_Y_TH1,0x08);
            VD_RegWrite(ADAP_BPF_Y_TH2,0x03);
            VD_RegWrite(ADAP_BPF_Y_TH3,0x00);
            VD_RegWrite(YC_BW_CTRL,0x01);
            VD_RegWrite(DEBUG_2D_MODE,0x00);
            VD_RegWrite(BW_detection,0x14);
            //VD_RegWrite(CHROMA_SATURATION_ADJ,0xba);

            VD_RegWrite(auto_mode_state_machine_statistic,0x02);
            VD_RegWrite(FPGA_test_reg1,0x22);

        }
    */
    /*
        // New: This is ported from Chin-How's King-Mice firmware.
        // AD-0:
        {
            VD_RegWrite(VIDEO_CONTROL1,0x01); //set to AV
            VD_RegWrite(YC_SEP_CONTROL,0x00); //YC seperation:adaptive
            VD_RegWrite(AD_SW_CTRL,0x1e);
            VD_RegWrite(CLAMP_SW_CTRL,0x1e);
            VD_RegWrite(CLAMP_UPDN_EN,0xff);  //clamping enable for all ADC
            VD_RegWrite(FSW_CTRL2,0x00);  //disable SCART function
            VD_RegWrite(VSYNC_TIME_CONST,0x8a);

            VD_RegWrite(OUTPUT_CONTROL,0x60); // enable blue screen mode

            VD_RegWrite(vd_basic_register_enable,0x00);

            VD_RegWrite(AD_SW_CTRL,0x40);
            VD_RegWrite(CLAMP_SW_CTRL,0x00);
            VD_RegWrite(CLAMP_UPDN_EN,0xff);
            VD_RegWrite(FSW_CTRL0,0x00);
            VD_RegWrite(FSW_CTRL1,0x00);
            VD_RegWrite(FSW_CTRL2,0x00);
            VD_RegWrite(AGC_GATE_TH_SWAP,0x0a);

            VD_RegWrite( FPGA_test_reg1, 0x22 );    // switch to from VBIOSD
        }
    */

    // This is from Wen-Chi's configuration to accept data from CVBS
    {
        VD_RegWrite( 0x5300, 0x10 );
        VD_RegWrite( 0x5308, 0x0F );
        VD_RegWrite( OUTPUT_CONTROL, 0x20 );
        VD_FSETS( AGC_GATE_TH_SWAP, ADC_UPDN_SWAP, DISABLE ); // VD_RegWrite( 0x508C, 0x0A );    // turn clamping swap off
        VD_RegWrite( FPGA_test_reg1, 0x22 );    // switch output mux to "from VBIOSD"
    }

    /*
        // Set VD free-run mode
        {
            VD_RegWrite( 0x51C0, 0x06 );
            VD_RegWrite( 0x53C4, 0x0B );
        }
    */


    return( TRUE );
}
#endif

/*---------------------------------------------------------------------------*/
bool_t
VD_Close (void)
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

#if 0	// not used
/*---------------------------------------------------------------------------*/
void
VD_Reset (void)
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
    VD_FSETS( RESET, SOFT, ASSERT   );
    delay1ms(5);
    VD_FSETS( RESET, SOFT, DEASSERT );

    // Reset FIFO status
    VD_RSET ( FIFO_STATUS, VD_FIFO_STATUS_RMKS( CLEAR ,  /* FULL  */  \
              CLEAR    /* EMPTY */  \
                                              ));
}
#endif

#if TTX_VERIFY  /* if enabled verficiation of TTX related IPs */

/*---------------------------------------------------------------------------*/
bool_t
VD_Test (void)
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
    // Register read-write tests
//    VD_TestRegsReadWrite();

    return( TRUE );
}

#endif  /* end of #if TTX_VERIFY */

#if 0	// not used
/*---------------------------------------------------------------------------*/
void
VD_TestPwrOnDefaults   (void)
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
\*****************************************************************************/
{
    VD_Log( LOGGER_VERBOSE, "[VD]" "[Trace]" "Entering VD_TestPwrOnDefaults()... ");

#define     VD_DEFAULT_TEST(reg)  VD_ENSURE( VD_RGET( reg ) == VD_##reg##_DEFAULT )
    VD_DEFAULT_TEST( CTRL0 );
    VD_DEFAULT_TEST( OUTPUT_CTRL );
    VD_DEFAULT_TEST( RESET );
    VD_DEFAULT_TEST( HSYNC_RISING );
    VD_DEFAULT_TEST( HSYNC_GATE_END );
    VD_DEFAULT_TEST( FIFO_CTRL );
    VD_DEFAULT_TEST( FIFO_STATUS );

    VD_Log( LOGGER_VERBOSE, "[done]\n");
}
#endif

/* -- Implementations of Private Functions ----------------------------------*/

#if TTX_VERIFY  /* if enabled verficiation of TTX related IPs */

/*---------------------------------------------------------------------------*/
static void
VD_TestRegsReadWrite   (void)
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
\*****************************************************************************/
{
    volatile uint8_t orig;   // all VD's registers' values are 8-bit wide

    VD_Log( LOGGER_VERBOSE,   "[VD]" "[Trace]" "Entering VD_TestRegsReadWrite()... ");

    {
        orig = VD_RGET( CTRL0 );    // store original register value
        VD_RSET( CTRL0, 0x80 );
        VD_ENSURE( VD_RGET( CTRL0 ) == 0x80 );
        VD_RSET( CTRL0, orig );     // restore original register value
    }

    /* <Janus>2008/09/30: I don't know why this fails.
        {
            orig = VD_RegRead( 0x5194 );  // store original register value
            VD_RegWrite( 0x5194, 0x08 );
            VD_ENSURE( VD_RegRead( 0x5194 ) == 0x08 );
            VD_Log( LOGGER_VERBOSE,   "[VD]" "[Trace]" "Reg0x5194 == 0x%X\n", (uint32_t)VD_RegRead(0x5194) );
            VD_RegWrite( 0x5194, orig );  // restore original register value
        }
    */

    {
        orig = VD_RegRead( 0x5038 );  // store original register value
        VD_RegWrite( 0x5038, 0x20 );
        VD_ENSURE( VD_RegRead( 0x5038 ) == 0x20 );
        VD_RegWrite( 0x5038, orig );  // restore original register value
    }

    // The bit-2 and bit-3 of this register are tie to 0 in bit-file v20081117.
    {
        orig = VD_RegRead( 0x5180 );  // store original register value
        VD_RegWrite( 0x5180, 0xFF );
        VD_ENSURE( VD_RegRead( 0x5180 ) == 0xFF );
        VD_RegWrite( 0x5180, orig );  // restore original register value
    }


    VD_Log( LOGGER_VERBOSE,   "[done]\n");
}

#endif  /* end of #if TTX_VERIFY */


/*---------------------------------------------------------------------------*\
 * end of vd.c                                                              *
\*---------------------------------------------------------------------------*/

