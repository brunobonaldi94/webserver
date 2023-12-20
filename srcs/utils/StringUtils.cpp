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

void StringUtils::PrintSeparator()
{
	std::cout << std::string(100, '-') << std::endl;
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

std::string::iterator StringUtils::AdvaceOnWhiteSpace(std::string str)
{
  std::string::iterator it;
  for (it = str.begin(); it != str.end(); it++)
  {
    if (std::isspace(*it))
      continue;
  }
  return it;
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

std::string StringUtils::ExtractWord(std::string::iterator &it, std::string &fileContent)
{
  std::string word = "";
  while (!std::isspace(*it))
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
  return StringUtils::ForEach(str, std::toupper);
}

std::string StringUtils::LowerCase(std::string str)
{
  return StringUtils::ForEach(str, std::tolower);
}

std::string StringUtils::ForEach(std::string str, int (* fun)(int))
{
  std::string transformedStr;
  for (size_t i = 0; i < str.size(); i++)
  {
    transformedStr += fun(static_cast<int>(str[i]));
  }
  return transformedStr;
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

std::vector<std::string> StringUtils::SplitAtFirstDelimiter(std::string str, std::string delimiters)
{
  std::vector<std::string> vec;
  size_t pos = str.find_first_of(delimiters);
  if (pos == std::string::npos)
    return std::vector<std::string>();
  vec.push_back(str.substr(0, pos));
  vec.push_back(str.substr(pos + 1, str.size()));
  return  vec;
}

std::string StringUtils::TrimLeft(std::string str, const char *chars)
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

std::string StringUtils::TrimRight(std::string str, const char *chars)
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

std::string StringUtils::Trim(std::string str, const char *chars)
{
    str = StringUtils::TrimLeft(str, chars);
    str = StringUtils::TrimRight(str, chars);
    return str;
}

size_t StringUtils::CountChar(std::string str, const char c)
{
  size_t count = 0;
  for (size_t i = 0; i < str.size(); i++)
  {
    if (str[i] == c)
      count++;
  }
  return count;
}

size_t StringUtils::FindNthOccurrence(std::string str, char c, size_t n)
{
  size_t occurrences = n;
  if (n == 0)
    return std::string::npos;
  for (size_t i = 0; i < str.size(); i++)
  {
    if (str[i] == c)
      occurrences--;
    if (occurrences == 0)
      return i;
  }
  return std::string::npos;
}

 void StringUtils::AddToString(std::string &str, std::string str2)
{
  str.append(str2);
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
template std::string StringUtils::ConvertNumberToString<unsigned int>(unsigned int number);
template std::string StringUtils::ConvertNumberToString<unsigned long>(unsigned long number);
template std::string StringUtils::ConvertNumberToString<unsigned long long>(unsigned long long number);
template std::string StringUtils::ConvertNumberToString<short>(short number);