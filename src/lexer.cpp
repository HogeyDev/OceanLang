#include "lexer.h"
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

Lexer::Lexer(std::string contents) {
  this->code  = contents;
  this->index = 0;
}

void Lexer::skipWhitespace() {
  while (std::isspace(this->get())) {
    this->index++;
  }
}

Token Lexer::nextToken() {
  this->skipWhitespace();
  if (std::isalpha(this->get())) {
    std::string value;
    while (std::isalnum(this->get())) {
      value += this->get();
      this->index++;
    }
    return {TOKEN_IDENTIFIER, value};
  }
  if (std::isdigit(this->get())) {
    std::string value;
    while (std::isdigit(this->get())) {
      value += this->get();
      this->index++;
    }
    return {TOKEN_INTEGER, value};
  }
  if (this->get() == '\"') {
    this->index++;
    std::string value;
    while (this->get() != '\"') {
      value += this->get();
      this->index++;
    }
    this->index++;
    return {TOKEN_STRING, value};
  }
  switch (this->get()) {
  case '(':
    return this->advanceWith(TOKEN_LPAREN);
  case ')':
    return this->advanceWith(TOKEN_RPAREN);
  case '{':
    return this->advanceWith(TOKEN_LBRACE);
  case '}':
    return this->advanceWith(TOKEN_RBRACE);
  case ';':
    return this->advanceWith(TOKEN_SEMI);
  case ',':
    return this->advanceWith(TOKEN_COMMA);
  case '=':
    return this->advanceWith(TOKEN_EQUALS);
  case '+':
    return this->advanceWith(TOKEN_PLUS);
  case '-':
    return this->advanceWith(TOKEN_MINUS);
  case '*':
    return this->advanceWith(TOKEN_STAR);
  case '/':
    return this->advanceWith(TOKEN_SLASH);
  case '!':
    return this->advanceWith(TOKEN_BANG);
  case '\0':
    return this->advanceWith(TOKEN_EOF);
  }
  std::cerr << "Could not find a token for: " << this->get() << std::endl;
  exit(1);
}

std::vector<Token> Lexer::tokenize() {
  std::vector<Token> tokens;

  Token t;
  do {
    t = this->nextToken();
    tokens.push_back(t);
  } while (t.type != TOKEN_EOF);

  return tokens;
}

char Lexer::get() { return this->code[this->index]; }

Token Lexer::advanceWith(TokenType type) {
  this->index++;
  return {type, ""};
}
