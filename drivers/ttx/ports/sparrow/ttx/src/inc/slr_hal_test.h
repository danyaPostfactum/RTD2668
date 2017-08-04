/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     slr_hal_test.h
 * @brief    Compile-time tests for Slicer HAL.
 * @details  Include this file to test CCL. No obj code will be generated.
 *             This file to be included in private CPP file.
 * @author   Janus Cheng
 * @date     Aug. 21, 2008
 * @version  0.1
\*****************************************************************************/

#include "slr_hal.h"                /* Provides: SLR HAL macros to be tested */
#include <ttx_assert.h>             /* Provides: TTX_ASSERT_COMPILE() */


/* Test SLR_CTRL0_REG */
TTX_ASSERT_COMPILE( SLR_CTRL0_DEFAULT        == 0x00000000 );

TTX_ASSERT_COMPILE( SLR_CTRL0_RMKS(RUNNING,  /*   DMA   */ \
                                   DISABLE,  /*   DSE   */ \
                                   VD  ,  /*   SRC   */ \
                                   NTSC ,  /*   SYS   */ \
                                   NORMAL ,  /*  FLDIND */ \
                                   NORMAL    /* FLDLNCNT*/ \
                                  ) == 0x00000000 );

TTX_ASSERT_COMPILE( SLR_CTRL0_RMKS(STOPPED,  /*   DMA   */ \
                                   DISABLE,  /*   DSE   */ \
                                   VD  ,  /*   SRC   */ \
                                   NTSC ,  /*   SYS   */ \
                                   NORMAL ,  /*  FLDIND */ \
                                   NORMAL    /* FLDLNCNT*/ \
                                  ) == 0x08000000 );

TTX_ASSERT_COMPILE( SLR_CTRL0_RMKS(STOPPED,  /*   DMA   */ \
                                   DISABLE,  /*   DSE   */ \
                                   YPBPR ,  /*   SRC   */ \
                                   NTSC ,  /*   SYS   */ \
                                   INVERSE,  /*  FLDIND */ \
                                   NORMAL    /* FLDLNCNT*/ \
                                  ) == 0x080000A0 );

TTX_ASSERT_COMPILE( SLR_CTRL0_RMKS(RUNNING,  /*   DMA   */ \
                                   ENABLE ,  /*   DSE   */ \
                                   VD  ,  /*   SRC   */ \
                                   PAL  ,  /*   SYS   */ \
                                   NORMAL ,  /*  FLDIND */ \
                                   INVERSE   /* FLDLNCNT*/ \
                                  ) == 0x04000050 );

TTX_ASSERT_COMPILE( SLR_CTRL0_RMKS(STOPPED,  /*   DMA   */ \
                                   ENABLE ,  /*   DSE   */ \
                                   YPBPR ,  /*   SRC   */ \
                                   PAL  ,  /*   SYS   */ \
                                   INVERSE,  /*  FLDIND */ \
                                   INVERSE   /* FLDLNCNT*/ \
                                  ) == 0x0C0000F0 );


/*-------------------------------------------------------------------------*\
 * End of slr_hal_test.h                                                   *
\*-------------------------------------------------------------------------*/

