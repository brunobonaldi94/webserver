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

void RequestHandler::sendJsonResponse(std::string json) {
    this->sendResponse(HTTPStatus::OK.code, HTTPStatus::OK.description);
	this->sendHeader("Cache-Control", "no-cache, private");
	this->sendHeader("Content-Type", "application/json");
	this->sendHeader("Content-Length", json.size());
	this->endHeaders();
	this->writeContent(json);
}


void RequestHandler::doGET() {
    std::string path = this->GetPath();
    Logger::PrintMessage(INFO,"Path: " + path);
    if (path == "/api/files")
        return this->sendJsonResponse("{\"files\": [\"file1\", \"file2\"]}");
    std::string content = this->getContent(path);
    if (content.empty())
        return this->sendNotFoundError();
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
    std::string path = this->GetPath();
    if (path == "/api/files/file1" || path == "/api/files/file2")
        return this->sendJsonResponse("{\"message\": \"File deleted\"}");
}

