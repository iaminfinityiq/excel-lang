#include "nodes.hpp"
#include <string>
#include <vector>
#include <variant>

#pragma once

// Expressions
struct BinaryExpression : Expression {
    Expression lhs;
    std::string op;
    Expression rhs;

    NodeType node_type();
};

struct UnaryExpression : Expression {
    std::string sign;
    Expression value;

    NodeType node_type();
};

struct CallExpression : Expression {
    std::string function_name;
    std::vector<Expression> arguments;

    NodeType node_type();
};

struct NumberExpression : Expression {
    double value;
    bool has_decimal;

    NodeType node_type();
};

struct CellExpression : Expression {
    std::string column;
    long long row;

    NodeType node_type();
};

struct RangedExpression : Expression {
    CellExpression lhs;
    CellExpression rhs;

    NodeType node_type();
};

// Statements
struct BlockStatement : Statement {
    std::vector<Statement> body;
    NodeType node_type();
};

struct ExpressionStatement : Statement {
    Expression expr;
    NodeType node_type();
};

struct AssignmentStatement : Statement {
    std::variant<CellExpression, RangedExpression> assignee;
    Expression value;

    NodeType node_type();
};
