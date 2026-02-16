// src/lexer/scaner.cpp
#include "string_view"
#include "unordered_map"
#include "string"
#include "stdexcept"
#include "unordered_set"
#include "lexer/lexer.hpp"
#include "utils/pos.hpp"
#include "lexer/token.hpp"

namespace cxz::lexer {

token::Token Lexer::scan_number() {
    size_t start = cursor_;
    utils::Pos start_pos = pos_;

    while (std::isdigit(static_cast<unsigned char>(current_ch_))) {
        advance();
    }

    bool is_float = false;

    if (current_ch_ == '.' &&
        std::isdigit(static_cast<unsigned char>(char_at(1)))) {

        is_float = true;
        advance();

        while (std::isdigit(static_cast<unsigned char>(current_ch_))) {
            advance();
        }
    }

    size_t len = cursor_ - start;
    std::string value(code_.substr(start, len));

    if (is_float) {
        return token::Token(
            token::TokenKind::FLOAT_LITERAL,
            std::stod(value),
            start_pos
        );
    }

    return token::Token(
        token::TokenKind::INT_LITERAL,
        std::stoll(value),
        start_pos
    );
    }// token::Token Lexer::scan_number()

    token::Token Lexer::scan_string() {
        utils::Pos start_pos = pos_;
        advance();

        std::string buffer;
        buffer.reserve(16);

        while (current_ch_ != '"' && current_ch_ != '\0') {
            char c;

            if (current_ch_ == '\\') {
                advance();
                switch (current_ch_) {
                    case 'n':
                        c = '\n';break;
                    case 't':
                        c = '\t'; break;
                    case '"':
                        c = '\"'; break;
                    case '\\':
                        c = '\\'; break;
                    case 'r':
                        c = '\r'; break;
                    case 'a':
                        c = '\a'; break;
                    case 'b':
                        c = '\b'; break;
                    case '0':
                        c = '\0'; break;
                    default:   c = current_ch_;
                }
            } else {
                c = current_ch_;
            }

            buffer.push_back(c);
            advance();
        }

        if (current_ch_ != '"') {
            throw std::runtime_error("error: lost closing double quote");
        }

        advance(); // закрывающая "
        return token::Token(token::TokenKind::STRING_LITERAL, buffer, start_pos);
    }// token::Token Lexer::scan_string()

    token::Token Lexer::scan_char() {
        utils::Pos start_pos = pos_;
        advance();
        char value;

        if (current_ch_ == '\\') {
            advance();
            switch (current_ch_) {
                case 'n':
                    value = '\n';break;
                case 't':
                    value = '\t'; break;
                case '\'':
                    value = '\''; break;
                case '\\':
                    value = '\\'; break;
                case 'r':
                    value = '\r'; break;
                case 'a':
                    value = '\a'; break;
                case 'b':
                    value = '\b'; break;
                case '0':
                    value = '\0'; break;
                default:
                    value = current_ch_;
            }

        } else {
            value = current_ch_;
        }

        advance();

        if (current_ch_ != '\'') {
            throw std::runtime_error("error:  lost closing quote or empty char");
        }

        advance();
        return token::Token(token::TokenKind::CHAR_LITERAL, value, start_pos);
    }// token::Token Lexer::scan_char()

}// namespace cxz::lexer
