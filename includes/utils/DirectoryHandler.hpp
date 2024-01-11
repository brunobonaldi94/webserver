#pragma once

#include <iostream>
#include <sys/stat.h>
#include <vector>
#include <dirent.h>
#include <ctime>
#include "VectorUtils.hpp"
#include "StringUtils.hpp"

class ADirectoryHandler
{
    public:
        virtual ~ADirectoryHandler();
        virtual std::vector<std::string> getFilesInDirectory(std::string path, bool &directoryExists) = 0;
        virtual std::vector<std::string> getFilesInDirectory(std::string path) = 0;
        virtual bool isInDirectory(std::string path, std::string directory) = 0;
        virtual bool getFileStat(std::string path, struct stat *fileStat);
        std::string getLastModified(struct stat sb);
        std::string getFileFromPath(std::string path);
        std::string getFileExtension(std::string path);
        bool isValidDirectory(std::string path);
        bool isValidFile(std::string path);
        std::string getBaseDirectory(std::string path);
};

class DirectoryHandler : public ADirectoryHandler
 {
    public:
        DirectoryHandler();
        DirectoryHandler(const DirectoryHandler& other);
        DirectoryHandler& operator=(const DirectoryHandler& other);
        ~DirectoryHandler();
        std::vector<std::string> getFilesInDirectory(std::string path);
        std::vector<std::string> getFilesInDirectory(std::string path, bool &directoryExists);
        bool isInDirectory(std::string path, std::string directory);
};