#pragma once

#include "AContext.hpp"

class AContextCreator
{
protected:
    std::vector<std::string>    _allowedDirectives;
    std::vector<std::string>    _allowedSubContexts;
public:
    virtual ~AContextCreator();

    virtual AContext *createContext() const = 0;

    virtual AContext* parseContext(std::string &content);
};