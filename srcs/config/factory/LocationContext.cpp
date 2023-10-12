#include "LocationContext.hpp"

LocationContext::LocationContext(): AContext() {};

LocationContext::LocationContext(LocationContext const &other): AContext(other) {};

LocationContext::~LocationContext() {};

bool LocationContext::ParseContext(std::string &content)
{
    std::string::iterator it = content.begin();
    for (; it != content.end(); it++)
    {
        while(std::isspace(*it))
            it++;
        if (*it == '{')
        {
            content.erase(content.begin(), it);
            return (true);
        }
        else if (*it != ' ' && *it != '\t')
            return (false);
    }
    content.erase(content.begin(), it);
    return (false);
}