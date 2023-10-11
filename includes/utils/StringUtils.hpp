#include <iostream>
#include "VectorUtils.hpp"


void AdvanceOnComment(std::string::iterator &it, std::string &fileContent);

void AdvaceOnWhiteSpace(std::string::iterator &it, std::string &fileContent);

std::string ExtractWord(std::string::iterator &it, std::string &fileContent, std::vector<std::string> &allowedChars);