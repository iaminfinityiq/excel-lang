#include "lexer/lexer.hpp"
#include <iostream>
#include <vector>

int main() {
    Lexer lexer = create_lexer("Hello = 45");
    auto tokens = lexer.tokenize();

    for (const auto& token : tokens) {
        std::cout << token.value << std::endl;
    }

    return 0;
}
