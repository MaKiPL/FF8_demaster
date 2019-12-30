#include <stdio.h>
#include <Windows.h>
#include "coreHeader.h"

BOOL modPage(DWORD address, int size)
{
	PDWORD lastProtect = 0;
	DWORD failure = VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &lastProtect);
	if (failure == 0)
	{
		DWORD myError = GetLastError();
		printf("err %08X", myError);
		return FALSE;
	}
	return TRUE;
}

BYTE* InjectJMP(DWORD address, DWORD functionAddress, int JMPsize)
{
	BYTE* fopenPatchMnemonic = address;
	BYTE* IO_backAddress = fopenPatchMnemonic + JMPsize;
	DWORD jmpParam = functionAddress - (DWORD)fopenPatchMnemonic - 5;
	modPage(fopenPatchMnemonic, 5);
	*fopenPatchMnemonic = 0xE9; //JMP [DW]
	*(DWORD*)(fopenPatchMnemonic + 1) = jmpParam;
	return IO_backAddress;
}

void ReplaceCALLWithNOP(DWORD address)
{
	modPage(IMAGE_BASE + address, 5);
	*(DWORD*)(IMAGE_BASE + address) = 0x90909090;
	*(BYTE*)(IMAGE_BASE + address + 4) = 0x90;
}

void InjectDWORD(DWORD address, DWORD value)
{
	modPage(address, 4);
	*(DWORD*)(address) = value;
}