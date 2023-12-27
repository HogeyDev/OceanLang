#pragma once

#include <string>

void interpret(std::string code);
void interpretFile(std::string filepath);
std::string compile(std::string code);
std::string compileFile(std::string filepath);
