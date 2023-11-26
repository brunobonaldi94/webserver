#include "LocationConfig.hpp"

LocationConfig::LocationConfig(LocationContext *locationContext): _locationContext(locationContext)
{
    this->SetValuesFromLocationContext();
}

LocationConfig::LocationConfig(LocationConfig const & other)
{
    *this = other;
}

LocationConfig::~LocationConfig(){}

LocationConfig& LocationConfig::operator=(LocationConfig const & other)
{
    if (this != &other)
    {
        this->_locationContext = other._locationContext;
    }
    return *this;
}

std::string LocationConfig::GetErrorPage() const
{
    return this->_errorPage;
}

int LocationConfig::GetErrorPageCode() const
{
    return this->errorPageCode;
}

int LocationConfig::GetMaxBodySize() const
{
    return this->_maxBodySize;
}

std::string LocationConfig::GetRootPath() const
{
    return this->_rootPath;
}

std::string LocationConfig::GetIndexFile() const
{
    return this->_indexFile;
}

bool LocationConfig::GetAutoIndex() const
{
    return this->_autoIndex;
}

std::vector<std::string> LocationConfig::GetAllowedMethods() const
{
    return this->allowedMethods;
}

void LocationConfig::SetErrorPage(std::string errorPage)
{
    this->_errorPage = errorPage;
}

void LocationConfig::SetErrorPageCode(int errorPageCode)
{
    this->errorPageCode = errorPageCode;
}

void LocationConfig::SetMaxBodySize(int maxBodySize)
{
    this->_maxBodySize = maxBodySize;
}

void LocationConfig::SetRootPath(std::string rootPath)
{
    this->_rootPath = rootPath;
}

void LocationConfig::SetIndexFile(std::string indexFile)
{
    this->_indexFile = indexFile;
}

void LocationConfig::SetAutoIndex(bool autoIndex)
{
    this->_autoIndex = autoIndex;
}

void LocationConfig::SetAllowedMethods(std::vector<std::string> allowedMethods)
{
    this->allowedMethods = allowedMethods;
}

std::string LocationConfig::GetPath() const
{
    return this->path;
}

void LocationConfig::SetPath(std::string path)
{
    this->path = path;
}

void LocationConfig::SetValuesFromLocationContext()
{
    this->path = this->_locationContext->GetUri();

    ErrorPageDirective *errorPageDirective = static_cast<ErrorPageDirective *>(this->_locationContext->GetDirective("error_page"));
    this->_errorPage = errorPageDirective->GetPath();
    this->errorPageCode = std::atoi(errorPageDirective->GetCode().c_str());

    ClientMaxBodySizeDirective *maxBodySizeDirective = static_cast<ClientMaxBodySizeDirective *>(this->_locationContext->GetDirective("client_max_body_size"));
    this->_maxBodySize = BytesConversor(maxBodySizeDirective->GetSize());

    RootDirective *rootDirective = static_cast<RootDirective *>(this->_locationContext->GetDirective("root"));
    this->_rootPath = rootDirective->GetPath();

    IndexDirective *indexDirective = static_cast<IndexDirective *>(this->_locationContext->GetDirective("index"));
    this->_indexFile = indexDirective->GetIndex()[0];

    AutoIndexDirective *autoIndexDirective = static_cast<AutoIndexDirective *>(this->_locationContext->GetDirective("autoindex"));
    this->_autoIndex = autoIndexDirective->GetAutoIndex();

    LimitExceptDirective *allowMethodsDirective = static_cast<LimitExceptDirective *>(this->_locationContext->GetDirective("limit_except"));
    this->allowedMethods = allowMethodsDirective->GetMethods();
}