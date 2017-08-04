
#include "sparrow.h"
#include  "mode.h"
#include "mode_customer.h"

//PDR11956 -Make sure crop value produces an even number.
// Macro to calculate the new Widths and Heights based on a Crop Percent:
/*
UINT8 code EDID_KEY_TABLE[] = 	// EDID for VGA
{
#ifdef CONFIG_PANEL_AU_M170EG01//1280x1024
0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x4A,0x8B,0x00,0x60,0x01,0x00,0x00,0x00,
0x01,0x14,0x01,0x03,0x68,0x22,0x1B,0x78,0xEA,0xF0,0x65,0x98,0x57,0x51,0x91,0x27,
0x21,0x50,0x54,0xAD,0xCF,0x00,0x81,0x80,0x81,0x40,0x71,0x4F,0xD1,0xC0,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x01,0x30,0x2A,0x00,0x98,0x51,0x00,0x2A,0x40,0x30,0x70,
0x13,0x00,0x52,0x0E,0x11,0x00,0x00,0x1E,0x00,0x00,0x00,0xFD,0x00,0x38,0x4B,0x1F,
0x50,0x0E,0x00,0x0A,0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x00,0x00,0xFC,0x00,0x4B,
0x31,0x37,0x0A,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x00,0x00,0xFE,
0x00,0x0A,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00,0xF0,
#else //1366x768
0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x20, 0x32, 0x10, 0x19, 0x01, 0x01, 0x01, 0x01,
0x00, 0x14, 0x01, 0x03, 0x68, 0x29, 0x17, 0x78, 0x2A, 0x3D, 0x85, 0xA6, 0x56, 0x4A, 0x9A, 0x24,
0x12, 0x50, 0x54, 0xAF, 0xCE, 0x00, 0x81, 0xC0, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x66, 0x21, 0x50, 0xB0, 0x51, 0x00, 0x1B, 0x30, 0x40, 0x70,
0x36, 0x00, 0x9A, 0xE6, 0x10, 0x00, 0x00, 0x1E, 0xA9, 0x1A, 0x00, 0xA0, 0x50, 0x00, 0x16, 0x30,
0x30, 0x20, 0x37, 0x00, 0x9A, 0xE6, 0x10, 0x00, 0x00, 0x1A, 0x00, 0x00, 0x00, 0xFD, 0x00, 0x38,
0x4C, 0x1F, 0x3D, 0x09, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFC,
0x00, 0x48, 0x4C, 0x31, 0x39, 0x44, 0x32, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x12,

#endif
};
*/
//forster modified 061228
UINT16 code tVIDEO8_PRESET_TABLE[][5] =
{
    //IH_TOTAL   IH_ACT_STA  IH_ACT_WID  IV_ACT_STA  IV_ACT_LEN
//       {    858,  	146,     	698,      	29,      	232 },  // VIDEO-60(NTSC)
//	{    864,  	154,	  	698,      	37,         278 },  // VIDEO-50(PAL& SECAM)
//      {    858,  	153,     	684,      	31,      	224},//228 },  // VIDEO-60(NTSC)
    {    858,  	148/*151*/,     	688,      	25,      	224},//228 },  // VIDEO-60(NTSC)
    {    864,  	150/*157*/,      	684,      	34,         256},//274 },  // VIDEO-50(PAL& SECAM)

};

UINT16 code tAV_PRESET_TABLE[][5] = //for P¥d//Tim 0325
{
    //	IH_TOTAL	IH_ACT_STA		IH_ACT_WID		IV_ACT_STA	IV_ACT_LEN
//   {	858,		            168,/*168,*/        640,/*688,*/         25,/*31,*/	    226,/*224*/},//228 },  // VIDEO-60(NTSC)
   {	858,		            148,/*173,*//*168,*/        670+20,/*650,*//*688,*/         23,/*25,*//*31,*/	    236-6+4/*226,*//*224*/},//228 },  // VIDEO-60(NTSC)
   //{	864+6,		            161/*155*/,/*193,*/        668/*640*/,/*684,*/         26+6,			    265+8/*256*/},//274 },  // VIDEO-50(PAL& SECAM)
   {    864+6,                  151/*161*//*155*/,/*193,*/         668+20/*640*/,/*684,*/         25,/*31, */               265+6+12/*256*/},//274 },  // VIDEO-50(PAL& SECAM)

};

UINT16 code tSV_PRESET_TABLE[][5] = //for P¥d
{
#if 0
    //	IH_TOTAL	IH_ACT_STA		IH_ACT_WID		IV_ACT_STA	IV_ACT_LEN
    {	858,		168,/*173,*/    640,/*688,*/    22,/*31,*/	226,/*224*/},//228 },  // VIDEO-60(NTSC)
    {	864,		172,/*193,*/    640,/*684,*/    26,			268/*256*/},//274 },  // VIDEO-50(PAL& SECAM)
#else
    //	IH_TOTAL	IH_ACT_STA		IH_ACT_WID		IV_ACT_STA	IV_ACT_LEN
//   {	858,		            168,/*168,*/        640,/*688,*/         25,/*31,*/	    226,/*224*/},//228 },  // VIDEO-60(NTSC)
   {	858,		            155,/*173,*//*168,*/        670,/*650,*//*688,*/         20,/*25,*//*31,*/	    236-6/*226,*//*224*/},//228 },  // VIDEO-60(NTSC)
   //{	864+6,		            161/*155*/,/*193,*/        668/*640*/,/*684,*/         26+6,			    265+8/*256*/},//274 },  // VIDEO-50(PAL& SECAM)
   {    864+6,                  161/*155*/,/*193,*/        668/*640*/,/*684,*/         28,                265+6/*256*/},//274 },  // VIDEO-50(PAL& SECAM)

#endif
};


ModeTableType code tINPUTMODE_PRESET_TABLE[_MAX_PRESET_MODE] =
{
    //--------------------------------------------------
    // Preset Modes
    //--------------------------------------------------
    {   // Mode 0 : 640 x 350 x 70 Hz
        _MODE_640x350_70HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HP_VN | _OVER_LAP_MODE_DOS,                          // Polarity Flags,
        640, /*400,*/350,//reduce DCLK for less than panel max DCLK                                                                 // InputWidth, InputHeight,
        315, 700,                                                                 // IHFreq in kHz, IVFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,       // IHFreqTolerance in kHz, IVFreqTolerance in Hz, Thomas 070326
        800, 449,                                                                 // IHTotal, IVTotal,
        144, /*30,*/61,                                                                  // IHStartPos, IVStartPos,
        448, 										// IVCount
//		2,                                                                           // IVsyncWidth
    },
    {   // Mode 1 : 640 x 400 x 56 Hz
        _MODE_640x400_56HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        640, 400,                                                           // InputWidth, InputHeight,
        248, 564,                                                           // IHFreq in kHz, IVFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,      // IHFreqTolerance in kHz, IVFreqTolerance in Hz,
        848, 440,                                                           // IHTotal, IVTotal,
        144, 33,                                                            // IHStartPos, IVStartPos,
        439, 										// IVCount
//		2,                                                                           // IVsyncWidth
    },
    {   // Mode 2 : 640 x 400 x 70 Hz
        _MODE_640x400_70HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _OVER_LAP_MODE_DOS,          // Polarity Flags,
        640, 400,                                                           // InputWidth, InputHeight,
        314, 700,                                                           // IHFreq in kHz, IVFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,      // IHFreqTolerance in kHz, IVFreqTolerance in Hz,
        800, 449,                                                           // IHTotal, IVTotal,
        145, 25,                                                            // IHStartPos, IVStartPos,
        448, 										// IVCount
//		2,                                                                           // IVsyncWidth
    },
    {   // Mode 3 : 720 x 400 x 70 Hz
        _MODE_720x400_70HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        720, 400,                                                                                // InputWidth, InputHeight,
        315, 700,                                                                                // IHFreq in kHz, IVFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                              // IHFreqTolerance in kHz, IVFreqTolerance in Hz,
        900, 449,                                                                                // IHTotal, IVTotal,
        153, 37,                                                                                  // IHStartPos, IVStartPos,
        448, 										// IVCount
//		2,                                                                                           // IVSyncWidth
    },


    {   // Mode 4 : 700 x 570 x 50 Hz  PAL
        _MODE_700x570_50HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        700, 570,                                                                                                  // InputWidth, InputHeight,
        312, 500,                                                                                                  // IHFreq in kHz, IVFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                                // IHFreqTolerance in kHz, IVFreqTolerance in Hz,
        920, 625,                                                                                                  // IHTotal, IVTotal,
        176, 42,                                                                                                    // IHStartPos, IVStartPos,
        624, 										// IVCount
//		3,
    },
    {   // Mode 5 : 640 x 480 x 60 Hz
        _MODE_640x480_60HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        640, 480,                                                                                                  // InputWidth, InputHeight,
        315, 600,                                                                                                  // IHFreq in kHz, IVFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                                // IHFreqTolerance in kHz, IVFreqTolerance in Hz,
        800, 525,                                                                                                  // IHTotal, IVTotal,
        144, 35,                                                                                                    // IHStartPos, IVStartPos,
        524, 															// IVCount
//		2,                                                                                                             // IVSyncWidth
    },
    {   // Mode 6 : 640 x 480 x 66 Hz
        _MODE_640x480_66HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        640, 480,                                                                                                  // InputWidth, InputHeight,
        350, 666,                                                                                                  // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                                // HFreqTolerance in kHz, VFreqTolerance in Hz,
        864, 525,                                                                                                  // HTotal, VTotal,
        160, 42,                                                                                                    // HStartPos, VStartPos,
        524, 										// IVCount
//		3,
    },
    {   // Mode 7 : 640 x 480 x 70Hz
        _MODE_640x480_70HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HN_VP ,					          // Polarity Flags,
        640, 480,                                                           // InputWidth, InputHeight,
        350, 704,                                                           // IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                 // HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        840, 500,  //515                                                         // HTotal, VTotal,
        168,16/*31*/,                                                            // HStartPos, VStartPos,
        499,//444, 										// IVCount
//		3,                                                                                                             // IVSyncWidth
    },
    {   // Mode 8 : 640 x 480 x 72 Hz
        _MODE_640x480_72HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        640, 480,                                                                                                  // InputWidth, InputHeight,
        378, 728,                                                                                                  // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                                // HFreqTolerance in kHz, VFreqTolerance in Hz,
        832, 520,                                                                                                  // HTotal, VTotal,
        168, 31,                                                                                                    // HStartPos, VStartPos,
        519, 										// IVCount
//		3,                                                                                                             // IVSyncWidth
    },
    {   // Mode 9 : 640 x 480 x 75 Hz
        _MODE_640x480_75HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        640, 480,                                                                                                  // InputWidth, InputHeight,
        375, 750,                                                                                                  // IHFreq in kHz, IVFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                                // IHFreqTolerance in kHz, IVFreqTolerance in Hz,
        840, 500,                                                                                                  // IHTotal, IVTotal,
        184, 19,                                                                                                    // IHStartPos, IVStartPos,
        499, 															// IVCount
//		3,                                                                                                             // IVSyncWidth
    },

    {   // Mode 12 : 640 x 480 x 85 Hz
        _MODE_640x480_85HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        640, 480,                                                           // InputWidth, InputHeight,
        433, 850,                                                           // IHFreq in kHz, IVFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                 // IHFreqTolerance in kHz, IVFreqTolerance in Hz,
        832, 510,                                                           // IHTotal, IVTotal,
        136, 28,                                                            // IHStartPos, IVStartPos,
        508,
    },
/*
    {   // Mode 10 : 800 x 600 x 56 Hz
        _MODE_800x600_56HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        800, 600,                                                                                                  // InputWidth, InputHeight,
        351, 562,                                                                                                  // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                                // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1024, 625,                                                                                                // HTotal, VTotal,
        200, 24,                                                                                                   // HStartPos, VStartPos,
        624, 										// IVCount
//		2,                                                                                                            // IVSyncWidth
    },*/

    {   // Mode 11 : 800 x 600 x 60 Hz
        _MODE_800x600_60HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        800, 600,                                                                                                  // InputWidth, InputHeight,
        378, 603,                                                                                                  // IHFreq in kHz, IVFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                                // IHFreqTolerance in kHz, IVFreqTolerance in Hz,
        1056, 628,                                                                                                // IHTotal, IVTotal,
        216, 27,                                                                                                   // IHStartPos, IVStartPos,
        627, 										// IVCount
//		4,                                                                                                            // IVSyncWidth
    },

    {   // Mode 12 : 800 x 600 x 70 Hz
        _MODE_800x600_70HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        800, 600,                                                                                                  // InputWidth, InputHeight,
        439, 703,                                                                                                  // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                                // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1040, 628,   //-666                                                                                    // HTotal, VTotal,
        200, 25,
        777,                                                                                                  // HStartPos, VStartPos,
//		6,                                                                                                            // IVSyncWidth
    },

    {   // Mode 13 : 800 x 600 x 72 Hz
        _MODE_800x600_72HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        800, 600,                                                                                                  // InputWidth, InputHeight,
        483, 725,                                                                                                  // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                                // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1040, 666,                                                                                                // HTotal, VTotal,
        184, 29,                                                                                                   // HStartPos, VStartPos,
        665, 										// IVCount
//		6,                                                                                                            // IVSyncWidth
    },
    {   // Mode 14 : 800 x 600 x 75 Hz
        _MODE_800x600_75HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        800, 600,                                                                                                  // InputWidth, InputHeight,
        468, 750,                                                                                                  // IHFreq in kHz, IVFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                                // IHFreqTolerance in kHz, IVFreqTolerance in Hz,
        1056, 625,                                                                                                // IHTotal, IVTotal,
        240, 24,                                                                                                   // IHStartPos, IVStartPos,
        624, 										// IVCount
//		3,                                                                                                            // IVSyncWidth
    },
    {   // Mode 15 : 832 x 624 x 75 Hz
        _MODE_832x624_75HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        832, 624,                                                           // InputWidth, InputHeight,
        497, 745,                                                           // IHFreq in kHz, IVFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // IHFreqTolerance in kHz, IVFreqTolerance in Hz,
        1152, 667,                                                         // IHTotal, IVTotal,
        288, 42,                                                            // IHStartPos, IVStartPos,
        667, 										// IVCount
//		3,											// IVSyncWidth
    },

    {   // Mode 16 : 960 x 600 x 60 Hz
        _MODE_960x600_60HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        960, 600,                                                                                                  // InputWidth, InputHeight,
        373, 601,                                                                                                  // IHFreq in kHz, IVFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                                // IHFreqTolerance in kHz, IVFreqTolerance in Hz,
        1226, 624,                                                                                           // IHTotal, IVTotal,
        224, 21,
        777,                                                                                                 // IHStartPos, IVStartPos,
//		4,                                                                                                            // IVSyncWidth
    },
    {   // Mode 17 : 960 x 600 x 72 Hz
        _MODE_960x600_72HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        960, 600,                                                                                                  // InputWidth, InputHeight,
        451, 722,                                                                                                  // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                                // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1236, 627,                                                                                                // HTotal, VTotal,
        190, 25,                                                                                                   // HStartPos, VStartPos,
        665, 															// IVCount
//		6,                                                                                                            // IVSyncWidth
    },
    {   // Mode 18 : 960 x 600 x 75 Hz
        _MODE_960x600_75HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        960, 600,                                                                                                  // InputWidth, InputHeight,
        471, 750,                                                                                                  // IHFreq in kHz, IVFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                                // IHFreqTolerance in kHz, IVFreqTolerance in Hz,
        1256, 627,                                                                                                // IHTotal, IVTotal,
        230, 24,                                                                                                   // IHStartPos, IVStartPos,
        624, 										// IVCount
//		3,                                                                                                            // IVSyncWidth
    },

    {   // Mode 19 : 848 x 480 x 60 Hz
        _MODE_848x480_60HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        848, 480,                                                           // InputWidth, InputHeight,
        310, 600,                                                           // IHFreq in kHz, IVFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // IHFreqTolerance in kHz, IVFreqTolerance in Hz,
        1088, 517,                                                         // IHTotal, IVTotal,
        200, 23,                                                            // IHStartPos, IVStartPos,
        516, 										// IVCount
//		3,											// IVSyncWidth
    },
    {   // Mode 20 : 848 x 480 x 75 Hz
        _MODE_848x480_75HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        848, 480,                                                           // InputWidth, InputHeight,
        376, 747,                                                           // IHFreq in kHz, IVFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // IHFreqTolerance in kHz, IVFreqTolerance in Hz,
        1088, 504,                                                         // IHTotal, IVTotal,
        200, 25,                                                            // IHStartPos, IVStartPos,
        503, 										// IVCount
//		3,											// IVSyncWidth
    },
    {   // Mode 21 : 1024 x 768 x 60 Hz
        _MODE_1024x768_60HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP |_OVER_LAP_MODE_CVT, 		 // Polarity Flags,
        1024, 768,                                                          // InputWidth, InputHeight,
        483, 600,                                                           // IHFreq in kHz, IVFreq in Hz,
        _HFREQ_TOLERANCE+2, _VFREQ_TOLERANCE+2, // IHFreqTolerance in kHz, IVFreqTolerance in Hz,
        1344, 806,                                                         // IHTotal, IVTotal,
        296, 35,                                                            // IHStartPos, IVStartPos,
        805, 										// IVCount
//		6, 											// IVSyncWidth
    },


    {   // Mode 22 : 1024 x 768 x 66 Hz
        _MODE_1024x768_66HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP, 		 // Polarity Flags,
        1024, 768,                                                          // InputWidth, InputHeight,
        539, 661,                                                           // IHFreq in kHz, IVFreq in Hz,
        _HFREQ_TOLERANCE+2, _VFREQ_TOLERANCE+2, // IHFreqTolerance in kHz, IVFreqTolerance in Hz,
        1328, 816,                                                         // IHTotal, IVTotal,
        280, 36,                                                            // IHStartPos, IVStartPos,
        815, 										// IVCount
//		6, 											// IVSyncWidth
    },
    {   // Mode 23 : 1024 x 768 x 70 Hz
        _MODE_1024x768_70HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HP_VP |_OVER_LAP_MODE_CVT,                        // Polarity Flags,
        1024, 768,                                                          // InputWidth, InputHeight,
        564, 700,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1328, 806,                                                         // HTotal, VTotal,
        280, 35,                                                            // HStartPos, VStartPos,
        805, 										// IVCount
//		6,											// IVSyncWidth
    },
    {   //Mode 24 : 1024 x 768 x 72Hz
        _MODE_1024x768_72HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 |_SYNC_HP_VP | _SYNC_HP_VN |_SYNC_HN_VP| _SYNC_HN_VN |_OVER_LAP_MODE_CVT,
        1024,768, 								// InputWidth, InputHeight,
        577, 721, 								// HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,       // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1354, 800, 	//1304							// HTotal, VTotal,
        280, 22,//20,//18, 								// HTotal, VTotal,
        798, 										// IVCount
//		3,											// IVSyncWidth

    },
    {   // Mode 25 : 1024 x 768 x 75 Hz
        _MODE_1024x768_75HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP |_OVER_LAP_MODE_CVT, 		 // Polarity Flags,
        1024, 768,                                                          // InputWidth, InputHeight,
        600, 750,                                                           // IHFreq in kHz, IVFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // IHFreqTolerance in kHz, IVFreqTolerance in Hz,
        1312, 800,                                                          // IHTotal, IVTotal,
        272, 31,                                                            // IHStartPos, IVStartPos,
        799, 										// IVCount
//	    	3,                                                                     // IVSyncWidth
    },

    {   // Mode 23 : 1024 x 768 x 85 Hz
        _MODE_1024x768_85HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP , 		 // Polarity Flags,
        1024, 768,                                                          // InputWidth, InputHeight,
        682, 850,                                                           // IHFreq in kHz, IVFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                 // IHFreqTolerance in kHz, IVFreqTolerance in Hz,
        1376, 808,                                                          // IHTotal, IVTotal,
        304, 39,                                                            // IHStartPos, IVStartPos,
        807,
    },

    {   // Mode 26 : 1024 x 800 x 73 Hz
        _MODE_1024x800_73HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        1024, 800,                                                          // InputWidth, InputHeight,
        620, 740,                                                           // IHFreq in kHz, IVFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // IHFreqTolerance in kHz, IVFreqTolerance in Hz,
        1328, 837,                                                         // IHTotal, IVTotal,
        264, 35,                                                            // IHStartPos, IVStartPos,
        444, 										// IVCount
//		2, // temporately
    },
    {   // Mode 27 : 1152 x 864 x 60 Hz
        _MODE_1152x864_60HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        1152, 864,                                                          // InputWidth, InputHeight,
        537, 600,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1520, 895/* 900 */,                                                         // HTotal, VTotal,
        288, 26,                                                            // HStartPos, VStartPos,
        894, 										// IVCount
//		3,										// IVSyncWidth
    },
    {   // Mode 28 : 1152 x 864 x 70 Hz
        _MODE_1152x864_70HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        1152, 864,                                                          // InputWidth, InputHeight,
        640, 702,              				                        // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1536, 912,//912,        		                              // HTotal, VTotal,
        312, 47,//35,               		                                    // HStartPos, VStartPos,
        911, 										// IVCount
//		3,										// IVSyncWidth
    },
    {   // Mode 29 : 1152 x 864 x 72 Hz
        _MODE_1152x864_72HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        1152, 864,                                                          // InputWidth, InputHeight,
        650, 721,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1526, 900,                                                         // HTotal, VTotal,
        288, 35,                                                           // HStartPos, VStartPos,
        900, 										// IVCount
//		3,										// IVSyncWidth
    },
    {   // Mode 30 : 1152 x 864 x 75 Hz
        _MODE_1152x864_75HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        1152, 864,                                                          // InputWidth, InputHeight,
        675, 750,                                                           // IHFreq in kHz, IVFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // IHFreqTolerance in kHz, IVFreqTolerance in Hz,
        1600, 900,                                                         // IHTotal, IVTotal,
        384, 35,                                                            // IHStartPos, IVStartPos,
        899, 										// IVCount
//		3,										// IVSyncWidth
    },
    {   // Mode 31 : 1152 x 870 x 75 Hz
        _MODE_1152x870_75HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        1152, 870,                                                          // InputWidth, InputHeight,
        686, 750,                                                           // IHFreq in kHz, IVFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // IHFreqTolerance in kHz, IVFreqTolerance in Hz,
        1456, 915,                                                         // IHTotal, IVTotal,
        272, 42,                                                            // IHStartPos, IVStartPos,
        914, 										// IVCount
//		3,										// IVSyncWidth
    },
    {   // Mode 32 : 1152 x 900 x 66 Hz
        _MODE_1152x900_66HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        1152, 900,                                                          // InputWidth, InputHeight,
        618, 660,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1528, 937,                                                         // HTotal, VTotal,
        336, 35,                                                            // HStartPos, VStartPos,
        444, 										// IVCount
//		4,										// IVSyncWidth
    },
    {   // Mode 33 : 1152 x 900 x 76 Hz
        _MODE_1152x900_76HZ | ((_SOURCE_VGA & 0xff) << 8),
        0,          // Polarity Flags,
        1152, 900,                                                          // InputWidth, InputHeight,
        720 , 764,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1472, 943,                                                         // HTotal, VTotal,
        208+96, 33,                                                            // HStartPos, VStartPos,
        777,
//		8,										// IVSyncWidth
    },

    {   // Mode 34 : 1280 x 720 x 60 Hz
        _MODE_1280x720_60HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        1280, 720,                                                          // InputWidth, InputHeight,
        447, 600,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1656, 746,                                                         // HTotal, VTotal,
        300, 25,                                                            // HStartPos, VStartPos,
        749, 										// IVCount
//		5,										// IVSyncWidth
    },
    {   // Mode 35 : 1280 x 720 x 70 Hz
        _MODE_1280x720_70HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        1280, 720,                                                          // InputWidth, InputHeight,
        525, 700,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1688, 750,                                                         // HTotal, VTotal,
        320, 24,                                                            // HStartPos, VStartPos,
        749, 										// IVCount
//		5,										// IVSyncWidth
    },
    {   // Mode 36 : 1280 x 720 x 72 Hz
        _MODE_1280x720_72HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        1280, 720,                                                          // InputWidth, InputHeight,
        541, 720,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1688, 750,                                                         // HTotal, VTotal,
        340, 24,                                                            // HStartPos, VStartPos,
        746, 										// IVCount
//		5,										// IVSyncWidth
    },
    {   // Mode 37 : 1280 x 720 x 75 Hz
        _MODE_1280x720_75HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        1280, 720,                                                          // InputWidth, InputHeight,
        564, 750,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1696, 755,                                                         // HTotal, VTotal,
        336, 32,                                                            // HStartPos, VStartPos,
        444, 										// IVCount
//		5, // temporately
    },
    {   // Mode 38 : 1280 x 768 x 60 Hz
        _MODE_1280x768_60HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN  | _SYNC_HN_VP | _SYNC_HP_VP, 		 // Polarity Flags,
        1280, 768,                                                          // InputWidth, InputHeight,
        477, 600,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1668, 798,                                                         // HTotal, VTotal,
        332, 27,                                                            // HStartPos, VStartPos,
        797, 										// IVCount
//		7,										// IVSyncWidth
    },
    {   // Mode 39 : 1280 x 768 x 60 Hz (Reducing Blanking)
        _MODE_1280x768_60HZ_RB | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP, 		 // Polarity Flags,
        1280, 768,                                                          // InputWidth, InputHeight,
        477, 600,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1440, 790,                                                         // HTotal, VTotal,
        112, 19,                                                            // HStartPos, VStartPos,
        444, 										// IVCount
//		7,										// IVSyncWidth
    },

    {   // Mode 40 : 1280 x 768 x 70 Hz
        _MODE_1280x768_70HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP, 		 // Polarity Flags,
        1280, 768,                                                          // InputWidth, InputHeight,
        560, 700,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1692, 802,                                                         // HTotal, VTotal,
        336, 31,                                                            // HStartPos, VStartPos,
        777,
//		7, // temporately
    },
    {   // Mode 41 : 1280 x 768 x 72 Hz
        _MODE_1280x768_72HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP, 		 // Polarity Flags,
        1280, 768,                                                          // InputWidth, InputHeight,
        579, 720,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1692, 799,                                                         // HTotal, VTotal,
        336, 31,                                                            // HStartPos, VStartPos,
        777,
//	        7, // temporately
    },
    {   // Mode 42 : 1280 x 768 x 75 Hz
        _MODE_1280x768_75HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP, 		 // Polarity Flags,
        1280, 768,                                                          // InputWidth, InputHeight,
        601, 750,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1706, 801,                                                          // HTotal, VTotal,
        346, 33,                                                            // HStartPos, VStartPos,
        801, 										// IVCount
//		7,										// IVSyncWidth
    },

    {   // Mode 43 : 1280 x 800 x 60 Hz
        _MODE_1280x800_60HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP, 		 // Polarity Flags,
        1280, 800,                                                          // InputWidth, InputHeight,
        497, 598,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1680, 831,                                                         // HTotal, VTotal,
        300, 26,                                                            // HStartPos, VStartPos,
        830, 										// IVCount
//		7,										// IVSyncWidth
    },
    {   // Mode 44 : 1280 x 800 x 60 Hz (Reducing Blanking)
        _MODE_1280x800_60HZ_RB | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP, 		 // Polarity Flags,
        1280, 800,                                                          // InputWidth, InputHeight,
        493, 599,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1440, 823,                                                         // HTotal, VTotal,
        112, 20,                                                            // HStartPos, VStartPos,
        822, 										// IVCount
//		7,										// IVSyncWidth
    },
    {   // Mode 45 : 1280 x 800 x 70 Hz
        _MODE_1280x800_70HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP, 		 // Polarity Flags,
        1280, 800,                                                          // InputWidth, InputHeight,
        584, 702,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1696, 832,                                                         // HTotal, VTotal,
        300, 29,                                                            // HStartPos, VStartPos,
        444, 										// IVCount
//		7,										// IVSyncWidth
    },
    {   // Mode 46 : 1280 x 800 x 70 Hz(RB)
        _MODE_1280x800_70HZ_RB | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP, 		 // Polarity Flags,
        1280, 800,                                                          // InputWidth, InputHeight,
        602, 706,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1696, 853,                                                         // HTotal, VTotal,
        300, 29,                                                            // HStartPos, VStartPos,
        444, 										// IVCount
//		7,										// IVSyncWidth
    },
    {   // Mode 47 : 1280 x 800 x 72 Hz
        _MODE_1280x800_72HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP, 		 // Polarity Flags,
        1280, 800,                                                          // InputWidth, InputHeight,
        602, 722,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1716, 832,                                                         // HTotal, VTotal,
        300, 29,                                                            // HStartPos, VStartPos,
        444, 										// IVCount
//		7,										// IVSyncWidth
    },
    {   // Mode 48 : 1280 x 800 x 75 Hz
        _MODE_1280x800_75HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP, 		 // Polarity Flags,
        1280, 800,                                                          // InputWidth, InputHeight,
        627, 749,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1696, 838,                                                         // HTotal, VTotal,
        300, 29,                                                            // HStartPos, VStartPos,
        444, 										// IVCount
//		7,										// IVSyncWidth
    },
    {   // Mode 49 : 1280 x 960 x 60 Hz
        _MODE_1280x960_60HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        1280, 960,                                                          // InputWidth, InputHeight,
        600, 600,                                                           // IHFreq in kHz, IVFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // IHFreqTolerance in kHz, IVFreqTolerance in Hz,
        1800, 1000,                                                       // IHTotal, IVTotal,
        424, 39,                                                            // IHStartPos, IVStartPos,
        999, 										// IVCount
//		3,										// IVSyncWidth
    },
    {   // Mode 50 : 1280 x 960 x 70 Hz
        _MODE_1280x960_70HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        1280, 960,                                                          // InputWidth, InputHeight,
        696, 697,                                                           // IHFreq in kHz, IVFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // IHFreqTolerance in kHz, IVFreqTolerance in Hz,
        1730, 1000,                                                       // IHTotal, IVTotal,
        368, 39,                                                            // IHStartPos, IVStartPos,
        999, 										// IVCount
//		3,										// IVSyncWidth
    },
    {   // Mode 51 : 1280 x 960 x 72 Hz
        _MODE_1280x960_72HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        1280, 960,                                                          // InputWidth, InputHeight,
        722, 722,                                                           // IHFreq in kHz, IVFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // IHFreqTolerance in kHz, IVFreqTolerance in Hz,
        1730, 1000,                                                       // IHTotal, IVTotal,
        368, 39,                                                            // IHStartPos, IVStartPos,
        999, 										// IVCount
//		3,										// IVSyncWidth
    },
    {   // Mode 52 : 1280 x 960 x 75 Hz
        _MODE_1280x960_75HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        1280, 960,                                                          // InputWidth, InputHeight,
        750, 750,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1728, 1002,                                                       // HTotal, VTotal,
        348, 39,                                                            // HStartPos, VStartPos,
        1001, 										// IVCount
//		3, // temporately
    },
    {   // Mode 53 : 1280 x 1024 x 60 Hz
        _MODE_1280x1024_60HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP, 		 // Polarity Flags,
        1280, 1024,                                                         // InputWidth, InputHeight,
        640, 600,                                                           // IHFreq in kHz, IVFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // IHFreqTolerance in kHz, IVFreqTolerance in Hz,
        1688, 1066,                                                        // IHTotal, IVTotal,
        360, 41,                                                            // IHStartPos, IVStartPos,
        1065, 										// IVCount
//		3,										// IVSyncWidth
    },
    {   // Mode 54 : 1280 x 1024 x 70 Hz
        _MODE_1280x1024_70HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        1280, 1024,                                                         // InputWidth, InputHeight,
        746, 700,                                                            // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,          // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1722, 1065,                                                        // HTotal, VTotal,
        359, 40,                                                            // HStartPos, VStartPos,
        444, 										// IVCount
//		2,
    },
    {   // Mode 55 : 1280 x 1024 x 72 Hz
        _MODE_1280x1024_72HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        1280, 1024,                                                         // InputWidth, InputHeight,
        772, 720,                                                            // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE+2, _VFREQ_TOLERANCE,          // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1728, 1070,                                                        // HTotal, VTotal,
        360, 43,                                                            // HStartPos, VStartPos,
        1063, 										// IVCount
//		2,
    },
    {   // Mode 56 : 1280 x 1024 x 75 Hz
        _MODE_1280x1024_75HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        1280, 1024,                                                         // InputWidth, InputHeight,
        800, 750,                                                            // IHFreq in kHz, IVFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,          // IHFreqTolerance in kHz, IVFreqTolerance in Hz,
        1688, 1066,                                                        // IHTotal, IVTotal,
        392, 40,                                                             // IHStartPos, IVStartPos,
        1065, 										// IVCount
//		2,                                                                      // IVSyncWidth
    },

    {   // Mode 57 : 1360 x 768 x 60HZ
        _MODE_1360x768_60HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HP_VP,                                                    // Polarity Flags,
        1360, 768,                                                          // InputWidth, InputHeight,
        475, 598,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE , _VFREQ_TOLERANCE ,
        1782, 794,                                                          // HTotal, VTotal,
        352,23,                                                              // HStartPos, VStartPos,
        794, 										// IVCount
//		2,                                                                      // IVSyncWidth
    },
    {   // Mode 58 : 1440 x 900 x 60 Hz
        _MODE_1440x900_60HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HN_VP | _SYNC_HP_VP | _SYNC_HP_VN,		 // Polarity Flags,
        1440, 900,                                                          // InputWidth, InputHeight,
        559, 598,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1904, 934,                                                         // HTotal, VTotal,
        384, 31,                                                            // HStartPos, VStartPos,
        933, 										// IVCount
//		6,                                                                     // IVSyncWidth
    },
    {   // Mode 59 : 1440 x 900 x 60 Hz  (Reduced Blanking)
        _MODE_1440x900_60RHZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP, 		 // Polarity Flags,
        1440, 900,                                                          // InputWidth, InputHeight,
        557, 600,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1600, 926,                                                         // HTotal, VTotal,
        112, 23,                                                            // HStartPos, VStartPos,
        925, 										// IVCount
//		6,                                                                     // IVSyncWidth
    },

    {   // Mode 60 : 1440 x 900 x 70 Hz
        _MODE_1440x900_70HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        1440, 900,                                                          // InputWidth, InputHeight,
        660, 705,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1936, 936,                                                         // HTotal, VTotal,
        400, 26,                                                            // HStartPos, VStartPos,
        777,
//		3,  // Guest
    },
    {   // Mode 61 : 1440 x 900 x 72 Hz
        _MODE_1440x900_72HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        1440, 900,                                                          // InputWidth, InputHeight,
        680, 725,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1936, 936,                                                         // HTotal, VTotal,
        400, 26,                                                            // HStartPos, VStartPos,
        777,
//		3,  // Guest
    },

    {   // Mode 62 : 1440 x 900 x 75 Hz
        _MODE_1440x900_75HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        1440, 900,                                                          // InputWidth, InputHeight,
        706, 750,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1936, 942,                                                         // HTotal, VTotal,
        400, 39,                                                            // HStartPos, VStartPos,
        941, 										// IVCount
//		3,  // Guest
    },
    {   // Mode 63 : 1600 x 1200 x 50 Hz
        _MODE_1600x1200_50HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        1600, 1200,                                                         // InputWidth, InputHeight,
        617, 499,                                                           // IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                 // HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        2128, 1238,                                                         // HTotal, VTotal,
        432,36,                                                            // HStartPos, VStartPos,
        1237, 										// IVCount
//		3,                                                                     // IVSyncWidth
    },
    {	// Mode 64 : 1600 x 1200 x 60 Hz
        _MODE_1600x1200_60HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP |  _OVER_LAP_MODE_CVT, 		 		// Polarity Flags,
        1600, 1200,								// InputWidth, InputHeight,
        750, 600,									// HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,	// HFreqTolerance in kHz, VFreqTolerance in Hz,
        2160, 1245,								// HTotal, VTotal,
        496, 42,									// HStartPos, VStartPos,
        1245, 										// IVCount
//		3,                                                                     // IVSyncWidth
    },
    {	 // Mode 65 : 1600 x 1200 x 65 Hz
        _MODE_1600x1200_65HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP, 		 // Polarity Flags,
        1600, 1200,								// InputWidth, InputHeight,
        812, 650,									// HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,	// HFreqTolerance in kHz, VFreqTolerance in Hz,
        2160, 1250,								// HTotal, VTotal,
        496, 49,									// HStartPos, VStartPos,
        1249, 										// IVCount
//		6, // guest
    },
    {	 // Mode 66 : 1600 x 1200 x 70 Hz
        _MODE_1600x1200_70HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP, 		 // Polarity Flags,
        1600, 1200,								// InputWidth, InputHeight,
        875, 700,									// HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,	// HFreqTolerance in kHz, VFreqTolerance in Hz,
        2160, 1250,								// HTotal, VTotal,
        496, 49,									// HStartPos, VStartPos,
        444, 										// IVCount
//		6, // guest
    },
    {	 // Mode 67 : 1600 x 1200 x 75 Hz
        _MODE_1600x1200_75HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP, 		 // Polarity Flags,
        1600, 1200,								// InputWidth, InputHeight,
        937, 750,									// HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,	// HFreqTolerance in kHz, VFreqTolerance in Hz,
        2160, 1250,								// HTotal, VTotal,
        496, 49,									// HStartPos, VStartPos,
        444, 										// IVCount
//		6, // guest
    },
    {   // Mode 68 : 1680 x 1050 x 60 Hz
        _MODE_1680x1050_60HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP, 		 // Polarity Flags,
        1680, 1050,                                                         // InputWidth, InputHeight,
        653, 600,                                                            // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,          // HFreqTolerance in kHz, VFreqTolerance in Hz,
        2240, 1089,                                                         // HTotal, VTotal,
        456, 36,                                                             // HStartPos, VStartPos,
        1079, 										// IVCount
//		6,                                                                     // IVSyncWidth
    },
    {   // Mode 69 : 1680 x 1050 x 60 Hz (Reduced Blanking)
        _MODE_1680x1050_60HZ_RB | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP, 		 // Polarity Flags,
        1680, 1050,                                                         // InputWidth, InputHeight,
        647, 600,                                                             // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,           // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1840, 1080,                                                         // HTotal, VTotal,
        112, 27,                                                              // HStartPos, VStartPos,
        1088, 										// IVCount
//		6,                                                                     // IVSyncWidth
    },
    {   // Mode 70 : 1680 x 1050 x 75 Hz
        _MODE_1680x1050_75HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP, 		 // Polarity Flags,
        1680, 1050,                                                         // InputWidth, InputHeight,
        823, 750,                                                            // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,          // HFreqTolerance in kHz, VFreqTolerance in Hz,
        2272, 1099,                                                         // HTotal, VTotal,
        472, 46,                                                             // HStartPos, VStartPos,
        444, 										// IVCount
//		6,  // Guest
    },
    {   // Mode 71 : 1920 x 1080 x 60 @138.5Mhz, XBOX360
        _MODE_1920x1080_60Hz_138 | ((_SOURCE_VGA & 0xff) << 8),
        0 |_SYNC_HP_VN |_OVER_LAP_MODE,        // Polarity Flags,,
        1920, 1080,                                                                 // InputWidth, InputHeight,
        667, 600,                                                                     // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                   // HFreqTolerance in kHz, VFreqTolerance in Hz,
        2080, 1111,                                                                 // HTotal, VTotal,
        42+63,5+20,                                                               // HStartPos, VStartPos,
        444, 										// IVCount
//		5,                                                                               // IVSyncWidth
    },

    {   // Mode 72 : 1920 x 1080 x 60	@148.5Mhz
        _MODE_1920x1080_60Hz_148 | ((_SOURCE_VGA & 0xff) << 8),
        0 |_SYNC_HN_VN | _SYNC_HP_VP |_OVER_LAP_MODE,        			// Polarity Flags,,
        1920, 1080,                                                            // InputWidth, InputHeight,
        675, 600,                                                               // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,             // HFreqTolerance in kHz, VFreqTolerance in Hz,
        2200, 1125/*1106*/,                                             // HTotal, VTotal,
        44+148,41,                                                       // HStartPos, VStartPos,
        444, 										// IVCount
//		5,                                                                         // IVSyncWidth
    },

    {   // Mode 73 : 1920 x 1080 x 60 @173.1Mhz
        _MODE_1920x1080_60Hz_173 | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VP | _OVER_LAP_MODE,        // Polarity Flags,,
        1920, 1080,                                                                 // InputWidth, InputHeight,
        671, 599,                                                                     // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                   // HFreqTolerance in kHz, VFreqTolerance in Hz,
        2576, 1120,                                                                 // HTotal, VTotal,
        200+328,37,                                                                       // HStartPos, VStartPos,
//		5,                                                                               // IVSyncWidth
        444, 										// IVCount
    },

    {   // Mode 73-1 : 1920 x 1080 x 60 @220.75Mhz
        _MODE_1920x1080_60Hz_220 | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VP | _OVER_LAP_MODE,        // Polarity Flags,,
        1920, 1080,                                                                 // InputWidth, InputHeight,
        846, 749,                                                                     // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                   // HFreqTolerance in kHz, VFreqTolerance in Hz,
        2608, 1130,                                                                 // HTotal, VTotal,
        200+328,45,                                                                       // HStartPos, VStartPos,
//		5,                                                                               // IVSyncWidth
        444, 										// IVCount
    },

    {   // Mode 74 : 1920 x 1200x 60 //
        _MODE_1920x1200_60Hz | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,        // Polarity Flags,
        1920, 1200,                                                            // InputWidth, InputHeight,
        740, 599,                                                               // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,             // HFreqTolerance in kHz, VFreqTolerance in Hz,
        2080, 1235,                                                           // HTotal, VTotal,
        112,32,                                                                 // HStartPos, VStartPos,
        444, 										// IVCount
//		5,                                                                         // IVSyncWidth
    },
//[Code Sync][AlanLi][0980527][1]
//add MAC timing
    {   // Mode 75 : 1024 x 576 x 60 Hz //for MAC
        _MODE_1024x576_60HZ | ((_SOURCE_VGA & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        1024, 576,                                                                                                  // InputWidth, InputHeight,
        630, 601,                                                                                                  // IHFreq in kHz, IVFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                                // IHFreqTolerance in kHz, IVFreqTolerance in Hz,
        1300, 599,                                                                                                // IHTotal, IVTotal,
        195, 27,
        777,                                                                                                 // IHStartPos, IVStartPos,
    },
    {   // Mode 76 : 1920 x 1080 x 60 @173.1Mhz //for MAC
        _MODE_1920x1080_60Hz_173MAC | ((_SOURCE_VGA & 0xff) << 8),
        0 |_SYNC_HN_VN ,        		     // Polarity Flags,
        1920, 1080,                                                                 // InputWidth, InputHeight,
        667, 600,                                                                     // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                   // HFreqTolerance in kHz, VFreqTolerance in Hz,
        2380, 1111,                                                                 // HTotal, VTotal,
        428 ,80,                                                               		// HStartPos, VStartPos,
        444, 											// IVCount
    },
//[Code Sync][AlanLi][0980527][1][end]

    // if we move YPbPr mode to another place,
    // remember to change the mapping of mode numbe in CModeSetup()
    // function, because 480i & 576i should enter IP, others should not.
    {   // Mode 86 : 480i
        _MODE_480I | (((_SOURCE_VGA | _SOURCE_YPBPR) & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        	704-20+14, 224+8+1,                                                 // InputWidth, InputHeight,
        	157, 600,                                                           // HFreq in kHz, VFreq in Hz,
        	_HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                 // HFreqTolerance in kHz, VFreqTolerance in Hz,
        	858, 262,                                                           // HTotal, VTotal,
        	129+20-14/2, 22-1,//27,                                             // HStartPos, VStartPos,
        0, 										// IVCount
    },
    {   // Mode 87 : 576i
        _MODE_576I | (((_SOURCE_VGA | _SOURCE_YPBPR) & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        	704-6, 272+10-2,                                                    // InputWidth, InputHeight,
        	156, 500,                                                           // HFreq in kHz, VFreq in Hz,
        	_HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                 // HFreqTolerance in kHz, VFreqTolerance in Hz,
        	864, 312,                                                           // HTotal, VTotal,
        	143+6/2, 24+2/2,//31,                                               // HStartPos, VStartPos,
        0, 										// IVCount
    },
    {   // Mode 88 : 480p
        _MODE_480P | (((_SOURCE_VGA | _SOURCE_YPBPR) & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        	704-20+14, 464+1,                                                   // InputWidth, InputHeight,
        	315, 600,                                                           // HFreq in kHz, VFreq in Hz,
        	_HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                 // HFreqTolerance in kHz, VFreqTolerance in Hz,
        	858, 526,                                                           // HTotal, VTotal,
        	131+10-14/2, 45-1,                                                  // HStartPos, VStartPos,
        0, 										// IVCount
    },
    {   // Mode 89 : 576p
        _MODE_576P | (((_SOURCE_VGA | _SOURCE_YPBPR) & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        	704-14+8, 560-8+7,                                                  // InputWidth, InputHeight,
        	312, 500,                                                           // HFreq in kHz, VFreq in Hz,
        	_HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                 // HFreqTolerance in kHz, VFreqTolerance in Hz,
        	864, 624,                                                           // HTotal, VTotal,
        	141+10-8/2-8, 48+8-7/2-1, // melon  121201 H¼õÏòÓÒ£¬V¼õÏòÏÂ                                              // HStartPos, VStartPos,
        0, 										// IVCount
    },
    {   // Mode 90 : 480p(1440x480)
        _MODE_480P_1440 | (((_SOURCE_VGA | _SOURCE_YPBPR) & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        1368, 456,															// InputWidth, InputHeight,
        315, 600,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                 // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1716, 525,                                                           // HTotal, VTotal,
        184+32, 48,															// HStartPos, VStartPos,
        0, 										// IVCount
    },
    {   // Mode 91 : 576p(1440x576)
        _MODE_576P_1440 | (((_SOURCE_VGA | _SOURCE_YPBPR) & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        1368, 548,															// InputWidth, InputHeight,
        312, 500,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                 // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1728, 624,                                                           // HTotal, VTotal,
        208+32, 56,															// HStartPos, VStartPos,
        0, 										// IVCount
    },
    {   // Mode 92 : 720p50
        _MODE_720P50 | (((_SOURCE_VGA | _SOURCE_YPBPR) & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        1242,698/*684*/ ,                                                    		// InputWidth, InputHeight,
        376, 502,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,         						// HFreqTolerance in kHz, VFreqTolerance in Hz,
        1984/* 1980*/, 750,                                                          // HTotal, VTotal,
        298+18, 26,															// HStartPos, VStartPos,
        0, 										// IVCount
    },
    {   // Mode 93 : 720p60
        _MODE_720P60 | (((_SOURCE_VGA | _SOURCE_YPBPR) & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        1242,698/*684*/ ,                                                    		// InputWidth, InputHeight,
        450, 600,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                 // HFreqTolerance in kHz, VFreqTolerance in Hz,
        1650, 750,                                                          // HTotal, VTotal,
        //288, 42,//VG859															// HStartPos, VStartPos,
        298+18, 26,															// HStartPos, VStartPos,
        0, 										// IVCount
    },
    {   // Mode 94 : 1080i25
        _MODE_1080I25 | (((_SOURCE_VGA | _SOURCE_YPBPR) & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        1862/*1824*/,524/* 512*/,                                                          // InputWidth, InputHeight,
        281, 500,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                 // HFreqTolerance in kHz, VFreqTolerance in Hz,
        2640, 562,                                                          // HTotal, VTotal,
        248+32-18, 32-6,															// HStartPos, VStartPos,
        0, 										// IVCount
    },
    {   // Mode 95 : 1080i30
        _MODE_1080I30 | (((_SOURCE_VGA | _SOURCE_YPBPR) & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
        1862/*1824*/,524/* 512*/,                                                          // InputWidth, InputHeight,
        337, 600,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,									// HFreqTolerance in kHz, VFreqTolerance in Hz,
        2200, 562,                                                          // HTotal, VTotal,
        248+32-18, 32-6,															// HStartPos, VStartPos,
        0, 										// IVCount
    },
    {   // Mode 96 : 1080p50
        _MODE_1080P50 | (((_SOURCE_VGA | _SOURCE_YPBPR) & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
       1862/* 1824*/, 1048/*1026*/,															// InputWidth, InputHeight,
        563, 501,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                 // HFreqTolerance in kHz, VFreqTolerance in Hz,
        2640, 1125,                                                         // HTotal, VTotal,
        248+32-18, 68-12,															// HStartPos, VStartPos,
        0, 										// IVCount
    },

    {   // Mode 97 : 1080p60
        _MODE_1080P60 | (((_SOURCE_VGA | _SOURCE_YPBPR) & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,          // Polarity Flags,
       1862/* 1824*/, 1048/*1026*/,															// InputWidth, InputHeight,
        675, 600,                                                           // HFreq in kHz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                 // HFreqTolerance in kHz, VFreqTolerance in Hz,
        2200, 1125,															// HTotal, VTotal,
        248+32-18, 70-12,															// HStartPos, VStartPos,
        0, 										// IVCount
    },
    {   // Mode 98 : 1080pm50(960x1080)
        _MODE_1080PM50 | (((_SOURCE_VGA | _SOURCE_YPBPR) & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP, 	// Polarity Flags,
        0x03A4, 0x0418,												// InputWidth, InputHeight,
        0x0232, 0x01f4,												// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x0528, 0x045e,												// HTotal, VTotal,
        0x000e+32, 0x0010,												// HStartPos, VStartPos,
        0, 										// IVCount
    },

    {   // Mode 99 : 1080pm60(960x1080)
        _MODE_1080PM60 | (((_SOURCE_VGA | _SOURCE_YPBPR) & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,	// Polarity Flags,
        0x03A4, 0x0418,												// InputWidth, InputHeight,
        0x02a3, 0x0258,												// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x044c, 0x045e,												// HTotal, VTotal,
        0x000e+32, 0x0010,												// HStartPos, VStartPos,
        0, 										// IVCount
    },

    // CSW+ 0971225 Add YPbPr timing 1080P/25HZ/24HZ/23.97HZ/29.97HZ/30HZ
    // [Code Sync][CSW][0980410][1]
    // Double DVS to solve 1080p24 & 1080p30 video flicker
    {   // Mode 100 : 1080p24 (24 Hz)
        _MODE_1080P24, 0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,           // Polarity Flags,
        1840, 1032,                                                                  // InputWidth, InputHeight,
        270, 240,                                                                                         // IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                                  // HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        2750, 1125,                                                                                         // HTotal, VTotal,
        /*246*/276, 58,                                                                                         // HStartPos, VStartPos,
        0, 										// IVCount
    },

    {   // Mode 102 : 1080p25 (25 Hz)
        _MODE_1080P25, 0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,           // Polarity Flags,
        1824, 1026,                                                                // InputWidth, InputHeight,
        281, 250,                                                                                         // IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                                  // HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        2640, 1125,                                                                                         // HTotal, VTotal,
        /*246*/284, 58,                                                                                         // HStartPos, VStartPos,
        0, 										// IVCount
    },

    {   // Mode 104 : 1080p30 (30 Hz)
        _MODE_1080P30, 0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,           // Polarity Flags,
        0x0730, 0x0408,                                                               // InputWidth, InputHeight,
        0x0151, 0x012c,                                                                                         // IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,                                                  // HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x0898, 0x0465,                                                                                         // HTotal, VTotal,
        0x00f5+32, 0x003e,                                                                                         // HStartPos, VStartPos,
        0, 										// IVCount
    },

    {   // Mode 00 : 480i
        _HDMI_MODE_480I | ((_SOURCE_HDMI & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,	// Polarity Flags,
        720, 243,//720, 240,												// InputWidth, InputHeight,
        0x009D, 0x0258,												// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x035A, 262,													// HTotal, VTotal,
        116, 18,  //0x000a,0x0003												// HStartPos, VStartPos,
        0, 										// IVCount
    },

    {   // Mode 01 : 576i
        _HDMI_MODE_576I | ((_SOURCE_HDMI & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,	// Polarity Flags,
        720, 295,	//288											// InputWidth, InputHeight,
        0x009C, 0x01F4,												// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x0360, 312,													// HTotal, VTotal,
        129, 19,//22,	//0x000a,0x0004													// HStartPos, VStartPos,
        0, 										// IVCount
    },

    {   // Mode 02 : 480p
        _HDMI_MODE_480P | ((_SOURCE_HDMI & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,	// Polarity Flags,
        720, 490,	// 720, 480,											// InputWidth, InputHeight,
        0x013B, 0x0258,												// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x035A, 525,													// HTotal, VTotal,
         120, 34,//126, 44,	//0x000a,0x0007												// HStartPos, VStartPos,
        0, 										// IVCount
    },

    {   // Mode 03 : 576p
        _HDMI_MODE_576P | ((_SOURCE_HDMI & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,	// Polarity Flags,
        720, 591,//576,												// InputWidth, InputHeight,
        0x0138, 0x01F4,												// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x0360, 625,													// HTotal, VTotal,
        130/*132*/, 38,//48,	//0x000a,0x0008												// HStartPos, VStartPos,
        0, 										// IVCount
    },

    {   // Mode 04 : 720p50
        _HDMI_MODE_720P50 | ((_SOURCE_HDMI & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,	// Polarity Flags,
        1280,740,//1280, 720,												// InputWidth, InputHeight,
        0x0177, 0x01f4,												// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x07bc, 750,													// HTotal, VTotal,
        220,12,//688, 29,												// HStartPos, VStartPos,
        0, 										// IVCount
    },

    {   // Mode 05 : 720p60
        _HDMI_MODE_720P60 | ((_SOURCE_HDMI & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,	// Polarity Flags,
        1280, 730,//1280, 720, 												// InputWidth, InputHeight,
        0x01C2, 0x0258,												// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x0672, 750,													// HTotal, VTotal,
        220,20,//358, 29,												// HStartPos, VStartPos,
        0, 										// IVCount
    },

    {   // Mode 06 : 1080i25
        _HDMI_MODE_1080I25 | ((_SOURCE_HDMI & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,	// Polarity Flags,
        1920, 556, //1920, 540,	/* 0x0748, 0x020C,*/												// InputWidth, InputHeight,
        0x0119, 0x01F4,												// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x0a50, 0x0228,												// HTotal, VTotal,
        145,8,//145, 15,												// HStartPos, VStartPos,
        0, 										// IVCount
    },

    {   // Mode 07 : 1080i30
        _HDMI_MODE_1080I30 | ((_SOURCE_HDMI & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,	// Polarity Flags,
        1920, 548,	/* 0x0748, 0x0418,  */										// InputWidth, InputHeight,
        0x0151, 0x0258,												// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x0898,  0x022A,												// HTotal, VTotal,
        145, 15,												// HStartPos, VStartPos,
        0, 										// IVCount
    },

    {   // Mode 08 : 1080p50
        _HDMI_MODE_1080P50 | ((_SOURCE_HDMI & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,	// Polarity Flags,
        1920, 1115, //1920, 1080, 												// InputWidth, InputHeight,
        0x0232, 0x01f4,												// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x0a50, 0x045e, 												// HTotal, VTotal,
        145, 20,//44,												// HStartPos, VStartPos,
        0, 										// IVCount
    },

    {   // Mode 09 : 1080p60
        _HDMI_MODE_1080P60 | ((_SOURCE_HDMI & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,	// Polarity Flags,
        1920, 1096,	///1920,/*0x0748,*/ 1080,												// InputWidth, InputHeight,
        0x02a3, 0x0258,												// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x0898, 0x045e,												// HTotal, VTotal,
        145, 36,// 268, 44,												// HStartPos, VStartPos,
        0, 										// IVCount
    },

    {   // Mode 10 : 1080pm50(960x1080)
        _HDMI_MODE_1080PM50 | ((_SOURCE_HDMI & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP, 	// Polarity Flags,
        960, 1080,												// InputWidth, InputHeight,
        0x0232, 0x01f4,												// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x0528, 0x0466,//0x0528, 0x045e,												// HTotal, VTotal,
        348,44,//708, 44,										// HStartPos, VStartPos,
        0, 										// IVCount
    },


    {   // Mode 11 : 1080pm60(960x1080)
        _HDMI_MODE_1080PM60 | ((_SOURCE_HDMI & 0xff) << 8),
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,	// Polarity Flags,
        960, 1080,												// InputWidth, InputHeight,
        0x02a3, 0x0258,												// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x044c, 0x0466,//0x044c, 0x045e,										// HTotal, VTotal,
        136, 44,		//268, 44,									// HStartPos, VStartPos,
        0, 										// IVCount
    },
    // [Code Sync][CSW][0980410][1]
    // Double DVS to solve 1080p24 & 1080p30 video flicker
    // Add  HDMI 1080P24 &  HDMI 1080P30
    {   // Mode 12 : 1080p24
        _HDMI_MODE_1080P24, 0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,		// Polarity Flags,
        1920, 1080,//0x06e0,/*0x0748,*/ 0x0418,								// InputWidth, InputHeight,
        0x010e, 0x00F0,	//270,240											// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x0ABD, 0x464,	//2749,1124										// HTotal, VTotal,
        818, 44,											// HStartPos, VStartPos,
        0, 										// IVCount
    },


    {   // Mode 13 : 1080p30
        _HDMI_MODE_1080P30, 0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,		// Polarity Flags,
        1920, 1080,//0x06e0,/*0x0748,*/ 0x0418,								// InputWidth, InputHeight,
        0x0151, 0x012C,	//337,300											// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x0898, 0x465,	//2200,1125										// HTotal, VTotal,
        160/*268*/, 44,											// HStartPos, VStartPos,
        0, 										// IVCount
    },
    {   // Mode 00 : 480i
        _AV_MODE_480I | ((_SOURCE_CVBS & 0xff) << 8),
        0,										// not used Polarity Flags,
        640, 226,									// InputWidth, InputHeight,
        0, 	0,									// not used IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,	// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        858, 262,									// HTotal, VTotal(not used),
        168, 25, 									// HStartPos, VStartPos,
        0, 										// IVCount
    },

    {   // Mode 01 : 576i
        _AV_MODE_576I | ((_SOURCE_CVBS & 0xff) << 8),
        0,										// not used Polarity Flags,
        640, 272,									// InputWidth, InputHeight,
        0, 	0,									// not used IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,	// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        864, 262,									// HTotal, VTotal(not used),
        172, 29, 									// HStartPos, VStartPos,
        0, 										// IVCount
    },
    {   // Mode 00 : 480i
        _SV_MODE_480I | ((_SOURCE_SV & 0xff) << 8),
        0,										// not used Polarity Flags,
        640, 226,									// InputWidth, InputHeight,
        0, 0,										// not used IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,	// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        858, 262,									// HTotal, VTotal(not used),
        168, 22, 									// HStartPos, VStartPos,
        0, 										// IVCount
    },

    {   // Mode 01 : 576i
        _SV_MODE_576I | ((_SOURCE_SV & 0xff) << 8),
        0,										// not used Polarity Flags,
        640, 226,									// InputWidth, InputHeight,
        0, 0,										// not used IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,	// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        858, 262,									// HTotal, VTotal(not used),
        168, 28, 									// HStartPos, VStartPos,
        0, 										// IVCount
    },
    {   // Mode 00 : 480i
        _TV_MODE_480I | ((_SOURCE_TV & 0xff) << 8),
        0,										// not used Polarity Flags,
        640, 226,									// InputWidth, InputHeight,
        0, 0,										// not used IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,	// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        858, 262,									// HTotal, VTotal(not used),
        168, 25, 									// HStartPos, VStartPos,
        0, 										// IVCount
    },

    {   // Mode 01 : 576i
        _TV_MODE_576I | ((_SOURCE_TV & 0xff) << 8),
        0,										// not used Polarity Flags,
        640, 272,									// InputWidth, InputHeight,
        0, 0,										// not used IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,	// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        864, 262,									// HTotal, VTotal(not used),
        172, 29, 									// HStartPos, VStartPos,
        0, 										// IVCount
    },
    {   // Mode 12 : NEW
        _MODE_NEW,
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,	// Polarity Flags,
        0x03A4, 0x0418,												// InputWidth, InputHeight,
        0x02a3, 0x0258,												// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x044c, 0x045e, 												// HTotal, VTotal,
        0x0024, 0x0010,												// HStartPos, VStartPos,
        0, 										// IVCount
    }
};

#if 0	// not used
ModeTableType code tINPUTMODE_HDMI_TABLE[_MAX_HDMI_MODE] =
{

    {   // Mode 00 : 480i
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,	// Polarity Flags,
        0x02BC, 0x00EA,												// InputWidth, InputHeight,
        0x009D, 0x0258,												// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x035A, 262,													// HTotal, VTotal,
        0x0004, 0x0003,  //0x000a,0x0003												// HStartPos, VStartPos,
        0, 										// IVCount
    },

    {   // Mode 01 : 576i
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,	// Polarity Flags,
        0x02BC, 0x0118,												// InputWidth, InputHeight,
        0x009C, 0x01F4,												// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x0360, 312,													// HTotal, VTotal,
        0x0004, 0x0004,	//0x000a,0x0004													// HStartPos, VStartPos,
        0, 										// IVCount
    },

    {   // Mode 02 : 480p
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,	// Polarity Flags,
        0x02BC, 0x01D2,												// InputWidth, InputHeight,
        0x013B, 0x0258,												// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x035A, 525,													// HTotal, VTotal,
        0x0006, 0x0007,	//0x000a,0x0007												// HStartPos, VStartPos,
        0, 										// IVCount
    },

    {   // Mode 03 : 576p
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,	// Polarity Flags,
        0x02BC, 0x0230,												// InputWidth, InputHeight,
        0x0138, 0x01F4,												// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x0360, 625,													// HTotal, VTotal,
        0x0006, 0x0008,	//0x000a,0x0008												// HStartPos, VStartPos,
        0, 										// IVCount
    },

    {   // Mode 04 : 720p50
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,	// Polarity Flags,
        0x04D8, 0x02BC,												// InputWidth, InputHeight,
        0x0177, 0x01f4,												// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x07bc, 750,													// HTotal, VTotal,
        0x0014, 0x000a,												// HStartPos, VStartPos,
        0, 										// IVCount
    },

    {   // Mode 05 : 720p60
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,	// Polarity Flags,
        0x04D8, 0x02BC, 												// InputWidth, InputHeight,
        0x01C2, 0x0258,												// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x0672, 750,													// HTotal, VTotal,
        0x0014, 0x000a,												// HStartPos, VStartPos,
        0, 										// IVCount
    },

    {   // Mode 06 : 1080i25
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,	// Polarity Flags,
        0x06f0, 0x0204,	/* 0x0748, 0x020C,*/												// InputWidth, InputHeight,
        0x0119, 0x01F4,												// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x0a50, 0x0228,												// HTotal, VTotal,
        0x0044, 0x0010,												// HStartPos, VStartPos,
        0, 										// IVCount
    },

    {   // Mode 07 : 1080i30
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,	// Polarity Flags,
        0x06f0, 0x0204,	/* 0x0748, 0x0418,  */										// InputWidth, InputHeight,
        0x0151, 0x0258,												// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x0898,  0x022A,												// HTotal, VTotal,
        0x0044, 0x0010,												// HStartPos, VStartPos,
        0, 										// IVCount
    },

    {   // Mode 08 : 1080p50
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,	// Polarity Flags,
        0x0748, 0x0418, 												// InputWidth, InputHeight,
        0x0232, 0x01f4,												// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x0a50, 0x045e, 												// HTotal, VTotal,
        0x0018, 0x0010,												// HStartPos, VStartPos,
        0, 										// IVCount
    },

    {   // Mode 09 : 1080p60
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,	// Polarity Flags,
        0x06e0,/*0x0748,*/ 0x0418,												// InputWidth, InputHeight,
        0x02a3, 0x0258,												// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x0898, 0x045e,												// HTotal, VTotal,
        0x0048, 0x0013,												// HStartPos, VStartPos,
        0, 										// IVCount
    },

    {   // Mode 10 : 1080pm50(960x1080)
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP, 	// Polarity Flags,
        0x03A4, 0x0418,												// InputWidth, InputHeight,
        0x0232, 0x01f4,												// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x0528, 0x045e,												// HTotal, VTotal,
        0x000e, 0x0010,												// HStartPos, VStartPos,
        0, 										// IVCount
    },


    {   // Mode 11 : 1080pm60(960x1080)
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,	// Polarity Flags,
        0x03A4, 0x0418,												// InputWidth, InputHeight,
        0x02a3, 0x0258,												// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x044c, 0x045e,												// HTotal, VTotal,
        0x000e, 0x0010,												// HStartPos, VStartPos,
        0, 										// IVCount
    },

    // [Code Sync][CSW][0980410][1]
    // Double DVS to solve 1080p24 & 1080p30 video flicker
    // Add  HDMI 1080P24 &  HDMI 1080P30
    {   // Mode 12 : 1080p24
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,		// Polarity Flags,
        1920, 1080,//0x06e0,/*0x0748,*/ 0x0418,								// InputWidth, InputHeight,
        0x010e, 0x00F0,	//270,240											// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x0ABD, 0x464,	//2749,1124										// HTotal, VTotal,
        0x015e - 0x0051, 0x0036,											// HStartPos, VStartPos,
        0, 										// IVCount
    },


    {   // Mode 13 : 1080p30
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,		// Polarity Flags,
        1920, 1080,//0x06e0,/*0x0748,*/ 0x0418,								// InputWidth, InputHeight,
        0x0151, 0x012C,	//337,300											// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x0898, 0x465,	//2200,1125										// HTotal, VTotal,
        0x015e - 0x0051, 0x0036,											// HStartPos, VStartPos,
        0, 										// IVCount
    },

    {   // Mode 14 : NEW
        0 | _SYNC_HN_VN | _SYNC_HP_VN | _SYNC_HN_VP | _SYNC_HP_VP,	// Polarity Flags,
        0x03A4, 0x0418,												// InputWidth, InputHeight,
        0x02a3, 0x0258,												// IHFreq in 100Hz, VFreq in Hz,
        _HFREQ_TOLERANCE, _VFREQ_TOLERANCE,							// HFreqTolerance in 100Hz, VFreqTolerance in 0.1Hz,
        0x044c, 0x045e, 												// HTotal, VTotal,
        0x0024, 0x0010,												// HStartPos, VStartPos,
        0, 										// IVCount
    }
};
#endif

ModeTableTypeTable stModeTableTypeTable =
{
    sizeof(tINPUTMODE_PRESET_TABLE) / sizeof(ModeTableType),
    tINPUTMODE_PRESET_TABLE
};
