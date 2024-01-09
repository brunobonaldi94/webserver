#pragma once

#include <iostream>
#include <map>
#include "MapUtils.hpp"
class MimeTypes
{
public:
    MimeTypes();
    std::string ExtensionToType(const std::string& extension);
private:
    std::map<std::string,std::string> m_mime;
};
