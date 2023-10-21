#include "BaseHTTPRequestHandler.hpp"

void BaseHTTPRequestHandler::sendResponse(int statusCode, std::string message) {

}

void BaseHTTPRequestHandler::sendHeader(std::string key, std::string value) {
	this->headersBuffer <<  key << ": " << value << std::endl;
}

void BaseHTTPRequestHandler::sendError() {

}

const std::string BaseHTTPRequestHandler::headersBufferToString() const {
	return this->headersBuffer.str();
}


