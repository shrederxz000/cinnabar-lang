#include "fmt/core.h"
#include "lexer/token.hpp"
#include "lexer/lexer.hpp"

int main() {
    cxz::lexer::Lexer lexer;
    auto tokens = lexer.tokenize("main.cbar", "def main(){print(\"Hello, world!\")}");

    for (const auto& tok : tokens) {
        fmt::print(
                "token: {:<15} value: ",
                cxz::token::to_string(tok.kind())
        );

        std::visit(
                [](const auto& v) {
                    using T = std::decay_t<decltype(v)>;
                    if constexpr (std::is_same_v<T, std::monostate>) {
                        fmt::print("<none>");
                    } else {
                        fmt::print("{}", v);
                    }
                },
                tok.value()
        );

        fmt::println(" pos:[{}:{}]", tok.pos().line, tok.pos().column);
    }

    return 0;
}
