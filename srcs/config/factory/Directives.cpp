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

int ListenDirective::GetPort() const
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

bool ListenDirective::ParseDirective(std::string &line)
{
    try 
    {
        ADirective::ParseDirective(line);
        std::vector<std::string> hostPort = StringUtils::Split(line, ":");
        if (hostPort.size() == 0)
            return false;
        if (hostPort.size() == 1 && this->ValidatePort(hostPort[0]))
        {
            this->_host = "0.0.0.0";
            this->_port = std::atoi(hostPort[0].c_str());
            return true;
        }
        else if (hostPort.size() == 1 && this->ValidateHost(hostPort[0]))
        {
            this->_host = hostPort[0];
            this->_port = 80;
            return true;
        }
        else if (hostPort.size() == 2 && this->ValidateHost(hostPort[0]) && this->ValidatePort(hostPort[1]))
        {
            this->_host = hostPort[0];
            this->_port = std::atoi(hostPort[1].c_str());
            return true;
        }
        return false;
    } catch (std::exception &e)
    {
        Logger::debug(e.what(), ERROR, "ListenDirective::ParseDirective");
        return false;
    }
}

void ListenDirective::PrintDirective() const
{
    std::string port = StringUtils::ConvertNumberToString(this->_port);
    std::string msg = "host: " + this->_host + " port: " + port;
    Logger::debug(msg, SUCCESS, "ListenDirective::PrintDirective");
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

bool ServerNameDirective::ParseDirective(std::string &line)
{
    ADirective::ParseDirective(line);
    std::vector<std::string> tokens = StringUtils::Split(line, SPACE);
    if (tokens.size() == 0)
        return false;
    for (size_t i = 0; i < tokens.size(); i++)
    {
        this->_names.push_back(tokens[i]);
    }
    return true;
}

void ServerNameDirective::PrintDirective() const
{
    std::string names;
    for (size_t i = 0; i < this->_names.size() - 1; i++)
    {
        names += this->_names[i] + " ";
    }
    names += this->_names[this->_names.size() - 1];
    Logger::debug(names, SUCCESS, "ServerNameDirective::PrintDirective");
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

int ErrorPageDirective::GetCode() const
{
    return this->_code;
}

std::string ErrorPageDirective::GetPath() const
{
    return this->_path;
}

bool ErrorPageDirective::ValidateCode(std::string code) const
{
    for (size_t i = 0; i < code.size(); i++)
    {
        if (!isdigit(code[i]))
            return false;
    }
    int codeNumber = std::atoi(code.c_str());
    int httpStatusCodes[] = {100, 101, 102, 103, 200, 201, 202, 203, 204, 205,
                            206, 207, 208, 226, 300, 301, 302, 303, 304, 305,
                            306, 307, 308, 400, 401, 402, 403, 404, 405, 406,
                            407, 408, 409, 410, 411, 412, 413, 414, 415, 416,
                            417, 418, 421, 422, 423, 424, 425, 426, 428, 429,
                            431, 451, 500, 501, 502, 503, 504, 505, 506, 507,
                            508, 510, 511};
    for (size_t i = 0; i < sizeof(httpStatusCodes) / sizeof(int); i++)
    {
        if (codeNumber == httpStatusCodes[i])
            return true;
    }
    return false;
}


bool ErrorPageDirective::ParseDirective(std::string &line)
{
    ADirective::ParseDirective(line);
    std::vector<std::string> tokens = StringUtils::Split(line, SPACE);
    if (tokens.size() < 2)
        return false;
    if (!this->ValidateCode(tokens[0]))
        return false;
    this->_code = std::atoi(tokens[0].c_str());
    this->_path = tokens[1];
    return true;
}

void ErrorPageDirective::PrintDirective() const
{
    std::string code = StringUtils::ConvertNumberToString(this->GetCode());
    std::string msg = "code: " + code + " path: " + this->_path;
    Logger::debug(msg, SUCCESS, "ErrorPageDirective::PrintDirective");
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
    if (std::toupper(size[size.size() -1]) != 'M')
        return false;
    int sizeNumber = std::atoi(size.c_str());
    if (sizeNumber < 0)
        return false;
    return true;
}

bool ClientMaxBodySizeDirective::ParseDirective(std::string &line)
{
    ADirective::ParseDirective(line);
    std::vector<std::string> tokens = StringUtils::Split(line, SPACE);
    if (tokens.size() == 0 || tokens.size() > 1)
        return false;
    if (!this->ValidateSize(tokens[0]))
        return false;
    this->_size = tokens[0];
    return true;
}

void ClientMaxBodySizeDirective::PrintDirective() const
{
    std::string msg = "size: " + this->_size;
    Logger::debug(msg, SUCCESS, "ClientMaxBodySizeDirective::PrintDirective");
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
    {
        this->_index = other._index;
    }
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

bool IndexDirective::ParseDirective(std::string &line)
{
    ADirective::ParseDirective(line);
    std::vector<std::string> tokens = StringUtils::Split(line, SPACE);
    if (tokens.size() == 0)
        return false;
    for (size_t i = 0; i < tokens.size(); i++)
    {
        this->_index.push_back(tokens[i]);
    }
    return true;
}

void IndexDirective::PrintDirective() const
{
    std::string index;
    for (size_t i = 0; i < this->_index.size() - 1; i++)
    {
        index += this->_index[i] + " ";
    }
    index += this->_index[this->_index.size() - 1];
    Logger::debug(index, SUCCESS, "IndexDirective::PrintDirective");
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
    {
        this->_path = other._path;
    }
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

bool RootDirective::ParseDirective(std::string &line)
{
    ADirective::ParseDirective(line);
    std::vector<std::string> tokens = StringUtils::Split(line, SPACE);
    if (tokens.size() != 1)
        return false;
    this->_path = tokens[0];
    return true;
}

void RootDirective::PrintDirective() const
{
    std::string msg = "path: " + this->_path;
    Logger::debug(msg, SUCCESS, "RootDirective::PrintDirective");
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
    {
        this->_autoIndex = other._autoIndex;
    }
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

bool AutoIndexDirective::ParseDirective(std::string &line)
{
    ADirective::ParseDirective(line);
    std::vector<std::string> tokens = StringUtils::Split(line, SPACE);
    if (tokens.size() != 1)
        return false;
    if (tokens[0] == "on")
        this->_autoIndex = true;
    else if (tokens[0] == "off")
        this->_autoIndex = false;
    else
        return false;
    return true;
}

void AutoIndexDirective::PrintDirective() const
{
    std::string msg = "autoindex: " + std::string(this->_autoIndex ? "on" : "off");
    Logger::debug(msg, SUCCESS, "AutoIndexDirective::PrintDirective");
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
    {
        this->_methods = other._methods;
    }
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

bool LimitExceptDirective::ParseDirective(std::string &line)
{
    ADirective::ParseDirective(line);
    std::vector<std::string> tokens = StringUtils::Split(line, SPACE);
    if (tokens.size() == 0)
        return false;
    for (size_t i = 0; i < tokens.size(); i++)
    {
        this->_methods.push_back(tokens[i]);
    }
    return true;
}

void LimitExceptDirective::PrintDirective() const
{
    std::string methods;
    for (size_t i = 0; i < this->_methods.size() - 1; i++)
    {
        methods += this->_methods[i] + " ";
    }
    methods += this->_methods[this->_methods.size() - 1];
    Logger::debug(methods, SUCCESS, "LimitExceptDirective::PrintDirective");
}