#pragma once

#include <string>

typedef enum {
  TOKEN_EOF,
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_IDENTIFIER,
  TOKEN_LBRACE,
  TOKEN_RBRACE,
  TOKEN_SEMI,
  TOKEN_STRING,
  TOKEN_INTEGER,
  TOKEN_COMMA,
  TOKEN_EQUALS,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_STAR,
  TOKEN_SLASH,
  TOKEN_BANG,
  TOKEN_EQUALS_EQUALS,
  TOKEN_BANG_EQUALS,
  TOKEN_LESS_THAN,
  TOKEN_MORE_THAN,
  TOKEN_LESS_THAN_EQUALS,
  TOKEN_MORE_THAN_EQUALS,
  TOKEN_PIPE_PIPE,
  TOKEN_AND_AND,
  TOKEN_PERCENT,
} TokenType;

typedef struct {
  TokenType type;
  std::string value;
  int line;
  int column;
} Token;

std::string printableTokenType(TokenType type);
std::string printableToken(Token token);
