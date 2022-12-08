#include "debug.h"

#include <string>

void OutputDebug(const char* fmt, ...)
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

    if (logFile)
    {
        fwrite(tmp_str, sizeof(char), strlen(tmp_str), logFile.get());
        fflush(logFile.get());
    }
#endif
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