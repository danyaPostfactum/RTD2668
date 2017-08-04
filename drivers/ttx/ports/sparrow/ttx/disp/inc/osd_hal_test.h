/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     osd_hal_test.h
 * @brief    Compile-time tests for OSD HAL.
 * @details  Include this file to test CCL. No obj code will be generated.
 *             This file to be included in private CPP file.
 * @author   Janus Cheng
 * @date     Aug. 21, 2008
 * @version  0.1
\*****************************************************************************/

#include "osd_hal.h"                /* Provides: OSD HAL macros to be tested */
#include "../../inc/ttx_assert.h"   /* Provides: TTX_ASSERT_COMPILE() */


/* Test OSD_CTRL1_REG */
TTX_ASSERT_COMPILE( OSD_CTRL1_DEFAULT        == 0x00000000 );

TTX_ASSERT_COMPILE( _CCL_PER_FINSERT( OSD, CTRL1, VTYPE, 0x80000000, \
                                      _CCL_PER_FSYM(OSD, CTRL1, VTYPE, INTL) ) == 0x80000200 );

TTX_ASSERT_COMPILE( _CCL_PER_FINSERT( OSD, CTRL1, VTYPE, 0x8ABCD2EF, \
                                      _CCL_PER_FSYM(OSD, CTRL1, VTYPE, INTL) ) == 0x8ABCD2EF );

TTX_ASSERT_COMPILE( OSD_CTRL1_RMKS(DISABLE,  /*  PATEN  */ \
                                   DISABLE,  /*  UVTOG  */ \
                                   DISABLE,  /*   LPF   */ \
                                   VIDEO  ,  /* TPCSEL  */ \
                                   0x00   ,  /*  PXDLY  */ \
                                   DISABLE,  /* FLDTOG  */ \
                                   DISABLE,  /* CCF2PH  */ \
                                   DISABLE,  /* FRINGEE */ \
                                   ES     ,  /*  FMODE  */ \
                                   DISABLE,  /* CCSCUT  */ \
                                   PROG   ,  /*  VTYPE  */ \
                                   1ST    ,  /* DLTSEL  */ \
                                   SD     ,  /*  WDDR   */ \
                                   DISABLE,  /* TTR25E  */ \
                                   DISABLE,  /* IDXOUT  */ \
                                   1P     ,  /* TTMODE  */ \
                                   0      ,  /*  CMODE  */ \
                                   TT     ,  /*  OTYPE  */ \
                                   STD    ,  /*  OMODE  */ \
                                   DISABLE   /*    OE   */ \
                                  ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_CTRL1_RMKS(ENABLE ,  /*  PATEN  */ \
                                   DISABLE,  /*  UVTOG  */ \
                                   DISABLE,  /*   LPF   */ \
                                   VIDEO  ,  /* TPCSEL  */ \
                                   0x00   ,  /*  PXDLY  */ \
                                   DISABLE,  /* FLDTOG  */ \
                                   DISABLE,  /* CCF2PH  */ \
                                   DISABLE,  /* FRINGEE */ \
                                   ES     ,  /*  FMODE  */ \
                                   DISABLE,  /* CCSCUT  */ \
                                   PROG   ,  /*  VTYPE  */ \
                                   1ST    ,  /* DLTSEL  */ \
                                   SD     ,  /*  WDDR   */ \
                                   DISABLE,  /* TTR25E  */ \
                                   DISABLE,  /* IDXOUT  */ \
                                   1P     ,  /* TTMODE  */ \
                                   0      ,  /*  CMODE  */ \
                                   TT     ,  /*  OTYPE  */ \
                                   STD    ,  /*  OMODE  */ \
                                   DISABLE   /*    OE   */ \
                                  ) == 0x80000000 );

TTX_ASSERT_COMPILE( OSD_CTRL1_RMKS(ENABLE ,  /*  PATEN  */ \
                                   ENABLE ,  /*  UVTOG  */ \
                                   ENABLE ,  /*   LPF   */ \
                                   GRAY   ,  /* TPCSEL  */ \
                                   0xFF   ,  /*  PXDLY  */ \
                                   ENABLE ,  /* FLDTOG  */ \
                                   ENABLE ,  /* CCF2PH  */ \
                                   ENABLE ,  /* FRINGEE */ \
                                   EWSN   ,  /*  FMODE  */ \
                                   ENABLE ,  /* CCSCUT  */ \
                                   INTL   ,  /*  VTYPE  */ \
                                   2ND    ,  /* DLTSEL  */ \
                                   NOSD   ,  /*  WDDR   */ \
                                   ENABLE ,  /* TTR25E  */ \
                                   ENABLE ,  /* IDXOUT  */ \
                                   2P     ,  /* TTMODE  */ \
                                   1      ,  /*  CMODE  */ \
                                   CC     ,  /*  OTYPE  */ \
                                   MIX    ,  /*  OMODE  */ \
                                   ENABLE    /*    OE   */ \
                                  ) == 0xFFF747FF );

TTX_ASSERT_COMPILE( OSD_CTRL1_RMKS(ENABLE ,  /*  PATEN  */ \
                                   DISABLE,  /*  UVTOG  */ \
                                   ENABLE ,  /*   LPF   */ \
                                   VIDEO  ,  /* TPCSEL  */ \
                                   0x5A   ,  /*  PXDLY  */ \
                                   ENABLE ,  /* FLDTOG  */ \
                                   DISABLE,  /* CCF2PH  */ \
                                   ENABLE ,  /* FRINGEE */ \
                                   EN     ,  /*  FMODE  */ \
                                   DISABLE,  /* CCSCUT  */ \
                                   INTL   ,  /*  VTYPE  */ \
                                   1ST    ,  /* DLTSEL  */ \
                                   NOSD   ,  /*  WDDR   */ \
                                   DISABLE,  /* TTR25E  */ \
                                   ENABLE ,  /* IDXOUT  */ \
                                   1P     ,  /* TTMODE  */ \
                                   1      ,  /*  CMODE  */ \
                                   TT     ,  /*  OTYPE  */ \
                                   MIX    ,  /*  OMODE  */ \
                                   DISABLE   /*    OE   */ \
                                  ) == 0xA5A512AA );

TTX_ASSERT_COMPILE( OSD_CTRL1_RMKS(DISABLE,  /*  PATEN  */ \
                                   ENABLE ,  /*  UVTOG  */ \
                                   DISABLE,  /*   LPF   */ \
                                   GRAY   ,  /* TPCSEL  */ \
                                   0xA5   ,  /*  PXDLY  */ \
                                   DISABLE,  /* FLDTOG  */ \
                                   ENABLE ,  /* CCF2PH  */ \
                                   DISABLE,  /* FRINGEE */ \
                                   WN     ,  /*  FMODE  */ \
                                   DISABLE,  /* CCSCUT  */ \
                                   PROG   ,  /*  VTYPE  */ \
                                   2ND    ,  /* DLTSEL  */ \
                                   SD     ,  /*  WDDR   */ \
                                   ENABLE ,  /* TTR25E  */ \
                                   DISABLE,  /* IDXOUT  */ \
                                   2P     ,  /* TTMODE  */ \
                                   0      ,  /*  CMODE  */ \
                                   CC     ,  /*  OTYPE  */ \
                                   STD    ,  /*  OMODE  */ \
                                   ENABLE    /*    OE   */ \
                                  ) == 0x5A523155 );


/* Test OSD_CTRL2_REG */
TTX_ASSERT_COMPILE( OSD_CTRL2_DEFAULT        == 0x08610249 );

TTX_ASSERT_COMPILE( OSD_CTRL2_RMK(0x000,   /* MBM */  \
                                  0x000    /* MLM */  \
                                 ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_CTRL2_RMK(0xFFF,   /* MBM */  \
                                  0x3FF    /* MLM */  \
                                 ) == 0x0FFF03FF );

TTX_ASSERT_COMPILE( OSD_CTRL2_RMK(0xABC,   /* MBM */  \
                                  0x2DE    /* MLM */  \
                                 ) == 0x0ABC02DE );

TTX_ASSERT_COMPILE( OSD_CTRL2_RMK(0xA5A,   /* MBM */  \
                                  0x5A5    /* MLM */  \
                                 ) == 0x0A5A01A5 );

TTX_ASSERT_COMPILE( OSD_CTRL2_RMK(0x5A5,   /* MBM */  \
                                  0xA5A    /* MLM */  \
                                 ) == 0x05A5025A );

TTX_ASSERT_COMPILE( OSD_CTRL2_RMK(0x0F0,   /* MBM */  \
                                  0xF0F    /* MLM */  \
                                 ) == 0x00F0030F );

TTX_ASSERT_COMPILE( OSD_CTRL2_RMK(0xF0F,   /* MBM */  \
                                  0x0F0    /* MLM */  \
                                 ) == 0x0F0F00F0 );


/* Test OSD_CTRL3_REG */
TTX_ASSERT_COMPILE( OSD_CTRL3_DEFAULT        == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_CTRL3_RMKS(DISABLE,   /*  SMEN  */  \
                                   ENABLE ,   /* CNCDIS */  \
                                   MODE0  ,   /*  TTVS  */  \
                                   MODE0  ,   /*  VSR   */  \
                                   0      ,   /*  VDLY  */  \
                                   0          /*  HDLY1 */  \
                                  ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_CTRL3_RMKS(ENABLE ,   /*  SMEN  */  \
                                   DISABLE,   /* CNCDIS */  \
                                   MODE1  ,   /*  TTVS  */  \
                                   MODE2  ,   /*  VSR   */  \
                                   511    ,   /*  VDLY  */  \
                                   255        /*  HDLY1 */  \
                                  ) == 0xE201FFFF );

TTX_ASSERT_COMPILE( OSD_CTRL3_RMKS(DISABLE,   /*  SMEN  */  \
                                   DISABLE,   /* CNCDIS */  \
                                   MODE0  ,   /*  TTVS  */  \
                                   MODE1  ,   /*  VSR   */  \
                                   427    ,   /*  VDLY  */  \
                                   205        /*  HDLY1 */  \
                                  ) == 0x4101ABCD );

TTX_ASSERT_COMPILE( OSD_CTRL3_RMKS(ENABLE ,   /*  SMEN  */  \
                                   ENABLE ,   /* CNCDIS */  \
                                   MODE0  ,   /*  TTVS  */  \
                                   MODE1  ,   /*  VSR   */  \
                                   427    ,   /*  VDLY  */  \
                                   205        /*  HDLY1 */  \
                                  ) == 0x8101ABCD );


/* Test OSD_CTRL4_REG */
TTX_ASSERT_COMPILE( OSD_CTRL4_DEFAULT        == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_CTRL4_RMK(   0,     /*  HDLY2 */  \
                                     0      /*  HDLY3 */  \
                                 ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_CTRL4_RMK( 511,     /*  HDLY2 */  \
                                   511      /*  HDLY3 */  \
                                 ) == 0x01FF01FF );

TTX_ASSERT_COMPILE( OSD_CTRL4_RMK( 427,     /*  HDLY2 */  \
                                   205      /*  HDLY3 */  \
                                 ) == 0x01AB00CD );

TTX_ASSERT_COMPILE( OSD_CTRL4_RMK( 205,     /*  HDLY2 */  \
                                   427      /*  HDLY3 */  \
                                 ) == 0x00CD01AB );


/* Test OSD_CTRL5_REG */
TTX_ASSERT_COMPILE( OSD_CTRL5_DEFAULT        == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_CTRL5_RMKS(DISABLE ,   /*  HDREXT  */  \
                                   DISABLE ,   /*  HDDWEN  */  \
                                   DISABLE ,   /*  HDDHEN  */  \
                                   MICRONAS,   /*  HDDWMD  */  \
                                   DISABLE ,   /*  HDPNSP  */  \
                                   DISABLE ,   /*  DHCMDEN */  \
                                   DISABLE ,   /*  DWCMDEN */  \
                                   DISABLE ,   /*  DSCMDEN */  \
                                   0      ,   /*  HDG1CS  */  \
                                   0          /*  HDG0CS  */  \
                                  ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_CTRL5_RMKS(ENABLE  ,   /*  HDREXT  */  \
                                   ENABLE  ,   /*  HDDWEN  */  \
                                   ENABLE  ,   /*  HDDHEN  */  \
                                   PHILIPS ,   /*  HDDWMD  */  \
                                   ENABLE  ,   /*  HDPNSP  */  \
                                   ENABLE  ,   /*  DHCMDEN */  \
                                   ENABLE  ,   /*  DWCMDEN */  \
                                   ENABLE  ,   /*  DSCMDEN */  \
                                   0x1F    ,   /*  HDG1CS  */  \
                                   0x1F        /*  HDG0CS  */  \
                                  ) == 0xFF001F1F );

TTX_ASSERT_COMPILE( OSD_CTRL5_RMKS(ENABLE  ,   /*  HDREXT  */  \
                                   DISABLE ,   /*  HDDWEN  */  \
                                   ENABLE  ,   /*  HDDHEN  */  \
                                   MICRONAS,   /*  HDDWMD  */  \
                                   DISABLE ,   /*  HDPNSP  */  \
                                   ENABLE  ,   /*  DHCMDEN */  \
                                   DISABLE ,   /*  DWCMDEN */  \
                                   ENABLE  ,   /*  DSCMDEN */  \
                                   0x12    ,   /*  HDG1CS  */  \
                                   0x34        /*  HDG0CS  */  \
                                  ) == 0xA5001214 );

TTX_ASSERT_COMPILE( OSD_CTRL5_RMKS(DISABLE ,   /*  HDREXT  */  \
                                   ENABLE  ,   /*  HDDWEN  */  \
                                   DISABLE ,   /*  HDDHEN  */  \
                                   PHILIPS ,   /*  HDDWMD  */  \
                                   ENABLE  ,   /*  HDPNSP  */  \
                                   DISABLE ,   /*  DHCMDEN */  \
                                   ENABLE  ,   /*  DWCMDEN */  \
                                   DISABLE ,   /*  DSCMDEN */  \
                                   0x45    ,   /*  HDG1CS  */  \
                                   0x37        /*  HDG0CS  */  \
                                  ) == 0x5A000517 );


/* Test OSD_CSBMPST_REG */
TTX_ASSERT_COMPILE( OSD_CSBMPST_DEFAULT        == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_CSBMPST_RMK(0xF0000000  /* CSBMPST  */  \
                                   ) == 0x10000000 );

TTX_ASSERT_COMPILE( OSD_CSBMPST_RMK(0x5A5A5A5A  /* CSBMPST  */  \
                                   ) == 0x1A5A5A5A );

TTX_ASSERT_COMPILE( OSD_CSBMPST_RMK(0xA5A5A5A5  /* CSBMPST  */  \
                                   ) == 0x05A5A5A5 );

TTX_ASSERT_COMPILE( OSD_CSBMPST_RMK(0xFFFFFFFF  /* CSBMPST  */  \
                                   ) == 0x1FFFFFFF );

TTX_ASSERT_COMPILE( OSD_CSBMPST_RMK(0x87654321  /* CSBMPST  */  \
                                   ) == 0x07654321 );

TTX_ASSERT_COMPILE( OSD_CSBMPST_RMK(0xFEDCBA09  /* CSBMPST  */  \
                                   ) == 0x1EDCBA09 );


/* Test OSD_ATTST1_REG */
TTX_ASSERT_COMPILE( OSD_ATTST1_DEFAULT        == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_ATTST1_RMK(0xF0000000   /* ATTST1  */  \
                                  ) == 0x10000000 );

TTX_ASSERT_COMPILE( OSD_ATTST1_RMK(0x5A5A5A5A   /* ATTST1  */  \
                                  ) == 0x1A5A5A5A );

TTX_ASSERT_COMPILE( OSD_ATTST1_RMK(0xA5A5A5A5   /* ATTST1  */  \
                                  ) == 0x05A5A5A5 );

TTX_ASSERT_COMPILE( OSD_ATTST1_RMK(0xFFFFFFFF   /* ATTST1  */  \
                                  ) == 0x1FFFFFFF );

TTX_ASSERT_COMPILE( OSD_ATTST1_RMK(0x87654321   /* ATTST1  */  \
                                  ) == 0x07654321 );

TTX_ASSERT_COMPILE( OSD_ATTST1_RMK(0xFEDCBA09   /* ATTST1  */  \
                                  ) == 0x1EDCBA09 );

/* Test OSD_ATTST2_REG */
TTX_ASSERT_COMPILE( OSD_ATTST2_DEFAULT        == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_ATTST2_RMK(0xF0000000   /* ATTST2  */  \
                                  ) == 0x10000000 );

TTX_ASSERT_COMPILE( OSD_ATTST2_RMK(0x5A5A5A5A   /* ATTST2  */  \
                                  ) == 0x1A5A5A5A );

TTX_ASSERT_COMPILE( OSD_ATTST2_RMK(0xA5A5A5A5   /* ATTST2  */  \
                                  ) == 0x05A5A5A5 );

TTX_ASSERT_COMPILE( OSD_ATTST2_RMK(0xFFFFFFFF   /* ATTST2  */  \
                                  ) == 0x1FFFFFFF );

TTX_ASSERT_COMPILE( OSD_ATTST2_RMK(0x87654321   /* ATTST2  */  \
                                  ) == 0x07654321 );

TTX_ASSERT_COMPILE( OSD_ATTST2_RMK(0xFEDCBA09   /* ATTST2  */  \
                                  ) == 0x1EDCBA09 );

/* Test OSD_UPDATE_REG */
TTX_ASSERT_COMPILE( OSD_UPDATE_DEFAULT        == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_UPDATE_RMKS(NORMAL,     /*  GRU    */ \
                                    NORMAL      /* TTP1RU  */ \
                                   ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_UPDATE_RMKS(NOTIFY,     /*  GRU    */ \
                                    NORMAL      /* TTP1RU  */ \
                                   ) == 0x80000000 );

TTX_ASSERT_COMPILE( OSD_UPDATE_RMKS(NORMAL,     /*  GRU    */ \
                                    NOTIFY      /* TTP1RU  */ \
                                   ) == 0x40000000 );

TTX_ASSERT_COMPILE( OSD_UPDATE_RMKS(NOTIFY,     /*  GRU    */ \
                                    NOTIFY      /* TTP1RU  */ \
                                   ) == 0xC0000000 );


/* Test OSD_INTERRUPT_REG */
TTX_ASSERT_COMPILE( OSD_INTERRUPT_DEFAULT        == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_INTERRUPT_RMKS(  NOP   ,     /*  HSIDLES */ \
                    STOP  ,     /*  SCROLS  */ \
                    NOP   ,     /*   RURIE  */ \
                    NOP   ,     /*   RURID  */ \
                    NOP   ,     /*   RURIS  */ \
                    NOP   ,     /*  VSYNCIE */ \
                    NOP   ,     /*  VSYNCID */ \
                    NOP         /*  VSYNCIS */ \
                                      ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_INTERRUPT_RMKS(ASSERTED,     /*  HSIDLES */ \
                                       STOP  ,     /*  SCROLS  */ \
                                       ENABLE ,     /*   RURIE  */ \
                                       NOP   ,     /*   RURID  */ \
                                       ASSERTED,     /*   RURIS  */ \
                                       NOP   ,     /*  VSYNCIE */ \
                                       DISABLE ,     /*  VSYNCID */ \
                                       NOP         /*  VSYNCIS */ \
                                      ) == 0x8000002A );

TTX_ASSERT_COMPILE( OSD_INTERRUPT_RMKS(  NOP   ,     /*  HSIDLES */ \
                    START ,     /*  SCROLS  */ \
                    NOP   ,     /*   RURIE  */ \
                    DISABLE,     /*   RURID  */ \
                    NOP   ,     /*   RURIS  */ \
                    ENABLE ,     /*  VSYNCIE */ \
                    NOP   ,     /*  VSYNCID */ \
                    ASSERTED      /*  VSYNCIS */ \
                                      ) == 0x40000015 );

TTX_ASSERT_COMPILE( OSD_INTERRUPT_RMKS(  NOP   ,     /*  HSIDLES */ \
                    STOP  ,     /*  SCROLS  */ \
                    NOP   ,     /*   RURIE  */ \
                    NOP   ,     /*   RURID  */ \
                    NOP   ,     /*   RURIS  */ \
                    NOP   ,     /*  VSYNCIE */ \
                    NOP   ,     /*  VSYNCID */ \
                    ASSERTED      /*  VSYNCIS */ \
                                      ) == 0x00000001 );

TTX_ASSERT_COMPILE( OSD_INTERRUPT_RMKS(  NOP   ,     /*  HSIDLES */ \
                    STOP  ,     /*  SCROLS  */ \
                    NOP   ,     /*   RURIE  */ \
                    NOP   ,     /*   RURID  */ \
                    NOP   ,     /*   RURIS  */ \
                    NOP   ,     /*  VSYNCIE */ \
                    DISABLE,     /*  VSYNCID */ \
                    NOP         /*  VSYNCIS */ \
                                      ) == 0x00000002 );

TTX_ASSERT_COMPILE( OSD_INTERRUPT_RMKS(  NOP   ,     /*  HSIDLES */ \
                    STOP  ,     /*  SCROLS  */ \
                    NOP   ,     /*   RURIE  */ \
                    NOP   ,     /*   RURID  */ \
                    NOP   ,     /*   RURIS  */ \
                    ENABLE,     /*  VSYNCIE */ \
                    NOP   ,     /*  VSYNCID */ \
                    NOP         /*  VSYNCIS */ \
                                      ) == 0x00000004 );

TTX_ASSERT_COMPILE( OSD_INTERRUPT_RMKS(  NOP   ,     /*  HSIDLES */ \
                    STOP  ,     /*  SCROLS  */ \
                    NOP   ,     /*   RURIE  */ \
                    NOP   ,     /*   RURID  */ \
                    ASSERTED,     /*   RURIS  */ \
                    NOP   ,     /*  VSYNCIE */ \
                    NOP   ,     /*  VSYNCID */ \
                    NOP         /*  VSYNCIS */ \
                                      ) == 0x00000008 );

TTX_ASSERT_COMPILE( OSD_INTERRUPT_RMKS(  NOP   ,     /*  HSIDLES */ \
                    STOP  ,     /*  SCROLS  */ \
                    NOP   ,     /*   RURIE  */ \
                    DISABLE,     /*   RURID  */ \
                    NOP   ,     /*   RURIS  */ \
                    NOP   ,     /*  VSYNCIE */ \
                    NOP   ,     /*  VSYNCID */ \
                    NOP         /*  VSYNCIS */ \
                                      ) == 0x00000010 );

TTX_ASSERT_COMPILE( OSD_INTERRUPT_RMKS(  NOP   ,     /*  HSIDLES */ \
                    STOP  ,     /*  SCROLS  */ \
                    ENABLE ,     /*   RURIE  */ \
                    NOP   ,     /*   RURID  */ \
                    NOP   ,     /*   RURIS  */ \
                    NOP   ,     /*  VSYNCIE */ \
                    NOP   ,     /*  VSYNCID */ \
                    NOP         /*  VSYNCIS */ \
                                      ) == 0x00000020 );

TTX_ASSERT_COMPILE( OSD_INTERRUPT_RMKS(  NOP   ,     /*  HSIDLES */ \
                    START  ,     /*  SCROLS  */ \
                    NOP   ,     /*   RURIE  */ \
                    NOP   ,     /*   RURID  */ \
                    NOP   ,     /*   RURIS  */ \
                    NOP   ,     /*  VSYNCIE */ \
                    NOP   ,     /*  VSYNCID */ \
                    NOP         /*  VSYNCIS */ \
                                      ) == 0x40000000 );

TTX_ASSERT_COMPILE( OSD_INTERRUPT_RMKS(ASSERTED,     /*  HSIDLES */ \
                                       STOP  ,     /*  SCROLS  */ \
                                       NOP   ,     /*   RURIE  */ \
                                       NOP   ,     /*   RURID  */ \
                                       NOP   ,     /*   RURIS  */ \
                                       NOP   ,     /*  VSYNCIE */ \
                                       NOP   ,     /*  VSYNCID */ \
                                       NOP         /*  VSYNCIS */ \
                                      ) == 0x80000000 );

/* Test OSD_CRCDAT_REG */
TTX_ASSERT_COMPILE( OSD_CRCDAT_DEFAULT        == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_CRCDAT_RMK(0x00000000  /* CRCDAT */ \
                                  ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_CRCDAT_RMK(0xFFFFFFFF  /* CRCDAT */ \
                                  ) == 0xFFFFFFFF );

TTX_ASSERT_COMPILE( OSD_CRCDAT_RMK(0x5A5AA5A5  /* CRCDAT */ \
                                  ) == 0x5A5AA5A5 );

TTX_ASSERT_COMPILE( OSD_CRCDAT_RMK(0xA5A55A5A  /* CRCDAT */ \
                                  ) == 0xA5A55A5A );

TTX_ASSERT_COMPILE( OSD_CRCDAT_RMK(0x12345678  /* CRCDAT */ \
                                  ) == 0x12345678 );

TTX_ASSERT_COMPILE( OSD_CRCDAT_RMK(0xABCDEF12  /* CRCDAT */ \
                                  ) == 0xABCDEF12 );

/* Test OSD_P1CTRL_REG */
TTX_ASSERT_COMPILE( OSD_P1CTRL_DEFAULT        == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1CTRL_RMKS(WINDOW ,     /*   BOX   */ \
                                    OFF  ,     /*   SPO   */ \
                                    0   ,     /*  LSPNUM */ \
                                    0   ,     /*   SCT   */ \
                                    0   ,     /*   SCE   */ \
                                    1OVER2 ,     /*   HSR   */ \
                                    DISABLE,     /*  RHDE   */ \
                                    DISABLE,     /*  TCDE   */ \
                                    ENABLE       /*    DE   */ \
                                   ) == 0x00000011 );

TTX_ASSERT_COMPILE( OSD_P1CTRL_RMKS(WINDOW ,     /*   BOX   */ \
                                    ON   ,     /*   SPO   */ \
                                    15   ,     /*  LSPNUM */ \
                                    2   ,     /*   SCT   */ \
                                    4   ,     /*   SCE   */ \
                                    7OVER5 ,     /*   HSR   */ \
                                    DISABLE,     /*  RHDE   */ \
                                    ENABLE ,     /*  TCDE   */ \
                                    DISABLE      /*    DE   */ \
                                   ) == 0x4F001424 );

TTX_ASSERT_COMPILE( OSD_P1CTRL_RMKS(  BOX  ,     /*   BOX   */ \
                                      ON   ,     /*   SPO   */ \
                                      15   ,     /*  LSPNUM */ \
                                      2   ,     /*   SCT   */ \
                                      4   ,     /*   SCE   */ \
                                      7OVER10,     /*   HSR   */ \
                                      DISABLE,     /*  RHDE   */ \
                                      ENABLE ,     /*  TCDE   */ \
                                      DISABLE      /*    DE   */ \
                                   ) == 0xCF001434 );

TTX_ASSERT_COMPILE( OSD_P1CTRL_RMKS(  BOX  ,     /*   BOX   */ \
                                      ON   ,     /*   SPO   */ \
                                      13   ,     /*  LSPNUM */ \
                                      1   ,     /*   SCT   */ \
                                      7   ,     /*   SCE   */ \
                                      7OVER5 ,     /*   HSR   */ \
                                      ENABLE ,     /*  RHDE   */ \
                                      DISABLE,     /*  TCDE   */ \
                                      ENABLE       /*    DE   */ \
                                   ) == 0xCD000F29 );

TTX_ASSERT_COMPILE( OSD_P1CTRL_RMKS(WINDOW ,     /*   BOX   */ \
                                    ON   ,     /*   SPO   */ \
                                    1   ,     /*  LSPNUM */ \
                                    3   ,     /*   SCT   */ \
                                    1   ,     /*   SCE   */ \
                                    1   ,     /*   HSR   */ \
                                    ENABLE ,     /*  RHDE   */ \
                                    ENABLE ,     /*  TCDE   */ \
                                    ENABLE       /*    DE   */ \
                                   ) == 0x4100190D );

TTX_ASSERT_COMPILE( OSD_P1CTRL_RMKS(WINDOW ,     /*   BOX   */ \
                                    OFF  ,     /*   SPO   */ \
                                    1   ,     /*  LSPNUM */ \
                                    3   ,     /*   SCT   */ \
                                    1   ,     /*   SCE   */ \
                                    1   ,     /*   HSR   */ \
                                    ENABLE ,     /*  RHDE   */ \
                                    ENABLE ,     /*  TCDE   */ \
                                    ENABLE       /*    DE   */ \
                                   ) == 0x0100190D );


/* Test OSD_P1_PARA1_REG */
TTX_ASSERT_COMPILE( OSD_P1_PARA1_DEFAULT      == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1_PARA1_RMK(0x00,   /*  R0DC  */ \
                                     0x00,   /*  R1DC  */ \
                                     0x00,   /*  R2DC  */ \
                                     0x00    /*  R3DC  */ \
                                    ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1_PARA1_RMK(0x1F,   /*  R0DC  */ \
                                     0x1F,   /*  R1DC  */ \
                                     0x1F,   /*  R2DC  */ \
                                     0x1F    /*  R3DC  */ \
                                    ) == 0x1F1F1F1F );

TTX_ASSERT_COMPILE( OSD_P1_PARA1_RMK(0x12,   /*  R0DC  */ \
                                     0x34,   /*  R1DC  */ \
                                     0x56,   /*  R2DC  */ \
                                     0x78    /*  R3DC  */ \
                                    ) == 0x12141618 );


/* Test OSD_P1_PARA2_REG */
TTX_ASSERT_COMPILE( OSD_P1_PARA2_DEFAULT      == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1_PARA2_RMK(0x00,   /*  R4DC  */ \
                                     0x00,   /*  R5DC  */ \
                                     0x00,   /*  R6DC  */ \
                                     0x00    /*  R7DC  */ \
                                    ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1_PARA2_RMK(0x1F,   /*  R4DC  */ \
                                     0x1F,   /*  R5DC  */ \
                                     0x1F,   /*  R6DC  */ \
                                     0x1F    /*  R7DC  */ \
                                    ) == 0x1F1F1F1F );

TTX_ASSERT_COMPILE( OSD_P1_PARA2_RMK(0x12,   /*  R4DC  */ \
                                     0x34,   /*  R5DC  */ \
                                     0x56,   /*  R6DC  */ \
                                     0x78    /*  R7DC  */ \
                                    ) == 0x12141618 );


/* Test OSD_P1_PARA3_REG */
TTX_ASSERT_COMPILE( OSD_P1_PARA3_DEFAULT      == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1_PARA3_RMK(0x00,   /*  R8DC  */ \
                                     0x00,   /*  R9DC  */ \
                                     0x00,   /*  R10DC */ \
                                     0x00    /*  R11DC */ \
                                    ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1_PARA3_RMK(0x1F,   /*  R8DC  */ \
                                     0x1F,   /*  R9DC  */ \
                                     0x1F,   /*  R10DC */ \
                                     0x1F    /*  R11DC */ \
                                    ) == 0x1F1F1F1F );

TTX_ASSERT_COMPILE( OSD_P1_PARA3_RMK(0x12,   /*  R8DC  */ \
                                     0x34,   /*  R9DC  */ \
                                     0x56,   /*  R10DC */ \
                                     0x78    /*  R11DC */ \
                                    ) == 0x12141618 );

/* Test OSD_P1_PARA4_REG */
TTX_ASSERT_COMPILE( OSD_P1_PARA4_DEFAULT      == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1_PARA4_RMK(0x00,   /*  R12DC */ \
                                     0x00,   /*  R13DC */ \
                                     0x00,   /*  R14DC */ \
                                     0x00    /*  R15DC */ \
                                    ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1_PARA4_RMK(0x1F,   /*  R12DC */ \
                                     0x1F,   /*  R13DC */ \
                                     0x1F,   /*  R14DC */ \
                                     0x1F    /*  R15DC */ \
                                    ) == 0x1F1F1F1F );

TTX_ASSERT_COMPILE( OSD_P1_PARA4_RMK(0x12,   /*  R12DC */ \
                                     0x34,   /*  R13DC */ \
                                     0x56,   /*  R14DC */ \
                                     0x78    /*  R15DC */ \
                                    ) == 0x12141618 );


/* Test OSD_P1_PARA5_REG */
TTX_ASSERT_COMPILE( OSD_P1_PARA5_DEFAULT      == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1_PARA5_RMK(0x00,   /*  R16DC */ \
                                     0x00,   /*  R17DC */ \
                                     0x00,   /*  R18DC */ \
                                     0x00    /*  R19DC */ \
                                    ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1_PARA5_RMK(0x1F,   /*  R16DC */ \
                                     0x1F,   /*  R17DC */ \
                                     0x1F,   /*  R18DC */ \
                                     0x1F    /*  R19DC */ \
                                    ) == 0x1F1F1F1F );

TTX_ASSERT_COMPILE( OSD_P1_PARA5_RMK(0x12,   /*  R16DC */ \
                                     0x34,   /*  R17DC */ \
                                     0x56,   /*  R18DC */ \
                                     0x78    /*  R19DC */ \
                                    ) == 0x12141618 );



/* Test OSD_P1_PARA6_REG */
TTX_ASSERT_COMPILE( OSD_P1_PARA6_DEFAULT      == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1_PARA6_RMK(0x00,   /*  R20DC */ \
                                     0x00,   /*  R21DC */ \
                                     0x00,   /*  R22DC */ \
                                     0x00    /*  R23DC */ \
                                    ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1_PARA6_RMK(0x1F,   /*  R20DC */ \
                                     0x1F,   /*  R21DC */ \
                                     0x1F,   /*  R22DC */ \
                                     0x1F    /*  R23DC */ \
                                    ) == 0x1F1F1F1F );

TTX_ASSERT_COMPILE( OSD_P1_PARA6_RMK(0x12,   /*  R20DC */ \
                                     0x34,   /*  R21DC */ \
                                     0x56,   /*  R22DC */ \
                                     0x78    /*  R23DC */ \
                                    ) == 0x12141618 );


/* Test OSD_P1_PARA7_REG */
TTX_ASSERT_COMPILE( OSD_P1_PARA7_DEFAULT      == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1_PARA7_RMK(0x00,   /*  R24DC */ \
                                     0x00    /*  R25DC */ \
                                    ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1_PARA7_RMK(0x1F,   /*  R24DC */ \
                                     0x1F    /*  R25DC */ \
                                    ) == 0x1F1F0000 );

TTX_ASSERT_COMPILE( OSD_P1_PARA7_RMK(0x12,   /*  R24DC */ \
                                     0x78    /*  R25DC */ \
                                    ) == 0x12180000 );



/* Test OSD_P1PNUM_CTRL_REG */
TTX_ASSERT_COMPILE( OSD_P1PNUM_CTRL_DEFAULT      == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1PNUM_CTRL_RMKS(0x1F ,   /*   FGC   */ \
                    0x1F ,   /*   BGC   */ \
                    0x1F ,   /*   CS    */ \
                    ENABLE   /*   DE    */ \
                                        ) == 0x1F1F1F01 );

TTX_ASSERT_COMPILE( OSD_P1PNUM_CTRL_RMKS(0x1F ,   /*   FGC   */ \
                    0x1F ,   /*   BGC   */ \
                    0x1F ,   /*   CS    */ \
                    DISABLE  /*   DE    */ \
                                        ) == 0x1F1F1F00 );

TTX_ASSERT_COMPILE( OSD_P1PNUM_CTRL_RMKS(0x15 ,   /*   FGC   */ \
                    0x05 ,   /*   BGC   */ \
                    0x0A ,   /*   CS    */ \
                    DISABLE  /*   DE    */ \
                                        ) == 0x15050A00 );

TTX_ASSERT_COMPILE( OSD_P1PNUM_CTRL_RMKS(0x0A ,   /*   FGC   */ \
                    0x1A ,   /*   BGC   */ \
                    0x15 ,   /*   CS    */ \
                    ENABLE   /*   DE    */ \
                                        ) == 0x0A1A1501 );

TTX_ASSERT_COMPILE( OSD_P1PNUM_CTRL_RMKS(0x00 ,   /*   FGC   */ \
                    0x00 ,   /*   BGC   */ \
                    0x00 ,   /*   CS    */ \
                    ENABLE   /*   DE    */ \
                                        ) == 0x00000001 );


/* Test OSD_P1PNUM_CINDEX1_REG */
TTX_ASSERT_COMPILE( OSD_P1PNUM_CINDEX1_DEFAULT      == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1PNUM_CINDEX1_RMK(0x00 ,   /*  C0  */ \
                    0x00 ,   /*  C1  */ \
                    0x00 ,   /*  C2  */ \
                    0x00     /*  C3  */ \
                                          ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1PNUM_CINDEX1_RMK(0xFF ,   /*  C0  */ \
                    0xFF ,   /*  C1  */ \
                    0xFF ,   /*  C2  */ \
                    0xFF     /*  C3  */ \
                                          ) == 0x7F7F7F7F );

TTX_ASSERT_COMPILE( OSD_P1PNUM_CINDEX1_RMK(0xFF ,   /*  C0  */ \
                    0x00 ,   /*  C1  */ \
                    0xFF ,   /*  C2  */ \
                    0x00     /*  C3  */ \
                                          ) == 0x7F007F00 );

TTX_ASSERT_COMPILE( OSD_P1PNUM_CINDEX1_RMK(0x00 ,   /*  C0  */ \
                    0xFF ,   /*  C1  */ \
                    0x00 ,   /*  C2  */ \
                    0xFF     /*  C3  */ \
                                          ) == 0x007F007F );

TTX_ASSERT_COMPILE( OSD_P1PNUM_CINDEX1_RMK(0x5A ,   /*  C0  */ \
                    0xAF ,   /*  C1  */ \
                    0x5F ,   /*  C2  */ \
                    0xF5     /*  C3  */ \
                                          ) == 0x5A2F5F75 );

TTX_ASSERT_COMPILE( OSD_P1PNUM_CINDEX1_RMK(0xA5 ,   /*  C0  */ \
                    0x5A ,   /*  C1  */ \
                    0xA5 ,   /*  C2  */ \
                    0x5A     /*  C3  */ \
                                          ) == 0x255A255A );

/* Test OSD_P1PNUM_CINDEX2_REG */
TTX_ASSERT_COMPILE( OSD_P1PNUM_CINDEX2_DEFAULT      == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1PNUM_CINDEX2_RMK(0x00 ,   /*  C4  */ \
                    0x00 ,   /*  C5  */ \
                    0x00 ,   /*  C6  */ \
                    0x00     /*  C7  */ \
                                          ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1PNUM_CINDEX2_RMK(0xFF ,   /*  C4  */ \
                    0xFF ,   /*  C5  */ \
                    0xFF ,   /*  C6  */ \
                    0xFF     /*  C7  */ \
                                          ) == 0x7F7F7F7F );

TTX_ASSERT_COMPILE( OSD_P1PNUM_CINDEX2_RMK(0xFF ,   /*  C4  */ \
                    0x00 ,   /*  C5  */ \
                    0xFF ,   /*  C6  */ \
                    0x00     /*  C7  */ \
                                          ) == 0x7F007F00 );

TTX_ASSERT_COMPILE( OSD_P1PNUM_CINDEX2_RMK(0x00 ,   /*  C4  */ \
                    0xFF ,   /*  C5  */ \
                    0x00 ,   /*  C6  */ \
                    0xFF     /*  C7  */ \
                                          ) == 0x007F007F );

TTX_ASSERT_COMPILE( OSD_P1PNUM_CINDEX2_RMK(0x5A ,   /*  C4  */ \
                    0xAF ,   /*  C5  */ \
                    0x5F ,   /*  C6  */ \
                    0xF5     /*  C7  */ \
                                          ) == 0x5A2F5F75 );

TTX_ASSERT_COMPILE( OSD_P1PNUM_CINDEX2_RMK(0xA5 ,   /*  C4  */ \
                    0x5A ,   /*  C5  */ \
                    0xA5 ,   /*  C6  */ \
                    0x5A     /*  C7  */ \
                                          ) == 0x255A255A );


/* Test OSD_P1MSG_CTRL1_REG */
TTX_ASSERT_COMPILE( OSD_P1MSG_CTRL1_DEFAULT      == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1MSG_CTRL1_RMKS(0x00   ,   /*   FGC   */ \
                    0x00   ,   /*   BGC   */ \
                    0x00   ,   /*   CS    */ \
                    P1   ,   /*   PS    */ \
                    DISABLE,   /*   OL    */ \
                    DISABLE,   /*   UL    */ \
                    DISABLE,   /*   IT    */ \
                    DISABLE,   /*   FL    */ \
                    DISABLE    /*   DE    */ \
                                        ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1MSG_CTRL1_RMKS(0xFF   ,   /*   FGC   */ \
                    0xFF   ,   /*   BGC   */ \
                    0xFF   ,   /*   CS    */ \
                    P2   ,   /*   PS    */ \
                    ENABLE ,   /*   OL    */ \
                    ENABLE ,   /*   UL    */ \
                    ENABLE ,   /*   IT    */ \
                    ENABLE ,   /*   FL    */ \
                    ENABLE     /*   DE    */ \
                                        ) == 0x1F1F1F3F );

TTX_ASSERT_COMPILE( OSD_P1MSG_CTRL1_RMKS(0x5A   ,   /*   FGC   */ \
                    0xFF   ,   /*   BGC   */ \
                    0xA5   ,   /*   CS    */ \
                    P1   ,   /*   PS    */ \
                    ENABLE ,   /*   OL    */ \
                    DISABLE,   /*   UL    */ \
                    ENABLE ,   /*   IT    */ \
                    DISABLE,   /*   FL    */ \
                    ENABLE     /*   DE    */ \
                                        ) == 0x1A1F0515 );

TTX_ASSERT_COMPILE( OSD_P1MSG_CTRL1_RMKS(0x0F   ,   /*   FGC   */ \
                    0xA5   ,   /*   BGC   */ \
                    0xF0   ,   /*   CS    */ \
                    P2   ,   /*   PS    */ \
                    DISABLE,   /*   OL    */ \
                    ENABLE ,   /*   UL    */ \
                    DISABLE,   /*   IT    */ \
                    ENABLE ,   /*   FL    */ \
                    DISABLE    /*   DE    */ \
                                        ) == 0x0F05102A );

TTX_ASSERT_COMPILE( OSD_P1MSG_CTRL1_RMKS(0xF0   ,   /*   FGC   */ \
                    0x5A   ,   /*   BGC   */ \
                    0x0F   ,   /*   CS    */ \
                    P2   ,   /*   PS    */ \
                    DISABLE,   /*   OL    */ \
                    ENABLE ,   /*   UL    */ \
                    ENABLE ,   /*   IT    */ \
                    ENABLE ,   /*   FL    */ \
                    DISABLE    /*   DE    */ \
                                        ) == 0x101A0F2E );

TTX_ASSERT_COMPILE( OSD_P1MSG_CTRL1_RMKS(0x5A   ,   /*   FGC   */ \
                    0xA5   ,   /*   BGC   */ \
                    0xA5   ,   /*   CS    */ \
                    P1   ,   /*   PS    */ \
                    ENABLE ,   /*   OL    */ \
                    ENABLE ,   /*   UL    */ \
                    DISABLE,   /*   IT    */ \
                    ENABLE ,   /*   FL    */ \
                    DISABLE    /*   DE    */ \
                                        ) == 0x1A05051A );


/* Test OSD_P1MSG_CTRL2_REG */
TTX_ASSERT_COMPILE( OSD_P1MSG_CTRL2_DEFAULT      == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1MSG_CTRL2_RMK(   0   ,   /*   LEN   */ \
                    0x00   ,   /*   ROW   */ \
                    0x00       /*   COL   */ \
                                       ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1MSG_CTRL2_RMK(  10   ,   /*   LEN   */ \
                    0xFF   ,   /*   ROW   */ \
                    0xFF       /*   COL   */ \
                                       ) == 0x000A1F3F );

TTX_ASSERT_COMPILE( OSD_P1MSG_CTRL2_RMK(   9   ,   /*   LEN   */ \
                    0x00   ,   /*   ROW   */ \
                    0xFF       /*   COL   */ \
                                       ) == 0x0009003F );

TTX_ASSERT_COMPILE( OSD_P1MSG_CTRL2_RMK(   8   ,   /*   LEN   */ \
                    0xFF   ,   /*   ROW   */ \
                    0x00       /*   COL   */ \
                                       ) == 0x00081F00 );

TTX_ASSERT_COMPILE( OSD_P1MSG_CTRL2_RMK(   7   ,   /*   LEN   */ \
                    0x0F   ,   /*   ROW   */ \
                    0xF0       /*   COL   */ \
                                       ) == 0x00070F30 );

TTX_ASSERT_COMPILE( OSD_P1MSG_CTRL2_RMK(   6   ,   /*   LEN   */ \
                    0x5A   ,   /*   ROW   */ \
                    0xA5       /*   COL   */ \
                                       ) == 0x00061A25 );

TTX_ASSERT_COMPILE( OSD_P1MSG_CTRL2_RMK(   5   ,   /*   LEN   */ \
                    0xA5   ,   /*   ROW   */ \
                    0x5A       /*   COL   */ \
                                       ) == 0x0005051A );

TTX_ASSERT_COMPILE( OSD_P1MSG_CTRL2_RMK(   4   ,   /*   LEN   */ \
                    0x0F   ,   /*   ROW   */ \
                    0xA5       /*   COL   */ \
                                       ) == 0x00040F25 );

TTX_ASSERT_COMPILE( OSD_P1MSG_CTRL2_RMK(   3   ,   /*   LEN   */ \
                    0xA5   ,   /*   ROW   */ \
                    0x0F       /*   COL   */ \
                                       ) == 0x0003050F );

TTX_ASSERT_COMPILE( OSD_P1MSG_CTRL2_RMK(   2   ,   /*   LEN   */ \
                    0x07   ,   /*   ROW   */ \
                    0x70       /*   COL   */ \
                                       ) == 0x00020730 );

TTX_ASSERT_COMPILE( OSD_P1MSG_CTRL2_RMK(   1   ,   /*   LEN   */ \
                    0x12   ,   /*   ROW   */ \
                    0x34       /*   COL   */ \
                                       ) == 0x00011234 );

TTX_ASSERT_COMPILE( OSD_P1MSG_CTRL2_RMK(   0   ,   /*   LEN   */ \
                    0x43   ,   /*   ROW   */ \
                    0x21       /*   COL   */ \
                                       ) == 0x00000321 );


/* Test OSD_P1MSG_CINDEX1_REG */
TTX_ASSERT_COMPILE( OSD_P1MSG_CINDEX1_DEFAULT      == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1MSG_CINDEX1_RMK(0x00 ,   /*  C0  */ \
                    0x00 ,   /*  C1  */ \
                    0x00 ,   /*  C2  */ \
                    0x00     /*  C3  */ \
                                         ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1MSG_CINDEX1_RMK(0xFF ,   /*  C0  */ \
                    0xFF ,   /*  C1  */ \
                    0xFF ,   /*  C2  */ \
                    0xFF     /*  C3  */ \
                                         ) == 0x7F7F7F7F );

TTX_ASSERT_COMPILE( OSD_P1MSG_CINDEX1_RMK(0xFF ,   /*  C0  */ \
                    0x00 ,   /*  C1  */ \
                    0xFF ,   /*  C2  */ \
                    0x00     /*  C3  */ \
                                         ) == 0x7F007F00 );

TTX_ASSERT_COMPILE( OSD_P1MSG_CINDEX1_RMK(0x00 ,   /*  C0  */ \
                    0xFF ,   /*  C1  */ \
                    0x00 ,   /*  C2  */ \
                    0xFF     /*  C3  */ \
                                         ) == 0x007F007F );

TTX_ASSERT_COMPILE( OSD_P1MSG_CINDEX1_RMK(0x5A ,   /*  C0  */ \
                    0xAF ,   /*  C1  */ \
                    0x5F ,   /*  C2  */ \
                    0xF5     /*  C3  */ \
                                         ) == 0x5A2F5F75 );

TTX_ASSERT_COMPILE( OSD_P1MSG_CINDEX1_RMK(0xA5 ,   /*  C0  */ \
                    0x5A ,   /*  C1  */ \
                    0xA5 ,   /*  C2  */ \
                    0x5A     /*  C3  */ \
                                         ) == 0x255A255A );

/* Test OSD_P1MSG_CINDEX2_REG */
TTX_ASSERT_COMPILE( OSD_P1MSG_CINDEX2_DEFAULT      == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1MSG_CINDEX2_RMK(0x00 ,   /*  C4  */ \
                    0x00 ,   /*  C5  */ \
                    0x00 ,   /*  C6  */ \
                    0x00     /*  C7  */ \
                                         ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1MSG_CINDEX2_RMK(0xFF ,   /*  C4  */ \
                    0xFF ,   /*  C5  */ \
                    0xFF ,   /*  C6  */ \
                    0xFF     /*  C7  */ \
                                         ) == 0x7F7F7F7F );

TTX_ASSERT_COMPILE( OSD_P1MSG_CINDEX2_RMK(0xFF ,   /*  C4  */ \
                    0x00 ,   /*  C5  */ \
                    0xFF ,   /*  C6  */ \
                    0x00     /*  C7  */ \
                                         ) == 0x7F007F00 );

TTX_ASSERT_COMPILE( OSD_P1MSG_CINDEX2_RMK(0x00 ,   /*  C4  */ \
                    0xFF ,   /*  C5  */ \
                    0x00 ,   /*  C6  */ \
                    0xFF     /*  C7  */ \
                                         ) == 0x007F007F );

TTX_ASSERT_COMPILE( OSD_P1MSG_CINDEX2_RMK(0x5A ,   /*  C4  */ \
                    0xAF ,   /*  C5  */ \
                    0x5F ,   /*  C6  */ \
                    0xF5     /*  C7  */ \
                                         ) == 0x5A2F5F75 );

TTX_ASSERT_COMPILE( OSD_P1MSG_CINDEX2_RMK(0xA5 ,   /*  C4  */ \
                    0x5A ,   /*  C5  */ \
                    0xA5 ,   /*  C6  */ \
                    0x5A     /*  C7  */ \
                                         ) == 0x255A255A );

/* Test OSD_P1MSG_CINDEX3_REG */
TTX_ASSERT_COMPILE( OSD_P1MSG_CINDEX3_DEFAULT      == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1MSG_CINDEX3_RMK(0x00 ,   /*  C8  */ \
                    0x00     /*  C9  */ \
                                         ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1MSG_CINDEX3_RMK(0xFF ,   /*  C8  */ \
                    0xFF     /*  C9  */ \
                                         ) == 0x7F7F0000 );

TTX_ASSERT_COMPILE( OSD_P1MSG_CINDEX3_RMK(0xFF ,   /*  C8  */ \
                    0x00     /*  C9  */ \
                                         ) == 0x7F000000 );

TTX_ASSERT_COMPILE( OSD_P1MSG_CINDEX3_RMK(0x00 ,   /*  C8  */ \
                    0xFF     /*  C9  */ \
                                         ) == 0x007F0000 );

TTX_ASSERT_COMPILE( OSD_P1MSG_CINDEX3_RMK(0x5A ,   /*  C8  */ \
                    0xF5     /*  C9  */ \
                                         ) == 0x5A750000 );

TTX_ASSERT_COMPILE( OSD_P1MSG_CINDEX3_RMK(0xA5 ,   /*  C8  */ \
                    0x5A     /*  C9  */ \
                                         ) == 0x255A0000 );

/* Test OSD_ITG1_REG */
TTX_ASSERT_COMPILE( OSD_ITG1_DEFAULT      == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_ITG1_RMKS(     0    ,    /*  VSPRD  */ \
                                       0    ,    /*  VSITV  */ \
                                       UNDETECTED,    /*  VSOUT  */ \
                                       DISABLE ,    /* HSIDLEE */ \
                                       DEASSERT ,    /* HVSSTOP */ \
                                       DISABLE ,    /* HVSFMEN */ \
                                       0    ,    /* HVSMODE */ \
                                       ASIC  ,    /* FPGABP  */ \
                                       DISABLE      /* HVSGEN  */ \
                                 ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_ITG1_RMKS(   0x3FF  ,    /*  VSPRD  */ \
                                     0x1F   ,    /*  VSITV  */ \
                                     DETECTED ,    /*  VSOUT  */ \
                                     ENABLE  ,    /* HSIDLEE */ \
                                     ASSERT  ,    /* HVSSTOP */ \
                                     ENABLE  ,    /* HVSFMEN */ \
                                     1    ,    /* HVSMODE */ \
                                     FPGA  ,    /* FPGABP  */ \
                                     ENABLE       /* HVSGEN  */ \
                                 ) == 0x03FF1F7F );

TTX_ASSERT_COMPILE( OSD_ITG1_RMKS(   0xABC  ,    /*  VSPRD  */ \
                                     0xDE   ,    /*  VSITV  */ \
                                     UNDETECTED,    /*  VSOUT  */ \
                                     ENABLE  ,    /* HSIDLEE */ \
                                     DEASSERT ,    /* HVSSTOP */ \
                                     ENABLE  ,    /* HVSFMEN */ \
                                     0    ,    /* HVSMODE */ \
                                     FPGA  ,    /* FPGABP  */ \
                                     DISABLE      /* HVSGEN  */ \
                                 ) == 0x02BC1E2A );

TTX_ASSERT_COMPILE( OSD_ITG1_RMKS(   0x123  ,    /*  VSPRD  */ \
                                     0x45   ,    /*  VSITV  */ \
                                     DETECTED ,    /*  VSOUT  */ \
                                     DISABLE ,    /* HSIDLEE */ \
                                     ASSERT  ,    /* HVSSTOP */ \
                                     DISABLE ,    /* HVSFMEN */ \
                                     1    ,    /* HVSMODE */ \
                                     ASIC  ,    /* FPGABP  */ \
                                     ENABLE      /* HVSGEN  */ \
                                 ) == 0x01230555 );

TTX_ASSERT_COMPILE( OSD_ITG1_RMKS(   0x456  ,    /*  VSPRD  */ \
                                     0x78   ,    /*  VSITV  */ \
                                     DETECTED ,    /*  VSOUT  */ \
                                     DISABLE ,    /* HSIDLEE */ \
                                     DEASSERT ,    /* HVSSTOP */ \
                                     ENABLE  ,    /* HVSFMEN */ \
                                     0    ,    /* HVSMODE */ \
                                     FPGA  ,    /* FPGABP  */ \
                                     DISABLE      /* HVSGEN  */ \
                                 ) == 0x0056184A );

/* Test OSD_ITG2_REG */
TTX_ASSERT_COMPILE( OSD_ITG2_DEFAULT      == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_ITG2_RMK (  0x0000    ,    /*  HSPRD  */ \
                                    0x00           /*  HSITV  */ \
                                 ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_ITG2_RMK (  0xFFFF    ,    /*  HSPRD  */ \
                                    0xFF           /*  HSITV  */ \
                                 ) == 0xFFFF00FF );

TTX_ASSERT_COMPILE( OSD_ITG2_RMK (  0x0000    ,    /*  HSPRD  */ \
                                    0xFF           /*  HSITV  */ \
                                 ) == 0x000000FF );

TTX_ASSERT_COMPILE( OSD_ITG2_RMK (  0xFFFF    ,    /*  HSPRD  */ \
                                    0x00           /*  HSITV  */ \
                                 ) == 0xFFFF0000 );

TTX_ASSERT_COMPILE( OSD_ITG2_RMK (  0xF0F0    ,    /*  HSPRD  */ \
                                    0xFF           /*  HSITV  */ \
                                 ) == 0xF0F000FF );

TTX_ASSERT_COMPILE( OSD_ITG2_RMK (  0xFFFF    ,    /*  HSPRD  */ \
                                    0x0F           /*  HSITV  */ \
                                 ) == 0xFFFF000F );

TTX_ASSERT_COMPILE( OSD_ITG2_RMK (  0xABCD    ,    /*  HSPRD  */ \
                                    0xEF           /*  HSITV  */ \
                                 ) == 0xABCD00EF );

/* Test OSD_ITG3_REG */
TTX_ASSERT_COMPILE( OSD_ITG3_DEFAULT      == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_ITG3_RMK (0x3FF      /* LNCNT */ \
                                 ) == 0x000003FF );

TTX_ASSERT_COMPILE( OSD_ITG3_RMK (0x000      /* LNCNT */ \
                                 ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_ITG3_RMK (0xFFF      /* LNCNT */ \
                                 ) == 0x000003FF );

TTX_ASSERT_COMPILE( OSD_ITG3_RMK (0x123      /* LNCNT */ \
                                 ) == 0x00000123 );




/* Test OSD_P1TC_INIT_REG */
TTX_ASSERT_COMPILE( OSD_P1TC_INIT_DEFAULT      == 0x00070020 );

TTX_ASSERT_COMPILE( OSD_P1TC_INIT_RMKS(DISABLE,   /*  CS  */ \
                                       DISABLE,   /*  FL  */ \
                                       DISABLE,   /*  DH  */ \
                                       DISABLE,   /*  DW  */ \
                                       DISABLE,   /*  WP  */ \
                                       DISABLE,   /*  CN  */ \
                                       DISABLE,   /*  SM  */ \
                                       DISABLE,   /*  HM  */ \
                                       0x00  ,   /*  FG  */ \
                                       0x00  ,   /*  BG  */ \
                                       0x00      /*  HP  */ \
                                      ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_P1TC_INIT_RMKS(DISABLE,   /*  CS  */ \
                                       DISABLE,   /*  FL  */ \
                                       DISABLE,   /*  DH  */ \
                                       DISABLE,   /*  DW  */ \
                                       DISABLE,   /*  WP  */ \
                                       DISABLE,   /*  CN  */ \
                                       DISABLE,   /*  SM  */ \
                                       DISABLE,   /*  HM  */ \
                                       0x12  ,   /*  FG  */ \
                                       0x34  ,   /*  BG  */ \
                                       0x56      /*  HP  */ \
                                      ) == 0x00121456 );

TTX_ASSERT_COMPILE( OSD_P1TC_INIT_RMKS(ENABLE ,   /*  CS  */ \
                                       DISABLE,   /*  FL  */ \
                                       DISABLE,   /*  DH  */ \
                                       DISABLE,   /*  DW  */ \
                                       DISABLE,   /*  WP  */ \
                                       DISABLE,   /*  CN  */ \
                                       DISABLE,   /*  SM  */ \
                                       ENABLE ,   /*  HM  */ \
                                       0xFF  ,   /*  FG  */ \
                                       0xFF  ,   /*  BG  */ \
                                       0xFF      /*  HP  */ \
                                      ) == 0x811F1F7F );

TTX_ASSERT_COMPILE( OSD_P1TC_INIT_RMKS(ENABLE ,   /*  CS  */ \
                                       DISABLE,   /*  FL  */ \
                                       ENABLE ,   /*  DH  */ \
                                       DISABLE,   /*  DW  */ \
                                       ENABLE ,   /*  WP  */ \
                                       DISABLE,   /*  CN  */ \
                                       ENABLE ,   /*  SM  */ \
                                       DISABLE,   /*  HM  */ \
                                       0x5A  ,   /*  FG  */ \
                                       0xFF  ,   /*  BG  */ \
                                       0x00      /*  HP  */ \
                                      ) == 0xAA1A1F00 );

TTX_ASSERT_COMPILE( OSD_P1TC_INIT_RMKS(DISABLE,   /*  CS  */ \
                                       ENABLE ,   /*  FL  */ \
                                       DISABLE,   /*  DH  */ \
                                       ENABLE ,   /*  DW  */ \
                                       ENABLE ,   /*  WP  */ \
                                       DISABLE,   /*  CN  */ \
                                       ENABLE ,   /*  SM  */ \
                                       DISABLE,   /*  HM  */ \
                                       0xF0  ,   /*  FG  */ \
                                       0xA5  ,   /*  BG  */ \
                                       0x73      /*  HP  */ \
                                      ) == 0x5A100573 );


/* Test OSD_CLUT0_ENTRY0_REG */
TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY0_DEFAULT      == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY0_RMK(0x00,   /*  R  */ \
                    0x00,   /*  G  */ \
                    0x00    /*  B  */ \
                                        ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY0_RMK(0xFF,   /*  R  */ \
                    0xFF,   /*  G  */ \
                    0xFF    /*  B  */ \
                                        ) == 0x00FFFFFF );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY0_RMK(0x5A,   /*  R  */ \
                    0xA5,   /*  G  */ \
                    0x0F    /*  B  */ \
                                        ) == 0x005AA50F );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY0_RMK(0xA5,   /*  R  */ \
                    0x5A,   /*  G  */ \
                    0xA5    /*  B  */ \
                                        ) == 0x00A55AA5 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY0_RMK(0x01,   /*  R  */ \
                    0x02,   /*  G  */ \
                    0x03    /*  B  */ \
                                        ) == 0x00010203 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY0_RMK(0x03,   /*  R  */ \
                    0x02,   /*  G  */ \
                    0x01    /*  B  */ \
                                        ) == 0x00030201 );



/* Test OSD_CLUT0_ENTRY1_REG */
TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY1_DEFAULT      == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY1_RMK(0x00,   /*  R  */ \
                    0x00,   /*  G  */ \
                    0x00    /*  B  */ \
                                        ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY1_RMK(0xFF,   /*  R  */ \
                    0xFF,   /*  G  */ \
                    0xFF    /*  B  */ \
                                        ) == 0x00FFFFFF );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY1_RMK(0x5A,   /*  R  */ \
                    0xA5,   /*  G  */ \
                    0x0F    /*  B  */ \
                                        ) == 0x005AA50F );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY1_RMK(0xA5,   /*  R  */ \
                    0x5A,   /*  G  */ \
                    0xA5    /*  B  */ \
                                        ) == 0x00A55AA5 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY1_RMK(0x01,   /*  R  */ \
                    0x02,   /*  G  */ \
                    0x03    /*  B  */ \
                                        ) == 0x00010203 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY1_RMK(0x03,   /*  R  */ \
                    0x02,   /*  G  */ \
                    0x01    /*  B  */ \
                                        ) == 0x00030201 );

/* Test OSD_CLUT0_ENTRY2_REG */
TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY2_DEFAULT      == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY2_RMK(0x00,   /*  R  */ \
                    0x00,   /*  G  */ \
                    0x00    /*  B  */ \
                                        ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY2_RMK(0xFF,   /*  R  */ \
                    0xFF,   /*  G  */ \
                    0xFF    /*  B  */ \
                                        ) == 0x00FFFFFF );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY2_RMK(0x5A,   /*  R  */ \
                    0xA5,   /*  G  */ \
                    0x0F    /*  B  */ \
                                        ) == 0x005AA50F );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY2_RMK(0xA5,   /*  R  */ \
                    0x5A,   /*  G  */ \
                    0xA5    /*  B  */ \
                                        ) == 0x00A55AA5 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY2_RMK(0x01,   /*  R  */ \
                    0x02,   /*  G  */ \
                    0x03    /*  B  */ \
                                        ) == 0x00010203 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY2_RMK(0x03,   /*  R  */ \
                    0x02,   /*  G  */ \
                    0x01    /*  B  */ \
                                        ) == 0x00030201 );



/* Test OSD_CLUT0_ENTRY3_REG */
TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY3_DEFAULT      == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY3_RMK(0x00,   /*  R  */ \
                    0x00,   /*  G  */ \
                    0x00    /*  B  */ \
                                        ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY3_RMK(0xFF,   /*  R  */ \
                    0xFF,   /*  G  */ \
                    0xFF    /*  B  */ \
                                        ) == 0x00FFFFFF );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY3_RMK(0x5A,   /*  R  */ \
                    0xA5,   /*  G  */ \
                    0x0F    /*  B  */ \
                                        ) == 0x005AA50F );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY3_RMK(0xA5,   /*  R  */ \
                    0x5A,   /*  G  */ \
                    0xA5    /*  B  */ \
                                        ) == 0x00A55AA5 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY3_RMK(0x01,   /*  R  */ \
                    0x02,   /*  G  */ \
                    0x03    /*  B  */ \
                                        ) == 0x00010203 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY3_RMK(0x03,   /*  R  */ \
                    0x02,   /*  G  */ \
                    0x01    /*  B  */ \
                                        ) == 0x00030201 );


/* Test OSD_CLUT0_ENTRY4_REG */
TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY4_DEFAULT      == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY4_RMK(0x00,   /*  R  */ \
                    0x00,   /*  G  */ \
                    0x00    /*  B  */ \
                                        ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY4_RMK(0xFF,   /*  R  */ \
                    0xFF,   /*  G  */ \
                    0xFF    /*  B  */ \
                                        ) == 0x00FFFFFF );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY4_RMK(0x5A,   /*  R  */ \
                    0xA5,   /*  G  */ \
                    0x0F    /*  B  */ \
                                        ) == 0x005AA50F );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY4_RMK(0xA5,   /*  R  */ \
                    0x5A,   /*  G  */ \
                    0xA5    /*  B  */ \
                                        ) == 0x00A55AA5 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY4_RMK(0x01,   /*  R  */ \
                    0x02,   /*  G  */ \
                    0x03    /*  B  */ \
                                        ) == 0x00010203 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY4_RMK(0x03,   /*  R  */ \
                    0x02,   /*  G  */ \
                    0x01    /*  B  */ \
                                        ) == 0x00030201 );


/* Test OSD_CLUT0_ENTRY5_REG */
TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY5_DEFAULT      == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY5_RMK(0x00,   /*  R  */ \
                    0x00,   /*  G  */ \
                    0x00    /*  B  */ \
                                        ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY5_RMK(0xFF,   /*  R  */ \
                    0xFF,   /*  G  */ \
                    0xFF    /*  B  */ \
                                        ) == 0x00FFFFFF );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY5_RMK(0x5A,   /*  R  */ \
                    0xA5,   /*  G  */ \
                    0x0F    /*  B  */ \
                                        ) == 0x005AA50F );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY5_RMK(0xA5,   /*  R  */ \
                    0x5A,   /*  G  */ \
                    0xA5    /*  B  */ \
                                        ) == 0x00A55AA5 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY5_RMK(0x01,   /*  R  */ \
                    0x02,   /*  G  */ \
                    0x03    /*  B  */ \
                                        ) == 0x00010203 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY5_RMK(0x03,   /*  R  */ \
                    0x02,   /*  G  */ \
                    0x01    /*  B  */ \
                                        ) == 0x00030201 );


/* Test OSD_CLUT0_ENTRY6_REG */
TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY6_DEFAULT      == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY6_RMK(0x00,   /*  R  */ \
                    0x00,   /*  G  */ \
                    0x00    /*  B  */ \
                                        ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY6_RMK(0xFF,   /*  R  */ \
                    0xFF,   /*  G  */ \
                    0xFF    /*  B  */ \
                                        ) == 0x00FFFFFF );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY6_RMK(0x5A,   /*  R  */ \
                    0xA5,   /*  G  */ \
                    0x0F    /*  B  */ \
                                        ) == 0x005AA50F );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY6_RMK(0xA5,   /*  R  */ \
                    0x5A,   /*  G  */ \
                    0xA5    /*  B  */ \
                                        ) == 0x00A55AA5 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY6_RMK(0x01,   /*  R  */ \
                    0x02,   /*  G  */ \
                    0x03    /*  B  */ \
                                        ) == 0x00010203 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY6_RMK(0x03,   /*  R  */ \
                    0x02,   /*  G  */ \
                    0x01    /*  B  */ \
                                        ) == 0x00030201 );


/* Test OSD_CLUT0_ENTRY7_REG */
TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY7_DEFAULT      == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY7_RMK(0x00,   /*  R  */ \
                    0x00,   /*  G  */ \
                    0x00    /*  B  */ \
                                        ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY7_RMK(0xFF,   /*  R  */ \
                    0xFF,   /*  G  */ \
                    0xFF    /*  B  */ \
                                        ) == 0x00FFFFFF );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY7_RMK(0x5A,   /*  R  */ \
                    0xA5,   /*  G  */ \
                    0x0F    /*  B  */ \
                                        ) == 0x005AA50F );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY7_RMK(0xA5,   /*  R  */ \
                    0x5A,   /*  G  */ \
                    0xA5    /*  B  */ \
                                        ) == 0x00A55AA5 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY7_RMK(0x01,   /*  R  */ \
                    0x02,   /*  G  */ \
                    0x03    /*  B  */ \
                                        ) == 0x00010203 );

TTX_ASSERT_COMPILE( OSD_CLUT0_ENTRY7_RMK(0x03,   /*  R  */ \
                    0x02,   /*  G  */ \
                    0x01    /*  B  */ \
                                        ) == 0x00030201 );


/* Test OSD_COEF0_REG */
TTX_ASSERT_COMPILE( OSD_COEF0_DEFAULT   == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_COEF0_RMK(0x000,   /* COEF0  */ \
                                  0x000    /* COEF1  */ \
                                 ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_COEF0_RMK(0x7FF,   /* COEF0  */ \
                                  0x2FF    /* COEF1  */ \
                                 ) == 0x07FF02FF );

TTX_ASSERT_COMPILE( OSD_COEF0_RMK(0x123,   /* COEF0  */ \
                                  0x456    /* COEF1  */ \
                                 ) == 0x01230456 );

TTX_ASSERT_COMPILE( OSD_COEF0_RMK(0x5A5,   /* COEF0  */ \
                                  0xA5A    /* COEF1  */ \
                                 ) == 0x05A5025A );


/* Test OSD_COEF1_REG */
TTX_ASSERT_COMPILE( OSD_COEF1_DEFAULT   == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_COEF1_RMK(0x000,   /* COEF2  */ \
                                  0x000    /* COEF3  */ \
                                 ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_COEF1_RMK(0x7FF,   /* COEF2  */ \
                                  0x2FF    /* COEF3  */ \
                                 ) == 0x07FF02FF );

TTX_ASSERT_COMPILE( OSD_COEF1_RMK(0x123,   /* COEF2  */ \
                                  0x456    /* COEF3  */ \
                                 ) == 0x01230456 );

TTX_ASSERT_COMPILE( OSD_COEF1_RMK(0x5A5,   /* COEF2  */ \
                                  0xA5A    /* COEF3  */ \
                                 ) == 0x05A5025A );

/* Test OSD_COEF2_REG */
TTX_ASSERT_COMPILE( OSD_COEF2_DEFAULT   == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_COEF2_RMK(0x000,   /* COEF4  */ \
                                  0x000    /* COEF5  */ \
                                 ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_COEF2_RMK(0x7FF,   /* COEF4  */ \
                                  0x2FF    /* COEF5  */ \
                                 ) == 0x07FF02FF );

TTX_ASSERT_COMPILE( OSD_COEF2_RMK(0x123,   /* COEF4  */ \
                                  0x456    /* COEF5  */ \
                                 ) == 0x01230456 );

TTX_ASSERT_COMPILE( OSD_COEF2_RMK(0x5A5,   /* COEF4  */ \
                                  0xA5A    /* COEF5  */ \
                                 ) == 0x05A5025A );

/* Test OSD_COEF3_REG */
TTX_ASSERT_COMPILE( OSD_COEF3_DEFAULT   == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_COEF3_RMK(0x000,   /* COEF6  */ \
                                  0x000    /* COEF7  */ \
                                 ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_COEF3_RMK(0x7FF,   /* COEF6  */ \
                                  0x2FF    /* COEF7  */ \
                                 ) == 0x07FF02FF );

TTX_ASSERT_COMPILE( OSD_COEF3_RMK(0x123,   /* COEF6  */ \
                                  0x456    /* COEF7  */ \
                                 ) == 0x01230456 );

TTX_ASSERT_COMPILE( OSD_COEF3_RMK(0x5A5,   /* COEF6  */ \
                                  0xA5A    /* COEF7  */ \
                                 ) == 0x05A5025A );


/* Test OSD_COEF4_REG */
TTX_ASSERT_COMPILE( OSD_COEF4_DEFAULT   == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_COEF4_RMK(0x000,   /* COEF8  */ \
                                  0x000    /* COEF9  */ \
                                 ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_COEF4_RMK(0x7FF,   /* COEF8  */ \
                                  0x2FF    /* COEF9  */ \
                                 ) == 0x07FF02FF );

TTX_ASSERT_COMPILE( OSD_COEF4_RMK(0x123,   /* COEF8  */ \
                                  0x456    /* COEF9  */ \
                                 ) == 0x01230456 );

TTX_ASSERT_COMPILE( OSD_COEF4_RMK(0x5A5,   /* COEF8  */ \
                                  0xA5A    /* COEF9  */ \
                                 ) == 0x05A5025A );


/* Test OSD_COEF5_REG */
TTX_ASSERT_COMPILE( OSD_COEF5_DEFAULT   == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_COEF5_RMK(0x000,   /* COEF10 */ \
                                  0x000    /* COEF11 */ \
                                 ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_COEF5_RMK(0x7FF,   /* COEF10 */ \
                                  0x2FF    /* COEF11 */ \
                                 ) == 0x07FF02FF );

TTX_ASSERT_COMPILE( OSD_COEF5_RMK(0x123,   /* COEF10 */ \
                                  0x456    /* COEF11 */ \
                                 ) == 0x01230456 );

TTX_ASSERT_COMPILE( OSD_COEF5_RMK(0x5A5,   /* COEF10 */ \
                                  0xA5A    /* COEF11 */ \
                                 ) == 0x05A5025A );


/* Test OSD_COEF6_REG */
TTX_ASSERT_COMPILE( OSD_COEF6_DEFAULT   == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_COEF6_RMK(0x000,   /* COEF12 */ \
                                  0x000    /* COEF13 */ \
                                 ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_COEF6_RMK(0x7FF,   /* COEF12 */ \
                                  0x2FF    /* COEF13 */ \
                                 ) == 0x07FF02FF );

TTX_ASSERT_COMPILE( OSD_COEF6_RMK(0x123,   /* COEF12 */ \
                                  0x456    /* COEF13 */ \
                                 ) == 0x01230456 );

TTX_ASSERT_COMPILE( OSD_COEF6_RMK(0x5A5,   /* COEF12 */ \
                                  0xA5A    /* COEF13 */ \
                                 ) == 0x05A5025A );


/* Test OSD_DM_MSB00_REG */
TTX_ASSERT_COMPILE( OSD_DM_MSB00_DEFAULT           == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB00_RMK( 0x00000000 ) == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB00_RMK( 0xFFFFFFFF ) == 0xFFFFFFFF );
TTX_ASSERT_COMPILE( OSD_DM_MSB00_RMK( 0x0F0F0F0F ) == 0x0F0F0F0F );
TTX_ASSERT_COMPILE( OSD_DM_MSB00_RMK( 0xF0F0F0F0 ) == 0xF0F0F0F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB00_RMK( 0x5AFFA5F0 ) == 0x5AFFA5F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB00_RMK( 0x12345678 ) == 0x12345678 );
TTX_ASSERT_COMPILE( OSD_DM_MSB00_RMK( 0x87654321 ) == 0x87654321 );
TTX_ASSERT_COMPILE( OSD_DM_MSB00_RMK( 0x9ABCDEF0 ) == 0x9ABCDEF0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB00_RMK( 0x0FEDCBA9 ) == 0x0FEDCBA9 );


/* Test OSD_DM_MSB01_REG */
TTX_ASSERT_COMPILE( OSD_DM_MSB01_DEFAULT           == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB01_RMK( 0x00000000 ) == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB01_RMK( 0xFFFFFFFF ) == 0xFFFFFFFF );
TTX_ASSERT_COMPILE( OSD_DM_MSB01_RMK( 0x0F0F0F0F ) == 0x0F0F0F0F );
TTX_ASSERT_COMPILE( OSD_DM_MSB01_RMK( 0xF0F0F0F0 ) == 0xF0F0F0F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB01_RMK( 0x5AFFA5F0 ) == 0x5AFFA5F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB01_RMK( 0x12345678 ) == 0x12345678 );
TTX_ASSERT_COMPILE( OSD_DM_MSB01_RMK( 0x87654321 ) == 0x87654321 );
TTX_ASSERT_COMPILE( OSD_DM_MSB01_RMK( 0x9ABCDEF0 ) == 0x9ABCDEF0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB01_RMK( 0x0FEDCBA9 ) == 0x0FEDCBA9 );


/* Test OSD_DM_MSB02_REG */
TTX_ASSERT_COMPILE( OSD_DM_MSB02_DEFAULT           == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB02_RMK( 0x00000000 ) == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB02_RMK( 0xFFFFFFFF ) == 0xFFFFFFFF );
TTX_ASSERT_COMPILE( OSD_DM_MSB02_RMK( 0x0F0F0F0F ) == 0x0F0F0F0F );
TTX_ASSERT_COMPILE( OSD_DM_MSB02_RMK( 0xF0F0F0F0 ) == 0xF0F0F0F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB02_RMK( 0x5AFFA5F0 ) == 0x5AFFA5F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB02_RMK( 0x12345678 ) == 0x12345678 );
TTX_ASSERT_COMPILE( OSD_DM_MSB02_RMK( 0x87654321 ) == 0x87654321 );
TTX_ASSERT_COMPILE( OSD_DM_MSB02_RMK( 0x9ABCDEF0 ) == 0x9ABCDEF0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB02_RMK( 0x0FEDCBA9 ) == 0x0FEDCBA9 );


/* Test OSD_DM_MSB03_REG */
TTX_ASSERT_COMPILE( OSD_DM_MSB03_DEFAULT           == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB03_RMK( 0x00000000 ) == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB03_RMK( 0xFFFFFFFF ) == 0xFFFFFFFF );
TTX_ASSERT_COMPILE( OSD_DM_MSB03_RMK( 0x0F0F0F0F ) == 0x0F0F0F0F );
TTX_ASSERT_COMPILE( OSD_DM_MSB03_RMK( 0xF0F0F0F0 ) == 0xF0F0F0F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB03_RMK( 0x5AFFA5F0 ) == 0x5AFFA5F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB03_RMK( 0x12345678 ) == 0x12345678 );
TTX_ASSERT_COMPILE( OSD_DM_MSB03_RMK( 0x87654321 ) == 0x87654321 );
TTX_ASSERT_COMPILE( OSD_DM_MSB03_RMK( 0x9ABCDEF0 ) == 0x9ABCDEF0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB03_RMK( 0x0FEDCBA9 ) == 0x0FEDCBA9 );


/* Test OSD_DM_MSB04_REG */
TTX_ASSERT_COMPILE( OSD_DM_MSB04_DEFAULT           == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB04_RMK( 0x00000000 ) == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB04_RMK( 0xFFFFFFFF ) == 0xFFFFFFFF );
TTX_ASSERT_COMPILE( OSD_DM_MSB04_RMK( 0x0F0F0F0F ) == 0x0F0F0F0F );
TTX_ASSERT_COMPILE( OSD_DM_MSB04_RMK( 0xF0F0F0F0 ) == 0xF0F0F0F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB04_RMK( 0x5AFFA5F0 ) == 0x5AFFA5F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB04_RMK( 0x12345678 ) == 0x12345678 );
TTX_ASSERT_COMPILE( OSD_DM_MSB04_RMK( 0x87654321 ) == 0x87654321 );
TTX_ASSERT_COMPILE( OSD_DM_MSB04_RMK( 0x9ABCDEF0 ) == 0x9ABCDEF0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB04_RMK( 0x0FEDCBA9 ) == 0x0FEDCBA9 );


/* Test OSD_DM_MSB05_REG */
TTX_ASSERT_COMPILE( OSD_DM_MSB05_DEFAULT           == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB05_RMK( 0x00000000 ) == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB05_RMK( 0xFFFFFFFF ) == 0xFFFFFFFF );
TTX_ASSERT_COMPILE( OSD_DM_MSB05_RMK( 0x0F0F0F0F ) == 0x0F0F0F0F );
TTX_ASSERT_COMPILE( OSD_DM_MSB05_RMK( 0xF0F0F0F0 ) == 0xF0F0F0F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB05_RMK( 0x5AFFA5F0 ) == 0x5AFFA5F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB05_RMK( 0x12345678 ) == 0x12345678 );
TTX_ASSERT_COMPILE( OSD_DM_MSB05_RMK( 0x87654321 ) == 0x87654321 );
TTX_ASSERT_COMPILE( OSD_DM_MSB05_RMK( 0x9ABCDEF0 ) == 0x9ABCDEF0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB05_RMK( 0x0FEDCBA9 ) == 0x0FEDCBA9 );


/* Test OSD_DM_MSB06_REG */
TTX_ASSERT_COMPILE( OSD_DM_MSB06_DEFAULT           == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB06_RMK( 0x00000000 ) == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB06_RMK( 0xFFFFFFFF ) == 0xFFFFFFFF );
TTX_ASSERT_COMPILE( OSD_DM_MSB06_RMK( 0x0F0F0F0F ) == 0x0F0F0F0F );
TTX_ASSERT_COMPILE( OSD_DM_MSB06_RMK( 0xF0F0F0F0 ) == 0xF0F0F0F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB06_RMK( 0x5AFFA5F0 ) == 0x5AFFA5F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB06_RMK( 0x12345678 ) == 0x12345678 );
TTX_ASSERT_COMPILE( OSD_DM_MSB06_RMK( 0x87654321 ) == 0x87654321 );
TTX_ASSERT_COMPILE( OSD_DM_MSB06_RMK( 0x9ABCDEF0 ) == 0x9ABCDEF0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB06_RMK( 0x0FEDCBA9 ) == 0x0FEDCBA9 );


/* Test OSD_DM_MSB07_REG */
TTX_ASSERT_COMPILE( OSD_DM_MSB07_DEFAULT           == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB07_RMK( 0x00000000 ) == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB07_RMK( 0xFFFFFFFF ) == 0xFFFFFFFF );
TTX_ASSERT_COMPILE( OSD_DM_MSB07_RMK( 0x0F0F0F0F ) == 0x0F0F0F0F );
TTX_ASSERT_COMPILE( OSD_DM_MSB07_RMK( 0xF0F0F0F0 ) == 0xF0F0F0F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB07_RMK( 0x5AFFA5F0 ) == 0x5AFFA5F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB07_RMK( 0x12345678 ) == 0x12345678 );
TTX_ASSERT_COMPILE( OSD_DM_MSB07_RMK( 0x87654321 ) == 0x87654321 );
TTX_ASSERT_COMPILE( OSD_DM_MSB07_RMK( 0x9ABCDEF0 ) == 0x9ABCDEF0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB07_RMK( 0x0FEDCBA9 ) == 0x0FEDCBA9 );


/* Test OSD_DM_MSB08_REG */
TTX_ASSERT_COMPILE( OSD_DM_MSB08_DEFAULT           == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB08_RMK( 0x00000000 ) == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB08_RMK( 0xFFFFFFFF ) == 0xFFFFFFFF );
TTX_ASSERT_COMPILE( OSD_DM_MSB08_RMK( 0x0F0F0F0F ) == 0x0F0F0F0F );
TTX_ASSERT_COMPILE( OSD_DM_MSB08_RMK( 0xF0F0F0F0 ) == 0xF0F0F0F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB08_RMK( 0x5AFFA5F0 ) == 0x5AFFA5F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB08_RMK( 0x12345678 ) == 0x12345678 );
TTX_ASSERT_COMPILE( OSD_DM_MSB08_RMK( 0x87654321 ) == 0x87654321 );
TTX_ASSERT_COMPILE( OSD_DM_MSB08_RMK( 0x9ABCDEF0 ) == 0x9ABCDEF0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB08_RMK( 0x0FEDCBA9 ) == 0x0FEDCBA9 );


/* Test OSD_DM_MSB09_REG */
TTX_ASSERT_COMPILE( OSD_DM_MSB09_DEFAULT           == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB09_RMK( 0x00000000 ) == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB09_RMK( 0xFFFFFFFF ) == 0xFFFFFFFF );
TTX_ASSERT_COMPILE( OSD_DM_MSB09_RMK( 0x0F0F0F0F ) == 0x0F0F0F0F );
TTX_ASSERT_COMPILE( OSD_DM_MSB09_RMK( 0xF0F0F0F0 ) == 0xF0F0F0F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB09_RMK( 0x5AFFA5F0 ) == 0x5AFFA5F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB09_RMK( 0x12345678 ) == 0x12345678 );
TTX_ASSERT_COMPILE( OSD_DM_MSB09_RMK( 0x87654321 ) == 0x87654321 );
TTX_ASSERT_COMPILE( OSD_DM_MSB09_RMK( 0x9ABCDEF0 ) == 0x9ABCDEF0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB09_RMK( 0x0FEDCBA9 ) == 0x0FEDCBA9 );


/* Test OSD_DM_MSB10_REG */
TTX_ASSERT_COMPILE( OSD_DM_MSB10_DEFAULT           == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB10_RMK( 0x00000000 ) == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB10_RMK( 0xFFFFFFFF ) == 0xFFFFFFFF );
TTX_ASSERT_COMPILE( OSD_DM_MSB10_RMK( 0x0F0F0F0F ) == 0x0F0F0F0F );
TTX_ASSERT_COMPILE( OSD_DM_MSB10_RMK( 0xF0F0F0F0 ) == 0xF0F0F0F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB10_RMK( 0x5AFFA5F0 ) == 0x5AFFA5F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB10_RMK( 0x12345678 ) == 0x12345678 );
TTX_ASSERT_COMPILE( OSD_DM_MSB10_RMK( 0x87654321 ) == 0x87654321 );
TTX_ASSERT_COMPILE( OSD_DM_MSB10_RMK( 0x9ABCDEF0 ) == 0x9ABCDEF0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB10_RMK( 0x0FEDCBA9 ) == 0x0FEDCBA9 );


/* Test OSD_DM_MSB11_REG */
TTX_ASSERT_COMPILE( OSD_DM_MSB11_DEFAULT           == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB11_RMK( 0x00000000 ) == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB11_RMK( 0xFFFFFFFF ) == 0xFFFFFFFF );
TTX_ASSERT_COMPILE( OSD_DM_MSB11_RMK( 0x0F0F0F0F ) == 0x0F0F0F0F );
TTX_ASSERT_COMPILE( OSD_DM_MSB11_RMK( 0xF0F0F0F0 ) == 0xF0F0F0F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB11_RMK( 0x5AFFA5F0 ) == 0x5AFFA5F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB11_RMK( 0x12345678 ) == 0x12345678 );
TTX_ASSERT_COMPILE( OSD_DM_MSB11_RMK( 0x87654321 ) == 0x87654321 );
TTX_ASSERT_COMPILE( OSD_DM_MSB11_RMK( 0x9ABCDEF0 ) == 0x9ABCDEF0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB11_RMK( 0x0FEDCBA9 ) == 0x0FEDCBA9 );


/* Test OSD_DM_MSB12_REG */
TTX_ASSERT_COMPILE( OSD_DM_MSB12_DEFAULT           == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB12_RMK( 0x00000000 ) == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB12_RMK( 0xFFFFFFFF ) == 0xFFFFFFFF );
TTX_ASSERT_COMPILE( OSD_DM_MSB12_RMK( 0x0F0F0F0F ) == 0x0F0F0F0F );
TTX_ASSERT_COMPILE( OSD_DM_MSB12_RMK( 0xF0F0F0F0 ) == 0xF0F0F0F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB12_RMK( 0x5AFFA5F0 ) == 0x5AFFA5F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB12_RMK( 0x12345678 ) == 0x12345678 );
TTX_ASSERT_COMPILE( OSD_DM_MSB12_RMK( 0x87654321 ) == 0x87654321 );
TTX_ASSERT_COMPILE( OSD_DM_MSB12_RMK( 0x9ABCDEF0 ) == 0x9ABCDEF0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB12_RMK( 0x0FEDCBA9 ) == 0x0FEDCBA9 );


/* Test OSD_DM_MSB13_REG */
TTX_ASSERT_COMPILE( OSD_DM_MSB13_DEFAULT           == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB13_RMK( 0x00000000 ) == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB13_RMK( 0xFFFFFFFF ) == 0xFFFFFFFF );
TTX_ASSERT_COMPILE( OSD_DM_MSB13_RMK( 0x0F0F0F0F ) == 0x0F0F0F0F );
TTX_ASSERT_COMPILE( OSD_DM_MSB13_RMK( 0xF0F0F0F0 ) == 0xF0F0F0F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB13_RMK( 0x5AFFA5F0 ) == 0x5AFFA5F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB13_RMK( 0x12345678 ) == 0x12345678 );
TTX_ASSERT_COMPILE( OSD_DM_MSB13_RMK( 0x87654321 ) == 0x87654321 );
TTX_ASSERT_COMPILE( OSD_DM_MSB13_RMK( 0x9ABCDEF0 ) == 0x9ABCDEF0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB13_RMK( 0x0FEDCBA9 ) == 0x0FEDCBA9 );


/* Test OSD_DM_MSB14_REG */
TTX_ASSERT_COMPILE( OSD_DM_MSB14_DEFAULT           == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB14_RMK( 0x00000000 ) == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB14_RMK( 0xFFFFFFFF ) == 0xFFFFFFFF );
TTX_ASSERT_COMPILE( OSD_DM_MSB14_RMK( 0x0F0F0F0F ) == 0x0F0F0F0F );
TTX_ASSERT_COMPILE( OSD_DM_MSB14_RMK( 0xF0F0F0F0 ) == 0xF0F0F0F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB14_RMK( 0x5AFFA5F0 ) == 0x5AFFA5F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB14_RMK( 0x12345678 ) == 0x12345678 );
TTX_ASSERT_COMPILE( OSD_DM_MSB14_RMK( 0x87654321 ) == 0x87654321 );
TTX_ASSERT_COMPILE( OSD_DM_MSB14_RMK( 0x9ABCDEF0 ) == 0x9ABCDEF0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB14_RMK( 0x0FEDCBA9 ) == 0x0FEDCBA9 );


/* Test OSD_DM_MSB15_REG */
TTX_ASSERT_COMPILE( OSD_DM_MSB15_DEFAULT           == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB15_RMK( 0x00000000 ) == 0x00000000 );
TTX_ASSERT_COMPILE( OSD_DM_MSB15_RMK( 0xFFFFFFFF ) == 0xFFFFFFFF );
TTX_ASSERT_COMPILE( OSD_DM_MSB15_RMK( 0x0F0F0F0F ) == 0x0F0F0F0F );
TTX_ASSERT_COMPILE( OSD_DM_MSB15_RMK( 0xF0F0F0F0 ) == 0xF0F0F0F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB15_RMK( 0x5AFFA5F0 ) == 0x5AFFA5F0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB15_RMK( 0x12345678 ) == 0x12345678 );
TTX_ASSERT_COMPILE( OSD_DM_MSB15_RMK( 0x87654321 ) == 0x87654321 );
TTX_ASSERT_COMPILE( OSD_DM_MSB15_RMK( 0x9ABCDEF0 ) == 0x9ABCDEF0 );
TTX_ASSERT_COMPILE( OSD_DM_MSB15_RMK( 0x0FEDCBA9 ) == 0x0FEDCBA9 );

/* Test OSD_DM_LSB0_REG */
TTX_ASSERT_COMPILE( OSD_DM_LSB0_DEFAULT      == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_DM_LSB0_RMK(0x00,   /*  00  */ \
                                    0x00,   /*  01  */ \
                                    0x00,   /*  02  */ \
                                    0x00,   /*  03  */ \
                                    0x00,   /*  04  */ \
                                    0x00,   /*  05  */ \
                                    0x00,   /*  06  */ \
                                    0x00    /*  07  */ \
                                   ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_DM_LSB0_RMK(0xFF,   /*  00  */ \
                                    0xFF,   /*  01  */ \
                                    0xFF,   /*  02  */ \
                                    0xFF,   /*  03  */ \
                                    0xFF,   /*  04  */ \
                                    0xFF,   /*  05  */ \
                                    0xFF,   /*  06  */ \
                                    0xFF    /*  07  */ \
                                   ) == 0xFFFFFFFF );

TTX_ASSERT_COMPILE( OSD_DM_LSB0_RMK(0x00,   /*  00  */ \
                                    0xFF,   /*  01  */ \
                                    0x00,   /*  02  */ \
                                    0xFF,   /*  03  */ \
                                    0x00,   /*  04  */ \
                                    0xFF,   /*  05  */ \
                                    0x00,   /*  06  */ \
                                    0xFF    /*  07  */ \
                                   ) == 0x0F0F0F0F );

TTX_ASSERT_COMPILE( OSD_DM_LSB0_RMK(0xFF,   /*  00  */ \
                                    0x00,   /*  01  */ \
                                    0xFF,   /*  02  */ \
                                    0x00,   /*  03  */ \
                                    0xFF,   /*  04  */ \
                                    0x00,   /*  05  */ \
                                    0xFF,   /*  06  */ \
                                    0x00    /*  07  */ \
                                   ) == 0xF0F0F0F0 );

TTX_ASSERT_COMPILE( OSD_DM_LSB0_RMK(0x01,   /*  00  */ \
                                    0xF2,   /*  01  */ \
                                    0xE3,   /*  02  */ \
                                    0xD4,   /*  03  */ \
                                    0xC5,   /*  04  */ \
                                    0xB6,   /*  05  */ \
                                    0xA7,   /*  06  */ \
                                    0x98    /*  07  */ \
                                   ) == 0x12345678 );

TTX_ASSERT_COMPILE( OSD_DM_LSB0_RMK(0x89,   /*  00  */ \
                                    0x7A,   /*  01  */ \
                                    0x6B,   /*  02  */ \
                                    0x5C,   /*  03  */ \
                                    0x4D,   /*  04  */ \
                                    0x3E,   /*  05  */ \
                                    0x2F,   /*  06  */ \
                                    0x10    /*  07  */ \
                                   ) == 0x9ABCDEF0 );

/* Test OSD_DM_LSB1_REG */
TTX_ASSERT_COMPILE( OSD_DM_LSB1_DEFAULT      == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_DM_LSB1_RMK(0x00,   /*  08  */ \
                                    0x00,   /*  09  */ \
                                    0x00,   /*  10  */ \
                                    0x00,   /*  11  */ \
                                    0x00,   /*  12  */ \
                                    0x00,   /*  13  */ \
                                    0x00,   /*  14  */ \
                                    0x00    /*  15  */ \
                                   ) == 0x00000000 );

TTX_ASSERT_COMPILE( OSD_DM_LSB1_RMK(0xFF,   /*  08  */ \
                                    0xFF,   /*  09  */ \
                                    0xFF,   /*  10  */ \
                                    0xFF,   /*  11  */ \
                                    0xFF,   /*  12  */ \
                                    0xFF,   /*  13  */ \
                                    0xFF,   /*  14  */ \
                                    0xFF    /*  15  */ \
                                   ) == 0xFFFFFFFF );

TTX_ASSERT_COMPILE( OSD_DM_LSB1_RMK(0x00,   /*  08  */ \
                                    0xFF,   /*  09  */ \
                                    0x00,   /*  10  */ \
                                    0xFF,   /*  11  */ \
                                    0x00,   /*  12  */ \
                                    0xFF,   /*  13  */ \
                                    0x00,   /*  14  */ \
                                    0xFF    /*  15  */ \
                                   ) == 0x0F0F0F0F );

TTX_ASSERT_COMPILE( OSD_DM_LSB1_RMK(0xFF,   /*  08  */ \
                                    0x00,   /*  09  */ \
                                    0xFF,   /*  10  */ \
                                    0x00,   /*  11  */ \
                                    0xFF,   /*  12  */ \
                                    0x00,   /*  13  */ \
                                    0xFF,   /*  14  */ \
                                    0x00    /*  15  */ \
                                   ) == 0xF0F0F0F0 );

TTX_ASSERT_COMPILE( OSD_DM_LSB1_RMK(0x01,   /*  08  */ \
                                    0xF2,   /*  09  */ \
                                    0xE3,   /*  10  */ \
                                    0xD4,   /*  11  */ \
                                    0xC5,   /*  12  */ \
                                    0xB6,   /*  13  */ \
                                    0xA7,   /*  14  */ \
                                    0x98    /*  15  */ \
                                   ) == 0x12345678 );

TTX_ASSERT_COMPILE( OSD_DM_LSB1_RMK(0x89,   /*  08  */ \
                                    0x7A,   /*  09  */ \
                                    0x6B,   /*  10  */ \
                                    0x5C,   /*  11  */ \
                                    0x4D,   /*  12  */ \
                                    0x3E,   /*  13  */ \
                                    0x2F,   /*  14  */ \
                                    0x10    /*  15  */ \
                                   ) == 0x9ABCDEF0 );



/* Test OSD_FMSYNC_CTRL1_REG */
TTX_ASSERT_COMPILE( OSD_FMSYNC_CTRL1_DEFAULT      == 0x00000000 );


/* Test OSD_FMSYNC_CTRL2_REG */
TTX_ASSERT_COMPILE( OSD_FMSYNC_CTRL2_DEFAULT      == 0x08610249 );





/*-------------------------------------------------------------------------*\
 * End of osd_hal_test.h                                                   *
\*-------------------------------------------------------------------------*/

