#include "coreHeader.h"

DWORD _bhpBackAdd1;
DWORD _bhpBackAdd2;
DWORD _bhpBackAdd3;

char* _bhpStrPointer;
DWORD* texture_file_enemy_ex_Id;

//instead of hardcoding data to DLL, we make it dynamic so it seeks the textures on it's own
DWORD _bhpMonsterStructVoid()
{
	int batId;
	__asm
	{
		MOV batId, EDX
	}
	batId -= 1000;
	if (batId < 0 || batId>144)
		return -1;
	char localn[256];
	sprintf(localn, "%stextures\\battle.fs\\hd_new\\c0m%03d_0.png", DIRECT_IO_EXPORT_DIR, batId);
	int maxPal = 0;
	int _strlen = strlen(localn);
	while (1)
	{
		localn[_strlen - 5] = '0' + maxPal;
		DWORD attr = GetFileAttributesA(localn);
		if (attr == INVALID_FILE_ATTRIBUTES)
		{
			if (maxPal == 0)
			{
				sprintf(localn, "_bhpMonsterStructVoid::Not found entry of c0m%03d_0.png- treating as null;\n", batId);
				OutputDebug(localn);
				return -1;
			}
			break;
		}
		maxPal++;
	}
	sprintf(localn, "_bhpMonsterStructVoid::Custom worker- found C0M%03d that have %d pages\n", batId, maxPal);
	OutputDebug(localn);
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

BYTE _bhpVoid()
{
	char localn[256];
	char localPath[256];
	sprintf(localn, "texturepatchv2::battleHooks::BhpVoid(%s)\n", _bhpStrPointer);
	OutputDebug(localn);
	
	char bhpChechker = _bhpStrPointer[0];
	bhpChechker = tolower(bhpChechker);

	//currentStage = -1;

	if (bhpChechker == 'a')
	{
		char stageId[4];
		strncpy(stageId, &_bhpStrPointer[5], 3);
		stageId[3] = '\0';
		int intStageId = atoi(stageId);
		currentStage = intStageId;

		//probably some stages are on different tpages (is it possible?) so I am leaving it so the code will check before releasing
		//vanilla texture and if something bad happens- it will go back to default tex instead of black

		//sprintf(localPath, "%stextures\\battle.fs\\hd_new\\a0stg%03d_16.png", DIRECT_IO_EXPORT_DIR, intStageId);
		//DWORD attr = GetFileAttributesA(localPath);
		//if (attr == INVALID_FILE_ATTRIBUTES) //file doesn't exist, so please do not replace textures 
		//{
		//	currentStage = -1;
		//	return 0;
		//}

		//*texture_file_enemy_ex_Id = 1200 + intStageId;
		currentStage = intStageId;
		return 1;
	}
	else if (bhpChechker == 'c')
	{
		char monsId[4];
		strncpy(monsId, &_bhpStrPointer[3], 3);
		monsId[3] = '\0';
		int intMonsId = atoi(monsId);

		sprintf(localPath, "%stextures\\battle.fs\\hd_new\\c0m%03d_0.png", DIRECT_IO_EXPORT_DIR, intMonsId);
		DWORD attr = GetFileAttributesA(localPath);
		if (attr == INVALID_FILE_ATTRIBUTES) //file doesn't exist, so please do not replace textures
			return 0;

		*texture_file_enemy_ex_Id = 1000 + intMonsId;
		return 1;
	}
	else 
		return 0;

	return;
}

//due to the way the function is constructed (break from while(1)) we need to re-check
//if the algorithm found Id- if yes on ANY case, then jumpout. If all indexes are zero
//then let's search texture replacement our way by checking file exists and not hardcoded struct
__declspec(naked) void _bhp()
{
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
		ADD ECX, 0x16482A8
		CALL ECX
		ADD ESP, 4

		_exitbph:
		JMP _bhpBackAdd1
	}
}

DWORD _bhpJmpAdd1;

__declspec(naked) void _bhptpage()
{
	__asm
	{
		CMP ECX, 0x10
		JAE _forceJumpBhp
		_getback:
			ADD ECX, 0xFFFFFFF6
			CMP ECX, 5
			JMP _bhpBackAdd3

		_forceJumpBhp:
			CMP currentStage, 0xFFFFFFFF
			JE _getback
			MOV EAX, [EDI]
			ADD EAX, 0x14
			MOV ECX, currentStage
			ADD ECX, 1200
			MOV [EAX], ECX
			JMP _bhpJmpAdd1
	}
}

void ApplyBattleHookPatch()
{
	currentStage = -1;
	_bhpBackAdd1 = InjectJMP(IMAGE_BASE + 0x157DA5D, (DWORD)_bhp, 17);
	_bhpBackAdd2 = InjectJMP(IMAGE_BASE + 0x157DD05, (DWORD)_bhpMonsterStruct, 5); //GetBattleMonsterStructPalCount _notfound
	
	//_bhpJmpAdd1 = IMAGE_BASE + 0x157EF85; //Jump to 13-15 cases!!
	//_bhpBackAdd3 = InjectJMP(IMAGE_BASE + 0x157EF72, (DWORD)_bhptpage, 6); //bHdAvailable_Battle -> check TPage for stage replacement
}