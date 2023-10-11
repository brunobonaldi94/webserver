#pragma once

#include <iostream>
#include "AContext.hpp"

class AContext;

class ADirective
{
protected:
  AContext*    _parentContext;
  
public:
 
    virtual ~ADirective();

    virtual bool ParseDirective(std::string &line) const = 0;
};