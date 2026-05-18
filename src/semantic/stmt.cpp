#include "string"
#include "semantic/semantic.hpp"

void SemanticVisitor::check_stmt(Stmt* node) {
    switch (node->kind) {
        case NodeKind::VarStmt:
            check_var_stmt(static_cast<VarStmt*>(node)); break;
        case NodeKind::ExprStmt: {
            ExprStmt* es = static_cast<ExprStmt*>(node);
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

    // проверяем правую часть
    TypeInfo val_info = check_expr(node->value.get());

    TypeInfo info;
    if (node->var_type_kind == TokenKind::ID) {
        // автовывод типа (a := 5)
        info.type = val_info.type;
        info.is_null = val_info.is_null;
    } else {
        // явное указание типа (a:int = 5)
        info.type = token_kind_to_type(node->var_type_kind);
        info.is_null = val_info.is_null;

        // проверяем совместимость типов
        check_type_compatibility(info.type, val_info.type, node->pos, "variable declaration");
    }

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
    for (const std::unique_ptr<Stmt>& stmt : node->statements) {
        check_stmt(stmt.get());
    }
    pop_scope();
}