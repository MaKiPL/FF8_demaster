//Contains modified parts of the code from FFNx

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

#pragma once
#include <iostream>
#include <Windows.h>
#include "log.h"

/**
 * Modifies the memory protection of a specified page range to allow execution and read/write access.
 *
 * @param address A pointer to the base address of the region of pages to be modified.
 * @param size The size of the region to be modified, in bytes.
 */
inline void ModPage(const void* address, const size_t size)
{
    DWORD lastProtect = 0;
    const DWORD failure = VirtualProtect(const_cast<LPVOID>(address),
        size, PAGE_EXECUTE_READWRITE, &lastProtect);
    
    if (failure == 0)
    {
        const DWORD myError = GetLastError();
        spdlog::error("[PATCH] ModPage error: %08lX", myError);
    }
}

/**
 * Retrieves the absolute 32-bit value stored at a specified offset from a base address.
 *
 * @param base A pointer to the base address from which to calculate the offset.
 * @param offset The number of bytes to offset from the base address.
 * @return The 32-bit value stored at the calculated address.
 */
inline uint32_t GetAbsoluteValue(const void* base, const size_t offset)
{
    const uint8_t* bytePtr = static_cast<const uint8_t*>(base);
    return *reinterpret_cast<const uint32_t*>(bytePtr + offset);
}

/**
 * Replaces a function call at a specified memory offset with a call to a new function.
 *
 * @param offset A pointer to the memory location where the call should be replaced.
 * @param func A pointer to the new function that should be called.
 */
inline void ReplaceCall(void* offset, void* func)
{
    ModPage(offset, 5);
    uint8_t* bytePtr = static_cast<uint8_t*>(offset);
    bytePtr[0] = 0xE9; //   CALL opcode
    *reinterpret_cast<int32_t*>(bytePtr + 1) = static_cast<int32_t>(
        reinterpret_cast<uintptr_t>(func) - reinterpret_cast<uintptr_t>(offset) - 5);
}

/**
 * Replaces a jump instruction at a specified memory offset with a jump to a new location.
 *
 * @param offset A pointer to the memory location where the jump should be replaced.
 * @param func A pointer to the new location that should be jumped to.
 */
inline void ReplaceJmp(void* offset, void* func)
{
    ModPage(offset, 5);
    uint8_t* bytePtr = static_cast<uint8_t*>(offset);
    bytePtr[0] = 0xE8; //   JMP opcode
    *reinterpret_cast<int32_t*>(bytePtr + 1) = static_cast<int32_t>(
        reinterpret_cast<uintptr_t>(func) - reinterpret_cast<uintptr_t>(offset) - 5);
}

/**
 * Injects a 32-bit value (DWORD) at a specified memory address.
 *
 * @param address A pointer to the memory location where the DWORD should be injected.
 * @param value The 32-bit value to be injected.
 */
inline void InjectDword(void* address, const uint32_t value)
{
    ModPage(address, sizeof(uint32_t));
    *static_cast<uint32_t*>(address) = value;
}

/**
 * Injects a 16-bit value (WORD) at a specified memory address.
 *
 * @param address A pointer to the memory location where the WORD should be injected.
 * @param value The 16-bit value to be injected.
 */
inline void InjectWord(void* address, const uint16_t value)
{
    ModPage(address, sizeof(uint16_t));
    *static_cast<uint16_t*>(address) = value;
}