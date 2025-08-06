#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "parser/ast_nodes.hpp"
#include <iostream>
#include <vector>
#include <string>

int main() {
    std::string code = "1 * 2 - 3";
    Lexer lexer = create_lexer(code);
    std::vector<Token> tokens = lexer.tokenize();

    Parser parser = create_parser(tokens);
    BlockStatement block = parser.parse();

    return 0;
}