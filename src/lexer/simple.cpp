// src/lexer/lexer.cpp
#include "string_view"
#include "unordered_map"
#include "string"
#include "stdexcept"
#include "unordered_set"
#include "lexer/lexer.hpp"
#include "utils/pos.hpp"
#include "lexer/token.hpp"

namespace cxz::lexer {

    std::vector<token::Token>Lexer::tokenize(std::string_view filepath,std::string_view code) {
        reset(filepath, code);
        std::vector<token::Token> tokens;

        while (current_ch_ != '\0' && cursor_ < code_length_)  {

            switch (current_ch_) {
                case '=': {
                    if (char_at(1) == '=') {tokens.push_back(emit(token::TokenKind::EQ,2));}
                    if (char_at(1) == '>') {tokens.push_back(emit(token::TokenKind::IMPL_ARR,2));}
                    else {tokens.push_back(emit(token::TokenKind::ASSIGN,1));}break;
                }

                case '+': {
                    if (char_at(1) == '=') {tokens.push_back(emit(token::TokenKind::PLUS_ASS,2));}
                    else {tokens.push_back(emit(token::TokenKind::PLUS,1));}break;
                }

                case '-': {
                    if (char_at(1) == '=') {tokens.push_back(emit(token::TokenKind::MINUS_ASS,2));}
                    if (char_at(1) == '>') {tokens.push_back(emit(token::TokenKind::RETURN_ARR,2));}
                    else {tokens.push_back(emit(token::TokenKind::MINUS,1));}break;
                }

                case '/': {
                    if (char_at(1) == '*' || char_at(1) == '/') {skip_comments();}
                    else if (char_at(1) == '=') {tokens.push_back(emit(token::TokenKind::SLASH_ASS,2));}
                    else {tokens.push_back(emit(token::TokenKind::SLASH,1));}break;
                }

                case '*': {
                    if (char_at(1) == '*' && char_at(2) == '=') {tokens.push_back(emit(token::TokenKind::POW_ASS,3));}
                    else if (char_at(1) == '=') {tokens.push_back(emit(token::TokenKind::STAR_ASS,2));}
                    else if (char_at(1) == '*') {tokens.push_back(emit(token::TokenKind::POW,2));}
                    else {tokens.push_back(emit(token::TokenKind::STAR,1));}break;
                }

                case '&': {
                    if (char_at(1) == '&') {tokens.push_back(emit(token::TokenKind::AND,2));}
                    else {tokens.push_back(emit(token::TokenKind::AMPERSAND,1));}break;
                }

                case '%': {
                    if (char_at(1) == '=') {tokens.push_back(emit(token::TokenKind::PERCENT_ASS,2));}
                    else {tokens.push_back(emit(token::TokenKind::PERCENT,1));}break;
                }


                case '|': {
                    if (char_at(1) == '|') {tokens.push_back(emit(token::TokenKind::OR,2));}
                    else {tokens.push_back(emit(token::TokenKind::PIPE,1));}break;
                }


                case ':': {
                    if (char_at(1) == ':') {tokens.push_back(emit(token::TokenKind::DCOLON,2));}
                    else {tokens.push_back(emit(token::TokenKind::COLON,1));}break;
                }


                case '.': {
                    if (char_at(1) == '.' && char_at(2) == '.') {tokens.push_back(emit(token::TokenKind::ELLIPSIS,3));}
                    else if (char_at(1) == '.') {tokens.push_back(emit(token::TokenKind::RANGE,2));}
                    else {tokens.push_back(emit(token::TokenKind::DOT,1));}break;
                }

                case '>': {
                    if (char_at(1) == '=') {tokens.push_back(emit(token::TokenKind::GE,2));}
                    else if (char_at(1) == '>') {tokens.push_back(emit(token::TokenKind::SHIFT_RIGHT,2));}
                    else {tokens.push_back(emit(token::TokenKind::GT,1));}break;
                }

                case '<': {
                    if (char_at(1) == '=') {tokens.push_back(emit(token::TokenKind::LE,2));}
                    else if (char_at(1) == '<') {tokens.push_back(emit(token::TokenKind::SHIFT_LEFT,2));}
                    else {tokens.push_back(emit(token::TokenKind::LT,1));}break;
                }

                case '!': {
                    if (char_at(1) == '=') {tokens.push_back(emit(token::TokenKind::NEQ,2));}
                    else {tokens.push_back(emit(token::TokenKind::BANG,1));}break;
                }

                case ';': {tokens.push_back(emit(token::TokenKind::SEMICOLON,1));break;}
                case '#': {tokens.push_back(emit(token::TokenKind::HASH,1));break;}
                case '$': {tokens.push_back(emit(token::TokenKind::DOLLAR,1));break;}
                case '(': {tokens.push_back(emit(token::TokenKind::RPAR,1));break;}
                case ')': {tokens.push_back(emit(token::TokenKind::LPAR,1));break;}
                case '[': {tokens.push_back(emit(token::TokenKind::LBRACKET,1));break;}
                case ']': {tokens.push_back(emit(token::TokenKind::RBRACKET,1));break;}
                case '{': {tokens.push_back(emit(token::TokenKind::LBRACE,1));break;}
                case '}': {tokens.push_back(emit(token::TokenKind::RBRACE,1));break;}
                case ',': {tokens.push_back(emit(token::TokenKind::COMMA,1));break;}
                case '\'': {tokens.push_back(scan_char());break;}
                case '"':{tokens.push_back(scan_string());break;}
                case '@': {tokens.push_back(emit(token::TokenKind::AT,1));break;}
                case '~': {tokens.push_back(emit(token::TokenKind::TILDE,1));break;}
                case '?': {tokens.push_back(emit(token::TokenKind::QUESTION,1));break;}
                case '^': {tokens.push_back(emit(token::TokenKind::CARET,1));break;}

                default: {

                    if(isspace(current_ch_)) {skip_whitespace();}
                    else if(isalpha(current_ch_) || current_ch_ == '_') {tokens.push_back(scan_id_or_keyword());}
                    else if (isdigit(current_ch_)) {tokens.push_back(scan_number());}
                    else {throw std::runtime_error("error: unexpected syntax");}
                }

            }// switch (current_ch_)

        }// while (current_ch_ != '\0' && cursor_ < code_length_)

        tokens.push_back(emit(token::TokenKind::Eof));
        return tokens;
    }// std::vector<token::Token>Lexer::tokenize()

}// namespace cxz::lexer
