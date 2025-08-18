#include "interpolation.hpp"
#include <string>
#include <sstream>
#include <stdexcept>
#include "../parser/node_types.hpp"
#include "../parser/statements.hpp"
#include "../parser/expressions.hpp"

Number::Number(const long long start_column, const long long start_line, const double value) {
    this->data_type = DataType::NUMBER;
    this->value = value;

    this->start_column = start_column;
    this->start_line = start_line;
}

String::String(const long long start_column, const long long start_line, const std::string value) {
    this->data_type = DataType::STRING;
    this->value = value;

    this->start_column = start_column;
    this->start_line = start_line;
}

Interpolator::Interpolator(BlockStatement* ast) {
    this->ast = ast;
    this->instructions.clear();
}

void Interpolator::interpolate() {
    this->interpolate_block_statement(this->ast);
}

// Statements
void Interpolator::interpolate_statement(Statement* statement) {
    switch (statement->node_type) {
    case NodeType::BLOCK_STATEMENT:
        this->interpolate_block_statement(static_cast<BlockStatement*>(statement));
        break;
    case NodeType::CELL_ASSIGNMENT_STATEMENT:
        this->interpolate_cell_assignment_statement(static_cast<CellAssignmentStatement*>(statement));
        break;
    case NodeType::RANGE_ASSIGNMENT_STATEMENT:
        this->interpolate_range_assignment_statement(static_cast<RangeAssignmentStatement*>(statement));
        break;
    case NodeType::EXPRESSION_STATEMENT:
        this->interpolate_expression_statement(static_cast<ExpressionStatement*>(statement));
        break;
    default:
        this->throw_statement_node_type_not_supported(statement);
    }
}

void Interpolator::interpolate_block_statement(BlockStatement* block) {
    for (long long i = 0; i < block->block.size(); i++) {
        this->interpolate_statement(block->block[i]);
    }
}

void Interpolator::interpolate_cell_assignment_statement(CellAssignmentStatement* cell_assignment) {
    this->interpolate_expression(cell_assignment->value);
    this->instructions.push_back(Instruction{InstructionType::STOC, cell_assignment->start_column, cell_assignment->start_line, {String(cell_assignment->assignee->column.column, cell_assignment->assignee->column.line, cell_assignment->assignee->column.value), Number(cell_assignment->assignee->row.column, cell_assignment->assignee->row.line, std::stoll(cell_assignment->assignee->row.value))}});
}

void Interpolator::interpolate_range_assignment_statement(RangeAssignmentStatement* range_assignment) {
    this->interpolate_expression(range_assignment->value);
    this->instructions.push_back(Instruction{InstructionType::STOR, range_assignment->start_column, range_assignment->start_line, {String(range_assignment->assignee->lhs->column.column, range_assignment->assignee->lhs->column.line, range_assignment->assignee->lhs->column.value), Number(range_assignment->assignee->lhs->row.column, range_assignment->assignee->lhs->row.line, std::stoll(range_assignment->assignee->lhs->row.value)), String(range_assignment->assignee->rhs->column.column, range_assignment->assignee->rhs->column.line, range_assignment->assignee->rhs->column.value), Number(range_assignment->assignee->rhs->row.column, range_assignment->assignee->rhs->row.line, std::stoll(range_assignment->assignee->rhs->row.value))}});
}

void Interpolator::interpolate_expression_statement(ExpressionStatement* expression) {
    this->interpolate_expression(expression->expression);
}

// Expressions
void Interpolator::interpolate_expression(Expression* expression) {
    switch (expression->node_type) {
    case NodeType::BINARY_EXPRESSION:
        this->interpolate_binary_expression(static_cast<BinaryExpression*>(expression));
        break;
    case NodeType::UNARY_EXPRESSION:
        this->interpolate_unary_expression(static_cast<UnaryExpression*>(expression));
        break;
    case NodeType::CALL_EXPRESSION:
        this->interpolate_call_expression(static_cast<CallExpression*>(expression));
        break;
    case NodeType::NUMBER_EXPRESSION:
        this->interpolate_number_expression(static_cast<NumberExpression*>(expression));
        break;
    case NodeType::NULL_EXPRESSION:
        this->interpolate_null_expression(static_cast<NullExpression*>(expression));
        break;
    case NodeType::CELL_EXPRESSION:
        this->interpolate_cell_expression(static_cast<CellExpression*>(expression));
        break;
    case NodeType::RANGED_EXPRESSION:
        this->interpolate_ranged_expression(static_cast<RangedExpression*>(expression));
        break;
    default:
        this->throw_expression_node_type_not_supported(expression);
    }
}

void Interpolator::interpolate_binary_expression(BinaryExpression* binary) {
    this->interpolate_expression(binary->lhs);
    this->interpolate_expression(binary->rhs);

    switch (binary->op.token_type) {
    case TokenType::PLUS:
        this->instructions.push_back(Instruction{InstructionType::ADD, binary->start_column, binary->start_line, {}});
        break;
    case TokenType::MINUS:
        this->instructions.push_back(Instruction{InstructionType::SUB, binary->start_column, binary->start_line, {}});
        break;
    case TokenType::MULTIPLY:
        this->instructions.push_back(Instruction{InstructionType::MUL, binary->start_column, binary->start_line, {}});
        break;
    case TokenType::DIVIDE:
        this->instructions.push_back(Instruction{InstructionType::DIV, binary->start_column, binary->start_line, {}});
        break;
    default:
        this->throw_binary_expression_sign_not_supported(binary);
    }
}

void Interpolator::interpolate_unary_expression(UnaryExpression* unary) {
    this->interpolate_expression(unary->value);

    switch (unary->sign.token_type) {
    case TokenType::PLUS:
        this->instructions.push_back(Instruction{InstructionType::UPLUS, unary->start_column, unary->start_line, {}});
        break;
    case TokenType::MINUS:
        this->instructions.push_back(Instruction{InstructionType::UMINUS, unary->start_column, unary->start_line, {}});
        break;
    default:
        this->throw_unary_expression_sign_not_supported(unary);
    }
}

void Interpolator::interpolate_call_expression(CallExpression* call) {
    long long argument_amount = call->arguments.size();
    for (long long i = 0; i < argument_amount; i++) {
        this->interpolate_expression(call->arguments[i]);
    }

    this->instructions.push_back(Instruction{InstructionType::CALL, call->start_column, call->start_line, {String(call->function_name.column, call->function_name.line, call->function_name.value), Number(0, 0, argument_amount)}});
}

void Interpolator::interpolate_number_expression(NumberExpression* number) {
    this->instructions.push_back(Instruction{InstructionType::PUSH, number->start_column, number->start_line, {Number(number->start_column, number->start_line, number->value)}});
}

void Interpolator::interpolate_null_expression(NullExpression* null) {
    this->instructions.push_back(Instruction{InstructionType::PUSH, null->start_column, null->start_line, {Number(null->start_column, null->start_line, 0)}});
}

void Interpolator::interpolate_cell_expression(CellExpression* cell) {
    this->instructions.push_back(Instruction{InstructionType::LODC, cell->start_column, cell->start_line, {String(cell->column.column, cell->column.line, cell->column.value), Number(cell->row.column, cell->row.line, std::stoll(cell->row.value))}});
}

void Interpolator::interpolate_ranged_expression(RangedExpression* ranged) {
    this->instructions.push_back(Instruction{InstructionType::LODR, ranged->start_column, ranged->start_line, {String(ranged->lhs->column.column, ranged->lhs->column.line, ranged->lhs->column.value), Number(ranged->lhs->row.column, ranged->lhs->row.line, std::stoll(ranged->lhs->row.value)), String(ranged->rhs->column.column, ranged->rhs->column.line, ranged->rhs->column.value), Number(ranged->rhs->row.column, ranged->rhs->row.line, std::stoll(ranged->rhs->row.value))}});
}

// Errors
void Interpolator::throw_statement_node_type_not_supported(Statement* statement) {
    std::stringstream ss;
    ss << "Node type " << static_cast<int>(statement->node_type) << " at " << statement->start_column << ":" << statement->start_line << " not supported for statements or it is an expression node.";
    throw std::runtime_error(ss.str());
}

void Interpolator::throw_expression_node_type_not_supported(Expression* expression) {
    std::stringstream ss;
    ss << "Node type " << static_cast<int>(expression->node_type) << " at " << expression->start_column << ":" << expression->start_line << " not supported for expressions or it is an statement node.";
    throw std::runtime_error(ss.str());
}

void Interpolator::throw_binary_expression_sign_not_supported(BinaryExpression* binary) {
    std::stringstream ss;
    ss << "Sign '" << binary->op.value << "' at " << binary->op.column << ":" << binary->op.line << " not supported for binary expression.";
    throw std::runtime_error(ss.str());
}

void Interpolator::throw_unary_expression_sign_not_supported(UnaryExpression* unary) {
    std::stringstream ss;
    ss << "Sign '" << unary->sign.value << "' at " << unary->sign.column << ":" << unary->sign.line << " not supported for unary expression.";
    throw std::runtime_error(ss.str());
}

Interpolator* create_interpolator(BlockStatement* ast) {
    return new Interpolator(ast);
}
