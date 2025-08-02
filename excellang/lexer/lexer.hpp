#include <string>
#include <vector>

#pragma once

enum class TokenType;

struct Token {
    TokenType token_type;
    std::string value;
};

struct Lexer {
    std::string source;
    long long position;

    std::vector<Token> tokenize();
};

Lexer create_lexer(std::string source);
