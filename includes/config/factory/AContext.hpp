#pragma once

#include <iostream>
#include <vector>
#include "ADirective.hpp"

class ADirective;

class AContext
{
private:
    AContext*                   _parentContext;
    std::vector<std::string>    _allowedDirectives;
    std::vector<std::string>    _allowedSubContexts;
    std::vector<AContext *>     _subContexts;
    std::vector<ADirective *>   _directives;

public:
    AContext(std::vector<std::string> allowedDirectives, std::vector<std::string> allowedSubContexts, AContext *parentContext = NULL);
    AContext(AContext const & other);
    virtual ~AContext();

    AContext& operator=(AContext const & other);

    std::vector<ADirective *> GetDirectives() const;
    void AddDirective(ADirective *directive);
    std::vector<AContext *> GetSubContexts() const;
    void AddSubContext(AContext *subContext);
    AContext *GetParentContext() const;
    void SetParentContext(AContext *parentContext);
    std::vector<std::string> GetAllowedDirectives() const;
    std::vector<std::string> GetAllowedSubContexts() const;

    virtual bool ParseContext(std::string &content) = 0;
    
};