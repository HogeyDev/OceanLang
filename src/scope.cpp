#include "scope.h"
#include <iostream>
#include <string>

DefinedScope::DefinedScope() {
  this->stackSize = 0;
  this->strings   = new std::vector<std::vector<std::string>>();
  this->functions = new std::vector<std::vector<std::string>>();
}

bool DefinedScope::hasVariable(std::string name) {
  // std::cout << this->stackSize << std::endl;
  // for (std::vector<std::string> var : this->variables) {
  //   std::cout << var[1] << " " << var[0] << " @ " << var[2] << std::endl;
  // }
  for (std::vector<std::string> v : this->variables) {
    if (v[0] == name)
      return true;
  }
  return false;
}

void DefinedScope::createVariable(std::string name, std::string type) {
  if (this->hasVariable(name)) {
    std::cerr << "Variable \'" << name << "\' already exists" << std::endl;
    exit(1);
  }
  this->stackSize++;
  std::vector<std::string> var = {name, type, std::to_string(this->stackSize)};
  this->variables.push_back(var);
  // std::cout << this->stackSize << std::endl;
  // for (std::vector<std::string> var : this->variables) {
  //   std::cout << var[1] << " " << var[0] << " @ " << var[2] << std::endl;
  // }
}

std::string DefinedScope::getVariableType(std::string name) {
  return this->variables[this->getVariableIndex(name)][1];
}

int DefinedScope::getVariableIndex(std::string name) {
  // std::cout << this->stackSize << std::endl;
  // for (std::vector<std::string> var : this->variables) {
  //   std::cout << var[1] << " " << var[0] << " @ " << var[2] << std::endl;
  // }
  for (std::vector<std::string> &v : this->variables) {
    if (v[0] == name)
      return this->stackSize - std::stoi(v[2]);
  }
  std::cerr << "Could not find variable with name \'" << name
            << "\' in current scope\n";
  exit(1);
}

int DefinedScope::stringId(std::string value) {
  for (unsigned int i = 0; i < this->strings->size(); i++) {
    if (this->strings->at(i).at(1) == value)
      return i;
  }
  // std::cout << "STRING TABLE:" << std::endl;
  // for (auto &s : *this->strings) {
  //   std::cout << "\t" << s.at(0) << ": " << s.at(1) << std::endl;
  // }
  return this->newString(value);
}

int DefinedScope::newString(std::string value) {
  int strLabel                       = this->strings->size();
  std::vector<std::string> newString = {"STR" + std::to_string(strLabel),
                                        value};
  this->strings->push_back(newString);
  // std::cout << "\t" << newString.at(0) << ": \'" << newString.at(1) <<
  // "\'\n";
  return strLabel;
}

DefinedScope *DefinedScope::newSubScope() {
  DefinedScope *sub = new DefinedScope();
  sub->inheritStrings(this);
  sub->functions = this->functions;
  for (std::vector<std::string> v : this->variables) {
    sub->variables.push_back(v);
  }

  sub->stackSize = this->stackSize;
  return sub;
}

void DefinedScope::inheritStrings(DefinedScope *scope) {
  // (void)scope;
  this->strings = scope->strings;
}

std::string DefinedScope::push(std::string pushed) {
  this->stackSize++;
  return "\tpush " + pushed + "\n";
}

std::string DefinedScope::pop(std::string popped) {
  this->stackSize--;
  return "\tpop " + popped + "\n";
}

std::string DefinedScope::call(std::string procedure) {
  // this->stackSize++;
  return "\tcall " + procedure + "\n";
}

void DefinedScope::createFunction(std::string name, std::string type,
                                  std::vector<std::string> argTypes) {
  if (this->hasFunction(name)) {
    std::cerr << "Function with name \'" << name << "\' already exists\n";
    exit(1);
  }

  std::vector<std::string> newFunction = {name, type};
  for (std::string at : argTypes) {
    newFunction.push_back(at);
  }

  this->functions->push_back(newFunction);
}

bool DefinedScope::hasFunction(std::string name) {
  for (std::vector<std::string> f : *this->functions) {
    if (f.at(0) == name) {
      return true;
    }
  }

  return false;
}

std::vector<std::string> DefinedScope::getFunctionData(std::string name) {
  for (std::vector<std::string> f : *this->functions) {
    if (f.at(0) == name) {
      return f;
    }
  }

  std::cerr << "Cannot find a function with name: " << name << std::endl;
  exit(1);
}
