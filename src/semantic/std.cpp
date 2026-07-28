#include <string>
#include <unordered_set>
#include "semantic/semantic.hpp"

const std::unordered_set<std::string> SemanticVisitor::builtins_ = {"print"};
