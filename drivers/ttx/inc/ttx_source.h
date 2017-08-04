/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_source.h
 * @brief    Teletext input data processor.
 * @author   Janus Cheng
 * @date     Nov. 21, 2008
 * @version  0.1
 * @ingroup  osd
\*****************************************************************************/

#ifndef __ENHANCED_TELETEXT_DECODER_LIBRARY_DATA_SOURCE_PROCESSOR_HEADER_INCLUDED__
#define __ENHANCED_TELETEXT_DECODER_LIBRARY_DATA_SOURCE_PROCESSOR_HEADER_INCLUDED__ (1)

/* -- TTX headers -----------------------------------------------------------*/
#include <ttx_pkg.h>    /* Provides: uint32_t */
#include <ttx_obj.h>    /* Provides: TTX_Handle */

#ifdef __cplusplus
extern "C"
{
#endif

    /* -- Global Macro Definitions ----------------------------------------------*/




    /* -- Global typedef Declarations -------------------------------------------*/

    /* TTX_SrcOpen() flags */
    typedef enum
    {
        TTX_SRC_OPEN_NOP   = 0x00000000u,
        TTX_SRC_OPEN_RESET = 0x00000001u
    } TTX_SRC_OpenFlag_t;

    typedef struct TTX_SRC_CollectingPage_t
    {
        TTX_STD_Page_AddrWithSubCode_t   pageAddr;
        TTX_STD_Page_CtrlBits_t          ctrlBits;
        uint8_t    counter;
        uint8_t	buf_index;
    } TTX_SRC_CollectingPage_t;



    /* -- Function Prototypes ---------------------------------------------------*/

    UINT32 TTX_SRC_Parse_Read(UINT16 addr);
    void TTX_SRC_Parse_Write(UINT16 addr, UINT32 val);


// This is the ISR handler for PPR field data valid
    void TTX_SRC_PprFetch(void);

    /* Initialize Teletext data source */
    bool_t                  TTX_SRC_Open                        (const TTX_SRC_OpenFlag_t flags);

    /* Close Teletext data source */
    bool_t                  TTX_SRC_Close                       (void);

    void                    TTX_SRC_Reset                       (void);

    /* self test */
    bool_t                  TTX_SRC_Test                        (void);



    /* Parse input buffer to *pDest. */
    bool_t                  TTX_SRC_ParseBuffer                 ();

    /* Initialize PPR to accept data */
    void                    TTX_SRC_PrepareToAcceptNextField    (void);



#ifdef __cplusplus
}   /* extern "C" */
#endif


#endif /* !__ENHANCED_TELETEXT_DECODER_LIBRARY_DATA_SOURCE_PROCESSOR_HEADER_INCLUDED__ */


/*---------------------------------------------------------------------------*\
 * End of ttx_source.h                                                       *
\*---------------------------------------------------------------------------*/
