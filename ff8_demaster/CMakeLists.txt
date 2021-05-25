# Some of the lines in here can be removed. This is a series of trial and error.
include(cmake/glew.cmake)
add_library(${PROJECT_NAME}_demaster_dll SHARED
        dllmain.cpp
        iopatch.cpp
        openglHook.cpp
        patch.cpp
        texturepatch_v2.cpp
        texturepatch_v2_battleCharacter.cpp
        texturepatch_v2_battleHooks.cpp
        texturepatch_v2_battleMonster.cpp
        texturepatch_v2_battleScenery.cpp
        texturepatch_v2_fieldBackground.cpp
        texturepatch_v2_fieldChara.cpp
        texturepatch_v2_world.cpp
        uvpatch.cpp
        )
#target_link_libraries(${PROJECT_NAME}_demaster_dll)
target_include_directories(${PROJECT_NAME}_demaster_dll
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/include/compat/msvc
        )
target_link_options(${PROJECT_NAME}_demaster_dll
        PRIVATE /FORCE:MULTIPLE # fatal error LNK1169: one or more multiply defined symbols found
        #see https://docs.microsoft.com/en-us/cpp/error-messages/tool-errors/linker-tools-warning-lnk4098?view=msvc-160&viewFallbackFrom=vs-2019
        PRIVATE /NODEFAULTLIB:MSVCRT # warning LNK4098: defaultlib 'MSVCRT' conflicts with use of other libs
        PRIVATE /NODEFAULTLIB:LIBCMT # warning LNK4098: defaultlib 'LIBCMT' conflicts with use of other libs
        PRIVATE /NODEFAULTLIB:LIBCMTD # warning LNK4098: defaultlib 'LIBCMTD' conflicts with use of other libs
        PRIVATE /DYNAMICBASE:NO # Disable address space layout randomization
        #PRIVATE /SUBSYSTEM:WINDOWS # WINDOWS is the default
        #PRIVATE /NXCOMPAT # Compatible with Data Execution Prevention default enabled
        #PRIVATE /TLBID:1 # Specify Resource ID for TypeLib default is 1
        PRIVATE /MANIFESTUAC:NO # The linker doesn't embed UAC information in the program manifest.
        PRIVATE /DLL # Builds a DLL.
        PRIVATE /MACHINE:X86 # The /MACHINE option specifies the target platform for the program.
        PRIVATE /ignore:4099 # HIDES warning LNK4099: PDB '*.pdb' was not found with *.lib, linking object as if no debug info.
        PRIVATE /ignore:4075 # HIDES warning LNK4075: ignoring '/INCREMENTAL' due to '/FORCE' specification.
        )
message(STATUS "${glew_fetch_SOURCE_DIR}/include")
target_link_directories(${PROJECT_NAME}_demaster_dll
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/lib
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/lib/bgfx
        PUBLIC "${glew_fetch_SOURCE_DIR}/include"
        )
target_link_libraries(${PROJECT_NAME}_demaster_dll
        PRIVATE StackWalker.lib
        PRIVATE libglew_static
        #PRIVATE glew32s.lib
        PRIVATE opengl32.lib
        PRIVATE psapi.lib
        PRIVATE bxDebug.lib
        PRIVATE bimgDebug.lib
        PRIVATE bimg_decodeDebug.lib
        PRIVATE bimg_encodeDebug.lib
        PRIVATE msvcrtd.lib # /MTd requires this library
        #        PRIVATE kernel32.lib
        #        PRIVATE user32.lib
        #        PRIVATE gdi32.lib
        #        PRIVATE winspool.lib
        #        PRIVATE comdlg32.lib
        #        PRIVATE advapi32.lib
        #        PRIVATE shell32.lib
        #        PRIVATE ole32.lib
        #        PRIVATE oleaut32.lib
        #        PRIVATE uuid.lib
        #        PRIVATE odbc32.lib
        #        PRIVATE odbccp32.lib
        #        PRIVATE legacy_stdio_definitions.lib
        )
set_target_properties(${PROJECT_NAME}_demaster_dll PROPERTIES
        OUTPUT_NAME "ff8_demaster"
        #MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
        #COMPILE_FLAGS /MTd
        MSVC_RUNTIME_LIBRARY "MultiThreadedDebug" # cl : Command line warning D9025 : overriding '/MD' with '/MTd'
        )
target_compile_options(${PROJECT_NAME}_demaster_dll
        #        PRIVATE /JMC
        #        PRIVATE /permissive-
        #        PRIVATE /GS
        #        PRIVATE /analyze-
        #        PRIVATE /W3
        #        PRIVATE /Zc:wchar_t
        #        #PRIVATE /ZI
        #        PRIVATE /Gm-
        #        PRIVATE /Od # default suppresses code movement, it simplifies the debugging process.
        #        PRIVATE /Ob0 # default with /0d, Disables inline expansions.
        #        PRIVATE /sdl
        #        PRIVATE /Zc:inline
        #        PRIVATE /fp:precise
        #        PRIVATE /errorReport:prompt
        #        PRIVATE /WX-
        #        PRIVATE /Zc:forScope
        #        PRIVATE /RTC1
        #        PRIVATE /Gd
        #        PRIVATE /Oy-
        #        PRIVATE /LDd
        #        PRIVATE /FC
        #        PRIVATE /EHsc
        #        PRIVATE /nologo
        PRIVATE /diagnostics:column
        )
target_compile_definitions(${PROJECT_NAME}_demaster_dll
        PRIVATE NOMINMAX # This will suppress the min and max definitions in Windef.h.
        PRIVATE GLEW_STATIC # Required to not need GLEW.dll
        #        PRIVATE WIN32
        #        PRIVATE _DEBUG
        #        PRIVATE FF8DEMASTER_EXPORTS
        #        PRIVATE _WINDOWS
        #        PRIVATE _USRDLL
        #        PRIVATE _WINDLL
        #        PRIVATE _UNICODE
        #        PRIVATE UNICODE
        #        PRIVATE _CRT_SECURE_NO_WARNINGS
        )
#set_target_properties(${PROJECT_NAME}_demaster_dll PROPERTIES CXX_STANDARD 14)
#set_target_properties(${PROJECT_NAME}_demaster_dll PROPERTIES C_STANDARD 99)