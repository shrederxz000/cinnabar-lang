#pragma once
#include <variant>
#include <cstdint>
// #include "fmt/core.h"
#include "utils.hpp"

enum class TokenKind {
    PLUS,         // +
    MINUS,        // -
    SLASH,        // /
    STAR,         // *
    POW,          // **
    ASSIGN,       // =
    OR,           // ||
    AND,          // &&
    RPAR,         // )
    LPAR,         // (
    LBRACE,       // {
    RBRACE,       // }
    SEMICOLON,    // ;
    LT,           // <
    GT,           // >
    LE,           // <=
    GE,           // >=
    EQ,           // ==
    NEQ,          // !=
    BANG,         // !
    COLON,        // :
    COLON_ASSIGN, // :=
    COMMA,        // ,

    // keywords
    ID,
    IF,
    ELSE,
    WHILE,
    FLOAT,
    INT,
    STR,
    CHAR,
    BOOL,
    Null,
    PRINT,

    // literals
    INT_LITERAL,
    FLOAT_LITERAL,
    STRING_LITERAL,
    CHAR_LITERAL,
    BOOL_LITERAL, // true of false
    Eof,
};

using TokenValue = std::variant<std::monostate, int64_t, double, std::string, char, bool>;

class Token {
  private:
    TokenKind kind_;
    TokenValue value_;
    Pos pos_;

  public:
    Token(TokenKind kind, Pos pos) 
        : kind_(kind), value_(std::monostate{}), pos_(pos) {}

    template <typename T>
    Token(TokenKind kind, T&& value, Pos pos)
        : kind_(kind), value_(std::forward<T>(value)), pos_(pos) {}

    TokenKind kind() const noexcept {
        return kind_;
    }
    const Pos& pos() const noexcept {
        return pos_;
    }
    const TokenValue& value() const noexcept {
        return value_;
    }
    bool has_value() const noexcept {
        return !std::holds_alternative<std::monostate>(value_);
    }
    template <typename T> bool is() const noexcept {
        return std::holds_alternative<T>(value_);
    }
    template <typename T> const T& as() const {
        return std::get<T>(value_);
    }
};

inline std::string to_string(TokenKind kind) {
    switch (kind) {
    case TokenKind::PLUS:
        return "+";
    case TokenKind::MINUS:
        return "-";
    case TokenKind::SLASH:
        return "/";
    case TokenKind::STAR:
        return "*";
    case TokenKind::POW:
        return "**";
    case TokenKind::ASSIGN:
        return "=";
    case TokenKind::OR:
        return "||";
    case TokenKind::AND:
        return "&&";
    case TokenKind::RPAR:
        return ")";
    case TokenKind::LPAR:
        return "(";
    case TokenKind::LBRACE:
        return "{";
    case TokenKind::RBRACE:
        return "}";
    case TokenKind::SEMICOLON:
        return ";";
    case TokenKind::LT:
        return "<";
    case TokenKind::GT:
        return ">";
    case TokenKind::LE:
        return "<=";
    case TokenKind::GE:
        return ">=";
    case TokenKind::EQ:
        return "==";
    case TokenKind::NEQ:
        return "!=";
    case TokenKind::COLON:
        return ":";
    case TokenKind::COLON_ASSIGN:
        return ":=";
    case TokenKind::IF:
        return "if";
    case TokenKind::ELSE:
        return "else";
    case TokenKind::WHILE:
        return "while";
    case TokenKind::FLOAT:
        return "float";
    case TokenKind::INT:
        return "int";
    case TokenKind::STR:
        return "str";
    case TokenKind::CHAR:
        return "char";
    case TokenKind::BOOL:
        return "bool";
    case TokenKind::PRINT:
        return "print";
    case TokenKind::Null:
        return "null";
    case TokenKind::Eof:
        return "EOF";
    default:
        return "UNKNOWN";
    }
}

std::string token_to_string(const Token& tok);
