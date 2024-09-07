#pragma once
#include <string>

/*
 * Basic HEXT format implementation
 * HEXT tools are created by Daniel L. P. Burke
 * D71983@gmail.com
*/

inline int memoryOffset;

bool InjectHext();
void ReadAndInjectHext(std::string filename);
void ParseMemoryPermission(std::string& line);
void ParseMemoryOffset(std::string& line);
void ParseMemoryOverwrite(std::string& line);