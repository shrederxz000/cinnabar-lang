// include/lexer/token.hpp
#pragma once
#include "variant"
#include "utils/pos.hpp"
#include <cstdint>
namespace cxz::token {
enum class TokenKind{
    // operators
    PLUS,       // +
    MINUS,      // -
    SLASH,      // /
    STAR,       // *
    ASSIGN,     // =
    AT,         // @
    BANG,       // !
    HASH,       // #
    DOLLAR,     // $
    PERCENT,    // %
    CARET,      // ^
    AMPERSAND,  // &
    QUESTION,   // ?
    TILDE,      // ~

    RPAR,       // )
    LPAR,       // (
    LBRACE,     // {
    RBRACE,     // }
    LBRACKET,   // [
    RBRACKET,   // ]
    COLON,      // :
    SEMICOLON,  // ;
    DOT,        // .
    COMMA,      // 
    APOSTROPHE, // ' // TODO: сделать логику отличия литерала от одинарной кавычки.
    LT,         // <
    GT,         // >

    // keywords
    ID,
    KEYWORD,


    IF,
    ELSE,
    SWITCH,
    CASE,
    LET,
    CONST,
    WHILE,
    FOR,
    BREAK,
    CONTINUE,
    DEF,
    RETURN,
    FLOAT,
    INT,
    STR,
    CHAR,
    PRINT,
    STATIC,
    PUBLIC,
    STRUCT,
    SCOPE,
    ASYNC,
    SPAWN,
    DELETE,

    

    // literals
    INT_LITERAL,
    FLOAT_LITERAL,
    STRING_LITERAL,
    CHAR_LITERAL,
    BOOL_LITERAL, // true of false

    Eof,
};
    inline const char* to_string(TokenKind kind) {
        switch (kind) {
        case TokenKind::PLUS: return "PLUS";
        case TokenKind::MINUS: return "MINUS";
        case TokenKind::SLASH: return "SLASH";
        case TokenKind::STAR: return " STAR";
        case TokenKind::ASSIGN: return "ASSIGN";
        case TokenKind::AT: return "AT";
        case TokenKind::BANG: return "BANG";
        case TokenKind::HASH: return "HASH";
        case TokenKind::DOLLAR: return "DOLLAR";
        case TokenKind::PERCENT: return "PERCENT";
        case TokenKind::CARET: return "CARET";
        case TokenKind::AMPERSAND: return "AMPERSAND";
        case TokenKind::QUESTION: return "QUESTION";
        case TokenKind::TILDE: return "TILDE";
        case TokenKind::RPAR: return "RPAR";
        case TokenKind::LPAR: return "LPAR";
        case TokenKind::LBRACE: return "LBRACE";
        case TokenKind::RBRACE: return "RBRACE";
        case TokenKind::LBRACKET: return "LBRACKET";
        case TokenKind::RBRACKET: return "RBRACKET";
        case TokenKind::COLON: return "COLON";
        case TokenKind::SEMICOLON: return "SEMICOLON";
        case TokenKind::DOT: return "DOT";
        case TokenKind::COMMA: return "COMMA";
        case TokenKind::APOSTROPHE: return "APOSTROPHE";
        case TokenKind::LT: return "LT";
        case TokenKind::GT: return "GT";
        case TokenKind::ID: return "ID";
        case TokenKind::KEYWORD: return "KEYWORD";
        case TokenKind::IF: return "IF";
        case TokenKind::ELSE: return "ELSE";
        case TokenKind::SWITCH: return "SWITCH";
        case TokenKind::CASE: return "CASE";
        case TokenKind::LET: return "LET";
        case TokenKind::CONST: return "CONST";
        case TokenKind::WHILE: return "WHILE";
        case TokenKind::FOR: return "FOR";
        case TokenKind::BREAK: return "BREAK";
        case TokenKind::CONTINUE: return "CONTINUE";
        case TokenKind::DEF: return "DEF";
        case TokenKind::RETURN: return "RETURN";
        case TokenKind::FLOAT: return "FLOAT";
        case TokenKind::INT: return "INT";
        case TokenKind::STR: return "STR";
        case TokenKind::CHAR: return "CHAR";
        case TokenKind::PRINT: return "PRINT";
        case TokenKind::STATIC: return "STATIC"; 
        case TokenKind::PUBLIC: return "PUBLIC";
        case TokenKind::STRUCT: return "STRUCT";
        case TokenKind::SCOPE: return "SCOPE";
        case TokenKind::ASYNC: return "ASYNC";
        case TokenKind::SPAWN: return "SPAWN";
        case TokenKind::DELETE: return "DELETE";

        case TokenKind::INT_LITERAL: return "INT_LITERAL";
        case TokenKind::FLOAT_LITERAL: return "FLOAT_LITERAL";
        case TokenKind::STRING_LITERAL: return "STRING_LITERAL";
        case TokenKind::CHAR_LITERAL: return "CHAR_LITERAL";
        case TokenKind::BOOL_LITERAL: return "BOOL_LITERAL";
        default:  return "<unknown>";
        }
    }


    using TokenValue = std::variant<
    std::monostate,
    int64_t,
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
