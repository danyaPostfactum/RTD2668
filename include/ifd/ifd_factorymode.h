/**
* @file ifd.h
*
* @author
* @email
* @date 2007/3/21
* @version 1.0
* @ingroup ifd
*
*/

#ifndef _IFD_FACTORYMODE_H_
#define _IFD_FACTORYMODE_H_

typedef struct
{
    UINT8	tunerI2C_ADD;
    UINT32	tunerMinFreq;
    UINT32	tunerVHF_LowFreq;
    UINT32	tunerVHF_HighFreq;
    UINT32	tuner_MaxFreq;
    UINT32	tunerPIF;
    UINT32	tunerRATIOStepSize;
    UINT32	tunerBandFreq[3];
    UINT8	tunerRegisterByte[6];
    UINT8	tunerRegisterByteSearch[6];
    UINT8	tunerBandNo;
    UINT8	tunerRegNoPerBand;
    UINT8	tunerUsedTag;
    UINT8	tunerAGCtype;
    UINT8	tunerType;
    UINT32	tunerSpecificTag;
    UINT8	tunerID;
} StructTunerDataType;

typedef struct
{
    UINT32	ifdMagicCode;
    UINT8	ifdAGCPGAMode;
    UINT16	AGCVtop[10];
    UINT16	AGC_PGA_MIN[10];
    UINT8	AFClockthd;
    UINT8     AFC_STAGE_MAX_COUNT;
    UINT16	SCANdelay;
    UINT16	SETCHANNELdelay;
    UINT8	CRinverse_debounce;
    UINT8	CRpgain;
    UINT8	CRigain;
    UINT8	CRlock_err_thd;
    UINT8	CRweight_speed;
    UINT8	CRphasebad_en;
    UINT8	GRPdelayNTSC;
    UINT8	GRPdelayPAL;
    UINT8	GRPdelaySECAML;
    UINT8	GRPdelaySECAMLA;
    UINT8	VideoBPFNTSC;
    UINT8	VideoBPFPAL_BG;
    UINT8	VideoBPFPAL_DK_I;
    UINT8	VideoBPFSECAM;
    UINT8	AudioNF1NTSC;
    UINT8	AudioNF2NTSC;
    UINT8	AudioNF1PAL;
    UINT8	AudioNF2PAL;
    UINT8	AudioNF1SECAM;
    UINT8	AudioNF2SECAM;
    UINT8	AudioBPF_NTSC;
    UINT8	AudioBPF_PAL;
    UINT8	AudioBPF_AUTO;
    UINT8	VideoUpSampleLPFNTSC;
    UINT8	VideoUpSampleLPFPAL;
    UINT8	VideoUpSampleLPFSECAM;
    UINT16   AGC_rf_max;
    UINT8	AGCmax_slowcharge_en;
    UINT8	AGCpeak_slowcharge_en;
    UINT16	AGCloop1_maxtargetNTSC;
    UINT16	AGCloop1_maxtargetPAL;
    UINT16	AGCloop1_maxtargetSECAM;
    UINT16	AGCloop1_difftarget;
    UINT16	AGCloop1_peaktarget;
    UINT8	AGCloop1_kpgain;
    UINT8	AGCloop1_kigain;
    UINT8	AGCloop1_kpkishift;
    UINT16	AGCloop1_dnsample;
    UINT16	AGCloop1_finednsample;
    UINT16	AGCloop2_maxtargetNTSC;
    UINT16	AGCloop2_maxtargetPAL;
    UINT16	AGCloop2_maxtargetSECAM;
    UINT16	AGCloop2_difftarget;
    UINT16	AGCloop2_peaktarget;
    UINT8	AGCloop2_kpgain;
    UINT8	AGCloop2_kigain;
    UINT8	AGCloop2_kpshift;
    UINT8	AGCloop2_kishift;
    UINT16	AGCloop2_dnsample;
    UINT16	AGCloop2_finednsample;
    //StructTunerDataType	tunerPara;
} StructIFDDataType;

extern UINT8 xdata g_curr_tunerband;

#define GET_IFD_AGCPGA_MODE() (stIFDUserData.ifdAGCPGAMode)
#define GET_IFD_AFCLOCK_THD() (stIFDUserData.AFClockthd)
#define GET_IFD_SCAN_DELAY() (stIFDUserData.SCANdelay)
#define GET_IFD_SETCHANNEL_DELAY() (stIFDUserData.SETCHANNELdelay)
#define GET_IFD_CRINVERSE_DEBOUNCE() (stIFDUserData.CRinverse_debounce)
#define GET_IFD_CR_PGAIN() (stIFDUserData.CRpgain)
#define GET_IFD_CR_IGAIN() (stIFDUserData.CRigain)
#define GET_IFD_CR_LOCK_ERR_THD() (stIFDUserData.CRlock_err_thd)
#define GET_IFD_CR_WEIGHT_SPEED() (stIFDUserData.CRweight_speed)
#define GET_IFD_CR_PHASEBAD_EN() (stIFDUserData.CRphasebad_en)
#define GET_IFD_GRPDEALY_NTSC() (stIFDUserData.GRPdelayNTSC)
#define GET_IFD_GRPDEALY_PAL() (stIFDUserData.GRPdelayPAL)
#define GET_IFD_GRPDEALY_SECAML() (stIFDUserData.GRPdelaySECAML)
#define GET_IFD_GRPDEALY_SECAMLA() (stIFDUserData.GRPdelaySECAMLA)
#define GET_IFD_VIDEOBPF_NTSC() (stIFDUserData.VideoBPFNTSC)
#define GET_IFD_VIDEOBPF_PALBG() (stIFDUserData.VideoBPFPAL_BG)
#define GET_IFD_VIDEOBPF_PALDKI() (stIFDUserData.VideoBPFPAL_DK_I)
#define GET_IFD_VIDEOBPF_SECAM() (stIFDUserData.VideoBPFSECAM)
#define GET_IFD_AUDIOBPF_NF1NTSC() (stIFDUserData.AudioNF1NTSC)
#define GET_IFD_AUDIOBPF_NF2NTSC() (stIFDUserData.AudioNF2NTSC)
#define GET_IFD_AUDIOBPF_NF1PAL() (stIFDUserData.AudioNF1PAL)
#define GET_IFD_AUDIOBPF_NF2PAL() (stIFDUserData.AudioNF2PAL)
#define GET_IFD_AUDIOBPF_NF1SECAM() (stIFDUserData.AudioNF1SECAM)
#define GET_IFD_AUDIOBPF_NF2SECAM() (stIFDUserData.AudioNF2SECAM)
#define GET_IFD_AUDIOBPF_NTSC() (stIFDUserData.AudioBPF_NTSC)
#define GET_IFD_AUDIOBPF_PAL() (stIFDUserData.AudioBPF_PAL)
#define GET_IFD_AUDIOBPF_AUTO() (stIFDUserData.AudioBPF_AUTO)
#define GET_IFD_VIDEOUPSAMPLE_LPF_NTSC() (stIFDUserData.VideoUpSampleLPFNTSC)
#define GET_IFD_VIDEOUPSAMPLE_LPF_PAL() (stIFDUserData.VideoUpSampleLPFPAL)
#define GET_IFD_VIDEOUPSAMPLE_LPF_SECAM() (stIFDUserData.VideoUpSampleLPFSECAM)
#define GET_IFD_AGC_RF_MAX() (stIFDUserData.AGC_rf_max)
#define GET_IFD_AGCMAX_SLOWCHARGE_EN() (stIFDUserData.AGCmax_slowcharge_en)
#define GET_IFD_AGCPEAK_SLOWCHARGE_EN() (stIFDUserData.AGCpeak_slowcharge_en)
#define GET_IFD_AGC_VTOP() (stIFDUserData.AGCVtop[g_curr_tunerband]) //CTunerCheckBand(g_curr_freq) is wrong in pacific.
#define GET_IFD_AGC_PGA_MINIMUM() (stIFDUserData.AGC_PGA_MIN[g_curr_tunerband]) //CTunerCheckBand(g_curr_freq) is wrong in pacific.
#define GET_IFD_AGCLOOP1_MAXTARGET_NTSC() (stIFDUserData.AGCloop1_maxtargetNTSC)
#define GET_IFD_AGCLOOP1_MAXTARGET_PAL() (stIFDUserData.AGCloop1_maxtargetPAL)
#define GET_IFD_AGCLOOP1_MAXTARGET_SECAM() (stIFDUserData.AGCloop1_maxtargetSECAM)
#define GET_IFD_AGCLOOP1_DIFFTARGET() (stIFDUserData.AGCloop1_difftarget)
#define GET_IFD_AGCLOOP1_PEAKTARGET() (stIFDUserData.AGCloop1_peaktarget)
#define GET_IFD_AGCLOOP1_KPGAIN() (stIFDUserData.AGCloop1_kpgain)
#define GET_IFD_AGCLOOP1_KIGAIN() (stIFDUserData.AGCloop1_kigain)
#define GET_IFD_AGCLOOP1_KPKISHIFT() (stIFDUserData.AGCloop1_kpkishift)
#define GET_IFD_AGCLOOP1_DNSAMPLE() (stIFDUserData.AGCloop1_dnsample)
#define GET_IFD_AGCLOOP1_FINEDNSAMPLE() (stIFDUserData.AGCloop1_finednsample)
#define GET_IFD_AGCLOOP2_MAXTARGET_NTSC() (stIFDUserData.AGCloop2_maxtargetNTSC)
#define GET_IFD_AGCLOOP2_MAXTARGET_PAL() (stIFDUserData.AGCloop2_maxtargetPAL)
#define GET_IFD_AGCLOOP2_MAXTARGET_SECAM() (stIFDUserData.AGCloop2_maxtargetSECAM)
#define GET_IFD_AGCLOOP2_DIFFTARGET() (stIFDUserData.AGCloop2_difftarget)
#define GET_IFD_AGCLOOP2_PEAKTARGET() (stIFDUserData.AGCloop2_peaktarget)
#define GET_IFD_AGCLOOP2_KPGAIN() (stIFDUserData.AGCloop2_kpgain)
#define GET_IFD_AGCLOOP2_KIGAIN() (stIFDUserData.AGCloop2_kigain)
#define GET_IFD_AGCLOOP2_KPSHIFT() (stIFDUserData.AGCloop2_kpshift)
#define GET_IFD_AGCLOOP2_KISHIFT() (stIFDUserData.AGCloop2_kishift)
#define GET_IFD_AGCLOOP2_DNSAMPLE() (stIFDUserData.AGCloop2_dnsample)
#define GET_IFD_AGCLOOP2_FINEDNSAMPLE() (stIFDUserData.AGCloop2_finednsample)
#define GET_IFD_AFC_STAGE_MAX_COUNT() (stIFDUserData.AFC_STAGE_MAX_COUNT)

#if 0
//=================================================================
//IFD Factory Mode
//=================================================================
//////////////////////////////////////////////////////////////////////////////////////
//Page 1
/////////////////////////////////////////////////////////////////////////////////////
//=================================================================
/**
 * drvif_ifd_factorymode_set_tuner_freq
 *	Set tuner freuency.
 *     <Uint> 10kHz
 * @param <freq>
 * @return { void }
 */
void drvif_ifd_factorymode_set_tuner_freq(UINT32 freq);
//=================================================================
/**
 * drvif_ifd_factorymode_ifdagc_setting
 *	Set IFD AGC setting
 *
 * @param {enable}<1:IFAGC/0:GPIO>
 * @return  {void}
 */
void drvif_ifd_factorymode_ifdagc_setting(bool enable);
//=================================================================
/**
 * drvif_ifd_factorymode_ifdagc_setting_get_info
 *	Get IFD AGC setting
 *
 * @param   {void}
 * @return   <_TURE:IFAGC/_FLASE:GPIO>
 */
UINT8 drvif_ifd_factorymode_ifdagc_setting_get_info(void);
//=================================================================
/**
 * drvif_ifd_factorymode_rfagc_setting
 *	Set RF AGC setting
 *
 * @param {enable}<1:IFAGC/0:GPIO>
 * @return  {void}
 */
void drvif_ifd_factorymode_rfagc_setting(bool enable);
//=================================================================
/**
 * drvif_ifd_factorymode_rfagc_setting_get_info
 *	Get RF AGC setting
 *
 * @param   {void}
 * @return   <_TURE:IFAGC/_FLASE:GPIO>
 */
UINT8 drvif_ifd_factorymode_rfagc_setting_get_info(void);
//=================================================================
/**
 * drvif_ifd_factorymode_pll_lvds7x_turnoff
 *	Turn off LVDS 7x PLL
 *
 * @param  {void}
 * @return  {void}
 */
void drvif_ifd_factorymode_pll_lvds7x_turnoff(void);
//=================================================================
/**
 * drvif_ifd_factorymode_pll_bus_turnoff
 *	Turn off BUS PLL
 *
 * @param  {void}
 * @return  {void}
 */
void drvif_ifd_factorymode_pll_bus_turnoff(void);
//=================================================================
/**
 * drvif_ifd_factorymode_pll_dpll_turnoff
 *	Turn off DPLL
 *
 * @param  {void}
 * @return  {void}
 */
void drvif_ifd_factorymode_pll_dpll_turnoff(void);
//=================================================================
/**
 * drvif_ifd_factorymode_pll_audiopll_turnoff
 *	Turn off Audio PLL
 *
 * @param  {void}
 * @return  {void}
 */
void drvif_ifd_factorymode_pll_audiopll_turnoff(void);
#endif

#endif


