#include "config.h"
#include "log.h"

#include <filesystem>
#include <iostream>
#include <pugixml.hpp>
#include <map>


//unused - rather just do reinterpret 
template <typename T>
T get_value(pugi::xml_node node)
{
	switch(std::string(node.attribute("type").value()))
	{
	case "string": return std::string(node.text().as_string());
	case "bool": return std::string(node.text().as_bool(false));
	case "int": return std::string(node.text().as_int());
	case "float": return std::string(node.text().as_float());
	}
}

Config::Config()
{
	if (!std::filesystem::exists(path)) 
	{
		spdlog::warn("[CONFIG]\tCreating new config file!");
		std::filesystem::copy("defaultConfig.xml", "config.xml");
	}

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(path.c_str());
	if (!result)
		throw new std::exception("Failed to load config file");

	pugi::xml_node root = doc.child("config");
	for(pugi::xml_node configValues : root)
	{
		config.emplace(
			std::make_pair(configValues.name(),
				configValues.text().as_string()));
				//get_value(configValues)));
		spdlog::info("[CONFIG]\t{}: {}", configValues.name(),
			configValues.text().as_string());
	}
}

bool Config::check_value_exists(const std::string &key)
{
	if(!config.contains(key))
	{
		spdlog::error("[CONFIG] Key {} was not found in config!", key);
		return false;
	}
	return true;
}

bool Config::GetBooleanValue(const std::string &key)
{
	if (!check_value_exists(key))
		return false;
	return config[key].compare("true");
}

std::string Config::GetStringValue(const std::string &key)
{
	if (!check_value_exists(key))
		return "NULL";
	return config[key];
}

int Config::GetIntValue(const std::string &key)
{
	if (!check_value_exists(key))
		return 0;
	return std::stoi(config[key]);
}

float Config::GetFloatValue(const std::string &key)
{
	if (!check_value_exists(key))
		return 0.0f;
	return std::stof(config[key]);
}