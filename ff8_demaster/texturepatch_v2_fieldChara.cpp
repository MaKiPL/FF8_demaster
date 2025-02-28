#include <filesystem>

#include "coreHeader.h"
#include "texture.h"

int width_fcp=768,height_fcp=768;

#define SCSTR(x) x.str().c_str()

BYTE* fcpBackAdd1;
BYTE* fcpBackAdd2;
BYTE* fcpBackAdd3; //for blinking

inline bool hd_rectangle_texture = false;

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

	hd_rectangle_texture=false;

	if (!std::filesystem::exists(testPath))
		DDSorPNG(testPath, 256,"%s_new%s", SCSTR(texPath), SCSTR(tempSprint));
	else
		if (!std::filesystem::exists(testPath))
			strcat(testPath, std::string(texPath.str() + "_new\\d000.0.png").c_str()); //ERROR !!!!
	//we have full path, however I want to see if _1.XXX exists. So we have path in PNG or DDS.
	
	//take 5th character from the path and if it's 0, then make it 1
	bool isSingleRowAtlas = false;
	std::string testPathStr = testPath;
	if(testPathStr.ends_with("0.png") || testPathStr.ends_with("0.dds"))
	{
		testPathStr[testPathStr.length()-5] = '1';
		isSingleRowAtlas = !std::filesystem::exists(testPathStr);
	}

	//No need to load whole image- let's do it the GetImageResolutionFast way
	//const SafeBimg img = LoadImageFromFile(testPath);
	
	if(!std::filesystem::exists(testPath))
		return;

	Vector2Di textureResolution0 = GetImageResolutionFast(testPath);

	uint32_t finalHeight = textureResolution0.height*2;
	
	if(!isSingleRowAtlas)
	{
		const Vector2Di textureResolution1 = GetImageResolutionFast(testPathStr.c_str());
		if(textureResolution1.height>textureResolution0.height)
			finalHeight = textureResolution1.height*2;
	}

	height_fcp = static_cast<int>(finalHeight);

	hd_rectangle_texture = textureResolution0.height == textureResolution0.width;

	OutputDebug("_fcpObtainTextureDatas:: img:%dx%d, fcp:%dx%d, filename=%s\n", textureResolution0.width,
		textureResolution0.height, width_fcp, height_fcp, testPath);

	//If we take two textures, take their bigger height variant (d017_0 is 384 and 288), then bigger one * 2 is 768
	//in this case. Therefore there's no need to check the height if it's %384 and work on upscaling it to bigger number.
	//It should work out-of-box. d017_0 and d019_0 are the 288 textures. Even in upscale- 576 for d017_0, but 768 for
	//d017_1, so the 768*2=1536 is the final height no matter the first texture

	
	// if (finalHeight%384 != 0) //d017_0 and d019_0. The modulo is clever :D
	// 	height_fcp = 768;
	
	// uint32_t upperResolution = 384; //288, 576 <> 384,768,1536..
	// const uint32_t scaler = isSingleRowAtlas?1:2;
	// for(int idx=0; idx<5; idx++)
	// 			if(img->m_height<=upperResolution)
	// 			{
	// 				width_fcp = height_fcp = static_cast<int32_t>(upperResolution * scaler);
	// 				break;
	// 			}


	// OutputDebug("_fcpObtainTextureDatas:: img:%dx%d, fcp:%dx%d, scaler=%d, filename=%s\n", img->m_width,
	// 	img->m_height, width_fcp, height_fcp, scaler,texPath.str().c_str());
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
	//ESI - width
	//EDI - height
	__asm
	{
		CMP [TEX_TYPE], 57
		JNE originalcode
		CMP[EBP + 0x0C], 0 //this is width. Caution
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

__declspec(naked) void _fcpFieldCharBlink()
{
	__asm
	{
		//it comapres DWORD PTR [ebp-0x8c] to 0x300. We want it to be modulo 768 instead
		//I do not store and restore the registers. It doesn't matter but destroy the stack

		//we want all field_hd textures to be not blinking (there's no 0x300 texture in field_hd)
		CMP hd_rectangle_texture, 0
		MOV BL, 0
		JE _start
		JMP _out
		

		//let's start:
		_start:
		MOV EAX, [width_fcp] //EAX is now lower-half
		XOR EDX, EDX //as DIV works on EDX:EAX, we need to make sure it's empty
		MOV EBX, 0x300 //Yes, we divide the dynamic width by maximum vanilla atlas

		DIV EBX //EAX=quotient, EDX=remainder

		//If EDX contains remainder, then it's simple modulo of 768. If atlas is 768*N, then it's 0

		//check if EDX is 0
		TEST EDX, EDX //it saves the flags

		//although we restored all the values, the flags are still there. The original code was setting the BL. We do the same
		SETE BL //set BL if EDX is 0

		_out:
		JMP fcpBackAdd3
	}
}

void ApplyFieldEntityPatch()
{
	//step 1. obtain needed data for tex_struct and etc.
	//Maki: ouch, same-name vars
	fcpBackAdd1 = InjectJMP(IMAGE_BASE + GetAddress(FCPBACKADD1), reinterpret_cast<DWORD>(_fcpObtainData), 18);


	//step 2. disable out of bounds error- we know that, but we are using new, bigger buffers
	ModPage(IMAGE_BASE + GetAddress(FIELDCHARENT1), 1);
	*reinterpret_cast<BYTE*>(IMAGE_BASE + GetAddress(FIELDCHARENT1)) = 0xEB; //JBE -> JMP

	ModPage(IMAGE_BASE + GetAddress(FIELDCHARENT2), 1);
	*reinterpret_cast<BYTE*>(IMAGE_BASE + GetAddress(FIELDCHARENT2)) = 0xEB; //JBE -> JMP

	//1160545A - set
	fcpBackAdd2 = InjectJMP(IMAGE_BASE + GetAddress(FCPBACKADD2), reinterpret_cast<DWORD>(_fcpSetYoffset), 6);

	//0x1160594F - checks for tex height. If not 768, then it's not blinking. CMP + SETZ bl, thne mov al, bl
	//DISABLED- TODO, 3rd character is crashing when going into menu lol 
	fcpBackAdd3 = InjectJMP(IMAGE_BASE+GetAddress(FIELD_CHAR_BLINK),
		reinterpret_cast<DWORD>(_fcpFieldCharBlink), 13);
}