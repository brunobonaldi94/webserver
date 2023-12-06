#include "LocationConfig.hpp"

LocationConfig::LocationConfig(LocationContext *locationContext): _locationContext(locationContext), _returnCode(-1)
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

std::vector<std::string> LocationConfig::GetIndexFiles() const
{
    return this->_indexFiles;
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

void LocationConfig::SetIndexFiles(std::vector<std::string> indexFiles)
{
    this->_indexFiles = indexFiles;
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
    this->_indexFiles = indexDirective->GetIndex();
    this->_indexFileNotFound = indexDirective->GetIndex().size() == 0 ? true : false;

    AutoIndexDirective *autoIndexDirective = static_cast<AutoIndexDirective *>(this->_locationContext->GetDirective("autoindex"));
    this->_autoIndex = autoIndexDirective->GetAutoIndex();

    LimitExceptDirective *allowMethodsDirective = static_cast<LimitExceptDirective *>(this->_locationContext->GetDirective("limit_except"));
    this->allowedMethods = allowMethodsDirective->GetMethods();

    ReturnDirective *returnDirective = static_cast<ReturnDirective *>(this->_locationContext->GetDirective("return"));
    this->_returnCode = std::atoi(returnDirective->GetCode().c_str());
    this->_returnPath = returnDirective->GetPath();
}

std::vector<std::string> LocationConfig::GetFilesFullPath() const
{
    std::vector<std::string> filesFullPath;
    for (std::vector<std::string>::const_iterator it = this->_indexFiles.begin(); it != this->_indexFiles.end(); it++)
    {
        filesFullPath.push_back(this->_rootPath + "/" + *it);
    }
    return  filesFullPath;
}

bool LocationConfig::GetIndexFileNotFound() const
{
    return this->_indexFileNotFound;
}

void LocationConfig::SetIndexFileNotFound(bool indexFileNotFound)
{
    this->_indexFileNotFound = indexFileNotFound;
}

int LocationConfig::GetReturnCode() const
{
    return this->_returnCode;
}

void LocationConfig::SetReturnCode(int returnCode)
{
    this->_returnCode = returnCode;
}

bool LocationConfig::ShouldRedirect() const
{
    return this->_returnCode != -1;
}

StatusCode LocationConfig::ReturnRedirectStatus() const
{
    if (this->_returnCode == 301)
        return HTTPStatus::MOVED_PERMANENTLY;
    else if (this->_returnCode == 302)
        return HTTPStatus::FOUND;
    else if (this->_returnCode == 303)
        return HTTPStatus::SEE_OTHER;
    else if (this->_returnCode == 307)
        return HTTPStatus::TEMPORARY_REDIRECT;
    else if (this->_returnCode == 308)
        return HTTPStatus::PERMANENT_REDIRECT;
    return HTTPStatus::INTERNAL_SERVER_ERROR;
}

std::string LocationConfig::GetReturnPath() const
{
    return this->_returnPath;
}

void LocationConfig::SetReturnPath(std::string returnPath)
{
    this->_returnPath = returnPath;
}