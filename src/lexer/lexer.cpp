#include "lexer.hpp"
#include <cctype>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

// Static map for single-character tokens
static const std::unordered_map<char, TokenType> SINGLE_CHAR_TOKENS = {
    {'+', TokenType::PLUS},
    {'-', TokenType::MINUS},
    {'*', TokenType::MULTIPLY},
    {'/', TokenType::DIVIDE},
    {'(', TokenType::LEFT_PARENTHESES},
    {')', TokenType::RIGHT_PARENTHESES},
    {'=', TokenType::EQUALS},
    {':', TokenType::COLON},
};

Lexer::Lexer(const std::string &source) : source_(source), position_(0) {}

Token Lexer::create_token(TokenType token_type,
                          const std::string &value) const {
  return Token{token_type, value};
}

std::vector<Token> Lexer::tokenize() {
  std::vector<Token> tokens;
  tokens.reserve(source_.size() / 4); // Reserve space to reduce reallocations

  while (position_ < source_.size()) {
    char current_char = source_[position_];

    // Skip whitespace
    if (std::isspace(current_char)) {
      skip_whitespace();
      continue;
    }

    // Handle numbers (including decimals)
    if (std::isdigit(current_char) || current_char == '.') {
      tokens.push_back(tokenize_number());
      continue;
    }

    // Handle identifiers
    if (std::isalpha(current_char) || current_char == '_') {
      tokens.push_back(tokenize_identifier());
      continue;
    }

    // Handle single-character tokens
    auto it = SINGLE_CHAR_TOKENS.find(current_char);
    if (it != SINGLE_CHAR_TOKENS.end()) {
      tokens.push_back(create_token(it->second, std::string(1, current_char)));
      position_++;
      continue;
    }

    // Invalid character
    throw_invalid_character_error(current_char);
  }

  return tokens;
}

void Lexer::skip_whitespace() {
  while (position_ < source_.size() && std::isspace(source_[position_])) {
    position_++;
  }
}

Token Lexer::tokenize_number() {
  std::string number;
  bool has_decimal = false;

  while (position_ < source_.size()) {
    char current_char = source_[position_];

    if (std::isdigit(current_char)) {
      number += current_char;
      position_++;
    } else if (current_char == '.' && !has_decimal) {
      has_decimal = true;
      number += current_char;
      position_++;
    } else {
      break;
    }
  }

  // Handle the case where token starts with '.' but has no digits
  if (number == ".") {
    throw std::runtime_error("Invalid number format: standalone decimal point");
  }

  // Add 'd' prefix for decimal numbers (preserving original behavior)
  if (has_decimal) {
    number = "d" + number;
  }

  return create_token(TokenType::NUMBER, number);
}

Token Lexer::tokenize_identifier() {
  std::string identifier;

  while (position_ < source_.size()) {
    char current_char = source_[position_];

    if (std::isalnum(current_char) || current_char == '_') {
      identifier += current_char;
      position_++;
    } else {
      break;
    }
  }

  return create_token(TokenType::IDENTIFIER, identifier);
}

void Lexer::throw_invalid_character_error(char invalid_char) const {
  std::ostringstream oss;
  oss << "Invalid character: '" << invalid_char << "' at position "
      << position_;
  throw std::runtime_error(oss.str());
}

// Factory function for creating lexer instances
Lexer create_lexer(const std::string &source) { return Lexer(source); }
