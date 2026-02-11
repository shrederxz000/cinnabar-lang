// src/parser/parser.cpp
#include "stdexcept"
#include "vector"
#include "tuple"
#include "any"
#include "memory"
#include "lexer/token.hpp"
#include "ast/program.hpp"
#include "ast/expr.hpp"
#include "ast/stmt.hpp"
#include "parser/parser.hpp"

namespace cxz::parser {

Parser::Parser(const std::vector<token::Token>& tokens)
    : tokens_(tokens) {}

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

// MAIN FUNCTION
std::unique_ptr<ast::Program> Parser::parse_program() {
    auto prog = std::make_unique<ast::Program>(peek().pos());

    while (peek().kind() != token::TokenKind::Eof) {
        prog->body.push_back(parse_statement());
    }

    return prog;
}

//'{' <stmt> '}' 
std::unique_ptr<ast::Block> Parser::parse_block() {
    auto block = std::make_unique<ast::Block>(peek().pos());
    expect(token::TokenKind::LBRACE, "expected '{'");

    while (peek().kind() != token::TokenKind::RBRACE) {
        block->statements.push_back(parse_statement());
    }

    expect(token::TokenKind::RBRACE, "expected '}'");
    return block;
}

std::unique_ptr<ast::Node> Parser::parse_statement() {

    switch (peek().kind()) {
        case token::TokenKind::LET: {return parse_let();}
        case token::TokenKind::CONST: {return parse_let();}    
        case token::TokenKind::RETURN: {return parse_return();}
            
        default: {return parse_expr_stmt();} 
    }
}

// "let"/"const",<id>,":",<type>,"=", <literal>
    std::unique_ptr<ast::Node> Parser::parse_let() {
        bool is_const = match(token::TokenKind::CONST);

        if (!is_const) {
            expect(token::TokenKind::LET, "expected 'let'");
        }

        expect(token::TokenKind::ID, "expected identifier");
        auto name = tokens_[pos_ - 1];  // токен, который съел expect

        expect(token::TokenKind::COLON, "expected ':'");

        auto typing = advance(); // тип (int / float / etc)

        expect(token::TokenKind::ASSIGN, "expected '='");

        auto value = parse_expression();

        expect(token::TokenKind::SEMICOLON, "expected ';'");

        return std::make_unique<ast::LetStmt>(
                name.as<std::string>(),
                is_const,
                typing.kind(),
                std::move(value),
                name.pos()
        );
    }



// 'return'<expr>';' 
std::unique_ptr<ast::Node> Parser::parse_return() {
    auto kw = advance();
    auto value = parse_expression();
    expect(token::TokenKind::SEMICOLON, "expected ';'");

    return std::make_unique<ast::ReturnStmt>(
        std::move(value),
        kw.pos()
    );
}

std::unique_ptr<ast::Node> Parser::parse_expr_stmt() {
    auto expr = parse_expression();
    expect(token::TokenKind::SEMICOLON, "expected ';'");
    return expr;
}

std::tuple<int, int> Parser::precedence(token::TokenKind kind) {

    switch (kind) {
        case token::TokenKind::POW: {return {30, 0};} // [0]: сила, [1]: 0 - левоассоциативный, 1 - правоассоциативный
        case token::TokenKind::STAR: {return {20, 1};}
        case token::TokenKind::SLASH: {return {20, 1};}
        case token::TokenKind::PLUS: {return {10, 1};}
        case token::TokenKind::MINUS: {return {10, 1};}
        case token::TokenKind::EQ: {return {5, 1};}
        case token::TokenKind::NEQ: {return {5, 1};}

        default: {return {0, 1};}
    }
}

std::unique_ptr<ast::Node> Parser::parse_prefix() {
    const auto& tok = advance();// а тут финальная точка краха, так как следующий токен должен быть Eof

    switch (tok.kind()) {
        case token::TokenKind::INT_LITERAL: {return std::make_unique<ast::Literal>(tok.value(), tok.pos());}
        case token::TokenKind::FLOAT_LITERAL: {return std::make_unique<ast::Literal>(tok.value(), tok.pos());}
        case token::TokenKind::STRING_LITERAL: {return std::make_unique<ast::Literal>(tok.value(), tok.pos());}
        case token::TokenKind::CHAR_LITERAL: {
            return std::make_unique<ast::Literal>(tok.value(), tok.pos());
        }

        case token::TokenKind::ID: {
            return std::make_unique<ast::Identifier>(
                tok.as<std::string>(),
                tok.pos()
            );
        }

        case token::TokenKind::LPAR: {
            auto expr = parse_expression();
            expect(token::TokenKind::RPAR, "expected ')'");
            return expr;
        }

        default: {
            throw std::runtime_error("unexpected token in expression");
        }
    }
}
    bool Parser::is_binary_op(token::TokenKind kind) const {
        switch (kind) {
            case token::TokenKind::PLUS:
            case token::TokenKind::MINUS:
            case token::TokenKind::STAR:
            case token::TokenKind::SLASH:
            case token::TokenKind::POW:
            case token::TokenKind::EQ:
            case token::TokenKind::NEQ:
            case token::TokenKind::LT:
            case token::TokenKind::LE:
            case token::TokenKind::GT:
            case token::TokenKind::GE:
                return true;
            default:
                return false;
        }
    }

 
std::unique_ptr<ast::Node> Parser::parse_expression(int min_prec) {
    auto left = parse_prefix();

    while (true) {
        auto kind = peek().kind(); // это проблемная часть. тут оказывается не литерал, а ";"
        auto [prec, prec_side] = precedence(kind);

        if (prec == 0 || prec < min_prec) {break;}

        auto op_tok = advance();//
        auto right = parse_expression(prec + prec_side);//
        ast::BinaryOp op;

        switch (op_tok.kind()) { // проверка токена и запись, а как оператор в ast
            case token::TokenKind::PLUS: {op = ast::BinaryOp::ADD; break;}
            case token::TokenKind::MINUS: {op = ast::BinaryOp::SUB; break;}
            case token::TokenKind::STAR: {op = ast::BinaryOp::MUL; break;}
            case token::TokenKind::SLASH: {op = ast::BinaryOp::DIV; break;}
            case token::TokenKind::POW: {op = ast::BinaryOp::POW; break;}

            default: {
                throw std::runtime_error("unknown binary operator");
            }
        }

        left = std::make_unique<ast::BinaryExpr>(
            op,
            std::move(left),
            std::move(right),
            op_tok.pos()
        );
    }

    return left;
}

}// namespace cxz::parser
/*
TODO: надо сделать поддержку унарных операторов и еще надо сделать возвраты сравнения в виде bool
*/
