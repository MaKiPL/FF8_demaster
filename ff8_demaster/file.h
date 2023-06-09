// ReSharper disable CppClangTidyModernizePassByValue
#pragma once
#include <string>
#include <iostream>
#include <fstream>

enum class FileOpenMode
{
    read,
    write,
    append,
    readWrite,
};

class File
{
public:
    explicit File(const std::string& path) : filePath(path) {}
    explicit File(const std::string& path, FileOpenMode mode);

    void Open(FileOpenMode mode);
    void Close();
    
    [[nodiscard]] bool IsOpened() const { return bOpened; }
    [[nodiscard]] std::string GetPath() const { return filePath; }
    [[nodiscard]] int GetCurrentPosition();
    void Seek(int seekType, int offset);

    unsigned char ReadByte();
    char ReadChar();
    short ReadShort();
    unsigned short ReadUShort();
    int ReadInt();
    unsigned int ReadUInt();
    char* ReadBuffer(int size);

    void WriteByte(unsigned char value);
    void WriteChar(char value);
    void WriteShort(short value);
    void WriteUShort(unsigned short value);
    void WriteInt(int value);
    void WriteUInt(unsigned int value);
    void WriteBuffer(const char* buffer, int size);
    

    
    

private:
    std::string filePath;
    bool bOpened = false;
    std::ofstream fd;
};
