#include "util.h"

std::vector<std::string> splitString(std::string input, char delimiter) {
  std::vector<std::string> output = {};

  std::string buf                 = "";
  for (unsigned int i = 0; i < input.size(); i++) {
    char c = input.at(i);
    if (c == delimiter) {
      output.push_back(buf);
      buf = "";
      continue;
    }
    buf += c;
    if (i == (input.size() - 1)) {
      output.push_back(buf);
    }
  }

  return output;
}

std::string joinString(std::vector<std::string> input, std::string joiner) {
  std::string output = input.at(0);

  if (input.size() == 1)
    return output;

  for (unsigned int i = 1; i < input.size(); i++) {
    output += joiner + input.at(i);
  }

  return output;
}
