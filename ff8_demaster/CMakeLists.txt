
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
target_link_libraries(${PROJECT_NAME}_demaster_dll)
target_include_directories(${PROJECT_NAME}_demaster_dll
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/include/compat/msvc
        )
target_link_options(${PROJECT_NAME}_demaster_dll
        PRIVATE /FORCE:MULTIPLE
        PRIVATE /NODEFAULTLIB:MSVCRT
        PRIVATE /NODEFAULTLIB:MSVCRTD
        PRIVATE /NODEFAULTLIB:LIBCMT
        PRIVATE /DYNAMICBASE:NO
        PRIVATE /SUBSYSTEM:WINDOWS
        PRIVATE /NXCOMPAT
        PRIVATE /TLBID:1
        PRIVATE /MANIFESTUAC:NO
        )

target_link_directories(${PROJECT_NAME}_demaster_dll
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/lib
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/lib/bgfx
        )
target_link_libraries(${PROJECT_NAME}_demaster_dll
        PRIVATE StackWalker.lib
        PRIVATE glew32s.lib
        PRIVATE opengl32.lib
        PRIVATE psapi.lib
        PRIVATE bxDebug.lib
        PRIVATE bimgDebug.lib
        PRIVATE bimg_decodeDebug.lib
        PRIVATE bimg_encodeDebug.lib
        PRIVATE kernel32.lib
        PRIVATE user32.lib
        PRIVATE gdi32.lib
        PRIVATE winspool.lib
        PRIVATE comdlg32.lib
        PRIVATE advapi32.lib
        PRIVATE shell32.lib
        PRIVATE ole32.lib
        PRIVATE oleaut32.lib
        PRIVATE uuid.lib
        PRIVATE odbc32.lib
        PRIVATE odbccp32.lib
        )
set_target_properties(${PROJECT_NAME}_demaster_dll PROPERTIES OUTPUT_NAME "ff8_demaster")
target_compile_options(${PROJECT_NAME}_demaster_dll PRIVATE /Od /Ob0)
target_compile_definitions(${PROJECT_NAME}_demaster_dll PRIVATE
        NOMINMAX
        GLEW_STATIC
        WIN32
        _DEBUG
        FF8DEMASTER_EXPORTS
        _WINDOWS
        _USRDLL
        _CRT_SECURE_NO_WARNINGS
        #__STDC_LIMIT_MACROS
        #__STDC_FORMAT_MACROS
        #__STDC_CONSTANT_MACROS
        )
set_target_properties(${PROJECT_NAME}_demaster_dll PROPERTIES CXX_STANDARD 20)
set_target_properties(${PROJECT_NAME}_demaster_dll PROPERTIES C_STANDARD 11)