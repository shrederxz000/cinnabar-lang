#include "lexer/lexer.hpp"
#include "lexer/token.hpp"
#include "utils.hpp"
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

Token Lexer::scan_id_or_keyword() {
    size_t start = cursor_;
    Pos start_pos = pos_;

    while (std::isalnum(static_cast<unsigned char>(current_ch_)) || current_ch_ == '_') {
        advance();
    }
    size_t len = cursor_ - start;
    std::string value(code_.substr(start, len));

    static const std::unordered_map<std::string, TokenKind> keywords = {
        {"if", TokenKind::IF},
        {"else", TokenKind::ELSE},
        {"while", TokenKind::WHILE},
        {"int", TokenKind::INT},
        {"float", TokenKind::FLOAT},
        {"str", TokenKind::STR},
        {"char", TokenKind::CHAR},
        //                {"print",  TokenKind::PRINT},
        {"bool", TokenKind::BOOL},
        {"true", TokenKind::BOOL_LITERAL},
        {"false", TokenKind::BOOL_LITERAL},
        {"null", TokenKind::Null}};

    auto it = keywords.find(value);
    if (it != keywords.end()) {
        switch (it->second) {
        case TokenKind::BOOL_LITERAL:
            return Token(TokenKind::BOOL_LITERAL, value == "true", start_pos);
        case TokenKind::Null:
            return Token(TokenKind::Null, "null", start_pos);
        default:
            return Token(it->second, value, start_pos);
        }
    }

    return Token(TokenKind::ID, value, start_pos);
}
