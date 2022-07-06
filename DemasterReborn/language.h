#pragma once
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Language
{
private:
	std::filesystem::path path = "language.xml";
	bool bRead = false;

	void read_file();

public:
	std::string defaultLanguage = "en"; //this value is read from config

	Language();
	Language(const std::filesystem::path &path);


};