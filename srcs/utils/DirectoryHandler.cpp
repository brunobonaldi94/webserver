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

std::string ADirectoryHandler::getFileExtension(std::string path)
{
  size_t lastDotPos = path.find_last_of(".");
  if (lastDotPos == std::string::npos)
    return "";
  return path.substr(lastDotPos + 1);
}

std::string ADirectoryHandler::getFileFromPath(std::string path)
{
  size_t lastSlashPos = path.find_last_of("/");
  if (lastSlashPos == std::string::npos)
    return path;
  return path.substr(lastSlashPos + 1);
}

bool ADirectoryHandler::directoryExists(std::string path)
{
  struct stat sb;
  return (stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode));
}

DirectoryHandler::DirectoryHandler() {}

DirectoryHandler::~DirectoryHandler() {}

DirectoryHandler& DirectoryHandler::operator=(const DirectoryHandler& other)
{
    if (this != &other)
    {
    }
    return *this;
}

DirectoryHandler::DirectoryHandler(const DirectoryHandler& other)
{
    *this = other;
}

std::vector<std::string> DirectoryHandler::getFilesInDirectory(std::string path)
{
    std::vector<std::string> files;
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir (path.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL)
            files.push_back(ent->d_name);
        closedir (dir);
    } 
    else 
    {
        throw std::runtime_error("Error while reading directory");
    }
    return files;
}

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
  if (path.empty())
    return false;
  std::vector<std::string> files = this->getFilesInDirectory(directory, directoryExists);
  if (!directoryExists)
    return false;
  std::string fileNameFromPath = StringUtils::Split(path, "/").back();
  return VectorUtils<std::string>::hasElement(files, fileNameFromPath);
}

