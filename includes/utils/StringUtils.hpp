#pragma once
#include <iostream>
#include <sstream>
#include "VectorUtils.hpp"

class StringUtils
{
private:
  StringUtils();
  ~StringUtils();
public:
  static void AdvanceOnComment(std::string::iterator &it, std::string &fileContent);

  static void AdvaceOnWhiteSpace(std::string::iterator &it, std::string &fileContent);

  static std::string ExtractWord(std::string::iterator &it, std::string &fileContent, std::vector<std::string> &allowedChars);

  static std::vector<std::string> Split(std::string str, std::string delimiters);

  static std::string UpperCase(std::string str);
};