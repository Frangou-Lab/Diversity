//
//  Splitter.cpp
//  libgene
//
//  Created by Denis Musatov on 08/11/16.
//  Copyright 2018 Frangou Lab.
//

#include "Splitter.hpp"
#include "StringUtils.hpp"

#include <cassert>
#include <type_traits>

Splitter::Splitter(std::string_view exp, char delimiter)
: Splitter::Splitter(delimiter)
{
    SetExpression(exp);
}

Splitter::Splitter(char delimiter)
: delimiter_(delimiter)
{
    position_ = 0;
}

void Splitter::SetExpression(std::string_view exp_view)
{
    expression_ = exp_view;
    position_ = 0;
}

std::string&& Splitter::GetNextValue()
{
    return std::move(current_lexeme_);
}

void Splitter::FormLexeme_(int64_t end)
{
    lexeme_start_pos_ = position_;
    lexeme_length_ = end - lexeme_start_pos_;
    current_lexeme_.assign(&expression_[lexeme_start_pos_], lexeme_length_);
    utils::ReplaceOccurrencesOfString(current_lexeme_, "\"\"", "\"");
    position_ = end;
}

void Splitter::TrimLexeme_()
{
    if (lexeme_length_ > 0) {
        char c = expression_[lexeme_start_pos_];
        while (c == ' ' || c == '\r' || c == '\n') {
            c = expression_[++lexeme_start_pos_];
            lexeme_length_--;
        }
    }
    if (lexeme_length_ > 0) {
        char c = expression_[lexeme_start_pos_ + lexeme_length_ - 1];
        while (c == ' ' || c == '\r' || c == '\n') {
            lexeme_length_--;
            c = expression_[lexeme_start_pos_ + lexeme_length_ - 1];
        }
    }
    current_lexeme_.assign(&expression_[lexeme_start_pos_], lexeme_length_);
    utils::ReplaceOccurrencesOfString(current_lexeme_, "\"\"", "\"");
}

bool Splitter::ReadNext()
{
    lexeme_start_pos_ = 0;
    lexeme_length_ = 0;
    bool escaping = false;
    const int64_t length = expression_.size();
    
    // Skip whitespace
    for (; position_ < length; ++position_) {
        char ch = expression_[position_];
        
        if (delimiter_ == ch)
            break;
        
        if (ch != ' ' && ch != '\r' && ch != '\n')
            break;
    }
    
    if (position_ >= length) {
        return false;
    }
    
    if (delimiter_ == expression_[position_]) {
        ++position_; // Skip delimiter
    }
    
    for (int64_t pos = position_; pos < length; ++pos) {
        if (expression_[pos] == '\"') {
            if (!escaping) {
                escaping = true;
                continue;
            }
            if ((pos + 1 < length) && ('\"' == expression_[pos + 1])) {
                pos++;
                continue;
            }
            ++position_;
            FormLexeme_(pos);
            ++position_;
            return true;
        }
        
        if (escaping)
            continue;
        
        if (expression_[pos] == delimiter_) {
            FormLexeme_(pos);
            TrimLexeme_();
            return true;
        }
    }
    
    // The last value
    lexeme_start_pos_ = position_;
    lexeme_length_ = expression_.size() - lexeme_start_pos_;
    current_lexeme_.assign(&expression_[lexeme_start_pos_], lexeme_length_);
    utils::ReplaceOccurrencesOfString(current_lexeme_, "\"\"", "\"");
    position_ = expression_.size();
    return true;
}

