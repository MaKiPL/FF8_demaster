#pragma once

class Patch
{
public:
	/**
	 * \brief Enables and puts patch for registration
	 */
	Patch(std::vector<unsigned char> payload, 
		std::vector<std::string> instructions, unsigned int address = 0);
	Patch(unsigned char payload[], std::vector<std::string> instructions,
		unsigned int address = 0);
	unsigned int knownAddress;

private:
	std::vector<unsigned char> payload;
	std::vector<std::string> patchInstructions;
	bool bIsPatched;
	bool bisEnabled;
};
