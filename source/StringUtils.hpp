//
//  StringUtils.cpp
//  libgene
//
//  Created by Denis Musatov on 11/10/17.
//  Copyright 2018 Frangou Lab.
//


#ifndef StringUtils_hpp
#define StringUtils_hpp

#include <string>
#include <algorithm>
#include <vector>

#define BEGIN_NAMESPACE_LIBGENE_UTILS_ namespace utils {
#define END_NAMESPACE_LIBGENE_UTILS_ }

BEGIN_NAMESPACE_LIBGENE_UTILS_

std::string InsertSuffixBeforePathExtension(std::string path, std::string suffix);

bool HasExtension(const std::string& path, const std::string ext);

std::string GetExtension(const std::string& str);
std::string GetLastPathComponent(const std::string& str);

void ReplaceOccurrencesOfString(std::string& targetStr, std::string what, std::string withWhat);
std::string StringByDeletingPathExtension(const std::string& str);

bool StringContainsCharacter(const std::string& src, char ch);
std::string UppercaseString(const std::string& str);
std::string LowercaseString(const std::string& str);

void CommaEscapeString(std::string& stringToEscape);

// Returns a string value of 'number' padded with leading zeroes.
// For example: 'number' = 17, length = 4
// Result: "0017"
std::string PaddedToLengthString(int number, int length);


END_NAMESPACE_LIBGENE_UTILS_

#endif  // StringUtils_hpp
