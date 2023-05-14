#include "coreHeader.h"

BOOL ModPage(const DWORD address, const int size)
{
	DWORD lastProtect = 0;
	const DWORD failure = VirtualProtect(reinterpret_cast<LPVOID>(address),
		size, PAGE_EXECUTE_READWRITE, &lastProtect);
	if (failure == 0)
	{
		const DWORD myError = GetLastError();
		printf("ModPage error: %08lX", myError);
		return FALSE;
	}
	return TRUE;
}

// ReSharper disable once CppInconsistentNaming
BYTE* InjectJMP(const DWORD address, const DWORD functionAddress, const int JMPsize)
{
	BYTE* targetInstruction = reinterpret_cast<BYTE*>(address);
	BYTE* returnAddress = targetInstruction + JMPsize;
	const DWORD jmpParam = functionAddress - reinterpret_cast<DWORD>(targetInstruction) - 5;
	ModPage(reinterpret_cast<DWORD>(targetInstruction), 5);
	*targetInstruction = 0xE9; //JMP [DW]
	*reinterpret_cast<DWORD*>(targetInstruction + 1) = jmpParam;
	return returnAddress;
}

// ReSharper disable once CppInconsistentNaming
void ReplaceWithNOP(const DWORD address, const int instructionSize)
{
	ModPage(IMAGE_BASE + address, instructionSize);
	for(int i = 0; i < instructionSize; i++)
		*reinterpret_cast<BYTE*>(IMAGE_BASE + address + i) = 0x90;
}

// ReSharper disable once CppInconsistentNaming
void InjectDWORD(const DWORD address, const DWORD value)
{
	ModPage(address, 4);
	*reinterpret_cast<DWORD*>(address) = value;
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

void CheckIsCall(const char *name, const uint32_t base, const uint32_t offset, const uint8_t instruction)
{
	if (instruction != 0xE8 && instruction != 0xE9)
		// Warning to diagnose errors faster
		OutputDebug("%s: Unrecognized call/jmp instruction at 0x%X + 0x%X (0x%X): 0x%X\n", name, base, offset,
		            base + offset, instruction);
}

void ReplaceFunction(const uint32_t offset, void *func)
{
	ModPage(offset, 5);
	*reinterpret_cast<unsigned char*>(offset) = 0xE9;
	*reinterpret_cast<uint32_t*>(offset + 1) = reinterpret_cast<uint32_t>(func) - offset - 5;
}

uint32_t GetRelativeCall(const uint32_t base, const uint32_t offset)
{
	const uint8_t instruction = *reinterpret_cast<uint8_t*>(base + offset);
	CheckIsCall(__func__, base, offset, instruction);
	const uint32_t ret = base + *reinterpret_cast<uint32_t*>(base + offset + 1) + offset + 5;
	return ret;
}

uint32_t GetAbsoluteValue(const uint32_t base, const uint32_t offset)
{
	return *reinterpret_cast<uint32_t*>(base + offset);
}

void ReplaceCall(const uint32_t offset, void *func)
{
	CheckIsCall(__func__, offset, 0, *reinterpret_cast<uint8_t*>(offset));
	ModPage(offset, 5);
	*reinterpret_cast<uint32_t*>(offset + 1) = reinterpret_cast<uint32_t>(func) - offset - 5;
}

void ReplaceCallFunction(const uint32_t offset, void* func)
{
	ModPage(offset, 5);
	*reinterpret_cast<unsigned char*>(offset) = 0xE8;
	*reinterpret_cast<uint32_t*>(offset + 1) = reinterpret_cast<uint32_t>(func) - offset - 5;
}