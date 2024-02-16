#pragma once

#include "scope.h"
#include <string>

void interpret(std::string code);
void interpretFile(std::string filepath);
std::string compile(std::string code, DefinedScope *scope);
std::string compileFile(std::string filepath, DefinedScope *scope);
std::string compileFile(std::string filepath);
