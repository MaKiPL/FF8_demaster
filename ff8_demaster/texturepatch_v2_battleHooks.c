#include "coreHeader.h"

DWORD _bhpBackAdd1;

char* _bhpStrPointer;
DWORD* texture_file_enemy_ex_Id;


void _bhpVoid()
{
	char localn[256];
	char localPath[256];
	sprintf(localn, "texturepatchv2::battleHooks::BhpVoid(%s)\n", _bhpStrPointer);
	OutputDebugStringA(localn);
	
	char bhpChechker = _bhpStrPointer[0];
	bhpChechker = tolower(bhpChechker);

	if (bhpChechker == 'a')
	{
		char stageId[4];
		strncpy(stageId, &_bhpStrPointer[5], 3);
		stageId[3] = '\0';
		int intStageId = atoi(stageId);

		sprintf(localPath, "%stextures\\battle.fs\\hd_new\\a0stg%03d_0.png", DIRECT_IO_EXPORT_DIR, intStageId);
		DWORD attr = GetFileAttributesA(localPath);
		if (attr == INVALID_FILE_ATTRIBUTES) //file doesn't exist, so please do not replace textures
			return;
		//int stageTpageSize = 1; //is it needed?

		//for (int i = 1; i < 8; i++)
		//{
		//	localPath[strlen(localPath) - 5] = '0' + i;
		//	DWORD attr = GetFileAttributesA(localPath);
		//	if (attr == INVALID_FILE_ATTRIBUTES) //file doesn't exist, so please do not replace textures
		//		break;
		//	stageTpageSize++;
		//}

		*texture_file_enemy_ex_Id = 1200 + intStageId;
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
			return;
		//int stageTpageSize = 1; //is it needed?

		//for (int i = 1; i < 8; i++)
		//{
		//	localPath[strlen(localPath) - 5] = '0' + i;
		//	DWORD attr = GetFileAttributesA(localPath);
		//	if (attr == INVALID_FILE_ATTRIBUTES) //file doesn't exist, so please do not replace textures
		//		break;
		//	stageTpageSize++;
		//}

		*texture_file_enemy_ex_Id = 1000 + intMonsId;
	}
	else 
		return;

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

		_isreplaced:
		
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

void ApplyBattleHookPatch()
{
	_bhpBackAdd1 = InjectJMP(IMAGE_BASE + 0x157DA5D, (DWORD)_bhp, 17);
}