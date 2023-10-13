#pragma once

#include <iostream>
#include <exception>

class SyntaxErrorException : public std::exception
{
    private:
      std::string _syntaxErrorMsg;
    public:
        SyntaxErrorException(std::string syntaxErrorMsg);
        virtual ~SyntaxErrorException() throw();
        virtual const char* what() const throw();
};