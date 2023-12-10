#include "compiler.hpp"
#include "error.hpp"
#include "io.hpp"
#include <string>

int main(int argc, char **argv) {
  if (argc < 2) {
    errorOut("Usage: ./compiler path/to/file.ocn");
  }

  std::string inFile = "example/main.ocn";
  std::string outFile = "example/build/main";
  std::string asmCode = compileFile(inFile);
  writeFile(outFile, asmCode);

  // system("nasm -f elf64 example/build/main.asm -o example/build/main.o -g");
  // system("ld -m elf_x86_64 example/build/main.o -o example/build/main -e
  // main"); system("rm example/build/main.o");

  return 0;
}
