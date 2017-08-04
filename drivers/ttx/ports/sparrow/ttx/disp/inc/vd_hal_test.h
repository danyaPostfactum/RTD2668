/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     vd_hal_test.h
 * @brief    Compile-time tests for VD HAL.
 * @details  Include this file to test CCL. No obj code will be generated.
 *             This file to be included in private CPP file.
 * @author   Janus Cheng
 * @date     Sep. 14, 2008
 * @version  0.1
\*****************************************************************************/

#include "vd_hal.h"                /* Provides: VD HAL macros to be tested */
#include "../../inc/ttx_assert.h"   /* Provides: TTX_ASSERT_COMPILE() */

/* Test VD_CTRL0_REG */
TTX_ASSERT_COMPILE( VD_CTRL0_DEFAULT        == 0x00 );

TTX_ASSERT_COMPILE( VD_CTRL0_RMKS(DISABLE,    /* HDLY  */ \
                                  858PPL ,    /* HPXL  */ \
                                  525    ,    /* VLINE */ \
                                  NTSC        /* COLOR */ \
                                 ) == 0x00 );

TTX_ASSERT_COMPILE( VD_CTRL0_RMKS(ENABLE ,    /* HDLY  */ \
                                  944PPL ,    /* HPXL  */ \
                                  625    ,    /* VLINE */ \
                                  PAL60       /* COLOR */ \
                                 ) == 0xFE );

TTX_ASSERT_COMPILE( VD_CTRL0_RMKS(ENABLE ,    /* HDLY  */ \
                                  858PPL ,    /* HPXL  */ \
                                  525    ,    /* VLINE */ \
                                  NTSC        /* COLOR */ \
                                 ) == 0x80 );

TTX_ASSERT_COMPILE( VD_CTRL0_RMKS(DISABLE,    /* HDLY  */ \
                                  944PPL ,    /* HPXL  */ \
                                  525    ,    /* VLINE */ \
                                  NTSC        /* COLOR */ \
                                 ) == 0x60 );

TTX_ASSERT_COMPILE( VD_CTRL0_RMKS(DISABLE,    /* HDLY  */ \
                                  780PPL ,    /* HPXL  */ \
                                  525    ,    /* VLINE */ \
                                  NTSC        /* COLOR */ \
                                 ) == 0x40 );

TTX_ASSERT_COMPILE( VD_CTRL0_RMKS(DISABLE,    /* HDLY  */ \
                                  944PPL ,    /* HPXL  */ \
                                  625    ,    /* VLINE */ \
                                  NTSC        /* COLOR */ \
                                 ) == 0x70 );

TTX_ASSERT_COMPILE( VD_CTRL0_RMKS(DISABLE,    /* HDLY  */ \
                                  858PPL ,    /* HPXL  */ \
                                  525    ,    /* VLINE */ \
                                  NTSC443B    /* COLOR */ \
                                 ) == 0x0C );


/* Test VD_STATUS1_REG */
TTX_ASSERT_COMPILE( VD_STATUS1_DEFAULT        == 0x00 );

/* Test VD_OUTPUT_CTRL_REG */
TTX_ASSERT_COMPILE( VD_OUTPUT_CTRL_DEFAULT        == 0xA0 );

TTX_ASSERT_COMPILE( VD_OUTPUT_CTRL_RMKS(DISABLE,    /* SFE  */ \
                                        DISABLE,    /* CSWP */ \
                                        DISABLE,    /*  BM  */ \
                                        0x0         /* YCD  */ \
                                       ) == 0x00 );

TTX_ASSERT_COMPILE( VD_OUTPUT_CTRL_RMKS(ENABLE ,    /* SFE  */ \
                                        DISABLE,    /* CSWP */ \
                                        DISABLE,    /*  BM  */ \
                                        0x0         /* YCD  */ \
                                       ) == 0x80 );

TTX_ASSERT_COMPILE( VD_OUTPUT_CTRL_RMKS(DISABLE,    /* SFE  */ \
                                        ENABLE ,    /* CSWP */ \
                                        DISABLE,    /*  BM  */ \
                                        0x0         /* YCD  */ \
                                       ) == 0x40 );

TTX_ASSERT_COMPILE( VD_OUTPUT_CTRL_RMKS(DISABLE,    /* SFE  */ \
                                        DISABLE,    /* CSWP */ \
                                        AUTO   ,    /*  BM  */ \
                                        0x0         /* YCD  */ \
                                       ) == 0x20 );

TTX_ASSERT_COMPILE( VD_OUTPUT_CTRL_RMKS(DISABLE,    /* SFE  */ \
                                        DISABLE,    /* CSWP */ \
                                        ENABLE ,    /*  BM  */ \
                                        0x0         /* YCD  */ \
                                       ) == 0x10 );

TTX_ASSERT_COMPILE( VD_OUTPUT_CTRL_RMKS(DISABLE,    /* SFE  */ \
                                        DISABLE,    /* CSWP */ \
                                        DISABLE,    /*  BM  */ \
                                        0xF         /* YCD  */ \
                                       ) == 0x0F );

TTX_ASSERT_COMPILE( VD_OUTPUT_CTRL_RMKS(DISABLE,    /* SFE  */ \
                                        ENABLE ,    /* CSWP */ \
                                        DISABLE,    /*  BM  */ \
                                        0xA         /* YCD  */ \
                                       ) == 0x4A );

/* Test VD_AGC_GATE_TH_SWAP_REG */
TTX_ASSERT_COMPILE( VD_AGC_GATE_TH_SWAP_DEFAULT        == 0x8A );

TTX_ASSERT_COMPILE( VD_AGC_GATE_TH_SWAP_RMKS(DISABLE,    /* ADC_UPDN_SWAP      */ \
                    DISABLE,    /* ADC_INPUT_SWAP     */ \
                    DISABLE,    /* ADC_CBCR_PUMP_SWAP */ \
                    0x00        /* AGC_GATE_THRESH    */ \
                                            ) == 0x00 );

TTX_ASSERT_COMPILE( VD_AGC_GATE_TH_SWAP_RMKS(ENABLE ,    /* ADC_UPDN_SWAP      */ \
                    ENABLE ,    /* ADC_INPUT_SWAP     */ \
                    ENABLE ,    /* ADC_CBCR_PUMP_SWAP */ \
                    0x1F        /* AGC_GATE_THRESH    */ \
                                            ) == 0xFF );

TTX_ASSERT_COMPILE( VD_AGC_GATE_TH_SWAP_RMKS(DISABLE,    /* ADC_UPDN_SWAP      */ \
                    ENABLE ,    /* ADC_INPUT_SWAP     */ \
                    DISABLE,    /* ADC_CBCR_PUMP_SWAP */ \
                    0x15        /* AGC_GATE_THRESH    */ \
                                            ) == 0x55 );

TTX_ASSERT_COMPILE( VD_AGC_GATE_TH_SWAP_RMKS(ENABLE ,    /* ADC_UPDN_SWAP      */ \
                    DISABLE,    /* ADC_INPUT_SWAP     */ \
                    ENABLE ,    /* ADC_CBCR_PUMP_SWAP */ \
                    0x0A        /* AGC_GATE_THRESH    */ \
                                            ) == 0xAA );

TTX_ASSERT_COMPILE( VD_AGC_GATE_TH_SWAP_RMKS(DISABLE,    /* ADC_UPDN_SWAP      */ \
                    DISABLE,    /* ADC_INPUT_SWAP     */ \
                    DISABLE,    /* ADC_CBCR_PUMP_SWAP */ \
                    0x83        /* AGC_GATE_THRESH    */ \
                                            ) == 0x03 );

TTX_ASSERT_COMPILE( VD_AGC_GATE_TH_SWAP_RMKS(ENABLE ,    /* ADC_UPDN_SWAP      */ \
                    DISABLE,    /* ADC_INPUT_SWAP     */ \
                    DISABLE,    /* ADC_CBCR_PUMP_SWAP */ \
                    0x07        /* AGC_GATE_THRESH    */ \
                                            ) == 0x87 );

TTX_ASSERT_COMPILE( VD_AGC_GATE_TH_SWAP_RMKS(DISABLE,    /* ADC_UPDN_SWAP      */ \
                    ENABLE ,    /* ADC_INPUT_SWAP     */ \
                    ENABLE,    /* ADC_CBCR_PUMP_SWAP */ \
                    0x14        /* AGC_GATE_THRESH    */ \
                                            ) == 0x74 );

TTX_ASSERT_COMPILE( VD_AGC_GATE_TH_SWAP_RMKS(ENABLE ,    /* ADC_UPDN_SWAP      */ \
                    ENABLE ,    /* ADC_INPUT_SWAP     */ \
                    DISABLE,    /* ADC_CBCR_PUMP_SWAP */ \
                    0x19        /* AGC_GATE_THRESH    */ \
                                            ) == 0xD9 );


/* Test VD_RESET_REG */
TTX_ASSERT_COMPILE( VD_RESET_DEFAULT        == 0x00 );

TTX_ASSERT_COMPILE( VD_RESET_RMKS(DEASSERT    /* SOFT */ \
                                 ) == 0x00 );

TTX_ASSERT_COMPILE( VD_RESET_RMKS(ASSERT      /* SOFT */ \
                                 ) == 0x01 );


/* Test VD_HSYNC_RISING_REG */
TTX_ASSERT_COMPILE( VD_HSYNC_RISING_DEFAULT        == 0x3E );

TTX_ASSERT_COMPILE( VD_HSYNC_RISING_RMK(0x00    /* VALUE */ \
                                       ) == 0x00 );

TTX_ASSERT_COMPILE( VD_HSYNC_RISING_RMK(0xFF    /* VALUE */ \
                                       ) == 0xFF );

TTX_ASSERT_COMPILE( VD_HSYNC_RISING_RMK(0x0F    /* VALUE */ \
                                       ) == 0x0F );

TTX_ASSERT_COMPILE( VD_HSYNC_RISING_RMK(0xF0    /* VALUE */ \
                                       ) == 0xF0 );

TTX_ASSERT_COMPILE( VD_HSYNC_RISING_RMK(0x5A    /* VALUE */ \
                                       ) == 0x5A );

TTX_ASSERT_COMPILE( VD_HSYNC_RISING_RMK(0xA5    /* VALUE */ \
                                       ) == 0xA5 );


/* Test VD_HSYNC_GATE_END_REG */
TTX_ASSERT_COMPILE( VD_HSYNC_GATE_END_DEFAULT        == 0x80 );

TTX_ASSERT_COMPILE( VD_HSYNC_GATE_END_RMK(0x00    /* VALUE */ \
                                         ) == 0x00 );

TTX_ASSERT_COMPILE( VD_HSYNC_GATE_END_RMK(0xFF    /* VALUE */ \
                                         ) == 0xFF );

TTX_ASSERT_COMPILE( VD_HSYNC_GATE_END_RMK(0x0F    /* VALUE */ \
                                         ) == 0x0F );

TTX_ASSERT_COMPILE( VD_HSYNC_GATE_END_RMK(0xF0    /* VALUE */ \
                                         ) == 0xF0 );

TTX_ASSERT_COMPILE( VD_HSYNC_GATE_END_RMK(0x5A    /* VALUE */ \
                                         ) == 0x5A );

TTX_ASSERT_COMPILE( VD_HSYNC_GATE_END_RMK(0xA5    /* VALUE */ \
                                         ) == 0xA5 );

/* Test VD_FIFO_CTRL_REG */
TTX_ASSERT_COMPILE( VD_FIFO_CTRL_DEFAULT        == 0x80 );

TTX_ASSERT_COMPILE( VD_FIFO_CTRL_RMKS(  ORIG   ,  /* V8FMT */  \
                                        NO_SWAP ,  /* PSWAP */  \
                                        DEASSERT   /* INIT  */  \
                                     ) == 0x00  );

TTX_ASSERT_COMPILE( VD_FIFO_CTRL_RMKS(  ORIG   ,  /* V8FMT */  \
                                        NO_SWAP ,  /* PSWAP */  \
                                        ASSERT    /* INIT  */  \
                                     ) == 0x01  );

TTX_ASSERT_COMPILE( VD_FIFO_CTRL_RMKS(  ORIG   ,  /* V8FMT */  \
                                        SWAP   ,  /* PSWAP */  \
                                        DEASSERT   /* INIT  */  \
                                     ) == 0x04  );

TTX_ASSERT_COMPILE( VD_FIFO_CTRL_RMKS(  ORIG   ,  /* V8FMT */  \
                                        SWAP  ,  /* PSWAP */  \
                                        ASSERT   /* INIT  */  \
                                     ) == 0x05  );

TTX_ASSERT_COMPILE( VD_FIFO_CTRL_RMKS(  DIRECT ,  /* V8FMT */  \
                                        NO_SWAP ,  /* PSWAP */  \
                                        DEASSERT   /* INIT  */  \
                                     ) == 0x80  );

TTX_ASSERT_COMPILE( VD_FIFO_CTRL_RMKS(  DIRECT ,  /* V8FMT */  \
                                        NO_SWAP ,  /* PSWAP */  \
                                        ASSERT    /* INIT  */  \
                                     ) == 0x81  );

TTX_ASSERT_COMPILE( VD_FIFO_CTRL_RMKS(  DIRECT ,  /* V8FMT */  \
                                        SWAP   ,  /* PSWAP */  \
                                        DEASSERT   /* INIT  */  \
                                     ) == 0x84  );

TTX_ASSERT_COMPILE( VD_FIFO_CTRL_RMKS(  DIRECT ,  /* V8FMT */  \
                                        SWAP  ,  /* PSWAP */  \
                                        ASSERT   /* INIT  */  \
                                     ) == 0x85  );



/* Test VD_FIFO_STATUS_REG */
TTX_ASSERT_COMPILE( VD_FIFO_STATUS_DEFAULT      == 0x00 );

TTX_ASSERT_COMPILE( VD_FIFO_STATUS_RMKS( NOP   ,  /* FULL  */  \
                    NOP      /* EMPTY */  \
                                       ) == 0x00  );

TTX_ASSERT_COMPILE( VD_FIFO_STATUS_RMKS( NOP   ,   /* FULL  */  \
                    CLEAR     /* EMPTY */  \
                                       ) == 0x01  );

TTX_ASSERT_COMPILE( VD_FIFO_STATUS_RMKS( CLEAR ,  /* FULL  */  \
                    NOP      /* EMPTY */  \
                                       ) == 0x02  );

TTX_ASSERT_COMPILE( VD_FIFO_STATUS_RMKS( CLEAR ,  /* FULL  */  \
                    CLEAR    /* EMPTY */  \
                                       ) == 0x03  );



/*-------------------------------------------------------------------------*\
 * End of vd_hal_test.h                                                   *
\*-------------------------------------------------------------------------*/

