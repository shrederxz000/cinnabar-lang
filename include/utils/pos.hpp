// include/utils/pos.hpp
#pragma once
#include "string"
namespace tf{
    struct Pos{
        std::string filepath;
        size_t line;
        size_t column;
    };
}
