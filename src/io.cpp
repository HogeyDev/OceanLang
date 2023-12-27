#include "io.h"
#include <fstream>
#include <iostream>
#include <string>

std::string readFile(std::string filepath) {
  std::ifstream file(filepath);
  if (!file.is_open()) {
    std::cerr << "Could not open file: " << filepath << std::endl;
    exit(1);
  }
  std::string contents((std::istreambuf_iterator<char>(file)),
                       (std::istreambuf_iterator<char>()));
  file.close();
  return contents;
}

void writeFile(std::string filepath, std::string contents) {
  std::ofstream file(filepath);
  file << contents;
  file.close();
}
