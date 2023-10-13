#include "SyntaxErrorException.hpp"

SyntaxErrorException::SyntaxErrorException(std::string syntaxErrorMsg): _syntaxErrorMsg(syntaxErrorMsg)
{
}

SyntaxErrorException::~SyntaxErrorException() throw()
{
}

const char* SyntaxErrorException::what() const throw()
{
    return this->_syntaxErrorMsg.c_str();
}