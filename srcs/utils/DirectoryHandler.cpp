#include "DirectoryHandler.hpp"


ADirectoryHandler::~ADirectoryHandler() {}

bool ADirectoryHandler::getFileStat(std::string path, struct stat *fileStat)
{
  if (stat(path.c_str(), fileStat) < 0)
    return false;
  return true;
}

std::string ADirectoryHandler::getLastModified(struct stat sb)
{
 
  time_t t = sb.st_mtime;
  struct tm *tm = localtime(&t);

  char buffer[100];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm);
  return std::string(buffer);
}

DirectoryHandler::DirectoryHandler() {}

DirectoryHandler::~DirectoryHandler() {}

std::vector<std::string> DirectoryHandler::getFilesInDirectory(std::string path, bool &directoryExists)
{
  std::vector<std::string> files;
  DIR *dir;
  struct dirent *ent;
  directoryExists = false;
  if ((dir = opendir (path.c_str())) != NULL)
  {
    directoryExists = true;
    while ((ent = readdir (dir)) != NULL)
    {
      std::string fileName = ent->d_name;
      files.push_back(fileName);
    }
    closedir (dir);
  }
  else
  {
    std::cout << "Could not open directory" << std::endl;
  }
  return files;
}

bool DirectoryHandler::isInDirectory(std::string path, std::string directory)
{
  bool directoryExists;
  std::vector<std::string> files = this->getFilesInDirectory(directory, directoryExists);
  if (!directoryExists)
    return false;
  std::string fileNameFromPath = StringUtils::Split(path, "/").back();
  return VectorUtils<std::string>::hasElement(files, fileNameFromPath);
}

