#include "coreHeader.h"
#include "texture.h"

DWORD _bhpBackAdd1;
DWORD _bhpBackAdd2;

char* _bhpStrPointer;
DWORD* texture_file_enemy_ex_Id;

//instead of hardcoding data to DLL, we make it dynamic so it seeks the textures on it's own
DWORD _bhpMonsterStructVoid()
{
	//InjectMonsterAtlasResolution(0x180);
	
	int batId;
	__asm
	{
		MOV batId, EDX
	}
	batId -= 1000;
	if (batId < 0 || batId>144)
		return -1;
	char localn[256]{ 0 };
	DDSorPNG(localn, 256, "%stextures\\battle.fs\\hd_new\\c0m%03d_0", DIRECT_IO_EXPORT_DIR, batId);
	int maxPal = 0;
	int _strlen = strlen(localn);
	
	if(GetFileAttributesA(localn) != INVALID_FILE_ATTRIBUTES)
	{
		DWORD res = GetImageResolutionFast(localn).width;
		InjectMonsterAtlasResolution(res);
	}
	else
		InjectMonsterAtlasResolution(DEFAULT_MONSTER_ATLAS_TEX_RESOLUTION);
	
	while (true)
	{
		localn[_strlen - 5] = '0' + maxPal;
		if (GetFileAttributesA(localn) == INVALID_FILE_ATTRIBUTES)
		{
			if (maxPal == 0)
			{
				OutputDebug("_bhpMonsterStructVoid::Not found entry of c0m%03d_0.(dds|png)- treating as null;\n", batId);
				return -1;
			}
			break;
		}
		maxPal++;
	}
	//InjectMonsterAtlasResolution(DEFAULT_MONSTER_ATLAS_TEX_RESOLUTION);
	OutputDebug("_bhpMonsterStructVoid::Custom worker- found C0M%03d that have %d pages\n", batId, maxPal);
	return maxPal;
}

__declspec(naked) void _bhpMonsterStruct()
{
	__asm
	{
		CALL _bhpMonsterStructVoid
		TEST EAX, EAX
		JZ _out
		//EAX HERE CONTAINS THE PAL NUM
		POP EBP
		RETN

		_out:
		OR EAX, 0xFFFFFFFF
		POP EBP
		RETN
	}
}

//sub_116059B0
void InjectMonsterAtlasResolution(const DWORD monsterTexResolution)
{
	OutputDebug("InjectMonsterAtlasResolution::Injecting atlas resolution of: %d\n", monsterTexResolution);
	
	constexpr int addr1Shift = 0x13ED + 6;
	constexpr int addr2Shift = 0x13FE + 6;
	
	#ifdef JAPANESE_PATCH 
	addr1Shift = 0x130F+6;
	addr2Shift = 0x1320+6;
	#endif
	
	InjectDWORD(IMAGE_BASE + GetAddress(SUB_116059B0)+addr1Shift, monsterTexResolution*2); //sub_116059B0+13ED + 6
	InjectDWORD(IMAGE_BASE + GetAddress(SUB_116059B0)+addr2Shift, monsterTexResolution*2); //sub_116059B0+13FE + 6
	InjectWORD(IMAGE_BASE + GetAddress(LOAD_BATTLE_EXT)+0xBC1+1, static_cast<WORD>(monsterTexResolution)); //load_battle_ext+BC1 + 1
}

BYTE _bhpVoid()
{
	char localPath[256]{ 0 };
	OutputDebug("texturepatchv2::battleHooks::BhpVoid(%s)\n", _bhpStrPointer);
	
	char bhpChechker = _bhpStrPointer[0];
	bhpChechker = tolower(bhpChechker);

	//currentStage = -1;

	if (bhpChechker == 'a' && !strncmp("A0STG", _bhpStrPointer, 5))
	{
		char stageId[4];
		strncpy(stageId, &_bhpStrPointer[5], 3);
		stageId[3] = '\0';
		int intStageId = atoi(stageId);
		currentStage = intStageId;

		//*texture_file_enemy_ex_Id = 1200 + intStageId;
		currentStage = intStageId;
		return 1;
	}
	else if (bhpChechker == 'c')
	{
		InjectMonsterAtlasResolution(DEFAULT_MONSTER_ATLAS_TEX_RESOLUTION);
		char monsId[4];
		strncpy(monsId, &_bhpStrPointer[3], 3);
		monsId[3] = '\0';
		int intMonsId = atoi(monsId);

		DDSorPNG(localPath, 256, "%stextures\\battle.fs\\hd_new\\c0m%03d_0", DIRECT_IO_EXPORT_DIR, intMonsId);
		if (GetFileAttributesA(localPath) == INVALID_FILE_ATTRIBUTES) //file doesn't exist, so please do not replace textures
			return 0;

		*texture_file_enemy_ex_Id = 1000 + intMonsId;

		DWORD monsterTexResolution = GetImageResolutionFast(localPath).width;

		OutputDebug("_bhpVoid::Injecting atlas resolution of: %d\n", monsterTexResolution);

		InjectMonsterAtlasResolution(monsterTexResolution);
		
		return 1;
	}
	else 
		return 0;

	return 0;
}

//due to the way the function is constructed (break from while(1)) we need to re-check
//if the algorithm found Id- if yes on ANY case, then jumpout. If all indexes are zero
//then let's search texture replacement our way by checking file exists and not hardcoded struct
__declspec(naked) void _bhp()
{
	InjectMonsterAtlasResolution(DEFAULT_MONSTER_ATLAS_TEX_RESOLUTION);
	__asm
	{
		//set current [ebp+str] to our dword for debugging and pre-checkin
		MOV ECX, [ebp - 0x38]
		MOV _bhpStrPointer, ECX

		//check texture_file_enemy_ex_id [1157db04]
		MOV ECX, DWORD PTR[ebp - 0x34] //texture_file_enemy_ex_id
		MOV texture_file_enemy_ex_Id, ECX //we would need that later
		MOV ECX, [ECX]
		TEST ECX, ECX
		JNZ _isreplaced

		//texture_file_enemy_id [1157dade]
		mov    ecx, DWORD PTR[ebp - 0x30]
		MOV ECX, [ECX]
		TEST ECX, ECX
		JNZ _isreplaced

		//texture_file_gf_ex_id [1157dabd]
		mov    ecx, DWORD PTR[ebp - 0x2c]
		MOV ECX, [ECX]
		TEST ECX, ECX
		JNZ _isreplaced

		//texture_file_gf_in [1157da9c]
		mov    ecx, DWORD PTR[ebp - 0x44]
		MOV ECX, [ECX]
		TEST ECX, ECX
		JNZ _isreplaced

		//NOT REPLACED? Then call our function [bhpStrPointer is already set]
		CALL _bhpVoid
		MOV BL, AL
		JMP _isreplaced2
		_isreplaced:
		XOR BL, BL
		//OR currentStage, 0xFFFFFFFF
		_isreplaced2:
		cmp     dword ptr[ebp - 0x14], 0x10
		JB	_exitbph
		push   DWORD PTR[ebp - 0x28]
		MOV ECX, OFFSET IMAGE_BASE
		MOV ECX, [ECX]
		PUSH EAX
		PUSH ECX
		PUSH BHP
		CALL GetAddress
		POP ECX
		ADD ECX, EAX
		POP EAX
		CALL ECX
		ADD ESP, 4

		_exitbph:
		JMP _bhpBackAdd1
	}
}

void ApplyBattleHookPatch()
{
	currentStage = -1;
	_bhpBackAdd1 = reinterpret_cast<DWORD>(InjectJMP(IMAGE_BASE + GetAddress(_BHPBACKADD1),
		reinterpret_cast<DWORD>(_bhp), 17));
	_bhpBackAdd2 = reinterpret_cast<DWORD>(InjectJMP(IMAGE_BASE + GetAddress(_BHPBACKADD2),
		reinterpret_cast<DWORD>(_bhpMonsterStruct), 5)); //GetBattleMonsterStructPalCount _notfound
	
}