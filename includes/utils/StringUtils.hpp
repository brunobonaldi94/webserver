#pragma once
#include <iostream>
#include <sstream>
#include "VectorUtils.hpp"
#include "defines.hpp"

class StringUtils
{
private:
  StringUtils();
  ~StringUtils();
public:
  static void AdvanceOnComment(std::string::iterator &it, std::string &fileContent);

  static void AdvaceOnWhiteSpace(std::string::iterator &it, std::string &fileContent);

  static void AdvaceOnDelimiters(std::string::iterator &it, std::string &fileContent, std::string delimeters);

  static std::string ExtractWord(std::string::iterator &it, std::string &fileContent, std::vector<std::string> &allowedChars);

  static std::string ExtractLine(std::string::iterator &it, std::string &fileContent);

  static std::vector<std::string> Split(std::string str, std::string delimiters);

  static std::string UpperCase(std::string str);

  template <typename T>
  static std::string ConvertNumberToString(T number);
};