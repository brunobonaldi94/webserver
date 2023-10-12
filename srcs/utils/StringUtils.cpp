#include "StringUtils.hpp"

void StringUtils::AdvanceOnComment(std::string::iterator &it, std::string &fileContent)
{
   if (*it == '#')
    {
      while (*it != '\n')
      {
        if (it == fileContent.end())
          return;
        ++it;
      }
      ++it;
    }
}

void StringUtils::AdvaceOnWhiteSpace(std::string::iterator &it, std::string &fileContent)
{
    while (std::isspace(*it))
    {
        if (it == fileContent.end())
            return;
        ++it;
    }
}

std::string StringUtils::ExtractWord(std::string::iterator &it, std::string &fileContent, std::vector<std::string> &allowedChars)
{
  std::string word;
  char prevChar;
  while (VectorUtils<std::string>::SafeFindVector(allowedChars, std::string(1, *it)) != NULL)
  {
    if (*it == '}')
      return word;
    if (prevChar == *it)
      throw std::runtime_error("Two consecutive special characters");
    if (it == fileContent.end())
      return word;
    prevChar = *it;
    ++it;
  }
  while (!std::isspace(*it) && VectorUtils<std::string>::SafeFindVector(allowedChars, std::string(1, *it)) == NULL)
  {
    word += *it;
    ++it;
  }
  return word;
}

std::vector<std::string> StringUtils::Split(std::string str, std::string delimiters)
{
	std::stringstream ss(str);
	std::string currentString;
	std::vector<std::string> strSplit;
  for (size_t i = 0; i < delimiters.size(); i++)
  {
	  while(std::getline(ss, currentString, delimiters[i]))
		  strSplit.push_back(currentString);
  }
	return strSplit;
}

std::string StringUtils::UpperCase(std::string str)
{
  std::string upperCaseStr;
  for (size_t i = 0; i < str.size(); i++)
  {
    upperCaseStr += std::toupper(str[i]);
  }
  return upperCaseStr;
}