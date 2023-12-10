#include <string>
#include <vector>

typedef enum {
  TOKEN_IDENTIFIER,
  TOKEN_STRING,
  TOKEN_INTEGER,
  TOKEN_SEMI,
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_LBRACE,
  TOKEN_RBRACE,
  TOKEN_EQUALS,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_STAR,
  TOKEN_SLASH,
  TOKEN_EOF,
} TokenType;

typedef struct {
  TokenType type;
  std::string value;
} Token;

class Tokenizer {
  std::string source;
  int index;

public:
  Tokenizer(std::string code);
  Token nextToken();
  std::vector<Token> tokenize();
  char get(int index);
  char peek(int offset = 0);
  Token parseIdentifier();
  Token parseInteger();
  Token parseString();
  Token advanceWith(TokenType type);
  void skipWhitespace();
};

void printToken(Token tok);
std::string getTokenType(TokenType type);
