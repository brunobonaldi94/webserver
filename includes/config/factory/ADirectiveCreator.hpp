#pragma once

#include "ADirective.hpp"

class ADirective;
class ADirectiveCreator
{
public:
    ADirectiveCreator();
    virtual ~ADirectiveCreator() = 0;
    virtual ADirective *CreateDirective() const = 0;

};