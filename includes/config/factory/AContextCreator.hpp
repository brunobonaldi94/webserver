#pragma once

#include "AContext.hpp"

class AContextCreator
{
public:
    AContextCreator();
    virtual ~AContextCreator();

    virtual AContext *CreateContext() const = 0;

    virtual AContext* ParseContext(std::string &content);
};