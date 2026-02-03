// include/utils/pos.hpp
#pragma once
#include "string_view"
namespace cxz::pos{
    struct Pos{
        std::string_view filepath;
        size_t line;
        size_t column;
    };
}
