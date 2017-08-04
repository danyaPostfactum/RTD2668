/*=============================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2005
  * All rights reserved.
  * ============================================ */

/*================= File Description ================= */
/**
 * @file
 * The file is a declaration and definition header file
 *
 * @author 	$Author:  $
 * @date 	$Date:  $
 * @version 	$Revision:  $
 * @ingroup
 */

/**
* @addtogroup
* @
*/

/*==================== Definitions ================= */


#ifndef  _FLOW_AUDIO_H
#define  _FLOW_AUDIO_H

#include "sparrow.h"
#include "source.h"
#include "audio\audio_api.h"
#include "autoconf.h"

extern UINT8 amp_mute_delay;
extern UINT8 amp_unmute_delay;
extern bit bearphone;
typedef enum _MUTE_FLAG
{
    SCALER = 0x01,
    BLOCK = 0x02,
    OSD = 0x04,
    VCHIP = 0x08,
    HEADPHONE = 0x10,
} MUTE_FLAG;

/*==================== Functions ================== */
/**
 * flow_audio_selectSource
 * This API is used to setting audio setting Spatial
 *
 * @param [input] :  <mode>		{ 0:Disable, 1:WOW, 2:BBE, 3:Realtek Surround}
  * @param [input] :  <mode>		{ 0:Disable, 1:WOW, 2:BBE, 3:Realtek Surround}
   @param [input] :  <plib_info_bbe>		{ the parameters of BBE method}
 * @return None
 *
 */
void flow_audio_selectSource(StructSourceInput * ptSourceInput);
/**
 * flow_audio_setsurround
 * This API is used to setting audio setting Spatial
 *
 * @param [input] :  <mode>		{ 0:Disable, 1:WOW, 2:BBE, 3:Realtek Surround}
  * @param [input] :  <mode>		{ 0:Disable, 1:WOW, 2:BBE, 3:Realtek Surround}
   @param [input] :  <plib_info_bbe>		{ the parameters of BBE method}
 * @return None
 *
 */
void flow_audio_Setsurround(UINT8 mode, void* plib_info_bbe);

/**
 * flow_audio_ctrlspatial
 * This API is used to setting audio setting Spatial
 *
 * @param [input] :  <mode>		{ 0:original, 1:stereo, 2:1m effect, 3:2m effect, 4:4m effect}
 * @return None
 *
 */
void flow_audio_Ctrlspatial(UINT8 mode);//yyfor compile12.8
/**
 * flow_audio_setsoundmode
* This API is used to process TV audio mode control
 *
 * @param <mode>	{ _SOUND_DK,_SOUND_I, _SOUND_BG, _SOUND_MN}
 * @param [output] :
 * @return None
 *
 */
void flow_audio_Setsoundmode(UINT8 mode);

/**
 * fw_audio_setmute
 * This API is used to initial config audio setting(Spatial,balance,volume,avl,eq,path)
 *
 * @param [input] :  <enable>		{ 1:MUte,0:UNMUTE}
 * @param <flag>	{ SCALER | BLOCK| OSD }
 * @param <ch_idx>	{ CH_L | CH_R | CH_LS | CH_RS | CH_C | CH_SW }
 * @param [output] :
 * @return None
 *
 */
void fw_audio_Startconfig(void);

/**
 * fwif_audio_setmute
 * This API is used to process system mute/unmute control
 *
 * @param [input] :  <enable>		{ 1:MUte,0:UNMUTE}
 * @param <flag>	{ SCALER | BLOCK| OSD | VCHIP }
 * @param <ch_idx>	{ CH_L | CH_R | CH_LS | CH_RS | CH_C | CH_SW }
 * @param [output] :
 * @return None
 *
 */
void flow_audio_Setmute(bit enable, MUTE_FLAG flag, ADC_CH_IDX ch_idx);

/**
 * flow_audio_setsoundlanguage
 * This API is used to process TV audio loanuage mode
 *
 * @param <mode>	{ MONO, STEREO, SAP}
 * @param [output] :
 * @return None
 *
 */
void flow_audio_Setsoundlanguage(UINT8 mode);

#ifdef CONFIG_ENABLE_HDMI
bit flow_audio_Interrupt_Proc(void);
#endif
void flow_audio_Handler(void);


/**
 * @
 */

#endif // #ifndef  _FLOW_AUDIO_H
