#pragma once

#include <iostream>
#include <vector>
#include "ADirective.hpp"

class ADirective;

class AContext
{
private:
    std::vector<ADirective *>    _directives;
    std::vector<AContext *>      _subContexts;
    AContext*                    _parentContext;

public:
    AContext();
    AContext(AContext const & other);
    virtual ~AContext();

    AContext& operator=(AContext const & other);

    virtual bool parseContext(std::string &content) const = 0;
};