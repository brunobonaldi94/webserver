#pragma once

#include "AContext.hpp"
#include "StringUtils.hpp"
#include "VectorUtils.hpp"
#include "MapUtils.hpp"
#include "DirectivesCreator.hpp"
#include "LocationContextCreator.hpp"

class ServerContext : public AContext
{
public:
    ServerContext();
    ServerContext(ServerContext const & other);
    ~ServerContext();

    ServerContext& operator=(ServerContext const & other);
    
    ListenDirective *GetListenDirective();
    
    void ParseContext(std::string &content);
    void PrintContext();
    void FillDefaultValues();
};