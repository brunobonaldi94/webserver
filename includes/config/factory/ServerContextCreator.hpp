#pragma once

#include "AContextCreator.hpp"

class ServerContextCreator : public AContextCreator
{
public:
    ServerContextCreator();
    ServerContextCreator(ServerContextCreator const & other);
    ~ServerContextCreator();

    ServerContextCreator& operator=(ServerContextCreator const & other);

    AContext *createContext(std::string const &name) const;
};