#pragma once
#include <string>

//Based on https://github.com/julianxhokaxhiu/FFNx/blob/290759fa6509e2a6f822a7d3c048984dd8d4ea5a/src/utils.h#L62


// trim from start (in place)
inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::ranges::find_if(s, [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}
inline void Trim(std::string &s)
{
    ltrim(s);
    rtrim(s);
}

inline std::vector<std::string> Split(std::string s, const std::string& delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        std::string token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

inline bool Contains (const std::string& s, const std::string& token)
{
    return s.find(token) != std::string::npos;
}

inline bool ReplaceOnce(std::string& str, const std::string& from, const std::string& to)
{
    const size_t startPos = str.find(from);

    if (startPos == std::string::npos)
        return false;

    str.replace(startPos, from.length(), to);

    return true;
}