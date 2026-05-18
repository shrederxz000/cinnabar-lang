#include "stdexcept"
#include "string"
#include "semantic/semantic.hpp"

void SemanticVisitor::push_scope() {
    scopes_.push_back({});
}

void SemanticVisitor::pop_scope() {
    scopes_.pop_back();
}

TypeInfo* SemanticVisitor::lookup(const std::string& name) {
    for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
        auto found = it->find(name);
        if (found != it->end()) {
            return &found->second;
        }
    }
    return nullptr;
}

void SemanticVisitor::declare(const std::string& name, TypeInfo info) {
    std::unordered_map<std::basic_string<char>, TypeInfo> &current = scopes_.back();
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