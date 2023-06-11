#include "debug.h"
#include "coreHeader.h"

#include <string>

bool StringContainsBadCharacters(const std::string& str)
{
    for (const auto c : str)
        if ((c < 0x20 && c != 0x0A) || c > 0x7E)
            return true;
    return false;
}

void OutputDebug(const char* fmt, ...)
{
    if(fmt < reinterpret_cast<const char*>(0x100))
        return;
    if (IsBadReadPtr(fmt, 4)) return;
    const std::string fmtString = std::string(fmt);
    if (fmtString.substr(0, 7) == std::string("GLERROR"))
        return;
    if(StringContainsBadCharacters(fmtString))
        return;
    
    va_list args;
    char tmpStr[1024];

    va_start(args, fmt);
    vsnprintf(tmpStr, sizeof(tmpStr), fmt, args);
    va_end(args);

    printf(tmpStr);

    if (logFile)
    {
        fwrite(tmpStr, sizeof(char), strlen(tmpStr), logFile.get());
        fflush(logFile.get());
    }
}

//Maki: Why this doesn't work?
LONG WINAPI ExceptionHandler(EXCEPTION_POINTERS* ep)
{
    DemasteredStackWalker sw;

    OutputDebug("*** Exception 0x%x, address 0x%x ***\n", ep->ExceptionRecord->ExceptionCode, ep->ExceptionRecord->ExceptionAddress);
    sw.ShowCallstack(
        GetCurrentThread(),
        ep->ContextRecord
    );

    // show cursor in case it was hidden
    ShowCursor(true);

    MessageBoxA(0, "Oops! Something very bad happened.\n\nPlease provide a copy of demasterlog.txt when reporting this error at https://github.com/MaKiPL/FF8_demastered/issues.\n", "Error", MB_OK);

    // let OS handle the crash
    SetUnhandledExceptionFilter(0);
    return EXCEPTION_CONTINUE_EXECUTION;
}