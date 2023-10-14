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

class ADirective;
class ADirectiveCreator;
class AContextCreator;
class AContext
{
private:
    AContext*                                       _parentContext;
    std::vector<AContext *>                         _subContexts;
    std::vector<ADirective *>                       _directives;
protected:
    MapDirCreator                                   _allowedDirectives;
    MapContextCreator                               _allowedSubContexts;
    std::vector<std::string>                        _allowedChars;

public:
    AContext(AContext *parentContext = NULL);
    AContext(AContext const & other);
    virtual ~AContext();

    AContext& operator=(AContext const & other);

    std::vector<ADirective *> GetDirectives() const;
    void AddDirective(ADirective *directive);
    std::vector<AContext *> GetSubContexts() const;
    void AddSubContext(AContext *subContext);
    AContext *GetParentContext() const;
    void SetParentContext(AContext *parentContext);
    MapDirCreator GetAllowedDirectives() const;
    MapContextCreator GetAllowedSubContexts() const;

    void HandleContextCreation(std::string &content, std::string &word, std::string contextName = "");
    void HandleDirectiveCreation(std::string::iterator &it, std::string &content, std::string &word, std::string directiveName = "");

    virtual void ParseContext(std::string &content) = 0;
    
};