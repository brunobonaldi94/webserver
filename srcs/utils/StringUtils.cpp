#include "StringUtils.hpp"

void AdvanceOnComment(std::string::iterator &it, std::string &fileContent)
{
   if (*it == '#')
    {
      while (*it != '\n')
      {
        if (it == fileContent.end())
          return;
        ++it;
      }
    }
}

void AdvaceOnWhiteSpace(std::string::iterator &it, std::string &fileContent)
{
    while (std::isspace(*it))
    {
        if (it == fileContent.end())
            return;
        ++it;
    }
}

std::string ExtractWord(std::string::iterator &it, std::string &fileContent, std::vector<std::string> &allowedChars)
{
   std::string word;
   while (!std::isspace(*it) && SafeFindInVector<std::string>(allowedChars, std::string(1, *it)) == NULL)
    {
      if (it == fileContent.end())
        return word;
      word += *it;
      ++it;
    }
    return word;
}
