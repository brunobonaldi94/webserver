#pragma once

#include "ADirective.hpp"

class ADirectiveCreator
{
public:
    virtual ~ADirectiveCreator();

    virtual ADirective *createDirective() const = 0;

    virtual ADirective *parseDirective();
};