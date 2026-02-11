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

void Lexer::reset(std::string_view filepath, std::string_view code) {
    code_ = code;
    code_length_ = code_.size();
    cursor_ = 0;
    current_ch_ = code.empty() ? '\0' : code[0];
    pos_ = utils::Pos {filepath, 1, 1};
}// void Lexer::reset()

void Lexer::advance(){

    if (current_ch_ != '\0' && cursor_ < code_length_) {

        if (current_ch_ == '\n') {
            pos_.line++;
            pos_.column = 1;
        }else {
            pos_.column++;
        }

        cursor_++;
        current_ch_ = (cursor_ < code_length_) ? code_[cursor_]: '\0';
    }
}// void Lexer::advance()

char Lexer::char_at(size_t jmp) {

    if(cursor_ + jmp < code_length_) {
        return code_[cursor_ + jmp];
    } else {
        return '\0';
    }
}// char Lexer::char_at()

void Lexer::skip_whitespace() {

    while(isspace(current_ch_)) {advance();}

}// void Lexer::skip_whitespace()

void Lexer::skip_comments() {
    advance();

    if (current_ch_ == '/') {
        advance();

        while (current_ch_ != '\n' && current_ch_ != '\0') {advance();}

    } else if (current_ch_ == '*') {
        advance();

        while (current_ch_ != '\0') {

            if (current_ch_ == '*' && char_at(1) == '/') {
                advance();
                advance();
                return;
            }

            advance();
        }

        throw std::runtime_error("error: lost closing part of comment block");
    }
}


token::Token Lexer::scan_number() {
    size_t start = cursor_;
    utils::Pos start_pos = pos_;
    unsigned int has_dot = 0;

    while (std::isdigit(static_cast<unsigned char>(current_ch_)) || current_ch_ == '.') {

        if (current_ch_ == '.') {
            has_dot++;

            if (has_dot > 1) {
                throw std::runtime_error("error: two dots in number");
            }
        }

        advance();
    }

    size_t len = cursor_ - start;
    std::string value(code_.substr(start, len));

    if (has_dot) {
       return token::Token(
            token::TokenKind::FLOAT_LITERAL,
            std::stod(value),
            start_pos
            );
    }

    return token::Token(
        token::TokenKind::INT_LITERAL,
        std::stoi(value),
        start_pos
        );
}// token::Token Lexer::scan_number()

token::Token Lexer::scan_string() {
    utils::Pos start_pos = pos_;
    advance();

    std::string buffer;
    buffer.reserve(16);

    while (current_ch_ != '"' && current_ch_ != '\0') {
        char c;

        if (current_ch_ == '\\') {
            advance();
            switch (current_ch_) {
                case 'n':  c = '\n'; break;
                case 't':  c = '\t'; break;
                case '"':  c = '"';  break;
                case '\\': c = '\\'; break;
                default:   c = current_ch_;
            }
        } else {
            c = current_ch_;
        }

        buffer.push_back(c);
        advance();
    }

    if (current_ch_ != '"') {
        throw std::runtime_error("error: lost closing double quote");
    }

    advance(); // закрывающая "
    return token::Token(token::TokenKind::STRING_LITERAL, buffer, start_pos);
}// token::Token Lexer::scan_string()

token::Token Lexer::scan_char() {
    utils::Pos start_pos = pos_;
    advance();
    char value;

    if (current_ch_ == '\\') {
        advance();
        switch (current_ch_) {
            case 'n':
                value = '\n';break;
            case 't':
                value = '\t'; break;
            case '\'':
                value = '\''; break;
            case '\\':
                value = '\\'; break;
            default:
                value = current_ch_;
        }

    } else {
        value = current_ch_;
    }

    advance();

    if (current_ch_ != '\'') {
        throw std::runtime_error("error:  lost closing quote or empty char");
    }

    advance();
    return token::Token(token::TokenKind::CHAR_LITERAL, value, start_pos);
}// token::Token Lexer::scan_char()

    
token::Token Lexer::scan_id_or_keyword() {
    size_t start = cursor_;
    utils::Pos start_pos = pos_;

    while (
        std::isalnum(static_cast<unsigned char>(current_ch_))
        || current_ch_ == '_'
    ) {advance();}

    size_t len = cursor_ - start;
    std::string value(code_.substr(start, len));

    static const std::unordered_map<std::string,token::TokenKind> keywords = {
            {"if",     token::TokenKind::IF},
            {"else",   token::TokenKind::ELSE},
            {"switch", token::TokenKind::SWITCH},
            {"case",   token::TokenKind::CASE},
            {"let",    token::TokenKind::LET},
            {"const",  token::TokenKind::CONST},
            {"for",    token::TokenKind::FOR},
            {"while",  token::TokenKind::WHILE},
            {"break",  token::TokenKind::BREAK},
            {"continue",token::TokenKind::CONTINUE},
            {"def",    token::TokenKind::DEF},
            {"return", token::TokenKind::RETURN},
            {"int",    token::TokenKind::INT},
            {"float",  token::TokenKind::FLOAT},
            {"str",    token::TokenKind::STR},
            {"char",   token::TokenKind::CHAR},
            {"print",  token::TokenKind::PRINT},
            {"static",  token::TokenKind::STATIC},
            {"pub",  token::TokenKind::PUBLIC},
            {"struct",  token::TokenKind::STRUCT},
            {"scope",  token::TokenKind::SCOPE},
            {"del",  token::TokenKind::DELETE},
            {"bool", token::TokenKind::BOOL},
            {"true", token::TokenKind::BOOL_LITERAL},
            {"false", token::TokenKind::BOOL_LITERAL}
    };// static const std::unordered_map<std::string,token::TokenKind> keywords

    auto it = keywords.find(value);

    if (it != keywords.end()) {
        
        switch (it->second) {
            case token::TokenKind::BOOL_LITERAL:
                return token::Token(
                    token::TokenKind::BOOL_LITERAL,
                    value == "true",
                    start_pos
                );

            default:
                return token::Token(it->second, value, start_pos);

        }

    }
    return token::Token(token::TokenKind::ID, value, start_pos);
}// token::Token Lexer::scan_id_or_keyword()

token::Token Lexer::emit(token::TokenKind kind, size_t len) {
    utils::Pos pos = pos_;
    token::Token result = token::Token(kind, pos);

    for (int i = 0; i < len; i++) {advance();}

    return result;
}// token::Token Lexer::emit()

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
