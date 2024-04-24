#include <filesystem>

#include "coreHeader.h"

int width_fcp=768,height_fcp=768;

#define SCSTR(x) x.str().c_str()

BYTE* fcpBackAdd1;
BYTE* fcpBackAdd2;

//casual is 384x384 or 768x768, therefore the final should be 1st height * 2
void _fcpObtainTextureDatas(const int rowIndex, const int objectIndex)
{
	std::ostringstream texPath;
	std::ostringstream tempSprint;
	texPath << DIRECT_IO_EXPORT_DIR << "textures\\field.fs\\field_hd";

	if (objectIndex >= 0xC19) //those ugly numbers come based on do*emu func (probably)
		tempSprint << "\\p" << std::setfill('0') << std::setw(3) <<  objectIndex - 3097 << "_" << rowIndex;
	else if (objectIndex < 0x831)
		if (objectIndex < 0x449)
			tempSprint << "\\d" << std::setfill('0') << std::setw(3) << objectIndex - 97 << "_" << rowIndex;
		else
			tempSprint << "\\c" << std::setfill('0') << std::setw(3) << objectIndex - 1097 << "_" << rowIndex;
		else
		tempSprint << "\\o" << std::setfill('0') << std::setw(3) << objectIndex - 2097 << "_" << rowIndex;



	char testPath[256]{0};
	DDSorPNG(testPath,256, "%s%s", SCSTR(texPath), SCSTR(tempSprint));

	if (!std::filesystem::exists(testPath))
		DDSorPNG(testPath, 256,"%s_new%s", SCSTR(texPath), SCSTR(tempSprint));
	else
		if (!std::filesystem::exists(testPath))
			strcat(testPath, std::string(texPath.str() + "_new\\d000.0.png").c_str()); //ERROR !!!!
	//we have full path, however I want to see if _1.XXX exists. So we have path in PNG or DDS.
	
	//take 5th character from the path and if it's 0, then make it 1
	bool isSingleRowAtlas = false; 
	if(std::string testPathStr = testPath; testPathStr.ends_with("0.png") || testPathStr.ends_with("0.dds"))
	{
		testPathStr[testPathStr.length()-5] = '1';
		isSingleRowAtlas = std::filesystem::exists(testPathStr);
	}
	
	const SafeBimg img = LoadImageFromFile(testPath);

	//the most important is height here
	if (!img)
		return;
	if (img->m_height == 288)
		height_fcp = 768;
	else height_fcp = img->m_height * 2;
	uint32_t upperResolution = 384; //288, 576 <> 384,768,1536..
	const uint32_t scaler = isSingleRowAtlas?1:2;
	for(int idx=0; idx<5; idx++)
				if(img->m_height<=upperResolution)
				{
					width_fcp = height_fcp = static_cast<int32_t>(upperResolution * scaler);
					break;
				}


	OutputDebug("_fcpObtainTextureDatas:: img:%dx%d, fcp:%dx%d, scaler=%d, filename=%s\n", img->m_width,
		img->m_height, width_fcp, height_fcp, scaler,texPath.str().c_str());
}

//this creates atlas
__declspec(naked) void _fcpObtainData()
{
	__asm
	{
		MOV EAX, dword ptr[EDI + 0x10] //yes, once again we get tex_struct + 0xd0 as edi+10h
		MOV tex_struct, EAX
		TEST EAX, EAX
		JZ _fcpSecondRow

		PUSH EAX
		MOV EAX, dword ptr[EDI+0x14]
		DEC EAX
		PUSH EAX
		JMP _fcpCallTexture

		_fcpSecondRow:
		MOV EAX, dword ptr[EDI+0x28]
		PUSH EAX
		MOV EAX, dword ptr [EDI+0x2C]
		DEC EAX
		PUSH EAX

		_fcpCallTexture:
		CALL _fcpObtainTextureDatas
		POP EAX
		POP EAX

		PUSH 0
		PUSH 0
		PUSH 0
		PUSH[height_fcp]
		PUSH[height_fcp]
		PUSH ECX // to restore later
		PUSH NEWGLTEX_CHARA 
		CALL GetAddress //_stdcall so stack so no need to clean stack
		MOV ECX, EAX //EAX as always should return return-value
		MOV EAX, OFFSET IMAGE_BASE
		MOV EAX, [EAX]
		ADD EAX, ECX //createGLTexture
		POP ECX //restore ECX from stack
		CALL EAX

		JMP fcpBackAdd1
	}
}

DWORD _fcpCurrentTexMode;

__declspec(naked) void _fcpSetYoffset()
{
	__asm
	{
		CMP [TEX_TYPE], 57
		JNE originalcode
		CMP[EBP + 0x0C], 0 //the hell is this? original code?
		JE originalcode
		MOV EAX, [height_fcp]
		SHR EAX, 1 //768/2
		PUSH EAX
		PUSH 0
		JMP returnhere

		originalcode :
		push[ebp + 0x0C]
		push[ebp + 0x08]
		returnhere:
		JMP fcpBackAdd2
	}
}

void ApplyFieldEntityPatch()
{
	//step 1. obtain needed data for tex_struct and etc.
	//Maki: ouch, same-name vars
	fcpBackAdd1 = InjectJMP(IMAGE_BASE + GetAddress(FCPBACKADD1), (DWORD)_fcpObtainData, 18);


	//step 2. disable out of bounds error- we know that, but we are using new, bigger buffers
	ModPage(IMAGE_BASE + GetAddress(FIELDCHARENT1), 1);
	*(BYTE*)(IMAGE_BASE + GetAddress(FIELDCHARENT1)) = 0xEB; //JBE -> JMP

	ModPage(IMAGE_BASE + GetAddress(FIELDCHARENT2), 1);
	*(BYTE*)(IMAGE_BASE + GetAddress(FIELDCHARENT2)) = 0xEB; //JBE -> JMP

	//1160545A - set
	fcpBackAdd2 = InjectJMP(IMAGE_BASE + GetAddress(FCPBACKADD2), (DWORD)_fcpSetYoffset, 6);
}