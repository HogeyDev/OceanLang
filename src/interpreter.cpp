#include "interpreter.h"
#include <iostream>

Interpreter::Interpreter(AST::Scope *root) { this->root = root; }

void Interpreter::run() { this->runScope(this->root); }

void Interpreter::runScope(AST::Scope *scope) {
  for (auto &stmt : scope->statements) {
    (void)stmt;
  }
}
