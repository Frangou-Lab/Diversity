//
//  StringStream.hpp
//  geneutils
//
//  Created by Denis Musatov on 02/11/16.
//  Copyright 2018 Frangou Lab. All rights reserved.
//

#ifndef StringStream_hpp
#define StringStream_hpp

#define BUFLEN_CPP 255

#include <string>

class StringStream {
protected:
    FILE *file_{nullptr};
    char buf_[BUFLEN_CPP + 1];
    int64_t pos_;
    int64_t read_;
    int64_t length_;
    std::string path_;

 public:
    StringStream() = default;
    explicit StringStream(const std::string& filePath);
    virtual ~StringStream();

    virtual std::string name() const;
    virtual std::string path() const;
    virtual int64_t position() const;
    virtual int64_t length() const;
};

#endif  // StringStream_hpp
