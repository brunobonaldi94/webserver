#include "BaseHTTPRequestHandler.hpp"


BaseHTTPRequestHandler::BaseHTTPRequestHandler(ADirectoryHandler *directoryHandler): contentLength(0), _directoryHandler(directoryHandler), allowDirectoryListing(false), contentNotFound(false), isCgiRootPath(false), shouldExecuteCgi(false), currentServerConfig(NULL), currentRequestContent(NULL), mimeTypes(MimeTypes())
{

}

BaseHTTPRequestHandler::BaseHTTPRequestHandler(const BaseHTTPRequestHandler& other)
{
	*this = other;
}
BaseHTTPRequestHandler &BaseHTTPRequestHandler::operator=(const BaseHTTPRequestHandler& other)
{
	if (this != &other)
	{
		this->contentLength = other.contentLength;
		this->_directoryHandler = other._directoryHandler;
		this->allowDirectoryListing = other.allowDirectoryListing;
		this->contentNotFound = other.contentNotFound;
		this->currentServerConfig = other.currentServerConfig;
		this->currentRequestContent = other.currentRequestContent;
		this->directoryListingPath = other.directoryListingPath;
		this->fileName = other.fileName;
		this->fullResourcePath = other.fullResourcePath;
		this->headersBuffer << other.headersBuffer.str();
		this->path = other.path;
		this->requestMethod = other.requestMethod;
		this->requestVersion = other.requestVersion;
		this->isCgiRootPath = other.isCgiRootPath;
		this->shouldExecuteCgi = other.shouldExecuteCgi;
	}
	return *this;
}

BaseHTTPRequestHandler::~BaseHTTPRequestHandler()
{
	delete this->_directoryHandler;
}

void BaseHTTPRequestHandler::sendResponse(int statusCode, std::string message)
{
	this->headersBuffer << "HTTP/1.1 " << statusCode << " " << message << CRLF;
}

void BaseHTTPRequestHandler::endHeaders() {
	this->headersBuffer << CRLF;
}

void BaseHTTPRequestHandler::writeContent(const std::string content)
{
	this->headersBuffer << content;
}

std::string BaseHTTPRequestHandler::GetPath() const
{
	return this->path;
}

void BaseHTTPRequestHandler::sendError(const std::string& content, const StatusCode& status)
{
  this->sendResponse(status.code, status.description);
	this->writeDefaultResponse(content);
	this->currentRequestContent->setHasErrorInRequest(true);
}

void BaseHTTPRequestHandler::sendNotFoundError()
{
	std::string page = this->currentServerConfig->GetErrorPage();
	int code = this->currentServerConfig->GetErrorPageCode();
	std::string content = this->readContent(page);
	if (content.empty())
		this->sendError("<h1>Not Found</h1>", HTTPStatus::NOT_FOUND);
	else
	{
		StatusCode status(code, HTTPStatus::NOT_FOUND.description, HTTPStatus::NOT_FOUND.details);
		this->sendError(content, HTTPStatus::NOT_FOUND);
	}
}

void BaseHTTPRequestHandler::clearHeadersBuffers()
{
	this->headersBuffer.str("");
	this->headersBuffer.clear();
}

bool BaseHTTPRequestHandler::shouldClearRequestContent(int clientSocket)
{
	if (this->currentRequestContent->hasParsedAllRequest() || this->currentRequestContent->getHasErrorInRequest())
	{
		this->clearHeadersBuffers();
		this->clearRequestContent(clientSocket);
		this->directoryListingPath.clear();
		this->allowDirectoryListing = false;
		this->contentLength = 0;
		this->contentNotFound = false;
		this->fileName.clear();
		this->scriptName.clear();
		this->fullResourcePath.clear();
		this->isCgiRootPath = false;
		this->shouldExecuteCgi = false;
		this->mimeType.clear();
		this->pathAdder.clear();
		this->rawRequest.clear();
		this->multipartDatum.clear();
		return true;
	}
	return false;
}

bool BaseHTTPRequestHandler::hasParsedAllRequestContent()
{
	return this->currentRequestContent->hasParsedAllRequest();
}

void BaseHTTPRequestHandler::setRequestLines(const std::vector<std::string> requestLines)
{
	this->requestMethod = requestLines[0];
	this->path = this->getPath(requestLines[1]);
	this->requestVersion = requestLines[2];
}

bool BaseHTTPRequestHandler::isValidFirstRequestHeaderLine(std::string firstRequestHeaderLine)
{
	std::vector<std::string> versionNumber;

	std::vector<std::string> firstRequestLine = StringUtils::Split(firstRequestHeaderLine, " ");
	if (firstRequestLine.size() < 3)
	{
			this->sendError("<h1>Bad Request</h1>", HTTPStatus::BAD_REQUEST);
			return false;
	}
	this->currentRequestContent->setFirstRequestLine(firstRequestHeaderLine);
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
		return false;
	}
	return true;
}

void BaseHTTPRequestHandler::parseHeaders(std::vector<std::string> &requestLines)
{
	bool hasParserAllHeader = this->currentRequestContent->getHeadersFullyRead();
	if (hasParserAllHeader)
		return;
	if (requestLines.size() == 0 && !hasParserAllHeader && this->rawRequest.size() < BUFFER_SIZE - 1)
	{
		this->sendError("<h1>Bad Request</h1>", HTTPStatus::BAD_REQUEST);
		return;
	}
	std::string contentLengthStr;
	for (std::vector<std::string>::iterator it = requestLines.begin(); it != requestLines.end(); it++)
	{
		std::string line = *it;
		if (line.empty())
			break;
		this->currentRequestContent->parseHeader(line);
	}
	contentLengthStr = this->currentRequestContent->getHeader("Content-Length");
	if (!contentLengthStr.empty())
		this->contentLength = std::atoll(contentLengthStr.c_str());
	this->currentRequestContent->isMultiPartFormData();
	this->currentRequestContent->isChunkedBody();
	this->currentRequestContent->setHeadersFullyRead(true);
}

bool BaseHTTPRequestHandler::shouldContinueParsing(std::string requestBodyLines)
{
	std::string &bodyUnparsed = this->currentRequestContent->getBodyObject().getBodyUnparsed();
	if (this->contentLength == 0 && this->currentRequestContent->isChunkedBody())
	{
		size_t posEnd = requestBodyLines.find("0\r\n\r\n");
		if (posEnd == std::string::npos)
			return false;
		StringUtils::AddToString(bodyUnparsed, requestBodyLines);
		return true;
	}
	else if (this->contentLength == 0)
		return false;
	StringUtils::AddToString(bodyUnparsed, requestBodyLines);
	if (bodyUnparsed.size() < this->contentLength)
		return false;
	return true;
}

bool BaseHTTPRequestHandler::parseMultiPartBody(std::string bodyUnparsed)
{
	bool bodyParsed = false;
	std::vector<std::string> multipartDatum = StringUtils::SliceAtSubstring(bodyUnparsed, "--" + this->currentRequestContent->getBoundary() + CRLF);
	if (multipartDatum.size() == 0)
	{
		this->sendError("<h1>Bad Request</h1>", HTTPStatus::BAD_REQUEST);
		return false;
	}
	for (std::vector<std::string>::iterator it = multipartDatum.begin(); it != multipartDatum.end(); it++)
	{
		std::string multipartData = "--" + this->currentRequestContent->getBoundary() + CRLF + *it;
		bodyParsed = this->currentRequestContent->parseMultiPartBody(multipartData, multipartData.size());
		if (!bodyParsed)
		{
			this->sendError("<h1>Bad Request</h1>", HTTPStatus::BAD_REQUEST);
			return false;
		}
		this->multipartDatum.push_back(this->currentRequestContent->getMultiPartData());
		if (it + 1 != multipartDatum.end())
			this->currentRequestContent->resetMultiPartData();
	}
	return bodyParsed;
}

bool BaseHTTPRequestHandler::parseBody(std::string &requestBodyLines)
{
	
	if (!this->shouldContinueParsing(requestBodyLines))
		return false;
	std::string &bodyUnparsed = this->currentRequestContent->getBodyObject().getBodyUnparsed();
	bool bodyParsed = false;
	if (this->currentRequestContent->getHasMultiPartFormData())
	{
		bodyParsed = this->parseMultiPartBody(bodyUnparsed);
	}
	else if (this->currentRequestContent->isChunkedBody())
		bodyParsed = this->currentRequestContent->parseChunkedBody(bodyUnparsed);
	else
		bodyParsed = this->currentRequestContent->parseBody(bodyUnparsed, this->contentLength);
	return bodyParsed;
}

std::vector<std::string> BaseHTTPRequestHandler::SplitRequest(std::string request)
{

	std::istringstream iss(request);
	//Logger::Debug("BaseHTTPRequestHandler::SplitRequest", ERROR , request);
	std::vector<std::string> requestHeaderLines;
	std::string line;
	bool isBodyUnparsedEmpty = this->currentRequestContent->getBodyObject().getBodyUnparsed().empty();

	std::getline(iss, line);
	if (isBodyUnparsedEmpty && !this->isValidFirstRequestHeaderLine(line))
	{
		this->sendError("<h1>Bad Request</h1>", HTTPStatus::BAD_REQUEST);
		throw std::runtime_error("Bad Request");
	}
	if (!isBodyUnparsedEmpty)
		iss.str(request);
	while (isBodyUnparsedEmpty && std::getline(iss, line))
	{
		if (!line.empty() && line[line.size() - 1] == CR)
		{
			if (line.size() == 1)
				break;
			line.erase(line.size() - 1);
		}
		requestHeaderLines.push_back(line);
	}
	this->parseHeaders(requestHeaderLines);
	std::string requestBodyLines((std::istreambuf_iterator<char>(iss)), std::istreambuf_iterator<char>());
	this->parseBody(requestBodyLines);
	return requestHeaderLines;
}

bool BaseHTTPRequestHandler::checkBodyLimit()
{
	std::string contentLength = this->currentRequestContent->getHeader("Content-Length");
	if (contentLength.empty())
		return true;
	ssize_t contentLengthNbr = std::atoll(contentLength.c_str());
	ssize_t maxContentLength = this->currentServerConfig->GetMaxBodySize();
	if (contentLengthNbr < 0 || contentLengthNbr > maxContentLength)
		return false;
	return true;
}

bool BaseHTTPRequestHandler::validateServerName(ServerConfig *serverConfig)
{
	if (serverConfig == NULL)
		serverConfig = this->currentServerConfig;
	std::string host = this->currentRequestContent->getHeader("Host");
	if (host.empty())
		return false;
	std::vector<std::string> hostParts = StringUtils::Split(host, ":");
	if (hostParts.size() < 2)
		return false;
	std::string serverName = hostParts[0];
	std::string port = hostParts[1];
	std::string portServer = serverConfig->GetPort();
	if (portServer != port)
		return false;
	std::vector<std::string> serverNames = serverConfig->GetServerNames();
	bool hasServer = VectorUtils<std::string>::hasElement(serverNames, serverName);
	bool isLocalhost = serverName == "localhost" || serverName == "127.0.0.1";
	bool hasLocalhost = VectorUtils<std::string>::hasElement(serverNames, std::string("localhost")) 
		|| VectorUtils<std::string>::hasElement(serverNames, std::string("127.0.0.1"));
	return hasServer || (isLocalhost && hasLocalhost); 
}

bool BaseHTTPRequestHandler::checkRedirect()
{
	if (this->isCgiRootPath || this->shouldExecuteCgi)
		return false;
	std::string path = this->path;
	LocationConfig *location = this->currentServerConfig->GetLocationConfig(path);
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

void BaseHTTPRequestHandler::addCurrentRequestContentAndServerConfig(int clientSocket, ServerConfig *serverConfig)
{
	this->setClientSockerRequestContentMap(clientSocket, serverConfig);
	this->currentRequestContent = &this->clientSocketRequestContentMap[clientSocket];
	this->currentServerConfig = this->currentRequestContent->getServerConfig();
}

BaseHTTPRequestHandler::RequestMethodFunction BaseHTTPRequestHandler::parseRequestForClientSocket(std::string request, int clientSocket, ServerConfig *serverConfig)
{
	this->addCurrentRequestContentAndServerConfig(clientSocket, serverConfig);
	if (this->currentServerConfig == NULL)
	{
		this->sendError("<h1>Bad Request</h1>", HTTPStatus::BAD_REQUEST);
		return NULL;
	}
	BaseHTTPRequestHandler::RequestMethodFunction method = this->parseRequest(request);
	return method;
}

BaseHTTPRequestHandler::RequestMethodFunction BaseHTTPRequestHandler::parseRequest(std::string request)
{
	try
	{
		std::vector<std::string> firstRequestLine;
		std::vector<std::string> versionNumber;
		StringUtils::AddToString(this->rawRequest, request);
		std::vector<std::string> requestLines = this->SplitRequest(request);
		if (this->currentRequestContent->getHasErrorInRequest())
			return NULL;
		if (requestLines.size() == 0 && this->currentRequestContent->hasParsedAllRequest() == false)
			return NULL;
		if (!this->checkBodyLimit())
		{
			this->sendError("<h1>Request Entity Too Large</h1>", HTTPStatus::CONTENT_TOO_LARGE);
			return NULL;
		}
		if (!this->validateServerName())
		{
			this->sendError("<h1>Bad Request</h1>", HTTPStatus::BAD_REQUEST);
			return NULL;
		}
		this->checkRedirect();
		if (isDirectoryListingAllowed(this->fullResourcePath) && this->requestMethod == "GET")
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
		this->contentNotFound = false;
	}
	else
		this->contentNotFound = true; 
  f.close();
	this->mimeType = this->mimeTypes.ExtensionToType(this->_directoryHandler->getFileExtension(path));
  return content;
}

std::string BaseHTTPRequestHandler::createDirectoryListing(std::string rootPath, std::string path, std::string adder_path)
{
	struct stat sb;

	std::string content("<html><head><title>Index of " + path + "</title></head><body><h1>Index of " + path + "</h1><pre>");
	content += "<table style=\"width: 100%;text-align: justify;\"><thead><tr><th>Name</th><th>Size</th><th>Last Modified</th></tr></thead>";
	content += "<tbody>";

	std::string port = this->currentServerConfig->GetPort();
	std::string fullPath = rootPath + adder_path;
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

bool BaseHTTPRequestHandler::checkExecuteCgi(std::string path)
{
	if (StringUtils::StartsWith(path, CGI_PATH) && path != CGI_PATH)
	{
		std::string fileName = path.substr(std::string(CGI_PATH).size() + 1);
		std::string cgiExtension = this->currentServerConfig->GetCgiExtension();
		bool fileExists = this->_directoryHandler->isInDirectory(fileName, "wwwroot/cgi-bin");
		bool hasCgiExtension = StringUtils::EndsWith(fileName, cgiExtension);
		if (!hasCgiExtension && fileExists)
		{
			this->sendError("<h1>Not Implemented</h1>", HTTPStatus::NOT_IMPLEMENTED);
			return false;
		}
		if (!fileExists)
		{
			this->sendError("<h1>Not Found</h1>", HTTPStatus::NOT_FOUND);
			return false;
		}	
		if (fileExists && hasCgiExtension)
		{
			this->scriptName = fileName;
			this->shouldExecuteCgi = true;
		}
		return this->shouldExecuteCgi;
	}
	return false;
}

bool BaseHTTPRequestHandler::isDirectoryListingAllowed(std::string path)
{
	std::vector<LocationConfig *> locations;
	if (this->shouldExecuteCgi)
		return false;
	locations = this->currentServerConfig->GetLocationConfigs();
	for (std::vector<LocationConfig *>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		std::string locationPath = (*it)->GetPath();
		bool isAutoIndexOn = (*it)->GetAutoIndex();
		if (locationPath == "/" && isAutoIndexOn && path == "/")
			return true;
		size_t countSlashs = StringUtils::CountChar(locationPath, '/');
		size_t findNthSlashOcurrence = StringUtils::FindNthOccurrence(path, '/', countSlashs + 1);
		std::string pathToCompare = path.substr(0, findNthSlashOcurrence);
		if (pathToCompare == locationPath && locationPath != path && isAutoIndexOn)
		{
			std::string directory = (*it)->GetRootPath();
			path.replace(0, locationPath.size(), "");
			std::string fullPath = directory + path;
			bool isDirectory = this->_directoryHandler->isValidDirectory(fullPath);
			if (path[path.size() -1] == '/')
					path = path.substr(0, path.size() - 1);
			if (isDirectory)
			{
				directory += path;
				this->pathAdder = path;
				this->path = locationPath;
				return true;
			}
			bool isFile = this->_directoryHandler->isValidFile(fullPath);
			if (isFile)
			{
				this->allowDirectoryListing = true;
				this->directoryListingPath = fullPath;
				return this->allowDirectoryListing;

			}
			this->sendError("<h1>Not Found</h1>", HTTPStatus::NOT_FOUND);
			return false;
		}
	}
	return false;
}

std::string BaseHTTPRequestHandler::getContent(const std::string path)
{
	std::string content("");
	//TODO: create cgi class
	if (this->shouldExecuteCgi)
	{
		std::string binaryPath = this->currentServerConfig->GetCgiBinaryPath();
		CGIRequestHandler cgiHandler(this->currentRequestContent, this->scriptName, binaryPath);
		cgiHandler.execute();
		return cgiHandler.response();
	}
	if (this->isCgiRootPath)
		return this->createDirectoryListing("wwwroot", path, path);
	LocationConfig *location = this->currentServerConfig->GetLocationConfig(path);
	if (this->allowDirectoryListing)
	{
		this->allowDirectoryListing = false;
		return this->readContent(this->directoryListingPath);
	}
	if (location == NULL)
		return content;
	if ((location->GetIndexFileNotFound() && location->GetAutoIndex()))
		return this->createDirectoryListing(location->GetRootPath(), path + this->pathAdder, this->pathAdder);
	if (!this->fileName.empty())
		return this->getContentByFileName(this->fileName);
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
	if (api.size() < 3 || api[1] != "delete" || !StringUtils::EndsWith(api[2], ".json"))
		return methodsAllowed;
	LocationConfig *location = this->currentServerConfig->GetLocationConfig("/delete");
	if (location)
		return location->GetAllowedMethods();
	methodsAllowed.push_back("DELETE");
	return methodsAllowed;
}

std::vector<std::string> BaseHTTPRequestHandler::getMethodsAllowed() const {
	std::vector<std::string> methodsAllowed;
	if (this->isCgiRootPath)
	{
		methodsAllowed.push_back("GET");
		return methodsAllowed;
	}
	if (this->shouldExecuteCgi)
	{
		methodsAllowed.push_back("GET");
		methodsAllowed.push_back("POST");
		return methodsAllowed;
	}
	methodsAllowed = this->getMethodsAllowedForApi();
	if (methodsAllowed.size() > 0)
		return methodsAllowed;
	LocationConfig *location = this->currentServerConfig->GetLocationConfig(this->path);
	if (location == NULL)
		return methodsAllowed;
	return location->GetAllowedMethods();
}

void BaseHTTPRequestHandler::writeDefaultResponse(std::string content, std::string content_type) 
{
	if (content_type.empty())
		content_type = this->mimeType;
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

void BaseHTTPRequestHandler::setClientSockerRequestContentMap(int clientSocket, ServerConfig *serverConfig)
{
	std::pair<const int, RequestContent> *foundRequest = MapUtils<int, RequestContent>::SafeFindMap(this->clientSocketRequestContentMap, clientSocket);
	if (foundRequest)
	{
		if (foundRequest->second.getServerConfig()->GetServerId() == serverConfig->GetServerId())
			return;
	}
	this->clientSocketRequestContentMap[clientSocket] = RequestContent(serverConfig);
}

std::string BaseHTTPRequestHandler::generateRandomString(int length) {
    const std::string charset =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::srand(static_cast<unsigned int>(std::time(NULL)));

    std::string randomString;
    for (int i = 0; i < length; ++i) {
        int randomIndex = std::rand() % charset.length();
        randomString += charset[randomIndex];
    }
    return randomString;
}


RequestContent *BaseHTTPRequestHandler::getCurrentRequestContent()
{
	return this->currentRequestContent;
}

ServerConfig *BaseHTTPRequestHandler::getCurrentServerConfig()
{
	return this->currentServerConfig;
}

std::string BaseHTTPRequestHandler::getPath(std::string path)
{
	this->fullResourcePath = path;
	if (path[path.size() -1] == '/' && path.size() > 1)
		path = path.substr(0, path.size() - 1);
	std::vector<std::string> pathSplit = StringUtils::Split(path, "/");
	size_t posQueryString = path.find("?");
	if (posQueryString != std::string::npos)
	{
		this->currentRequestContent->setQueryString(path.substr(posQueryString + 1));
		path = path.substr(0, posQueryString);
	}
	if (pathSplit.size() == 0)
		return path;
	if (path == CGI_PATH)
	{
		this->isCgiRootPath = true;
		return path;
	}
	if (this->checkExecuteCgi(path))
		return path;
	std::string last = pathSplit[pathSplit.size() - 1];
	size_t posDot = last.find_last_of(".");
	if (pathSplit.size() > 1 && pathSplit[1] == "delete")
		return path;
	if (posDot != std::string::npos && posDot != last.size() - 1)
	{
		this->fileName = last;
		size_t lastSlashPos = path.find_last_of("/");
		if (lastSlashPos == 0)
			return "/";
		return path.substr(0, path.find_last_of("/"));
	}
	return path;
}

std::string BaseHTTPRequestHandler::getContentByFileName(const std::string fileName)
{
	std::string content("");
	std::string path = this->path;
	bool addSlash = path[path.size() - 1] != '/';
	LocationConfig *location = this->currentServerConfig->GetLocationConfig(path);
	if (location == NULL)
		return content;
	std::string fullPath = location->GetRootPath() + path + (addSlash ? "/" : "") + fileName;
	content = this->readContent(fullPath);
	if (content.empty())
	{
		this->sendNotFoundError();
		return "";
	}
	return content;
}

std::vector<MultiPartData> BaseHTTPRequestHandler::getMultiPartData()
{
	return this->multipartDatum;
}