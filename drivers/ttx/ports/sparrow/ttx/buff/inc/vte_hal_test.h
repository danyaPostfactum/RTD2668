/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     vte_hal_test.h
 * @brief    Compile-time tests for VTE HAL.
 * @details  This file to be included in private CPP file.
 * @author   Janus Cheng
 * @date     Aug. 21, 2008
 * @version  0.1
\*****************************************************************************/

#include "vte_hal.h"                /* Provides: VTE HAL macros to be tested */
#include "../../inc/ttx_assert.h"   /* Provides: TTX_ASSERT_COMPILE() */

/* Test VTE_INT_ST_REG */
TTX_ASSERT_COMPILE( VTE_INT_ST_DEFAULT        == 0x00000000 );
TTX_ASSERT_COMPILE( VTE_INT_ST_RMKS(RUNNING)  == 0x00000000 );
TTX_ASSERT_COMPILE( VTE_INT_ST_RMKS( DONE  )  == 0x00010000 );
TTX_ASSERT_COMPILE( VTE_INT_ST_RMKS(CLEARED)  == 0x00000000 );
TTX_ASSERT_COMPILE( VTE_INT_ST_RMKS(  NOP  )  == 0x00000000 );
TTX_ASSERT_COMPILE( VTE_INT_ST_RMKS( RESET )  == 0x00010000 );


/* Test VTE_INT_EN_REG */
TTX_ASSERT_COMPILE( VTE_INT_EN_DEFAULT        == 0x00000000 );

TTX_ASSERT_COMPILE( VTE_INT_EN_RMKS( NOP   ,  /*    MIE    */   \
                                     ASSERT   /* CD_INT_EN */   \
                                   ) == 0x00010000  );

TTX_ASSERT_COMPILE( VTE_INT_EN_RMKS( ASSERT,  /*    MIE    */   \
                                     NOP      /* CD_INT_EN */   \
                                   ) == 0x80000000  );

TTX_ASSERT_COMPILE( VTE_INT_EN_RMKS( ASSERT,  /*    MIE    */   \
                                     ASSERT   /* CD_INT_EN */   \
                                   ) == 0x80010000  );


/* Test VTE_INT_DIS_REG */
TTX_ASSERT_COMPILE( VTE_INT_DIS_DEFAULT        == 0x00000000 );

TTX_ASSERT_COMPILE( VTE_INT_DIS_RMKS( NOP   ,  /*    MID     */  \
                                      ASSERT   /* CD_INT_DIS */  \
                                    ) == 0x00010000  );

TTX_ASSERT_COMPILE( VTE_INT_DIS_RMKS( ASSERT,  /*    MID     */  \
                                      NOP      /* CD_INT_DIS */  \
                                    ) == 0x80000000  );

TTX_ASSERT_COMPILE( VTE_INT_DIS_RMKS( ASSERT,  /*    MID     */  \
                                      ASSERT   /* CD_INT_DIS */  \
                                    ) == 0x80010000  );


/* Test VTE_CD_SADDR_REG */
TTX_ASSERT_COMPILE( VTE_CD_SADDR_DEFAULT         == 0x00000000 );
TTX_ASSERT_COMPILE( VTE_CD_SADDR_RMK(0x00000000) == 0x00000000 );
TTX_ASSERT_COMPILE( VTE_CD_SADDR_RMK(0xFFFFFFFF) == 0x07FFFFFF );
TTX_ASSERT_COMPILE( VTE_CD_SADDR_RMK(0x5A5A5A5A) == 0x025A5A5A );
TTX_ASSERT_COMPILE( VTE_CD_SADDR_RMK(0xA5A5A5A5) == 0x05A5A5A5 );


/* Test VTE_CD_DADDR_REG */
TTX_ASSERT_COMPILE( VTE_CD_DADDR_DEFAULT         == 0x00000000 );
TTX_ASSERT_COMPILE( VTE_CD_DADDR_RMK(0x00000000) == 0x00000000 );
TTX_ASSERT_COMPILE( VTE_CD_DADDR_RMK(0xFFFFFFFF) == 0x07FFFFFF );
TTX_ASSERT_COMPILE( VTE_CD_DADDR_RMK(0x5A5A5A5A) == 0x025A5A5A );
TTX_ASSERT_COMPILE( VTE_CD_DADDR_RMK(0xA5A5A5A5) == 0x05A5A5A5 );


/* Test VTE_CD_CTRL_REG */
TTX_ASSERT_COMPILE( VTE_CD_CTRL_DEFAULT          == 0x00000000 );

TTX_ASSERT_COMPILE( VTE_CD_CTRL_RMKS( 0x001  ,  /*  DATA_LEN  */  \
                                      CLEAR  ,  /*  DATA_ERR  */  \
                                      H2418  ,  /*  MODE      */  \
                                      ENABLE    /*  EN        */  \
                                    ) == 0x00010005  );

TTX_ASSERT_COMPILE( VTE_CD_CTRL_RMKS( 0x1FF  ,  /*  DATA_LEN  */  \
                                      ERROR  ,  /*  DATA_ERR  */  \
                                      AIT    ,  /*  MODE      */  \
                                      ENABLE    /*  EN        */  \
                                    ) == 0x01FF0019  );

TTX_ASSERT_COMPILE( VTE_CD_CTRL_RMKS( 0x0FF  ,  /*  DATA_LEN  */  \
                                      ERROR  ,  /*  DATA_ERR  */  \
                                      AIT    ,  /*  MODE      */  \
                                      ENABLE    /*  EN        */  \
                                    ) == 0x00FF0019  );


TTX_ASSERT_COMPILE( VTE_CD_CTRL_RMKS( 0x05A  ,  /*  DATA_LEN  */  \
                                      ERROR  ,  /*  DATA_ERR  */  \
                                      H84M0  ,  /*  MODE      */  \
                                      ENABLE    /*  EN        */  \
                                    ) == 0x005A0011  );

TTX_ASSERT_COMPILE( VTE_CD_CTRL_RMKS( 0x0A5  ,  /*  DATA_LEN  */  \
                                      OK     ,  /*  DATA_ERR  */  \
                                      ODDPAR ,  /*  MODE      */  \
                                      DISABLE   /*  EN        */  \
                                    ) == 0x00A50006  );


TTX_ASSERT_COMPILE( VTE_CD_CTRL_RMKS( 0x055  ,  /*  DATA_LEN  */  \
                                      CLEAR  ,  /*  DATA_ERR  */  \
                                      AIT    ,  /*  MODE      */  \
                                      DISABLE   /*  EN        */  \
                                    ) == 0x00550008  );


/* Test VTE_CD_ERR_RVALUE_REG */
TTX_ASSERT_COMPILE( VTE_CD_ERR_RVALUE_DEFAULT   == 0x00000000 );

TTX_ASSERT_COMPILE( VTE_CD_ERR_RVALUE_RMK( 0xFFFFFF   /*  ERR_RVALUE */  \
                                         ) == 0x00FFFFFF  );

TTX_ASSERT_COMPILE( VTE_CD_ERR_RVALUE_RMK( 0x000000   /*  ERR_RVALUE */  \
                                         ) == 0x00000000  );

TTX_ASSERT_COMPILE( VTE_CD_ERR_RVALUE_RMK( 0x5A5A5A   /*  ERR_RVALUE */  \
                                         ) == 0x005A5A5A  );

TTX_ASSERT_COMPILE( VTE_CD_ERR_RVALUE_RMK( 0xA5A5A5   /*  ERR_RVALUE */  \
                                         ) == 0x00A5A5A5  );


/*-------------------------------------------------------------------------*\
 * End of vte_hal_test.h                                                   *
\*-------------------------------------------------------------------------*/
