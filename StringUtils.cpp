//
//  StringUtils.cpp
//  libgene
//
//  Created by Denis Musatov on 11/10/17.
//  Copyright 2018 Frangou Lab. All rights reserved.
//

#include <cassert>
#include <algorithm>
#include <locale>
#include <cctype>
#include <dirent.h>

#include "StringUtils.hpp"

BEGIN_NAMESPACE_LIBGENE_UTILS_

std::string InsertSuffixBeforePathExtension(std::string path, std::string suffix)
{
    return StringByDeletingPathExtension(path) + suffix + '.' + GetExtension(path);
}

bool HasExtension(const std::string& path, const std::string ext)
{
    const int64_t size = path.size();
    const int64_t extSize = ext.size();
    std::string extension = path.substr(std::max(size - extSize - 1, 0ll), extSize + 1);
    std::for_each(extension.begin(), extension.end(), [](char& c){
        return std::tolower(c, std::locale());
    });
    return (size >= extSize + 1 && extension == "." + ext);
}

std::string GetExtension(const std::string& str)
{
    int64_t pointPosition = str.rfind('.');

    if (str.empty() || pointPosition == std::string::npos || pointPosition + 1 == str.size())
        return "";

    return str.substr(pointPosition + 1);
}

std::string GetLastPathComponent(const std::string& str)
{
    char path_delimiter = '/';
#ifdef _MSC_VER
    path_delimiter = '\\';
#endif
    int64_t slash_position = str.rfind(path_delimiter);
    if (str.empty() || slash_position == std::string::npos)
        return str;

    return str.substr(slash_position + 1);
}

void ReplaceOccurrencesOfString(std::string& targetStr, std::string what, std::string withWhat)
{
    std::string::size_type start = 0;
    while ((start = targetStr.find(what, start)) != std::string::npos) {
        auto occurence = targetStr.begin() + start;
        targetStr.replace(occurence, occurence + what.size(), withWhat);
        start += withWhat.size();
    }
}

bool StringContainsCharacter(const std::string& src, char ch)
{
    return src.find(ch) != std::string::npos;
}

std::string UppercaseString(const std::string& str)
{
    std::string uppercase = str;
    std::for_each(uppercase.begin(), uppercase.end(), [](char& c){
        c = std::toupper(c);
    });
    return uppercase;
}

std::string LowercaseString(const std::string& str)
{
    std::string lowercase = str;
    std::for_each(lowercase.begin(), lowercase.end(), [](char& c){
        return (char)std::tolower(c);
    });
    return lowercase;
}

std::string StringByDeletingPathExtension(const std::string& str)
{
    std::string::size_type dotPosition = str.rfind('.');
    if (dotPosition == std::string::npos)
        return str;

    return str.substr(0, dotPosition);
}

void CommaEscapeString(std::string& stringToEscape)
{
    if (stringToEscape.find('"') == std::string::npos)
        return;

    auto iter = stringToEscape.begin();
    while (iter != stringToEscape.end()) {
        if (*iter == '"') {
            iter = stringToEscape.insert(iter, '\"');
            ++iter;
        }
        ++iter;
    }
}

std::string PaddedToLengthString(int number, int length)
{
    assert(length < 10);
    char buffer[10];
    snprintf(buffer, length + 1, ("%0" + std::to_string(length) + "d").c_str(), number);
    return std::string(buffer);
}

END_NAMESPACE_LIBGENE_UTILS_
