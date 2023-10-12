#include "ServerContextCreator.hpp"

ServerContextCreator::ServerContextCreator(): AContextCreator() {}

ServerContextCreator::ServerContextCreator(ServerContextCreator const & other): AContextCreator(other) 
{
    *this = other;
}

ServerContextCreator::~ServerContextCreator() {}

ServerContextCreator& ServerContextCreator::operator=(ServerContextCreator const & other)
{
    (void)other;
    return (*this);
}

AContext *ServerContextCreator::CreateContext() const
{
    return (new ServerContext());
}