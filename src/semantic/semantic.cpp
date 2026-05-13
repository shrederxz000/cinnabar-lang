#include "stdexcept"
#include "string"
#include "unordered_set"
#include "semantic/semantic.hpp"

// встроенные функции языка
const std::unordered_set<std::string>
SemanticVisitor::builtins_ = {
        "print"
};

// ── scope utils ──────────────────────────────────────────

void SemanticVisitor::push_scope() {scopes_.push_back({});}

void SemanticVisitor::pop_scope() {scopes_.pop_back();}

TypeInfo* SemanticVisitor::lookup(const std::string& name) {
    for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
        auto found = it->find(name);
        if (found != it->end()) return &found->second;
    }
    return nullptr;
}

void SemanticVisitor::declare(const std::string& name, TypeInfo info) {
    auto& current = scopes_.back();
    if (current.count(name)) {
        throw std::runtime_error("error: '" + name + "' is already declared in this scope");
    }
    current[name] = info;
}

void SemanticVisitor::error(const Pos& pos, const std::string& msg) {
    throw std::runtime_error(
            "error at " + std::to_string(pos.line) + ":" + std::to_string(pos.column) + " — " + msg
    );
}

// ── entrypoint ───────────────────────────────────────────

void SemanticVisitor::check(Program* node) {
    push_scope();
    for (const auto& child : node->body) {
        check_stmt(static_cast<Stmt*>(child.get()));
    }
    pop_scope();
}

// ── statements ───────────────────────────────────────────

void SemanticVisitor::check_stmt(Stmt* node) {
    switch (node->kind) {
        case NodeKind::VarStmt:
            check_var_stmt(static_cast<VarStmt*>(node)); break;
        case NodeKind::ExprStmt: {
            auto* es = static_cast<ExprStmt*>(node);
            check_expr(es->expr.get());
            break;
        }
        case NodeKind::IfStmt:
            check_if(static_cast<IfStmt*>(node)); break;
        case NodeKind::WhileStmt:
            check_while(static_cast<WhileStmt*>(node)); break;
        case NodeKind::BlockStmt:
            check_block(static_cast<Block*>(node)); break;
        default:
            break;
    }
}

// x:int=5;  или  x:=5;
void SemanticVisitor::check_var_stmt(VarStmt* node) {
    // нельзя переобъявить в том же scope
    if (scopes_.back().count(node->name)) {
        error(node->pos, "'" + node->name + "' is already declared in this scope");
    }

    TypeInfo info;
//    info.is_const = node->is_const;
    info.type = node->var_type_kind;

    // проверяем правую часть
    TypeInfo val_info = check_expr(node->value.get());
    info.is_null = val_info.is_null;

    declare(node->name, info);
}

void SemanticVisitor::check_if(IfStmt* node) {
    check_expr(node->condition.get());
    check_block(node->then_branch.get());
    if (node->else_branch) {
        check_block(node->else_branch.get());
    }
}

void SemanticVisitor::check_while(WhileStmt* node) {
    check_expr(node->condition.get());
    check_block(node->loop_body.get());
}

void SemanticVisitor::check_block(Block* node) {
    push_scope();
    for (const auto& stmt : node->statements) {
        check_stmt(stmt.get());
    }
    pop_scope();
}

// ── expressions ──────────────────────────────────────────

TypeInfo SemanticVisitor::check_expr(Expr* node) {
    switch (node->kind) {
        case NodeKind::LiteralExpr:
            return check_literal(static_cast<LiteralExpr*>(node));
        case NodeKind::IdentifierExpr:
            return check_identifier(static_cast<IdentifierExpr*>(node));
        case NodeKind::BinaryExpr:
            return check_binary(static_cast<BinaryExpr*>(node));
        case NodeKind::UnaryExpr:
            return check_unary(static_cast<UnaryExpr*>(node));
        case NodeKind::CallExpr:
            return check_call(static_cast<CallExpr*>(node));
        case NodeKind::AssignExpr:
            return check_assign(static_cast<AssignExpr*>(node));
        default:
            error(node->pos, "unknown expression");
    }
    return {TokenKind::Null,  true};
}

TypeInfo SemanticVisitor::check_literal(LiteralExpr* node) {
    return std::visit([](const auto& v) -> TypeInfo {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, std::monostate>) return {TokenKind::Null,  true};
        if constexpr (std::is_same_v<T, int64_t>)        return {TokenKind::INT,   false};
        if constexpr (std::is_same_v<T, double>)         return {TokenKind::FLOAT, false};
        if constexpr (std::is_same_v<T, std::string>)    return {TokenKind::STR,   false};
        if constexpr (std::is_same_v<T, char>)           return {TokenKind::CHAR,  false};
        if constexpr (std::is_same_v<T, bool>)           return {TokenKind::BOOL,  false};
        return {TokenKind::Null, true};
    }, node->value);
}

TypeInfo SemanticVisitor::check_identifier(IdentifierExpr* node) {
    // встроенные функции не проверяем в таблице символов
    if (builtins_.count(node->name)) {
        return {TokenKind::ID, false};
    }

    TypeInfo* info = lookup(node->name);
    if (!info) {
        error(node->pos, "'" + node->name + "' is not declared");
    }
    return *info;
}

TypeInfo SemanticVisitor::check_binary(BinaryExpr* node) {
    check_expr(node->lhs.get());
    check_expr(node->rhs.get());
    // тип результата — потом уточнит typechecker
    return {TokenKind::INT, false};
}

TypeInfo SemanticVisitor::check_unary(UnaryExpr* node) {
    return check_expr(node->operand.get());
}

TypeInfo SemanticVisitor::check_call(CallExpr* node) {
    // проверяем что callee существует
    check_expr(node->callee.get());
    // проверяем аргументы
    for (const auto& arg : node->args) {
        check_expr(arg.get());
    }
    return {TokenKind::Null, false};
}

TypeInfo SemanticVisitor::check_assign(AssignExpr* node) {
    auto* target = static_cast<IdentifierExpr*>(node->target.get());

    TypeInfo* info = lookup(target->name);
    if (!info) {
        error(node->pos, "'" + target->name + "' is not declared");
    }
//    if (info->is_const) {
//        error(node->pos, "'" + target->name + "' is const and cannot be reassigned");
//    }

    TypeInfo val_info = check_expr(node->value.get());
    info->is_null = val_info.is_null;

    return *info;
}