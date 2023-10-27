#include "BaseHTTPRequestHandler.hpp"


void BaseHTTPRequestHandler::sendResponse(int statusCode, std::string message) {
	this->headersBuffer << "HTTP/1.1 " << statusCode << " " << message << std::endl;
}

void BaseHTTPRequestHandler::endHeaders() {
	this->headersBuffer << std::endl;
}

void BaseHTTPRequestHandler::writeContent(const std::string content) {
	this->headersBuffer << content;
}

void BaseHTTPRequestHandler::sendError() {

}

bool BaseHTTPRequestHandler::parseRequest(const char* request) const {
	std::istringstream iss(request);
	std::cout << request << std::endl;
	std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

	if (parsed.size() >= 3 && parsed[0] == "GET")
		return true;
	return false;
}

const std::string BaseHTTPRequestHandler::headersBufferToString() const {
	const std::string headersBufferStr = this->headersBuffer.str();
	return headersBufferStr;
}

std::string BaseHTTPRequestHandler::getContent(const std::string path) {
    std::string content  = "";
	char const *file = path.c_str();
	std::ifstream f(file);

	if (f.good())
	{
		std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
		return str;
	}
    f.close();
    return content;
}

