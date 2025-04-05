#pragma once

#define MAINEXE "FFVIII.exe"
#define EFIGSDLL "FFVIII_EFIGS.dll"
#define JPDLL "FFVIII_JP.dll"

//standard
#include <iostream>
#include <string>
#include <filesystem>
#include <Windows.h>

inline std::string FF8DLL;
inline DWORD IMAGEBASE = 0x1000000;

//internal
#include "config.h"
inline std::unique_ptr<Config> CONFIG;

//vcpkg
#include "log.h"
