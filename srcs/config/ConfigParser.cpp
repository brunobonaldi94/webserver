/*
  block interface
    list of directives
    allowed directive
    list of sub-blocks
    allowed sub-blocks
    allowed parent blocks
    hierarchy of directive (i.e.  listen: based on host:port first, if server_name is not specified)
  directive interface
    default
    parse arg
  

  no listen => ip 0.0.0.0:80
  no server_name => server_name localhost
  no root => root html
*/

#include "ConfigParser.hpp"

ConfigParser::ConfigParser(const std::string& fileName): _fileName(fileName) 
{
}

ConfigParser::ConfigParser(ConfigParser const & other)
{
    *this = other;
}

ConfigParser::~ConfigParser()
{
}

ConfigParser& ConfigParser::operator=(ConfigParser const & other)
{
    if (this != &other)
        _fileContent = other._fileContent;
    return *this;
}

bool ConfigParser::DirectoryExists(const std::string& path)
{
   DIR* dir = opendir(path.c_str());
    if (dir) 
    {
        closedir(dir);
        return true;
    }
    return false;
}

void ConfigParser::ReadFile()
{

  try 
  {
     std::string path("../config_files/");
    if (!this->DirectoryExists(path))
      throw ConfigParser::NotFoundException("config_files directory");    
    std::string fullName = path + this->_fileName;
    std::ifstream file(fullName.c_str());
    if (!file.good())
      throw ConfigParser::NotFoundException(this->_fileName);
    std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    this->_fileContent = contents;
  }
  catch(const std::exception& e)
  {
		std::cerr << e.what() << std::endl;
  }
}

void ConfigParser::ParseConfigFile()
{
  this->ReadFile();
  std::cout << this->_fileContent << std::endl;
}

ConfigParser::NotFoundException::NotFoundException(const std::string& objectName): _objectName(objectName + " not found")
{
}

const char* ConfigParser::NotFoundException::what() const throw()
{
    return this->_objectName.c_str();
}

ConfigParser::NotFoundException::~NotFoundException() throw()
{
}