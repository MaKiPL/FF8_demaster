#pragma once
#include <fstream>
#include <string>
#include <vector>

template<typename T>
void ReadBinary(std::ifstream& file, T& value)
{
    file.read(reinterpret_cast<char*>(&value), sizeof(T));
}


class zzz
{
    struct zzzEntry
    {
        uint32_t fileNameLen;
        std::string fileName;
        uint64_t fileOffset;
        uint32_t fileSize;
    };
    
public:
    explicit zzz(std::string filename, bool forceUnpack = false);

private:

    
    uint32_t fileCount;
    std::vector<zzzEntry> entries;
};
