/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ppr.h
 * @brief    Vertical Blanking Interval (VBI) Slicer driver.
 * @author   Janus Cheng
 * @date     Apr. 22, 2009
 * @version  0.1
 * @ingroup  vbi
\*****************************************************************************/

/**
 * @addtogroup vbi
 * @{
 */
#ifndef __REALTEK_VERTICAL_BLANKING_INTERVAL_PRE_PROCESSOR_HEADER_INCLUDED__
#define __REALTEK_VERTICAL_BLANKING_INTERVAL_PRE_PROCESSOR_HEADER_INCLUDED__ (1)

/* -- headers ---------------------------------------------------------------*/
#include <stdint.h>     /* Provides: uint_least8_t */

#ifdef __cplusplus
extern "C"
{
#endif

    /* -- Global Macro Definitions ----------------------------------------------*/

    /* PPR_open() flags */
    typedef enum
    {
        PPR_OPEN_NOP   = 0x00000000u,
        PPR_OPEN_RESET = 0x00000001u
    } PPR_OpenFlag_t;

    /* private object, not to be used by application code */
    typedef struct
    {
        uint_least8_t dummy;
    } PPR_Obj, *PPR_Handle;


    /* device ID */
    typedef uint_fast8_t PPR_DevId_t;

    /* -- Global typedef Declarations -------------------------------------------*/




    /* -- Function Prototypes ---------------------------------------------------*/

    PPR_Handle drvif_PPR_Open (const PPR_DevId_t idDev, const PPR_OpenFlag_t flags);
    void drvif_PPR_Close (PPR_Handle hPpr);
    void drvif_PPR_Reset (PPR_Handle hPpr);
    bit drvif_PPR_GetInitialPage(uint16_t *pAddr);
    bit drvif_PPR_GetCNI (uint16_t *pF1_NI, uint16_t *pF2_CNI, uint16_t *pVPS_CNI);
    bit drvif_PPR_Isr (void);

#ifdef __cplusplus
}   /* extern "C" */
#endif


#endif /* !__REALTEK_VERTICAL_BLANKING_INTERVAL_PRE_PROCESSOR_HEADER_INCLUDED__ */

/**
 * @}
 */

/*---------------------------------------------------------------------------*\
 * end of ppr.h                                                              *
\*---------------------------------------------------------------------------*/
