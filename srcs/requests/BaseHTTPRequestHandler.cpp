#include "BaseHTTPRequestHandler.hpp"


BaseHTTPRequestHandler::BaseHTTPRequestHandler(ADirectoryHandler *directoryHandler) :_directoryHandler(directoryHandler),  allowDirectoryListing(false), contentNotFound(false)
{

}

BaseHTTPRequestHandler::~BaseHTTPRequestHandler()
{
}

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

ServerConfig *BaseHTTPRequestHandler::getServerConfig() const {
	return this->serverConfig;
}

void BaseHTTPRequestHandler::setServerConfig(ServerConfig *serverConfig) {
	this->serverConfig = serverConfig;
}

void BaseHTTPRequestHandler::sendError(const std::string& content, const StatusCode& status) {
  this->sendResponse(status.code, status.description);
	this->writeDefaultResponse(content);
}

void BaseHTTPRequestHandler::sendNotFoundError()
{
	std::string page = this->serverConfig->GetErrorPage();
	int code = this->serverConfig->GetErrorPageCode();
	std::string content = this->readContent(page);
	if (content.empty())
		this->sendError("<h1>Not Found</h1>", HTTPStatus::NOT_FOUND);
	else
	{
		StatusCode status(code, HTTPStatus::NOT_FOUND.description, HTTPStatus::NOT_FOUND.details);
		this->sendError(content, HTTPStatus::NOT_FOUND);
	}
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

void BaseHTTPRequestHandler::parseHeaders(std::vector<std::string> headers)
{
	for (std::vector<std::string>::iterator it = headers.begin(); it != headers.end(); it++)
	{
		std::vector<std::string> keyValue = StringUtils::SplitAtFirstDelimiter(*it, ":");
		if (keyValue.size() < 2)
			continue;
		std::string key = keyValue[0];
		std::string value = keyValue[1];
		this->headers.setHeader(key, value);
	}
}

std::vector<std::string> BaseHTTPRequestHandler::SplitRequest(const char* request) {
	std::istringstream iss(request);
	std::cout << request << std::endl;
	std::vector<std::string> requestLines;
	std::string line;
	bool bodyStart = false;

	while (std::getline(iss, line))
	{
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
	this->parseHeaders(requestLines);
	return requestLines;
}

bool BaseHTTPRequestHandler::checkBodyLimit()
{
	std::string contentLength = this->headers.getHeader("Content-Length");
	if (contentLength.empty())
		return true;
	ssize_t contentLengthNbr = std::atoll(contentLength.c_str());
	ssize_t maxContentLength = this->serverConfig->GetMaxBodySize();
	if (contentLengthNbr < 0 || contentLengthNbr > maxContentLength)
		return false;
	return true;
}

bool BaseHTTPRequestHandler::validateServerName()
{
	std::string host = this->headers.getHeader("Host");
	if (host.empty())
		return false;
	std::vector<std::string> hostParts = StringUtils::Split(host, ":");
	if (hostParts.size() < 2)
		return false;
	std::string serverName = hostParts[0];
	std::string port = hostParts[1];
	std::string portServer = this->serverConfig->GetPort();
	if (portServer != port)
		return false;
	std::vector<std::string> serverNames = this->serverConfig->GetServerNames();
	for (std::vector<std::string>::iterator it = serverNames.begin(); it != serverNames.end(); it++)
	{
		if (*it == serverName)
			return true;
	}
	return false;
}

bool BaseHTTPRequestHandler::checkRedirect()
{
	std::string path = this->path;
	LocationConfig *location = this->serverConfig->GetLocationConfig(path);
	if (location == NULL || location->ShouldRedirect() == false)
		return false;
	StatusCode status = location->ReturnRedirectStatus();
	this->sendResponse(status.code, status.description);
	std::string locationStr = location->GetReturnPath();
	this->sendHeader("Location", locationStr);
	this->endHeaders();
	this->path = locationStr;
	return true;
}

BaseHTTPRequestHandler::RequestMethodFunction BaseHTTPRequestHandler::parseRequest(const char *request)
{
	try
	{
		std::vector<std::string> firstRequestLine;
		std::vector<std::string> versionNumber;
		std::vector<std::string> requestLines = this->SplitRequest(request);
		if (!this->validateServerName())
		{
			this->sendError("<h1>Bad Request</h1>", HTTPStatus::BAD_REQUEST);
			return NULL;
		}
		if (!this->checkBodyLimit())
		{
			this->sendError("<h1>Request Entity Too Large</h1>", HTTPStatus::CONTENT_TOO_LARGE);
			return NULL;
		}
		firstRequestLine = StringUtils::Split(requestLines[0], " ");
		if (firstRequestLine.size() < 2 || (firstRequestLine.size() == 2 && firstRequestLine[0] != "GET"))
		{
			this->sendError("<h1>Bad Request</h1>", HTTPStatus::BAD_REQUEST);
			return NULL;
		}
		this->setRequestLines(firstRequestLine);
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
		if (std::atoi(versionNumber[0].c_str()) != 1 ||
			std::atoi(versionNumber[1].c_str()) != 1)
		{
			this->sendError("<h1>HTTP Version Not Supported</h1>", HTTPStatus::HTTP_VERSION_NOT_SUPPORTED);
			return NULL;
		}
		this->requestMethod = firstRequestLine[0];
		this->path = firstRequestLine[1];
		this->checkRedirect();
		if (isDirectoryListingAllowed(this->path) && this->requestMethod == "GET")
			return this->getMethod("GET");
		std::vector<std::string> methodsAllowed = this->getMethodsAllowed();
		if (methodsAllowed.size() == 0)
		{
			this->sendNotFoundError();
			return NULL;
		}
		if (!VectorUtils<std::string>::hasElement(methodsAllowed, this->requestMethod))
		{
			this->sendError("<h1>Method Not Allowed</h1>", HTTPStatus::METHOD_NOT_ALLOWED);
			return NULL;
		}
		BaseHTTPRequestHandler::RequestMethodFunction method = this->getMethod(this->requestMethod);
		return method;
	}
	catch (const std::exception &e)
	{
		this->sendError("<h1>Bad Request</h1>", HTTPStatus::BAD_REQUEST);
		return NULL;
	}
}

const std::string BaseHTTPRequestHandler::headersBufferToString() const {
	const std::string headersBufferStr = this->headersBuffer.str();
	return headersBufferStr;
}

std::string BaseHTTPRequestHandler::readContent(const std::string path) 
{
	std::string content("");
	std::ifstream f(path.c_str());
	if (f.good())
	{
		std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
		content = str;
	}
	else
		this->contentNotFound = true; 
  f.close();
  return content;
}

std::string BaseHTTPRequestHandler::createDirectoryListing(LocationConfig *location, std::string path)
{
	struct stat sb;

	std::string content("<html><head><title>Index of " + path + "</title></head><body><h1>Index of " + path + "</h1><pre>");
	content += "<table style=\"width: 25%;text-align: justify;\"><thead><tr><th>Name</th><th>Size</th><th>Last Modified</th></tr></thead>";
	content += "<tbody>";

	std::string port = this->serverConfig->GetPort();
	std::string fullPath = location->GetRootPath() + path;
	bool directoryExists = false;
	std::vector<std::string> files = this->_directoryHandler->getFilesInDirectory(fullPath, directoryExists);
	if (directoryExists == false)
	{
		this->sendError("<h1>Forbidden</h1>", HTTPStatus::FORBIDDEN);
		return "";
	}
	for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); it++)
	{
		std::string fileName = *it;
		std::string filePath = fullPath + "/" + fileName;
		if (this->_directoryHandler->getFileStat(filePath.c_str(), &sb) == false)
			continue;
		if (S_ISDIR(sb.st_mode))
			fileName += "/";
		content +=  "<tr><td><a href=\"http://localhost:" + port + path + "/" + fileName + "\">" + fileName + "</a><td>";
		content += StringUtils::ConvertNumberToString(sb.st_size) + "</td>";
		content += "<td>" + this->_directoryHandler->getLastModified(sb) + "</td></tr>";
	}
	content += "</tbody></pre><hr></table></body></html>";
	return content;
}


bool BaseHTTPRequestHandler::isDirectoryListingAllowed(std::string path)
{
	std::vector<LocationConfig *> locations = this->serverConfig->GetLocationConfigs();
	for (std::vector<LocationConfig *>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		std::string locationPath = (*it)->GetPath();
		if (locationPath == "/" && (*it)->GetAutoIndex() == true && path == "/")
			return true;
		size_t countSlashs = StringUtils::CountChar(locationPath, '/');
		size_t findNthSlashOcurrence = StringUtils::FindNthOccurrence(path, '/', countSlashs + 1);
		std::string pathToCompare = path.substr(0, findNthSlashOcurrence);
		if (pathToCompare == locationPath && locationPath != path && (*it)->GetAutoIndex() == true)
		{
			std::string directory = (*it)->GetRootPath() + locationPath;
			this->allowDirectoryListing = this->_directoryHandler->isInDirectory(path, directory);
			this->directoryListingPath = (*it)->GetRootPath() + path;
			return this->allowDirectoryListing;
		}
	}
	return false;
}

std::string BaseHTTPRequestHandler::getContent(const std::string path)
{
	std::string content("");
	LocationConfig *location = this->serverConfig->GetLocationConfig(path);
	if (this->allowDirectoryListing)
	{
		this->allowDirectoryListing = false;
		return this->readContent(this->directoryListingPath);
	}
	if (location == NULL)
		return content;
	if (location->GetIndexFileNotFound() && location->GetAutoIndex())
		return this->createDirectoryListing(location, path);
	std::vector <std::string> indexFiles = location->GetFilesFullPath();
	for (std::vector<std::string>::const_iterator it = indexFiles.begin(); it != indexFiles.end(); it++)
	{
		content = this->readContent(*it);
		if (!content.empty())
			return content;
	}
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

std::vector<std::string> BaseHTTPRequestHandler::getMethodsAllowedForApi() const {
	std::vector<std::string> methodsAllowed;

	std::vector<std::string> api = StringUtils::Split(path, "/");
	if (api.size() < 3 || (api[1] != "api" && api[2] != "files"))
		return methodsAllowed;
	if (api.size() == 3)
	{
		methodsAllowed.push_back("GET");
		return methodsAllowed;
	}
	else
	{
		methodsAllowed.push_back("DELETE");
		return methodsAllowed;
	}
	return methodsAllowed;
}

std::vector<std::string> BaseHTTPRequestHandler::getMethodsAllowed() const {
	std::vector<std::string> methodsAllowed = this->getMethodsAllowedForApi();
	if (methodsAllowed.size() > 0)
		return methodsAllowed;
	LocationConfig *location = this->serverConfig->GetLocationConfig(this->path);
	if (location == NULL)
		return methodsAllowed;
	return location->GetAllowedMethods();
}

void BaseHTTPRequestHandler::writeDefaultResponse(std::string content, std::string content_type) 
{
	this->sendHeader("Cache-Control", "no-cache, private");
	this->sendHeader("Content-Type", content_type);
	this->sendHeader("Content-Length", content.size());
	this->endHeaders();
	this->writeContent(content);
}

bool BaseHTTPRequestHandler::getContentNotFound() const
{
	return this->contentNotFound;
}