#include <string>
#include <vector>

#ifndef TOKEN_H
#define TOKEN_H

enum class TokenType;

struct Token {
    TokenType token_type;
    std::string value;
};

#endif

#ifndef LEXER_H
#define LEXER_H

struct Lexer {
    std::string source;
    long long position;

    std::vector<Token> tokenize();
};

Lexer create_lexer(std::string source);

#endif