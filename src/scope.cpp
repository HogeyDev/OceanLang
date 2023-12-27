#include "scope.h"
#include <iostream>
#include <string>

DefinedScope::DefinedScope() { this->stackSize = 0; }

bool DefinedScope::hasVariable(std::string name) {
  for (auto var : this->variables) {
    if (var[0] == name)
      return true;
  }
  return false;
}

void DefinedScope::createVariable(std::string name, std::string type) {
  if (this->hasVariable(name)) {
    std::cerr << "Variable \'" << name << "\' already exists" << std::endl;
    exit(1);
  }
  std::vector<std::string> var = {name, type, std::to_string(this->stackSize)};
  this->stackSize++;
  this->variables.push_back(var);
}

std::string DefinedScope::getVariableType(std::string name) {
  return this->variables[this->getVariableIndex(name)][1];
}

int DefinedScope::getVariableIndex(std::string name) {
  for (auto &v : this->variables) {
    if (v[0] == name)
      return std::stoi(v[2]);
  }
  return -1; // or maybe error out???
  // TODO
}

void DefinedScope::inheritStrings(DefinedScope *scope) {
  for (std::vector<std::string> string : scope->strings) {
    this->strings.push_back(string);
  }
}
