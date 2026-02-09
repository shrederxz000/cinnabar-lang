#include "fmt/core.h"
#include "cinnabar.hpp"
#include "utils/io.hpp"
#include "lexer/token.hpp"
#include "lexer/lexer.hpp"



int main(int argc, char* argv[]) {

    for (int i = 1; i < argc; ++i) {
        try {
            std::string filename = argv[i];
            std::string content = cxz::utils::read_file(filename);

            fmt::println("File: {}", filename);
            fmt::println("Content:\n{}", content);
        } catch (const std::exception& e) {
            fmt::println("Error reading file '{}': {}", argv[i], e.what());
        }
    }

    return 0;
}

// int main(int argc, char *argv[]) {
    // cxz::lexer::Lexer lexer;
    // auto tokens = lexer.tokenize("main.cbar", "def main(){print(\"Hello, world!\")}");

    // for (const auto& tok : tokens) {
    //     fmt::print(
    //             "token: {:<15} value: ",
    //             cxz::token::to_string(tok.kind())
    //     );

    //     std::visit(
    //             [](const auto& v) {
    //                 using T = std::decay_t<decltype(v)>;
    //                 if constexpr (std::is_same_v<T, std::monostate>) {
    //                     fmt::print("<none>");
    //                 } else {
    //                     fmt::print("{}", v);
    //                 }
    //             },
    //             tok.value()
    //     );

    //     fmt::println(" pos:[{}:{}]", tok.pos().line, tok.pos().column);
    // }
    // return 0;
// }
