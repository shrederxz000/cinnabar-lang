// src/lexer/lexer.hpp
#include "string_view"
#include "unordered_map"
#include "string"
#include "stdexcept"
#include "unordered_set"
#include "lexer/lexer.hpp"
#include "utils/pos.hpp"
#include "lexer/token.hpp"

namespace cxz::lexer{
    void Lexer::reset(std::string_view filepath, std::string_view code){
        code_ = code;
        code_length_ = code_.size();
        cursor_ = 0;
        current_ch_ = code.empty() ? '\0' : code[0];
        pos_ = pos::Pos {filepath, 1, 1};
    }

    // helpers
    void Lexer::advance(){
        if (current_ch_ != '\0' && cursor_ < code_length_){
            if (current_ch_ == '\n'){
                pos_.line++;
                pos_.column = 1;    
            }else{
                pos_.column++;
            }
                cursor_++;
                current_ch_ = (cursor_ < code_length_) ? code_[cursor_]: '\0';
        }
        
    }
    char Lexer::char_at(size_t jmp){
        if(cursor_ + jmp < code_length_){
            return code_[cursor_ + jmp];
        }else{
            return '\0';
        }
    }
    void Lexer::skip_whitespace(){
        while(isspace(current_ch_)){advance();}
    }

    void Lexer::skip_comments(){
        advance(); 
        if (current_ch_ == '/'){ 
            advance(); 
            while (current_ch_ != '\n' && current_ch_ != '\0'){advance();}
        }else if (current_ch_ == '*'){ 
            advance(); 
            while (current_ch_ != '\0'){ 
                /* 
                * TODO: implements error message 
                * if comment block not closed 
                */
                if (current_ch_ == '*' && char_at(1) == '/'){
                    advance(); 
                    advance(); 
                    return;
                }
                advance();
            }
            throw std::runtime_error("error: lost closing part of comment block");
        }
    }

    // scaners
    token::Token Lexer::scan_number() {
        size_t start = cursor_;
        pos::Pos start_pos = pos_;
        unsigned int has_dot = 0;

        while (
            std::isdigit(static_cast<unsigned char>(current_ch_)) 
            || current_ch_ == '.'
        ) {
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
    }

    token::Token Lexer::scan_string(){
        pos::Pos start_pos = pos_;
        advance(); // пропускаем "

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
    }

    token::Token Lexer::scan_char() {
        pos::Pos start_pos = pos_;
        advance(); // пропускаем '

        char value;

        if (current_ch_ == '\\') {
            advance();
            switch (current_ch_) {
                case 'n':  value = '\n'; break;
                case 't':  value = '\t'; break;
                case '\'': value = '\''; break;
                case '\\': value = '\\'; break;
                default:   value = current_ch_;
            }
        } else {
            value = current_ch_;
        }

        advance();

        if (current_ch_ != '\'') {
            throw std::runtime_error("error:  lost closing quote or empty char");
        }

        advance(); // закрывающая '
        return token::Token(token::TokenKind::CHAR_LITERAL, value, start_pos);
    }

    
    token::Token Lexer::scan_id_or_keyword() {
        size_t start = cursor_;
        pos::Pos start_pos = pos_;

        while (
            std::isalnum(static_cast<unsigned char>(current_ch_)) 
            || current_ch_ == '_'
        ) {advance();}

        size_t len = cursor_ - start;
        std::string value(code_.substr(start, len));

        // ключевые слова
        static const std::unordered_map<
            std::string, 
            token::TokenKind
        > keywords = {
            {"if",     token::TokenKind::KEYWORD},
            {"else",   token::TokenKind::KEYWORD},
            {"while",  token::TokenKind::KEYWORD},
            {"for",    token::TokenKind::KEYWORD},
            {"return", token::TokenKind::KEYWORD},
            {"let",    token::TokenKind::KEYWORD},
            {"def",    token::TokenKind::KEYWORD},
            {"elif",   token::TokenKind::KEYWORD},
            {"switch", token::TokenKind::KEYWORD},
            {"case",   token::TokenKind::KEYWORD},
            {"float",  token::TokenKind::KEYWORD},
            {"int",    token::TokenKind::KEYWORD},
            {"str",    token::TokenKind::KEYWORD},
            {"char",   token::TokenKind::KEYWORD},
            {"print",  token::TokenKind::KEYWORD},
        };

        auto it = keywords.find(value);
        if (it != keywords.end()) {
            return token::Token(it->second, value, start_pos);
        }

        return token::Token(token::TokenKind::ID, value, start_pos);
    }

    // simple token maker
    token::Token Lexer::emit(token::TokenKind kind, size_t len){
        pos::Pos pos = pos_;
        token::Token result = token::Token(kind, pos); 
        for (int i = 0; i < len; i++){advance();}
        return result;
    }

    std::vector<token::Token> 
    Lexer::tokenize(
        std::string_view filepath, 
        std::string_view code
    ){
        reset(filepath, code);
        std::vector<token::Token> tokens;
        while (current_ch_ != '\0' && cursor_ < code_length_){
            switch (current_ch_){
            case '+':
                tokens.push_back(emit(token::TokenKind::PLUS,1));
                break;
            case '-':
                tokens.push_back(emit(token::TokenKind::MINUS,1));
                break;
            case '/':
                if (char_at(1) == '*' || char_at(1) == '/'){
                    skip_comments();
                }else{
                tokens.push_back(emit(token::TokenKind::SLASH,1));
                }
                break;
            case '*':
                tokens.push_back(emit(token::TokenKind::STAR,1));
                break;
            case '(':
                tokens.push_back(emit(token::TokenKind::RPAR,1));
                break;
            case ')':
                tokens.push_back(emit(token::TokenKind::LPAR,1));
                break;
            case '[':
                tokens.push_back(emit(token::TokenKind::LBRACKET,1));
                break;
            case ']':
                tokens.push_back(emit(token::TokenKind::RBRACKET,1));
                break;
            case '{':
                tokens.push_back(emit(token::TokenKind::LBRACE,1));
                break;
            case '}':
                tokens.push_back(emit(token::TokenKind::RBRACE,1));
                break;
            case ':':
                tokens.push_back(emit(token::TokenKind::COLON,1));
                break;
            case ';':
                tokens.push_back(emit(token::TokenKind::SEMICOLON,1));
                break;
            case '\'':
                tokens.push_back(scan_char());
                break;
            case '"':
                tokens.push_back(scan_string());
                break;
            default:
                if(isspace(current_ch_)){
                    skip_whitespace();
                }
                else if(isalpha(current_ch_) || current_ch_ == '_'){
                    tokens.push_back(scan_id_or_keyword());
                }
                else if (isdigit(current_ch_)){
                    tokens.push_back(scan_number());
                }else {
                    throw std::runtime_error("error: unexpected syntax");
                }
            }
        }
        emit(token::TokenKind::Eof);
        return tokens;
    }
}
