/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ppr.c
 * @brief    Sparrow imlementation of VBI slicer driver.
 * @details  This header declares the public access application interface for this TtLib library.
 * @author   Janus Cheng
 * @date     Apr. 17, 2009
 * @version  0.1
\*****************************************************************************/
#include "vbi/ppr.h"            /* Provides: functions to be implemented */

#include <stdlib.h>         /* Provides: NULL */
#include <reg_def.h>	/* Provides: VBIPPR_CTRL_REG_0x4000... */
#include <sparrow.h>	/* Provides: rtd_inl() */
#include <timer_event.h>	/* Provides: fw_timer_event_DelayXms() */
#include <rosprintf.h>      /* Provides: RTD_Log() */
#include <isr_utility.h>    /* Provides: SYS_ISR_VBIPPR */
#include <osdcore.h>	/* Provides: COsdCoreGetState() */
#include <message.h>	/* Provides: fw_isr_ui_message_push() */

#ifdef CONFIG_TTX_SUPPORT
#include <ttx_source.h> /* Provides: functions to be implemented */
#include <ttx_std.h> /* Provides: functions to be implemented */
#include <ttx/ttx.h>	/* Provides: TTX_Handle */
#include <ttx_evt.h> /* Provides: functions to be implemented */
#endif

#define NUMPORTS (1) /* number of devices supported by this driver */

typedef struct _PID
{
    uint8_t LCI;
    uint8_t LUF;
    uint8_t PRF;
    uint8_t MI;
    uint8_t Country;
    uint8_t Network;
    uint8_t Day;
    uint8_t Month;
    uint8_t Hour;
    uint8_t Minute;
    uint8_t PTY;
} PID_DATA;

typedef enum CNI_TYPE
{
    CNI_UNKNOW_TYPE=0,
    CNI_VPS_TYPE,
    CNI_830F1_TYPE,
    CNI_830F2_TYPE
} ;

#ifdef CONFIG_TTX_SUPPORT
static PPR_Obj ports [NUMPORTS];
/* -- Prototypes of Private Functions ---------------------------------------*/
//static uint8_t ReverseBit (uint8_t inData);
//static void CDecodePID	(PID_DATA* pPidData);
static UINT8 DecodeHamming84 (UINT8 in, UINT8*pOut);
static uint8_t ReverseBitOrder (uint8_t inData, size_t numOfBits);
static bit drvif_PPR_Read830F1(uint16_t *NI);
static bit drvif_PPR_Read830F2(uint16_t *pF2Cni);
static bit drvif_PPR_ReadVPS(uint16_t *VpsCni);
static void drvif_PPR_initTTx_CNI(void);
static void drvif_PPR_initVPS_CNI(void);
static void drvif_PPR_FetchVpsData(void);
static void drvif_PPR_Fetch830F1Data(void);
static void drvif_PPR_Fetch830F2Data(void);
static bit drvif_PPR_Read830_InitialPage(uint16_t* pPageAdrr);

/* -- global valus ---------------------------------------*/
static volatile UINT8 RecCniType = CNI_UNKNOW_TYPE;
static UINT8 FetchBuf[40];
static uint16_t Fetch830F1_InitPage, Fetch830F2_InitPage;

extern void flow_timer_Reset_WatchDog(void);

#define RETRY_COUNT 35
/* -- Implementations of Public Functions -----------------------------------*/


/*---------------------------------------------------------------------------*/
bit drvif_PPR_Isr (void)
/*****************************************************************************\
 *  @brief   Interrupt service routine.
 *  @return  the initialization status.
 *  @retval  RT_ISR_UNHANDLED to bypass this IRQ to other IPs.
 *  @retval  RT_ISR_HANDLED   to notify that this is our IRQ.
\*****************************************************************************/
{
    static uint8_t delay_count = 0; // a counter to prevent too many ui messages

    /* Currently, VBI PPR, VTE and OSD shares the same pending bit as VBIOSD */
    if ( rtd_isr_inb( SYS_GIP_1_0x2c39 ) & _BIT3 )
    {
        const volatile UINT32 intStatus = rtd_isr_inl( VBIPPR_INT_ST_REG_0x4008 );
        if ( intStatus != 0 )
        {
            if ( intStatus & _BIT6 )  // VBI_PPR_FB_VALID_ST
            {
                TTX_SRC_PprFetch();
                delay_count++;
                if ( delay_count >= 25 )   // send one message per second
                {
                    if (m_hTtx->Correct_X0_cnt >=3)
                        fw_isr_ui_message_push(_TTX_SIGNAL_OK_MESSAGE);
                    delay_count = 0;
                }
//				fw_isr_ui_message_push(_TTX_10MS_NOTIFY_MESSAGE);
            }
            if ( intStatus & _BIT3 )  // VBI_PPR_FB_VALID_ST
            {
                drvif_PPR_FetchVpsData();
                RecCniType=CNI_VPS_TYPE;
            }
            if ( intStatus & _BIT4 )  // VBI_PPR_FB_VALID_ST
            {
                drvif_PPR_Fetch830F1Data();
                RecCniType=CNI_830F1_TYPE;
            }
            if ( intStatus & _BIT5 )  // VBI_PPR_FB_VALID_ST
            {
                drvif_PPR_Fetch830F2Data();
                RecCniType=CNI_830F2_TYPE;

            }
        }

        rtd_isr_outl( VBIPPR_INT_ST_REG_0x4008, intStatus );	// clear local pending bit

        return( RT_ISR_HANDLED );
    }

    return( RT_ISR_UNHANDLED );
}


bit drvif_PPR_GetInitialPage(uint16_t* pAddr)
{
    bool_t    ret = 0;
    uint16_t PageAddr;
    uint8_t retryCnt = 0;

    // Read NI from TT 8/30F1
    drvif_PPR_initTTx_CNI();
    retryCnt = 3;
    while ( retryCnt )
    {
        if ( drvif_PPR_Read830_InitialPage(&PageAddr) )
        {
            RTD_Log(LOGGER_INFO, "drvif_PPR_Read830_InitialPage == TRUE\n");
            ret = 1;
            break;
        }
        else
        {
            RTD_Log(LOGGER_INFO, "drvif_PPR_Read830_InitialPage == FALSE\n");
            retryCnt--;
        }
    }

    if (ret)
    {
        *pAddr = PageAddr;
    }
    else
    {
        *pAddr = 0;
        return (0);
    }
    return (1);
}

static bit drvif_PPR_Read830_InitialPage(uint16_t* pPageAdrr)
{
    UINT8 retryCnt = RETRY_COUNT;

    while (( RecCniType != CNI_830F1_TYPE )&&(RecCniType != CNI_830F2_TYPE))
    {
        if ( retryCnt <= 0 )
        {
            break;
        }
        else
        {
            fw_timer_event_DelayXms(10);
            retryCnt--;
        }
    }

    if ( RecCniType == CNI_830F1_TYPE )
        *pPageAdrr = Fetch830F1_InitPage;
    else if (RecCniType == CNI_830F2_TYPE )
        *pPageAdrr = Fetch830F2_InitPage;
    else
    {
        *pPageAdrr = 0; // it can't get initial page from 8/30/F1 and 8/30/F2
        RecCniType = CNI_UNKNOW_TYPE;
        return 0;
    }

    RecCniType = CNI_UNKNOW_TYPE;
    return 1;
}

/*****************************************************************************\
 * @brief		Get Country and Network Identification.
 * @note		Mimic CTTReadCNI().
 * @param	<pF1_NI>	NI in 8/30 Format 1
 * @param	<pF2_CNI>	CNI in 8/30 Format 2
 * @param	<pVPS_CNI>	CNI in VPS
 *
 *  @return  the initialization status.
 *  @retval  TRUE  success
 *  @retval  FALSE failure
\*****************************************************************************/
bit drvif_PPR_GetCNI (uint16_t *pF1_NI, uint16_t *pF2_CNI, uint16_t *pVPS_CNI)
{
    uint16_t CNI = 0;
    uint8_t retryCnt = 0;

    // reset to default
    *pF1_NI = *pF2_CNI = *pVPS_CNI = 0xFFFF;

    // set slicer
    TTX_SRC_Open( TTX_SRC_OPEN_RESET );

    // Read CNI from VPS
    drvif_PPR_initVPS_CNI();
    retryCnt = 3;
    while ( retryCnt )
    {
        if ( drvif_PPR_ReadVPS(&CNI) )
        {
            *pVPS_CNI = CNI;
            break;
        }
        else
        {
            retryCnt--;
        }
    }

    // Read NI from TT 8/30F1
    drvif_PPR_initTTx_CNI();
    retryCnt = 3;
    while ( retryCnt )
    {
        if ( drvif_PPR_Read830F1(&CNI) )
        {
            *pF1_NI = CNI;
            break;
        }
        else
        {
            retryCnt--;
        }
    }

    flow_timer_Reset_WatchDog();

    // Read CNI from TT 8/30F2
    drvif_PPR_initTTx_CNI();
    retryCnt = 3;
    while ( retryCnt )
    {
        if ( drvif_PPR_Read830F2(&CNI) )
        {
            *pF2_CNI = CNI;
            break;
        }
        else
        {
            retryCnt--;
        }
    }
    flow_timer_Reset_WatchDog();

    RTD_Log(LOGGER_INFO," GetCNI complete \n" );
    return( 1 );	// TRUE
}



/* -- Implementations of Private Functions ----------------------------------*/

static void drvif_PPR_initVPS_CNI(void)
{
    volatile UINT32 regData;

    // set line 17 for VPS
    rtd_outl(TT_VBI_DATA_TYPE_CONFIG_REG_FOR_LINE_17_200X1803_0A10_0x5a10, 0x66666633);

    /*
    	rtd_outl(0x5a04, 0x33330000);	// line 7,8
    	rtd_outl(0x5a08, 0x33333333);	// line 9,10,11,12
    	rtd_outl(0x5a0c, 0x33333333);	// line 13,14,15,16
    	rtd_outl(0x5a10, 0x33333333);	// line 17,18,19,20
    	rtd_outl(0x5a14, 0x33333333); 	// line 21,22,23,24
    	rtd_outl(0x5a18, 0x0);

    	//       field test all line setup end

    	rtd_outl(0x5800, 0x00000040);
    	rtd_outl(0x5a00, 0x004b2713);	// slicer level determined by
    	rtd_outl(0x5a30, 0x00004600);	// tt_vbi_noise_threshold : the lowest of adaptive slicer level when no tt signal
    								//  46 is  for  noise/eye pattern
    	rtd_outl(0x5a60, 0x2150a023);	// bit 29  set that data from vd put through 16 tape low pass filter
    								// start code correction is 80(50h)
    	rtd_outl(0x5a6c, 0x00000000);
    	rtd_outl(0x5a74, 0x0f143110);	//  bit 20 = 1 : enable fix slicer level
    	rtd_outl(0x5a94 , 0x800e01bb);	//  setting  check tt-line
    	//========================================= For  vd  Slicer_1   TT setting  end =============================================

    	// -------------------------   For LV   vd  Slicer_1   VPS  WSS  setting  START ------------------------------------------
    	rtd_outl(0x5a00 , 0x4b4b2713);
    	rtd_outl(0x5a30 , 0x00004600);
    	rtd_outl(0x5a1c , 0x00008888);

    	rtd_outl(0x5a28 , 0xbe78b4a8);
    	rtd_outl(0x5a24 , 0x15991599);
    	rtd_outl(0x5a8c , 0x00000000);
    */
    regData = rtd_inl(VBIPPR_CTRL_REG_0x4000);
    regData = regData | _BIT0;
    rtd_outl(VBIPPR_CTRL_REG_0x4000, regData);	// Enable VBIPPR data acquisition

    regData = rtd_inl(VBIPPR_INT_EN_REG_0x400c);
    regData = regData | _BIT31 | _BIT3;
    rtd_outl(VBIPPR_INT_EN_REG_0x400c, regData);

    //initiate PPR VPS interrupt
    regData = rtd_inl(VBIPPR_INT_ST_REG_0x4008);
    regData |= _BIT3;
    rtd_outl(VBIPPR_INT_ST_REG_0x4008, regData);	// Clear the data status

    regData = rtd_inl(VBIPPR_DATA_RX_CMP_EN_REG_0x4018);
    regData |= _BIT3;
    rtd_outl(VBIPPR_DATA_RX_CMP_EN_REG_0x4018, regData);	// Clear the data status
}

static void drvif_PPR_initTTx_CNI(void)
{
    volatile UINT32 regData;

    // set line 17 for TT
    rtd_outl(TT_VBI_DATA_TYPE_CONFIG_REG_FOR_LINE_17_200X1803_0A10_0x5a10, 0x66666666);

    /*
    	rtd_outl(0x5a04, 0x66660000);	// line 7,8
    	rtd_outl(0x5a08, 0x66666666);	// line 9,10,11,12
    	rtd_outl(0x5a0c, 0x66666666);	// line 13,14,15,16
    	rtd_outl(0x5a10, 0x66666666);	// line 17,18,19,20
    	rtd_outl(0x5a14, 0x66666666); 	// line 21,22,23,24
    	rtd_outl(0x5a18, 0x0);
    */

    regData = rtd_inl(VBIPPR_CTRL_REG_0x4000);
    if ( !(regData&_BIT0) )
    {
        regData = regData | _BIT0;
        rtd_outl(VBIPPR_CTRL_REG_0x4000, regData);	// Enable VBIPPR data acquisition
    }

    regData = rtd_inl(VBIPPR_INT_EN_REG_0x400c);
    //if(regData&_BIT31){
    regData = regData | _BIT31 | _BIT4 | _BIT5;
    rtd_outl(VBIPPR_INT_EN_REG_0x400c, regData);
    //}

    //Reset the CNI parameters: 8/30-1, 8/30-2, VPS

    regData = rtd_inl(VBIPPR_INT_ST_REG_0x4008);
    regData |= (_BIT4|_BIT5);
    rtd_outl(VBIPPR_INT_ST_REG_0x4008, regData);

    regData = rtd_inl(VBIPPR_DATA_RX_CMP_EN_REG_0x4018);
    regData |= (_BIT4|_BIT5);
    rtd_outl(VBIPPR_DATA_RX_CMP_EN_REG_0x4018, regData);
}

#if 0 // not used
/*---------------------------------------------------------------------------*/
static uint8_t
ReverseBit	(uint8_t inData)
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
    uint8_t tmp;
    uint8_t i;

    tmp = 0;
    for (i=0; i<8; i++)
    {
        tmp = tmp << 1;
        if ( (inData & 0x01) != 0)
            tmp = tmp | 0x01;
        inData = inData >> 1;
    }

    return tmp;
}
#endif

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
static uint8_t ReverseBitOrder	(uint8_t inData, size_t numOfBits)
{
    uint8_t temp = 0;
    uint8_t j;

    temp = 0;

    for (j=numOfBits; j>0; j--)
    {
        temp = temp<<1|(inData & 0x01);
        inData = inData >> 1;
    }
    return temp;
}

#if 0 // not used
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
static void CDecodePID	(PID_DATA *pPidData)
{

    volatile UINT32 regData;

    UINT8 buf[13];/* = {0x15, 0x15, 0xea, 0x1d, 0x00, 0x49, 0x73, 0x15, 0x15, 0x64, 0xb6, 0x95, 0x22};*/

    // Read 8/30F2 PID from RTD2638
    regData = rtd_inl(0x5A4c);
    buf[0] = *((UINT8*)&regData);			// PID byte 0
    buf[1] = *(((UINT8*)&regData) + 1);		// PID byte 1
    buf[2] = *(((UINT8*)&regData) + 2);		// PID byte 2
    buf[3] = *(((UINT8*)&regData) + 3);		// PID byte 3

    regData = rtd_inl(0x5A48);
    buf[4] = *((UINT8*)&regData);			// PID byte 4
    buf[5] = *(((UINT8*)&regData) + 1);		// PID byte 5
    buf[6] = *(((UINT8*)&regData) + 2);		// PID byte 6
    buf[7] = *(((UINT8*)&regData) + 3);		// PID byte 7

    regData = rtd_inl(0x5A44);
    buf[8] = *((UINT8*)&regData);			// PID byte 8
    buf[9] = *(((UINT8*)&regData) + 1);		// PID byte 9
    buf[10] = *(((UINT8*)&regData) + 2);	// PID byte 10
    buf[11] = *(((UINT8*)&regData) + 3);	// PID byte 11

    regData = rtd_inl(0x5A40);
    buf[12] = *((UINT8*)&regData);			// PID byte 12
#if 1

//	pPidData->LCI = (buf[0]&0x0c) >> 2;
//	pPidData->LUF = (buf[0]&0x02) >> 1;
//	pPidData->PRF = (buf[0]&0x01);

//	pPidData->MI = (buf[1]&0x02) >> 1;
    pPidData->PTY = ReverseBitOrder(buf[12],8);
    pPidData->Day = ReverseBitOrder((buf[8]&0x7c)>>2,5);

    pPidData->Month = ReverseBitOrder(((buf[8]&0x80)>>7)|((buf[9]&0x07)<<1),4);
    pPidData->Hour = ReverseBitOrder(((buf[9]&0xF8)>>3) ,5);
    pPidData->Minute = ReverseBitOrder((buf[10]&0x3F)  ,6);
    regData=((buf[10]&0xC0)>>2)|((buf[11]&0x03)<<6)|((buf[2]&0xF0)>>4);

    pPidData->Country = ReverseBitOrder(regData  ,8);


    regData=(buf[11]&0xFC) |(buf[8]&0x03) ;
    pPidData->Network = ReverseBitOrder(regData  ,8);

#else

    for (i=0; i<13; i++)
    {
        DecodeHamming84(buf[i], &buf[i]);
    }

    pPidData->LCI = (buf[0]&0x0c) >> 2;
    pPidData->LUF = (buf[0]&0x02) >> 1;
    pPidData->PRF = (buf[0]&0x01);

    pPidData->MI = (buf[1]&0x02) >> 1;
    pPidData->PTY = ((buf[11]&0x0f) << 4) | (buf[12]&0x0f);

    pPidData->Day = ((buf[3]&0x03) << 3) | ((buf[4]&0x0e) >> 1);
    pPidData->Month = ((buf[4]&0x01) << 3) | ((buf[5]&0x0e) >> 1);
    pPidData->Hour = ((buf[5]&0x01) << 4) | (buf[6]&0x0f);
    pPidData->Minute = ((buf[7]&0x0f) << 2) | ((buf[8]&0x0c) >> 2);

    pPidData->Country = ((buf[2]&0x0f) << 4) | ((buf[8]&0x03) << 2) | ((buf[9]&0x0c) >> 2);
    pPidData->Network = ((buf[3]&0x0c) << 4) | ((buf[9]&0x03) << 4) | (buf[10]&0x0f);
#endif
}
#endif

/*---------------------------------------------------------------------------*/
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
static UINT8 DecodeHamming84(UINT8 in, UINT8*pOut)
{
    UINT8 A, B, C, D;

    A = ((in&_BIT7) >> 7) ^ ((in&_BIT5) >> 5) ^ ((in&_BIT1) >> 1) ^ (in&_BIT0);
    B = ((in&_BIT7) >> 7) ^ ((in&_BIT3) >> 3) ^ ((in&_BIT2) >> 2) ^ ((in&_BIT1) >> 1);
    C = ((in&_BIT5) >> 5) ^ ((in&_BIT4) >> 4) ^ ((in&_BIT3) >> 3) ^ ((in&_BIT1) >> 1);
    D = ((in&_BIT7) >> 7) ^ ((in&_BIT6) >> 6) ^ ((in&_BIT5) >> 5) ^ ((in&_BIT4) >> 4) ^ ((in&_BIT3) >> 3) ^ ((in&_BIT2) >> 2) ^ ((in&_BIT1) >> 1) ^ (in&_BIT0);
    //ROSPrintf("A B C D = %d %d %d %d    ", A, B, C, D);

    if ( D && (!A || !B || !C))		// Double error
    {
        RTD_Log(LOGGER_INFO,"DecodeHamming84 decode error \n");
        return 0;	// reject data bits
    }
    if ( !D && !(A && B && C))	// 1 bit error
    {
        if ( !A && !B && !C )		// D1 error
            in = in ^ _BIT1;			// correct D1
        else if ( A && !B && !C )				// D2 error
            in = in ^ _BIT3;			// correct D2
        else if ( !A && B && !C )				// D3 error
            in = in ^ _BIT5;			// correct D3
        else if ( !A && !B && C )				// D4 error
            in = in ^ _BIT7;			// correct D4
    }

    *pOut = 0;
    if ( (in&_BIT1) != 0)
        *pOut = *pOut | 0x08;
    if ( (in&_BIT3) != 0)
        *pOut = *pOut | 0x04;
    if ( (in&_BIT5) != 0)
        *pOut = *pOut | 0x02;
    if ( (in&_BIT7) != 0)
        *pOut = *pOut | 0x01;
    //ROSPrintf("Decode = %x\n", *pOut);

    return 1;
}

/*****************************************************************************\
 *  @brief   The initialization routine.
 *  @details Use this function to initialize this library.
 *  @note    Must be called in advance of any other functions.
 *
 *  @return  none
 *  @retval  TTX_TRUE  success
 *  @retval  TTX_FALSE failure
 *
 *  @ingroup ttx
 *  @see     TTX_Deinit()
\*****************************************************************************/
static bit drvif_PPR_Read830F2(uint16_t *pF2Cni)
{
    UINT8 ucCountry, ucNI, decstatus = 0;
    UINT8 i;
    UINT8 retryCnt = RETRY_COUNT;

    // Read 8/30F2 PID from RTD2688
    while ( RecCniType != CNI_830F2_TYPE )
    {
        if ( retryCnt <= 0 )
        {
            break;
        }
        else
        {
            fw_timer_event_DelayXms(10);
            retryCnt--;
        }
    }

    if ( RecCniType != CNI_830F2_TYPE )
    {
        *pF2Cni = 0xffff;
        return 0;
    }

    for ( i=8 ; i<20 ; i++ )
    {
        decstatus = DecodeHamming84(FetchBuf[i], &FetchBuf[i]);
        if ( decstatus == 0 )
        {
            *pF2Cni = 0xffff;
            return 0;
        }
    }

    ucCountry = ((FetchBuf[9]&0x0f) << 4) | ((FetchBuf[15]&0x03) << 2) | ((FetchBuf[16]&0x0c) >> 2);
    ucNI = ((FetchBuf[10]&0x0c) << 4) | ((FetchBuf[16]&0x03) << 2) | (FetchBuf[17]&0x0f);
    *pF2Cni = ((UINT16)ucCountry << 8) | ucNI;
    RecCniType = CNI_UNKNOW_TYPE;

    //RTD_Log(LOGGER_INFO,"ucCountry = %x\n",(UINT32)ucCountry );
    //RTD_Log(LOGGER_INFO,"ucNID = %x\n",(UINT32)ucNI );
    return 1;
}

/*****************************************************************************\
 *  @brief   The initialization routine.
 *  @details Use this function to initialize this library.
 *  @note    Must be called in advance of any other functions.
 *
 *  @return  none
 *  @retval  TTX_TRUE  success
 *  @retval  TTX_FALSE failure
 *
 *  @ingroup ttx
 *  @see     TTX_Deinit()
\*****************************************************************************/
static bit drvif_PPR_Read830F1(uint16_t *F1Ni)
{
    UINT8 ucCountry, ucNI;
    UINT8 retryCnt = RETRY_COUNT;

    // Read 8/30F2 PID from RTD2688
    while ( RecCniType != CNI_830F1_TYPE )
    {
        if ( retryCnt <= 0 )
        {
            break;
        }
        else
        {
            fw_timer_event_DelayXms(10);
            retryCnt--;
        }
    }

    if ( RecCniType != CNI_830F1_TYPE )
    {
        *F1Ni = 0xffff;
        return 0;
    }

#if 0 // 2668
    ucCountry = ReverseBitOrder(FetchBuf[8],8);
    ucNI = ReverseBitOrder(FetchBuf[7],8);
#else // sparrow
    ucCountry = ReverseBitOrder(FetchBuf[7],8);
    ucNI = ReverseBitOrder(FetchBuf[8],8);
#endif

    *F1Ni = ((UINT16)ucCountry << 8) | ucNI;
    RecCniType = CNI_UNKNOW_TYPE;
    return 1;
}

/*****************************************************************************\
 * @brief		read CNI from VPS
 * @param	<VpsCni> CNI in VPS
 *
 * @retval	TTX_TRUE	success
 * @retval	TTX_FALSE	failure
\*****************************************************************************/
static bit drvif_PPR_ReadVPS (uint16_t *VpsCni)
{
    UINT32 tmpData;
    PID_DATA pidData;
    UINT8 retryCnt = RETRY_COUNT;

    while ( RecCniType != CNI_VPS_TYPE )
    {
        if ( retryCnt <= 0 )
        {
            break;
        }
        else
        {
            fw_timer_event_DelayXms(10);
            retryCnt--;
        }
    }

    if ( RecCniType != CNI_VPS_TYPE )
    {
        *VpsCni = 0xffff;
        return 0;
    }

    pidData.PTY = ReverseBitOrder(FetchBuf[12], 8);
    pidData.Day = ReverseBitOrder((FetchBuf[8]&0x7c)>>2, 5);
    pidData.Month = ReverseBitOrder(((FetchBuf[8]&0x80)>>7)|((FetchBuf[9]&0x07)<<1), 4);
    pidData.Hour = ReverseBitOrder(((FetchBuf[9]&0xF8)>>3), 5);
    pidData.Minute = ReverseBitOrder((FetchBuf[10]&0x3F), 6);
    tmpData = ((FetchBuf[10]&0xC0)>>2)|((FetchBuf[11]&0x03)<<6)|((FetchBuf[2]&0xF0)>>4);
    pidData.Country = ReverseBitOrder(tmpData, 8);
    tmpData = (FetchBuf[11]&0xFC) |(FetchBuf[8]&0x03) ;
    pidData.Network = ReverseBitOrder(tmpData, 8);
    *VpsCni = (pidData.Country)<<8 | pidData.Network;

    RecCniType = CNI_UNKNOW_TYPE;
    return 1;
}

/*****************************************************************************\
 *  @brief   fecth 830 format 1 data in interrupt period
 *  @details Use this function to initialize this library.
 *  @note    this function is called by ppr interrupt
 *
 *  @return  none
\*****************************************************************************/
static void drvif_PPR_Fetch830F1Data(void)
{
    volatile UINT32 regData;
    uint16_t PageAddr;
    uint32_t temp;
    uint8_t page_tens;
    uint8_t page_units;
    uint8_t mag;
    uint8_t b0, b1,b2;

    regData=rtd_isr_inl(VBIPPR_830F1_B7_REG_0x405c);
    FetchBuf[7] = *(((UINT8*)&regData) + 3);

    regData=rtd_isr_inl(VBIPPR_830F1_B8B11_REG_0x4060);
    FetchBuf[8] = *(((UINT8*)&regData) + 0);
    FetchBuf[9] = *(((UINT8*)&regData) + 1);
    FetchBuf[10] = *(((UINT8*)&regData) + 2);
    FetchBuf[11] = *(((UINT8*)&regData) + 3);

    // Get Inital Page
    temp = rtd_inl(VBIPPR_830F1_B0B3_REG_0x4054);
    //RTD_Log(LOGGER_INFO, "Temp : 0x%x \n", (UINT32) temp);
    page_units = (temp & 0x000F0000)>>16;
    page_tens = (temp & 0x00000F00)>>8;
    temp= rtd_inl(VBIPPR_830F1_B4B7_REG_0x4058);
    (temp & _BIT27)?(b0=1):(b0=0);
    (temp & _BIT10)?(b1=1):(b1=0);
    (temp & _BIT11)?(b2=1):(b2=0);
    mag = ((b2<<2)|(b1<<1)|b0);

    if (mag == 0)
        mag = 8;

    PageAddr = mag<<8|page_tens<<4|page_units;
    Fetch830F1_InitPage = PageAddr;
    /*
    	RTD_Log(LOGGER_INFO, "(PageA : %x, mag : %d, tens : %d, units : %d)\n",
    	   		(UINT32) (Fetch830F1_InitPage), (UINT32)mag, (UINT32)page_tens, (UINT32)page_units);
    */
    //disable interrupt
    regData=rtd_isr_inl(VBIPPR_INT_EN_REG_0x400c );
    regData = regData &(~_BIT4);
    rtd_isr_outl(VBIPPR_INT_EN_REG_0x400c, regData);
}

/*****************************************************************************\
 *  @brief   fecth 830 format 2 data in interrupt period
 *  @details Use this function to initialize this library.
 *  @note    this function is called by ppr interrupt
 *
 *  @return  none
\*****************************************************************************/
static void drvif_PPR_Fetch830F2Data(void)
{
    volatile UINT32 regData;
    uint16_t PageAddr;
    uint32_t temp;
    uint8_t page_tens;
    uint8_t page_units;
    uint8_t mag;
    uint8_t b0, b1,b2;

    regData=rtd_isr_inl(VBIPPR_830F2_B8B11_REG_0x408c);
    FetchBuf[8] = *(((UINT8*)&regData) + 0);
    FetchBuf[9] = *(((UINT8*)&regData) + 1);
    FetchBuf[10] = *(((UINT8*)&regData) + 2);
    FetchBuf[11] = *(((UINT8*)&regData) + 3);

    regData=rtd_isr_inl(VBIPPR_830F2_B12B15_REG_0x4090);
    FetchBuf[12] = *(((UINT8*)&regData) +0);
    FetchBuf[13] = *(((UINT8*)&regData) + 1);
    FetchBuf[14] = *(((UINT8*)&regData) + 2);
    FetchBuf[15] = *(((UINT8*)&regData) + 3);

    regData=rtd_isr_inl(VBIPPR_830F2_B16B19_REG_0x4094);
    FetchBuf[16] = *(((UINT8*)&regData) + 0);
    FetchBuf[17] = *(((UINT8*)&regData) + 1);
    FetchBuf[18] = *(((UINT8*)&regData) + 2);
    FetchBuf[19] = *(((UINT8*)&regData) + 3);

    // Get Inital Page
    temp = rtd_inl(VBIPPR_830F2_B0B3_REG_0x4080);
    //RTD_Log(LOGGER_INFO, "Temp : 0x%x \n", (UINT32) temp);
    page_units = (temp & 0x00FF0000)>>16;
    page_tens = (temp & 0x0000FF00)>>8;
    temp= rtd_inl(VBIPPR_830F2_B4B6_REG_0x4084);
    (temp & _BIT27)?(b0=1):(b0=0);
    (temp & _BIT10)?(b1=1):(b1=0);
    (temp & _BIT11)?(b2=1):(b2=0);
    mag = ((b2<<2)|(b1<<1)|b0);

    if (mag == 0)
        mag = 8;

    PageAddr = mag<<8|page_tens<<4|page_units;
    Fetch830F2_InitPage = PageAddr;
    /*
    	RTD_Log(LOGGER_INFO, "Read 8/30 F2  (PageA : %x, mag : %d, tens : %d, units : %d)\n",
    		(UINT32) (Fetch830F2_InitPage), (UINT32)mag, (UINT32)page_tens, (UINT32)page_units);
    */
    //disable interrupt
    regData=rtd_isr_inl(VBIPPR_INT_EN_REG_0x400c );
    regData = regData &(~_BIT5);
    rtd_isr_outl(VBIPPR_INT_EN_REG_0x400c, regData);
}

/*****************************************************************************\
 *  @brief   fecth VPS data in interrupt period
 *  @details this
 *  @note    this function is called by ppr interrupt
 *  @return  none
\*****************************************************************************/
static void drvif_PPR_FetchVpsData(void)
{
    volatile UINT32 regData;
    regData = rtd_isr_inl(VBIPPR_VPS_B0B3_REG_0x4044);
    FetchBuf[0] = *((UINT8*)&regData);			// PID byte 0
    FetchBuf[1] = *(((UINT8*)&regData) + 1); 	// PID byte 1
    FetchBuf[2] = *(((UINT8*)&regData) + 2); 	// PID byte 2
    FetchBuf[3] = *(((UINT8*)&regData) + 3); 	// PID byte 3

    regData = rtd_isr_inl(VBIPPR_VPS_B4B7_REG_0x4048);
    FetchBuf[4] = *((UINT8*)&regData);			// PID byte 4
    FetchBuf[5] = *(((UINT8*)&regData) + 1); 	// PID byte 5
    FetchBuf[6] = *(((UINT8*)&regData) + 2); 	// PID byte 6
    FetchBuf[7] = *(((UINT8*)&regData) + 3); 	// PID byte 7

    regData = rtd_isr_inl(VBIPPR_VPS_B8B11_REG_0x404c);
    FetchBuf[8] = *((UINT8*)&regData);			// PID byte 8
    FetchBuf[9] = *(((UINT8*)&regData) + 1); 	// PID byte 9
    FetchBuf[10] = *(((UINT8*)&regData) + 2);	// PID byte 10
    FetchBuf[11] = *(((UINT8*)&regData) + 3);	// PID byte 11

    regData = rtd_isr_inl(VBIPPR_VPS_B12_REG_0x4050);
    FetchBuf[12] = *(((UINT8*)&regData)+3);			// PID byte 12

    //disable interrupt
    regData=rtd_isr_inl(VBIPPR_INT_EN_REG_0x400c );
    regData = regData &(~_BIT3);
    rtd_isr_outl(VBIPPR_INT_EN_REG_0x400c, regData);
}
#endif
/*---------------------------------------------------------------------------*\
 * end of ppr.c                                                              *
\*---------------------------------------------------------------------------*/

