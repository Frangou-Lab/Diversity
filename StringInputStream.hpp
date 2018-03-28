//
//  StringInputStream.hpp
//  geneutils
//
//  Created by Denis Musatov on 02/11/16.
//  Copyright 2018 Frangou Lab. All rights reserved.
//

#ifndef LIBGENE_IO_STREAMS_STRINGINPUTSTREAM_HPP_
#define LIBGENE_IO_STREAMS_STRINGINPUTSTREAM_HPP_

#include <string>
#include <memory>

#include "StringStream.hpp"

class StringInputStream : public StringStream {
 public:
    virtual ~StringInputStream() = default;
    
    virtual std::string ReadLineAndAdd(char ch) = 0;
    virtual std::string ReadLine() = 0;
    virtual char PeekNextCharacter() = 0;
    virtual void ResetFilePointer() = 0;
    bool empty() const;
};

#endif  // LIBGENE_IO_STREAMS_STRINGINPUTSTREAM_HPP_
