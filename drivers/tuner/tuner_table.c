
#include "ifd\ifd.h"
#include "ifd\ifd_factorymode.h"
#include "ifd\ifd_sparrow.h"

StructTunerDataType IFD_XDATA gTunerData[_Tuner_MAX_NO];
//=================================================================
/**
 * tuner_data_init
 *
 *
 * @param {void}
 * @return {void}
 */
#ifdef _VIDEO_TV_SUPPORT 
#ifndef CONFIG_TUNER_SILICON
void tuner_data_init(void)
{
    UINT8 i =0;
#ifdef CONFIG_TUNER_XUGUANG_TDQ6K_T116CW
    if ((i+1)>_Tuner_MAX_NO)
    {
        IFD_DEBUG_MSG(IFD_LOG_LEVEL, "The number of tuner selected exceed the Tuner Max Number.\n");
        while (1);
    }
    gTunerData[i].tunerI2C_ADD = 0xc0; // tuner i2c device address
    gTunerData[i].tunerMinFreq = 432500;
    gTunerData[i].tunerVHF_LowFreq = 1442500;
    gTunerData[i].tunerVHF_HighFreq = 4322500;
    gTunerData[i].tuner_MaxFreq = 8632500;
    gTunerData[i].tunerPIF = 389000;
    gTunerData[i].tunerRATIOStepSize = 500;//625;

    gTunerData[i].tunerBandFreq[0] = 1442500;
    gTunerData[i].tunerBandFreq[1] = 4322500;
    gTunerData[i].tunerBandFreq[2] = 8632500;
    gTunerData[i].tunerRegisterByte[0] = 0x8c; //band1
    gTunerData[i].tunerRegisterByte[1] = 0x01;
    gTunerData[i].tunerRegisterByte[2] = 0x8c; //band2
    gTunerData[i].tunerRegisterByte[3] = 0x02;
    gTunerData[i].tunerRegisterByte[4] = 0x8c; //band3
    gTunerData[i].tunerRegisterByte[5] = 0x08;
    gTunerData[i].tunerRegisterByteSearch[0] = 0xcc; //band1
    gTunerData[i].tunerRegisterByteSearch[1] = 0x01;
    gTunerData[i].tunerRegisterByteSearch[2] = 0xcc; //band2
    gTunerData[i].tunerRegisterByteSearch[3] = 0x02;
    gTunerData[i].tunerRegisterByteSearch[4] = 0xcc; //band3
    gTunerData[i].tunerRegisterByteSearch[5] = 0x08;

    gTunerData[i].tunerBandNo = 3;
    gTunerData[i].tunerRegNoPerBand= 2;
    gTunerData[i].tunerUsedTag = _FALSE;
    gTunerData[i].tunerAGCtype = TUNER_RFAGC_IFAGC_IC_NONE;//TUNER_NONE_IC_RFAGC;
    gTunerData[i].tunerType = TUNER_NORMAL;
    gTunerData[i].tunerSpecificTag = 0;
    gTunerData[i].tunerID = TUNER_XUGUANG_TDQ6K_T116CW;

    i++;
#elif defined(CONFIG_TUNER_TCL_F41CT_2_E)
    if ((i+1)>_Tuner_MAX_NO)
    {
        IFD_DEBUG_MSG(IFD_LOG_LEVEL, "The number of tuner selected exceed the Tuner Max Number.\n");
        while (1);
    }
    gTunerData[i].tunerI2C_ADD = 0xc0; // tuner i2c device address
    gTunerData[i].tunerMinFreq = 482500;
    gTunerData[i].tunerVHF_LowFreq = 1472500;
    gTunerData[i].tunerVHF_HighFreq = 4232500;
    gTunerData[i].tuner_MaxFreq = 8632500;
    gTunerData[i].tunerPIF = 380000;
    gTunerData[i].tunerRATIOStepSize = 625;

    gTunerData[i].tunerBandFreq[0] = 1472500;
    gTunerData[i].tunerBandFreq[1] = 4232500;
    gTunerData[i].tunerBandFreq[2] = 8632500;
    gTunerData[i].tunerRegisterByte[0] = 0x8e; //band1
    gTunerData[i].tunerRegisterByte[1] = 0x01;
    gTunerData[i].tunerRegisterByte[2] = 0x8e; //band2
    gTunerData[i].tunerRegisterByte[3] = 0x02;
    gTunerData[i].tunerRegisterByte[4] = 0x8e; //band3
    gTunerData[i].tunerRegisterByte[5] = 0x08;
    gTunerData[i].tunerRegisterByteSearch[0] = 0xce; //band1
    gTunerData[i].tunerRegisterByteSearch[1] = 0x01;
    gTunerData[i].tunerRegisterByteSearch[2] = 0xce; //band2
    gTunerData[i].tunerRegisterByteSearch[3] = 0x02;
    gTunerData[i].tunerRegisterByteSearch[4] = 0xce; //band3
    gTunerData[i].tunerRegisterByteSearch[5] = 0x08;

    gTunerData[i].tunerBandNo = 3;
    gTunerData[i].tunerRegNoPerBand= 2;
    gTunerData[i].tunerUsedTag = _FALSE;
    gTunerData[i].tunerAGCtype = TUNER_NONE_IC_RFAGC;
    gTunerData[i].tunerType = TUNER_NORMAL;
    gTunerData[i].tunerSpecificTag = 0;
    gTunerData[i].tunerID = TUNER_TCL_F41CT_2_E;

    i++;

#elif defined(CONFIG_TUNER_TCL_F13WP_2DP_E)
    if ((i+1)>_Tuner_MAX_NO)
    {
        IFD_DEBUG_MSG(IFD_LOG_LEVEL, "The number of tuner selected exceed the Tuner Max Number.\n");
        while (1);
    }
    gTunerData[i].tunerI2C_ADD = 0xc0; // tuner i2c device address
    gTunerData[i].tunerMinFreq = 432500;
    gTunerData[i].tunerVHF_LowFreq = 1522500;
    gTunerData[i].tunerVHF_HighFreq = 4402500;
    gTunerData[i].tuner_MaxFreq = 8632500;
    gTunerData[i].tunerPIF = 380000;
    gTunerData[i].tunerRATIOStepSize = 625;

    gTunerData[i].tunerBandFreq[0] = 1522500;
    gTunerData[i].tunerBandFreq[1] = 4402500;
    gTunerData[i].tunerBandFreq[2] = 8632500;
    gTunerData[i].tunerRegisterByte[0] = 0x8e; //band1
    gTunerData[i].tunerRegisterByte[1] = 0x01;
    gTunerData[i].tunerRegisterByte[2] = 0x8e; //band2
    gTunerData[i].tunerRegisterByte[3] = 0x02;
    gTunerData[i].tunerRegisterByte[4] = 0x8e; //band3
    gTunerData[i].tunerRegisterByte[5] = 0x04;
    gTunerData[i].tunerRegisterByteSearch[0] = 0xce; //band1
    gTunerData[i].tunerRegisterByteSearch[1] = 0x01;
    gTunerData[i].tunerRegisterByteSearch[2] = 0xce; //band2
    gTunerData[i].tunerRegisterByteSearch[3] = 0x02;
    gTunerData[i].tunerRegisterByteSearch[4] = 0xce; //band3
    gTunerData[i].tunerRegisterByteSearch[5] = 0x04;

    gTunerData[i].tunerBandNo = 3;
    gTunerData[i].tunerRegNoPerBand= 2;
    gTunerData[i].tunerUsedTag = _FALSE;
    gTunerData[i].tunerAGCtype = TUNER_NONE_IC_RFAGC;
    gTunerData[i].tunerType = TUNER_NORMAL;
    gTunerData[i].tunerSpecificTag = 0;
    gTunerData[i].tunerID = TUNER_TCL_F13WP_2DP_E;

    i++;

#elif defined(CONFIG_TUNER_LG_TDTC_G29D)
    if ((i+1)>_Tuner_MAX_NO)
    {
        IFD_DEBUG_MSG(IFD_LOG_LEVEL, "The number of tuner selected exceed the Tuner Max Number.\n");
        while (1);
    }
    gTunerData[i].tunerI2C_ADD = 0xc0; // tuner i2c device address
    gTunerData[i].tunerMinFreq = 482500;
    gTunerData[i].tunerVHF_LowFreq = 1542500;
    gTunerData[i].tunerVHF_HighFreq = 4232500;
    gTunerData[i].tuner_MaxFreq = 8552500;
    gTunerData[i].tunerPIF = 389000;
    gTunerData[i].tunerRATIOStepSize = 500;

    gTunerData[i].tunerBandFreq[0] = 1542500;
    gTunerData[i].tunerBandFreq[1] = 4242500;
    gTunerData[i].tunerBandFreq[2] = 8552500;
    gTunerData[i].tunerRegisterByte[0] = 0xbd;//band1
    gTunerData[i].tunerRegisterByte[1] = 0x01;
    gTunerData[i].tunerRegisterByte[2] = 0xd1;
    gTunerData[i].tunerRegisterByte[3] = 0xbd;//band2
    gTunerData[i].tunerRegisterByte[4] = 0x02;
    gTunerData[i].tunerRegisterByte[5] = 0xd1;
    gTunerData[i].tunerRegisterByte[6] = 0xbd;//band3
    gTunerData[i].tunerRegisterByte[7] = 0x08;
    gTunerData[i].tunerRegisterByte[8] = 0xd1;
    gTunerData[i].tunerRegisterByteSearch[0] = 0xbd;//band1
    gTunerData[i].tunerRegisterByteSearch[1] = 0x01;
    gTunerData[i].tunerRegisterByteSearch[2] = 0xf1;
    gTunerData[i].tunerRegisterByteSearch[3] = 0xbd;//band2
    gTunerData[i].tunerRegisterByteSearch[4] = 0x02;
    gTunerData[i].tunerRegisterByteSearch[5] = 0xf1;
    gTunerData[i].tunerRegisterByteSearch[6] = 0xbd;//band3
    gTunerData[i].tunerRegisterByteSearch[7] = 0x08;
    gTunerData[i].tunerRegisterByteSearch[8] = 0xf1;

    gTunerData[i].tunerBandNo = 3;
    gTunerData[i].tunerRegNoPerBand= 2;
    gTunerData[i].tunerUsedTag = _FALSE;
    gTunerData[i].tunerAGCtype = TUNER_NONE_IC_RFAGC;
    gTunerData[i].tunerType = TUNER_NORMAL;
    gTunerData[i].tunerSpecificTag = 0;
    gTunerData[i].tunerID = TUNER_LG_TDTC_G29D;

    i++;


#elif defined(CONFIG_TUNER_QINGJIA_AFT7_W000G)
    if ((i+1)>_Tuner_MAX_NO)
    {
        IFD_DEBUG_MSG(IFD_LOG_LEVEL, "The number of tuner selected exceed the Tuner Max Number.\n");
        while (1);
    }
    gTunerData[i].tunerI2C_ADD = 0xc0; // tuner i2c device address
    gTunerData[i].tunerMinFreq = 497500;
    gTunerData[i].tunerVHF_LowFreq = 1602500;
    gTunerData[i].tunerVHF_HighFreq = 4642500;
    gTunerData[i].tuner_MaxFreq = 8632500;
    gTunerData[i].tunerPIF = 380000;
    gTunerData[i].tunerRATIOStepSize = 625;

    gTunerData[i].tunerBandFreq[0] = 1602500;
    gTunerData[i].tunerBandFreq[1] = 4642500;
    gTunerData[i].tunerBandFreq[2] = 8632500;
    gTunerData[i].tunerRegisterByte[0] = 0x8e; //band1
    gTunerData[i].tunerRegisterByte[1] = 0x01;
    gTunerData[i].tunerRegisterByte[2] = 0x8e; //band2
    gTunerData[i].tunerRegisterByte[3] = 0x02;
    gTunerData[i].tunerRegisterByte[4] = 0x8e; //band3
    gTunerData[i].tunerRegisterByte[5] = 0x08;
    gTunerData[i].tunerRegisterByteSearch[0] = 0xce; //band1
    gTunerData[i].tunerRegisterByteSearch[1] = 0x01;
    gTunerData[i].tunerRegisterByteSearch[2] = 0xce; //band2
    gTunerData[i].tunerRegisterByteSearch[3] = 0x02;
    gTunerData[i].tunerRegisterByteSearch[4] = 0xce; //band3
    gTunerData[i].tunerRegisterByteSearch[5] = 0x08;

    gTunerData[i].tunerBandNo = 3;
    gTunerData[i].tunerRegNoPerBand= 2;
    gTunerData[i].tunerUsedTag = _FALSE;
    gTunerData[i].tunerAGCtype = TUNER_NONE_IC_RFAGC;
    gTunerData[i].tunerType = TUNER_NORMAL;
    gTunerData[i].tunerSpecificTag = 0;
    gTunerData[i].tunerID = TUNER_XUGUANG_TDQ6K_T116CW;

    i++;

#elif defined(CONFIG_TUNER_KEH_FTDC3S20V04)
    if ((i+1)>_Tuner_MAX_NO)
    {
        IFD_DEBUG_MSG(IFD_LOG_LEVEL, "The number of tuner selected exceed the Tuner Max Number.\n");
        while (1);
    }
    gTunerData[i].tunerI2C_ADD = 0xc0; // tuner i2c device address
    gTunerData[i].tunerMinFreq = 497500;
    gTunerData[i].tunerVHF_LowFreq = 1362500;
    gTunerData[i].tunerVHF_HighFreq = 4242500;
    gTunerData[i].tuner_MaxFreq = 8632500;
    gTunerData[i].tunerPIF = 380000;
    gTunerData[i].tunerRATIOStepSize = 625;

    gTunerData[i].tunerBandFreq[0] = 1362500;
    gTunerData[i].tunerBandFreq[1] = 4242500;
    gTunerData[i].tunerBandFreq[2] = 8632500;
    gTunerData[i].tunerRegisterByte[0] = 0x8e; //band1
    gTunerData[i].tunerRegisterByte[1] = 0x01;
    gTunerData[i].tunerRegisterByte[2] = 0x8e; //band2
    gTunerData[i].tunerRegisterByte[3] = 0x02;
    gTunerData[i].tunerRegisterByte[4] = 0x8e; //band3
    gTunerData[i].tunerRegisterByte[5] = 0x08;
    gTunerData[i].tunerRegisterByteSearch[0] = 0xce; //band1
    gTunerData[i].tunerRegisterByteSearch[1] = 0x01;
    gTunerData[i].tunerRegisterByteSearch[2] = 0xce; //band2
    gTunerData[i].tunerRegisterByteSearch[3] = 0x02;
    gTunerData[i].tunerRegisterByteSearch[4] = 0xce; //band3
    gTunerData[i].tunerRegisterByteSearch[5] = 0x08;

    gTunerData[i].tunerBandNo = 3;
    gTunerData[i].tunerRegNoPerBand= 2;
    gTunerData[i].tunerUsedTag = _FALSE;
    gTunerData[i].tunerAGCtype = TUNER_NONE_IC_RFAGC;
    gTunerData[i].tunerType = TUNER_NORMAL;
    gTunerData[i].tunerSpecificTag = 0;
    gTunerData[i].tunerID = TUNER_KEH_FTDC3S20V04;

    i++;

#elif defined(CONFIG_TUNER_XUGUANG_TDQ_6FT_W116H)
    if ((i+1)>_Tuner_MAX_NO)
    {
        IFD_DEBUG_MSG(IFD_LOG_LEVEL, "The number of tuner selected exceed the Tuner Max Number.\n");
        while (1);
    }
    gTunerData[i].tunerI2C_ADD = 0xc0; // tuner i2c device address
    gTunerData[i].tunerMinFreq = 432500;
    gTunerData[i].tunerVHF_LowFreq = 1362500;
    gTunerData[i].tunerVHF_HighFreq = 4242500;
    gTunerData[i].tuner_MaxFreq = 8632500;
    gTunerData[i].tunerPIF = 380000;
    gTunerData[i].tunerRATIOStepSize = 625;

    gTunerData[i].tunerBandFreq[0] = 1362500;
    gTunerData[i].tunerBandFreq[1] = 4242500;
    gTunerData[i].tunerBandFreq[2] = 8632500;
    gTunerData[i].tunerRegisterByte[0] = 0x8e; //band1
    gTunerData[i].tunerRegisterByte[1] = 0x01;
    gTunerData[i].tunerRegisterByte[2] = 0x8e; //band2
    gTunerData[i].tunerRegisterByte[3] = 0x02;
    gTunerData[i].tunerRegisterByte[4] = 0x8e; //band3
    gTunerData[i].tunerRegisterByte[5] = 0x08;
    gTunerData[i].tunerRegisterByteSearch[0] = 0xce; //band1
    gTunerData[i].tunerRegisterByteSearch[1] = 0x01;
    gTunerData[i].tunerRegisterByteSearch[2] = 0xce; //band2
    gTunerData[i].tunerRegisterByteSearch[3] = 0x02;
    gTunerData[i].tunerRegisterByteSearch[4] = 0xce; //band3
    gTunerData[i].tunerRegisterByteSearch[5] = 0x08;

    gTunerData[i].tunerBandNo = 3;
    gTunerData[i].tunerRegNoPerBand= 2;
    gTunerData[i].tunerUsedTag = _FALSE;
    gTunerData[i].tunerAGCtype = TUNER_NONE_IC_RFAGC;
    gTunerData[i].tunerType = TUNER_NORMAL;
    gTunerData[i].tunerSpecificTag = 0;
    gTunerData[i].tunerID = TUNER_XUGUANG_TDQ_6FT_W116H;


    i++;

#elif defined(CONFIG_TUNER_XUGUANG_TDQ_6ID_GW115HR)
    if ((i+1)>_Tuner_MAX_NO)
    {
        IFD_DEBUG_MSG(IFD_LOG_LEVEL, "The number of tuner selected exceed the Tuner Max Number.\n");
        while (1);
    }
    gTunerData[i].tunerI2C_ADD = 0xc0; // tuner i2c device address
    gTunerData[i].tunerMinFreq = 392500;
    gTunerData[i].tunerVHF_LowFreq = 1362500;
    gTunerData[i].tunerVHF_HighFreq = 4242500;
    gTunerData[i].tuner_MaxFreq = 8632500;
    gTunerData[i].tunerPIF = 380000;
    gTunerData[i].tunerRATIOStepSize = 625;

    gTunerData[i].tunerBandFreq[0] = 1362500;
    gTunerData[i].tunerBandFreq[1] = 4242500;
    gTunerData[i].tunerBandFreq[2] = 8632500;
    gTunerData[i].tunerRegisterByte[0] = 0x8e; //band1
    gTunerData[i].tunerRegisterByte[1] = 0x01;
    gTunerData[i].tunerRegisterByte[2] = 0x8e; //band2
    gTunerData[i].tunerRegisterByte[3] = 0x02;
    gTunerData[i].tunerRegisterByte[4] = 0x8e; //band3
    gTunerData[i].tunerRegisterByte[5] = 0x08;
    gTunerData[i].tunerRegisterByteSearch[0] = 0xce; //band1
    gTunerData[i].tunerRegisterByteSearch[1] = 0x01;
    gTunerData[i].tunerRegisterByteSearch[2] = 0xce; //band2
    gTunerData[i].tunerRegisterByteSearch[3] = 0x02;
    gTunerData[i].tunerRegisterByteSearch[4] = 0xce; //band3
    gTunerData[i].tunerRegisterByteSearch[5] = 0x08;

    gTunerData[i].tunerBandNo = 3;
    gTunerData[i].tunerRegNoPerBand= 2;
    gTunerData[i].tunerUsedTag = _FALSE;
    gTunerData[i].tunerAGCtype = TUNER_NONE_IC_RFAGC;
    gTunerData[i].tunerType = TUNER_NORMAL;
    gTunerData[i].tunerSpecificTag = 0;
    gTunerData[i].tunerID = TUNER_XUGUANG_TDQ_6ID_GW115HR;

    i++;



#else
    if ((i+1)>_Tuner_MAX_NO)
    {
        //IFD_DEBUG_MSG(IFD_LOG_LEVEL, "The number of tuner selected exceed the Tuner Max Number.\n");
        while (1);
    }
    gTunerData[i].tunerI2C_ADD = 0xc0; // tuner i2c device address
    gTunerData[i].tunerMinFreq = 432500;
    gTunerData[i].tunerVHF_LowFreq = 1442500;
    gTunerData[i].tunerVHF_HighFreq = 4322500;
    gTunerData[i].tuner_MaxFreq = 8632500;
    gTunerData[i].tunerPIF = 380000;
    gTunerData[i].tunerRATIOStepSize = 625;

    gTunerData[i].tunerBandFreq[0] = 1442500;
    gTunerData[i].tunerBandFreq[1] = 4322500;
    gTunerData[i].tunerBandFreq[2] = 8632500;
    gTunerData[i].tunerRegisterByte[0] = 0x8e; //band1
    gTunerData[i].tunerRegisterByte[1] = 0x01;
    gTunerData[i].tunerRegisterByte[2] = 0x8e; //band2
    gTunerData[i].tunerRegisterByte[3] = 0x02;
    gTunerData[i].tunerRegisterByte[4] = 0x8e; //band3
    gTunerData[i].tunerRegisterByte[5] = 0x08;
    gTunerData[i].tunerRegisterByteSearch[0] = 0xce; //band1
    gTunerData[i].tunerRegisterByteSearch[1] = 0x01;
    gTunerData[i].tunerRegisterByteSearch[2] = 0xce; //band2
    gTunerData[i].tunerRegisterByteSearch[3] = 0x02;
    gTunerData[i].tunerRegisterByteSearch[4] = 0xce; //band3
    gTunerData[i].tunerRegisterByteSearch[5] = 0x08;

    gTunerData[i].tunerBandNo = 3;
    gTunerData[i].tunerRegNoPerBand= 2;
    gTunerData[i].tunerUsedTag = _FALSE;
    gTunerData[i].tunerAGCtype = TUNER_NONE_IC_RFAGC;
    gTunerData[i].tunerType = TUNER_NORMAL;
    gTunerData[i].tunerSpecificTag = 0;
    gTunerData[i].tunerID = TUNER_XUGUANG_TDQ6K_T116CW;

    i++;
#endif
}
#endif
#endif






