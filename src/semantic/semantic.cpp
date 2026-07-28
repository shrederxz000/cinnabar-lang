#include "semantic/semantic.hpp"

void SemanticVisitor::check(Program* node) {
    push_scope();
    for (const auto& child : node->body) {
        check_stmt(static_cast<Stmt*>(child.get()));
    }
    pop_scope();
}