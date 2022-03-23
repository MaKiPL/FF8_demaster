#include "log.h"

#define OOF_IMPL
#include <oof.h>

#include <iostream>
#include <chrono>

#include <cctz/time_zone.h>

inline std::string GetTimestamp()
{
	return format("%Y%m%d-%H:%M:%S",
	              std::chrono::system_clock::now(), cctz::local_time_zone());
}

void Log::output_string(const std::string msg, const int colorIndex) const
{
	std::cout << oof::fg_color(colorIndex);
	std::cout << msg;
}


void Log::LogMessage(const char* msg) const
{
	Log::output_string(GetTimestamp());
	Log::output_string("\tMESSAGE: ");
	Log::output_string(msg, 0);
}