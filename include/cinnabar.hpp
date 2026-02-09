// include/cinnabar.hpp
#pragma once

#include "vector"
#include "lexer/token.hpp"
#include "ast/node.hpp"

namespace cxz {
    void print_tokens(const std::vector<token::Token>& tokens);
    void print_ast(const ast::Node* node, int indent = 0);
}// namespace cxz

