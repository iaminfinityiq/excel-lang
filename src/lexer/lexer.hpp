#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>

enum class TokenType {
  PLUS,
  MINUS,
  MULTIPLY,
  DIVIDE,
  LEFT_PARENTHESES,
  RIGHT_PARENTHESES,
  EQUALS,
  COLON,
  NUMBER,
  IDENTIFIER
};

struct Token {
  TokenType token_type;
  std::string value;
};

class Lexer {
public:
  explicit Lexer(const std::string &source);

  std::vector<Token> tokenize();

private:
  const std::string &source_;
  size_t position_;

  Token create_token(TokenType token_type, const std::string &value) const;
  void skip_whitespace();
  Token tokenize_number();
  Token tokenize_identifier();
  void throw_invalid_character_error(char invalid_char) const;
};

// Factory function
Lexer create_lexer(const std::string &source);

#endif // LEXER_HPP
