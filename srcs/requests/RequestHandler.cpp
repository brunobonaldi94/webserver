#include "RequestHandler.hpp"

RequestHandler::RequestHandler(){

}

RequestHandler::RequestHandler(const RequestHandler& other) {
    (void) other;
}

RequestHandler& RequestHandler::operator=(const RequestHandler& other) {
    if (this != &other) {
        
    }
    return *this;
}

void RequestHandler::doGET() {
    std::string path = this->GetPath();
    if (path == "/")
        path = "/index.html";
    bool foundContent = false;
    std::string content = this->getContent("wwwroot/" + path, foundContent);
    if (foundContent == false)
        return this->sendError("<h1>Not Found</h1>", HTTPStatus::NOT_FOUND);
    this->sendResponse(HTTPStatus::OK.code, HTTPStatus::OK.description);
	this->sendHeader("Cache-Control", "no-cache, private");
	this->sendHeader("Content-Type", "text/html");
	this->sendHeader("Content-Length", content.size());
	this->endHeaders();
	this->writeContent(content);
}

void RequestHandler::doPOST() {
    this->doGET();
}

void RequestHandler::doDELETE() {

}

