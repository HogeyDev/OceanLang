#pragma once

#include <string>
#include <vector>

class DefinedScope {
public:
  std::vector<std::vector<std::string>> variables; // NAME, TYPE, OFFSET
  std::vector<std::vector<std::string>> strings;   // NAME, VALUE
  int stackSize;

  DefinedScope();
  bool hasVariable(std::string name);
  void createVariable(std::string name, std::string type);
  std::string getVariableType(std::string name);
  int getVariableIndex(std::string name);
  void inheritStrings(DefinedScope *scope);
};
