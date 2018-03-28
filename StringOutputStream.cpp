//
//  StringOutputStream.cpp
//  geneutils
//
//  Created by Denis Musatov on 07/11/16.
//  Copyright 2018 Frangou Lab. All rights reserved.
//

#include "StringOutputStream.hpp"

#include <stdexcept>
#include <cstdio>

static const char kNewlineSequence[] = "\n";

StringOutputStream::StringOutputStream(const std::string& fileName)
: StringStream(fileName)
{
    file_ = fopen(fileName.c_str(), "wt");
    if (!file_) {
        throw std::runtime_error("Couldn't open file " + fileName + " for writing.");
    }
    length_ = 0;
}

std::unique_ptr<StringOutputStream>
StringOutputStream::StreamWithFileName(const std::string& fileName)
{
    return std::make_unique<StringOutputStream>(fileName);
}

void StringOutputStream::WriteLine(const std::string& str)
{
    fputs(str.c_str(), file_);
    length_ += str.size();
    WriteLine();
}

void StringOutputStream::WriteLine()
{
    fputs(kNewlineSequence, file_);
    length_ += sizeof(kNewlineSequence) - 1 /* \0 - C-String ending length*/;
}

void StringOutputStream::WriteQuoted(const std::string& str)
{
    fputc('\"', file_);
    fputs(str.c_str(), file_);
    fputc('\"', file_);
    length_ += str.size() + 2;
}

void StringOutputStream::Write(const std::string& str)
{
    fputs(str.c_str(), file_);
    length_ += str.size();
}

void StringOutputStream::Write(char c)
{
    fputc(c, file_);
    ++length_;
}
