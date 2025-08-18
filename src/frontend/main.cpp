#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "parser/statements.hpp"
#include "parser/node_types.hpp"
#include "interpolation/interpolation.hpp"
#include <iostream>
#include <vector>
#include <string>

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

int main(std::string vm_src) {
    std::string code = "EXCELLANG(A1, A2, ,,,,, 69)";
    Lexer* lexer = create_lexer(code);
    std::vector<Token> tokens = lexer->tokenize();

    Parser* parser = create_parser(tokens);
    BlockStatement* block = parser->parse();
    print_stmt(0, block);

    Interpolator* interpolator = create_interpolator(block);
    interpolator->interpolate();

    return 0;
}
