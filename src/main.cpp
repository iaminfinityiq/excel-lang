#include "lexer/lexer.hpp"
#include <iostream>
#include <vector>
#include <string>

int main() {
    std::string code = "something = abc(A2:A3)";
    Lexer lexer = create_lexer(code);
    auto tokens = lexer.tokenize();

    for (const auto& token : tokens) {
        std::cout << token.value << std::endl;
    }

    return 0;
}
