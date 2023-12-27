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
} TokenType;

typedef struct {
  TokenType type;
  std::string value;
} Token;

std::string printableTokenType(TokenType type);
std::string printableToken(Token token);
