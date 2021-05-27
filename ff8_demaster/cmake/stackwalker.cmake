include(FetchContent)
#options for FetchContent are at https://cmake.org/cmake/help/latest/module/ExternalProject.html
FetchContent_Declare(
        stackwalker_fetch
        GIT_REPOSITORY https://github.com/JochenKalmbach/StackWalker.git
        GIT_TAG origin/master
        GIT_REMOTE_UPDATE_STRATEGY CHECKOUT
)
#FetchContent_MakeAvailable(StackWalker)
FetchContent_GetProperties(stackwalker_fetch)
if (NOT stackwalker_fetch_POPULATED)
    FetchContent_Populate(stackwalker_fetch)
    #add_subdirectory(${stackwalker_fetch_SOURCE_DIR} ${stackwalker_fetch_BINARY_DIR} EXCLUDE_FROM_ALL)
    #their cmakelists.txt isn't friendly to including via fetch content.-
    add_library(stackwalker_paths STATIC
            ${stackwalker_fetch_SOURCE_DIR}/Main/StackWalker/StackWalker.cpp
            )
    target_include_directories(stackwalker_paths
            PUBLIC ${stackwalker_fetch_SOURCE_DIR}/Main/StackWalker
            )
    set_target_properties(stackwalker_paths PROPERTIES
            MSVC_RUNTIME_LIBRARY "MultiThreadedDebug" # cl : Command line warning D9025 : overriding '/MD' with '/MTd'
            )
endif ()