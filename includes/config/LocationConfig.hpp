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
    std::string _indexFile;
    bool _autoIndex;
    std::vector<std::string> allowedMethods;
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
    std::string GetIndexFile() const;
    bool GetAutoIndex() const;
    std::vector<std::string> GetAllowedMethods() const;
    std::string GetPath() const;

    void SetErrorPage(std::string errorPage);
    void SetErrorPageCode(int errorPageCode);
    void SetMaxBodySize(int maxBodySize);
    void SetRootPath(std::string rootPath);
    void SetIndexFile(std::string indexFile);
    void SetAutoIndex(bool autoIndex);
    void SetAllowedMethods(std::vector<std::string> allowedMethods);
    void SetPath(std::string path);
};