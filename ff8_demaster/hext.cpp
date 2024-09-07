/****************************************************************************/
//    Copyright (C) 2009 Aali132                                            //
//    Copyright (C) 2018 quantumpencil                                      //
//    Copyright (C) 2018 Maxime Bacoux                                      //
//    Copyright (C) 2020 myst6re                                            //
//    Copyright (C) 2020 Chris Rizzitello                                   //
//    Copyright (C) 2020 John Pritchard                                     //
//    Copyright (C) 2024 Julian Xhokaxhiu                                   //
//                                                                          //
//    This file is part of FFNx                                             //
//                                                                          //
//    FFNx is free software: you can redistribute it and/or modify          //
//    it under the terms of the GNU General Public License as published by  //
//    the Free Software Foundation, either version 3 of the License         //
//                                                                          //
//    FFNx is distributed in the hope that it will be useful,               //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of        //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         //
//    GNU General Public License for more details.                          //
/****************************************************************************/

//This file was modified from the original source code.

#include "hext.h"
#include "string_helper.h"
#include "coreHeader.h"
#include <filesystem>
#include <fstream>

Hext hextPatcher;

// PRIVATE

int Hext::GetAddress(const std::string& token) const
{
    std::vector<std::string> sparts = Split(token, "+");
    std::vector<int> iparts;

    if (sparts[0].ends_with("^"))
    {
        std::stringstream ss;
        int *ptr = reinterpret_cast<int*>(std::stoi(sparts[0].substr(0, sparts[0].length() - 1), nullptr, 16) + inGlobalOffset);
        ss << std::hex << *ptr;
        sparts[0] = ss.str();
    }

    for (auto &part : sparts)
        iparts.push_back(std::stoi(part, nullptr, 16));

    int ret = iparts[0];

    if (Contains(token, "+"))
        ret += iparts[1];
    else if (Contains(token, "-"))
        ret -= iparts[1];

    return ret + inGlobalOffset;
}

std::vector<char> Hext::getBytes(const std::string& token)
{
    std::vector<char> ret;

    if (Contains(token, ":"))
    {
        std::vector<std::string> parts = Split(token, ":");
        int count = std::stoi(parts[1], nullptr, 0);
        while (count > 0)
        {
            ret.push_back(std::stoi(parts[0], nullptr, 16));
            count--;
        }
    }
    else
    {
        std::vector<std::string> bytes = Split(token, " ");
        for (const auto& byte : bytes)
        {
            std::string trimmedByte = byte;
            Trim(trimmedByte);
            if (trimmedByte.empty())
                continue;
            ret.push_back(std::stoi(byte, nullptr, 16));
        }
    }

    return ret;
}

bool Hext::hasCheckpoint(const std::string& token)
{
    return token.starts_with("!");
}

bool Hext::parseCheckpoint(std::string token, std::string checkpoint)
{
    if (token.starts_with("!"))
        if (Contains(token, checkpoint))
            return true;

    return false;
}

bool Hext::ParseCommands(std::string token)
{
    if (token.starts_with("<<"))
    {
        ReplaceOnce(token, "<<", "");

        Trim(token);

        return true;
    }

    return false;
}

bool Hext::ParseComment(const std::string& token)
{
    if (isMultilineComment)
    {
        if (token.ends_with("}}")) isMultilineComment = false;
        return true;
    }

    if (token.starts_with("{{"))
    {
        isMultilineComment = true;
        return true;
    }

    if (token.starts_with("#")) return true;
    if (token.starts_with("{")) return true;
    if (token.starts_with(".")) return true;

    return false;
}

bool Hext::ParseGlobalOffset(const std::string& token)
{
    if (token.starts_with("+"))
    {
        inGlobalOffset = std::stoi(token.substr(1), nullptr, 16);

        return true;
    }
    if (token.starts_with("-"))
    {
        inGlobalOffset = -std::stoi(token.substr(1), nullptr, 16);

        return true;
    }

    return false;
}

bool Hext::parseMemoryPermission(const std::string& token) const
{
    if (Contains(token, ":"))
    {
        DWORD dummy;

        std::vector<std::string> parts = Split(token, ":");
        const int addr = GetAddress(parts[0]);
        const int length = std::stoi(parts[1], nullptr, 16);

        VirtualProtect(reinterpret_cast<LPVOID>(addr), length, PAGE_EXECUTE_READWRITE, &dummy);

        return true;
    }

    return false;
}

bool Hext::ParseMemoryPatch(const std::string& token) const
{
    if (Contains(token, "="))
    {
        std::vector<std::string> parts = Split(token, "=");
        int addr = GetAddress(parts[0]);
        std::vector<char> bytes = getBytes(parts[1]);

        DWORD dummy;
        ModPage(addr, bytes.size());

        VirtualProtect(reinterpret_cast<void*>(addr), reinterpret_cast<SIZE_T>(bytes.data()),
            PAGE_EXECUTE_READWRITE, &dummy);

        memcpy(reinterpret_cast<void*>(addr), bytes.data(), bytes.size());

        return true;
    }

    return false;
}

// PUBLIC

void Hext::Apply(const std::string& filename)
{
    std::string line;
    std::ifstream ifs(filename);

    while (std::getline(ifs, line))
    {
        if (line.empty()) continue;

        // Check if delayed, if so it should not be applied
        if (hasCheckpoint(line)) {
            ifs.close();
            return;
        }

        // Check if is a comment
        if (ParseComment(line)) continue;

        // Check if is a command
        if (ParseCommands(line)) continue;

        // Check if is a global offset
        if (ParseGlobalOffset(line)) continue;

        // Check if is a memory permission range
        if (parseMemoryPermission(line)) continue;

        // Check if is a memory patch instruction
        if (ParseMemoryPatch(line)) continue;
    }

    ifs.close();
}

void Hext::ApplyDelayed(const std::string& filename, const std::string& checkpoint)
{
    std::string line;
    std::ifstream ifs(filename);

    bool matchCheckpoint = false;

    while (std::getline(ifs, line))
    {
        if (line.empty()) continue;

        // Check if is a comment
        if (ParseComment(line)) continue;

        // Check if is a delayed patch.
        if (parseCheckpoint(line, checkpoint))
        {
            matchCheckpoint = true;
            continue;
        }

        if (matchCheckpoint)
        {
            // Check if is a command
            if (ParseCommands(line)) continue;

            // Check if is a global offset
            if (ParseGlobalOffset(line)) continue;

            // Check if is a memory permission range
            if (parseMemoryPermission(line)) continue;

            // Check if is a memory patch instruction
            if (ParseMemoryPatch(line)) continue;
        }
        else
            break;
    }

    ifs.close();
}

void Hext::ApplyAll(const std::string& checkpoint)
{
    std::filesystem::path hext_patching_path(HEXT_PATH);
    if(!std::filesystem::exists(hext_patching_path))
    {
        OutputDebug("Hext::applyAll: There's no 'HEXT' folder");
        return;
    }
    if (std::filesystem::exists(hext_patching_path))
        if (!checkpoint.empty())
        {
            for (const auto& entry : std::filesystem::directory_iterator(hext_patching_path))
            {
                if (entry.is_regular_file())
                    ApplyDelayed(entry.path().string(), checkpoint);
                inGlobalOffset = 0;
            }
        }
        else
        {
            for (const auto& entry : std::filesystem::directory_iterator(hext_patching_path))
            {
                if (entry.is_regular_file())
                    Apply(entry.path().string());
                inGlobalOffset = 0;
            }
        }
}

