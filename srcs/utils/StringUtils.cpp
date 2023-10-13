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

void StringUtils::AdvaceOnDelimiters(std::string::iterator &it, std::string &fileContent, std::string delimeters)
{
  for (size_t i = 0; i < delimeters.size(); i++)
  {
    while (*it == delimeters[i])
    {
      if (it == fileContent.end())
        return;
      ++it;
    }
  }
}


std::string StringUtils::ExtractWord(std::string::iterator &it, std::string &fileContent, std::vector<std::string> &allowedChars)
{
  std::string word = "";
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
  fileContent = std::string(it, fileContent.end());
  it = fileContent.begin();
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

std::string StringUtils::ExtractLine(std::string::iterator &it, std::string &fileContent)
{
  std::string line;
  StringUtils::AdvaceOnDelimiters(it, fileContent, SPACE);
  for (; it != fileContent.end(); ++it)
  {    
    if (*it == '\n')
    {
      ++it;
      fileContent = std::string(it, fileContent.end());
      it = fileContent.begin();
      return line;
    }
    line += *it;
  }
  return line;
}

bool StringUtils::CheckNextCharAfterWhiteSpace(std::string::iterator &it, std::string &fileContent, char c)
{
  StringUtils::AdvaceOnWhiteSpace(it, fileContent);
  if (*it == c)
  {
    ++it;
    return true;
  }
  return false;
}

template <typename T>
std::string StringUtils::ConvertNumberToString(T number)
{
  std::stringstream ss;
  ss << number;
  return ss.str();
}

template std::string StringUtils::ConvertNumberToString<int>(int number);
template std::string StringUtils::ConvertNumberToString<float>(float number);
template std::string StringUtils::ConvertNumberToString<double>(double number);
template std::string StringUtils::ConvertNumberToString<long>(long number);
template std::string StringUtils::ConvertNumberToString<long long>(long long number);