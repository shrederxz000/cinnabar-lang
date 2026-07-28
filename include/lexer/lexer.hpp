#pragma once
#include <string>
#include <string_view>
#include <vector>
#include "utils.hpp"
#include "lexer/token.hpp"

class Lexer {
  private:
    std::string_view code_;
    size_t cursor_ = 0;
    size_t code_length_ = code_.size();
    char current_ch_ = '\0';
    Pos pos_;

    void reset(std::string_view filepath, std::string_view code);
    void advance();
    char char_at(size_t offset = 0);
    void skip_whitespace();
    void skip_comments();
    Token scan_number();
    Token scan_string();
    Token scan_char();
    Token scan_id_or_keyword();
    Token emit(TokenKind kind, size_t len = 1);

  public:
    Lexer() = default;
    std::vector<Token> tokenize(std::string_view filepath, std::string_view code);
};
