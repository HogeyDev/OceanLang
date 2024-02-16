#include "asm.h"
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
  DefinedScope *globalScope = new DefinedScope();
  std::string compiled      = compileFile(inFile, globalScope);
  compiled += "section .text\nglobal _start\n_start:\n\tcall _main\n\tpush "
              "rax\n\tmov rax, "
              "60\n\tpop rdi\n\tsyscall\n";
  compiled += "section .data\n";
  for (unsigned int i = 0; i < globalScope->strings->size(); i++) {
    compiled += globalScope->strings->at(i).at(0) + " db \"" +
                globalScope->strings->at(i).at(1) + "\", 0\n";
  }
  compiled += "section .text\n";
  compiled = (new AsmSectionGrouper(compiled))->group();
  writeFile(outFile, compiled);
  // interpretFile(inFile);
  return 0;
}
