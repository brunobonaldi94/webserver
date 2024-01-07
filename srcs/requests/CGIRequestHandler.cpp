#include "CGIRequestHandler.hpp"

CGIRequestHandler::CGIRequestHandler(ADirectoryHandler *directoryHandler)
: BaseHTTPRequestHandler(directoryHandler)
{

}

CGIRequestHandler::~CGIRequestHandler()
{
}

CGIRequestHandler::CGIRequestHandler(const CGIRequestHandler& other): BaseHTTPRequestHandler(other._directoryHandler)
{
    *this = other;
}

CGIRequestHandler& CGIRequestHandler::operator=(const CGIRequestHandler& other) {
    if (this != &other)
    {
        
    }
    return *this;
}

void CGIRequestHandler::doGET() {
    std::string content = "Hello CGI!";
    //std::string path = this->GetPath();
    this->sendResponse(HTTPStatus::OK.code, HTTPStatus::OK.description);
    this->writeDefaultResponse(content);
}

void CGIRequestHandler::doPOST() {
    this->doGET();
}

void CGIRequestHandler::execute() {
    std::cout << "Executando o script!" << std::endl;
}
