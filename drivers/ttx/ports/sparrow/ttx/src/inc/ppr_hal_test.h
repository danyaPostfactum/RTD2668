/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ppr_hal_test.h
 * @brief    Compile-time tests for PPR HAL.
 * @details  This file to be included in private CPP file.
 * @author   Janus Cheng
 * @date     Aug. 21, 2008
 * @version  0.1
\*****************************************************************************/

#include "ppr_hal.h"                /* Provides: PPR HAL macros to be tested */
#include "../../inc/ttx_assert.h"   /* Provides: TTX_ASSERT_COMPILE() */


/* Test PPR_CTRL_REG */
TTX_ASSERT_COMPILE( PPR_CTRL_DEFAULT            == 0x00000000 );
TTX_ASSERT_COMPILE( PPR_CTRL_RMKS(625, DISABLE) == 0x00000000 );
TTX_ASSERT_COMPILE( PPR_CTRL_RMKS(625, ENABLE ) == 0x00000001 );
TTX_ASSERT_COMPILE( PPR_CTRL_RMKS(525, DISABLE) == 0x00000002 );
TTX_ASSERT_COMPILE( PPR_CTRL_RMKS(525, ENABLE ) == 0x00000003 );

/* Test PPR_VBIINT_ST_REG */
TTX_ASSERT_COMPILE( PPR_VBIINT_ST_DEFAULT       == 0x00000000 );

TTX_ASSERT_COMPILE( PPR_VBIINT_ST_RMKS( INACTIVE,  /*    OSD     */  \
                                        INACTIVE,  /*    VTE     */  \
                                        INACTIVE   /*    PPR     */  \
                                      ) == 0x00000000  );

TTX_ASSERT_COMPILE( PPR_VBIINT_ST_RMKS( INACTIVE,  /*    OSD     */  \
                                        INACTIVE,  /*    VTE     */  \
                                        ACTIVE   /*    PPR     */  \
                                      ) == 0x00000001  );

TTX_ASSERT_COMPILE( PPR_VBIINT_ST_RMKS( INACTIVE,  /*    OSD     */  \
                                        ACTIVE,  /*    VTE     */  \
                                        INACTIVE   /*    PPR     */  \
                                      ) == 0x00000002  );

TTX_ASSERT_COMPILE( PPR_VBIINT_ST_RMKS( INACTIVE,  /*    OSD     */  \
                                        ACTIVE,  /*    VTE     */  \
                                        ACTIVE   /*    PPR     */  \
                                      ) == 0x00000003  );

TTX_ASSERT_COMPILE( PPR_VBIINT_ST_RMKS(   ACTIVE,  /*    OSD     */  \
                    INACTIVE,  /*    VTE     */  \
                    INACTIVE   /*    PPR     */  \
                                      ) == 0x00000004  );

TTX_ASSERT_COMPILE( PPR_VBIINT_ST_RMKS(   ACTIVE,  /*    OSD     */  \
                    INACTIVE,  /*    VTE     */  \
                    ACTIVE   /*    PPR     */  \
                                      ) == 0x00000005  );

TTX_ASSERT_COMPILE( PPR_VBIINT_ST_RMKS(   ACTIVE,  /*    OSD     */  \
                    ACTIVE,  /*    VTE     */  \
                    INACTIVE   /*    PPR     */  \
                                      ) == 0x00000006  );

TTX_ASSERT_COMPILE( PPR_VBIINT_ST_RMKS(   ACTIVE,  /*    OSD     */  \
                    ACTIVE,  /*    VTE     */  \
                    ACTIVE   /*    PPR     */  \
                                      ) == 0x00000007  );


/* Test PPR_INT_ST_REG */
TTX_ASSERT_COMPILE( PPR_INT_ST_DEFAULT          == 0x00000000 );
TTX_ASSERT_COMPILE( PPR_INT_ST_RMKS(  ACTIVE,   /*  DEBUG_INT */   \
                                      DONE,     /*  MEMSET    */   \
                                      DONE,     /*  MEMSAR    */   \
                                      DONE,     /*    PFE     */   \
                                      RECEIVED, /*    X31     */   \
                                      ACTIVE,   /*  FB_FUL    */   \
                                      ACTIVE,   /*  FB_VALID  */   \
                                      RECEIVED, /*   830_F2   */   \
                                      RECEIVED, /*   830_F1   */   \
                                      RECEIVED, /*    VPS     */   \
                                      RECEIVED, /*    CGMS    */   \
                                      RECEIVED, /*    WSS     */   \
                                      RECEIVED  /*    CC      */   \
                                   ) == 0x800701FF  );


TTX_ASSERT_COMPILE( PPR_INT_ST_RMKS(  INACTIVE, /*  DEBUG_INT */   \
                                      RUNNING,  /*  MEMSET    */   \
                                      RUNNING,  /*  MEMSAR    */   \
                                      RUNNING,  /*    PFE     */   \
                                      NOT_RCVD, /*    X31     */   \
                                      INACTIVE, /*  FB_FUL    */   \
                                      INACTIVE, /*  FB_VALID  */   \
                                      NOT_RCVD, /*   830_F2   */   \
                                      NOT_RCVD, /*   830_F1   */   \
                                      NOT_RCVD, /*    VPS     */   \
                                      NOT_RCVD, /*    CGMS    */   \
                                      NOT_RCVD, /*    WSS     */   \
                                      NOT_RCVD  /*    CC      */   \
                                   ) == 0x00000000  );


TTX_ASSERT_COMPILE( PPR_INT_ST_RMKS(  ACTIVE,   /*  DEBUG_INT */   \
                                      DONE,     /*  MEMSET    */   \
                                      RUNNING,  /*  MEMSAR    */   \
                                      DONE,     /*    PFE     */   \
                                      RECEIVED, /*    X31     */   \
                                      ACTIVE,   /*  FB_FUL    */   \
                                      ACTIVE,   /*  FB_VALID  */   \
                                      RECEIVED, /*   830_F2   */   \
                                      NOT_RCVD, /*   830_F1   */   \
                                      RECEIVED, /*    VPS     */   \
                                      NOT_RCVD, /*    CGMS    */   \
                                      RECEIVED, /*    WSS     */   \
                                      NOT_RCVD  /*    CC      */   \
                                   ) == 0x800501EA  );

/* Test PPR_INT_EN_REG */
TTX_ASSERT_COMPILE( PPR_INT_EN_DEFAULT          == 0x00000000 );
TTX_ASSERT_COMPILE( PPR_INT_EN_RMKS(  NOP   ,   /*  MASTER    */   \
                                      NOP   ,   /*  DEBUG_INT */   \
                                      NOP   ,   /*  MEMSET    */   \
                                      NOP   ,   /*  MEMSAR    */   \
                                      NOP   ,   /*    PFE     */   \
                                      NOP   ,   /*    X31     */   \
                                      NOP   ,   /*  FB_FUL    */   \
                                      NOP   ,   /*  FB_VALID  */   \
                                      NOP   ,   /*   830_F2   */   \
                                      NOP   ,   /*   830_F1   */   \
                                      NOP   ,   /*    VPS     */   \
                                      NOP   ,   /*    CGMS    */   \
                                      NOP   ,   /*    WSS     */   \
                                      NOP       /*    CC      */   \
                                   ) == 0x00000000  );

TTX_ASSERT_COMPILE( PPR_INT_EN_RMKS(  ENABLE,   /*  MASTER    */   \
                                      ENABLE,   /*  DEBUG_INT */   \
                                      ENABLE,   /*  MEMSET    */   \
                                      ENABLE,   /*  MEMSAR    */   \
                                      ENABLE,   /*    PFE     */   \
                                      ENABLE,   /*    X31     */   \
                                      ENABLE,   /*  FB_FUL    */   \
                                      ENABLE,   /*  FB_VALID  */   \
                                      ENABLE,   /*   830_F2   */   \
                                      ENABLE,   /*   830_F1   */   \
                                      ENABLE,   /*    VPS     */   \
                                      ENABLE,   /*    CGMS    */   \
                                      ENABLE,   /*    WSS     */   \
                                      ENABLE    /*    CC      */   \
                                   ) == 0xC00701FF  );

TTX_ASSERT_COMPILE( PPR_INT_EN_RMKS(  ENABLE,   /*  MASTER    */   \
                                      NOP   ,   /*  DEBUG_INT */   \
                                      NOP   ,   /*  MEMSET    */   \
                                      NOP   ,   /*  MEMSAR    */   \
                                      NOP   ,   /*    PFE     */   \
                                      NOP   ,   /*    X31     */   \
                                      NOP   ,   /*  FB_FUL    */   \
                                      NOP   ,   /*  FB_VALID  */   \
                                      NOP   ,   /*   830_F2   */   \
                                      NOP   ,   /*   830_F1   */   \
                                      NOP   ,   /*    VPS     */   \
                                      NOP   ,   /*    CGMS    */   \
                                      NOP   ,   /*    WSS     */   \
                                      NOP       /*    CC      */   \
                                   ) == 0x80000000  );

TTX_ASSERT_COMPILE( PPR_INT_EN_RMKS(  NOP   ,   /*  MASTER    */   \
                                      ENABLE,   /*  DEBUG_INT */   \
                                      NOP   ,   /*  MEMSET    */   \
                                      ENABLE,   /*  MEMSAR    */   \
                                      NOP   ,   /*    PFE     */   \
                                      ENABLE,   /*    X31     */   \
                                      NOP   ,   /*  FB_FUL    */   \
                                      ENABLE,   /*  FB_VALID  */   \
                                      NOP   ,   /*   830_F2   */   \
                                      ENABLE,   /*   830_F1   */   \
                                      NOP   ,   /*    VPS     */   \
                                      ENABLE,   /*    CGMS    */   \
                                      NOP   ,   /*    WSS     */   \
                                      ENABLE    /*    CC      */   \
                                   ) == 0x40020155  );

TTX_ASSERT_COMPILE( PPR_INT_EN_RMKS(  ENABLE,   /*  MASTER    */   \
                                      NOP   ,   /*  DEBUG_INT */   \
                                      ENABLE,   /*  MEMSET    */   \
                                      NOP   ,   /*  MEMSAR    */   \
                                      ENABLE,   /*    PFE     */   \
                                      NOP   ,   /*    X31     */   \
                                      ENABLE,   /*  FB_FUL    */   \
                                      NOP   ,   /*  FB_VALID  */   \
                                      ENABLE,   /*   830_F2   */   \
                                      NOP   ,   /*   830_F1   */   \
                                      ENABLE,   /*    VPS     */   \
                                      NOP   ,   /*    CGMS    */   \
                                      ENABLE,   /*    WSS     */   \
                                      NOP       /*    CC      */   \
                                   ) == 0x800500AA  );

TTX_ASSERT_COMPILE( PPR_INT_EN_RMKS(  ENABLE,   /*  MASTER    */   \
                                      ENABLE,   /*  DEBUG_INT */   \
                                      NOP   ,   /*  MEMSET    */   \
                                      NOP   ,   /*  MEMSAR    */   \
                                      ENABLE,   /*    PFE     */   \
                                      NOP   ,   /*    X31     */   \
                                      ENABLE,   /*  FB_FUL    */   \
                                      NOP   ,   /*  FB_VALID  */   \
                                      NOP   ,   /*   830_F2   */   \
                                      NOP   ,   /*   830_F1   */   \
                                      ENABLE,   /*    VPS     */   \
                                      ENABLE,   /*    CGMS    */   \
                                      ENABLE,   /*    WSS     */   \
                                      NOP       /*    CC      */   \
                                   ) == 0xC001008E  );

/* Test PPR_INT_DIS_REG */
TTX_ASSERT_COMPILE( PPR_INT_DIS_DEFAULT          == 0x00000000 );
TTX_ASSERT_COMPILE( PPR_INT_DIS_RMKS( NOP   ,   /*  MASTER    */   \
                                      NOP   ,   /*  DEBUG_INT */   \
                                      NOP   ,   /*  MEMSET    */   \
                                      NOP   ,   /*  MEMSAR    */   \
                                      NOP   ,   /*    PFE     */   \
                                      NOP   ,   /*    X31     */   \
                                      NOP   ,   /*  FB_FUL    */   \
                                      NOP   ,   /*  FB_VALID  */   \
                                      NOP   ,   /*   830_F2   */   \
                                      NOP   ,   /*   830_F1   */   \
                                      NOP   ,   /*    VPS     */   \
                                      NOP   ,   /*    CGMS    */   \
                                      NOP   ,   /*    WSS     */   \
                                      NOP       /*    CC      */   \
                                    ) == 0x00000000  );

TTX_ASSERT_COMPILE( PPR_INT_DIS_RMKS( DISABLE,  /*  MASTER    */   \
                                      DISABLE,  /*  DEBUG_INT */   \
                                      DISABLE,  /*  MEMSET    */   \
                                      DISABLE,  /*  MEMSAR    */   \
                                      DISABLE,  /*    PFE     */   \
                                      DISABLE,  /*    X31     */   \
                                      DISABLE,  /*  FB_FUL    */   \
                                      DISABLE,  /*  FB_VALID  */   \
                                      DISABLE,  /*   830_F2   */   \
                                      DISABLE,  /*   830_F1   */   \
                                      DISABLE,  /*    VPS     */   \
                                      DISABLE,  /*    CGMS    */   \
                                      DISABLE,  /*    WSS     */   \
                                      DISABLE   /*    CC      */   \
                                    ) == 0xC00701FF  );

TTX_ASSERT_COMPILE( PPR_INT_DIS_RMKS( DISABLE,  /*  MASTER    */   \
                                      NOP   ,   /*  DEBUG_INT */   \
                                      NOP   ,   /*  MEMSET    */   \
                                      NOP   ,   /*  MEMSAR    */   \
                                      NOP   ,   /*    PFE     */   \
                                      NOP   ,   /*    X31     */   \
                                      NOP   ,   /*  FB_FUL    */   \
                                      NOP   ,   /*  FB_VALID  */   \
                                      NOP   ,   /*   830_F2   */   \
                                      NOP   ,   /*   830_F1   */   \
                                      NOP   ,   /*    VPS     */   \
                                      NOP   ,   /*    CGMS    */   \
                                      NOP   ,   /*    WSS     */   \
                                      NOP       /*    CC      */   \
                                    ) == 0x80000000  );

TTX_ASSERT_COMPILE( PPR_INT_DIS_RMKS( NOP    ,  /*  MASTER    */   \
                                      DISABLE,  /*  DEBUG_INT */   \
                                      NOP    ,  /*  MEMSET    */   \
                                      DISABLE,  /*  MEMSAR    */   \
                                      NOP    ,  /*    PFE     */   \
                                      DISABLE,  /*    X31     */   \
                                      NOP    ,  /*  FB_FUL    */   \
                                      DISABLE,  /*  FB_VALID  */   \
                                      NOP    ,  /*   830_F2   */   \
                                      DISABLE,  /*   830_F1   */   \
                                      NOP    ,  /*    VPS     */   \
                                      DISABLE,  /*    CGMS    */   \
                                      NOP    ,  /*    WSS     */   \
                                      DISABLE   /*    CC      */   \
                                    ) == 0x40020155  );

TTX_ASSERT_COMPILE( PPR_INT_DIS_RMKS( DISABLE,  /*  MASTER    */   \
                                      NOP    ,  /*  DEBUG_INT */   \
                                      DISABLE,  /*  MEMSET    */   \
                                      NOP    ,  /*  MEMSAR    */   \
                                      DISABLE,  /*    PFE     */   \
                                      NOP    ,  /*    X31     */   \
                                      DISABLE,  /*  FB_FUL    */   \
                                      NOP    ,  /*  FB_VALID  */   \
                                      DISABLE,  /*   830_F2   */   \
                                      NOP    ,  /*   830_F1   */   \
                                      DISABLE,  /*    VPS     */   \
                                      NOP    ,  /*    CGMS    */   \
                                      DISABLE,  /*    WSS     */   \
                                      NOP       /*    CC      */   \
                                    ) == 0x800500AA  );

TTX_ASSERT_COMPILE( PPR_INT_DIS_RMKS( DISABLE,  /*  MASTER    */   \
                                      DISABLE,  /*  DEBUG_INT */   \
                                      NOP    ,  /*  MEMSET    */   \
                                      NOP    ,  /*  MEMSAR    */   \
                                      DISABLE,  /*    PFE     */   \
                                      NOP    ,  /*    X31     */   \
                                      DISABLE,  /*  FB_FUL    */   \
                                      NOP    ,  /*  FB_VALID  */   \
                                      NOP    ,  /*   830_F2   */   \
                                      NOP    ,  /*   830_F1   */   \
                                      DISABLE,  /*    VPS     */   \
                                      DISABLE,  /*    CGMS    */   \
                                      DISABLE,  /*    WSS     */   \
                                      NOP       /*    CC      */   \
                                    ) == 0xC001008E  );


/* Test DATA_ACQU_ST_REG */
TTX_ASSERT_COMPILE( PPR_DATA_ACQU_ST_DEFAULT        == 0x00000000 );

TTX_ASSERT_COMPILE( PPR_DATA_ACQU_ST_RMK( FALSE,    /* FB_FULL  */ \
                    FALSE     /* FB_VALID */ \
                                        ) == 0x00000000  );

TTX_ASSERT_COMPILE( PPR_DATA_ACQU_ST_RMK( FALSE,    /* FB_FULL  */ \
                    TRUE      /* FB_VALID */ \
                                        ) == 0x00000001  );

TTX_ASSERT_COMPILE( PPR_DATA_ACQU_ST_RMK( TRUE ,    /* FB_FULL  */ \
                    FALSE     /* FB_VALID */ \
                                        ) == 0x00000002  );

TTX_ASSERT_COMPILE( PPR_DATA_ACQU_ST_RMK( TRUE ,    /* FB_FULL  */ \
                    TRUE      /* FB_VALID */ \
                                        ) == 0x00000003  );


/* Test PKT_RX_CNT_REG */
TTX_ASSERT_COMPILE( PPR_PKT_RX_CNT_DEFAULT        == 0x00000000 );

TTX_ASSERT_COMPILE( PPR_PKT_RX_CNT_RMKS( NOP    /* RST  */ \
                                       ) == 0x00000000  );

TTX_ASSERT_COMPILE( PPR_PKT_RX_CNT_RMKS( RST    /* RST  */ \
                                       ) == 0x80000000  );

/* Test PKT_ERR_CNT_REG */
TTX_ASSERT_COMPILE( PPR_PKT_ERR_CNT_DEFAULT        == 0x00000000 );

TTX_ASSERT_COMPILE( PPR_PKT_ERR_CNT_RMKS( NOP    /* RST  */ \
                                        ) == 0x00000000  );

TTX_ASSERT_COMPILE( PPR_PKT_ERR_CNT_RMKS( RST    /* RST  */ \
                                        ) == 0x80000000  );


/* Test FB_IND_REG */
TTX_ASSERT_COMPILE( PPR_FB_IND_DEFAULT          == 0x00000000 );

TTX_ASSERT_COMPILE( PPR_FB_IND_RMK( 0x00   ,    /* LAST_PKT_ADDR */ \
                                    0x00000     /*   PKT_X0_IND  */ \
                                  ) == 0x00000000  );

TTX_ASSERT_COMPILE( PPR_FB_IND_RMK( 0xFF   ,    /* LAST_PKT_ADDR */ \
                                    0xFFFFF     /*   PKT_X0_IND  */ \
                                  ) == 0xFF03FFFF  );

TTX_ASSERT_COMPILE( PPR_FB_IND_RMK( 0x12   ,    /* LAST_PKT_ADDR */ \
                                    0x34567     /*   PKT_X0_IND  */ \
                                  ) == 0x12034567  );

TTX_ASSERT_COMPILE( PPR_FB_IND_RMK( 0x5A   ,    /* LAST_PKT_ADDR */ \
                                    0xA55A5     /*   PKT_X0_IND  */ \
                                  ) == 0x5A0255A5  );

TTX_ASSERT_COMPILE( PPR_FB_IND_RMK( 0xA5   ,    /* LAST_PKT_ADDR */ \
                                    0x5AA5A     /*   PKT_X0_IND  */ \
                                  ) == 0xA501AA5A );


/* Test BFILTER_CTRL_REG */
TTX_ASSERT_COMPILE( PPR_BFILTER_CTRL_DEFAULT        == 0x00000000 );

TTX_ASSERT_COMPILE( PPR_BFILTER_CTRL_RMKS( KEEP    /* MAGPKTDC */   \
                                         ) == 0x00000000  );

TTX_ASSERT_COMPILE( PPR_BFILTER_CTRL_RMKS( DISCARD /* MAGPKTDC */   \
                                         ) == 0x00000001  );


/* Test PPR_MAGk_REG */
#define PPR_MAGx_COMPILE_TIME_TEST(x)                                   \
    TTX_ASSERT_COMPILE( PPR_##x##_DEFAULT            == 0x00000000 );   \
    TTX_ASSERT_COMPILE( PPR_##x##_RMK(   0x00000000   /*  DES_BADDR */  \
                      ) == 0x00000000  );                               \
    TTX_ASSERT_COMPILE( PPR_##x##_RMK(   0x00FFFFFF   /*  DES_BADDR */  \
                      ) == 0x003FFFFF  );                               \
    TTX_ASSERT_COMPILE( PPR_##x##_RMK(   0x005A5A5A   /*  DES_BADDR */  \
                      ) == 0x001A5A5A  );                               \
    TTX_ASSERT_COMPILE( PPR_##x##_RMK(   0x00A5A5A5   /*  DES_BADDR */  \
                      ) == 0x0025A5A5  )


PPR_MAGx_COMPILE_TIME_TEST( MAG1 );
PPR_MAGx_COMPILE_TIME_TEST( MAG2 );
PPR_MAGx_COMPILE_TIME_TEST( MAG3 );
PPR_MAGx_COMPILE_TIME_TEST( MAG4 );
PPR_MAGx_COMPILE_TIME_TEST( MAG5 );
PPR_MAGx_COMPILE_TIME_TEST( MAG6 );
PPR_MAGx_COMPILE_TIME_TEST( MAG7 );
PPR_MAGx_COMPILE_TIME_TEST( MAG8 );
PPR_MAGx_COMPILE_TIME_TEST( BPKT );


/* Test PPR_LVLD_TOP_REG */
TTX_ASSERT_COMPILE( PPR_LVLD_TOP_DEFAULT == 0x00160005 );

TTX_ASSERT_COMPILE( PPR_LVLD_TOP_RMK( 0x05A,     /* END_LINE   */   \
                                      0x0FF      /* START_LINE */   \
                                    ) == 0x005A00FF  );

TTX_ASSERT_COMPILE( PPR_LVLD_TOP_RMK( 0x123,     /* END_LINE   */   \
                                      0x056      /* START_LINE */   \
                                    ) == 0x01230056  );


/* Test PPR_LVLD_BTM_REG */
TTX_ASSERT_COMPILE( PPR_LVLD_BTM_DEFAULT == 0x00160005 );

TTX_ASSERT_COMPILE( PPR_LVLD_BTM_RMK( 0x05A,     /* END_LINE   */   \
                                      0x0FF      /* START_LINE */   \
                                    ) == 0x005A00FF  );

TTX_ASSERT_COMPILE( PPR_LVLD_BTM_RMK( 0x123,     /* END_LINE   */   \
                                      0x056      /* START_LINE */   \
                                    ) == 0x01230056  );


/* Test PG_MODE_REG */
TTX_ASSERT_COMPILE( PPR_PG_MODE_DEFAULT       == 0x00000000 );

TTX_ASSERT_COMPILE( PPR_PG_MODE_RMKS(TT  ,PTNGEN,ENABLE ) == 0x0000003 );
TTX_ASSERT_COMPILE( PPR_PG_MODE_RMKS(CC  ,PTNGEN,DISABLE) == 0x0000012 );
TTX_ASSERT_COMPILE( PPR_PG_MODE_RMKS(VPS ,SLICER,DISABLE) == 0x0000020 );
TTX_ASSERT_COMPILE( PPR_PG_MODE_RMKS(WSS ,SLICER,ENABLE ) == 0x0000031 );
TTX_ASSERT_COMPILE( PPR_PG_MODE_RMKS(CGMS,PTNGEN,DISABLE) == 0x0000042 );

/* Test PG_LINE_REG */
TTX_ASSERT_COMPILE( PPR_PG_LINE_DEFAULT         == 0x00000000 );

TTX_ASSERT_COMPILE( PPR_PG_LINE_RMKS( ENABLE  ) == 0x000001 );
TTX_ASSERT_COMPILE( PPR_PG_LINE_RMKS( DISABLE ) == 0x000000 );

/* Test PGDATA_BxBy_REG */
#define PPR_PGDATA_BxBy_COMPILE_TIME_TEST(x,y)                                 \
    TTX_ASSERT_COMPILE( PPR_PGDATA_B##x##B##y##_DEFAULT == 0x00000000 );   \
    TTX_ASSERT_COMPILE( PPR_PGDATA_B##x##B##y##_RMK(0x5A,  /*  BYTE1  */   \
                                                    0xA5,  /*  BYTE2  */   \
                                                    0x00,  /*  BYTE3  */   \
                                                    0xFF   /*  BYTE4  */   \
                      ) == 0x5AA500FF  );                                  \
    TTX_ASSERT_COMPILE( PPR_PGDATA_B##x##B##y##_RMK(0x00,  /*  BYTE1  */   \
                                                    0xFF,  /*  BYTE2  */   \
                                                    0xA5,  /*  BYTE3  */   \
                                                    0x5A   /*  BYTE4  */   \
                      ) == 0x00FFA55A  )

PPR_PGDATA_BxBy_COMPILE_TIME_TEST( 1, 4);
PPR_PGDATA_BxBy_COMPILE_TIME_TEST( 5, 8);
PPR_PGDATA_BxBy_COMPILE_TIME_TEST( 9,12);
PPR_PGDATA_BxBy_COMPILE_TIME_TEST(13,16);
PPR_PGDATA_BxBy_COMPILE_TIME_TEST(17,20);
PPR_PGDATA_BxBy_COMPILE_TIME_TEST(21,24);
PPR_PGDATA_BxBy_COMPILE_TIME_TEST(25,28);
PPR_PGDATA_BxBy_COMPILE_TIME_TEST(29,32);
PPR_PGDATA_BxBy_COMPILE_TIME_TEST(33,36);
PPR_PGDATA_BxBy_COMPILE_TIME_TEST(37,40);
PPR_PGDATA_BxBy_COMPILE_TIME_TEST(41,44);

/* Since PPR_PGDATA_B45B47_REG has only 3 fields which is different from other registers,
   we shall write custom tests for this register.
*/
TTX_ASSERT_COMPILE( PPR_PGDATA_B45B47_DEFAULT == 0x00000000 );

TTX_ASSERT_COMPILE( PPR_PGDATA_B45B47_RMK(0x5A,  /*  BYTE1  */   \
                    0xA5,  /*  BYTE2  */   \
                    0x00   /*  BYTE3  */   \
                                         ) == 0x5AA50000  );

TTX_ASSERT_COMPILE( PPR_PGDATA_B45B47_RMK(0x00,  /*  BYTE1  */   \
                    0xFF,  /*  BYTE2  */   \
                    0xA5   /*  BYTE3  */   \
                                         ) == 0x00FFA500  );

/* Test PPR_DEBUG_REG */
TTX_ASSERT_COMPILE( PPR_DEBUG_DEFAULT == 0x01FFFFFF );

TTX_ASSERT_COMPILE( PPR_DEBUG_RMK(   0x00000000     /*  CLKCNT   */   \
                                 ) == 0x00000000  );

TTX_ASSERT_COMPILE( PPR_DEBUG_RMK(   0x01234567     /*  CLKCNT   */   \
                                 ) == 0x01234567  );

TTX_ASSERT_COMPILE( PPR_DEBUG_RMK(   0x05A5A5A5     /*  CLKCNT   */   \
                                 ) == 0x01A5A5A5  );

TTX_ASSERT_COMPILE( PPR_DEBUG_RMK(   0x0A5A5A5A     /*  CLKCNT   */   \
                                 ) == 0x005A5A5A  );

/* Test PPR_DBOUNCE2_REG */
TTX_ASSERT_COMPILE( PPR_DBOUNCE2_DEFAULT == 0x000000FF );

TTX_ASSERT_COMPILE( PPR_DEBUG_RMK(   0x00     /*  LIM_VSYNC_CNT  */   \
                                 ) == 0x00000000  );

TTX_ASSERT_COMPILE( PPR_DEBUG_RMK(   0x5A     /*  LIM_VSYNC_CNT  */   \
                                 ) == 0x0000005A  );

TTX_ASSERT_COMPILE( PPR_DEBUG_RMK(   0xA5     /*  LIM_VSYNC_CNT  */   \
                                 ) == 0x000000A5  );

TTX_ASSERT_COMPILE( PPR_DEBUG_RMK(   0x12     /*  LIM_VSYNC_CNT  */   \
                                 ) == 0x00000012  );


/* Test PPR_PFE_ACTIVE_REG */
TTX_ASSERT_COMPILE( PPR_PFE_ACTIVE_DEFAULT == 0x00000000 );

TTX_ASSERT_COMPILE( PPR_PFE_ACTIVE_RMKS(INACTIVE,  /*  BPKT  */   \
                                        INACTIVE,  /*  MAG8  */   \
                                        INACTIVE,  /*  MAG7  */   \
                                        INACTIVE,  /*  MAG6  */   \
                                        INACTIVE,  /*  MAG5  */   \
                                        INACTIVE,  /*  MAG4  */   \
                                        INACTIVE,  /*  MAG3  */   \
                                        INACTIVE,  /*  MAG2  */   \
                                        INACTIVE   /*  MAG1  */   \
                                       ) == 0x00000000  );

TTX_ASSERT_COMPILE( PPR_PFE_ACTIVE_RMKS(INACTIVE,  /*  BPKT  */   \
                                        ACTIVE ,  /*  MAG8  */   \
                                        INACTIVE,  /*  MAG7  */   \
                                        ACTIVE ,  /*  MAG6  */   \
                                        INACTIVE,  /*  MAG5  */   \
                                        ACTIVE ,  /*  MAG4  */   \
                                        INACTIVE,  /*  MAG3  */   \
                                        ACTIVE ,  /*  MAG2  */   \
                                        INACTIVE   /*  MAG1  */   \
                                       ) == 0x000000AA  );

TTX_ASSERT_COMPILE( PPR_PFE_ACTIVE_RMKS( ACTIVE ,  /*  BPKT  */   \
                    INACTIVE,  /*  MAG8  */   \
                    ACTIVE ,  /*  MAG7  */   \
                    INACTIVE,  /*  MAG6  */   \
                    ACTIVE ,  /*  MAG5  */   \
                    INACTIVE,  /*  MAG4  */   \
                    ACTIVE ,  /*  MAG3  */   \
                    INACTIVE,  /*  MAG2  */   \
                    ACTIVE    /*  MAG1  */   \
                                       ) == 0x00000155  );

TTX_ASSERT_COMPILE( PPR_PFE_ACTIVE_RMKS(INACTIVE,  /*  BPKT  */   \
                                        ACTIVE ,  /*  MAG8  */   \
                                        INACTIVE,  /*  MAG7  */   \
                                        ACTIVE ,  /*  MAG6  */   \
                                        INACTIVE,  /*  MAG5  */   \
                                        INACTIVE,  /*  MAG4  */   \
                                        ACTIVE ,  /*  MAG3  */   \
                                        INACTIVE,  /*  MAG2  */   \
                                        ACTIVE    /*  MAG1  */   \
                                       ) == 0x000000A5  );

TTX_ASSERT_COMPILE( PPR_PFE_ACTIVE_RMKS( ACTIVE ,  /*  BPKT  */   \
                    INACTIVE,  /*  MAG8  */   \
                    ACTIVE ,  /*  MAG7  */   \
                    INACTIVE,  /*  MAG6  */   \
                    ACTIVE ,  /*  MAG5  */   \
                    ACTIVE ,  /*  MAG4  */   \
                    INACTIVE,  /*  MAG3  */   \
                    ACTIVE ,  /*  MAG2  */   \
                    INACTIVE   /*  MAG1  */   \
                                       ) == 0x0000015A  );

TTX_ASSERT_COMPILE( PPR_PFE_ACTIVE_RMKS( ACTIVE ,  /*  BPKT  */   \
                    INACTIVE,  /*  MAG8  */   \
                    INACTIVE,  /*  MAG7  */   \
                    INACTIVE,  /*  MAG6  */   \
                    INACTIVE,  /*  MAG5  */   \
                    INACTIVE,  /*  MAG4  */   \
                    INACTIVE,  /*  MAG3  */   \
                    ACTIVE ,  /*  MAG2  */   \
                    ACTIVE    /*  MAG1  */   \
                                       ) == 0x00000103  );


/* Test PPR_PFE_NPROC_ADDR_REG */
TTX_ASSERT_COMPILE( PPR_PFE_NPROC_ADDR_DEFAULT == 0x00000000 );

TTX_ASSERT_COMPILE( PPR_PFE_NPROC_ADDR_RMK(0x00  /* ADDR */   \
                                          ) == 0x00000000  );

TTX_ASSERT_COMPILE( PPR_PFE_NPROC_ADDR_RMK(0xFF  /* ADDR */   \
                                          ) == 0x000000FF  );

TTX_ASSERT_COMPILE( PPR_PFE_NPROC_ADDR_RMK(0x5A  /* ADDR */   \
                                          ) == 0x0000005A  );

TTX_ASSERT_COMPILE( PPR_PFE_NPROC_ADDR_RMK(0xA5  /* ADDR */   \
                                          ) == 0x000000A5  );



/* Test PPR_PFE_CTRL_REG */
TTX_ASSERT_COMPILE( PPR_PFE_CTRL_DEFAULT == 0x00000000 );

TTX_ASSERT_COMPILE( PPR_PFE_CTRL_RMKS(    NOP,    /*    VPS     */   \
                    NOP,    /*    CC      */   \
                    NOP,    /*    WSS     */   \
                    NOP,    /*   CGMS     */   \
                    NOP,    /*    830     */   \
                    DISABLE,  /* PARITY_CHK */   \
                    CLEARED,  /* MODE3_IND  */   \
                    DISABLE,  /* MODE3_EN   */   \
                    CLEARED,  /* MODE2_IND  */   \
                    DISABLE,  /* MODE2_EN   */   \
                    CLEARED,  /* MODE1_IND  */   \
                    DISABLE,  /* MODE1_EN   */   \
                    CLEARED,  /* MODE0_IND  */   \
                    DISABLE,  /* MODE0_EN   */   \
                    MULTI,    /*   PAGE     */   \
                    DISABLE   /*   START    */   \
                                     ) == 0x00000000  );

TTX_ASSERT_COMPILE( PPR_PFE_CTRL_RMKS(  RESET,    /*    VPS     */   \
                                        NOP,    /*    CC      */   \
                                        RESET,    /*    WSS     */   \
                                        NOP,    /*   CGMS     */   \
                                        RESET,    /*    830     */   \
                                        DISABLE,  /* PARITY_CHK */   \
                                        LEFT,    /* MODE3_IND  */   \
                                        DISABLE,  /* MODE3_EN   */   \
                                        LEFT,    /* MODE2_IND  */   \
                                        DISABLE,  /* MODE2_EN   */   \
                                        LEFT,    /* MODE1_IND  */   \
                                        DISABLE,  /* MODE1_EN   */   \
                                        LEFT,    /* MODE0_IND  */   \
                                        DISABLE,  /* MODE0_EN   */   \
                                        10,     /*   PAGE     */   \
                                        ENABLE    /*   START    */   \
                                     ) == 0x0000AAAB  );

TTX_ASSERT_COMPILE( PPR_PFE_CTRL_RMKS(    NOP,    /*    VPS     */   \
                    RESET,    /*    CC      */   \
                    NOP,    /*    WSS     */   \
                    RESET,    /*   CGMS     */   \
                    NOP,    /*    830     */   \
                    ENABLE,   /* PARITY_CHK */   \
                    CLEARED,  /* MODE3_IND  */   \
                    ENABLE,   /* MODE3_EN   */   \
                    CLEARED,  /* MODE2_IND  */   \
                    ENABLE,   /* MODE2_EN   */   \
                    CLEARED,  /* MODE1_IND  */   \
                    ENABLE,   /* MODE1_EN   */   \
                    LEFT,    /* MODE0_IND  */   \
                    DISABLE,  /* MODE0_EN   */   \
                    10,     /*   PAGE     */   \
                    DISABLE   /*   START    */   \
                                     ) == 0x0000555A  );


/* Test PPR_FB_PKTx_REG */
#define PPR_FB_PKTx_COMPILE_TIME_TEST(x)                                      \
    TTX_ASSERT_COMPILE( PPR_FB_PKT##x##_DEFAULT            == 0x00000000 );   \
    TTX_ASSERT_COMPILE( PPR_FB_PKT##x##_RMKS(   OK,    /*  DERR   */   \
                                                OK,    /*  HCERR  */   \
                                               0x0,    /*  DCODE  */   \
                                               0x00,   /* PKT_NUM */   \
                                               0x0     /* MAG_NUM */   \
                      ) == 0x00000000  );                                     \
    TTX_ASSERT_COMPILE( PPR_FB_PKT##x##_RMKS(ERROR,    /*  DERR   */   \
                                             ERROR,    /*  HCERR  */   \
                                               0xF,    /*  DCODE  */   \
                                               0x1F,   /* PKT_NUM */   \
                                               0xF     /* MAG_NUM */   \
                      ) == 0x00003FFF  );                                     \
    TTX_ASSERT_COMPILE( PPR_FB_PKT##x##_RMKS(ERROR,    /*  DERR   */   \
                                                OK,    /*  HCERR  */   \
                                               0x8,    /*  DCODE  */   \
                                               0x1C,   /* PKT_NUM */   \
                                               0x5     /* MAG_NUM */   \
                      ) == 0x000028E5  );                                     \
    TTX_ASSERT_COMPILE( PPR_FB_PKT##x##_RMKS(   OK,    /*  DERR   */   \
                                             ERROR,    /*  HCERR  */   \
                                               0xC,    /*  DCODE  */   \
                                               0x1F,   /* PKT_NUM */   \
                                               0x3     /* MAG_NUM */   \
                      ) == 0x00001CFB  )

PPR_FB_PKTx_COMPILE_TIME_TEST( 1);
PPR_FB_PKTx_COMPILE_TIME_TEST( 2);
PPR_FB_PKTx_COMPILE_TIME_TEST( 3);
PPR_FB_PKTx_COMPILE_TIME_TEST( 4);
PPR_FB_PKTx_COMPILE_TIME_TEST( 5);
PPR_FB_PKTx_COMPILE_TIME_TEST( 6);
PPR_FB_PKTx_COMPILE_TIME_TEST( 7);
PPR_FB_PKTx_COMPILE_TIME_TEST( 8);
PPR_FB_PKTx_COMPILE_TIME_TEST( 9);
PPR_FB_PKTx_COMPILE_TIME_TEST(10);
PPR_FB_PKTx_COMPILE_TIME_TEST(11);
PPR_FB_PKTx_COMPILE_TIME_TEST(12);
PPR_FB_PKTx_COMPILE_TIME_TEST(13);
PPR_FB_PKTx_COMPILE_TIME_TEST(14);
PPR_FB_PKTx_COMPILE_TIME_TEST(15);
PPR_FB_PKTx_COMPILE_TIME_TEST(16);
PPR_FB_PKTx_COMPILE_TIME_TEST(17);
PPR_FB_PKTx_COMPILE_TIME_TEST(18);





/*-------------------------------------------------------------------------*\
 * End of ppr_hal_test.h                                                   *
\*-------------------------------------------------------------------------*/

