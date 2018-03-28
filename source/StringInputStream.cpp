//
//  StringInputStreamCpp.cpp
//  geneutils
//
//  Created by Denis Musatov on 02/11/16.
//  Copyright 2018 Frangou Lab.
//

#include <memory>
#include <cassert>

#include "StringInputStream.hpp"
#include "PlainStringInputStream.hpp"
#include "StringUtils.hpp"

bool StringInputStream::empty() const
{
    // The second condition checks if we've reached the end of file, but still
    // have something in buffer
    return feof(file_) != 0 && pos_ == read_;
}

