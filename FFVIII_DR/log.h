#pragma once
#include <string>

inline std::string GetTimestamp();

class Log
{
public:
	void LogMessage(const char* msg) const;


private:
	void output_string(std::string msg, const int colorIndex = 0) const;
};
