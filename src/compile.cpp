#include "compile.h"
#include "asm.h"
#include "ast.h"
#include "interpreter.h"
#include "io.h"
#include "lexer.h"
#include "parser.h"
#include <vector>

void interpret(std::string code) {
  std::vector<Token> tokens = Lexer(code).tokenize();
  AST::Scope *root          = Parser(tokens).parse();
  Interpreter interpreter   = Interpreter(root);
  interpreter.run();

  // for (Token t : tokens) {
  //   std::cout << "{ type: " << t.type << ", value: " << t.value << "}\n";
  // }
}

std::string compile(std::string code, DefinedScope *scope) {
  std::vector<Token> tokens  = Lexer(code).tokenize();
  AST::Scope *root           = Parser(tokens).parse();
  std::string compiled       = compileToAsm(root, scope);
  // compiled                   = optimizer->optimize();
  AsmSectionGrouper *grouper = new AsmSectionGrouper(compiled);
  std::string grouped        = grouper->group();
  // AsmOptimizer *optimizer    = new AsmOptimizer(grouped);
  // compiled                   = optimizer->optimize();

  // return compiled;
  return grouped;
}

void interpretFile(std::string filepath) { interpret(readFile(filepath)); }
std::string compileFile(std::string filepath, DefinedScope *scope) {
  return compile(readFile(filepath), scope);
}

std::string compileFile(std::string filepath) {
  return compileFile(filepath, new DefinedScope());
}
