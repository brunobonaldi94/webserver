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
  std::string contexts[] = {"server", "location"};
  AContextCreator* contextsCreators[] = {new ServerContextCreator(), new LocationContextCreator()};
  FillMapFromArray(this->_availableContexts, contexts, contextsCreators, sizeof(contexts) / sizeof(std::string));
  //std::string directives[] = {"listen", "server_name", "error_page", "client_max_body_size", "limit_except", "root", "index", "autoindex"};
  //this->_availableDirectives = std::vector<std::string>(directives, directives + sizeof(directives) / sizeof(std::string));
  std::string chars[] = {"{", "}", ";", "\n"};
  this->_allowedChars = std::vector<std::string>(chars, chars + sizeof(chars) / sizeof(std::string));
}

ConfigParser::ConfigParser(ConfigParser const & other)
{
    *this = other;
}

ConfigParser::~ConfigParser()
{
  if (this->_availableContexts.size() > 0)
  {
    for (std::map<std::string, AContextCreator *>::iterator it = this->_availableContexts.begin(); it != this->_availableContexts.end(); ++it)
      delete it->second;
    this->_availableContexts.clear();
  }
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
     std::string path("./config_files/");
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
  std::string word;
  this->ReadFile();
  std::cout << this->_fileContent << std::endl;
  for (std::string::iterator it = this->_fileContent.begin(); it != this->_fileContent.end(); ++it)
  {
    AdvaceOnWhiteSpace(it, this->_fileContent);
    AdvanceOnComment(it, this->_fileContent);
    word = ExtractWord(it, this->_fileContent, this->_allowedChars);
    this->_fileContent = std::string(it, this->_fileContent.end());
    this->ParseContent(this->_fileContent, word);
    if (this->_fileContent.size() == 0)
      return;
    it = this->_fileContent.begin();
    word.clear();
  }
}

bool ConfigParser::ParseContent(std::string& content, std::string& word)
{
  std::pair<const std::string, AContextCreator*> *contextCreator = SafeFindInMap(this->_availableContexts, word);
  if (contextCreator != NULL)
  {
    this->_serverContexts.push_back(contextCreator->second->CreateContext());
    this->_serverContexts.back()->ParseContext(content);
    return true;
  }
  std::pair<const std::string, ADirectiveCreator*> *directiveCreator = SafeFindInMap(this->_availableDirectives, word);
  if (directiveCreator != NULL)
  {
    return true;
  }
  return false;
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


ConfigParser::NotAllowedException::NotAllowedException(const std::string& objectName): _objectName(objectName + " not allowed directive or context")
{
}

const char* ConfigParser::NotAllowedException::what() const throw()
{
    return this->_objectName.c_str();
}

ConfigParser::NotAllowedException::~NotAllowedException() throw()
{
}