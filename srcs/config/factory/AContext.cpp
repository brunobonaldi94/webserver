#include "AContext.hpp"

AContext::AContext(AContext *parentContext) : _parentContext(parentContext)
{
    
}

AContext::AContext(AContext const & other): _parentContext(other._parentContext) 
{
    *this = other;
}

AContext::~AContext() {}

AContext& AContext::operator=(AContext const & other)
{
    if (this != &other)
    {
        this->_parentContext = other._parentContext;
        this->_directives = other._directives;
        this->_subContexts = other._subContexts;
        this->_allowedDirectives = other._allowedDirectives;
        this->_allowedSubContexts = other._allowedSubContexts;
    }
    return (*this);
}

void AContext::AddDirective(ADirective *directive)
{
    this->_directives.push_back(directive);
}

void AContext::AddSubContext(AContext *subContext)
{
    this->_subContexts.push_back(subContext);
}

void AContext::SetParentContext(AContext *parentContext)
{
    this->_parentContext = parentContext;
}

std::vector<ADirective *> AContext::GetDirectives() const
{
    return this->_directives;
}

std::vector<AContext *> AContext::GetSubContexts() const
{
    return this->_subContexts;
}

AContext *AContext::GetParentContext() const
{
    return (this->_parentContext);
}

std::map<std::string, ADirectiveCreator *> AContext::GetAllowedDirectives() const
{
    return (this->_allowedDirectives);
}

std::map<std::string, AContextCreator *> AContext::GetAllowedSubContexts() const
{
    return (this->_allowedSubContexts);
}