#ifndef BASE_HTT_PREQUES_THANDLER_HPP
#define BASE_HTT_PREQUES_THANDLER_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <vector>
#include<map>
#include <iterator> 
#include <stdlib.h>
#include <stdexcept>
#include <dirent.h>
#include <sys/stat.h>
#include <cstdlib>
#include <ctime>
#include "StringUtils.hpp"
#include "HTTPStatus.hpp"
#include "ServerConfig.hpp"
#include "Headers.hpp"
#include "DirectoryHandler.hpp"
#include "RequestContent.hpp"

class BaseHTTPRequestHandler {
    public:
		BaseHTTPRequestHandler(ADirectoryHandler *directoryHandler);
		virtual ~BaseHTTPRequestHandler();
		BaseHTTPRequestHandler(const BaseHTTPRequestHandler& other);
		BaseHTTPRequestHandler& operator=(const BaseHTTPRequestHandler& other);

		typedef void (BaseHTTPRequestHandler::*RequestMethodFunction)(void);
		
		const std::string headersBufferToString() const;
		RequestMethodFunction getMethod(const std::string& method);
		RequestMethodFunction parseRequestForClientSocket(std::string request, int clientSocket, ServerConfig *serverConfig);
		void clearHeadersBuffers();
		bool getContentNotFound() const;
		void setClientSockerRequestContentMap(int clientSocket, ServerConfig *serverConfig);
		bool shouldClearRequestContent(int clientSocket);
		bool shouldContinueParsing(std::string requestBodyLines);
		virtual void doGET() = 0;
		virtual void doPOST() = 0;
		virtual void doDELETE() = 0;
		virtual void clearRequestContent(int clientSocket) = 0;
		bool hasParsedAllRequestContent();
		template <typename T>
		void sendHeader(std::string key, T value);
		RequestMethodFunction parseRequest(std::string request);
		RequestContent *getCurrentRequestContent();

	protected:
		void sendResponse(int statusCode, std::string message);
		void endHeaders();
		void sendError(const std::string& content, const StatusCode& status);
		void sendNotFoundError();
		void writeContent(const std::string content);
		void setRequestLines(const std::vector<std::string> requestLines);
		bool isValidFirstRequestHeaderLine(std::string requestHeaderLine);
		std::vector<std::string> getMethodsAllowed() const;
		std::string readContent(const std::string path);
		std::string getContent(const std::string path);
		std::string getContentByFileName(const std::string fileName);
	  std::string GetPath() const;
		std::vector<std::string> getMethodsAllowedForApi() const;
		void parseHeaders(std::vector<std::string> &headers);
		bool parseBody(std::string &bodyLines);
		std::vector<std::string> SplitRequest(std::string request);
		bool checkBodyLimit();
		std::string createDirectoryListing(std::string rootPath, std::string path);
		bool isDirectoryListingAllowed(std::string path);
		bool validateServerName();
		bool checkRedirect();
		void writeDefaultResponse(std::string content, std::string mimeType = "text/html");
		void addCurrentRequestContentAndServerConfig(int clientSocket, ServerConfig *serverConfig);
		std::string generateRandomString(int length);
		ServerConfig *getCurrentServerConfig();
		std::string getPath(std::string path);
		bool checkExecuteCgi(std::string path);
		
		size_t contentLength;
		ADirectoryHandler *_directoryHandler;
		std::map<int, RequestContent> clientSocketRequestContentMap;
    
		private:
		std::ostringstream headersBuffer;
		std::string requestMethod;
		std::string path;
		std::string requestVersion;
		std::string directoryListingPath;
		bool allowDirectoryListing;
		bool contentNotFound;
		bool isCgiRootPath;
		bool shouldExecuteCgi;
		std::string fileName;
		std::string fullResourcePath;
		ServerConfig *currentServerConfig;
		RequestContent *currentRequestContent;
};

template <typename T>
void BaseHTTPRequestHandler::sendHeader(std::string key, T value) {
	this->headersBuffer <<  key << ": " << value << CRLF;
}
#endif
