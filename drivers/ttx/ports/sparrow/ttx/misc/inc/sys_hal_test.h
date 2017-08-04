/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     sys_hal_test.h
 * @brief    Compile-time tests for SYS HAL.
 * @details  Include this file to test CCL. No obj code will be generated.
 *             This file to be included in private CPP file.
 * @author   Janus Cheng
 * @date     Sep. 14, 2008
 * @version  0.1
\*****************************************************************************/

#include "sys_hal.h"            /* Provides: SYS HAL macros to be tested */
#include <ttx_assert.h>         /* Provides: TTX_ASSERT_COMPILE() */

/* Test SYS_IMEM4K_ADDR_REG */
TTX_ASSERT_COMPILE( SYS_IMEM4K_ADDR_DEFAULT        == 0x00 );

TTX_ASSERT_COMPILE( SYS_IMEM4K_ADDR_RMK(0xFF  /* ADDR */ \
                                       ) == 0x0F );

TTX_ASSERT_COMPILE( SYS_IMEM4K_ADDR_RMK(0x5A  /* ADDR */ \
                                       ) == 0x0A );

TTX_ASSERT_COMPILE( SYS_IMEM4K_ADDR_RMK(0xA5  /* ADDR */ \
                                       ) == 0x05 );

TTX_ASSERT_COMPILE( SYS_IMEM4K_ADDR_RMK(0xF0  /* ADDR */ \
                                       ) == 0x00 );

TTX_ASSERT_COMPILE( SYS_IMEM4K_ADDR_RMK(0x0F  /* ADDR */ \
                                       ) == 0x0F );

TTX_ASSERT_COMPILE( SYS_IMEM4K_ADDR_RMK(0x03  /* ADDR */ \
                                       ) == 0x03 );


/* Test SYS_IMEM4K_BANK_REG */
TTX_ASSERT_COMPILE( SYS_IMEM4K_BANK_DEFAULT        == 0x00 );

TTX_ASSERT_COMPILE( SYS_IMEM4K_BANK_RMK(0xFF  /* BANK */ \
                                       ) == 0xFF );

TTX_ASSERT_COMPILE( SYS_IMEM4K_BANK_RMK(0x5A  /* BANK */ \
                                       ) == 0x5A );

TTX_ASSERT_COMPILE( SYS_IMEM4K_BANK_RMK(0xA5  /* BANK */ \
                                       ) == 0xA5 );

TTX_ASSERT_COMPILE( SYS_IMEM4K_BANK_RMK(0xF0  /* BANK */ \
                                       ) == 0xF0 );

TTX_ASSERT_COMPILE( SYS_IMEM4K_BANK_RMK(0x0F  /* BANK */ \
                                       ) == 0x0F );

TTX_ASSERT_COMPILE( SYS_IMEM4K_BANK_RMK(0x83  /* BANK */ \
                                       ) == 0x83 );


/* Test SYS_IMEM4K_REMAP_REG */
TTX_ASSERT_COMPILE( SYS_IMEM4K_REMAP_DEFAULT        == 0x00 );

TTX_ASSERT_COMPILE( SYS_IMEM4K_REMAP_RMKS(STOP  /* REMAP */ \
                                         ) == 0x00 );

TTX_ASSERT_COMPILE( SYS_IMEM4K_REMAP_RMKS(START /* REMAP */ \
                                         ) == 0x01 );


/* Test SYS_IMEM8K_ADDR_REG */
TTX_ASSERT_COMPILE( SYS_IMEM8K_ADDR_DEFAULT        == 0x00 );

TTX_ASSERT_COMPILE( SYS_IMEM8K_ADDR_RMK(0xFF  /* ADDR */ \
                                       ) == 0x0F );

TTX_ASSERT_COMPILE( SYS_IMEM8K_ADDR_RMK(0x5A  /* ADDR */ \
                                       ) == 0x0A );

TTX_ASSERT_COMPILE( SYS_IMEM8K_ADDR_RMK(0xA5  /* ADDR */ \
                                       ) == 0x05 );

TTX_ASSERT_COMPILE( SYS_IMEM8K_ADDR_RMK(0xF0  /* ADDR */ \
                                       ) == 0x00 );

TTX_ASSERT_COMPILE( SYS_IMEM8K_ADDR_RMK(0x0F  /* ADDR */ \
                                       ) == 0x0F );

TTX_ASSERT_COMPILE( SYS_IMEM8K_ADDR_RMK(0x03  /* ADDR */ \
                                       ) == 0x03 );


/* Test SYS_IMEM8K_BANK_REG */
TTX_ASSERT_COMPILE( SYS_IMEM8K_BANK_DEFAULT        == 0x00 );

TTX_ASSERT_COMPILE( SYS_IMEM8K_BANK_RMK(0xFF  /* BANK */ \
                                       ) == 0xFF );

TTX_ASSERT_COMPILE( SYS_IMEM8K_BANK_RMK(0x5A  /* BANK */ \
                                       ) == 0x5A );

TTX_ASSERT_COMPILE( SYS_IMEM8K_BANK_RMK(0xA5  /* BANK */ \
                                       ) == 0xA5 );

TTX_ASSERT_COMPILE( SYS_IMEM8K_BANK_RMK(0xF0  /* BANK */ \
                                       ) == 0xF0 );

TTX_ASSERT_COMPILE( SYS_IMEM8K_BANK_RMK(0x0F  /* BANK */ \
                                       ) == 0x0F );

TTX_ASSERT_COMPILE( SYS_IMEM8K_BANK_RMK(0x83  /* BANK */ \
                                       ) == 0x83 );


/* Test SYS_IMEM8K_REMAP_REG */
TTX_ASSERT_COMPILE( SYS_IMEM8K_REMAP_DEFAULT        == 0x00 );

TTX_ASSERT_COMPILE( SYS_IMEM8K_REMAP_RMKS(STOP  /* REMAP */ \
                                         ) == 0x00 );

TTX_ASSERT_COMPILE( SYS_IMEM8K_REMAP_RMKS(START /* REMAP */ \
                                         ) == 0x01 );

/* Test SYS_REGEND_REG */
TTX_ASSERT_COMPILE( SYS_REGEND_DEFAULT        == 0x00 );

TTX_ASSERT_COMPILE( SYS_REGEND_RMKS(BIG     /* SWAP */ \
                                   ) == 0x00 );

TTX_ASSERT_COMPILE( SYS_REGEND_RMKS(LITTLE  /* SWAP */ \
                                   ) == 0x01 );

/* Test SYS_GMAP_REG */
TTX_ASSERT_COMPILE( SYS_GMAP_DEFAULT        == 0x00 );

TTX_ASSERT_COMPILE( SYS_GMAP_RMKS(STOP , /* IMEM4K */ \
                                  STOP   /* IMEM8K */ \
                                 ) == 0x00 );

TTX_ASSERT_COMPILE( SYS_GMAP_RMKS(STOP , /* IMEM4K */ \
                                  START  /* IMEM8K */ \
                                 ) == 0x01 );

TTX_ASSERT_COMPILE( SYS_GMAP_RMKS(START, /* IMEM4K */ \
                                  STOP   /* IMEM8K */ \
                                 ) == 0x02 );

TTX_ASSERT_COMPILE( SYS_GMAP_RMKS(START, /* IMEM4K */ \
                                  START  /* IMEM8K */ \
                                 ) == 0x03 );

/* Test SYS_SDMA_FLASHLSB_REG */
TTX_ASSERT_COMPILE( SYS_SDMA_FLASHLSB_DEFAULT        == 0x00 );

TTX_ASSERT_COMPILE( SYS_SDMA_FLASHLSB_RMK(0x00  /* ADDR */ \
                                         ) == 0x00 );

TTX_ASSERT_COMPILE( SYS_SDMA_FLASHLSB_RMK(0xFF  /* ADDR */ \
                                         ) == 0xFF );

TTX_ASSERT_COMPILE( SYS_SDMA_FLASHLSB_RMK(0x0F  /* ADDR */ \
                                         ) == 0x0F );

TTX_ASSERT_COMPILE( SYS_SDMA_FLASHLSB_RMK(0xF0  /* ADDR */ \
                                         ) == 0xF0 );

TTX_ASSERT_COMPILE( SYS_SDMA_FLASHLSB_RMK(0xA5  /* ADDR */ \
                                         ) == 0xA5 );

TTX_ASSERT_COMPILE( SYS_SDMA_FLASHLSB_RMK(0x5A  /* ADDR */ \
                                         ) == 0x5A );

TTX_ASSERT_COMPILE( SYS_SDMA_FLASHLSB_RMK(0x12  /* ADDR */ \
                                         ) == 0x12 );

/* Test SYS_SDMA_FLASHMSB_REG */
TTX_ASSERT_COMPILE( SYS_SDMA_FLASHMSB_DEFAULT        == 0x00 );

TTX_ASSERT_COMPILE( SYS_SDMA_FLASHMSB_RMK(0x00  /* ADDR */ \
                                         ) == 0x00 );

TTX_ASSERT_COMPILE( SYS_SDMA_FLASHMSB_RMK(0xFF  /* ADDR */ \
                                         ) == 0xFF );

TTX_ASSERT_COMPILE( SYS_SDMA_FLASHMSB_RMK(0x0F  /* ADDR */ \
                                         ) == 0x0F );

TTX_ASSERT_COMPILE( SYS_SDMA_FLASHMSB_RMK(0xF0  /* ADDR */ \
                                         ) == 0xF0 );

TTX_ASSERT_COMPILE( SYS_SDMA_FLASHMSB_RMK(0xA5  /* ADDR */ \
                                         ) == 0xA5 );

TTX_ASSERT_COMPILE( SYS_SDMA_FLASHMSB_RMK(0x5A  /* ADDR */ \
                                         ) == 0x5A );

TTX_ASSERT_COMPILE( SYS_SDMA_FLASHMSB_RMK(0x12  /* ADDR */ \
                                         ) == 0x12 );

/* Test SYS_SDMA_BANK_REG */
TTX_ASSERT_COMPILE( SYS_SDMA_BANK_DEFAULT        == 0x00 );

TTX_ASSERT_COMPILE( SYS_SDMA_BANK_RMK(0x00  /* BANK */ \
                                     ) == 0x00 );

TTX_ASSERT_COMPILE( SYS_SDMA_BANK_RMK(0xFF  /* BANK */ \
                                     ) == 0x1F );

TTX_ASSERT_COMPILE( SYS_SDMA_BANK_RMK(0x0F  /* BANK */ \
                                     ) == 0x0F );

TTX_ASSERT_COMPILE( SYS_SDMA_BANK_RMK(0xF0  /* BANK */ \
                                     ) == 0x10 );

TTX_ASSERT_COMPILE( SYS_SDMA_BANK_RMK(0xA5  /* BANK */ \
                                     ) == 0x05 );

TTX_ASSERT_COMPILE( SYS_SDMA_BANK_RMK(0x5A  /* BANK */ \
                                     ) == 0x1A );

TTX_ASSERT_COMPILE( SYS_SDMA_BANK_RMK(0x12  /* BANK */ \
                                     ) == 0x12 );

/* Test SYS_SDMA_CTRL_REG */
TTX_ASSERT_COMPILE( SYS_SDMA_CTRL_DEFAULT        == 0x00 );

TTX_ASSERT_COMPILE( SYS_SDMA_CTRL_RMKS(  0    ,   /* MODE */ \
                    STOP       /*  GO  */ \
                                      ) == 0x00 );

TTX_ASSERT_COMPILE( SYS_SDMA_CTRL_RMKS(   1    ,   /* MODE */ \
                    STOP        /*  GO  */ \
                                      ) == 0x04 );

TTX_ASSERT_COMPILE( SYS_SDMA_CTRL_RMKS(   2    ,   /* MODE */ \
                    STOP        /*  GO  */ \
                                      ) == 0x08 );

TTX_ASSERT_COMPILE( SYS_SDMA_CTRL_RMKS(   0    ,   /* MODE */ \
                    START       /*  GO  */ \
                                      ) == 0x01 );

TTX_ASSERT_COMPILE( SYS_SDMA_CTRL_RMKS(   1    ,   /* MODE */ \
                    START       /*  GO  */ \
                                      ) == 0x05 );

TTX_ASSERT_COMPILE( SYS_SDMA_CTRL_RMKS(   2    ,   /* MODE */ \
                    START       /*  GO  */ \
                                      ) == 0x09 );

/* Test SYS_SDMA_LENLSB_REG */
TTX_ASSERT_COMPILE( SYS_SDMA_LENLSB_DEFAULT        == 0x00 );

TTX_ASSERT_COMPILE( SYS_SDMA_LENLSB_RMK(0x00  /* SIZE */ \
                                       ) == 0x00 );

TTX_ASSERT_COMPILE( SYS_SDMA_LENLSB_RMK(0xFF  /* SIZE */ \
                                       ) == 0xFF );

TTX_ASSERT_COMPILE( SYS_SDMA_LENLSB_RMK(0x0F  /* SIZE */ \
                                       ) == 0x0F );

TTX_ASSERT_COMPILE( SYS_SDMA_LENLSB_RMK(0xF0  /* SIZE */ \
                                       ) == 0xF0 );

TTX_ASSERT_COMPILE( SYS_SDMA_LENLSB_RMK(0xA5  /* SIZE */ \
                                       ) == 0xA5 );

TTX_ASSERT_COMPILE( SYS_SDMA_LENLSB_RMK(0x5A  /* SIZE */ \
                                       ) == 0x5A );

TTX_ASSERT_COMPILE( SYS_SDMA_LENLSB_RMK(0x12  /* SIZE */ \
                                       ) == 0x12 );

/* Test SYS_SDMA_LENMSB_REG */
TTX_ASSERT_COMPILE( SYS_SDMA_LENMSB_DEFAULT        == 0x00 );

TTX_ASSERT_COMPILE( SYS_SDMA_LENMSB_RMK(0x00  /* SIZE */ \
                                       ) == 0x00 );

TTX_ASSERT_COMPILE( SYS_SDMA_LENMSB_RMK(0xFF  /* SIZE */ \
                                       ) == 0xFF );

TTX_ASSERT_COMPILE( SYS_SDMA_LENMSB_RMK(0x0F  /* SIZE */ \
                                       ) == 0x0F );

TTX_ASSERT_COMPILE( SYS_SDMA_LENMSB_RMK(0xF0  /* SIZE */ \
                                       ) == 0xF0 );

TTX_ASSERT_COMPILE( SYS_SDMA_LENMSB_RMK(0xA5  /* SIZE */ \
                                       ) == 0xA5 );

TTX_ASSERT_COMPILE( SYS_SDMA_LENMSB_RMK(0x5A  /* SIZE */ \
                                       ) == 0x5A );

TTX_ASSERT_COMPILE( SYS_SDMA_LENMSB_RMK(0x12  /* SIZE */ \
                                       ) == 0x12 );

/* Test SYS_SDMA_DESTLSB_REG */
TTX_ASSERT_COMPILE( SYS_SDMA_DESTLSB_DEFAULT        == 0x00 );

TTX_ASSERT_COMPILE( SYS_SDMA_DESTLSB_RMK(0x00  /* ADDR */ \
                                        ) == 0x00 );

TTX_ASSERT_COMPILE( SYS_SDMA_DESTLSB_RMK(0xFF  /* ADDR */ \
                                        ) == 0xFF );

TTX_ASSERT_COMPILE( SYS_SDMA_DESTLSB_RMK(0x0F  /* ADDR */ \
                                        ) == 0x0F );

TTX_ASSERT_COMPILE( SYS_SDMA_DESTLSB_RMK(0xF0  /* ADDR */ \
                                        ) == 0xF0 );

TTX_ASSERT_COMPILE( SYS_SDMA_DESTLSB_RMK(0xA5  /* ADDR */ \
                                        ) == 0xA5 );

TTX_ASSERT_COMPILE( SYS_SDMA_DESTLSB_RMK(0x5A  /* ADDR */ \
                                        ) == 0x5A );

TTX_ASSERT_COMPILE( SYS_SDMA_DESTLSB_RMK(0x12  /* ADDR */ \
                                        ) == 0x12 );

/* Test SYS_SDMA_DESTMSB_REG */
TTX_ASSERT_COMPILE( SYS_SDMA_DESTMSB_DEFAULT        == 0x00 );

TTX_ASSERT_COMPILE( SYS_SDMA_DESTMSB_RMK(0x00  /* ADDR */ \
                                        ) == 0x00 );

TTX_ASSERT_COMPILE( SYS_SDMA_DESTMSB_RMK(0xFF  /* ADDR */ \
                                        ) == 0xFF );

TTX_ASSERT_COMPILE( SYS_SDMA_DESTMSB_RMK(0x0F  /* ADDR */ \
                                        ) == 0x0F );

TTX_ASSERT_COMPILE( SYS_SDMA_DESTMSB_RMK(0xF0  /* ADDR */ \
                                        ) == 0xF0 );

TTX_ASSERT_COMPILE( SYS_SDMA_DESTMSB_RMK(0xA5  /* ADDR */ \
                                        ) == 0xA5 );

TTX_ASSERT_COMPILE( SYS_SDMA_DESTMSB_RMK(0x5A  /* ADDR */ \
                                        ) == 0x5A );

TTX_ASSERT_COMPILE( SYS_SDMA_DESTMSB_RMK(0x12  /* ADDR */ \
                                        ) == 0x12 );

/* Test SYS_SRST0_REG */
TTX_ASSERT_COMPILE( SYS_SRST0_DEFAULT        == 0x00 );

TTX_ASSERT_COMPILE( SYS_SRST0_RMKS(RESET,  /*   HDMI   */ \
                                   RESET,  /*   SLCR   */ \
                                   RESET,  /*   IFD    */ \
                                   RESET,  /*   VDEC   */ \
                                   RESET,  /*  FLASH   */ \
                                   RESET,  /*  SPIDMA  */ \
                                   RESET   /*   RBUS   */ \
                                  ) == 0x00 );

TTX_ASSERT_COMPILE( SYS_SRST0_RMKS( NOP ,  /*   HDMI   */ \
                                    NOP ,  /*   SLCR   */ \
                                    NOP ,  /*   IFD    */ \
                                    NOP ,  /*   VDEC   */ \
                                    NOP ,  /*  FLASH   */ \
                                    NOP ,  /*  SPIDMA  */ \
                                    NOP    /*   RBUS   */ \
                                  ) == 0xF7 );

TTX_ASSERT_COMPILE( SYS_SRST0_RMKS(RESET,  /*   HDMI   */ \
                                   NOP ,  /*   SLCR   */ \
                                   RESET,  /*   IFD    */ \
                                   NOP ,  /*   VDEC   */ \
                                   RESET,  /*  FLASH   */ \
                                   NOP ,  /*  SPIDMA  */ \
                                   RESET   /*   RBUS   */ \
                                  ) == 0x52 );

TTX_ASSERT_COMPILE( SYS_SRST0_RMKS( NOP ,  /*   HDMI   */ \
                                    RESET,  /*   SLCR   */ \
                                    NOP ,  /*   IFD    */ \
                                    RESET,  /*   VDEC   */ \
                                    NOP ,  /*  FLASH   */ \
                                    RESET,  /*  SPIDMA  */ \
                                    NOP    /*   RBUS   */ \
                                  ) == 0xA5 );

TTX_ASSERT_COMPILE( SYS_SRST0_RMKS( NOP ,  /*   HDMI   */ \
                                    RESET,  /*   SLCR   */ \
                                    RESET,  /*   IFD    */ \
                                    RESET,  /*   VDEC   */ \
                                    RESET,  /*  FLASH   */ \
                                    RESET,  /*  SPIDMA  */ \
                                    RESET   /*   RBUS   */ \
                                  ) == 0x80 );

TTX_ASSERT_COMPILE( SYS_SRST0_RMKS(RESET,  /*   HDMI   */ \
                                   NOP ,  /*   SLCR   */ \
                                   RESET,  /*   IFD    */ \
                                   RESET,  /*   VDEC   */ \
                                   RESET,  /*  FLASH   */ \
                                   RESET,  /*  SPIDMA  */ \
                                   RESET   /*   RBUS   */ \
                                  ) == 0x40 );

TTX_ASSERT_COMPILE( SYS_SRST0_RMKS(RESET,  /*   HDMI   */ \
                                   RESET,  /*   SLCR   */ \
                                   NOP ,  /*   IFD    */ \
                                   RESET,  /*   VDEC   */ \
                                   RESET,  /*  FLASH   */ \
                                   RESET,  /*  SPIDMA  */ \
                                   RESET   /*   RBUS   */ \
                                  ) == 0x20 );

TTX_ASSERT_COMPILE( SYS_SRST0_RMKS(RESET,  /*   HDMI   */ \
                                   RESET,  /*   SLCR   */ \
                                   RESET,  /*   IFD    */ \
                                   NOP,  /*   VDEC   */ \
                                   RESET,  /*  FLASH   */ \
                                   RESET,  /*  SPIDMA  */ \
                                   RESET   /*   RBUS   */ \
                                  ) == 0x10 );

TTX_ASSERT_COMPILE( SYS_SRST0_RMKS(RESET,  /*   HDMI   */ \
                                   RESET,  /*   SLCR   */ \
                                   RESET,  /*   IFD    */ \
                                   RESET,  /*   VDEC   */ \
                                   NOP ,  /*  FLASH   */ \
                                   RESET,  /*  SPIDMA  */ \
                                   RESET   /*   RBUS   */ \
                                  ) == 0x04 );

TTX_ASSERT_COMPILE( SYS_SRST0_RMKS(RESET,  /*   HDMI   */ \
                                   RESET,  /*   SLCR   */ \
                                   RESET,  /*   IFD    */ \
                                   RESET,  /*   VDEC   */ \
                                   RESET,  /*  FLASH   */ \
                                   NOP ,  /*  SPIDMA  */ \
                                   RESET   /*   RBUS   */ \
                                  ) == 0x02 );

TTX_ASSERT_COMPILE( SYS_SRST0_RMKS(RESET,  /*   HDMI   */ \
                                   RESET,  /*   SLCR   */ \
                                   RESET,  /*   IFD    */ \
                                   RESET,  /*   VDEC   */ \
                                   RESET,  /*  FLASH   */ \
                                   RESET,  /*  SPIDMA  */ \
                                   NOP    /*   RBUS   */ \
                                  ) == 0x01 );

TTX_ASSERT_COMPILE( SYS_SRST0_RMKS(RESET,  /*   HDMI   */ \
                                   NOP ,  /*   SLCR   */ \
                                   NOP ,  /*   IFD    */ \
                                   RESET,  /*   VDEC   */ \
                                   RESET,  /*  FLASH   */ \
                                   NOP ,  /*  SPIDMA  */ \
                                   NOP    /*   RBUS   */ \
                                  ) == 0x63 );



/* Test SYS_SRST1_REG */
TTX_ASSERT_COMPILE( SYS_SRST1_DEFAULT        == 0x00 );

TTX_ASSERT_COMPILE( SYS_SRST1_RMKS(RESET,  /*  DISPD  */ \
                                   RESET,  /*  DISPI  */ \
                                   RESET,  /*  ADAC2  */ \
                                   RESET,  /*  ADAC1  */ \
                                   RESET,  /*  AADC   */ \
                                   RESET   /*   AUD   */ \
                                  ) == 0x00 );

TTX_ASSERT_COMPILE( SYS_SRST1_RMKS( NOP ,  /*  DISPD  */ \
                                    NOP ,  /*  DISPI  */ \
                                    NOP ,  /*  ADAC2  */ \
                                    NOP ,  /*  ADAC1  */ \
                                    NOP ,  /*  AADC   */ \
                                    NOP    /*   AUD   */ \
                                  ) == 0x3F );

TTX_ASSERT_COMPILE( SYS_SRST1_RMKS( NOP ,  /*  DISPD  */ \
                                    RESET,  /*  DISPI  */ \
                                    RESET,  /*  ADAC2  */ \
                                    RESET,  /*  ADAC1  */ \
                                    RESET,  /*  AADC   */ \
                                    RESET   /*   AUD   */ \
                                  ) == 0x20 );

TTX_ASSERT_COMPILE( SYS_SRST1_RMKS(RESET,  /*  DISPD  */ \
                                   NOP ,  /*  DISPI  */ \
                                   RESET,  /*  ADAC2  */ \
                                   RESET,  /*  ADAC1  */ \
                                   RESET,  /*  AADC   */ \
                                   RESET   /*   AUD   */ \
                                  ) == 0x10 );

TTX_ASSERT_COMPILE( SYS_SRST1_RMKS(RESET,  /*  DISPD  */ \
                                   RESET,  /*  DISPI  */ \
                                   NOP ,  /*  ADAC2  */ \
                                   RESET,  /*  ADAC1  */ \
                                   RESET,  /*  AADC   */ \
                                   RESET   /*   AUD   */ \
                                  ) == 0x08 );

TTX_ASSERT_COMPILE( SYS_SRST1_RMKS(RESET,  /*  DISPD  */ \
                                   RESET,  /*  DISPI  */ \
                                   RESET,  /*  ADAC2  */ \
                                   NOP ,  /*  ADAC1  */ \
                                   RESET,  /*  AADC   */ \
                                   RESET   /*   AUD   */ \
                                  ) == 0x04 );

TTX_ASSERT_COMPILE( SYS_SRST1_RMKS(RESET,  /*  DISPD  */ \
                                   RESET,  /*  DISPI  */ \
                                   RESET,  /*  ADAC2  */ \
                                   RESET,  /*  ADAC1  */ \
                                   NOP ,  /*  AADC   */ \
                                   RESET   /*   AUD   */ \
                                  ) == 0x02 );

TTX_ASSERT_COMPILE( SYS_SRST1_RMKS(RESET,  /*  DISPD  */ \
                                   RESET,  /*  DISPI  */ \
                                   RESET,  /*  ADAC2  */ \
                                   RESET,  /*  ADAC1  */ \
                                   RESET,  /*  AADC   */ \
                                   NOP    /*   AUD   */ \
                                  ) == 0x01 );

TTX_ASSERT_COMPILE( SYS_SRST1_RMKS(RESET,  /*  DISPD  */ \
                                   NOP ,  /*  DISPI  */ \
                                   RESET,  /*  ADAC2  */ \
                                   NOP ,  /*  ADAC1  */ \
                                   RESET,  /*  AADC   */ \
                                   NOP    /*   AUD   */ \
                                  ) == 0x15 );

TTX_ASSERT_COMPILE( SYS_SRST1_RMKS( NOP ,  /*  DISPD  */ \
                                    RESET,  /*  DISPI  */ \
                                    NOP ,  /*  ADAC2  */ \
                                    RESET,  /*  ADAC1  */ \
                                    NOP ,  /*  AADC   */ \
                                    RESET   /*   AUD   */ \
                                  ) == 0x2A );


/* Test SYS_SRST2_REG */
TTX_ASSERT_COMPILE( SYS_SRST2_DEFAULT        == 0x00 );

TTX_ASSERT_COMPILE( SYS_SRST2_RMKS(RESET,  /*   OSD    */ \
                                   RESET,  /*   VTE    */ \
                                   RESET,  /*   PPR    */ \
                                   RESET,  /*  VBIBUS  */ \
                                   RESET,  /* SM_UI12K */ \
                                   RESET,  /* SM_UI8K  */ \
                                   RESET   /* SM_TT13K */ \
                                  ) == 0x00 );

TTX_ASSERT_COMPILE( SYS_SRST2_RMKS( NOP ,  /*   OSD    */ \
                                    NOP ,  /*   VTE    */ \
                                    NOP ,  /*   PPR    */ \
                                    NOP ,  /*  VBIBUS  */ \
                                    NOP ,  /* SM_UI12K */ \
                                    NOP ,  /* SM_UI8K  */ \
                                    NOP    /* SM_TT13K */ \
                                  ) == 0xF7 );

TTX_ASSERT_COMPILE( SYS_SRST2_RMKS(RESET,  /*   OSD    */ \
                                   NOP ,  /*   VTE    */ \
                                   RESET,  /*   PPR    */ \
                                   NOP ,  /*  VBIBUS  */ \
                                   RESET,  /* SM_UI12K */ \
                                   NOP ,  /* SM_UI8K  */ \
                                   RESET   /* SM_TT13K */ \
                                  ) == 0x52 );

TTX_ASSERT_COMPILE( SYS_SRST2_RMKS( NOP ,  /*   OSD    */ \
                                    RESET,  /*   VTE    */ \
                                    NOP ,  /*   PPR    */ \
                                    RESET,  /*  VBIBUS  */ \
                                    NOP ,  /* SM_UI12K */ \
                                    RESET,  /* SM_UI8K  */ \
                                    NOP    /* SM_TT13K */ \
                                  ) == 0xA5 );

TTX_ASSERT_COMPILE( SYS_SRST2_RMKS( NOP ,  /*   OSD    */ \
                                    RESET,  /*   VTE    */ \
                                    RESET,  /*   PPR    */ \
                                    RESET,  /*  VBIBUS  */ \
                                    RESET,  /* SM_UI12K */ \
                                    RESET,  /* SM_UI8K  */ \
                                    RESET   /* SM_TT13K */ \
                                  ) == 0x80 );

TTX_ASSERT_COMPILE( SYS_SRST2_RMKS(RESET,  /*   OSD    */ \
                                   NOP ,  /*   VTE    */ \
                                   RESET,  /*   PPR    */ \
                                   RESET,  /*  VBIBUS  */ \
                                   RESET,  /* SM_UI12K */ \
                                   RESET,  /* SM_UI8K  */ \
                                   RESET   /* SM_TT13K */ \
                                  ) == 0x40 );

TTX_ASSERT_COMPILE( SYS_SRST2_RMKS(RESET,  /*   OSD    */ \
                                   NOP ,  /*   VTE    */ \
                                   RESET,  /*   PPR    */ \
                                   RESET,  /*  VBIBUS  */ \
                                   RESET,  /* SM_UI12K */ \
                                   RESET,  /* SM_UI8K  */ \
                                   RESET   /* SM_TT13K */ \
                                  ) == 0x40 );

TTX_ASSERT_COMPILE( SYS_SRST2_RMKS(RESET,  /*   OSD    */ \
                                   RESET,  /*   VTE    */ \
                                   NOP ,  /*   PPR    */ \
                                   RESET,  /*  VBIBUS  */ \
                                   RESET,  /* SM_UI12K */ \
                                   RESET,  /* SM_UI8K  */ \
                                   RESET   /* SM_TT13K */ \
                                  ) == 0x20 );

TTX_ASSERT_COMPILE( SYS_SRST2_RMKS(RESET,  /*   OSD    */ \
                                   RESET,  /*   VTE    */ \
                                   RESET,  /*   PPR    */ \
                                   NOP ,  /*  VBIBUS  */ \
                                   RESET,  /* SM_UI12K */ \
                                   RESET,  /* SM_UI8K  */ \
                                   RESET   /* SM_TT13K */ \
                                  ) == 0x10 );

TTX_ASSERT_COMPILE( SYS_SRST2_RMKS(RESET,  /*   OSD    */ \
                                   RESET,  /*   VTE    */ \
                                   RESET,  /*   PPR    */ \
                                   RESET,  /*  VBIBUS  */ \
                                   NOP ,  /* SM_UI12K */ \
                                   RESET,  /* SM_UI8K  */ \
                                   RESET   /* SM_TT13K */ \
                                  ) == 0x04 );

TTX_ASSERT_COMPILE( SYS_SRST2_RMKS(RESET,  /*   OSD    */ \
                                   RESET,  /*   VTE    */ \
                                   RESET,  /*   PPR    */ \
                                   RESET,  /*  VBIBUS  */ \
                                   RESET,  /* SM_UI12K */ \
                                   NOP ,  /* SM_UI8K  */ \
                                   RESET   /* SM_TT13K */ \
                                  ) == 0x02 );

TTX_ASSERT_COMPILE( SYS_SRST2_RMKS(RESET,  /*   OSD    */ \
                                   RESET,  /*   VTE    */ \
                                   RESET,  /*   PPR    */ \
                                   RESET,  /*  VBIBUS  */ \
                                   RESET,  /* SM_UI12K */ \
                                   RESET,  /* SM_UI8K  */ \
                                   NOP    /* SM_TT13K */ \
                                  ) == 0x01 );


/* Test SYS_SRST3_REG */
TTX_ASSERT_COMPILE( SYS_SRST3_DEFAULT        == 0x00 );

TTX_ASSERT_COMPILE( SYS_SRST3_RMKS(RESET,  /*  PWM   */ \
                                   RESET,  /*  WDT   */ \
                                   RESET,  /*  TMR2  */ \
                                   RESET,  /*  TMR1  */ \
                                   RESET,  /*  TMR0  */ \
                                   RESET,  /*  UART1 */ \
                                   RESET,  /*  UART0 */ \
                                   RESET   /*  LSADC */ \
                                  ) == 0x00 );

TTX_ASSERT_COMPILE( SYS_SRST3_RMKS( NOP ,  /*  PWM   */ \
                                    NOP ,  /*  WDT   */ \
                                    NOP ,  /*  TMR2  */ \
                                    NOP ,  /*  TMR1  */ \
                                    NOP ,  /*  TMR0  */ \
                                    NOP ,  /*  UART1 */ \
                                    NOP ,  /*  UART0 */ \
                                    NOP    /*  LSADC */ \
                                  ) == 0xFF );

TTX_ASSERT_COMPILE( SYS_SRST3_RMKS( NOP ,  /*  PWM   */ \
                                    RESET,  /*  WDT   */ \
                                    NOP ,  /*  TMR2  */ \
                                    RESET,  /*  TMR1  */ \
                                    NOP ,  /*  TMR0  */ \
                                    RESET,  /*  UART1 */ \
                                    NOP ,  /*  UART0 */ \
                                    RESET   /*  LSADC */ \
                                  ) == 0xAA );

TTX_ASSERT_COMPILE( SYS_SRST3_RMKS(RESET,  /*  PWM   */ \
                                   NOP ,  /*  WDT   */ \
                                   RESET,  /*  TMR2  */ \
                                   NOP ,  /*  TMR1  */ \
                                   RESET,  /*  TMR0  */ \
                                   NOP ,  /*  UART1 */ \
                                   RESET,  /*  UART0 */ \
                                   NOP    /*  LSADC */ \
                                  ) == 0x55 );

TTX_ASSERT_COMPILE( SYS_SRST3_RMKS( NOP ,  /*  PWM   */ \
                                    RESET,  /*  WDT   */ \
                                    NOP ,  /*  TMR2  */ \
                                    RESET,  /*  TMR1  */ \
                                    RESET,  /*  TMR0  */ \
                                    NOP ,  /*  UART1 */ \
                                    RESET,  /*  UART0 */ \
                                    NOP    /*  LSADC */ \
                                  ) == 0xA5 );

TTX_ASSERT_COMPILE( SYS_SRST3_RMKS(RESET,  /*  PWM   */ \
                                   NOP ,  /*  WDT   */ \
                                   RESET,  /*  TMR2  */ \
                                   NOP ,  /*  TMR1  */ \
                                   NOP ,  /*  TMR0  */ \
                                   RESET,  /*  UART1 */ \
                                   NOP ,  /*  UART0 */ \
                                   RESET   /*  LSADC */ \
                                  ) == 0x5A );

TTX_ASSERT_COMPILE( SYS_SRST3_RMKS( NOP ,  /*  PWM   */ \
                                    RESET,  /*  WDT   */ \
                                    RESET,  /*  TMR2  */ \
                                    RESET,  /*  TMR1  */ \
                                    RESET,  /*  TMR0  */ \
                                    RESET,  /*  UART1 */ \
                                    RESET,  /*  UART0 */ \
                                    RESET   /*  LSADC */ \
                                  ) == 0x80 );

TTX_ASSERT_COMPILE( SYS_SRST3_RMKS(RESET,  /*  PWM   */ \
                                   NOP ,  /*  WDT   */ \
                                   RESET,  /*  TMR2  */ \
                                   RESET,  /*  TMR1  */ \
                                   RESET,  /*  TMR0  */ \
                                   RESET,  /*  UART1 */ \
                                   RESET,  /*  UART0 */ \
                                   RESET   /*  LSADC */ \
                                  ) == 0x40 );

TTX_ASSERT_COMPILE( SYS_SRST3_RMKS(RESET,  /*  PWM   */ \
                                   RESET,  /*  WDT   */ \
                                   NOP ,  /*  TMR2  */ \
                                   RESET,  /*  TMR1  */ \
                                   RESET,  /*  TMR0  */ \
                                   RESET,  /*  UART1 */ \
                                   RESET,  /*  UART0 */ \
                                   RESET   /*  LSADC */ \
                                  ) == 0x20 );

TTX_ASSERT_COMPILE( SYS_SRST3_RMKS(RESET,  /*  PWM   */ \
                                   RESET,  /*  WDT   */ \
                                   RESET,  /*  TMR2  */ \
                                   NOP ,  /*  TMR1  */ \
                                   RESET,  /*  TMR0  */ \
                                   RESET,  /*  UART1 */ \
                                   RESET,  /*  UART0 */ \
                                   RESET   /*  LSADC */ \
                                  ) == 0x10 );

TTX_ASSERT_COMPILE( SYS_SRST3_RMKS(RESET,  /*  PWM   */ \
                                   RESET,  /*  WDT   */ \
                                   RESET,  /*  TMR2  */ \
                                   RESET,  /*  TMR1  */ \
                                   NOP ,  /*  TMR0  */ \
                                   RESET,  /*  UART1 */ \
                                   RESET,  /*  UART0 */ \
                                   RESET   /*  LSADC */ \
                                  ) == 0x08 );

TTX_ASSERT_COMPILE( SYS_SRST3_RMKS(RESET,  /*  PWM   */ \
                                   RESET,  /*  WDT   */ \
                                   RESET,  /*  TMR2  */ \
                                   RESET,  /*  TMR1  */ \
                                   RESET,  /*  TMR0  */ \
                                   NOP ,  /*  UART1 */ \
                                   RESET,  /*  UART0 */ \
                                   RESET   /*  LSADC */ \
                                  ) == 0x04 );

TTX_ASSERT_COMPILE( SYS_SRST3_RMKS(RESET,  /*  PWM   */ \
                                   RESET,  /*  WDT   */ \
                                   RESET,  /*  TMR2  */ \
                                   RESET,  /*  TMR1  */ \
                                   RESET,  /*  TMR0  */ \
                                   RESET,  /*  UART1 */ \
                                   NOP ,  /*  UART0 */ \
                                   RESET   /*  LSADC */ \
                                  ) == 0x02 );

TTX_ASSERT_COMPILE( SYS_SRST3_RMKS(RESET,  /*  PWM   */ \
                                   RESET,  /*  WDT   */ \
                                   RESET,  /*  TMR2  */ \
                                   RESET,  /*  TMR1  */ \
                                   RESET,  /*  TMR0  */ \
                                   RESET,  /*  UART1 */ \
                                   RESET,  /*  UART0 */ \
                                   NOP    /*  LSADC */ \
                                  ) == 0x01 );

/* Test SYS_SRST4_REG */
TTX_ASSERT_COMPILE( SYS_SRST4_DEFAULT        == 0x00 );

TTX_ASSERT_COMPILE( SYS_SRST4_RMKS(RESET,  /*  GPIO  */ \
                                   RESET,  /*  IRDA  */ \
                                   RESET,  /*  I2C2  */ \
                                   RESET,  /*  I2C1  */ \
                                   RESET   /*  I2C0  */ \
                                  ) == 0x00 );

TTX_ASSERT_COMPILE( SYS_SRST4_RMKS( NOP ,  /*  GPIO  */ \
                                    NOP ,  /*  IRDA  */ \
                                    NOP ,  /*  I2C2  */ \
                                    NOP ,  /*  I2C1  */ \
                                    NOP    /*  I2C0  */ \
                                  ) == 0x1F );

TTX_ASSERT_COMPILE( SYS_SRST4_RMKS( NOP ,  /*  GPIO  */ \
                                    RESET,  /*  IRDA  */ \
                                    NOP ,  /*  I2C2  */ \
                                    RESET,  /*  I2C1  */ \
                                    NOP    /*  I2C0  */ \
                                  ) == 0x15 );

TTX_ASSERT_COMPILE( SYS_SRST4_RMKS(RESET,  /*  GPIO  */ \
                                   NOP ,  /*  IRDA  */ \
                                   RESET,  /*  I2C2  */ \
                                   NOP ,  /*  I2C1  */ \
                                   RESET   /*  I2C0  */ \
                                  ) == 0x0A );

TTX_ASSERT_COMPILE( SYS_SRST4_RMKS( NOP ,  /*  GPIO  */ \
                                    RESET,  /*  IRDA  */ \
                                    RESET,  /*  I2C2  */ \
                                    RESET,  /*  I2C1  */ \
                                    RESET   /*  I2C0  */ \
                                  ) == 0x10 );

TTX_ASSERT_COMPILE( SYS_SRST4_RMKS(RESET,  /*  GPIO  */ \
                                   NOP ,  /*  IRDA  */ \
                                   RESET,  /*  I2C2  */ \
                                   RESET,  /*  I2C1  */ \
                                   RESET   /*  I2C0  */ \
                                  ) == 0x08 );

TTX_ASSERT_COMPILE( SYS_SRST4_RMKS(RESET,  /*  GPIO  */ \
                                   RESET,  /*  IRDA  */ \
                                   NOP ,  /*  I2C2  */ \
                                   RESET,  /*  I2C1  */ \
                                   RESET   /*  I2C0  */ \
                                  ) == 0x04 );

TTX_ASSERT_COMPILE( SYS_SRST4_RMKS(RESET,  /*  GPIO  */ \
                                   RESET,  /*  IRDA  */ \
                                   RESET,  /*  I2C2  */ \
                                   NOP ,  /*  I2C1  */ \
                                   RESET   /*  I2C0  */ \
                                  ) == 0x02 );

TTX_ASSERT_COMPILE( SYS_SRST4_RMKS(RESET,  /*  GPIO  */ \
                                   RESET,  /*  IRDA  */ \
                                   RESET,  /*  I2C2  */ \
                                   RESET,  /*  I2C1  */ \
                                   NOP    /*  I2C0  */ \
                                  ) == 0x01 );

TTX_ASSERT_COMPILE( SYS_SRST4_RMKS( NOP ,  /*  GPIO  */ \
                                    NOP ,  /*  IRDA  */ \
                                    RESET,  /*  I2C2  */ \
                                    RESET,  /*  I2C1  */ \
                                    RESET   /*  I2C0  */ \
                                  ) == 0x18 );


/* Test SYS_CLKEN0_REG */
TTX_ASSERT_COMPILE( SYS_CLKEN0_DEFAULT        == 0x00 );

TTX_ASSERT_COMPILE( SYS_CLKEN0_RMKS(DISABLE,  /* LSADC */ \
                                    DISABLE,  /*  TMR  */ \
                                    DISABLE,  /*  I2CS */ \
                                    DISABLE,  /*  DDC1 */ \
                                    DISABLE,  /*  DDC0 */ \
                                    DISABLE,  /*  HDMI */ \
                                    DISABLE,  /*  VDEC */ \
                                    DISABLE   /*  VBI  */ \
                                   ) == 0x00 );

TTX_ASSERT_COMPILE( SYS_CLKEN0_RMKS(ENABLE ,  /* LSADC */ \
                                    ENABLE ,  /*  TMR  */ \
                                    ENABLE ,  /*  I2CS */ \
                                    ENABLE ,  /*  DDC1 */ \
                                    ENABLE ,  /*  DDC0 */ \
                                    ENABLE ,  /*  HDMI */ \
                                    ENABLE ,  /*  VDEC */ \
                                    ENABLE    /*  VBI  */ \
                                   ) == 0xFF );

TTX_ASSERT_COMPILE( SYS_CLKEN0_RMKS(ENABLE ,  /* LSADC */ \
                                    DISABLE,  /*  TMR  */ \
                                    ENABLE ,  /*  I2CS */ \
                                    DISABLE,  /*  DDC1 */ \
                                    DISABLE,  /*  DDC0 */ \
                                    ENABLE ,  /*  HDMI */ \
                                    DISABLE,  /*  VDEC */ \
                                    ENABLE    /*  VBI  */ \
                                   ) == 0xA5 );

TTX_ASSERT_COMPILE( SYS_CLKEN0_RMKS(DISABLE,  /* LSADC */ \
                                    ENABLE ,  /*  TMR  */ \
                                    DISABLE,  /*  I2CS */ \
                                    ENABLE ,  /*  DDC1 */ \
                                    ENABLE ,  /*  DDC0 */ \
                                    DISABLE,  /*  HDMI */ \
                                    ENABLE ,  /*  VDEC */ \
                                    DISABLE   /*  VBI  */ \
                                   ) == 0x5A );

TTX_ASSERT_COMPILE( SYS_CLKEN0_RMKS(ENABLE ,  /* LSADC */ \
                                    DISABLE,  /*  TMR  */ \
                                    DISABLE,  /*  I2CS */ \
                                    DISABLE,  /*  DDC1 */ \
                                    DISABLE,  /*  DDC0 */ \
                                    DISABLE,  /*  HDMI */ \
                                    DISABLE,  /*  VDEC */ \
                                    DISABLE   /*  VBI  */ \
                                   ) == 0x80 );

TTX_ASSERT_COMPILE( SYS_CLKEN0_RMKS(DISABLE,  /* LSADC */ \
                                    ENABLE ,  /*  TMR  */ \
                                    DISABLE,  /*  I2CS */ \
                                    DISABLE,  /*  DDC1 */ \
                                    DISABLE,  /*  DDC0 */ \
                                    DISABLE,  /*  HDMI */ \
                                    DISABLE,  /*  VDEC */ \
                                    DISABLE   /*  VBI  */ \
                                   ) == 0x40 );

TTX_ASSERT_COMPILE( SYS_CLKEN0_RMKS(DISABLE,  /* LSADC */ \
                                    DISABLE,  /*  TMR  */ \
                                    ENABLE ,  /*  I2CS */ \
                                    DISABLE,  /*  DDC1 */ \
                                    DISABLE,  /*  DDC0 */ \
                                    DISABLE,  /*  HDMI */ \
                                    DISABLE,  /*  VDEC */ \
                                    DISABLE   /*  VBI  */ \
                                   ) == 0x20 );

TTX_ASSERT_COMPILE( SYS_CLKEN0_RMKS(DISABLE,  /* LSADC */ \
                                    DISABLE,  /*  TMR  */ \
                                    DISABLE,  /*  I2CS */ \
                                    ENABLE ,  /*  DDC1 */ \
                                    DISABLE,  /*  DDC0 */ \
                                    DISABLE,  /*  HDMI */ \
                                    DISABLE,  /*  VDEC */ \
                                    DISABLE   /*  VBI  */ \
                                   ) == 0x10 );

TTX_ASSERT_COMPILE( SYS_CLKEN0_RMKS(DISABLE,  /* LSADC */ \
                                    DISABLE,  /*  TMR  */ \
                                    DISABLE,  /*  I2CS */ \
                                    DISABLE,  /*  DDC1 */ \
                                    ENABLE ,  /*  DDC0 */ \
                                    DISABLE,  /*  HDMI */ \
                                    DISABLE,  /*  VDEC */ \
                                    DISABLE   /*  VBI  */ \
                                   ) == 0x08 );

TTX_ASSERT_COMPILE( SYS_CLKEN0_RMKS(DISABLE,  /* LSADC */ \
                                    DISABLE,  /*  TMR  */ \
                                    DISABLE,  /*  I2CS */ \
                                    DISABLE,  /*  DDC1 */ \
                                    DISABLE,  /*  DDC0 */ \
                                    ENABLE ,  /*  HDMI */ \
                                    DISABLE,  /*  VDEC */ \
                                    DISABLE   /*  VBI  */ \
                                   ) == 0x04 );

TTX_ASSERT_COMPILE( SYS_CLKEN0_RMKS(DISABLE,  /* LSADC */ \
                                    DISABLE,  /*  TMR  */ \
                                    DISABLE,  /*  I2CS */ \
                                    DISABLE,  /*  DDC1 */ \
                                    DISABLE,  /*  DDC0 */ \
                                    DISABLE,  /*  HDMI */ \
                                    ENABLE ,  /*  VDEC */ \
                                    DISABLE   /*  VBI  */ \
                                   ) == 0x02 );

TTX_ASSERT_COMPILE( SYS_CLKEN0_RMKS(DISABLE,  /* LSADC */ \
                                    DISABLE,  /*  TMR  */ \
                                    DISABLE,  /*  I2CS */ \
                                    DISABLE,  /*  DDC1 */ \
                                    DISABLE,  /*  DDC0 */ \
                                    DISABLE,  /*  HDMI */ \
                                    DISABLE,  /*  VDEC */ \
                                    ENABLE    /*  VBI  */ \
                                   ) == 0x01 );


/* Test SYS_CLKEN1_REG */
TTX_ASSERT_COMPILE( SYS_CLKEN1_DEFAULT        == 0x00 );

TTX_ASSERT_COMPILE( SYS_CLKEN1_RMKS(DISABLE,  /*  I2CM1  */ \
                                    DISABLE,  /*  I2CM0  */ \
                                    DISABLE,  /*   SPI   */ \
                                    DISABLE,  /*  DISPM  */ \
                                    DISABLE,  /* UART98M */ \
                                    DISABLE,  /* IFD49M  */ \
                                    DISABLE,  /* AUD49M  */ \
                                    DISABLE   /* AUD98M  */ \
                                   ) == 0x00 );

TTX_ASSERT_COMPILE( SYS_CLKEN1_RMKS(ENABLE ,  /*  I2CM1  */ \
                                    ENABLE ,  /*  I2CM0  */ \
                                    ENABLE ,  /*   SPI   */ \
                                    ENABLE ,  /*  DISPM  */ \
                                    ENABLE ,  /* UART98M */ \
                                    ENABLE ,  /* IFD49M  */ \
                                    ENABLE ,  /* AUD49M  */ \
                                    ENABLE    /* AUD98M  */ \
                                   ) == 0xFF );

TTX_ASSERT_COMPILE( SYS_CLKEN1_RMKS(ENABLE ,  /*  I2CM1  */ \
                                    DISABLE,  /*  I2CM0  */ \
                                    ENABLE ,  /*   SPI   */ \
                                    DISABLE,  /*  DISPM  */ \
                                    DISABLE,  /* UART98M */ \
                                    ENABLE ,  /* IFD49M  */ \
                                    DISABLE,  /* AUD49M  */ \
                                    ENABLE    /* AUD98M  */ \
                                   ) == 0xA5 );

TTX_ASSERT_COMPILE( SYS_CLKEN1_RMKS(DISABLE,  /*  I2CM1  */ \
                                    ENABLE ,  /*  I2CM0  */ \
                                    DISABLE,  /*   SPI   */ \
                                    ENABLE ,  /*  DISPM  */ \
                                    ENABLE ,  /* UART98M */ \
                                    DISABLE,  /* IFD49M  */ \
                                    ENABLE ,  /* AUD49M  */ \
                                    DISABLE   /* AUD98M  */ \
                                   ) == 0x5A );

TTX_ASSERT_COMPILE( SYS_CLKEN1_RMKS(ENABLE ,  /*  I2CM1  */ \
                                    DISABLE,  /*  I2CM0  */ \
                                    DISABLE,  /*   SPI   */ \
                                    DISABLE,  /*  DISPM  */ \
                                    DISABLE,  /* UART98M */ \
                                    DISABLE,  /* IFD49M  */ \
                                    DISABLE,  /* AUD49M  */ \
                                    DISABLE   /* AUD98M  */ \
                                   ) == 0x80 );

TTX_ASSERT_COMPILE( SYS_CLKEN1_RMKS(DISABLE,  /*  I2CM1  */ \
                                    ENABLE ,  /*  I2CM0  */ \
                                    DISABLE,  /*   SPI   */ \
                                    DISABLE,  /*  DISPM  */ \
                                    DISABLE,  /* UART98M */ \
                                    DISABLE,  /* IFD49M  */ \
                                    DISABLE,  /* AUD49M  */ \
                                    DISABLE   /* AUD98M  */ \
                                   ) == 0x40 );

TTX_ASSERT_COMPILE( SYS_CLKEN1_RMKS(DISABLE,  /*  I2CM1  */ \
                                    DISABLE,  /*  I2CM0  */ \
                                    ENABLE ,  /*   SPI   */ \
                                    DISABLE,  /*  DISPM  */ \
                                    DISABLE,  /* UART98M */ \
                                    DISABLE,  /* IFD49M  */ \
                                    DISABLE,  /* AUD49M  */ \
                                    DISABLE   /* AUD98M  */ \
                                   ) == 0x20 );

TTX_ASSERT_COMPILE( SYS_CLKEN1_RMKS(DISABLE,  /*  I2CM1  */ \
                                    DISABLE,  /*  I2CM0  */ \
                                    DISABLE,  /*   SPI   */ \
                                    ENABLE ,  /*  DISPM  */ \
                                    DISABLE,  /* UART98M */ \
                                    DISABLE,  /* IFD49M  */ \
                                    DISABLE,  /* AUD49M  */ \
                                    DISABLE   /* AUD98M  */ \
                                   ) == 0x10 );

TTX_ASSERT_COMPILE( SYS_CLKEN1_RMKS(DISABLE,  /*  I2CM1  */ \
                                    DISABLE,  /*  I2CM0  */ \
                                    DISABLE,  /*   SPI   */ \
                                    DISABLE,  /*  DISPM  */ \
                                    ENABLE ,  /* UART98M */ \
                                    DISABLE,  /* IFD49M  */ \
                                    DISABLE,  /* AUD49M  */ \
                                    DISABLE   /* AUD98M  */ \
                                   ) == 0x08 );

TTX_ASSERT_COMPILE( SYS_CLKEN1_RMKS(DISABLE,  /*  I2CM1  */ \
                                    DISABLE,  /*  I2CM0  */ \
                                    DISABLE,  /*   SPI   */ \
                                    DISABLE,  /*  DISPM  */ \
                                    DISABLE,  /* UART98M */ \
                                    ENABLE ,  /* IFD49M  */ \
                                    DISABLE,  /* AUD49M  */ \
                                    DISABLE   /* AUD98M  */ \
                                   ) == 0x04 );

TTX_ASSERT_COMPILE( SYS_CLKEN1_RMKS(DISABLE,  /*  I2CM1  */ \
                                    DISABLE,  /*  I2CM0  */ \
                                    DISABLE,  /*   SPI   */ \
                                    DISABLE,  /*  DISPM  */ \
                                    DISABLE,  /* UART98M */ \
                                    DISABLE,  /* IFD49M  */ \
                                    ENABLE ,  /* AUD49M  */ \
                                    DISABLE   /* AUD98M  */ \
                                   ) == 0x02 );

TTX_ASSERT_COMPILE( SYS_CLKEN1_RMKS(DISABLE,  /*  I2CM1  */ \
                                    DISABLE,  /*  I2CM0  */ \
                                    DISABLE,  /*   SPI   */ \
                                    DISABLE,  /*  DISPM  */ \
                                    DISABLE,  /* UART98M */ \
                                    DISABLE,  /* IFD49M  */ \
                                    DISABLE,  /* AUD49M  */ \
                                    ENABLE    /* AUD98M  */ \
                                   ) == 0x01 );

/* Test SYS_CLKEN2_REG */
TTX_ASSERT_COMPILE( SYS_CLKEN2_DEFAULT        == 0x00 );

TTX_ASSERT_COMPILE( SYS_CLKEN2_RMKS(DISABLE,  /* PWM_XTAL */ \
                                    DISABLE,  /*  IR_XTAL */ \
                                    DISABLE,  /* UR1_XTAL */ \
                                    DISABLE,  /* UR0_XTAL */ \
                                    DISABLE,  /*  AUD16M  */ \
                                    DISABLE,  /*  AUD24M  */ \
                                    DISABLE,  /*  DISPD   */ \
                                    DISABLE   /*  DISPI   */ \
                                   ) == 0x00 );

TTX_ASSERT_COMPILE( SYS_CLKEN2_RMKS(ENABLE ,  /* PWM_XTAL */ \
                                    ENABLE ,  /*  IR_XTAL */ \
                                    ENABLE ,  /* UR1_XTAL */ \
                                    ENABLE ,  /* UR0_XTAL */ \
                                    ENABLE ,  /*  AUD16M  */ \
                                    ENABLE ,  /*  AUD24M  */ \
                                    ENABLE ,  /*  DISPD   */ \
                                    ENABLE    /*  DISPI   */ \
                                   ) == 0xFF );

TTX_ASSERT_COMPILE( SYS_CLKEN2_RMKS(ENABLE ,  /* PWM_XTAL */ \
                                    DISABLE,  /*  IR_XTAL */ \
                                    ENABLE ,  /* UR1_XTAL */ \
                                    DISABLE,  /* UR0_XTAL */ \
                                    DISABLE,  /*  AUD16M  */ \
                                    ENABLE ,  /*  AUD24M  */ \
                                    DISABLE,  /*  DISPD   */ \
                                    ENABLE    /*  DISPI   */ \
                                   ) == 0xA5 );

TTX_ASSERT_COMPILE( SYS_CLKEN2_RMKS(DISABLE,  /* PWM_XTAL */ \
                                    ENABLE ,  /*  IR_XTAL */ \
                                    DISABLE,  /* UR1_XTAL */ \
                                    ENABLE ,  /* UR0_XTAL */ \
                                    ENABLE ,  /*  AUD16M  */ \
                                    DISABLE,  /*  AUD24M  */ \
                                    ENABLE ,  /*  DISPD   */ \
                                    DISABLE   /*  DISPI   */ \
                                   ) == 0x5A );

TTX_ASSERT_COMPILE( SYS_CLKEN2_RMKS(ENABLE ,  /* PWM_XTAL */ \
                                    DISABLE,  /*  IR_XTAL */ \
                                    DISABLE,  /* UR1_XTAL */ \
                                    DISABLE,  /* UR0_XTAL */ \
                                    DISABLE,  /*  AUD16M  */ \
                                    DISABLE,  /*  AUD24M  */ \
                                    DISABLE,  /*  DISPD   */ \
                                    DISABLE   /*  DISPI   */ \
                                   ) == 0x80 );

TTX_ASSERT_COMPILE( SYS_CLKEN2_RMKS(DISABLE,  /* PWM_XTAL */ \
                                    ENABLE ,  /*  IR_XTAL */ \
                                    DISABLE,  /* UR1_XTAL */ \
                                    DISABLE,  /* UR0_XTAL */ \
                                    DISABLE,  /*  AUD16M  */ \
                                    DISABLE,  /*  AUD24M  */ \
                                    DISABLE,  /*  DISPD   */ \
                                    DISABLE   /*  DISPI   */ \
                                   ) == 0x40 );

TTX_ASSERT_COMPILE( SYS_CLKEN2_RMKS(DISABLE,  /* PWM_XTAL */ \
                                    DISABLE,  /*  IR_XTAL */ \
                                    ENABLE ,  /* UR1_XTAL */ \
                                    DISABLE,  /* UR0_XTAL */ \
                                    DISABLE,  /*  AUD16M  */ \
                                    DISABLE,  /*  AUD24M  */ \
                                    DISABLE,  /*  DISPD   */ \
                                    DISABLE   /*  DISPI   */ \
                                   ) == 0x20 );

TTX_ASSERT_COMPILE( SYS_CLKEN2_RMKS(DISABLE,  /* PWM_XTAL */ \
                                    DISABLE,  /*  IR_XTAL */ \
                                    DISABLE,  /* UR1_XTAL */ \
                                    ENABLE ,  /* UR0_XTAL */ \
                                    DISABLE,  /*  AUD16M  */ \
                                    DISABLE,  /*  AUD24M  */ \
                                    DISABLE,  /*  DISPD   */ \
                                    DISABLE   /*  DISPI   */ \
                                   ) == 0x10 );

TTX_ASSERT_COMPILE( SYS_CLKEN2_RMKS(DISABLE,  /* PWM_XTAL */ \
                                    DISABLE,  /*  IR_XTAL */ \
                                    DISABLE,  /* UR1_XTAL */ \
                                    DISABLE,  /* UR0_XTAL */ \
                                    ENABLE ,  /*  AUD16M  */ \
                                    DISABLE,  /*  AUD24M  */ \
                                    DISABLE,  /*  DISPD   */ \
                                    DISABLE   /*  DISPI   */ \
                                   ) == 0x08 );

TTX_ASSERT_COMPILE( SYS_CLKEN2_RMKS(DISABLE,  /* PWM_XTAL */ \
                                    DISABLE,  /*  IR_XTAL */ \
                                    DISABLE,  /* UR1_XTAL */ \
                                    DISABLE,  /* UR0_XTAL */ \
                                    DISABLE,  /*  AUD16M  */ \
                                    ENABLE ,  /*  AUD24M  */ \
                                    DISABLE,  /*  DISPD   */ \
                                    DISABLE   /*  DISPI   */ \
                                   ) == 0x04 );


TTX_ASSERT_COMPILE( SYS_CLKEN2_RMKS(DISABLE,  /* PWM_XTAL */ \
                                    DISABLE,  /*  IR_XTAL */ \
                                    DISABLE,  /* UR1_XTAL */ \
                                    DISABLE,  /* UR0_XTAL */ \
                                    DISABLE,  /*  AUD16M  */ \
                                    DISABLE,  /*  AUD24M  */ \
                                    ENABLE ,  /*  DISPD   */ \
                                    DISABLE   /*  DISPI   */ \
                                   ) == 0x02 );

TTX_ASSERT_COMPILE( SYS_CLKEN2_RMKS(DISABLE,  /* PWM_XTAL */ \
                                    DISABLE,  /*  IR_XTAL */ \
                                    DISABLE,  /* UR1_XTAL */ \
                                    DISABLE,  /* UR0_XTAL */ \
                                    DISABLE,  /*  AUD16M  */ \
                                    DISABLE,  /*  AUD24M  */ \
                                    DISABLE,  /*  DISPD   */ \
                                    ENABLE    /*  DISPI   */ \
                                   ) == 0x01 );


/* Test SYS_SYNC_IEN_REG */
TTX_ASSERT_COMPILE( SYS_SYNC_IEN_DEFAULT        == 0x00 );

TTX_ASSERT_COMPILE( SYS_SYNC_IEN_RMKS(DISABLE,  /*  DV  */ \
                                      DISABLE   /*  IV  */
                                     ) == 0x00 );

TTX_ASSERT_COMPILE( SYS_SYNC_IEN_RMKS(DISABLE,  /*  DV  */ \
                                      ENABLE    /*  IV  */
                                     ) == 0x01 );

TTX_ASSERT_COMPILE( SYS_SYNC_IEN_RMKS(ENABLE ,  /*  DV  */ \
                                      DISABLE   /*  IV  */
                                     ) == 0x02 );

TTX_ASSERT_COMPILE( SYS_SYNC_IEN_RMKS(ENABLE ,  /*  DV  */ \
                                      ENABLE    /*  IV  */
                                     ) == 0x03 );


/* Test SYS_SYNC_INT_REG */
TTX_ASSERT_COMPILE( SYS_SYNC_INT_DEFAULT        == 0x00 );

/* Test SYS_GIE_1_REG */
TTX_ASSERT_COMPILE( SYS_GIE_1_DEFAULT        == 0x00 );

TTX_ASSERT_COMPILE( SYS_GIE_1_RMKS(DISABLE,  /*   VDIE   */ \
                                   DISABLE,  /*   ADIE   */ \
                                   DISABLE,  /* VBISLIIE */ \
                                   DISABLE,  /* VBIPPRIE */ \
                                   DISABLE,  /* VBIOSDIE */ \
                                   DISABLE,  /*   APLLIE */ \
                                   DISABLE,  /* AUTOSOYIE*/ \
                                   DISABLE   /*   SPIE   */
                                  ) == 0x00 );

TTX_ASSERT_COMPILE( SYS_GIE_1_RMKS( ENABLE,  /*   VDIE   */ \
                                    ENABLE,  /*   ADIE   */ \
                                    ENABLE,  /* VBISLIIE */ \
                                    ENABLE,  /* VBIPPRIE */ \
                                    ENABLE,  /* VBIOSDIE */ \
                                    ENABLE,  /*   APLLIE */ \
                                    ENABLE,  /* AUTOSOYIE*/ \
                                    ENABLE   /*   SPIE   */
                                  ) == 0xFF );

TTX_ASSERT_COMPILE( SYS_GIE_1_RMKS( ENABLE,  /*   VDIE   */ \
                                    DISABLE,  /*   ADIE   */ \
                                    ENABLE,  /* VBISLIIE */ \
                                    DISABLE,  /* VBIPPRIE */ \
                                    ENABLE,  /* VBIOSDIE */ \
                                    ENABLE,  /*   APLLIE */ \
                                    DISABLE,  /* AUTOSOYIE*/ \
                                    ENABLE   /*   SPIE   */
                                  ) == 0xAD );

TTX_ASSERT_COMPILE( SYS_GIE_1_RMKS(DISABLE,  /*   VDIE   */ \
                                   ENABLE,  /*   ADIE   */ \
                                   DISABLE,  /* VBISLIIE */ \
                                   ENABLE,  /* VBIPPRIE */ \
                                   DISABLE,  /* VBIOSDIE */ \
                                   DISABLE,  /*   APLLIE */ \
                                   ENABLE,  /* AUTOSOYIE*/ \
                                   DISABLE   /*   SPIE   */
                                  ) == 0x52 );

/* Test SYS_GIP_1_REG */
TTX_ASSERT_COMPILE( SYS_GIP_1_DEFAULT        == 0x00 );


/* Test SYS_SHM_REG */
TTX_ASSERT_COMPILE( SYS_SHM_DEFAULT        == 0x00 );

TTX_ASSERT_COMPILE( SYS_SHM_RMKS( 0,  /* CK_SEL */ \
                                  0   /*  MODE  */
                                ) == 0x00 );

TTX_ASSERT_COMPILE( SYS_SHM_RMKS( 0,  /* CK_SEL */ \
                                  1   /*  MODE  */
                                ) == 0x01 );

TTX_ASSERT_COMPILE( SYS_SHM_RMKS( 0,  /* CK_SEL */ \
                                  2   /*  MODE  */
                                ) == 0x02 );

TTX_ASSERT_COMPILE( SYS_SHM_RMKS( 1,  /* CK_SEL */ \
                                  0   /*  MODE  */
                                ) == 0x04 );

TTX_ASSERT_COMPILE( SYS_SHM_RMKS( 1,  /* CK_SEL */ \
                                  1   /*  MODE  */
                                ) == 0x05 );

TTX_ASSERT_COMPILE( SYS_SHM_RMKS( 1,  /* CK_SEL */ \
                                  2   /*  MODE  */
                                ) == 0x06 );

TTX_ASSERT_COMPILE( SYS_SHM_RMKS( 2,  /* CK_SEL */ \
                                  0   /*  MODE  */
                                ) == 0x08 );

TTX_ASSERT_COMPILE( SYS_SHM_RMKS( 2,  /* CK_SEL */ \
                                  1   /*  MODE  */
                                ) == 0x09 );

TTX_ASSERT_COMPILE( SYS_SHM_RMKS( 2,  /* CK_SEL */ \
                                  2   /*  MODE  */
                                ) == 0x0A );

/* Test SYS_NSDMA_REG */
TTX_ASSERT_COMPILE( SYS_NSDMA_DEFAULT        == 0x00 );

TTX_ASSERT_COMPILE( SYS_NSDMA_RMKS(DISABLE  /* ENABLE */ \
                                  ) == 0x00 );

TTX_ASSERT_COMPILE( SYS_NSDMA_RMKS(ENABLE   /* ENABLE */ \
                                  ) == 0x01 );


/*-------------------------------------------------------------------------*\
 * End of sys_hal_test.h                                                   *
\*-------------------------------------------------------------------------*/

