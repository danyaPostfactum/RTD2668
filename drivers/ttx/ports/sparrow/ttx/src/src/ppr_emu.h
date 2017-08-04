/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ppr_emu.h
 * @brief    Internal (package scope) shared interfaces for ppr_emu.c and ppr_test_emu.c.
 * @author   Janus Cheng
 * @date     Dec. 11, 2008
\*****************************************************************************/

/**
 * @addtogroup ttx
 * @{
 */
#ifndef __REALTEK_SPARROW_VERTICAL_BLANKING_INTERVAL_PRE_PROCESSOR_SOFTWARE_EMULATOR_HEADER_INCLUDED__
#define __REALTEK_SPARROW_VERTICAL_BLANKING_INTERVAL_PRE_PROCESSOR_SOFTWARE_EMULATOR_HEADER_INCLUDED__ (1)

#include <ppr.h>    /* Provides: PPR interfaces to be implemented */

#ifdef __cplusplus
extern "C"
{
#endif

    /* -- Global typedef Declarations -------------------------------------------*/
#define PPREMU_PFE_MODE_NUM (4) /* the number of modes of PPR_PfeMode_t */


    /* -- Public Function Prototypes --------------------------------------------*/

    /* VBIPPR_CTRL_REG */
    void            PPREMU_SetDacquEnable          (void);
    void            PPREMU_SetDacquDisable         (void);

    /* VBIPPR_PFE_CTRL_REG */
    bool_t          PPREMU_GetPfeFldMode0Left      (void);
    bool_t          PPREMU_GetPfeFldMode1Left      (void);
    bool_t          PPREMU_GetPfeFldMode2Left      (void);
    bool_t          PPREMU_GetPfeFldMode3Left      (void);





    /* -- Private Function Prototypes ( Do NOT use ) ----------------------------*/


    /* */
    typedef struct PFE_MagX_Cfg
    {
        uint8_t     m_bIsActivated      :  1;
        uint32_t    m_8byteDesBaseAddr;    // Models: VBIPPR_MAGX_DES_BADDR_REG: The 22-bit specifies the page destination base address of magazine 2 in page cache. It is used by PFEMOVENG for page pre-fetch. The PFEMOVENG uses the register to get destination address of a pre-fetch page. It is 8-byte basis.
    } PFE_MagX_Cfg;

// Models VBIPPR_FB_PKT1_REG
    typedef struct PFE_FbPktDecodedStatus
    {
        uint8_t     m_bOddParityErr : 1;    // only valid for packet X/0
        uint8_t     m_bHammingErr   : 1;
        uint8_t     m_desigCode     : 4;    // only valid for X/26~X/31
        uint8_t     m_pktNum        : 5;    // Hamming 8/4 decoded
        uint8_t     m_magNum        : 3;    // Hamming 8/4 decoded
    } PFE_FbPktDecodedStatus;

    /* Pre-Fetch moving Engine */
    typedef struct PFE_Obj
    {
        PFE_MagX_Cfg            m_cfgMag[9];   // use TTX_STD_MagazineNum_t to index magazines. [0] is un-used because we use the same definition as TTX_STD_MagazineNum_t, which alleges magazine-8 is 8, not 0.
        PFE_MagX_Cfg            m_cfgBpkt;

        // Models VBIPPR_PFE_CTRL_REG
        PPR_PfeMode_t           m_mode;
        uint8_t                 m_bLeft[PPREMU_PFE_MODE_NUM]; // use PPR_PfeMode_t to index left packets.

        // Models VBIPPR_FB_IND_REG
        uint8_t                 m_addrLastPkt; // 4-byte basis
        uint32_t                m_pktX0Ind;


        // Models PFE_NPROC_ADDR
        uint8_t                 m_addrNextProc; // 4-byte basis

        PFE_FbPktDecodedStatus  m_fbPktDecodedStatus [PPR_FIELD_BUFF_PKT_NUM];

        // Models VBIPPR_X0_WORD0_REG ~ VBIPPR_X0_WORD7_REG
        uint8_t                 m_currentX0data [TTX_STD_PACKET_X0_PAYLOAD_SIZE_BYTES];


    } PFE_Obj;


    typedef enum PG_Mode
    {
        PG_MODE_TT   = 0,   // PPR_PG_MODE_MODE_TT
        PG_MODE_CC   = 1,   // PPR_PG_MODE_MODE_CC
        PG_MODE_VPS  = 2,   // PPR_PG_MODE_MODE_VPS
        PG_MODE_WSS  = 3,   // PPR_PG_MODE_MODE_WSS
        PG_MODE_CGMS = 4    // PPR_PG_MODE_MODE_CGMS
    } PG_Mode;

    /* Pattern Generator */
    typedef struct PG_Obj
    {
        uint8_t m_bLineActive : 1;
        uint8_t m_bFromSlicer : 1;  // TRUE: from Slicer;  FALSE: from Pattern Generator
        uint8_t m_bEnabled    : 1;
        PG_Mode m_mode;
    } PG_Obj;

// Models PPR_FB_PKTX_REG
    typedef struct FB_PktInfo
    {
        uint8_t                 m_derr  : 1;    // the packet exists odd parity error (the bit valid only for packet X/0)
        uint8_t                 m_hcerr : 1;    // the packet exists hamming error

        TTX_STD_DesigCode_t     m_dcode;        // designation code (Hamming 8/4 Decoded), the field valid only packet X/26~X31.
        TTX_STD_Packet_Num_t    m_pktNum;       // the page packet number (Hamming 8/4 Decoded)
        TTX_STD_MagazineNum_t   m_magNum;       // The page magazine number (Hamming 8/4 Decoded), magazine-8 is 0
    } FB_PktInfo;


    /* Data Acquisition */
    typedef struct DACQU_Obj
    {
        uint16_t        m_cntPktRx;     // models VBIPPR_PKT_RX_CNT_REG
        uint16_t        m_cntPktErr;    // models VBIPPR_PKT_ERR_CNT_REG

        uint8_t         m_bEnabled   : 1;

        uint8_t         m_bFbValid   : 1;
        uint8_t         m_bFbFull    : 1;

        uint8_t         m_vbiData[PPR_PTNGEN_PKT_LEN_BYTES];

        uint8_t         m_fieldBuff     [PPR_FIELD_BUFF_PKT_NUM][PPR_FIELD_BUFF_PKT_SIZE];
        FB_PktInfo      m_fieldBuffInfo [PPR_FIELD_BUFF_PKT_NUM];
    } DACQU_Obj;

    /* private object, not to be used by application code */
    typedef struct PPR_Obj
    {
        PFE_Obj     m_pfe;      /* Emulator of Prefetch Moving Engine */
        DACQU_Obj   m_dacqu;    /* Emulator of Data Acquisition module */
        PG_Obj      m_pg;       /* Emulator of Pattern Generator */
    } PPREMU_Obj, *PPREMU_Handle;


    extern PPREMU_Obj pprEmu;


#ifdef __cplusplus
}   /* extern "C" */
#endif


#endif /* !__REALTEK_SPARROW_VERTICAL_BLANKING_INTERVAL_PRE_PROCESSOR_SOFTWARE_EMULATOR_HEADER_INCLUDED__ */

/**
 * @}
 */

/*---------------------------------------------------------------------------*\
 * end of ppr_emu.h                                                          *
\*---------------------------------------------------------------------------*/
