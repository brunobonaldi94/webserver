#include "ServerConfig.hpp"

ServerConfig::ServerConfig(ServerContext *serverContext): _serverContext(serverContext)
{
    this->SetValuesFromServerContext();
}

ServerConfig::ServerConfig(ServerConfig const & other)
{
    *this = other;
}

ServerConfig::~ServerConfig()
{
    VectorUtils<LocationConfig *>::clearVector(this->_locations);
}

ServerConfig & ServerConfig::operator=(ServerConfig const & other)
{
    if (this != &other)
    {
        this->_serverContext = other._serverContext;
        this->_locations = other._locations;
        this->_port = other._port;
        this->_host = other._host;
        this->_serverNames = other._serverNames;
        this->_errorPage = other._errorPage;
        this->errorPageCode = other.errorPageCode;
        this->_maxBodySize = other._maxBodySize;
        this->_rootPath = other._rootPath;
        this->_indexFiles = other._indexFiles;
        this->_autoIndex = other._autoIndex;
        this->_cgiBinaryPath = other._cgiBinaryPath;
        this->_cgiExtension = other._cgiExtension;
    }
    return *this;
}

std::string ServerConfig::GetPort() const
{
    return this->_port;
}

std::string ServerConfig::GetHost() const
{
    return this->_host;
}

std::vector<std::string> ServerConfig::GetServerNames() const
{
    return this->_serverNames;
}

std::string ServerConfig::GetErrorPage() const
{
    return this->_errorPage;
}

int ServerConfig::GetErrorPageCode() const
{
    return this->errorPageCode;
}

ssize_t ServerConfig::GetMaxBodySize() const
{
    return this->_maxBodySize;
}

std::string ServerConfig::GetRootPath() const
{
    return this->_rootPath;
}

std::vector<std::string>  ServerConfig::GetIndexFiles() const
{
    return this->_indexFiles;
}

bool ServerConfig::GetAutoIndex() const
{
    return this->_autoIndex;
}

std::string ServerConfig::GetCgiBinaryPath() const
{
    return this->_cgiBinaryPath;
}

std::string ServerConfig::GetCgiExtension() const
{
    return this->_cgiExtension;
}

void ServerConfig::SetPort(std::string port)
{
    this->_port = port;
}

void ServerConfig::SetHost(std::string host)
{
    this->_host = host;
}

void ServerConfig::SetServerNames(std::vector<std::string> serverNames)
{
    this->_serverNames = serverNames;
}

void ServerConfig::SetErrorPage(std::string errorPage)
{
    this->_errorPage = errorPage;
}

void ServerConfig::SetErrorPageCode(int errorPageCode)
{
    this->errorPageCode = errorPageCode;
}

void ServerConfig::SetMaxBodySize(ssize_t maxBodySize)
{
    this->_maxBodySize = maxBodySize;
}

void ServerConfig::SetRootPath(std::string rootPath)
{
    this->_rootPath = rootPath;
}

void ServerConfig::SetIndexFiles(std::vector<std::string>  indexFile)
{
    this->_indexFiles = indexFile;
}

void ServerConfig::SetAutoIndex(bool autoIndex)
{
    this->_autoIndex = autoIndex;
}

void ServerConfig::SetCgiBinaryPath(std::string cgiBinaryPath)
{
    this->_cgiBinaryPath = cgiBinaryPath;
}

void ServerConfig::SetCgiExtension(std::string cgiExtension)
{
    this->_cgiExtension = cgiExtension;
}


void ServerConfig::SetValuesFromServerContext()
{
  std::vector<AContext *> *locationContexts = this->_serverContext->GetSubContextsByName("location");
  if (locationContexts == NULL)
    return;
  for (std::vector<AContext *>::iterator it = locationContexts->begin(); it != locationContexts->end(); it++)
  {
    LocationContext *locationContext = static_cast<LocationContext *>(*it);
    LocationConfig *locationConfig = new LocationConfig(locationContext);
    this->_locations.push_back(locationConfig);
  }
  ListenDirective *listenDirective = static_cast<ListenDirective *>(this->_serverContext->GetDirective("listen"));
  this->_port = listenDirective->GetPort();
  this->_host = listenDirective->GetHost();
  
  ServerNameDirective *serverNameDirective = static_cast<ServerNameDirective *>(this->_serverContext->GetDirective("server_name"));
  this->_serverNames = serverNameDirective->GetNames();

  ErrorPageDirective *errorPageDirective = static_cast<ErrorPageDirective *>(this->_serverContext->GetDirective("error_page"));
  this->_errorPage = errorPageDirective->GetPath();
  this->errorPageCode = std::atoi(errorPageDirective->GetCode().c_str());

  ClientMaxBodySizeDirective *maxBodySizeDirective = static_cast<ClientMaxBodySizeDirective *>(this->_serverContext->GetDirective("client_max_body_size"));
  this->_maxBodySize = BytesConversor(maxBodySizeDirective->GetSize());

  RootDirective *rootDirective = static_cast<RootDirective *>(this->_serverContext->GetDirective("root"));
  this->_rootPath = rootDirective->GetPath();

  IndexDirective *indexDirective = static_cast<IndexDirective *>(this->_serverContext->GetDirective("index"));
  this->_indexFiles = indexDirective->GetIndex();

  AutoIndexDirective *autoIndexDirective = static_cast<AutoIndexDirective *>(this->_serverContext->GetDirective("autoindex"));
  this->_autoIndex = autoIndexDirective->GetAutoIndex();

  CgiDirective *cgiPathDirective = static_cast<CgiDirective *>(this->_serverContext->GetDirective("cgi"));
  this->_cgiBinaryPath = cgiPathDirective->GetBinaryPath();
  this->_cgiExtension = cgiPathDirective->GetExtension();
}

LocationConfig *ServerConfig::GetLocationConfig(std::string path)
{
  for (std::vector<LocationConfig *>::iterator it = this->_locations.begin(); it != this->_locations.end(); it++)
  {
    if ((*it)->GetPath() == path)
      return (*it);
  }
  return NULL;
}

std::vector<LocationConfig *> ServerConfig::GetLocationConfigs()
{
  return this->_locations;
}