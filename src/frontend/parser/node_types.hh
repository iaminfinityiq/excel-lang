#pragma once
#include <cstdint>

enum class NodeType
{
    // Statements
    BLOCK_STATEMENT,
    CELL_ASSIGNMENT_STATEMENT,
    RANGE_ASSIGNMENT_STATEMENT,
    EXPRESSION_STATEMENT,

    // Expressions
    BINARY_EXPRESSION,
    UNARY_EXPRESSION,
    CALL_EXPRESSION,
    NUMBER_EXPRESSION,
    NULL_EXPRESSION,
    CELL_EXPRESSION,
    RANGED_EXPRESSION
};

class Statement
{
public:
    NodeType node_type;
    uint64_t start_column;
    uint64_t start_line;
};

class Expression
{
public:
    NodeType node_type;
    uint64_t start_column;
    uint64_t start_line;
};