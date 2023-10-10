#pragma once

#include <iostream>
#include <fstream>
#include <exception>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

class ConfigParser
{
private:
    std::string _fileName;
    std::string _fileContent;

    void ReadFile();

public:
    ConfigParser(const std::string& fileName);
    ConfigParser(ConfigParser const & other);
    ~ConfigParser();

    ConfigParser& operator=(ConfigParser const & other);

    bool DirectoryExists(const std::string& path);
    void ParseConfigFile();

    class NotFoundException : public std::exception
    {
        private:
            std::string _objectName;
        public:
            NotFoundException(const std::string& objectName);
            virtual ~NotFoundException() throw();
            virtual const char* what() const throw();
    };
};