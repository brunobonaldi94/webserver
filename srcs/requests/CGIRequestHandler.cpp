#include "CGIRequestHandler.hpp"

CGIRequestHandler::CGIRequestHandler(RequestContent *RequestContent) : _requestContent(RequestContent), envp(NULL)
{
    this->setEnv();
    this->createEnvp();
}

CGIRequestHandler::~CGIRequestHandler()
{
    if (!this->envp)
        return;
    for(size_t index; index < this->env.size(); index++)
        delete[] this->envp[index];
    delete[] this->envp;
}

CGIRequestHandler::CGIRequestHandler(const CGIRequestHandler& other)
{
    *this = other;
}

CGIRequestHandler& CGIRequestHandler::operator=(const CGIRequestHandler& other) {
    if (this != &other)
    {
        this->_requestContent = other._requestContent;
    }
    return *this;
}

std::string CGIRequestHandler::execute() {
    std::cout << "Executando o script!" << std::endl;
    return "";
}

void CGIRequestHandler::setEnv()
{
    std::cout << "Setando o env!" << std::endl;
    ServerConfig *serverConfig = this->_requestContent->getServerConfig();
    std::vector<std::string> firstLineSplit = StringUtils::Split(this->_requestContent->getFirstRequestLine(), " ");
    std::string authorization = this->_requestContent->getHeader("Authorization");
    this->env["AUTH_TYPE"] = authorization.empty() ? authorization : StringUtils::Split(authorization, " ")[0];
    this->env["CONTENT_LENGTH"] = this->_requestContent->getHeader("Content-Length");
    this->env["CONTENT_TYPE"] = this->_requestContent->getHeader("Content-Type");
    this->env["GATEWAY_INTERFACE"] = "CGI/1.1";
    this->env["HTTP_HOST"] = this->_requestContent->getHeader("Host");
    this->env["SERVER_PORT"] = serverConfig->GetPort();
    this->env["SERVER_PROTOCOL"] = firstLineSplit[2];
    this->env["REQUEST_METHOD"] = firstLineSplit[0];
    this->env["PATH_INFO"] = firstLineSplit[1].replace(0, std::string(CGI_PATH).length(), "");
    this->env["PATH_TRANSLATED"] = "wwwroot" + this->env["PATH_INFO"];
    this->env["SCRIPT_NAME"] = firstLineSplit[1].substr(firstLineSplit[1].find_last_of("/") + 1);
    this->env["QUERY_STRING"] = this->_requestContent->getQueryString();
}

char ** CGIRequestHandler::createEnvp()
{
    this->envp = new char *[this->env.size() + 1];
    size_t envp_index = 0;
    for (std::map<std::string,std::string>::iterator it = this->env.begin(); it != this->env.end(); it++)
    {
        std::string key = it->first;
        std::string value = it->second;
        std::string env = key + "=" + value;
        this->envp[envp_index] = strdup(env.c_str());
        envp_index++;
    }
    this->envp[envp_index] = NULL;
    return this->envp;
}