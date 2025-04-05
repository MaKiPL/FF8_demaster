#include "zzz.h"
#include <filesystem>
#include <fstream>

#include "dllmain.h"
#include "log.h"

zzz::zzz(std::string filename, bool forceUnpack)
{
    if(!std::filesystem::exists(filename))
    {
        spdlog::error("[ZZZ] File not found: {}", filename);
        return;
    }

    spdlog::info("[ZZZ] Opening {}...", filename);
    std::ifstream file(filename, std::ios::binary);

    ReadBinary(file, fileCount);

    // Read entries 
    entries = std::vector<zzzEntry>(fileCount);
    for(auto& [fileNameLen, fileName, fileOffset, fileSize] : entries)
    {
        ReadBinary(file, fileNameLen);
        fileName.resize(fileNameLen);
        file.read(fileName.data(), fileNameLen);

        ReadBinary(file, fileOffset);
        ReadBinary(file, fileSize);
    }
    spdlog::info("[ZZZ] {} entries found.", fileCount);

    // Unpack entry if needed 
    for (const auto& entry : entries)
    {
        if(!std::filesystem::exists(EXPORTPATH + entry.fileName) || forceUnpack)
        {
            spdlog::info("[ZZZ] Extracting {}...", entry.fileName);
            create_directories(std::filesystem::path(EXPORTPATH + entry.fileName).parent_path());
            
            std::ofstream outputFile(EXPORTPATH + entry.fileName, std::ios::binary);
            
            file.seekg(static_cast<std::streamoff>(entry.fileOffset));
            
            std::vector<char> buffer(entry.fileSize);
            file.read(buffer.data(), entry.fileSize);
            
            outputFile.write(buffer.data(), entry.fileSize);
            outputFile.close();
        }
    }

    file.close();
}
