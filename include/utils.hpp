#pragma once
#include "fstream"
#include "string"
#include "stdexcept"
#include "string_view"

inline std::string read_file(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::in | std::ios::binary);
    if (!file) {throw std::runtime_error("Failed to open file: " + filepath);}
    std::string content;
    std::string line;
    while (std::getline(file, line)) {content += line;content += '\n';}
    return content;
}
struct Pos{
    std::string_view filepath;
    size_t line;
    size_t column;
};
