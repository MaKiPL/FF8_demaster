#include "language.h"

Language::Language(const std::filesystem::path &path)
{
	this->path = path;
	read_file();
}

Language::Language()
{
	read_file();
}

void Language::read_file()
{
	if(!std::filesystem::exists(path))
		throw new std::runtime_error("Language file not found");

	std::ifstream file(path);

}