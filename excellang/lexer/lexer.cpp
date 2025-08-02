#include "lexer.hpp"
#include <vector>
#include <stdexcept>
#include <sstream>
#include <string>
#include <iostream>
#include <map>

enum class TokenType {
    END_OF_FILE,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    LEFT_PARENTHESES,
    RIGHT_PARENTHESES,
    COLON,
    EQUALS,
    NUMBER,
    STRING,
    IDENTIFIER
};

Token create_token(TokenType token_type, std::string value) {
    Token token;
    token.token_type = token_type;
    token.value = value;

    return token;
}

std::vector<Token> Lexer::tokenize() {
    std::map<char, TokenType> one_char_map;
    one_char_map['+'] = TokenType::PLUS;
    one_char_map['-'] = TokenType::MINUS;
    one_char_map['*'] = TokenType::MULTIPLY;
    one_char_map['/'] = TokenType::DIVIDE;
    one_char_map['('] = TokenType::LEFT_PARENTHESES;
    one_char_map[')'] = TokenType::RIGHT_PARENTHESES;
    one_char_map['='] = TokenType::EQUALS;
    one_char_map[':'] = TokenType::COLON;

    std::vector<Token> tokens(0);

    int size = this->source.size();
    while (this->position < size) {
        if (one_char_map.find(this->source[this->position]) == one_char_map.end()) {
            if (this->source[this->position] == ' ' || this->source[this->position] == '\t') {
                this->position++;
                continue;
            }

            if (this->source[this->position] == '.' || ('0' <= this->source[this->position] && this->source[this->position] <= '9')) {
                std::string number = "";
                bool dot = false;
                while (this->position < size && (this->source[this->position] == '.' || ('0' <= this->source[this->position] && this->source[this->position] <= '9'))) {
                    if (this->source[this->position] == '.') {
                        if (dot) {
                            break;
                        }

                        dot = true;
                    }

                    number += this->source[this->position];
                    this->position++;
                }

                if (dot) {
                    number = "d" + number;
                }

                tokens.push_back(create_token(TokenType::NUMBER, number));
                continue;
            }
            
            if (('A' <= this->source[this->position] && this->source[this->position] <= 'Z') || ('a' <= this->source[this->position] && this->source[this->position] <= 'z')) {
                std::string identifier = "";
                while (this->position < size && ('A' <= this->source[this->position] && this->source[this->position] <= 'Z') || ('a' <= this->source[this->position] && this->source[this->position] <= 'z')) {
                    identifier += this->source[this->position];
                    this->position++;
                }

                tokens.push_back(create_token(TokenType::IDENTIFIER, identifier));
                continue;
            }

            std::stringstream ss;
            ss << "Invalid character: '" << this->source[this->position] << "'\n";
            throw std::runtime_error(ss.str());
        }

        std::string token_string = "";
        token_string += this->source[this->position];
        tokens.push_back(create_token(one_char_map[this->source[this->position]], token_string));
        this->position++;
    }

    return tokens;
}

Lexer create_lexer(std::string source) {
    Lexer lexer;
    lexer.source = source;
    lexer.position = 0;

    return lexer;
}
