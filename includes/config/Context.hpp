#pragma once

#include <iostream>
#include <vector>
#include "Directive.hpp"

class Context
{
private:
    std::string _name;
    std::vector<Directive *>    _directives;
    std::vector<Context *>      _subContexts;
    Context*                    _parentContext;
    std::vector<std::string>    _allowedDirectives;
    std::vector<std::string>    _allowedSubContexts;

public:
    Context();
    Context(Context const & other);
    ~Context();

    Context& operator=(Context const & other);
};