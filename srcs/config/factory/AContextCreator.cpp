#include "AContextCreator.hpp"


AContextCreator::AContextCreator() {}

AContextCreator::~AContextCreator() {}

AContext* AContextCreator::ParseContext(std::string &content)
{
    AContext* context = this->CreateContext();
    if (context->ParseContext(content) == false)
    {
        delete context;
        return (NULL);
    }
    return (context);
}