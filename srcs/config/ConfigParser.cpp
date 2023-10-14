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

  std::string path("./config_files/");
  if (!this->DirectoryExists(path))
    throw NotFoundException("config_files directory");    
  std::string fullName = path + this->_fileName;
  std::ifstream file(fullName.c_str());
  if (!file.good())
    throw NotFoundException(this->_fileName);
  std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  this->_fileContent = contents;
}

bool ConfigParser::ParseConfigFile()
{
  try 
  {
    std::string word;
    this->ReadFile();
    Logger::Debug("ConfigParser::ParseConfigFile _fileContent = ", INFO, this->_fileContent);
    for (std::string::iterator it = this->_fileContent.begin(); it != this->_fileContent.end(); ++it)
    {
      StringUtils::AdvaceOnWhiteSpace(it, this->_fileContent);
      StringUtils::AdvanceOnComment(it, this->_fileContent);
      word = StringUtils::ExtractWord(it, this->_fileContent);
      this->ParseContent(this->_fileContent, word);
      if (this->_fileContent.size() == 0)
        return true;
      it = this->_fileContent.begin();
      word.clear();
    }
    return true;
  } 
  catch(const std::exception& e) 
  {
    Logger::PrintMessage(ERROR, e.what());
    return false;
  }
}

bool ConfigParser::ParseContent(std::string& content, std::string& word)
{
  if (word != "server")
  {
    throw SyntaxErrorException("No server context block");
    return false;
  }
  ServerContext *serverContext = new ServerContext();
  this->_serverContexts.push_back(serverContext);
  serverContext->ParseContext(content);
  return true;
}