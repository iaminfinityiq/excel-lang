#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

#pragma once

enum class TokenType
{
  END_OF_FILE,
  NEWLINE,
  SEMICOLON,
  PLUS,
  MINUS,
  MULTIPLY,
  DIVIDE,
  LEFT_PARENTHESES,
  RIGHT_PARENTHESES,
  EQUALS,
  COLON,
  COMMA,
  NUMBER,
  IDENTIFIER
};

std::unordered_map<TokenType, std::string> type_to_str();

struct Token
{
  TokenType token_type;
  std::string value;
  uint64_t line;
  uint64_t column;
};

class Lexer
{
public:
  explicit Lexer(const std::string &source);

  std::vector<Token> tokenize();

private:
  const std::string &source_;
  size_t position_;
  uint64_t line_;
  uint64_t col_;

  Token create_token(TokenType token_type, const std::string &value) const;
  void advance();
  void skip_whitespace();
  Token tokenize_number();
  Token tokenize_identifier();
  void throw_invalid_character_error(char invalid_char) const;
};

// Factory function
Lexer *create_lexer(const std::string &source);