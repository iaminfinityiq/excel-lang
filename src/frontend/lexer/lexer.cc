#include "lexer.hh"
#include <cctype>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>

const std::unordered_map<TokenType, std::string> map = {
    {TokenType::END_OF_FILE, "EOF"},
    {TokenType::NEWLINE, "\\n"},
    {TokenType::SEMICOLON, ";"},
    {TokenType::PLUS, "+"},
    {TokenType::MINUS, "-"},
    {TokenType::MULTIPLY, "*"},
    {TokenType::DIVIDE, "/"},
    {TokenType::LEFT_PARENTHESES, "("},
    {TokenType::RIGHT_PARENTHESES, ")"},
    {TokenType::EQUALS, "="},
    {TokenType::COLON, ":"},
    {TokenType::COMMA, ","},
    {TokenType::NUMBER, "number"},
    {TokenType::IDENTIFIER, "identifier"},
};

std::unordered_map<TokenType, std::string> type_to_str()
{
  return map;
}

// Static map for single-character tokens
static const std::unordered_map<char, TokenType> SINGLE_CHAR_TOKENS = {
    {'\n', TokenType::NEWLINE},
    {';', TokenType::SEMICOLON},
    {'+', TokenType::PLUS},
    {'-', TokenType::MINUS},
    {'*', TokenType::MULTIPLY},
    {'/', TokenType::DIVIDE},
    {'(', TokenType::LEFT_PARENTHESES},
    {')', TokenType::RIGHT_PARENTHESES},
    {'=', TokenType::EQUALS},
    {':', TokenType::COLON},
    {',', TokenType::COMMA},
};

Lexer::Lexer(const std::string &source) : source_(source), position_(0), line_(1), col_(1) {}

void Lexer::advance()
{
  char current_char = source_[position_];
  position_++;
  if (current_char == '\n')
  {
    line_++;
    col_ = 1;
  }
  else
  {
    col_++;
  }
}

Token Lexer::create_token(TokenType token_type,
                          const std::string &value) const
{
  return Token{token_type, value, line_, col_};
}

std::vector<Token> Lexer::tokenize()
{
  std::vector<Token> tokens;
  tokens.reserve(source_.size() / 4); // Reserve space to reduce reallocations

  while (position_ < source_.size())
  {
    char current_char = source_[position_];

    // Skip whitespace
    if (std::isspace(current_char))
    {
      skip_whitespace();
      continue;
    }

    // Handle numbers (including decimals)
    if (std::isdigit(current_char) || current_char == '.')
    {
      tokens.push_back(tokenize_number());
      continue;
    }

    // Handle identifiers
    if (std::isalpha(current_char) || current_char == '_')
    {
      tokens.push_back(tokenize_identifier());
      continue;
    }

    // Handle single-character tokens
    auto it = SINGLE_CHAR_TOKENS.find(current_char);
    if (it != SINGLE_CHAR_TOKENS.end())
    {
      tokens.push_back(create_token(it->second, std::string(1, current_char)));
      advance();
      continue;
    }

    // Invalid character
    throw_invalid_character_error(current_char);
  }

  tokens.push_back(create_token(TokenType::END_OF_FILE, "EOF"));
  return tokens;
}

void Lexer::skip_whitespace()
{
  while (position_ < source_.size() && std::isspace(source_[position_]))
  {
    advance();
  }
}

Token Lexer::tokenize_number()
{
  std::string number;
  bool has_decimal = false;

  uint64_t start_col = col_;
  uint64_t start_line = line_;
  while (position_ < source_.size())
  {
    char current_char = source_[position_];

    if (std::isdigit(current_char))
    {
      number += current_char;
      advance();
    }
    else if (current_char == '.' && !has_decimal)
    {
      has_decimal = true;
      number += current_char;
      advance();
    }
    else
    {
      break;
    }
  }

  // Handle the case where token starts with '.' but has no digits
  if (number == ".")
  {
    throw std::runtime_error("Invalid number format: standalone decimal point");
  }

  // Add 'd' prefix for decimal numbers (preserving original behavior)
  if (has_decimal)
  {
    number = "d" + number;
  }

  uint64_t end_col = col_;
  uint64_t end_line = line_;

  col_ = start_col;
  line_ = start_line;

  Token returned = create_token(TokenType::NUMBER, number);

  col_ = end_col;
  line_ = end_line;

  return returned;
}

Token Lexer::tokenize_identifier()
{
  std::string identifier;

  uint64_t start_col = col_;
  uint64_t start_line = line_;
  while (position_ < source_.size())
  {
    char current_char = source_[position_];

    if (std::isalpha(current_char))
    {
      identifier += current_char;
      advance();
    }
    else
    {
      break;
    }
  }

  uint64_t end_col = col_;
  uint64_t end_line = line_;

  col_ = start_col;
  line_ = start_line;

  Token returned = create_token(TokenType::IDENTIFIER, identifier);

  col_ = end_col;
  line_ = end_line;

  return returned;
}

void Lexer::throw_invalid_character_error(char invalid_char) const
{
  std::ostringstream oss;
  oss << "Invalid character: '" << invalid_char << "' at " << col_ << ":" << line_;
  throw std::runtime_error(oss.str());
}

// Factory function for creating lexer instances
Lexer *create_lexer(const std::string &source)
{
  return new Lexer(source);
}