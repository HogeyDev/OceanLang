#include "global.h"

void setMainFilePrefixDirectory(std::string value) {
  mainFilePrefixDirectory = value;
}

std::string getMainFilePrefixDirectory() { return mainFilePrefixDirectory; }
