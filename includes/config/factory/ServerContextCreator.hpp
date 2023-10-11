#pragma once

#include "AContextCreator.hpp"
#include "ServerContext.hpp"

class ServerContextCreator : public AContextCreator
{
public:
    ServerContextCreator();
    ServerContextCreator(ServerContextCreator const & other);
    ~ServerContextCreator();

    ServerContextCreator& operator=(ServerContextCreator const & other);

    AContext *CreateContext() const;
};
