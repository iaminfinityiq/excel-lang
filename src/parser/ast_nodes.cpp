#include "ast_nodes.hpp"
#include "nodes.hpp"

// Statements
NodeType BlockStatement::node_type() {
    return NodeType::BLOCK_STATEMENT;
}

NodeType ExpressionStatement::node_type() {
    return NodeType::EXPRESSION_STATEMENT;
};

NodeType AssignmentStatement::node_type() {
    return NodeType::ASSIGNMENT_STATEMENT;
}

// Expressions
NodeType BinaryExpression::node_type() {
    return NodeType::BINARY_EXPRESSION;
}

NodeType UnaryExpression::node_type() {
    return NodeType::UNARY_EXPRESSION;
}

NodeType CallExpression::node_type() {
    return NodeType::CALL_EXPRESSION;
}

NodeType NumberExpression::node_type() {
    return NodeType::NUMBER_EXPRESSION;
}

NodeType CellExpression::node_type() {
    return NodeType::CELL_EXPRESSION;
}

NodeType RangedExpression::node_type() {
    return NodeType::RANGED_EXPRESSION;
}