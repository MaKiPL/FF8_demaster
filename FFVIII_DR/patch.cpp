#include "core.h"
#include "patch.h"

Patch::Patch(std::vector<unsigned char> payload, 
	std::vector<std::string> instructions, unsigned int address)
{
	this->payload = payload;
	this->patchInstructions = instructions;
	patches.push_back(*this);
	knownAddress = address;
}

Patch::Patch(unsigned char payload[], 
	std::vector<std::string> instructions, unsigned int address)
{
	this->payload = std::vector<unsigned char>(payload, payload + sizeof(payload));
	this->patchInstructions = instructions;
	patches.push_back(*this);
	knownAddress = address;
}
