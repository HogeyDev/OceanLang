#include "token.h"
#include <string>

std::string printableTokenType(TokenType type) {
  static const std::string values[] = {
      "TOKEN_EOF",
      "TOKEN_LPAREN",
      "TOKEN_RPAREN",
      "TOKEN_IDENTIFIER",
      "TOKEN_LBRACE",
      "TOKEN_RBRACE",
      "TOKEN_SEMI",
      "TOKEN_STRING",
      "TOKEN_INTEGER",
      "TOKEN_COMMA",
      "TOKEN_EQUALS",
      "TOKEN_PLUS",
      "TOKEN_MINUS",
      "TOKEN_STAR",
      "TOKEN_SLASH",
      "TOKEN_BANG",
      "TOKEN_PIPE",
      "TOKEN_AMPERSAND",
      "TOKEN_EQUALS_EQUALS",
      "TOKEN_BANG_EQUALS",
      "TOKEN_LESS_THAN",
      "TOKEN_MORE_THAN",
      "TOKEN_LESS_THAN_EQUALS",
      "TOKEN_MORE_THAN_EQUALS",
      "TOKEN_PIPE_PIPE",
      "TOKEN_AND_AND",
      "TOKEN_PERCENT",
  };
  return values[type];
}

std::string printableToken(Token token) {
  std::string printable = "";
  printable += "{ line: ";
  printable += token.line;
  printable += ", column: ";
  printable += token.column;
  // printable += "{ type: ";
  printable += ", type: ";
  printable += printableTokenType(token.type);
  printable += ", value: " + token.value + " }";
  return printable;
}
