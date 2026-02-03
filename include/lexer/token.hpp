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
    bool has_value() const noexcept {
        return !std::holds_alternative<std::monostate>;
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
