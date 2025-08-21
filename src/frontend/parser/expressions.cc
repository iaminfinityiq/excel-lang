#include "lexer/lexer.hh"
#include "node_types.hh"
#include "expressions.hh"
#include <vector>
#include <string>
#include <stdexcept>
#include <sstream>

void throw_not_a_valid_row_number(Token token) {
    std::stringstream ss;
    ss << token.value.substr(1, token.value.size()-1) << " at " << token.column << ":" << token.line << " is not a valid row number!";

    throw std::runtime_error(ss.str());
}

BinaryExpression::BinaryExpression(Expression* lhs, Token op, Expression* rhs) {
    this->node_type = NodeType::BINARY_EXPRESSION;
    this->lhs = lhs;
    this->op = op;
    this->rhs = rhs;

    this->start_column = this->lhs->start_column;
    this->start_line = this->lhs->start_line;
}

UnaryExpression::UnaryExpression(Token sign, Expression* value) {
    this->node_type = NodeType::UNARY_EXPRESSION;
    this->sign = sign;
    this->value = value;

    this->start_column = this->sign.column;
    this->start_line = this->sign.line;
}

CallExpression::CallExpression(Token function_name_token, std::vector<Expression*> arguments) {
    this->node_type = NodeType::CALL_EXPRESSION;
    this->function_name = function_name_token;
    this->arguments = arguments;

    this->start_column = function_name_token.column;
    this->start_line = function_name_token.line;
}

NumberExpression::NumberExpression(Token number_token) {
    this->node_type = NodeType::NUMBER_EXPRESSION;
    if (number_token.value[0] == 'd') {
        this->has_decimal = true;
        this->value = std::stod(number_token.value.substr(1, number_token.value.size()-1));
    } else {
        this->has_decimal = false;
        this->value = std::stod(number_token.value);
    }

    this->start_column = number_token.column;
    this->start_line = number_token.line;
}

NullExpression::NullExpression(Token semicolon) {
    this->node_type = NodeType::NULL_EXPRESSION;
    this->start_column = semicolon.column;
    this->start_line = semicolon.line;
}

CellExpression::CellExpression(Token column_token, Token row_token) {
    this->node_type = NodeType::CELL_EXPRESSION;
    this->column = column_token;
    if (row_token.value[0] == 'd' || std::stoll(row_token.value) < 0) {
        throw_not_a_valid_row_number(row_token); // This works because of short-circuit expressions
    }

    this->row = row_token;
    
    this->start_column = column_token.column;
    this->start_line = column_token.line;
}

RangedExpression::RangedExpression(CellExpression* lhs, CellExpression* rhs) {
    this->node_type = NodeType::RANGED_EXPRESSION;
    this->lhs = lhs;
    this->rhs = rhs;

    this->start_column = this->lhs->start_column;
    this->start_line = this->lhs->start_line;
}