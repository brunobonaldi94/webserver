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

    virtual bool parseDirective(std::string &line) const = 0;
};