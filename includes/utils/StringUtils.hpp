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
  static std::string::iterator AdvaceOnWhiteSpace(std::string str);
  static void AdvaceOnDelimiters(std::string::iterator &it, std::string &fileContent, std::string delimeters);
  static void PrintSeparator();
  static std::string ExtractWord(std::string::iterator &it, std::string &fileContent, std::vector<std::string> &allowedChars);
  static std::string ExtractWord(std::string::iterator &it, std::string &fileContent);
  static std::string ExtractLine(std::string::iterator &it, std::string &fileContent);
  static std::vector<std::string> Split(std::string str, std::string delimiters);
  static std::string UpperCase(std::string str);
  static std::string LowerCase(std::string str);
  static std::string ForEach(std::string str, int (* fun)(int));
  static bool CheckNextCharAfterWhiteSpace(std::string::iterator &it, std::string &fileContent, char c);
  template <typename T>
  static std::string ConvertNumberToString(T number);
  static std::vector<std::string> SplitAtFirstDelimiter(std::string str, std::string delimiters);
  static std::string TrimLeft(std::string str, const char *chars = WHITE_SPACE);
  static std::string TrimRight(std::string str, const char *chars = WHITE_SPACE);
  static std::string Trim(std::string str, const char *chars = WHITE_SPACE);
  static size_t CountChar(std::string str, char c);
  static size_t FindNthOccurrence(std::string str, char c, size_t n);
  static void AddToString(std::string &str, std::string line, bool addNewLine = true);
};