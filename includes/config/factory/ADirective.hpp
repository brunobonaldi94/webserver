#pragma once

#include <iostream>
#include "AContext.hpp"

class AContext;

class ADirective
{
private:
  AContext*    _parentContext;
public:
    
    virtual ~ADirective();

    void SetParentContext(AContext* parentContext);
    AContext* GetParentContext() const;
    virtual bool ParseDirective(std::string &line) = 0;
    virtual void PrintDirective() const = 0;

    class DirectiveSyntaxErrorException : public std::exception
    {
        private:
          std::string _syntaxErrorMsg;
        public:
            DirectiveSyntaxErrorException(std::string syntaxErrorMsg);
            virtual ~DirectiveSyntaxErrorException() throw();
            virtual const char* what() const throw();
    };
};