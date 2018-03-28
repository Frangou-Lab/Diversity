//
//  StringStreamCpp.cpp
//  geneutils
//
//  Created by Denis Musatov on 02/11/16.
//  Copyright 2018 Frangou Lab.
//

#include "StringStream.hpp"
#include "StringUtils.hpp"

#include <cstdio>

StringStream::StringStream(const std::string& filePath)
: path_(filePath), pos_(0), read_(0)
{
}

StringStream::~StringStream()
{
    if (file_) {
        fclose(file_);
        file_ = nullptr;
    }
}

int64_t StringStream::length() const
{
    return length_;
}

int64_t StringStream::position() const
{
    return ftell(file_) - read_ + pos_;
}

std::string StringStream::name() const
{
    return utils::GetLastPathComponent(path_);
}

std::string StringStream::path() const
{
    return path_;
}

