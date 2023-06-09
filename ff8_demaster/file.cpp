// ReSharper disable CppClangTidyModernizePassByValue
#include "file.h"

File::File(const std::string& path, const FileOpenMode mode) : filePath(path)
{
    Open(mode);
}

void File::Open(const FileOpenMode mode)
{
    if(fd.is_open())
        fd.close();
    if(filePath.length() == 0)
        return;
    std::ios::openmode openMode = std::ios::binary;
    if(mode == FileOpenMode::append)
        openMode |= std::ios::ate;
    else if(mode != FileOpenMode::read)
        openMode |= std::ios::trunc;
    openMode |= mode == FileOpenMode::read ? std::ios::in
    : mode==FileOpenMode::readWrite ? std::ios::in | std::ios::out : std::ios::out;

    fd.open(filePath, openMode);
    bOpened = fd.is_open();
}

void File::Close()
{
    if(fd.is_open())
        fd.close();
    bOpened = false;
}

int File::GetCurrentPosition() { if(fd.is_open()) return fd.tellp(); return -1; }

void File::WriteBuffer(const char* buffer, const int size)
{
    if(!fd.is_open())
        return;
    fd.write(buffer, size);
}


