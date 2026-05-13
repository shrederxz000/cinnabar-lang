#include "stdexcept"
#include "tuple"
#include "memory"
#include "lexer/token.hpp"
#include "ast/expr.hpp"
#include "parser/parser.hpp"

std::tuple<int, int> Parser::precedence(TokenKind kind) {
    switch (kind) {
        /* [0]: сила, [1]: 0 - правоассоциативный, 1 - левоассоциативный */
        case TokenKind::OR:    return {2,  1};
        case TokenKind::AND:   return {3,  1};
        case TokenKind::EQ:
        case TokenKind::NEQ:
        case TokenKind::LT:
        case TokenKind::GT:
        case TokenKind::LE:
        case TokenKind::GE:    return {5,  1};

        case TokenKind::PLUS:
        case TokenKind::MINUS: return {10, 1};

        case TokenKind::STAR:
        case TokenKind::SLASH: return {20, 1};

        case TokenKind::POW:   return {30, 0}; // правоассоциативный
        default:               return {0,  1};
    }
}
std::unique_ptr<Expr> Parser::parse_call(std::unique_ptr<Expr> callee) {
    Pos pos = peek().pos();
    advance(); // съедаем (

    std::vector<std::unique_ptr<Expr>> args;
    while (!check(TokenKind::RPAR) && !check(TokenKind::Eof)) {
        args.push_back(parse_expression());
        if (!match(TokenKind::COMMA)) break;
    }
    expect(TokenKind::RPAR, "expected ')'");

    return std::make_unique<CallExpr>(std::move(callee), std::move(args), pos);
}

std::unique_ptr<Expr> Parser::parse_prefix() {
    const Token& tok = advance();
    switch (tok.kind()) {
        case TokenKind::INT_LITERAL:
            return std::make_unique<LiteralExpr>(tok.value(), tok.pos());
        case TokenKind::FLOAT_LITERAL:
            return std::make_unique<LiteralExpr>(tok.value(), tok.pos());
        case TokenKind::STRING_LITERAL:
            return std::make_unique<LiteralExpr>(tok.value(), tok.pos());
        case TokenKind::CHAR_LITERAL:
            return std::make_unique<LiteralExpr>(tok.value(), tok.pos());
        case TokenKind::BOOL_LITERAL:
            return std::make_unique<LiteralExpr>(tok.value(), tok.pos());
        case TokenKind::Null:
            return std::make_unique<LiteralExpr>(std::monostate{}, tok.pos());

        case TokenKind::MINUS: {
            std::unique_ptr<Expr> operand = parse_prefix();
            return std::make_unique<UnaryExpr>(PrefixOp::MINUS, std::move(operand), tok.pos());
        }

        case TokenKind::ID:
            return std::make_unique<IdentifierExpr>(tok.as<std::string>(), tok.pos());

        case TokenKind::LPAR: {
            std::unique_ptr<Expr> expr = parse_expression();
            expect(TokenKind::RPAR, "expected ')'");
            return expr;
        }

        default:
            throw std::runtime_error("unexpected token in expression");
    }
}

std::unique_ptr<Expr> Parser::parse_expression(int min_prec) {
    std::unique_ptr<Expr> left = parse_prefix();

    while (check(TokenKind::LPAR)) {
        left = parse_call(std::move(left));
    }

    while (true) {
        TokenKind kind = peek().kind();
        auto [prec, assoc] = precedence(kind);

        if (prec == 0 || prec < min_prec) break;

        Token op_tok = advance();

        // правоассоциативный: следующий уровень = prec
        // левоассоциативный:  следующий уровень = prec + 1
        int next_prec = (assoc == 0) ? prec : prec + 1;
        std::unique_ptr<Expr> right = parse_expression(next_prec);

        InfixOp op;
        switch (op_tok.kind()) {
            case TokenKind::PLUS:  op = InfixOp::ADD; break;
            case TokenKind::MINUS: op = InfixOp::SUB; break;
            case TokenKind::STAR:  op = InfixOp::MUL; break;
            case TokenKind::SLASH: op = InfixOp::DIV; break;
            case TokenKind::POW:   op = InfixOp::POW; break;
            case TokenKind::EQ:    op = InfixOp::EQ;  break;
            case TokenKind::NEQ:   op = InfixOp::NEQ; break;
            case TokenKind::LT:    op = InfixOp::LT;  break;
            case TokenKind::GT:    op = InfixOp::GT;  break;
            case TokenKind::LE:    op = InfixOp::LE;  break;
            case TokenKind::GE:    op = InfixOp::GE;  break;
            default:
                throw std::runtime_error("unknown binary operator");
        }

        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right), op_tok.pos());
    }

    return left;
}