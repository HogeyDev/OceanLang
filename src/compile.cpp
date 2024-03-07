#include "compile.h"
#include "asm.h"
#include "ast.h"
#include "interpreter.h"
#include "io.h"
#include "lexer.h"
#include "parser.h"
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> compiledSources = {};

FileProperties getFileProperties(std::string &code) {
  FileProperties prop = {0};
  std::stringstream ss;
  ss << code;
  std::string line;
  std::string outCode = "";
  while (std::getline(ss, line, '\n')) {
    if (line == "#trait include_once") {
      prop.include_once = true;
    } else {
      outCode.append(line + '\n');
    }
  }
  code = outCode;
  return prop;
}

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
  std::string contents = readFile(filepath);
  FileProperties prop  = getFileProperties(contents);
  if (prop.include_once) {
    if (std::find(compiledSources.begin(), compiledSources.end(), filepath) !=
        compiledSources.end()) {
      // already compiled
      return "";
    }
  }
  compiledSources.push_back(filepath);
  return compile(contents, scope);
}

std::string compileFile(std::string filepath) {
  return compileFile(filepath, new DefinedScope());
}
