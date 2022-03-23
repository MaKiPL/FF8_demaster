#include "core.h"

/**
 * \brief Main entry point when loaded by EFIGS
 * \return 
 */
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        return ERROR_BAD_CONFIGURATION;
    }

    //TODO: Maki: create logger so we can allocconsole and control output with warnings and logs
    //with colours and etc.

    return TRUE;
}

