#include "coreHeader.h"
//#include "texturepatch_battle_data.h"


int width_bcp = 768,height_bcp = 768;

BYTE* bcpBackAdd1;
BYTE* bcpBackAdd2;
BYTE* bcpBackAdd3;

//casual is 384x384, therefore the final should be 1st texture *2
void BcpObtainTextureDatas(const int aIndex)
{
	char n[256]{ 0 };

	DDSorPNG(n, 256, R"(%stextures\battle.fs\hd_new\d%xc%03u_0)", DIRECT_IO_EXPORT_DIR,
		(aIndex - 4097) / 100, (aIndex - 4097) % 100);

	const SafeBimg img = LoadImageFromFile(n); //chara 0
	if (!img)
		return;
	width_bcp = img->m_width * 2;
	height_bcp = img->m_height * 2;
	
	PLOG_VERBOSE << "_bcpObtainTextureDatas:: width=" << width_bcp << ", height=" << height_bcp << ", filename=" << n;
}

__declspec(naked) void BcpObtainData()
{
	__asm
	{
		MOV EAX, dword ptr [EDI+0x10]
		MOV tex_struct, EAX
		MOV langIdent_ESI, ESI

		PUSH EAX
		CALL BcpObtainTextureDatas
	POP EAX

		PUSH 0
		PUSH 0
		PUSH 0
		PUSH [height_bcp]
		PUSH [width_bcp]
		PUSH ECX
		PUSH NEWGLTEX_CHARA
		CALL GetAddress
		MOV ECX, EAX
		
		MOV EAX, OFFSET IMAGE_BASE
		MOV EAX, [EAX]
		ADD EAX, ECX //createGLTexture
		POP ECX
		CALL EAX

		JMP bcpBackAdd1
	}
}

__declspec(naked) void BcpPushHeightOffsetY()
{
	__asm
	{
		//assembler already pushed eax, so we can use it
		MOV EAX, [height_bcp]
		SHR EAX, 1
		PUSH EAX
		JMP bcpBackAdd2
	}
}

__declspec(naked) void BcpPushHeightOffsetYMinusHeight()
{
	__asm
	{
		//assembler already pushed eax, so we can use it
		MOV EAX, [height_bcp]
		SHR EAX, 3
		NEG EAX //we now have -24
		ADD EAX, [height_bcp] //-24 + height = correct Y


		PUSH EAX
		JMP bcpBackAdd3
	}
}

void ApplyBattleCharacterPatch()
{
	//step 1. obtain needed data for tex_struct and etc.
	bcpBackAdd1 = InjectJMP(IMAGE_BASE + GetAddress(BCPBACKADD1)
		, reinterpret_cast<DWORD>(BcpObtainData), 18);

	//step 2. we now have correct dynamic glTexture, but we need to change the LoadBattleCharaTexture and WeaponTexture
	bcpBackAdd2 = InjectJMP(IMAGE_BASE + GetAddress(BCPBACKADD2)
		, reinterpret_cast<DWORD>(BcpPushHeightOffsetY), 5);


	//step 3. The last thing we need is replacing the weapon texture. The Y offset is 0x2a0 (768-96)
	bcpBackAdd3 = InjectJMP(IMAGE_BASE + GetAddress(BCPBACKADD3)
		, reinterpret_cast<DWORD>(BcpPushHeightOffsetYMinusHeight), 5);
}