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

MapDirCreator AContext::GetAllowedDirectives() const
{
    return (this->_allowedDirectives);
}

MapContextCreator AContext::GetAllowedSubContexts() const
{
    return (this->_allowedSubContexts);
}

void AContext::HandleContextCreation(std::string &content, std::string &word)
{
    PairContextCreator *contextCreator = MapUtils<std::string, AContextCreator *>::SafeFindMap(this->_allowedSubContexts, word);
    if (contextCreator && contextCreator->second == NULL)
        throw NotAllowedException("Error: Context " + word + " not allowed in " + typeid(this).name() + " context");
    if (contextCreator != NULL)
    {
        AContext *subContext = contextCreator->second->CreateContext();
        subContext->SetParentContext(this);
        this->AddSubContext(subContext);
        if (!subContext->ParseContext(content))
            throw NotAllowedException("Error: Context " + word + " failed to parse");
    }
}

void AContext::HandleDirectiveCreation(std::string::iterator &it, std::string &content, std::string &word)
{
   PairDirCreator *directiveCreator = MapUtils<std::string, ADirectiveCreator *>::SafeFindMap(this->_allowedDirectives, word);
    if (directiveCreator && directiveCreator->second == NULL)
        throw NotAllowedException("Error: Directive " + word + " not allowed in " + typeid(this).name() + " context");
    if (directiveCreator != NULL)
    {
        std::string line = StringUtils::ExtractLine(it, content);
        ADirective *directive = directiveCreator->second->CreateDirective();
        directive->SetParentContext(this);
        this->AddDirective(directive);
        if (!directive->ParseDirective(line))
            throw SyntaxErrorException("Error: Directive " + word + " failed to parse");
        directive->PrintDirective();
    }
}