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

#include "PlainStringInputStream.hpp"
#include "StringStream.hpp"

#include <algorithm>
#include <stdexcept>

PlainStringInputStream::PlainStringInputStream(const std::string& file_path)
{
    path_ = file_path;
    pos_ = 0;
    read_ = 0;
    file_ = fopen(file_path.c_str(), "rt");
    if (file_) {
        fseek(file_, 0L, SEEK_END);
        length_ = ftell(file_);
        fseek(file_, 0L, SEEK_SET);
    }
}

std::string PlainStringInputStream::ReadLineAndAdd(char ch)
{
    std::string ret_str;
    ret_str.reserve(100);
    do {
        if (pos_ < read_) {  // Have something in buf
            char *p1 = strchr(buf_ + pos_, '\r');
            char *p2 = strchr(buf_ + pos_, '\n');
            if (p1 || p2) {  // Found string
                char *p;
                if (p1 && p2)
                    p = std::min(p1, p2);
                else if (p1)
                    p = p1;
                else
                    p = p2;
                
                *p = 0;
                std::string ret(buf_ + pos_);
                // eat all 0xd, 0xa
                pos_ = (int)(p-buf_)+1;
                for (; pos_ < read_; pos_++) {
                    if (buf_[pos_] == 0xd || buf_[pos_] == 0xa)
                        buf_[pos_] = 0;
                    else
                        break;
                }
                
                ret_str.append(ret);
                
                if (ret_str.empty())
                    continue;
                
                if (ch)
                    ret_str += static_cast<char>(ch);
                
                return ret_str;
            }
            // Add readed to ret and continue
            ret_str.append(buf_ + pos_);
        }
        // Read more data
        pos_ = 0;
        read_ = (int)fread(buf_, 1, BUFLEN_CPP, file_);
        buf_[read_] = 0; // EOL
    } while (read_);
    
    if (ret_str.empty()) // last
        return "";
    
    if (ch)
        ret_str += ch;
    
    return ret_str;
}

char PlainStringInputStream::PeekNextCharacter()
{
    if (pos_ < read_) {
        // Have something in buffer
        return buf_[pos_];
    } else {
        char c;
        int64_t current_position = ftell(file_);
        int64_t read = fread(&c, 1, 1, file_);
        if (read == 1)
            fseek(file_, current_position, SEEK_SET); // Roll back one character

        return c;
    }
    return EOF;
}

std::string PlainStringInputStream::ReadLine()
{
   return this->ReadLineAndAdd('\0');
}

void PlainStringInputStream::ResetFilePointer()
{
    fseek(file_, 0L, SEEK_SET);
    pos_ = 0;
    read_ = 0;
    std::memset(buf_, 0, BUFLEN_CPP);
}

PlainStringInputStream::operator bool() const
{
    return (file_ != nullptr) && (ftell(file_) < length_);
}

