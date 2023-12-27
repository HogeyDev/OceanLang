#pragma once

#include "ast.h"
#include "token.h"
#include <vector>

class Parser {
  std::vector<Token> tokenList;
  int index;
  Token currTok;

public:
  Token eat(TokenType type);
  Token advance();
  Token get(int index);
  Token peek(int offset);
  Parser(std::vector<Token> tokens);
  AST::Scope *parse();
  AST::Scope *scope();
  AST::Expression *expression();
  AST::Expression *comparison();
  AST::Expression *term();
  AST::Expression *factor();
  AST::Expression *unary();
  AST::Expression *primary();
};
