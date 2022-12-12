#pragma once
#include <cstdio>
#include <memory>
#include <StackWalker.h>
#include <plog/Log.h>
#include "plog/Helpers/HexDump.h"
#include "plog/Converters/UTF8Converter.h"

inline void OutputDebugPatch(const char* fmt, ...)
{
#if _DEBUG
    if (IsBadReadPtr(fmt, 4)) return;
    std::string fmtString = std::string(fmt);
    if (fmtString.substr(0, 7) == std::string("GLERROR"))
        return;
    va_list args;
    char tmp_str[1024];

    va_start(args, fmt);
    vsnprintf(tmp_str, sizeof(tmp_str), fmt, args);
    va_end(args);

    printf(tmp_str);
    PLOG_VERBOSE << tmp_str;
#endif
}