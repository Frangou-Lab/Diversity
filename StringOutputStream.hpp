//
//  StringOutputStream.hpp
//  geneutils
//
//  Created by Denis Musatov on 07/11/16.
//  Copyright 2018 Frangou Lab. All rights reserved.
//

#ifndef LIBGENE_IO_STREAMS_STRING_OUTPUTSTREAM_HPP_
#define LIBGENE_IO_STREAMS_STRING_OUTPUTSTREAM_HPP_

#include <memory>

#include "StringStream.hpp"

class StringOutputStream : public StringStream {
 public:
    StringOutputStream(const std::string& fileName);
    static std::unique_ptr<StringOutputStream> StreamWithFileName(const std::string& fileName);
    
    void Write(const std::string& str);
    void WriteLine(const std::string& str);
    void WriteLine();
    void WriteQuoted(const std::string& str);
    void Write(char c);
};

#endif  // LIBGENE_IO_STREAMS_STRING_OUTPUTSTREAM_HPP_
