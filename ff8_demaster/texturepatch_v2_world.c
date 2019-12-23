#include "coreHeader.h"

DWORD _wtpBackAdd1;
DWORD _wtpBackAdd2;


DWORD _wtpSaveTpage = -1;

//you can't just create new folder because >WEEP< - no, really, idk but it doesn't work
char* _wtpReplaceVoid();

BOOL bWtpHdAvailable(int tPage)
{
	_wtpSaveTpage = tPage;
	char localn[256];
	sprintf(localn, "%stextures\\", DIRECT_IO_EXPORT_DIR);
	char* wtprep = _wtpReplaceVoid();
	strcat(localn, wtprep);
	strcat(localn, ".png");
	DWORD attr = GetFileAttributesA(localn);
	if (attr == INVALID_FILE_ATTRIBUTES)
		return FALSE;
	else return TRUE;
}

void wtpVoid()
{
	char localn[256];
	sprintf(localn, "_wtp::Load world module: %d at TPage: %d\n", *(DWORD*)(IMAGE_BASE + 0x178207C), *(DWORD*)(IMAGE_BASE + 0x1782084));
	OutputDebugStringA(localn);

	int currentTpage = *(DWORD*)(IMAGE_BASE + 0x1782084);
	if (currentTpage < 16 || currentTpage>26)
		return;
	if (!bWtpHdAvailable(currentTpage))
		return;
	*(DWORD*)(IMAGE_BASE + 0x1782080) = 1; //HDavailable DWORD 0-vanilla tex 1- use texture_handler by given tex type
	return;
}


char* _wtpReplaceVoid()
{
	char localn[256];
	sprintf(localn, "field.fs\\field_hd_new\\hp_texpg_%d", _wtpSaveTpage - 3);
	//sprintf(localn, "FIELD.FS\\field_hd_new\\wmset_014_0"); - another dotemu limiter for files ehh
	
	return localn;
}


DWORD _wtpSaveEax = 0;

__declspec(naked) void _wtpReplace()
{
	__asm 
	{
		MOV _wtpSaveEax, EAX
		CALL _wtpReplaceVoid
		PUSH EAX

		MOV EAX, _wtpSaveEax
		JMP _wtpBackAdd2
	}
}

__declspec(naked) void _wtp()
{
	__asm
	{
		MOV EAX, OFFSET IMAGE_BASE
		MOV EAX, [EAX]
		ADD EAX, 0x1782084 //currentTpage dword
		MOV [EAX], EDI

		CMP EDX, 0x12
		JE _worldHack
		//CMP ...
		//JE  ...

		_out:
		JMP _wtpBackAdd1

			_worldHack:
		PUSH EAX
			PUSH EBX
			PUSH ECX
			PUSH EDX
		CALL wtpVoid
			POP EDX
			POP ECX
			POP EBX
			POP EAX
		JMP _out
	}
}

void ApplyWorldPatch()
{
	_wtpBackAdd1 = InjectJMP(IMAGE_BASE + 0x1591847, (DWORD)_wtp, 6); //this is to force bHdAvailable on wm tex
	_wtpBackAdd2 = InjectJMP(IMAGE_BASE + 0x16065F4, (DWORD)_wtpReplace, 5); //this is to pre-select PNG if available
}