#include "utils.hpp"
#include "fmt/core.h"
#include "lexer/lexer.hpp"
#include "lexer/token.hpp"
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

void Lexer::reset(std::string_view filepath, std::string_view code) {
    code_ = code;
    code_length_ = code_.size();
    cursor_ = 0;
    current_ch_ = code.empty() ? '\0' : code[0];
    pos_ = Pos{filepath, 1, 1};
}

void Lexer::advance() {
    if (current_ch_ != '\0' && cursor_ < code_length_) {
        if (current_ch_ == '\n') {
            pos_.line++;
            pos_.column = 1;
        } else {
            pos_.column++;
        }
        cursor_++;

        if (cursor_ < code_length_) {
            current_ch_ = code_[cursor_];
        } else {
            current_ch_ = '\0';
        }
    }
}

char Lexer::char_at(size_t offset) {
    if (cursor_ + offset < code_length_) {
        return code_[cursor_ + offset];
    } else {
        return '\0';
    }
}

void Lexer::skip_whitespace() {

    while (isspace(current_ch_)) {
        advance();
    }
}

void Lexer::skip_comments() {
    advance();
    if (current_ch_ == '/') {
        advance();
        while (current_ch_ != '\n' && current_ch_ != '\0') {
            advance();
        }
    } else if (current_ch_ == '*') {
        advance();
        while (current_ch_ != '\0') {
            if (current_ch_ == '*' && char_at(1) == '/') {
                advance();
                advance();
                return;
            }
            advance();
        }
        throw std::runtime_error("error: lost closing part of comment block");
    }
}

Token Lexer::emit(TokenKind kind, size_t len) {
    Pos pos = pos_;
    Token result = Token(kind, pos);
    for (int i = 0; i < len; i++) {
        advance();
    }
    return result;
}

std::string token_to_string(const Token& tok) {
    switch (tok.kind()) {
    case TokenKind::INT_LITERAL:
        return fmt::format("int_literal({})", tok.as<int64_t>());
    case TokenKind::FLOAT_LITERAL:
        return fmt::format("float_literal({})", tok.as<double>());
    case TokenKind::STRING_LITERAL:
        return fmt::format("string_literal(\"{}\")", tok.as<std::string>());
    case TokenKind::CHAR_LITERAL:
        return fmt::format("char_literal('{}')", tok.as<char>());
    case TokenKind::BOOL_LITERAL:
        return fmt::format("bool_literal({})", tok.as<bool>() ? "true" : "false");
    case TokenKind::Null:
        return fmt::format("null_literal({})", "null");
    case TokenKind::ID:
        return fmt::format("id({})", tok.as<std::string>());
    default:
        return to_string(tok.kind());
    }
}
