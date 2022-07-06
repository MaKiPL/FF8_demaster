#pragma once

#define MAINEXE "FFVIII.exe"
#define EFIGSDLL "FFVIII_EFIGS.dll"
#define JPDLL "FFVIII_JP.dll"

//standard
#include <iostream>
#include <string>
#include <filesystem>

//internal
#include "config.h"
std::unique_ptr<Config> config;
#include "language.h"
std::unique_ptr<Language> language;

//vcpkg
#include "log.h"
#include <pugixml.hpp>
