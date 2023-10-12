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

class ConfigParser
{
private:
    std::string                                 _fileName;
    std::string                                 _fileContent;
    MapContextCreator                           _availableContexts;
    MapDirCreator                               _availableDirectives;
    std::vector<std::string>                    _allowedChars;
    std::vector<AContext *>                     _serverContexts;

    void ReadFile();

public:
    ConfigParser(const std::string& fileName);
    ConfigParser(ConfigParser const & other);
    ~ConfigParser();

    ConfigParser& operator=(ConfigParser const & other);

    bool DirectoryExists(const std::string& path);
    void ParseConfigFile();
    bool ParseContent(std::string& content, std::string& word);

    class NotFoundException : public std::exception
    {
        private:
            std::string _objectName;
        public:
            NotFoundException(const std::string& objectName);
            virtual ~NotFoundException() throw();
            virtual const char* what() const throw();
    };

    class NotAllowedException : public std::exception
    {
        private:
            std::string _objectName;
        public:
            NotAllowedException(const std::string& objectName);
            virtual ~NotAllowedException() throw();
            virtual const char* what() const throw();
    };
};