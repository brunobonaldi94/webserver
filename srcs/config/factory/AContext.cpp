#include "AContext.hpp"

AContext::AContext(AContext *parentContext, std::string contextName) : _parentContext(parentContext), _contextName(contextName)
{
}

AContext::AContext(AContext const & other): _parentContext(other._parentContext) 
{
    *this = other;
}

AContext::~AContext()
{
    MapUtils<std::string, ADirective *>::ClearMap(this->_directives);
    for (MapContexts::iterator it = this->_subContexts.begin(); it != this->_subContexts.end(); ++it)
        VectorUtils<AContext *>::clearVector(it->second);
}

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
    this->_directives[name] = directive;
}

void AContext::AddSubContext(std::string name, AContext *subContext)
{
    this->_subContexts[name].push_back(subContext);
}

void AContext::SetParentContext(AContext *parentContext)
{
    this->_parentContext = parentContext;
}

MapDirectives AContext::GetDirectives() const
{
    return this->_directives;
}
MapContexts AContext::GetSubContexts() const
{
    return this->_subContexts;
}

void AContext::SetContextName(std::string contextName)
{
    this->_contextName = contextName;
}

std::string AContext::GetContextName() const
{
    return (this->_contextName);
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
    }
}

void AContext::PrintContext()
{
    std::cout << "Context: " << this->_contextName << std::endl;
    if (this->GetParentContext())
        std::cout << "Parent context: " << this->GetParentContext()->_contextName << std::endl;
    else
        std::cout << "Parent context: NULL" << std::endl;
    StringUtils::PrintSeparator();
    std::cout << "Directives: " << std::endl;
    for (MapDirectives::iterator it = this->_directives.begin(); it != this->_directives.end(); ++it)
    {
        Logger::Debug("Directive name = ", INFO, it->first);
        if (it->second->GetParentContext())
            Logger::Debug("Parent context = ", INFO, it->second->GetParentContext()->_contextName);
        else
            Logger::Debug("Parent context = ", INFO, "NULL");
        it->second->PrintDirective();

    }
    std::cout << "SubContexts: " << std::endl;
    for (MapContexts::iterator it = this->_subContexts.begin(); it != this->_subContexts.end(); ++it)
    {
        Logger::Debug("SubContext name = ", INFO, it->first);
        for (std::vector<AContext *>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
            (*it2)->PrintContext();
    }
    StringUtils::PrintSeparator();
}

void AContext::FillDefaultValuesDirectives()
{
    MapDirectives directives = this->GetDirectives();
    for (MapDirCreator::iterator it = this->_allowedDirectives.begin(); it != this->_allowedDirectives.end(); ++it)
    {
        std::string directiveName = it->first;
        if (MapUtils<std::string, ADirective*>::SafeFindMap(directives, directiveName) == NULL && it->second != NULL)
        {
            ADirective *directive = it->second->CreateDirective();
            directive->SetParentContext(this);
            this->AddDirective(directiveName, directive);
            directive->FillDefaultValues();
        }
    }
}
ADirective *AContext::GetDirective(std::string name)
{
    PairDirCreator *directivePairCreator = 
        MapUtils<std::string, ADirectiveCreator *>::SafeFindMap(this->_allowedDirectives, name);
    if (directivePairCreator == NULL || directivePairCreator->second == NULL)
        throw NotAllowedException(name + " directive not allowed in " + this->_contextName + " context");
    PairDirectives *directivePair = 
        MapUtils<std::string, ADirective *>::SafeFindMap(this->_directives, name);
    if (directivePair == NULL)
        return (NULL);
    return (directivePair->second);
}

std::vector<AContext *> *AContext::GetSubContextsByName(std::string name)
{
    
    std::pair<const std::string, std::vector<AContext *> > *subContext = MapUtils<std::string, std::vector<AContext *> >::SafeFindMap(this->_subContexts, name);
    if (subContext == NULL)
        return NULL;
    return &subContext->second;
}