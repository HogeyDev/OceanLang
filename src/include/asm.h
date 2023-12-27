#pragma once

#include <string>

class AsmOptimizer {
public:
  std::string compiled;

  AsmOptimizer(std::string root);
  std::string optimize();
};
