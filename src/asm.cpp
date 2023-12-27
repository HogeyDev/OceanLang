#include "asm.h"
#include "util.h"
#include <iostream>

AsmOptimizer::AsmOptimizer(std::string root) { this->compiled = root; }

std::string AsmOptimizer::optimize() {
  std::string ret                = "";

  std::vector<std::string> split = splitString(this->compiled, '\n');
  for (unsigned int i = 0; i < split.size(); i++) {
    if (split.at(i) == "\tpush rax") {
      if (split.at(i + 1) == "\tpop rax") {
        i++;
        continue;
      }
    }
    ret += split.at(i) + '\n';
    std::cout << "LINE: " << split.at(i) << std::endl;
  }

  return ret;
}
