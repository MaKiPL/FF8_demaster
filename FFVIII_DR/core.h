#pragma once
#define DRVERSION "0.0"
#define ENGINEVER "FFVIII Remastered 1.0.3.0 WIN32"

//STD
#include <iostream>
#include <vector>


/**
 * \brief Windows basic includes
 */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

		//THIRDPARTY
//https://github.com/gabime/spdlog
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#define LOGNAME "demaster logger"
#define LOGFILENAME "dr_log.txt"
#define LOGFILESIZE 1048576
#define LOGFILECNT 3
#define DR_CREATE_LOGGER spdlog::rotating_logger_mt(LOGNAME,			\
	LOGFILENAME,LOGFILESIZE, LOGFILECNT); spdlog::info(					\
	"Demaster Reborn v. {}\t Build date: {} {}\tfor: {}\n \tBy Maki",	\
	DRVERSION, __DATE__, __TIME__, ENGINEVER);

#include <MinHook.h>


///=====END OF INCLUDES======///
enum gameVersions : int {v1000,v1010,v1020,v1030,};
enum dllLangVersions : int { EFIGS, JP, OTHER };
enum ingameLanguage : int {
	ENGLISH, FRENCH_, ITALIAN, GERMAN, SPANISH, MOD_POLISH,
	MOD_UKRAINIAN, MOD_HUNGARIAN, MOD_PLACEHOLDER, JAPANESE = 10,
	MOD_CHINESE
};

//patch
#include "patch.h"
std::vector<Patch> patches;