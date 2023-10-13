#include "NotFoundException.hpp"

NotFoundException::NotFoundException(const std::string& objectName): _objectName(objectName + " not found")
{
}

const char* NotFoundException::what() const throw()
{
    return this->_objectName.c_str();
}

NotFoundException::~NotFoundException() throw()
{
}
