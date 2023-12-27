#pragma once

#include <string>

static std::string mainFilePrefixDirectory = "";
static int globalLabelCount                = 0;

void setMainFilePrefixDirectory(std::string value);
std::string getMainFilePrefixDirectory();
int getGlobalLabelCount();
int newGlobalLabel();
