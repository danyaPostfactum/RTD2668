/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     osd.h
 * @brief    Internal (package scope) Ttx/C interface.
 * @details  This header declares the platform-dependent interface for this ttx library.
 * @author   Janus Cheng
 * @date     Dec. 21, 2007
 * @version  0.1
 * @ingroup  osd
\*****************************************************************************/

/**
 * @addtogroup osd
 * @{
 */
#ifndef __REALTEK_SPARROW_ON_SCREEN_DISPLAY_DRIVER_HEADER_INCLUDED__
#define __REALTEK_SPARROW_ON_SCREEN_DISPLAY_DRIVER_HEADER_INCLUDED__ (1)

/* -- CCL headers -----------------------------------------------------------*/
#include "osd_hal.h"

#ifdef CONFIG_TTX_SUPPORT
#include <ttx_pkg.h>    /* Provides: uint32_t */
#include <ttx_log.h>    /* Provides: TTX_Warning() */
#include <ttx_assert.h> /* Provides: TTX_REQUIRE() and TTX_ENSURE() */
#include <ttx_std.h>    /* Provides: TTX_STD_Color_t */
#include <ttx_disp.h>   /* Provides: TTX_DISP_MAX_ROW_LEN */
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    /* -- Global Macro Definitions ----------------------------------------------*/
#ifdef __KEIL__  /* KeilC 8051 compiler */
#define OSD_SOFTWARE_EMULATOR           (0)     // Define (1) to use software OSD; (0) to use hardware OSD.
#else
#define OSD_SOFTWARE_EMULATOR           (1)     // Define (1) to use software OSD; (0) to use hardware OSD.
#endif

#if TTX_VERIFY
#define OSD_VERIFY (1)
#else
#define OSD_VERIFY (0)
#endif  /* end of #ifdef TTX_VERIFY */

#if !OSD_SOFTWARE_EMULATOR
    /* Same register read/write functions as CCL */
#define OSD_RegRead(addr)               _OSD_REG_READ(addr)
#define OSD_RegWrite(addr,val)          _OSD_REG_WRITE(addr,val)
#endif  /* end of #if !OSD_SOFTWARE_EMULATOR */

    /* OSD Log functions */
#define OSD_Log                         TTX_Log


    /* OSD Assertion functions */
#define OSD_REQUIRE(_test)              TTX_REQUIRE(_test)
#define OSD_ENSURE(_test)               TTX_ENSURE(_test)
#define OSD_ALLEGE(_test)               TTX_ALLEGE(_test)
#define OSD_ASSERT_COMPILE(_test)       TTX_ASSERT_COMPILE(_test)
#define OSD_ERROR()                     TTX_ERROR()

    /* -- Global typedef Declarations -------------------------------------------*/
    typedef TTX_Address_t   OSD_Address_t;
    typedef TTX_RegData_t   OSD_Data_t;     /* width of data bus of OSD */

// this is the type of zipped TTX_DISP_Attr_t that stored in memory.
    typedef uint32_t OSD_TtAttr_t;


    /* OSD Configurations */
#define OSD_VDELAY                      (0x23)//(0x13)//(0x30)//for demo //(0x24) //for FPGA/QC

#define OSD_HDELAY_START_POS            (0x93) //(0xe)//(0x26)//(0x10)//(0xaa) //162

    /* OSD Constants */
#define OSD_MAX_MSG_LEN                 (10)    // the maxium length of message
#define OSD_MAX_DM_NUM                  (16)    // the maxium number of diacritical marks
#define OSD_MAX_DM_LINE_NUM             (3)     // the maxium number of lines in one diacritical mark

#define OSD_MAX_CHAR_SET_NUM            (4)     // the maxium number of character sets

#define OSD_MAX_CLUT_NUM                (4)     // the maxium number of color lookup tables
#define OSD_MAX_CLUT_ENTRY_NUM          (8)     // the maxium number of entries in one color lookup table
#define OSD_TOTAL_CLUT_NUM              (1)     // the total number of color lookup tables
#define OSD_MAX_PNUM_LEN                (8)     // the maxium length of page number
#define OSD_MAX_COFF_NUM                (14)    // the maxium number of scaling coefficients

#define OSD_FONT_TOTAL_CHARSETS         (7)     // the number of character sets in OSD_ttFont[][][].
#define OSD_FONT_CHAR_NUM_PER_SET       (96)    // the number of caharacters in a character set
#define OSD_FONT_CHAR_SIZE              (16)    // the size of a character bitmap in bytes
#define OSD_FONT_WIDTH                  (12)    // the width of each character in pixel.
#define OSD_FONT_HEIGHT                 (10)    // the height of each character in pixel.
#define OSD_FONT_MAX_WIDTH              OSD_FONT_WIDTH  // the maximum width of each character in pixel. This is motivated from various font resolutions.
#define OSD_FONT_MAX_HEIGHT             OSD_FONT_HEIGHT // the maximum height of each character in pixel. This is motivated from various font resolutions.


    /* OSD auto-computed macros */
#define OSD_HDELAY_START                (OSD_HDELAY_START_POS)//(0x26)//(0x14)//(0xaa) //162
#define OSD_ACTIVE_REGION_SIZE          (12*40)//(12*40) //12-pixel * 40-col
#define OSD_SIDEPANEL_SIZE              (12*16) //12-pixel * 16-col
#define OSD_HDELAY1                     (OSD_HDELAY_START+OSD_SIDEPANEL_SIZE/2)
#define OSD_HDELAY2                     (0x1B)  // (OSD_HDELAY_START+(OSD_ACTIVE_REGION_SIZE+OSD_SIDEPANEL_SIZE)/2)
#define OSD_HDELAY3                     (OSD_HDELAY2+(OSD_SIDEPANEL_SIZE/4))

    OSD_ASSERT_COMPILE( ( OSD_CTRL3_HDLY1_MIN <= OSD_HDELAY1 ) && ( OSD_HDELAY1 <= OSD_CTRL3_HDLY1_MAX ) ); // ERROR: OSD_HDELAY1 is beyond valid range
    OSD_ASSERT_COMPILE( ( OSD_CTRL4_HDLY2_MIN <= OSD_HDELAY2 ) && ( OSD_HDELAY2 <= OSD_CTRL4_HDLY2_MAX ) ); // ERROR: OSD_HDELAY2 is beyond valid range
    OSD_ASSERT_COMPILE( ( OSD_CTRL4_HDLY3_MIN <= OSD_HDELAY3 ) && ( OSD_HDELAY3 <= OSD_CTRL4_HDLY3_MAX ) ); // ERROR: OSD_HDELAY3 is beyond valid range


    /* OSD_open() flags */
    typedef enum
    {
        OSD_OPEN_NOP   = 0x00000000u,
        OSD_OPEN_RESET = 0x00000001u
    } OSD_OpenFlag_t;

    /* -- Global typedef Declarations -------------------------------------------*/

    /* Initialization configuration structure */
    typedef struct
    {
        /* Only Writeable control registers here. */
        _OSD_CTRL1_TYPE   ctrl1;
    } OSD_Config;



// Formally PAGE_NUMBER
    typedef struct OSD_PageNum_t
    {
        uint8_t            bEnable;
        TTX_STD_CharSet_t  csel;
        TTX_STD_Color_t    fg;
        TTX_STD_Color_t    bg;
        char               str[OSD_MAX_PNUM_LEN];
    } OSD_PageNum_t;

    /** The attributes of a message.
     *  This structure is used to store the attribute of a message.
     */
// Formally MESSAGE
    typedef struct OSD_Message_t
    {
        uint8_t enable;             // enable or disable message
        uint8_t inpage;             // the page in which the message displays
        uint8_t csel;               // character set selection
        uint8_t fg;                 // foreground color
        uint8_t bg;                 // background color
        uint8_t o;                  // overline
        uint8_t u;                  // underline
        uint8_t i;                  // italic
        uint8_t fl;                 // flash
        uint8_t row;                // the start row of the message
        uint8_t col;                // the start column of the message
        uint8_t len;                // the length
        char str[OSD_MAX_MSG_LEN];  // the content
    } OSD_Message_t;

    /** Color lookup table value.
     *  This structure is used to represent the color format of a single pixel.
     */
    typedef struct OSD_CLUT_t
    {
        uint8_t r;			// red
        uint8_t g;			// green
        uint8_t b;			// blue
        uint8_t a;			// alpha
    } OSD_CLUT_t;


    /* -- Function Prototypes ---------------------------------------------------*/

    bool_t                  OSD_Open                            (const OSD_OpenFlag_t flags);
    bool_t                  OSD_Close                           (void);
    void                    OSD_Reset                           (void);
    void                    OSD_TestPwrOnDefaults               (void);
    bool_t                  OSD_Test                            (void);

    void                    OSD_SetYuvToggle                    (bool_t bEnable);

    void                    OSD_SetOsdPg                        (bool_t bEnable);

    void                    OSD_SetOutputFormat                 (bool_t bInterlace);

    void                    OSD_SetDisplayMode                  (bool_t bClosedCaption);
    void                    OSD_SetMixedMode                    (bool_t bEnable);
    void                    OSD_SetVbiEnable                    (bool_t bEnable);
    void                    OSD_SetVdelay                       (uint16_t vdelay);
    void                    OSD_SetHdelay1                      (uint8_t  hdelay1);
    void                    OSD_SetHdelay2                      (uint16_t hdelay2);
    void                    OSD_SetHdelay3                      (uint16_t hdelay3);
    void                    OSD_SetTtMultipageMode              (bool_t bEnable);

    bool_t                  OSD_GetVsyncIntrStatus              (void);
    void                    OSD_ClearVsyncIntrStatus            (void);

    void                    OSD_SetClutAll                      (OSD_CLUT_t clut_ary[OSD_TOTAL_CLUT_NUM][OSD_MAX_CLUT_ENTRY_NUM]);

    uint32_t                OSD_CombineRGB                      (uint8_t redValue, uint8_t greenValue, uint8_t blueValue);

    void                    OSD_ApplyEntireScreenChanges        (bool_t bBlock);
    void                    OSD_ApplyRow0Changes                (bool_t bBlock);
    void                    OSD_ApplyPageContentChanges         (bool_t bBlock);
    void                    OSD_ApplyTopCtrlChanges             (bool_t bBlock);
    void                    OSD_ApplySubPageNavCtrlChanges      (bool_t bBlock);

    void                    OSD_SetTtCharsetBitmapAddress       (OSD_Address_t addr);
    OSD_Address_t           OSD_GetTtCharsetBitmapAddress       (void);
    void                    OSD_SetTtAttr1Address               (OSD_Address_t addr);
    OSD_Address_t           OSD_GetTtAttr1Address               (void);
    void                    OSD_SetTtR25E                       (bool_t bEnable);
    bool_t                  OSD_GetTtR25E                       (void);
    void                    OSD_SetTtVsr                        (uint8_t rate, uint8_t rowfix);
    void                    OSD_SetTtHsr                        (uint8_t rate);
    void                    OSD_SetTtSmooth                     (bool_t bEnable);
    void                    OSD_SetTtConceal                    (bool_t bEnable);
    void                    OSD_SetTtR0G1Set                    (uint8_t cset);
    void                    OSD_SetTtMosaicSeparatedMask        (uint16_t  mlm, uint16_t  mbm);
    void                    OSD_GetTtMosaicSeparatedMask        (uint16_t* mlm, uint16_t* mbm);
    void                    OSD_SetTtDisplayEnable              (bool_t bEnable);
    bool_t                  OSD_isDisplaying                    (void);
    void                    OSD_SetTtBoxEnable                  (bool_t bEnable);
    bool_t                  OSD_GetTtBoxEnable                  (void);
    void                    OSD_SetTtRollingEnable              (bool_t bEnable);
    void                    OSD_SetTtTimecodeEnable             (bool_t bEnable);
    void                    OSD_SetTtR0Enable                   (bool_t bEnable);
    void                    OSD_SetTtR0DhDwConfig               (bool_t extForDh, bool_t dwEnable, bool_t dhEnable, bool_t dwMode, bool_t setSpace);
    void                    OSD_SetTtR0DhCmdEnable              (bool_t bEnable);
    void                    OSD_SetTtR0DwCmdEnable              (bool_t bEnable);
    void                    OSD_SetTtR0DsCmdEnable              (bool_t bEnable);
    void                    OSD_SetTtLspNo                      (uint8_t numOfCols);
    void                    OSD_SetTtSidepanelEn                (bool_t bEnable);
    bool_t                  OSD_GetTtSidepanelEn                (void);
    void                    OSD_SetTtScreenColor                (uint8_t clut_entry);
    void                    OSD_SetTtRowColorIndex              (uint8_t row, uint8_t clut_entry);

    void                    OSD_SetTtPageNumberSeq              (bool_t enable, uint8_t csel, uint8_t fg, uint8_t bg, const char *str);
    void                    OSD_SetTtPageNumber                 (const OSD_PageNum_t* pn);
    void                    OSD_GetTtPageNumber                 (OSD_PageNum_t* pn);

    void                    OSD_SetTtMessageSeq                 (bool_t enable, uint8_t csel, uint8_t fg, uint8_t bg, bool_t o, bool_t u,
            bool_t i, bool_t fl, uint8_t row, uint8_t col, const char* str);
    void                    OSD_SetTtMessage                    (const OSD_Message_t* msg);
    void                    OSD_GetTtMessage                    (OSD_Message_t* msg);

    void                    OSD_SetTtAllDiacriticalmarkBitmap   (void);

    void                    OSD_SetTtScaleCoefficient           (const uint32_t coeff[OSD_MAX_COFF_NUM]);

    /* Event handlers */
    void                    OSD_onVerticalSync                  (void);
    void                    OSD_onScreenCanBeRefreshed          (void);


#ifdef __cplusplus
}   /* extern "C" */
#endif


#endif /* !__REALTEK_SPARROW_ON_SCREEN_DISPLAY_DRIVER_HEADER_INCLUDED__ */

/**
 * @}
 */

/*---------------------------------------------------------------------------*\
 * End of osd.h                                                              *
\*---------------------------------------------------------------------------*/
