#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "parser/statements.hpp"
#include "parser/node_types.hpp"
#include "interpolation/interpolation.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

void print_expr(Expression* expr) {
    switch (expr->node_type) {
    case NodeType::BINARY_EXPRESSION: {
        BinaryExpression* binary = static_cast<BinaryExpression*>(expr);
        std::cout << "(";
        print_expr(binary->lhs);
        std::cout << " " << binary->op.value << " ";
        print_expr(binary->rhs);
        std::cout << ")";
        break;
    }
    case NodeType::UNARY_EXPRESSION: {
        UnaryExpression* unary = static_cast<UnaryExpression*>(expr);
        std::cout << unary->sign.value;
        print_expr(unary->value);
        break;
    }
    case NodeType::CALL_EXPRESSION: {
        CallExpression* call = static_cast<CallExpression*>(expr);
        std::cout << call->function_name.value << "(";

        long long arguments_size = call->arguments.size();
        for (long long i = 0; i < arguments_size; i++) {
            print_expr(call->arguments[i]);
            if (i < arguments_size-1) {
                std::cout << ", ";
            }
        }

        std::cout << ")";
        break;
    }
    case NodeType::NUMBER_EXPRESSION:
        std::cout << static_cast<NumberExpression*>(expr)->value;
        break;
    case NodeType::NULL_EXPRESSION:
        std::cout << "NULL";
        break;
    case NodeType::CELL_EXPRESSION: {
        CellExpression* cell = static_cast<CellExpression*>(expr);
        std::cout << cell->column.value << cell->row.value;
        break;
    }
    case NodeType::RANGED_EXPRESSION: {
        RangedExpression* ranged = static_cast<RangedExpression*>(expr);
        print_expr(ranged->lhs);
        std::cout << ":";
        print_expr(ranged->rhs);
        break;
    }
    }
}

void print_stmt(const long long indentation, Statement* stmt) {
    for (long long i = 0; i < indentation; i++) {
        std::cout << "  ";
    }

    switch (stmt->node_type) {
    case NodeType::BLOCK_STATEMENT: {
        BlockStatement* block = static_cast<BlockStatement*>(stmt);
        std::cout << "(BLOCK STATEMENT length=" << block->block.size() << ", depth=" << indentation <<")\n";
        for (long long i = 0; i < block->block.size(); i++) {
            print_stmt(indentation+1, block->block[i]);
        }

        break;
    }
    case NodeType::EXPRESSION_STATEMENT:
        print_expr(static_cast<ExpressionStatement*>(stmt)->expression);
        break;
    }
}

void debug_instructions(Interpolator* interpolator) {
    for (long long i = 0; i < interpolator->instructions.size(); i++) {
        switch (interpolator->instructions[i].instruction_type) {
        case InstructionType::NOP:
            std::cout << "NOP";
            break;
        case InstructionType::PUSH:
            std::cout << "PUSH";
            break;
        case InstructionType::POP:
            std::cout << "POP";
            break;
        case InstructionType::ADD:
            std::cout << "ADD";
            break;
        case InstructionType::SUB:
            std::cout << "SUB";
            break;
        case InstructionType::MUL:
            std::cout << "MUL";
            break;
        case InstructionType::DIV:
            std::cout << "DIV";
            break;
        case InstructionType::UPLUS:
            std::cout << "UPLUS";
            break;
        case InstructionType::UMINUS:
            std::cout << "UMINUS";
            break;
        case InstructionType::STOC:
            std::cout << "STOC";
            break;
        case InstructionType::STOR:
            std::cout << "STOR";
            break;
        case InstructionType::LODC:
            std::cout << "LODC";
            break;
        case InstructionType::LODR:
            std::cout << "LODR";
            break;
        case InstructionType::CALL:
            std::cout << "CALL";
            break;
        }

        std::cout << " " << interpolator->instructions[i].start_column << ":" << interpolator->instructions[i].start_row;
        for (long long j = 0; j < interpolator->instructions[i].arguments.size(); j++) {
            std::cout << " " << interpolator->instructions[i].arguments[j]->start_column << " " << interpolator->instructions[i].arguments[j]->start_line << " ";
            if (interpolator->instructions[i].arguments[j]->data_type == DataType::NUMBER) {
                std::cout << static_cast<Number*>(interpolator->instructions[i].arguments[j])->value;
            } else {
                std::cout << std::quoted(static_cast<String*>(interpolator->instructions[i].arguments[j])->value);
            }
        }

        std::cout << "\n";
    }
}

int main(std::string vm_src) {
    std::string code = "EXCELLANG(A1, A2, ,,,,, 69)";
    Lexer* lexer = create_lexer(code);
    std::vector<Token> tokens = lexer->tokenize();

    std::cout << "lexer:\n";
    for (long long i = 0; i < tokens.size(); i++) {
        std::cout << tokens[i].column << ":" << tokens[i].line << " " << static_cast<int>(tokens[i].token_type) << " " << tokens[i].value << "\n";
    }

    std::cout << "\n";

    Parser* parser = create_parser(tokens);
    BlockStatement* block = parser->parse();

    std::cout << "parser:\n";
    print_stmt(0, block);
    std::cout << "\n\n";

    Interpolator* interpolator = create_interpolator(block);
    interpolator->interpolate();

    std::cout << "VM:\n";
    debug_instructions(interpolator);
    return 0;
}