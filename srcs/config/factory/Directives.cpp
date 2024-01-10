#include "Directives.hpp"

ListenDirective::ListenDirective()
{
}

ListenDirective::ListenDirective(ListenDirective const & other)
{
    *this = other;
}

ListenDirective::~ListenDirective()
{
}

ListenDirective& ListenDirective::operator=(ListenDirective const & other)
{
    if (this != &other)
    {
        this->_host = other._host;
        this->_port = other._port;
    }
    return (*this);
}

void ListenDirective::SetHost(std::string host)
{
    this->_host = host;
}

void ListenDirective::SetPort(int port)
{
    this->_port = port;
}

std::string ListenDirective::GetHost() const
{
    return this->_host;
}

std::string ListenDirective::GetPort() const
{
    return this->_port;
}

bool ListenDirective::ValidatePort(std::string port) const
{
    for (size_t i = 0; i < port.size(); i++)
    {
        if (!isdigit(port[i]))
            return false;
    }
    int portNumber = std::atoi(port.c_str());
    if (portNumber < 0 || portNumber > 65535)
        return false;
    return true;
}

//http://nginx.org/en/docs/http/ngx_http_core_module.html#listen
//check this link to a better validation
bool ListenDirective::ValidateHost(std::string host) const
{
    std::vector<std::string> tokens = StringUtils::Split(host, ".");
    if (tokens.size() != 4)
    {
        tokens = StringUtils::Split(host, ":");
        if (tokens.size() != 8)
            return false;
        return true;
    }
    return true;
}

void ListenDirective::ParseDirective(std::string &line)
{
    ADirective::ParseDirective(line);
    std::vector<std::string> hostPort = StringUtils::Split(line, ":");
    if (hostPort.size() == 0)
        throw SyntaxErrorException("No host:port specified");
    if (hostPort.size() == 1 && this->ValidatePort(hostPort[0]))
    {
        this->_host = "127.0.0.1";
        this->_port = hostPort[0];
    }
    else if (hostPort.size() == 1 && this->ValidateHost(hostPort[0]))
    {
        this->_host = hostPort[0];
        this->_port = "80";
    }
    else if (hostPort.size() == 2 && this->ValidateHost(hostPort[0]) && this->ValidatePort(hostPort[1]))
    {
        this->_host = hostPort[0];
        this->_port = hostPort[1];
    }
    else
        throw SyntaxErrorException("Invalid host:port " + line);
}

void ListenDirective::PrintDirective() const
{
    std::string msg = "host: " + this->_host + " port: " + this->_port;
    Logger::Debug("ListenDirective::PrintDirective", SUCCESS, msg);
}

void ListenDirective::FillDefaultValues()
{
    this->_port = "80";
    this->_host = "127.0.0.1";
}

ServerNameDirective::ServerNameDirective()
{
}

ServerNameDirective::ServerNameDirective(ServerNameDirective const & other)
{
    *this = other;
}

ServerNameDirective::~ServerNameDirective()
{
}

ServerNameDirective& ServerNameDirective::operator=(ServerNameDirective const & other)
{
    if (this != &other)
    {
        this->_names = other._names;
    }
    return (*this);
}

void ServerNameDirective::AddName(std::string name)
{
    this->_names.push_back(name);
}

std::vector<std::string> ServerNameDirective::GetNames() const
{
    return this->_names;
}

void ServerNameDirective::ParseDirective(std::string &line)
{
    ADirective::ParseDirective(line);
    std::vector<std::string> tokens = StringUtils::Split(line, SPACE);
    if (tokens.size() == 0)
        throw SyntaxErrorException("No server name specified");
    for (size_t i = 0; i < tokens.size(); i++)
        this->_names.push_back(tokens[i]);
}

void ServerNameDirective::PrintDirective() const
{
    std::string names;
    for (size_t i = 0; i < this->_names.size() - 1; i++)
        names += this->_names[i] + " ";
    names += this->_names[this->_names.size() - 1];
    Logger::Debug("ServerNameDirective::PrintDirective", SUCCESS, names);
}

void ServerNameDirective::FillDefaultValues()
{
    if (this->SetDefaultFromParent())
        return ;
    this->_names.push_back("localhost");
}

ErrorPageDirective::ErrorPageDirective()
{
}

ErrorPageDirective::ErrorPageDirective(ErrorPageDirective const & other)
{
    *this = other;
}

ErrorPageDirective::~ErrorPageDirective()
{
}

ErrorPageDirective& ErrorPageDirective::operator=(ErrorPageDirective const & other)
{
    if (this != &other)
    {
        this->_code = other._code;
        this->_path = other._path;
    }
    return (*this);
}

void ErrorPageDirective::SetCode(int code)
{
    this->_code = code;
}

void ErrorPageDirective::SetPath(std::string path)
{
    this->_path = path;
}

std::string ErrorPageDirective::GetCode() const
{
    return this->_code;
}

std::string ErrorPageDirective::GetPath() const
{
    return this->_path;
}

bool ErrorPageDirective::ValidateCode(std::string code) const
{
    return HTTPStatus::validateStatusCode(code);
}


void ErrorPageDirective::ParseDirective(std::string &line)
{
    ADirective::ParseDirective(line);
    std::vector<std::string> tokens = StringUtils::Split(line, SPACE);
    if (tokens.size() < 2)
        throw SyntaxErrorException("Invalid error_page directive - " + line);
    if (!this->ValidateCode(tokens[0]))
        throw SyntaxErrorException("Invalid error_page code - " + tokens[0]);
    this->_code = tokens[0];
    this->_path = tokens[1];
}

void ErrorPageDirective::PrintDirective() const
{
    std::string msg = "code: " + this->_code + " path: " + this->_path;
    Logger::Debug("ErrorPageDirective::PrintDirective", SUCCESS, msg);
}

void ErrorPageDirective::FillDefaultValues()
{
    if (this->SetDefaultFromParent())
        return ;
    this->_code = "404";
    this->_path = "/404.html";
}

bool ErrorPageDirective::SetDefaultFromParent()
{
    AContext *parent = this->GetContextUpToLevel(2);
    if (!parent)
        return false;
    MapDirectives directives = parent->GetDirectives();
    PairDirectives *parentErrorPage = MapUtils<std::string, ADirective* >::SafeFindMap(directives, "error_page");
    if (parentErrorPage == NULL)
        return false;
    ErrorPageDirective *errorPageDirective = dynamic_cast<ErrorPageDirective *>(parentErrorPage->second);
    if (!errorPageDirective)
        return false;
    this->_code = errorPageDirective->GetCode();
    this->_path = errorPageDirective->GetPath();
    return true;
}

ClientMaxBodySizeDirective::ClientMaxBodySizeDirective()
{
}

ClientMaxBodySizeDirective::ClientMaxBodySizeDirective(ClientMaxBodySizeDirective const & other)
{
    *this = other;
}

ClientMaxBodySizeDirective::~ClientMaxBodySizeDirective()
{
}

ClientMaxBodySizeDirective& ClientMaxBodySizeDirective::operator=(ClientMaxBodySizeDirective const & other)
{
    if (this != &other)
    {
        this->_size = other._size;
    }
    return (*this);
}

void ClientMaxBodySizeDirective::SetSize(std::string size)
{
    this->_size = size;
}

std::string ClientMaxBodySizeDirective::GetSize() const
{
    return this->_size;
}

bool ClientMaxBodySizeDirective::ValidateSize(std::string size) const
{
    for (size_t i = 0; i < size.size() - 1; i++)
    {
        if (!isdigit(size[i]))
            return false;
    }
    std::string unitsArr[] = {"B", "K", "M", "G", "T", "P", "E", "Z", "Y"};
    std::vector<std::string> units;
    VectorUtils<std::string>::FillVectorFromArray(units, unitsArr, 9);
    std::string unitSize = std::string(1, std::toupper(size[size.size() -1]));
    if (!VectorUtils<std::string>::hasElement(units, unitSize) && !isdigit(size[size.size() -1]))
        return false;
    int sizeNumber = std::atoi(size.c_str());
    if (sizeNumber < 0)
        return false;
    return true;
}

void ClientMaxBodySizeDirective::ParseDirective(std::string &line)
{
    ADirective::ParseDirective(line);
    std::vector<std::string> tokens = StringUtils::Split(line, SPACE);
    if (tokens.size() == 0 || tokens.size() > 1)
        throw SyntaxErrorException("Invalid client_max_body_size directive - " + line);
    if (!this->ValidateSize(tokens[0]))
        throw SyntaxErrorException("Invalid client_max_body_size size - " + tokens[0]);
    this->_size = tokens[0];
}

void ClientMaxBodySizeDirective::PrintDirective() const
{
    std::string msg = "size: " + this->_size;
    Logger::Debug("ClientMaxBodySizeDirective::PrintDirective", SUCCESS, msg);
}

void ClientMaxBodySizeDirective::FillDefaultValues()
{
    if (this->SetDefaultFromParent())
        return ;
    this->_size = "1M";
}

bool ClientMaxBodySizeDirective::SetDefaultFromParent()
{
    AContext *parent = this->GetContextUpToLevel(2);
    if (!parent)
        return false;
    MapDirectives directives = parent->GetDirectives();
    PairDirectives *parentClientMaxBodySize = MapUtils<std::string, ADirective* >::SafeFindMap(directives, "client_max_body_size");
    if (parentClientMaxBodySize == NULL)
        return false;
    ClientMaxBodySizeDirective *clientMaxBodySizeDirective = dynamic_cast<ClientMaxBodySizeDirective *>(parentClientMaxBodySize->second);
    if (!clientMaxBodySizeDirective)
        return false;
    if (clientMaxBodySizeDirective)
        this->_size = clientMaxBodySizeDirective->GetSize();
    return true;
}

IndexDirective::IndexDirective()
{
}

IndexDirective::IndexDirective(IndexDirective const & other)
{
    *this = other;
}

IndexDirective::~IndexDirective()
{
}

IndexDirective& IndexDirective::operator=(IndexDirective const & other)
{
    if (this != &other)
        this->_index = other._index;
    return (*this);
}

void IndexDirective::AddIndex(std::string index)
{
    this->_index.push_back(index);
}

std::vector<std::string> IndexDirective::GetIndex() const
{
    return this->_index;
}

void IndexDirective::ParseDirective(std::string &line)
{
    ADirective::ParseDirective(line);
    std::vector<std::string> tokens = StringUtils::Split(line, SPACE);
    if (tokens.size() == 0)
        throw SyntaxErrorException("No index specified");
    for (size_t i = 0; i < tokens.size(); i++)
        this->_index.push_back(tokens[i]);
}

void IndexDirective::PrintDirective() const
{
    std::string index;
    if (this->_index.size() == 0)
    {
        index = "No index found";
        Logger::Debug("IndexDirective::PrintDirective", ERROR , index);
        return ;
    }
    for (size_t i = 0; i < this->_index.size() - 1; i++)
        index += this->_index[i] + " ";
    index += this->_index[this->_index.size() - 1];
    Logger::Debug("IndexDirective::PrintDirective", SUCCESS, index);
}

void IndexDirective::FillDefaultValues()
{
    if (this->SetDefaultFromParent())
        return ;
    AContext *context = this->GetParentContext();
    if (!context || dynamic_cast<ServerContext *>(context))
        return ;
    this->_index.push_back("index.html");
}

bool IndexDirective::SetDefaultFromParent()
{
    AContext *parent = this->GetContextUpToLevel(2);
    if (!parent)
        return false;
    MapDirectives directives = parent->GetDirectives();
    PairDirectives *parentIndex = MapUtils<std::string, ADirective* >::SafeFindMap(directives, "index");
    if (parentIndex == NULL)
        return false;
    IndexDirective *indexDirective = dynamic_cast<IndexDirective *>(parentIndex->second);
    if (!indexDirective)
        return false;
    if (indexDirective)
        this->_index = indexDirective->GetIndex();
    return true;
}

RootDirective::RootDirective()
{
}

RootDirective::RootDirective(RootDirective const & other)
{
    *this = other;
}

RootDirective::~RootDirective()
{
}

RootDirective& RootDirective::operator=(RootDirective const & other)
{
    if (this != &other)
        this->_path = other._path;
    return (*this);
}

void RootDirective::SetPath(std::string path)
{
    this->_path = path;
}

std::string RootDirective::GetPath() const
{
    return this->_path;
}

void RootDirective::ParseDirective(std::string &line)
{
    ADirective::ParseDirective(line);
    std::vector<std::string> tokens = StringUtils::Split(line, SPACE);
    if (tokens.size() != 1)
        throw SyntaxErrorException("Invalid root directive - " + line);
    this->_path = tokens[0];
}

void RootDirective::PrintDirective() const
{
    std::string msg = "path: " + this->_path;
    Logger::Debug("RootDirective::PrintDirective", SUCCESS, msg);
}

void RootDirective::FillDefaultValues()
{
    if (this->SetDefaultFromParent())
        return ;
    this->_path = "html";
}

bool RootDirective::SetDefaultFromParent()
{
    AContext *parent = this->GetContextUpToLevel(2);
    if (!parent)
        return false;
    MapDirectives directives = parent->GetDirectives();
    PairDirectives *parentRoot = MapUtils<std::string, ADirective* >::SafeFindMap(directives, "root");
    if (parentRoot == NULL)
        return false;
    RootDirective *rootDirective = dynamic_cast<RootDirective *>(parentRoot->second);
    if (!rootDirective)
        return false;
    this->_path = rootDirective->GetPath();
    return true;
}

AutoIndexDirective::AutoIndexDirective()
{
}

AutoIndexDirective::AutoIndexDirective(AutoIndexDirective const & other)
{
    *this = other;
}

AutoIndexDirective::~AutoIndexDirective()
{
}

AutoIndexDirective& AutoIndexDirective::operator=(AutoIndexDirective const & other)
{
    if (this != &other)
        this->_autoIndex = other._autoIndex;
    return (*this);
}

void AutoIndexDirective::SetAutoIndex(bool autoIndex)
{
    this->_autoIndex = autoIndex;
}

bool AutoIndexDirective::GetAutoIndex() const
{
    return this->_autoIndex;
}

void AutoIndexDirective::ParseDirective(std::string &line)
{
    ADirective::ParseDirective(line);
    std::vector<std::string> tokens = StringUtils::Split(line, SPACE);
    if (tokens.size() != 1)
        throw SyntaxErrorException("Invalid autoindex directive - " + line);
    if (tokens[0] == "on")
        this->_autoIndex = true;
    else if (tokens[0] == "off")
        this->_autoIndex = false;
    else
        throw SyntaxErrorException("Invalid autoindex directive - " + line);
}

void AutoIndexDirective::PrintDirective() const
{
    std::string msg = "autoindex: " + std::string(this->_autoIndex ? "on" : "off");
    Logger::Debug("AutoIndexDirective::PrintDirective", SUCCESS, msg);
}

void AutoIndexDirective::FillDefaultValues()
{
    if (this->SetDefaultFromParent())
        return ;
    this->_autoIndex = false;
}

bool AutoIndexDirective::SetDefaultFromParent()
{
    AContext *parent = this->GetContextUpToLevel(2);
    if (!parent)
        return false;
    MapDirectives directives = parent->GetDirectives();
    PairDirectives *parentAutoIndex = MapUtils<std::string, ADirective* >::SafeFindMap(directives, "autoindex");
    if (parentAutoIndex == NULL)
        return false;
    AutoIndexDirective *autoIndexDirective = dynamic_cast<AutoIndexDirective *>(parentAutoIndex->second);
    if (!autoIndexDirective)
        return false;
    this->_autoIndex = autoIndexDirective->GetAutoIndex();
    return true;
}

LimitExceptDirective::LimitExceptDirective()
{
}

LimitExceptDirective::LimitExceptDirective(LimitExceptDirective const & other)
{
    *this = other;
}

LimitExceptDirective::~LimitExceptDirective()
{
}

LimitExceptDirective& LimitExceptDirective::operator=(LimitExceptDirective const & other)
{
    if (this != &other)
        this->_methods = other._methods;
    return (*this);
}

void LimitExceptDirective::AddMethod(std::string method)
{
    this->_methods.push_back(method);
}

std::vector<std::string> LimitExceptDirective::GetMethods() const
{
    return this->_methods;
}

bool LimitExceptDirective::ValidateMethods(std::string methods) const
{
    std::vector<std::string> tokens = StringUtils::Split(methods, SPACE);
    if (tokens.size() == 0)
        return false;
    std::string httpMethods[] = {"GET", "HEAD", "POST", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE"};
    for (size_t i = 0; i < tokens.size(); i++)
    {
        bool found = false;
        for (size_t j = 0; j < sizeof(httpMethods) / sizeof(std::string); j++)
        {
            if (tokens[i] == httpMethods[j])
            {
                found = true;
                break;
            }
        }
        if (!found)
            return false;
    }
    return true;
}

void LimitExceptDirective::ParseDirective(std::string &line)
{
    ADirective::ParseDirective(line);
    std::vector<std::string> tokens = StringUtils::Split(line, SPACE);
    if (tokens.size() == 0)
        throw SyntaxErrorException("No method specified");
    if (!this->ValidateMethods(line))
        throw SyntaxErrorException("Invalid method - " + line);
    for (size_t i = 0; i < tokens.size(); i++)
        this->_methods.push_back(tokens[i]);
}

void LimitExceptDirective::PrintDirective() const
{
    std::string methods;
    for (size_t i = 0; i < this->_methods.size() - 1; i++)
        methods += this->_methods[i] + " ";
    methods += this->_methods[this->_methods.size() - 1];
    Logger::Debug("LimitExceptDirective::PrintDirective", SUCCESS, methods);
}

void LimitExceptDirective::FillDefaultValues()
{
    this->_methods.push_back("GET");
}

CgiDirective::CgiDirective()
{
}

CgiDirective::CgiDirective(CgiDirective const & other)
{
    *this = other;
}

CgiDirective::~CgiDirective()
{
}

CgiDirective& CgiDirective::operator=(CgiDirective const & other)
{
    if (this != &other)
    {
        this->_binaryPath = other._binaryPath;
        this->_extension = other._extension;
    }
    return (*this);
}

void CgiDirective::SetBinaryPath(std::string binaryPath)
{
    this->_binaryPath = binaryPath;
}

void CgiDirective::SetExtension(std::string extension)
{
    this->_extension = extension;
}

std::string CgiDirective::GetBinaryPath() const
{
    return this->_binaryPath;
}

std::string CgiDirective::GetExtension() const
{
    return this->_extension;
}

bool CgiDirective::ValidateExtension(std::string extension) const
{
    if (extension[0] != '.')
        return false;
    for (size_t i = 1; i < extension.size(); i++)
    {
        if (!std::isalnum(extension[i]))
            return false;
    }
    return true;
}

void CgiDirective::ParseDirective(std::string &line)
{
    ADirective::ParseDirective(line);
    std::vector<std::string> tokens = StringUtils::Split(line, SPACE);
    if (tokens.size() != 2 && !this->ValidateExtension(tokens[0]))
        throw SyntaxErrorException("Invalid cgi directive - " + line);
    this->_extension = tokens[0];
    this->_binaryPath = tokens[1];
}

void CgiDirective::PrintDirective() const
{
    std::string msg = "extension: " + this->_extension + " binary path: " + this->_binaryPath;
    Logger::Debug("CgiDirective::PrintDirective", SUCCESS, msg);
}

void CgiDirective::FillDefaultValues()
{
    if (this->SetDefaultFromParent())
        return ;
    this->_extension = ".php";
    this->_binaryPath = "/usr/bin/php-cgi";
}

bool CgiDirective::SetDefaultFromParent()
{
    AContext *parent = this->GetContextUpToLevel(2);
    if (!parent)
        return false;
    MapDirectives directives = parent->GetDirectives();
    PairDirectives *parentCgiDirective = MapUtils<std::string, ADirective* >::SafeFindMap(directives, "cgi");
    if (directives.size() == 0)
        return false;
    CgiDirective *cgiDirective = dynamic_cast<CgiDirective *>(parentCgiDirective->second);
    if (!parentCgiDirective)
        return false;
    this->_extension = cgiDirective->GetExtension();
    this->_binaryPath = cgiDirective->GetBinaryPath();
    return true;
}

ReturnDirective::ReturnDirective(): _code("-1"), _path("")
{
}

ReturnDirective::ReturnDirective(ReturnDirective const & other)
{
    *this = other;
}

ReturnDirective::~ReturnDirective()
{
}

ReturnDirective& ReturnDirective::operator=(ReturnDirective const & other)
{
    if (this != &other)
    {
        this->_code = other._code;
        this->_path = other._path;
    }
    return (*this);
}

void ReturnDirective::SetCode(int code)
{
    this->_code = code;
}

void ReturnDirective::SetPath(std::string path)
{
    this->_path = path;
}

std::string ReturnDirective::GetCode() const
{
    return this->_code;
}

std::string ReturnDirective::GetPath() const
{
    return this->_path;
}

bool ReturnDirective::ValidateCode(std::string code) const
{
    int redirectStatusCodes[] = {301, 302, 303, 307, 308};
    return HTTPStatus::validateStatusCode(code, redirectStatusCodes, sizeof(redirectStatusCodes) / sizeof(int));
}

bool ReturnDirective::ValidatePath(std::string path) const
{
    if (path[0] != '/')
        return false;
    return true;
}

void ReturnDirective::PrintDirective() const
{
    std::string msg = "code: " + this->_code + " path: " + this->_path;
    Logger::Debug("ReturnDirective::PrintDirective", SUCCESS, msg);
}

void ReturnDirective::ParseDirective(std::string &line)
{

    ADirective::ParseDirective(line);
    std::vector<std::string> tokens = StringUtils::Split(line, SPACE);
    if (tokens.size() != 2 && !this->ValidateCode(tokens[0]) && !this->ValidatePath(tokens[1]))
        throw SyntaxErrorException("Invalid return directive - " + line);
    this->_code = tokens[0];
    this->_path = tokens[1];
}

void ReturnDirective::FillDefaultValues()
{
   this->SetDefaultFromParent();
}

bool ReturnDirective::SetDefaultFromParent()
{
    AContext *parent = this->GetContextUpToLevel(2);
    if (!parent)
        return false;
    MapDirectives directives = parent->GetDirectives();
    PairDirectives *parentReturnDirective = MapUtils<std::string, ADirective* >::SafeFindMap(directives, "return");
    if (parentReturnDirective == NULL)
        return false;
    ReturnDirective *returnDirective = dynamic_cast<ReturnDirective *>(parentReturnDirective->second);
    if (!returnDirective)
        return false;
    this->_code = returnDirective->GetCode();
    this->_path = returnDirective->GetPath();
    return true;
}