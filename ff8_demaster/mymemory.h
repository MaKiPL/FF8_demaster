#pragma once

//See address table at:
//https://docs.google.com/spreadsheets/d/1hFGSoQNbamIrqmhBwr8B9RvD43KO140Zyb5eiWm3O_4/edit?usp=sharing

#include <iostream>
#include <vector>
#include <map>

enum ADDRESS_NAME
{
	UVPATCH0, UVPATCH1, UVPATCH2, UVPATCH3, UVPATCH4, UVPATCH5, UVPATCH6, UVPATCH7, UVPATCH8, UVPATCH9, UVPATCH10, UVPATCH11, UVPATCH12,
	UVFUNCPNTR, WORLDGETTEXIDX0, WORLDGETTEXIDX1, WORLDGETTEXIDX2, WORLDGETTEXIDX3, FCPBACKADD1, FIELDCHARENT1, FIELDCHARENT2, FCPBACKADD2, 
	BGFILENAME1, BGFILENAME2, CHECKTEXREPAVAIL, _ASM_FIELDBGRETADDR3, DISABLETPAGELIMIT, _ASM_FIELDBGRETADDR2, _BSP, DS_FREE, DS_TEXIMG, 
	_BSPBACKADD1, _BSPBACKADD2, BATTLEJMPPATCH1, BATTLEJMPPATCH2, BTLMON0, BTLMON1, BTLMON2, BTLMON3, BTLMON4, BTLMON5, BTLMON6, BHP, 
	_BHPBACKADD1, _BHPBACKADD2, NEWGLTEX_CHARA, BCPBACKADD1, BCPBACKADD2, BCPBACKADD3, CLTBACKADD1, CLTBACKADD2, UPLOADVRAM, IOFUNC1, 
	IOFUNC2, IOFUNC3, IOFUNC4, IOFUNC5, IOFUNC6, FILTERPATCH1, FILTERPATCH2, FILTERPATCH3, FILTERPATCH4, WINDOWTITLE
};

void InitTable();
//Forcing _stdcall for assembler PUSH based CALLS for EAX as return value and stack cleaned by function
unsigned int _stdcall GetAddress(const ADDRESS_NAME name);

#define FORCEGAME 1



//doing enum and address hardcoding at once? any ideas? also no double declaration, hmm...
//Use Excel for auto-generating such lines- no one is going to type this by hand lol
/*static*/const std::map<ADDRESS_NAME, std::vector<unsigned int>> PATCHADDRESS
{
	//KEY-ENUM		//VANILLA, 1010, 1020, ??? - all are for english- for japanese this list should be eventually swapped to other map
{UVPATCH0,            std::vector<unsigned int>{0x8A2CB2, 0x911A05, 0x912725}},
{UVPATCH1,            std::vector<unsigned int>{0x8A2CB8, 0x911A0B, 0x91272b}},
{UVPATCH2,            std::vector<unsigned int>{0x8A2D17, 0x911A67, 0x912787}},
{UVPATCH3,            std::vector<unsigned int>{0x8A2D1D, 0x911A6D, 0x91278d}},
{UVPATCH4,            std::vector<unsigned int>{0x8A2D6D, 0x911AC4, 0x0}},
{UVPATCH5,            std::vector<unsigned int>{0x8A2D73, 0x911ACA, 0x0}},
{UVPATCH6,            std::vector<unsigned int>{0x8A2DC5, 0x911B23, 0x0}},
{UVPATCH7,            std::vector<unsigned int>{0x8A2DCB, 0x911B29, 0x0}},
{UVPATCH8,            std::vector<unsigned int>{0x8A2E1B, 0x911B80, 0x0}},
{UVPATCH9,            std::vector<unsigned int>{0x8A2E21, 0x911B86, 0x0}},
{UVPATCH10,           std::vector<unsigned int>{0x8A2E6F, 0x911BC6, 0x0}},
{UVPATCH11,           std::vector<unsigned int>{0x8A2E75, 0x911BCC, 0x0}},
{UVFUNCPNTR,          std::vector<unsigned int>{0x8A2CB2, 0x911A05, 0x0}},
{WORLDGETTEXIDX0,     std::vector<unsigned int>{0x0,      0x17424B0,0x0}},
{WORLDGETTEXIDX1,     std::vector<unsigned int>{0x0,      0x17424B4,0x0}},
{WORLDGETTEXIDX2,     std::vector<unsigned int>{0x0,      0x17424B8,0x0}},
{WORLDGETTEXIDX3,     std::vector<unsigned int>{0x0,      0x17424BC,0x0}},
{FCPBACKADD1,         std::vector<unsigned int>{0x0,      0x16061CC,0x0}},
{FIELDCHARENT1,       std::vector<unsigned int>{0x0,      0x160C43A,0x0}},
{FIELDCHARENT2,       std::vector<unsigned int>{0x0,      0x160C467,0x0}},
{FCPBACKADD2,         std::vector<unsigned int>{0x0,      0x160C4AD,0x0}},
{BGFILENAME1,         std::vector<unsigned int>{0x0,      0x1782140,0x0}},
{BGFILENAME2,         std::vector<unsigned int>{0x0,      0x189559C,0x0}},
{CHECKTEXREPAVAIL,    std::vector<unsigned int>{0x0,      0x1782080,0x0}},
{_ASM_FIELDBGRETADDR3,std::vector<unsigned int>{0x0,      0x1591B75,0x0}},
{DISABLETPAGELIMIT,   std::vector<unsigned int>{0x0,      0x1606595,0x0}},
{_ASM_FIELDBGRETADDR2,std::vector<unsigned int>{0x0,      0x1606540,0x0}},
{_BSP,                std::vector<unsigned int>{0x0,      0x166B2A8,0x0}},
{DS_FREE,             std::vector<unsigned int>{0x0,      0x166b2a8,0x0}},
{DS_TEXIMG,           std::vector<unsigned int>{0x0,      0x166b4a0,0x0}},
{_BSPBACKADD1,        std::vector<unsigned int>{0x0,      0x1573AFF,0x0}},
{_BSPBACKADD2,        std::vector<unsigned int>{0x0,      0x1573B54,0x0}},
{BATTLEJMPPATCH1,     std::vector<unsigned int>{0x0,      0x156CED4,0x0}},
{BATTLEJMPPATCH2,     std::vector<unsigned int>{0x0,      0x156D30B,0x0}},
{BTLMON0,             std::vector<unsigned int>{0x0,      0x15FF3D0,0x0}},
{BTLMON1,             std::vector<unsigned int>{0x0,      0x15FF3DA,0x0}},
{BTLMON2,             std::vector<unsigned int>{0x0,      0x15FF3F9,0x0}},
{BTLMON3,             std::vector<unsigned int>{0x0,      0x15FF400,0x0}},
{BTLMON4,             std::vector<unsigned int>{0x0,      0x1601E02,0x0}},
{BTLMON5,             std::vector<unsigned int>{0x0,      0x1601E06,0x0}},
{BTLMON6,             std::vector<unsigned int>{0x0,      0x1601CFE,0x0}},
{BHP,                 std::vector<unsigned int>{0x0,      0x16482A8,0x0}},
{_BHPBACKADD1,        std::vector<unsigned int>{0x0,      0x157DA5D,0x0}},
{_BHPBACKADD2,        std::vector<unsigned int>{0x0,      0x157DD05,0x0}},
{NEWGLTEX_CHARA,      std::vector<unsigned int>{0x0,      0x160b670,0x0}},
{BCPBACKADD1,         std::vector<unsigned int>{0x0,      0x16066BD,0x0}},
{BCPBACKADD2,         std::vector<unsigned int>{0x0,      0x1604087,0x0}},
{BCPBACKADD3,         std::vector<unsigned int>{0x0,      0x1605040,0x0}},
{CLTBACKADD1,         std::vector<unsigned int>{0x0,      0x155CD05,0x0}},
{CLTBACKADD2,         std::vector<unsigned int>{0x0,      0x155CD7A,0x0}},
{CLTBACKADD2,         std::vector<unsigned int>{0x0,      0x1573A40,0x0}},
{IOFUNC1,             std::vector<unsigned int>{0x15D41EB,0x364CC,  0x0}},
{IOFUNC2,             std::vector<unsigned int>{0x15D4797,0x36B37,  0x0}},
{IOFUNC3,             std::vector<unsigned int>{0x15D47D8,0x36B78,  0x0}},
{IOFUNC4,             std::vector<unsigned int>{0x15D4947,0x36D37,  0x0}},
{IOFUNC5,             std::vector<unsigned int>{0x15D42B7,0x365F8,  0x0}},
{IOFUNC6,             std::vector<unsigned int>{0x15D42B7,0x3656A,  0x0}},
{FILTERPATCH1,        std::vector<unsigned int>{0x0,      0x15693EF,0x0}},
{FILTERPATCH2,        std::vector<unsigned int>{0x0,      0x1569409,0x0}},
{FILTERPATCH3,        std::vector<unsigned int>{0x0,      0x156A348,0x0}},
{FILTERPATCH4,        std::vector<unsigned int>{0x0,      0x156A359,0x0}},
{WINDOWTITLE,         std::vector<unsigned int>{0x0,      0x1601065,0x0}},
};