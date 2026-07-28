#include <string>
#include <unordered_set>
#include "semantic/semantic.hpp"

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
    return {Type::NULL_TYPE, true};
}

TypeInfo SemanticVisitor::check_literal(LiteralExpr* node) {
    return std::visit(
        [](const auto& v) -> TypeInfo {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<T, std::monostate>)
                return {Type::NULL_TYPE, true};
            if constexpr (std::is_same_v<T, int64_t>)
                return {Type::INT, false};
            if constexpr (std::is_same_v<T, double>)
                return {Type::FLOAT, false};
            if constexpr (std::is_same_v<T, std::string>)
                return {Type::STR, false};
            if constexpr (std::is_same_v<T, char>)
                return {Type::CHAR, false};
            if constexpr (std::is_same_v<T, bool>)
                return {Type::BOOL, false};
            return {Type::NULL_TYPE, true};
        },
        node->value);
}

TypeInfo SemanticVisitor::check_identifier(IdentifierExpr* node) {
    // встроенные функции не проверяем в таблице символов
    if (builtins_.count(node->name)) {
        return {Type::UNKNOWN, false}; // функции имеют свой тип возврата
    }

    TypeInfo* info = lookup(node->name);
    if (!info) {
        error(node->pos, "'" + node->name + "' is not declared");
    }
    return *info;
}

TypeInfo SemanticVisitor::check_binary(BinaryExpr* node) {
    TypeInfo lhs_info = check_expr(node->lhs.get());
    TypeInfo rhs_info = check_expr(node->rhs.get());

    Type result_type = infer_binary_result_type(node->op, lhs_info.type, rhs_info.type, node->pos);
    return {result_type, false};
}

TypeInfo SemanticVisitor::check_unary(UnaryExpr* node) {
    TypeInfo operand_info = check_expr(node->operand.get());

    if (node->op == PrefixOp::MINUS) {
        if (!is_numeric(operand_info.type)) {
            error(node->pos, "unary minus requires numeric type");
        }
        return operand_info;
    }

    if (node->op == PrefixOp::NOT) {
        if (operand_info.type != Type::BOOL) {
            error(node->pos, "logical NOT requires boolean type");
        }
        return {Type::BOOL, false};
    }

    error(node->pos, "unknown unary operator");
    return {Type::UNKNOWN, false};
}

TypeInfo SemanticVisitor::check_call(CallExpr* node) {
    // проверяем что callee существует
    TypeInfo callee_info = check_expr(node->callee.get());

    // проверяем аргументы
    for (const std::unique_ptr<Expr>& arg : node->args) {
        check_expr(arg.get());
    }

    // для встроенных функций определяем тип возврата
    if (node->callee->kind == NodeKind::IdentifierExpr) {
        IdentifierExpr* id = static_cast<IdentifierExpr*>(node->callee.get());
        if (id->name == "print") {
            return {Type::NULL_TYPE, true}; // print ничего не возвращает
        }
    }

    return {Type::UNKNOWN, false};
}

TypeInfo SemanticVisitor::check_assign(AssignExpr* node) {
    IdentifierExpr* target = static_cast<IdentifierExpr*>(node->target.get());

    TypeInfo* info = lookup(target->name);
    if (!info) {
        error(node->pos, "'" + target->name + "' is not declared");
    }

    TypeInfo val_info = check_expr(node->value.get());

    // проверяем совместимость типов при присваивании
    check_type_compatibility(info->type, val_info.type, node->pos, "assignment");

    info->is_null = val_info.is_null;

    return *info;
}
