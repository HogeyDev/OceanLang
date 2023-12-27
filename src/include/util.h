#pragma once

#include <string>
#include <vector>

std::vector<std::string> splitString(std::string input, char delimiter);
std::string joinString(std::vector<std::string> input, std::string joiner);
