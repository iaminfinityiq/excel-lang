#include "ast_nodes.hpp"
#include "nodes.hpp"
#include "../lexer/lexer.hpp"
#include <vector>

#pragma once

class Parser {
private:
    long long position;
    std::vector<Token> tokens;

    Statement parse_statement();

    Expression parse_expression();
    Expression parse_additive_expression();
    Expression parse_multiplicative_expression();
    Expression parse_unary_expression();
    Expression parse_primary_expression();
    CallExpression parse_call_expression(const std::string& function_name);
    CellExpression parse_cell_expression();
    Expression parse_ranged_expression();

    void throw_invalid_syntax_error(const Token& token);
    void throw_not_matching_token(const TokenType& expected, const Token& token);
    void throw_not_an_integer(const Token& token);
    void advance_newline();
    void newline_check();
public:
    explicit Parser(const std::vector<Token>& tokens);
    BlockStatement parse();
};

Parser create_parser(const std::vector<Token> &tokens);