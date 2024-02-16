#include "asm.h"
#include "util.h"
#include <iostream>
#include <string>

AsmOptimizer::AsmOptimizer(std::string root) { this->compiled = root; }

std::string AsmOptimizer::optimize() {
  std::string ret                = "";

  std::vector<std::string> split = splitString(this->compiled, '\n');
  for (unsigned int i = 0; i < split.size(); i++) {
    if (split.at(i) == "\tpush rax") {
      if (split.at(i + 1) == "\tpop rax") {
        i++;
        continue;
      }
    }
    if (splitString(split.at(i), ' ').at(0) == "push") {
      std::string pushed = splitString(split.at(i), ' ').at(1);
      if (splitString(split.at(i + 1), ' ').at(0) == "pop") {
        std::string dest = splitString(split.at(i + 1), ' ').at(1);
        i++;
        ret += "mov " + dest + ", " + pushed + '\n';
        continue;
      }
    }
    ret += split.at(i) + '\n';
    // std::cout << "LINE: " << split.at(i) << std::endl;
  }

  return ret;
}

AsmSectionGrouper::AsmSectionGrouper(std::string assembly) {
  this->assembly = assembly;
}

std::string AsmSectionGrouper::group() {
  // std::cout << this->assembly << std::endl;
  std::string ret                        = "";
  std::vector<std::string> seenSections  = {};
  std::vector<std::string> sectionValues = {};

  std::string buf                        = "";
  std::vector<std::string> lines         = splitString(this->assembly, '\n');
  std::string currentSection             = "";
  int currentSectionIndex                = -1;
  for (unsigned int i = 0; i < lines.size(); i++) {
    std::string line               = lines.at(i);
    std::vector<std::string> words = splitString(line, ' ');
    if (words.size() == 0)
      continue;
    if (words.at(0) == "section") {
      if (buf.size() > 0) {
        sectionValues.at(currentSectionIndex) += buf;
        buf = "";
      }

      currentSection      = words.at(1);
      currentSectionIndex = -1;
      // std::cout << "SECTION SWITCHED TO: " << currentSection << std::endl;
      for (unsigned int i = 0; i < seenSections.size(); i++) {
        if (seenSections.at(i) == currentSection) {
          currentSectionIndex = i;
          break;
        }
      }
      if (currentSectionIndex == -1) {
        seenSections.push_back(currentSection);
        sectionValues.push_back("");
        currentSectionIndex = seenSections.size() - 1;
      }
    } else {
      buf += line + '\n';
    }
  }
  sectionValues.at(currentSectionIndex) += buf;

  // std::cout << "NEW STUFFS\n";
  for (unsigned int i = 0; i < seenSections.size(); i++) {
    // std::cout << "S-----=====-----\n"
    //           << "section " + seenSections.at(i) + '\n' + sectionValues.at(i)
    //           << "E=====-----=====\n"
    //           << std::endl;
    ret += "section " + seenSections.at(i) + '\n' + sectionValues.at(i);
  }

  return ret;
}
