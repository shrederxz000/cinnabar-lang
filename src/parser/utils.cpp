// src/parser/parser.cpp
#include "stdexcept"
#include "vector"
#include "any"
#include "lexer/token.hpp"
#include "parser/parser.hpp"

namespace cxz::parser {

const token::Token& Parser::peek(size_t offset) const {

    if (pos_ + offset >= tokens_.size()){
        return tokens_.back();
    }

    return tokens_[pos_ + offset];
}

const token::Token& Parser::advance() {return tokens_[pos_++];}

bool Parser::match(token::TokenKind kind) {

    if (peek().kind() == kind) {
        advance();
        return true;
    }

    return false;
}

void Parser::expect(token::TokenKind kind, const char* msg) {

    if (!match(kind)){
        throw std::runtime_error(msg);
    }

}


}// namespace cxz::parser

