#include "ServerContext.hpp"

ServerContext::ServerContext(std::vector<std::string> allowedDirectives, std::vector<std::string> allowedSubContexts) : AContext(allowedDirectives, allowedSubContexts, NULL)
{
}

ServerContext::ServerContext(ServerContext const &other) : AContext(other)
{
}

ServerContext::~ServerContext()
{
}

ServerContext &ServerContext::operator=(ServerContext const &other)
{
    AContext::operator=(other);
    return (*this);
}

bool ServerContext::ParseContext(std::string &content)
{
    std::string word;
    std::string::iterator it = content.begin();
    size_t pos = content.find_first_of("{");
    if (pos == std::string::npos)
        return (false);
    for (; it != content.end(); it++)
    {
        while(std::isspace(*it))
            it++;
        while (!std::isspace(*it) && *it != '{' && *it != '}' && *it != ';')
        {
            word += *it;
            it++;
        }
        std::cout << word << std::endl;
        word.clear();
    }
    content.erase(content.begin(), it);
    return (false);
}