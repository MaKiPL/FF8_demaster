#include <iostream>
#define OOF_IMPL
#include "log.h"


void Log::output_string(const std::string& msg, const oof::color colorIndex) const
{
	std::cout << fg_color(colorIndex);
	std::cout << msg << std::endl;
}

inline void Log::output_timestamp()
{
	std::cout << oof::underline() << GetTimestamp() << oof::underline(false);
}

inline void Log::output_specifier(const std::string& spec)
{
	std::cout << oof::bold() << spec << ": " << oof::bold(false);
}


void Log::LogMessage(const char* msg) const
{
	std::cout << fg_color(Colors::white);
	output_timestamp();
	output_specifier("MESSAGE");
	output_string(msg);
}

void Log::LogWarning(const char* msg) const
{
	std::cout << fg_color(Colors::yellow);
	output_timestamp();
	output_specifier("WARNING");
	output_string(msg);
}

void Log::LogError(const char* msg) const
{
	std::cout << fg_color(Colors::red);
	output_timestamp();
	output_specifier("ERROR");
	output_string(msg);
}

Log::Log()
{
	
}


Log::~Log()
{
	
}
