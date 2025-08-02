#include <iostream>
#include <vector>
#include "lexer/lexer.hpp"

int main() {
    Lexer lexer = create_lexer("###################");
    std::vector<Token> tokens = lexer.tokenize();

    for (long long i = 0; i < tokens.size(); i++) {
        Token token = tokens[i];
        std::cout << token.value << "\n";
    }

    return 0;
}