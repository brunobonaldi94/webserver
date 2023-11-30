#pragma once

#include <iostream>
#include "ServerContext.hpp"
#include "LocationConfig.hpp"
#include "Conversor.hpp"

class ServerConfig
{
private:

    ServerContext *_serverContext;
    std::vector<LocationConfig *> _locations;
    std::string _port;
    std::string _host;
    std::vector<std::string> _serverNames;
    std::string _errorPage;
    int errorPageCode;
    ssize_t _maxBodySize;
    std::string _rootPath;
    std::vector<std::string> _indexFiles;
    bool _autoIndex;
    std::string _cgiBinaryPath;
    std::string _cgiExtension;
    void SetValuesFromServerContext();


public:
    ServerConfig(ServerContext * serverContext);
    ServerConfig(ServerConfig const & other);
    ~ServerConfig();

    ServerConfig& operator=(ServerConfig const & other);

    std::string GetPort() const;
    std::string GetHost() const;
    std::vector<std::string> GetServerNames() const;
    std::string GetErrorPage() const;
    int GetErrorPageCode() const;
    ssize_t GetMaxBodySize() const;
    std::string GetRootPath() const;
    std::vector<std::string> GetIndexFiles() const;
    bool GetAutoIndex() const;
    std::string GetCgiBinaryPath() const;
    std::string GetCgiExtension() const;

    void SetPort(std::string port);
    void SetHost(std::string host);
    void SetServerNames(std::vector<std::string> serverNames);
    void SetErrorPage(std::string errorPage);
    void SetErrorPageCode(int errorPageCode);
    void SetMaxBodySize(ssize_t maxBodySize);
    void SetRootPath(std::string rootPath);
    void SetIndexFiles(std::vector<std::string> indexFiles);
    void SetAutoIndex(bool autoIndex);
    void SetCgiBinaryPath(std::string cgiBinaryPath);
    void SetCgiExtension(std::string cgiExtension);

    LocationConfig * GetLocationConfig(std::string path);

};