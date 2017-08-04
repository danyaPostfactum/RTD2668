/*===========================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2009
 * All rights reserved.
 * ========================================================== */

/*==================== File Description ===================== */
/**
 * @file
 * 	This file is customer struct data for ap.
 *
 * @author 	$Author: cf.tsai $
 * @date 	$Date: 2009-03-16 $
 * @version $Revision: 10 $
 */

//----------------------------------------------------------------------------------------------------
// ID Code      : Struct.c No.0000
// Update Note  :
//
//----------------------------------------------------------------------------------------------------

#define _CNI_TABLE_C

#include "autoconf.h"
#include "rtd_types.h"
#include "rosprintf.h"
#include "vbi/ppr.h"
#include "ui_struct.h"
#include "osdfont.h"
#include "cni_table.h"
#include "rt_storage.h"
#include "storage_system.h"

#define _ENABLE_CNI_TABLE		1

// cftsai 20061124 added, Network name
// ATS    	F1 NI	F2 CNI	Name
// *****	*****	*****	*****
#if defined(CONFIG_CHANNEL_ATS)
CNI_NETWORK_TABLE code Ats_Ranking_List_Austria[] =
{
#if _ENABLE_CNI_TABLE
// VPS CNI code
    {   1, 0x0000, 0x0ac1, {'O', 'R', 'F', '-', '1'}},//ORF-FS1
    {   2, 0x0000, 0x0ac2, {'O', 'R', 'F', '-', '2'}},//ORF-FS2
    {   3, 0x0000, 0x0ac3, {'O', 'R', 'F', '-', '3'}},//ORF FS3
    {   0, 0x0000, 0x0ac4, {'S', 'P', 'O', 'R', 'T'}},//ORF-Sport+
    {   0, 0x0000, 0x0ac7, {' ', ' ', 'T', 'W', '1'}},//TW 1
    {   0, 0x0000, 0x0ac8, {'N', 'I', 'C', 'K', ' '}},//Nick - Viva
    {   0, 0x0000, 0x0ac9, {' ', ' ', 'M', 'T', 'V'}},//MTV Austria
    {   0, 0x0000, 0x0aca, {'A', 'T', 'V', '+', ' '}},//ATV+
    {   2, 0x0000, 0x0acb, {'O', 'R', 'F', '-', '2'}},//ORF FS2/Burgenland
    {   2, 0x0000, 0x0acc, {'O', 'R', 'F', '-', '2'}},//ORF FS2/Karnten
    {   2, 0x0000, 0x0acd, {'O', 'R', 'F', '-', '2'}},//ORF FS2/Niederosterreich
    {   2, 0x0000, 0x0ace, {'O', 'R', 'F', '-', '2'}},//ORF FS2/Oberosterreich
    {   2, 0x0000, 0x0acf, {'O', 'R', 'F', '-', '2'}},//ORF FS2/Salzburg
    {   2, 0x0000, 0x0ad0, {'O', 'R', 'F', '-', '2'}},//ORF FS2/Steiermark
    {   2, 0x0000, 0x0ad1, {'O', 'R', 'F', '-', '2'}},//ORF FS2/Tirol
    {   2, 0x0000, 0x0ad2, {'O', 'R', 'F', '-', '2'}},//ORF FS2/Vorarlberg
    {   2, 0x0000, 0x0ad3, {'O', 'R', 'F', '-', '2'}},//ORF FS2/Wein
// CNI (Teletext based Systems)
    {    1, 0x4301, 0x0000, {'O', 'R', 'F', '-', '1'}},//ORF-1
//ASIG change{    2, 0x4302, 0x0000, {'O', 'R', 'F', '-', '2'}},//ORF-2
    {    2, 0x4302, 0xfac2, {'O', 'R', 'F', '-', '2'}},//ORF-2,ASIG add
    {    0, 0x4303, 0x0000, {' ', ' ', 'O', 'R', 'F'}},//ORF future use
    {    0, 0x4304, 0x0000, {' ', ' ', 'O', 'R', 'F'}},//ORF future use
    {    0, 0x4305, 0x0000, {' ', ' ', 'O', 'R', 'F'}},//ORF future use
    {    0, 0x4306, 0x0000, {' ', ' ', 'O', 'R', 'F'}},//ORF future use
    {    0, 0x4307, 0x0000, {' ', ' ', 'O', 'R', 'F'}},//ORF future use
    {    0, 0x4308, 0x0000, {' ', ' ', 'O', 'R', 'F'}},//ORF future use
    {    0, 0x4309, 0x0000, {' ', ' ', 'O', 'R', 'F'}},//ORF future use
    {    0, 0x430a, 0x0000, {' ', ' ', 'O', 'R', 'F'}},//ORF future use
    {    0, 0x430b, 0x0000, {' ', ' ', 'O', 'R', 'F'}},//ORF future use
    {    0, 0x430c, 0x0000, {' ', ' ', 'A', 'T', 'V'}},//ATV
    {    0, 0x8888, 0x0000, {'O', 'K', 'T', 'O', ' '}},//OKTO
#endif
    {0xff, 0xffff, 0xffff, {'-', '-', '-', '-', '-'}}, //END_OF_TABLE
};
// 31 itmes


CNI_NETWORK_TABLE code Ats_Ranking_List_Belgium[] =
{
#if _ENABLE_CNI_TABLE
    {   0, 0x320c, 0x0000, {' ', ' ', 'A', 'B', '3'}},//AB3
    {   0, 0x320d, 0x0000, {' ', 'A', 'B', '4', 'E'}},//AB4e
    {   1, 0x3201, 0x1601, {' ', ' ', 'T', 'V', '1'}},//BRTN TV1
    {   2, 0x3202, 0x1602, {' ', ' ', 'T', 'V', '2'}},//
    {   5, 0x3203, 0x0000, {'R', 'T', 'B', 'F', '1'}},//
    {   6, 0x3204, 0x0000, {'R', 'T', 'B', 'F', '2'}},//
    {   7, 0x3205, 0x1605, {' ', ' ', 'V', 'T', 'M'}},//
    {   3, 0x3206, 0x1606, {' ', ' ', 'K', 'A', '2'}},//Ka2
    {   0, 0x3207, 0x0000, {' ', 'R', 'T', 'B', 'F'}},//RTBF Sat
    {   0, 0x3208, 0x0000, {' ', 'R', 'T', 'B', 'F'}},//RTBF future use
    {   0, 0x3209, 0x0000, {'R', 'T', 'L', ' ', ' '}},//RTL-TVI
    {   0, 0x320a, 0x0000, {'C', 'L', 'U', 'B', ' '}},//CLUB-RTL
    {   4, 0x0404, 0x1604, {' ', ' ', 'V', 'T', '4'}},//
    {   0, 0x320f, 0x0000, {'J', 'I', 'M', ' ', ' '}},//JIM.tv
    {   0, 0x3225, 0x0000, {'P', 'L', 'U', 'G', ' '}},//PLUG TV
    {   0, 0x3210, 0x0000, {'R', 'T', 'V', '-', 'K'}},//RTV-Kempen
    {   0, 0x3211, 0x0000, {'R', 'T', 'V', '-', 'M'}},//RTV-Mechelen
    {   0, 0x3212, 0x0000, {' ', ' ', 'M', 'C', 'M'}},//MCM Belgium
    {   0, 0x3213, 0x0000, {'V', 'I', 'T', 'A', '.'}},//Vitaya
    {   0, 0x3214, 0x0000, {' ', ' ', 'W', 'T', 'V'}},//WTV
    {   0, 0x3215, 0x0000, {'F', 'O', 'C', 'U', 'S'}},//FocusTV
    {   0, 0x3216, 0x0000, {'1', ' ', 'A', 'N', 'A'}},//Be 1 ana
    {   0, 0x3217, 0x0000, {'1', ' ', 'N', 'U', 'M'}},//Be 1 num
    {   0, 0x3218, 0x0000, {'C', 'I', 'N', 'E', '1'}},//Be Cine 1
    {   0, 0x3219, 0x0000, {'S', 'P', 'O', '.', '1'}},//Be Sport 1
    {   0, 0x32A7, 0x0000, {'1', '+', '1', 'H', ' '}},//Be 1 + 1h
    {   0, 0x32a8, 0x0000, {'C', 'I', 'N', 'E', '2'}},//Be Cine 2
    {   0, 0x32a9, 0x0000, {'S', 'P', 'O', '.', '2'}},//Be Sport 2
    {   0, 0x321a, 0x0000, {'P', 'R', '.', 'S', '1'}},//PRIME Sport 1
    {   0, 0x321b, 0x0000, {'P', 'R', '.', 'S', '2'}},//PRIME sport 2
    {   0, 0x321c, 0x0000, {'P', 'R', '.', 'A', 'C'}},//PRIME Action
    {   0, 0x321d, 0x0000, {' ', ' ', 'P', 'R', '1'}},//PRIME One
    {   0, 0x3221, 0x0000, {'L', 'I', 'M', 'B', '.'}},//TV Limburg
    {   0, 0x3222, 0x0000, {' ', ' ', 'K', 'A', '3'}},//Kanaal 3
    {   0, 0X320E, 0X0000, {' ', 'R', 'I', 'N', 'G'}},//Ring TV
    {   0, 0x321E, 0x0000, {'B', 'R', 'U', 'S', '.'}},//TV Brussel
    {   0, 0x321f, 0x0000, {'A', 'V', 'S', 'E', ' '}},//AVSe
    {   0, 0x3223, 0x0000, {' ', ' ', 'A', 'T', 'V'}},//ATV
    {   0, 0x3224, 0x0000, {' ', ' ', 'R', 'O', 'B'}},//ROB TV
    {   0, 0x3228, 0x0000, {'L', 'I', 'F', 'E', ' '}},//Life!tv
    {   0, 0x3229, 0x0000, {' ', ' ', 'M', 'T', 'V'}},//NTV Belgium(French)
    {   0, 0x3230, 0x0000, {'T', '.', 'B', 'R', 'U'}},//Tele Bruxelles
    {   0, 0x3231, 0x0000, {'T', '.', 'S', 'A', 'M'}},//Telesambre
    {   0, 0x3232, 0x0000, {'T', 'V', 'C', 'O', 'M'}},//TV Com
    {   0, 0x3233, 0x0000, {'C', 'Z', 'O', 'O', 'M'}},//Canal Zoom
    {   0, 0x3234, 0x0000, {'V', 'I', 'D', 'E', 'O'}},//Videoscope
    {   0, 0x3235, 0x0000, {'C', 'a', 'n', ' ', 'C'}},//Canal C
    {   0, 0x3236, 0x0000, {' ', 'T', '.', 'M', 'B'}},//Tele MB
    {   0, 0x3237, 0x0000, {'A', 'N', 'T', '.', 'C'}},//Antenne Centre
    {   0, 0x3238, 0x0000, {'T', 'E', 'L', '.', 'V'}},//Televesdre
//ASIG change{   0, 0x3239, 0x0000, {' ', ' ', 'R', 'T', 'L'}},//RTC Tele Liege
    {   0, 0x3239, 0xfd79, {' ', ' ', 'R', 'T', 'L'}},//RTC Tele Liege,ASIG add.
    {   0, 0x3240, 0x0000, {'N', 'O', ' ', 'T', 'E'}},//No tele
    {   0, 0x3241, 0x0000, {' ', ' ', 'L', 'U', 'X'}},//TV Lux
    {   0, 0x325a, 0x0000, {'K', 'A', ' ', 'Z', ' '}},//Kanaal Z - NL
    {   0, 0x325b, 0x0000, {'C', 'A', 'N', ' ', 'Z'}},//CANAL Z - FR
    {   0, 0x326a, 0x0000, {'C', 'N', '-', 'N', 'L'}},//CARTOON Network - NL
    {   0, 0x326b, 0x0000, {'C', 'N', '-', 'F', 'R'}},//CARTOON Network -FR
    {   0, 0x327a, 0x0000, {'L', 'C', '-', 'N', 'L'}},//LIBERTY CHANNEL - NL
    {   0, 0x327b, 0x0000, {'L', 'C', '-', 'F', 'R'}},//LIBERTY CHANNEL - FR
    {   0, 0x328A, 0x0000, {'T', 'C', 'M', 'N', 'L'}},//TCM - NL
    {   0, 0x328B, 0X0000, {'T', 'C', 'M', 'F', 'R'}},//TCM - FR
    {   0, 0x3298, 0x0000, {'M', 'O', 'Z', 'A', '.'}},//Mozaiek/Mosaique
    {   0, 0x3299, 0x0000, {'C', '.', 'I', 'N', 'F'}},//Info Kanaal/Canal Info
    {   0, 0x3226, 0x0000, {'S', 'P', 'O', 'R', '.'}},//Sporza
    {   0, 0x3227, 0x0000, {'V', 'I', 'J', 'F', ' '}},//VIJF tv
    {   0, 0x3220, 0x0000, {'S', ' ', 'T', 'E', 'L'}},//S televisie
#endif
    {0xff, 0xffff, 0xffff, {'-', '-', '-', '-', '-'}}, //END_OF_TABLE
};
// 67 items

CNI_NETWORK_TABLE code Ats_Ranking_List_Croatia[] =
{
#if _ENABLE_CNI_TABLE
    {   1, 0x0385, 0x0000, {'H', 'R', 'T', '1', ' '}},//HRT1
    {   2, 0x0387, 0x0000, {'H', 'R', 'T', '2', ' '}},//HRT2
    {   0, 0x0388, 0x0000, {'H', 'R', 'T', '+', ' '}},//HRT PLUS
    {   0, 0x0386, 0x0000, {'N', 'O', 'V', 'A', ' '}},//Nova TV
    {   0, 0x0400, 0x0000, {' ', ' ', 'R', 'T', 'L'}},//RTL Televizija
#endif
    {0xff, 0xffff, 0xffff, {'-', '-', '-', '-', '-'}}, //END_OF_TABLE
};
// 6 items


// Czech (PDC)
CNI_NETWORK_TABLE code Ats_Ranking_List_Czech[] =
{
#if _ENABLE_CNI_TABLE
    {   1, 0x4201, 0x32c1, {' ', ' ', 'C', 'T', '1'}},//CT 1
    {   2, 0x4202, 0x32c2, {' ', ' ', 'C', 'T', '2'}},//CT 2
    {   1, 0x4231, 0x32f1, {' ', ' ', 'C', 'T', '1'}},//CT1 Regional
    {   1, 0x4211, 0x32d1, {' ', ' ', 'C', 'T', '1'}},//CT1 Regional,Brno
    {   1, 0x4221, 0x32e1, {' ', ' ', 'C', 'T', '1'}},//CT1 Regional,Ostravia
    {   2, 0x4232, 0x32f2, {' ', ' ', 'C', 'T', '2'}},//CT2 Regional
    {   2, 0x4212, 0x32d2, {' ', ' ', 'C', 'T', '2'}},//CT2 Regional,Brno
    {   2, 0x4222, 0x32e2, {' ', ' ', 'C', 'T', '2'}},//CT2 Regional,Ostravia
    {   3, 0x4203, 0x32c3, {' ', 'N', 'O', 'V', 'A'}},//NOVA TV
//ASIG change{   0, 0x4204, 0x32c4, {'P', 'R', 'I', 'M', 'A'}},//Prima TV
    {   0, 0x4299, 0x0200, {'P', 'R', 'I', 'M', 'A'}},//Prima TV,ASIG add
    {   0, 0x4205, 0x0000, {'P', 'R', 'A', 'H', 'A'}},//TV Praha
    {   0, 0x4206, 0x0000, {'T', 'V', ' ', 'H', 'K'}},//TV HK
    {   0, 0x4207, 0x0000, {'P', 'A', 'R', 'D', '.'}},//TV Pardubice
    {   0, 0x4208, 0x0000, {'B', 'R', 'N', 'O', ' '}},//TV Brno
    {   0, 0x420a, 0x32ca, {'C', 'T', '2', '4', ' '}},//CT24
    {   4, 0x420b, 0x32cb, {' ', ' ', 'C', 'T', '4'}},//CT4 SPORT
    {   0, 0x420c, 0x32cc, {' ', ' ', ' ', 'C', 'T'}},//CT future use
    {   0, 0x420d, 0x32cd, {' ', ' ', ' ', 'C', 'T'}},//CT future use
    {   0, 0x420e, 0x32ce, {' ', ' ', ' ', 'C', 'T'}},//CT future use
    {   0, 0x420f, 0x32cf, {' ', ' ', ' ', 'C', 'T'}},//CT future use
#endif
    {0xff, 0xffff, 0xffff, {'-', '-', '-', '-', '-'}}, //END_OF_TABLE
};
// 21 items


CNI_NETWORK_TABLE code Ats_Ranking_List_Denmark[] =
{
#if _ENABLE_CNI_TABLE
    {   0, 0x4504, 0x0000, {'D', 'I', 'S', 'C', '.'}},//Discovery Denmark
    {   1, 0x7392, 0x2901, {' ', ' ', 'D', 'R', '1'}},//
    {   2, 0x49cf, 0x2903, {' ', ' ', 'D', 'R', '2'}},//
    {   3, 0x4502, 0x2902, {' ', ' ', 'T', 'V', '2'}},//
    {   3, 0x4503, 0x2904, {' ', ' ', 'T', 'V', '2'}},//TV 2 Zulu
    {   3, 0x4505, 0x2905, {' ', ' ', 'T', 'V', '2'}},//TV 2 Charlie
    {   0, 0x4506, 0x2906, {'T', 'V', ' ', 'D', ' '}},//TV Danmark
    {   0, 0x4507, 0x2907, {' ', ' ', 'K', 'A', '5'}},//Kanal 5
    {   0, 0x4508, 0x2908, {'T', 'V', '2', ' ', 'F'}},//TV 2 Film
    {   0, 0x4509, 0x2909, {'T', 'V', '2', ' ', 'N'}},//TV 2 News
#endif
    {0xff, 0xffff, 0xffff, {'-', '-', '-', '-', '-'}}, //END_OF_TABLE
};
// 11 items

CNI_NETWORK_TABLE code Ats_Ranking_List_Finland[] =
{
#if _ENABLE_CNI_TABLE
    {   3, 0x358f, 0x260f, {' ', 'O', 'W', 'L', '3'}},//
    {   0, 0x3583, 0x2603, {' ', ' ', 'Y', 'L', 'E'}},//YLE future use
    {   0, 0x3584, 0x2604, {' ', ' ', 'Y', 'L', 'E'}},//YLE future use
    {   0, 0x3585, 0x2605, {' ', ' ', 'Y', 'L', 'E'}},//YLE future use
    {   0, 0x3586, 0x2606, {' ', ' ', 'Y', 'L', 'E'}},//YLE future use
    {   0, 0x3587, 0x2607, {' ', ' ', 'Y', 'L', 'E'}},//YLE future use
    {   0, 0x3588, 0x2608, {' ', ' ', 'Y', 'L', 'E'}},//YLE future use
    {   0, 0x3589, 0x2609, {' ', ' ', 'Y', 'L', 'E'}},//YLE future use
    {   0, 0x358a, 0x260a, {' ', ' ', 'Y', 'L', 'E'}},//YLE future use
    {   0, 0x358b, 0x260b, {' ', ' ', 'Y', 'L', 'E'}},//YLE future use
    {   0, 0x358c, 0x260c, {' ', ' ', 'Y', 'L', 'E'}},//YLE future use
    {   0, 0x358d, 0x260d, {' ', ' ', 'Y', 'L', 'E'}},//YLE future use
    {   0, 0x358e, 0x260e, {' ', ' ', 'Y', 'L', 'E'}},//YLE future use
    {   1, 0x3581, 0x2601, {' ', 'Y', 'L', 'E', '1'}},//
    {   2, 0x3582, 0x2602, {' ', 'Y', 'L', 'E', '2'}},//
#endif
    {0xff, 0xffff, 0xffff, {'-', '-', '-', '-', '-'}}, //END_OF_TABLE
};
// 16 items

// France (PDC)
CNI_NETWORK_TABLE code Ats_Ranking_List_France[] =
{
    {  18, 0x33c1, 0x2fc1, {' ', ' ', 'A', 'B', '1'}},//
    {  26, 0x3320, 0x2f20, {' ', 'A', 'Q', 'U', 'I'}},//Aqui TV
    {   5, 0x330a, 0x2f0a, {' ', 'A', 'R', 'T', 'E'}},//Arte/La Cinquieme
    {  24, 0x33c2, 0x2fc2, {'C', 'A', 'N', ' ', 'J'}},//Canal J
    {  13, 0x33c3, 0x2fc3, {'J', 'I', 'M', 'M', 'Y'}},//Canal Jimmy
    {   4, 0x33f4, 0x2f04, {' ', 'C', 'A', 'N', '+'}},//Canal+
    {   9, 0xfe01, 0x2fe1, {' ', 'N', 'E', 'W', 'S'}},//Euronews
//ASIG change {   7, 0xf101, 0x2fe2, {'S', 'P', 'O', 'R', 'T'}},//Eurosport
    {   7, 0xf101, 0x0000, {'S', 'P', 'O', 'R', 'T'}},//Eurosport,ASIG add
    {   0, 0xf102, 0x2fe3, {'S', 'P', 'O', ' ', '2'}},//Eurosport2
    {   0, 0xf103, 0x2fe4, {'S', 'P', 'O', ' ', 'N'}},//Eurosportnews
    {   2, 0x33f2, 0x2f02, {' ', 'F', 'T', 'V', '2'}},//France 2
    {   3, 0x33f3, 0x2f03, {' ', 'F', 'T', 'V', '3'}},//France 3
    {   5, 0x33f5, 0x2f05, {' ', 'F', 'T', 'V', '5'}},//France 5,ASIG add
    {   5, 0x3308, 0x2f05, {' ', 'A', 'R', 'T', 'E'}},//ARTE,ASIG add
    {  11, 0x33c5, 0x2fc5, {' ', ' ', 'L', 'C', 'M'}},//La Chaine Meteo
    {  10, 0x33c4, 0x2fc4, {' ', ' ', 'L', 'C', 'I'}},//
    {   6, 0x33f6, 0x2f06, {' ', ' ', ' ', 'M', '6'}},//
    {  14, 0x33c6, 0x2fc6, {' ', ' ', 'M', 'C', 'M'}},//
    {  15, 0x33c8, 0x2fc8, {'P', 'R', 'E', 'M', '.'}},//Paris Premiere
    {  12, 0x33c9, 0x2fc9, {'P', 'L', 'A', 'N', '.'}},//Planete
    {  19, 0x3311, 0x2f11, {' ', 'R', 'F', 'O', '1'}},//
    {  20, 0x3312, 0x2f12, {' ', 'R', 'F', 'O', '2'}},//
    {   0, 0x33b2, 0x0000, {'S', 'a', 'i', 'l', '.'}},//Sailing Channel
    {  23, 0x33ca, 0x2fca, {' ', 'C', 'L', 'U', 'B'}},//Serie Club
    {  22, 0x33cb, 0x2fcb, {'T', 'E', 'L', 'E', 'T'}},//Teletoon
    {  21, 0x33cc, 0x2fcc, {' ', 'T', 'E', 'V', 'A'}},//
    {   1, 0x33f1, 0x2f01, {' ', ' ', 'T', 'F', '1'}},//
    {  16, 0x3321, 0x2f21, {' ', ' ', 'T', 'L', 'M'}},//
    {  25, 0x3322, 0x2f22, {' ', ' ', 'T', 'L', 'T'}},//
    {  17, 0x33c7, 0x2fc7, {' ', ' ', 'T', 'M', 'C'}},//TMC Monte-Carlo
//ASIG change{   8, 0xf500, 0x2fe5, {' ', ' ', 'T', 'V', '5'}},//
    {   8, 0xf500, 0x59a6, {' ', ' ', 'T', 'V', '5'}},// TV5ASIG add
// {   0, 0x0d76, 0x0000, {'D', 'A', 'S', ' ', 'V'}},// DAS ASIG add
    {0xff, 0xffff, 0xffff, {'-', '-', '-', '-', '-'}}, //END_OF_TABLE
};
// 30 items


CNI_NETWORK_TABLE code Ats_Ranking_List_Germany[] =
{
#if _ENABLE_CNI_TABLE
// {   1, 0x0000, 0x0dc1, {' ', ' ', 'A', 'R', 'D'}},//ARD
// {   2, 0x0000, 0x0dc2, {' ', ' ', 'Z', 'D', 'F'}},//ZDF
    {   1, 0x4901, 0x0dc1, {' ', ' ', 'A', 'R', 'D'}},//ARD@ ASIG
    {   2, 0x4902, 0x0dc3, {' ', ' ', 'Z', 'D', 'F'}},//ZDF@ ASIG
    {   0, 0x0000, 0x0dc3, {'A', 'R', '/', 'Z', 'D'}},//ARD/ZDF
    {   0, 0x0000, 0x0dc4, {'A', 'R', 'D', 'x', ' '}},//ARD-TV
    {   0, 0x0000, 0x0dc5, {'A', 'R', 'D', 'x', 'x'}},//ARD-TV-Sternpunkt
    {  13, 0x0000, 0x0dc7, {' ', '3', 'S', 'A', 'T'}},//Satelliten-Programm
    {   0, 0x0000, 0x0dc8, {'P', 'H', 'O', 'E', 'N'}},//Phoenix ARD/ZDF
    {   0, 0x0000, 0x0dc9, {' ', 'K', 'I', 'K', 'A'}},//Kinderkanal ARD/ZDF
    {   0, 0x0000, 0x0dca, {' ', 'B', 'R', '-', '1'}},//BR-1
//ASIG change{  15, 0x0000, 0x0dcb, {' ', 'B', 'R', '-', '3'}},//BR-3
    {  15, 0x49cb, 0x0dcb, {' ', 'B', 'R', '-', '3'}},//BR-3,ASIG add
    {  15, 0x0000, 0x0dcc, {' ', 'B', 'R', '-', '3'}},//BR-3/Sud
    {  15, 0x0000, 0x0dcd, {' ', 'B', 'R', '-', '3'}},//BR-3/Nord
    {   0, 0x0000, 0x0dce, {' ', 'H', 'R', '-', '1'}},//HR-1/Regionalprogramm
    {  16, 0x0000, 0x0dcf, {'H', 'E', 'S', ' ', '3'}},//Hessen 3
    {   0, 0x0000, 0x0dd0, {'N', 'D', 'R', '-', '1'}},//NDR-1/Landesprogramm
    {   0, 0x0000, 0x0dd1, {'N', 'D', 'R', '-', '1'}},//NDR-1/Hamburg
    {   0, 0x0000, 0x0dd2, {'N', 'D', 'R', '-', '1'}},//NDR-1/Niedersachsen
    {   0, 0x0000, 0x0dd3, {'N', 'D', 'R', '-', '1'}},//NDR-1/Schleswig-Hlstein
//ASIG Change {   3, 0x0000, 0x0dd4, {' ', ' ', ' ', 'N', '3'}},//Nord-3
    {   0, 0x0000, 0x0dd4, {'N', 'D', 'R', '-', '3'}},//NDR-3,ASIG add
    {   0, 0x0000, 0x0dd5, {'N', 'D', 'R', '-', '3'}},//NDR-3/dreilanderweit
    {   0, 0x0000, 0x0dd6, {'N', 'D', 'R', '-', '3'}},//NDR-3/Hamburg
    {   0, 0x0000, 0x0dd7, {'N', 'D', 'R', '-', '3'}},//NDR-3/Niedersachsen
    {   0, 0x0000, 0x0dd8, {'N', 'D', 'R', '-', '3'}},//RB-1
    {   0, 0x0000, 0x0dda, {' ', 'R', 'B', '-', '3'}},//RB-3
    {   0, 0x0000, 0x0ddb, {'S', 'F', 'B', '-', '1'}},//SFB-1
    {   0, 0x0000, 0x0ddc, {'S', 'F', 'B', '-', '3'}},//SFB-3
    {   0, 0x0000, 0x0ddd, {'S', 'D', 'R', 'W', 'F'}},//SDR-1 + SWF-1
    {   0, 0x0000, 0x0dde, {'S', 'W', 'F', '-', '1'}},//SWF-1
    {   0, 0x0000, 0x0ddf, {' ', 'S', 'R', '-', '1'}},//SR-1
    {   0, 0x0000, 0x0de0, {' ', ' ', 'S', 'W', '3'}},//SW3
//ASIG change {   0, 0x0000, 0x0de1, {' ', ' ', 'S', 'W', '3'}},//SW3/Baden-Wurttemberg
    {   0, 0x49e1, 0x0de1, {' ', ' ', 'S', 'W', '3'}},//SW3/Baden-Wurttemberg,ASIG add
    {   0, 0x0000, 0x0de2, {' ', ' ', 'S', 'W', '3'}},//SW3/Saarland
    {   0, 0x0000, 0x0de3, {' ', ' ', 'S', 'W', '3'}},//SW3/Baden-Wurttemberg Sud
    {   0, 0x0000, 0x0de4, {' ', ' ', 'S', 'W', '3'}},//SW3/Rheinland-Pfalz
    {   0, 0x0000, 0x0de5, {'W', 'D', 'R', '-', '1'}},//WDR-1
    {   0, 0x0000, 0x0de6, {'W', 'D', 'R', '-', '3'}},//WDR-3/Landesweit
    {   0, 0x0000, 0x0de7, {'W', 'D', 'R', '-', '3'}},//
    {   0, 0x0000, 0x0de8, {'W', 'D', 'R', '-', '3'}},//
    {   0, 0x0000, 0x0de9, {'W', 'D', 'R', '-', '3'}},//
    {   0, 0x0000, 0x0dea, {'W', 'D', 'R', '-', '3'}},//
    {   0, 0x0000, 0x0deb, {'W', 'D', 'R', '-', '3'}},//
    {   0, 0x0000, 0x0dec, {'S', 'D', 'R', '-', '1'}},//SDR-1
    {   0, 0x0000, 0x0ded, {' ', ' ', 'S', 'W', '3'}},//SW 3
    {   0, 0x0000, 0x0dee, {' ', ' ', 'S', 'W', '3'}},//
    {   0, 0x0000, 0x0def, {'S', 'D', 'R', 'W', 'F'}},//SDR-1 + SWF-1
    {   0, 0x0000, 0x0df0, {'S', 'W', 'F', '-', '1'}},//SWF-1/Regionalprogramm
    {   0, 0x0000, 0x0df1, {'N', 'D', 'R', '-', '1'}},//NDR-1/Landesprogramm
    {   0, 0x0000, 0x0df2, {'N', 'D', 'R', '-', '3'}},//NDR-3
    {   0, 0x0000, 0x0df3, {'M', 'D', 'R', '-', '1'}},//MDR-1
    {   0, 0x0000, 0x0df4, {'M', 'D', 'R', '-', '3'}},//MDR-3
    {  18, 0x0000, 0x0df5, {' ', ' ', 'M', 'D', 'R'}},//MDR/Dresden
    {   0, 0x0000, 0x0df6, {'M', 'D', 'R', '-', '1'}},//MDR-1/Landesprogramm Sachsen-Anhalt
    {  14, 0x0000, 0x0df7, {' ', ' ', 'W', 'D', 'R'}},//Lokal-Programm WDR
    {   0, 0x0000, 0x0df8, {'M', 'D', 'R', '-', '3'}},//MDR-3
    {  18, 0x0000, 0x0df9, {' ', ' ', 'M', 'D', 'R'}},//MDR
    {   0, 0x0000, 0x0dfa, {'M', 'D', 'R', '-', '1'}},//MDR-1
    {   0, 0x0000, 0x0dfb, {'M', 'D', 'R', '-', '3'}},//MDR-3
    {  18, 0x0000, 0x0dfc, {' ', ' ', 'M', 'D', 'R'}},//
    {   0, 0x0000, 0x0dfd, {'M', 'D', 'R', '-', '1'}},//
    {   0, 0x0000, 0x0dfe, {'M', 'D', 'R', '-', '3'}},//
    {   0, 0x0000, 0x0d81, {'O', 'R', 'B', '-', '1'}},//ORB-1
    {   0, 0x0000, 0x0d82, {'O', 'R', 'B', '-', '3'}},//
    {  22, 0x0000, 0x0d85, {' ', 'A', 'R', 'T', 'E'}},//
    {   0, 0x0000, 0x0d87, {'1', 'A', '-', 'T', 'V'}},//
    {  12, 0x0000, 0x0d88, {' ', 'V', 'I', 'V', 'A'}},//
    {   0, 0x0000, 0x0d89, {'V', 'I', 'V', 'A', '2'}},//
    {  10, 0x0000, 0x0d8a, {'S', ' ', 'R', 'T', 'L'}},//
    {   0, 0x0000, 0x0d8b, {' ', 'C', 'L', 'U', 'B'}},//
    {  25, 0x0000, 0x0d8c, {' ', 'N', '-', 'T', 'V'}},//
    {  20, 0x0000, 0x0d8d, {' ', ' ', 'D', 'S', 'F'}},//
    {   9, 0x0000, 0x0d8e, {' ', ' ', 'V', 'O', 'X'}},//
    {   8, 0x0000, 0x0d8f, {' ', 'R', 'T', 'L', '2'}},//
    {   8, 0x0000, 0x0d90, {' ', 'R', 'T', 'L', '2'}},//
    {  19, 0x0000, 0x0d91, {'S', 'P', 'O', 'R', 'T'}},//
    {   6, 0x0000, 0x0d92, {'K', 'A', 'B', 'E', 'L'}},//
    {   7, 0x0000, 0x0d94, {' ', 'P', 'R', 'O', '7'}},//
    {   5, 0x0000, 0x0d95, {' ', 'S', 'A', 'T', '1'}},//
    {   5, 0x0000, 0x0d96, {' ', 'S', 'A', 'T', '1'}},//
    {   5, 0x0000, 0x0d97, {' ', 'S', 'A', 'T', '1'}},//
    {   5, 0x0000, 0x0d98, {' ', 'S', 'A', 'T', '1'}},//
    {   5, 0x0000, 0x0d99, {' ', 'S', 'A', 'T', '1'}},//
    {   4, 0x0000, 0x0d9a, {' ', ' ', 'R', 'T', 'L'}},//
    {   4, 0x0000, 0x0d9b, {' ', ' ', 'R', 'T', 'L'}},//
    {   4, 0x0000, 0x0d9c, {' ', ' ', 'R', 'T', 'L'}},//
    {   4, 0x0000, 0x0d9d, {' ', ' ', 'R', 'T', 'L'}},//
    {   4, 0x0000, 0x0d9e, {' ', ' ', 'R', 'T', 'L'}},//
    {   4, 0x0000, 0x0d9f, {' ', ' ', 'R', 'T', 'L'}},//
    {   4, 0x0000, 0x0da0, {' ', ' ', 'R', 'T', 'L'}},//
    {   4, 0x0000, 0x0da1, {' ', ' ', 'R', 'T', 'L'}},//
    {   4, 0x0000, 0x0da2, {' ', ' ', 'R', 'T', 'L'}},//
    {   4, 0x0000, 0x0da3, {' ', ' ', 'R', 'T', 'L'}},//
    {   4, 0x0000, 0x0da4, {' ', ' ', 'R', 'T', 'L'}},//
    {   4, 0x0000, 0x0da5, {' ', ' ', 'R', 'T', 'L'}},//
    {   4, 0x0000, 0x0da6, {' ', ' ', 'R', 'T', 'L'}},//
    {   4, 0x0000, 0x0da7, {' ', ' ', 'R', 'T', 'L'}},//
    {   4, 0x0000, 0x0da8, {' ', ' ', 'R', 'T', 'L'}},//
    {   4, 0x0000, 0x0da9, {' ', ' ', 'R', 'T', 'L'}},//
    {   4, 0x0000, 0x0daa, {' ', ' ', 'R', 'T', 'L'}},//
//ASIG change{   0, 0x0000, 0x0dab, {' ', 'P', 'L', 'U', 'S'}},//
    {  21, 0x0000, 0x0dac, {'P', 'R', 'E', 'M', '.'}},//
    {   5, 0x0000, 0x0dad, {' ', 'S', 'A', 'T', '1'}},//
    {   5, 0x0000, 0x0dae, {' ', 'S', 'A', 'T', '1'}},//
    {   5, 0x0000, 0x0daf, {' ', 'S', 'A', 'T', '1'}},//
    {   5, 0x0000, 0x0db0, {' ', 'S', 'A', 'T', '1'}},//
    {   5, 0x0000, 0x0db1, {' ', 'S', 'A', 'T', '1'}},//
    {   5, 0x0000, 0x0db2, {' ', 'S', 'A', 'T', '1'}},//
    {   5, 0x0000, 0x0db3, {' ', 'S', 'A', 'T', '1'}},//
    {   5, 0x0000, 0x0db4, {' ', 'S', 'A', 'T', '1'}},//
    {   5, 0x0000, 0x0db5, {' ', 'S', 'A', 'T', '1'}},//
    {   5, 0x0000, 0x0db6, {' ', 'S', 'A', 'T', '1'}},//
    {   5, 0x0000, 0x0db7, {' ', 'S', 'A', 'T', '1'}},//
    {   5, 0x0000, 0x0db8, {' ', 'S', 'A', 'T', '1'}},//
    {   5, 0x0000, 0x0db9, {' ', 'S', 'A', 'T', '1'}},//
//ASIG change{   0, 0x0000, 0x0dba, {' ', ' ', 'T', 'M', '3'}},//
    {   0, 0xc578, 0xfdba, {'9', 'L', 'I', 'V', 'E'}},//ASIG Add
    {   0, 0x0000, 0x0dbb, {' ', 'D', 'W', 'T', 'V'}},//
    {   0, 0x0000, 0x0dbd, {'O', '.', 'K', 'A', 'N'}},//
    {   0, 0x0000, 0x0dbe, {'B', 'M', 'I', 'X', '2'}},//
    {   0, 0x0000, 0x0dbf, {'B', 'M', '1', 'X', '1'}},//
    {   0, 0x0000, 0x0d41, {'F', 'E', 'S', 'T', '.'}},//FESTIVAL
    {   0, 0x0000, 0x0d42, {' ', 'M', 'U', 'X', 'X'}},//
    {   0, 0x0000, 0x0d43, {'E', 'X', 'T', 'R', 'A'}},//
    {   0, 0x0000, 0x0d7c, {' ', 'O', 'N', 'Y', 'X'}},//ONYX-TV
    {   0, 0x0000, 0x0d7d, {' ', ' ', 'Q', 'V', 'C'}},//
    {   0, 0x0000, 0x0d7e, {'N', 'I', 'C', 'K', '.'}},//
//ASIG Change {   0, 0x0000, 0x0d7f, {' ', ' ', 'H', 'O', 'T'}},//
    {   0, 0x0000, 0x0d7f, {' ', ' ', 'H', 'S', 'E'}},//ASIG add
// Germany for other country UK & Turkey & France
//Germany of PDC
//ASIG change{  22, 0x490a, 0x0000, {' ', 'A', 'R', 'T', 'E'}},//Arte
    {  22, 0x490a, 0xfd85, {' ', 'A', 'R', 'T', 'E'}},//Arte,ASIG add
    {   0, 0x5c49, 0x0000, {'Q', 'V', 'C', ' ', 'D'}},//QVC D Gmbh
    {   0, 0x4101, 0x0000, {'S', 'F', '1', '/', 'D'}},//SF1 D Gmbh,ASIG add
    {   0, 0x4918, 0x0000, {'P', 'H', 'O', 'E', 'N'}},//PHOENix,,ASIG add
    {   0, 0x49be, 0xfd78, {'T', 'E', 'L', 'E', '5'}},//TELE5,,ASIG add
    {   0, 0x0000, 0xfd8c, {'N', 'T', 'V', '/', 'N'}},//NTV/N,,ASIG add
    {   0, 0x0000, 0xfd8e, {' ', ' ', 'V', 'O', 'X'}},//VOX,,ASIG add
    {  11, 0x0000, 0x0000, {' ', ' ', 'M', 'T', 'V'}},//MTV,ASIG add
//ASIG change {   0, 0x0000, 0xfdc9, {'K', 'I', 'N', 'D', '/'}},//KIND/,ASIG add
    {   0, 0x0000, 0xfd8d, {' ', ' ', 'D', 'S', 'F'}},//DSF,ASIG add
    {   0, 0x0000, 0xfd92, {'K', 'A', 'B', 'E', 'L'}},//KABEL,ASIG add
    {   0, 0x0000, 0xfd8f, {' ', 'R', 'T', 'L', '2'}},// RTL2,ASIG add
    {   0, 0x0000, 0xfde6, {' ', 'W', 'D', 'R', '3'}},// WDR3,ASIG add
    {   0, 0x0000, 0xfd7a, {' ', ' ', 'N', '2', '4'}},// N24,ASIG add
    {   0, 0x0000, 0xfd7f, {'H', 'O', 'T', '/', 'H'}},// HOT/H,ASIG add
//{  23, 0x0000, 0x0d76, {' ', ' ', 'N', 'B', 'C'}},// NBC,ASIG add
    {   23, 0x0000, 0x0d76, {'D', 'A', 'S', ' ', 'V'}},// DAS ASIG add
    {   0, 0x0000, 0xfdab, {' ', ' ', 'R', 'T', 'L'}},// RTL,ASIG add
    {   0, 0x4944, 0x0d44, {'B', 'R', '-', 'A', '/'}},// BR-A/,ASIG add
//ASIG change {   9, 0x490c, 0x0000, {' ', ' ', 'V', 'O', 'X'}},//VOX Television
//ASIG change {  11, 0x4d54, 0x0000, {' ', ' ', 'M', 'T', 'V'}},//MTV
//ASIG change {  23, 0x4d55, 0x0000, {' ', ' ', 'N', 'B', 'C'}},//
    {  26, 0x4d56, 0x0000, {' ', 'N', 'E', 'W', 'S'}},//
//ASIG Change {  27, 0x01f2, 0x0000, {' ', 'C', 'N', 'N', 'I'}},//CNNI
    {  27, 0x01f2, 0x0000, {' ', ' ', 'C', 'N', 'N'}},//CNN,ASIG add
    {  29, 0x4d58, 0x0000, {' ', 'V', 'H', '-', '1'}},//VH-1
    {  29, 0x4d59, 0x0000, {' ', 'V', 'H', '-', '1'}},//VH-1(German lang.)
    {  29, 0x4d57, 0x0000, {' ', 'V', 'H', '-', '1'}},//VH-1 future
    //Turkey
    {  24, 0x9001, 0x0000, {' ', 'T', 'R', 'T', '1'}},//TRT-1
    //France
    {  28, 0xf500, 0x0000, {' ', ' ', 'T', 'V', '5'}},//TV 5
#endif
    {0xff, 0xffff, 0xffff, {'-', '-', '-', '-', '-'}}, //END_OF_TABLE
};
// 138 items

CNI_NETWORK_TABLE code Ats_Ranking_List_Greece[] =
{
#if _ENABLE_CNI_TABLE
    {   1, 0x3001, 0x2101, {' ', 'E', 'T', '-', '1'}},//
    {   3, 0x3003, 0x2103, {' ', 'E', 'T', '-', '3'}},//
    {   2, 0x3002, 0x2102, {' ', ' ', 'N', 'E', 'T'}},//
    {   0, 0x3004, 0x0000, {' ', ' ', ' ', 'E', 'T'}},//ET future
    {   0, 0x3005, 0x0000, {' ', ' ', ' ', 'E', 'T'}},//ET future
    {   0, 0x3006, 0x0000, {' ', ' ', ' ', 'E', 'T'}},//ET future
    {   0, 0x3007, 0x0000, {' ', ' ', ' ', 'E', 'T'}},//ET future
    {   0, 0x3008, 0x0000, {' ', ' ', ' ', 'E', 'T'}},//ET future
    {   0, 0x3009, 0x0000, {' ', ' ', ' ', 'E', 'T'}},//ET future
    {   0, 0x300a, 0x0000, {' ', ' ', ' ', 'E', 'T'}},//ET future
    {   0, 0x300b, 0x0000, {' ', ' ', ' ', 'E', 'T'}},//ET future
    {   0, 0x300c, 0x0000, {' ', ' ', ' ', 'E', 'T'}},//ET future
    {   0, 0x300d, 0x0000, {' ', ' ', ' ', 'E', 'T'}},//ET future
    {   0, 0x300e, 0x0000, {' ', ' ', ' ', 'E', 'T'}},//ET future
    {   0, 0x300f, 0x0000, {' ', ' ', ' ', 'E', 'T'}},//ET future
#endif
    {0xff, 0xffff, 0xffff, {'-', '-', '-', '-', '-'}}, //END_OF_TABLE
};
// 16 items

CNI_NETWORK_TABLE code Ats_Ranking_List_Hungary[] =
{
#if _ENABLE_CNI_TABLE
    {   0, 0X3636, 0X0000, {'D', 'U', 'N', 'A', ' '}},//Duna Televizio
    {   1, 0x3601, 0x0000, {' ', 'M', 'T', 'V', '1'}},//MTV1
    {   1, 0x3681, 0x0000, {' ', 'M', 'T', 'V', '1'}},//MTV1 future
    {   1, 0x3611, 0x0000, {' ', 'M', 'T', 'V', '1'}},//MTV1 Budapest
    {   1, 0x3651, 0x0000, {' ', 'M', 'T', 'V', '1'}},//MTV1 Debrecen
    {   1, 0x3661, 0x0000, {' ', 'M', 'T', 'V', '1'}},//MTV1 Miskolc
    {   1, 0x3621, 0x0000, {' ', 'M', 'T', 'V', '1'}},//MTV1 Pecs
    {   1, 0x3631, 0x0000, {' ', 'M', 'T', 'V', '1'}},//MTV1 Szeged
    {   1, 0x3641, 0x0000, {' ', 'M', 'T', 'V', '1'}},//MTV1 Szombath.
    {   2, 0x3602, 0x0000, {' ', 'M', 'T', 'V', '2'}},//MTV2
    {   2, 0x3682, 0x0000, {' ', 'M', 'T', 'V', '2'}},//MTV2 future
    {   0, 0x3622, 0x0000, {' ', ' ', 'T', 'V', '2'}},//tv2
    {   0, 0x3620, 0x0000, {' ', ' ', 'T', 'V', '2'}},//tv2 future use
#endif
    {0xff, 0xffff, 0xffff, {'-', '-', '-', '-', '-'}}, //END_OF_TABLE
};
// 14 items

CNI_NETWORK_TABLE code Ats_Ranking_List_Iceland[] =
{
#if _ENABLE_CNI_TABLE
    {   1, 0x3541, 0x0000, {'R', '-', 'S', ' ', ' '}},//Rikisutvarpid-Sjonvarp
#endif
    {0xff, 0xffff, 0xffff, {'-', '-', '-', '-', '-'}}, //END_OF_TABLE
};
// 2 items

CNI_NETWORK_TABLE code Ats_Ranking_List_Ireland[] =
{
#if _ENABLE_CNI_TABLE
    {   2, 0x3532, 0x4202, {' ', 'N', 'E', 'T', '2'}},//Network 2
    {   0, 0x3534, 0x4204, {' ', ' ', 'R', 'T', 'E'}},//RTE future
    {   0, 0x3535, 0x4205, {' ', ' ', 'R', 'T', 'E'}},//RTE future
    {   0, 0x3536, 0x4206, {' ', ' ', 'R', 'T', 'E'}},//RTE future
    {   0, 0x3537, 0x4207, {' ', ' ', 'R', 'T', 'E'}},//RTE future
    {   0, 0x3538, 0x4208, {' ', ' ', 'R', 'T', 'E'}},//RTE future
    {   0, 0x3539, 0x4209, {' ', ' ', 'R', 'T', 'E'}},//RTE future
    {   0, 0x353a, 0x420a, {' ', ' ', 'R', 'T', 'E'}},//RTE future
    {   0, 0x353b, 0x420b, {' ', ' ', 'R', 'T', 'E'}},//RTE future
    {   0, 0x353c, 0x420c, {' ', ' ', 'R', 'T', 'E'}},//RTE future
    {   0, 0x353d, 0x420d, {' ', ' ', 'R', 'T', 'E'}},//RTE future
    {   0, 0x353e, 0x420e, {' ', ' ', 'R', 'T', 'E'}},//RTE future
    {   0, 0x353f, 0x420f, {' ', ' ', 'R', 'T', 'E'}},//RTE future
    {   1, 0x3531, 0x4201, {' ', 'R', 'T', 'E', '1'}},//RTE1
    {   4, 0x3533, 0x4203, {' ', ' ', 'T', 'N', 'G'}},//Teilifis na Gaeilge
    {   3, 0x3333, 0x0000, {' ', ' ', 'T', 'V', '3'}},//TV3
#endif
    {0xff, 0xffff, 0xffff, {'-', '-', '-', '-', '-'}}, //END_OF_TABLE
};
// 17 items

// Italy (PDC)
CNI_NETWORK_TABLE code Ats_Ranking_List_Italy[] =
{
#if _ENABLE_CNI_TABLE
    {   1, 0x3901, 0x0000, {' ', 'R', 'A', 'I', '1'}},//RAI 1
    {   2, 0x3902, 0x0000, {' ', 'R', 'A', 'I', '2'}},//
    {   3, 0x3903, 0x0000, {' ', 'R', 'A', 'I', '3'}},//
    {   8, 0x3904, 0x0000, {'R', 'E', 'T', 'E', 'A'}},//Rete A
//ASIG change {   0, 0x3905, 0x1505, {' ', 'C', '.', 'I', '.'}},//Canale Italia
    {   0, 0x9CA0, 0x5063, {' ', 'C', '.', 'I', '.'}},//Canale Italia,ASIG add
    {   0, 0x3906, 0x0000, {'7', 'G', 'O', 'L', 'D'}},//7 Gold
    {   0, 0x3909, 0x0000, {'N', 'O', 'V', 'A', ' '}},//Telenova
    {   0, 0x390a, 0x0000, {' ', 'A', 'R', 'T', 'E'}},//Arte
    {   6, 0x3910, 0x0000, {' ', ' ', 'T', 'R', 'S'}},//TRS TV
    {   0, 0x3911, 0x1511, {'S', 'K', 'Y', ' ', 'C'}},//Sky Cinema Classic
    {   0, 0x3912, 0x1512, {' ', ' ', 'S', 'K', 'Y'}},//Sky Future use
    {   0, 0x3913, 0x1513, {'S', '.', 'C', 'A', '1'}},//Sky Calicio 1
    {   0, 0x3914, 0x1514, {'S', '.', 'C', 'A', '2'}},//Sky Calicio 2
    {   0, 0x3915, 0x1515, {'S', '.', 'C', 'A', '3'}},//Sky Calicio 3
    {   0, 0x3916, 0x1516, {'S', '.', 'C', 'A', '4'}},//Sky Calicio 4
    {   0, 0x3917, 0x1517, {'S', '.', 'C', 'A', '5'}},//Sky Calicio 5
    {   0, 0x3918, 0x1518, {'S', '.', 'C', 'A', '6'}},//Sky Calicio 6
    {   0, 0x3919, 0x1519, {'S', '.', 'C', 'A', '7'}},//Sky Calicio 7
    {   0, 0x3920, 0x0000, {'R', 'N', '2', '4', ' '}},//RaiNotizie24
    {   0, 0x3921, 0x0000, {'R', 'A', 'I', ' ', 'M'}},//RAI Med
    {   0, 0x3922, 0x0000, {'R', 'S', 'S', ' ', ' '}},//RAI SportSat
    {   0, 0x3923, 0x0000, {'R', '.', 'E', 'D', '1'}},//RAI Edu1
    {   0, 0x3924, 0x0000, {'R', '.', 'E', 'D', '2'}},//RAI Edu2
    {   0, 0x3925, 0x0000, {'R', '.', 'N', 'S', '1'}},//RAI NettunoSat1
    {   0, 0x3926, 0x0000, {'R', '.', 'N', 'S', '2'}},//RAI NettunoSat2
    {   0, 0x3927, 0x0000, {'C', '.', 'D', 'E', 'P'}},//CameraDeputati
    {   0, 0x3928, 0x0000, {'S', 'E', 'N', 'A', '.'}},//Senato
    {   0, 0x3929, 0x0000, {'R', 'A', 'I', ' ', 'U'}},//RAIUtile
    {   0, 0x392a, 0x0000, {'R', '.', 'D', 'F', ' '}},//RAIDocFutura
    {   0, 0x392b, 0x0000, {'R', '.', 'T', '2', ' '}},//RAITest2
    {   0, 0x392c, 0x0000, {' ', ' ', 'R', 'A', 'I'}},//RAI future use
    {   0, 0x392d, 0x0000, {' ', ' ', 'R', 'A', 'I'}},//RAI future use
    {   0, 0x392e, 0x0000, {' ', ' ', 'R', 'A', 'I'}},//RAI future use
    {   0, 0x392f, 0x0000, {' ', ' ', 'R', 'A', 'I'}},//RAI future use
    {   0, 0x3930, 0x0000, {'D', 'I', 'S', 'C', '.'}},//Discovery Italy
    {   0, 0x3933, 0x0000, {' ', ' ', 'M', 'T', 'V'}},//MTV Italia
    {   0, 0x3934, 0x0000, {'M', 'T', 'V', ' ', 'N'}},//MTV Brand New
    {   0, 0x3935, 0x0000, {'M', 'T', 'V', ' ', 'H'}},//MTV Hits
    {  13, 0x3938, 0x0000, {'R', 'T', 'V', '3', '8'}},//RTV38
    {   0, 0x3939, 0x0000, {' ', ' ', 'G', 'A', 'Y'}},//GAY TV
    {   0, 0x3940, 0x0000, {'V', '.', 'I', 'T', 'A'}},//Video Italia
    {   0, 0x3941, 0x0000, {'S', '2', '0', '0', '0'}},//SAT 2000
    {   0, 0x3942, 0x1542, {'J', 'I', 'M', 'M', 'Y'}},//Jimmy
    {   0, 0x3943, 0x1543, {'P', 'L', 'A', 'N', 'E'}},//Planet
    {   0, 0x3944, 0x1544, {' ', ' ', ' ', 'C', 'N'}},//Cartoon Network
    {   0, 0x3945, 0x1545, {'B', 'O', 'O', 'M', 'E'}},//Boomerang
    {   0, 0x3946, 0x1546, {'C', 'N', 'N', ' ', 'I'}},//CNN International
    {   0, 0x3947, 0x1547, {'C', 'N', '+', '1', ' '}},//Cartoon Network +1
    {   0, 0x3948, 0x1548, {'S', '.', 'S', 'P', '3'}},//Sky Sports 3
    {   0, 0x3949, 0x1549, {'S', '.', 'D', 'G', ' '}},//Sky Diretta Gol
    {   0, 0x3950, 0x0000, {'R', 'S', '.', 'A', 'L'}},//RAISat Album
    {   0, 0x3951, 0x0000, {'R', 'S', '.', 'A', 'R'}},//RAISat Art
    {   0, 0x3952, 0x0000, {'R', 'S', '.', 'C', 'I'}},//RAISat Cinema
    {   0, 0x3953, 0x0000, {'R', 'S', '.', 'F', 'I'}},//RAISat Fiction
    {   0, 0x3954, 0x0000, {'R', 'S', '.', 'G', 'R'}},//RAISat Gambero Rosso
    {   0, 0x3955, 0x0000, {'R', 'S', '.', 'R', 'A'}},//RAISat Ragazzi
    {   0, 0x3956, 0x0000, {' ', ' ', 'R', 'A', 'I'}},//RAI future use
    {   0, 0x3957, 0x0000, {' ', ' ', 'R', 'A', 'A'}},//RAI future use
    {   0, 0x3958, 0x0000, {'R', 'S', '.', 'C', 'W'}},//RAISat Cinema World
    {   0, 0x3959, 0x0000, {'R', 'S', '.', 'E', 'X'}},//RAISat Extra
    {   0, 0x395a, 0x0000, {'R', 'S', '.', 'P', 'R'}},//RAISat Premium
    {   0, 0x395b, 0x0000, {' ', ' ', 'R', 'A', 'I'}},//RAISat future use
    {   0, 0x395c, 0x0000, {' ', ' ', 'R', 'A', 'I'}},//RAISat future use
    {   0, 0x395d, 0x0000, {' ', ' ', 'R', 'A', 'I'}},//RAISat future use
    {   0, 0x395e, 0x0000, {' ', ' ', 'R', 'A', 'I'}},//RAISat future use
    {   0, 0x395f, 0x0000, {' ', ' ', 'R', 'A', 'I'}},//RAISat future use
    {   0, 0x3960, 0x1560, {'S', 'C', 'I', ' ', 'F'}},//SCI FI CHANNEL
    {   0, 0x3961, 0x0000, {'D', 'I', 'S', '.', 'C'}},//Discovery Civilisations
    {   0, 0x3962, 0x0000, {'D', 'I', '.', 'T', 'A'}},//Discovery Travel and Adventure
    {   0, 0x3963, 0x0000, {'D', 'I', '.', 'S', 'I'}},//Discovery Science
    {   0, 0x3968, 0x1568, {'S', '.', 'M', '2', '4'}},//Sky Meteo24
    {   0, 0x3970, 0x0000, {'S', '.', 'C', 'I', '2'}},//Sky Cinema 2
    {   0, 0X3971, 0X0000, {'S', '.', 'C', 'I', '3'}},//Sky Cinema 3
    {   0, 0x3972, 0x0000, {'S', '.', 'C', 'I', 'A'}},//Sky Cinema Autore
    {   0, 0x3973, 0x0000, {'S', '.', 'C', 'I', 'M'}},//Sky Cinema Max
    {   0, 0x3974, 0x0000, {'S', '.', 'C', 'I', 'W'}},//Sky Cinema 16:9
    {   0, 0x3975, 0x0000, {'S', '.', 'S', 'P', '2'}},//Sky Sports 2
    {   0, 0x3976, 0x0000, {'S', 'T', 'G', '2', '4'}},//Sky TG24
    {   0, 0x3977, 0x1577, {' ', ' ', 'F', 'O', 'X'}},//Fox
    {   0, 0x3978, 0x1578, {'F', 'O', 'X', ' ', 'L'}},//Foxlife
    {   0, 0x3979, 0x1579, {' ', ' ', 'N', 'G', 'C'}},//National Geographic Channel
    {   0, 0x3980, 0x1580, {' ', ' ', ' ', 'A', '1'}},//A1
    {   0, 0x3981, 0x1581, {'H', 'I', 'S', '.', 'C'}},//History Channel
    {   0, 0x3985, 0x0000, {'F', '.', 'K', 'I', 'D'}},//FOX KIDS
    {   0, 0x3986, 0x0000, {'P', 'T', '-', 'R', '7'}},//PEOPLE TV - RETE 7
    {   0, 0x3987, 0x0000, {'F', '.', 'K', '+', '1'}},//FOX KIDS + 1
    {   0, 0x3988, 0x0000, {' ', ' ', 'L', 'A', '7'}},//LA7
    {   0, 0x3989, 0x0000, {'P', 'R', 'I', 'M', 'A'}},//PrimaTV
    {   0, 0x398A, 0x0000, {'S', 'P', '.', 'I', 'T'}},//SportItalia
    {   0, 0x398F, 0x0000, {'E', 'S', 'P', '.', ' '}},//Espansione TV
    {   0, 0x3990, 0x1590, {'S', '.', 'U', 'N', 'I'}},//STUDIO UNIVERSAL
    {   0, 0x3991, 0x1591, {'M', 'A', 'R', 'C', 'O'}},//Marcopolo
    {   0, 0x3992, 0x1592, {'A', 'L', 'I', 'C', 'E'}},//Alice
    {   0, 0x3993, 0x1593, {'N', 'U', 'V', 'O', '.'}},//Nuvolari
    {   0, 0x3994, 0x1594, {'L', 'E', 'O', 'N', '.'}},//Leonardo
    {   0, 0x3996, 0x1596, {'S', 'U', 'P', '.', 'C'}},//SUPERPIPPA CHANNEL
    {   0, 0x3997, 0x0000, {'S', '.', 'S', 'P', '1'}},//Sky Sports 1
    {   0, 0x3998, 0x0000, {'S', '.', 'C', 'I', '1'}},//Sky Cinema 1
    {  12, 0x3999, 0x0000, {'T', 'E', 'L', '+', '3'}},//Tele+3
    {   0, 0x399A, 0x150A, {'S', 'I', 'T', '.', '1'}},//Sitcom 1
    {   0, 0x399b, 0x150b, {'S', 'I', 'T', '.', '2'}},//Sitcom 1
    {   0, 0x399c, 0x150c, {'S', 'I', 'T', '.', '3'}},//Sitcom 3
    {   0, 0x399d, 0x150d, {'S', 'I', 'T', '.', '4'}},//Sitcom 4
    {   0, 0x399e, 0x150e, {'S', 'I', 'T', '.', '5'}},//Sitcom 5
    {   0, 0x399f, 0x150f, {' ', ' ', 'I', 'N', 'M'}},//Italiani nel Mondo
    {   0, 0x39A0, 0X15A0, {'S', '.', 'C', 'A', '8'}},//Sky Calcio 8
    {   0, 0x39A1, 0X15A1, {'S', '.', 'C', 'A', '9'}},//Sky Calcio 9
    {   0, 0x39A2, 0X15A2, {'S', 'C', 'A', '1', '0'}},//Sky Calcio 10
    {   0, 0x39A3, 0X15A3, {'S', 'C', 'A', '1', '1'}},//Sky Calcio 11
    {   0, 0x39A4, 0X15A4, {'S', 'C', 'A', '1', '2'}},//Sky Calcio 12
    {   0, 0x39A5, 0X15A5, {'S', 'C', 'A', '1', '3'}},//Sky Calcio 13
    {   0, 0x39A6, 0X15A6, {'S', 'C', 'A', '1', '4'}},//Sky Calcio 14
    {   0, 0x39A7, 0x15A7, {'T', '.', 'S', 'A', 'N'}},//Telesanterno
    {   0, 0x39A8, 0x15A8, {'T', '.', 'C', 'E', 'N'}},//Telecentro
    {   0, 0x39A9, 0x15A9, {'T', '.', 'S', 'T', 'E'}},//Telestense
    {   0, 0x39B0, 0x15B0, {'D', 'S', 'C', '+', '1'}},//Disney Channel + 1
    {   0, 0x39B1, 0x0000, {'S', 'A', 'I', '.', 'C'}},//Sailing Channel
    {   0, 0x39B2, 0x15B2, {' ', ' ', 'D', 'S', 'C'}},//Disney Channel
    {   0, 0x39B3, 0x15B3, {'7', 'G', '-', 'S', 'A'}},//7 Gold-Sestra Rete
    {   0, 0x39B4, 0x15B4, {'R', '8', 'V', 'G', 'A'}},//Rete 8-VGA
    {   0, 0x39B5, 0x15B5, {'N', 'U', 'O', 'V', '.'}},//Nuovarete
    {   0, 0x39B6, 0x15B6, {'R', 'I', 'T', 'V', ' '}},//Radio Italia TV
    {   0, 0x39B7, 0x15B7, {'R', 'E', 'T', 'E', '7'}},//Rete 7
    {   0, 0x39B8, 0x15B8, {'E', ' ', 'E', 'T', 'V'}},//E! Entertainment Television
    {   0, 0x39B9, 0x15B9, {'T', '.', 'D', 'S', 'C'}},//Toon Disney
    {   0, 0x39C7, 0x15C7, {'B', 'A', 'S', '.', ' '}},//Bassano TV
    {   0, 0x39C8, 0x15C8, {'E', 'S', 'P', 'N', ' '}},//ESPA Classic sport
    {   0, 0x39CA, 0x0000, {'V', '.', 'L', 'I', 'N'}},//VIDEOLINA
    {   0, 0x39D2, 0x15D2, {'M', 'P', 'R', 'E', '1'}},//Mediaset Premium 1
    {   0, 0x39D3, 0x15D3, {'M', 'P', 'R', 'E', '2'}},//Mediaset Premium 2
    {   0, 0x39D4, 0x15D4, {'M', 'P', 'R', 'E', '3'}},//Mediaset Premium 3
    {   0, 0x39D5, 0x15D5, {'M', 'P', 'R', 'E', '4'}},//Mediaset Premium 4
    {   0, 0x39D1, 0x15D1, {'M', 'P', 'R', 'E', '5'}},//Mediaset Premium 5
    {   0, 0x39D6, 0x15D6, {'B', 'O', 'I', 'N', 'G'}},//BOING
    {   0, 0x39D7, 0x15D7, {'P', 'L', 'S', '.', 'I'}},//Playlist Italia
    {   0, 0x39D8, 0x15D8, {'M', 'A', 'T', '.', 'M'}},//MATCH MUSIC
    {   0, 0x39DA, 0x0000, {'M', 'E', 'D', ' ', 'S'}},//Mediashopping
    {   0, 0x39DB, 0x0000, {'M', 'E', 'D', 'I', 'A'}},//Mediaset Future Use
    {   0, 0x39DC, 0x0000, {'M', 'E', 'D', 'I', 'A'}},//Mediaset Future Use
    {   0, 0x39DD, 0x0000, {'M', 'E', 'D', 'I', 'A'}},//Mediaset Future Use
    {   0, 0x39DE, 0x0000, {'M', 'E', 'D', 'I', 'A'}},//Mediaset Future Use
    {   0, 0x39DF, 0x0000, {'M', 'E', 'D', 'I', 'A'}},//Mediaset Future Use
    {   0, 0x39E1, 0x15E1, {'N', 'G', '+', '1', ' '}},//National Geographic + 1
    {   0, 0x39E2, 0x15E2, {'H', 'C', '+', '1', ' '}},//History Chanel + 1
    {   0, 0x39e3, 0x15e3, {'S', 'K', 'Y', 'T', 'V'}},//Sky TV
    {   0, 0x39E4, 0x15E4, {' ', ' ', 'G', 'X', 'T'}},//GXT
    {   0, 0x39E5, 0x15e5, {'P', '.', 'D', 'S', 'N'}},//Playhouse Disney
    {   0, 0x39E6, 0x15e6, {'S', 'C', '2', '2', '4'}},//Sky Canale224
    {   0, 0x39F1, 0x0000, {' ', ' ', 'T', 'R', 'C'}},//Teleradiocity
    {   0, 0x39F2, 0x0000, {'T', 'R', 'C', 'G', ' '}},//Teleradiocity Genova
    {   0, 0x39F3, 0x0000, {'T', 'R', 'C', 'L', ' '}},//Teleradiocity Lombardia
    {   0, 0x39F4, 0x0000, {'T', 'S', 'P', 'I', 'E'}},//Telestar Piemonte
    {   0, 0x39F5, 0x0000, {'T', 'S', 'L', 'I', 'G'}},//Telestar Liguria
    {   0, 0x39F6, 0x0000, {'T', 'S', 'L', 'O', 'M'}},//Telestar Lombardia
    {   0, 0x39F7, 0x0000, {'I', '8', 'P', 'I', 'E'}},//Italia 8 Piemonte
    {   0, 0x39f8, 0x0000, {'I', '8', 'L', 'O', 'M'}},//Italia 8 Lombardia
    {   0, 0x39F9, 0x0000, {' ', 'R', 'T', 'E', 'U'}},//Radio Tele Europa
    {   4, 0xfa04, 0x0000, {'R', 'E', 'T', 'E', '4'}},//Rete 4
    {   5, 0xfa05, 0x0000, {'C', 'A', 'N', ' ', '5'}},//Canale 5
    {   6, 0xfa06, 0x0000, {'I', 'T', 'A', 'L', '1'}},//Italia 1
    {   0, 0x39E7, 0x0000, {'M', ' ', 'B', 'O', 'X'}},//Music Box
    {   0, 0x39d9, 0x0000, {'T', 'S', 'U', 'P', '3'}},//Televisiva SUPER3
    {   0, 0x39E9, 0x0000, {'T', 'N', '7', ' ', ' '}},//TN7 Telenorba
    {   0, 0x391A, 0x0000, {'T', 'N', '8', ' ', ' '}},//TN8 Telenorba
    {   0, 0x393A, 0x0000, {'T', 'N', '9', ' ', ' '}},//TN9 Telepuglia
    {   0, 0x394A, 0x0000, {'T', 'G', ' ', 'N', ' '}},//TG NORBA

// {  10, 0x3997, 0x0000, {'T', 'E', 'L', '+', '1'}},//Tele+1
// {  11, 0x3998, 0x0000, {'T', 'E', 'L', '+', '2'}},//Tele+2
    {   7, 0xfa08, 0x0000, {' ', ' ', 'T', 'M', 'C'}},//
#endif
    {0xff, 0xffff, 0xffff, {'-', '-', '-', '-', '-'}}, //END_OF_TABLE
};
// 168 items


CNI_NETWORK_TABLE code Ats_Ranking_List_Netherlands[] =
{
#if _ENABLE_CNI_TABLE
    {   1, 0x3101, 0x4801, {' ', 'N', 'E', 'D', '1'}},//Nederland 1
    {   2, 0x3102, 0x4802, {' ', 'N', 'E', 'D', '2'}},//Nederland 2
    {   3, 0x3103, 0x4803, {' ', 'N', 'E', 'D', '3'}},//Nederland 3
    {   4, 0x3104, 0x4804, {' ', 'R', 'T', 'L', '4'}},//RTL 4
    {   5, 0x3105, 0x4805, {' ', 'R', 'T', 'L', '5'}},//RTL 5
// {   6, 0x3106, 0x4806, {'V', 'E', 'R', 'O', 'N'}},//Veronica
    {   7, 0x3147, 0x4847, {' ', 'R', 'T', 'L', '7'}},//RTL7
    {   0, 0x3110, 0x0000, {' ', ' ', 'N', 'O', 'S'}},//NOS future use
    {   0, 0x3111, 0x0000, {' ', ' ', 'N', 'O', 'S'}},//NOS future use
    {   0, 0x3112, 0x0000, {' ', ' ', 'N', 'O', 'S'}},//NOS future use
    {   0, 0x3113, 0x0000, {' ', ' ', 'N', 'O', 'S'}},//NOS future use
    {   0, 0x3114, 0x0000, {' ', ' ', 'N', 'O', 'S'}},//NOS future use
    {   0, 0x3115, 0x0000, {' ', ' ', 'N', 'O', 'S'}},//NOS future use
    {   0, 0x3116, 0x0000, {' ', ' ', 'N', 'O', 'S'}},//NOS future use
    {   0, 0x3117, 0x0000, {' ', ' ', 'N', 'O', 'S'}},//NOS future use
    {   0, 0x3118, 0x0000, {' ', ' ', 'N', 'O', 'S'}},//NOS future use
    {   0, 0x3119, 0x0000, {' ', ' ', 'N', 'O', 'S'}},//NOS future use
    {   0, 0x311a, 0x0000, {' ', ' ', 'N', 'O', 'S'}},//NOS future use
    {   0, 0x311b, 0x0000, {' ', ' ', 'N', 'O', 'S'}},//NOS future use
    {   0, 0x311c, 0x0000, {' ', ' ', 'N', 'O', 'S'}},//NOS future use
    {   0, 0x311d, 0x0000, {' ', ' ', 'N', 'O', 'S'}},//NOS future use
    {   0, 0x311e, 0x0000, {' ', ' ', 'N', 'O', 'S'}},//NOS future use
    {   0, 0x311f, 0x0000, {' ', ' ', 'N', 'O', 'S'}},//NOS future use
    {   0, 0x3107, 0x4807, {' ', ' ', 'N', 'O', 'S'}},//NOS future use
    {   0, 0x3108, 0x4808, {' ', ' ', 'N', 'O', 'S'}},//NOS future use
    {   0, 0x3109, 0x4809, {' ', ' ', 'N', 'O', 'S'}},//NOS future use
    {   0, 0x310a, 0x480A, {' ', ' ', 'N', 'O', 'S'}},//NOS future use
    {   0, 0x310b, 0x480B, {' ', ' ', 'N', 'O', 'S'}},//NOS future use
    {   0, 0x310c, 0x480C, {' ', ' ', 'N', 'O', 'S'}},//NOS future use
    {   0, 0x310d, 0x480D, {' ', ' ', 'N', 'O', 'S'}},//NOS future use
    {   0, 0x310e, 0x480E, {' ', ' ', 'N', 'O', 'S'}},//NOS future use
    {   0, 0x310f, 0x480F, {' ', ' ', 'N', 'O', 'S'}},//NOS future use
    {   0, 0x3120, 0x4820, {' ', ' ', 'B', 'O', 'X'}},//The BOX
    {   0, 0x3121, 0x0000, {'D', 'I', 'S', 'C', 'O'}},//Discovery Netherlands
    {   0, 0x3122, 0x4822, {'N', 'I', 'C', 'K', '.'}},//Nickelodeon
    {   0, 0x3123, 0x0000, {' ', ' ', 'A', 'P', 'B'}},//Animal Planet Benelux
    {   0, 0x3124, 0x0000, {'T', 'A', 'L', 'P', 'A'}},//TALPA TV
    {   0, 0x3125, 0x0000, {' ', 'N', 'E', 'T', '5'}},//NET5
    {   0, 0x3126, 0x0000, {' ', 'S', 'B', 'S', '6'}},//SBS6
    {   0, 0x3127, 0x0000, {' ', ' ', 'S', 'B', 'S'}},//SBS future use
    {   0, 0x3128, 0x0000, {' ', ' ', ' ', 'V', '8'}},//V8
    {   0, 0x3129, 0x0000, {' ', ' ', 'S', 'B', 'S'}},//SBS future use
    {   0, 0x312a, 0x0000, {' ', ' ', 'S', 'B', 'S'}},//SBS future use
    {   0, 0x312b, 0x0000, {' ', ' ', 'S', 'B', 'S'}},//SBS future use
    {   0, 0x312c, 0x0000, {' ', ' ', 'S', 'B', 'S'}},//SBS future use
    {   0, 0x312d, 0x0000, {' ', ' ', 'S', 'B', 'S'}},//SBS future use
    {   0, 0x312e, 0x0000, {' ', ' ', 'S', 'B', 'S'}},//SBS future use
    {   0, 0x312f, 0x0000, {' ', ' ', 'S', 'B', 'S'}},//SBS future use
    {   0, 0x3130, 0x0000, {' ', ' ', 'T', 'M', 'F'}},//TMF (Netherlands service)
    {   0, 0x3131, 0x0000, {' ', ' ', 'T', 'M', 'F'}},//TMF (Belgian Flanders service)
    {   0, 0x3132, 0x0000, {'M', 'T', 'V', 'N', 'L'}},//MTV NL
    {   0, 0x3137, 0x0000, {' ', 'R', 'N', 'N', '7'}},
#endif
    {0xff, 0xffff, 0xffff, {'-', '-', '-', '-', '-'}}, //END_OF_TABLE
};
// 52 items

CNI_NETWORK_TABLE code Ats_Ranking_List_Norway[] =
{
#if _ENABLE_CNI_TABLE
    {   1, 0x4701, 0x0000, {' ', 'N', 'R', 'K', '1'}},//
    {   2, 0x4703, 0x0000, {' ', 'N', 'R', 'K', '2'}},//
    {   3, 0x4702, 0x0000, {' ', ' ', 'T', 'V', '2'}},//
    {   4, 0x4704, 0x0000, {'N', 'O', 'R', 'G', 'E'}},//TV Norge
    {   0, 0x4720, 0x0000, {'D', 'I', 'S', 'C', 'O'}},//Discovery Nordic
#endif
    {0xff, 0xffff, 0xffff, {'-', '-', '-', '-', '-'}}, //END_OF_TABLE
};
// 6 items

CNI_NETWORK_TABLE code Ats_Ranking_List_Poland[] =
{
#if _ENABLE_CNI_TABLE
    {   0, 0x4831, 0x0000, {'A', '.', 'P', 'L', 'A'}},//Animal Planet
    {   0, 0x4830, 0x0000, {'D', 'I', 'S', 'C', 'O'}},//Discovery Poland
    {   3, 0x4810, 0x0000, {'T', 'V', 'P', 'O', 'L'}},//TV Polonia
    {   1, 0x4801, 0x0000, {' ', 'T', 'V', 'P', '1'}},//
    {   2, 0x4802, 0x0000, {' ', 'T', 'V', 'P', '2'}},//
    {   1, 0x4880, 0x0000, {'T', 'V', 'P', 'W', 'A'}},//TVP Warszawa
    {   1, 0x4881, 0x0000, {'T', 'V', 'P', 'B', 'I'}},//TVP Bialystok
    {   1, 0x4882, 0x0000, {'T', 'V', 'P', 'B', 'Y'}},//TVP Bydgoszcz
    {   1, 0x4883, 0x0000, {'T', 'V', 'P', 'G', 'D'}},//TVP Gdansk
    {   1, 0x4884, 0x0000, {'T', 'V', 'P', 'K', 'A'}},//TVP Katowice
    {   1, 0x4886, 0x0000, {'T', 'V', 'P', 'A', 'R'}},//TVP Krakow
    {   1, 0x4887, 0x0000, {'T', 'V', 'P', 'L', 'U'}},//TVP Lublin
    {   1, 0x4888, 0x0000, {'T', 'V', 'P', 'L', 'O'}},//TVP Lodz
    {   1, 0x4890, 0x0000, {'T', 'V', 'P', 'R', 'Z'}},//TVP Rzeszow
    {   1, 0x4891, 0x0000, {'T', 'V', 'P', 'P', 'O'}},//TVP Poznan
    {   1, 0x4892, 0x0000, {'T', 'V', 'P', 'S', 'Z'}},//TVP Szczecin
    {   1, 0x4893, 0x0000, {'T', 'V', 'P', 'W', 'R'}},//TVP Wroclaw
    {   0, 0x4820, 0x0000, {' ', ' ', 'T', 'V', 'N'}},//TVN
    {   0, 0x4821, 0x0000, {'T', 'V', 'N', 'S', 'I'}},//TVN Siedem
    {   0, 0x4822, 0x0000, {'T', 'V', 'N', '2', '4'}},//TVN24
#endif
    {0xff, 0xffff, 0xffff, {'-', '-', '-', '-', '-'}}, //END_OF_TABLE
};
// 21 items

CNI_NETWORK_TABLE code Ats_Ranking_List_Portugal[] =
{
#if _ENABLE_CNI_TABLE
    {   1, 0x3510, 0x0000, {' ', 'R', 'T', 'P', '1'}},//
    {   2, 0x3511, 0x0000, {' ', 'R', 'T', 'P', '2'}},//
    {   5, 0x3512, 0x0000, {'R', 'T', 'P', 'A', 'F'}},//
    {   6, 0x3514, 0x0000, {'R', 'T', 'P', 'A', 'Z'}},//
    {   3, 0x3513, 0x0000, {' ', 'R', 'T', 'P', 'I'}},//
    {   4, 0x3515, 0x0000, {' ', 'R', 'T', 'P', 'M'}},//
#endif
    {0xff, 0xffff, 0xffff, {'-', '-', '-', '-', '-'}}, //END_OF_TABLE
};
// 7 items

CNI_NETWORK_TABLE code Ats_Ranking_List_Sanmarino[] =
{
#if _ENABLE_CNI_TABLE
    {   1, 0x3781, 0x0000, {' ', ' ', 'R', 'T', 'V'}},//
#endif
    {0xff, 0xffff, 0xffff, {'-', '-', '-', '-', '-'}}, //END_OF_TABLE
};
// 2 items


CNI_NETWORK_TABLE code Ats_Ranking_List_Slovakia[] =
{
#if _ENABLE_CNI_TABLE
    {   1, 0x42A1, 0x35a1, {' ', 'S', 'T', 'V', '1'}},//STV1
    {   1, 0x42A5, 0x35a5, {' ', 'S', 'T', 'V', '1'}},//STV1 Regional, B. Bystrica
    {   1, 0x42A3, 0x35a3, {' ', 'S', 'T', 'V', '1'}},//STV1 Regional, Kosice
    {   2, 0x42a2, 0x35a2, {' ', 'S', 'T', 'V', '2'}},//STV2
    {   2, 0x42a6, 0x35a6, {' ', 'S', 'T', 'V', '2'}},//STV2 Regional, B. Bystrica
    {   2, 0x42a4, 0x35a4, {' ', 'S', 'T', 'V', '2'}},//STV2 Regional Kosice
    {   0, 0x42B1, 0x35b1, {'T', 'V', 'J', 'O', 'J'}},//TV JOJ
#endif
    {0xff, 0xffff, 0xffff, {'-', '-', '-', '-', '-'}}, //END_OF_TABLE
};
// 8 items


CNI_NETWORK_TABLE code Ats_Ranking_List_Slovenia[] =
{
#if _ENABLE_CNI_TABLE
    {   0, 0xaae5, 0x0000, {'P', 'O', 'P', 'T', 'V'}},//POP TV
    {   0, 0xAAE6, 0x0000, {'K', 'A', 'N', 'A', 'L'}},//KANAL A
    {   0, 0xAAE3, 0x0000, {' ', ' ', ' ', 'K', 'C'}},//KC
    {   0, 0xAAE1, 0x0000, {' ', 'S', 'L', 'O', '1'}},//SLO1
    {   0, 0xAAE2, 0x0000, {' ', 'S', 'L', 'O', '2'}},//SLO2
    {   0, 0xAAF1, 0x0000, {' ', 'S', 'L', 'O', '3'}},//SLO3
    {   0, 0xAAE4, 0x0000, {' ', ' ', 'T', 'L', 'M'}},//TLM
#endif
    {0xff, 0xffff, 0xffff, {'-', '-', '-', '-' ,'-'}}, //END_OF_TABLE
};
// 8 items



// Spain (PDC)
CNI_NETWORK_TABLE code Ats_Ranking_List_Spain[] =
{
#if _ENABLE_CNI_TABLE
    {   0, 0x340a, 0x0000, {' ', 'A', 'R', 'T', 'E'}},//
    {   4, 0xca33, 0x0000, {' ', ' ', 'C', '3', '3'}},//
    {  10, 0xba01, 0x0000, {' ', 'E', 'T', 'B', '1'}},//
    {  11, 0x3402, 0x0000, {' ', 'E', 'T', 'B', '2'}},//
    {   3, 0xca03, 0x0000, {' ', ' ', 'T', 'V', '3'}},//
    {   1, 0x3e00, 0x0000, {' ', 'T', 'V', 'E', '1'}},//
    {   2, 0xe100, 0x0000, {' ', 'T', 'V', 'E', '2'}},//
    {   0, 0xe200, 0x0000, {'T', 'V', 'E', 'I', 'E'}},//TVE Internacional Europa
    {   0, 0x3403, 0x0000, {'C', 'A', 'N', '9', ' '}},//CANAL 9
    {   0, 0x3404, 0x0000, {'P', 'U', 'N', 'T', '2'}},//PUNT 2
    {   0, 0x3405, 0x0000, {' ', ' ', 'C', 'C', 'V'}},//CCV
    {   0, 0x3406, 0x0000, {'C', 'A', 'N', ' ', '9'}},//CANAL 9 NEWS 24H Future use
    {   0, 0x3407, 0x0000, {'C', 'A', 'N', ' ', '9'}},//CANAL 9 Future Use
    {   0, 0x3408, 0x0000, {'C', 'A', 'N', ' ', '9'}},//CANAL 9 DVB Future Use
    {   0, 0x3409, 0x0000, {'C', 'A', 'N', ' ', '9'}},//CANAL 9 DVB Future Use
    {   0, 0x340B, 0x0000, {'C', 'A', 'N', ' ', '9'}},//CANAL 9 DVB Future Use
    {   0, 0x340C, 0x0000, {'C', 'A', 'N', ' ', '9'}},//CANAL 9 DVB Future Use
    {   0, 0x340D, 0x0000, {'C', 'A', 'N', ' ', '9'}},//CANAL 9 DVB Future Use
    {   0, 0x340E, 0x0000, {'C', 'A', 'N', ' ', '9'}},//CANAL 9 DVB Future Use
    {   0, 0x340F, 0x0000, {'C', 'A', 'N', ' ', '9'}},//CANAL 9 DVB Future Use
    {   0, 0x3410, 0x0000, {'C', 'A', 'N', ' ', '9'}},//CANAL 9 DVB Future Use
    {   0, 0x3411, 0x0000, {'C', 'A', 'N', ' ', '9'}},//CANAL 9 DVB Future Use
    {   0, 0x3412, 0x0000, {'C', 'A', 'N', ' ', '9'}},//CANAL 9 DVB Future Use
    {   0, 0x3413, 0x0000, {'C', 'A', 'N', ' ', '9'}},//CANAL 9 DVB Future Use
    {   0, 0x3414, 0x0000, {'C', 'A', 'N', ' ', '9'}},//CANAL 9 DVB Future Use
    {   0, 0x3420, 0x3E20, {'T', 'M', 'A', 'D', 'R'}},//Telemadrid
    {   0, 0x3421, 0x3e21, {' ', 'O', 'T', 'R', 'A'}},//La Otra
    {   0, 0x3422, 0x3e22, {'T', 'M', 'S', 'A', 'T'}},//TM SAT
    {   0, 0x3423, 0x3e23, {'S', 'E', 'X', 'T', 'A'}},//La sexta
    {   0, 0x3424, 0x3e24, {'A', 'N', 'T', ' ', '3'}},//Antena 3
    {   0, 0x3425, 0x3e25, {' ', 'N', 'E', 'O', 'X'}},//Neox
    {   0, 0x3426, 0x3e26, {' ', 'N', 'O', 'V', 'A'}},//Nova
    {   0, 0x3427, 0x3e27, {'C', 'U', 'A', 'T', 'R'}},//Cuatro
    {   0, 0x3428, 0x3e28, {' ', 'C', 'N', 'N', '+'}},//CNN+
    {   0, 0x3429, 0x3e29, {'4', '0', 'L', 'A', 'T'}},//40 Latino
    {   0, 0x342a, 0x3e2a, {'2', '4', 'H', 'O', 'R'}},//24 Horas
    {   0, 0x342b, 0x3e2b, {'C', ' ', 'T', 'V', 'E'}},//Clan TVE
    {   0, 0x342c, 0x3e2c, {'T', ' ', 'D', 'E', 'P'}},//Teledeporte
    {   0, 0xe500, 0x1fe5, {'T', 'E', 'L', 'E', '5'}},//Tele5
#endif
    {0xff, 0xffff, 0xffff, {'-', '-', '-', '-', '-'}}, //END_OF_TABLE
};
// 40 items


CNI_NETWORK_TABLE code Ats_Ranking_List_Sweden[] =
{
#if _ENABLE_CNI_TABLE
    {   1, 0x4601, 0x0000, {' ', 'S', 'V', 'T', '1'}},//
    {   2, 0x4602, 0x0000, {' ', 'S', 'V', 'T', '2'}},//
    {   0, 0x4603, 0x0000, {' ', ' ', 'S', 'V', 'T'}},//SVT future
    {   0, 0x4604, 0x0000, {' ', ' ', 'S', 'V', 'T'}},//SVT future
    {   0, 0x4605, 0x0000, {' ', ' ', 'S', 'V', 'T'}},//SVT future
    {   0, 0x4606, 0x0000, {' ', ' ', 'S', 'V', 'T'}},//SVT future
    {   0, 0x4607, 0x0000, {' ', ' ', 'S', 'V', 'T'}},//SVT future
    {   0, 0x4608, 0x0000, {' ', ' ', 'S', 'V', 'T'}},//SVT future
    {   0, 0x4609, 0x0000, {' ', ' ', 'S', 'V', 'T'}},//SVT future
    {   0, 0x460a, 0x0000, {' ', ' ', 'S', 'V', 'T'}},//SVT future
    {   0, 0x460b, 0x0000, {' ', ' ', 'S', 'V', 'T'}},//SVT future
    {   0, 0x460c, 0x0000, {' ', ' ', 'S', 'V', 'T'}},//SVT future
    {   0, 0x460d, 0x0000, {' ', ' ', 'S', 'V', 'T'}},//SVT future
    {   0, 0x460e, 0x0000, {' ', ' ', 'S', 'V', 'T'}},//SVT future
    {   0, 0x460f, 0x0000, {' ', ' ', 'S', 'V', 'T'}},//SVT future
    {   0, 0x4600, 0x0000, {' ', 'T', 'E', 'S', 'T'}},//SVT Test Txmns
    {   4, 0x4640, 0x0000, {' ', ' ', 'T', 'V', '4'}},//
    {   4, 0x4641, 0x0000, {' ', ' ', 'T', 'V', '4'}},//TV 4 future
    {   4, 0x4642, 0x0000, {' ', ' ', 'T', 'V', '4'}},//TV 4 future
    {   4, 0x4643, 0x0000, {' ', ' ', 'T', 'V', '4'}},//TV 4 future
    {   4, 0x4644, 0x0000, {' ', ' ', 'T', 'V', '4'}},//TV 4 future
    {   4, 0x4645, 0x0000, {' ', ' ', 'T', 'V', '4'}},//TV 4 future
    {   4, 0x4646, 0x0000, {' ', ' ', 'T', 'V', '4'}},//TV 4 future
    {   4, 0x4647, 0x0000, {' ', ' ', 'T', 'V', '4'}},//TV 4 future
    {   4, 0x4648, 0x0000, {' ', ' ', 'T', 'V', '4'}},//TV 4 future
    {   4, 0x4649, 0x0000, {' ', ' ', 'T', 'V', '4'}},//TV 4 future
    {   4, 0x464a, 0x0000, {' ', ' ', 'T', 'V', '4'}},//TV 4 future
    {   4, 0x464b, 0x0000, {' ', ' ', 'T', 'V', '4'}},//TV 4 future
    {   4, 0x464c, 0x0000, {' ', ' ', 'T', 'V', '4'}},//TV 4 future
    {   4, 0x464d, 0x0000, {' ', ' ', 'T', 'V', '4'}},//TV 4 future
    {   4, 0x464e, 0x0000, {' ', ' ', 'T', 'V', '4'}},//TV 4 future
    {   4, 0x464f, 0x0000, {' ', ' ', 'T', 'V', '4'}},//TV 4 future
#endif
    {0xff, 0xffff, 0xffff, {'-', '-', '-', '-', '-'}}, //END_OF_TABLE
};
// 33 items

CNI_NETWORK_TABLE code Ats_Ranking_List_Switzerland[] =
{
#if _ENABLE_CNI_TABLE
    {   1, 0x4101, 0x24c1, {' ', 'S', 'F', ' ', '1'}},//SF 1
    {   2, 0x4107, 0x24c7, {' ', 'S', 'F', ' ', '2'}},//
    {   0, 0x410a, 0x24ca, {' ', ' ', 'S', 'A', 'T'}},//SAT ACCESS
//ASIG Change {   0, 0x4103, 0x24c3, {'T', 'S', 'I', ' ', '1'}},//
    {   0, 0x4103, 0xf4c3, {'T', 'S', 'I', ' ', '1'}},//ASIG add
//ASIG Change  {   0, 0x4109, 0x24c9, {'T', 'S', 'I', ' ', '2'}},//
    {   0, 0x0000, 0xf4c9, {'T', 'S', 'I', ' ', '2'}},//
    {   0, 0x4102, 0x24c2, {'T', 'S', 'R', ' ', '1'}},//
    {   0, 0x4108, 0x24c8, {'T', 'S', 'R', ' ', '2'}},//
    {   0, 0x4121, 0x2421, {' ', ' ', ' ', 'U', '1'}},//U1
    {   0, 0x4122, 0X2422, {'T', 'E', 'L', 'E', 'Z'}},//TeleZuri
// Switzerland (VPS)
    {   1, 0x0000, 0x04c1, {' ', ' ', 'S', 'F', '1'}},//SRG
    {   0, 0x0000, 0x04c2, {' ', 'T', 'S', 'R', '1'}},//SSR
    {   0, 0x0000, 0x04c3, {' ', 'T', 'S', 'I', '1'}},//SSR
    {   2, 0x0000, 0x04c7, {' ', ' ', 'S', 'F', '2'}},//SRG
    {   0, 0x0000, 0x04c8, {' ', 'T', 'S', 'R', '2'}},//SSR
    {   0, 0x0000, 0x04c9, {' ', 'T', 'S', 'I', '2'}},//SSR
    {   0, 0x0000, 0x04ca, {'S', '-', 'S', 'A', 'T'}},//SRG
    {   0, 0x0000, 0x04cc, {' ', ' ', 'S', 'F', 'I'}},//SRG SSR Schweizer Fernsehen Infokanal, SFi
    {   0, 0x0000, 0x0481, {'T', 'E', 'L', 'E', 'Z'}},//TeleZuri
    {   0, 0x0000, 0x0482, {'T', 'C', 'L', 'U', 'B'}},//Teleclub Abonnements-Fernsehen
    {   0, 0x0000, 0x0483, {'Z', 'U', 'R', 'I', '1'}},//ZUERI 1
    {   0, 0x0000, 0x0484, {'T', 'E', 'L', 'E', 'B'}},//TeleBern
    {   0, 0x0000, 0x0485, {' ', 'T', '.', 'M', '1'}},//Tele M1
    {   0, 0x0000, 0x0486, {' ', 'S', 'T', 'A', 'R'}},//Star TV
    {   0, 0x0000, 0x0487, {'P', 'R', 'O', ' ', '7'}},//Pro 7
    {   0, 0x0000, 0x0488, {'T', 'O', 'P', 'T', 'V'}},//TopTV
    {   0, 0x0000, 0x0489, {'T', 'E', 'L', '2', '4'}},//Reserved Tele 24
    {   0, 0x0000, 0x048a, {'K', 'A', 'B', 'E', '1'}},//Kabel 1
//ASIG change{   0, 0x0000, 0x048d, {'S', 'W', 'I', 'Z', 'Z'}},//Swizz Music Television
    {   0, 0x048d, 0x048d, {' ', 'V', 'I', 'V', 'A'}},//Swizz's VIVA Television,ASIG add
//ASIG change in llieler{   0, 0xa101, 0x04af, {' ', ' ', 'I', 'T', 'V'}},//ANGLIA TV,ASIG add
    {   0, 0xa101, 0x0000, {'K', 'I', 'E', 'T', 'V'}},//KIEL TV,ASIG add
    {   0, 0x0000, 0x048e, {'I', 'N', 'T', 'R', 'O'}},//Intro TV
    {   0, 0x0000, 0x048f, {' ', 'T', 'E', 'L', 'L'}},//Tele Tell
    {   0, 0x0000, 0x0490, {' ', ' ', 'T', 'O', 'P'}},//Tele Top
    {   0, 0x0000, 0x0491, {'T', 'S', 'O', 'C', 'H'}},//TV Sudostschweiz
    {   0, 0x0000, 0x0492, {' ', ' ', 'T', 'V', 'O'}},//TV Ostshweiz
    {   0, 0x0000, 0x0493, {'T', 'E', 'L', 'T', 'I'}},//Tele ticino
    {   0, 0x0000, 0x0494, {' ', ' ', 'S', 'H', 'F'}},//Schaffhauser Fernsehen
    {   0, 0x0000, 0x0495, {' ', ' ', ' ', 'U', '1'}},//U1 TV
    {   0, 0x0000, 0x0496, {' ', ' ', 'M', 'T', 'V'}},//MTV Swiss
#endif
    {0xff, 0xffff, 0xffff, {'-', '-', '-', '-', '-'}}, //END_OF_TABLE
};
// 38 items


CNI_NETWORK_TABLE code Ats_Ranking_List_Turkey[] =
{
#if _ENABLE_CNI_TABLE
    {   0, 0x900a, 0x0000, {' ', ' ', 'A', 'T', 'V'}},//ATV
    {   0, 0x9006, 0x0000, {'A', 'V', 'R', 'A', 'S'}},//AVRASYA
    {   0, 0x900e, 0x0000, {'B', 'R', 'A', 'V', 'O'}},//BRAVO TV
    {   0, 0x9008, 0x0000, {'C', 'I', 'N', 'E', '5'}},//Cine 5
    {   0, 0x900d, 0x0000, {' ', ' ', 'E', 'K', 'O'}},//EKO TV
    {   0, 0x900c, 0x0000, {'E', 'U', 'R', 'O', 'D'}},//EURO D
    {   0, 0x9010, 0x0000, {' ', ' ', 'F', 'U', 'N'}},//FUN TV
    {   0, 0x900f, 0x0000, {'G', 'A', 'L', 'A', 'K'}},//GALAKSI TV
    {   0, 0x900b, 0x0000, {'K', 'A', 'N', '.', 'D'}},//KANAL D
    {   0, 0x9012, 0x0000, {'K', 'A', 'N', '.', 'D'}},//KANAL D future
    {   0, 0x9013, 0x0000, {'K', 'A', 'N', '.', 'D'}},//KANAL D future
    {   0, 0x9007, 0x0000, {' ', 'S', 'H', 'O', 'W'}},//Show TV
    {   0, 0x9015, 0x0000, {' ', 'S', 'H', 'O', 'W'}},//Show TV future use
    {   0, 0x9016, 0x0000, {' ', 'S', 'H', 'O', 'W'}},//Show TV future use
    {   0, 0x9017, 0x0000, {' ', 'E', 'U', 'R', 'O'}},//Show Euro
    {   0, 0x9020, 0x0000, {' ', 'S', 'T', 'A', 'R'}},//STAR TV
    {   0, 0x9021, 0x0000, {'S', '.', 'M', 'A', 'X'}},//STARMAX
    {   0, 0x9022, 0x0000, {'K', 'A', 'N', ' ', '6'}},//KANAL 6
    {   0, 0x9023, 0x0000, {'S', 'T', 'A', 'R', '4'}},//STAR 4
    {   0, 0x9024, 0x0000, {'S', 'T', 'A', 'R', '5'}},//STAR 5
    {   0, 0x9025, 0x0000, {'S', 'T', 'A', 'R', '6'}},//STAR 6
    {   0, 0x9026, 0x0000, {'S', 'T', 'A', 'R', '7'}},//STAR 7
    {   0, 0x9027, 0x0000, {'S', 'T', 'A', 'R', '8'}},//STAR 8
    {   0, 0X9028, 0X0000, {' ', 'S', 'T', 'A', 'R'}},//STAR TV future use
    {   0, 0X9029, 0X0000, {' ', 'S', 'T', 'A', 'R'}},//STAR TV future use
    {   0, 0X9030, 0X0000, {' ', 'S', 'T', 'A', 'R'}},//STAR TV future use
    {   0, 0X9031, 0X0000, {' ', 'S', 'T', 'A', 'R'}},//STAR TV future use
    {   0, 0X9032, 0X0000, {' ', 'S', 'T', 'A', 'R'}},//STAR TV future use
    {   0, 0X9033, 0X0000, {' ', 'S', 'T', 'A', 'R'}},//STAR TV future use
    {   0, 0X9034, 0X0000, {' ', 'S', 'T', 'A', 'R'}},//STAR TV future use
    {   0, 0X9035, 0X0000, {' ', 'S', 'T', 'A', 'R'}},//STAR TV future use
    {   0, 0X9036, 0X0000, {' ', 'S', 'T', 'A', 'R'}},//STAR TV future use
    {   0, 0X9037, 0X0000, {' ', 'S', 'T', 'A', 'R'}},//STAR TV future use
    {   0, 0X9038, 0X0000, {' ', 'S', 'T', 'A', 'R'}},//STAR TV future use
    {   0, 0X9039, 0X0000, {' ', 'S', 'T', 'A', 'R'}},//STAR TV future use
    {   0, 0x9009, 0x0000, {'S', 'P', 'O', 'R', 'T'}},//Super Sport
    {   0, 0x9011, 0x0000, {'T', 'E', 'M', 'P', 'O'}},//TEMPO TV
    {   0, 0x9014, 0x0000, {' ', 'T', 'G', 'R', 'T'}},//
    {   1, 0x9001, 0x0000, {'T', 'R', 'T', '-', '1'}},//
    {   2, 0x9002, 0x0000, {'T', 'R', 'T', '-', '2'}},//
    {   3, 0x9003, 0x0000, {'T', 'R', 'T', '-', '3'}},//
    {   4, 0x9004, 0x0000, {'T', 'R', 'T', '-', '4'}},//
    {   0, 0x9005, 0x0000, {'T', 'R', 'T', '-', 'I'}},//TRT-INT
#endif
    {0xff, 0xffff, 0xffff, {'-', '-', '-', '-', '-'}}, //END_OF_TABLE
};
// 45 items


CNI_NETWORK_TABLE code Ats_Ranking_List_UK[] =
{
#if _ENABLE_CNI_TABLE
    {   3, 0xfb9c, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//ANGLIA TV
    {   3, 0xfb9d, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//ANGLIA TV future use
    {   3, 0xfb9e, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//ANGLIA TV future use
    {   3, 0xfb9f, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//ANGLIA TV future use
    {   6, 0x4469, 0x0000, {' ', 'N', 'E', 'W', 'S'}},//BBC News 24
    {   0, 0x4468, 0x0000, {'P', 'R', 'I', 'M', 'E'}},//BBC Prime
    {   0, 0x4457, 0x0000, {'W', 'O', 'R', 'L', 'D'}},//BBC World
    {   0, 0x4458, 0x0000, {'B', 'B', 'C', 'W', 'W'}},//BBC Worldwide fut.01
    {   0, 0x4459, 0x0000, {'B', 'B', 'C', 'W', 'W'}},//BBC Worldwide fut.02
    {   0, 0x445a, 0x0000, {'B', 'B', 'C', 'W', 'W'}},//BBC Worldwide fut.03
    {   0, 0x445b, 0x0000, {'B', 'B', 'C', 'W', 'W'}},//BBC Worldwide fut.04
    {   0, 0x445c, 0x0000, {'B', 'B', 'C', 'W', 'W'}},//BBC Worldwide fut.05
    {   0, 0x445d, 0x0000, {'B', 'B', 'C', 'W', 'W'}},//BBC Worldwide fut.06
    {   0, 0x445e, 0x0000, {'B', 'B', 'C', 'W', 'W'}},//BBC Worldwide fut.07
    {   0, 0x445f, 0x0000, {'B', 'B', 'C', 'W', 'W'}},//BBC Worldwide fut.08
    {   0, 0x4460, 0x0000, {'B', 'B', 'C', 'W', 'W'}},//BBC Worldwide fut.09
    {   0, 0x4461, 0x0000, {'B', 'B', 'C', 'W', 'W'}},//BBC Worldwide fut.10
    {   0, 0x4462, 0x0000, {'B', 'B', 'C', 'W', 'W'}},//BBC Worldwide fut.11
    {   0, 0x4463, 0x0000, {'B', 'B', 'C', 'W', 'W'}},//BBC Worldwide fut.12
    {   0, 0x4464, 0x0000, {'B', 'B', 'C', 'W', 'W'}},//BBC Worldwide fut.13
    {   0, 0x4465, 0x0000, {'B', 'B', 'C', 'W', 'W'}},//BBC Worldwide fut.14
    {   0, 0x4466, 0x0000, {'B', 'B', 'C', 'W', 'W'}},//BBC Worldwide fut.15
    {   0, 0x4467, 0x0000, {'B', 'B', 'C', 'W', 'W'}},//BBC Worldwide fut.16
    {   1, 0x447f, 0x0000, {' ', 'B', 'B', 'C', '1'}},//BBC1
    {   1, 0x4443, 0x0000, {' ', 'B', 'B', 'C', '1'}},//BBC1 future 01
    {   1, 0x4445, 0x0000, {' ', 'B', 'B', 'C', '1'}},//BBC1 future 02
    {   1, 0x4479, 0x0000, {' ', 'B', 'B', 'C', '1'}},//BBC1 future 03
    {   1, 0x4447, 0x0000, {' ', 'B', 'B', 'C', '1'}},//BBC1 future 04
    {   1, 0x4477, 0x0000, {' ', 'B', 'B', 'C', '1'}},//BBC1 future 05
    {   1, 0x4449, 0x0000, {' ', 'B', 'B', 'C', '1'}},//BBC1 future 06
    {   1, 0x4475, 0x0000, {' ', 'B', 'B', 'C', '1'}},//BBC1 future 07
    {   1, 0x444b, 0x0000, {' ', 'B', 'B', 'C', '1'}},//BBC1 future 08
    {   1, 0x4473, 0x0000, {' ', 'B', 'B', 'C', '1'}},//BBC1 future 09
    {   1, 0x444d, 0x0000, {' ', 'B', 'B', 'C', '1'}},//BBC1 future 10
    {   1, 0x4471, 0x0000, {' ', 'B', 'B', 'C', '1'}},//BBC1 future 11
    {   1, 0x444f, 0x0000, {' ', 'B', 'B', 'C', '1'}},//BBC1 future 12
    {   1, 0x446f, 0x0000, {' ', 'B', 'B', 'C', '1'}},//BBC1 future 13
    {   1, 0x4451, 0x0000, {' ', 'B', 'B', 'C', '1'}},//BBC1 future 14
    {   1, 0x446d, 0x0000, {' ', 'B', 'B', 'C', '1'}},//BBC1 future 15
    {   1, 0x4453, 0x0000, {' ', 'B', 'B', 'C', '1'}},//BBC1 future 16
    {   1, 0x446b, 0x0000, {' ', 'B', 'B', 'C', '1'}},//BBC1 future 17
    {   1, 0x4455, 0x0000, {' ', 'B', 'B', 'C', '1'}},//BBC1 future 18
    {   1, 0x4441, 0x0000, {' ', 'B', 'B', 'C', '1'}},//BBC1 NI
    {   1, 0x447b, 0x0000, {' ', 'B', 'B', 'C', '1'}},//BBC1 Scotland
    {   1, 0x447d, 0x0000, {' ', 'B', 'B', 'C', '1'}},//BBC1 Wales
//{   1, 0xfa6f, 0x0000, {' ', 'B', 'B', 'C', '1'}},//BBC1              //sam
    {   1, 0xfa6f, 0x2c2f, {' ', 'B', 'B', 'C', '1'}},//BBC1,ASIG Add
//ASIG change {   2, 0x4440, 0x0000, {' ', 'B', 'B', 'C', '2'}},//BBC2
    {   2, 0x4440, 0x2c40, {' ', 'B', 'B', 'C', '2'}},//BBC2,ASIG add
    {   2, 0x447c, 0x0000, {' ', 'B', 'B', 'C', '2'}},//BBC2 future 01
    {   2, 0x447a, 0x0000, {' ', 'B', 'B', 'C', '2'}},//BBC2 future 02
    {   2, 0x4446, 0x0000, {' ', 'B', 'B', 'C', '2'}},//BBC2 future 03
    {   2, 0x4478, 0x0000, {' ', 'B', 'B', 'C', '2'}},//BBC2 future 04
    {   2, 0x4448, 0x0000, {' ', 'B', 'B', 'C', '2'}},//BBC2 future 05
    {   2, 0x4476, 0x0000, {' ', 'B', 'B', 'C', '2'}},//BBC2 future 06
    {   2, 0x444a, 0x0000, {' ', 'B', 'B', 'C', '2'}},//BBC2 future 07
    {   2, 0x4474, 0x0000, {' ', 'B', 'B', 'C', '2'}},//BBC2 future 08
    {   2, 0x444c, 0x0000, {' ', 'B', 'B', 'C', '2'}},//BBC2 future 09
    {   2, 0x4472, 0x0000, {' ', 'B', 'B', 'C', '2'}},//BBC2 future 10
    {   2, 0x444e, 0x0000, {' ', 'B', 'B', 'C', '2'}},//BBC2 future 11
    {   2, 0x4470, 0x0000, {' ', 'B', 'B', 'C', '2'}},//BBC2 future 12
    {   2, 0x4450, 0x0000, {' ', 'B', 'B', 'C', '2'}},//BBC2 future 13
    {   2, 0x446e, 0x0000, {' ', 'B', 'B', 'C', '2'}},//BBC2 future 14
    {   2, 0x4452, 0x0000, {' ', 'B', 'B', 'C', '2'}},//BBC2 future 15
    {   2, 0x446c, 0x0000, {' ', 'B', 'B', 'C', '2'}},//BBC2 future 16
    {   2, 0x4454, 0x0000, {' ', 'B', 'B', 'C', '2'}},//BBC2 future 17
    {   2, 0x446a, 0x0000, {' ', 'B', 'B', 'C', '2'}},//BBC2 future 18
    {   2, 0x4456, 0x0000, {' ', 'B', 'B', 'C', '2'}},//BBC2 future 19
    {   2, 0x447e, 0x0000, {' ', 'B', 'B', 'C', '2'}},//BBC2 NI
    {   2, 0x4444, 0x0000, {' ', 'B', 'B', 'C', '2'}},//BBC2 Scotland
    {   2, 0x4442, 0x0000, {' ', 'B', 'B', 'C', '2'}},//BBC2 Wales
    {   2, 0xa2fe, 0x0000, {' ', 'B', 'B', 'C', '2'}},//BBC2                  // sam
    {   3, 0xb7f7, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//BORDER TV
    {   0, 0x4405, 0x0000, {'B', 'R', 'A', 'V', 'O'}},//BRAVO
    {   0, 0x82e2, 0x0000, {' ', 'S', 'E', 'L', '.'}},//CARLTON SEL. Fut.
    {   0, 0x82e1, 0x0000, {' ', 'S', 'E', 'L', '.'}},//CARLTON SELECT
    {   3, 0x82dd, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//CARLTON TV
    {   3, 0x82de, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//CARLTON TV future
    {   3, 0x82df, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//CARLTON TV future
    {   3, 0x82e0, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//CARLTON TV future
    {   3, 0x2f27, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//CENTRAL TV
    {   3, 0x5699, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//CENTRAL TV future
//ASIG change {   4, 0xfcd1, 0x0000, {' ', ' ', 'C', 'H', '4'}},//CHANNEL 4
    {   4, 0xfcd1, 0x2c11, {' ', ' ', 'C', 'H', '4'}},//CHANNEL 4
    {   5, 0x9602, 0x0000, {' ', ' ', 'C', 'H', '5'}},//CHANNEL5(1)
    {   5, 0x1609, 0x0000, {' ', ' ', 'C', 'H', '5'}},//CHANNEL5(2)
    {   5, 0x28eb, 0x0000, {' ', ' ', 'C', 'H', '5'}},//CHANNEL5(3)
    {   5, 0xc47b, 0x0000, {' ', ' ', 'C', 'H', '5'}},//CHANNEL5(4)
    {   3, 0xfce4, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//CHANNEL TV
    {   0, 0x4404, 0x0000, {' ', ' ', 'T', 'C', 'C'}},//CHILDREN's CHAN.
    {   0, 0x01f2, 0x0000, {' ', ' ', 'C', 'N', 'N'}},//
    {   0, 0x4407, 0x0000, {'D', 'I', 'S', 'C', 'V'}},//DISCOVERY
    {   0, 0x4420, 0x0000, {'D', 'I', 'S', 'C', 'V'}},//Discovery Home&Leisure
    {   0, 0x4421, 0x0000, {'A', 'N', 'I', 'M', 'A'}},//Animal Planet
    {   0, 0x44d1, 0x0000, {'D', 'I', 'S', 'N', 'Y'}},//DISNEY CHANNEL
    {   0, 0x4408, 0x0000, {' ', ' ', 'T', 'F', 'C'}},//FAMILY CHANNEL
    {   0, 0xc4f4, 0x42f4, {'F', 'I', 'L', 'M', '4'}},//FilmFour
    {   3, 0xaddc, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//GMTV
    {   3, 0xaddd, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//GMTV future use
    {   3, 0xadde, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//GMTV future use
    {   3, 0xaddf, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//GMTV future use
    {   3, 0xade0, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//GMTV future use
    {   3, 0xade1, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//GMTV future use
    {   3, 0xf33a, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//GRAMPIAN TV
    {   0, 0x4d5a, 0x0000, {' ', 'P', 'L', 'U', 'S'}},//GRANADA
    {   0, 0x4d5b, 0x0000, {' ', 'T', 'I', 'M', 'E'}},//GRANADA Timeshare
    {   3, 0xadd8, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//GRANADA TV
    {   3, 0xadd9, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//GRANADA TV future
    {   0, 0xfcf4, 0x0000, {'H', 'I', 'S', 'T', '.'}},//HISTORY Ch.
    {   3, 0x5aaf, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//HTV
    {   3, 0xf258, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//HTV future use
    {   3, 0xc8de, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//ITV NETWORK
    {   0, 0x4406, 0x0000, {'L', 'E', 'A', 'R', 'N'}},//LEARNING Ch.
    {   0, 0x4409, 0x0000, {' ', 'L', 'I', 'V', 'E'}},//Live TV
    {   3, 0x884b, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//LWT
    {   3, 0x884c, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//LWT future use
    {   3, 0x884d, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//LWT future use
    {   3, 0x884f, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//LWT future use
    {   3, 0x8850, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//LWT future use
    {   3, 0x8851, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//LWT future use
    {   3, 0x8852, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//LWT future use
    {   3, 0x8853, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//LWT future use
    {   3, 0x8854, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//LWT future use
    {   3, 0x10e4, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//MERIDAN
    {   3, 0xdd50, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//MERIDAN future use
    {   3, 0xdd51, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//MERIDAN future use
    {   3, 0xdd52, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//MERIDAN future use
    {   3, 0xdd53, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//MERIDAN future use
    {   3, 0xdd54, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//MERIDAN future use
    {   3, 0xdd55, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//MERIDAN future use
    {   0, 0xfcfb, 0x0000, {'M', 'O', 'V', 'I', 'E'}},//MOVIE CHANNEL
    {   0, 0x4d54, 0x0000, {' ', ' ', 'M', 'T', 'V'}},//MTV
    {   0, 0x4d55, 0x0000, {' ', ' ', 'M', 'T', 'V'}},//MTV future use
    {   0, 0x4d56, 0x0000, {' ', ' ', 'M', 'T', 'V'}},//MTV future use
    {   0, 0x320B, 0x0000, {' ', ' ', 'N', 'G', 'C'}},//National Geographic Channel
    {   0, 0x8e71, 0x0000, {' ', ' ', 'N', 'B', 'C'}},//NBC Europe
    {   0, 0x5343, 0x0000, {' ', ' ', 'N', 'B', 'C'}},//NBC Europe future use
    {   0, 0x8e72, 0x0000, {' ', ' ', 'N', 'B', 'C'}},//NBC Europe future use
    {   0, 0x8e73, 0x0000, {' ', ' ', 'N', 'B', 'C'}},//NBC Europe future use
    {   0, 0x8e74, 0x0000, {' ', ' ', 'N', 'B', 'C'}},//NBC Europe future use
    {   0, 0x8e75, 0x0000, {' ', ' ', 'N', 'B', 'C'}},//NBC Europe future use
    {   0, 0x8e76, 0x0000, {' ', ' ', 'N', 'B', 'C'}},//NBC Europe future use
    {   0, 0x8e77, 0x0000, {' ', ' ', 'N', 'B', 'C'}},//NBC Europe future use
    {   0, 0x8e78, 0x0000, {' ', ' ', 'N', 'B', 'C'}},//NBC Europe future use
    {   0, 0x8e79, 0x0000, {' ', ' ', 'N', 'B', 'C'}},//NBC Europe future use
    {   0, 0xa460, 0x0000, {'N', 'I', 'C', 'K', '.'}},//Nickelodeon UK
    {   0, 0xa465, 0x0000, {'P', 'A', 'R', 'A', 'M'}},//Paramount Comedy Ch
    {   0, 0x5c33, 0x0000, {' ', ' ', 'Q', 'V', 'C'}},//QVC future use
    {   0, 0x5c34, 0x0000, {' ', ' ', 'Q', 'V', 'C'}},//QVC future use
    {   0, 0x5c39, 0x0000, {' ', ' ', 'Q', 'V', 'C'}},//QVC future use
    {   0, 0x5c44, 0x0000, {' ', ' ', 'Q', 'V', 'C'}},//QVC UK
    {   0, 0xfcf3, 0x0000, {' ', 'R', 'A', 'C', 'E'}},//RACING Ch.
    {   4, 0xb4c7, 0x0000, {' ', ' ', 'S', '4', 'C'}},//
    {   0, 0xfcf5, 0x0000, {'S', 'C', 'I', 'F', 'I'}},//SCI CHANNEL
    {   3, 0xf9d2, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//SCOTTISH TV
    {   0, 0xfcf9, 0x0000, {' ', 'G', 'O', 'L', 'D'}},//SKY GOLD
    {   0, 0xfcfc, 0x0000, {' ', 'P', 'L', 'U', 'S'}},//SKY MOVIES PLUS
    {   6, 0xfcfd, 0x0000, {' ', 'N', 'E', 'W', 'S'}},//SKY NEWS
    {   0, 0xfcfe, 0x0000, {' ', 'S', 'K', 'Y', '1'}},//SKY ONE
    {   0, 0xfcf7, 0x0000, {'S', 'O', 'A', 'P', 'S'}},//SKY SOAPS
    {   0, 0xfcfa, 0x0000, {'S', 'P', 'O', 'R', 'T'}},//SKY SPORTS
    {   0, 0xfcf8, 0x0000, {'S', 'P', 'R', 'T', '2'}},//SKY SPORTS 2
    {   0, 0xfcf6, 0x0000, {'T', 'R', 'A', 'V', '.'}},//SKY TRAVEL
    {   0, 0xfcff, 0x0000, {' ', 'S', 'K', 'Y', '2'}},//SKY TWO
    {   0, 0x37e5, 0x0000, {' ', 'S', 'S', 'V', 'C'}},//
    {   0, 0x44c1, 0x0000, {' ', ' ', 'T', 'N', 'T'}},//
    {   3, 0xa82c, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//TYNE TEES TV
    {   3, 0xa82d, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//TYNE TEES TV future
    {   3, 0xa82e, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//TYNE TEES TV future
    {   0, 0x4401, 0x0000, {' ', 'G', 'O', 'L', 'D'}},//UK GOLD
    {   0, 0x4411, 0x0000, {' ', 'G', 'O', 'L', 'D'}},//UK GOLD future use
    {   0, 0x4412, 0x0000, {' ', 'G', 'O', 'L', 'D'}},//UK GOLD future use
    {   0, 0x4413, 0x0000, {' ', 'G', 'O', 'L', 'D'}},//UK GOLD future use
    {   0, 0x4414, 0x0000, {' ', 'G', 'O', 'L', 'D'}},//UK GOLD future use
    {   0, 0x4415, 0x0000, {' ', 'G', 'O', 'L', 'D'}},//UK GOLD future use
    {   0, 0x4402, 0x0000, {'L', 'I', 'V', 'N', 'G'}},//UK LIVING
    {   3, 0x833b, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//ULSTER TV
    {   0, 0x4d58, 0x0000, {' ', 'V', 'H', '-', '1'}},//
    {   0, 0x4d59, 0x0000, {' ', 'V', 'H', '-', '1'}},//VH-1 (German lang.)
    {   0, 0x4d57, 0x2c22, {' ', 'V', 'H', '-', '1'}},//VH-1 future use
    {   3, 0x25d1, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//WESTCOUNTRY fut.
    {   3, 0x25d2, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//WESTCOUNTRY fut.
    {   3, 0x25d0, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//WESTCOUNTRY TV
    {   0, 0x4403, 0x0000, {' ', 'W', 'I', 'R', 'E'}},//WIRE TV
    {   3, 0xfa2c, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//YORKSHIRE TV
    {   3, 0xfa2d, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//YORKSHIRE TV fut.
    {   3, 0xfa2e, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//YORKSHIRE TV fut.
    {   3, 0xfa2f, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//YORKSHIRE TV fut.
    {   3, 0xfa30, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//YORKSHIRE TV fut.
    {   3, 0xfa31, 0x0000, {' ', ' ', 'I', 'T', 'V'}},//YORKSHIRE TV fut.
#endif
    {0xff, 0xffff, 0xffff, {'-', '-', '-', '-', '-'}}, //END_OF_TABLE
};
// 188 items

CNI_NETWORK_TABLE code Ats_Ranking_List_Ukraine[] =
{
#if _ENABLE_CNI_TABLE
    {   0, 0x7700, 0x07c0, {'1', '+', '1', ' ', ' '}}, // 1 + 1
    {   0, 0x7701, 0x0000, {' ', ' ', '1', '+', '1'}}, // 1+1 future use
    {   0, 0x7702, 0x0000, {' ', ' ', '1', '+', '1'}}, // 1+1 future use
    {   0, 0x7703, 0x0000, {' ', ' ', '1', '+', '1'}}, // 1+1 future use
    {   0, 0x7705, 0x07c5, {' ', ' ', ' ', 'M', '1'}}, // M1
    {   0, 0x7707, 0x0000, {' ', 'I', 'C', 'T', 'V'}}, // ICTV
    {   0, 0x7708, 0x07c8, {' ', 'N', 'O', 'V', 'Y'}}, // Novy Kanal

#endif
    {0xff, 0xffff, 0xffff, {'-', '-', '-', '-', '-'}}, //END_OF_TABLE
};
// 8 items


CNI_NETWORK_TABLE code Ats_Ranking_List_Other[] =
{
// Luxembourg
    {   0, 0x4000, 0x0000, {'R', 'T', 'L', ' ', ' '}},// RTL Tele Letzebuerg

    {0xff, 0xffff, 0xffff, {'-', '-', '-', '-', '-'}}, //END_OF_TABLE
};
// 2 items

CNI_GLOBAL_TABLE code Ats_Ranking_List_Table[] =
{
    {Ats_Ranking_List_Austria, sizeof(Ats_Ranking_List_Austria) / sizeof(CNI_NETWORK_TABLE) },	   //  0 PAL-BG(CCIR)
    {Ats_Ranking_List_Belgium, sizeof(Ats_Ranking_List_Belgium) / sizeof(CNI_NETWORK_TABLE) },    //  1 PAL-BN(CCIR)
    {Ats_Ranking_List_Croatia, sizeof(Ats_Ranking_List_Croatia) / sizeof(CNI_NETWORK_TABLE) },    //  2 PAL-BH克羅埃西亞共和國
    {Ats_Ranking_List_Czech,   sizeof(Ats_Ranking_List_Czech)   / sizeof(CNI_NETWORK_TABLE) },    //  3 SECAM/PAL-DK(OIRT)
    {Ats_Ranking_List_Denmark, sizeof(Ats_Ranking_List_Denmark) / sizeof(CNI_NETWORK_TABLE) },    //  4 PAL_BG(CCIR)
    {Ats_Ranking_List_Finland, sizeof(Ats_Ranking_List_Finland) / sizeof(CNI_NETWORK_TABLE) },    //  5 PAL-BG(CCIR)
    {Ats_Ranking_List_France,  sizeof(Ats_Ranking_List_France)  / sizeof(CNI_NETWORK_TABLE) },    //  6 SECAM-L(FRENCH)

    {Ats_Ranking_List_Germany, sizeof(Ats_Ranking_List_Germany) / sizeof(CNI_NETWORK_TABLE) },    //  7 PAL-BG(CCIR)
    //ATS_RANKING_LIST_GERMANY1,           //  7+1

    {Ats_Ranking_List_Greece,  sizeof(Ats_Ranking_List_Greece)  / sizeof(CNI_NETWORK_TABLE) },    //  8 SECAM-BH(CCIR)
    {Ats_Ranking_List_Hungary, sizeof(Ats_Ranking_List_Hungary) / sizeof(CNI_NETWORK_TABLE) },   //  9 SECAM/PAL-DK(OIRT)
    {Ats_Ranking_List_Iceland, sizeof(Ats_Ranking_List_Iceland) / sizeof(CNI_NETWORK_TABLE) },    // 10 PAL_BG(CCIR)
    {Ats_Ranking_List_Ireland, sizeof(Ats_Ranking_List_Ireland) / sizeof(CNI_NETWORK_TABLE) },    // 11 PAL-AI(IR)
    {Ats_Ranking_List_Italy,   sizeof(Ats_Ranking_List_Italy)   / sizeof(CNI_NETWORK_TABLE) },    // 12 PAL-BG(IT)
    {Ats_Ranking_List_Netherlands, sizeof(Ats_Ranking_List_Netherlands) / sizeof(CNI_NETWORK_TABLE) },       // 13 PAL-BG(CCIR)
    {Ats_Ranking_List_Norway,  sizeof(Ats_Ranking_List_Norway) / sizeof(CNI_NETWORK_TABLE) },     // 14 PAL-BG(CCIR)
    {Ats_Ranking_List_Poland,  sizeof(Ats_Ranking_List_Poland) / sizeof(CNI_NETWORK_TABLE) },           // 15 SECAM/PAL-DK(OIRT)
    {Ats_Ranking_List_Portugal, sizeof(Ats_Ranking_List_Portugal) / sizeof(CNI_NETWORK_TABLE) },          // 16 PAL-BG(CCIR*)
    {Ats_Ranking_List_Sanmarino, sizeof(Ats_Ranking_List_Sanmarino) / sizeof(CNI_NETWORK_TABLE) },         // 17 PAL-BG(IT),聖馬利諾(南歐國家)
    {Ats_Ranking_List_Spain,   sizeof(Ats_Ranking_List_Spain)  / sizeof(CNI_NETWORK_TABLE) },           // 18 PAL-BG(CCIR)
    {Ats_Ranking_List_Sweden,  sizeof(Ats_Ranking_List_Sweden) / sizeof(CNI_NETWORK_TABLE) },      // 19 PAL-BG(CCIR)

    {Ats_Ranking_List_Switzerland, sizeof(Ats_Ranking_List_Switzerland) / sizeof(CNI_NETWORK_TABLE) },       // 20 PAL-BG(CCIR)
    //ATS_RANKING_LIST_SWITZERLAND1,       // 20+1

    {Ats_Ranking_List_Turkey, sizeof(Ats_Ranking_List_Turkey) / sizeof(CNI_NETWORK_TABLE) },            // 21 PAL-BG(CCIR)
    {Ats_Ranking_List_UK,     sizeof(Ats_Ranking_List_UK) / sizeof(CNI_NETWORK_TABLE) },            // 22 PAL-AI(UK)
    {Ats_Ranking_List_Other,  sizeof(Ats_Ranking_List_Other) / sizeof(CNI_NETWORK_TABLE) },        // 23 Other...
};

void drv_CNI_Init(void)
{
    //======================================= For  vd  Slicer_1   TT  setting  start =============================================
    //       field test all line setup start
    rtd_outl(0x5a04, 0x33330000);	// line 7,8
    rtd_outl(0x5a08, 0x33333333);	// line 9,10,11,12
    rtd_outl(0x5a0c, 0x33333333);	// line 13,14,15,16
    rtd_outl(0x5a10, 0x33333333);	// line 17,18,19,20
    rtd_outl(0x5a14, 0x33333333); 	// line 21,22,23,24
    rtd_outl(0x5a18, 0xcc00);
    //       field test all line setup end

    rtd_outl(0x5800, 0x00000040);
//	rtd_outl(0x5a00, 0x004b2713);	// slicer level determined by
//	rtd_outl(0x5a30, 0x00004600);	// tt_vbi_noise_threshold : the lowest of adaptive slicer level when no tt signal
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
}

UINT8 CGetNetworkInfo(UINT8* pChnName)
{
    //#define SET_CNI_HIT(_x)			ucRetryCnt = (ucRetryCnt & 0x0f) | ((_x) << 4)
    //#define GET_CNI_HIT()			((ucRetryCnt&0xf0)>>4)
#define SET_CNI_HIT(_x)			cniHit = (_x)
#define GET_CNI_HIT()			cniHit
    //#define GET_RETRY_CNT()			ucRetryCnt //(ucRetryCnt&0x0f)
    //#define DEC_RETRY_CNT()			ucRetryCnt-- //ucRetryCnt = (ucRetryCnt & 0x0f) | ((ucRetryCnt&0x0f) - 1)

    UINT8 cniHit = 0;
//	UINT32 tmp32;
    UINT8 j;
    UINT8 ucRetryCnt;
    UINT16 F1_NI, F2_CNI,VPS_CNI;
//	UINT16 pF1_NI, pF2_CNI;
    UINT8 country_idx;
    UINT8 ucAtsRanking;
    UINT8 nameIdx;
    UINT8 country_sel;

    ucAtsRanking = 0;
    ucRetryCnt = 4;
    SET_CNI_HIT(0);

    drvif_PPR_GetCNI(&F1_NI, &F2_CNI,&VPS_CNI);


    RTD_Log(LOGGER_INFO, "F1 NI = %x\n", (UINT32)F1_NI);
    RTD_Log(LOGGER_INFO, "F2 CNI = %x\n", (UINT32)F2_CNI);
    RTD_Log(LOGGER_INFO, "VPS CNI = %x\n", (UINT32)VPS_CNI);

    // Search specified country table
    country_sel = GET_TV_COUNTRY();
    country_idx = country_sel;
    for (j=0; j< Ats_Ranking_List_Table[country_idx].count; j++)
    {
        if (( F1_NI != 0 && Ats_Ranking_List_Table[country_idx].pTable[j].format1_ni == F1_NI)
                || (F2_CNI != 0 && Ats_Ranking_List_Table[country_idx].pTable[j].format2_cni == F2_CNI)
                || (VPS_CNI != 0 && Ats_Ranking_List_Table[country_idx].pTable[j].format2_cni == VPS_CNI)
           )
        {
            ucAtsRanking = Ats_Ranking_List_Table[country_idx].pTable[j].ats_ranking;
            for (nameIdx=0; nameIdx<5; nameIdx++)
            {

                pChnName[nameIdx] = Ats_Ranking_List_Table[country_idx].pTable[j].name[nameIdx];
                pChnName[nameIdx]=pChnName[nameIdx]-0x20;
            }
            SET_CNI_HIT(1);
            break;
        }
    }

    if ( GET_CNI_HIT() != 1)
    {	// Search other country table
        for (country_idx = 0; country_idx < (sizeof(Ats_Ranking_List_Table) / sizeof(CNI_GLOBAL_TABLE)); country_idx++)
        {
            if ( country_idx != country_sel /* GET_TV_COUNTRY()*/)
            {
                for (j=0; j<Ats_Ranking_List_Table[country_idx].count; j++)
                {
                    if (( F1_NI != 0 && Ats_Ranking_List_Table[country_idx].pTable[j].format1_ni == F1_NI)
                            || (F2_CNI != 0 && Ats_Ranking_List_Table[country_idx].pTable[j].format2_cni == F2_CNI)
                       )
                    {
                        ucAtsRanking = Ats_Ranking_List_Table[country_idx].pTable[j].ats_ranking;
                        for (nameIdx=0; nameIdx<5; nameIdx++)
                            pChnName[nameIdx] = Ats_Ranking_List_Table[country_idx].pTable[j].name[nameIdx];
                        SET_CNI_HIT(2);
                        break;
                    }
                }
            }
        }
    }

    if ( GET_CNI_HIT() == 2)
        return 0;
    else if ( GET_CNI_HIT() == 1)
    {
        if ( ucAtsRanking == 0)
            ucAtsRanking = 0xfe;
        return ucAtsRanking;
    }

    for (j=0; j<5; j++)
        pChnName[j] = _MINUS_;

    return 0;
}
#endif

//--------------------------------------------------
// Global Variables
//--------------------------------------------------

