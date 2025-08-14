#include "statements.hpp"
#include "node_types.hpp"
#include "parser.hpp"
#include "../lexer/lexer.hpp"
#include <sstream>

Parser::Parser(const std::vector<Token>& tokens) {
    this->tokens = tokens;
    this->position = 0;
}

BlockStatement* Parser::parse() {
    std::vector<Statement*> block(0);
    while (this->tokens[this->position].token_type != TokenType::END_OF_FILE) {
        this->advance_newline();
        if (this->tokens[this->position].token_type == TokenType::END_OF_FILE) {
            break;
        }

        block.push_back(this->parse_statement());
    }

    return new BlockStatement(block);
}

Statement* Parser::parse_statement() {
    switch (this->tokens[this->position].token_type) {
    default:
        Expression* expression = this->parse_expression();
        ExpressionStatement* returned = new ExpressionStatement(expression);
        this->newline_check();
        return returned;
    }
}

Expression* Parser::parse_expression() {
    return this->parse_additive_expression();
}

Expression* Parser::parse_additive_expression() {
    Expression* lhs = this->parse_multiplicative_expression();
    while (this->tokens[this->position].token_type == TokenType::PLUS || this->tokens[this->position].token_type == TokenType::MINUS) {
        Token op = this->tokens[this->position];
        this->position++;
        
        Expression* rhs = this->parse_multiplicative_expression();
        lhs = new BinaryExpression(lhs, op, rhs);
    }

    return lhs;
}

Expression* Parser::parse_multiplicative_expression() {
    Expression* lhs = this->parse_unary_expression();
    while (this->tokens[this->position].token_type == TokenType::MULTIPLY || this->tokens[this->position].token_type == TokenType::DIVIDE) {
        Token op = this->tokens[this->position];
        this->position++;
        
        Expression* rhs = this->parse_unary_expression();
        lhs = new BinaryExpression(lhs, op, rhs);
    }

    return lhs;
}

Expression* Parser::parse_unary_expression() {
    switch (this->tokens[this->position].token_type) {
    case TokenType::PLUS: {
        Token op = this->tokens[this->position];
        this->position++;
        
        return new UnaryExpression(op, this->parse_primary_expression());
    }
    case TokenType::MINUS: {
        Token op = this->tokens[this->position];
        this->position++;
        
        return new UnaryExpression(op, this->parse_primary_expression());
    }
    default:
        return this->parse_primary_expression();
    }
}

Expression* Parser::parse_primary_expression() {
    switch (this->tokens[this->position].token_type) {
    case TokenType::NUMBER: {
        Token current_token = this->tokens[this->position];
        NumberExpression* returned = new NumberExpression(current_token);
        
        this->position++;
        return returned;
    }
    case TokenType::IDENTIFIER: {
        Token identifier = this->tokens[this->position];
        this->position++;

        if (this->tokens[this->position].token_type == TokenType::NUMBER && this->tokens[this->position].value[0] != 'd') {
            this->position--;
            return this->parse_ranged_expression();
        } else {
            return this->parse_call_expression(identifier);
        }
    }
    case TokenType::LEFT_PARENTHESES: {
        long long left_parenthese_column = this->tokens[this->position].column;
        long long left_parenthese_line = this->tokens[this->position].line;

        this->position++;
        Expression* expression = this->parse_expression();
        if (this->tokens[this->position].token_type != TokenType::RIGHT_PARENTHESES) {
            this->throw_not_matching_token(TokenType::RIGHT_PARENTHESES, this->tokens[this->position]);
        }

        this->position++;
        expression->start_column = left_parenthese_column;
        expression->start_line = left_parenthese_line;

        return expression;
    }
    default:
        this->throw_invalid_syntax_error(this->tokens[this->position]);
        return nullptr;
    }
}

CellExpression* Parser::parse_cell_expression() {
    Token column = this->tokens[this->position];
    this->position++;

    if (this->tokens[this->position].token_type != TokenType::NUMBER) {
        Token errored_token = this->tokens[this->position];
        this->throw_not_matching_token(TokenType::NUMBER, errored_token);
    }

    Token row = this->tokens[this->position];
    this->position++;
    
    CellExpression* returned = new CellExpression(column, row);
    return returned;
}

CallExpression* Parser::parse_call_expression(Token function_name_token) {
    if (this->tokens[this->position].token_type != TokenType::LEFT_PARENTHESES) {
        this->throw_not_matching_token(TokenType::LEFT_PARENTHESES, this->tokens[this->position]);
    }

    this->position++;

    bool comma = false;
    std::vector<Expression*> arguments(0);
    while (this->tokens[this->position].token_type != TokenType::RIGHT_PARENTHESES) {
        if (comma && this->tokens[this->position].token_type != TokenType::COMMA) {
            this->throw_not_matching_token(TokenType::COMMA, this->tokens[this->position]);
        }

        comma = true;
        this->position++;
        while (this->tokens[this->position].token_type == TokenType::COMMA) {
            arguments.push_back(new NullExpression(this->tokens[this->position]));
            this->position++;
        }

        if (this->tokens[this->position].token_type == TokenType::RIGHT_PARENTHESES) {
            break;
        }

        arguments.push_back(this->parse_expression());
    }

    this->position++;
    return new CallExpression(function_name_token, arguments);
}

Expression* Parser::parse_ranged_expression() {
    CellExpression* corner1 = parse_cell_expression();
    if (this->tokens[this->position].token_type != TokenType::COLON) {
        return corner1;
    }

    this->position++;

    CellExpression* corner2 = parse_cell_expression();
    RangedExpression* returned = new RangedExpression(corner1, corner2);
    return returned;
}

void Parser::throw_invalid_syntax_error(const Token token) {
    std::stringstream ss;
    ss << "Invalid syntax at " << token.column << ":" << token.line;
    throw std::runtime_error(ss.str());
}

void Parser::throw_not_matching_token(const TokenType& expected, const Token token) {
    std::stringstream ss;
    ss << "Expected '" << type_to_str()[expected] << "' at " << token.column << ":" << token.line << ", got " << token.value;
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

Parser* create_parser(const std::vector<Token>& tokens) {
    return new Parser(tokens);
}