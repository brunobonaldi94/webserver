#include "BaseHTTPRequestHandler.hpp"


void BaseHTTPRequestHandler::sendResponse(int statusCode, std::string message) {
	(void) message;
	this->headersBuffer << "HTTP/1.1 " << statusCode << " OK" << std::endl;
}

void BaseHTTPRequestHandler::endHeaders() {
	this->headersBuffer << std::endl;
}

void BaseHTTPRequestHandler::writeContent(const std::string content) {
	this->headersBuffer << content;
}

void BaseHTTPRequestHandler::sendError() {

}

const std::string BaseHTTPRequestHandler::headersBufferToString() const {
	const std::string headersBufferStr = this->headersBuffer.str();
	return headersBufferStr;
}
