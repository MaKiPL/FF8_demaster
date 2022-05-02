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


    DR_CREATE_LOGGER

    

    return TRUE;
}

