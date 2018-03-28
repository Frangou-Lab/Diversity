/*
 * Copyright 2018 Frangou Lab
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
