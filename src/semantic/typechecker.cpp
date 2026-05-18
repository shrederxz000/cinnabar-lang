#include "semantic/semantic.hpp"

Type SemanticVisitor::token_kind_to_type(TokenKind kind) {
    switch (kind) {
        case TokenKind::INT:   return Type::INT;
        case TokenKind::FLOAT: return Type::FLOAT;
        case TokenKind::STR:   return Type::STR;
        case TokenKind::CHAR:  return Type::CHAR;
        case TokenKind::BOOL:  return Type::BOOL;
        case TokenKind::Null:  return Type::NULL_TYPE;
        default:               return Type::UNKNOWN;
    }
}

bool SemanticVisitor::is_numeric(Type t) {
    return t == Type::INT || t == Type::FLOAT;
}

bool SemanticVisitor::is_comparable(Type t) {
    return t == Type::INT || t == Type::FLOAT || t == Type::CHAR || t == Type::BOOL;
}

Type SemanticVisitor::infer_binary_result_type(InfixOp op, Type lhs, Type rhs, const Pos& pos) {
    // Арифметические операции
    if (op == InfixOp::ADD || op == InfixOp::SUB || op == InfixOp::MUL ||
        op == InfixOp::DIV || op == InfixOp::POW) {

        // Специальный случай: string + string = string
        if (op == InfixOp::ADD && lhs == Type::STR && rhs == Type::STR) {
            return Type::STR;
        }

        if (!is_numeric(lhs) || !is_numeric(rhs)) {
            error(pos, "arithmetic operations require numeric types");
        }

        // float + int = float, int + int = int
        if (lhs == Type::FLOAT || rhs == Type::FLOAT) {
            return Type::FLOAT;
        }
        return Type::INT;
    }

    // Операции сравнения
    if (op == InfixOp::LT || op == InfixOp::GT || op == InfixOp::LE || op == InfixOp::GE) {
        if (!is_comparable(lhs) || !is_comparable(rhs)) {
            error(pos, "comparison operations require comparable types");
        }
        if (lhs != rhs) {
            error(pos, "cannot compare different types");
        }
        return Type::BOOL;
    }

    // Операции равенства
    if (op == InfixOp::EQ || op == InfixOp::NEQ) {
        if (lhs != rhs && lhs != Type::NULL_TYPE && rhs != Type::NULL_TYPE) {
            error(pos, "equality check requires same types");
        }
        return Type::BOOL;
    }

    // Логические операции
    if (op == InfixOp::AND || op == InfixOp::OR) {
        if (lhs != Type::BOOL || rhs != Type::BOOL) {
            error(pos, "logical operations require boolean operands");
        }
        return Type::BOOL;
    }

    error(pos, "unknown binary operation");
    return Type::UNKNOWN;
}

void SemanticVisitor::check_type_compatibility(Type expected, Type actual, const Pos& pos, const std::string& context) {
    if (expected == Type::UNKNOWN) {
        return; // автовывод типа
    }

    if (actual == Type::NULL_TYPE) {
        return; // null совместим с любым типом
    }

    if (expected != actual) {
        error(pos, context + ": expected type '" + std::to_string(static_cast<int>(expected)) +
              "' but got '" + std::to_string(static_cast<int>(actual)) + "'");
    }
}
