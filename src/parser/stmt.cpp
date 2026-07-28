#include <stdexcept>
#include <memory>
#include "lexer/token.hpp"
#include "ast/stmt.hpp"
#include "ast/expr.hpp"
#include "parser/parser.hpp"

std::unique_ptr<Stmt> Parser::parse_statement() {
    // объявление переменной: a:int=5 или a:=5
    if (check(TokenKind::ID) &&
        (peek(1).kind() == TokenKind::COLON || peek(1).kind() == TokenKind::COLON_ASSIGN)) {
        return parse_var_stmt();
    }

    switch (peek().kind()) {
    case TokenKind::IF:
        return parse_if_stmt();
    case TokenKind::WHILE:
        return parse_while_stmt();
    default:
        return parse_expr_stmt();
    }
}

// a:int=5   →  ID COLON тип ASSIGN expr SEMICOLON
// a:=5      →  ID COLON_ASSIGN expr SEMICOLON
std::unique_ptr<Stmt> Parser::parse_var_stmt() {
    const Token& name_tok = advance(); // ID
    std::string name = name_tok.as<std::string>();

    TokenKind type_kind = TokenKind::ID; // заглушка для автовывода

    if (match(TokenKind::COLON_ASSIGN)) {
        // a := 5  — автовывод типа
        // type_kind остаётся заглушкой, семантик разберётся
    } else {
        // a:int=5
        expect(TokenKind::COLON, "expected ':' in variable declaration");
        type_kind = advance().kind(); // int / float / str / char / bool
        expect(TokenKind::ASSIGN, "expected '=' in variable declaration");
    }

    std::unique_ptr<Expr> value = parse_expression();
    expect(TokenKind::SEMICOLON, "expected ';'");

    return std::make_unique<VarStmt>(name, type_kind, std::move(value), name_tok.pos());
}

// expr;
std::unique_ptr<Stmt> Parser::parse_expr_stmt() {
    Pos pos = peek().pos();

    // присваивание: a = expr
    if (check(TokenKind::ID) && peek(1).kind() == TokenKind::ASSIGN) {
        const Token& name_tok = advance(); // ID
        advance();                         // ASSIGN
        std::unique_ptr<Expr> value = parse_expression();
        expect(TokenKind::SEMICOLON, "expected ';'");

        std::unique_ptr<IdentifierExpr> target =
            std::make_unique<IdentifierExpr>(name_tok.as<std::string>(), name_tok.pos());
        return std::make_unique<ExprStmt>(
            std::make_unique<AssignExpr>(std::move(target), std::move(value), name_tok.pos()), pos);
    }

    std::unique_ptr<Expr> expr = parse_expression();
    expect(TokenKind::SEMICOLON, "expected ';'");
    return std::make_unique<ExprStmt>(std::move(expr), pos);
}

// if (cond) { ... } else { ... }
std::unique_ptr<Stmt> Parser::parse_if_stmt() {
    Pos pos = peek().pos();
    expect(TokenKind::IF, "expected 'if'");
    expect(TokenKind::LPAR, "expected '('");
    std::unique_ptr<Expr> cond = parse_expression();
    expect(TokenKind::RPAR, "expected ')'");

    std::unique_ptr<Block> then_branch = parse_block();

    std::unique_ptr<Block> else_branch = nullptr;
    if (match(TokenKind::ELSE)) {
        else_branch = parse_block();
    }

    return std::make_unique<IfStmt>(std::move(cond), std::move(then_branch), std::move(else_branch),
                                    pos);
}

// while (cond) { ... }
std::unique_ptr<Stmt> Parser::parse_while_stmt() {
    Pos pos = peek().pos();
    expect(TokenKind::WHILE, "expected 'while'");
    expect(TokenKind::LPAR, "expected '('");
    std::unique_ptr<Expr> cond = parse_expression();
    expect(TokenKind::RPAR, "expected ')'");

    std::unique_ptr<Block> body = parse_block();

    return std::make_unique<WhileStmt>(std::move(cond), std::move(body), pos);
}
