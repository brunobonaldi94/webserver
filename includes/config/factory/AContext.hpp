#pragma once

#include <iostream>
#include <vector>
#include <map>
#include "AContextCreator.hpp"
#include "ADirectiveCreator.hpp"
#include "ADirective.hpp"

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
    std::map<std::string, ADirectiveCreator *>      _allowedDirectives;
    std::map<std::string, AContextCreator *>        _allowedSubContexts;
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
    std::map<std::string, ADirectiveCreator *> GetAllowedDirectives() const;
    std::map<std::string, AContextCreator *> GetAllowedSubContexts() const;

    virtual bool ParseContext(std::string &content) = 0;
    
};