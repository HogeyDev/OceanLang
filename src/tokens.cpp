#include "tokens.hpp"
#include "error.hpp"
#include <cctype>
#include <iostream>
#include <vector>

Tokenizer::Tokenizer(std::string code) {
  this->source = code;
  this->index = 0;
}

Token Tokenizer::nextToken() {
  this->skipWhitespace();

  if (std::isalpha(this->peek())) {
    return this->parseIdentifier();
  }
  if (std::isdigit(this->peek())) {
    return this->parseInteger();
  }
  if (this->peek() == '\"') {
    return this->parseString();
  }

  switch (this->peek()) {
  case ';':
    return this->advanceWith(TOKEN_SEMI);
  case '(':
    return this->advanceWith(TOKEN_LPAREN);
  case ')':
    return this->advanceWith(TOKEN_RPAREN);
  case '{':
    return this->advanceWith(TOKEN_LBRACE);
  case '}':
    return this->advanceWith(TOKEN_RBRACE);
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
  case '\0':
    return this->advanceWith(TOKEN_EOF);

  default:
    std::cerr << "[TOKENIZER] Unknown character: " << this->peek() << std::endl;
    exit(1);
  }
  return {};
}

std::vector<Token> Tokenizer::tokenize() {
  std::vector<Token> tokenList = {};

  Token currentToken;
  while ((currentToken = this->nextToken()).type != TOKEN_EOF) {
    tokenList.push_back(currentToken);
  }

  return tokenList;
}

char Tokenizer::get(int index) {
  if (index < 0 || index >= (int)this->source.size())
    return '\0';
  return this->source[index];
}

char Tokenizer::peek(int offset) { return this->get(this->index + offset); }

Token Tokenizer::parseIdentifier() {
  std::string val;
  while (std::isalnum(this->peek())) {
    val += this->peek();
    this->index++;
  }
  return {.type = TOKEN_IDENTIFIER, .value = val};
}

Token Tokenizer::parseInteger() {
  std::string val;
  while (std::isdigit(this->peek())) {
    val += this->peek();
    this->index++;
  }
  return {.type = TOKEN_INTEGER, .value = val};
}

Token Tokenizer::parseString() {
  this->index++;
  std::string val;
  while (this->peek() != '\"') {
    val += this->peek();
    this->index++;
  }
  this->index++;
  return {.type = TOKEN_STRING, .value = val};
}

Token Tokenizer::advanceWith(TokenType type) {
  this->index++;
  return {.type = type};
}

void Tokenizer::skipWhitespace() {
  while (std::isspace(this->peek())) {
    this->index++;
  }
}

void printToken(Token tok) {
  std::cout << "{ type: " << getTokenType(tok.type) << "\tvalue: " << tok.value
            << " }" << std::endl;
}

std::string getTokenType(TokenType type) {
  static const std::string names[] = {
      "TOKEN_IDENTIFIER", "TOKEN_STRING", "TOKEN_INTEGER", "TOKEN_SEMI",
      "TOKEN_LPAREN",     "TOKEN_RPAREN", "TOKEN_LBRACE",  "TOKEN_RBRACE",
      "TOKEN_EQUALS",     "TOKEN_PLUS",   "TOKEN_MINUS",   "TOKEN_STAR",
      "TOKEN_SLASH",      "TOKEN_EOF",
  };
  return names[type];
}
