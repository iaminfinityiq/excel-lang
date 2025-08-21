#include "node_types.hh"
#include "lexer/lexer.hh"
#include <string>
#include <vector>

#pragma once

class BinaryExpression : public Expression {
public:
    BinaryExpression(Expression* lhs, Token op, Expression* rhs);
    Expression* lhs;
    Token op;
    Expression* rhs;
};

class UnaryExpression : public Expression {
public:
    UnaryExpression(Token sign, Expression* value);
    Token sign;
    Expression* value;
};

class CallExpression : public Expression {
public:
    using Expression::start_column;
    using Expression::start_line;
    CallExpression(Token function_name_token, std::vector<Expression*> arguments);
    Token function_name;
    std::vector<Expression*> arguments;
};

class NumberExpression : public Expression {
public:
    NumberExpression(Token number_token);
    double value;
    bool has_decimal;
};

class NullExpression : public Expression {
public:
    NullExpression(Token semicolon);
};

class CellExpression : public Expression {
public:
    CellExpression(Token column_token, Token row_token);
    Token column;
    Token row;
};

class RangedExpression : public Expression {
public:
    RangedExpression(CellExpression* lhs, CellExpression* rhs);
    CellExpression* lhs;
    CellExpression* rhs;
};