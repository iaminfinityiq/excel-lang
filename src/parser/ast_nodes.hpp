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

    BinaryExpression(const Expression& lhs, const std::string& op, const Expression& rhs);
    NodeType node_type();
};

struct UnaryExpression : Expression {
    std::string sign;
    Expression value;

    UnaryExpression(const std::string& sign, const Expression& value);
    NodeType node_type();
};

struct CallExpression : Expression {
    std::string function_name;
    std::vector<Expression> arguments;

    CallExpression(const std::string& function_name, const std::vector<Expression>& arguments);
    NodeType node_type();
};

struct NumberExpression : Expression {
    double value;
    bool has_decimal;

    NumberExpression(const double& value, const bool& has_decimal);
    NodeType node_type();
};

struct CellExpression : Expression {
    std::string column;
    long long row;

    CellExpression(const std::string& column, const long long& row);
    NodeType node_type();
};

struct RangedExpression : Expression {
    CellExpression lhs;
    CellExpression rhs;

    RangedExpression(const CellExpression& lhs, const CellExpression& rhs);
    NodeType node_type();
};

// Statements
struct BlockStatement : Statement {
    std::vector<Statement> body;

    BlockStatement();
    NodeType node_type();
};

struct ExpressionStatement : Statement {
    Expression expr;

    ExpressionStatement(const Expression& expr);
    NodeType node_type();
};

struct AssignmentStatement : Statement {
    std::variant<CellExpression, RangedExpression> assignee;
    Expression value;

    AssignmentStatement(const std::variant<CellExpression, RangedExpression>& assignee, const Expression& value);
    NodeType node_type();
};