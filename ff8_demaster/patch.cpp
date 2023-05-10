#include "coreHeader.h"

BOOL modPage(DWORD address, int size = 5)
{
	DWORD lastProtect = 0;
	DWORD failure = VirtualProtect((LPVOID)address, size, PAGE_EXECUTE_READWRITE, &lastProtect);
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
	BYTE* fopenPatchMnemonic = (BYTE*)address;
	BYTE* IO_backAddress = fopenPatchMnemonic + JMPsize;
	DWORD jmpParam = functionAddress - (DWORD)fopenPatchMnemonic - 5;
	modPage((DWORD)fopenPatchMnemonic, 5);
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

/****************************************************************************/
//    Copyright (C) 2009 Aali132                                            //
//    Copyright (C) 2018 quantumpencil                                      //
//    Copyright (C) 2018 Maxime Bacoux                                      //
//    Copyright (C) 2020 myst6re                                            //
//    Copyright (C) 2020 Chris Rizzitello                                   //
//    Copyright (C) 2020 John Pritchard                                     //
//    Copyright (C) 2023 Julian Xhokaxhiu                                   //
/****************************************************************************/
//From FFNx : https://github.com/julianxhokaxhiu/FFNx/blob/master/src/patch.cpp

void check_is_call(const char *name, const uint32_t base, const uint32_t offset, const uint8_t instruction)
{
	if (instruction != 0xE8 && instruction != 0xE9)
		// Warning to diagnose errors faster
		OutputDebug("%s: Unrecognized call/jmp instruction at 0x%X + 0x%X (0x%X): 0x%X\n", name, base, offset,
		            base + offset, instruction);
}

uint32_t replace_function(uint32_t offset, void *func)
{
	DWORD dummy;

	VirtualProtect(reinterpret_cast<void*>(offset), 5, PAGE_EXECUTE_READWRITE, &dummy);

	*reinterpret_cast<unsigned char*>(offset) = 0xE9;
	*reinterpret_cast<uint32_t*>(offset + 1) = reinterpret_cast<uint32_t>(func) - offset - 5;
	return 0;
}

uint32_t get_relative_call(uint32_t base, uint32_t offset)
{
	const uint8_t instruction = *reinterpret_cast<uint8_t*>(base + offset);
	check_is_call(__func__, base, offset, instruction);
	const uint32_t ret = base + *reinterpret_cast<uint32_t*>(base + offset + 1) + offset + 5;
	return ret;
}

uint32_t get_absolute_value(uint32_t base, uint32_t offset) { return *reinterpret_cast<uint32_t*>(base + offset); }

void replace_call(uint32_t offset, void *func)
{
	DWORD dummy;
	check_is_call(__func__, offset, 0, *reinterpret_cast<uint8_t*>(offset));
	VirtualProtect(reinterpret_cast<void*>(offset), 5, PAGE_EXECUTE_READWRITE, &dummy);
	*reinterpret_cast<uint32_t*>(offset + 1) = reinterpret_cast<uint32_t>(func) - offset - 5;
}

uint32_t replace_call_function(uint32_t offset, void* func)
{
	DWORD dummy;
	VirtualProtect(reinterpret_cast<void*>(offset), 5, PAGE_EXECUTE_READWRITE, &dummy);
	*reinterpret_cast<unsigned char*>(offset) = 0xE8;
	*reinterpret_cast<uint32_t*>(offset + 1) = reinterpret_cast<uint32_t>(func) - offset - 5;
	return 0;
}