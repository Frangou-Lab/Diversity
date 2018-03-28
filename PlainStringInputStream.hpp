//
//  PlainStringInputStreamCpp.hpp
//  geneutils
//
//  Created by Denis Musatov on 02/11/16.
//  Copyright 2018 Frangou Lab. All rights reserved.
//

#ifndef PlainStringInputStream_hpp
#define PlainStringInputStream_hpp

#include <string>

#include "StringInputStream.hpp"

class PlainStringInputStream final : public StringInputStream {
 public:
    PlainStringInputStream() = default;
    explicit PlainStringInputStream(const std::string& file_path);
    ~PlainStringInputStream() = default;

    std::string ReadLineAndAdd(char ch) override;
    std::string ReadLine() override;
    char PeekNextCharacter() override;
    void ResetFilePointer() override;
    explicit operator bool() const;
};

#endif  // PlainStringInputStream_hpp
