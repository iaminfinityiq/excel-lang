#include "ast_nodes.hpp"
#include "nodes.hpp"
#include <string>
#include <vector>
#include <variant>

// Expressions
BinaryExpression::BinaryExpression(const Expression& lhs, const std::string& op, const Expression& rhs) : lhs(lhs), op(op), rhs(rhs) {}
NodeType BinaryExpression::node_type() {
    return NodeType::BINARY_EXPRESSION;
}

UnaryExpression::UnaryExpression(const std::string& sign, const Expression& value) : sign(sign), value(value) {}
NodeType UnaryExpression::node_type() {
    return NodeType::UNARY_EXPRESSION;
}

CallExpression::CallExpression(const std::string& function_name, const std::vector<Expression>& arguments) : function_name(function_name), arguments(arguments) {}
NodeType CallExpression::node_type() {
    return NodeType::CALL_EXPRESSION;
}

NumberExpression::NumberExpression(const double& value, const bool& has_decimal) : value(value), has_decimal(has_decimal) {}
NodeType NumberExpression::node_type() {
    return NodeType::NUMBER_EXPRESSION;
}

CellExpression::CellExpression(const std::string& column, const long long& row) : column(column), row(row) {}
NodeType CellExpression::node_type() {
    return NodeType::CELL_EXPRESSION;
}

RangedExpression::RangedExpression(const CellExpression& lhs, const CellExpression& rhs) : lhs(lhs), rhs(rhs) {}
NodeType RangedExpression::node_type() {
    return NodeType::RANGED_EXPRESSION;
}

// Statements
BlockStatement::BlockStatement() : body(std::vector<Statement>()) {}
NodeType BlockStatement::node_type() {
    return NodeType::BLOCK_STATEMENT;
}

ExpressionStatement::ExpressionStatement(const Expression& expr) : expr(expr) {}
NodeType ExpressionStatement::node_type() {
    return NodeType::EXPRESSION_STATEMENT;
};

AssignmentStatement::AssignmentStatement(const std::variant<CellExpression, RangedExpression>& assignee, const Expression& value) : assignee(assignee), value(value) {}
NodeType AssignmentStatement::node_type() {
    return NodeType::ASSIGNMENT_STATEMENT;
}