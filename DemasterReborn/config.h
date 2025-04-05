#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include <map>

class Config
{
private:
	const std::filesystem::path defaultConfigPath = "DefaultConfig.cfg";
	const std::filesystem::path configPath = "Config.cfg";

	std::map <std::string, std::string> config;
	bool CheckValueExists(const std::string &key) const;
	void LoadConfig();

public:
	Config();

	bool GetBooleanValue(const std::string &key);
	std::string GetStringValue(const std::string &key);
	int GetIntValue(const std::string &key);
	float GetFloatValue(const std::string &key);
};