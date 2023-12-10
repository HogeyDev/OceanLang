#include "compiler.hpp"
#include "io.hpp"
#include "tokens.hpp"
#include <string>
#include <vector>

std::string compile(std::string code) {
  std::vector<Token> tokenList = (new Tokenizer(code))->tokenize();
  for (Token tok : tokenList) {
    printToken(tok);
  }
  // AST_Root *root = (new Parser(tokenList))->parse();
  // std::string asmCode = (new AsmFrontend(root))->compile();
  return "";
  // return asmCode;
}
std::string compileFile(std::string fileName) {
  return compile(readFile(fileName.c_str()));
}
