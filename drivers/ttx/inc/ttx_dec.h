/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_decode_top.h
 * @brief    Teletext Table of Pages software decoder routines.
 * @author   Janus Cheng
 * @date     Jan. 23, 2009
\*****************************************************************************/

/**
 * @addtogroup ttx
 * @{
 */
#ifndef __REALTEK_TELETEXT_SOFTWARE_DECODER_HEADER_INCLUDED__
#define __REALTEK_TELETEXT_SOFTWARE_DECODER_HEADER_INCLUDED__    (1)

#include <ttx_disp.h>   /* Provides: TTX_DISP_AttrMem_t */
#include <ttx_std.h>    /* Provides: TTX_STD_ColorTable_t */

#ifdef __cplusplus
extern "C"
{
#endif

    /* -- Global typedef Declarations -------------------------------------------*/
#if 0
    typedef enum TTX_SizeAttrMode_t
    {
        TTX_SIZE_ATTR_MODE_MICRONAS,
        TTX_SIZE_ATTR_MODE_PHILIPS
    } TTX_SizeAttrMode_t;
#endif

    typedef struct TTX_LopParam_t
    {
        // character set
        uint8_t DEFAULT_G0_CHARSET : 5;
        uint8_t SECOND_G0_CHARSET : 5;
        uint8_t G1_CHARSET : 5;
        uint8_t G2_CHARSET : 5;
        uint8_t C12C13C14 : 3;

        // L1.5 or L2.5
        uint8_t Alpha_Black_Enable : 1;
        uint8_t Double_Width_Enable : 1;
        uint8_t Double_Size_Enable : 1;
        uint8_t ESC_Enable : 1;
        uint8_t MOSAICS_BLACK_Enable : 1;
        uint8_t NewBackground_Enable : 1;
        //uint8_t SizeAttrMode : 1; // please see TTX_SizeAttrMode_t

        // enable subtitle/newsflash
        uint8_t Newsflash_Subtitle : 1;

        //uint8_t X26_Enable : 1;
        //uint8_t X24_Enable : 1;

        //uint8_t Mode : 1;

        // color related settings
        uint8_t Default_BGC : 3;
        TTX_STD_ColorTable_t FG_CLUT : 2;
        TTX_STD_ColorTable_t BG_CLUT : 2;
        uint8_t REMAP_CLUT : 2;
    } TTX_LopParam_t;
#if 0
    typedef struct TTX_LopParam_t
    {
        uint8_t              Alpha_Black_Enable       : 1;
        uint8_t              MOSAICS_BLACK_Enable     : 1;
        uint8_t              NewBackground_Enable     : 1;
        uint8_t              Double_Width_Enable      : 1;
        uint8_t              Double_Size_Enable       : 1;
        uint8_t              ESC_Enable               : 1;
        uint8_t              X26_Enable;
        uint8_t              X24_Enable               : 1;
        uint8_t              Newsflash_Subtitle       : 1;
        uint8_t              Default_BGC;
        uint8_t              DEFAULT_G0_CHARSET       : 5;
        uint8_t              SECOND_G0_CHARSET        : 5;
        TTX_STD_ColorTable_t FG_CLUT;
        TTX_STD_ColorTable_t BG_CLUT;
        uint8_t              REMAP_CLUT;
        uint8_t              G1_CHARSET               : 5;
        uint8_t              C12C13C14                : 3;
        uint8_t              Mode                     : 1;
        TTX_SizeAttrMode_t   SizeAttrMode;
    } TTX_LopParam_t;
#endif
    typedef enum TTX_X26Function_t {
        TTX_X26Function_UNKNOWN = 0, // 0x00
        // Level 1.5
        TTX_X26Function_SET_ACTIVE_POSITION, // 0x01
        TTX_X26Function_ADDRESS_DISPLAY_ROW_0, // 0x02
        TTX_X26Function_G0_CHAR_WITHOUT_DM, // 0x03
        TTX_X26Function_G0_CHAR_WITH_DM, // 0x04
        TTX_X26Function_CHAR_FROM_G2, // 0x05
        TTX_X26Function_SMOOTHED_MOSAIC_CHAR_FROM_G3, // 0x06
        // PDC
        TTX_X26Function_PDC, // 0x07
        // Level 2.5
        TTX_X26Function_FULL_SCREEN_COLOUR, // 0x08
        TTX_X26Function_FULL_ROW_COLOUR, // 0x09
        TTX_X26Function_ORIGIN_MODIFIER, // 0x0A
        TTX_X26Function_ACTIVE_OBJECT_INVOCATION, // 0x0B
        TTX_X26Function_ADAPTIVE_OBJECT_INVOCATION, // 0x0C
        TTX_X26Function_PASSIVE_OBJECT_INVOCATION, // 0x0D
        TTX_X26Function_ACTIVE_OBJECT_DEFINITION, // 0x0E
        TTX_X26Function_ADAPTIVE_OBJECT_DEFINITION, // 0x0F
        TTX_X26Function_PASSIVE_OBJECT_DEFINITION, // 0x10
        TTX_X26Function_DRCS_MODE, // 0x11
        TTX_X26Function_TERMINATION_MARKER, // 0x12
        TTX_X26Function_FOREGROUND_COLOUR, // 0x13
        TTX_X26Function_BLOCK_MOSAIC_CHARACTER_FROM_THE_G1_SET, // 0x14
        TTX_X26Function_BACKGROUND_COLOUR, // 0x15
        TTX_X26Function_ADDITIONAL_FLASH_FUNCTIONS, // 0x16
        TTX_X26Function_MODIFIED_G0_AND_G2_CHARACTER_SET_DESIGN, // 0x17
        TTX_X26Function_CHARACTER_FROM_THE_G0_SET, // 0x18
        TTX_X26Function_LINE_DRAWING_OR_SMOOTHED_MOSAIC_CHARACTER_FROM_THE_G3_SET, // 0x19
        TTX_X26Function_DISPLAY_ATTRIBUTES, // 0x1A
        TTX_X26Function_DRCS_CHARACTER_INVOCATION, // 0x1B
        // Level 3.5
        TTX_X26Function_FONT_STYLE, // 0x1C
    } TTX_X26Function_t;

    typedef struct TTX_X26DecodedData_t
    {
        uint8_t row : 8; // row: 0~25
        uint8_t col : 8; // column: 0 ~ 39
        uint8_t c_index : 8; // character index: 0x20 ~ 0x7F
        uint8_t dm_index : 5; // diacritical mark index: 0x00 ~ 0x1F
        uint8_t function : 3; // X26 function: 0 ~ 6
    } TTX_X26DecodedData_t;

    typedef struct TTX_X26Info_t
    {
        TTX_X26DecodedData_t x26_decoded_data[13*15]; // 13 tripleets per row * (16-1) rows [ignore X/26/15]
        uint8_t counter;
    } TTX_X26Info_t;

    extern TTX_X26Info_t xdata ttx_x26_info; // X/26 decoded data


    /* -- Public Function Prototypes --------------------------------------------*/

    /* @brief Decode a page of Teletext to background attribute memory. */
    bool_t TTX_DecodePage (uint8_t idx, TTX_STD_Page_CtrlBits_t ctrlBits, TTX_LopParam_t *pParam);
    bool_t TTX_DecodeX24 (uint8_t idx, const TTX_LopParam_t* pParam);

#ifdef __cplusplus
}   /* extern "C" */
#endif


#endif /* !__REALTEK_TELETEXT_SOFTWARE_DECODER_HEADER_INCLUDED__ */

/**
 * @}
 */

/*---------------------------------------------------------------------------*\
 * end of ttx_dec.h                                                          *
\*---------------------------------------------------------------------------*/
