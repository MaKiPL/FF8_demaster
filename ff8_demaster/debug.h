#pragma once
#include <cstdio>
#include <memory>
#include <StackWalker.h>

void OutputDebug(const char* fmt, ...);
LONG WINAPI ExceptionHandler(EXCEPTION_POINTERS* ep);

#define CRASHLOG OutputDebug("%s::%d::%s\n", __FILE__, __LINE__,__func__)
inline std::unique_ptr<FILE, decltype(&fclose)> logFile{ nullptr, fclose };

class DemasteredStackWalker final : public StackWalker
{
public:
    DemasteredStackWalker() : StackWalker() {}
protected:
    void OnDbgHelpErr(LPCSTR szFuncName, DWORD gle, DWORD64 addr) override
    {
        // Silence is golden.
    }

    void OnOutput(LPCSTR szText) override
    {
        OutputDebug(szText);
    }
};
