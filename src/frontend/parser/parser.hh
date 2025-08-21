#include "statements.hh"
#include "node_types.hh"
#include "lexer/lexer.hh"
#include <vector>
#include <string>

#pragma once

class Parser
{
public:
    explicit Parser(const std::vector<Token> &tokens);
    BlockStatement *parse();

private:
    // Init
    std::vector<Token> tokens;
    uint64_t position;

    // Statements
    Statement *parse_statement();

    // Expressions
    Expression *parse_expression();
    Expression *parse_additive_expression();
    Expression *parse_multiplicative_expression();
    Expression *parse_unary_expression();
    Expression *parse_primary_expression();
    CellExpression *parse_cell_expression();
    Expression *parse_ranged_expression();
    CallExpression *parse_call_expression(Token function_name_token);

    // Errors
    void throw_not_matching_token(const TokenType &expected, const Token token);
    void throw_invalid_syntax_error(const Token token);

    // Helpers
    void advance_newline();
    void newline_check();
};

Parser *create_parser(const std::vector<Token> &tokens);