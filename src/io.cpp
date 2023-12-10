#include "io.hpp"
#include "error.hpp"
#include <fstream>
#include <string>

std::string readFile(const char *filepath) {
  std::ifstream file(filepath);
  if (!file.is_open()) {
    errorOut("Could not open file: " + (std::string)filepath);
  }
  std::string content((std::istreambuf_iterator<char>(file)),
                      (std::istreambuf_iterator<char>()));
  file.close();
  return content;
}

void writeFile(std::string filepath, std::string contents) {
  std::ofstream file(filepath);
  if (!file.is_open()) {
    errorOut("Could not open file: " + (std::string)filepath);
  }
  file << contents;
  file.close();
}
