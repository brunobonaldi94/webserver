#pragma once

#include <iostream>
#include "LocationContext.hpp"
#include "Conversor.hpp"

class LocationConfig
{
private:
    LocationContext *_locationContext;
    std::string path;
    std::string _errorPage;
    int errorPageCode;
    int _maxBodySize;
    std::string _rootPath;
    std::vector<std::string> _indexFiles;
    bool _indexFileNotFound;
    bool _autoIndex;
    std::vector<std::string> allowedMethods;
    int _returnCode;
    std::string _returnPath;
    void SetValuesFromLocationContext();

public:
    LocationConfig(LocationContext * locationContext);
    LocationConfig(LocationConfig const & other);
    ~LocationConfig();

    LocationConfig& operator=(LocationConfig const & other);

    std::string GetErrorPage() const;
    int GetErrorPageCode() const;
    int GetMaxBodySize() const;
    std::string GetRootPath() const;
    std::vector<std::string> GetIndexFiles() const;
    bool GetAutoIndex() const;
    std::vector<std::string> GetAllowedMethods() const;
    std::string GetPath() const;
    std::vector<std::string> GetFilesFullPath() const;
    bool GetIndexFileNotFound() const;
    int GetReturnCode() const;
    std::string GetReturnPath() const;

    void SetErrorPage(std::string errorPage);
    void SetErrorPageCode(int errorPageCode);
    void SetMaxBodySize(int maxBodySize);
    void SetRootPath(std::string rootPath);
    void SetIndexFiles(std::vector<std::string> indexFiles);
    void SetAutoIndex(bool autoIndex);
    void SetAllowedMethods(std::vector<std::string> allowedMethods);
    void SetPath(std::string path);
    void SetIndexFileNotFound(bool indexFileNotFound);
    void SetReturnCode(int returnCode);
    bool ShouldRedirect() const;
    StatusCode ReturnRedirectStatus() const;
    void SetReturnPath(std::string returnPath);

};