#include "hext.h"
#include "coreHeader.h"
#include "string_helper.h"
#include <filesystem>


bool InjectHext()
{
    std::filesystem::path path(HEXT_PATH);
    if(!std::filesystem::exists(path))
    {
        OutputDebug("Hext::InjectHext - \"HEXT\" folder not found.\n");
        return false;
    }
    for(const std::filesystem::directory_iterator dirIt(path); const auto& n : dirIt)
        if(n.is_regular_file() && n.path().extension() == ".txt")
            ReadAndInjectHext(n.path().string());
    return false;
}

void ReadAndInjectHext(std::string filename)
{
    std::ifstream file(filename);
    if(!file.is_open())
    {
        OutputDebug("Hext::ReadAndInjectHext - Failed to open file %s.\n", filename.c_str());
        return;
    }
    OutputDebug("Hext::ReadAndInjectHext - Injecting hex values from %s...\n", filename.c_str());
    std::string line;
    while(std::getline(file, line))
    {
        //trim whitespaces
         trim(line);
        if(line.empty() || line.starts_with("#"))
            continue;
        //if line contains = then skip
        if(line.starts_with('+'))
            ParseMemoryOffset(line);
        if(line.find('=') != std::string::npos)
            ParseMemoryOverwrite(line);
        else
        {
            if(line.find(':') != std::string::npos)
                ParseMemoryPermission(line);
            if(line.starts_with('+'))
                ParseMemoryOffset(line);
        }
    }
}

void ParseMemoryPermission(std::string& line)
{
    const std::vector<std::string> tokens = split(line, ":");
    if(tokens.size() != 2)
        return;
    const int baseAddr = std::stoi(tokens[0]);
    const int count = std::stoi(tokens[1]);
    if(count <= 0)
        return;
    //ModPage(baseAddr, count); //do we really need this?
}

void ParseMemoryOffset(std::string& line)
{
    const std::string offset = split(line, "+")[1];
    memoryOffset = std::stoi(offset, nullptr, 16);
}

void ParseMemoryOverwrite(std::string& line)
{
    std::vector<std::string> tokens = split(line, "=");
    std::string offset = tokens[0];
    trim(offset);
    int baseAddr = std::stoi(offset);
    baseAddr += memoryOffset;
    std::string values = tokens[1];
    trim(values);
    std::vector<std::string> byteValues = split(values, " ");
    for(std::string val : byteValues)
    {
        trim(val);
        ModPage(baseAddr, 1); 
        *reinterpret_cast<char*>(baseAddr) = static_cast<char>(std::stoi(val));
        baseAddr++;
    }
}
