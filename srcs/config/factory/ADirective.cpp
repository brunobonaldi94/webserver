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

void ADirective::ParseDirective(std::string &line)
{
    if (line.empty())
        throw SyntaxErrorException("Syntax error: empty line");
    if (line[line.size() -1] != ';')
        throw SyntaxErrorException("Syntax error: missing ';' at the end of the line -" + line);
    line = line.substr(0, line.size() - 1);
}

bool ADirective::SetDefaultFromParent()
{
    return true;
}