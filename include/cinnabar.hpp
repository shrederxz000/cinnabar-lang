#pragma once
#include <vector>
#include "lexer/token.hpp"
#include "ast/node.hpp"

void print_tokens(const std::vector<Token>& tokens);
void print_ast(const Node* node, int indent = 0);
int main(int argc, char* argv[]);
