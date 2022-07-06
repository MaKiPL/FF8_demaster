#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include <map>

class Config
{
private:
	const std::filesystem::path path = "config.xml";

	std::map <std::string, std::string> config;
	bool check_value_exists(const std::string &key);

public:
	Config();

	bool GetBooleanValue(const std::string &key);
	std::string GetStringValue(const std::string &key);
	int GetIntValue(const std::string &key);
	float GetFloatValue(const std::string &key);
};