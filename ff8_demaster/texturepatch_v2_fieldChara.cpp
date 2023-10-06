#include <filesystem>

#include "coreHeader.h"

int width_fcp=768,height_fcp=768;

#define SCSTR(x) x.str().c_str()

BYTE* fcpBackAdd1;
BYTE* fcpBackAdd2GlTexSubImage;

//casual is 384x384 or 768x768, therefore the final should be 1st height * 2
//row index should be either 0 or 1. objectIndex is the character/object ID from the map
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
	// if (img->m_height == 288)
	// 	height_fcp = 768;
	// else height_fcp = img->m_height * 2;
	uint32_t upperResolution = 384; //288, 576 <> 384,768,1536..
	const uint32_t scaler = isSingleRowAtlas?1:2;
	for(int idx=0; idx<5; idx++)
		//so for 288- it's already lower than 384, so it gets height_fcp of 768
		//for modded it's 576 - it's higher than 384, therefore we test more
		// 576 is now less than 768, so we get 1536
		//for normal texture let's say 384, it's the same as 384, so we get 768
		//for 768, it's the same as 768, so we get 1536
		//for 1536 textures we get 3072, but that didn't happen
		//max is: [0]384, [1]768, [2]1536, [3]3072, [4]6144
	{
		//so for 288- it's already lower than 384, so it gets height_fcp of 768
		//for modded it's 576 - it's higher than 384, therefore we test more
		// 576 is now less than 768, so we get 1536
		//for normal texture let's say 384, it's the same as 384, so we get 768
		//for 768, it's the same as 768, so we get 1536
		//for 1536 textures we get 3072, but that didn't happen
		//max is: [0]384, [1]768, [2]1536, [3]3072, [4]6144
		if(img->m_height<=upperResolution)
		{
			width_fcp = height_fcp = static_cast<int32_t>(upperResolution * scaler);
			break;
		}
		upperResolution*=2;
	}

	OutputDebug("_fcpObtainTextureDatas:: img:%dx%d, fcp:%dx%d, scaler=%d, filename=%s\n", img->m_width,
		img->m_height, width_fcp, height_fcp, scaler,texPath.str().c_str());
}

//This creates the atlas
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
		PUSH[width_fcp]
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

void ReportXOffset(const DWORD x)
{
	OutputDebug("x offset: %d. Bound texture: %d\n", x, lastGlBoundTexture);
}


DWORD fcpH, fcpW, fcpX, fcpY, pixels;
/**
 * \brief Injected before call to glTexSubImage2D which creates other half of the atlas for field characters?
 * Need to push:
 * EBX = Texture pixels (EBP-0x110)
 * 0x1401 = GL_RGBA const
 * 0x1908 = GL_UNSIGNED_BYTE const
 * EDI = Texture Height
 * ESI = Texture Width
 * [EBP+0x0C] = Texture subImage Y offset
 * [EBP+0x08] = Texture subImage X offset
 * PUSH 0 = Texture subImage Level
 * PUSH 0xDE1 = GL_TEXTURE_2D const
 */
__declspec(naked) void _fcpSetYoffset()
{
	//fcp_width and fcp_height is resolution of final buffer to be created. Should be at least 2 times bigger
	//than original texture
	__asm
	{
		MOV fcpH, EDI //fcp height
		MOV fcpW, ESI //fcp width

#if DEBUG_LOG_VERBOSE
		MOV pixels, EBX //pixels
#endif // LOG_VERBOSE
		
		MOV EAX,[EBP+0x0C]
		MOV fcpY, EAX
		MOV EAX, [EBP+0x08]
		MOV fcpX, EAX
		
		
	}
#if DEBUG_LOG_VERBOSE
	OutputDebug("TEX_TYPE_PRE: %d Resolution: %dx%d offset: %dx%d. fcpRes: %dx%d. Pixels=%08X\n", TEX_TYPE,
	fcpW, fcpH, fcpX, fcpY, width_fcp, height_fcp, pixels);
#else
	OutputDebug("TEX_TYPE: %d Resolution: %dx%d offset: %dx%d. fcpRes: %dx%d\n", TEX_TYPE,
	fcpW, fcpH, fcpX, fcpY, width_fcp, height_fcp);
#endif
		
	__asm
	{
		PUSH EBX //Pixels
		PUSH 0x1401 //GL_RGBA
		PUSH 0x1908 //GL_UNSIGNED_BYTE
		PUSH fcpH //Tex Height
		PUSH fcpW //Tex Width

		//We need to check if the texture type is 57 (field character). If we reuse the code for any other texture
		//type like Battle Character, then the algorithm with getting the height from height_fcp will be wrong for
		//battle chara and produce glitches.
		//In field, the character is always 384x384 or 768x768. In rare cases individual textures have 284 in height
		//The buffer that is created is double the size of bigger texture (smaller is loaded second).
		//The bigger texture gets into first row of the atlas/buffer created and by using glTexSubImage2D
		//the second texture is drawn just after the first one. So if first is 384x384, then the buffer/atlas should be
		//384x768 (384*2). The second should be 384, and will be drawn at Yoffset = first texture height
		//for battle characters instead- the Yoffset is not patterned, therefore some texture can be bigger than the
		//other, and the second one should be after the first one. Weapons texture resolutions are loaded first AFAIR
		//then the bigger character texture. The calculation of weaponheight*2 is not enough for the buffer size. 
		CMP [TEX_TYPE], TEX_TYPE_FIELD_CHARACTER
		JE _fieldOffset //if TRUE, then goes to Yoffset of height_fcp/2 (will show always directly 50% of buffer height)
						//If the buffer is 288, then it's always upped to 384 and then multiplied=768. That way I'll
						//support Irvine modded and Irvine original texture fixing finally the famous prison crash 
		PUSH [EBP+0x0C] //Yoffset
		PUSH [EBP+0x08] //Xoffset
		JMP _epilogue

		_fieldOffset:
		MOV EAX, fcpY
		TEST EAX, EAX //if original Y offset is 0, then do nothing with it 
		JE _skipYoffset
		MOV EAX, [height_fcp] //if original Y offset is other than 0, like 384px for 768px texture, then
		SHR EAX, 1 //the height_fcp contains whole target buffer size, so we need to draw into the 
		_skipYoffset: //second half of the image. So 1536px height should be 1536/2=768 yOffset instead of 384
		PUSH EAX // yOffset
		PUSh 0
		
		
		_epilogue:
		PUSH 0 // Level
		PUSH 0xDE1 //GL_TEXTURE_2D const

		JMP fcpBackAdd2GlTexSubImage
	}
}

void ApplyFieldEntityPatch()
{
	//step 1. obtain needed data for tex_struct and etc.
	//Maki: ouch, same-name vars
	fcpBackAdd1 = InjectJMP(IMAGE_BASE + GetAddress(FCPBACKADD1),
		reinterpret_cast<DWORD>(_fcpObtainData), 18);


	//step 2. disable out of bounds error- we know that, but we are using new, bigger buffers
	ModPage(IMAGE_BASE + GetAddress(FIELDCHARENT1), 1);
	*reinterpret_cast<BYTE*>(IMAGE_BASE + GetAddress(FIELDCHARENT1)) = 0xEB; //JBE -> JMP

	ModPage(IMAGE_BASE + GetAddress(FIELDCHARENT2), 1);
	*reinterpret_cast<BYTE*>(IMAGE_BASE + GetAddress(FIELDCHARENT2)) = 0xEB; //JBE -> JMP

	//1160545A - set
	fcpBackAdd2GlTexSubImage = InjectJMP(IMAGE_BASE + ADR_FCP_PRETEXSUBIMAGE,
		reinterpret_cast<DWORD>(_fcpSetYoffset), 26);
}