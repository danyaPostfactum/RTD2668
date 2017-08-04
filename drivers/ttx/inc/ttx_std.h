/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_std.h
 * @brief    Platform-independent standard Teletext interface.
 * @author   Janus Cheng
 * @date     Sep. 26, 2008
 * @version  0.1
 *
 * @pre      TTX_CFG_PRESENT_LEVEL() has already been #defined.
 \*****************************************************************************/

#ifndef __ENHANCED_TELETEXT_STANDARD_DECODER_LIBRARY_PLATFORM_INDEPENDENT_PUBLIC_INTERFACE_HEADER_INCLUDED__
#define __ENHANCED_TELETEXT_STANDARD_DECODER_LIBRARY_PLATFORM_INDEPENDENT_PUBLIC_INTERFACE_HEADER_INCLUDED__ (1)
#ifdef CONFIG_TTX_SUPPORT

#include <ttx_port.h>   /* Provides: uint8_t */
#include <ttx_assert.h> /* Provides: TTX_ASSERT_COMPILE() */
#endif
#ifdef __cplusplus
extern "C"
{
#endif

    /* -- Configurations --------------------------------------------------------*/

    /* -- Auto-Computed Macros --------------------------------------------------*/

    /* Constants to support TTX_STD_PRESENT_LEVEL */
#define _TTX_STD_PRESENT_LEVEL_10   (10)    /* presentation level 1.0 */
#define _TTX_STD_PRESENT_LEVEL_15   (15)    /* presentation level 1.5 */
#define _TTX_STD_PRESENT_LEVEL_25   (25)    /* presentation level 2.5 */
#define _TTX_STD_PRESENT_LEVEL_35   (35)    /* presentation level 3.5 */

    /* Transforms content of TTX_CFG_LOG_LEVEL to TTX_LogLevel_t */
#define TTX_STD_PRESENT_LEVEL(level) _TTX_STD_PRESENT_LEVEL_##level

    typedef uint8_t TTX_STD_MagazineNum_t;  //@note magazine-8 is 8, not 0
#define TTX_MAG_NUM_TOTAL		(8)


    typedef uint8_t TTX_STD_DesigCode_t;

    typedef uint8_t TTX_STD_Page_Addr_Num_Tens_t;
    typedef uint8_t TTX_STD_Page_Addr_Num_Units_t;

    typedef struct TTX_STD_Page_Addr_Number_t
    {
        TTX_STD_Page_Addr_Num_Units_t units : 4;  // 0x0~0xF
        TTX_STD_Page_Addr_Num_Tens_t  tens  : 4;  // 0x0~0xF
    } TTX_STD_Page_Addr_Number_t;

    typedef uint8_t TTX_STD_Page_Addr_Num_Hex_t; /* the hex     representation of TTX_STD_Page_Addr_Number_t */
    typedef uint8_t TTX_STD_Page_Addr_Num_Dec_t; /* the decimal representation of TTX_STD_Page_Addr_Number_t */


    typedef struct TTX_STD_Page_Addr_SubCode_t
    {
        uint8_t s4 : 4;   // MSB: 0x00~0x03
        uint8_t s3 : 4;   //      0x00~0x0F
        uint8_t s2 : 4;   //      0x00~0x07
        uint8_t s1 : 4;   // LSB: 0x00~0x0F
    } TTX_STD_Page_Addr_SubCode_t;

    typedef uint16_t TTX_STD_Page_Addr_SubCode_Hex_t; /* the hex     representation of TTX_STD_Page_Addr_SubCode_t */


    typedef struct TTX_STD_Page_Addr_t
    {
        TTX_STD_MagazineNum_t           magNum;
        TTX_STD_Page_Addr_Number_t   pageNum;
    } TTX_STD_Page_Addr_t;

    typedef uint16_t TTX_STD_Page_Address_Dec_t; /* the decimal representation of TTX_STD_Page_Addr_t */

    typedef struct TTX_STD_Page_AddrWithSubCode_t
    {
        uint16_t pageA;
        uint16_t subcodeA;
    } TTX_STD_Page_AddrWithSubCode_t;

    typedef struct TTX_STD_Page_AddrWithInputSubCode_t
    {
        uint16_t pageA;
        uint16_t subcodeA;
        uint8_t   user_input_status;
    } TTX_STD_Page_AddrWithInputSubCode_t;


    typedef struct TTX_STD_Page_AddrWithSubCode_t1
    {
        TTX_STD_MagazineNum_t           magNum;
        TTX_STD_Page_Addr_Number_t   pageNum;
        TTX_STD_Page_Addr_SubCode_t  subCode;
    } TTX_STD_Page_AddrWithSubCode_t1;


    typedef uint32_t TTX_STD_Page_AddrWithSubCode_Hex_t; /* the hex representation TTX_STD_Page_AddrWithSubCode_t */


    typedef struct TTX_STD_Page_CtrlBits_t
    {
        uint8_t     bErasePage  ;   /* erase page                      : C4            */
        uint8_t     bNewsFlash  ;   /* news flash                      : C5            */
        uint8_t     bSubtitle       ;   /* subtitle                        : C6            */
        uint8_t     bSuppressHeader   ;   /* suppress header                 : C7            */
        uint8_t     bUpdate             ;   /* update indicator                : C8            */
        uint8_t     bInterruptedSequence ;   /* interrupted sequence            : C9            */
        uint8_t     bInhibitDisplay      ;   /* inhibit display                 : C10           */
        uint8_t     bSerialMode     ;   /* magazine serial                 : C11           */
        uint8_t     natlOpt              : 3;   /* national option character subset: C12, C13, C14 */
    } TTX_STD_Page_CtrlBits_t;


    typedef struct TTX_STD_Page
    {
        TTX_STD_Page_AddrWithSubCode_t dummy;
    } TTX_STD_Page;


    typedef uint8_t     TTX_STD_Packet_Num_t;

    typedef struct TTX_STD_Packet_Address_t
    {
        TTX_STD_MagazineNum_t   magNum;
        TTX_STD_Packet_Num_t    pktNum;
    } TTX_STD_Packet_Address_t;

// Character Set
    typedef enum TTX_STD_CharSet_t
    {
        TTX_CHARSET_G0_LATIN = 0,
        TTX_CHARSET_G0_LATIN_CZECH_SLOVAK = 1,
        TTX_CHARSET_G0_LATIN_ENGLISH = 2,
        TTX_CHARSET_G0_LATIN_ESTONIAN = 3,
        TTX_CHARSET_G0_LATIN_FRENCH = 4,
        TTX_CHARSET_G0_LATIN_GERMAN = 5,
        TTX_CHARSET_G0_LATIN_ITALIAN = 6,
        TTX_CHARSET_G0_LATIN_LETTISH_LITHUANIAN = 7,
        TTX_CHARSET_G0_LATIN_POLISH = 8,
        TTX_CHARSET_G0_LATIN_PORTUGUESE_SPANISH = 9,
        TTX_CHARSET_G0_LATIN_RUMANIAN = 10,
        TTX_CHARSET_G0_LATIN_SERBIAN_CROATIAN_SLOVENIAN = 11,
        TTX_CHARSET_G0_LATIN_SWEDISH_FINNISH = 12,
        TTX_CHARSET_G0_LATIN_TURKISH = 13,
        TTX_CHARSET_G2_LATIN = 14,
        TTX_CHARSET_G0_CYRILLIC_OP1 = 15,
        TTX_CHARSET_G0_CYRILLIC_OP2 = 16,
        TTX_CHARSET_G0_CYRILLIC_OP3 = 17,
        TTX_CHARSET_G2_CYRILLIC = 18,
        TTX_CHARSET_G0_GREEK = 19,
        TTX_CHARSET_G2_GREEK = 20,
        TTX_CHARSET_G0_ARABIC = 21,
        TTX_CHARSET_G2_ARABIC = 22,
        TTX_CHARSET_G0_HEBREW = 23,
        TTX_CHARSET_G1_MOSAICS = 24,
        TTX_CHARSET_G3_MOSAICS = 25,
        TTX_CHARSET_GDRCS = 26,
        TTX_CHARSET_DRCS = 27,
        TTX_CHARSET_GDRCS2 = 28,
        TTX_CHARSET_DRCS2 = 29,
        TTX_CHARSET_RESERVED = 30
    } TTX_STD_CharSet_t;

    typedef uint16_t    TTX_STD_CharIndex_t;
    typedef uint8_t     TTX_STD_ColorTable_t;   // selection of color table


    typedef enum TTX_STD_Char_t
    {
        TTX_CHAR_SPACE = 0x20
    } TTX_STD_Char_t;

// Spacing attribute (Section 12.2 (p.70))
    typedef enum TTX_STD_SpacingAttr_t
    {
        TTX_SPATT_ALPHA_BLACK      = 0,
        TTX_SPATT_ALPHA_RED           ,
        TTX_SPATT_ALPHA_GREEN         ,
        TTX_SPATT_ALPHA_YELLOW        ,
        TTX_SPATT_ALPHA_BLUE          ,
        TTX_SPATT_ALPHA_MAGENTA       ,
        TTX_SPATT_ALPHA_CYAN          ,
        TTX_SPATT_ALPHA_WHITE         ,
        TTX_SPATT_FLASH               ,
        TTX_SPATT_STEADY              ,
        TTX_SPATT_END_BOX             ,
        TTX_SPATT_START_BOX           ,
        TTX_SPATT_NORMAL_SIZE         ,
        TTX_SPATT_DOUBLE_HEIGHT       ,
        TTX_SPATT_DOUBLE_WIDTH        ,
        TTX_SPATT_DOUBLE_SIZE         ,
        TTX_SPATT_MOSAICS_BLACK       ,
        TTX_SPATT_MOSAICS_RED         ,
        TTX_SPATT_MOSAICS_GREEN       ,
        TTX_SPATT_MOSAICS_YELLOW      ,
        TTX_SPATT_MOSAICS_BLUE        ,
        TTX_SPATT_MOSAICS_MAGENTA     ,
        TTX_SPATT_MOSAICS_CYAN        ,
        TTX_SPATT_MOSAICS_WHITE       ,
        TTX_SPATT_CONCEAL             ,
        TTX_SPATT_CON_MOSAICS_GRAPHICS,
        TTX_SPATT_SEP_MOSAICS_GRAPHICS,
        TTX_SPATT_ESC                 ,
        TTX_SPATT_BLACK_BACKGROUND    ,
        TTX_SPATT_NEW_BACKGROUND      ,
        TTX_SPATT_HOLD_MOSAICS        ,
        TTX_SPATT_RELEASE_MOSAICS
    } TTX_STD_SpacingAttr_t;


// Color map (Section 12.4 (p.89))
    typedef uint8_t TTX_STD_Color_t;

    /* @brief Available values for type TTX_STD_Color_t.
     * @note  We seperate it from TTX_STD_Color_t because 'enum' is treated as type 'int'
     *          by compilers. When we use TTX_STD_Color_t as an 'enum' member of a struct as:
     *            struct MyStruct
     *            {
     *                TTX_STD_Color_t myfield : 1;  // ERROR: this bit will be assigned to sign bit of int. (Microsoft VC++ Warning C6299)
     *            };
     */
    typedef enum    TTX_STD_ColorValues_t
    {
        TTX_COLOR_BLACK = 0,
        TTX_COLOR_RED,
        TTX_COLOR_GREEN,
        TTX_COLOR_YELLOW,
        TTX_COLOR_BLUE,
        TTX_COLOR_MAGENTA,
        TTX_COLOR_CYAN,
        TTX_COLOR_WHITE
    } TTX_STD_ColorValues_t;

//*****************************************************************************
// Graphic type definition
//*****************************************************************************
#ifdef __KEIL__
    typedef uint8_t TTX_STD_Graphic_t;  /* We don't use bool_t here because KeilC doesn't accept bit type as an element of a struct. */
#else
    typedef bool_t  TTX_STD_Graphic_t;
#endif

    /* @brief Available values for type TTX_STD_Graphic_t.
     * @note  We seperate it from TTX_STD_Graphic_t because 'enum' is treated as type 'int'
     *          by compilers. When we use TTX_STD_Graphic_t as an 'enum' member of a struct as:
     *            struct MyStruct
     *            {
     *                TTX_STD_Graphic_t myfield : 1;  // ERROR: this bit will be assigned to sign bit of int. (Microsoft VC++ Warning C6299)
     *            };
     */
    typedef enum   TTX_STD_GraphicValues_t
    {
        TTX_GRAPHIC_OFF = 0,
        TTX_GRAPHIC_ON
    } TTX_STD_GraphicValues_t;

//*****************************************************************************
// Flash mode type definition (p.85)
//*****************************************************************************
#if TTX_CFG_PRESENT_LEVEL >= TTX_STD_PRESENT_LEVEL(25)
    typedef uint8_t TTX_STD_FlashMode_t;
#else
#ifdef __KEIL__
    typedef uint8_t TTX_STD_FlashMode_t;  /* We don't use bool_t here because KeilC doesn't accept bit type as an element of a struct. */
#else
    typedef bool_t  TTX_STD_FlashMode_t;
#endif  /* end of #ifdef __KEIL__ */
#endif  /* end of #if TTX_CFG_PRESENT_LEVEL >= TTX_STD_PRESENT_LEVEL(25) */

    /* @brief Available values for type TTX_STD_FlashMode_t.
     * @note  We seperate it from TTX_STD_FlashMode_t because 'enum' is treated as type 'int'
     *          by compilers. When we use TTX_STD_FlashMode_t as an 'enum' member of a struct as:
     *            struct MyStruct
     *            {
     *                TTX_STD_FlashMode_t myfield : 1;  // ERROR: this bit will be assigned to sign bit of int. (Microsoft VC++ Warning C6299)
     *            };
     */
    typedef enum    TTX_STD_FlashModeValues_t
    {
        TTX_FLASH_STEADY = 0,
        TTX_FLASH_NORMAL,

#if TTX_CFG_PRESENT_LEVEL >= TTX_STD_PRESENT_LEVEL(25)
        TTX_FLASH_INVERT_PHASE,
        TTX_FLASH_INVERT_CLUT
#endif

    } TTX_STD_FlashModeValues_t;


#if TTX_CFG_PRESENT_LEVEL >= TTX_STD_PRESENT_LEVEL(25)

//*****************************************************************************
// Flash rate type definition (p.85)
//*****************************************************************************
    typedef uint8_t TTX_STD_FlashRate_t;

    /* @brief Available values for type TTX_STD_FlashRate_t.
     * @note  We seperate it from TTX_STD_FlashRate_t because 'enum' is treated as type 'int'
     *          by compilers. When we use TTX_STD_FlashRate_t as an 'enum' member of a struct as:
     *            struct MyStruct
     *            {
     *                TTX_STD_FlashRate_t myfield : 1;  // ERROR: this bit will be assigned to sign bit of int. (Microsoft VC++ Warning C6299)
     *            };
     */
    typedef enum TTX_STD_FlashRateValues_t
    {
        TTX_FLASH_SLOW = 0,
        TTX_FLASH_FAST_PHASE1,
        TTX_FLASH_FAST_PHASE2,
        TTX_FLASH_FAST_PHASE3,
        TTX_FLASH_FAST_INC,
        TTX_FLASH_FAST_DEC
    } TTX_STD_FlashRateValues_t;

#endif  /* end of #if TTX_CFG_PRESENT_LEVEL >= TTX_STD_PRESENT_LEVEL(25) */



//*****************************************************************************
// Conceal type definition
//*****************************************************************************
#ifdef __KEIL__
    typedef uint8_t TTX_STD_ConcealFlag_t;  /* We don't use bool_t here because KeilC doesn't accept bit type as an element of a struct. */
#else
    typedef bool_t  TTX_STD_ConcealFlag_t;
#endif

    /* @brief Available values for type TTX_STD_ConcealFlag_t.
     * @note  We seperate it from TTX_STD_ConcealFlag_t because 'enum' is treated as type 'int'
     *          by compilers. When we use TTX_STD_ConcealFlag_t as an 'enum' member of a struct as:
     *            struct MyStruct
     *            {
     *                TTX_STD_ConcealFlag_t myfield : 1;  // ERROR: this bit will be assigned to sign bit of int. (Microsoft VC++ Warning C6299)
     *            };
     */
    typedef enum    TTX_STD_ConcealFlagValues_t
    {
        TTX_CONCEAL_OFF = 0,
        TTX_CONCEAL_ON
    } TTX_STD_ConcealFlagValues_t;

//*****************************************************************************
// Mosaics type definition
//*****************************************************************************
#ifdef __KEIL__
    typedef uint8_t TTX_STD_Mosaics_t;  /* We don't use bool_t here because KeilC doesn't accept bit type as an element of a struct. */
#else
    typedef bool_t  TTX_STD_Mosaics_t;
#endif

    /* @brief Available values for type TTX_STD_Mosaics_t.
     * @note  We seperate it from TTX_STD_Mosaics_t because 'enum' is treated as type 'int'
     *          by compilers. When we use TTX_STD_Mosaics_t as an 'enum' member of a struct as:
     *            struct MyStruct
     *            {
     *                TTX_STD_Mosaics_t myfield : 1;  // ERROR: this bit will be assigned to sign bit of int.
     *            };
     */
    typedef enum    TTX_STD_MosaicsValues_t
    {
        TTX_MOSAICS_CON = 0,
        TTX_MOSAICS_SEP
    } TTX_STD_MosaicsValues_t;

//*****************************************************************************
// Box type definition
//*****************************************************************************
#ifdef __KEIL__
    typedef uint8_t TTX_STD_Box_t;  /* We don't use bool_t here because KeilC doesn't accept bit type as an element of a struct. */
#else
    typedef bool_t  TTX_STD_Box_t;
#endif

    /* @brief Available values for type TTX_STD_Box_t.
     * @note  We seperate it from TTX_STD_Box_t because 'enum' is treated as type 'int'
     *          by compilers. When we use TTX_STD_Box_t as an 'enum' member of a struct as:
     *            struct MyStruct
     *            {
     *                TTX_STD_Box_t myfield : 1;  // ERROR: this bit will be assigned to sign bit of int. (Microsoft VC++ Warning C6299)
     *            };
     */
    typedef enum    TTX_STD_BoxValues_t
    {
        TTX_BOX_END = 0,     // Add: gellmann@2005/06/27@ver0.6.50
        TTX_BOX_START
    } TTX_STD_BoxValues_t;

//*****************************************************************************
// Diacritical mark type definition
//*****************************************************************************
// Annex E. (p.134)
    typedef uint8_t TTX_STD_DiacMark_t;

    /* @brief Available values for type TTX_STD_DiacMark_t.
     * @note  We seperate it from TTX_STD_DiacMark_t because 'enum' is treated as type 'int'
     *          by compilers. When we use TTX_STD_DiacMark_t as an 'enum' member of a struct as:
     *          struct MyStruct
     *          {
     *              TTX_STD_DiacMark_t myfield : 1;  // ERROR: this bit will be assigned to sign bit of int. (Microsoft VC++ Warning C6299)
     *          };
     */
    typedef enum    TTX_STD_DiacMarkValues_t
    {
        TTX_DIAC_MARK_DISABLE   = 0x00,
        TTX_DIAC_MARK_NONE      = 0x10,
        TTX_DIAC_MARK_1         = 0x11,
        TTX_DIAC_MARK_2         = 0x12,
        TTX_DIAC_MARK_3         = 0x13,
        TTX_DIAC_MARK_4         = 0x14,
        TTX_DIAC_MARK_5         = 0x15,
        TTX_DIAC_MARK_6         = 0x16,
        TTX_DIAC_MARK_7         = 0x17,
        TTX_DIAC_MARK_8         = 0x18,
        TTX_DIAC_MARK_9         = 0x19,
        TTX_DIAC_MARK_10        = 0x1A,
        TTX_DIAC_MARK_11        = 0x1B,
        TTX_DIAC_MARK_12        = 0x1C,
        TTX_DIAC_MARK_13        = 0x1D,
        TTX_DIAC_MARK_14        = 0x1E,
        TTX_DIAC_MARK_15        = 0x1F
    } TTX_STD_DiacMarkValues_t;

    typedef uint8_t TTX_STD_SizeCfg_t;

    /* @brief Available values for type TTX_STD_SizeCfg_t.
     * @note  We seperate it from TTX_STD_SizeCfg_t because 'enum' is treated as type 'int'
     *          by compilers. When we use TTX_STD_SizeCfg_t as an 'enum' member of a struct as:
     *            struct MyStruct
     *            {
     *                TTX_STD_SizeCfg_t myfield : 1;  // ERROR: this bit will be assigned to sign bit of int. (Microsoft VC++ Warning C6299)
     *            };
     */
    typedef enum    TTX_STD_SizeCfgValues_t
    {
        TTX_STD_SIZE_CFG_NORMAL      =  0,

        TTX_STD_SIZE_CFG_DH_UPPER    =  8,
        TTX_STD_SIZE_CFG_DH_LOWER    = 10,

        TTX_STD_SIZE_CFG_DW_LEFT     =  4,
        TTX_STD_SIZE_CFG_DW_RIGHT    =  5,

        TTX_STD_SIZE_CFG_DS_L_UPPER  = 12,
        TTX_STD_SIZE_CFG_DS_R_UPPER  = 13,
        TTX_STD_SIZE_CFG_DS_L_LOWER  = 14,
        TTX_STD_SIZE_CFG_DS_R_LOWER  = 15
    } TTX_STD_SizeCfgValues_t;


    /* Generic Packet */
#define TTX_STD_PACKET_SIZE_BYTES				(42)    /* length of a teletext packet in bytes without clock run-in and without framing code */
#define TTX_STD_PACKET_PAYLOAD_SIZE_BYTES	(40)
    typedef struct TTX_STD_Packet_t
    {
        TTX_STD_Packet_Address_t addr;
        uint8_t dataBytes[TTX_STD_PACKET_PAYLOAD_SIZE_BYTES];
    } TTX_STD_Packet_t;

// Packet X/0
#define TTX_STD_PACKET_X0_PAYLOAD_SIZE_BYTES    (32)
    typedef struct TTX_STD_PacketX0_t
    {
        TTX_STD_Packet_Address_t addr;

        TTX_STD_Page_AddrWithSubCode_t1 pageAddr;
        TTX_STD_Page_CtrlBits_t ctrlBits;

        uint8_t dataBytes[TTX_STD_PACKET_X0_PAYLOAD_SIZE_BYTES];
    } TTX_STD_PacketX0_t;


// Table of Pages Service

    /* The size of the page contains "Basic TOP Table", "Basic TOP Table List" and
     *   "Page-Linking Table". But does NOT include X/0.
     */
#define TTX_STD_TOP_BASIC_TOP_PAGE_SIZE         (23*40)     /* unit: bytes */
#define TTX_STD_TOP_BASIC_TOP_TABLE_ENTRY_NUM   (899-100+1) /* number of entries of a Basic TOP Table */

    typedef uint8_t TTX_STD_TOP_BttCode_t;  /* used to store an element of Basic TOP Table */

    /* @brief Available values for type TTX_STD_TOP_BttCode_t.
     * @details Permissible content of Basic TOP Table (p.27)
     * @note  We seperate it from TTX_STD_TOP_BttCode_t because 'enum' is treated as type 'int'
     *          by compilers. When we use TTX_STD_TOP_BttCode_t as an 'enum' member of a struct as:
     *            struct MyStruct
     *            {
     *                TTX_STD_TOP_BttCode_t myfield : 1;  // ERROR: this bit will be assigned to sign bit of int. (Microsoft VC++ Warning C6299)
     *            };
     */
    typedef enum TTX_STD_TOP_BttCodeValues_t
    {
        TTX_STD_TOP_BTT_CODE_PAGE_NOT_INCLUDED = 0x00,
        TTX_STD_TOP_BTT_CODE_SUBTITLE_PAGE_A   = 0x01, /*                 subtitle page with additional info              */
        TTX_STD_TOP_BTT_CODE_PREV_BLK_PAGE_A   = 0x02, /* programme-preview block  page with additional info              */
        TTX_STD_TOP_BTT_CODE_PREV_BLK_PAGE_MA  = 0x03, /* programme-preview block  page with additional info as multipage */
        TTX_STD_TOP_BTT_CODE_BLK_PAGE_A        = 0x04, /*                   block  page with additional info              */
        TTX_STD_TOP_BTT_CODE_BLK_PAGE_MA       = 0x05, /*                   block  page with additional info as multipage */
        TTX_STD_TOP_BTT_CODE_GRP_PAGE_A        = 0x06, /*                   group  page with additional info              */
        TTX_STD_TOP_BTT_CODE_GRP_PAGE_MA       = 0x07, /*                   group  page with additional info as multipage */
        TTX_STD_TOP_BTT_CODE_NRM_PAGE          = 0x08, /*                   normal page                                   */
        TTX_STD_TOP_BTT_CODE_NRM_PAGE_A        = 0x09, /*                   normal page with additional info              */
        TTX_STD_TOP_BTT_CODE_NRM_PAGE_M        = 0x0A, /*                   normal page                      as multipage */
        TTX_STD_TOP_BTT_CODE_NRM_PAGE_MA       = 0x0B  /*                   normal page with additional info as multipage */
    } TTX_STD_TOP_BttCodeValues_t;

    typedef uint8_t TTX_STD_TOP_BttListCode_t;

    /* @brief Available values for type TTX_STD_TOP_BttListCode_t.
     * @details Permissible content of Basic TOP Table List (p.28)
     * @note  We seperate it from TTX_STD_TOP_BttListCode_t because 'enum' is treated as type 'int'
     *          by compilers. When we use TTX_STD_TOP_BttListCode_t as an 'enum' member of a struct as:
     *            struct MyStruct
     *            {
     *                TTX_STD_TOP_BttListCode_t myfield : 1;  // ERROR: this bit will be assigned to sign bit of int. (Microsoft VC++ Warning C6299)
     *            };
     */
    typedef enum TTX_STD_TOP_BttListCodeValues_t
    {
        TTX_STD_TOP_BTT_LIST_CODE_NO_DATA              = 0x00,

        /* 0x01~0x08 represents magazine one to eight, respectively */

        TTX_STD_TOP_BTT_LIST_CODE_EXT1                 = 0x09,
        TTX_STD_TOP_BTT_LIST_CODE_EXT2                 = 0x0A,
        TTX_STD_TOP_BTT_LIST_CODE_EXT3                 = 0x0B,
        TTX_STD_TOP_BTT_LIST_CODE_MAG_WITHOUT_TOP      = 0x0C,
        TTX_STD_TOP_BTT_LIST_CODE_MAG_NOT_TRANSMITTED  = 0x0D,
        TTX_STD_TOP_BTT_LIST_CODE_EXT4                 = 0x0E,
        TTX_STD_TOP_BTT_LIST_CODE_EXT5                 = 0x0F
    } TTX_STD_TOP_BttListCodeValues_t;

    typedef uint8_t TTX_STD_SubPage_Displayable_Num_t;


    /* -- Function Prototypes ---------------------------------------------------*/

// Section 15.2 (P. 101)
//bool_t TTX_STD_GetCharSetFromDefaultG0G2 (const uint8_t triplet1, const uint8_t c12c13c14, TTX_STD_CharSet_t* pCharSet);
    bool_t TTX_STD_GetDefaultG0G2FromNatlOpt (const uint8_t c12c13c14, TTX_STD_CharSet_t* pG0CharSet, TTX_STD_CharSet_t* pG2CharSet);
    bool_t TTX_STD_GetSecondG0 (TTX_STD_CharSet_t* pCharSet);
    TTX_STD_Page_Addr_Num_Dec_t TTX_STD_PageAddrNumToDec (const TTX_STD_Page_Addr_Number_t* pPageAddressNumber);
    TTX_STD_Page_Address_Dec_t TTX_STD_PageAddrToDec (const TTX_STD_Page_Addr_t* pPageAddress);

    bool_t TTX_STD_DecToPageAddrNum (const TTX_STD_Page_Addr_Num_Dec_t  pageAddrNum, TTX_STD_Page_Addr_Number_t* pPageAddressNumber);
    bool_t TTX_STD_DecToPageAddr (const TTX_STD_Page_Address_Dec_t   pageAddr, TTX_STD_Page_Addr_t* pPageAddress);

    /* Transforms from TTX_STD_Page_AddrWithSubCode_t to TTX_STD_Page_Addr_t */
    bool_t TTX_STD_RemoveSubCode (const TTX_STD_Page_AddrWithSubCode_t* pFrom, TTX_STD_Page_Addr_t* pTo);

    bool_t TTX_STD_ParseCtrlBits (const uint16_t input, TTX_STD_Page_CtrlBits_t *output);

    bool_t TTX_STD_isPageTerminator (const TTX_STD_Page_AddrWithSubCode_t* pPageAddress);
//uint8_t TTX_STD_isDisplayablePage (const TTX_STD_Page_AddrWithSubCode_t*  pPageAddress) TTX_REENTRANT;
    bool_t TTX_STD_canHaveSubPage (const TTX_STD_Page_AddrWithSubCode_t* pPageAddress);

    bool_t TTX_STD_GetDisplayableSubPageNum (TTX_STD_SubPage_Displayable_Num_t* pSubPageNum, const TTX_STD_Page_Addr_SubCode_t* pSubCode);

    uint8_t TTX_ISR_PageAddrWithSubCodeAreEqual (const TTX_STD_Page_AddrWithSubCode_t* pAddr1, const TTX_STD_Page_AddrWithSubCode_t* pAddr2, uint8_t bCompareSubPage ) TTX_REENTRANT;

    uint8_t TTX_STD_PageAddrWithSubCodeAreEqual (const TTX_STD_Page_AddrWithSubCode_t*  pPageAddress1, const TTX_STD_Page_AddrWithSubCode_t* pPageAddress2, uint8_t bCompareSubPage ) TTX_REENTRANT;
    bool_t TTX_STD_isDisplayablePage2(const TTX_STD_Page_AddrWithSubCode_t*  pPageAddress);
    bool_t TTX_STD_Page_AddrIsValid (const TTX_STD_Page_AddrWithSubCode_t*  pPageAddress);
    bool_t TTX_STD_PageAddrSubCodeIsValid (uint16_t subcode);
    bool_t TTX_STD_MagNumIsValid (const TTX_STD_MagazineNum_t magNum);
//uint8_t TTX_STD_PageAddrNumberIsValid (const TTX_STD_Page_Addr_Number_t pageNumber) TTX_REENTRANT;
//uint8_t TTX_STD_PageAddrSubCodeIsValid (const TTX_STD_Page_Addr_SubCode_t subcode) TTX_REENTRANT;

#define TTX_STD_OddParityDecode(val) (val & 0x7F)
    bool_t TTX_STD_OddParityCheck (const uint8_t val);
    bool_t TTX_STD_Hamming84Decode (uint8_t* des, const uint8_t  src);
    bool_t TTX_STD_Hamming2418Decode (uint8_t* des, const uint8_t* src);


    uint16_t TTX_STD_PageAddrHexToInt(uint16_t pageAddr);
    uint16_t TTX_STD_PageAddrIntToHex(uint16_t pageAddr);
    uint8_t TTX_STD_SubCodeAddrHexToInt(uint16_t SubCode);
    uint8_t TTX_STD_SubCodeAddrIntToHex(uint8_t SubCode);
    TTX_STD_Page_Addr_SubCode_t TTX_STD_SubCodeAToSubcode(TTX_STD_Page_Addr_SubCode_Hex_t subcodeA);
    TTX_STD_Page_Addr_SubCode_Hex_t TTX_STD_SubCodeToSubcodeA(TTX_STD_Page_Addr_SubCode_t subcode);

#ifdef __cplusplus
}   /* extern "C" */
#endif

#endif /* !__ENHANCED_TELETEXT_STANDARD_DECODER_LIBRARY_PLATFORM_INDEPENDENT_PUBLIC_INTERFACE_HEADER_INCLUDED__ */


/*---------------------------------------------------------------------------*\
 * end of ttx_std.h                                                          *
\*---------------------------------------------------------------------------*/
