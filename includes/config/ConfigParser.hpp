#pragma once

#include <iostream>
#include <fstream>
#include <exception>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <cctype>
#include <algorithm>

class ConfigParser
{
private:
    std::string _fileName;
    std::string _fileContent;
    std::vector<std::string> _availableContexts;
    std::vector<std::string> _availableDirectives;
    std::vector<std::string> _allowedChars;

    void ReadFile();

public:
    ConfigParser(const std::string& fileName);
    ConfigParser(ConfigParser const & other);
    ~ConfigParser();

    ConfigParser& operator=(ConfigParser const & other);

    bool DirectoryExists(const std::string& path);
    void ParseConfigFile();
    bool parseWord(const std::string& word) const;

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