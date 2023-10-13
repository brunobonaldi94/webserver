#pragma once

#include <iostream>
#include "AContext.hpp"
#include "SyntaxErrorException.hpp"

class AContext;

class ADirective
{
private:
  AContext*    _parentContext;
public:
    
    virtual ~ADirective();

    void SetParentContext(AContext* parentContext);
    AContext* GetParentContext() const;
    virtual bool ParseDirective(std::string &line) = 0;
    virtual void PrintDirective() const = 0;
};