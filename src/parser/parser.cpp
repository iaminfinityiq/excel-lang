#include "parser.hpp"
#include "nodes.hpp"
#include "ast_nodes.hpp"
#include "../lexer/lexer.hpp"
#include <vector>
#include <string>
#include <stdexcept>
#include <sstream>

Parser::Parser(const std::vector<Token>& tokens) : position(0), tokens(tokens) {}

BlockStatement Parser::parse() {
    BlockStatement block = BlockStatement();
    while (this->tokens[this->position].token_type != TokenType::END_OF_FILE) {
        this->advance_newline();
        if (this->tokens[this->position].token_type != TokenType::END_OF_FILE) {
            break;
        }

        block.body.push_back(this->parse_statement());
    }

    return block;
}

Statement Parser::parse_statement() {
    switch (this->tokens[this->position].token_type) {
    default:
        Expression expr = this->parse_expression();
        this->newline_check();
        return ExpressionStatement(expr);
    }
}

Expression Parser::parse_expression() {
    return this->parse_additive_expression();
}

Expression Parser::parse_additive_expression() {
    Expression lhs = this->parse_multiplicative_expression();
    while (this->tokens[this->position].token_type == TokenType::PLUS || this->tokens[this->position].token_type == TokenType::MINUS) {
        std::string op = this->tokens[this->position].value;
        this->position++;

        Expression rhs = this->parse_multiplicative_expression();
        lhs = BinaryExpression(lhs, op, rhs);
    }

    return lhs;
}

Expression Parser::parse_multiplicative_expression() {
    Expression lhs = this->parse_primary_expression();
    while (this->tokens[this->position].token_type == TokenType::MULTIPLY || this->tokens[this->position].token_type == TokenType::DIVIDE) {
        std::string op = this->tokens[this->position].value;
        this->position++;

        Expression rhs = this->parse_primary_expression();
        lhs = BinaryExpression(lhs, op, rhs);
    }

    return lhs;
}

Expression Parser::parse_unary_expression() {
    switch (this->tokens[this->position].token_type) {
    case TokenType::PLUS:
        this->position++;
        Expression value = this->parse_unary_expression();
        return UnaryExpression("+", value);
    case TokenType::MINUS:
        this->position++;
        Expression value = this->parse_unary_expression();
        return UnaryExpression("-", value);
    default:
        return this->parse_primary_expression();
    }
}

Expression Parser::parse_primary_expression() {
    std::string number;
    std::string identifier;
    switch (this->tokens[this->position].token_type) {
    case TokenType::NUMBER:
        number = this->tokens[this->position].value;
        if (number[0] == 'd') {
            number = number.substr(1, number.size()-1);
            return NumberExpression(std::stod(number), true);
        }

        return NumberExpression(std::stod(number), false);
    case TokenType::IDENTIFIER:
        identifier = this->tokens[this->position].value;
        this->position++;

        if (this->tokens[this->position].token_type == TokenType::NUMBER && this->tokens[this->position].value[0] != 'd') {
            this->position--;
            return this->parse_ranged_expression();
        } else {
            return this->parse_call_expression(identifier);
        }
    default:
        Parser::throw_invalid_syntax_error(this->tokens[this->position]);
    }
}

CellExpression Parser::parse_cell_expression() {
    std::string column = this->tokens[this->position].value;
    this->position++;

    if (this->tokens[this->position].token_type != TokenType::NUMBER) {
        this->throw_not_matching_token(TokenType::NUMBER, this->tokens[this->position]);
    }

    if (this->tokens[this->position].value[0] != 'd') {
        this->throw_not_an_integer(this->tokens[this->position]);
    }

    long long row = std::stoll(this->tokens[this->position].value);
    this->position++;
    return CellExpression(column, row);
}

Expression Parser::parse_ranged_expression() {
    CellExpression corner1 = parse_cell_expression();
    if (this->tokens[this->position].token_type != TokenType::COLON) {
        return corner1;
    }

    this->position++;

    CellExpression corner2 = parse_cell_expression();
    return RangedExpression(corner1, corner2);
}

CallExpression Parser::parse_call_expression(const std::string& function_name) {
    if (this->tokens[this->position].token_type != TokenType::LEFT_PARENTHESES) {
        this->throw_not_matching_token(TokenType::LEFT_PARENTHESES, this->tokens[this->position]);
    }

    this->position++;

    bool comma = false;
    std::vector<Expression> arguments;
    while (this->tokens[this->position].token_type != TokenType::RIGHT_PARENTHESES) {
        if (comma && this->tokens[this->position].token_type != TokenType::COMMA) {
            this->throw_not_matching_token(TokenType::COMMA, this->tokens[this->position]);
        }

        comma = true;
        this->position++;
        while (this->tokens[this->position].token_type == TokenType::COMMA) {
            arguments.push_back(NumberExpression(0, false));
            this->position++;
        }

        if (this->tokens[this->position].token_type == TokenType::RIGHT_PARENTHESES) {
            break;
        }

        arguments.push_back(this->parse_expression());
    }

    return CallExpression(function_name, arguments);
}

void Parser::throw_invalid_syntax_error(const Token& token) {
    std::stringstream ss;
    ss << "Invalid syntax at " << token.column << ":" << token.line;
    throw std::runtime_error(ss.str());
}

void Parser::throw_not_matching_token(const TokenType& expected, const Token& token) {
    std::stringstream ss;
    ss << "Expected '" << type_to_str[expected] << "' at " << token.column << ":" << token.line << ", got " << token.value;
    throw std::runtime_error(ss.str());
}

void Parser::throw_not_an_integer(const Token& token) {
    std::stringstream ss;
    ss << "Expected an integer at " << token.column << ":" << token.line << ", got " << token.value.substr(1, token.value.size()-1);
    throw std::runtime_error(ss.str());
}

void Parser::advance_newline() {
    while (this->tokens[this->position].token_type == TokenType::NEWLINE || this->tokens[this->position].token_type == TokenType::SEMICOLON) {
        this->position++;
    }
}

void Parser::newline_check() {
    if (this->tokens[this->position].token_type != TokenType::NEWLINE && this->tokens[this->position].token_type != TokenType::SEMICOLON && this->tokens[this->position].token_type != TokenType::END_OF_FILE) {
        std::stringstream ss;
        ss << "Expected newline or semicolon at " << this->tokens[this->position].column << ":" << this->tokens[this->position].line << ", got " << this->tokens[this->position].value;
        throw std::runtime_error(ss.str());
    }
}

Parser create_parser(const std::vector<Token>& tokens) {
    return Parser(tokens);
}