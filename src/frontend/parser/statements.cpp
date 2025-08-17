#include "statements.hpp"
#include "node_types.hpp"
#include <vector>

BlockStatement::BlockStatement(std::vector<Statement*> block) {
    this->node_type = NodeType::BLOCK_STATEMENT;
    this->block = block;

    if (this->block.empty()) {
        this->start_column = 0;
        this->start_line = 0;
    } else {
        this->start_column = this->block[0]->start_column;
        this->start_line = this->block[0]->start_line;
    }
}

CellAssignmentStatement::CellAssignmentStatement(CellExpression* assignee, Expression* value) {
    this->node_type = NodeType::CELL_ASSIGNMENT_STATEMENT;
    this->assignee = assignee;
    this->value = value;

    this->start_column = assignee->start_column;
    this->start_line = assignee->start_line;
}

RangeAssignmentStatement::RangeAssignmentStatement(RangedExpression* assignee, Expression* value) {
    this->node_type = NodeType::RANGE_ASSIGNMENT_STATEMENT;
    this->assignee = assignee;
    this->value = value;

    this->start_column = assignee->start_column;
    this->start_line = assignee->start_line;
}

ExpressionStatement::ExpressionStatement(Expression* expression) {
    this->node_type = NodeType::EXPRESSION_STATEMENT;
    this->expression = expression;

    this->start_column = expression->start_column;
    this->start_line = expression->start_line;
}