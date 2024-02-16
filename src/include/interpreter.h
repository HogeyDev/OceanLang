#pragma once

#include "ast.h"
class Interpreter {
  AST::Scope *root;

public:
  Interpreter(AST::Scope *root);
  void run();
  void runScope(AST::Scope *scope);
};
