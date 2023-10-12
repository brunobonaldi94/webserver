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
    if (line[line.size() -1] != ';')
    {
        throw ADirective::DirectiveSyntaxErrorException("Syntax error: missing ';' at the end of the line");
        return false;
    }
    line = line.substr(0, line.size() - 1);
    return true;
}

ADirective::DirectiveSyntaxErrorException::DirectiveSyntaxErrorException(std::string syntaxErrorMsg): _syntaxErrorMsg(syntaxErrorMsg)
{
}

ADirective::DirectiveSyntaxErrorException::~DirectiveSyntaxErrorException() throw()
{
}

const char* ADirective::DirectiveSyntaxErrorException::what() const throw()
{
    return this->_syntaxErrorMsg.c_str();
}