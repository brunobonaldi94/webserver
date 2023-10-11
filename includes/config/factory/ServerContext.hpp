#pragma once

#include "AContext.hpp"

class ServerContext : public AContext
{
public:
    ServerContext();
    ServerContext(ServerContext const & other);
    ~ServerContext();

    ServerContext& operator=(ServerContext const & other);
    bool parseContext();
};