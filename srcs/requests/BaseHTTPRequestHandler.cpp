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

void BaseHTTPRequestHandler::sendError(const std::string& content, const StatusCode& status) {
    this->sendResponse(status.code, status.description);
	this->sendHeader("Cache-Control", "no-cache, private");
	this->sendHeader("Content-Type", "text/html");
	this->sendHeader("Content-Length", content.size());
	this->endHeaders();
	this->writeContent(content);
}

void BaseHTTPRequestHandler::clearHeadersBuffers() {
	this->headersBuffer.str("");
	this->headersBuffer.clear();
}

void BaseHTTPRequestHandler::setRequestLines(const std::vector<std::string> requestLines) {
	this->requestMethod = requestLines[0];
	this->path = requestLines[1];
	this->requestVersion = requestLines[2];
}

bool BaseHTTPRequestHandler::parseRequest(const char* request) {
	
	std::istringstream iss(request);
	//log request message
	std::cout << request << std::endl;
	std::vector<std::string> requestLines((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	this->setRequestLines(requestLines);

	if (requestLines.size() == 0)
		return false;
	std::vector<std::string> versionNumber;
	if (requestLines.size() >= 3) {
		try {
			std::string baseVersion = "HTTP/";
			if (this->requestVersion.compare(0, baseVersion.size(), baseVersion) != 0)
				throw std::invalid_argument("");
			std::vector<std::string> baseVersionNumberVector = StringUtils::Split(this->requestVersion, "/");
			if (baseVersionNumberVector.size() != 2)
				throw std::runtime_error("");
			std::string baseVersionNumber = baseVersionNumberVector[1];
			versionNumber = StringUtils::Split(baseVersionNumber, ".");
			if (versionNumber.size() != 2  || 
				(versionNumber.size() == 2 && versionNumber[0] == ""))
				throw std::runtime_error("");
		}
		catch(const std::exception& e) {
			this->sendError("<h1>Bad Request</h1>", HTTPStatus::BAD_REQUEST);
			return false;
		}
		if (std::atoi(versionNumber[0].c_str()) != 1 || 
			std::atoi(versionNumber[1].c_str()) != 1) {
			this->sendError("<h1>HTTP Version Not Supported</h1>", HTTPStatus::HTTP_VERSION_NOT_SUPPORTED);
			return false;
		}
		
	}

	if (this->path != "/") {
		this->sendError("<h1>Not Found</h1>", HTTPStatus::NOT_FOUND);
		return false;
	}
	if (requestLines.size() >= 3 && this->requestMethod == "GET")
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

