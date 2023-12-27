#pragma once

#include "token.h"
#include <string>
#include <vector>

class Lexer {
  std::string code;
  int index;

public:
  Lexer(std::string contents);
  void skipWhitespace();
  Token advanceWith(TokenType type);
  Token nextToken();
  std::vector<Token> tokenize();
  char get();
};
