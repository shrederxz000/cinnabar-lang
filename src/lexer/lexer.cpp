#include "string_view"
#include "unordered_map"
#include "string"
#include "stdexcept"
#include "unordered_set"
#include "lexer/lexer.hpp"
#include "lexer/token.hpp"

std::vector<Token>Lexer::tokenize(std::string_view filepath,std::string_view code) {
    reset(filepath, code);
    std::vector<Token> tokens;

    while (current_ch_ != '\0' && cursor_ < code_length_)  {
        switch (current_ch_) {
            case '=': {
                if (char_at(1) == '=') {tokens.push_back(emit(TokenKind::EQ,2));}
                else {tokens.push_back(emit(TokenKind::ASSIGN,1));}break;
            }
            case '+': {
                tokens.push_back(emit(TokenKind::PLUS,1));break;
            }

            case '-': {
                tokens.push_back(emit(TokenKind::MINUS,1));break;
            }

            case '/': {
                if (char_at(1) == '*' || char_at(1) == '/') {skip_comments();}
                else {tokens.push_back(emit(TokenKind::SLASH,1));}break;
            }

            case '*': {
                if (char_at(1) == '*') {tokens.push_back(emit(TokenKind::POW,2));}
                else {tokens.push_back(emit(TokenKind::STAR,1));}break;
            }

            case '>': {
                if (char_at(1) == '=') {tokens.push_back(emit(TokenKind::GE,2));}
                else {tokens.push_back(emit(TokenKind::GT,1));}break;
            }

            case '<': {
                if (char_at(1) == '=') {tokens.push_back(emit(TokenKind::LE,2));}
                else {tokens.push_back(emit(TokenKind::LT,1));}break;
            }

            case ';': {tokens.push_back(emit(TokenKind::SEMICOLON,1));break;}
            case ':': {
                if (char_at(1) == '=') {tokens.push_back(emit(TokenKind::COLON_ASSIGN,2));}
                else {tokens.push_back(emit(TokenKind::COLON,1));}break;
            }
            case '(': {tokens.push_back(emit(TokenKind::LPAR,1));break;}
            case ')': {tokens.push_back(emit(TokenKind::RPAR,1));break;}
            case '{': {tokens.push_back(emit(TokenKind::LBRACE,1));break;}
            case '}': {tokens.push_back(emit(TokenKind::RBRACE,1));break;}
            case '\'': {tokens.push_back(scan_char());break;}
            case '"':{tokens.push_back(scan_string());break;}
            case '&': {
                if (char_at(1) == '&') {tokens.push_back(emit(TokenKind::AND, 2));}
                break;
            }
            case '|': {
                if (char_at(1) == '|') {tokens.push_back(emit(TokenKind::OR, 2));}
                break;
            }
            case '!': {
                if (char_at(1) == '=') {tokens.push_back(emit(TokenKind::NEQ, 2));}
                else {tokens.push_back(emit(TokenKind::BANG, 1));}
                break;
            }

            default: {
                if(isspace(current_ch_)) {skip_whitespace();}
                else if(isalpha(current_ch_) || current_ch_ == '_') {tokens.push_back(scan_id_or_keyword());}
                else if (isdigit(current_ch_)) {tokens.push_back(scan_number());}
                else {throw std::runtime_error("error: unexpected syntax");}
            }
        }
    }
    tokens.push_back(emit(TokenKind::Eof));
    return tokens;
}
