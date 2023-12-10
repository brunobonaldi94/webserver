#include "RequestHandler.hpp"

RequestHandler::RequestHandler(ADirectoryHandler *directoryHandler)
: BaseHTTPRequestHandler(directoryHandler)
{

}

RequestHandler::~RequestHandler()
{
}

RequestHandler::RequestHandler(const RequestHandler& other): BaseHTTPRequestHandler(other._directoryHandler)
{
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
    if (this->getContentNotFound())
        return this->sendNotFoundError();
    this->sendResponse(HTTPStatus::OK.code, HTTPStatus::OK.description);
    this->writeDefaultResponse(content);
}

void RequestHandler::doPOST() {
    this->doGET();
}

void RequestHandler::doDELETE() {
    std::string path = this->GetPath();
    if (path == "/api/files/file1" || path == "/api/files/file2")
        return this->sendJsonResponse("{\"message\": \"File deleted\"}");
}


void RequestHandler::clearRequestContent(int clientSocket)
{
    if (MapUtils<int, RequestContent>::SafeFindMap(this->clientSocketRequestContentMap, clientSocket))
        this->clientSocketRequestContentMap[clientSocket].clear();
}