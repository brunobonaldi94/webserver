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

void AContext::AddDirective(std::string name, ADirective *directive)
{
    this->_directives[name].push_back(directive);
}

void AContext::AddSubContext(std::string name, AContext *subContext)
{
    this->_subContexts[name].push_back(subContext);
}

void AContext::SetParentContext(AContext *parentContext)
{
    this->_parentContext = parentContext;
}

std::map<std::string,std::vector<ADirective *> >  AContext::GetDirectives() const
{
    return this->_directives;
}

std::map<std::string,std::vector<AContext *> > AContext::GetSubContexts() const
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

void AContext::HandleContextCreation(std::string &content, std::string &word, std::string contextName)
{
    std::string name = contextName.size() > 0 ? contextName : typeid(this).name();
    PairContextCreator *contextCreator = MapUtils<std::string, AContextCreator *>::SafeFindMap(this->_allowedSubContexts, word);
    if (contextCreator && contextCreator->second == NULL)
        throw NotAllowedException(word + " context not allowed in " + name + " context");
    if (contextCreator != NULL)
    {
        AContext *subContext = contextCreator->second->CreateContext();
        subContext->SetParentContext(this);
        this->AddSubContext(contextCreator->first, subContext);
        subContext->ParseContext(content);
    }
}

void AContext::HandleDirectiveCreation(std::string::iterator &it, std::string &content, std::string &word, std::string directiveName)
{
   std::string name = directiveName.size() > 0 ? directiveName : typeid(this).name();
   PairDirCreator *directiveCreator = MapUtils<std::string, ADirectiveCreator *>::SafeFindMap(this->_allowedDirectives, word);
    if (directiveCreator && directiveCreator->second == NULL)
        throw NotAllowedException(word + " directive not allowed in " + name + " context");
    if (directiveCreator != NULL)
    {
        std::string line = StringUtils::ExtractLine(it, content);
        ADirective *directive = directiveCreator->second->CreateDirective();
        directive->SetParentContext(this);
        this->AddDirective(directiveCreator->first, directive);
        directive->ParseDirective(line);
        directive->PrintDirective();
    }
}