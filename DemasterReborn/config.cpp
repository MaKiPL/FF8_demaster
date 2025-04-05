#include "config.h"
#include "log.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>

Config::Config()
{
	if (!exists(configPath) && exists(defaultConfigPath)) 
	{
		spdlog::warn("[CONFIG]\tCreating new config file!");
		copy(defaultConfigPath, configPath);
	}
	LoadConfig();
}

bool Config::CheckValueExists(const std::string &key) const
{
	if(!config.contains(key))
	{
		spdlog::error("[CONFIG] Key {} was not found in config!", key);
		return false;
	}
	return true;
}

void Config::LoadConfig()
{
	//Open config file
	std::ifstream file(configPath);
    if (!file.is_open())
    {
        spdlog::error("[CONFIG] Failed to open config file!");
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        const size_t pos = line.find('=');
        if (pos == std::string::npos)
            continue;

        std::string key = line.substr(0, pos);
        const std::string value = line.substr(pos + 1);
    	spdlog::info("[CONFIG]\t{} = {}", key, value);

        config[key] = value;
    }

    file.close();
    spdlog::info("[CONFIG] Config loaded successfully!");
}

bool Config::GetBooleanValue(const std::string &key)
{
	if (!CheckValueExists(key))
		return false;
	return config[key] == "true";
}

std::string Config::GetStringValue(const std::string &key)
{
	if (!CheckValueExists(key))
		return "NULL";
	return config[key];
}

int Config::GetIntValue(const std::string &key)
{
	if (!CheckValueExists(key))
		return 0;
	return std::stoi(config[key]);
}

float Config::GetFloatValue(const std::string &key)
{
	if (!CheckValueExists(key))
		return 0.0f;
	return std::stof(config[key]);
}