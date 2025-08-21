#include "node_types.hh"
#include "expressions.hh"
#include <vector>

#pragma once

class BlockStatement : public Statement {
public:
    BlockStatement(std::vector<Statement*> block);
    std::vector<Statement*> block;
};

class CellAssignmentStatement : public Statement {
public:
    CellAssignmentStatement(CellExpression* assignee, Expression* value);
    CellExpression* assignee;
    Expression* value;
};

class RangeAssignmentStatement : public Statement {
public:
    RangeAssignmentStatement(RangedExpression* assignee, Expression* value);
    RangedExpression* assignee;
    Expression* value;
};

class ExpressionStatement : public Statement {
public:
    ExpressionStatement(Expression* expression);
    Expression* expression;
};