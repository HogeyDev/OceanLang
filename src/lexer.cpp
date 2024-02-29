#include "lexer.h"
#include "token.h"
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

Lexer::Lexer(std::string contents) {
  this->code   = contents;
  this->index  = 0;
  this->line   = 0;
  this->column = 0;
}

void Lexer::skipWhitespace() {
  while (std::isspace(this->peek(0))) {
    this->index++;
  }
}

void Lexer::skipComment() {
  while (this->peek(0) != '\n') {
    this->index++;
  }
}

Token Lexer::nextToken() {
  this->skipWhitespace();
  if (std::isalpha(this->peek(0)) || this->peek(0) == '_') {
    std::string value;
    while (std::isalnum(this->peek(0)) || this->peek(0) == '_') {
      value += this->peek(0);
      this->index++;
    }
    return {TOKEN_IDENTIFIER, value, this->line, this->column};
  }
  if (std::isdigit(this->peek(0))) {
    std::string value;
    while (std::isdigit(this->peek(0))) {
      value += this->peek(0);
      this->index++;
    }
    return {TOKEN_INTEGER, value, this->line, this->column};
  }
  if (this->peek(0) == '\"') {
    this->index++;
    std::string value;
    while (this->peek(0) != '\"') {
      std::string c = "";
      c += this->peek(0);
      if (c[0] == '\\') {
        switch (this->peek(1)) {
        case 'n':
          c = "\", 0xa, \"";
          this->index++;
          break;
        }
      }
      value += c;
      this->index++;
    }
    this->index++;
    return {TOKEN_STRING, value, this->line, this->column};
  }
  switch (this->peek(0)) {
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
    if (this->peek(1) == '=') {
      this->index++;
      return this->advanceWith(TOKEN_EQUALS_EQUALS);
    }
    return this->advanceWith(TOKEN_EQUALS);
  case '+':
    return this->advanceWith(TOKEN_PLUS);
  case '-':
    return this->advanceWith(TOKEN_MINUS);
  case '*':
    return this->advanceWith(TOKEN_STAR);
  case '/':
    if (this->peek(1) == '/') {
      this->skipComment();
      return this->nextToken();
    }
    return this->advanceWith(TOKEN_SLASH);
  case '!':
    if (this->peek(1) == '=') {
      this->index++;
      return this->advanceWith(TOKEN_BANG_EQUALS);
    }
    return this->advanceWith(TOKEN_BANG);
  case '<':
    if (this->peek(1) == '=') {
      this->index++;
      return this->advanceWith(TOKEN_LESS_THAN_EQUALS);
    }
    return this->advanceWith(TOKEN_LESS_THAN);
  case '>':
    if (this->peek(1) == '=') {
      this->index++;
      return this->advanceWith(TOKEN_MORE_THAN_EQUALS);
    }
    return this->advanceWith(TOKEN_MORE_THAN);
  case '|':
    if (this->peek(1) == '|') {
      this->index++;
      return this->advanceWith(TOKEN_PIPE_PIPE);
    }
    break;
  case '&':
    if (this->peek(1) == '&') {
      this->index++;
      return this->advanceWith(TOKEN_AND_AND);
    }
    break;
  case '%':
    return this->advanceWith(TOKEN_PERCENT);
  case '\0':
    return this->advanceWith(TOKEN_EOF);
  }
  std::cerr << "Could not find a token for: " << this->peek(0) << std::endl;
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

char Lexer::get(int index) {
  if (index < 0 || index >= (int)this->code.size())
    return '\0';
  return this->code[index];
}

char Lexer::peek(int offset) { return this->get(this->index + offset); }

void Lexer::advance() {
  this->index++;
  this->column++;
  if (this->peek(0) == '\n') {
    this->column = 0;
    this->line++;
  }
}

Token Lexer::advanceWith(TokenType type) {
  this->index++;
  return {type, "", this->line, this->column};
}
