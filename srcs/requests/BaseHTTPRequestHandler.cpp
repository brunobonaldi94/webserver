#include "BaseHTTPRequestHandler.hpp"


BaseHTTPRequestHandler::~BaseHTTPRequestHandler() {}

void BaseHTTPRequestHandler::sendResponse(int statusCode, std::string message) {
	this->headersBuffer << "HTTP/1.1 " << statusCode << " " << message << CRLF;
}

void BaseHTTPRequestHandler::endHeaders() {
	this->headersBuffer << CRLF;
}

void BaseHTTPRequestHandler::writeContent(const std::string content) {
	this->headersBuffer << content;
}

std::string BaseHTTPRequestHandler::GetPath() const {
	return this->path;
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

std::vector<std::string> BaseHTTPRequestHandler::SplitRequest(const char* request) {
	std::istringstream iss(request);
	std::cout << request << std::endl;
	std::vector<std::string> requestLines;
	std::string line;
	bool bodyStart = false;

	while (std::getline(iss, line)) {
			if (!line.empty() && line[line.size() - 1] == CR) {
				  if (line.size() == 1) {
						bodyStart = true;
					}
					line.erase(line.size() - 1);
			}
			if (bodyStart == true)
				this->body += line;
			else 
				requestLines.push_back(line);
	}
	return requestLines;
}

BaseHTTPRequestHandler::RequestMethodFunction BaseHTTPRequestHandler::parseRequest(const char *request)
{
	// log request message
	std::vector<std::string> requestLines = this->SplitRequest(request);
	std::vector<std::string> firstRequestLine = StringUtils::Split(requestLines[0], " ");

	if (firstRequestLine.size() == 0)
		return NULL;
	std::vector<std::string> versionNumber;
	if (firstRequestLine.size() >= 3)
	{
		this->setRequestLines(firstRequestLine);
		try
		{
			std::string baseVersion = "HTTP/";
			if (this->requestVersion.compare(0, baseVersion.size(), baseVersion) != 0)
				throw std::invalid_argument("");
			std::vector<std::string> baseVersionNumberVector = StringUtils::Split(this->requestVersion, "/");
			if (baseVersionNumberVector.size() != 2)
				throw std::runtime_error("");
			std::string baseVersionNumber = baseVersionNumberVector[1];
			versionNumber = StringUtils::Split(baseVersionNumber, ".");
			if (versionNumber.size() != 2 ||
				(versionNumber.size() == 2 && versionNumber[0] == ""))
				throw std::runtime_error("");
		}
		catch (const std::exception &e)
		{
			this->sendError("<h1>Bad Request</h1>", HTTPStatus::BAD_REQUEST);
			return NULL;
		}
		if (std::atoi(versionNumber[0].c_str()) != 1 ||
			std::atoi(versionNumber[1].c_str()) != 1)
		{
			this->sendError("<h1>HTTP Version Not Supported</h1>", HTTPStatus::HTTP_VERSION_NOT_SUPPORTED);
			return NULL;
		}
	}
	if (!(2 <= firstRequestLine.size() && firstRequestLine.size() <= 3))
	{
		this->sendError("<h1>Bad Request</h1>", HTTPStatus::BAD_REQUEST);
		return NULL;
	}

	this->requestMethod = firstRequestLine[0];
	this->path = firstRequestLine[1];
	if (firstRequestLine.size() == 2)
	{
		// missing close connection
		if (this->requestMethod != "GET")
		{
			this->sendError("<h1>Bad Request</h1>", HTTPStatus::BAD_REQUEST);
			return NULL;
		}
	}
	std::vector<std::string> methodsAllowed = this->getMethodsAllowed();
	if (firstRequestLine.size() >= 2 &&
		VectorUtils<std::string>::hasElement(methodsAllowed, this->requestMethod))
	{
		BaseHTTPRequestHandler::RequestMethodFunction method = this->getMethod(this->requestMethod);
		return method;
	}
	return NULL;

}

const std::string BaseHTTPRequestHandler::headersBufferToString() const {
	const std::string headersBufferStr = this->headersBuffer.str();
	return headersBufferStr;
}

std::string BaseHTTPRequestHandler::getContent(const std::string path, bool& foundContent) {
	std::string content  = "";
	char const *file = path.c_str();
	std::ifstream f(file);
	foundContent = false;
	if (f.good())
	{
		std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
		foundContent = true;
		content = str;
	}
  f.close();
  return content;
}

BaseHTTPRequestHandler::RequestMethodFunction BaseHTTPRequestHandler::getMethod(const std::string& method) {
	if (method == "GET")
		return &BaseHTTPRequestHandler::doGET;
	else if (method == "POST")
		return &BaseHTTPRequestHandler::doPOST;
	else if (method == "DELETE")
		return &BaseHTTPRequestHandler::doDELETE;
	return NULL;
}

std::vector<std::string> BaseHTTPRequestHandler::getMethodsAllowed() const {
	std::vector<std::string> methodsAllowed;
	return StringUtils::Split("GET PUT POST DELETE PATCH HEAD OPTIONS TRACE CONNECT", " ");
}