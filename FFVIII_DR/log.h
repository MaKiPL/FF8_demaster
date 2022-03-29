#pragma once
#include <string>
#include <cctz/time_zone.h>
#include <oof.h>

#define LOG_FILE_NAME "DRlog.txt"


inline std::string GetTimestamp()
{
	return format("%Y%m%d_%H:%M:%S",
		std::chrono::system_clock::now(), cctz::local_time_zone());
}

#define DEFINE_COLOR(name, r, g, b) \
	inline static constexpr oof::color name{r, g, b}
class Colors
{
public:
	DEFINE_COLOR(white, 255, 255, 255);
	DEFINE_COLOR(black, 0, 0, 0);
	DEFINE_COLOR(red, 255, 0, 0);
	DEFINE_COLOR(green, 0, 255, 0);
	DEFINE_COLOR(blue, 0, 0, 255);
	DEFINE_COLOR(yellow, 255, 255, 0);
	DEFINE_COLOR(cyan, 0, 255, 255);
	DEFINE_COLOR(magenta, 255, 0, 255);

};

class Log
{
public:
	/**
	 * \brief Outputs default log message with white color and no error flag 
	 * \param msg Message to display
	 */ 
	void LogMessage(const char* msg) const;
	void LogError(const char* msg) const;
	void LogWarning(const char* msg) const;
	Log();
	~Log();


private:
	void output_string(const std::string& msg, oof::color colorIndex = oof::color{255,255,255}) const;
	static inline void output_timestamp();
	static inline void output_specifier(const std::string& spec);
	std::vector<std::string> log_messages = std::vector<std::string>(64); //64 messsages buffer
};
