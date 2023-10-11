#pragma once

#include "ADirective.hpp"

class ADirectiveCreator
{
public:
    virtual ~ADirectiveCreator();

    //virtual ADirective *CreateDirective() const = 0;

    virtual ADirective *ParseDirective();
};