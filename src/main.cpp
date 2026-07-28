#include <fstream>
#include "fmt/core.h"
#include "utils.hpp"
#include "lexer/token.hpp"
#include "lexer/lexer.hpp"
#include "ast/node.hpp"
#include "parser/parser.hpp"
#include "semantic/semantic.hpp"
#include "codegen/serializer.hpp"

void print_tokens(const std::vector<Token>& tokens) {
    for (const auto& tok : tokens) {
        fmt::print("{} at {}:{}\n", token_to_string(tok), tok.pos().line, tok.pos().column);
    }
}

void print_ast(const Node* node, int indent = 0) {
    std::string pad(indent, ' ');

    switch (node->kind) {
    case NodeKind::Program: {
        fmt::println("{}Program", pad);
        const auto* prog = static_cast<const Program*>(node);
        for (const auto& child : prog->body)
            print_ast(child.get(), indent + 2);
        break;
    }
    case NodeKind::BlockStmt: {
        fmt::println("{}Block", pad);
        const auto* block = static_cast<const Block*>(node);
        for (const auto& stmt : block->statements)
            print_ast(stmt.get(), indent + 2);
        break;
    }
    case NodeKind::VarStmt: {
        const auto* var = static_cast<const VarStmt*>(node);
        fmt::println("{}VarStmt {}", pad, var->name);
        print_ast(var->value.get(), indent + 2);
        break;
    }
    case NodeKind::IfStmt: {
        fmt::println("{}IfStmt", pad);
        const auto* if_s = static_cast<const IfStmt*>(node);
        fmt::println("{}  condition:", pad);
        print_ast(if_s->condition.get(), indent + 4);
        fmt::println("{}  then:", pad);
        print_ast(if_s->then_branch.get(), indent + 4);
        if (if_s->else_branch) {
            fmt::println("{}  else:", pad);
            print_ast(if_s->else_branch.get(), indent + 4);
        }
        break;
    }
    case NodeKind::WhileStmt: {
        fmt::println("{}WhileStmt", pad);
        const auto* w = static_cast<const WhileStmt*>(node);
        fmt::println("{}  condition:", pad);
        print_ast(w->condition.get(), indent + 4);
        fmt::println("{}  body:", pad);
        print_ast(w->loop_body.get(), indent + 4);
        break;
    }
    case NodeKind::ExprStmt: {
        fmt::println("{}ExprStmt", pad);
        const auto* es = static_cast<const ExprStmt*>(node);
        print_ast(es->expr.get(), indent + 2);
        break;
    }
    case NodeKind::BinaryExpr: {
        const auto* bin = static_cast<const BinaryExpr*>(node);
        fmt::println("{}BinaryExpr", pad);
        print_ast(bin->lhs.get(), indent + 2);
        print_ast(bin->rhs.get(), indent + 2);
        break;
    }
    case NodeKind::UnaryExpr: {
        fmt::println("{}UnaryExpr", pad);
        const auto* un = static_cast<const UnaryExpr*>(node);
        print_ast(un->operand.get(), indent + 2);
        break;
    }
    case NodeKind::IdentifierExpr: {
        const auto* id = static_cast<const IdentifierExpr*>(node);
        fmt::println("{}Identifier {}", pad, id->name);
        break;
    }
    case NodeKind::LiteralExpr: {
        const auto* lit = static_cast<const LiteralExpr*>(node);
        std::visit(
            [&](const auto& v) {
                using T = std::decay_t<decltype(v)>;
                if constexpr (std::is_same_v<T, std::monostate>)
                    fmt::println("{}Literal <empty>", pad);
                else if constexpr (std::is_same_v<T, std::string>)
                    fmt::println("{}Literal \"{}\"", pad, v);
                else if constexpr (std::is_same_v<T, bool>)
                    fmt::println("{}Literal {}", pad, v ? "true" : "false");
                else if constexpr (std::is_same_v<T, char>)
                    fmt::println("{}Literal '{}'", pad, v);
                else
                    fmt::println("{}Literal {}", pad, v);
            },
            lit->value);
        break;
    }
    case NodeKind::CallExpr: {
        const auto* call = static_cast<const CallExpr*>(node);
        fmt::println("{}CallExpr", pad);
        print_ast(call->callee.get(), indent + 2);
        for (const auto& arg : call->args)
            print_ast(arg.get(), indent + 2);
        break;
    }
    case NodeKind::AssignExpr: {
        const auto* assign = static_cast<const AssignExpr*>(node);
        fmt::println("{}AssignExpr", pad);
        print_ast(assign->target.get(), indent + 2);
        print_ast(assign->value.get(), indent + 2);
        break;
    }
    default:
        fmt::println("{}<unknown>", pad);
    }
}

int main( int argc, char* argv[] ) {
    if (argc < 2) {
        fmt::println("usage: cinnabar <file> [--json] [--output <file>] [--codegen]");
        fmt::println("  --json       Output AST as JSON");
        fmt::println("  --output     Save JSON to file");
        fmt::println("  --codegen    Generate and execute Python code");
        return 1;
    }

    bool output_json = false;
    bool run_codegen = false;
    std::string output_file;
    std::string input_file = argv[1];

    // Parse command line arguments
    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--json") {
            output_json = true;
        } else if (arg == "--codegen") {
            run_codegen = true;
            output_json = true; // codegen requires JSON
        } else if (arg == "--output" && i + 1 < argc) {
            output_file = argv[++i];
            output_json = true;
        }
    }

    try {
        std::string content = read_file(input_file);

        if (!output_json) {
            fmt::println("=== File: {} ===", input_file);
            fmt::println("{}\n", content);
        }

        Lexer lexer;
        std::vector<Token> tokens = lexer.tokenize(input_file, content);

        if (!output_json) {
            fmt::println("=== Tokens ===");
            print_tokens(tokens);
        }

        Parser parser(tokens);
        std::unique_ptr<Program> ast = parser.parse_program();

        if (!output_json) {
            fmt::println("\n=== AST ===");
            print_ast(ast.get());
            fmt::println("\n=== Semantic ===");
        }

        SemanticVisitor semantic;
        try {
            semantic.check(ast.get());
            if (!output_json) {
                fmt::println("OK — no errors");
            }
        } catch (const std::exception& e) {
            fmt::println("semantic error: {}", e.what());
            return 1;
        }

        // Generate JSON if requested
        if (output_json) {
            std::string json = ASTSerializer::to_json(ast.get());

            if (!output_file.empty()) {
                std::ofstream out(output_file);
                out << json;
                out.close();
                if (!run_codegen) {
                    fmt::println("JSON AST saved to: {}", output_file);
                }
            } else if (!run_codegen) {
                fmt::println("{}", json);
            }

            // Run Python codegen if requested
            if (run_codegen) {
                std::string temp_json = "/tmp/cinnabar_ast.json";
                std::ofstream temp_out(temp_json);
                temp_out << json;
                temp_out.close();

                std::string codegen_cmd = "python3 codegen.py " + temp_json + " --execute";
                int result = system(codegen_cmd.c_str());

                // Clean up temp file
                std::remove(temp_json.c_str());

                return result;
            }
        }

    } catch (const std::exception& e) {
        fmt::println("error in '{}': {}", input_file, e.what());
        return 1;
    }

    return 0;
}
