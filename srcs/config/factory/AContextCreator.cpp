#include "AContextCreator.hpp"

AContextCreator::~AContextCreator() {}

AContext* AContextCreator::parseContext(std::string &content)
{
    AContext* context = this->createContext();
    if (context->parseContext(content) == false)
    {
        delete context;
        return (NULL);
    }
    return (context);
}