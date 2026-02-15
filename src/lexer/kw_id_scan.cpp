// src/lexer/kw_id_scan.cpp
#include "string_view"
#include "unordered_map"
#include "string"
#include "stdexcept"
#include "unordered_set"
#include "lexer/lexer.hpp"
#include "utils/pos.hpp"
#include "lexer/token.hpp"

namespace cxz::lexer {

    token::Token Lexer::scan_id_or_keyword() {
        size_t start = cursor_;
        utils::Pos start_pos = pos_;

        while (
                std::isalnum(static_cast<unsigned char>(current_ch_))
                || current_ch_ == '_'
                ) {advance();}

        size_t len = cursor_ - start;
        std::string value(code_.substr(start, len));

        static const std::unordered_map<std::string,token::TokenKind> keywords = {
                {"if",     token::TokenKind::IF},
                {"else",   token::TokenKind::ELSE},
                {"switch", token::TokenKind::SWITCH},
                {"case",   token::TokenKind::CASE},
                {"let",    token::TokenKind::LET},
                {"const",  token::TokenKind::CONST},
                {"for",    token::TokenKind::FOR},
                {"while",  token::TokenKind::WHILE},
                {"break",  token::TokenKind::BREAK},
                {"continue",token::TokenKind::CONTINUE},
                {"def",    token::TokenKind::DEF},
                {"return", token::TokenKind::RETURN},
                {"int",    token::TokenKind::INT},
                {"float",  token::TokenKind::FLOAT},
                {"str",    token::TokenKind::STR},
                {"char",   token::TokenKind::CHAR},
                {"print",  token::TokenKind::PRINT},
                {"static",  token::TokenKind::STATIC},
                {"pub",  token::TokenKind::PUBLIC},
                {"struct",  token::TokenKind::STRUCT},
                {"scope",  token::TokenKind::SCOPE},
                {"del",  token::TokenKind::DELETE},
                {"bool", token::TokenKind::BOOL},
                {"true", token::TokenKind::BOOL_LITERAL},
                {"false", token::TokenKind::BOOL_LITERAL}
        };// static const std::unordered_map<std::string,token::TokenKind> keywords

        auto it = keywords.find(value);

        if (it != keywords.end()) {

            switch (it->second) {
                case token::TokenKind::BOOL_LITERAL:
                    return token::Token(
                            token::TokenKind::BOOL_LITERAL,
                            value == "true",
                            start_pos
                    );

                default:
                    return token::Token(it->second, value, start_pos);

            }

        }
        return token::Token(token::TokenKind::ID, value, start_pos);
    }// token::Token Lexer::scan_id_or_keyword()

}// namespace cxz::lexer
