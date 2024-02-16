#include "global.h"

void setMainFilePrefixDirectory(std::string value) {
  mainFilePrefixDirectory = value;
}

std::string getMainFilePrefixDirectory() { return mainFilePrefixDirectory; }

int getGlobalLabelCount() { return globalLabelCount; }
int newGlobalLabel() {
  int lblC = globalLabelCount;
  globalLabelCount++;
  return lblC;
}
