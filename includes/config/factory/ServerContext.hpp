#pragma once

#include "AContext.hpp"

class ServerContext : public AContext
{
public:
    ServerContext(std::vector<std::string> allowedDirectives, std::vector<std::string> allowedSubContexts);
    ServerContext(ServerContext const & other);
    ~ServerContext();

    ServerContext& operator=(ServerContext const & other);
    bool ParseContext(std::string &content);
};