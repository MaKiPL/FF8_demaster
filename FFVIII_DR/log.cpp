#include <iostream>
#define OOF_IMPL
#include "log.h"

void Log::output_string(const std::string& msg, logtype logType) const
{
	std::stringstream sstream;
	std::string separator;
	oof::color color;
	switch (logType)
	{
	default:
	case logtype::information:
		separator = INFO;
		color = Colors::white;
		break;
	case logtype::warning:
		separator = WARN;
		color = Colors::yellow;
		break;
	case logtype::error:
		separator = ERRO;
		color = Colors::red;
		break;
	case logtype::ok:
		separator = OKAY;
		color = Colors::green;
		break;
	}
	std::string timestamp = GetTimestamp();
	sstream << oof::underline(true) << oof::fg_color(color) << \
		timestamp << oof::underline(false) << oof::bold(true) << \
		separator << oof::bold(false) << msg << std::endl;

	log_messages->emplace_back(sstream.str());
	std::cout << sstream.str();
	
}


void Log::LogMessage(const char* msg) const
{
	output_string(msg, logtype::information);
}

void Log::LogWarning(const char* msg) const
{
	output_string(msg, logtype::warning);
}

void Log::LogError(const char* msg) const
{
	output_string(msg, logtype::error);
}

void Log::LogOk(const char* msg) const
{
	output_string(msg, logtype::ok);
}

Log::Log()
{

}


Log::~Log()
{
	
}

