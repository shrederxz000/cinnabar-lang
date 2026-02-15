// src/lexer/utils.cpp
#include "string_view"
#include "unordered_map"
#include "string"
#include "stdexcept"
#include "unordered_set"
#include "lexer/lexer.hpp"
#include "utils/pos.hpp"
#include "lexer/token.hpp"

namespace cxz::lexer {

    void Lexer::reset(std::string_view filepath, std::string_view code) {
        code_ = code;
        code_length_ = code_.size();
        cursor_ = 0;
        current_ch_ = code.empty() ? '\0' : code[0];
        pos_ = utils::Pos {filepath, 1, 1};
    }// void Lexer::reset()

    void Lexer::advance(){

        if (current_ch_ != '\0' && cursor_ < code_length_) {

            if (current_ch_ == '\n') {
                pos_.line++;
                pos_.column = 1;
            }else {
                pos_.column++;
            }

            cursor_++;
            current_ch_ = (cursor_ < code_length_) ? code_[cursor_]: '\0';
        }
    }// void Lexer::advance()

    char Lexer::char_at(size_t jmp) {

        if(cursor_ + jmp < code_length_) {
            return code_[cursor_ + jmp];
        } else {
            return '\0';
        }
    }// char Lexer::char_at()

    void Lexer::skip_whitespace() {

        while(isspace(current_ch_)) {advance();}

    }// void Lexer::skip_whitespace()

    void Lexer::skip_comments() {
        advance();

        if (current_ch_ == '/') {
            advance();

            while (current_ch_ != '\n' && current_ch_ != '\0') {advance();}

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

    token::Token Lexer::emit(token::TokenKind kind, size_t len) {
        utils::Pos pos = pos_;
        token::Token result = token::Token(kind, pos);

        for (int i = 0; i < len; i++) {advance();}

        return result;
    }// token::Token Lexer::emit()

}// namespace cxz::lexer
