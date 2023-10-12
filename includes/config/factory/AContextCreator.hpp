#pragma once

#include "AContext.hpp"

class AContext;

class AContextCreator
{
public:
    AContextCreator();
    virtual ~AContextCreator() = 0;

    virtual AContext *CreateContext() const = 0;

};