#pragma once

#include <string>

class AsmOptimizer {
public:
  std::string compiled;

  AsmOptimizer(std::string root);
  std::string optimize();
};

class AsmSectionGrouper {
public:
  std::string assembly;

  AsmSectionGrouper(std::string assembly);
  std::string group();
};
