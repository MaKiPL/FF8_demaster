#pragma once
#include "dllmain.h"

inline int LANGUAGE_POINTER = -1;

enum ADDRESS_NAME
{
    UVPATCH0, UVPATCH1, UVPATCH2, UVPATCH3, UVPATCH4, UVPATCH5, UVPATCH6, UVPATCH7, UVPATCH8, UVPATCH9, UVPATCH10, UVPATCH11, UVPATCH12,
    UVFUNCPNTR, WORLDGETTEXIDX0, WORLDGETTEXIDX1, WORLDGETTEXIDX2, WORLDGETTEXIDX3, FCPBACKADD1, FIELDCHARENT1, FIELDCHARENT2, FCPBACKADD2,
    BGFILENAME1, BGFILENAME2, CHECKTEXREPAVAIL, _ASM_FIELDBGRETADDR3, DISABLETPAGELIMIT, _ASM_FIELDBGRETADDR2, DS_FREE, DS_TEXIMG,
    _BSPBACKADD1, _BSPBACKADD2, BATTLEJMPPATCH1, BATTLEJMPPATCH2, BTLMON0, BTLMON1, BTLMON2, BTLMON3, BTLMON4, BTLMON5, BTLMON6, BHP, BHP_ATLAS,
    _BHPBACKADD1, _BHPBACKADD2, _BHPBACKADD3, NEWGLTEX_CHARA, BCPBACKADD1, BCPBACKADD2, BCPBACKADD3, CLTBACKADD1, CLTBACKADD2, UPLOADVRAM, IOFUNC1,
    IOFUNC2, IOFUNC3, IOFUNC4, IOFUNC5, IOFUNC6, FILTERPATCH1, FILTERPATCH2, FILTERPATCH3, FILTERPATCH4, WINDOWTITLE, NULLSUB_DEBUG,
 BGRESPATCH1, GLFWINIT, GLFWWINDOWHINT, CHANGEMODE, STEAMADDR, FIELD_CHAR_BLINK, SUB_116059B0, LOAD_BATTLE_EXT
 };

const std::map<ADDRESS_NAME, std::vector<unsigned int>> PATCHADDRESS =
{
{_ASM_FIELDBGRETADDR2, std::vector<unsigned int>{0x1606A20,0x16382A2}},
{_ASM_FIELDBGRETADDR3, std::vector<unsigned int>{0x1591EA5,0x15C37E5}},
{_BHPBACKADD1, std::vector<unsigned int>{0x157DFBD,0x15B495D}},
{_BHPBACKADD2, std::vector<unsigned int>{0x157E265,0x15B4C05}},
{_BSPBACKADD1, std::vector<unsigned int>{0x1573F4F,0x15AA9EF}},
{_BSPBACKADD2, std::vector<unsigned int>{0x1573FA4,0x15AAA44}},
{BATTLEJMPPATCH1, std::vector<unsigned int>{0x156D324,0x15A3DB4}},
{BATTLEJMPPATCH2, std::vector<unsigned int>{0x156D75B,0x15A41EB}},
{BCPBACKADD1, std::vector<unsigned int>{0x1606B9D,0x163841F}},
{BCPBACKADD2, std::vector<unsigned int>{0x1604567,0x1635E77}},
{BCPBACKADD3, std::vector<unsigned int>{0x1605520,0x1636E30}},
{BGFILENAME1, std::vector<unsigned int>{0x1782160,0x1793CB4}},
{BGFILENAME2, std::vector<unsigned int>{0x18955BC,0x18A8418}},
{BGRESPATCH1, std::vector<unsigned int>{0x156E2C7,0x15A4D57}},
{BHP, std::vector<unsigned int>{0x1648938,0x1679F98}},
{BHP_ATLAS, std::vector<unsigned int>{0x15FF640,0x00000000}}, //??????
{BTLMON0, std::vector<unsigned int>{0x15FF6F0,0x1631230}},
{BTLMON1, std::vector<unsigned int>{0x15FF6FA,0x163123A}},
{BTLMON2, std::vector<unsigned int>{0x15FF719,0x1631259}},
{BTLMON3, std::vector<unsigned int>{0x15FF720,0x1631260}},
{BTLMON4, std::vector<unsigned int>{0x16022E2,0x1633BF2}},
{BTLMON5, std::vector<unsigned int>{0x16022E6,0x1633BF6}},
{BTLMON6, std::vector<unsigned int>{0x16021DE,0x1633AEE}},
{CHANGEMODE, std::vector<unsigned int>{0x1600EB0,0x16327C0}},
{CHECKTEXREPAVAIL, std::vector<unsigned int>{0x17820A0,0x1793BB0}},
{CLTBACKADD1, std::vector<unsigned int>{0x155D1A5,0x15938D5}},
{CLTBACKADD2, std::vector<unsigned int>{0x155D21A,0x159394A}},
{DISABLETPAGELIMIT, std::vector<unsigned int>{0x1606A75,0x16382F7}},
{DS_FREE, std::vector<unsigned int>{0x166B2A8,0x169D2A8}},
{DS_TEXIMG, std::vector<unsigned int>{0x166B4AC,0x169D4AC}},
{FCPBACKADD1, std::vector<unsigned int>{0x16066AC,0x1637F2E}},
{FCPBACKADD2, std::vector<unsigned int>{0x160C94D,0x163E33D}},
{FIELD_CHAR_BLINK, std::vector<unsigned int>{0x160594F,0x1637192}},
{FIELDCHARENT1, std::vector<unsigned int>{0x160C8DA,0x163E2CC}},
{FIELDCHARENT2, std::vector<unsigned int>{0x160C907,0x163E2F7}},
{FILTERPATCH1, std::vector<unsigned int>{0x156985F,0x159FE5F}},
{FILTERPATCH2, std::vector<unsigned int>{0x1569879,0x159FE79}},
{FILTERPATCH3, std::vector<unsigned int>{0x156A7B8,0x15A0D86}},
{FILTERPATCH4, std::vector<unsigned int>{0x156A7C9,0x15A0D97}},
{IOFUNC1, std::vector<unsigned int>{0x000364CC,0x3649C}}, //only save file names?
{IOFUNC2, std::vector<unsigned int>{0x00036B37,0x36B07}}, //open read 
{IOFUNC3, std::vector<unsigned int>{0x00036b78,0x36B48}}, //seek
{IOFUNC4, std::vector<unsigned int>{0x00036d37,0x36D07}}, //archive::file::seek
{IOFUNC5, std::vector<unsigned int>{0x000365F8,0x365C8}}, //sets filesize, but if I overtake weep::openfile? returns filesize?
    //so the archive* +0x10 is filesize
{IOFUNC6, std::vector<unsigned int>{0x0003656A,0x3653A}},
{LOAD_BATTLE_EXT, std::vector<unsigned int>{0x1604610,0x1635F20}},
{NEWGLTEX_CHARA, std::vector<unsigned int>{0x160BB50,0x163D500}},
{NULLSUB_DEBUG, std::vector<unsigned int>{0x33DC0,0x33E50}},
{STEAMADDR, std::vector<unsigned int>{0x16EED10,0x16FED48}},
{SUB_116059B0, std::vector<unsigned int>{0x16059B0,0x1638422}},
{UPLOADVRAM, std::vector<unsigned int>{0x1573E90,0x15AA930}},
{UVFUNCPNTR, std::vector<unsigned int>{0x9113B5,0x94A8FC}},
{UVPATCH0, std::vector<unsigned int>{0x9113B5,0x94A8FC}},
{UVPATCH1, std::vector<unsigned int>{0x9113bb,0x94A902}},
{UVPATCH10, std::vector<unsigned int>{0x911576,0x94AAC1}},
{UVPATCH11, std::vector<unsigned int>{0x91157C,0x94AAC7}},
{UVPATCH2, std::vector<unsigned int>{0x911417,0x94A95E}},
{UVPATCH3, std::vector<unsigned int>{0x91141d,0x94A964}},
{UVPATCH4, std::vector<unsigned int>{0x911474,0x94A9BD}},
{UVPATCH5, std::vector<unsigned int>{0x91147A,0x94A9C3}},
{UVPATCH6, std::vector<unsigned int>{0x9114D3,0x94AA1C}},
{UVPATCH7, std::vector<unsigned int>{0x9114D9,0x94AA22}},
{UVPATCH8, std::vector<unsigned int>{0x911530,0x94AA7B}},
{UVPATCH9, std::vector<unsigned int>{0x911536,0x94AA81}},
{WINDOWTITLE, std::vector<unsigned int>{0x1601535,0x1632E45}},
{WORLDGETTEXIDX0, std::vector<unsigned int>{0x17424B0,0x1753F70}},
{WORLDGETTEXIDX1, std::vector<unsigned int>{0x17424B4,0x1753F74}},
{WORLDGETTEXIDX2, std::vector<unsigned int>{0x17424B8,0x1753F78}},
{WORLDGETTEXIDX3, std::vector<unsigned int>{0x17424Bc,0x1753F7C}},
};

inline uint32_t GetAddress(const ADDRESS_NAME name)
{
    return IMAGEBASE + PATCHADDRESS.at(name)[LANGUAGE_POINTER];
}

inline void* GetAddressVoid(const ADDRESS_NAME name)
{
    return reinterpret_cast<void*>(GetAddress(name));
}
