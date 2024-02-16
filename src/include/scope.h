#pragma once

#include <string>
#include <vector>

class DefinedScope {
public:
  std::vector<std::vector<std::string>> variables; // NAME, TYPE, OFFSET
  std::vector<std::vector<std::string>> *strings;  // NAME, VALUE
  std::vector<std::vector<std::string>>
      *functions; // NAME, TYPE, ARG0_TYPE, ARG1_TYPE ... ARGN_TYPE
  int stackSize;

  DefinedScope();
  bool hasVariable(std::string name);
  void createVariable(std::string name, std::string type);
  std::string getVariableType(std::string name);
  int getVariableIndex(std::string name);
  int stringId(std::string value);
  int newString(std::string value);
  DefinedScope *newSubScope();
  void inheritStrings(DefinedScope *scope);
  std::string push(std::string pushed);
  std::string pop(std::string popped);
  std::string call(std::string procedure);
  void createFunction(std::string name, std::string type,
                      std::vector<std::string> argTypes);
  bool hasFunction(std::string name);
  std::vector<std::string> getFunctionData(std::string name);
};
