#include <gtest/gtest.h>
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "ast/node.hpp"
#include "ast/expr.hpp"


static std::unique_ptr<Program> parse(const std::string& src) {
    Lexer lexer;
    auto tokens = lexer.tokenize("test", src);
    Parser parser(tokens);
    return parser.parse_program();
}

// ── AssignExpr ───────────────────────────────────────────

TEST(ParserTest, AssignExpr) {
    auto prog = parse("x=10;");
    ASSERT_EQ(prog->body.size(), 1);

    auto* es = dynamic_cast<ExprStmt*>(prog->body[0].get());
    ASSERT_NE(es, nullptr);

    auto* assign = dynamic_cast<AssignExpr*>(es->expr.get());
    ASSERT_NE(assign, nullptr);

    auto* target = dynamic_cast<IdentifierExpr*>(assign->target.get());
    ASSERT_NE(target, nullptr);
    EXPECT_EQ(target->name, "x");

    auto* val = dynamic_cast<LiteralExpr*>(assign->value.get());
    ASSERT_NE(val, nullptr);
    EXPECT_EQ(std::get<int64_t>(val->value), 10);
}

// ── CallExpr ─────────────────────────────────────────────

TEST(ParserTest, CallExprNoArgs) {
    auto prog = parse("foo();");
    ASSERT_EQ(prog->body.size(), 1);

    auto* es = dynamic_cast<ExprStmt*>(prog->body[0].get());
    ASSERT_NE(es, nullptr);

    auto* call = dynamic_cast<CallExpr*>(es->expr.get());
    ASSERT_NE(call, nullptr);
    EXPECT_EQ(call->args.size(), 0);

    auto* callee = dynamic_cast<IdentifierExpr*>(call->callee.get());
    ASSERT_NE(callee, nullptr);
    EXPECT_EQ(callee->name, "foo");
}

TEST(ParserTest, CallExprWithArgs) {
    auto prog = parse("print(\"hi\");");
    ASSERT_EQ(prog->body.size(), 1);

    auto* es = dynamic_cast<ExprStmt*>(prog->body[0].get());
    ASSERT_NE(es, nullptr);

    auto* call = dynamic_cast<CallExpr*>(es->expr.get());
    ASSERT_NE(call, nullptr);
    ASSERT_EQ(call->args.size(), 1);

    auto* arg = dynamic_cast<LiteralExpr*>(call->args[0].get());
    ASSERT_NE(arg, nullptr);
    EXPECT_EQ(std::get<std::string>(arg->value), "hi");
}

// ── BinaryExpr ───────────────────────────────────────────

TEST(ParserTest, BinaryExprPrecedence) {
    // 1+2*3 должно быть 1+(2*3)
    auto prog = parse("1+2*3;");
    ASSERT_EQ(prog->body.size(), 1);

    auto* es = dynamic_cast<ExprStmt*>(prog->body[0].get());
    auto* add = dynamic_cast<BinaryExpr*>(es->expr.get());
    ASSERT_NE(add, nullptr);
    EXPECT_EQ(add->op, InfixOp::ADD);

    // правая часть должна быть 2*3
    auto* mul = dynamic_cast<BinaryExpr*>(add->rhs.get());
    ASSERT_NE(mul, nullptr);
    EXPECT_EQ(mul->op, InfixOp::MUL);
}

TEST(ParserTest, BinaryExprPow) {
    // 2**3**2 должно быть 2**(3**2) — правоассоциативный
    auto prog = parse("2**3**2;");

    auto* es = dynamic_cast<ExprStmt*>(prog->body[0].get());
    auto* pow1 = dynamic_cast<BinaryExpr*>(es->expr.get());
    ASSERT_NE(pow1, nullptr);
    EXPECT_EQ(pow1->op, InfixOp::POW);

    // правая часть должна быть 3**2
    auto* pow2 = dynamic_cast<BinaryExpr*>(pow1->rhs.get());
    ASSERT_NE(pow2, nullptr);
    EXPECT_EQ(pow2->op, InfixOp::POW);
}

// ── VarStmt ──────────────────────────────────────────────

TEST(ParserTest, VarStmtExplicitType) {
    auto prog = parse("x:int=5;");
    ASSERT_EQ(prog->body.size(), 1);

    auto* var = dynamic_cast<VarStmt*>(prog->body[0].get());
    ASSERT_NE(var, nullptr);
    EXPECT_EQ(var->name, "x");
    EXPECT_EQ(var->var_type_kind, TokenKind::INT);

    auto* lit = dynamic_cast<LiteralExpr*>(var->value.get());
    ASSERT_NE(lit, nullptr);
    ASSERT_TRUE(std::holds_alternative<int64_t>(lit->value));
    EXPECT_EQ(std::get<int64_t>(lit->value), 5);
}

TEST(ParserTest, VarStmtAutoType) {
    auto prog = parse("x:=5;");
    ASSERT_EQ(prog->body.size(), 1);

    auto* var = dynamic_cast<VarStmt*>(prog->body[0].get());
    ASSERT_NE(var, nullptr);
    EXPECT_EQ(var->name, "x");

    auto* lit = dynamic_cast<LiteralExpr*>(var->value.get());
    ASSERT_NE(lit, nullptr);
    EXPECT_EQ(std::get<int64_t>(lit->value), 5);
}

TEST(ParserTest, VarStmtNull) {
    auto prog = parse("a:int=null;");
    ASSERT_EQ(prog->body.size(), 1);

    auto* var = dynamic_cast<VarStmt*>(prog->body[0].get());
    ASSERT_NE(var, nullptr);
    EXPECT_EQ(var->name, "a");

    auto* lit = dynamic_cast<LiteralExpr*>(var->value.get());
    ASSERT_NE(lit, nullptr);
    EXPECT_TRUE(std::holds_alternative<std::monostate>(lit->value));
}

// ── IfStmt ───────────────────────────────────────────────

TEST(ParserTest, IfStmtNoElse) {
    auto prog = parse("if(x==5){}");
    ASSERT_EQ(prog->body.size(), 1);

    auto* if_s = dynamic_cast<IfStmt*>(prog->body[0].get());
    ASSERT_NE(if_s, nullptr);
    EXPECT_NE(if_s->condition, nullptr);
    EXPECT_NE(if_s->then_branch, nullptr);
    EXPECT_EQ(if_s->else_branch, nullptr);
}

TEST(ParserTest, IfStmtWithElse) {
    auto prog = parse("if(x==5){}else{}");
    auto* if_s = dynamic_cast<IfStmt*>(prog->body[0].get());
    ASSERT_NE(if_s, nullptr);
    EXPECT_NE(if_s->else_branch, nullptr);
}

// ── WhileStmt ────────────────────────────────────────────

TEST(ParserTest, WhileStmt) {
    auto prog = parse("while(x>0){}");
    ASSERT_EQ(prog->body.size(), 1);

    auto* w = dynamic_cast<WhileStmt*>(prog->body[0].get());
    ASSERT_NE(w, nullptr);
    EXPECT_NE(w->condition, nullptr);
    EXPECT_NE(w->loop_body, nullptr);
}