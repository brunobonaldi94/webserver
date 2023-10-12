#include "ADirective.hpp"

ADirective::~ADirective()
{
}

void ADirective::SetParentContext(AContext* parentContext)
{
    _parentContext = parentContext;
}

AContext* ADirective::GetParentContext() const
{
    return _parentContext;
}

bool ADirective::ParseDirective(std::string &line)
{
    if (line.empty())
        return false;
    size_t posNewLineSemiCollon = line.find_first_of(";\n");
    size_t posSemiCollon = line.find_first_of(";");
    if (posNewLineSemiCollon == std::string::npos && posSemiCollon == std::string::npos)
        return false;
    return true;
}