include(FetchContent)
#options for FetchContent are at https://cmake.org/cmake/help/latest/module/ExternalProject.html
FetchContent_Declare(
        glew_fetch
        #GIT_REPOSITORY https://github.com/nigels-com/glew.git # do not reference this directly requires generating source fork does that for us.
        #GIT_TAG origin/master
        GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
        GIT_TAG origin/master
        GIT_REMOTE_UPDATE_STRATEGY CHECKOUT
)
#FetchContent_MakeAvailable(glew)
FetchContent_GetProperties(glew_fetch)
if (NOT glew_fetch_POPULATED)
    FetchContent_Populate(glew_fetch)
    add_subdirectory(${glew_fetch_SOURCE_DIR} ${glew_fetch_BINARY_DIR} EXCLUDE_FROM_ALL)
endif ()