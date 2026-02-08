// src/parser/parser.cpp
#include "stdexcept"
#include "vector"
#include "tuple"
#include "parser/parser.hpp"

namespace cxz::parser {

Parser::Parser(const std::vector<token::Token>& tokens)
    : tokens_(tokens) {}


const token::Token& Parser::peek(size_t offset) const {// по умолчанию сдвиг равоно 0
    if (pos_ + offset >= tokens_.size())// проверка на превышение кол-во индекса над кол-вом токенов
        return tokens_.back(); // в этом случе вернем последний
    return tokens_[pos_ + offset]; // по умолчанию вернем токен со сдвигом индекса (по умолчанию вернем текеущий токен)
}


const token::Token& Parser::advance() {
    return tokens_[pos_++];// возвращаем следующий токен из списка + сдвигаем pos на 1
    // ВОПРОС:интересно, здесь созраняется позиция в полях или нет?
}

 
bool Parser::match(token::TokenKind kind) {
    if (peek().kind() == kind) { // если текущий kind токена соотвествует ожидаемому, сдвинемся 
        advance(); // двигаемся вперед 
        return true;
    }
    return false;
}


void Parser::expect(token::TokenKind kind, const char* msg) {
    if (!match(kind)) // если match возвращает false (тоесть заявленный токен не совпадает с текущим), то вы выбросим ошибку
        throw std::runtime_error(msg);
}

// MAIN FUNCTION
std::unique_ptr<ast::Program> Parser::parse_program() {
    auto prog = std::make_unique<ast::Program>(peek().pos());// создаем корневой pointer

    while (peek().kind() != token::TokenKind::Eof) { // если kind текущего токена не конец программы, то в тело парсим утверждения и добавляем в тело
        prog->body.push_back(parse_statement());
    }

    return prog; // возращаем pointer
}

//'{' <stmt> '}' 
std::unique_ptr<ast::Block> Parser::parse_block() {
    auto block = std::make_unique<ast::Block>(peek().pos());// создаем block pointer 
    expect(token::TokenKind::LBRACE, "expected '{'");// проверка на наличие {

    while (peek().kind() != token::TokenKind::RBRACE) { // пока токен не } парсим утверждения и записываем в тело блока
        block->statements.push_back(parse_statement());
    }

    expect(token::TokenKind::RBRACE, "expected '}'");// проверяем на наличие } (а тут вообще ради безопасности)
    return block; // возвращаем этот pointer
}


std::unique_ptr<ast::Node> Parser::parse_statement() {
    switch (peek().kind()) { // проверяем какой текцщий токен и приминяем парсинг для конкретного токена
        case token::TokenKind::LET:
        case token::TokenKind::CONST:
            return parse_let();

        case token::TokenKind::RETURN:
            return parse_return();

        default: // а по умолчанию парсим общие утверждения
            return parse_expr_stmt();
    }
}

// 'let'/'const'<ID>'='<expr>';' 
std::unique_ptr<ast::Node> Parser::parse_let() {
    bool is_const = match(token::TokenKind::CONST); // тут проверка на const
    if (!is_const) advance(); // если проверка не сработала и is_const остался false значит это let и двигаемся вперед 

    auto name = advance(); // записываем токен и именем и двигаемся вперед
    expect(token::TokenKind::ID, "expected identifier"); // проверка на наличие имени

    expect(token::TokenKind::ASSIGN, "expected '='"); // проверка на наличие оператора присвоения

    auto value = parse_expression(); // парсим выражение и записываем как значение
    expect(token::TokenKind::SEMICOLON, "expected ';'"); // проверка на ;

    return std::make_unique<ast::LetStmt>(
        name.as<std::string>(), // имя переменной как str
        is_const, // флаг константности
        std::move(value), // скопируем значение
        name.pos() // возврат в родительский struct pos
        // еще должен сказать, что kind уже определен в дочерней LetStmt
    ); // вернем LetStmt с прарметрами выше
}


// 'return'<expr>';' 
std::unique_ptr<ast::Node> Parser::parse_return() {
    auto kw = advance(); // записываем return
    auto value = parse_expression();// сканируем выражение и записываем в значение 
    expect(token::TokenKind::SEMICOLON, "expected ';'"); // проверка на ;

    return std::make_unique<ast::ReturnStmt>(
        std::move(value),
        kw.pos()
    ); // вернем скопированное значение и позицию родительской структуре
}

std::unique_ptr<ast::Node> Parser::parse_expr_stmt() {
    auto expr = parse_expression(); // парсим выражения и записываем
    expect(token::TokenKind::SEMICOLON, "expected ';'"); // проверка на ;
    return expr;// возвращаем
}

std::tuple<int, int> Parser::precedence(token::TokenKind kind) const {
    switch (kind) { // тут сила каждого оператора и возвращаем приоретет
        case token::TokenKind::POW:return {30, 0}; // [0]: сила, [1]: 0 - левоассоциативный, 1 правоассоциативный
        case token::TokenKind::STAR:
        case token::TokenKind::SLASH: return {20, 1};
        case token::TokenKind::PLUS:
        case token::TokenKind::MINUS: return {10, 1};
        case token::TokenKind::EQ:
        case token::TokenKind::NEQ: return {5, 1};
        default: return {0, 1};
    }
}

std::unique_ptr<ast::Node> Parser::parse_prefix() {
    const auto& tok = advance(); // записываем токен + продвигаемся

    switch (tok.kind()) { // проверяем токен
        case token::TokenKind::INT_LITERAL:
        case token::TokenKind::FLOAT_LITERAL:
        case token::TokenKind::STRING_LITERAL:
        case token::TokenKind::CHAR_LITERAL:
            return std::make_unique<ast::Literal>(tok.value(), tok.pos());// если литерал, то создаем pointer Literal со значением этого литерала и позицию в родительскую структуру

        case token::TokenKind::ID: // если id, то создаем pointer ID с именем id и позицией
            return std::make_unique<ast::Identifier>(
                tok.as<std::string>(),
                tok.pos()
            );

        case token::TokenKind::LPAR: { // если токен ( 
            auto expr = parse_expression(); // парсим выражения
            expect(token::TokenKind::RPAR, "expected ')'"); // проверка на закрытие )
            return expr; // возврат выражения
        }

        default: // ну а тут выбрасываем
            throw std::runtime_error("unexpected token in expression");
    }
}

 
std::unique_ptr<ast::Node> Parser::parse_expression(int min_prec) { // по умолчанию 0
    auto left = parse_prefix();// парсим префикс и записываем в левый операнд

    while (true) { // создаем цикл. если что, то pattern matching остановит цикл
        auto kind = peek().kind(); // берем kind токена (это оператор, так как parse_perfix уже продвинул вепред)
        auto [prec, prec_side] = precedence(kind);// берем силу оператора и сторону ассоциации
        if (prec < min_prec) break; // если сила меньше заявленной, то сбрасываем цикл

        auto op_tok = advance(); // записываем опертор и продвигаемяс вперед
        auto right = parse_expression(prec + prec_side); // рекурсивно проходимся, каждый раз повышая минимальную силу исходя от текущего токена и записываем подвыражение в правый операнд 

        ast::BinaryOp op;
        switch (op_tok.kind()) { // проверка токена и запись а как оператор в ast
            case token::TokenKind::PLUS: op = ast::BinaryOp::ADD; break;
            case token::TokenKind::MINUS: op = ast::BinaryOp::SUB; break;
            case token::TokenKind::STAR: op = ast::BinaryOp::MUL; break;
            case token::TokenKind::SLASH: op = ast::BinaryOp::DIV; break;
            case token::TokenKind::POW: op = ast::BinaryOp::POW; break;
            
            default:
                throw std::runtime_error("unknown binary operator");
        }

        left = std::make_unique<ast::BinaryExpr>(
            op,
            std::move(left),
            std::move(right),
            op_tok.pos()
        );
    }

    return left;
}

};
/*
TODO: надо сделать поддержку унарных операторов и еще надо сделать возвраты сравнения в виде bool
*/
