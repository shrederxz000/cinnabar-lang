// src/main.cpp
#include "fmt/core.h"
#include "cinnabar.hpp"
#include "utils/io.hpp"
#include "lexer/token.hpp"
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "ast/node.hpp"
#include "ast/program.hpp"

namespace cxz {
void print_tokens(const std::vector<cxz::token::Token>& tokens) {
    for (const auto& tok : tokens) {
        fmt::print("{} at {}:{}\n", cxz::token::to_string(tok.kind()), tok.pos().line, tok.pos().column);
    }
}

void print_ast(const cxz::ast::Node* node, int indent) {
    std::string pad(indent, ' ');
    using namespace cxz::ast;

    switch (node->kind) {
        case NodeKind::Program: {
            fmt::println("{}Program", pad);
            const auto* prog = static_cast<const Program*>(node);
            for (const auto& stmt : prog->body) print_ast(stmt.get(), indent + 2);
            break;
        }
        case NodeKind::Block: {
            fmt::println("{}Block", pad);
            const auto* block = static_cast<const Block*>(node);
            for (const auto& stmt : block->statements) print_ast(stmt.get(), indent + 2);
            break;
        }
        case NodeKind::LetStmt: {
            const auto* let_stmt = static_cast<const LetStmt*>(node);
            fmt::println("{}LetStmt {}{} = ...", pad, let_stmt->has_const ? "const " : "", let_stmt->name);
            break;
        }
        case NodeKind::ReturnStmt: {
            fmt::println("{}ReturnStmt", pad);
            break;
        }
        case NodeKind::BinaryExpr: {
            fmt::println("{}BinaryExpr", pad);
            break;
        }
        case NodeKind::Identifier: {
            const auto* id = static_cast<const Identifier*>(node);
            fmt::println("{}Identifier {}", pad, id->name);
            break;
        }
        case NodeKind::Literal: {
            fmt::println("{}Literal", pad);
            break;
        }
        default:
            fmt::println("{}<unknown node>", pad);
    }
}

}// namespace cxz

int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        try {
            std::string filename = argv[i];
            std::string content = cxz::utils::read_file(filename);

            fmt::println("File: {}", filename);
            fmt::println("Content:\n{}", content);

            // 1️⃣ Лексинг
            cxz::lexer::Lexer lexer;
            auto tokens = lexer.tokenize("test",
                                         "+ "
                                         "+= "
                                         "- "
                                         "-= "
                                         "/ "
                                         "/= "
                                         "* "
                                         "*= "
                                         "** "
                                         "**= "
                                         "= "
                                         "@ "
                                         "! "
                                         "# "
                                         "$ "
                                         "% "
                                         "%= "
                                         "^ "
                                         "& "
                                         "? "
                                         "~ "
                                         "| "
                                         "|| "
                                         "&& "
                                         "<= "
                                         ">= "
                                         "== "
                                         "!= "
                                         "<< "
                                         ">> "
                                         "=> "
                                         "-> "
            );
            fmt::println("Tokens:");
            cxz::print_tokens(tokens);

            // 2️⃣ Парсинг
            cxz::parser::Parser parser(tokens);
            auto ast = parser.parse_program();
            fmt::println("AST:");
            cxz::print_ast(ast.get());

        } catch (const std::exception& e) {
            fmt::println("Error processing file '{}': {}", argv[i], e.what());
        }
    }
    return 0;
}
