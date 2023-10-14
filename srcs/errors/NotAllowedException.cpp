
#include "NotAllowedException.hpp"

NotAllowedException::NotAllowedException(const std::string& objectName): _objectName(objectName)
{
}

const char* NotAllowedException::what() const throw()
{
    return this->_objectName.c_str();
}

NotAllowedException::~NotAllowedException() throw()
{
}