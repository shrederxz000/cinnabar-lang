#include <any>
#include <stdexcept>
#include <vector>
#include "lexer/token.hpp"
#include "parser/parser.hpp"

const Token& Parser::peek(size_t offset) const {
    /// возвращает текущий токен с любым смещением
    if (pos_ + offset >= tokens_.size()) {
        return tokens_.back();
    }
    return tokens_[pos_ + offset];
}

const Token& Parser::advance() {
    /// сдвигает вперед по вектору
    return tokens_[pos_++];
}

bool Parser::check(TokenKind kind) const {
    /// если проверка совпадает с ожидаемым - то вернет true
    return peek().kind() == kind;
}

bool Parser::match(TokenKind kind) {
    /// если проверка совпадает с ожидаемым - то вернет true и продвинется дальше
    if (check(kind)) {
        advance();
        return true;
    }
    return false;
}

void Parser::expect(TokenKind kind, const char* msg) {
    /// если тип токена не совпадает с ожидаемым, то вернет ошибку
    if (!match(kind)) {
        throw std::runtime_error(msg);
    }
}
