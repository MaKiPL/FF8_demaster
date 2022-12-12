#include "coreHeader.h"

BOOL ModPage(const DWORD address, const int size = 5)
{
	DWORD lastProtect = 0;
	const DWORD failure = VirtualProtect(reinterpret_cast<LPVOID>(address), size
		, PAGE_EXECUTE_READWRITE, &lastProtect);
	if (failure == 0)
	{
		const DWORD myError = GetLastError();
		PLOG_ERROR << "VirtualProtect failed with error code: " << myError;
		return FALSE;
	}
	return TRUE;
}

BYTE* InjectJMP(DWORD address, DWORD functionAddress, int JMPsize)
{
	BYTE* fopenPatchMnemonic = (BYTE*)address;
	BYTE* IO_backAddress = fopenPatchMnemonic + JMPsize;
	DWORD jmpParam = functionAddress - (DWORD)fopenPatchMnemonic - 5;
	ModPage((DWORD)fopenPatchMnemonic, 5);
	*fopenPatchMnemonic = 0xE9; //JMP [DW]
	*(DWORD*)(fopenPatchMnemonic + 1) = jmpParam;
	return IO_backAddress;
}

void ReplaceCALLWithNOP(DWORD address)
{
	ModPage(IMAGE_BASE + address, 5);
	*(DWORD*)(IMAGE_BASE + address) = 0x90909090;
	*(BYTE*)(IMAGE_BASE + address + 4) = 0x90;
}

void InjectDWORD(DWORD address, DWORD value)
{
	ModPage(address, 4);
	*(DWORD*)(address) = value;
}