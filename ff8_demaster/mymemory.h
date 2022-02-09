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
   BGFILENAME1, BGFILENAME2, CHECKTEXREPAVAIL, _ASM_FIELDBGRETADDR3, DISABLETPAGELIMIT, _ASM_FIELDBGRETADDR2, DS_FREE, DS_TEXIMG,
   _BSPBACKADD1, _BSPBACKADD2, BATTLEJMPPATCH1, BATTLEJMPPATCH2, BTLMON0, BTLMON1, BTLMON2, BTLMON3, BTLMON4, BTLMON5, BTLMON6, BHP,
   _BHPBACKADD1, _BHPBACKADD2, NEWGLTEX_CHARA, BCPBACKADD1, BCPBACKADD2, BCPBACKADD3, CLTBACKADD1, CLTBACKADD2, UPLOADVRAM, IOFUNC1,
   IOFUNC2, IOFUNC3, IOFUNC4, IOFUNC5, IOFUNC6, FILTERPATCH1, FILTERPATCH2, FILTERPATCH3, FILTERPATCH4, WINDOWTITLE, NULLSUB_DEBUG
};

void InitTable();
//Forcing _stdcall for assembler PUSH based CALLS for EAX as return value and stack cleaned by function
unsigned int _stdcall GetAddress(const ADDRESS_NAME name);

#define FORCEGAME 3
//#define JAPANESE_PATCH 1 - see VS compiler settings

//doing enum and address hardcoding at once? any ideas? also no double declaration, hmm...
//Use Excel for auto-generating such lines- no one is going to type this by hand lol

const std::map<ADDRESS_NAME, std::vector<unsigned int>> PATCHADDRESS =
{
#if JAPANESE_PATCH==0
   //KEY-ENUM		//VANILLA, 1010, 1020, ??? - all are for english- for japanese this list should be eventually swapped to other map
	{ _ASM_FIELDBGRETADDR2 , std::vector<unsigned int>{ 0x0       , 0x1606540 , 0x1606e20 ,0x1606A20,} },
	{ _ASM_FIELDBGRETADDR3 , std::vector<unsigned int>{ 0x0       , 0x1591B75 , 0x1592185 ,0x1591EA5,} },
	{ _BHPBACKADD1         , std::vector<unsigned int>{ 0x0       , 0x157DA5D , 0x157decd ,0x157DFBD,} },
	{ _BHPBACKADD2         , std::vector<unsigned int>{ 0x0       , 0x157DD05 , 0x157e175 ,0x157E265,} },
	{ _BSPBACKADD1         , std::vector<unsigned int>{ 0x0       , 0x1573AFF , 0x1573f7f ,0x1573F4F,} },
	{ _BSPBACKADD2         , std::vector<unsigned int>{ 0x0       , 0x1573B54 , 0x1573fd4 ,0x1573FA4,} },
	{ BATTLEJMPPATCH1      , std::vector<unsigned int>{ 0x0       , 0x156CED4 , 0x156d354 ,0x156D324,} },
	{ BATTLEJMPPATCH2      , std::vector<unsigned int>{ 0x0       , 0x156D30B , 0x156d78b ,0x156D75B,} },
	{ BCPBACKADD1          , std::vector<unsigned int>{ 0x0       , 0x16066BD , 0x1606f9d ,0x1606B9D,} },
	{ BCPBACKADD2          , std::vector<unsigned int>{ 0x0       , 0x1604087 , 0x1604967 ,0x1604567,} },
	{ BCPBACKADD3          , std::vector<unsigned int>{ 0x0       , 0x1605040 , 0x1605920 ,0x1605520,} },
	{ BGFILENAME1          , std::vector<unsigned int>{ 0x0       , 0x1782140 , 0x178b168 ,0x1782160,} },
	{ BGFILENAME2          , std::vector<unsigned int>{ 0x0       , 0x189559C , 0x18955bc ,0x18955BC,} },
	{ BHP                  , std::vector<unsigned int>{ 0x0       , 0x16482A8 , 0x1648d78 ,0x1648938,} },
	{ BTLMON0              , std::vector<unsigned int>{ 0x0       , 0x15FF3D0 , 0x15ff9c0 ,0x15FF6F0,} },
	{ BTLMON1              , std::vector<unsigned int>{ 0x0       , 0x15FF3DA , 0x15ff9ca ,0x15FF6FA,} },
	{ BTLMON2              , std::vector<unsigned int>{ 0x0       , 0x15FF3F9 , 0x15ff9e9 ,0x15FF719,} },
	{ BTLMON3              , std::vector<unsigned int>{ 0x0       , 0x15FF400 , 0x15ff9f0 ,0x15FF720,} },
	{ BTLMON4              , std::vector<unsigned int>{ 0x0       , 0x1601E02 , 0x16026e2 ,0x16022E2,} },
	{ BTLMON5              , std::vector<unsigned int>{ 0x0       , 0x1601E06 , 0x16026e6 ,0x16022E6,} },
	{ BTLMON6              , std::vector<unsigned int>{ 0x0       , 0x1601CFE , 0x16025de ,0x16021DE,} },
	{ CHECKTEXREPAVAIL     , std::vector<unsigned int>{ 0x0       , 0x1782080 , 0x178b0a0 ,0x17820A0,} },
	{ CLTBACKADD1          , std::vector<unsigned int>{ 0x0       , 0x155CD05 , 0x155d0b5 ,0x155D1A5,} },
	{ CLTBACKADD2          , std::vector<unsigned int>{ 0x0       , 0x155CD7A , 0x155d12a ,0x155D21A,} },
	{ DISABLETPAGELIMIT    , std::vector<unsigned int>{ 0x0       , 0x1606595 , 0x1606e75 ,0x1606A75,} },
	{ DS_FREE              , std::vector<unsigned int>{ 0x0       , 0x166b2a8 , 0x166b2a8 ,0x166B2A8,} },
	{ DS_TEXIMG            , std::vector<unsigned int>{ 0x0       , 0x166b4a0 , 0x166b4b4 ,0x166B4AC,} },
	{ FCPBACKADD1          , std::vector<unsigned int>{ 0x0       , 0x16061CC , 0x1606aac ,0x16066AC,} },
	{ FCPBACKADD2          , std::vector<unsigned int>{ 0x0       , 0x160C4AD , 0x160cd8d ,0x160C94D,} },
	{ FIELDCHARENT1        , std::vector<unsigned int>{ 0x0       , 0x160C43A , 0x160cd1c ,0x160C8DC,} },
	{ FIELDCHARENT2        , std::vector<unsigned int>{ 0x0       , 0x160C467 , 0x160cd47 ,0x160C907,} },
	{ FILTERPATCH1         , std::vector<unsigned int>{ 0x0       , 0x15693EF , 0x156987f ,0x156985F,} },
	{ FILTERPATCH2         , std::vector<unsigned int>{ 0x0       , 0x1569409 , 0x1569899 ,0x1569879,} },
	{ FILTERPATCH3         , std::vector<unsigned int>{ 0x0       , 0x156A348 , 0x156a7d8 ,0x156A7B8,} },
	{ FILTERPATCH4         , std::vector<unsigned int>{ 0x0       , 0x156A359 , 0x156a7e9 ,0x156A7C9,} },
	{ IOFUNC1              , std::vector<unsigned int>{ 0x15D41EB , 0x364CC   , 0x364cc ,0x364CC,} },
	{ IOFUNC2              , std::vector<unsigned int>{ 0x15D4797 , 0x36B37   , 0x36B37   ,0x36B37,} },
	{ IOFUNC3              , std::vector<unsigned int>{ 0x15D47D8 , 0x36B78   , 0x36b78   ,0x36b78,} },
	{ IOFUNC4              , std::vector<unsigned int>{ 0x15D4947 , 0x36D37   , 0x36d37   ,0x36d37,} },
	{ IOFUNC5              , std::vector<unsigned int>{ 0x15D42B7 , 0x365F8   , 0x365f8   ,0x365F8,} },
	{ IOFUNC6              , std::vector<unsigned int>{ 0x15D42B7 , 0x3656A   , 0x3656a   ,0x3656A,} },
	{ NEWGLTEX_CHARA       , std::vector<unsigned int>{ 0x0       , 0x160b670 , 0x160bf50 ,0x160BB50,} },
	{ NULLSUB_DEBUG        , std::vector<unsigned int>{ 0x0       , 0x0       , 0x33DC0 ,0x33DC0,} },
	{ UPLOADVRAM           , std::vector<unsigned int>{ 0x0       , 0x1573A40 , 0x1573ec0 ,0x1573E90,} },
	{ UVFUNCPNTR           , std::vector<unsigned int>{ 0x8A2CB2  , 0x911A05  , 0x912725  ,0x9113B5,} },
	{ UVPATCH0             , std::vector<unsigned int>{ 0x8A2CB2  , 0x911A05  , 0x912725  ,0x9113B5,} },
	{ UVPATCH1             , std::vector<unsigned int>{ 0x8A2CB8  , 0x911A0B  , 0x91272b  ,0x9113bb,} },
	{ UVPATCH10            , std::vector<unsigned int>{ 0x8A2E6F  , 0x911BC6  , 0x9128e6  ,0x911576,} },
	{ UVPATCH11            , std::vector<unsigned int>{ 0x8A2E75  , 0x911BCC  , 0x9128ec  ,0x91157C,} },
	{ UVPATCH2             , std::vector<unsigned int>{ 0x8A2D17  , 0x911A67  , 0x912787  ,0x911417,} },
	{ UVPATCH3             , std::vector<unsigned int>{ 0x8A2D1D  , 0x911A6D  , 0x91278d  ,0x91141d,} },
	{ UVPATCH4             , std::vector<unsigned int>{ 0x8A2D6D  , 0x911AC4  , 0x9127e4  ,0x911474,} },
	{ UVPATCH5             , std::vector<unsigned int>{ 0x8A2D73  , 0x911ACA  , 0x9127ea  ,0x91147A,} },
	{ UVPATCH6             , std::vector<unsigned int>{ 0x8A2DC5  , 0x911B23  , 0x912843  ,0x9114D3,} },
	{ UVPATCH7             , std::vector<unsigned int>{ 0x8A2DCB  , 0x911B29  , 0x912849  ,0x9114D9,} },
	{ UVPATCH8             , std::vector<unsigned int>{ 0x8A2E1B  , 0x911B80  , 0x9128a0  ,0x911530,} },
	{ UVPATCH9             , std::vector<unsigned int>{ 0x8A2E21  , 0x911B86  , 0x9128a6  ,0x911536,} },
	{ WINDOWTITLE          , std::vector<unsigned int>{ 0x0       , 0x1601065 , 0x1601935 ,0x1601535,} },
	{ WORLDGETTEXIDX0      , std::vector<unsigned int>{ 0x0       , 0x17424B0 , 0x17424b0 ,0x17424B0,} },
	{ WORLDGETTEXIDX1      , std::vector<unsigned int>{ 0x0       , 0x17424B4 , 0x17424b4 ,0x17424B4,} },
	{ WORLDGETTEXIDX2      , std::vector<unsigned int>{ 0x0       , 0x17424B8 , 0x17424b8 ,0x17424B8,} },
	{ WORLDGETTEXIDX3      , std::vector<unsigned int>{ 0x0       , 0x17424BC , 0x17424bc ,0x17424Bc,} },

#else
{IOFUNC1              , std::vector<unsigned int>{ 0x0 , 0x0 , 0x3649C   }},
{IOFUNC6              , std::vector<unsigned int>{ 0x0 , 0x0 , 0x3653A   }},
{IOFUNC5              , std::vector<unsigned int>{ 0x0 , 0x0 , 0x365C8   }},
{IOFUNC2              , std::vector<unsigned int>{ 0x0 , 0x0 , 0x36B07   }},
{IOFUNC3              , std::vector<unsigned int>{ 0x0 , 0x0 , 0x36B48   }},
{IOFUNC4              , std::vector<unsigned int>{ 0x0 , 0x0 , 0x36D07   }},
{UVPATCH0             , std::vector<unsigned int>{ 0x0 , 0x0 , 0x9452CC  }},
{UVFUNCPNTR           , std::vector<unsigned int>{ 0x0 , 0x0 , 0x9452CC  }},
{UVPATCH1             , std::vector<unsigned int>{ 0x0 , 0x0 , 0x9452D2  }},
{UVPATCH2             , std::vector<unsigned int>{ 0x0 , 0x0 , 0x94532E  }},
{UVPATCH3             , std::vector<unsigned int>{ 0x0 , 0x0 , 0x945334  }},
{UVPATCH4             , std::vector<unsigned int>{ 0x0 , 0x0 , 0x94538D  }},
{UVPATCH5             , std::vector<unsigned int>{ 0x0 , 0x0 , 0x945393  }},
{UVPATCH6             , std::vector<unsigned int>{ 0x0 , 0x0 , 0x9453EC  }},
{UVPATCH7             , std::vector<unsigned int>{ 0x0 , 0x0 , 0x9453F2  }},
{UVPATCH8             , std::vector<unsigned int>{ 0x0 , 0x0 , 0x94544B  }},
{UVPATCH9             , std::vector<unsigned int>{ 0x0 , 0x0 , 0x945451  }},
{UVPATCH10            , std::vector<unsigned int>{ 0x0 , 0x0 , 0x945491  }},
{UVPATCH11            , std::vector<unsigned int>{ 0x0 , 0x0 , 0x945497  }},
{CLTBACKADD1          , std::vector<unsigned int>{ 0x0 , 0x0 , 0x1593985 }},
{CLTBACKADD2          , std::vector<unsigned int>{ 0x0 , 0x0 , 0x15939FA }},
{FILTERPATCH1         , std::vector<unsigned int>{ 0x0 , 0x0 , 0x159FFBF }},
{FILTERPATCH2         , std::vector<unsigned int>{ 0x0 , 0x0 , 0x159FFD9 }},
{FILTERPATCH3         , std::vector<unsigned int>{ 0x0 , 0x0 , 0x15A0EE6 }},
{FILTERPATCH4         , std::vector<unsigned int>{ 0x0 , 0x0 , 0x15A0EF7 }},
{BATTLEJMPPATCH1      , std::vector<unsigned int>{ 0x0 , 0x0 , 0x15A3F14 }},
{BATTLEJMPPATCH2      , std::vector<unsigned int>{ 0x0 , 0x0 , 0x15A434B }},
{UPLOADVRAM           , std::vector<unsigned int>{ 0x0 , 0x0 , 0x15AAAB0 }},
{_BSPBACKADD1         , std::vector<unsigned int>{ 0x0 , 0x0 , 0x15AAB6F }},
{_BSPBACKADD2         , std::vector<unsigned int>{ 0x0 , 0x0 , 0x15AABC4 }},
{_BHPBACKADD1         , std::vector<unsigned int>{ 0x0 , 0x0 , 0x15B4C4D }},
{_BHPBACKADD2         , std::vector<unsigned int>{ 0x0 , 0x0 , 0x15B4EF5 }},
{_ASM_FIELDBGRETADDR3 , std::vector<unsigned int>{ 0x0 , 0x0 , 0x15C3CE5 }},
{BTLMON0              , std::vector<unsigned int>{ 0x0 , 0x0 , 0x1631700 }},
{BTLMON1              , std::vector<unsigned int>{ 0x0 , 0x0 , 0x163170A }},
{BTLMON2              , std::vector<unsigned int>{ 0x0 , 0x0 , 0x1631729 }},
{BTLMON3              , std::vector<unsigned int>{ 0x0 , 0x0 , 0x1631730 }},
{WINDOWTITLE          , std::vector<unsigned int>{ 0x0 , 0x0 , 0x1633405 }},
{BTLMON6              , std::vector<unsigned int>{ 0x0 , 0x0 , 0x16340AE }},
{BTLMON4              , std::vector<unsigned int>{ 0x0 , 0x0 , 0x16341B2 }},
{BTLMON5              , std::vector<unsigned int>{ 0x0 , 0x0 , 0x16341B6 }},
{BCPBACKADD2          , std::vector<unsigned int>{ 0x0 , 0x0 , 0x1636436 }},
{BCPBACKADD3          , std::vector<unsigned int>{ 0x0 , 0x0 , 0x16373F0 }},
{FCPBACKADD1          , std::vector<unsigned int>{ 0x0 , 0x0 , 0x16384EE }},
{_ASM_FIELDBGRETADDR2 , std::vector<unsigned int>{ 0x0 , 0x0 , 0x1638862 }},
{DISABLETPAGELIMIT    , std::vector<unsigned int>{ 0x0 , 0x0 , 0x16388B7 }},
{BCPBACKADD1          , std::vector<unsigned int>{ 0x0 , 0x0 , 0x16389DF }},
{NEWGLTEX_CHARA       , std::vector<unsigned int>{ 0x0 , 0x0 , 0x163DAC0 }},
{FIELDCHARENT1        , std::vector<unsigned int>{ 0x0 , 0x0 , 0x163E88C }},
{FIELDCHARENT2        , std::vector<unsigned int>{ 0x0 , 0x0 , 0x163E8B7 }},
{FCPBACKADD2          , std::vector<unsigned int>{ 0x0 , 0x0 , 0x163E8FD }},
{BHP                  , std::vector<unsigned int>{ 0x0 , 0x0 , 0x167A558 }},
{DS_FREE              , std::vector<unsigned int>{ 0x0 , 0x0 , 0x169D2A8 }},
{DS_TEXIMG            , std::vector<unsigned int>{ 0x0 , 0x0 , 0x169D4B4 }},
{WORLDGETTEXIDX0      , std::vector<unsigned int>{ 0x0 , 0x0 , 0x1753F70 }},
{WORLDGETTEXIDX1      , std::vector<unsigned int>{ 0x0 , 0x0 , 0x1753F74 }},
{WORLDGETTEXIDX2      , std::vector<unsigned int>{ 0x0 , 0x0 , 0x1753F78 }},
{WORLDGETTEXIDX3      , std::vector<unsigned int>{ 0x0 , 0x0 , 0x1753F7C }},
{CHECKTEXREPAVAIL     , std::vector<unsigned int>{ 0x0 , 0x0 , 0x1793BA8 }},
{BGFILENAME1          , std::vector<unsigned int>{ 0x0 , 0x0 , 0x1793C94 }},
{BGFILENAME2          , std::vector<unsigned int>{ 0x0 , 0x0 , 0x18A85A4 }},
{NULLSUB_DEBUG        , std::vector<unsigned int>{ 0x0 , 0x0 , 0x33E50   }},
#endif
};