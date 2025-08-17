#include <vector>
#include <string>
#include "../parser/statements.hpp"

#pragma once

enum class DataType {
    NUMBER,
    STRING
};

class RuntimeValue {
public:
    DataType data_type;
    long long start_column;
    long long start_line;
};

class Number : public RuntimeValue {
public:
    Number(const long long start_column, const long long start_line, const double value);
    double value;
};

class String : public RuntimeValue {
public:
    String(const long long start_column, const long long start_line, std::string value);
    std::string value;
};

enum class InstructionType {
    NOP, // Format: NOP. This is a placeholder
    PUSH, // Format: PUSH value (value = expr). Pushes the value to the stack.
    POP, // Format: POP. Pops the value from the stack.
    ADD, // Format: ADD. Pops the 2 top values from the stack and pushes the added result.
    SUB, // Format: SUB. Pops the 2 top values from the stack and pushes the subtracted result.
    MUL, // Format: MUL. Pops the 2 top values from the stack and pushes the multiplied result.
    DIV, // Format: DIV. Pops the 2 top values from the stack and pushes the divided result.
    UPLUS, // Format: UPLUS. Pops the top value and pushes the unary plus result on the popped value.
    UMINUS, // Format: UMINUS. Pops the top value and pushes the unary minus result on the popped value.
    STOC, // Format: STOC column row (column = string, row = number). Pops the top value and store it to the f"{column}{row}" cell.
    LODC, // Format: LODC column row (column = string, row = number). Pushes the value of cell f"{column}{row}" to the stack.
    STOR, // Format: STOR column1 row1 column2 row2 (column1, column2 = string, row1, row2 = number). Pops the top value and store it to the f"{column1}{row1}:{column2}{row2}" range.
    LODR, // Format: LODR column1 row1 column2 row2 (column1, column2 = string, row1, row2 = number). Returns the top-left corner value and pushes it to the stack.
    CALL // Format: CALL function argument_amount (function = string, argument_amount = number). Pops the corresponding argument_amount arguments and pass them correspondingly to function(). This also pushes the returned value to the stack.
};

struct Instruction {
    InstructionType instruction_type;
    long long start_column;
    long long start_row;
    std::vector<RuntimeValue> arguments;
};

class Interpolator {
public:
    Interpolator(BlockStatement* ast);
    void interpolate();
private:
    BlockStatement* ast;
    std::vector<Instruction> instructions;

    void interpolate_statement(Statement* statement);
    void interpolate_expression(Expression* expression);

    // Statements
    void interpolate_block_statement(BlockStatement* block);
    void interpolate_cell_assignment_statement(CellAssignmentStatement* cell_assignment);
    void interpolate_range_assignment_statement(RangeAssignmentStatement* range_assignment);
    void interpolate_expression_statement(ExpressionStatement* expression);

    // Expressions
    void interpolate_binary_expression(BinaryExpression* binary);
    void interpolate_unary_expression(UnaryExpression* unary);
    void interpolate_call_expression(CallExpression* call);
    void interpolate_number_expression(NumberExpression* number);
    void interpolate_null_expression(NullExpression* null);
    void interpolate_cell_expression(CellExpression* cell);
    void interpolate_ranged_expression(RangedExpression* range);

    // Errors
    void throw_statement_node_type_not_supported(Statement* statement);
    void throw_expression_node_type_not_supported(Expression* expression);
    void throw_binary_expression_sign_not_supported(BinaryExpression* binary);
    void throw_unary_expression_sign_not_supported(UnaryExpression* unary);
};