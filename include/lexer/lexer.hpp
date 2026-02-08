// lexer/lexer.hpp
#pragma once
#include "string"
#include "vector"
#include "string_view"
#include "lexer/token.hpp"
#include "utils/pos.hpp"

namespace cxz::lexer {

class Lexer {
private:
    std::string_view code_;
    size_t cursor_ = 0;
    size_t code_length_ = code_.size();
    char current_ch_ = '\0';
    utils::Pos pos_;
 
    void reset(std::string_view filepath, std::string_view code);
    void advance();
    char char_at(size_t jmp = 0);
    void skip_whitespace();
    void skip_comments();
    token::Token scan_number();
    token::Token scan_string();
    token::Token scan_char();
    token::Token scan_id_or_keyword();
    token::Token emit(token::TokenKind kind, size_t len = 1);

public:
    Lexer() = default;

    std::vector<token::Token> tokenize(std::string_view filepath, std::string_view code);
};// class Lexer

}// namespace cxz::lexer
