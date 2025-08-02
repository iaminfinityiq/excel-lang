#include <vector>

#pragma once

enum class NodeType {
    BLOCK_STATEMENT,
    EXPRESSION_STATEMENT,
    ASSIGNMENT_STATEMENT,
    BINARY_EXPRESSION,
    UNARY_EXPRESSION,
    CALL_EXPRESSION,
    NUMBER_EXPRESSION,
    CELL_EXPRESSION,
    RANGED_EXPRESSION
};

struct Statement {
    NodeType node_type();
};

struct Expression {
    NodeType node_type();
};