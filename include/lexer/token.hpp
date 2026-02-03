// include/lexer/token.hpp
#pragma once
#include "variant"
#include "utils/pos.hpp"
namespace cxz::token {
enum class TokenKind{
    PLUS, MINUS, SLASH, STAR, ASSIGN, 
    LPAR, RPAR, 
    LBRACE, RBRACE, 
    LBRACKET, RBRACKET, 
    COLON, SEMICOLON, 
    ID, KEYWORD, 
    INT_LITERAL, FLOAT_LITERAL, STRING_LITERAL, CHAR_LITERAL, 
    Eof,
};
    inline const char* to_string(TokenKind kind) {
        switch (kind) {
            case TokenKind::PLUS:           return "PLUS";
            case TokenKind::MINUS:          return "MINUS";
            case TokenKind::STAR:           return "STAR";
            case TokenKind::SLASH:          return "SLASH";
            case TokenKind::ASSIGN:         return "ASSIGN";
            case TokenKind::LPAR:           return "LPAR";
            case TokenKind::RPAR:           return "RPAR";
            case TokenKind::LBRACE:         return "LBRACE";
            case TokenKind::RBRACE:         return "RBRACE";
            case TokenKind::LBRACKET:       return "LBRACKET";
            case TokenKind::RBRACKET:       return "RBRACKET";
            case TokenKind::COLON:          return "COLON";
            case TokenKind::SEMICOLON:      return "SEMICOLON";
            case TokenKind::ID:             return "ID";
            case TokenKind::KEYWORD:        return "KEYWORD";
            case TokenKind::INT_LITERAL:    return "INT_LITERAL";
            case TokenKind::FLOAT_LITERAL:  return "FLOAT_LITERAL";
            case TokenKind::STRING_LITERAL: return "STRING_LITERAL";
            case TokenKind::CHAR_LITERAL:   return "CHAR_LITERAL";
            case TokenKind::Eof:            return "EOF";
            default:                        return "<unknown>";
        }
    }


    using TokenValue = std::variant<
    std::monostate,
    int,
    double,
    std::string,
    char
>;

class Token{
private:
    TokenKind kind_;
    TokenValue value_;
    pos::Pos pos_;

public:
    Token(TokenKind kind, pos::Pos pos)
        :kind_(kind),
        value_(std::monostate{}),
        pos_(pos){}

    template<typename T>
    Token(TokenKind kind, T&& value, pos::Pos pos)
        :kind_(kind),
        value_(std::forward<T>(value)),
        pos_(pos) {}

    TokenKind kind() const noexcept { return kind_;}
    const pos::Pos& pos() const noexcept {return pos_;}

    const TokenValue& value() const noexcept {
        return value_;
    }


    bool has_value() const noexcept {
        return !std::holds_alternative<std::monostate>(value_);
    }
    template <typename T>
    bool is() const noexcept {
        return std::holds_alternative<T>(value_);
    }

    template <typename T>
    const T& as() const {
        return std::get<T>(value_);
    }

};
}
