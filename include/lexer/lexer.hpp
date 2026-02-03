// lexer/lexer.hpp
#pragma once
#include "string"
#include "vector"
#include "string_view"
#include "lexer/token.hpp"
#include "utils/pos.hpp"

namespace tf {
class Lexer {
public:
    Lexer() = default;

    std::vector<Token> tokenize(std::string_view filepath, std::string_view code);

private:
    std::string_view code_;
    std::string_view filepath_;
    size_t cursor_ = 0;
    char current_ch_ = '\0';
    Pos pos_;
 
    void reset(std::string_view filepath, std::string_view code);
    void advance();
    char char_at(size_t jmp = 0);
    void skip_whitespace();
    void skip_comments();
    Token scan_number();
    Token scan_string();
    Token scan_char();
    Token scan_id_or_keyword();

    /* 
    подробнее: emit()
    автоматически заталкивает токен в вектор
    + 
    автопродвижение токена на нужное кол-во advance()
    ограничения: приминения только к просьым токенам (ident, keyword, literal) не подойдут, да и есть отдельные сканеры
    */
    Token emit();

};
}
