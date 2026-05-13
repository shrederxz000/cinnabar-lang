#include "stdexcept"
#include "vector"
#include "any"
#include "lexer/token.hpp"
#include "parser/parser.hpp"

const Token& Parser::peek(size_t offset) const {
    if (pos_ + offset >= tokens_.size()) {
        return tokens_.back();
    }
    return tokens_[pos_ + offset];
}

const Token& Parser::advance() {
    return tokens_[pos_++];
}

bool Parser::check(TokenKind kind) const {
    return peek().kind() == kind;
}

bool Parser::match(TokenKind kind) {
    if (check(kind)) {
        advance();
        return true;
    }
    return false;
}

void Parser::expect(TokenKind kind, const char* msg) {
    if (!match(kind)) {
        throw std::runtime_error(msg);
    }
}