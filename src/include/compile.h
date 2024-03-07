#pragma once

#include "scope.h"
#include <string>

typedef struct {
  bool include_once;
} FileProperties;

FileProperties getFileProperties(std::string *code);
void interpret(std::string code);
void interpretFile(std::string filepath);
std::string compile(std::string code, DefinedScope *scope);
std::string compileFile(std::string filepath, DefinedScope *scope);
std::string compileFile(std::string filepath);
