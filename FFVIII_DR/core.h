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
https://github.com/gabime/spdlog
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




///=====END OF INCLUDES======///

//patch
