#include "patchio.h"
#include "dllmain.h"
#include "memory"

#include <filesystem>
#include <spdlog/spdlog.h>

#include "memory.h"
#include "patch.h"

void PatchIO()
{
    spdlog::info("[IO] Patching IO functions...");


    if(!std::filesystem::exists(EXPORTPATH))
    {
        spdlog::error("[IO] Export path does not exist: {}", EXPORTPATH);
        return;
    }

    //IO_FUNC1
    
    //IO_FUNC2
    
    // IO_FUNC3
    // at WEEP::OpenFile (0x10036B78 we have fseek to archive ulong. As we are not reading archive, we just seek to zero
    //
    // PUSH 0; Origin
    // ->> PUSH dword ptr [ebp+Offset]; Offset -> PUSH 0 + NOP (3 bytes)
    // PUSH dword ptr [esi+0x1C]; Stream
    // call ds:fseek
    //InjectDword(GetAddressVoid(IOFUNC3), 0xFF90006A);
    
    //IO_FUNC4
    // at WEEP::Archive::File::Seek (0x10036D37)
    // it adds various offset to stream pointer
    // mov     eax, [edi+8] -> we want this to be 0, because [edi+8] is archive file pointer
    InjectDword(GetAddressVoid(IOFUNC4), 0x8B90C031);
    
    //IO_FUNC5
    
    //IO_FUNC6
    // at OpenArchiveAndSeek (0x1003656A) - seems like some check for .zzz file format, so we just skip it
    //
    // call    sub_10037780
    // test    eax, eax
    // ->> js      short loc_10036572
    //InjectWord(GetAddressVoid(IOFUNC6), 0x9090); //We put two NOP (2 bytes) 
}
