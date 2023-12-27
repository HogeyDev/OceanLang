#include "compile.h"
#include "global.h"
#include "io.h"
#include "util.h"
#include <filesystem>
#include <iostream>
#include <string>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Usage: ./ocean path/to/file.ocn" << std::endl;
    exit(1);
  }
  std::string pwd     = std::filesystem::current_path().string();
  std::string inFile  = pwd + '/' + argv[1];
  std::string outFile = "";
  {
    std::vector<std::string> splitSlash = splitString(inFile, '/');
    splitSlash.pop_back();
    setMainFilePrefixDirectory(joinString(splitSlash, "/") + '/');

    std::vector<std::string> splitDot = splitString(inFile, '.');
    splitDot.pop_back();
    outFile = joinString(splitDot, ".") + ".asm";
  }
  std::string compiled = compileFile(inFile);
  compiled += "global _start\n_start:\n\tcall main\n\tmov rax, 60\n\tmov rdi, "
              "0\n\tsyscall";
  writeFile(outFile, compiled);
  // interpretFile(inFile);
  return 0;
}
