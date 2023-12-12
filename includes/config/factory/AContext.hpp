#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <typeinfo>
#include "AContextCreator.hpp"
#include "ADirectiveCreator.hpp"
#include "ADirective.hpp"
#include "types.hpp"
#include "NotAllowedException.hpp"
#include "NotFoundException.hpp"
#include "SyntaxErrorException.hpp"
#include "StringUtils.hpp"
#include "Logger.hpp"

class ADirective;
class ADirectiveCreator;
class AContextCreator;
class AContext
{
private:
    AContext*                                           _parentContext;
    MapContexts                                         _subContexts;
    MapDirectives                                       _directives;
protected:
    MapDirCreator                                       _allowedDirectives;
    MapContextCreator                                   _allowedSubContexts;
    std::string                                         _contextName;

public:
    AContext(AContext *parentContext = NULL, std::string contextName = "");
    AContext(AContext const & other);
    virtual ~AContext();

    AContext& operator=(AContext const & other);

    MapDirectives  GetDirectives() const;
    void AddDirective(std::string name, ADirective *directive);
    MapContexts GetSubContexts() const;
    std::vector<AContext *> *GetSubContextsByName(std::string name);
    void AddSubContext(std::string name, AContext *subContext);
    AContext *GetParentContext() const;
    void SetParentContext(AContext *parentContext);
    MapDirCreator GetAllowedDirectives() const;
    MapContextCreator GetAllowedSubContexts() const;

    ADirective *GetDirective(std::string name);

    void SetContextName(std::string contextName);
    std::string GetContextName() const;

    void HandleContextCreation(std::string &content, std::string &word, std::string contextName = "");
    void HandleDirectiveCreation(std::string::iterator &it, std::string &content, std::string &word, std::string directiveName = "");
    void FillDefaultValuesDirectives();
    virtual void FillDefaultValues() = 0;
    virtual void PrintContext() = 0;
    virtual void ParseContext(std::string &content) = 0;
};