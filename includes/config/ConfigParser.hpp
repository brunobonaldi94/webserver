#pragma once

#include <iostream>
#include <fstream>
#include <exception>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <map>
#include <cctype>
#include <algorithm>
#include "AContextCreator.hpp"
#include "ServerContextCreator.hpp"
#include "LocationContextCreator.hpp"
#include "ADirectiveCreator.hpp"
#include "DirectivesCreator.hpp"
#include "MapUtils.hpp"
#include "VectorUtils.hpp"
#include "StringUtils.hpp"
#include "types.hpp"
#include "NotAllowedException.hpp"
#include "NotFoundException.hpp"
#include "ServerConfig.hpp"

class ConfigParser
{
private:
    std::string                                 _fileName;
    std::string                                 _fileContent;
    std::vector<AContext *>                     _serverContexts;

    void ReadFile();

public:
    ConfigParser(const std::string& fileName);
    ConfigParser(ConfigParser const & other);
    ~ConfigParser();

    ConfigParser& operator=(ConfigParser const & other);

    std::vector<AContext *> GetServerContexts() const;
    void AddServerContext(AContext *serverContext);


    bool DirectoryExists(const std::string& path);
    bool ParseConfigFile();
    bool ParseContent(std::string& content, std::string& word);
    void PrintContexts() const;
    void FillDefaultValues();
    std::vector<ServerConfig *> CreateServerConfigs();
};