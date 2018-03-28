//
//  Splitter.hpp
//  geneutils
//
//  Created by Denis Musatov on 08/11/16.
//  Copyright 2018 Frangou Lab. All rights reserved.
//

#ifndef Splitter_hpp
#define Splitter_hpp

#include <string>
#include <string_view>

class Splitter {
 private:
    std::string_view expression_;
    int64_t position_;
    const char delimiter_;
    int64_t lexeme_start_pos_;
    int64_t lexeme_length_;
    std::string current_lexeme_;
    
    void FormLexeme_(int64_t pos);
    void TrimLexeme_();
    
 public:
    explicit Splitter(char delimiter);
    Splitter(std::string_view exp_view, char delimiter);
    Splitter(std::string_view exp, std::string delimiter);
    void SetExpression(std::string_view exp_view);
    bool ReadNext();
    
    // Allows to read the value only once!
    std::string&& GetNextValue();
};

#endif  // Splitter_hpp
