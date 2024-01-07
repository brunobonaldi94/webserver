#include "CGIRequestHandler.hpp"

CGIRequestHandler::CGIRequestHandler(RequestContent *RequestContent) : _requestContent(RequestContent)
{

}

CGIRequestHandler::~CGIRequestHandler()
{
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

void CGIRequestHandler::execute() {
    std::cout << "Executando o script!" << std::endl;
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
    this->env["REQUEST_METHOD"] = firstLineSplit[0];
    this->env["SERVER_PORT"] = serverConfig->GetPort();
    this->env["SERVER_PROTOCOL"] = firstLineSplit[2];

}
